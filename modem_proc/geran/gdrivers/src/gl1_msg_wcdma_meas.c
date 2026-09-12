/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       G L 1  M E S S A G E  L A Y E R -  W C D M A  M E A S U R M E N T

GENERAL DESCRIPTION
   This module contains the procedures used for making wcdma measurments
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_wcdma_srchdrv_init
   gl1_msg_wcdma_init
   gl1_msg_cfg_wcdma_meas
   gl1_msg_cfg_wcdma_meas_ded
   gl1_msg_leave_wcdma_meas_ded
   gl1_msg_cleanup_wcdma_meas
   gl1_msg_wcdma_identify
   gl1_msg_abort_wcdma_identify
   gl1_msg_cfg_wcdma_reconfirm
   gl1_msg_wcdma_reconfirm
   gl1_msg_abort_wcdma_reconfirm
   gl1_msg_wcdma_ded_srch
   gl1_msg_abort_wcdma_ded_srch



INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002 - 2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_wcdma_meas.c#9 $
$DateTime: 2020/02/06 02:36:07 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
04/02/20   nv		CR2615257 Reverting the hack made to handle L1_TIME_LINE_DELAY scenario in HA as part of FR:51554
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
09/01/20   sc       CR2599376 clk 2.0: Reset g2w_mcpm_scenario after g2w deinit cnf is received
06/01/20   sc       CR2571123 Update MCPM for STOP sfter G2X init_cnf in all states except inactive mode.
06/01/20   sc      CR2561139 Update MCPM about Start/Stop for every G2X IRAT search gap in Idle mode.
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
28/09/17   ng       CR2117623 Set g2w_driver_state to ABORT_PENDING before sending msgr message to Wl1
12/07/17   ng       CR2074605 Do not leave idle mode until G2X init confirm is received
22/09/16   akm      CR1067259 CPU clock boost for G2W IRAT in QTA gap
01/06/16   dg       CR1023017 Don't set init req variable when already target tech is initialized
26/05/16   sk       CR1020757 Add DTF events for G2X IRAT scenarios
09/05/16   dg       CR1012577 Set g2x init flag to TRUE on receiving a new irat list if target tech is not initialized
15/04/16   dg       CR1003409 Don't crash during G2W initialiation if G2W driver state is DEINIT pending
12/02/16   dg       CR972232 Don't send abort flag to TRUE while sending RF exit request to WL1 after completion of G2W ID search
14/01/16   dg       CR961154 Don't send GERAN stop to MCPM if X tech hasn't sent STOP cnf
25/11/15   cja      CR942064 Add mcvs speed bump for G2L and G2T in dedicated mode 
24/11/15   cc       CR935704 PCH burst overlaps with G2W idle reconf gap
08/10/15   dg       CR920293 Set drx task as active while sending INIT_REQ to Xl1 on receiving DEINIT_CNF
02/10/15   cja      CR916645 Wait for G2W abort cnf before sending RF exit.
28/08/15   cja      CR892470 G2X to cope with overlapping init/deinit/init 
14/08/14    ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
07/08/14   dp       CR886529 Only send abort_cb when idle id handler is done
08/04/15   nk       CR880564 Set returned results from ded ABORT callback and avoid calling wcdma_ded_srch_abort_cb.
29/07/15   dp/dv    CR878618: Changes to wait for rf exit confirmation in gl1 for X2W srch
18/05/15    dg      CR838227 Remove frame_slaming_flag as block level checks are already there
                    to ensure no G2X irat is triggered if TS adjustment is pending
10/07/15   cja      CR870919 IRAT mutex to prevent race condition when G2W results recieved
30/06/15   cws      CR860979 Correct the frame counter in wcdma_reconfirm_handler 
26/06/15   zf       CR862712: For idle mode G2W search, wait until RF exit done before releasing TRM lock
29/06/15   cja      CR856110 Fix G2W dedicated being stuck in aborting state
25/06/15   zf       CR861092: Do not schedule NULL2X IRAT in transfer mode
19/06/15   hd       CR857428 Send GFW G2X startup and cleanup command in single SIM mode from IRAT search handler
15/06/15   zf       CR854868 After received ID search results, send RF_exit_ind 
11/06/15   zf       CR847878: Using PRE_SRCH_REQ and CNF for G2W
09/06/15   cja      CR850999 Correct qta cleanup for QTA gap for G2W IRAT
28/05/15   cws      CR842048 Do not abort during G2W idle reconf if abort comes after rf_enter
05/06/15   dp       CR833596 Making use of MSGR support of multi-instances of SUBs using MSGR variant    
20/05/15   aga      CR840984 NULL2X IRAT changes for G2W IRAT
19/05/15   cws      CR837503 Handle G2W abort properly when abort comes after rf_enter sent and before g2w_startup_cmd
11/05/15   cja      CR827065 Handle G2W initialisation requested when G2W shutdown in progress
07/05/15   cjl      CR834181 Do not check if W reconf in page block is OK when scheduling W reconf outside page block
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
06/05/15   cja      CR832886 Handle ID search abort before g2x startup
05/05/15   dp       CR830787 ensure srch handlers complete before SCE cleans up
01/05/15   aga      CR831392 Use the appropriate WCDMA client ID gased on static AS_ID mapping
27/05/15   jj       CR 828930   remove ISR lock from wcdmadrv_srch_abort_cb
21/04/15   cja      CR826376 When shutdown G2W only call wcdma inact once.
20/04/15   dp       CR825803: increase dedicated search timeout to prevent unintended crashes
10/03/15   zf       CR806007: Build G2W idle mode W reconf RF scripts in ISR context
09/04/15   dp       CR819956 Thor 2.x RF API change, G2W use MSGR
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
20/02/15   js       CR693771: W+T removal. Resolving compilation errors
30/01/15   dg       CR786825 On entering dedicated mode, configure G2W dedicated mode meas. only if no ded srch is in progress
20/01/14   zf       CR772571: Build G2W RF scripts one frame earlier
17/02/15   br       CR791646 Clear ded_srch_in_progres  while removing the handler , if GL1 aborting the search before issuing init wfw
17/02/15   br       CR768202 GL1  to remove the w id handler and send cleanup command in case if there are no step1 peaks found
13/02/15   pa       CR756156:G2X RF device handling using TRM apis.
12/01/15   sjv      CR756154 Enhance Band Registration for Activities
10/12/14   dg       CR765859 Do not go to sleep if MCPM is not updated for G2X MEAS STOP activity
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
25/11/14   dp       CR762681 Move featurization outside of MSG macro
17/11/14   dp       CR700052 Move g2w drivers into W filetree
23/10/14   pa       CR744906: Allow one IRAT DRDSDS activity at a time.
03/10/14   aga      CR732455 Changes required to tune away framework for TH 1.0 single sim build
13/08/14   xz        CR708896 Remove unuseful aborting treatment for TDS
29/08/14   cah      CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
29/04/14   dg       CR644521: While calling WCDMA search driver, if source RF device is invalid or max no., set
                    it to RFM_DEVICE_0
22/07/14   cah      CR697586 - trm irat meas end state crash.
11/07/14   dp       CR674070 - Use new G2W drivers interface (phase 1)
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS
07/05/14   cja      CR657218 In dedicated mode delay G2W gap start by 1250qs
29/04/14   dg      CR653626: WHile initiating G2W dedicated mode search, don't request MCPM to change the speed if
                                         Gl1 is already in packet transfer mode
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF and QDSP6
06/02/14   cja      CR612138 Always atore PN_pos as cx8, covert to/from cx1 for dedicated measure
02/12/14   js       CR595142 :  T+G compilation errors removed
01/12/13   sk       CR583361 Backing out the CR549390
27/11/13   aga      CR549390 G2W_TA IRAT Enable W ID search and reconf in case the other G sub is in data call
26/11/13   sai      CR578912: Clear G2W reconf search state if incorrectly left pending when no G2W reconfirmation
                              search is in progress.
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
25/11/13   pa   CR58117:Bolt: Support for GPLT G2W Dedicated Mode Measurement
08/05/13   dg       CR479252: Call wcdma_ded_srch_abort_cb in function l1_sc_wcdma_abort_ded_search
22/11/13   cja      CR455527 Remove featurisation from files in API directory
04/10/13   cja      CR551853 If abort W ID search before it starts do not remove handler until abort complete
06/05/13   br       CR483765 prevent issuing abort scond time after idsearch timeout
18/07/13   gk       CR513938:Mon counter for WCDMA reconf should not be incremented, if a WCDMA reconf is already in progress
21/08/13   cja      CR531211 Prevent mutex deadlock between GL1 ISR Lock and g2w srch drv lock
05/08/13   cja      CR515391 Set gl1_msg_wcdma_reconf_start_rf_cmd to NULL when initiating W recofirm
05/08/13   cja      CR514730 Send reconfirm abort to G2W driver, even if G2W initiate not complete
01/08/13   sp       CR520386: If abort ID search before W RF started, still send abort to G2W driver (merge CR415641).
01/08/13   cja      CR511042 Separate RF ID for active and CM.
31/07/13   cja      CR498800 Use TRM to get IRAT RF ID
23/07/13   og       CR498539. Delay sending the G2W ID search results by one frame when aborting.
11/07/13   cja      CR511042 Separate RF ID for active and CM.
18/06/13   cja      CR498800 Use TRM to get IRAT RF ID
20/06/13   sai      CR500680: Ensure G2W driver shutdown at abort immediately followed by
                          GL1 task deact at G next frame tick.
20/06/13   cs       Major Triton TSTS Syncup
29/05/13   dv       CR492212 Introduce SM to handle G2W abort
21/05/13   cs       TSTS Updates
17/05/13   cja      CR486815 Mutex lock for W neighbour inactive
16/05/13   og       CR453577.
06/05/13   dv        CR482086 Ensure when leaving Idle that wcdma_id_handler runs to completion
03/05/13   sjw      CR380127 Dedicated abort should return indication of failure
06/02/13   og       CR448695.Allow two frames between the G2X cleanup command in idle mode
                    and the set app mode command.
04/02/13   og       CR447348. Seperate the WCDMA ID search reporting in the message
                    layer so it happens one frame after the G2X cleanup is issued.
03/02/13   sjw      CR380127 Dedicated abort should return indication of failure
31/01/13   sjw      CR293070 Configure WCDMA callbacks for a specific GAS ID
15/04/13   cja      CR474368/CR445490 Do not abort twice when abort is frame after timeout.
03/04/13   cja      CR458005 Turn on W clock for G2W BPLMN
02/04/13   ss       CR467363 Set wcdma srch state to null after WCDMA search timeout
20/02/13   cja      CR454583 Correct g2w state for rex set on WFW start
11/01/13   cja      CR439767 Do not turn off W neighbour clock when going incative (needed for handover).
29/11/12   jj       CR425555 GL1 is stuck in Idle mode due to incomplete
                    abort of WCDMA reconfirmation.
22/11/12   cja      CR414098 For Dime only need W clks if W neighbours
29/10/12   cja      CR415641 If abort ID search before W RF started, still send abort to G2W driver.
16/10/12   cja      CR406119 Allow W clocks off in TCH if no W neighbours
17/08/12   cja      CR388504 Rework CR380970 for reconfirm so WFW shutdown on early abort
03/08/12    og      CR383632. Don't request a tuneback to GSM when the G2L startup request
                    is not sent to LTE L1.
08/08/12   cgc      CR380970 reworked gl1_msg_wcdma_id_srch_start_rf_cmd boolean to state.
07/08/12   cgc      CR380970 fix regression on previous Cr
24/07/12   cja      CR380970 If W mearurement RF not started when aborting call abort cb.
14/06/12   cja      CR359280 Turn off W/L/T clocks after WFW/LFW/TFW disable
31/05/12   cja      CR348100 Call srch cb if search stopped early as no peaks.
28/06/12   ws       CR364339 Only suspend power meas for IDLE frame during WCDMA ID search
                    avoids Ncell fluctuations in PTM as power meas in IDLE+1 is ignored
11/04/12   cja      CR350852 If G2W not scheduled due to slam, call abort cb.
21/03/12    jj      CR340963 Avoid scheduling g2x gap when frame is slammed
12/03/12   cgc      CR339393 Rework speedup renamed gl1_tch_speedup() to gl1_clkdata_speed()
24/02/12   og       CR339002 Handle failure to abort W ID search
03/15/12   mm       CR 344531 G2W drivers new design
13/03/12   og       CR302517. Reset the dedicated mode flag when starting WCDMA reconfirm
                    in idle mode.
29/02/12   cja      CR339905 Allow concurrent G2W and channel assignment speed-up
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
19/12/11   cja      CR326066 If id srch is aborted, issue g2x_cleanup straight away
26/10/11   mm       CR306187 Correct W RSSI report in dedicated
17/10/11   og       CR312232. Disable the extra debug messages which lead to GSTMR ISR delay.
04/10/11   cja     CR310265 Disable WFW after suspend WFW
23/09/11   mm       CR306187. Added extra debug F3 messages.
01/09/11   mm       Flip the order of calling g2w_srch_initiate_dedicated_search() and
                    gl1_hw_rf_sample_ram_cmd() for NikeL to take care of callflow change
24/08/11   og       CR302517. Reset the dedicated mode flag when starting WCDMA reconfirm
                    in idle mode.
01/08/11   scm      Change MSG_ERROR to ERR_FATAL on "Timeout on WCDMA reconfirm".
20/07/11   dp       CR291166. Ensure the sleep command is sent to WFW when the
                    search is done.
18/03/11   ws       Added Initial MCPM updates
15/02/11   ws       CR 275370 - Disable clk switching,Request 340 MIPS for duration
                    of GERAN RAT
14/01/11   npt      CR264682 Add support for asynch dsp completion interrupt
08/12/10   ws       CR 267264 - Merge NPA clients for CPU resource to 1 client to avoid
                    mutiple npa_cancel_request() which can take 1-2 mSecs to return
09/12/10   cs       CR264682 Add support for asynch dsp completion interrupt
30/11/10   og       CR266049 G2W dedicated timeline fix.
14/10/10   og       Featurisation fixes.
08/10/10   og       Issue WCDMA reconfirm and dedicated search startup and cleanup
                    commands from the ISR instead of TASK. CR259226.
15/09/10   og       CR254147. Make sure the WCDMA ID search g2x cleanup is not issued
                    without a g2x startup beforehand.
15/09/10   ip       CR248185 Increase MARM speed to 384MHz
24/08/10   og       CR250892. Ensure the WCDMA reconf handler runs after the ccch handler.
10/06/10   scm      Wait to sleep WFW until after search abort completes.
07/06/10   cja      Add W measurement start/stop RF API for BB Rx clock gating.
02/06/10   cja      CR239657. Remove early switch off W clocks, needed to read sample RAM
12/05/10   cja      CR235307. Switch off W clocks in frame after W measure
07/05/10   ip       CR235204 DCVS settings added for Poseidon 2
01/03/10   og       g2x and x2g updates.
23/02/10   ws       Integrate G2W IRAT driver from MDM8220
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
14/01/08   scm      Set WFW_IRAT_G2W_DEDICATED_MODE in wcdma_ded_srch_handler().
23/12/08   agv      Changed Enum define WfwG2WTypeEnum to align with new WFW.
12/15/08   scm      Make search TIMEOUT values variables for runtime modification.
10/12/08   agv      Added a new I/F for Mustang WFW.
08/12/08   cs       Add in new split gl1_hw_clk_ctl.h from gl1_hw.h
09/25/08   scm      More mods to support FEATURE_GSM_GPRS_MUSTANG feature.
26/08/08   og       Mustang GPLT changes.
28/07/98   hv       Added KxMutex support
25/10/07   cs       Remove dependency on l1_sc.h for gsm_only indication for CR117884
30/04/07   ws       Changes to Featurisation for first pass GSM Only build
25/09/06   nt       CR101231: - Added access function gl1_msg_get_id_srch_in_progress()
                    so PL1 knows if it needs to abort an active search before changing
                    its state.
04/12/06   ws       Removed FEATURE_MDSP_MULTI_RF_CHAIN_INTERFACE
03/27/06   ws       Corrected Featurisation compile error in prev. version
03/08/06   agv      Added FEATURE_MDSP_MULTI_RF_CHAIN_INTERFACE for diversity builds.
12/19/05   gfr      Move the clock enable from gl1_hw_wcdma_inact to
                    gl1_msg_cleanup_wcdma_meas since this function is called
                    from L1 as well.
09/22/05   gfr      Removed FEATURE_SBI_MDSP_CONTROLLER
07/29/05   gfr      DTM support
05/13/05   gfr      Added missing featurization
05/12/05   yh       Add wcdma init function.
02/03/05   bm       Support for ID searches in TBF
12/15/04   bm       Possible INTLOCK, INTFREE mismatch in wcdmadrv_ded_srch_cb
12/06/04   gfr      Featurize the mDSP SBI controller
11/18/04   bm       Wrapped wcdmadrv_ded_srch_cb with INTLOCK/INTFREE to avoid
                    curr_ded_rpt_ptr[gas_id] is beging set to NULL by high priotiy interrupt
                    resulting in a data abort
09/23/04   bm       Added funtion wcdma_ded_srch_timout()to be called at dedicated
                    search time out
07/26/04   gfr      Allow extra start time when reconfirming in paging block
06/28/04   gfr      Suspend peripheral SBI during WCDMA reconfirm.
06/02/04   gfr      Lint cleanup for Lint 7.50ad.
05/20/04   gfr      Mainlined FEATURE_COMMON_PWR_HANDLER
04/19/04   bm/jc    Set curr_ded_reconf_rpt_ptr[gas_id] to NULL if there are no cells to
                    reconfirm in gl1_msg_wcdma_ded_srch()
04/08/04   bm       Support for Packet Measurement Report in IDLE mode
03/05/04   gfr      Lint cleanup.
02/15/04   jc       Merge in xfer changes from Keith, changes per code review.
02/11/04   gfr      Support for reconfirm in GPRS mode.
01/20/04   jc       Updated for new gtow ho measurment algorithm.
12/17/03   tb       Fixes for aborting reconfirmation searches.
09/09/03   tb       Removed redundant MSG_HIGH for dedicated mode results.
08/07/03   tb       Changes from code review, and move dedi mode clock, rf
                    and sbi calls to gl1_hw.c
06/06/03   tb       Changes to support dedicated mode measurements
05/13/03   gfr      GSM/GPRS Common power handler support.
04/28/03   tb       Changes to allow registration/deregistration of MDSP isrs
02/13/03   tb       Fixes for aborting and debug msgs for systems testing.
02/04/03   mk       Added mechanism to allow only one initialization of the
                    WCDMA Searcher block.
11/26/02   gr       Suspend GPRS power monitors for the next frame.
11/25/02   jc/cp    Small changes requested by CP.
11/13/02   tb       Added support for list searching in paging block.
11/03/02   tb       Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


/* #define DEBUG_SYSTEMS_RESELECTION_GTOW */

#include "comdef.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_clk_ctl_g.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "msg.h"
#include "gl1_msg_wcdma_meas.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_mutex.h"
#include "l1_sc_int.h"
#include "l1_sc_drv.h"
#include "l1_sc_irat.h"
#include "l1i.h"
#include "l1_task.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"

#include "l1_drx.h"

#ifdef FEATURE_G2X_TUNEAWAY
#include "gpl1_dual_sim.h"
#endif

#include "gpl1_grm_intf.h"

#include "wl1x2wsrchapi.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

/*****************************************************

 LOCAL DEFINES

******************************************************/

/* Number of GSM frames (4.615 msec/frame) to wait before search timeout. */
#define IDENTIFY_TIMEOUT  90
#define RECONFIRM_TIMEOUT 20
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
#define DEDICATED_TIMEOUT 30
#endif

/* If we are doing reconfirms immediately following a PCH in idle mode
   then some extra delay is needed to allow for the search window */
#define PCH_EXTRA_START_OFFSET_QS (60)

/* Offset into idle frame for G2W gap start in dedicated */
#define G2W_DED_START_OFFSET  1250

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/* enum to describe the state of this module with the WL1 G2W driver */
typedef enum
{
  /* driver has not been initialized yet and cannot be used */
  G2W_DRIVER_NULL,
  /* driver has been sent an INIT_REQ message and gl1 is waiting for its
     completion */
  G2W_DRIVER_INIT_PENDING,
  /* driver is init and ready for search commands */
  G2W_DRIVER_READY,
  /* driver has been sent RF_ENTER_IND and PRE_SEARCH_REQ messages for an
     upcomming search. (PRE_SEARCH_REQ will cause building of scripts and
     start wfw). */
  G2W_DRIVER_SRCH_REQ_PENDING,
  /* driver has been sent SEARCH_REQ message for idle ID search and gl1 is
     waiting for results */
  G2W_DRIVER_IDLE_ID_PENDING,
  /* driver has been sent SEARCH_REQ message for idle reconf search and gl1 is
     waiting for results */
  G2W_DRIVER_IDLE_RECONF_PENDING,
  /* driver has been sent SEARCH_REQ message for dedicated search and gl1 is
     waiting for results */
  G2W_DRIVER_DEDICATED_PENDING,
  /* driver has been sent an ABORT_REQ message for whatever search was ongoing
     and gl1 is waiting for ABORT_CNF */
  G2W_DRIVER_ABORT_PENDING,
  /* driver has been sent a DEINIT_REQ and gl1 is waiting for its completion */
  G2W_DRIVER_DEINIT_PENDING
} gl1_msg_driver_state_enum_type;

typedef struct
{
  /* TRUE from when search request is given by upper layer until frame tick
     handler is uninstalled (the duration of one search transaction) */
  boolean srch_in_progress;

  /* TRUE if called abort callback or search_done_cb to upper layer */
  boolean returned_result;

  /* TRUE if this search transaction was aborted by upper layer or gl1 drivers
     decided to abort itself. Once this is set to TRUE gl1 will work toward
     deallocating all lower-layer gap resources until everything is back to a
     state ready for the next search transaction. This flag should remain TRUE
     for the rest of this search transaction. */
  boolean abort;
  
  /*TRUE if search is aborted by WCDMA because RF was not ready by the time 
   * WFW tries to receive the burst */
  boolean w_aborted_gap;
  
  /* TRUE if we started gap procedure with GFW (startup cmd sent) */
  boolean started_gfw_gap;

  /* TRUE between when rf_enter is called and rf_exit is called */
  boolean rf_exit_needed;

  /* gives a reference frame count relative to the air gap's first frame.  For
     example, if the gap physically starts in frame N and frame_timeline is -2
     that means we're currently performing actions for the frame N-2  */
  int8 gap_start_timeline;

} g2w_srch_state_struct_type;

/* gap_start_timeline field in below struct is initialized to this */
#define TIMELINE_INVALID 127

#endif

/*****************************************************

 LOCAL STORAGE

******************************************************/

#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW

static gl1_msg_wcdma_meas_report_cb_type  wcdma_identify_report_cb[NUM_GERAN_DATA_SPACES];
static gl1_msg_wcdma_meas_abort_cb_type  wcdma_identify_abort_cb[NUM_GERAN_DATA_SPACES];
static gl1_msg_wcdma_meas_report_cb_type  wcdma_reconfirm_report_cb[NUM_GERAN_DATA_SPACES];
static gl1_msg_wcdma_meas_abort_cb_type  wcdma_reconfirm_abort_cb[NUM_GERAN_DATA_SPACES];
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
static gl1_msg_wcdma_meas_handler_done_cb_type wcdma_srch_handler_done_cb[NUM_GERAN_DATA_SPACES];
#endif

/* storage of parameters for frame layer to use */
static gl1_wcdma_cell_meas_rpt *curr_id_rpt_ptr[NUM_GERAN_DATA_SPACES] =
                                  { INITIAL_VALUE( NULL ) };
static gl1_wcdma_cell_meas_rpt *curr_recon_rpt_ptr[NUM_GERAN_DATA_SPACES] =
                                  { INITIAL_VALUE( NULL ) };

static uint16 idle_id_UARFCN[NUM_GERAN_DATA_SPACES];
static uint16 idle_reconf_UARFCN[NUM_GERAN_DATA_SPACES];
static uint16 curr_start_time[NUM_GERAN_DATA_SPACES];

/* Flag to indicate if wcdma_identify measurements are to be suspended */
static boolean abort_id_srch[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static boolean abort_recon_search[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
/* Flag to if a wcdma measurement is currently in process */
static boolean id_srch_in_progress[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static boolean recon_srch_in_progress[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
/* Count used by frame tick handler to manage transaction */
static int reconfirm_frame_count[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };
static int identify_frame_count[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };

static boolean wcdmadrv_search_done[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static boolean wcdmadrv_valid_results[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
static boolean returned_idle_reconf_result[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static wl1_x2w_idle_reconf_input_struct_type idle_reconf_cell_params[NUM_GERAN_DATA_SPACES];
#else
static wl1_x2w_srch_srchdrv_id_parms_type id_srch_params[NUM_GERAN_DATA_SPACES];
static wl1_x2w_srch_g2w_list_input_task_type list_srch_params[NUM_GERAN_DATA_SPACES];
#endif

#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
static gl1_msg_wcdma_ded_report_cb_type  wcdma_ded_srch_report_cb[NUM_GERAN_DATA_SPACES];
static gl1_msg_wcdma_ded_abort_cb_type   wcdma_ded_srch_abort_cb[NUM_GERAN_DATA_SPACES];
static gl1_msg_wcdma_ded_reconf_cb_type  wcdma_ded_srch_reconf_cb[NUM_GERAN_DATA_SPACES];
static gl1_wcdma_cell_ded_rpt *curr_ded_rpt_ptr[NUM_GERAN_DATA_SPACES] =
                                 { INITIAL_VALUE( NULL ) };
static gl1_wcdma_cell_ded_reconf_rpt    *curr_ded_reconf_rpt_ptr[NUM_GERAN_DATA_SPACES] =
                                 { INITIAL_VALUE( NULL ) };

static uint16 curr_ded_UARFCN[NUM_GERAN_DATA_SPACES];
static uint16 curr_ded_start_time[NUM_GERAN_DATA_SPACES];

#ifndef FEATURE_GSM_TO_WCDMA_MSGR
static uint16 remaining_ded_cells[NUM_GERAN_DATA_SPACES];
static boolean abort_ded_srch[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static boolean ded_srch_in_progress[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static boolean ded_srch_frame_tick_active[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };
static int ded_srch_frame_count[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
static wl1_x2w_dedicated_input_struct_type ded_cell_params[NUM_GERAN_DATA_SPACES];
#else
/* WCDMA FW APP status for G2W */
static wl1_x2w_srch_wfw_status_enum_type            g2w_wfw_status[NUM_GERAN_DATA_SPACES];

static wl1_x2w_srch_g2w_ded_mode_srch_parms_type    ded_srch_params[NUM_GERAN_DATA_SPACES];
static wl1_x2w_srch_g2w_list_input_task_type        ded_reconf_srch_parms[NUM_GERAN_DATA_SPACES];

static void wcdmadrv_ded_srch_cb(
              wl1_x2w_srch_g2w_ded_mode_list_srch_struct_type *list_ptr,gas_id_t gas_id );
static void wcdmadrv_ded_reconf_cb(
              wl1_x2w_srch_list_srch_struct_type *list_ptr,gas_id_t gas_id );

#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

static boolean wcdma_ded_srch_handler( boolean start_of_block,
                                   uint32  time_stamp,gas_id_t gas_id);
#endif /* FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW */

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
static gl1_msg_driver_state_enum_type gl1_msg_g2w_driver_state[NUM_GERAN_DATA_SPACES] =
                   {INITIAL_VALUE(G2W_DRIVER_NULL)};

g2w_srch_state_struct_type ded_srch_state[NUM_GERAN_DATA_SPACES];

static boolean gl1_msg_g2w_pre_srch_cnf[NUM_GERAN_DATA_SPACES] =
                   {INITIAL_VALUE(FALSE)};
#else /* FEATURE_GSM_TO_WCDMA_MSGR */
typedef enum
{
   G2WDRV_SRCH_ABORT_FALSE,
   G2WDRV_SRCH_ABORTING,
   G2WDRV_SRCH_ABORT_COMPLETE
} g2wdrv_srch_abort_e;

static g2wdrv_srch_abort_e wcdmadrv_search_aborted[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(G2WDRV_SRCH_ABORT_FALSE) };

typedef enum
{
  G2W_CMD_NULL,
  G2W_CMD_WAITING_FOR_TRIGGER,
  G2W_CMD_TRIGGERED
} g2w_cmd_trigger_e;

/* This flag is checked in the wcdma_id_handler and is used
 * in idle mode G2W search only.
 */
static g2w_cmd_trigger_e  gl1_msg_wcdma_id_srch_start_rf_cmd[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( G2W_CMD_NULL ) };

static g2w_cmd_trigger_e  gl1_msg_wcdma_reconf_start_rf_cmd[NUM_GERAN_DATA_SPACES]  =
                 { INITIAL_VALUE( G2W_CMD_NULL ) };

#define MEAS_PTR_CB_VALID(ptr) (NULL != (ptr))
#define MEAS_GAS_ID_VALID(gid) ((int)(gid) < NUM_GERAN_DATA_SPACES)
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/
static boolean wcdma_id_handler( boolean start_of_block,
                                   uint32  time_stamp,gas_id_t gas_id );
static boolean wcdma_reconfirm_handler( boolean start_of_block,
                                   uint32  time_stamp,gas_id_t gas_id );

#ifndef FEATURE_GSM_TO_WCDMA_MSGR
static void wcdmadrv_step3_search_cb(wl1_x2w_srch_pn_after_nastt_results_type *pn_after_nastt_ptr,gas_id_t gas_id);
static void wcdmadrv_list_srch_cb(wl1_x2w_srch_list_srch_struct_type  *list_ptr,gas_id_t gas_id);
static void wcdmadrv_wfw_status(wl1_x2w_srch_wfw_status_enum_type,gas_id_t gas_id);

static void wcdmadrv_srch_abort_cb(gas_id_t gas_id);
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */


/*****************************************************

 EXPORTED FUNCTIONS

******************************************************/

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
boolean gl1_wcdma_meas_get_wcdma_srch_aborted(gas_id_t gas_id)
{
  boolean result = (gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_ABORT_PENDING);

  MSG_GERAN_HIGH_1_G("G2W: gl1_wcdma_meas_get_wcdma_srch_aborted: %d",
                     result);

  return result;
}
#else /* FEATURE_GSM_TO_WCDMA_MSGR */
boolean gl1_wcdma_meas_get_wcdma_srch_aborted(gas_id_t gas_id)
{
  MSG_GERAN_HIGH_1_G("gl1_wcdma_meas_get_wcdma_srch_aborted: %d",wcdmadrv_search_aborted[gas_id]);

  return wcdmadrv_search_aborted[gas_id];
}

void gl1_wcdma_meas_set_wcdma_srch_aborted(gas_id_t gas_id, boolean value)
{
  MSG_GERAN_HIGH_2_G("gl1_wcdma_meas_set_wcdma_srch_aborted: %d, value:%d",
                     wcdmadrv_search_aborted[gas_id],value);

  wcdmadrv_search_aborted[gas_id] = value;
}
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

/*===========================================================================
FUNCTION gl1_msg_get_recon_srch_progress

DESCRIPTION
  This function gives the status of configured recon srch for the specified gas id

DEPENDENCIES
  None

RETURN VALUE
  recon srch status

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_get_recon_srch_progress ( gas_id_t gas_id)
{
  return recon_srch_in_progress[gas_id];
}

/*===========================================================================

FUNCTION gl1_msg_wcdma_srchdrv_init

DESCRIPTION
  This function performs initialization needed after each load of mdsp
  firmware before wcdma searching can take place. This is to initialize
  MDSP variables that will not change.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_wcdma_srchdrv_init( rfm_device_enum_type  source_rf_device, gas_id_t gas_id )
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
  rfm_device_enum_type  target_rf_device;

  MSG_GERAN_HIGH_0_G("G2W: gl1_msg_wcdma_srchdrv_init.");

  /* Get WCDMA RF device ID, use default band (should really use actual W band to be used) */
  //target_rf_device = gl1_hw_get_wcdma_rf_id(0);
  /* For now use RF device 0 */
  target_rf_device = RFM_DEVICE_0;

  if((source_rf_device == RFM_MAX_DEVICES) || (source_rf_device == RFM_INVALID_DEVICE))
  {
    MSG_ERROR("Source RF device is invalid. Set it to RF Device 0", 0, 0, 0);
    source_rf_device = RFM_DEVICE_0;
  }
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  /* sent INIT_REQ to wl1 driver */
  if(gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_NULL)
  {
    l1_send_IRAT_G2W_INIT_REQ(gl1_msg_get_multi_sim_sys_mode(),
                              gas_id);
    gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_INIT_PENDING;
  }
  else if (gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_DEINIT_PENDING)
  {
     MSG_GERAN_HIGH_0_G("Deinit ongoing.Init will happen on receiving DEINIT_CNF.");
  
  }
  else
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("G2W: Trying to send INIT_REQ when driver isn't in NULL state", 0, 0, 0);
  }
#else /* FEATURE_GSM_TO_WCDMA_MSGR */
  /* call driver init function */
  wl1_x2w_srch_wcdma_cfg_init(wcdmadrv_step3_search_cb,
                              wcdmadrv_list_srch_cb,
                              wcdmadrv_srch_abort_cb,
                              wcdmadrv_wfw_status,
                              source_rf_device,
                              target_rf_device,
                              gas_id);

  /* clear this variable in case previous exit failed */
  ded_srch_in_progress[gas_id] = FALSE;

#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
#endif /*FEATURE_INTERRAT_CELL_RESELECTION_GTOW*/
}

/*===========================================================================

FUNCTION gl1_msg_wcdma_init

DESCRIPTION
  This function cleans all the state variables whenever GSM is brought up.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_wcdma_init( gas_id_t gas_id )
{
  #ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
  wcdma_identify_report_cb[gas_id]     = NULL;
  wcdma_identify_abort_cb[gas_id]      = NULL;
  wcdma_reconfirm_report_cb[gas_id]    = NULL;
  wcdma_reconfirm_abort_cb[gas_id]     = NULL;
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  wcdma_srch_handler_done_cb[gas_id]   = NULL;
  #endif

  curr_id_rpt_ptr[gas_id]              = NULL;
  curr_recon_rpt_ptr[gas_id]           = NULL;
  idle_id_UARFCN[gas_id]               = 0;
  idle_reconf_UARFCN[gas_id]            = 0;
  curr_start_time[gas_id]              = 0;
  abort_id_srch[gas_id]                = FALSE;
  abort_recon_search[gas_id]           = FALSE;
  id_srch_in_progress[gas_id]          = FALSE;
  recon_srch_in_progress[gas_id]       = FALSE;
  reconfirm_frame_count[gas_id]        = 0;
  identify_frame_count[gas_id]         = 0;

  wcdmadrv_search_done[gas_id]         = FALSE;
  wcdmadrv_valid_results[gas_id]       = FALSE;
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  returned_idle_reconf_result[gas_id]  = FALSE;
  #else
  wcdmadrv_search_aborted[gas_id]      = G2WDRV_SRCH_ABORT_FALSE;
  #endif

  #ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
  wcdma_ded_srch_report_cb[gas_id]     = NULL;
  wcdma_ded_srch_abort_cb[gas_id]      = NULL;
  wcdma_ded_srch_reconf_cb[gas_id]     = NULL;

  curr_ded_rpt_ptr[gas_id]             = NULL;
  curr_ded_reconf_rpt_ptr[gas_id]      = NULL;
  curr_ded_UARFCN[gas_id]              = 0;
  curr_ded_start_time[gas_id]          = 0;
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
  memset(&ded_srch_state[gas_id], 0, sizeof(ded_srch_state[gas_id]));
#else
  remaining_ded_cells[gas_id]          = 0;
  abort_ded_srch[gas_id]               = FALSE;
  ded_srch_in_progress[gas_id]         = FALSE;
  ded_srch_frame_count[gas_id]         = 0;
#endif

#ifndef FEATURE_GSM_TO_WCDMA_MSGR
  /* Initialize WCDMA FW APP status */
  g2w_wfw_status[gas_id]               = WL1_X2W_WFW_DISABLED;
#endif
#endif
#endif /*FEATURE_INTERRAT_CELL_RESELECTION_GTOW*/
}

/*===========================================================================

FUNCTION gl1_msg_set_gsm_only

DESCRIPTION
  Store the status of the local gsm_only_mode status.

DEPENDENCIES
  None

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_set_gsm_only( boolean gl1_gsm_only_mode,gas_id_t gas_id )
{
  gl1_hw_set_gsm_only_mode( gl1_gsm_only_mode,gas_id );
}

/*===========================================================================

FUNCTION gl1_msg_wcdma_srch_active

DESCRIPTION
  Store the status of the W search active to enable the drivers layer
  to detect when W searches are really active.

DEPENDENCIES
  None

RETURN
  Whether W search is active in terms of RR Ncell lists

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_wcdma_srch_active( gas_id_t gas_id )
{
  return ( l1_sc_wcdma_srch_active(gas_id) );
}

/*===========================================================================

FUNCTION gl1_msg_cfg_wcdma_meas

DESCRIPTION
  This function stores callback functions and performs initial configuration
  to allow for future calls to gl1_msg_wcdma_identify. This function
  should be called once after each entry to GSM mode.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cfg_wcdma_meas(
            gl1_msg_wcdma_meas_report_cb_type id_report_cb,
            gl1_msg_wcdma_meas_abort_cb_type  id_abort_cb,
            gl1_msg_wcdma_meas_report_cb_type recon_report_cb,
            gl1_msg_wcdma_meas_abort_cb_type  recon_abort_cb,
            #ifdef FEATURE_GSM_TO_WCDMA_MSGR
            gl1_msg_wcdma_meas_handler_done_cb_type handler_done_cb,
            #endif
            gas_id_t gas_id)
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
  MSG_GERAN_HIGH_0_G("G2W: Idle callbacks configured.");

  /* store callback pointers */
  wcdma_identify_report_cb[gas_id]   = id_report_cb;
  wcdma_identify_abort_cb[gas_id]    = id_abort_cb;
  wcdma_reconfirm_report_cb[gas_id]  = recon_report_cb;
  wcdma_reconfirm_abort_cb[gas_id]   = recon_abort_cb;
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  wcdma_srch_handler_done_cb[gas_id] = handler_done_cb;
  #endif
#endif /*FEATURE_INTERRAT_CELL_RESELECTION_GTOW*/
}

/*===========================================================================

FUNCTION gl1_msg_cfg_wcdma_meas_ded

DESCRIPTION
  This function stores callback functions and performs initial configuration
  to allow for future calls to gl1_msg_wcdma_. This function
  should be called once after each entry to GSM mode.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cfg_wcdma_meas_ded(
            gl1_msg_wcdma_ded_report_cb_type ded_report_cb,
            gl1_msg_wcdma_ded_abort_cb_type ded_abort_cb,
            gl1_msg_wcdma_ded_reconf_cb_type ded_reconf_cb,
            #ifdef FEATURE_GSM_TO_WCDMA_MSGR
            gl1_msg_wcdma_meas_handler_done_cb_type handler_done_cb,
            #endif
            gas_id_t gas_id)
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW

  MSG_GERAN_HIGH_0_G("G2W: Ded callbacks configured");

  /* store callback pointers */
  wcdma_ded_srch_report_cb[gas_id] = ded_report_cb;
  wcdma_ded_srch_abort_cb[gas_id]  = ded_abort_cb;
  wcdma_ded_srch_reconf_cb[gas_id] = ded_reconf_cb;
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  wcdma_srch_handler_done_cb[gas_id] = handler_done_cb;
  #endif

#ifndef FEATURE_GSM_TO_WCDMA_MSGR
  /* call driver init function */
  wl1_x2w_srch_wcdma_ded_mode_cfg_init (wcdmadrv_ded_srch_cb, wcdmadrv_ded_reconf_cb, wcdmadrv_wfw_status,gas_id);
#endif

#endif /* FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW */
#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
}

/*===========================================================================

FUNCTION gl1_msg_leave_wcdma_meas_ded

DESCRIPTION
  This function stores callback functions and performs initial configuration
  to allow for future calls to gl1_msg_wcdma_. This function
  should be called once after each entry to GSM mode.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_leave_wcdma_meas_ded( gas_id_t gas_id )
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW

  MSG_GERAN_HIGH_0_G("gl1_msg_leave_wcdma_meas_ded.");

#endif /* FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW */
#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
}


/*===========================================================================

FUNCTION gl1_msg_cleanup_wcdma_meas

DESCRIPTION
  This function cleans-up the WCDMA driver when we are done with it.
  Should only be called once all g2w processing is complete.

===========================================================================*/
void gl1_msg_cleanup_wcdma_meas( gas_id_t gas_id )
{
#if defined(FEATURE_WCDMA) && defined (FEATURE_INTERRAT_CELL_RESELECTION_GTOW)
  /* Cleanup WCDMA measurements. Disable WCDMA FW APP will happen */

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
  {
    if(gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_READY)
    {
      MSG_GERAN_HIGH_0_G("G2W: Shutdown: Sending DEINIT_REQ MSG to Wl1");
      l1_send_IRAT_G2W_DEINIT_REQ(gas_id);
      gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_DEINIT_PENDING;
    }
    else if(gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_NULL)
    {
      MSG_GERAN_HIGH_0_G("G2W: Shutdown: WL1 driver is already shutdown. No MSG sent.");
    }
    else if(gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_DEINIT_PENDING)
    {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_FATAL("G2W: Shutdown: Error: trying to cleanup with driver in non-ready state", 0, 0, 0);
    }
  }
#else /* ! FEATURE_GSM_TO_WCDMA_MSGR */
  {
    MSG_GERAN_HIGH_0_G("Shutting down g2w driver");

    {
      GDRV_ISR_LOCK(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        {
          trm_get_info_return_type get_granted;

          wl1_x2w_srch_g2w_cleanup_wcdma_meas(
                       (TRM_DENIAL == grm_get_granted(WCDMA_CLIENT_FOR_THIS_SUB(gas_id), &get_granted, gas_id)),
                       gas_id);
        }
#else
        {
          wl1_x2w_srch_g2w_cleanup_wcdma_meas(gas_id);
        }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      GDRV_ISR_UNLOCK(gas_id);
    }
  }
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
#endif /* FEATURE_WCDMA && FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
}

/*===========================================================================

FUNCTION gl1_msg_wcdmadrv_abort_search

DESCRIPTION
  This function calls the g2w driver function to abort the G2W search

===========================================================================*/
void gl1_msg_wcdmadrv_abort_search(gas_id_t gas_id)
{
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
  if(gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_ABORT_PENDING)
  {
    MSG_GERAN_HIGH_0_G("G2W: Abort: ABORT_REQ already pending. Not sending WL1 another ABORT_REQ.");
  }
  else if(gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_SRCH_REQ_PENDING ||
          gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_IDLE_ID_PENDING ||
          gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_IDLE_RECONF_PENDING ||
          gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_DEDICATED_PENDING)
  {
    gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_ABORT_PENDING;
    L1_send_IRAT_G2W_ABORT_REQ(gas_id);
  }
  else
  {
    MSG_GERAN_HIGH_1_G("G2W: Abort: WL1 has no active searches (state %d). Not sending ABORT_REQ.",
                       gl1_msg_g2w_driver_state[gas_id]);
  }

#else /* FEATURE_GSM_TO_WCDMA_MSGR */

  /* This will be set complete when response from driver arrives */
  wcdmadrv_search_aborted[gas_id]= G2WDRV_SRCH_ABORTING;
  /* Tell the driver to abort */
  wl1_x2w_srch_wcdmadrv_abort_search(gas_id);
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}

/*===========================================================================

FUNCTION gl1_msg_wcdma_identify

DESCRIPTION
  This function starts a wcdma id search. Parameters are saved off or
  passed to the wcdma search driver, and a frame tick handler function
  is set up to handle the transaction.

  UARFCN           - The WCDMA channel number.
  rpt              - Pointer to the report that will be filled up and passed
                     to the callback.

DEPENDENCIES
  gl1_msg_cfg_wcdma_meas must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  Results or Abort callback will eventually be called.
===========================================================================*/
void gl1_msg_wcdma_identify(uint16                       UARFCN,
                            gl1_wcdma_cell_meas_rpt      *rpt,
                            gas_id_t                     gas_id)
{
  #ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW

  if (id_srch_in_progress[gas_id])
  {
    MSG_GERAN_ERROR_0_G("G2W: Idle: WCDMA measurement already in progress.");
    return;
  }

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  if(gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_READY)
  {
    MSG_GERAN_ERROR_1_G("G2W: Driver not ready for ID search (state: %d)", gl1_msg_g2w_driver_state[gas_id]);
    return;
  }
  #endif

  MSG_GERAN_HIGH_1_G("G2W: Idle ID: search started on UARFCN %d", UARFCN);
  
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
  /*Inform MCPM about critical scenario now if needed*/
  if((l1_sc_irat_get_g2x_abort_count(gas_id) != 0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
  {
    gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0, L1_TIME_LINE_DELAY, gas_id);
    critical_scn_updated_to_mcpm[gas_id] = TRUE;
  }
#endif

  /* set status variables to initial values */
  id_srch_in_progress[gas_id] = TRUE;
  wcdmadrv_search_done[gas_id] = FALSE;
  wcdmadrv_valid_results[gas_id] = FALSE;
  #ifndef FEATURE_GSM_TO_WCDMA_MSGR
  wcdmadrv_search_aborted[gas_id] = G2WDRV_SRCH_ABORT_FALSE;
  #endif

  /* save off parameters and setup struct to pass to driver */
  curr_id_rpt_ptr[gas_id] = rpt;

  idle_id_UARFCN[gas_id] = UARFCN;

#ifndef FEATURE_GSM_TO_WCDMA_MSGR
 /* Disable mDSP BB Rx clock gating for duration of W measurement */
  gl1_hw_enable_bbrx_clock_gating(FALSE);
#endif

  /* set up frame handler */
  identify_frame_count[gas_id] = -1;
  gl1_msgi_add_ft_handler(wcdma_id_handler, GL1_MSG_FT_HDLR_ACQ_NCELL, gas_id);

  #endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
}


/*===========================================================================

FUNCTION gl1_msg_wcdma_reconfirm

DESCRIPTION
  This function starts a wcdma list search. Parameters are saved off or
  passed to the wcdma search driver, and a frame tick handler function
  is set up to handle the transaction.

  UARFCN           - The WCDMA channel number.
  num_cell_params  - Size of the cell params array.
  cells            - Array of structs with cell specific info.
  rpt              - Pointer to the report that will be filled up and passed
                     to the callback.
  tn_offset        - How many timeslots to offset the start of the reconfirm,
                     used in GPRS.

DEPENDENCIES
  gl1_msg_cfg_wcdma_meas must have been called at some time prior.

RETURN VALUE
  TRUE: Reconfirmation scheduled.

SIDE EFFECTS
  Results or Abort callback will eventually be called.
===========================================================================*/
boolean gl1_msg_wcdma_reconfirm (
                             uint16                       UARFCN,
                             uint16                       num_cell_params,
                             gl1_wcdma_cell_params_type   *cells,
                             gl1_wcdma_cell_meas_rpt      *rpt,
                             gl1_defs_tn_type             tn_offset,
                             uint16                       freq_idx,
                             gas_id_t                     gas_id )

{
  boolean reconf_scheduled = FALSE;
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
  int16 idx;
  boolean activity_can_run = TRUE;

  if(num_cell_params > MAX_WCDMA_CELLS_PER_UARFCN) {
    MSG_GERAN_ERROR_0_G("G2W: Idle Reconf: Unexpected number of wcdma cell descriptors.");
    num_cell_params = MAX_WCDMA_CELLS_PER_UARFCN;
  }

  if(recon_srch_in_progress[gas_id])
  {
    MSG_GERAN_ERROR_0_G("G2W: Idle Reconf: WCDMA measurement already in progress.");
    return FALSE;
  }

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  if(gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_READY)
  {
    MSG_GERAN_ERROR_1_G("G2W: Idle Reconf: Driver not ready for search (state: %d)", gl1_msg_g2w_driver_state[gas_id]);
    return FALSE;
  }
  #endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( FM_PRI_WCDMA_RECONF != l1_fm_get_running_activity(gas_id))
  {
    /* Before scheduling W reconfirmation, check TRM Band Registration */
    /*report band to TRM and check for denial before scheduling WCDMA reconf*/
    activity_can_run = grm_check_w_t_reconf_in_pg_block_ok(MAX_FRAMES_FOR_W_RECONF,
                                                           freq_idx,
                                                           gas_id);
  }
#endif

  MSG_GERAN_HIGH_2_G("G2W: Idle Reconf: Start UARFCN %d (can run: %d)", UARFCN, activity_can_run);

  if (activity_can_run)
  {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
    /*Inform MCPM about critical scenario now if needed*/
    if((l1_sc_irat_get_g2x_abort_count(gas_id) !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
    {
      gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
      critical_scn_updated_to_mcpm[gas_id] = TRUE;
    }
#endif
    /* set up frame handler */
    gl1_msgi_add_ft_handler( wcdma_reconfirm_handler, GL1_MSG_FT_HDLR_ACQ_NCELL,gas_id );

    /*Ideally, gl1_msgi_add_ft_handler should return TRUE/FALSE, but this function
      has beeen used at so many places, therefore we can keep this exercise in
      our low priority tasks list*/
    /*Check whether handler is installed or not.*/
    reconf_scheduled = gl1_msgi_is_ft_handler_registered(wcdma_reconfirm_handler,
                                                         GL1_MSG_FT_HDLR_ACQ_NCELL,
                                                         gas_id);
  }

  if (reconf_scheduled && activity_can_run)
  {
    /* set status variables to initial values */
    recon_srch_in_progress[gas_id] = TRUE;
    wcdmadrv_search_done[gas_id] = FALSE;
    wcdmadrv_valid_results[gas_id] = FALSE;
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
    returned_idle_reconf_result[gas_id] = FALSE;
#else
    wcdmadrv_search_aborted[gas_id] = G2WDRV_SRCH_ABORT_FALSE;
#endif

    /* save off parameters and setup struct to pass to driver */
    curr_recon_rpt_ptr[gas_id] = rpt;
    curr_start_time[gas_id] = ((uint16)tn_offset + 1) * QS_PER_SLOT + PCH_EXTRA_START_OFFSET_QS;

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
    reconfirm_frame_count[gas_id] = -1;

    idle_reconf_UARFCN[gas_id] = UARFCN;

    idle_reconf_cell_params[gas_id].num_cells = num_cell_params;
    for (idx=0; idx<num_cell_params && idx<WL1_X2W_SRCH_MAX_PSC_PER_FREQ; idx++)
    {
      idle_reconf_cell_params[gas_id].cells[idx].psc = cells[idx].scr_code;
      idle_reconf_cell_params[gas_id].cells[idx].pos = cells[idx].PN_pos;
    }

#else /* FEATURE_GSM_TO_WCDMA_MSGR */
    reconfirm_frame_count[gas_id] = 0;

    idle_reconf_UARFCN[gas_id] = list_srch_params[gas_id].freq = UARFCN;

    list_srch_params[gas_id].num_tasks = num_cell_params;
    for (idx=0; idx<num_cell_params; idx++)
    {
      list_srch_params[gas_id].list_srch_tasks[idx].scr_code = cells[idx].scr_code;
      list_srch_params[gas_id].list_srch_tasks[idx].pn_pos = cells[idx].PN_pos;
    }

    /* Disable mDSP BB Rx clock gating for duration of W measurement */
    gl1_hw_enable_bbrx_clock_gating ( FALSE );
    
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

  }

#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
  return reconf_scheduled;
}


/*===========================================================================

FUNCTION gl1_msg_wcdma_ded_srch

DESCRIPTION
  This function starts a wcdma id search. Parameters are saved off or
  passed to the wcdma search driver, and a frame tick handler function
  is set up to handle the transaction.

  UARFCN           - The WCDMA channel number.
  num_cell_params  - Size of the cell params array.
  cells            - Array of structs with cell specific info.
  rpt              - Pointer to the report that will be filled up and passed
                     to the callback.
  reconf_rpt       - Pointer to the report of reconfirmed cells that will
                     be filled and passed up to the callback.
  tn_offset        - How many timeslots to offset the start of the reconfirm,
                     used in GPRS.

DEPENDENCIES
  gl1_msg_cfg_wcdma_identify must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  Results or Abort callback will eventually be called.
===========================================================================*/
void gl1_msg_wcdma_ded_srch (
                             uint16                       UARFCN,
                             uint16                       num_cell_params,
                             gl1_wcdma_cell_params_type   *cells,
                             gl1_wcdma_cell_ded_rpt       *rpt,
                             uint16                       num_cell_reconf,
                             gl1_wcdma_cell_params_type   *reconf_cells,
                             gl1_wcdma_cell_ded_reconf_rpt *reconf_rpt,
                             gl1_defs_tn_type             tn_offset,
                             gas_id_t                     gas_id )
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW

  int16 idx;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if ((num_cell_params==0) && (num_cell_reconf==0))
  {
    MSG_GERAN_ERROR_0_G("G2W: Ded: search requested with zero cells");
    return;
  }

  if (num_cell_params > MAX_WCDMA_CELLS_PER_DED_SRCH) {
    MSG_GERAN_ERROR_1_G("G2W: Ded: Unexpected number %d of wcdma cell descriptors.",
         num_cell_params);
    num_cell_params = MAX_WCDMA_CELLS_PER_DED_SRCH;
  }

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  if(ded_srch_state[gas_id].srch_in_progress == TRUE)

  {
    MSG_GERAN_ERROR_0_G("G2W: Ded: WCDMA measurement already in progress.");
    return;
  }

  MSG_GERAN_HIGH_3_G("G2W: Ded: Start UARFCN %d. %d ID / %d Reconf cells", UARFCN, num_cell_params, num_cell_reconf);

#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
  /*Inform MCPM about critical scenario now if needed*/
  if((l1_sc_irat_get_g2x_abort_count(gas_id) !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
  {
    gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0, L1_TIME_LINE_DELAY,gas_id);
    critical_scn_updated_to_mcpm[gas_id] = TRUE;
  }
#endif
  memset(&ded_srch_state[gas_id], 0, sizeof(ded_srch_state[gas_id]));

  /* set status variables to initial values */
  if  ( (num_cell_params) || (num_cell_reconf) )
  {
    ded_srch_state[gas_id].srch_in_progress = TRUE;
  }

  #else /* FEATURE_GSM_TO_WCDMA_MSGR */

  if (ded_srch_in_progress[gas_id])
  {
    MSG_GERAN_ERROR_0_G("G2W: Ded: WCDMA measurement already in progress.");
    return;
  }

  MSG_GERAN_HIGH_3_G("G2W: Ded: Start UARFCN %d. %d ID / %d Reconf cells", UARFCN, num_cell_params, num_cell_reconf);

  /* set status variables to initial values */
  if  ( (num_cell_params) || (num_cell_reconf) )
  {
    ded_srch_in_progress[gas_id] = TRUE;  // True if ID or reconfirmations are in progress
  }
  ded_srch_frame_tick_active[gas_id] = TRUE;

  wcdmadrv_search_aborted[gas_id] = G2WDRV_SRCH_ABORT_FALSE;
  #endif /* FEATURE_GSM_TO_WCDMA_MSGR */

  /* save off parameters and setup struct to pass to driver */
  if (num_cell_params)
  {
    curr_ded_rpt_ptr[gas_id]        = rpt;                // stage1/stage 2 results here
  }
  if (num_cell_reconf)
  {
    curr_ded_reconf_rpt_ptr[gas_id] = reconf_rpt;         // reconfirmation results here
  }
  else
  {
    curr_ded_reconf_rpt_ptr[gas_id] = NULL;               // ensure ptr is zeroed out
  }

  curr_ded_start_time[gas_id] = G2W_DED_START_OFFSET;

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  curr_ded_UARFCN[gas_id] = UARFCN;
  ded_cell_params[gas_id].num_id_cells = num_cell_params;
  for (idx=0; idx<num_cell_params && idx < WL1_X2W_SRCH_DED_MAX_PSC_PER_FREQ; idx++)
  {
    ded_cell_params[gas_id].id_cell_pscs[idx] = cells[idx].scr_code;
  }

  ded_cell_params[gas_id].num_reconf_cells = num_cell_reconf;
  for (idx=0; idx<num_cell_reconf && idx<WL1_X2W_SRCH_DED_MAX_PSC_PER_FREQ; idx++)
  {
    ded_cell_params[gas_id].reconf_cells[idx].psc = reconf_cells[idx].scr_code;
    ded_cell_params[gas_id].reconf_cells[idx].pos = reconf_cells[idx].PN_pos;
  }

  ded_srch_state[gas_id].gap_start_timeline = TIMELINE_INVALID;

  #else /* FEATURE_GSM_TO_WCDMA_MSGR */

  curr_ded_UARFCN[gas_id] = ded_srch_params[gas_id].freq = UARFCN;
  remaining_ded_cells[gas_id] = ded_srch_params[gas_id].num_cells = num_cell_params;
  ded_srch_params[gas_id].num_cells_to_be_searched_per_sample_ram = num_cell_params;
  for (idx=0; idx<num_cell_params; idx++)
  {
    ded_srch_params[gas_id].ded_mode_cell[idx].scr_code = cells[idx].scr_code;
    /* Convert from cx8 to cx1 for ded srch PN pos */
    ded_srch_params[gas_id].ded_mode_cell[idx].pn_pos = (uint16) (cells[idx].PN_pos >> 3);
  }

  /* fill in the reconfirmation data of last known cells on this freq */
  ded_reconf_srch_parms[gas_id].freq      = UARFCN;
  ded_reconf_srch_parms[gas_id].num_tasks = num_cell_reconf;
  for (idx=0; idx<num_cell_reconf; idx++)
  {
    ded_reconf_srch_parms[gas_id].list_srch_tasks[idx].scr_code      = reconf_cells[idx].scr_code;
    ded_reconf_srch_parms[gas_id].list_srch_tasks[idx].pn_pos        = reconf_cells[idx].PN_pos;
  }

  /* set up frame handler */
  ded_srch_frame_count[gas_id] = -1;
  #endif /* FEATURE_GSM_TO_WCDMA_MSGR */

  gl1_msgi_add_ft_handler( wcdma_ded_srch_handler, GL1_MSG_FT_HDLR_ACQ_NCELL,gas_id );

#endif /* FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW */
#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
}

/*===========================================================================

FUNCTION gl1_msg_abort_wcdma_ded_srch

DESCRIPTION
  This functions aborts currently scheduled wcdma search.

DEPENDENCIES
  gl1_msg_wcdma_ded_srch must have been called at some time prior.

RETURN VALUE
  FALSE if no search was in progress when this abort was requested


SIDE EFFECTS
  Abort callback will eventually be called.
===========================================================================*/
boolean gl1_msg_abort_wcdma_ded_srch ( gas_id_t gas_id )
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  MSG_GERAN_HIGH_0_G("G2W: Ded: Abort: gl1_msg_abort_wcdma_ded_srch.");

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  /* called in task context, so lock interrupts */
  GDRV_ISR_LOCK(gas_id);

  if (ded_srch_state[gas_id].srch_in_progress == FALSE)
  {
    MSG_GERAN_ERROR_0_G("G2W: Ded: Abort: No WCDMA measurement already in progress.");
    GDRV_ISR_UNLOCK(gas_id);
    return(FALSE);
  }

  ded_srch_state[gas_id].abort = TRUE;
  l1_sc_wcdma_data_ptr->g2x_aborted = TRUE;

  GDRV_ISR_UNLOCK(gas_id);

  #else /* FEATURE_GSM_TO_WCDMA_MSGR */

  if (!ded_srch_in_progress[gas_id])
  {
    MSG_GERAN_ERROR_0_G("G2W: Ded: Abort: No WCDMA measurement already in progress.");
    return(FALSE);
  }

  /* called in task context, so lock interrupts */
  GDRV_ISR_LOCK(gas_id);
  /* set boolean to signal frame handler */
  if (ded_srch_frame_tick_active[gas_id])
  {
    /* set flag to let wcdma_ded_search_handler abort the search */
    abort_ded_srch[gas_id] = TRUE;
  }
  else
  {
     /* Tell the driver to abort */
     gl1_msg_wcdmadrv_abort_search(gas_id);
  }
  GDRV_ISR_UNLOCK(gas_id);
  #endif

#endif /* FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW */
#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */

  return(TRUE);
}




/*===========================================================================

FUNCTION gl1_msg_abort_wcdma_identify

DESCRIPTION
  This functions aborts currently scheduled wcdma search.

DEPENDENCIES
  gl1_msg_wcdma_identify must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  Abort callback will eventually be called.
===========================================================================*/
boolean  gl1_msg_abort_wcdma_identify ( gas_id_t gas_id )
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW

  MSG_GERAN_HIGH_0_G("G2W: Idle ID: gl1_msg_abort_wcdma_identify.");

  if (!id_srch_in_progress[gas_id])
  {
    MSG_GERAN_ERROR_0_G("G2W: Idle ID: No WCDMA measurement in progress.");
    return  FALSE;
  }

  /* set boolean to signal frame handler */
  abort_id_srch[gas_id] = TRUE;

  return TRUE;

#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
}


/*===========================================================================

FUNCTION gl1_msg_get_id_srch_in_progress

DESCRIPTION
  This functions gets the value of the id_srch_in_progress[gas_id] flag

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_get_id_srch_in_progress ( gas_id_t gas_id )
{
 #ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
 return (id_srch_in_progress[gas_id]);
 #else
 return ( FALSE );
 #endif
}


/*===========================================================================

FUNCTION gl1_msg_abort_wcdma_reconfirm

DESCRIPTION
  This functions aborts currently scheduled wcdma search.

DEPENDENCIES
  gl1_msg_wcdma_reconfirm must have been called at some time prior.

RETURN VALUE
  boolean

SIDE EFFECTS
  Abort callback will eventually be called.
===========================================================================*/
boolean gl1_msg_abort_wcdma_reconfirm ( gas_id_t gas_id )
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW

  MSG_GERAN_HIGH_0_G("G2W: Idle Reconf: gl1_msg_abort_wcdma_reconfirm.");

  if (!recon_srch_in_progress[gas_id])
  {
    MSG_GERAN_ERROR_0_G("G2W: Idle Reconf: No WCDMA measurement in progress.");
    return FALSE;
  }

  /* set boolean to signal frame handler */
  abort_recon_search[gas_id] = TRUE;
  return TRUE;
#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */

}

#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW



/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/*===========================================================================

FUNCTION  gl1_process_x2w_idle_id_search_result

DESCRIPTION
 process g2w idle id search result

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_process_x2w_idle_id_search_result(
  /* ID cell results */
  wl1_x2w_idle_id_result_struct_type *idle_id_result,
  /* frequency that was searched */
  uint16 uarfcn,
  /* the received signal strength indicator */
  int16 wcdma_rssi,
  /*If gap was aborted by WCDMA*/
  boolean gap_aborted,
  /* subscription (for multisim) */
  gas_id_t gas_id)
{
  int16 idx;

  MSG_GERAN_HIGH_4_G("G2W: Idle ID: Received %d ID cells from WL1 on UARFCN %d RSSI %d W aborted gap %d",
                     idle_id_result->num_cells,
                     uarfcn,
                     wcdma_rssi,
                     gap_aborted);

  l1_sc_irat_inc_dec_g2x_abort_count(gap_aborted, gas_id);
  
  if(0 == idle_id_result->num_cells && !gap_aborted)
  {
   wcdmadrv_valid_results[gas_id] = FALSE;
   wcdmadrv_search_done[gas_id] = TRUE;
   return;
  }

  if(curr_id_rpt_ptr[gas_id] == NULL)
  {
    MSG_GERAN_HIGH_0_G("G2W: Idle ID: No current rpt pointer. Ignoring results.");
  }
  else
  {
    curr_id_rpt_ptr[gas_id]->UARFCN = uarfcn;
    curr_id_rpt_ptr[gas_id]->w_aborted_gap = gap_aborted;

    if(idle_id_result->num_cells > MAX_WCDMA_CELLS_PER_UARFCN)
    {
      curr_id_rpt_ptr[gas_id]->num_cell_measurements = MAX_WCDMA_CELLS_PER_UARFCN;
      MSG_GERAN_ERROR_1_G("G2W: Idle ID: Too many results! Only saving %d ID cells.",
                          MAX_WCDMA_CELLS_PER_UARFCN);
    }
    else
    {
      curr_id_rpt_ptr[gas_id]->num_cell_measurements = idle_id_result->num_cells;
    }

    for(idx = 0; idx < curr_id_rpt_ptr[gas_id]->num_cell_measurements; idx++)
    {
      curr_id_rpt_ptr[gas_id]->meas[idx].cell.scr_code = idle_id_result->cells[idx].psc;
      curr_id_rpt_ptr[gas_id]->meas[idx].cell.PN_pos = idle_id_result->cells[idx].pos;
      curr_id_rpt_ptr[gas_id]->meas[idx].RSCP = idle_id_result->cells[idx].rscp;
      curr_id_rpt_ptr[gas_id]->meas[idx].EcNo = idle_id_result->cells[idx].ecio;
      curr_id_rpt_ptr[gas_id]->meas[idx].RSSI = wcdma_rssi;
    }
  }

  wcdmadrv_valid_results[gas_id] = TRUE;
  wcdmadrv_search_done[gas_id] = TRUE;
}
#else /* FEATURE_GSM_TO_WCDMA_MSGR */
/*===========================================================================

FUNCTION wcdmadrv_step3_search_cb

DESCRIPTION
  This function is the callback function used by the WCDMA searcher
  driver to return results. This is called in the MDSP SRCH_DONE
  interrupt context.

DEPENDENCIES
  gl1_wcdmadrv_cfg_init is called to pass this function address to
  the driver.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void wcdmadrv_step3_search_cb(
  wl1_x2w_srch_pn_after_nastt_results_type *pn_after_nastt_ptr,
  gas_id_t gas_id
  )
{
  int16 idx;
  uint32 energy;
  wl1_x2w_srch_g2w_eng_parm_struct_type tmp_cell_ptr;

#if defined(DEBUG_SYSTEMS_RESELECTION_GTOW)
  int16  wcdma_rssi;
#endif

  /* If search stopped because no peaks found */
  if (pn_after_nastt_ptr == NULL)
  {
     wcdmadrv_valid_results[gas_id] = FALSE;
     wcdmadrv_search_done[gas_id] = TRUE;
     return;
  }

  if (curr_id_rpt_ptr[gas_id] == NULL)
  {
    MSG_GERAN_HIGH_0_G("Step 3 search callback with no current rpt pointer");
  }
  else
  {

    MSG_GERAN_HIGH_0_G("Step 3 search callback");
    curr_id_rpt_ptr[gas_id]->UARFCN = pn_after_nastt_ptr->frequency;
    curr_id_rpt_ptr[gas_id]->num_cell_measurements = pn_after_nastt_ptr->num_peaks;
    for (idx=0; idx<pn_after_nastt_ptr->num_peaks; idx++)
    {
      curr_id_rpt_ptr[gas_id]->meas[idx].cell.scr_code =
        pn_after_nastt_ptr->srch_pn_pilots[idx].scr_code;
      curr_id_rpt_ptr[gas_id]->meas[idx].cell.PN_pos =
        pn_after_nastt_ptr->srch_pn_pilots[idx].max_pos;
      energy = pn_after_nastt_ptr->srch_pn_pilots[idx].max_eng;


      /* Obtain RSCP, EcNo and RSSI values   */
      tmp_cell_ptr.cell_eng = energy;
      wl1_x2w_srch_get_ecio_rscp_rssi(&tmp_cell_ptr,gas_id);

      curr_id_rpt_ptr[gas_id]->meas[idx].RSCP = tmp_cell_ptr.rscp;
      curr_id_rpt_ptr[gas_id]->meas[idx].EcNo = tmp_cell_ptr.ecio;
      curr_id_rpt_ptr[gas_id]->meas[idx].RSSI = tmp_cell_ptr.rx_agc;
    }
  }

#ifdef DEBUG_SYSTEMS_RESELECTION_GTOW
  wcdma_rssi = wl1_x2w_srch_mdsp_get_rssi(gas_id);

  MSG_GERAN_HIGH_1_G("WCDMA RSSI %d dBm", wcdma_rssi);
#endif

  wcdmadrv_valid_results[gas_id] = TRUE;
  wcdmadrv_search_done[gas_id] = TRUE;
}
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/*===========================================================================

FUNCTION  gl1_process_x2w_idle_reconfirm_search_result

DESCRIPTION
 process g2w idle re confirmation search result

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_process_x2w_idle_reconfirm_search_result(
  /* reconf cell results */
  wl1_x2w_idle_reconf_result_struct_type *idle_reconf_result,
  /* the wcdma frequency searched */
  uint16 uarfcn,
  /* the received signal strength indicator */
  int16 wcdma_rssi,
  /*If gap was aborted by WCDMA*/
  boolean gap_aborted,
  /* subscription (for multisim) */
  gas_id_t gas_id)
{
  int16 idx;

  MSG_GERAN_HIGH_4_G("G2W: Idle Reconf: Received %d reconf cells from WL1 on UARFCN %d RSSI %d Gap aborted by W %d",
                     idle_reconf_result->num_cells,
                     uarfcn,
                     wcdma_rssi,
                     gap_aborted);

  l1_sc_irat_inc_dec_g2x_abort_count(gap_aborted, gas_id);

  if(curr_recon_rpt_ptr[gas_id] == NULL)
  {
    MSG_GERAN_HIGH_0_G("G2W: Idle Reconf: no current rpt pointer. ignoring results.");
    wcdmadrv_valid_results[gas_id] = FALSE;
    wcdmadrv_search_done[gas_id] = TRUE;
    return;
  }

  curr_recon_rpt_ptr[gas_id]->UARFCN = uarfcn;
  curr_recon_rpt_ptr[gas_id]->w_aborted_gap = gap_aborted;

  if(idle_reconf_result->num_cells > MAX_WCDMA_CELLS_PER_UARFCN)
  {
    curr_recon_rpt_ptr[gas_id]->num_cell_measurements = MAX_WCDMA_CELLS_PER_UARFCN;
    MSG_GERAN_ERROR_1_G("G2W: Idle ID: Too many results! Only saving %d ID cells.",
                        MAX_WCDMA_CELLS_PER_UARFCN);
  }
  else
  {
    curr_recon_rpt_ptr[gas_id]->num_cell_measurements = idle_reconf_result->num_cells;
  }

  for(idx = 0; idx < curr_recon_rpt_ptr[gas_id]->num_cell_measurements; idx++)
  {
    curr_recon_rpt_ptr[gas_id]->meas[idx].cell.scr_code = idle_reconf_result->cells[idx].psc;
    curr_recon_rpt_ptr[gas_id]->meas[idx].cell.PN_pos = idle_reconf_result->cells[idx].pos;
    curr_recon_rpt_ptr[gas_id]->meas[idx].RSCP = idle_reconf_result->cells[idx].rscp;
    curr_recon_rpt_ptr[gas_id]->meas[idx].EcNo = idle_reconf_result->cells[idx].ecio;
    curr_recon_rpt_ptr[gas_id]->meas[idx].RSSI = wcdma_rssi;
  }

  wcdmadrv_valid_results[gas_id] = TRUE;
  wcdmadrv_search_done[gas_id] = TRUE;
}

#else /* FEATURE_GSM_TO_WCDMA_MSGR */

/*===========================================================================

FUNCTION wcdmadrv_list_search_cb

DESCRIPTION
  This function is the callback function used by the WCDMA searcher
  driver to return results. This is called in the MDSP SRCH_DONE
  interrupt context.

DEPENDENCIES
  gl1_wcdmadrv_cfg_init is called to pass this function address to
  the driver.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void wcdmadrv_list_srch_cb(
  wl1_x2w_srch_list_srch_struct_type  *list_ptr,
  gas_id_t gas_id)
{
  int16 idx;
  uint32 energy;
  wl1_x2w_srch_g2w_eng_parm_struct_type tmp_cell_ptr;

#if defined(DEBUG_SYSTEMS_RESELECTION_GTOW)
  int16  wcdma_rssi;
#endif

  if (curr_recon_rpt_ptr[gas_id] == NULL)
  {
    MSG_GERAN_HIGH_0_G("List search callback with no current rpt pointer");
    return;
  }

  MSG_GERAN_HIGH_0_G("List search callback");
  curr_recon_rpt_ptr[gas_id]->UARFCN = list_ptr->frequency;
  curr_recon_rpt_ptr[gas_id]->num_cell_measurements = list_ptr->num_tasks;
  for (idx=0; idx<list_ptr->num_tasks; idx++)
  {
    curr_recon_rpt_ptr[gas_id]->meas[idx].cell.scr_code =
      list_ptr->cell[idx].scr_code;
    curr_recon_rpt_ptr[gas_id]->meas[idx].cell.PN_pos =
      list_ptr->cell[idx].max_pos;
    energy = list_ptr->cell[idx].max_eng;

    /* Obtain RSCP, EcNo and RSSI values   */
    tmp_cell_ptr.cell_eng = energy;
    wl1_x2w_srch_get_ecio_rscp_rssi(&tmp_cell_ptr,gas_id);

    curr_recon_rpt_ptr[gas_id]->meas[idx].RSCP = tmp_cell_ptr.rscp;
    curr_recon_rpt_ptr[gas_id]->meas[idx].EcNo = tmp_cell_ptr.ecio;
    curr_recon_rpt_ptr[gas_id]->meas[idx].RSSI = tmp_cell_ptr.rx_agc;
  }
  wcdmadrv_valid_results[gas_id] = TRUE;
  wcdmadrv_search_done[gas_id] = TRUE;

#ifdef DEBUG_SYSTEMS_RESELECTION_GTOW
  wcdma_rssi = wl1_x2w_srch_mdsp_get_rssi(gas_id);

  MSG_GERAN_HIGH_1_G("WCDMA RSSI %d dBm", wcdma_rssi);
#endif
}

#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/*===========================================================================

FUNCTION  gl1_process_x2w_dedicated_search_result

DESCRIPTION
 process g2w dedicated id and re confirmation search result

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_process_x2w_dedicated_search_result(
  /* dedicated id AND reconf cell results */
  wl1_x2w_dedicated_result_struct_type *dedicated_result,
  /* the wcdma frequency searched */
  uint16 uarfcn,
  /* the received signal strength indicator */
  int16 wcdma_rssi,
  /*If gap was aborted by WCDMA*/
  boolean gap_aborted,
  /* subscription (for multisim) */
  gas_id_t gas_id)
{
  uint16 idx = 0;

  if(!dedicated_result)
  {

#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("NULL ptr passed into function", 0, 0, 0);
  }

  MSG_GERAN_HIGH_5_G("G2W: Ded: received %d ID, %d reconf cells from WL1 on UARFCN %d RSSI %d W aborted gap %d",
                     dedicated_result->num_id_cells,
                     dedicated_result->num_reconf_cells,
                     uarfcn,
                     wcdma_rssi,
                     gap_aborted);

  l1_sc_irat_inc_dec_g2x_abort_count(gap_aborted, gas_id);

  if(dedicated_result->num_id_cells > 0)
  {
    if(curr_ded_rpt_ptr[gas_id] == NULL)
    {
      MSG_GERAN_HIGH_0_G("G2W: Ded: No current ID rpt pointer. Ignoring ID results.");
    }
    else
    {
      if(dedicated_result->num_id_cells > MAX_WCDMA_CELLS_PER_DED_SRCH)
      {
        MSG_GERAN_ERROR_1_G("G2W: Ded: Too many ID results! Only saving %d cells.",
                            MAX_WCDMA_CELLS_PER_DED_SRCH);
      }

      /* would be nice to call upper layers once with all results, but that will require another code cleanup */
      for(idx = 0; idx < dedicated_result->num_id_cells && idx < MAX_WCDMA_CELLS_PER_DED_SRCH; idx++)
      {
        curr_ded_rpt_ptr[gas_id]->UARFCN   = uarfcn;
        curr_ded_rpt_ptr[gas_id]->scr_code = dedicated_result->id_cells[idx].psc;
        /* Store PN pos as cx8, so convert from given cx1 */
        curr_ded_rpt_ptr[gas_id]->PN_pos   = dedicated_result->id_cells[idx].pos * 8;
        curr_ded_rpt_ptr[gas_id]->RSCP     = dedicated_result->id_cells[idx].rscp;
        curr_ded_rpt_ptr[gas_id]->EcNo     = dedicated_result->id_cells[idx].ecio;
        curr_ded_rpt_ptr[gas_id]->tot_eng  = dedicated_result->id_cells[idx].eng;
        curr_ded_rpt_ptr[gas_id]->rssi     = wcdma_rssi;

        /* again, DedID search is weird. upper layers expect this called once per cell */
        if(wcdma_ded_srch_report_cb[gas_id])
        {
          wcdma_ded_srch_report_cb[gas_id](curr_ded_rpt_ptr[gas_id], gas_id);
        }
        else
        {
          MSG_GERAN_ERROR_0_G("G2W: Ded: No callback for ID search results");
        }
      }

      curr_ded_rpt_ptr[gas_id] = NULL;
    }
  }

  if(dedicated_result->num_reconf_cells > 0)
  {
    if(curr_ded_reconf_rpt_ptr[gas_id] == NULL)
    {
      MSG_GERAN_HIGH_0_G("G2W: Ded: No current Reconf rpt pointer. Ignoring Reconf results.");
    }
    else
    {
      curr_ded_reconf_rpt_ptr[gas_id]->UARFCN = uarfcn;

      if(dedicated_result->num_reconf_cells > MAX_WCDMA_CELLS_PER_DED_RECONF)
      {
        curr_ded_reconf_rpt_ptr[gas_id]->num_cell_measurements = MAX_WCDMA_CELLS_PER_DED_RECONF;
        MSG_GERAN_ERROR_1_G("G2W: Ded: Too many reconf results! Only saving %d cells.",
                            MAX_WCDMA_CELLS_PER_DED_RECONF);
      }
      else
      {
        curr_ded_reconf_rpt_ptr[gas_id]->num_cell_measurements = dedicated_result->num_reconf_cells;
      }

      for(idx=0; idx<curr_ded_reconf_rpt_ptr[gas_id]->num_cell_measurements; idx++)
      {
        curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].scr_code = dedicated_result->reconf_cells[idx].psc;
        curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].PN_pos   = dedicated_result->reconf_cells[idx].pos;
        curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].RSCP     = dedicated_result->reconf_cells[idx].rscp;
        curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].EcNo     = dedicated_result->reconf_cells[idx].ecio;
        curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].rssi     = wcdma_rssi;
      }

      /* invoke the L1 callback */
      if(wcdma_ded_srch_reconf_cb[gas_id])
      {
        wcdma_ded_srch_reconf_cb[gas_id](curr_ded_reconf_rpt_ptr[gas_id],gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_0_G("G2W: Ded: No callback for reconfirmation search results");
      }

      /* reset the report ptr to NULL now that it's been sent */
      curr_ded_reconf_rpt_ptr[gas_id] = NULL;
    }
  }

  ded_srch_state[gas_id].returned_result = TRUE;
  ded_srch_state[gas_id].w_aborted_gap = gap_aborted ;
}

#else /* FEATURE_GSM_TO_WCDMA_MSGR */

/*===========================================================================

FUNCTION wcdmadrv_ded_srch_cb

DESCRIPTION
  This function is the callback function used by the WCDMA searcher
  driver to return results. This is called in the MDSP SRCH_DONE
  interrupt context.  (results from Stage 2 processing)

DEPENDENCIES
  wl1_x2w_srch_wcdma_ded_mode_cfg_init is called to pass this function
  address to the driver.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void wcdmadrv_ded_srch_cb(
  wl1_x2w_srch_g2w_ded_mode_list_srch_struct_type *list_ptr,
  gas_id_t gas_id)
{
  /* This function is wrapped with INTLOCK to avoid curr_ded_rpt_ptr[gas_id]
   * being set to NULL resulting in a DATA ABORT when accessing
   * curr_ded_rpt_ptr[gas_id] after checking it against NULL
   * This function consumes 65 GSM symbols, so INTLOCK should be OK
   */
  GDRV_ISR_LOCK(gas_id);

  if (curr_ded_rpt_ptr[gas_id] == NULL)         // ptr set in gl1_msg_wcdma_ded_srch()
  {
      MSG_GERAN_ERROR_0_G("Dedicated search callback with no pointer");
  }
  else
  {
      MSG_GERAN_HIGH_0_G("Dedicated search callback");
      // wcdma_diversity_type diversity;  /* cell diversity */
      curr_ded_rpt_ptr[gas_id]->UARFCN = list_ptr->frequency;
      curr_ded_rpt_ptr[gas_id]->scr_code = list_ptr->scr_code;
      /* Store PN pos as cx8, so convert from cx1 */
      curr_ded_rpt_ptr[gas_id]->PN_pos = (list_ptr->pn_pos*8);
      curr_ded_rpt_ptr[gas_id]->RSCP = list_ptr->cell_rscp;
      curr_ded_rpt_ptr[gas_id]->EcNo = list_ptr->cell_ecno;
      curr_ded_rpt_ptr[gas_id]->tot_eng = list_ptr->tot_eng;
      curr_ded_rpt_ptr[gas_id]->rssi = list_ptr->rssi;


#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
      if(wcdma_ded_srch_report_cb[gas_id])
      {
        wcdma_ded_srch_report_cb[gas_id](curr_ded_rpt_ptr[gas_id],gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_0_G("No callback for wcdma ded search ...");
      }
#endif

      if (--remaining_ded_cells[gas_id] == 0)
      {
        curr_ded_rpt_ptr[gas_id] = NULL;
      }

      /* if there were no reconfirmation scheduled then turn off clocks now */
      if ( (curr_ded_reconf_rpt_ptr[gas_id] == NULL) && (remaining_ded_cells[gas_id]==0) )
      {
        ded_srch_in_progress[gas_id] = FALSE;
      }
  }

  GDRV_ISR_UNLOCK(gas_id);
}


/*===========================================================================

FUNCTION wcdmadrv_ded_reconf_cb

DESCRIPTION
  This function is the callback function used by the WCDMA searcher
  driver to return results after a reconfirmation in dedicated mode.

DEPENDENCIES
  gl1_msg_cfg_wcdma_meas_ded is called to pass this function address to
  the driver.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void wcdmadrv_ded_reconf_cb(
  wl1_x2w_srch_list_srch_struct_type  *list_ptr,
  gas_id_t gas_id)
{
  int16  idx;
  uint32 energy;
  uint16 num_tasks;

  wl1_x2w_srch_g2w_eng_parm_struct_type tmp_cell_ptr;

  if (curr_ded_reconf_rpt_ptr[gas_id] == NULL)
  {
    MSG_GERAN_ERROR_0_G("Reconfirmation callback missing rpt pointer");
    return;
  }

  MSG_GERAN_HIGH_3_G("Reconfirmation callback. freq:%d num_tasks:%d task_limit:%d",
           list_ptr->frequency, list_ptr->num_tasks, MAX_WCDMA_CELLS_PER_DED_RECONF);

  /* Save off the reconfirmed cells to pass back to the driver again */


  curr_ded_reconf_rpt_ptr[gas_id]->UARFCN                = list_ptr->frequency;
  curr_ded_reconf_rpt_ptr[gas_id]->num_cell_measurements = list_ptr->num_tasks;

  /* make sure we don't go over the limit */
  num_tasks = (list_ptr->num_tasks > MAX_WCDMA_CELLS_PER_DED_RECONF)
                               ? MAX_WCDMA_CELLS_PER_DED_RECONF : list_ptr->num_tasks;

  for (idx=0; idx<num_tasks; idx++)
  {
    curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].scr_code = list_ptr->cell[idx].scr_code;
    curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].PN_pos   = list_ptr->cell[idx].max_pos;
    energy                                      = list_ptr->cell[idx].max_eng;
    /* Obtain RSCP, EcNo and RSSI values   */
    tmp_cell_ptr.cell_eng = energy;
    wl1_x2w_srch_get_ecio_rscp_rssi(&tmp_cell_ptr,gas_id);

    curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].RSCP     = tmp_cell_ptr.rscp;
    curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].EcNo     = tmp_cell_ptr.ecio;
    curr_ded_reconf_rpt_ptr[gas_id]->meas[idx].rssi     = tmp_cell_ptr.rx_agc;
  }

#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
  /* invoke the L1 callback */
  if(wcdma_ded_srch_reconf_cb[gas_id])
  {
    wcdma_ded_srch_reconf_cb[gas_id](curr_ded_reconf_rpt_ptr[gas_id],gas_id);
  }
  else
  {
    MSG_GERAN_ERROR_0_G("No callback for wcdma ded reconfirmation");
  }
#endif

  /* done with reconfirmations */
  curr_ded_reconf_rpt_ptr[gas_id] = NULL;

  ded_srch_in_progress[gas_id] = FALSE;

}

#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
#endif /* FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW */

/*===========================================================================

FUNCTION wcdmadrv_srch_abort_cb

DESCRIPTION
  This function is the callback function used by the WCDMA searcher
  driver to confirm a search abort.

DEPENDENCIES
  gl1_wcdmadrv_cfg_init is called to pass this function address to
  the driver.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void wcdmadrv_srch_abort_cb(gas_id_t gas_id)
{
  MSG_GERAN_HIGH_0_G("G2W: Search aborted");



  #ifndef FEATURE_GSM_TO_WCDMA_MSGR
  wcdmadrv_search_aborted[gas_id] = G2WDRV_SRCH_ABORT_COMPLETE;
  #endif

  #ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  if (ded_srch_state[gas_id].srch_in_progress)
  {
    ded_srch_state[gas_id].returned_result = TRUE;
  }

  #else /* FEATURE_GSM_TO_WCDMA_MSGR */

  if (ded_srch_in_progress[gas_id])
  {
    /* if this is a dedicated search aborting, the frame tick handler
       has already exited, so call the abort callback from here */
    if(wcdma_ded_srch_abort_cb[gas_id])
    {
      wcdma_ded_srch_abort_cb[gas_id](gas_id);
    }
    else
    {
      MSG_GERAN_ERROR_0_G("G2W: Ded: No abort callback for search ...");
    }
    ded_srch_in_progress[gas_id] = FALSE;
    curr_ded_rpt_ptr[gas_id] = NULL;
  }
  #endif /* FEATURE_GSM_TO_WCDMA_MSGR */
  #endif /* FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW */


}

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/*===========================================================================

FUNCTION  gl1_msg_g2w_irat_abort_cnf

DESCRIPTION
 process g2w msgr abort message

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msg_g2w_irat_abort_cnf(
  wl1_irat_x2w_abort_cnf_struct_type *abort_cnf,
  gas_id_t gas_id)
{
  MSG_GERAN_HIGH_0_G("G2W: Abort: Received ABORT_CNF from wl1");

  gl1_irat_mutex_lock(gas_id);

  if(G2W_DRIVER_ABORT_PENDING != gl1_msg_g2w_driver_state[gas_id])
  {
    MSG_GERAN_HIGH_1_G("G2W: Abort: Received ABORT_CNF when abort wasn't pending (g2w driver state %d)",
                        gl1_msg_g2w_driver_state[gas_id]);
  }
  else
  {
    gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_READY;

    wcdmadrv_srch_abort_cb(gas_id);
  }

  gl1_irat_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION  gl1_msg_g2w_irat_rf_exit_cnf

DESCRIPTION
 process g2w msgr rf_exit_cnf message

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msg_g2w_irat_rf_exit_cnf(
  gas_id_t gas_id)
{
  MSG_GERAN_HIGH_0_G("G2W: Received RF_EXIT_CNF from wl1");

  gl1_irat_mutex_lock(gas_id);

  if(G2W_DRIVER_IDLE_ID_PENDING != gl1_msg_g2w_driver_state[gas_id] && 
     G2W_DRIVER_IDLE_RECONF_PENDING != gl1_msg_g2w_driver_state[gas_id] &&
     G2W_DRIVER_DEDICATED_PENDING != gl1_msg_g2w_driver_state[gas_id] &&
     G2W_DRIVER_ABORT_PENDING != gl1_msg_g2w_driver_state[gas_id] &&
     G2W_DRIVER_READY != gl1_msg_g2w_driver_state[gas_id])
  {
    MSG_GERAN_FATAL_1_G("G2W: Received RF_EXIT_CNF in unexpected g2w driver state %d",
                         gl1_msg_g2w_driver_state[gas_id]);
  }
  else
  {
    gl1_hw_g2w_set_done_with_rf(gas_id);
  }

  gl1_irat_mutex_unlock(gas_id);
}

#else /* FEATURE_GSM_TO_WCDMA_MSGR */
/*===========================================================================

FUNCTION wcdmadrv_wfw_status

DESCRIPTION
  This function is the callback function used by the WCDMA searcher
  driver to pass the status of WCDMA FW APP.

DEPENDENCIES
  wl1_x2w_srch_wcdma_cfg_init and gl1_msg_cfg_wcdma_meas_ded is called to pass
  this function address to  the driver.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void wcdmadrv_wfw_status(
  wl1_x2w_srch_wfw_status_enum_type wfw_status,
  gas_id_t gas_id)
{
  /* Set WCDMA FW APP status to value returned in G2W CB */
  g2w_wfw_status[gas_id] = wfw_status;

  if (wfw_status == WL1_X2W_WFW_DISABLED)
  {
    /*Wake-up from sleep and set active task count */
    gl1_drx_require_next_tick(gas_id);
    gl1_drx_task_active(gas_id);

    GL1_ISR_LOCK(gas_id);
    if (l1_sc_get_wcdma_update_list_irat_active(gas_id) == FALSE)
    {
      /* If going inactive or BPLMN suspend/resume do not turn off WSTMR as need to be continuous for WL1 timing */
      if ((l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE)&&(l1_get_background_wcdma_plmn_scan_active(gas_id) == FALSE))
      {
        if(!gl1_deep_sleep_pending[gas_id])
        {
         gl1_hw_mcpm_neighbor_update(MCPM_G2W_MEAS_STOP,gas_id);
        }
        gl1_deep_sleep_pending[gas_id] = FALSE;
      }
      else /*For inactive state, check if STOP needs to be sent to MCPM*/
      if(l1_tsk_buffer[gas_id].l1_state == L1_INACTIVE_MODE)
      {
        if(gl1_deep_sleep_pending[gas_id] == TRUE)
        {
          gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_STOP_REQ, GL1_MCPM_GERAN_STOP,gas_id);
          gstmr_force_reinit(gas_id);
          MSG_GERAN_HIGH_0_G("G2L: MCPM_GERAN_STOP_REQ sent");
         }
         gl1_deep_sleep_pending[gas_id] = FALSE;
      }
      else /*Decrement active task count */
      {
         gl1_drx_task_inactive(gas_id);
      }
      l1_sc_set_wcdma_irat_mcpm_active(FALSE,gas_id);
    }
    else /*Decrement active task count */
    {
      gl1_drx_task_inactive(gas_id);
    }
    GL1_ISR_UNLOCK(gas_id);
  }
}
#endif /* ! FEATURE_GSM_TO_WCDMA_MSGR */

/*===========================================================================

FUNCTION g2w_is_wfw_active

DESCRIPTION
  Returns whether FW_APP_WCDMA is currently enabled from Wl1 via any G2W
  transactions. This will return TRUE any time gl1 has sent a SEARCH_REQ
  message and G2W drivers hasn't sent back an ABORT_CNF or a SEARCH_DONE
  message yet.  It also will return TRUE when WL1 is busy with INIT command,
  since they enable WFW while processing that command.

RETURN VALUE
  TRUE, if wfw is enabled (non suspended or disabled)
  FALSE, otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean g2w_is_wfw_active(gas_id_t gas_id)
{
#ifdef FEATURE_GSM_TO_WCDMA_MSGR

  if (gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_INIT_PENDING ||
      gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_SRCH_REQ_PENDING ||
      gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_IDLE_ID_PENDING ||
      gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_IDLE_RECONF_PENDING ||
      gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_DEDICATED_PENDING ||
      gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_ABORT_PENDING ||
      id_srch_in_progress[gas_id] ||
      recon_srch_in_progress[gas_id] ||
      ded_srch_state[gas_id].srch_in_progress)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }

#else /* FEATURE_GSM_TO_WCDMA_MSGR */

  if (g2w_wfw_status[gas_id] == WL1_X2W_WFW_ENABLED)
  {
    return (TRUE);
  }
  else
  {
    return (FALSE);
  }

#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}

/*===========================================================================

FUNCTION wcdma_id_handler

DESCRIPTION
  This function should be called every frame.  This handler is installed as
  part of gl1_msg_wcdma_identify().  The handler does not care about
  block boundaries.  gl1_msg_wcdma_identify must be called one frame
  before the id search should start.

  When the handler senses that there are no more work to do,
  it will signal that it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if wcdma id search is complete
  FALSE if wcdma id search is still in progress

SIDE EFFECTS
  None
===========================================================================*/
static boolean wcdma_id_handler
(
   boolean start_of_block, /* next frame is paging block */
   uint32  time_stamp,     /* currently not used          */
   gas_id_t gas_id
)
{
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  boolean        cleanup_handler                               = FALSE;
  static boolean g2x_startup_issued[NUM_GERAN_DATA_SPACES]     = { INITIAL_VALUE( FALSE ) };
  static boolean id_rf_exit_procedure_active[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
  static boolean id_rf_exit_req_sent[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };


  gas_id = check_gas_id(gas_id);

  /* This is put in to help debug when lower layers get stuck */
  if(identify_frame_count[gas_id] >= IDENTIFY_TIMEOUT)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("G2W: Idle ID frame timeout reached. Crashing for debug.", 0, 0, 0);
  }

  /* Always send rf_exit_req msg now if we set this flag in the last frame */
  if(id_rf_exit_procedure_active[gas_id])
  {

    /* Do not un-install the handler until WL1 has confirmed it has completed RF exit. 
     * When handler is uninstalled then TRM is released allowing other SUB to call
     * rf_enter_mode causing a race condition if same device is used and we don't wait
     * for the G2W tranaction to completely finish
     */

    MSG_GERAN_HIGH_2_G("G2W: IDLE ID: rf exit proc: using_rf %d, rf_exit_req_sent %d",
                       gl1_hw_g2w_check_if_using_rf(gas_id), id_rf_exit_req_sent[gas_id]);

    /* should happen in 1st frame id_rf_exit_procedure_active is TRUE */
    if(!id_rf_exit_req_sent[gas_id])
    {
      /*When abort happens at the time of frame count 1, there are two possibilities:
           1. WL1 is processing Pre search req
           2. Wl1 has processed pre search req and waiting for search req*/
      if(identify_frame_count[gas_id] == 1)
      {
       gl1_hw_g2w_send_rf_exit_req(TRUE, gas_id);
      }/*For normal cases don't abort*/
      else
      {
       gl1_hw_g2w_send_rf_exit_req(l1_sc_wcdma_data_ptr->g2x_aborted, gas_id);
	   l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;
      }

      g2x_startup_issued[gas_id] = FALSE;
      id_rf_exit_req_sent[gas_id] = TRUE;
    }
    /* should happen in 2nd frame id_rf_exit_procedure_active is TRUE */
    else if (!gl1_hw_g2w_check_if_using_rf(gas_id))
    {
      cleanup_handler = TRUE;
      id_rf_exit_req_sent[gas_id]= FALSE;
      id_rf_exit_procedure_active[gas_id] = FALSE;
    }
  }

  else if(abort_id_srch[gas_id])
  {
    /* Check if search hasn't started yet */
    if(identify_frame_count[gas_id] == -1)
    {
      /* SEARCH_REQ message hasn't been given yet.  Simply de-register this handler */
      cleanup_handler = TRUE;
    }
    /* Check if search has already finished. */
    else if(gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_READY)
    {
      /* issue cleanup command to GFW if necessary */
      if(g2x_startup_issued[gas_id])
      {
         gl1_hw_g2x_cleanup(TRUE, TRUE, gas_id );
		 l1_sc_wcdma_data_ptr->g2x_aborted = TRUE;
         MSG_GERAN_HIGH_0_G("G2W: Idle ID: Abort: GFW Cleanup Sent");
      }

      /* Now GFW will tune back at start of next frame, so send meas_exit_req
         to wl1 next frame. */
      id_rf_exit_procedure_active[gas_id] = TRUE;
    }
    /* In any other case, send abort command if not already sent */
    else if(gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_ABORT_PENDING)
    {
      /* send ABORT message to wl1 driver */
      gl1_msg_wcdmadrv_abort_search(gas_id);
    }
  }
  else
  {
    if(identify_frame_count[gas_id] == -1)
    {
      if(start_of_block)
      {
        /* perform actions needed 2 frames before rf sampling
           (this is the first frame of the gl1 "search block") */
        identify_frame_count[gas_id] = 1;
		
		#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
		gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_START_ID);
		#endif

        MSG_GERAN_HIGH_1_G("G2W: Idle ID: preparing wl1 for UARFCN %d search", idle_id_UARFCN[gas_id]);

        if(gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_READY)
        {
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
          ERR_FATAL("Driver not ready for search.  Bad state: %d", gl1_msg_g2w_driver_state[gas_id], 0, 0);
        }

        /* Boosting the clock for 10 ms */
        if( gl1_hw_qta_gap_active(gas_id) )
        {
          gl1_hw_mcpm_boost_request(L1_IDLE_G2W_CPU,MCPM_CPU_BOOST_MASK, 10000, gas_id);
        }

        /* causes WL1 to call RF with rfm_meas_enter() */
        gl1_hw_g2w_send_rf_enter_ind(idle_id_UARFCN[gas_id], gas_id);

        /* causes WL1 to start WFW and build scripts */
        gl1_msg_g2w_pre_srch_cnf[gas_id] = FALSE;
        gl1_hw_g2w_send_pre_search_req(gas_id);

        gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_SRCH_REQ_PENDING;
      }
      else
      {
        MSG_GERAN_HIGH_0_G("G2W: Idle ID: Waiting for start_of_block");
      }
    }
    else
    {
      identify_frame_count[gas_id]++;
      if(!gl1_msg_g2w_pre_srch_cnf[gas_id])
      {
        identify_frame_count[gas_id] = 1;
        MSG_GERAN_HIGH_0_G("G2W No Pre SRCH CNF, wait for next frame");
      }

      if(identify_frame_count[gas_id] == 2)
      {
        /* perform actions needed 1 frame before rf sampling
           (2nd frame from start of block is one frame before the rf starts) */

        /* suspend power measurements during the G2W search period
           (we'll enable them again at the end) */
        gl1_msg_suspend_pwr_meas(gas_id);

        /* send g2w startup command with startup offset of 0
           (corresponding to start of next G frame) */
        if(!l1_sc_x2g_ta_irat_gap(gas_id))
        {
          gl1_hw_g2x_mdsp_start_wcdma_rf_cmd(0, gas_id);
          g2x_startup_issued[gas_id] = TRUE;
        }
        else
        {
           set_qta_cleanup_pending(FALSE, gas_id);
        }

        gl1_hw_g2w_search(idle_id_UARFCN[gas_id],
                          1, /* the air-gap is in 1 G-frame from now */
                          0, /* QS. gap starts at start of frame */
                          WL1_X2W_IDLE_ID_SRCH,
                          NULL, /* idle id has no cell params */
                          gas_id);

        gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_IDLE_ID_PENDING;

      }
      else
      {
        /* Mutex lock for checking results done */
        gl1_irat_mutex_lock(gas_id);

        if(wcdmadrv_search_done[gas_id])
        {
          wcdmadrv_search_done[gas_id] = FALSE;  /* Need to reset this here to avoid reporting twice */

          MSG_GERAN_HIGH_0_G("G2W: Idle ID: FT handler sees search results");

          if(g2x_startup_issued[gas_id] == TRUE)
          {
            gl1_hw_g2x_cleanup(TRUE, (!curr_id_rpt_ptr[gas_id]->w_aborted_gap), gas_id );
          }
          /* Now GFW will tune back at start of next frame, so send meas_exit_req
             to wl1 next frame. */
          id_rf_exit_procedure_active[gas_id] = TRUE;

          /* send results of search back to SCE */
          if (wcdmadrv_valid_results[gas_id] == TRUE)
          {
            if(wcdma_identify_report_cb[gas_id])
            {
              wcdma_identify_report_cb[gas_id](curr_id_rpt_ptr[gas_id], gas_id);
            }
            else
            {
              MSG_GERAN_HIGH_0_G("G2W: Idle ID: No search-done callback");
            }
          }
          else
          {
            /* For some reason SCE wants an abort callback if ID search finds 0 
               cells (valid_results is FALSE). Setting this to TRUE here will 
               cause wcdma_identify_abort_cb to be called after the rf_exit 
               procedure */
            abort_id_srch[gas_id] = TRUE;
          }
        }
        gl1_irat_mutex_unlock(gas_id);
      }
    }
  }

  if(cleanup_handler)
  {
    id_srch_in_progress[gas_id] = FALSE;

    gl1_msg_resume_pwr_meas(gas_id);

    /* call abort callback if this was an abort */
    if(abort_id_srch[gas_id])
    { 
      abort_id_srch[gas_id] = FALSE;
      if(wcdma_identify_abort_cb[gas_id])
      {
        wcdma_identify_abort_cb[gas_id](gas_id);
      }
    }
    
    /* let upper layer know msg layer is done */
    if(wcdma_srch_handler_done_cb[gas_id])
    {
      wcdma_srch_handler_done_cb[gas_id](gas_id);
    }
    curr_id_rpt_ptr[gas_id]->w_aborted_gap = FALSE;
  }

  return cleanup_handler;


  #else /* FEATURE_GSM_TO_WCDMA_MSGR */

  boolean          remove_handler    = FALSE;
  boolean          force_cleanup     = FALSE;


  static  boolean  delay_one_further_frame = FALSE; /* static_dual_spaced_ignore */

  static  boolean  issue_g2x_cleanup[NUM_GERAN_DATA_SPACES] =
                     { INITIAL_VALUE( FALSE ) };

  if (identify_frame_count[gas_id] == -1)
  {
    /* When frame count = -1, we are waiting for start_of_frame */
    if (abort_id_srch[gas_id])
    {
        if ( issue_g2x_cleanup[gas_id] )
        {
          force_cleanup = TRUE;

          delay_one_further_frame = TRUE;
        }
        else if ( delay_one_further_frame )
        {
          delay_one_further_frame = FALSE;
        }
        else
        {
          if(wcdma_identify_abort_cb[gas_id])
          {
            wcdma_identify_abort_cb[gas_id](gas_id);
          }
          abort_id_srch[gas_id] = FALSE;
          remove_handler = TRUE;
        }
    }
    else if (start_of_block)
    {
      /* this is frame-1 for the transaction. */

      /* start id search */
      identify_frame_count[gas_id] = 1;

      MSG_GERAN_HIGH_1_G("G2W: Enable WCDMA RX for ID search. UARFCN:%d", idle_id_UARFCN[gas_id]);

      /* This will be set TRUE when response from driver arrives */
      wcdmadrv_search_done[gas_id] = FALSE;
      wcdmadrv_valid_results[gas_id] = FALSE;


      gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id] = G2W_CMD_NULL;

      /* This will switch remainder of search start processing to task context. */
      wl1_x2w_srch_g2w_initiate_search(
                          WFW_IRAT_G2W_IDLE_MODE,
                          idle_id_UARFCN[gas_id],
                          0,
                          #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_GSM_COEX)
                          gl1_msg_get_multi_sim_sys_mode(),
                          #endif
                          gas_id);

      gl1_msg_suspend_pwr_meas(gas_id);

      }

    issue_g2x_cleanup[gas_id] = FALSE;

  }
  else
  {
    /* We have an active id search */
    /* Have we successfully aborted, or are we timing out an abort response? */
    /* (id searches are timed out when frame_count == IDENTIFY_TIMEOUT) */
    if ((wcdmadrv_search_aborted[gas_id]== G2WDRV_SRCH_ABORT_COMPLETE) ||
        (identify_frame_count[gas_id] > IDENTIFY_TIMEOUT+5))
    {
      if ( issue_g2x_cleanup[gas_id] )
      {
        force_cleanup           = TRUE;
      }
      else if ( delay_one_further_frame )
      {
        delay_one_further_frame = FALSE;
      }
      else
      {
        wcdmadrv_search_aborted[gas_id] = G2WDRV_SRCH_ABORT_FALSE;
        if (MEAS_PTR_CB_VALID(wcdma_identify_abort_cb[gas_id]) && MEAS_GAS_ID_VALID(gas_id))
        {
          wcdma_identify_abort_cb[gas_id](gas_id);
        }
        else
        {
           MSG_GERAN_HIGH_0_G("No abort callback for wcdma id search");
        }
        MSG_GERAN_HIGH_0_G("G2W Search Done");

        gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id] = G2W_CMD_NULL;
        remove_handler = TRUE;
      }
    }
    /* Have we received search completion notification from driver? */
    else if (wcdmadrv_search_done[gas_id])
    {
      if ( issue_g2x_cleanup[gas_id] )
      {
        force_cleanup           = TRUE;
      }
      else if ( delay_one_further_frame )
      {
        delay_one_further_frame = FALSE;
      }
      else
      {
        wcdmadrv_search_done[gas_id] = FALSE;
        if (wcdmadrv_valid_results[gas_id] == TRUE)
        {
          wcdmadrv_valid_results[gas_id] = FALSE;
          if (MEAS_PTR_CB_VALID(wcdma_identify_report_cb[gas_id]) && MEAS_GAS_ID_VALID(gas_id))
          {
            wcdma_identify_report_cb[gas_id](curr_id_rpt_ptr[gas_id], gas_id);
          }
          else
          {
             MSG_GERAN_HIGH_0_G("No identify callback for wcdma id search GAS");
          }
        }
        else /* If WCDMA results are invalid, inform L1 SC */
        {
          if (MEAS_PTR_CB_VALID(wcdma_identify_abort_cb[gas_id]) && MEAS_GAS_ID_VALID(gas_id))
          {
            wcdma_identify_abort_cb[gas_id](gas_id);
          }
        }
        MSG_GERAN_HIGH_0_G("G2W Search Done");
        gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id] = G2W_CMD_NULL;
        remove_handler = TRUE;
      }
    }

    /* Is PL1 telling us to abort, or are we timing out an id search? */
    else if ((abort_id_srch[gas_id]) ||
        (identify_frame_count[gas_id] == IDENTIFY_TIMEOUT))
    {
      /* Clear this abort notification */
      abort_id_srch[gas_id] = FALSE;

       if (identify_frame_count[gas_id] > IDENTIFY_TIMEOUT)
      {
         MSG_GERAN_HIGH_0_G("Already aborting id search due to timeout");
      }
      else
      {
        /* Tell the driver to abort */
        gl1_msg_wcdmadrv_abort_search(gas_id);
        /* set frame count to be used for timeout of abort response */
        identify_frame_count[gas_id] = IDENTIFY_TIMEOUT;

        if (gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id] != G2W_CMD_NULL)
        {
          MSG_GERAN_HIGH_1_G("Aborting id search, frame count = %d", identify_frame_count[gas_id]);

          if( FALSE != issue_g2x_cleanup[gas_id] )
          {
             force_cleanup = TRUE;
          }
          gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id] = G2W_CMD_NULL;
        }
  else
  {
           MSG_GERAN_HIGH_0_G("Aborting id search before W starts ...");
        }
      }
    }
#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW

    /* @@OG: We might need to increase the WCDMA id timeout
     * by one TDMA frame to accomodate this new mechanism.
     */
    if( (G2W_CMD_WAITING_FOR_TRIGGER == gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id]) && (wcdmadrv_search_aborted[gas_id] == G2WDRV_SRCH_ABORT_FALSE) )
    {
#ifdef FEATURE_WCDMA
      gl1_hw_start_wcdma_rf_cmd( idle_id_UARFCN[gas_id], 0 ,gas_id);
#endif
      gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id] = G2W_CMD_TRIGGERED;

    issue_g2x_cleanup[gas_id] = TRUE;

    }

#endif /*#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW*/
    identify_frame_count[gas_id]++;
  }

  if( force_cleanup )
  {
    if( FALSE != issue_g2x_cleanup[gas_id] )
    {
#ifdef FEATURE_WCDMA
      gl1_hw_g2x_cleanup( TRUE, gas_id );
#endif
      issue_g2x_cleanup[gas_id] = FALSE;
      delay_one_further_frame = TRUE;
    }
  }

  if (remove_handler)
  {
    curr_id_rpt_ptr[gas_id] = NULL;
    id_srch_in_progress[gas_id] = FALSE;
    abort_id_srch[gas_id] = FALSE;
    gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id] = G2W_CMD_NULL;

    /* Re-enable mDSP BB Rx clock gating, disabled for duration of W measurement */
    gl1_hw_enable_bbrx_clock_gating ( TRUE );


    if( FALSE != issue_g2x_cleanup[gas_id] )
    {
#ifdef FEATURE_WCDMA
      gl1_hw_g2x_cleanup( TRUE,gas_id );
#endif
      issue_g2x_cleanup[gas_id] = FALSE;
    }

    gl1_msg_resume_pwr_meas(gas_id);

    delay_one_further_frame = FALSE;


  }

  return (remove_handler);

  #endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}

/*===========================================================================

FUNCTION wcdma_reconfirm_handler

DESCRIPTION
  This function should be called every frame.  This handler is installed as
  part of gl1_msg_wcdma_reconfirm().  The handler does not care about
  block boundaries.  gl1_msg_wcdma_reconfirm must be called one frame
  before the list search should start.

  When the handler senses that there are no more work to do,
  it will signal that it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if wcdma list search is complete
  FALSE if wcdma list search is still in progress

SIDE EFFECTS
  None
===========================================================================*/
static boolean wcdma_reconfirm_handler
(
   boolean start_of_block, /* next frame is pg block */
   uint32  time_stamp,     /* currently not used          */
   gas_id_t gas_id
)
{
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr;
  
  gas_id = check_gas_id(gas_id);
  
  l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /* This is put in to help debug when lower layers get stuck */
  if(reconfirm_frame_count[gas_id] >= RECONFIRM_TIMEOUT)
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("G2W: Idle Reconf: Frame timeout reached. Crashing for debug.", 0, 0, 0);
  }

  if(returned_idle_reconf_result[gas_id] == FALSE)
  {
    if(abort_recon_search[gas_id] && (reconfirm_frame_count[gas_id] == -1))
    {
        /* call abort callback */
        if(wcdma_reconfirm_abort_cb[gas_id])
        {
          wcdma_reconfirm_abort_cb[gas_id](gas_id);
        }

        /* SEARCH_REQ message hasn't been given yet.  Simply de-register this handler */
        returned_idle_reconf_result[gas_id] = TRUE;
    }
    else
    {
      if(reconfirm_frame_count[gas_id] == -1)
      {
        /* Start off our gap frame-by-frame state machine.  Once this becomes
           non-negative it indicates we're interacting with wl1 drivers, and thus
           can't abort the air gap. (We can still abort the search). */
        reconfirm_frame_count[gas_id] = 0;
      }

      /* Mutex lock for checking results done */
      gl1_irat_mutex_lock(gas_id);

      if(wcdmadrv_search_done[gas_id])
      {
        wcdmadrv_search_done[gas_id] = FALSE;  /* Need to reset this here to avoid reporting twice */
        returned_idle_reconf_result[gas_id] = TRUE;

        MSG_GERAN_HIGH_0_G("G2W: Idle Reconf: Received search results from WL1");

        /* send results of search back to SCE */
        if(wcdma_reconfirm_report_cb[gas_id])
        {
          wcdma_reconfirm_report_cb[gas_id](curr_recon_rpt_ptr[gas_id],gas_id);
        }
        else
        {
          MSG_GERAN_HIGH_0_G("G2W: Idle Reconf: No reconf callback. Results dropped.");
        }
      }

      gl1_irat_mutex_unlock(gas_id);
    }
  }

  /* if we involved wl1 drivers in our search */
  if(reconfirm_frame_count[gas_id] >= 0)
  {
    reconfirm_frame_count[gas_id]++;

    if(reconfirm_frame_count[gas_id] == 1)
    {
      /* 1st frame of handler: perform actions needed 2 frames before air gap */
      MSG_GERAN_HIGH_1_G("G2W: Idle Reconf: preparing wl1 for UARFCN %d search", idle_reconf_UARFCN[gas_id]);

      if(gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_READY)
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_FATAL("Driver not ready for search.  Bad state: %d", gl1_msg_g2w_driver_state[gas_id], 0, 0);
      }
	  
	  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
	  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_START_RECONFIRM); 
      #endif

      /* Boosting the clock for 10 ms */
      if( gl1_hw_qta_gap_active(gas_id) )
      {
        gl1_hw_mcpm_boost_request(L1_IDLE_G2W_CPU, MCPM_CPU_BOOST_MASK, 10000, gas_id);
      }

      /* causes WL1 to call RF with rfm_meas_enter() */
      gl1_hw_g2w_send_rf_enter_ind(idle_reconf_UARFCN[gas_id], gas_id);

      /* causes WL1 to start WFW and build scripts */
      gl1_msg_g2w_pre_srch_cnf[gas_id] = FALSE;
      gl1_hw_g2w_send_pre_search_req(gas_id);

      gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_SRCH_REQ_PENDING;
    }
    else if(reconfirm_frame_count[gas_id] == 2)
    {
      if(!gl1_msg_g2w_pre_srch_cnf[gas_id])
      {
        reconfirm_frame_count[gas_id] = 1;
        MSG_GERAN_HIGH_0_G("G2W No Pre SRCH CNF, wait for next frame");
        return FALSE;
      }

      /* 2nd frame of handler: perform actions needed 1 frames before air gap */

      if(!l1_sc_x2g_ta_irat_gap(gas_id))
      {
        /* causes GFW to open up a gap in G timeline (with startup/cleanup) */
        gl1_hw_g2x_mdsp_rf_sample_ram_cmd(curr_start_time[gas_id], gas_id);
      }
      else
      {
        set_qta_cleanup_pending(FALSE, gas_id);
      }
     /* causes WL1 to trigger WFW to search in that gap */
      gl1_hw_g2w_search(idle_reconf_UARFCN[gas_id],
                        1, /* the air-gap is in 1 G-frame from now */
                        curr_start_time[gas_id], /* QS */
                        WL1_X2W_IDLE_RECONF_SRCH,
                        (wl1_x2w_srch_input_union_type *)&idle_reconf_cell_params[gas_id],
                        gas_id);
      gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_IDLE_RECONF_PENDING;

      /* suspend power measurements during the G2W search period
         (we'll enable them next frame) */
      gl1_msg_suspend_pwr_meas(gas_id);

    }
    else if(reconfirm_frame_count[gas_id] == 3)
    {
      /* 3rd frame of handler: perform actions needed during air gap */

      gl1_msg_resume_pwr_meas(gas_id);
    }
    else if(reconfirm_frame_count[gas_id] == 4)
    {
      /* 4th frame of handler: perform actions needed 1 frame after air gap */
      gl1_hw_g2w_send_rf_exit_req(l1_sc_wcdma_data_ptr->g2x_aborted, gas_id);
          l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;
    }
  }

  /* check handler-exit conditions */

  if(/* finished interacting with l1_sc_irat */
     returned_idle_reconf_result[gas_id] &&
     /* finished a gap procedure */        /* never started a gap procedure */
     (reconfirm_frame_count[gas_id] >= 4 || reconfirm_frame_count[gas_id] < 0) &&
     /* rf exit procedure has finished */
     !gl1_hw_g2w_check_if_using_rf(gas_id))
  {
    recon_srch_in_progress[gas_id] = FALSE;
    abort_recon_search[gas_id] = FALSE;

    if(wcdma_srch_handler_done_cb[gas_id])
    {
      wcdma_srch_handler_done_cb[gas_id](gas_id);
    }
    curr_recon_rpt_ptr[gas_id]->w_aborted_gap = FALSE;
    return TRUE;
  }

  return FALSE;

  #else /* FEATURE_GSM_TO_WCDMA_MSGR */

  boolean remove_handler = FALSE;

  if ((wcdmadrv_search_aborted[gas_id] == G2WDRV_SRCH_ABORT_COMPLETE) ||
      (reconfirm_frame_count[gas_id] > RECONFIRM_TIMEOUT+1))
  {
    wcdmadrv_search_aborted[gas_id] = G2WDRV_SRCH_ABORT_FALSE;
    if(wcdma_reconfirm_abort_cb[gas_id])
    {
      wcdma_reconfirm_abort_cb[gas_id](gas_id);
    }
    else
    {
      MSG_GERAN_HIGH_0_G("No abort callback for wcdma list search ...");
    }
    MSG_GERAN_HIGH_0_G("G2W Reconfirm Rx Done");

    remove_handler = TRUE;

  gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_NULL;
  }

  else if ((reconfirm_frame_count[gas_id] > 0) && (wcdmadrv_search_done[gas_id]))
  {
    wcdmadrv_search_done[gas_id] = FALSE;
    if(wcdma_reconfirm_report_cb[gas_id])
    {
      wcdma_reconfirm_report_cb[gas_id](curr_recon_rpt_ptr[gas_id],gas_id);
    }
    else
    {
      MSG_GERAN_HIGH_0_G("No reconfirm callback for wcdma list search ...");
    }
    MSG_GERAN_HIGH_0_G("G2W Reconfirm Search Done");

    remove_handler = TRUE;

    gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_NULL;
  }
  /* Is PL1 telling us to abort? */
  else if (abort_recon_search[gas_id])
  {
    abort_recon_search[gas_id] = FALSE;
    if (reconfirm_frame_count[gas_id] < 1)
    {
      /* haven't issued the reconfirm yet, so just call the abort
       * callback and remove this handler */
      if(wcdma_reconfirm_abort_cb[gas_id])
      {
        wcdma_reconfirm_abort_cb[gas_id](gas_id);
      }
      else
      {
        MSG_GERAN_HIGH_0_G("No abort callback for wcdma list search ...");
      }
      MSG_GERAN_HIGH_0_G("Aborting reconfirm before W starts ...");
      remove_handler = TRUE;
    }
    else
    {
      /* set up frame counter to timeout the abort */
      reconfirm_frame_count[gas_id] = RECONFIRM_TIMEOUT;
      /* Tell the driver to abort */
      gl1_msg_wcdmadrv_abort_search(gas_id);
      MSG_GERAN_HIGH_0_G("Aborting WCDMA reconfirm...");
      gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_NULL;
    }
  }
  /* are we timing out an list search? */
  else if (reconfirm_frame_count[gas_id] == RECONFIRM_TIMEOUT)
  {
    MSG_GERAN_ERROR_0_G("Timeout on WCDMA reconfirm...");
    /* Tell the driver to abort */
    gl1_msg_wcdmadrv_abort_search(gas_id);
  }
  else if (reconfirm_frame_count[gas_id] == 0)
  {
    /* If this is frame-1 for the transaction. */

    MSG_GERAN_HIGH_2_G("G2W: Enable WCDMA RX for reconf search. UARFCN:%d start_time:%d",
             idle_reconf_UARFCN[gas_id], curr_start_time[gas_id]);

    /* pass parameters to driver */
    wl1_x2w_srch_wcdmadrv_program_list_srch(&list_srch_params[gas_id], gas_id);

    gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_NULL;
    /* This will switch remainder of search start processing to task context. */
    wl1_x2w_srch_g2w_initiate_search(
                          WFW_IRAT_G2W_DEDICATED_MODE,
                          idle_reconf_UARFCN[gas_id],
                          curr_start_time[gas_id],
                          #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_GSM_COEX)
                          gl1_msg_get_multi_sim_sys_mode(),
                          #endif
                          gas_id);

    /* Suspend power measures for the next frame */
    gl1_msg_suspend_pwr_meas(gas_id);
  }

  /* If this is the frame of the transaction we can re-enable power measures */
  if (reconfirm_frame_count[gas_id] == 1)
  {
    gl1_msg_resume_pwr_meas(gas_id);
  }

  reconfirm_frame_count[gas_id]++;


  if( (G2W_CMD_WAITING_FOR_TRIGGER == gl1_msg_wcdma_reconf_start_rf_cmd[gas_id]) && (wcdmadrv_search_aborted[gas_id] == G2WDRV_SRCH_ABORT_FALSE) )
  {
#ifdef FEATURE_WCDMA
    gl1_hw_rf_sample_ram_cmd( idle_reconf_UARFCN[gas_id], curr_start_time[gas_id],gas_id );
#endif
    gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_TRIGGERED;
  }

  if (remove_handler)
  {
    curr_recon_rpt_ptr[gas_id] = NULL;
    recon_srch_in_progress[gas_id] = FALSE;
    abort_recon_search[gas_id] = FALSE;
    /* Re-enable mDSP BB Rx clock gating, disabled for duration of W measurement */
    gl1_hw_enable_bbrx_clock_gating ( TRUE );

    gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_NULL;


  }

  return (remove_handler);

  #endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}


void gl1_hw_wcdma_cleanup_reconf_state_at_idle_abort(gas_id_t gas_id)
{
  curr_recon_rpt_ptr[gas_id] = NULL;
  recon_srch_in_progress[gas_id] = FALSE;
  abort_recon_search[gas_id] = FALSE;
#ifndef FEATURE_GSM_TO_WCDMA_MSGR
  gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_NULL;
#endif

  gl1_msgi_remove_ft_handler(wcdma_reconfirm_handler,gas_id);

  return;
}

#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */

/*===========================================================================

FUNCTION wcdma_ded_srch_handler

DESCRIPTION
  This function should be called every frame.  This handler is installed as
  part of gl1_msg_wcdma_ded_srch().  The handler does not care about
  block boundaries.  gl1_msg_wcdma_ded_srch must be called one frame
  before the list search should start.

  When the handler senses that there are no more work to do,
  it will signal that it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if message reception is complete
  FALSE if messages are still being received

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
static boolean wcdma_ded_srch_handler(
   boolean start_of_block, /* next frame is start */
   uint32  time_stamp,      /* currently not used  */
   gas_id_t gas_id)
{
  boolean remove_handler = FALSE;

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR

  g2w_srch_state_struct_type *ded = &ded_srch_state[gas_id];
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /* Always return abort call to upper layers right away if abort flag is true
     and wl1 driver isn't active.  (If driver is active, results will come from
     search or abort confirmation handler) */
  if (ded->gap_start_timeline == TIMELINE_INVALID)
  {
    /* Always return abort call to upper layers right away if abort flag is true
       and wl1 driver isn't active. */
    if(ded->abort)
    {
       ded->returned_result = TRUE;
    }
    else if(start_of_block)
    {
      /* set our timeline to 2 frames before the gap since that's where
         start_of_block becomes TRUE */
      ded->gap_start_timeline = -2;

      /* pass parameters to driver */
      MSG_GERAN_HIGH_1_G("G2W: Ded: preparing wl1 for UARFCN %d search", curr_ded_UARFCN[gas_id]);

      if(gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_READY)
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_FATAL("Driver not ready for search.  Bad state: %d", gl1_msg_g2w_driver_state[gas_id], 0, 0);
      }

      /* causes WL1 to call RF with rfm_meas_enter() */
      gl1_hw_g2w_send_rf_enter_ind(curr_ded_UARFCN[gas_id], gas_id);
      ded->rf_exit_needed = TRUE;
	  
	  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
	  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_START_DED); 
	  #endif

      /* causes WL1 to start WFW and build scripts */
      gl1_msg_g2w_pre_srch_cnf[gas_id] = FALSE;
      gl1_hw_g2w_send_pre_search_req(gas_id);
      gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_SRCH_REQ_PENDING;
    }
  }
  else
  {
    ded->gap_start_timeline++;


    if(ded->gap_start_timeline > DEDICATED_TIMEOUT)
    {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_FATAL("G2W: Ded: search taking too long (cur %d frames after gap)", ded->gap_start_timeline, 0, 0);
    }

    /* special case that can cause an abort internally */
    if(ded->gap_start_timeline == -1 &&
       (!gl1_msg_g2w_pre_srch_cnf[gas_id]))
    {
      MSG_GERAN_HIGH_1_G("G2W: Ded: Abort: caused by PRE_SRCH_CNF %d",
                          gl1_msg_g2w_pre_srch_cnf[gas_id]);
      /* make sure we set ourselves to abort state if frame slamming or driver issue */
      ded->abort = TRUE;
    }

    /* send abort to wl1 if needed regardless of timeline */
    if(ded->abort)
    {
       if((gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_READY) && (ded->rf_exit_needed))
       {
        /* Tell W driver to call rf_meas_exit(). Since the tuning
           scripts never executed, pass abort-flag as TRUE. */
        gl1_hw_g2w_send_rf_exit_req(TRUE, gas_id);
		l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;

        ded->rf_exit_needed = FALSE;
       }
       else if (gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_ABORT_PENDING)
       {
          /* Tell W driver to abort */
          gl1_msg_wcdmadrv_abort_search(gas_id);
       }
    }

    /* timeline specific actions... */

    if(ded->gap_start_timeline == -1)
    {
      /* perform actions needed 1 frames before air gap */

      if(ded->abort == FALSE)
      {
        /* get GFW to open the gap in the next frame
           (and close it after samples are collected) */
        gl1_hw_g2x_mdsp_rf_sample_ram_cmd(curr_ded_start_time[gas_id], gas_id);

        /* tell wl1 to go ahead with the search in the next frame */
        gl1_hw_g2w_search(curr_ded_UARFCN[gas_id],
                          1, /* the air-gap is in 1 G-frame from now */
                          curr_ded_start_time[gas_id],
                          WL1_X2W_DEDICATED_SRCH,
                          (wl1_x2w_srch_input_union_type *)&ded_cell_params[gas_id],
                          gas_id);
        gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_DEDICATED_PENDING;

        /* Suspend power measures for the next frame (during the gap) */
        gl1_msg_suspend_pwr_meas(gas_id);

        /* remember we instructed gfw to open the gap */
        ded->started_gfw_gap = TRUE;
      }
    }
    else if(ded->gap_start_timeline == 0)
    {
      if(ded->started_gfw_gap == TRUE)
      {
        /* re-enable pwr meas for the frame after the gap */
        gl1_msg_resume_pwr_meas(gas_id);
      }
    }
    else if(ded->gap_start_timeline == 1)
    {
      if((ded->rf_exit_needed == TRUE) && (ded->abort == FALSE))
      {
        /* tell wl1 to send rf_exit now that gap is physically over */
        gl1_hw_g2w_send_rf_exit_req(FALSE, gas_id);

        ded->rf_exit_needed = FALSE;
      }
    }
  }

  /* Mutex lock for checking results done */
  gl1_irat_mutex_lock(gas_id);

  if (/* finished talking to upper layer */
      ded->returned_result == TRUE &&
      ded->rf_exit_needed == FALSE)
  {
    ded->srch_in_progress = FALSE;

    if(ded->abort || ded->w_aborted_gap)
    {
       if(wcdma_ded_srch_abort_cb[gas_id])
       {
         wcdma_ded_srch_abort_cb[gas_id](ded->w_aborted_gap, gas_id);
       }
    }

    if(wcdma_srch_handler_done_cb[gas_id])
    {
      wcdma_srch_handler_done_cb[gas_id](gas_id);
    }
    ded->w_aborted_gap = FALSE;
    remove_handler = TRUE;
  }

  gl1_irat_mutex_unlock(gas_id);
  return (remove_handler);

  #else /* FEATURE_GSM_TO_WCDMA_MSGR */

  if (ded_srch_frame_count[gas_id] == -1)
  {
    /* When frame count = -1, we are waiting for start_of_frame */
    if (abort_ded_srch[gas_id])
    {
#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
      if(wcdma_ded_srch_abort_cb[gas_id])
      {
        wcdma_ded_srch_abort_cb[gas_id](gas_id);
      }
#endif
      abort_ded_srch[gas_id] = FALSE;
      remove_handler = TRUE;
      ded_srch_in_progress[gas_id] = FALSE;
    }
    else if (start_of_block)
    {
      /* this is frame-1 for the transaction. */
      ded_srch_frame_count[gas_id] = 0;

      /* Disable mDSP BB Rx clock gating for duration of W measurement */
      gl1_hw_enable_bbrx_clock_gating ( FALSE );

      /* pass parameters to driver */
      if (!wl1_x2w_srch_wcdmadrv_program_ded_mode_search(
                                                &ded_srch_params[gas_id],
                                                &ded_reconf_srch_parms[gas_id],
                                                gas_id))
      {
        MSG_GERAN_ERROR_0_G("Srch driver error");

#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
        if(wcdma_ded_srch_abort_cb[gas_id])
        {
          wcdma_ded_srch_abort_cb[gas_id](gas_id);
        }
#endif
        remove_handler = TRUE;
        ded_srch_in_progress[gas_id] = FALSE;
      }
      else
      {
        gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_NULL;
        /* This will switch remainder of search start processing to task context. */
        wl1_x2w_srch_g2w_initiate_dedicated_search(
                          WFW_IRAT_G2W_DEDICATED_MODE,
                          curr_ded_UARFCN[gas_id],
                          curr_ded_start_time[gas_id],
                          #if defined(FEATURE_DUAL_SIM) && defined(FEATURE_GSM_COEX)
                          gl1_msg_get_multi_sim_sys_mode(),
                          #endif
                          gas_id);
      }
    }
  }
  else
  {
    if (ded_srch_frame_count[gas_id] == 0)
    {
      if (abort_ded_srch[gas_id] ||
         (gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] != G2W_CMD_WAITING_FOR_TRIGGER)
          )
      {
        if (gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] != G2W_CMD_WAITING_FOR_TRIGGER)
        {
          MSG_GERAN_ERROR_1_G( "Do not schedule G2W because WFW not ready, gas_id %d", gas_id);
        }
        /* Tell the driver to abort */
        gl1_msg_wcdmadrv_abort_search(gas_id);
        abort_ded_srch[gas_id] = FALSE;
        remove_handler = TRUE;
      }
      else
      {
        gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_TRIGGERED;
        MSG_GERAN_HIGH_2_G("G2W: Enable WCDMA RX for dedicated search. UARFCN:%d start_time:%d",
                 curr_ded_UARFCN[gas_id], curr_ded_start_time[gas_id]);

        /* Make sure the command is issued from the ISR */
#ifdef FEATURE_WCDMA
        gl1_hw_rf_sample_ram_cmd( curr_ded_UARFCN[gas_id], curr_ded_start_time[gas_id],gas_id );
#endif
        /* Suspend power measures for the next frame */
        gl1_msg_suspend_pwr_meas(gas_id);
     }
    }
    else if (ded_srch_frame_count[gas_id] == 1)
    {
      /* If this is the frame of sample ram fill, re-enable pwr meas & PSBI */
      gl1_msg_resume_pwr_meas(gas_id);
    }
    else if (ded_srch_frame_count[gas_id] == 2)
    {
      /* We have an active ded search */
      /* Has sample ram been filled for this search, and can can we */
      /* shut down RF related stuff */
      MSG_GERAN_HIGH_0_G("G2W Dedicated Search Done");

      remove_handler = TRUE;

      /* Is PL1 telling us to abort, or are we timing out a ded search? */
      if (abort_ded_srch[gas_id])
      {
        /* Clear this abort notification */
        abort_ded_srch[gas_id] = FALSE;

        /* Tell the driver to abort */
        gl1_msg_wcdmadrv_abort_search(gas_id);
      }
    }

    ded_srch_frame_count[gas_id]++;
  }

  if (remove_handler)
  {
    gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_NULL;
    ded_srch_frame_tick_active[gas_id] = FALSE;
    /* Re-enable mDSP BB Rx clock gating, disabled for duration of W measurement */
    gl1_hw_enable_bbrx_clock_gating ( TRUE );

  }

  return (remove_handler);

  #endif /* FEATURE_GSM_TO_WCDMA_MSGR */
}
#endif /* FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW */
#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */


/*===========================================================================

FUNCTION wcdma_ded_srch_timout

DESCRIPTION
  This function should be called when dedicated serach is timed out

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wcdma_ded_srch_timout(gas_id_t gas_id)
{
#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW
#ifdef FEATURE_INTERRAT_NON_BLIND_HANDOVER_GTOW
    wcdmadrv_srch_abort_cb(gas_id);
#endif
#endif
}

#ifdef FEATURE_GSM_TO_WCDMA_MSGR

/*===========================================================================

FUNCTION  gl1_msg_g2w_init_cnf

DESCRIPTION
 process g2w Msgr messages

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msg_g2w_init_cnf(
  /* the message to handle */
  wl1_irat_x2w_init_cnf_struct_type *init_cnf,
  /* subscription ID (for multisim) */
  gas_id_t gas_id)
{
  l1_sc_globals_T     *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  gl1_irat_mutex_lock(gas_id);
  if(G2W_DRIVER_INIT_PENDING == gl1_msg_g2w_driver_state[gas_id])
  {
    gl1_hw_wcdma_save_g2w_timing_params(&init_cnf->rf_timing_params, gas_id);
    gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_READY;
  }
  else
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("G2W: Received INIT_CNF without pending init (g2w driver state %d)",
              gl1_msg_g2w_driver_state[gas_id], 0, 0);
  }

  if (l1_sc_globals_ptr->g2w_shutdown_pending == TRUE)
  {
     l1_sc_g2w_shutdown(gas_id);
  }
  else
  {
     l1_sc_set_wcdma_irat_active(TRUE,gas_id);
   
    /*FR 47064 Stop MCPM for ded/xfer mode- Start at the time of G2W gap*/
    if(l1_tskisr_blk->l1_state != L1_INACTIVE_MODE)
    {
      if(g2w_active_mcpm_scenario[gas_id] != 0)
      {
        gl1_drx_task_active(gas_id);
        gl1_hw_mcpm_neighbor_update(MCPM_G2W_MEAS_STOP, g2w_active_mcpm_scenario[gas_id], gas_id);
        g2w_active_mcpm_scenario[gas_id] = 0; 
      }
    }
  }

  gl1_irat_mutex_unlock(gas_id);

  /*Decrement active task count (incremented when start MCPM on for G2W)*/
  gl1_drx_task_inactive(gas_id);

}


/*===========================================================================

FUNCTION  gl1_msg_g2w_deinit_cnf

DESCRIPTION
 process g2w Msgr messages

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msg_g2w_deinit_cnf(
  /* the message to handle */
  wl1_irat_x2w_deinit_cnf_struct_type *deinit_cnf,
  /* subscription ID (for multisim) */
  gas_id_t gas_id)
{
  gl1_irat_mutex_lock(gas_id);

  if(G2W_DRIVER_DEINIT_PENDING != gl1_msg_g2w_driver_state[gas_id])
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_FATAL("G2W: Received DEINIT_CNF without pending deinit (g2w driver state %d)",
               gl1_msg_g2w_driver_state[gas_id], 0, 0);
  }

  gl1_msg_g2w_driver_state[gas_id]= G2W_DRIVER_NULL ;
  if(l1_sc_get_wcdma_update_list_irat_active(gas_id) == FALSE)
  {
    /* If going inactive or BPLMN suspend/resume do not turn off WSTMR as need to be continuous for WL1 timing */
    if((l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE)&&(l1_get_background_wcdma_plmn_scan_active(gas_id) == FALSE))
    {
     if(g2w_active_mcpm_scenario[gas_id] != 0)
     {
      gl1_drx_task_active(gas_id);
      gl1_hw_mcpm_neighbor_update(MCPM_G2W_MEAS_STOP, g2w_active_mcpm_scenario[gas_id],gas_id);
     }
     gl1_deep_sleep_pending[gas_id] = FALSE; 
   }
   else if(l1_tsk_buffer[gas_id].l1_state == L1_INACTIVE_MODE)
   {
     if(gl1_deep_sleep_pending[gas_id] == TRUE)
     {
       gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_STOP_REQ, GL1_MCPM_GERAN_STOP,gas_id);
       gstmr_force_reinit(gas_id);
       MSG_GERAN_HIGH_0_G("G2L: MCPM_GERAN_STOP_REQ sent");
     }
    gl1_deep_sleep_pending[gas_id] = FALSE;
   }
   g2w_active_mcpm_scenario[gas_id] = 0;
   l1_sc_set_wcdma_irat_mcpm_active(FALSE,gas_id);
   MSG_GERAN_HIGH_0_G("G2W: Shuhtdown complete");
  }
  else
  {
     MSG_GERAN_HIGH_0_G("G2W: Deinit complete, start init again now");
     /* Re-initialise G2W drivers */
     gl1_hw_wcdma_init(gas_id);
  }
  l1_sc_mode_cnf_pending(FALSE, bWCDMA_RAT_INACTIVE, gas_id);
  gl1_irat_mutex_unlock(gas_id);
  /*Decrement active task count*/
  gl1_drx_task_inactive(gas_id);
}

/*===========================================================================

FUNCTION  gl1_msg_g2w_irat_pre_srch_cnf

DESCRIPTION
 process g2w Msgr messages

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msg_g2w_irat_pre_srch_cnf(
  wl1_irat_x2w_pre_srch_cnf_struct_type *pre_srch_done,
  gas_id_t gas_id)
{
  gl1_irat_mutex_lock(gas_id);

  gl1_msg_g2w_pre_srch_cnf[gas_id] = TRUE;

  gl1_irat_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION  gl1_msg_g2w_irat_srch_cnf

DESCRIPTION
 process g2w Msgr messages

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msg_g2w_irat_srch_cnf(
  wl1_irat_x2w_srch_cnf_struct_type *srch_done,
  gas_id_t gas_id)
{
  gl1_irat_mutex_lock(gas_id);

  if(G2W_DRIVER_IDLE_ID_PENDING == gl1_msg_g2w_driver_state[gas_id] &&
     WL1_X2W_IDLE_ID_SRCH == srch_done->mode)
  {
    gl1_process_x2w_idle_id_search_result(&srch_done->result.idle_id,
                                          srch_done->uarfcn,
                                          srch_done->rssi,
                                          !srch_done->srch_allowed,
                                          gas_id);
    gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_READY;
  }
  else if(G2W_DRIVER_IDLE_RECONF_PENDING == gl1_msg_g2w_driver_state[gas_id] &&
          WL1_X2W_IDLE_RECONF_SRCH == srch_done->mode)
  {
    gl1_process_x2w_idle_reconfirm_search_result(&srch_done->result.idle_reconf,
                                                 srch_done->uarfcn,
                                                 srch_done->rssi,
                                                 !srch_done->srch_allowed,
                                                 gas_id);
    gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_READY;
  }
  else if(G2W_DRIVER_DEDICATED_PENDING == gl1_msg_g2w_driver_state[gas_id] &&
          WL1_X2W_DEDICATED_SRCH == srch_done->mode)
  {
    gl1_process_x2w_dedicated_search_result(&srch_done->result.dedicated,
                                            srch_done->uarfcn,
                                            srch_done->rssi,
                                            !srch_done->srch_allowed,
                                            gas_id);
    gl1_msg_g2w_driver_state[gas_id] = G2W_DRIVER_READY;
  }
  else
  {
    /* This state can happen in some valid scenarios since WL1 sometimes sends
       GL1 a SEARCH_DONE even after an ABORT_DONE for the same search
       transaction */
    MSG_GERAN_HIGH_2_G("G2W: unexpected msg/driver-state combo (%d/%d)",
                        srch_done->mode,
                        gl1_msg_g2w_driver_state[gas_id]);
  }

  gl1_irat_mutex_unlock(gas_id);
}

/*===========================================================================

FUNCTION  gl1_handle_wcdma_irat_mgsr_msgs

DESCRIPTION
 process g2w Msgr messages

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_handle_wcdma_irat_mgsr_msgs(
  /* the message to handle */
  geran_gl1_msgr_t *cmd_ptr,
  /* subscription id this function is running under */
  gas_id_t gas_id)
{
  switch(cmd_ptr->msg.hdr.id)
  {
    case WCDMA_IRAT_G2W_INIT_CNF:
    {
      gl1_msg_g2w_init_cnf(&cmd_ptr->msg.wl1_irat_g2w_init_cnf, gas_id);
      break;
    }
    case WCDMA_IRAT_G2W_DEINIT_CNF:
    {
      gl1_msg_g2w_deinit_cnf(&cmd_ptr->msg.wl1_irat_g2w_deinit_cnf, gas_id);
      break;
    }
    case WCDMA_IRAT_G2W_PRE_SRCH_CNF:
    {
      gl1_msg_g2w_irat_pre_srch_cnf(&cmd_ptr->msg.wl1_irat_g2w_pre_srch_cnf, gas_id);
      break;
    }
    case WCDMA_IRAT_G2W_SRCH_CNF:
    {
      gl1_msg_g2w_irat_srch_cnf(&cmd_ptr->msg.wl1_irat_g2w_srch_done, gas_id);
      break;
    }
    case WCDMA_IRAT_G2W_ABORT_CNF:
    {
      gl1_msg_g2w_irat_abort_cnf(&cmd_ptr->msg.wl1_irat_g2w_abort_cnf, gas_id);
      break;
    }
    case WCDMA_IRAT_G2W_RF_EXIT_CNF:
    {
      gl1_msg_g2w_irat_rf_exit_cnf(gas_id);
      break;
    }
    default:
    {
      MSG_GERAN_ERROR_1_G("G2W: Unknown message router msg id %d", cmd_ptr->msg.hdr.id);
      break;
    }
  }
}

#else /* FEATURE_GSM_TO_WCDMA_MSGR */

#ifdef FEATURE_INTERRAT_CELL_RESELECTION_GTOW

/*===========================================================================

FUNCTION wcdma_start_rf_cmd

DESCRIPTION
  This function is called in GSM L1 task context in idle mode and sets
  a flag to trigger the start rf command in the ISR.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  wcdma_start_rf_cmd( gas_id_t gas_id )
{
  /* Set this flag to TRUE to trigger the
   * rf start command in ISR.
   */
  GDRV_ISR_LOCK(gas_id);

  gl1_msg_wcdma_id_srch_start_rf_cmd[gas_id] = G2W_CMD_WAITING_FOR_TRIGGER;

  GDRV_ISR_UNLOCK(gas_id);
}


void wcdma_reconf_start_rf_cmd( gas_id_t gas_id )
{
  GDRV_ISR_LOCK(gas_id);

  gl1_msg_wcdma_reconf_start_rf_cmd[gas_id] = G2W_CMD_WAITING_FOR_TRIGGER;

  GDRV_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION wcdma_set_srch_done

DESCRIPTION
 setting wcdmadrv_search_done
DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wcdma_set_srch_done(gas_id_t gas_id)
{
  wcdmadrv_search_done[gas_id] = TRUE;
}

/*===========================================================================

FUNCTION gl1_check_ded_srch_in_progress

DESCRIPTION
Retruns the value of ded_srch_in_progress flag

DEPENDENCIES
 NA

RETURN VALUE
  ded_srch_in_progress flag

SIDE EFFECTS

===========================================================================*/
boolean gl1_check_ded_srch_in_progress(gas_id_t gas_id)
{
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  return ded_srch_state[gas_id].srch_in_progress;
  #else
  return ded_srch_in_progress[gas_id];
  #endif
}

#endif /* FEATURE_INTERRAT_CELL_RESELECTION_GTOW */
#endif /* ! FEATURE_GSM_TO_WCDMA_MSGR */

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/* Ec/Io Table size entries */
#define G2W_SRCH_ECIO_TAB_SIZ 64
uint32 g2w_srch_ecio_tab[G2W_SRCH_ECIO_TAB_SIZ] =
{
  9216, /*   0.0 dB */
  8214, /*   0.5 dB */
  7321, /*   1.0 dB */
  6525, /*   1.5 dB */
  5816, /*   2.0 dB */
  5184, /*   2.5 dB */
  4621, /*   3.0 dB */
  4119, /*   3.5 dB */
  3671, /*   4.0 dB */
  3272, /*   4.5 dB */
  2917, /*   5.0 dB */
  2600, /*   5.5 dB */
  2318, /*   6.0 dB */
  2066, /*   6.5 dB */
  1842, /*   7.0 dB */
  1642, /*   7.5 dB */
  1464, /*   8.0 dB */
  1305, /*   8.5 dB */
  1164, /*   9.0 dB */
  1038, /*   9.5 dB */
  925,  /*  10.0 dB */
  825,  /*  10.5 dB */
  736,  /*  11.0 dB */
  656,  /*  11.5 dB */
  585,  /*  12.0 dB */
  522,  /*  12.5 dB */
  466,  /*  13.0 dB */
  415,  /*  13.5 dB */
  371,  /*  14.0 dB */
  331,  /*  14.5 dB */
  295,  /*  15.0 dB */
  264,  /*  15.5 dB */
  235,  /*  16.0 dB */
  210,  /*  16.5 dB */
  188,  /*  17.0 dB */
  168,  /*  17.5 dB */
  150,  /*  18.0 dB */
  134,  /*  18.5 dB */
  120,  /*  19.0 dB */
  107,  /*  19.5 dB */
  96,   /*  20.0 dB */
  86,   /*  20.5 dB */
  77,   /*  21.0 dB */
  69,   /*  21.5 dB */
  62,   /*  22.0 dB */
  56,   /*  22.5 dB */
  50,   /*  23.0 dB */
  45,   /*  23.5 dB */
  41,   /*  24.0 dB */
  37,   /*  24.5 dB */
  33,   /*  25.0 dB */
  30,   /*  25.5 dB */
  27,   /*  26.0 dB */
  25,   /*  26.5 dB */
  22,   /*  27.0 dB */
  20,   /*  27.5 dB */
  19,   /*  28.0 dB */
  17,   /*  28.5 dB */
  16,   /*  29.0 dB */
  14,   /*  29.5 dB */
  13,   /*  30.0 dB */
  12,   /*  30.5 dB */
  11,   /*  31.0 dB */
  11    /*  31.5 dB */
};

static uint16 gl1_g2w_srch_get_ecio ( uint32 cell_eng )
{
  /* Three indices into the array for binary search */
  register uint16  top;
  register uint16  bot;
  register uint16  mid;

  /* Iteration counter   */
  uint16 iter = 0;

  /* Clip too-small energies to 63, or 31.5 dB down */
  if ( cell_eng < g2w_srch_ecio_tab[ G2W_SRCH_ECIO_TAB_SIZ - 2 ] )
  {
    return ( G2W_SRCH_ECIO_TAB_SIZ - 1 );
  }
  else   /* In range, so do a binary search */
  {
    top = 1;
    bot = G2W_SRCH_ECIO_TAB_SIZ - 2;
    /* Start with top and bot at the extreme ends. Note that we start
       bot at 62, not 63, because the 63 case has already been handled. */

    for (;;)    /* Loop forever (or until slipping out the back door) */
    {
      if ( ++iter > G2W_SRCH_ECIO_TAB_SIZ )
      {
        /* Something bad happened. Binary search should never
           iterate more than this. We're hung. */
        MSG_GERAN_ERROR_0("G2W: Binary search failed - max iterations");
        return ( 63 ); /* Return min value */
      }

      mid = (top + bot) / 2;

      /* Sanity for range checking */
      if ( ( mid < 1 ) ||
           ( mid >= ( sizeof( g2w_srch_ecio_tab ) / sizeof( g2w_srch_ecio_tab[0] ) ) )
         )
      {
        return ( 0 );
      }
      /* Find an entry near the middle of the current range */
      else
      {
        if ( cell_eng >= g2w_srch_ecio_tab[ mid ] )   /* Energy larger? */
        {
          if ( cell_eng < g2w_srch_ecio_tab[ mid - 1 ] )
          {
            /* Should return mid here, but return an index one lower than
              mid to overcome the 0.6 db negative bias on the common pilot
              results reported by UE*/
            return ( mid - 1 );          /* Just right! */
          }
          else
          {
            bot = mid - 1;             /* Too small, go to bigger half */
          }
        }
        else    /* Energy smaller */
        {
          top = mid + 1;               /* Too large, go to smaller half */
        }
      }
    } /* for (;;) */

  } /* else value in range */
}

/*===========================================================================

FUNCTION      gl1_wcdma_ecio_logarithmic_average

DESCRIPTION   This function calculates the  linear average of 16-bit previous and
              current Ec/Io values and returns ecio corresponding to  average_eng calculated
              as below.It works by getting the actual linear values corresponding to the
              indexes pointed by current_ecio and previous_ecio in the ecio_tab.The average
              sum is calculated by
              average_eng= 0.9 * eng corresponding to current_ecio +
              0.1 * eng corresponding to previous_ecio

DEPENDENCIES  The previous and current ecio values are twice of the actual ecio values.

RETURN VALUE  16-bits Ec/Io value. If the energy is out of range,  the return value is
              clipped to 0 or 63.

SIDE EFFECTS

===========================================================================*/
int16  gl1_g2w_get_ecio_logarithmic_average(int16  current_ecio, int16 previous_ecio)
{
  uint32 average_eng; /*sum of eng1 and eng2 */

  current_ecio = (((current_ecio) < 0) ? -(current_ecio) : current_ecio);
  previous_ecio =(((previous_ecio) < 0) ? -(previous_ecio) : previous_ecio);

  /* both ecio values are >= 63 */
  if (((current_ecio) >= (G2W_SRCH_ECIO_TAB_SIZ-1)) &&
      ((previous_ecio) >= (G2W_SRCH_ECIO_TAB_SIZ-1)))
  {
    /* Return the last value in the table */
    return ( (-1)*(G2W_SRCH_ECIO_TAB_SIZ - 1) );
  }
  else   /* Value < 63 */
  {
    average_eng = (9*g2w_srch_ecio_tab[current_ecio] +
                   g2w_srch_ecio_tab[previous_ecio])/10;
    return((-1)*gl1_g2w_srch_get_ecio ( average_eng ));
  }
}

/*===========================================================================

FUNCTION gl1_check_if_g2w_init_pend

DESCRIPTION

Check if no init req has been sent to WCDMA 
  
Parameters
NA

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean gl1_check_if_g2w_init_pend(gas_id_t gas_id)
{

 MSG_GERAN_HIGH_1_G("Check pend init_sent %d",gl1_msg_g2w_driver_state[gas_id]);
 
 /*If G2W driver state us not pending/ready/deinit ongoing, it means no INit is sent to WL1*/
 if((gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_INIT_PENDING) && (gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_READY)
 	&& (gl1_msg_g2w_driver_state[gas_id] != G2W_DRIVER_DEINIT_PENDING))
 {
   return TRUE;
 }
 else
 {
   return FALSE;
 }
}

#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

/*===========================================================================

FUNCTION gl1_check_if_g2w_init_deinit_pend

DESCRIPTION

Check if init/deinit req has been sent to WCDMA and response is pending
  
Parameters
NA

RETURN VALUE
TRUE: if pending
FALSE: if confirmation already received from WL1

SIDE EFFECTS
  None

===========================================================================*/

boolean gl1_check_if_g2w_init_deinit_pend(gas_id_t gas_id)
{
 
 /*If G2W driver state is not pending, it means Init/deinit confirm is already received from WL1*/
 if((gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_INIT_PENDING) || (gl1_msg_g2w_driver_state[gas_id] == G2W_DRIVER_DEINIT_PENDING))
 {
   return TRUE;
 }
 else
 {
   return FALSE;
 }
}
