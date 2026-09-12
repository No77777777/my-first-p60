/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         H D R   T I M E - S T A M P   S E R V I C E S

GENERAL DESCRIPTION
  This module contains functions for the management of time stamps.

EXTERNALIZED FUNCTIONS
  hdrts_init
    Initializes the HDR Timestamp services.
  hdrts_set
    Set the system timestamp value.
  hdrts_sleep
    CDMA time information is assumed out-of-date from this call
      until hdrts_wake
  hdrts_wake
    Restores CDMA system time operation.
  hdrts_get
    Return the system timestamp value.
  hdrts_get_frame_time
    Return the time in units of frames.
  hdrts_get_slot_time
    Return the time within a page cycle in units of slots.
  hdrts_get_page_cycle
    Return the time in units of page cycles.
  hdrts_get_cycle_slot
    Returns the time in arbitrary cycles and slots into cycle.
  hdrts_from_cycle_slot
    Returns a timestamp for an arbitrary cycle and slot in cycle.
  hdrts_get_frame_time2
    Return the time in units of frames. Value valid during sync.
  hdrts_get_current_time
    Return a time stamp and sclk count.
    
INITIALIZATION AND SEQUENCING REQUIREMENTS
  hdrts_init( ) needs to be called to initialize time keeping functions.

  hdrts_set() needs to be called when accurate time is known from the
  base station.

  hdrts_sleep() needs to be called as the mobile enters a "sleep" cycle, to
  ensure hdrts_get() relies on some other means of determining time.

  hdrts_wake() needs to be called after the mobile has "slept", to adjust for
  the time elapsed during sleep.


  Copyright (c) 2000-2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*



============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrts.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     --------------------------------------------------------- 
08/28/15   rmv     Account time adjustments to mod view while handling scheduled 
                   events and also while reporting time
03/19/14   mbs     Removed unnecessary mutex protection for VSTMR calls
11/21/13   mbs     Added support for enabling VSTMR after STMR block is clocked
11/12/13   mbs     Protected VSTMR registrations
11/08/13   mbs     Corrected time returned from this module
10/29/13   sat     Reduced F3 messages (or/and) ASSERTs
10/10/13   mbs     Added support for multiple timeline usage
10/09/13   mbs     Fixed compilation error for BOLT build
09/26/13   mbs     Added HDR SW changes to use VSTMR
09/30/13   tnp     Changed FEATURE_BOLT_MODEM to FEATURE_HDR_BOLT_MODEM
09/30/13   tnp     Added temp ifndef FEATURE_BOLT_MODEM flag for hdrts_reset_rtc
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
08/09/13   dsp     Added hdrts_reset_rtc().
06/25/13   smd     Removed reading SYS_ONLINE register. 
05/15/13   kss     Added hdrts APIs for checking tx chip time.
05/06/13   mbs     Fixed compiler errors for BOLT build
02/11/13   vko     Fixed compiler warnings.
01/31/13   ljl     Added hdrts_config_mod_mp_time_ref(0 and hdrts_save_mstr().
09/21/12   vke     Fix for deadlock between hdrsrch task and IST4.
11/12/12   kss     Remove frame count workaround.
04/27/12   dsp     Fixed compiler warnings. 
04/10/12   cnx     Fixed compiler errors.
03/21/12   ljl     Added hdrts_clear_pn_roll_interrupt().
03/20/12   ljl     Updated  HDR_HDEM_FRAME_INT_ID.
03/19/12   ljl     Disabled HDRTS_FRAME_COUNT_WORKAROUND for Dime.
03/19/12   ljl     Fixed Dime compiler errors.
01/26/12   ljl     Replaced tramp with DAL.
12/12/11   wsh     Safeguarded hdrts_get() from being used before init
11/07/11   wsh     Init critical section before entering in ts_invalidate()
10/17/11   rmg     Removed inclusion of clk.h.
06/14/11   ssu     In hdrts_get(), unit of low 16 bits of ts_val now in cx32 unit. 
06/13/11   ssu     Replaced time_sync_time_of_day() with 
                   hdrts_update_ts_time() in hdrts_2sec_tick().
06/13/11   ssu     Added function hdrts_update_ts_time.  
04/28/11   kss     Fixed compiler warning.
04/06/11   rmg     Added workaround for frame counter bug on NikeL.
03/16/11   kss     Don't read modulator in hdrts_sleep() if time not valid. 
11/26/10   grl     Protect frame_sync_isr and invalidate from concurrency. 
06/21/10   cnx     Fixed compiler warnings.
06/10/10   kss     Remove cast of REX_ENABLE_PREMPTION due to Core change.
05/18/10   arm     Fixed high lint warnings. 
03/24/01   pxu     Removed FEATURE_MSM6500_DSP_SLOT_TIMER features 
03/10/10   grl     Added missing qw.h include
02/20/10   lyl     Increased PN roll callback function buffer;
01/21/10   kss     Removed even second work around.
12/01/09   kss     Implemented even second int work around.
11/25/09   rkc     Removed time_sync_time_of_day() from hdrts_frame_sync_isr().
11/16/09   kss     Fix uint64/qword alignment issue on Q6. 
09/09/09   wsh     CMI: to support changes in clk and ts APIs
09/24/08   smd     Fixed lint errors.
12/05/08   kss     Removed/replaced intlocks.
12/03/08   kss     Lint fixes.
10/04/07   rkc     Resolved Lint High warnings.
05/16/06   ljl     Added hdrts_get_current_time().
02/27/06   kss     Added hdrts_get_frame_time2().
07/05/05   ajn     Uninstall DEMOD roll ISR during sleep.
02/24/05   sq      Updated copyright info
08/02/04   sq      Print hstr value as mod 512 in addition to the raw value
11/12/03   sq      hdrts_get_roll_count now returns "uint16", not "int16".     
10/21/03   ajn     Minor cleanup - formatting.
10/06/03   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
09/18/03   ajn     Merge from Jaguar orphan branch
09/10/03   mpa     Converted F3 messages to use new MSG2.0 HDR search SSID
08/26/03   ajn     Added support for MSM6500's 32-bit modulator timestamp.
07/18/03   sq      Added support for FEATURE_MSM6500_DSP_SLOT_TIMER
06/05/03   ajn     Corrected _get_roll_count calculation.
03/11/03   ajn     Updated Copyright notice
11/07/02   ajn     Roll handler moved to hdrts.
10/30/02   ajn     Slot based sleep duration support added.
08/19/02   ajn     De-install Demod PN Roll callbacks on hdrts_invalidate()
05/15/02   ajn     Move Sync Time-of-Day to time-valid callback trampoline.
05/09/02   ajn     Avoid dbl cnt of 2sec intrpt @ frame_sync_isr nf=0,of=74
02/19/02   ajn     Extended the range of sleep beyond 100 seconds.
02/11/02   ajn     Added INTLOCK in hdrts_sleep to avoid 2 second time jump
01/30/02   ajn     Preparations for non-roll based sleep
11/29/01   om      Changed hdrts_get_frame_time() to be sleep aware
11/01/01   aaj/ajn Increment PN Roll count after wakeup
10/24/01   ajn     Added a callback for when time becomes valid.
10/19/01   ajn     Removed even second ISR when HDR time invalidated.
09/04/01   ajn     Removed hdrts_is_valid - return valid flag from hdrts_get
                   Added Modulator Time Block workaround code for setting
                   frame number during MTB reset.
08/09/01   ajn     Sleep timeline corrections
06/21/01   kss     Reset even_2sec at sync; added hdrts_is_valid().
04/24/01   aaj     Changes to HSTR register for TAZ3 migration
03/09/01   ajn     Implemented hdrts_sleep & hdrts_wake
02/27/01   ajn     Added cycle/slot functions
09/28/00   kss     Removed some debug-related code.
09/25/00   kss     Added hdrts_get_frame_time().
04/15/00   ajn     Created.

==========================================================================*/

/*==========================================================================

                     IMPLEMENTATION DEPENDANCIES

==========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"

#ifdef FEATURE_HDR_BOLT_MODEM
#include "hdrsrchdef.h"
#else
#include "DDIInterruptController.h"
#include "DalDevice.h"
#include "DALDeviceId.h"
#include "DALStdErr.h"
#include "tramp.h"  /* TBD to remove for Dime */
#endif /* FEATURE_HDR_BOLT_MODEM */

#include "hdrts.h"
#include "msm.h"
#include "rex.h"

#include "qw.h"
#include "hdrmdspmcr.h"
#include "err.h"

#include "hdrdebug.h"

#include "time_genoff.h"

#include "mcs_hwio_modem.h"

/*==========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

==========================================================================*/


/* EJECT */
/*==========================================================================

                      SYSTEM TIMESTAMP FORMAT


   |<------------ 48 bits --------------->|<----- 16 bits ------->|
   +--------------------------------------+-----------------------+
   |      1.25 ms counter                 |   1/32 chip counter   |
   +--------------------------------------+-----------------------+
         (11K years of dynamic range)          (1.25 ms dynamic
                                                range. Rolls over
                                                at count 49152)

  Note: In HDR, the h/w time value is given in units of at least 1/4 slots,
  where a slot is 1.67ms.

  To convert 1/4 slots to 1.25ms, the quarter-slot time is divide by 3.
  The remainder, 0, 1 or 2 becomes the upper 2 bits of the 16-bit 1/32 chip
  counter.  If the HDR h/w time is given in units smaller than 1/4 slot,
  these extra bits are merely appended to the upper 2-bits to create a
  finer quantized timestamp.
==========================================================================*/


/*==========================================================================

                     MODULE CONSTANTS

==========================================================================*/

/* Bit position where the 1.25ms time counter starts
*/
#define TS_SHIFT    16

/* Shift to convert between 80ms counts and TS_80MS values
*/
#define TS_80MS_SHIFT   (TS_SHIFT+6)

/* Number of 1.25 millisecond ticks in ...
*/
#define TS_1p25MS (   1L << TS_SHIFT)
#define TS_5MS    (   4L << TS_SHIFT)
#define TS_20MS   (  16L << TS_SHIFT)
#define TS_80MS   (  64L << TS_SHIFT)
#define TS_2SEC   (1600L << TS_SHIFT)

/* Number of slots in ...
*/
#define TS_SLOTS_PER_2SEC   1200

/* Number of 80ms super frames in ...
*/
#define TS_80MS_FRAMES_PER_2SEC   25

/* Number of 26.7ms frames in ...
*/
#define TS_FRAMES_PER_2SEC        75

/* Chip count per 2 second for use with mod tx time rollover */
#define TS_CHIP_PER_2S          (1228800 * 2)


/*-------------------------------------------------------------------------
  Modulator Time Stamp Macros
---------------------------------------------------------------------------*/

#ifdef FEATURE_HDR_BOLT_MODEM

#define TS_MODTIME_VAR_DECL \
  vstmr_hstmr_t                   mod_time; \
  mod_time.all = 0;

#define TS_MODTIME_CAPTURE() \
  mod_time = vstmr_hdr_get_current_hstmr(hdrts_mod.view);

#define TS_MODTIME_REPLACE_FRAMES( frame )  \
        mod_time.frame_cnt = frame;

#define TS_MODTIME_TO_SLOTX32()   \
        (uint16)((mod_time.frame_cnt << 9 ) | (mod_time.pnroll_cx1 >> 6))

#define TS_MODTIME_TO_FRAMES()    (mod_time.frame_cnt)
#define TS_MODTIME_TO_SLOTS()     (mod_time.slot_cnt_mod16)

#define HDRTS_PERIODIC_EVT_IDX              0
#define HDRTS_PERIODIC_EVT_MASK             0x1
#define HDRTS_DMOD_ROLL_EVT_START_SLOT      0
#define HDRTS_DMOD_ROLL_EVT_PERIOD_SLOT     16

#else

#define TS_MODTIME_VAR_DECL \
uint32                          mod_time = 0;

#define TS_MODTIME_CAPTURE() \
HWIO_RTC_REVMOD_TX_TIME_LATCH_OUT(0x01); \
mod_time = HWIO_RTC_REVMOD_TX_TIME_RD_IN;

#define TS_MODTIME_REPLACE_FRAMES( frame )  \
        mod_time = ((mod_time & 0x3ffff) | (frame << TS_MODTIME2FRAME_SHIFT));

#define TS_MODTIME_TO_SLOTX32()   (uint16)(mod_time >> 9)

#define TS_MODTIME_TO_FRAMES()    ((mod_time>>TS_MODTIME2FRAME_SHIFT) & 0x7f)
#define TS_MODTIME_TO_SLOTS()     ((mod_time>>TS_MODTIME2SLOT_SHIFT) & 0x7ff)

#define TS_MODTIME_EVEN_2SEC_FLAG() \
(( TS_EVEN_2SEC_REG & TS_EVEN_2SEC_FLAG ) ? TRUE : FALSE)

#define TS_EVEN_SECOND_FLAG       0x02000000UL
#define TS_SHIFT_CX8_TO_SLOTX32   9

/* TBD this needs to be updated for Dime */
#ifdef FEATURE_HDR_DIME_MODEM
#define HDR_HMOD_EVEN_SEC_INT_ID 107 /* hdr_rtc_even_sec_irq */
#define HDR_HDEM_FRAME_INT_ID    104 /* hdr_rtc_pnroll_irq */
#else
#ifdef T_MSM8960
#define HDR_HMOD_EVEN_SEC_INT_ID TRAMP_HMOD_EVEN_SEC_ISR
#define HDR_HDEM_FRAME_INT_ID    TRAMP_HDEM_FRAME_ISR
#endif /* T_MSM8960 */
#endif /* FEATURE_HDR_DIME_MODEM */
/* TBD to add interrupt ID for Dime later */
#endif /* FEATURE_HDR_BOLT_MODEM */

/*==========================================================================

                     MODULE DATA

==========================================================================*/


/*-------------------------------------------------------------------------

                        PN ROLL ISR Callback Mechanism

 PN Roll ISR will be installed only if there is at least one callback
 registered. This mechanism is added to save CPU cycles in taking PN
 roll ISR if no module needs it

---------------------------------------------------------------------------*/

#define HDRTS_MAX_PN_ROLL_CB      6
  /* Max callback functions for PN roll ISR */

typedef struct
{
  int8                            cb_cnt;
    /* Count of registered PN roll callbacks */

  boolean                         sleeping;
    /* Whether or not HDR's cx8 clock is gated off by the sleep controller */

  boolean                         isr_installed;
    /* Whether or not HDR DEMOD Roll isr installed */

  hdrts_roll_cb_type              cb[HDRTS_MAX_PN_ROLL_CB];
    /* Registered PN Roll Callbacks */
}
hdrts_roll_cb_struct_type;


/* Module data structure type */

typedef struct
{
  boolean                         valid;
    /* Flag indicating whether the CDMA time value is valid */

  uint16                          sleep_start_slot;
    /* Revmod slot at which sleep started */

  hdrts_time_valid_cb_type        time_valid_cb;
    /* Callback for when time has been set */

  hdrts_roll_cb_struct_type       pn_roll;
    /* PN Roll Callback struct */

  uint64                          curr_cdma_time_frames;
    /* Total number of frames elapsed from UTC0 */

  uint32                          last_updated_frame_cnt;
    /* The last updated frame count used to ensure returned time
       stamp from HDR TS is correct */

  uint32                          mstr_rtc_offset;
    /* saved mstr */
}
hdrts_struct_type;

rex_crit_sect_type                hdrts_timestamp_crit_sect;
  /* Critical section for hdr time stamp */

boolean                           hdrts_initialized = FALSE;

/* Module data */

LOCAL hdrts_struct_type           hdrts;

#ifndef FEATURE_HDR_BOLT_MODEM
DalDeviceHandle                   *hdrts_hInthandle;
  /* DAL device handle */
#endif /* FEATURE_HDR_BOLT_MODEM */


#ifdef FEATURE_HDR_BOLT_MODEM
/* vstmr timeline type */
typedef struct
{
  const char                      *name;
    /*  Name of the timeline */

  hdrts_vstmr_cb_type             cb;
    /*  vstmr ISR call back function */
    
  boolean                         registered;
    /*  Is vstmr registered? */

  boolean                         enabled;
    /*  Is vstmr enabled? */
  vstmr_hdr_event_tbl_s           evt_table;
    /* Declare event table */

  vstmr_hdr_view_s                *view;
    /* View handle */

  vstmr_hstmr_t                   hstmr;
    /* bit field structure */

}hdrts_timeline_struct_type;

hdrts_timeline_struct_type hdrts_rtc = {"RTC", NULL, FALSE, FALSE},
                           hdrts_mod = {"MOD", NULL, FALSE, FALSE},
                           hdrts_dmod = {"DMOD", NULL, FALSE, FALSE};

#endif /* FEATURE_HDR_BOLT_MODEM */


/* EJECT */
/*==========================================================================

                            FORWARD DECLARATIONS

==========================================================================*/

static void hdrts_update_dmod_roll_isr( void );

#ifdef FEATURE_HDR_BOLT_MODEM
/* ISR to be registered with vstmr to receive event notification */
static void hdrts_vstmr_isr
(
  vstmr_hdr_event_tbl_s *event_tbl,
  void *arg,
  uint32 pending
);

LOCAL void hdrts_frame_tick_isr( void );

static void hdrts_enable
(
  hdrts_timeline_struct_type *timeline,
  hdrts_vstmr_cb_type cb
);
static void hdrts_disable(hdrts_timeline_struct_type *timeline);
static void hdrts_activate_periodic_trigger
(
  hdrts_timeline_struct_type *timeline,
  uint32  first_trigger_point_in_slots,
  uint32  period_in_slots
);
static void hdrts_deactivate_event
(
  hdrts_timeline_struct_type *timeline
);
#endif /* FEATURE_HDR_BOLT_MODEM */

/* EJECT */
/*==========================================================================

FUNCTION HDRTS_GET_TIME_STAMP

DESCRIPTION
  This procedure captures the Time Stamp and Revmod_Time_Stamp

DEPENDENCIES
  Must be called in INTLOCK context. The returned value is valid only
  if the time service is valid. Otherwise the value may not be correct.

RETURN VALUE
  Revmod_Time_Stamp value in 1/32 slot units since last 2-second rollover.

SIDE EFFECTS
  Fills "ts" with "hdrts.stamp".

==========================================================================*/

LOCAL uint16 hdrts_get_time_stamp
(
  hdr_timestamp_t                 ts
)
{
  TS_MODTIME_VAR_DECL
    /* Variable(s) for reading modulator time-stamp values */

  uint16                          frame;
    /* 26ms frame (0 .. 74) within a 2 second period */

  uint64                          frame_time;
    /* Overall CDMA system time in frames */

  uint64                          frame_2sec;
    /* Time in frames of last 2 second boundary */

  uint64                          stamp;
    /* uint64 representation of timestamp */

  uint32                          last_updated_frame_cnt;
    /* HW frame count on last update */

  uint32                          check_frame;
    /* HW frame count on this query */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  frame_time = hdrts.curr_cdma_time_frames;

  last_updated_frame_cnt = hdrts.last_updated_frame_cnt;
    /* Capture recorded HW frame on last frame update */  
  
  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  TS_MODTIME_CAPTURE();
    /* Capture the modulator timestamp value */

  check_frame = TS_MODTIME_TO_FRAMES();
    /* Get current HW frame */

  /* Check if frame boundary has been reached but the frame count
     is yet to be updated */
  if (last_updated_frame_cnt != check_frame)
  {
    if ( last_updated_frame_cnt == ((check_frame + 1) & 0x7F) )
    {
      /* Please read detailed comments in hdrts_frame_tick_isr 
         Case 1: -----------------Ta--F--F'---------------
         we have incremented curr_cdma_time_frames at F, before
         reaching actual frame boundary. Curernt request for time came
         betwee F--F', i.e scheduled event fired but frame bounday hasn't
         reached. Hence while returning time we need to bump down and report
       */

      frame_time--;
      HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "HSTR Debug: Case 1 Hit %d %d %x %x", 
          last_updated_frame_cnt, check_frame,
          (0xFFFFFFFF & (frame_time >> 32)),
          (0xFFFFFFFF & frame_time) );
    }
    else
    {
      /* Please read detailed comments in hdrts_frame_tick_isr 
         Case 2: -----------------Ta--F'--F---------------
         we have already reached frame boundary at F' but have not incremented
         curr_cdma_time_frames as we are yet to reach F (scheduled frame event)
       
         Curernt request for time came between F'--F
         Hence while returning time add a frame that we are yet to account for
         and report time.
       */
      frame_time++;
       /* Add 1 frame that we are yet to account for */
      HDR_MSG_SRCH_4( MSG_LEGACY_LOW, "HSTR Debug: Case 2 Hit %d %d %x %x", 
          last_updated_frame_cnt, check_frame,
          (0xFFFFFFFF & (frame_time >> 32)),
          (0xFFFFFFFF & frame_time) );
    }

    if( hdrts.valid )
    {
      /* Values should never be off by more than 1 */
      if (( check_frame - last_updated_frame_cnt ) != 1 )
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                        "HDRTS error - missed interrupt? [ %d - %d > 1 ]",
                        check_frame, last_updated_frame_cnt );
      }
    }
    else
    {
      HDR_MSG_SRCH_4( MSG_LEGACY_MED, "TS is not valid yet %d %d %x %x",
                      check_frame, last_updated_frame_cnt, 
                      (0xFFFFFFFF & (frame_time >> 32)),
                      (0xFFFFFFFF & frame_time) );
    }
  }

  frame = (uint16) (frame_time % TS_FRAMES_PER_2SEC);
    /* Compute 26.7ms frame offset since most recent 2 second boundary.
       Range: 0 .. 74 */

  frame_2sec = frame_time - frame;
    /* Round frame time down to nearest 2-second boundary */

  stamp = ( frame_2sec / 3 ) << TS_80MS_SHIFT;
    /* Convert from frame_time into TimeStamp format */

  qw_set
    (
      ts,
      (uint32)((stamp >> 32) & 0xFFFFFFFF), 
      (uint32)(stamp & 0xFFFFFFFF)
    );
    /* Load into qword format */

  TS_MODTIME_REPLACE_FRAMES( frame );
    /* Update frame bits with current frame */

  return TS_MODTIME_TO_SLOTX32();

} /* hdrts_get_time_stamp( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_INIT

DESCRIPTION
  This function initializes the time stamp services module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Time is marked as invalid.

===========================================================================*/

void hdrts_init( void )
{
  int                             i;
    /* Loop index */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Init time service only once */
  if ( hdrts_initialized )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TS is already initialized");
    return;
  }

  rex_init_crit_sect( &hdrts_timestamp_crit_sect );

  hdrts.valid       = FALSE;
    /* Accurate time is not known */

#ifdef FEATURE_HDR_BOLT_MODEM

  /* Register various timelines */
  hdrts_register_timeline(HDRTS_RTC);
  hdrts_register_timeline(HDRTS_MOD);
  hdrts_register_timeline(HDRTS_DMOD);

#endif /* FEATURE_HDR_BOLT_MODEM */

  /*-- Initialize PN ROLL Callback mechanism --*/

  for ( i = 0; i < HDRTS_MAX_PN_ROLL_CB; i++ )
  {
    hdrts.pn_roll.cb[i] = NULL;
  }

  hdrts.pn_roll.cb_cnt = 0;
    /* No callbacks to start with */

  hdrts.pn_roll.sleeping = FALSE;
    /* HDR is not sleeping */

  hdrts.pn_roll.isr_installed = FALSE;

#ifndef FEATURE_HDR_BOLT_MODEM

  if ( ( DAL_DeviceAttach( DALDEVICEID_INTERRUPTCONTROLLER, 
                           &hdrts_hInthandle ) != DAL_SUCCESS ) ||
       ( hdrts_hInthandle == NULL ) )
  {
    ERR_FATAL( "DAL_DeviceAttach failed", 0, 0, 0);
  }

#endif /* FEATURE_HDR_BOLT_MODEM */

  hdrts_initialized = TRUE;

} /* hdrts_init( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRTS_INVALIDATE

DESCRIPTION
  This function asserts that HDR CDMA time is no longer reliable.  1x CDMA
  time, or Time-of-Day should be used instead.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Time is marked as invalid.

==========================================================================*/

void hdrts_invalidate( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( !hdrts_initialized )
  {
    /* only invalidate HDRTS time if it's already initialized */
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TS is yet to be initialized");
    return;
  }

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "hdrts_invalidate");

  /* Ensure no HDR Time Services ISR are pending/installed */
  hdrts_deregister_roll_callback( hdrts_frame_tick_isr );

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  hdrts.valid = FALSE;
    /* Accurate time is not known */
  
  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

} /* hdrts_invalidate( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_SET

DESCRIPTION
  This function is used sets the current HDR time.

DEPENDENCIES
  hdrts_init()

RETURN VALUE
  None

SIDE EFFECTS
  Temporarily installs a modulator frame ISR to set time valid.
  Callback is called when HDR CDMA time is marked as valid.
  An Even-Second interrupt handler is installed to help maintain "gross
  time".

===========================================================================*/

void hdrts_set
(
  uint64                          frame_time,
    /* Time in frames (26.7ms) from UTC=0 (Jan 6, 1980 00:00:00 GMT) */

  hdrts_time_valid_cb_type        time_valid_cb
    /* Function to call when time has been set */
)
{
  TS_MODTIME_VAR_DECL
    /* Variable(s) for reading modulator time-stamp values */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( !hdrts_initialized )
  {
    /* only invalidate HDRTS time if it's already initialized */
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TS is yet to be initialized");
    return;
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDRTS hdrts_set 0x%lx",
      0xFFFFFFFF & (frame_time));

  hdrts.time_valid_cb = time_valid_cb;
    /* Save time valid callback */

  TS_MODTIME_CAPTURE();
    /* Capture the modulator timestamp value */

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  hdrts.curr_cdma_time_frames = frame_time;
    /* Set frame number on next roll, and mark time valid */

  hdrts.last_updated_frame_cnt = TS_MODTIME_TO_FRAMES();
    /* Record current from. This allows us to detect if time
       request arrives before time has been updated. */
  
  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  hdrts_register_roll_callback( hdrts_frame_tick_isr );
    /* Start frame isr to track frame time */

  if ( hdrts.time_valid_cb != NULL)
  {
    hdrts.time_valid_cb( );
      /* Let caller know that time is now valid */
  }

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  hdrts.valid     = TRUE;
    /* Mark time as correct */

  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "mod_time [%x], slot %x",
                    TS_MODTIME_TO_SLOTX32(), TS_MODTIME_TO_SLOTS() );

} /* hdrts_set( ) */


/*===========================================================================

FUNCTION HDRTS_GET

DESCRIPTION
  This function gets the current HDR time.

DEPENDENCIES
  hdrts_set()

RETURN VALUE
  TRUE if HDR Time is valid, and return the timestamp in 'ts_val'
  FALSE if HDR Time is invalid.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrts_get
(
  hdr_timestamp_t                 ts_val
    /* Output: Timestamp value
       The top 48 bits of ts_val store the number of 1.25ms ticks since 0000hrs 1/6/1980 UTC
       the low 16 bits of ts_val store the number of cx32 ticks since the last 1.25 ms boundary */
)
{
  uint32                          mod_time;
    /* Modulator time, in 1/32 slots */

  uint32                          t_1p25ms;
    /* Modulator time, in 1.25ms ticks */

  uint32                          temp;
    /* Temporary */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( !hdrts_initialized )
  {
    /* return invalid time if not initialized */
    return FALSE;
  }

  mod_time = hdrts_get_time_stamp( ts_val );
    /* Capture the 2-second count and revmod_time_stamp values */
    /* ts_val now has the number of 1.25ms ticks since 0000hrs 1/6/1980 UTC (in 2 sec blocks),
       and modtime has the remainder of the time  since the last 2 sec boundary (in 1/32 slot unit)*/
  
  t_1p25ms = mod_time / 24;
    /* This give the number of 1.25ms (3/4 slot) ticks since the last 2 second boundary*/

  mod_time %= 24;
    /* Remaining 1/32th of a slot (0..23) (5-bits) */

  mod_time = mod_time << 11;
    /* Convert from 1/32 slot to cx32 ticks. */

  temp  = t_1p25ms << TS_SHIFT;
    /* To fill up the ticks since last 2-sec boundary in top bits of ts_val*/

  temp |= mod_time;
    /* Populate the low 16 bits of temp with mod_time */      

  qw_inc( ts_val, temp );
    /* Add sub 2-second timestamp value */

  return hdrts.valid;

} /* hdrts_get( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_GET_FRAME_TIME

DESCRIPTION
  This function gets the current HDR time (since THE BEGINNING) in frames.

DEPENDENCIES
  hdrts_set()

RETURN VALUE
  TRUE if HDR Time is valid, and return frame count is in 'frame_time'
  FALSE if HDR Time is invalid.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrts_get_frame_time
(
  qword                          frame_time
    /* Output: Timestamp value */
)
{
  uint32                          mod_time;
    /* Modulator time, in 1/32 slots */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  mod_time = hdrts_get_time_stamp( frame_time );
    /* Capture the 2-second count and revmod_time_stamp values */

  mod_time = mod_time / 512;
    /* Number of HDR frames past 2 sec; convert 1/32 slots to slots,
       then slots to frames (1/16).  (16*32=512) */

  /* This operation simply shifts frame_val by 16.  Use qw_shift! */
  qw_shift(frame_time, -TS_SHIFT);

  /* To convert CDMA slots to HDR frames, multiply by 3/64.
     (Multiply by 3/4 to convert CDMA (1.25ms) slots to HDR (1.67ms) slots,
      then divide by 16 slots per HDR frame for (3/4)*(1/16)= 3/64).  */
  qw_mul(frame_time,frame_time, 3);
  (void) qw_div_by_power_of_2(frame_time,frame_time, 6);

  qw_inc(frame_time, mod_time);
    /* Add number of frames that have occured since the last 2 sec. isr */

  return hdrts.valid;

} /* hdrts_get_frame_time( ) */

/*===========================================================================

FUNCTION HDRTS_GET_CURRENT_TIME

DESCRIPTION
  This function gets the current HDR time (since THE BEGINNING) in slots 
  and the current sclk count.

DEPENDENCIES
  hdrts_set()

RETURN VALUE
  TRUE if HDR Time is valid, and return current time in 'now_slot'
  FALSE if HDR Time is invalid.

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrts_get_current_time
(
  qword                          now_slot,
    /* Output: Time stamp in slots */
 
  uint32                         *now_sclk
    /* Sclk count */
)
{
  uint16                          mod_time;
    /* Modulator time, in 1/32 slots */
  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  mod_time = hdrts_get_time_stamp( now_slot );
    /* Capture the 2-second count and revmod_time_stamp values */

  if ( now_sclk != NULL )
  {
    *now_sclk = timetick_get_safe();
      /* Get current sclk count */
  }

  /* Convert the current time to slots */
  qw_shift( now_slot, 4-TS_80MS_SHIFT );
  qw_mul( now_slot, now_slot, 3);
    /* Convert to slots:
       right shift by TS_80MS_SHIFT to get 80ms count,
         multiply by 3 to get 26.67ms count,
         multiply by 16 (Left shift 4) to get slots */

  qw_inc( now_slot, mod_time / 32 );
    /* Combine 2-second slots and modulator slots to get
       total number of slots since beginning of time */
  
  return hdrts.valid;

}  /* hdrts_get_current_time */

/* EJECT */
/*===========================================================================

FUNCTION HDRTS_GET_FRAME_TIME2

DESCRIPTION
  This function gets the current HDR time (since THE BEGINNING) in frames.
  This function will return a valid frame time, even if sync is in progress.

  Thus, this function is the similar to hdrts_get_frame_time() except
  that its return value is valid even while sync is in progress.

DEPENDENCIES
  hdrts_set() or hdrts_wake()

RETURN VALUE
  HDR frame count is returned in 'frame_val'

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrts_get_frame_time2
(
  qword                          frame_time
    /* Output: Timestamp value */
)
{
  uint32                          mod_time = 0;
    /* Modulator time, in 1/32 slots */

  boolean                         valid = FALSE;
    /* Flag indicating HDR time is valid */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrts.valid )
  {
    mod_time = hdrts_get_time_stamp( frame_time );
      /* Capture the 2-second count and revmod_time_stamp values */

    mod_time = mod_time / 512;
      /* Number of HDR frames past 2 sec; convert 1/32 slots to slots,
         then slots to frames (1/16).  (16*32=512) */

    valid = TRUE;
  }

  if (valid)
  {
    /* This operation simply shifts frame_val by 16.  Use qw_shift! */
    qw_shift(frame_time, -TS_SHIFT);

    /* To convert CDMA slots to HDR frames, multiply by 3/64.
       (Multiply by 3/4 to convert CDMA (1.25ms) slots to HDR (1.67ms) slots,
        then divide by 16 slots per HDR frame for (3/4)*(1/16)= 3/64).  */
    qw_mul(frame_time,frame_time, 3);
    (void) qw_div_by_power_of_2(frame_time,frame_time, 6);

    qw_inc(frame_time, mod_time);
      /* Add number of frames that have occured since the last 2 sec. isr */

  }

  return valid;

} /* hdrts_get_frame_time2( ) */



/* EJECT */
/*===========================================================================

FUNCTION HDRTS_GET_80MS_TIME

DESCRIPTION
  This function gets the current HDR time (since THE BEGINNING) in 80ms
  superframes.

DEPENDENCIES
  hdrts_set()

RETURN VALUE
  HDR frame count is returned in 'superframe_val'

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_get_80ms_time
(
  qword                          superframe_time
)
{
  uint32 mod_time;
    /* Modulator time, in 1/32 slots */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  mod_time = hdrts_get_time_stamp( superframe_time );
    /* Capture the 2-second count and revmod_time_stamp values */

  mod_time = mod_time / 1536;
    /* Number of 80ms superframes past 2 sec; convert 1/32 slots to slots,
       then slots to frames (1/16), and finally frames to superframes (1/3).
       (3*16*32=1536) */

  qw_shift(superframe_time, -TS_80MS_SHIFT);
    /* Convert time-stamp to 80ms units */

  qw_inc(superframe_time, mod_time);
    /* Add number of frames that have occured since the last 2 sec. isr */

} /* hdrts_get_80ms_time( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_GET_SLOT_TIME

DESCRIPTION
  This function returns the current slot (1.67ms) since the latest 5.12s
  boundary (0..3071).

DEPENDENCIES
  hdrts.valid == TRUE

RETURN VALUE
  Slots (1.67ms) since latest 5.12 second boundary (0..3071)

SIDE EFFECTS
  None

===========================================================================*/

uint16 hdrts_get_slot_time( void )
{
  uint16                          mod_time = 0;
    /* Modulator time, in 1/32 slots */

  uint16                          slots;
    /* Slots since last 2-second interrupt */

  qword                           ts_val;
    /* Temporary */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  mod_time = hdrts_get_time_stamp( ts_val );
    /* Capture the 2-second count and revmod_time_stamp values */

  slots = mod_time / 32;
    /* Convert 1/32 slots into slot count */

  qw_shift( ts_val, 4-TS_80MS_SHIFT );
  qw_mul( ts_val, ts_val, 3);
    /* Convert to slots:
         right shift by TS_80MS_SHIFT to get 80ms count,
         multiply by 3 to get 26.67ms count,
         multiply by 16 (Left shift 4) to get slots */

  qw_inc( ts_val, slots );
    /* Combine 2-second slots and modulator slots to get
       total number of slots since beginning of time */

  return qw_div( ts_val, ts_val, HDRTS_SLOTS_PER_CCC );
  /* Slots since beginning of time, modulo-5.12seconds */

} /* hdrts_get_slot_time( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_GET_PAGE_CYCLE

DESCRIPTION
  This function returns the current 5.12 second paging slot since the
  beginning of time ( Jan 6, 1980 )

DEPENDENCIES
  hdrts.valid == TRUE

RETURN VALUE
  Paging slot cycles since UTC=0.

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrts_get_page_cycle( void )
{
  uint16                          mod_time;
    /* Modulator time, in 1/32 slots */

  uint16                          superframes;
    /* 80ms superframes since last 2-second interrupt */

  qword                           ts_val;
    /* Temporary */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  mod_time = hdrts_get_time_stamp( ts_val );
    /* Capture the 2-second count and revmod_time_stamp values */

  superframes = mod_time / (32*16*3);
      /* divide by 32 to get slots,
       divide by 16 to get 26.7ms frames,
       divide by 3 to get 80ms frames */

  qw_shift( ts_val, -TS_80MS_SHIFT );
    /* Convert to 80ms superframes */

  qw_inc( ts_val, superframes );
    /* Combine 2-second superframes and modulator superframes to get
       total number of superframes since beginning of time */

  ( void ) qw_div( ts_val, ts_val, HDRTS_80MS_PER_CCC);
    /* 64 superframes (0.080s) in every 5.12 second period */

  return qw_lo(ts_val);
    /* Range: Year 1980 to Year 2687 */

} /* hdrts_get_page_cycle( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_GET_CYCLE_AND_SLOT

DESCRIPTION
  This function returns the current time in terms of arbitrary length cycles
  and slots into the cycle

DEPENDENCIES
  hdrts.valid == TRUE

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrts_get_cycle_and_slot
(
  qword                           cycle,
    /* Output: arbitrary length cycle index since UTC=0 */

  uint32                        * slot,
    /* Output: slot in the above cycle */

  uint32                          slots_per_cycle
    /* Arbitrary cycle length, in slots */
)
{
  uint16 mod_time;
    /* Modulator time, in 1/32 slots */

  qword  ts_val;
    /* Temporary */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  mod_time = hdrts_get_time_stamp( ts_val );
    /* Capture the 2-second count and revmod_time_stamp values */

  qw_shift( ts_val, 4-TS_80MS_SHIFT );
  qw_mul( ts_val, ts_val, 3);
    /* Convert to slots:
           right shift by TS_80MS_SHIFT to get 80ms count,
             multiply by 3 to get 26.67ms count,
             multiply by 16 (Left shift 4) to get slots */

  qw_inc( ts_val, mod_time / 32 );
    /* Combine 2-second slots and modulator slots to get
       total number of slots since beginning of time */

  *slot = qw_div( cycle, ts_val, slots_per_cycle);
    /* Convert to cycles and slots */

} /* hdrts_get_cycle_and_slot( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_FROM_CYCLE_AND_SLOT

DESCRIPTION
  This function returns the time corresponding to a cycle and slot in cycle,
  for an arbitrary length cycle.

DEPENDENCIES
  hdrts.valid == TRUE

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrts_from_cycle_and_slot
(
  qword                           timestamp,
    /* Output: Time corresponding to the given cycle/slot */

  qword                           cycle,
    /* Arbitrary length cycle index since UTC=0 */

  uint32                          slot,
    /* Slot in the above cycle */

  uint32                          slots_per_cycle
    /* Arbitrary cycle length, in slots */
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  qw_mul( timestamp, cycle, slots_per_cycle );
    /* Convert cycle into slots from UTC=0 */

  qw_inc( timestamp, slot );
    /* Add in slot from beginning of cycle */


  /* Convert from slots to time-stamp format */

  qw_mul( timestamp, timestamp, 4);
  ( void ) qw_div( timestamp, timestamp, 3);
    /* Slots * 4 / 3 --> 1.25ms units */

  qw_shift( timestamp, TS_SHIFT );
    /* Shift 1.25ms units up to form timestamp */

} /* hdrts_from_cycle_and_slot( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_GET_ROLL_COUNT

DESCRIPTION
  Returns a number of PN rolls, wrapping around every 65536 rolls, or
  approximately every 29 minutes.

  HDRTS_PN_ROLL_COUNT_COMPARE( ) can be used to compare two count times
  within a 14 minute range, to determine which is earlier.

DEPENDENCIES
  None

RETURN VALUE
  Roll index.

SIDE EFFECTS
  None

===========================================================================*/

uint16 hdrts_get_roll_count( void )
{
  TS_MODTIME_VAR_DECL
    /* Variable(s) for reading modulator time-stamp values */

  uint16                          roll_count = 0;
    /* 80ms superframes since last 2-second interrupt */

  uint16                          check_frame, last_updated_frame_cnt;
    /* Used to check whether SW frame count has been updated for current
       frame */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "HDRTS hdrts_get_roll_count %d",
                                      hdrts.curr_cdma_time_frames & 0xFFFF);

  if( !hdrts.valid )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "TS is not valid yet");
    return roll_count;
  }

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  last_updated_frame_cnt = hdrts.last_updated_frame_cnt;

  roll_count = hdrts.curr_cdma_time_frames & 0xFFFF;
    /* Use lower bits of system time for roll count */

  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  TS_MODTIME_CAPTURE();
    /* Capture the modulator timestamp value */

  check_frame = TS_MODTIME_TO_FRAMES();

  /* Check whether frame count has been updated for current frame */
  if (last_updated_frame_cnt != check_frame)
  {
    if ( last_updated_frame_cnt == ((check_frame + 1) & 0x7F) )
    {
      /* Please read detailed comments in hdrts_frame_tick_isr 
         Case 1: -----------------Ta--F--F'---------------
         we have incremented curr_cdma_time_frames at F, before
         reaching actual frame boundary. Curernt request for time came
         betwee F--F', i.e scheduled event fired but frame bounday hasn't
         reached. Hence while returning time we need to bump down and report
       */

      roll_count--;
    }
    else
    {
      /* Please read detailed comments in hdrts_frame_tick_isr 
         Case 2: -----------------Ta--F'--F---------------
         we have already reached frame boundary at F' but have not incremented
         curr_cdma_time_frames as we are yet to reach F (scheduled frame event)
       
         Curernt request for time came between F'--F
         Hence while returning time add a frame that we are yet to account for
         and report time.
       */
      roll_count++;
        /* Add 1 frame that we are yet to account for */
    }

    /* Values should never be off by more than 1 */
    if (( check_frame - last_updated_frame_cnt ) != 1 )
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                      "HDRTS error - missed interrupt? [ %d - %d > 1 ]",
                      check_frame, last_updated_frame_cnt );
    }
  }

  return roll_count;

} /* hdrts_get_roll_count( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_SLEEP

DESCRIPTION
  This function is used to inform the HDR Time Stamp Services about an
  upcoming discontinuity in the HDR CDMA time, cause by sleep.

DEPENDENCIES
  hdrts_set()

RETURN VALUE
  None

SIDE EFFECTS
  HDR CDMA time is marked invalid

===========================================================================*/

void hdrts_sleep( void )
{
  TS_MODTIME_VAR_DECL
    /* Variable(s) for reading modulator time-stamp values */

  boolean ts_valid = FALSE;
    /* variable to check validity for logging */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );
    /* Guard against interrupt */

  /* Only capture modtime if time is valid */
  if (hdrts.valid)
  {
    TS_MODTIME_CAPTURE();
      /* Capture the modulator timestamp value */

    TS_MODTIME_REPLACE_FRAMES( 0 );
      /* Remove HW frame count from mod_time */
  
    hdrts.sleep_start_slot = TS_MODTIME_TO_SLOTS();
      /* Slots since last 2 second interrupt */

    ts_valid = TRUE;
      /* store the validity for logging */
  }

  hdrts.valid = FALSE;
    /* Mark HDR CDMA time as unknown.  Fall back on Time-of-Day */

  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  hdrts.pn_roll.sleeping = TRUE;
  hdrts_update_dmod_roll_isr();
    /* Chipx8 is being turned off - uninstall the DEMOD Roll ISR */

#ifdef FEATURE_HDR_BOLT_MODEM

  /* Disable timeline. No events will be generated after this point
   * from the respective timeline
   */
  hdrts_disable_timeline(HDRTS_DMOD);

#endif /* FEATURE_HDR_BOLT_MODEM */

  /* moving the logging explicitly outside the mutex protection section to
       to avoid deadlock condition when timer_q6_?mutex is used withing 
       timer services */
  if (ts_valid == TRUE)
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_MED,
        "HDRTS entering sleep, start=%d, mod_time[%x]",
         hdrts.sleep_start_slot, TS_MODTIME_TO_SLOTX32());
  }

} /* hdrts_sleep( ) */


/*===========================================================================

FUNCTION HDRTS_WAKE

DESCRIPTION
  This function is used to inform the HDR Time Stamp Services that the MSM
  has woke from sleep, as well as the length of the sleep.  The Time Stamp
  Services will then adjust for the discontinuity.

DEPENDENCIES
  hdrts_sleep()

RETURN VALUE
  None

SIDE EFFECTS
  HDR CDMA time is advanced by the duration of the sleep interval.
  HDR CDMA time is marked valid

===========================================================================*/

void hdrts_wake
(
  uint32                          sleep_dur_slots,
    /* The length of the sleep, in slots (1.67ms) */

  hdrts_time_valid_cb_type        time_valid_cb
    /* Function to call when time has been set */
)
{
  TS_MODTIME_VAR_DECL
    /* Variable(s) for reading modulator time-stamp values */

  uint16                          wake_slot;
    /* Revmod Time-stamp at wakeup */

  uint32                          current_slot;
    /* Current slot time */

  uint32                          frame;
    /* Current frame time */

  uint32                          tmp_frame_cnt;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrts.time_valid_cb = time_valid_cb;
    /* When time is valid, let the caller know through the given callback */

  TS_MODTIME_CAPTURE();
    /* Capture the modulator timestamp value */

  tmp_frame_cnt = TS_MODTIME_TO_FRAMES();
    /* Record current from. This allows us to detect if time
       request arrives before time has been updated. */

  TS_MODTIME_REPLACE_FRAMES( 0 );
    /* Remove HW frame count from mod_time */

  wake_slot = TS_MODTIME_TO_SLOTS();
    /* Slots since last sw time update */

  current_slot = hdrts.sleep_start_slot + sleep_dur_slots + 1;
    /* Advance time from the start of sleep by the length of sleep.
       +1 since sleep won't start until the next slot */

  frame = current_slot / HDRTS_SLOTS_PER_FRAME;

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  hdrts.last_updated_frame_cnt = tmp_frame_cnt;
  hdrts.curr_cdma_time_frames += frame;
    /* Update frame time based on time slept. */

  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

#ifdef FEATURE_HDR_BOLT_MODEM

  /* Enable timeline. Events can be enabled and generated after this point
   * from the respective timeline. ISR needs to be registered to ensure
   * any events are delivered as per the registration.
   */
  hdrts_enable_timeline(HDRTS_DMOD, hdrts_vstmr_isr);

#endif /* FEATURE_HDR_BOLT_MODEM */

  hdrts.pn_roll.sleeping = FALSE;
  hdrts_update_dmod_roll_isr();
    /* re-enabled demod roll interrupt processing */

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  hdrts.valid     = TRUE;
    /* Mark time as correct */

  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  if ( hdrts.time_valid_cb != NULL)
  {
    hdrts.time_valid_cb( );
      /* Let caller know that time is now valid */
  }

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "hdrts wake: mod_time [%x], slot %x",
                  TS_MODTIME_TO_SLOTX32(), TS_MODTIME_TO_SLOTS() );

  /* moving the logging explicitly outside the mutex protection section to
         to avoid deadlock condition when timer_q6_?mutex is used withing 
         timer services */
  HDR_MSG_SRCH_4( MSG_LEGACY_MED,
              "wake_slot=%d  dur=%d  cur_slot=%d frame %d",
              wake_slot, sleep_dur_slots, current_slot,
              hdrts.curr_cdma_time_frames);

} /* hdrts_wake( ) */



/*===========================================================================

FUNCTION HDRTS_ABORT_SLEEP

DESCRIPTION
  This function is used to inform the HDR Time Stamp Services that the MSM
  has woke from sleep, but HDR timing does not need to be maintained.

DEPENDENCIES
  hdrts_sleep()

RETURN VALUE
  None

SIDE EFFECTS
  Demod Roll ISR processing may be re-enabled

===========================================================================*/

void hdrts_abort_sleep( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( !hdrts_initialized )
  {
    if ( hdrts.pn_roll.sleeping == FALSE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Cannot abort TS when it is active");
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Aborting before hdrts initialized");
    }
    return;
  }

#ifdef FEATURE_HDR_BOLT_MODEM

  /* Enable timeline. Events can be enabled and generated after this point
   * from the respective timeline. ISR needs to be registered to ensure
   * any events are delivered as per the registration.
   */
  hdrts_enable_timeline(HDRTS_DMOD, hdrts_vstmr_isr);

#endif /* FEATURE_HDR_BOLT_MODEM */

  hdrts.pn_roll.sleeping = FALSE;
  hdrts_update_dmod_roll_isr();
    /* re-enabled demod roll interrupt processing */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Sleep aborted");

} /* hdrts_abort_sleep( ) */



/* EJECT */
/*===========================================================================

FUNCTION HDRTS_DMOD_ROLL_ISR

DESCRIPTION
  This is the DMOD PN ROLL ISR.  It handles the PN Roll Callbacks.

  The callback functions are executed in the PN ROLL ISR context so it is
  very important to have these callback functions quick and simple.  It is
  recommended that these callback functions just set a Rex Signal to the
  concerned task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrts_dmod_roll_isr
(
  uint32 unused_param
    /* Parameter */
)
{
  int                             i;
    /* loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );
  /* Ensure CB are not deregistered while delivering them */

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "hdrts_dmod_roll_isr %d",
                hdrts.pn_roll.cb_cnt);

  for ( i = 0; i < HDRTS_MAX_PN_ROLL_CB; i++ )
  {
    if ( hdrts.pn_roll.cb[i] )
    {
      hdrts.pn_roll.cb[i]();
        /* execute the callback function */
    }
  }

  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );
} /* hdrts_dmod_roll_isr */



/* EJECT */
/*===========================================================================

FUNCTION HDRTS_UPDATE_DMOD_ROLL_ISR

DESCRIPTION
  This function installs or uninstalls the demod roll isr, if appropriate.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

/* static */ void hdrts_update_dmod_roll_isr( void )
{
  boolean                         install;
    /* Whether or not the ISR should be installed */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "hdrts_update_dmod_roll_isr %d, %d",
                  hdrts.pn_roll.cb_cnt, hdrts.pn_roll.sleeping );

  install = (hdrts.pn_roll.cb_cnt > 0)  &&  !hdrts.pn_roll.sleeping;
    /* Check if this ISR should be installed */

  if ( install != hdrts.pn_roll.isr_installed )
  {
    /* Either the ISR is installed, and shouldn't be,
        -or-  the ISR isn't installed, and should be. */

    if ( install )
    {
#ifdef FEATURE_HDR_BOLT_MODEM

      /* After the timeline is enabled, events (both single and periodic)
       * can be activated on that timeline
       * DMOD ROLL event to start on HDRTS_DMOD_ROLL_EVT_START_SLOT
       * DMOD ROLL event to occur every HDRTS_DMOD_ROLL_EVT_PERIOD_SLOT
       */
      hdrts_activate_periodic_event
      (
        HDRTS_DMOD,
        HDRTS_DMOD_ROLL_EVT_START_SLOT,
        HDRTS_DMOD_ROLL_EVT_PERIOD_SLOT
      );

#else /* FEATURE_HDR_BOLT_MODEM */

      /* Install the demod roll isr */
      DalInterruptController_RegisterISR( hdrts_hInthandle,
                                        ( uint32 ) HDR_HDEM_FRAME_INT_ID,
                                        ( DALIRQ ) hdrts_dmod_roll_isr,
                                        ( const DALIRQCtx )0,
                                        DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER );

#endif /* FEATURE_HDR_BOLT_MODEM */
    }
    else /* !install */
    {
#ifdef FEATURE_HDR_BOLT_MODEM

      /* Disable timeline and stop all events activated on that timeline */
      hdrts_deactivate_timeline_event( HDRTS_DMOD );

#else /* FEATURE_HDR_BOLT_MODEM */
      
      /* Uninstall the demod roll isr */
      DalInterruptController_Unregister( hdrts_hInthandle, HDR_HDEM_FRAME_INT_ID );;
#endif /* FEATURE_HDR_BOLT_MODEM */
    }

    hdrts.pn_roll.isr_installed = install;
      /* Update installed status */
  }

} /* hdrts_update_dmod_roll_isr */


/*===========================================================================

FUNCTION HDRTS_CLEAR_PN_ROLL_INTERRUPT

DESCRIPTION
  This function clears PN roll interrupt.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrts_clear_pn_roll_interrupt( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_BOLT_MODEM

  /* Disable timeline and stop all events activated on that timeline */
  hdrts_deactivate_timeline_event( HDRTS_DMOD );

#else /* FEATURE_HDR_BOLT_MODEM */

  DalInterruptController_InterruptClear( hdrts_hInthandle, ( uint32 ) HDR_HDEM_FRAME_INT_ID );

#endif /* FEATURE_HDR_BOLT_MODEM */

}  /* hdrts_clear_pn_roll_interrupt */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_REGISTER_ROLL_CALLBACK

DESCRIPTION
  This function registers PN ROLL callback function which will be called
  after receiving the PN ROLL ISR.

  The callback functions are executed in the PN ROLL ISR context so it is
  very important to have these callback functions quick and simple.  It is
  recommended that these callback functions just set a Rex Signal to the
  concerned task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  May begin periodic PN roll ISR

===========================================================================*/

void hdrts_register_roll_callback
(
  hdrts_roll_cb_type              cb
    /* callback function to be registered */
)
{
  int                             i;
    /* loop variable */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Check if this callback is already registered */
  for ( i = 0; i < HDRTS_MAX_PN_ROLL_CB; i++ )
  {
    if ( hdrts.pn_roll.cb[i] == cb )
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "CB already registered [%d]", i );
      /* Callback is already registered. Exit at this very moment */
      return;
    }
  }

  /* Do we have room for one more callback ? */
  if ( hdrts.pn_roll.cb_cnt == HDRTS_MAX_PN_ROLL_CB )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "PN Roll callbacks full [%d]",
                    hdrts.pn_roll.cb_cnt );
    return;
  }

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );
  /* Make sure cb_cnt is not altered in between */

  /* New callback function. Find an empty location for it in the cb array */

  for ( i = 0; i < HDRTS_MAX_PN_ROLL_CB; i++ )
  {
    if ( hdrts.pn_roll.cb[i] == NULL )
    {
      /* Found an empty spot where the Callback can be registered */

      hdrts.pn_roll.cb[i] = cb;
        /* register the callback */

      hdrts.pn_roll.cb_cnt++;
        /* increment count of registered callbacks */

      break;

    }
  }

  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Registering CB %d",
                  hdrts.pn_roll.cb_cnt );

  hdrts_update_dmod_roll_isr();
    /* See if we need to install the ISR */

  /* moving the logging explicitly outside the mutex protection section to
      to avoid deadlock condition when timer_q6_?mutex is used withing 
      timer services */
  if ( i == HDRTS_MAX_PN_ROLL_CB )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Max Callback reached ? %d",
                    hdrts.pn_roll.cb_cnt );
  }

}/* hdrts_register_roll_callback */


/* EJECT */
/*===========================================================================

FUNCTION HDRTS_DEREGISTER_ROLL_CALLBACK

DESCRIPTION
  This function unregisters PN ROLL callback function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  May end periodic PN roll ISR

===========================================================================*/

void hdrts_deregister_roll_callback
(
  hdrts_roll_cb_type              cb
    /* callback function to be registered */
)
{
  int                             i;
    /* loop variable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  /* Check if this callback is registered */

  for ( i = 0; i < HDRTS_MAX_PN_ROLL_CB; i++ )
  {
    if ( hdrts.pn_roll.cb[i] == cb )
    {
      hdrts.pn_roll.cb[i] = NULL;
        /* remove the function callback */

      hdrts.pn_roll.cb_cnt--;
        /* One less callback to worry about */

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "DeRegistering CB %d",
                      hdrts.pn_roll.cb_cnt );
    }
  }

  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  hdrts_update_dmod_roll_isr();
    /* Uninstall dmod roll isr, if it is no longer needed */

}/* hdrts_deregister_roll_callback */


/*===========================================================================

FUNCTION HDRTS_UPDATE_TS_TIME

DESCRIPTION
  This function updates the time managed by time services.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrts_update_ts_time( void )
{
  
  hdr_timestamp_t                          ts_val;
    /* Variable for holding current timestamp value */

  time_genoff_args_struct_type                         args;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if( hdrts_get(ts_val) )
  {
   
    args.base = ATS_HDR;
    args.ts_val = ts_val;
    args.unit = TIME_STAMP;
    args.operation = T_SET;
    args.base_source = TIME_SCLK;

    time_genoff_opr(&args);

  }
  else
  {
    HDR_MSG_SRCH(MSG_LEGACY_ERROR, "Could not acquire timestamp using hdrts_get()");
  }

} /* hdrts_update_ts_time */

/*===========================================================================

FUNCTION HDRTS_SAVE_MODTIME_REF

DESCRIPTION
  This function saves the rtc offset so that it can be set to modtime_ref.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrts_save_mstr
( 
  uint32                       offset
    /* Tx RTC offset */
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrts.mstr_rtc_offset = offset;
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "MSTR_RTC_OFFSET 0x%x",
                  hdrts.mstr_rtc_offset );

} /* hdrts_save_modtime_ref */


/*===========================================================================

FUNCTION HDRTS_CONFIG_MODTIME_REF

DESCRIPTION
  This function configures modtime_ref.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrts_config_mod_mp_time_ref( void )
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef FEATURE_HDR_BOLT_MODEM
  HWIO_HDR_MODTIME_REF_OUT( hdrts.mstr_rtc_offset );
  HWIO_HDR_MPTIME_REF_OUT( hdrts.mstr_rtc_offset );
#endif /* FEATURE_HDR_BOLT_MODEM */

  HDR_MSG_SRCH_1(MSG_LEGACY_MED, "Write 0x%x to HDR_MODTIME_REF reg and HDR_MPTIME_REF reg", hdrts.mstr_rtc_offset );

} /* hdrts_config_modtime_ref */


/*===========================================================================

FUNCTION HDRTS_UINV_DUMP

DESCRIPTION
  This function dumps RTC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrts_univ_dump( void )
{
#ifndef FEATURE_HDR_BOLT_MODEM
  uint32 univ_stmr_time_status_rd1 = 0;
  uint32 univ_stmr_time_status_rd2 = 0;
  uint32 rtc_hdr_time_status_rd = 0;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  do
  {
    HWIO_UNIV_STMR_STATUS_DUMP_CMD_OUT( 1 );

    univ_stmr_time_status_rd1 = HWIO_UNIV_STMR_TIME_STATUS_RD_IN;

    rtc_hdr_time_status_rd = HWIO_RTC_HDR_TIME_STATUS_RD_IN;

    univ_stmr_time_status_rd2 = HWIO_UNIV_STMR_TIME_STATUS_RD_IN;

    HDR_MSG_SRCH_4(MSG_LEGACY_HIGH, 
                   "univ_stmr_rd1 0x%x, univ_stmr_time_rd2 0x%x rtc_hdr_rd 0x%x rtc_hdr_rd>>2 0x%x",
                   (uint32) univ_stmr_time_status_rd1, (uint32) univ_stmr_time_status_rd2, 
                   (uint32) rtc_hdr_time_status_rd, ((uint32)rtc_hdr_time_status_rd) >> 2 );
  }
  while ( univ_stmr_time_status_rd1 != univ_stmr_time_status_rd2  ); 

#endif /* FEATURE_HDR_BOLT_MODEM */

} /* hdrts_univ_dump */

/*===========================================================================

FUNCTION HDRTS_GET_TX_CHIP_TIME

DESCRIPTION
  Returns Tx time from modulator, in chips.

DEPENDENCIES
  Clocks must be on. Function does not check clock state.

RETURN VALUE
  Tx time in chips. Wraps at 75 frames. 

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrts_get_tx_chip_time( void )
{
  TS_MODTIME_VAR_DECL
    /* Variable(s) for reading modulator time-stamp values */

  uint64                          frame_time;
    /* Overall CDMA system time in frames */

  uint32                          last_updated_frame_cnt;
    /* HW frame count on last update */

  uint32                          check_frame;
    /* HW frame count on this query */

  uint32                          tx_chip_time = 0;
    /*  Computed tx chip time */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );

  frame_time = hdrts.curr_cdma_time_frames;

  last_updated_frame_cnt = hdrts.last_updated_frame_cnt;
    /* Capture recorded HW frame on last frame update */  
  
  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  TS_MODTIME_CAPTURE();
    /* Capture the modulator timestamp value */

  check_frame = TS_MODTIME_TO_FRAMES();
    /* Get current HW frame */

  /* Check if frame boundary has been reached but the frame count
     is yet to be updated */
  if( last_updated_frame_cnt != check_frame )
  {
    if ( last_updated_frame_cnt == ((check_frame + 1) & 0x7F) )
    {
      /* Please read detailed comments in hdrts_frame_tick_isr 
         Case 1: -----------------Ta--F--F'---------------
         we have incremented curr_cdma_time_frames at F, before
         reaching actual frame boundary. Curernt request for time came
         betwee F--F', i.e scheduled event fired but frame bounday hasn't
         reached. Hence while returning time we need to bump down and report
       */

      frame_time--;
    }
    else
    {
      /* Please read detailed comments in hdrts_frame_tick_isr 
         Case 2: -----------------Ta--F'--F---------------
         we have already reached frame boundary at F' but have not incremented
         curr_cdma_time_frames as we are yet to reach F (scheduled frame event)
       
         Curernt request for time came between F'--F
         Hence while returning time add a frame that we are yet to account for
         and report time.
       */
      frame_time++;
        /* Add 1 frame that we are yet to account for */
    }

    if( hdrts.valid )
    {
      /* Values should never be off by more than 1 */
      if (( check_frame - last_updated_frame_cnt ) != 1 )
      {
        HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, 
                        "HDRTS error - missed interrupt? [ %d - %d > 1 ]",
                        check_frame, last_updated_frame_cnt );
      }
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "TS is not valid yet");
    }
  }
  tx_chip_time = ((frame_time  % TS_FRAMES_PER_2SEC) * HDR_PN_CIRC_X1) +
                                            (mod_time.pnroll_cx1);

  return tx_chip_time;
} /* hdrts_get_tx_chip_time */


/*===========================================================================

FUNCTION HDRTS_GET_TX_CHIP_TIME_ELAPSED

DESCRIPTION
  Compares and returns the difference between two Tx chips times.

DEPENDENCIES
  Time between samples should be < 2 seconds

RETURN VALUE
  Difference in chips

SIDE EFFECTS
  None

===========================================================================*/

uint32 hdrts_get_tx_chip_time_elapsed
(
  uint32 start,
  uint32 end
)
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  return ((end > start)?(end - start):(end + TS_CHIP_PER_2S - start));

} /* hdrts_get_tx_chip_time */


/*===========================================================================

FUNCTION HDRTS_FRAME_TICK_ISR

DESCRIPTION
  This frame tick Interrupt Service Routine is used to maintain system
  time while the MSM is awake.

DEPENDENCIES
  This ISR will be installed after the frame sync isr just to ensure
  a known start point

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrts_frame_tick_isr(void)
{

  TS_MODTIME_VAR_DECL
    /* Variable(s) for reading modulator time-stamp values */

  qword current_frametime;

  uint32                          curr_vtsmr_frame_cnt;
    /* Latest Frame count from VSTMR Mod view */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_3( MSG_LEGACY_LOW, "Begin Frame interrupt %d %x %x", 
          hdrts.last_updated_frame_cnt, 
          (0xFFFFFFFF & (hdrts.curr_cdma_time_frames >> 32)),
          (0xFFFFFFFF & hdrts.curr_cdma_time_frames) );

  if((hdrts.curr_cdma_time_frames % 10) == 0)
  {
    (void) hdrts_get_frame_time( current_frametime );
    HDR_MSG_PROT_2( MSG_LEGACY_LOW,
                    "Now: 0x%x, 0x%x",
                      qw_hi(current_frametime),qw_lo(current_frametime) );
  }

  TS_MODTIME_CAPTURE();
    /* Capture the modulator timestamp value */

  curr_vtsmr_frame_cnt  = TS_MODTIME_TO_FRAMES();
    /* Latest Frame count from VSTMR Mod view */

  /* 
         F  - Frame boundary as per old time
         Ta - Time Adjust happened (ex: FW slam )
         F' - New Frame boundary as per adjusted time
     
     Note 1: Regardless of time adjustment, scheduled frame event will still
     fire at F as scheduled. Only from subsequent frame boundaries VSTMR does 
     re-scheduling of events as per adjusted time.
   
     Also note Time Adjustment can move time by a little in past or future
     Hence below two cases are possible.
   
     We are in this function means, we are at F.
     
     Case 1: -----------------Ta--F--F'---------------
   
     Case 2: -----------------Ta--F'--F---------------
  */

  /* Handle Case 1 : HW hasn't incremented the frame yet */
  if (curr_vtsmr_frame_cnt == hdrts.last_updated_frame_cnt) 
  {
      /* Check if it is indeed Case 1 - we should be close to frame boundary. */
    if (TS_MODTIME_TO_SLOTS() == 15)
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HSTR Debug: Case 1 hit - %d", 
          curr_vtsmr_frame_cnt);

      curr_vtsmr_frame_cnt++;
      /* We have accounted for the upcoming frame boundary here as we
         arrived early and will not get this opportunity at F'.
      
         Note 2: Since we are updating it early, from now to the actual frame
         boundary (F--F'), if any request for time_stamp, roll_count,
         chip_time etc comes then we would return value based on
         HW adjusted time */

      curr_vtsmr_frame_cnt = curr_vtsmr_frame_cnt & 0x7F;
        /* Check if we have roll over */
    }
  }

  REX_ISR_LOCK( &hdrts_timestamp_crit_sect );
  hdrts.curr_cdma_time_frames++;
    /* Advance time by 1 frame */

  /* Handle Case 2 : HW already incremented the frame at F', so we just assign
     the advanced value from HW */
  hdrts.last_updated_frame_cnt = curr_vtsmr_frame_cnt;
  REX_ISR_UNLOCK( &hdrts_timestamp_crit_sect );

  HDR_MSG_SRCH_4( MSG_LEGACY_LOW, "HSTR Debug: Updated last_updated_frame_cnt"
                                  " %d, curr_cdma_time_frames %x %x, pn_cx1 %d", 
          hdrts.last_updated_frame_cnt, 
          (0xFFFFFFFF & (hdrts.curr_cdma_time_frames >> 32)),
                  (0xFFFFFFFF & hdrts.curr_cdma_time_frames),
                  mod_time.pnroll_cx1 );

  /* Update as even sec interrupt would have */
  if ((hdrts.curr_cdma_time_frames % 75) == 0)
  {
    hdrts_update_ts_time();
      /* Update TOD as CDMA time */
  }

} /* hdrts_frame_tick_isr( ) */

#ifdef FEATURE_HDR_BOLT_MODEM
/*===========================================================================

FUNCTION HDRTS_VSTMR_ISR

DESCRIPTION
  This vstmr Interrupt Service Routine is used to route all frame
  boundary event requests while the MSM is awake.

DEPENDENCIES
  This ISR will be installed after the module receives first request
  for frame boundary event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_vstmr_isr
(
  vstmr_hdr_event_tbl_s *event_tbl,
  void *arg,
  uint32 pending
)
{
  /*  vstmr allows client to register total of 16 events on any time line.
   *  The first set  of events using event index 0-7  (total 8 events) are
   *  designated as  periodic events  and rest 8-15  (total 8  events) are
   *  designated as  one time events. If  more than 1 event  occurs at the
   *  same time, vstmr  calls the registered ISR only 1  time and delivers
   *  mask indicating the events that  have expired at that instance. Here
   *  same time  could be  vstmr definition  how close  the events  at any
   *  given instance. For example, if  the client registered 3 events with
   *  event  index 0,  3 (periodic)  and 9  (one-shot) all  at the  slot 3
   *  binary 1000001001 or  0x209. Essentially 0th evt maps to  bit 0 (1st
   *  bit in LSB)  and 15th evt maps  to bit number 15 (which  is 16th bit
   *  from  LSB). This  is as  per explanation  from vstmr  team only  (no
   *  document is available).
   */

  if (pending & HDRTS_PERIODIC_EVT_MASK)
  {
    hdrts_dmod_roll_isr(0);
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Incorrect VSTMR event %d", pending );
  }
}

/*===========================================================================

FUNCTION HDRTS_GET_RTC_CX1

DESCRIPTION
  A simple access function to return RTC in CX1 format from VSTMR

DEPENDENCIES
  Expects VSTMR to be active and event table registered

RETURN VALUE
  RTC in CX1

SIDE EFFECTS
  None

===========================================================================*/
uint32 hdrts_get_rtc_cx1(void)
{
  vstmr_hstmr_t         curr_time;

  curr_time.pnroll_cx1 = 0;

  /* Return valid time only if hdrts module is initialized */
  if( hdrts_rtc.registered == TRUE )
  {
    curr_time = vstmr_hdr_get_current_hstmr(hdrts_rtc.view);
      /* Read current time from vstmr */
  }
  return curr_time.pnroll_cx1;
}

/*===========================================================================

FUNCTION HDRTS_GET_RTC_CX2

DESCRIPTION
  A simple access function to return RTC in CX2 format from VSTMR

DEPENDENCIES
  Expects VSTMR to be active and event table registered

RETURN VALUE
  RTC in CX2

SIDE EFFECTS
  None

===========================================================================*/
uint32 hdrts_get_rtc_cx2(void)
{
  vstmr_hstmr_t         curr_time;

  curr_time.pnroll_cx2 = 0;

  /* Return valid time only if hdrts module is initialized */
  if( hdrts_rtc.registered == TRUE )
  {
    curr_time = vstmr_hdr_get_current_hstmr(hdrts_rtc.view);
      /* Read current time from vstmr */
  }
  return curr_time.pnroll_cx2;
}

/*===========================================================================

FUNCTION HDRTS_GET_RTC_CX8

DESCRIPTION
  A simple access function to return RTC in CX8 format from VSTMR

DEPENDENCIES
  Expects VSTMR to be active and event table registered

RETURN VALUE
  RTC in CX8

SIDE EFFECTS
  None

===========================================================================*/
uint32 hdrts_get_rtc_cx8(void)
{
  vstmr_hstmr_t         curr_time;

  curr_time.pnroll_cx8 = 0;

  /* Return valid time only if hdrts module is initialized */
  if( hdrts_rtc.registered == TRUE )
  {
    curr_time = vstmr_hdr_get_current_hstmr(hdrts_rtc.view);
      /* Read current time from vstmr */
  }
  return curr_time.pnroll_cx8;
}

/*===========================================================================

FUNCTION HDRTS_GET_FRAME_CNT

DESCRIPTION
  A simple access function to return current frame count from VSTMR

DEPENDENCIES
  Expects VSTMR to be active and event table registered

RETURN VALUE
  Frame count

SIDE EFFECTS
  None

===========================================================================*/
uint32 hdrts_get_frame_cnt(void)
{
  vstmr_hstmr_t         curr_time;

  curr_time.frame_cnt = 0;

  /* Return valid time only if hdrts module is initialized */
  if( hdrts_rtc.registered == TRUE )
  {
    curr_time = vstmr_hdr_get_current_hstmr(hdrts_rtc.view);
      /* Read current time from vstmr */
  }
  return curr_time.frame_cnt;
}

/*===========================================================================

FUNCTION HDRTS_REGISTER_TIMELINE

DESCRIPTION
  Allows various timeline to be registered with vstmr

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  VSTMR views are registered

===========================================================================*/
void hdrts_register_timeline(hdrts_timeline_enum_type timeline)
{
  switch(timeline)
  {
    case HDRTS_RTC: /* Request RTC view */
      if(hdrts_rtc.registered == FALSE)
      {
        hdrts_rtc.view = vstmr_hdr_get_view_handle
                        (
                          VSTMR_RTC_HDR_SUB0,
                          VSTMR_HDR_RTC_VIEW
                        );
          /* Obtain view handle from VSTMR */

        hdrts_rtc.registered = TRUE;
          /* Now VSTMR RTC views are ready to be used */

        HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "RTC registered");
      }
      break;

    case HDRTS_MOD: /* Request MOD view */
      if(hdrts_mod.registered == FALSE)
      {
        hdrts_mod.view = vstmr_hdr_get_view_handle
                        (
                          VSTMR_RTC_HDR_SUB0,
                          VSTMR_HDR_TX_SYS_TIME_VIEW
                        );
          /* Obtain view handle from VSTMR */

        hdrts_mod.registered = TRUE;
          /* Now VSTMR MOD views are ready to be used */

        HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "MOD registered");
      }
      break;

    case HDRTS_DMOD: /* Request DMOD view */
      if(hdrts_dmod.registered == FALSE)
      {
        hdrts_dmod.view = vstmr_hdr_get_view_handle
                        (
                          VSTMR_RTC_HDR_SUB0,
                          VSTMR_HDR_RX_SYS_TIME_VIEW
                        );
          /* Obtain view handle from VSTMR */

        hdrts_dmod.registered = TRUE;
          /* Now VSTMR DMOD views are ready to be used */

        HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "DMOD registered");
      }
      break;

    default: /* Only if forced with non enum value to timeline param */
      ERR_FATAL("Unknown timeline registration %d", timeline,0,0);
      break;
  }
}

/*===========================================================================

FUNCTION HDRTS_ENABLE

DESCRIPTION
  Enables requested timeline if it has not been enabled

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_enable
(
  hdrts_timeline_struct_type *timeline,
  hdrts_vstmr_cb_type cb
)
{
  uint32 cb_arg = 0;

  if((timeline->registered == TRUE) &&
     (timeline->enabled == FALSE) &&  /* Ensure called in a critical section */
     (cb != NULL))
  {
    timeline->cb = cb;
    vstmr_hdr_event_tbl_register
      (
        &(timeline->evt_table),
        timeline->view,
        VSTMR_INT_NORMAL
      );
      /* Register an event table with VSTMR */

    vstmr_hdr_interrupt_register
      (
        &(timeline->evt_table),
        timeline->cb,
        &(cb_arg)
      );
    timeline->enabled = TRUE;
      /* Register ISR with VSTMR */

    HDR_MSG_DRIVERS_SPRINTF_1( MSG_LEGACY_HIGH, "%s enabled",
                                                    timeline->name );
  }
  else
  {
    HDR_MSG_DRIVERS_SPRINTF_1( MSG_LEGACY_ERROR,
        "%s already registered or CB NULL", timeline->name );
  }
}

/*===========================================================================

FUNCTION HDRTS_ENABLE_TIMELINE

DESCRIPTION
  Allows clients to request enabling of a timeline

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_enable_timeline
(
  hdrts_timeline_enum_type timeline,
  hdrts_vstmr_cb_type cb
)
{
  switch(timeline)
  {
    case HDRTS_MOD:
      hdrts_enable(&hdrts_mod, cb);
        /* Enable TX timeline */
      break;

    case HDRTS_DMOD:
      hdrts_enable(&hdrts_dmod, cb);
        /* Enable RX timeline */
      break;

    default: /* Only if forced with non enum value to timeline param */
      ERR_FATAL("Unknown timeline %d", timeline,0,0);
      break;
  }
}

/*===========================================================================

FUNCTION HDRTS_DISABLE

DESCRIPTION
  Disables requested timeline only if it has been enabled

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_disable(hdrts_timeline_struct_type *timeline)
{
  if(timeline->enabled == TRUE) /* Ensure called in a critical section */
  {
    vstmr_hdr_interrupt_deregister(&(timeline->evt_table));
      /* Deregister ISR with VSTMR */

    vstmr_hdr_event_tbl_deregister(&(timeline->evt_table));
      /* Deregister event table with VSTMR */

    timeline->enabled = FALSE;
      /* Timeline is disabled now */

    HDR_MSG_DRIVERS_SPRINTF_1( MSG_LEGACY_HIGH, "Disable %s",
                                                    timeline->name );
  }
  else
  {
    HDR_MSG_DRIVERS_SPRINTF_1( MSG_LEGACY_ERROR, "%s already disabled",
                                                    timeline->name );
  }
}

/*===========================================================================

FUNCTION HDRTS_DISABLE_TIMELINE

DESCRIPTION
  Allows clients to request disabling of a timeline

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_disable_timeline(hdrts_timeline_enum_type timeline)
{
  switch(timeline)
  {
    case HDRTS_MOD:
      hdrts_disable(&hdrts_mod);
        /* Disable TX timeline */
      break;

    case HDRTS_DMOD:
      hdrts_disable(&hdrts_dmod);
        /* Disable TX timeline */
      break;

    default: /* Only if forced with non enum value to timeline param */
      ERR_FATAL("Unknown timeline %d", timeline,0,0);
      break;
  }
}

/*===========================================================================

FUNCTION HDRTS_ACTIVATE_PERIODIC_TRIGGER

DESCRIPTION
  Activates a periodic event on a requested timeline

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Overwrites any prior activations silently

===========================================================================*/
void hdrts_activate_periodic_trigger
(
  hdrts_timeline_struct_type *timeline,
  uint32  first_trigger_point_in_slots,
  uint32  period_in_slots
)
{
  if(timeline->enabled == TRUE) /* Ensure called in a critical section */
  {
    timeline->hstmr.slot_cnt = period_in_slots;
    vstmr_hdr_event_sched_periodic
      (
        &(timeline->evt_table), /* Event table handle */
        HDRTS_PERIODIC_EVT_IDX, /* Event index */
        (first_trigger_point_in_slots * 2048 * 8),/* convert slots to chipx8 */
        timeline->hstmr /* Periodicity */
      );
      /* Register periodic event with VSTMR */
      HDR_MSG_DRIVERS_SPRINTF_1( MSG_LEGACY_HIGH, "Start %s periodic event",
                                                    timeline->name );
  }
  else
  {
    HDR_MSG_DRIVERS_SPRINTF_1( MSG_LEGACY_ERROR, "%s not enabled",
                                                    timeline->name );
  }
}

/*===========================================================================

FUNCTION HDRTS_ACTIVATE_PERIODIC_EVENT

DESCRIPTION
  Allows clients to request activation of a periodic event

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_activate_periodic_event
(
  hdrts_timeline_enum_type timeline,
  uint32  first_trigger_point_in_slots,
  uint32  period_in_slots
)
{
  switch(timeline)
  {
    case HDRTS_MOD:
      hdrts_activate_periodic_trigger
                      (
                        &hdrts_mod,
                        first_trigger_point_in_slots,
                        period_in_slots
                      );
        /* Activate the TX periodic event */
      break;

    case HDRTS_DMOD:
      hdrts_activate_periodic_trigger
                      (
                        &hdrts_dmod,
                        first_trigger_point_in_slots,
                        period_in_slots
                      );
        /* Activate the RX periodic event */
      break;

    default: /* Only if forced with non enum value to timeline param */
      ERR_FATAL("Unknown timeline %d", timeline,0,0);
      break;
  }
}

/*===========================================================================

FUNCTION HDRTS_DEACTIVATE_EVENT

DESCRIPTION
  Dectivates a periodic event on a requested timeline

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_deactivate_event
(
  hdrts_timeline_struct_type *timeline
)
{
  if(timeline->enabled == TRUE) /* Ensure called in a critical section */
  {
    vstmr_hdr_event_cancel(&(timeline->evt_table), HDRTS_PERIODIC_EVT_IDX);
      /* Cancel the periodic event registered with VSTMR */
    HDR_MSG_DRIVERS_SPRINTF_1( MSG_LEGACY_HIGH, "Stop %s periodic event",
                                                  timeline->name );
  }
  else
  {
    HDR_MSG_DRIVERS_SPRINTF_1( MSG_LEGACY_ERROR, "%s not registered",
                                                    timeline->name );
  }
}

/*===========================================================================

FUNCTION HDRTS_DEACTIVATE_TIMELINE_EVENT

DESCRIPTION
  Allows clients to request deactivate the events on a timeline

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrts_deactivate_timeline_event
(
  hdrts_timeline_enum_type timeline
)
{
  switch(timeline)
  {
    case HDRTS_MOD:
      hdrts_deactivate_event(&hdrts_mod);
        /* Deactivate TX timeline */
      break;

    case HDRTS_DMOD:
      hdrts_deactivate_event(&hdrts_dmod);
        /* Deactivate RX timeline */
      break;

    default: /* Only if forced with non enum value to timeline param */
      ERR_FATAL("Unknown timeline %d", timeline,0,0);
      break;
  }
}

#endif /* FEATURE_HDR_BOLT_MODEM */

/*===========================================================================

FUNCTION HDRTS_RESET_RTC

DESCRIPTION
  Resets RTC count

DEPENDENCIES
  None
 
RETURN VALUE
  None
 
SIDE EFFECTS
  None

===========================================================================*/
void hdrts_reset_rtc( void )
{
#ifndef FEATURE_HDR_BOLT_MODEM
    /* Set FCW to 0 - stops RTC */ 
    /* Set RESET_HDR in RTC_CTL to send a reset pulse */
    HWIO_OUT( HDR_RTC_FCW_CTL, 0 );
    HWIO_OUT( RTC_CTL, HWIO_FMSK( RTC_CTL, RESET_HDR ) );

    /* Program first compare counts - the mask is set to 1024 so that wake */
    /* Take it out of reset by programming the nominal FCW word. */
    HWIO_OUT( HDR_RTC_FCW_CTL, HDRTS_HDRFW_RTC_FCW_INIT );
#endif /* FEATURE_HDR_BOLT_MODEM */
} /* hdrts_reset_rtc */ 




