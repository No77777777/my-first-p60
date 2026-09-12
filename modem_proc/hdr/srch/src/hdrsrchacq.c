/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                H D R    S R C H    A C Q U I S I T I O N

                           Search Acq Module

GENERAL DESCRIPTION
  This module provides initial acquisition procedures.

EXTERNALIZED FUNCTIONS
  None

REGIONAL FUNCTIONS
  hdrsrchacq_acquire

INITIALIZATION AND SEQUENCING REQUIREMENTS
  hdrsrchacq_acquire( ) - causes acquisition to begin.

  Copyright (c) 2000-2018 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchacq.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     -------------------------------------------------------
10/22/2018   vke     Deassign all fingers in FW if slam fails 
09/11/2018   vke     Reset all fingers to clear stray fingers if any
09/11/2018   vaa     Starting a timer on receiving RTT req from LTE
06/26/2018   vke     Changes to process slam response status from fw
03/27/2018   vke     Increase the R&N duration to 200ms
05/27/2016   vko     QSH framework for events capturing
05/16/2016   vko     Reduce excessive F3
02/23/2016   rmv     Restore the Acq priority back to normal upon Acq failure 
02/15/2016   rmv     In Redir TA, deassign fingers before DEC deactivate
02/15/2016   vko     Changes to ignore SLAM rsp when TA already started
12/15/2015   vke     Klockworks fix
12/07/2015   rmv     Changes to reset no_tune_away flag for IRAT REDIR_ACQ case
11/09/2015   vke     Changes to disable TA while slamming
10/27/2015   wsh     Trigger sys loss if tune fails
10/02/2015   vke     Changes to revert CR876655
09/25/2015   vke     Changes to Re-Initialize stage2 to HDRSRCHACQ_S2_IDLE 
                     on getting the RF grant in Acquisition
08/10/2015   mbs/vke Supported ACQ/ACQ_INV mechanism to be 
                     compliant with TRMU
09/21/2015   vke     Changes to reset no_tuneaway flag for IRAT cases
09/01/2015   wsh     Fixed TA blocking logic for redirection ACQ
07/13/2015   arm     Mainlined FEATURE_RF_SVDO_API feature.
07/08/2015   rmv     Check and delay release div if div search is active 
06/29/2015   rmv     In L2D redirection, in case of acquisition failure, disable 
                     tune away before exchanging chain
06/26/2015   vke     Added tune_to_chan() timeout value
06/04/2015   wsh     Fixed FreqID mismatch 
05/22/2015   wsh     Fixed crash in FTM call
05/11/2015   wsh     Added support for RxTx Split and TRM grouped API
03/10/2015   sat     Updating dual antenna params in Anti Alias search
03/10/2015   sat     Resetting Tuneaway Count when deactivating
03/10/2015   sat     Changes to not release Diversity when Max TA reached.
10/13/2013   arm     Updated hdrsrchutil_calc_time_to_tune_away to return if
                     TA time was updated or not
09/29/2014   sat     Disabling TA for Unlock by in Redirection Acqusition.
09/22/2014   arm     DR-DSDS code cleanup
09/10/2014   bb      Changes to fix race condition between clearing TA timer and
                     processing primary chain grant signal
09/02/2014   mbs     Supported tune away on failure to extend lock during redir
08/15/2014   mbs     Supported fall-back to TRM R&N when trm_exchange fails
08/07/2014   mbs     Added support for trm lock exchange during IRAT procedures
07/29/2014   tnp     FR22375: Fix Medium compiler warnings in HDR
06/30/2014   arm     Changes to support DR-DSDS.
06/09/2014   ljl     Incremented predetect bin after coset group was searched.
02/25/2014   vke     Fix to pass correct TA overhead value to calculate 
                     1x conflict
10/31/2013   smd     Fixed bugs for next predect search in case of slam time
                     out.
04/09/2014   vke     Fix to prevent HDR to report system lost in 
                     Acq/Sync state because of GSM re-select
05/12/2014   vke     Klockworks fix
04/29/2014   sat     Changes to avoid next predetect in brute force acquisition
09/20/2013   arm     Make sure entire coset group is searched before detect.
05/02/2014   vlc     Use shallow search always for brute force acq. 
04/24/2014   vke     Fix to prevent HDR to stuck in Acqusition after  
                     processing unlock required during tune
01/28/2014   vke     Fix to start next pre-detect after processing
                     unlock cancel 
01/22/2014   vke     Fix for limiting the acquisition failure due to tune-away
02/27/2014   bb      Changes to avoid enabling diversity if there is contention
                     for primary chain
01/17/2014   vke     Fix to process TA notification after extend duration
11/29/2013   sat     NV control for Lock Release Enhancement (LRE)
01/21/2014   mbs     Support small SRCH window size in BOLT
10/07/2013   vko     Replaced ASSERTs on external/global variables as per 
                     go/assert guidelines 
11/12/2013   mbs     Protected VSTMR registrations
09/25/2013   wsh     Changes to support BOLT bring up
09/04/2013   wsh     Changes to support BOLT bring up
08/09/2013   dsp     Reset RTC before enabling FW when we go to active state.
08/09/2013   vke     On logging the Acq information when Acq deactivate
                     check whether the logging reason is valid
08/08/2013   vke     Added handling of sync substate callback
07/15/2013   vke     Added system lost reporting when RF chain is released
06/27/2013   vke     Removed the disable tuneaway in handling cancel unlock
06/07/2013   vke     Removed trm_extend_duration in the begining of 
                     predetect to allow tuneaway in acquisition
05/10/2013   rkc     Added tuneaway for Lock Release Enhancement (LRE). 
04/22/2013   arm     Added changes for DSDA coexistence manager.
02/11/2013   vko     Fixed compiler warnings
01/23/2013   wsh     Initialize DIV/ARD on timesync
01/18/2012   smd     Added RUMI Tx support.
01/10/2013   vlc     Set search mode accordingly before sending search list. 
10/19/2012   smd     Told fw whether the standby to active transition is for wakeup.
09/25/2012   rmg     Added support for including freq info in TRM interaction.
09/05/2012   ljl     Removed PLL hack. 
08/12/2012   ljl     Config for CCS bootup.
06/14/2012   rkc     Only turned on MCPM wakeup clocks when granted TRM chain. 
05/25/2012   wsh     Moved ARD logic into it's own state machine 
05/22/2012   ljl     Reset coset idx to avoid exceeding the maximum.
05/21/2012   ljl     Disabled acq div on Dime RUMI.
04/24/2012   ljl     Supported XO fine factory cal removal.
04/12/2012   ljl     Disabled div before checking whether to delay RF chain 0 release.
03/16/2012   arm     Resolve TCB errors for Dime.  
03/07/2012   smd     Added a F3 for acq diverstity thresh1.
02/29/2012   smd     Used general timer to postpone releasing RF.
04/27/2012   wsh     Fixed using wrong band info after IDLE OFS/ACQ
02/14/2012   wsh     Added Klockwork warning fix back in 
02/06/2011   ljl/smd Cleared HDRSRCH_PRI_CHAIN_EVENT_SIG before requesting TRM. 
02/03/2012   ljl     Checked RF cal before acquisition.
02/01/2012   ljl     Deactivate decoder before disabling RF.
01/11/2012   ljl     Added the parameter for search priority to hdrsrchdrv_enable_srch4().
01/09/2012   smd     Supported dynamic acquisition diversity. 
12/16/2011   ljl     Do not increment coset group when tuning RF.. 
12/05/2011   ljl     Delayed RF tune and disable for 1x paging.
11/07/2011   vlc     Fixed Klocwork warnings. 
10/17/2011   rmg     Removed calls to obsolete clk regim APIs.
09/12/2011   smd     Started afc open loop before brute-force acq. 
08/01/2011   ljl     Kept iteration and depth when RF was granted. 
07/15/2011   ljl     Reset coset_group if all four groups had been searched. 
09/12/2011   smd     Started afc open loop before brute-force acq.
05/02/2011   kss     Fixed compiler warning.
04/27/2011   kss     Added RF include to remove warnings. 
04/25/2011   ljl     Cleared HDRSRCH_PRI_CHAIN_EVENT_SIG before requesting TRM.
04/25/2011   ljl     Cleared no dump cnt when starting acquisition.
04/12/2011   ljl     Fixed brute acq in XO mode.
04/15/2011   smd     Fixed a bug of using srch4 search structure. 
04/12/2011   smd     Used srch4 search structure. 
03/24/2011   kss     Updated RF SVDO API support.
03/03/2011   arm     Fixed NPA crash due to overlapping NPA calls.  
03/17/2011   ljl     Fixed compiler error. 
03/10/2011   ljl     Supported XO acquisition range improvement.
03/02/2011   ljl     Set stage1 state to idle when predetect search was aborted. 
01/12/2011   smd     Added F3 message for searched coset in brute-force acq. 
12/01/2010   ljl     Supported XO on 9k.
10/11/2010   smd     Continued acquisition if finding a stronger coset in 
                     anti-alias search of brute-force acq. 
10/01/2010   smd     Fixed the bug in brute-force acq predect search.
09/14/2010   smd     Added timeout signal in wait mask for slam_rsp timeout  
                     in brute-force acq. 
08/16/2010   lyl     Supported FW power management with suspend/resume.
08/06/2010   arm     DCVS changes for MDM9k 
07/14/2010   smd     Used octant search with 4k win size for brute-force acq. 
07/13/2010   cnx     Fixed Klocwork warnings.
06/29/2010   vlc     Fixed compiler warnings. 
06/25/2010   smd     Supported hstr slam in slam cfg message.
06/22/2010   ljl     Used 4k chips window size for pre-detect search.
06/15/2010   ljl     Supported RF MSGR interface.
05/20/2010   smd     Fixed bugs in Reverse Time Transfer.
05/19/2010   ljl     Corrected the feature for searcher fw workaround.
05/12/2010   ljl     Disabled rx before hdr fw standby.
04/26/2010   lyl     Added brute force acquisition support.
04/30/2010   ljl     Set burst len to 1 for Voyager 2.0 bringup.
04/15/2010   smd     Adapted to new params in hdrsrchdrv_start_slam().
03/24/2010   pxu     Removed FEATURE_7800_HDEM_WORKAROUnD features.
02/11/2010   ljl     Supported the fw timing state.
02/12/2010   ljl     Configured div settings after div stm was started.
01/06/2010   rkc     Removed HDRSRCHUTIL_NPA_CPU_ACTIVATED npa request.
12/16/2009   ljl     Configured CDMA clocks before HDR App enable.
12/09/2009   rkc     Mainlined FEATURE_HDR_ACQ_PULLIN_ENH.
12/02/2009   rkc     Fixed coset group indexing.
11/11/2009   rkc     Fixed #elif/define error.
11/11/2009   rkc     Added NPA support.
11/09/2009   ljl     Checked NULL pointer before calling acq_cb().
10/20/2009   ljl     Checked whetehr acq_cb() was NULL or not.
09/24/2009   rkc     Moved PN roll install after mdsp app is enabled.
                     Temporarily removed EAFT support
09/14/2009   ljl     Supported srch MSGR interface.
09/09/2009   wsh     CMI: to support changes in clk and ts APIs
07/14/2009   ljl     Supported EAFT.
06/30/2009   lyl     Updated for generic responses process.
                     Updated for rxAGC unlocked mode interface.
06/23/2009   lyl     Fixed the lint errors.
06/19/2009   grl     Switched to the hdrsrchrfdiv API.
05/18/2009   ljl     Updated for firmware MSGR interface.
04/21/2009   etv     Replaced obsolete hdrrx APIs with hdrdec APIs.
04/06/2009   ljl     Installed/uninstalled PN roll isr when RF chain was
                     granted/released.
03/18/2009   rkc     Added RF SVDO API changes.
11/20/2008   kss     Removed/replaced intlocks.
10/07/2008   lyl     Fixed lint errors.
05/09/2008   rkc     Fixed compiler warnings.
04/28/2008   ljl     Added hdrsrchrf_diversity_init().
04/17/2008   rkc     Fixed compiler warning.
04/07/2008   ljl     Checked whether array index out of boundary.
01/30/2007   jyw     Included the demod assignment list in the RF interface.
12/03/2007   etv     Added support for MC Searcher log packet.
11/20/2007   jyw     Removed the sleep.h from the header file list.
10/17/2007   grl     Merged Rev B changes from branch.
09/25/2007   rkc     Change HDRSRCHACQ_MAX_PREDETECT_PEAKS to 16 for XO only.
08/27/2007   rkc     Fixed bin.idx and freq offset when pullin fails.
08/25/2007   grl     Added array boundary check.
08/21/2007   rkc     Rename union name to "payload".
08/19/2007   grl     Resolved Lint High warnings.
08/09/2007   rkc     Printed depth when peak(s) found in detect dump.
08/01/2007   grl     Fixed memory write out of array bounds.
07/31/2007   grl     Changed hdrsrchacq_slam_poll to be static
07/26/2007   rkc     Reset bin index when detect fails
07/23/2007   rkc     Use FEATURE_HDR_SRCH_ACQ_PULLIN_ENH: pullin enhancements
07/20/2007   rkc     In hdrsrchacq_detect_dump(), log current and best eng
06/20/2007   rkc     Handled new xo_cal_mode payload to set XO binning mode
06/18/2007   rkc     Use interpolated peak pos in pullin
06/18/2007   rkc     Added support for FEATURE_XO
06/12/2007   grl     Register the HDR activated clk client.
04/10/2006   mt      Changes for 1x pilot search support.
04/02/2007   grl     Changed sleep voting to entry and exit of st state.
03/20/2007   grl     Deregister for high clock when waiting for the rf lock.
02/02/2007   ljl     Deactivated AFC if stage2 failed.
09/21/2006   ljl     Added HDR SRCH AFC stm.
08/28/2006   grl     Removed OK to sleep vote which should not be there.
08/21/2006   grl     Interface change to hdrsrchrf_set_agc_acq_params
08/08/2006   grl     Disabled the MDSP when releasing the RF lock.
08/01/2006   mt      Changes for HDR AGC acquisition. 
06/14/2006   sq      Added FEATURE_HDR_AGC_OPTIMIZATION_P1
04/17/2006   ljl     Updated HDR afc with channel info when RF is granted.
04/17/2006   ljl     Restored VCO if VCO pullin failed.
10/24/2005   jyw     Fixed the lint errors.
09/06/2005   ljl     Changed acquisition peak threshold to -10dB
09/06/2005   ljl     Added optimization after system loss
09/06/2005   ds      Added support for Rotator Based Acq and Track
08/11/2005   grl     Mainlined FEATURE_HDR_TCXOMGR.
07/27/2005   grl     Removed extra trk_lo write and added debug message.
05/31/2005   grl     Added support for TCXOMgr and TRM.
03/31/2005   grl     Fixed HDR's manipulation of the trk_lo to be GPS friendly
03/23/2005   hrk     Replaced clk_busy_wait by hdrsrch_timed_wait_usecs
03/11/2005   hrk     Replaced hdrdec_deactivate by hdrsrchutil_dec_deactivate 
11/03/2004   mpa     Removed dependency on std.h (merged with sys.h)
09/07/2004   sq      Do not request partial results when aborting searches
08/23/2004   ds      Update HDR debug display record after RF tuning
07/31/2004   sq      Reset no_dump_cnt when receiving micro acq command
07/22/2004   ajn     Added common signals to state machine
06/30/2004   dna     Support for inactive state
06/24/2004   sq      Bug fixes in ACQ info log packet: search state, acq time
06/21/2004   sq      Changed tune_timer to use SCLK timer
06/08/2004   sq      Bug fix for ACQ INFO LOG packet
06/01/2004   sq      Replaced srch4_enable with new API
05/12/2004   sq      Mainlined FEATURE_HDR_TC_DIVERSITY_ONLY
01/30/2004   sq      Put RF to sleep unconditionally
01/13/2004   sq      Restored finger merge protect code
12/03/2003   aaj     Perform acquisition init after getting RF lock
12/02/2003   sq      Merged in code review comment
11/26/2003   aaj     Disable Srch4 when releasing RF. Enable when HDR gets RF.
11/21/2003   sq      Supported 3 new log packets( acq,reacq,sleep )
11/18/2003   aaj     Added system usability callback. Enabled merge protect
11/04/2003   bt      Replaced srch4_hdr_en/disable with srch4_enable iface.
10/30/2003   aaj     Removed unnecessary rfm_enter_mode from acq state
10/28/2003   aaj     Support enable/disable of hdrmdsp and srch4 
10/14/2003   aaj     Use a common unified #define for minimum path energy
10/06/2003   aaj     Mainlined T_MSM6500 and HDRSRCH_MULTIPATH_WIN_CENTER
10/03/2003   ajn     Removed unnecessary dog_autokick (done by sleep task)
09/23/2003   ajn     Removed 1x finger disable code (moved to srch.c)
09/18/2003   aaj     Restored rf_hdr_sleep change for hybrid mode integration
09/18/2003   aaj     Temporarily back out "rf_hdr_sleep( )->hdrsrchrf_sleep( )"
09/18/2003   aaj     Temporarily back out "rf_hdr_sleep( )->hdrsrchrf_sleep( )"
09/10/2003   ajn     rf_hdr_sleep( ) -> hdrsrchrf_sleep( )
09/05/2003   sq      Removed temporary code for HDR_pllEnable and HDRFLL_Enable
07/31/2003   dyc     Added support for FEATURE_MULTI_RX_CHAIN
07/20/2003   sq      Ported the searcher interface for T_MSM6500
03/11/2003   ajn     Updated Copyright notice
02/26/2003   aaj     Mark the mps threshold change as pending after dsp dload
01/30/2003   sq      Disable diversity when we enter acquisition
11/13/2002   aaj     Added support for acquisition modes
11/07/2002   ajn     enable_roll( ) utility replaces register roll callback
08/12/2002   aaj     Reset Enc/Mod during acq init. Redownload DSP
06/10/2002   aaj     Removed debug code under FEATURE_RESET_ON_SAM_STALL
05/24/2002   aaj     Added FEATURE_RESET_ON_SAM_STALL
05/23/2002   aaj     Clear LOCK_GRANTED_SIG when exiting acq state
04/29/2002   aaj     Add a 1 second timeout for MSTR slam polling
04/25/2002   aaj     Acquire only after previous pending search is complete
04/25/2002   aaj     Moved DSP inits to hdrsrchdrv_startup_proc function
03/01/2002   ajn     Reversed order of RF Unlock and Acq Failure report
03/04/2002   kss     Added hdrdec/hdrmdsp activate/deactivate.
03/04/2002   aaj     Set diversity combining mode during acq init
02/20/2002   ajn     Added DUMP flag to abort search.
02/19/2002   sst     Enabled parallel Predetect searches with FLL
02/16/2002   aaj     Disable flush decoder buffer during acq init. Revisit this
02/06/2002   ajn     hdrerrno -> errno
01/18/2002   aaj     Reset rx packet seq number during acq init.
01/17/2002   aaj     Changed sector struct to contain band+chan number
12/31/2001   ajn     Changed lock notify to use GP0 signal.
                     Added a virtual acquisition timer (RF owned by HDR).
                     Reduced acquisition lock required times.
12/07/2001   aaj     Change non-coherent length to 1 for diversity
11/26/2001   aaj     Support for RF cold start. srch status logging removal
11/16/2001   aaj     Disable PN ROM bug fix
10/24/2001   ajn     Change state header files to common hdrsrchstates.h
                     Command parameters passed by "retry_cmd()".
10/15/2001   aaj     Enable default diversity from s/w side.
10/05/2001   aaj     Fixed RTC offset problem for anti-alias searches
09/27/2001   aaj     Added temporary fix for bestAspFL = 2
09/15/2001   aaj     Added registration for PN roll callback
09/05/2001   aaj     Antenna Diversity integrated in the idle state
08/28/2001   aaj     Support for acquisition duration, corrected msg levels
08/09/2001   ajn     Output SLAM message before calling ACQ callback.
08/02/2001   aaj     Enable/Disable PN ROM Fix. Diversity Support changes.
07/17/2001   ks/aaj  Support for Cellular Band operation
06/20/2001   aaj     HDR Shutdown/Startup/Resume/Suspend support added
06/19/2001   aaj     Replaced directly MDSP_READs/WRITEs with macros
06/11/2001   aaj     Fixed a MSG_HIGH anamoly slam function
06/07/2001   ajn     Completed sleep signals (support in sleep.h)
06/06/2001   ajn     Roughed in sleep signals, set mstr to zero.
05/27/2001   aaj     No parallel predetect searching and frequency pullin
04/30/2001   aaj     Fixed sorting of predetect energies
04/11/2001   ajn     Adjusted messaging.
04/02/2001   ajn     Corrected coset acquisition order
03/08/2001   ajn     Reworked acquisition - parallel search & pulling.
03/02/2001   aaj     included customer.h file
03/02/2001   aaj     External control for antenna diversity
02/12/2001   ajn     Added system arbitration support
02/08/2001   ajn     Declare acquisition failure if both stages 1 & 2 fail.
02/06/2001   aaj     functions name changes to conform with conventions
02/02/2001   aaj     Replaced SRCH_SELECT_HACK with global HDR_SRCH_SELECT
12/18/2000   aaj     Changed HDR_SRCH_SELECT to normal search single antenna
10/31/2000   ajn     Added General Purpose Signals to State Machine
10/05/2000   ajn     Used hdrsrchutil functions to clean up code
09/26/2000   aaj     Added DSP Firmware Reset to ACQ init
09/25/2000   aaj     Code cleanup
09/11/2000   aaj     Added acquisition debug support. Added distx2() function
07/07/2000   ajn     Merged Acquisition drivers to main acq state/code cleanup
06/27/2000   aaj     Compiled the code successfully
06/23/2000   aaj     Added bunch of comments after design/code review
06/21/2000   aaj     Completed acquisition state function
06/19/2000   aaj     Implemented temporary verify state proc code
06/16/2000   aaj     Completed coding of detect state result processing
06/15/2000   aaj     Created this file
==========================================================================*/



/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "customer.h"
#include "time_svc.h"
#include "hdrsrch.h"

#include "hdrsrchstate.h"
#include "hdrsrchstates.h"
#include "hdrsrchi.h"
#include "hdrsrchtask.h"
#include "hdrsrchutil.h"
#include "hdrsrchdrv.h"
#include "hdrsrchfing.h"
#include "hdrmdspmcr.h"
#include "hdrmdsp.h"
#include "hdrsrchdef.h"
#include "hdrsrchrf.h"
#include "hdrsrchrfi.h"
#include "hdrsrchrfdiv.h"
#include "tcxomgr.h"
#include "hdrsrchafc.h"
#include "hdrts.h"
#include "hdrsrchlog.h"
#include "hdrsrchmsg.h"
#include "hdrsrchsync.h"

#include "hdrrx.h"
#include "hdrdec.h"
#include "hdrenc.h"

#include "timetick.h"

#include "hdrdebug.h"

#include "srch_hw.h"
#include "srch_hdr.h"

#ifdef FEATURE_MULTI_RX_CHAIN
#include "rfm_common.h"
#include "rfm_cdma.h"
#endif

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
#include "hdrsrchard.h"
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */

#include "hdrsrchmcpm.h"

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
#include "hdrmultirat.h"
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#include "ftm.h"
#include "hdr_log_qsh.h"
#include "hdrutil.h"

/*==========================================================================

                     DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, typesdefs,
and other items needed by this module.

==========================================================================*/

/* EJECT */
/*==========================================================================

                     STATE DISPATCH TABLE

This section provides the dispatch functions for events that are received
by the task while in this state.

==========================================================================*/

#define HDRSRCHACQ_S2_POLL_TIMER    &hdrsrch.gp_timer0
#define HDRSRCHACQ_S2_POLL_SIG      HDRSRCH_GP_TMR0_SIG

#define HDRSRCHACQ_BAND_GRANT_TIMEOUT_MS           (50)

LOCAL void            hdrsrchacq_init( hdrsrch_state_enum_type last );
LOCAL void            hdrsrchacq_done( hdrsrch_state_enum_type next );
LOCAL errno_enum_type hdrsrchacq_cmd ( const hdrsrch_cmd_type * cmd );
LOCAL void            hdrsrchacq_dump( void );
LOCAL void            hdrsrchacq_tune(hdrsrchrf_tune_status_enum_type status);
LOCAL void            hdrsrchacq_slam_timeout( void );
LOCAL void            hdrsrchacq_gp_time1( void );
LOCAL void            hdrsrchacq_pri_chain_event( void );
LOCAL void            hdrsrchacq_init_on_rf_lock( void );
LOCAL void            hdrsrchacq_process_int_cmd
                      (
                        hdrsrchsm_cmd_payload_type *
                      );
LOCAL void            hdrsrchacq_process_low_pri_rsp_ind
                      (
                        hdrsrchmsg_s_type         *msg
                      );
LOCAL void            hdrsrchacq_yield_rf( void );
LOCAL void            hdrsrchacq_lock_rf( void );
LOCAL void            hdrsrchacq_start_inv_timer( void );
LOCAL void            hdrsrchacq_priority_inv_timeout( void );

LOCAL hdrsrch_state_struct_type hdrsrchacq_state =
{
  HDRSRCH_ACQ_STATE,
  hdrsrchacq_init,
  hdrsrchacq_done,
  hdrsrchacq_cmd,
  hdrsrchacq_dump,
  hdrsrch_default_roll,
  hdrsrch_default_wake,
  hdrsrch_default_sched,
  hdrsrch_default_lost_dump,
  hdrsrchacq_priority_inv_timeout,
  hdrsrchacq_slam_timeout,
  hdrsrchacq_gp_time1,
  hdrsrch_default_gp_sig0,
  hdrsrch_default_gp_sig1,
  hdrsrch_default_time_valid,
  hdrsrch_default_rxchipx16,
  hdrsrchacq_pri_chain_event,
  hdrsrchacq_process_int_cmd,
  hdrsrchacq_process_low_pri_rsp_ind
};


/* EJECT */
/*==========================================================================

                ACQUISITION DEPTHS and STAGES STATES

==========================================================================*/

/*---------------------------------------------------------------------
   Acquisition Search Depths
---------------------------------------------------------------------*/

typedef enum
{
  HDRSRCHACQ_DEEP,
    /* Deep acquisition search, but only good for 1-2 ppm freq error */

  HDRSRCHACQ_SHALLOW,
    /* Shallow search, but better for larger frequency errors */

  HDRSRCHACQ_NUM_DEPTHS         /* how many depths do we have ? */
}
hdrsrchacq_depth_enum_type;


/*---------------------------------------------------------------------
   Acquisition Times (per coset group)
      Deep acquisition is slower than shallow.
---------------------------------------------------------------------*/

LOCAL const uint2 hdrsrchacq_depth_time[ HDRSRCHACQ_NUM_DEPTHS ] =
{
  410, /* Deep:    200ms predetect + 10ms antialias + 200ms pullin */
  355  /* Shallow: 145ms predetect + 10ms antialias + 200ms pullin */
};

/*---------------------------------------------------------------------
   Acquisition Depth Dependent Parameters
---------------------------------------------------------------------*/

#ifdef FEATURE_XO

#define HDRSRCHACQ_LOW_TEMP_THRESH  ((int32) -40)
#define HDRSRCHACQ_HIGH_TEMP_THRESH 60

#define HDRSRCHACQ_MAX_ITERATIONS_FOR_DEBUG 3

/* For XO, all searches are shallow, even if deep is requested. The arrays
   are indexed via initial acq iteration (not depth like non-XO. */
LOCAL int hdrsrchacq_coh_int[]        = { 48, 48, 48 };
LOCAL int hdrsrchacq_noncoh_int[]     = { 2, 2, 2 }; /* w/o diversity */
LOCAL int hdrsrchacq_noncoh_int_div[] = { 2, 2, 2 }; /* w/ diversity */
LOCAL int hdrsrchacq_burst_len[]      = { 2, 2, 2 }; /* w/o diversity */
#else

#define HDRSRCHACQ_MAX_ITERATIONS_FOR_DEBUG 2

/* For Non-XO targets, the following arrays are indexed via search depth,
   0=Deep and 1=Shallow.  */
LOCAL int hdrsrchacq_coh_int[]        = { 96, 48 };
LOCAL int hdrsrchacq_noncoh_int[]     = { 1, 2 }; /* w/o diversity */
LOCAL int hdrsrchacq_noncoh_int_div[] = { 1, 2 }; /* w/ diversity */

LOCAL int hdrsrchacq_burst_len[]      = { 2, 2 }; /* w/o diversity */
#endif /* FEATURE_XO */

LOCAL const int hdrsrchacq_burst_len_div[]  = { 1, 1 }; /* with diversity */

LOCAL const int hdrsrchacq_coh_trunc        = 2; /* Coherent truncation */


/*---------------------------------------------------------------------
   Cosets Groups

   Cosets are divided into 4 groups.
     CG0 is the doubly even group - the only group that needs to be
         searched on PN_INC=4n systems.
     CG1 is the singly even group.  Together with CG0, these compose
         all of the even PN offsets, and would be the only 2 groups
         that need to be searched in a PN_INC=2n system.
     CG2 & CG3 together make up the odd PN offsets.  Searching groups
         CG0, CG1, CG2, and CG3 completely search PN_INC=1 space.
---------------------------------------------------------------------*/

typedef enum
{
  HDRSRCHACQ_COSET_GROUP_0,     /* Cosets 0,  4,  8, & 12 */
  HDRSRCHACQ_COSET_GROUP_1,     /* Cosets 2,  6, 10, & 14 */
  HDRSRCHACQ_COSET_GROUP_2,     /* Cosets 1,  5,  9, & 13 */
  HDRSRCHACQ_COSET_GROUP_3,     /* Cosets 3,  7, 11, & 15 */

  HDRSRCHACQ_NUM_COSET_GROUPS   /* How many coset groups are there? */
}
hdrsrchacq_coset_group_enum_type;

LOCAL const int hdrsrchacq_coset_group_start[ ][4] = {
  
  { 0, 2, 1, 3 },
  { 1, 0, 2, 3 },
  { 2, 0, 1, 3 },
  { 3, 0, 2, 1 } 
  
};

/*---------------------------------------------------------------------
   Acquisition Stages
---------------------------------------------------------------------*/

/* Stage 1: Search / Dump driven */

typedef enum
{
  HDRSRCHACQ_S1_IDLE,
    /* Waiting for a lock on RF resources */

  HDRSRCHACQ_S1_WAITING_FOR_TCXO_OPEN_LOOP,
    /* Waiting for permission to set the TCXO to a reasonable value. */

  HDRSRCHACQ_S1_PREDETECT,
    /* Predetect in progress - Waiting for predetect search results */

  HDRSRCHACQ_S1_WAIT,
    /* Predetect complete, but waiting for Stage2 to finish (fail) */

  HDRSRCHACQ_S1_DETECT,
    /* Similar to dwell. Search strongest "pilots" */

  HDRSRCHACQ_S1_ANTI_ALIAS
    /* Detect searches at +/- 1 coset */
}
hdrsrchacq_stage1_enum_type;

/*-------------------------------------------------------------------*/

/* Stage 2: Poll Timer driven driven */

typedef enum
{
  HDRSRCHACQ_S2_IDLE,
    /* Waiting for detect/anti-alias results */

  HDRSRCHACQ_S2_WAITING_FOR_TCXO_CLOSE_LOOP,
    /* Waiting for permission to close the TCXO loop. */

  HDRSRCHACQ_S2_PULLIN,
    /* Assign fingers and pull the frequency lock in */

  HDRSRCHACQ_S2_SLAM
    /* Slam MSTR timing change and wait */
}
hdrsrchacq_stage2_enum_type;


#define HDRSRCHACQ_SLAM_TIMEOUT_MS   1000 /*ms*/
  /* MSTR slam should take effect immediately however give 1sec worth time
     for before timing out */

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
#define HDRSRCHACQ_DIVERSITY_ENABLE_TIME 10 /*ms*/
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

/* EJECT */
/*==========================================================================

                        ACQUISITION CONSTANTS

==========================================================================*/

enum
{
#ifdef FEATURE_HDR_SMALL_SRCH_WINDOW
  HDRSRCHACQ_PREDETECT_WINSIZE  = 0x800,
#else
  HDRSRCHACQ_PREDETECT_WINSIZE  = 0x2000,
#endif /* FEATURE_HDR_SMALL_SRCH_WINDOW */

  HDRSRCHACQ_DETECT_WINSIZE     = 32,
  HDRSRCHACQ_VERIFY_WINSIZE     = 8
};

enum
{
#ifdef FEATURE_HDR_SMALL_SRCH_WINDOW
  HDRSRCHACQ_MAX_SLICE  = HDR_PN_CIRC_X1 / HDRSRCHACQ_PREDETECT_WINSIZE,
#endif /* FEATURE_HDR_SMALL_SRCH_WINDOW */
  HDRSRCHACQ_COSET_GRP_INC      = 4,
  HDRSRCHACQ_NUM_COSETS         = 16,
  HDRSRCHACQ_NUM_COSETS_PER_GRP = 4,
  HDRSRCHACQ_QUARTER_CIRCLE_X2  = 0x4000
};

#define HDRSRCHACQ_PREDETECT_MIN_PEAK_DIFF_X2   16
  /* Peaks must be further apart than 8 chips to be consider unique
     during predetect */

#define HDRSRCHACQ_VERIFY_FING_CNT              2
  /* Only 2 fingers used during verify */

#define HDRSRCHACQ_MIN_PATH_ENG_DEFAULT         52
  /* Min path energy is for a path to be valid in linear units
    -10dB => dB = -10log(linear/512) or linear = 512 * 10^(dB/10) */

#define HDRSRCHACQ_MIN_PATH_ENG                 hdrsrchacq.detect.path_thresh
  /* Mininum path energy set by NV item or HDRSRCHACQ_MIN_PATH_ENG_DEFAULT */

#define HDRSRCHACQ_INIT_SINR                    26
  /* Min path energy is for a path to be valid in linear units
    -13dB => dB = -10log(linear/512) or linear = 512 * 10^(dB/10) */

#define HDRSRCHACQ_INIT_SRCH_INTERPOLATION      0
  /* Enable srch interpolation during pulling stage */

#define HDRSRCHACQ_VERIFY_WALSH                 3
  /* */

#define HDRSRCHACQ_MIN_AGC_FOR_FULL_ACQ         -95
  /* Minimum AGC necessary to perform a full acquisition */

enum
{
  HDRSRCHACQ_VERIFY_ASP_IDX_PRE  = 0,
  HDRSRCHACQ_VERIFY_ASP_IDX_POST = 1
};
/* */

#define HDRSRCHACQ_VERIFY_CELL_IDX              1
  /* */

#define HDRSRCHACQ_MAX_ITERATIONS               2
  /* Maximum number of initial acquition iterations */

#define HDRSRCHACQ_MAX_PREDETECT_PEAKS          16
  /* Maximum number of predetect peaks to pass to detect stage */

#define HDRSRCHACQ_PREDETECT_PEAKS_STORAGE      32
  /* Number of predetect peaks stored to pass to detect stage.
     This number is large to support dynamic debug changes.
     REVISIT - Probably make this smaller when we are happy with testing. */

#define HDRSRCHACQ_MAX_BINS                     5
  /* Maximum supported number of bins storing freq offsets in ppm
     used in detect stage. */

#define  HDRSRCHACQ_UNIT_10_PPM 10 /* unit is 10 PPM */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW

#define HDR_DSDA_MAX_PREDETECT_PEAKS ( 2 * HDRSRCHACQ_MAX_PREDETECT_PEAKS )

#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

#define  HDRSRCHACQ_PRIORITY_INVERSION_TIMEOUT_MS    (1000) 
  /* Acqusition inversion timer timeout value */

#define HDRSRCHACQ_MAX_TUNEAWAY_DURATION_MS          (30)
  /* Maximum time taken by HDR to tuneaway in ms
  if TRM gives the chain for R&N */

#define HDRSRCHACQ_AVG_ACQUISITION_DURATION_MS       (170)
  /* Average Acqusition duration */

#ifdef FEATURE_XO
/* EJECT */
/*==========================================================================

                     HDR SEARCH ACQUISITION BINNING

==========================================================================*/

enum
{
  HDRSRCHACQ_RGS_NUM_BINS = 3,
  HDRSRCHACQ_FT1_NUM_BINS = 5,
  HDRSRCHACQ_FT_WIDE_NUM_BINS = 5,
  HDRSRCHACQ_FT2_NUM_BINS = 3,
  HDRSRCHACQ_PRE_DETECT_5PPM_NUM_BINS = 1,
  HDRSRCHACQ_PRE_DETECT_15PPM_NUM_BINS = 3,
  HDRSRCHACQ_PRE_DETECT_20PPM_NUM_BINS = 5,
};
  /* Number of bins depending on the XO mode */

typedef enum
{
  HDRSRCHACQ_UNIT_1_PPM, /* unit is 1 PPM */
  HDRSRCHACQ_UNIT_1_5_PPM /* unit is 1.5 PPM */
}
hdrsrchacq_bin_units_type;
  /* freq offset units for a particular bin*/

#define HDRSRCHACQ_PRE_DETECT_BIN_OFFSET_HZ \
  ( hdrsrchacq_convert_ppm_to_hz( HDRSRCHACQ_UNIT_10_PPM * hdrsrchacq.pre_detect_bin.bin[hdrsrchacq.pre_detect_bin.idx], \
                                  hdrsrchacq.params.channel.band, \
                                  hdrsrchacq.params.channel.chan_num ) )

/* If binning units are 1.5 ppm, multiple hz by 3/2.
   Use C shortcut if-then-else. */
#define HDRSRCHACQ_BIN_OFFSET_HZ \
  ( hdrsrchacq.detect_bin.units == HDRSRCHACQ_UNIT_1_5_PPM ) ? \
  ( ( hdrsrchacq_convert_ppm_to_hz( hdrsrchacq.detect_bin.bin[hdrsrchacq.detect_bin.idx], \
                                    hdrsrchacq.params.channel.band, \
                                    hdrsrchacq.params.channel.chan_num ) * \
      3 ) / 2 ) : \
  ( hdrsrchacq_convert_ppm_to_hz( hdrsrchacq.detect_bin.bin[hdrsrchacq.detect_bin.idx], \
                                  hdrsrchacq.params.channel.band, \
                                  hdrsrchacq.params.channel.chan_num ) \
  )

/* Pred-detect bins */
LOCAL const int16 hdrsrchacq_pre_detect_5ppm_bin[HDRSRCHACQ_PRE_DETECT_5PPM_NUM_BINS] =
  { 0 };
LOCAL const int16 hdrsrchacq_pre_detect_15ppm_bin[HDRSRCHACQ_PRE_DETECT_15PPM_NUM_BINS] =
  { 0, -1, 1 };
LOCAL const int16 hdrsrchacq_pre_detect_20ppm_bin[HDRSRCHACQ_PRE_DETECT_20PPM_NUM_BINS] =
  { 0, -1, 1, -2, 2 };

/* RGS and FT2 frequency offsets are in units of 1 PPM
   FT2 frequency offsets are in units of 1.5 PPM.
   See hdrsrchacq_bin_units_type for types of units. */
LOCAL const int16 hdrsrchacq_bin_rgs[HDRSRCHACQ_RGS_NUM_BINS] =
  { -1, 0, 1 };
LOCAL const int16 hdrsrchacq_bin_ft1[HDRSRCHACQ_FT1_NUM_BINS] =
  { -2, -1, 0, 1, 2 };
LOCAL const int16 hdrsrchacq_bin_ft_wide[HDRSRCHACQ_FT_WIDE_NUM_BINS] =
  { -2, -1, 0, 1, 2 };
LOCAL const int16 hdrsrchacq_bin_ft2[HDRSRCHACQ_FT2_NUM_BINS] =
  { -2, 0, 2 };

#endif /* FEATURE_XO */

#define HDRSRCHACQ_MIN_RF_TUNEAWAY_MS       15
  /* Tuneaway overhead in ms */

#define HDRSRCHACQ_TA_TUNEAWAY_US       6000
  /* Tuneaway overhead in ms = disable of primary RF chain, FW and HW 
     without diversity RF chain disable */

#define HDRSRCHACQ_MIN_RF_TUNEAWAY_SCLK \
          (TIMETICK_SCLK_FROM_MS(HDRSRCHACQ_MIN_RF_TUNEAWAY_MS))
  /* Tuneaway overhead in sclks */

#define HDRSRCHACQ_MAX_TA_ALLOWED_COUNT (3)
  /* Maximum TA allowed in Acq state */

typedef enum
{
  HDRSRCHACQ_REDIR_NONE,           /* Regular ACQ */
  HDRSRCHACQ_REDIR_ACQ,            /* ACQ to redirect */
  HDRSRCHACQ_REDIR_ACQ_EXCHANGE,   /* ACQ to redirect through exchange */
  HDRSRCHACQ_REDIR_TA,             /* TA during ACQ */
  HDRSRCHACQ_REDIR_SUCC,           /* redirection success */
  HDRSRCHACQ_REDIR_FAIL,           /* redirection failure */
  HDRSRCHACQ_REDIR_ABORT           /* redirection aborted */
}
hdrsrchacq_redir_state_type;
  /* Various states during IRAT redirection */

/* EJECT */
/*==========================================================================

                     HDR SEARCH ACQUISITION DATA

As much ACQ data is lumped together in one structure, so that it becomes
possible to dynamically allocate acquisition data memory during the
acquisition state, and discard the memory when no longer needed.

This will become important when RAM is based on SDRAM.

All variables are accessed as "hdrsrchacq.member_name".  When dynamic
allocation is implemented, a pointer to the structure is needed instead.
At this point, hdrsrchacq must become a dereferenced pointer, instead of
a variable.  Ie)

  #define hdrsrchacq  (*hdrsrchacq_struct_ptr)

==========================================================================*/

typedef struct
{
  uint8                             coset;
    /* Coset energy peak was discovered on */

  uint16                            eng;
    /* Strength of energy peak */

  uint16                            posx2;
    /* Position of energy peak */
}
hdrsrchacq_peak_struct_type;


typedef struct
{
  uint32                            winsize;
    /* Predetect search window size in chips */

  int16                             max_peaks;
    /* Maximum number of peaks to pass to detect */

  hdrsrchacq_peak_struct_type       peaks[ HDRSRCHACQ_PREDETECT_PEAKS_STORAGE ];
    /* Predetect peaks */

  int16                             num_peaks;
    /* Number of predetection peaks. */

  int16                             num_peaks_remaining;
    /* In detect stage, this is used as the number of predetect peaks
       remaining to be searched.  */

  int16                             peak_idx;
    /* Index into predetect peaks used during detect stage */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW

  int dsda_num_peaks;
    /* Number of peaks stored */

  hdrsrchacq_peak_struct_type  dsda_peaks[HDRSRCHACQ_PREDETECT_PEAKS_STORAGE];
    /* Store Predetect peaks in DSDA modee here */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */


}
hdrsrchacq_predetect_struct_type;


typedef struct
{
  uint32                            winsize;
    /* Detect search window size in chips */

  uint16                            path_thresh;
    /* Ec/Io threshold above which a path is combined in
       computing pilot energy.  See mps_ps2eng_table[] */

  hdrsrchdrv_srch_res_struct_type   paths;
    /* Pilot detect paths (peaks & posx2) */

  uint32                            energy;
    /* Pilot energy */

  uint2                             coset;
    /* Coset where best pilot was found */

#ifdef FEATURE_XO
  uint8                             bin_idx;
    /* Bin idx with the best energy */
#endif /* FEATURE_XO */
}
hdrsrchacq_detect_struct_type;

typedef struct
{
  qword                             start_ms;
    /* Wall clock time the timer is started at */

  qword                             elapsed_ms;
    /* Duration time has run for */
}
hdrsrchacq_timer_struct_type;

typedef struct
{
  hdrsrchacq_timer_struct_type      real;
    /* Time from ACQUIRE_CMD being received */

  hdrsrchacq_timer_struct_type      task;
    /* Time during which HDR owned the RF */
}
hdrsrchacq_timers_struct_type;

#ifdef FEATURE_XO
typedef struct
{
  tcxomgr_uncertainty_type          freq_uncertainty_ppm;
    /* Freq uncertainty in ppm */

  const int16                       *bin;
    /* Pre-detect bin */

  int8                              idx;
    /* Bin index */

  int8                              max_idx;
    /* Maximum bin index */

  int16                             freq_offset;
    /* Calculated freq_offset for current bin (Hz) */

}
hdrsrchacq_pre_detect_bin_struct_type;

typedef struct
{
  hdrsrchafc_xo_cal_mode_enum_type  xo_cal_mode;
    /* XO calibration mode from AFC state machine */

  hdrsrchacq_bin_units_type         units;
    /* Units of offset in each bin, e.g., 1 PPM, 1.5 PPM */

  const int16                       *bin;
    /* Pointer to bin storing freq offsets for searches in detect stage */

  int8                              idx;
    /* Bin index */

  int8                              max_idx;
    /* Maximum bin index */

  int16                             freq_offset;
    /* Calculated freq_offset for current bin (Hz) */

  int16                             best_peak_freq_offset;
    /* Saved freq_offset from best peak (Hz) */
}
hdrsrchacq_detect_bin_struct_type;
#endif /* FEATURE_XO */

typedef struct
{
  sys_pilot_pn_type                 coset;
    /* Coset PN in the brute force search list */

  uint32                            eng;
    /* The search energy */

  hdrsrchdrv_srch_res_struct_type   paths;
    /* Location to save the search result */
}
hdrsrchacq_coset_struct_type;

typedef struct
{
  hdrsrchacq_coset_struct_type      coset_list[HDRSRCHACQ_NUM_COSETS];
    /* Coset list for brute force search */

  uint8                             coset_cnt;
    /* Number of cosets in the list */

  uint8                             coset_idx;
    /* Coset index for current predetect search in pilot measurement */

  uint32                            acq_time;
    /* Total time for brute force acquisition */

  uint8                             num_strong_sectors;
    /* Number of strong sector found in detect and anti-alias search */
}
hdrsrchacq_brute_force_acq_struct_type;

typedef struct
{
  timer_type                              timer;
    /* Timer used for tune away */

  boolean                                 no_tune_away;
    /* Do we know when we are supposed to tune away yet? */

  timetick_type                           start_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  timetick_type                           end_time_sclks;
    /* Sclk timestamp for when we should tune away. */

  boolean                                 req_notify_pending;
  /* TRM Primary Chain Request and notify pending */

  uint8                                   tune_away_cnt;
    /* counts the number of times TA happened for unlock-by */

  hdrsrchrf_reason_enum_t                 acq_priority;
    /* Priority with which HDR is attempting ACQ */

  boolean                                 restore_ta_cnt;
    /* status whether the tune away conuter has to be restored */
}
hdrsrchacq_tune_away_struct_type;

typedef struct
{
  hdrsrch_acquire_params_type       params;
    /* Channel acquisition parameters */

  hdrsrchacq_depth_enum_type        depth;
    /* Acquisition depth */

  hdrsrchacq_coset_group_enum_type  coset_group;
    /* Coset in acquisition depth */

  hdrsrchacq_stage1_enum_type       stage1;
    /* State in coset acquisiton (Predetect, Detect, Anti-Alias, ...) */

  hdrsrchacq_stage2_enum_type       stage2;
    /* State in pulling pipeline (Pull-in, MSTR slam, Sync, ...) */

  hdrsrchacq_predetect_struct_type  predetect;
    /* Predetect data */

  hdrsrchacq_detect_struct_type     detect;
    /* Detect data */

  int2                              anti_alias;
    /* Anti-alias detect coset */

  hdrsrchacq_timers_struct_type     timer;
    /* Real and Virtual time for acquisition */

  boolean                           acq_init_pending;
    /* Check if Acquisition init is pending */

  uint8                             coset_idx;
    /* Index to hdrsrchacq_coset_group_start */

  uint8                             coset;
    /* Current coset the search is programmed on */

  boolean                           coset_group_searched;
    /* The current coset group has been searched */

  uint8                             num_iterations;
    /* Number of initial acquisition iterations */

  uint8                             max_iterations;
    /* Maximum number of inital acquisition iterations */

  uint8                             disable_srch_int;
    /* 0 = Enable srch interpolation,
       1 = Disable srch interpolation */

  int8                              init_sinr;
    /* Initial SINR to assign fingers in linear units for pull-in stage */

  int8                              mdsp_ft_gain;
    /* MDSP FTP Gain */

  hdrsrchacq_brute_force_acq_struct_type  brute_force_acq;
    /* Brute force acquisition data */

#ifdef FEATURE_XO

  hdrsrchacq_pre_detect_bin_struct_type    pre_detect_bin;
    /* Pre-detect binning related data */

  hdrsrchacq_detect_bin_struct_type        detect_bin;
    /* Detect binning related data */

  int16                                    freq_offset;
    /* Calculated freq_offset for current bin (Hz) */

  int16                                    best_peak_freq_offset;
    /* Saved freq_offset from best peak (Hz) */

#endif /* FEATURE_XO */

  hdrsrchacq_tune_away_struct_type         tune_away;
    /* Tune away state */

  hdrsrchacq_redir_state_type              redir_state;
    /* IRAT redirection state */
} 
hdrsrchacq_struct_type;


LOCAL hdrsrchacq_struct_type        hdrsrchacq;

/* EJECT */

/*==========================================================================

                       ACQUISITION STATE MACROS

==========================================================================*/

/* EJECT */
/*==========================================================================

                       ACQUISITION DEBUG LOGGING

 Code has been added to facilitate debugging of acquisition algorithm. Lots
 of acquisition related information is stored in hdrsrchacq_debug which
 can be viewed using emulators.

 This logging is featurized so that it can be removed from the final
 commercial code.  Removal is highly recommended, since it currently takes
 a whopping 37kbytes of storage!

==========================================================================*/

#undef HDRSRCHACQ_DEBUG

#ifdef HDRSRCHACQ_DEBUG

#define HDRSRCHACQ_DEBUG_FING_NLOG  16

typedef struct
{
  hdrsrchdrv_srch_list_struct_type  cmd[16];
  hdrsrchdrv_srch_res_struct_type   res[16];
  int                               n;
} hdrsrchacq_debug_srch_t;

typedef struct
{
  hdrsrchfing_fing_struct_type      fing[HDRSRCHACQ_DEBUG_FING_NLOG]
                                        [HDRSRCHACQ_VERIFY_FING_CNT];
  int                               n;
} hdrsrchacq_debug_finger_log_t;


typedef struct
{
  hdrsrchacq_debug_srch_t           pre_d;
  hdrsrchacq_debug_srch_t           detect;
  hdrsrchacq_debug_srch_t           alias[2];
  hdrsrchacq_debug_finger_log_t     pullin;
  hdrsrchacq_debug_finger_log_t     slam;
} hdrsrchacq_debug_group_t;

typedef struct
{
  int                               s1;
  int                               s2;
  hdrsrchacq_debug_group_t          grp[8];
} hdrsrchacq_debug_t;

hdrsrchacq_debug_t  hdrsrchacq_debug;

static void debug_srch(hdrsrchacq_debug_srch_t *srch, int n)
{
  int i;
  srch->n = n;
  for(i=0; i<n; i++)
  {
    srch->cmd[i] = hdrsrchdrv.srch_list[i];
  }
}

static void debug_dump(hdrsrchacq_debug_srch_t *srch)
{
  int i;
  for(i=0; i<srch->n; i++)
  {
    srch->res[i] = *hdrsrchdrv.srch_list[i].srch_task.res_ptr;
  }
}

static void debug_fing(hdrsrchacq_debug_finger_log_t *log)
{
  int f;
  for(f=0; f<HDRSRCHFING_VERIFY_ACQ_FING_CNT; f++)
  {
    log->fing[log->n][f] = hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][f];
  }

  if ( ++log->n == HDRSRCHACQ_DEBUG_FING_NLOG )
    log->n = 1;
    /* Wrap, but save 0th entry */
}

#define DEBUG_PREDETECT_SRCH(n) \
  debug_srch(&hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].pre_d, n);

#define DEBUG_DETECT_SRCH(n) \
  debug_srch(&hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].detect, n);

#define DEBUG_ANTI_ALIAS_SRCH(n,a) \
  debug_srch(&hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].alias[a>0], n);


#define DEBUG_PREDETECT_DUMP() \
  debug_dump(&hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].pre_d);

#define DEBUG_DETECT_DUMP() \
  debug_dump(&hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].detect);

#define DEBUG_ANTI_ALIAS_DUMP(a) \
  debug_dump(&hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].alias[a>0]);


#define DEBUG_PULLIN_FING() \
  debug_fing(&hdrsrchacq_debug.grp[hdrsrchacq_debug.s2].pullin);

#define DEBUG_SLAM_FING() \
  debug_fing(&hdrsrchacq_debug.grp[hdrsrchacq_debug.s2].slam);


#define DEBUG_INIT() \
  hdrsrchacq_debug.s1 = -1, \
  hdrsrchacq_debug.s2 = -1;


#define DEBUG_S1_ADVANCE() \
  hdrsrchacq_debug.s1++, \
  hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].pre_d.n    = 0, \
  hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].detect.n   = 0, \
  hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].alias[0].n = 0, \
  hdrsrchacq_debug.grp[hdrsrchacq_debug.s1].alias[1].n = 0;

#define DEBUG_S2_ADVANCE() \
  hdrsrchacq_debug.s2++, \
  hdrsrchacq_debug.grp[hdrsrchacq_debug.s2].pullin.n = 0, \
  hdrsrchacq_debug.grp[hdrsrchacq_debug.s2].slam.n   = 0;


#else /* No Acquisition Debugging required */


#define DEBUG_PREDETECT_SRCH(n)
#define DEBUG_DETECT_SRCH(n)
#define DEBUG_ANTI_ALIAS_SRCH(n,a)

#define DEBUG_PREDETECT_DUMP()
#define DEBUG_DETECT_DUMP()
#define DEBUG_ANTI_ALIAS_DUMP(a)

#define DEBUG_PULLIN_FING()
#define DEBUG_SLAM_FING()

#define DEBUG_INIT()

#define DEBUG_S1_ADVANCE()
#define DEBUG_S2_ADVANCE()


#endif /* HDRSRCHACQ_DEBUG */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHBCACQ_ENABLE_TUNE_AWAYS

DESCRIPTION
  Turns on the ability for tune aways to happen within the ACQ state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchacq_enable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are now interested in primary chain events. */

  hdrsrchacq.tune_away.start_time_sclks = timetick_get();
    /* Save the current time in sclks. */

  hdrsrchacq.tune_away.no_tune_away = TRUE;
    /* There is no tune away as of now. */

  hdrsrchrf_retain_lock( HDRSRCHRF_PRI_CHAIN, hdrsrch_rf_unlock_cb );
    /* Register an unlock callback so that we may be interrupted. */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                "HDR LRE: ACQ enable tune_aways" );  /* REVISIT */

} /* hdrsrchacq_enable_tune_aways */


/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHACQ_TIMER_INIT

DESCRIPTION
  Initialize RF lock timer after entering ACQ state.  This is used
  for tuneaway timer, i.e., TRM unlock by calculation.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  NOne

===========================================================================*/

LOCAL void hdrsrchacq_timer_init( void )
{
  static boolean          tune_away_timer_defined = FALSE;
    /* initialize the extend lock timer to be not defined for first time */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* One time timer definition */

  if ( !tune_away_timer_defined )
  {
    timer_def( &hdrsrchacq.tune_away.timer, NULL, HDRSRCH_TCB, 
               HDRSRCH_PRI_CHAIN_EVENT_SIG, NULL, 0 );

    hdrsrchacq.tune_away.tune_away_cnt = 0;

    hdrsrchacq.tune_away.acq_priority = HDRSRCHRF_ACQUISITION;

    hdrsrchacq.tune_away.restore_ta_cnt = FALSE;
    
    tune_away_timer_defined = TRUE;
  }

  hdrsrchacq.tune_away.req_notify_pending = FALSE;
    /* No TRM request and notify is pending */

}/* hdrsrchacq_timer_init */

/* EJECT */
/*==========================================================================

FUNCTION     : HDRSRCHACQ_START_TIMER

DESCRIPTION  : Starts an acquisition timer

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS :

==========================================================================*/

LOCAL void hdrsrchacq_start_timer
(
  hdrsrchacq_timer_struct_type  * timer
    /* Acquisition timer to start */
)
{
  time_get_ms ( timer->start_ms );
    /* Record start time */

} /* hdrsrchacq_start_timer */


/*==========================================================================

FUNCTION     : HDRSRCHACQ_STOP_TIMER

DESCRIPTION  : Stops an acquisition timer

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Updates "Elapsed Time" for the timer

==========================================================================*/

LOCAL void hdrsrchacq_stop_timer
(
  hdrsrchacq_timer_struct_type  * timer
    /* Acquisition timer to start */
)
{
  qword                           end_ms;
    /* Time the timer is stopped at */

  qword                           delta_ms;
    /* Time the timer ran for */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  time_get_ms ( end_ms );
    /* Record start time */

  qw_sub ( delta_ms, end_ms, timer->start_ms );
    /* Compute the time since "start" */

  qw_add( timer->elapsed_ms, timer->elapsed_ms, delta_ms );
    /* Add duration to the total elapsed time */

} /* hdrsrchacq_stop_timer */

/*==========================================================================

FUNCTION     : HDRSRCHACQ_START_INV_TIMER

DESCRIPTION  : Starts an acquisition inversion timer

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

==========================================================================*/

LOCAL void hdrsrchacq_start_inv_timer( void )
{
  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                  "HDR Srch Acq start inv timer %d", 
                  hdrsrchacq.tune_away.acq_priority );

  rex_set_timer( &hdrsrch.timeout_timer, HDRSRCHACQ_PRIORITY_INVERSION_TIMEOUT_MS );
  hdrsrch_add_to_wait_mask( HDRSRCH_TIMEOUT_SIG );
    /* Start inversion timeout timer for 1sec to perform priority transistion */

} /* hdrsrchacq_start_inv_timer */


#ifdef FEATURE_XO
/*==========================================================================

FUNCTION     : HDRSRCHACQ_SET_BINNING_MODE

DESCRIPTION  : Set binning mode based on input from AFC.  If NV item debug
               is override, ignore this input.  If the input value shows
               calibration has not succeeded, complain.

DEPENDENCIES : None

INPUT        : xo_cal_mode - XO calibration mode

RETURN VALUE : FALSE - Not NV override AND xo_cal_mode implies not calibrated
               TRUE  - NV override OR xo_cal_mode is in range

SIDE EFFECTS : Overrides some default values in hdrsrchacq

==========================================================================*/

LOCAL boolean hdrsrchacq_set_binning_mode
(
  hdrsrchafc_xo_cal_mode_enum_type    xo_cal_mode,
    /* XO calibration mode */

  uint8                               freq_uncertainty
    /* Freq uncertainty in ppm */
)
{
  boolean xo_cal_mode_valid = TRUE;
    /* Indicate if xo_cal_mode is valid (TRUE) or not (FALSE) */

  uint32  curr_time;
    /* Current time */

  tcxomgr_temp_set_type curr_temp;
    /* Current temperature */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Determine pre-detetc binning */
  hdrsrchacq.pre_detect_bin.freq_uncertainty_ppm = ( tcxomgr_uncertainty_type) freq_uncertainty;

  /* Determine one of the following:
     1. Override xo_cal_mode with NV Item's binning mode
     2. No NV item override, i.e.,use tcxomgr xo_cal_mode, XO is NOT calibrated
     3. No NV item override, i.e.,use tcxomgr xo_cal_mode, XO is calibrated */

  /* Determine if NV item override or not.  TRUE for override if:
     1. hdrsrch_dbg_array is enabled for ACQ  AND
     2. NV binning mode is NOT from tcxomgr */
  if ( ( hdrsrch_dbg_array[HDRSRCHACQ_NV_ENABLED_IDX] ==
         HDRSRCH_DBG_ARRAY_ACQ_ENABLED ) &&
       ( hdrsrch_dbg_array[HDRSRCHACQ_BINNING_MODE_IDX] !=
         HDRSRCH_DBG_FROM_TCXOMGR_BIN_MODE ) )
  {
    /* Do nothing.  hdrsrchacq.detect_bin.xo_cal_mode has already been set from NV. */
    HDR_MSG_SRCH_1( MSG_LEGACY_MED,
                    "Binning mode OVERRIDE from NV (mode=%d)",
                    hdrsrchacq.detect_bin.xo_cal_mode );
  }
  else
  {
    /* No NV item override.  Check if calibrated or not */
    if ( xo_cal_mode != HDRSRCHACQ_NOT_CAL )
    {
      /* Note: If xo_cal_mode is not recognized, the default is FT wide binning. */
      hdrsrchacq.detect_bin.xo_cal_mode = xo_cal_mode;

      /* TBD to be removed after XO Mgr supports FT wide bin mode */
      tcxomgr_get_time_and_temp( &curr_time, &curr_temp );
        /* Get time and temperature */

      if ( ( hdrsrchacq.detect_bin.xo_cal_mode != HDRSRCHACQ_RGS_MODE )                        && 
           ( ( TCXOMGR_XO_TEMP_TO_DEGC( curr_temp.xo ) > HDRSRCHACQ_HIGH_TEMP_THRESH )  ||
             ( TCXOMGR_XO_TEMP_TO_DEGC( curr_temp.xo ) < HDRSRCHACQ_LOW_TEMP_THRESH )     ) )
      {
        hdrsrchacq.detect_bin.xo_cal_mode = HDRSRCHACQ_FT_WIDE_MODE;
          /* Use FT wide mode */
      }

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH,
                      "Binning mode (%d) set by AFC. Temp %d C",
                      hdrsrchacq.detect_bin.xo_cal_mode, 
                      TCXOMGR_XO_TEMP_TO_DEGC( curr_temp.xo ) );

    }
    else
    {
      xo_cal_mode_valid = FALSE;
        /* XO not cal-ed */

      hdrsrchacq.detect_bin.xo_cal_mode = HDRSRCHACQ_FT_WIDE_MODE;
        /* use FT wide bin mode */

      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                      "Binning mode foed to %d. XO not calibrated!",
                      hdrsrchacq.detect_bin.xo_cal_mode );
    }

  }

  return( xo_cal_mode_valid );

} /* hdrsrchacq_set_binning_mode */

#endif /* FEATURE_XO */

/*==========================================================================

FUNCTION     : HDRSRCHACQ_GET_TUNABLE_PARAMS

DESCRIPTION  : Read NV items from hdrsrch_dbg_array[] into hdrsrchacq
               variables to override default values if
               hdrsrch_dbg_array[HDRSRCHACQ_NV_ENABLED_IDX] is enabled.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : Overrides some default values in hdrsrchacq

==========================================================================*/

LOCAL void hdrsrchacq_get_tunable_params( void )
{
  int16 i;  /* Loop variable */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Check if tunable parameters for hdrsrchacq are enabled */
  if ( hdrsrch_dbg_array[HDRSRCHACQ_NV_ENABLED_IDX] ==
         HDRSRCH_DBG_ARRAY_ACQ_ENABLED )
  {

    /* Tunable parameters are enabled */

    hdrsrchacq.max_iterations = 
      hdrsrch_dbg_array[HDRSRCHACQ_NUM_ITERATION_IDX];
        /* Number of full initial acquisition loops to attempt*/

    /* Start of predetect tunable parameters */

    for ( i = 0; i < HDRSRCHACQ_MAX_ITERATIONS_FOR_DEBUG; i++ )
    {
      hdrsrchacq_coh_int[i]        =
        hdrsrch_dbg_array[HDRSRCHACQ_PREDETECT_COH_INT_LEN_IDX];
          /* Predetect coherent integration length*/

      hdrsrchacq_noncoh_int[i]     =  /* w/o diversity */
        hdrsrch_dbg_array[HDRSRCHACQ_PREDETECT_NON_COH_INT_LEN_IDX];
          /* Predetect non-coherent integration length*/

      hdrsrchacq_noncoh_int_div[i] =  /* w/ diversity */
        hdrsrch_dbg_array[HDRSRCHACQ_PREDETECT_NON_COH_INT_LEN_IDX];
          /* Predetect non-coherent integration length*/

      hdrsrchacq_burst_len[i]      =  /* w/o diversity */
        hdrsrch_dbg_array[HDRSRCHACQ_PREDETECT_NUM_PILOT_BURSTS_IDX];
          /* Predetect number of pilot bursts*/
    }

    hdrsrchacq.predetect.winsize =
      hdrsrch_dbg_array[HDRSRCHACQ_PREDETECT_SRCH_WIN_SIZE_IDX] << 8;
        /* Predetect search window size (unit is 256 in dbg array)
           Supported: 0x08=2048, 0x10=4096, 0x20=8192 */

    hdrsrchacq.predetect.max_peaks =
      hdrsrch_dbg_array[HDRSRCHACQ_PREDETECT_NUM_DETECT_PEAKS_IDX] * 16;
        /* Predetect number of detect peaks passed to detect stage
           (unit is 16)*/

    /* Start of detect tunable parameters */

    for ( i=0; i < HDRSRCHACQ_MAX_ITERATIONS_FOR_DEBUG; i++ )
    {
      hdrsrchacq_coh_int[i]        =
        hdrsrch_dbg_array[HDRSRCHACQ_DETECT_COH_INT_LEN_IDX];
          /* Detect coherent integration length*/

      hdrsrchacq_noncoh_int[i]     =  /* w/o diversity */
        hdrsrch_dbg_array[HDRSRCHACQ_DETECT_NON_COH_INT_LEN_IDX];
          /* Detect non-coherent integration length*/

      hdrsrchacq_noncoh_int_div[i] =  /* w/ diversity */
        hdrsrch_dbg_array[HDRSRCHACQ_DETECT_NUM_PILOT_BURSTS_IDX];
          /* Detect non-coherent integration length*/

      hdrsrchacq_burst_len[i]      =  /* w/o diversity */
        hdrsrch_dbg_array[HDRSRCHACQ_DETECT_SRCH_WIN_SIZE_IDX];
          /* Detect number of pilot bursts*/
    }

    hdrsrchacq.detect.path_thresh =
      hdrsrch_dbg_array[HDRSRCHACQ_DETECT_PATH_THRESH_IDX];
        /* Detect path threshold to combine pilot energy,
           linear units = 512 * 10^(dB/10)*/

#ifdef FEATURE_XO
    /* Setup for frequency mode binning.  Map freq error to freq error mode. */
    if ( hdrsrch_dbg_array[HDRSRCHACQ_BINNING_MODE_IDX] ==
         HDRSRCH_DBG_RGS_BIN_MODE )
    {
      hdrsrchacq.detect_bin.xo_cal_mode = HDRSRCHACQ_RGS_MODE;
    }
    else if ( hdrsrch_dbg_array[HDRSRCHACQ_BINNING_MODE_IDX] ==
              HDRSRCH_DBG_FT1_BIN_MODE )
    {
      hdrsrchacq.detect_bin.xo_cal_mode = HDRSRCHACQ_FT1_MODE;
    }
    else if ( hdrsrch_dbg_array[HDRSRCHACQ_BINNING_MODE_IDX] ==
              HDRSRCH_DBG_FT2_BIN_MODE )
    {
      hdrsrchacq.detect_bin.xo_cal_mode = HDRSRCHACQ_FT2_MODE;
    }
    else
    {
      hdrsrchacq.detect_bin.xo_cal_mode = HDRSRCHACQ_FT1_MODE;
        /* Choose worst case 5ppm binning */
    }
#endif /* FEATURE_XO */

#if 0

    hdrsrchacq.num_anti_alias_peaks = /* Not supported yet */
      hdrsrch_dbg_array[HDRSRCHACQ_DETECT_NUM_ANTI_ALIAS_PEAKS_IDX];
        /* Detect number of peaks passed to anti-alias */

    hdrsrchacq.num_pullin_fings = /* Not supported yet */
      hdrsrch_dbg_array[HDRSRCHACQ_DETECT_NUM_PULLIN_FINGS_IDX];
        /* Detect number of fingers assigned before pullin */

#endif

    /* Start of pull-in tunable parameters */

    hdrsrchacq.init_sinr =
      hdrsrch_dbg_array[HDRSRCHACQ_PULLIN_INIT_RSSI_IDX];
        /* Pull-in initial RSSI to assign finger's init_sinr */

    /* Start of MDSP tunable parameters */

    hdrsrchacq.mdsp_ft_gain =
      hdrsrch_dbg_array[HDRSRCHACQ_MDSP_FT_GAIN_IDX];
        /* MDSP FT gain */

    /* More general tunable parameters */
    hdrsrchacq.disable_srch_int = 
      hdrsrch_dbg_array[HDRSRCHACQ_SRCH_INT_IDX];
        /* Disable srch interpolation */

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH,
                    "Init Acq Tunable Params Enabled: %d %d %d %d",
                    hdrsrchacq.predetect.winsize,
                    hdrsrchacq.predetect.max_peaks,
                    hdrsrchacq.init_sinr,
                    hdrsrchacq.disable_srch_int
                  );
  } /* if tunable params enabled */

} /* hdrsrchacq_get_tunable_params */

/*==========================================================================

FUNCTION     : HDRSRCHACQ_MEASURE_TIME

DESCRIPTION  :
  Measure the time between "start_ms" of specified timer and now.
  This function needs to run with interrupt locked.

DEPENDENCIES : 
  We assume the timer used for this time measurement has been started before
  this function is called, which is true because both "real" and "task" 
  timers are already running when we measure RF warm-up time. And RF warm-up
  time measurement is the first measurement for the ACQ INFO log packet.

RETURN VALUE :
  The measured elapsed time in ms.

SIDE EFFECTS :
  The specified timer will be stopped and re-started.

==========================================================================*/

uint16 hdrsrchacq_measure_time ( void )
{
  qword                           end_ms;
    /* Time the timer is stopped at */

  qword                           delta_ms;
    /* Time the timer ran for */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  time_get_ms ( end_ms );
    /* Record start time */

  qw_sub ( delta_ms, end_ms, hdrsrchacq.timer.task.start_ms );
    /* Compute the time since "start" */

  REX_DISABLE_PREMPTION();

  hdrsrchacq_stop_timer( &hdrsrchacq.timer.task );

  hdrsrchacq_start_timer( &hdrsrchacq.timer.task );

    /* In order to get the next time measurement, we need to update the 
       "start_ms" field of this timer to the current time, which is done by 
       calling "start_timer" function. We also need to call "stop_timer" 
       function to update "elapsed_time" field, which depends on "start_ms" 
       field. "stop_timer" function needs to be called before "start_timers" 
       function.                                                            */

  REX_ENABLE_PREMPTION();

  return (uint16) qw_lo( delta_ms );

} /* hdrsrchacq_measure_time */

/*=========================================================================

FUNCTION     : HDRSRCHACQ_UPDATE_ACQ_INFO

DESCRIPTION  : 
  Updates ACQ INFO global structure, according to current acquisition result.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : none

=========================================================================*/

LOCAL void hdrsrchacq_update_acq_info
( 
  log_hdr_acq_result_enum_type result 
  /* current acquisition result when ACQ INFO is updated  */
)
{
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchlog.acq_info.result          = result;
  /* Acquisition result. This info is needed for updating those cumulative 
     counters ( for example, attempt_success_count )                      */

  REX_DISABLE_PREMPTION();

  hdrsrchacq_stop_timer( &hdrsrchacq.timer.task );
  hdrsrchacq_stop_timer( &hdrsrchacq.timer.real );
    /* Timers have to be stopped to update the elapsed_ms field */

  hdrsrchacq_start_timer( &hdrsrchacq.timer.task );
  hdrsrchacq_start_timer( &hdrsrchacq.timer.real );
    /* Re-start the timers right away, so normal time keeping functionality 
       won't be affected by logging. Also see comments in function 
       "hdrsrchacq_measure_time" about starting/stopping timers           */

  REX_ENABLE_PREMPTION();

  hdrsrchlog.acq_info.total_time_no_1x_tuneaway_ms  =
    qw_lo( hdrsrchacq.timer.task.elapsed_ms );
      /* "task" timer gives the elapsed time without 1x tune-away */

  hdrsrchlog.acq_info.total_time_1x_tuneaway_ms     =
    qw_lo( hdrsrchacq.timer.real.elapsed_ms );
      /* "real" timer gives the elapsed time including 1x tune-away */
  
  hdrsrchlog.acq_info.coset = hdrsrchacq.detect.coset;
    /* Record the coset info  */

  switch( result )
  {
    case  HDRLOG_ACQ_SLAM_SUCCESS:
      /* We are about to enter SYNC state now and we need to collect
         acquisition info before we change state.   */

      hdrsrchlog.acq_info.total_time_ms +=
        hdrsrchlog.acq_info.total_time_no_1x_tuneaway_ms;
        /* Total pilot acquisition time excluding sync time           */

      break;

    case  HDRLOG_ACQ_MICRO_FAILURE:
    case  HDRLOG_ACQ_PILOT_FAILURE:
    case  HDRLOG_ACQ_FLL_FAILURE:
    case  HDRLOG_ACQ_SLAM_FAILURE:
      /* The last acquisition effort on the last coset pilot failed
         at various stages. And now it is time to record the time     */

      hdrsrchlog.acq_info.total_failure_time_ms +=
        hdrsrchlog.acq_info.total_time_no_1x_tuneaway_ms;

      break;

    default:

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Can not recognize result: %d",
                      (uint8)result );
      break;
  }

  hdrsrchlog.acq_info.srch_cmd.mode   = hdrsrchacq.params.mode;
    /* Record acquisition mode ( micro, deep, etc. )  */

  if ( hdrsrchlog.acq_info.srch_cmd.mode == SYS_HDR_ACQ_MODE_MICRO )
  {
    hdrsrchlog.acq_info.srch_cmd.stage = 0xf;
      /* search stage ( deep/shallow ) does not apply to micro search */
  }
  else
  {
    hdrsrchlog.acq_info.srch_cmd.stage = ( hdrsrchacq.depth < 
      HDRSRCHACQ_NUM_DEPTHS ) ? hdrsrchacq.depth : HDRSRCHACQ_DEEP;
      /* Record the last search stage ( deep/shallow )  */
  }
} /* hdrsrchacq_update_acq_info */

/* EJECT */
/*==========================================================================

FUNCTION     : HDRSRCHACQ_DISTX2

DESCRIPTION  : Returns the distance between two points on the chipx2 PN
               circle. It takes care of wrapping the counts around the PN
               circle boundary.

DEPENDENCIES : None

RETURN VALUE : disance between two points on pn circle in chipx2 units

SIDE EFFECTS :

==========================================================================*/

LOCAL int hdrsrchacq_distx2
(
  int pos1,
  int pos2
)
{
  int delta;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  delta = ( pos1 - pos2 ) & HDR_PN_CIRC_MASK_X2;

  if ( (uint32) delta > HDR_HALF_PN_CIRC_X2 )
  {
     delta = ( int32 )HDR_PN_CIRC_X2 - delta;
  }

  return delta;

} /* hdrsrchacq_distx2() */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_GET_BEST_PEAKS

DESCRIPTION
  Extract the largest peaks from the search result buffer

DEPENDENCIES
  Results in result buffer

INPUTS
  best_peaks    - Storage for extracted strongest peaks
  srch_res      - Search result buffer

RETURN VALUE
  Number of peaks placed in best_peaks[ ] array.

SIDE EFFECTS
  None

==========================================================================*/

LOCAL int hdrsrchacq_get_best_peaks
(
  hdrsrchacq_peak_struct_type       *best_peaks,
    /* Output: Array of best peaks */

  int                               max_peaks,
    /* Number of best peaks to accumulate */

  hdrsrchdrv_srch_list_struct_type  *list,
    /* Search command list */

  int                               num_searches
    /* Number of search commands in search list */
)
{
  int                               num_peaks;
    /* Number of peaks in best_peaks[ ] array */

  int                               weakest_eng;
    /* Energy of weakest peak in best_peak[ ] array. */

  int                               weakest_inx;
    /* Index position of weakest peak in best_peak array. */

  int                               search;
    /* Loop index for all search commands in search list */

  int                               peak;
    /* Loop index for all peaks in a search result buffer */

  int                               i;
    /* Loop Index */

  int                               dist_x2;
    /* Distance between peaks on same coset */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  num_peaks = 0;
    /* No entries in best peaks array (yet) ... */

  weakest_inx = 0;
  weakest_eng = -1;
    /* ... so the weakest best peak is pretty weak. */


  /* For each search command in search list */

  for ( search = 0; search < num_searches; search++)
  {
    /* For each peak in the search results for that search command */

    for( peak = 0; peak < HDR_MAX_SRCH_PEAKS; peak++)
    {
      /* Select peaks stronger than the weakest of the strong */

      if ( list->srch_task.res_ptr->eng[peak] > weakest_eng )
      {
        /* We have found a peak strong enough to include in peaks[ ].
           Ensure it is not a multipath of another peak already selected */

        for ( i = 0; i < num_peaks; i++ )
        {
          if ( list->srch_task.pn_offset == best_peaks[i].coset )
          {
            /* Peak is on same coset - check if too close */


            dist_x2 = hdrsrchacq_distx2( best_peaks[i].posx2,
                                         list->srch_task.res_ptr->posx2[peak] );
              /* calculate the difference between the peak positions */

            if ( dist_x2 <= HDRSRCHACQ_PREDETECT_MIN_PEAK_DIFF_X2 )
            {
              /* Peak is too close - break out of loop */
              break;
            }
          }
        }

        /* If "i" reaches "num_peaks", then peak is unique */

        if ( i == num_peaks )
        {
          /* If there is room for one more, add to end of best_peaks */

          if ( num_peaks < max_peaks )
          {
            weakest_inx = num_peaks++;
          }

          /* Put new peak in best_peak[ ] array */

          best_peaks[ weakest_inx ].coset = list->srch_task.pn_offset;
          best_peaks[ weakest_inx ].eng   = list->srch_task.res_ptr->eng[ peak ];
          best_peaks[ weakest_inx ].posx2 = (uint16)list->srch_task.res_ptr->posx2[ peak ];

          /* If best_peak[ ] array is full, determine weakest */

          if ( num_peaks == max_peaks )
          {
            /* Determine new weakest peak */

            weakest_eng = best_peaks[0].eng;
              weakest_inx = 0;

            for ( i = 1; i < num_peaks ; i++ )
            {
              if ( best_peaks[i].eng < weakest_eng )
              {
                weakest_eng = best_peaks[i].eng;
                weakest_inx = i;
                  /* new index of the weakest */
              }
            }

          } /* If we needed to determine a new weakest */

        } /* If peak was unique */

      } /* If peak was strong */

    } /* For each peak */

    list++;

  } /* For each search command in search list */

  return num_peaks;

} /* hdrsrchacq_get_best_peaks( ) */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHACQ_GET_PILOT_ENERGY

DESCRIPTION
  Return the pilot energy from a search result buffer.

DEPENDENCIES
  None

INPUT
  res_ptr - The search result buffer.

RETURN VALUE
  Pilot energy.

SIDE EFFECTS
  None

==========================================================================*/

LOCAL uint32 hdrsrchacq_get_pilot_energy
(
  hdrsrchdrv_srch_res_struct_type   *res_ptr
    /* Energy peaks of search "pilot_idx". */
)
{
  uint32                            pilot_eng;
    /* Pilot energy */

  int                               peak;
    /* Peak loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Compute "Pilot Energy" for this pilot.
     Pilot Energy is calculated as: "E1 + E2/2 + E3/3 + E4/4"
       instead of (Mehmet's memo) : "E1"
       or (CR&D code)             : "E1 + E2 + E3 + E4"
       as a compromise between the two methods.
  */

  pilot_eng = 0;

  for( peak = 0; peak < HDR_MAX_SRCH_PEAKS; peak++ )
  {
    if ( res_ptr->eng[ peak ] >= HDRSRCHACQ_MIN_PATH_ENG )
    {
      pilot_eng += res_ptr->eng[ peak ] / (peak + 1);
    }
  }

  return pilot_eng;

} /* hdrsrchacq_get_pilot_energy() */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHACQ_FIND_STRONGEST_SECTOR

DESCRIPTION
  This function scans through the search list's result pointers and finds a
  pilot with maximum total pilot energy.

  This function returns index into the search list to the search which
  returned the strongest pilot.

DEPENDENCIES
  None

INPUT
  srch_list       - Search command list
  srch_cnt        - # of entries on search command list
  best_pilot_eng  - OUTPUT: Energy of best sector

RETURN VALUE
  Search item in the search list with strongest total engergy

SIDE EFFECTS
  None

==========================================================================*/

LOCAL int hdrsrchacq_find_strongest_sector
(
  hdrsrchdrv_srch_list_struct_type  *srch_list,
    /* Search list which was srched */

  uint8                             srch_cnt,
    /* Number of searches in the list */

  uint32                            *best_pilot_eng
    /* OUTPUT: Energy of the best pilot */
)
{
  int                               best_pilot_idx;
    /* Index of the best pilot */

  uint32                            energy;
    /* Pilot energy */

  int                               index;
    /* Search list loop index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Haven't found the best pilot yet... */
  *best_pilot_eng = 0;
  best_pilot_idx  = -1;


  for ( index = 0; index < srch_cnt; index++ )
  {
    energy = hdrsrchacq_get_pilot_energy( srch_list[ index ].srch_task.res_ptr );
      /* Get the pilot energy for this pilot */

    if ( energy > *best_pilot_eng )
    {
      *best_pilot_eng = energy;
      best_pilot_idx  = index;
    }
  }

  return best_pilot_idx;

} /* hdrsrchacq_find_strongest_sector() */


#ifdef FEATURE_XO
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_SUBMIT_PREDETECT_SEARCH

DESCRIPTION
  Programs a predetect search on a given coset group

DEPENDENCIES
  Searcher must be idle

INPUTS
  refresh       - Flag to control refresh of sample ram.

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_submit_predetect_search
(
  hdrsrchdrv_cap_ctrl_enum_type     refresh
    /* Refresh sample ram before starting predetect search, or not? */
)
{
  hdrsrchdrv_srch_list_struct_type  *cmd_ptr;
    /* Search list to be created */

  hdrsrchdrv_srch_res_struct_type   *srch_res;
    /* Location to save the search results */

  int                               sub_coset_idx;
    /* Index of the sub-coset, i.e., fraction, to search */

  int                               srch_cnt;
    /* Search Entries programmed in list */

  int                               coset_circle_fraction_cx2;
    /* Coset circle fraction in cx2 */

  int                               num_srch_per_coset;
    /* Number of searches to divide coset into */

  boolean                           div_search;
    /* With diversity or not */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "PREDETECT Depth=%d CosetGroup=%d ",
                  hdrsrchacq.depth, hdrsrchacq.coset_group );

  cmd_ptr = hdrsrchdrv.srch_list;
    /* Use global space from hdrsrchdrv to program search */

  srch_res = hdrsrchdrv.resbuf;
    /* Use global space from hdrsrchdrv for search results */

  srch_cnt = 0;
    /* No items yet */

  div_search = hdrsrchrfdiv_diversity_demod_is_enabled() ? TRUE : FALSE;
  hdrsrchdrv.srch_task_type = div_search ? HDRSRCH_DIVERSITY_SRCH : HDRSRCH_NON_DIV_SRCH ;

  /* Check if still searching in the middle of a coset group */
  if ( hdrsrchacq.coset >= HDRSRCHACQ_NUM_COSETS )
  {
    /* Finished searching last coset group.  Get first coset in next group */
    hdrsrchacq.coset = hdrsrchacq_coset_group_start[ hdrsrchacq.coset_group ]
                                                   [ hdrsrchacq.coset_idx ];
  }
    /* Get starting coset of the coset group */

  HDR_MSG_SRCH_6( MSG_LEGACY_HIGH,
                  "Starting coset for group %d coset_idx %d coset %d, pre-detect bin[%d]=%d ppm, freq offset %dHz",
                  hdrsrchacq.coset_group,
                  hdrsrchacq.coset_idx,
                  hdrsrchacq.coset,
                  hdrsrchacq.pre_detect_bin.idx,
                  hdrsrchacq.pre_detect_bin.bin[hdrsrchacq.pre_detect_bin.idx],
                  hdrsrchacq.pre_detect_bin.freq_offset );

  /* Calculate circle' fraction size using quarter circle cx2 as base.  8192
     window size matches quarter circle (cx1), so take a fraction of that for
     window sizes 2048 or 4096.
     Note: 2048, 4096 and 8192 are he supported window sizes, making even mod
     16 (max per search) arithmetic results assumed */
  coset_circle_fraction_cx2 = ( HDRSRCHACQ_QUARTER_CIRCLE_X2 *
                                hdrsrchacq.predetect.winsize ) / 8192;

  /* Calculate how many srch windows to divide coset circle given the predetect
     win size, e.g., for win size of 8092, circle is divided into quarters */
  num_srch_per_coset = 32768 / hdrsrchacq.predetect.winsize;

  /* Keep on looping if there are still cosets to search and the srch list is
     not full */
  while ( ( hdrsrchacq.coset < HDRSRCHACQ_NUM_COSETS ) &&
          ( srch_cnt < HDR_MAX_SRCH_CNT ) )
  {
    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH,
                "PREDETECT CosetGroup=%d CosetIdx=%d srch_cnt=%d win_size =%d",
                hdrsrchacq.coset_group, hdrsrchacq.coset_idx, srch_cnt,
                hdrsrchacq.predetect.winsize );

    /* Note: Since 2048, 4096 and 8192 are the only supported win sizes,
       whenever the srch list is full (16), the for loop will always be at
       a coset boundary, i.e., the next srch list will never begin in the
       middle of a coset. */
    for ( sub_coset_idx = 0; sub_coset_idx < num_srch_per_coset; sub_coset_idx++)
    {
      cmd_ptr->srch_task.rtc_offsetx2 = sub_coset_idx * coset_circle_fraction_cx2;
      HDR_MSG_SRCH_4( MSG_LEGACY_LOW,
                "PREDETECT coset=%d coset_idx=%d srch_cnt=%d rtc_offsetx2=%d",
                hdrsrchacq.coset, hdrsrchacq.coset_idx,
                srch_cnt, cmd_ptr->srch_task.rtc_offsetx2 );
      cmd_ptr->srch_task.pn_offset    = hdrsrchacq.coset;

      cmd_ptr->srch_task.win_size     = hdrsrchacq.predetect.winsize;
      cmd_ptr->srch_task.dual_ant     = div_search;

      cmd_ptr->srch_task.coh_int      = hdrsrchacq_coh_int[hdrsrchacq.num_iterations];

      cmd_ptr->srch_task.coh_trunc    = hdrsrchacq_coh_trunc;

      cmd_ptr->srch_task.carrier      = ( srch4_carrier_type )HDRSRCH_CARRIER_0;

      if ( cmd_ptr->srch_task.dual_ant )
      {
        /* Diversity parameter */
        cmd_ptr->srch_task.burst_length =
          hdrsrchacq_burst_len_div[hdrsrchacq.num_iterations];
        cmd_ptr->srch_task.noncoh_int   =
          hdrsrchacq_noncoh_int_div[hdrsrchacq.num_iterations];
      }
      else
      {
        /* Non-diversity parameter */
        cmd_ptr->srch_task.burst_length =
          hdrsrchacq_burst_len[hdrsrchacq.num_iterations];
        cmd_ptr->srch_task.noncoh_int   =
          hdrsrchacq_noncoh_int[hdrsrchacq.num_iterations];
      }

      cmd_ptr->srch_task.ant_sel      = ( boolean ) hdrsrchdrv.params.default_ant;

      cmd_ptr->sect_ptr               = NULL;
        /* Acq search: set the list sector pointer to null */

      cmd_ptr->srch_task.res_ptr      = srch_res;
        /* Set the result pointers */

      cmd_ptr++, srch_res++, srch_cnt++;
        /* Point to next search & result items, & keep count of searches */

      /* Exit for loop if maximum searches have been reached/setup */
      if (srch_cnt >= HDR_MAX_SRCH_CNT)
      {
        break;
      }
    }


    /* Increment to the next coset and check if the coset group is complete */
    hdrsrchacq.coset_idx++;
    hdrsrchacq.coset += HDRSRCHACQ_COSET_GRP_INC;

    if (hdrsrchacq.coset_idx >= HDRSRCHACQ_NUM_COSETS_PER_GRP)
    {
      /* Reset coset idx. */
      hdrsrchacq.coset_idx = 0;

      hdrsrchacq.coset_group_searched = TRUE;
        /* The current coset group has been searched */

      /* coset group is not incremented here since we may search next bin for the same coset group */
      //hdrsrchacq.coset_group++;
      break; /* Add all coset in this group */
    }
  }

  DEBUG_PREDETECT_SRCH(srch_cnt)

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  hdrsrchdrv_send_srch_list( hdrsrchdrv.srch_list,
                             refresh, srch_cnt,
                             HDRSRCH_SYSTYPE_DO,
                             hdrsrchacq.pre_detect_bin.freq_offset,
                             hdrsrchutil_do_atomic_search()
                           );
    /* Submit predetect search list to search driver */

} /* hdrsrchacq_submit_predetect_search( ) */
#else
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_SUBMIT_PREDETECT_SEARCH

DESCRIPTION
  Programs a predetect search on a given coset group

DEPENDENCIES
  Searcher must be idle

INPUTS
  refresh       - Flag to control refresh of sample ram.

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_submit_predetect_search
(
  hdrsrchdrv_cap_ctrl_enum_type     refresh
    /* Refresh sample ram before starting predetect search, or not? */
)
{
  hdrsrchdrv_srch_list_struct_type  *cmd_ptr;
    /* Search list to be created */

  hdrsrchdrv_srch_res_struct_type   *srch_res;
    /* Location to save the search results */

  int                               coset = 0;
    /* Coset the search is programmed on */

  int                               quarter;
    /* Quarter of the coset to search */

  int                               srch_cnt;
    /* Search Entries programmed in list */

  boolean                           div_search;
    /* With diversity or not */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_2( MSG_LEGACY_LOW, "PREDETECT Depth=%d CosetGroup=%d ",
                  hdrsrchacq.depth, hdrsrchacq.coset_group );

  if ( hdrsrchacq.depth >= HDRSRCHACQ_NUM_DEPTHS ) 
  {
    /* This is not expected */
    ERR_FATAL( "depth %d is too large", hdrsrchacq.depth, 0, 0 );
  }

  cmd_ptr = hdrsrchdrv.srch_list;
    /* Use global space from hdrsrchdrv to program search */

  srch_res = hdrsrchdrv.resbuf;
    /* Use global space from hdrsrchdrv for search results */

  srch_cnt = 0;
    /* No items yet */

  div_search = hdrsrchrfdiv_diversity_demod_is_enabled() ? TRUE : FALSE;
  hdrsrchdrv.srch_task_type = div_search ? HDRSRCH_DIVERSITY_SRCH : HDRSRCH_NON_DIV_SRCH ;	

  if ( hdrsrchacq.coset_group < HDRSRCHACQ_NUM_COSET_GROUPS ) 
  {
    coset = hdrsrchacq_coset_group_start[ hdrsrchacq.coset_group ]
                                        [ hdrsrchacq.coset_idx ];
      /* Get starting coset of the coset group */
  }
  else
  {
    /* This is not expected */
    ERR_FATAL( "coset_group %d is too large", hdrsrchacq.coset_group, 0, 0 );

  }

  HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "coset_idx %d coset %d",
                                    hdrsrchacq.coset_idx, coset );

  while ( coset < HDRSRCHACQ_NUM_COSETS )
  {
    for ( quarter = 0; quarter < 4; quarter++)
    {
      cmd_ptr->srch_task.rtc_offsetx2 = quarter * HDRSRCHACQ_QUARTER_CIRCLE_X2;
      cmd_ptr->srch_task.pn_offset    = coset;

      cmd_ptr->srch_task.win_size     = HDRSRCHACQ_PREDETECT_WINSIZE;

      cmd_ptr->srch_task.dual_ant     = div_search;

      cmd_ptr->srch_task.coh_int      = hdrsrchacq_coh_int[hdrsrchacq.depth];

      cmd_ptr->srch_task.coh_trunc    = hdrsrchacq_coh_trunc;

      cmd_ptr->srch_task.carrier      = (srch4_carrier_type)HDRSRCH_CARRIER_0;

      if ( cmd_ptr->srch_task.dual_ant )
      {
        /* Diversity parameter */
        cmd_ptr->srch_task.burst_length = hdrsrchacq_burst_len_div[hdrsrchacq.depth];
        cmd_ptr->srch_task.noncoh_int   = hdrsrchacq_noncoh_int_div[hdrsrchacq.depth];
      }
      else
      {
        /* Non-diversity parameter */
        cmd_ptr->srch_task.burst_length = hdrsrchacq_burst_len[hdrsrchacq.depth];
        cmd_ptr->srch_task.noncoh_int   = hdrsrchacq_noncoh_int[hdrsrchacq.depth];
      }

      cmd_ptr->srch_task.ant_sel      = ( boolean ) hdrsrchdrv.params.default_ant;

      cmd_ptr->sect_ptr               = NULL;
        /* Acq search: set the list sector pointer to null */

      cmd_ptr->srch_task.res_ptr      = srch_res;
        /* Set the result pointers */


      cmd_ptr++, srch_res++, srch_cnt++;
        /* Point to next search & result items, & keep count of searches */
    }

    coset += HDRSRCHACQ_COSET_GRP_INC;
  }

  DEBUG_PREDETECT_SRCH(srch_cnt)

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  hdrsrchdrv_send_srch_list( hdrsrchdrv.srch_list,
                             refresh,
                             srch_cnt,
                             HDRSRCH_SYSTYPE_DO,
                             0,
                             hdrsrchutil_do_atomic_search()
                           );
    /* Submit predetect search list to search driver */

} /* hdrsrchacq_submit_predetect_search( ) */

#endif /* FEATURE_XO */

#ifdef FEATURE_XO
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_SUBMIT_DETECT_SEARCH

DESCRIPTION
  XO version:
  Program small searches around the best peaks detected in predetect using
  binning.

DEPENDENCIES
  hdrsrchacq.predetect filled in with valid information.

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_submit_detect_search( void )
{

  hdrsrchdrv_srch_list_struct_type  *cmd_ptr;
    /* search list which was srched */

  hdrsrchdrv_srch_res_struct_type   *srch_res;
    /* Location to save the search results */

  int                               srch_cnt;
    /* Search Entries programmed in list */

  boolean                           div_search;
    /* With diversity or not */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
              "DETECT remaining predetect peaks=%d predetect.peak_idx=%d iteration=%d ",
              hdrsrchacq.predetect.num_peaks_remaining,
              hdrsrchacq.predetect.peak_idx,
              hdrsrchacq.num_iterations );

  cmd_ptr = hdrsrchdrv.srch_list;
    /* Use global space from hdrsrchdrv to program search */

  srch_res = hdrsrchdrv.resbuf;
    /* Use global space from hdrsrchdrv for search results */

  div_search = hdrsrchrfdiv_diversity_demod_is_enabled() ? TRUE : FALSE;
  hdrsrchdrv.srch_task_type = div_search ? HDRSRCH_DIVERSITY_SRCH : HDRSRCH_NON_DIV_SRCH ;

  /* Search a small window around each peak in the best_peak[ ] array */

  for ( srch_cnt = 0; srch_cnt < HDR_MAX_SRCH_CNT ; srch_cnt++ )
  {
    /*  For MSM6500, we program the "start" of the search window to DSP,
        instead of the center. Go back by half search window. Note chipx2 units */
    cmd_ptr->srch_task.rtc_offsetx2 = 
      (hdrsrchacq.predetect.peaks[hdrsrchacq.predetect.peak_idx].posx2
       - HDRSRCHACQ_DETECT_WINSIZE ) & 0xffff;

    cmd_ptr->srch_task.pn_offset    =
      hdrsrchacq.predetect.peaks[hdrsrchacq.predetect.peak_idx].coset;

    cmd_ptr->srch_task.win_size     = hdrsrchacq.detect.winsize;

    HDR_MSG_SRCH_3( MSG_LEGACY_LOW,
              "DETECT pn_offset=%d srch_cnt=%d rtc_offsetx2=%d",
              cmd_ptr->srch_task.pn_offset, srch_cnt, cmd_ptr->srch_task.rtc_offsetx2 );

    cmd_ptr->srch_task.ant_sel      = ( boolean ) hdrsrchdrv.params.default_ant;
    cmd_ptr->srch_task.dual_ant     = div_search;

    cmd_ptr->srch_task.coh_int      = hdrsrchacq_coh_int[hdrsrchacq.num_iterations];

    cmd_ptr->srch_task.coh_trunc    = hdrsrchacq_coh_trunc;

    cmd_ptr->srch_task.carrier      = ( srch4_carrier_type )HDRSRCH_CARRIER_0;

    if ( cmd_ptr->srch_task.dual_ant )
    {
      cmd_ptr->srch_task.burst_length = hdrsrchacq_burst_len_div[hdrsrchacq.num_iterations];
      cmd_ptr->srch_task.noncoh_int   = hdrsrchacq_noncoh_int_div[hdrsrchacq.num_iterations];
    }
    else
    {
      cmd_ptr->srch_task.burst_length = hdrsrchacq_burst_len[hdrsrchacq.num_iterations];
      cmd_ptr->srch_task.noncoh_int   = hdrsrchacq_noncoh_int[hdrsrchacq.num_iterations];
    }

    cmd_ptr->sect_ptr     = NULL;
    cmd_ptr->srch_task.res_ptr      = srch_res;

    cmd_ptr++;
    srch_res++;
    hdrsrchacq.predetect.peak_idx++;

    hdrsrchacq.predetect.num_peaks_remaining--;
      /* Update remaining number of remaining predetect peaks to search */

    if (hdrsrchacq.predetect.num_peaks_remaining <= 0)
    {
      /* Finished with predetect peaks.
         Increment srch_cnt since we are leaving loop early with no top of
         loop increment. */
      srch_cnt++;
      break;
    }
  }

  DEBUG_DETECT_SRCH(srch_cnt)

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  hdrsrchdrv_send_srch_list( hdrsrchdrv.srch_list,
                             REFRESH_BEFORE,
                             srch_cnt,
                             HDRSRCH_SYSTYPE_DO,
                             hdrsrchacq.freq_offset,
                             hdrsrchutil_do_atomic_search()
                           );
    /* Send detect search list to the hardware */

  HDR_MSG_SRCH_6( MSG_LEGACY_HIGH,
                  "DETECT # of srches=%d  bin freq offset=%d (%d ppm), predetect bin freq offset=%d [%d ppm], total freq offset %d(Hz)",
              srch_cnt,
                  hdrsrchacq.detect_bin.freq_offset,
                  hdrsrchacq.detect_bin.bin[hdrsrchacq.detect_bin.idx],
                  hdrsrchacq.pre_detect_bin.freq_offset,
                  hdrsrchacq.pre_detect_bin.bin[hdrsrchacq.pre_detect_bin.idx],
                  hdrsrchacq.freq_offset );

  } /* hdrsrchacq_submit_detect_search() */

#else

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_SUBMIT_DETECT_SEARCH

DESCRIPTION
  Program small searches around the best peaks detected in predetect.

DEPENDENCIES
  hdrsrchacq.predetect filled in with valid information.

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_submit_detect_search( void )
{

  hdrsrchdrv_srch_list_struct_type  *cmd_ptr;
    /* search list which was srched */

  hdrsrchdrv_srch_res_struct_type   *srch_res;
    /* Location to save the search results */

  int                               srch_cnt;
    /* Search Entries programmed in list */

  boolean                           div_search;
    /* With diversity or not */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DETECT Depth=%d CosetGroup=%d ",
                  hdrsrchacq.depth, hdrsrchacq.coset_group );

  if ( hdrsrchacq.depth >= HDRSRCHACQ_NUM_DEPTHS ) 
  {
    /* This is not expected */
    ERR_FATAL( "depth %d is too large", hdrsrchacq.depth, 0, 0 );
  }

  cmd_ptr = hdrsrchdrv.srch_list;
    /* Use global space from hdrsrchdrv to program search */

  srch_res = hdrsrchdrv.resbuf;
    /* Use global space from hdrsrchdrv for search results */

  div_search = hdrsrchrfdiv_diversity_demod_is_enabled() ? TRUE : FALSE;
  hdrsrchdrv.srch_task_type = div_search ? HDRSRCH_DIVERSITY_SRCH : HDRSRCH_NON_DIV_SRCH ;

  /* Search a small window around each peak in the best_peak[ ] array */

  for ( srch_cnt = 0; srch_cnt < hdrsrchacq.predetect.num_peaks ; srch_cnt++ )
  {
    /*  For MSM6500, we program the "start" of the search window to DSP,
        instead of the center. Go back by half search window. Note chipx2 units */
    cmd_ptr->srch_task.rtc_offsetx2 = (hdrsrchacq.predetect.peaks[srch_cnt].posx2
                              - HDRSRCHACQ_DETECT_WINSIZE ) & 0xffff;

    cmd_ptr->srch_task.pn_offset    = hdrsrchacq.predetect.peaks[srch_cnt].coset;
    cmd_ptr->srch_task.win_size     = hdrsrchacq.detect.winsize;

    cmd_ptr->srch_task.ant_sel      = ( boolean ) hdrsrchdrv.params.default_ant;
    cmd_ptr->srch_task.dual_ant     = div_search;

    cmd_ptr->srch_task.coh_int      = hdrsrchacq_coh_int[hdrsrchacq.depth];

    cmd_ptr->srch_task.coh_trunc    = hdrsrchacq_coh_trunc;

    cmd_ptr->srch_task.carrier      = (srch4_carrier_type)HDRSRCH_CARRIER_0;

    if ( cmd_ptr->srch_task.dual_ant )
    {
      cmd_ptr->srch_task.burst_length = hdrsrchacq_burst_len_div[hdrsrchacq.depth];
      cmd_ptr->srch_task.noncoh_int   = hdrsrchacq_noncoh_int_div[hdrsrchacq.depth];
    }
    else
    {
      cmd_ptr->srch_task.burst_length = hdrsrchacq_burst_len[hdrsrchacq.depth];
      cmd_ptr->srch_task.noncoh_int   = hdrsrchacq_noncoh_int[hdrsrchacq.depth];
    }

    cmd_ptr->sect_ptr     = NULL;
    cmd_ptr->srch_task.res_ptr      = srch_res;

    cmd_ptr++;
    srch_res++;
  }

  DEBUG_DETECT_SRCH(srch_cnt)

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  hdrsrchdrv_send_srch_list( hdrsrchdrv.srch_list,
                             REFRESH_BEFORE,
                             srch_cnt,
                             HDRSRCH_SYSTYPE_DO,
                             0,
                             hdrsrchutil_do_atomic_search()
                           );
    /* Send detect search list to the hardware */

} /* hdrsrchacq_submit_detect_search() */

#endif /* FEATURE_XO */

#ifdef FEATURE_XO
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_SUBMIT_ANTI_ALIAS_SEARCH

DESCRIPTION
  Program small searches around the best peak detected in detect stage
  at -1 and +1.

DEPENDENCIES
  hdrsrchacq.detect filled in with valid information.
  hdrsrchacq.detect_bin.best_peak_freq_offset for binning frequency offset

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_submit_anti_alias_search( void )
{

  hdrsrchdrv_srch_list_struct_type  *cmd_ptr;
    /* search list which was srched */

  int                               srch_cnt;
    /* Search Entries programmed in list */

  boolean                           div_search;
    /* With diversity or not */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  cmd_ptr = hdrsrchdrv.srch_list;
    /* Use global space from hdrsrchdrv to program search */

  div_search = hdrsrchrfdiv_diversity_demod_is_enabled() ? TRUE : FALSE;
  hdrsrchdrv.srch_task_type = div_search ? HDRSRCH_DIVERSITY_SRCH : HDRSRCH_NON_DIV_SRCH ;

  /* Anti-alias searches reuse previous search commands
     For XO, only anti-alias best detect peak at -1 and +1 */
  for ( srch_cnt = 0; srch_cnt < 2 ; srch_cnt++ )
  {
    /* only fill in the parameters which have changed from detect search */

    /*---------------------------------------------------------------------
      If you add 1 to coset number, subtract 64 from the RTC offset
      If you subtract 1 from coset number, add 64 to the RTC offset

      To move from coset 0 to coset 15, add 1024 chipx1 to RTC offset
      To move from coset 15 to coset 0, subtract 1024 chipx1 from RTC offset
     *---------------------------------------------------------------------*/


    cmd_ptr->srch_task.rtc_offsetx2 = ( (uint16)hdrsrchacq.detect.paths.posx2[0]
                             - (hdrsrchacq.anti_alias * 64 * 2) );
      /* Move RTC offset by +/- 1 coset */

    cmd_ptr->srch_task.pn_offset    = ( hdrsrchacq.detect.coset
                               + hdrsrchacq.anti_alias ) & 15;
      /* And switch search coset */

    cmd_ptr->srch_task.dual_ant     = div_search;

    if ( hdrsrchacq.detect.coset == 0
         && hdrsrchacq.anti_alias == -1 )
    {
      /* Need to roll RTC offset back to coset 15 */
      cmd_ptr->srch_task.rtc_offsetx2 -= (1024) * 2;
    }

    if ( hdrsrchacq.detect.coset == 15
         && hdrsrchacq.anti_alias == +1 )
    {
      /* Need to roll RTC offset forward to coset 0 */
      cmd_ptr->srch_task.rtc_offsetx2 += (1024) * 2;
    }

    HDR_MSG_SRCH_4( MSG_LEGACY_HIGH, "ANTIALIAS %d CosetGroup=%d, best peak offset=%d ",
                    hdrsrchacq.anti_alias, 
                    hdrsrchacq.coset_group,
                    hdrsrchacq.anti_alias,
                    hdrsrchacq.best_peak_freq_offset );

    cmd_ptr++;
    hdrsrchacq.anti_alias = 1;
      /* Check other side of best peak */
  }

  DEBUG_ANTI_ALIAS_SRCH(srch_cnt, hdrsrchacq.anti_alias)

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  hdrsrchdrv_send_srch_list( hdrsrchdrv.srch_list,
                             NO_REFRESH,
                             srch_cnt,
                             HDRSRCH_SYSTYPE_DO,
                             hdrsrchacq.best_peak_freq_offset,
                             hdrsrchutil_do_atomic_search()
                             );
    /* Send anti-alias search list to the hardware */

} /* hdrsrchacq_submit_anti_alias_search() */

#else
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_SUBMIT_ANTI_ALIAS_SEARCH

DESCRIPTION
  Program small searches around the best peaks detected in predetect.

DEPENDENCIES
  hdrsrchacq.predetect filled in with valid information.

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_submit_anti_alias_search( void )
{

  hdrsrchdrv_srch_list_struct_type  *cmd_ptr;
    /* search list which was srched */

  int                               srch_cnt;
    /* Search Entries programmed in list */

  boolean                           div_search;
    /* With diversity or not */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_3( MSG_LEGACY_LOW, "ANTIALIAS Depth=%d CosetGroup=%d,%d ",
                  hdrsrchacq.depth, 
                  hdrsrchacq.coset_group, 
                  hdrsrchacq.anti_alias);

  cmd_ptr = hdrsrchdrv.srch_list;
    /* Use global space from hdrsrchdrv to program search */

  div_search = hdrsrchrfdiv_diversity_demod_is_enabled() ? TRUE : FALSE;
  hdrsrchdrv.srch_task_type = div_search ? HDRSRCH_DIVERSITY_SRCH : HDRSRCH_NON_DIV_SRCH ;

  /* Anti-alias searches reuse previous search commands */

  for ( srch_cnt = 0; srch_cnt < hdrsrchacq.predetect.num_peaks ; srch_cnt++ )
  {
    /* only fill in the parameters which have changed from detect search */

    /*---------------------------------------------------------------------
      If you add 1 to coset number, subtract 64 from the RTC offset
      If you subtract 1 from coset number, add 64 to the RTC offset

      To move from coset 0 to coset 15, add 1024 chipx1 to RTC offset
      To move from coset 15 to coset 0, subtract 1024 chipx1 from RTC offset
     *---------------------------------------------------------------------*/


    cmd_ptr->srch_task.rtc_offsetx2 = hdrsrchacq.predetect.peaks[srch_cnt].posx2
                             - hdrsrchacq.anti_alias * 64 * 2;
      /* Move RTC offset by +/- 1 coset */

    cmd_ptr->srch_task.pn_offset    = ( hdrsrchacq.predetect.peaks[srch_cnt].coset
                               + hdrsrchacq.anti_alias ) & 15;
      /* And switch search coset */

    cmd_ptr->srch_task.dual_ant     = div_search;

    if ( hdrsrchacq.predetect.peaks[srch_cnt].coset == 0
         && hdrsrchacq.anti_alias == -1 )
    {
      /* Need to roll RTC offset back to coset 15 */
      cmd_ptr->srch_task.rtc_offsetx2 -= (1024) * 2;
    }

    if ( hdrsrchacq.predetect.peaks[srch_cnt].coset == 15
         && hdrsrchacq.anti_alias == +1 )
    {
      /* Need to roll RTC offset forward to coset 0 */
      cmd_ptr->srch_task.rtc_offsetx2 += (1024) * 2;
    }

    cmd_ptr++;
  }

  DEBUG_ANTI_ALIAS_SRCH(srch_cnt, hdrsrchacq.anti_alias)

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  hdrsrchdrv_send_srch_list( hdrsrchdrv.srch_list,
                             NO_REFRESH,
                             srch_cnt,
                             HDRSRCH_SYSTYPE_DO,
                             0,
                             hdrsrchutil_do_atomic_search()
                           );
    /* Send anti-alias search list to the hardware */

} /* hdrsrchacq_submit_anti_alias_search() */

#endif /* FEATURE_XO */


#ifdef FEATURE_XO
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_INIT_PRE_DETECT_BIN

DESCRIPTION
  Acquisition State Bin Initialization

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/

LOCAL void hdrsrchacq_init_pre_detect_bin( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchacq.coset_group_searched = FALSE;
    /* Reset coset group for pre-detect */

  hdrsrchacq.pre_detect_bin.idx = 0;
    /* Reset binning index */

  if ( hdrsrchacq.pre_detect_bin.freq_uncertainty_ppm <= 5 )
  {
    /* 0 - 5ppm */
    hdrsrchacq.pre_detect_bin.bin = hdrsrchacq_pre_detect_5ppm_bin;
    hdrsrchacq.pre_detect_bin.max_idx = HDRSRCHACQ_PRE_DETECT_5PPM_NUM_BINS -1;
  }
  else
  if ( hdrsrchacq.pre_detect_bin.freq_uncertainty_ppm <= 15 )
  {
    /* 5 - 15ppm */
    hdrsrchacq.pre_detect_bin.bin = hdrsrchacq_pre_detect_15ppm_bin;
    hdrsrchacq.pre_detect_bin.max_idx = HDRSRCHACQ_PRE_DETECT_15PPM_NUM_BINS -1;
  }
  else
  {
    /* > 15ppm */
    hdrsrchacq.pre_detect_bin.bin = hdrsrchacq_pre_detect_20ppm_bin;
    hdrsrchacq.pre_detect_bin.max_idx = HDRSRCHACQ_PRE_DETECT_20PPM_NUM_BINS -1;
  }
  
  hdrsrchacq.pre_detect_bin.freq_offset = HDRSRCHACQ_PRE_DETECT_BIN_OFFSET_HZ;
    /* freq offset */

  HDR_MSG_SRCH_5( MSG_LEGACY_HIGH,                              
                  "Pre-detect freq uncertainty %d ppm, max bin %d, bin freq offset=%d pre_detect_bin.bin[%d]=%d ppm",    
                  hdrsrchacq.pre_detect_bin.freq_uncertainty_ppm,
                  hdrsrchacq.pre_detect_bin.max_idx,
                  hdrsrchacq.pre_detect_bin.freq_offset,                       
                  hdrsrchacq.pre_detect_bin.idx,                               
                  hdrsrchacq.pre_detect_bin.bin[hdrsrchacq.pre_detect_bin.idx] );

}/* hdrsrchacq_init_pre_detect_bin */

/*==========================================================================

FUNCTION        HDRSRCHACQ_INIT_DETECT_BIN

DESCRIPTION
  Acquisition State Bin Initialization

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/

LOCAL void hdrsrchacq_init_detect_bin( void )
{

  sys_channel_type          band_chan;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Depending on XO mode, set the max index to the number of bins -1 for
     0-based indexing */
  if (  hdrsrchacq.detect_bin.xo_cal_mode == HDRSRCHACQ_RGS_MODE )
  {
    /* RGS mode */
    hdrsrchacq.detect_bin.max_idx =
      sizeof(hdrsrchacq_bin_rgs)/sizeof(hdrsrchacq_bin_rgs[0]) - 1;
    hdrsrchacq.detect_bin.units   = HDRSRCHACQ_UNIT_1_PPM;
    hdrsrchacq.detect_bin.bin     = hdrsrchacq_bin_rgs;
  }
  else if (  hdrsrchacq.detect_bin.xo_cal_mode == HDRSRCHACQ_FT2_MODE )
  {
    /* FT2 mode */
    hdrsrchacq.detect_bin.max_idx =
      sizeof(hdrsrchacq_bin_ft2)/sizeof(hdrsrchacq_bin_ft2[0]) - 1;
    hdrsrchacq.detect_bin.units   = HDRSRCHACQ_UNIT_1_PPM;
    hdrsrchacq.detect_bin.bin     = hdrsrchacq_bin_ft2;
  }
  else if ( hdrsrchacq.detect_bin.xo_cal_mode == HDRSRCHACQ_FT1_MODE )
  {
    /* FT1 mode */
    hdrsrchacq.detect_bin.max_idx =
      sizeof(hdrsrchacq_bin_ft1)/sizeof(hdrsrchacq_bin_ft1[0]) - 1;
    hdrsrchacq.detect_bin.units   = HDRSRCHACQ_UNIT_1_5_PPM;
    hdrsrchacq.detect_bin.bin     = hdrsrchacq_bin_ft1;
  }
  else
  {
    /* FT Wide mode or Unrecognized XO mode.  Use FT Wide mode as default */
    hdrsrchacq.detect_bin.max_idx =
        sizeof(hdrsrchacq_bin_ft_wide)/sizeof(hdrsrchacq_bin_ft_wide[0]) - 1;
    hdrsrchacq.detect_bin.units   = HDRSRCHACQ_UNIT_1_5_PPM;
    hdrsrchacq.detect_bin.bin     = hdrsrchacq_bin_ft_wide;
  }

  hdrsrchacq.detect_bin.idx = 0;
    /* Start with the 1st bin */

  hdrsrchacq.detect_bin.freq_offset = HDRSRCHACQ_BIN_OFFSET_HZ;
    /* Convert bin's PPM value to frequency offset in Hz */

  hdrsrchacq.freq_offset = hdrsrchacq.detect_bin.freq_offset + hdrsrchacq.pre_detect_bin.freq_offset;
    /* Update freq offset */

  band_chan.band            = hdrsrchacq.params.channel.band;
  band_chan.chan_num        = hdrsrchacq.params.channel.chan_num;
  HDR_MSG_SRCH_6( MSG_LEGACY_HIGH,                              
                  "xo mode %d max bin %d bin freq offset=%d detect_bin.bin[%d]=%d ppm carrier freq=%u",    
                  hdrsrchacq.detect_bin.xo_cal_mode,
                  hdrsrchacq.detect_bin.max_idx,
                  hdrsrchacq.detect_bin.freq_offset,                       
                  hdrsrchacq.detect_bin.idx,                               
                  hdrsrchacq.detect_bin.bin[hdrsrchacq.detect_bin.idx],           
                  rfm_cdma_get_rx_carrier_freq( band_chan ) );

} /* hdrsrchacq_init_detect_bin */

#endif /* FEATURE_XO */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_BRUTE_FORCE_ACQ_PREDETECT_SEARCH

DESCRIPTION
  Programs a predetect search for brute force acquisition.

DEPENDENCIES
  Searcher must be idle

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

void hdrsrchacq_brute_force_acq_predetect_search( void )
{
  hdrsrchdrv_srch_list_struct_type  *cmd_ptr;
    /* Search list to be created */

  hdrsrchdrv_srch_res_struct_type   *srch_res;
    /* Location to save the search results */

  int                               quarter;
    /* Quarter of the coset to search */

  int                               srch_cnt;
    /* Search Entries programmed in list */

  int                               pn;
    /* PN index */

  boolean                           div_search;
    /* With diversity or not */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_SMALL_SRCH_WINDOW
  if ( HDRSRCHACQ_MAX_SLICE >= HDR_MAX_SRCH_CNT )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Too small SRCH window size %d",
        HDRSRCHACQ_PREDETECT_WINSIZE);
    return;
  }
#endif /* FEATURE_HDR_SMALL_SRCH_WINDOW */    

  hdrsrchacq.stage1 = HDRSRCHACQ_S1_PREDETECT;
    /* Predetect stage */

  cmd_ptr = hdrsrchdrv.srch_list;
    /* Use global space from hdrsrchdrv to program search */

  srch_res = hdrsrchdrv.resbuf;
    /* Use global space from hdrsrchdrv for search results */

  srch_cnt = 0;
    /* No items yet */

  div_search = hdrsrchrfdiv_diversity_demod_is_enabled() ? TRUE : FALSE;
  hdrsrchdrv.srch_task_type = div_search ? HDRSRCH_DIVERSITY_SRCH : HDRSRCH_NON_DIV_SRCH ;	

  /* Search n cosets (m searches) for each predetect search
   * where m = max ( HDRSRCHACQ_MAX_SLICE, max_srch_task_count)
   * n = floor (max_srch_task_count/srch_task_count_per_coset)
   */
  pn = hdrsrchacq.brute_force_acq.coset_idx;

  while ( ( pn < hdrsrchacq.brute_force_acq.coset_cnt ) && 
      ( srch_cnt < HDR_MAX_SRCH_CNT ) )
  {
#ifdef FEATURE_HDR_SMALL_SRCH_WINDOW
    for ( quarter = 0; quarter < HDRSRCHACQ_MAX_SLICE; quarter++)
#else
    for ( quarter = 0; quarter < 4; quarter++)
#endif /* FEATURE_HDR_SMALL_SRCH_WINDOW */    
    {
      cmd_ptr->srch_task.pn_offset    =
          hdrsrchacq.brute_force_acq.coset_list[pn].coset;
#ifdef FEATURE_HDR_SMALL_SRCH_WINDOW
      cmd_ptr->srch_task.rtc_offsetx2 =
          quarter * HDRSRCHACQ_PREDETECT_WINSIZE * 2;
#else
      cmd_ptr->srch_task.rtc_offsetx2 =
          quarter * HDRSRCHACQ_QUARTER_CIRCLE_X2;
#endif /* FEATURE_HDR_SMALL_SRCH_WINDOW */    
      cmd_ptr->srch_task.win_size     = HDRSRCHACQ_PREDETECT_WINSIZE;
      cmd_ptr->srch_task.dual_ant     = div_search;
      cmd_ptr->srch_task.coh_int      = hdrsrchacq_coh_int[hdrsrchacq.depth];
      cmd_ptr->srch_task.coh_trunc    = hdrsrchacq_coh_trunc;
      cmd_ptr->srch_task.carrier      = (srch4_carrier_type)HDRSRCH_CARRIER_0;

      if ( cmd_ptr->srch_task.dual_ant )
      {
        /* Diversity parameter */
        cmd_ptr->srch_task.burst_length =
            hdrsrchacq_burst_len_div[hdrsrchacq.depth];
        cmd_ptr->srch_task.noncoh_int   =
            hdrsrchacq_noncoh_int_div[hdrsrchacq.depth];
      }
      else
      {
        /* Non-diversity parameter */
        cmd_ptr->srch_task.burst_length =
            hdrsrchacq_burst_len[hdrsrchacq.depth];
        cmd_ptr->srch_task.noncoh_int   =
            hdrsrchacq_noncoh_int[hdrsrchacq.depth];
      }

      cmd_ptr->srch_task.ant_sel      =
          ( boolean ) hdrsrchdrv.params.default_ant;

      cmd_ptr->sect_ptr     = NULL;
        /* Acq search: set the list sector pointer to null */

      cmd_ptr->srch_task.res_ptr      = srch_res;
        /* Set the result pointers */

#ifdef FEATURE_HDR_SMALL_SRCH_WINDOW
      HDR_MSG_SRCH_6( MSG_LEGACY_LOW,
        "IRAT: PN %d qrtr %d pn_off %d rtc_off %d ws %d ms %d",
        pn,
        quarter,
        cmd_ptr->srch_task.pn_offset,
        cmd_ptr->srch_task.rtc_offsetx2,
        cmd_ptr->srch_task.win_size,
        HDRSRCHACQ_MAX_SLICE);
#endif /* FEATURE_HDR_SMALL_SRCH_WINDOW */    

      cmd_ptr++, srch_res++, srch_cnt++;
        /* Point to next search & result items, & keep count of searches */
    } /* for */

   HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR Brute-Force Acq search coset: %d ",
                   hdrsrchacq.brute_force_acq.coset_list[pn].coset);
      pn++;
    } /* while */

  hdrsrchacq.brute_force_acq.coset_idx = pn ;
    /* Increment coset_idx of 4 for remaining coset if any left */

  hdrsrchdrv_set_srch4_param( SRCH4_MODE_DEFAULT, 0 );
    /* Set Srch4 param to default values */

  srch_cnt = MIN ( srch_cnt , HDR_MAX_SRCH_CNT );
    /* KW fix : Limit the srch_cnt to HDR_MAX_SRCH_CNT */

  hdrsrchdrv_send_srch_list( hdrsrchdrv.srch_list,
                             NO_REFRESH,
                             srch_cnt,
                             HDRSRCH_SYSTYPE_DO,
                             0,
                             hdrsrchutil_do_atomic_search()
                           );
    /* Submit predetect search list to search driver */
} /* hdrsrchacq_brute_force_acq_predectect_search */


/*==========================================================================

FUNCTION        HDRSRCHACQ_BRUTE_FORCE_TUNE_DONE

DESCRIPTION
  Brute force acquisition RF tune done processing. Once the RF has finished 
  tuning, we can start searching.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

void hdrsrchacq_brute_force_tune_done
( 
  hdrsrchrf_tune_status_enum_type status
)
{
  if ( status != HDRSRCHRF_TUNE_SUCCESS )
{
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
    return;
  }

#ifdef FEATURE_XO
  hdrsrchacq_init_pre_detect_bin();
    /* Intialize pre-detect binning */

  hdrsrchacq_init_detect_bin();
    /* Initialize detect binning */
#endif /* FEATURE_XO */

  hdrsrchacq_brute_force_acq_predetect_search( );
    /* Now that we are here, let's search!  */

  hdrsrch_add_to_wait_mask( HDRSRCH_DUMP_SIG | HDRSRCH_LOSTDUMP_SIG );
    /* Wait for Dump signals, and perhaps Lost Dump signals */

} /* hdrsrchacq_brute_force_tune_done */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_START_ACQ

DESCRIPTION
  This function starts the acquisition process by initializing the RF
  and decoder hardware and tuning the radio.

DEPENDENCIES
  Called when all required RF and TCXO resources are available to start
  HDR acquisition.

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchacq_start_acq( void )
{

#ifdef FEATURE_HDR_REVB
  hdrsrch_demod_idx_type   acq_demod_idx = HDRSRCH_DEMOD_IDX_0;
    /* Dictates the demod index used in the HDR acquisition. */
#endif /* FEATURE_HDR_REVB */

  uint32                   delay_rf_tune_us = 0;
    /* Delay to tune RF to aovid collision with 1x paging */

  boolean tune_status;
    /* Stores the status of tune to chan */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchacq.acq_init_pending )
  {
    hdrsrchacq.acq_init_pending = FALSE;
      /* We are performing one time initialization of the acq state */

    hdrsrchacq_init_on_rf_lock();
      /* perform the acquisition initialization */
  }

  hdrsrchdrv_enable_srch4( SRCH4_PRIO_HDR_ACQ );
    /* Enable SRCH4 for HDR */

  /* RF resources have been acquired and locked.  We can now tune to the
     the requested channel for acquisition */

  hdrsrchacq_start_timer( &hdrsrchacq.timer.task );
    /* Start recording "Virtual" time for acquisition */

  hdrdec_activate();
    /* Enable HDR decoder */

  hdrenc_init();
    /* Enable HDR encode and modulator */

  hdrsrchrf_set_agc_acq_params( HDRSRCHRF_NORMAL_AGC_ACQ_MODE, 0, 0 );
    /* Set AGC acquisition parameters for normal mode */

  hdrsrchacq_init_pre_detect_bin();
    /* Intialize pre-detect binning */

  if( hdrsrchutil_is_brute_force_acq() == FALSE )
  {
    delay_rf_tune_us = srch_hdr_time_until_tune_complete( hdrsrchsleep_get_cur_warmup_reacq_time() );
      /* Get RF tune delay from 1x */

    if ( delay_rf_tune_us != 0 )
    {
      hdrsrch_timed_wait_usecs( delay_rf_tune_us );
        /* Timed wait */
    }

  
      /* Enable RF and Tune to the channel */
      tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                              &hdrsrchacq.params.channel,
#ifdef FEATURE_HDR_REVB
                            &acq_demod_idx,
                            1,
#endif /* FEATURE_HDR_REVB */
                              hdrsrchacq_tune,
                              HDRSRCHACQ_BAND_GRANT_TIMEOUT_MS );
    }
    else
    {
  
      /* Enable RF and tune to the channel */
      tune_status = hdrsrchrf_tune_to_chan( HDRSRCHRF_PRI_CHAIN,
                              &hdrsrchacq.params.channel,
#ifdef FEATURE_HDR_REVB
                              &acq_demod_idx,
                              1,
#endif /* FEATURE_HDR_REVB */
                              hdrsrchacq_brute_force_tune_done,
                              HDRSRCHACQ_BAND_GRANT_TIMEOUT_MS );
    }

  if ( !hdrsrch_is_lre_enabled() )
  {
    hdrsrch_remove_from_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
  }

  if ( tune_status == FALSE )
  {
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );

    return;
  }

} /* hdrsrchacq_start_acq */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHACQ_DISABLE_TUNE_AWAYS

DESCRIPTION
  Turns off the ability for tune aways to happen within the ACQ state.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchacq_disable_tune_aways( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) timer_clr( &hdrsrchacq.tune_away.timer, T_NONE );
    /* Stop the tune away timer. */

  hdrsrchrf_unregister_unlock_cb( HDRSRCHRF_PRI_CHAIN );
    /* Unregister for unlock callbacks.  We now will respond to them. */

  hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* We are no longer interested in primary chain events. */

  hdrsrch_clear_tune_away_on_searcher_idle( );
   /* Clear tune away on search idle if not NULL. We don't want to perform
      pending tune way if there is any.
   */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                "HDR LRE: ACQ disable tune_aways" );  /* REVISIT */

} /* hdrsrchacq_disable_tune_aways */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHACQ_ON_SEARCH_IDLE_CB

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

void hdrsrchacq_on_search_idle_cb( void )
{
  boolean rsp_received = FALSE;
   /* FW response for finger de-assign */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_2( MSG_LEGACY_MED, "CB: ACQ reason %d Redir %d",
      hdrsrchacq.params.reason,
      hdrsrchacq.redir_state );

  if ( hdrsrchacq.redir_state == HDRSRCHACQ_REDIR_ACQ )
  {
    /* Now HDR was in the process of ACQ for redirection but received
     * unlock CB. So release TRM lock for high priority request
     */
    HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDRSRCHACQ_REDIR_TA" );
    hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_TA;
    /* Now tune away from ACQ during redirection */

    if (hdrsrchfing_are_fings_assigned() == TRUE)
    {
       hdrsrchfing_deassign_all_fings();
       /* De-Assign fingers here. This is to to ensure FW stops decoding packets,
          so that FW response to dec deactivate is quick later when it is
          called in the unlock_rf */

       rsp_received = hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US,
                                          HDRSRCH_FING_ASSIGN_RSP );
       /* Wait for 1 ms to ensure that all fingers are deassigned */

       HDR_MSG_SRCH_1( MSG_LEGACY_MED, "HDRSRCHACQ_REDIR_TA: De-assign fing rsp:%d",
                       rsp_received);
    }

    hdrsrchacq_yield_rf();
  }
  else if ( hdrsrch_is_lre_enabled() )
  {
    /* Not worth continuing ACQ if it is taking too long and scheduled
     * 1x demod wake-up is waiting. */
    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_ADVISE_UNLOCK_RF );
      /* RF unavailable hence report system lost, SD will reinitiate HDR */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "Deactivate and Reacquire with same band-channel");

    hdrsrchacq.tune_away.restore_ta_cnt = TRUE;
      /* Continue the tune away count after deactivate */

    hdrsrch_deactivate(NULL);
      /* Deactivate HDR */
    
    hdrsrch_acquire(&hdrsrchacq.params);
      /* Start initial acquisition from the beginning on same channel */    
  }

} /* hdrsrchacq_on_search_idle_cb */

/* EJECT */
/*===========================================================================

FUNCTION HDRSRCHTCACQ_PROCESS_TUNE_AWAY

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

LOCAL void hdrsrchacq_process_tune_away
(
  unsigned                   extend_ms
    /* maximum duration for which we can have the RF */
)
{
  if ( extend_ms > HDRSRCHACQ_MIN_RF_TUNEAWAY_MS )
  {
    timer_set(&hdrsrchacq.tune_away.timer, extend_ms - HDRSRCHACQ_MIN_RF_TUNEAWAY_MS, 0, T_MSEC);
      /* Set the timer for the appropriate duration */

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "HDR LRE: %dms - %dms (buffer) from tune-away Next event=%dms",
                    extend_ms,
                    HDRSRCHACQ_MIN_RF_TUNEAWAY_MS,
                    extend_ms - HDRSRCHACQ_MIN_RF_TUNEAWAY_MS );

  }
  else
  {
    /* Tuneaway now */

    hdrsrchdrv_abort_search( FALSE );
      /* Abort pending searches, if any */

    hdrsrchacq_disable_tune_aways();
      /* Disable tune aways now because we do not permit processing unlock
         cancels which will abort the tune away at this stage of the
         process. */

    hdrsrch_tune_away_on_searcher_idle( hdrsrchacq_on_search_idle_cb );
      /* transition to other state, only when searcher is idle  */

    hdrsrchacq.tune_away.tune_away_cnt++;
      /* Count tune-away which results in Acquisition failure */

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "HDR LRE: %dms <= %dms (buffer) from tune-away, start tune-away now",
                    extend_ms,
                    HDRSRCHACQ_MIN_RF_TUNEAWAY_MS );

  }

}  /* hdrsrchacq_process_tune_away */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_PRI_CHAIN_EVENT

DESCRIPTION
  Handles both:
  1. Called when the RF resources needed for acq is granted to HDR.
  2. RF unlock event, including unlock immediate and unlock by.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Requests TCXO services so that acquisition can start after TCXO is
  granted.

==========================================================================*/

void hdrsrchacq_pri_chain_event( void )
{
  unsigned                       ms_left;
    /* maximum duration for which we can have the RF */

  hdrsrchrf_retain_lock_state_t  rl_state;
    /* State of retain lock. */

  uint32                         sclks_left;
    /* Sclks remaining until lock release */

  uint32                         sclks_now;
    /* Sclks remaining until lock release */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Race conditon might have happening between TA timer expiring and clearing 
   * primary chain event signal from HDRSRCH TCB and for these scenario,
   * there is chance of getting this event without owning RF chain
   */
  if ( (hdrsrchacq.tune_away.req_notify_pending == TRUE) && 
       !hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) )
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "Ignore primary chain event R&N=%d rf lock%d",
                    hdrsrchacq.tune_away.req_notify_pending,
                    hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) );
    return;
  }
  

  /* If R&N is pending and just got the RF grant process following */
  if ( hdrsrchacq.tune_away.req_notify_pending == TRUE
    || !hdrsrch_is_lre_enabled() )
  {
    if ( hdrsrch_is_lre_enabled() )
    {
      hdrsrchacq.tune_away.req_notify_pending = FALSE;
      /* No TRM request and notify is pending */
    }

    if ( hdrsrchacq.tune_away.acq_priority == HDRSRCHRF_ACQUISITION )
    {

      hdrsrch_stop_timer( &hdrsrch.timeout_timer );
        /* TRM granted chain to HDR with Acquisition priority hence stop timer */

      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                      "HDR Srch Acq stop inv timer %d", 
                      hdrsrchacq.tune_away.acq_priority );
    }
    else if ( hdrsrchacq.tune_away.acq_priority == HDRSRCHRF_ACQUISITION_INV )
    {
      hdrsrchacq_start_inv_timer();
        /* HDR can only be with Acq inversion priority for maximum of 1sec 
        hence run timer with 1sec duration */
    }

    HDR_MSG_SRCH_1( MSG_LEGACY_MED,
                    "HDR Srch Acq lock granted %d", 
                    hdrsrchacq.tune_away.acq_priority );

#ifdef FEATURE_NPA
    ( void ) hdrsrchutil_npa_issue_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
      /* Request CPU for HDR wakeup. */
#endif /* FEATURE_NPA */

    hdrsrchmcpm_set_state( HDRSRCHMCPM_WAKE_UP_REQ );
      /* Tell MCPM to turn on clocks grouped under wake-up */

    hdrts_reset_rtc();
     /* Reset RTC count before enabling FW */

    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "HDR LRE: Pri Chain Event: HDR Acq Mode %d: Band=%d Chan=%d",
                    hdrsrchacq.params.mode,
                    hdrsrchacq.params.channel.band,
                    hdrsrchacq.params.channel.chan_num );

    hdrsrchacq.stage1 = HDRSRCHACQ_S1_WAITING_FOR_TCXO_OPEN_LOOP;
      /* Before starting acquisition, find out whether HDR can own
         the TCXO and if so, take control of it and set it to a known
         good frequency. */
  
    hdrsrchacq.stage2 = HDRSRCHACQ_S2_IDLE;
      /* Iniitalize the stage2 to HDRSRCHACQ_S2_IDLE after RF grant */
      
    if ( hdrsrchacq.acq_init_pending == TRUE )
    {  
      hdrmdsp_fws_enable_app();
        /* Enable fws hdr app */
  
      hdrsrchdrv_request_srch4_mdsp_app();
      /* Request use of the search4 app and make sure it is downloaded into
         the firmware. */
    }
  
    hdrmdsp_fws_resume_app();
      /* Wake up FW is it's been suspended by hdr app */
  
    hdrmdsp_config_hdr_app( HDRFW_STATE_ACTIVE, TRUE, FALSE ); 
      /* We need to make sure the hdr related apps are loaded before
         acquiring the TCXO. */
  
    hdrts_abort_sleep();
      /* Re-enable time stamp service's demod roll isr */
  
    hdrsrchutil_enable_roll();
      /* Register the PN Roll callback */
  
    hdrsrchafc_set_fll_chan( hdrsrchacq.params.channel );
      /* Set the channel info to afc */
  
    hdrsrchafc_request_open_loop();
      /* Request open loop */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    hdrmultirat_dsda_mode_change();
      /* If in DSDA tell FW */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */  

#ifdef FEATURE_HDR_DR_DSDS
    hdrsrchrf_update_dr_mode();
      /* Check with TRM if we are in DR mode */
#endif /* FEATURE_HDR_DR_DSDS */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_FW
    hdrmultirat_send_dsdx_frequency_cfg_msg( );
      /* During sleep FW loses freq id info, 
         so resend during reacq */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_FW */
  }
  else
  {
    /* ACQ state must already own RF lock here.  Handle unlock events. */

    hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
      /* Retrieve the retain lock state. */

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, 
                    "HDR LRE: Pri Chain Event: rf_unlock_event() event: %d", 
                    rl_state.event  );

    /* Must already own RF.  Check unlock events. */
    if ( rl_state.event == HDRSRCHRF_UNLOCK_IMMEDIATELY )
    {
      /* Unlock now, e.g., for 1x rude wakeup */
   
      HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                    "HDR LRE: Pri Chain Event: ACQ unlock immediately requested" );
   
      hdrsrchacq_disable_tune_aways();
        /* Unregister for unlock callbacks. */

      hdrsrchutil_system_lost( HDRSRCH_SYSLOST_ADVISE_UNLOCK_RF );
        /* Since this is probably due to 1x rude wakeup, there is no reason to
           continue ACQ */
    }
    else if ( rl_state.event == HDRSRCHRF_UNLOCK_CANCELLED )
    {
      (void) timer_clr( &hdrsrchacq.tune_away.timer, T_NONE );
        /* Stop the tune away timer. */

      hdrsrchacq.tune_away.no_tune_away = TRUE;
        /* Current TA cancelled */

      HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                      "HDR LRE: Pri Chain Event: ACQ unlock Cancelled: Process unlock event: %d. No tune away",
                      rl_state.event );
    }
    else if ( ( rl_state.event == HDRSRCHRF_UNLOCK_BY ) 
      || ( rl_state.event == HDRSRCHRF_UNLOCK_REQUIRED ) )
    {
      /* This handles both unlock_by and the unlock_by timeout. */
      if ( hdrsrchacq.tune_away.tune_away_cnt < HDRSRCHACQ_MAX_TA_ALLOWED_COUNT )
      {
        /* if Acq stopped because of TA for MAX TA times then ignore unlock by notification */

      sclks_now = timetick_get();
        /* Current time in sclks. */ 

      sclks_left = hdrsrchutil_calc_time_to_tune_away( 
              sclks_now,
              (int32) HDRSRCHACQ_MIN_RF_TUNEAWAY_SCLK,
              &rl_state,
              &hdrsrchacq.tune_away.no_tune_away,
              &hdrsrchacq.tune_away.end_time_sclks,
              NULL );
        /* How much more time should we stay in idle access? */
    
      ms_left = timetick_cvt_from_sclk( sclks_left, T_MSEC );
        /* Convert lock extension to milliseconds. */
    
      hdrsrchacq_process_tune_away( ms_left );
        /* Process tuneaway request */
      }
      else
      {
          hdrsrchrf.pri_chain_unlock_ignored = TRUE;
          HDR_MSG_SRCH( MSG_LEGACY_HIGH,
          "HDR LRE: Acquisition stopped for MAX TA count, cancelling current tune away");
      }
      }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Invalid RF event received" );
    }
  }
} /* hdrsrchacq_pri_chain_event( ) */



/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_LOCK_RF

DESCRIPTION
  Determine time length of next acquisition segment, and attempt to lock
  the RF for that period.

DEPENDENCIES
  hdrsrchacq.depth must be set.

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  hdrsrchacq_rf_lock_notify( ) will be called when RF lock is granted.

==========================================================================*/

LOCAL void hdrsrchacq_lock_rf( void )
{
  hdrsrchrf_resource_enum_t resource = HDRSRCHRF_RX_BEST;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchacq.depth >= HDRSRCHACQ_NUM_DEPTHS ) 
  {
    /* This is not expected */
    ERR_FATAL( "depth %d is too large", hdrsrchacq.depth, 0, 0 );
  }

#ifdef FEATURE_FACTORY_TESTMODE
  if ( ftm_get_mode( ) == FTM_MODE ) 
  {
    resource = HDRSRCHRF_RXTX_BEST;
  }
#endif /* FEATURE_FACTORY_TESTMODE */

  hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* Watch for Lock Granted signal */

  (void) rex_clr_sigs( HDRSRCH_TCB, HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* Clear the Lock Granted signal */

  switch ( hdrsrchacq.redir_state )
  {
    case HDRSRCHACQ_REDIR_TA:    /* Now TA during ACQ */
      hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_ACQ;
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "TA->HDRSRCHACQ_REDIR_ACQ" );
      /* Now start ACQ to redirect */
      break;

    case HDRSRCHACQ_REDIR_ACQ:    /* Now start ACQ for redirection */
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDRSRCHACQ_REDIR_ACQ" );
      if ( hdrsrchrf_exchange ( 
          TIMETICK_SCLK_FROM_MS( HDRSRCHACQ_MAX_TUNEAWAY_DURATION_MS 
          + HDRSRCHACQ_AVG_ACQUISITION_DURATION_MS ),
                                HDRSRCHRF_ACQUISITION ) == TRUE )
      {
        hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_ACQ_EXCHANGE;
      }
      else
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TRM lock could not be exchanged" );
      }
      break;

    default: /* About to start ACQ */
      HDR_MSG_SRCH( MSG_LEGACY_HIGH, "ACQ No Redir" );
      break;
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "ACQ new TRM Priority %d", hdrsrchacq.tune_away.acq_priority );

      hdrsrchrf_request_and_notify( HDRSRCHRF_PRI_CHAIN,
                                    resource,
                                TIMETICK_SCLK_FROM_MS( HDRSRCHACQ_MAX_TUNEAWAY_DURATION_MS 
                                  + HDRSRCHACQ_AVG_ACQUISITION_DURATION_MS ),
                                hdrsrchacq.tune_away.acq_priority,
                                    hdrsrch_rf_grant_cb );
        /* Notify this module when RF resources have been locked */

  if ( hdrsrch_is_lre_enabled() )
  {
    hdrsrchacq.tune_away.req_notify_pending = TRUE;
      /* TRM request and notify is pending */
  }

  if ( hdrsrchacq.tune_away.acq_priority == HDRSRCHRF_ACQUISITION )
  {
    hdrsrchacq_start_inv_timer();
      /* Start inversion timer to bump up ACQ priority */
  }
} /* hdrsrchacq_lock_rf( ) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_UNLOCK_RF

DESCRIPTION
  Free the RF resources

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchacq_unlock_rf( void )
{
  uint32                   delay_rf_tune_us = 0;
    /* Delay to tune RF to aovid collision with 1x paging */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "ACQ Div: disable diversity");

  hdrsrchrfdiv_diversity_ctrl( FALSE );
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

  delay_rf_tune_us = srch_hdr_time_until_tune_complete( HDRSRCHACQ_TA_TUNEAWAY_US );
    /* Get RF tune delay from 1x provided time taken by HDR from current state to Pri RF chain release */

  if ( delay_rf_tune_us != 0 )
  {
    hdrsrch_timed_wait_usecs( delay_rf_tune_us );
      /* Timed wait */
  }

  hdrsrchutil_dec_deactivate( );
    /* Disable HDR decoder */

  hdrsrchrf_prep_to_sleep();
      /* Prepare for sleep: Stop PDM Loops and Disable PDMs */

  hdrsrchafc_deactivate();
    /* Notify tcxo manager we are going back into open loop mode. */

  hdrts_sleep();
    /* Ensure demod roll ISR is disabled when HDR clk_regime is disabled. */

  hdrsrchrf_disable_rx( HDRSRCHRF_PRI_CHAIN );
    /* Disable rx */

  hdrmdsp_config_hdr_app( HDRFW_STATE_STANDBY, TRUE, FALSE );
    /* Disable the HDR MDSP Application. */

  hdrsrchdrv_disable_srch4( );
    /* Disable SRCH4 operation  */

  hdrmdsp_fws_suspend_app( );
    /* Suspend FW */

  if ( hdrsrch_is_lre_enabled() )
  {
    hdrsrchacq_disable_tune_aways();
      /* Disable tuneaway handling. */
  }

  if ( hdrsrchacq.redir_state == HDRSRCHACQ_REDIR_ACQ_EXCHANGE )
  {
    /* Release lock through exchange iff HDR received lock from exchanging */
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "HDRSRCHACQ_REDIR_ACQ_EXCHANGE" );
    if( hdrsrchrf_exchange( 0, HDRSRCHRF_LAST_REASON ) == FALSE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "TRM lock could not be exchanged" );
      hdrsrchrf_release( HDRSRCHRF_PRI_CHAIN );
      /* Release the RF resources */
    }
    /* Only in failure case HDR releases lock through exchange. Hence
     * failure handler will finally set the state of the redir_state
     */
  }
  else
  {
    /* Release the lock for tune away during ACQ/REDIR */
    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "Acq Redir State: %d", hdrsrchacq.redir_state);
    hdrsrchrf_release( HDRSRCHRF_PRI_CHAIN );
    /* Release the RF resources */
  }

  hdrsrchacq.tune_away.no_tune_away = TRUE;
    /* Reset tune away related variable */

  hdrsrchmcpm_set_state( HDRSRCHMCPM_GO_TO_SLEEP_REQ );
    /* Tell MCPM to switch to sleep state clock configuration */

} /* hdrsrchacq_unlock_rf( ) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_YIELD_RF

DESCRIPTION
  Free the RF resources, and reattempt to acquire it.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchacq_yield_rf( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchacq_stop_timer( &hdrsrchacq.timer.task );
    /* Stop recording "Virtual" time for acquisition. */

  hdrsrchacq_unlock_rf( );
    /* Release the RF resources */

  hdrsrchacq_lock_rf( );
    /* And asked for them back ... when time again permits. */

#ifdef FEATURE_NPA
  ( void ) hdrsrchutil_npa_cancel_cpu_request( HDRSRCHUTIL_NPA_CPU_WAKEUP );
    /* Cancel CPU request for HDR wakeup. */
#endif /* FEATURE_NPA */

} /* hdrsrchacq_yield_rf */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_FAILED

DESCRIPTION
  Acquisition has failed

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchacq_failed( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchacq_stop_timer( &hdrsrchacq.timer.real );
  hdrsrchacq_stop_timer( &hdrsrchacq.timer.task );
    /* Stop acquisition timers */

  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "HDR ACQ failure took %dms (%dms) A=%d",
                  qw_lo( hdrsrchacq.timer.task.elapsed_ms),
                  qw_lo( hdrsrchacq.timer.real.elapsed_ms),
                  hdrsrchacq.params.mode );

  hdrsrchacq_unlock_rf( );
    /* Release the RF resources */

  if ( ( hdrsrchacq.redir_state == HDRSRCHACQ_REDIR_ACQ ) ||
       ( hdrsrchacq.redir_state == HDRSRCHACQ_REDIR_ACQ_EXCHANGE ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDRSRCHACQ_REDIR_FAIL" );
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_L2D_REDIR_FAILURE);
#endif
    hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_FAIL;
    /* Redirection successful */
  }
  else
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    hdr_qsh_event_notify(hdrutil_get_hdr_sub_id(),QSH_CLT_HSRCH, HSRCH_QSH_EVENT_RLF);
#endif
  }

  hdrsrchacq.tune_away.tune_away_cnt = 0;
    /* Acquistion failed reset the tune_away_cnt */

  hdrsrchacq.tune_away.restore_ta_cnt = FALSE;
    /* Initialize the restore flag */

  hdrsrchacq.tune_away.acq_priority = HDRSRCHRF_ACQUISITION;
    /* Restore the prioriy back to Normal Acquisition */

  if ( hdrsrchacq.params.acq_cb != NULL ) 
  {
    hdrsrchacq.params.acq_cb( &hdrsrchacq.params.channel,
                              HDRSRCH_ACQUIRE_FAILED );
    /* Give up - we've done our best on this channel */
  }

} /* hdrsrchacq_failed( ) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_START_PREDETECT

DESCRIPTION
  Starts a predetect search on a given coset group

DEPENDENCIES
  Searcher must be idle.

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_start_predetect( void )
{
  hdrsrchdrv_cap_ctrl_enum_type     refresh;
    /* Refresh sample ram before starting predetect search, or not? */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  DEBUG_S1_ADVANCE( )

  /* Start of new predetect, i.e., new iteration of initial acq */
  hdrsrchacq.stage1 = HDRSRCHACQ_S1_PREDETECT;

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                  "PREDETECT: Start ************ iteration=%d",
                  hdrsrchacq.num_iterations );

  /* Predetect state engine is now in searching state */

  /* Determine whether or not to refresh the sample ram prior to the
     predetection search */

  if ( hdrsrchacq.stage2 == HDRSRCHACQ_S2_IDLE )
  {
    /* Frequency pull-in is not being performed - free to capture fresh
       samples. */

    refresh = REFRESH_BEFORE;
  }
  else
  {
    /* We are doing a pull-in for the previous coset group.  TCXO frequency
       is being played with so we shouldn't capture samples.  However,
       the sample ram is relatively fresh - reuse it. */

    refresh = NO_REFRESH;
  }

  #ifdef FEATURE_XO
  hdrsrchacq_init_detect_bin();
    /* Initialize binning */
  #endif /* FEATURE_XO */

  hdrsrchacq_submit_predetect_search( refresh );
    /* Submit search list for the current coset */

} /* hdrsrchacq_start_predetect( ) */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_NEXT_PREDETECT

DESCRIPTION
  Computes the next coset group and depth to restart predetect.
  This function is primarily used at the end of an initial acquisition
  iteration, i.e., predetect -> detect -> anti-alias -> pull-in.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  hdrsrchacq.predetect updated.

==========================================================================*/

LOCAL void hdrsrchacq_next_predetect( void )
{
  unsigned                          extension_time_ms = 0;
    /* Time required for next acquisition segment */

  boolean                           next_predet;
    /* Whether or not to perform next predetect */

  sys_hdr_acq_mode_e_type           mode;
    /* local copy of acq mode */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW

  /* If in DSDA mode and we have searched all 4 cosets of that group or we arenot in DSDA at all */
  if ( ( ( hdrmultirat_get_mode_pref() == HDRMULTIRAT_DUAL_SIM_DUAL_ACTIVE ) &&
        ( hdrsrchacq.coset_idx == 0 ) ) ||
       ( hdrmultirat_get_mode_pref() != HDRMULTIRAT_DUAL_SIM_DUAL_ACTIVE))
  {
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */


#ifdef FEATURE_XO

  if ( hdrsrchacq.coset_group_searched == TRUE )
  {
    hdrsrchacq.pre_detect_bin.idx++;
      /* Search next pre-detect bin */
    
    hdrsrchacq.coset_group_searched = FALSE;

    HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, 
                    "Pre-detect bin is incremented to %d, coset idx %d", 
                    hdrsrchacq.pre_detect_bin.idx, hdrsrchacq.coset_idx );
  }
  
    if ( hdrsrchacq.pre_detect_bin.idx >= ( hdrsrchacq.pre_detect_bin.max_idx + 1 ))
    {
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, 
                      "Pre-detect reached max bin %d, coset_idx %d coset %d",
                      hdrsrchacq.pre_detect_bin.idx,
                      hdrsrchacq.coset_idx,
                      hdrsrchacq.coset );

      hdrsrchacq.pre_detect_bin.idx = 0;
        /* Reset bin index */

      /* Check if all coset groups have been searched.  If yes, setup for
         next iteration, if any. */
      hdrsrchacq.coset_group++;
        /* Search next coset group */


      if ( hdrsrchacq.coset_group >= HDRSRCHACQ_NUM_COSET_GROUPS )
      {
        /* Finished searching all 4 coset groups. */
        hdrsrchacq.coset_group = HDRSRCHACQ_COSET_GROUP_0;
        hdrsrchacq.num_iterations++;
      }

    }

    hdrsrchacq.pre_detect_bin.freq_offset = HDRSRCHACQ_PRE_DETECT_BIN_OFFSET_HZ;
    /* Set pre-detect freq offset */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  }
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

  HDR_MSG_SRCH_6( MSG_LEGACY_HIGH, "NEXT_PREDETECT: cosgrp=%d cosidx=%d depth=%d, bin[%d]=%d *10 ppm, freq offset %d Hz",
                  hdrsrchacq.coset_group,
                  hdrsrchacq.coset_idx,
                  hdrsrchacq.depth,
                  hdrsrchacq.pre_detect_bin.idx,
                  hdrsrchacq.pre_detect_bin.bin[hdrsrchacq.pre_detect_bin.idx],
                  hdrsrchacq.pre_detect_bin.freq_offset );

#else
  if ( hdrsrchacq.depth < HDRSRCHACQ_NUM_DEPTHS )
  {
    if ( ++hdrsrchacq.coset_group == HDRSRCHACQ_NUM_COSET_GROUPS )
    {
      hdrsrchacq.coset_group = HDRSRCHACQ_COSET_GROUP_0;
      hdrsrchacq.depth++;
    }
  }
#endif /* FEATURE_XO */

  mode = hdrsrchacq.params.mode;
    /* convenience variable */


#ifdef FEATURE_XO
  next_predet = (hdrsrchacq.num_iterations < hdrsrchacq.max_iterations) ?
    TRUE : FALSE;
  SYS_ARG_NOT_USED(mode)
#else
  next_predet =
    (mode==SYS_HDR_ACQ_MODE_FULL && hdrsrchacq.depth < HDRSRCHACQ_NUM_DEPTHS) ||
    (mode==SYS_HDR_ACQ_MODE_DEEP && hdrsrchacq.depth == HDRSRCHACQ_DEEP     ) ||
    (mode==SYS_HDR_ACQ_MODE_SHALLOW && hdrsrchacq.depth == HDRSRCHACQ_SHALLOW );
#endif /* FEATURE_XO */

  if ( next_predet )
  {
    /* Predetect searches are left - get to work, assuming there is time */
    if( !( hdrsrch_is_lre_enabled() ) ||
         ( ( hdrsrchacq.tune_away.no_tune_away == TRUE ) ||
          ( timer_get( &hdrsrchacq.tune_away.timer, T_TICK ) > 0) ) )
    {
      /* if there is no TA scheduled or enough time is left for TA 
               then go for next pre-detect */

      extension_time_ms = HDRSRCHACQ_MAX_TUNEAWAY_DURATION_MS;

      /* Do not call for extend duration if Acquisition for Redirection */
      if ( hdrsrchacq.params.reason == HDRSRCH_ACQ_REDIRECTION )
      {
        hdrsrchacq_start_predetect( );
        /* Begin next predetect attempt */
      }

    /* If we are granted an extension, we'll start the predetect. */
      else /* Normal Acquisition */
      {
    if ( hdrsrchrf_extend_duration( HDRSRCHRF_PRI_CHAIN, 
                TIMETICK_SCLK_FROM_MS( extension_time_ms )) 
                == TRM_GRANTED )
      {
        if( hdrsrch_is_lre_enabled() )
    {
          /* extend duration is called so need to call retain lock */
          
          hdrsrchrf_retain_lock( HDRSRCHRF_PRI_CHAIN, hdrsrch_rf_unlock_cb );
            /* Register an unlock callback so that we may be interrupted. */
        }

      /* We are allowed to search a bit longer */
      hdrsrchacq_start_predetect( );
      /* Begin next predetect attempt */
    }
    else
    {
      hdrsrchacq.stage1 = HDRSRCHACQ_S1_IDLE;
      /* Wait for RF resources to be re-acquired */

      /* Our extension has been denied - we cannot start another predetect.
         However, we do not have to give up the RF immediately - our current
         lock extended until the the end of the previous attempt, which
         could still be in progress */

      if ( hdrsrchacq.stage2 == HDRSRCHACQ_S2_IDLE )
      {
        if ( hdrsrchacq.redir_state != HDRSRCHACQ_REDIR_NONE )
        {
          /* Now HDR was in the process of ACQ for redirection but could not
           * extend the lock. So release TRM lock for high priority request
           */
          HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDRSRCHACQ_REDIR_TA" );
          hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_TA;
            /* Now tune away from ACQ during redirection */
        }

        hdrsrchacq_yield_rf( );
          /* Release the RF ... for the moment. */
      }
    }
   }
  } 
    else
    {
      /* Tune away started and hence hdrsrchacq_start_predetect() is not called */
      HDR_MSG_SRCH_3( 
        MSG_LEGACY_HIGH,
        "Tune away started and hence Stop NEXT_PREDETECT: lre_enabled = %d, \
        no_tune_away = %d, Time left for tuneaway %d ms",
        hdrsrch_is_lre_enabled(),
        hdrsrchacq.tune_away.no_tune_away,
        timer_get( &hdrsrchacq.tune_away.timer, T_MSEC ) );
    }
  }
  else
  {
   /* No more predetect searches left on this channel */

    if ( hdrsrchacq.stage2 == HDRSRCHACQ_S2_IDLE )
    {
      /* Second stage in not busy - we've failed to acquire. */

      hdrsrchacq_update_acq_info( HDRLOG_ACQ_PILOT_FAILURE );
        /* Update ACQ INFO global structure                  */

      hdrsrchacq_failed( );
    }
  }

} /* hdrsrchacq_next_predetect( ) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_START_DETECT

DESCRIPTION
  Program small searches around the best peaks detected in predetect.

DEPENDENCIES
  Predetected completed.

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_start_detect( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchacq.stage1 = HDRSRCHACQ_S1_DETECT;
    /* Beginning detect state */

  HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DETECT: Start ************" );

  hdrsrchacq.predetect.peak_idx = 0;
    /* Init index into predetect peaks */

  hdrsrchacq.detect.energy = 0;
    /* Init best detect energy to 0 (only used if multiple srch lists are
       searched in detect) */

#ifdef FEATURE_XO
  hdrsrchacq.detect.bin_idx = 0;
#endif /* FEATURE_XO */

  hdrsrchacq_init_detect_bin();
    /* Initialize detect binning */

  hdrsrchacq.freq_offset = hdrsrchacq.pre_detect_bin.freq_offset + hdrsrchacq.detect_bin.freq_offset;
    /* set freq offset */

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  hdrsrchacq.predetect.dsda_num_peaks = 0;
    /* Reset dsda srch list count */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

  hdrsrchacq_submit_detect_search( );
    /* Program detect search based on predetect results */

} /* hdrsrchacq_start_detect() */



/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_START_ANTI_ALIAS

DESCRIPTION
  Program anti-alias searches around the detect searches.

DEPENDENCIES
  None

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  Searcher programmed - Dump will occur in the future.

==========================================================================*/

LOCAL void hdrsrchacq_start_anti_alias( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchacq.stage1 = HDRSRCHACQ_S1_ANTI_ALIAS;
    /* Beginning detect state */

  hdrsrchacq.anti_alias = -1;
    /* Start with coset one earlier */

  hdrsrchacq_submit_anti_alias_search( );
    /* Program anti-alias search */

} /* hdrsrchacq_start_anti_alias() */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_STAGE2_FAILED

DESCRIPTION
  Pullin was unsuccessful.  Start the pending detect search (if any)

DEPENDENCIES
  None

INPUT
  log_hdr_acq_result_enum_type. Stage2 failure reason 

RETURN VALUE
  None

SIDE EFFECTS
  Declares acquisition failure and starts next detect or predetect

==========================================================================*/

static void hdrsrchacq_stage2_failed
( 
  log_hdr_acq_result_enum_type reason 
    /* Failure reason code         */
)
{
  boolean                           next_predet;
    /* Whether or not to perform next predetect */

  sys_hdr_acq_mode_e_type        mode;
    /* local copy of acq mode */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_3( MSG_LEGACY_MED, "PULLIN/SLAM failed s1=%d s2=%d A=%d",
                  hdrsrchacq.stage1,
                  hdrsrchacq.stage2,
                  hdrsrchacq.params.mode );

  hdrsrchafc_deactivate();
    /* Deactivate AFC */

  hdrsrchfing_deassign_all_fings();
    /* Deassign finger */

  ( void ) hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US,
                                          HDRSRCH_FING_ASSIGN_RSP );
    /* Wait for 1.8ms to ensure that all fingers are deassigned */

  hdrsrchacq.stage2 = HDRSRCHACQ_S2_IDLE;
    /* We need to wait for the next Detect results */

  mode = hdrsrchacq.params.mode;
    /* convenience variable */

#ifdef FEATURE_XO
  next_predet = (hdrsrchacq.num_iterations < hdrsrchacq.max_iterations) ?
    TRUE : FALSE;
  SYS_ARG_NOT_USED(mode)
#else
  next_predet =
   (mode==SYS_HDR_ACQ_MODE_FULL && hdrsrchacq.depth < HDRSRCHACQ_NUM_DEPTHS) ||
   (mode==SYS_HDR_ACQ_MODE_DEEP && hdrsrchacq.depth == HDRSRCHACQ_DEEP     ) ||
   (mode==SYS_HDR_ACQ_MODE_SHALLOW && hdrsrchacq.depth == HDRSRCHACQ_SHALLOW );
#endif /* FEATURE_XO */

  hdrsrchafc_request_open_loop();
    /* Notify tcxo manager we are going back into open loop mode. */

  if ( hdrsrch_is_lre_enabled() )
  {
    if ( next_predet || ( hdrsrchacq.stage1 != HDRSRCHACQ_S1_IDLE ) )
    {
      /* Continue with Acqusition and enable TA */
      
      hdrsrch_add_to_wait_mask( HDRSRCH_PRI_CHAIN_EVENT_SIG );
        /* We are now interested in primary chain events. */
    
      hdrsrchacq.tune_away.no_tune_away = TRUE;
        /* There is no tune away as of now. */
    
      hdrsrchrf_retain_lock( HDRSRCHRF_PRI_CHAIN, hdrsrch_rf_unlock_cb );
        /* Register an unlock callback so that we may be interrupted. */
    }
  }

  if ( !next_predet )
  {
    /* There are no more coset/depths to try to acquire.
       There will be no more detect results. */

    hdrsrchacq_update_acq_info( reason );
      /* Update ACQ INFO global structure                  */

    hdrsrchacq_failed( );
  }
  else if ( hdrsrchacq.stage1 == HDRSRCHACQ_S1_WAIT )
  {
    /* Uh ... the results are actually already ready. */

    hdrsrchacq_start_detect( );
      /* Start detect search */
  }
  else if ( hdrsrchacq.stage1 == HDRSRCHACQ_S1_IDLE )
  {
    if ( hdrsrchacq.redir_state != HDRSRCHACQ_REDIR_NONE )
    {
      /* Now HDR was in the process of ACQ for redirection but received
       * unlock CB. So release TRM lock for high priority request
       */
      HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDRSRCHACQ_REDIR_TA" );
      hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_TA;
        /* Now tune away from ACQ during redirection */
    }

    /* Predetect is wait for an RF lock.
       The RF was not released, since pull-in was in progress,
       so it must be released now. */

    hdrsrchacq_yield_rf( );
      /* Release the RF resources - and reacquire when time permits */
  }

  else if ( hdrsrchacq.stage1 == HDRSRCHACQ_S1_ANTI_ALIAS )
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "Trigger next predetect" );
    hdrsrchacq_next_predetect( );
  }

  else if (hdrsrchacq.stage1 == HDRSRCHACQ_S1_PREDETECT)
  {
    /* do nothing, since we need to wait for the next dump */
  }

  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "What stage are we in? %d", 
                    hdrsrchacq.stage1);
  }
} /* hdrsrchacq_stage2_failed( ) */


/*==========================================================================

FUNCTION        HDRSRCHACQ_PREPARE_FINGER_AND_CREATE_ASET

DESCRIPTION
  This function prepares finger structure with value from searched strongest 
  paths, and create an ASET 

DEPENDENCIES
  None

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

LOCAL void hdrsrchacq_prepare_finger_and_create_aset
(
   uint16                            coset_pn_offset,
     /* the smallest pn offset in the coset */
   hdrsrchdrv_srch_res_struct_type   * p_finger_paths
     /* pointer to finger paths */
)
{

  int                               f;
    /* Finger loop index */

  hdrsrchfing_fing_struct_type      *fing;
    /* Finger pointer corresponding to finger[f] */

  uint32                            rtc_offx8;
    /* Finger position in chipx8 */

#ifdef FEATURE_HDR_REVB
  const sys_channel_type            *chan_list;
    /* A pointer to a channel list table */

  const hdrsrch_demod_idx_type      *demod_list;
    /* A pointer to the demod index assignment from hdrsrchrf. */

  int8                              i;
    /* Loop index */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchfing_set_demod_carrier_mask( HDRSRCHFING_DEMOD_CARRIER_0_MASK );
    /* Enabled demod carrier 0 */

  for ( f = 0; f < HDRSRCHFING_VERIFY_ACQ_FING_CNT ; f ++ )
  {
    if ( p_finger_paths->eng[ f ] >= HDRSRCHACQ_MIN_PATH_ENG )
    {
      fing = &hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][ f ];

      if (hdrsrchacq.disable_srch_int == 0)
      {
        /* Searcher interpolation is enabled */
        rtc_offx8 = (((uint32)p_finger_paths->posx8_int[ f ]) );
          /* Interpolated position is already cx8 */
      }
      else
      {
        /* Searcher interpolation is disabled */
        rtc_offx8 = (((uint16)p_finger_paths->posx2[ f ]) * 4 );
          /* Convert peak position in chipx2 to chipx8 */
      }

      fing->modified                = TRUE;
      fing->pn_offset               = coset_pn_offset;

      fing->dsp_regs.ant_sel        = hdrsrchfing.params.default_ant;

      fing->dsp_regs.mac_idx        = HDRSRCHACQ_VERIFY_WALSH;

      /* TBD */
      /* Need to discuss with firmware */
      /* The following function hdrsrchset_create_aset() will update 
         asp_idx for the finger */
      fing->dsp_regs.asp_idx        = HDRSRCHACQ_VERIFY_ASP_IDX_PRE;
      fing->dsp_regs.rpc_cell_idx   = HDRSRCHACQ_VERIFY_CELL_IDX;
      fing->dsp_regs.init_sinr      = hdrsrchacq.init_sinr;

      fing->dsp_regs.div            = hdrsrchrfdiv_diversity_demod_is_enabled();

      hdrsrchfing_set_rtc_offset( fing, rtc_offx8 );

      fing->locked = TRUE; /* Force finegr locked */
      fing->sect_ptr = NULL;

      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "PULLIN/brute-force acq:ASSIGN finger %d rtc_offx8=%d eng_int=%d",
                      f,
                      rtc_offx8,
                      p_finger_paths->eng_int[0]);

    }
  }

  /*-----------------------------------*
   * Create asp                        * 
   *-----------------------------------*/

#ifndef FEATURE_HDR_REVB
  hdrsrch.aset_channel = *hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN );
    /* Acquired on this channel.  This is now the ASET channel */
#else
  chan_list = hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN,
                                     &hdrsrch.aset_channel_cnt,
                                     &demod_list );

  ASSERT( hdrsrch.aset_channel_cnt == 1 );
    /* Assert the channel count in the acquisition state. */

  for( i=0; i<hdrsrch.aset_channel_cnt; i++ )
  {
    hdrsrch.aset_channel[i] = chan_list[i];
    hdrsrch.aset_demod[i]   = demod_list[i];
  }
    /* Acquired on this channel.  This is now the ASET channel */
#endif /* FEATURE_HDR_REVB */

  /* Create a new ASP */
  /* This function empties aset, creates a setcor using finger's pn offset, 
     rpc cell idx, and mac idx, and adds the sector to aset. It updates finger 
     with asp_idx which is created when the sector is added to aset. 
     Then updates ASP and finger to firmware. */
  hdrsrchset_create_aset();

  /*----------------------------------------*
   * Commit fingers assignments to hardware *
   *----------------------------------------*/
#ifdef FEATURE_HDR_NEEDS_QDSP6_PORT
  HDRFING_ENABLE_EAFT();
    /* Enable early arriving finger threshold. */
#endif /* FEATURE_HDR_NEEDS_QDSP6_PORT */

} /* hdrsrchacq_prepare_finger_and_create_aset */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_START_PULLIN

DESCRIPTION
  Program fingers to paths in detect paths[ ] array, and begins frequency
  pulling.

DEPENDENCIES
  None

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  Programs fingers before pull-in. Changes finger structure and
  assigns fingers

==========================================================================*/

LOCAL void hdrsrchacq_start_pullin( void )
{
  boolean                           assignment;
    /* Finger assignment success indication */

#ifdef FEATURE_XO
  int32                             freq_offset_ppb;
    /* Freq iffset in ppb unit */
#endif /* FEATURE_XO */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "PULLIN Started. Acq Mode A=%d", 
                  hdrsrchacq.params.mode);

  hdrsrchacq.stage2 = HDRSRCHACQ_S2_PULLIN;
    /* Beginning pull-in state */

  /*-----------------------------------*
   * Program fingers or Pull-In State *
   *-----------------------------------*/

  hdrsrchacq_prepare_finger_and_create_aset (
      hdrsrchacq.detect.coset,
      &hdrsrchacq.detect.paths
  );

  DEBUG_PULLIN_FING()

  assignment = hdrsrchfing_program_fings();
  /* Assign fingers to the hardware. If the finger assignment failed then we
     need to do something. What to do is not clear yet. May be restart the
     acquisition with predetection state or may be wait for a little while
     and try again. */

  if ( assignment )
  {
#ifdef FEATURE_XO
    freq_offset_ppb = ( hdrsrchacq.detect_bin.bin[hdrsrchacq.detect.bin_idx] << 10 ) * 3 / 2 +
                      (hdrsrchacq.pre_detect_bin.bin[hdrsrchacq.pre_detect_bin.idx] << 10 ) * 10;
        /* Get pullin freq offset in ppb */

      HDR_MSG_SRCH_2( MSG_LEGACY_HIGH, "detect bin idx %d pullin ppb %d", 
                      hdrsrchacq.detect.bin_idx, freq_offset_ppb );
    
      hdrsrchafc_start_fll_acq( (int16) ( - freq_offset_ppb ) );
        /* Start pullin with freq offset. Invert the sign since rot accum and rot hw are opposite. */
#else
      hdrsrchafc_start_fll_acq( 0 );
        /* Start pullin */
#endif /* FEATURE_XO */
   
    hdrsrchacq.stage2 = HDRSRCHACQ_S2_PULLIN;
  }
  else
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, "Can't assign fings in PULL-IN 0x%x", 
                    assignment );

    hdrsrchacq_stage2_failed( HDRLOG_ACQ_FLL_FAILURE );
      /* Declare failure for this coset/depth */
  }
} /* hdrsrchacq_start_pullin() */



/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_START_SLAM

DESCRIPTION
  Program fingers to paths in detect paths[ ] array, and begins frequency
  pulling.

DEPENDENCIES
  None

INPUT
  None

RETURN VALUE
  None

SIDE EFFECTS
  Starts MSTR slam. Programs

==========================================================================*/

LOCAL void hdrsrchacq_start_slam( void )
{
  int                               f;
    /* Finger loop index */

  boolean                           assignment;
    /* Finger assignment success indication */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "Slamming..." );

  /* Following change is added in the code to initialize the bestAspFL
     register in the DSP so that during sync channel demodulation the
     DSP looks for ASP = 1 as the best ASP. Problem case is as follows
       a. DMSS enters traffic with 2 ASPs
       b. there is handoff from ASP=1 to ASP=2
       c. Best ASP is 2
       d. Call is dropped due to bad channel conditions
       e. DMSS declares system loss and enters Acquisition
       f. DMSS looks for sync channel and adds ASP=1 to the DSP
       g. bestAsp = 2 still so DSP will look for control channel from 2
       h. DSP will never find control channel

   */

  /* TBD */
  /* Assume ASP always starts from 0 and there is no hole in ASP assignment */
  hdrsrchmsg_basp_ho_ovrd( HDRMDSP_DEMOD_CARRIER_0, 0 );
    /* Set the best ASP to the first piot after slamming is done */

  /* For each locked finger, change the ASP_IDX and restart the finger */

  for ( f = 0; f < HDRSRCHFING_VERIFY_ACQ_FING_CNT; f++ )
  {
    if ( hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][f].locked )
    {
      hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][f].modified           = TRUE;
      hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][f].dsp_regs.init_sinr = 
                            hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][f].rssi;
      /* TBD */
      /* Need to set asp_idx correctly by getting from aset[] */
#ifndef FEATURE_HDR_REVB
      if ( ( hdrsrchset.aset_cnt != 1 ) || ( hdrsrchset.aset[0] == NULL ) )
    {
        ERR_FATAL( "Aset[0] is null", 0, 0, 0 );
      }

      /* We used different asp idx for pullin and slam in Rev A/Rel 0.
         We can use the same asp for slam as pullin in Rev B. */
      /*hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][f].dsp_regs.asp_idx   = 
                                                HDRSRCHACQ_VERIFY_ASP_IDX_POST;*/
      hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][f].dsp_regs.asp_idx   = 
                                                hdrsrchset.aset[0]->aset.asp_idx;
#else
      if ( ( hdrsrchset.subaset_cnt != 1 )         || 
           ( hdrsrchset.subaset[0].aset_cnt != 1 ) || 
           ( hdrsrchset.subaset[0].aset[0] == NULL ) )
      {
        ERR_FATAL( "Aset[0] is null", 0, 0, 0 );
      }

      /* We used different asp idx for pullin and slam in Rev A/Rel 0.
         We can use the same asp for slam as pullin in Rev B. */
      hdrsrchfing.fings[HDRMDSP_DEMOD_CARRIER_0][f].dsp_regs.asp_idx   = 
               hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[0]->aset.asp_idx;
#endif /* FEATURE_HDR_REVB */
    }
  }

  DEBUG_SLAM_FING()

  assignment = hdrsrchfing_program_fings();
    /* Program modified fingers */

  if ( assignment )
  {
    if ( hdrsrch_is_lre_enabled() )
    {
      hdrsrchacq_disable_tune_aways();
        /* Slamming in-progress disable TA */
    }

    hdrsrch_add_to_wait_mask( HDRSRCHACQ_S2_POLL_SIG );
      /* Watch for stage2 poll signal. It is used for slam_rsp timeout */

    hdrsrchdrv_start_slam( FALSE, 0, 0, 0, 0 );
      /* Synchronize the MSTR with the earliest arriving, locked, finger.
       This will be cleared when the update is complete. (Next 1/2 slot) */

    (void) rex_set_timer( HDRSRCHACQ_S2_POLL_TIMER, HDRSRCHACQ_SLAM_TIMEOUT_MS );
      /* Poll timer for checking for MSTR slam */

    hdrsrchacq.stage2 = HDRSRCHACQ_S2_SLAM;
    /* Wait for MSTR slam to complete */
  }
  else
  {
    if ( hdrsrchutil_is_brute_force_acq() == TRUE )
    {
      HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                    "IRAT: finger assignment failed" );
      hdrsrchacq_failed( );
    }
    else
    {
      hdrsrchacq_stage2_failed( HDRLOG_ACQ_SLAM_FAILURE );
       /* Declare failure for this coset/depth */
    }
  }

} /* hdrsrchacq_start_slam( ) */


/*==========================================================================

FUNCTION        HDRSRCHACQ_ON_FIRST_ACQ

DESCRIPTION
  Process the first acq.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/

void hdrsrchacq_on_first_acq( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchrfdiv_start_stm();
    /* Start div stm */

  hdrsrchrfdiv_user_diversity_pref( hdrsrch.div_ctrl, hdrsrch.div_band_mask );
    /* set user preference for antenna diversity setting */

#ifdef FEATURE_HDR_FL_DATA_ADAPTIVE_RXD
  hdrsrchard_start_stm();
#endif /* FEATURE_HDR_FL_DATA_ADAPTIVE_RXD */
} /* hdrsrchacq_on_first_acq */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_INIT_ON_RF_LOCK

DESCRIPTION
  Acquisition State Initialization to be done only after we get RF lock

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/

LOCAL void hdrsrchacq_init_on_rf_lock( void )
{
  /* Enable finger merge prevent algorithm. The following switch is needed
     only for temporary enable/disable of the algorithm. Once testing is done
     the DSP will enable this algo as default and we will remove these writes */

  hdrsrchfing.mps.pending = TRUE;
  /* Since the DSP is downloaded completely, need to restore the finger
     thresholds that were set before the system loss occured */

  hdrsrchdrv_startup_proc();
    /* Do the startup processing */

  hdrdec_reset_pkt_sequences();
    /* Reset packet sequence numbers */

  /*-------------------------------------------------*
   * Initialize Drivers required for Modem operation *
   *-------------------------------------------------*/

  hdrsrchrf_init();
    /* initialize RF driver afresh */

  hdrsrch_init_div_sm();
} /* hdrsrchacq_init_on_rf_lock( ) */

/*==========================================================================

FUNCTION        HDRSRCHACQ_BRUTE_FORCE_ACQ_INIT

DESCRIPTION
  Initialization for brute-force search

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/

LOCAL void hdrsrchacq_brute_force_acq_init( void )
{
  /* Init variables shared by different state */
  hdrsrch.brute_force_params.overall_time_allowed_ms = 0;
  hdrsrch.brute_force_params.is_brute_force_acq = FALSE;
  hdrsrch.brute_force_params.time_for_acq_ms = 0;

  /* Init variables within acq state */
  hdrsrchacq.brute_force_acq.coset_cnt = 0;
  hdrsrchacq.brute_force_acq.coset_idx = 0;
  hdrsrchacq.brute_force_acq.num_strong_sectors = 0;

} /* hdrsrchacq_brute_force_acq_init */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_INIT

DESCRIPTION
  Acquisition State Initialization

DEPENDENCIES
  None

INPUTS
  old_state - Last HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  Many

==========================================================================*/

LOCAL void hdrsrchacq_init( hdrsrch_state_enum_type last_state )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch_default_init( last_state );
    /* Use default handling */

  hdrsrchacq_brute_force_acq_init();
    /* initialize variable related to brute force acq */

  hdrsrchacq.acq_init_pending = TRUE;
    /* Initialize the acquisition state variables which depend upon the 
       DSP clock enabling only after we get the RF lock*/

  /* Initialize default values to local static */
  hdrsrchacq.num_iterations         = 0;
  hdrsrchacq.max_iterations         = HDRSRCHACQ_MAX_ITERATIONS;
  hdrsrchacq.coset                  = HDRSRCHACQ_NUM_COSETS;
    /* Setting to HDRSRCHACQ_NUM_COSETS will force hdrsrchacq_submit_predetect
       to initialize the coset */

      hdrsrchacq.predetect.winsize      = HDRSRCHACQ_PREDETECT_WINSIZE;

  hdrsrchacq.predetect.max_peaks    = HDRSRCHACQ_MAX_PREDETECT_PEAKS;
  hdrsrchacq.detect.winsize         = HDRSRCHACQ_DETECT_WINSIZE;
  hdrsrchacq.detect.path_thresh     = HDRSRCHACQ_MIN_PATH_ENG_DEFAULT;
  hdrsrchacq.init_sinr              = HDRSRCHACQ_INIT_SINR;
  hdrsrchacq.disable_srch_int       = HDRSRCHACQ_INIT_SRCH_INTERPOLATION;
  hdrsrchacq.mdsp_ft_gain           = 0;
#ifdef FEATURE_XO
  hdrsrchacq.detect_bin.xo_cal_mode        = HDRSRCHACQ_FT1_MODE;
    /* Default to FT1 binning mode.  This is used when the calibration mode
       is not RGS, FT1 or FT2. */
#endif /* FEATURE_XO */

  /* Check if NV items exist to override default values. */
  (void) hdrsrchacq_get_tunable_params();

  /* We initialized set before creating aset at sync in Rev A/Rel 0.
     Since we need to create asp at acquisition, initialize set here. */
  hdrsrchset_init_sets();
    /* Initialize set variables */

  if ( hdrsrch_is_lre_enabled())
  { 
    hdrsrchacq_timer_init();
      /* Enable tune aways */
  }
#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  hdrsrchacq.predetect.dsda_num_peaks = 0;
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

  HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDRSRCHACQ_REDIR_NONE" );
  hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_NONE;
  /* Initialize to no redirection to begin with */

  hdrsrchacq.tune_away.no_tune_away = TRUE;
    /* Initialize tune away related variable */

  DEBUG_INIT();

} /* hdrsrchacq_init( ) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_ACQUIRE_CMD

DESCRIPTION
  This function switches the HDR Searcher state to Acquisition Mode.
  Parameters for the acquisition are stored until the Acquisition state
  is entered.

DEPENDENCIES
  None.

INPUTS
  params    - Acquisition Parameters.

RETURN VALUE
  None

SIDE EFFECTS
  State change: old state is exited.

==========================================================================*/

LOCAL void hdrsrchacq_acquire_cmd( void )
{

  sys_channel_type              *channel;
    /* Channel of system loss */

  hdrsrchrf_trm_freq_info_type  freq_info;
    /* Band/chan info for TRM reservation. */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*----------------------------*
   * Acq Related Initialization *
   *----------------------------*/

#ifdef FEATURE_XO
  hdrsrchacq.depth = HDRSRCHACQ_SHALLOW;
  /* Only use shallow acquisition */

  if ( hdrsrchacq.params.mode != SYS_HDR_ACQ_MODE_FULL )
  {
    hdrsrchacq.max_iterations = 1;
      /* Only 1 iteration for non-full initial acq */
  }
#else
  hdrsrchacq.depth = HDRSRCHACQ_DEEP;
    /* Deep acquisition is for more normal cases where the frequency error
       is small.  We expect the mobile will acquire with deep searching
       most of the time. */
#endif /* FEATURE_XO */

  if ( hdrsrchacq.params.mode == SYS_HDR_ACQ_MODE_SHALLOW )
  {
    hdrsrchacq.depth            = HDRSRCHACQ_SHALLOW;
      /* Start with shallow acquisition */
  }
  
  channel = hdrsrchutil_get_system_loss_chan();
    /* Get the channel before system loss */

  if ( SYS_CHAN_CMP_EQUAL( *channel, hdrsrchacq.params.channel ) )
  {
    hdrsrchacq.coset_idx        = hdrsrchutil_get_system_loss_coset_group( );
      /* Get the index to hdrsrchacq_coset_group_start */
  }
  else
  {
    hdrsrchacq.coset_idx        = 0;
    hdrsrchutil_reset_system_loss_info();
      /* Clear system loss infor */
  }

  hdrsrchacq.coset_group        =  HDRSRCHACQ_COSET_GROUP_0; 
    /* Start with coset group 1 (PN_INC=4 cosets) */

  hdrsrchacq.stage1             = HDRSRCHACQ_S1_IDLE;
    /* 1st Stage Idle - Waiting for RF resources to be acquired */

  hdrsrchacq.stage2             = HDRSRCHACQ_S2_IDLE;
    /* 2nd Stage Idle - Waiting for detect/anti-alias results */

  hdrsrchdrv.mstr_valid         = FALSE;
  hdrsrchdrv.mstr_x8            = 0;
    /* Clear the RTC_MSTR_OFFSET as the system has not been acquired yet */


  qw_set( hdrsrchacq.timer.real.elapsed_ms, 0, 0);
  qw_set( hdrsrchacq.timer.task.elapsed_ms, 0, 0);
    /* Reset both real and virtual timers */

  hdrsrchacq_start_timer( &hdrsrchacq.timer.real );
    /* Start recording "Real" time for acquisition */

  hdrsrchlog.triggered.acq  = TRUE;
    /* ACQ_INFO packet logging is triggered at every acquisition attempt  */

  hdrsrchlog.acq_info.channel.chan_no    = hdrsrchacq.params.channel.chan_num;
  hdrsrchlog.acq_info.channel.band_class = (uint16) (int)
                                               hdrsrchacq.params.channel.band;
    /* Record channel information here                                    */
  
  hdrsrchlog.acq_info.srch_cmd.stage     = 0xf;
    /* Field not applicable when triggered                  */

  hdrsrchlog.acq_info.result             = HDRLOG_ACQ_NULL;
    /* No result when triggered                             */

  memset(&freq_info, 0, sizeof(freq_info));
  freq_info.channel_list[0] = hdrsrchacq.params.channel.chan_num;
  freq_info.freq_info.num_channels = 1;
  freq_info.freq_info.band = hdrsrchacq.params.channel.band;
  hdrsrchrf_set_trm_freq_info( &freq_info );
    /* Convey band/chan info update for TRM interaction. */
  
  if ( hdrsrchacq.params.reason == HDRSRCH_ACQ_REDIRECTION )
  {
    HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDRSRCHACQ_REDIR_ACQ" );
    hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_ACQ;
    /* Now start ACQ to redirect */
  }

  hdrsrchfing_reset_all_fings();
    /* Clear all fingers so that stray finger will not used */

  hdrsrchacq_lock_rf( );
    /* Attempt to acquire the RF resources */

} /* hdrsrchacq_acquire_cmd() */

/*==========================================================================

FUNCTION        HDRSRCHACQ_START_BRUTE_FORCE_ACQ

DESCRIPTION
  This function set up brute force acq related parameters before starting to 
  search.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

void hdrsrchacq_start_brute_force_acq( void )
{

  hdrsrchrf_trm_freq_info_type  freq_info;
    /* Band/chan info for TRM reservation, */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchacq.depth = HDRSRCHACQ_SHALLOW;
    /* Use shallow search always for brute force acq. */

  hdrsrchacq.stage1             = HDRSRCHACQ_S1_IDLE;
    /* 1st Stage Idle */

  hdrsrchacq.stage2             = HDRSRCHACQ_S2_IDLE;
    /* 2nd Stage Idle */

  hdrsrchdrv.mstr_valid         = FALSE;
  hdrsrchdrv.mstr_x8            = 0;
    /* Clear the RTC_MSTR_OFFSET as the system has not been acquired yet */

  qw_set( hdrsrchacq.timer.real.elapsed_ms, 0, 0);
  qw_set( hdrsrchacq.timer.task.elapsed_ms, 0, 0);
    /* Reset both real and virtual timers */

  hdrsrchacq_start_timer( &hdrsrchacq.timer.real );
    /* Start recording brute force acquisition time */

  hdrsrchlog.triggered.acq  = TRUE;
    /* ACQ_INFO packet logging is triggered at every acquisition attempt  */

  hdrsrchlog.acq_info.channel.chan_no    = hdrsrchacq.params.channel.chan_num;
  hdrsrchlog.acq_info.channel.band_class = (uint16) (int)
                                               hdrsrchacq.params.channel.band;
    /* Record channel information here                                    */

  hdrsrchlog.acq_info.srch_cmd.stage     = 0xf;
    /* Field not applicable when triggered                  */

  hdrsrchlog.acq_info.result             = HDRLOG_ACQ_NULL;
    /* No result when triggered                             */

  memset(&freq_info, 0, sizeof(freq_info));
  freq_info.channel_list[0] = hdrsrchacq.params.channel.chan_num;
  freq_info.freq_info.num_channels = 1;
  freq_info.freq_info.band = hdrsrchacq.params.channel.band;
  hdrsrchrf_set_trm_freq_info( &freq_info );
    /* Convey band/chan information for TRM reservation. */

  hdrsrchacq_lock_rf( );
    /* To acquire the RF resources  */

} /* hdrsrchacq_start_brute_force_acq */


/*==========================================================================

FUNCTION        HDRSRCHACQ_CREAT_BRUTE_FORCE_ACQ_COSET_LIST

DESCRIPTION
  This function creates brute force acq coset list based on the PN list from 
  LTE pilot measurement request. 

DEPENDENCIES
  None  

INPUTS
  pn_list - PN list to search  
  num     - number of PNs in the PN list

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

void hdrsrchacq_create_brute_force_acq_coset_list
(
  const sys_pilot_pn_type          *pn_list,
    /* PN list to search */

  uint8                            num
    /* number of PNs in the PN list */
)
{
  uint8                 i, j;
    /* loop index */

  sys_pilot_pn_type     coset_pn;
    /* coset PN */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  memset( hdrsrchacq.brute_force_acq.coset_list, 
          0, 
          sizeof( hdrsrchacq.brute_force_acq.coset_list ) );

  for ( i = 0; i < num; i++ )
  {
    coset_pn = pn_list[i] % HDRSRCHACQ_NUM_COSETS;
      /* Get the coset PN of 0 - 15 */

    for( j = 0; j < hdrsrchacq.brute_force_acq.coset_cnt; j++ )
    {
      if ( coset_pn == hdrsrchacq.brute_force_acq.coset_list[j].coset )
      {
        /* Found in the coset list */
        break;
      }
    }

    if ( j == hdrsrchacq.brute_force_acq.coset_cnt )
    {
      if ( j < HDRSRCHACQ_NUM_COSETS )
      {
        /* Not in the coset list yet and add in the coset list */
        hdrsrchacq.brute_force_acq.coset_list[j].coset = coset_pn;
        hdrsrchacq.brute_force_acq.coset_cnt++;
      }
      else 
      {
        HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                        "coset_cnt: %d, coset list is full", j);      
      }
    }
  }
} /* hdrsrchacq_create_brute_force_acq_coset_list */

/*===========================================================================

FUNCTION HDRSRCHACQ_CMD

DESCRIPTION
  This function processes commands in the acquisition state.

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

LOCAL errno_enum_type hdrsrchacq_cmd
(
  const hdrsrch_cmd_type      * cmd
    /* Command to be processed. */
)
{
  errno_enum_type               status = E_SUCCESS;
    /* Command completion status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_LOW, "Received cmd %d", cmd->name );

  switch ( cmd->name )
  {
    case HDRSRCH_ACQUIRE_CMD:
      hdrsrchacq.params = cmd->params.acquire;
      hdrsrchacq.params.overall_time_allowed_ms += HDRSRCH_BUF_FOR_MMODE_HO_CNF_MS;
        /* Record acquisition parameters */
      HDR_MSG_SRCH_6( MSG_LEGACY_HIGH, 
        "Acquire CMD band= %d chan= %d mode= %d num_pn= %d pn= %d max_time= %d",
         hdrsrchacq.params.channel.band,
         hdrsrchacq.params.channel.chan_num,
         hdrsrchacq.params.mode,
         hdrsrchacq.params.num_of_pn,
         hdrsrchacq.params.pn_list[0],
         hdrsrchacq.params.overall_time_allowed_ms);

      hdrsrch.system_lost = cmd->params.acquire.system_lost_cb;
        /* The SYSTEM LOST parameter is needed everywhere in HDR Search! */

      hdrsrch.system_usable_cb = cmd->params.acquire.usability_cb;
        /* The SYSTEM USABLE param is needed everywhere in hdr search !*/

        hdrsrch_reset_no_dump_cnt( );
          /* reset no_dump_cnt when receiving new acquisition command */

#ifndef FEATURE_HDR_RUMI
      /* Check RF cal. If RF cal is bad, declare system lost */
      if ( !hdrsrchrf_is_band_chan_supported_v2(
                    hdrsrchacq.params.channel.band,
                    hdrsrchacq.params.channel.chan_num ) )
      {
        /* Bad RF Cal */
        HDR_MSG_SRCH_2( MSG_LEGACY_ERROR,
                        "Bad RF cal for band %d chan %d",
                        hdrsrchacq.params.channel.band,
                        hdrsrchacq.params.channel.chan_num );

        hdrsrchutil_system_lost( HDRSRCH_SYSLOST_BAD_RF_CAL );
        break;
      }
#endif /* FEATURE_HDR_RUMI */
      
      if ( cmd->params.acquire.num_of_pn == 0 )
      { 
        /* No pn list in acquire command. This is a normal acquisition */
      hdrsrch_on_searcher_idle( hdrsrchacq_acquire_cmd );
      }
      else
      {
        /* There is a pn list. This is a brute force acquisition for reverse 
           time transfer */

        hdrsrch.brute_force_params.is_brute_force_acq = TRUE;
          /* Indicate the start of brute force acquisition */

        if ( (cmd->params.acquire.overall_time_allowed_ms + 
                    HDRSRCH_BUF_FOR_MMODE_HO_CNF_MS) < 
                    HDRSRCH_MIN_TIME_FOR_ACQ_SYNC_REVERSE_TIME_TRAN_MS )
        {

          HDR_MSG_SRCH_1( MSG_LEGACY_ERROR,
                          "IRAT: time allowed is too short allowed= %d ",
                           cmd->params.acquire.overall_time_allowed_ms +
                           HDRSRCH_BUF_FOR_MMODE_HO_CNF_MS );
          if ( hdrsrchacq.params.acq_cb != NULL ) 
          {
            hdrsrchacq.params.acq_cb( &hdrsrchacq.params.channel,
                              HDRSRCH_ACQUIRE_FAILED );
      }
        }
        else
        {

          uint8 num_of_pn = MIN ( cmd->params.acquire.num_of_pn , HDRSRCH_LTE_SECTOR_LIST_MAX_SIZE );
            /* KW fix : Limit the num_of_pn to maximum */

          hdrsrchacq_create_brute_force_acq_coset_list( 
               cmd->params.acquire.pn_list,
               num_of_pn );
            /* Create brute force search list */

          hdrsrch_on_searcher_idle( hdrsrchacq_start_brute_force_acq );
        }
      }

      if ( hdrsrch_is_lre_enabled() )
      {
        hdrsrchsync_save_sync_substate_callback( 
                           hdrsrchacq.params.sync_suspend_cb );
      }
      break;

    case HDRSRCH_DEACTIVATE_CMD:
      /* Log only when the result is appropriate */
      if( (hdrsrchlog.acq_info.result != HDRLOG_ACQ_NULL) ||
                                          (!hdrsrch_is_lre_enabled()) )
      {
        hdrsrchlog_log_acq_info( );
          /* Need to log ACQ INFO when HDRSRCH transitions from ACQ to INACTIVE 
             state. Fall through, no BREAK here. Command will be handled next */
      }

      if( hdrsrchacq.tune_away.restore_ta_cnt == FALSE )
      {
         hdrsrchacq.tune_away.tune_away_cnt = 0;
          /* Deactivating - reset the tune_away_cnt */
      }
      else
      {
        hdrsrchacq.tune_away.restore_ta_cnt = FALSE;
      }

    default:
      status = hdrsrch_default_cmd( cmd );
        /* Handle state-independent commands */

  } /* switch (cmd->name) */

  return status;

} /* hdrsrchacq_cmd(cmd) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_DONE

DESCRIPTION
  Acquisition State Exit Code

DEPENDENCIES
  None

INPUTS
  new_state - Next HDR Searcher State

RETURN VALUE
  None

SIDE EFFECTS
  Kills the RF tune timer, if running.

==========================================================================*/

LOCAL void hdrsrchacq_done( hdrsrch_state_enum_type next_state )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrch_is_lre_enabled() )
  {
    hdrsrchacq_disable_tune_aways();
      /* Disable ACQ tune away.  Tune away is enabled at the start of SYNC if necessary */
  }

  hdrsrch_kill_timer( &hdrsrch.timeout_timer );
    /* Completed ACQ attempt so stop the inversion timer */

  hdrsrch_kill_timer( HDRSRCHACQ_S2_POLL_TIMER );
    /* Clear the stage2 timer, and remove from wait mask */

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
  hdrsrch_kill_timer( &hdrsrch.gp_timer1 );
  hdrsrch_remove_from_wait_mask( HDRSRCH_GP_TMR1_SIG ); 
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

  hdrsrch_default_done( next_state );
    /* Use default handling */

  hdrsrch_kill_signal( HDRSRCH_PRI_CHAIN_EVENT_SIG );
    /* clear the lock granted signal if it was set */
} /* hdrsrchacq_done() */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_PREDETECT_DUMP

DESCRIPTION
  Acquisition Predetect Search Dump Processing

DEPENDENCIES
  Must be called from hdrsrchacq_dump( )

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  May start a detect search.

==========================================================================*/

LOCAL void hdrsrchacq_predetect_dump( void )
{
 
#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  uint8                        idx,cnt;
    /* Loop Variables */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "PREDETECT Dump Count=%d", 
                  hdrsrchdrv.dump_cnt );

  /* Extract top 16 peaks from search result buffer  */

  DEBUG_PREDETECT_DUMP()

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
  
  /* If in DSDA mode make sure all 4 cosets have been searched before
     DETECT, save best 16 peaks from both predetects and submit 32 peaks in
     DETECT for that coset group.
     */
  if ( hdrmultirat_get_mode_pref() == HDRMULTIRAT_DUAL_SIM_DUAL_ACTIVE )
  {
#ifdef FEATURE_XO
    /* Another side effect of this function to remove peaks that are too close */
    hdrsrchacq.predetect.num_peaks = hdrsrchacq_get_best_peaks(
       hdrsrchacq.predetect.dsda_peaks, hdrsrchacq.predetect.max_peaks,
       hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt
       );

#else
    hdrsrchacq.predetect.num_peaks = hdrsrchacq_get_best_peaks(
       hdrsrchacq.predetect.dsda_peaks, HDR_MAX_SRCH_CNT,
       hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt
       );

#endif /* FEATURE_XO */

    /* Copy all the peaks in this predetect over to hdrsrchacq struct */
    for ( idx = hdrsrchacq.predetect.dsda_num_peaks, cnt = 0; 
          cnt < HDRSRCHACQ_MAX_PREDETECT_PEAKS; idx++,cnt++ )
    {
      hdrsrchacq.predetect.peaks[idx].coset = 
        hdrsrchacq.predetect.dsda_peaks[cnt].coset;
      hdrsrchacq.predetect.peaks[idx].eng = 
        hdrsrchacq.predetect.dsda_peaks[cnt].eng;
      hdrsrchacq.predetect.peaks[idx].posx2 = 
        hdrsrchacq.predetect.dsda_peaks[cnt].posx2;
      hdrsrchacq.predetect.dsda_num_peaks++;
    }

    hdrsrchacq.predetect.num_peaks = hdrsrchacq.predetect.dsda_num_peaks;
    hdrsrchacq.predetect.num_peaks_remaining = hdrsrchacq.predetect.num_peaks;
      /* Store number of peaks remaining value for use in detect stage */

      /* Attempt next predetect if not in brute force acquisition */
    if ( !hdrsrch.brute_force_params.is_brute_force_acq )
    {
    /* If we have not searched all cosets in the group. coset_idx will be reset
        to 0 when all the cosets in one coset group are done.
     */
    if ( hdrsrchacq.coset_idx != 0 )
    {
      /* Complete the predetect for the coset group */      
      hdrsrchacq_next_predetect();
      return;
    }
  }
 }
  else
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
  {
    
#ifdef FEATURE_XO
  /* Another side effect of this function to remove peaks that are too close */
  hdrsrchacq.predetect.num_peaks = hdrsrchacq_get_best_peaks(
          hdrsrchacq.predetect.peaks, hdrsrchacq.predetect.max_peaks,
          hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt
    );

  hdrsrchacq.predetect.num_peaks_remaining = hdrsrchacq.predetect.num_peaks;
    /* Store number of peaks remaining value for use in detect stage */

#else
  hdrsrchacq.predetect.num_peaks = hdrsrchacq_get_best_peaks(
          hdrsrchacq.predetect.peaks, HDR_MAX_SRCH_CNT,
          hdrsrchdrv.srch_list, hdrsrchdrv.list_cnt
    );
#endif /* FEATURE_XO */

  }
  /* Results of this predetect stage are available! */


  if ( hdrsrchacq.stage2 == HDRSRCHACQ_S2_IDLE )
  {
    hdrsrchacq_start_detect( );
      /* Begin detect search */
  }
  else
  {
    hdrsrchacq.stage1 = HDRSRCHACQ_S1_WAIT;
      /* We must wait for frequency pull-in to complete (succeed or fail)
         before continuing with detect -- for parallel srch only */
  }

} /* hdrsrchacq_predetect_dump( ) */

/*==========================================================================

FUNCTION        HDRSRCHACQ_BRUTE_FORCE_ACQ_UPDATE_COSET_PEAKS

DESCRIPTION
  This function updates /saves the search energy and path info of the coset 
  which has to be existed in the coset list.

DEPENDENCIES
  None

INPUTS
  coset - coset to be updated

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

LOCAL void hdrsrchacq_brute_force_acq_update_coset_peaks
(
  uint8              coset,
    /* coset to update */

  uint8              coset_alias
    /* coset found in anti-alias search */
)
{
  int                index;
    /* loop index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  for ( index = 0; index < hdrsrchacq.brute_force_acq.coset_cnt; index++ )
  {
    if ( coset == hdrsrchacq.brute_force_acq.coset_list[index].coset )
    {
      /* Found the coset in the list and store the energy and paths */

      hdrsrchacq.brute_force_acq.coset_list[index].eng = hdrsrchacq.detect.energy;
      hdrsrchacq.brute_force_acq.coset_list[index].paths = hdrsrchacq.detect.paths;
        /* Update coset energy and paths */
      hdrsrchacq.brute_force_acq.num_strong_sectors ++;
        /* Increment number of found strong sectors */

      if ( coset != coset_alias )
      {
        hdrsrchacq.brute_force_acq.coset_list[index].coset = coset_alias;
          /* updated coset number if found an coset alias */
      }
      break;
    }
  }

  if ( index == hdrsrchacq.brute_force_acq.coset_cnt )
  {
    ERR_FATAL( " IRAT MEAS: detected coset is not in coset_list ", 0, 0, 0 );
      /* This should not happen.*/
  }
} /* hdrsrchacq_brute_force_acq_update_coset_peaks */


#ifdef FEATURE_XO
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_DETECT_DUMP

DESCRIPTION
  Acquisition Detect Search Dump Processing.
  The top 16 peaks from the predetect search have been re-searched.

DEPENDENCIES
  Must be called from hdrsrchacq_dump( )

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  May start anti-alias searching.

==========================================================================*/

LOCAL void hdrsrchacq_detect_dump( void )
{
  int                               index;
    /* Best "pilot" from detect search */

  uint32                            energy;
    /* Energy of strongest pilot */

  sys_channel_type          band_chan;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  DEBUG_DETECT_DUMP()

  HDR_ASSERT ( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT );
    /* KW fix : Limit the list_cnt to MAX */

  /* Determine which pilot was the best (strongest) */

  index = hdrsrchacq_find_strongest_sector(
             hdrsrchdrv.srch_list,
             hdrsrchdrv.list_cnt,
             &energy );

  if ( index >= 0 )
  {
    /* If detect stage is partitioned into multiple searches (currently max 16
       at a time), then compare against previous best peak, if any. */
    if ( energy > hdrsrchacq.detect.energy )
    {
      ASSERT( index < HDR_MAX_SRCH_CNT );
        /* Array boundary sanity check */

      hdrsrchacq.detect.coset  =  hdrsrchdrv.srch_list[ index ].srch_task.pn_offset;
      hdrsrchacq.detect.energy =  energy;
      hdrsrchacq.detect.paths  = *hdrsrchdrv.srch_list[ index ].srch_task.res_ptr;
        /* Save coset and peak energy / positions */

      hdrsrchacq.detect_bin.best_peak_freq_offset = hdrsrchacq.detect_bin.freq_offset;
      hdrsrchacq.best_peak_freq_offset = hdrsrchacq.detect_bin.best_peak_freq_offset +
                                         hdrsrchacq.pre_detect_bin.freq_offset;
        /* Save frequency offset from binning for use in anti-aliasing */

      hdrsrchacq.detect.bin_idx = hdrsrchacq.detect_bin.idx;
        /* Save the bin with the best energy */
    }

    HDR_MSG_SRCH_4( MSG_LEGACY_MED, "DETECT: Coset=%d Posx2=%x Eng=%d (Best Eng=%d)",
                    hdrsrchacq.detect.coset,
                    hdrsrchacq.detect.paths.posx2[0],
                    energy,
                    hdrsrchacq.detect.energy );

  }
  else
  {
    HDR_MSG_SRCH_2( MSG_LEGACY_MED, "DETECT: No new pilots found. CGrp=%d A=%d",
                    hdrsrchacq.coset_group, 
                    hdrsrchacq.params.mode );

    DEBUG_S2_ADVANCE( )

  }

  /* Check if done with current bin and bins remaining.   */
  if ( (hdrsrchacq.predetect.num_peaks_remaining <= 0) &&
       (hdrsrchacq.detect_bin.idx < hdrsrchacq.detect_bin.max_idx) )
  {
    /* Still have bin(s) to search.  Re-search all peaks for next detect_bin.
       Increment index to next bin, calculate the new frequency offset (Hz)
       of the new bin, reinitialize to search all predetect peaks */
    hdrsrchacq.detect_bin.idx++;

    hdrsrchacq.detect_bin.freq_offset = HDRSRCHACQ_BIN_OFFSET_HZ;
    hdrsrchacq.freq_offset = hdrsrchacq.pre_detect_bin.freq_offset + hdrsrchacq.detect_bin.freq_offset;
      /* Convert bin's PPM value to frequency offset in Hz */

    band_chan.band            = hdrsrchacq.params.channel.band;
    band_chan.chan_num        = hdrsrchacq.params.channel.chan_num;
    HDR_MSG_SRCH_6( MSG_LEGACY_HIGH,                              
                    "xo mode %d max bin %d bin freq offset=%d (rot res 4.7Hz) detect_bin.bin[%d]=%d (1 or 1.5 ppm) carrier freq=%u KHz",
                    hdrsrchacq.detect_bin.xo_cal_mode,
                    hdrsrchacq.detect_bin.max_idx,
                    hdrsrchacq.detect_bin.freq_offset,                       
                    hdrsrchacq.detect_bin.idx,                               
                    hdrsrchacq.detect_bin.bin[hdrsrchacq.detect_bin.idx],           
                    rfm_cdma_get_rx_carrier_freq( band_chan ) );

    hdrsrchacq.predetect.num_peaks_remaining =
      hdrsrchacq.predetect.num_peaks;

    hdrsrchacq.predetect.peak_idx = 0;
  }

  /* Any remaining predetect peaks to pass to detect stage? */
  if (hdrsrchacq.predetect.num_peaks_remaining > 0)
  {
    /* There are more predetect peaks to pass to detect  */
    hdrsrchacq_submit_detect_search();
  }
  /* No more predetect peaks to pass to detect stage.
     Was a peak found in detect stage? */
  else if (hdrsrchacq.detect.energy != 0)
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_MED, "DETECT: End Coset=%d Posx2=%x Best Eng=%d",
                    hdrsrchacq.detect.coset,
                    hdrsrchacq.detect.paths.posx2[0],
                    hdrsrchacq.detect.energy );

    if ( hdrsrchutil_is_brute_force_acq() == TRUE )
    {
      hdrsrchacq_brute_force_acq_update_coset_peaks( 
        hdrsrchacq.detect.coset,  hdrsrchacq.detect.coset );
        /* Update the coset peak info if necessary */
    }

    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "ANTI-ALIAS: Start ************" );
    hdrsrchacq_start_anti_alias( );
      /* Begin Anti-Alias searches */
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_HIGH, "DETECT: End... No Peaks Found ************" );

    if ( hdrsrchutil_is_brute_force_acq() == FALSE )
    {
      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "DETECT: 0 pilots.CGrp=%d Depth=%d A=%d",
                      hdrsrchacq.coset_group, 
                      hdrsrchacq.depth, 
                      hdrsrchacq.params.mode);
  
      DEBUG_S2_ADVANCE( )
  
      hdrsrchacq_init_detect_bin();
      /* Initialize binning */

    hdrsrchacq_next_predetect( );
      /* No peaks found in detect stage.  Check if start predetect again. */
  }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "IRAT MEAS: Brute force acq DETECT: 0 pilots..." );

      if ( hdrsrchacq.brute_force_acq.coset_idx < hdrsrchacq.brute_force_acq.coset_cnt )
      {

        hdrsrchacq_init_detect_bin();
          /* Initialize binning */

        hdrsrchacq_brute_force_acq_predetect_search( );
          /* start brute force search again */
      }
      else
      {
        /* We have already done predect search for all cosets */

        if ( hdrsrchacq.brute_force_acq.num_strong_sectors == 0 )
        {
          /* We don't find any sector in previous predect/detect/anti-alias search */
          HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                    "IRAT: no coset was found " );
          hdrsrchacq_failed( );
        }
        else
        {
          /* We have found strong sector in previous search. We should request rotator tracking
             and then do slam */
          hdrsrchafc_request_force_rot_trk( );
            /* Now request AFC to force rotator tracking */
        }
      }
    }
  }

} /* hdrsrchacq_detect_dump( ) */
#else
/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_DETECT_DUMP

DESCRIPTION
  Acquisition Detect Search Dump Processing.
  The top 16 peaks from the predetect search have been re-searched.

DEPENDENCIES
  Must be called from hdrsrchacq_dump( )

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  May start anti-alias searching.

==========================================================================*/

LOCAL void hdrsrchacq_detect_dump( void )
{
  int                               index;
    /* Best "pilot" from detect search */

  uint32                            energy;
    /* Energy of strongest pilot */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  DEBUG_DETECT_DUMP()


  HDR_ASSERT ( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT );
    /* KW fix : Limit the list_cnt to MAX */

  /* Determine which pilot was the best (strongest) */

  index = hdrsrchacq_find_strongest_sector(
             hdrsrchdrv.srch_list,
              hdrsrchdrv.list_cnt,
              &energy
            );

  if ( index >= 0 )
  {
    hdrsrchacq.detect.coset  =  hdrsrchdrv.srch_list[ index ].srch_task.pn_offset;
    hdrsrchacq.detect.energy =  energy;
    hdrsrchacq.detect.paths  = *hdrsrchdrv.srch_list[ index ].srch_task.res_ptr;
      /* Save coset and peak energy / positions */

    HDR_MSG_SRCH_4( MSG_LEGACY_MED, "DETECT: Coset=%d Posx2=%x Eng=%d Depth=%d",
                    hdrsrchacq.detect.coset,
                    hdrsrchacq.detect.paths.posx2[0],
                    hdrsrchacq.detect.energy,
                    hdrsrchacq.depth );

    if ( hdrsrchutil_is_brute_force_acq() == TRUE )
    {
      hdrsrchacq_brute_force_acq_update_coset_peaks( 
        hdrsrchacq.detect.coset,  hdrsrchacq.detect.coset );
        /* Update the coset peak info if necessary */
    }

    hdrsrchacq_start_anti_alias( );
      /* Begin Anti-Alias searches */
  }
  else
  {
    if ( hdrsrchutil_is_brute_force_acq() == FALSE )
    {
      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "DETECT: 0 pilots.CGrp=%d Depth=%d A=%d",
                      hdrsrchacq.coset_group, 
                      hdrsrchacq.depth, 
                      hdrsrchacq.params.mode);
  
      DEBUG_S2_ADVANCE( )
  
      hdrsrchacq_next_predetect( );
    }
    else
    {
      HDR_MSG_SRCH( MSG_LEGACY_MED, "IRAT MEAS: Brute force acq DETECT: 0 pilots..." );

      if ( hdrsrchacq.brute_force_acq.coset_idx < hdrsrchacq.brute_force_acq.coset_cnt )
      {
        hdrsrchacq_brute_force_acq_predetect_search( );
          /* start brute force search again */
      }
      else
      {
        /* We have already done predect search for all cosets */

        if ( hdrsrchacq.brute_force_acq.num_strong_sectors == 0 )
        {
          /* We don't find any sector in previous predect/detect/anti-alias search */
          HDR_MSG_SRCH( MSG_LEGACY_HIGH,
                    "IRAT: no coset was found " );
          hdrsrchacq_failed( );
        }
        else
        {
          /* We have found strong sector in previous search. We should request rotator tracking
             and then do slam */

          hdrsrchafc_request_force_rot_trk( );
            /* Now request AFC to force rotator tracking */
        }
      }
    }
  }
} /* hdrsrchacq_detect_dump( ) */
#endif /* FEATURE_XO */

/*==========================================================================

FUNCTION        HDRSRCHACQ_BRUTE_FORCE_ACQ_GET_STRONGEST_COSET

DESCRIPTION
  This functions returns the coset index with the strongest energy.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  coset index of the strongest coset

SIDE EFFECTS
  None
==========================================================================*/

uint8 hdrsrchacq_brute_force_acq_get_strongest_coset( void )
{
  uint32          strong_eng;
    /* The strongest coset energy */

  uint8           strong_coset;
    /* The strongest coset index */

  int             i;
    /* loop index */

  strong_eng = hdrsrchacq.brute_force_acq.coset_list[0].eng;
    /* Init the strongest energy */

  strong_coset = 0;
    /* Init the strongest coset index */

  /* coset_cnt is checked against HDRSRCHACQ_NUM_COSETS whenever it increases.
     So it is always less or equal to HDRSRCHACQ_NUM_COSETS. Add ASSERT
     here to eliminate Klocwork warning */
  if(!(hdrsrchacq.brute_force_acq.coset_cnt <= HDRSRCHACQ_NUM_COSETS))
  {
      ERR_FATAL("Check hdrsrchacq.brute_force_acq.coset_cnt <= "
                "HDRSRCHACQ_NUM_COSETS failed", 0, 0, 0);
  }

  for ( i = 1; i < hdrsrchacq.brute_force_acq.coset_cnt; i++ )
  {
    if ( hdrsrchacq.brute_force_acq.coset_list[i].eng > strong_eng )
    {
      strong_eng = hdrsrchacq.brute_force_acq.coset_list[i].eng;
        /* Remember the strongest energy */

      strong_coset = i;
        /* Remmeber the strongest coset index */
    }
  }

  return strong_coset;
} /* hdrsrchacq_brute_force_acq_get_strongest_coset */


/*==========================================================================

FUNCTION        HDRSRCHACQ_BRUTE_FORCE_ACQ_ASSIGN_FINGER

DESCRIPTION
  This function assign fingers for brute force acq based on the strongest coset.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

void hdrsrchacq_brute_force_acq_assign_finger( void )
{
  uint8                           idx;
    /* the coset idx in the coset list */

  hdrsrchacq_coset_struct_type   *coset_ptr;
    /* Coset pointer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  idx = hdrsrchacq_brute_force_acq_get_strongest_coset( );
    /* Get the strongest coset */

  coset_ptr = &hdrsrchacq.brute_force_acq.coset_list[idx];
    /* convenient pointer */


  hdrsrchfing_set_demod_carrier_mask( HDRSRCHFING_DEMOD_CARRIER_0_MASK );
    /* Enable demod carrier 0 */

  hdrsrchacq_prepare_finger_and_create_aset (
      coset_ptr->coset,
      &(coset_ptr->paths)
  );

  ( void )hdrsrchfing_program_fings();
    /* Assign fingers */
} /* hdrsrchacq_brute_force_acq_assign_finger */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_ANTI_ALIAS_DUMP

DESCRIPTION
  Acquisition Detect Search Dump Processing.
  The top 16 peaks from the predetect search have been re-searched.

DEPENDENCIES
  Must be called from hdrsrchacq_dump( )

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  May start anti-alias searching.

==========================================================================*/

LOCAL void hdrsrchacq_anti_alias_dump( void )
{
  int                               index;
    /* Best "pilot" from detect search */

  uint32                            energy;
    /* Energy of strongest pilot */

  uint8                             original_coset;
    /* Original coset number */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  DEBUG_ANTI_ALIAS_DUMP(hdrsrchacq.anti_alias)

  HDR_ASSERT ( hdrsrchdrv.list_cnt <= HDR_MAX_SRCH_CNT );
    /* KW fix : Limit the list_cnt to MAX */

  /* Determine which pilot was the best (strongest) */

  index = hdrsrchacq_find_strongest_sector(
          hdrsrchdrv.srch_list,
          hdrsrchdrv.list_cnt,
          &energy
    );


  /* Is this pilot better than the best detect pilot? */
  if ( index >= 0 )
  {
    if ( energy > hdrsrchacq.detect.energy )
    {
      ASSERT( index < HDR_MAX_SRCH_CNT );
        /* Array boundary sanity check */

      original_coset = (uint8) hdrsrchacq.detect.coset;
        /* store the original coset */
      hdrsrchacq.detect.coset  =  hdrsrchdrv.srch_list[ index ].srch_task.pn_offset;
      hdrsrchacq.detect.energy =  energy;
      hdrsrchacq.detect.paths  = *hdrsrchdrv.srch_list[ index ].srch_task.res_ptr;
        /* Save coset and peak energy / positions */

      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "ANTIALIAS: Coset=%d Posx2=%x Eng=%d",
                      hdrsrchacq.detect.coset,
                      hdrsrchacq.detect.paths.posx2[0],
                      hdrsrchacq.detect.energy );
      
      if ( hdrsrchutil_is_brute_force_acq() == TRUE )
      {
        hdrsrchacq_brute_force_acq_update_coset_peaks( 
           original_coset, hdrsrchacq.detect.coset );
          /* Update the coset peak info if necessary */
      }
    }
    else
    {
      ASSERT( index < HDR_MAX_SRCH_CNT );
        /* Array boundary sanity check */

      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "ANTIALIAS: Coset=%d Posx2=%x Eng=%d wk",
                      hdrsrchdrv.srch_list[ index ].srch_task.pn_offset,
                      hdrsrchdrv.srch_list[ index ].srch_task.res_ptr->posx2[0],
                      energy );
    }
  }
  else
  {
    HDR_MSG_SRCH( MSG_LEGACY_MED, "ANTIALIAS: No pilots found" );
  }

#ifndef FEATURE_XO
  if ( hdrsrchacq.anti_alias == -1 )
  {
    /* Search other side */

    hdrsrchacq.anti_alias = +1;
    hdrsrchacq_submit_anti_alias_search( );
  }
  else
#endif /* !FEATURE_XO */
  {
    if ( hdrsrchutil_is_brute_force_acq() == FALSE )
    {
      DEBUG_S2_ADVANCE( )
  
      hdrsrchlog.acq_info.pilot_acq_time_ms =
        hdrsrchacq_measure_time( );
        /* This will be the pilot acquisition( predetect/detect/anti-alias before 
           we proceed to pull-in stage. And hopefully this will be the pilot
           acquisition time for a successful acquisition     */
  
      hdrsrchacq.stage2 = HDRSRCHACQ_S2_WAITING_FOR_TCXO_CLOSE_LOOP;
      /* Change the tcxo state to ready for pullin so we know to start pullin
         when TCXO Manager grants permission to close the TCXO loop. */
      
      hdrsrchafc_request_close_loop();
        /* Now lets get TCXO Manager's permission to close the loop. 
           Once we get permission, we will start pullin and start
           the next predetect. */
    }
    else
    {
      if ( hdrsrchacq.brute_force_acq.coset_idx < hdrsrchacq.brute_force_acq.coset_cnt )
      {

#ifdef FEATURE_XO
        hdrsrchacq_init_detect_bin();
          /* Initialize binning */
#endif /* FEATURE_XO */

        hdrsrchacq_brute_force_acq_predetect_search( );
          /* Search the remaining cosets if any left */
      }
      else
      {
        hdrsrchafc_request_force_rot_trk( );
          /* Now request AFC to force rotator tracking */
      }
    }
  }

} /* hdrsrchacq_anti_alias_dump( ) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_DUMP

DESCRIPTION
  Acquisition Search Dump Processing

DEPENDENCIES
  Search results are ready.

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Substate may be advanced, MSTR slam may occur, sync state may be
  initialized.

==========================================================================*/

LOCAL void hdrsrchacq_dump( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrchlog_log_search_record();
    /* do the logging in the acquisition state */

  hdrsrchlog_log_mc_search_record();
    /* Log multi-carrier search record in acquisition state */

  switch ( hdrsrchacq.stage1 )
  {
    case HDRSRCHACQ_S1_PREDETECT:
      hdrsrchacq_predetect_dump( );
      break;

    case HDRSRCHACQ_S1_DETECT:
      hdrsrchacq_detect_dump( );
      break;

    case HDRSRCHACQ_S1_ANTI_ALIAS:
      hdrsrchacq_anti_alias_dump( );
      break;

    default:
      HDR_MSG_SRCH_2( MSG_LEGACY_ERROR, "Unexpected dump Depth=%d Stage1=%d",
                      hdrsrchacq.depth, hdrsrchacq.stage1 );
  }

} /* hdrsrchacq_dump( ) */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_TUNE

DESCRIPTION
  Acquisition Search RF Tune Processing.

  When we start an acquisition, or continue acquisition after a brief
  visit to 1x, we first acquire the RF resources and tune the RF to the
  desired channel.  Once the RF has finished tuning, we can start searching.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  Starts the predetection search.

==========================================================================*/

LOCAL void hdrsrchacq_tune
( 
  hdrsrchrf_tune_status_enum_type status
)
{
  int               rx0_agc, rx1_agc, agc;
    /* Rx0 and Rx1 AGC measurements */

  boolean           diversity;
    /* Diversity settings */

  boolean           search_ok = TRUE;
    /* its ok to search */

  hdrsrchrf_retain_lock_state_t  rl_state = {0};
    /* State of retain lock. */

#ifdef FEATURE_HDR_REVB
  uint8             chan_cnt;
    /* Channel count */

  const hdrsrch_demod_idx_type      *demod_list;
    /* A pointor to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( status != HDRSRCHRF_TUNE_SUCCESS )
  {
    HDR_MSG_SRCH_1(MSG_LEGACY_ERROR, "Tune status %d not expected",
                   status);

    hdrsrchutil_system_lost( HDRSRCH_SYSLOST_NO_RESOURCES );
    return;
  }

#ifdef FEATURE_HDR_BOLT_RUMI
  hdrsrchstate_sram_check( TRUE );
#endif /* FEATURE_HDR_BOLT_RUMI */

  if ( hdrsrch_is_lre_enabled() &&
       ( hdrsrchacq.params.reason != HDRSRCH_ACQ_REDIRECTION ) )
  {
    HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                    "LRE enabled, enable tuneaway for reason %d", 
                    hdrsrchacq.params.reason );

    hdrsrchacq_enable_tune_aways();
      /* Enable tune aways. */
  }

  hdrsrchlog.acq_info.rf_warmup_tune_time_ms =
    hdrsrchacq_measure_time( );
    /* Measure the RF warmup and tune time here  */

   /* RF tune is complete and AGC loops have settled so lets send searches.
     Figure out which mode to use for sending searches */

  hdrsrchrf_get_rx_power( &rx0_agc, &rx1_agc, &diversity );
    /* get power measurements */

  agc = MAX( rx0_agc, rx1_agc );
      /* Get the maximum of the AGCs for quick look at rx power */

  if (  hdrsrchacq.params.mode == SYS_HDR_ACQ_MODE_MICRO )
  {

    if ( agc < HDRSRCHACQ_MIN_AGC_FOR_FULL_ACQ )
    {
#ifdef FEATURE_HDR_REVB
      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Micro scan failed. Rx=%ddBm ch=(%d %d)",
                      agc, 
  hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list )->chan_num,
  hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, &chan_cnt, &demod_list )->band );
#else
      HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Micro scan failed. Rx=%ddBm ch=(%d %d)",
                      agc, 
                      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN )->chan_num,
                      hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN )->band );
#endif /* FEATURE_HDR_REVB */

      search_ok = FALSE;
        /* pilot is too weak to pass micro acquisition criterion */

      hdrsrchacq_update_acq_info( HDRLOG_ACQ_MICRO_FAILURE );
        /* Update ACQ INFO global structure                  */

      hdrsrchacq_failed();
        /* declare micro_and_full acquisition attempt failed */
    }
    else
    {
      HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "Micro scan success. Rx=%ddBm A:%d->%d",
               agc, hdrsrchacq.params.mode, SYS_HDR_ACQ_MODE_FULL );

#ifdef FEATURE_XO
      hdrsrchacq.depth = HDRSRCHACQ_SHALLOW;
      /* Only use shallow acquisition */
#else
      hdrsrchacq.depth = HDRSRCHACQ_DEEP;
      /* Start with deep acquisition */
#endif /* FEATURE_XO */

      hdrsrchacq.params.mode = SYS_HDR_ACQ_MODE_FULL;
        /* Since micro acquisition has been successful, perform full acq */
    }
  }
  else
  {
#ifdef FEATURE_XO
    /* Check if all coset groups have been searched.  If yes, setup for
       next iteration, if any. */
    if ( hdrsrchacq.coset_group >= HDRSRCHACQ_NUM_COSET_GROUPS )
    {
      /* Finished searching all 4 coset groups. */
      hdrsrchacq.coset_group = HDRSRCHACQ_COSET_GROUP_0;
      hdrsrchacq.coset_idx = 0;
      hdrsrchacq.coset_group_searched = FALSE;
    }
#else
    if ( hdrsrchacq.depth < HDRSRCHACQ_NUM_DEPTHS )
    {

      if ( hdrsrchacq.coset_group == HDRSRCHACQ_NUM_COSET_GROUPS )
      {
        hdrsrchacq.coset_group = HDRSRCHACQ_COSET_GROUP_0;
        hdrsrchacq.coset_idx = 0;
        hdrsrchacq.coset_group_searched = FALSE;
      }
    }
#endif /* FEATURE_XO */
  }

#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV
  if ( HDRSRCHACQ_ACQ_DIV_ENABLED )
  {
     HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "ACQ Div:  RxAGC0=%ddBm", rx0_agc);
     if ( rx0_agc < hdrsrch.acq_div.acq_div_thresh0 )
     {
       if ( hdrsrch_is_lre_enabled() )
       {
         hdrsrchrf_get_retain_lock_state( HDRSRCHRF_PRI_CHAIN, &rl_state );
         /* Retrieve the retain lock state. */
       }

       /* Must already own RF. Check unlock events. */
       if ( ( hdrsrch_is_lre_enabled() ) &&
            ( rl_state.event == HDRSRCHRF_UNLOCK_IMMEDIATELY ) )
       {
          hdrsrchacq_pri_chain_event();
          /* Avoid delay of processing unlock immediate */

          return;
       }
       else 
       { 
       HDR_MSG_SRCH_1( MSG_LEGACY_HIGH,
                       "ACQ Div:  enable diversity acq_div_thresh0=%d",
                       hdrsrch.acq_div.acq_div_thresh0 );

       hdrsrchrfdiv_diversity_ctrl( TRUE ); 
         /* Enable Diversity */ 
       
       hdrsrch_add_to_wait_mask( HDRSRCH_GP_TMR1_SIG );   
       (void) rex_set_timer( &hdrsrch.gp_timer1, HDRSRCHACQ_DIVERSITY_ENABLE_TIME );
         /* Wait a few ms for TRM grant and diversity enable */
     }
   }
  }
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

  if ( search_ok )
  {
    hdrsrchacq_start_predetect( );
      /* Now that we are here, let's search!  */

    hdrsrch_add_to_wait_mask( HDRSRCH_DUMP_SIG | HDRSRCH_LOSTDUMP_SIG );
      /* Wait for Dump signals, and perhaps Lost Dump signals */
  }

  hdrsrchlog_debug_display_fill_rec();
    /* Update HDR debug display record */

} /* hdrsrchacq_tune() */

/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_SLAM_TIMEOUT

DESCRIPTION
  This function processes slam timeout and failed slam.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchacq_slam_timeout( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  HDR_MSG_SRCH( MSG_LEGACY_MED, "ACQ Slam failure - timeout" );

  if ( hdrsrchutil_is_brute_force_acq() == TRUE )
  {
    hdrsrchfing_deassign_all_fings();
    /* Deassign finger */

    ( void ) hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US,
                                          HDRSRCH_FING_ASSIGN_RSP );
       /* Wait for 1.8ms to ensure that all fingers are deassigned */

     hdrsrchacq_failed( ); 
  }
  else
  {
    hdrsrchacq_stage2_failed( HDRLOG_ACQ_SLAM_FAILURE );
      /* declare stage 2 failed */
  }

} /* hdrsrchacq_slam_timeout */

/*==========================================================================

FUNCTION        HDRSRCHACQ_GP_TIMER1

DESCRIPTION
  This function processes gp_time1 expiration event.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

LOCAL void hdrsrchacq_gp_time1( void )
{
#ifdef FEATURE_HDR_DYNAMIC_ACQ_DIV

  int               rx0_agc, rx1_agc;
    /* Rx0 and Rx1 AGC measurements */

  boolean           diversity;
    /* Diversity settings */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* When timer expires, we check the RxAGC of diverstiy chain */
  
  hdrsrchrf_get_rx_power( &rx0_agc, &rx1_agc, &diversity );
    /* get power measurements */
  HDR_MSG_SRCH_3( MSG_LEGACY_HIGH,
                  "ACQ Div: pri rxagc=%d diversity rxagc =%d,acq_div_thresh1=%d",
                  rx0_agc,rx1_agc, hdrsrch.acq_div.acq_div_thresh1);
  if ( rx1_agc <  hdrsrch.acq_div.acq_div_thresh1 )
  {
    /* We don't want to enable diversity chain if it is too weak */
    HDR_MSG_SRCH( MSG_LEGACY_HIGH,"ACQ Div: disable diversity");
    hdrsrchrfdiv_diversity_ctrl( FALSE );
      /* Turn off diversity */
  }
#else
  hdrsrch_default_gp_timer1();
#endif /* FEATURE_HDR_DYNAMIC_ACQ_DIV */

} /* hdrsrchacq_gp_time1 */

/*==========================================================================

FUNCTION        HDRSRCHACQ_PROCESS_SLAM_RSP

DESCRIPTION
  This function processes slam response for acquisition state.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

static void hdrsrchacq_process_slam_rsp( void )
{
#ifdef FEATURE_HDR_REVB
  uint8                             i;
    /* Loop index and channel count */

  const hdrsrch_demod_idx_type      *demod_list;
    /* A pointor to the demod index assignment from hdrsrchrf. */
#endif /* FEATURE_HDR_REVB */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrsrchacq.stage2 != HDRSRCHACQ_S2_SLAM ) 
  {
    /* Not wating for slam rsp, either slam timed out or bogus rsp */
    HDR_MSG_SRCH( MSG_LEGACY_MED, "Unexpected slam rsp" );
    return;
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Process slam rsp!! Acq redir_state: %d",
                  hdrsrchacq.redir_state );

  hdrsrchfing_read_fing_status();
    /* Read the finger status */

  DEBUG_SLAM_FING()

  hdrsrchlog.acq_info.slam_time_ms = hdrsrchacq_measure_time();

#ifndef FEATURE_HDR_REVB
    hdrsrch.aset_channel = *hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN );
      /* Acquired on this channel.  This is now the ASET channel */
#else
    hdrsrch.aset_channel[0] = 
                *hdrsrchrf_get_channel( HDRSRCHRF_PRI_CHAIN, 
                                        &hdrsrch.aset_channel_cnt,
                                        &demod_list );

    ASSERT( hdrsrch.aset_channel_cnt == 1 );
      /* Assert the channel count in the acquisition state. */

    hdrsrch.aset_demod[0] = demod_list[0];
      /* Acquired this channel.  This is now the ASET channel. From this point,
         the aset_channel and aset_demod are updated. */

    /* Update ASET info */
    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].channel = 
                                                    hdrsrch.aset_channel[0];

    hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].demod_idx = 
                                                      hdrsrch.aset_demod[0];

    for( i=0; i<hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset_cnt; i++ )
    {
      hdrsrchset.subaset[HDRSRCH_IDLE_SUBASET_IDX].aset[i]->chan = 
                                                    hdrsrch.aset_channel[0];
    }

#endif /* FEATURE_HDR_REVB */

    hdrsrchacq_update_acq_info( HDRLOG_ACQ_SLAM_SUCCESS );
      /* Update ACQ INFO global structure                  */

    hdrsrchacq_stop_timer( &hdrsrchacq.timer.real );
    hdrsrchacq_stop_timer( &hdrsrchacq.timer.task );
      /* Stop acquisition timers */

  if ( hdrsrchutil_is_brute_force_acq() == FALSE )
  {
    HDR_MSG_SRCH_3( MSG_LEGACY_HIGH, "HDR ACQ (M=%d) took %d ms (%d ms)",
                    hdrsrchacq.params.mode,
                    qw_lo( hdrsrchacq.timer.task.elapsed_ms),
                    qw_lo( hdrsrchacq.timer.real.elapsed_ms));
  }
  else
  {
    hdrsrchacq.brute_force_acq.acq_time = 
      qw_lo( hdrsrchacq.timer.real.elapsed_ms);

    hdrsrch.brute_force_params.time_for_acq_ms = hdrsrchacq.brute_force_acq.acq_time;

    HDR_MSG_SRCH_1( MSG_LEGACY_HIGH, "HDR brute force ACQ took %d ms",
                    hdrsrchacq.brute_force_acq.acq_time );
  }

  if ( ( hdrsrchacq.redir_state == HDRSRCHACQ_REDIR_ACQ ) ||
       ( hdrsrchacq.redir_state == HDRSRCHACQ_REDIR_ACQ_EXCHANGE ) )
  {
    HDR_MSG_SRCH( MSG_LEGACY_LOW, "HDRSRCHACQ_REDIR_SUCC" );
    hdrsrchacq.redir_state = HDRSRCHACQ_REDIR_SUCC;
    /* Redirection successful */
  }

  hdrsrchrf_update_priority(HDRSRCHRF_ACQUISITION);
  /* Go back to normal priority mode */

  if ( hdrsrchacq.params.acq_cb != NULL )
  {
#ifdef FEATURE_HDR_REVB
    hdrsrchacq.params.acq_cb( &hdrsrch.aset_channel[0], HDRSRCH_ACQUIRED );
      /* Declare acquisition successful */
#else
    hdrsrchacq.params.acq_cb( &hdrsrch.aset_channel, HDRSRCH_ACQUIRED );
      /* Declare acquisition successful */
#endif /* FEATURE_HDR_REVB */
  }

  hdrsrchacq.tune_away.tune_away_cnt = 0;
    /* Acq is success reset the tune_away_cnt */

  hdrsrchacq.tune_away.restore_ta_cnt = FALSE;
    /* Initialize the restore flag */

  hdrsrchacq.tune_away.acq_priority = HDRSRCHRF_ACQUISITION;
    /* Reset Acq priority */

    hdrsrchsync_init_sync_state( );
      /* Enter IDLE state and wait for SCC Msg */

} /* hdrsrchacq_process_slam_rsp() */


/* EJECT */
/*==========================================================================

FUNCTION HDRSRCHACQ_PROCESS_INT_CMD

DESCRIPTION
  This function processes internal commands for acq state.

DEPENDENCIES
  None

INPUTS
  payload   - Command+payload to be processed

RETURN VALUE
  None
  
SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchacq_process_int_cmd
(
  hdrsrchsm_cmd_payload_type                     *cmd_payload
    /* The internal command + payload */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  switch ( cmd_payload->cmd )
  {
    case HDRSRCH_AFC_OPEN_LOOP_CMD:
      
      /* AFC is in open loop and HDR can start search */
      if ( hdrsrchacq.stage1 == HDRSRCHACQ_S1_WAITING_FOR_TCXO_OPEN_LOOP )
      {
      
        hdrsrchacq.stage1 = HDRSRCHACQ_S1_IDLE;
          /* Now return to the idle state until the tune is completed.
             Then we'll move to the predetect stage. */

        #ifdef FEATURE_XO

        (void) hdrsrchacq_set_binning_mode(
          (hdrsrchafc_xo_cal_mode_enum_type)cmd_payload->payload.open_loop.xo_cal_mode,
          cmd_payload->payload.open_loop.ppm );
          /* Set binning mode based on xo_cal_mode*/

        #endif /* FEATURE_XO */

        hdrsrchacq_start_acq();
          /* Now that we have TCXO going, start acquisition. */
      }
      break;

    case HDRSRCH_AFC_CLOSE_LOOP_CMD:
      if ( hdrsrchutil_is_brute_force_acq() == FALSE )
      {
        /* For regular acquire */
        /* AFC is in close loop and HDR can start pullin */
        if ( hdrsrchacq.stage2 == HDRSRCHACQ_S2_WAITING_FOR_TCXO_CLOSE_LOOP )
        {
          hdrsrchacq.stage2 = HDRSRCHACQ_S2_IDLE;
            /* Now return to the idle state until we assigne fingers.
               Then we'll move to the pullin stage. */

          hdrsrchacq_start_pullin( );
            /* Pull in on best peak */

#ifndef FEATURE_XO
          hdrsrchacq_next_predetect( ); /* Enable parallel searching */
            /* Once pullin has started - begin next predetect */
#endif /* !FEATURE_XO */
        }
      }
      else
      {
        /* For brute force acq. CLOSE_LOOP_CMD indicates continue tracking has 
           started. */
        HDR_MSG_SRCH( MSG_LEGACY_HIGH, 
                      " IRAT RTT: receive HDRSRCH_AFC_CLOSE_LOOP_CMD");
        hdrsrchacq_brute_force_acq_assign_finger( );
          /* Assing fingers and create ASET for brute force acq */

        hdrsrchacq_start_slam( );
          /* Start slam */
      }
      break;

    case HDRSRCH_AFC_ACQ_STARTED_CMD:
      
      /* Pullin started */

#ifndef FEATURE_XO
      hdrsrchacq_next_predetect( ); /* Enable parallel searching */
        /* Once pullin has started - begin next predetect */
#endif /* !FEATURE_XO */

      break;
    
    case HDRSRCH_AFC_ACQ_SUCCEEDED_CMD:
      
      /* Pullin suceeded */
      
      hdrsrchdrv_abort_search( FALSE );
        /* Abort parallel predetect search, if one is in progress.
           We don't care about the search results, but we will need
           the dump to keep the searcher state machine primed. */

      hdrsrchacq.stage1 = HDRSRCHACQ_S1_IDLE;
        /* Set stage1 to idle since predetect was aborted */

      hdrsrchacq_start_slam();
        /* Pull-in complete, now start MSTR slamming */

      break;

    case HDRSRCH_AFC_ACQ_FAILED_CMD:
      
      /* Pullin failed */

      hdrsrchacq_stage2_failed( HDRLOG_ACQ_FLL_FAILURE );
        /* declare stage 2 failed */

      break;

    default:
      HDR_MSG_SRCH_1( MSG_LEGACY_ERROR, 
                      "Unexpected internal cmd %d at ACQ state",
                      cmd_payload->cmd );
      break;
  }  /* switch */

}  /* hdrsrchacq_process_int_cmd */


/* EJECT */
/*==========================================================================

FUNCTION        HDRSRCHACQ_INIT_ACQ_STATE

DESCRIPTION
  This function switches the HDR Searcher state to Acquisition Mode.

DEPENDENCIES
  Command parameters must be left pending the the ACQUIRE command

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchacq_init_acq_state( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  hdrsrch_change_state( &hdrsrchacq_state );
    /* Enter the acquisition substate */

} /* hdrsrchacq_init_acq_state() */


/*==========================================================================

FUNCTION        HDRSRCHACQ_STAGE2_WAITING_FOR_CLOSE_LOOP    

DESCRIPTION
  This function checks whether stage2 is waiting for close loop.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  TRUE: stage2 is waiting for close loop.
  FALSE: otherwise.

SIDE EFFECTS
  None

==========================================================================*/

boolean hdrsrchacq_stage2_waiting_for_close_loop( void )
{

  boolean                    stage2 = FALSE;
    /* Return value */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  if ( hdrsrchacq.stage2 == HDRSRCHACQ_S2_WAITING_FOR_TCXO_CLOSE_LOOP )
  {
    stage2 = TRUE;
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                  "HDR Srch Acq stage2 is %d", 
                  hdrsrchacq.stage2 );

  return stage2;

}  /* hdrsrchacq_stage2_waiting_for_close_loop */


/*==========================================================================

FUNCTION        HDRSRCHACQ_PROCESS_LOW_PRI_RSP_IND

DESCRIPTION
  This function processes low priority responses and indications from FW.

DEPENDENCIES
  None

INPUTS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==========================================================================*/

void hdrsrchacq_process_low_pri_rsp_ind
(
   hdrsrchmsg_s_type         *msg
    /* Message */
)
{

/*-----------------------------------------------------------------------*/

  switch( msg->msg.hdr.id )
  {
    case HDR_FW_SRCH_SLAM_RSP:

      if ( ( hdrsrchacq.redir_state == HDRSRCHACQ_REDIR_TA ) ||
           ( !hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) ) )
      {
        HDR_MSG_SRCH_3( MSG_LEGACY_MED, "Ignored Slam rsp: msg %s umid 0x%8x"
                                         " Acq redir_state %d",
                    hdrsrchmsg_get_msg_name( msg->msg.hdr.id ),
                    msg->msg.hdr.id, hdrsrchacq.redir_state);
      }
      else
      {
          /* Process slam rsp for acq only if TA not started OR
             HDR has chain */
        if ( msg->msg.hdr_msg.rsp.srch_slam.slamSuccess == TRUE )
        {
          hdrsrchacq_process_slam_rsp( );
        }
        else
        {
          hdrsrchmsg_process_rcvd_rsp( HDRSRCH_MSTR_SLAM_RSP );

          HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Slam in FW failed, exit acquisition");

          hdrsrchfing_deassign_all_fings();
            /* Deassign fingers after slam failure */
          
          ( void ) hdrsrch_wait_for_high_pri_rsp( HDRSRCHFING_ASSIGNMENT_DONE_WAIT_US,
                                                HDRSRCH_FING_ASSIGN_RSP );
            /* Wait for 1.8ms to ensure that all fingers are deassigned */

          hdrsrchacq_failed();

          break;
        }
      }    

      /* Fall through */

    default:
      hdrsrch_default_process_low_pri_rsp_ind( msg );
      break;
  }
} /* hdrsrchacq_process_low_pri_rsp_ind */

/*===========================================================================

FUNCTION HDRSRCHACQ_TA_TIMER_EVENT

DESCRIPTION
  This function processes the ACQ tune away timer event.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchacq_ta_timer_event( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrsrchdrv_abort_search( FALSE );
    /* Abort pending searches, if any   */

  hdrsrchacq_disable_tune_aways();
    /* We will start tune away. We don't want to process unlock notify in the
       middle of tune away
    */

}  /* hdrsrchacq_ta_timer_event */

/*===========================================================================

FUNCTION HDRSRCHACQ_PRIORITY_INV_TIMEOUT

DESCRIPTION
  This function processes the ACQ inversion timer event

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void hdrsrchacq_priority_inv_timeout ( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  hdrsrch_stop_timer( &hdrsrch.timeout_timer );

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
    "Process Priroity Invserion timer timeout, current acquisition priority:%d", 
    hdrsrchacq.tune_away.acq_priority );
    /* Inversion timer is active and has expired */

  if ( hdrsrchacq.tune_away.acq_priority == HDRSRCHRF_ACQUISITION_INV )
  {
    if ( hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) )
    {
      hdrsrchacq.tune_away.acq_priority = HDRSRCHRF_ACQUISITION;
        /* 1 sec inversion timer expiry and hence change the priority to Acquisition */

      hdrsrchrf_change_priority( 
        HDRSRCHRF_PRI_CHAIN, 
        hdrsrchacq.tune_away.acq_priority );
    }
    else
    {
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                      "Acquisition priority already inverted %d", 
                      hdrsrchacq.tune_away.acq_priority );
    }
  }
  else if ( hdrsrchacq.tune_away.acq_priority == HDRSRCHRF_ACQUISITION )
  {  
    /* If RF still not granted to HDR then request with inversion priority */
    if ( ! hdrsrchrf_has_lock( HDRSRCHRF_PRI_CHAIN ) )
    {
      hdrsrchacq.tune_away.acq_priority = HDRSRCHRF_ACQUISITION_INV;
      /* Bump up priority level */
     
      HDR_MSG_SRCH_1( MSG_LEGACY_MED, 
                      "HDR Srch Acq bump up prio %d", 
                      hdrsrchacq.tune_away.acq_priority );

      hdrsrchrf_change_priority( 
        HDRSRCHRF_PRI_CHAIN, 
        hdrsrchacq.tune_away.acq_priority );
        /* Change to inversion priority so that the chance of getting RF chain is increased */
    }      
  }
} /* hdrsrchacq_priority_inv_timeout */

