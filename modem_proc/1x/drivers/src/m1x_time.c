
/*=============================================================================

                   T I M E   S E R V I C E   S U B S Y S T E M

                       C D M A   T I M E   K E E P I N G

GENERAL DESCRIPTION
  Implements cdma time-keeping functions using the symbol combiner.


EXTERNALIZED FUNCTIONS
  m1x_time_set_valid
    Indicates whether or not CDMA time is available

  m1x_time_set
    Set the system timestamp value in 80 ms units.

  m1x_time_tick
    Increment the timestamp by a PN roll.

  m1x_time_inc
    Increment the 64-bit system timestamp in 26.6 ms units

  m1x_time_get
    Return the system timestamp value.

  m1x_time_get_slot_time
    Return the 64-bit system timestamp in units of slot time.

  m1x_time_get_20ms_frame_time
    Return the 64-bit system timestamp in units of 20 millisecond frame time.

  m1x_time_get_20ms_frame_offset_time
    Return the 64-bit system timestamp in units of 20 millisecond frame time,
    adjusted for a frame offset.

  m1x_time_set_20ms_frame_offset
    Set the 20ms frame offset


INITIALIZATION AND SEQUENCING REQUIREMENTS
  m1x_time_set() needs to be called when accurate time is available from the
  base station.


Copyright (c) 2012 - 2020 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

=============================================================================*/

/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/drivers/src/m1x_time.c#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
07/22/16   bb      More Changes to provideaccurate CDMA snapshot for LTE-D
07/17/16   bb/ab   Changes to provide CDMA snapshot for LTE-D
04/17/14   cjb     Clean-up related to using 1x MSG macros for printing F3s
05/30/13   pap     Added changes to update time services in case of 1x going
                   out of service.
03/27/13   as      Added m1x_timetick_diff method.
02/22/13   ssh     Added Support to return the non-1X time when CDMA time is
                   not availabe (e.g., CSFB sceanrios).
01/23/13   as      Updated sleep profiling to use XO clock and 64 bit time ticks
10/30/12   cjb     Fixed conversion from TICKs to SCLKs.
10/18/12   srk     Use m1x time debug F3s for unnecessary F3s.
09/19/12   trc     Added m1x_time_init() API.
08/14/12   cjb     Added changes to use SLPC(new sleepctl replacement module).
04/04/12   trc     Initial version, ported from time_cdma.c

=============================================================================*/



/*=============================================================================

                           INCLUDE FILES

=============================================================================*/
/* Common */
#include "1x_variation.h"
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"
#include "qw.h"

/* 1x */
#include "m1x_time_i.h"
#include "srch.h"

/* Others */
#include "DalDevice.h"
#include "DDITimetick.h"
#include "m1x_diag.h"
#include "m1x_stmr_i.h"
#include "srch_comb.h"
#include "vstmr.h"
#include "vstmr_1x.h"
#include "rex.h"
#include "timetick.h"
#include "time_genoff.h"
#include "amssassert.h"
#include "srch_comb.h"
/*=============================================================================

                           DATA DEFINITIONS

=============================================================================*/

/*-----------------------------------------------------------------------------
      Constants & Macros
-----------------------------------------------------------------------------*/
#define M1XTIME_DBG_MSG( ... ) \
     M1X_MSG( MUX, DBG_M1XTIME, __VA_ARGS__ )

#define M1XTIME_DBG_STR( ... ) \
     M1X_STR( MUX, DBG_M1XTIME, __VA_ARGS__ )

/*-----------------------------------------------------------------------------
  Constants for converting to the qword timestamp format
-----------------------------------------------------------------------------*/

/* Number of 1.25 millisecond ticks in 80 milliseconds (64) or 2^6
*/
#define M1X_TIME_80_MS_TICKS_SHIFT 6
#define M1X_TIME_80_MS_TICKS (1<<M1X_TIME_80_MS_TICKS_SHIFT)

/* Number of bits to right shift time to get the milliseconds
** portion of the timestamp
*/
#define M1X_TIME_MS_SHIFT    16

/* 80 milliseconds is 64 * 1.25 milliseconds, we then shift this up
** by 16 bits to get into the 1.25 ms counter part of the timestamp
*/
#define M1X_TIME_80_MS       ((dword) M1X_TIME_80_MS_TICKS << 16L)
#define M1X_TIME_40_MS       (M1X_TIME_80_MS >> 1)

#define M1X_TIME_80MS_SHIFT  (M1X_TIME_MS_SHIFT+M1X_TIME_80_MS_TICKS_SHIFT)


#define M1X_TIME_SUB80MS_MSK ((1<<M1X_TIME_80MS_SHIFT)-1)

/* Max slot cycle length
*/
#define M1X_TIME_MAX_SLOT    2048

/* Take the  2-chip counter and get phase from it by shifting it down. */
#define M1X_TIME_PHASE_SHIFT 14


/* Convert 2-chip units to timestamp LSBs */
#define M1X_TIME_TS_FROM_2CHIPS( cnt2 ) \
          (((dword)((cnt2) / 768) << 16) + ((dword)((cnt2) % 768) * 64))

/*-----------------------------------------------------------------------------
  CDMA Time static data
-----------------------------------------------------------------------------*/

typedef struct
{
  /* Timestamp at start of sleep */
  time_type                       timestamp;

  /* Slow clock count at the start of CDMA sleep */
  timetick_type                   sclk;
}
m1x_time_sleep_struct_type;

typedef struct
{
  /* XO time in Cx */
  uint64              xo_time_cx;

  /* CDMA system Time in Cx */
  uint64              sys_time_cx;
}
m1x_time_cdma_xo_time_struct_type;


typedef struct
{
  /* Timestamp at last 80ms roll */
  time_type                       t80ms;

  /* 80ms phase (0=0-26ms, 1=27-53ms, 2=54-79ms) */
  uint16                          phase80;

  /* CDMA time valid flag */
  boolean                         valid;

  /* Timetick-based dead-reckoning valid flag */
  boolean                         dead_reckoning_valid;

  /* Frame offset */
  uint8                           frame_offset;

  /* Timestamp and slow clock count at the start of sleep */
  m1x_time_sleep_struct_type     sleep;
  
  /* CDMA System time and XO time */
  m1x_time_cdma_xo_time_struct_type  sys_time;
}
m1x_time_struct_type;


static m1x_time_struct_type      m1x_time =
{
  { 0, 0 },         /* January 6, 1980, 00:00:00 */
  0,                /* Phase 0, for lack of any other phase value */
  FALSE,            /* CDMA time is _NOT_ valid */
  FALSE,            /* Dead-reckoning is _NOT_ valid */
  0,                /* No frame offset */
  { { 0, 0 }, 0 },  /* Sleep data */
  { 0, 0 }, 
};


/*--------------------------------------------------------------------------
                     Critical Section Support
--------------------------------------------------------------------------*/

static boolean m1x_time_crit_sect_init = FALSE;
static rex_crit_sect_type m1x_time_crit_sect;

static inline void m1x_time_enter_crit_sect( void )
{
  /* Init the critical section if it hasn't already been done */
  if ( !m1x_time_crit_sect_init )
  {
    rex_init_crit_sect(&m1x_time_crit_sect);
    m1x_time_crit_sect_init = TRUE;
    /* There can be a race condition if this function is called by
       two tasks simultaneous prior to the critical section being
       initialized.  To prevent this, there should be a guaranteed-early
       single call that initializes the critical section.  Historically,
       this would be done during the 1x SRCH task startup, since SRCH is
       likely the first 'user' of m1x_time services. */
  }

  /* Enter the critical section here */
  rex_enter_crit_sect( &m1x_time_crit_sect );
}

static inline void m1x_time_leave_crit_sect( void )
{
  /* Leave the critical section here */
  rex_leave_crit_sect( &m1x_time_crit_sect );
}

/*=============================================================================

                           FUNCTION DEFINITIONS

=============================================================================*/


/*=============================================================================

FUNCTION M1X_TIME_SET_GENOFF

DESCRIPTION
  Synchronize 1x time to time genoff, if valid

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void m1x_time_set_genoff( void )
{
  time_genoff_args_struct_type time_genoff_args;
  time_type ts_val;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* If 1x time is valid, push it outward */
  if ( m1x_time_get( ts_val ) == M1X_TIME_CDMA_SOURCE )
  {
    /* Source is 1x, in timestamp format, 'set' operation */
    time_genoff_args.base = ATS_1X;
    time_genoff_args.ts_val = (void *)ts_val;
    time_genoff_args.unit = TIME_STAMP;
    time_genoff_args.operation = T_SET;
    time_genoff_opr( &time_genoff_args );

    M1XTIME_DBG_MSG(
      "1x system time synchronized to TIME::ATS_1X -> 0x%08x%08x",
      qw_hi(ts_val),
      qw_lo(ts_val) );
  }

} /* m1x_time_set_genoff */

/*=============================================================================

FUNCTION M1X_TIME_REPORT_OUT_COVERAGE

DESCRIPTION
  Send a message to time genoff that 1x is moving out of coverage.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void m1x_time_report_out_coverage( void )
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  time_genoff_report_out_of_coverage(ATS_1X);
  M1XTIME_DBG_MSG("1x report out of coverage to TIME::ATS_1X");

}/*m1x_time_report_out_coverage*/

/*=============================================================================

FUNCTION M1X_TIME_ADVANCE_TIMESTAMP_BY_SCLK

DESCRIPTION
  Advances timestamp by the number of slow clocks.  Slow clock is assumed
  to be nominal XO / 586 = 19.2MHz / 586 for simplicity.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
static void m1x_time_advance_timestamp_by_sclk
(
  time_type       timestamp,
  uint64          sclks_elapsed
)
{
  uint64 cx32_elapsed;
  uint64 base_1p25ms_portion;
  uint16 base_cx32_portion;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Extract the base 1.25ms units and cx32 remainder from timestamp */
  base_1p25ms_portion = (((uint64)qw_hi( timestamp ) << 32) |
                         (uint64)qw_lo( timestamp ))
                        >> 16;
  base_cx32_portion = (uint16)qw_lo( timestamp );

  /* Compute the sclk time elapsed in cx32 (assumes sclk = 19.2MHz/586) */
  cx32_elapsed = ( sclks_elapsed * 2048ULL * 586ULL ) / 1000ULL;

  /* Add in the cx32 portion of the timestamp to the elapsed cx32 amount */
  cx32_elapsed += (uint64)base_cx32_portion;

  /* Get the new cx32 remainder (remainder after 1.25ms unit granularity) */
  base_cx32_portion = (uint16)( cx32_elapsed % 0xC000ULL );

  /* Add the 1.25ms portion of the cx32 elapsed to base 1.25ms quantity */
  base_1p25ms_portion += cx32_elapsed / 0xC000ULL;

  /* Reformat it into timestamp format (upper 48 = 1.25ms, lower 16 = cx32 */
  base_1p25ms_portion <<= 16;
  base_1p25ms_portion |= (uint64)base_cx32_portion;

  /* Update the timestamp passed in */
  qw_set( timestamp, base_1p25ms_portion >> 32, (uint32)base_1p25ms_portion );

} /* m1x_time_advance_timestamp_by_sclk */


/*=============================================================================

FUNCTION M1X_TIME_INIT

DESCRIPTION
  Initialize 1x timekeeping subsystem.  Should be called at bootup, once only.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void m1x_time_init( void )
{
  /* Get the critical section.  This also does double duty in initializing
     the m1x_time critical section, since this function should be called
     once very early and long before anyone else calls m1x_time APIs. */
  m1x_time_enter_crit_sect();

  /* Could do other initialization steps here if needed */

  /* Release the critical section - m1x_time should be operational at this
     point, although it won't have valid time until 1x protocol initially
     seeds it. */
  m1x_time_leave_crit_sect();
}

/*=============================================================================

FUNCTION M1X_TIME_SET_VALID

DESCRIPTION
  Permits other code in the phone to tell M1X_TIME that the contents of the
  demodulator are not meaningful. This causes TIME to fall back on an
  alternate time source, or as a last resort, "Time Of Day".

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void m1x_time_set_valid
(
  /* Flag indicating whether CDMA time is valid or not */
  boolean                         valid
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Store the flag */
  m1x_time.valid = valid;

  /* If CDMA time is now "valid" ... */
  if ( valid )
  {
    /* Sync time genoff to CDMA time */
    m1x_time_set_genoff();
  }
  else
  {
    /* Report out of coverage */
    m1x_time_report_out_coverage();
  }

} /* m1x_time_set_valid */

/*=============================================================================

FUNCTION M1X_TIME_SET

DESCRIPTION
  This procedure sets the system timestamp to a specified value which is
  the combination of a multiple of 80 milliseconds and a 26.67 ms phase
  of 80 milliseconds (0, 1, 2).

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Initializes the system timestamp and marks CDMA time as valid.

=============================================================================*/

void m1x_time_set
(
  /* Number of elapsed 80 ms units since base date. */
  time_type                       set_val,

  /* The current phase of the 80 ms. */
  uint16                          phase80
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


  /* Critical section entry */
  m1x_time_enter_crit_sect();

  /* Set timestamp count to equivalent number of 1.25 ms units
     and shift value into correct position. */

  qw_equ( m1x_time.t80ms, set_val);
  qw_shift( m1x_time.t80ms, M1X_TIME_80MS_SHIFT );


  /* Record current phase of 80ms counter. */

  m1x_time.phase80 = phase80;


  /* Mark CDMA time as now valid */

  m1x_time_set_valid( TRUE );


  /* Critical section exit */
  m1x_time_leave_crit_sect();

} /* m1x_time_set */


/*=============================================================================

FUNCTION M1X_TIME_GET

DESCRIPTION
  This procedure is called to read the 64-bit CDMA system timestamp.

DEPENDENCIES
  m1x_time_set() needs to be called when accurate time is available from
  the base station.  This function does not fall back on "Time Of Day".

RETURN VALUE
  TRUE if CDMA system time is available, FALSE otherwise.
  Parameter ts_val is set to the current system time, in timestamp format.

SIDE EFFECTS
  None

=============================================================================*/

m1x_time_source_type  m1x_time_get
(
  /* Address of qword in which to return 64-bit system timestamp. */
  time_type                       ts_val
)
{
  /* Flag indicating if CDMA time is valid */
  m1x_time_source_type            source = M1X_TIME_INVALID;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Critical section entry */
  m1x_time_enter_crit_sect();

  /* Use CDMA source if available */
  if ( m1x_time.valid )
  {
    /* Virtual symbol combiner position */
    word                            cnt2;

    /* Set the source */
    source = M1X_TIME_CDMA_SOURCE;

    /* Get timestamp accurate in 1.25ms units */
    qw_equ( ts_val, m1x_time.t80ms );


    /* Read the value of the symbol combiner. Since SEARCH is horsing around
       with it sometimes in order to save power and increase standby, the
       logic of this required TIME to know too much about what SEARCH is doing.
       Therefore, SEARCH provides a virtualized interface to the symbol
       combiner, and it will automatically compensate for any possible
       error induced in the symbol combiner value, and return what it ought
       to be. */
    cnt2 = srch_scomb_get_vsc_time();

    /* Add in 2 chip counter. Add the count of 1.25 ms into the high 48 bits,
       and the count of chip*32's into the low 16 bits.  Note:  768 2-chip
       counts is 1.25 ms, and 64 chip/32 is 2 chips. */
    qw_inc( ts_val, M1X_TIME_TS_FROM_2CHIPS( cnt2 ));
  }
  else if ( m1x_time.dead_reckoning_valid )
  {
    /* Otherwise, use dead-reckoning value */
    source = M1X_TIME_DEAD_RECKONING_SOURCE;

    /* Get the base timestamp before dead reckoning */
    qw_equ( ts_val, m1x_time.sleep.timestamp );

    /* Advanced the base timestamp by the sclks elapsed since base timestamp */
    m1x_time_advance_timestamp_by_sclk( ts_val,
                                        timetick_get() - m1x_time.sleep.sclk );
  }
  else
  {
    /* We could fall back to non-1x system time here, but that might be
       overstepping 1x knowledge.  Instead, just take the timetick64 value and
       extrapolate it.  Note that we'll still be returning M1X_TIME_INVALID,
       so the client will know not to trust it. */
    source = M1X_TIME_INVALID;
    qw_set( ts_val, 0, 0 );
    m1x_time_advance_timestamp_by_sclk( ts_val, timetick_get_sclk64() ) ;
  }

  /* Critical section exit */
  m1x_time_leave_crit_sect();

  return source;

} /* m1x_time_get */


/*=============================================================================

FUNCTION M1X_TIME_TICK

DESCRIPTION
  This function is called at every PN Roll (26.67 ms).  It increments the
  timestamp by 80 ms each time phase80 is 0.  Phase80 being 0 indicates that
  80 ms have passed.

DEPENDENCIES
  The CDMA system clock must be set to a reasonable value in order for
  this function to perform a meaningful operation on the timestamp.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void m1x_time_tick
(
  /* Specifies the current phase of the 80 ms. */
  uint16                          phase80
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Critical section entry */
  m1x_time_enter_crit_sect();

  /* Determine if phase80 value has rolled */

  if ( phase80 < m1x_time.phase80 )
  {
    /* phase80 value has 'rolled' */

    /* Increase coarse time by 80ms */
    qw_inc( m1x_time.t80ms, M1X_TIME_80_MS );
  }

  /* Store new phase */
  m1x_time.phase80 = phase80;

  /* Critical section exit */
  m1x_time_leave_crit_sect();

} /* m1x_time_tick */


/*=============================================================================

FUNCTION M1X_TIME_SLEEP

DESCRIPTION
  Invalidates CDMA system time, in preperation for CDMA slotted sleep.

DEPENDENCIES
  The CDMA system clock must accurate when this function is called.

RETURN VALUE
  None

SIDE EFFECTS
  Updates the slow-clock based "Time of Day" before invalidating CDMA time.

=============================================================================*/

void m1x_time_sleep( void )
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Sync time genoff to CDMA time */
  m1x_time_set_genoff();

  /* Critical section entry */
  m1x_time_enter_crit_sect();

  /* Capture the start of sleep timestamp & slow-clock counter values.
     These will be used to update the CDMA timestamp at wakeup and
     provide dead-reckoning time whilst asleep */
  (void) m1x_time_get( m1x_time.sleep.timestamp );
  m1x_time.sleep.sclk = timetick_get();

  /* Dead reckoning is now possible, using the above */
  m1x_time.dead_reckoning_valid = TRUE;

  /* Critical section exit */
  m1x_time_leave_crit_sect();

  /* Invalidate CDMA system time. */
  m1x_time.valid = FALSE;

} /* m1x_time_sleep */


/*=============================================================================

FUNCTION M1X_TIME_WAKE

DESCRIPTION
  Updates and asserts valid CDMA system time, after CDMA slotted sleep.

DEPENDENCIES
  The symbol combiner timing must be correct when this function is called.

RETURN VALUE
  None

SIDE EFFECTS
  None.

=============================================================================*/

void m1x_time_wake( void )
{
  /* Slow clock count at wakeup */
  timetick_type                   sclk;

  /* Symbol combiner position, in 2-chip units */
  uint32                          sc_pos;

  /* Timestamp at most-recent 80ms boundary */
  time_type                       t80ms;

  /* Slow-clock based timestamp at end of sleep */
  time_type                       t_wake_sclk;

  /* Symbol-combiner based timestamp at end of sleep */
  time_type                       t_wake_sc;


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Critical section entry */
  m1x_time_enter_crit_sect();

  /* Get the slow clock count at the wakeup point */
  sclk = timetick_get();

  /* Get the symbol combiner position at the wakeup point as well */
  sc_pos = srch_scomb_get_vsc_time();

  /* Compute slow-clock based end-of-sleep timestamp */
  qw_equ( t_wake_sclk, m1x_time.sleep.timestamp );
  m1x_time_advance_timestamp_by_sclk( t_wake_sclk,
                                      sclk - m1x_time.sleep.sclk );

  /* Round down to nearest 80ms boundary */
  qw_equ( t80ms, t_wake_sclk );
  qw_dec( t80ms, qw_lo( t80ms ) & M1X_TIME_SUB80MS_MSK );

  /* Compute symbol-combiner based end-of-sleep timestamp, and 80ms boundary
     computed above. */
  qw_equ( t_wake_sc, t80ms );
  qw_inc( t_wake_sc, M1X_TIME_TS_FROM_2CHIPS( sc_pos ) );

  /* If symbol combiner based timestamp differs from the slow clock based
     timestamp by more the 40ms, the computed 80ms timestamp is off by 80ms */
  qw_dec( t_wake_sc, M1X_TIME_40_MS );
  if (qw_cmp( t_wake_sclk, t_wake_sc) < 0)
  {
    /* t_wake_sclk < t_wake_sc - 40, therefore t80ms is 80ms too big */
    qw_dec( t80ms, M1X_TIME_80_MS);
  }
  else
  {
    qw_inc( t_wake_sc, M1X_TIME_80_MS ); /* -40 + 80 = +40 */
    if (qw_cmp( t_wake_sclk, t_wake_sc) > 0 )
    {
      /* t_wake_sclk > t_wake_sc + 40, therefore t80ms is 80ms too small */
      qw_inc( t80ms, M1X_TIME_80_MS);
    }
  }

  /* Set CDMA system time */
  qw_equ( m1x_time.t80ms, t80ms);
  m1x_time.phase80 = sc_pos >> M1X_TIME_PHASE_SHIFT;

  /* Mark CDMA system time as valid */
  m1x_time_set_valid( TRUE );

  /* Critical section exit */
  m1x_time_leave_crit_sect();

} /* m1x_time_wake */



/*=============================================================================

FUNCTION M1X_TIME_GET_SLOT_TIME

DESCRIPTION
  This procedure is returns the 64-bit system timestamp in units of
  slot time, 26.6 millisecond units, modulo the maximum slot cycle
  length.

DEPENDENCIES
  The value returned by this function is meaningless unless the demodulator
  is running and contains a valid value.  This function does not fall back on
  "Time Of Day".

RETURN VALUE
  The current timestamp value, as specified above.

SIDE EFFECTS
  None

=============================================================================*/

uint16 m1x_time_get_slot_time( void )
{
  /* Current CDMA time-stamp */
  time_type                       ts_val;

  /* Virtual symbol combiner position */
  uint16                          cnt2;

  /* Virtual symbol combiner position */
  uint16                          phase80;

  /* Resulting slot time */
  uint16                          slot_time;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Critical section entry */
  m1x_time_enter_crit_sect();

  /* Get timestamp accurate in 1.25ms units */
  qw_equ( ts_val, m1x_time.t80ms );

  /* Get the demodulator value from SEARCH, so that it can compensate for
  ** any funny business it's been pulling with the combiner value.
  */
  cnt2 = srch_scomb_get_vsc_time();

  /* Determine the phase of the symbol combiner */
  phase80 = cnt2 >> M1X_TIME_PHASE_SHIFT;

  if ( phase80 < m1x_time.phase80 )
  {
    /* Phase has wrapped around - increase coarse 80ms count */

    qw_inc( ts_val, M1X_TIME_80_MS );
  }

  /* Critical section exit */
  m1x_time_leave_crit_sect();

  /* Get ts_val in 80ms units (in the 1.25 ms tick position) */
  (void) qw_div_by_power_of_2( ts_val, ts_val, M1X_TIME_80_MS_TICKS_SHIFT );

  /* Take the low dword, modulo Max slot cycle length * 2, and multiply it
     by 3 to get PN Rolls. (M1X_TIME_MAX_SLOT is a power of 2) */
  slot_time = (uint16) (qw_lo( ts_val ) >> M1X_TIME_MS_SHIFT);
  slot_time = (slot_time & (M1X_TIME_MAX_SLOT*2 - 1) ) * 3;

  /* Add in the phase from the symbol combiner (0, 1, or 2 extra PN Rolls). */
  slot_time = slot_time + phase80;

  return slot_time;

} /* m1x_time_get_slot_time */


/*=============================================================================

FUNCTION M1X_TIME_GET_20MS_FRAME_TIME

DESCRIPTION
  This procedure is returns the 64-bit system timestamp in units of
  20 millisecond frame time (traffic / paging / access channel frame
  time).

DEPENDENCIES
  The value returned by this function is meaningless unless the demodulator
  is running and contains a valid value.
  This function DOES fall back on "Time Of Day", if CDMA time is not available

RETURN VALUE
  The current timestamp value, as specified above.

SIDE EFFECTS
  None

=============================================================================*/

void m1x_time_get_20ms_frame_time
(
  /* Address of qword in which to return 64-bit frame time. */
  time_type                       ts_val
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get timestamp accurate in 1.25ms units
  */
  (void) m1x_time_get( ts_val );

  /* Divide by 64*1024 to truncate the low 16 bits of the time stamp,
  ** then divide the 1.25 ms units by 16 to get 20ms units.
  */
  /* A divide by 16*(64*1024) = 2^20. So, use a bitshift for speed */

  (void) qw_div_by_power_of_2( ts_val, ts_val, 20 );

} /* m1x_time_get_20ms_frame_time */


/*=============================================================================

FUNCTION M1X_TIME_SET_FRAME_OFFSET

DESCRIPTION
   Set the frame offset, so calls to m1x_time_get_20ms_frame_offset_time( )
   are adjusted by the frame offset.

DEPENDENCIES
  The CDMA system clock must be set to a reasonable value in order for
  this function to perform a meaningful operation on the timestamp.

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void m1x_time_set_frame_offset
(
  /* New 20ms frame offset */
  uint8                           frame_offset
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  m1x_time.frame_offset = frame_offset;

} /* m1x_time_set_frame_offset */


/*=============================================================================

FUNCTION M1X_TIME_GET_20MS_FRAME_OFFSET_TIME

DESCRIPTION
  This procedure returns the 64-bit system timestamp (in units of
  20 millisecond frame time) adjusted by frame offset.

DEPENDENCIES
  The value returned by this function is meaningless unless the demodulator
  is running and contains a valid value.  This function does not fall back on
  "Time Of Day".

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void m1x_time_get_20ms_frame_offset_time
(
  /* Address of qword in which to return 64-bit frame time. */
  time_type                       ts_offset_val
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get timestamp accurate in 1.25ms units
  */
  (void) m1x_time_get( ts_offset_val );

  /* Divide by 64*1024 to truncate the low 16 bits of the time stamp,
  */
  (void) qw_div_by_power_of_2( ts_offset_val, ts_offset_val, 16 );

  /* Subtract frame_offset from the resulting time in 1.25 ms units
  */
  qw_dec( ts_offset_val, (dword) m1x_time.frame_offset );

  /* then divide the 1.25 ms units by 16 to get 20ms units.
  */
  (void) qw_div_by_power_of_2( ts_offset_val, ts_offset_val, 4 );

} /* m1x_time_get_20ms_frame_offset_time */



/*=============================================================================

FUNCTION M1X_TIME_GET_MS

DESCRIPTION
  Get the system time, in # of milliseconds since "the beginning of time".

DEPENDENCIES
  A valid CDMA time, or a valid HDR time, or a valid Time-of-Day, etc.

RETURN VALUE
  Milliseconds since 6 Jan 1980 00:00:00 returned to the time parameter

SIDE EFFECTS
  None

=============================================================================*/

void m1x_time_get_ms
(
  /* Out: Time in milliseconds since 6 Jan 1980 00:00:00 */
  time_type                       ts_val
)
{
  uint64 base_1p25ms_portion;
  uint16 base_cx32_portion;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Read the current time of day */
  (void) m1x_time_get( ts_val );

  /* Extract the base 1.25ms units and cx32 remainder from timestamp */
  base_1p25ms_portion = (((uint64)qw_hi( ts_val ) << 32) |
                         (uint64)qw_lo( ts_val ))
                        >> 16;
  base_cx32_portion = (uint16)qw_lo( ts_val );

  /* Convert 1.25ms units into 0.25ms units */
  base_1p25ms_portion *= 5ULL;

  /* Convert cx32 to 0.25ms units, slightly rounded down (+3 cx32) */
  base_cx32_portion /= (49152 + 3) / 5;

  /* Add the two 0.25ms quantities and divide by 4 to get 1ms units */
  base_1p25ms_portion = ( base_1p25ms_portion + (uint64)base_cx32_portion ) / 4;

  /* Push the 1ms value back into qw units */
  qw_set( ts_val,
          (uint32)(base_1p25ms_portion >> 32),
          (uint32)base_1p25ms_portion );

} /* m1x_time_get_ms */



/*=============================================================================

FUNCTION M1X_TIME_GET_MS_NATIVE

DESCRIPTION
  Get a timestamp from System Time in millisecond units from 6 Jan 1980
  00:00:00.

DEPENDENCIES
  None

RETURN VALUE
  The time in ms from 6 Jan 1980 00:00:00.

SIDE EFFECTS
  None

=============================================================================*/

uint64 m1x_time_get_ms_native( void )
{
  /* Time in quadword format (time_type). */
  time_type                       ts_val;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Get the current time in ms and return it in native uint64 format. */
  m1x_time_get_ms( ts_val );

  return ( ((uint64)qw_hi( ts_val ))<<32 ) | ( (uint64)qw_lo( ts_val ) );

} /* m1x_time_get_ms_native */

/*=============================================================================

FUNCTION M1X_TIME_UPDATE_XO_SNAPSHOT_TIME

DESCRIPTION
 Updates the current CDMA system Time and the current XO time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/
void m1x_time_update_xo_snapshot_time( void )
{
  uint64        curr_time_cx = 0;
  uint64        sc_pos_chips = 0;
  uint64        sys_time_in_80ms = 0;
  qword         sys_time = { 0, 0 };
  uint64        xo_time = 0;
  uint32        sync80_cx8;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */   

  /* ensure that next SYNC80 roll over is min QPCH_COMB_ROLL_OVER_CHIPS away */
  srch_scomb_resolve_sync80_rollover( 150 );

  SRCH_ENTER_ATOMIC_CRIT_SECT();

  /* Get current time in 80ms */
  m1x_time_get_20ms_frame_time( sys_time);
  qw_shift(sys_time, -2);
  sys_time_in_80ms = ONEX_CVT_Q2N(sys_time);  

  /* Convert to CX1 */
  curr_time_cx  = sys_time_in_80ms * 12288 * 8;  // # ms since epoch

  /* get xo time */
  xo_time = VSTMR_XO_READ_FULL();

  /* snapshot of combiner count for XO */
  sync80_cx8 = m1x_stmr_get_comb_snap_shot((uint32)(xo_time & 0xffffffff));

  /* convert to CX1 */
  sc_pos_chips = (sync80_cx8 / 8);

  /* Store the system time */
  m1x_time.sys_time.sys_time_cx = (curr_time_cx + sc_pos_chips + FING_COMB_OFFSET_CHIPS);
  
  /* get the current full 56-bit xo value */
  m1x_time.sys_time.xo_time_cx  = xo_time;

  SRCH_LEAVE_ATOMIC_CRIT_SECT();

  M1XTIME_DBG_MSG(
   "CDMA SNAPSHOT: CDMA sys time high %x:%x, xo time high 0x%x:%x",
    m1x_time.sys_time.sys_time_cx >> 32,
   (uint32) m1x_time.sys_time.sys_time_cx,
   (m1x_time.sys_time.xo_time_cx >> 32 ),
   (uint32) m1x_time.sys_time.xo_time_cx);

} /* m1x_time_update_xo_snapshot_time */


/*=============================================================================

FUNCTION M1X_TIME_GET_XO_SNAPSHOT_TIME

DESCRIPTION
  Get the current CDMA system Time and the current XO time in Cx.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=============================================================================*/

void m1x_time_get_xo_snapshot_time
( 
  uint64* onex_xo_tick, 
  uint64* onex_sys_time
)
{
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
   
  /* If the current 1X time is Valid, do a fresh computation of the 
     requested time. If the 1x time is invalid, return the last known
     good time */

  if ( m1x_time.valid )
  {
    m1x_time_update_xo_snapshot_time();
  }

  *onex_xo_tick  = m1x_time.sys_time.xo_time_cx;
  *onex_sys_time = m1x_time.sys_time.sys_time_cx;

} /* m1x_time_get_xo_snapshot_time */



/*===========================================================================

                     TIMETICK ACCESS AND CONVERSION APIs

===========================================================================*/
typedef struct
{
  boolean           init_done;    /* Timetick handle initialized? */
  DalDeviceHandle*  handle;       /* Handle used by timetick APIs */
} m1x_timetick_struct_type;

static m1x_timetick_struct_type m1x_timetick;

/*=============================================================================

FUNCTION      M1X_TIMETICK_INIT
DESCRIPTION   One time initialization before using other TIMETICK wrapper APIs

DEPENDENCIES  None
RETURN VALUE  None
SIDE EFFECTS  None

=============================================================================*/
void m1x_timetick_init( void )
{
  DALResult result;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Bail if already initialized */
  if( m1x_timetick.init_done )
  {
    return;
  }
  else
  {
    /* Get handle for SystemTimer */
    result = DalTimetick_Attach( "SystemTimer", &m1x_timetick.handle );

    if( ( result != DAL_SUCCESS ) || ( m1x_timetick.handle == NULL ) )
    {
      ONEX_ERR_FATAL( "Cannot attach to SystemTimer: result=%d, handle=0x%X",
                 result, m1x_timetick.handle, 0 );
    }
    else
    {
      m1x_timetick.init_done = TRUE;
      M1X_MSG( MUX, LEGACY_HIGH,
        "M1x Timetick handle initialized" );
    }
  }

} /* m1x_timetick_init */


/*=============================================================================

FUNCTION      M1X_TIMETICK_CONV_FROM_TICKS
DESCRIPTION   Conversion: Ticks to requested "unit"

DEPENDENCIES  The m1x_timetick_init() routine must have been called previously
RETURN VALUE  Time in "unit" requested
SIDE EFFECTS  None

=============================================================================*/
uint64 m1x_timetick_conv_from_ticks
(
  DalTimetickTime64Type     t_ticks,    /* Time in sclk64(or ticks) format */
  DalTimetickUnitType       unit        /* Desired unit for return value */
)
{
  uint64  t_unit = 0; /* Converted time in "unit" format */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ( void ) DalTimetick_CvtFromTimetick64( m1x_timetick.handle,
                                          t_ticks,
                                          unit,
                                          &t_unit );

  return ( t_unit );

} /* m1x_timetick_conv_from_ticks */


/*=============================================================================

FUNCTION      M1X_TIMETICK_CONV_TO_TICKS
DESCRIPTION   Conversion: Input time "unit" to Ticks

DEPENDENCIES  The m1x_timetick_init() routine must have been called previously
RETURN VALUE  Time in "unit" requested
SIDE EFFECTS  None

=============================================================================*/
DalTimetickTime64Type m1x_timetick_conv_to_ticks
(
  uint64                    t_unit,     /* Time in "unit" format */
  DalTimetickUnitType       unit        /* Unit of "t_unit" */
)
{
  uint64  t_ticks = 0; /* Converted time in TICKS */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ( void ) DalTimetick_CvtToTimetick64( m1x_timetick.handle,
                                        t_unit,
                                        unit,
                                        &t_ticks );

  return ( t_ticks );

} /* m1x_timetick_conv_to_ticks */


/*=============================================================================

FUNCTION      M1X_TIMETICK_CONV_TO_SCLK64
DESCRIPTION   Conversion: Input time "unit" to Ticks

DEPENDENCIES  The m1x_timetick_init() routine must have been called previously
RETURN VALUE  Time in "unit" requested
SIDE EFFECTS  None

=============================================================================*/
uint64 m1x_timetick_conv_to_sclk64
(
  uint64                    t_unit,     /* Time in "unit" format */
  DalTimetickUnitType       unit        /* Unit of "t_unit" */
)
{
  uint64  t_sclk64 = 0; /* Converted time in SCLK64 */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch( unit )
  {
    case T_TICK:
    {
      /* Handle T_TICK explicitly.
         NOTE: timetick_cvt_to_sclk64() treats T_TICK==T_SCLK
               This is not valid anymore.
         Conversion formula: sclks = ticks / 586
         Current units & frequencies:
               USTMR ==> T_TICK ==> 19.2 MHz
               SCLK  ==> T_SCLK ==> ~32.7645 kHz */
      t_sclk64 = t_unit / 586;
      break;
    }

    default:
      t_sclk64 = timetick_cvt_to_sclk64( t_unit, unit );
      break;
  }

  return ( t_sclk64 );

} /* m1x_timetick_conv_to_ticks */

/*=============================================================================

FUNCTION      M1X_TIMETICK_GET_XO_TICK
DESCRIPTION   Gets the current DalTimetick in XO time ticks

DEPENDENCIES  The m1x_timetick_init() routine must have been called previously
RETURN VALUE  None
SIDE EFFECTS  None

=============================================================================*/
void  m1x_timetick_get_xo_tick( uint64* value )
{

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if(value != NULL && m1x_timetick.init_done)
  {
    *value = 0ull;
    (void)DalTimetick_GetTimetick64(m1x_timetick.handle, value);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "value is NULL or m1x_timetick not initialized");
  }

} /* m1x_timetick_get_tick */

/*=============================================================================

FUNCTION      M1X_TIMETICK_DIFF
DESCRIPTION   Computes the difference between two timetick counts

DEPENDENCIES  The m1x_timetick_init() routine must have been called previously
RETURN VALUE  None
SIDE EFFECTS  None

=============================================================================*/
void m1x_timetick_diff
(
  DalTimetickTime64Type  start, /* Timetick value at the start of the interval */
  DalTimetickTime64Type  end,   /* Timetick value at the end of the interval */
  DalTimetickUnitType    unit,  /* Units to return time interval in */
  DalTimetickTime32Type* diff   /* Time difference between the two timetick
                                   counts, in the unit given */
)
{

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (m1x_timetick.init_done)
  {
    (void)DalTimetick_Diff( m1x_timetick.handle,
                            (start & 0xFFFFFFFF),
                            (end   & 0xFFFFFFFF),
                            unit,
                            diff );
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "m1x_timetick not initialized");
  }

} /* m1x_timetick_diff */

/*=============================================================================

FUNCTION      M1X_TIMETICK_GET_XO_FREQUENCY
DESCRIPTION   Gets the DalTimetick frequency in Hz

DEPENDENCIES  The m1x_timetick_init() routine must have been called previously
RETURN VALUE  Frequency in Hz (uint32)
SIDE EFFECTS  None

=============================================================================*/
uint32 m1x_timetick_get_xo_frequency( void )
{
  uint32 freq = 0;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if (m1x_timetick.init_done)
  {
    DalTimetick_GetFreq(m1x_timetick.handle, &freq);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "m1x_timetick not initialized");
  }

  return freq;

} /* m1x_timetick_get_frequency */
