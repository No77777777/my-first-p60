/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 H D R    S R C H    R E A C Q    S T A T E

                             Search ReAcq Module

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2002 - 2015 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchreacq.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
05/27/2016   vko     QSH framework for events capturing
08/25/2015   wsh     Handle tune_to_chan failure.
08/07/2015   wsh     Prevent chipx16 cb blocking deactivate 
06/26/2015   vke     Added timeout value for tune_to_chan()
06/17/2015   wsh     Fixed deactivate not processed in REACQ
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
02/16/2015   vke     Changes to use the Aset win size shared via IRAT
01/16/2015   ljl     Retry deactivate cmd before RF warmup done.
12/04/2014   arm     Changes to support new unified micro priority changes
12/01/2014   mbs     Updated mode preference if HDR is in DR mode
10/31/2014   arm     Updated DR mode after 5->3f transition.
10/01/2014   wsh     Added Full PN search for reacq and IRAT searches
08/27/2014   bb      Changes to send DSDA STATE when DO enters reacquisition
                     from TIME SYNC state
08/20/2014   arm     Disable Tx after qta if doing OFS or channel change.
08/15/2014   mbs     Supported fall-back to TRM R&N when trm_exchange fails
08/07/2014   mbs     Added support for trm lock exchange during IRAT procedures
07/22/2014   bb      Changes to fix RXLM buffer leakage issue during QTA
07/17/2014   bb      Change to fix watchdog timeout issue when QTA is enabled
06/04/2014   arm     Changes to support DO to G QTA
10/14/2013   arm     Send freq id list again to FW in Reacq state
05/12/2014   vke     Klockworks fix
05/12/2014   mbs/vlc Moved hdrsrchset_reacq_reset_filters() to end of reacq init.
11/26/2013   arm     Handle SSSS->DSDA transition in online mode.
11/15/2013   smd     Seached ASET only for fast acq
10/07/2013   rmv     Replaced ASSERTs on external/global variables as per 
                     go/assert guidelines 
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
08/29/2013   rkc     Removed checking for RF granted sleep check.
08/21/2013   rkc     After reacq time is valid, check if sleep is necessary 
                     if rf granted before RF wait timeout. 
08/12/2013   rmg     Added fix for FPD being skipped.
05/24/2013   rmg     Added D-LNA support.
05/23/2013   wsh     Fixed deactivate received before chipx16 not working
04/22/2013   arm     Added changes for DSDA coexistence manager. 
05/02/2013   ljl     Configured FPD 1ms after sleep end.
04/25/2013   wsh     Fixed deactivate not processed due to bad RF cal
04/29/2013   dsp     Added code to update prev_cc_start in reacq_init().
03/21/2013   kss     Remove unused legacy code.
03/18/2013   arm     Used Pilot trk srch mode when no reacq rxd.
03/19/2013   vke     Fix for preventing AT decoding QPCH during RAHO 
                     scenarios when QPCH feature is not enabled 
03/18/2013   smd     Added ForcePreamDetect field in QPCH msg for Triton.
03/15/2013   smd     Disabled using new FW fields in QPCH msg for Triton. 
01/10/2013   grl     Disable QPCH force preamble detection after RAHO.
01/10/2013   grl     Disable FPD when SCC offset was not known at sleep time  
12/03/2012   grl     Disable QPCH preamble detection after the sync capsule 
                     preamble slots 
02/16/2013   arm     Do default search if enabling div for reacq timeouts
                     or current rtc and pilot pos are not within 1 ms.
02/14/2013   dsp     Decreased the reacq srch window size to 320 chips.
02/14/2013   vko     Fixed AT not decoding Quick page message
10/30/2012   dsp     Skipped reporting slew to slpc if sleep dur. <= 64 slots.
10/19/2012   smd     Told fw whether the standby to active transition is for wakeup.
09/27/2012   ljl     Skipped error feedback to slpc temporarily.
07/26/2012   arm     Removed hdrsrchrfdiv_set_fl_data_adaptive_rxd_allowed 
                     function as it is not needed with new ARD state mc. 
07/09/2012   dsp     Removed unnecessary F3s.
07/05/2012   arm     Removed Resume AGC as its no longer needed. 
06/21/2012   grl     Do not do FPD if past the CC boundary in intrascc. 
05/25/2012   wsh     Fixed FPD cancel related crashes
05/20/2012   arm     Fix idle div featurization bug.
04/02/2012   arm     Supported diversity in reacq. 
05/01/2012   ljl     Reset filter after failed reacq search. 
04/30/2012   dsp     Fixed compiler warnings on Dime. 
03/29/2012   smd     Added DIME sleep updates.
03/19/2012   smd     Added aleep controller API updates for DIME. 
02/22/2012   arm     Fixed Idle Div bug for intra scc sleep. 
02/02/2012   arm     Changed idle div to record only max 255 psinr for any
                     idle wakeup.
02/06/2012   arm     Resolved size conflict in function definitions in hdrsrch.  
02/06/2012   wsh     Delay preparing search list after tune_done callback
01/23/2012   arm     Idle Diversity bug fixes.
01/11/2012   ljl     Added the parameter for search priority to hdrsrchdrv_enable_srch4().
11/11/2011   arm     HDR idle and access state diversity implemetation. 
11/11/2011   smd     Fixed bugs of search window size adjustment.  
11/08/2011   smd     Reported slew to sleepctl when reacq succeeds.
11/07/2011   ljl     Disabled WUTA.
10/24/2011   wsh     Updated cancel FPD procedure 
07/12/2011   arm     Resolved size conflict in function definitions in hdrsrch.  
06/13/2011   ssu     Replaced time_sync_time_of_day() with 
                     hdrts_update_ts_time() in hdrsrchreacq_on_time_valid(). 
05/12/2011   ljl     Removed sleep clk estimate and slew update to sclk.
04/12/2011   smd     Used srch4 search structure.
12/09/2010   pxu     Moved extern sleep algorithm function prototypes to hdrsrchstates.h 
11/21/2010   pxu     Added code to readjust search window size on next searches. 
11/12/2010   ljl     Moved fw enable to reacq state. 
10/14/2010   ljl     Supported FPD enable/disable.
04/06/2010   rkc     Fixed HDR_forcePreamType i/f to 1 and 2 for rates 1 and 2.
06/25/2010   smd     Supported hstr slam in slam cfg message.
06/23/2010   smd     Don't limit window size in debug mode.
06/04/2010   smd/vlc Added changes to transition to Idle Monitor state upon 
                     successful fast acq plus minor function name change.
05/24/2010   smd/vlc Added changes to get srch dump in Fast Acq mode.
05/10/2010   smd     Returned HDRSRCH_FAST_ACQ_SUCCEEDED if fast acq succeeds.
04/15/2010   smd     Added support for fast_acq.
04/09/2010   ljl/lyl Fixed the state comparison in hdrsrchreacq_init().
03/24/2010   pxu     Removed FEATURE_HDR_QSM features and 
                             FEATURE_HDR_IDLE_TIMELINE_V3 checks.
02/21/2010   kss/lyl Enable AGC immediately after RTC on.
02/20/2010   lyl     Updated for initial slotted mode functional.
02/09/2010   smd     Added feature of LTE to DO measurement and reselection.
11/25/2009   rkc     Added time_sync_time_of_day() to hdrsrchsync_on_time_valid
09/30/2009   smd     Fixed KW errors.
07/16/2009   lyl     Supported CC config MSGR interface.
07/14/2009   ljl     Supported EAFT.
07/06/2009   lyl     Fixed the compilation issue in RUMI build.
06/23/2009   lyl     Fixed the lint errors.
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
05/18/2009   ljl/lyl Updated for firmware MSGR interface.
04/21/2009   etv     Replaced obsolete hdrrx APIs with hdrdec APIs.
03/18/2009   rkc     Removed rf.h.
02/06/2009   smd     Initialized n in function hdrsrchreacq_deactivate().
01/30/2009   ljl     Searched max 8 pilots at the first reacq search.
01/26/2009   lyl     Logged the number of reacq search pilots in enh_sleep_info..
12/12/2008   smd     Do not request OFS if reacq fails after intra-scc sleep
12/02/2008   rkc     Added FEATURE_HDR_QUICK_IDLE back for 7K targets.
                     Fixed lint errors.
11/20/2008   rkc     Removed FEATURE_HDR_QUICK_IDLE ifdef check
11/17/2008   grl     Added Enh Sleep Info version 1 support.
11/13/2008   lyl     Renamed to the more meaningful function name of 
                     hdrsrchsleep_is_qpch_enabled().
10/13/2008   lyl     Fixed lint errors.
10/28/2008   smd     Fixed tab special character and indent issues.
10/09/2008   lyl     Added QPCH support.
10/02/2008   smd     Added improvement for emergency RAHO.
10/01/2008   smd     Dereferenced proper aset sector pointer for debug message.
09/23/2008   rkc     Used srch_list's res_ptr instead of resbuf.
09/18/2008   rkc     Added Intra-SCC support.
08/29/2008   lyl     Set the search task type when preparing the search list.
08/19/2008   smd     Added idle handoff improvement.
07/25/2008   rkc     Added non-best NSET pilots to srch list if possible.
07/15/2008   lyl     Fixed lint error (525: Negative indentation)
03/28/2008   grl     Schedule AGC start after the tune if RTC is on.
02/25/2008   grl     Defer deactivation until after second sleep int.
02/21/2008   ljl     Preferred the same subnet handoff.
02/08/2007   jyw     Matched the deadtime F3 to the deadtime logging.
01/30/2008   jyw     Included demod index assignment in the RF interfaces.
11/13/2007   jyw     Added the power strobe event id.
10/17/2007   jyw     Merged Rev B changes from branch.
09/27/2007   grl     Added support for the reva sleep_info pkt.
08/05/2007   grl     Fixed WUTA estimate made when coming from suspended mode.
08/19/2007   grl     Resolved Lint High warnings.
08/08/2007   mt      Do not reset OFS time after REACQ failure.
06/30/2007   mt      Perform OFS after REACQ failure.
06/20/2007   rkc     Changed hdrsrchreacq_process_int_cmd() input to payload ptr
06/02/2007   grl     Implemented the WUTA Algorithm.
01/17/2007   ljl     Added HDR SRCH AFC stm.
12/07/2006   mt      Disable FPD if scc offset is not known.
12/05/2006   grl     Changed the SyncCC HSTR to be earlier.
11/30/2006   grl     Disabled FPD after RAHO.
10/31/2006   mt      Renamed hdrsrchset_reset_filters().
10/26/2006   etv     Added support for on-arrival delivery of RX packets.
10/20/2006   kss     Adjust RTC for sleep slots before RTC is enabled.
10/04/2006   mt      Forced preamble detection/ Missed CC ISR changes.
09/19/2006   grl     Give the protocols control over reacq system loss.
09/14/2006   mt      Updated forced preamble detection for interface change.
08/28/2006   sq      Added support for FEATURE_HDR_QSM
08/23/2006   grl     Turn off DAST during BCMCS.
08/21/2006   grl     Implemented DAST.
08/16/2006   mt      Forced Preamble Detection algorithm changes.
08/15/2006   mt      Improved the accuracy of sleep deadtime calculation.
08/07/2006   mt      AGC acq interface change in hdrsrchreacq_enable_and_tune().
08/01/2006   mt      Changes for HDR AGC acquisition. 
07/27/2006   mt      Fixed and moved forced preamble detection here.
07/24/2006   jyw     Added power profiling strobes.
06/28/2006   jyw     Added support to MPC.
07/10/2006   mt      Added support for sleep timeline optimization.
06/15/2006   sq      Added FEATURE_HDR_AGC_OPTIMIZATION_P1
06/15/2006   grl     Fixed compilation warning.
06/07/2006   ljl     Added SBHO optimization
05/12/2006   sq      Activate decoder at tune_done 
03/06/2006   sq      Added wake-up reason in wake-up callback
12/02/2005   sq      Added new re-acq result enum
10/18/2005   sq      Added FEATURE_HDR_REVA_L1
09/28/2005   sq      Set the CC_START when waking up
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
06/28/2005   jyw     Added probes for HDR sleep time line profiling
05/31/2005   grl     Added support for TRM and TCXOMgr.
03/31/2005   grl     Changed HDR's use of trk_lo to be GPS friendly
03/11/2005   sq      Handled HDRSRCH_UPDATE_BC_INFO_CMD in reacq state
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
08/24/2004   ajn     Error feedback is now in sysclk's (chipx8's)
08/12/2004   ajn     sleephdr -> sleepctl
07/22/2004   ajn     Added common signals to state machine
06/24/2004   sq      Bug fix for REACQ INFO LOG packet
06/08/2004   sq      Bug fix for SLEEP INFO LOG packet
06/07/2004   aaj     Replaced hdrsrchidle_init_idle_state with new function
03/04/2004   aaj     Support for minimizing subnet handoffs 
12/02/2003   sq      Merged in code review comment
12/01/2003   sq      Stop sleep_info logging after re-acq failure
11/18/2003   sq      Increments search_list_cnt for every search dump
11/11/2003   sq      Supported new log packet ( REACQ_INFO )
10/21/2003   aaj     Compiler warning cleanup
10/06/2003   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
09/18/2003   ajn     Merge from Jaguar orphan branch
09/10/2003   mpa     Converted F3 messages to use new MSG2.0 HDR search SSID
08/26/2003   ajn     Support HSTR range increase from 512 to 65536
03/20/2003   ajn     Only update slow clock estimate if reacq after sleep.
03/11/2003   ajn     Replaced adjust_sci( ) with alter_sci( ), to allow
                     SCI to climb rapidly if slews are small.
03/11/2003   ajn     Updated Copyright notice
11/04/2002   aaj     Remove FEATURE_HDRSRCH_PROFILE. F3 change
10/30/2002   aaj     F3 change
10/16/2002   sst     Fixed reacq error when all nset pilots are ofreqs
10/01/2002   aaj     Activate decoder clock when switched from 3a to 3f
09/18/2002   aaj     Support for multipath window centering for HDR
06/13/2002   aaj     Do not reset HSCI=0 after first reacq search failure
03/12/2002   ajn     Enable Demod after reacq tune done. 
                     Set timevalid flag if chipx16 is already on (sus->reacq)
02/21/2002   ajn     Altered hdrsrchutil function signatures.
                     Removed enum_to_winsize table.
02/19/2002   sst     Modified how search list is filled
02/19/2002   sst     Add multiple reacq searches
02/16/2002   aaj     Support for multiple CCC sleep. Fixed deactivate_pending
02/06/2002   ajn     hdrerrno -> errno
01/16/2002   ajn     Created this file (split from hdrsrchsleep.c)

==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchdef.h"
#include "hdrsrchi.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchset.h"
#include "hdrsrchfing.h"
#include "hdrsrchlog.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchafc.h"
#include "hdrsrchmsg.h"

#include "hdrcmac.h"
#include "hdrts.h"

#include "time_svc.h"

#include "hdrrx.h"
/* included this file temporarily to allow RX to reset seq nos on wakeup
   replace this with hdrrxsrch.h which defines searcher-RX interfaces */

#include "hdrdec.h"

#ifdef FEATURE_HDR_DIME_MODEM
#include "slpc.h"
#else
#include "sleepctl.h"
#endif /* FEATURE_HDR_DIME_MODEM */

#include "hdrmdspmcr.h"

#include "rf.h"

#include "hdrdebug.h"
#include "err.h"

#include "tcxomgr.h"


#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
#include "hdrsrchidlei.h"
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#include "hdr_log_qsh.h"
#include "hdrutil.h"

#define HDR_SRCH_MAX_INTRA_SCC_SLEEP_SLOTS 64
/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/


#define HDRSRCHREACQ_MAX_WINDOW_SIZE   HDRSRCH_WINSIZE_320_CHIPS
#define HDRSRCHREACQ_MIN_WINDOW_SIZE   HDRSRCH_WINSIZE_80_CHIPS

#define HDRSRCHREACQ_HS_PER_SCC        512
  /* Half-Slot count per synchronous control channel capsule */

#define HDRSRCHREACQ_HS_PER_SCC_MASK       ( HDRSRCHREACQ_HS_PER_SCC - 1 )
  /* Mask for Half-Slot count within an SCC cycle */


#define HDRSRCHREACQ_MAX_SCC_OFFSET         3
#define HDRSRCHREACQ_QPCH_MACIDX            71
#define HDRSRCHREACQ_FPD_HSTR_TOO_EARLY     470
#define HDRSRCHREACQ_FPD_HSTR_TOO_LATE      5
#define HDRSRCHREACQ_FPD_HSTR_MARGIN        63U

  /* These macros define the range of acceptable hstr 
     values to enable Forced Preamble Detection */
#define HDRSRCHREACQ_FPD_HSTR_EARLY_DELTA   \
      ( HDRSRCHREACQ_HS_PER_SCC - HDRSRCHREACQ_FPD_HSTR_TOO_EARLY )
  /* The acceptable range of HSTR value to enable Sub-sync CC FPD */
#define HDRSRCHREACQ_QPCH_BEFORE_CC_IN_HS   2
  /* QPCH packet is 2 half slots earlier than CC packet */

#define HDRSRCHREACQ_MIN_SRCH_ATTEMPTS      3
  /* Minimum number of reacq srch attemps for failed searches.  May search more
     if neighbor list is larger to ensure all neighbors are searched. */

#define HDRSRCHREACQ_INTRA_SCC_EXTRA_SRCH_SLOTS     4
  /* Number of extra slots to search during intra-SCC reacq failure */

#define HDRSRCH_FIRST_REACQ_MAX_SRCH_TASKS_CNT      8
  /* Maximum number of the search tasks in the first reacq search */

#define HDRSRCHREACQ_FPD_TIMER_MS                   1
  /* 1 ms after sleep end */

#define HDRSRCHREACQ_BAND_GRANT_TIMEOUT_MS         (0)
  /* Timeout waiting for band grant */

/* <EJECT> */
/*==========================================================================

                     HDR SEARCH REACQ STATE DATA

As much IDLE data is lumped together in one structure, so that it becomes
possible to dynamically allocate acquisition data memory during the
IDLE state, and discard the memory when no longer needed.

This will become important when RAM is based on SDRAM.

All variables are accessed as "hdrsrchreacq.member_name".  When dynamic
allocation is implemented, a pointer to the structure is needed instead.
At this point, hdrsrchacq must become a dereferenced pointer, instead of
a variable.  Ie)

  #define hdrsrchreacq  (*hdrsrchreacq_struct_ptr)

==========================================================================*/

/*---------------------------------------------------------------------
   Reacq States
---------------------------------------------------------------------*/

typedef enum
{
  HDRSRCHREACQ_READY_TO_TUNE_STATE,
  HDRSRCHREACQ_WARMUP_STATE,
  HDRSRCHREACQ_TUNE_STATE,
  HDRSRCHREACQ_WAIT_FOR_DIV_STATE,
  HDRSRCHREACQ_SEARCH_STATE,
  HDRSRCHREACQ_TUNE_FAILED_STATE
}
hdrsrchreacq_state_enum_type;

/*---------------------------------------------------------------------
   Reacq State data structure
---------------------------------------------------------------------*/

typedef struct
{
  hdrsrchreacq_state_enum_type                state;
    /* State of the sleep controller */

  hdrsrch_wakeup_cb_type                      wakeup_cb;
    /* Wakeup from Sleep callback */

  hdrsrch_fast_acq_cb_type                    fast_acq_cb;
    /* Fast acq call back */

  boolean                                     reacquired;
    /* Flag indicating that reacquisition was successful */

  boolean                                     deactivate_pending;
    /* Flag to indicate that a DEACTIVATE command is pending */

  uint8                                       search_list_cnt;
    /* Counter for number of reacq searches */

  uint8                                       nset_pilots_searched;
    /* Number of nset pilots searched in reacq searches */

  boolean                                     slept;
    /* Flag to indicate whether we slept (came from state 3e) or
       if we didn't sleep (came from some other state, like 3a) */

  boolean                                     chipx16_on;
    /* Chipx16 is on */

  boolean                                     forced_preamble_activated;
    /* whether forced premable has been activated or not */ 
  
  uint8                                       win_size_fctr;
    /* adjustment to the search window size on next reacq search */

  uint8                                       max_win_size_fctr;
   /* maximum search window size adjustment */

  int16                                       slew_x2;
   /* Reacq slew in chipx2 */

  timer_type                                  fpd_timer;
    /* Timer to schedule preamble detection in FW. */

}
hdrsrchreacq_struct_type;



LOCAL hdrsrchreacq_struct_type    hdrsrchreacq;



/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_DEACTIVATE

DESCRIPTION
  This function completes the DEACTIVATE command

DEPENDENCIES
  This function may be called in interrupt context.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Transitions to the start state.

===========================================================================*/

LOCAL void hdrsrchreacq_deactivate( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_complete_cmd();
    /* Deactivate command is now complete */

  hdrsrchreacq.deactivate_pending = FALSE;
    /* mark deactivate pending as false since we have acted upon this */

  hdrsrchst_init_inactive_state( );
    /* And transition back to start state */

} /* hdrsrchreacq_deactivate */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_RESET_WINSIZEFACTOR

DESCRIPTION
  This function sets the winsizefactor field in hdrsrchreaq to 1;

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
void hdrsrchreacq_reset_winsize_fctr( void )
{
  hdrsrchreacq.win_size_fctr = 1;
}

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_CALC_MAX_WIN_FACTOR

DESCRIPTION
  This function caculates the factor required to maximize a window size;

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
void hdrsrchreacq_calc_max_win_factor
( 
   hdrsrch_winsize_enum_type winsize 
    /* window size enum */
)
{
  uint16 max_winsize_x1;
    /* max win size in chipx1 */

  uint16 cur_winsize_x1;
    /* win win size in chipx1 */

  uint4 win_size_factor;
    /* calculated window size factor */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  max_winsize_x1 = hdrsrchutil_get_winsize_x1(HDRSRCHREACQ_MAX_WINDOW_SIZE);

  cur_winsize_x1 = hdrsrchutil_get_winsize_x1(winsize);

  win_size_factor = 1;

  while( cur_winsize_x1 < max_winsize_x1 )
  {
    cur_winsize_x1 *= 2;
    win_size_factor *= 2;
  }
  hdrsrchreacq.max_win_size_fctr  = win_size_factor;
}

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_PREPARE_SEARCH_LIST

DESCRIPTION
  This function handles the preparation of the reacq search list.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Modifies nset_pilots_searched.

===========================================================================*/

void hdrsrchreacq_prepare_search_list( void )
{
  int                             max;
    /* Maximum number of neighbours that can fit in the reacq list */

  uint8                           n_aset = 0;
    /* Number of ASET pilots added to the reacquisition search list */

  uint8                           n_cset = 0;
    /* Number of CSET pilots added to the reacquisition search list */

  uint8                           n_nset = 0;
    /* Number of neighbours added to the reacquisition search list */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_init_search_list( );
    /* Reset the search list */

  /* Set the search task type before programming the the search list */
  if ( hdrsrchrfdiv_diversity_demod_is_enabled( ) ) 
  {
    hdrsrchdrv.srch_task_type = HDRSRCH_DIVERSITY_SRCH;
  
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "Unexpected search task type in reacq: Diversity SRCH!");
  }
  else
  {
    hdrsrchdrv.srch_task_type = HDRSRCH_NON_DIV_SRCH;
  }

#ifdef FEATURE_HDR_REVB
  n_aset = hdrsrchutil_add_aset_to_search_list( hdrsrch.aset_channel, 1);
    /* Add the ASET to the reacquisition list */
#else
  n_aset = hdrsrchutil_add_aset_to_search_list( &hdrsrch.aset_channel );
    /* Add the ASET to the reacquisition list */
#endif /* FEATURE_HDR_REVB */

  /* Only search for ASET pilot if intra-scc is enabled. */
  if ( ( hdrsrchsleep_get_intra_scc_valid() == FALSE ) &&
       ( hdrsrchreacq.fast_acq_cb == NULL ) )
  {

#ifdef FEATURE_HDR_REVB
    n_cset = hdrsrchutil_add_cset_to_search_list( hdrsrch.aset_channel, 1,
                                       hdrsrchset.cset_cnt );
      /* Add the CSET to the reacquisition list */
#else
    n_cset = hdrsrchutil_add_cset_to_search_list( &hdrsrch.aset_channel );
      /* Add the CSET to the reacquisition list */
#endif /* FEATURE_HDR_REVB */


    if ( hdrsrchreacq.search_list_cnt == 0 )
    {
      /* 1st search */

#ifdef FEATURE_HDR_REVB
      max = HDRSRCH_FIRST_REACQ_MAX_SRCH_TASKS_CNT                -
            hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt -
            hdrsrchset.cset_cnt;
        /* Compute maximum number of nset pilots that can fit in list */
#else
      max = HDRSRCH_FIRST_REACQ_MAX_SRCH_TASKS_CNT - 
            hdrsrchset.aset_cnt                    - 
            hdrsrchset.cset_cnt;
        /* Compute maximum number of nset pilots that can fit in list */
#endif /* FEATURE_HDR_REVB */

      if ( max > 0 ) 
      {
#ifdef FEATURE_HDR_REVB
        n_nset = hdrsrchutil_add_best_nset_to_search_list( &hdrsrch.aset_channel[0], 
                                                           (uint8) max );
          /* Add the strongest nset pilots to the search list */
#else
        n_nset = hdrsrchutil_add_best_nset_to_search_list( &hdrsrch.aset_channel,
                                                           (uint8) max );
          /* Add the strongest nset pilots to the search list */
#endif /* FEATURE_HDR_REVB */

      }

      /* Check if there is any room to add any more nset pilots.  This
         increases RAHO probability to an NSET pilot that just became hot. */
      if ( ( max - n_nset ) > 0 )
      {
#ifdef FEATURE_HDR_REVB
        n_nset = hdrsrchutil_add_nset_to_search_list( &hdrsrch.aset_channel[0],
                                                    1,
                                                    max - n_nset,
                                                    TRUE );
#else
        n_nset = hdrsrchutil_add_nset_to_search_list( &hdrsrch.aset_channel,
                                                    max - n_nset,
                                                    TRUE );
          /* Add as many neighbors as possible, in a circular fashion.
             Note: Do not update hdrsrchreacq.nset_pilots_searched so that NSET
                   pilots added here will get searched again in the last srch
                   to maintain the behavior of best NSET pilots being searched
                   at least twice. */
#endif /* FEATURE_HDR_REVB */

        hdrsrchlog.sleep_info.num_reacq_pilots_srched = n_aset + n_cset + n_nset;
          /* Update number of pilots searched in 1st reacq search only. */
      }
    }
    else
    {
      /* Subsequent searches */

#ifdef FEATURE_HDR_REVB
      max = HDR_MAX_SRCH_CNT - 
      hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt - hdrsrchset.cset_cnt;
        /* Compute maximum number of nset pilots that can fit in list */
#else
      max = HDR_MAX_SRCH_CNT - hdrsrchset.aset_cnt - hdrsrchset.cset_cnt;
        /* Compute maximum number of nset pilots that can fit in list */
#endif /* FEATURE_HDR_REVB */

#ifdef FEATURE_HDR_REVB
      n_nset = hdrsrchutil_add_nset_to_search_list( &hdrsrch.aset_channel[0],
                                             1,
                                             (uint8) max,
                                             FALSE );
        /* Add as many neighbours as possible, in a circular fashion */
#else
      n_nset = hdrsrchutil_add_nset_to_search_list( &hdrsrch.aset_channel,
                                             (uint8) max,
                                             FALSE );
        /* Add as many neighbours as possible, in a circular fashion */
#endif /* FEATURE_HDR_REVB */
      if ( n_nset > 0 )
      {
        hdrsrchreacq.nset_pilots_searched += n_nset;
          /* Count these neighbours, to ensure we crawl through entire list */
      }
      else
      {
        hdrsrchreacq.nset_pilots_searched = HDR_MAX_NSET_SIZE;
          /* no neighbors added to search, so force value to max to account for
           the case of a nset of only o-freq neighbors */
      }
    }

  } /* if not intra-scc */
  else
  {
    if ( hdrsrchreacq.fast_acq_cb != NULL  )
    {
       HDR_MSG_SRCH( MSG_LEGACY_HIGH, " IRAT fast acq: search ASET only in reacq ");
    }
  }

  if( ! HDRSRCH_PILOT_MEAS_PN_CIRCLE_SEARCH_NV_ENABLE ) 
  {
    /* When pn circle search is enabled, window size will be 8192. Calling 
       the following functions would mess up search4 parameters 
    */
    hdrsrchutil_adjust_window_size(
                                   hdrsrchreacq.win_size_fctr,
                                   HDRSRCHREACQ_MAX_WINDOW_SIZE,
                                   HDRSRCHREACQ_MIN_WINDOW_SIZE
                                   );
  }
    /* Readjust and limit search window sizes */

} /* hdrsrchreacq_prepare_search_list() */

#ifdef FEATURE_HDR_QUICK_IDLE
/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_ENABLE_QPCH_DET

DESCRIPTION
  This function sets up firmware QPCH preamble detection and disables
  QPCH forced preamble detection.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

LOCAL void hdrsrchreacq_enable_qpch_det( void )
{
  uint16  current_hstr;  
    /* Current HSTR value */
   
  uint16  last_scc_offset_hstr = 0;
    /* HSTR value of the last SCC offset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  current_hstr = HDRHSTR_GET_COUNT();
    /* Read the current HSTR register value. The value in the MDSP
       has been updated during the second ISR far enough in time
       ago to rely on the MDSP's HDR count. */

  /* Determine the HSTR of the last scc offset of the next SCC boundary */
  last_scc_offset_hstr = current_hstr + ( HDRSRCHREACQ_HS_PER_SCC );
  last_scc_offset_hstr = last_scc_offset_hstr - ( last_scc_offset_hstr % HDRSRCHREACQ_HS_PER_SCC );
  last_scc_offset_hstr += ( HDRSRCHREACQ_MAX_SCC_OFFSET * 2 );

  hdrsrchmsg.qpch_msg.qpchMACIndex = HDRSRCHREACQ_QPCH_MACIDX;
    /* Enable quick page normal preamble detection */

  hdrsrchmsg.qpch_msg.qForcePreamHSTR = last_scc_offset_hstr;
    /* Set up the HSTR for when to clear QPCH detection*/

  /* Current FW API is not updated on TRITON.
     Remove featurization after update */

  hdrsrchmsg.qpch_msg.qForcePreamDetect = 0x0;
   /* Disable QPCH forced preamble detection */

  hdrsrchmsg_send_msg( &hdrsrchmsg.qpch_msg, 
                       sizeof( hdrfw_qpch_msg_t ) );

  HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                 "Use QPCH Preamble Detection hstr now = %d, disable QPCH detection at hstr = %d",
                  current_hstr,
                  last_scc_offset_hstr );

} /* hdrsrchreacq_enable_qpch_det */
#endif  /* FEATURE_HDR_QUICK_IDLE */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_SCHEDULE_PREAMBLE_DET

DESCRIPTION
  This function calculates the HSTR for the next CC/QPCH packet and -
  - If FPD is enabled, sets up firmware for CC/QPCH decoding if CC/QPCH
    preamble is missed.
  - If D-LNA is enabled, schedules odd preamble detect for CC/QPCH packet.

DEPENDENCIES
  Must be called at HDR reacquisition, before finger is assigned but after 
  firmware application is available.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/

LOCAL void hdrsrchreacq_schedule_preamble_det( void )
{
  uint16  cc_hstr = 0;
    /* HSTR value when next SCC packet or Intra-SCC CC packet is expected
       to start including SCC offset. */

  uint8   scc_rate;
    /* SCC rate */

  uint16  scc_offset;
    /* SCC offset */

  uint16  current_hstr;  
    /* Current HSTR value */

  boolean perform_cc_forced_preamble;
    /* Flag for whether forced sync /subsync CC preamble detection can be done */

#ifdef FEATURE_HDR_QUICK_IDLE
  uint16  qpch_hstr = 0;
    /* HSTR value when next QPCH packet is expected to start */

  uint16  last_scc_offset_hstr = 0;
    /* HSTR value of the last SCC offset */

  boolean perform_qpch_forced_preamble = TRUE;
    /* Flag for whether forced qpch preamble detection can be done */
#endif  /* FEATURE_HDR_QUICK_IDLE */

  uint16  sci;
    /* Last sleep sci value */

  uint16  num_of_hs_in_curr_sci;
    /* Number of half slot in current SCI */

  uint8   cur_hstr_in_sci;
    /* Current HSTR after modular with the number of half slot in current SCI */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  perform_cc_forced_preamble = TRUE;
    /* Assume SCC or subsync CC forced preamble can be done */

  scc_rate = hdrsrchsleep_get_scc_rate();

  if ( ( scc_rate != 1 ) && ( scc_rate != 2 ) )
  {
    ERR("Forced Preamble Det. Invalid SCC RATE:%d", scc_rate, 0, 0 );
    
    perform_cc_forced_preamble = FALSE;
      /* Forced preamble detect can not be done */
  }

  /* Determine the appropriate SCC offset */

  scc_offset = hdrsrchsleep_get_scc_offset( );
    /* Read the SCC offset saved before going to sleep by hdrsrchsleep
       note that may return 0xFFFF as offset so the range check below 
       excludes 0xFFFF since scc_offset is unit16 */

  if ( scc_offset > HDRSRCHREACQ_MAX_SCC_OFFSET )
  {
    scc_offset = 0;
      /* Otherwise use 0 as initial/default SCC offset value */

    perform_cc_forced_preamble = FALSE;
      /* Forced Sync or Subsync CC preamble detect can not be done */

#ifdef FEATURE_HDR_QUICK_IDLE
    perform_qpch_forced_preamble = FALSE;
      /* Forced QPCH preamble detect can not be done */
#endif  /* FEATURE_HDR_QUICK_IDLE */
  }

  /* Calculate the hstr value where next scc packet is expected */

  current_hstr = HDRHSTR_GET_COUNT();
    /* Read the current HSTR register value. The value in the MDSP
       has been updated during the second ISR far enough in time
       ago to rely on the MDSP's HDR count. */

#ifdef FEATURE_HDR_QUICK_IDLE

  /* Determine the HSTR of the last scc offset of the next SCC boundary */
  last_scc_offset_hstr = current_hstr + ( HDRSRCHREACQ_HS_PER_SCC );
  last_scc_offset_hstr = last_scc_offset_hstr - ( last_scc_offset_hstr % HDRSRCHREACQ_HS_PER_SCC );
  last_scc_offset_hstr += ( HDRSRCHREACQ_MAX_SCC_OFFSET * 2 );

#endif  /* FEATURE_HDR_QUICK_IDLE */

  if ( hdrsrchsleep_get_intra_scc_valid() == TRUE )
  {
    /* Intra-SCC Sleep. */
    cc_hstr = ( current_hstr & ~HDRSRCHREACQ_HS_PER_SCC_MASK ) +
              ( ( 2 * ( hdrsrchsleep_get_intra_scc_start_slot() + 
                      scc_offset ) ) & HDRSRCHREACQ_HS_PER_SCC_MASK ); 
      /* Assuming the intra-scc start slot is within the current SCC cycle, use
         this value instead of the SCC slot cycle bits (low order 9-bits) of
         the current hstr.  Also adjust cc_hstr for SCC offset value. */

    
    /* If the hstr boundary - the current hstr is less than than or 
       equal than 0 or greater than 64 half slots we don't perform 
       fpd. We subtract an extra 1 from the boundary as the hstrs
       can't be the same. */
    if ( (uint16)( cc_hstr - current_hstr - 1 ) > HDRSRCHREACQ_FPD_HSTR_MARGIN )
    {
      perform_cc_forced_preamble = FALSE;
    }
  }
  else if ( ( ( current_hstr % HDRSRCHREACQ_HS_PER_SCC ) > HDRSRCHREACQ_FPD_HSTR_TOO_EARLY ) ||
            ( ( current_hstr % HDRSRCHREACQ_HS_PER_SCC ) < HDRSRCHREACQ_FPD_HSTR_TOO_LATE  ) )
  {
    /* We are either a few slots before the upcoming scc cycle boundary 
       or just missed it. The following computation rounds
       the HSTR value to that scc cycle boundary */
    cc_hstr = current_hstr + (HDRSRCHREACQ_HS_PER_SCC/2);
    cc_hstr = cc_hstr - (cc_hstr % HDRSRCHREACQ_HS_PER_SCC);

    cc_hstr += (scc_offset * 2);
      /* Adjust cc_hstr for SCC offset value */

#ifdef FEATURE_HDR_QUICK_IDLE
    qpch_hstr = cc_hstr - HDRSRCHREACQ_QPCH_BEFORE_CC_IN_HS;
      /* Set the HSTR count for Quick Page */
#endif  /* FEATURE_HDR_QUICK_IDLE */
  }
  /* Assuming the smallest SCI subsync QPCH supports is 5 */
  else if ( ( ( sci = hdrsrchsleep_get_last_sleep_sci() ) < 0x06 ) && 
            ( hdrsrchsleep_is_qpch_enabled() ) )
  {
    num_of_hs_in_curr_sci = HDRSRCHREACQ_HS_PER_SCC >> ( 0x06 - sci );
      /* Get number of half slot in last SCI cycle */

    cur_hstr_in_sci = current_hstr % num_of_hs_in_curr_sci;
      /* Current HSTR after modular with number of half slot in current SCI */

    if ( ( cur_hstr_in_sci < HDRSRCHREACQ_FPD_HSTR_TOO_LATE ) ||
         ( cur_hstr_in_sci > ( num_of_hs_in_curr_sci - 
                               HDRSRCHREACQ_FPD_HSTR_EARLY_DELTA ) ) )
    {
      /* We are either a few slots before the upcoming Subsync CC cycle boundary 
         or justed missed it. The following computation rounds the HSTR value 
         to that subsync CC cycle boundary */
      cc_hstr = current_hstr + ( num_of_hs_in_curr_sci - 1 );
      cc_hstr = cc_hstr - ( cc_hstr % num_of_hs_in_curr_sci );

      last_scc_offset_hstr = cc_hstr + ( HDRSRCHREACQ_MAX_SCC_OFFSET * 2 );

      cc_hstr += (scc_offset * 2);
        /* Adjust cc_hstr for Subsync CC offset value */

#ifdef FEATURE_HDR_QUICK_IDLE
      qpch_hstr = cc_hstr - HDRSRCHREACQ_QPCH_BEFORE_CC_IN_HS;
        /* Set the HSTR count for Quick Page */
#endif  /* FEATURE_HDR_QUICK_IDLE */
    }
    else
    {
      /* We are either out of range for subsync CC boundary or 
         it is too late to program Subsync CC/QPCH forced preamble detection */
      perform_cc_forced_preamble = FALSE;
#ifdef FEATURE_HDR_QUICK_IDLE
      perform_qpch_forced_preamble = FALSE;
#endif  /* FEATURE_HDR_QUICK_IDLE */
    }
  }
  else
  {
    /* We are either out of range for SCC boundary or
       it is too late to program for SCC/QPCH forced preamble detection. */
    perform_cc_forced_preamble = FALSE;
#ifdef FEATURE_HDR_QUICK_IDLE
    perform_qpch_forced_preamble = FALSE;
#endif  /* FEATURE_HDR_QUICK_IDLE */
  }

#ifdef FEATURE_HDR_QUICK_IDLE
  if ( hdrsrchsleep_is_qpch_enabled() && !hdrsrchsleep_get_intra_scc_valid() )
  { 
    hdrsrchmsg.qpch_msg.qpchMACIndex = HDRSRCHREACQ_QPCH_MACIDX;
    /* Enable quick page normal preamble detection */

    if ( perform_qpch_forced_preamble == TRUE )
    {
      /* Current FW API is not updated on TRITON.
         Remove featurization after update */
      hdrsrchmsg.qpch_msg.qForcePreamDetect = 0x8000;
        /* Enable QPCH forced preamble detection */

      hdrsrchmsg.qpch_msg.qForcePreamHSTR = qpch_hstr;
        /* Set up the HSTR count to attempt forced QPCH detection */

      hdrsrchmsg_send_msg( &hdrsrchmsg.qpch_msg, 
                           sizeof( hdrfw_qpch_msg_t ) );
        /* Enable QPCH forced preamble detection */

      HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
                     "QFPD: @hstr= %d, hstr now = %d, scc offset = %d",
                      qpch_hstr, current_hstr, scc_offset );
    }
    else
    {
      /* Current FW API is not updated on TRITON.
         Remove featurization after update */
      hdrsrchmsg.qpch_msg.qForcePreamDetect = 0x0;

      hdrsrchmsg.qpch_msg.qForcePreamHSTR = last_scc_offset_hstr;
        /* When this HSTR is reached the FW will disable QPCH preamble detection. */

      hdrsrchmsg_send_msg( &hdrsrchmsg.qpch_msg, 
                           sizeof( hdrfw_qpch_msg_t ) );

      HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                     "Skipped QPCH fpd, hstr now = %d, disable QPCH detection at hstr = %d",
                      current_hstr,
                      last_scc_offset_hstr );
    }
  }
  else
  {
    hdrsrchmsg.qpch_msg.qpchMACIndex = 0x0;

    /* Current FW API is not updated on TRITON.
       Remove featurization after update */
    hdrsrchmsg.qpch_msg.qForcePreamDetect = 0x0;

    hdrsrchmsg.qpch_msg.qForcePreamHSTR = 0x0;

    hdrsrchmsg_send_msg( &hdrsrchmsg.qpch_msg, 
                         sizeof( hdrfw_qpch_msg_t ) );

    HDR_MSG_SRCH( MSG_LEGACY_MED,
                  "Quick page is not enabled"); 
  }  /* hdrsrchsleep_is_qpch_enabled( ) */
#endif  /* FEATURE_HDR_QUICK_IDLE */

  if ( HDRSRCH_FORCED_PREAMBLE_DETECT_NV_ENABLE )
  {
    if ( perform_cc_forced_preamble == TRUE )
    {
      hdrsrchmsg.fpd_msg.forcePreamType = scc_rate; 
        /* Set the scc data rate */
  
      hdrsrchmsg.fpd_msg.forcePreamHSTR = cc_hstr; 
        /* Set up the HSTR count to attempt forced detection */
  
      hdrsrchmsg.fpd_msg.forcePreamDetect = 1;
        /* Enable FPD */
  
      hdrsrchmsg_send_msg( &hdrsrchmsg.fpd_msg.hdr, 
                           sizeof( hdrfw_fpd_msg_t ) );
        /* Enable forced preamble detection */
  
      HDR_MSG_SRCH_4( MSG_LEGACY_MED, 
                     "FPD: @hstr= %d, hstr now = %d, scc offset = %d, rate= %d.",
                      cc_hstr, current_hstr, scc_offset, scc_rate);
  
      hdrsrchreacq.forced_preamble_activated = TRUE ;
        /* Set flag that FPD has been activated */
  
      hdrrx_disable_missed_scc_detection();
        /* Disable missed CC ISR for now */
  
      hdrdec_enable_on_arrival_delivery();
        /* Enable on-arrival packet delivery */
  
    } /* perform_cc_forced_preamble */
    else
    {
      hdrsrchreacq.forced_preamble_activated = FALSE;
        /* Set flag that FPD has not been activated */
  
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                      "Skipped forced preamble detect, hstr now = %d ",
                      current_hstr );
    }  /* perform_cc_forced_preamble */

  } /* If FPD is enabled via NV */

#ifdef FEATURE_HDR_DLNA
  if ( hdrsrchutil_dlna_is_enabled() )
  {
    hdrsrchutil_schedule_odd_pream_det( 
      ( (cc_hstr >> 1) % HDRTS_SLOTS_PER_SCC ),
      1 );
      /* Schedule odd premble detect for the upcoming CC packet. */

  }
#endif /* FEATURE_HDR_DLNA */

} /* hdrsrchreacq_schedule_preamble_det( ) */


/*=========================================================================

FUNCTION HDRSRCHREACQ_FPD_TIMER_CB

DESCRIPTION 
  Callback for FPD timer event.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchreacq_fpd_timer_cb
(
  timer_cb_data_type              data
    /* Arbitrary data given to timer_def( ) to pass to this callback function */
)
{

  if ( ( ( HDRSRCH_FORCED_PREAMBLE_DETECT_NV_ENABLE ) 
#ifdef FEATURE_HDR_DLNA
    || ( hdrsrchutil_dlna_is_enabled() ) 
#endif /* FEATURE_HDR_DLNA */
    ) && ( hdrsrch_state_id == HDRSRCH_REACQ_STATE ) )
  {
    hdrsrchreacq_schedule_preamble_det();
  }
  else
  {    
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                    "Skipped forced preamble detect, hdrsrch state = %d ",
                    hdrsrch_state_id );
  }
} /* hdrsrchreacq_fpd_timer_cb */


/*==========================================================================

FUNCTION HDRSRCHREACQ_START_SEARCH

DESCRIPTION
  This function starts search process for reacq 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchreacq_start_search( void )
{
#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  boolean default_search;
#endif 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchreacq.state = HDRSRCHREACQ_SEARCH_STATE;
    /* Switch to reacquisition state */ 
 
  hdrdec_activate();
    /* Enable HDR decoder clock */

#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT
  HDRFING_ENABLE_EAFT();
    /* Enable early arriving finger threshold. */
#endif /* FEATURE_HDR_NEEDS_QDSP6_PORT */

  hdrsrchreacq_prepare_search_list( );
    /* Prepare reacquisition list and be ready to program it after warmup */

  default_search = FALSE;
#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  if ( hdrsrch.idle_state_div.wakeup_info.div_enabled )
  {
    default_search = hdrsrch.idle_state_div.wakeup_info.div_timeout;
      /* If we timeout do default search, because it is too late 
         for pilot pn to be valid. */
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

  hdrsrchutil_program_reacq_search( default_search );
    /* ... And start searching again. */
    
  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_TUNE_DONE ); 

}/* hdrsrchreacq_start_search */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_TUNE_DONE

DESCRIPTION
  This function is called after the RF has been warmuped up and tuned.

DEPENDENCIES
  Must only be used as a callback from hdrsrchrf_enable_and_tune( )

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Starts reacquisition searching

===========================================================================*/

LOCAL void hdrsrchreacq_tune_done
( 
  hdrsrchrf_tune_status_enum_type status
)
{

  uint16                    scc_offset;
    /* SCC offset for logging */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  uint8                     div_mask = 0;
    /* diversity state mask */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_PROFILE( REACQ_TUNE_DONE );
    /* sleep timeline profiling output if enabled */

  if ( status != HDRSRCHRF_TUNE_SUCCESS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_RF_TUNE_FREQ_ERROR);
#endif
    hdrsrchreacq.state = HDRSRCHREACQ_TUNE_FAILED_STATE;
      /* Move to failed state. In failed state, reacq state machine
         does nothing and expects to be deactivated. */

    if ( hdrsrchreacq.deactivate_pending )
    {
      hdrsrchreacq_deactivate( );
    }

    return;
  }


#ifdef FEATURE_HDR_AGC_OPTIMIZATION_P1
  hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
      /* Set AGC acquisition params back to normal mode */

#endif /* FEATURE_HDR_AGC_OPTIMIZATION_P1 */

  /* RF Warmup and tuning is complete ... */

  hdrsrchlog.reacq_info.rf_warmup_tune_time_ms = (uint16)
    hdrsrchutil_stop_timer_ms( hdrsrchlog.reacq_timer );
    /* Measure and record RF warmup and tune time here       */

  if ( hdrsrchsleep_get_intra_scc_valid() == FALSE )
  {
    hdrsrchlog.reacq_info.tune_to_scc_start_hs =
      HDRSRCHREACQ_HS_PER_SCC - (HDRHSTR_GET_COUNT( ) % HDRSRCHREACQ_HS_PER_SCC);
      /* Log " Tune Done " to "start of SCC", in half_slot unit */
  }
  else
  {
    /* Intra-SCC sleep requires a different calculation from tune done to
       SCC start. */

    scc_offset = hdrsrchsleep_get_scc_offset();
    if ( scc_offset > HDRSRCHREACQ_MAX_SCC_OFFSET )
    {
      scc_offset = 0;
        /* SCC Offset might be 0xFFFF for decoding all interleaves.  Assume 0
           slots for wakeup purpose */
    }

    hdrsrchlog.reacq_info.tune_to_scc_start_hs =
      ( 2 * ( hdrsrchsleep_get_intra_scc_start_slot() + scc_offset ) -
      ( HDRHSTR_GET_COUNT() % HDRSRCHREACQ_HS_PER_SCC ) );
      /* Log " Tune Done " to "start of SCC", in half_slot unit */
  }

  if ( hdrsrchreacq.deactivate_pending )
  {
    /* ... if we've been told to deactivate, now is the time */

    hdrsrchreacq_deactivate( );
  }
  else
  {
    /* ... otherwise, it is time to start reacquisition. */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
    if ( hdrsrch.idle_state_div.idle_div_enabled &&  
         hdrsrch.idle_state_div.wakeup_info.div_enabled )
    {
      hdrsrchreacq.state = HDRSRCHREACQ_WAIT_FOR_DIV_STATE;
      HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                    "Idle Div: enabling diversity in reacq" );
      hdrsrchrfdiv_enable_div_in_non_tc( div_mask | HDRSRCH_DIV_IN_REACQ );
        /* Enable Diversity */                          
      
    }
    else
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
    {
      hdrsrchreacq_start_search();
        /* If we don't have to enable diversity program search */   
    }      
  }

} /* hdrsrchreacq_tune_done( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_RXCHIPX16_ON

DESCRIPTION
  This function is called when RxChipx16 is gated back on.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Resets Pilot Filter

===========================================================================*/

LOCAL void hdrsrchreacq_rxchipx16_on( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_RXCHIPX16_ON );

  hdrsrchreacq.chipx16_on = TRUE;
    /* Chipx16 is now on. */

  if ( hdrsrchreacq.deactivate_pending )
  {
    hdrsrchreacq_deactivate( );
  }
  else if ( hdrsrchreacq.state == HDRSRCHREACQ_WARMUP_STATE )
  {
    hdrsrchreacq.state = HDRSRCHREACQ_TUNE_STATE;

    hdrsrch_add_to_wait_mask( HDRSRCH_TUNE_SIG );
      /* Restore tune signal to wait mask, to allow tune_done function to be
         called when tuning is complete. */

      /* ... although the RF is "warm", we may still be tuning. */
  }
  else if ( hdrsrchreacq.state == HDRSRCHREACQ_TUNE_FAILED_STATE )
  { 
    /* If code reached here, we would've added TUNE_SIG prior to 
       this CR fix, and would've crashed. */
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "ChipX16 received in tune failed state!" );
  }
} /* hdrsrchreacq_rxchipx16_on( ) */



/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_ENABLE_AND_TUNE

DESCRIPTION
  This function will enable the RF, if required, and
  tune the RF to the correct ASET frequency.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  TBD

===========================================================================*/

LOCAL void hdrsrchreacq_enable_and_tune
(
  boolean                         rxchipx16_on
    /* Flag indication if the RxChipx16 clock is off or on */
)
{
  boolean tune_status;
    /* Status of tune to chan */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_RF_TUNE_START );

  hdrsrchreacq.state = HDRSRCHREACQ_WARMUP_STATE;
      /* Warming up now */

#ifdef FEATURE_HDR_AGC_OPTIMIZATION_P1

    hdrsrchrf_set_agc_acq_params( HDRSRCHRF_FAST_AGC_ACQ_MODE,
                                  hdrsrchutil_get_agc_rtcoffset(),
                                  0 );
      /* Set AGC acquisition params for fast mode  */
#endif /* FEATURE_HDR_AGC_OPTIMIZATION_P1 */

  /* If RTC is off, we need to adjust RTC for time slept. 
     Note: rxchipx16 is being used as an indicator of sleep. */
  if ( !rxchipx16_on )
  {

#ifdef FEATURE_HDR_DIME_MODEM
   /* In Dime, start HDR RTC, disable Rx/Tx compare count and send new sleep_adj message to
      fw. This should be done before online interrupt.
   */

   hdrsrchsleep_send_sleep_adjust_msg();
#else
   /* In Nikel, FW uses sleep duration sent in sleep_adjust message to advance RTC and HSTR.
      sleep_adjust message is sent after wakeup and FW is already in active state.
      In Dime we send sleep_adjust before sleep and give wakeup_mstr in sleep_adjust message.
   */
   hdrsrchsleep_adjust_rtc();

#endif /* FEATURE_HDR_DIME_MODEM */

  }

  if ( rxchipx16_on )
  {
    hdrsrchreacq.state = HDRSRCHREACQ_TUNE_STATE;
      /* Waiting for Tune Signal */
  }
  else
  {
    hdrsrchreacq.state = HDRSRCHREACQ_WARMUP_STATE;
      /* Fast RF Warmup - PDM's are precharging DC loop capacitors. */

    hdrsrch_remove_from_wait_mask( HDRSRCH_TUNE_SIG );
      /* Ensure the tune signal is not acted on until rxchipx16 is on.
         hdrsrchsleep_rf_warmup_done( ) will re-add signal to wait mask. */

    hdrsrch_add_to_wait_mask( HDRSRCH_RXCHIPX16_SIG );
      /* Wait for RxChipx16 to be re-enabled */
  }

  /* Tune to the right channel.  The RF is automatically enabled if need
     be, and in this case we need to. */
#ifdef FEATURE_HDR_REVB

  HDR_ASSERT ( hdrsrch.aset_channel_cnt <= HDRSRCH_MAX_NUM_CARRIERS ) ;
    /* KW fix : Check to limit hdrsrch.aset_channel_cnt to max */

  tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                          hdrsrch.aset_channel,
                          hdrsrch.aset_demod,
                          hdrsrch.aset_channel_cnt,
                          hdrsrchreacq_tune_done,
                          HDRSRCHREACQ_BAND_GRANT_TIMEOUT_MS );
#else
   tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                          &hdrsrch.aset_channel,
                          hdrsrchreacq_tune_done,
                          HDRSRCHREACQ_BAND_GRANT_TIMEOUT_MS );
#endif /* FEATURE_HDR_REVB */

  /* If deactivate was pending before tune_to_chan call then 
  hdrsrchst_init_inactive_state is called already and now HDR 
  is in inactive state */
  if ( ( tune_status == FALSE ) 
    && ( hdrsrch_get_current_state() == HDRSRCH_REACQ_STATE ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "ReACQ tune failed" );

    hdrsrchreacq.state = HDRSRCHREACQ_TUNE_FAILED_STATE;

    /* call only when current state is reacq state and not inactive state */
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }

#ifdef FEATURE_HDR_WUTA
  if ( hdrsrchreacq.slept == TRUE )
  {
    if ( HDRSRCH_WUTA_NV_ENABLE )
    {
      hdrsrchsleep_adjust_rf_warmup_on_tune_done();
        /* Make an adjustment to the RF warmup duration if necessary. */
    }
    else
    {
      hdrsrchsleep_check_slack_on_tune_done();
        /* Check slack */
    }
  }
#endif /* FEATURE_HDR_WUTA */

} /* hdrsrchreacq_enable_and_tune( ) */



/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_ON_TIME_VALID

DESCRIPTION
  This function is called when time becomes valid and the time_valid
  signal exists in the wait mask (we have reacquired after sleep).
  This function transitions back to the idle state.

DEPENDENCIES
  Must only exist in the state machine table.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchreacq_on_time_valid( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Synchronize Time of Day to HDR System Time" );

  hdrts_update_ts_time();
    /* Synchronize Time of Day to HDR System Time */


  hdrsrchmon_init_idle_state();
    /* We've reacquired after sleep - transition to idle state */

} /* hdrsrchreacq_on_time_valid( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_REPORT

DESCRIPTION
  This function reports wakeup & reacquisition, with a possible handoff
  to a new pilot.

DEPENDENCIES
  None

PARAMETERS
  sect - New active sector
  NULL - Same old sector

RETURN VALUE
  None.

SIDE EFFECTS
  Transition to IDLE/Monitor

===========================================================================*/

LOCAL void hdrsrchreacq_report
(
  hdrsrch_sect_struct_type        *sect,
    /* Sector to report on */

  boolean                          reacq_succeeded
    /* Reacquisition succeeded on this idle wakeup. */
)
{
  sys_link_type                   link;
     /* Link report structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchreacq.wakeup_cb )
  {
    if ( sect != NULL )
    {
      link.pilot_pn           = sect->pn_offset;
      link.channel            = sect->chan;

      hdrsrchreacq.wakeup_cb( sect->link_id, &link, 
                              hdrsrchsleep_get_wakeup_reason( ),
                              reacq_succeeded );
        /* Inform protocol of ReAcq Handoff to given sector */
    }
    else
    {
      hdrsrchreacq.wakeup_cb( HDRSRCH_REF_LINK_ID, NULL,
                              hdrsrchsleep_get_wakeup_reason( ),
                              reacq_succeeded );
        /* Inform protocol that we woke up on same pilot */
    }
  }

} /* hdrsrchreacq_report( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_CALCULATE_WIN_SIZE_FCTR

DESCRIPTION
  This function caculates the window size factor based on the slew

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE : 
  None

SIDE EFFECTS
  None.

===========================================================================*/
void hdrsrchreacq_calc_win_size_fctr( void )
{
  int16                           slew_x1;
   /* slew in chipx1 to which the window size will be compared against */

  uint16                          winsize_x1;
   /* Current search window size, in chipx1 */

 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  slew_x1 = hdrsrchreacq.slew_x2 / 2;

  /* Convert negative slews to positive slews */
  if (slew_x1 < 0)
  {
    slew_x1 = -slew_x1;
  }

  winsize_x1 = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->win_sizex1;
    /* get the normal winsize */
  winsize_x1 *= hdrsrchreacq.win_size_fctr;
  winsize_x1 = (uint16) hdrsrchutil_limit_window_size( 
                                             winsize_x1,
                                             HDRSRCHREACQ_MAX_WINDOW_SIZE,
                                             HDRSRCHREACQ_MIN_WINDOW_SIZE);
    /* get the adjusted seach win size for reacq search */

  HDR_MSG_SRCH_3(MSG_LEGACY_HIGH,
                 "srch winsize adj slew=%d,curr_win_size=%d,win_sizw_fctr=%d",
                 slew_x1, winsize_x1,hdrsrchreacq.win_size_fctr );

  if ( slew_x1 > ( winsize_x1 / 4 ) )
  {
    if( winsize_x1 < hdrsrchutil_get_winsize_x1(HDRSRCHREACQ_MAX_WINDOW_SIZE))
    {
      hdrsrchreacq.win_size_fctr *= 2;
      HDR_MSG_SRCH_2(MSG_LEGACY_HIGH, "next Reacq srch winsize fctr=%d max_win_size=%d",
                     hdrsrchreacq.win_size_fctr, hdrsrchutil_get_winsize_x1(HDRSRCHREACQ_MAX_WINDOW_SIZE));
    }
    else
    {
      (void) hdrsrchsleep_decrement_wakeup_sci();
        /* if window size will become too big, reduce sleep time*/
    }
  }
  else if( slew_x1 < (winsize_x1 / 8) )
  {
    if ( !hdrsrchsleep_increment_wakeup_sci() )
      /* if window is small, double sleep time if possible*/
    { 
      if( ((hdrsrchreacq.win_size_fctr / 2) > 0 )  &&
           winsize_x1 > hdrsrchutil_get_winsize_x1(HDRSRCHREACQ_MIN_WINDOW_SIZE) )
        /* if sleep time cannot be increased, half window size */
      {
        hdrsrchreacq.win_size_fctr /= 2;
        HDR_MSG_SRCH_1(MSG_LEGACY_HIGH, "next Reacq srch winsize fctr=%d",
                       hdrsrchreacq.win_size_fctr );
      }
    }
  } 
} /* hdrsrchreacq_calc_win_size_fctr */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_POST_PROC_REACQ

DESCRIPTION

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchreacq_post_proc_reacq
(
  int16                           slew_x2
    /* Reacquisition slew - the chipx2 error that occurred during sleep */
)
{
  int                             winsize;
    /* Reacquisition window size, in chips */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchreacq.slept )
  {
    /* Since we've slept, the reacquisition slew is proportional to the
       frequency estimate error of the slow clock.
       
       Update the maximum sleep duration, based on this last sleep duration,
       and the reacquisition window size.
       */
#ifdef FEATURE_HDR_REVB
    winsize = hdrsrchutil_limit_window_size( 
          hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->win_sizex1,
                                             HDRSRCHREACQ_MAX_WINDOW_SIZE,
                                             HDRSRCHREACQ_MIN_WINDOW_SIZE);
#else
    winsize = hdrsrchutil_limit_window_size( hdrsrchset.aset[0]->win_sizex1,
                                             HDRSRCHREACQ_MAX_WINDOW_SIZE,
                                             HDRSRCHREACQ_MIN_WINDOW_SIZE);
#endif /* FEATURE_HDR_REVB */

    hdrsrchsleep_adjust_sci( slew_x2, winsize );
      /* Report reacquisition slew and window size back to HDRSRCHSLEEP,
         to adjust the SCI (if applicable) */
  }

  hdrsrchreacq_report( NULL, TRUE );
    /* Inform protocol of ReAcq of old active pilot */
}

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_UPDATE_REACQ_INFO

DESCRIPTION
  This function updates REACQ INFO log packet depending on reacq result.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchreacq_update_reacq_info
(
  hdrsrch_sect_struct_type        *sect,
  /* sector that the "RAHOed" or old pilot belongs to   */
  
  log_hdr_reacq_result_enum_type  result
  /* Re-acquisition result              */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchlog.reacq_info.result        = result;
    /* Record re-acquisition result */

  if ( ( result == HDRLOG_REACQ_LOST )  ||
       ( result == HDRLOG_REACQ_LOST_DSP_NOT_READY ) )
  {
    INC_SAT( hdrsrchlog.reacq_info.failure_count );
      /* Record the cumulative failure_count. It saturates at 0xffff  */
  } /* if result */

  if ( log_status(LOG_HDR_REACQ_INFO_C) )
  {
    hdrsrchlog.reacq_info.srch_count = hdrsrchreacq.search_list_cnt;
      /* Updates the number of searches during Re-acq                 */

    switch( result )
    {
      case HDRLOG_REACQ_RAHO:

        hdrsrchlog.reacq_info.pilot_energy  = sect->total_eng;
        hdrsrchlog.reacq_info.pilot_pn      = sect->pn_offset;
          /* Updates the pilot info of the "RAHOed" pilot */

#ifdef FEATURE_HDR_REVB
        hdrsrchlog.reacq_info.old_pn = 
           hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset;
        hdrsrchlog.reacq_info.old_pn_energy = 
           hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng;
          /* Updates the old pilot info                   */
#else
        hdrsrchlog.reacq_info.old_pn        = hdrsrchset.aset[0]->pn_offset;
        hdrsrchlog.reacq_info.old_pn_energy = hdrsrchset.aset[0]->total_eng;
          /* Updates the old pilot info                   */
#endif /* FEATURE_HDR_REVB */

        break;

      case HDRLOG_REACQ_OLD_PILOT:

        hdrsrchlog.reacq_info.pilot_energy  = sect->total_eng;
        hdrsrchlog.reacq_info.pilot_pn      = sect->pn_offset;
          /* Updates the old pilot info                   */

        break;

    case HDRLOG_REACQ_LOST:
    case HDRLOG_REACQ_LOST_DSP_NOT_READY:

        break;

      default:

        HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Un-recognized REACQ result: %d",
                        result );

        break;
    
    } /* switch */
  
  } /* if log_status */

} /* hdrsrchreacq_update_reacq_info */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHUTIL_FIND_SECT_WITH_SUFFICIENT_ENG

DESCRIPTION
  This function finds a sector with sufficient instantaneous energy to be acquired.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE : 
  index of sector 

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL uint8 hdrsrchreacq_find_sect_with_sufficient_eng (void)
{
  uint16                          comp_eng;
    /* eng compare var. biased (boosted) energy value of the ASET sector */

  uint16                          min_eng;
    /* Minimum energy for handoff candidate */

  boolean                         ho_candidate;
    /* Whether this is a HO candidate */

  boolean                         same_subnet_ho = FALSE;
    /* Whether this is the same-subnet handoff */

  uint8                           ho_candidate_idx = 0;
    /* Index of handoff candidate */

  uint8                           i;
    /* Search loop index */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  min_eng = hdrsrchset_scale_eng( 0,
                                  HDRSRCHIDLE_REACQ_HO_THRESH_HALF_DB );
  /* boost the aset energy by x dB so that we find the would be RAHO sector
     only if it exceeds this boosted energy */

  comp_eng = min_eng;
   /* We need at least this much for new pilot */
   
  if ( !( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT ) )
  {
     ERR_FATAL("Check hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT Failed", 0, 0, 0);
  }

  for ( i = 1; i < hdrsrchdrv.list_cnt; i++ )
    /* The first sector in search list is active set, so i starts from 1 */
  {
    /* Then ... look for a better pilot */

    if (hdrsrchdrv.srch_list[i].sect_ptr->inst_eng > 0)
    {
      /* We want to find a pilot with best filtered energy and its instantaneous
         energy is greater than 0 */

      ho_candidate = hdrsrchutil_sect_is_ho_candidate(
                        hdrsrchdrv.srch_list[0].sect_ptr,
                        hdrsrchdrv.srch_list[i].sect_ptr,
                        TRUE,    /* use instantaneous energy */
                        comp_eng,
                        min_eng,
                        &same_subnet_ho );

      /* Check whether it is a HO candidate */

      if ( ho_candidate == TRUE )
      {
         ho_candidate_idx = i;
           /* Found the stronger sector */

         comp_eng  = ( hdrsrchdrv.srch_list[i].sect_ptr )->total_eng;
           /* Move the bar for energy comaprison higher */
      }
    }
  } /* for() */

  return ho_candidate_idx;
} /* hdrsrchreacq_find_sect_with_sufficient_eng */


/*===========================================================================

FUNCTION HDRSRCHREACQ_GET_HO_CANDIDATE

DESCRIPTION
  This function finds the best CSET pilot, subject to it being stronger than
  the current ASET pilot. The threshold to find the best CSET pilot is fixed.
  Factors as CSET sector being cached and how strong the ASET sector pilot 
  are not considered.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Index of stronger pilot as handoff candidate or 0

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL uint8 hdrsrchreacq_get_ho_candidate( void )
{
  uint8                           i;
    /* Search loop index */

  hdrsrch_sect_struct_type        *aset_sect;
    /* Strongest sector */

  uint8                           ho_candidate_idx = 0;
    /* Index of handoff candidate */

  uint16                          min_eng;
    /* Minimum energy */

  uint16                          comp_eng;
    /* eng compare var. biased (boosted) energy value of the ASET sector */

  boolean                         ho_candidate;
    /* Whether this is a HO candidate */

  boolean                         same_subnet_ho = FALSE;
    /* Whether this is the same-subnet handoff */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  aset_sect      = hdrsrchdrv.srch_list[0].sect_ptr;
    /* Assume first pilot (ASet pilot) is the best */

  comp_eng = hdrsrchset_scale_eng( aset_sect->total_eng,
                                   HDRSRCHIDLE_REACQ_HO_THRESH_HALF_DB );
    /* boost the aset energy by x dB so that we find the would be RAHO sector
       only if it exceeds this boosted energy */

  min_eng = comp_eng;
    /* We need at least this much for new pilot */
  
  if ( !( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT ) )
  {
     ERR_FATAL("Check hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT Failed", 0, 0, 0);
  }

  for(i=1; i<hdrsrchdrv.list_cnt; i++)
    /* The first sector in search list is active set, so i starts from 1 */
  {
    /* Then ... look for a better pilot */

    ho_candidate = hdrsrchutil_sect_is_ho_candidate(
                                                     aset_sect,
                                                     hdrsrchdrv.srch_list[i].sect_ptr,
                                                     FALSE,  /* use filtered energy */ 
                                                     comp_eng,
                                                     min_eng,
                                                     &same_subnet_ho );

    /* Check whether it is a HO candidate */

    if ( ho_candidate == TRUE )
    {
      comp_eng  = ( hdrsrchdrv.srch_list[i].sect_ptr )->total_eng;
        /* Move the bar higher */

      ho_candidate_idx = i;
        /* Found a stronger sector */
    }
  }

  if ( ho_candidate_idx == 0 )
  {
    /* we didn't find any handoff candidate based on filtered energy */

    if ( aset_sect->inst_eng == 0 ) 
    {
      /* If instantaneous energy of aset sector is 0 (< 12db, too low to acquire),
         we need to find a candidate with sufficient instantaneous energy, even 
         though there is no qualified candidate based on filtered energy */

      ho_candidate_idx = hdrsrchreacq_find_sect_with_sufficient_eng();

    }
    /* aset_inst_eng > 0 means that instantaneous energy of aset sector is
       sufficient to acquire. We return ho_candidate_idx as 0.It means no handoff
       candidate is found.
    */
  }

  /* return the index of handoff candidate */
  return ho_candidate_idx;

} /* hdrsrchreacq_get_ho_candidate */

/*===========================================================================

FUNCTION HDRSRCHREACQ_GET_HO_CANDIDATE_DYNAMIC_THRESH

DESCRIPTION
  This function finds the best CSET pilot, subject to it being stronger than
  the current ASET pilot. If a sector is cached, it has higher priority than
  those noncached. The threshold to select this sector is lower than the 
  threshold of selecting a noncached sector. If a the ASET sector has stronger
  power (greater than -7db), the threshold to select a CSET sector as handoff 
  candidate is higher.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Index of stronger pilot as handoff candidate or 0

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL uint8 hdrsrchreacq_get_ho_candidate_dynamic_thresh( void )
{
  uint8                           i;
    /* Search loop index */

  hdrsrch_sect_struct_type        *aset_sect;
    /* ASET sector */

  uint16                          best_eng = 0;
    /* eng compare var. biased (boosted) energy value of the ASET sector */

  boolean                         ho_candidate;
    /* Whether this is a HO candidate */

  boolean                         same_subnet_ho = FALSE;
    /* Whether this is the same-subnet handoff */

  uint16                          min_thresh_cached;
    /* Minimum threshold eng for sector is cached*/

  uint16                          min_thresh_notcached;
    /* Minimum threshold eng for sector is not cached*/

  boolean                         candidate_cached = FALSE;
    /* Flag whether the candidate is cached or not */

  uint8                           ho_candidate_idx = 0;
    /* Index of handoff candidate */

  hdrsrch_sect_struct_type        *sect = NULL;
    /* Strongest sector */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  aset_sect = hdrsrchdrv.srch_list[0].sect_ptr;
    /* Assume first pilot (ASet pilot) is the best */

  /* calculate handoff threshold */
  hdrsrchutil_calc_dynamic_ho_thresholds(
      aset_sect->total_eng, 
      &min_thresh_cached, &min_thresh_notcached );

  if ( !( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT ) )
  {
     ERR_FATAL("Check hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT Failed", 0, 0, 0);
  }

  for ( i = 1; i < hdrsrchdrv.list_cnt; i++ )
    /* The first sector in search list is active set, so i starts from 1 */
  {
    /* Then ... look for a better pilot */

      ho_candidate = hdrsrchutil_sect_is_ho_candidate_dynamic_thresh(
                        aset_sect,
                        hdrsrchdrv.srch_list[i].sect_ptr,
                        FALSE,  /* use filtered energy */ 
                        min_thresh_cached,
                        min_thresh_notcached,
                        best_eng,
                        sect,
                        &candidate_cached,
                        &same_subnet_ho
                     );

    /* Check whether it is a HO candidate */

    if ( ho_candidate == TRUE )
    {
      sect      = hdrsrchdrv.srch_list[i].sect_ptr;
        /* Remember the strongest sector */

      best_eng  = sect->total_eng;
        /* Move the bar higher */

      ho_candidate_idx = i;
        /* Update the stronger pilot index */

    }
  }


  if ( ho_candidate_idx == 0 )
  {
    /* we didn't find any handoff candidate based on filtered energy */

    if ( aset_sect->inst_eng == 0 ) 
    {
      /* If instantaneous energy of aset sector is 0 (< 12db, too low to acquire),
         we need to find a candidate with sufficient instantaneous energy, even 
         though there is no qualified candidate based on filtered energy */

      ho_candidate_idx = hdrsrchreacq_find_sect_with_sufficient_eng();

    }
    /* aset_inst_eng > 0 means that instantaneous energy of aset sector is
       sufficient to acquire. We return ho_candidate_idx as 0.It means no handoff
       candidate is found.
    */
  }

  /* return the index of handoff candidate */
  return ho_candidate_idx;

} /* hdrsrchreacq_get_ho_candidate_dynamic_thresh */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_ASET_MGMT

DESCRIPTION
  This function handles reacquisition search ASET management

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchreacq_aset_mgmt
(
  int new_rpts
    /* Number of new pilot report events (R/N to C, C>A, Weak A) */
)
{
  uint8                           raho_sect_idx; 
    /* index of RAHO candidate in search list */

  hdrsrch_sect_struct_type        *sect = NULL;
    /* Strongest sector */

  hdrsrch_sect_struct_type        *aset_sect;
    /* ASET sector */

  uint16                          aset_inst_eng;
    /* Best energy so far */

  int16                           slew_x2;
    /* Reacquisition slew - the chipx2 error that occurred during sleep */

  int16                           prev_slew_x2;
    /* Slew of the weaker pilot from which AT is RAHOing to strong pilot */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  aset_sect = hdrsrchdrv.srch_list[0].sect_ptr;
  aset_inst_eng  = aset_sect->inst_eng;
  slew_x2   = hdrsrchdrv.srch_list[0].srch_task.res_ptr->posx2[0];
  slew_x2  -= aset_sect->win_cenx2;
    /* Assume first pilot (ASet pilot) is the best */

  /* Only look for a better pilot if intra-scc is disabled,
     i.e., disable RAHO for intra-scc reacq */
  if ( hdrsrchsleep_get_intra_scc_valid() == FALSE )
  {

    if (( hdrsrchsleep_get_last_sleep_sci() > 6 )||
      ( HDRSRCH_ENABLE_IDHO_DYNAMIC_THRESH == FALSE ))
    {
      /* the sleep time is greater than 1 second, we don't need idle handoff
         improvement */

      raho_sect_idx = hdrsrchreacq_get_ho_candidate();
    }
    else
    {
      /* the sleep time is less than 1 second, we want to use dynamic 
         thresholds*/

      raho_sect_idx = hdrsrchreacq_get_ho_candidate_dynamic_thresh();
    }

    if ( ( raho_sect_idx > 0 ) && ( raho_sect_idx < HDR_MAX_SRCH_CNT ) ) 
    {
      /* find a handoff candidate */
    
      sect      = hdrsrchdrv.srch_list[raho_sect_idx].sect_ptr;
      slew_x2   = hdrsrchdrv.resbuf[raho_sect_idx].posx2[0];
      slew_x2  -= sect->win_cenx2;
    }

  } /* if not intra-scc */

  hdrsrchreacq.slew_x2 = slew_x2;

  /*------------------------------------*
   * Check if aset needs to be replaced *
   *------------------------------------*/
  if ( sect != NULL )
  {
     /* A handoff candidate is found*/

    hdrsrchmsg.fpd_msg.forcePreamDetect = 0;
      /* Disable FPD */

    hdrsrchmsg_send_msg( &hdrsrchmsg.fpd_msg.hdr, 
                         sizeof( hdrfw_fpd_msg_t ) );
      /* Disable forced preamble detection */

    hdrsrchreacq.forced_preamble_activated = FALSE;
      /* Set flag that FPD has not been activated */


    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "RAHO to: PN=%d Eng=%d Slew=%d cx2",
                    sect->pn_offset, sect->total_eng, slew_x2 );

    hdrsrchlog.reacq_info.slew_x2 = slew_x2;
      /* Records slew info                 */

    prev_slew_x2  = hdrsrchdrv.srch_list[0].srch_task.res_ptr->posx2[0];
    prev_slew_x2 -= hdrsrchdrv.srch_list[0].sect_ptr->win_cenx2;

#ifdef FEATURE_HDR_REVB
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "RAHO from: PN=%d Eng=%d Slew=%d cx2",
                hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset,
                hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng,
                prev_slew_x2 );
#else
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "RAHO from: PN=%d Eng=%d Slew=%d cx2",
                    hdrsrchset.aset[0]->pn_offset,
                    hdrsrchset.aset[0]->total_eng,
                    prev_slew_x2 );
#endif /* FEATURE_HDR_REVB */

    hdrsrchreacq_update_reacq_info( sect, HDRLOG_REACQ_RAHO );
      /* Update REACQ INFO for RAHO */

#ifdef FEATURE_HDR_QUICK_IDLE
    /* enable QPCH decoding only when QPCH feature is enabled */
    if(hdrsrchsleep_is_qpch_enabled())
    {
      hdrsrchreacq_enable_qpch_det();
      /* Disable QPCH FPD and enable QPCH preamble detection */
    }
#endif /* FEATURE_HDR_QUICK_IDLE */
    
    hdrsrchutil_aset_idle_handoff( sect );
      /* Handoff to new pilot */

    hdrsrch.ho_time = timetick_get_ms();
      /* Record the current time */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HO time %d",
                    hdrsrch.ho_time );

    hdrsrchreacq.reacquired = TRUE;
      /* We reacquired */

    hdrsrchreacq_report( sect, TRUE );
      /* Inform protocol of ReAcq Handoff to given sector */

  }
  else if ( aset_inst_eng > 0)
  { /* no handoff candidate is found */
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Reacq: PN=%d Eng=%d Slew=%d cx2",
                    aset_sect->pn_offset, aset_inst_eng, slew_x2 );

      /* Only reports slew to sleep ctrl if reacq succeeds with previous pilot.
         Don't report slew if there is RAHO
      */
#ifndef FEATURE_HDR_DIME_MODEM
    sleepctl_error_feedback( SLEEPCTL_HDR, slew_x2/2 );
      /* Feedback the sleep error, in chipx1's */
#else /* FEATURE_HDR_DIME_MODEM */

    // We report slew only if it is not intra-scc sleep (more than 64 slots)
    if ( hdrsrchsleep_get_sleep_slots() > HDR_SRCH_MAX_INTRA_SCC_SLEEP_SLOTS ) 
    {
      slpc_error_feedback( SLPC_HDR, slew_x2*4);
      /* Feedback the sleep error in chipx8. Confirmed with Chris Barrett that this is
         the correct unit */
 
      HDR_MSG_SRCH ( MSG_LEGACY_HIGH, "slpc_error_feedback() is called" );
    }
#endif /* FEATURE_HDR_DIME_MODEM */

    hdrsrchlog.reacq_info.slew_x2 = slew_x2;
      /* Records slew info                 */

    hdrsrchsleep_send_msg_update_scc_offset( );
      /* Restore the known SCC offset since we acquired the old pilot */

    hdrsrchreacq_update_reacq_info( aset_sect, HDRLOG_REACQ_OLD_PILOT );
      /* Update REACQ INFO for re-acq on the old pilot */

    hdrsrchreacq.reacquired = TRUE;
      /* We reacquired */

    hdrsrchreacq_post_proc_reacq( slew_x2 );
      /* post process reacquisition */
  }
  else
  {
    hdrsrchreacq.reacquired = FALSE;
      /* We haven't reacquired (yet) */
  }

} /* hdrsrchreacq_aset_mgmt( ) */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_STOP_SRCH_ATTEMPTS

DESCRIPTION
  This function returns TRUE if the all previous reacq srches failed for this
  wakeup and either a timeout (intra-scc sleep) or a minimum number of
  searches have been performed (non-intra-scc sleep).

  - If intra-SCC is disabled, make sure minimum number of searches are
    attempted AND all neighbors are searched.
  - If intra-SCC is enabled, then return TRUE if the current HSTR is
    greater than or equal to (converted to half slots):
    2 * (SCC start slot + SCC Offset +  HDRSRCHREACQ_INTRA_SCC_MAX_SRCH_SLOTS)
    Note: Since HSTR is checked during processing of srch dump, the last
    srch dump will be the half slot BEFORE the actual SCC boundary of the
    next interlaced slot.  If an extra srch was performed, the srch dump
    processing will probably occur too late, i.e., after the SCC boundary.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL boolean hdrsrchreacq_stop_srch_attempts( void )
{
  boolean               stop_srch_attempts;
    /* TRUE when no more search attempts are necessary */

  uint16                scc_offset_adjustment;
    /* SCC offset adjustment */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchsleep_get_intra_scc_valid() )
  {
    scc_offset_adjustment = hdrsrchsleep_get_scc_offset();
      /* Get saved SCC offset */

    if ( scc_offset_adjustment > HDRSRCHREACQ_MAX_SCC_OFFSET )
    {
      scc_offset_adjustment = 3;
        /* The saved SCC offset could be 0xFFFF to decode all interleaves.
           Assume worst case and set SCC offset adjustment to 3. */
    }

    /* For Intra-SCC reacq search failure, keep searching until just before
       the 2nd interleave slot (4 extra slots).  Need to take SCC offset into
       account too.*/
    if ( ( HDRHSTR_GET_COUNT() % HDRSRCHREACQ_HS_PER_SCC ) >=
         ( 2 * ( hdrsrchsleep_get_intra_scc_start_slot() +
                 scc_offset_adjustment +
                 HDRSRCHREACQ_INTRA_SCC_EXTRA_SRCH_SLOTS ) ) )
    {
      stop_srch_attempts = TRUE;
        /* Stop reacq searching */
    }
    else
    {
      stop_srch_attempts = FALSE;
        /* Continue reacq searching */
    }

  }
  else /* Not intra-SCC reacq */
  {
    /* Check if minimum number of searches AND all nset pilots have been
       searched */
    if ( ( hdrsrchreacq.search_list_cnt >= HDRSRCHREACQ_MIN_SRCH_ATTEMPTS ) &&
         ( hdrsrchreacq.nset_pilots_searched >= hdrsrchset.nset_cnt ) )
    {
      stop_srch_attempts = TRUE;
        /* Stop reacq searching */
    }
    else
    {
      stop_srch_attempts = FALSE;
        /* Continue reacq searching */
    }
  }

  return stop_srch_attempts;

} /* hdrsrchreacq_stop_srch_attempts */

/*===========================================================================

FUNCTION HDRSRCHREACQ_PROCESS_FAST_ACQ

DESCRIPTION
  This function reports the fast acq result to upper layer.  If re-acq is
  successful, this function also sets the signal to transition to Idle Monitor
  state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchreacq_process_fast_acq( void )
{

    if ( hdrsrchreacq.fast_acq_cb != NULL )
    {
      if ( hdrsrchreacq.reacquired == TRUE )
      {
        HDR_MSG_SRCH( MSG_LEGACY_MED, "fast_acq success");
        hdrsrchrf_update_priority(HDRSRCHRF_DEMOD_PAGE);
        /* Go back to normal priority mode */

        hdrsrchreacq.fast_acq_cb( HDRSRCH_FAST_ACQ_SUCCEEDED );
        /* Inform upper layer that fast acq succeeds */

        HDRSRCH_SIG_SET( HDRSRCH_TIME_VALID_SIG );
          /* Set signal to transition to the next state, timing is now valid */
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_MED, "fast_acq failure");
        hdrsrchreacq.fast_acq_cb( HDRSRCH_FAST_ACQ_FAILED );
        /* Inform upper layer that fast acq fails */
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_L2D_RESEL_FAILURE);
#endif
      }
    }

    hdrsrchreacq.fast_acq_cb = NULL;
      /* clear fast_acq_cb since only the first reacq after timesync is for 
         fast acq, the rest are normal reacq */

  hdrsrchutil_restore_aset_win_size();
    /* Restore the actual Aset window size */

}/* hdrsrchreacq_process_fast_acq */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_DUMP

DESCRIPTION
  This function handles the reacquisition search dump signal.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchreacq_dump( void )
{
  uint16                          hstr;
    /* Half slot timing reference at finger assignment */

  uint16                          deadtime_hstr;
    /* The hstr time stamp relative to the current cc_start. */

  uint16                          scc_offset;
    /* SCC Offset */

  boolean                         is_subsync_wakeup = FALSE;
    /* Whether the current wakeup is to decode a subsynce packet. */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  boolean                         default_search;
    /* To submit default search or not */
#endif 

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_PROFILE( REACQ_DUMP );
    /* sleep timeline profiling output if enabled */

  hdrsrchreacq.search_list_cnt++;
    /* Increment the search count */


  /* check if we did full PN search and triage the result */
  hdrsrchutil_chk_triage_full_pn( );

  hdrsrchutil_process_search_set( hdrsrchreacq_aset_mgmt );
    /* Process reacquisition search */
 
  hdrsrchreacq_process_fast_acq();

  if ( hdrsrchreacq.reacquired )
  {
    /* Acquisition was successful */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
    if( hdrsrch.idle_state_div.idle_div_enabled )
    {
      hdrlog_register_slot_logging_cb ( HDRLOG_SRCHDIV_MDSP_SLOT_LOGGING_CLIENT,
                                        hdrsrchidle_process_psinr_mdsp_logging_event);
        /* Register call back for slot based logging of psinr */
    }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */


    hdrsrchdrv_start_slam( FALSE, 0, 0, 0, 0 );
      /* Adjust timing to the sector we reacquired on */

    hdrsrch_add_to_wait_mask( HDRSRCH_TIME_VALID_SIG );
      /* We've acquired.
         Watch for time being set valid, so we can advance the state. */

    hstr = HDRHSTR_GET_COUNT();
      /* Get the time that the finger has been assigned at */

    if ( hdrsrchsleep_get_intra_scc_valid() == FALSE )
    {
      deadtime_hstr = 
        ( ( ( hdrsrchsleep_get_cc_start() * 2 ) + HDRSRCHREACQ_HS_PER_SCC ) - 
            ( hstr % HDRSRCHREACQ_HS_PER_SCC) ) % HDRSRCHREACQ_HS_PER_SCC;
        /* Calculate the dead time, in units of half slots */
    
      /* Convert deadtime to a signed number. */
      if ( deadtime_hstr > ( HDRSRCHREACQ_HS_PER_SCC / 2 ) )
      {
        hdrsrchlog.sleep_info.dead_time_hs = deadtime_hstr - HDRSRCHREACQ_HS_PER_SCC;
      }
      else
      {
        hdrsrchlog.sleep_info.dead_time_hs = deadtime_hstr;
      }
    }
    else
    {
      /* Calculate Intra-SCC deadtime. First determine SCC Offset. */

      scc_offset = hdrsrchsleep_get_scc_offset();
      if ( scc_offset > HDRSRCHREACQ_MAX_SCC_OFFSET )
      {
        scc_offset = 0;
          /* SCC Offset might be 0xFFFF for decoding all interleaves.  Assume 0
             slots for wakeup purpose */
      }

      deadtime_hstr = 
        ( ( 2 * ( hdrsrchsleep_get_intra_scc_start_slot() + scc_offset ) -
          ( hstr % HDRSRCHREACQ_HS_PER_SCC ) ) % HDRSRCHREACQ_HS_PER_SCC );
        /* Calculate deadtime (SCC start slot - now) in half_slot unit */

      hdrsrchlog.sleep_info.dead_time_hs = deadtime_hstr;
        /* Save the dead time into the sleep info logging packet. */
    }

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                    "Fing assigned @hstr=%d (%d) DeadTime=%dms",
                    hstr & 511, hstr,
                    HALFSLOTS_TO_MS( hdrsrchlog.sleep_info.dead_time_hs ) );

    HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_RACQ_DUMP );
  }
  else
  {
    /* We've completed a search, and it failed ... move on to the next */

    hdrsrchreacq.win_size_fctr = hdrsrchreacq.max_win_size_fctr;
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "REACQ FAILURE:next Reacq srch window size fctr: %d",
                   hdrsrchreacq.win_size_fctr);
      /* double the window size factor due to reacq failure */

    hstr = HDRHSTR_GET_COUNT();

    HDR_MSG_SRCH_5( MSG_LEGACY_HIGH, "Reacq Dump failed: Is HSTR:%d >= %d ?  intra_valid:%d start_slot:%d SCC_Offset:%d",
                    ( hstr % HDRSRCHREACQ_HS_PER_SCC ),
                    ( 2 * ( hdrsrchsleep_get_intra_scc_start_slot() +
                               hdrsrchsleep_get_scc_offset() +
                               HDRSRCHREACQ_INTRA_SCC_EXTRA_SRCH_SLOTS ) ),
                    hdrsrchsleep_get_intra_scc_valid(),
                    hdrsrchsleep_get_intra_scc_start_slot(),
                    hdrsrchsleep_get_scc_offset() );

    /* Search at least minimum number of times AND make sure all neighbors are
       searched.  If Intra-SCC is enabled, search an extra
       HDRSRCHREACQ_INTRA_SCC_EXTRA_SRCH_SLOTS slots. */
    if ( hdrsrchreacq_stop_srch_attempts() == TRUE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Lost acquisition after sleep" );


  /* TBD: need stop fpd msg */
      /*HDRHSTR_STOP_FORCEPREAM_DET( );*/
        /* Deactivate the CC forced premable detection in firmware */

      hdrsrchreacq.forced_preamble_activated = FALSE;
        /* Set flag that FPD has not been activated */


      /* Note: To maintain legacy behavior, update reacq and sleep info for
         Intra-SCC reacq failure too. */

      hdrsrchreacq_update_reacq_info( NULL, HDRLOG_REACQ_LOST );
        /* Update REACQ INFO for lost acquisition   */

      if ( hdrsrchsleep_get_intra_scc_valid() == TRUE )
      {
        INC_SAT( hdrsrchlog.sleep_info.intra_scc_reacq_failure_cnt );
          /* Increment the intra SCC reacq failure count in the SLEEP INFO log
             packet. */
      }
      else
      {
      INC_SAT( hdrsrchlog.sleep_info.reacq_loss_count );
        /* Also increments the reacq failure count in SLEEP INFO log packet.
           It saturates at 0xffff as well                                    */
      }

      hdrsrchlog.sleep_info.cc_status = HDRLOG_CC_REACQ_FAIL;
        /* Track the CC status. */
#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV 

      if( hdrsrch.idle_state_div.idle_div_enabled )
      {
        HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Idle Div: Reacq failed will "
                                       "enable div for reacq next time");

        hdrsrch.idle_state_div.wakeup_info.syncc_cc_missed = TRUE;
        hdrsrch.idle_state_div.wakeup_info.reacq_failed = TRUE;
        hdrsrch.idle_state_div.wakeup_info.div_enabled = TRUE;
       
     }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV  */

      /* Determine whether we are waking up for a subsync packet for
         logging. */
      if ( ( hdrsrchsleep_get_intra_scc_valid() == FALSE ) &&
           ( hdrsrchsleep_get_last_sleep_sci() < 6 ) &&
           ( hdrsrchsleep_get_cc_start() != 0 ) )
      {
        is_subsync_wakeup = TRUE;
      }

      hdrsrchlog_update_cc_info( HDRHSTR_GET_COUNT() % 
                                   ( 512 - ( hdrsrchsleep_get_cc_start() * 2 ) ),
                                 0xff,
                                 0xff,
                                 FALSE,
                                 TRUE,
                                 hdrsrchsleep_is_qpch_enabled()? 0 : 1, 
                                 is_subsync_wakeup );
        /* Update the control channel infomation for SLEEP INFO log packet  */

#ifndef FEATURE_HDR_DDARF
      hdrsrchutil_stop_timer_ms( hdrsrchlog.awake_timer );
      hdrsrchlog.triggered.sleep  = FALSE;
        /* Stop sleep_info logging in case of re-acq failure                 */
#endif /* FEATURE_HDR_DDARF */


#ifdef FEATURE_HDR_DDARF
       hdrsrchreacq_report( NULL, FALSE );
        /* We have finished reacquisition attempts so go ahead and wake protocols
           back up into monitor state. But indicate that the reacq failed. */

      hdrsrchrf_force_resume_agc_off();
        /* Next idle slotted cycle should be with the resume AGC algorithm off. */

      hdrsrch_add_to_wait_mask( HDRSRCH_TIME_VALID_SIG );
        /* We've acquired.
           Watch for time being set valid, so we can advance the state. */

      hstr = HDRHSTR_GET_COUNT();
        /* Get the time that the finger has been assigned at */

      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                      "Failed Reacq @hstr=%d (%d) DeadTime=%dms",
                      hstr & 511, hstr,
                      HALFSLOTS_TO_MS( (512-(hstr&511)) ) );

      if ( hdrsrchsleep_get_intra_scc_valid() == FALSE )
      {
        /* For the wakeup after Intra-SCC sleep, we don't do OFS if reacq
           fails. 
        */

        hdrsrchofs_request_idle_ofs();
          /* indicate that OFS search is required due to reacq failure */
      }
      
#else /* !FEATURE_HDR_DDARF */
      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_LOST_ACQ_AFTER_SLEEP );
        /* Inform protocol that we've lost the system, and exit to inactive. */
#endif /* !FEATURE_HDR_DDARF */

    }
    else
    {
      /* Assuming a srch list size of 16, we need to search at most 5 different
       * lists since the max size of the neighbor set is HDR_MAX_NSET_SIZE.
       * With the max size of the aset is 1 and the max size of cset is 6 (skip
       * rset during reacq) 9 entries remain for searching the nset.  If
       * HDR_MAX_NSET_SIZE is 40, then 40 / 9 = 5 srch lists, worst case.
       */

      hdrsrchset_reacq_reset_filters( );
        /* Reset pilot filter lengths after failed reacq search. */

      hdrsrchreacq_prepare_search_list( );
      /* Check the rest of the nset for a good pilot */
      /* reprogram search list for reacq */

      default_search = TRUE ;
        /* Submit second search as default search always */
      
      hdrsrchutil_program_reacq_search( default_search );
      /* send another search to the searcher */

      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "No Pilot, try again. Srch cnt %d.  Srch %d of %d NSET",
                      hdrsrchreacq.search_list_cnt,
                      hdrsrchreacq.nset_pilots_searched,
                      hdrsrchset.nset_cnt);
    }
  }

} /* hdrsrchreacq_dump( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_DEACTIVATE_CMD

DESCRIPTION
  Exit immediately to the deactivate state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Command status

SIDE EFFECTS
  Disables the HDR autokicker

===========================================================================*/

LOCAL errno_enum_type hdrsrchreacq_deactivate_cmd( void )
{
  errno_enum_type               status;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "DEACTIVATE_CMD: Reacq state=%d", 
                  hdrsrchreacq.state );

  if ( ( hdrsrchreacq.slept == TRUE ) && ( hdrsrchreacq.chipx16_on == FALSE ))
  {
    /* Deactivate cmd is received before sleep warmup done, retry the cmd */

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Retry DEACTIVATE_CMD: Reacq slept %d chipx16_on %d",
                    hdrsrchreacq.slept, hdrsrchreacq.chipx16_on );

    hdrsrchreacq.deactivate_pending = TRUE;

    status = E_AGAIN;
    return status;
  }

  switch ( hdrsrchreacq.state )
  {
    case HDRSRCHREACQ_READY_TO_TUNE_STATE:
    case HDRSRCHREACQ_WARMUP_STATE:
    case HDRSRCHREACQ_TUNE_STATE:
    {
      /* RF Warmup and Tuning is in progress - complete it for consistancy.
         Deactivate when tuning is complete. */

      hdrsrchreacq.deactivate_pending = TRUE;
        /* Signal hdrsrchsleep_tune_done( ) to deactivate */

      status = E_IN_PROGRESS;
        /* Command will be completed by hdrsrchsleep_deactivate() */

    } break;

    default:
    {
      /* RF Warmup and Tuning is done; reacquisition searching has started.
         Treat as fully awake, and transition to start state. */

      hdrsrchst_init_inactive_state( );
        /* And transition back to start state */

      status = E_SUCCESS;

    } break;

  } /* switch */

  return status;

} /* hdrsrchreacq_deactivate_cmd */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_CMD

DESCRIPTION
  This function dispatches commands to the appropriate command handler

DEPENDENCIES
  None

PARAMETERS
  cmd   - Command to be processed

RETURN VALUE
  E_SUCCESS       - command has been successfully executed
  E_FAILURE       - command has failed
  E_NOT_SUPPORTED - command is not support
  E_IN_PROGRESS   - command has been started, but has not completed
  E_AGAIN         - command cannot be started in present state, but is a
                    valid command.  Execute hdrsrch_retry_cmd( ) at a
                    later point in time to reattempt the command.
                    [hdrsrch_retry_cmd( ) is called at every state change]

SIDE EFFECTS
  If E_IN_PROGRESS or E_AGAIN is returned, command processing is disabled
  until the command buffer is passed to hdrsrch_complete_cmd( ).

===========================================================================*/

LOCAL errno_enum_type hdrsrchreacq_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d", cmd->name );

  /* We are attempting to reacquire the system ... */

  switch ( cmd->name )
  {
    case HDRSRCH_DEACTIVATE_CMD:
    {
      /* ... but the protocol doesn't want us to.  Give up immediately */

      status = hdrsrchreacq_deactivate_cmd( );
        /* Exit HDR gracefully */

    } break;

#ifdef FEATURE_HDR_BCMCS
    case HDRSRCH_UPDATE_BC_INFO_CMD:

      hdrsrchbc_set_bc_status( cmd->params.bc_info.bcmcs_status );

      status = E_SUCCESS;
      
      break;
#endif /* FEATURE_HDR_BCMCS */
    default:
    {
      /* ... and we are being asked to do something else.
         What ever it is will have to wait until we've reacquired */

      status = E_AGAIN;
    }

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchreacq_cmd(cmd) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_INIT

DESCRIPTION
  Reacq State Initialization

DEPENDENCIES
  Sleep on an HDR System

PARAMETERS
  last_state  - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  RF is turned on, tuned, clocks are enabled, demod enabled,
  fingers assigned, etc.

===========================================================================*/

LOCAL void hdrsrchreacq_init( hdrsrch_state_enum_type last_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_PROFILE( REACQ_INIT );
    /* sleep timeline profiling output if enabled */

  HDR_PWR_PROFILE_STRB_TOGGLE( HDRSRCH_PWR_EVENT_RACQ_INIT );

  hdrsrch_default_init( last_state );
    /* Use default handling */

  hdrsrchreacq.search_list_cnt = 0;
    /* Number of reacq searches */

  hdrsrchreacq.nset_pilots_searched = 0;
    /* Clear total number of nset pilots searched */

  /* Power on the RF (if necessary) and tune to the Active Set Frequency */

  hdrsrchutil_start_timer( &hdrsrchlog.reacq_timer );
    /* Start the timer, for measuring RF warmup and tune time            */

  hdrsrchlog.triggered.reacq                = TRUE;
    /* Indicates that logging of REACQ_INFO has been triggered           */

  hdrsrchlog.reacq_info.last_srch_state     = last_state;
    /* Record the last HDRSRCH state  */

  hdrsrchlog.reacq_info.result              = HDRLOG_REACQ_NULL;
  /* No result when triggered  */

  hdrsrchlog.reacq_info.slew_x2             = (int8)0x7f;
    /* Initialize slew_x2 to 0x7f */

  hdrsrchlog.triggered.sleep                = TRUE;
    /* Indicates that logging of SLEEP_INFO has been triggered           */
  
  hdrsrchlog.sleep_info.cc_status           = HDRLOG_CC_NULL;
    /* No control channel status when triggered  */

  hdrsrchlog.sleep_info.ofs_status          = HDRLOG_OFS_NO_OFS;
    /* No OFS status at when triggered           */

  hdrsrchlog.sleep_info.handoff_count       = 0;
    /* This field records the handoff count during the current wake-up,
       so it needs to be initialized to 0 at the start of every wake-up  */

  hdrsrchlog.sleep_info.num_reacq_pilots_srched = 0;
    /* Reset number of reacq pilots searched at the start of every wake-up */

  if ( last_state == HDRSRCH_SLEEP_STATE )
  {
    hdrsrchreacq.fast_acq_cb = NULL;
      /* clear fast_acq callback in case it is not cleared at the end of fast acq */

    hdrsrchreacq.slept = TRUE;
      /* Remember that we actually were asleep */

    hdrsrchreacq.chipx16_on = FALSE;
      /* Chipx16 is not on since we were just sleeping. */

    hdrmdsp_fws_resume_app( );
      /* Wake up FW if it's been suspended by hdr app */

    hdrmdsp_config_hdr_app( HDRFW_STATE_ACTIVE, TRUE, TRUE );
      /* Enable the HDR MDSP Application.*/

    hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_REACQ_PAGING );
      /* Enable SRCH4 for HDR */

    hdrsrchafc_request_continue_trk();
      /* Get permission to close the TCXO loop before tuning and
         assigning fingers.  Someone else could conceivably be
         controlling the TCXO, unless I have open loop control 
         already. */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    hdrmultirat_dsda_mode_change();
      /* If in DSDA tell FW */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */  

  }
  else if ( last_state == HDRSRCH_TIME_SYNC_STATE )
  {
    /* came from timesync state. This is for fast acq */

    hdrsrch_add_to_wait_mask( HDRSRCH_DUMP_SIG | HDRSRCH_LOSTDUMP_SIG );

    hdrsrchreacq.slept = FALSE;
      /* Remember that we actually did not sleep */

    hdrdec_reset_pkt_sequences();
      /* Reset packet sequence numbers */

    hdrdec_activate();
      /* activate HDR decoder before going to 1x */

    hdrsrchreacq.chipx16_on = TRUE;
    /* Chipx16 is still on when coming from a suspend/timesync state */

    hdrdec_enter_mode( HDRDEC_MONITOR_MODE );
    /* Set the decoder for idle/sync mode where the packets are delivered
       as soon as they are decoded.  Usually decoder mode is set in sync
       state. Since we skipped Sync state, we need to set it here. */                                  

    hdrsrchafc_request_fast_acq();
    /* Request AFC state machine to perform fast acquisition mode, which
       will eventually move to continuous tracking mode similar to re-acq. */
     
    #ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    hdrmultirat_dsda_mode_change();
      /* If in DSDA tell FW */
    #endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */  

#ifdef FEATURE_HDR_DR_DSDS
    hdrsrchrf_update_dr_mode();
      /* Check with TRM & update mode preference if we are in DR mode */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,"HDR updating DR mode" );
#endif /* FEATURE_HDR_DR_DSDS */

     
  }
  else
  {
    /* last state was probably IDLE_SUSPENDED STATE */

    hdrsrchreacq.fast_acq_cb = NULL;
      /* clear fast_acq callback in case it is not cleared at the end of fast acq */

    hdrsrchreacq.slept = FALSE;
      /* Remember that we actually did not sleep */

    hdrdec_reset_pkt_sequences();
      /* Reset packet sequence numbers */

    hdrdec_activate();
      /* activate HDR decoder before going to 1x */

    HDRSRCH_SIG_SET( HDRSRCH_TIME_VALID_SIG );
      /* Chipx16 is on, thus time must be valid */

    hdrsrchreacq.chipx16_on = TRUE;
    /* Chipx16 is still on when coming from a suspend/timesync state */

#ifdef FEATURE_HDR_QTA
    if ( hdrsrchtc_is_qta_enabled() )
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH,"HDR QTA end, turning Tx OFF!" );

      hdrsrchtc_disable_tx_qta();

      hdrsrchreacq_tune_done( HDRSRCHRF_TUNE_SUCCESS );
    }
    else
#endif /* FEATURE_HDR_QTA */
    {

      hdrsrchafc_request_continue_trk();
        /* Get permission to close the TCXO loop before tuning and
           assigning fingers.  Someone else could conceivably be
           controlling the TCXO, unless I have open loop control 
           already. */

      hdrsrchreacq.state = HDRSRCHREACQ_READY_TO_TUNE_STATE;
        /* Ready to tune once we get permission to close the TCXO loop.  
           Without TCXO, we really can't reacq since we don't have 
          rotators. */
    }

  }

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_FW
  hdrmultirat_send_dsdx_frequency_cfg_msg( );
    /* During sleep FW loses freq id info, 
       so resend during reacq */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_FW */


#ifdef FEATURE_HDR_UNIFIED_MICRO_PRIORITY
  hdrmultirat_priority_switch( HDRMULTIRAT_HIGH_PRIORITY );
    /* Increase priority for reacq */
#else
  hdrmultirat_priority_switch( HDRMULTIRAT_HIGH_PRIORITY_10 );
    /* Increase priority for reacq */
#endif /* FEATURE_HDR_UNIFIED_MICRO_PRIORITY */

  hdrmultirat_report_coex_power_with_bias();
    /* Report power from when we slept and start
       timer to keep reporting after that */

  timer_set( &hdrsrch.coex_timer, HDRMULTIRAT_COEX_PWR_TIMER_MS,
              0, T_MSEC );
    /* Set the timer to go off again after 20 ms . */

#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

  if ( ( last_state == HDRSRCH_TIME_SYNC_STATE ) ||
       ( last_state == HDRSRCH_SLEEP_STATE ) )
  {
    hdrsrchreacq.state = HDRSRCHREACQ_READY_TO_TUNE_STATE;
      /* Ready to tune once we get permission to close the TCXO loop.  
         Without TCXO, we really can't reacq since we don't have 
         rotators. */
  }


  hdrsrchsleep_update_prev_cc_start();

  HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Reset reacq filters");
  hdrsrchset_reacq_reset_filters( );
    /* Reset pilot filter lengths, since we've been asleep for a long
       time. */

  
} /* hdrsrchreacq_init */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHREACQ_DONE

DESCRIPTION
  Sleep State Finalization

DEPENDENCIES
  Exiting Sleep state

PARAMETERS
  next_state  - State we are transitioning to

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchreacq_done( hdrsrch_state_enum_type next_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW

#ifdef FEATURE_HDR_UNIFIED_MICRO_PRIORITY
  hdrmultirat_priority_switch( HDRMULTIRAT_NORMAL_PRIORITY );
#else
  hdrmultirat_priority_switch( HDRMULTIRAT_LOWER_PRIORITY_20 );
    /* Increase priority for reacq */
#endif /* FEATURE_HDR_UNIFIED_MICRO_PRIORIY */

#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

  hdrsrch_default_done( next_state );
    /* Use default handling */

  hdrsrch_kill_signal( HDRSRCH_TIME_VALID_SIG );
    /* Clear TimeValid processing */

  hdrsrch_kill_signal( HDRSRCH_RXCHIPX16_SIG );
    /* Clear RxChipx16 processing */

  hdrsrchlog_log_reacq_info( );
    /* Log REACQ_INFO packet when leaving REACQ state   */

  hdrsrchsleep_reset_wakeup_reason( );
    /* AT just wokeup, there is no pending wakeup       */

} /* hdrsrchreacq_done( ) */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
/*==========================================================================

FUNCTION        HDRSRCHREACQ_DIV_ENABLED

DESCRIPTION
  This function processes div enabled event.

DEPENDENCIES
  None

INPUTS
  boolean - div_successfully_enabled
  TRUE:  diversity was successfully enabled.
  FALSE: diversity was not enabled in time allowed.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchreacq_div_enabled
( 
  boolean div_successfully_enabled
)
{

  int               rx0_agc, rx1_agc;
    /* Rx0 and Rx1 AGC measurements */

  boolean           diversity;
    /* Diversity settings */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchrf_get_rx_power( &rx0_agc, &rx1_agc, &diversity );
    /* get rxagc */

  if( div_successfully_enabled )
  {
    hdrsrch.idle_state_div.wakeup_info.div_timeout = FALSE;

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,"Idle Div: pri rxagc=%d "
                    "diversity rxagc =%d",
                    rx0_agc,rx1_agc);
  }
  else
  {
    hdrsrch.idle_state_div.wakeup_info.div_timeout = TRUE;
    HDR_MSG_SRCH( MSG_LEGACY_HIGH,"Idle Div: Div enable timeout,"
                                  " reacq without div!");
  }

  hdrsrchreacq_start_search();

} /* hdrsrchreacq_div_enabled */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHREACQ_PROCESS_INT_CMD

DESCRIPTION
  This function processes internal commands for reacq state.

DEPENDENCIES
  None

INPUTS
  payload   - Command+payload to be processed

RETURN VALUE
  None
  
SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchreacq_process_int_cmd
(
  hdrsrchsm_cmd_payload_type                     *payload
    /* The internal command + payload */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch ( payload->cmd )
  {
    case HDRSRCH_AFC_CLOSE_LOOP_CMD:
    
      if ( hdrsrchreacq.state == HDRSRCHREACQ_READY_TO_TUNE_STATE)
      {

        hdrsrchreacq_enable_and_tune( hdrsrchreacq.chipx16_on );
          /* Now that we have the rights, go ahead start the
             tune and try to reacquire. */
      }
      break;

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
    case HDRSRCH_DIV_ENABLED_CMD:
    
      if ( hdrsrchreacq.state == HDRSRCHREACQ_WAIT_FOR_DIV_STATE )
      {

        hdrsrchreacq_div_enabled( TRUE );
          /* Now that diversity is up, start the reacq. */
      }
      break;

    case HDRSRCH_DIV_ENABLE_TIMEOUT_CMD:
    
      if ( hdrsrchreacq.state == HDRSRCHREACQ_WAIT_FOR_DIV_STATE )
      {

        hdrsrchreacq_div_enabled( FALSE );
          /* Diversity could not be enabled in given time,
             start the reacq without Div */
      }
      break;
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

    default:

      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Unexpected internal cmd %d at Reacq state",
                      payload->cmd );
      break;
  }

}  /* hdrsrchreacq_process_int_cmd */ 


/* EJECT */
/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/

LOCAL hdrsrch_state_struct_type hdrsrchreacq_state =
{
  HDRSRCH_REACQ_STATE,
  hdrsrchreacq_init,
  hdrsrchreacq_done,
  hdrsrchreacq_cmd,
  hdrsrchreacq_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrch_default_sched,
  hdrsrch_default_lost_dump,
  hdrsrch_default_timeout,
  hdrsrch_default_gp_timer0,
  hdrsrch_default_gp_timer1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrchreacq_on_time_valid,
  hdrsrchreacq_rxchipx16_on,
  hdrsrch_default_pri_chain_event,
  hdrsrchreacq_process_int_cmd,
  hdrsrch_default_process_low_pri_rsp_ind
};


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCH_INIT_REACQ_STATE

DESCRIPTION
  This function causes the state machine to begin reacq state processing.

DEPENDENCIES
  Idle state

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Transitions to the reacq state

===========================================================================*/

void hdrsrchreacq_init_reacq_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_change_state( &hdrsrchreacq_state );
    /* Switch to sleep state processing */

} /* hdrsrchreacq_init_reacq_state( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHREACQ_SET_WAKEUP_REPORT

DESCRIPTION
  This function specifies who to inform when we wakeup.

DEPENDENCIES
  None

PARAMETERS
  wakeup_cb - Who to inform when we wakeup

RETURN VALUE
  None

SIDE EFFECTS
  Wakeup cycle and slot is computed.

===========================================================================*/

void hdrsrchreacq_set_wakeup_report
(
  hdrsrch_wakeup_cb_type                      wakeup_cb
    /* What to do when we wakeup? */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchreacq.wakeup_cb = wakeup_cb;
    /* Remember Sleep Until time and Wakeup callback */

} /* hdrsrchreacq_set_wakeup( ) */


/*===========================================================================

FUNCTION HDRSRCHREACQ_WAKEUP_REPORT

DESCRIPTION
  This function reports wakeup & reacquisition, with a possible handoff
  to a new pilot.

DEPENDENCIES
  None

PARAMETERS
  sect - Sector to a reacquisition handoff was performed, NULL if old pilot.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchreacq_wakeup_report
(
  hdrsrch_sect_struct_type      * sect
    /* RAHO sector, or NULL if no reacquisition handoff  */
)
{
  sys_link_type                   link;
     /* Link report structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchreacq.reacquired = TRUE;
    /* We have reacquired. */

  if ( hdrsrchreacq.wakeup_cb != NULL )
  {
    if ( sect != NULL )
    {
      link.pilot_pn           = sect->pn_offset;
      link.channel            = sect->chan;

      hdrsrchreacq.wakeup_cb( sect->link_id, &link,
                              hdrsrchsleep_get_wakeup_reason( ),
                              TRUE );
        /* Inform protocol of ReAcq Handoff to given sector */
    }
    else
    {
      hdrsrchreacq.wakeup_cb( HDRSRCH_REF_LINK_ID, NULL,
                              hdrsrchsleep_get_wakeup_reason( ),
                              TRUE );
        /* Inform protocol that we woke up on same pilot */
    }

    hdrsrchreacq.wakeup_cb = NULL;
      /* Clear the report callback for next time */
  }

} /* hdrsrchreacq_wakeup_report( ) */


#ifdef FEATURE_HDR_DDARF
/*==========================================================================

FUNCTION HDRSRCHREACQ_LAST_REACQ_SUCCEEDED

DESCRIPTION
  This function returns whether the last reacquisition succeeded.

DEPENDENCIES
  None 

PARAMETERS
  None

RETURN VALUE
  Whether the reacquisition succeeded.

SIDE EFFECTS
  None

==========================================================================*/

boolean hdrsrchreacq_last_reacq_succeeded( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrsrchreacq.reacquired;
    /* Return whether the last reacquisition succeeded. */

} /* hdrsrchreacq_last_reacq_succeeded */
#endif /* FEATURE_HDR_DDARF */


 /* EJECT */
/*===========================================================================
FUNCTION HDRSRCHREACQ_GET_FPD_ACTIVATION_STATUS

DESCRIPTION
  This function returns the flag whether forced premable detection
  has been activated or not.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
    
RETURN VALUE
  Boolean flag indicating whether forced premable detection
  has been activated or not.

SIDE EFFECTS
  None.
===========================================================================*/
boolean hdrsrchreacq_get_fpd_activation_status( void )
{
  return ( hdrsrchreacq.forced_preamble_activated );

} /* hdrsrchreacq_get_fpd_activation_status() */

 /* EJECT */
/*===========================================================================
FUNCTION HDRSRCHREACQ_RESET_FPD_ACTIVATION_STATUS

DESCRIPTION
  This function resets the flag for forced premable detection
  activation status.
  
DEPENDENCIES
  None.

PARAMETERS
  None.
    
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrsrchreacq_reset_fpd_activation_status( void )
{
  hdrsrchreacq.forced_preamble_activated = FALSE;
    /* reset forced preamble active flag. */

} /* hdrsrchreacq_reset_fpd_activation_status() */


/*==========================================================================

FUNCTION HDRSRCHREACQ_SET_FAST_ACQ_REPORT

DESCRIPTION
  This function specifies fast acq callback function 

DEPENDENCIES
  None

PARAMETERS
  fast_acq_cb - Who to inform when we wakeup

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchreacq_set_fast_acq_report
(
  hdrsrch_fast_acq_cb_type                      fast_acq_cb
    /* What to do when we do fast acq */
)
{
  hdrsrchreacq.fast_acq_cb = fast_acq_cb;

}/* hdrsrchreacq_set_fast_acq_report */


/*==========================================================================

FUNCTION HDRSRCHREACQ_DEFINE_FPD_TIMER

DESCRIPTION
  This function defines FPD timer. 

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchreacq_define_fpd_timer( void )
{
  timer_def( &hdrsrchreacq.fpd_timer, NULL, NULL,
             0, hdrsrchreacq_fpd_timer_cb, 0 );

} /* hdrsrchreacq_define_fpd_timer */


/*==========================================================================

FUNCTION HDRSRCHREACQ_START_FPD_TIMER

DESCRIPTION
  This function starts timer for arriving at next SCC slot and programming
  it to FW for FPD, QPCH and DLNA.

DEPENDENCIES
  None

PARAMETERS
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/
void hdrsrchreacq_start_fpd_timer( void )
{
  timer_set( &hdrsrchreacq.fpd_timer, 
             HDRSRCHREACQ_FPD_TIMER_MS, 0, T_MSEC );

} /* hdrsrchreacq_start_fpd_timer */

