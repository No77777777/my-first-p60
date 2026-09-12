/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          L 1   M A I N

GENERAL DESCRIPTION
   This module contains the entry point for the L1 task. It
   calls initialisation functions for the tasks, then sits in an infinite
   loop collecting messages and distributing them to L1.

EXTERNALIZED FUNCTIONS
  gsm_l1_main    L1 'task' entry point.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_task.c#9 $
$DateTime: 2024/09/09 06:12:21 $ $Author: pwbldsvc $


when       who       what, where, why
--------   ---       ---------------------------------------------------------
12/02/19   ng        CR2386174 Defer Panic reset until IRAT state machine is clear, remove block in QSH handler
30/11/21   sal      CR3069703 Add mutex protection for mcpm_gsm_param read/write.
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
06/01/20   sc       CR2571123 Update MCPM for STOP sfter G2X init_cnf in all states except inactive. 
06/01/20   sc      CR2561139 Update MCPM about Start/Stop for every G2X IRAT search gap in Idle mode.
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
15/06/18   ng        CR2261488 Remove 'l1_rcvd_non_drx_ind' related handling, it is no more needed
05/03/18   nsa       CR2199885 while handling L1_GRM_RELEASE message no need of setting TRM status to null which implies releasing RX chain.
06/02/17   sk        CR2002002 Esnure DRX is not turned off before panic reset happens in the task
07/02/18   rc      CR2184167 Sending MAC_L1_RECOVERY_CNF after L1 moved to Idle State.
25/04/18   rv       CR2225983 - GERAN F3 and log pkt Reduction TA.3.0
26/03/18   rc........CR2210144 Calling Inform_grant with TRM DENIAL after grm_request from task when PCHXFER is not the winning client
25/05/17   hd        CR2051434 Make the variable l1_rcvd_non_drx_ind dual spaced
09/02/18   km        CR2184116 Set g2l_abort to TRUE in case of QTA gap abort
14/09/17   ng        CR2109695 Do not do panic reset until G2X state machine is clear
06/07/17   sp        CR2071477 Push loosing client's TRM request of PCHxFER to Task to avoid load on ISR.
28/06/17   sid       CR2068343 Check TRM status from grm_info data instead of TRM
31/10/17   sp        CR2125524 Update frame duration in task context when unlock_cancell is received
18/01/18   km        CR2172348 Addition of GTOL_SRCH_STATE_ABORT_CNF_WAIT in IRAT sm
09/01/18   km        CR2166879 G2L abort handling in QTA/non-QTA scenarios
03/01/18   ksb       CR2164555 Geran support to send Global Time Services Information TO GPS module
25/01/16   cws       CR962379 Disable/enable ASDIV in IDLE/RACH state by NV
10/08/16   snjv      CR1050221 Remove usage of Feature_gsm_rx_tx_split
29/08/16   ng        CR1059347 Do multi sim deact while doing TRM Reservation if resource lock is active during CSFB
29/04/15   zc        CR829225 Trigger G2X abort earlier on panic reset
15/12/17   ap        CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
05/12/17   km        CR987605 G2X immediate cleanup
12/06/17   sn        CR2054908 Handling to back to back DR mode change requests
14/03/16   sn        CR982754 offload TRM exchange operation from ISR to L1_TASK
20/07/16   sp        CR1040702: Allow TA during 2Phase access
10/01/17   cc        CR1109751 clear G2L message CNF flag after receving ABORT CNF
10/30/17   km        CR2133917 GL1 support for L+L
08/07/16   bg        CR1033899 Added mutex lock for G2X Tuneaway to avoid race conditions
02/02/17   br        CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
12/06/17   sn        CR2059023 Handling of MAC_L1_RECOVERY_REQ in States Other than Transfer Mode
16/03/17   mn        CR2020307 FR40846:Packet transfer Error Recovery mechanism.
13/07/16   nm        CR1040020 Fix compilation issues with fEATURE_DUAL_DATA enabled(required for G2L TA FR)
27/01/17   sk        CR1104513 Fix for the crash during panic reset in IDLE mode and DTM mode
24/3/2016  sn        CR990634 Skip RANE when TRM status is TRM ABORTING...
22/08/16   sk        CR1053862 Reduce the number of error messages.
04/05/16   snjv      CR1010404 Drop Tx Chain when resource lock is released in Acq.
29/06/16   hd        CR1035604 Send a message to L1 task to turn tx off when dedicated channel is released.
07/06/16   mn        CR1025190 Remove warning implicit declaration of function 'l1_multi_sim_bcch_reserve_at_priority_change'
01/06/16   dg        CR1023017 Don't set init req variable when already target tech is initialized
26/04/16   sn        CR1000407 Updation of PTMSI in Transfer mode
24/05/16    sn       CR1016717:Removing Excess F3 logging for packet transfer
05/17/16   mn        CR988646 Performing reservation for demod page inv when GL1 in reselection trm wait.
16/05/16   db        CR1009490 Move CBCH priority handling from GRR to GL1
22/01/15   js        CR965841 Reset qta in progress after lock exchange while releasing TRM
14/01/16   dg        CR961154 Don't send GERAN stop to MCPM if X tech hasn't sent STOP cnf
21/12/15   sk	     CR938638 Reducing no of F3s
07/12/15   hd        CR947385 Ensure that the TRM reservation is done properly when the priority is inverted for SIB_PROC
03/12/15   dp        CR929648 reduce power usage when waiting for TRM during find_bcch
04/12/15   sjw       CR947229: code improvements to traffic msg logging
02/12/15   ap        CR939311 Change reason from ACCESS to TRAFFIC only when DRDSDS disabled 
23/11/15   sp        CR942146: Increment/decrement task count irrespective of reservation being made through wakeup cancellation
05/11/15   cah       CR934800 call DR disable in task context when PG is received from ACCESS
11/05/15   cc        CR931979 Move EFS read to l1 task init
28/09/15   cjl       CR929471 Delay sleep after geran lock release in idle
08/10/15   dg        CR920293 Set drx task as active while sending INIT_REQ to Xl1 on receiving DEINIT_CNF
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
05/10/15   dg        CR916160 Call g2l shutdown on receiving init conf from ML1 instead of calling g2x pending shutdown
16/09/15   cjl       CR908527 improve system procedures used for TRM access
21/09/15   nm        CR767743 Using actual duration in trm unlock cb for unlock by event
18/09/15   cws       CR907895 Change unnecessary F3 to LOW 
15/08/15   zf        CR887132: Use variant for IRAT CM_GSM_INIT MSGs.
02/09/15   zf        CR905869: Set trm_status to NULL after grm release
07/09/15   js        CR903209 Backing out CR891616
08/09/15   cah       CR892575: DR DSDS: Add support for diversity chain unlock in transfer mode
07/09/15   mko       CR902403:Decrement Activate task count while processing L1_GRM_REQUET/L1_GRM_DIVRX_RELEASE
03/09/15   kb        CR863908:Added new EFS file structure for sleep control
28/08/15   cja       CR892470 G2X to cope with overlapping init/deinit/init 
12/08/15   pa        CR887572: RxD: Schedule DRX wakeup after Rx/Tx and request TRM in task context.
27/08/15   sjw       CR894729: send MPH_ACQUISITION_STATE_IND(WAITING) for NOMINAL req-and-notify
25/08/15   cjl       CR896193 Obtain a traffic chain if we don't have one when we get a resource lock request
26/08/15   js        CR891616 Setting correct duration to tune away table  
20/08/15   cgc       CR893193 back to back cell updates, wait for IRAT init_cnf/deinit_cnf's before next is actioned.
20/08/15   ap        CR849948 Race condition between handling MPH_SET_PRIORITY_IND and TRM grant callback 
11/08/15   js        CR887692 Updating reserve reason to TRM when received priority chenage indication
12/08/15   cws       CR864883 FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
05/08/15   hd        CR884936: Handle QTA STARTUP and CLEANUP messages in MSGR IF
29/07/15   dp/dv     CR878618: Changes to wait for rf exit confirmation in gl1 for X2W srch
13/07/15   sjv       CR870548 On Panic Reset Disable Cipher Mode before Disabling MDSP 
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons 
23/07/15   hd        CR878402 Handle MPH_SET_PRIORITY_IND for GPRS proc and CONN prc to change priority properly
23/07/15   fan/akh   CR865360 Use correct TRM reservation period when doing wakeup cancelation
23/07/15   akh       CR876576 Avoid TRM adjustment to include warmup when already included
22/04/15   zc        CR814811 Move ASDIV state to inactive if G releases/reserves TRM during acq
15/07/15   fan/akh   CR857332 FR19329: WMGR support for rescheduling non-PCH activities
26/06/15   dg        CR862078 Ensure Gl1 doesn't go to sleep when it is going to initialize G2X IRAT
29/01/15   dg        CR788555 Clear FM entry for TDS ACQ on receiving TDSCDMA_STOP_IRAT_CNF
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
06/07/15   sp        CR862778: Reduce F3 logging in Idle as its impacting QTA timeline
24/06/15   ap        CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR 
03/07/15   pa        CR865455: Don't clear grm frequency info during Single SIM initialisation.  
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
16/06/15   og        CR854535. Remove redundant message router queue.
11/06/15   zf        CR847878: Using PRE_SRCH_REQ and CNF for G2W
10/06/15   sjw       CR850737: variable connection priority for TRM-Unification
09/06/15   pa        CR846421: Partial grant, release chains outside the scope of callback context.  
09/06/15   js        CR832817: LTE API for L2G QBTA, removing changes done in 785901
02/06/15   pg        CR846507 Allow non-signaling RF devices to be controlled by FTM 
04/06/15   ab        CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
21/05/15   am        CR840759 Correct trm_reservation timing from wake-up mgr
22/04/15   am        CR803948 TA in idle mode when mm_non_drx is enabled
03/06/15   ar        CR845750 Removing the as_id initialization based on gas_id since it is now derived from msgr variant
29/05/15   sv/npt    CR805550 Removed the handling of msg router message during panic reset while G2T ongoing
05/06/15   dp        CR833596 Making use of MSGR support of multi-instances of SUBs using MSGR variant    
10/04/15   am        CR508867 Panic reset in multi sim mode
11/05/15   cja       CR827065 Handle G2W initialisation requested when G2W shutdown in progress
08/05/15   jk        CR834822:Wait for G2L_DEINIT_CNF  before sending MCPM GERAN STOP request
23/03/15   jj        CR 811970 Mutex dead lock l1_sci_tick_wcdma 
                        L1_send_LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ caused by merge  of CR  765859 
11/05/15   sjw       CR835799 Allow SIB_PROCEDURE inversion to return to nominal priority.
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
15/04/15   pa        CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
07/05/15   fan/akh   CR834237 FR19329 Full G wake-up cancellation + code cleanup
05/07/15   zc        CR826075 Band Specific ASDIV Enable
06/05/15   aga       CR833610 dual data space gl1_wcdma_rxlm_buf_idx and gl1_hw_g2w_rf_enter_called_last
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
29/04/15   pjr       CR830296 Remove call to l1_sc_init_femto_timer_delete
23/04/15   cjl       CR822231 Cross WTR IRAT reselection changes to GL1 exchange mechanism
21/04/15   jj        CR826375 check gas_id matches nas_id in X2G_IRAT_CM_GSM_INIT_REQ
21/04/15   sjw       CR822898: removal of deprecated ACQUISITION reasons
16/04/15   smd       CR822202 Dual space data for IRAT and QTA
16/04/15   smd       CR822202 Remove the gl1_drx_task_inactive in msg handler for L1_GRM_QTA_START_OR_STOP
16/04/15   aga       CR822202 TRM interface for QTA
07/04/15   cjl       CR811729 FR22272 - TRM Unification - GL1 changes to allow lock retention using GRM
09/04/15   dp        CR819956 Thor 2.x RF API change, G2W use MSGR
20/02/15   ab        CR773948 GL1 Support for provding Dumps and Metrics for QSH
08/04/15   pjr       CR818535 FR25951 Changes to support Single Voice Session 
31/03/15   cja       CR800650 Optimise irat state machine
23/02/15   sjv       CR799313: Using new mcfg APIs replacing efs_get for Geran
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
30/03/15   ap        CR811753 FR22272 - TRM Unification - GL1 changes to support 
                                        SysInfo priority inversion during early camping
30/03/15   nm        CR765891 Allowing TA for TDS during PS RACH
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
24/03/15   sjw       CR812407: update for new MPH_SET_PRIORITY_IND payload
16/03/15   pg        CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
19/03/15   dp        CR810858: remove non-api header file inclusion in wl1
11/03/15   zc        CR806341 Release asdiv switch on req_n_notify for G acquisition
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
11/03/15   pjr       CR806250 Fix featurisation for changes submitted under CR787199
06/03/15   cgc       CR794323 FR25799 support x8 LTE with fast idle search.
03/04/15   br        CR763774 GL1 logging improvement to ease debugging
23/02/15   js        CR785901: GERAN GL1 to support L2G QBTA 
24/02/15   pjr       CR787199 Acquire vocoder immediately after IMS releases it during srvcc handover
27/02/15   am        CR646584 WLAN coex
24/02/15   js        CR749012: GL1 changes for LTE removal
20/02/15   sjv       CR798382 Adding msg_id L1_GRM_RELEASE_IN_CALLBACK in l1_multi_sim_determine_gas_id
17/02/15   sjv       CR796616 Moving g2l_rf_meas_exit handling as part of G2L Abort inside l1_sc_lte_process_timed_search_results function
04/02/14   nk        CR767734 Allowing TA during 2-phase access
20/02/15   js        CR693771: W+T removal. Resolving compilation errors
02/19/15   zc        CR790902: Initialize Asdiv Acq state only when TRM is granted
11/02/15   sjv       CR791268 G2L Abort support in case ML1 aborts
09/02/15   sjv       CR793538 On Panic Reset GL1 will wait for G2L to complete
03/02/15   zf        CR780458: Adding L1 task grm_release handling for callback
18/12/14   cgc       CR766576 FR23109 Adaptive Power monitor during CS in DSDA G + TDD ( TDS/LTE TDD)
17/02/15   br        CR778910 Clear LTE ID search frames on reciving G2L deinit cnf only if lte_update_list_irat_active is FALSE
12/02/15   cja       CR794676 Only set tx band iss to GFW false if RF set Tx band is sent to RF
04/02/15   dp        CR791452 Move crit sect init function to a safe place (crashes in G+T)
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs.
27/01/15   pa        CR786694: Initialise RF device to invalid value on startup.
21/01/15   jj        CR 784462 remove FEATURE_THOR_G2X_MSGR_REWORK
12/01/15   sjv       CR756154 Enhance Band Registration for Activities
19/12/15   mc        CR769958 : Removing Deprecated TRM APIs in GERAN
10/12/14   dg        CR765859 Do not go to sleep if MCPM is not updated for G2X MEAS STOP activity
05/01/14   npt       CR770381 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled
19/12/14   hd        CR773558 Typecast L1_GRM_ABORT_CALLBACK properly in the function l1_multi_sim_determine_gas_id()
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
11/12/14   ws        CR769901 Compile warning cleanup for Tabasco modem
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
10/12/14   ws        CR768376 Q6 compile warning fixes
22/10/14   mko       CR744202 In L1_MULTI_SIM_PANIC_RESET msg handling,after FW load wait for next frame tick before calling multi_sim_panic_reset()
25/11/14   jk        CR759927:Wait for receiving TDSCDMA_IRAT_STOP_CNF before calling gl1_hw_panic_inact in l1_do_panic _reset
04/12/14   sjv       CR766784 Backing out CR759568
03/12/14   npt       CR759467 Fix compiler warnings
03/12/14   cgc       CR715394 G2L code Idle/Dedicated rework FEATURE_THOR_G2L_REWORK
02/12/14   pg        CR739607 Remove duplicate initialisation of gl1_hw_qta_crit_sec
29/05/14   mko       CR639716 GCF TC 21.1 failure, Limit the changes of CR592893 to idle mode only
25/11/14   zc/zf/sj  CR759568 Delay TRM release if exiting QBTA
17/11/14   dp        CR700052 Move g2w drivers into W filetree
28/10/14   cgc       CR745548 GL1 add IS_FRAME_NUM_LATER, gl1_drx_require_and_wait_for_next_tick() conditional on awake
13/11/14   npt       CR746180 Remove KW errors
01/11/14   df        CR748602 Rework and enable adaptive timeline
04/11/14   zc        CR748323: On TRM release, change asdiv mode for GBTA enabled case
07/11/14   jj        CR 752569 Fix thor bring up issues
17/10/14   jj        CR741205 moving  G2X CNF from l1 task to MSGR
29/10/14   ab        CR745429 Featurization correction for PCHXFER for SS / CR merges.
17/10/14   nm        CR741161 Enable feature PCH in transfer.
28/10/14   sjv       CR747201 Handle multiple calls for grm_reserve from task message queue (made from l1_fm_grm_reserve)
22/10/14   ws        CR744361 Data+MMS reduce MSC if commanded by CFCM
20/10/14   cah       CR742923: L1 not able to exit panic reset due to L1 grant status not being set on TRM grant
11/08/14   dg        CR707297 Rework on CR706001 fix to issue GERAN_STOP to MCPM only when
                     GL1 is in inactive mode and deep_sleep is pending
07/08/14   xz        CR706001 (update CR700617) Don't stop GSM in MCPM when G2T stop_cnf has not feedback yet.
14/10/14   cgc       CR738236 G2L idle measurement finishes early, add GTOW_SRCH_STATE_LTE_CLEANUP_WAIT to prevent sleep.
26/09/14   zf        CR732436: Delay starting ACQ when RF wakes up late
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build
30/09/14   pa        CR732428: Mutex lock grm api to prevent ISR and Task to write grm_info at the same time.
18/12/13   ka        CR580134 RLF optimizations for CMCC for SGLTE (FR18608)
20/05/14   dg        CR613013 Long Term fix on GL1 for TS 34.123 & TC 8.3.11.12 GCF Testcase failure due to RACH failure on GSM cell
22/09/14   ap        CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
17/09/14   pa        CR725833: Prevent ISR and L1 to access grm_send_band_release_to_trm at the same time.
12/09/14   fan       CR636420: Run Sleep commit after GSTMR ISR at wakeup
08/09/14   cja       CR715411 Add support of MPH_DEEP_SLEEP_IND
06/08/14   am        CR704022 Don't reserve TRM unconditionaly on DL_TBF_CONFIG
29/08/14   cah       CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
22/08/14   cgc       CR713634 obsoleted G2L code and featurisations FEATURE_GSM_G2L_TIMED_SRCH and FEATURE_CM_G2L removed
14/08/14   cja       CR709452 Race condition between QTA cleanup and QTA fw rf init.
08/08/14   zf        CR706869: Correction of active task count update in Async interrupt handling
11/06/14   jj        CR 674199 get TRM lock before trm_asd_set_mode_and_config
14/08/14   mko       CR681323: Disable the ciphering in panic reset state
11/08/14   sjw       CR706459: use L1_COMPLETE_DS_ABORT for race condition between abort and release
23/04/14   jj        CR 653796 function  cannot called inside ASSERT
12/08/14   cah       CR707445 PTM mode overlap between PM meas periods causing transaction id corruption
11/08/14   fan       CR703945 Adaptive warm up timeline
08/08/14   ka        CR706866 FR18739: DR-DSDS RxD Idle, part 2 - new TRM clients
06/08/14   ka        CR699964 Implement Idle RXD support in GL1
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
04/08/14   sp        CR703980: Fix Compile error on disabling feature
01/08/14   npt       CR688084 Release VFR if panic reset is triggered during voice call
31/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
28/07/14   zf        CR700730: Removing RXLM buf idx in set GFW app mode
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
27/07/14   ap        CR678067: Race condition between APP mode as ACTIVE and Device mode change as DSDA leads to crash
09/07/14   am        CR691142 Update new multisim mode when all subs are inactive
11/07/14   dp        CR674070 - Use new G2W drivers interface (phase 1)
10/07/14   js        CR593074- ROTA/QBTA Feature implementation
19/05/14   ss        CR668474 X2G Init via task : Seperate message header for each tech
12/06/14   npt       CR630396 Wake-up error recovery mechanism
17/06/14   cgc       CR680138 external service of LTE msg's expose gl1_handle_message_router_msgs()
10/07/14   cws       CR690009 Each sub check its own TRM lock only and add check before updating grm_info
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
02/07/14   aga       CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
02/07/14   sp        CR688527: Fix linker error when feature_sglte disabled
20/06/14   ws        Added support for grm_get_data_sub_priority() and grm_set_data_sub_priority()
30/05/14   ws        CR657674 Data + MMS Phase 1
25/06/14   cs        Remove FEATURE_ASYMMETRIC_GERAN_BANDS
25/06/14   cs        Mainline FEATURE_GSM_DIME_TRM
24/06/14   df        CR672016: Improve function parameter checking to avoid possible corruption
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
16/06/14   sjw       CR680636 Handle new PH_RANDOM_ACCESS_IND with dummy ready-to-send
06/06/14   cja       CR676197 Prevent timed G2L getting stuck in GTOW_SRCH_STATE_LTE_CLEANUP
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS: Scell measurement changes
12/05/14   cgc       CR662650 fix update for CR659270
12/05/14   cja       CR662836 Add support for qta_cleanup_pending to Bolt.
12/05/14   cgc       CR662650 fix update for CR659270
12/05/14   zc        CR660275: Do not set asdiv wakeup config unless granted TRM
12/05/14   ssh       CR661514 Additional protection around panic reset handling [workaround for buffer overflow]
08/05/14   sp        CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
06/05/14   cgc       CR659270 mode change request causes race condition between gl1_TDS_IRAT_INIT_CNF_pending and L1_send_TDS_IRAT_STOP_CMD
01/05/14   ap        CR657068 Ensure app0 first when switching DSDA to DSDS when both ACTIVE
25/04/14   ap        CR650286 Handle DEVICE_MODE_INDICATION whilst two G stacks active.
24/04/14   pg        CR655101 Compilation error fixes due to CR644441
16/04/14   smd       CR650539:Added sending MCS_CXM_STX_SET_POWER_IND back.
16/04/14   cgc       CR641709 Check TDSCDMA_IRAT_INIT_CNF response for failure and reset TDS irat state
14/04/14    ss       CR646261 FR GSM CM INIT via message in GL1 task context
11/04/14   cja       CR644441 Bolt: Preload script for QTA
07/04/14   cja       CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
04/04/14   pg        CR643846 RFLM/FED architecture support for multi-SIM
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
28/03/14   zc        CR640583: QTA ASDIV sleep/wakeup transition fix
09/04/14   ap        CR647283: Set fw loaded flag when switching from SS -> DSDA
09/04/14   ap        CR645276: Device Mode not changing from SS<>DSDA causing GFW Crash
07/04/14   smd       CR644634:Block GSTMR ISR handler running in the middle if releaseing trm lock.
31/03/14   ws        CR641076 Ensure GL1 sub is awake for handling MPH_DEVICE_MODE_IND
28/03/14   sjw       CR637346: Handle new_procedure flag in MPH_SET_PRIORITY_IND
28/03/14   sjw       CR637346: Handle new_procedure flag in MPH_SET_PRIORITY_IND
27/03/14   cja       CR638302 For G2L timed search in idle ensure state is NULL after search.
19/03/14   cja       CR632037 Use RF Task for X2G
18/03/14   ap        CR629844 SGLTE BPLMN support when G1 in PS call in G+G - PS Hopping
17/03/14   jk        CR629569:Incorrect gain values used in DTM
18/03/14   ap        CR614949 FR 19574: Performing LTE/TDS measurements while GERAN is in connected mode using the second WTR
14/03/14   am        CR629668 Update trm_status in single sim mode also
09/03/14   sjw       CR628082: Separate handler function for MPH_DEVICE_MODE_IND
07/03/14   og        CR628402. mutex protect the mdsp async intf commands.
14/01/14   og        CR589868. T+G DSDA support.
13/02/14   ap        CR612587: Ensure call RF exit GSM mode when releasing TRM in DSDA mode
02/10/13   smd       CR548408: remove sending MCS_CXM_STX messages.
11/02/14   ap        CR612459: Deactivation and sync rx occurring simultaneously resulting in GFW crash
10/02/14   cja       CR612218 For G2L add meas enter and meas exit for LTE RF
05/02/14   br        CR576666 Prevent the denit if we have already sent the G2L INIT
07/02/14   nk        CR576899 GBTA feature is not working during DSDA to DSDS fallback
24/01/14  cah        CR598552: enable sleep for DSDS GSTMR+VSTMR
17/01/14   cgc       CR600002 propagate missing code from CR504088, support for SGLTE
06/01/14   nm        CR574905 Set the FW load status correctly when Reserve At is in progress
10/12/13   sk        CR583133 Ensure sleep ready async int doesnt get processed when decode is not done by FW
07/01/14   sjw       CR596709: created new grm_release_or_exchange() function
14/12/13   sjw       CR589502: Install different unlock callback for change of acq priority
12/12/13   sjw       CR586687 Inter-RAT exchange support during TRM release
04/12/13   js        CR576915 GL1 needs to check if GSTMR is initialized before invoking gstmr_rd_qsymbol_count() - issue detected after panic reset
03/12/13   sk        CR584247 Ensure correct sys mode passed to GL1 Arbitrator
12/11/13   nm        CR571396 Reset GPRS channels & FT handlers during panic reset
29/11/13   js        CR569912 Avoiding new FW load msg, when FW load is already in progress
26/11/13   dv        CR580909 Ensure of a 'empty' QTA gap the pending cleanup flag is set
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
19/11/13   ssh/ip    CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   nm        CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
12/11/13   cs        CR569543 Avoid sleep if holding lock for pending activity
12/11/13   sai       CR 571606: Multispace G2W abort tracking variable,ensure G2W TA abort happens
                     only in W + G mode.
11/11/13   sk        CR573517 Ensure correct sys mode used by GL1 Arb while giving coex priority
08/11/13   ip        CR 547955 - Use multi threads on wake from sleep
31/10/13   sai       CR567424: Enhance G2W TA suspension mechanism.
25/10/13   jj        CR 566934   MSG_GERAN_HIGH_3_G replaced with  MSG_HIGH
05/12/13   cs        CR584551 Use correct gas_id for GPS Timetagging in SS mode
03/12/13   cja       CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
12/02/13   sk        CR573517 Ensure correct sys mode used by GL1 Arb while giving coex priority
28/11/13   sjw       CR578873: Priority Inversion Management for Multi-SIM (PIMMS)
12/11/13   cs        CR569543 Avoid sleep if holding lock for pending activity
12/02/13   sai       CR 571606: Multispace G2W abort tracking variable,ensure G2W TA abort happens
                     only in W + G mode.
22/11/13   cja       CR455527 Remove featurisation from files in API directory
31/10/13   sai       CR567424: Enhance G2W TA suspension mechanism.
14/10/13   pa        CR536820:Added GL1 Msgr IF Task.
01/10/13   cja       CR552831 Add mutex for logging handler timestamps
05/11/13   us        CR569912 Avoiding new FW load msg, when FW load is already in progress
31/10/13   cgc       CR569801  Add wrapper functions gl1_cm_mutex_lock() gl1_cm_mutex_unlock()
26/11/12   jj        CR 424199 Adding Scell measurement for tx-diversity when BA list is empty
02/11/12   jj        CR 402579 Support of Tx Diversity in GERAN.
05/10/13   sk        CR554665 Backout the power opt temperorily(CR537690)
23/08/13   ws        CR533027 data space DAL timetick handle
05/10/13   sk        CR554456 Ensure the MCS_CXM_WWCOEX_STATE_UPDATE_IND is not processed when the SUB is inactive
01/10/13   cja       CR552831 Add mutex for logging handler timestamps
23/08/13   ws        CR 531414 Implemented critical section for Cipher clock on/off
17/09/13   pjr       CR538893 Changes to enable CXM logging
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
09/10/13   ka        CR556038 Message queue size incorrect causing message queue overflow
27/09/13   cgc       CR523552  add support for FR 16939 L+DSDS LTA/QTA
25/09/13   aga       CR547585 - G2W - avoid trm_gta_preemption_callback processing while suspended / suspend moved in ISR
24/09/13   zf        CR537690: Reduce G wakeup timeline for MS mode
08/05/13   jj        CR  522740 Do not check ready for sleep while panic reset if gfw is inactive
09/09/13   ab        CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
13/08/13   pjr       CR525797: Block trm_reserve_at when in inactive mode
14/09/13   sp        CR543645  Set FW_APP mode to Idle state on trm_release.
14/06/13   sp        CR500736: Correct handling of MPH_L1_DS_ABORT_IND for abort types other than RR_DS_ABORT_DEADISR
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
14/09/13   ap/aga    CR545423 - G2W Tune Away Feature
02/09/13   sp        CR500736: [Propagation] Correct handling of MPH_L1_DS_ABORT_IND for abort types other than RR_DS_ABORT_DEADISR
13/08/13   pjr       CR521654 added critical section to protect gsm_l1_common_rfa_task_msgr_client
13/08/13   zf        CR528482: Don't go to sleep with pending panic reset.
21/08/13   cja       CR531211 Prevent mutex deadlock between GL1 ISR Lock and g2w srch drv lock
21/08/13   npt       CR531633 - Remove FEATURE_DIME_MODEM dependencies
04/09/13   cs        CR532298 Integrate all wakeup-mgr changes for Multi-SI
15/08/13   sk        CR524039 GBTA bringup changes
08/05/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
15/07/13   sp        CR508123: Update panic reset to use multi-sim data space
01/08/13   sk        CR521009 Ensure the global flag rr_send_stop_gsm_mode_req is data spaced
29/07/13   sk        CR519663 Partial QBTA bringup changes
25/06/13   sai       CR503864: Fix MCPM-WMGR mutex deadlock when W and G go to sleep at the same time.
25/06/13   cs        Major Triton TSTS Syncup
17/06/13   sai       CR494015: Implemented GERAN wake up cancellation
13/06/13   sai       CR495383: Add support for therm read in DSDS mode.
05/06/13   sai       CR 491810: Enhance empty QTA gap support.
16/05/13   ab        CR487396 - Support EGPRS IR Reduced Window in GFW
02/06/13   sai       CR 491810: Added support to handle empty QTA gap.
24/05/13   sai       CR 468127: Retain TRM lock if next activity is to execute within 4 frames.
08/03/13   pg        CR 447563 - Ensure SAIC is re-enabled after a GTA gap
22/05/13   am        CR489198 Adding missed MAC signals in l1_dual_sim_determine_gas_id
22/05/13   am        CR461064 Inactivate drx task while ignoring the reserve_at to retain the TRM lock
18/05/13   am        CR414248 Clear all active signals in task when do panic reset
18/02/13   ab        CR 453181 : Do not reset packet Transfer state variables id in DSDx mode
17/05/13   cja       CR486815 Mutex lock for W neighbour inactive
12/06/13   cs        TSTS Updates
16/05/13   og        CR463282. Don't process the FW RF LOAD message when the gas id is inactive.
16/05/13   sai       CR488588: Program RF events as part of QTA cleanup at TRM release, if this happens
                     inside QTA gap.
13/05/13   jk        CR463435:Using DAL interface to calculate GSM sleep time tag
09/05/13   cja       CR481603 Do not shutdown G2L on DEINIT conf if INIT conf already sent.
03/05/13   mc        CR481964 Ensure gl1_hw_multi_sim_deact() is called when a GTA GAP is closed
                     with TRM_RELEASE
02/05/13   pjr       CR482754 call gl1_hw_efs_read_debug during initialisation
29/04/13   ws        CR481359 enable GTA support
25/04/13   ws        CR478944 Pass in correct gas_id to gpl1_gprs_serving_cell_meas_if()
14/04/13   pjr       CR475908 dual data spaced firmware queue
13/04/13   mc        CR471759 DSDS: QTA Support : Initial code changes & Triton Bring-Up
12/04/13   zf        CR408126: Remove double task inactive in FW LOAD.
06/04/13   sv        CR462075: Corrected the logic when ignoring GRR commands in GTW PLMN cases.
14/03/13   cja       CR458005 Turn on W clock for G2W BPLMN
25/03/13   mc        CR 464496 : COEX Support - GERAN arbitrator SW CXM integration
18/03/13   cja       CR464426 Relinquish W/T nbrs after handover complete so W/T STMR stays on
08/03/13   cja       CR461030 Add RF CM Enter and Exit
25/02/13   ap        CR 450915 DSDS: Not decoding PCH as per DRX
21/02/13   ap        CR 454529 : XCCH decode failures in GTA gaps leading to all MT call failures.
18/02/13   ab        CR 453181 : AYSYNC SIM should not reset SYNC SIM Transfer state variables.
10/02/13   ip        CR448394
06/02/13   cs        DSDS CR438977: merged support for CBCH in DSDS mode
30/01/13   ip        DSDS CR443746 - missing call task inactive
18/01/13   cgc       CR441483 during panic reset add gl1_anystop_commands_required() for mdsp
23/01/13   mc        DSDS CR332154 Ensure that a PCH following a BCCH gets decoded upon a missed BCCH block
20/12/12   ab        CR 434798 L1 should not perform TRM_RELEASE when TRM is retained for access.
13/09/12   ws        CR 399455 Fix compile errors when FEATURE_LTE_TO_GSM is disabled
28/02/13   pjr       CR457802 Moved initialiasation of gl1_msg_multi_sim_mode to gsm_l1_fn_init
14/02/13   pjr       CR452272 added support to set multisim mode at activation time
30/01/13   dg        CR441613 On panic reset , set all the pending RRBP messages as invalid
17/01/13   ws        Now send RF driver UMID Sets based on gas id rather than device
16/01/13   ws        Added RFA SUB2 UMID's
07/01/12   tjw       refactor l1_handle_message() if else chain -> switch(),
                     separate out l1_task_state_machine()
05/12/12   pg        CR427361: GFW/RF shared memory interface cleanup
28/11/12   br        CR383165 compiler warning fix
26/11/12   pg        CR423852: G2W startup can be scheduled in next frame on Dime modem
28/08/12   jj        CR388499  clear the flag rr_send_stop_gsm_mode_req before panic reset
02/10/12   jj        CR404844 bringdown the WFW after panic reset is triggered
22/11/12   cja       CR414098 W clock changes for Dime
21/11/12   jj        CR367081 Ignoring RR messages once received MPH_STOP_GSM_MODE_REQ
                     until next state change
21/11/12   og        CR399652. Call require next tick when processing the panic reset in idle mode.
09/11/12   pg        CR416728: Updates for WG IRAT on Dime modem
31/05/12   ab        CR 415136 RX and TX Time Power averaging update using msg router intf.
18/07/12   og        Merge the fix for CR369385.
09/07/12   pa        CR377132: Don't increment tds_state on receiving TDS_IRAT_INIT_CNF.
03/07/12   pa        CR375720: Don't delay TBF release in the idle frame.
13/09/12   ws        CR 399455 Fix compile errors when FEATURE_LTE_TO_GSM is disabled
08/08/12   pg        Add critical section support for GSM frame number mutex
25/07/12   ky        Moving the message router initialisation for blocking messages to
                     l1 task initialisations
03/07/12   ky        Async message router sleep, app mode, enter mode interface changes
25/06/12   cja       CR372395 Clear cipher enabled flag on panic reset
22/06/12   pa        CR371580: Delay TBF release during G2T activity.
22/06/12   pa        CR372464: Disable tds_irat_active after stopping IRAT. Handling of Abort
                     on receiving modified WCDMA_UPDATE_LIST.
20/06/12   ky        Added support of using new CCS interface and passing new CCS
                     buffer pointers in RF task messages
20/06/12   cja       CR370195 Force G2W shutdown for panic reset
12/06/12   pg        AEEstd.h deprecated for Dime modem
31/05/12   ab        CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
25/05/12   pg        Add support for RF Task
19/06/12   cs        CR370453 Use the correct wait sig for the TRM/TCXO Manager timeouts
29/05/12   cja       CR359280 Turn off W/L/T clocks after WFW/LFW/TDFW disable
18/05/12   ky        Async message router Interface Changes between SW & FW
15/05/11   jj        CR357981 Abort aquicition earlier if panic reset triggered
16/05/12   dv        CR357448 G2T Abort mechanism
19/04/12   pa        CR349239: G2T handling of idle mode IRAT measurement using FM
14/04/12   og        CR352194. Resolve G2T GSTMR ISR delay.
14/04/12   og        CR352228. T2G updates and fixes.
28/03/12   pa        CR 346062 Workaround for G2T Abort.
27/03/12   cja       CR338055 Recovery for Nikel RxFE Stuck
21/03/12   pg        FEATURE_GERAN_CRIT_SECTION_SUPPORT compilation errors
15/03/12   mm        CR 344531 G2W drivers new design
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
09/02/12   pm        removed the added reference to gl1_hw_init_sleep() which is deprecated
25/01/12   ws        removed reference to gl1_hw_init_sleep() as now deprecated
13/01/12   ws        Added missing Mutex Init call for gl1_voc_mtx
11/01/12   npt       CR315272 - Remove usage of deprecated legacy sleep APIs
16/12/11   cja       CR322557 In panic reset wait for WFW shutdown
16/12/11   ws        Added missing LTE message router routines for GSM to LTE feature
16/12/11   ws        Free floating API change - provide API for NAS GSVOICE to copy vocoder
                     data from/to GL1 to MVS structures
16/11/11   og        CR318773. Ignore the watchdog ISR timeout when GSM L1 is inactive.
14/11/11   cja       CR318816 Call g2X shutdown in panic reset
26/10/11   mm        CR313554 X2W_MODE_CFG_CMD is issued only after WFW ack'ed start
19/09/11   og        CR307591. Ensure deactivation and startup never overlap at
                     the task level.
09/09/11   cja       CR303806  Ensure G2X disable on suspend and inactive
01/09/11   og        Port over CR298710 to Nikel.
03/06/11   og        CR287418. Pend the G2L DEINIT REQ message on the cleanup cnf
                     when the search is active.
12/04/11   ws        Removed GP CLK code, not needed on NikeL
28/02/11   ip        CR267278 GP clock vote handled by RF drivers
23/02/11   pg        CR275480: Ensure GP_CLK is switched on before starting panic reset
                     when GSM clock is currently voted off (due to sleep)
31/08/11   nt        DSDS CR296418 - Panic reset for ISR stall getting overwritten
31/08/11   nt        DSDS CR287818 - Sanity check that the FW can be released
22/07/11   ap        CR297650 DSDS Develop the watchdog deactivate functionality for Dual Sim
29/06/11   nt        DSDS CR291280 If stalled ISR PR occurs, clear out pending RR Panic.
29/06/11   nt        DSDS CR288563 Ensure that the panic reset is always executed from task context
16/05/11   cs        DSDS CR282434 Reinstate CBCH for DSDS
20/06/11   cs        DSDS CR287999 Allow sleep to start upon completion of reserve_at
16/05/11   sjw       DSDS CR281084: recovery mechanism for stalled ISR during shutdown
16/05/11   sjw       Do not wait for ISR to fire if panic reset caused by stall
22/03/11   ap        CR279430 DSDS Timing value wraps casusing frame misalignment
03/02/11   ky        Fixed compiler error casued because of merging CR 266163
03/02/11   ky        266163:Reverted CR261247 fix. Added three frame delay before deactivating the
                     FW during the panic reset procedure to allow fw to process all the commands
                     and ready for deactivation. Also removed sleep during the inactivation procedure
04/03/11   cs        DSDS Tag the LPM req with GL1 gas_id
25/02/11   dmb       Move L1 task command queue to heap allocation. (Merged from LCU 4.x / 7x27 DSDS)
22/02/11   ip/cs     CR267278 GP clock vote handled by RF drivers
10/02/11   sjw       DSDS CR255895 - use timetick_get_safe() in ISR context
27/01/11   dmb       The following RR<->L1 messages are removed (not used):
                     MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
21/01/11   cs        CR271591 Remove NCELL power monitors signal/init powersave timers for DSDS
10/01/11   ap        DSDS CR270377 SYNC SIM is disabling L1 ISR while ASYNC SIM is still
                     active when consecutive start/stop GSM mode req
22/11/10   ky        Panic reset mechanism is changed. For all errors gl1 first
                     sees an nv item set or not. if this nv item is set then does
                     error fatal immediately other wise gl1 sends signal to grr for
                     recovery
16/09/10   pg        CR256046: Add WDOG API calls for dynamic WDOG 1.5
21/07/10   cs        CR246418 Allow disabling of SAIC F/W processing in idle
21/07/10   ems       Added support for ignoring SACCH power control
01/07/10   tjw       Lint
25/06/10   ap        CR244291 - Prevent frequency list memory freeing with FEATURE_EOOS
09/06/10   ip        CR238981 - Removed code from for all targets
08/06/10   ip        CR238981 Removed use of FEATURE_DUAL_RFR for 9k builds
19/05/10   ip        CR238981 Call Dal services once from task context
08/05/10   nt        CR221084 Introduction of FEATURE_GSM_FEMTO_SEARCH
28/04/10   nt        CR234959 Optimisation to NCELL power monitors in the idle mode
22/04/10   ab        CR: 234113 - For FEATURE_GSM_EGPRS_IR_OPT - 32 byte alignment for
                     heap address as part of requirement for DMA by FW
09/04/10   ab        CR#: 234043 L1 do re-initialize MDSP with EDGE IR MEMORY
                     start address after panic rest.
01/04/10   ws        Replaced deprecated call to task_start() with tmc_task_start()
25/02/10   ip        CR227540 Add NULL pointer check for Mem Heap alloc/dealloc
23/11/09   ip        Call DAL_Device_attach from task context
11/11/09   ws        Changed mutex name from gl1_global_mtx to gl1_local_mtx
10/11/09   ws        Merged Mutex changes from Mustang baseline
30/10/09   nf        Added l1_check_for_wtog_activity
31.07/09   ws        Renamed MUSTANG to POSEIDON for previous checkin
15/07/09   ip        CR167172 GL1 enters LPM before L1 task is run
11/07/09   ws        Added FEATURE_MODEM_HEAP around GPRS_MEM_FREE
29/05/09   ps        Zero Initialise memory relocation to Heap for selected data structure
28/05/09   cs        Updates to avoid spurious restriction update when TCXO released
29/04/09   ws        Fixed Lint Warnings for MOB environment
04/09/08   ws        Moved gsmdiag_init() and umtsdiag_init() from diag VU
28/07/98   hv        Added KxMutex support
15/11/07   ws        Corrected resuming priority for CR130973 - L1 was being set to a
                     low priority resulting in L1 queue becomming full
02/11/07   ws        CR130973 Lower L1 priority to 1 below GPS CC task while wait handler
                     is waiting signal mask from mdsp,TRM,tcxo mgr or NV
12/10/07   cs        CR129559 Add support for asserting sleep at task startup
06/06/07   ws        CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                     is called when we reselect to PBCCH cell stopping NC2 reporting
10/05/07   cs        Frequency error update changes for Aries GPS
30/04/07   ws        Changes to Feturisation for first pass GSM Only build
20/03/07   cs        Update to support restriction management
19/02/07   av        Added l1_check_gsm_state for gstmr use to resolve WDCMA Intlock issue.
02/03/07   cs        Add support for new CGPS timetag
01/02/07   og        gprs transfer log summary.
23/01/07   og        Resolve RVCT2.1 compiler warnings, merge from the
                     gpl1_hummer.10.00.43.01 branch.
05/01/07   og        Replace err_fatal with panic reset when tbf assignment connected
                     from single block 2-phase access has no valid start time.
22/11/06   og        Reverse the order of stopping SCE and setting the dedicated
                     stop main command. Resolves CR105367.
10/11/06   agv       Update for 7200A TRM and TCXO Mgr3.0
18/10/06   av        Added TRM and TCXO MGR 3.0 timer for 7600.
14/08/06   ws        Allow dedicated to be timeslot slammed to lowest received tn
                     and also to place the CS timeslot on TN 2
29/06/06   og        Change header file "mc.h" include to be "tmc.h".
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
11/02/05   nt        Added functions calls in L1_initialise_L1_data to reset
                     static gprs data and flags.
09/28/05   gfr       Move gl1_hw_sleep_cont_panic_inact to gl1_hw.c
09/27/05   gw        Changed debug messages for Panic Reset to make them less
                     alarming.
08/26/05   sv        Initialize wcdma bplmn variable.
26/08/05   og        Removal of DEBUG_TRANSFER compiler switch and associated
                     code.
08/16/05   sv        Add support for stuck in FIND_BCCH mode.
08/12/05   ws        Added initial FEATURE_GSM_DTM support
06/14/05   sv        Move ERR_FATAL to after the PANIC RESET is done.
06/02/05   og        Removal of header-header includes.
05/25/05   dp        Added call to l1_init_dedicated_data() in L1_initialise_l1_data()
04/21/05   sv        Add support for L1 "Panic Reset"
04/11/05   gfr       Added support for sleep timetag
03/11/05   sv        Mainlined FEATURE_GSM_CB
02/11/04   bm        Support for Background WCDMA PLMN Scan
08/26/04   sv        Lint cleanup
08/18/04   ws        Removed call to gpl1_log_gprs_air_interface_summary() as it
                     is currently not supported with new NPL1 interface
07/26/04   bk        Featurized call to gpl1_log_gprs_air_interface_summary()
06/30/04   hg        Added FEATURE_ERR_EXTENDED_STORE
06/22/04   sv        Added L1 NV logging mechanism on Error Fatals.
06/03/04   gfr       Made ISR watchdog msg_error instead of err_fatal for now.
05/26/04   sv        Lint Cleanup.
03/31/04   dp        Fixed compiler warnings
03/26/04   gfr       Time tagging support
03/22/04   gfr       Support for the ISR watchdog, removed L1_ACTIVITY_MONITOR.
02/26/04   gfr       Removed obselete call to gl1_hw_register_fn_access_routine()
02/25/04   gw        Lint clean up.
10/09/03   gsc       Added support for GSM Cell Broadcast under FEATURE_GSM_CB
04/09/03   ws        Added call to gpl1_log_gprs_air_interface_summary()
03/07/03   ws        Added initialization of release_pending for GPRS
05/02/03   gw        Merged in power transfer changes from branch.
04/02/03   gw        Changes to reflect modifications in monscan module.
03/10/03   gw        Removed featurization around call to gl1_drx_reset() in
                     L1_initialise_L1_data().
12/13/02   ws        Removed old host debug code for GPRS
11/18/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/13/02   plm       Renames mc_task_start() to task_start().
11/14/02   ws/gw     Added new L1_GPRS_IDLE state
11/01/02   pjr       Added serving cell measurements under feature switch
                     FEATURE_GPRS_MEAS.
10/30/02   he        Replaced l1_tcb by gsm_l1_tcb for GPRS.
                     Removed gl1_hw_init() from gsm_l1_main() for GPRS.
10/14/02   ATM       Added central get and set FN routines
10/10/02   mk        Merged GPRS release 1 phase1 code to baseline.
10/04/02   gw        Added call to sleep_enter_mode() to put sleep task in
                     GSM mode in single mode builds.
09/06/02   plm       Added #include "gl1_hw.h" to for gl1_hw_register_fn_access_routine()
                     function to eliminate compiler warning.
08/12/02   gw        Changed reference to MAXNO_ARFCNS to RR_MAX_PSCAN_FREQ_SCAN.
06/27/02   gw        Added call to l1_init_idle_data in L1_initialise_L1_data.
07/12/02   jc        Renamed some l1_xxx things to gsm_l1_xxx to resolve dual mode issues.
06/13/02   mk        Declared l1_tsk_buffer struct as volatile to suppress
                     compiler optimizations.
06/10/02   gw        Removed calls to gsleep_xxx() functions.  These functions
                     are now accessed though the DRX manager and the msg and
                     hw layers.  Added call to initialize DRX manager.
05/30/02   pjr/mk    Added PL1 GPRS code hooks under switch FEATURE_GSM_GPRS_L1.
05/30/02   dp        Declared l1_tskisr_blk as volatile to suppress compiler
                     optimizations
05/16/02   mk        Added L1 ISR activity monitor using T100 timer callback.
05/10/02   mk        Added dual mode Cheetah build support.
05/10/02   tb        Added FEATURE_MULTIMODE_GSM support.
03/30/02   AT        Fixed merge problem.
03/28/02   ATM       Merge GS_GSM and GS functions together
02/27/02   JC        Replaced gl1_hw_gsm_init() with gl1_hw_init().
02/21/02   dp        Added back call to gl1_hw_register_fn_access_routine
                     which was left out in gm1005
02/21/02   gw        Added support for sleep, featurized by GSM_SLEEP.
02/12/02   thh       Give srand() an initial seed.  Srand() will be seeded
                     again with FN.
                     Updated copyright date.
02/07/02   gw        Added #include of l1_drx.h.
01/22/02   JC        Added l1_wait() to support gl1_hw_gsm_init() interface
                     changes. Also, moved gl1_hw_gsm_init() call till after
                     TASK_START_SIG received. This insures NV task started.
01/24/02   ATM       Added calls to l1_change_state() for logging
12/21/01   gw        Changes to support DRX scheduling.
12/17/01   AT        Defined l1_tsk_buffer locally instead of in gs_gsm module.
12/10/01   thh       Hw_*() functions are obsolete.  Use the new interface
                     gl1_hw*() functions.
08/31/01   JC        Added this header block and removed compile switches.
                     Removed legacy code and added some error messages.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#include "geran_nv.h"

/* Alphabetical Order */
#include "comdef.h"
#include "dog.h"

#include "geran_tasks.h"
#include "gtmrs_g.h"

#include "gprs_mem.h"
#include "l1.h"

#include "l1_drx.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "rcinit.h"

#include "ms.h"
#include "geran_msgs.h"
#include "sys_stru.h"
#include "l1_log.h"      /* for l1_log_init() */
#include "l1_utils.h"    /* for l1_state_change and gl1_get_FN() */
#include "l1_smscb.h"
#include "l1i.h"
#include "gl1_msg_g.h"

#include "l1_os.h"
#include "fs_lib.h"

#include "gl1_hw.h"
#include "gl1_hwi.h"

#include "memory.h" // for memset()
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"

#include "gpl1_gprs_task.h"
#include "gpl1_gprs_log.h"

#ifdef PL1_FW_SIM
/* Include files required with Firmware Simulator */
#include "ghw_sim_defs.h"
#include <string.h>
#endif

#include "gl1_msg_pdch.h"


#include "gl1_hw_tcxo_mgr3.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"

#include "gpl1_grm_intf.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gpl1_dual_sim.h"
#include "wmgr.h"
#include "l1_fm.h"
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#include "gl1_hw_g.h"
#include "l1_sc.h"
#include "gl1_mutex.h"
#include "msgr.h"
#include "msgr_rex.h"
#include "msgr_geran.h"
#include "lte_cphy_irat_meas_msg.h"

#ifdef FEATURE_GSM_TDS
#include "tdsiratif.h"
#endif

#include "rfgsm_msg.h"
#include "mcpm_api.h"

#include "gl1_msg_wcdma_meas.h"
#include "gl1_hw_clk_ctl_g.h"

#include "mdsp_async_intf.h"
#include "mdsp_intf_g.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include "cfcm.h"

#include "l1_sc_irat.h"
#include "l1_sc_lte_irat.h"

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
#include "gl1_msgrif_init.h"
#endif

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP) || defined(FEATURE_QSH_MDUMP)

#include "l1_log_qsh.h"
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
extern rex_tcb_type gsm_l1_tcb_2;

#if defined ( FEATURE_TRIPLE_SIM )
extern rex_tcb_type gsm_l1_tcb_3;
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
#define L1_ISR_WATCHDOG_TIMEOUT  10000  /* ms = 10 seconds */

#define GL1_W_SHUTDOWN_DELAY_LOOP   50
#define GL1_QUEUE_SIZE_OVERHEAD     (7)

#if defined ( FEATURE_DUAL_ACTIVE ) || defined ( FEATURE_DUAL_SIM ) \
    || defined ( FEATURE_TRIPLE_SIM )
#define GL1_MAX_RF_TASKS NUM_GERAN_DATA_SPACES
#else
#define GL1_MAX_RF_TASKS 1
#endif /* FEATURE_DUAL_ACTIVE | FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/* for DSDS, GL1 needs to cope with twice as many L1->L1 messages, */
/* AND twice as many RR->L1 messages, making a factor of 4 overall */
#define GSM_L1_DEFAULT_QUEUE_SIZE  (((GS_DEFAULT_QUEUE_SIZE)*(NUM_GERAN_DATA_SPACES << 2))+GL1_QUEUE_SIZE_OVERHEAD)
#else
#define GSM_L1_DEFAULT_QUEUE_SIZE  (((GS_DEFAULT_QUEUE_SIZE)*(1))+GL1_QUEUE_SIZE_OVERHEAD)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

const geran_task_id_t gl1_tasks_id[NUM_GERAN_DATA_SPACES] =
  { INITIAL_VALUE_DIFFERENT(GERAN_TASK_ID_GL1, GERAN_TASK_ID_GL1_2, GERAN_TASK_ID_GL1_3) };

/* Used for internally storing packet transfer data */
transfer_data_T           l1_transfer_data[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_DTM
/* DTM specific data */
dtm_data_T                gpl1_dtm_data[NUM_GERAN_DATA_SPACES];
#endif

/* used for buffering received PACCH messages from MAC */
mac_ul_ctrl_buffer_T       gpl1_gprs_mac_ul_buffer[NUM_GERAN_DATA_SPACES];

#if defined(FEATURE_QBTA)
#define QBTA_MASK 0x04
uint8 gl1_wcdma_qta_rota_nv = 0x00 ;
#endif

volatile ISRTIM_CMD_BLK   l1_tsk_buffer[NUM_GERAN_DATA_SPACES];
dedicated_data_T          l1_dedicated_data[NUM_GERAN_DATA_SPACES];
l1_dcch_buffer_T          l1_dcch_s0_buffer[NUM_GERAN_DATA_SPACES];
l1_sacch_buffer_T         l1_sacch_s0_buffer[NUM_GERAN_DATA_SPACES];
sys_vfr_e_type            gl1_stored_vfr_map[NUM_GERAN_DATA_SPACES];

#if defined  (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS)                              // To look at ----------
geran_gl1_msgr_t          gsm_l1_msgr[ GL1_MSGR_QUEUE_SIZE ];                               // To look at ----------
#endif                                                                                      // To look at ----------
geran_rf_task_msgr_t      gsm_l1_rf_task_msgr[GL1_MAX_RF_TASKS][GL1_RF_TASK_MSGR_QUEUE_SIZE];
#if defined (FEATURE_GSM_COEX_SW_CXM)
geran_cxm_msgr_t      gsm_l1_cxm_msgr[NUM_GERAN_DATA_SPACES][GL1_CXM_MSGR_QUEUE_SIZE];
#endif

#define NUM_GERAN_PRIORITY_PROCEDURES ((GERAN_GPRS_PROCEDURE)+1)
typedef struct
{
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
  l1_wtog_bplmn_struct_T l1_wtog_bplmn_task;
#endif
  uint32 prev_fn;
  /* one system procedure type is stored for each priority-based GRR procedure type */
  sys_proc_type_e_type l1_sys_proc_type[NUM_GERAN_PRIORITY_PROCEDURES];
} l1_task_data_t;


static l1_task_data_t l1_task_data[NUM_GERAN_DATA_SPACES];
idle_data_T           l1_idle_data_store[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_QSC_TX_DIV
antenna_switch_UL_DL_delta_table_T asdiv_UL_DL_delta_table_store = {0};
uint16 gl1_antenna_switch_control_mask = 0;
#endif

/**
 *  CR346062 : Workaround to avoid T IRAT abort.
 *  Should be deleted after abort feature available.
 */
#ifdef FEATURE_GSM_TDS
int                    l1_g2t_delay_tbf_rel = 0xff;            // To look at ----------
boolean                l1_g2t_issue_tbf_rel = FALSE;           // To look at ----------
#endif

q_type l1_cmd_q[NUM_GERAN_DATA_SPACES];

gs_queue_id_T l1_queues[NUM_GERAN_DATA_SPACES] =
          { INITIAL_VALUE_DIFFERENT(GS_QUEUE_LAYER_1,
                                    GS_QUEUE_LAYER_1_2,
                                    GS_QUEUE_LAYER_1_3) };

q_type  gsm_l1_rf_task_msgr_q[GL1_MAX_RF_TASKS];
q_type  gsm_l1_rf_task_msgr_free_q[GL1_MAX_RF_TASKS];
#if defined(FEATURE_GSM_COEX_SW_CXM)
q_type  gsm_l1_cxm_msgr_q[NUM_GERAN_DATA_SPACES];
q_type  gsm_l1_cxm_msgr_free_q[NUM_GERAN_DATA_SPACES];
#endif


boolean panic_reset_occured[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
int16   tx_diversity_threshold ,tx_div_min_delta_btw_antenna ;

int16   tx_diversity_threshold ,tx_div_min_delta_btw_antenna ;

static boolean l1_isr_alive[NUM_GERAN_DATA_SPACES];
static rex_timer_type l1_isr_watchdog_timer[NUM_GERAN_DATA_SPACES];
#ifdef FEATURE_DUAL_DATA
L1_cfcm_data_struct gl1_cfcm_data = {GL1_GFCM_NULL, L1_GPRS_INVALID_FN};
#endif


/*-----------------------------------------------------------------
** Critical Section to protect GL1 shared vars.
**-----------------------------------------------------------------
*/

rex_crit_sect_type gl1_local_crit_sec[NUM_GERAN_DATA_SPACES];

rex_crit_sect_type gl1_task_crit_sec[NUM_GERAN_DATA_SPACES];

rex_crit_sect_type gl1_hw_sleep_wakeup_cancel_crit_sect[NUM_GERAN_DATA_SPACES];

rex_crit_sect_type gl1_trm_crit_sec;

rex_crit_sect_type gl1_g2x_tuneaway_crit_sec;

rex_crit_sect_type gdrivers_fn_crit_sec[NUM_GERAN_DATA_SPACES];

rex_crit_sect_type gl1_multi_sim_mode_crit_sec;
rex_crit_sect_type gl1_multi_sim_mode_crit_gbta_sec;


rex_crit_sect_type gdrivers_rfa_msgr_crit_sec[NUM_GERAN_DATA_SPACES];
rex_crit_sect_type gdrivers_cipher_crit_sec[NUM_GERAN_DATA_SPACES];

rex_crit_sect_type gstmr_log_crit_sec[NUM_GERAN_DATA_SPACES];

rex_crit_sect_type mdsp_async_intf_crit_sec[NUM_GERAN_DATA_SPACES];
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
rex_crit_sect_type gl1_rach_crit_sec[NUM_GERAN_DATA_SPACES];
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */
rex_crit_sect_type gl1_grm_info_crit_sec[NUM_GERAN_DATA_SPACES];
rex_crit_sect_type gl1_hw_qta_crit_sec[NUM_GERAN_DATA_SPACES];
rex_crit_sect_type gl1_hw_app_mode_cfg_crit_sec[NUM_GERAN_DATA_SPACES];
rex_crit_sect_type gl1_hw_sleep_warmup_cb_crit_sect[NUM_GERAN_DATA_SPACES];
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
rex_crit_sect_type gl1_hw_sleep_warmup_tmln_crit_sect[NUM_GERAN_DATA_SPACES];
#endif


rex_crit_sect_type gl1_irat_crit_sec[NUM_GERAN_DATA_SPACES];

rex_crit_sect_type gl1_dpc_queue_crit_sec[NUM_GERAN_DATA_SPACES];

/* handle used for timetick services */
DalDeviceHandle *gsmtt_handle[NUM_GERAN_DATA_SPACES];
static void gsm_l1_dal_init_for_sleep_timetag(gas_id_t gas_id);

/* Function to action the panic always from task context */
void l1_task_action_panic_reset( void *, gas_id_t gas_id);

extern void l1_init_gpl1_gprs_rrbp_data(gas_id_t gas_id);

#ifndef TEST_FRAMEWORK  /* so l1_multi_sim_determine_gas_id can be unit-tested */
#define STATIC static
#else
#error code not present
#endif

gas_id_t l1_multi_sim_determine_gas_id(IMH_T *msg_header);

extern void l1_sc_wcdma_init_gsm_only_mode(boolean gsm_only, gas_id_t gas_id);
extern void L1_multi_sim_bcch_reserve_at_priority_change( gas_id_t gas_id );

/*-----------------------------------------------------------------
** LTE & TDS related declarations
**-----------------------------------------------------------------
*/


#ifdef FEATURE_GSM_TDS
extern volatile boolean  gl1_TDS_IRAT_INIT_CNF_pending;
#endif

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
uint8 antenna_swith_tx_div_nv_enable ;
#endif

/*===========================================================================
                         FUNCTION DECLARATIONS
===========================================================================*/
void l1_handle_message( l1_queue_message_T* message );
void gl1_panic_reset_callback( gas_id_t gas_id );
void L1_initialise_L1_data( gas_id_t gas_id );
void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id );
void l1_log_reset_type( l1_panic_reset_type_T error_code, gas_id_t gas_id );
boolean l1_check_gsm_state( gas_id_t gas_id );
static void L1_initialise_L1_powersave_timers( gas_id_t gas_id );
void gpl1_gprs_handle_rlc_ph_data_req_sig(gas_id_t gas_id);
void gl1_state_log(gas_id_t gas_id);
#if defined(FEATURE_GSM_COEX_SW_CXM)
void l1_initialise_cxm_msg_router_q( gas_id_t gas_id );
void  gl1_handle_cxm_msgr_msgs( gas_id_t gas_id );
#endif
extern void gsmdiag_init (void);
extern void umtsdiag_init (void);

static panic_reset_T panic_reset_triggered_by_l1[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(DONT_PANIC)} ;

static l1_panic_reset_type_T l1_error_code[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(L1_STUCK_IN_NULL_MODE)} ;

static uint32 panic_reset_received_frame_no[NUM_GERAN_DATA_SPACES]= { INITIAL_VALUE(L1_GPRS_INVALID_FN)};


#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
void  gpl1_call_in_task_do_time_pwr_avg(gas_id_t gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

rex_tcb_type* l1_task_tcb_read(gas_id_t gas_id);

#ifdef FEATURE_GSM_COEX
static void l1_register_arbitrated_gas(boolean irat, gas_id_t gas_id);
static void l1_unregister_arbitrated_gas(boolean irat, gas_id_t gas_id);
#endif /* FEATURE_GSM_COEX */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static void gl1_handle_rach_timeout(gas_id_t gas_id);
#endif

static void gl1_handle_qta_mgsr_msgs( geran_gl1_msgr_t *cmd_ptr, gas_id_t gas_id );
static void l1_task_state_machine(IMH_T *msg_header, gas_id_t arg_gas_id);

/*===========================================================================
                          FUNCTION DEFINITIONS
===========================================================================*/

/*===========================================================================

FUNCTION  l1_log_reset_trace

DESCRIPTION
  This function will reset the L1 trace buffer for the specified GAS ID

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void l1_log_reset_trace( gas_id_t gas_id )
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    l1_NVLog_struct_T *l1_nv_log_ptr = &l1_nv_log[gas_id];

    memset(l1_nv_log_ptr, 0, sizeof(l1_NVLog_struct_T));
  }
  return;
}

/*===========================================================================

FUNCTION l1_initialise_global_data

DESCRIPTION
             Initialise all global data values

PARAMS       None

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void l1_initialise_global_data(gas_id_t gas_id)
{
  l1_log_reset_trace(gas_id);
  init_gpl1_gprs_ptcch_data(gas_id);
  init_local_idle_data(gas_id);
  init_l1_bcch_list_data(gas_id);
  init_l1_acq_data(gas_id);
  init_gpl1_gprs_serving_cell_data(gas_id);
  init_gpl1_gprs_single_data(gas_id);
  init_l1_utils_data(gas_id);
  init_l1_isr_data(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  init_l1_multi_sim(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  init_sc_wcdma_data(gas_id);
  L1_sdcch_init_data_struct(gas_id);
  L1_tch_init_data_struct(gas_id);
  l1_tch_amr_init_data(gas_id);

  l1_reset_panic_reset_received_frame_no(gas_id);
}

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
l1_wtog_bplmn_sub_state_T l1_get_wtog_bplmn_task_substate(gas_id_t gas_id)
{
  l1_task_data_t *l1_task_data_ptr = &l1_task_data[gas_id];
  return l1_task_data_ptr->l1_wtog_bplmn_task.sub_state ;
}
background_HPLMN_activity_type l1_get_wtog_bplmn_task_state(gas_id_t gas_id)
{
  l1_task_data_t *l1_task_data_ptr = &l1_task_data[gas_id];
  return l1_task_data_ptr->l1_wtog_bplmn_task.state;
}
void l1_set_wtog_bplmn_task_substate(l1_wtog_bplmn_sub_state_T sub_state, gas_id_t gas_id)
{
  l1_task_data_t *l1_task_data_ptr = &l1_task_data[gas_id];
  l1_task_data_ptr->l1_wtog_bplmn_task.sub_state=sub_state;
}
void l1_set_wtog_bplmn_task_state(background_HPLMN_activity_type state, gas_id_t gas_id)
{
  l1_task_data_t *l1_task_data_ptr = &l1_task_data[gas_id];
  l1_task_data_ptr->l1_wtog_bplmn_task.state=state;
}
#endif /* FEATURE_WTOG_BACKGROUND_PLMN_SEARCH */

/*===========================================================================

FUNCTION gl1_get_specific_sys_proc

DESCRIPTION
  Return the system procedure subreason associated with a specific RR procedure

DEPENDENCIES
  None

RETURN VALUE
  sys_proc_type_e_type system procedure subreason

SIDE EFFECTS
  None

===========================================================================*/
sys_proc_type_e_type gl1_get_specific_sys_proc(geran_procedure_t geran_procedure, gas_id_t gas_id)
{
  l1_task_data_t *l1_task_data_ptr = &l1_task_data[gas_id];
  unsigned int index = (unsigned int)geran_procedure;

  if (grm_get_resource_lock_state(gas_id))
  {
    index = (unsigned int)GERAN_LOCK_HOLD;
  }
  ASSERT ( NUM_GERAN_PRIORITY_PROCEDURES >= index );

  return l1_task_data_ptr->l1_sys_proc_type[index];
}

/*===========================================================================

FUNCTION gl1_set_specific_sys_proc

DESCRIPTION
  Update the system procedure subreason associated with a specific RR procedure

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_set_specific_sys_proc(geran_procedure_t geran_procedure, sys_proc_type_e_type subreason, gas_id_t gas_id)
{
  l1_task_data_t *l1_task_data_ptr = &l1_task_data[gas_id];
  unsigned int index = (unsigned int)geran_procedure;

  ASSERT ( NUM_GERAN_PRIORITY_PROCEDURES >= index );
  l1_task_data_ptr->l1_sys_proc_type[index] = subreason;

  return;
}

/*===========================================================================
FUNCTION  l1_get_state_for_gsm_client

DESCRIPTION
  Extracts the L1 state from the appropriate L1 task/ISR buffer

DEPENDENCIES
  None

RETURN VALUE
  Byte representing the current GSM L1 state (see l1i.h for enumeration)

SIDE EFFECTS
  Calls into TRM under Dual SIM conditions - assumes that trm_get_granted()
  does NOT perform any resource locking that could delay this function call
===========================================================================*/
static byte l1_get_state_for_gsm_client(gas_id_t gas_id)
{
  byte l1_state;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if ( gl1_msg_get_multi_sim_mode() )
   {
     trm_get_info_return_type get_grant;

     /* Multi SIM is enabled but neither client holds the TRM lock */
     /* so treat it as inactive for the purposes of this function */
     l1_state = (byte)L1_INACTIVE_MODE;

    if ( grm_get_trm_status(l1_tsk_buffer[gas_id].client_id, gas_id) == GL1_TRM_GRANTED )
       {
         l1_state = l1_tsk_buffer[gas_id].l1_state;
     }

     /* note that L1_MULTI_SIM_INACTIVE is considered as an ACTIVE state */
     /* since it is ONLY valid for the panic reset mechanism within GL1 */
  }
  else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  {
    l1_state = l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].l1_state;
  }

  return(l1_state);
}

/*===========================================================================
FUNCTION  l1_check_gsm_state

DESCRIPTION
  This function lets GSTMR know GSM is the Active RAT.

DEPENDENCIES
  None

RETURN VALUE
  boolean - TRUE if GSM is Activated, FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_check_gsm_state( gas_id_t gas_id )
{
  return(l1_get_state_for_gsm_client(gas_id) != (byte)L1_INACTIVE_MODE);
}

#if !defined (FEATURE_WCDMA_PLT) && !defined (FEATURE_GSM_PLT)
/*===========================================================================
FUNCTION  l1_check_for_wtog_activity

DESCRIPTION
  This function lets GSTMR know GSM if W to G is active.
  In dual sim the

DEPENDENCIES
  None

RETURN VALUE
  boolean - TRUE Active, FALSE otherwise.

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_check_for_wtog_activity ( gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   boolean result = FALSE;

   if((l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE) ||
      (l1_tskisr_blk->l1_state == L1_SUSPEND_MODE) ||
      (l1_tskisr_blk->l1_state == L1_INACTIVE_MODE))
   {
      result = TRUE;
   }
   return (result);
}
#endif

/*===========================================================================

FUNCTION L1_initialise_L1_powersave_timers

DESCRIPTION
  Initializes task power saving timers

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void L1_initialise_L1_powersave_timers( gas_id_t gas_id )
{
  /* only call this init routine now if static memory is to be used */
  l1_sc_init_ncell_monitor_timer(gas_id);

  /* The idle SAIC disable decision timer */
  l1_idle_saic_disable_timer_create(gas_id);
}


#ifdef FEATURE_GSM_COEX
/*===========================================================================

FUNCTION l1_register_arbitrated_gas

DESCRIPTION
  This function registers the specified GAS ID with the GL1 arbitrator

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_register_arbitrated_gas(boolean irat, gas_id_t gas_id)
{
  garb_intf_register(gl1_hw_get_arbitration(gas_id), gl1_msg_get_multi_sim_sys_mode(), irat, gas_id );
}

/*===========================================================================

FUNCTION l1_unregister_arbitrated_gas

DESCRIPTION
  This function de-registers the specified GAS ID with the GL1 arbitrator

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_unregister_arbitrated_gas(boolean irat, gas_id_t gas_id)
{
  NOTUSED(irat);

  garb_intf_unregister( gas_id );
}
#endif

/*===========================================================================

FUNCTION L1_initialise_L1_task_data

DESCRIPTION
  Initializes task/queue information

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static msgr_client_t    l1_msgr_client[NUM_GERAN_DATA_SPACES];

void L1_initialise_L1_task_data(gas_id_t gas_id)
{
  errno_enum_type  msgr_error;

#ifdef FEATURE_GSM_CFCM
  cfcm_reg_req_type_s client_req;
#endif

  /* Create a watchdog timer for watching the ISR */
  rex_def_timer(&l1_isr_watchdog_timer[gas_id], l1_task_tcb_read(gas_id), L1_ISR_WATCHDOG_TIMER_SIG);

  /* Set here as the FTM start request will set to FALSE */
  (void)gl1_set_sacch_tx_pwr_control( TRUE, gas_id );

  (void) q_init(&l1_cmd_q[gas_id]);

  (void) gs_init( l1_queues[gas_id],
                  rex_self(),
                  L1_CMD_Q_SIG,
                  &l1_cmd_q[gas_id],
                  NULL,
                  sizeof(l1_queue_message_T),
                  GSM_L1_DEFAULT_QUEUE_SIZE,
                  (gs_get_handler_ptr)l1_handle_message);

#if defined(FEATURE_GSM_COEX_SW_CXM)
  l1_initialise_cxm_msg_router_q(gas_id);
#endif

  /* create message router mailbox for receiving RFA Task CNF messages */
  msgr_error = gl1_hw_rf_init_rfa_task_msgr_mailbox(gas_id) ;

  if( msgr_error != E_SUCCESS)
  {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_0_G(" gl1_hw_rf_init_rfa_task_msgr_mailbox Failure");
  }

  ASSERT(msgr_error == E_SUCCESS);

  /* create message router mailbox for receiving GFW CNF messages */
  mdsp_async_intf_init(gas_id);

#ifdef FEATURE_GSM_COEX
 /* Install callbacks that the message layer can use to initiate
  * registration and de-registration with the GL1 arbitrator
  */
  gl1_hw_cfg_arbitration(l1_register_arbitrated_gas, l1_unregister_arbitrated_gas, gas_id);

#ifdef FEATURE_WLAN_COEX_SW_CXM
  garb_cxm_reset_coex_active_policy(gas_id);
  garb_cxm_reset_coex_metrics(gas_id);
#endif
#endif

  /* Ensure the TRM resource lock was not left in locked state */
  grm_set_resource_lock_state(FALSE,gas_id);

  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  { /* The rest of the initialisation only has to be done for one sub */
    l1_sc_init_femto_timer();

  }
}


#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION l1_initialise_cxm_msg_router_q

DESCRIPTION
  Initialises CXM->GL1 message router queue information

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static msgr_client_t    cxm_msgr_client[NUM_GERAN_DATA_SPACES];

void l1_initialise_cxm_msg_router_q( gas_id_t gas_id )
{
   errno_enum_type  msgr_error;
   msgr_id_t        msgr_id;
   uint32           i = 0;

   (void) q_init( &gsm_l1_cxm_msgr_q[gas_id] );
   (void) q_init( &gsm_l1_cxm_msgr_free_q[gas_id] );

   while( i < GL1_CXM_MSGR_QUEUE_SIZE )
   {
     msgr_hdr_struct_type*  tmp_ptr   = (msgr_hdr_struct_type *) &(gsm_l1_cxm_msgr[gas_id][i].msg.hdr);
     q_link_type*           link_item_ptr = q_link( tmp_ptr, &(gsm_l1_cxm_msgr[gas_id][i].link) );

     q_put( &gsm_l1_cxm_msgr_free_q[gas_id], link_item_ptr );

     i++;
   }

   /* Register the queue and free queue with the message router */
   msgr_error = msgr_client_create(&cxm_msgr_client[gas_id]);

   ASSERT(msgr_error == E_SUCCESS);

   msgr_error = msgr_client_add_rex_q(
     &cxm_msgr_client[gas_id],                        //!< Message Client Handle
     rex_self(),                          //!< REX TCB pointer
     GL1_CXM_MSGR_SIG,                    //!< REX TCB pointer
     &gsm_l1_cxm_msgr_q[gas_id],          //!< queue to place the msg
     &gsm_l1_cxm_msgr_free_q[gas_id],     //!< queue to get empty buffer
     offsetof(geran_cxm_msgr_t, msg),     //!< offset to msgr_hdr
     MSGR_NO_OFFSET,                      //!< offset to legacy cmd_type
     sizeof(geran_cxm_msgr_t),            //!< max size of msgs
     &msgr_id                             //!< queue identifier
   );

   ASSERT(msgr_error == E_SUCCESS);

   switch ( gas_id )
   {
     case GERAN_ACCESS_STRATUM_ID_1:
     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_FREQID_LIST_GSM1_IND
      );

      ASSERT(msgr_error == E_SUCCESS);

     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_REQUEST_ACTIVITY_INFO_GSM1_IND
      );
      ASSERT(msgr_error == E_SUCCESS);

#ifdef FEATURE_WLAN_COEX_SW_CXM
     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_COEX_METRICS_GSM1_REQ
      );
      ASSERT(msgr_error == E_SUCCESS);

     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_COEX_ACTIVE_POLICY_GSM1_IND
      );
      ASSERT(msgr_error == E_SUCCESS);
#endif

//   msgr_error = msgr_register(
//    MSGR_GERAN_GL1,
//    &cxm_msgr_client[gas_id],
//    msgr_id,
//    MCS_CXM_WWCOEX_STATE_UPDATE_IND
//    );
//    ASSERT(msgr_error == E_SUCCESS);
     break;

     case GERAN_ACCESS_STRATUM_ID_2:
     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_FREQID_LIST_GSM2_IND
      );

     ASSERT(msgr_error == E_SUCCESS);

     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_REQUEST_ACTIVITY_INFO_GSM2_IND
      );
     ASSERT(msgr_error == E_SUCCESS);

     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_WWCOEX_STATE_UPDATE_IND
      );
      ASSERT(msgr_error == E_SUCCESS);

#ifdef FEATURE_WLAN_COEX_SW_CXM
     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_COEX_METRICS_GSM2_REQ
      );
      ASSERT(msgr_error == E_SUCCESS);

     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_COEX_ACTIVE_POLICY_GSM2_IND
      );
      ASSERT(msgr_error == E_SUCCESS);
#endif

     msgr_error = msgr_register(
      MSGR_GERAN_GL1,
      &cxm_msgr_client[gas_id],
      msgr_id,
      MCS_CXM_COEX_TECH_STATE_BCAST_IND
      );
      ASSERT(msgr_error == E_SUCCESS);
     break;

#if defined ( FEATURE_TRIPLE_SIM )
     case GERAN_ACCESS_STRATUM_ID_3:
       msgr_error = msgr_register(
        MSGR_GERAN_GL1,
        &cxm_msgr_client[gas_id],
        msgr_id,
        MCS_CXM_FREQID_LIST_GSM3_IND
        );

      ASSERT(msgr_error == E_SUCCESS);

       msgr_error = msgr_register(
        MSGR_GERAN_GL1,
        &cxm_msgr_client[gas_id],
        msgr_id,
        MCS_CXM_REQUEST_ACTIVITY_INFO_GSM3_IND
        );

#ifdef FEATURE_WLAN_COEX_SW_CXM
       msgr_error = msgr_register(
        MSGR_GERAN_GL1,
        &cxm_msgr_client[gas_id],
        msgr_id,
        MCS_CXM_COEX_METRICS_GSM3_REQ
        );
        ASSERT(msgr_error == E_SUCCESS);

       msgr_error = msgr_register(
        MSGR_GERAN_GL1,
        &cxm_msgr_client[gas_id],
        msgr_id,
        MCS_CXM_COEX_ACTIVE_POLICY_GSM3_IND
        );
        ASSERT(msgr_error == E_SUCCESS);
#endif

     break;
#endif /* FEATURE_TRIPLE_SIM */

     case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
     default:
       MSG_GERAN_FATAL_0_G( "unassigned gas id" );
     break;
   }

   ASSERT(msgr_error == E_SUCCESS);
}
#endif

/*===========================================================================

FUNCTION l1_do_panic_reset_after_delay

DESCRIPTION
  This function calls panic reset mechanism via task context.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_do_panic_reset_after_delay(gas_id_t gas_id)
{
  MSG_GERAN_HIGH_0_G("SEND TO TASK l1_task_action_panic_reset");
  l1_scdrv_call_in_task( l1_task_action_panic_reset, (void *)0, gas_id);
}

/*===========================================================================

FUNCTION l1_get_panic_reset_received_frame_no

DESCRIPTION
  Returns the frame number when the panic reset was triggered

DEPENDENCIES
  None

RETURN VALUE
  Returns the frame number when the panic reset was triggered

SIDE EFFECTS
  None

===========================================================================*/
uint32 l1_get_panic_reset_received_frame_no(gas_id_t gas_id)
{
   return panic_reset_received_frame_no[gas_id];
}

/*===========================================================================

FUNCTION l1_set_panic_reset_received_frame_no

DESCRIPTION
  Sets the panic reset frame number

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_set_panic_reset_received_frame_no(gas_id_t gas_id, uint32 frame_no)
{
   /* Lets do a range check first*/
   if( (gas_id < GERAN_ACCESS_STRATUM_ID_1) || (gas_id > (NUM_GERAN_DATA_SPACES - 1)))
   {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_0_G("l1_set_panic_reset_received_frame_no: gas_id range check failed");
   }

   panic_reset_received_frame_no[gas_id] = frame_no;
   MSG_GERAN_HIGH_1_G("l1_set_panic_reset_received_frame_no - %d", panic_reset_received_frame_no[gas_id]);
}


/*===========================================================================

FUNCTION l1_reset_panic_reset_received_frame_no

DESCRIPTION
  Resets the panic reset frame number to default value

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_reset_panic_reset_received_frame_no(gas_id_t gas_id)
{
   /* Lets do a range check first*/
   if( (gas_id < GERAN_ACCESS_STRATUM_ID_1) || (gas_id > (NUM_GERAN_DATA_SPACES - 1)))
   {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_0_G("l1_reset_panic_reset_received_frame_no: gas_id range check failed");
   }

   panic_reset_received_frame_no[gas_id] = L1_GPRS_INVALID_FN;
   MSG_GERAN_HIGH_1_G("l1_reset_panic_reset_received_frame_no - %d", panic_reset_received_frame_no[gas_id]);
}


/*===========================================================================

FUNCTION l1_check_for_panic_reset

DESCRIPTION
  This function return TRUE if panic reset condition is met, FALSE otherwise

DEPENDENCIES
  None

RETURN VALUE
  This function return TRUE if panic reset condition is met, FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_check_for_panic_reset(gas_id_t gas_id)
{
   boolean ret_val = FALSE;
   if ( (l1_get_panic_reset_received_frame_no(gas_id) != L1_GPRS_INVALID_FN )
        && (panic_reset_triggered_by_l1[gas_id] != DONT_PANIC))
   {
      if ( l1_sc_check_irat_active(gas_id) || (!l1_sc_is_irat_state_null(gas_id))
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
           || gl1_hw_qta_gap_active( gas_id) || IS_GBTA_ACTIVE() 
#endif
         )
      {
        l1_set_panic_reset_received_frame_no(gas_id, gl1_get_FN( gas_id ));
        MSG_GERAN_HIGH_0_G("Defer processing panic reset while inside the TA gap");
      }
      else
      {
        MSG_GERAN_ERROR_3_G("l1_check_for_panic_reset: curr_fn - %d, panic_reset_received_frame_no - %8X, panic_reset_triggered_by_l1 - %d",
        GSTMR_GET_FN_GERAN( gas_id ), l1_get_panic_reset_received_frame_no(gas_id), panic_reset_triggered_by_l1[gas_id]);
        ret_val = TRUE;
      }
      
   }
   else if (l1_error_code[gas_id] == L1_STALLED_ISR)
   {
     ret_val = TRUE;
   }

   return ret_val;
}

/*===========================================================================

FUNCTION l1_task_action_panic_reset

DESCRIPTION
  This function actions a reset mechanism via task context

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_task_action_panic_reset( void *unused, gas_id_t gas_id)
{
  NOTUSED(unused);

  /* clear all the active signals in task */
  (void)rex_clr_sigs( l1_task_tcb_read(gas_id), rex_get_sigs( l1_task_tcb_read(gas_id) ) );

  switch ( panic_reset_triggered_by_l1[gas_id])
  {
  case RR_TRIGGERED_PANIC:
    {
      MSG_GERAN_ERROR_0_G("Recovery requested by RR.");

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( gl1_msg_get_multi_sim_mode() )
      {
        gpl1_multi_sim_panic_reset( RR_PANIC, MPH_RESET_CNF, gas_id );
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
        l1_do_panic_reset( RR_PANIC, MPH_RESET_CNF, gas_id );
      }

      /* Ensure variables are reset */
      l1_reset_rr_current_msg_id( gas_id );
    }
    break;

  case L1_TRIGGERED_PANIC_AFTER_RR:
    {
      MSG_GERAN_ERROR_0_G("Recovery requested by L1 and RR.");

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( gl1_msg_get_multi_sim_mode() )
      {
        gpl1_multi_sim_panic_reset( l1_error_code[gas_id], MPH_RESET_CNF, gas_id );
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
        l1_do_panic_reset( l1_error_code[gas_id], MPH_RESET_CNF, gas_id );
      }

      /* Ensure variables are reset */
      l1_reset_rr_current_msg_id( gas_id );
    }
    break;

  default:
    {
      MSG_GERAN_ERROR_1_G("Recovery requested by default panic_reset_triggered_by_l1:%d", panic_reset_triggered_by_l1[gas_id]);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( gl1_msg_get_multi_sim_mode() )
      {
        gpl1_multi_sim_panic_reset( l1_error_code[gas_id], MPH_RESET_IND, gas_id );
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
        l1_do_panic_reset( l1_error_code[gas_id], MPH_RESET_IND, gas_id );
      }
    }
    break;
  }
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
void l1_reset_fw_loaded_and_ready_flag(void)
{
  GOnly_mode_settings.fw_loaded_and_ready = FALSE;
}

/*===========================================================================

FUNCTION l1_handle_device_mode_ind

DESCRIPTION
  Takes appropriate actions on reception of MPH_DEVICE_MODE_IND

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  May affect RF device assignments and update firmware with device mode

===========================================================================*/
static void l1_handle_device_mode_ind(mph_device_mode_ind_T *msg_ptr, const gas_id_t gas_id)
{
#ifdef FEATURE_SGLTE_DUAL_SIM
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif

  MSG_GERAN_HIGH_3_G("Received Device Mode Indication:current: %d target Mode: %d Cause %d",
                     gl1_msg_get_multi_sim_sys_mode (),
                     (int)(msg_ptr->multi_sim_mode),
                     (int)(msg_ptr->device_mode_cause));

#ifdef FEATURE_DUAL_ACTIVE
  switch(msg_ptr->device_mode_cause)
  {
#if defined(FEATURE_SGLTE_DUAL_SIM) || defined(FEATURE_SVLTE_DUAL_SIM)
    case GERAN_DEVICE_MODE_CAUSE_PLMN:
      /* new handler for RF device control after PLMN search ended  */
      /* If PS on non multi mode device abort TBF
       * If PS waiting for non multi mode device release TRM and re-request for multi mode device
       */
       l1_handle_device_mode_cause_plmn(gas_id);


      break;
#endif /* FEATURE_SGLTE_DUAL_SIM || FEATURE_SVLTE_DUAL_SIM */
#ifdef FEATURE_SGLTE_DUAL_SIM
    case GERAN_DEVICE_MODE_CAUSE_MMCP:

      if (SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY == msg_ptr->multi_sim_mode)
      {
        if (gl1_msg_get_multi_sim_sys_mode () != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY)
        {
          gl1_set_switch_multi_sim_mode(msg_ptr->multi_sim_mode);

          if (OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
          {
            trm_get_info_return_type get_granted;

            grm_client_enum_t client_id = l1_tsk_buffer[gas_id].client_id;

            // Initialize GTA when moving to DSDS mode from DSDA
            if ( TRM_DENIAL != grm_get_granted( client_id, &get_granted, gas_id ) )
            {
              grm_retain_lock( client_id,
                               (grm_unlock_callback_t)&grm_gta_preemption_callback,
                               gas_id );

              /* Need to register an abort callback with TRM. This allows any booking by any other client to
                 invoke this callback and pass the time when the booking is required */
              MSG_GERAN_HIGH_0_G(" DS G TA ds_init_pch_sus");
            }
          }
        }
      }
      else if (SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE == msg_ptr->multi_sim_mode)
      {
        if ( gl1_msg_get_multi_sim_sys_mode () == SYS_MODEM_DEVICE_MODE_SINGLE_SIM )
        {
          /* switching from SS to DSDA need to set the fw loaded flag now
           * as there are checks for this in DSDA mode
           */
          MSG_GERAN_ERROR_0_G("Switching Single Sim to DSDA");
          l1_tskisr_blk->firmware_loaded = TRUE;
        }
        if (gl1_msg_get_multi_sim_sys_mode() != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
        {
          gl1_set_switch_multi_sim_mode(msg_ptr->multi_sim_mode);
        }
      }

      MSG_GERAN_HIGH_1_G("Device Mode: %d", gl1_msg_get_multi_sim_sys_mode());
      break;
#endif /* FEATURE_SGLTE_DUAL_SIM*/

    default:
      MSG_GERAN_ERROR_0_G("Unhandled cause value in MPH_DEVICE_MODE_IND");
      break;
  }
#endif /* FEATURE_DUAL_ACTIVE */

  return;
}
#endif /*defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )*/

/*===========================================================================

FUNCTION l1_handle_message

DESCRIPTION
  This processes a single message off the layer 1 queue

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_handle_message( l1_queue_message_T* message )
{
  if ( message == NULL)
  { /* Get the error case out of the way here, not 500 lines hence */
    return;
  }
  else
  {  /* CMD is not empty -- strip off link */
    IMH_T *msg_header = (IMH_T*)&(message->msg);
    /* determine which SIM this message is intended for and set the ptr */
    gas_id_t gas_id = l1_multi_sim_determine_gas_id(msg_header);
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
	gl1_trm_state_t trm_status = GL1_TRM_NULL;

#ifdef L1_MSG_TRAFFIC_LOGGING
    /* Log messages(other tasks->L1)to the NV. Also logs the current frame no */
    l1_log_msg_traffic(msg_header->message_set, msg_header->message_id, GSTMR_GET_FN_GERAN(gas_id), TRUE, gas_id);
#endif /* L1_MSG_TRAFFIC_LOGGING */

    switch (msg_header->message_set)
    {
    /********************************** message_set **********************************/
    case MS_RR_L1:
      switch ( msg_header->message_id)
      {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        case MPH_L1_DS_ABORT_IND:
          {
            l1_task_state_machine(msg_header, gas_id);
          }
          break;

          case MPH_DEVICE_MODE_IND:
          {
            /* Ensure GL1 Sub is awake before handling device mode */
            if (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
            {
              gl1_drx_require_next_tick( gas_id );
              mdsp_wait_for_frame_tick_non_blocking(l1_task_tcb_read(gas_id),L1_TIMEOUT_HW_SIG, gas_id);
            }
            l1_handle_device_mode_ind((mph_device_mode_ind_T *)msg_header, gas_id);
          }
          break;

          case MPH_SET_PRIORITY_IND:
          {
            mph_set_priority_ind_T *msg_ptr = (mph_set_priority_ind_T *)msg_header;

            MSG_GERAN_HIGH_4_G("MPH_SET_PRIORITY_IND for RRprocedure=%d NASprocedure=%d priority=%d (new=%d)",
                               (int)(msg_ptr->procedure_type), 
                               (int)(msg_ptr->sys_proc_type), 
                               (int)(msg_ptr->new_priority), 
                               (int)(msg_ptr->new_procedure));

            /* Store the system procedure type for future use */
            if (msg_ptr->sys_proc_type != SYS_PROC_TYPE_NONE)
            {
              gl1_set_specific_sys_proc (msg_ptr->procedure_type,msg_ptr->sys_proc_type,gas_id);
            }
            /* use the procedure type to determine how to process the specified priority */
            switch (msg_ptr->procedure_type)
            {
              case GERAN_ACQ_PROCEDURE:
                /* always update GRM for this procedure type, before any state-specific handlers */
                if (GERAN_PRIORITY_NOMINAL == msg_ptr->new_priority)
                {
                  grm_set_acquisition_priority(GERAN_PRIORITY_NOMINAL, gas_id);
                }
                else if (GERAN_PRIORITY_INVERTED == msg_ptr->new_priority)
                {
                  grm_set_acquisition_priority(GERAN_PRIORITY_INVERTED, gas_id);
                }

                if (FALSE == msg_ptr->new_procedure)
                {
                  /* for an existing procedure, pass the message on to the state machine */
                  l1_task_state_machine(msg_header, gas_id);
                }
                else
                {
                  /* GRR has indicated that it is about to send a further primitive (e.g. MPH_POWER_SCAN_REQ) */
                  /* so the new priority stored above will be applied by default when that acquisition begins */
                  /* and there is no need to pass the priority message itself through to the L1 state machine */
                  MSG_GERAN_HIGH_0_G("Further cell selection primitive expected and will use this priority");
                }
                break;

              case GERAN_SIB_PROCEDURE:
                /* update the priority for any active system info procedure */
                if (GERAN_PRIORITY_INVERTED == msg_ptr->new_priority)
                {
                  gpl1_set_invert_sys_info_trm_priority(TRUE, gas_id);
                 
                 /* if previous reservation was for BCCH then ensure TRM reservation is updated with inverted priority */
                 if( gl1_msg_get_multi_sim_mode() && 
                     (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE) &&
                     l1_tskisr_blk->reservation_for_bcch &&
                     (l1_get_fm_trm_priority(gas_id) == FM_PRI_CCCH) )
                 {
                   grm_set_trm_release_required(TRUE, l1_tskisr_blk->client_id, gas_id);
                   gl1_drx_require_next_tick( gas_id );
                 }
				 else if(gl1_msg_get_multi_sim_mode() && 
				 	    (l1_tskisr_blk->multi_sim_reselection_state ==L1_MULTI_SIM_RESELECTION_TRM_WAIT) && 
				 	    ( GL1_TRM_NULL == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id)) && 
				 	    (l1_tskisr_blk->l1_state==L1_MULTI_SIM_FIND_BCCH) )
		                 	{
		                       L1_multi_sim_bcch_reserve_at_priority_change( gas_id );
		                    }
				 }
                else
                {
                  gpl1_set_invert_sys_info_trm_priority(FALSE, gas_id);
                }
                break;

              case GERAN_LOCK_HOLD:
              {
                trm_get_info_return_type   trm_info, tx_trm_info;
                gas_id_t                   gas_id = msg_ptr->gas_id;
                grm_client_enum_t          this_client = gl1_map_gas_id_to_client_id (gas_id);
                trm_grant_return_enum_type grant_status = grm_get_granted(this_client, &trm_info, gas_id);
#ifdef FEATURE_GSM_RX_TX_SPLIT
                trm_grant_return_enum_type tx_grant_status = grm_get_granted(grm_map_rx_to_tx_client_id(this_client, gas_id),
                                                                             &tx_trm_info,
                                                                             gas_id);
#endif

                /* This procedure stops release of held TRM resources to prevent interruption during e.g. CSFB setup */
                grm_set_resource_lock_state( TRUE, gas_id );

                /* For now we assume that the only use case for procedure locking without client exchange */
                /* is to hold and lock a Tx/Rx chain for traffic so if we don't already have one then get one now */
#ifdef FEATURE_GSM_RX_TX_SPLIT
                if ((TRM_GRANTED != grant_status    && TRM_PENDING != grant_status   ) || 
                    (TRM_GRANTED != tx_grant_status && TRM_PENDING != tx_grant_status))

#else
                if (TRM_GRANTED != grant_status && TRM_PENDING != grant_status)
#endif
                {
                  trm_reason_enum_t       reason = TRM_TRAFFIC;
                  trm_resource_enum_t     requested_chain_type = gl1_ms_get_trm_resource (gas_id, reason);
                  sys_proc_type_e_type    subreason = gl1_get_specific_sys_proc (GERAN_CONN_PROCEDURE, gas_id);
                  byte                    l1_state = l1_tskisr_blk->l1_state;

                  /* Make a request for a traffic chain which we can lock */
                  grm_force_request_and_notify ( this_client,
                                                 requested_chain_type,
                                                 GRM_ACCESS_DURATION_SCLKS,
                                                 reason,
                                                 subreason,
                                                 GRM_RAN_ACCESS_RETRY);

                  if ((l1_state != L1_INACTIVE_MODE) && 
                      (l1_state != L1_NULL_MODE) && 
                      (l1_state != L1_MULTI_SIM_NULL) &&
                      (l1_state != L1_MULTI_SIM_INACTIVE))
                  {
                    /* We should not get here - GRR should only be trying to lock us when GL1 is inactive */
                    /* otherwise we risk an ongoing L1 activity using a chain we no longer own */
                    MSG_GERAN_ERROR_0_G("Resource lock getting Rx/Tx while GL1 active");
                  }
                }

                break;
              }
              case GERAN_LOCK_RELEASE:
              {
                byte l1_state = l1_tskisr_blk->l1_state;
                
                if (l1_state == L1_MULTI_SIM_IDLE)
                {
                  /* Delay resource lock sleep blocking until idle has rebooked its next activity */
                  grm_set_resource_lock_enum (GRM_RESOURCE_LOCK_GOING_INACTIVE, gas_id);
                }
                else 
                {
                  /* This procedure allows the release of locked TRM resources to take place */
                  grm_set_resource_lock_enum (GRM_RESOURCE_LOCK_INACTIVE, gas_id);
                }/*Else If Idle*/

                  /*If Resource Lock is released in Non Tx States then Drop Tx Chain 
                    in task as we wont be using it*/
                  if ((l1_state == L1_MULTI_SIM_FIND_BCCH)
                       ||(l1_state == L1_MULTI_SIM_NULL)
                       ||(l1_state == L1_INACTIVE_MODE)
                       ||(l1_state == L1_MULTI_SIM_IDLE))
                  {
                    grm_client_enum_t     tx_client_id    = grm_map_rx_to_tx_client_id(gl1_map_gas_id_to_client_id (gas_id), gas_id);
                    gl1_trm_state_t       tx_grant_status = grm_get_trm_status(tx_client_id, gas_id);
                  
                    /*If in MSim Find BCCH, Then Drop the Tx Chain*/
                    if ((tx_grant_status == GL1_TRM_GRANTED)||(tx_grant_status == GL1_TRM_RETAINED_FOR_ACCESS))
                    {
                      grm_release_tx_chain(tx_client_id, gas_id);
                    }/*If Tx Chain */
                  }/*If MSim Find BCCH||MSim NULL||Inactive Mode*/

                break;
              }
#ifdef FEATURE_DUAL_DATA
              case GERAN_MMS_PROCEDURE:
                gl1_set_data_sub_priority(msg_ptr->new_priority, gas_id);
                break;
#endif /* FEATURE_DUAL_DATA */

              case GERAN_CONN_PROCEDURE:
                /* this procedure type indicates the reason code to use for CS dedicated mode */
                /* which applies during random access mode as well as in call connected state */
                if (GERAN_PRIORITY_INVERTED == msg_ptr->new_priority)
                {
                  grm_set_connection_priority(GRM_CS_CONNECTION, TRM_TRAFFIC, gas_id);
                }
                else
                {
                  grm_set_connection_priority(GRM_CS_CONNECTION, TRM_ACCESS, gas_id);
                }
                if (FALSE == msg_ptr->new_procedure)
                {
                  /* for an existing procedure, pass the message on to the state machine */
                  l1_task_state_machine(msg_header, gas_id);
                }
                break;

              case GERAN_GPRS_PROCEDURE:
                if (msg_ptr->sys_proc_type == SYS_PROC_TYPE_NONE)
                {
                  /* If RR doesn't know why we need PS traffic then it should be user plane */
                  gl1_set_specific_sys_proc (GERAN_GPRS_PROCEDURE,SYS_PROC_TYPE_PS_CALL_USER_PLANE,gas_id);
                }
                /* this procedure type indicates the reason code to use for packet transfer mode */
                if (GERAN_PRIORITY_INVERTED == msg_ptr->new_priority)
                {
                  grm_set_connection_priority(GRM_PS_CONNECTION, TRM_ACCESS, gas_id);
                }
                else
                {
                  grm_set_connection_priority(GRM_PS_CONNECTION, TRM_BG_TRAFFIC, gas_id);
                }
                if (FALSE == msg_ptr->new_procedure)
                {
                  /* for an existing procedure, pass the message on to the state machine */
                  l1_task_state_machine(msg_header, gas_id);
                }

                break;

              default:
                MSG_GERAN_ERROR_1_G("Unhandled procedure type %d for MPH_SET_PRIORITY_IND",msg_ptr->procedure_type);
                break;
            } /* end switch(procedure_type) */
          }
          break;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
       case MPH_XMSI_UPDATE_IND:
         {
             mph_xmsi_update_ind_T *msg_ptr = (mph_xmsi_update_ind_T *)msg_header;
             GL1_ISR_LOCK(gas_id);
             l1_xmsi_copy_params(&msg_ptr->xmsi,gas_id);
             GL1_ISR_UNLOCK(gas_id);
        }
        break;
        case MPH_RESET_REQ:
          {
			/*Abort G2X*/
            l1_sc_irat_check_active(0, 0, TRUE, 0, FALSE, gas_id);
            l1_set_panic_reset_received_frame_no(gas_id, gl1_get_FN( gas_id ));
            panic_reset_triggered_by_l1[gas_id] = RR_TRIGGERED_PANIC;
            MSG_GERAN_ERROR_1_G("MPH_RESET_REQ received from RR, panic FN:%d", l1_get_panic_reset_received_frame_no(gas_id));
          }
          /* fallthrough */
        default:
          l1_task_state_machine(msg_header, gas_id);
          break;
        }
        break;

    /********************************** message_set **********************************/
    case MS_L1_L1:
    {
      switch (msg_header->message_id)
      {
        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_PANIC_RESET_CALL_IN_TASK_CNF:
          {
            l1_panic_reset_call_in_task_T* msg = ( l1_panic_reset_call_in_task_T* )msg_header; /*lint !e740 unusual pointer cast */

            l1_error_code[gas_id] = msg->error_code;

#ifdef GPL1_PANIC_RESET_DBG
            MSG_GERAN_ERROR_2_G("L1_PANIC_RESET_CALL_IN_TASK_CNF: l1_error_code - %d, panic_reset_triggered_by_l1 - %d",
                      l1_error_code[gas_id], panic_reset_triggered_by_l1[gas_id]);
#endif
            if (l1_error_code[gas_id] == L1_STALLED_ISR)
            {
              /* If the ISR stalled and RR has already sent a panic, clear this out but ensure that the
              final MPH_RESET_CNF is sent back to RR. The value of the tri-state variable
              panic_reset_triggered_by_l1 remembers this */
              if ( panic_reset_triggered_by_l1[gas_id] == RR_TRIGGERED_PANIC )
              {
                panic_reset_triggered_by_l1[gas_id] = L1_TRIGGERED_PANIC_AFTER_RR;
              }
              else
              {
                panic_reset_triggered_by_l1[gas_id] = L1_TRIGGERED_PANIC;
              }

              /* This will clear any potential pending PANIC that may be waiting in the gtmrs.c file */
              l1_reset_panic_reset_received_frame_no(gas_id);

              /* GSTMR not firing, just panic now */
              l1_do_panic_reset_after_delay(gas_id );
            }
            else
            {
              panic_reset_triggered_by_l1[gas_id] = L1_TRIGGERED_PANIC;

              /* allow GSTMR to panic in a few frames time */
              l1_set_panic_reset_received_frame_no(gas_id, gl1_get_FN( gas_id ));
              
              /*Abort G2X*/
              l1_sc_irat_check_active(0, 0, TRUE, 0, FALSE, gas_id);
              rr_send_stop_gsm_mode_req[gas_id] = FALSE;
            }

            /* Task is made active when the callback message is sent.
            * Now that the message has been processed make it inactive
            * again.  Only applies in Idle Mode.
            */
            gl1_drx_task_inactive(gas_id);
          }
          break;

        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_SC_CALL_IN_TASK_CNF:
          {
            if (l1_tskisr_blk->l1_state != L1_RESET_MODE)
            {
              l1_sc_call_in_task_T* msg = ( l1_sc_call_in_task_T* )msg_header; /*lint !e740 unusual pointer cast */
              /* already calculated at the top of the fn:    gas_id = check_gas_id(msg->gas_id); */
              msg->code( msg->data, gas_id );

              /* Task is made active when the callback message is sent.
              * Now that the message has been processed make it inactive
              * again.  Only applies in Idle Mode.
              */
              gl1_drx_task_inactive(gas_id);
            }
            else
            {
              l1_task_state_machine(msg_header, gas_id);
            }
          }
          break;

        /*********************** message_set MS_L1_L1, message_id ********************/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        case L1_GPLUSG_DEACTIVATE:
          {
            if ( GOnly_mode_settings.fw_loaded_and_ready )
            {
              /* deactivate */
              gl1_hw_multi_sim_deact(gas_id);
            }

            MSG_GERAN_HIGH_1_G("L1 actioned L1_GPLUSG_DEACTIVATE %d",GOnly_mode_settings.fw_loaded_and_ready);

            GOnly_mode_settings.fw_loaded_and_ready = FALSE;

            /* Task is made active when the callback message is sent.
            * Now that the message has been processed make it inactive
            * again.  Only applies in Idle Mode.
            */
            gl1_drx_task_inactive(gas_id);
          }
          break;

        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_MULTI_SIM_PANIC_RESET:
          {
            multi_sim_panic_reset_t* msg = ( multi_sim_panic_reset_t* )msg_header; /*lint !e740 unusual pointer cast */
            grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);
            grm_set_trm_grant_status(TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);

#ifdef GPL1_PANIC_RESET_DBG
            MSG_GERAN_ERROR_1_G("L1_MULTI_SIM_PANIC_RESET: panic_reset_triggered_by_l1 - %d",panic_reset_triggered_by_l1[gas_id]);
#endif

            /* sleep was inhibited when the callback posted the message, restore it now */
           gl1_drx_set_sleep_inhibit_for_trm(FALSE, gas_id);

              /* Issue the call to load up the firmware */
              gl1_hw_multi_sim_fw_rf_init((l1_tskisr_blk->sub_state == L1_IDLE_WAITING),
                l1_task_tcb_read(gas_id),
                gsm_l1_wait,
                L1_WAIT_HW_SIG,
                L1_TIMEOUT_HW_SIG,
                GL1_HW_INIT_RX_ONLY,
                l1_tskisr_blk->client_id,
                FALSE,
                gas_id
                );

            l1_tskisr_blk->firmware_loaded = TRUE;

            gl1_drx_require_and_wait_for_next_tick(gas_id);

            gpl1_multi_sim_panic_reset(msg->error_code, msg->reset_response, gas_id);
          }
          break;

        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_GRM_REQUEST:
        /* Task handling of the TRM interaction */
        {
          grm_info_t  *grm_info_p = NULL;
          boolean trm_denial = 0;
          grm_request_data_t *msg = ( grm_request_data_t * )msg_header;
          
          if (msg->divrx_req != GRM_DIVRX_TRM_REQ_INVALID) 
          {
            gl1_hw_rxdiv_handle_task_grm_rane(msg);
          }
          else
          {
             GL1_ISR_SAVE_LOCK(gas_id);
             if(TRM_DENIAL == grm_handle_trm_request(msg->client_id,
                   msg->duration,
                   msg->reason,
                   msg->sub_reason,
                   msg->resource,
                   msg->divrx_req,
                   gas_id))
             {
                trm_denial = 1;
             }
             GL1_ISR_SAVE_UNLOCK(gas_id);


            grm_info_p = grm_get_info_ptr_from_client(msg->client_id, gas_id);

            if (grm_info_p != NULL)
            {
              if((gl1_get_gas_id_for_ptm_client() == gas_id) && trm_denial)
              {
                grm_inform_grant_event(msg->client_id,
                                     gl1_ms_get_trm_resource(gas_id, grm_info_p->rsv_reason),
                                     grm_info_p->duration,
                                     grm_info_p->rsv_reason,
                                     TRM_DENIED
                                     );
              }
            }
            else
            {
              MSG_GERAN_ERROR_1_G("GRM info NULL for client id %d", msg->client_id);
            }

          }
          gl1_drx_task_inactive(gas_id);
        }
        break;

        case L1_GRM_REQ_AND_NOTIFY_ENH:
          /* Task handling of the TRM interaction */
          {
            grm_request_and_notify_enhanced_data_t *msg =
              ( grm_request_and_notify_enhanced_data_t * )msg_header; /*lint !e740 unusual pointer cast */

            if(grm_get_trm_status(msg->client_id, gas_id) == GL1_TRM_ABORTING)
            {
              gl1_drx_task_inactive(gas_id);
              MSG_GERAN_HIGH_0_G("RANE SKIPPPED due to TRM Status Aborting");
              break;
            }

            /* if requesting for nominal acquisition, update GRR with the state now */
            /* do this before making any request to TRM since grant is asynchronous */
            /* and do it outside of mutex protected sections to avoid any deadlocks */
            if (GRM_ACQUISITION_REASON(msg->reason) ||
                GRM_ACQUISITION_DR_REASON(msg->reason))
            {
              if (GERAN_PRIORITY_NOMINAL == grm_get_acquisition_priority(gas_id))
              {
                L1_send_MPH_ACQUISITION_STATE_IND(RR_L1_ACQ_WAITING, GERAN_PRIORITY_NOMINAL, gas_id);
              }
            }

            /* Lock ISR so that L1 and ISR shouldn't send band release for the same
            band registration transaction at the same time. TRM calls should be in L1 task context,
            but it is not true always. grm_request_and_notify_enhanced is also called in ISR context.*/
            GL1_ISR_LOCK(gas_id);

            /* Set the trm status to reflect the request */
            grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, msg->client_id, gas_id);

#ifdef FEATURE_GSM_QSC_TX_DIV
            /*Request and notify assumes an implicit release, which is happening elsewhere. 
            When this happens, need to also change the ASDIV state when in FIND_BCCH*/
            if(l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH)
            {
              gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_INACTIVE ,gas_id);
            }
#endif

            /* Issue the TRM request */
            grm_request_and_notify_enhanced( msg->client_id,
                                             msg->resource,
                                             msg->duration,
                                             msg->reason,
                                             msg->sub_reason,
                                             msg->grm_tag );

            GL1_ISR_UNLOCK(gas_id);

            /* Task is made active when the callback message is sent.
            * Now that the message has been processed make it inactive
            * again.  Only applies in Idle Mode.
            */
            gl1_drx_task_inactive(gas_id);
          }
          break;

        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_GRM_RELEASE:
          /* Task handling of the TRM interaction */
          {
            grm_release_data_t *msg = (grm_release_data_t *)msg_header; /*lint !e740 unusual pointer cast */

#ifdef FEATURE_GSM_QSC_TX_DIV
            trm_get_info_return_type get_grant;
            trm_grant_return_enum_type grant = grm_get_granted(msg->client_id, &get_grant, gas_id);
            
            if( (grant != TRM_DENIAL) ||
               ((GET_GBTA_STATE() != GRM_GBTA_NONE) && (GET_GBTA_STATE() != GRM_GBTA_BLOCKED)))
            {
              if( (l1_tskisr_blk->l1_state != L1_MULTI_SIM_IDLE) && (l1_tskisr_blk->l1_state != L1_IDLE_MODE))
              {
                gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_INACTIVE ,gas_id);
              }
              else
              {
                gl1_hw_set_asdiv_mode_config(gas_id, TRM_CLIENT_MODE_SLEEP);
              }
            }
            else
            {
              MSG_GERAN_HIGH_0_G("TRM Not granted, don't call gl1_hw_set_asdiv_mode_config");
            }
#endif

            /* already calculated at the top of the fn:    gas_id = check_gas_id(msg->gas_id); */

            GL1_ISR_LOCK(gas_id);

            l1_tskisr_blk->firmware_unload_in_progress = TRUE;

            if ( l1_tskisr_blk->firmware_loaded == TRUE )
            {
              l1_tskisr_blk->firmware_loaded = FALSE;

              if ( gl1_check_any_other_client_has_lock( gas_id) )
              {
                /* For RFLM/FED architecture, always call deact as though in DSDA.
                   Also, setting of GFW to MULTISIM idle is always peformed within the multi_sim_deact function */
                gl1_hw_multi_sim_deact(gas_id);
              }
              else
              {
                /* deactivate*/
                gl1_hw_multi_sim_deact(gas_id);

                GOnly_mode_settings.fw_loaded_and_ready = FALSE;

#ifdef FEATURE_QTA
                if ( l1_tskisr_blk->qta_in_progress )
                {
                 
#if defined ( FEATURE_QTA ) && !defined (FEATURE_CXM_QTA)
#if defined ( FEATURE_QBTA )
                  if(gl1_hw_check_in_QBTA(gas_id))
                  {
                    gl1_hw_qta_fw_rf_deinit(gas_id, GL1_FW_QBTA_CM_CLEAN_UP_OFFSET, FALSE);
                  }
                  else
#endif
                  {
                    gl1_hw_qta_fw_rf_deinit(gas_id, 0, FALSE);
                  }
#endif
                }
#endif
              }
            }
           

            /* release the currently held RF chain, or exchange it */
            grm_release_or_exchange(msg->client_id, gas_id);

            /* Task is made active when the callback message is sent.
            * Now that the message has been processed make it inactive
            * again.  Only applies in Idle Mode.
            */
            l1_tskisr_blk->firmware_load_status = FW_UNLOADED;
            gl1_drx_task_inactive(gas_id);
            l1_tskisr_blk->firmware_unload_in_progress = FALSE;

            #ifdef FEATURE_QTA
            l1_tskisr_blk->qta_in_progress = FALSE;
            #endif /* FEATURE_QTA */

            GL1_ISR_UNLOCK(gas_id);



            /* All finished so try and sleep */
            gl1_drx_task_sleep( FALSE, gas_id );
          }
          break;

        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_GRM_RELEASE_IN_CALLBACK:
          /* Task handling of the TRM interaction */
          {
            grm_release_data_t *msg = (grm_release_data_t *)msg_header; /*lint !e740 unusual pointer cast */

            GL1_ISR_LOCK(gas_id);

            /* release the currently held RF chain, or exchange it */
            grm_release(msg->client_id, gas_id);

            GL1_ISR_UNLOCK(gas_id);

          }
          break;

        case L1_GRM_DISABLE_ENABLE_DR:
          {
            grm_disable_enable_dr_t *msg = (grm_disable_enable_dr_t *)msg_header; /*lint !e740 unusual pointer cast */

            GL1_ISR_LOCK(gas_id);
            (void)grm_enable_disable_dual_receive( msg->dr_setting, msg->client_id, gas_id );
            GL1_ISR_UNLOCK(gas_id);
          }

          break;

        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_GRM_DIVRX_RELEASE:
        {
          grm_release_data_t *msg = (grm_release_data_t *)msg_header;

          GL1_ISR_LOCK(gas_id);

          /* release the currently held RF chain, or exchange it */
          grm_release_rx_sec(msg->client_id, gas_id);

          gl1_drx_task_inactive(gas_id);
          
          GL1_ISR_UNLOCK(gas_id);
        }
        break;
        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_GRM_RESERVE_AT:
          /* Task handling of the TRM interaction */
          {
            grm_reserve_at_data_t *msg = ( grm_reserve_at_data_t* )msg_header; /*lint !e740 unusual pointer cast */
#ifdef FEATURE_GSM_QSC_TX_DIV
            trm_get_info_return_type get_grant;
            trm_grant_return_enum_type grant = grm_get_granted(msg->client_id, &get_grant, gas_id);
#endif
            /* To avoid deadlock, mutex lock order has to be ISR -> Wmgr -> Grm */
            GL1_ISR_LOCK(gas_id);

            MSG_GERAN_LOW_0_G( "WMGR_SLEEP: ML GRM_RSV");
            gl1_hw_sleep_wmgr_mutex_lock(gas_id);

            if(gl1_hw_sleep_wmgr_cancel_in_prog[gas_id])
            {
              /* wakeup cancellation in progress */
              int32 warmup_dur_in_sclks;

              warmup_dur_in_sclks = gl1_hw_sleep_warmup_dur_in_sclks(gas_id);
              if(warmup_dur_in_sclks)
              {
                /* TRM reservation done here will include warmup time (store FN) */
                gl1_hw_sleep_wmgr_warmup_incl_in_trm_fn[gas_id] = msg->frames_required;
              }

              /* calculate when time */
              msg->when = gl1_hw_sleep_wmgr_calc_grm_when_time(msg->frames_required, gas_id);

              MSG_GERAN_MED_5_G("WMGR_SLEEP: L1 call to grm_reserve_at reason:%d client:%d pri:%d, when_o: %u, when_n: %u",
                msg->reason,
                msg->client_id,
                msg->pri,
                gl1_hw_sleep_wmgr_trm_when_at_gts[gas_id],
                msg->when );

              /* Issue the reservation and implicit TRM release after deactivating */
              grm_reserve_at_enhanced(
                msg->client_id, /* need to range check client_id */
                msg->when,
                msg->duration + warmup_dur_in_sclks,
                msg->reason,
                msg->sub_reason,
                msg->frames_required,
                msg->frame_duration,
                msg->resource,
                msg->pri,
                gas_id );

              
    gl1_drx_task_inactive(gas_id);

              MSG_GERAN_LOW_0_G( "WMGR_SLEEP: MU GRM_RSV");
              gl1_hw_sleep_wmgr_mutex_unlock(gas_id);

              GL1_ISR_UNLOCK(gas_id);
            }
            else 
            {
              /* continue with normal operation */
              MSG_GERAN_MED_0_G( "WMGR_SLEEP: MU GRM_RSV");
              gl1_hw_sleep_wmgr_mutex_unlock(gas_id);

              GL1_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_QSC_TX_DIV
            if( (grant != TRM_DENIAL) ||
                ((GET_GBTA_STATE() != GRM_GBTA_NONE) && (GET_GBTA_STATE() != GRM_GBTA_BLOCKED)))

            {
              if( (l1_tskisr_blk->l1_state != L1_MULTI_SIM_IDLE) && (l1_tskisr_blk->l1_state != L1_IDLE_MODE))
              {
                gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_INACTIVE ,gas_id);
              }
              else
              {
                gl1_hw_set_asdiv_mode_config(gas_id, TRM_CLIENT_MODE_SLEEP);
              }
            }
            else
            {
              MSG_GERAN_HIGH_0_G("TRM Not granted, don't call gl1_hw_set_asdiv_mode_config");
            }
#endif

            GL1_ISR_LOCK(gas_id);
            if ((grm_get_trm_status(msg->client_id, gas_id) == GL1_TRM_RETAINED_FOR_ACCESS ) ||
                /* Block when in inactive mode */
                (l1_tskisr_blk->l1_state == L1_INACTIVE_MODE) ||
                /* Block when in Multi-SIM panic reset */
                (l1_tskisr_blk->l1_state == L1_MULTI_SIM_INACTIVE) ||
                (!IS_FRAME_NUM_LATER(msg->frames_required,gl1_get_FN( gas_id )))
               )
            {
               MSG_GERAN_HIGH_3_G( "Ignore L1 call to grm_reserve_at reason:%d client:%d curr sclk:%d",
                                   msg->reason, msg->client_id, timetick_get_safe() );

               /* Re-set firmware_load_status to non-pending state because
                * with reserve_at firmware_load_status is changed to FW_PENDING_RELEASE
                * which does not allow idle abort
                */
               if ( l1_tskisr_blk->firmware_loaded )
               {
                 l1_tskisr_blk->firmware_load_status = FW_LOADED;
               }
               else
               {
                 l1_tskisr_blk->firmware_load_status = FW_UNLOADED;
               }

               /* Always do this as task actve count is pre-incremented */
               gl1_drx_task_inactive( gas_id );

               /* To avoid sleep starting on next ISR if activity is pending */
               if ( l1_tskisr_blk->l1_state != L1_INACTIVE_MODE )
               {
                 gl1_drx_require_next_tick( gas_id );
               }
            }
            else
            {
            l1_tskisr_blk->firmware_unload_in_progress = TRUE;

            if ( l1_tskisr_blk->firmware_loaded == TRUE )
            {

              l1_tskisr_blk->firmware_loaded = FALSE;

              if ( gl1_check_any_other_client_has_lock( gas_id) )
              {
                /* For RFLM/FED architecture, always call deact as though in DSDA.
                   Also, setting of GFW to MULTISIM idle is always peformed within the multi_sim_deact function */
                gl1_hw_multi_sim_deact(gas_id);
              }
              else
              {
                /* deactivate*/
                  gl1_hw_multi_sim_deact(gas_id);

                GOnly_mode_settings.fw_loaded_and_ready = FALSE;
                /* De-activate FW */

#if defined (FEATURE_QTA) && !defined (FEATURE_CXM_QTA)
                if ( l1_tskisr_blk->qta_in_progress )
                {
#if defined ( FEATURE_QBTA )
                   if(gl1_hw_check_in_QBTA(gas_id))
                   {
                      gl1_hw_qta_fw_rf_deinit(gas_id,GL1_FW_QBTA_CM_CLEAN_UP_OFFSET, FALSE);
                   }
                   else
#endif
                   {
                      gl1_hw_qta_fw_rf_deinit(gas_id,0, FALSE);
                   }
                }
#endif /* FEATURE_QTA */
              }

           }
           if(!grm_get_resource_lock_state(gas_id))
           {
             MSG_GERAN_HIGH_4_G("L1 call to grm_reserve_at reason:%d client:%d pri:%d curr sclk:%d",
                                msg->reason,
                                msg->client_id,
                                msg->pri,
                                timetick_get_safe());

             /* TRM reservation done here does not include warmup time, update it with invalid FN */
             gl1_hw_sleep_wmgr_warmup_incl_in_trm_fn[gas_id] = NOT_A_FRAME_NUMBER;

             /*
             * Finally tag TRM request with latched local time allows for the isr
             * generated request to be processed at some point in the future when the
             * task request to TRM is actually actioned
             */
             msg->when =
              ( gpl1_multi_sim_calculate_grm_reservation_time( msg->frames_required, gas_id ) +
                timetick_get_safe() );

             /* Issue the reservation and implicit TRM release after deactivating */
             grm_reserve_at_enhanced(
              msg->client_id, /* need to range check client_id */
              msg->when,
              msg->duration,
              msg->reason,
              msg->sub_reason,
              msg->frames_required,
              msg->frame_duration,
              msg->resource,
              msg->pri,
              gas_id );

             
           }

            l1_tskisr_blk->firmware_load_status = FW_UNLOADED;

            /* Task is made active when the callback message is sent.
            * Now that the message has been processed make it inactive
            * again.  Only applies in Idle Mode.
            */
            gl1_drx_task_inactive(gas_id);
            l1_tskisr_blk->firmware_unload_in_progress = FALSE;
          }
          GL1_ISR_UNLOCK(gas_id);

          /* All finished so try and sleep */
          gl1_drx_task_sleep( FALSE, gas_id );
         } /* else - if(gl1_hw_sleep_wmgr_cancel_in_prog[gas_id]) */
         } /* case L1_GRM_RESERVE_AT: */
         break;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
          case L1_GRM_RESERVE_AT_CCCH:
          {
            grm_reserve_at_data_t* msg = ( grm_reserve_at_data_t* )msg_header; /*lint !e740 unusual pointer cast */

           /*
            * Finally tag TRM request with latched local time allows for the isr
            * generated request to be processed at some point in the future when the
            * task request to TRM is actually actioned
            */
             msg->when =
               ( gpl1_multi_sim_calculate_grm_reservation_time( msg->frames_required, gas_id ) +
                 timetick_get_safe() );

             /* Issue the reservation and implicit TRM release after deactivating */
             grm_reserve_at_ccch_in_trans(
               msg->client_id,
               msg->when,
               msg->duration,
               msg->reason,
               msg->frames_required,
               msg->frame_duration,
               msg->resource,
               gas_id);
         }
         break;
#endif /* FEATURE_DUAL_SIM */
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

         case L1_GRM_ABORT_CALLBACK:
         {
           gl1_grm_abort_callback_t *msg = (gl1_grm_abort_callback_t *)msg_header;
           /* already calculated at the top of the fn:    gas_id = check_gas_id(msg->gas_id); */

            if(msg->event == TRM_UNLOCK_CANCELLED)
            {
               grm_set_frame_duration(msg->frames_grant, msg->client, gas_id);  
            }
            else
            {
           gl1_handle_abort_callback(msg->client, msg->event, msg->unlock_by_sclk, gas_id);
         }
         }
         break;

#ifdef FEATURE_GSM_RX_DIVERSITY
        case L1_GRM_DRX_UNLOCK_CALLBACK:
        {
          gl1_grm_diversity_unlock_callback_t *msg = (gl1_grm_diversity_unlock_callback_t *)msg_header;
          gl1_handle_drx_unlock(msg->diversity_client_id, gas_id);
        
        }
        break;
#endif 
#if defined( FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
        case L1_GRM_IDLE_ABORT_CALLBACK:
        {
          gl1_grm_abort_callback_t *msg = (gl1_grm_abort_callback_t *)msg_header;

          gl1_handle_idle_abort_callback(grm_map_grm_client_id_to_trm_client_id(msg->client), msg->event, msg->unlock_by_sclk,msg->unlock_dur, gas_id);
        }
        break;
#endif /* FEATURE_G2X_IDLE_TUNEAWAY ||FEATURE_G2X_REORG_TUNEAWAY */

#ifdef FEATURE_G2X_TUNEAWAY
         case L1_GTA_ABORT_CALLBACK:
         {
           gl1_grm_preemption_abort_callback_t *msg = (gl1_grm_preemption_abort_callback_t *)msg_header;
           /* already calculated at the top of the fn:    gas_id = check_gas_id(msg->gas_id); */

           g2x_ta_handle_abort_callback(msg->tbf_client_id, msg->event, msg->unlock_in_sclk, msg->winning_client_duration , msg->ta_info, gas_id);
         }
         break;
#ifdef FEATURE_GTA_2PHASE_ACCESS
         case L1_GTA_FOR_2PHASE_ACCESS_ABORT_CALLBACK:
         {
           gl1_grm_2phase_access_abort_callback_t *msg = (gl1_grm_2phase_access_abort_callback_t *)msg_header;

           gta_handle_abort_2phase_access_callback(msg->tbf_client_id, msg->event, msg->unlock_in_sclk, gas_id);
         }
         break;
#endif /*FEATURE_GTA_2PHASE_ACCESS*/
#endif /* FEATURE_G2X_TUNEAWAY */
#ifdef FEATURE_SGLTE
         case L1_GTA_ABORT_FOR_L_OR_T_CALLBACK:
         {
           gl1_grm_abort_callback_t *msg = (gl1_grm_abort_callback_t *)msg_header;

           /* already calculated at the top of the fn:    gas_id = check_gas_id(msg->gas_id); */
           gta_handle_abort_for_l_or_t_callback(msg->client, msg->event, msg->unlock_by_sclk, gas_id);
         }
         break;
#endif
#ifdef FEATURE_GTA_DURING_PS_RACH
        case L1_GTA_DUR_RACH_PS_CALLBACK:
        {
          gl1_grm_rach_ps_gta_abort_callback_t *msg = (gl1_grm_rach_ps_gta_abort_callback_t *)msg_header;
          gta_handle_abort_RACH_callback(msg->winning_client_id, msg->winning_client_reason, msg->event, msg->unlock_in_sclk, gas_id);
        }
        break;
#endif /*FEATURE_GTA_DURING_PS_RACH*/
         case L1_GRM_GRANT_CALLBACK:
        {
          gl1_grm_grant_callback_t *msg = (gl1_grm_grant_callback_t *)msg_header;
          /* already calculated at the top of the fn:    gas_id = check_gas_id(msg->gas_id); */

#ifdef FEATURE_GSM_QSC_TX_DIV
          if( (l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH) && (msg->grant_status == TRM_GRANTED))
          {
            gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXACQ ,gas_id);
          }
#endif

          gl1_handle_grant_callback( msg->client, msg->duration, gas_id );
        }
        break;
#ifdef FEATURE_GSM_RX_TX_SPLIT
        case L1_GRM_GRANT_MODIFY_TO_TRAFFIC_CALLBACK:
        {
          gl1_grm_grant_callback_t *msg = (gl1_grm_grant_callback_t *)msg_header;

          GL1_ISR_LOCK(gas_id);
          gl1_handle_grant_callback( msg->client, msg->duration, gas_id );
          grm_check_connected_reason(msg->client, GRM_CS_CONNECTION, gas_id);
          GL1_ISR_UNLOCK(gas_id);

        }
        break;
#endif /* FEATURE_GSM_RX_TX_SPLIT */
        case L1_COMPLETE_DS_ABORT:
        {
          gl1_complete_ds_abort_t *msg = (gl1_complete_ds_abort_t *)msg_header;

          /* complete processing for a MPH_L1_DS_ABORT_IND received previously */
          gpl1_gprs_complete_ds_abort(msg->client, msg->msg_id, gas_id);
        }
        break;

        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_MULTI_SIM_FW_RF_LOAD:
          {
            multi_sim_rf_fw_load_t *msg = (multi_sim_rf_fw_load_t *)msg_header;
            grm_client_enum_t dest_client = msg->client_id;
            if( l1_tskisr_blk->l1_state != L1_INACTIVE_MODE )
            {
              if ( l1_tskisr_blk->firmware_loaded )
              {
                MSG_GERAN_HIGH_0_G("Firmware already loaded - don't reload");
              }
              else if ( (FALSE == l1_tskisr_blk->firmware_loaded) && (l1_tskisr_blk->firmware_load_status == FW_LOAD_IN_PROGRESS) )
              {
                MSG_GERAN_HIGH_0_G("Firmware load in progress - don't reload");
              }
              else
              {
                 gl1_trm_state_t trm_status = grm_get_trm_status(dest_client, gas_id);

                if ( (trm_status == GL1_TRM_GRANTED) ||
                     (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
                {
                 l1_tskisr_blk->firmware_load_status = FW_LOAD_IN_PROGRESS;

                  /* Issue the call to load up the firmware */
                  gl1_hw_multi_sim_fw_rf_init((l1_tskisr_blk->sub_state == L1_IDLE_WAITING),
                    l1_task_tcb_read(gas_id),
                    gsm_l1_wait,
                    L1_WAIT_HW_SIG,
                    L1_TIMEOUT_HW_SIG,
                    GL1_HW_INIT_RX_ONLY,
                    dest_client,
                    FALSE,
                    gas_id
                    );

                  MULTI_SIM_FW_LOAD_FRAMES = MULTI_SIM_FW_LOAD_FRAMES_DEFAULT;

                  GOnly_mode_settings.fw_loaded_and_ready = TRUE;


#ifdef FEATURE_G2X_TUNEAWAY
                if (gl1_is_any_sub_in_transfer() && (!gpl1_g2x_ta_is_seq_registered_not_G(gas_id)))
                {
                   /* Ensure that the freq correction is applied for this GSM client gets skipped
                     in gl1_hw_multi_sim_fw_rf_init because fw state is ACTIVE */
                   gl1_msg_acq_tcxo_and_rotator_init(gas_id);

                   /* re-enable SAIC, which may have been disabled during GTA gap */
                   gl1_hw_cfg_saic( TRUE, gas_id );
                }
#endif

                /* set trigger for calculation of the correct cycle swallower programming value, picked up
                * at start of next frame
                */
                gl1_hw_set_active_cycle_swallower_adj( gl1_get_serving_cell_arfcn(gas_id),gas_id );

                /* DRX needs to know that TX has been potentially turned back off with the rf_init() */
                gl1_drx_rf_reinitialised(gas_id);
                l1_tskisr_blk->firmware_loaded = TRUE;
                l1_tskisr_blk->firmware_load_status = FW_LOADED;
                GOnly_mode_settings.active_client = l1_tskisr_blk->client_id;

                /* Task is made active when the callback message is sent.
                * Now that the message has been processed make it inactive
                * again.  Only applies in Idle Mode.
                */
                // CR 408126 gl1_drx_task_inactive(gas_id);
              }
              else
              {
                MSG_GERAN_ERROR_1_G("No trm GRANT status GSM client %d",dest_client);
              }
            }

            /* Task is made active when the callback message is sent.
            * Now that the message has been processed make it inactive
            * again.  Only applies in Idle Mode.
            */
            gl1_drx_task_inactive(gas_id);
            } else
            {
              gl1_drx_task_inactive(gas_id);
              MSG_GERAN_ERROR_0_G( "Ignoring RF FW LOAD in inactive state");
            }
          }
          break;
         case L1_HANDLE_RACH_TIMEOUT:
         {
           /* Handle message from timeout callback*/
           gl1_handle_rach_timeout(gas_id);
         }
         break;

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_DO_TIME_PWR_AVG:
          {
#ifdef FEATURE_GSM_COEX_SW_CXM
            int32 tx_power = gpl1_calc_avg_tx_pwr(gas_id);
            int32 rx_power = gpl1_calc_avg_rx_pwr(gas_id);

            gl1_cxm_stx_set_avg_power( geran_map_gas_id_to_cxm_id( gas_id ), tx_power, rx_power );
#else
            (void) gpl1_calc_avg_tx_pwr(gas_id);
            (void) gpl1_calc_avg_rx_pwr(gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

          }
          break;
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

        case L1_CALL_IN_TASK_TURN_TX_OFF:
          {
            mdsp_is_mdsp_idle(gas_id);

            if (!gl1_hw_turn_tx_off(TRUE, gas_id))
            {
              MSG_GERAN_ERROR_0_G("GL1 hw turn tx off unable to disable tx");
            }
            else
            {
              gl1_clear_drx_tx_status ( gas_id );
            }

          }
          break;

#ifdef FEATURE_QSH_EVENT_METRIC
        /*********************** message_set MS_L1_L1, message_id ********************/
        case L1_QSH_METRIC_IND:
          {
             gl1_process_qsh_metric_ind((l1_qsh_metric_ind_T *)msg_header, gas_id);
          }
          break;
#endif /* FEATURE_QSH_EVENT_METRIC */
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
        case L1_GRM_GPRS_LOCK_EXCHANGE:
          {

             grm_gprs_lock_exchnage_t *msg = (grm_gprs_lock_exchnage_t *)msg_header;
             grm_pchxfer_dr_state_t pchxfer_dr_state =  msg->dr_status;

              MSG_GERAN_HIGH_1_G( "L1_GRM_LOCK_EXCHANGE dr_status %d",pchxfer_dr_state);
             if(pchxfer_dr_state == GRM_PCHXFER_DR_OFF_PENDING || pchxfer_dr_state == GRM_PCHXFER_DR_ON_PENDING)
             {
                GL1_ISR_LOCK(gas_id);
                if(l1_tskisr_blk->sub_state != L1_TRANSFER_INIT)
                {
                  gpl1_ccch_xfer_exchange_trm_clients(gas_id);
                  grm_change_priority( l1_tskisr_blk->client_id,
                                        grm_get_connection_priority(GRM_PS_CONNECTION,gas_id),
                                        gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id),
                                          gas_id );

                  grm_retain_lock( l1_tskisr_blk->client_id,
                                   (grm_unlock_callback_t)&grm_gta_preemption_callback,
                                    gas_id );
                }


                if(pchxfer_dr_state == GRM_PCHXFER_DR_OFF_PENDING)
                {
                   gl1_grm_set_pchxfer_dr_status(GRM_PCHXFER_DR_OFF);
                }
                else
                {
                    gl1_grm_set_pchxfer_dr_status(GRM_PCHXFER_DR_ON);
                }
                GL1_ISR_UNLOCK(gas_id);
             }  
           }
        break;
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/


        /*********************** message_set MS_L1_L1, message_id ********************/
        default:
          l1_task_state_machine(msg_header, gas_id);
          break;
      }
      }
      break; /* msg_header->message_set == MS_L1_L1 */

    /********************************** message_set **********************************/
    case MS_CGPS_GL1:
      {
        switch (msg_header->message_id)
        {
          case GL1_CGPS_MSG_TIMETAG_REQUEST:
            {
              gl1_drx_process_timetag_request( msg_header );
            }
            break;
          default:
            {
              MSG_GERAN_ERROR_1_G("Unexpected message from CGPS %d", msg_header->message_id);
            }
            break;
        }
      }
      break;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
   case MS_MAC_L1:
    {
        switch (msg_header->message_id)
        {
         case MAC_L1_RECOVERY_REQ:
          {
              if (l1_tskisr_blk->l1_state != L1_TRANSFER_MODE)
              {
                 MSG_GERAN_HIGH_0_G( "MAC_L1_RECOVERY_REQ in Unexpected State State");
                 gpl1_gprs_recovery_send_l1_mac_recovery_cnf(gas_id);
              }
              else
              {
                l1_task_state_machine(msg_header, gas_id);
              }
          }
          break;

          default:
           l1_task_state_machine(msg_header, gas_id);
          break;

         
        }
    }
    break;
 #endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */
   
    /********************************** message_set **********************************/
    default: /* msg_header->message_set */
      {
        l1_task_state_machine(msg_header, gas_id);
      }
      break;
    /********************************** message_set **********************************/
    }  /* end switch msg_header->message_set */
  }/* end if ( msg not NULL )*/
}

static void l1_task_state_machine(IMH_T *msg_header, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint32 curr_lag = 0;
  l1_task_data_t *l1_task_data_ptr = &l1_task_data[gas_id];
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  boolean transfer_flag = FALSE;

  if (rr_send_stop_gsm_mode_req[gas_id] )
  {
    MSG_GERAN_MED_0_G("L1(INACTIVE) ...");
    L1_handle_inactive_mode(msg_header);
  }
  else
  {
    switch(l1_tskisr_blk->l1_state)
    {
      case L1_INACTIVE_MODE:
        MSG_GERAN_MED_0_G("L1(INACTIVE) ...");
        L1_handle_inactive_mode(msg_header);
        break;

      case L1_RESET_MODE:
        MSG_GERAN_MED_0_G("L1(RESET) ...");
        L1_handle_reset_mode(msg_header);
        break;

      case L1_NULL_MODE:
        MSG_GERAN_MED_0_G("L1(NULL) ...");
        L1_handle_null_mode(msg_header);
        break;

      #ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
      case L1_SUSPEND_MODE:
        MSG_GERAN_HIGH_0_G("L1 task SUSPEND MODE");
        L1_handle_suspend_mode(msg_header);
        break;
      #endif

      #ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
      case L1_WTOG_BPLMN_MODE:
        MSG_GERAN_HIGH_0_G("L1 task WTOG BPLMN MODE");
        L1_handle_WTOG_BPLMN_mode(msg_header);
        break;
      #endif

      case L1_FIND_BCCH:
        MSG_GERAN_LOW_0_G("L1(BCCH) ...");
        L1_handle_find_bcch(msg_header);
        break;

      case L1_IDLE_MODE:
        MSG_GERAN_LOW_0_G("L1(IDLE) ...");
        gl1_drx_task_active(gas_id);
        L1_handle_idle_mode(msg_header, gas_id);
        gl1_drx_task_inactive(gas_id);
        break;

      case L1_RANDOM_ACCESS_MODE:
      case L1_PACKET_ACCESS_MODE:
        MSG_GERAN_LOW_0_G("L1(ACCESS) ...");
        L1_handle_random_access_mode(msg_header);
        break;

      case L1_DEDICATED_MODE:
        MSG_GERAN_LOW_0_G("L1(DEDICATED) ...");
        L1_handle_dedicated_mode(msg_header);
        break;

      case L1_TRANSFER_MODE:
        MSG_GERAN_LOW_0_G("L1(TRANSFER) ...");
        gpl1_gprs_handle_transfer_mode((IMH_T *)msg_header);
        transfer_flag = TRUE;
        break;

      #ifdef FEATURE_GSM_DTM
      case L1_DTM_MODE:
        MSG_GERAN_LOW_0_G("L1(DTMode) ...");
        gpl1_gprs_handle_dtm_mode((IMH_T *)msg_header, gas_id);
        break;
      #endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      case L1_MULTI_SIM_NULL:
        MSG_GERAN_LOW_0_G("L1(L1_MULTI_SIM_NULL) ...");
        gpl1_gprs_handle_multi_sim_null(msg_header);
        break;

      case L1_MULTI_SIM_IDLE:
        gl1_drx_task_active(gas_id);
        MSG_GERAN_LOW_0_G("L1(L1_MULTI_SIM_IDLE) ...");
        gpl1_gprs_handle_multi_sim_idle(msg_header, gas_id);
        gl1_drx_task_inactive(gas_id);
        break;

      case L1_MULTI_SIM_FIND_BCCH:
        MSG_GERAN_LOW_0_G("L1(L1_MULTI_SIM_FIND_BCCH) ...");
        gpl1_gprs_handle_multi_sim_find_bcch(msg_header);
        break;

      case L1_MULTI_SIM_DECOUPLED:
        MSG_GERAN_MED_0_G("L1(L1_MULTI_SIM_DECOUPLED) ...");
        gpl1_gprs_handle_multi_sim_decoupled(msg_header);
        break;
        /* etc .... */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      default:
        MSG_GERAN_ERROR_1_G("Unexpected L1 state: %d",l1_tskisr_blk->l1_state);
        break;
        gl1_state_log(gas_id);  
    }/* end switch*/
  }

  /* log air interface summary only for Packet Transfer activity */
#ifdef FEATURE_GERAN_LOG_OPT  
  if  ( ( l1_tskisr_blk->l1_state == L1_TRANSFER_MODE ) || ( l1_tskisr_blk->l1_state == L1_DTM_MODE ) || ( gl1_hw_disable_conditional_logging()!= FALSE )  )
#endif
  {
    uint32 curr_fn = gl1_get_FN(gas_id);
    curr_lag = SUB_FN(curr_fn,l1_task_data_ptr->prev_fn);
    if ( (curr_lag >= 217) && (gprs_l1_idle_data != NULL) )
    {
      gpl1_log_gprs_air_interface_summary( transfer_flag, gas_id );
      l1_task_data_ptr->prev_fn = gl1_get_FN(gas_id);
    }
  }
}

/*===========================================================================

FUNCTION GPL1_GPRS_HANDLE_RLC_PH_DATA_REQ_SIG
DESCRIPTION
  This function handles the signal set from RLC for GPRS
  currently only for RLC_PH_DATA_REQ message

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_rlc_ph_data_req_sig(gas_id_t gas_id)
{
  (void)rex_clr_sigs(l1_task_tcb_read(gas_id), RLC_PH_DATA_REQ_SIG);

  MSG_GERAN_DEBUG_OPT_0_G("PL1: RLC_PH_DATA_REQ received");
  /*
   * Receiving this signal in this state indicates that
   * RLC has data available to send on the Uplink.
   *
   * Not sure if we need to handle this signal in
   * L1_RANDOM_ACCESS_MODE as MAC needs to confirm
   * successful TBF establishment before RLC sends
   * L1 RLC_PH_DATA_REQ
   */
 l1_transfer_data[gas_id].rlc_ul_data_available = TRUE;

} /* gpl1_gprs_handle_rlc_sig */

/*===========================================================================

FUNCTION GSM_L1_REPORT_WATCHDOG

DESCRIPTION
  This function reports to the watchdog task and restarts the timer for
  the next report.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gsm_l1_report_watchdog( gas_id_t gas_id )
{
  (void)rex_clr_sigs(l1_task_tcb_read(gas_id), L1_RPT_TIMER_SIG);

}   /* gsm_l1_report_watchdog */


/*===========================================================================
FUNCTION l1_isr_watchdog_*

DESCRIPTION
  These functions handle ensuring that the L1 ISR is executing.

===========================================================================*/
void l1_isr_watchdog_check ( gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if(l1_tskisr_blk->l1_state != L1_INACTIVE_MODE )
  {
    (void) rex_clr_sigs(l1_task_tcb_read(gas_id), L1_ISR_WATCHDOG_TIMER_SIG);

    /* Check if we are alive */
    if (!l1_isr_alive[gas_id])
    {
       MSG_GERAN_ERROR_0_G("*** GSM ISR stalled ***");
       l1_call_panic_reset_in_task(L1_STALLED_ISR, gas_id);
    }
    else
    {
       l1_isr_alive[gas_id] = FALSE;
       (void) rex_set_timer(&l1_isr_watchdog_timer[gas_id], L1_ISR_WATCHDOG_TIMEOUT);
    }
  }
  else
  {
   /* Disable the timer and clear any pending signal */
   (void) rex_clr_sigs(l1_task_tcb_read(gas_id), L1_ISR_WATCHDOG_TIMER_SIG);
   (void) rex_clr_timer(&l1_isr_watchdog_timer[gas_id]);
  }
}


void l1_isr_watchdog_activate (gas_id_t gas_id)
{
   (void) rex_clr_sigs(l1_task_tcb_read(gas_id), L1_ISR_WATCHDOG_TIMER_SIG);
   (void) rex_set_timer(&l1_isr_watchdog_timer[gas_id], L1_ISR_WATCHDOG_TIMEOUT);

   /* We better be kicked by the ISR within 10 seconds */
   l1_isr_alive[gas_id] = FALSE;
}


void l1_isr_watchdog_deactivate (gas_id_t gas_id)
{
   /* Disable the timer and clear any pending signal */
   (void) rex_clr_sigs(l1_task_tcb_read(gas_id), L1_ISR_WATCHDOG_TIMER_SIG);
   (void) rex_clr_timer(&l1_isr_watchdog_timer[gas_id]);
}


void l1_isr_watchdog_kick (gas_id_t gas_id)
{
   l1_isr_alive[gas_id] = TRUE;
}


/*===========================================================================

FUNCTION GSM_L1_WAIT

DESCRIPTION
  This function is L1's interface to rex_wait(). It handles reporting to the
  watchdog task in case the watchdog signal arrives before the expected
  signal.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gsm_l1_wait (rex_sigs_type mask, gas_id_t gas_id)
{
  rex_sigs_type sigs = 0;

  /* if no signal is required to be received,return immediately */
  if (mask == sigs)
  {
    return;
  }

  MSG_GERAN_MED_2_G( "gsm_l1_wait called sigs=0x%x, mask=0x%x ", sigs, mask);


  /* wait until at least one signal in mask is received */
  while ((sigs & mask) == 0)
  {
    sigs = rex_wait(mask | L1_RPT_TIMER_SIG );

    MSG_GERAN_LOW_2_G("gsm_l1_wait2 called sigs=0x%x, mask=0x%x", sigs, mask);
    /* if the watchdog signal is received, report to the watchdog */
    if (sigs &  L1_RPT_TIMER_SIG)
    {
      MSG_GERAN_MED_2_G( "watchdog sig recvd in gsm_l1_wait sigs=0x%x l1_rpt_sig = 0x%x",
                sigs, L1_RPT_TIMER_SIG);

      gsm_l1_report_watchdog(gas_id);
    }
  }


} /* gsm_l1_wait */

/*===========================================================================

FUNCTION  gl1_mdsp_panic_reset_callback

DESCRIPTION
  Callback function when the mDSP has got into a "BAD" state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_mdsp_panic_reset_callback( gas_id_t gas_id )
{
   l1_call_panic_reset_in_task( NPL1_PANIC, gas_id);
}
/*===========================================================================

FUNCTION  gl1_panic_reset_callback

DESCRIPTION
  Callback function when the mDSP has got into a "BAD" state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_panic_reset_callback( gas_id_t gas_id )
{
   if(geran_get_nv_recovery_restart_enabled(gas_id))
   {
      l1_call_panic_reset_in_task( NPL1_PANIC, gas_id);
   }
   else
   {
      MSG_GERAN_FATAL_0_G( "NPL1 Panic Error" );
   }
}

/*===========================================================================

FUNCTION  l1_call_panic_reset_in_task

DESCRIPTION
  Function called when L1 has got into a panic reset\error fatal condition.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id )
{
    l1_panic_reset_call_in_task_T msg;

    gas_id = check_gas_id(gas_id);
    msg.header.message_set = MS_L1_L1;
    msg.header.message_id  = L1_PANIC_RESET_CALL_IN_TASK_CNF;
    msg.error_code         = error_code;
    msg.gas_id = gas_id;
    PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

#ifdef GPL1_PANIC_RESET_DBG
    MSG_GERAN_ERROR_1_G("l1_call_panic_reset_in_task: error_code - %d", error_code);
#endif

    /* When we are in idle mode we need to tell the DRX manager
     * when we send a callback to the task so it knows that the
     * task is active.  This call is ignore when we are not in
     * Idle Mode because the DRX manager is off.
     */
    gl1_drx_task_active(msg.gas_id);

    ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );

}

/*===========================================================================

FUNCTION  l1_do_panic_reset

DESCRIPTION
  This function is responsible for changing the Layer1 state to RESET.
  It tells NPL1 to reset the mDSP and tells RR that L1 is resetting.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_do_panic_reset ( l1_panic_reset_type_T error_code, rr_l1_message_id_enum_T reset_response, gas_id_t gas_id )
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  volatile boolean wfw_shutdown_active;
#ifdef FEATURE_GSM_TDS
  volatile boolean local_tds_irat_active;
#endif /*FEATURE_GSM_TDS*/
#ifdef FEATURE_GSM_TO_LTE
  volatile boolean local_lte_irat_active;
#endif /*FEATURE_GSM_TO_LTE*/
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if( l1_check_for_panic_reset(gas_id) == FALSE )
  {
    return;
  }
  /* Stop the SCE here */
  l1_sc_stop(gas_id);

  panic_reset_occured[gas_id] = TRUE;
  if( L1_IDLE_MODE == l1_tskisr_blk->l1_state )
  {
    gl1_drx_require_next_tick(gas_id);
  }
   /* Force G2X shutdown and wait for WFW to be shutdown*/
   l1_sc_irat_check_active(0, 0, TRUE, 0, FALSE, gas_id);
   rr_send_stop_gsm_mode_req[gas_id] = FALSE;
   wfw_shutdown_active = g2w_is_wfw_active(gas_id);
   if (wfw_shutdown_active || l1_sc_globals_ptr->g2w_shutdown_pending == TRUE)
   {
     GL1_ISR_LOCK(gas_id);
     gl1_msg_wcdmadrv_abort_search(gas_id);
     l1_sc_globals_ptr->g2w_shutdown_pending = FALSE;
     GL1_ISR_UNLOCK(gas_id);
     MSG_GERAN_HIGH_1_G( "Waiting for WFW inactive (panic_reset_occured =%d) ", panic_reset_occured[gas_id]);

     while (wfw_shutdown_active == TRUE)
     {
        gl1_hw_delay(GL1_W_SHUTDOWN_DELAY_LOOP, gas_id);
        wfw_shutdown_active = g2w_is_wfw_active(gas_id);
     }
     l1_sc_g2w_shutdown(gas_id);
   }
   MSG_GERAN_HIGH_1_G( "Waiting for pwr scan finish (panic_reset_occured =%d) ", panic_reset_occured[gas_id]);
   if (mdsp_awake( gas_id ))
   {
     while( !mdsp_ready_for_sleep(gas_id) )
     {
       gl1_hw_delay(GL1_W_SHUTDOWN_DELAY_LOOP, gas_id);
     }
   }
   MSG_GERAN_HIGH_1_G( "mdsp ready for sleep panic_reset_occured =%d", panic_reset_occured[gas_id]);

#ifdef FEATURE_GSM_TDS
   /*CR759927:If TDSDMA IRAT is active it implies TDSCDMA_IRAT_STOP_CMD is sent and need to wait for
       * TDSCDMA_IRAT_STOP_CNF to be received before calling gl1_hw_panic_inact.
       * If l1_sc_globals_ptr->g2t_shutdown_pending == TRUE, shut down G2T and wait for TDSCDMA_IRAT_STOP_CNF
       */

   local_tds_irat_active = l1_sc_get_tds_irat_active(gas_id);
   if(local_tds_irat_active || (l1_sc_globals_ptr->g2t_shutdown_pending == TRUE))
   {
     if(l1_sc_globals_ptr->g2t_shutdown_pending == TRUE)
     {
       l1_sc_g2t_shutdown(gas_id);
     }

     while(local_tds_irat_active == TRUE)
     {
       gl1_hw_delay(GL1_W_SHUTDOWN_DELAY_LOOP, gas_id);
       local_tds_irat_active = l1_sc_get_tds_irat_active(gas_id);
     }
   }
#endif /*FEATURE_GSM_TDS*/

     /*If G2L search is active, then wait for it to complete
     if G2L MEAS_REQ is sent, then we need to wait for MEAS_CNF,
     and
     then send DEINIT_REQ and then wait for DEINIT_CNF*/
#ifdef FEATURE_GSM_TO_LTE

  local_lte_irat_active = l1_sc_get_lte_irat_active(gas_id);
  if(local_lte_irat_active)     /* this means G2L has sent INIT Req. */
  {
     /* this means G2L DEINIT is yet to be done,'l1_sc_irat_check_active' must have set this flag*/
     if(l1_sc_globals_ptr->g2l_shutdown_pending)
     {
       l1_sc_g2l_shutdown(gas_id);

       /*Now search state is NULL g2l_shutdown will have run, this will send the DEINIT REQ */
       /*wait for deinit CNF as well*/
       while(local_lte_irat_active == TRUE)
       {
         gl1_hw_delay(GL1_W_SHUTDOWN_DELAY_LOOP, gas_id);
         local_lte_irat_active = l1_sc_get_lte_irat_active(gas_id);
       }
       MSG_GERAN_HIGH_0_G("G2L: Panic_Reset: Deinit_Cnf Wait Over");
     }/*g2l_pending_shutdown*/
  }/*if lte_irat_active*/
#endif

   //Disable the ciphering if enabled
   if (gl1_msg_is_ciphering_enabled(gas_id))
   {
     gl1_hw_disable_ciphering (gas_id );
   }
   /* Clear cipher enabled flag */
   gl1_msg_clear_ciphering_enabled(gas_id);

 /* release VFR*/
   gl1_hw_terminate_tch ( gas_id );

   /* Tell NPL1 to reset and reload mDSP */
   gl1_hw_panic_inact(gas_id);

   GL1_ISR_LOCK(gas_id);

   gl1_msg_terminate(gas_id);

   /* Deactivate the ISR watchdog */
   l1_isr_watchdog_deactivate(gas_id);
   /* change PL1 to RESET state */
   l1_change_state( L1_RESET_MODE, gas_id);

   l1_init_gpl1_gprs_rrbp_data(gas_id);

   GL1_ISR_UNLOCK(gas_id);

   /* Inform RR that L1 is resetting */
   if (reset_response == MPH_RESET_IND)
   {
      L1_send_MPH_RESET_IND(gas_id );
   }
   else
   {
      L1_send_MPH_RESET_CNF(gas_id );
   }

   /* Reset the panic tri-state variable */
   panic_reset_triggered_by_l1[gas_id] = DONT_PANIC;

   l1_reset_panic_reset_received_frame_no(gas_id);

   /* Log the error code into NV. */
   l1_log_reset_type( error_code,gas_id );

   MSG_GERAN_ERROR_0_G("L1 attempting recovery");
   panic_reset_occured[gas_id] = FALSE;
}

/*===========================================================================

FUNCTION  L1_INITIALISE_L1_DATA

DESCRIPTION
  This function initialises data for the L1 task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_initialise_L1_data( gas_id_t gas_id )
{
   gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
   gl1_hw_panic_cb_type  rx_cb;

   /*initialise surrounding cell engine*/
   l1_sc_init(gas_id);

   l1_init_idle_data(gas_id);

   l1_init_dedicated_data(gas_id);

   grm_init_valid_gsm_sys_bands(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* init to TRUE - this will be corrected using the settings in the start_gsm_mode message */
   l1_sc_wcdma_init_gsm_only_mode( TRUE, gas_id );

   if ( !gl1_msg_get_multi_sim_mode() )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   {
     /* Initialize the DRX manager */
     gl1_drx_reset(gas_id);
   }

   /*
    * Do the initialisation for the ISR
    */
   L1_initialise_isr(gas_id);

   l1_smscb_init(gas_id);

#ifdef PL1_FW_SIM
   /* Initialise PACCH UL Buffer for the PL1 SIM, assume KOVU
    * bootup handles intialisation.
    */
   memset (&gpl1_gprs_mac_ul_buffer[gas_id],0,sizeof(mac_ul_ctrl_buffer_T));
#endif

   rx_cb = gl1_panic_reset_callback;
   gl1_hw_register_panic_cb( rx_cb, gas_id );

   /* -------------------- */
   /* SET UP DATA POINTERS */
   /* -------------------- */
   l1_dedicated_data_ptr->l1_dcch_s0_buffer_ptr = &l1_dcch_s0_buffer[gas_id];
   l1_dedicated_data_ptr->l1_sacch_s0_buffer_ptr = &l1_sacch_s0_buffer[gas_id];

#ifdef FEATURE_GSM_DTM
   l1_dedicated_data_ptr->l1_dtm_slam_mode  = L1_CS_TN_SLAM_LOWEST;
#endif
#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
   /* Initialize this variable to FALSE in case of recovering from a PANIC_RESET */
   l1_set_background_wcdma_plmn_scan_active( FALSE, gas_id);
#endif


    /*  Onetime initialisation of gl1_msg*** */
    gl1_msg_init(gas_id);


   /* Preset NC_MODE to FALSE, sending MPH_NC_Measurement_IND to RR
    * is controlled only by MPH_NC_MEASUREMENTS_REQ to enable and
    * MPH_STOP_NC_MEASUREMENTS_REQ to disable
    */
   gpl1_gprs_meas_mode_ptr->nc_mode = FALSE;

   /* Initialize Packet Transfer modules - this would have been executed when we
    * started up initially when task started
    */

     /* set up pacch data pointer */
   l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr = &gpl1_gprs_mac_ul_buffer[gas_id];

     /* set pacch_data_in_ptr and pacch_data_out_ptr to point to first
      * message of pacch_data buffer
      */
   gpl1_gprs_mac_ul_buffer[gas_id].pacch_data_in_ptr =
   gpl1_gprs_mac_ul_buffer[gas_id].last_pacch_txd_ptr =
   gpl1_gprs_mac_ul_buffer[gas_id].pacch_data_out_ptr = gpl1_gprs_mac_ul_buffer[gas_id].pacch_data;

     /* initialise rlc status buffer to zero's */
   memset(&(l1_transfer_data[gas_id].rlc_ul_status),0,sizeof(l1_transfer_data[gas_id].rlc_ul_status) );

     /* Initialise rlc_ul_status pointer to start  */
   l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr = l1_transfer_data[gas_id].rlc_ul_status.ul_desc;

     /* initialise flag to rlc_srv_timer FALSE, this gets set to true when RLC_PH_DATA_REQ
      * is received, i.e RLC has data to send
      */
   l1_transfer_data[gas_id].rlc_svc_timer_active = FALSE;

     /* Initialize release pending, used to inform MAC which TBF has been released */
   l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;

     /* Inistialise critical data and flags in the transfer related modules */
   gpl1_dynamic_data_init(gas_id);

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
   gpl1_gprs_set_recovery_cnf_pending(FALSE,gas_id);
   gpl1_gprs_recovery_reset(gas_id);
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */  
#ifdef FEATURE_GSM_DTM
   gpl1_dtm_data_init(gas_id);
#endif

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
   gpl1_reset_tx_pwr_avg(gas_id);
   gpl1_reset_rx_pwr_avg(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
   gpl1_reset_snr_avg(gas_id);
#endif

   gpl1_single_block_data_init(gas_id);

    /* Clearout GPL1_WAIT_FOR_ISR_TO_GET_FN sig */
   (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
}

void gsm_l1_fn_init(gas_id_t gas_id)
{
  /* Only use this mechanism to perform memory init, hardware init,
     or other startup init to satisfy dependency that follow. Avoid
     all HLOS mechanisms that will block. */
  gas_id = check_gas_id(gas_id);

  /* Initialise GL1 local critical section */
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    rex_init_crit_sect(&gl1_multi_sim_mode_crit_sec);
    rex_init_crit_sect(&gl1_multi_sim_mode_crit_gbta_sec);
    rex_init_crit_sect(&gl1_trm_crit_sec);
    rex_init_crit_sect(&gl1_g2x_tuneaway_crit_sec);
  }

  rex_init_crit_sect(&gdrivers_fn_crit_sec[gas_id]);
  rex_init_crit_sect(&gl1_local_crit_sec[gas_id]);
  rex_init_crit_sect(&gl1_task_crit_sec[gas_id]);
  rex_init_crit_sect(&gdrivers_rfa_msgr_crit_sec[gas_id]);
  rex_init_crit_sect(&gdrivers_cipher_crit_sec[gas_id]);
  rex_init_crit_sect(&gstmr_log_crit_sec[gas_id]);
  rex_init_crit_sect(&gl1_hw_sleep_wakeup_cancel_crit_sect[gas_id]);
  rex_init_crit_sect(&gl1_hw_sleep_warmup_cb_crit_sect[gas_id]);
  rex_init_crit_sect(&mdsp_async_intf_crit_sec[gas_id]);
  rex_init_crit_sect(&gl1_hw_app_mode_cfg_crit_sec[gas_id]);
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
  rex_init_crit_sect(&gl1_hw_sleep_warmup_tmln_crit_sect[gas_id]);
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  rex_init_crit_sect(&gl1_rach_crit_sec[gas_id]);
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */
  rex_init_crit_sect(&gl1_grm_info_crit_sec[gas_id]);
  rex_init_crit_sect(&gl1_hw_qta_crit_sec[gas_id]);

  rex_init_crit_sect(&gl1_irat_crit_sec[gas_id]);
  rex_init_crit_sect(&gl1_dpc_queue_crit_sec[gas_id]);

  switch ( gas_id )
  {
    case GERAN_ACCESS_STRATUM_ID_1:
      geran_tcb_set( gl1_tasks_id[gas_id], rex_self());
    break;

    case GERAN_ACCESS_STRATUM_ID_2:
      geran_tcb_set( gl1_tasks_id[gas_id], rex_self());
    break;

#if defined ( FEATURE_TRIPLE_SIM )
    case GERAN_ACCESS_STRATUM_ID_3:
      geran_tcb_set( gl1_tasks_id[gas_id], rex_self());
    break;
#endif /* FEATURE_TRIPLE_SIM */

    case GERAN_ACCESS_STRATUM_ID_UNDEFINED:
    default:
    break;
  }

  L1_set_multi_sim_client_ids(gas_id);
  L1_initialise_L1_task_data(gas_id);
  l1_initialise_global_data(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  l1_tsk_buffer[gas_id].l1_state      = L1_INACTIVE_MODE;
  l1_tsk_buffer[gas_id].l1_prev_state = L1_INACTIVE_MODE;
  l1_tsk_buffer[gas_id].going_active  = FALSE;
  l1_tsk_buffer[gas_id].stop_gsm_mode_cnf_sent = FALSE;
  l1_init_multi_sim_data(gas_id);
  /* Set DRDSDS or Non DRDSDS */
  grm_set_device_configuration(gas_id);
  /* Only initialise multi_sim_mode once */
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    gl1_msg_set_multi_sim_sys_mode(SYS_MODEM_DEVICE_MODE_NONE);
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  /* Set local copy of RF device on startup */
  gl1_hw_reset_gsm_rf_ids(l1_tsk_buffer[gas_id].client_id, gas_id);
#ifdef FEATURE_GSM_RX_TX_SPLIT
  gl1_hw_rf_reset_ftm_devices(gas_id);
#endif


  /* Only read l1_debug and sleep nv once */
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    gl1_hw_efs_read_debug();
    gl1_hw_efs_read_slp();
  }

  /* Read any MDSP EFS items */
  mdsp_load_efs( gas_id );

  /* Read any NV items used for CM */
  gl1_hw_cm_efs_read(gas_id);

  L1_initialise_L1_powersave_timers(gas_id);
  gl1_hw_rf_result_monitor_timer_init(gas_id );

  /* Initalise one sec Timer to send GTS Timing info to GPS module */
  gl1_gts_timer_init( gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  l1_rach_timer_create( gas_id );

#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

  L1_initialise_L1_data(gas_id);

  gl1_read_lte_prune_cells_below_threshold(gas_id);

  /* Set up the initial state */
  l1_change_state( L1_INACTIVE_MODE, gas_id);

  /* NOTE: This assumes that GL1 is initialized in TMC after DIAG */
  /*       (which is currently the case)                          */
  l1_log_init();
#ifdef FEATURE_GSM_LOG_COEX
  l1_log_cxm_table_init(gas_id);
#endif
  /* Register with DAL interface as DAL API is used to calculate GSM sleep time tag */
  gsm_l1_dal_init_for_sleep_timetag(gas_id);

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
  gl1_gprs_recovery_read_nv_data(gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP) || defined(FEATURE_QSH_MDUMP)

  /* Registration only once , sub_id independent */ 
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    gl1_qsh_register();
    gl1_gfw_qsh_register();
  }
#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */
  gl1_hw_mcpm_state_update_init(gas_id);
}

/*===========================================================================

FUNCTION  gsm_l1_main

DESCRIPTION
  This function is the entry point for the L1 task. It calls
  initialisation functions to perform task initialisation for the L1
  task, then sits in an infinite loop waiting for messages on the input
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gsm_l1_main( dword unused )
{
  rex_sigs_type sigs;        /* This task's signals */
  gas_id_t gas_id;

  NOTUSED(unused); /* silence compiler warnings of unused parameter */

  /* The gas_id handled by l1 tasks is determined by the tcb name
  of the task. GERAN_ACCESS_STRATUM_ID_1 will be handled by l1_task
  with the tcb gsm_l1_tcb_1, and GERAN_ACCESS_STRATUM_ID_2 is handled
  by the second instance of the task */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( rex_self() == &gsm_l1_tcb_2 )
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_2;
  }
#if defined ( FEATURE_TRIPLE_SIM )
  else if ( rex_self() == &gsm_l1_tcb_3 )
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_3;
  }
#endif /* FEATURE_TRIPLE_SIM */
  else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_1;
  }

  /* Perform startup init here. Satisfy dependency of other tasks
  provided by this task. Do not linger here, startup latency is
  impacted. Higher priority tasks in the same group will preempt
  this task at this point. Do not rely upon priority alone as the
  sole mechanism to arbitrate resource contention with other tasks. */

  rcinit_handshake_startup(); /* required rcinit handshake */
  /* Optional dog registration here */

/* Kill 2nd Thread if it's not Triton Modem */
#ifndef FEATURE_DUAL_SIM
  if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
    return;
#endif

  gsm_l1_fn_init(gas_id);

#ifdef FEATURE_QSH_MDUMP
   qsh_add_thread_id(QSH_CLT_GL1);
#endif


  /* Initialise GSM and UMTS diag service */
  gsmdiag_init ();
  umtsdiag_init();

  while (TRUE)    /*lint !e716 */
  {
     sigs = (L1_RPT_TIMER_SIG | L1_CMD_Q_SIG);
     sigs |= L1_ISR_WATCHDOG_TIMER_SIG;
     sigs |= RLC_PH_DATA_REQ_SIG;
     sigs |= SERV_MEAS_ISR_SIG;

#if defined(FEATURE_GSM_COEX_SW_CXM)
     sigs |= GL1_CXM_MSGR_SIG;
#endif

#ifdef TEST_FRAMEWORK
     #error code not present
#endif /* TEST_FRAMEWORK */

     sigs = rex_wait(sigs);

     /* handle RLC-> L1 signals
      * currently RLC_PH_DATA_REQ
      */
     if (sigs & RLC_PH_DATA_REQ_SIG)
     {
        gpl1_gprs_handle_rlc_ph_data_req_sig(gas_id);
       l1_transfer_data[gas_id].rlc_svc_timer_active=TRUE;
     }

     /*  Serving Cell Measurements */
     /*  handle serving cell isr signal */
     if (sigs & SERV_MEAS_ISR_SIG)
     {
       gpl1_gprs_serving_cell_meas_if(gas_id);
     }

     if ( sigs & L1_RPT_TIMER_SIG )
     {
        gsm_l1_report_watchdog(gas_id);
     }

     if (sigs & L1_ISR_WATCHDOG_TIMER_SIG)
     {
       l1_isr_watchdog_check(gas_id);
     }

#if defined(FEATURE_GSM_COEX_SW_CXM)
     if ( sigs & GL1_CXM_MSGR_SIG )
     {
       gl1_handle_cxm_msgr_msgs( gas_id );
     }
#endif

     if ( sigs & L1_CMD_Q_SIG )
     {
        gs_read_messages( l1_queues[gas_id] );
        /* which forwards them to l1_handle_message() for processing */
     }

#ifdef PL1_FW_SIM
     /* Keep debug in step */

     /* Clear our signals and wait */
     rex_clr_sigs(rex_self(), sigs);
#endif
#ifdef TEST_FRAMEWORK
  #error code not present
#endif /* TEST_FRAMEWORK */

   } /* end of L1 task continuous loop */

   /*UNREACHED*/
   NOTUSED(unused); /* silence compiler warnings of unused parameter */

  (void)msgr_client_delete(&l1_msgr_client[gas_id]);
  (void)msgr_client_delete(&cxm_msgr_client[gas_id]);
  mdsp_async_intf_teardown(gas_id);
}

/* Normalise naming */
void gsm_l1_task (dword dummy)
{
  gsm_l1_main(dummy);
}

/*===========================================================================

FUNCTION  l1_log_reset_type

DESCRIPTION
  This function will log the error type into the NV for L1 logging.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_log_reset_type( l1_panic_reset_type_T error_code, gas_id_t gas_id )
{
  l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
  l1_nv_log_ptr->error_code = error_code;
}

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
/*===========================================================================

FUNCTION  gpl1_call_in_task_do_time_pwr_avg

DESCRIPTION
  Perform TX and RX AVG in task time.

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS

===========================================================================*/
void gpl1_call_in_task_do_time_pwr_avg(gas_id_t gas_id)
{
  l1_do_time_pwr_avg_T msg;

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_DO_TIME_PWR_AVG;
  msg.gas_id             = gas_id;
  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
} /* gpl1_call_in_task_do_time_pwr_avg */

#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

/*===========================================================================

FUNCTION  l1_call_in_task_turn_tx_off

DESCRIPTION
  Perform Tx sleep request to RF and tx exit command from task.

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS

===========================================================================*/
void l1_call_in_task_turn_tx_off(gas_id_t gas_id)
{
  l1_turn_tx_off_ind_T msg;

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_CALL_IN_TASK_TURN_TX_OFF;
  msg.gas_id             = gas_id;
  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
} /* gpl1_call_in_task_do_time_pwr_avg */

#ifdef FEATURE_DUAL_DATA
/*===========================================================================

FUNCTION  l1_handle_cfcm_cpu_monitor

DESCRIPTION
  This function process the messages received from CFCM through Message router

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_handle_cfcm_cpu_monitor(cfcm_cmd_msg_type_s *msg_ptr)
{
  gas_id_t gl1_mms_low_priority_gas_id = gl1_get_low_priority_data_sub();

  switch (msg_ptr->fc_cmd.cmd)
  {
     case CFCM_CMD_DOWN:
     {
       /* Check if we are in Data+MMS mode and assign gas_id for DDS sub*/
       if (gl1_mms_low_priority_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED )
       {
         /* now wait for 150 mSecs. to determine if we need to change Multi-slot-class*/
         if ( (gl1_cfcm_data.gl1_mms_cfcm_state == GL1_GFCM_NULL) || (gl1_cfcm_data.gl1_mms_cfcm_state == GL1_GFCM_TIMER_STOPPED) )
       {
           gl1_cfcm_data.gl1_mms_gfn_timer = ADD_FN(GSTMR_GET_FN_GERAN(gl1_mms_low_priority_gas_id),33);
           gl1_cfcm_data.gl1_mms_cfcm_state = GL1_GFCM_TIMER_ACTIVE;
           MSG_GERAN_HIGH_3("GL1: CFCM state change state %d gfn %d [%d]",gl1_cfcm_data.gl1_mms_cfcm_state,
                                                                          gl1_cfcm_data.gl1_mms_gfn_timer,
                                                                          GSTMR_GET_FN_GERAN(gl1_mms_low_priority_gas_id) );
       }

       }

       break;
     }
     case CFCM_CMD_UP:
     case CFCM_CMD_FREEZE:
     case CFCM_CMD_FC_OFF:
     {
       /* Check if we are in Data+MMS mode and assign gas_id for DDS sub*/
       if (gl1_mms_low_priority_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED )
       {
         gl1_cfcm_data.gl1_mms_cfcm_state = GL1_GFCM_TIMER_STOPPED;
         gl1_cfcm_data.gl1_mms_gfn_timer = L1_GPRS_INVALID_FN;
         MSG_GERAN_HIGH_3("GL1: CFCM state change state %d gfn %d [%d]",gl1_cfcm_data.gl1_mms_cfcm_state,
                                                                        gl1_cfcm_data.gl1_mms_gfn_timer,
                                                                        GSTMR_GET_FN_GERAN(gl1_mms_low_priority_gas_id) );
       }

       break;
     }
     default:
     {
       MSG_GERAN_ERROR_1("GL1: Unsupported CFCM CMD %d ",msg_ptr->fc_cmd.cmd);
       break;
     }
   }
}
#endif /* FEATURE_DUAL_DATA */

void l1_handle_vfrmap_msg(geran_gl1_vfrmap_msgr_t *vfr_map_msg)
{
    uint8 as_id_index;

    for(as_id_index=0; as_id_index < MAX_AS_IDS; as_id_index++)
    {
        gl1_stored_vfr_map[geran_map_nas_id_to_gas_id(as_id_index)] = vfr_map_msg->msg.vfr_map[as_id_index];
    }
}

sys_vfr_e_type gl1_get_stored_vfrmap(gas_id_t gas_id)
{
    return gl1_stored_vfr_map[gas_id];
}


/*===========================================================================

FUNCTION  l1_multi_sim_determine_gas_id

DESCRIPTION
  Go through each message and determine the GAS ID, so that it is
  directed to the appropriate data space

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
gas_id_t l1_multi_sim_determine_gas_id(IMH_T *msg_header)
{
#if !defined ( FEATURE_DUAL_SIM ) && !defined ( FEATURE_TRIPLE_SIM )
  return GERAN_ACCESS_STRATUM_ID_1;
#else
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  if ( msg_header->message_set == MS_RR_L1 )
  {
    switch( msg_header->message_id )
    {
    case MPH_L1_DS_ABORT_IND:
      {
        mph_l1_ds_abort_ind_T* msg = ( mph_l1_ds_abort_ind_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_CHANNEL_ASSIGN_REQ:
      {
        mph_channel_assign_req_T* msg = ( mph_channel_assign_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_CHANNEL_MODE_MODIFY_REQ:
      {
        mph_channel_mode_modify_req_T* msg = ( mph_channel_mode_modify_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_CHANNEL_RELEASE_REQ:
      {
        mph_channel_release_req_T* msg = ( mph_channel_release_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_DEDICATED_UPDATE_REQ:
      {
        mph_dedicated_update_req_T* msg = ( mph_dedicated_update_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_FREQUENCY_REDEFINITION_REQ:
      {
        mph_frequency_redefinition_req_T* msg = ( mph_frequency_redefinition_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_HANDOVER_REQ:
      {
        mph_handover_req_T* msg = ( mph_handover_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_IMMED_ASSIGN_REQ:
      {
        mph_immed_assign_req_T* msg = ( mph_immed_assign_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_RECONNECT_CHANNEL_REQ:
      {
        mph_reconnect_channel_req_T* msg = ( mph_reconnect_channel_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_SET_CIPHERING_MODE_REQ:
      {
        mph_set_ciphering_mode_req_T* msg = ( mph_set_ciphering_mode_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_STOP_HANDOVER_ACCESS_REQ:
      {
        mph_stop_handover_access_req_T* msg = ( mph_stop_handover_access_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_ABORT_RA_REQ:
      {
        mph_abort_ra_req_T* msg = ( mph_abort_ra_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;
    case MPH_XMSI_UPDATE_IND:
      {
        mph_xmsi_update_ind_T* msg = ( mph_xmsi_update_ind_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;


    case MPH_DEDICATED_WCDMA_CELL_UPDATE_LIST_REQ:
      {
        mph_dedicated_wcdma_cell_update_list_req_T* msg = ( mph_dedicated_wcdma_cell_update_list_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_RESET_REQ:
      {
        mph_reset_req_T* msg = ( mph_reset_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ:
      {
        mph_resume_gsm_mode_for_bplmn_req_T* msg = ( mph_resume_gsm_mode_for_bplmn_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_START_GSM_MODE_FOR_BPLMN_REQ:
      {
        mph_start_gsm_mode_for_bplmn_req_T* msg = ( mph_start_gsm_mode_for_bplmn_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
    case MPH_RESUME_GSM_MODE_REQ:
      {
        mph_resume_gsm_mode_req_T* msg = ( mph_resume_gsm_mode_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_SUSPEND_GSM_MODE_REQ:
      {
        mph_suspend_gsm_mode_req_T* msg = ( mph_suspend_gsm_mode_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_INTERRAT_LIST_SRCH_TIME_REQ:
      {
        mph_interrat_list_srch_time_req_T* msg = ( mph_interrat_list_srch_time_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;
#endif /* FEATURE_GTOW_BACKGROUND_PLMN_SEARCH */

    case MPH_STOP_MM_NON_DRX_IND:
      {
        mph_stop_mm_non_drx_ind_t* msg = ( mph_stop_mm_non_drx_ind_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_START_MM_NON_DRX_IND:
      {
        mph_start_mm_non_drx_ind_t* msg = ( mph_start_mm_non_drx_ind_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_STOP_NC_MEASUREMENTS_REQ:
      {
        mph_stop_nc_measurements_req_t* msg = ( mph_stop_nc_measurements_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_NC_MEASUREMENTS_REQ:
      {
        mph_nc_measurements_req_t* msg = ( mph_nc_measurements_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_IGNORE_WCDMA_NEIGHBORS_REQ:
      {
        mph_ignore_wcdma_neighbors_req_T* msg = ( mph_ignore_wcdma_neighbors_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_WCDMA_CELL_UPDATE_LIST_REQ:
      {
        mph_wcdma_cell_update_list_req_T* msg = ( mph_wcdma_cell_update_list_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_UE_MODE_CHANGE_REQ:
      {
        mph_ue_mode_change_req_T* msg = ( mph_ue_mode_change_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_POWER_SCAN_ABORT_REQ:
      {
        mph_power_scan_abort_req_T* msg = ( mph_power_scan_abort_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_DECODE_BCCH_LIST_ABORT_REQ:
      {
        mph_decode_bcch_list_abort_req_T* msg = ( mph_decode_bcch_list_abort_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_GPRS_SURROUND_UPDATE_REQ:
      {
        mph_gprs_surround_update_req_t* msg = ( mph_gprs_surround_update_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_STOP_SURROUND_UPDATE_REQ:
      {
        mph_stop_surround_update_req_t* msg = ( mph_stop_surround_update_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_START_GSM_MODE_REQ:
      {
        mph_start_gsm_mode_req_T* msg = ( mph_start_gsm_mode_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_STOP_GSM_MODE_REQ:
      {
        mph_stop_gsm_mode_req_T* msg = ( mph_stop_gsm_mode_req_T* )msg_header;
        gas_id = msg->gas_id;
#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
        if (msg->g2x_resel_in_progress)
        {
          grm_els_state_update_G2X_resel( gas_id );
        }
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH 
        if( msg->g2x_resel_in_progress)
        {
          gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_IRAT_RESEL);
        }
        else if(msg->rr_l1_gsm_stop_reason == RR_L1_DEEP_SLEEP)
        {
          gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_OOS);
        }
#endif		
      }
      break;

    case MPH_GPRS_L1PARAM_UPDATE_REQ:
      {
        mph_gprs_l1param_update_req_t* msg = ( mph_gprs_l1param_update_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_IDLE_UPDATE_REQ:
      {
        mph_idle_update_req_T* msg = ( mph_idle_update_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_POWER_SCAN_REQ:
      {
        mph_power_scan_req_T* msg = ( mph_power_scan_req_T* )msg_header;
        gas_id = msg->gas_id;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH 

       if(msg->search_mode !=RR_SEARCH_BACKGROUND)
       {
         gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_ACQ_PSCAN);
       }
#endif		
      }
      break;

    case MPH_CHANGE_PAGE_MODE_REQ:
      {
        mph_change_page_mode_req_T* msg = ( mph_change_page_mode_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_DECODE_BCCH_LIST_REQ:
      {
        mph_decode_bcch_list_req_T* msg = ( mph_decode_bcch_list_req_T* )msg_header;
        gas_id = msg->gas_id;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH 
        if( msg->search_mode !=RR_SEARCH_BACKGROUND )
        {
          gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_ACQ_LIST_DECODE);
        }
#endif			
      }
      break;

    case MPH_SELECT_SPECIFIC_BCCH_REQ:
      {
        mph_select_specific_bcch_req_T * msg = ( mph_select_specific_bcch_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_READ_SYS_INFO_REQ:
      {
        mph_read_sys_info_req_T* msg = ( mph_read_sys_info_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_START_IDLE_REQ:
      {
        mph_start_idle_req_T* msg = ( mph_start_idle_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_EARLY_CAMPING_REQ:
      {
        mph_early_camping_req_T* msg = ( mph_early_camping_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_START_GPRS_IDLE_MODE_REQ:
      {
        mph_start_gprs_idle_mode_req_t* msg = ( mph_start_gprs_idle_mode_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_GPRS_RESELECTION_REQ:
      {
        mph_gprs_reselection_req_t* msg = ( mph_gprs_reselection_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_RAT_CHANGE_IND:
      {
        mph_rat_change_ind_T *msg = (mph_rat_change_ind_T *)msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_ENABLE_SLEEP_IND:
      {
        mph_enable_sleep_ind_T* msg = ( mph_enable_sleep_ind_T* )msg_header;
        gas_id =  msg->gas_id;
      }
      break;

    case MPH_DEVICE_MODE_IND:
      {
        mph_device_mode_ind_T* msg = ( mph_device_mode_ind_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case MPH_SET_PRIORITY_IND:
      {
        mph_set_priority_ind_T* msg = ( mph_set_priority_ind_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

#ifdef FEATURE_GSM_DTM
    case MPH_DTM_CHANNEL_RELEASE_REQ:
      {
        mph_dtm_channel_release_req_t* msg = ( mph_dtm_channel_release_req_t* )msg_header;
        gas_id =  msg->gas_id;
      }
      break;

#ifdef FEATURE_GSM_EDTM
    case MPH_DTM_L1PARAM_UPDATE_REQ:
      {
        mph_dtm_l1param_update_req_t* msg = ( mph_dtm_l1param_update_req_t* )msg_header;
        gas_id =  msg->gas_id;
      }
      break;
#endif /* FEATURE_GSM_EDTM */
#endif /* FEATURE_GSM_DTM */

    case MPH_DSC_THRESHOLD_IND:
      {
        mph_dsc_threshold_ind_T* msg = (mph_dsc_threshold_ind_T*) msg_header;
        gas_id =  msg->gas_id;
      }
      break;

    case MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ:
      {
        mph_specific_irat_freq_update_req_T* msg = ( mph_specific_irat_freq_update_req_T* )msg_header;
        gas_id =  msg->gas_id;
      }
      break;

#ifdef FEATURE_MODEM_CONFIG_REFRESH
    case MPH_NV_REFRESH_REQ:
      {
        mph_nv_refresh_req_T * msg = ( mph_nv_refresh_req_T* )msg_header;
        gas_id =  msg->gas_id;
      }
      break;
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

    case MPH_DEEP_SLEEP_IND:
      {
        mph_deep_sleep_ind_T *msg = (mph_deep_sleep_ind_T *)msg_header;
        gas_id = msg->gas_id;
      }
      break;

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    case MPH_REDUCED_NCELL_MEAS_IND:
      {
        mph_reduced_ncell_meas_ind_T *msg = (mph_reduced_ncell_meas_ind_T *) msg_header;
        gas_id = msg->gas_id;
      }
      break;
#endif

    default:
        MSG_GERAN_ERROR_1_G("Unexpected RR-L1 message: %d",msg_header->message_id);
        break;
    }
  }
  else
  if ( msg_header->message_set == MS_L1_L1 )
  {
    switch( msg_header->message_id )
    {
      case ISR_SCAN_CNF:
        {
          isrtim_scan_cnf_T* msg = ( isrtim_scan_cnf_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case ISR_SCH_CNF:
        {
          l1_sch_cnf_T* msg = ( l1_sch_cnf_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case ISR_DEACT_CNF:
        {
          isrtim_deact_cnf_T* msg = ( isrtim_deact_cnf_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case ISR_FIRST_FCB_CNF:
        {
          isrtim_first_fcb_cnf_T* msg = ( isrtim_first_fcb_cnf_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
      case ISR_SUSPEND_CNF:
        {
          isrtim_suspend_cnf_T* msg = ( isrtim_suspend_cnf_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;
#endif /* FEATURE_GTOW_BACKGROUND_PLMN_SEARCH */

      case ISR_HANDOVER_CNF:
        {
          isrtim_handover_cnf_T* msg = ( isrtim_handover_cnf_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_REP_PERIOD_IND:
        {
          l1_rep_period_ind_T* msg = ( l1_rep_period_ind_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_SC_CALL_IN_TASK_CNF:
        {
          l1_sc_call_in_task_T* msg = ( l1_sc_call_in_task_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_GRM_REQ_AND_NOTIFY_ENH:
        {
          grm_request_and_notify_enhanced_data_t *msg =
            (grm_request_and_notify_enhanced_data_t *)msg_header;

          gas_id = msg->gas_id;
        }
        break;

      case L1_GRM_REQUEST:
        {
          grm_request_data_t *msg = (grm_request_data_t *)msg_header;

          gas_id = msg->gas_id;
        }
        break;

      case L1_GRM_RELEASE_IN_CALLBACK:
      /*Fall Through as same msg types used*/
      case L1_GRM_RELEASE:
      case L1_GRM_DIVRX_RELEASE:
        {
          grm_release_data_t *msg = (grm_release_data_t *)msg_header;

          gas_id = msg->gas_id;
        }
        break;

      case L1_GRM_DISABLE_ENABLE_DR:
        {
          grm_disable_enable_dr_t *msg = (grm_disable_enable_dr_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;

        
      case L1_GRM_RESERVE_AT:
        {
          grm_reserve_at_data_t *msg = (grm_reserve_at_data_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;

#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
      case L1_GRM_RESERVE_AT_CCCH:
        {
          grm_reserve_at_data_t* msg = ( grm_reserve_at_data_t* )msg_header;
          gas_id = msg->gas_id;
        }
        break;
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
#ifdef FEATURE_GSM_RX_TX_SPLIT
      case L1_GRM_GRANT_MODIFY_TO_TRAFFIC_CALLBACK:
        {
          gl1_grm_grant_callback_t *msg = (gl1_grm_grant_callback_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;
#endif /* FEATURE_GSM_RX_TX_SPLIT */
      case L1_GRM_GRANT_CALLBACK:
        {
          gl1_grm_grant_callback_t *msg = (gl1_grm_grant_callback_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_COMPLETE_DS_ABORT:
        {
          gl1_complete_ds_abort_t *msg = (gl1_complete_ds_abort_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;

#ifdef FEATURE_G2X_IDLE_TUNEAWAY
      case L1_GRM_IDLE_ABORT_CALLBACK:
#endif /* FEATURE_G2X_IDLE_TUNEAWAY */
      case L1_GTA_ABORT_FOR_L_OR_T_CALLBACK:
      case L1_GRM_ABORT_CALLBACK:
        {
          gl1_grm_abort_callback_t *msg = (gl1_grm_abort_callback_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;
#ifdef FEATURE_GSM_RX_DIVERSITY
     case L1_GRM_DRX_UNLOCK_CALLBACK:
       {
           gl1_grm_abort_callback_t *msg = (gl1_grm_abort_callback_t *)msg_header;
           gas_id = msg->gas_id; 

       }
       break;
#endif       
       
      case L1_GTA_ABORT_CALLBACK:
        {
          gl1_grm_preemption_abort_callback_t *msg = (gl1_grm_preemption_abort_callback_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;
#ifdef FEATURE_GTA_2PHASE_ACCESS
      case L1_GTA_FOR_2PHASE_ACCESS_ABORT_CALLBACK:
        {
          gl1_grm_2phase_access_abort_callback_t *msg = (gl1_grm_2phase_access_abort_callback_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;
#endif //FEATURE_GTA_2PHASE_ACCESS
#ifdef FEATURE_GTA_DURING_PS_RACH
      case L1_GTA_DUR_RACH_PS_CALLBACK:
        {
          gl1_grm_rach_ps_gta_abort_callback_t *msg = (gl1_grm_rach_ps_gta_abort_callback_t *)msg_header;
          gas_id = msg->gas_id;
        }
        break;
#endif /*FEATURE_GTA_DURING_PS_RACH*/
      case L1_GPLUSG_DEACTIVATE:
        {
          multi_sim_gonly_deactivate_t* msg = ( multi_sim_gonly_deactivate_t* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_MULTI_SIM_FW_RF_LOAD:
        {
          multi_sim_rf_fw_load_t* msg = ( multi_sim_rf_fw_load_t* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_MULTI_SIM_PANIC_RESET:
        {
          multi_sim_panic_reset_t* msg = ( multi_sim_panic_reset_t* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_PANIC_RESET_CALL_IN_TASK_CNF:
        {
          l1_panic_reset_call_in_task_T* msg = ( l1_panic_reset_call_in_task_T* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_ISR_SINGLE_BLK_FAILURE_CNF:
        {
          t_l1_isr_single_blk_failure_cnf* msg = ( t_l1_isr_single_blk_failure_cnf* )msg_header;
          gas_id = msg->gas_id;
        }
        break;

      case L1_ENTER_LPM_REQ:
        {
          l1_enter_lpm_req_T *msg = ( l1_enter_lpm_req_T* )msg_header;
          gas_id                  = msg->gas_id;
        }
        break;

      case L1_DO_TIME_PWR_AVG:
        {
          l1_do_time_pwr_avg_T *msg = (l1_do_time_pwr_avg_T *)msg_header;
          gas_id                    = msg->gas_id;
        }
        break;

      case L1_CALL_IN_TASK_TURN_TX_OFF:
        {
          l1_turn_tx_off_ind_T *msg = (l1_turn_tx_off_ind_T *)msg_header;
          gas_id                    = msg->gas_id;
        }
        break;

#ifdef  FEATURE_QSH_EVENT_METRIC
      case L1_QSH_METRIC_IND:
        {
          l1_qsh_metric_ind_T *msg = (l1_qsh_metric_ind_T *)msg_header;
          gas_id                   = msg->gas_id;
        }
        break;
#endif /* FEATURE_QSH_EVENT_METRIC */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      case L1_HANDLE_RACH_TIMEOUT:
        {
          gl1_rach_timeout_callback_t *msg = (gl1_rach_timeout_callback_t *)msg_header;
          gas_id                    = msg->gas_id;
        }
        break;
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
      case L1_GRM_GPRS_LOCK_EXCHANGE:
       {
          grm_gprs_lock_exchnage_t *msg = (grm_gprs_lock_exchnage_t *)msg_header;
          gas_id  = msg->gas_id;
       }
       break;
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/ 

      default:
        MSG_GERAN_ERROR_1_G("Unexpected L1-L1 message: %d",msg_header->message_id);
      break;
    }

  }
  else
  if ( msg_header->message_set == MS_L2_L1 )
  {
    switch ( msg_header->message_id )
    {
    case PH_RANDOM_ACCESS_REQ:
      {
        ph_random_access_req_T* msg = ( ph_random_access_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case PH_DATA_REQ:
      {
        ph_data_req_T* msg = ( ph_data_req_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case PH_DATA_CNF:
      {
        ph_data_cnf_T* msg = ( ph_data_cnf_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    case PH_STOP_SACCH_MEASUREMENT_REPORT:
      {
        ph_stop_sacch_measurement_report_T* msg = ( ph_stop_sacch_measurement_report_T* )msg_header;
        gas_id =  msg->gas_id;
      }
      break;

    case PH_RANDOM_ACCESS_IND:
      {
        ph_random_access_ind_T* msg = ( ph_random_access_ind_T* )msg_header;
        gas_id = msg->gas_id;
      }
      break;

    default:
      MSG_GERAN_ERROR_1_G("Unexpected L2-L1 message: %d",msg_header->message_id);
      break;
    }
  }
  else
  if ( msg_header->message_set == MS_CB_L1)
  {
    if (msg_header->message_id == CB_SCHEDULING_REQ)  /*lint !e641 */
    {
      cb_scheduling_req_type *message_ptr;
      message_ptr = (cb_scheduling_req_type *) msg_header; /*lint !e740 */

      gas_id = geran_map_nas_id_to_gas_id(message_ptr->as_id);
    }
    else if (msg_header->message_id == CB_SKIP_REQ)  /*lint !e641 */
    {
      cb_skip_req_type *message_ptr;
      message_ptr = (cb_skip_req_type *) msg_header; /*lint !e740 */

      gas_id = geran_map_nas_id_to_gas_id(message_ptr->as_id);
    }
    else if (msg_header->message_id == CB_DEACTIVATE_REQ )  /*lint !e641 */
    {
      cb_deactivate_req_type *message_ptr;
      message_ptr = (cb_deactivate_req_type *) msg_header; /*lint !e740 */

      gas_id = geran_map_nas_id_to_gas_id(message_ptr->as_id);
    }
    else
    {
      MSG_GERAN_ERROR_1_G("Unexpected CB-L1 message: %d",msg_header->message_id);
    }
  }
  else
  if ( msg_header->message_set == MS_MAC_L1 )
  {
    switch ( msg_header->message_id )
    {
    case MAC_L1_FREQ_CONFIG:
      {
        mac_l1_freq_config_t* msg = ( mac_l1_freq_config_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_DYNAMIC_TBF_CONFIG:
      {
        mac_l1_dynamic_tbf_config_t* msg = ( mac_l1_dynamic_tbf_config_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_SINGLE_BLOCK_CONFIG:
      {
        mac_l1_single_block_config_t* msg = ( mac_l1_single_block_config_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_DL_SINGLE_BLOCK:
      {
        mac_l1_dl_single_block_t* msg = ( mac_l1_dl_single_block_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_DL_TBF_CONFIG:
      {
        mac_l1_dl_tbf_config_t* msg = ( mac_l1_dl_tbf_config_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_PACCH_RRBP:
      {
        mac_l1_pacch_rrbp_t* msg = ( mac_l1_pacch_rrbp_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_CS_RECONFIG:
      {
        mac_l1_cs_reconfig_t* msg = ( mac_l1_cs_reconfig_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_POWER_CONTROL:
      {
        mac_l1_pwr_ctrl_t* msg = ( mac_l1_pwr_ctrl_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_PAGE_MODE:
      {
        mac_l1_page_mode_t* msg = ( mac_l1_page_mode_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_DUMMY_CTRL_BLOCK:
      {
        mac_l1_dummy_ctrl_block_t* msg = ( mac_l1_dummy_ctrl_block_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_RELEASE:
      {
        mac_l1_release_t* msg = ( mac_l1_release_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_QUALITY_RPT:
      {
        mac_l1_quality_rpt_t* msg = ( mac_l1_quality_rpt_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_TS_RECONFIG:
      {
        mac_l1_ts_reconfig_t* msg = ( mac_l1_ts_reconfig_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_CONTENTION_RES:
      {
        mac_l1_contention_res_t* msg = ( mac_l1_contention_res_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_STOP_RLC_TX:
      {
        mac_l1_stop_rlc_tx_t* msg = ( mac_l1_stop_rlc_tx_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

#ifdef FEATURE_GSM_DTM
    case MAC_L1_DTM_CHANNEL_ASSIGN_REQ:
      {
        mac_l1_dtm_channel_assign_req_t* msg = ( mac_l1_dtm_channel_assign_req_t* )msg_header;
        gas_id = msg->gas_id;
}
    break;
#endif /* FEATURE_GSM_DTM */

#ifdef FEATURE_GSM_EDTM
    case MAC_L1_EDTM_CS_REL_TBF_ALLOC:
      {
        mac_l1_edtm_cs_rel_tbf_alloc_t* msg = ( mac_l1_edtm_cs_rel_tbf_alloc_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_DELETE_UL_CTRL_MSG:
{
        mac_l1_delete_ul_ctrl_msg_t* msg = ( mac_l1_delete_ul_ctrl_msg_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;
#endif /* FEATURE_GSM_EDTM */

    case MAC_PH_RANDOM_ACCESS_REQ:
      {
        mac_ph_random_access_req_t* msg = ( mac_ph_random_access_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_PH_DATA_REQ:
      {
        mac_ph_data_req_t* msg = ( mac_ph_data_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_RANDOM_ACCESS_ABORT:
  {
        mac_l1_random_access_abort_t* msg = ( mac_l1_random_access_abort_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_FIXED_TBF_CONFIG:
      {
        mac_l1_fixed_tbf_config_t* msg = ( mac_l1_fixed_tbf_config_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_PDCH_REL_TS:
      {
        mac_l1_pdch_rel_ts_t* msg = ( mac_l1_pdch_rel_ts_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
    case MAC_L1_RECOVERY_REQ:
      {
          mac_l1_recovery_req_t* msg = ( mac_l1_recovery_req_t* )msg_header;
          gas_id = msg->gas_id;
      }
    break;
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */


#ifdef FEATURE_GPRS_PS_HANDOVER
    case MAC_L1_PSHO_REQ:
      {
        mac_l1_psho_req_t* msg = ( mac_l1_psho_req_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_PSHO_STATUS:
      {
        mac_l1_psho_status_t* msg = ( mac_l1_psho_status_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;

    case MAC_L1_PSHO_CANCEL_PENDING_CONFIG:
      {
        mac_l1_psho_cancel_pending_config_t* msg = ( mac_l1_psho_cancel_pending_config_t* )msg_header;
        gas_id = msg->gas_id;
      }
    break;
#endif

    default:
      MSG_GERAN_ERROR_1_G("Unexpected MAC-L1 message: %d",msg_header->message_id);
      break;
  }
  }
  else
  if (msg_header->message_set == MS_CGPS_GL1)
  {
    if (msg_header->message_id == GL1_CGPS_MSG_TIMETAG_REQUEST)
    {
      gl1_cgps_timetag_type* msg = ( gl1_cgps_timetag_type* )msg_header;
      gas_id = (gas_id_t)msg->gas_id;
    }
  }

  /* a valid GAS ID should always be available */
  if ( gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED )
  {
    MSG_GERAN_ERROR_3_G( "GL1 is unable to determine the active AS msg_set %d msg_id %d gas_id %d",
              msg_header->message_set, msg_header->message_id, gas_id );
  }
       /* determine which SIM this message is intended for and set the ptr */
  return check_gas_id(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}


#if defined(FEATURE_GSM_COEX_SW_CXM)
/*===========================================================================

FUNCTION  gl1_handle_rf_task_msgr_msgs

DESCRIPTION
  Handler function for message router CNF messages sent by CXM

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_handle_cxm_msgr_msgs( gas_id_t gas_id )
{
  geran_cxm_msgr_t*  cmd_ptr  = NULL;

  gas_id = check_gas_id(gas_id);
  /* Clear the OS signal */
  (void)rex_clr_sigs( l1_task_tcb_read(gas_id),
                      GL1_CXM_MSGR_SIG
                    );

  /* get messages from the RF Task MSGR queue*/
  while ((cmd_ptr = (geran_cxm_msgr_t *) q_get(&gsm_l1_cxm_msgr_q[gas_id])) != NULL)
  {
    /* Check the UMID */
    switch(cmd_ptr->msg.hdr.id)
    {
    case MCS_CXM_FREQID_LIST_GSM1_IND:
          garb_cxm_notify_freqid_list((cxm_freqid_info_ind_s*)&cmd_ptr->msg.cxm_freqid_info_ind, GERAN_ACCESS_STRATUM_ID_1);
      break;

#ifdef FEATURE_WLAN_COEX_SW_CXM
    case MCS_CXM_COEX_METRICS_GSM1_REQ:
      garb_cxm_coex_metrics_req((cxm_coex_metrics_req_s*)&cmd_ptr->msg.cxm_coex_metrics_req,GERAN_ACCESS_STRATUM_ID_1);
      break;

    case MCS_CXM_COEX_ACTIVE_POLICY_GSM1_IND:
      garb_cxm_coex_active_policy((cxm_coex_active_policy_ind_s*)&cmd_ptr->msg.cxm_coex_active_policy_ind,GERAN_ACCESS_STRATUM_ID_1);
      break;
#endif

#if defined ( FEATURE_DUAL_SIM )
    case MCS_CXM_FREQID_LIST_GSM2_IND:
          garb_cxm_notify_freqid_list((cxm_freqid_info_ind_s*)&cmd_ptr->msg.cxm_freqid_info_ind, GERAN_ACCESS_STRATUM_ID_2);
      break;

    case MCS_CXM_REQUEST_ACTIVITY_INFO_GSM1_IND:
          garb_cxm_notify_request_activity_info((cxm_request_activity_info_s*)&cmd_ptr->msg.cxm_request_activity_info, GERAN_ACCESS_STRATUM_ID_1);
      break;

    case MCS_CXM_REQUEST_ACTIVITY_INFO_GSM2_IND:
          garb_cxm_notify_request_activity_info((cxm_request_activity_info_s*)&cmd_ptr->msg.cxm_request_activity_info, GERAN_ACCESS_STRATUM_ID_2);
      break;

#ifdef FEATURE_WLAN_COEX_SW_CXM
    case MCS_CXM_COEX_METRICS_GSM2_REQ:
      garb_cxm_coex_metrics_req((cxm_coex_metrics_req_s*)&cmd_ptr->msg.cxm_coex_metrics_req,GERAN_ACCESS_STRATUM_ID_2);
      break;

    case MCS_CXM_COEX_ACTIVE_POLICY_GSM2_IND:
      garb_cxm_coex_active_policy((cxm_coex_active_policy_ind_s*)&cmd_ptr->msg.cxm_coex_active_policy_ind,GERAN_ACCESS_STRATUM_ID_2);
      break;
#endif

    case MCS_CXM_COEX_TECH_STATE_BCAST_IND:
      if( (gl1_msg_get_multi_sim_sys_mode() != SYS_MODEM_DEVICE_MODE_SINGLE_SIM) 
#ifdef FEATURE_SGLTE
       || (gl1_hw_get_sglte_mode(gas_id))
#endif
        )
      {
        garb_cxm_notify_coex_bcast_ind((cxm_coex_tech_state_bcast_ind_s*)&cmd_ptr->msg.cxm_coex_tech_state_bcast_info);
      }
      break;

#endif /* FEATURE_DUAL_SIM */

#if defined ( FEATURE_TRIPLE_SIM )
    case MCS_CXM_FREQID_LIST_GSM3_IND:
      garb_cxm_notify_freqid_list((cxm_freqid_info_ind_s*)&cmd_ptr->msg.cxm_freqid_info_ind, GERAN_ACCESS_STRATUM_ID_3);
      break;

    case MCS_CXM_REQUEST_ACTIVITY_INFO_GSM3_IND:
       garb_cxm_notify_request_activity_info((cxm_request_activity_info_s*)&cmd_ptr->msg.cxm_request_activity_info, GERAN_ACCESS_STRATUM_ID_3);
      break;

#ifdef FEATURE_WLAN_COEX_SW_CXM
    case MCS_CXM_COEX_METRICS_GSM3_REQ:
      garb_cxm_coex_metrics_req((cxm_coex_metrics_req_s*)&cmd_ptr->msg.cxm_coex_metrics_req,GERAN_ACCESS_STRATUM_ID_3);
      break;

    case MCS_CXM_COEX_ACTIVE_POLICY_GSM3_IND:
      garb_cxm_coex_active_policy((cxm_coex_active_policy_ind_s*)&cmd_ptr->msg.cxm_coex_active_policy_ind,GERAN_ACCESS_STRATUM_ID_3);
      break;
#endif
#endif /* FEATURE_TRIPLE_SIM */
    }

    /* Return message to the free queue */
    q_put( &gsm_l1_cxm_msgr_free_q[gas_id], &cmd_ptr->link);
  }

}
#endif

/*===========================================================================

FUNCTION gl1_cm_mutex_lock

DESCRIPTION
  Clean up drivers after an async rx.  Used to make aborting and early
  decoding cleanup simpler for the message layer.

===========================================================================*/
void gl1_cm_mutex_lock (sys_modem_as_id_e_type as_id)
{
   gas_id_t gas_id =  geran_map_nas_id_to_gas_id(as_id);
   gl1_mutex_lock(gas_id);
}

void  gl1_mutex_lock( gas_id_t gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_enter_crit_sect(&gl1_local_crit_sec[gas_id]);
}
/*===========================================================================

FUNCTION gl1_cm_mutex_unlock

DESCRIPTION
  Clean up drivers after an async rx.  Used to make aborting and early
  decoding cleanup simpler for the message layer.

===========================================================================*/
void gl1_cm_mutex_unlock (sys_modem_as_id_e_type as_id)
{
   gas_id_t gas_id =  geran_map_nas_id_to_gas_id(as_id);
   gl1_mutex_unlock(gas_id);
}

void  gl1_mutex_unlock( gas_id_t gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&gl1_local_crit_sec[gas_id]);
}

void  gl1_task_mutex_lock( gas_id_t gas_id )
{
  rex_enter_crit_sect(&gl1_task_crit_sec[gas_id]);
}

void  gl1_task_mutex_unlock( gas_id_t gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&gl1_task_crit_sec[gas_id]);
}

void  gdrivers_FN_mtx_lock( gas_id_t gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_enter_crit_sect(&gdrivers_fn_crit_sec[gas_id]);
}

void  gdrivers_FN_mtx_unlock( gas_id_t gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&gdrivers_fn_crit_sec[gas_id]);
}

void  gstmr_log_mtx_lock( gas_id_t gas_id)
{
    rex_enter_crit_sect(&gstmr_log_crit_sec[gas_id]);
}

void  gstmr_log_mtx_unlock( gas_id_t gas_id )
{
    rex_leave_crit_sect(&gstmr_log_crit_sec[gas_id]);
}



void  mdsp_async_intf_mtx_lock( gas_id_t  gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_enter_crit_sect(&mdsp_async_intf_crit_sec[gas_id]);
}

void  mdsp_async_intf_mtx_unlock( gas_id_t gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&mdsp_async_intf_crit_sec[gas_id]);
}
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
void  gl1_rach_mutex_lock( gas_id_t  gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_enter_crit_sect(&gl1_rach_crit_sec[gas_id]);
}

void  gl1_rach_mutex_unlock( gas_id_t gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&gl1_rach_crit_sec[gas_id]);
}
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */
void  gl1_grm_info_mutex_lock(gas_id_t  gas_id)
{
  gas_id = check_gas_id(gas_id);
  rex_enter_crit_sect(&gl1_grm_info_crit_sec[gas_id]);
}

void  gl1_grm_info_mutex_unlock(gas_id_t gas_id)
{
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&gl1_grm_info_crit_sec[gas_id]);
}

void  gl1_hw_qta_mutex_lock( gas_id_t  gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_enter_crit_sect(&gl1_hw_qta_crit_sec[gas_id]);
}

void  gl1_hw_qta_mutex_unlock( gas_id_t gas_id )
{
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&gl1_hw_qta_crit_sec[gas_id]);
}

void  gl1_hw_app_mode_cfg_mutex_lock(gas_id_t  gas_id)
{
  gas_id = check_gas_id(gas_id);
  rex_enter_crit_sect(&gl1_hw_app_mode_cfg_crit_sec[gas_id]);
}

void  gl1_hw_app_mode_cfg_mutex_unlock(gas_id_t gas_id)
{
  gas_id = check_gas_id(gas_id);
  rex_leave_crit_sect(&gl1_hw_app_mode_cfg_crit_sec[gas_id]);
}

void  gl1_irat_mutex_lock(gas_id_t  gas_id)
{
 gas_id = check_gas_id(gas_id);
 rex_enter_crit_sect(&gl1_irat_crit_sec[gas_id]);
}

void  gl1_irat_mutex_unlock(gas_id_t gas_id)
{
 gas_id = check_gas_id(gas_id);
 rex_leave_crit_sect(&gl1_irat_crit_sec[gas_id]);

}

/*===========================================================================

FUNCTION  l1_task_tcb_read

DESCRIPTION
  Returns a pointer to the tcb of l1 tasks

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
rex_tcb_type* l1_task_tcb_read(gas_id_t gas_id)
{
 gas_id = check_gas_id(gas_id);
 return geran_tcb_read(gl1_tasks_id[gas_id]);
}

static void gsm_l1_dal_init_for_sleep_timetag(gas_id_t gas_id)
{
 DALResult dal_result;

 /* Ensure DAL is initialised */
 DALSYS_InitMod(NULL);

 dal_result = DalTimetick_Attach("SystemTimer", &gsmtt_handle[gas_id]);
 ASSERT( dal_result == DAL_SUCCESS );
}


/*===========================================================================

FUNCTION antenna_swth_get_nv_item

DESCRIPTION
  This function will copy the antenna switching NV items to local variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void antenna_swth_get_nv_item (void)
{

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
  int16 theshold ,delta_btw_antenna ;

  if(geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_TX_DIV_EN, &antenna_swith_tx_div_nv_enable, sizeof(antenna_swith_tx_div_nv_enable)) < 0)
  {
     antenna_swith_tx_div_nv_enable = FALSE;
  }

  if((geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_TX_DIV_TH, &theshold, sizeof(theshold)) < 0) ||
     (geran_efs_read_primary(GERAN_EFS_ANT_SWTCH_TX_DIV_MIN_DELTA_BTWN_ANT, &delta_btw_antenna, sizeof(delta_btw_antenna)) <0))
  {
    theshold = 0;
    delta_btw_antenna = 0;  
  }
 
   tx_diversity_threshold = 111 + theshold; // conversion of threshold from dbm to Rxlev_sub
   efs_get("/nv/item_files/gsm/gl1/antenna_switch_tx_div_min_delta_btw_antenna", &delta_btw_antenna, sizeof(delta_btw_antenna)) ;
   tx_div_min_delta_btw_antenna = delta_btw_antenna*16 ;// converting into dbmX16

  geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_UL_DL_DELTA_TABLE, 
                         &asdiv_UL_DL_delta_table_store, 
                         sizeof(asdiv_UL_DL_delta_table_store));
  geran_efs_read_primary(GERAN_EFS_ANT_SWTCHNG_GSM_CONTROL_MASK, 
                         &gl1_antenna_switch_control_mask, 
                         sizeof(gl1_antenna_switch_control_mask));		
  MSG_HIGH("gl1_antenna_switch_control_mask %d",gl1_antenna_switch_control_mask,0,0);
  MSG_HIGH (" jj tx_diversity_threshold %d div_min_delta = %d tx_div_nv_enable =%d" , tx_diversity_threshold,tx_div_min_delta_btw_antenna,antenna_swith_tx_div_nv_enable );
#endif /*FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/
}

idle_data_T* get_ptr_to_idle_data( gas_id_t gas_id)
{
  return &l1_idle_data_store[gas_id];
}

/*===========================================================================

FUNCTION  l1_non_connected_state

DESCRIPTION
  checks gl1 state and returns TRUE if GL1 state is not in connected mode

RETURN VALUE
  boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean L1_non_connected_state( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean result = FALSE;

  if ( ( l1_tskisr_blk->l1_state != L1_RANDOM_ACCESS_MODE )
       && ( l1_tskisr_blk->l1_state != L1_PACKET_ACCESS_MODE )
       && ( l1_tskisr_blk->l1_state != L1_DEDICATED_MODE )
       && ( l1_tskisr_blk->l1_state != L1_TRANSFER_MODE )
       && ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
    )
  {
    result = TRUE;
  }

  return (result);
}

#if defined(FEATURE_QBTA)
/*===========================================================================

FUNCTION      gl1_qbta_read_nv_qbta_support

DESCRIPTION   Read the NV from EFS

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void gl1_read_nv_qbta_support()
{
   int8 efs_ret_val = geran_efs_read_primary(GERAN_EFS_WCDMA_L1_QTA_ROTA,
               &gl1_wcdma_qta_rota_nv,
               sizeof(gl1_wcdma_qta_rota_nv)) ;


   if(efs_ret_val == -1)
   {
     gl1_wcdma_qta_rota_nv = 0x00;
   }
   MSG_HIGH("QBTA support %d",gl1_wcdma_qta_rota_nv,0,0);
}

boolean  gl1_get_qbta_nv_support()
{
  if((gl1_wcdma_qta_rota_nv & QBTA_MASK)== QBTA_MASK )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

#endif
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION gl1_handle_rach_timeout

DESCRIPTION
  This function handle the message L1_HANDLE_RACH_TIMEOUT posted when rach timer expires

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_handle_rach_timeout(gas_id_t gas_id)
{

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;

  gl1_rach_mutex_lock(gas_id);
  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  if(l1_tskisr_blk->l1_state != L1_MULTI_SIM_DECOUPLED)
  {
    MSG_GERAN_ERROR_0_G("Not in DECOUPLED state");
  }
  trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
  if ( (trm_status == GL1_TRM_GRANTED) ||
       (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
  {
    /* race condition TRM has just been granted*/
    MSG_GERAN_HIGH_0_G("trm cb won race between rach timer and trm grant");
  }else
  {
     MSG_GERAN_HIGH_0_G("RACH disable timer");
    grm_release(l1_tskisr_blk->client_id, (gas_id_t)gas_id );
    L1_send_PH_RANDOM_ACCESS_CNF_with_result((gas_id_t)gas_id, GERAN_ACCESS_DENIED_BY_TRM);
  }
  gl1_rach_mutex_unlock(gas_id);
}
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */
/*===========================================================================

FUNCTION gl1_handle_irat_mgsr_msgs

DESCRIPTION
  This function handle the message IRAT mgsr msgs posted

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_handle_irat_mgsr_msgs (geran_gl1_msgr_t*  cmd_ptr ,gas_id_t gas_id )
{
#if defined (FEATURE_GSM_TO_LTE) || defined (FEATURE_GSM_TDS)
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
#endif

#ifdef FEATURE_GSM_TDS
  byte               tds_state_local;
#endif
   volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   uint8 variant;
   gsmtr_fn_qs_type current_fn_qs;
   sys_modem_as_id_e_type as_id;

   variant = MSGR_GET_VARIANT(&cmd_ptr->msg.hdr);

   as_id = (sys_modem_as_id_e_type)variant;

   MSGR_CLEAR_VARIANT(cmd_ptr->msg.hdr.id);

   /* Check the UMID */
    switch( cmd_ptr->msg.hdr.id )
    {

      /* $X2G_CM_INT_VIA_MSG : Handle the init request and send response here*/
      case GERAN_GL1_X2G_IRAT_CM_GSM_INIT_REQ:
      {
        x2g_irat_cm_gsm_init_req_type *init_req_ptr;

        init_req_ptr = &(cmd_ptr->msg.x2g_irat_cm_gsm_init_req);

        gl1_hw_cm_gsm_init_geran ( init_req_ptr->tdma_isr_ptr , rex_self() , init_req_ptr->task_wait_handler, init_req_ptr->task_wait_sig, init_req_ptr->task_wait_sig_1, as_id, init_req_ptr->serving_rat );
        L1_send_X2G_CM_GSM_INIT_RSP( gas_id,  init_req_ptr->serving_rat );
      }
      break;

#ifdef FEATURE_GSM_TO_LTE

      case LTE_CPHY_IRAT_MEAS_G2L_INIT_CNF:
      {

        /*Decrement active task count*/
        gl1_drx_task_inactive(gas_id);

        MSG_GERAN_MED_0_G("G2L:LTE_CPHY_IRAT_G2L_MEAS_INIT_CNF ");

        gl1_irat_mutex_lock( gas_id);

        l1_sc_wcdma_data_ptr->lte_irat_active = TRUE;
        l1_sc_set_ml1_init(TRUE, gas_id);
        g2l_set_Init_Sent_status(LTE_G2L_INIT_CNF, gas_id);
        /*
        ** handle back to back init/deinit req's
        ** but not if panic reset is in progress.
        */
        if((!panic_reset_occured[gas_id]) && (l1_sc_globals_ptr->g2l_shutdown_pending == TRUE))
        {
           l1_sc_g2l_shutdown(gas_id);
        }

        /*FR 47064 Stop MCPM for ded/xfer mode- Start it at time of G2L search scheduling*/
        else if(l1_tskisr_blk->l1_state != L1_INACTIVE_MODE)
        {
          if(g2l_active_mcpm_scenario[gas_id] != 0)
          {
            gl1_drx_task_active(gas_id);
            gl1_hw_mcpm_neighbor_update(MCPM_G2L_MEAS_STOP, g2l_active_mcpm_scenario[gas_id], gas_id);
            g2l_active_mcpm_scenario[gas_id] = 0;
          }
        }

        gl1_irat_mutex_unlock( gas_id);

      }
      break;

      case LTE_CPHY_IRAT_MEAS_G2L_DEINIT_CNF:
      {
         uint16  curr_init_mode;

         gl1_irat_mutex_lock( gas_id);

         curr_init_mode    = g2l_get_Init_Sent_status(gas_id); 

         MSG_GERAN_MED_1_G("G2L:LTE_CPHY_IRAT_G2L_MEAS_DEINIT_CNF init=0x%X", curr_init_mode );

         g2l_set_Init_Sent_status(LTE_G2L_DEINIT_CNF, gas_id);

         l1_sc_mode_cnf_pending(FALSE, bLTE_RAT_INACTIVE, gas_id);
 
         l1_sc_wcdma_data_ptr->lte_irat_active = FALSE;
       
         /*
         ** back to back deinit/init, handle delayed init 
         */
         if(l1_sc_wcdma_data_ptr->lte_update_list_irat_active == TRUE)
         {
           
            gl1_hw_allocate_lte_rxlm_buf(gas_id);                           /* deinit_req deallocs the bufs */
 
            /* call resets G2L_INIT_sent */ 
            L1_send_LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ(TRUE, gas_id);  

         }
         else
         {
            /* LTE going inactive */
               l1_fm_clear_required_frames(FM_PRI_LTE_IDSEARCH, gas_id);

               if ((l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE) && (l1_get_background_wcdma_plmn_scan_active(gas_id) == FALSE))
               {
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT) )
                 MSG_GERAN_HIGH_0_G("GPLT DBG: Not calling gl1_hw_mcpm_neighbor_update(MCPM_G2L_MEAS_STOP)");
#else
                 if(g2l_active_mcpm_scenario[gas_id] != 0)
                 {
                    gl1_drx_task_active(gas_id);
                    gl1_hw_mcpm_neighbor_update(MCPM_G2L_MEAS_STOP, g2l_active_mcpm_scenario[gas_id],gas_id);
                 }
                 gl1_deep_sleep_pending[gas_id] = FALSE;
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
               }
               else
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
               g2l_active_mcpm_scenario[gas_id] = 0;
               l1_sc_set_lte_irat_mcpm_active(FALSE,gas_id);
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
               teardown_LTE_irat_messages(gas_id);
#endif  /* FEATURE_GSM_TO_WCDMA_MSGR */
         }
         
         gl1_irat_mutex_unlock( gas_id);
         
        /*Decrement active task count*/
         gl1_drx_task_inactive(gas_id);
      }
      break;
      case LTE_CPHY_IRAT_MEAS_G2L_BUILD_RF_SCRIPT_CNF:
      {
        MSG_GERAN_HIGH_2_G("G2L:LTE_CPHY_IRAT_MEAS_G2L_BUILD_RF_SCRIPT_CNF srch_state=%d idx=%d",
                           l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                           l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset);
        
        l1_sc_g2l_build_script_pending[gas_id] = FALSE;
       
        if (cmd_ptr->msg.lte_cphy_irat_meas_build_rf_script_cnf.script_aborted)
        {
          l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
        }
      }
      break;
      case LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_CNF:
      {
        l1_sc_lte_process_timed_search_results( &cmd_ptr->msg.lte_cphy_irat_meas_timed_srch_meas_cnf , gas_id);
      }
      break;

      /*
      ** Not currently used, no support from ML1
      */
      case LTE_CPHY_IRAT_MEAS_G2L_ABORT_CNF:
      {
         MSG_GERAN_MED_0_G("G2L:LTE_CPHY_IRAT_MEAS_ABORT_CNF ");

         gl1_irat_mutex_lock( gas_id);

         if(gl1_hw_get_g2x_rf_enter_called_last(gas_id))
         {
            g2l_irat_x2l_meas_exit(l1_sc_wcdma_data_ptr->g2x_aborted, gas_id);
            l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;
            gl1_hw_set_g2x_rf_enter_called_last(FALSE, gas_id);
         } 
         
         g2l_set_abort_cnf_pending_status(FALSE, gas_id);

         gl1_irat_mutex_unlock( gas_id);

         g2l_clr_Init_Sent_status(LTE_G2L_CNF_PENDING, gas_id);
      }
      break;

#endif /*FEATURE_GSM_TO_LTE*/

#ifdef FEATURE_GSM_TDS

    case TDSCDMA_IRAT_INIT_CNF:
      {

        MSG_GERAN_MED_1_G(" TDSCDMA_IRAT_INIT_CNF TDS State %d",l1_sc_get_tds_state(gas_id));

        /*Decrement active task count*/
        gl1_drx_task_inactive(gas_id);

        gl1_irat_mutex_lock( gas_id);


        gl1_TDS_IRAT_INIT_CNF_pending = FALSE;


        if(cmd_ptr->msg.tdsirat_init_cnf.err == E_FAILURE)
        {
           MSG_GERAN_ERROR_0_G("TDSCDMA_IRAT_INIT_CMD Failed to initialise TDS");

           l1_sc_reset_G2T_state(gas_id);
        }
        else if(gl1_TDS_IRAT_STOP_MODE_pending == TRUE)
        {
           l1_sc_g2t_shutdown(gas_id);
        }
        else
        {
           l1_sc_set_tds_irat_active(TRUE, gas_id);  /* active after init_cnf rx'd */

           /*FR 47064 Stop MCPM for ded/xfer mode-Start it time of G2T gap */
           if(l1_tskisr_blk->l1_state!= L1_INACTIVE_MODE)
           {
             if(g2t_active_mcpm_scenario[gas_id] != 0)
             {
               gl1_drx_task_active(gas_id);
               gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_STOP, g2t_active_mcpm_scenario[gas_id], gas_id);
               g2t_active_mcpm_scenario[gas_id] = 0;
             }
           }
        }

        gl1_irat_mutex_unlock( gas_id);


      }
      break;

    case TDSCDMA_IRAT_ACQ_RSP:
      {
        GL1_ISR_LOCK(gas_id);
        gl1_irat_mutex_lock( gas_id);

        MSG_GERAN_MED_3_G(" TDSCDMA_IRAT_ACQ_RSP TDS State %d GFN %d qbit = %d",
          l1_sc_get_tds_state(gas_id),GSTMR_GET_FN_GERAN(gas_id),gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)) );

        l1_sc_tds_process_acq_results( &cmd_ptr->msg.tdsirat_acq_rsp.freq_list, gas_id );

        /* Try and obtain shared resources back from TDS.
        * The code below does not check to see if TDL1 correctly
        * took into account the gap length given to them. This is
        * to help with developemnt and should allow them to tune their handling.
        * Should be fine to do this in Idle case
        */

        if (l1_sc_globals_ptr->mode == L1SCModeIdle)
        {
          if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE)
          {
            l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP;

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
            if (l1_sc_g2x_timeline_opt_enable[gas_id] && (l1_g2t_delay_for_fw_startup == 0) && !l1_sc_x2g_ta_irat_gap(gas_id))
            {
              /* read current FN, QS */
              current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

              /* We check if task runs before ISR. If HW_FN != SW_FN, then go to option 2 */
              if ( (current_fn_qs.fn) == GSTMR_GET_FN_GERAN(gas_id) )
              {
                if (current_fn_qs.qs < G2X_MAX_OFFSET_FOR_PROCESS_CLEANUP)
                {
                  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;

                  mdsp_g2x_cleanup_cmd( 0, gl1_hw_get_gsm_rxlm_buf_idx(gas_id), TRUE, TRUE, gas_id );
                  mdsp_async_intf_send_immediate_cmd(GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);

                  gl1_cleanup_decrement_time_offset[gas_id] = 0;
                  mdsp_decrement_cleanup_time_offset_flag[gas_id] = FALSE;
                  l1_g2t_delay_for_fw_cleanup= TRUE;
                }
              }
            }
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
          }
        }

        tds_state_local = l1_sc_get_tds_state(gas_id);
        l1_sc_set_tds_state(++tds_state_local,gas_id);



        gl1_irat_mutex_unlock( gas_id);
        GL1_ISR_UNLOCK(gas_id);

      }
      break;

    case TDSCDMA_IRAT_MEAS_RSP:
      {
        GL1_ISR_LOCK(gas_id);
        gl1_irat_mutex_lock( gas_id);

        MSG_GERAN_MED_2_G(" TDSCDMA_IRAT_MEAS_RSP GFN %d qbit = %d",
          GSTMR_GET_FN_GERAN( gas_id),gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
        l1_sc_tds_process_meas_results( &cmd_ptr->msg.tdsirat_meas_rsp.freq_list, gas_id  );

        /* CR 349239*/
        if (l1_sc_globals_ptr->mode == L1SCModeIdle)
        {
          if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE)
          {
            l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP;

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
            if (l1_sc_g2x_timeline_opt_enable[gas_id] && (l1_g2t_delay_for_fw_startup == 0) && !l1_sc_x2g_ta_irat_gap(gas_id))
            {
              /* read current FN, QS */
              current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

              /* We check if task runs before ISR. If HW_FN != SW_FN, then go to option 2 */
              if ( (current_fn_qs.fn) == GSTMR_GET_FN_GERAN(gas_id) )
              {
                if (current_fn_qs.qs < G2X_MAX_OFFSET_FOR_PROCESS_CLEANUP)
                {
                  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;

                  mdsp_g2x_cleanup_cmd( 0, gl1_hw_get_gsm_rxlm_buf_idx(gas_id), TRUE, TRUE, gas_id );
                  mdsp_async_intf_send_immediate_cmd(GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);

                  gl1_cleanup_decrement_time_offset[gas_id] = 0;
                  mdsp_decrement_cleanup_time_offset_flag[gas_id] = FALSE;
                  l1_g2t_delay_for_fw_cleanup= TRUE;
                }
              }
            }
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
          }
        }

        gl1_irat_mutex_unlock( gas_id);
        GL1_ISR_UNLOCK(gas_id);
      }
      break;

    case TDSCDMA_IRAT_ABORT_CNF:
      {


        MSG_GERAN_MED_3_G(" TDSCDMA_IRAT_ABORT_CNF Received GFN %d qbit = %d success %d",
          GSTMR_GET_FN_GERAN( gas_id),gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)),
          ((cmd_ptr->msg.tdsirat_abort_cnf.err) == 0 ? TRUE:FALSE) );
        if (cmd_ptr->msg.tdsirat_abort_cnf.err == E_SUCCESS)
        {
          /*Abort from TL1 was succesful*/
          l1_sc_tds_process_abort(gas_id);
        }
        else
        {
          /*Abort from TL1 was not successful - need to wait until end of gap.*/
        }

 
      }
      break;

    case TDSCDMA_IRAT_STOP_CNF:
       {
          gl1_irat_mutex_lock( gas_id);

          /*Received Stop Cnf from TD, reset this flag*/
          g2t_stop_mode_cnf_pending = FALSE;

          if (l1_sc_wcdma_data_ptr->tds_update_list_irat_active == FALSE)
          {

             /*Clear FM entry for TDS*/
             l1_fm_clear_required_frames(FM_PRI_TDS_ACQ, gas_id);

             #ifdef FEATURE_GSM_TO_WCDMA_MSGR
             teardown_TDSCDMA_irat_messages(gas_id);
             #endif /*FEATURE_GSM_TO_WCDMA_MSGR*/

             if ((l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE)&&(l1_get_background_wcdma_plmn_scan_active(gas_id) == FALSE))
             {
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT) )
        MSG_GERAN_HIGH_0_G("GPLT DBG: Not calling gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_STOP)");
#else
               if(g2t_active_mcpm_scenario[gas_id] != 0)
                 {
                     gl1_drx_task_active(gas_id);
                     gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_STOP,g2t_active_mcpm_scenario[gas_id],gas_id);
                 }
                 else
                 {
                   gl1_deep_sleep_pending[gas_id] = FALSE;
                 }

#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
             }
             else if(l1_tsk_buffer[gas_id].l1_state == L1_INACTIVE_MODE)
             {
               if(gl1_deep_sleep_pending[gas_id] == TRUE)
               {
                 gl1_deep_sleep_pending[gas_id] = FALSE;
                 gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_STOP_REQ, GL1_MCPM_GERAN_STOP,gas_id);
                 gstmr_force_reinit(gas_id);
                 MSG_GERAN_HIGH_0_G(" MCPM_GERAN_STOP_REQ sent here%d");
               }
             }
             l1_sc_set_tds_irat_mcpm_active(FALSE,gas_id);
             g2t_active_mcpm_scenario[gas_id] = 0;
          }
          else
          {
           

             gl1_hw_allocate_tds_rxlm_buf();
             L1_send_TDS_IRAT_INIT_CMD(gas_id);
          }

          l1_sc_mode_cnf_pending(FALSE, bTDS_RAT_INACTIVE, gas_id);

          MSG_GERAN_MED_1_G(" TDSCDMA_IRAT_STOP_CNF Received GFN %d", GSTMR_GET_FN_GERAN( gas_id));
          gl1_irat_mutex_unlock( gas_id);

          /*Decrement active task count*/
          gl1_drx_task_inactive(gas_id);
      }
      break;

#endif /*FEATURE_GSM_TDS*/

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
    case WCDMA_IRAT_G2W_INIT_CNF:
    case WCDMA_IRAT_G2W_DEINIT_CNF:
    case WCDMA_IRAT_G2W_PRE_SRCH_CNF:
    case WCDMA_IRAT_G2W_SRCH_CNF:
    case WCDMA_IRAT_G2W_ABORT_CNF:
    case WCDMA_IRAT_G2W_RF_EXIT_CNF:
      gl1_handle_wcdma_irat_mgsr_msgs(cmd_ptr, gas_id);
      break;
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

    case GERAN_GL1_QTA_STARTUP_REQ:
    case GERAN_GL1_QTA_CLEANUP_REQ:
      if ( gas_id == geran_map_nas_id_to_gas_id(as_id))
      {
        gl1_handle_qta_mgsr_msgs(cmd_ptr, gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_2_G("Message id %d for unknown as_id %d ",cmd_ptr->msg.hdr.id,as_id);
      }
      break;
    default:

      /* */
      MSG_GERAN_ERROR_1_G(" Unknown message router msg id %d ",cmd_ptr->msg.hdr.id);

      break;
    } /* End of switch */
    
} /* gl1_handle_irat_mgsr_msgs */

/*===========================================================================

FUNCTION  L1_set_multi_sim_client_ids

DESCRIPTION
  This function sets the TRM client IDs according to the access stratum ID

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_set_multi_sim_client_ids( gas_id_t gas_id )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  grm_client_enum_t grm_clients[] =
    { GRM_GSM1, 
      GRM_GSM2
#ifdef FEATURE_TRIPLE_SIM
    , GRM_GSM3 
#endif
    };

  gas_id_t i;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if( !(l1_tsk_buffer[i].client_id == GRM_GPRS1 || l1_tsk_buffer[i].client_id == GRM_GPRS2))
    {
      l1_tsk_buffer[i].client_id = grm_clients[i];
    }
    else  
    {
      MSG_GERAN_HIGH_2_G("Dont reset trm client for sub %d, curr trm cleint id %d", (i+1), l1_tsk_buffer[i].client_id);
    }
  }
#else
  /* single GERAN stack only (e.g. C+G), check where GAS ID #1 maps to */
  if (geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1) == SYS_MODEM_AS_ID_2)
  {
    /* upper layers have assigned AS_ID #2 to the single GSM access stratum */
    l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].client_id = GRM_GSM2;
  }
  else
  {
    l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].client_id = GRM_GSM1;
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  return;
}

/*===========================================================================

FUNCTION  gl1_handle_qta_mgsr_msgs

DESCRIPTION
 process QTA Msgr messages

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_handle_qta_mgsr_msgs(
  /* the message to handle */
  geran_gl1_msgr_t *cmd_ptr,
  /* subscription id this function is running under */
  gas_id_t gas_id)
{
  switch(cmd_ptr->msg.hdr.id)
  {
    case GERAN_GL1_QTA_STARTUP_REQ:
    {
      gl1_hw_trm_qta_startup(&cmd_ptr->msg.geran_gl1_qta_startup_req, gas_id);
      break;
    }
    case GERAN_GL1_QTA_CLEANUP_REQ:
    {
      gl1_hw_trm_qta_cleanup(&cmd_ptr->msg.geran_gl1_qta_cleanup_req, gas_id);
      break;
    }
    default:
    {
      MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"Unknown message router msg id %d", cmd_ptr->msg.hdr.id);
      break;
    }
  }
}

/* EOF */
