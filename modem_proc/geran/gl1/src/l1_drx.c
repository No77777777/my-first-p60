/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      L A Y E R  1  D R X

GENERAL DESCRIPTION
   This module contains procedures to support DRX in Layer 1.
   Together these functions make up the DRX manager.

EXTERNALIZED FUNCTIONS
   gl1_drx_reset
   gl1_drx_on
   gl1_drx_off
   gl1_drx_tick
   gl1_drx_task_active
   gl1_drx_task_inactive
   gl1_drx_post_tick
   gl1_drx_require_next_tick
   gl1_drx_require_tick
   gl1_drx_release_tick
   gl1_drx_require_TX

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_drx.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
-------   ---      ---------------------------------------------------------- 
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
05/01/17   sn       CR2166914 Removing check for TRX_state
22/04/16   ksb      CR1005869 To mainlne FEATURE_GL1_SLEEP_DEBUG_STRUCT
15/05/17   akm      CR2047050 Avoid GL1 going to sleep during rude wakeup if RF wakeup CNF is not received
01/08/16   og       CR1004621. Prevent require tx from running for just two TDMA frames.
29/06/16   hd       CR1035604 Rename gl1_clear_drx_tx_status_at_multi_sim_deact
16/06/16   sk       CR1029565 No need to check drx_ref for stale activity as stale activities's tick release already taken care in gl1_drx_tick()
25/04/16   ksb      CR993771 GSM DSDS: G_data+G_Idle Scenario, dont prevent the SUB2(Idle column) to sleep after page decode is done
15/03/16   am       CR984797 Added DRX logging in the circular buffer for debug purpose
28/09/15   cjl      CR929471 Delay sleep after geran lock release in idle
09/10/15   akh      CR912421 FN/QS not correct for active ttg, sleep ttg resp delayed by 1s
29/09/15   zf       CR914601 Crash when L1 on sleep without unload firmware
16/09/15   hd       CR905810 While releasing tick decrement the no_req_ticks only when it is greater than MAX value
03/09/15   kb       CR863908 Added new EFS file structure for sleep control
19/08/15   am       CR890530 Ensure drx entry is removed
14/08/14    ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
15/07/15   fan/akh  CR857332 FR19329: WMGR support for rescheduling non-PCH activities
10/06/15   cjl      CR851285 Don't sleep during CSFB in progress
28/05/15   df       CR842242 Check min gap to sleep when BG_traffic flag is transitioning to set 
11/05/15   cja      CR827065 Handle G2W initialisation requested when G2W shutdown in progress
11/05/15   df       CR834120 Delay sleep if VSTMR frame extension in progress
07/05/15   fan/akh  CR834237 FR19329 Full G wake-up cancellation + code cleanup
01/05/15   aga      CR831392 Use the appropriate WCDMA client ID gased on static AS_ID mapping
27/04/15   js       CR821190 Gl1 should not go to sleep if QTA gap is active.
30/04/15   ip       CR799995 FR26290 Sleep when X tech is in connected state
24/04/15   fan      CR796481 Full G wakeup cancellation
20/04/15   ip       CR811868 Dual Multisim mode code changes + debug overhaul
27/02/15   cws      CR800778 Do not set asynch_int_gstmr_ignore if gl1_drx_task_sleep() is called late in the frame
16/04/15   smd      CR822202 Dual space data for IRAT and QTA
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
16/03/15   pg       CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
12/03/15   zf       CR808041: Don't sleep while QTA cleanup pending
27/02/15   am       CR646584 WLAN coex
22/01/15   zf       CR784496: Enable sleep when BG traffic enabled.
18/02/15   am       CR791074 ISR overrun check for drx task sleep
17/02/15   df       CR748521 Remove 5 frame delay to sleep when entering DRX mode
15/02/15   sp       CR729492 Remove unimportant logging
27/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
23/01/15   fan      CR785961 Change GL1_DRX_MIN_GAP_TO_SLEEP from 4 to 1 to increase the min sleep duration
15/12/14   ws       CR771244 GL1: Compile warnings for LTE,1X,GERAN build flavour
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
14/11/14   fan      Change minimum number of sleep frames to 4 just for TH1.0
28/10/14   cgc      CR745548 GL1 make gl1_drx_require_and_wait_for_next_tick() conditional on awake
05/11/14   fan      CR751146 Fix wakeup error recovery mechanism on Thor
17/10/14   sk       CR743536 Ensure gap to CCCH is updated on rude wakeup before ISR runs
15/10/14   ak       CR730234 Sleep debug code tidy up
14/10/14   df       CR676084 Sleep debug structure improvements
14/10/14   zf       CR737010 Update FN when forced wakeup
01/10/14   df       CR611845 SBD interface updates for missed wakeup
20/06/14   jj       CR656872 fix critical gl1 kw error 
25/08/14   df       CR660576 Enhanced timeline debug
21/08/14   df       CR712987 Change GL1_DRX_MIN_GAP_TO_SLEEP from 4 to 1 to increase the min sleep duration
24/07/14   nk       CR699150 W2G IRAT timeline improvement (remove F3s from GL1)
08/08/14   zf       CR706869: Correction of active task count update in Async interrupt handling
12/06/14   fan      CR677961- run ISR at the requested frame number when other sub is in transfer
08/08/14   df       CR590590 Reducing DRX MIN_GAP_TO_SLEEP from 10 frames to 4 frames
06/08/14   df       CR598015 Avoid silent frame DRX processing when pending firmware activity trigger is set
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
03/07/14   zf       CR689783: Don't do DRX task sleep if it is not awake
11/06/14   npt      CR630396 Wake-up error recovery mechanism
18/06/14   ssh      CR598637: Missing Merge
17/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
04/06/14   ssh      CR671681: Reset missed frames in drx_tick if not running ISR and drx_go_to_sleep
17/04/14   gk       CR645704: G idle Sub is intermittently unable to sleep between pch decodes
15/04/14   mk       CR642199:active_task_count should not be set based on Rude wake up state of other sub
11/04/14   pg       CR645875 Fix Bolt multisim compilation errors
09/04/14   ap       CR640266 L1 is unable to sleep because X2G reselect in progress is always indicated
04/04/14   pg       CR643846 RFLM/FED architecture support for multi-SIM
04/04/14   cah      CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
17/03/14   ssh      CR631959: Remove forced rude wake up for G+G(+G) case
13/03/14   ssh      CR631223: Force rude wake for all idle subs when transfer starts in G+G(+G) mode
12/03/14   ssh      CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
12/03/14   ssh      CR629612: Do not clear total error recovery frames when it is next frame after sleep
08/03/14   am       CR624825 If TRM is retained for TBF assignment then TRM release should happen on TBF release
10/03/14   am       CR624678 TX-ON should happen in full frame
27/02/14   ip       CR608864 Dime3.0 DB timeline changes
25/02/14   am       CR615566 Activities should not clear their DRX entries based on reference
19/02/14   ap     CR600310 GSM L1 to send channel Id to GSM FW corresponding to the primary RF device
09/01/14   zf       CR598637: DRX only updates TRM booking if DRX activity is matching with the TRM booking
13/01/14   npt      CR606388 Temp disable opt3
11/02/14   ka       CR614268 Add debug to determine lack of BPLMN resume
21/11/13   dv       CR579907 Back out CR 519537
13/02/14   ap       CR612587: Ensure call RF exit GSM mode when releasing TRM in DSDA mode
24/01/14  cah       CR598552: enable sleep for DSDS GSTMR+VSTMR
06/01/14   sk       CR593161 Ensure CB reservations not made when other SUB is in Data
20/12/13   js       CR592583 : Removing compilation errors : MPSS.2.0.1 CRM for T+G (W removed)
11/12/13   ap       CR586661 GPRS suspension during SI read after W2G reselection
17/12/13   sk       CR586741 Update TRM reservation to QTA compatible one if WCDMA is in QTA mode
26/11/13   npt/ip   CR582180 Enable sleep on Bolt
26/11/13   ssh      CR581870: Force idle G sub to wake-up (enter non-drx) when W/G enters BG traffic
19/11/13   ssh/ip   CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   nm       CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
08/11/13   ip       CR 547955 useful debug for parallelisation work
05/12/13   cs       CR584551 Use correct gas_id for GPS Timetagging in SS mode
22/11/13   cja      CR455527 Remove featurisation from files in API directory
25/09/13   fan      CR537338 - QDSS logging
25/10/13   sai      CR 565999: Clear RF Tx status post RF shutdown
05/11/13   cja      CR571501 In DRX on do not turn Tx off if leaving idle in the same frame
25/10/13   sai      CR 565999: Clear RF Tx status post RF shutdown
09/10/13   js       CR519541: Add wakeup from sleep during QTA support in idle ISR state mc,
                    block sleep in QTA mode.
08/10/13   ssh      CR554677 - XO shutdown broken in G for TR 1.1
09/10/13   ip       CR537689 Reduce the minimum DRX gap for sleep to 4
02/10/13   ian/ss   CR542492 Triton|G power saving change, allow option 3 to run later in the frame, extend threshold in DRX manager
19/07/13   gk       CR515617:Don't delay the interrupt if W has the lock and its a rude wakeup
03/09/13   ip       CR536229 Use ERR_FATAL
03/09/13   fan      CR537338 - QDSS events for GL1 timeline Profiling
19/08/13   fan      CR530376 -  GL1 starts sleep before slpc OLS serviced, so that slpc can not configure sleep duration and GL1 will not wakeup
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
18/06/13   npt      CR497521 - Added VSTMR changes - temporary disable sleep for Bolt
19/08/13   fan      CR530376 -  GL1 starts sleep before slpc OLS serviced, so that slpc can not configure sleep duration and GL1 will not wakeup
13/08/13   fan      CR521324 -  Do not use the OLS sleep controller callback to reprogram the terminal count
31/07/13   sp       CR519537: Enable GSTMR F3s
25/07/13   sai      CR515382: Adjust short frame duration at wakeup from sleep.
10/07/13   ip       CR508975 - Adjust thresolds for CS power goals
12/07/13   cs       CR509747 Apply frequency drift to TRM_DENIED SUB(s)
01/07/13   sai      CR506781: Cancel wakeup only if time left until warmup int is less than 40 ms.
01/07/13   ip       CR506628 FC power code changes
25/06/13   sai      CR503864: Fix MCPM-WMGR mutex deadlock when W and G go to sleep at the same time
21/06/13   cs       Major Triton TSTS Syncup
17/06/13   sai      CR494015: Implemented GERAN wake up cancellation
12/06/13   ip       CR359544: Removed repeated debug
23/05/13   ip       CR359541 Option 3 to work in G+G and G+W modes
20/05/13   pg       CR476609 - Vote for next frame when entering DRX_AWAKE
06/06/13   cs       TSTS Updates
23/05/13   kb/zf    CR413148: Clear missed_frames in the second wakeup frame
20/05/13   npt      CR417466 - Ensure that opt3 is not kicking in
                    if change to GSM only mode is being processed
17/05/13   kb/npt   CR385548 - Decode PCH after sleep wakeup error recovery.
15/05/13   zf       CR486346: Disable sleep when CM init for QTA.
13/05/13   og       CR475689. Use the gstmr interrupt pending before running go to sleep.
02/05/13   zf       CR483023: Disable sleep in GTA gap.
29/04/13   zf       CR480972: Disable sleep while W is in BG traffic in DSDS
28/03/13   cja      CR458005 Turn on W clock for G2W BPLMN
27/03/13   ws       CR466973 - DSDS BU, Fix for LPM crash when in DSDS mode
26/03/13   ws       CR466973 - DSDS BU, Temporary disable Sleep in DSDS mode
04/02/12   og       CR424373. Don't send a tx config command when leaving idle
                    during deactivation.
14/01/13   npt      Added DRX_WAITING_TO_SLEEP check for Dime
03/01/13   npt      CR430322 - Modify sleep debug structure
06/12/12   npt      CR428630 - Maintain WCDMA frame reference count while in sleep
06/12/12   cja      CR383165 Remove compiler warnings
05/12/12   pg       CR426795: do not turn Tx on if leaving idle to deactivate
21/11/12   npt      CR423308 - Disable sleep if there are WCDMA neighbours
14/11/12   npt      Disable sleep for T_MDM9X25
09/11/12   npt      CR380795 - Added core CPU vdd NPA client handle
09/11/12   npt      CR419261 - Enable opt3 for Dime
24/10/12   npt      CR413992 - Remove definition of feature DEBUG_DRX_TRX_STATES
23/10/12   npt      CR413635 - Port sleep code for DIME modem
17/08/12   ip       CR384421 Disable GSM sleep during CCO activity
22/06/12   ip       CR362050 - Set tx state to DRX_TX_ON_ALWAYS when comming from NULL
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
24/05/12   npt      CR361228 - Updated - Print out warning message when TCXO shutdown is prevented
15/05/12   npt      CR361228 - Print out warning message when TCXO shutdown is prevented
24/04/12   npt      CR354790 - Remove old sleep debug (under DEBUG_GL1_SLEEP) as it is not needed
                    after FEATURE_GL1_SLEEP_DEBUG_STRUCT is introduced
29/03/12   cja      CR316437 Inhibit sleep on RAT change until it is complete
15/03/12   mm       CR 344531 G2W drivers new design
14/03/12   pg       Add support for QuRT
08/03/12   ip       CR326699 - Code changed for wake up recovery mechanism
10/02/12   og       CR335121 Do not go to sleep if GCF test
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
24/01/12   npt      CR330084 - Fixed boundary overrun in sleep debug structure
11/01/12   npt      CR311544 - Added sleep debug structure - FEATURE_GL1_SLEEP_DEBUG_STRUCT
11/01/12   npt      CR315272 - Remove usage of deprecated legacy sleep APIs
06/01/12   ip       CR328092 Increased the minimum time to sleep for Nikel to 20 frames
28/11/11   cja      CR321479 Do not sleep if G2W WFW active
17/11/11   cja      CR319954 Do not sleep unless GSM MCPM is idle
02/11/11   og       CR316172. Back out the fix for CR283433.
21/10/11   cja      CR311525 Turn on Tx straight away rather than on next frame tick
20/10/11   ip       CR283433 Added FEATURE_GSM_LEAVE_GP_CLK_ON, no sleep with test SIM
06/10/11   og       CR311036. Don't rude wakeup if remaining sleep frames are zero.
28/09/11   pm       Change GL1_DRX_MIN_GAP_TO_SLEEP from 6 to 10 to increase the min sleep duration
22/09/11   dv       CR 308203 - Change GL1_DRX_MIN_GAP_TO_SLEEP from 5 to 6 to avoid sleeping
                    for less than 25mS
01/09/11   ip       CR293147 Change status of trx state on drx reset to off.
31/08/11   cja      CR304401. Wait for frame tick so FW not deactivated when suspended.
24/05/11   cja      Ensure for TxLM have gl1_hw_turn_tx_off
03/05/11   cja      TxLM - ensure gl1_hw_turn_tx_on is included
04/04/11   nf       CR280679 added gl1_drx_get_next_tick_req
14/02/11   cs       Slight redesign of asynch int to allow for a silent frame
24/06/11   nf       CR289274 ported
20/06/11   cs       DSDS CR287999 Allow sleep to start upon completion of reserve_at
14/06/11   npt      CR280679 added gl1_drx_get_next_tick_req
14/06/11   npt      Slight redesign of asynch int to allow for a silent frame
                    interrupt for the frame after a failed asynch attempt
14/06/11   npt      CR287491 Move post tick processing for asynch into DPC to
                    allow for mdsp sleep cmd isr to be processed
24/01/11   nt       Reintroduced the FEE change for DSDS code that was removed
21/01/11   cs       CR264682 DSDS Add support for asynch pch completion interrupt
09/12/10   cs       CR264682 Add support for asynch pch completion interrupt
17/08/10   ip       CR251246 Do not allow sleep to start from task context
23/07/10   ip       CR245474 Added Sleep debug code
24/06/10   nf       Ported over CR215981
10/06/10   nf       Disable sleep for Voyager via NV Item 5107
07/06/10   og       Disable sleep for voyager 2.0 IRAT.
04/06/10   ip       CR240251 - Updates to the FEE calculation
23/11/09   ip       Call DAL_Device_attach from task context, and TRX state debug
09/11/09   ip       Allow Sleep on SCMM
13/10/09   ws       Fixed case of DALDeviceId.h
17/09/09   ps       Resolve KW ABR errors
14/08/09   ws       Fix crash is previous checkin such that DAL_DeviceAttach()
                    is called before DalSoftwareSwitches_GetMask()
28/07/09   tjw      Use new core BSP API to access BIO_SW that disables sleep
13/07/09   nf       Added DRX State to stop a race condition
01/07/09   nf       Reorganised DRX logging
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
12/02/09   ap       Fix compiler warning
30/01/09   nf       CR168086 - No Sleep while W-BPLMN active
30/01/09   ip       CR 167135 Added API call for sleep debug code
14/01/09   cs       For MUSTANG force disabling of GL1 sleep
19/12/08   ip       CR167134 Added DRX logging states
02/10/08   cs       Lint fixes
23/09/08   cs       CR149660 Updated Core LPM that supports UMTS
22/08/08   ws       Moved include l1_utils.h outside of CGPS define
06/08/08   nf       Changed MSG_ERR to ERR_FATAL in gl1_drx_tick
28/07/98   hv       Added KxMutex support
23/05/08   ip       CR14710 Fix for GCF 42.4.6.3
09/04/08   ip       Allow GSM sleep for LCU
22/01/08   ws       Disable sleep for LCU until it's been sufficiently tested
12/10/07   cs       CR129559 Add support for asserting sleep at task startup
31/08/07   ws       Fixed Klocwork warning
02/03/07   cs       Add support for new CGPS timetag
03/29/06   gfr      Added gl1_drx_first_awake_frame
10/02/05   gfr      Fixed sleep timetag featurization
08/31/05   gfr      Use gl1_hw_enter/leave_idle functions which exist
                    independently of FEATURE_GSM_SLEEP_CONTROLLER
08/04/05   sv       Flush out invalid frame number when gap to sleep is very big.
06/28/05   gw       Added DRX_TX_TURNING_OFF state to allow NPL1 time to turn
                    tx chain off.
06/16/05   sv       Added a check when we go to sleep for INVALID number of frames.
06/07/05   gw       Extended TX state machine so that tx turn on is delayed
                    until ISR is running. Removed some obsolete code.
06/08/05   pp       Reduced GL1_DRX_MIN_GAP_TO_SLEEP to 5 frames from 20 frames.
05/25/05   gw       Suppress Lint error.
05/12/05   gw/pp    Added INTLOCK/INTFREE in gl1_drx_post_tick().  Increased
                    minimum gap to sleep from 5 frames to 20 frames to allow
                    for bigger warm up times on MSM6275.  This should be
                    shortened again soon.
05/04/05   gfr      Compiler and lint warning cleanup related to sleep timetag
03/30/05   gfr      Added support for sleep timetag
01/04/05   gw       Make sure that DRX_TX_state is always consistent even if
                    QXDM dip switch is used to turn sleep off and on.
05/26/04   sv       Lint Cleanup.
05/12/04   gw       Vote against TCXO shutdown when initiating forced wake up.
02/23/04   gw       Lint clean up.
09/09/03   gw       Modified gl1_drx_task_{in}active() to count tasks when
                    DRX manager is off too. Also added INTLOCK/INTFREE in
                    gl1_drx_task_active().
09/09/03   dlh      Removed pl1_52_idle_wait as no longer needed.
08/21/03   gw       Added correct handling of gl1_drx_require_next_tick() in
                    DRX_WAITING_FOR_FEE state.
07/21/03   dlh      added pl1_52_idle_wait for FEE when in 52 idle.
07/12/03   dlh      changed MAX_NO_REQ_TICKS from 3 to 5 for 52 mode.
07/11/03   gfr      Lint cleanup.
07/11/03   gw       Added handling for gl1_drx_require_tick() when already
                    going to sleep.
07/08/03   drl/mk   Added FEATURE_GSM_RF_SLEEP
06/19/03   gw       Changed GL1_DRX_MIN_GAP_TO_SLEEP from 10 to 5 frames.
06/03/03   gw       Cleaned up debug messages.
05/23/03   gw       Added code to disable LCD during Idle Mode under
                    FEATURE_BIO_LCD_CONTROL.
05/21/03   gw       Added code to prevent LED blinking during Idle Mode under
                    FEATURE_LED_POWER_CONTROL.
05/01/03   ATM      Moved Frame Number management entirely inside GTMRS
                    fixed frame number miscalculation.
04/30/03   gw       Added check to gl1_drx_require_tick() to catch case where
                    current FN is required.
04/03/03   drl      Removed Warning when building MSM6300 code.
03/10/03   gw       Improved error checking in gl1_drx_require_tick().
02/25/03   gw       Added function gl1_drx_require_TX() to allow Tx chain to
                    remain on when entering Idle Mode - required for GPRS.
02/12/03   gw       Added support for FEE timing error functionality.
01/02/03   gw       Added support for starting sleep timeline from the task
                    context.
12/06/02   gw       Added counting of awake frames to support new version of
                    gl1_hw_sleep_gsm_start_sleep().
11/13/02   gw       Synced up pretend sleep with new frame number counting
                    scheme introduced with gl1_drx_post_tick().
11/08/02   gw       Added code to update counters by one frame when
                    gl1_drx_go_to_sleep() returns FALSE.
10/30/02   gw       Added function gl1_drx_post_tick() as part of optimization
                    of sleep time line.
10/01/02   ATM      Added central get and set FN routines
10/11/02   gw       Changed going to sleep to support hw layer delaying sleep
                    e.g. to finish processing SBI transactions.
10/04/02   gw       Added code to invert uart interrupt polarity during Idle
                    Mode. Added support for FEATURE_SLEEP_CTRL_VIA_QXDM_DIPSW.
07/23/02   gw       Increased GL1_DRX_MIN_GAP_TO_SLEEP.  Changes to support
                    changed timing of clock switching in drivers.
07/09/02   gw       Revised support for rude wake up.  Removed precompiler
                    statements to define DEBUG_DRX_LEDS.
06/25/02   gw       Changed macros used to communicate with sleep task.
06/10/02   gw       Added new states to add support for rude wakeup and FEE.
                    Made pretend sleep more like real sleep. Removed parameter
                    from gl1_drx_tick().  Changed GSM_SLEEP to
                    FEATURE_GSM_SLEEP_CONTROLLER.
                    Added #include of customer.h.
04/24/02   JC/gw    Added calls to gl1_msg_adjust_time_stamp() so that message
                    layer FN counter is maintained during DRX.  This is needed
                    for AGC slew rate limiting.
03/09/02   gw       Added fix to stop ISR sleeping when it should be switching
                    to Random Access Mode. Only works when sleep controller is
                    not being used.
02/19/02   gw       Improved error handling.
01/28/02   gw       Disabled DRX_LEDs when TIMETEST is defined.
10/08/01   gw       Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "geran_tasks.h"
#include "l1_drx.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1i.h"
#include "geran_msgs.h"
#include "rex.h"
#include "l1_os.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#ifdef FEATURE_SLEEP_CTRL_VIA_QXDM_DIPSW
#include "DDISoftwareSwitches.h"
#include "DALSys.h"
#include "DALDeviceId.h"
#endif

#include "l1_log.h"     /* For logging */
#include "vtmrs_g.h"
#include "gtmrs.h"
#include "gtmrs_g.h"      /* for GSTMR_GET_FN_GERAN( gas_id) */
#include "l1_utils.h"
#include "l1_sc_int.h"

#include "gl1lsmif.h"

#include "gpl1_dual_sim.h"
#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "gpl1_grm_intf.h"

#include "gl1_hw_clk_ctl.h"

#include "gl1_msg_wcdma_meas.h"

#include "gl1_hw_sleep_ctl.h"

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
#include "tracer.h"
#include "gdrivers_tracer_event_ids.h"
#include "tracer_config.h"
#endif
#include "gl1_msg_g.h"
#include "geran_dual_sim_g.h"

#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#endif

#include "gl1_mutex.h"

/*****************************************************

 LOCAL DEFINES

******************************************************/
#define GL1_DRX_MIN_GAP_TO_SLEEP 1
#define GL1_DRX_MAX_GAP_TO_SLEEP (21*MULTIFRAME_51) /* (5 seconds) */

#define MAX_NO_REQ_TICKS         5
#define EMPTY_FRAME_NO           0xFFFFFFF0

/* This value is used to determine which of two  */
/* fame numbers is earlier.  A lower frame       */
/* number may be later if the frame number just  */
/* wrapped around.                               */
#define FN_WRAP_THRESHOLD        (FRAMES_IN_HYPERFRAME/2)
#define FN_MAX_PLUS_ONE          FRAMES_IN_HYPERFRAME

#define GL1_DRX_BIG_TE_THRESHOLD 24 /* quarter symbols */
#define GL1_DRX_MAX_NUM_BIG_TE    2 /* was 4, but now with early decode and SBD changed to 2 */
#define GL1_TE_FAILURE_COUNT      2

/* Include DRX state logging in GINT builds */

#define DEBUG_DRX_TRX_STATES

/* DRX logging macros */
#define GL1_DRX_REQ_TICK      1
#define GL1_DRX_REL_TICK      0
#define GL1_DRX_INVALID_REF   0
#define GL1_DRX_INVALID_FRAME 0xFFFFFFFF

/* General messgae header for sleep controller debug */
#define GL1_DRX_HDR "GL1_DRX_MGR: "
/*****************************************************

 LOCAL STORAGE

******************************************************/

typedef enum {
   DRX_NULL=0,
   DRX_AWAKE,
   DRX_GOING_TO_SLEEP,
   DRX_ASLEEP,
   DRX_WARMING_UP,
   DRX_RUDE_WAKEUP,
   DRX_WAKEUP_WHILE_GOING_TO_SLEEP
} l1_drx_state_id;

static l1_drx_state_id gl1_DRX_state[NUM_GERAN_DATA_SPACES];

typedef enum {
   DRX_TX_OFF=0,
   DRX_TX_ON_TIMED,
   DRX_TX_ON_ALWAYS,
   DRX_TX_TURNING_ON_ALWAYS,
   DRX_TX_TURNING_OFF
} l1_trx_state_id;

static uint32 tx_down_count[NUM_GERAN_DATA_SPACES];
static l1_trx_state_id TRX_state[NUM_GERAN_DATA_SPACES];

/* List frame numbers when ticks are required     */
/* The reference passed in and out of the DRX     */
/* manager is the index into this array + 1.      */
static uint32  req_frame_nos[NUM_GERAN_DATA_SPACES][MAX_NO_REQ_TICKS];

/* List of indexes into frame number array sorted */
/* to have soonest first.  The index is offset by */
/* 1 and 0 represents no entry.                   */
static uint8   req_frame_no_order[NUM_GERAN_DATA_SPACES][MAX_NO_REQ_TICKS];

/* List of references for tasks that are running. */
static uint8   active_task_count[NUM_GERAN_DATA_SPACES];

static uint8   no_req_ticks[NUM_GERAN_DATA_SPACES];
static boolean next_tick_required[NUM_GERAN_DATA_SPACES];

/* Used to hold the number of frames remaining in
 * original sleep period when we do a rude wakeup.
 */
static uint32 remaining_frames[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };

/* Used to count how many frames since we last
   woke up.  Sometimes the hw layer needs this
   info.
 */
static uint32 frames_awake[NUM_GERAN_DATA_SPACES];

/* Holds the number of rames to adjust sleep gap for in fast pch */
static int16 gl1_asynch_pch_offset[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };

/*
 * Used to control if we execute the next gstmr interrupt normally
 * or if the asynch int has failed then we have already run the ISR
 * relevant processing in the current frame so no need to run again
 * in the next ISR.
 */
static boolean gl1_asynch_int_gstmr_ignore[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static boolean sleep_timetag_pending[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

#if defined( DEBUG_DRX_TRX_STATES )
#include "timetick.h"
#define GL1_LOG_BUFFER_SIZE 12

typedef struct
{
  l1_drx_state_id        drx_state;

  ustmr_type            start_ustmr;

  /* Timestamp of event*/
  timetick_type         gl1_timestamp;

  /* GSM Frame Number */
  uint32                frame_number;

} gl1_drx_event_type;

typedef struct
{
  /* Index of last event */
  uint32                last;

  /* Buffer to hold trace events */
  gl1_drx_event_type  events[GL1_LOG_BUFFER_SIZE];

} gl1_drx_type;

typedef struct
{
  l1_trx_state_id        trx_state;

  ustmr_type            start_ustmr;

  /* Timestamp of event*/
  timetick_type         gl1_timestamp;

  /* GSM Frame Number */
  uint32                frame_number;

} gl1_trx_event_type;

typedef struct
{
  /* Index of last event */
  uint32                last;

  /* Buffer to hold trace events */
  gl1_trx_event_type  events[GL1_LOG_BUFFER_SIZE];

} gl1_trx_type;

void gl1_drx_state_debug ( l1_drx_state_id drx_state, gas_id_t gas_id );
void gl1_trx_state_debug ( l1_trx_state_id trx_state, gas_id_t gas_id );

#define GL1_DRX_STATE_LOGGING( drx_state, gas_id )\
          gl1_drx_state_debug( drx_state, gas_id );

#define GL1_TRX_STATE_LOGGING( trx_state, gas_id )\
          gl1_trx_state_debug( trx_state, gas_id );

#else
#define GL1_DRX_STATE_LOGGING( drx_state, gas_id )
#define GL1_TRX_STATE_LOGGING( trx_state, gas_id )
#endif

#if defined( DEBUG_DRX_TRX_STATES )
gl1_drx_type gl1_drx_state[NUM_GERAN_DATA_SPACES];
gl1_trx_type gl1_trx_state[NUM_GERAN_DATA_SPACES];
#endif

boolean sleep_active[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( TRUE ) };
boolean gl1_drx_ignore_next_frame_int[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE( FALSE ) };

#if  defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
/* this variable indicates whether Compressed Mode is active */
static l1_drx_state_id cm_drx_state[NUM_GERAN_DATA_SPACES] =
                         { INITIAL_VALUE( DRX_NULL ) };
#endif /* FEATURE_COMPRESSED_MODE */

volatile boolean  avoid_gps_update[NUM_GERAN_DATA_SPACES] =
                    { INITIAL_VALUE( FALSE ) };

boolean gl1_drx_is_gsm_asleep( gas_id_t gas_id );

extern uint16 gl1_sleep_cnt[NUM_GERAN_DATA_SPACES];
extern gl1_hw_sleep_type gl1_hw_sleep_struct[NUM_GERAN_DATA_SPACES][NUM_OF_GL1_DEBUG_CYCLES];

static boolean gl1_drx_sleep_inhibit[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static boolean gl1_drx_sleep_cco_inhibit[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static boolean gl1_drx_sleep_trm_inhibit[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

extern boolean gl1_hw_sleep_wmgr_cancel_in_prog[NUM_GERAN_DATA_SPACES];

extern uint32 gl1_hw_wkupmgr_wakeup_point_fn[NUM_GERAN_DATA_SPACES];

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/
static boolean gl1_drx_is_frame_num_later( uint32 fn_a, uint32 fn_b);
static void gl1_drx_remove_req_frame_no( uint8 index, gas_id_t gas_id );

static boolean gl1_drx_go_to_sleep( uint32 num_frames, boolean check_too_early, uint32 wakeup_fn, gas_id_t gas_id );
static void gl1_drx_wake_up( gas_id_t gas_id );
static void gl1_drx_turn_tx_on( gas_id_t gas_id );
static boolean gl1_drx_turn_tx_off( gas_id_t gas_id );
void gl1_drx_start_int_callback( gas_id_t gas_id );
static void gl1_drx_warmup_int_callback( gas_id_t gas_id );
static void gl1_drx_set_drx_state ( l1_drx_state_id state, gas_id_t gas_id );

int16 gl1_drx_get_pch_offset( gas_id_t gas_id );
void gl1_drx_update_counters( int32 bump, boolean do_gstmr, gas_id_t gas_id );

boolean gl1_drx_asynch_int_sleep_possible_precheck( gas_id_t gas_id );

#if  (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) ) && defined (FEATURE_WCDMA)
static boolean gl1_drx_is_cm_mode_enabled( gas_id_t gas_id );
#endif /* ( FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM ) & FEATURE_COMPRESSED_MODE */

static void l1_drx_log( uint8 drx_req_rel, uint32 frame_number, uint8 ref, l1_drx_ref_error_T error, gas_id_t gas_id );

/*===========================================================================

FUNCTION gl1_drx_sleep_efs_flag

DESCRIPTION
    Reads L1 EFS variable

===========================================================================*/
static void gl1_drx_sleep_efs_flag (gas_id_t gas_id)
{
  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_SLEEP ) )
  {
    sleep_active[gas_id] = FALSE;
  }
  else
  {
    sleep_active[gas_id] = TRUE;
  }
}

/*===========================================================================

FUNCTION gl1_sleep_active

DESCRIPTION
    TRUE if sleep is active

===========================================================================*/
boolean gl1_sleep_active ( gas_id_t gas_id )
{
    return(sleep_active[gas_id]);
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION gl1_drx_rf_reinitialised

DESCRIPTION
  Calling task can identify if DRX is off/disabled
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
void gl1_drx_rf_reinitialised( gas_id_t gas_id )
{
  if ( TRX_state[gas_id] != DRX_TX_OFF )
  {
    MSG_GERAN_HIGH_1_G(GL1_DRX_HDR" gl1_drx_rf_reinitialised - from %d to DRX_TX_OFF",TRX_state[gas_id]);
  }

  TRX_state[gas_id] = DRX_TX_OFF;
}

/*===========================================================================

FUNCTION gl1_drx_cm_starting

DESCRIPTION
  This function is called by gl1_hw_cm_gsm_init().

DEPENDENCIES
  This function is called only when FEATURE_DUAL_SIM is defined, but can be
  called in either Dual or Single Standby runtime modes

RETURN VALUE
  TRUE if GSM sleep can be asserted by the CM init function

SIDE EFFECTS


===========================================================================*/
boolean gl1_drx_cm_starting(gas_id_t gas_id)
{
  boolean                  assert_okts;
  trm_get_info_return_type get_grant;

  /* if WCDMA client holds the TRM lock, Dual Standby is now active */
  /* Note that this is NOT the same as gl1_msg_get_multi_sim_mode(), */
  /* since WCDMA may be online before GL1 knows if DSDS is required */
  if ( TRM_DENIAL != grm_get_granted( WCDMA_CLIENT_FOR_THIS_SUB(gas_id), &get_grant, gas_id ) )
  {
    boolean msim_enabled = gl1_msg_get_multi_sim_mode();

    if (cm_drx_state[gas_id] != DRX_RUDE_WAKEUP)
    {
      /* set global state flag to indicate that CM init has occurred in DSDS */
      cm_drx_state[gas_id] = DRX_RUDE_WAKEUP;
    }

#ifdef FEATURE_WCDMA
    /* ensure the sleep module does not try to maintain the WCDMA timeline */
    gl1_hw_sleep_wcdma_active(TRUE, gas_id);
#endif

    MSG_GERAN_HIGH_3_G(GL1_DRX_HDR" CM starting in DRX state=%d, task_count=%d (MSIM=%d)",
                       (int)gl1_DRX_state[gas_id], (int)active_task_count[gas_id], (int)msim_enabled);

    if (gl1_DRX_state[gas_id] != DRX_NULL)
    {
      /* require the next tick, which will execute rude */
      /* wakeup and prevent GL1 from returning to sleep */
      gl1_drx_require_next_tick(gas_id);

      /* GL1 must prevent sleep because WL1 is using the GSTMR */
      assert_okts = FALSE;
    }
    else
    {
      /* DRX state machine is inactive, no need to vote against TCXO shutdown */
    assert_okts = TRUE;
  }
  }
  else
  {
    /* Compressed Mode without the WCDMA client holding a */
    /* lock implies that Single Standby must be in effect */
    /* Note that client TRM_UMTS is used in that scenario */
    cm_drx_state[gas_id] = DRX_NULL;

    /* legacy code in cm_init() can allow GL1 to sleep */
    assert_okts = TRUE;
  }

  return(assert_okts);
}

/*===========================================================================

FUNCTION gl1_drx_cm_shutdown

DESCRIPTION
  This function is called by gl1_hw_cm_gsm_terminate()

DEPENDENCIES
  This function is called only when FEATURE_DUAL_SIM is defined, but can be
  called in either Dual or Single Standby runtime modes

RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
void gl1_drx_cm_shutdown(gas_id_t gas_id)
{
  boolean msim_enabled = gl1_msg_get_multi_sim_mode();

  /* determine whether gl1_drx_cm_starting() was previously called in DSDS mode */
  if (cm_drx_state[gas_id] == DRX_RUDE_WAKEUP)
  {
    /* reset the global state flag */
    cm_drx_state[gas_id] = DRX_NULL;
  }

#ifdef FEATURE_WCDMA
  gl1_hw_sleep_wcdma_active(FALSE, gas_id);
#endif /* FEATURE_WCDMA */

  /* output some debug of the current DRX states */
  MSG_GERAN_HIGH_3_G(GL1_DRX_HDR" CM shutdown in DRX state=%d, task_count=%d (MSIM=%d)",
                     (int)gl1_DRX_state[gas_id], (int)active_task_count[gas_id], (int)msim_enabled);

  return;
}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION gl1_is_drx_on

DESCRIPTION
  Calling task can identify if DRX is off/disabled
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
boolean gl1_is_drx_on( gas_id_t gas_id )
{
  return (gl1_DRX_state[gas_id] != DRX_NULL);
}

#if defined( DEBUG_DRX_TRX_STATES )
/*===========================================================================

FUNCTION gl1_drx_state_debug

DESCRIPTION
Logging mechanism for the DRX states

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
void gl1_drx_state_debug (l1_drx_state_id drx_state, gas_id_t gas_id)
{

  /* Pointer to current sleep event */
  gl1_drx_event_type    *msg;

  /* Circular shared memory log buffer.
     Reached end of allocated space so reset index */
  if(++gl1_drx_state[gas_id].last == GL1_LOG_BUFFER_SIZE)
  {
    gl1_drx_state[gas_id].last = 0;
  }

  /* Get pointer to current event */
  msg = &gl1_drx_state[gas_id].events[gl1_drx_state[gas_id].last];

  /* write log data into shared memory buffer with valid idx */
  msg->drx_state        = drx_state;
  GL1_ISR_SAVE_LOCK(gas_id);
  msg->start_ustmr = slpc_get_tick();
  msg->gl1_timestamp    = timetick_get_safe();
  msg->frame_number     = gl1_get_FN(gas_id);
  GL1_ISR_SAVE_UNLOCK(gas_id);
}
/*===========================================================================

FUNCTION gl1_trx_state_debug

DESCRIPTION
Logging mechanism for the TRX states

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
void gl1_trx_state_debug (l1_trx_state_id trx_state, gas_id_t gas_id)
{

  /* Pointer to current sleep event */
  gl1_trx_event_type    *msg;

  /* Circular shared memory log buffer.
     Reached end of allocated space so reset index */
  if(++gl1_trx_state[gas_id].last == GL1_LOG_BUFFER_SIZE)
  {
    gl1_trx_state[gas_id].last = 0;
  }

  /* Get pointer to current event */
  msg = &gl1_trx_state[gas_id].events[gl1_trx_state[gas_id].last];

  /* write log data into shared memory buffer with valid idx */
  msg->trx_state        = trx_state;
  GL1_ISR_SAVE_LOCK(gas_id);
  msg->start_ustmr = slpc_get_tick();
  msg->gl1_timestamp    = timetick_get_safe();
  msg->frame_number     = gl1_get_FN(gas_id);
  GL1_ISR_SAVE_UNLOCK(gas_id);
}

#endif

/*===========================================================================

FUNCTION gl1_drx_reset

DESCRIPTION
  Sets the drx state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_drx_set_drx_state ( l1_drx_state_id state, gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_drx_set_drx_state: Return Immediately");
   return;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

#if defined(DEBUG_DRX_STATES)
   #error code not present
#endif

  GL1_ISR_LOCK(gas_id);

  gl1_DRX_state[gas_id] = state;

  GL1_ISR_UNLOCK(gas_id);

#if defined( DEBUG_DRX_TRX_STATES )
   GL1_DRX_STATE_LOGGING( state, gas_id );
#endif

#if defined(DEBUG_DRX_STATES)
   #error code not present
#endif
}

/*===========================================================================

FUNCTION gl1_trx_set_trx_state

DESCRIPTION
  Sets the drx state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_drx_set_trx_state ( l1_trx_state_id state, gas_id_t gas_id )
{
   TRX_state[gas_id] = state;
#if defined( DEBUG_DRX_TRX_STATES )
   GL1_TRX_STATE_LOGGING( state, gas_id );
   // MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" TX_State %d",state);
#endif
}

void gl1_clear_drx_tx_status(gas_id_t gas_id)
{
  gl1_drx_set_trx_state ( DRX_TX_OFF, gas_id );
}

/*===========================================================================

FUNCTION gl1_trx_on

DESCRIPTION
  Sets the trx state to on for a W2G HO, allow tx disable to be called on
  entery to Idle

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_set_tx_on_always ( gas_id_t gas_id )
{
   gl1_drx_set_trx_state ( DRX_TX_ON_ALWAYS, gas_id );
}

#if ( defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM ) ) \
/*===========================================================================

FUNCTION gl1_drx_force_rude_wake_for_ta

DESCRIPTION
  This function puts all the active subs in non-drx modes if there is an active data call. Called by W/G/T/L techs

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_drx_force_rude_wake_for_ta(void)
{
   MSG_GERAN_HIGH_0(GL1_DRX_HDR" API deprecated: No more rude wake up during TA");
}

#endif
/*===========================================================================

FUNCTION gl1_drx_reset

DESCRIPTION
  Resets the DRX manager.  Should be called at init time and each time
  MS enters Idle Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  State of the DRX manager is reset.

===========================================================================*/
void gl1_drx_reset( gas_id_t gas_id )
{
#if defined(DEBUG_DRX_TRX_STATES)
   static boolean once[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

#endif
   int i;

   gl1_drx_sleep_efs_flag(gas_id);

   GL1_ISR_LOCK(gas_id);

   for ( i=0; i < MAX_NO_REQ_TICKS; i++ )
   {
      req_frame_no_order[gas_id][i] = 0;
      req_frame_nos[gas_id][i]      = EMPTY_FRAME_NO;
   }

   active_task_count[gas_id]  = 0;

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
   if(l1_get_background_wcdma_plmn_scan_active(gas_id) == TRUE)
   {
       active_task_count[gas_id]  = 1;
   }
#endif

#if  defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
   if (cm_drx_state[gas_id] == DRX_RUDE_WAKEUP)
   {
     /* WCDMA was using Compressed Mode when this function was called */
     /* so maintain the task count and prevent GL1 returning to sleep */
     active_task_count[gas_id] = 1;
   }
#endif /* FEATURE_DUAL_SIM && FEATURE_COMPRESSED_MODE */

   no_req_ticks[gas_id]       = 0;
   next_tick_required[gas_id] = FALSE;
   gl1_drx_set_drx_state ( DRX_NULL, gas_id );
   gl1_drx_set_trx_state ( DRX_TX_OFF, gas_id );
   frames_awake[gas_id]       = 0;
   tx_down_count[gas_id]      = 0;

   gl1_hw_sleep_clear_missed_frames( gas_id );

#if defined(DEBUG_DRX_TRX_STATES)
  /* Make sure we start from the beginning */
   if (!once[gas_id])
   {
     gl1_drx_state[gas_id].last = 0;
     gl1_trx_state[gas_id].last = 0;

     /* Clear the entire events buffer */
     memset(gl1_drx_state[gas_id].events, 0,
         sizeof( gl1_drx_state[gas_id].events[0] ) * GL1_LOG_BUFFER_SIZE );
     /* Clear the entire events buffer */
     memset(gl1_trx_state[gas_id].events, 0,
         sizeof( gl1_trx_state[gas_id].events[0] ) * GL1_LOG_BUFFER_SIZE );

     once[gas_id] = TRUE;
   }
#endif

   /* register the sleep start callback */
   gl1_hw_sleep_gsm_register_start_cb( gl1_drx_start_int_callback, gas_id );
   gl1_hw_sleep_gsm_register_warmup_cb( gl1_drx_warmup_int_callback, gas_id );

   GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gl1_drx_on, gl1_drx_off

DESCRIPTION
  Turn the DRX manager on and off.  When the DRX manager is off it will
  ignore calls to gl1_drx_require_tick, gl1_drx_release_tick(),
  gl1_drx_require_next_tick(), gl1_drx_task_active() and
  gl1_drx_task_inactive.  This allows these functions to be placed in code
  that may be called in other modes in addition to Idle Mode.

  gl1_drx_on() should be called when entering Idle Mode.  gl1_drx_off()
  should be called when leaving Idle Mode.  These functions rely on being
  called at these times to perform various operations e.g. starting and
  aborting the FEE.

  These functions should only be called from the L1 ISR context.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  State of the DRX manager is reset.

===========================================================================*/
void gl1_drx_on( boolean keep_tx_on, gas_id_t gas_id )
{
  /* Inform hardware layer we are entering idle mode */
  gl1_hw_enter_idle(gas_id);

  if (!keep_tx_on)
  {
    if(FALSE == gl1_drx_turn_tx_off(gas_id))
    {
      return;
    }
  }

  /* This ensures in next call to gl1_drx_tick() it will consider if sleep is possible */
  gl1_drx_set_drx_state ( DRX_AWAKE, gas_id );
  frames_awake[gas_id] = 0;
}

/* ===========================================================================*/

void gl1_drx_off(boolean  turn_tx_on, gas_id_t gas_id )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   int i;

   gl1_drx_set_drx_state ( DRX_NULL, gas_id );

   gl1_hw_sleep_clear_missed_frames( gas_id );

   if(turn_tx_on)
   {
     GL1_ISR_LOCK(gas_id);

   /* Make sure Tx chain is on - it might be already. */
     if ( (TRX_state[gas_id] != DRX_TX_ON_ALWAYS) &&
          (TRX_state[gas_id] != DRX_TX_ON_TIMED) )
   {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     /* In dual sim mode don't allow TX on unless firmware is loaded */
     if ( gl1_msg_get_multi_sim_mode() && ( l1_tskisr_blk->firmware_loaded == FALSE ) )
     {
         MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" Cannot call gl1_drx_turn_tx_on in dual sim");
     }
     else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    {
      gl1_drx_turn_tx_on(gas_id);
   }
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* In dual sim mode don't allow TX on unless firmware is loaded */
   if ( gl1_msg_get_multi_sim_mode() && ( l1_tskisr_blk->firmware_loaded == FALSE ))
   {
       MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" Cannot call gl1_drx_turn_tx_on in dual sim");
   }
   else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   {
       gl1_drx_set_trx_state ( DRX_TX_ON_ALWAYS,gas_id);
     }
     GL1_ISR_UNLOCK(gas_id);
   }

   /* Inform hardware layer we are leaving idle mode */
   gl1_hw_leave_idle(gas_id);

   /* Reset things ready for next time.
    * Don't reset active_task_count
    * since this applies when DRX is
    * off too.
    */
   for ( i=0; i < MAX_NO_REQ_TICKS; i++ )  {
      req_frame_no_order[gas_id][i] = 0;
      req_frame_nos[gas_id][i]      = EMPTY_FRAME_NO;
   }

   no_req_ticks[gas_id]       = 0;
   next_tick_required[gas_id] = FALSE;
   frames_awake[gas_id]       = 0;
   tx_down_count[gas_id]      = 0;

}

/*===========================================================================

FUNCTION gl1_drx_tick

DESCRIPTION
  Frame tick handler for the DRX manager. This should be called at the
  beginning of the frame tick ISR.

  This function will start the sleep controller time line if it determines
  that there is a large enough gap with now air interface activity.

  This function will also update the frame number when the MS wakes up
  from a period of sleep.

  The return value of this function indicates whether the remainder of
  the frame tick ISR should be executed.  It is important that this is
  observed otherwise the L1 frame number counters will be corrupted.

DEPENDENCIES
  The decision on whether or not to sleep is based on information supplied
  by gl1_require_next_tick, gl1_require_tick and gl1_task_(in)active.

RETURN VALUE
  TRUE  - the remainder of the frame tick ISR should be exectued.
  FALSE - the remainder of the frame tick ISR should not be executed.

SIDE EFFECTS
  May stop frame tick interupts and start sleep controller time line.
  Manipulates frame number counter to account for missed frame ticks.

===========================================================================*/
boolean gl1_drx_tick( gas_id_t gas_id )
{
   uint32  gap     = 0,i;
   boolean run_isr = TRUE;
   uint32  this_FN;
   uint8   missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);
   uint32  missed_this_FN;
   uint32  required_frame = EMPTY_FRAME_NO;

   gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];
   uint8   burst_index = 0;

   /* Do control of Tx - applies even when DRX manager is off. */
   switch (TRX_state[gas_id])
   {
      case DRX_TX_ON_TIMED:
         if (tx_down_count[gas_id] > 0)
         {
            tx_down_count[gas_id]--;
         }
         else
         {
            gl1_drx_turn_tx_off(gas_id);
         }
         break;

      case DRX_TX_TURNING_OFF:
         /* try to turn tx off again */
         gl1_drx_turn_tx_off(gas_id);
         break;

      case DRX_TX_TURNING_ON_ALWAYS:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         /* In dual sim mode don't allow TX on unless firmware is loaded */
         if ( gl1_msg_get_multi_sim_mode() && ( (l1_tsk_buffer[gas_id].firmware_loaded == FALSE )) )
         {
           MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" DRX_TX_TURNING_ON_ALWAYS:Cannot call gl1_drx_turn_tx_on in dual sim");
         }
         else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
         {
           gl1_drx_turn_tx_on(gas_id);
           gl1_drx_set_trx_state ( DRX_TX_ON_ALWAYS, gas_id );
         }
         break;

      case DRX_TX_OFF:
      case DRX_TX_ON_ALWAYS:
      default:
         /* do nothing */
         break;
   }

   if (gl1_DRX_state[gas_id] == DRX_NULL)
   {
      /* Not in Idle Mode - don't do DRX. */
      return TRUE;
   }


   /* Process RUDE_WAKEUP state first because
    * we need to re-adjust FN before it is used.
    */
   if (gl1_DRX_state[gas_id] == DRX_RUDE_WAKEUP)
   {
      /* now proceed with normal wakeup */
      gl1_drx_set_drx_state ( DRX_ASLEEP, gas_id );
   }

   this_FN = GSTMR_GET_FN_GERAN( gas_id);
    /* We may wakeup a few frames later due to error recovery. */
   missed_this_FN = SUB_FN(this_FN, missed_frames);

   /* Check if anyone required this tick.  This is done to
    * remove expired ticks from the list.
    */
   while ( (no_req_ticks[gas_id] > 0) &&
           ( req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] != EMPTY_FRAME_NO ) &&
           (req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] == missed_this_FN )  )
   {
      gl1_drx_remove_req_frame_no( 0, gas_id );
      next_tick_required[gas_id] = TRUE;
   }

   while ( ( no_req_ticks[gas_id] > 0 ) && ( req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] != EMPTY_FRAME_NO ) &&
        IS_FRAME_NUM_LATER( this_FN, req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] ) )
   {
      MSG_GERAN_ERROR_3_G(GL1_DRX_HDR" DRX first entry FN:%d is in the past curr FN:%d no_req_ticks:%d",
                 req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ], this_FN, no_req_ticks[gas_id] );
   
      /*Remove the DRX first entry FN which is in the past of the curr FN*/
      gl1_drx_remove_req_frame_no( 0, gas_id );
   }

   /* Disable sleep if QXDM dip switch is set. */
   if (!gl1_sleep_active(gas_id))
   {
      next_tick_required[gas_id] = TRUE;
   }

   if (TRX_state[gas_id] != DRX_TX_OFF)
   {
      next_tick_required[gas_id] = TRUE;
   }

   switch (gl1_DRX_state[gas_id])
   {
      /**********************************/
      /*          DRX_AWAKE             */
      /**********************************/
      case DRX_AWAKE:
         frames_awake[gas_id]++;

         /* check if we have allocated space to record details of current burst */
         if (frames_awake[gas_id] <= NUM_OF_GL1_DEBUG_BURSTS)
         {
           burst_index = frames_awake[gas_id] - 1;

           gl1_hw_sleep_cycle->burst[burst_index].time_ustmr   = slpc_get_tick();
           gl1_hw_sleep_cycle->burst[burst_index].frame_number = gl1_get_FN(gas_id);
           gl1_hw_sleep_cycle->burst[burst_index].frames_awake = frames_awake[gas_id];

           /* There is no duration to calculate for the first burst, but for
              subsequent bursts save the duration since the previous burst. */
           if (burst_index > 0) 
           {
             ustmr_type duration_usmtr = 0;
             duration_usmtr = gl1_hw_sleep_cycle->burst[burst_index].time_ustmr - 
                                              gl1_hw_sleep_cycle->burst[(burst_index-1)].time_ustmr;
             gl1_hw_sleep_cycle->burst[burst_index].duration_us = ((duration_usmtr * 107 ) >> 11 );

           }

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
           if (burst_index == 0) 
           {
             MSG_GERAN_HIGH_2_G(GL1_DRX_HDR" GSM burst %d, fn %d",
                                frames_awake[gas_id], 
                                gl1_hw_sleep_cycle->burst[burst_index].frame_number);
           }
           else
           {
             MSG_GERAN_HIGH_3_G(GL1_DRX_HDR" GSM burst %d, fn %d, duration since previous burst %dus",
                                frames_awake[gas_id], 
                                gl1_hw_sleep_cycle->burst[burst_index].frame_number,
                                gl1_hw_sleep_cycle->burst[burst_index].duration_us );
           }
#endif
         }

         if ( frames_awake[gas_id] == 1)
         {
             /* This is first frame after sleep so save OLS details to debug structure*/
             gl1_hw_sleep_cycle->ols.ols_delay.actually_run = slpc_get_tick();
             gl1_hw_sleep_cycle->ols.ols_delay.delay = gl1_hw_sleep_cycle->ols.ols_delay.actually_run
                                                            - gl1_hw_sleep_cycle->ols.ols_delay.programmed_to_run;
             gl1_hw_sleep_cycle->ols.ols_delay.delay_us = ((( gl1_hw_sleep_cycle->ols.ols_delay.delay ) * 107 ) >> 11 );
             gl1_hw_sleep_cycle->ols.ols_delay.frame_number = gl1_get_FN(gas_id);

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
             QDSS_logging( gas_id, GERAN_DRX_GSTMR1_START_1);
#endif /* FEATURE_GL1_SLEEP_QDSS_LOG */

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
           MSG_GERAN_HIGH_2_G(GL1_DRX_HDR" GSM GSTMR_1 fn %d ustmr delay %dus", 
                               gl1_hw_sleep_cycle->ols.ols_delay.frame_number,
                               gl1_hw_sleep_cycle->ols.ols_delay.delay_us);
#endif /* FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3 */

         } /* if(frames_awake[gas_id] == 1) */

#if defined (FEATURE_GL1_SLEEP_QDSS_LOG)
         /* check if this is the second burst after waking up */
         if ( frames_awake[gas_id] == 2)
         {
       QDSS_logging( gas_id, GERAN_DRX_GSTMR2_START_1);
         }
#endif /* FEATURE_GL1_SLEEP_QDSS_LOG */

         if ( next_tick_required[gas_id] || active_task_count[gas_id] )
         {
            /* Someone wants to do something in this frame */

           /* check if we have allocated space to record details of current burst */
           if (frames_awake[gas_id] <= NUM_OF_GL1_DEBUG_BURSTS)
           {
             gl1_hw_sleep_cycle->burst[burst_index].next_tick_req     = next_tick_required[gas_id];
             gl1_hw_sleep_cycle->burst[burst_index].active_task_count = active_task_count[gas_id];
           }

            gap = 0;

            /* Clear next tick only if task activities complete. */
            if (!active_task_count[gas_id])
            {
              next_tick_required[gas_id] = FALSE;
            }
         }
         else if (no_req_ticks[gas_id] == 0)
         {
            /* We don't need to be awake but we don't know when
             * to wake up.  We can't go to sleep.
             */
             MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" no wakeup time FN=%d", this_FN);
             gap = 0;
         }
         else
         {
            /* No one wants this frame.  Work out gap to next wanted frame.
             * The gap is defined as illustrated below.
             *
             *    |     |     |     |     |     |
             *    ^                 ^
             *    |                 next req. tick
             *    this tick
             *
             *    gap = 3
             *
             *
             */
            required_frame = req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ];
            if ( this_FN == required_frame )
	    {
	      gap = 0;
	     }
	    else if( this_FN < required_frame ) 
	    {
              gap = required_frame - this_FN;
            }
            else
            {
               gap = required_frame + FRAMES_IN_HYPERFRAME - this_FN;
         }
         }

         /* Catch the case where we go to sleep for an INVALID no of frames */
         if (gap > GL1_DRX_MAX_GAP_TO_SLEEP)
         {
            for ( i=0; i < MAX_NO_REQ_TICKS; i++ )
            {
               MSG_GERAN_ERROR_2_G(GL1_DRX_HDR" Ref%d Reqd FN%d",req_frame_no_order[gas_id][i],req_frame_nos[gas_id][i]);
            }

            /* Remove the culprit required_FN and set gap to zero; so no sleep for now */
            gl1_drx_remove_req_frame_no( 0, gas_id );
            gap = 0;
         }

         /* Can we go to sleep? */
         if (gap > GL1_DRX_MIN_GAP_TO_SLEEP)
         {
            l1_drx_state_id saved_state = gl1_DRX_state[gas_id];
            gl1_drx_set_drx_state(DRX_GOING_TO_SLEEP, gas_id);

            /* We can go to sleep */
            if(gl1_drx_go_to_sleep(gap,FALSE, required_frame, gas_id) == FALSE)
            {
                gl1_drx_set_drx_state(saved_state, gas_id);
            }
            else
            {
                /* GSTMR has been updated on this frame but
                 * other counters won't be unless we do it now.
                 */
                gl1_drx_update_counters( 1, FALSE, gas_id );

                run_isr = FALSE;

            }
         }
         else if ( gap > 0 )
         {
            /* We're not doing anything but the gap is too small
             * for us to go to sleep.
             *
             * We'll save power by not running the ISR but we also
             * need to update the counters.  But don't update
             * GSTMR since this already happened.
             */
            gl1_drx_update_counters( 1, FALSE, gas_id );

            run_isr   = FALSE;

            MSG_GERAN_HIGH_1_G(GL1_DRX_HDR" unable to sleep gap %d", gap);
         }
         else
         {
            /* We need to be awake. */
            run_isr   = TRUE;
         }
         break;

      /**********************************/
      /*          DRX_ASLEEP            */
      /**********************************/
      case DRX_ASLEEP:
      case DRX_WARMING_UP:
         /* This must be the GSTMR interrupt just after we've woken up.
          */
         gl1_drx_wake_up(gas_id);

         first_awake_frame_after_sleep[gas_id] = TRUE;
         next_tick_required[gas_id] = TRUE;

         if( gl1_hw_sleep_get_forcedwakeup(gas_id))
         {
           l1_drx_set_gap_to_CCCH_on_rude_wakeup(gas_id);
         }

         /* Update the DRX state and signal we need to run the rest of the
          * frame tick ISR.
          */
         gl1_drx_set_drx_state ( DRX_AWAKE, gas_id );
         run_isr   = TRUE;
         break;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      case DRX_GOING_TO_SLEEP:
        gl1_drx_require_next_tick(gas_id);
        break;

      case DRX_WAKEUP_WHILE_GOING_TO_SLEEP:
        if ( gl1_msg_get_multi_sim_mode() )
        {
          MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" gl1_drx_tick DRX_WAKEUP_WHILE_GOING_TO_SLEEP");
        }
        else
        {
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
          ERR_GERAN_FATAL_1_G(GL1_DRX_HDR" unexpected state DRX_WAKEUP_WHILE_GOING_TO_SLEEP, DRX_TX %d",TRX_state[gas_id]);
        }
        break;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      default:
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
          ERR_GERAN_FATAL_2_G(GL1_DRX_HDR" unexpected state DRX %d, DRX_TX %d",gl1_DRX_state[gas_id],TRX_state[gas_id]);
         break;
   } /* switch */


   /* If we are not going to run the ISR and if the missed frames are set, then clear it */
   if(!run_isr && gl1_hw_sleep_get_missed_frames(gas_id))
   {
      gl1_hw_sleep_clear_missed_frames(gas_id);
   }

return run_isr;
}

/*===========================================================================

FUNCTION gl1_drx_post_tick_dpc

DESCRIPTION
  A DPC (direct-procedure-call) function used when gl1_drx_post_tick_dpc
  is called from ISR context during the mdsp asynch interrupt.
  Will allow any sleep command processing isr to execute when trying
  to send the firmware to sleep.

===========================================================================*/
#define POST_TICK_RET_VAL
void gl1_drx_post_tick_dpc(gas_id_t gas_id )
{
  if ( gl1_asynch_int_dbg( gas_id ) )
  {
   /*
    * Debug to show when the DPC was queued in the asynch isr and then when
    * it is actually executed.
    */
    MSG_GERAN_HIGH_1_G(GL1_DRX_HDR" DPC execute gl1_drx_post_tick %dqs",
             gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)) );
  }

  /*
   * Called from the asynch int to allow the mdsp sleep isr command to fire
   * with the non-isr flag set TRUE.
   * This avoids the problem of the sleep command isr being generated from
   * the mdsp asynch interrupt isr, which can be deferred until the asynch
   * fails to sleep and is only then received and processed.
   * By moving the processing to the DPC we avoid any holdoff in this case.
   */
  gl1_drx_task_sleep( TRUE, gas_id );

  return POST_TICK_RET_VAL;
/*lint -esym(715,param3)*/
}

/*===========================================================================

FUNCTION gl1_drx_post_tick

DESCRIPTION
  Similar to gl1_drx_tick() but intended to be called at the end of the
  frame tick ISR rather than at the beginning.

  Uses the same tests that would be use by gl1_hw_tick() on the next frame
  to determine whether L1 can go to sleep now.

  The in_task_context argument should be set to TRUE if this function was
  not called from the context of the frame tick ISR.  This value is passed
  into the drivers which use it to avoid starting the go to sleep sequence
  to near the frame boundary.

  Unlike gl1_drx_tick(), this function does not remove required frame
  numbers from the required frame number list and it does not reset
  next_tick_required.

DEPENDENCIES
  The decision on whether or not to sleep is based on information supplied
  by gl1_require_next_tick, gl1_require_tick and gl1_task_(in)active.

RETURN VALUE
  None

SIDE EFFECTS
  May stop frame tick interupts and start sleep controller time line.
  Manipulates frame number counter to account for missed frame ticks.

===========================================================================*/
void gl1_drx_post_tick( boolean in_task_context, gas_id_t gas_id )
{
   uint32  gap     = 0, i, this_FN;
   boolean asynch_dbg = gl1_asynch_int_dbg( gas_id );
   uint32  required_frame = EMPTY_FRAME_NO;

   GL1_ISR_LOCK(gas_id);

   this_FN = GSTMR_GET_FN_GERAN( gas_id);

   /* Can only do anything if in awake state. */
   if (gl1_DRX_state[gas_id] == DRX_AWAKE)
   {
     /* Disable sleep if QXDM dip switch is set. */
     if (!gl1_sleep_active(gas_id))
     {
        next_tick_required[gas_id] = TRUE;
     }

     if (TRX_state[gas_id] != DRX_TX_OFF)
     {
        next_tick_required[gas_id] = TRUE;
     }

     if ( next_tick_required[gas_id] || active_task_count[gas_id] )
     {
        /* Someone wants to do something in the next frame */
        gap = 0;
     }
     else if (no_req_ticks[gas_id] == 0)
     {
        /* We don't need to be awake but we don't know when
         * to wake up.  We can't go to sleep.
         */
        gap = 0;

        /* Avoid this debug if asynch processing ongoing */
        if ( !gstmr_get_asynch_int_executing( gas_id ) &&
             !gl1_drx_get_asynch_int_ignore_gstmr( gas_id ) )
        {
          MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" no wakeup time FN=%d", this_FN);
        }
     }
     else
     {
        /* No one wants the next frame.  Work out gap to next wanted frame.
         * The gap is defined as illustrated below.
         *
         *    |     |     |     |     |     |
         *    ^                 ^
         *    |                 next req. tick
         *    this tick
         *
         *    gap = 3
         *
         *
         */
        required_frame = req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ];
        if ( this_FN == required_frame )
        {
          gap = 0;
        }
        else if ( this_FN < required_frame )
        {
          gap = required_frame - this_FN;
        }
        else
        {
          gap = required_frame + FRAMES_IN_HYPERFRAME - this_FN;
        }

        /*
         * As we can sleep on this frame need to increase gap to wakeup by 1
         * when executing the asynch int otherwise add 0
         */
        if ( gap )
        {
          gap += gl1_drx_get_pch_offset(gas_id);

          MSG_GERAN_HIGH_1_G(GL1_DRX_HDR" drx_post_tick active gap:%d", gap );
        }
     }

     /* Catch the case where we go to sleep for an INVALID no of frames */
     if (gap > GL1_DRX_MAX_GAP_TO_SLEEP)
     {
        for ( i=0; i < MAX_NO_REQ_TICKS; i++ )
        {
          MSG_GERAN_ERROR_2_G(GL1_DRX_HDR" Ref%d Reqd FN%d",req_frame_no_order[gas_id][i],req_frame_nos[gas_id][i]);
        }

        /* Remove the culprit required_FN and set gap to zero; so no sleep for now */
        gl1_drx_remove_req_frame_no( 0, gas_id );
        gap = 0;
     }

     /* Can we go to sleep? */
     if (gap > GL1_DRX_MIN_GAP_TO_SLEEP)
     {
       l1_drx_state_id saved_state = gl1_DRX_state[gas_id];

       gl1_drx_set_drx_state(DRX_GOING_TO_SLEEP, gas_id);

       /* We can go to sleep */
       if(gl1_drx_go_to_sleep(gap,in_task_context, required_frame, gas_id) == FALSE)
       {
         gl1_drx_set_drx_state(saved_state, gas_id);
       }
     }

   } /* if awake */

   /*
    * We are executing the asynch int so need special processing
    * which needs to be performed outside of AWAKE DRX state as FEE etc
    * could be active, this allows us to unwind the changes in FN etc
    * in the case when we do not sleep on this frame
    */
   if ( gstmr_get_asynch_int_executing(gas_id) )
   {
     /*
      * If we don't sleep in asynch int then need to unwind FN changes
      * for next real tick
      */
     if ( gl1_DRX_state[gas_id] != DRX_ASLEEP )
     {
       if ( asynch_dbg )
       {
         MSG_GERAN_HIGH_1_G(GL1_DRX_HDR" drx_post_tick active async %dqs",
                             gstmr_rd_qsymbol_count_geran( geran_map_gas_id_to_nas_id( gas_id ) ) );
       }

       /* Set this so that next gstmr is ignored as all ISR processing already performed */
       gl1_drx_set_asynch_int_ignore_gstmr( TRUE, gas_id );

       if ( asynch_dbg )
       {
         MSG_GERAN_HIGH_3_G(GL1_DRX_HDR" No sleep during asynch sleep update GSTMR hw/sw current:%d new:%d task_cnt:%d",
                 this_FN, GSTMR_GET_FN_GERAN( gas_id), active_task_count[gas_id] );
       }
     }
     else
     {
       if ( asynch_dbg )
       {
         /* Gstmr off as slept so no timestamping */
         MSG_GERAN_HIGH_1_G(GL1_DRX_HDR" drx_post_tick active async silent_frame:%d",
                             gl1_drx_get_asynch_int_ignore_gstmr( gas_id ) );
       }

       if ( gl1_drx_get_asynch_int_ignore_gstmr( gas_id ) )
       {
         /*
          * Clear this so that next gstmr is never ignored as we have
          * slept and GSTMR is off here
          */
         gl1_drx_set_asynch_int_ignore_gstmr( FALSE, gas_id );
       }
     }

     /* Reset asynch PCH offset to ensure next cycle is treated cleanly */
     gl1_drx_set_pch_offset( 0, gas_id );
   }

   /*
    * Able to sleep on frame so clear for next wakeup
    * otherwise leave set so we could get decode on next frame
    * safety to avoid trigger left active.
    */
   if ( ( ( gl1_DRX_state[gas_id] == DRX_GOING_TO_SLEEP )
          || ( gl1_drx_get_asynch_int_ignore_gstmr( gas_id ) &&
               !gl1_msg_is_asynch_sleep_enabled( gas_id )
             )
        )
        && gl1_hw_get_mdsp_asynch_int( gas_id ) )
   {
     gl1_hw_set_mdsp_asynch_int( FALSE, gas_id );
   }

  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gl1_drx_task_sleep

DESCRIPTION
  Once we have completed any task based activity such as calling the
  reserve_at in DSDS mode then if we have enough spare time in the frame
  then attempt to start the sleep timeline. This can save upto a frame in
  DSDS mode.

DEPENDENCIES
  The decision on whether or not to sleep is based on information supplied
  by gl1_require_next_tick, gl1_require_tick and gl1_task_(in)active.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

/*
 * How far into the frame in qs we allow the task sleep to occur any later then
 * sleep may fail and an early frame check to ensure we have not had a frame
 * tick in between request to reserve_at/release and sleep start
 */

#define GL1_DRX_POST_TICK_LATEST_QSYM_CNT 4450

#define GL1_DRX_POST_TICK_LATEST_ASYNCH_INT_QSYM_CNT \
          ( GL1_DRX_POST_TICK_LATEST_QSYM_CNT + 50 )
#define GL1_DRX_POST_TICK_EARLIEST_QSYM_CNT          150

void gl1_drx_task_sleep( boolean called_from_asynch_dpc, gas_id_t gas_id )
{
  uint32  qsym_cnt   = 0;
  boolean gstmr_int  = FALSE;
  boolean asynch_dbg = FALSE;
  boolean asynch_int_active_task_count_update = FALSE;

  if ( gl1_DRX_state[gas_id] != DRX_AWAKE )
  {
    MSG_GERAN_HIGH_0_G( GL1_DRX_HDR" DRX is not awake" );
    return;
  }

  qsym_cnt   = gstmr_rd_qsymbol_count_geran( geran_map_gas_id_to_nas_id( gas_id ) );
  gstmr_int  = gstmr_is_int_pending( gas_id );
  asynch_dbg = gl1_asynch_int_dbg( gas_id );

  if ( gstmr_int || ( qsym_cnt >= QS_PER_FRAME ) 
        || (get_vstmr_FN_modMaxFN(geran_map_gas_id_to_nas_id(gas_id)) != vstmr_rd_modMaxFN_frame_num(gas_id))
     )
  {
    if ( asynch_dbg )
    {
      MSG_GERAN_HIGH_3_G( GL1_DRX_HDR" Asynch post tick task current:%dqs called_from_asynch_dpc:%d pending GSTMR:%d",
                          qsym_cnt, called_from_asynch_dpc, gstmr_int );
    }

    return;
  }

  if ( !called_from_asynch_dpc &&
      ( active_task_count[gas_id] == 1 ) )
  {
     gl1_drx_task_inactive( gas_id );
     MSG_GERAN_MED_2_G(GL1_DRX_HDR" next_tick_required %d active_task_count %d",
                       next_tick_required[gas_id], active_task_count[gas_id] );

     asynch_int_active_task_count_update = TRUE;
  }

  if ( gstmr_get_asynch_int_executing( gas_id ) )
  {
    if ( asynch_dbg )
    {
      MSG_GERAN_HIGH_3_G(GL1_DRX_HDR" # Asynch Post tick task current:%dqs called_from_asynch_dpc:%d threshold:%dqs",
                          qsym_cnt, called_from_asynch_dpc, GL1_DRX_POST_TICK_LATEST_ASYNCH_INT_QSYM_CNT );
    }

    /* If MSIM active then TRM reserve etc will initiate possible task sleep */
    if ( ( qsym_cnt < GL1_DRX_POST_TICK_LATEST_ASYNCH_INT_QSYM_CNT ) &&
         ( qsym_cnt > GL1_DRX_POST_TICK_EARLIEST_QSYM_CNT ) )
    {
      gl1_drx_post_tick( TRUE, gas_id );
    }
    else
    {
      if ( gl1_asynch_int_dbg( gas_id ) )
      {
        MSG_GERAN_HIGH_2_G( "No sleep during asynch sleep update GSTMR hw/sw current:%d task_cnt:%d",
                           GSTMR_GET_FN_GERAN( gas_id ), active_task_count[gas_id] );
      }
    }
  }
  else
  {
    if ( asynch_dbg )
    {
      MSG_GERAN_HIGH_3_G(GL1_DRX_HDR" Post tick task current:%dqs called_from_asynch_dpc:%d threshold:%dqs",
                          qsym_cnt, called_from_asynch_dpc, GL1_DRX_POST_TICK_LATEST_QSYM_CNT );
    }

    /*
     * Check how far into the frame the task completes and if not too late
     * attempt to start the goto sleep activity early.
     */
    if (
         ( ( qsym_cnt < GL1_DRX_POST_TICK_LATEST_QSYM_CNT ) &&
           ( qsym_cnt > GL1_DRX_POST_TICK_EARLIEST_QSYM_CNT ) )
       )
    {
      gl1_drx_post_tick( TRUE, gas_id );
    }
  }

  if ( gl1_DRX_state[gas_id] != DRX_ASLEEP )
  {
    /* If we performed a pre-decrement on task_cnt then restore here */
    if ( asynch_int_active_task_count_update )
    {
       gl1_drx_task_active( gas_id );
       MSG_GERAN_MED_2_G(GL1_DRX_HDR" next_tick_required %d active_task_count %d",
                         next_tick_required[gas_id], active_task_count[gas_id] );
    }
  }

}

/*===========================================================================

FUNCTION gl1_drx_asynch_post_tick

DESCRIPTION
  Once we have completed any asynch interrupt processing of the frame tick
  the if we are not going to sleep then still set the silent frame
  flag to allow for the next dummy frame to execute correctly.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_drx_asynch_post_tick( gas_id_t gas_id )
{
   /*
    * We are executing the asynch int so need special processing
    * which needs to be performed outside of AWAKE DRX state as FEE etc
    * could be active, this allows us to unwind the changes in FN etc
    * in the case when we do not sleep on this frame
   */
   if ( gstmr_get_asynch_int_executing( gas_id ) )
   {
     /* Set this so that next gstmr is ignored as all ISR processing already performed */
     gl1_drx_set_asynch_int_ignore_gstmr( TRUE, gas_id );

     if ( gl1_asynch_int_dbg( gas_id ) )
     {
       MSG_GERAN_HIGH_2_G(GL1_DRX_HDR" No sleep during asynch sleep update GSTMR hw/sw current:%d task_cnt:%d",
                           GSTMR_GET_FN_GERAN( gas_id ), active_task_count[gas_id] );
     }
   }
}


/*===========================================================================

FUNCTION gl1_drx_task_active, gl1_task_inactive

DESCRIPTION
  Used to tell the DRX manager that a task has become active or has finished
  being active.  While at least one task is active the DRX manager will not
  allow sleep.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  May start the sleep timeline.

===========================================================================*/
void gl1_drx_task_active( gas_id_t gas_id )
{
   boolean overflow = FALSE;

  /* Mutex lock while setting active_task_count, could be updated by 2 threads */
  gstmr_log_mtx_lock(gas_id);

   if (active_task_count[gas_id] < 255) 
   {
      active_task_count[gas_id]++;
   }
   else
   {
      overflow = TRUE;
   }

#if defined(DEBUG_DRX_STATES)
   #error code not present
#endif

  gstmr_log_mtx_unlock(gas_id);

   if (overflow) {
      MSG_GERAN_ERROR_0_G(GL1_DRX_HDR" active_task_count overflow");
   }
}

void gl1_drx_task_inactive( gas_id_t gas_id )
{
   boolean underflow = FALSE;

  /* Mutex lock while setting active_task_count, could be updated by 2 threads */
  gstmr_log_mtx_lock(gas_id);

   if (active_task_count[gas_id] > 0)
   {
      active_task_count[gas_id]--;
   }
   else
   {
       underflow = TRUE;
   }

#if defined(DEBUG_DRX_STATES)
   #error code not present
#endif

  gstmr_log_mtx_unlock(gas_id);

   if (underflow) {
      MSG_GERAN_ERROR_0_G(GL1_DRX_HDR" active_task_count underflow");
   }
}

/*===========================================================================

FUNCTION gl1_drx_require_next_tick

DESCRIPTION
  Used to tell the DRX manager that a process requires the next frame tick.
  This will hold off sleep for one more tick.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_drx_require_next_tick( gas_id_t gas_id )
{
   boolean rude_wakeup = FALSE;
   boolean wakeup_while_going_to_sleep = FALSE;

   /* Use INTLOCK to make sure states don't get
    * changed on us part way through.  Don't
    * put debug messages inside the INTLOCK
    * - use flags to remember what messages
    * to put out after INTFREE.
    */
   GL1_ISR_SAVE_LOCK(gas_id);
   switch (gl1_DRX_state[gas_id])
   {
      case DRX_AWAKE:
      case DRX_WARMING_UP:
         next_tick_required[gas_id] = TRUE;
         break;

      case DRX_ASLEEP:
         /* need to do rude wakeup */

         /* tell sleep controller */
         remaining_frames[gas_id] = gl1_hw_sleep_gsm_force_wakeup( gas_id );
         rude_wakeup      = TRUE;

         if( 0 == remaining_frames[gas_id] )
         {
           /* We are about to wake up anyways so no need to be rude ! */
           GL1_ISR_SAVE_UNLOCK(gas_id);

           return;
         }

         gl1_drx_set_drx_state ( DRX_RUDE_WAKEUP, gas_id );
         break;

      case DRX_GOING_TO_SLEEP:
         /* Need to let go to sleep complete and then do rude wakeup. */
         gl1_drx_set_drx_state ( DRX_WAKEUP_WHILE_GOING_TO_SLEEP, gas_id );
         wakeup_while_going_to_sleep = TRUE;
         break;

      default:
         break;
   }

   GL1_ISR_SAVE_UNLOCK(gas_id);

   if (rude_wakeup) {
      MSG_GERAN_MED_1_G(GL1_DRX_HDR" L1 waking up %d frames early",remaining_frames[gas_id]);
   }
   else if (wakeup_while_going_to_sleep) {
      MSG_GERAN_MED_0_G(GL1_DRX_HDR" early wake up while going to sleep");
   }

}
/*===========================================================================

FUNCTION gl1_drx_require_and_wait_for_next_tick

DESCRIPTION
  Used to tell the DRX manager that a process requires the next frame tick.
  This will hold off sleep for one more tick. Then waits for the frame tick
  to occur to ensure that FW is awake before continuing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_drx_require_and_wait_for_next_tick( gas_id_t gas_id )
{
   avoid_gps_update[gas_id] = TRUE;

   /* wake up layer 1 isr */
   gl1_drx_require_next_tick(gas_id);

   /* Wait for next frame tick */
   (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
   (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );

   avoid_gps_update[gas_id] = FALSE;
}

/*===========================================================================

FUNCTION gl1_drx_require_and_wait_for_next_tick_conditional

DESCRIPTION
  Similar to gl1_drx_require_and_wait_for_next_tick() with GL1_ISR_LOCK
  But if already awake, does not wait for the frame tick, saves delays.
  Used primarily for ensuring GL1 is awake to process new messages.
  
  Used to tell the DRX manager that a process requires the next frame tick.
  This will hold off sleep for one more tick. Then waits for the frame tick
  to occur to ensure that FW is awake before continuing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_drx_require_and_wait_for_next_tick_conditional( gas_id_t gas_id )
{

   GL1_ISR_LOCK(gas_id);

   gl1_drx_require_next_tick(gas_id);  

   if(gl1_DRX_state[gas_id] == DRX_AWAKE)
   {
     /* awake so no need to wait */ 
     GL1_ISR_UNLOCK(gas_id);
   }
   else
   {
     avoid_gps_update[gas_id] = TRUE;
 
     GL1_ISR_UNLOCK(gas_id);
     /* Wait for next frame tick */
     (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
     (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
  
     avoid_gps_update[gas_id] = FALSE;
   }
}

/*===========================================================================

FUNCTION  l1_drx_log

DESCRIPTION
  Create a log of the drx require and release by saving these to a buffer
  each time they called.  Used for debugging.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_drx_log( uint8 drx_req_rel, uint32 frame_number, uint8 ref, l1_drx_ref_error_T error, gas_id_t gas_id )
{
   l1_NVLog_struct_T *l1_nv_log_ptr = &l1_nv_log[gas_id];
   l1_drx_ref_log_T *l1_drx_ref_log_ptr = &l1_nv_log_ptr->l1_drx_ref_log_buf[l1_nv_log_ptr->l1_drx_ref_log_buf_index];

   l1_drx_ref_log_ptr->CurrentFN = GSTMR_GET_FN_GERAN(gas_id);
   l1_drx_ref_log_ptr->DrxFN = frame_number;
   l1_drx_ref_log_ptr->drx_req_rel = drx_req_rel;
   l1_drx_ref_log_ptr->no_req_ticks = no_req_ticks[gas_id];
   l1_drx_ref_log_ptr->ref_count = ref;
   l1_drx_ref_log_ptr->drx_state = gl1_DRX_state[gas_id];
   l1_drx_ref_log_ptr->l1_drx_ref_error = error;
   l1_nv_log_ptr->l1_drx_ref_log_buf_index++;

   if (l1_nv_log_ptr->l1_drx_ref_log_buf_index >= L1_DRX_LOG_LEN)
   {
      l1_nv_log_ptr->l1_drx_ref_log_buf_index = 0;
   }
}

/*===========================================================================

FUNCTION gl1_drx_require_tick

DESCRIPTION
  Used to tell the DRX manager that a process requires a frame tick at some
  time in the future.  The DRX manager will ensure that the MS is awake and
  that the frame tick ISR processes the requested tick.

DEPENDENCIES
  None

RETURN VALUE
  Reference that should be passed to gl1_drx_release_tick() if the tick is
  no longer required.

SIDE EFFECTS
  None

===========================================================================*/
uint8 gl1_drx_require_tick( uint32 frame_number, gas_id_t gas_id )
{
  uint8    ref, i;
  uint32   next_fn;

  if (gl1_DRX_state[gas_id] != DRX_NULL) 
  {

    if((gl1_DRX_state[gas_id]!=DRX_AWAKE) &&
       (gl1_DRX_state[gas_id]!=DRX_RUDE_WAKEUP) &&
       !gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] )
      {
        MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" unexpected state %d",gl1_DRX_state[gas_id]);
      }

    /* We can only store so many required ticks, if we
     * already have that many then ignore this one. */
    if (no_req_ticks[gas_id]>=MAX_NO_REQ_TICKS)  
    {
      MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" too many required ticks (%d)",no_req_ticks[gas_id]);
      l1_drx_log(GL1_DRX_REQ_TICK, frame_number, GL1_DRX_INVALID_REF, GL1_DRX_ERROR_TOO_MANY_REQ_TICKS, gas_id);
      return 0;
    }

    /* We don't want to put the current FN in the list.
     * This makes the DRX manager sleep for 3 hours! */
    if (frame_number == GSTMR_GET_FN_GERAN( gas_id)) 
    {
      MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" required current FN (%d)",frame_number);

      next_tick_required[gas_id] = TRUE;
      l1_drx_log(GL1_DRX_REQ_TICK, frame_number, GL1_DRX_INVALID_REF, GL1_DRX_ERROR_REQ_CURR_FN, gas_id);
      return 0;
    }

    if (no_req_ticks[gas_id]==0)  
    {
      /* There are no other entires so adding this  */
      /* one is easy.                               */
      ref                            = 1;
      req_frame_nos[gas_id][0]       = frame_number;
      req_frame_no_order[gas_id] [0] = ref;
      no_req_ticks[gas_id]           = 1;
    }
    else
    {
      /* Search through the frame no array for an   */
      /* empty entry and add the new frame no.      */
      /* We know there is at least one entry and at */
      /* least one free space.                      */
      for (ref=0; (ref < MAX_NO_REQ_TICKS) && (req_frame_nos[gas_id][ref]!=EMPTY_FRAME_NO); ref++)
           ;

      if (!(ref<MAX_NO_REQ_TICKS)) 
      {
         MSG_GERAN_ERROR_0_G(GL1_DRX_HDR" can't require tick.");
         l1_drx_log(GL1_DRX_REQ_TICK, frame_number, ref, GL1_DRX_ERROR_CAN_NOT_REQ, gas_id);
         return 0;
      }

      req_frame_nos[gas_id][ref] = frame_number;
      ref++;   /* reference is index + 1 */

      /* Insert a reference in the appropriate place   */
      /* in the sorted index array.                    */
      /* Work up the list moving down each reference   */
      /* to a frame no. later than the new one.        */
      for ( i=no_req_ticks[gas_id]; i>0; i-- )  
      {
        next_fn = req_frame_nos[gas_id][ req_frame_no_order[gas_id][i-1]-1 ];
        if ( gl1_drx_is_frame_num_later( next_fn, frame_number ) ) 
        {
          req_frame_no_order[gas_id][i] = req_frame_no_order[gas_id][i-1];
        }
        else
        {
          break;
        }
      }
      req_frame_no_order[gas_id][i] = ref;
      no_req_ticks[gas_id]++;
    }

    l1_drx_log(GL1_DRX_REQ_TICK, frame_number, ref, GL1_DRX_NO_ERROR, gas_id);
    return ref;
  }
  else 
  {
    /* DRX_State == DRX_NULL */
    l1_drx_log(GL1_DRX_REQ_TICK, frame_number, GL1_DRX_INVALID_REF, GL1_DRX_ERROR_INVALID_STATE, gas_id);
    return 0;
  }
}

/*===========================================================================

FUNCTION gl1_drx_release_tick

DESCRIPTION
  Used to cancel a previous request made via gl1_drx_require_tick.  The
  request to cancel is indicated by the reference.

DEPENDENCIES
  The reference must correspond to a previously requested tick.  The frame
  number of the requested tick must not have occured yet (i.e. you can't
  cancel a tick that has already happened).

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_drx_release_tick( uint8 reference, gas_id_t gas_id )
{
   uint8 i;

   if (gl1_DRX_state[gas_id] != DRX_NULL)
   {

      if ( gl1_DRX_state[gas_id] != DRX_AWAKE )
      {
         MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" unexpected state %d.",gl1_DRX_state[gas_id]);
      }

      /* Check for invalid references */
      if ( (reference==0) || (reference>MAX_NO_REQ_TICKS) )
      {
         MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" bad reference %d.",reference);
         l1_drx_log(GL1_DRX_REL_TICK, GL1_DRX_INVALID_FRAME, reference, GL1_DRX_ERROR_BAD_REF, gas_id);
         return;
      }

      /* check for no_req_ticks so we don't read or write pass end of buffer */
      if(no_req_ticks[gas_id] > MAX_NO_REQ_TICKS)
      {
        no_req_ticks[gas_id] = (MAX_NO_REQ_TICKS -1);
      }

      /* Find the reference in the ordered array */
      for (i=0; (i<no_req_ticks[gas_id]) && (req_frame_no_order[gas_id][i]!=reference); i++) {};

      /* Only remove if i looks valid */
      if ( i < no_req_ticks[gas_id] )
      {
        /* Remove */
        l1_drx_log(GL1_DRX_REL_TICK, req_frame_nos[gas_id][ req_frame_no_order[gas_id][i]-1 ], i, GL1_DRX_NO_ERROR, gas_id);
        gl1_drx_remove_req_frame_no( i, gas_id );
      }
      else if (no_req_ticks[gas_id] != 0)
      {
        MSG_GERAN_ERROR_2_G(GL1_DRX_HDR" can't release tick index %d num req ticks %d",i,no_req_ticks[gas_id]);
        l1_drx_log(GL1_DRX_REL_TICK, GL1_DRX_INVALID_FRAME, i, GL1_DRX_ERROR_CAN_NOT_REL, gas_id);
      }
   }
}

/*===========================================================================

FUNCTION gl1_drx_release_fn_tick

DESCRIPTION
  Used to cancel a previous request made via gl1_drx_require_tick.  The
  request to cancel is indicated by the frame.

DEPENDENCIES
  The reference frame number must correspond to a previously requested tick.  The frame
  number of the requested tick must not have occured yet (i.e. you can't
  cancel a tick that has already happened).

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_drx_release_fn_tick( uint32 frame_number, gas_id_t gas_id )
{
  uint8 ref;

  if( ( frame_number == EMPTY_FRAME_NO ) || ( frame_number == GL1_DEFS_INVALID_FN ) )
  {
    return;
  }

  /* find the reference which has this frame number and then release */
  for (ref=0; ref < MAX_NO_REQ_TICKS; ref++)
  {
    if( req_frame_nos[gas_id][ref] == frame_number )
    {
      gl1_drx_release_tick( ref+1, gas_id );
      MSG_GERAN_HIGH_1_G(GL1_DRX_HDR" Deleted reference: drx_ref %d ", ref);
      break;
    }
  }
}

/*===========================================================================

FUNCTION gl1_drx_require_TX

DESCRIPTION
  Tells the DRX manager that the PA needs to be on.  num_frames specifies
  how many frames the PA needs to be on for.  After num_frames have elapsed
  the DRX manager will turn the PA off.  If num_frames is 0 the PA stays on
  indefinitely.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Will stop L1 from sleeping while PA is on.

===========================================================================*/
/* Number of frames per radio block */
#define GL1_DRX_MIN_REQ_TX_DURATION  (4)

void gl1_drx_require_TX( uint32 num_frames, gas_id_t gas_id  )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   GL1_ISR_LOCK(gas_id);

   if (num_frames == 0)
   {
      /* turn on for always */
      if ( (TRX_state[gas_id] == DRX_TX_OFF) ||
           (TRX_state[gas_id] == DRX_TX_TURNING_OFF) )
      {
         gl1_drx_set_trx_state ( DRX_TX_TURNING_ON_ALWAYS, gas_id );
      }
      else if (TRX_state[gas_id] == DRX_TX_ON_TIMED)
      {
         gl1_drx_set_trx_state ( DRX_TX_ON_ALWAYS, gas_id );
      }
   }
   else
   {
      /* turn on for a limited time */
      if ( (TRX_state[gas_id] == DRX_TX_OFF) ||
           (TRX_state[gas_id] == DRX_TX_TURNING_ON_ALWAYS) )
      {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         /* In dual sim mode don't allow TX on unless firmware is loaded */
         if ( gl1_msg_get_multi_sim_mode() && ( l1_tskisr_blk->firmware_loaded == FALSE ) )
         {
           MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" DRX_TX_TURNING_ON_ALWAYS: Cannot call gl1_drx_turn_tx_on in dual sim");
         }
         else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
         if( ( num_frames < GL1_DRX_MIN_REQ_TX_DURATION ) && (TRX_state[gas_id] == DRX_TX_OFF) )
         {
           MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" Invalid timed TX ON %d when TX is OFF ", num_frames );

           GL1_ISR_UNLOCK(gas_id);

           return;
         }
         else
         {
           gl1_drx_turn_tx_on(gas_id);
         }

         MSG_GERAN_ERROR_0_G(GL1_DRX_HDR" Setting timed TX ON when TX is OFF.");

      }

         tx_down_count[gas_id] = num_frames-1;
         gl1_drx_set_trx_state ( DRX_TX_ON_TIMED, gas_id );
   }
  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gl1_drx_first_awake_frame

DESCRIPTION
  Returns whether or not this is the first frame after waking up.  This
  may be useful if certain activity needs to be avoided during this frame.
  Should only be called from the ISR.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if this is the first frame after waking up.
  will be zero, and go up from there.

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_drx_first_awake_frame ( gas_id_t gas_id )
{
   return ( ( gl1_DRX_state[gas_id] != DRX_NULL ) && ( frames_awake[gas_id] == 0 ) );
}

/*===========================================================================

FUNCTION gl1_drx_set/get_pch_offset

DESCRIPTION
  Either set or return the active asynch interrupt frame offset that
  is added onto the sleep frames gap calculated and also subtracted
  from the wakeup frame number so it is correct.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_drx_set_pch_offset( int16 asynch_pch_offset, gas_id_t gas_id )
{
  if ( gl1_asynch_int_dbg( gas_id ) )
  {
    /*MSG_GERAN_MED_2_G(GL1_DRX_HDR" Set pch_offset before:%d after:%d",
                       gl1_asynch_pch_offset[gas_id], asynch_pch_offset );*/
  }

  gl1_asynch_pch_offset[gas_id] = asynch_pch_offset;
}

int16 gl1_drx_get_pch_offset( gas_id_t gas_id )
{
  if ( gl1_asynch_int_dbg( gas_id ) && !gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] )
  {
    MSG_GERAN_HIGH_2_G(GL1_DRX_HDR" Get pch_offset:%d FN:%d",
                       gl1_asynch_pch_offset[gas_id], GSTMR_GET_FN_GERAN( gas_id ) );
  }

  return ( gl1_asynch_pch_offset[gas_id] );
}

/*===========================================================================

FUNCTION gl1_drx_get/set_asynch_int_ignore_gstmr

DESCRIPTION
  Either set or return the active flag to determine if we have failed an
  asynch int in which case we can effectively ignore the next gstmr as
  the code that would have run then has already been executed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_drx_set_asynch_int_ignore_gstmr(
       boolean asynch_int_ignore_next_gstmr, gas_id_t gas_id )
{

  gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

  gl1_hw_sleep_cycle->async_msg.ignore_next_gstmr =  asynch_int_ignore_next_gstmr;
  gl1_asynch_int_gstmr_ignore[gas_id] = asynch_int_ignore_next_gstmr;
}

boolean gl1_drx_get_asynch_int_ignore_gstmr( gas_id_t gas_id )
{
  return ( gl1_asynch_int_gstmr_ignore[gas_id] );
}

/*===========================================================================

FUNCTION gl1_drx_set_frame_number_adjustment

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_drx_set_frame_number_adjustment ( int32 num, gas_id_t gas_id )
{
  gl1_drx_update_counters (num, TRUE, gas_id);
}

/*===========================================================================

FUNCTION gl1_drx_asynch_int_sleep_possible_precheck

DESCRIPTION
  Check whether we have an activity already booked into the DRX that will
  not allow the Asych PCH Int to be executed, in which case do not try
  and execute the asynch int as no current saving.

DEPENDENCIES
  None

RETURN VALUE
  TRUE is it may be possible to sleep, FALSE if a DRX event already booked.

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_drx_asynch_int_sleep_possible_precheck( gas_id_t gas_id )
{
  uint32  this_FN, gap = 0;
  boolean sleep_possible = FALSE;

  /* Always allow for asynch sleep only needed for OPT3 */
  if ( gl1_is_asynch_sleep_enabled( gas_id ) )
  {
    sleep_possible = TRUE;
  }
  else
  {
    GL1_ISR_LOCK(gas_id);

    this_FN = GSTMR_GET_FN_GERAN( gas_id);

    /* Can only do anything if in awake state. */
    if ( gl1_DRX_state[gas_id] == DRX_AWAKE )
    {
      /*
       * For this case we try and exec the asynch int as wakeup event
       * can be booked after PCH
       */
      if ( !no_req_ticks[gas_id] || !req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] )
      {
        /* We don't need to be awake but we don't know when
         * to wake up.  We can't go to sleep.
         */
        MSG_GERAN_LOW_2_G( GL1_DRX_HDR" no wakeup time FN:%d req_frame_nos:%d",
                 this_FN, req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ]);

        sleep_possible = TRUE;
      }
      /* No one wants the next frame.  Work out gap to next wanted frame.
       * The gap is defined as illustrated below.
       *
       *    |     |     |     |     |     |
       *    ^                 ^
       *    |                 next req. tick
       *    this tick
       *
       *    gap = 3
       *
       *
       */
      else if ( req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] == EMPTY_FRAME_NO )
      {
        MSG_GERAN_LOW_0_G(GL1_DRX_HDR" Empty DRX FN");

        sleep_possible = TRUE;
      }
      else if ( this_FN == req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] )
      {
        MSG_GERAN_LOW_0_G(GL1_DRX_HDR" attaching DRX FN");
      }
      else if ( this_FN < req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] )
      {
        gap = ( req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] - this_FN );

        MSG_GERAN_LOW_3_G(GL1_DRX_HDR" DRX Precheck gap:%d FN:%d req_frame_nos:%d",
                 gap, this_FN, req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] );
      }
      else
      {
        gap = ( ( req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ]
                  + (uint32)FRAMES_IN_HYPERFRAME ) - this_FN );

        MSG_GERAN_LOW_3_G(GL1_DRX_HDR" DRX Precheck gap:%d FN:%d req_frame_nos:%d",
                 gap, this_FN, req_frame_nos[gas_id][ req_frame_no_order[gas_id][0]-1 ] );
      }

      /* Check that we have a valid gap period available */
      if ( ( gap > GL1_DRX_MIN_GAP_TO_SLEEP ) && ( gap < GL1_DRX_MAX_GAP_TO_SLEEP )
           && !sleep_possible )
      {
        sleep_possible = TRUE;
      }
    }

    GL1_ISR_UNLOCK(gas_id);
  }

  return ( sleep_possible );
}

/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/

/*===========================================================================

FUNCTION gl1_drx_go_to_sleep

DESCRIPTION
  Starts the sleep time line for the air interface hardware.

  The arguments num_frames and check_too_early are passed into
  the drivers:
  -  num_frames is the number of frames elapsed since we last
     woke up
  -  check_to_early should be set to TRUE if this function was
     not called from the frame tick ISR context.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean gl1_drx_go_to_sleep(uint32   num_frames, 
                                   boolean  check_too_early, 
                                   uint32   wakeup_fn,
                                   gas_id_t gas_id )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif

  /* Irrespective of the decision to go to sleep, 
   * if we have come this far the missed frames should be cleared */
  if(gl1_hw_sleep_get_missed_frames(gas_id))
  {
    gl1_hw_sleep_clear_missed_frames(gas_id);
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( grm_get_trm_release_required(l1_tskisr_blk->client_id, gas_id) )
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" Do not sleep trm release pending");
    return FALSE;
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  if( mdsp_commands_pending(gas_id) )
  {
    MSG_GERAN_ERROR_0_G(GL1_DRX_HDR" mdsp commands pending ");
    return  FALSE;
  }

  if (g1l_hw_is_mcpm_idle(gas_id) == FALSE)
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" MCPM not idle");
    return  FALSE;
  }

  if(g2w_is_wfw_active(gas_id)== TRUE)
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" WFW active");
    return  FALSE;
  }

  if(gl1_drx_sleep_inhibit[gas_id])
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" X2G reselect in progress");
    return  FALSE;
  }

  if(gl1_drx_sleep_cco_inhibit[gas_id])
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" CCO reselect in progress");
    return  FALSE;
  }

  if(l1_get_background_wcdma_plmn_scan_active(gas_id))
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" G2X BPLMN active");
    return  FALSE;
  }

  if (gl1_drx_get_sleep_inhibit_for_trm())
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" Don't sleep due to TRM activity in progress");
    return  FALSE;
  }

  /* This could be improved, resource lock is currently only used for CSFB */
  /* but this should probably handle any occasion where we retain a TRM grant */
  /* through sleep or the rf/fw failure on wake-up could be fixed... CJL 15/6/15 */
  if (GRM_RESOURCE_LOCK_INACTIVE != grm_get_resource_lock_enum(gas_id))
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" Don't sleep due to CSFB in progress");
    return  FALSE;
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_QTA
  if(gl1_hw_qta_gap_active(gas_id))
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" Don't sleep, pending QTA cleanup");
    return  FALSE;
  }
#endif /* FEATURE_QTA */

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  if(vstmr_is_frame_extension_in_progress(gas_id))
  {
    return  FALSE;
  }

  if(gl1_hw_sleep_get_rf_cnf_pending(gas_id))
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" Don't sleep, Waiting for RF CNF");
    return  FALSE;
  }

   /* Subtract one from number of frames since
    * sleep timeline doesn't start until the
    * next frame tick.
    */
   num_frames--;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* Wake up one frame earlier if qta is active */
  if (gl1_hw_sleep_get_qta_active(gas_id))
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" Sleep from QTA");
    num_frames--;

    if (num_frames < 1)
    {
      MSG_GERAN_HIGH_0_G("Don't sleep, gap too small for sleep during QTA");
      return FALSE;
    }
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#if ( defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM ) ) \
    && defined(FEATURE_WCDMA)
  if (gl1_drx_is_cm_mode_enabled(gas_id))
  {
    MSG_GERAN_HIGH_0_G(GL1_DRX_HDR" WCDMA still using Compressed Mode, don't sleep");
    return(FALSE);
  }
#endif /* FEATURE_DUAL_SIM && FEATURE_WCDMA */

  if (gl1_hw_sleep_gsm_start_sleep( num_frames, frames_awake[gas_id], check_too_early, wakeup_fn, gas_id ))
  {
    uint32 fn = GSTMR_GET_FN_GERAN( gas_id);

    if( gl1_hw_sleep_disable[gas_id] )
    {
      /* disable going to sleep / don't go to sleep */
      MSG_GERAN_HIGH_3_G(GL1_DRX_HDR" L1 on sleep clock (for %d frames) FN=%d %d (SLEEP_DIS_EFS)",num_frames,fn,timetick_get_safe());
    }
    else
    {
      MSG_GERAN_HIGH_3_G(GL1_DRX_HDR" L1 on sleep clock (for %d frames) FN=%d %d",num_frames,fn,timetick_get_safe());
    }

    /* Clear rat change flag to ensure opt3 is not disabled for next awake cycle*/
    l1_sc_set_rat_change_active ( FALSE, gas_id );

    if (!(gl1_hw_efs_get_debug(gas_id) & GL1_EFS_DEBUG_DISABLE_DRX_DEBUG_CRASH))
    {
      if (l1_tskisr_blk->firmware_loaded == TRUE)
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G(GL1_DRX_HDR"L1 on sleep without unload firmware");
      }
    }

    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

#if  (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) ) && defined (FEATURE_WCDMA)
static boolean gl1_drx_is_cm_mode_enabled( gas_id_t gas_id )
{
    boolean cm_mode = FALSE;

        cm_mode = (cm_drx_state[gas_id] == DRX_RUDE_WAKEUP);

    return (cm_mode);
}
#endif /* ( FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM ) & FEATURE_COMPRESSED_MODE */

/*===========================================================================

FUNCTION gl1_drx_is_gsm_asleep

DESCRIPTION
  Returns TRUE if DRX state is ASLEEP, FALSE otherwise

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_drx_is_gsm_asleep( gas_id_t gas_id )
{
  boolean is_drx_asleep = FALSE;

  if ( gl1_DRX_state[gas_id] == DRX_ASLEEP )
  {
    is_drx_asleep = TRUE;
  }

  return ( is_drx_asleep );
}

/*===========================================================================

FUNCTION gl1_drx_wake_up

DESCRIPTION
  Performs any actions required when the sleep time line of the air
  interface hardware ends.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_drx_wake_up( gas_id_t gas_id )
{
   uint32 fn = GSTMR_GET_FN_GERAN( gas_id);

   MSG_GERAN_MED_1_G(GL1_DRX_HDR" L1 on TCXO FN:%d",fn);

  frames_awake[gas_id] = 0;   /*lint -esym(529, fn) suppress 'fn' not subsequently referenced */
}

/*===========================================================================

FUNCTION gl1_drx_is_frame_num_later

DESCRIPTION
  Determines if one frame number is later than another taking into account
  wrapping of the frame number.


When a > b
  |--------------------------b----------a----------------------|
  |                  |                         |               |
  |                  |<---FN_WRAP_THRESHOLD--->|               |
 a is later than b

  |---------b------------------------------------------a-------|
  |                  |                         |               |
  |                  |<---FN_WRAP_THRESHOLD--->|               |
 a is not later than b


When b > a
  |---------------------a----------b---------------------------|
  |       |                                       |            |
  |       |<-FN_MAX_PLUS_ONE - FN_WRAP_THRESHOLD->|            |
 a is not later than b

  |-----a----------------------------------------------b-------|
  |       |                                       |            |
  |       |<-FN_MAX_PLUS_ONE - FN_WRAP_THRESHOLD->|            |
 a is later than b

  |                                                            |
  |                                                            |
  |                                                            |
  |<---------------------FN_MAX_PLUS_ONE---------------------->|

DEPENDENCIES
  None

RETURN VALUE
  TRUE       fn_a is later than fn_b
  FALSE      fn_a is not later than fn_b

SIDE EFFECTS
  None
===========================================================================*/
static boolean gl1_drx_is_frame_num_later( uint32 fn_a, uint32 fn_b)
{
  if (fn_a > fn_b)  {
    return (  (fn_a - fn_b) < FN_WRAP_THRESHOLD );
  }
  else {
    return ( (fn_b - fn_a) > (FN_MAX_PLUS_ONE - FN_WRAP_THRESHOLD) );
  }
}

static void gl1_drx_remove_req_frame_no( uint8 index, gas_id_t gas_id )
{
   uint8 i;

   /* Check that the index is sensible */
   if (index >= no_req_ticks[gas_id]) {
      MSG_GERAN_ERROR_1_G(GL1_DRX_HDR" bad index %d.",index);
      /* do nothing */
      return;
   }

   if ( no_req_ticks[gas_id] > 0 )
   {
      no_req_ticks[gas_id]--;

      /* Remove the frame no. from the frame no. array */
      req_frame_nos[gas_id][ req_frame_no_order[gas_id][index]-1 ] = EMPTY_FRAME_NO;

      /* Remove the reference from the ordered array    */
      for ( i=index; ((i<no_req_ticks[gas_id]) && (i<MAX_NO_REQ_TICKS-1)); i++)
      {
         req_frame_no_order[gas_id][i] = req_frame_no_order[gas_id][i+1];
      }
      req_frame_no_order[gas_id][no_req_ticks[gas_id]] = 0;
   }
}

/*===========================================================================

FUNCTION gl1_drx_update_counters

DESCRIPTION
  Used to update counters that will not be incremented for one or more
  frames because the drame tick ISR is not executing.

  bump - the number of frames to bump the counters by.  Absolute value
         should be less than FRAMES_IN_HYPERFRAME (51x26x1024).

  do_gstmr - controls whether the FN counter maintained by the
             GSTMR driver is updated.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_drx_update_counters( int32 bump, boolean do_gstmr, gas_id_t gas_id )
{
  if (do_gstmr) {
     /* Main L1 GSM frame number counter */
     if (bump >= 0) { /* ADJUST FORWARD */

        GSTMR_ADVANCE_FN_GERAN( (uint32)bump,gas_id  );
     }
     else { /* ADJUST BACKWARD */
        GSTMR_ADVANCE_FN_GERAN( (uint32)(FRAMES_IN_HYPERFRAME + bump), gas_id );
     }
  }

  /* Message layer frame number counter */
  gl1_msg_adjust_time_stamp(bump, gas_id);
}

/*===========================================================================

FUNCTION gl1_drx_start_int_callback

DESCRIPTION
  This function is called by the sleep controller drivers when they have
  transered maintenance of the physical layer timebase from the TCXO to
  the sleep clock.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_drx_start_int_callback( gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Check for immediate wake up. */
   if (gl1_DRX_state[gas_id] == DRX_WAKEUP_WHILE_GOING_TO_SLEEP)
   {
      /* Do a rude wake up. */
      gl1_drx_set_drx_state ( DRX_ASLEEP, gas_id );
      gl1_drx_require_next_tick(gas_id);

      /* If we are waiting on a sleep timetag, we can schedule one on the
         next frame tick now */
      if (sleep_timetag_pending[gas_id])
      {
        /* Tell CGPS we are leaving this cell */
        gl1_timetag_cgps(l1_tskisr_blk->l1_state, CGPS_TIMETAG_REQUEST,gas_id);

         sleep_timetag_pending[gas_id] = FALSE;
      }
   }
   else
   {
      gl1_drx_set_drx_state ( DRX_ASLEEP, gas_id );
      gl1_hw_sleep_start_from_drx ( gas_id );
      /* We are now on sclk and so can send a sleep timetag */
      if (sleep_timetag_pending[gas_id])
      {
         gl1_timetag_cgps_asleep(l1_tskisr_blk->l1_state, gas_id);

         sleep_timetag_pending[gas_id] = FALSE;
      }
   }

}

/*===========================================================================

FUNCTION gl1_drx_warmup_int_callback

DESCRIPTION
  This function is called by the sleep controller drivers when they
  receive the interrupt indicating that the TCXO should be turned
  on again in preparation for the end of a sleep period.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_drx_warmup_int_callback( gas_id_t gas_id )
{
   /* If we are asleep, change to warming up.  We may already be in
      RUDE_WAKEUP state, in which case no need to go to warming up */
   if (gl1_DRX_state[gas_id] == DRX_ASLEEP)
   {
      gl1_drx_set_drx_state ( DRX_WARMING_UP, gas_id );
   }
}

/*===========================================================================

FUNCTION gl1_drx_turn_tx_on(), gl1_drx_turn_tx_off()

DESCRIPTION
  Turn tx chain on or off respecting feature definitions and QXDM switches.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_drx_turn_tx_on( gas_id_t gas_id )
{
   /* Turn on the Tx chain */
   gl1_hw_turn_tx_on(gas_id);
}

static boolean gl1_drx_turn_tx_off( gas_id_t gas_id )
{
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_drx_turn_tx_off: Return Immediately");
   return TRUE;
#else
   if ( !gl1_hw_turn_tx_off( TRUE, gas_id ) )
   {
       gl1_drx_set_trx_state ( DRX_TX_TURNING_OFF, gas_id );
   }
   else
   {
      gl1_drx_set_trx_state ( DRX_TX_OFF, gas_id );
   }
   return TRUE;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
}

/*===========================================================================

FUNCTION gl1_drx_process_timetag_request

DESCRIPTION
  Called when we receive a request for a timetag from GPS

===========================================================================*/
void gl1_drx_process_timetag_request(IMH_T *msg_header)
{
   gas_id_t gas_id;
   uint8    l1_state;
   volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
   l1_drx_state_id drx_state;

   gl1_cgps_timetag_type *msg = (gl1_cgps_timetag_type *)msg_header;  /*lint !e740 unusual pointer cast */

   gas_id = check_gas_id (msg->gas_id);

   l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   GL1_ISR_SAVE_LOCK(gas_id);

   l1_state = l1_tskisr_blk->l1_state;

   drx_state = gl1_DRX_state[gas_id];

   MSG_GERAN_MED_4_G(GL1_DRX_HDR" gl1_drx_process_timetag_request l1_state %d, gl1_DRX_state %d, allow_sleep %d, sleep_state %d",
                      l1_state, drx_state, msg->allow_sleep_timetag, gl1_hw_get_sleep_state(gas_id) );

   /* Take action based on the DRX state */
   switch (drx_state)
   {
      /* If we are going to sleep, wait until we are really asleep then
         use the sleep clock */
      case DRX_GOING_TO_SLEEP:
         if (msg->allow_sleep_timetag)
         {
            sleep_timetag_pending[gas_id] = TRUE;
         }
         else
         {
            /* Tell CGPS we are leaving this cell */
            gl1_timetag_cgps( l1_state, CGPS_TIMETAG_REQUEST, gas_id );
         }
         break;

      /* If supported, send a timetag based on the sleep clock */
      case DRX_ASLEEP:
         if (msg->allow_sleep_timetag)
         {
            gl1_timetag_cgps_asleep( l1_state, gas_id );
         }
         else
         {
            /* Tell CGPS we are leaving this cell */
            gl1_timetag_cgps( l1_state, CGPS_TIMETAG_REQUEST, gas_id );
         }
         break;

      /* If we are awake then schedule a timetag to be sent on the next
         frame tick */
      case DRX_NULL:
      case DRX_AWAKE:
      case DRX_RUDE_WAKEUP:
      case DRX_WARMING_UP:
      case DRX_WAKEUP_WHILE_GOING_TO_SLEEP:
         /* Tell CGPS we are leaving this cell */
         gl1_timetag_cgps( l1_state, CGPS_TIMETAG_REQUEST, gas_id );
         break;
   }

   GL1_ISR_SAVE_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION gl1_drx_get_next_tick_req

DESCRIPTION
  Returns next_tick_required

===========================================================================*/
boolean gl1_drx_get_next_tick_req ( gas_id_t gas_id )
{
    return(next_tick_required[gas_id]);
}
/*===========================================================================

FUNCTION gl1_drx_set_sleep_inhibit

DESCRIPTION
  Sets sleep inhibit flag

===========================================================================*/
void gl1_drx_set_sleep_inhibit (boolean sleep_inhibit, gas_id_t gas_id)
{
    gl1_drx_sleep_inhibit[gas_id] = sleep_inhibit;
    MSG_GERAN_HIGH_1_G(GL1_DRX_HDR" sleep_inhibit %d",sleep_inhibit);
}

/*===========================================================================

FUNCTION gl1_drx_get_sleep_inhibit

DESCRIPTION
  gets sleep inhibit flag

===========================================================================*/
boolean gl1_drx_get_sleep_inhibit (void)
{
  int     i;

  for (i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++)
  {
    if(gl1_drx_sleep_inhibit[i] == TRUE)
    {
      return (TRUE);
    }
  }

  return(FALSE);
}


/*===========================================================================

FUNCTION gl1_drx_set_sleep_inhibit_for_cco_req

DESCRIPTION
  Sets sleep inhibit cco flag; cannot sleep when handling cco request,
  if we have cco failure, need to return to WCDMA, if we sleep
  CFN numbers are lost due to WCDMA clocks off

  If sleep_cco_inhibit is TRUE, sleep is inhibited, set to FALSE on power
  up and MPH_ENABLE_SLEEP_IND is received.

===========================================================================*/
void gl1_drx_set_sleep_inhibit_for_cco_req (boolean sleep_cco_inhibit, gas_id_t gas_id)
{
    gl1_drx_sleep_cco_inhibit[gas_id] = sleep_cco_inhibit;
}

/*===========================================================================

FUNCTION gl1_drx_set_sleep_inhibit_for_trm

DESCRIPTION
  Updates sleep inhibit vote for the specified GAS ID

===========================================================================*/
void gl1_drx_set_sleep_inhibit_for_trm(boolean sleep_inhibit, gas_id_t gas_id)
{
  GL1_TRM_LOCK();
  if ((int)gas_id < NUM_GERAN_DATA_SPACES)
  {
    gl1_drx_sleep_trm_inhibit[(int)gas_id] = sleep_inhibit;
  }
  GL1_TRM_UNLOCK();

  return;
}

/*===========================================================================

FUNCTION gl1_drx_get_sleep_inhibit_for_trm

DESCRIPTION
  Gets cumulative sleep inhibit flag (treating each data space as a "vote")

===========================================================================*/
boolean gl1_drx_get_sleep_inhibit_for_trm(void)
{
  boolean sleep_inhibit = FALSE;
  int     i;

  GL1_TRM_LOCK();
  for (i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++)
  {
    sleep_inhibit |= gl1_drx_sleep_trm_inhibit[i];
  }
  GL1_TRM_UNLOCK();

  return(sleep_inhibit);
}

#ifdef FEATURE_WLAN_COEX_SW_CXM
/*===========================================================================

FUNCTION gl1_drx_garb_send_sleep_duration

DESCRIPTION
  This function passes the sleep duration to GARB.

===========================================================================*/
void gl1_drx_garb_send_sleep_duration( boolean sleep, uint32 duration, gas_id_t gas_id)
{
  /* if sleeping for 0 duration then this sleep should be ignored
   * and sleep indication is not required to pass to GARB
   */
  if( sleep && ( duration == 0 ) )
  {
    return;
  }
  
  /* pass sleep duration in ms ( already rounded down) to CXM */
  garb_cxm_send_sleep_duration( sleep, duration, gas_id );
}
#endif

/* EOF */
