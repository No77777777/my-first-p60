#ifndef SRCH_COMB_H
#define SRCH_COMB_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            S E A R C H E R    C O M B I N E R

            H E A D E R F I L E


GENERAL DESCRIPTION
   This file is part of the breakup of the former file srchdrv.c. This file
   consists of all functions that deal with the symbol, power, and frequency
   combiner in the searcher subsystem.

  Copyright (c) 2001 - 2018 Qualcomm Technologies, Inc.
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

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/srch/inc/srch_comb.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/05/18   pk      Changes to slew FW before registering PN roll with VSTMR
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
05/15/14   as      Changes to support FW VPE init flag
05/02/14   bb      Changes to fix system loss issue under fading scenarios
02/04/14   as      BOLT Clean up
11/25/13   cjb     Switched logic for FEATURE_MODEM_1X_BOLT_OBSOLETE feature
10/08/13   vks     Update combiner delay to 8760 cx8
09/20/13   vks     Remove srch_scomb_set_chan() and srch_scomb_chan_enable()
09/19/13   vks     Export api to return combiner delay and slew system time
09/10/13   vks     Removed obsolete functions
08/23/13   vks     srch_comb_enable_dsp_ints obsolete on bolt
                   srch_comb_init not required as well
10/25/12   trc     Changes to define functions to avoid SYNC80/SYNC26 roll
                   over issues
11/01/10   jtm     Modem 1x SU API changes.
11/06/09   bb      Defined srch_comb_init() method
09/03/10   vks     Replace NO_FINGER with ONEX_SRCH_NO_FINGER to fix compiler
                   warning.
04/01/09   adw     Categorized included header files and standardized format.
03/31/09   adw     Cleaned up incs and eliminated implicit includes.
03/26/09   adw     Remove T_MSM6700 featurization.
03/25/09   adw     Include modem_1x_defs.h for 1x specific modem defines.
12/19/06   sst     Merge the following change from 11.02.02B branch
12/01/06   sst     Remove setting of combiner_gain, it is now controlled by AFC
10/06/05   bt      Added srch_comb_get_rtc_at_sync80.
09/08/05   ejv     Add prototypes for functions in srch_comb.c.
07/27/05   pa      Moved quick slam related functions from srchzz_tl_common
                   to here.
02/17/05   abh/gs  Removed quantization effects on the TrkLo PDM
09/20/04   ejv     Update MAX_PCTL_COMB_CNT.
09/03/04   sfm     Finger Driver changes
09/03/04   ejv     Added srch_comb_chan_type, srch_pcomb_set_init_offset and
                   srch_set_chan_lc_mask.
008/09/04   ejv     Added srch_comb_chip_symbol_type and
                   removed srch_scomb_set_siz.
06/24/04   sfm     Removed prototype for srch_pcomb_set_thrsh()
04/08/04   ejv     Mainline SRCH_HAS_TRK_LO_CNTL.
03/31/04   ejv     Added function srch_scomb_sync() for 6700.
02/20/04   sfm     Added srch_comb_chan_data typedef and updated
                   srch_comb_set_chan() prototype
01/28/04   ejv     Mainline T_MSM6500.
11/11/03   bt      Added srch_comb_trk_lo_parked.
11/05/03   sst     Modified how trk_lo_adj parameters are handled
10/20/03   bt      Added srch_comb_enable_dsp_ints function.
10/10/03   bt      Added srch_scomb_get_master_fing function.
09/29/03   dna     Use FING_SC_DELAY_VAL in multiple files
07/22/03   ejv     Updated interface to srch_comb_set_chan().
03/24/03   kwo     Added srch_scomb_set_ch0_sym_size
08/23/02   kwo     Added srch_scomb_timeline_roll
07/30/02   gs      Initial merge into perforce for cougar.
05/30/02   ejv     Mainlined T_MSM5000.
05/24/02   ejv     Mainlined FEATURE_IS2000_REL_A_VP2.
03/15/02   ejv     Added FEATURE_IS2000_REL_A_VP2.
02/27/02   ejv     Modified date format in change history comments.
02/06/02   ejv     Added srch_comb_set_chan().
01/14/02   SST     Merged into tip to replace srchdrv.c
01/11/02   SST     Completed function name changes.
                        srch_comb_ttrk_ena -> srch_scomb_ttrk_ena
                        srch_comb_ttrk_dis -> srch_scomb_ttrk_dis
                        srch_comb_arm_slam -> srch_scomb_arm_slam
                        srch_comb_set_pn_phs -> srch_scomb_set_pn_phs
                        srch_comb_slam_timeline -> srch_scomb_slam_timeline
                        srch_set_sym_siz -> srch_scomb_set_siz
                        srch_get_ref_time -> srch_scomb_get_ref_time
                        srch_get_vsc_time -> srch_scomb_get_vsc_time
                        srch_clr_manual_roll -> srch_scomb_clr_manual_roll
                        srch_gen_manual_roll -> srch_scomb_gen_manual_roll
                        srch_comb_ftrk_acq -> srch_fcomb_ftrk_acq
                        srch_set_carrier_freq -> srch_fcomb_set_carrier_freq
                        srch_get_carrier_freq -> srch_fcomb_get_carrier_freq
                        srch_set_frq_trk_acc -> srch_fcomb_set_ftrk_acc
                        srch_get_ftrk_acc -> srch_fcomb_get_ftrk_acc
                        srch_park_trk_lo_adj -> srch_fcomb_park_trk_lo_adj
                        srch_pctl_ena -> srch_pcomb_ena
                        srch_pctl_rate_set -> srch_pcomb_rate_set
                        srch_set_pwr_comb_thrsh -> srch_pcomb_set_thrsh
01/10/02   SST     finished move for srchdrv.c breakup
01/01/02   SST     Created Module - refer to srch_hw.c for earlier edit
                      history.

==========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/* Common */
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"

/* Srch */
#include "srchi.h"
#include "srchi_i.h"
#include "ulpn.h"

/* Other */
#include "qw.h"


/*===========================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================
      Constants
===========================================================================*/

#define MAX_PCTL_COMB_CNT    6
  /* Max number of power control symbol combiners suppoted by hardware */

#define FING_SC_DELAY_VAL                          0x1bf3e7L
  /* 0x1bf3c1 = 0x1c0000 - 0xc1a + 1...
     0xc1a = 3098 = 387.25 chips * 8 chipx8/chip...
     delay to be used to set the combiner position
     relative to the finger being slammed to. */


/* offset in cx8 = 8*(327+64+384[is qlic_delay]+320[is bolt_delay] */
#define FING_COMB_OFFSET_CX8     8760     /* offset between comb. and finger */
#define FING_COMB_OFFSET_CHIPS   1095     /* offset between combiner and
                                             finger in chips */

/*===========================================================================
      Typedefs
===========================================================================*/

typedef enum
{
  COMB_CHAN_0 = 0,  /* Indicates CH0 */
  COMB_CHAN_1,      /* Indicates CH1 */
  COMB_CHAN_2,      /* Indicates CH2 */
  COMB_CHAN_3,      /* Indicates CH3 */
  COMB_CHAN_4,      /* Indicates CH4 */
  COMB_CHAN_5,      /* Indicates CH5 */
  COMB_CHAN_6,      /* Indicates CH6 */
  COMB_CHAN_7,      /* Indicates CH7 */
  COMB_CHAN_8       /* Indicates CH8 */
}
srch_comb_chan_type;

typedef enum
{
  CHIP_SYMBOL_64,   /* Indicates  64 chips per symbol */
  CHIP_SYMBOL_128,  /* Indicates 128 chips per symbol */
  CHIP_SYMBOL_256,  /* Indicates 256 chips per symbol */
  CHIP_SYMBOL_512   /* Indicates 512 chips per symbol */
}
srch_comb_chip_symbol_type;

/* Combiner Channel Data for srch_comb_set_chan() */
typedef struct
{
  boolean vp2;         /* Is VP2 mode active? */
  uint8   num_scch;    /* Number of SCCH channels */
  uint8   num_sch;     /* Number of SCH channels */
  uint8   num_pdcch;   /* Number of PDCCH channels */
}
srch_comb_chan_data;


/*===========================================================================

                    FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

FUNCTION SRCH_SCOMB_GET_MASTER_FING

DESCRIPTION    This function returns the finger that we should be time tracking
               as far as triage is concerned.

DEPENDENCIES   None

RETURN VALUE   master finger or ONEX_SRCH_NO_FINGER

SIDE EFFECTS   None

===========================================================================*/
extern uint16 srch_scomb_get_master_fing(void);

/*===========================================================================

FUNCTION SRCH_SCOMB_TTRK_ENA

DESCRIPTION    This function enables the symbol combiner time tracking
               The combiner tries to keep its time aligned with a given
               reference finger (typically the earliest one) by slewing to
               match the finger's movements.  Finger time tracking has a
               strictly limited (by the CAI) rate.

               If a slam has been armed by "srch_comb_arm_phase_slam",
               setting the timetrack reference finger would alter the slam
               operation. Instead, the finger reference is merely stored, to
               be set once the slam takes effect.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void  srch_scomb_ttrk_ena
(
  uint16  fing /* Reference(earliest) finger to be tracked by the combiner */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_TTRK_DIS

DESCRIPTION    This function disables the symbol combiner time tracking.

               If a slam has been armed by "srch_comb_arm_phase_slam", clearing
               the timetrack reference finger would alter the slam operation.
               Instead, the timetrack disable operation is remembered, to be
               executed once the slam takes effect.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void  srch_scomb_ttrk_dis ( void );

/*===========================================================================

FUNCTION SRCH_SCOMB_ARM_SLAM

DESCRIPTION    This function arms the symbol combiner to align itself in time
               with ("slam to") the specified finger timing. The latch happens
               immediately.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void  srch_scomb_arm_slam
(
  uint16  f_ref      /* The reference finger to align combiner timing with */
);

/*===========================================================================

FUNCTION       SRCH_SCOMB_QUICK_SLAM_TO_FING

DESCRIPTION    This function quick slams the symbol combiner immediately to
               a given finger in addition to a user specified slew.

DEPENDENCIES   The finger that is slammed to must not be slewing when this
               function is executed. Otherwise the slam will fail and timing
               will be in an undefined state.

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_scomb_quick_slam_to_fing
(
  int8  fing,             /* finger to quick slam to                       */
  int32 slew,             /* additional slew value                         */
  int8  phase             /* phase to quick slam to                        */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_SET_PN_PHS

DESCRIPTION    This function sets the symbol combiner to a desired
               phase80 value.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_scomb_set_pn_phs
(
  uint16 phase             /* Phase of 80ms we want to set the combiner to */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_SLEW_SYSTEM_TIME

DESCRIPTION    This function slews the combiner time

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_scomb_slew_system_time
(
  int32 slew,              /* Amount to slew the sytem time                */
  boolean vpeInit          /* Flag to control VPE initialization           */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_SLAM_TIMELINE

DESCRIPTION    This function arms the symbol combiner to align itself in time
               with ("slam to") the specified finger timing. The combiner phase
               is set to the given phase after the slam takes effect.

DEPENDENCIES   Neither the symbol combiner nor the finger we are slamming to
               should be about to roll.

RETURN VALUE   None

SIDE EFFECTS   Noen

===========================================================================*/
extern void  srch_scomb_slam_timeline
(
  uint16  f_ref,     /* The reference finger to align combiner timing with */
  uint16  new_phase, /* Desired phase after slam takes effect              */
  uint16  new_offset /* Offset from system timing, in 2-chip units         */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_TIMELINE_ROLL

DESCRIPTION   This function completes the slam timeline on a PN Roll.  It
              should be called only from ISR context after a slam_timeline is
              armed.

DEPENDENCIES   srch_scomb_slam_timeline should have been previously called.

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void srch_scomb_timeline_roll(void);

/*===========================================================================

FUNCTION SRCH_SCOMB_GET_REF_TIME

DESCRIPTION    This function reads the symbol combiner value

DEPENDENCIES   None

RETURN VALUE   The 16-bit time reference register value.  Note that this
               value is not exactly binary.  It is binary encoded, but its
               maximum value is 0xBFFF.  Each LSB is two PN chips.  The top
               two bits are the 26.666... ms phase within an 80ms superframe.

SIDE EFFECTS   None

===========================================================================*/
extern uint16  srch_scomb_get_ref_time ( void );

/*===========================================================================

FUNCTION SRCH_SCOMB_GET_VSC_TIME

DESCRIPTION    This function reads the "virtual symbol combiner's" time.

DEPENDENCIES   None

RETURN VALUE   The 16-bit time reference register value.  Note that this value
               is not exactly binary.  It is binary encoded.  Each LSB is two
               PN chips.  The value returned may correspond to a position
               larger than 80ms.

SIDE EFFECTS   None

===========================================================================*/
extern uint16  srch_scomb_get_vsc_time ( void );

/*===========================================================================

FUNCTION SRCH_SCOMB_GET_FING_SC_POS

DESCRIPTION    This function fills in the finger and symbol combiner PN
               counts for the given finger.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_scomb_get_fing_sc_pos
(
  int8    fing,       /* finger to quick slam to */
  int32 * f_cnt_ptr,  /* ptr to finger PN counter value in Cx8 */
  int32 * sc_cnt_ptr  /* ptr to symbol combiner count in Cx8 */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_GET_COMBINER_DELAY_CX8

DESCRIPTION    This function returns the combiner delay (offset beteen finger
               and combiner) in chipx8.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern uint32 srch_scomb_get_combiner_delay_cx8( void );

/*===========================================================================

FUNCTION SRCH_SCOMB_SET_ULPN_STATE

DESCRIPTION    This function sends the demod long code state to firmware.

DEPENDENCIES   None.

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void  srch_scomb_set_ulpn_state
(
  ulpn_type  lc_state
);

/*===========================================================================

FUNCTION SRCH_GET_ULPN_STATE

DESCRIPTION    This function returns the User Long PN Code state.

DEPENDENCIES   Calling this function asynchronously with the 26.666... ms
               boundary is dangerous.  If the 26.666... ms boundary is close
               by, this function waits until the roll over happens before
               returning.

RETURN VALUE   No formal return value,  but the User Long PN Code state is
               returned in a ulpn_type passed in by reference.  This state is
               the one that was valid at the last 26.666... ms boundary.

SIDE EFFECTS   None

===========================================================================*/
extern void srch_scomb_get_ulpn_state
(
  ulpn_type   *ulpn  /* Pointer to where to put the ULPN value read from the
                        demod */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_SET_LC_MASK

DESCRIPTION    This function sets the demod long code mask passed in to all
               the channels. To set the mask to a specific channel use
               srch_scomb_set_chan_lc_mask()

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void  srch_scomb_set_lc_mask
(
  qword pn_mask       /* PN long code mask to load into the mask register */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_SET_CHAN_LC_MASK

DESCRIPTION    This function sets the demod long code mask to a value for the
               specified channel.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void  srch_scomb_set_chan_lc_mask
(
  srch_comb_chan_type chan,                   /* Channel number to load    */
  qword pn_mask                               /* PN long code mask to load */
);

/*===========================================================================

FUNCTION SRCH_SCOMB_ROLL_CHECK

DESCRIPTION    This function check if a pn roll isr is pending

DEPENDENCIES   None

RETURN VALUE   TRUE if a ROLL has been signalled

SIDE EFFECTS   None

===========================================================================*/
extern boolean srch_scomb_roll_check( void );

/*===========================================================================

FUNCTION       SRCH_SCOMB_GET_RTC_AT_SYNC80

DESCRIPTION    This function returns the RTC at the SYNC80 position of the
               symbol combiner.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern int32 srch_scomb_get_rtc_at_sync80( void );

/*===========================================================================

FUNCTION       SRCH_COMB_RESOLVE_SYNC80_ROLLOVER

DESCRIPTION    This function ensures that next SYNC80 is atleast cx1_margin
               away, and if it is less than cx1_margin, then it explicitly waits
               until SYNC80 roll over has happened and returns controls back
               to calling functions to avoid race conditon issues.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_scomb_resolve_sync80_rollover
(
  uint32 cx1_margin
);

/*===========================================================================

FUNCTION       SRCH_COMB_RESOLVE_SYNC26_ROLLOVER

DESCRIPTION    This function ensures that next SYNC26 is atleast cx1_margin
               away, and if it is less than cx1_margin, then it explicitly waits
               until next SYNC26 roll over has happened and returns controls back
               to calling functions to avoid race conditon issues.

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
extern void srch_scomb_resolve_sync26_rollover
(
  uint32 cx1_margin
);
/*===========================================================================
FUNCTION       SRCH_SCOMB_QUICK_SLAM_TO_FING

DESCRIPTION    This function calculates distance between time tracking finger
               and symbol combiner, and make sure that sombiner is tracking
               master finger within FING_COMB_OFFSET_CX8 value

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None
===========================================================================*/
extern void srch_scomb_check_dist_to_fing
(
  boolean apply_slew,
  boolean vpe_init
);

#endif /* SRCH_COMB_H */
