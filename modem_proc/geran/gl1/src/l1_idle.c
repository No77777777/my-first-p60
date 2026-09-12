/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            L 1  I D L E  I S R   C O N T E X T  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to control IDLE mode processing.
   It handles scheduling of receive of PCH for example.
   This procedure executes in the L1 ISR context.

EXTERNALIZED FUNCTIONS
  L1_idle_mode_ISR
    Procedure to initiate a synchronous receive of a CCH message.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_idle.c#5 $
$DateTime: 2025/04/11 04:48:16 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
11/04/25   hr        CR4106231 KW P1 issues- gl1
15/11/18   rks       CR2337603 Clear Missed Frames when TRM denied for CCCH activity in second frame
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
17/01/19   sal       CR2600781 TA in Reorg mode: Reset ta_prepared flag when moving back to Normal page mode.
06/05/19   hp        calculating current frame position currectly incase of page reorg inside QTA gap
15/06/18   ng        CR2261488 Remove 'l1_rcvd_non_drx_ind' related handling, it is no more needed
30/07/18   sk        CR2286080 Do not init TA in page-reorg inside QTA gap
18/01/16   zf        CR961805: Set first burst flag in x2g_startup cmd
07/02/18   rc        CR2184167 Sending MAC_L1_RECOVERY_CNF after L1 moved to Idle State.
20/01/17   akm       CR1109976 Optimization in G2X BPLMN search
19/04/18   sp        CR2225702 Remove reservations (CR2174600) for page before GSM Suspend for G2X PLMN search, instead provide less time for PLMN Search.
28/03/18   sk        CR2212923 Release TRM (DO not extend GBTA) when the PTM SUB is releasing TBF
28/03/18   sk        CR2214105 Invalidate the l1_local_idle_data_ptr->next_req_frame on GSM resume so that 
                               CCCH FN is again added into DRX table
06/11/17   sk        CR2137714 Do not skip CCCH block in QTA until 3 bursts missed. While skipping,
                               clear missed frames counter and indicate missed block to RR
26/02/18   nsa      CR2195353 Modifying non-drx timer check so that page decoding will not get affected. 
20/02/18   shm      Change Request 2192580 : KW P1 errors on TA.3.0 fix
15/02/18   nsa       CR2188236 No need of resetting gap to next activity to zero in SDCCH_Combined mode & 
                               Reserving with LTA reason-subreason when non-drx timer is set. 
25/05/17   hd        CR2051434 Make the variable l1_rcvd_non_drx_ind dual spaced
02/02/18   nsa       CR2174600 Not releasing TRM reservations when BPLMN search is ongoing so that there is enough time when GSM resumes for page.
10/05/17   sk        CR2024440 Add an explicit intlock for DivRx meas buffer update
01/02/18   km        CR2081337 Avoid resetting GBTA state when gap_to_next_ccch_activity is less than 4 frames
04/08/17   hd        CR2085522 Force crash when missed frame is set, but not cleared in a previous activity
14/06/17   nm        CR2056096 Clear pending freq ptr and ul/dl st at Idle init
21/12/17   shm		CR2162686 Clearing the missing frames when we are in non-drx mode.
06/06/17   ksb       CR2053763 Defer the cleanup command instead of giving immediate async when xcch recieve is cancelled in the QTA gap
18/12/17   rc        CR2160237 GBTA Active count check is not valid when l1_idle_non_drx_timer_flag is TRUE.
27/12/17   sk        CR2163890 Avoid BCCH reservation in non-DRX mode to avpid other subsequent issues
27/06/17   sn        CR2064535 Calculate the GAP correctly to next CCCH actvity during SDCCH combined case
07/12/17   nsa       CR2153366 After Transfer when we are in non-drx mode if we are in middle of CCCH block then we have to reserve for next block start.
11/09/17   rc       CR2105004 Debug code to check why client_id exchange is not happening in transfer mode init.
15/12/17   sk        CR2159738 Ensure idle TA is not initialized when inside the QTA gap
13/12/16   mk        CR1098240 Clear leaving_idle_mode flag at the end of idle mode to avoid SCH reconfirmation getting abandoned in SDCCH channel
27/03/17   akm       CR2023752 Missed CB indication is sent if CBCH is aborted due to error recovery
16/05/17   akm       CR2047705 Add mutex lock when releasing CBCH DRX entry by task
16/08/17   km        CR2089992 DTF handler to abort G2L IRAT search in idle mode
30/08/16   tsk       CR1026504: Update to 0x5096 RxD log packet with additional details.
10/11/16   ksb       CR1087007 Correction in calculation of OTA CBCH frame while sending the CBCH missed indication to CB
09/08/16   sn        CR1049179 Do not move to transfer mode when Idle mode monitors are still going on in SS mode
05/01/16   zf        CR954362: After prioritized NBCCH cancelled CCCH, clear its required frames
12/08/16   sk        CR1053849 Ensure ISR overrun conditions validated after loading the FW in Multisim CBCH state machine
09/09/16   sn        CR1062272 TRM RESERVE AT during IDLE to transfer
20/12/17   sk        CR2030816 GERAN changes for PBR Randomization algorithm
15/09/15   br        CR904773: Dont don't allow any scheduling while fw release is pending.
19/12/17   rv        CR958652 Give suffcient time for idle mode to abort properly in single sim mode if idle rx  inprogress is set
22/04/16   ksb       CR1005869 To mainlne FEATURE_GL1_SLEEP_DEBUG_STRUCT
20/07/16   rv        CR1039950:Fix for-FW_UNLOAD happened due to unexpected call_in_task_grm_release during NCELL SCH abort in IDLE_WAITING
25/11/17   sn       CR2148990 Fixing Compilation issue with CR1061730 with new build Flavor
25/04/16   sp        CR953124: Dont Prioritize CB over NBCCH
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
30/01/17   mk        CR1113152 Keep RxD always enabled in FTM mode based on RxDForcedON NV
17/08/17   br        CR2093742 Ta in reorg: While booking for page after TA frame number align the reservation to block start.
12/07/17   sp        CR2075151 TA in Reorg mode: Reset TA info when moving back to Normal page.
17/07/17   br        CR2073011 In case  UE  moves to REORG mode in QTA gap, book for CCCH in next multiframe
05/07/17   br        CR2060300 TA in Reorg mode: Extended page misses on one sub during page reorg on other sub.
02/02/17   br        CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
08/08/17   nm        CR2040229 Enable PCH read in 2 phase access for both Idle sub and data sub using GBTA
20/04/17   ksb       CR2031204 GL1 to send message to RR indicating if BCCH missed is due to TRM denial/CRC failure 
06/09/16   sp        CR969186: Send CB Missed INdication, if planned CB FN is elapsed.
18/08/16   gmr       CR1051051 Power measurement to be completed for current SUB before Aborting Idle when  other SUB in transfer 
02/08/16   mn        CR1049025 Extend TRM lock if current activity is PCH and previous activity is SCH ,when QTA is under progress
27/06/16   zf        CR1035011: In non DRX mode, move to IDLE Receive state.
24/06/16   nm        CR1032619 Back out CR1021832 
20/06/16   bg        CR1025662 Do not clear the CCCH frames if Rx activity is ongoing in Single SIM case
13/06/16   nm        CR1021832 Debug code to crash when RRBP vs PCH collision is not detected by GL1
03/06/16   sk        CR1011063 Do not update average power if all the CCCH bursts have been yielded
18/5/16    sm        CR1017901 Added GL1 idle mode QSH events 
19/05/16   bg        CR1014642 Correction in calculating gap to next activity
16/05/16   db        CR1009490 Move CBCH priority handling from GRR to GL1
12/05/16   dp        CR1007422 Implement wakeup cancellation of G2X IRAT (during sleep error recovery)
12/05/16   akm       CR1010520 DTF sleep scenario "PCH and NCELL activity cancellation"
03/05/16   mko       CR1010446: When idle sub monitors are scheduled in transfer sub idle frame, then don't put idle sub to sleep before completing the moniotors
28/04/16   hd        CR1008839 Do not clear GBTA flag if already CCCH is scheduled for the immediate block 
26/04/16   hd        CR998694 Define l1_update_and_send_srch_time_ind
26/4/16    sn        CR997902 Own paging will be TRUE for extended paging block also.
09/03/15   br        CR985405 GL1 to ignore ASDIV switch if CCCHis not scheduled due to Tx Sub conflict
29/02/15   mn        CR982922 Remove TRM realease when UT Aborted during NCell sch
26/02/16   js        CR979228 Don't reserve for CCCH after receiving DL_TBF 
23/02/16   sk        CR978034 Do not vote for next frame tick for GSM BPLMN when other RAT is in BG TRAFFIC
12/02/16   db        CR974503 GL1 reserving wrongly for BCCH during Non-DRX mode 
04/02/15   sk        CR972370/CR887629 Do not clear the CCCH frames if Rx activity is ongoing
12/01/16   sk        CR960922 Ensure GBTA active frame count is reset at the end of the GBTA gap
06/01/16   sp        CR957494: Allow clear of CBCH DRx ref, but maintain next CBCH Occasion so that skip ind can be sent when X-tech in traffic.
09/12/15   ab        CR947584 : IDLE Sub incorrectly scheduling GBTA POWER monitor when other Sub in Two Phase Access
11/12/15   sk        CR943240 Consider AGC params as valid when the block decoded in single burst
24/11/15   zf        CR942824: Cleanup QBTA if a xCCH block is not required.
26/10/15   snjv      CR929109 Update FM booking if TRM denied for CCCH in Idle Wait sub state
06/11/15   ab        CR927639 : CXM In QDTA QDA use desense_band_id as freq id to avoid X blanking.
24/11/15   sp        CR924033 Apply RR requested CB priority inversion, only on next TC=0 occassion.
18/11/15   cjl       CR939889 Fix GSM stuck in QTA gap booking blocked ncell activity
06/11/15   ab        CR935174 : Perform G IDLE Freq after all RX done to avoid X desense the G PCH RX.
09/11/15   zf        CR937427 Enable error recovery in non-QTA mode
16/11/15   zf        CR939824: Clear entered_warmup_recovery flag after abort CCCH
22/10/15   zf        CR923858 Rework Sleep error recovery in QTA
24/10/15   mko       CR921690 Update CB Skip Request handling, Check each entry of skip req and schedule intermediate TCs
26/09/15   hd        CR913981 Send unknown block quality indication for BCCH decoding failure due to TRM denial
28/09/15   mko       CR912681 Don't start monitors scheduling from idle sub when monitors event status is already active/scheduled
30/09/15   zf        CR916540 Do not sleep when BPLMN pending
30/09/15   sk        CR916227 Disable frequent QSH events and add PCH rsv handler
28/09/15   snjv      CR913042 Re-Enable DR if disabled, when leaving Idle mode
16/09/15   cjl       CR908527 improve system procedures used for TRM access
10/09/15   nm        CR896012 Release TRM in SDCCH COMB case for Page Reorg after all CCCH blocks in MF
08/09/15   nk        CR900020 Reset BPLMN related variable at Idle init
14/08/15   nm        CR890023 Do not Resv for BCCH in non drx mode
04/09/15   cjl       CR892689 Ensure BPLMN client does not retain TRM chain when aborted
02/09/15   zf        CR905869: Set trm_status to NULL after grm release
31/08/15   mko       CR896740:sleep error recovery changes in QTA gap
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
19/08/15   mc        CR874662 : CXM Freq ID support for X2G startup and X2G preload
14/08/14   ws        CR889273 Fix Static declarations of variables which are not Dual data spaced
11/08/15   fan/akh   CR871795 FR19329: Avoid cancelled CCCH booking after rude wakeup
09/08/15   zf        CR884852: When ISR overrun, wait for a frame before release TRM
10/08/15   og        CR884495. Avoid resetting the campedon cell agc structure when dealing
                     with unfinished power monitors.
04/08/15   og        CR881751. Missing gl1_hw_turn_tx_on( ) when sending RACH
                     results in a GFW TXLM crash.
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons
27/05/15   am        CR843429 Take missed frames by sleep recovery into account for ACQ
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types
03/06/15   dg        CR847036 Allow idle state machine to run if id state is WAIT
11/06/15   fm        CR852155 : reset all_paging_blocks flag after changing to extended paging mode
15/07/15   fan/akh   CR857332 FR19329: WMGR support for rescheduling non-PCH activities
07/07/15   nk        CR867358 Move BPLMN state to INACTIVE in suspension callback
14/07/15   cws       CR845909 Switch antenna when AGCH decode fails
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update
                     during OOS ACQ/IDLE MONITORS
09/07/15   cah       CR811835: FR 26893: Band-by-Band OOS and BPLMN Search - GL1
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
06/07/15   sp        CR862778: Reduce F3 logging in Idle as its impacting QTA timeline
30/06/15   ab        CR851337 : DR-DSDS. Design to allow SR(GBTA) to DR Transition.
29/06/15   shr       CR860767: Add XCCH Debug Capability in DR-DSDS using NV control
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
10/04/15   dg        CR819629 Don't run idle frame state machine when G2X activity ongoing
23/06/15   fan/akh   CR859092 Adapt WMGR Cancel handling to RACH Rescheduling in DSDA, DR-DSDS
06/06/15   zf        CR848635: Use QBTA cleanup offset not 0 when abort a QTA
08/06/15   ip        CR847902 Miss first burst in QTA gap when warm up time is too short instead of missing PCH
04/06/15   ab        CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
29/05/15   fan       CR843017 Correct handling of pending_deferred_wmgr_event flag and gl1_hw_wkupmgr_wakeup_point_fn
22/05/15   mko       CR842062: Don't call l1_sci_tick_idle() after deferring CCCH wakeup to next DRX cycle
22/04/15   am        CR803948 TA in idle mode when mm_non_drx is enabled
18/03/15   nm        CR808145 Reset reduced pow mon index if PCH missed due to sleep error recovery
20/05/15   aga       CR840984 NULL2X IRAT changes for G2W IRAT
19/05/15   sjw       CR840271: clear inactive CBCH FM entries when priority is changed
14/05/15   akh       CR813711 Improvements to power monitor debug structure
13/05/15   sp        CR819508: Dont reserve for next CBCH in post tick, instead wait for next tick - as GL1 may get Skip Req.
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
15/04/15   pa        CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
07/05/15   ip        CR833523 Clear Error Recovery flag when updating default scheduled activity
07/05/15   fan/akh   CR834237 FR19329 Full G wake-up cancellation + code cleanup
05/07/15   zc        CR826075 Band Specific ASDIV Enable
30/04/15   ip        CR799995 FR26290 Sleep when X tech is in connected state
24/04/15   fan       CR796481 Full G wakeup cancellation
21/04/15   cja       CR826384 For reconf do not use idle monitors in progress flag
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX
10/04/15   cah       CR819666 Problem in the logic for adding the denied bands to trm_reserve_at and trm_request
08/04/15   cja       CR800650 Optimise irat state machine
03/04/15   ab        CR809082 Do not schedule PWR_MON_EVENT for missed page ,
                     when Transfer Sub is Releasing TBF.
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
27/03/15   dv/pa     CR813849: In X+G DR-DSDS, revert back to SR after 3 consequtive TRM denials
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
16/03/15   pg        CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP
12/03/15   zf        CR806937: Do not force crash based on QTA count
03/03/15   nm        CR801161 Fix GBTA_DEBUG compilation error
06/03/15   zf        CR804422: Do not double increase idle abort crash count.
03/04/15   br        CR763774 GL1 logging improvement to ease debugging
17/11/14   dg        CR758006 IRAT improvements to ease debugging
24/02/15   js        CR749012: GL1 changes for LTE removal
22/01/15   zf        CR784496: Enable sleep when BG traffic enabled.
20/02/15   js        CR693771: W+T removal. Resolving compilation errors
19/02/15   sp        CR787028 Dont retain GBTA lock if next BCCH is far
19/02/15   js        CR793536 Clearing NBCCH while aborting idle and ensuring trm is released in QTA,correcting fix done in CR790660
18/02/15   js        CR791114 Dont clear CBCH activity immediatley at FM after sleep recovery.
18/02/15   cah       CR782091: Ensure correct Rxalpha is passed to GFW in CM init and when leaving idle.
18/02/15   am        CR770270 Ensure CBCH is not scheduled if it is cleared from frame manager
15/02/15   sp        CR729492 Remove unimportant logging
13/02/15   df        CR726974 Erase stale list scheduled events during err recovery
11/02/15   js        CR775188 GL1 changes to receive EPD preference
20/01/15   cws       CR686152 [ASDIV] calculate idle average power properly and reset it after switching
10/02/15   og        CR788148. Debug code for the l1 idle mode frame manager lock up.
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs.
23/01/15   npt       CR697236 Mainline FEATURE_GERAN_VSTMR
16/01/15   cgc       CR782093 Add receive_in_progress to stuck in idle ERR_FATAL
23/12/14   pa        CR773120: In DRDSDS, don't enable TX on exiting from Idle mode.
19/12/14   fan       CR739462  mod3 buf idx not incrementing on sleep error recovery
16/12/14   am        CR771799 Don't sleep until QTA clean-up is called
16/12/14   mc        CR770807 [DSDS] IDLE to TRANSFER state transition on GS1 does not complete when GS2 is holding TRM lock for PSCAN
12/12/14   sk        CR763710 Ensure NBCCH is read when consistently colliding with serving PCH or other sub's PCH
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   sjv       CR769423 Ensure l1_idle_mode_isr returns TRUE only when it is safe to release TRM
12/02/14   sp        CR764561 Correct logging of SNRs during CCCH burst metrics
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
10/12/14   ws        CR768376 Q6 compile warning fixes
03/12/14   npt       CR759467 Fix compiler warnings
26/11/14   sp        CR730862:Clear CBCH frames only if next cbch req frame is invalid
25/11/14   cah     CR740519: fix to trigger generic GFW comamnd when state changes and rx aplha needs to be updated.
21/11/14   js        CR745917 Set gl1_hw_first_gap_after_preload flag to abort scripts and set FW load flag to false if there is no async command issued to GFW.
20/11/14   dp        CR759046 Fix debug code so it doesn't crash in unintended scenarios
20/11/14   js        CR646029 : Avoid CM startup in W+G scenario if current QS is more 2300
14/11/14   mko       CR754509 Backing out the CR664288
20/10/14   sk        CR735697 Ensure GL1 doesnt toggle between SI3/SI4 on TRM denial, and does explicit TRM release before making reservation in non-G only mode
18/11/14   sj/ap/ch CR757734 When transitioning to transfer from Idle, request TRM after Idle abort finishes, and deact FW/RF
11/11/14   dp        CR 750446: add debug code to crash earlier when stuck leaving idle
01/11/14   df        CR748602 Rework and enable adaptive timeline
07/11/14   js        CR751659 Clear any inactive CBCH activity in FM table, before setting new CB activity.
28/10/14   nm        CR747048 Clear cbch frames on Idle abort
27/10/14   ss        CR746450 At idle abort check qta active using gl1_hw_qta_gap_active
10/10/14   js        CR736278 : Avoid setting of qta in progress flag explicitly,it will be properly set when TRM is released.
30/10/14   cs        CR701894 Enhanced Page Detection Support
29/10/14   ab        CR745429 - Featurization correction for PCHXFER for SS / CR merges.
20/10/14   mko       CR725933:Sending UNKNOWN_BLOCK_QUALITY when page is missed due to conflict with RRBP transmission, sothat page sharing works
22/10/14   cah       CR737592: optimisation for persistent denial based on Scell RSSI and SnR
21/10/14   fan       CR734490 Rework the wake up recovery mechanism
21/10/14   sjv       CR743461 Reset persistent denial when exiting Idle mode
17/10/14   ws        CR734358 Fix Scheduling bug in DRX CB where CBCH was not getting tasked to gdriver
08/10/14   ak        CR730234 Sleep debug code tidy up
24/09/14   zf        CR727923 Clear the EXTENDED_PAGING status when we enter idle mode.
26/09/14   fan       CR730788 Moving the start RF timer next to setting RF_WAKEUP flag to the commit mask
                              and correctly abort PWR mon
01/10/14   df        CR611845 SBD interface updates for missed wakeup
24/09/14   js        CR716049 Clearing QTA flag after calling TRM release
22/09/14   cah       CR723144 Bolt 2.0 DRDSDS - W+G - G2G interband ncell monitor activity denied while in W+G FC mode
17/09/14   ws        CR 694807 Reduction in High occuring F3
18/09/14   am        CR725978 Enable rach_ready_to_send for GPLT
15/09/14   ss        CR717111 Check if power monitor scheduled on GSTMR of transfer sub before exiting idle mode.
16/09/14   cah       CR718841 GL1 missing support for RFSW changes for RXFE bus contention
12/09/14   fan       CR636420 Run Sleep commit after GSTMR ISR at wakeup
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
09/09/14   pa        CR719024 Implementation of Hybrid Tuneaway Fallback Algorithm
08/09/14   nm        CR627338 Do not wait for DRX cycle for re-requesting TRM if in Page -reorg mode when W is in BG traffic(supporst TA )
04/09/14   am        CR682760 Addressing KW GL1 P1 errors
03/09/14   sp        CR715190 Function call to reset GBTA state from Active to Allowed/None
23/08/14   sk        CR713574: Ensure Gl1 doesnt go to sleep when IRAT activity ongoing
25/08/14   df        CR660576 Enhanced timeline debug
18/08/14   df        CR708449 Correct minor propagation error
14/08/14   js        CR654246 Avoid G2W measurements in extended BCCH scenario
05/08/14   zf        CR704506 Tick next_cell_bcch after PCH decode
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
04/08/14   nm        CR675844 Clear decoding_cbch flags correctly on all CB data(good/bad) /missing indications
04/08/14   nm        CR662106 not releasing TRM and going to sleep during CB activity after receiving CBCH deact req
28/07/14   ssh       CR700687: Handle X2G start up from warm up ISR
24/07/14   zf        CR500399: Don't schedule PCH receive between PCH and extended PCH
10/06/14   br        CR677468 Non drx timer FN was not adjusted /cleared During Reselction if non drx timer is running
16/06/14   nk        CR678832 Override TRM reason to QTA compatible reason one and avoid updating TRm reservation for TA
19/05/14   br        CR665231 Race condition between handling of RACH in task and is state moving to GTOW_SRCH_STATE_WAIT state
21/07/14   ss        CR696060 Clear idle_rx_in_progress while aborting RX CCCH
16/05/14   hd        CR664288: Not to schedule CBCH Rx when it is conflicting with NBCCH
20/07/14   mko       CR688547:Gl1 should Reset idle_update_afc_tt  flag when IDle abort triggered in QTA GAP
10/07/14   js        CR593074- ROTA/QBTA Feature implementation
11/07/14   sjv       CR676103 Further fixes on top of CR658307 for Extended Paging & combined channel scan.
13/06/14   ka        CR675110:TRM is released in extended paging scenario
13/06/14   ka        CR679410 EXT_Paging n+1 block calculation mistake for non combined MF with 1 AGCH Block
02/07/14   aga       CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
30/05/14   ws        CR657674 Data + MMS Phase 1
10/06/14   jj        CR 668965 merge ASDIV from DI.3.0 to BO 2.0
24/06/14   cs        CR684714 Add back in functionality removed for initial updated TRM API support
30/05/14   pg        CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
12/05/14   dv        CR658307 Crash due to no release of lock in QTA when in ext-paging & combined channel
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
08/05/14   br        CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
08/05/14   nk        CR660831: GSM Layer 1 compares wrong enum values for internal TRM_DENIAL status. Change TRM_DENIAL enum to TRM_NOT_GRANTED to aoid any confusion going forward.
28/04/14   dv        CR655739 ensure GFW does not sleep before receiving STOP CNF from G2X RATS
25/04/14   pg        CR649948 Re-instate CBCH Rx start code (lost during merging)
11/04/14   cja       CR644441 Bolt: Preload script for QTA
09/04/14   sp        CR635810:Don't release TRM lock for access when the lock is granted after request_and_notify(), Correct/620698
08/04/14   cs        CR638933 Allow equal priority event to be booked when suspend is set
08/04/14   sp        CR639742: Update Idle activitity to QTA reason from multisim trm manager
07/04/14   cja       CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
12/02/14   jj        CR 596556 Returning switch permission to LTE when GSM is in OOS in SGLTE mode
29/03/14   mc        CR639462 : GBTA : Ext BCCH is not read after reading BCCH
19/03/14   ws        CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
25/03/14   sp        CR631846: Attempting page decode with 2,3 and 4 frames if ISR OVERUN is detected
14/03/14   ab        CR 458061  DSDS : GL1 should abort TBF suspension when just initiated to pending PS activity
13/03/14   ap        CR629792  GSM acq takes really long time after power up when X rat in BG Traffic
11/03/14   sk        CR627248 Ensure Extended paging block is read correctly in idle mode
03/03/14   am        CR630494 Don't release FW in the same frame as FW setup
12/03/14   ssh       CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
27/02/14   gk        CR604978: Reset Idle DRX reference when activity is denied by TRM.
25/02/14   am        CR615566 Activities should not clear their DRX entries based on reference
21/02/14   sk/smd    CR620698:Don't release TRM lock for access when the lock is granted after request_and_notify()
2/17/14    ab        CR612460 Fix race condition for power monitor update
21/11/13   dv        CR579907 Back out CR 519537
13/02/14   ap        CR612587: Ensure call RF exit GSM mode when releasing TRM in DSDA mode
11/02/14   am        CR499225 Do not tick cbch if idle_data_ptr is NULL
20/01/14   gk        CR585299 GBTA ACTIVE flag not being reset at time of CCCH abort
16/01/14   sp        CR597862: GL1 should not update page mode if imsi not valid
04/02/14   am        CR604532 Don't release FW in the same frame as FW setup
24/01/14   dv        CR607109: W2QTA: RXAGC floored still occasionally seen
20/01/14   ab        CR582408: DSDA - Update Rx Power in IDLE and Do not Update Power when RX OFF
09/01/14   sp         CR597391  - corrected regression of CR/588107
07/01/14   dg        CR593574 GL1 to send Block Quality Ind to RR for the paging
                     blocks which belong to its group in RACH mode
06/01/14   sp         CR596088: Retain GBTA Active State for Extended Paging.
14/01/14   jj        CR600114 tx- diversity in Dime SGLTE part 2
03/12/13   ab        CR582408: DSDA - Update Rx Power in IDLE and Do not Update Power when RX OFF
07/01/14   nm        CR596568 UE going to ReOrg mode after Page decode due to incorrect condition in GL1
06/01/14   sk        CR593161 Ensure CB reservations not made when other SUB is in Data
03/01/14   mko       CR586827 Use combination of pre-MIMO and post-MIMO SNR to compute the SNR_is_bad flag when SAIC is enabled
03/01/13   gk        CR586607: Correct the condition to release DRX reference, so that its not removed twice in the same frame
02/01/13   dv        CR592228 RxAGC flooring after few QTAs
26/12/13   dv        CR593654 Crash in T+G DSDS Extended paging mode as TDS does not support QBTA
05/12/13   mko       CR584654: Gap to next activity is corrected for extended paging when when hyperframe wrap around happens
23/12/13   nm        CR589209 Check the next ccch block gap in case of sdcch combined channel comb correctly
                            this code was removed with power otps..brought back again.
22/12/13   sp        CR588107: Ensure invalid metrics are not considered in afc/tt/agc/snr averaging/logging.
17/12/13   sk        CR586741 Update TRM reservation to QTA compatible one if WCDMA is in QTA mode
09/12/13   sk        CR586591 Ensure GBTA status is not reset when GBTA is active in TSTS mode
16/12/13   dv        CR569122 High missed page rate due to W ID search
05/12/13   mko       CR584654: Gap to next activity is corrected for extended paging when when hyperframe wrap around happens
12/12/13   ab        CR586481: GBTA-Idle abort after Power Monitor for IDLE is completed by Transfer SUB
05/12/13   mko       CR584654: Gap to next activity is corrected for extended paging when when hyperframe wrap around happens
04/12/13   ap        CR584084 Addressing WCDMA reconfirmation search whilst GERAN in Transfer
26/11/13   sai       CR578912: Clear G2W reconf search state if incorrectly left pending when no G2W reconfirmation
                              search is in progress.
26/11/13   sk        CR580900 Ensure priority inversion works with power optimizations
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
22/11/13   sk        CR576246 Ensure Clenup scripts status not cleared while populating startup scripts inside the QTA gap
20/11/13   zf        CR579309: Don't go to sleep when wakeup not completed.
19/11/13   ssh/ip    CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   nm        CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
12/11/13   sai       CR 571606: Multispace G2W abort tracking variable,ensure G2W TA abort happens
                     only in W + G mode.
05/11/13   cja       CR571501 In DRX on do not turn Tx off if leaving idle in the same frame
25/10/13   zf        CR537690: Reduce G wakeup timeline for MS mode
31/10/13   sai       CR567424: Enhance G2W TA suspension mechanism.
30/10/13   aga       CR567773 clear G2W TA flags at idle init time
26/10/13   js        CR564876 In QTA while reading extended paging block G sub sets the paging message to skip resulting to extend TRM reservation duration.
24/10/13   sk        CR566281 Correcting the merge error
02/12/13   ws        CR580132 - Ensure GSM Rx is scheduled before we build cleanup script for QTA
12/02/13   sai       CR 571606: Multispace G2W abort tracking variable,ensure G2W TA abort happens
                     only in W + G mode.
26/11/13   cja       CR455527 Remove featurisation from files in API directory
11/26/13   aga       CR567773 clear G2W TA flags at idle init time
11/26/13   js        CR564876 In QTA while reading extended paging block G sub sets the paging message to skip resulting to extend TRM reservation duration.
19/10/13   sk        CR563877 Ensure BCCH reads are given priority over PCH when doing reselection and consider early wakeup time into TRM reservation
31/10/13   sai       CR567424: Enhance G2W TA suspension mechanism.
13/11/13   ap        CR562878 FR 17482: Page Blocking Management in Multi-SIM Single Radio Devices
05/11/13   cja       CR571501 In DRX on do not turn Tx off if leaving idle in the same frame
08/10/13   sp        CR561007: Start SC meas based on mode
26/11/12   jj        CR 424199 Adding Scell measurement for tx-diversity when BA list is empty
02/11/12   jj        CR 402579 Support of Tx Diversity in GERAN.
17/10/13   js        CR536512: Keeping GL1 awake while it is leaving idle mode in DS mode
17/10/13   sk        CR562312 Ensure CM startup not issued from the warmup gstmr isr
10/10/13   js        CR557173 Corecction in calculation of gap to next activity in non combined CCCH/SDCCH
27/09/13   nm        CR548977: Increase power monitor number to 5 per PCH frame & remove conditional compilation Opt19
                     Also check fow IRAT active when goping to sleep in Idle receiving after FTM
08/10/13   js        CR519541: Add wakeup from sleep during QTA support in idle ISR state mc,block sleep in QTA mode
05/10/13   sk        CR554665 Backout the power opt temperorily(CR537690)
04/06/13   dg    CR490986: Stop sending SYNC_RX_CMD to GFW if wcdma srch id and reconfirm states are not NULL
04/10/13   gk        CR492165: Read Extended Page if the second block of the page is between 42-49 in 51 MF
02/10/13   js        CR545531: Partial QBTA for extended paging
01/10/13   sk        CR552259 CR sync from 1.0/1.1
27/09/13   ab        CR550563 TR 1.1 - GBTA - Make Pseudo TRM_GRANT NULL after activity over.
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
24/09/13   gk        CR549082:Sync from 1.1 and 1.0 to 2.0
09/09/13   ab        CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
06/09/13   ab        CR 536790 GBTA GL1 not updating AFC when doing EXT PCH
04/09/13   pjr       CR538295: Removed CR422884
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
11/09/13   sp        CR539988: Add ERR_FATAL when stuck in Idle Abort loop
10/09/13   nm        CR488624:Clear UL PACCH buffers when aborting DTM mode.
04/09/13   sk        CR537595 Adding an explicit F3 to differentiate xCCH failure due to activity conflict
02/09/13   sp        CR536875: Perform post_idle_init from l1_cbch_tick if invoked earlier than idle activity
29/08/13   gk        CR496693:The drx reference should be removed when the reservation FN has been reached
05/08/13   ws        CR522969 Only Enable Tx when we need need it while leaving IDLE mode
23/08/13   sp        CR529865:  Ensure correct transfer sub is sending the release confirmation, Discard CR515386 changes
21/08/13   npt       CR531633 - Remove FEATURE_DIME_MODEM dependencies
16/09/13   pa        CR539506: Bolt: Added support for GPLT dedicated command via DIAG
20/08/13   ab        CR 529794 GBTA Enhancement: facilitate PAGE decode on Idle SUB
                      when Other SUB is in GBTA and doing NCell decode
15/08/13   sk        CR524039 GBTA bringup changes
12/08/13   sp        CR523830: Clear the fm activity, if we no longer need to read ext_bcch
02/08/13   cs        CR522369 Include residual XO drift handling in CR509747
07/16/13   zc        CR470197 Replace memcpy with memscpy
31/07/13   sp        CR519537: Enable GSTMR F3s
29/07/13   sk        CR519663 Partial QBTA bringup changes
15/07/13   mko       CR508974: Incorrect SNR logging in GSM Idle
12/07/13   cs        CR509747 Apply frequency drift to TRM_DENIED SUB(s)
05/07/13   sk        CR509202 Priority inversion and alternate page skipping changes
24/06/13   cs        Major Trition TSTS Syncup
05/06/13   dv        CR496269 Ensure no G2X active in Idle before running l1_dual_sim_trm_manager
28/05/13   ap        CR461268: DSDS XO management + async time drift problem
29/05/13   am        CR488891 Ensure SCE is stopped on randomm access
23/05/13   ip        CR359541  Option 3 working for G+G and G+W modes
23/05/13   sai       CR489525: Block CBCH activity during WCDMA BG traffic.
23/05/13   jk        CR476088:Delay sending of L1_MAC_SINGLE_BLOCK_SENT till idle init
22/05/13   ap        CR485912: Revised GFW state transitions
22/05/13   ap        CR463960: DSDS: Consolidated extended paging changes:CR362437,372023,380441,304398,304398
22/05/13   ap        CR362437: DSDS: sys info cancelation causes a page loss
22/05/13   mc        CR372023 In L1_IDLE_WAITING do not attempt to get TRM on every block for EXTENDED_PAGING
22/05/13   ap        CR380441 Ensure all PCH - BCCH paging blocks are read in paging reorg and paging mode
                              description is only taken from Paging Request Type messgaes
22/05/13   nt        DSDS CR304398 Correct DSDS handling of extended paging
22/05/13   ap        CR373628 correct gap calculation to next ext/norm bcch read when in extnd paging
21/05/13   am        CR488393 Return DSDS CBCH state to INACTIVE on CBCH abort
10/07/12   mc        CR376315 Prevent new activities from running if a TRM release is pending due to L1 task latency
21/05/13   am        CR374583 GL1 not reserving TRM after CCCH rx abort due to stuck idle_update_afc_tt flag.
01/04/13   ab        CR470239 Reassert CCCH handlers if not installed as part of Idle init.
15/03/13   ap        CR461103 Problem reading all blocks in Non DRX mode
06/06/13   cs        TSTS Updates
17/05/13   kb/npt    CR385548 - Decode PCH after sleep wakeup error recovery.
11/04/13   am        CR473166 Do not release TRM if FW_LOADING is active
03/04/13   am        CR470946 Move to L1_IDLE_MULTI_SIM_FW_LOADING when extended paging is enabled
02/05/13   zf        CR480969: Fix G+G DSDS camping
01/05/13   jk        CR476514: Return correct BSIC in gl1_get_serving_cell_BSIC( )
13/04/13   mc        CR471759 DSDS: QTA Support : Initial code changes & Triton Bring-Up
28/03/13   pg        CR467228 Do not enable Tx when leaving idle for SUSPEND
24/03/13   ws        CR466973 - DSDS BU, don't call gl1_drx_off() when leaving idle mode if in DSDx mode
19/03/12   ss        CR465249 Disable OPT3 for PCH decode
13/03/13   mc        CR459487 : COEX Support - GERAN arbitrator framework
19/02/13   ap        CR 454462 transmitter is not switched on when operating in non dual sim mode
14/02/13   ab        CR 453633 Reset TT update during Idle abort when not reset due to cont CCCH Read .
14/02/13   ab        CR 452602 Regressionof CR 444404 DSDS Delay Idle abort when Rx/monitor/AFC TT update in progress
12/02/13   ab        CR 444404 DSDS Delay Idle abort when Rx/monitor/AFC TT update in progress
11/02/13   ap        CR451388 DSDS Correct use of local_FNmod51 in calculating gap to next activity
07/02/13   ap        CR449859 Always enter GFW state MEAS_ACTIVE for gonly sub else
                              enter GFW ACTIVE state
06/02/13   cs        DSDS CR438977: merged support for CBCH in DSDS mode
02/02/13   ap        DSDS CR445977
23/01/13   mc        DSDS CR332154 - Ensure that a PCH following a BCCH gets decoded upon a missed BCCH block
20/01/13   mc        DSDS CR341520 - Prevent multiple TRM requests for the same DL block
18/01/13   mc        CR437838
18/12/12   og        CR424373. Don't send a tx config command when leaving idle
                     during deactivation.
26/11/12   zf        CR417719: Optimization of Ncell acquisition
14/01/13   npt       Temporary removed SAIC disabled check for opt3
02/01/13   dv        CR432431. Fix to enable GCF 20.10 & TC 6.2.2.3a (G2T) to pass
03/01/13   npt       Reintroduced SAIC disabled check for opt3
06/12/12   npt       Temporary removed SAIC disabled check for opt3
28/11/12   br        CR383165 Removed variable curren_FN in cbch state machine to avoid compilation warning
26/11/12   pg        CR422884: When leaving idle, remove idle handlers before scheduling current frame
21/10/12   dv        CR397922: Fix for 6.2.2.3a G2T TC
23/10/12   npt       CR413635 - Port sleep code for DIME modem
18/10/12   pg        CR405396: Avoid switching off Tx clocks until GFW Tx processing has completed
24/08/11   kb        CR302696 - Added Initialization function to initialize local idle data values
12/06/12   cs        CR358963 In CBCH post tick do not try and release a stale DRX ref
12/06/12   ws        CR368509 - integration bug from previous change ensure
                     l1_smscb_set_first_non_drx_block_read() is called when CBCH TCO is decoded
08/06/12   pa        GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
01/05/12   ws        CR308681 - CBCH integration from MDM6610
04/01/12   cs        CR297422 defer the EXPIRY_IND until after last CB block decode
02/05/12   pg        CR351723: fix compilation errors when FEATURE_GSM_CB is not defined
25/04/12   pa        CR355881: Removed warning.
13/03/12   ws        CR 330862 - Change dedicated mode F3 level's and add sacch burst
                     metrics and compact message metrics log packets
19/04/12   pa        CR349239: Removal of CR349266
19/04/12   cja       CR351581 In FTM do not error fatal if stuck.
13/04/12   cja       CR350770 Set state suspended if suspend search when NO_HPLM_SEARCH
12/04/12   pa        CR351579: G2T: Fix for workaround: Stop scheduling IRAT when BCCH block to read
04/04/12   pa        CR349266: G2T: Workaround for not to do IRAT when there is a BCCH block to read
28/03/12   pa        CR 346062 Workaround for G2T Abort.
15/03/12   cs        CR329173 Ensure DRX is ticked when SKIP_REQ is received
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
20/01/12   cs        CR309232 Always reset CBCH start FN for DSDS when abort is called
25/11/11   ap        DSDS CR321468 DSDS Paging Re-org not able to complete
24/11/11   ap        DSDS CR321379 FW access when FW not loaded when FEATURE_DUAL_GERAN not enabled
17/03/11   cja       CR279518 Add single slot FTM power store for GPRS/EDGE
18/03/11   ws        Added Initial MCPM updates
25/08/11   nt        DSDS CR300961 ADDITION - always perform post idle init
24/08/11   nt        DSDS CR300961 defer the post idle init until the FW is loaded.
29/06/11   nt        DSDS CR289633 Ensure that the CNF is sent after suspension state in BPLMN
15/06/11   cs        DSDS Enable CBCH firmware download
20/05/11   sjw/cs    DSDS: refresh xMSI parameters when G+G optimised mode is enabled
09/05/11   sjw       Use MSG_HIGH instead of MSG_MED for PCH decodes
03/05/11   ap        DSDS CR285091 "PCH Decode" debug not printed
05/04/11   nt        DSDS CR281061 Ensure pending AFC/TT update is performed upon TRM_NOT_GRANTED
19/03/11   cja       CR279518 Add single slot FTM power store for GPRS/EDGE
18/03/11   sjw       DSDS CR276370: ensure that the CCCH callbacks are initialised in L1_IDLE_WAITING
21/01/11   cs        CR246418 Allow disabling of SAIC F/W processing in idle
25/11/10   nt        DSDS CR265734 Move back to waiting upon fm/trm denial
22/11/10   nt        DSDS CR261791 Perform post idle init is other SIM has the lock
19/11/10   nt        DSDS Enhancement to SCE scheduling during Tuneaway
20/08/10   cs        Fix ADS 2.2 compile problem for CR246418
21/07/10   cs        CR246418 Allow disabling of SAIC F/W processing in idle
13/07/10   cs        Updates for CB_DRX support
25/06/10   pg        Make idle_serv_meas_buf non-static for Q6 (required for CR244125)
28/04/10   nt        CR234959 Optimisation to NCELL power monitors in the idle mode
14/04/10   cs/ca     CR225427 Revisit to always double AFC update when SBD
10-03-16   tjw       Renamed UNUSED to __UNUSED_ARG to avoid name clash.
08/03/10   cs        CR225427 Double update AFC loop for successful SBD to improve AFC
                     loop update rate
20/01/10   cs        CR221032 21.3.1 failure with SAIC enabled as AFC/TT not updating
09-10-28   tjw       Clean up medium Lint errors
03/07/09   sjw       Support Extended BCCH parameter for l1_compute_tc()
09/06/09   nf        Added FEATURE_GSM_PCH_SINGLE_BURST_DEC
30/03/09   ws        Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                     to replace FEATURE_GSM_GPRS_MUSTANG
02/12/08   cs        Added in new gl1_hw_clk_ctl.h include from gl1_hw.h
20/11/08   nf        CR149658 clear non-DRX on Idle init
15/09/08   cs        CR130550 Add in the ability to request a higher/lower clock speed if req'd
                     under the FEATURE_GPRS_USES_HIGH_ARM9_CLOCK
21/08/08   nt        Lint
15/05/08   nf        CR117393 fix for NC2 non-drx handling
04/03/08   ap        CR143065 call the function l1_smscb_cbch_expected() without adding 1 to
                     the parameter this_FN for the case when FNmod102 = 7 or 58
20/03/08   agv       Fixed the LCU PMIC Band setup for a Channel Change.
27/02/08   cs        Use an access function in gl1_hw for the g2wIsrLockOut checks
14/02/08   cs        CR136944 Use the new g2wIsrLockOut to ensure no mdsp stall when
                     disabling the WSTMR in dedicated
07/12/07   cs/ip     CR134215 Remove the problem with GL1/WCDMA sleep getting out of synch on
                     early wakeup
17/09/07   agv       CR127820 - iRAT W2G handover AGC setting.
31/08/07   agv       CR127468 Fix for SAIC TT handling in Idle.
02/05/07   ip        CR117884 Added FEATURE_GSM_DISABLE_CHIPXN_DIV4 control
16/05/07   ws        Added FEATURE_GSM_L1_CONTROL_SET_BAND CR 114613
02/05/07   ip        CR117884 Added FEATURE_GSM_DISABLE_CHIPXN_DIV4 control
18/05/07   cs        Fix featurisation for Aries changes
10/05/07   cs        Frequency error update changes for Aries GPS
23/01/07   ws        CR 90771 - Fix frame manager issue with back to back CBCH and PCH
11/01/07   og        Resolve RVCT2.1 compiler warnings.
19/12/06   nt        CR107800: non_drx timer is started only when leaving transfer.
27/10/06   ws        Removed unused Variables
06/04/06   av        Feature SAIC MIMO elements added to the burst metrics.
12/14/05   sv        Support the changed prototype for MPH_DECODE_BCCH_LIST_CNF.
11/16/05   ws        More Lint Cleanup
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
11/07/05   sv        Reset BPLMN state when deactivating GSM.
08/17/05   gfr       Use gl1_msg_init_frame_tick_info to init the ftn structure
08/10/05   sv        Fixed bug due to incorrect EXTENDED_PAGING mode handling.
08/04/05   sv        Fixed bug where we pass incorrect FN to DRX Manager.
08/03/05   sv        No need to pass "gap_to_next_activity" to SCE.
07/29/05   gfr       Support for NPL1 DTM interface changes
06/15/05   sv        Check if BPLMN is running before leaving Idle mode.
05/25/05   dp        Initialized l1_idle_data to NULL in L1_initialize_l1_idle_data
05/23/05   sv        Split the CCCH and CBCH processes. This fixes some bugs related to
                     CBCH scheduling in the Frame Manager.
05/09/05   sv        Cleanup of CBCH abort scenario.
04/21/05   gfr       Improve timing error message
04/21/05   sv        Add support for L1 "Panic Reset"
04/20/05   og        Addition of filtered bep params to the msg metrics log packet.
04/11/05   sv        Update the CCCH gap in Frame Manager when trying to schedule
                     CBCH.
04/01/05   ws        Added Enabling of MM_NON_DRX_MODE
03/11/05   sv        Mainlined FEATURE_GSM_CB, FEATURE_GSM_GPRS_L1, FEATURE_SCE
                     in l1_idle.c + code cleanup.
02/11/05   bm        Support for Background WCDMA PLMN Scan
01/26/05   sv        Add support for Frame Manager.
10/01/04   bm        Enhanced Parallel BCCH decode support
01/10/05   dp        Lint cleanup
09/12/04   og        Fix to tx power control by tracking c value on ccch.
12/02/04   gw        Make sure that l1_serving_cell_meas.no_of_measurements is
                     updated since this is used by logging functions.
10/20/04   sv        Optimization of Ncell SCE awake time
10/12/04   ws        Added Multiblock allocation for EGPRS
08/26/04   sv        Lint cleanup
08/25/04   gsc       Fixed a bug where we were sending CB_MISSING_BLOCK_IND to
                     CB task even though CB was disabled.
08/23/04   dp        Fixed corner cases in background PLMN aborts
08/09/04   gsc       Added support for sending CB_MISSING_BLOCK_IND to CB task
                     when L1 pre-empts CB decodes.
06/15/04   gfr       Function name changes and set the ncell block start for CBCH
06/12/04   gfr       Control slew rate limiting in PL1.
06/30/04   hg        Added missing FEATURE_GSM_CB
06/22/04   dp        Lint cleanup
06/21/04   gsc       Incorporate changes to l1_sci_tick_idle interface. Correctly
                     set gap_to_next_activity and gap_to_next_cbch_activity in
                     L1_IDLE_WAITING state.
06/18/04   gsc       CB + BHPLMN search fixes
06/17/04   dp        Fixed background PLMN search which was broken
06/11/04   gsc       Do not set gap_to_next_activity to 0 when in L1_IDLE_WAITING
                     in case CBCH decode is pre-empted by other activity.
06/11/04   gsc       Use different AGC struct for CBCH on SDCCH_8.
06/11/04   gsc       Corrected fix for ncell bcch abort problem.
06/10/04   gsc       Fixed ncell bcch abort problem. Made changes to CB
                     scheduling in SDCCH_8 configuration.
06/04/04   sv        Check and set "L1_confirm_ID" to appropriate enum values.
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
05/24/04   gw        Calculate gap to next activity correctly when FN wraps
                     around.
05/14/04   gsc       Added support for properly aborting CBCH and pre-empting
                     CBCH decodes during Background HPLMN searches.
04/21/04   gw        Increased time to output debug messages before ERR_FATAL.
04/02/04   gw        Removed some lint errors.
03/26/04   gfr       Time tagging support.
03/24/04   gsc       Correctly merged backgroud HPLMN search code so that it
                     works well with Cell Broadcast.
03/22/04   gfr       Detect if too many PCH decodes fail and RR does not react.
03/18/04   dlh       Added handling for l1_idle.cancel_non_drx flag used for when
                     GRR wants to take L1 out of Non_DRX when doing surround update.
03/18/04   ws        Added support in NC2 non drx timer
03/15/04   gsc       Restored CB code lost during Background HPLMN search code
                     merge.
02/25/04   gw        Lint clean up.
02/09/04   dp        Support for background HPLMN searching
01/28/04   gw        Changes to support reselection optimizations.
01/14/04   gw        Changed DEBUG_BIG_TE_SNR_THRESHOLD from 0dB to 2dB.
01/13/04   gsc       Merged ARFCN_T changes to Cell Broadcast code.
01/09/04   gw        Merged in support for new SI reading API from branch.
11/05/03   gfr       Support for quad-band.
11/07/03   gw        Make sure that serving cell running average window is the
                     correct length even with early paging decode.
10/09/03   gsc       Added GSM Cell Broadcast support under FEATURE_GSM_CB
09/03/03   gw        Change to handle paging mode SAME_AS_BEFORE correctly when
                     previous mode was EXTENDED_PAGING.
08/29/03   gw        Addded code to output F3 message with SNRs for each
                     received block.  Also added some debug messages for
                     paging mode.
08/28/03   ws        Added debug messages for MM non-DRX mode.
07/29/03   gw        Renamed DEBUG_L1_STATE_LOG to FEATURE_GSM_L1_STATE_LOG.
07/16/03   bk        Added ERR_FATAL when stuck in Idle Abort mode.
07/15/03   gw        Revamped error messaging on Idle Abort.
07/14/03   gw        Changed call to l1_sci_monscan_go() to l1_sci_go().
07/11/03   gfr       Lint cleanup.
07/11/03   gw        Added checking for divide by zeros.
06/26/03   pjr       Added updating of parameters due to MPH_GPRS_L1PARAM_UPDATE_REQ
                     being received.
06/19/03   gw        Added more info in debug code added in previous revision.
06/19/03   gw        Added code to help debug situations where we can't abort
                     Idle Mode.
05/30/03   dp        Merged Sanyo fix for 26.6.2.2 to mainline
05/23/03   gw        Change to calculate_gap_to_next_activity() to allow early
                     decode to work properly for BCCH.
05/21/03   gw        Changes to support early paging decode.
05/16/03   gw        Removed featurization around include of gl1_sys_params.h.
05/13/03   ws        Corrected mm non drx mode to continue after non-drx timer expires
05/07/03   ws        Downgraded MSG_ERROR to MSG_LOW in gl1_get_serving_cell_arfcn()
                     as function is also used in packet transfer mode
05/02/03   gw        Merged in power transfer changes from branch.
04/28/03   gw        Added call to gl1_drx_off() when leaving Idle Mode.  Changes
                     for preserving power measure info between modes.
04/16/03   ws/mk     Added peripheral SBI control for GPRS.
04/04/03   gw        Removed call to l1_sci_paging_block_tick() when not our own
                     paging block.  Added functions to get serving cell BSIC
                     and rx_power.
04/03/03   drl       Fixed MSM6300 releated warning.
04/02/03   gw        Changes to reflect modifications in monscan module.
03/10/03   dp        Cetecom fixes: To handle SAME_AS_BEFORE when previous mode
                     is EXTENDED_PAGING.     NORMAL_PAGING is now no longer
                     controlled by L1.
02/25/03   gw        Removed call to gl1_drx_reset() when initializing Idle Mode.
                     This is required to allow the Tx chain to remain on (for a
                     short time) when coming from Transfer Mode.
02/12/03   gw        Changes to support FEE timing error functionality.
02/10/03   ws        Fixed bug with mm non drx timer mode
01/28/03   tb        Added call to l1_sci_check_ok_to_sleep.
01/10/03   gw        Changed handling of extended paging in L1_control_paging_mode
                     to reflect getting CCCH data one frame earlier.
01/02/03   gw        Modified call to gl1_msg_cfg_dl_ccch() to reflect addition
                     of another parameter.
01/02/03   gw        Optimizations for sleep timeline.
12/05/02   gw        Added support for decoding SI 2quater on extended BCCH.
12/05/02   ws        Moved gl1_reset() into packet transfer INIT state
12/04/02   ws        Re-enabled idle=TRUE, gl1_msg_reset() in GPRS as covered by TRANSFER_COMMAND
12/03/02   mk        Disabled idle=TRUE, gl1_msg_reset() in GPRS.
11/27/02   pjr       Added idle=TRUE when leaving_idle_mode and main_command is
                     set to L1_TRANSFER_COMMAND
11/27/02   ws        Added MM non-drx mode support
11/22/02   mk        Added multiband AFC support
11/19/02   mk/gw     Updated non drx timer support.
11/18/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/15/02   dp        Added gl1_get_serving_cell_arfcn()
11/14/02   ws/gw     Added gw changes to support non drx timer
11/03/02   tb        Additions to support INTERRAT_GTOW
10/30/02   gw        Changes to optimze sleep timeline: Corrected calc of gap
                     to next paging block in calculate_gap_to_next_activity().
                     Added re-check of sub-state at end of L1_idle_mode_ISR().
10/14/02   ATM       Added central get and set FN routines
10/10/02   mk        Merged GPRS release 1 phase1 code to baseline.
10/02/02   dp        Fix to abort idle mode on state transition to
                     L1_ENTER_NULL_COMMAND
09/18/02   dp        Fix to continue decoding paging blocks if the SIM is
                     invalid
09/17/02   plm       Removed local variable message_type from function
                     L1_control_paging_mode() which was declared but not used
                     to eliminate compiler warning.
09/03/02   dp        Fix to handle corner cases in extended paging.Fixed
                     problem with getting stuck in "Idle abort"
08/29/02   gw        Mainlined code to print out error message on large timing
                     error.
08/28/02   mk/dp     Cleaned up L1_control_paging_mode().  We now use the
                     return status to filter fill pages in L1
07/11/02   gw        Changed reference to power_dBm_x16 in metrics report to
                     pwr_dBm_x16.
06/27/02   dp        Added support to handle leaving idle during
                     L1_FIND_SCH_COMMAND.  Temp bumped up msgs levels for the
                     BCCH/PCH decodes
06/27/02   gw        Modified serving cell power measurement averaging to use
                     l1_running_avg_... functions.
06/10/02   gw        Moved calls to gl1_drx_tick to l1_tdma_1_ISR.  Removed
                     calls to gsleep_xxx() functions - these are now accessed
                     through the msg and hw layers.
06/06/02   mk        Support for aborts on all channels.
05/21/02   gw        Added variable idle_monitors_in_progress instead of
                     counting monitors in idle_rx_in_progress.  Added "Stopping
                     Idle" message.
05/10/02   tb        Code to support L1_DEACTIVATE_COMMAND.
05/09/02   JC        Fixed MPH_BLOCK_QUALITY_IND reporting on good blocks.
05/09/02   JC        Fixed metrics logging s.t. burst metrics and message
                     metrics are logged as separate packets; ensures more
                     trust-worthy logging.  Demoted successful decode MSG
                     level and elevated bad decode MSG to ERROR.
05/02/02   JC        Fixed casting error in idle_average_serv_meas.
05/01/02   gw        Changed type of total variable in idle_average_serv_meas()
                     from dBm16_T to int32, to prevent underflow.
04/30/02   gw        Replaced call to L1_send_PH_DATA_IND with call to
                     L1_send_sysinfo_PH_DATA_IND so we can send TC to RR.
04/22/02   gw        Improved averaging of serving cell measurements.
04/19/02   ATM       Adding support for LOG_GSM_RECEIVE_METRICS_C packet
04/18/02   mk        Sending MPH_BLOCK_QUALITY_IND only for own paging block.
04/16/02   mk        Re-enabled SCE done status. Fixed reselection bug
                     in l1 task.
04/15/02   mk        Added link failure detection support using
                     MPH_BLOCK_QUALITY_IND indication on xcch.
04/15/02   mk        Disabled SCE done status, doesn't work reliably.
03/22/02   JC        Cleanup for RXLEV reporting.
03/05/02   JC        Added support for RXLEV reporting during IDLE to meet
                     C1 criteria in GSM05.08.
02/19/02   gw        Changes to support sleep.  Featurized by GSM_SLEEP.
02/14/02   gw        Added call to gl1_msg_reset() to support AGC.
                     Changed IDLE_SCE to FEATURE_SCE.
02/07/02   gw        Added support for metrics on back-to-back blocks and
                     streamlined multiframe state machine function.
02/05/02   JC        Interface changes to support AGC design. Removed
                     write_sig_gains() function. Mainlined IDLE_METRICS.
02/01/02   mk        Updated to support latest afc/tt algorithms.
01/16/02   mk        Added support for afc/tt.
12/21/01   gw        Extensive changes to support DRX.  Extracted two sections
                     of code from L1_idle_mode_ISR to create the functions
                     calculate_gap_to_next_activity and
                     idle_multiframe_state_machine.  Changed logic used to
                     calculate gap to next activity to simplify and to fix bugs.
                     Introduced two new substatesfor Idle Mode -
                     L1_IDLE_RECEIVING and L1_IDLE_WAITING.
12/18/01   dp        Changes to use new acquisition API for SC acquisition and
                     to abort power measurements and SC operations before leaving
                     idle mode
12/11/01   gw        Changes to match new interface for getting debug (logging)
                     data.
11/29/01   mk        Disabled MPH_SERVING_IDLE_MEAS_IND.
11/29/01   mk        Temporarily disabled scell metrics using IDLE_METRICS.
11/27/01   mk        Added L1_IDLE_INIT, ..._RUNNING, modified
                     ccch_rx_callback() to send data up for all blocks. Disabled
                     ncell monitoring for now. Misc cleanup.
11/9/01    bk        Updated NPL1 FTN block info to conform to the interface change
10/31/01   dp/gw     Incorporated changes due to new CCCH decode_rpt format.
                     Added support for logging featurized by IDLE_LOG.
10/23/01   gw        Modified scheduling of monitors so that they occur in the
                     frames of our own paging block.  Fixed bug in
                     ccch_rx_metrics_callback.
10/16/01   gw        Removed conversion to dBm from ccch_metrics_callback().
                     Changed all references to byte to uint8.
10/10/01   gw        Added support for monitoring and Ncell reception using
                     message layer functions.
09/27/01   gw        Changes to match new gl1_msg_rx_ccch interface allowing
                     passing of the channel type.
09/25/01   mk/gw     Converted to message/frame layer interface.
08/31/01   JC        Removed obsoleted compile switch and added std QCT
                     fcn header blocks.
08/17/01   mk/gw     Added Ncell support, featurized via MONITOR_DEBUG.
05/13/01   JC        Replaced frame number variables.
05/05/01   JC        Initial version.  Masked BSIC with 0x07 to obtain TSC.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "l1_drx.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_sc_int.h"
#include "gl1_msg_g.h"
#include "gl1_sys_algo.h"
#include "geran_msgs.h"
#include "l1_log.h"
#include "l1_utils.h"    /* for gl1_get_FN( gas_id ) */
#include "l1_sc.h"
#include "gl1_sys_params.h"
#include "l1_fm.h"
#include "l1i.h"
#include <stringl/stringl.h>

#include "l1_task.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_task.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"

#include "l1_smscb.h"
#include "vtmrs_g.h"
#include "mdsp_dtm.h"
#include "mdsp_intf_g.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "ftm.h"
#include "l1_sc_irat.h"

#include "gl1_hw_tcxo_mgr3.h"
#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT */
#include "gpl1_grm_intf.h"
#include "l1_smscb.h"

extern void l1_state_log_dump( gas_id_t gas_id );

#ifdef FEATURE_GSM_QSC_TX_DIV
extern boolean qsc_got_switch_control[ NUM_GERAN_DATA_SPACES ];

//extern void gl1_trm_ant_switch_set_control ( trm_ant_switch_set_ctrl_enum control_req_type );
#endif /* FEATURE_GSM_QSC_TX_DIV */

#include "gpl1_dual_sim.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#include "gl1_arbitrator_cxm.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include "geran_multi_sim.h"

#ifdef FEATURE_GPRS_GBTA
extern boolean ignore_xcch;
#endif /* FEATURE_GPRS_GBTA */

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#include "gl1_arbitrator_utils.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include "gl1_hw_sleep_ctl.h"
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */
extern uint16 gl1_sleep_cnt[NUM_GERAN_DATA_SPACES];
extern gl1_hw_sleep_type gl1_hw_sleep_struct[NUM_GERAN_DATA_SPACES][NUM_OF_GL1_DEBUG_CYCLES];

extern uint32 gl1_hw_wkupmgr_wakeup_point_fn[NUM_GERAN_DATA_SPACES];

/*===========================================================================

                     LOCAL DEFINES

===========================================================================*/


/* TODO - this value needs to be linked to the GPRS function that
 * decides whether the starting time has passed or not.
 */
#define L1_IDLE_STARTING_TIME_LEAD_TIME 4 // was 3 /* frames */

#define DEBUG_BIG_TE_SNR_THRESHOLD 256 /* same as TT_SNR_THRESH */

#define SNR_BAD_THRESHOLD_dBx16    32 /* dBx16 => 2 dB */
#define SAIC_SNR_BAD_THRESHOLD_dBx16    64 /* dBx16 => 4 dB */

/* Keep track of how many PCH blocks failed in a row to ensure that RR is
   paying attention */
#define L1_MAX_FAILED_PCH_DECODES 180  /* Typically 3 minutes of failures */
#define L1_MAX_FAILED_PCH_DECODES_SC_DEBLACKLIST  4
#define L1_MIN_GOOD_SCELL_RX_LVL  (-100*16) /* -100 dBm */

#define FIRST_CCCH_BLOCK_FN_MOD51 6
#define SKIP_BCCH_BLOCK 4
/* threshold set to determine the MIN amount of time until the next activity */
#define FRAME_ACTIVITY_GAP_THRESHOLD 5

// maximum number of frames allowed for idle abort before crash.
#define L1_IDLE_ABORT_MAX_FRAMES    20

// maxi,um number of frames allowed to be in QTA before crash
#define L1_MAX_QTA_FRAMES        50

/* From 24.007 - 11.2.3.1.1 Protocol discriminator
  0 1 1 0 radio resources management messages
*/
#define  RR_MANAGEMENT_MSG 0x06
/* Paging and Notification messages:
   0 0 1 0 0 0 0 1  PAGING REQUEST TYPE 1
   0 0 1 0 0 0 1 0  PAGING REQUEST TYPE 2
   0 0 1 0 0 1 0 0  PAGING REQUEST TYPE 3
   0 0 1 1 1 1 1 1  IMMEDIATE ASSIGNMENT
   0 0 1 1 1 0 0 1  IMMEDIATE ASSIGNMENT EXTENDED
   0 0 1 1 1 0 1 0  IMMEDIATE ASSIGNMENT REJECT
*/
#define PAGING_REQUEST_TYPE_1         0x21
#define PAGING_REQUEST_TYPE_2         0x22
#define PAGING_REQUEST_TYPE_3         0x24
#define IMMEDIATE_ASSIGNMENT          0x3F
#define IMMEDIATE_ASSIGNMENT_EXTENDED 0x39
#define IMMEDIATE_ASSIGNMENT_REJECT   0x3A

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define WAITING_FOR_TRANSFER_TRANSITION \
((l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND) && \
(l1_tskisr_blk->current_params.L1Data.pIdle_data->starting_time_valid))

#define MULTI_SIM_CBCH_FW_LOAD_GUARD 10
#define MAX_CCCH_RX_INSTANCES 2
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#define L1_IDLE_SAIC_DISABLE_TIMEOUT  50000 /* ms = 50 secs */

#define BLOCK_ALIGN_OFFSET 2
#define PRE_PROGRAMMING_FRAMES 1
#define IDLE_TA_DURATION 20

#define L1_GAP_TO_TUNEAWAY_FN  6

/* consider upto 500 frames away from cancelled ccch */
#define MAX_DIST_FROM_CAN_CCCH 500

#define L1_DURATION_CBCH         8
#define L1_MIN_DURATION_CBCH     2
uint32  gl1_non_drx_wait_frames_max[NUM_GERAN_DATA_SPACES]= { INITIAL_VALUE( MULTIFRAME_51) };

/* Minimum required gap for g2x bplmn to start 4.615 * 65 = 300 ms till next CCCH activity */
#define MIN_REQUIRED_FRAMES_FOR_IRAT_PLMN_SEARCH 65

/*===========================================================================

                            LOCAL STORAGE

===========================================================================*/

boolean  next_block_ccch_rx = FALSE;

rr_l1_epd_preference_t epd_preference[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(RR_L1_EPD_ON_PREFERRED)} ;

const uint8 paging_frame[9] = {6,12,16,22,26,32,36,42,46};
int8 find_ota_cbch_offset(uint32 offset,gas_id_t gas_id);

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

#ifdef FEATURE_GSM_QSC_TX_DIV
int32 type2_alg_antenna_to_use [NUM_GERAN_DATA_SPACES]= { INITIAL_VALUE( 1 ) };
boolean  pch_crc_failure [NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
int32 power_idle_average [NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
int16 avg_cntr [NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
extern int16  idle_mdm_threshold[NUM_GERAN_DATA_SPACES],gsm_idle_sens[NUM_GERAN_DATA_SPACES];
int32 power_idle_average1[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
int32 power_idle_average2[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
int32 power_idle_average3[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
int32 power_idle_average4[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
static void clear_idle_power_avg_buffer (gas_id_t gas_id );
#endif /* FEATURE_GSM_QSC_TX_DIV   */



/* this array is used to convert nc_non_DRX_period to frame ticks */
uint16 nc_non_DRX_period[8] = {
0,     /* 000 no non DRX mode  */
52,    /* 001 = 52 frame ticks */
104,   /* 010 = 104  "     "   */
156,   /* 011 = 156 etc */
208,   /* 100 */
260,   /* 101 */
312,   /* 110 */
416    /* 111 */
};





static l1_local_idle_data_t  l1_local_idle_data[NUM_GERAN_DATA_SPACES];

l1_local_idle_data_t* l1_get_l1_local_idle_data(gas_id_t gas_id)
{
    l1_local_idle_data_t *l1_local_idle_data_p = &l1_local_idle_data[gas_id];
    
    return l1_local_idle_data_p;
}


/*===========================================================================

                     PROTOTYPES FOR LOCAL FUNCTIONS

===========================================================================*/
void    write_arfcns(ARFCN_T rx_arfcns[], ARFCN_T arfcn );
void    L1_suspend_background_HPLMN_search( gas_id_t gas_id );
void    L1_background_HPLMN_search_suspend_cb( gas_id_t gas_id );
boolean L1_background_HPLMN_search_running( gas_id_t gas_id );
background_HPLMN_activity_type L1_get_current_background_HPLMN_search_activity( gas_id_t gas_id );

void l1_tick_cbch( gas_id_t gas_id );
void l1_post_tick_cbch( gas_id_t gas_id );
void l1_clear_cbch_activity(gas_id_t gas_id);

boolean l1_is_page_sharing_enabled( void );

boolean l1_get_epd_pref(gas_id_t gas_id);
void  l1_set_epd_pref(rr_l1_epd_preference_t epdpref , gas_id_t gas_id);
extern uint32 calculate_gap_to_next_page( idle_data_T  *idle_data_ptr, byte   paging_mode,gas_id_t gas_id );
static void ccch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void ccch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );
static void cbch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void cbch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );
static void cbch_aborted( void* dummy, gas_id_t gas_id );
static void cbch_abort_callback( gas_id_t gas_id );
static void l1_run_cbch_state_machine( idle_data_T  *idle_data_ptr, gas_id_t gas_id );
static void l1_initialise_cbch_state( gas_id_t gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static uint32  calculate_gap_to_next_activity( idle_data_T  *idle_data_ptr, uint32 frame_offset, gas_id_t gas_id );
static uint32 l1_idle_calculate_gap_to_next_activity( idle_data_T  *idle_data_ptr, uint32 frame_offset, gas_id_t gas_id );
static boolean is_own_page_read( idle_data_T *idle_data_ptr, uint8 missed_frames, gas_id_t gas_id );
static uint32  gpl1_multi_sim_get_next_block_start_gap( idle_data_T *idle_data_ptr, gas_id_t gas_id);
#else
static uint32  calculate_gap_to_next_activity( idle_data_T  *idle_data_ptr, gas_id_t gas_id );
static uint32 l1_idle_calculate_gap_to_next_activity( idle_data_T  *idle_data_ptr, gas_id_t gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
static uint32 l1_idle_wmgr_modify_gap_for_ccch( uint32 gap, idle_data_T  *idle_data_ptr, uint32 frame_offset, gas_id_t gas_id );

static boolean idle_multiframe_state_machine( idle_data_T  *idle_data_ptr, gas_id_t gas_id );

static boolean is_cbch_rx_block( idle_data_T *idle_data_ptr, int32 frame_offset, gas_id_t gas_id );
static dBx16_T fast_average( dBx16_T*, uint32 num );

static void l1_idle_saic_disable_timer_handler( unsigned long gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static uint8 l1_get_page_skipping_factor( idle_data_T  *idle_data_ptr , gas_id_t gas_id);

#ifndef FEATURE_GPRS_GBTA
static uint32 calculate_gap_to_next_pch_frame( idle_data_T  *idle_data_ptr, gas_id_t gas_id );
#endif

#define L1_PAGE_SKIP_FRAMES_GAP 65

#define GET_PCH_GAP_DIFF( a, b ) \
   ( (a) > (b) ) ? \
      (((a) - (b))) : \
      (((b) - (a)))

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

void l1_set_asynch_int_possible( gas_id_t gas_id );
boolean l1_get_non_drx_timer_flag(gas_id_t gas_id);

/*===========================================================================

                     PROTOTYPES FOR EXTERNAL FUNCTIONS

===========================================================================*/
extern void L1_pscan_suspend( void* dummy, gas_id_t gas_id);
extern void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id );

extern void gl1_msg_pass_xmsi_to_mdsp (boolean imsi_valid,  uint8 *imsi_ptr,
                                       boolean tmsi_valid,  uint8 *tmsi_ptr,
                                       boolean ptmsi_valid, uint8 *ptmsi_ptr,
                                       gas_id_t gas_id);

#ifdef FEATURE_GSM_TDS
extern boolean l1_sc_irat_performing_acq_meas(gas_id_t gas_id);
#endif

extern void gl1_hw_wcdma_cleanup_reconf_state_at_idle_abort(gas_id_t gas_id);



#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*required now as PCH reads in dual sim require a call to l1_fm_are_frames_available()*/
static void ccch_aborted( void* dummy, gas_id_t gas_id )
{
  MSG_GERAN_HIGH_0_G("call to CCCH abort callback ");
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

void l1_idle_rebook_drx_tick(gas_id_t gas_id);

/*===========================================================================

FUNCTION init_local_idle_data

DESCRIPTION
             Initializes local idle data values.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void init_local_idle_data( gas_id_t gas_id )
{
  memset ( &l1_local_idle_data[gas_id], 0 , sizeof ( l1_local_idle_data_t) );

  l1_local_idle_data[gas_id].background_HPLMN_search_suspended = TRUE;

  l1_local_idle_data[gas_id].l1_is_asynch_int_possible = FALSE;
}

uint8 l1_get_idle_monitors_in_progress( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  return l1_local_idle_data_ptr->idle_monitors_in_progress;
}

void l1_set_idle_monitors_in_progress(uint8 idle_monitors_in_progress, gas_id_t gas_id)
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  l1_local_idle_data_ptr->idle_monitors_in_progress=idle_monitors_in_progress;
}

void l1_increment_idle_monitors_in_progress( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  l1_local_idle_data_ptr->idle_monitors_in_progress++;
}

void l1_decrement_idle_monitors_in_progress( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  if ( l1_local_idle_data_ptr->idle_monitors_in_progress > 0)
  {
  l1_local_idle_data_ptr->idle_monitors_in_progress--;
  }else
  {
    MSG_GERAN_MED_0_G("decrement_idle_monitors when 0!!");
  }
}

void l1_reset_idle_monitors_in_progress( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  l1_local_idle_data_ptr->idle_monitors_in_progress = 0;
}

void l1_set_background_HPLMN_search_suspended(boolean background_HPLMN_search_suspended, gas_id_t gas_id)
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  l1_local_idle_data_ptr->background_HPLMN_search_suspended=background_HPLMN_search_suspended;
}

dBx16_T * l1_get_idle_serv_meas_buf( gl1_power_meas_buffer_type prx_drx_buffer,
                                     gas_id_t gas_id )
{
  dBx16_T *buffer = NULL;
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  switch (prx_drx_buffer)
  {
    case PRX_MEAS_BUFFER:
      buffer = l1_local_idle_data_ptr->idle_serv_meas_buf;
      break;

    case DIVRX_MEAS_BUFFER:
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
      buffer = l1_local_idle_data_ptr->idle_serv_divrx_meas_buf;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
      break;

    default:
      break;
  }

  if (!buffer)
  {
    MSG_GERAN_ERROR_1_G("Wrong buffer request %d", prx_drx_buffer);
  }
  return buffer;
}

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
uint32 l1_get_ind_timer_value( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  return l1_local_idle_data_ptr->ind_timer_value;
}

uint32 l1_get_ind_timer_FN( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  return l1_local_idle_data_ptr->ind_timer_FN;
}
#endif

/***********************************************************************
 *     Function name :  l1_idle_rebook_drx_tick
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     This function rebook the DRX TICK for the PCH read. This function is called
 *     when TRM reserve is called for CCCH, this is to ensure if in any collison cases
 *     DRX entry is deleted, ex: PCH and CBCH collision CBCH wins and later because of
 *     upper layer trigger if the CBCH no more needed, we do a TRM reserve for the PCH but
 *     there wont be a any DRX entry for this. Probably need to extend this to all state
 *     machines in future based on need.
 *
 *     Parameters :
 *     ---------------
 *     void
 *
 *     Returns :
 *     -----------
 *     void
 *
 *************************************************************************/

void l1_idle_rebook_drx_tick(gas_id_t gas_id)
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  if ( (l1_local_idle_data_ptr->drx_ref == 0) && gl1_is_drx_on(gas_id) )
  {
    MSG_GERAN_HIGH_1_G( "l1_idle_rebook_drx_tick to %d",
               l1_local_idle_data_ptr->next_req_frame );
    l1_local_idle_data_ptr->drx_ref =
               gl1_drx_require_tick( l1_local_idle_data_ptr->next_req_frame, gas_id );
  }
}

#if defined( FEATURE_G2X_IDLE_TUNEAWAY) ||defined(FEATURE_G2X_REORG_TUNEAWAY)
/*===========================================================================

FUNCTION  l1_idle_init_ta

DESCRIPTION
  This function registers a callback with trm and reduces the trm priority.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_idle_ta_init( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* Reduce TRM priority from TRM_TRAFFIC to TRM_BG_TRAFFIC and
   * register an unlock callback if all other G subs are inactive
   */
  if ( (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_GRANTED))
  {
    MSG_GERAN_HIGH_0_G("IDLE TA idle_init_ta");

    grm_retain_lock( l1_tskisr_blk->client_id,
                     (grm_unlock_callback_t)&grm_idle_abort_callback,
                     gas_id );

    grm_change_priority( l1_tskisr_blk->client_id,
                         TRM_BG_TRAFFIC,
                         gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id),
                         gas_id ); /* could cause issues with TRM-U dynamic connected mode priority */
  }
}

/*===========================================================================

FUNCTION  gl1_handle_idle_abort_callback

DESCRIPTION
  This function handles the idle TA abort callback
  For TRM_UNLOCK_CANCELLED event idle TA is cancelled
  For TRM_UNLOCK_BY ebent idle TA frame is calculated

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_handle_idle_abort_callback(
       trm_client_enum_t       client,
       trm_unlock_event_enum_t event,
       uint32                  unlock_by_sclk,
       uint32                  unlock_dur,
       gas_id_t                gas_id )
{
  uint32 frame_duration;
  l1_idle_ta_t *l1_idle_ta_p = &(l1_local_idle_data[gas_id].l1_idle_ta);
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  GL1_ISR_LOCK(gas_id);

  switch(event)
  {
    case TRM_UNLOCK_CANCELLED:
       // Canel any pending suspension
       l1_idle_ta_p->ta_fn = GL1_DEFS_INVALID_FN;
       l1_idle_ta_p->ta_prepared = FALSE;
       break;

    case TRM_UNLOCK_BY:
       if( gl1_msg_get_multi_sim_mode() )
       {
         l1_idle_ta_p->ta_sclk = unlock_by_sclk;
         unlock_by_sclk -= timetick_get_safe();
         frame_duration = MSECS_TO_FRAMES(timetick_cvt_from_sclk(unlock_by_sclk, T_MSEC));

         l1_idle_ta_p->ta_fn = ADD_FN( GSTMR_GET_FN_GERAN(gas_id), frame_duration );

#ifdef FEATURE_G2X_IDLE_TUNEAWAY
         /* Avoid TA untill 200ms (44 frames) after transition from Transfer to Idle mode */
         if ((l1_tskisr_blk->transfer_to_idle_fn != GL1_DEFS_INVALID_FN) && IS_FRAME_NUM_LATER( ADD_FN(l1_tskisr_blk->transfer_to_idle_fn,MULTIFRAME_51), l1_idle_ta_p->ta_fn))
         {
           l1_idle_ta_p->ta_fn  = GL1_DEFS_INVALID_FN;
         }
         else
         {
#endif
         l1_idle_ta_p->ta_dur= MSECS_TO_FRAMES(timetick_cvt_from_sclk(unlock_dur, T_MSEC));

#ifdef FEATURE_G2X_IDLE_TUNEAWAY
         }
#endif
         l1_idle_ta_p->ta_prepared = FALSE;
         
         MSG_GERAN_HIGH_4_G("IDLE TA FN %d for client %d from sclk:%d , ta dur %d", l1_idle_ta_p->ta_fn, client, unlock_by_sclk, l1_idle_ta_p->ta_dur);
       }
       else
       {
         l1_idle_ta_p->ta_fn = GL1_DEFS_INVALID_FN;
         l1_idle_ta_p->ta_prepared = FALSE;
         MSG_GERAN_HIGH_0_G("Ignoring unlock");
       }
       break;

    default:
       MSG_GERAN_ERROR_1_G("unhandled unlock event %d", (int)event);
       break;
  }

  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION  l1_idle_is_ta_changed

DESCRIPTION
  This function checks if the unlock time for TA has changed.

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_idle_is_ta_changed( uint32 unlock_by_sclk, gas_id_t gas_id )
{
  l1_idle_ta_t *l1_idle_ta_p = &l1_local_idle_data[gas_id].l1_idle_ta;
  boolean ta_changed = TRUE;

  if ( ( l1_idle_ta_p->ta_fn != GL1_DEFS_INVALID_FN ) && ( unlock_by_sclk == l1_idle_ta_p->ta_sclk ))
  {
    ta_changed = FALSE;
    MSG_GERAN_HIGH_3_G("l1_idle_is_ta_changed, ta_fn:%2x, ta_sclk:%2x, unlock_by_sclk:%2x",
                          l1_idle_ta_p->ta_fn, l1_idle_ta_p->ta_sclk, unlock_by_sclk);
  }

  return ta_changed;
}
#endif /* FEATURE_G2X_IDLE_TUNEAWAY */

#define DEBUG_CCCH_ABORT


/*===========================================================================

FUNCTION  l1_handle_ccch_block_missed

DESCRIPTION
  On missing CCCH block due to missed frames(due to warmup delay or issr overrun) or FM/TRM denial,
  this function informs unknown block quality to RR and takes care of other things like power monitor 
  in GBTA and hybrid DRDSDS algorithm.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_handle_ccch_block_missed( idle_data_T* idle_data_ptr, boolean fm_denial, gas_id_t gas_id )
{
   l1_local_idle_data_t      *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   uint8 missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);

   if ( is_own_page_read( idle_data_ptr, missed_frames, gas_id ) )
   {
     /* handle the TRM lock request failure/missed frames here and notify RR if this
     was our own page - stay awake for another tick to make sure that the
     next page block reservation takes place */
#ifdef FEATURE_WCDMA
     l1_sci_trm_blocked_inc_wcdma_page_cycle_tick(gas_id);
#endif /* FEATURE_WCDMA */
     L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);

     if(TRUE == fm_denial)
     {
        /*If DR-DSDS enabled and G page is denied in X+G, then treat TRM denial
         in same manner as PCHdecode failure however no DSC/rssi/SNR thresholds apply*/
        if(gl1_drdsds_enabled(gas_id))
        {
          /* Trigger Hybrid Tuneaway State machine*/
          gl1_drdsds_hybrid_tuneaway_trigger(DRDSDS_TRM_DENIED_EVENT,
                                             NULL,
                                             l1_tskisr_blk->client_id,
                                             gas_id);
        }
     }
     /* As we have missed a page need to tick next_cell_bcch */
     l1_sc_report_period_done(gas_id);
 
#ifdef FEATURE_GPRS_GBTA
     /* Differ Idle Power Moniter when in Transfer and allowed , we have a missed page here */
     if ( gl1_msg_get_multi_sim_standby_mode() &&
          OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) &&
 
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
         /*Idle pwr monitors allowed in 2 phase access with this feature*/
         (!(OTHER_GSM_CLIENT_IN_SINGLE_BLOCK_ALLOC(OTHER_GAS_ID(gas_id)) && !IS_TWO_PHASE_ACCESS(OTHER_GAS_ID(gas_id)) )) &&
#else
         (!OTHER_GSM_CLIENT_IN_SINGLE_BLOCK_ALLOC(OTHER_GAS_ID(gas_id))) &&
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
          !(OTHER_SUB_RELEASE_PENDING())       &&
          gl1_gbta_schedule_idle_per_mon_if_allowed(gas_id))
     {
       GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_GBTA_Q_PWR_MON_MISSED_PAGE, gas_id);
       grm_notify_event(GBTA_EV_IDLE_PWR_MON,FALSE,gas_id);
     }
#endif /* FEATURE_GPRS_GBTA */
 
     /* reset the bad block count here */
     l1_local_idle_data_ptr->l1_bad_pch_block_cnt = 0;
   }
   else if(l1_tskisr_blk->reservation_for_bcch == TRUE)
   {
     /* Inform GRR that we were blocked/missed while attempting to read BCCH using block quality UNKNOWN */
     L1_send_MPH_BLOCK_QUALITY_IND(BCCH, GL1_BLOCK_QUALITY_UNKNOWN, gas_id);
   }
 
   if(idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING)
   {
     /* Need to clear the EXTENDED_PAGING status if we are denied/missed frames whilst looking for an EXTENDED_PAGING block */
 
     idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;

 
     if(TRUE == fm_denial)
      {
         MSG_GERAN_HIGH_1_G("FN = %d Extended PCH missed due to FM denial, reverting to NORMAL_PAGING", gl1_get_FN( gas_id ));
      }
     else
      {
         MSG_GERAN_HIGH_1_G("FN = %d Extended PCH missed due to missed frames, reverting to NORMAL_PAGING", gl1_get_FN( gas_id ));
      }
   }
 
   /*Reset Drx_ref, Tick removed by this time from l1_drx_tick*/
   l1_local_idle_data_ptr->drx_ref = 0; 
}
/*===========================================================================

FUNCTION  L1_IDLE_MODE_ISR

DESCRIPTION
  This function controls idle mode.

  There are three states:
  L1_IDLE_INIT - Each time the MS enters Idle Mode the sub_state should be
  set to this state.  The first call to the ISR will then set up the ISR,
  the message layer, the DRX manager and the sleep controller.

  L1_IDLE_RECEIVING - The ISR is in this state during serving cell reception.
  When serving cell reception finishes the ISR calculated the frame number
  the next seving cell reception will occur on and then enters the WAITING
  state.

  L1_IDLE_WAITING - In this state the ISR is waiting to for the frame number
  that the next reception will start on.  When this frame number occurs it
  enters the RECEIVING state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_idle_mode_ISR( boolean leaving_idle_mode, gas_id_t gas_id )
{
   gprs_l1_idle_data_t       *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   l1_local_idle_data_t      *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_serving_cell_meas_T    *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   uint32                    use_idle_rx_alpha = (uint32)FALSE;
#endif

   /* Locals */
   idle_data_T   *idle_data_ptr;
   uint32        gap_to_next_activity=0;
   uint32        new_next_req_frame;
   uint32                     this_FN;

   ARFCN_T       serving_BCCH;
   sys_band_mask_type             band_mask;

   /* Variables for msg layer interface */
   gl1_msg_frame_tick_type    ftn_type;
   boolean                    dl_block_start=FALSE;
   gl1_msg_rx_cb_type         rx_cb;

   boolean                    idle;
   uint8                      missed_frames;

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
   boolean                    idle_act_prog = FALSE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   /* Code */
   idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

   this_FN = gl1_get_FN( gas_id );

   l1_fm_set_leaving_idle_mode(leaving_idle_mode, gas_id);

   /* Substate Machine */
   switch (l1_tskisr_blk->sub_state)
   {
      /**************************************
                  L1_IDLE_INIT
      **************************************/
      case L1_IDLE_INIT:

         if (gprs_l1_idle_data->update_pending)
         {
            /*  The signal MPH_GPRS_L1PARAM_UPDATE_REQ has been received
                update now  */
            gpl1_gprs_update_L1_params(gas_id);

            /*  Clear flag now update has occurred  */
            gprs_l1_idle_data->update_pending = FALSE;

         }
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
           if(trans[gas_id].tuneaway_and_ncell_data_initialized)
           {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
             ERR_GERAN_FATAL_0_G("DEBUG CRASH please report this to geran.l1.pkt.transfer ");
           }
#endif
         if(TRUE == gpl1_gprs_get_recovery_cnf_pending(gas_id))
         {
           MSG_GERAN_HIGH_1_G("Moved To IDLE Due to PTM Error Recovery Recovery_pending=%d",gpl1_gprs_get_recovery_cnf_pending(gas_id));
          gpl1_gprs_recovery_send_l1_mac_recovery_cnf(gas_id);
          gpl1_gprs_set_recovery_cnf_pending(FALSE,gas_id);
         }  

         /* Reset the Frame manager*/
         l1_fm_reset(gas_id);

         rx_cb.data_cb    = ccch_rx_callback;
         rx_cb.metrics_cb = ccch_metrics_callback;
         rx_cb.log_cb     = NULL;
         rx_cb.abort_cb   = NULL;

         /* Configure callbacks for message layer */
         gl1_msg_reset(gas_id);

         gl1_msg_cfg_ccch((uint8)(idle_data_ptr->campedon_cell_BSIC & 0x07), TRUE, &rx_cb, gas_id );

         gl1_msg_cch_afc_tt_init( gl1_get_serving_cell_arfcn(gas_id),gas_id );

         l1_initialise_cbch_state(gas_id);

         l1_local_idle_data_ptr->idle_rx_in_progress = 0;
         l1_local_idle_data_ptr->idle_monitors_in_progress = 0;
         l1_local_idle_data_ptr->idle_update_afc_tt = FALSE;

         /* We need the correct TRX state transitions to occur hence
          * force TX to be OFF because it will be turned ON at the end
          * of the idle ISR based on main command.
          */
         gl1_drx_on( FALSE,     /* leaving idle mode flag */
                     gas_id
                   );

         gl1_hw_set_ftm_power_store_init(FALSE,gas_id);

         /* If we are GPRS atached we might need to setup the
          * non-DRX timer.
          */

          /* reset the cancel_non_drx flag in case it was previously set by the MPH_SURROUND_UPDATE signal */
         l1_idle_data->cancel_non_drx = FALSE;

         MSG_GERAN_MED_3_G("tmr [%d] flg [%d] fn [%d]",idle_data_ptr->non_drx_timer,
                 l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag,
                 l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN);

         /* if mm non-drx mode is active then enter non-drx mode */
         if (l1_idle_data->mm_non_drx_mode)
         {
            l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag = TRUE;
         }
         else
         {
            l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag = FALSE;
         }

         MSG_GERAN_HIGH_1_G("mm ndrx mode %d",l1_idle_data->mm_non_drx_mode);

         l1_running_avg_clear_meas( &(idle_data_ptr->campedon_cell_pwr_meas_avg) );

         l1_sc_ncell_monitor_timer_start(gas_id);

         /* set no. meas to 0 -- only affects logging */
         l1_serving_cell_meas_ptr->no_of_measurements = 0;

         /* Start the SCE monitoring process - nothing actually happens
          * until we call l1_sci_paging_block_tick().  Start it only
          * if we're not already aborting idle mode.  This fixes the
          * "idle abort" spin loop issue
          */
         if ( !leaving_idle_mode && (l1_sc_get_curr_mode(gas_id) == L1SCModeIdle))
         {
            l1_sci_go(gas_id);
         }

#if defined( FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
         l1_local_idle_data_ptr->l1_idle_ta.ta_prepared = FALSE;
         l1_local_idle_data_ptr->l1_idle_ta.ta_fn = GL1_DEFS_INVALID_FN;
         l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started = FALSE;
         l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn = GL1_DEFS_INVALID_FN;
#endif /* FEATURE_G2X_IDLE_TUNEAWAY ||FEATURE_G2X_REORG_TUNEAWAY */
         l1_local_idle_data_ptr->not_idle_cnt = 0;
         l1_local_idle_data_ptr->l1_bad_pch_block_cnt = 0;

         /* get serving cel BCCH */
         serving_BCCH = gl1_get_serving_cell_arfcn(gas_id);

         /* If we are about to camp on a 1800 band cell ensure we are running at
          * low clock speed, otherwise set some other band to clock at high speed
          */
         if(serving_BCCH.band == SYS_BAND_DCS_1800)
         {
           band_mask = SYS_BAND_MASK_GSM_DCS_1800;
         }else
         {
           band_mask = SYS_BAND_MASK_GSM_850;
         }

         gl1_set_band_group(band_mask, gas_id);

         if(idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING)
         {
           /* Need to clear the EXTENDED_PAGING status when we enter idle mode */
           idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;
           MSG_GERAN_HIGH_0_G("Clear Extended Paging mode");
         }

         /* Paging info for debug */
         MSG_GERAN_MED_3_G("Page mode=%d APB=%d BS_PA_MFRMS=%d",
                 idle_data_ptr->paging_data.paging_mode,
                 idle_data_ptr->paging_data.all_paging_blocks,
                 idle_data_ptr->campedon_cell_data.BS_PA_MFRMS);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         gl1_msim_reset_reserve_at_count( gas_id );

         /* DR-DSDS runtime check */
         /* As per Hybrid Tuneaway Fallback Algorithm, if other sub is not
               in transfer then enable DRDSDS. Don't disable DRDSDS*/
         gl1_drdsds_hybrid_tuneaway_trigger(DRDSDS_IDLE_INIT_EVENT,
                                            NULL,
                                            l1_tskisr_blk->client_id,
                                            gas_id);

         /*In DR-DSDS enabled idle mode set the correct Rxalpha and CCS priority*/
         if(gl1_drdsds_enabled(gas_id))
         {
             /*indicate to GFW correct RX alpha to use*/
             use_idle_rx_alpha = (uint32)TRUE;
             MSG_GERAN_HIGH_0_G("DR enabled: Idle init: use idleRxalpha ");
         }
         /*notify GFW of correct idle rx alpha to use*/
         mdsp_update_shared_variable(GFW_RX_ALPHA,&use_idle_rx_alpha,gas_id);


         /*re-initialise persistant TRM denial monitoring for PM's upon entering idle mode*/
         (void)grm_persistent_denial_monitoring(GRM_RESET_PERSISTENT_DENIAL_MONITORING,
                                                l1_tskisr_blk->client_id,
                                                GRM_NULL_COMMAND,
                                                gas_id);

         /*re-init the serving cell RSSI & SNR thresholds*/
         (void)grm_persistent_denial_monitoring(GRM_INIT_SERVING_CELL_LEVELS,
                                                l1_tskisr_blk->client_id,
                                                GRM_NULL_COMMAND,
                                                gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

         MSG_GERAN_HIGH_0_G("Idle init ...");

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
         //Restart the Ncell activity after completing WCDMA PLMN scan
         l1_local_idle_data_ptr->background_wcdma_plmn_scan_suspend_ncell = FALSE;
#endif
         /* clear out the PACCH UL FIFO if we have any msgs remaining from transfer */
         gpl1_gprs_reset_pacch_fifo(gas_id);
         l1_transfer_data[gas_id].polling_at_tbf_start_time = FALSE;

       if(l1_tskisr_blk->l1_prev_state == L1_TRANSFER_MODE && 
          (TRUE == gl1_is_geran_tuneaway_allowed(gas_id)))
         {
           /*Update abort callback to Idle one.*/
           grm_retain_lock(l1_tskisr_blk->client_id, (grm_unlock_callback_t)&grm_idle_abort_callback, gas_id);
         }

       /* To ensure the right sub is sending the MAC release cnf in dual sim*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       if (l1_tskisr_blk->l1_prev_state == L1_RANDOM_ACCESS_MODE ||
           l1_tskisr_blk->l1_prev_state == L1_PACKET_ACCESS_MODE ||
           l1_tskisr_blk->l1_prev_state == L1_TRANSFER_MODE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
       {
         /* Send any delayed TBF release request to MAC - immediately after sending the MAC CNF
              a pending RACH may be requested which requires L1 idle init to have been completed */
         if (l1_transfer_data[gas_id].confirm_pending == SEND_AT_IDLE_INIT)
         {
           /*  Indicate to MAC release procedure is complete */
           /*  send confirm  */
           gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

           /* reset relelase type and rel confirm status*/
           l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
           l1_transfer_data[gas_id].confirm_pending = SENT;
           /* sce may not have been started , when abnormal DS_ABORT_IND is not sent */
           /*set state of SC engine*/
           l1_sc_start(L1SCModeIdle, gas_id);
         }

         if ( l1_transfer_data[gas_id].send_l1_mac_single_block_sent )
         {
            /* If L1_MAC_SINGLE_BLOCK_SENT is sent in L1_WAIT_FOR_MDSP_TO_COMPLETE state
             * becasue of parallel thread execution, MAC can send RACH request beofre GL1
             * moved to idle init. And during the idle init procedure, GL1 looses the information that
             * RACH request is received and hence RACH request is not handled.
             * To solve this issuse, delaying sending of L1_MAC_SINGLE_BLOCK_SENT till idle init.
             */
           l1_transfer_data[gas_id].send_l1_mac_single_block_sent = FALSE;

           gpl1_gprs_send_l1_mac_single_block_sent(gas_id);
         }
       }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       else
       {
          MSG_GERAN_MED_2_G("release_pending %d, mac_single_blk_sent_cnf %d", l1_transfer_data[gas_id].release_pending, l1_transfer_data[gas_id].send_l1_mac_single_block_sent);

          if (l1_transfer_data[gas_id].confirm_pending == SEND_AT_IDLE_INIT ||
              l1_transfer_data[gas_id].send_l1_mac_single_block_sent)
          {
            MSG_GERAN_MED_2_G("L1/MAC Cnf not sent as l1_prev_state=%d, l1_curr_state=%d", (int)l1_tskisr_blk->l1_prev_state, l1_tskisr_blk->l1_state);
          }
       }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

	     /*Reset the transfer data variables just in case they were left uncleared*/
         l1_transfer_data[gas_id].pending_frequency_info_ptr = NULL;
         l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
         l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
         l1_transfer_data[gas_id].pending_single_tbf_ptr = NULL;

         l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
         l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
         /* Always reset the iRAT handover Ind used for AGC. */
         l1_interrat_ho_update(FALSE, gas_id);
         l1_sc_bcch_reset_high_priority_flag(gas_id);

         l1_local_idle_data_ptr->next_req_frame = GL1_DEFS_INVALID_FN;
         l1_tskisr_blk->sub_state = L1_IDLE_WAITING;

#ifdef FEATURE_GSM_COEX_SW_CXM
         /* Notify the arbitrator of the transition to IDLE */
         garb_intf_notify_idle( serving_BCCH,
                                             idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB,
                                             idle_data_ptr->paging_data.paging_mode,
                                             idle_data_ptr->campedon_cell_data.BS_PA_MFRMS,
                                             idle_data_ptr->paging_data.paging_multiframe,
                                             idle_data_ptr->paging_data.paging_block_index,
                                             gas_id );
#endif

         /* fall through */

         /**************************************
                    L1_IDLE_WAITING
         **************************************/
      case L1_IDLE_WAITING:
         /* Check whether we are trying to leave Idle Mode.  If we are we don't
          * want to start anymore receives or go to sleep.
          */
         if ( !leaving_idle_mode )
         {
            missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);

            if (missed_frames)
            {
            MSG_GERAN_HIGH_3_G("L1_IDLE_WAITING 1 next required frame %d missed frames %d current fn %d ...",
                               l1_local_idle_data_ptr->next_req_frame, missed_frames, this_FN );
            }
            /* If this is our FN then run MFSM and transition to receiving state. */
            /* Offset missed frames due to error recovery */
            if ( (l1_local_idle_data_ptr->next_req_frame != GL1_DEFS_INVALID_FN) &&
                 (this_FN == ADD_FN(l1_local_idle_data_ptr->next_req_frame, missed_frames)) &&
                 (missed_frames < 3) )
            {
#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
                 l1_local_idle_data_ptr->background_wcdma_plmn_scan_suspend_ncell = FALSE;
#endif            
 #if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               /* When in Dual SIM there can be no assumption that the page read will */
               /* be allowed, so trm must be asked for the lock here before proceeding */
               if ( l1_fm_are_frames_available( FM_PRI_CCCH,
                                                L1_SC_DURATION_CCCH-missed_frames,
                                                ccch_aborted, 0, gas_id) )
               {

                 l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started = FALSE;
                 l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn = GL1_DEFS_INVALID_FN;

                 /* Make sure that FW/RF are initialised and if this is "NOT" first QTA gap after wake up from sleep*/
                 if ( gl1_msg_get_multi_sim_mode() &&
                     ( l1_tskisr_blk->firmware_loaded == FALSE ) &&
                      ( gl1_check_qta_active_in_warmup(gas_id) == FALSE ) )
                 {
                   MSG_GERAN_LOW_1_G("FN = %d CCCH - frames available - init FW",
                                           gl1_get_FN( gas_id ));

                   if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_NOT_GRANTED )
                   {
                     l1_tskisr_blk->firmware_loaded = TRUE;

#ifdef FEATURE_QTA
                     if(l1_tskisr_blk->qta_in_progress)
                     {
                       ARFCN_T serving_BCCH = gl1_get_serving_cell_arfcn(gas_id);
                       boolean interband_monitors = l1_sc_monscan_is_next_idle_scan_interband(gas_id);

                       if(!gl1_hw_qta_fw_rf_init(FALSE, GL1_FW_QBTA_CM_START_UP_OFFSET, FALSE, serving_BCCH, interband_monitors, TRUE, gas_id))
                       {
                          l1_tskisr_blk->firmware_loaded = FALSE;
                           /* If we do not have enough time to send pre load and start cmds to GFW */
                           /* miss first/this burst (adaptive timeline should give more time for subsequent QTA wake ups) */
                           l1_fm_set_suspend_trm_release(SUSPENSION_FRAME_DELAY,FM_PRI_RESERVED,gas_id);
                           gl1_hw_sleep_set_missed_frames(gas_id, 1);
                           gl1_drx_require_next_tick(gas_id);
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
                          gl1_hw_set_adaptive_warmup_time(gas_id, QTA_WARMUP_TIME);
#endif
                           MSG_GERAN_HIGH_2_G(GL1_SLEEP_HDR "FN = %d CCCH - Qs %d: qta_fw_rf_init too late, skip this burst",
                                             gl1_get_FN( gas_id ),gstmr_rd_qsymbol_count(geran_map_gas_id_to_nas_id(gas_id)) );
                           break;
                         }
                       else
                       {
                         /* Prevent FM from releasing in the same frame */
                         l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                        FM_PRI_CCCH, gas_id );
                       }

                       MSG_GERAN_LOW_3_G("QTA with one frame %d %d %d",
                                          l1_local_idle_data_ptr->idle_rx_in_progress,
                                          l1_local_idle_data_ptr->idle_monitors_in_progress,
                                          gap_to_next_activity);
                     }
                     else
#endif /* FEATURE_QTA */
                     {
                       /* Issue the call to init the firmware and RF */
                       gl1_hw_multi_sim_fw_rf_init(TRUE,
                                                   l1_task_tcb_read(gas_id),
                                                   gsm_l1_wait,
                                                   L1_WAIT_HW_SIG,
                                                   L1_TIMEOUT_HW_SIG,
                                                   GL1_HW_INIT_RX_ONLY,
                                                   l1_tskisr_blk->client_id,
                                                   FALSE,
                                                   gas_id);
                     }
                   }
                 }
#ifdef FEATURE_QTA
				 else if( (gl1_msg_get_multi_sim_mode()) &&
                           ( l1_tskisr_blk->firmware_loaded == TRUE) &&
                           (l1_tskisr_blk->qta_in_progress) && 
                           (!gl1_hw_get_qta_cm_fw_rf_init_status(gas_id)))
	  		     {
				       ARFCN_T serving_BCCH = gl1_get_serving_cell_arfcn(gas_id);
                       boolean interband_monitors = l1_sc_monscan_is_next_idle_scan_interband(gas_id);
                       if(!gl1_hw_qta_fw_rf_init(FALSE,GL1_FW_QBTA_CM_START_UP_OFFSET,FALSE,serving_BCCH,interband_monitors,TRUE,gas_id))
                       {
                          l1_tskisr_blk->firmware_loaded = FALSE;
                       }
                       else
                       {
                         /* Prevent FM from releasing in the same frame */
                         l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                        FM_PRI_CCCH, gas_id );
                       }

                       MSG_GERAN_HIGH_2_G("QTA when firmware_loaded is true FW_load %d qta status %d",
                                          l1_tskisr_blk->firmware_loaded,
                                          l1_tskisr_blk->qta_in_progress);	  		   
	  		      }
#endif /* FEATURE_QTA */

                 gl1_hw_set_idle_mode(TRUE,gas_id);

                 if (gl1_check_possible_isr_overrun(gas_id))
                 {
                   /* If we are too much into frame after fw init, skip issuing commands to FW in this frame */
                   gl1_hw_sleep_set_missed_frames(gas_id, 1);
                   /* Prevent FM from releasing in the same frame */
                   l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                  FM_PRI_CCCH, gas_id );
                   MSG_GERAN_HIGH_1_G(GL1_SLEEP_HDR"Avoid first burst as possible isr overrun,  skip running this idle state miss frames %d", gl1_hw_sleep_get_missed_frames(gas_id));
                   break;
                 }
                 else if (l1_tskisr_blk->qta_in_progress == FALSE)
                 {
                   /* Prevent FM from releasing in the same frame */
                   l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                  FM_PRI_CCCH, gas_id );
                 }

                 if(((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
                 || (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT)))
                 {
                   dl_block_start = idle_multiframe_state_machine( idle_data_ptr, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                   if( dl_block_start == FALSE &&
                       l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag == FALSE &&
                       gl1_msg_get_multi_sim_mode() &&
                       l1_tskisr_blk->firmware_loaded == TRUE )
                   {
                     /* Clear out required frames for a block is not required */
                     l1_fm_clear_required_frames(FM_PRI_CCCH, gas_id);
                     MSG_GERAN_HIGH_0_G("xCCH block receive is not required");
					 gl1_hw_sleep_clear_missed_frames(gas_id);
					 
#ifdef FEATURE_QTA
                     /* Cleanup QBTA if a block is not required. */
                     if(l1_tskisr_blk->qta_in_progress)
                     {
                       gl1_hw_qta_fw_rf_deinit(gas_id, GL1_FW_QBTA_CM_CLEAN_UP_OFFSET, TRUE);
                     }
#endif /* FEATURE_QTA */
                   }
                   else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
                   {
                     l1_tskisr_blk->sub_state = L1_IDLE_RECEIVING;
                   }

                   if (l1_local_idle_data_ptr->idle_rx_in_progress ||
                       l1_local_idle_data_ptr->idle_monitors_in_progress)
                    {
                      gap_to_next_activity = 0;
                    }
                   }
                   else
                   {
                     l1_fm_clear_required_frames(FM_PRI_CCCH, gas_id);
                     MSG_GERAN_HIGH_0_G("Don't run idle mframe state machine as G2X search is active");
                   }
               }
               else
               {
                 l1_handle_ccch_block_missed(idle_data_ptr, TRUE, gas_id);
                    /*Clear the missed frames*/
                 gl1_hw_sleep_clear_missed_frames(gas_id); 

                 /* calculate gap to next activity */
                 gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, 0, gas_id );

                 /* Update FM */
                 if (gap_to_next_activity > 0)
                 {
                    l1_local_idle_data_ptr->next_req_frame = this_FN + gap_to_next_activity + 1;

                    if (l1_local_idle_data_ptr->next_req_frame >= FRAMES_IN_HYPERFRAME)
                    {
                      l1_local_idle_data_ptr->next_req_frame -= FRAMES_IN_HYPERFRAME;
                    }

                    l1_fm_set_required_frame_number( FM_PRI_CCCH, l1_local_idle_data_ptr->next_req_frame, L1_SC_DURATION_CCCH, gas_id);

                    /*  pass to drx manager              */
                    l1_local_idle_data_ptr->drx_ref = gl1_drx_require_tick( l1_local_idle_data_ptr->next_req_frame, gas_id );

                    /* transition to waiting            */
                    l1_tskisr_blk->sub_state = L1_IDLE_WAITING;

                 }/*if gap_to_next_activity*/

                 gl1_drx_require_next_tick(gas_id);
               }
 #else
         if(((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
         || (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT)))
         {
                 dl_block_start = idle_multiframe_state_machine( idle_data_ptr, gas_id );

                 l1_tskisr_blk->sub_state = L1_IDLE_RECEIVING;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                 l1_local_idle_data_ptr->idle_ds_receive_guard = MULTI_SIM_CCCH_FW_LOAD_GUARD;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

               if (l1_local_idle_data_ptr->idle_rx_in_progress || l1_local_idle_data_ptr->idle_monitors_in_progress)
               {
                  gap_to_next_activity = 0;
               }
        }
 #endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
              #error code not present
#endif
            }
            else
            {
              if( missed_frames && (this_FN == l1_local_idle_data_ptr->next_req_frame))
              {
#ifdef FEATURE_QSH_MDUMP
                QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                /* Missed frame should be cleared during the previous activity, here it should not come  */
                /* Force crash to trap the issue earlier, which activity did not clear the missed frames */
                ERR_GERAN_FATAL_1_G("missed frames: %d is not cleared in previous activity", missed_frames);
              }
              if(missed_frames >= 3)
              {
                 MSG_GERAN_HIGH_1_G("%d bursts got missed, skip Block", missed_frames);
                 l1_fm_clear_required_frames(FM_PRI_CCCH, gas_id);                                  
                 l1_handle_ccch_block_missed(idle_data_ptr, FALSE, gas_id);
                 gl1_hw_sleep_clear_missed_frames(gas_id);                       
              }
            /* calculate gap to next activity for SCE */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
              gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, 0, gas_id );

#else
              gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, gas_id );
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                /*******************************************************************************
                            If in a multi paging mode and the gap = 0 then ensure that we don't keep
                            re-requesting the same block again and again. Once one DL block is denied only
                            ask for the one after this
                           ********************************************************************************/
               if ( (( l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag ) ||
                     ( idle_data_ptr->paging_data.all_paging_blocks ) ||
                     ( l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag ))
                    &&
                     ( gap_to_next_activity == 0 )
                    &&
                     ( l1_tskisr_blk->firmware_loaded == FALSE )
                   )
               {
                  gap_to_next_activity = gpl1_multi_sim_get_next_block_start_gap( idle_data_ptr, gas_id );
               }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

               new_next_req_frame = gl1_get_FN( gas_id ) + gap_to_next_activity + 1;

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
               #error code not present
#endif

               if (new_next_req_frame >= FRAMES_IN_HYPERFRAME)
               {
                   new_next_req_frame -= FRAMES_IN_HYPERFRAME;
               }

               if (new_next_req_frame != l1_local_idle_data_ptr->next_req_frame)
               {


                  /* remove old frame */
                  gl1_drx_release_fn_tick(l1_local_idle_data_ptr->next_req_frame, gas_id);

                  l1_local_idle_data_ptr->next_req_frame = new_next_req_frame;

                  l1_local_idle_data_ptr->drx_ref = gl1_drx_require_tick( l1_local_idle_data_ptr->next_req_frame, gas_id );

                  l1_fm_set_required_frame_number( FM_PRI_CCCH, l1_local_idle_data_ptr->next_req_frame, L1_SC_DURATION_CCCH, gas_id);
               }
#ifdef FEATURE_QTA
               //gl1_update_trm_reservation_for_ta(gas_id);
#endif /* FEATURE_QTA */
#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
               /* suspend ncell is only relevant when WTOG BPLMN is active */
               if(l1_local_idle_data_ptr->background_wcdma_plmn_scan_suspend_ncell && l1_get_background_wcdma_plmn_scan_active(gas_id) )
               {
                 gap_to_next_activity = 0;
                 MSG_GERAN_HIGH_0_G("background_wcdma_plmn_scan_suspend_ncell");
               }
#endif /* FEATURE_GTOW_BACKGROUND_PLMN_SEARCH */
            }

#if defined (FEATURE_GSM_SIMULATE_GL1_RF)  || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
            #error code not present
#endif
            {
              gl1_non_drx_wait_frames_max[gas_id] = MULTIFRAME_51;
            }
             
            if (  (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag)
               && (l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started)
                && (l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn != GL1_DEFS_INVALID_FN)
                && (IS_FRAME_NUM_LATER( this_FN, ADD_FN(l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn,gl1_non_drx_wait_frames_max[gas_id]) ))
               )
            {
              MSG_GERAN_HIGH_3_G("RR_L1_PS_PDU_TUNEAWAY_OVERRUN NON DRX = %d GAP_START = %d  GAP_START_FN = %d",
              l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag,l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started,
              l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn);

              L1_send_MPH_PREEMPTION_IND(RR_L1_PS_PDU_TUNEAWAY_OVERRUN, gas_id);
              l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started = FALSE;
              l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn = GL1_DEFS_INVALID_FN;
            }

           if ( gl1_msg_is_sbd_enabled( gas_id ) )
           {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
              if ( gl1_msg_get_multi_sim_mode() )
              {
                if ( l1_tskisr_blk->firmware_loaded &&
                     idle_data_ptr->imsi_data.updated )
                {
                  gl1_msg_pass_xmsi_to_mdsp( idle_data_ptr->imsi_data.imsi_full_valid,&idle_data_ptr->imsi_data.imsi[0],
                                             idle_data_ptr->imsi_data.tmsi_valid,&idle_data_ptr->imsi_data.tmsi[0],
                                             idle_data_ptr->imsi_data.ptmsi_valid,&idle_data_ptr->imsi_data.ptmsi[0],
                                             gas_id );

                  idle_data_ptr->imsi_data.updated = FALSE;
                }
              }
              else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
              {
                if ( idle_data_ptr->imsi_data.updated )
                {
                  gl1_msg_pass_xmsi_to_mdsp( idle_data_ptr->imsi_data.imsi_full_valid,&idle_data_ptr->imsi_data.imsi[0],
                                             idle_data_ptr->imsi_data.tmsi_valid,&idle_data_ptr->imsi_data.tmsi[0],
                                             idle_data_ptr->imsi_data.ptmsi_valid,&idle_data_ptr->imsi_data.ptmsi[0],
                                             gas_id );

                  idle_data_ptr->imsi_data.updated = FALSE;
                }
             }
           }
         }
         else
         {
#ifdef FEATURE_QTA
            if((l1_tskisr_blk->qta_in_progress) &&  (idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING))
            {
              idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;
			  if (!l1_fm_any_activity_running(gas_id))
			  {
                call_in_task_grm_release(l1_tskisr_blk->client_id);
			  }
            }
#endif
            /* We are trying to leave Idle Mode. */
            gl1_drx_require_next_tick(gas_id);
            gap_to_next_activity = 0;
         }
         break;

         /**************************************
                     L1_IDLE_RECEIVING
         **************************************/
      case L1_IDLE_RECEIVING:
         /* Check whether we are trying to leave Idle Mode.  If we are we don't
          * want to start anymore receives or go to sleep.
          */
         if ( !leaving_idle_mode )
         {

#if defined( FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)

           /* Init IDLE TA if non drx mode is active or if page mode is reorg*/
            if ( gl1_msg_get_multi_sim_mode() && 
                 (((TRUE == l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) && gl1_hw_is_g2x_non_drx_ta_enabled() )||
                 ((l1_idle_data->mm_non_drx_mode)&&(FALSE == idle_data_ptr->non_updated_lai_in_nmo1))) && 
                  gl1_is_geran_tuneaway_allowed(gas_id) &&
                 ( gl1_hw_qta_gap_active(gas_id) == FALSE ))
            {
              l1_idle_ta_init( gas_id );
            }
            else
            {
              MSG_GERAN_HIGH_3_G("G2x TA not allowed MM_Non_Drx %d Non_Drx_timer %d page mode %d",l1_idle_data->mm_non_drx_mode,l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag,idle_data_ptr->paging_data.paging_mode);
            }
#endif /* FEATURE_G2X_IDLE_TUNEAWAY || FEATURE_G2X_REORG_TUNEAWAY*/
            
#ifdef FEATURE_G2X_REORG_TUNEAWAY
			
           /* Init IDLE TA if non drx mode is active or if page mode is reorg*/
            if ( gl1_msg_get_multi_sim_mode() && 
				((idle_data_ptr->paging_data.paging_mode == PAGING_REORGANIZATION)&& gl1_hw_is_g2x_reorg_ta_enabled()) && 
				 gl1_is_geran_tuneaway_allowed(gas_id))
            {
              l1_idle_ta_init( gas_id );
            }
			else
			{
              MSG_GERAN_HIGH_1_G("G2x TA not allowed page mode %d",idle_data_ptr->paging_data.paging_mode);
			}
#endif /* FEATURE_G2X_REORG_TUNEAWAY */

#ifdef FEATURE_G2X_REORG_TUNEAWAY
			
           /* Init IDLE TA if non drx mode is active or if page mode is reorg*/
            if ( gl1_msg_get_multi_sim_mode() && 
				((idle_data_ptr->paging_data.paging_mode == PAGING_REORGANIZATION)&& gl1_hw_is_g2x_reorg_ta_enabled()) && 
                  gl1_is_geran_tuneaway_allowed(gas_id) &&
                 ( gl1_hw_qta_gap_active(gas_id) == FALSE )
               )
            {
              l1_idle_ta_init( gas_id );
            }
			else
			{
              MSG_GERAN_HIGH_1_G("G2x TA not allowed page mode %d",idle_data_ptr->paging_data.paging_mode);
			}
#endif /* FEATURE_G2X_REORG_TUNEAWAY */
#if defined (FEATURE_GSM_SIMULATE_GL1_RF)  || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
             #error code not present
#endif
             {
                 gl1_non_drx_wait_frames_max[gas_id] = MULTIFRAME_51;
             }
             if (  (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag)
              && (l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started)
              && (l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn != GL1_DEFS_INVALID_FN)
              && (IS_FRAME_NUM_LATER( this_FN, ADD_FN(l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn,gl1_non_drx_wait_frames_max[gas_id]) ))
             )
             {
                   MSG_GERAN_HIGH_3_G("RR_L1_PS_PDU_TUNEAWAY_OVERRUN NON DRX = %d GAP_START = %d	GAP_START_FN = %d",
                   l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag,l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started,
                   l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn);

                 L1_send_MPH_PREEMPTION_IND(RR_L1_PS_PDU_TUNEAWAY_OVERRUN,gas_id);
                 l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started = FALSE;
                 l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn = GL1_DEFS_INVALID_FN;
             }


            /* OK to start receives so run MFSM */
            dl_block_start = idle_multiframe_state_machine( idle_data_ptr, gas_id );

            if( l1_local_idle_data_ptr->next_req_frame == gl1_get_FN(gas_id))
            {
              l1_local_idle_data_ptr->drx_ref = 0;
            }

            if ((l1_local_idle_data_ptr->idle_rx_in_progress==0) && (l1_local_idle_data_ptr->idle_monitors_in_progress==0))
            {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               /* calculate gap to next activity */
               gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, 0, gas_id );

               if ( gl1_msg_get_multi_sim_mode() && (gap_to_next_activity == 0) )
               {
                 /* If we need to carry on receiving here then make sure that this is ok with TRM first */
                 if ( l1_fm_are_frames_available( FM_PRI_CCCH, L1_SC_DURATION_CCCH, ccch_aborted, 0, gas_id) == FALSE )
                 {
                   /* not allowed so go back to the WAITING state for the next activity after this block */
                   gap_to_next_activity = calculate_gap_to_next_activity(idle_data_ptr, 1, gas_id);

                   /* As the frame_offset 1 used above in calculate_gap_to_next_activity(), 
                    * resulting 1 frame less than the actual gap_to_next_activity value. So 
                    * adding 1 frame to compensate that. */
                   gap_to_next_activity = gap_to_next_activity + 1;

                   /**********************************************************************************
                   * The activity has been blocked by the frame manager/TRM so force a transition to *
                   * the waiting state - L1_IDLE_WAITING. This will happen below anyway within       *
                   * if (gap_to_next_activity > 0)... but we must cover the case where the           *
                   * gap_to_next_activity is 0. For example in paging reorg                          *
                   ***********************************************************************************/

                   l1_tskisr_blk->sub_state = L1_IDLE_WAITING;
                   /* If the AFC and TT need updating do this now before releasing the lock.
                   If this isn't done then the idle_update_afc_tt will lock out the release
                   mechanism.*/
                   if ( l1_local_idle_data_ptr->idle_update_afc_tt )
                   {
                     if ( l1_tskisr_blk->firmware_loaded )
                     {
                       gl1_msg_cch_tt_adjust(gas_id);
                       gl1_msg_cch_afc_adjust(gas_id);
                     }

                     l1_local_idle_data_ptr->idle_update_afc_tt = FALSE;
                   }

                   if(idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING)
                   {
                      /* Need to clear the EXTENDED_PAGING status if we are denied frames whilst looking for an EXTENDED_PAGING block */

                      idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;

                      MSG_GERAN_HIGH_1_G("FN = %d Extended PCH missed due to FM denial, reverting to NORMAL_PAGING", gl1_get_FN( gas_id ));
                   }
                 }
               }
#else
               /* calculate gap to next activity */
               gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

               if (gap_to_next_activity > 0)
               {
                     /* check if its time to update afc and tt */
                     if (l1_local_idle_data_ptr->idle_update_afc_tt)
                     {
                        gl1_msg_cch_tt_adjust(gas_id);
                        gl1_msg_cch_afc_adjust(gas_id);
                        l1_local_idle_data_ptr->idle_update_afc_tt = FALSE;
                     }
                     else
                     {
                        l1_local_idle_data_ptr->next_req_frame = this_FN + gap_to_next_activity + 1;

                        if (l1_local_idle_data_ptr->next_req_frame >= FRAMES_IN_HYPERFRAME)
                        {
                           l1_local_idle_data_ptr->next_req_frame -= FRAMES_IN_HYPERFRAME;
                        }

                        l1_fm_set_required_frame_number( FM_PRI_CCCH, l1_local_idle_data_ptr->next_req_frame, L1_SC_DURATION_CCCH, gas_id);

                         /* pass to drx manager              */
                        l1_local_idle_data_ptr->drx_ref = gl1_drx_require_tick( l1_local_idle_data_ptr->next_req_frame, gas_id );

                        /* transition to waiting            */
                        l1_tskisr_blk->sub_state = L1_IDLE_WAITING;

                     }
               }
            }
            else
            {
               /* carry on with current reception */
               gap_to_next_activity = 0;
            }
         }
         else
         {
            /* We are trying to leave Idle Mode */
           gl1_drx_require_next_tick(gas_id);
            gap_to_next_activity = 0;
         }

         if (gprs_l1_idle_data->update_pending)
         {
            /*  The signal MPH_GPRS_L1PARAM_UPDATE_REQ has been received
             update now  */
            gpl1_gprs_update_L1_params(gas_id);

            /*  Clear flag now update has occurred  */
            gprs_l1_idle_data->update_pending = FALSE;
        }
        break;

      default:
         MSG_GERAN_HIGH_1_G("Bad Idle Mode substate. %d", l1_tskisr_blk->sub_state);
         break;
   }  /* end of switch */

#if defined( FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
   /* invalidtae TA as idle TA fn has passed */
   if ( ( l1_local_idle_data_ptr->l1_idle_ta.ta_fn != GL1_DEFS_INVALID_FN )
        && IS_FRAME_NUM_LATER( this_FN, ADD_FN(l1_local_idle_data_ptr->l1_idle_ta.ta_fn,1) )
      )
   {
     l1_local_idle_data_ptr->l1_idle_ta.ta_prepared = FALSE;
     l1_local_idle_data_ptr->l1_idle_ta.ta_fn = GL1_DEFS_INVALID_FN;
     MSG_GERAN_HIGH_2_G("IDLE TA frame over, reset TA and find next possible CCCH gap rx_in_prog:%d, mon:%d", 
                        l1_local_idle_data_ptr->idle_rx_in_progress,
                        l1_local_idle_data_ptr->idle_monitors_in_progress);
     if(!(l1_local_idle_data_ptr->idle_rx_in_progress || l1_local_idle_data_ptr->idle_monitors_in_progress))
     {
	     /* calculate gap to next activity */
    gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, 0, gas_id );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        if ( (( l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag ) ||
              ( idle_data_ptr->paging_data.all_paging_blocks ) || 
              ( l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag ))
              &&
              ( gap_to_next_activity == 0 )
              &&
              ( l1_tskisr_blk->firmware_loaded == FALSE )
            )
            {
             gap_to_next_activity = gpl1_multi_sim_get_next_block_start_gap( idle_data_ptr, gas_id );
            }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    l1_local_idle_data_ptr->next_req_frame = this_FN + gap_to_next_activity + 1;
    if (l1_local_idle_data_ptr->next_req_frame >= FRAMES_IN_HYPERFRAME)
    {
      l1_local_idle_data_ptr->next_req_frame -= FRAMES_IN_HYPERFRAME;
    }
    l1_fm_set_required_frame_number( FM_PRI_CCCH, l1_local_idle_data_ptr->next_req_frame, L1_SC_DURATION_CCCH, gas_id);
     /*  pass to drx manager              */
    l1_local_idle_data_ptr->drx_ref = gl1_drx_require_tick( l1_local_idle_data_ptr->next_req_frame, gas_id );

    /* transition to waiting            */
    l1_tskisr_blk->sub_state = L1_IDLE_WAITING;         
    gl1_drx_require_next_tick(gas_id);
   }
   }

  if ( l1_local_idle_data_ptr->l1_idle_ta.ta_prepared )
   {
     /* Don't sleep until idle TA frame is over
      * when idle TA frame is over then idle TA will be invalidated and
      * gl1 will start requesting TRM for decoding every page blocks
      */
     gl1_drx_require_next_tick(gas_id);
   }
#endif /* FEATURE_G2X_IDLE_TUNEAWAY  ||FEATURE_G2X_REORG_TUNEAWAY */

   /***********************************************************
      Make sure that everything that needs a tick gets one
   ***********************************************************/
   l1_fm_tick(gap_to_next_activity, gas_id);

   l1_tick_cbch(gas_id);

   l1_sci_tick_idle(gas_id);

   gl1_msg_init_frame_tick_info(&ftn_type);
   ftn_type.next_frame_is_dl_block_start             = dl_block_start;
   ftn_type.next_frame_is_acq_ncell_block_start      = gl1_is_ncell_block_start(gas_id);
   ftn_type.next_frame_is_pwr_meas_block_start       = gl1_is_pwr_meas_block_start(gas_id);
   gl1_set_ncell_block_start(FALSE,gas_id);
   gl1_set_pwr_meas_block_start(FALSE,gas_id);

   /* If HPLMN search is in progress, don't call frame tick
      notification from here, it will be called by the
      subsequent pscan or bcch list or l1_acq handlers
    */
   if ( L1_background_HPLMN_search_running(gas_id) == FALSE )
   {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( gl1_msg_get_multi_sim_mode() )
      {
         /* In Dual sim operation only perform the frame tick when firmware has been downloaded
          * this implies that the lock is granted too
          */
          if ( l1_tskisr_blk->firmware_loaded == TRUE
              || gl1_msg_get_id_srch_in_progress(gas_id)||gl1_msg_get_recon_srch_progress(gas_id))
          {
             gl1_msg_frame_tick_notification ( &ftn_type,gas_id );
          }
      }
      else
      {
         gl1_msg_frame_tick_notification ( &ftn_type,gas_id );
      }

#ifndef FEATURE_GSM_RFLM_FED
#if defined ( FEATURE_QTA ) && defined ( FEATURE_CXM_QTA )
      if( gl1_hw_get_qta_cm_cleanup_scripts_status(gas_id))
      {
        /* Ensure Sync Rx is scheduled before we build cleanup script
      * Otherwise WCDMA will get Floored AGC on tuneback
      */
       /* If GSM uses RFA Task but TDS does not then need to wait for GSM RFA Task to complete */
#ifdef FEATURE_WCDMA
     gl1_hw_g2x_wait_rf_buffer_completion(gas_id);
#endif
     gl1_hw_qta_build_cm_cleanup_scripts( gas_id );
   }
#endif
#endif /* FEATURE_GSM_RFLM_FED */


#else
     gl1_msg_frame_tick_notification ( &ftn_type,gas_id );
#endif
   }

   l1_post_tick_cbch(gas_id);

   /* Following function added for Ncell SCE awake time optimization.
      In case, the callback during frame tick notification indicates
      completion of a receive, we can go to sleep in the same frame
      provided no other Ncell ACQ is to be scheduled
   */
   l1_sci_post_tick_idle(gas_id);

   /* Re-check whether we can sleep. */
   if (l1_tskisr_blk->sub_state==L1_IDLE_RECEIVING)
   {
      if ( (l1_local_idle_data_ptr->idle_rx_in_progress==0) &&
           (l1_local_idle_data_ptr->idle_monitors_in_progress==0) &&
           (!l1_local_idle_data_ptr->idle_update_afc_tt) &&
           (l1_local_idle_data_ptr->cbch_rx_in_progress ==0) &&
           (!l1_sc_check_irat_active(gas_id)) &&
           (!leaving_idle_mode) )
      {

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         /* calculate gap to next activity */
        gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, 0, gas_id );

        if ( gl1_msg_get_multi_sim_mode() && (gap_to_next_activity == 0) )
        {
          /* If we need to carry on receiving here then make sure that this is ok with TRM */
          if ( l1_fm_are_frames_available( FM_PRI_CCCH, L1_SC_DURATION_CCCH, ccch_aborted, 0, gas_id) == FALSE )
          {
            /* not allowed go back to the WAITING state for the next activity after this block */
            gap_to_next_activity = calculate_gap_to_next_activity(idle_data_ptr, 1, gas_id);

            /* As the frame_offset 1 used above in calculate_gap_to_next_activity(), 
             * resulting 1 frame less than the actual gap_to_next_activity value. So 
             * adding 1 frame to compensate that. */
            gap_to_next_activity = gap_to_next_activity + 1;

            /**********************************************************************************
            * The activity has been blocked by the frame manager/TRM so force a transition to *
            * the waiting state - L1_IDLE_WAITING. This will happen below anyway within       *
            * if (gap_to_next_activity > 0)... but we must cover the case where the           *
            * gap_to_next_activity is 0. For example in paging reorg                          *
            ***********************************************************************************/

            if(idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING)
            {
              /* Need to clear the EXTENDED_PAGING status if we are denied frames whilst looking for an EXTENDED_PAGING block */

              idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;

              MSG_GERAN_HIGH_1_G("FN = %d Extended PCH missed due to FM denial, reverting to NORMAL_PAGING", gl1_get_FN( gas_id ));
            }

            l1_tskisr_blk->sub_state = L1_IDLE_WAITING;
          }
          else
          {
            /*ok to carry on the RX go and wait for the start of the block in the L1_IDLE_MULTI_SIM_FW_LOADING
            state. Ensure that the FW is loaded too and if not start the load prior to the move to the state */

            if ( l1_tskisr_blk->firmware_loaded == FALSE )
            {
                /* Issue the call to load up the firmware */
                gl1_hw_multi_sim_fw_rf_init((l1_tskisr_blk->sub_state = L1_IDLE_WAITING),
                                           l1_task_tcb_read(gas_id),
                                           gsm_l1_wait,
                                           L1_WAIT_HW_SIG,
                                           L1_TIMEOUT_HW_SIG,
                                           GL1_HW_INIT_RX_ONLY,
                                           l1_tskisr_blk->client_id,
                                           FALSE,
                                           gas_id);
                l1_tskisr_blk->firmware_loaded = TRUE;
            }

#ifdef FEATURE_G2X_IDLE_TUNEAWAY
            /* Init IDLE TA if mm_non_drx is not active and L1 NON DRX timer running*/
            if ( gl1_msg_get_multi_sim_mode() && (FALSE == l1_idle_data->mm_non_drx_mode) && (TRUE == l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) )
            {
              l1_idle_ta_init( gas_id );
            }
			else
			{
              MSG_GERAN_HIGH_2_G("G2x TA not allowed MM_Non_Drx %d Non_Drx_timer %d",l1_idle_data->mm_non_drx_mode,l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag);
			}
			
#endif /* FEATURE_G2X_IDLE_TUNEAWAY */
          }
        }
#else
        /* calculate gap to next activity for SCE */
         gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

         if (gap_to_next_activity > 0)
         {
             l1_local_idle_data_ptr->next_req_frame = this_FN + gap_to_next_activity + 1;

             if (l1_local_idle_data_ptr->next_req_frame >= FRAMES_IN_HYPERFRAME)
             {
                l1_local_idle_data_ptr->next_req_frame -= FRAMES_IN_HYPERFRAME;
             }

             l1_fm_set_required_frame_number( FM_PRI_CCCH, l1_local_idle_data_ptr->next_req_frame, L1_SC_DURATION_CCCH, gas_id);

             /* pass to drx manager              */
             l1_local_idle_data_ptr->drx_ref = gl1_drx_require_tick( l1_local_idle_data_ptr->next_req_frame, gas_id );

             /* transition to waiting            */
             l1_tskisr_blk->sub_state = L1_IDLE_WAITING;

             if (!l1_sci_check_ok_to_sleep (gap_to_next_activity, gas_id) )
             {
                 /* Required to make ncell acq work with optimised timeline */
                 gl1_drx_require_next_tick(gas_id);
             }
         }
         else
         {
            gl1_drx_require_next_tick(gas_id);
         }
      }
      else
      {
         gl1_drx_require_next_tick(gas_id);
      }

   }

  /*
   *  Save off the last sub sub state.  This is required because during
   *  background HPLMN searching, both idle mode and the HPLMN search
   *  state machine are running, so the states get overwritten
   */
   l1_local_idle_data_ptr->last_idle_sub_state = l1_tskisr_blk->sub_state;

   idle = FALSE;

  if(leaving_idle_mode == FALSE)
  {
    l1_local_idle_data_ptr->not_idle_cnt = 0;
  }

   /*
    * If either the GL1 or WCDMA wakeup timelines havn't completed then wait
    * otherwise the gl1_drx_off is called and we clear any callbacks and
    * the state can be incorrect for the next sleep attempt causing a crash
    */
#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
   /* for DUAL geran one SIM may be here whilst the other is in dedicated */
   if ( gl1_is_drx_on(gas_id) && !gl1_hw_wakeup_complete( gas_id ) )
#else
   if ( !gl1_hw_wakeup_complete( gas_id ) )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   {
      MSG_GERAN_LOW_1_G( "Waiting for wakeup to complete FN:%d", gl1_get_FN( gas_id ));
   }
   /* Return TRUE only if no serving and no neighbor cell activities
    * are in progress.
    */
   else if ( leaving_idle_mode )
   {
      if ( L1_background_HPLMN_search_running(gas_id) == TRUE )
      {
         L1_suspend_background_HPLMN_search(gas_id);
      }

      l1_sc_reset_state_machine_in_leave_idle(gas_id);

      idle = !l1_sci_is_op_in_progress(gas_id);

      /*Check if GL1 is stuck in this state because of unfinished IRAT activity*/
      gl1_check_if_irat_state_stuck(gas_id);

      idle &= ( L1_background_HPLMN_search_running(gas_id) == FALSE) ? TRUE : FALSE;

      if ((l1_local_idle_data_ptr->cbch_rx_in_progress) && (!l1_local_idle_data_ptr->cbch_abort_in_progress))
      {
         l1_local_idle_data_ptr->cbch_abort_in_progress = TRUE;

         /* Abort the message layer only when CBCH rx is in progress*/
         gl1_msg_abort_rx_cbch(gas_id);
      }
      else if( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_CALLBACK_PENDING )
      {
         MSG_GERAN_HIGH_0_G("Leaving idle , FM clear cbch activity");
         l1_clear_cbch_activity(gas_id);
      }

      idle &= (l1_local_idle_data_ptr->cbch_rx_in_progress == 0) ? TRUE : FALSE;

#ifdef FEATURE_QTA
     /* Don't IDLE abort until QTA is complete */
     idle &= !gl1_hw_qta_gap_active(gas_id);
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
      /* If there is an TRM/FW release pending remain in IDLE until this is processed by L1 Task. This
      is to prevent the non-IDLE state machines from proceeding based on l1_tskisr_blk->trm_status only
      to have TRM released from underneath them shortly afterwards. */
      idle &= (l1_tskisr_blk->firmware_load_status != FW_PENDING_RELEASE) ? TRUE : FALSE;

      /* If there is a firmware load in progress allow it to complete before aborting. This prevents
      leaving IDLE mode upon a FW load which will be followed immediatley by an unload
      if GL1_TRM_RETAINED_FOR_ACCESS is not set. This can result is the next state machine assuming all
      is well then having the lock and the FW removed whilst in use. CR240232 */
      idle &= (l1_tskisr_blk->firmware_load_status != FW_LOAD_IN_PROGRESS) ? TRUE : FALSE;

      idle &= !(l1_local_idle_data_ptr->idle_rx_in_progress || 
                               l1_local_idle_data_ptr->idle_monitors_in_progress);

      /* Abort BCCH/PCH Rx - but only if we have the lock */
      if ( gl1_msg_get_multi_sim_mode() )
      {
        /* In Dual sim operation only perform the msg layer abort if the lock is granted */
        if ( l1_tskisr_blk->firmware_loaded == TRUE )
        {
          /* Do not abort Idle Imm if CCCH in progress as could cause raise condition
          *  when doing deact ( stop_gsm_mode)
          */
          /*
          ** By checking IDLE = TRUE , keeping old behavior.
          */
#ifdef FEATURE_GPRS_GBTA
          if (IS_IDLE_EV_PWR_MON_SCHEDULED(gas_id)||IS_IDLE_EV_PWR_MON_ACTIVE(gas_id))
          {
            MSG_GERAN_HIGH_2_G("GL1 GBTA PWR_MON_SCHEDULED(T) %d idle %d",  idle_act_prog, idle );
            idle_act_prog = TRUE;
          }
#endif /* FEATURE_GPRS_GBTA */

          idle &= ! idle_act_prog;

          if(  idle == FALSE)
          {
            MSG_GERAN_HIGH_3_G( "Defer CCCH abort FN:%d idle_act_prog %d idle %d", gl1_get_FN( gas_id ), idle_act_prog, idle );

            /* Wait for ccch call back */
          }
          else
          {
             MSG_GERAN_MED_3_G( "gl1_msg_abort_rx_ccch FN:%d idle_act_prog %d idle %d", gl1_get_FN( gas_id ), idle_act_prog, idle );
             l1_local_idle_data_ptr->idle_rx_in_progress = 0 ;
             gl1_msg_abort_rx_ccch (gas_id);

#ifdef FEATURE_GPRS_GBTA
             if (IS_GBTA_ACTIVE())
             {
               gpl1_gbta_reset_active_state(gas_id);
               grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);  // As previously it was GL1_TRM_GRANTED.
             }

             /* This allows not to schedule Idle Power monitor when programmed */
             grm_notify_event(GBTA_EV_NONE,TRUE,gas_id);
#endif  /*FEATURE_GPRS_GBTA */

          }
      if (l1_local_idle_data_ptr->idle_update_afc_tt)
          {
            l1_local_idle_data_ptr->idle_update_afc_tt = FALSE;
            MSG_GERAN_HIGH_3_G("DBG idle_update_afc_tt force Reset rx %d im %d tt %d",l1_local_idle_data_ptr->idle_rx_in_progress,
                 l1_local_idle_data_ptr->idle_monitors_in_progress,l1_local_idle_data_ptr->idle_update_afc_tt);
          }
          l1_fm_clear_required_frames(FM_PRI_CCCH, gas_id);
          
#if defined FEATURE_QTA
          if( l1_tskisr_blk->qta_in_progress)
          {
            grm_set_trm_release_required(TRUE, l1_tskisr_blk->client_id, gas_id);
          }
#endif
        }
#ifdef FEATURE_GPRS_GBTA
        else if (IS_IDLE_EV_PWR_MON_SCHEDULED(gas_id)|| IS_IDLE_EV_PWR_MON_ACTIVE(gas_id))
        {
            MSG_GERAN_HIGH_0_G("GL1 GBTA PWR_MON_SCHEDULED(T) - Wait for completion");
            idle = FALSE;
        }
#endif /* FEATURE_GPRS_GBTA */
        else if( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_CALLBACK_PENDING )
        {
              l1_fm_clear_required_frames(FM_PRI_CCCH, gas_id);
            }

#ifdef FEATURE_QTA
        /* do pending SCE stop when QTA is completed */
        if( l1_idle_data->sc_stop_pending && !l1_tskisr_blk->qta_in_progress )
        {
          l1_idle_data->sc_stop_pending = FALSE;

          /* Tell SCE to stop its idle mode procedures */
          l1_sc_stop(gas_id);
        }
#endif
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
 
	
         if(idle == FALSE) 
         { 
            MSG_GERAN_HIGH_1_G( "Defer CCCH abortFN: idle_abort %d", idle ); 
         } 
         else 
         { 
            /* Abort BCCH/PCH Rx */
            l1_local_idle_data_ptr->idle_rx_in_progress = 0 ;
            gl1_msg_abort_rx_ccch (gas_id);
            idle &=  (!l1_local_idle_data_ptr->idle_monitors_in_progress);
         }
      }

      if (idle)
      {
        /* When in dual sim operation with another active SIM, don't turn off the DRX. This prevents
           the other SIM from performing DRX. Also clear any loaded wakeup times for this client/SIM */
        /* if we need to Tx then set turn_tx_on to TRUE */

        /*In DR, the allocated rf device might be non Tx capable, therefore don't keep Tx On.*/
        if (((l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND) ||
             (l1_tskisr_blk->main_command == L1_ACCESS_COMMAND) ||
             (l1_tskisr_blk->main_command == L1_DEDICATED_COMMAND))
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            && (FALSE == gl1_drdsds_enabled(gas_id))
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
#ifdef FEATURE_GSM_RX_TX_SPLIT
            && (SYS_MODEM_DEVICE_MODE_SINGLE_SIM == gl1_msg_get_multi_sim_sys_mode())
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
            )
        {
          gl1_drx_off(TRUE, gas_id);
        }
        else
        {
          gl1_drx_off( FALSE, gas_id );
        }

        // Clear non drx timer flag during.
        l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag = FALSE;


        gl1_hw_wcdma_cleanup_reconf_state_at_idle_abort(gas_id);
      }
      else
      {
         l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

         l1_local_idle_data_ptr->not_idle_cnt++;

         #ifdef DEBUG_CCCH_ABORT
         /* if the  monscan_in_progress flag hasn't been reset after 20 frames,
            something is very wrong. Crash for debug. */
        if((l1_sc_globals_ptr->monscan_in_progress || l1_sc_globals_ptr->receive_in_progress) &&
           l1_local_idle_data_ptr->not_idle_cnt >= GL1_MAX_FRAMES_FOR_IDLE_ABORT)
         {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_FATAL("not_idle_cnt(%d) >= %d frames.  Crash for debug.",
                      l1_local_idle_data_ptr->not_idle_cnt, GL1_MAX_FRAMES_FOR_IDLE_ABORT, 0);
         }
         #endif

        if( l1_local_idle_data_ptr->not_idle_cnt == L1_IDLE_ABORT_MAX_FRAMES )
        {
          if(geran_get_nv_recovery_restart_enabled(gas_id))
          {
            l1_local_idle_data_ptr->not_idle_cnt = 0;
            MSG_GERAN_ERROR_0_G("Stuck while aborting idle mode");
            l1_call_panic_reset_in_task(L1_STUCK_IN_IDLE_MODE, gas_id);
          }
          else
          {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
            ERR_GERAN_FATAL_0_G("Stuck while aborting idle mode");
          }
        }
      }
      MSG_GERAN_HIGH_3_G("Idle abort:%d FN:%d #fr-not-idle:%d",
                         idle, gl1_get_FN( gas_id ), l1_local_idle_data_ptr->not_idle_cnt);
   }

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
   if ( (l1_get_background_wcdma_plmn_scan_active(gas_id)) && 
        (l1_fm_any_activity_running(gas_id) == FALSE) && 
        (l1_local_idle_data_ptr->background_wcdma_plmn_scan_suspend_ncell == FALSE) &&
        (!leaving_idle_mode) )
   {
      uint32 gap_to_next_activity = SUB_FN(l1_local_idle_data_ptr->next_req_frame, this_FN);
      
      /* Send MPH_INTERRAT_LIST_SRCH_TIME_IND to GRR only when there is more than 4.615 * 65 = 300 ms gap till next CCCH activity */
      if(gap_to_next_activity > MIN_REQUIRED_FRAMES_FOR_IRAT_PLMN_SEARCH)
       {
        l1_local_idle_data_ptr->ind_timer_value = gap_to_next_activity;
        l1_local_idle_data_ptr->ind_timer_FN = gl1_get_FN(gas_id);
        /*providing a lesser time for X-plmn search, on resuming and gsm places a new reservation
          Other tech will be informed erlier so that QTA gap be opened earlier.*/
        if(gl1_msg_get_multi_sim_mode())
        {
          l1_local_idle_data_ptr->ind_timer_value = SUB_FN(l1_local_idle_data_ptr->ind_timer_value, QTA_OVERHEAD);
        }        
        MSG_GERAN_HIGH_1_G("l1_local_idle_data_ptr->ind_timer_value=%d",(l1_local_idle_data_ptr->ind_timer_value*4.6));
        L1_send_MPH_INTERRAT_LIST_SRCH_TIME_IND((uint32)(l1_local_idle_data_ptr->ind_timer_value*4.6), gas_id);
        l1_local_idle_data_ptr->background_wcdma_plmn_scan_suspend_ncell = TRUE;
       }
   }

   if(l1_get_background_wcdma_plmn_scan_active(gas_id))
   {
       //Don't sleep when background WCDMA PLMN scan is active
       gl1_drx_require_next_tick(gas_id);
   }

#endif

  if (L1_MULTI_SIM_BGRND_HPLMN_NULL != gpl1_get_multi_sim_bplmn_state(gas_id))
  {
    if ( ( garb_intf_gas_is_xpg_coex_victim(gas_id) == TRUE ) && ( L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT == gpl1_get_multi_sim_bplmn_state(gas_id)) )
    {
      MSG_GERAN_HIGH_0(" BPLMN shall not prevent sleep when other RAT is in BG TRAFFIC ");    
    }
    else
    {
    gl1_drx_require_next_tick(gas_id);
    MSG_GERAN_HIGH_0("Do not sleep when BPLMN pending");
  }
  }

  if (l1_sc_g2x_shutdown_cnf_pending(gas_id))
  {
      gl1_drx_require_next_tick(gas_id);
      MSG_GERAN_HIGH_0("Do not sleep when G2X CNF pending");
  }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( (l1_local_idle_data_ptr->idle_rx_in_progress==0) &&
       (l1_local_idle_data_ptr->idle_monitors_in_progress==0) &&
       (l1_local_idle_data_ptr->idle_update_afc_tt ==FALSE)  &&
       (l1_local_idle_data_ptr->cbch_rx_in_progress ==0)  &&
       (!l1_sc_check_irat_active(gas_id))
     )
  {
    static uint32  idle_lockup_counter[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

    boolean        local_idle          = idle;

#ifdef FEATURE_GSM_COEX_SW_CXM
    /* Update FREQ ID if pending before going to sleep */
    garb_intf_notify_idle_update_reg_arfcns(leaving_idle_mode,gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

    /*l1_multi_sim_trm_manager will return FALSE if any activity is running or cleanup is still pending */
    idle &= l1_multi_sim_trm_manager(gas_id);

    if( ( FALSE == idle ) && ( FALSE != local_idle ) )
    {
      ++idle_lockup_counter[gas_id];

      if( idle_lockup_counter[gas_id] >= GL1_MAX_FRAMES_FOR_IDLE_ABORT )
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_FATAL( "fm idle_lockup_counter %d >= %d debug",
                   l1_local_idle_data_ptr->not_idle_cnt,
                   GL1_MAX_FRAMES_FOR_IDLE_ABORT,
                   0
                 );
      }
    }
    else
    {
      idle_lockup_counter[gas_id] = 0;
    }

  }
  else if ( l1_sc_check_irat_active(gas_id) )
  {
    gl1_drx_require_next_tick(gas_id);
    idle = FALSE;
  }
  else if( OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) && l1_get_idle_monitors_in_progress(gas_id))
  {
   /* idle sub montiors are getting scheduled in transfer sub idle frame, before completing monitors, idle sub should not go to sleep*/
   /*without FW deload if G is going to sleep then it hit the crash condition in drx manager*/
    gl1_drx_require_next_tick(gas_id);
  }

  if(idle )
  {
    /*re-initialise persistant TRM denial monitoring for PM's when exiting idle mode*/
    (void)grm_persistent_denial_monitoring(GRM_RESET_PERSISTENT_DENIAL_MONITORING,
                                           l1_tskisr_blk->client_id,
                                           GRM_NULL_COMMAND,
                                           gas_id);

    /*When leaving idle mode make sure the default RX alpha is used by GFW*/
    mdsp_update_shared_variable(GFW_RX_ALPHA,&use_idle_rx_alpha,gas_id);

    /*Re_Enable DR if disabled when Exiting Idle Mode */
    if(grm_device_config_is_drdsds()
        && !gl1_drdsds_enabled(gas_id))
    {
      L1_enable_dual_receive(l1_tskisr_blk->client_id, gas_id);
      MSG_GERAN_HIGH_0_G(GL1_DRDS_DBG_HDR"Re_Enable DR on Idle Exit");
    }
  }

#ifdef FEATURE_PBR_RANDOMIZATION
    if ( (idle != FALSE ) &&  ( grm_get_reason( l1_tskisr_blk->client_id, gas_id) == TRM_DEMOD_PAGE ) )
    {
      uint32 wakeup_identifier = grm_get_wakeup_identifier( l1_tskisr_blk->client_id, gas_id);		
      if ( ( wakeup_identifier != NOT_A_FRAME_NUMBER ) && IS_FRAME_NUM_LATER( wakeup_identifier,this_FN ) )
      {
        grm_inform_grant_event( l1_tskisr_blk->client_id,
                                gl1_ms_get_trm_resource(gas_id, TRM_DEMOD_PAGE),
                                l1_fm_get_trm_duration( gas_id ),
                                TRM_DEMOD_PAGE,
                                TRM_DENIED
                              );
      }
    }
#endif
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/

   /* Reset leaving_idle_mode flag to avoid problems in SDCCH NCELL activities */
   if( idle )
   {
     l1_fm_set_leaving_idle_mode(FALSE, gas_id);
   }

   return( idle );

}

/******************************************************************************
 *
 *  Function name:  calculate_gap_to_next_activity
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Return the gap to the next Rx activity.
 *  Its a wrapper over the old function as now we want to move TRM booking for CCCH forward if
 *  the CCCH activity was denied in the past. To achieve that, first we call the original function
 *  l1_idle_calculate_gap_to_next_activity() to find the gap, and then we will move it to next CCCH
 *  if previously we had detected cancellation for that CCCH.
 *
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr - pointer to the idle data store containing paging info etc
 *  frame_offset  - offset to cur FN (if we want to find next activity from that offset from cur FN)
 *
 *
 *  Returns:
 *  --------
 *  Gap to next activity as described above.  0 if no gap.
 *
 *****************************************************************************/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static uint32 calculate_gap_to_next_activity( idle_data_T  *idle_data_ptr, uint32 frame_offset, gas_id_t gas_id )
#else
static uint32 calculate_gap_to_next_activity( idle_data_T  *idle_data_ptr, gas_id_t gas_id )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
{
  uint32 gap;
  uint32 gap_old;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  gap_old = l1_idle_calculate_gap_to_next_activity( idle_data_ptr, frame_offset, gas_id );
  gap = l1_idle_wmgr_modify_gap_for_ccch( gap_old, idle_data_ptr, frame_offset, gas_id );
#else
  gap_old = l1_idle_calculate_gap_to_next_activity( idle_data_ptr, gas_id );
  gap = l1_idle_wmgr_modify_gap_for_ccch( gap_old, idle_data_ptr, 0, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  return gap;
}


/******************************************************************************
 *
 *  Function name:  l1_idle_calculate_gap_to_next_activity
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 * Calculates the number of frames between the current frame and the frame
 * the next receive starts in (frame where message layer driver called)
 *
 * e.g.
 *
 * F S C C C C F S C C C C F S C C C C
 *         ^ 1 2 3 4 5 6 7 8   ^
 *         |                   + next block to receive
 *         |
 *         + current frame
 *
 *  gap_to_next_activity = 8
 *
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr - pointer to the idle data store containing paging info etc
 *  frame_offset  - offset to cur FN (if we want to find next activity from that offset from cur FN)
 *
 *
 *  Returns:
 *  --------
 *  Gap to next activity as described above.  0 if no gap.
 *
 *****************************************************************************/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static uint32 l1_idle_calculate_gap_to_next_activity( idle_data_T  *idle_data_ptr, uint32 frame_offset, gas_id_t gas_id )
#else
static uint32 l1_idle_calculate_gap_to_next_activity( idle_data_T  *idle_data_ptr, gas_id_t gas_id )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
{
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   uint32 gap_to_next_activity = 0;
   uint32 gap_to_next_bcch     = 0;
   uint32 gap_to_next_ext_bcch = 0;
   uint32 current_frame_pos;
   uint32 paging_frame_pos  = 0;
   uint32 multiframe_div_51;
   uint32 paging_period;
   uint32 gap_to_next_starting_time;
   boolean all_paging_blocks;
   uint32 local_FN = gl1_get_FN( gas_id );
   uint32 local_FNmod51 = frame_counters[gas_id].FNmod51;
   boolean enable_ta_activity = FALSE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* frame_offset - allows a frame number to be loaded in so that the next activity
      after this offset can be found */
   local_FN = ADD_FN(local_FN, frame_offset);
   local_FNmod51     = (local_FNmod51 + frame_offset)%MULTIFRAME_51;

   /* For LTA and QTA remain in NORMAL_PAGING until we have the TRM lock */
   all_paging_blocks = ( idle_data_ptr->paging_data.all_paging_blocks && !(l1_tskisr_blk->qta_in_progress)&&
                         (  (gl1_msg_get_multi_sim_standby_mode() && (gl1_get_num_inactive_subs() != 0 ))
                           || ((grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_GRANTED)
#ifdef FEATURE_G2X_REORG_TUNEAWAY
                           || gl1_hw_is_g2x_reorg_ta_enabled()
#endif
                            )) );
   l1_tskisr_blk->reservation_for_bcch = FALSE;
#else
   all_paging_blocks = idle_data_ptr->paging_data.all_paging_blocks;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   multiframe_div_51 = local_FN / MULTIFRAME_51;

   /* Check if the mm non-drx mode has been enabled */
   if (l1_idle_data->mm_non_drx_mode)
   {
      l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag = TRUE;

#ifdef FEATURE_G2X_IDLE_TUNEAWAY
      MSG_GERAN_LOW_1_G("IDLE TA activity ta_fn %d",l1_local_idle_data_ptr->l1_idle_ta.ta_fn);
        
      if ((TRUE == gl1_is_geran_tuneaway_allowed(gas_id))&&
          (FALSE == l1_tskisr_blk->current_params.L1Data.pIdle_data->non_updated_lai_in_nmo1)&&
          (l1_local_idle_data_ptr->l1_idle_ta.ta_fn != GL1_DEFS_INVALID_FN )&&
           IS_FRAME_NUM_LATER( ADD_FN(l1_local_idle_data_ptr->l1_idle_ta.ta_fn,1), local_FN )
         )
      {
        enable_ta_activity = TRUE;
        MSG_GERAN_HIGH_0_G("IDLE TA activity enabled");
      }
#endif /* FEATURE_G2X_IDLE_TUNEAWAY */
   }
   else
#ifdef FEATURE_G2X_REORG_TUNEAWAY
   if (idle_data_ptr->paging_data.paging_mode == PAGING_REORGANIZATION)
   {
      MSG_GERAN_LOW_1_G("IDLE TA activity ta_fn %d",l1_local_idle_data_ptr->l1_idle_ta.ta_fn);
      if ( ( l1_local_idle_data_ptr->l1_idle_ta.ta_fn != GL1_DEFS_INVALID_FN )
            && IS_FRAME_NUM_LATER( ADD_FN(l1_local_idle_data_ptr->l1_idle_ta.ta_fn,1), local_FN )
         )
      {
        enable_ta_activity = TRUE;
        MSG_GERAN_HIGH_0_G("IDLE TA activity enabled");
      }
    }
    else
#endif /*FEATURE_G2X_REORG_TUNEAWAY */
    {
      /*neither in Non Drx Mode nor PageReorg mode*/
      if ( l1_local_idle_data_ptr->l1_idle_ta.ta_fn != GL1_DEFS_INVALID_FN )
      {
          enable_ta_activity = FALSE;
          l1_local_idle_data_ptr->l1_idle_ta.ta_prepared = FALSE;
          MSG_GERAN_HIGH_2_G("Reset ta_fn %d and ta_prepared %d",l1_local_idle_data_ptr->l1_idle_ta.ta_fn,l1_local_idle_data_ptr->l1_idle_ta.ta_prepared);
          l1_local_idle_data_ptr->l1_idle_ta.ta_fn = GL1_DEFS_INVALID_FN;
          
      }
    }

   if ((all_paging_blocks)||
       l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag        ||
       ( l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag ) )
   {
     if(idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB)
     {
       if ( local_FNmod51 < 18 ) /* 18 to allow for early paging decode */
       {
         gap_to_next_activity =  0;
       }
       else if(all_paging_blocks)
       {
         gap_to_next_activity = MULTIFRAME_51 - local_FNmod51;
       }
       else if( l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag ||
         l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag )
       {
           gap_to_next_activity = MULTIFRAME_51 - local_FNmod51 + SKIP_BCCH_BLOCK;
       }
   } /* all_paging */
     /*else gap_to_next_activity is 0 */
   }
   else
   {
      /*
        In case paging parameters are unknown i.e. if the SIM is invalid
        from the start, use default values
      */
      if ( idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 0 )
      {
         idle_data_ptr->campedon_cell_data.BS_PA_MFRMS = 2;
         MSG_GERAN_ERROR_0_G("BS_PA_MFRMS was 0.  Setting to 2.");
      }

      /* Start by working out the gap to the next paging block.
       * This calculation is performed modulo BS_PA_MFRMS*MULTIFRAME_51,
       * i.e. modulo the paging period.
       */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      paging_period   = (idle_data_ptr->campedon_cell_data.BS_PA_MFRMS * l1_get_page_skipping_factor(idle_data_ptr, gas_id))*MULTIFRAME_51;
#else
      paging_period   = (idle_data_ptr->campedon_cell_data.BS_PA_MFRMS )*MULTIFRAME_51;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      current_frame_pos = local_FN % paging_period;

      if (idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING)
      {
          paging_frame_pos  = (idle_data_ptr->paging_data.ext_paging_multiframe * 51) +
                              paging_frame[idle_data_ptr->paging_data.ext_paging_block_index];
      }
      else
      {
          paging_frame_pos  = (idle_data_ptr->paging_data.paging_multiframe * 51) +
                              paging_frame[idle_data_ptr->paging_data.paging_block_index];
      }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      /* If  we are in page-reorg mode when W is in BG traffic
	 dont wait until next own page block or dont request for the immediate block( W needs minum time to open the gap). so requesting
      for CCCH block in nexxt multi frame as LTA gap */
      if( ((gl1_get_num_inactive_subs() > 0 )) &&
      ( idle_data_ptr->paging_data.paging_mode == PAGING_REORGANIZATION ))
      {
        paging_period = MULTIFRAME_51;
        /* In this case we need to read all paging blocks*/
        current_frame_pos = local_FN % paging_period;
        paging_frame_pos = paging_frame[idle_data_ptr->paging_data.paging_block_index];
        MSG_GERAN_HIGH_2_G("paging_period changed to %d paging_frame_pos %d", paging_period, paging_frame_pos);
      }
#endif

      /* required because of definition of gap */
      current_frame_pos +=  BLOCK_ALIGN_OFFSET;

      if (current_frame_pos > paging_period)
      {
         current_frame_pos -= paging_period;
      }

      if ( paging_frame_pos >= current_frame_pos )
      {
         gap_to_next_activity = paging_frame_pos - current_frame_pos;
      }
      else
      {
         gap_to_next_activity = (paging_frame_pos + paging_period) - current_frame_pos;
      }

    /* Check for hyperframe wrap around.  If FRAMES_IN_HYPERFRAME is not a
       * multiple of BS_PA_MFRMS then the modulo BS_PA_MFRMS*MULTIFRAME_51 calculation
       * above will give the wrong gap.
       */
      if ( (local_FN + gap_to_next_activity) >= (uint32)(FRAMES_IN_HYPERFRAME) )
      {
         /* The next paging block is after the FN wrap around. Do
          * calculation again, taking this into account.
          */
         gap_to_next_activity = (FRAMES_IN_HYPERFRAME + paging_frame_pos) - local_FN - BLOCK_ALIGN_OFFSET;
      }
   } /* end of not all_paging */

      /* Work out gap to next normal BCCH block to receive
       * if any.
       */
      if (idle_data_ptr->BCCH_mask != SERV_BCCH_MASK_NONE)
      {
         uint32 BCCH_mask;
         uint32 TC;
         uint32 gap_to_next_block_0;
         uint32 TC_gap;

         /* Work out gap to next block received by adding
          * gap to next block 0 to 51 * number of mframes
          * until next TC value.
          */
         BCCH_mask = idle_data_ptr->BCCH_mask;
         TC        = multiframe_div_51 % 8;

         if (local_FNmod51 > 0)
         {
            /* next block 0 is in next multiframe */
            gap_to_next_block_0 = MULTIFRAME_51 - local_FNmod51;
            TC++;
            TC %= 8;
         }
         else
         {
            /* next block 0 is in current multiframe */
            gap_to_next_block_0 = 0;
         }

         for (TC_gap=0; TC_gap<8; TC_gap++)
         {
            if ( (BCCH_mask & (1uL<<TC)) != 0 )
            {
               /* we found the next multiframe to
                * receive on
                */
               break;
            }

            TC++;
            TC %= 8;
         }

         gap_to_next_bcch = gap_to_next_block_0 + ( MULTIFRAME_51 * TC_gap );

      }
      else
      {
         /*no BCCH, so set gap_to_next_bcch to same as next activity*/
         gap_to_next_bcch = gap_to_next_activity;
      }

      /* Work out gap to next extended BCCH block to receive
       * if any.
       */
      if (idle_data_ptr->ext_BCCH_mask != SERV_EXT_BCCH_MASK_NONE)
      {
         uint32 ext_BCCH_mask;
         uint32 TC;
         uint32 gap_to_next_block_1;
         uint32 TC_gap;

         /* Works the same as the code for BCCH above
          * except that ext BCCH is on block 1 instead
          * of block 0.
          */
         ext_BCCH_mask = idle_data_ptr->ext_BCCH_mask;
         TC            = multiframe_div_51 % 8;

         if (local_FNmod51 <= 4)
         {
            /* next block 1 is in current multiframe */
           gap_to_next_block_1 = 4 - local_FNmod51;
         }
         else
         {
            /* next block 1 is in next multiframe */
            gap_to_next_block_1 = 55 - local_FNmod51;
            TC++;
            TC %= 8;
         }

         for (TC_gap=0; TC_gap<8; TC_gap++)
         {
            if ( (ext_BCCH_mask & (1uL<<TC)) != 0 )
            {
               /* we found the next multiframe to
                * receive on
                */
               break;
            }

            TC++;
            TC %= 8;
         }

         gap_to_next_ext_bcch = gap_to_next_block_1 + ( MULTIFRAME_51 * TC_gap );
      }
      else
      {
         gap_to_next_ext_bcch = gap_to_next_activity;
      }

      /* Take the minimum of the gaps */
      if (gap_to_next_bcch < gap_to_next_activity)
      {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         l1_tskisr_blk->reservation_for_bcch= TRUE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

         gap_to_next_activity = gap_to_next_bcch;
      }

      if (gap_to_next_ext_bcch < gap_to_next_activity)
      {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         l1_tskisr_blk->reservation_for_bcch= TRUE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

         gap_to_next_activity = gap_to_next_ext_bcch;
      }

#if defined( FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
      /*  check if next CCCH overlaps with the TA activity */
      if ( enable_ta_activity )
      {
        uint32 ta_activity_fn = l1_local_idle_data_ptr->l1_idle_ta.ta_fn;

        /* if next idle frame has already been determined then return the current gap to next activity
         */
        if ( l1_local_idle_data_ptr->l1_idle_ta.ta_prepared )
        {
          MSG_GERAN_HIGH_3_G("IDLE TA active ta_fn = %d gap_to_next_activity %d FN %d",l1_local_idle_data_ptr->l1_idle_ta.ta_fn,gap_to_next_activity,gl1_get_FN( gas_id ));
        }
        else
        {
          uint32 idle_activity_fn = gl1_get_FN( gas_id ) + gap_to_next_activity;
          uint32 gap_to_pch = 0;
          uint32 gap_to_bcch = 0;

#if defined(FEATURE_G2X_REORG_TUNEAWAY)
          if (idle_data_ptr->paging_data.paging_mode == PAGING_REORGANIZATION)
          {
            gap_to_pch  = calculate_gap_to_next_page( idle_data_ptr,NORMAL_PAGING,gas_id);
            gap_to_bcch = L1_gap_to_next_bcch( gl1_get_FN( gas_id ), l1_idle_data->BCCH_mask, l1_idle_data->ext_BCCH_mask, gas_id );
          if(gap_to_bcch >= gap_to_pch)
          {
		    idle_activity_fn = gl1_get_FN( gas_id ) + gap_to_pch; 
          }
		  else
		  {
		    idle_activity_fn = gl1_get_FN( gas_id ) + gap_to_bcch;
		  }
          }
#endif
          MSG_GERAN_HIGH_3_G("IDLE TA idle_activity_fn %d ta_activity_fn %d FN %d",idle_activity_fn,ta_activity_fn,gl1_get_FN( gas_id ));

          if ( IS_FRAME_NUM_LATER( ta_activity_fn, gl1_get_FN( gas_id ) )  )              
          {
            /* if there is still a big gap, continue with CCCH RX */
            if (  (SUB_FN(ta_activity_fn, gl1_get_FN( gas_id )) ) <= 5 )
            {
              
               
             if(!(!(l1_idle_data->mm_non_drx_mode)&&
                     IS_FRAME_NUM_LATER_OR_EQUAL(idle_activity_fn, ta_activity_fn)&&
                    !IS_FRAME_NUM_LATER(idle_activity_fn, ADD_FN(ta_activity_fn,L1_GAP_TO_TUNEAWAY_FN))
                    
                    ))
             {
              
            
             if(!(!(l1_idle_data->mm_non_drx_mode)&&
                      IS_FRAME_NUM_LATER(ta_activity_fn,idle_activity_fn)&&
                    !IS_FRAME_NUM_LATER(ta_activity_fn, ADD_FN(idle_activity_fn,L1_GAP_TO_TUNEAWAY_FN))
                    ))
             {
              
              /* prepared for TA */
              l1_local_idle_data_ptr->l1_idle_ta.ta_prepared = TRUE;
              if(l1_idle_data->mm_non_drx_mode)
              {
               l1_local_idle_data_ptr->l1_idle_ta.ta_gap_started= TRUE;
               l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn = gl1_get_FN( gas_id );
               
              }
              MSG_GERAN_HIGH_2_G("IDLE TA :: TA possible :: gap_to_next_activity %d  GAP_START_FN = %d", 
                                gap_to_next_activity,l1_local_idle_data_ptr->l1_idle_ta.ta_gap_start_fn);
                        }
                        }
                }
            
          }
          
        }
      }
#endif /* FEATURE_G2X_IDLE_TUNEAWAY */


   return gap_to_next_activity;
}


/******************************************************************************
 *
 *  Function name:  l1_idle_wmgr_modify_gap_for_ccch
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Moves the CCCH to next CCCH if TRM for CCCH was denied in the past
 *
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr - pointer to the idle data store containing paging info etc
 *  frame_offset  - offset to cur FN (if we want to find next activity from that offset from cur FN)
 *
 *
 *  Returns:
 *  --------
 *  Gap to next activity as described above.  0 if no gap.
 *
 *****************************************************************************/
static uint32 l1_idle_wmgr_modify_gap_for_ccch( uint32 gap, idle_data_T  *idle_data_ptr, uint32 frame_offset, gas_id_t gas_id )
{
  uint32 this_FN;
  uint32 new_wkpt_ccch_fn;
  uint32 next_allowed_ccch_fn;
  uint32 gap_old;
  uint32 gap_interim;
  int32 dist_can_ccch_to_new_wkpt_ccch_fn;
  int32 dist_can_ccch_to_this_fn;

  this_FN = gl1_get_FN( gas_id );

  /* find next CCCH based on caller's provided gap */
  new_wkpt_ccch_fn = ADD_FN(this_FN, (frame_offset+gap+1) );

  /* find earliest CCCH FN which is allowed */
  next_allowed_ccch_fn = gl1_hw_sleep_next_allowed_ccch_fn[gas_id];

  /* check if there was some wkup cancellation */
  if(next_allowed_ccch_fn != NOT_A_FRAME_NUMBER)
  {
    /* find the distance between ccch fn and allowed ccch fn */
    dist_can_ccch_to_new_wkpt_ccch_fn = SUB_FN(next_allowed_ccch_fn, new_wkpt_ccch_fn);

    /* Check if gl1_hw_sleep_next_allowed_ccch_fn is valid.
     * (Cancelled wkup pt can be upto 459 frames in the future (for MFRM 9))
     */
    if( (dist_can_ccch_to_new_wkpt_ccch_fn > 0) && (dist_can_ccch_to_new_wkpt_ccch_fn < MAX_DIST_FROM_CAN_CCCH) )
    {
      /* gap is modified only if wkup pt found by L1 is before the next_allowed_ccch_fn
       *
       * CCCH is at next_allowed_ccch_fn
       * But, if we call l1_idle_calculate_gap_to_next_activity() at CCCH FN, it will give next CCCH.
       * So, this_FN + frame Offset should be one less than next_allowed_ccch_fn.
       * eg. for CCCH at 1800, if this_FN is 1799, it will return gap as 0.
       *                       if this_FN is 1800, then it will return gap as 50.
       */
      dist_can_ccch_to_this_fn = SUB_FN(next_allowed_ccch_fn, this_FN) - 1;

      /* store old gap for debugging */
      gap_old = gap;

      /* find next activity at this_FN + */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      gap_interim = l1_idle_calculate_gap_to_next_activity( idle_data_ptr, frame_offset + dist_can_ccch_to_this_fn, gas_id );
#else
      gap_interim = l1_idle_calculate_gap_to_next_activity( idle_data_ptr, gas_id );
#endif
      gap = gap_interim + dist_can_ccch_to_this_fn;

      MSG_GERAN_MED_1_G("WMGR_SLEEP: gap_i: %d", gap_interim);
      MSG_GERAN_MED_5_G("WMGR_SLEEP: Moving CCCH Wkpt from %u to %u (earliest: %u), gap_o: %d, gap_n: %d",
        new_wkpt_ccch_fn, ADD_FN(this_FN, gap+1), next_allowed_ccch_fn, gap_old, gap);
    }   /* if( (dist_can_ccch_to_new_wkpt_ccch_fn > 0) && (dist_can_ccch_to_new_wkpt_ccch_fn < 500) ) */
  }     /* if(next_allowed_ccch_fn != NOT_A_FRAME_NUMBER)                     */

  return gap;
}


/******************************************************************************
 *
 *  Function name:  l1_sc_get_gap_to_next_activity
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Return the gap to the next Rx activity.
 *
 *
 *  Parameters:
 *  -----------
 *  None
 *
 *
 *  Returns:
 *  --------
 *  Gap to next activity as described above.  0 if no gap.
 *
 *****************************************************************************/
uint32 l1_sc_get_gap_to_next_activity( gas_id_t gas_id )
{
  return gl1_idle_get_gap_to_next_activity(0, gas_id);
}

/******************************************************************************
 *
 *  Function name:  l1_get_gap_to_next_activity
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Return the gap to the next Rx activity.
 *
 *
 *  Parameters:
 *  -----------
 *  None
 *
 *
 *  Returns:
 *  --------
 *  Gap to next activity as described above.  0 if no gap.
 *
 *****************************************************************************/
boolean l1_sc_is_idle_rx_in_progress( gas_id_t gas_id )
{
    l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

    return (l1_local_idle_data_ptr->idle_rx_in_progress ? TRUE : FALSE);
}

/******************************************************************************
 *
 *  Function name:  idle_multiframe_state_machine
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Determines the start of a CCCH Rx block and issues command to NPL1
 *  to start the operation.
 *
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr - pointer to the idle data store containing paging info etc
 *
 *
 *  Returns:
 *  --------
 *  Start of an CCCH Rx block
 *
 *****************************************************************************/

static boolean idle_multiframe_state_machine( idle_data_T  *idle_data_ptr, gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   ARFCN_T           arfcn, rx_arfcns[4];
   sys_algo_agc_T    *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   sys_algo_agc_T    *agc_ptr_divrx_ccch; // added CCCH temporarily for diferentiating on the other agc pointers.
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
   uint8             own_paging_block;
   boolean           start_rx;
   gl1_msg_chan_type chan_type = GL1_MSG_CCCH;

   /* Code */
   start_rx = FALSE;

   own_paging_block = FALSE;

   /* Determine if a CCCH rx in the next frame, so that we can set
    * dl_block_start and start the receive.  The previous call is only
    * during background HPLMN searches and is only predictive.  Doesn't
    * schedule a rx
    */
   start_rx = is_ccch_rx_block( idle_data_ptr, &own_paging_block, &chan_type, gas_id );

#if defined(FEATURE_GSM_TDS_DEBUG) || defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
   if ( start_rx )
   {
     MSG_GERAN_HIGH_3_G( "start_rx %d own_paging_block %d chan_type %d",
                         start_rx, own_paging_block, chan_type );
   }
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* as a precaution don't allow any scheduling unless FW is loaded up in Dual Sim modes */
   if ( gl1_msg_get_multi_sim_mode() && ( (l1_tskisr_blk->firmware_loaded == FALSE) || (l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)))
   {
     MSG_GERAN_HIGH_0_G("Block start but no firmware loaded yet!");
     start_rx = FALSE;
   }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   if (start_rx)
   {
      if(l1_local_idle_data_ptr->tag)
      {
         l1_local_idle_data_ptr->idle_data_ptr_1 = idle_data_ptr;
         l1_local_idle_data_ptr->tag = 0;
      }
      else
      {
         l1_local_idle_data_ptr->idle_data_ptr_2 = idle_data_ptr;
         l1_local_idle_data_ptr->tag = 1;
      }

      l1_local_idle_data_ptr->idle_rpt.data       = &l1_local_idle_data_ptr->idle_decode_rpt;
      l1_local_idle_data_ptr->idle_rpt.metrics[0] = &l1_local_idle_data_ptr->idle_metrics_rpts[l1_local_idle_data_ptr->tag][0];
      l1_local_idle_data_ptr->idle_rpt.metrics[1] = &l1_local_idle_data_ptr->idle_metrics_rpts[l1_local_idle_data_ptr->tag][1];
      l1_local_idle_data_ptr->idle_rpt.metrics[2] = &l1_local_idle_data_ptr->idle_metrics_rpts[l1_local_idle_data_ptr->tag][2];
      l1_local_idle_data_ptr->idle_rpt.metrics[3] = &l1_local_idle_data_ptr->idle_metrics_rpts[l1_local_idle_data_ptr->tag][3];

      arfcn   = idle_data_ptr->campedon_cell_ARFCN;
      agc_ptr = &idle_data_ptr->campedon_cell_agc;

      if( FALSE == agc_ptr->valid )
      {
        MSG_GERAN_ERROR_0_G("re-initializing campedon cell agc" );

        sys_algo_agc_init(agc_ptr, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );
      }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
      agc_ptr_divrx_ccch = &idle_data_ptr->campedon_cell_agc_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

      write_arfcns( rx_arfcns, arfcn );

      /* For a CCCH setup the asynch int possible flag */
      if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) ||
           ( own_paging_block && gl1_msg_is_sleep_opt3_enabled( gas_id ) )
         )
      {
         l1_set_asynch_int_possible( gas_id );
      }

      gl1_msg_rx_ccch( rx_arfcns, agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                       agc_ptr_divrx_ccch,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                       l1_local_idle_data_ptr->tag, own_paging_block,
                       chan_type, &l1_local_idle_data_ptr->idle_rpt,
                       gas_id );

      l1_local_idle_data_ptr->idle_rx_in_progress++;

      if ( own_paging_block )
      {
#ifdef FEATURE_GPRS_GBTA
       /* Differ Idle Power Moniter */
     if ( gl1_msg_get_multi_sim_standby_mode() &&
          (OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)) &&
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
          /*Idle pwr monitors allowed in 2 phase access with this feature*/
          (!(OTHER_GSM_CLIENT_IN_SINGLE_BLOCK_ALLOC(OTHER_GAS_ID(gas_id)) && !IS_TWO_PHASE_ACCESS(OTHER_GAS_ID(gas_id)) )) &&
#else
          (!OTHER_GSM_CLIENT_IN_SINGLE_BLOCK_ALLOC(OTHER_GAS_ID(gas_id))) &&
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
          !(OTHER_SUB_RELEASE_PENDING()) &&
          gl1_gbta_schedule_idle_per_mon_if_allowed(gas_id))
       {
         GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_GBTA_Q_PWR_MON_ON_PCH, gas_id);
         grm_notify_event(GBTA_EV_IDLE_PWR_MON,FALSE,gas_id);
         if(gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY)
         {
           l1_sci_paging_block_gbta_tick(gas_id);
         }
       }
       else if( IS_IDLE_EV_PWR_MON_SET(gas_id) )
       {
         /*idle sub monitors are getting scheduled from transfer sub so don't give paging tick and increment surround tick*/
         gl1_allow_sci_inc_surround_tick_gbta(gas_id);
         MSG_GERAN_HIGH_0_G("GBTA -  Idle monitors to be scheduled from trans sub");
       }
       else
#endif /* FEATURE_GPRS_GBTA */
       {
         l1_sci_paging_block_tick(gas_id);
       }
      }
   }

   return start_rx;
}

/******************************************************************************
 *
 *  Function name:  l1_clear_cbch_activity
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Clears all CBCH activity bookings from the Frame Manager
 *
 *
 *  Parameters:
 *  -----------
 *  VOID
 *
 *
 *  Returns:
 *  --------
 *  NONE
 *
 *****************************************************************************/
void l1_clear_cbch_activity(gas_id_t gas_id)
{
  l1_fm_clear_required_frames( FM_PRI_CBCH, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  l1_fm_clear_required_frames( FM_PRI_MSIM_CBCH, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}

/******************************************************************************
 *
 *  Function name:  l1_run_cbch_state_machine
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Determines the start of a CBCH Rx block and issues command to NPL1
 *  to start the operation.
 *
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr - pointer to the idle data store containing CBCH info etc
 *
 *
 *  Returns:
 *  --------
 *  NONE
 *
 *****************************************************************************/

static void l1_run_cbch_state_machine( idle_data_T  *idle_data_ptr, gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   uint32          current_FN = gl1_get_FN( gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   boolean           isr_over_run = FALSE;
   ARFCN_T           arfcn, rx_arfcns[4];
   sys_algo_agc_T    *agc_ptr;
   /* Only set this when we have the actual CBCH block start FN */
   boolean         start_rx   = is_cbch_rx_block( idle_data_ptr, 0, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if (start_rx)
   {
        /* Make sure that FW/RF are initialised. */
        if ( gl1_msg_get_multi_sim_mode() && ( l1_tskisr_blk->firmware_loaded == FALSE ) )
        {
            MSG_GERAN_HIGH_1_G("FN = %d CBCH - init FW", gl1_get_FN( gas_id ));

            if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_NOT_GRANTED )
            {
                l1_tskisr_blk->firmware_loaded = TRUE;
#ifdef FEATURE_QTA
                if( l1_tskisr_blk->qta_in_progress )
                {
                    ARFCN_T serving_BCCH = gl1_get_serving_cell_arfcn(gas_id);

                    if(!gl1_hw_qta_fw_rf_init(FALSE,GL1_FW_QBTA_CM_START_UP_OFFSET,FALSE,serving_BCCH,FALSE,TRUE,gas_id))
                    {
                        l1_clear_cbch_activity(gas_id);
                        l1_tskisr_blk->firmware_loaded = FALSE;
                    }
                    else
                    {
                      /* Prevent FM from releasing in the same frame */
                      l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                     l1_fm_get_active_CBCH_pri( gas_id ), gas_id );
                    }
                }
                else
#endif /* FEATURE_QTA */
                {
                    /* Issue the call to init the firmware and RF */
                    gl1_hw_multi_sim_fw_rf_init(TRUE,
                                                l1_task_tcb_read(gas_id),
                                                gsm_l1_wait,
                                                L1_WAIT_HW_SIG,
                                                L1_TIMEOUT_HW_SIG,
                                                GL1_HW_INIT_RX_ONLY,
                                                l1_tskisr_blk->client_id,
                                                FALSE,
                                                gas_id);

                     /* Prevent FM from releasing in the same frame */
                     l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                    l1_fm_get_active_CBCH_pri( gas_id ), gas_id );
                }
            }
        }

        isr_over_run = gl1_check_possible_isr_overrun(gas_id);
		
        if ( gl1_msg_get_multi_sim_mode() && ( ( l1_tskisr_blk->firmware_loaded == FALSE ) || (TRUE == isr_over_run)) )
        {
          if(isr_over_run)
          {       
            l1_clear_cbch_activity(gas_id);
            gl1_drx_require_next_tick(gas_id);
          }
          start_rx = FALSE;
        }
		
   }
   MSG_GERAN_HIGH_3_G( L1_CBCH_HDR"start_rx:%d MSM %d FN %d", start_rx, gl1_msg_get_multi_sim_mode(), current_FN);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   if (start_rx)
   {
      if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) )
      {
        /* For a CBCH setup the asynch int possible flag */
        l1_set_asynch_int_possible( gas_id );
      }
      if ( l1_local_idle_data_ptr->tag )
      {
         l1_local_idle_data_ptr->idle_data_ptr_1 = idle_data_ptr;
         l1_local_idle_data_ptr->tag             = 0;
      }
      else
      {
         l1_local_idle_data_ptr->idle_data_ptr_2 = idle_data_ptr;
         l1_local_idle_data_ptr->tag             = 1;
      }

      l1_local_idle_data_ptr->idle_rpt.data       = &l1_local_idle_data_ptr->idle_decode_rpt;
      l1_local_idle_data_ptr->idle_rpt.metrics[0] = &l1_local_idle_data_ptr->idle_metrics_rpts[l1_local_idle_data_ptr->tag][0];
      l1_local_idle_data_ptr->idle_rpt.metrics[1] = &l1_local_idle_data_ptr->idle_metrics_rpts[l1_local_idle_data_ptr->tag][1];
      l1_local_idle_data_ptr->idle_rpt.metrics[2] = &l1_local_idle_data_ptr->idle_metrics_rpts[l1_local_idle_data_ptr->tag][2];
      l1_local_idle_data_ptr->idle_rpt.metrics[3] = &l1_local_idle_data_ptr->idle_metrics_rpts[l1_local_idle_data_ptr->tag][3];

      arfcn   = idle_data_ptr->campedon_cell_ARFCN;
      agc_ptr = &idle_data_ptr->campedon_cell_agc;

      if( FALSE == agc_ptr->valid )
      {
        MSG_GERAN_ERROR_0_G("re-initializing campedon cell agc" );

        sys_algo_agc_init(agc_ptr, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );
      }

      write_arfcns( rx_arfcns, arfcn );

      if ( l1_local_idle_data_ptr->receive_cbch_sdcch_4 )
      {
         /*
          * CBCH is always on time slot 0 for SDCCH_4
          */
         int16 timing_offset =
           ( QS_PER_FRAME - (int16)(625 * ( (idle_data_ptr->paging_data.ccch_group << 1 ) ) ) );
         gl1_msg_store_cbch_TSC( idle_data_ptr->campedon_cell_data.cbch_channel_information.TSC,gas_id );

         gl1_msg_rx_cbch( rx_arfcns, agc_ptr, timing_offset, &l1_local_idle_data_ptr->idle_rpt,gas_id );

         /* CBCH is handled by the NPL1 as an ncell activity */
         gl1_set_ncell_block_start( TRUE,gas_id );

         l1_local_idle_data_ptr->cbch_rx_in_progress++;
      }
      else if ( l1_local_idle_data_ptr->receive_cbch_sdcch_8 )
      {
         /*
          * CBCH may not be on timeslot 0
          */
         int16 timing_offset, offset;
         int8 i;

         agc_ptr = &l1_local_idle_data_ptr->cbch_agc;

         if ( l1_local_idle_data_ptr->CBCH_TN <= l1_local_idle_data_ptr->CCCH_TN )
         {
            timing_offset =
              ( QS_PER_FRAME - (int16)(625 * (l1_local_idle_data_ptr->CCCH_TN - l1_local_idle_data_ptr->CBCH_TN)) );

            offset = 2;
         }
         else
         {
            timing_offset =
              (int16)(625 * ( l1_local_idle_data_ptr->CBCH_TN - l1_local_idle_data_ptr->CCCH_TN ));

            offset = 1;
         }

         /*
          * Calculate the ARFCNS
          */
         for ( i = 0; i < 4; i++ )
         {
            rx_arfcns[i] =
              L1_calculate_ARFCN( (i+offset),
                &(idle_data_ptr->campedon_cell_data.cbch_channel_information), gas_id );
         }
         gl1_msg_store_cbch_TSC( idle_data_ptr->campedon_cell_data.cbch_channel_information.TSC,gas_id );

         gl1_msg_rx_cbch( rx_arfcns, agc_ptr, timing_offset, &l1_local_idle_data_ptr->idle_rpt,gas_id );

         /* CBCH is handled by the NPL1 as an ncell activity */
         gl1_set_ncell_block_start( TRUE,gas_id );

         l1_local_idle_data_ptr->cbch_rx_in_progress++;
      }
   }
}

/******************************************************************************
 *
 *  Function name:  is_ccch_rx_block
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Determines the start of a CCCH Rx block
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr    - pointer to the idle data store containing paging info etc
 *  own_paging_block - pointer to whether own paging block
 *  chan_type        - pointer to the channel type - whether CCCH or BCCH
 *
 *  Returns:
 *  --------
 *  Start of an CCCH Rx block = TRUE
 *               else = FALSE
 *****************************************************************************/
boolean is_ccch_rx_block( idle_data_T       *idle_data_ptr,
                                 uint8             *own_paging_block,
                                 gl1_msg_chan_type *chan_type,
                                 gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   boolean           own_paging_multiframe;
   boolean           extended_paging;
   boolean           extended_paging_multiframe;
   uint8             paging_block_index;
   uint8             extended_paging_block_index;
   uint32            TC;
   uint8             multiframe;
   boolean           start_rx;
   uint32            this_FN = gl1_get_FN( gas_id );
   uint8             missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);
   uint8             this_FNmod102;

   /* Code */
   start_rx = FALSE;

   /* Offset the block starting frame by the number of missed frames due to
    * sleep wakeup error recovery.
    * There is no need to schedule RX when missed 3 frames as it needs
    * at least two bursts to decode xCCH. */
   if ( missed_frames >= 3 )
   {
       return ( FALSE );
   }
   this_FN = SUB_FN(this_FN, missed_frames);
   this_FNmod102 = ( this_FN % ( MULTIFRAME_51 * 2 ) );

   /*
      If SIM becomes invalid, use paging parameters from before.
      Don't stop decoding paging blocks.  This will ensure that
      we continue shipping up measurements to RR so that we can
      reselect if need be.
   */
   if ( idle_data_ptr->imsi_data.imsi_valid == FALSE )
   {
      idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;
   }

   paging_block_index = idle_data_ptr->paging_data.paging_block_index;
   extended_paging_block_index = idle_data_ptr->paging_data.ext_paging_block_index;

   /*
      In case paging parameters are unknown i,e if the SIM is invalid
      from the start, use default values
   */
   if ( idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 0 )
   {
      idle_data_ptr->campedon_cell_data.BS_PA_MFRMS = 2;
      MSG_GERAN_ERROR_0_G("BS_PA_MFRMS was 0.  Setting to 2.");
   }

   /* controlled by the mod51 frame number */
   /* Add offset to predict in future */
   TC         = ( this_FN / MULTIFRAME_51 );

   multiframe = (uint8)(TC % idle_data_ptr->campedon_cell_data.BS_PA_MFRMS);
   TC        %= 8;

   if ( idle_data_ptr->paging_data.paging_multiframe == multiframe )
   {
      own_paging_multiframe = TRUE;
   }
   else
   {
      own_paging_multiframe = FALSE;
   }

   extended_paging_multiframe =
      (( idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING ) &&
       ( idle_data_ptr->paging_data.ext_paging_multiframe == multiframe ));

   /* Add the offset if oassed in to predict if CCCH block is at this offset */
   switch (  this_FNmod102 )
   {
      case 1:
      case 52:
         /* Coming up is the BCCH */
         if( ( (idle_data_ptr->BCCH_mask & (1uL<<TC)) != 0 )||
               (idle_data_ptr->paging_data.all_paging_blocks ))
         {
            *chan_type = GL1_MSG_BCCH;
            start_rx  = TRUE;
         }
         break;

      case 5:
      case 56:
         /* Coming up is block 0 for the CCCH */
         *own_paging_block = ((paging_block_index == 0) &&
                              own_paging_multiframe);
         extended_paging = ((extended_paging_block_index == 0) &&
                            extended_paging_multiframe);

         /* Check for BCCHext */
         if ( (idle_data_ptr->ext_BCCH_mask & (1uL<<TC)) != 0 )
         {
            *chan_type = GL1_MSG_BCCH;
            start_rx  = TRUE;
         }
         /* Otherwise this block is available for paging */
         if (((idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES < 1) &&
                (*own_paging_block||extended_paging)) ||
                idle_data_ptr->paging_data.all_paging_blocks)
         {
           *chan_type = GL1_MSG_CCCH;
           start_rx  = TRUE;
         }
         else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
         {
           *chan_type = GL1_MSG_CCCH;
            start_rx  = TRUE;
         }
         break;

      case 11:
      case 62:
         /* Coming up is block 1 for the CCCH */

         *own_paging_block = ((paging_block_index == 1) &&
                              own_paging_multiframe);
         extended_paging = ((extended_paging_block_index == 1) &&
                            extended_paging_multiframe);

         if (((idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES < 2) &&
              (*own_paging_block||extended_paging)) ||
              idle_data_ptr->paging_data.all_paging_blocks)
         {
           *chan_type = GL1_MSG_CCCH;
           start_rx  = TRUE;
         }
         else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
         {
           *chan_type = GL1_MSG_CCCH;
           start_rx  = TRUE;
         }

         break;

      case 15:
      case 66:
         /* Coming up is block 2 for the CCCH */

         *own_paging_block = ((paging_block_index == 2) &&
                              own_paging_multiframe);
         extended_paging = ((extended_paging_block_index == 2) &&
                            extended_paging_multiframe);

         if (idle_data_ptr->paging_data.all_paging_blocks)
         {
           /*receive any PCH block*/
           *chan_type = GL1_MSG_CCCH;
           start_rx  = TRUE;
         }
         else if ((idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES < 3) &&
                  (*own_paging_block||extended_paging))
         {
           /*receive own paging block*/
           *chan_type = GL1_MSG_CCCH;
           start_rx  = TRUE;
         }
         else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
         {
           *chan_type = GL1_MSG_CCCH;
           start_rx  = TRUE;
         }

         break;

      case 21:
      case 72:
         /* Coming up is block 3 for the CCCH */
         if (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE)
         {
            *own_paging_block = ((paging_block_index == 3) &&
                                 own_paging_multiframe);
            extended_paging = ((extended_paging_block_index == 3) &&
                               extended_paging_multiframe);

            if (((idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES < 4) &&
                    (*own_paging_block||extended_paging)) ||
                idle_data_ptr->paging_data.all_paging_blocks)
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
         }
         break;

      case 25:
      case 76:
         /* Coming up is block 4 for the CCCH */
         if (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE)
         {
            *own_paging_block = ((paging_block_index == 4) &&
                                 own_paging_multiframe);
            extended_paging = ((extended_paging_block_index == 4) &&
                               extended_paging_multiframe);

            if (idle_data_ptr->paging_data.all_paging_blocks)
            {
               /*receive any PCH block*/
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if ((idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES < 5) &&
                        (*own_paging_block||extended_paging))
               {
               /*receive own paging block*/
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
         }
         break;

      case 31:
      case 82:
         /* Coming up is block 5 for the CCCH */
         if (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE)
         {
            *own_paging_block = ((paging_block_index == 5) &&
                                  own_paging_multiframe);
            extended_paging = ((extended_paging_block_index == 5) &&
                               extended_paging_multiframe);

            if (((idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES < 6) &&
                    (*own_paging_block||extended_paging)) ||
                idle_data_ptr->paging_data.all_paging_blocks)
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
         }
         break;

      case 35:
      case 86:
         /* Coming up is block 6 for the CCCH */
         if (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE)
         {
            *own_paging_block =  ((paging_block_index == 6) &&
                                  own_paging_multiframe);
            extended_paging = ((extended_paging_block_index == 6) &&
                               extended_paging_multiframe);

            if (idle_data_ptr->paging_data.all_paging_blocks)
            {
               /*receive any PCH block*/
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if ((idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES < 7) &&
                        (*own_paging_block||extended_paging))
               {
               /*receive own paging block*/
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
         }
         break;

      case 41:
      case 92:
         /* Coming up is block 7 for the CCCH */
         if (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE)

         {
            *own_paging_block =  ((paging_block_index == 7) &&
                                  own_paging_multiframe);
            extended_paging = ((extended_paging_block_index == 7) &&
                               extended_paging_multiframe);

            if (*own_paging_block ||
                idle_data_ptr->paging_data.all_paging_blocks||
                extended_paging)
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
         }
         break;

      case 45:
      case 96:
         /* Coming up is block 8 for the CCCH */
         if (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == FALSE)
         {
            *own_paging_block =  ((paging_block_index == 8) &&
                                  own_paging_multiframe);
            extended_paging = ((extended_paging_block_index == 8) &&
                               extended_paging_multiframe);

            if (idle_data_ptr->paging_data.all_paging_blocks)
            {
               /*receive any PCH block*/
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if (*own_paging_block||extended_paging)
            {
               /*receive own paging block*/
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
            else if ((l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) || (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag))
            {
               *chan_type = GL1_MSG_CCCH;
               start_rx  = TRUE;
            }
         }
         break;

      default:
         break;

   } /* end of switch */

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif
   return ( start_rx );
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/******************************************************************************
 *
 *  Function name:  is_own_ccch_rx_block
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Determines if the next receive is for the MS's designated CCCH Rx block, this
 *  is specifically for use in dual sim mode, as it will take into account the
 *  time required to load the firmware.
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr    - pointer to the idle data store containing paging info etc
 *
 *  Returns:
 *  --------
 *  Start of an CCCH Rx block = TRUE
 *               else = FALSE
 *****************************************************************************/
static boolean is_own_page_read( idle_data_T *idle_data_ptr, uint8 missed_frames, gas_id_t gas_id )
{

  uint32 current_frame_pos;
  uint32 paging_frame_pos;
  uint32 local_FN = SUB_FN(gl1_get_FN( gas_id ), missed_frames);
  uint32 paging_period;
  uint32 gap_to_next_own_paging_block;

  if ( idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 0 )
  {
    idle_data_ptr->campedon_cell_data.BS_PA_MFRMS = 2;
    MSG_GERAN_ERROR_0_G("BS_PA_MFRMS was 0.  Setting to 2.");
  }
  paging_period     = ( idle_data_ptr->campedon_cell_data.BS_PA_MFRMS * MULTIFRAME_51 );
  current_frame_pos = local_FN % paging_period;
  paging_frame_pos  = (idle_data_ptr->paging_data.paging_multiframe * MULTIFRAME_51) +
                      paging_frame[idle_data_ptr->paging_data.paging_block_index];

  if (current_frame_pos > paging_period)
  {
    current_frame_pos -= paging_period;
  }

  if ( paging_frame_pos >= current_frame_pos )
  {
    gap_to_next_own_paging_block = paging_frame_pos - current_frame_pos;
  }
  else
  {
    gap_to_next_own_paging_block = (paging_frame_pos + paging_period) - current_frame_pos;
  }

  /* Check for hyperframe wrap around.  If FRAMES_IN_HYPERFRAME is not a
   * multiple of BS_PA_MFRMS then the modulo BS_PA_MFRMS*51 calculation
   * above will give the wrong gap.
   */
  if ( (local_FN + gap_to_next_own_paging_block) >= (uint32)(FRAMES_IN_HYPERFRAME) )
  {
   /* The next paging block is after the FN wrap around. Do
    * calculation again, taking this into account.
    */
    gap_to_next_own_paging_block = (FRAMES_IN_HYPERFRAME + paging_frame_pos) - local_FN - 2;
  }


  MSG_GERAN_HIGH_1_G("next OPB=%d",gap_to_next_own_paging_block);


  return ( gap_to_next_own_paging_block <= ( MULTI_SIM_FW_LOAD_FRAMES + PRE_PROGRAMMING_FRAMES ) );
}

/******************************************************************************
 *
 *  Function name:  gpl1_multi_sim_get_next_block_start_gap
 *  ------------------------------------------------------
 *
 *  Description:
 *  ------------
 *  Calculates the gap until the next BCCH/CCCH activity. This includes the DSDS
 *  firmware load overhead time and also the concept of programming the frame
 *  before.
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr    - pointer to the idle data store containing paging info etc
 *
 *  Returns:
 *  --------
 *  Gap until the next time the trm request is needed.
 *****************************************************************************/
static uint32 gpl1_multi_sim_get_next_block_start_gap( idle_data_T *idle_data_ptr, gas_id_t gas_id )
{

  #define MAX_BCCH_CCCH_BLOCKS 12
  const uint8 block_position[MAX_BCCH_CCCH_BLOCKS] = {2,6,12,16,22,26,32,36,42,46,53,57};
  uint32 current_frame_pos = frame_counters[gas_id].FNmod51;
  int32 next_block_start_gap = 0;
  uint32 i;
  uint32 pre_block_frames = ( /*MULTI_SIM_FW_LOAD_FRAMES + */PRE_PROGRAMMING_FRAMES + current_frame_pos );
  l1_local_idle_data_t      *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  uint32 next_mf_start_block = 0;

  /*we do not need to read bcch block in non drx mode*/
  if((l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag||
      l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) &&
     (!idle_data_ptr->paging_data.all_paging_blocks))
  {
    i = 1;
    next_mf_start_block = 57; /*51+6*/
  }
  else
  {
    i = 0;
    next_mf_start_block = 53; /*51+2*/
  }

  for ( ; i < MAX_BCCH_CCCH_BLOCKS ; i++  )
  {
    if ( current_frame_pos <= block_position[i] )
    {
      break;
    }
  }

  if ( i >= MAX_BCCH_CCCH_BLOCKS)
  {
    i = MAX_BCCH_CCCH_BLOCKS - 1;
    MSG_GERAN_ERROR_0_G("Block index exceeded!!");
  }
  /******************************************************************************************
    Need to take away from the DL block start one frame for the pre programming frame
    the DSDS load time and then the current position. This gives the number of frames until
    then next request time.
      0 1 2 3 4 5 6 7 8 9 10 11 12 13
      F S B B B B C C C C F  S  C  C  C
                  ^
                  |current pos
                                ^
                                |Next DL block

    12 - 1 = 11 frame to program sync_rx up on
    11 - MULTI_SIM_FW_LOAD_FRAMES (say 1) = 10 frame to ask TRM for lock on
    10 - current pos (6 in example above) = 4
  *******************************************************************************************/
    /* Safety mechanism to make sure that the subtraction from block frames is always positive */
  for ( ; ( block_position[i] <= pre_block_frames ) && ( i < MAX_BCCH_CCCH_BLOCKS ) ; i++  )
    {
        MSG_GERAN_HIGH_3_G("readjust the block position=%d current_frame_pos=%d pre_block_frames=%d",block_position[i],current_frame_pos,pre_block_frames);

        if ( i >= ( MAX_BCCH_CCCH_BLOCKS - 1 ) )
        {
          break;
        }
    }

    if ( i >= MAX_BCCH_CCCH_BLOCKS)
    {
      i = MAX_BCCH_CCCH_BLOCKS - 1;
      MSG_GERAN_HIGH_0_G("Rounding off to Max Block index");
    }

    /*we do not need to read bcch block in non drx mode*/
    if ((l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag||
         l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag) && 
        (!idle_data_ptr->paging_data.all_paging_blocks) &&
        ((i==0)||(i==10))
       )
    {
      i++;
    }

    next_block_start_gap = block_position[i] - pre_block_frames - 1;

    /* for sdcch comb channel combination, next cch block after block starting from FN mod 51 = 16
      will be in the next multiframe(at FNmod 51 = 2)*/
    if( (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == TRUE) &&
        (block_position[i] > 16) )
    {
      next_block_start_gap = next_mf_start_block - pre_block_frames - 1;
      MSG_GERAN_MED_3_G("BS_CCCH_SDCCH_COMB - next_block_start_gap = %d currFNmod51 %d next_mf_start_block %d", next_block_start_gap, current_frame_pos, next_mf_start_block);
    }

  if(next_block_start_gap < 0)
  {
    next_block_start_gap =0;
  }

  MSG_GERAN_MED_2_G("next_block_start_gap=%d current_frame_pos=%d ",next_block_start_gap,current_frame_pos);

  return next_block_start_gap;

}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/******************************************************************************
 *
 *  Function name:  is_cbch_rx_block
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Determines the start of a CBCH Rx block
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr    - pointer to the idle data store containing paging info etc
 *  frame_offset     - the extra frame overhead for DSDS
 *
 *  Returns:
 *  --------
 *  Start of an CBCH Rx block = TRUE
 *    else = FALSE
 *****************************************************************************/

static boolean is_cbch_rx_block( idle_data_T *idle_data_ptr, int32 frame_offset, gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   idle_data_T               *l1_idle_data      = &l1_idle_data_store[gas_id];
   boolean start_rx               = FALSE;
   uint8   req_cb_frames             = L1_DURATION_CBCH;

   /*
    * For DSDS we need to allow for an offset that includes the firmware
    * download overhead
    */
   uint32           this_FN       = ADD_FN( gl1_get_FN( gas_id ), frame_offset );
   uint32           this_FNmod102 =
                      ADD_MOD( frame_counters[gas_id].FNmod102, frame_offset, ( MULTIFRAME_51 * 2 ) );
   l1_fm_priority_T pri_cbch      = l1_fm_get_active_CBCH_pri(gas_id);

#ifdef GL1_EXTENDED_CBCH_DBG
   MSG_GERAN_MED_3_G( L1_CBCH_HDR"is_cbch_rx_block FN:%d FNmod102:%d frame_offset:%d",
            this_FN, this_FNmod102, frame_offset );
#endif /* GL1_EXTENDED_CBCH_DBG */

   l1_local_idle_data_ptr->receive_cbch_sdcch_4 = FALSE;
   l1_local_idle_data_ptr->receive_cbch_sdcch_8 = FALSE;

   
   /* To Resolve KW issue*/
   if((pri_cbch != FM_PRI_CBCH )&& (pri_cbch != FM_PRI_MSIM_CBCH ) )
   {
       return FALSE;
   }

   l1_local_idle_data_ptr->CBCH_TN =
     idle_data_ptr->campedon_cell_data.cbch_channel_information.TN;

   l1_local_idle_data_ptr->CCCH_TN =
     (uint8)( idle_data_ptr->paging_data.ccch_group << 1 );

   if (l1_smscb_get_chan_type(gas_id) == SDCCH_8)
   {
      if (l1_idle_data->paging_data.paging_block_index == 0 ||
       l1_idle_data->paging_data.paging_block_index == 1)
      {
        /*To support parallel read of CBCH when CCCH overlaps with SDCCH/8*/
        req_cb_frames = L1_MIN_DURATION_CBCH;
      }
   }
   /* Add the offset if passed in to predict if CBCH block is at this offset */
   switch ( this_FNmod102 )
   {
      case 6:
      case 57:
         /*
          * Coming up is a CBCH block on SDCCH_8. Make sure that CBCH is only
          * scheduled if we are not receiving anything.
          * NOTE: Schedule CBCH decode in N-2 if CBCH Time Slot is less than
          * or equal to CCCH Time Slot.
          */
         if ((l1_smscb_get_chan_type(gas_id) == SDCCH_8) &&
              ( l1_local_idle_data_ptr->CBCH_TN <= l1_local_idle_data_ptr->CCCH_TN ) )
         {
            if ( l1_smscb_cbch_expected( ADD_FN( this_FN, 1 ), gas_id ) )
            {
            if ( !l1_local_idle_data_ptr->idle_rx_in_progress &&
                 !l1_local_idle_data_ptr->idle_monitors_in_progress &&
                    l1_fm_are_frames_available( pri_cbch, req_cb_frames, cbch_aborted, 0, gas_id ) )
               {
                    MSG_GERAN_MED_1_G( L1_CBCH_HDR"Scheduling: chan=SDCCH_8 fn=%d",
                             this_FN);

                    l1_fm_extend_required_frames( pri_cbch, L1_DURATION_CBCH, gas_id );

                    l1_local_idle_data_ptr->receive_cbch_sdcch_8 = start_rx = TRUE;
               }
               else
               {
                    MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"Pre-empting decode");

                    L1_send_CB_MISSING_BLOCK_IND( ADD_FN( this_FN, 2 ), gas_id );

                    l1_fm_clear_required_frames( pri_cbch, gas_id );
                  }
               }
            }
         break;

      case 7:
      case 58:
         /*
          * Coming up is a CBCH block on SDCCH_8. Make sure that CBCH is only
          * scheduled if we are not receiving anything.
          * NOTE: Schedule CBCH decode in N-1 if CBCH Time Slot is greater than
          * CCCH Time Slot.
         */
         if ((l1_smscb_get_chan_type(gas_id) == SDCCH_8) &&
              ( l1_local_idle_data_ptr->CBCH_TN > l1_local_idle_data_ptr->CCCH_TN ) )
         {
            if ( l1_smscb_cbch_expected(this_FN, gas_id))
            {
            if ( !l1_local_idle_data_ptr->idle_rx_in_progress &&
                 !l1_local_idle_data_ptr->idle_monitors_in_progress &&
                    l1_fm_are_frames_available( pri_cbch, req_cb_frames, cbch_aborted, 0, gas_id ) )
               {
                     MSG_GERAN_MED_1_G( L1_CBCH_HDR"Scheduling: chan=SDCCH_8 fn=%d",
                              this_FN);

                     l1_fm_extend_required_frames( pri_cbch, L1_DURATION_CBCH, gas_id );

                     l1_local_idle_data_ptr->receive_cbch_sdcch_8 = start_rx = TRUE;
               }
               else
               {
                     MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"Pre-empting decode");

                     L1_send_CB_MISSING_BLOCK_IND( ADD_FN( this_FN, 1 ), gas_id );

                     l1_fm_clear_required_frames( pri_cbch, gas_id );
                  }
               }
            }
         break;

      case 30:
      case 81:
         /* Coming up is block 5 CCCH */
         if ( idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB &&
             (l1_smscb_get_chan_type(gas_id) == SDCCH_4 ))
         {
            if ( l1_smscb_cbch_expected( ADD_FN( this_FN, 1 ), gas_id ) )
            {
            if ( !l1_local_idle_data_ptr->idle_monitors_in_progress &&
                    l1_fm_are_frames_available( pri_cbch,
                                                req_cb_frames,
                                                cbch_aborted, 0, gas_id ) )
               {
                  l1_local_idle_data_ptr->receive_cbch_sdcch_4 = TRUE;

                  start_rx = TRUE;

                  MSG_GERAN_MED_1_G( L1_CBCH_HDR"Scheduling: chan=SDCCH_4 fn=%d",
                           this_FN);
               }
               else
               {
                  MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"Pre-empting decode");

                  L1_send_CB_MISSING_BLOCK_IND( ADD_FN( this_FN, 2 ), gas_id );

                  l1_fm_clear_required_frames( pri_cbch, gas_id );
               }
            }
         }
         break;

      default:
         break;

   } /* end of switch */

   return ( start_rx );
}

/******************************************************************************
 *
 *  Function name:  cbch_aborted
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Callback to be called when an CBCH decode has to be aborted because
 *  layer1 needs frames that were previously available to the surround
 *  cell engine.
 *
 *  Parameters:
 *  -----------
 *  IN - dummy - Not used.
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/

static void cbch_aborted( void* dummy, gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   NOTUSED( dummy );
   ASSERT_INT();

   if ( l1_local_idle_data_ptr->cbch_rx_in_progress &&
        !l1_local_idle_data_ptr->cbch_abort_in_progress )
   {
     l1_local_idle_data_ptr->cbch_abort_in_progress = TRUE;

     /* Abort the message layer only when CBCH rx is in progress*/
     gl1_msg_abort_rx_cbch(gas_id);
   }
   else
   {
     /* no CBCH rx has started yet; clear the Frame Manager elements of CBCH */
     l1_clear_cbch_activity(gas_id);
   }
}

/******************************************************************************
 *
 *  Function name:  l1_initialise_cbch_state
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Initialises the CBCH L1 module to the CBCH_INIT state
 *
 *  Parameters:
 *  -----------
 *  NONE.
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/

static void l1_initialise_cbch_state( gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   l1_local_idle_data_ptr->cbch_state = CBCH_INIT;
}

/******************************************************************************
 *
 *  Function name:  l1_clear_cbch_scheduling
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Clears old scheduled CBCH if CBCH is not in receiving state
 *
 *  Parameters:
 *  -----------
 *  NONE.
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/
void l1_clear_cbch_scheduling( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  GL1_ISR_LOCK(gas_id);
  if( l1_local_idle_data_ptr->cbch_state == CBCH_WAITING )
  {
    gl1_drx_release_fn_tick( l1_local_idle_data_ptr->next_req_cbch_frame, gas_id );
    l1_local_idle_data_ptr->next_req_cbch_frame = MAX_CBCH_GAP_PERIOD;
  }
  GL1_ISR_UNLOCK(gas_id);
}

/******************************************************************************
 *
 *  Function name:  l1_add_ccch_drx_ref
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  l1_add_ccch_drx_ref
 *
 *  Parameters:
 *  -----------
 *  NONE.
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/
void l1_add_ccch_drx_ref( uint32 FN,gas_id_t gas_id)
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  
  GL1_ISR_LOCK(gas_id);

   /*  pass to drx manager              */
  l1_local_idle_data_ptr->drx_ref = gl1_drx_require_tick( FN, gas_id );
  

  GL1_ISR_UNLOCK(gas_id);
}

/******************************************************************************
 *
 *  Function name:  l1_clear_cbch_drx_ref
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Clears Only CBCH Drx Reference
 *
 *  Parameters:
 *  -----------
 *  NONE.
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/
void l1_clear_cbch_drx_ref( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  if( l1_local_idle_data_ptr->cbch_state == CBCH_WAITING && 
      l1_local_idle_data_ptr->next_req_cbch_frame != MAX_CBCH_GAP_PERIOD)
  {
    gl1_drx_release_fn_tick( l1_local_idle_data_ptr->next_req_cbch_frame, gas_id );
  }
}

/******************************************************************************
 *
 *  Function name:  l1_tick_cbch
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Called from within the SCE in each frame during Idle mode.
 *
 *  There are three states:
 *  CBCH_INIT - Each time the MS enters Idle Mode the CBCH state should be
 *  set to this state.  It will register the CBCH receive and abort callbacks
 *  with the NPL1 layer.
 *
 *  CBCH_RECEIVING - We get into this state during CBCH reception.
 *  When CBCH reception finishes we calculate the frame number when
 *  the next CBCH reception will occur  and then enter the CBCH_WAITING
 *  state.
 *
 *  CBCH_WAITING - In this state we are waiting for the frame number
 *  that the next reception will start on.  When this frame number occurs, we
 *  enter the CBCH_RECEIVING state.
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr - pointer to the idle data store containing CBCH info etc
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/

void l1_tick_cbch( gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Locals */
   idle_data_T   *idle_data_ptr;
   uint32             new_next_req_cbch_frame, this_FN;
   gl1_msg_rx_cb_type rx_cb;
   uint32        gap_to_next_cbch_activity = 0;
   l1_fm_priority_T   pri_cbch                  = l1_fm_get_active_CBCH_pri(gas_id);
   int8 cbch_offset;
   idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
   this_FN       = gl1_get_FN( gas_id );

   /* To Resolve KW issue*/
   if((pri_cbch != FM_PRI_CBCH)&& (pri_cbch != FM_PRI_MSIM_CBCH ) )
   {
      return;
   }

   if ( !idle_data_ptr )
   {
      return;
   }

#ifdef GL1_EXTENDED_CBCH_DBG
   MSG_GERAN_HIGH_3_G( L1_CBCH_HDR"tick FN:%d for state:%d next_req_cbch_frame:%d",
             this_FN, l1_local_idle_data_ptr->cbch_state,
             l1_local_idle_data_ptr->next_req_cbch_frame );
#endif /* GL1_EXTENDED_CBCH_DBG */

   switch ( l1_local_idle_data_ptr->cbch_state )
   {
      case CBCH_INIT:
         rx_cb.data_cb    = cbch_rx_callback;
         rx_cb.metrics_cb = cbch_metrics_callback;
         rx_cb.log_cb     = NULL;
         rx_cb.abort_cb   = cbch_abort_callback;
         rx_cb.tch_metrics_cb = NULL;
         l1_reset_non_drx_tc0(gas_id);
         gl1_msg_cfg_cbch((uint8)(idle_data_ptr->campedon_cell_BSIC & 0x07),
                          TRUE, &rx_cb, gas_id );
         l1_local_idle_data_ptr->cbch_rx_in_progress    = 0;
         l1_local_idle_data_ptr->cbch_abort_in_progress = FALSE;

         if( FALSE == idle_data_ptr->campedon_cell_agc.valid )
         {
           MSG_GERAN_ERROR_0_G("re-initializing campedon cell agc" );

           sys_algo_agc_init(&idle_data_ptr->campedon_cell_agc, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );
         }

         l1_local_idle_data_ptr->cbch_agc               = idle_data_ptr->campedon_cell_agc;
         sys_algo_agc_disable_slew_rate_limiting( &l1_local_idle_data_ptr->cbch_agc );

         MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"init ...");

         l1_local_idle_data_ptr->cbch_state          = CBCH_WAITING;

         /* Always set to an impossible FN at init phase */
         l1_local_idle_data_ptr->next_req_cbch_frame = MAX_CBCH_GAP_PERIOD;

         /* Always set to no valid drx ref */
         l1_local_idle_data_ptr->cbch_drx_ref        = 0;

         /* Reset CBCH priority */
         l1_fm_set_CBCH_pri(FALSE, gas_id);
         pri_cbch = l1_fm_get_pending_CBCH_pri(gas_id );

         /* fall through */

      case CBCH_WAITING:
         /* If this is our FN then run MFSM and transition to receiving state. */
         if ( ( l1_local_idle_data_ptr->next_req_cbch_frame != MAX_CBCH_GAP_PERIOD ) &&
              IS_FRAME_NUM_LATER( this_FN, l1_local_idle_data_ptr->next_req_cbch_frame ) )
         {

             /*if last expected CBCH frame is elapsed, then trigger a CB Missed indication*/

 cbch_offset=find_ota_cbch_offset(l1_local_idle_data_ptr->next_req_cbch_frame,gas_id);
              if(cbch_offset>0)
              {
                L1_send_CB_MISSING_BLOCK_IND( ADD_FN( l1_local_idle_data_ptr->next_req_cbch_frame, cbch_offset), gas_id );
                MSG_GERAN_HIGH_2_G("fake CB Missing indication at FN:%d and offset=%d", l1_local_idle_data_ptr->next_req_cbch_frame,cbch_offset);
               }
               else
               {
                 MSG_GERAN_ERROR_1_G("Invalid Fn and fake CB Missing indication at FN:%d", l1_local_idle_data_ptr->next_req_cbch_frame);
               }          
         }
         if ( ( l1_local_idle_data_ptr->next_req_cbch_frame != MAX_CBCH_GAP_PERIOD ) &&
              ( this_FN == l1_local_idle_data_ptr->next_req_cbch_frame ) )
         {
            l1_local_idle_data_ptr->cbch_drx_ref = 0;

            l1_run_cbch_state_machine( idle_data_ptr, gas_id );

            l1_local_idle_data_ptr->cbch_state = CBCH_RECEIVING;
         }
         else
         {
            gap_to_next_cbch_activity = l1_smscb_gap_to_next_activity(gas_id);

            if ( ( gap_to_next_cbch_activity > 0 ) &&
                 ( gap_to_next_cbch_activity != MAX_CBCH_GAP_PERIOD ) )
            {
                new_next_req_cbch_frame =
                ADD_FN( this_FN, ( gap_to_next_cbch_activity  + 1 ) );

                if ( new_next_req_cbch_frame != l1_local_idle_data_ptr->next_req_cbch_frame )
                {
                   uint8 next_cbch_tc = 0xFF;
                   /* remove old frame */
                   gl1_drx_release_fn_tick( l1_local_idle_data_ptr->next_req_cbch_frame, gas_id );

                   l1_local_idle_data_ptr->next_req_cbch_frame = new_next_req_cbch_frame;
				   next_cbch_tc = (new_next_req_cbch_frame / 51) % 8;

                   l1_local_idle_data_ptr->cbch_drx_ref =
                     gl1_drx_require_tick( l1_local_idle_data_ptr->next_req_cbch_frame, gas_id );
				   if (next_cbch_tc == 0)
				   {
				     pri_cbch = l1_fm_updated_CBCH_pri(gas_id);
                     /* To Resolve KW issue*/
                     if((pri_cbch != FM_PRI_CBCH)&& (pri_cbch != FM_PRI_MSIM_CBCH ) )
                     {
                       return;
                     }
					 /*Toggle the priority for next CBCH TC cycle and keep it in pending priority 
                       It will be used when setting priority for next occasion of TC==0 */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) 
                     l1_fm_toggle_CBCH_pri(gas_id);
#endif
				   }

                   l1_fm_set_required_frame_number( pri_cbch,
                                                    l1_local_idle_data_ptr->next_req_cbch_frame,
                                                    L1_DURATION_CBCH, gas_id );

                   /* Ensure new DRX entry is always processed */
                   gl1_drx_require_next_tick(gas_id);
                }
            }
            /*CBCH not supported*/
            else if(gap_to_next_cbch_activity == MAX_CBCH_GAP_PERIOD)
            {

              if(l1_local_idle_data_ptr->next_req_cbch_frame != MAX_CBCH_GAP_PERIOD)
              {
                l1_clear_cbch_activity(gas_id);
                gl1_drx_release_fn_tick( l1_local_idle_data_ptr->next_req_cbch_frame, gas_id );
                l1_local_idle_data_ptr->next_req_cbch_frame = MAX_CBCH_GAP_PERIOD;
              }
            }
         }
         break;

      case CBCH_RECEIVING:
         if( l1_local_idle_data_ptr->next_req_cbch_frame == gl1_get_FN(gas_id))
         {
      l1_local_idle_data_ptr->cbch_drx_ref = 0;
         }

         l1_run_cbch_state_machine( idle_data_ptr, gas_id );

         if ( l1_local_idle_data_ptr->cbch_rx_in_progress == 0 )
         {
            gap_to_next_cbch_activity = l1_smscb_gap_to_next_activity(gas_id);

            if ( ( gap_to_next_cbch_activity > 0 ) &&
                 ( gap_to_next_cbch_activity != MAX_CBCH_GAP_PERIOD ) )
            {
              uint8 next_cbch_tc;
              new_next_req_cbch_frame = ADD_FN( this_FN, ( gap_to_next_cbch_activity + 1 ) );

              /* remove old frame */
              gl1_drx_release_fn_tick( l1_local_idle_data_ptr->next_req_cbch_frame, gas_id );

              l1_local_idle_data_ptr->next_req_cbch_frame = new_next_req_cbch_frame;
			  next_cbch_tc        = (new_next_req_cbch_frame / 51)%8;

			  
			  if (next_cbch_tc == 0)
			  {
				pri_cbch = l1_fm_updated_CBCH_pri(gas_id);
                /* To Resolve KW issue*/
                if((pri_cbch != FM_PRI_CBCH)&& (pri_cbch != FM_PRI_MSIM_CBCH ) )
                {
                    return;
                }
				/*Toggle the priority for next CBCH TC cycle and keep it in pending priority 
                  It will be used when setting priority for next occasion of TC==0 */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) 
                l1_fm_toggle_CBCH_pri(gas_id);
#endif
			  }

              l1_fm_set_required_frame_number( pri_cbch,
                                               l1_local_idle_data_ptr->next_req_cbch_frame,
                                               L1_DURATION_CBCH, gas_id );

              /* pass to drx manager */
                   l1_local_idle_data_ptr->cbch_drx_ref =
                     gl1_drx_require_tick( l1_local_idle_data_ptr->next_req_cbch_frame, gas_id );

              l1_local_idle_data_ptr->cbch_state   = CBCH_WAITING;
            }
            else if(gap_to_next_cbch_activity == MAX_CBCH_GAP_PERIOD)
            {
              l1_clear_cbch_activity(gas_id);
              l1_local_idle_data_ptr->cbch_state   = CBCH_WAITING;
            }
         }
         break;

      default:
         MSG_GERAN_HIGH_1_G( "Bad CBCH substate %d",
                   l1_local_idle_data_ptr->cbch_state);
         break;
   }
}
/******************************************************************************
 *
 *  Function name:  find_ota_cbch_offset
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Calculates the OTA CBCH offset from the missed CBCH frame based on channel type and CCCH TN
 *
 *  Parameters:
 *  -----------
 *  Missed CBCH frame number and gs_id
 *
 *  Return Value:
 *  --------
 *  offset value  if missed frame number  is correct and -1 incorrect 
 *****************************************************************************/


int8 find_ota_cbch_offset(uint32 next_req_cbch_frame,gas_id_t gas_id)
{ 
  int8 offset =-1;
  uint32 passed_cbch_fn= next_req_cbch_frame% MULTIFRAME_51;
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
	
		
    switch(l1_smscb_get_chan_type(gas_id))
    {
      case SDCCH_8 :
      {
         if (( l1_local_idle_data_ptr->CBCH_TN > l1_local_idle_data_ptr->CCCH_TN  )&&(passed_cbch_fn==7 ))
          {
            offset=1;
          }
	  else if(passed_cbch_fn==6)
	  {
            offset=2;
          }
	  else
          {
            offset=-1;
          }
        }
        break;
        case SDCCH_4:
        {
          if(passed_cbch_fn==30)
          {
            offset=2;
	  }
          else
          {
	    offset=-1; //something ambiguos
          }
        }
        break;
      }
    return offset;		   	

}

/******************************************************************************
 *
 *  Function name:  l1_post_tick_cbch
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Called after the NPL1 layer has run in the frame. Rechecking the gap to next
 *  CBCH activity before going to sleep.
 *
 *  Parameters:
 *  -----------
 *  NONE.
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/

void l1_post_tick_cbch( gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

   if ( l1_local_idle_data_ptr->cbch_state == CBCH_RECEIVING )
   {
      if ( l1_local_idle_data_ptr->cbch_rx_in_progress == 0 )
      {
          gl1_drx_require_next_tick(gas_id);

          l1_local_idle_data_ptr->cbch_state  = CBCH_WAITING;

		  l1_clear_inactive_cbch_entries( gas_id );

		  if (l1_local_idle_data_ptr->next_req_cbch_frame != MAX_CBCH_GAP_PERIOD)
		  {
		     gl1_drx_release_fn_tick( l1_local_idle_data_ptr->next_req_cbch_frame, gas_id );
		     l1_local_idle_data_ptr->next_req_cbch_frame = MAX_CBCH_GAP_PERIOD;
		  }
      }
      else
      {
        gl1_drx_require_next_tick(gas_id);
      }
   }

}

/*****************************************************************************
 *
 *  Function name:  L1_control_paging_mode
 *  ------------------------------------
 *
 *  Description:
 *  ------------
 *  This function reads the paging mode and controls extended paging
 *  see GSM 04.08 section 3.3.2.1
 *
 *  Parameters:
 *  -----------
 *  own_paging_block
 *  message       - pointer to CCCH data
 *  idle_data_ptr - pointer to the idle data store containing paging info etc
 *
 *  Returns:
 *  --------
 *  skip_this_message
 *
 ****************************************************************************/

boolean L1_control_paging_mode(boolean      own_paging_block,
                               uint8        *message,
                                idle_data_T  *idle_data_ptr,
                                gas_id_t      gas_id )
{
   /* locals */
   uint8    page_mode;
   uint8    prev_page_mode;
   boolean  skip_this_message = FALSE;

   prev_page_mode = idle_data_ptr->paging_data.paging_mode;

   /* See if we are already in extended paging */
   if ( idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING)
   {
      MSG_GERAN_HIGH_2_G("extended paging: MF %d block %d",
              idle_data_ptr->paging_data.ext_paging_multiframe,
              idle_data_ptr->paging_data.ext_paging_block_index);
         idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;
         idle_data_ptr->paging_data.all_paging_blocks = FALSE;
   }
   else
   {
      /* Read the paging mode in the PCH message.
         This can be a Paging Request types 1, 2, or 3
         (9.1.22) or an Immediate assignment  (9.1.18) */

      if ( own_paging_block )
      {
       uint8 protocol_desc = *(message + 1);

       if (protocol_desc == RR_MANAGEMENT_MSG)
       {

         uint8 message_type = *(message + 2);

         switch (message_type)
         {
            case PAGING_REQUEST_TYPE_1:
            case PAGING_REQUEST_TYPE_2:
            case PAGING_REQUEST_TYPE_3:
            case IMMEDIATE_ASSIGNMENT:
            case IMMEDIATE_ASSIGNMENT_EXTENDED:
            case IMMEDIATE_ASSIGNMENT_REJECT:
         page_mode = *(message + 3) & 3;

         if ( page_mode != SAME_AS_BEFORE )
         {
            idle_data_ptr->paging_data.prev_paging_mode = page_mode;
            MSG_GERAN_HIGH_1_G("Storing prev paging mode = %d",idle_data_ptr->paging_data.prev_paging_mode);
         }
              break;
            default:
              page_mode = idle_data_ptr->paging_data.paging_mode;
              MSG_GERAN_HIGH_1_G("sysinfo message in PAGING_REORG type 0x%x", message_type);
              break;
         }
       } else
       {
         page_mode = idle_data_ptr->paging_data.paging_mode;
         MSG_GERAN_HIGH_1_G("Not RR_MANAGEMENT_MSG Type 0x%x", protocol_desc);
       }
      }
      else
      {
         page_mode = idle_data_ptr->paging_data.paging_mode;
      }

      switch (page_mode)
      {
         case NORMAL_PAGING:

            if (own_paging_block)
            {
               /*
                  Normal paging is now handled by RR.  L1 does not override
                  RR.   This is to ensure that we always remain in whatever
                  mode RR sets us to initially during early camping irrespective
                  of what the network sets us to subsequently.  This is to get
                  20.19 to pass
               */
               //idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;
               //idle_data_ptr->paging_data.all_paging_blocks = FALSE;
               skip_this_message = FALSE;
            }
            else
            {
               skip_this_message = TRUE;
            }

            break;

         case EXTENDED_PAGING:

            /*
                     If SIM becomes invalid, use paging parameters from before.
                     Don't stop decoding paging blocks.  This will ensure that
                     we continue shipping up measurements to RR so that we can
                     reselect if need be.
                  */
            if ( own_paging_block && idle_data_ptr->imsi_data.imsi_valid == FALSE )
            {
               idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;
            }
            else
            {
               if (own_paging_block)
               {
                  idle_data_ptr->paging_data.paging_mode = EXTENDED_PAGING;
                  idle_data_ptr->first_ext_pch_block_read = TRUE;
				  /* reset this flag to avoid MS missing the extended paging*/
				  idle_data_ptr->paging_data.all_paging_blocks = FALSE; 
               }
            }
            skip_this_message = FALSE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_EXTENDED_PAGING);
#endif
            break;

         case PAGING_REORGANIZATION:

            if (own_paging_block)
            {
               idle_data_ptr->paging_data.paging_mode = PAGING_REORGANIZATION;
               idle_data_ptr->paging_data.all_paging_blocks = TRUE;
               skip_this_message = FALSE;
            }

            /* Discard fill paging messages in paging reorganization */
            else
            {
               if ( (*(message + 1)== 0x06) && /* RR protocol disciminator */
                    ((*(message + 2) & 0xf8) == 0x20) && /* Paging message type */
                    ((*(message + 5) & 0x07) == 0) ) /* No mobile identity */
               {
                  MSG_GERAN_LOW_0_G("Fill page");
                  skip_this_message = TRUE;
               }
               else
               {
                  skip_this_message = FALSE;
               }
            }
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_PAGING_REORGANIZATION);
#endif
            break;

         case SAME_AS_BEFORE:
            if ( own_paging_block && idle_data_ptr->paging_data.prev_paging_mode == EXTENDED_PAGING )
            {
               MSG_GERAN_HIGH_0_G("Same as before:Prev mode extended");
               idle_data_ptr->paging_data.paging_mode = EXTENDED_PAGING;
            }
            skip_this_message = FALSE;
            break;

         default:
            MSG_GERAN_ERROR_1_G("Invalid paging mode %d", page_mode);
            break;
      }
   }

   /* Do debug message if page mode changed, except for extended paging */
   if ( prev_page_mode != idle_data_ptr->paging_data.paging_mode  )
   {
      MSG_GERAN_MED_3_G("Page mode: %d->%d. APB=%d.",
              prev_page_mode,
              idle_data_ptr->paging_data.paging_mode,
              idle_data_ptr->paging_data.all_paging_blocks);
   }

   return(skip_this_message);
}

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
/*===========================================================================

FUNCTION gpl1_read_page_mode

DESCRIPTION
  This function decode ccch data.

DEPENDENCIES
  none.

RETURN VALUE
  page mode

SIDE EFFECTS
  None
===========================================================================*/
byte gpl1_read_page_mode(byte    *message,gas_id_t gas_id)
{
  uint8 page_mode = NORMAL_PAGING;
  uint8 protocol_desc = *(message + 1);

  if (protocol_desc == RR_MANAGEMENT_MSG)
  {
    uint8 message_type = *(message + 2);

    switch (message_type)
    {
       case PAGING_REQUEST_TYPE_1:
       case PAGING_REQUEST_TYPE_2:
       case PAGING_REQUEST_TYPE_3:
       case IMMEDIATE_ASSIGNMENT:
       case IMMEDIATE_ASSIGNMENT_EXTENDED:
       case IMMEDIATE_ASSIGNMENT_REJECT:
         page_mode = *(message + 3) & 3;

         if ( page_mode != SAME_AS_BEFORE )
         {
           if ( page_mode == PAGING_REORGANIZATION )
           {
              MSG_GERAN_HIGH_0_G("PCH is PAGING_REORGANIZATION :Ignore");
              page_mode = NORMAL_PAGING;
           }
           else if ( page_mode == EXTENDED_PAGING )
           {
              MSG_GERAN_HIGH_0_G("PCH is EXTENDED_PAGING");
           }
           else
           {
             page_mode = NORMAL_PAGING;
           }
         }
         else
         {
           page_mode = NORMAL_PAGING;
         }

         break;
       default:
         page_mode = NORMAL_PAGING;
         MSG_GERAN_HIGH_2_G("sysinfo message in PAGING_REORG type 0x%x", message_type,page_mode);
         break;
    }
  }
  else
  {
    page_mode = NORMAL_PAGING;
    MSG_GERAN_HIGH_1_G("Not RR_MANAGEMENT_MSG Type 0x%x", protocol_desc);
  }

  return page_mode;
}  /* gpl1_read_page_mode */

#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

/*===========================================================================

FUNCTION ccch_rx_callback

DESCRIPTION
  This function is called when the data from the CCCH has been decoded. It
  sends a message to L2 indicating if message is good or not.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void ccch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
   idle_data_T  *local_idle_data_ptr = NULL;
   uint8 gap_to_next_activity = 0xFF;
   boolean local_own_paging_block = FALSE;

#ifdef FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17
   l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr = &l1_serving_cell_meas[gas_id];
#endif

   if(gl1_hw_sleep_next_allowed_ccch_fn[gas_id] != NOT_A_FRAME_NUMBER)
   {
      if( IS_FRAME_NUM_LATER(GSTMR_GET_FN_GERAN(gas_id), gl1_hw_sleep_next_allowed_ccch_fn[gas_id]) )
      {
         MSG_GERAN_LOW_2_G("WMGR_SLEEP: RESET next_allowed_ccch_fn: %u, FN: %u", gl1_hw_sleep_next_allowed_ccch_fn[gas_id], GSTMR_GET_FN_GERAN(gas_id));
         gl1_hw_sleep_next_allowed_ccch_fn[gas_id] = NOT_A_FRAME_NUMBER;
      }
   }

   switch (rpt->ccch.tag)
   {
      case 0:
         local_idle_data_ptr = l1_local_idle_data_ptr->idle_data_ptr_1;
         break;
      case 1:
         local_idle_data_ptr = l1_local_idle_data_ptr->idle_data_ptr_2;
         break;
      default:
         MSG_GERAN_ERROR_1_G("PCH(%d) wrong tag ... ",rpt->ccch.tag);
         return;
   }

   /* Update the RXqual full on this callback */
   gl1_msg_update_hard_rxqual_ave( (uint16)rpt->ccch.hdr.rx_qual, gas_id );

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_rx_callback: making rpt->ccch.hdr.good_data TRUE");
   MSG_GERAN_HIGH_1_G("GPLT DBG: ccch_rx_callback: rpt->ccch.own_paging_block = %d", rpt->ccch.own_paging_block);
   rpt->ccch.hdr.good_data = TRUE;
   MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_rx_callback: making rpt->ccch.own_paging_block TRUE");
   rpt->ccch.own_paging_block = TRUE;
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
   if(rpt->ccch.own_paging_block)
   {
      if((uint16)rpt->ccch.hdr.rx_qual >= SCE_REDUCED_NCELL_LIST_SCELL_MIN_RXQUAL)
      {
         /* scell rxqual is good, so use optimised top 2 mode */
         l1_sci_set_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_SCELL_RXQUAL_EN, gas_id);
      }
      else
      {
         /* scell rxqual is not good, so use normal top 6 mode */
         l1_sci_clear_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_SCELL_RXQUAL_EN, gas_id);
      }
   } /* if(rpt->ccch.own_paging_block) */
#endif

   if((rpt->ccch.own_paging_block)|| (local_idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING))
   {
      local_own_paging_block = TRUE;
   }

   if (rpt->ccch.hdr.good_data)
   {
      switch (rpt->ccch.hdr.chan)
      {
         case CCCH:
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
            // In case of GPLT and no RF, return default/canned result
            MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_rx_callback: making paging data");
            memscpy(rpt->ccch.data,sizeof(rpt->ccch.data),gplt_paging_result.data, sizeof(gplt_paging_result.data));
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

            if ( (L1_control_paging_mode(rpt->ccch.own_paging_block,
                                         rpt->ccch.data, local_idle_data_ptr, gas_id) == FALSE) ||
                 (l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag)  ||
                 (l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag) || (l1_idle_data->paging_data.paging_mode == PAGING_REORGANIZATION) )
            {
#ifdef FEATURE_GL1_GPLT
         if (gplt_canned_nw_data) {
          MSG_GERAN_HIGH_0_G("GPLT DBG: Canned Paging Message");
          memscpy(rpt->ccch.data,sizeof(rpt->ccch.data),gplt_paging_result.data, sizeof(gplt_paging_result.data));
         }
#endif

               L1_send_PH_DATA_IND(TRUE,
                                   rpt->ccch.hdr.chan,
                                   local_own_paging_block,
                                   rpt->ccch.arfcn,
                                   rpt->ccch.data,
                                   (uint16)MAX_CCCH_OCTETS, gas_id);

              MSG_GERAN_HIGH_3_G(
                     "PCH decoded (Arfcn=%d PgBlk=%d fn=%d)",
                     rpt->ccch.arfcn.num,
                     rpt->ccch.own_paging_block, gl1_get_FN( gas_id ));
#ifdef FEATURE_GPRS_GBTA_DEBUG
              if (gl1_drdsds_enabled(gas_id) && IS_IDLE_EV_PWR_MON_SET(gas_id))
              {
#ifdef FEATURE_QSH_MDUMP
                QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                ERR_GERAN_FATAL_0_G("GBTA DR-DSDS ENABLED , IDLE SUB has set GBTA_EV_IDLE_PWR_MON incorrectly in IDLE");
              }
#endif /* FEATURE_GPRS_GBTA_DEBUG*/
            }
            else
            {
               MSG_GERAN_HIGH_3_G("PCH(%d) skipped (arfcn=%d fn=%d)",
                        rpt->ccch.tag, rpt->ccch.arfcn.num, frame_counters[gas_id].FNmod102);
            }

            if( FALSE != gprs_serv_cell_meas_ptr->cal_c_value_51_idle )
            {
                /* If c value calculations are required on
                 * the ccch logical channel then call
                 * the serv cell data function.
                 */
                gpl1_scell_cs_idle_data_cb( TRUE , gas_id);
            }

#ifdef GL1_HW_DEBUG_FEU
            #error code not present
#endif /* GL1_HW_DEBUG_FEU */

            gl1_hw_set_cgps_feu_force_rpush( gas_id );
            break;

         case BCCH:
           {
             uint8 si_tc;

             si_tc = l1_compute_tc( gl1_get_FN( gas_id ),
                                    rpt->ccch.extbcch );
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
            // In case of GPLT and no RF, return default/canned result
            MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_rx_callback: making si data");
            switch (si_tc)
            {
            case 2:
            case 6:
             {
                 MSG_GERAN_HIGH_0_G("GPLT DBG: Making SI3");
                 memscpy(rpt->ccch.data,sizeof(rpt->ccch.data), &gplt_si3_result, sizeof(gplt_si3_result));
                 break;
             }
            case 3:
            case 7:
             {
               //si4
               MSG_GERAN_HIGH_0_G("GPLT DBG: Making SI4");
               memscpy(rpt->ccch.data,sizeof(rpt->ccch.data),&gplt_si4_result, sizeof(gplt_si4_result));
               break;
             }
            case 4:
            case 5:
             {
               MSG_GERAN_HIGH_0_G("GPLT DBG: Making SI2Quater");
               memscpy(rpt->ccch.data,sizeof(rpt->ccch.data),&gplt_si2quater_result, sizeof(gplt_si2quater_result));
               break;
             }
            default:
             {
               break;
             }
            }
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
             L1_send_sysinfo_PH_DATA_IND(TRUE,
                                         rpt->ccch.hdr.chan,
                                         local_own_paging_block,
                                         rpt->ccch.arfcn,
                                         si_tc,
                                         rpt->ccch.data,
                                         MAX_BCCH_OCTETS, gas_id);

             MSG_GERAN_MED_3_G("BCCH(tag=%d) decoded (arfcn=%d TC=0x%02X)", rpt->ccch.extbcch, rpt->ccch.arfcn.num, si_tc);
           }
           break;

         default:
            MSG_GERAN_ERROR_3_G("Unknown(%d) decoded (arfcn=%d fn=%d)",
                       rpt->ccch.tag, rpt->ccch.arfcn.num, frame_counters[gas_id].FNmod102);
            break;
      }

      if (rpt->ccch.own_paging_block)
      {
         L1_send_MPH_BLOCK_QUALITY_IND((byte)rpt->ccch.hdr.chan, GL1_BLOCK_QUALITY_GOOD, gas_id);
         l1_local_idle_data_ptr->l1_bad_pch_block_cnt = 0;
#ifdef FEATURE_GSM_QSC_TX_DIV
           pch_crc_failure [gas_id]= FALSE ;
#endif /* FEATURE_GSM_QSC_TX_DIV   */
      }
   }
   else
   {
      if ((rpt->ccch.hdr.chan == CCCH) && (local_idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING))
      {
            local_idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;
            local_idle_data_ptr->paging_data.all_paging_blocks = FALSE;
      }

      /* Bad Block received */
      L1_send_PH_DATA_IND(FALSE,
                          rpt->ccch.hdr.chan,
                          local_own_paging_block,
                          NULL_ARFCN,
                          NULL,
                          0, gas_id);

      if (rpt->ccch.own_paging_block)
      {
#ifdef FEATURE_GPRS_GBTA
        if( ignore_xcch && IS_GBTA_ACTIVE())
        {
         L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
        }
        else
#endif /* FEATURE_GPRS_GBTA */
        {
         L1_send_MPH_BLOCK_QUALITY_IND((byte)rpt->ccch.hdr.chan, GL1_BLOCK_QUALITY_BAD, gas_id);
        }

         if (++l1_local_idle_data_ptr->l1_bad_pch_block_cnt > L1_MAX_FAILED_PCH_DECODES)
         {
          if ( ftm_get_mode() == FTM_MODE )
          {
           MSG_GERAN_HIGH_0_G("Stuck on bad cell in FTM mode");
          }
          else
          {
              MSG_GERAN_ERROR_0_G("Stuck on bad cell");
              l1_call_panic_reset_in_task(L1_STUCK_ON_BAD_CCH, gas_id);
            }
         }

#ifdef FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17
         /*if Serving Cell power is too bad and Page decodes are failing, then de-blacklist neighbours to allow a faster reselection 
                   than an OOS-->Cell Selection scenario*/
         if (l1_local_idle_data_ptr->l1_bad_pch_block_cnt > L1_MAX_FAILED_PCH_DECODES_SC_DEBLACKLIST 
             && l1_serving_cell_meas_ptr->rx_power < L1_MIN_GOOD_SCELL_RX_LVL)
         {
            l1_sc_deblacklist_ba_list(gas_id);
         }
#endif

#ifdef FEATURE_GSM_QSC_TX_DIV
         pch_crc_failure [gas_id]= TRUE ;
        MSG_HIGH("PCH crc failed from qsc ",0,0,0);
#endif /* FEATURE_GSM_QSC_TX_DIV   */

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
      if (gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE1)
#ifdef FEATURE_GPRS_GBTA	  
	   && (!ignore_xcch)
#endif
        )
      {
         gl1_hw_switch_antenna_tx_div_ccch_fail(TRUE, gas_id);
        }
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING  */
    }

#ifdef FEATURE_GPRS_GBTA
     if( ignore_xcch && IS_GBTA_ACTIVE())
     {
       MSG_GERAN_ERROR_3_G("CCCH(%d) not scheduled due to Tx Sub conflict(arfcn=%d fn=%d)", rpt->ccch.tag,
        rpt->ccch.arfcn.num, frame_counters[gas_id].FNmod102);

     }else
#endif /* FEATURE_GPRS_GBTA */
     {
       MSG_GERAN_ERROR_3_G("xCCH(%d) failed (arfcn=%d fn=%d)", rpt->ccch.hdr.chan,
         rpt->ccch.arfcn.num, frame_counters[gas_id].FNmod102);
     }

      if(GL1_HW_EFS_DEBUG_ENABLED( GL1_EFS_DEBUG_XCCH_IN_DRDSDS ) && (gl1_drdsds_enabled(gas_id)))
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_3_G("xCCH(%d) failed (arfcn=%d fn=%d) on GAS ID 0x01",
        rpt->ccch.tag, rpt->ccch.arfcn.num, frame_counters[gas_id].FNmod102);
      }

#ifdef FEATURE_GPRS_GBTA_DEBUG
      if(GL1_HW_EFS_DEBUG_ENABLED( GL1_EFS_DEBUG_XCCH ) && (ignore_xcch==FALSE) && (IS_GBTA_ACTIVE()))
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_3_G("xCCH(%d) failed (arfcn=%d fn=%d) on GAS ID 0x01",
                 rpt->ccch.tag, rpt->ccch.arfcn.num, frame_counters[gas_id].FNmod102);
      }
#endif /* FEATURE_GPRS_GBTA_DEBUG */

      if( FALSE != gprs_serv_cell_meas_ptr->cal_c_value_51_idle )
      {
         /* If c value calculations are required on
          * the ccch logical channel then call
          * the serv cell data function.
          */
         gpl1_scell_cs_idle_data_cb( FALSE, gas_id );
      }

   }

   /* As we have now decoded, tick next_cell_bcch */
   l1_sc_report_period_done(gas_id);


   gap_to_next_activity = calculate_gap_to_next_activity( l1_tskisr_blk->current_params.L1Data.pIdle_data, 0, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* calculate gap to next activity and if not immediate clear out the frames */
   if ( gl1_msg_get_multi_sim_mode() )
   {
     if ((gap_to_next_activity > 0)
         &&(l1_local_idle_data_ptr->idle_rx_in_progress < MAX_CCCH_RX_INSTANCES)
        )
     {
       /* clean out the booking and release the TRM lock if there is a gap before the activity */
       l1_fm_clear_required_frames_after_pch_decode( FM_PRI_CCCH, gas_id );
     }
   }
   else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   if ( ( gap_to_next_activity > 0 )
        &&( l1_local_idle_data_ptr->idle_rx_in_progress < MAX_CCCH_RX_INSTANCES ))
   {
      MSG_GERAN_HIGH_0_G("Clearing CCCH frame as gap_to_activity > 0");
      l1_fm_clear_required_frames(FM_PRI_CCCH, gas_id);
   }


#ifdef FEATURE_GPRS_GBTA
   ignore_xcch = FALSE;
   /* After PCH decode , rset back to GBTA_ALLOWED */
   if (IS_GBTA_ACTIVE())
   {
     /*Dont Release TRM if we are in page reorg mode and we already have the GBTA grant
      else we may not get TRM for all CCCH blocks in page reorg mode if we release it now*/
     if((local_idle_data_ptr->paging_data.paging_mode == PAGING_REORGANIZATION ||
         local_idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING ||
         (gap_to_next_activity  <= FRAMES_PER_BLOCK) ||
         (l1_local_idle_data_ptr->idle_rx_in_progress > 1)) &&
        (GET_GBTA_STATE() != GRM_GBTA_NONE) &&
        (GET_GBTA_STATE() != GRM_GBTA_BLOCKED) &&
        (!OTHER_SUB_RELEASE_PENDING()))        
     {
       MSG_GERAN_MED_1_G("Dont release TRM GBTA state %d", GET_GBTA_STATE());
       if (!OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) && (l1_local_idle_data_ptr->idle_rx_in_progress < MAX_CCCH_RX_INSTANCES ))
       {
         MSG_GERAN_HIGH_0_G("OTHER_GSM_CLIENT exited Pkt Transfer in GBTA Gap");
         /* as we never got the lock but if it gets extended then usefull to know if that was gbta extension */
         grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);
         SET_GBTA_STATUS(GRM_GBTA_NONE);
         l1_reset_gbta_active_count(gas_id);
       }
     }
     else
     {
        gpl1_gbta_reset_active_state(gas_id);

        /* Enable DR is DR state in pending on a successful PAGE DECODE */
        if ( (rpt->ccch.hdr.good_data) && (pending_dr_state == PENDING_DR_ENABLE) && IS_GBTA_ALLOWED())
        {
          SET_GBTA_STATUS(GRM_GBTA_NONE);

          /* This allows not to schedule Idle Power monitor when programmed */
          grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);

          /* surround_tick is in units of 51 multi frames.
           * In Idle mode, each paging block is every bs_pa_mfrms (n=2 to 9).
           * surround_tick should be incremented irrespective of TRM getting denied / PCH failure.
           */
          gl1_allow_sci_inc_surround_tick_gbta(gas_id);

          grm_set_dr_enabled_status(l1_tskisr_blk->client_id,gas_id);
          
          MSG_GERAN_HIGH_0_G("GL1_DR_DSDS switch GBTA to DR_DSDS");
        }

       /* as we are not making TRM request , we need to wipe the booking with TRM */
       grm_release( l1_tskisr_blk->client_id, gas_id );

       SET_MISSED_PAGE_STATUS(FALSE,gas_id);
     }
   }
#endif /*   FEATURE_GPRS_GBTA */

   /* If the non drx timer is active then check
    * whether it has expired.
    */
   if (l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag)
   {
      uint32 local_FN = gl1_get_FN( gas_id );
      if ((IS_FRAME_NUM_LATER( local_FN, l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN )) || l1_idle_data->cancel_non_drx)
      {
         MSG_GERAN_HIGH_3_G("ndrx tmr expd fn %d mm ndrx flg %d mode %d ",
                  local_FN,
                  l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag,
                  l1_idle_data->mm_non_drx_mode);

         l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag = FALSE;
         l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nc_non_drx_valid = FALSE;
      }
   }

   /* Check if the mm non-drx mode has been disabled */
   if (!l1_idle_data->mm_non_drx_mode)
   {
      l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag = FALSE;
   }

   /* LOG MESSAGE METRICS */
   l1_log_message_metrics ( GL1_MSG_CCCH, rpt, gas_id );
   l1_log_message_metrics_compact(GL1_MSG_CCCH, rpt, gas_id);
     l1_local_idle_data_ptr->idle_rx_in_progress--;
   }

/******************************************************************************
 *
 *  Function name:  write_arfcns
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Updates ARFCN array with values.
 *
 *  Parameters:
 *  -----------
 *  rx_arfcns - ARFCN array.
 *  arfcn     - ARFCN value
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/
void write_arfcns(
                   ARFCN_T rx_arfcns[],
                   ARFCN_T arfcn )
{
   uint8 i;

   for (i=0; i<4; i++)
   {
      rx_arfcns[i] = arfcn;
   }
}

/*===========================================================================

FUNCTION cbch_rx_callback

DESCRIPTION
      This function is called when the data from the CCCH has been decoded. It
      sends a message to L2 indicating if message is good or not.

DEPENDENCIES
      none.

RETURN VALUE
      None

SIDE EFFECTS
      None
===========================================================================*/
static void cbch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

   if (rpt->ccch.hdr.good_data)
   {
      MSG_GERAN_MED_2_G("Good CBCH decoded (fn102 = %d fn = %d)",
               frame_counters[gas_id].FNmod102, gl1_get_FN( gas_id ));

      L1_send_PH_DATA_IND(TRUE,
                          rpt->ccch.hdr.chan,
                          rpt->ccch.own_paging_block,
                          rpt->ccch.arfcn,
                          rpt->ccch.data,
                          (uint16)MAX_CCCH_OCTETS, gas_id);

      l1_smscb_set_first_non_drx_block_read(gas_id);
   }
   else
   {
      MSG_GERAN_MED_2_G("Bad CBCH decoded (fn102 = %d fn = %d)",
               frame_counters[gas_id].FNmod102, gl1_get_FN( gas_id ));

      /* Bad Block received */
      L1_send_PH_DATA_IND(FALSE,
                          rpt->ccch.hdr.chan,
                          rpt->ccch.own_paging_block,
                          NULL_ARFCN,
                          NULL,
                          0, gas_id);

   }

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
   /* If an expiry ind needs to be sent after last block decode */
   l1_smscb_send_deferred_expiry_ind(gas_id);
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

   /* Wait an extra frame to hopefully get the CB skip req if required */
   gl1_drx_require_next_tick(gas_id);

   l1_clear_cbch_activity(gas_id);

   /* If the non drx timer is active then check
    * whether it has expired.
    */
   if (l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag)
   {
      uint32 local_FN = gl1_get_FN( gas_id );
      if (IS_FRAME_NUM_LATER( local_FN, l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN ) || l1_idle_data->cancel_non_drx)
      {
         l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag = FALSE;
      }
   }

   /* Check if the mm non-drx mode has been disabled */
   if (!l1_idle_data->mm_non_drx_mode)
   {
      l1_local_idle_data_ptr->l1_idle_mm_non_drx_mode_flag = FALSE;
   }

   /* LOG MESSAGE METRICS */
   l1_log_message_metrics ( GL1_MSG_CBCH, rpt, gas_id );
   l1_log_message_metrics_compact(GL1_MSG_CBCH, rpt, gas_id);

   l1_local_idle_data_ptr->cbch_rx_in_progress--;

   /*reset the decoding_cbch flags as we have sent the CBCH data indication above*/
      l1_smscb_set_decoding_basic_cbch (FALSE, gas_id );
      l1_smscb_set_decoding_extended_cbch(FALSE, gas_id );
   }

/******************************************************************************
 *
 *  Function name:  clear_idle_power_avg_buffer
 *  ------------------------------------
 *
 *  Description:
 *  ------------
 *  clear the running average window 
 *
 *
 *****************************************************************************/   
#ifdef FEATURE_GSM_QSC_TX_DIV
static void clear_idle_power_avg_buffer (gas_id_t gas_id )
{

power_idle_average1[gas_id] = 0;
power_idle_average2[gas_id] = 0;
power_idle_average3[gas_id] = 0;
power_idle_average4[gas_id] = 0;
avg_cntr[gas_id] = 0;
power_idle_average[gas_id] = 0 ;
}
#endif /* FEATURE_GSM_QSC_TX_DIV   */

/******************************************************************************
 *
 *  Function name:  ccch_metrics_callback
 *  ------------------------------------
 *
 *  Description:
 *  ------------
 *  Callback function called to process metrics for each burst in a
 *  BCCH/PCH block.
 *
 *  This function also checks whether it is time to send the next measurement
 *  report to RR, and if so, sends the report.
 *
 *  Parameters:
 *  -----------
 *  Metrics report pointer.
 *
 *
 *  Returns:
 *  --------
 *  nothing
 *
 *****************************************************************************/
static void ccch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr = &l1_serving_cell_meas[gas_id];

  uint8               num_bursts;
  uint8               valid_num_bursts;
  idle_data_T        *idle_data_ptr;
  dBx16_T             power;
  dBx16_T             power_array[GL1_DEFS_FRAMES_IN_BLK_PERIOD] = {GL1_INVALID_POWER_LEVEL};
  dBx16_T             power_instant = GL1_INVALID_POWER_LEVEL;
  running_avg_struct *ra_ptr;
  int16               te_array[GL1_DEFS_FRAMES_IN_BLK_PERIOD] = {0};
  uint16              snr_array[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
  dBx16_T             snr_array_dBx16[GL1_DEFS_FRAMES_IN_BLK_PERIOD] = {GL1_INVALID_POWER_LEVEL};
  uint16              premimo_snr_array[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
  dBx16_T             premimo_snr_array_dBx16[GL1_DEFS_FRAMES_IN_BLK_PERIOD] = {GL1_INVALID_POWER_LEVEL};
  boolean             big_te = FALSE;
  dBx16_T             avg_SNR_dBx16;
  dBx16_T             premimo_avg_SNR_dBx16;
  dBx16_T             snr_array_msglog[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
  boolean             is_saic_enabled = FALSE;
  uint8               avg_meas_count = 0, i = 0;
  gl1_power_meas_type pwr_meas_type = PRX_MEAS;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  dBx16_T             divrx_power_array[GL1_DEFS_FRAMES_IN_BLK_PERIOD] = {GL1_INVALID_POWER_LEVEL};
  dBx16_T             divrx_power_instant = GL1_INVALID_POWER_LEVEL;
  dBx16_T             divrx_power = GL1_INVALID_POWER_LEVEL;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/


  /* Be careful with SNR arrays in this function because there
   * are two. One contains the raw values from the metrics report,
   * the other contains values in dBx16. This function calls both
   * functions that require the raw array and functions that
   * require the dBx16 array.
   */

  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  ra_ptr        = &(idle_data_ptr->campedon_cell_pwr_meas_avg);

  /* Process metrics */
  /* Process timing offset and freq. offset.
   * Not all bursts are necessarily received so
   * count the actual number of bursts.
   */
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if(gl1_get_IMRD_ctl_nv(gas_id))
  {
    pwr_meas_type = PRX_DIVRX_MEAS;
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  valid_num_bursts = 0;
  for ( num_bursts = 0; num_bursts < GL1_DEFS_FRAMES_IN_BLK_PERIOD; num_bursts++ )
  {
    if (( rpt[num_bursts]->valid ) && (!rpt[num_bursts]->yield))
    {
      power_array[valid_num_bursts]     = rpt[num_bursts]->pwr_dBm_x16;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
      /* Use PRX power if div rx power is zero*/
      if(gl1_get_IMRD_ctl_nv(gas_id))
      {
         if(rpt[num_bursts]->pwr_dBm_x16_divrx != GL1_INVALID_POWER_LEVEL)
         {
           divrx_power_array[valid_num_bursts] = rpt[num_bursts]->pwr_dBm_x16_divrx;
         }
         else
         {
           /* DivRX pwr is invalid, use PRX power to keep running average counters/
           variables in sync. */
           divrx_power_array[valid_num_bursts] = rpt[num_bursts]->pwr_dBm_x16;
         }
      }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
      te_array[valid_num_bursts]        = rpt[num_bursts]->timing_offset;

      if (rpt[num_bursts]->own_paging_block &&
          gl1_msg_saic_in_burst(rpt[num_bursts]->misc_flags))
      {
        premimo_snr_array[valid_num_bursts] = rpt[num_bursts]->pre_mimo_trained_complete_snr;
        premimo_snr_array_dBx16[valid_num_bursts] = gl1_msg_SNR_to_dBx16(premimo_snr_array[valid_num_bursts]);
        is_saic_enabled = TRUE;
      }

      snr_array[valid_num_bursts] = rpt[num_bursts]->snr;
      snr_array_dBx16[valid_num_bursts] = gl1_msg_SNR_to_dBx16( snr_array[valid_num_bursts] );
      snr_array_msglog[num_bursts] = gl1_msg_SNR_to_dBx16( snr_array[valid_num_bursts] );

      /* Check for big timing errors.  We'll print a message later. */
      if ( (snr_array[valid_num_bursts] > DEBUG_BIG_TE_SNR_THRESHOLD) &&
           ((te_array[valid_num_bursts] > 8) || (te_array[valid_num_bursts] < -8)) )
      {
        big_te = TRUE;
      }

      /* If SAIC is used in a burst the mDSP sets bit 14 of */
      /* the misc_flag true. Additionally, for AFC tracking */
      /* the flag selects the specific GMSK SAIC threshold. */
      if ( gl1_msg_saic_in_burst( rpt[num_bursts]->misc_flags ) )
      {
        gl1_msg_cch_tt_update( te_array[valid_num_bursts],
                               gl1_msg_saic_tt_threshold_snr( rpt[num_bursts] ),
                               TRUE ,
                               gas_id );
        gl1_msg_cch_afc_update( rpt[num_bursts]->freq_offset,
                                gl1_msg_saic_afc_threshold_snr( rpt[num_bursts] ),
                                TRUE,
                                gas_id );
      }
      else
      {
        gl1_msg_cch_tt_update( te_array[valid_num_bursts],
                               snr_array[valid_num_bursts],
                               FALSE ,
                               gas_id);
        gl1_msg_cch_afc_update( rpt[num_bursts]->freq_offset,
                                snr_array[valid_num_bursts],
                                FALSE,
                                gas_id);
      }

      if( FALSE != gprs_serv_cell_meas_ptr->cal_c_value_51_idle )
      {
        /* If c value calculations are required on
         * the ccch logical channel then call
         * the serv cell metrics function inorder
         * to accumulate the rssi measurements.
         */
          gpl1_scell_cs_idle_metrics_cb(num_bursts,
                                        rpt[num_bursts]->pwr_dBm_x16,
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
                                        divrx_power_array[valid_num_bursts],
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
                                        pwr_meas_type,
                                        gas_id );
      }
      valid_num_bursts++;
    }
    else
    {
      snr_array_msglog[num_bursts] = 0;
    }



      /*Below condition checks if 3 or more burst got yielded but our arbitrator didnt detect it, crash*/
        /*make sure we have a NV to disable the crash if it is too frequent*/
  }

  /* If PCH detect with single burst then use data again for AFC
      loop to ensure it is still fast enough */
  if ( gl1_msg_is_sbd_enabled( gas_id ) &&
       ( valid_num_bursts == 1 ) )
  {
    /* If SAIC is used in a burst the mDSP sets bit 14 of */
    /* the misc_flag true. Additionally, for AFC tracking */
    /* the flag selects the specific GMSK SAIC threshold. */
    if ( gl1_msg_saic_in_burst( rpt[valid_num_bursts-1]->misc_flags ) )
    {
      gl1_msg_cch_afc_update( rpt[valid_num_bursts-1]->freq_offset,
                              gl1_msg_saic_afc_threshold_snr( rpt[valid_num_bursts-1] ),
                              TRUE ,
                              gas_id);
    }
    else
    {
      gl1_msg_cch_afc_update( rpt[valid_num_bursts-1]->freq_offset,
                              snr_array[valid_num_bursts-1],
                              FALSE,
                              gas_id);
    }

    /* update the agc range to be valid . As the block decoded in SBD */
    idle_data_ptr->campedon_cell_agc.valid = TRUE;
  }
  
  if(valid_num_bursts > 0)
  {
  /* Calculate avg power and SNR over just this block
   * using switch statement to be more efficient.
   */
   
  power_instant = fast_average( power_array, valid_num_bursts );
   #ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if(TRUE == gl1_get_IMRD_ctl_nv(gas_id))
  {
    divrx_power_instant = fast_average(divrx_power_array, valid_num_bursts );
    l1_serving_cell_meas_ptr->divrx_power_instant = divrx_power_instant;
  }
   #endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
   
  if( TRUE == is_saic_enabled)
  {
    avg_SNR_dBx16 = fast_average( snr_array_dBx16, valid_num_bursts );
    premimo_avg_SNR_dBx16 = fast_average( premimo_snr_array_dBx16, valid_num_bursts );
    /* pre-MIMO SNR being < 2 dB (or 32 in dBx16)AND post-MIMO SNR is also < 4 dB (or 64 in dBx16) then the SNR is bad flag should be set.*/
    l1_serving_cell_meas_ptr->SNR_is_bad =
      (premimo_avg_SNR_dBx16 < SNR_BAD_THRESHOLD_dBx16) && (avg_SNR_dBx16 < SAIC_SNR_BAD_THRESHOLD_dBx16);
  }
  else
  {
    avg_SNR_dBx16 = fast_average( snr_array_dBx16, valid_num_bursts );
    /* SNR being < 2 dB then SNR is bad flag should be set.*/
    l1_serving_cell_meas_ptr->SNR_is_bad = (avg_SNR_dBx16 < SNR_BAD_THRESHOLD_dBx16) ? TRUE : FALSE;
  }
   
  /* Save pst-mimo SNR value*/
  l1_serving_cell_meas_ptr->snr = avg_SNR_dBx16;
  /*  Serving Cell Measurements */
  /* Populate l1_serving_idle_meas for L1_send_MPH_SERVING_IDLE_MEAS_IND
   * and others.
   */
  l1_serving_cell_meas_ptr->rx_power_instant = power_instant;
  }

  /* Do processing that only applies on our own paging block */
  if (rpt[0]->own_paging_block)
  {
    /* Update AFC and TT.  This assumes that the data callback
     * occurs _before_ the metrics callback.
     */
    if (l1_local_idle_data_ptr->idle_rx_in_progress == 0)
    {
      /* No other receives are in progress.  Do AFC and
       * time tracking right now so we can go to sleep
       * sooner.  NOTE: mdsp might not get a chance to
       * make the adjustments until the next time we
       * wake up.
       */
      gl1_msg_cch_tt_adjust(gas_id);
      gl1_msg_cch_afc_adjust(gas_id);
      l1_local_idle_data_ptr->idle_update_afc_tt = FALSE;
    }
    else
    {
      l1_local_idle_data_ptr->idle_update_afc_tt = TRUE;
    }

#ifdef FEATURE_GSM_TDS
    if(l1_sc_get_tds_irat_active(gas_id))
    {
      avg_meas_count = GL1_TDS_RUNNING_AVG_COUNT;
      MSG_GERAN_HIGH_0_G("update running average 4x");
    }
    else
#endif /*FEATURE_GSM_TDS*/
    {
      avg_meas_count = 1;
    }
    
    if(valid_num_bursts > 0)
    {
    /* Update running average  - input the averaged block power twice to give more weight*/
    for (i = 0; i < avg_meas_count; i++)
    {
      l1_running_avg_add_meas(ra_ptr,
                              power_instant,
                              divrx_power_instant,
                              pwr_meas_type,
                              gas_id );
    }
    }
    else
    {
        MSG_GERAN_HIGH_0_G("No need to update running avg as all bursts are invalid/yielded");  
    } 
    power = l1_running_avg_get_avg( ra_ptr );

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    if(PRX_DIVRX_MEAS == pwr_meas_type)
    {
      divrx_power = l1_running_avg_get_divrx_avg( ra_ptr );
    }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/


    MSG_GERAN_HIGH_1_G("update running average normal: pwr %d",power);

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
      if(power >= SCE_REDUCED_NCELL_LIST_SCELL_MIN_PWR)
      {
         /* scell is strong, so use optimised top 2 mode */
         l1_sci_set_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_SCELL_PWR_EN, gas_id);
      }
      else
      {
         /* scell is weak, so use normal top 6 mode */
         l1_sci_clear_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_SCELL_PWR_EN, gas_id);
      }
#endif

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
    /* In case of GPLT and no RF, return default/canned result */
    MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_metrics_callback: making l1_serving_cell_meas.rx_power");
    l1_serving_cell_meas_ptr->rx_power = gplt_scan_power_data.pwr_dBm_x16;
#else
    l1_serving_cell_meas_ptr->rx_power = power;
    l1_serving_cell_meas_ptr->divrx_power = divrx_power;
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

    l1_serving_cell_meas_ptr->no_of_measurements = 1;

#ifdef FEATURE_GSM_QSC_TX_DIV
      MSG_HIGH ( "power_inst =%d   power_idle_average = %d  " ,power_instant,power_idle_average,0 );
 if (gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE2))
 {
      avg_cntr[gas_id]++;
      power_idle_average4[gas_id]=power_idle_average3[gas_id];
      power_idle_average3[gas_id]=power_idle_average2[gas_id];
      power_idle_average2[gas_id]=power_idle_average1[gas_id];
      power_idle_average1[gas_id] = (power_instant ) ;
      MSG_HIGH ( "power_inst =%d   power_idle_average = %d  " ,power_instant,power_idle_average,0 );
      if (avg_cntr [gas_id]>= 4)
      {
       avg_cntr[gas_id] --;
       power_idle_average [gas_id]= (((power_idle_average1[gas_id]+power_idle_average2[gas_id]+power_idle_average3[gas_id]+power_idle_average4[gas_id])/4)>>4 );
       MSG_HIGH ( "  power_idle_average = %d  " ,power_idle_average[gas_id],0,0 );
       if ( power_idle_average[gas_id] >=idle_mdm_threshold[gas_id])
       {
       MSG_HIGH ( "set idle MDM_switch_permission true " ,0,0,0 );
       gl1_trm_ant_switch_set_control( TRM_ANT_SWITCH_CTRL_RELEASE ,gas_id);

       }
       else
   {
        MSG_HIGH ( "set idle MDM_switch_permission false " ,0,0,0 );
        gl1_trm_ant_switch_set_control( TRM_ANT_SWITCH_CTRL_REQUEST ,gas_id );

       }

       if (( gsm_idle_sens[gas_id]<power_idle_average[gas_id])&& ( power_idle_average[gas_id] <idle_mdm_threshold [gas_id]))
     {
           MSG_HIGH (" idle  Neither MDM nor QSC does not have switch permission (to avoid ping pong ) ",0,0,0 );
       }

       if ((power_idle_average [gas_id]< gsm_idle_sens [gas_id])&&pch_crc_failure[gas_id])
       {
            MSG_HIGH (" idle qsc sends switch request to trm  ",0,0,0 );
             if (qsc_got_switch_control[gas_id])
       {

                 if (type2_alg_antenna_to_use[gas_id] ==1)
         {
                   type2_alg_antenna_to_use [gas_id]= 2;
         }
         else
         {
                  type2_alg_antenna_to_use [gas_id]= 1;
                  }
                 gl1_hw_switch_antenna_tx_div_ccch_fail(TRUE, gas_id);
				 clear_idle_power_avg_buffer (gas_id);
         }
       }



     }
   }
#endif /* FEATURE_GSM_QSC_TX_DIV */

    if ( (power == 0) ||
         (power < SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16) )
    {
      l1_set_serving_RLA_C(SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16, gas_id);
    }
    else
    {
      l1_set_serving_RLA_C(power, gas_id);
    }
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
    /* In case of GPLT and no RF, return default/canned result */
    MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_metrics_callback: making serving_RLA_C");
    l1_set_serving_RLA_C(gplt_scan_power_data.pwr_dBm_x16, GERAN_ACCESS_STRATUM_ID_1);
#endif /* defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT)*/

    /* Send measurement report to (G)RR. */
    L1_send_MPH_SERVING_IDLE_MEAS_IND( idle_data_ptr->campedon_cell_BSIC, gas_id );

#ifdef FEATURE_GSM_COEX_SW_CXM
      garb_cxm_set_desense_band(gas_id);
      garb_cxm_power_indication_arfcn(idle_data_ptr->campedon_cell_ARFCN,
                                      gpl1_get_rx_pwr_avg_dbm10_coex(gas_id),
                                      gas_id);
#endif

    L1_send_MPH_SERVING_AUX_MEAS_IND( TRUE, gas_id );
  }
#ifdef FEATURE_GSM_COEX_SW_CXM
  else
  /* CCCH power should be reported to avoid Desence */
  {
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
     // In case of GPLT and no RF, return default/canned result
     MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_metrics_callback: making l1_serving_cell_meas.rx_power");
     l1_serving_cell_meas_ptr->rx_power = gplt_scan_power_data.pwr_dBm_x16;
#else
     l1_serving_cell_meas_ptr->rx_power = power_instant;
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
     l1_serving_cell_meas_ptr->no_of_measurements = 1;

     garb_cxm_set_desense_band(gas_id);
     garb_cxm_power_indication_arfcn(idle_data_ptr->campedon_cell_ARFCN,
                                     gpl1_get_rx_pwr_avg_dbm10_coex(gas_id),
                                     gas_id);
   }
#endif /* FEATURE_GSM_COEX_SW_CXM */


   /* Output a debug message showing the SNRs */
   l1_snr_msg( snr_array_msglog, valid_num_bursts, gas_id );

  /* LOG BURST METRICS */
  l1_log_burst_metrics ( GL1_MSG_CCCH, rpt, gas_id  );

  /* Log auxiliary measurements */
  l1_log_serving_aux_meas(gas_id);

  /* If we got a big timing error then print an error
   * message.
   */
  if (big_te)
  {
    MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
            "xCCH timing error > 8 qs: %d, %d, %d, %d",
            te_array[0], te_array[1], te_array[2], te_array[3]);
  }
  return;
}
/******************************************************************************
 *
 *  Function name:  cbch_metrics_callback
 *  ------------------------------------
 *
 *  Description:
 *  ------------
 *  Callback function called to process metrics for each burst in a
 *  CBCH block.
 *
 *  This function also checks whether it is time to send the next measurement
 *  report to RR, and if so, sends the report.
 *
 *  Parameters:
 *  -----------
 *  Metrics report pointer.
 *
 *
 *  Returns:
 *  --------
 *  nothing
 *
 *****************************************************************************/
static void cbch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
   /* LOG BURST METRICS */
   l1_log_burst_metrics ( GL1_MSG_CBCH, rpt, gas_id  );
}

/******************************************************************************
 *
 *  Function name:  l1_init_idle_data
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Initializes idle data values
 *
 *  Parameters:
 *  -----------
 *  NONE
 *
 *  Return Value:
 *  --------
 *  NONE
 *****************************************************************************/

void l1_init_idle_data( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   /* Zero out l1_idle_data */
   memset ( l1_idle_data, NULL , sizeof ( idle_data_T) );

   /* Initialise the running average struct */
   l1_running_avg_init( &l1_idle_data->campedon_cell_pwr_meas_avg,
                        l1_get_idle_serv_meas_buf(PRX_MEAS_BUFFER, gas_id),
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                        l1_get_idle_serv_meas_buf(DIVRX_MEAS_BUFFER, gas_id),
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                        L1_IDLE_SERV_MEAS_BUF_LEN,
                        gas_id);

   /* New Idle configuration clear the non-DRX timer just in case its still active */
   l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag = FALSE;

   l1_idle_data->first_ext_pch_block_read = FALSE;



   /* Start the SAIC disable timer for idle and leave running */
   l1_idle_saic_disable_timer_start( gas_id );

#ifdef FEATURE_GL1_GPLT
   l1_set_rach_ready_to_send( TRUE, gas_id );
#endif
}

/*===========================================================================

FUNCTION  CBCH_ABORT_CALLBACK

DESCRIPTION
  Callback function called when a CBCH block has been aborted

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void cbch_abort_callback( gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   l1_local_idle_data_ptr->cbch_rx_in_progress    = 0;
   l1_local_idle_data_ptr->cbch_abort_in_progress = FALSE;

   l1_clear_cbch_activity(gas_id);
}

/*===========================================================================

FUNCTION  gl1_get_serving_cell_arfcn

DESCRIPTION
  Return info about the serving cell ARFCN.

DEPENDENCIES
  None

RETURN VALUE
  serving cell ARFCN

SIDE EFFECTS
  None

===========================================================================*/
ARFCN_T gl1_get_serving_cell_arfcn( gas_id_t gas_id )
{
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Check that we are in Idle Mode.  If not data is
    * probably incorrect.
    */
   if ( (l1_tskisr_blk->l1_state != L1_IDLE_MODE)
        && (l1_tskisr_blk->l1_state != L1_GPRS_IDLE_MODE)
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        && (l1_tskisr_blk->l1_state != L1_MULTI_SIM_IDLE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      )
   {
      /* This is also used in Packet transfer mode */
      MSG_GERAN_LOW_0_G("Maybe bad serving info.");
   }

   return l1_idle_data->campedon_cell_ARFCN;
}

/*===========================================================================

FUNCTION  gl1_get_serving_cell_BSIC

DESCRIPTION
  Returns info about the serving cell BSIC

DEPENDENCIES
  None

RETURN VALUE
  serving cell BSIC

SIDE EFFECTS
  None

===========================================================================*/

uint8 gl1_get_serving_cell_BSIC( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Check that we are in Idle Mode.  If not data is
    * probably incorrect.
    */
   if ( (l1_tskisr_blk->l1_state != L1_IDLE_MODE)
        && (l1_tskisr_blk->l1_state != L1_GPRS_IDLE_MODE)
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        && (l1_tskisr_blk->l1_state != L1_MULTI_SIM_IDLE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      )
   {
      MSG_GERAN_LOW_0_G("Maybe bad serving info.");
   }

   return(uint8)(l1_idle_data->campedon_cell_BSIC);
}

/*===========================================================================

FUNCTION  gl1_get_serving_cell_rx_power

DESCRIPTION
  Returns info about the serving cell Rx power

DEPENDENCIES
  None

RETURN VALUE
  serving cell Rx power

SIDE EFFECTS
  None

===========================================================================*/

dBx16_T gl1_get_serving_cell_rx_power( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Check that we are in Idle Mode.  If not data is
    * probably incorrect.
    */
   if ( (l1_tskisr_blk->l1_state != L1_IDLE_MODE)
        && (l1_tskisr_blk->l1_state != L1_GPRS_IDLE_MODE)
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        && (l1_tskisr_blk->l1_state != L1_MULTI_SIM_IDLE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      )
   {
      MSG_GERAN_ERROR_0_G("Maybe bad serving info.");
   }

   if (l1_running_avg_get_num_meas(&l1_idle_data->campedon_cell_pwr_meas_avg) > 0)
   {
      return l1_running_avg_get_avg(&l1_idle_data->campedon_cell_pwr_meas_avg);
   }
   else
   {
      return SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;
   }
}


/*===========================================================================

FUNCTION  FAST_AVERAGE

DESCRIPTION
  Returns the average of the elements of array.  num must be 2,3 or 4.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static dBx16_T fast_average( dBx16_T* array, uint32 num )
{
   dBx16_T result = array[0] + array[1];

   switch ( num )
   {
      case 1:
         result = array[0];
         break;

      case 2:
         result /= 2;
         break;

      case 3:
         result += array[2];
         result /= 3;
         break;

      case 4:
         result += array[2];
         result += array[3];
         result /= 4;
         break;

      default:
         /* should be impossible */
         MSG_GERAN_ERROR_1( "Unexpected no. of bursts %d", num );
         break;
   }

   return result;
}

/*===========================================================================

FUNCTION  L1_get_last_idle_sub_state

DESCRIPTION
  This function returns the last stored idle sub state.
  Used by the background HPLMN search engine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
byte L1_get_last_idle_sub_state( gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   return ( l1_local_idle_data_ptr->last_idle_sub_state );
}

/*===========================================================================

FUNCTION  L1_suspend_background_HPLMN_search

DESCRIPTION
  This function suspends the active background HPLMN search

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_suspend_background_HPLMN_search( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

   MSG_GERAN_MED_0_G("Suspending background HPLMN search");

   /* If a rx is scheduled, suspend any background HPLMN search */
   switch ( l1_idle_data->background_HPLMN_search_activity )
   {
         case POWER_SCAN:
            L1_pscan_suspend(0,gas_id);
            break;

         case ACQ_BCCH_LIST:
            L1_bcch_list_acq_bcch_suspend(0, gas_id);
            break;

         case NO_HPLMN_SEARCH:
            l1_local_idle_data_ptr->background_HPLMN_search_suspended = TRUE;
            break;

         default:
            break;
   }
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_BPLMN_SRCH_SUSPEND);
#endif
}

/*===========================================================================

FUNCTION  L1_background_HPLMN_search_suspend_cb

DESCRIPTION
  This acq, bcch_list and pscan abort routines call into this
  function after they're done aborting

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_background_HPLMN_search_suspend_cb( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   MSG_GERAN_MED_2_G("Background HPLMN search suspended FN=%d id:%d",gl1_get_FN( gas_id ),l1_tskisr_blk->current_params.L1_confirm_ID);
   l1_local_idle_data_ptr->background_HPLMN_search_suspended = TRUE;

  /* Send MPH_DECODE_BCCH_LIST_CNF to RR if specified */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( ((gpl1_get_multi_sim_bplmn_state(gas_id) == L1_MULTI_SIM_BGRND_HPLMN_RUNNING) ||
        (gpl1_get_multi_sim_bplmn_state(gas_id) == L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING))
       &&
       (l1_tskisr_blk->current_params.L1_confirm_ID != L1_NULL_CNF) )
  {
    /* in BPLMN modes under dual sim don't allow the confirmation to be sent until the
    suspension is completed. This is because if the BPLMN is aborting RR may send a
    requests to start another action before the abort and cleanup has completed */
    MSG_GERAN_MED_0_G("Deferring Background HPLMN abort confirmation");
  }
  else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_DECODE_BCCH_LIST_CNF )
  {
     /* reset to a NULL_CNF so that we dont send the BCCH_LIST_CNF twice */
     l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
     l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
     l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     gpl1_set_multi_sim_bplmn_state(L1_MULTI_SIM_BGRND_HPLMN_END,gas_id);
#else
     l1_idle_data->g_bplmn_running_state = BPLMN_INACTIVE;
#endif
     l1_tskisr_blk->current_params.L1Data.pIdle_data =
     l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;
     L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE, gas_id);
  }
  else if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_POWER_SCAN_CNF )
  {
     /* reset to a NULL_CNF so that we dont send the POWER_SCAN_CNF twice */
     l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
     l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
     l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     gpl1_set_multi_sim_bplmn_state(L1_MULTI_SIM_BGRND_HPLMN_END,gas_id);
#else
     l1_idle_data->g_bplmn_running_state = BPLMN_INACTIVE;
#endif
     l1_tskisr_blk->current_params.L1Data.pIdle_data =
     l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;
     L1_send_MPH_POWER_SCAN_CNF(FALSE, gas_id);
  }
  else if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_DEACTIVATE_BPLMN_CNF )
  {
     l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
     l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
     l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     gpl1_set_multi_sim_bplmn_state(L1_MULTI_SIM_BGRND_HPLMN_END,gas_id);
#else
     l1_idle_data->g_bplmn_running_state = BPLMN_INACTIVE;
#endif
  }
  /*lint --restore*/

  l1_fm_clear_required_frames(FM_PRI_BACKGROUND_PLMN, gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_BPLMN_SRCH_SUSPEND);
#endif

}

/*===========================================================================

FUNCTION  L1_background_HPLMN_search_running

DESCRIPTION
  This function determines if a background HPLMN search activity is
  running.   We could have a search going on but it could be suspended
  by a CCCH rx block etc

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_background_HPLMN_search_running( gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   return ( l1_local_idle_data_ptr->background_HPLMN_search_suspended == FALSE );
}

/*===========================================================================

FUNCTION  L1_background_HPLMN_search_in_progress

DESCRIPTION
  This function determines if a background HPLMN search activity is
  scheduled.   We could have a search going on but it could be suspended
  by a CCCH rx block etc

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean L1_background_HPLMN_search_in_progress( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   return ( l1_idle_data->background_HPLMN_search_activity != NO_HPLMN_SEARCH );
}

/*===========================================================================

FUNCTION  L1_get_current_background_HPLMN_search_activity

DESCRIPTION
  This function determines what component of the background HPLMN search
  is running i,e ACQ_LIST, POWER_SCAN or BCCH_LIST

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
background_HPLMN_activity_type L1_get_current_background_HPLMN_search_activity(gas_id_t gas_id)
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  return ( l1_idle_data->background_HPLMN_search_activity );
}


/*===========================================================================

FUNCTION  gpl1_gprs_setup_non_drx

DESCRIPTION
  This function sets up idle non drx periods. it determines these from the
  nc_non_drx_period and the non_drx_period. It should be called upon leaving
  transfer mode, or single block transfer after sending a measurement report.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_gprs_setup_idle_non_drx( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T  *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  uint32 current_fn = gl1_get_FN( gas_id );

  /* We have sent just a PMR and are returning back to IDLE, use the NC_NON_DRX_PERIOD */
  if( (l1_transfer_data[gas_id].current_alloc_type == L1_SINGLE_BLOCK_ALLOC) &&
      (l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nc_non_drx_valid) )
  {
    l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN =
      nc_non_DRX_period[l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nc_non_drx_period];
  }
  else
  /* We have just left transfer mode use the NON_DRX_PERIOD - This was not a PMR  */
  if (idle_data_ptr->non_drx_timer < idle_data_ptr->drx_timer_max)
  {
    l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN = idle_data_ptr->non_drx_timer * 216;
  }
  else
  {
    l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN = idle_data_ptr->drx_timer_max * 216;
  }

  if (l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN > 0)
  {
    l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag = TRUE;
    l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN = ADD_FN( current_fn, l1_local_idle_data_ptr->l1_idle_last_non_drx_timer_FN );
  }
  else
  {
    l1_local_idle_data_ptr->l1_idle_non_drx_timer_flag = FALSE;
  }
}

/*===========================================================================
FUNCTION l1_idle_saic_disable_timer_handler

DESCRIPTION
  Handler called when the SAIC in idle timer expires and decides whether
  the idle SAIC processing can be disabled or not.
  Always clears the current averager vars to make sure we always start
  from fresh.
===========================================================================*/
static void l1_idle_saic_disable_timer_handler( unsigned long gas_id )
{
  l1_local_idle_data_t *loc_l1_local_idle_data_ptr = &l1_local_idle_data[(int)gas_id];
  boolean               saic_disable_valid;

  /* Check if we can disable SAIC in idle */
  saic_disable_valid = gl1_msg_is_saic_disable_in_idle_valid( (gas_id_t)gas_id );

  if (saic_disable_valid)
  {
    /* Stop the timer, restarted when we receive a bad rx_qual block */
    l1_idle_saic_disable_timer_deactivate( (gas_id_t)gas_id );
  }
  else
  {
    MSG_GERAN_ERROR_0_G( "SAIC Disable timer handler clear SAIC disable" );
  }

  /* Disable SAIC in idle and reset RXQUAL averager */
  gl1_msg_set_saic_disable_in_idle( saic_disable_valid, (gas_id_t)gas_id );
  gl1_msg_reset_hard_rxqual_ave( (gas_id_t)gas_id );

  loc_l1_local_idle_data_ptr->is_saic_disable_possible = saic_disable_valid;

  return;
}

/*===========================================================================
FUNCTION l1_idle_saic_disable_timer_start

DESCRIPTION
  Starts the ncell monitor shutdown timer for the time period defined
===========================================================================*/
void l1_idle_saic_disable_timer_start( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  gl1_rxd_control_type gl1_rxd_ctl_flag = gl1_get_rxd_control_flags(gas_id);
  boolean    gl1_in_ftm_mode = (ftm_get_mode() == FTM_MODE)? TRUE:FALSE;

  if ( l1_local_idle_data_ptr )
  {
    MSG_GERAN_LOW_0_G( "SAIC_IDLE_DISABLE timer start" );

    /* Turn SAIC in idle back on for the next PCH */
    gl1_msg_set_saic_disable_in_idle( FALSE, gas_id );

    /* Reset averager for SAIC disable */
    gl1_msg_reset_hard_rxqual_ave( gas_id );

    l1_local_idle_data_ptr->is_saic_disable_possible = FALSE;

    /* If RxdForcedOn NV is set, Do not start disable timer because Rxd should be always on */
    if(gl1_rxd_ctl_flag.RxdForcedOn && gl1_in_ftm_mode)
      return;

  /* Start the timer, when expires call handler */
    (void)rex_set_timer( &l1_local_idle_data_ptr->l1_idle_saic_disable_timer,
                         L1_IDLE_SAIC_DISABLE_TIMEOUT );
}
}

/*===========================================================================
FUNCTION l1_idle_saic_get_timer_remaining

DESCRIPTION
  Gets the time remaining value in ms for the SAIC timer.
===========================================================================*/
uint32 l1_idle_saic_get_timer_remaining(gas_id_t gas_id)
{
  uint32 xCCHtimerMilliSec = 0;
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  if (l1_local_idle_data_ptr)   
  {
    /* Get the remaining count for the SAIC timer */
    xCCHtimerMilliSec = (uint32)rex_get_timer(&l1_local_idle_data_ptr->l1_idle_saic_disable_timer);
  }
  return xCCHtimerMilliSec;
}

/*===========================================================================
FUNCTION l1_idle_saic_disable_timer_create

DESCRIPTION
  creates/defines the idle saic disable timer
===========================================================================*/
void l1_idle_saic_disable_timer_create( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  /* Create a timer with handler for SAIC disable monitoring */
  memset(&(l1_local_idle_data_ptr->l1_idle_saic_disable_timer), 0, sizeof(rex_timer_type));

  rex_def_timer_ex( &(l1_local_idle_data_ptr->l1_idle_saic_disable_timer),
                    (rex_timer_cb_type)l1_idle_saic_disable_timer_handler,
                    (unsigned long)gas_id );
}

/*===========================================================================
FUNCTION l1_idle_saic_disable_timer_deactivate

DESCRIPTION
  Deactivates the ncell monitor shutdown timer
===========================================================================*/
void l1_idle_saic_disable_timer_deactivate( gas_id_t gas_id )
{
  l1_local_idle_data_t *loc_l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  /* Disable the timer */
  (void)rex_clr_timer( &loc_l1_local_idle_data_ptr->l1_idle_saic_disable_timer );
}

#if defined (FEATURE_DUAL_SIM) || defined ( FEATURE_TRIPLE_SIM)

/******************************************************************************
 *
 *  Function name:  l1_get_page_skipping_factor
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 * This function is to calculate the page skipping factor if needed based on the below conditions:
 *   1.  TRM extension flag will be used to start/stop PCH drop mechanism.
 *   2.  WCDMA will set the extension flag when it requires TRM lock of 300ms.
 *   3.  WCDMA will clear the extension flag when it no longer requires TRM lock for 300ms.
 *   4.  GERAN will double the paging DRx cycle for one subscription only provided the following conditions are met:
 *         a.  The extension flag is set
 *         b.  Mobile is operating in W+G+G mode, not in G+G mode
 *         c.  Paging DRx cycle is 470ms for both G subscriptions.
 *         d.  Paging sub channel alignments for the two subscriptions are such that the gap between the two PCH reservations is <300ms.
 *         e.  Neither subscription is in non-DRx mode.
 *   5.  The extension flag will have no impact on any other reservations (SCH, FCCH, BCCH etc).
 *
 *
 *  Returns:
 *  --------
 *  page_skipping_factor
 *
 *
 * Note: in WGG mode This function applies page skipping to lowest gas id when applicable
 *
 * In GGG mode if one of the SUB is in BG Traffic, and other two subs are in idle
 * if any of the SUB is using a paging DRX cycle of 470ms, alternate page skip is implemented.
 *
 *****************************************************************************/

static uint8 l1_get_page_skipping_factor( idle_data_T  *idle_data_ptr , gas_id_t gas_id)
{
  uint8 page_skipping_factor = 1;
#ifndef FEATURE_GPRS_GBTA
  idle_data_T *other_sub_idle_data_ptr;
  uint32 this_sub_pch_gap, other_sub_pch_gap;
  gas_id_t other_idle_sub_gas_id = gl1_get_gas_id_of_other_idle_sub( gas_id );
  int as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );
  int as_idx_other_idle_sub = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( other_idle_sub_gas_id );

  if( other_idle_sub_gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED )
  {
    return page_skipping_factor;
  }

  if( IS_WCDMA_VOTED_PAGE_SKIP() && ( gl1_get_num_idle_subs()== 2 ) )
  {
    if( as_idx < as_idx_other_idle_sub )
    {
      other_sub_idle_data_ptr= &l1_idle_data_store[other_idle_sub_gas_id];
      if(( idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 2 ) && ( other_sub_idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 2 ))
      {
        this_sub_pch_gap    = calculate_gap_to_next_pch_frame( idle_data_ptr, gas_id );
        other_sub_pch_gap   = calculate_gap_to_next_pch_frame( other_sub_idle_data_ptr, other_idle_sub_gas_id );
        if( GET_PCH_GAP_DIFF( this_sub_pch_gap, other_sub_pch_gap ) < L1_PAGE_SKIP_FRAMES_GAP )
        {
          page_skipping_factor = 2;
        }
      }
    }
  }
  else if( gl1_check_any_other_sub_in_bg_traffic( gas_id ) && ( gl1_get_num_idle_subs() == 2 ) )
  {
    if( idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 2 )
    {
      page_skipping_factor =2;
    }
  }
#endif

 return page_skipping_factor;

}


/******************************************************************************
 *
 *  Function name:  calculate_gap_to_next_pch_frame
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 * Calculates the number of frames between the current frame and the frame
 * the next PCH receive starts
 *
 *
 *  Parameters:
 *  -----------
 *  idle_data_ptr - pointer to the idle data store containing paging info etc
 *
 *
 *  Returns:
 *  --------
 *  Gap to next PCH block start
 *
 *****************************************************************************/
#ifndef FEATURE_GPRS_GBTA

static uint32 calculate_gap_to_next_pch_frame( idle_data_T  *idle_data_ptr, gas_id_t gas_id )
{
   const  uint8 paging_frames[9] = {6,12,16,22,26,32,36,42,46};
   uint32 gap_to_next_activity = 0;
   uint32 current_frame_pos,paging_frame_pos,local_FN,paging_period ;

   if ( idle_data_ptr != NULL)
   {
      local_FN          = gl1_get_FN( gas_id );
      paging_period     = idle_data_ptr->campedon_cell_data.BS_PA_MFRMS*MULTIFRAME_51;
      current_frame_pos = local_FN % paging_period;
      paging_frame_pos  = (idle_data_ptr->paging_data.paging_multiframe * MULTIFRAME_51) +
                          paging_frames[idle_data_ptr->paging_data.paging_block_index];

      current_frame_pos += 2;

      if (current_frame_pos > paging_period)
      {
        current_frame_pos -= paging_period;
      }

      if ( paging_frame_pos >= current_frame_pos )
      {
        gap_to_next_activity = paging_frame_pos - current_frame_pos;
      }
      else
      {
        gap_to_next_activity = (paging_frame_pos + paging_period) - current_frame_pos;
      }

      if ( (local_FN + gap_to_next_activity) >= (uint32)(FRAMES_IN_HYPERFRAME) )
      {

        gap_to_next_activity = (FRAMES_IN_HYPERFRAME + paging_frame_pos) - local_FN - 2;
      }

      return gap_to_next_activity;

   }
  return 0;
}
#endif
#endif

/*===========================================================================
FUNCTION l1_set/get_asynch_int_possible

DESCRIPTION
  Inform lower layers if asynch int processing is possible
===========================================================================*/
void l1_set_asynch_int_possible( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_p = &l1_local_idle_data[gas_id];
  gl1_hw_sleep_type  *gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]];

  if ( l1_local_idle_data_p )
  {
    l1_local_idle_data_p->l1_is_asynch_int_possible = FALSE;

    if ( !l1_sc_wcdma_srch_active(gas_id)
#ifdef FEATURE_GPRS_GBTA
         /* Don't allow Asynch processing in GBTA as not useful */
         && !IS_GBTA_ACTIVE()
#endif /* FEATURE_GPRS_GBTA */
       )
    {
      /* Always allow for Asynch sleep enable */
      if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) )
      {
        l1_local_idle_data_p->l1_is_asynch_int_possible = TRUE;
      }
      /* If OPT3 then check SAIC off and LMM state */
      else if ( gl1_msg_is_sleep_opt3_enabled( gas_id )
                && l1_sc_ncell_pch_low_monitor_active(gas_id)
                && l1_local_idle_data_p->is_saic_disable_possible
       )
    {
        l1_local_idle_data_p->l1_is_asynch_int_possible = TRUE;
      }
    }

   gl1_hw_sleep_cycle->async_msg.saic_status = l1_local_idle_data_p->is_saic_disable_possible;

    if ( gl1_asynch_int_dbg( gas_id ) )
    {
#ifdef FEATURE_GPRS_GBTA
      MSG_GERAN_HIGH_3_G( "irat_srch_active:%d l1_asynch_int_possible:%d GBTA:%d",
                l1_sc_wcdma_srch_active( gas_id ), l1_get_asynch_int_possible( gas_id ),
                IS_GBTA_ACTIVE() );
#else
      MSG_GERAN_HIGH_2_G( "irat_srch_active:%d l1_asynch_int_possible:%d",
                l1_sc_wcdma_srch_active( gas_id ), l1_get_asynch_int_possible( gas_id ) );
#endif /* FEATURE_GPRS_GBTA */
    }
  }
    }

boolean l1_get_asynch_int_possible( gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_p = &l1_local_idle_data[gas_id];

  if ( l1_local_idle_data_p )
  {
    boolean irat_search_active = l1_sc_wcdma_srch_active( gas_id );

    /*
     * Always check if an irat search has been configured in between the idle
     * CCCH being configured to allow the Asynch to be ignored if any irat
     * activity requested whilst waiting for the Asynch Sleep message to be returned
     */
    if ( l1_local_idle_data_p->l1_is_asynch_int_possible &&
         irat_search_active )
    {
      if ( gl1_asynch_int_dbg( gas_id ) )
      {
        MSG_GERAN_HIGH_2_G( "irat_srch_active:%d l1_asynch_int_possible:%d",
                  irat_search_active, l1_local_idle_data_p->l1_is_asynch_int_possible );
      }

      /* Re-populate the asynch possible with correct value if irat status changed */
      l1_local_idle_data_p->l1_is_asynch_int_possible = FALSE;
    }

    return ( l1_local_idle_data_p->l1_is_asynch_int_possible );
  }
  else
  {
    return ( FALSE );
  }
}

/*===========================================================================
FUNCTION l1_is_cell_acq_complete

DESCRIPTION
  Inform lower layers if cell acquired successfully
===========================================================================*/
boolean l1_is_cell_acq_complete( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  return ( gl1_cell_valid_state_check( l1_tskisr_blk->l1_state ) );
}

#if defined (FEATURE_QTA ) && defined (FEATURE_CXM_QTA)
/*===========================================================================
FUNCTION l1_get_qta_active

DESCRIPTION
  Inform lower layers if asynch int processing is possible in QTA mode
===========================================================================*/
boolean l1_get_qta_active( gas_id_t gas_id )
{
  boolean qta_active = l1_tsk_buffer[gas_id].qta_in_progress;

  return ( qta_active );
}
#endif /* FEATURE_QTA */

/*===========================================================================
FUNCTION gl1_hw_abort_idle_act_prog

DESCRIPTION
  This checks if there are any idle acitivities are in progress and aborts
  them. (Only to be) Called from sleep warmup recovery CB.

===========================================================================*/
void gl1_hw_abort_idle_act_prog(gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  /* Abort CCCH Rx */
  MSG_GERAN_HIGH_3_G( "gl1_msg_abort_rx_ccch FN:%d idle_rx_in_progress %d idle_monitors_in_progress %d",
      gl1_get_FN( gas_id ), l1_local_idle_data_ptr->idle_rx_in_progress, l1_local_idle_data_ptr->idle_monitors_in_progress);
  gl1_msg_abort_rx_ccch(gas_id);
  l1_local_idle_data_ptr->idle_rx_in_progress = 0;

  /* Abort PWR measurements */
  if(l1_get_idle_monitors_in_progress(gas_id))
  {
    /* None commands sent to FW yet. So, calling this is safe here.
       This function will abort the monitor SM and reset mDSP buffers etc., */
    gl1_msg_abort_pwr_meas(gas_id);
    l1_local_idle_data_ptr->idle_monitors_in_progress = 0;
  }

  l1_local_idle_data_ptr->idle_update_afc_tt = FALSE;

  if (  l1_fm_current_activity[gas_id] == FM_PRI_CBCH
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        || ( l1_fm_current_activity[gas_id] == FM_PRI_MSIM_CBCH )
#endif
     )
  {
    l1_clear_cbch_activity(gas_id);
  }
  else
  {
    l1_fm_clear_required_frames(l1_fm_current_activity[gas_id], gas_id);
  }
}



/*===========================================================================
FUNCTION gl1_update_xcch_activity_in_progress

DESCRIPTION
  This checks if there are any serv cell xCCH acitivity in progress and
  either updates it or aborts it.

===========================================================================*/
static void gl1_update_xcch_activity_in_progress(gas_id_t gas_id )
{
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  uint8 missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);

  /* If we have missed less than 3 frames, then we still have scope for doing xCCH acitivity */
  if (gl1_msg_ccch_is_rx_on_next_frame(gas_id) && (missed_frames < 3)
#if defined (FEATURE_QBTA)
      /* No PCH error recovery in QTA due to CXM registration */
      && ((!gl1_hw_qta_gap_active(gas_id)) || gl1_hw_check_in_QBTA(gas_id))
#endif
      )
  {
    /* USTMR ref command */
   // mdsp_frame_ref_cmd((uint32) gl1_sleepctl_get_ustmr_at_ols(gas_id), gas_id);

    gl1_msg_recovery_update_ccch_params(gas_id);

    /* Issue scheduled Rx commands, this ONLY populates the mDSP interface buffer */
    gl1_hw_dtm_frame_tick(gas_id);

    /* AFC update command is also required as it is removed from buffer */
    if( mdsp_commands_pending(gas_id) )
    {
      gl1_hw_set_gfw_config_cycle_valid_state(gas_id, TRUE);
      gl1_hw_gfw_config_cycle_swallower(gas_id);
    }

    /* Send the pending commands to GFW as an ASYNC message */
    if (mdsp_commands_pending(gas_id))
    {
      mdsp_wakeup_issue_async_msg (GFW_ASYNC_CMD, gas_id, gl1_hw_get_frame_number_mdsp_wakeup(gas_id) );
      MSG_GERAN_ERROR_0_G("Error Recovery: (Serv CELL xCCCH) Trigger the async command with FN updated with number of missed frames");
    }
  }

  /* Missed an oppurtunity to do any xCCH acitivity. Lets abort it */
  else
  {
    /* Abort CCCH Rx */
    MSG_GERAN_HIGH_3_G( "Abort Rx CCCH FN:%d idle_rx_in_progress %d idle_monitors_in_progress %d",
        gl1_get_FN( gas_id ), l1_sc_is_idle_rx_in_progress(gas_id), l1_get_idle_monitors_in_progress(gas_id));

    gl1_msg_abort_rx_ccch(gas_id);
    l1_sc_reset_idle_rx_in_progress(gas_id);
    gl1_hw_sleep_set_entered_recovery(gas_id, FALSE);

    l1_local_idle_data_ptr->idle_update_afc_tt = FALSE;

    l1_fm_clear_required_frames(FM_PRI_CCCH, gas_id);
  }
}

/*===========================================================================

FUNCTION gl1_update_scheduled_activity

DESCRIPTION
  ISR would have run and updated the mDSP buffer, but we've had to do a recovery due to ISR Overrun.
  This function will go through the list of activities and update/abort them.

DEPENDENCIES
  Only to be in Idle and when an ISR Overrun has occurred.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_update_scheduled_activity(gas_id_t gas_id)
{
  l1_fm_priority_T running_activity = l1_fm_get_running_activity(gas_id);
  idle_data_T      *l1_idle_data_ptr = &l1_idle_data_store[gas_id];
  l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  /* First things first. Reset the mDSP interface as it is dirty now.
     No need reset for NCELL_ACQ (not in QTA) since that activity will be carried
     out without rebuilding the command */
  if((running_activity != FM_PRI_NCELL_ACQ)
#ifdef FEATURE_QTA
     || gl1_hw_qta_gap_active(gas_id)
#endif
    )
  {
    mdsp_sleep_reset_mdsp_intf(gas_id);
    mdsp_frame_tick_process(gas_id);
    /* Also erase now stale list of events in scheduler. This list will be
       correctly rebuilt later */
    gl1_hw_schedule_reset_events (gas_id);

#ifdef FEATURE_QTA
    if(gl1_hw_qta_gap_active(gas_id))
    {
      gl1_hw_reset_qta_flags_skipping_gap(gas_id);
    }
#endif
  }

  /* Abort PWR measurements */
  if(l1_get_idle_monitors_in_progress(gas_id))
  {
    /* No commands sent to FW yet. So, calling this is safe here.
         * This function will abort the monitor SM */
    gl1_msg_abort_pwr_meas(gas_id);

    /*this will ensure we reset the reduced pwr mon index so that we do monitors in the next cycle*/
    l1_idle_data_ptr->idle_rx_missed_page = TRUE;
  }

  switch(running_activity)
  {
    /* Idle serving cell activities */
    case FM_PRI_CCCH:
      gl1_update_xcch_activity_in_progress(gas_id);
      break;

    case FM_PRI_CBCH:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    case FM_PRI_MSIM_CBCH:
#endif
    {
      int8 cbch_offset;
      /* CBCH SM is agnostic of missed frames. Abort the activity*/
      MSG_GERAN_ERROR_0_G("Error Recovery: Aborting CBCH activity");
#if defined (FEATURE_GSM) && defined (FEATURE_GSM_CB)
      gl1_msg_abort_rx_cbch(gas_id);
#endif
      cbch_offset=find_ota_cbch_offset(l1_local_idle_data_ptr->next_req_cbch_frame,gas_id);
      if (cbch_offset > 0)
      {
        L1_send_CB_MISSING_BLOCK_IND( ADD_FN( l1_local_idle_data_ptr->next_req_cbch_frame, cbch_offset ), gas_id );
        MSG_GERAN_HIGH_2_G("CB Missing indication at FN:%d and offset=%d", l1_local_idle_data_ptr->next_req_cbch_frame,cbch_offset);
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Invalid Fn and CB Missing indication at FN:%d", l1_local_idle_data_ptr->next_req_cbch_frame);
      }
      gl1_drx_require_next_tick(gas_id);
      break;
    }

    /* Idle NCELL activities */
    case FM_PRI_NCELL_BCCH:
    case FM_PRI_NCELL_SCH:
      /* Abort the NCELL Rx activities, this resets the statemachine only */
      MSG_GERAN_ERROR_1_G("Error Recovery: Aborting NCELL activity %d", running_activity);
      abort_receive(gas_id);
      break;

    case FM_PRI_NCELL_ACQ:
#ifdef FEATURE_QTA
      if(gl1_hw_qta_gap_active(gas_id))
      {
        MSG_GERAN_ERROR_1_G("Error Recovery: Aborting NCELL Acquisition %d", running_activity);
        abort_receive(gas_id);
      }
      else
#endif
      {
      /* NCELL ACQ can still be run */
      if(mdsp_commands_pending(gas_id))
      {
        /* Update FCB start fn with the missed frames */
        l1_sc_update_fcb_start_fn( gl1_hw_sleep_get_missed_frames(gas_id), gas_id );
        mdsp_wakeup_issue_async_msg (GFW_ASYNC_CMD, gas_id, gl1_hw_get_frame_number_mdsp_wakeup(gas_id) );
        MSG_GERAN_ERROR_0_G("Error Recovery: (NCELL ACQ) Trigger the async command with FN updated with number of missed frames");
      }
      }
      break;

    case FM_PRI_BACKGROUND_PLMN:
      MSG_GERAN_ERROR_1_G("Error Recovery: BPLMN cancellation during wakeup not implemented!", running_activity);
      break;

    case FM_PRI_WCDMA_IDSEARCH:
    case FM_PRI_WCDMA_RECONF:
#ifdef  FEATURE_GSM_TO_LTE
    case FM_PRI_LTE_IDSEARCH:
#endif
#ifdef FEATURE_GSM_TDS
    case FM_PRI_TDS_ACQ:
#endif
      
      l1_sc_abort_irat_at_sleep_err_recovery(gas_id);
      MSG_GERAN_ERROR_0_G("Error Recovery: aborted IRAT search %d");
      break;

    default:
      MSG_GERAN_ERROR_1_G("Error Recovery: Activity %d cancelation during wakeup not implemented!", running_activity);
      break;
  } /* switch(running_activity) */
  vstmr_schedule_event(gas_id, gl1_hw_get_frame_number_ols(gas_id), FALSE);
}

/*===========================================================================
 FUNCTION l1_sc_reset_idle_rx_in_progress

 DESCRIPTION
   Resets the counter for Idle Rx in Progress.

 DEPENDENCIES


 RETURN VALUE
   None

 SIDE EFFECTS
   None
 ===========================================================================*/

void l1_sc_reset_idle_rx_in_progress( gas_id_t gas_id )
{
   l1_local_idle_data_t *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
   l1_local_idle_data_ptr->idle_rx_in_progress = 0;
}

/*===========================================================================
 FUNCTION l1_set_epd_pref

DESCRIPTION
   Sets the epd_preference.

 DEPENDENCIES


 RETURN VALUE
   None

 SIDE EFFECTS
   None
===========================================================================*/

void  l1_set_epd_pref(rr_l1_epd_preference_t epdpref , gas_id_t gas_id)
{
  if(epdpref != RR_L1_EPD_NO_CHANGE)
{
    epd_preference[gas_id] = epdpref;
  }

}

/*===========================================================================
 FUNCTION l1_get_epd_pref

 DESCRIPTION
   gets the epd_preference.

 DEPENDENCIES


 RETURN VALUE
   TRUE if epd ois set to RR_L1_EPD_ON_PREFERRED else FALSE

 SIDE EFFECTS
   None
 ===========================================================================*/
boolean l1_get_epd_pref(gas_id_t gas_id)
{
  boolean ret_flag = FALSE;
  switch(epd_preference[gas_id])
  {
    case RR_L1_EPD_OFF_PREFERRED:
   	    ret_flag = FALSE;
	    break;

    case RR_L1_EPD_ON_PREFERRED:
        ret_flag = TRUE;
	    break;

    default:
   	    MSG_GERAN_HIGH_1_G("Prefernce %d", epd_preference[gas_id]);
	    break;
  }

  MSG_GERAN_LOW_1_G("epd=%d",ret_flag);
  return ret_flag;
}

/*===========================================================================

FUNCTION gl1_hw_sleep_wakeup_conflict_rebook

DESCRIPTION
  Handles rebooking for new activity during conflict.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_hw_wmgr_drx_require_tick(uint32 reprog_fn, gas_id_t gas_id)
{
  l1_local_idle_data_t    *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  uint8 prev_drx_ref;
  uint32 prev_req_frame;

  prev_req_frame = l1_local_idle_data_ptr->next_req_frame;
  prev_drx_ref = l1_local_idle_data_ptr->drx_ref;

  /* remove old frame */
  if (reprog_fn != prev_req_frame)
  {
    /* for DRX, tell that we want to have an activity at this frame number */
    if(
      (gl1_hw_sleep_wmgr_result[gas_id] & GL1_HW_SLEEP_WMGR_RES_BK_PRI_MASK)
      ==
      GL1_HW_SLEEP_WMGR_RES_BK_PRI(FM_PRI_CCCH) )
    {
      l1_local_idle_data_ptr->next_req_frame = reprog_fn;
    }
    l1_local_idle_data_ptr->drx_ref = gl1_drx_require_tick(reprog_fn, gas_id );
  }
  else
  {
    /* TODO: chk */
    MSG_GERAN_MED_0_G("WMGR_SLEEP: WARN: DRX rebook not required");
  }

  MSG_GERAN_MED_4_G("WMGR_SLEEP: DRX rebook: prev_frame: %d, new_frame: %d, prev_drx_ref: %d, new_drx_ref: %d",
    prev_req_frame, l1_local_idle_data_ptr->next_req_frame, prev_drx_ref, l1_local_idle_data_ptr->drx_ref);
}

/*===========================================================================

FUNCTION gl1_idle_get_gap_to_next_activity

DESCRIPTION
  Return the gap to the next Rx activity

DEPENDENCIES
  None

RETURN VALUE
  Gap to next activity as described above.  0 if no gap.

SIDE EFFECTS
  None

===========================================================================*/
uint32 gl1_idle_get_gap_to_next_activity(uint32 frame_offset, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    idle_data_T *idle_data_ptr;
    uint32 gap_to_next_activity;

    idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /* calculate gap to next activity */
    gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, frame_offset, gas_id );
#else
    /* calculate gap to next activity */
    gap_to_next_activity = calculate_gap_to_next_activity( idle_data_ptr, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

    return gap_to_next_activity;
}

/*===========================================================================

FUNCTION gl1_idle_release_currect_ccch_drx_tick

DESCRIPTION
  Release DRX tick for current CCCH activity

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_idle_release_currect_ccch_drx_tick( gas_id_t gas_id )
{
  l1_local_idle_data_t    *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];

  gl1_drx_release_fn_tick(l1_local_idle_data_ptr->next_req_frame, gas_id);

  l1_local_idle_data_ptr->next_req_frame = GL1_DEFS_INVALID_FN;
}

#if defined( FEATURE_G2T_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
/*===========================================================================

FUNCTION l1_get_reorg_sub_ccch_fn

DESCRIPTION
  This functions returns the Frame number to be booked(Reorg sub) after tune=away to other idle sub 

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
uint32 l1_get_reorg_sub_ccch_fn(gas_id_t gas_id)
{ 
  l1_local_idle_data_t      *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  return (l1_local_idle_data_ptr->l1_idle_ta.ta_fn + l1_local_idle_data_ptr->l1_idle_ta.ta_dur + FRAMES_PER_BLOCK);
}
/*===========================================================================

FUNCTION l1_is_idle_ta_prepared

DESCRIPTION
  This functions returns TRUE if ta ia prepeared ,FALSE if not

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_is_idle_ta_prepared(gas_id_t gas_id)
{
  l1_local_idle_data_t      *l1_local_idle_data_ptr = &l1_local_idle_data[gas_id];
  return(l1_local_idle_data_ptr->l1_idle_ta.ta_prepared);
}
#endif /* (FEATURE_G2T_IDLE_TUNEAWAY ||FEATURE_G2X_REORG_TUNEAWAY) */

boolean l1_get_non_drx_timer_flag(gas_id_t gas_id)
{
  boolean non_drx_timer_flag;
  non_drx_timer_flag = l1_local_idle_data[gas_id].l1_idle_non_drx_timer_flag;
  return non_drx_timer_flag;
}  

/* EOF */
