/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  H D R    S R C H    I D L E    S T A T E

                           Search Idle Module

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2000 - 2018  by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchidle.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
02/20/2018   vke     Change RF reason to OVHD for access->idle state change
01/24/2018   vke     Disallow ASdiv switch when connection close is inprogress
01/21/2016   wsh     Fixed idle chan change ptr not cleaned up after failure
12/16/2015   vko     Fixed KW issues
10/27/2015   wsh     Trigger sys loss if tune fails
06/26/2015   vke     Added timeout value for tune_to_chan()
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
04/30/2015   vke     Changes to disable TA if TCA is inprogress
02/04/2015   wsh     Added support 2->2a transition, removed PA on requirement
10/17/2014   wsh     Fixed reporting wrong mode to TRM in 4a->3a transistion
10/13/2013   arm     Updated hdrsrchutil_calc_time_to_tune_away to return if
                     TA time was updated or not
08/01/2014   sat     Changes to ignore Fast connect TCA processing while TA
08/03/2014   arm     Changes to request RX_ANY chain in DR mode for idle.
06/10/2014   dsp     Clear Access TA Timer & Signal in addition to disabling 
                     tuneaways in Access until AMAC ready.
05/12/2014   vke     Klockworks fix
05/02/2014   dsp     Disable tuneways in Access until AMAC ready.
05/05/2014   ljl     Added hdrsrchidle_channel_change_and_nset_update_cmd().
12/09/2013   smd     Requested the correct priority for access diversity.
08/19/2013   arm     Merged DSDS feature for triton. 
11/27/2013   dsp     FR17482 for Page blocking management.
10/29/2013   sat     Reduced F3 messages (or/and) ASSERTs
10/24/2013   vke     Remove disable of TA after entering Access state and 
                     handle only unlock immediate
10/24/2013   rmg     Disabled DLNA in idle state.
01/22/2013   wsh     Delay tuneaway when chan_change is in progress
09/03/2013   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
07/16/2013   wsh     Disable QPCH upon access 
06/26/2013   wsh     Added Selective TxD feature
06/21/2013   arm     Reverted disabling access handoff fix.
06/20/2013   arm     Fixed Bolt compiler errors.
05/30/2013   arm     Disabled access handoff after receiving ac ack. 
05/31/2013   vko     Fixed klocwork issues
05/24/2013   rmg     Added D-LNA support.
05/24/2013   smd     Reverted KW fixes.
04/22/2013   arm     Added changes for DSDA coexistence manager.
05/14/2013   ljl     Activate decoder after RF tune.
01/27/2012   smd     Fixed the bug of holding primary chain forever if there
                     is not enough time for sleep.
10/25/2012   rkc     Ignore TRM unlock cancel if tuneaway has started.
10/05/2012   smd     Disabled idle tune away before entering sleep. 
09/27/2012   ljl     Updated RGS before deassigning fingers.
09/14/2012   ljl     Increased tx_shutdown timeout to 100ms.
08/15/2012   smd     Supported idle tune away.
07/21/2012   arm     Disabled diversity when moving from monitor to TC.
07/15/2012   smd     Checked the rigth field for no tune away condition.
06/21/2012   mbs     Initialize monitor state when entering from reacq.
06/06/2012   grl     Make channel change inline with the idle transition
05/25/2012   wsh     Moved ARD logic into it's own state machine 
05/25/2012   wsh     Fixed FPD cancel related crashes
05/17/2011   smd     Fixed HDR not releasing div when tune away is disabled. 
                     Informed ACMAC ta time when tune away is disabled. 
04/02/2012   arm     Supported diversity in reacq. 
04/03/2012   wsh     Cancel FPD before tuning in IDLE
03/19/2012   ljl     Fixed Dime compiler errors.
03/19/2012   vlc     Fixed pilot purging to work with gRICE. 
02/27/2012   smd     Added updates of FTM msgr interface changes.
02/14/2012   arm     Allow diversity tuneaway during idle/access state. 
02/02/2012   arm     Changed idle div to record only max 255 psinr for any
                     idle wakeup. 
02/01/2012   ljl     Deactivate decoder before disabling or tuning RF.
01/05/2012   wsh     Fixed ASSERT in IDLE channel change cmd
12/09/2011   rmg     Added wait for tx stop confirmation before putting rx 
                     to sleep during tuneaway.
11/29/2011   arm     Idle diversity bug fixes.
11/29/2011   arm     Fixed idle diversity crash during channel change. 
11/11/2011   arm     HDR idle and access state diversity implemetation.
10/24/2011   wsh     Updated cancel FPD procedure 
10/17/2011   rmg     Removed inclusion of clkregim.h.
08/05/2011   ljl     Added change to deassign fingers before RF tune.
05/12/2011   ljl     Removed sleep clk estimate.
04/27/2011   rmg     Added HDR MCPM support changes.
10/19/2010   lyl     Added null pointer check after hdrsrchset_get_sect().
09/15/2010   vlc     Added support for DO->LTE pilot measurements/reselection.
03/24/2010   pxu     Removed FEATURE_HDR_QSM features  
03/18/2010   ljl     Supported FEATURE_HDR_DIV_SRCH_WORKAROUND.
12/16/2009   rkc     Moved tune away processing after srch is idle.
09/30/2009   smd     Fixed KW errors.
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
05/18/2009   ljl     Updated for firmware MSGR interface.
03/30/2009   rkc     Used renamed hdrsrchset_pilot_group_purge_mc() function
03/20/2009   rkc     Passed param rup_subtype to hdrsrchutil_update_nset_pilots
03/06/2009   pmk     Added support for QTF
02/10/2009   ljl     Reset OFS filter after channel hash.
02/04/2009   lyl     Fixed the lint errors.
01/29/2009   ljl     Removed fll acq after channel change.
12/10/2008   smd     Replaced assert.h with amssassert.h
11/18/2008   ljl     Killed tune away timer when processing deactivate cmd.
11/10/2008   ljl     Supported HDRSRCH_ACMAC_DISABLE_TA_CMD.
10/09/2008   lyl     Added QPCH support.
10/02/2008   smd     Fixed line indenting errors. 
09/25/2008   smd     Added a semicolon for ASSERT.
09/18/2008   rkc     Used Intra-SCC sleep params in call to
                     hdrsrchsleep_set_wakeup().
08/29/2008   lyl     Supported primary chain ASET search.
08/19/2008   smd     Added idle handoff improvement.
03/27/2008   ljl     Added the avoided channel list to nset update function.
02/21/2008   ljl     Preferred the same subnet handoff.
02/01/2008   jyw     Added the sanity check after TC to idle transition.
01/30/2008   jyw     Included a possible channel change in TC to idle trans.
01/29/2008   jyw     Included the demod assignment into the tuning interface.
12/26/2007   ljl     Added amac_ready flag.
11/20/2007   jyw     Removed the sleep.h from the header file list.
10/19/2007   jyw     Relocated the channel change ind before the 1st search.
07/19/2007   jyw     Added revB set management support.
08/19/2007   grl     Resolved Lint High warnings.
07/06/2007   ljl     Supported rotator push.
04/27/2007   grl     Added support for access hybrid ctrl via the tc mode.
04/10/2007   mt      Added support for RUP update request.
01/17/2007   ljl     Added HDR SRCH AFC stm
12/21/2006   jyw     Cleared tune away on search idle when disable tune away.
10/11/2006   grl     Added passing of the negotiated SCI value to hdrsrchsleep.
10/02/2006   ljl     Fixed hybrid access problem related to AHO.
09/19/2006   grl     Added support for DDARF.
09/13/2006   sq      Merged code review comments for QSM
08/28/2006   sq      Added FEATURE_HDR_QSM
08/01/2006   ljl     Added debug bit for access hybrid
07/19/2006   jyw     Forced the srch wincenter to mstr at the channel change.
06/28/2006   jyw     Added support to MPC.
06/16/2006   mt      Fixed BCMCS featurization
06/15/2006   grl     Fixed compilation warning.
06/07/2006   ljl     Added SBHO optimization
06/07/2006   ljl     Added support for hybrid access
05/18/2006   sq      Properly return the current serving sector
05/18/2006   grl     Don't change the TRM access reason from bc access.
04/05/2006   sq      Added code review comments
03/06/2006   sq      Added wake-up reason in wake-up callback
12/02/2005   cab     Updated sclk_estimate_complete to pass system id
09/07/2005   grl     Change sclk estimate to be a fair estimate.
09/06/2005   ljl     Chose IHO candidate based on cached info
09/06/2005   ds      Renamed maintain_fll_lock() to check_fll_lock_status()
09/01/2005   grl     Added support for lock releases during access attempts.
08/29/2005   sq      Do not set priority in hdrsrchidle_init_monitor_state
07/28/2005   ds      Added hash channel list to nset update, SPMF support
07/25/2005   ljl     Added broadcast and directed nset update 
07/12/2005   sq      Added support for enhanced idle state protocol
06/28/2005   jyw     Added probes for the HDR sleep time line profiling
05/31/2005   grl     Added support for TRM and TCXOMgr. 
03/31/2005   grl     Changed HDR's use of trk_lo to be GPS friendly 
01/13/2005   sq      Mainlined FEATURE_HDR_NO_HO_DURING_SCC
12/02/2004   sq      Removed access_end cmd / code featurization & clean-up
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
10/06/2004   aaj     Fixed problem with BC transition during idle access
09/14/2004   sq      Added support of access handoff in BC access state
09/10/2004   sq      Removed function hdrsrchidle_handle_idle_ho
09/07/2004   sq      Undefined FEATURE_HDR_SLEEP_NO_ABORT
08/12/2004   ajn     sleephdr -> sleepctl
08/02/2004   sq      Print hstr value as mod 512 in addition to the raw value
07/30/2004   sq      Included more common idle state functions
07/15/2004   sq      Added support to FEATURE_HDR_BCMCS
06/14/2004   aaj     Postpone entering sleep if dump is pending
06/07/2004   aaj     Splitted idle monitor state from common idle operation
04/08/2004   aaj     Let protocols know about suspend_none in idle_init()
03/04/2004   aaj     Support for minimizing subnet handoffs 
02/18/2004   aaj/sq  Send extra handoff indication after entering IDLE state
01/06/2004   ajn     Corrected exit from HO disabled state
12/23/2003   sq      Set last_hstr to 0 when HO is enabled
12/02/2003   sq      Merged in code review comment
12/01/2003   sq      Add TIMEOUT_SIG to wait mask while entering IDLE state
11/18/2003   sq      Removed off-freq CSET pilot demotion in hdrsrchidle_done
11/11/2003   sq      Supported 3 new log packets( acq,reacq,sleep )
10/20/2003   aaj     Fixed HSTR modulo operation used in handoff logic
10/14/2003   aaj     Support to validate search results to avoid bogus PNs
10/06/2003   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
09/10/2003   mpa     Converted F3 message to use MSG2.0
07/30/2003   sq      Added support for FEATURE_PLT
06/10/2003   ajn     Corrected inverted logic decision.
05/30/2003   ajn     Ensure Disabled HO logic works for Reacq->Idle @ hstr=511
04/29/2003   ajn     Corrected sclk_estimate_complete test for E_FAILURE
04/29/2003   ajn     Prevent HO disable if Reenable cmd rcv'd before disable
04/04/2003   aaj     Added support to maintain FLL lock during steady state
04/04/2003   aaj     Re-enable missed preamble detection logic after 3.2
03/19/2003   aaj     Disable missed preamble detection logic
03/11/2003   ajn     alter_sci( 0, SET) changed to reset_sci( )
03/11/2003   ajn     Updated Copyright notice
03/10/2003   aaj     Stop pilot supervision timer before entering sleep.
02/28/2003   mpa     Added new F3 messages for no HO during SCC.
02/27/2003   aaj     Moved restoring finger threshold up in idle_init
02/24/2003   kss     Enable missed SyncCC detection when in idle.
02/19/2003   sq      Restore finger thresholds according to MPS flag
02/14/2003   aaj     Enable Freq Locked Loop after channel change is done
02/07/2003   ajn     Protect against IHO just before SCC starts as well.
02/03/2003   ajn     Corrected FEATURE_HDR_NO_HO_DURING_SCC
01/10/2002   ajn     Added FEATURE_HDR_NO_HO_DURING_SCC
11/25/2002   ajn     Store suspend callback.
11/07/2002   ajn     enable_roll( ) utility replaces register roll callback
11/04/2002   aaj     Remove FEATURE_HDRSRCH_PROFILE. F3 change
10/30/2002   aaj     Abort pending searches after sleep command is received.
09/18/2002   aaj     Support for multipath window centering for HDR
09/10/2002   aaj     Demote off-freq cset pilots if next state is connected
09/09/2002   aaj     Include NSET in pilot supervision. Implemented DDR#3,856-2
                     Lowered "access_ho while not in access" F3 from ERR to HIGH
                     Handle RTC_ACK_RECEIVED cmd in idle state. Do nothing.
08/22/2002   aaj     Start Freq pullin after channel change in idle
08/13/2002   ajn     Featurized system-time check
08/13/2002   aaj     Added support for E_WOULD_BLOCK to access_ho_cmd
08/09/2002   ajn     Added system time check
06/21/2002   ajn     Ensure leaving IDLE leaves IDLE/Access substate
06/14/2002   ajn     Access H/O Threshold added to ACCESS_MODE_CMD
05/15/2002   aaj     Set decoder stale timer to 0 for idle state
05/13/2002   aaj     Start schedular for 4->3 transition
05/07/2002   ajn     Return E_FAILURE from ACCESS_HO_CMD if not in ACCESS.
04/03/2002   ajn     Return E_IN_PROGRESS for SLEEP & ACCESS commands, if reqd
03/13/2002   ajn     Reduced frequency of Access HO Request to upper layer
03/12/2002   sq      Disabled triage while in FTM mode
02/20/2002   ajn     Corrected SLEEP_CMD's schedule/triage suppression logic
02/16/2002   aaj     Support for multiple CCC sleep. Introduced HDR SCI
02/14/2002   ajn     Fixed drop threshold comparison to avoid NULL pointer.
02/06/2002   ajn     hdrerrno -> errno
02/05/2002   aaj     Fixed drop threshold comparison in pilot supervision proc
01/31/2002   aaj     F3 messages for total finger rssi and sleep duration
01/30/2002   ajn     Force unique PNs on channel change.
                     Start FEE estimate if no FEE is in progress.
01/28/2002   ajn     GP0 signal removed from FEE calculations.
01/21/2002   aaj     Start schedular after transition from reacq to init
01/17/2002   aaj     Changed sector struct to contain band+chan number
01/16/2002   aaj     Added asserts to verify that HDR has lock
12/31/2001   ajn     Added periodic (1s) Rx power measurement F3 message.
12/07/2001   ajn     Added Pilot to Channel Change command.
12/07/2001   aaj     Idle handoff threshold support for OFS/SLEEP
12/05/2001   aaj     Fixed channel change bug. Set new channel to aset pilot
12/05/2001   aaj     Idle state now runs handoffs in override mode
11/26/2001   aaj     Reworked idle/ofs/sleep for including RF shutdown support
11/09/2001   aaj     Timetest support for srch profiling
11/01/2001   aaj     Changes to rf tune interface.deassign fings in start slp
10/29/2001   ajn     Do not perform triage if AT is entering sleep state
10/24/2001   ajn     Change state header files to common hdrsrchstates.h
10/22/2001   ajn     Sleep code moved to hdrsrchsleep.c
10/19/2001   ajn     Minor cleanup.  Backed out 1.39 dog changes.
10/15/2001   aaj     Added diag messages for pilot supervision timer
10/11/2001   ajn     Added keypress handler, to allow response during sleep.
10/08/2001   ajn     Reworked command retry processing & system lost
09/27/2001   ajn     Reworked ReAcq Dump.
09/25/2001   aaj     Enabled MSTR Slam operation in wakeups
09/25/2001   ajn     Added fast wakeup.
09/04/2001   ajn     sleep() interface changed.  Enabled transitions from
                     sleep to idle, access, and traffic.
08/09/2001   ajn     SLEEP VU Integration.
08/09/2001   aaj     Fixed ping-pong handoffs in idle monitor state
06/19/2001   aaj     Set PN roll ISR in idle_init, code cleanup.
06/07/2001   ajn     Completed sleep signals (support added in sleep.h)
06/06/2001   ajn     OnSearch/OnDump now module global functions.
                     Added InitMonitorState function.
                     Roughed in sleep signals
04/11/2001   ajn     Sleep and Reacquisition after sleep.
03/08/2001   ajn     Tune now uses on_searcher_idle( )
03/02/2001   aaj     included customer.h file
02/06/2001   aaj     Changed external function names to start with filename
11/22/2000   ajn     Code review changes / param restructuring / HASH->CYCLE
11/08/2000   ajn     Added temporary LinkID support.
10/31/2000   ajn     Added General Purpose Signals to State Machine.
                     Added SClk frequency determination code.
10/23/2000   ajn     Implemented access handoff command.
10/11/2000   ajn     Idle h/o uses aset_idle_handoff func
07/07/2000   ajn     Created this file

==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"

#include "hdrsrchidlei.h"

#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchstate.h"
#include "hdrsrchset.h"
#include "hdrsrchfing.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfdiv.h"
#include "hdrsrchlog.h"
#include "hdrsrchafc.h"
#include "hdrsrch.h"

#include "hdrts.h"
#include "hdrrx.h"
#include "hdrdec.h"

#ifndef FEATURE_HDR_DIME_MODEM
#include "sclk.h"
#endif /* FEATURE_HDR_DIME_MODEM */

#include "hdrmdspmcr.h"

#include "hdrdebug.h"
#include "err.h"
#include "amssassert.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "ftm.h"
#endif


/* This feature was originally defined, because abort mechanism was not used 
   to stop searches before going to sleep. The code instead waits for the dump 
   to be complete and then starts the process of entering to sleep. After 
   HDRSRCH abort mechanism is implemented later, this feature needs to be 
   undefined  */


#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV
#include "hdrsrchutil.h"
#endif

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */
/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/* Converts a number into a natural number ( greater or equal to zero ). */
#define HDRSRCHIDLE_NAT_NUM( num ) ( num < 0 ? 0 : num )

/* Time limit to wait for tx powerdown confirmation from access MAC */
#define HDRSRCHIDLE_TX_POWERDOWN_WAIT_MS    100
  
#define HDRSRCHIDLE_BAND_GRANT_TIMEOUT_MS   (50)
  /* timeout value for band grant */

LOCAL void hdrsrchidle_channel_change_complete( void );

/* EJECT */
/*==========================================================================

                     IDLE STATE COMMON DATA DECLARATION

==========================================================================*/


hdrsrchidle_common_struct_type        hdrsrchidle;
  /* Common parameters for different states for HDR idle operation */
  
hdrsrch_channel_change_params_type    hdrsrchidle_idle_link;
  /* Link information for inline idle channel changes. */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_ENABLE_TUNE_AWAYS

DESCRIPTION
  Turns on the ability for tune aways to happen within the idle state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_enable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Tuneaways enabled");

  hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are now interested in primary chain events. */

  hdrsrchrf_retain_lock( HDRSRCHRF_PRI_CHAIN, hdrsrch_rf_unlock_cb );
    /* Register an unlock callback so that we may be interrupted. */

} /* hdrsrchidle_enable_tune_aways */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_ENABLE_TUNE_AWAY

DESCRIPTION
  Retain rf lock and initialize tuneaway data structure

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_monitor_enable_tune_away( void )
{
  /* Initialize monitor_ta */
  hdrsrchidle.monitor_ta.rl_state.event = HDRSRCHRF_UNLOCK_CANCELLED;
  hdrsrchidle.monitor_ta.no_tune_away = TRUE;
  hdrsrchidle.monitor_ta.end_time_sclks = HDRSRCHRF_MAX_DURATION;
  hdrsrchidle.monitor_ta.wait_for_channel_change = FALSE;

  hdrsrchidle.monitor_ta.start_time_sclks = timetick_get();
    /* Save the current time in sclks. */

  hdrsrchidle_enable_tune_aways( );
    /* retain rf lock */

} /* hdrsrchidle_monitor_enable_tune_away */

/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_DISABLE_TUNE_AWAY

DESCRIPTION
  Unregister unlock callback and kill ta timer

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_monitor_disable_tune_away( void )
{
   hdrsrch_kill_timer( HDRSRCHIDLE_MONITOR_TA_TIMER );
    /* Kill monitor tune away timer. */

   hdrsrchrf_unregister_unlock_cb( HDRSRCHRF_PRI_CHAIN );
     /* Unregister for unlock callbacks.  We no longer care. */

   hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
     /* We are no longer interested in primary chain events. */

   hdrsrch_clear_tune_away_on_searcher_idle( );
     /* Clear tune away on search idle if none NULL. We don't want to perform
        pending tune way if there is any.
     */

} /* hdrsrchidle_monitor_disable_tune_away */
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_DISABLE_TUNE_AWAYS

DESCRIPTION
  Turns off the ability for tune aways to happen within the idle state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_disable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Tuneaways disabled");

  hdrsrchrf_unregister_unlock_cb( HDRSRCHRF_PRI_CHAIN );
    /* Unregister for unlock callbacks.  We no longer care. */

  hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are no longer interested in primary chain events. */

  hdrsrch_clear_tune_away_on_searcher_idle( );
    /* Clear tune away on search idle if none NULL */

} /* hdrsrchidle_disable_tune_aways */


/* <EJECT> */
#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================

FUNCTION HDRSRCHIDLE_TA_TIME_TO_SLOTS

DESCRIPTION
  This function converts sclk to slots.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Time in slots.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_ta_time_to_slots
(
  qword                      ta_time_slot,
    /* Output: Tune away time in slots */

  timetick_type              ta_time_sclk
    /* Input: Tune away time in sclks */
)
{

  timetick_type              sclk_now;
    /* Current sclk count */

  uint32                     sclk_left;
    /* The number of sclks left */

  uint32                     ms_left;
    /* The number of ms left */

  uint32                     slot_left;
    /* The number of slots left */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "hybrid_access : ta_time_slck 0x%x",
                  ta_time_sclk );
  
  (void) hdrts_get_current_time( ta_time_slot, &sclk_now );
    /* Get current time in slots and sclks */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                  "hybrid_access : now sclk 0x%x slots: 0x%x:%x",
                  sclk_now,
                  ta_time_slot[1],
                  ta_time_slot[0] );

  /* Get left time in slot */
  sclk_left = 
              HDRSRCHIDLE_NAT_NUM( (int32)( ta_time_sclk - sclk_now ) );
    /* Calculate the number of sclks until the next tune away prior to the
       new calculation. Don't permit negative numbers. */
  
  ms_left = timetick_cvt_from_sclk( sclk_left, T_MSEC );
    /* Convert to ms */

  slot_left = ms_left * 3 / 5;
    /* Convert to 1.67ms slots */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                  "hybrid_access : ms_left 0x%x, slot_left 0x%x",
                  ms_left,
                  slot_left);

  /* Get the tune away time in slots */
  qw_inc( ta_time_slot, slot_left );
  
}  /* hdrsrchidle_ta_time_to_slots() */



/*===========================================================================

FUNCTION HDRSRCHIDLE_TX_POWERDOWN_CB

DESCRIPTION
  This is the callback function to notify HDRSRCH task by HDRTX task that
  that tx is powered down by access channel MAC.  

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_tx_powerdown_cb( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "hdrsrchidle_tx_powerdown_cb is called");

  HDRSRCH_SIG_SET( HDRSRCH_TX_POWERDOWN_SIG );
    /* Set the signal */
} /* hdrsrchidle_tx_powerdown_cb */


/*===========================================================================

FUNCTION HDRSRCHIDLE_TUNE_AWAY_PROC

DESCRIPTION
  Tune away.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  NOne

===========================================================================*/

LOCAL void hdrsrchidle_tune_away_proc( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_STARTED );
    /* Let the protocol know we are tuning away. */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
  if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE  )
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
  {

     /* Stop access. This is only for access tune away */
  if ( hdrsrchidle.access.stop_cb != NULL )
  {
    hdrsrchidle.access.stop_cb( hdrsrchidle_tx_powerdown_cb );

    hdrsrch_timed_wait_for_sig( HDRSRCH_TX_POWERDOWN_SIG,
                                HDRSRCHIDLE_TX_POWERDOWN_WAIT_MS );
    /* Wait for tx powerdown signal in the time limit */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "hybrid_access: no cb to stp access" );
  }
  }

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV 

  if( hdrsrch.idle_state_div.idle_div_enabled ||
      hdrsrch.idle_state_div.access_div_enabled )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Idle Div: tune away disable div" );

    hdrsrchrfdiv_diversity_ctrl( FALSE );
      /* Disable Diversity */
    
    if( hdrsrch.idle_state_div.idle_div_enabled )
    {
      hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg = 
        hdrsrchidle_get_prev_wakeup_psinr();
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Idle Div: PSINR avg %d",
                      hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg );
        /* Save previous wakeup info */

      /* Deactivate decode m slot indication */
      hdrsrchsleep_deactivate_scc_decode_ind();
  
      hdrlog_register_slot_logging_cb ( HDRLOG_SRCHDIV_MDSP_SLOT_LOGGING_CLIENT,
                                        NULL );
    }   
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */


  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* Stop scheduler ... we are tuning away */

  hdrsrchutil_start_stop_watch();
    /* Start measuring for how long AT stays away from HDR Access */

  hdrsrchafc_rotator_push();
    /* Push rotator samples */

  hdrsrchafc_update_rgs();
    /* Report RGS */

  hdrsrchutil_cancel_fpd( );

  hdrsrchfing_deassign_all_fings( );
    /* Unassign fingers */

  hdrsrchutil_dec_deactivate( );
    /* Deactivate HDR decoder before going to 1x */

  hdrsrchrf_pdm_capture_values();
    /* Capture PDM values for fast RF warmup */

  hdrsrchafc_stop_fll_tracking();
    /* Disable FLL tracking and save the FLL accum value. */

  hdrsrchrf_prep_to_sleep();
    /* Prepare the RF for sleep */

  hdrsrchdrv_shutdown_proc();
    /* Stop RX AGC */

}  /* hdrsrchidle_tune_away_proc() */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_ON_SEARCH_IDLE_CB

DESCRIPTION
  This callback function is invoked when searcher is idle during tune away
  processing.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_on_search_idle_cb( void )
{

  hdrsrchidle_tune_away_proc();
    /* perform tune away processing */

  hdrsrchsus_init_suspended_state();
    /* Begin suspended IDLE state */

} /* hdrsrchidle_on_search_idle_cb */


/*===========================================================================

FUNCTION HDRSRCHIDLE_PROCESS_ACCESS_TUNE_AWAY

DESCRIPTION
  This function performs proper tune away procedure based on the time that
  is currently left on the lock and the current tune away state. It then
  schedules the next event to be processed.

DEPENDENCIES
  None

PARAMETERS
  extend_ms - maximum duration for which we can have the RF.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchidle_process_access_tune_away
( 
  unsigned                   extend_ms
    /* maximum duration for which we can have the RF */
)
{
  unsigned                   next_event_ms = 0;
    /* when do we want to check it */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,  
                  "hybrid_access : hdrsrchidle_process_access_tune_away() %dms",
                  extend_ms );

  if ( hdrsrchidle.channel_change != NULL )
  {
    hdrsrch_kill_timer( HDRSRCHIDLE_ACCESS_TA_TIMER );
      /* Kill access tune away timer **/

    hdrsrchidle.access.waiting_chan_change = TRUE;
      /* If chan change is pending, delay tuneaway after chan change */

    HDR_MSG_SRCH( MSG_LEGACY_MED, "tuneaway delayed after chan change" );
  }
  else if ( extend_ms <= HDRSRCHIDLE_OVERHEAD_MS )
  {
    /* Less than or equal to 15ms from tune away */

    hdrsrchdrv_abort_search( FALSE );
      /* Abort pending searches, if any   */

    hdrsrchidle.access.tune_away.state = HDRSRCHIDLE_TUNE_AWAY_GONE;
      /* Plenty of time before tune-away */

    hdrsrch_kill_timer( HDRSRCHIDLE_ACCESS_TA_TIMER );
      /* Kill access tune away timer **/

#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

    hdrsrch_tune_away_on_searcher_idle( hdrsrchidle_on_search_idle_cb );
      /* change to suspended idle state, only when searcher is idle  */
  }
  else
  {

    hdrsrchidle.access.tune_away.state = HDRSRCHIDLE_TUNE_AWAY_NO_CONFLICT;
      /* Plenty of time before tune-away */
      
    next_event_ms             = extend_ms - HDRSRCHIDLE_OVERHEAD_MS;;
    /* next event to be HDRSRCHBC_RELEASE_LOCK_MS before tune-away */

    /* Program next event if necessary */
    (void) rex_set_timer( HDRSRCHIDLE_ACCESS_TA_TIMER, next_event_ms );
      /* set timer for next event */
    
    hdrsrch_add_to_wait_mask( HDRSRCHIDLE_ACCESS_TA_SIG );
      /* Watch for access TA signal */

  }

}  /* hdrsrchidle_process_access_tune_away */


/*===========================================================================

FUNCTION HDRSRCHIDLE_TUNE_BACK

DESCRIPTION
  Tune back to Idle after visit to 1x.

DEPENDENCIES

PARAMETERS

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchidle_tune_back
( 
  hdrsrchrf_tune_status_enum_type status
)
{
  dword                      tune_away_ms;
    /* time in msec for which AT had tuned away */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( status != HDRSRCHRF_TUNE_SUCCESS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
    return;
  }
  
  hdrdec_activate();
    /* Activate HDR decoder before going to 1x */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Activate hdr dec" );
  
#ifdef FEATURE_HDR_ACCESS_DIV
  if ( (hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE) &&
        hdrsrch.idle_state_div.access_div_enabled ) 
  {
    /* If tuning back to access turn on diversity */
   
     if( !hdrsrchrfdiv_diversity_is_enabled() )
     {
       HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Turning on div in access mode");

       hdrsrchrfdiv_diversity_ctrl( TRUE );
        /* Enable diversity in access state. */
     }
     else
     {
       HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Not turning on div in access"
                                     " mode as it is already on");
     }
  }
#endif /* FEATURE_HDR_ACCESS_DIV */

  hdrsrchidle_sched();
    /* start search schedular and send first set of searches */

  tune_away_ms = hdrsrchutil_read_stop_watch();
    /* read the time between start of tune away and present time */

  hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_ENDED );
    /* We are starting Ramp */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "hybrid_access : Resume HDR Idle Access after %dms", 
                  tune_away_ms );
  
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY  
if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE )
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
  {
  hdrsrchidle_enable_tune_aways();
    /* Enable tune aways. */

  hdrsrchidle.access.tune_away.state = HDRSRCHIDLE_TUNE_AWAY_NO_CONFLICT;
    /* initialize tune away state to no conflict */
  }
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
  else
  {
    hdrsrchidle_monitor_enable_tune_away();
      /* Enable tune aways for monitor substate. */
  }
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
} /* hdrsrchidle_tune_back */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_RESUME_IDLE_INIT

DESCRIPTION
  Idle State Resumption after tune-away

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  NOne

===========================================================================*/

void hdrsrchidle_resume_idle_init ( void )
{
  boolean tune_status;
    /* Status of tune to chan */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Enable RF and Tune to the channel */
#ifdef FEATURE_HDR_REVB

  HDR_ASSERT ( hdrsrch.aset_channel_cnt <= HDRSRCH_MAX_NUM_CARRIERS );
    /* KW fix : Check to limit the hdrsrch.aset_channel_cnt to max */

  tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                          hdrsrch.aset_channel,
                          hdrsrch.aset_demod,
                          hdrsrch.aset_channel_cnt,
                          hdrsrchidle_tune_back,
                          HDRSRCHIDLE_BAND_GRANT_TIMEOUT_MS );
#else
  tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                          &hdrsrch.aset_channel,
                          hdrsrchidle_tune_back,
                          HDRSRCHIDLE_BAND_GRANT_TIMEOUT_MS );
#endif /* FEATURE_HDR_REVB */

  if ( tune_status == FALSE )
  {
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }

  hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_ENDING );
    /* We are starting Ramp */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
  if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE )
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
  {
  hdrsrchidle.access.tune_away.state = HDRSRCHIDLE_TUNE_AWAY_RETURNING;
    /* note that we are returning from suspended idle */
  }

  hdrsrchidle.is_tx_pending = FALSE;
}/* hdrsrchidle_resume_idle_init */



/*===========================================================================

FUNCTION HDRSRCHIDLE_NOTIFY_ACMAC_TA_TIME

DESCRIPTION
  This function notifies ACMAC the tune away time.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_notify_acmac_ta_time( void )
{
  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

  qword                          ta_time;
    /* Tune away time in slots */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
    /* Retrieve the retain lock state. */

  /* Stop the previous tune away */
  hdrsrch_kill_timer( HDRSRCHIDLE_ACCESS_TA_TIMER );
    /* Kill access tune away timer **/

  if ( ( rl_state.event == HDRSRCHRF_UNLOCK_CANCELLED ) ||
       ( rl_state.event == HDRSRCHRF_UNLOCK_REQUIRED ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "hybrid_access : Process unlock event: %d. No tune away",
                    rl_state.event );

    hdrsrchidle.access.tune_away.rl_state.event = HDRSRCHRF_UNLOCK_CANCELLED; 
      /* Convert to unlock cancelled */

    hdrsrchidle.access.tune_away.no_tune_away = TRUE;
      /* No tune away */

    qw_set(ta_time, 0xFFFFFFFF, 0xFFFFFFFF );
      /* Set tune away time to max */

    if ( hdrsrchidle.access.ta_time_cb != NULL )
    {
      hdrsrchidle.access.ta_time_cb( TRUE, ta_time );
        /* Notify ACMAC */
    }

  }  /* HDRSRCHRF_UNLOCK_CANCELLED */
  else if ( rl_state.event == HDRSRCHRF_UNLOCK_BY )
  {

    hdrsrchidle.access.tune_away.rl_state.event = rl_state.event;
    hdrsrchidle.access.tune_away.rl_state.unlock_by_sclk = 
                                                     rl_state.unlock_by_sclk;
      /* Save the retain lock state */

    hdrsrchidle.access.tune_away.no_tune_away = FALSE;
    hdrsrchidle.access.tune_away.start_time_sclks = 
                                                  rl_state.unlock_by_sclk;
      /* Store tune away time */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                  "hybrid_access : Process unlock_by event");

    hdrsrchidle_ta_time_to_slots( ta_time, rl_state.unlock_by_sclk );
      /* Convert tune away time to slots */

    if ( hdrsrchidle.access.ta_time_cb != NULL )
    {
      hdrsrchidle.access.ta_time_cb( FALSE, ta_time );
        /* Notify ACMAC */
    }

  }  /* HDRSRCHRF_UNLOCK_BY */

}  /* hdrsrchidle_notify_acmac_ta_time */
#endif /* FEATURE_HDR_HYBRID_ACCESS */


#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
/*===========================================================================

FUNCTION HDRSRCHIDLE_PROCESS_MONITOR_TUNE_AWAY

DESCRIPTION
  This function performs proper tune away procedure based on the time that
  is currently left on the lock and the current tune away state. It then
  schedules the next event to be processed.

DEPENDENCIES
  None

PARAMETERS
  extend_ms - maximum duration for which we can have the RF.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchidle_process_monitor_tune_away
( 
  unsigned                   extend_ms
    /* maximum duration for which we can have the RF */
)
{
  unsigned                   next_event_ms = 0;
    /* when do we want to check it */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,  
                  "Idle unlock : hdrsrchidle_process_monitor_tune_away() %dms",
                  extend_ms );

  if ( extend_ms <= HDRSRCHIDLE_OVERHEAD_MS )
  {
    /* Less than or equal to 15ms from tune away */

    hdrsrchdrv_abort_search( FALSE );
      /* Abort pending searches, if any   */

    hdrsrchidle_monitor_disable_tune_away();
      /* We will start tune away. We don't want to process unlock notify in the
         middle of tune away
      */

    if ( hdrsrchidle.channel_change == NULL )
    {
#ifdef FEATURE_HDR_SELECTIVE_TXD
      hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      /* No channel change is going on. We can start tune away when search is idle */
      hdrsrch_tune_away_on_searcher_idle( hdrsrchidle_on_search_idle_cb );
        /* change to suspended idle state, only when searcher is idle  */
    }
    else
    {
      /* We are in the middle of channel change. Better to start tune away after
         channel change is done
      */
      hdrsrchidle.monitor_ta.wait_for_channel_change = TRUE;
    }
  }
  else
  {
    next_event_ms             = extend_ms - HDRSRCHIDLE_OVERHEAD_MS;;
    /* next event to be HDRSRCHBC_RELEASE_LOCK_MS before tune-away */

    /* Program next event if necessary */
    (void) rex_set_timer( HDRSRCHIDLE_MONITOR_TA_TIMER, next_event_ms );
      /* set timer for next event */
    
    hdrsrch_add_to_wait_mask( HDRSRCHIDLE_MONITOR_TA_SIG );
      /* Watch for monitor TA signal */
  }

}  /* hdrsrchidle_process_monitor_tune_away */


/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_RF_UNLOCK

DESCRIPTION
  This function processes the unlock RF event received in idle monitor state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_monitor_rf_unlock( void )
{
  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

  timetick_type                  sclks_left;
    /* Sclks remaining until lock release */

  timetick_type                  sclks_now;
    /* current sclks  count */

  unsigned                       ms_left;
    /* maximum duration for which we can have the RF */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
    /* Retrieve the retain lock state. */


  if ( ( rl_state.event == HDRSRCHRF_UNLOCK_CANCELLED ) ||
       ( rl_state.event == HDRSRCHRF_UNLOCK_REQUIRED ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "Idle unlock: Process unlock event: %d. No tune away",
                    rl_state.event );

    hdrsrchidle.monitor_ta.rl_state.event = HDRSRCHRF_UNLOCK_CANCELLED; 
      /* Convert to unlock cancelled */

    hdrsrchidle.monitor_ta.no_tune_away = TRUE;
      /* No tune away */
  }  /* HDRSRCHRF_UNLOCK_CANCELLED */
  else if ( rl_state.event == HDRSRCHRF_UNLOCK_BY )
  {

    hdrsrchidle.monitor_ta.rl_state.event = rl_state.event;
    hdrsrchidle.monitor_ta.rl_state.unlock_by_sclk = rl_state.unlock_by_sclk;
      /* Save the retain lock state */

    hdrsrchidle.monitor_ta.no_tune_away = FALSE;

    sclks_now = timetick_get();
      /* Current time in sclks. */ 
    hdrsrchidle.monitor_ta.start_time_sclks = sclks_now;
      /* Keep track of the current time. */

    sclks_left = (timetick_type) hdrsrchutil_calc_time_to_tune_away( 
                                          (uint32) sclks_now,
                                          (int32) HDRSRCHIDLE_OVERHEAD_SCLK,
                                          &hdrsrchidle.monitor_ta.rl_state,
                                          &hdrsrchidle.monitor_ta.no_tune_away,
                                          &hdrsrchidle.monitor_ta.end_time_sclks,
                                          NULL);
      /* How much more time should we can have RF chain? */


    ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
      /* Convert lock extension to milliseconds. */

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
          "Idle unlock : Process unlock_by %d sclk, ms_left %d, no_tune_away %d",
          rl_state.unlock_by_sclk, ms_left, hdrsrchidle.monitor_ta.no_tune_away );

    hdrsrchidle_process_monitor_tune_away( ms_left );
  }

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV 
  if( hdrsrch.idle_state_div.idle_div_enabled ||
      hdrsrch.idle_state_div.access_div_enabled )
  {
    hdrsrchrfdiv_resume_tuneaway();
      /* Send the command to the RF Diversity State Machine.
        This will kick start div tuneaway timer, if necessary. */
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV || FEATURE_HDR_ACCESS_DIV  */

} /* hdrsrchidle_monitor_rf_unlock */
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

/*===========================================================================

FUNCTION HDRSRCHIDLE_RF_UNLOCK_EVENT

DESCRIPTION
  This function processes the unlock RF event.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_rf_unlock_event( void )
{
  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
    /* Retrieve the retain lock state. */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                  "rf_unlock_event() event: %d", 
                  rl_state.event  );

  if ( rl_state.event == HDRSRCHRF_UNLOCK_IMMEDIATELY )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, 
                  "Primary chain unlock immediately requested" );
      /* Print out debug message. */

    hdrsrchidle_disable_tune_aways();
      /* We no longer care about tune aways since we are forcing
         an unlock. */

    hdrsrch_advise_unlock_rf();
      /* Tell search to unlock the RF as soon as possible. */
  }

#ifdef FEATURE_HDR_HYBRID_ACCESS

  else
  if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE )
  {
    if ( hdrsrchidle.access.tune_away.amac_ready == FALSE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, 
                    "Pending on ACMAC_READY CMD hence unlock ignored" );
    }
    else
    {
      /* A new RF unlock event has occured.
       If tuneaway has already started or tune away is disabled, don't notify ACMAC. 
       The new tune away time is saved in hdrsrchrf structure. */
      if ( ( hdrsrchidle.access.tune_away.state == HDRSRCHIDLE_TUNE_AWAY_GONE ) &&
         ( rl_state.event == HDRSRCHRF_UNLOCK_CANCELLED ) )
      {
        HDR_MSG_SRCH( MSG_LEGACY_MED, 
                      "UNLOCK_CANCEL ignored since IDLE tuneaway has already started" );
          /* This should be a rare occurance so if this F3 is seen, we know this was
             an issue if UNLOCK_CANCEL was not ignored. */
      }
      else if ( hdrsrchidle.access.tune_away.state != HDRSRCHIDLE_TUNE_AWAY_DISABLED )
      {
        hdrsrchidle_notify_acmac_ta_time();
      }
    }/* if ( hdrsrchidle.access.tune_away.amac_ready == FALSE ) */
  }  /* ACCESS_STATE */

#endif /* FEATURE_HDR_HYBRID_ACCESS */
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
  else /* IDLE MON state */
  {
    hdrsrchidle_monitor_rf_unlock();
  }
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
} /* hdrsrchidle_rf_unlock_event */


#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================

FUNCTION HDRSRCHIDLE_ACCESS_TA_TIMER_EVENT

DESCRIPTION
  This function processes the access tune away timer event.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_access_ta_timer_event( void )
{

  uint32                         sclks_now;
    /* Current slck count */

  uint32                         sclks_left;
    /* Sclks remaining until lock release */
  
  unsigned int                   ms_left;
    /* maximum duration for which we can have the RF */

  hdrsrchidle_access_tune_away_type *ta;
    /* Pointer to the tune away structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ta = &hdrsrchidle.access.tune_away;

  sclks_now = timetick_get();
    /* Current time in sclks. */ 

  if ( ta->state == HDRSRCHIDLE_TUNE_AWAY_DISABLED )
  {
    if ( ta->disable_ta.end_time_sclks > sclks_now )
    {
      /* Timer expired before end time */
      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                      "disabled ta, timer expired %d the expected time %d sclks",
                      sclks_now, ta->disable_ta.end_time_sclks);

      /* Restart the timer */
      (void) rex_set_timer( HDRSRCHIDLE_ACCESS_TA_TIMER, 
              timetick_cvt_from_sclk( ta->disable_ta.end_time_sclks - sclks_now,
                                      T_MSEC ) );

      hdrsrch_add_to_wait_mask( HDRSRCHIDLE_ACCESS_TA_SIG );
        /* Watch for access TA signal */

    }
    else
    {
      ta->state = HDRSRCHIDLE_TUNE_AWAY_NO_CONFLICT;
        /* Change the tune away state */

      /* Disable tune away expired, notify ACMAC the tune away time */
      hdrsrchidle_notify_acmac_ta_time();
    }
  }
  else
  {
    /* Tune away is not at disabled state */

    sclks_left = hdrsrchutil_calc_time_to_tune_away( 
            sclks_now,
            (int32) HDRSRCHIDLE_OVERHEAD_SCLK,
            &hdrsrchidle.access.tune_away.rl_state,
            &hdrsrchidle.access.tune_away.no_tune_away,
            &hdrsrchidle.access.tune_away.end_time_sclks,
            NULL );
      /* How much more time should we stay in idle access? */

    hdrsrchidle.access.tune_away.start_time_sclks = sclks_now;
      /* Keep track of the current time. */

    ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
      /* Convert lock extension to milliseconds. */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                    "hybrid_access : Prcoess timer event ms_left %dms",
                    ms_left);

    hdrsrchidle_process_access_tune_away( ms_left );
      /* To process tune away */

  }
}  /* hdrsrchidle_access_ta_timer_event */
#endif /* FEATURE_HDR_HYBRID_ACCESS */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_TA_TIMER_EVENT

DESCRIPTION
  This function processes the monitor tune away timer event.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_monitor_ta_timer_event( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchdrv_abort_search( FALSE );
    /* Abort pending searches, if any   */

  hdrsrchidle_monitor_disable_tune_away();
    /* We will start tune away. We don't want to process unlock notify in the
       middle of tune away
    */

  if ( hdrsrchidle.channel_change == NULL )
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

    /* No channel change is going on. We can start tune away when search is idle */
    hdrsrch_tune_away_on_searcher_idle( hdrsrchidle_on_search_idle_cb );
      /* change to suspended idle state, only when searcher is idle  */
  }
  else
  {
    /* We are in the middle of channel change. Better to start tune away after
       channel change is done
    */
    hdrsrchidle.monitor_ta.wait_for_channel_change = TRUE;
  }

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                  "Idle unlock: Process timer event, start tune away" );

}  /* hdrsrchidle_monitor_ta_timer_event */

/*===========================================================================

FUNCTION HDRSRCHIDLE_TA_TIMER_EVENT

DESCRIPTION
  This function processes the tune away timer event.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_ta_timer_event( void )
{

#ifdef FEATURE_HDR_HYBRID_ACCESS
  if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE )
  {
    hdrsrchidle_access_ta_timer_event();
  }
  else
  {
    hdrsrchidle_monitor_ta_timer_event();
  }
#else
    hdrsrchidle_monitor_ta_timer_event();
#endif /* FEATURE_HDR_HYBRID_ACCESS */

} /* hdrsrchidle_ta_timer_event */

#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_RX_POWER_REPORT

DESCRIPTION
  This function reports the RX power, and resets the timer for the next
  RX power report.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_rx_power_report( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_report_rx_power( );
    /* Report the RX power measurement */

  (void) rex_set_timer( HDRSRCHIDLE_RX_RPT_TIMER, HDRSRCHIDLE_RX_RPT_TIME_MS );
    /* Repeat as necessary */

} /* hdrsrchidle_rx_power_report( ) */


/*===========================================================================

FUNCTION HDRSRCHIDLE_KILL_RX_POWER_REPORT

DESCRIPTION
  This function reports the RX power, and resets the timer for the next
  RX power report.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_kill_rx_power_report( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_report_rx_power( );
    /* Report the RX power measurement one last time */

  hdrsrch_kill_timer( HDRSRCHIDLE_RX_RPT_TIMER );
    /* Clear the timer and any outstanding timer signal */

} /* hdrsrchidle_kill_rx_power_report( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_LINK_REPORT

DESCRIPTION
  This function generates a report to the upper protocol layer

DEPENDENCIES
  None

PARAMETERS
  sect          - Sector to report on
  report_func   - Link report function

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchidle_link_report
(
  hdrsrch_sect_struct_type        *sect,
    /* Sector to report on */

  hdrsrch_link_report_func_type   report_func
    /* Link report function */
)
{
  sys_link_type                   link;
    /* Link report structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( report_func != NULL )
  {
    link.pilot_pn         = sect->pn_offset;
    link.channel          = sect->chan;

    report_func( sect->link_id, &link );
      /* Make report */
  }
} /* hdrsrchidle_link_report( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_REQUEST_ACCESS_HANDOFF

DESCRIPTION
  This function generates an Access HO Request to the upper protocol layer.
  If a request has been recently made, the report will be supressed, to avoid
  "nagging" the upper layer.

DEPENDENCIES
  None

PARAMETERS
  sect          - Sector to request a handoff to

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_request_access_handoff
(
  hdrsrch_sect_struct_type  *sect
    /* Sector we desire to perform an access handoff to. */
)
{
  sys_link_type             link;
    /* Link report structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( sect != NULL )
  {
    /* Report access handoff desired to given sector */

    if ( hdrsrchidle.access.request_ho_cb != NULL )
    {
      if ( sect != hdrsrchidle.access.handoff_sector )
      {
        hdrsrchidle.access.request_holdoff = 0;
          /* This is a new pilot - don't wait to report it */
      }

      if ( hdrsrchidle.access.request_holdoff == 0 )
      {
        link.pilot_pn   = sect->pn_offset;
        link.channel    = sect->chan;

        hdrsrchidle.access.request_ho_cb( &link );
          /* Request handoff to this link */

        hdrsrchidle.access.handoff_sector  = sect;
        hdrsrchidle.access.request_holdoff = HDRSRCH_ACCESS_HANDOFF_HOLDOFF;
          /* Don't bother upper layer with this sector for a little while */
      }
      else
      {
        hdrsrchidle.access.request_holdoff--;
          /* Decrement holdoff - we'll report it soon */
      }
    }
  }
  else
  {
    hdrsrchidle.access.handoff_sector  = sect;
    hdrsrchidle.access.request_holdoff = 0;
      /* No current access handoff request; report new ones immediately. */
  }

} /* hdrsrchidle_aho_request_report( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_PREPARE_MONITOR_STATE

DESCRIPTION
  This function changes the searcher to IDLE MONITOR state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Alters priority with SAM

===========================================================================*/

void hdrsrchidle_prepare_monitor_state( void )
{

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
  
  boolean                          access_to_mon;
  /* Temp flag to track if we are changing from access to idle */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_SELECTIVE_TXD
  hdrsrchrftxd_event_payload_type payload;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
   if ( ( hdrsrch_state_id == HDRSRCH_IDLE_STATE )  &&
       ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE  ) &&
        hdrsrch.idle_state_div.idle_div_enabled )
   {
     access_to_mon = TRUE;
     HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Idle Div: access_to_mon %d",
                      access_to_mon );
   }
   else
   {
      access_to_mon = FALSE;
      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Idle Div: access_to_mon %d",
                      access_to_mon );
   }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

#ifdef FEATURE_HDR_SELECTIVE_TXD
   if ( ( hdrsrch_state_id == HDRSRCH_IDLE_STATE )  &&
        ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE  ) )
   {
     HDR_MSG_SRCH( MSG_LEGACY_MED, "TxD: prep mon exit access");
     payload.next_state = HDRSRCH_IDLE_STATE;
     hdrsrchrftxd_event_with_payload( HDRSRCHRFTXD_EVENT_EXITING_ACCESS,
                                      &payload );
   }
#endif /* FEATURE_HDR_SELECTIVE_TXD */

  hdrsrchidle.monitor_substate      = HDRSRCHIDLE_MONITOR_STATE;
    /* Idle handoffs are now under HDR SRCH control. */

  hdrsrchidle.access.suspend_ho_cb = NULL;
    /* Not entering access state, ... */

  hdrsrchidle_request_access_handoff(NULL);
    /* No access handoff requests in progress */

  hdrsrchidle.access.handoff       = NULL;
    /* and ... no access handoff in progress */

  hdrsrchidle_disable_tune_aways( );
    /* Turn off interruptions in the idle (non access) state for primary. */

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV 
  if( hdrsrch.idle_state_div.idle_div_enabled ||
      hdrsrch.idle_state_div.access_div_enabled )
  {
    hdrsrchrfdiv_resume_tuneaway();
    /* Send the command to the RF Diversity State Machine.
       This will kick start div tuneaway timer, if necessary. */
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV || FEATURE_HDR_ACCESS_DIV  */


#ifdef FEATURE_ATOM
  hdrsrchutil_operational_mode_get_nv_configuration();
    /* get NV item based configuration for AT operational mode */
#endif /* FEATURE_ATOM */


} /* hdrsrchidle_init_monitor_state( ) */

/*===========================================================================

FUNCTION HDRSRCHIDLE_INIT_MONITOR_STATE

DESCRIPTION
  This function initializes the searcher as it enters IDLE MONITOR state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Alters priority with SAM

===========================================================================*/

void hdrsrchidle_init_monitor_state( void )
{
  hdrsrchidle_prepare_monitor_state();
    /* Initialize many state variables as we enter Monitor state */
  
  hdrsrchutil_suspend_rpt( HDRSRCH_SUSPEND_NONE );
    /* We are entering Idle monitor state. There is no suspend related activity
       here. Let the protocol know about it */

  hdrsrchofs_reset_repeat_count();
    /* Reset OFS and RUP repeat counts */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  hdrsrchutil_report_coex_power( ( timer_cb_data_type ) NULL );
    /* Report coex power and keep reporting it */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
} /* hdrsrchidle_init_monitor_state( ) */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_IDLE_MODE_CMD

DESCRIPTION
  This function commands the searcher to transition to IDLE

DEPENDENCIES
  TBD

PARAMETERS
  params - IDLE_MODE parameters

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchidle_idle_mode_cmd
(
  const hdrsrch_idle_params_type *params
    /* Parameters for Idle Mode command */
)
{
#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type   idle_demod_idx = HDRSRCH_DEMOD_IDX_0;
    /* Use demod0 in the idle state */

  errno_enum_type                       status = E_SUCCESS;
     /* Command completion status */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "IDLE_MODE_CMD" );

  hdrsrchidle.handoff.report = params->idle_ho_cb;
    /* Remember Idle Handoff callback */

  hdrsrch.suspend_cb         = params->suspend_cb;
    /* Remember the Suspend (RF Gain/Loss) callback */

#ifdef FEATURE_HDR_REVB
  hdrsrch.schdgrp_rpt_cb     = NULL;
    /* Clean the schedulre group report callback */
#endif /* FEATURE_HDR_REVB */

  hdrsrchidle.is_tx_pending = FALSE;

  /* If we are transitioning from access substate to monitor substate, set the
     RF request priority back to OVHD */
  if ( ( hdrsrch_state_id == HDRSRCH_IDLE_STATE )                   &&
       ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE )  )
  {
#ifdef FEATURE_HDR_HYBRID_ACCESS
    hdrsrch_kill_timer( HDRSRCHIDLE_ACCESS_TA_TIMER );
      /* Kill access tune away timer */
#endif
    hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_OVHD );
      /* Exiting from access to idle alter RF priority to overhead */

#if defined(FEATURE_HDR_ACCESS_DIV) || defined (FEATURE_HDR_DYNAMIC_IDLE_DIV) 
    if( hdrsrchrfdiv_diversity_is_enabled() == TRUE  )
    {      
       HDR_MSG_SRCH( MSG_LEGACY_MED,
                     "Idle Div: Change TRM priority back to idle" );
       hdrsrchrf_change_priority( HDRSRCHRF_DIV_CHAIN, 
                                  HDRSRCHRF_IDLE_DIVERSITY );
     }    
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV || FEATURE_HDR_DYNAMIC_IDLE_DIV */

  }

  hdrsrchidle_init_monitor_state( );
    /* Transition to IDLE MONITOR state */

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY

  if( params->is_tca_processing == FALSE)
  {
    hdrsrchidle_monitor_enable_tune_away();
      /* Idle command could be received in either access substate or monitor substate.
	 Unlock callback is unregistered when calling hdrsrchidle_init_monitor_state().
	 We must retain lock again to register unlock callback. It is safe to retain 
	 lock in both cases of idle command in access and in monitor. 
      */
  }
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

#ifdef FEATURE_HDR_REVB
  if( hdrsrchrf_channel_reconfig_is_needed( 1, 
                        &hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel,
                                            &idle_demod_idx ) )
  {
    /* A channel change command is sent to HDR srch itself when the idle channel 
       OR the current demod in use does not match to the current RF configure. 
       This can be expected at the MC traffic to idle transition. */

    hdrsrchidle_idle_link.link.channel  = 
            hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel;
    hdrsrchidle_idle_link.link.pilot_pn = 
            hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->pn_offset;
    hdrsrchidle_idle_link.link.mode     = SYS_HDR_CHAN_REC_MODE_IS856;
    hdrsrchidle_idle_link.chan_change_rpt = NULL;

    status = hdrsrchidle_channel_change_cmd( &hdrsrchidle_idle_link );
      /* Send a channel change command inline to move the idle demod_idx to 0 */
  } 
#endif /* FEATURE_HDR_REVB */

  return status;

} /* hdrsrchidle_idle_mode_cmd( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_START_SLEEP

DESCRIPTION
  This function begins sleep

DEPENDENCIES
  Must only be called when searcher is idle.
  hdrsrchidle_compute_next_cycle( ) must be called prior to this function.

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchidle_start_sleep( void )
{
  boolean will_sleep;
    /* Flag that indicates whether AT is actually going to sleep or not    */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if 0 /* Revisit: might not be needed */
#ifdef FEATURE_HDR_SELECTIVE_TXD
  if ( hdrsrchrftxd_is_in_tx_mode() )
  {
    hdrsrch_on_searcher_idle( hdrsrchidle_start_sleep );
      /* Begin sleep initialization when searcher is idle */

    return;
  }
#endif /* FEATURE_HDR_SELECTIVE_TXD */
#endif

  HDRSRCH_PROFILE( SLP_START );
    /* sleep timeline profiling output if enabled */

  will_sleep = hdrsrchsleep_enter_sleep();
    /* Transition to sleep state.  hdrsrchsleep will decide whether or not
       we will actually sleep based on the time there is until
       the next control channel cycle.  */

  /* If not going to sleep and BCMCS is still enabled, go back to BC state */
  if ( will_sleep == FALSE )
  {
#ifdef FEATURE_HDR_BCMCS
    if ( hdrsrchbc_get_bc_status( ) == BCMCS_ENABLED )
    {
      hdrsrchbc_init_bc_state( );
        /* Transition to BC idle state if BCMCS is enabled at this moment */
    }
#endif /* FEATURE_HDR_BCMCS  */
  }
  else
  {
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
    hdrsrchidle_monitor_disable_tune_away();
      /* Disable tune away when entering sleep. Otherwise we may see race condition
         of idle tuneaway timer times out during entering sleep. We will enter suspend
         state instead of sleep.
         We don't want to disable tune-away if we don't really go to sleep. The timer
         will be stopped when disable_tune_away() is called. If we don't really go
         to sleep, we would hold the chain forever.
      */
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
  }
      
  hdrsrch_complete_cmd( );
    /* We've completed the SLEEP command now */

} /* hdrsrchidle_start_sleep( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_SLEEP_CMD

DESCRIPTION
  This function commands the searcher to transition to enter SLEEP

DEPENDENCIES
  None

PARAMETERS
  params - SLEEP parameters

RETURN VALUE
  E_IN_PROGRESS - if sleep will be started soon.
  E_FAILURE     - if we cannot sleep due to SC estimate not being valid.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchidle_sleep_cmd
(
  const hdrsrch_sleep_params_type *params
    /* Parameters for Idle Mode command */
)
{
  uint16                          hstr;
    /* HSTR count, for temporary storage  */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDRSRCH_PROFILE( SLP_CMD );
    /* sleep timeline profiling output if enabled */

  hstr = HDRHSTR_GET_COUNT( );
    /* Read HSTR count    */

#ifdef FEATURE_HDR_REVB
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "SLEEP_CMD rssi=%d(raw) hstr=%d (%d)",
                  HDRASP_GET_FILT_SINR( HDRMDSP_DEMOD_CARRIER_0, 
          hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->aset.asp_idx ),
                  hstr % 512,
                  hstr );
#else
  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "SLEEP_CMD rssi=%d(raw) hstr=%d (%d)",
                  HDRASP_GET_FILT_SINR( HDRMDSP_DEMOD_CARRIER_0, 
                                        hdrsrchset.aset[0]->aset.asp_idx ),
                  hstr % 512,
                  hstr );
#endif /* FEATURE_HDR_REVB */

  hdrsrchsleep_set_wakeup( params->hdr_slot_cycle,
                           params->hdr_neg_slot_cycle, 
                           params->qpch_enabled, 
                           params->wakeup_cb,
                           params->cc_start_slot_valid,
                           params->cc_start_slot
#ifdef FEATURE_HDR_TO_LTE
                           ,params->ornl_cycle
#endif /* FEATURE_HDR_TO_LTE */

#ifdef FEATURE_HDR_PAGE_BLOCKING_MGMT
                          ,params->session_is_open,
                           params->ovhd_is_updated
#endif /* FEATURE_HDR_PAGE_BLOCKING_MGMT */
                          );
    /* Pass critical sleep information to hdrsrchsleep */

  hdrsrchidle.perform_triage = FALSE;
    /* Suppress triage in next search - we will be unassigning fingers */

  if ( hdrsrchidle.supervision_timer_active )
  {
    hdrsrch_stop_timer( &hdrsrch.timeout_timer );
    hdrsrchidle.supervision_timer_active = FALSE;
      /* Stop the timer before entering sleep. No point in keeping it active
         since we are going to loose our timing info */

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Stop PILOT_SUPERVISION_T before SLEEP" );
  }/* if */
    

  hdrsrchdrv_abort_search( FALSE );
    /* abort on-going searches immediately without waiting for results */

  hdrsrch_kill_timer( &hdrsrch.lostdump_timer );
    /* We don't expect dump anymore */

  hdrsrch_on_searcher_idle( hdrsrchidle_start_sleep );
    /* Begin sleep initialization when searcher is idle */

  return E_IN_PROGRESS;
    /* Command will be completed when hdrsrchidle_start_sleep is called */


} /* hdrsrchidle_sleep_cmd( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_NSET_UPDATE_CMD

DESCRIPTION
  This function adjusts the neighbour set

DEPENDENCIES
  None

PARAMETERS
  params   - New neighbour list message

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchidle_nset_update_cmd
(
  const hdrsrch_nset_update_params_type *params
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "NSET_UPDATE_CMD Num=%d",
                  params->num_pilots );

  HDR_ASSERT ( ( params->num_pilots <= HDRSRCH_NSET_MAX_SIZE ) 
    && ( params->num_channels <= HDRSRCH_HASH_CHAN_MAX_SIZE ) 
    && ( params->num_sects <= HDRSRCH_MAX_CACHE_RECORDS ));
    /* KW fix : Check to limit to max */

  (void) hdrsrchutil_update_nset_pilots( params->nset_pilots, 
                                         params->num_pilots,
                                         params->update_type,
                                         params->channel_list,
                                         params->num_channels,
                                         params->avoided_channel_list,
                                         params->num_avoided_channels
#ifdef FEATURE_HDR_REVB
                                         , params->serving_pilot_gid,
                                         params->hdr_subtype
#endif /* FEATURE_HDR_REVB */
                                         , NULL );
    /* Update the neighbour set */

  hdrsrchutil_update_cached_sectors( params->cached_sects, 
                                     params->num_sects );
    /* Update cached sectors */

} /* hdrsrchidle_nset_update_cmd */


/* <EJECT> */
/*=========================================================================

FUNCTION HDRSRCHIDLE_TUNE

DESCRIPTION
  Tuning after Channel Change, or perhaps for O-Freq neighbour searches

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

=========================================================================*/

LOCAL void hdrsrchidle_tune
( 
  hdrsrchrf_tune_status_enum_type status
)
{
  uint8                     csr_idx;
    /* Carrier selecter index returned from the RF module. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( status != HDRSRCHRF_TUNE_SUCCESS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);

    hdrsrchidle.channel_change = NULL;

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
    return;
  }

  /* Sanity check to make sure the integrity of the TC to idle transition */
  if(!(( ( hdrsrch.aset_channel_cnt == 1 ) &&
          ( hdrsrchset.subaset_cnt == 1 )   &&
          ( hdrsrch.aset_demod[0]  == HDRSRCH_DEMOD_IDX_0 )      &&
          ( hdrsrchset.subaset[HDRSRCH_IDLE_DEMOD_IDX].demod_idx 
                                        == HDRSRCH_DEMOD_IDX_0 ) && 
           SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel[0],
             hdrsrchset.subaset[HDRSRCH_IDLE_DEMOD_IDX].channel ) ) &&
          ( hdrsrchrf_get_carrier_idx( 
           hdrsrchset.subaset[HDRSRCH_IDLE_DEMOD_IDX].channel, &csr_idx ) ) &&
          ( csr_idx == HDRSRCH_DEMOD_IDX_0 )))
   {
     ERR_FATAL( "Integrity check of TC to Idle transition failed", 0, 0, 0 );
   }

	
    /* Carrier index matches to the demod index */

  hdrsrchset.subaset[HDRSRCH_IDLE_DEMOD_IDX].csr_idx = csr_idx;
    /* Update the ASET CSR index after the tuning */

  if ( hdrsrchidle.channel_change )
  {
    /* Since we changed the frequency, start the FLL in acquisition mode */
#ifdef FEATURE_HDR_REVB
    ASSERT( hdrsrch.aset_channel_cnt ==1 );
      /* REVB idle state should only has one channel */
    if ( SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel[0],
                             hdrsrchidle.channel_change->link.channel ) )
#else
    if ( SYS_CHAN_CMP_EQUAL( hdrsrch.aset_channel,
                             hdrsrchidle.channel_change->link.channel ) )
#endif /* FEATURE_HDR_REVB */
    {
      hdrsrchidle_channel_change_complete();
        /* On next dump, complete the channel change; Because tune process 
           has to take at least 10ms, therefore we should not need to run
           this function on search idle. */
    }
  }

  HDRSRCH_SIG_SET( HDRSRCH_SCHEDULE_SIG );
    /* Set the schedule signal, to start the next search on the now current
       frequency */
} /* hdrsrchidle_tune() */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_CHANNEL_CHANGE_COMPLETE

DESCRIPTION
  This function completes the change of channel.

DEPENDENCIES
  Must only be called when searcher is idle.
  Ie, should be called from hdrsrch_on_searcher_idle( )

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchidle_channel_change_complete( void )
{
  hdrsrch_sect_struct_type      *curr_sect;
    /* Current serving sector                       */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  INC_SAT( hdrsrchlog.sleep_info.chan_hash_count );
    /* Increment the channel hash count */
#if !(defined (HDRSRCH_UT_PLATFORM) || defined(TEST_FRAMEWORK))
  /* TBD
  ** Need to clean up this code
  */
  curr_sect = hdrsrchset_get_asp_sect_ptr( HDRSRCH_IDLE_SUBASET_IDX, 
                               HDRASP_GET_BASP_FL( HDRMDSP_DEMOD_CARRIER_0 ) );
    /* Find the current serving sector   */
#else
  #error code not present
#endif /* HDRSRCH_UT_PLATFORM */

  if ( curr_sect != NULL)
  {
    hdrsrchidle_link_report( curr_sect,
                           hdrsrchidle.channel_change->chan_change_rpt );
    /* Report channel changed to given sector */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR, "NO serving sector after channel change!");
  }

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV 
  if ( ( hdrsrch.idle_state_div.access_div_enabled ||
       hdrsrch.idle_state_div.idle_div_enabled ) && 
       hdrsrch.idle_state_div.wakeup_info.div_enabled ) 
  {
    /* If tuning back to idle turn on diversity */
    
    if( !hdrsrchrfdiv_diversity_is_enabled() )
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Turning on div after channel change");

      hdrsrchrfdiv_diversity_ctrl( TRUE );
        /* Enable diversity in access state. */
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Not turning on div after channel change"
                                    " mode as it is already on");
    }
  }
#endif /* FEATURE_HDR_ACCESS_DIV */
  hdrsrchidle.channel_change = NULL;

  hdrdec_reset_pkt_sequences();
    /* Reset packet sequence numbers */

  hdrdec_activate();
    /* Activate HDR decoder after switching frequencies. */

  hdrsrch_complete_cmd();
    /* Command is now complete */

if ( hdrsrchidle.access.waiting_chan_change )
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "resume tuneaway after chan change" );    
    hdrsrchidle.access.waiting_chan_change = FALSE;
    hdrsrchidle_notify_acmac_ta_time();
      /* re-eval if tuneaway is needed */
  }

#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
  if ( hdrsrchidle.monitor_ta.wait_for_channel_change == TRUE )
  {
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_SUSPENDING );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

    /* Start tune away after channel change is done, if there is a pending tune away */
    hdrsrch_tune_away_on_searcher_idle( hdrsrchidle_on_search_idle_cb );
      /* change to suspended idle state, only when searcher is idle  */
    hdrsrchidle.monitor_ta.wait_for_channel_change = FALSE;
  }
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */
} /* hdrsrchidle_channel_change_complete( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_CHANNEL_CHANGE

DESCRIPTION
  This function changes the current channel.

DEPENDENCIES
  Must only be called when searcher is idle.
  Ie, should be called from hdrsrch_on_searcher_idle( )

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

LOCAL void hdrsrchidle_channel_change( void )
{

  hdrsrch_sect_struct_type  *sect;
    /* Sector pointer */

  hdrsrchset_pilotpurge_struct_type         pilotpurge;
    /* Contains input ptr and default action type to be passed to the pilot purge
       function. */

  boolean tune_status;
    /* status of tune to chan */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* No search is currently active - OK to tune */

  hdrsrch_stop_timer( &hdrsrch.schedule_timer );
    /* Stop scheduler ... we are tuning */

  /* get a new pilot or existing pilot with this channel */
  sect = hdrsrchset_get_sect(
                          hdrsrchidle.channel_change->link.pilot_pn,
                          hdrsrchidle.channel_change->link.channel );

  if ( sect != NULL )
  {
#ifdef FEATURE_HDR_REVB
    if ( sect->set == HDR_EMPTYSET )
    {
      /* If the sect comes from EMPTY SET, then update the gid with 
         the current aset gid. This is based on the assumption that
         sectors of the same pilot group as the ASET sector is purged
         during the nset update. */

      sect->pilot_gid = hdrsrchset.subaset[0].aset[0]->pilot_gid;
    }
#endif /* FEATURE_HDR_REVB */

    hdrsrchutil_force_wincenter_to_mstr( sect );
      /* update the srch window center based on the current mstr */

    hdrsrchutil_aset_idle_handoff( sect );
      /* idle handoff to the new frequency */

    hdrsrchset_reset_ofs_filter( sect );
      /* Reset OFS filter */

#ifdef FEATURE_HDR_REVB
    pilotpurge.action = HDRSRCHSET_PILOTPURGE_ACTION_NO_ACTION;
      /* Reset action to default value */
    pilotpurge.sect_ptr = hdrsrchset.subaset[0].aset[0];
    hdrsrchset_pilot_group_purge_mc( &pilotpurge, NULL );
      /* Enforce the group policy on the new aset pilot */
#endif /* FEATURE_HDR_REVB */

    hdrsrchset_reset_ofs_filter( sect );
      /* Reset OFS filter */

    hdrsrchset_remove_same_pn_on_diff_channel( sect, FALSE, NULL );
      /* Delete pilots with same PN on other channels. No channel change allowed
         back to this PN on any other channel until next NSET update is done. */
  }
  else
  {
    ERR_FATAL( "No free sector available during idle channel change!", 0, 0, 0 );
  }

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV 

  if( hdrsrch.idle_state_div.idle_div_enabled ||
      hdrsrch.idle_state_div.access_div_enabled )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Idle Div: channel change disable div" );

    hdrsrchrfdiv_diversity_ctrl( FALSE );
      /* Disable Diversity */
    
    if( hdrsrch.idle_state_div.idle_div_enabled )
    {
      hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg = 
        hdrsrchidle_get_prev_wakeup_psinr();
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Idle Div: PSINR avg %d",
                      hdrsrch.idle_state_div.wakeup_info.prev_psinr_avg );
        /* Save previous wakeup inof and reset */

      /* Deactivate decode m slot indication */
      hdrsrchsleep_deactivate_scc_decode_ind();
  
      hdrlog_register_slot_logging_cb ( HDRLOG_SRCHDIV_MDSP_SLOT_LOGGING_CLIENT,
                                        NULL );
    }   
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

  hdrsrchutil_cancel_fpd( );

  /* Deassign all fingers before retune */
  hdrsrchfing_deassign_all_fings();
  
  hdrsrchutil_dec_deactivate( );
    /* Deactivate HDR decoder before going to 1x */

#ifdef FEATURE_HDR_REVB
  HDR_ASSERT ( hdrsrch.aset_channel_cnt <= HDRSRCH_MAX_NUM_CARRIERS ) ;
    /* KW fix : Check to limit hdrsrch.aset_channel_cnt to max */
#endif /* FEATURE_HDR_REVB */

    tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
#ifndef FEATURE_HDR_REVB
                            &hdrsrchidle.channel_change->link.channel,
#else
                            hdrsrch.aset_channel,
                            hdrsrch.aset_demod,
                            hdrsrch.aset_channel_cnt,
#endif /* FEATURE_HDR_REVB */
                            hdrsrchidle_tune,
                            HDRSRCHIDLE_BAND_GRANT_TIMEOUT_MS );
      /* Tune the RF to the required channel */

  if ( tune_status == FALSE )
  {
  
    hdrsrchidle.channel_change  = NULL;
      /* Clear channel change cmd */
  
    hdrsrch_complete_cmd( );
      /* Discard the Channel change parameters complete the command */
  
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
  
    return;
  }    
} /* hdrsrchidle_channel_change( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_CHANNEL_CHANGE_CMD

DESCRIPTION
  This function switches the active channel

DEPENDENCIES
  None

PARAMETERS
  params        - New channel parameters

RETURN VALUE
  E_IN_PROGRESS - Command is not completed immediately.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchidle_channel_change_cmd
(
  const hdrsrch_channel_change_params_type *params
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrch_on_searcher_idle_reged() )
  {
    /* If tuneaway on search idle is reged, delay chan_change command */
    HDR_MSG_SRCH( MSG_LEGACY_MED, "chan change delayed after tuneaway" );    
    return E_AGAIN;
  }
  else
  {

    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "CHANNEL_CHANGE_CMD Ch=%d Band=%d PN=%d",
                    params->link.channel.chan_num,
                    params->link.channel.band,
                    params->link.pilot_pn );

    hdrsrchidle.channel_change = params;
      /* Hold on to the channel change parameters until command is done */

    hdrsrch_on_searcher_idle( hdrsrchidle_channel_change );
      /* When the searcher has stopped, change the channel */

    return E_IN_PROGRESS;
      /* Command is not complete yet */
  }

} /* hdrsrchidle_channel_change_cmd( ) */


/*===========================================================================

FUNCTION HDRSRCHIDLE_CHANNEL_CHANGE_AND_NSET_UPDATE_CMD

DESCRIPTION
  This function switches the active channel and update nset.

DEPENDENCIES
  None

PARAMETERS
  params        - New channel and nset parameters

RETURN VALUE
  E_IN_PROGRESS - Command is not completed immediately.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_channel_change_and_nset_update_cmd
(
  const hdrsrch_channel_change_and_nset_update_params_type *params
    /* params */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrch_on_searcher_idle_reged() )
  {
    /* If tuneaway on search idle is reged, delay chan_change command */
    HDR_MSG_SRCH( MSG_LEGACY_MED, "chan change delayed after tuneaway" );    
    return E_AGAIN;
  }
  else
  {

    HDR_MSG_SRCH_4( MSG_LEGACY_MED, "CHANNEL_CHANGE_AND_NSET_UPDATE_CMD Ch=%d Band=%d PN=%d Num Pilots %d",
                    params->link.channel.chan_num,
                    params->link.channel.band,
                    params->link.pilot_pn,
                    params->num_pilots );

    /* Nset update with the preference of the new channel */
    (void) hdrsrchutil_update_nset_pilots( params->nset_pilots, 
                                           params->num_pilots,
                                           params->update_type,
                                           params->channel_list,
                                           params->num_channels,
                                           params->avoided_channel_list,
                                           params->num_avoided_channels
#ifdef FEATURE_HDR_REVB
                                           , params->serving_pilot_gid,
                                           params->hdr_subtype
#endif /* FEATURE_HDR_REVB */
                                           , &(params->link.channel) );
    /* Update the neighbour set */

    hdrsrchutil_update_cached_sectors( params->cached_sects, 
                                     params->num_sects );
    /* Update cached sectors */

    /* Channel change */
    hdrsrchidle.channel_change = ( hdrsrch_channel_change_params_type *)params;
      /* Hold on to the channel change parameters until command is done */

    hdrsrch_on_searcher_idle( hdrsrchidle_channel_change );
      /* When the searcher has stopped, change the channel */

    return E_IN_PROGRESS;
      /* Command is not complete yet */
  }

} /* hdrsrchidle_channel_change_and_nset_update_cmd( ) */

/*===========================================================================

FUNCTION HDRSRCHIDLE_REQUEST_TX_CAPABLE_CHAIN

DESCRIPTION
  This function gives up current RX_ONLY chain by tranitioning to 3a,
  and requests for RXTX_BEST.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_request_tx_capable_chain( void )
{
  if ( !hdrsrchidle.is_tx_pending )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH,  
      "Request TX chain, delaying access!!" );

    hdrsrchidle.is_tx_pending = TRUE;
    hdrsrchidle_process_monitor_tune_away( 0 );
  }
} /* hdrsrchidle_request_tx_capable_chain */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_ACCESS_MODE_CMD

DESCRIPTION
  This function commands the searcher to transition to ACCESS

DEPENDENCIES
  None

PARAMETERS
  params - ACCESS_MODE parameters

RETURN VALUE
  E_SUCCESS     - if access mode is immediately entered.
  E_IN_PROGRESS - if access mode will be entered when IDLE is entered.

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchidle_access_mode_cmd
(
  const hdrsrch_access_params_type *params
    /* Parameters for Idle Mode command */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* get_rf_device() calls TRM API and returns MAX_DEVICE
     if no device is assigned */
  if ( hdrsrchrf_get_rf_device( TRM_HDR_TX ) == 
       RFM_MAX_DEVICES )
  {
    hdrsrchidle_request_tx_capable_chain();

    return E_AGAIN;
  }

  if ( hdrsrch_state_id == HDRSRCH_IDLE_STATE )
  {

#ifdef FEATURE_HDR_HYBRID_ACCESS
    if ( hdrsrchidle.monitor_substate != HDRSRCHIDLE_ACCESS_STATE) 
    {
#ifdef FEATURE_HDR_SELECTIVE_TXD
      hdrsrchrftxd_event( HDRSRCHRFTXD_EVENT_ACCESS_ENTERED );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      hdrsrchidle.access.tune_away.amac_ready = FALSE;
        /* Need to wait for AMAC ready cmd */

      hdrsrchidle.access.tune_away.state = HDRSRCHIDLE_TUNE_AWAY_NO_CONFLICT;
        /* Reset tune away state */

#ifdef FEATURE_HDR_QUICK_IDLE
      hdrsrchutil_disable_qpch( );
#endif /* FEATURE_QUICK_IDLE */

      hdrsrchidle.monitor_substate = HDRSRCHIDLE_ACCESS_STATE;
      /* Idle handoffs are now under HDR SRCH control. Not applicable to 
         BC state */

    /* Removed the disable TA so that HDR will handle unlocks */

      /* Alter priority to indicate we are originating. If this is a broadcast
         access this has already been done by the broadcast state machine. */
    if ( 
#ifdef FEATURE_HDR_DUAL_SIM
        ( !hdrmultirat_is_dsds_enabled() ) && 
#endif /* FEATURE_HDR_DUAL_SIM */
         HDRSRCH_ACCESS_HYBRID_NV_ENABLE )
    {
      hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, 
                                 HDRSRCHRF_ACCESS );
    }
    else
    {
      hdrsrchrf_change_priority( HDRSRCHRF_PRI_CHAIN, HDRSRCHRF_ACCESS_URGENT );
    }

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
    /* Change diversity priority if it is on */
    if( hdrsrchrfdiv_diversity_is_enabled() == TRUE ) 
    {      
       hdrsrchrf_change_priority( HDRSRCHRF_DIV_CHAIN, 
                                  HDRSRCHRF_ACCESS_DIVERSITY );
     }    
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */
  }
    else
    {
      HDR_MSG_SRCH_2( MSG_LEGACY_MED,
                      "Already in access, acmac_ready %d, ta state %d",
                      hdrsrchidle.access.tune_away.amac_ready,
                      hdrsrchidle.access.tune_away.state );
    }
#endif /* FEATURE_HDR_HYBRID_ACCESS */

  }

  hdrsrchidle.access.request_ho_cb = params->request_ho_cb;
    /* Remember Access Handoff Request callback */

  /* Save (and sanitize) Access Handoff threshold */

  hdrsrchidle.access.thresh_0p5_db = params->thresh_0p5_db;
  if (hdrsrchidle.access.thresh_0p5_db < HDRSRCHIDLE_ACCESS_HO_THRESH_HALF_DB)
  {
    hdrsrchidle.access.thresh_0p5_db = HDRSRCHIDLE_ACCESS_HO_THRESH_HALF_DB;
  }
  else if ( hdrsrchidle.access.thresh_0p5_db > 60 )
  {
    hdrsrchidle.access.thresh_0p5_db = 60;
      /* Maximum 30 dB supported */
  }


  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "ACCESS_CMD  HO @ %d.%d db (%d)",
                  (hdrsrchidle.access.thresh_0p5_db / 2),
                  (hdrsrchidle.access.thresh_0p5_db & 1) * 5,
                  params->thresh_0p5_db );


  hdrsrchidle_request_access_handoff(NULL);
    /* No access handoff requests in progress */

  hdrsrchidle.access.handoff = NULL;
    /* No access handoff in progress */

#ifdef FEATURE_HDR_ACCESS_DIV

  if( hdrsrch.idle_state_div.access_div_enabled )
  {      
     if( !hdrsrchrfdiv_diversity_is_enabled() )
     {
       HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Turning on div in access mode");

       hdrsrchrfdiv_diversity_ctrl( TRUE );
        /* Enable diversity in access state. */
     }
     else
     {
       HDR_MSG_SRCH( MSG_LEGACY_MED, "Idle Div: Not turning on div in access"
                                     " mode as it is already on");
     }    
  }
#endif /* FEATURE_HDR_ACCESS_DIV */

#ifdef FEATURE_HDR_BCMCS
  if ( ( hdrsrch_state_id == HDRSRCH_IDLE_STATE ) ||
       ( hdrsrch_state_id == HDRSRCH_BC_ACCESS_STATE ) )
#else /* FEATURE_HDR_BCMCS */
  if ( hdrsrch_state_id == HDRSRCH_IDLE_STATE )
#endif /* FEATURE_HDR_BCMCS */
  {

#ifdef FEATURE_HDR_HYBRID_ACCESS
    if ( hdrsrch_state_id != HDRSRCH_IDLE_STATE )
    {
      hdrsrchidle_enable_tune_aways( );
        /* Allow interruptions during access attempts. */
    }
    else
    {
      if ( hdrsrchidle.access.tune_away.amac_ready == FALSE )
      {
      hdrsrchidle_disable_tune_aways( );
        /* Disable tuneways till AMAC Ready cmd */

        hdrsrch_kill_timer( HDRSRCHIDLE_ACCESS_TA_TIMER );
          /* Kill access tune away timer */
      }
    }
#endif /* FEATURE_HDR_HYBRID_ACCESS */

#ifdef FEATURE_HDR_ACCESS_DIV 
  if( hdrsrch.idle_state_div.access_div_enabled )
  {
    hdrsrchrfdiv_resume_tuneaway();
    /* Send the command to the RF Diversity State Machine.
       This will kick start div tuneaway timer, if necessary. */
  }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV || FEATURE_HDR_ACCESS_DIV  */

    params->suspend_ho_cb( );
      /* Report to upper layer that handoffs have been suspended */

    return E_SUCCESS;
  }
  else
  {
    hdrsrchidle.access.suspend_ho_cb = params->suspend_ho_cb;
      /* Remember to report access mode entry */

    return E_IN_PROGRESS;
  }

} /* hdrsrchidle_access_mode_cmd( ) */


#ifdef FEATURE_HDR_HYBRID_ACCESS
/*===========================================================================

FUNCTION HDRSRCHIDLE_ACMAC_READY_CMD

DESCRIPTION
  This function processes acmac ready command. It calls retain lock 
  which triggers rf unlock event. RF unlock event causes tune away 
  time command sent to ACMAC.
  
DEPENDENCIES
  None

PARAMETERS
  params - Callback function to notify tune away time

RETURN VALUE
  E_SUCCESS     - Command succeeded.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_acmac_ready_cmd
(
  const hdrsrch_acmac_ready_params_type *params
    /* Parameters for acmac ready command */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchidle.access.ta_time_cb = params->ta_time_cb;
  hdrsrchidle.access.stop_cb    = params->stop_cb;

  hdrsrchidle.access.tune_away.amac_ready = TRUE;
    /* AMAC is ready */

  /* Enable tune away. This will trigger or simulate TRM event.
     TRM event may trigger callback function to notify ACMAC tune away time */
  hdrsrchidle_enable_tune_aways( );
    /* Allow interruptions during access attempts. */

  return E_SUCCESS;

} /* hdrsrchidle_acmac_ready_cmd */


/*===========================================================================

FUNCTION HDRSRCHIDLE_ACMAC_TA_TIME_OK_CMD

DESCRIPTION
  This function processes tune away time ok command. 

DEPENDENCIES
  None

PARAMETERS
  params - Callback function to notify tune away time

RETURN VALUE
  E_SUCCESS     - Command succeeded.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_acmac_ta_time_ok_cmd
(
  const hdrsrch_acmac_ta_time_ok_params_type *params
    /* Parameters for ta time ok command */
)
{

  uint32                         sclks_now;
    /* Current sclk count */

  uint32                         sclks_left;
    /* Sclks remaining until lock release */
  
  unsigned int                   ms_left;
    /* Maximum duration for which we can have the RF */

  hdrsrchidle_access_tune_away_type *ta;
    /* Pointer to the tune away structure */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchidle.monitor_substate  == HDRSRCHIDLE_ACCESS_STATE ) 
  {
    ta = &hdrsrchidle.access.tune_away;

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "hybrid_access : state %d Received amac ta_time_ok cmd %d", 
                    ta->state, params->ta_time_ok );

    if ( ta->state == HDRSRCHIDLE_TUNE_AWAY_DISABLED) 
    {
      /* Tune away is disabled by ACMAC */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                      "ta is disabled until %d sclk now %d sclk",
                      ta->disable_ta.end_time_sclks, timetick_get() );
    }
    else
    {
      /* Tune away is not disabled */

      if ( params->ta_time_ok == TRUE )
      {
        /* Tune away time is ok */

        sclks_now = timetick_get();
          /* Current time in sclks. */ 

        sclks_left = hdrsrchutil_calc_time_to_tune_away( 
                                     sclks_now,
                                     (int32) HDRSRCHIDLE_OVERHEAD_SCLK,
                                     &hdrsrchidle.access.tune_away.rl_state,
                                     &hdrsrchidle.access.tune_away.no_tune_away,
                                     &hdrsrchidle.access.tune_away.end_time_sclks,
                                     NULL );
          /* How much more time should we stay in idle access? */

        hdrsrchidle.access.tune_away.start_time_sclks = sclks_now;
          /* Keep track of the current time. */

        ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
          /* Convert lock extension to milliseconds. */

        hdrsrchidle_process_access_tune_away( ms_left );

#if defined FEATURE_HDR_DYNAMIC_IDLE_DIV || defined FEATURE_HDR_ACCESS_DIV 
       if( hdrsrch.idle_state_div.idle_div_enabled ||
           hdrsrch.idle_state_div.access_div_enabled )
       {
         hdrsrchrfdiv_resume_tuneaway();
          /* Send the command to the RF Diversity State Machine.
           This will kick start div tuneaway timer, if necessary. */
       }
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV || FEATURE_HDR_ACCESS_DIV  */

      } /* if ( params->ta_time_ok == TRUE ) */
    }  /* if ( ta->state == HDRSRCHIDLE_TUNE_AWAY_DISABLED) */
  } /* if ( hdrsrchidle.monitor_substate  == HDRSRCHIDLE_ACCESS_STATE )  */

  return E_SUCCESS;

} /* hdrsrchidle_acmac_ta_time_ok_cmd */

/*===========================================================================

FUNCTION HDRSRCHIDLE_ACMAC_DISABLE_TA_CMD

DESCRIPTION
  This function processes tune away disable command. 

DEPENDENCIES
  None

PARAMETERS
  params - The duration that tune away should be disabled in ms.

RETURN VALUE
  E_SUCCESS     - Command succeeded.

SIDE EFFECTS
  None

===========================================================================*/

errno_enum_type hdrsrchidle_acmac_disable_ta_cmd
(
  const hdrsrch_acmac_disable_ta_params_type *params
    /* Parameters for disable ta command */
)
{

   hdrsrchidle_access_tune_away_type *ta;
    /* Pointer to the tune away structure */

   qword                             ta_time;
     /* Tune away time in slots */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE )
  {
    ta = &hdrsrchidle.access.tune_away;

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "hybrid_access : state %d amac disable cmd duration %d", 
                    ta->state, params->no_ta_duration );


    if ( params->no_ta_duration > 0 )
    {
      /* Disable tune away */

      ta->state = HDRSRCHIDLE_TUNE_AWAY_DISABLED;

      ta->disable_ta.no_ta_duration = params->no_ta_duration;

      ta->disable_ta.start_time_sclks = timetick_get();
        /* Current time in sclks. */ 

      ta->disable_ta.end_time_sclks = ta->disable_ta.start_time_sclks +
                   timetick_cvt_to_sclk( (uint32) ta->disable_ta.no_ta_duration, T_MSEC );
        /* Convert disable duration to sclk */


      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                    "hybrid_access : ta disabled till %d pri unlock_by %d ",
                    ta->disable_ta.end_time_sclks, ta->rl_state.unlock_by_sclk );

      if ( ( ta->rl_state.event != HDRSRCHRF_UNLOCK_CANCELLED ) &&
           ( ta->rl_state.unlock_by_sclk < ta->disable_ta.end_time_sclks ) )
      {
        /* If pri chain tune away is disabled longer than unlock_by time, need to inform
           ACMAC that there is no tune away by ta_disabled end time.
        */

        hdrsrchidle_ta_time_to_slots( ta_time, ta->disable_ta.end_time_sclks );
          /* Convert tune away time to slots */

        if ( hdrsrchidle.access.ta_time_cb != NULL )
        {
          hdrsrchidle.access.ta_time_cb( FALSE, ta_time );
          /* Notify ACMAC */
        }

        /* And we need to start tune away timer for diversity chain. Because pri chain unlock_by
           time and diversity unlock_by time might be the same, tune away timer for diversity
           chain might not be started yet. 
        */
        hdrsrchrfdiv_resume_tuneaway();
      }

      hdrsrch_kill_timer( HDRSRCHIDLE_ACCESS_TA_TIMER );
        /* Kill access tune away timer **/

      (void) rex_set_timer( HDRSRCHIDLE_ACCESS_TA_TIMER, 
                            ta->disable_ta.no_ta_duration );
        /* set timer for next event */

      hdrsrch_add_to_wait_mask( HDRSRCHIDLE_ACCESS_TA_SIG );
        /* Watch for access TA signal */

    } /* if ( params->no_ta_duration > 0 ) */
  } /* if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE ) */
  
  return E_SUCCESS;

} /* hdrsrchidle_acmac_disable_ta_cmd */

#endif /* FEATURE_HDR_HYBRID_ACCESS */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_ACCESS_HANDOFF_CMD

DESCRIPTION
  This function allows search to perform 1 handoff

DEPENDENCIES
  None

PARAMETERS
  params        - ACCESS_HANDOFF parameters

RETURN VALUE
  E_IN_PROGRESS - Access handoff is not immediately completed

SIDE EFFECTS
  None

===========================================================================*/

LOCAL errno_enum_type hdrsrchidle_access_handoff_cmd
(
  const hdrsrch_access_handoff_params_type *params
    /* Parameters for Idle Mode command */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( 
#ifdef FEATURE_HDR_BCMCS
       ( hdrsrch_state_id               == HDRSRCH_BC_ACCESS_STATE  ) ||
#endif /* FEATURE_HDR_BCMCS */
       ((hdrsrch_state_id             == HDRSRCH_IDLE_STATE  )      &&
       ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE ) ))
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "ACCESS_HO_CMD" );

    hdrsrchidle.access.handoff = params;
      /* Record access handoff parameters.  Existence of parameters for an
         access handoff also implies an access handoff is in progress. */

    return E_WOULD_BLOCK;
      /* E_IN_PROGRESS, but ok to abort this command if DEACTIVATE'd */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "ACCESS_HO_CMD while not in ACCESS" );

    return E_FAILURE;
  }

} /* hdrsrchidle_access_handoff_cmd( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_TC_ASSIGN_CMD

DESCRIPTION
  This function transitions IDLE (Monitor, Access, or Sleep) state to
  the connected state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  FALSE - Transition to traffic channel will take several steps.  Command
          is not finished until all the steps are complete.

SIDE EFFECTS
  Transition to Connected (Traffic Channel) state.
  Command processor is suspended until the transition is complete.

===========================================================================*/

LOCAL errno_enum_type hdrsrchidle_tc_assign_cmd( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH( MSG_LEGACY_MED, "TC_ASSIGN_CMD" );

  /* get_rf_device() calls TRM API and returns MAX_DEVICE
     if no device is assigned */
  if ( hdrsrchrf_get_rf_device( TRM_HDR_TX ) == 
       RFM_MAX_DEVICES )
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH,  
      "Request TX chain, delaying TCA!!" );

    hdrsrchidle_request_tx_capable_chain();

    return E_AGAIN;
  }

  hdrsrchlog.sleep_info.next_srch_state = HDRSRCH_CONNECTED_STATE;
    /* Updates the next_srch_state field in the log packet            */

  hdrsrchlog_log_sleep_info( );
    /* State transition from IDLE to TRAFFIC, needs to log this packet    */

  hdrsrchidle_disable_tune_aways();
    /* We are not allowed to tune away in this state. */

  hdrsrchtc_init_connected_state(  );
    /* Transition to connected state */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV 
  if( hdrsrch.idle_state_div.idle_div_enabled )
  {
    hdrsrchrfdiv_diversity_ctrl( FALSE );

    hdrsrchsleep_deactivate_scc_decode_ind( );
    /* Deactivate decode m slot indication */

    hdrlog_register_slot_logging_cb ( HDRLOG_SRCHDIV_MDSP_SLOT_LOGGING_CLIENT,
                                      NULL );
      /* Disable psinr logging */

    hdrsrchutil_reset_prev_wakeup_info();
     /* Since we are moving to connected state
       Reset all prev wakeup info */
  }   
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

  return E_AGAIN;
    /* Retry this command once we have switched to the Connected State */

} /* hdrsrchidle_tc_assign_cmd( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_DEFAULT_IDLE_CMD

DESCRIPTION
  This function ...

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

LOCAL errno_enum_type hdrsrchidle_default_idle_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status = E_SUCCESS;
     /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch ( cmd->name )
  {
    case HDRSRCH_IDLE_CMD:
      status = hdrsrchidle_idle_mode_cmd( &cmd->params.idle );
      break;

    case HDRSRCH_NSET_UPDATE_CMD:
      hdrsrchidle_nset_update_cmd( &cmd->params.nset_update );
      break;

    case HDRSRCH_SLEEP_CMD:
      status = hdrsrchidle_sleep_cmd( &cmd->params.sleep );
      break;

    case HDRSRCH_ACCESS_CMD:

      hdrsrchlog.sleep_info.next_srch_state = HDRSRCH_IDLE_STATE;
        /* Since we don't have a dedicated state for IDLE access,
           here we just borrow HDRSRCH_IDLE_STATE and it will be
           treated as IDLE ACCESS state in the log parser output  */

      hdrsrchlog_log_sleep_info( );
        /* State transition from IDLE monitor to IDLE access,
           needs to log this packet    */

      status = hdrsrchidle_access_mode_cmd( &cmd->params.access );
      break;

    case HDRSRCH_SYNCHRONIZE_CMD:
      status = hdrsrchutil_sync_check( &cmd->params.synchronize );
      break;

    case HDRSRCH_REENABLE_HANDOFFS_CMD:
      if ( !hdrsrchidle.handoff.allowed )
      {
        HDR_MSG_SRCH( MSG_LEGACY_MED, "Enabling handoffs in idle." );

        hdrsrchidle.handoff.allowed = TRUE;
          /* Handoffs are allow again */
      }

      hdrsrchidle.handoff.last_hstr = HDRHSTR_GET_COUNT() % 512;
        /* Force next dump to find increased HSTR */

      break;
    
    case HDRSRCH_RUP_REQ_CMD:
      status = hdrsrchutil_rup_req_cmd( &cmd->params.rup_req );
      
      /* If this RUP request is for other channel then go to 
         idle OFS state when searcher becomes idle */
      if ( ( status == E_SUCCESS) && 
           ( hdrsrchofs_rup_pending() == TRUE ) )
      {
        hdrsrchofs_register_idle_ofs_return_state( HDRSRCH_IDLE_STATE );
          /* after idle ofs we should come back to monitor state */
      
        hdrsrch_on_searcher_idle( hdrsrchofs_init_ofs_state );
      }
      break;

    case HDRSRCH_DEACTIVATE_CMD:

      hdrsrchlog.sleep_info.next_srch_state = HDRSRCH_INACTIVE_STATE;
        /* Updates the next_srch_state field in the log packet      */

      hdrsrchlog_log_sleep_info( );
        /* State transition from IDLE to INACTIVE, needs to log this
           packet.                                                  */


      if ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE ) 
      {
        /* Kill tune away timer */
        hdrsrch_kill_timer( HDRSRCHIDLE_ACCESS_TA_TIMER );
      }
#ifdef FEATURE_HDR_IDLE_TUNE_AWAY
      else
      {
       /* Kill tune away timer */
        hdrsrch_kill_timer( HDRSRCHIDLE_MONITOR_TA_TIMER );
      }
#endif /* FEATURE_HDR_IDLE_TUNE_AWAY */

        /* Falls through to default                                 */

    default:
      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */
  }

  return status;

} /* hdrsrchidle_default_idle_cmd( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_PILOT_SUPERVISION

DESCRIPTION
  This function handles pilot supervision in the idle state.
  If all pilots drop below PilotDrop, a supervision timer starts running.
  If any pilot rises above PilotDrop, the timer is halted.
  [6.6.5.5.2]

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchidle_pilot_supervision( void )
{
  hdrsrch_sect_struct_type *    best_sect = NULL;
    /* Pointer to a strongest sector */

  int                           best_energy = -1;
    /* Strongest sector's energy */

  int                           drop_thr;
    /* Drop threshold in linear scale */

  int                           i;
    /* Sector loop index */

  int                           pilot_supervision_ms;
    /* pilot supervision timer is same as DROP timer as per IS-856-2 */

#ifdef FEATURE_HDR_REVB
  uint8                         aset_cnt;
    /* The number of aset sectors */

  hdrsrch_sect_struct_type *    aset_sect; 
    /* Sector pointor */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  drop_thr = hdrsrchset_ecio_to_eng(hdrsrch.same_chan_mgmt_params.pilot_drop);
    /* Convert the drop threshold from 0.5dB units to linear value */

#ifdef FEATURE_HDR_REVB
  pilot_supervision_ms = hdrsrchset_get_drop_timer_ms( 
                        hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0] );
    /* Get the drop timer for aset channel. That will be same as the pilot
       supervision timer as per IS856-2. DDR #3 */
#else
  pilot_supervision_ms = hdrsrchset_get_drop_timer_ms( hdrsrchset.aset[0] );
    /* Get the drop timer for aset channel. That will be same as the pilot
       supervision timer as per IS856-2. DDR #3 */
#endif /* FEATURE_HDR_REVB */

  /*------------*
   * Check ASET *
   *------------*/
#ifdef FEATURE_HDR_REVB
  aset_cnt = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt; 

  for(i=0; i<aset_cnt; i++)
  {
    aset_sect = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[i];

    if ( aset_sect->total_eng > best_energy )
    {
      best_sect   = aset_sect;
      best_energy = aset_sect->total_eng;
        /* Remember this sector and it's energy */
    }
  }
#else
  for(i=0; i<hdrsrchset.aset_cnt; i++)
  {
    if ( hdrsrchset.aset[i]->total_eng > best_energy )
    {
      best_sect   = hdrsrchset.aset[i];
      best_energy = hdrsrchset.aset[i]->total_eng;
        /* Remember this sector and it's energy */
    }
  }
#endif /* FEATURE_HDR_REVB */

  /*------------*
   * Check CSET *
   *------------*/

  for(i=0; i<hdrsrchset.cset_cnt; i++)
  {
#ifdef FEATURE_HDR_REVB
    if ( SYS_CHAN_CMP_EQUAL(hdrsrchset.cset[i]->chan, 
                 hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->chan ) && 
        ( hdrsrchset.cset[i]->total_eng > best_energy  ))
#else 
    if ( SYS_CHAN_CMP_EQUAL(hdrsrchset.cset[i]->chan, hdrsrchset.aset[0]->chan ) 
         && hdrsrchset.cset[i]->total_eng > best_energy  )
#endif /* FEATURE_HDR_REVB */
    {
      best_sect   = hdrsrchset.cset[i];
      best_energy = hdrsrchset.cset[i]->total_eng;
        /* Remember this sector and it's energy */
    }
  }

  /*------------*
   * Check NSET *
   *------------*/

  for (i=0; i<hdrsrchset.nset_cnt; i++)
  {
#ifdef FEATURE_HDR_REVB
    if ( SYS_CHAN_CMP_EQUAL(hdrsrchset.nset[i]->chan, 
                 hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->chan ) && 
        ( hdrsrchset.nset[i]->total_eng > best_energy  ))
#else 
    if ( SYS_CHAN_CMP_EQUAL(hdrsrchset.nset[i]->chan, hdrsrchset.aset[0]->chan)
         && hdrsrchset.nset[i]->total_eng > best_energy )
#endif /* FEATURE_HDR_REVB */
    {
      best_sect   = hdrsrchset.nset[i];
      best_energy = hdrsrchset.nset[i]->total_eng;
        /* Remember this sector and it's energy */
    }
  }

  /*------------* Supervision processing *------------*/

  if ( best_energy >= drop_thr )
  {
    /* We have a pilot which is strong enough! */

    if ( hdrsrchidle.supervision_timer_active )
    {
      hdrsrch_stop_timer( &hdrsrch.timeout_timer );
      hdrsrchidle.supervision_timer_active = FALSE;
        /* Stop the timer; we have a strong pilot. (whew!) */

      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Stop PILOT_SUPERVISION_T: PN=%d E=%d T=%ds",
                      best_sect->pn_offset,
                      best_energy,
                      pilot_supervision_ms );
    }
  }
  else
  {
    /* No strong pilots ... */

    if ( ( !hdrsrchidle.supervision_timer_active ) &&
         ( best_sect != NULL) )
    {
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Start PILOT_SUPERVISION_T: PN=%d E=%d T=%ds",
                      best_sect->pn_offset,
                      best_energy,
                      pilot_supervision_ms );

      (void) rex_set_timer( &hdrsrch.timeout_timer, pilot_supervision_ms );
      hdrsrchidle.supervision_timer_active = TRUE;
        /* Pray a pilot grows strong soon ...  */
    }
  }

} /* hdrsrchidle_pilot_supervision( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_GET_HO_CANDIDATE

DESCRIPTION
  This function finds the best CSET pilot, subject to it being stronger than
  the current ASET pilot. The threshold to find the best CSET pilot is fixed.
  Factors such as CSET sector being cached and how strong the ASET sector  
  pilot are not considered.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Strong CSET pilot or NULL

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL hdrsrch_sect_struct_type* hdrsrchidle_get_ho_candidate
(
  uint8                     ho_thresh
    /* threshold in 0.5dB units required to avoid ping-pong handoffs */
)
{
  hdrsrch_sect_struct_type  *sect;
    /* Strongest sector */

  int                       i;
    /* CSET loop index */

  uint16                    best_eng, min_thresh;
    /* Best energy so far and minimum threshold eng */

  boolean                   ho_candidate;
    /* Whether this is a HO candidate */

  boolean                   same_subnet_ho = FALSE;
    /* Whether this is the same-subnet handoff */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#ifdef FEATURE_HDR_REVB
  best_eng = hdrsrchset_scale_eng( 
   hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng, ho_thresh );
    /* Start with ASET energy. The sector should be at least x dB more
       than the active set to avoid ping-pong between equal energy candidate
       and active set sectors. */
#else
  best_eng = hdrsrchset_scale_eng( hdrsrchset.aset[0]->total_eng, ho_thresh );
    /* Start with ASET energy. The sector should be at least x dB more
       than the active set to avoid ping-pong between equal energy candidate
       and active set sectors. */
#endif /* FEATURE_HDR_REVB */

  min_thresh = best_eng;
    /* minimum threshold to perform handoff */

  sect = NULL;
    /* Active sector is the strongest sector ... so far */

  for(i=0; i<hdrsrchset.cset_cnt; i++)
  {
#ifdef FEATURE_HDR_REVB
    if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan,
           hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->chan ) )
#else
    if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan,
                             hdrsrchset.aset[0]->chan ) )
#endif /* FEATURE_HDR_REVB */
    {

      ho_candidate = hdrsrchutil_sect_is_ho_candidate( 
                          hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0],
                          hdrsrchset.cset[i],
                                            FALSE,  /* use filtered energy */ 
                          best_eng,
                          min_thresh,
                          &same_subnet_ho );

        /* Check whether it is a HO candidate */

      if ( ho_candidate == TRUE ) 
      {
        sect = hdrsrchset.cset[i];
          /* Found a stronger pilot */

        best_eng = sect->total_eng;
          /* Move the bar higher */

      }

    } /* if */
  
  }

  return sect;

} /* hdrsrchidle_get_ho_candidate( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_GET_IDLE_HO_CANDIDATE_DYNAMIC_THRESH

DESCRIPTION
  This function finds the best CSET pilot, subject to it being stronger than
  the current ASET pilot. If a sector is cached, it has higher priority than
  those noncached. The threshold to select this sector is lower than the 
  threshold of selecting a noncached sector. If the ASET sector has stronger
  power (greater than -7db), the threshold to select a CSET sector as handoff 
  candidate is higher.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  Strong CSET pilot or NULL

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL hdrsrch_sect_struct_type* hdrsrchidle_get_ho_candidate_dynamic_thresh
(
  void
)
{
  hdrsrch_sect_struct_type  *sect;
    /* Strongest sector */

  int                       i;
    /* CSET loop index */

  uint16                    best_eng = 0;
    /* Best energy so far */

  uint16                    min_thresh_cached;
    /* Minimum threshold eng for sector with cached overhead */

  uint16                    min_thresh_notcached;
    /* Minimum threshold eng for sector without cached overhead */

  boolean                   candidate_cached = FALSE;
    /* Flag whether the candidate is cached or not */

  boolean                   ho_candidate;
    /* Whether this is a HO candidate */

  boolean                   same_subnet_ho = FALSE;
    /* Whether this is the same-subnet handoff */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* calculate handoff threshold */
  hdrsrchutil_calc_dynamic_ho_thresholds( 
     hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->total_eng, 
     &min_thresh_cached, &min_thresh_notcached );

  sect = NULL;
    /* Active sector is the strongest sector ... so far */

  for( i = 0; i < hdrsrchset.cset_cnt; i++ )
  {
    if ( SYS_CHAN_CMP_EQUAL( hdrsrchset.cset[i]->chan,
                             hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->chan ) )
    {
      /* find out if this sector is a handoff candidate */
      ho_candidate = hdrsrchutil_sect_is_ho_candidate_dynamic_thresh(
                        hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0],
                        hdrsrchset.cset[i],
                        FALSE,  /* use filtered energy */ 
                        min_thresh_cached,
                        min_thresh_notcached,
                        best_eng,
                        sect,
                        &candidate_cached,
                        &same_subnet_ho
                     );

      if ( ho_candidate )
      {            
         sect = hdrsrchset.cset[i];
            /* Found a stronger pilot */

         best_eng = sect->total_eng;
            /* Move the bar higher */
      }

    } /* if */
  
  } /* for */

  return sect;

} /* hdrsrchidle_get_idle_ho_candidate_dynamic_thresh( ) */


/*===========================================================================

FUNCTION HDRSRCHIDLE_HANDOFF_LINK_REPORT

DESCRIPTION
  TBD

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_handoff_link_report
(
  hdrsrch_sect_struct_type  *sect
    /* sector to which handing off to */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchidle_link_report( sect, hdrsrchidle.handoff.report );
    /* Report idle handoff to given sector */

} /* hdrsrchidle_handoff_link_report( ) */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHIDLE_SCHED

DESCRIPTION
  This function programs searches, and schedules the next search request.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_sched( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Program the search only if the searcher is idle */
  if ( hdrsrchdrv.active_srch == SRCH_STATUS_INACTIVE )
  { 
    if ( hdrsrchrfdiv_diversity_demod_is_enabled( ) ) 
    {
      hdrsrchdrv.srch_task_type = HDRSRCH_DIVERSITY_SRCH;
    }
    else
    {
      hdrsrchdrv.srch_task_type = HDRSRCH_NON_DIV_SRCH;
    }

  /* Perform triage on every search, unless suppressed by SLEEP_CMD */
#ifdef FEATURE_FACTORY_TESTMODE
    if(ftm_get_mode() == FTM_MODE)
    {
      hdrsrchidle.perform_triage = FALSE;
    }
    else
    {
      hdrsrchidle.perform_triage = TRUE;
    }
#else
    hdrsrchidle.perform_triage = TRUE;
#endif

    (void) hdrsrchutil_program_search_set( );
      /* Perform the standard search scheduling */
  }

  (void) rex_set_timer( &hdrsrch.schedule_timer, HDRSRCH_IDLE_SEARCH_INTERVAL_MS );
    /* And schedule next search request */

} /* hdrsrchidle_sched( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_TIMEOUT

DESCRIPTION
  This function handles the Network Lost timer.
  If all pilots drop below PilotDrop, the TimeOut timer is started.
  If any pilot rises above PilotDrop before the timer expires, then
  the TimeOut timer is cleared.
  If the TimeOut timer expires, this function is called, and we must
  declare a Network Lost indication.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Transitions back to start state.

===========================================================================*/

void hdrsrchidle_timeout( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,"System Lost- T(RUPPilotSupervision) St=%x", 
                  hdrsrch_state_id );

  hdrsrchutil_system_lost( HDRSRCH_SYSLOST_RUP_PILOT_SUPERVISION );
    /* Let upper layer know the system has been lost, and deactivate */

} /* hdrsrchidle_timeout( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_DISABLE_HO_DURING_SCC

DESCRIPTION
  This function decides whether HO should be disabled, depending on the 
  current HSTR count.
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchidle_disable_ho_during_scc( void )
{
  int                             hstr;
    /* Half slot count - position within Synchronous Control Cycle */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hstr = HDRHSTR_GET_COUNT() % 512;

  if ( hstr >= HDRSRCHIDLE_NO_HANDOFF_HSTR )
  {
    /* HSTR count is about to wrap around to zero. */

    hstr -= 512;
      /* Wrap it around to less than zero now, to prevent an idle handoff
         just as the next SCC is about to start. */
  }

  if ( hstr < hdrsrchidle.handoff.last_hstr )
  {
    /* HSTR count has gone backwards - this is the first dump of a new CCC. */

    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "Disable HO (new CCC) hstr=%d [last=%d]",
                    hstr & 511,
                    hdrsrchidle.handoff.last_hstr );

    hdrsrchidle.handoff.allowed = FALSE;
      /* Disable handoffs until we're finished decoding */

      /* Here we assume CC decoding finishes AFTER this search dump. The 
         second case that CC decoding finishes just BEFORE this search dump
        will be handled separately when LMAC enables HO, by reseting
        "last_hstr" to the current HSTR value.                             */
  }

  hdrsrchidle.handoff.last_hstr = hstr;

} /* hdrsrchidle_disable_ho_during_scc */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_DUMP_PROC

DESCRIPTION
  This function handles the search dump signal. Different Idle states pass
  their own set management functions.  

DEPENDENCIES
  None

PARAMETERS
  Set management function pointer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_dump_proc
( 
  hdrsrchidle_dump_func_type dump_func 
    /* state dependent aset_management function as argument */
)
{
  int                             new_rpts;
    /* # of new pilot reports */

  boolean                    valid;
    /* Flag to indicate whether or not searcher results are valid */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  ASSERT( dump_func != NULL );
    /* Can not pass a NULL function pointer  */

  hdrsrchidle_disable_ho_during_scc( );

  /* Perform standard processing on the search results.
     (Ie, pilot strength filtering, set management, triage, etc) */

  valid = hdrsrchutil_validate_searcher_results();
    /* Perform searcher result validation before using the results */

  if ( valid )
  {
    new_rpts = hdrsrchutil_process_search_dump( );
      /* Perform initial processing, including C, N & R set processing */
      
    dump_func( new_rpts );
      /* Set management */

    /* Perform triage unless supressed due to a sleep command */
    if ( hdrsrchidle.perform_triage )
    {
      hdrsrchutil_process_search_triage( );
        /* Keep fingers on the search peaks */
    }

    hdrsrchidle_pilot_supervision( );
      /* Watch all pilot strengths */

    hdrsrchutil_update_win_center();
      /* Update the window center for each sector as per earliest multipath */
  }
  else
  {
    hdrsrchutil_invalidate_search_dump();
      /* invalidate searcher results */
  }

} /* hdrsrchide_dump_proc( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_HANDLE_ACCESS_HO

DESCRIPTION
  This function handles access handoff.

DEPENDENCIES
  None

PARAMETERS
  Pointer to a sector that we might handoff to.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchidle_handle_access_ho
(
  hdrsrch_sect_struct_type  *sect
    /* handoff sector */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrsrchidle.access.handoff != NULL )
  {
    /* We've been directed to perform an access handoff */

    if ( hdrsrchidle.access.handoff->use_new_pilot )
    {
      /* We've been told which pilot to hand off to. */

      sect = hdrsrchset_get_sect(
                hdrsrchidle.access.handoff->new_pilot.pilot_pn,
                hdrsrchidle.access.handoff->new_pilot.channel );
        /* Get a sector pointer for required pilot */
    }

    if ( sect != NULL )
    {
      /* Handoff to required pilot */

      hdrsrchutil_aset_idle_handoff( sect );

      INC_SAT( hdrsrchlog.sleep_info.handoff_count );
        /* Increment the idle hand-off count        */

      INC_SAT( hdrsrchlog.sleep_info.iho_count );
        /* Increment the TOTAL idle hand-off count  */
    }
    else
    {
      /* No good candidate pilot to handoff to.
         Report a "Handoff" to the current ASET pilot */
#ifdef FEATURE_HDR_REVB
      sect = hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[ 0 ];
#else
      sect = hdrsrchset.aset[ 0 ];
#endif /* FEATURE_HDR_REVB */
    }

    hdrsrchidle_link_report( sect, hdrsrchidle.access.handoff->access_ho_cb );
      /* Report access handoff performed to given sector */

    hdrsrchidle.access.handoff = NULL;
    hdrsrch_complete_cmd();
      /* Command is now complete - forget parameters, return command
         buffer to free queue, and re-enable command processing. */

    hdrsrchidle_request_access_handoff( NULL );
      /* No access handoff requests in progress */
  }
  else
  {
    if ( sect != NULL )
    {
      hdrsrchidle_request_access_handoff( sect );
        /* Request a handoff to this sector */
    }
  }/* else */

} /* hdrsrchidle_handle_access_ho( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_HANDLE_IDLE_HO

DESCRIPTION
  This function handles idle handoff in monitor state.

DEPENDENCIES
  None.

PARAMETERS
  Pointer to a sector that we might handoff to.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_monitor_handle_idle_ho
(
  hdrsrch_sect_struct_type      *sect
    /* hand-off candidate sector. NULL -> no handoff */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( sect )
  {
    if ( hdrsrchidle.handoff.allowed )
    {
      hdrsrchutil_aset_idle_handoff( sect );
        /* Handoff to new pilot */

      INC_SAT( hdrsrchlog.sleep_info.handoff_count );
        /* Increment the idle hand-off count        */

      INC_SAT( hdrsrchlog.sleep_info.iho_count );
        /* Increment the TOTAL idle hand-off count  */

      hdrsrchidle_handoff_link_report( sect );
        /* Report idle handoff to given sector */

    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "No handoff to %d during SCC decode",
                      sect->pn_offset );
    }
  } /* if ( sect ) */

} /* hdrsrchidle_monitor_handle_idle_ho( )  */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_MONITOR_ASET_MGMT

DESCRIPTION
  This function handles aset management in monitor state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchidle_monitor_aset_mgmt
(
  int new_rpts
    /* Number of new pilot report events (R/N to C, C>A, Weak A) */
)
{
  hdrsrch_sect_struct_type  *sect;
    /* Strongest sector */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  SYS_ARG_NOT_USED( new_rpts );
    /* mark the unused variable to avoid the lint errors */

  if (HDRSRCH_ENABLE_IDHO_DYNAMIC_THRESH == FALSE) 
  {
  sect = hdrsrchidle_get_ho_candidate( HDRSRCHIDLE_MONITOR_HO_THRESH_HALF_DB );
    /* Any CSET pilots stronger than the ASET? */
  }else
  {
     sect = hdrsrchidle_get_ho_candidate_dynamic_thresh();
       /* Any CSET pilots stronger than the ASET? */
  }

  hdrsrchidle_monitor_handle_idle_ho( sect );
    /* Evaluate Idle handoff possibility and perform one if necessary */

} /* hdrsrchidle_monitor_aset_mgmt( ) */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRSRCHIDLE_ACCESS_ASET_MGMT

DESCRIPTION
  This function handles aset management in access state

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

LOCAL void hdrsrchidle_access_aset_mgmt
(
  int new_rpts
    /* Number of new pilot report events (R/N to C, C>A, Weak A */
)
{
  hdrsrch_sect_struct_type  *sect;
    /* Strongest sector */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  SYS_ARG_NOT_USED( new_rpts );
    /* mark the unused variable to avoid the lint errors */

  if ( hdrsrchidle.access.handoff != NULL )
  {
    /* We've been directed to perform an access handoff */

    sect = hdrsrchidle_get_ho_candidate( 2 );
      /* Any CSET pilots at least 1dB stronger than the ASET */
  }
  else
  {
    sect = hdrsrchidle_get_ho_candidate( hdrsrchidle.access.thresh_0p5_db );
      /* Any CSET pilots N_dB stronger than the ASET? */
  }

  hdrsrchidle_handle_access_ho( sect );
    /* Evaluate Access handoff possibility and perform one if necessary */

} /* hdrsrchidle_access_aset_mgmt( ) */

#ifdef FEATURE_HDR_DYNAMIC_IDLE_DIV
/*===========================================================================

FUNCTION HDRSRCHIDLE_PROCESS_SCC_DECODE_IND

DESCRIPTION
  This function handles scc deocde indication from FW.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void hdrsrchidle_process_scc_decode_ind( void )
{
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Idle Div: Received, SCC DECODE IND frm FW");

  if ( ( hdrsrch_state_id == HDRSRCH_IDLE_STATE ) &&
       ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_MONITOR_STATE ) &&
       ( hdrsrch.idle_state_div.idle_div_enabled == TRUE ))
  {   
     if( !hdrsrchrfdiv_diversity_is_enabled() )
     {
       HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Idle Div: Turning on div, SCC DECODE IND frm FW");

       hdrsrchrfdiv_diversity_ctrl( TRUE );
        /* Enable diversity in access state. */
     }
     else
     {
       HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Idle Div: Not turning on div as it is already on");
     }  
  }

} /* hdrsrchidle_process_scc_decode_ind( ) */

/*===========================================================================

FUNCTION HDRSRCHIDLE_GET_PREV_WAKEUP_PSINR   

DESCRIPTION 
  This function calculates the psinr avg for the entire wakeup.
 
 DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

int16 hdrsrchidle_get_prev_wakeup_psinr ( void )
{
  int32 psinr_total = 0;
  /* Temp var to hold total psinr for all slots in this wakeup */

  int16 psinr_in_db;
  /* Temp var to hold psinr in db */

  int16 psinr_avg;
  /* Temp var to hold avg */

  int i;
  /* Temp loop variable */
  
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  for( i = 0; i < HDRSRCH_MAX_SLOTS_PSINR_PER_IDLE_WAKEUP; i++ )
  {
     psinr_total += hdrsrch.idle_state_div.wakeup_info.psinr_per_slot[i];

  }
  if( psinr_total == 0 ||
      hdrsrch.idle_state_div.wakeup_info.num_slots == 0 )
  {
    return ( HDRSRCHUTIL_IDLE_STATE_DIV_PSINR_DEFAULT_VALUE );
  }

  psinr_avg = ( psinr_total/hdrsrch.idle_state_div.wakeup_info.num_slots );

  psinr_in_db = hdrsrchset_sinr_linear_to_db( psinr_avg );

  return ( psinr_in_db );

  
} /* hdrsrchidle_get_prev_wakeup_psinr */

/*===========================================================================

FUNCTION HDRSRCHIDLE_PROCESS_PSINR_MDSP_LOGGING_EVENT   

DESCRIPTION 
  This function is the call back from mdsp logging event for PSINR 
  calculation.
 
 DEPENDENCIES
  None

RETURN VALUE 
  None 

SIDE EFFECTS 
  None 

===========================================================================*/

void hdrsrchidle_process_psinr_mdsp_logging_event
( 
  hdrlog_mdsp_slot_logging_data_type *sl_params 
)
{
  
  uint8 c_idx;
    /* Carrier Index */

  uint8 slot_idx;
    /* Slot index used as loop counter */

  int16 psinr_this_slot;
  /* temp var to hold current slot psinr */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 

  c_idx = 0;
  /* Idle state shud be only one carrier */

  /* For each slot logged by MDSP */
  for ( slot_idx = 0; 
        slot_idx < HDRFW_LOG_SLOTS; ++slot_idx )
  {
    psinr_this_slot = 
      HDRLOG_GET_PER_SLOT_PRX_SINR( sl_params->rx_log_buff_idx, c_idx, slot_idx );
    
    if( psinr_this_slot != 0 )
    {
	  HDR_ASSERT (hdrsrch.idle_state_div.wakeup_info.slot_index < HDRSRCH_MAX_SLOTS_PSINR_PER_IDLE_WAKEUP);
      hdrsrch.idle_state_div.wakeup_info.psinr_per_slot[hdrsrch.idle_state_div.wakeup_info.slot_index]
          = psinr_this_slot;
        /* Read PSINR for this slot */ 
  
      /* We have recorded less than 255 slots of psinr then we will cap at 255
      and start writing the buffer again.*/
      if( hdrsrch.idle_state_div.wakeup_info.slot_index  
          < HDRSRCH_MAX_SLOTS_PSINR_PER_IDLE_WAKEUP-1 )
      {
         hdrsrch.idle_state_div.wakeup_info.slot_index++;
      }
      else
      {
         hdrsrch.idle_state_div.wakeup_info.slot_index = 0;
      }

      if( hdrsrch.idle_state_div.wakeup_info.num_slots <
             HDRSRCH_MAX_SLOTS_PSINR_PER_IDLE_WAKEUP)
      {
         hdrsrch.idle_state_div.wakeup_info.num_slots++;
      }
        
    }
  }
    
} /* hdrsrchidle_process_psinr_mdsp_logging_event */
#endif /* FEATURE_HDR_DYNAMIC_IDLE_DIV */

/*===========================================================================

FUNCTION HDRSRCHIDLE_IS_PRI_TA_ENABLED

DESCRIPTION
  This function returns whether tune away is enabled
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE: tune away is not disabled
  FALSE: tune aaway is disabled

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchidle_is_pri_ta_enabled( void )
{

  return ( hdrsrchidle.access.tune_away.state != HDRSRCHIDLE_TUNE_AWAY_DISABLED );
} /* hdrsrchidle_is_pri_ta_enabled */


/*===========================================================================

FUNCTION HDRSRCHIDLE_IS_IN_ACCESS

DESCRIPTION
  This function returns whether we are in access substate
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE: in access
  FALSE: not in access, we are in monitor substate

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchidle_is_in_access( void )
{

  return ( hdrsrchidle.monitor_substate == HDRSRCHIDLE_ACCESS_STATE );
} /* hdrsrchidle_is_pri_ta_enabled */


/*===========================================================================

FUNCTION HDRSRCHIDLE_IS_TA_STARTED

DESCRIPTION
  This function returns tune away status in Idle. This will be helpful in 
  stopping Fast connect TCA processing while tuning away in Idle.

DEPENDENCIES 
  None. 

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if tune away processing has started.
  FALSE - otherwise

SIDE EFFECTS
  None.

===========================================================================*/

boolean hdrsrchidle_is_ta_started( void )
{

  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */
  
  timetick_type                  sclks_left;
    /* Sclks remaining until lock release */
  
  timetick_type                  sclks_now;
    /* current sclks  count */

  timetick_type                  end_time_sclks = HDRSRCHRF_MAX_DURATION;
    /* Sclk timestamp for when we should tune away. */  

  boolean                        no_tune_away;
    /* Do we know when we are supposed to tune away yet? */
    
  unsigned                       ms_left;
    /* maximum duration for which we can have the RF */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
    /* Retrieve the retain lock state. */

  if ( rl_state.event == HDRSRCHRF_UNLOCK_CANCELLED )
  {
    return FALSE;
  }
  else if (rl_state.event == HDRSRCHRF_UNLOCK_IMMEDIATELY )
  {
    return TRUE;
  }
  else
  {
    /* For Unlock By event */
    
    no_tune_away = FALSE;

    sclks_now = timetick_get();
      /* Current time in sclks. */ 
      
    sclks_left = (timetick_type) hdrsrchutil_calc_time_to_tune_away( 
                                      (uint32) sclks_now,
                                      (int32) (HDRSRCHIDLE_EXTRA_OVERHEAD_SCLK),
                                      &rl_state,
                                      &no_tune_away,
                                      &end_time_sclks,
                                      NULL);
      /* How much more time can we have RF chain? */

    ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
        /* Convert lock extension to milliseconds. */

    if (ms_left < HDRSRCHIDLE_EXTRA_OVERHEAD_MS)
    {
      return TRUE;
    }
    else 
    {
      return FALSE;
    }
  }

} /* hdrsrchidle_is_ta_started */

/*===========================================================================

FUNCTION HDRSRCHIDLE_IS_TX_PENDING

DESCRIPTION
  This function returns whether we are in tx pending state i.e
  either access of TCA is pending while we wait for RXTX_BEST
  
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE: yes access or TCA pending
  FALSE: no access or TCA pending

SIDE EFFECTS
  None

===========================================================================*/

boolean hdrsrchidle_is_tx_pending( void )
{
  return hdrsrchidle.is_tx_pending;
  
} /* hdrsrchidle_is_tx_pending */


/*==========================================================================

FUNCTION        HDRSRCHIDLE_GET_ACCESS_TA_TIME

DESCRIPTION
  This function return the primary chain tune away time (in sclk) in access state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  Primary chain tune away time (end_time_sclks)

SIDE EFFECTS
  None

==========================================================================*/
uint32 hdrsrchidle_get_access_ta_time( void )
{
  return hdrsrchidle.access.tune_away.end_time_sclks;
}/* hdrsrchidle_get_access_ta_time */

