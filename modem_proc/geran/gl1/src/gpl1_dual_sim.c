/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     L 1 MULTI SIM GENERAL PROCESSING FILE

GENERAL DESCRIPTION
This module handles Multi-SIM general purpose functions

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dual_sim.c#6 $
$DateTime: 2020/07/28 11:01:41 $ $Author: pwbldsvc $

when      who       what, where, why
--------  --------  ----------------------------------------------------------
24/07/20   sal      CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
11/02/20   sal      CR2619668 In case of SSIM, Don't increment missed_frame counter in first ISR after Rude wake up.
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
09/01/20   nv		CR2599009 Do not run activity in the very next frame after rude wakeup
28/02/18   rc       CR2195862 Handling back to back grant callbacks.
23/04/18   sn       CR2228272 Ignoring of Abort Callback during IDLE to TRANSFER Intermediatry States
23/02/18   sp       CR2211037 Dont consider suspension frame delay for foreground acq during abort callback. Its already handled in Find bcch statemachine
26/02/18   nsa      CR2195353 Modifying non-drx timer check so that page decoding will not get affected. 
16/02/18   sk       CR2190639 Check TRM status TRM_NOT_GRANTED also along with TRM_NULL when requesting TRM for RACH
15/02/18   nsa      CR2188236 No need of resetting gap to next activity to zero in SDCCH_Combined mode & 
                              Reserving with LTA reason-subreason when non-drx timer is set. 
3/03/17    ksb      CR2013989 Correcting TRM status from retained for access to granted for SS, as all paging blocks is ignored  in calculating the next page FN.
11/05/17   ksb      CR2033259 Dont delay the first activity after rude wakeup even if the FW, RF initialised in the same frame
01/02/18   ap       CR2022526 X tech SUB1 page denied by GSM SUB2 when reading PCH in Transfer
02/01/17   sk       CR2001022  Delay RANE for RACH till we get QTA Cleanup
01/02/18   km       CR2102442 Stop SCE for TRANS mode if DL_TBF_EST failed due to TRM failure
01/02/18   km       CR2093644 Activate SCE when IDLE COMMAND is set in idle mode
16/02/17   ks       CR2007493 Do not process QTA startup in suspend mode
03/10/17   sp       CR2115832 Avoid MCPM State transitions in FIND_BCCH if EMBMS is Active on other sub (to utilize available gaps effectively)
09/11/17   asr      CR2106486 Set TRM granted for TX client, only after checking if its waiting for TRM or not
12/01/18   rv       CR2170840: Fix for GSM TRM priority is lower then 1x priority during ACQUISITION
18/12/17   rv       CR1013937:GSM stuck waiting on TRM without issuing a TRM Req
09/09/16   sn       CR1062272 TRM RESERVE AT during IDLE to transfer
15/12/17   ap       CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
14/07/16   sn       CR1038675 When TRM status is aborted ,release the TRM and inform that MAC about TBF failure
07/06/16   snjv     CR1019245 Do RnN instead of TRM Req(ISR) 
08/09/17   sn       CR1061730 FR43082: TA during PS Procedures- GL1 changes
29/03/16   mn       CR995137 Access tune away support: Allowing other tech's paging at GPRS access
29/10/17   km       CR2133917 GL1 support for L+L
08/08/17   nm       CR2040229 Enable PCH read in 2 phase access for both Idle sub and data sub using GBTA
13/07/16   nm       CR1040020 Fix compilation issues with fEATURE_DUAL_DATA enabled(required for G2L TA FR)
23/05/16   snjv     CR1012850 Release Bands bef clearing Band Reg strct & also extend trm properly
15/12/16   sk       CR1101677 Do not call qta_fw_rf_deinit from sch handler if QBTA is not supported
27/01/17   sk       CR1104513 Fix for the crash during panic reset in IDLE mode and DTM mode
23/02/17   sk       CR2010751 Ensure sleep is not attempted in the same frame where activity skipped due to rude wakeup in SS mode
25/10/16   mn       CR1080861 Don't extended BPLMN activity if FW release is pending 
22/07/16   sn       CR1042914 To perform G-tuneaway in DL Unack mode 
23/06/16   mn       CR1032141 Protecting grm_gbta_info under GBTA lock instead of ISR lock for idle sub
18/05/16   bg       CR1012528 Set g_bplmn_running_state to BPLMN_INACTIVE when released the lock
18/5/16    sm       CR1017901 Added GL1 idle mode QSH events 
18/5/16    sn       CR1007565 PACKET Transfer DTF Events/Handlers Phase1
10/05/16   sk       CR1011560 Move to MULTI-SIM DECOUPLED STATE even if TRM RX_TX_status is COMPLETE_UNLOCK.
14/03/16   sk       CR987445 If we already have lock with the same reason, no need to tell TRM the reason change
16/04/14   akm      CR1004404 Compiller Warning : implicit declaration of function 'garb_intf_gas_is_xpg_coex_victim'
31/03/16   br       CR996055 GL1 to check GBTA active status while aborting idle mode for rach.
16/03/16   ksb      CR987978 Reset the flag from rude wake-up once the BPLMN activity starts such that first burst PCH decode happens 
08/03/16   dg       CR975456 Send tx on to RF after frame adjustment has happened for X2G Handover case
02/03/16   nm       CR982227 Correct the condition to move from TRM_WAIT to IDLE_ABORTED state
29/02/16   js       CR981627 Modify debug crash done in 942512
23/02/16   sk       CR978034 Do not vote for next frame tick for GSM BPLMN when other RAT is in BG TRAFFIC
22/01/16   sp       CR957208 Fetch bands supported for Diversity Chain on the granted device.
12/01/15   db       CR958290 Fix for handling secondary Rx chain allocation due to enabling of Rx Diversity in PTM mode
12/16/15   tsk      CR951099 TRM grant status should consider secondary client id for GBTA during TRM release.
17/11/15   snjv     CR932205 When TRM granted with Active BPLMN pending abort,then dont trigger TRM release
23/11/15   smd      CR942512 Treat GL1_TRM_RETAIN_FOR_ACCESS the same as GL1_TRM_GRANTED when processing decouple command
23/11/15   sp       CR942146 Increment/decrement task count irrespective of reservation being made through wakeup cancellation
10/11/15   smd      CR937168 Waiting for grm_release to complete before entering transfer mode
18/11/15   js       CR924407 Check for FW load status while moving to transfer
18/11/15   cah      CR900214 [WLAN+WAN] Failed in acquiring the Rxd chain in Single SIM Mode during Voice Call Attempt
28/10/15   smd      CR929775 Disable DR temporarily when there is partial grant for RACH
05/11/15   cah      CR934800 call DR disable in task context when PG is received from ACCESS
20/10/15   snjv     CR927604 Update duration for DR Band registration in G2W Ded search
16/10/15   cws      CR919210 use TRM_RX_BEST for all Tx reason
15/10/15   ap       CR923459 GSM logging procedure for Attach seen as CS instead of PS registration
10/09/15   sk       CR899847 GERAN changes to trigger QSH events and handler for debugging
02/09/15   nm       CR897078 Change the reason from Access to TRAFFIC if RACH for CS is received
28/09/15   nm       CR911122 Do not allow GBTA in RLC_UNACK mode of operation
16/09/15   cjl      CR908527 improve system procedures used for TRM access
23/09/15   snjv     CR910568 Crash when Power mon are queued for more than 4 frames
16/09/15   js       CR909022 Check for Power scan end in BG HPLMN suspending
04/09/15   cjl      CR892689 Ensure BPLMN client does not retain TRM chain when aborted
11/09/15   ap       CR905373 GERAN missing pages whilst WCDMA in DATA due to incorrect trm resoursce usage
02/09/15   zf       CR905869 Set trm_status to NULL after grm release
01/09/15   cjl      CR895025 Install callback for CS access priority activity
26/08/15   smd      CR896555 Don't start BPLMN in QTA gap.
26/08/15   zf       CR896636: Use correct sub_reason when override TRM reason
19/08/15   snju     CR893264 Set AsDiv Idle Mode when Beginning BPLMN after suspension
26/08/15   ap       CR896292: GL1 is not releasing trm on reception of MPH_L1_DS_ABORT_IND
20/08/15   zf       CR891607: When handling TRM grant callback for BPLMN, BPLMN should be active
20/08/15   br       CR892274 In page-reorg mode use TRM_DEMOD_PAGE_MAX_SENS(GBTA)trm reason instead of DEMOD_PAGE_MAX_SENS(GTA)
15/08/15   smd      CR891080 Handle GL1_TRM_GRANTED when receiving decouple_command.
20/08/15   ap       CR849948 Race condition between handling MPH_SET_PRIORITY_IND and TRM grant callback
19/08/15   sjw      CR893046: send MPH_ACQUISITION_STATE_IND for all acquisition grants
14/08/15   sjw      CR884733: send MPH_ACQUISITION_STATE_IND when granted for inverted acquisition
10/08/15   cjl      CR887585 Remove and rebook BPLMN when awaiting grant with no frames available
13/08/15   js       CR889763 Correcting state transition when HPLMN is in running state.
13/08/15   fm       CR889703: keep ACQ frame duration aligning with the duration granted by TRM
12/08/15   zf       CR888799: Don't set tx band during multi-SIM panic reset
10/08/15   sp       CR884488: Correctly Resume BCCH List in BPLMN state machine after page decode
06/08/15   zf       CR886776: Don't abort G2W reconf after TRM denied
05/08/15   hd       CR884936: Handle QTA STARTUP and CLEANUP messages in MSGR IF
04/08/15   nm       CR882878 Handle unlock cb when changed the reason to ACCESS in transfer mode
31/07/15   hd       CR882151 Clear trm_request_state when FW is loaded and idle to PTM transition happens
31/07/15   nk       CR882166 Perform TRM release for BPLMN client after BPLMN abort
31/07/15   ip/js    CR859696 Ignore rf_in_deep_sleep_whilst_forced_wakeup when in QTA GAP
31/07/15   cah      CR879928:  IRAT set client state  removed  for QTA
23/07/15   hd       CR878218: Don't reset trm_request_state to NULL when trm status is GRANTED/RETAINED_FOR_ACCESS/
                    CALLBACK_PENDING while handling the TRM request
29/05/15   nm       CR844645 Do not abort idle early while moving from Idle to transfer
14/07/15   sjv      CR872302 Clear Freq. info for TRM req. after bplmn is blocked
21/07/15   cjl      CR876903 Use system procedure types for TRM subreasons
27/07/15   ap       CR880017: BPLMN activity being aborted incorrectly
27/07/15   ap       CR877555: Reduction number of calls to TRM when doing BPLMN
22/07/15   ap       CR877707: Use TRM_RX_BEST/TRM_RX_ANY/TRM_TX_BEST instead of TRM_RXTX_ANY and TRM_RXTX_BEST
22/07/15   mc       CR871205 : Incorrect CXM Channel ID assigned to W2G activities in W+W mode
22/07/15   cah      Re-instate use of IRAT set_client_state use in GL1 for W ID search.
21/07/15   cjl      CR876741 Replace TRM subreasons with system procedure types
16/07/15   ap       CR872131 - Remove Debug Code CR865103
03/23/15   sjv      CR810208 update trm freq info for BPLMN only before TRM req
15/07/15   fan/akh  CR857332 FR19329: WMGR support for rescheduling non-PCH activities
14/07/15   ap       CR868991 The TRM Release for BPLMN is sometimes too late causing missed PCH
09/07/15   cah      CR811835: FR 26893: Band-by-Band OOS and BPLMN Search - GL1
30/06/15   ab       CR869795 : SR-DSDS : Perform GBTA with Transfer SUB FCCH/SCH using COEX.
17/06/15   cjl      CR853555 Use dynamic connected mode reasons for CS/PS traffic
06/07/15   sp       CR862778: Reduce F3 logging in Idle as its impacting QTA timeline
24/06/15   ap       CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR
03/07/15   ap       CR865103 - Add Code to capture when BPLMN has ended but not released RF Chain
30/06/15   ws       CR852519 - Resolve GL1 enum conversion compile warnings
30/06/15   cjl      CR863267 Treat GL1_TRM_RETAINED_FOR_ACCESS as GL1_TRM_GRANTED in *_TRM_WAIT states
26/06/15   nk       CR853484 : Dont update TRM status at GRANT callback if BPLMN is active and trm status is currently set to ABORTING
23/06/15   sp       CR856453: Release BPLMN TRM if PSCAN completes when suspension timer running.
17/06/15   ab       CR847473 : unify usage of gl1_map_gas_id_to_client_id for TX_ANT_DIV/DR_DSDS/RX_DIV/PCHXFER/AS_DIV
26/05/15   nm       CR842963 Release diversity chain too while opening G2X TA/GTA gap
29/05/15   sjv      CR845608: When panic reset is triggered by L1 and is actioned by L1 Task after ISR finishes, Irat SM abort is not handled
11/06/15   hd       CR852314 Do not call TRM release when TRM is denied while waiting on during BPLMN search
08/06/15   akh      CR849840 Wkup mgr cancellation - calculate TRM reservation time correctly on FN wraparound
10/06/15   cjl      CR851285 Fix RACH from idle state machine during RR procedure lock
02/06/15   ar       CR847207 TRM_UNLOCK_BAND_INCOMPATIBLE not handled by gl1
04/06/15   ab       CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
02/06/15   akh      CR841030 Avoid invalid crash from debug code
27/05/15   akh      CR841030 Debug code to check that pwr mon results come back within 4 frames for idle sub
21/05/15   cah      CR841633 Removal of deprecated TRM resource type TRM_RXTX_BEST in GL1
10/04/15   am       CR508867 Panic reset in multi sim mode
18/05/15   ap       CR839699 Clear resource lock state when going Inactive
10/04/15   am       CR508867 Panic reset in multi sim mode
11/05/15   fan/akh  CR834237 Klockworks problem
15/04/15   pa       CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
07/05/15   fan/akh  CR834237 FR19329 Full G wake-up cancellation + code cleanup
06/05/15   cjl      CR831599 Resolve persistent BCCH denial issue
06/05/15   ap       CR832807 GL1 releases TRM whilst doing Acquisition for BPLMN
14/05/15   am       CR824101 Abort ACQ and continue with pending BCCH in find_bcch state
01/05/15   aga      CR831392 Use the appropriate WCDMA client ID gased on static AS_ID mapping
30/04/15   ap       CR811057 FR22272 - TRM Unification  - Logging Changes
28/04/15   ap       CR824511: Gl1 is calculating an incorrect grant duration for bplmn.
                              GL1 is not assigning bplmn abort callback in trm grant cb.
30/04/15   ip       CR799995 FR26290 Sleep when X tech is in connected state
24/04/15   fan      CR796481 Full G wakeup cancellation
21/04/15   sjw      CR822898: removal of deprecated ACQUISITION reasons
16/04/15   smd      CR822202 Dual space data for IRAT and QTA
16/04/15   smd      CR822202 Fix the mutex deadlock in wl1 task and gl1 task
16/04/15   aga      CR822202 TRM interface for QTA
10/04/15   cah      CR819666 Problem in the logic for adding the denied bands to trm_reserve_at and trm_request
03/02/15   cjl      CR790433 Create separate TRM activity for WCDMA reconfirm when needed
08/04/15   cja      CR800650 Optimise irat state machine
06/04/15   zf       CR818766: Resume BCCH list if firmware has already loaded in BPLMN search
03/04/15   ab       CR809082 Do not schedule PWR_MON_EVENT for missed page ,
                    when Transfer Sub is Releasing TBF
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
30/03/15   ap       CR811753 FR22272 - TRM Unification - GL1 changes to support
                                        SysInfo priority inversion during early camping
27/03/15   ap       CR811054 FR22272 - TRM Unification  - Extended Paging with PBR
24/03/15   ap       CR811053:FR22272 - TRM Unification  - Access and Traffic attempts
23/03/15   ap       CR811046:FR22272 - TRM Unification  - Removal of _HI reasons/UNLOCK_IMMEDIATELY
25/03/15   ap       CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
24/03/15   sjw      CR812407: update for new MPH_SET_PRIORITY_IND payload
12/03/15   pa       CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
25/02/14   cah      CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good serving cell conditions
17/02/15   br       CR796211 QTA:Don't  start QTA from GL1, if gl1 deactivation is going on.
24/02/15   js       CR749012: GL1 changes for LTE removal
20/02/15   jj       CR 798186  IRAT enum duplication clean up
15/01/15   cah      CR781585: GL1 registers invalid TRM band for TD booking.
20/02/15   js       CR693771: W+T removal. Resolving compilation errors
13/02/15   pa       CR792105: Allow either GBTA or IRAT.
08/12/14   br       CR767658 Passing undefined gas_id  as  a parameter to macro to print GBTA F3
03/02/15   zf       CR780458: Adding L1 task grm_release handling for callback
30/01/15   nk       CR787609 Defer TS change if there's an activity still running
29/12/14   zf       CR776270: When trm_status is TRM_CALLBACK_PENDING, allow GL1 change from IDLE to DECOUPLED state.
17/02/15   cah      CR777305 Replace depricated TRM type TRM_DENIED with TRM_DENIAL
17/02/15   gk       CR728521 Choose TRM_DEMOD_PAGE as the global over ride reason, if QTA is allowed
13/02/15   pa       CR756156:G2X RF device handling using TRM apis.
23/01/15   npt      CR697236 Mainline FEATURE_GERAN_VSTMR
21/01/15   npt      CR783249 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled
21/01/15   pa       CR758792:RxD support for multi-sim.
19/01/15   mc       CR769958 : Removing Deprecated TRM APIs in GERAN
12/01/15   sjv      CR756154 Enhance Band Registration for Activities
15/12/14   ws       CR771244 GL1: Compile warnings for LTE,1X,GERAN build flavour
07/01/15   sp       CR764166: Ensure GRM status is granted only to allow release of lock in idle/decouple.
05/01/15   npt      CR770381 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled
29/12/14   am       CR774723 Avoid unnecessary wake-up on SYS_INFO_REQ
16/12/14   sk       CR770126 Ensure GL1 doesnt request for lock every frame for BPLMN when LTE is holding the chain with Channel maintenance reason
22/12/14   gk       CR746159 Incase of we are inside QTA, and time slot change is required, we should make time_slot_change_pending to TRUE
22/12/14   gk       CR725865 Avoid running idle isr while in L1_TRANSFER_COMMAND_WAIT_FOR_TRM.
23/12/14   ggu      CR772195: No need to continue check gbta_allowed status when there is no any G sub is under TRANS state
22/12/14   pa       CR773689: Set RF device to Invalid on RF exit.
21/12/14   zf       CR774120: After TRM granted, it should transit from WAIT_FOR_TRM to IDLE_ABORTED state.
16/12/14   sk       CR770126 Ensure GL1 doesnt request for lock every frame for BPLMN when LTE is holding the chain with Channel maintenance reason
22/12/14   gk       CR746159 Incase of we are inside QTA, and time slot change is required, we should make time_slot_change_pending to TRUE
22/12/14   gk       CR725865 Avoid running idle isr while in L1_TRANSFER_COMMAND_WAIT_FOR_TRM.
19/12/14   nm       CR761630 check the bcch in trans seq state also for allowing gbta
12/12/14   zf       CR770728: Adding function to call in task GRM release without requesting GL1 lock
12/04/14   gk       CR716499 Choose TRM_RESELECT as override reason, if G+G is active.
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
09/12/14   sp       CR765228: Start SCE when moving from decoupled to Idle.
08/12/14   ap       CR726899 Update Panic Reset mechanism - remove dependency on TRM
10/12/14   ws       CR768376 Q6 compile warning fixes
05/12/14   pa       CR767082:Use NV items to control Idle mode RxD.
04/12/14   sjv      CR766784 Backing out CR759568
03/12/14   npt      CR759467 Fix compiler warnings
05/09/14   ws       CR717967 GSM did not indicated TRM Lock reason correctly in case of extended page
27/11/14   nm       CR698030 Use correct trm client id to query other sub trm reason when pch in transfer is enabled
21/11/14   pg       CR760899 Enable Idle RxD for Thor single-SIM
11/11/14   ap       CR752479 GERAN will inform GRR everytime TRM is denied attempting BPLMN
18/11/14   sj/ap/ch CR757734 When transitioning to transfer from Idle, request TRM after Idle abort finishes, and deact FW/RF
13/11/14   am       CR754217 Keep GRM internal reason aligned with latest TRM function
10/11/14   sk       CR748901 Use TRM_TRAFFIC reason for TRM reservation when in mm ndrx mode
14/11/14   cah      CR751917: PDT crash,  passing undefined gas_id to macro returning client ID of sub in Xfer
10/11/14   ap       CR753835 Handling race conditions that occur due to receiving RA abort followed by GRR msg
05/11/14   js       CR750861 Suspend trm relase while aborting power scan in BPLMN
10/11/14   aga      CR751034 Release TRM lock when processing L1_DECOUPLE_MULTI_SIM_COMMAND
07/10/14   nm       CR734969 Release trm and set status to TRM_NULL in l1_isr_multi_sim_decoupled if moving to idle & TRM_CALLBACK_PENDING
11/10/14   js       CR719550 : Dont Reject DL TBF config when QTA is ongoing.
31/10/14   ws       CR749027 Compile error fixes for TABASCO modem
30/10/14   pa       CR748275: In DRDSDS (Transfer Mode): allow IRAT if band registration is successful.
30/10/14   cws      CR737486  Update req_n_notify_reason properly and do retain lock check after local reason is updated
14/10/14   mko      CR738702 Wait for QTA clean-up before moving to decouple state
29/10/14   ab       CR745429 - Featurization correction for PCHXFER for SS / CR merges.
23/10/14   sjv/aga  CR744425 using correct gas_id and TRM reasons in gl1_is_granted_geran_sub_doing_reselection
22/10/14   pg       CR744391 Tabasco compilation errors
21/10/14   sjv      CR743461 Reset persistent denial when exiting Idle mode, and start persistent denial only in idle mode
21/10/14   zf       CR741702: Avoid deedlock when two G subs acquire both TRM and GL1 TRM lock
20/10/14   ap       CR742915 DRDSDS: GL1 is holding non TX device when moving to Transfer mode from Idle
20/10/14   ap       CR742918: Corrected 719290 GBTA: Sub in transfer is denying RF resource to sub in Idle
09/10/14   ap       CR719290: GBTA: Sub in transfer is denying RF resource to sub in Idle
17/10/14   ssh      CR737668: Reset ISR overrun condition if leaving or entering idle after rude wake up
17/10/14   cgc      CR742912 add missing function call l1_isr_G2L_idle_trigger() to l1_isr_multi_sim_idle()
15/10/14   sjv      CR738845 Updating band calculation for G2T(use correct index) and G2L in Idle mode for TRM requests
14/10/14   pa       CR738110 Store requested TRM reason in the local structure on Request and Notify/Enhanced.
09/10/14   ap       CR719290: GBTA: Sub in transfer is denying RF resource to sub in Idle
09/10/14   fan      CR735456 Remove ISR overrun check during wakeup
07/10/14   sjv      CR734254  G2L rf devices retrieval for primary and diversity
09/10/14   ws       CR 736562 P1 klocwork fixes
06/10/14   pg       CR734405 Thor compilation errors
03/10/14   aga      CR732455 Changes required to tune away framework for TH 1.0 single sim build
03/10/14   ap       CR730903 Handling race conditions of receiving RA req following RA abort and select specific BCCH req in decoupled state
03/10/14   js       CR731597 Removing call in task trm release TRM while aborting RACH from dual sim decoupled.
26/09/14   zf       CR730930: Cancel pending GFW commands when RF does not wakeup before next frame to mitigate TRM/RF concurrency delay issue.
24/09/14   cah      CR729408: Updates to reduce number of TRM API calls in Gl1.
23/09/14   dp       CR727568 get trm band using correct id/reconf idx
22/09/14   cah       CR723144: Bolt 2.0 DRDSDS - W+G - G2G interband ncell monitor activity denied while in W+G FC mode
18/09/14   cah      CR720203: Invalid device ID being passed to RFSW in enter mode command.
15/09/14   cs       CR724334 Use non-deprecated TD band conversion function tdrm_get_band()
12/09/14   nm       CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
11/09/14   ws       CR723099 Resolve GL1 compile warnings
02/09/14   br       CR716713 Block TRM access during BPLMN if x-rat is doing acquisition
11/09/14   am       CR677361 Addressing KW GL1 P1 errors
07/09/14   js       CR711466:Reset the main command after drx off in W2G handover.
20/05/14   ap       CR668029 Handling Serving Cell BCCH reads during Early Camp when X Rat is in Data Traffic
05/09/14   ap       CR687551 Incorrect calculation for bplmn activity
05/09/14   js       CR717875 Detect ISR over run whenever RF is in deep sleep
19/03/14   sk       CR625317 updating the max required frames correctly so that BPLMN state machine and frame manager are in sync.
02/09/14   gk       CR687919 Block the TRM reservation each frame in SGLTE Mode and both RFM devices are already granted
03/09/14   sp       CR715190: Pass correct gas_id to stop/start PT sub trans tick in GBTA
28/08/14   sjv      CR717166 pending activities are organized in an array for trm band release to avoid overwrite in case of diff. activities
31/07/14   xz       CR697883 Not allow GBTA if current idle frame is doing TDS IRAT in xfer mode
28/08/14   sjv      CR717166 pending activities are organized in an array for trm band release to avoid overwrite in case of diff. activities
31/07/14   xz       CR697883 Not allow GBTA if current idle frame is doing TDS IRAT in xfer mode
22/08/14   sk       CR713509 Ensure TRM lock released intime for other SUB's activity
22/05/14   sk       CR668644 Ensure to check the secondary chain grant status before blocking BPLMN
23/08/14   sk       CR709663: Ensure extended page block is read inside QTA gap during WCDMA signalling
28/08/14   sjv      CR717166 pending activities are organized in an array for trm band release to avoid overwrite in case of diff. activities
31/07/14   xz       CR697883 Not allow GBTA if current idle frame is doing TDS IRAT in xfer mode
26/08/14   pa       CR715814 IFS measured client id is not updated from GL1 before passing to TRM
29/08/14   cah      CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release1
23/08/14   sp       CR692209 Use Hyperframe macro at conditional checks.
21/08/14   cs       CR707384 Allow reserve reason for other G sub check
19/08/14   sjv      CR711666 measured client id is not updated from GL1 before passing to TRM for band registration for IRAT and IFS
19/08/14   cah      CR711332 RF device not updated when switching from dev 2 to dev 0 for RACH
15/08/14   ap       CR702609 When MMS initiated need to simulate CS call TRM flow
08/08/14   ka        CR706866 FR18739: DR-DSDS RxD Idle, part 2 - new TRM clients
08/09/14   cah      CR704964: not handling TRM request pending correctly when attempting to change TRM reason to access urgent.
06/08/14   ap       CR705487 Not handling ACCESS Command in dual sim idle mode
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
06/08/14   ka       CR699964 Implement Idle RXD support in GL1
05/08/14   pa           CR704815: Fill band info during PTM Ncell activity.
05/08/14   ap       CR704010 Handle stop gsm mode in decoupled state
04/08/14   nm       CR664966 Fall through b/w case L1_TRANSFER_COMMAND_WAIT_FOR_FW & L1_TRANSFER_COMMAND_ABORTING_IDLE if FW is unloaded
04/08/14   nm       CR663403 Use TRM_BROADCAST_ACCESS trm reason in page reorg mode instead of DEMOD_PAGE_MAX_SENS
31/07/14   cos      CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
30/07/14   ssh      CR701245: Avoid running an adjoining activity after rude wake up if the FW/RF is being initialized in that frame
30/07/14   dp       CR701987 back out CR687605 & CR694913, which were causing PLMN search issues
29/07/14   ap       CR695234: Changes for GL1's New Rach interface
28/07/14   ssh      CR700687: Handle X2G start up from warm up ISR
13/06/14   am       CR679910 NULL check while accessing pTransfer_data
24/06/14   dv/cah   CR697365 ensure wcdma_srch_in_progress flag is reset
16/06/14   nk       CR678832 Override TRM reason to QTA compatible reason one
22/07/14   ss       CR696494: function grm_check_idle_sc_activity_ok should have activity_can_run initiazed to TRUE
22/07/14   sp       CR687317: After Max(10) failure attempts to acquire TRM for DL_TBF_CONFIG, clear the RANE by releasing TRM.
22/07/14   cah      CR697586  trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
09/07/14   xz       CR690934 Did not release trm agian if TRM denied us in grant callback.
07/16/14   sc       CR694913 Re-work of CR687605
10/07/14   js       CR593074- ROTA/QBTA Feature implementation
16/07/14   aga      CR695332 PCH scheduling  is not initiated, GBTA is not stated
16/07/14   cah      CR692374 MODEM - wl1trm.c:918   WL1_TRM: No band matched for freq: 38050
11/07/14   sjw      Standardise use of gl1_is_geran_tuneaway_allowed()
07/03/14   sc       CR687605 Add check if X-RAT has TRM lock before querying TRM reason
04/07/14   cs       CR688003 GRM functions moved to gpl1_grm_intf.c
02/07/14   mc       CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
20/06/14   ws       Added support for grm_get_data_sub_priority() and grm_set_data_sub_priority()
30/05/14   ws       CR657674 Data + MMS Phase 1
27/06/14   cs       CR685966 Add DR INFO async callback support
20/05/14   smd      CR662851: Release and request RF device again for RACH in SGLTE+G
24/06/14   sjw      CR684785 APS enable/disable support via GRM wrapper
24/06/14   cs       CR684714 Add back in functionality removed for initial updated TRM API support
12/05/14   dv       CR658307 Crash due to no release of lock in QTA when in ext-paging & combined channel
19/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
12/06/13   cs/dv    Updated TRM API for retain lock usage
03/06/14   cs/ch/dv Updates for initial development of new TRM API support
29/05/14   cs/pa    CR660831 Use new trm_status definitions specific to GL1
22/05/14   cah      Bolt 2.0: FEATURE_GERAN_DRDSDS
21/05/14   ssh      CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub
20/05/14   smd      CR665221 In SGLTE+G, don't abort CS call if L/T BPLMN is done.
19/05/14   ap       CR664926 Back out the change done by CR-650629
12/05/14   cja      CR662836 Add support for qta_cleanup_pending to Bolt.
08/05/14   sp       CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
08/05/14   br       CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
08/05/14   nk       CR660831: GSM Layer 1 compares wrong enum values for internal TRM_DENIAL status. Change TRM_DENIAL enum to TRM_NOT_GRANTED to aoid any confusion going forward.
02/05/14   ka       CR658525 Only hold TRM if in extended paging and on combined channel with block in current MF
29/04/14   gk       CR653811 Do TRM Extension for BCCH reading during HPLMN Running, while taking care of PCH schedule
28/04/14   ap       CR651176 RLC timer expiry after MPH_DEVICE_MODE_IND with cause GERAN_DEVICE_MODE_CAUSE_PLMN
25/04/14   dg       CR654695 Back-out CR649702 to avoid regression
24/04/14   ggu      CR650629 When enable PBR feature, make BCCH SI Reading as TRM_DEMOD_PAGE to treat collision with another G paging
21/04/14   fm       CR650747 Update trm_grant_status when being granted TRM
17/04/14   am       CR649702 Use max TRM priority during EARLY CAMPING if GRR has set to use MAX ACQUISITION priority
11/04/14   cja      CR644441 Bolt: Preload script for QTA
11/04/14   pg       CR645875 Fix Bolt multisim compialtion errors
04/04/14   cah      CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
03/04/14   ab       CR639723: Notify PS - to register ARFCN/Freq Id when starttime has elapsed when TRM lock is not acquired Single Shot
07/04/14   br       CR643145 TRM extending till next ccch activity without calling the retain lock causing page failures on other sub
07/04/14   br       CR643145 Back out for release
21/02/14   sp       CR635810: Don't release TRM lock for access when the lock is granted after request_and_notify(), improve CR/620698
08/04/14   cs       CR638933 Allow equal priority event to be booked when suspend is set
08/04/14   sp       CR639742: Update Idle activity to QTA reason from multisim trm manager
28/03/14   ssh      CR621230: Use extended warm up time in the event of G+G(+G) wake up collisions to avoid warm up ISR overrun
28/03/14   ap       CR630849:TRM gives incorrect device for GSM in QTA gap
23/03/14   sjw      CR637080:use new interface for BPLMN failures
22/03/14   ap       CR636650 When Sub1 does G2x measurements & Sub2 in data transfer G Sub1 misses many PCH decode
20/03/14   zf       CR636451 Do not reserve TRM with invalid reason.
20/03/14   dv       CR636603 G+G mode:ensure acquisition gets big enough gap during TBF
18/03/14   ap       CR629844 SGLTE BPLMN support when G1 in PS call in G+G - PS Hopping
21/03/14   sjw      CR635746: reduce default unlock time for acquisition inversion
19/03/14   ws       CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
18/03/14   ap       CR614949 FR 19574: Performing LTE/TDS measurements while GERAN is in connected mode using the second WTR
11/03/14   zc       CR629234 Prevent concurrent calls to grm_request from task and ISR
13/03/14   ssh      CR631223: Force rude wake for all idle subs when transfer starts in G+G(+G) mode
12/03/14   ssh      CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
08/03/14   am       CR624825 If TRM is retained for TBF assignment then TRM release should happen on TBF release
06/03/14   br       CR626780, RACE condition between TRANSFER G sub and IDLE G sub, issuing SCH in G2x gap.
07/03/14   dv       CR628324 Short term QTA fix for L2G BGTraffic transition
07/03/14   ssh      CR628028 Invalid TRM reason passed to TRM introduced as part of the FEATURE_PBR_MARKOV development
06/03/14   dv       CR623655 Ensure RF cleanup scripts are built
06/03/14   ap       CR626773 correct trm reason when QTA is possible
19/02/14   sk       CR602282 Ensure Priority inversion applied after the deferred wake up
28/02/14   ap       CR624060 Further changes to FEATURE_PBR_MARKOV applied
26/02/14   br       CR622897 Don't do Reserve at from idle isr in case of QTA support enabled, if trm is already granted.
28/02/14   sjw      CR620569: adaptive unlock callback for acquisition
28/02/14   sp       CR595548: GPRS suspension while other Idle Subs Acquisition.
25/02/14   ss       CR621328 Only change TRM priority in grm_request if trm already granted.
25/02/14   sk       CR614910 Update G BPLMN handling inline with DSDA-OMRD implementation
21/02/14   ap       CR620029 Ensure FEATURE_PBR_MARKOV is correctly applied
19/02/14   ap       CR600310 GSM L1 to send channel Id to GSM FW corresponding to the primary RF device
2/17/14    ab       CR612460 Fix race condition for power monitor update
09/01/14   zf       CR598637: DRX only updates TRM booking if DRX activity is matching with the TRM booking
24/01/14   mko      CR593859:Extend the TRM if SI read is in less than 11 frames after the paging in reselection state(HDR in BG Traffic)
13/02/14   zc       CR615778: Keep track of the AS_ID used for the QTA gap
17/01/13   sk       CR600574 Ensure GBTA state is reset from Active if TRM released/reserved without the activity being run
06/02/14   ssh      CR611563: Incorrect check for the ISR overrun detection in the non warm up ISR case.
15/01/14   nk       CR598060 L1 task and ISR stuck in deadlock as both are trying to do fw/rf load
29/01/14   zc       CR598805 SGLTE Idle hopping changes
28/01/14   zc       CR608458 Retain the lock in a CS call and Featurized Idle Hopping
17/01/14   zc       CR601634 Add a frame to TRM Unlock_by, if TRM granted but fw not loaded
16/01/14   ka       CR600056 Ensure that if in qta gap we have TRM from other tech
07/01/13   sk       CR596150 Update TRM RESLECT reservation to QTA compatible one if WCDMA is in QTA mode
06/01/14   sp       CR596088: Reset GBTA State if applicable when Idle Sub reserves/releases next activity.
06/01/13   sk       CR591280 Ensure DS Abort doesnt get triggered immediately when reselection ongoing
06/01/14   nm       CR574905 Set the FW load status correctly when Reserve At is in progress
06/01/13   sk       CR592996 Ensure GSTMR is not slammed inside the QTA gap
27/12/13   nm       CR593419 Dont call gl1_hw_multi_sim_deact if FW is not loaded
13/12/13   nk       CR588331 Don't request TRM at warmup if TRM is granted or retained for access
19/12/13   js       CR581840 IDLE SUBs releasing TRM immediately after GRANT leading to unnecessary GTA Gaps
11/12/13   ap       CR586661 GPRS suspension during SI read after W2G reselection
17/12/13   nm       CR587488 ncell bcch to use trm reason as demod page when other sub in transfer
17/12/13   sk       CR586741 Update TRM reservation to QTA compatible one if WCDMA is in QTA mode
16/12/13   dv       CR569122 High missed page rate due to W ID search
11/12/13   mko      CR588074 removed calling idle mode isr in L1 is in TRANSFER_COMMAND_WAIT_FOR_TRM state if TRM is granted.
11/12/13   sk       CR588079 Ensure Transfer doesnt start when TRM release pending in the task
12/01/14   sjw      CR592154: use new TRM reason for ACQUISITION_MAX
09/01/13   ws       CR591282 Fixed regression of GGTA/GTA in previous checkin
09/01/14   sjw      CR596709: created new grm_release_or_exchange() function
08/01/14   cah      CR597414 fix for missing handling of vstmr deregister signal.
18/12/13   cgc      CR591282 Enable LTE_GENERIC_QTA changes for setting qta_in_progress
02/01/14   ap       CR592157 L2G redirection failure on Sub1 when Sub2 in GSM idle camped
18/11/13   dv       CR572735 Ensure that when G is high prio SUB not use PAGE_INV for TRM
24/12/13   ap       CR592011: FR 17482: Page Blocking Management in Multi-SIM Single Radio Devices amendment
18/12/13   sjw      CR590908: correction to time calculation in trm_acquisition_unlock()
16/12/13   ap       CR590162: FR 15265: Temporarily remove the DSDA to DSDS dynamic mode switching from GL1
14/12/13   sjw      CR589502: Install different unlock callback for change of acq priority
13/12/13   ap       CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
13/12/13   ka       CR580906: Remove an extra frame from the unlockby frames
07/12/13   sk       CR586608 Removing the overhead frame removal opt changes for RACH procedures
04/12/13   ap       CR584084 Addressing WCDMA reconfirmation search whilst GERAN in Transfer
04/12/13   aga      CR549390 G2W_TA IRAT Enable W ID search and reconf in case the other G sub is in data call
03/12/13   sk       CR580201 Ensure QTA flag is not set when W has not initialized the QTA gap
03/12/13   js       CR574803 TRM extended for Ncell Acquisitions after PCH decode leading to missed page on next SUB
01/12/13   sk       CR583361 Backout the CR549390
29/11/13   js       CR576778 Add check in L1_MULTI_SIM_IDLE_COMMAND handling to avoid the race condition
27/11/13   aga      CR549390 G2W_TA IRAT Enable W ID search and reconf in case the other G sub is in data call
26/11/13   dv       CR580909 Ensure of a 'empty' QTA gap the pending cleanup flag is set
22/11/13   zf       CR581864: Retain TRM lock for transfer mode.
21/11/13   js       CR579116: Adding demod page inv to grm paging reason
20/11/13   zf       CR579309: Init FW when TRM granted.
18/11/13   sp       CR543075: BPLMN optimizations - to handle Si3/Si4 colliding with either sub pagings.
19/11/13   ssh/ip   CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
18/11/13   sp       CR576675 Reset the main command after calling off DRX
18/11/13   dv       CR572735 Ensure that when G is high prio SUB not use PAGE_INV for TRM
18/11/13   sk       CR573622 Ensure when in all paging blocks mode QTA reservations/requests are not made
15/11/13   js       CR574385 GSM losing Acquisition due to denail of chain by HDR
15/11/13   js       CR572966:Reselections to the Ncell are still failing as SUB2 is not able to decode all the SIs leading to SYS_INFO_TIMER expiry
14/11/13   zc       CR569943:Use correct resource for trm_freq_request
15/11/13   nm       CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
13/11/13   js       CR569507:Missed pages on sub2 when sub1 is in power scan mode
11/11/13   sk       CR573517 Ensure correct sys mode used by GL1 Arb while giving coex priority
08/11/13   sk       CR572873 Ensure non QTA activities doesnt run when W is in QTA mode
30/10/13   cs       CR569008 Correction for num_idle_subs calculation in SS mode
10/10/13   nk       CR557171 Ensure release confirmation is sent to MAC
21/10/13   sk       CR563878 Ensure TRM_DEMOD_PAGE_INV is not used to TRM_HI client
05/12/13   sjw      CR583608: Use TRM lock acquired using inter-RAT exchange
22/11/13   zf       CR581864: Retain TRM lock for transfer mode.
12/03/13   js       CR574385 GSM losing Acquisition due to denail of chain by HDR
12/02/13   sk       CR573517 Ensure correct sys mode used by GL1 Arb while giving coex priority
28/11/13   sjw      CR578873: Priority Inversion Management for Multi-SIM (PIMMS)
13/11/13   js       CR569507:Missed pages on sub2 when sub1 is in power scan mode
11/20/13   nk       CR557171 Ensure release confirmation is sent to MAC
19/10/13   sk       CR563877 Ensure BCCH reads are given priority over PCH when doing reselection and consider early wakeup time into TRM reservation
15/11/13   ap       CR575548 FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS
13/11/13   ap       CR562878 FR 17482: Page Blocking Management in Multi-SIM Single Radio Devices
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
19/10/13   sk       CR563877 Ensure BCCH reads are given priority over PCH when doing reselection and consider early wakeup time into TRM reservation
17/10/13   sp       CR556623: Initialize BPLMN activity for frames with gap_to_next_ccch activity
10/10/13   js       CR555417 Dont reset the main command when calling off DRX
11/10/13   sk       CR556552 Ensure idle sub doesn�t invert the priority when other sub using global override for reselection in QTA scenario.
09/10/13   sk       CR432888 Adjust BPLMN frames for acquisition of BCCH in DSDS mode
10/09/13   ss       CR540595 Release TRM one frame earlier than the frame in which TRM is required by the other sub
20/09/13   js       CR522426 Multi SIM CBCH priority decision to be taken only when we have more than one sub active
01/08/13   ss       CR522250 Send START_IDLE_MODE_CNF in dual_sim_idle_mode for L1_IDLE_COMMAND
05/10/13   sk       CR554665 Backout the power opt temporarily(CR537690)
05/10/13   sk       CR554659 Dont set DRX off when idle aborted with L1_DECOUPLE_MULTI_SIM_IDLE_COMMAND
16/08/13   gk       CR530309:In case of back-to-back PLMN search, no need to wait for 3 frames for the FW to be setup properly, if the PLMN state is NULL
05/10/13   sk       CR554393 Ensure RACH doesnt get processed inside the QTA gap
05/09/13   zf       CR537883: Deactivate DS when G deactivation
04/10/13   sp       CR553745: BPLMN Optimizations
04/10/13   sjw      CR546922: allow BPLMN during WCDMA acquisition
03/10/13   sk       CR553207 Set DRX off when idle is aborted for RACH and ensure trm reserve for Access is not made when going to sleep
02/10/13   js       CR545531:Partial QBTA for extended paging
22/08/13   aga      CR532822: updated for DSDA grm_paging_inversion_check() is explicitly checking for GRANTED_CHAIN0
01/10/13   sk       CR552259 CR sync from 1.0/1.1
27/09/13   cgc      CR523552 add support for FR 16939 L+DSDS LTA/QTA qta_in_progress LTE_GENERIC_QTA
24/09/13   zf       CR537690: Reduce G wakeup timeline for MS mode
09/09/13   ab       CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
16/09/13   cs       CR503042 Updated XO RGS handling for Triton
11/09/13   sk       CR499860:Page Inversion Check to be called only if the TRM request reason is Page
11/09/13   sk       CR540607 Apply TRM reason override for page reorg if GBTA is allowed
05/07/13   js       CR507521:The suspension timer running indefinitely due to back to back HPLMN search issued by upper layer.
02/09/13   sk       CR537258 Ensure LTA activity doesnt run when W is inside the QTA gap
02/09/13   nm       propagation from Nikel -CR511322 Reset trm request state if RACH is aborted
29/08/13   sk       CR536136 Ensure TRM requests for RXTX_BEST_SMODE resource doesnt happen Standby cases
26/08/13   gk       CR484897:W2G handover using exchange of TRM lock
26/08/13   sk       CR534005 Remove _HI reason codes from L1-TRM mapping table
22/08/13   sp       CR529521: Move from req_notify_enh_data static to local
10/09/13   cs       CR532298 Integrate all wakeup-mgr changes for Multi-SIM
20/08/13   sk       CR530970 Ensure GBTA works properly in DSDS config
20/08/13   ab       CR529794 GBTA Enhancement: facilitate PAGE decode on Idle SUB
                     when Other SUB is in GBTA and doing NCell decode
15/08/13   sk       CR524039 GBTA bringup changes
07/08/13   gk       CR509196 :Abort the previous power scan, in case of reception of back-to-back power scan request
01/08/13   sk       CR521884 Priority inversion not working in different DRX combination
15/07/13   sp       CR508123: Update panic reset to use multi-sim data space
01/08/13   sk       CR520888 Priority inversion not working in WGG combination
29/07/13   sk       CR519663 Partial QBTA bringup changes
18/07/13   aga      CR512422  avoid blocking BPLMN scan in DSDA
12/07/13   cs       CR509747 Apply frequency drift to TRM_DENIED SUB(s)
05/07/13   sk       CR509202 Priority inversion and alternate page skipping changes
26/06/13   cs       Major Triton TSTS Syncup
28/05/13   ap       CR461268: DSDS XO management + async time drift problem
08/04/13   ab       CR472222 - Not to allow RACH access for non-urgent (not MO call) cases
                    during GTA gap.
23/05/13   sjw      CR385159 Handle repeated abort callbacks during panic reset
21/05/13   am       CR454475 Block BPLMN search when QTA is enabled
21/05/13   am       CR436152 Check if GSTMR is pending while making TRM reservation if so consider offset accordingly
21/05/13   am       CR447573 NULL check for pIdle_data in l1_get_global_override_trm_reason
21/05/13   cs       TSTS Updates
17/05/13   pjr      CR489024 Set extension flags for QTA only in DSDS
15/04/13   am       CR473834 Avoid TRM release while FW loading is in progress
13/05/13   sv       CR460409: Allow BPLMN state machine to run when FW is already loaded.
27/03/13   am/ap    CR432888 Adjust BPLMN frames for acquisition of BCCH in DSDS mode
02/05/13   zf       CR480969: Fix G+G DSDS camping
01/04/13   am       CR469877 - CR413664+CR428401+CR415240+CR415240
26/04/13   zf       CR477992: Fix G2W IRAT issue
25/04/13   pjr      CR475805 removed gl1_msg_set_multi_sim_mode from l1_init_multi_sim_data
19/04/13   mc       CR477448 DSDS: QTA Support : Initial code changes & Triton Bring-Up - fix build errors
09/04/13   ap       CR460433 DSDS Crash in WFW as G released TRM without moving GFW to Idle
24/03/13   ws       CR466973 - Fix F3 msgs for DSDx
25/02/13   ap       CR 450915 DSDS: Not decoding PCH as per DRX
21/02/13   ap       CR 451556 DSDS: Bad AFC/timing on PCH decode when long TBF start time is given
12/02/13   ap       CR451891 DSDS SUB2 OOS and stuck in TRM wait for a very long time
09/02/13   ap       CR447817 DSDS Prevent sleep during W2G Blind CCO
07/02/13   ap       CR449859 Always enter GFW state MEAS_ACTIVE for gonly sub else
                             enter GFW ACTIVE state
07/02/13   ap        CR449348 DSDS W/G+G IRAT MO Voice: HO from W2G requires attention
08/02/12   ab        DSDS CR446084 - making sure IA with Poll RRBP tx by cancelling Pending Activity.
04/02/12   ap        DSDS CR448369  W/G+G DSDS:UE stuck in W2G HO and not recovered
08/02/12   mc        DSDS CR332154 Ensure that a PCH following a BCCH gets decoded upon a missed BCCH block
04/09/12   sk        CR395504 Sending the BCCH LIST decode cnf back to GRR when GL1 receives LIST REQ and ABORT within a frametick
23/10/12   sjw       DSDS CR309450: enhancement to G+G paging collision algorithms
22/10/12   sjw       DSDS CR349280: deprecate use of acquisition and SCI bitmaps within GRM
24/08/11   kb        CR302696 - Added initialization functions to initialize dual sim data values
28/02/13   pjr       CR457802 Moved initialisation of gl1_msg_multi_sim_mode to gsm_l1_fn_init
14/02/13   pjr       CR452272 added support to set multisim mode at activation time
18/12/12   og        CR424373. Don't send a tx config command when leaving idle
                     during deactivation.
30/1/12    ws        CR447197 - GERAN DSDA uninitialised rex_critical section in trm_enh_callback()
18/12/11   nt        DSDS CR327737 C+G deep sleep issue
05/10/11   ap        CR 308923: DSDS GL1 Task can be stuck waiting on signal after MPH_DS_L1_ABORT_IND
13/10/11   cs        CR312346 Use RETAIN_FOR_ACCESS to avoid packet activity with no firmware
02/09/11   nt        DSDS CR304188 Ensure that the configuration_is_GplusG flag is always cleared
                     upon DSDS deactivation
29/06/11   nt        DSDS CR289633 Ensure that the CNF is sent after suspension state in BPLMN
16/05/11   sjw       Do not wait for ISR to fire if panic reset caused by stall
04/04/11   sjw       Avoid rex_wait() calls in ISR context for DSDS panic reset
31/03/11   nt        CR281165 - Ensure that GSM is awake when panic reset begins
02/03/11   njt       DSDS CR276931 - BPLMN suspension changes
22/02/11   njt       DSDS CR276186 - only rebook during BPLMN if TRM status is NULL
06/04/11   nt        DSDS CR279823 - Change to ensure no race between ISR and task for access
16/02/11   sjw       DSDS - removal of FEATURE_MULTI_SIM_WCDMA_TUNEAWAY
11/02/11   sjw       DSDS CR272208 - avoid floating point operations in TRM callbacks
10/02/11   sjw       DSDS CR255895 - use timetick_get_safe() in ISR context
11/01/11   nt        DSDS CR269112 - Fix to manage sleep for DSDS scenarios
18/01/11   ap        DSDS CR270377 SYNC SIM is disabling L1 ISR while ASYNC SIM is still
                     active when consecutive start/stop GSM mode req
09/12/10   ap        DSDS CR261742 stack did not respond to CM_STOP_MODE_REQ.
02/12/10   nt        DSDS CR266320 Sleep crash during panic reset.
05/11/10   sjw       DSDS CR262550 Max out the GRM inversion bitmap for acquisition
22/10/10   nt        DSDS CR255013 Clear down TRM status for both subs on panic reset
13/09/10   ap        DSDS CR259184 Async CCCH getting scheduled when FW is about to be released
10/09/10   nt        DSDS CR254080 L1 is incorrectly programming W's sleep controller
13/08/10   nt        DSDS - CR249783 fix to prevent premature release
09/08/10   nt        DSDS - Addition to panic reset handling
03/08/10   nt        DSDS - CR249399 mdsp sleep crash
28/07/10   nt        DSDS - Fixes for GplusG mode
21/07/10   nt        DSDS - TCXO management for dual sim targets
20/07/10   nt        DSDS - Enable GpluG operating mode
09/07/10   nt        Move ASYNC to SYNC sim swap to ISR context
30/06/10   sjw       Introduction of GRM framework
23/06/05   nt        Always ensure that upon deactivating we trm_release to remove bookings
15/07/05   nt        Prevent start of drx upon return from L1_DECOUPLE_MULTI_SIM_IDLE_COMMAND
02/07/05   nt        CR240111 - BPLMN needs to book in next page after giving up
12/05/10   nt        CR232162 - DS delayed BPLMN abort conf's until fully completed
08/05/10   nt        CR237159 - DS decouple IDLE mode upon rach retry
05/05/10   nt        CR236680 - Upon early abort of BPLMN with no FW don't tick drivers
26/04/10   nt        CR234911 - BPLMN abort not getting handled correctly
13/04/10   nt        CR233867: Reset the BPLMN state when receiving a powerscan/bcch_decode
12/04/10   nt        CR232391 - Add the changes to handle iRAT MPH_RECONNECT_REQ
07/04/10   nt        CR233429 - DS turn off DRX when moving to dedicated for iRAT HO.
23/03/10   nt        Changes to prevent BPLMN mechanism from getting into the wrong state
===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "ms.h"
#include "l1.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_utils.h"

#include "gpl1_gprs_utils.h"

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "geran_multi_sim.h"
#include "geran_multi_sim_api.h"
#include "gpl1_grm_intf.h"
#include "gl1_hw_sleep_ctl.h"
#include "gl1_hwi.h"

#include "l1_os.h"
#include "gpl1_gprs_isr.h"
#include "l1_drx.h"
#include "l1_sc.h"
#include "l1_sc_irat.h"
#include "l1_fm.h"
#include "gl1_msg_wcdma_meas.h"

#include "rfm_device_types.h"

#include "vtmrs_g.h"

#include "gl1_hw_g.h"


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gpl1_dual_sim.h"

#include "gl1_msg_g.h"

#include "gl1_arbitrator_interface.h"

#include <stringl.h>

/*API's and types for converting T, W & L frequencies into TRM system bands*/

#include "l1rrcif.h"
#include "lte_as.h"
#include "wl1trm.h"

#if defined ( FEATURE_QSH_EVENT_NOTIFY_TO_QSH ) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
#include "l1_log_qsh.h"
#endif

#define BPLMN_MIN_DURATION_SCAN_MS 57 /* 57 ms - 12 frames */
#define BPLMN_MIN_DURATION_SCAN_FRAMES (((BPLMN_MIN_DURATION_SCAN_MS)*26)/120)
#define BPLMN_MIN_DURATION_ACQ_MS 80 /* 80 ms - 17 frames */
#define BPLMN_MIN_DURATION_ACQ_FRAMES (((BPLMN_MIN_DURATION_ACQ_MS)*26)/120)
#define BPLMN_RELEASE_OVERHEAD 3
/* Same as MIN_INTER_BCCH_DECODE_SEPARATION */
#define MIN_BCCH_DECODE_FRAMES 8
#define MAX_FRAMES_FOR_NEXT_BCCH_EXTENSION 10

/*===============DUAL DATA VARIABLES DECLARED HERE================*/

#define TRANSFER_MODE_TRM_ATTEMPTS 37
#define MIN_FRAMES_TO_MOVE_TO_TRANSFER 3
#define TBF_MIN_DURATION_FRAMES 8

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
typedef struct{
  trm_reason_enum_t reason;
  boolean valid_flag;
}global_trm_reason_t;
#endif

typedef enum{
  TRM_REQ_NULL,
  TRM_REQ_WAITING,
  TRM_GRANT_FW_LOAD_WAIT
}trm_request_state_t;

typedef struct
{
  trm_request_state_t     trm_request_state;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  global_trm_reason_t     global_trm_reason;
#endif
  boolean                 invert_sys_info_priority;
  uint16                  transfer_mode_lock_attempt_counter;
  background_hplm_state_t background_hplm_state;
  int32                   frames_required;
  int16                   suspension_timer;
  int16                   rf_fw_warmup_timer;
  trm_reason_enum_t       last_trm_reason;
}l1_multi_sim_t;

static l1_multi_sim_t l1_multi_sim[NUM_GERAN_DATA_SPACES];

/*Static var. to store the BPLMN band bitmap sent by RR*/
byte bplmn_idle_band_bitmap[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(0)};

l1_panic_reset_type_T DS_error_code[NUM_GERAN_DATA_SPACES] =
                        { INITIAL_VALUE( L1_NO_ERROR ) };
rr_l1_message_id_enum_T  DS_reset_response[NUM_GERAN_DATA_SPACES] =
                        { INITIAL_VALUE( MPH_INVALID_MESSAGE_ID ) };

/* Used to allow normal behaviour for firmware upon G plus G modes */
GOnly_mode_settings_t GOnly_mode_settings = { FALSE, GRM_GSM1, FALSE };

#define GBTA_INFO_INIT { TRUE, FALSE, GBTA_EV_NONE, GBTA_STATUS_NONE }

#ifdef FEATURE_GPRS_GBTA
grm_gbta_info_t      grm_gbta_info[NUM_GERAN_DATA_SPACES] =
                      { INITIAL_VALUE( GBTA_INFO_INIT ) };
grm_gbta_state_t     gbta_state = GRM_GBTA_NONE;
#endif /* FEATURE_GPRS_GBTA */


uint16 MULTI_SIM_FW_LOAD_FRAMES = MULTI_SIM_FW_LOAD_FRAMES_DEFAULT;

extern uint32 l1_max_rach_guard_time[NUM_GERAN_DATA_SPACES];

#ifdef  FEATURE_GSM_TO_LTE
/* LTE functions for G2L RF meas enter/exit */
extern boolean lte_ml1_common_band_get_band_from_dl_earfcn(lte_earfcn_t dl_earfcn, sys_sband_lte_e_type  * band_ptr);
extern sys_band_class_e_type lte_ml1_convert_lte_band_to_sys_band(sys_sband_lte_e_type lte_band);
#endif

/* 11/9/15 Temporary Data structure to debug CR892689 if it recurs - revert this before branching */
#define GL1_BPLMN_DEBUG
#ifdef GL1_BPLMN_DEBUG
  #define GL1_BPLMN_DEBUG_MAX 10
  typedef struct gl1_bplmn_debug_t {
    uint32                          frame_number;
    background_HPLMN_activity_type  background_HPLMN_search_activity;
    l1_bplmn_running_state_T        g_bplmn_running_state;
    background_hplm_state_t         background_hplm_state;
    gl1_trm_state_t                 trm_state;
    boolean                         bplmn_decode_bcch_inactive;
  } gl1_bplmn_debug_t;

  uint8 gl1_bplmn_debug_index[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };
  gl1_bplmn_debug_t gl1_bplmn_debug[NUM_GERAN_DATA_SPACES][GL1_BPLMN_DEBUG_MAX];
#endif

/*===============EXTERNAL VARIABLE DECLARATIONS================*/
extern uint32 sleep_duration_gsm_clks[NUM_GERAN_DATA_SPACES];

/*===============EXTERNAL FUNCTION DECLARATIONS================*/
extern void gl1_begin_INTERRAT_Handover( gas_id_t gas_id );
extern boolean l1_get_access_isr_send_mac_sb_failure( gas_id_t gas_id );
extern boolean re_calculate_initial_rach[NUM_GERAN_DATA_SPACES];

extern void gl1_msg_clear_acc_freq_err_valid( gas_id_t gas_id);
extern boolean switch_ds_task_pointers;

extern void L1_pscan_resume ( gas_id_t gas_id );
extern void L1_pscan_suspend ( void* dummy, gas_id_t gas_id );
extern boolean L1_background_HPLMN_search_running( gas_id_t gas_id );
extern void L1_background_HPLMN_search_suspend_cb( gas_id_t gas_id );

extern void l1_set_background_HPLMN_search_suspended(boolean background_HPLMN_search_suspended, gas_id_t gas_id);
void l1_ds_set_bplmn_state_to_null(gas_id_t gas_id);
background_hplm_state_t l1_ds_get_bplmn_state (gas_id_t gas_id);

static void gl1_ms_do_background_HPLMN_search( gas_id_t gas_id );
static void gl1_setup_request_and_notify_for_bplmn(grm_client_enum_t dest_client, uint32 duration, gas_id_t gas_id );
static void gl1_cancel_bplmn_request(grm_client_enum_t client , gas_id_t gas_id);
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
extern void l1_isr_wtog_bplmn_mode( gas_id_t gas_id );
#endif

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
extern void l1_isr_suspend_mode( gas_id_t gas_id );
#endif

#if defined (FEATURE_QTA) && defined (FEATURE_CXM_QTA)
extern boolean gl1_hw_cm_read_l2g_active(gas_id_t gas_id);
#endif

boolean wgg_ho_exchange_trm = FALSE;

#if defined (FEATURE_QBTA)
/*===========================================================================

FUNCTION gl1_check_qbta_allowed

DESCRIPTION
             Function to check if full QBTA ia allowed for an activity.
PARAMS
             gas_id

DEPENDENCIES

RETURN VALUE true or false

===========================================================================*/
boolean gl1_check_qbta_allowed(gas_id_t gas_id)
{
   return ( (gl1_hw_check_in_QBTA(gas_id)) &&  (gl1_hw_qta_gap_active(gas_id)) &&
        ((l1_tsk_buffer[gas_id].qta_in_progress)
         && (IS_QBTA_REASON(grm_gl1_get_current_trm_reason(l1_tsk_buffer[gas_id].client_id, gas_id),l1_fm_get_reserve_sub_reason(gas_id))) )
      );
}
#endif


/*===========================================================================

FUNCTION init_l1_multi_sim

DESCRIPTION
             Initializes multi sim data values .

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
/* Note for TRITON each GL1 task will call this function with Gas ID
 * So only initialise specified gas id
 */

void init_l1_multi_sim( gas_id_t gas_id )
{
  memset( &l1_multi_sim[gas_id], 0, sizeof( l1_multi_sim_t ) );

  l1_multi_sim[gas_id].trm_request_state            = TRM_REQ_NULL;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  l1_multi_sim[gas_id].global_trm_reason.valid_flag = FALSE;
#endif
  l1_multi_sim[gas_id].invert_sys_info_priority = FALSE;
  l1_multi_sim[gas_id].background_hplm_state        = L1_MULTI_SIM_BGRND_HPLMN_NULL;
  l1_multi_sim[gas_id].rf_fw_warmup_timer           = RF_FW_WARMUP_FRAME_DELAY;
  l1_multi_sim[gas_id].last_trm_reason              = TRM_DEMOD_PAGE;
}


/*===========================================================================

FUNCTION  gpl1_get_multi_sim_bplmn_state

DESCRIPTION
  returns the variable background_hplm_state
===========================================================================*/
background_hplm_state_t gpl1_get_multi_sim_bplmn_state( gas_id_t gas_id )
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];
  return ( l1_multi_sim_data_ptr->background_hplm_state );
}

/*===========================================================================

FUNCTION  gpl1_set_multi_sim_bplmn_state

DESCRIPTION
  sets the variable background_hplm_state
===========================================================================*/
void gpl1_set_multi_sim_bplmn_state( background_hplm_state_t background_hplm_state, gas_id_t gas_id )
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];
  l1_multi_sim_data_ptr->background_hplm_state = background_hplm_state;
}

/*===========================================================================

FUNCTION gl1_check_num_active_G_subs

DESCRIPTION
  Returns number of active SUBS

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/

int8 gl1_check_num_active_G_subs( void )
{
  int  i;
  int8 num_active_g_subs = 0;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk ;

  for ( i = 0; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    l1_tskisr_blk = &l1_tsk_buffer[i];
    if ( l1_tskisr_blk->l1_state != L1_INACTIVE_MODE )
    {
      num_active_g_subs++;
    }
  }

  return ( num_active_g_subs );
}

/*===========================================================================

FUNCTION  l1_isr_multi_sim_null

DESCRIPTION
  Do top level ISR processing for dual sim null mode.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_multi_sim_null(gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   boolean reset_state = TRUE;
   gl1_trm_state_t trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);


   if ((trm_status == GL1_TRM_CALLBACK_PENDING) ||
      ( trm_status == GL1_TRM_GRANTED ) ||
      ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
   {
     reset_state = FALSE;
   }

   if (l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND)
   {
      l1_isr_handle_multi_sim_find_bcch_command(gas_id);
   }
   else if (l1_tskisr_blk->main_command == L1_GET_TIMING_COMMAND)
   {
     if ( gl1_handle_grm_requests( GRM_CS_CONNECTION,
                                   reset_state,
                                   l1_tskisr_blk->next_params.ARFCN,
                                   GRM_DIVRX_TRM_REQ_INVALID,
                                   gas_id ) )
     {
       l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
       L1_transfer_new_data( gas_id );
       l1_tskisr_blk->sub_state     = L1_MULTI_SIM_AQUIRING;
       l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE_FOR_INTERRAT;
       l1_tskisr_blk->multi_sim_acquiring_state =  L1_MULTI_SIM_ACQUIRING_RUNNING;
       gl1_drx_off( FALSE,gas_id );
       l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
     }
     else
     {
       l1_tskisr_blk->main_command = L1_GET_TIMING_COMMAND_WAIT_FOR_TRM;
     }
   }
   else if (l1_tskisr_blk->main_command == L1_CM_HANDOVER_COMMAND )
   {
     if( gl1_handle_grm_requests( GRM_CS_CONNECTION,
                                  reset_state,
                                  gl1_get_serving_cell_arfcn( gas_id ),
                                  GRM_DIVRX_TRM_REQ_DED,
                                  gas_id ) )
     {
       /* This is run in the ISR context */
       /* Enable frame number counting */
       gl1_begin_INTERRAT_Handover(gas_id);
       l1_tskisr_blk->sub_state = L1_DED_SUSPEND;
       l1_tskisr_blk->main_command = L1_NULL_COMMAND;
       l1_tskisr_blk->command = L1_NULL_COMMAND;
     }
     else
     {
       l1_tskisr_blk->main_command = L1_CM_HANDOVER_COMMAND_WAIT_FOR_TRM;
     }
   }
   else if (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND)
   {
      l1_change_state( L1_INACTIVE_MODE, gas_id );
   }
   else if ( l1_tskisr_blk->main_command == L1_DEDICATED_COMMAND )
   {
     if ( gl1_handle_grm_requests( GRM_CS_CONNECTION,
                                   reset_state,
                                   gl1_get_serving_cell_arfcn( gas_id ),
                                   GRM_DIVRX_TRM_REQ_DED,
                                   gas_id ) )
     {
       gl1_begin_INTERRAT_Reconnect(gas_id);
       gl1_drx_off( TRUE, gas_id );
     }
   }else if  ( l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND )
   {
    L1_transfer_new_data(gas_id);
    if ( l1_tskisr_blk->current_params.L1_confirm_ID == L1_DECODE_BCCH_LIST_CNF )
    {
      l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
      l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
      L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
    }
   }else if ( l1_tskisr_blk->main_command == L1_GET_TIMING_COMMAND_WAIT_FOR_TRM )
   {
     if ( gl1_handle_grm_requests( GRM_CS_CONNECTION,
                                   FALSE,
                                   l1_tskisr_blk->next_params.ARFCN,
                                   GRM_DIVRX_TRM_REQ_INVALID,
                                   gas_id ) )
     {
       l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
       L1_transfer_new_data( gas_id );
       l1_tskisr_blk->sub_state     = L1_MULTI_SIM_AQUIRING;
       l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE_FOR_INTERRAT;
       l1_tskisr_blk->multi_sim_acquiring_state =  L1_MULTI_SIM_ACQUIRING_RUNNING;
       gl1_drx_off(FALSE, gas_id );
       l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
     }
   }
   else if ( l1_tskisr_blk->main_command == L1_CM_HANDOVER_COMMAND_WAIT_FOR_TRM )
   {
     if ( gl1_handle_grm_requests( GRM_CS_CONNECTION,
                                   FALSE,
                                   gl1_get_serving_cell_arfcn( gas_id ),
                                   GRM_DIVRX_TRM_REQ_DED,
                                   gas_id ) )
     {
       /* This is run in the ISR context */
       /* Enable frame number counting */
       gl1_begin_INTERRAT_Handover(gas_id);
       l1_tskisr_blk->sub_state = L1_DED_SUSPEND;
       l1_tskisr_blk->main_command = L1_NULL_COMMAND;
       l1_tskisr_blk->command = L1_NULL_COMMAND;
     }
   }

}

/*===========================================================================

FUNCTION  l1_multi_sim_idle_abort

DESCRIPTION
This function will ensure idle mode is aborted during Idle to transfer transistion
and Reserve at will be sent to TRM at TBF Starting time

DEPENDENCIES
  l1_tskisr_blk

PARAMS:
  gas_id
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_multi_sim_idle_abort(gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    idle_data_T 	 *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
    l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];
    boolean reset_state = TRUE;
    gl1_trm_state_t trm_status;
    transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;
    grm_client_enum_t client_id;
    trm_reason_enum_t    reason = grm_get_connection_priority (GRM_PS_CONNECTION,gas_id);
    trm_resource_enum_t  resource = gl1_ms_get_trm_resource(gas_id, reason);
    uint32 frames_required,delta_fn;

    client_id = l1_tskisr_blk->client_id;
    /*  Ensure this condition is met until L1_idle_mode_ISR returns TRUE  */
    /*First try to Abort Idle, then attempt TRM ; also, G was not trying to abort before*/
    if (L1_idle_mode_ISR( TRUE, gas_id ))
    {

        
        if(IS_FRAME_NUM_LATER(transfer_data_ptr->starting_time,gl1_get_FN(gas_id)))
         {
             delta_fn =  SUB_FN(transfer_data_ptr->starting_time,gl1_get_FN(gas_id));
             if(delta_fn > (NPL1_FN_OFFSET*2))
             {
                frames_required = SUB_FN(transfer_data_ptr->starting_time,NPL1_FN_OFFSET*1);
             }
             else
             {
                frames_required = ADD_FN(gl1_get_FN(gas_id), NPL1_FN_OFFSET*2);
             }
         }
         else
         {
             frames_required = ADD_FN(gl1_get_FN(gas_id), NPL1_FN_OFFSET*2);
             MSG_GERAN_HIGH_1_G("starting time elapsed %d",frames_required);
         }
        
        {
         /* send  RESERVE AT TBF STARTING TIME-2*/
         grm_reserve_at_data_t grm_reserve_at_data;
         l1_fm_reset_pri_res(gas_id);
         /* calculate the required parameters and pass in to TRM in the reservation request */
         grm_reserve_at_data.frames_required = frames_required;
         grm_reserve_at_data.frame_duration = TBF_MIN_DURATION_FRAMES;
         grm_reserve_at_data.reason = reason;
         grm_reserve_at_data.pri = FM_PRI_DL_TBF; /* Using ACCESS instead of GRR provided priority to increase the chances of acquiring lock */
         grm_reserve_at_data.sub_reason = gl1_get_specific_sys_proc(GERAN_GPRS_PROCEDURE, gas_id);
         grm_reserve_at_data.client_id = client_id;
         grm_reserve_at_data.gas_id = gas_id;
         grm_reserve_at_data.resource =resource;
         grm_reserve_at_data.duration= gpl1_multi_sim_convert_frames_to_sclcks(TBF_MIN_DURATION_FRAMES);
         
         gpl1_gbta_reset_active_state(gas_id);
         call_in_task_grm_reserve_at( grm_reserve_at_data );
         gl1_store_grm_sub_reason(grm_reserve_at_data.sub_reason,gas_id);
         MSG_GERAN_HIGH_1_G("l1_multi_sim_idle_abort:transfer_reserve_at frame %d",frames_required);
         }
         l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND_RESERVE_AT;
    }
    else
    {
      MSG_GERAN_MED_0_G("L1_TRANSFER_COMMAND_ABORTING_IDLE:Waiting for Idle Abort ");
    }

}
/*===========================================================================

FUNCTION  l1_isr_idle_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_multi_sim_idle( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T      *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  boolean reset_state = TRUE;
  gl1_trm_state_t trm_status;

   /* Set this signal for GPRS every tick to let L1 task
    * know that ISR is running.  This is used to let
    * task know when ISR is running again after a rude
    * wake up.
    */
   (void)rex_set_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
   /* Run HPLMN search handlers if they have been activated.  This
    * is controlled (suspend and resume ) from within l1_idle
    */

   /* G2L tick gap state machine in idle */
#ifdef  FEATURE_GSM_TO_LTE
   l1_isr_G2L_idle_trigger(gas_id);
#endif

#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_GSM_SCE_PWR_MEAS_DBG)
   {
      uint32 cur_FN;
      uint32 loc_do_pwr_meas_FN;

      loc_do_pwr_meas_FN = idle_data_ptr->do_pwr_meas_FN;

      if( GONLY_OPERATION_IS_ACTIVE()                    /* G / G+G only            */
          &&
          (loc_do_pwr_meas_FN != L1_GPRS_INVALID_FN) )   /* some pwr mon is active  */
      {
         cur_FN = GSTMR_GET_FN_GERAN(gas_id);
         if(IS_FRAME_NUM_LATER(cur_FN,  ADD_FN(loc_do_pwr_meas_FN, 4)))
         {
            /* pwr mon scheduled for > 4 frames, and results are still not available */
            MSG_GERAN_ERROR_2_G("SCE: ERROR: Pwr mon queued for long time in idle mode. Q_FN: %d, Cur_FN: %d",
               (uint32) loc_do_pwr_meas_FN, (uint32) cur_FN);
#ifdef FEATURE_GPRS_GBTA_DEBUG
#ifdef FEATURE_QSH_MDUMP
            QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif

			ERR_GERAN_FATAL_2_G("SCE: ERROR: Pwr mon queued for long time in idle mode. Q_FN: %d, Cur_FN: %d",
               (uint32) loc_do_pwr_meas_FN, (uint32) cur_FN);
#endif /*FEATURE_GPRS_GBTA_DEBUG*/
         }
      }
   }
#endif

   gl1_ms_do_background_HPLMN_search(gas_id);
   switch ( l1_tskisr_blk->main_command )
   {
      /******************************************************************************/

      case L1_NULL_COMMAND:
        {
         /* We are falling back to idle, check the flag
          * and post a message if necessary.
          */
         if( FALSE != l1_get_access_isr_send_mac_sb_failure(gas_id) )
         {
           L1_send_L1_ISR_SINGLE_BLK_FAILURE_CNF(gas_id );

           /* Reset the boolean */
           l1_set_access_isr_send_mac_sb_failure (FALSE,gas_id);
         }
           (void)L1_idle_mode_ISR( FALSE, gas_id );
        }
         break;

      /******************************************************************************/
      case L1_DEACTIVATE_COMMAND:

        if (L1_idle_mode_ISR( TRUE, gas_id ))
        {
          /* change mode */
          L1_transfer_new_data( gas_id );
          l1_change_state( L1_INACTIVE_MODE, gas_id );
          /* l1_tskisr_blk->main_command will be set to
           * L1_NULL_COMMAND in the next frame tick
           */

        }
         break;

      /******************************************************************************/
      case L1_ENTER_NULL_COMMAND:
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            /* change mode */
            L1_transfer_new_data( gas_id );
            l1_change_state( L1_MULTI_SIM_NULL, gas_id );
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         }
         break;

      /******************************************************************************/
      case L1_IDLE_COMMAND:
         l1_change_state( L1_MULTI_SIM_IDLE, gas_id );
         l1_tskisr_blk->main_command = L1_NULL_COMMAND;
         L1_send_MPH_START_IDLE_MODE_CNF(gas_id);

         /*set timeslot according to CCCH_GROUP*/
         if( gl1_hw_qta_gap_active(gas_id) || l1_fm_any_activity_running(gas_id))
         {
           MSG_GERAN_HIGH_1_G("Inside the QTA gap %d or FN activity still running, do not slam GSTMR here", gl1_hw_qta_gap_active(gas_id));
           l1_tskisr_blk->time_slot_change_pending = TRUE;
         }
         else
         {
           L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
         }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         if (l1_tskisr_blk->l1_prev_state == L1_RANDOM_ACCESS_MODE ||
             l1_tskisr_blk->l1_prev_state == L1_PACKET_ACCESS_MODE ||
             l1_tskisr_blk->l1_prev_state == L1_TRANSFER_MODE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
         {
           if ((l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR) &&
               (l1_transfer_data[gas_id].release_pending != L1_NO_RELEASE))
           {
             MSG_GERAN_HIGH_0_G("rel confirm isr");
             /* Indicate to MAC release procedure is complete */
             gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, gas_id);

            /* reset relelase type and rel confirm status*/
             l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
             l1_transfer_data[gas_id].confirm_pending = SENT;
           }
         }

         if(!l1_sc_globals_ptr->active)
         {
            l1_sci_go(gas_id);
         }

         (void)L1_idle_mode_ISR( FALSE, gas_id );
         break;

      /******************************************************************************/
      case L1_MULTI_SIM_IDLE_COMMAND:
        {
          if (L1_idle_mode_ISR( TRUE, gas_id ))
          {
            if ((l1_fm_get_suspend_trm_release(gas_id) == 0)
                && (GL1_TRM_NULL == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id))
                && (l1_tskisr_blk->firmware_loaded == FALSE))
            {

              l1_sc_start(L1SCModeIdle, gas_id);
              l1_tskisr_blk->sub_state = L1_IDLE_INIT;
              l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
            }
            gl1_drx_require_next_tick(gas_id);
          }
         }
          break;

      /******************************************************************************/
      case L1_DEDICATED_COMMAND:

         trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

         /* check that all Activity has finished */
         /* can this happen ??? */
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
           if ((trm_status == GL1_TRM_CALLBACK_PENDING) ||
              ( trm_status == GL1_TRM_GRANTED ) ||
              ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
           {
             reset_state = FALSE;
           }

           if ( gl1_handle_grm_requests( GRM_CS_CONNECTION,
                                         reset_state,
                                         gl1_get_serving_cell_arfcn( gas_id ),
                                         GRM_DIVRX_TRM_REQ_DED,
                                         gas_id ) )
           {
             /* change mode */
             L1_transfer_new_data( gas_id );
             l1_change_state( L1_DEDICATED_MODE, gas_id );
             l1_tskisr_blk->sub_state = L1_DED_INIT;
             gl1_drx_off(TRUE, gas_id );
             l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
           }
           else
           {
             l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND_WAIT_FOR_TRM;
           }
         }
         break;
      /******************************************************************************/
      case L1_DEDICATED_COMMAND_WAIT_FOR_TRM:
         /* check that all Activity has finished */
         /* can this happen ??? */
         if ( gl1_handle_grm_requests( GRM_CS_CONNECTION,
                                       FALSE,
                                       gl1_get_serving_cell_arfcn( gas_id ),
                                       GRM_DIVRX_TRM_REQ_DED,
                                       gas_id ) )
         {
           /* change mode */
           L1_transfer_new_data( gas_id );
           l1_change_state( L1_DEDICATED_MODE, gas_id );
           l1_tskisr_blk->sub_state = L1_DED_INIT;
           gl1_drx_off(TRUE, gas_id );
           l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
         }
         break;

      /******************************************************************************/
      case L1_FIND_SCH_COMMAND:
         /* check that all Activity has finished */
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            l1_isr_handle_multi_sim_find_sch_command(gas_id);
         }
         break;

      /******************************************************************************/
      case L1_FIND_BCCH_COMMAND:
         /* check that all Activity has finished */

         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            l1_isr_handle_multi_sim_find_bcch_command(gas_id);
         }
         break;

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
      /******************************************************************************/
      case L1_SUSPEND_COMMAND:
         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            /* change mode */
            L1_transfer_new_data( gas_id );
            l1_change_state( L1_SUSPEND_MODE, gas_id );
         }
         break;
#endif


      /******************************************************************************/
      case L1_GPRS_IDLE_COMMAND:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         if (l1_tskisr_blk->l1_prev_state == L1_RANDOM_ACCESS_MODE ||
             l1_tskisr_blk->l1_prev_state == L1_PACKET_ACCESS_MODE ||
             l1_tskisr_blk->l1_prev_state == L1_TRANSFER_MODE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
         {
           if( ( l1_transfer_data[gas_id].confirm_pending == SEND_IN_ISR)
             &&( l1_transfer_data[gas_id].release_pending != L1_NO_RELEASE))
           {
              MSG_GERAN_HIGH_0_G("rel confirm isr");

              /* Indicate to MAC release procedure is complete */
              gpl1_gprs_send_l1_mac_tbf_rel_confirm( l1_transfer_data[gas_id].release_pending, gas_id );

              /* reset relelase type and rel confirm status */
              l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
              l1_transfer_data[gas_id].confirm_pending = SENT;
           }
         }

         if (L1_idle_mode_ISR( TRUE, gas_id ))
         {
            l1_change_state(L1_GPRS_IDLE_MODE, gas_id);
            l1_tskisr_blk->sub_sub_state = L1_GPRS_IDLE_INIT;
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
            L1_send_MPH_START_IDLE_MODE_CNF(gas_id);
            L1_change_timeslot( 0, gas_id );
         }
         break;

      /******************************************************************************/
      case L1_TRANSFER_COMMAND:
      {
         transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;

         /* First check if a starting time has been specified */
         if (transfer_data_ptr->starting_time_valid)
         {
            /* Is the starting time about to expire in 3 frames
             * assuming starting time is always aligned to block boundary
             */
            boolean abort_idle_mode = FALSE;
            const uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id);

            uint32 delta_fn = 0;

            if ( IS_FRAME_NUM_LATER( transfer_data_ptr->starting_time,current_fn))
            {
              delta_fn = SUB_FN(transfer_data_ptr->starting_time,current_fn);
            }
            else
            {
              /*Starttime elapsed, good to go to traffic state at earliest*/
              delta_fn = 0;
            }

            if (delta_fn <= (MIN_FRAMES_TO_MOVE_TO_TRANSFER+3) ||
                (FALSE != l1_get_starting_time_elapsed(gas_id)))
            {
              abort_idle_mode = TRUE;
            }

            if (abort_idle_mode)
            {
               /*  Ensure this condition is met until L1_idle_mode_ISR returns TRUE  */
               l1_set_starting_time_elapsed(TRUE, gas_id );
               l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND_ABORTING_IDLE;
               l1_multi_sim_idle_abort(gas_id);
               /*First try to Abort Idle, then attempt TRM ; also, G was not trying to abort before*/
            }
            else
            {
               /* starting time set but too far away
                * continue with either 51 idle mode
                */
               (void)L1_idle_mode_ISR( FALSE, gas_id );
               gl1_drx_require_next_tick(gas_id);
            }
         }
         else // no starting time so change state to transfer asap
         {
            /*  There will always be a starting time at this point as the
                task will create one if one not present in the assisgnment  */
            MSG_GERAN_FATAL_1_G("TRANSFER COMMAND WITH NO ST %d",GSTMR_GET_FN_GERAN( gas_id));
         }

         } /* End of case L1_TRANSFER_COMMAND */
         break;

      /******************************************************************************/
      case L1_TRANSFER_COMMAND_WAIT_FOR_TRM:
      {
        transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;
        boolean reset_state = TRUE;
        gl1_trm_state_t trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        /* Stay here and see if the lock can be acquired - Then just wait for the firmware to load */
        if ( l1_multi_sim_data_ptr->transfer_mode_lock_attempt_counter-- > 0 )
        {

          if ((trm_status == GL1_TRM_CALLBACK_PENDING) ||
             ( trm_status == GL1_TRM_GRANTED ) ||
             ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
          {
            /* fixes potential race condition when trm has been released and
             * trm status moved to NULL. In which case need to put in a new R and N
             */
            reset_state = FALSE;
          }
          if (!gl1_handle_grm_requests(GRM_PS_CONNECTION,
                                       reset_state,
                                       gl1_get_serving_cell_arfcn( gas_id ),
                                       GRM_DIVRX_TRM_REQ_DATA,
                                       gas_id ) )
          {
            /*trm_status can be changed */
            trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
            if ( (trm_status != GL1_TRM_GRANTED) &&
                 (trm_status != GL1_TRM_RETAINED_FOR_ACCESS)
               )
            {
              MSG_GERAN_HIGH_1_G("Reattempting transfer mode lock attempt %d",
                                 l1_multi_sim_data_ptr->transfer_mode_lock_attempt_counter);
              /* Lock has failed retry again ten times */
            }
            else  if (gl1_hw_qta_gap_active(gas_id))
            {
               MSG_GERAN_HIGH_1_G("IN QTA : Reattempting transfer mode lock attempt %d",l1_multi_sim_data_ptr->transfer_mode_lock_attempt_counter);
               (void)L1_idle_mode_ISR( FALSE, gas_id );
               gl1_drx_require_next_tick(gas_id);
               break;
            }
            else
            {
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND_WAIT_FOR_FW;

              /*
               * If the lock is ours then hold on to it here, otherwise it may get released
               * and the firmware unloaded whilst transitioning to transfer mode. This may happen
               * when any idle activity is aborted or completes on the same sub.
               */
              if ( trm_status == GL1_TRM_GRANTED )
              {
                grm_set_trm_status(GL1_TRM_RETAINED_FOR_ACCESS, l1_tskisr_blk->client_id, gas_id);
              }
            }
          }
          else
          {
            /* change mode */
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND_IDLE_ABORTED;
          }
        }
        else
        {
          /* give up and tell MAC that this has failed */
          gpl1_gprs_send_l1_mac_ph_connect_ind_trm_failed(transfer_data_ptr->tbf_est_type, gas_id);
          l1_sc_stop(gas_id);
          l1_sc_start(L1SCModeIdle, gas_id);
          l1_tskisr_blk->sub_state = L1_IDLE_INIT;
          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          call_in_task_grm_release(l1_tskisr_blk->client_id);
        }

        gl1_drx_require_next_tick(gas_id);
      }
      break;

     /******************************************************************************/
      case L1_TRANSFER_COMMAND_WAIT_FOR_FW:
      {
        /* Stay here and see if the lock can be acquired - Then just wait for the firmware to load */
        if ( gl1_handle_grm_requests( GRM_PS_CONNECTION,
                                      FALSE,
                                      gl1_get_serving_cell_arfcn( gas_id ),
                                      GRM_DIVRX_TRM_REQ_DATA,
                                      gas_id ) )
        {
          /* change mode */
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND_IDLE_ABORTED;
        }
        else
        {
          gl1_drx_require_next_tick(gas_id);
        }
        break;
      }

      /******************************************************************************/
      case L1_TRANSFER_COMMAND_ABORTING_IDLE:
      {
        l1_multi_sim_idle_abort(gas_id);
      }
      break;

      /******************************************************************************/
      case L1_TRANSFER_COMMAND_RESERVE_AT:
      {
         transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;
         uint32 delta_fn = 0;
         trm_grant_return_enum_type grant_status = TRM_RETURN_MAX;
         trm_reason_enum_t   reason = grm_get_connection_priority (GRM_PS_CONNECTION,gas_id);
         trm_resource_enum_t  resource = gl1_ms_get_trm_resource(gas_id, reason);
         grm_client_enum_t tx_client_id = grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id);
         gl1_trm_state_t tx_trm_status = GL1_TRM_NULL;

         sys_proc_type_e_type sub_reason = gl1_get_specific_sys_proc(GERAN_GPRS_PROCEDURE, gas_id);

         ARFCN_T   arfcn_info = gl1_get_serving_cell_arfcn( gas_id );
         const uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id);
         if (IS_FRAME_NUM_LATER( transfer_data_ptr->starting_time,current_fn))
         {
             delta_fn = SUB_FN(transfer_data_ptr->starting_time,current_fn);
         }
         else
         {
           /*Starttime elapsed, good to go to traffic state at earliest*/
            delta_fn = 0;
         }
          if((l1_tskisr_blk->firmware_load_status != FW_UNLOADED) && 
              !(grm_get_resource_lock_state(gas_id) &&(l1_tskisr_blk->firmware_load_status == FW_LOADED)))
         {
           MSG_GERAN_HIGH_1_G ("Waitng to do TRM reserve at fw_load_status %d",l1_tskisr_blk->firmware_load_status);
           break;
         }
         MSG_GERAN_HIGH_3_G("RxStatus:%d, TxStatus:%d, delta_fn:%d", 
                              grm_get_trm_status(l1_tskisr_blk->client_id,gas_id),
                              grm_get_trm_status(tx_client_id, gas_id),
                              delta_fn);
 
         if(delta_fn <= NPL1_FN_OFFSET*2)
         {
           /*if Rx is not granted, then request for both Rx and Tx*/
           if(grm_get_trm_status(l1_tskisr_blk->client_id,gas_id) != GL1_TRM_GRANTED)
           {         
           (void)grm_add_arfcn_to_trm_freq_input_type(arfcn_info,l1_tskisr_blk->client_id,gas_id );
           grant_status = grm_request(l1_tskisr_blk->client_id, gpl1_multi_sim_convert_frames_to_sclcks(TBF_MIN_DURATION_FRAMES), reason, sub_reason, resource, GRM_DIVRX_TRM_REQ_INVALID);
           }
           else if(grm_get_trm_status(tx_client_id, gas_id) != GL1_TRM_GRANTED)
           {
             (void)grm_add_arfcn_to_trm_freq_input_type(arfcn_info,tx_client_id,gas_id );
             tx_trm_status =grm_request_tx_chain(tx_client_id,
                                  gpl1_multi_sim_convert_frames_to_sclcks(TBF_MIN_DURATION_FRAMES),
                                  reason,
                                  sub_reason,
                                  TRM_RX_ANY,
                                  gas_id);

             if (tx_trm_status ==  GL1_TRM_GRANTED)
             {
                 grant_status = TRM_GRANTED;
                 grm_change_priority(l1_tskisr_blk->client_id, reason, sub_reason, gas_id );
             }
             else
             {
                 grant_status = TRM_DENIAL;
                 
             }
           
           }
           else
           {
             grant_status = TRM_GRANTED;
             grm_change_priority(l1_tskisr_blk->client_id, reason, sub_reason, gas_id );
             grm_change_priority(tx_client_id, reason, sub_reason, gas_id );
             
           }
           
           if(grant_status == TRM_GRANTED )
           {
              grm_set_trm_grant_status(grant_status, l1_tskisr_blk->client_id, gas_id);
              grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);
              transfer_data_ptr->polling_at_tbf_start_time = FALSE;
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND_IDLE_ABORTED;
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
           }
           else if(grant_status == TRM_DENIAL && transfer_data_ptr->polling_at_tbf_start_time)
           {
             /*UE cannot send polling at the TBF starting time, NW will not respond further.So Better fail at this step and 
              wait for new assigment after moving to Idle*/
              gpl1_gprs_send_l1_mac_ph_connect_ind_trm_failed(transfer_data_ptr->tbf_est_type, gas_id);
              l1_sc_stop(gas_id);
              l1_sc_start(L1SCModeIdle, gas_id);
              l1_tskisr_blk->sub_state = L1_IDLE_INIT;
              l1_tskisr_blk->main_command = L1_NULL_COMMAND;
              transfer_data_ptr->polling_at_tbf_start_time = FALSE;
              break;
           }
           else
           {/* TRM not granted */
               transfer_data_ptr->polling_at_tbf_start_time = FALSE;
               l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND_IDLE_ABORTED;
               call_in_task_grm_release(l1_tskisr_blk->client_id);               
           }
         }
         else
         {
           MSG_GERAN_HIGH_1_G ("Waitng to do TRM request %d",delta_fn);
           break;
         }
      }
      /******************************************************************************/
      case L1_TRANSFER_COMMAND_IDLE_ABORTED:
      { /*Should enter this, when transitioning from Idle to Transfer after releasing TRM lock in DS*/

         transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;
         const uint32  current_fn = GSTMR_GET_FN_GERAN(gas_id);
         uint32 delta_fn = 0;

         l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];
         gl1_trm_state_t trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

         if ( l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE )
         {
           /* grm_release is called in task in previous state(L1_TRANSFER_COMMAND_ABORTING_IDLE)/previous frame tick.
              in rare case, GL1 task couldn't finish grm_release in one frame. GL1 has to wait here
           */
           MSG_GERAN_HIGH_0_G("FW Load Status: Release pending, wait for another tick to finish grm_release");
           return;
         }
         if ( IS_FRAME_NUM_LATER( transfer_data_ptr->starting_time,current_fn))
         {
           delta_fn = SUB_FN(transfer_data_ptr->starting_time,current_fn);
         }
         else
         {
           /*Starttime elapsed, good to go to traffic state at earliest*/
           delta_fn = 0;
         }

         if ((l1_tskisr_blk->firmware_loaded == FALSE )
             && (GL1_TRM_NULL == trm_status))
         {

           /* This is just a one shot request to TRM for resources here */
           if ( gl1_handle_grm_requests( GRM_PS_CONNECTION,
                                         TRUE,
                                         gl1_get_serving_cell_arfcn( gas_id ),
                                         GRM_DIVRX_TRM_REQ_DATA,
                                         gas_id ) )
           {

             if(delta_fn <= MIN_FRAMES_TO_MOVE_TO_TRANSFER)
             {
                /* change mode */
                L1_transfer_new_data( gas_id );

                l1_change_state( L1_TRANSFER_MODE, gas_id );

                l1_tskisr_blk->sub_state = L1_TRANSFER_INIT;
                l1_tskisr_blk->sub_sub_state = NULL_STATE;

                gl1_drx_off(TRUE, gas_id );
                l1_tskisr_blk->main_command  = L1_NULL_COMMAND;

                /* Clear start time valid flag for next time */
                idle_data_ptr->starting_time_valid = FALSE;

                /* If there are outstanding Serving cell Sys Info read requests
                 * that have not been serviced by L1 then send a failure indication
                 * to Upper layers with cause: state change.
                 */
                 if(  ( RR_L1_SI_TYPE_NONE != idle_data_ptr->si_mask )
                       &&(  ( SERV_BCCH_MASK_NONE != idle_data_ptr->BCCH_mask )
                       ||( SERV_EXT_BCCH_MASK_NONE != idle_data_ptr->ext_BCCH_mask )))
                 {
                     /* Send a sys info request failure indication */
                     L1_send_MPH_READ_SYS_INFO_FAILURE_IND( TRUE,FALSE, gas_id );
                 }
                 /*  Ensure this condition is met until L1_idle_mode_ISR returns TRUE  */
                    l1_set_starting_time_elapsed(FALSE, gas_id );

                    /*persistent denial monitoring is only applicable in idle mode*/
                    grm_persistent_denial_monitoring(GRM_RESET_PERSISTENT_DENIAL_MONITORING,
                                                     l1_tskisr_blk->client_id,
                                                     GRM_NULL_COMMAND,
                                                     gas_id);
             }
             else
             {
               MSG_GERAN_HIGH_1_G ("TRM lock granted : Waiting for MIN_FRAMES_TO_MOVE_TO_TRANSFER before moving to transfer %d",delta_fn);
             }
           }
           else
           {
               trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
               /* Test conditions to see if the lock failed for this request */
               if ( (trm_status != GL1_TRM_GRANTED) &&
                    (trm_status != GL1_TRM_RETAINED_FOR_ACCESS)
                 )
               {
                 /* Lock has failed retry again TRANSFER_MODE_TRM_ATTEMPTS times */
                 l1_multi_sim_data_ptr->transfer_mode_lock_attempt_counter = TRANSFER_MODE_TRM_ATTEMPTS;
                 l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND_WAIT_FOR_TRM;


                 MSG_GERAN_HIGH_1_G("transfer_mode_lock_attempt_counter -> TRANSFER_MODE_TRM_ATTEMPTS =%d",l1_multi_sim_data_ptr->transfer_mode_lock_attempt_counter);
                }
                else
                {

                   /*
                    * If the lock is ours then hold on to it here, otherwise it may get released
                    * and the firmware unloaded whilst transitioning to transfer mode. This may happen
                    * when any idle activity is aborted or completes on the same sub.
                    */
                   if ( trm_status == GL1_TRM_GRANTED )
                   {
                     grm_set_trm_status(GL1_TRM_RETAINED_FOR_ACCESS, l1_tskisr_blk->client_id, gas_id);
                   }
                }

                gl1_drx_require_next_tick(gas_id);
            }
          }
         else if (((trm_status == GL1_TRM_RETAINED_FOR_ACCESS)||(trm_status == GL1_TRM_GRANTED)) && (l1_tskisr_blk->firmware_load_status != FW_PENDING_RELEASE))
         {
           if (l1_tskisr_blk->firmware_loaded == TRUE &&
               (delta_fn <= MIN_FRAMES_TO_MOVE_TO_TRANSFER) )
           {

             /*safe to change state here*/
             /* change mode */
             L1_transfer_new_data( gas_id );
             l1_change_state( L1_TRANSFER_MODE, gas_id );
             l1_tskisr_blk->sub_state = L1_TRANSFER_INIT;
             l1_tskisr_blk->sub_sub_state = NULL_STATE;
             gl1_drx_off( TRUE,gas_id );
             l1_tskisr_blk->main_command = L1_NULL_COMMAND;
             l1_multi_sim_data_ptr->trm_request_state = TRM_REQ_NULL;

             /* Clear start time valid flag for next time */
             idle_data_ptr->starting_time_valid = FALSE;

             /* If there are outstanding Serving cell Sys Info read requests
              * that have not been serviced by L1 then send a failure indication
              * to Upper layers with cause: state change.
              */

            if(  ( RR_L1_SI_TYPE_NONE != idle_data_ptr->si_mask )
               &&(  ( SERV_BCCH_MASK_NONE != idle_data_ptr->BCCH_mask )
                  ||( SERV_EXT_BCCH_MASK_NONE != idle_data_ptr->ext_BCCH_mask )
                 )
              )
              {
                /* Send a sys info request failure indication */
                L1_send_MPH_READ_SYS_INFO_FAILURE_IND( TRUE,       /* state change */
                                                       FALSE, gas_id );
              }

               /*  Ensure this condition is met until L1_idle_mode_ISR returns TRUE  */
               l1_set_starting_time_elapsed(FALSE, gas_id );
             }
           else if (l1_multi_sim_data_ptr->trm_request_state != TRM_GRANT_FW_LOAD_WAIT)
           {
             gl1_multi_sim_fw_rf_load( gl1_map_gas_id_to_client_id(gas_id), gas_id );
             l1_multi_sim_data_ptr->trm_request_state = TRM_GRANT_FW_LOAD_WAIT;
           }

         }
         else if(trm_status == GL1_TRM_ABORTING)
         {
             MSG_GERAN_MED_0_G("L1_TRANSFER_COMMAND_IDLE_ABORTED: TRM_ABORTED");
             /* give up and tell MAC that this has failed */
             gpl1_gprs_send_l1_mac_ph_connect_ind_trm_failed(transfer_data_ptr->tbf_est_type, gas_id);
             l1_sc_stop(gas_id);
             l1_sc_start(L1SCModeIdle, gas_id);
             l1_tskisr_blk->sub_state = L1_IDLE_INIT;
             l1_tskisr_blk->main_command = L1_NULL_COMMAND;
             call_in_task_grm_release(l1_tskisr_blk->client_id);
         }
         else
         {
             MSG_GERAN_MED_0_G("L1_TRANSFER_COMMAND_IDLE_ABORTED: Waiting for FW Unload");
         }
      }
      break;

      /******************************************************************************/
      case L1_ACCESS_COMMAND:
        /* L1_ACCESS_COMMAND is only realy handled in decoupled state
         * If here then previous RACH has probably been aborted and new RACH initiated
         * RACH abort moved us to Idle - but new RACH initiated was in decoupled state
         * Move to decoupled state to handle L1_ACCESS_COMMAND
         */
         l1_change_state(L1_MULTI_SIM_DECOUPLED, gas_id);
         break;
      /******************************************************************************/
      case L1_DECOUPLE_MULTI_SIM_COMMAND:

        if (GL1_TRM_RETAINED_FOR_ACCESS == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id))
        {
          /* this is to prevent frame manager from retaining the lock */
          grm_set_trm_status(GL1_TRM_GRANTED,l1_tskisr_blk->client_id, gas_id);
        }
         /* Clean up idle SCE before RACHing */
        if (L1_idle_mode_ISR( TRUE, gas_id ))
        {
           if ( (l1_fm_get_suspend_trm_release(gas_id) == 0)
                && ( GL1_TRM_NULL == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) ||
                     GL1_TRM_CALLBACK_PENDING == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) ||
                     ( GL1_TRM_GRANTED == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) &&
                       grm_get_resource_lock_state (gas_id) == FALSE
                     )
                   )
                && ( l1_tskisr_blk->firmware_loaded == FALSE )&& (gl1_hw_qta_gap_active(gas_id) == FALSE)&&(IS_GBTA_ACTIVE() == FALSE))
           {
              MSG_GERAN_HIGH_0_G("Move to MULTI_SIM_DECOUPLED");
              l1_change_state(L1_MULTI_SIM_DECOUPLED, gas_id);
              l1_tskisr_blk->multi_sim_decoupled_state = L1_MULTI_SIM_DECOUPLED_WAIT_FOR_TRM_GRANT;
              /* Release the lock to clear any pending reservations at this point*/
              grm_release(l1_tskisr_blk->client_id, gas_id);
              gl1_drx_require_next_tick(gas_id);
              L1_send_PH_READY_TO_SEND_IND(L2_RACH, gas_id);
           }
#ifdef FEATURE_GSM_RX_TX_SPLIT
           else if (( GRM_COMPLETE_GRANT == grm_get_rx_tx_status(l1_tskisr_blk->client_id, gas_id) ) || 
                    (GRM_COMPLETE_UNLOCK == grm_get_rx_tx_status(l1_tskisr_blk->client_id, gas_id))
                   )
#else
           else if ( ( GL1_TRM_GRANTED == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) ) ||
                     ( GL1_TRM__RETAINED_FOR_ACCESS == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) )
                   )
#endif
           {
             /* In current implementation this should only occur when RR resource lock procedure is active */
             if (!grm_get_resource_lock_state (gas_id))
             {
               MSG_GERAN_ERROR_0_G ("Unexpected attempt to RACH with existing grant outside procedure lock");
             }
             MSG_GERAN_HIGH_0_G("already granted, Move to MULTI_SIM_DECOUPLED");
             l1_change_state(L1_MULTI_SIM_DECOUPLED, gas_id);

             /* Need to start here to take care of the fw loading requirement */
             l1_tskisr_blk->multi_sim_decoupled_state = L1_MULTI_SIM_DECOUPLED_WAIT_FOR_TRM_GRANT;
             gl1_drx_require_next_tick(gas_id);
             L1_send_PH_READY_TO_SEND_IND(L2_RACH, gas_id);
           }
           else
           {
             if ( ( grm_get_resource_lock_state (gas_id) ) &&
                  ( GRM_COMPLETE_GRANT != grm_get_rx_tx_status(l1_tskisr_blk->client_id, gas_id))&&
                  ( GRM_COMPLETE_UNLOCK != grm_get_rx_tx_status(l1_tskisr_blk->client_id, gas_id))&&
                  ( GRM_PARTIAL_UNLOCK != grm_get_rx_tx_status(l1_tskisr_blk->client_id, gas_id))
                )
             {
#ifdef FEATURE_QSH_MDUMP
                QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                ERR_GERAN_FATAL_0_G("resource lock is active but GL1 does not have complete Rx/Tx locks");
             }
             MSG_GERAN_HIGH_2_G("Waiting in idle trm_status %d, QTA status  %d",
                                grm_get_trm_status(l1_tskisr_blk->client_id, gas_id),
                                gl1_hw_qta_gap_active(gas_id));
           }
        }
         break;


         default:
            MSG_GERAN_HIGH_1_G("Bad L1 main_command %d.", l1_tskisr_blk->main_command);
            break;
   } /* End of switch on main command */
}

/*===========================================================================

FUNCTION  l1_isr_multi_sim_decoupled

DESCRIPTION
 IR multi sim decoupled state machiine

DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_multi_sim_decoupled( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gl1_trm_state_t trm_status;
  gl1_trm_state_t tx_trm_status;

  gl1_rach_mutex_lock(gas_id);

  trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
  tx_trm_status = grm_get_trm_status( grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id);


  if ((trm_status == GL1_TRM_GRANTED) ||
      (trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
  {
    if(grm_device_config_is_drdsds()&&
       !gl1_drdsds_enabled(gas_id))
    {
      /* DR mode might be disabled when partial grant is received in decouple state. After
         falling back to SR mode, GL1 will be able to get around of partial grant and come
         to here. We re-enable DR mode again.
      */
      L1_enable_dual_receive(l1_tskisr_blk->client_id, gas_id);
      MSG_GERAN_HIGH_0_G(GL1_DRDS_DBG_HDR"Re_Enable DR on exiting decouple state");
    }
  }
  else
  {
    if (l1_tskisr_blk->main_command != L1_ACCESS_COMMAND )
    {
      /* If main command is not ACCESS_COMMAND, GL1 will exit decouple state. Same as above,
         DR mode migh be disabled before. We need to enable it here
      */

       if(grm_device_config_is_drdsds())
       {
         (void)grm_enable_disable_dual_receive( TRUE, l1_tskisr_blk->client_id, gas_id );
         MSG_GERAN_HIGH_0_G(GL1_DRDS_DBG_HDR"Vote for DR on exiting decouple state");
       }
    }
  }
  if (l1_tskisr_blk->main_command == L1_ACCESS_COMMAND)
  {
       idle_data_T  *l1_idle_data = &l1_idle_data_store[gas_id];

    if(((trm_status == GL1_TRM_NULL) || (trm_status == GL1_TRM_NOT_GRANTED)) && 
       (!gl1_hw_qta_gap_active( gas_id)) && 
       (gl1_get_rach_type(gas_id) != NO_RACH))
    {
        idle_data_T               *l1_idle_data;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        trm_resource_enum_t       resource;
        gl1_trm_state_t           trm_status;
        ps_access_mode_t          packetservice = PS_ACCESS_NONE;
        trm_duration_t            duration = GRM_ACCESS_DURATION_SCLKS;
        trm_grant_return_enum_type trm_grant_status = TRM_RETURN_MAX;
        gas_id_t                  ptm_gas_id;
        grm_divrx_request_type    req_type = GRM_DIVRX_TRM_REQ_INVALID; 
        trm_reason_enum_t         reason = TRM_NUM_REASONS;
        sys_proc_type_e_type      sub_reason = SYS_PROC_TYPE_NONE;
        grm_unlock_callback_t     unlock_callback = NULL;
        geran_procedure_t         geran_procedure = GERAN_CONN_PROCEDURE;
        
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        /* determine the TRM parameters for this new RACH */
        if (gl1_get_rach_type(gas_id) == PS_RACH)
        {
          /* retrieve the reason and sub-reason to use for this PS RACH */
          gpl1_determine_rach_reasons(GRM_PS_CONNECTION, &reason, &sub_reason, gas_id);
          unlock_callback = (grm_unlock_callback_t)&grm_ps_access_abort_callback;
          geran_procedure = GERAN_GPRS_PROCEDURE;
          /* diversity flag set to pkt transfer mode */
          req_type = GRM_DIVRX_TRM_REQ_DATA;
          duration = GRM_ACCESS_DURATION_PS_SCLKS;          
        }
        else if (gl1_get_rach_type(gas_id) == CS_RACH)
        {
          /* retrieve the reason and sub-reason to use for this CS RACH */
          gpl1_determine_rach_reasons(GRM_CS_CONNECTION, &reason, &sub_reason, gas_id);
        
          if (reason == TRM_TRAFFIC)
          {
            unlock_callback = (grm_unlock_callback_t)&grm_cs_traffic_abort_callback;
          }
          else
          {
            unlock_callback = (grm_unlock_callback_t)&grm_cs_access_abort_callback;
          }
        
          /* diversity flag set to dedicated mode */
          req_type = GRM_DIVRX_TRM_REQ_DED;
        }
        
        resource = gl1_ms_get_trm_resource(gas_id, reason);
#ifdef FEATURE_SGLTE        
        if ((gl1_get_rach_type(gas_id) == PS_RACH)
#ifdef FEATURE_DUAL_DATA
          && grm_get_data_sub_priority(gas_id) != GERAN_PRIORITY_INVERTED
#endif
          )
        {
            if (gl1_hw_get_sglte_mode(gas_id) &&
              (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))
            {
              MSG_GERAN_MED_0_G("This is a PS call, set resource to bp_mod_2");
              resource = TRM_RX_BEST_POSSIBLE_MOD_2;
              packetservice = PS_ACCESS_NON_MM_DEVICE;
            }
        }
#endif
        trm_grant_status = grm_request(l1_tskisr_blk->client_id,
                                       duration,
                                       reason,
                                       sub_reason,
                                       resource,
                                       req_type );
        grm_set_trm_grant_status(trm_grant_status, l1_tskisr_blk->client_id, gas_id);
        if (trm_grant_status != TRM_DENIAL)
        {
          ptm_gas_id = gl1_get_gas_id_for_ptm_client();
          trm_status = grm_get_trm_status(l1_tsk_buffer[ptm_gas_id].client_id, gas_id);
          if (ptm_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED &&
            trm_status == GL1_TRM_CALLBACK_PENDING)
          {
            MSG_GERAN_HIGH_2_G("Other sub in GTA gap, state:%d, trm_status:%d", l1_tsk_buffer[ptm_gas_id].l1_state, trm_status);
        
            /* Dont requestAndNotify also inside GTA gap. Start RACH timer which on expiry sends a failure cnf to RR
            Release TRM so that Transfer SUB can get the lock*/
#ifdef FEAT_DIFF_SIG_PATH_PER_GSM_BAND
            grm_release(l1_tskisr_blk->client_id,TRUE, gas_id);
#else
            grm_release(l1_tskisr_blk->client_id,gas_id);
#endif
            grm_set_trm_grant_status(TRM_DENIAL, l1_tskisr_blk->client_id, gas_id);
            trm_grant_status = TRM_DENIAL;
            l1_rach_timer_start(gas_id);
            l1_tskisr_blk->main_command= L1_NULL_COMMAND;
            gl1_set_rach_type(NO_RACH, gas_id);
            return;
          }
          /* update this flag only if the RACH is going to proceed */
          re_calculate_initial_rach[gas_id] = FALSE;
        
          grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id,gas_id);
          grm_retain_lock( l1_tskisr_blk->client_id, (grm_unlock_callback_t)unlock_callback, gas_id );
        }
        else
        {
           MSG_GERAN_ERROR_0_G("TRM denial for PH_RANDOM_ACCESS_REQ");
           /* start timer before issuing trm request to combat race condition bewteen
           grant call back and starting timer
           */
           l1_rach_timer_start(gas_id);
           grm_setup_access_retry(FALSE, packetservice, reason,req_type ,geran_procedure, duration, gas_id);           
        }
        
        gl1_set_rach_type(NO_RACH, gas_id);
    }
    if (((trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS)) &&
         (tx_trm_status == GL1_TRM_GRANTED))
    {
      switch (l1_tskisr_blk->multi_sim_decoupled_state)
      {
         case L1_MULTI_SIM_DECOUPLED_WAIT_FOR_TRM_GRANT:
#ifdef FEATURE_GSM_QSC_TX_DIV
           gl1_hw_set_asdiv_mode_config(gas_id, TRM_CLIENT_MODE_IDLE_WAKEUP);	
#endif
           if ( l1_tskisr_blk->firmware_loaded == FALSE )
           {
             gl1_multi_sim_fw_rf_load(l1_tskisr_blk->client_id, gas_id);
             /* wait for fw loaded */
             l1_tskisr_blk->multi_sim_decoupled_state = L1_MULTI_SIM_DECOUPLED_WAIT_FOR_FW_LOAD;
             MSG_GERAN_HIGH_0_G("wait for fw load");
             break;
           }
         case L1_MULTI_SIM_DECOUPLED_WAIT_FOR_FW_LOAD:
           if (l1_tskisr_blk->firmware_loaded == TRUE)
           {
             idle_data_T  *idle_data_ptr;
             l1_max_rach_guard_time[gas_id] = L1_MAX_RANDOM_ACCESS_MODE_FRAMES;
             L1_transfer_new_data(gas_id);
             l1_change_state(L1_RANDOM_ACCESS_MODE, gas_id);
             l1_tskisr_blk->sub_state    = L1_ACCESS_INIT;
             l1_tskisr_blk->main_command = L1_NULL_COMMAND;
             gl1_drx_off(TRUE, gas_id);

             /*set timeslot according to CCCH_GROUP*/
             idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
             L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
             idle_data_ptr->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;

             l1_set_rach_read_all_AGCH(FALSE, gas_id);

             l1_set_access_isr_send_mac_sb_failure(FALSE, gas_id);

           } else
           {
             MSG_GERAN_HIGH_0_G("wait for fw load");
           }
           break;
         default:
           MSG_GERAN_HIGH_0_G("Unexpected decoupled state.");
           break;
      }
    }
  }
  else if ( (l1_tskisr_blk->main_command == L1_IDLE_COMMAND) ||
            (l1_tskisr_blk->main_command == L1_MULTI_SIM_IDLE_COMMAND ) )
  {
    idle_data_T  *idle_data_ptr;
    gl1_trm_state_t trm_status = GL1_TRM_NULL;
    l1_change_state(L1_MULTI_SIM_IDLE, gas_id);
	l1_sc_start( L1SCModeIdle, gas_id);
    l1_tskisr_blk->main_command = L1_NULL_COMMAND;
    l1_tskisr_blk->sub_state    = L1_IDLE_INIT;

    /* Inform GRR that L1 has transitioned to Idle */
    L1_send_MPH_START_IDLE_MODE_CNF(gas_id);

    /*set timeslot according to CCCH_GROUP*/
    idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
    L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
    trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
    if ((trm_status == GL1_TRM_GRANTED)
        || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
    {
      if(l1_tskisr_blk->firmware_loaded == TRUE)
      {
        call_in_task_grm_release(l1_tskisr_blk->client_id);
      }
      else
      {
        grm_release(l1_tskisr_blk->client_id, gas_id);
      }
    }
    else if(trm_status == GL1_TRM_CALLBACK_PENDING)
    {
      grm_release(l1_tskisr_blk->client_id, gas_id);
    }

  }
  else if ( l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
  {
    l1_change_state( L1_INACTIVE_MODE, gas_id );
    call_in_task_grm_release( l1_tskisr_blk->client_id );
  }
  else if ( l1_tskisr_blk->main_command == L1_FIND_BCCH_COMMAND )
  {
    l1_isr_handle_multi_sim_find_bcch_command(gas_id);
  }
  gl1_rach_mutex_unlock(gas_id);
  gl1_drx_require_next_tick(gas_id);
}

/*===========================================================================

FUNCTION  gl1_handle_trm_requests

DESCRIPTION
  Handles the issuing of TRM requests for sudden changes of state
  to access mode, dedicated mode, transfer mode. The issuing of the fw
  and rf loading is also contained within this state machine.

DEPENDENCIES
  l1_tskisr_blk,l1_isr_data

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_handle_grm_requests( grm_conn_type_t connection_type,
                                 boolean  reset_state,
                                 ARFCN_T           arfcn_info,
                                 grm_divrx_request_type req_type,
                                 gas_id_t gas_id )
{
  grm_client_enum_t client_id;

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  boolean proceed = FALSE;
  trm_reason_enum_t          reason = grm_get_connection_priority (connection_type,gas_id);
  trm_resource_enum_t       resource = gl1_ms_get_trm_resource(gas_id, reason);
  gl1_trm_state_t trm_status = GL1_TRM_NULL;

  if ( reset_state )
  {
    /* Reset the state machine - process is beginning */
    l1_multi_sim_data_ptr->trm_request_state = TRM_REQ_NULL;
  }

  client_id = l1_tskisr_blk->client_id;

  if((l1_tskisr_blk->firmware_load_status != FW_PENDING_RELEASE)&& (!gl1_hw_qta_gap_active( gas_id)) )
  {
   switch ( l1_multi_sim_data_ptr->trm_request_state )
   {
    case TRM_REQ_NULL:
    {
      sys_proc_type_e_type sub_reason;

      if (connection_type == GRM_CS_CONNECTION)
      {
        sub_reason = gl1_get_specific_sys_proc(GERAN_CONN_PROCEDURE, gas_id);
      }
      else
      {
        sub_reason = gl1_get_specific_sys_proc(GERAN_GPRS_PROCEDURE, gas_id);
      }/*Else PS Connection */

      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
      /* CHECK - if we have the lock if not then ask for it */
      if ( ( trm_status != GL1_TRM_GRANTED ) &&
           ( trm_status != GL1_TRM_RETAINED_FOR_ACCESS )
         )
      {
        /* Preload grm data with active freq band info */
        (void)grm_add_arfcn_to_trm_freq_input_type( arfcn_info,
                                                    client_id,
                                                    gas_id );

        grm_set_trm_grant_status(TRM_DENIAL, client_id, gas_id);

          if ( l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND )
          {
            /* In transfer do a single request and fail immediately */
            MSG_GERAN_ERROR_0_G("XFER lock request has failed");

            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          }
          else
          {
            grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;
            grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
            grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

            memset( &grm_request_and_notify_enhanced_data,NULL,sizeof(grm_request_and_notify_enhanced_data) );

            /* Ask and wait for a callback when granted. The duration is irrelevant here */
            /* as its unknown and is highest priority anyway. */
            grm_request_and_notify_enhanced_data_p->client_id = client_id;
            grm_request_and_notify_enhanced_data_p->resource  = resource;
            grm_request_and_notify_enhanced_data_p->duration  = GRM_ACCESS_DURATION_SCLKS;
            grm_request_and_notify_enhanced_data_p->reason    = reason;
            grm_request_and_notify_enhanced_data_p->sub_reason = sub_reason;
            grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_DENIED;

            grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, client_id, gas_id);
            l1_multi_sim_data_ptr->trm_request_state = TRM_REQ_WAITING;

            call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );
          }
        /* jump out of this state now. we are waiting for a callback */
          break;
        }
        else
        {
        grm_info_t *grm_info_p             = grm_get_info_ptr_from_client( client_id, gas_id );
        trm_modify_chain_state_return_data *modify_state_info_p = NULL;

        /* If we already have the lock at this point then tell TRM the reason change */
            grm_change_priority( client_id, reason, sub_reason, gas_id );
            modify_state_info_p = grm_get_modify_state_info(client_id, gas_id);

          if (modify_state_info_p)
           {
            MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR" GRM change priority to %d result %d ",
                                 reason ,
                                 modify_state_info_p->modify_info.mod_reason_info.result);
           }

        if ( grm_info_p && modify_state_info_p &&
             ((modify_state_info_p->modify_info.mod_reason_info.result == TRM_PENDING ) ||
              (modify_state_info_p->modify_info.mod_reason_info.result == TRM_DENIAL ))
          )
        {
          grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

          grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;

          grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

          memset( &grm_request_and_notify_enhanced_data,NULL,sizeof(grm_request_and_notify_enhanced_data) );

          MSG_GERAN_HIGH_1_G( GL1_DRDS_DBG_HDR"release chain and send req and notify to TRM for client id %d ",
                              client_id );

          /* Ask and wait for a callback when granted. The duration is irrelevant here */
          /* as its unknown and is highest priority anyway. */
          grm_request_and_notify_enhanced_data_p->client_id = client_id;
          grm_request_and_notify_enhanced_data_p->resource  = resource;
          grm_request_and_notify_enhanced_data_p->duration  = GRM_ACCESS_DURATION_SCLKS;
          grm_request_and_notify_enhanced_data_p->reason    = reason;
          grm_request_and_notify_enhanced_data_p->sub_reason = sub_reason;
          grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_DENIED;

          grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, client_id, gas_id);
          l1_multi_sim_data_ptr->trm_request_state = TRM_REQ_WAITING;

          call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );
          /* jump out of this state now. TRM has either denied or we are waiting for a callback */
          break;
        }
        else
        {
          grm_check_connected_reason (client_id, connection_type, gas_id);
        }
      }

      /* TRM request was successful, if FW is loaded immediately proceed otherwise start
         the FW load and move states */
      if ( l1_tskisr_blk->firmware_loaded )
      {
        proceed = TRUE;
      }
      else
      {
        gl1_multi_sim_fw_rf_load( client_id, gas_id );

        l1_multi_sim_data_ptr->trm_request_state = TRM_GRANT_FW_LOAD_WAIT;
      }
    }
    break;

    case TRM_REQ_WAITING:
    {
      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
      if ( ( trm_status == GL1_TRM_GRANTED ) ||
           ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS ) )
      {
        /* TRM request was successful, if FW is loaded immediately proceed otherwise start
          the FW load and move states */
        if ( l1_tskisr_blk->firmware_loaded == TRUE )
        {
          proceed = TRUE;

          l1_multi_sim_data_ptr->trm_request_state = TRM_REQ_NULL;
        }
        else
        {
          gl1_multi_sim_fw_rf_load( client_id, gas_id );

          l1_multi_sim_data_ptr->trm_request_state = TRM_GRANT_FW_LOAD_WAIT;
        }
      }
    }
    break;

    case TRM_GRANT_FW_LOAD_WAIT:
    {
      if ( l1_tskisr_blk->firmware_loaded == TRUE )
      {
        l1_multi_sim_data_ptr->trm_request_state = TRM_REQ_NULL;
        /* Prevent sleep immediately after loading F/W which was seen if
        the FEE estimator interrupts at this point */
        gl1_drx_require_next_tick(gas_id);
        proceed = TRUE;
      }
    }
    break;

   default:
    MSG_GERAN_ERROR_1_G("trm_request_state %d UNKNOWN",l1_multi_sim_data_ptr->trm_request_state);
    l1_multi_sim_data_ptr->trm_request_state = TRM_REQ_NULL;
    break;
   }
  }

  /* Prevent sleep immediately after loading F/W  */
  gl1_drx_require_next_tick(gas_id);

  return proceed;
}

/*===========================================================================

FUNCTION  l1_init_multi_sim_data

DESCRIPTION
  Initialises the DUAL SIM data

DEPENDENCIES
  l1_tskisr_blk, l1_tskisr_blk_1

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_init_multi_sim_data( gas_id_t gas_id )
{
  l1_tsk_buffer[gas_id].firmware_loaded      = FALSE;

  grm_set_trm_status(GL1_TRM_NULL, l1_tsk_buffer[gas_id].client_id, gas_id);
  grm_set_trm_status(GL1_TRM_NULL,
                     grm_map_prx_to_divrx_client_id(l1_tsk_buffer[gas_id].client_id,gas_id),
                     gas_id);

#ifdef FEATURE_GSM_RX_TX_SPLIT
  grm_configure_tx_info(l1_tsk_buffer[gas_id].client_id, gas_id);
  grm_set_trm_status(GL1_TRM_NULL,
                     grm_map_rx_to_tx_client_id(l1_tsk_buffer[gas_id].client_id,gas_id),
                     gas_id);
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  l1_tsk_buffer[gas_id].firmware_load_status = FW_UNLOADED;

  grm_set_trm_grant_fn(NOT_A_FRAME_NUMBER, l1_tsk_buffer[gas_id].client_id, gas_id);
  grm_set_trm_grant_fn(NOT_A_FRAME_NUMBER,
                       grm_map_prx_to_divrx_client_id(l1_tsk_buffer[gas_id].client_id,gas_id),
                       gas_id);

  grm_set_trm_grant_frames(0, l1_tsk_buffer[gas_id].client_id, gas_id);
  grm_set_trm_grant_frames(0,
                           grm_map_prx_to_divrx_client_id(l1_tsk_buffer[gas_id].client_id,gas_id),
                           gas_id);


  grm_set_trm_release_required(FALSE, l1_tsk_buffer[gas_id].client_id, gas_id);
  grm_set_trm_release_required(FALSE,
                               grm_map_prx_to_divrx_client_id(l1_tsk_buffer[gas_id].client_id,gas_id),
                               gas_id);

  l1_tsk_buffer[gas_id].time_slot_change_pending = FALSE;
  l1_tsk_buffer[gas_id].reservation_for_bcch = FALSE;

#ifdef FEATURE_QTA
  l1_tsk_buffer[gas_id].qta_in_progress      = FALSE;
#endif /* FEATURE_QTA */

  /* Ensure the TRM resource lock was not left in locked state */
  grm_set_resource_lock_state(FALSE,gas_id);

#if defined ( FEATURE_DUAL_SIM ) && defined ( FEATURE_CRAT_MSIM_TEST_FRAMEWORK ) 
  grm_clear_els_active();
#endif /* FEATURE_DUAL_SIM | FEATURE_CRAT_MSIM_TEST_FRAMEWORK */

}

/*===========================================================================

FUNCTION l1_swap_multi_sim_sync_state_or_deactivate

DESCRIPTION
  This function handles the deactivation of one of the dual sims
  If the SIM that is deactivating is the SYNC SIM, and there is
  an ASYNC SIM this must become the SYNC SIM.

DEPENDENCIES
  This function needs to be run at the end of the isr. Beacuse if the sync sim
  enters inactive mode this is the trigger to pull down the isr. If there are two sims
  and one becomes inactive we do not want to tear down the isr.

  +++++ This must occur in task context due to the call to grm_release_or_exchange() +++++

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void l1_swap_multi_sim_sync_state_or_deactivate( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gl1_trm_state_t trm_status =  GL1_TRM_NULL;

  /* This code manages the DUAL SIMs flags and pointers,
   1. if the SIM that is deactivating is the SYNC SIM, and there is
   an ASYNC SIM this must become the SYNC SIM.
   2. if the SIM that is deactivating is the ASYNC SIM and then NULL the
   async_sim_ptr only - minimum management required
   3. if the SIM is the SYNC SIM and no other sim is active, NULL the
   sync_sim_ptr and reset the multi_sim_mode flag */

  GL1_ISR_LOCK(gas_id);

  trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

  /* Firstly run the "Catch all" upon deactivation to prevent any holding onto the lock unncessarily */
  if ( l1_tskisr_blk->firmware_loaded &&
       ((trm_status == GL1_TRM_GRANTED) ||
        (trm_status == GL1_TRM_RETAINED_FOR_ACCESS)) )
  {
      /* deactivate */
      gl1_hw_multi_sim_deact(gas_id);
      l1_tskisr_blk->firmware_loaded = FALSE;
  }

  /* release the currently held RF chain, or exchange it */
  grm_release_or_exchange( l1_tskisr_blk->client_id, gas_id );

  grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);

  l1_init_multi_sim_data(gas_id);

  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION   call_in_task_grm_request_and_notify_enhanced

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   trm_request_and_notify_enhanced().

   IN - data - The data to be passed to the function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void call_in_task_grm_request_and_notify_enhanced(
       grm_request_and_notify_enhanced_data_t *params_p )
{
  grm_request_and_notify_enhanced_data_t msg;
  gas_id_t                               gas_id;

  if ( !params_p )
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"call_in_task_grm_request_and_notify_enhanced no valid params_p"  );

    return;
  }

  /* load the outgoing message with the supplied parameters */
  memscpy( &msg,
           sizeof(grm_request_and_notify_enhanced_data_t),  /* destination size */
           params_p,
           sizeof(grm_request_and_notify_enhanced_data_t) ); /* source size */

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_GRM_REQ_AND_NOTIFY_ENH;

  PUT_IMH_LEN( sizeof( grm_request_and_notify_enhanced_data_t ) - sizeof( IMH_T ), &msg );

  gas_id                 = gl1_map_client_id_to_gas_id( params_p->client_id );

  msg.gas_id             = gas_id;

  if ( gas_id < GERAN_ACCESS_STRATUM_ID_UNDEFINED )
  {
    grm_set_trm_release_required(FALSE, params_p->client_id, gas_id);
  }

  /* When we are in idle mode we need to tell the DRX manager
   * when we send a callback to the task so it knows that the
   * task is active.  This call is ignore when we are not in
   * Idle Mode because the DRX manager is off.
   */
  gl1_drx_task_active( gas_id );

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
}

/*===========================================================================

FUNCTION   call_in_task_grm_release

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   trm_release().

   IN - data - The data to be passed to the function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void call_in_task_grm_release(grm_client_enum_t client_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
  idle_data_T             *idle_data_ptr;
  grm_release_data_t       msg;

  /* Set gas_id to message client_id request */
  gas_id_t                 gas_id = gl1_map_client_id_to_gas_id( client_id );
  trm_get_info_return_type get_granted;

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_GRM_RELEASE;

  msg.client_id          = client_id;

  msg.gas_id             = gas_id;

#if defined (FEATURE_QTA ) && defined (FEATURE_CXM_QTA)
  if( l1_tskisr_blk->qta_in_progress )
  {
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
  }
#endif /* FEATURE_QTA */

  if(l1_tskisr_blk->time_slot_change_pending)
  {
    L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
    l1_tskisr_blk->time_slot_change_pending = FALSE;
  }

#ifdef FEATURE_GPRS_GBTA
  if ( ( grm_get_trm_status(client_id, gas_id) == GL1_TRM_GRANTED ) &&
       ( TRM_DENIAL == grm_get_granted( client_id, &get_granted, gas_id ) ) )
  {
    gpl1_gbta_reset_active_state( gas_id);
  }
#endif

  grm_set_trm_release_required(FALSE, client_id, gas_id);

  if (!grm_get_resource_lock_state(gas_id)) 
  {
    l1_tskisr_blk->firmware_load_status = FW_PENDING_RELEASE; 
  }

  MSG_GERAN_HIGH_1_G( "ISR CALL to call_in_task_grm_release for client_id:%d",
                      client_id );

  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  /* When we are in idle mode we need to tell the DRX manager
   * when we send a callback to the task so it knows that the
   * task is active.  This call is ignore when we are not in
   * Idle Mode because the DRX manager is off.
   */
  gl1_drx_task_active(gas_id);

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
}

/*===========================================================================

FUNCTION   call_in_task_grm_release_in_callback

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   trm_release().
   This is a streamlined version call_in_task_grm_release(). It does not
   contain any function requires GL1 lock, can be called from TRM callback
   functions to avoid mutex deadlock.

   IN - data - The data to be passed to the function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void call_in_task_grm_release_in_callback(grm_client_enum_t client_id )
{
  grm_release_data_t       msg;

  /* Set gas_id to message client_id request */
  gas_id_t                 gas_id = gl1_map_client_id_to_gas_id( client_id );

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_GRM_RELEASE_IN_CALLBACK;

  msg.client_id          = client_id;

  msg.gas_id             = gas_id;

  MSG_GERAN_HIGH_1_G( "ISR CALL to call_in_task_grm_release_in_callback for client_id:%d",
                      client_id );

  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );

}


/*===========================================================================

FUNCTION   call_in_task_grm_enable_disable_dual_receive

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   grm_enable_disable_dual_receive to disable DR.

   IN - data - The data to be passed to the function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void call_in_task_grm_enable_disable_dual_receive(grm_client_enum_t client_id,
                                                         boolean dr_setting)
{
  grm_disable_enable_dr_t       msg;

  /* Set gas_id to message client_id request */
  gas_id_t                 gas_id = gl1_map_client_id_to_gas_id( client_id );

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_GRM_DISABLE_ENABLE_DR;

  msg.client_id          = client_id;

  msg.gas_id             = gas_id;
  msg.dr_setting         = dr_setting;

  MSG_GERAN_HIGH_1_G( "ISR CALL to call_in_task_grm_enable_disable_dual_receive for client_id:%d",
                      client_id );

  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );

}


/*===========================================================================

FUNCTION   call_in_task_grm_reserve_at

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   trm_release().

   IN - data - The data to be passed to the function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void call_in_task_grm_reserve_at( grm_reserve_at_data_t data )
{
  trm_get_info_return_type  get_granted;

  gas_id_t                  gas_id = data.gas_id;

  volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T              *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

  grm_reserve_at_data_t msg = data;

  gas_id = check_gas_id(gas_id);

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_GRM_RESERVE_AT;
  msg.client_id          = l1_tskisr_blk->client_id;
  msg.gas_id             = gas_id;

#if defined (FEATURE_QTA ) && defined (FEATURE_CXM_QTA)
  if( l1_tskisr_blk->qta_in_progress )
  {
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
  }
#endif /* FEATURE_QTA */

  if(l1_tskisr_blk->time_slot_change_pending)
  {
    L1_change_timeslot((uint8)(idle_data_ptr->paging_data.ccch_group << 1), gas_id);
    l1_tskisr_blk->time_slot_change_pending = FALSE;
  }

#ifdef FEATURE_GPRS_GBTA
  if((grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_GRANTED) &&
     ( TRM_DENIAL == grm_get_granted(l1_tskisr_blk->client_id, &get_granted, gas_id) ))
  {
    gpl1_gbta_reset_active_state( gas_id);
  }
#endif

  MSG_GERAN_HIGH_3_G("ISR CALL to call_in_task_grm_reserve_at (overhead %d) for GAS ID 0x%02X client_id %d",
                     MULTI_SIM_FW_LOAD_FRAMES, (int)gas_id, (int)msg.client_id);

  grm_set_trm_release_required(FALSE, l1_tskisr_blk->client_id, gas_id);
  if((!gl1_hw_sleep_wmgr_cancel_in_prog[gas_id])&&(!grm_get_resource_lock_state(gas_id)))
  {
    l1_tskisr_blk->firmware_load_status = FW_PENDING_RELEASE;
  }
  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

    gl1_drx_task_active(gas_id);

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
}

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION   call_in_task_trm_freq_reserve_at_self_ccch

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   trm_reserve_at().



DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void call_in_task_grm_reserve_at_self_ccch( grm_reserve_at_data_t data)
{
  gas_id_t gas_id = data.gas_id;
  grm_reserve_at_data_t msg = data;

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_GRM_RESERVE_AT_CCCH;
  msg.client_id          = data.client_id;
  msg.gas_id             = data.gas_id;

  MSG_GERAN_HIGH_2_G("PCHXFER ISR CALL to call_in_task_grm_reserve_at for GAS ID 0x%02X client_id %d",
                    (int)msg.gas_id, (int)msg.client_id);

  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  ( void )L1_isr_send_message( l1_queues[msg.gas_id], &msg, TRUE, msg.gas_id );
}

/*===========================================================================

FUNCTION   gl1_trans_grm_reserve_at_self_ccch

DESCRIPTION

   trm_reserve_at().



DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_trans_grm_reserve_at_self_ccch( uint32 required_frame_nbr, gas_id_t gas_id )
{
  grm_reserve_at_data_t grm_freq_reserve_at_data;

  /* calculate the required parameters and pass in to TRM in the reservation request */
  grm_freq_reserve_at_data.frames_required    = required_frame_nbr;
  grm_freq_reserve_at_data.frame_duration     = gpl1_g2x_ta_data[gas_id].table[G2X_TA_PRI_SERV_PCH].duration;
    grm_freq_reserve_at_data.reason           = TRM_DEMOD_PAGE;
  grm_freq_reserve_at_data.client_id          = gl1_map_gas_id_to_client_id_pchxfer(gas_id);
  grm_freq_reserve_at_data.gas_id             = gas_id;
  grm_freq_reserve_at_data.resource           = gl1_ms_get_trm_resource(gas_id, TRM_DEMOD_PAGE);

  grm_freq_reserve_at_data.duration           = gpl1_multi_sim_convert_frames_to_sclcks(grm_freq_reserve_at_data.frame_duration);

  MSG_GERAN_MED_2_G( "TRM PCHXFER- grm_reserve_at- client_id %d trm_status:%d",
                     grm_freq_reserve_at_data.client_id,
                     grm_get_trm_status(grm_freq_reserve_at_data.client_id, gas_id));

  call_in_task_grm_reserve_at_self_ccch(grm_freq_reserve_at_data);
}
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

/*===========================================================================

FUNCTION   call_in_task_gplusg_deactivate

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   that will deactivate the mdsp and rf

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void call_in_task_gplusg_deactivate( gas_id_t gas_id )
{
  multi_sim_gonly_deactivate_t msg;

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_GPLUSG_DEACTIVATE;
  msg.gas_id             = gas_id;

  MSG_GERAN_HIGH_0_G("ISR CALL to call_in_task_gonly_deactivate");

  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  /* When we are in idle mode we need to tell the DRX manager
   * when we send a callback to the task so it knows that the
   * task is active.  This call is ignore when we are not in
   * Idle Mode because the DRX manager is off.
   */
  gl1_drx_task_active(gas_id);

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
}

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*===========================================================================

FUNCTION   l1_set_global_override_trm_reason

DESCRIPTION
   Sets a TRM reson to be used in IDLE mode, that will oveeride the normal
   expected reasons. This will mean that instead of usaing the TRM_DEMOD_PAGE
   the reason can be set elsewhere to increase the priority to TRM_RESELECT or
   TRM_DEMOD_PAGE_MAX_SENS.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_set_global_override_trm_reason( trm_reason_enum_t reason , gas_id_t gas_id)
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  l1_multi_sim_data_ptr->global_trm_reason.valid_flag = TRUE;
}

/*===========================================================================

FUNCTION gl1_get_trm_global_overide_status

DESCRIPTION
  Returns the SUB global override status for trm reason code

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_get_trm_global_overide_status( gas_id_t gas_id )
{
 return ( l1_multi_sim[gas_id].global_trm_reason.valid_flag );
}

/*===========================================================================

FUNCTION   l1_clear_global_override_trm_reason

DESCRIPTION
   Clears out the override reason for the TRM reservation request.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_clear_global_override_trm_reason( gas_id_t gas_id )
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  l1_multi_sim_data_ptr->global_trm_reason.valid_flag = FALSE;
}

#endif
/*===========================================================================

FUNCTION   gpl1_set_invert_sys_info_trm_priority

DESCRIPTION
   On reception of GERAN_SIB_PROCEDURE GRR-> GL1
   Sets invert_sys_info_priority. If:
   TRUE then Channel Maintanence will be Inverted to Channel Maintanence Inv for reading of
         Serving Cell BCCHthat will oveeride the normal
   FALSE then Channel Maintanence priority will be used for reading of
         Serving Cell BCCH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_set_invert_sys_info_trm_priority(boolean invert_sys_info_priority,  gas_id_t gas_id)
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  l1_multi_sim_data_ptr->invert_sys_info_priority = invert_sys_info_priority;
}

boolean gpl1_sys_info_trm_priority_inverted(gas_id_t gas_id)
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  return l1_multi_sim_data_ptr->invert_sys_info_priority;
}


/*===========================================================================

FUNCTION   l1_get_global_override_trm_reason

DESCRIPTION
   Gets a TRM reson to be used in IDLE mode, that will overide the normal
   expected reasons. If no override reason is set then TRM_DEMOD_PAGE is
   the default.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
trm_reason_enum_t l1_get_global_override_trm_reason( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];
  l1_local_idle_data_t      *l1_local_idle_data_p = l1_get_l1_local_idle_data(gas_id);

  trm_reason_enum_t return_reason = TRM_DEMOD_PAGE;
  sys_proc_type_e_type sub_reason = SYS_PROC_TYPE_PAGE;

  if( l1_tskisr_blk->current_params.L1Data.pIdle_data != NULL )
  {
    if ( ( l1_tskisr_blk->current_params.L1Data.pIdle_data->mm_non_drx_mode ) ||
     ( l1_tskisr_blk->current_params.L1Data.pIdle_data->paging_data.all_paging_blocks ) ||
     ( l1_local_idle_data_p->l1_idle_non_drx_timer_flag ))
    {
      /* NON-DRX page read or Page re-organization mode */
      return_reason = TRM_CHANNEL_MAINTENANCE_INV;
      sub_reason = SYS_PROC_TYPE_PAGE;
    }
    else if ( l1_tskisr_blk->reservation_for_bcch  == TRUE )
    {
      /* BCCH read */
      return_reason = TRM_CHANNEL_MAINTENANCE;
        sub_reason = SYS_PROC_TYPE_SYSTEM_INFO_READING;
    } 
    else if ( l1_tskisr_blk->current_params.L1Data.pIdle_data->paging_data.paging_mode == EXTENDED_PAGING )
    {
      /* Extended paging */
      sub_reason = SYS_PROC_TYPE_EXTENDED_PAGE;
      MSG_GERAN_HIGH_0_G("paging_mode == EXTENDED_PAGING");
    }
  }

  if((l1_tskisr_blk->current_params.L1Data.pIdle_data->mm_non_drx_mode ) 
     && (TRUE == gl1_is_geran_tuneaway_allowed(gas_id)) &&
        (FALSE == l1_tskisr_blk->current_params.L1Data.pIdle_data->non_updated_lai_in_nmo1))
  {
      return_reason = TRM_ACCESS;
  }

  /* this reason may have been bumped up if we are performing a cell reselection */
  if ( ( l1_multi_sim_data_ptr->invert_sys_info_priority ) &&
       ( l1_tskisr_blk->reservation_for_bcch  == TRUE ) )
  {
    /* Use CM INV for high-priority Sys info reading */
    return_reason = TRM_CHANNEL_MAINTENANCE_INV;
        sub_reason = SYS_PROC_TYPE_SYSTEM_INFO_READING;
  }

   if (return_reason != TRM_DEMOD_PAGE)
   {
    MSG_GERAN_HIGH_1_G("GL1 global override %d",  (int)return_reason);
   }
  gl1_store_grm_sub_reason(sub_reason , gas_id);
  return return_reason;
}


/*===========================================================================

FUNCTION   gl1_multi_sim_fw_rf_load

DESCRIPTION
   Sends a message to the layer1 task telling it to call the function
   to action the load of the firmware and RF modules

   IN - data - The data to be passed to the function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_multi_sim_fw_rf_load( grm_client_enum_t client_id, gas_id_t gas_id  )
{
  multi_sim_rf_fw_load_t msg;

  msg.client_id          = client_id;
  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_MULTI_SIM_FW_RF_LOAD;
  msg.gas_id             = gas_id;

  MSG_GERAN_HIGH_2_G("ISR CALL to call_in_task_multi_sim_fw_rf_load for GAS ID 0x%02X client_id %d",(int)msg.gas_id, (int)msg.client_id);


  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  /* When we are in idle mode we need to tell the DRX manager
   * when we send a callback to the task so it knows that the
   * task is active.  This call is ignore when we are not in
   * Idle Mode because the DRX manager is off.
   */
  gl1_drx_task_active(gas_id);

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
}

/*===========================================================================

FUNCTION  gl1_handle_abort_callback

DESCRIPTION
  This function handles the TRM abort request for a GSM client.
  It changes the trm_status within the client's global data, and
  this is subsequently picked up by the ISR.

DEPENDENCIES
  Must be called in task context, must not be used for GPRS Tuneaway gaps

RETURN VALUE
  VOID

SIDE EFFECTS
  None

===========================================================================*/
void gl1_handle_abort_callback(grm_client_enum_t       client,
       trm_unlock_event_enum_t event,
       uint32                  unlock_by_sclk,
       gas_id_t                gas_id )
{
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* sanity check the GAS ID before proceeding any further */
  if (gas_id > GERAN_ACCESS_STRATUM_ID_MAX)
  {
    MSG_GERAN_ERROR_2("Abort callback for bad GAS ID 0x%02X, client %d",
                      (int)gas_id,
                      (int)client);
    return;
  }

  GL1_ISR_LOCK(gas_id);

  trm_status = grm_get_trm_status(client, gas_id);
  /* sanitise the input parameters */
  /* Receive abort call back for Div RX ??? */
  if (l1_tsk_buffer[gas_id].client_id != client && !gl1_is_client_for_bplmn(client) )
  {
    MSG_GERAN_ERROR_2_G("Client %d received in abort callback, expected %d",
                        (int)client, (int)(l1_tsk_buffer[gas_id].client_id));
  }
  else
  if ((trm_status != GL1_TRM_GRANTED) && (trm_status != GL1_TRM_RETAINED_FOR_ACCESS))
  {
    if ((L1_MULTI_SIM_INACTIVE  == l1_tsk_buffer[gas_id].l1_state)   &&
        (GL1_TRM_ABORTING       == trm_status) &&
        ( ( ( (TRM_UNLOCK_REQUIRED == event) || (TRM_UNLOCK_BAND_INCOMPATIBLE == event) )
        || (TRM_UNLOCK_IMMEDIATELY == event)
        )))
    {
      /* panic reset is repeatedly trying to abort this client */
      MSG_GERAN_HIGH_3_G("TRM_UNLOCK_IMMEDIATELY for client %d lock state %d event %d",
                         (int)client, (int)(trm_status), (int)event);

      call_in_task_grm_release( client );
    }
    else
    {
      /* client's local status is not GL1_TRM_GRANTED, so an abort is not expected */
      MSG_GERAN_ERROR_3_G("Client %d, status %d not expecting unloc event %dk",
                (int)client, (int)(trm_status),(int)event);
    }
  }
    /*Ignore handling of Abort Callback during IDLE to TRANSFER Intermediate states. Once Gl1 moved to Transfer,Gl1 does retain lock and again abort callback will be received.
     and Gl1 can handle Abort callback Properly. */
  else if(((trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS)) &&
       ((l1_tsk_buffer[gas_id].main_command == L1_TRANSFER_COMMAND_WAIT_FOR_TRM) ||(l1_tsk_buffer[gas_id].main_command == L1_TRANSFER_COMMAND_WAIT_FOR_FW)
       ||(l1_tsk_buffer[gas_id].main_command == L1_TRANSFER_COMMAND_IDLE_ABORTED)))
  {
           MSG_GERAN_HIGH_0_G("Ignore TRM Abort During IDLE-->TRANSFER intermediate States");
  }
  else
  {
     if ( (TRM_UNLOCK_REQUIRED == event) || (TRM_UNLOCK_BAND_INCOMPATIBLE == event)
        || (TRM_UNLOCK_IMMEDIATELY == event)
        )
    {
      /* kill the process immediately */
      MSG_GERAN_HIGH_3_G("TRM_UNLOCK_IMMEDIATELY for client %d, status %d",
                         (int)client, (int)(trm_status), (int) event);

      grm_set_trm_status(GL1_TRM_ABORTING, client, gas_id);
    } else if (TRM_UNLOCK_BY == event)
    {
      uint32 frame_duration = 0;
      uint32 frames_to_own_ccch = MAX_GAP;
      /* setup the given unlock time */
      unlock_by_sclk -= timetick_get_safe();

      /* Convert sclks to number of GSM frame. Release TRM one frame ealier */
      frame_duration = MSECS_TO_FRAMES(timetick_cvt_from_sclk(unlock_by_sclk, T_MSEC))-1;

      if (frame_duration > MULTI_SIM_FW_LOAD_FRAMES)
      {
        /* in multisim mode we need to take into account of
         * the new client loading FW - this will directly reduce the
         * number of frames
         */
        frame_duration -= MULTI_SIM_FW_LOAD_FRAMES;
      }

      MSG_GERAN_HIGH_3_G("TRM_UNLOCK_BY %d frames Abort for Client %d from TRM sclk:%d", frame_duration, client, unlock_by_sclk);


      if (frame_duration > (REDUCE_FRAMES_TASK_TO_ISR_LATENCY + SUSPENSION_FRAME_DELAY))
      {
        if (l1_idle_data_store[gas_id].g_bplmn_running_state == BPLMN_ACTIVE)
        {
          frames_to_own_ccch = l1_fm_get_available_frames(FM_PRI_BACKGROUND_PLMN, gl1_get_FN(gas_id), gas_id);
          MSG_GERAN_HIGH_1_G("frames_to_own_ccch %d ", frames_to_own_ccch);
          frame_duration = MIN(frames_to_own_ccch, frame_duration);
          frame_duration -= BPLMN_RELEASE_OVERHEAD;
        }
        else if (!(l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH && l1_tskisr_blk->sub_state == L1_MULTI_SIM_AQUIRE_BCCH_LIST))
        /*No need to decrement susp fn dur for find_bcch state.*/
        {
          frame_duration -= (REDUCE_FRAMES_TASK_TO_ISR_LATENCY + SUSPENSION_FRAME_DELAY);
        }

        grm_set_frame_duration(frame_duration, client, gas_id);

        if ((l1_idle_data_store[gas_id].g_bplmn_running_state == BPLMN_ACTIVE) && (l1_tskisr_blk->firmware_load_status != FW_PENDING_RELEASE))
        {
          l1_fm_extend_required_frames(FM_PRI_BACKGROUND_PLMN, frame_duration, gas_id);
        }
      }
      else
      {
        grm_set_frame_duration(0, client, gas_id);
      }
    }
    else
    {
      MSG_GERAN_ERROR_2_G("Unhandled unlock event:%d for client:%d", (int)event, (int)client);
    }
  }

  GL1_ISR_UNLOCK(gas_id);

  return;
}


#if 0
/*===========================================================================

FUNCTION  grm_set_maximum_permissible_disruption

DESCRIPTION
  Stores the maximum amount of time (in 32kHz slow clock units) that the
  specified client can tolerate GERAN retaining the lock during acquisition

DEPENDENCIES
  This should not be called often. It is intended for system level use-cases
  where particular techs might have different response requirements. The intent
  here is that the function is used *at most* once per client, e.g. at power-on

RETURN VALUE
  VOID

SIDE EFFECTS
  None

===========================================================================*/
static void grm_set_maximum_permissible_disruption(
              const trm_client_enum_t client_id,
              const timetick_type new_limit )
{
  uint32 client_index = (uint32)client_id;

  if (client_index < TRM_MAX_CLIENTS)
  {
    grm_client_disruption_limits[client_index] = new_limit;
  }

  return;
}
#endif /* code currently compiled out : for future expansion only */

/*===========================================================================

FUNCTION  gl1_handle_grant_callback

DESCRIPTION
  This function handles the TRM grant callback for GSM clients. It changes
  the TRM status in the clients global data, which is picked up by the ISR.

DEPENDENCIES
  Must be called in task context, must not be used for GPRS Tuneaway gaps

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_handle_grant_callback(grm_client_enum_t          client,
       trm_duration_t             duration,
       gas_id_t                   gas_id )
{
/*The number of frames it takes off the allowed time, to tell ISR it has the TRM
  lock, and then again for the ISR to tell the task to release the lock. 1 Frame each way.*/
  volatile ISRTIM_CMD_BLK *l1_tsk_buffer_ptr;
  boolean                  release_trm = FALSE;
  boolean                  trm_not_granted = FALSE;  
  rfm_device_enum_type rf_device = RFM_INVALID_DEVICE;
  gas_id_t other_gas_id_transfer;
  grm_client_enum_t other_client_id_transfer;
  idle_data_T *l1_idle_data = &l1_idle_data_store[gas_id];
  trm_get_info_return_type  get_granted;
  trm_grant_return_enum_type grant_status;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client, gas_id);

  if (grm_info_p == NULL)
  {
    MSG_GERAN_ERROR_0_G("Null grm_info_p!");
    return;
  }

  /* update grant_status to be the TRUE status of TRM at this instant.
   * Handles race condition where for example, In callback context trm has
   * granted device and post message to l1 task.
   * Then set priority ind handling has released device and put in a new RaNE.
   * Potentially RaNE has not been granted. Then this handler runs for the initial
   * grant from grant callback.
   * Grant status that previously was passed in would have been GRANTED but really
   * its not because it got released by  priority ind handling
   */
  grant_status = grm_get_granted(client, &get_granted, gas_id );


  GL1_ISR_LOCK(gas_id);

#ifdef FEATURE_GSM_RX_TX_SPLIT
  if ((grm_is_a_tx_client(client)) &&
      (GL1_TRM_CALLBACK_PENDING == grm_get_trm_status(client, gas_id))
     )
  {
    grm_handle_tx_grant_callback(client, grant_status, gas_id);
	GL1_ISR_UNLOCK(gas_id);
	return;
  }
  else
#endif/*FEATURE_GSM_RX_TX_SPLIT*/
  {
    grm_set_req_and_notify_duration(INVALID_SCLKS, client, gas_id);

    l1_tsk_buffer_ptr = &l1_tsk_buffer[(int)gas_id];

    /* update trm_grant_status here */
    grm_set_trm_grant_status(grant_status, client, gas_id);

    /* Set the data inside the pointer */

    if (((l1_tsk_buffer_ptr->client_id  == client) || gl1_is_client_for_bplmn( client )
         || grm_is_a_secondary_client(client)) &&
      (grm_get_trm_status(client, gas_id) == GL1_TRM_CALLBACK_PENDING))
    {
      release_trm = FALSE;

      if ( grant_status != TRM_DENIAL )
      {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        if( gl1_hw_qta_gap_active(gas_id)
#ifdef FEATURE_GPRS_GBTA
          /* GBTA active case should not release trm if secondary.
             Also if other sub is not in transfer, TRM should not be released */
          || (IS_GBTA_ACTIVE() &&
             (grm_is_a_secondary_client(client) == FALSE) &&
              OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
#endif
       )
      {
        MSG_GERAN_HIGH_2_G("TRM grant(%d) call back for client %d is non QTA/GBTA activity, releasing TRM here",
          grant_status, client);
        release_trm = TRUE;
        trm_not_granted = TRUE;
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
        trm_reason_enum_t last_trm_reason;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        if(l1_tsk_buffer_ptr->l1_state == L1_MULTI_SIM_DECOUPLED)
        {
          /* GRANTED TRM stop the RACH timer*/
          l1_rach_timer_deactivate((gas_id_t)gas_id);
        }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
        duration -= timetick_get_safe();

        rf_device = grm_get_rf_device( client, gas_id );

        if (rf_device != RFM_INVALID_DEVICE)
        {
          uint32 frame_duration = MSECS_TO_FRAMES(timetick_cvt_from_sclk(duration, T_MSEC));

          if ( gl1_is_client_for_bplmn(client) &&
              (l1_idle_data->g_bplmn_running_state == BPLMN_ACTIVE) &&
              (frame_duration < (uint32)BPLMN_MIN_DURATION_SCAN_FRAMES ) )
          {
            /* when doing bplmn do not allow frame duration to be changed to less than
               BPLMN_MIN_DURATION_SCAN_FRAMES otherwise can end up with too few frames
               to do bplmn - setting trm status to not granted will allow bplmn state
               machine to re-request
             */
            gl1_hw_set_gsm_rf_id(RFM_INVALID_DEVICE, client, FALSE, gas_id);
            GL1_ISR_UNLOCK(gas_id);
            grm_release(client, gas_id);
            grm_set_trm_status(GL1_TRM_NOT_GRANTED, client, gas_id);

            MSG_GERAN_ERROR_2_G("bplmn dont grant client %d frame_duration %d",
                                client,
                                frame_duration);
            return;
          }

          if(rf_device != gl1_hw_get_gsm_rf_id(client, gas_id))
          {
            MSG_GERAN_ERROR_2_G("Device used by RF %d different to TRM granted device %d",
                                gl1_hw_get_gsm_rf_id(client, gas_id),
                                rf_device);
          }
          /*always update rf device ID on new grant from TRM*/
          gl1_hw_set_gsm_rf_id(rf_device, client, FALSE, gas_id);

          grm_set_trm_status(GL1_TRM_GRANTED, client, gas_id);

          /* need to set the trm status correctly if grant callback was received for ACCESS_URGENT
              * so that no other activity uses this TRM grant
            */
          last_trm_reason = grm_info_p->reason;
	
          if (GRM_ACQUISITION_REASON(last_trm_reason) ||
              GRM_ACQUISITION_DR_REASON(last_trm_reason))
          {
            geran_priority_t curr_priority;

            /* determine the current priority */
            if (GRM_INVERTED_ACQ_REASON(last_trm_reason))
            {
              curr_priority = GERAN_PRIORITY_INVERTED;
            }
            else
            {
              curr_priority = GERAN_PRIORITY_NOMINAL;
            }

            /* and then signal this grant to GRR */
            L1_send_MPH_ACQUISITION_STATE_IND(RR_L1_ACQ_RUNNING, curr_priority, gas_id);
          }
          else if((last_trm_reason == TRM_TRAFFIC)||(last_trm_reason == TRM_ACCESS)||(last_trm_reason == TRM_BG_TRAFFIC))
          {

            boolean rach_for_ps = FALSE;


            (void)geran_get_traffic_reason(&rach_for_ps, gas_id);

            /*allow diversity to be unlocked in CS/PS and RACH*/
            if(grm_is_a_secondary_client(client))
            {
              grm_retain_lock(client,(grm_unlock_callback_t)&grm_rx_diversity_unlock_cb,gas_id);
            }

            if (!rach_for_ps)
            {
              if (last_trm_reason == TRM_ACCESS)
              {
                /* Install unlock callback to allow CS signalling pre-emption by X traffic */
                grm_retain_lock(client,(grm_unlock_callback_t)&grm_cs_access_abort_callback,gas_id);
              }
              else if (last_trm_reason == TRM_TRAFFIC)
              {
                /* This shouldn't get called but at least we'll see the attempt if we install the callback */
                grm_retain_lock(client,(grm_unlock_callback_t)&grm_cs_traffic_abort_callback,gas_id);
              }
            }
          }


          MSG_GERAN_HIGH_3_G("Grant for Client %d from TRM for %d frames, rf_chain:%d",client,frame_duration,rf_device);

          if ( gl1_is_client_for_bplmn(client) && (l1_idle_data->g_bplmn_running_state == BPLMN_ACTIVE) &&
            !(l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH && l1_tskisr_blk->sub_state == L1_MULTI_SIM_AQUIRE_BCCH_LIST))
          {
            frame_duration = (BPLMN_MIN_DURATION_SCAN_FRAMES - BPLMN_RELEASE_OVERHEAD) ;
            /* instantly add the granted frames to frame manager */
            l1_fm_extend_required_frames(FM_PRI_BACKGROUND_PLMN, frame_duration, gas_id );
          }
          else
          {
            frame_duration -= (REDUCE_FRAMES_TASK_TO_ISR_LATENCY + SUSPENSION_FRAME_DELAY);
          }
          grm_set_frame_duration(frame_duration, client, gas_id);
        }
        else
        {
          MSG_GERAN_ERROR_0_G("Invalid Device given in Grant callback ");
          grm_set_trm_status(GL1_TRM_NOT_GRANTED, client, gas_id);
          MSG_GERAN_HIGH_1_G("Denial for Client %d from TRM",client);
          gl1_hw_set_gsm_rf_id(rf_device, client, FALSE, gas_id);
        }
      }
    }
    else
    {
      grm_set_trm_status(GL1_TRM_NULL, client, gas_id);
      MSG_GERAN_HIGH_1_G("Denial for Client %d from TRM",client);

      /*device id set to invalid as we don't have the grant from TRM*/
      gl1_hw_set_gsm_rf_id(rf_device, client, FALSE, gas_id);
    }

    gl1_drx_require_next_tick(gas_id);
  }
  else
  {
    /* this client is not waiting for the lock, clear the local status */
    MSG_GERAN_ERROR_0_G("Grant callback when not pending, releasing TRM");
    if( gl1_is_client_for_bplmn(client) &&
       (l1_idle_data->g_bplmn_running_state == BPLMN_ACTIVE) &&
       grm_get_trm_status(client, gas_id) == GL1_TRM_ABORTING )
    {
      MSG_GERAN_HIGH_0_G("Dont update TRM status here");
    }
  	else if(grant_status != TRM_GRANTED)
    {
      trm_not_granted = TRUE;
      if(grant_status != TRM_DENIAL)
      {
        release_trm = TRUE;
      }
    }

  }

  MSG_GERAN_HIGH_1_G("gl1_handle_grant_callback: RF device stored in gl1_hw: %d ",rf_device);

  GL1_ISR_UNLOCK(gas_id);

  /* do this outside of ISR_LOCK in case it invokes some other client's context */
  if ( release_trm )
  {
    grm_release( client, gas_id );
  }
  if ( trm_not_granted )
  {
    grm_set_trm_status(GL1_TRM_NOT_GRANTED, client, gas_id);
  }  
  }
  /*RANE was done with Serving Cell band for Diversity chain in Voice/packet data.
    Get Mapping of all bands supported by this device*/
  if (grm_is_a_secondary_client(client) && grm_get_trm_status(client, gas_id) == GL1_TRM_GRANTED)
  {
    grm_map_band_support_diversity_chain(client, gas_id);
  }
  return;
}


/*===========================================================================

FUNCTION  grm_setup_access_retry

DESCRIPTION


DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_setup_access_retry(boolean           pending_gl1_event,
                            ps_access_mode_t  access_for_ps,
                            trm_reason_enum_t reason,
                            grm_divrx_request_type    req_type,
                            geran_procedure_t geran_procedure,
                            trm_duration_t            duration,
                            gas_id_t          gas_id)
{
  trm_resource_enum_t  resource;
  grm_client_enum_t    client;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
//  trm_duration_t            duration = GRM_ACCESS_DURATION_SCLKS;

  client   = l1_tskisr_blk->client_id;

  if (access_for_ps == PS_ACCESS_NON_MM_DEVICE)
  {
    /* on non multimode sub*/
    resource = TRM_RX_BEST;
  }
  else if (access_for_ps == PS_ACCESS_MM_DEVICE)
  {
    resource = TRM_RX_BEST_POSSIBLE_MOD_2;
  }
  else
  {
    resource = gl1_ms_get_trm_resource( gas_id, reason );
  }

  /* Set the trm status to reflect the request */
  grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, client, gas_id);

  grm_set_ps_access_mode(access_for_ps, client, gas_id);

#ifdef FEATURE_GPRS_GBTA
  /* Block GBTA as were are doing RACH */
  if( OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) &&
      gl1_msg_get_multi_sim_standby_mode() )
  {
    SET_GBTA_STATUS(GRM_GBTA_BLOCKED);
    MSG_GERAN_MED_0_G("GBTA DUALSIM RACH REQUESTED: GRM_GBTA_BLOCKED");
  }
#endif  /* FEATURE_GPRS_GBTA */

  /* Request the lock now for this using the asynchronous TRM interface */
  grm_request_and_notify( client, resource,
                          GRM_ACCESS_DURATION_SCLKS,
                          reason,
                          req_type,
                          gl1_get_specific_sys_proc (geran_procedure, gas_id),
                          GRM_RAN_ACCESS_RETRY );

  return;
}


/*===========================================================================

FUNCTION  grm_enh_grant_panic_reset_callback

DESCRIPTION
  This function handles the TRM grant callback for panic reset. It changes
  the trm_status within the clients global data, this is picked up by the ISR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_enh_grant_panic_reset_callback(grm_client_enum_t          client,
  trm_grant_return_enum_type grant_status,
  trm_duration_t             duration
)
{
  multi_sim_panic_reset_t msg;

  gas_id_t gas_id = gl1_map_client_id_to_gas_id( client );

  MSG_GERAN_HIGH_1_G( "Panic reset grant callback for client:%d",
                      client );

  gas_id = check_gas_id(gas_id);

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_MULTI_SIM_PANIC_RESET;
  msg.gas_id             = gas_id;
  msg.error_code         = DS_error_code[gas_id];
  msg.reset_response     = DS_reset_response[gas_id];

  PUT_IMH_LEN( sizeof( multi_sim_panic_reset_t ) - sizeof( IMH_T ), &msg ); /*lint !e740 unusual pointer cast */

  /* vote against sleep until this message is processed by the task */
  gl1_drx_task_active(gas_id);

  ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );

  return;
}

/*===========================================================================

FUNCTION  l1_multi_sim_bplmn_abort

DESCRIPTION
  This function is used as the abort callback by the frame manager to ensure
  that the background activity cleanly completes. The action is
  simply to change the trm_status to GL1_TRM_ABORTING to force the state
  machine through its cleanup proceedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_multi_sim_bplmn_abort ( void* dummy, gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];
  grm_client_enum_t dest_client = gl1_convert_gas_id_to_bplmn_client(gas_id );

  if ( l1_multi_sim_data_ptr->background_hplm_state == L1_MULTI_SIM_BGRND_HPLMN_NULL )
  {
    MSG_HIGH("BPLMN abort: Background PLMN state NULL. Cnfm Id: %d",
          l1_tskisr_blk->current_params.L1_confirm_ID, 0, 0);
    /* In this state there is no BPLMN activity so ensure that the termination still occurs */
    l1_set_background_HPLMN_search_suspended(TRUE, gas_id);
    l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;

    /* Send correct confirmation to RRMPH_DECODE_BCCH_LIST_CNF to RR if specified */
    if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_DECODE_BCCH_LIST_CNF )
    {
       /* reset to a NULL_CNF so that we dont send the BCCH_LIST_CNF twice */
      l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
      l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
      l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
      l1_tskisr_blk->current_params.L1Data.pIdle_data =
      l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;
      l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_END;

      L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE, gas_id);
    }
    else if (l1_tskisr_blk->current_params.L1_confirm_ID == L1_POWER_SCAN_CNF )
    {
       /* reset to a NULL_CNF so that we dont send the POWER_SCAN_CNF twice */
      l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
      l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
      l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
      l1_tskisr_blk->current_params.L1Data.pIdle_data =
      l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;
      l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_END;
      L1_send_MPH_POWER_SCAN_CNF(FALSE, gas_id);
    }
  }
  else
  {
    MSG_HIGH("BPLMN abort: Background PLMN state %d", l1_multi_sim_data_ptr->background_hplm_state, 0, 0);
    grm_set_trm_status(GL1_TRM_ABORTING, dest_client, gas_id);
      }
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_BPLMN_SRCH_ABORT);
#endif
    }


/*
 *     Function name :  gl1_is_granted_geran_sub_doing_reselection
 *     --------------------------------
 *
 *     Description :
 *     ---------------
 *     This function checks that this client does not have trm lock and that the GERAN
 *     client that does hold trm lock, holds it for reason, TRM_RESELECT OR the various priorities of
 *     ACQUISTION. There is also a check that sleep inhibit has been set which will happen when
 *     x2g_resel_in_progress. x2g_resel_in_progress is checked in start gsm mode request.
 *     Returns :
 *     -----------
 *     TRUE -> x2g reselection/acquisition is in progress
 *     FALSE-> x2g reselection/acquisition not in progress
 */
boolean gl1_is_granted_geran_sub_doing_reselection(grm_client_enum_t active_client)
{
  if (gl1_msg_get_multi_sim_mode())
  {
    gas_id_t          gas_id, active_gas_id;
    grm_client_enum_t geran_trm_id;
    trm_reason_enum_t reason_held = TRM_NUM_REASONS;
    trm_get_info_return_type get_granted_info;

    /* Ensure only active clients grm_info data is updated */
    active_gas_id = gl1_map_client_id_to_gas_id( active_client );

    for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
    {
      geran_trm_id = gl1_map_gas_id_to_client_id( gas_id );

      /* consider any other client **except** the calling one */
      if ( active_client != geran_trm_id )
      {
        /*Alligning with DPM2.0, to check only if other client is granted lock*/
        if ( TRM_DENIAL != grm_get_granted( geran_trm_id, &get_granted_info, gas_id ))
        {
           /*
            * Can use grm_get_reason to allow for different client request
            * that updates the data store for the active gas_id only
            */
          reason_held = grm_get_reason( geran_trm_id, gas_id );

          if (IS_RESEL_OR_ACQ_REASON(reason_held))
          {
            return TRUE;
          }
        }/*end of if trm_denial*/
      }/*end of if*/
    } /* end of for loop */
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION  gl1_ms_do_background_HPLMN_search

DESCRIPTION
  This function is based on l1_do_background_HPLMN_search. It runs the appropriate
  handler depending on which component of the background HPLMN search is active
   i,e POWER_SCAN, BCCH_LIST or ACQ_LIST



DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_ms_do_background_HPLMN_search(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  boolean l_abort_find_bcch = FALSE;
  grm_client_enum_t dest_client = gl1_convert_gas_id_to_bplmn_client(gas_id);
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  static boolean check_bplmn_client_release[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

  switch( L1_get_current_background_HPLMN_search_activity(gas_id) )
  {
    case POWER_SCAN:
      {
        switch( l1_multi_sim_data_ptr->background_hplm_state )
        {
            case L1_MULTI_SIM_BGRND_HPLMN_NULL:
            {
                if ( !gpl1_get_grm_qta_state(gas_id) )
                {

                  l1_idle_data->g_bplmn_running_state = BPLMN_ACTIVE;

                  l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT;
                  gl1_setup_request_and_notify_for_bplmn(dest_client, BPLMN_MIN_DURATION_SCAN_MS, gas_id);
                }
                else
                {
                  /* Don't start BPLMN when in QTA gap */
                  MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_NULL for POWER_SCAN:wait for QTA ending ");
                }
              }
            break;
            case L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT:
            {
              uint32 frames_available = l1_fm_get_available_frames (FM_PRI_BACKGROUND_PLMN, GSTMR_GET_FN_GERAN(gas_id), gas_id);

              trm_status = grm_get_trm_status(dest_client, gas_id);

              if ( (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND) ||
                   (l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND) ||
                   (trm_status == GL1_TRM_NOT_GRANTED) || (trm_status == GL1_TRM_ABORTING)  )
              {
                 /* Cancel the trm request, and clear down */
                 grm_release_client_only(dest_client, gas_id);
                 gl1_cancel_bplmn_request(dest_client , gas_id);
                 MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT move to L1_MULTI_SIM_BGRND_HPLMN_NULL");
              }
              else if ((trm_status == GL1_TRM_GRANTED)||(trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
              {
                   /* Time is now ticking, decrement the frame count and keep watch for an abort */
                   /* as this may happen at any time during this state too */
                  if (grm_get_post_decrement_frame_duration(dest_client, gas_id) <= SUSPENSION_FRAME_DELAY)
                  {

                     MSG_GERAN_ERROR_0_G("BPLMN granted too few frames!!");
                     gl1_cancel_bplmn_request(dest_client, gas_id);
                     /* Release the TRM for BPLMN client */
                     call_in_task_grm_release(dest_client);
                  }
                  else
                  {
                     /* Need to load the RF and FW now, and only move to runnning once ready, this will be
                                    indicated by the flag l1_tskisr_blk->firmware_loaded */
                     gl1_multi_sim_fw_rf_load(dest_client, gas_id);
                     l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_FW_LOADING;
                  }

              }
              else if (((BPLMN_MIN_DURATION_SCAN_FRAMES) - 1) == frames_available)
              {
                /* Cancel and rebook this activity since it is unlikely to be granted now and */
                /* it will trigger TRM to restore any "unlock by" commands for our page activity */
                grm_release_client_only(dest_client,gas_id);
                l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
              }
              else
              {
                  MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_PSCAN_TRM_WAIT (trm_status=%d)", (int)(trm_status));
              }
            }
            break;

            case L1_MULTI_SIM_BGRND_HPLMN_FW_LOADING:
            {
               if ((grm_get_post_decrement_frame_duration(dest_client, gas_id) <= (SUSPENSION_FRAME_DELAY)) ||
                   (grm_get_trm_status(dest_client, gas_id) == GL1_TRM_ABORTING) ||
                   (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ) )
               {
                 /* nothing has resumed or started
                    simply release the lock and change the state */
                 gl1_cancel_bplmn_request(dest_client , gas_id);
                 /* Release the TRM for BPLMN client */
                 call_in_task_grm_release(dest_client);

                 MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_FW_LOADING move to L1_MULTI_SIM_BGRND_HPLMN_NULL");

               }
               else
               {
                 if( gl1_msg_get_multi_sim_mode() && ( l1_tskisr_blk->firmware_loaded == FALSE ) )
                 {
                   MSG_GERAN_MED_1_G("MSIM BPLMN FW loading at FN=%d", gl1_get_FN( gas_id ));
                 }
                 else
                 {

                   /* FW loaded
                                 */
                     L1_pscan_resume(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                     gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_BPLMN_SRCH_RESUME);
#endif
                     l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_RUNNING;
                     /* Schedule a check to ensure we have released the BPLMN client once we return to NULL */
                     check_bplmn_client_release[gas_id] = TRUE;
					 /* Reset the flag from rude-wakeup as it prevents PCH which is scheduled after BPLMN activity, to be decoded in first burst */
					 if(gl1_hw_get_rf_in_deep_sleep_whilst_forced_wakeup( gas_id ) )
					 {
					   gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup( gas_id, FALSE );
					 }

                     MSG_GERAN_MED_2_G("MSIM BPLMN will run now at FN=%d Dur=%d",
                                       gl1_get_FN( gas_id ),
                                       grm_get_frame_duration(dest_client, gas_id));
                 }
               }
            }
            break;

            case L1_MULTI_SIM_BGRND_HPLMN_RUNNING:
            {
              if ((grm_get_post_decrement_frame_duration(dest_client, gas_id) == SUSPENSION_FRAME_DELAY) || //2
                  (grm_get_trm_status(dest_client, gas_id) == GL1_TRM_ABORTING) ||
                  (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ) )
               {
                 l1_fm_set_suspend_trm_release((SUSPENSION_FRAME_DELAY + 1),
                                               FM_PRI_BACKGROUND_PLMN, gas_id);

                 /* Time to suspend BPLMN if we have run out of frames. */
                 L1_pscan_suspend(0, gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                 gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_BPLMN_SRCH_SUSPEND);
#endif

                 /* Correctly set the states here BEFORE running the L1_power_scan() This ensures that
                                 the power_scan_cnf isn't sent and the suspension state is always run too.
                              */
                 l1_multi_sim_data_ptr->suspension_timer = (SUSPENSION_FRAME_DELAY);
                 l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING;

                 MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_RUNNING move to L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING");
               }

               L1_power_scan(gas_id);

               if( l1_tskisr_blk->pscan_state == L1_SCAN_END )
               {
                 l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
                 grm_set_frame_duration(0, dest_client, gas_id);
                 call_in_task_grm_release(dest_client);
                 MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_RUNNING move to L1_MULTI_SIM_BGRND_HPLMN_NULL");
               }
            }
            break;

            case L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING:
            {
               if( --l1_multi_sim_data_ptr->suspension_timer <= 0 )
               {
                 /* set the new state here, as the L1_background_HPLMN_search_suspend_cb() function needs to
                                  be out of the RUNNING and SUSPENDED states to send the CNF message up to RR */
                 l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;

                 /* Suspension is now completed so send any relevant confirmation and terminate */
                 L1_background_HPLMN_search_suspend_cb(gas_id);
                 call_in_task_grm_release(dest_client);
                 MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING move to L1_MULTI_SIM_BGRND_HPLMN_NULL");
               }
               else
               {
                 MSG_GERAN_MED_1_G("background hplmn suspension timer running %d", l1_multi_sim_data_ptr->suspension_timer);
               }

               L1_power_scan(gas_id);

               if( l1_tskisr_blk->pscan_state == L1_SCAN_END )
               {
                 l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
                 grm_set_frame_duration(0, dest_client, gas_id);
                 call_in_task_grm_release(dest_client);
                 MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING move to L1_MULTI_SIM_BGRND_HPLMN_NULL");
               }
            }
            break;

          case L1_MULTI_SIM_BGRND_HPLMN_END:
            l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
            break;

          default:
            MSG_GERAN_HIGH_3_G("Incorrect MULTI_SIM_BGRND_HPLMN STATE %d at FN=%d on GAS ID 0x%02X"
              ,l1_multi_sim_data_ptr->background_hplm_state
              ,gl1_get_FN( gas_id )
              ,(int)gas_id);
            l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
            break;
        }
      }
      /* Don't sleep when background PLMN search running */
      if ( ( garb_intf_gas_is_xpg_coex_victim(gas_id) == TRUE ) && ( L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT == gpl1_get_multi_sim_bplmn_state(gas_id)) )
      {
        MSG_GERAN_HIGH_0(" BPLMN shall not prevent sleep when other RAT is in BG TRAFFIC "); 
      }
      else
      {
      gl1_drx_require_next_tick(gas_id);
      }
      break;

      case ACQ_BCCH_LIST:
      {

        switch( l1_multi_sim_data_ptr->background_hplm_state )
        {
          case L1_MULTI_SIM_BGRND_HPLMN_NULL:
            {
                if ( !gpl1_get_grm_qta_state(gas_id) )
                {

                  l1_idle_data->g_bplmn_running_state = BPLMN_ACTIVE;
                   /* set a minumum frames required and we don't know when activity is going to happen on other sim
                      This is needed for the DRDSDS case
                    */
                  l1_fm_set_required_frame_number(FM_PRI_BACKGROUND_PLMN, gl1_get_FN(gas_id), (uint32)BPLMN_MIN_DURATION_SCAN_FRAMES, gas_id);

                  if (l1_fm_are_bplmn_frames_available(BPLMN_MIN_DURATION_SCAN_FRAMES,
                                                       l1_multi_sim_bplmn_abort, 0, gas_id))
                  {

                    if (grm_get_trm_status(dest_client, gas_id) != GL1_TRM_GRANTED)
                    {
                      /* setup all the data required to make the request to trm for the lock */
                      l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT;
                      gl1_setup_request_and_notify_for_bplmn(dest_client, BPLMN_MIN_DURATION_ACQ_MS, gas_id);

                    }
                    else
                    {
                      /*BPLMN Can run now, trm granted but FW is not loaded.*/
                      MSG_GERAN_HIGH_1_G("wait for fw load/trm grant %d ", grm_get_post_decrement_frame_duration(dest_client, gas_id));

                      l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT;
                    }
                  }
                }
                else
                {
                  /* Don't start BPLMN when in QTA gap */
                  MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_NULL for ACQ_BCCH_LIST:wait for QTA ending ");
                }
              }
            break;
          case L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT:
            {
              uint32 frames_available = l1_fm_get_available_frames (FM_PRI_BACKGROUND_PLMN, GSTMR_GET_FN_GERAN(gas_id), gas_id);

              trm_status = grm_get_trm_status(dest_client, gas_id);

              if ((l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND) ||
                  (l1_tskisr_blk->main_command == L1_ENTER_NULL_COMMAND) ||
                  (trm_status == GL1_TRM_NOT_GRANTED) || (trm_status == GL1_TRM_ABORTING))
              {
                grm_release_client_only(dest_client, gas_id);
                gl1_cancel_bplmn_request(dest_client, gas_id);

                MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT move to L1_MULTI_SIM_BGRND_HPLMN_NULL");

              } else if ((trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS))
              {
                /* Time is now ticking by decrement the frame count and keep watch for an abort */
                /* as this may happen at any time during this state too */
                if (grm_get_post_decrement_frame_duration(dest_client, gas_id) <= SUSPENSION_FRAME_DELAY)
                {
                  gl1_cancel_bplmn_request(dest_client, gas_id);
                  /* Release the TRM for BPLMN client */
                  call_in_task_grm_release(dest_client);

                  MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT move to L1_MULTI_SIM_BGRND_HPLMN_NULL");
                } else
                {
                  /* Need to load the RF and FW now, and only move to runnning once ready, this will be
                  indicated by the flag l1_tskisr_blk->firmware_loaded */
                  gl1_multi_sim_fw_rf_load(dest_client, gas_id);

                  l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_FW_LOADING;
                }

              }
              else if (((BPLMN_MIN_DURATION_ACQ_FRAMES) - 1) == frames_available)
              {
                /* Cancel and rebook this activity since it is unlikely to be granted now and */
                /* it will trigger TRM to restore any "unlock by" commands for our page activity */
                grm_release_client_only(dest_client,gas_id);
                l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
             }
              else
              {
                MSG_GERAN_HIGH_1_G("L1_MULTI_SIM_ACQ_TRM_WAIT (trm_status=%d)", (int)(trm_status));
              }

              break;
            }

          case L1_MULTI_SIM_BGRND_HPLMN_FW_LOADING:
            {
              if ((grm_get_post_decrement_frame_duration(dest_client, gas_id) <= (SUSPENSION_FRAME_DELAY)) ||
                  (grm_get_trm_status(dest_client, gas_id) == GL1_TRM_ABORTING) ||
                  (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND))
              {
                gl1_cancel_bplmn_request(dest_client, gas_id);
                /* Release the TRM for BPLMN client */
                call_in_task_grm_release(dest_client);

                MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_FW_LOADING move to L1_MULTI_SIM_BGRND_HPLMN_NULL");
              }
              else
              {
                if (gl1_msg_get_multi_sim_mode() && (l1_tskisr_blk->firmware_loaded == FALSE))
                {
                  MSG_GERAN_MED_1_G("MS BGPLMN FW loading at FN=%d", gl1_get_FN(gas_id));
                } else
                {

#ifdef FEATURE_GSM_QSC_TX_DIV
                  /*Set AsDiv Mode to Idle Wake Now*/
                  gl1_hw_set_asdiv_mode_config(gas_id, TRM_CLIENT_MODE_IDLE_WAKEUP);
#endif /* FEATURE_GSM_QSC_TX_DIV */

                  /* We have suspended PLMN search before. Since frames are available now,
                   * resume the search.
                   */
                   grm_retain_lock(dest_client,
                    (grm_unlock_callback_t) &grm_abort_callback,
                     gas_id);

                  l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_RUNNING;
                  L1_bcch_list_acq_bcch_resume(gas_id);
 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                  gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_BPLMN_SRCH_RESUME);
#endif

                   /* Schedule a check to ensure we have released the BPLMN client once we return to NULL */
                   check_bplmn_client_release[gas_id] = TRUE;
				   /* Reset the flag from rude-wakeup as it prevents PCH which is scheduled after BPLMN activity, to be decoded in first burst*/
				   if(gl1_hw_get_rf_in_deep_sleep_whilst_forced_wakeup( gas_id ) )
				   {
				     gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup( gas_id, FALSE );
				   }
                  MSG_GERAN_MED_2_G("MSIM BPLMN will run now at FN=%d Dur=%d",
                                    gl1_get_FN(gas_id),
                                    grm_get_frame_duration(dest_client, gas_id));
                }
              }
              break;
            }

          case L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING:
            {
              if ( l1_multi_sim_data_ptr->suspension_timer > 0 )
              {
                l1_multi_sim_data_ptr->suspension_timer--;
              }
              else if (gpl1_multi_sim_bplmn_decode_bcch_inactive(gas_id))
              {
                /* set the new state here, as the L1_background_HPLMN_search_suspend_cb() function needs to
                be out of the RUNNING and SUSPENDED states to send the CNF message up to RR */
                l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_END;
                /* Suspension is now completed so send any relevant confirmation and terminate */
                L1_background_HPLMN_search_suspend_cb(gas_id);
                call_in_task_grm_release(dest_client);


                MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING move to L1_MULTI_SIM_BGRND_HPLMN_NULL");
                break;
              }
              else
              {
                MSG_GERAN_MED_0_G("background hplmn suspension timer running");
              }

              if (L1_get_bcch_list_fcch_sch_bcch(gas_id))
              {
                l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_END;
              }
              break;
            }

          case L1_MULTI_SIM_BGRND_HPLMN_RUNNING:

            if ((grm_get_post_decrement_frame_duration(dest_client, gas_id) == SUSPENSION_FRAME_DELAY) ||
                  (grm_get_trm_status(dest_client, gas_id) == GL1_TRM_ABORTING) ||
                (l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND ) )
            {
              /* This prevents the FM from releasing before end of bplmn by allowing an extra frame*/
              l1_fm_set_suspend_trm_release( (SUSPENSION_FRAME_DELAY+1),
                                             FM_PRI_BACKGROUND_PLMN, gas_id );

              /* Time to suspend BPLMN if we have run out of frames. */
              L1_bcch_list_acq_bcch_suspend(0, gas_id);
              l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING;
              (void)L1_get_bcch_list_fcch_sch_bcch(gas_id);
              l1_multi_sim_data_ptr->suspension_timer = SUSPENSION_FRAME_DELAY;
              MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_RUNNING move to L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING");
            }
            else
            {
              /* bplmn aborted locally in RUNNING state, ask for frame now and resume */
              if ( !L1_background_HPLMN_search_running(gas_id) )
              {
                uint32 frame_duration = grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id);;

                MSG_GERAN_HIGH_1_G("BPLMN got suspended, frame_duration %d", frame_duration);

                if ( frame_duration >= MIN_BCCH_DECODE_FRAMES )
                {
                  l1_fm_set_required_frame_number(FM_PRI_BACKGROUND_PLMN, gl1_get_FN( gas_id ), frame_duration, gas_id);

                  if( l1_fm_are_frames_available(FM_PRI_BACKGROUND_PLMN,frame_duration,
                                                  l1_multi_sim_bplmn_abort, 0, gas_id) )
                  {
                    L1_bcch_list_acq_bcch_resume(gas_id);
                    l1_set_background_HPLMN_search_suspended(FALSE, gas_id);
                  }
                  else
                  {
                    /* frames not available, move to NULL state */
                    l_abort_find_bcch = TRUE;
                  }
                }
                else
                {
                  /* not enough frames left, move to NULL state */
                  l_abort_find_bcch = TRUE;
                }
              }
              else
              {
                l_abort_find_bcch = L1_get_bcch_list_fcch_sch_bcch(gas_id);
              }

              if( (l_abort_find_bcch) || ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_NULL ) )
              {
                l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_END;
                l1_fm_set_suspend_trm_release(1,
                                              FM_PRI_BACKGROUND_PLMN, gas_id);
                call_in_task_grm_release(dest_client);
                MSG_GERAN_HIGH_0_G("L1_MULTI_SIM_BGRND_HPLMN_RUNNING move to L1_MULTI_SIM_BGRND_HPLMN_END");
              }
            }
            break;

          case L1_MULTI_SIM_BGRND_HPLMN_END:
            l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
            break;

          default:
            MSG_GERAN_ERROR_2_G("Incorrect MULTI_SIM_BGRND_HPLMN STATE %d FN=%d"
              ,l1_multi_sim_data_ptr->background_hplm_state
              ,gl1_get_FN( gas_id )
              );
            l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
            break;
        }
      }
      /* Don't sleep when background PLMN search running */
      if ( ( garb_intf_gas_is_xpg_coex_victim(gas_id) == TRUE ) && ( L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT == gpl1_get_multi_sim_bplmn_state(gas_id)) )
      {
        MSG_GERAN_HIGH_0("  BPLMN shall not prevent sleep when other RAT is in BG TRAFFIC "); 
      }
      else
      {
      gl1_drx_require_next_tick(gas_id);
      }
      break;

    case NO_HPLMN_SEARCH:

      if ( l1_multi_sim_data_ptr->background_hplm_state != L1_MULTI_SIM_BGRND_HPLMN_NULL )
      {
        if ( l1_multi_sim_data_ptr->background_hplm_state == L1_MULTI_SIM_BGRND_HPLMN_END)
        {
          if (grm_get_trm_status(dest_client, gas_id) != GL1_TRM_GRANTED)
          {
            l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
            l1_idle_data->g_bplmn_running_state = BPLMN_INACTIVE;
            MSG_GERAN_HIGH_0_G("BPLMN INACTIVE");
            break;
          }
        }

        MSG_GERAN_ERROR_2_G("NO_HPLMN_SEARCH - Incorrect MULTI_SIM_BGRND_HPLMN STATE %d FN=%d"
          ,l1_multi_sim_data_ptr->background_hplm_state
          ,gl1_get_FN( gas_id )
          );

        l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
      }
      else if (check_bplmn_client_release[gas_id] == TRUE)
      {
        trm_grant_return_enum_type bplmn_granted;
        trm_get_info_return_type bplmn_info;
        l1_bplmn_running_state_T running_state;

        running_state = l1_idle_data->g_bplmn_running_state;
        l1_idle_data->g_bplmn_running_state = BPLMN_ACTIVE;
        bplmn_granted = grm_get_granted (dest_client, &bplmn_info, gas_id);
        l1_idle_data->g_bplmn_running_state = running_state;

        if ( bplmn_granted != TRM_DENIAL )
        {
          if (!l1_tskisr_blk->firmware_loaded)
          {
            grm_release_client_only (dest_client, gas_id);
            l1_multi_sim_data_ptr->background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
            l1_idle_data->g_bplmn_running_state = BPLMN_INACTIVE;
            MSG_GERAN_HIGH_0_G("BPLMN INACTIVE");

          }
          else
          {
            ERR_GERAN_FATAL_1_G ("BPLMN client %d held while inactive with firmware loaded",dest_client);
          }
        }
        else if ((l1_multi_sim_data_ptr->background_hplm_state == L1_MULTI_SIM_BGRND_HPLMN_NULL) &&
                 (l1_idle_data->g_bplmn_running_state == BPLMN_ACTIVE))
        {
          l1_idle_data->g_bplmn_running_state = BPLMN_INACTIVE;
          MSG_GERAN_HIGH_0_G("BPLMN INACTIVE");
        }
        check_bplmn_client_release[gas_id] = FALSE;
      }
      break;

    default:
      break;
  }

#ifdef GL1_BPLMN_DEBUG
  {
    gl1_trm_state_t   trm_state;
    l1_bplmn_running_state_T running_state = l1_idle_data->g_bplmn_running_state;
    gl1_bplmn_debug_t* gl1_bplmn_debug_p = &(gl1_bplmn_debug[gas_id][gl1_bplmn_debug_index[gas_id]]);

    /* get the state of GPRS client even if we are somehow inactive */
    l1_idle_data->g_bplmn_running_state = BPLMN_ACTIVE;
    trm_state = grm_get_trm_status (dest_client,gas_id);
    l1_idle_data->g_bplmn_running_state = running_state;

    /* create a log entry any time the BPLMN states change */
    if ( ( gl1_bplmn_debug_p->background_HPLMN_search_activity != l1_idle_data->background_HPLMN_search_activity ) ||
         ( gl1_bplmn_debug_p->g_bplmn_running_state            != running_state ) ||
         ( gl1_bplmn_debug_p->background_hplm_state            != l1_multi_sim_data_ptr->background_hplm_state ) ||
         ( gl1_bplmn_debug_p->trm_state                        != trm_state ) )
    {
      if (++gl1_bplmn_debug_index[gas_id] == GL1_BPLMN_DEBUG_MAX)
      {
        gl1_bplmn_debug_index[gas_id] = 0;
        gl1_bplmn_debug_p = &(gl1_bplmn_debug[gas_id][gl1_bplmn_debug_index[gas_id]]);
      }
      gl1_bplmn_debug_p = &(gl1_bplmn_debug[gas_id][gl1_bplmn_debug_index[gas_id]]);
      /* record BPLMN FN state */
      gl1_bplmn_debug_p->frame_number                     = GSTMR_GET_FN_GERAN(gas_id);
      gl1_bplmn_debug_p->background_HPLMN_search_activity = l1_idle_data->background_HPLMN_search_activity;
      gl1_bplmn_debug_p->g_bplmn_running_state            = running_state;
      gl1_bplmn_debug_p->background_hplm_state            = l1_multi_sim_data_ptr->background_hplm_state;
      gl1_bplmn_debug_p->trm_state                        = trm_state;
      gl1_bplmn_debug_p->bplmn_decode_bcch_inactive       = gpl1_multi_sim_bplmn_decode_bcch_inactive(gas_id);
    }
}
#endif

}

/*===========================================================================

FUNCTION  l1_ds_set_bplmn_state_to_null

DESCRIPTION
          Initialise background_hplm_state to NULL. Safe guards being stuck in non NULL state

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_ds_set_bplmn_state_to_null(gas_id_t gas_id)
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  l1_multi_sim_data_ptr->background_hplm_state=L1_MULTI_SIM_BGRND_HPLMN_NULL;
}

/*===========================================================================

FUNCTION  l1_ds_get_bplmn_state

DESCRIPTION
          Retrieve background_hplm_state.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
background_hplm_state_t l1_ds_get_bplmn_state(gas_id_t gas_id)
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  return (l1_multi_sim_data_ptr->background_hplm_state);
}


/*===========================================================================

FUNCTION  gpl1_multi_sim_panic_reset

DESCRIPTION
          Performs the Dual sim specific panic reset

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_multi_sim_panic_reset( l1_panic_reset_type_T   error_code,
                                 rr_l1_message_id_enum_T reset_response,
                                 gas_id_t                gas_id )
{
  grm_client_enum_t          client;

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  idle_data_T      *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

  client                  = l1_tskisr_blk->client_id;

  if( l1_check_for_panic_reset(gas_id) == FALSE )
  {
    return;
  }

  /* Ensure that GSM wakes up - if not already awake */
  gl1_drx_require_next_tick(gas_id);

  if (error_code == L1_STALLED_ISR)
  {
    gstmr_force_reinit(gas_id);
  }
  else if ( !L1_IN_IRQ_MODE() )
  {
    (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
    (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
  }

  idle_data_ptr->do_pwr_meas_FN = L1_GPRS_INVALID_FN;

  panic_reset_occured[gas_id] = TRUE;
  if (error_code != L1_STALLED_ISR)
  {
    /* Prevent and re-init sleep */
    gl1_drx_off(FALSE, gas_id);
  }

  /* perform the reset now */
  l1_do_panic_reset(error_code, reset_response, gas_id);

  /* move Panic triggered SIM to inactive rather than the RESET state. This is because for
   * DUAL sim the FW is constantly loaded and unloaded so theres no benifits to the
   * reset state anyway.
   */
  GL1_ISR_LOCK(gas_id);

  l1_tskisr_blk->l1_state = L1_INACTIVE_MODE;

  l1_init_multi_sim_data(gas_id);

  GL1_ISR_UNLOCK(gas_id);

  MSG_GERAN_HIGH_1_G("PANIC RESET client:%d", client);
}

/*===========================================================================

FUNCTION  gl1_detect_page_collision_with_other_sub

DESCRIPTION Detects collision with other sub paging block.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/

boolean gl1_detect_page_collision_with_other_sub( gas_id_t gas_id )
{
 volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  switch (l1_tskisr_blk->client_id)
  {
    case GRM_GSM1:
    {
      if(gl1_page_collision_with_x_sub(GRM_GSM2, FM_PRI_NCELL_ACQ,gas_id))
      {
        return TRUE;
      }

#if defined ( FEATURE_TRIPLE_SIM )
      if(gl1_page_collision_with_x_sub(GRM_GSM3, FM_PRI_NCELL_ACQ,gas_id))
      {
        return TRUE;
      }
#endif /* FEATURE_TRIPLE_SIM */
    }
    break;
    case GRM_GSM2:
    {
      if(gl1_page_collision_with_x_sub(GRM_GSM1, FM_PRI_NCELL_ACQ,gas_id))
      {
        return TRUE;
      }
#if defined ( FEATURE_TRIPLE_SIM )
      if(gl1_page_collision_with_x_sub(GRM_GSM3, FM_PRI_NCELL_ACQ,gas_id))
      {
        return TRUE;
      }
#endif /* FEATURE_TRIPLE_SIM */
    }
    break;

#if defined ( FEATURE_TRIPLE_SIM )
    case GRM_GSM3:
    {
      if(gl1_page_collision_with_x_sub(GRM_GSM1, FM_PRI_NCELL_ACQ,gas_id))
      {
        return TRUE;
      }
      if(gl1_page_collision_with_x_sub(GRM_GSM2, FM_PRI_NCELL_ACQ,gas_id))
      {
        return TRUE;
      }
    }
    break;
#endif /* FEATURE_TRIPLE_SIM */

    default:
    break;
  }

  return FALSE;
}

#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION  gpl1_gbta_reset_active_state

DESCRIPTION
    Clears GBTA state if left set and Idle state machines called for TRM release/reserve
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
void gpl1_gbta_reset_active_state(gas_id_t gas_id)
{
  gas_id_t ptm_gas_id = gl1_get_gas_id_for_ptm_client();
  if (IS_GBTA_ACTIVE())
  {
    if (ptm_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED)
    {
      SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
      /* start PTM SCE */
      OTHER_SUB_START_SC_TICK_TRANS(ptm_gas_id);
    }
    else
    {
      MSG_GERAN_HIGH_0_G("OTHER_GSM_CLIENT exited Pkt Transfer in GBTA Gap");
      SET_GBTA_STATUS(GRM_GBTA_NONE);
    }
  }
}

/*===========================================================================

FUNCTION  grm_notify_event

DESCRIPTION
     Updates GRB GBTA Events state.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
void grm_notify_event(grm_gbta_idle_event_t event,boolean leaving_idle,gas_id_t gas_id )
{
  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  grm_gbta_info[gas_id].idle_event = event;
  grm_gbta_info[gas_id].leaving_idle_mode  = leaving_idle;
  grm_gbta_info[gas_id].idle_event_status  = GBTA_STATUS_NONE;
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();

  MSG_GERAN_MED_3_G( "GBTA notify_event event %d leaving_idle %d gas_id %d",event,leaving_idle,gas_id);
} /* grm_notify_event */


/*===========================================================================

FUNCTION  gl1_gbta_schedule_idle_per_mon_if_allowed

DESCRIPTION
    Performs check to see of Idle Pwr Mon can be scheduled.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
boolean gl1_gbta_schedule_idle_per_mon_if_allowed(gas_id_t gas_id)
{
  boolean mon_allowed = FALSE;

   /* Differ Idle Power Moniter */
  if (  (! IS_GBTA_INACTIVE()) &&  // GBTA possible.
        (! IS_IDLE_EV_PWR_MON_ACTIVE(gas_id)) && /* Idle sub has not scheduled event EVENT_IDLE_POW_MON */
        ( l1_sc_check_idle_mon_allowed(gas_id)) && /* check of ncell present */
        (!gl1_drdsds_enabled(gas_id)) /*Check if DR is enabled*/
     )
  {
    mon_allowed = TRUE;
  }

#ifdef FEATURE_GPRS_GBTA_DEBUG
  MSG_GERAN_MED_3_G("GBTA - mon_allowed %d gbta state %d idle_ev_allowed %d",
                    mon_allowed, GET_GBTA_STATE(),! IS_IDLE_EV_PWR_MON_ACTIVE(gas_id));
#endif /*  FEATURE_GPRS_GBTA_DEBUG */

  return mon_allowed;
} /* gl1_gbta_schedule_idle_per_mon_if_allowed */


/*===========================================================================

FUNCTION  gl1_gbta_scheduling_idle_event

DESCRIPTION
    Schedules idle power monitor event in Transfer for other sub.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
void gl1_gbta_scheduling_idle_event(grm_gbta_idle_event_t idle_event,gas_id_t gas_id)
{
  MSG_GERAN_HIGH_2_G("GBTA - Scheduling idle event %d gas_id %d",idle_event,gas_id);

  /* schdules power monitor */
  l1_sci_paging_block_tick(gas_id);
}

/*===========================================================================

FUNCTION  gl1_gbta_get_transfer_sub

DESCRIPTION
    gets transfer sub gas_id.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
gas_id_t gl1_gbta_get_transfer_sub(gas_id_t gas_id)
{
  gas_id_t other_gas_id = GERAN_ACCESS_STRATUM_ID_1;

  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    other_gas_id = GERAN_ACCESS_STRATUM_ID_2;
  }

  return other_gas_id;
} /* gl1_gbta_get_transfer_sub */

/*===========================================================================

FUNCTION  gl1_is_gbta_reason

DESCRIPTION
    Validates reaons for alllowing GBTA.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
boolean gl1_is_gbta_reason(trm_reason_enum_t reason,gas_id_t gas_id)
{
  boolean gbta_reason = FALSE;

  gbta_reason = IS_GBTA_REASON(reason, l1_fm_get_reserve_sub_reason(gas_id)) ;

  return gbta_reason;
} /* gl1_is_gbta_reason */

/*===========================================================================

FUNCTION  gpl1_check_no_transfer_sub_conflict

DESCRIPTION
    Does conflict check with Transfer Sub.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
boolean  gpl1_check_no_transfer_sub_conflict(uint32 frames_req,gas_id_t gas_id)
{
 if  (! (OTHER_SUB_RELEASE_PENDING() ||
         OTHER_SUB_RECONFIG_PROGRESS(frames_req) ||
         OTHER_SUB_SC_OP_IN_PROGRESS()   || // No PTM-Idle frame activity in progress
         OTHER_SUB_WCDMA_ID_SEARCHING_IN_TBF() ||
         OTHER_SUB_NCELL_MEAS_IN_PROGRESS() ||
         OTHER_SUB_IN_TBF_SUSP()  ||
         OTHER_SUB_SUSP_PLANNED() ||
         OTHER_SUB_IRAT_IN_IDLE_FRAME()
        )
     )
 {
   return TRUE;
 }

 return FALSE;
} /* gpl1_check_no_transfer_sub_conflict */

/*===========================================================================

FUNCTION  gl1_gbta_check_gbta_allowed

DESCRIPTION
    Verified is GBTA mode can be granted.
    Does conflict check with Transfer Sub.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
 NOTE: This function is not only validating conditions to enable GBTA but also
  enables GBTA if condition matches. It implies that don't use && or any logical
  operator after calling this function. This function call should be the last
  statement of the condition expression.

===========================================================================*/
boolean gl1_gbta_check_gbta_allowed(trm_reason_enum_t req_reason,uint32 frames_required,gas_id_t gas_id)
{
  boolean gbta_allowed = FALSE;
  gas_id_t ptm_gas_id = gl1_get_gas_id_for_ptm_client();
  boolean  no_conflict_with_transfer_sub = TRUE;
  grm_gbta_state_t            gbta_status = GRM_GBTA_NONE;

  if(ptm_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED)
  {
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[ptm_gas_id];
    transfer_data_T    *transfer_data_ptr =
                  (transfer_data_T *)l1_tskisr_blk->current_params.L1Data.pTransfer_data;

    /* GBTA Basic Check */
    /* Check if same client had done booking */
    /* check if no clashes - allowed to go   --->
     ** RRBP - > allow all RRBP , need to give low Prior to normal PDAN
    ** UL PTCCH  -allow with GBTA , prio is UL PTCCH when clash
    ** Dl PTCCH  - usually cancelled but allow with GBTA, low prio DL PTCCH
    ** Release  Pending - dont allow GBTA
    ** TBF reconfig - dont allow GBTA - can do frame check to be more precise  */

    if( IS_GBTA_ALLOWED () &&   // when TBF suspended GBTA is blocked
        OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)
     )
    {
      gbta_allowed = TRUE;
    }
    else
    {
      MSG_GERAN_HIGH_3_G("GBTA transfer active %d state %d rlc mode %d", OTHER_GSM_CLIENT_IN_TRANSFER(gas_id), GET_GBTA_STATE(), transfer_data_ptr->rlc_mode);
    }



  /* To avoid race condition between two subs on activating GBTA and reading
     gbta_not_allowed flag, set GBTA status as Active. If conditions
     doesn't meet, revert back to original state.*/
    gbta_status = GET_GBTA_STATE();

    SET_GBTA_STATUS(GRM_GBTA_ACTIVE);

    no_conflict_with_transfer_sub = gpl1_check_no_transfer_sub_conflict(frames_required, gas_id);
    if ((no_conflict_with_transfer_sub) && /* check clashes with transfer sub activity in progress */
        (gbta_allowed == TRUE) &&

       /*GBTA allowed in 2 phase access with this feature */
#ifndef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
        (!OTHER_GSM_CLIENT_IN_SINGLE_BLOCK_ALLOC(ptm_gas_id)) &&
#else
        (!(OTHER_GSM_CLIENT_IN_SINGLE_BLOCK_ALLOC(ptm_gas_id) && !IS_TWO_PHASE_ACCESS(ptm_gas_id))) &&
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

        (gl1_is_gbta_reason(req_reason, gas_id))    // gbta allowed for req_reason.
       )
    {
      gbta_allowed = TRUE;
    }
    else
    {
      SET_GBTA_STATUS(gbta_status);
      gbta_allowed = FALSE;
      MSG_GERAN_HIGH_3_G("GBTA is_gbta_reason %d check_no_transfer_sub_conflict %d other_client_in_xfer %d",
                               gl1_is_gbta_reason(req_reason, gas_id),
                               no_conflict_with_transfer_sub,
                               OTHER_GSM_CLIENT_IN_TRANSFER(ptm_gas_id));
    }
  }
  else
  {
    MSG_GERAN_HIGH_2_G("GBTA is_gbta_reason %d check_no_transfer_sub_conflict %d No client in Xfer mode",
                       gl1_is_gbta_reason(req_reason, gas_id),
                       no_conflict_with_transfer_sub);
  }

  MSG_GERAN_HIGH_1_G("GBTA gbta_allowed %d ", gbta_allowed );

  return gbta_allowed;
} /* gl1_gbta_check_gbta_allowed */

/*===========================================================================

FUNCTION  gl1_set_idle_event_status

DESCRIPTION
    Sets GBTA Event status when pwr mon activated/ finished.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
void gl1_set_idle_event_status(grm_gbta_idle_event_status_t ev_stat, gas_id_t gas_id)
{
   GL1_MULTI_SIM_MODE_GBTA_LOCK();
   grm_gbta_info[gas_id].idle_event_status = ev_stat;
   GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
}

#endif /* FEATURE_GPRS_GBTA */

/*===========================================================================

FUNCTION  gpl1_multi_sim_setup_afc_tt

DESCRIPTION
  Sets up the timing and frequency adjustments for the currently active GAS ID

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  May update the bitmap pattern to be used for priority inversion

===========================================================================*/
void gpl1_multi_sim_setup_afc_tt( gas_id_t  gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* now force the AFC and TT for the SUB about to come online next */
  /* Ensure that the freq correction is applied for this GSM client */
  gl1_msg_acq_tcxo_and_rotator_init(gas_id);

  /* set trigger for calculation of the correct cycle swallower programming value, picked up
   * at start of next frame */
  gl1_hw_set_active_cycle_swallower_adj( gl1_get_serving_cell_arfcn(gas_id), gas_id );

  MSG_GERAN_HIGH_1_G("Setup AFC for GSM client:%d",l1_tskisr_blk->client_id);
}


/*===========================================================================

FUNCTION  gpl1_multi_sim_calculate_grm_reservation_time

DESCRIPTION
  This function determines the required TRM request period in sclcks,
  factoring in the time during the frame at which this is called.

DEPENDENCIES
  None

RETURN VALUE
  TRM period request in sclcks.

SIDE EFFECTS
===========================================================================*/
uint32 gpl1_multi_sim_calculate_grm_reservation_time( uint32   requested_FN,
                                                      gas_id_t gas_id )
{
  uint32 gframe_duration, sclk_duration, qsym_cnt, current_GFN;

  /* Avoid any possible FN updates */
  GL1_ISR_SAVE_LOCK(gas_id);

  if(gl1_hw_sleep_wmgr_cancel_in_prog[gas_id])
  {
    current_GFN = gl1_wmgr_calculate_grm_reservation_time(gas_id);
  }
  else
  {
    current_GFN = gl1_get_FN( gas_id );
  }

  /* How many whole frames */
  gframe_duration = SUB_FN( requested_FN, current_GFN );

  /* Add offset as if running in asynch ISR then FN has been bumped */
  gframe_duration += gl1_drx_get_pch_offset( gas_id );

  if(!gl1_hw_sleep_wmgr_cancel_in_prog[gas_id])
  {
    /* Snapshot of where we are in gstmr for non frame synchronous activity */
    qsym_cnt      = gstmr_rd_qsymbol_count_geran( geran_map_gas_id_to_nas_id( gas_id ) );

    /* Extra frame update pending */
    if( gstmr_is_int_pending(gas_id) )
    {
      qsym_cnt += QS_PER_FRAME;
    }
  }

  GL1_ISR_SAVE_UNLOCK(gas_id);

  /* Number of whole frames in scaled sclcks */
  sclk_duration   =
    gpl1_multi_sim_convert_frames_to_sclcks( gframe_duration );

  if(!gl1_hw_sleep_wmgr_cancel_in_prog[gas_id])
  {
    /* Now work out how many sclks we are into the frame when actioning this */
	 qsym_cnt = ( ( qsym_cnt * QSYM_SCLK_SCALING_NUM ) / QSYM_SCLK_SCALING_DENOM );

    /* Subtract from whole frame period how many sclks into the frame this occurs */
    sclk_duration  -= timetick_cvt_to_sclk( qsym_cnt, T_USEC );
    MSG_GERAN_MED_3_G( "MS Calc GRM Res Time Gframes:%d qsym_cnt:%d sclk_duration:%d",
           gframe_duration, qsym_cnt, sclk_duration );
  }

  return ( sclk_duration );
}

/*===========================================================================

FUNCTION  gl1_wmgr_calculate_grm_reservation_time

DESCRIPTION
  This function determines the required TRM request period in sclks, during WMGR cancelation,
  factoring in the time during the frame at which this is called.

DEPENDENCIES
  None

RETURN VALUE
  TRM period request in sclcks.

SIDE EFFECTS
===========================================================================*/
uint32 gl1_wmgr_calculate_grm_reservation_time(gas_id_t gas_id)
{
  int32  gl1_wmgr_time_to_canceled_act_sclks;
  int32  gl1_wmgr_frames_remaining_to_sleep;
  uint32  gl1_wmgr_current_GFN;
  slpc_id_type  slpc_from_gas;

  slpc_from_gas = gl1_hw_slpc_from_gas_id( gas_id );

  /* request the number of GSM clocks we have slept for */
  /* time to next vstmr = (time we planned to sleep for)-(time slept so far) */
  gl1_wmgr_time_to_canceled_act_sclks = (int32) (sleep_duration_gsm_clks[gas_id] - (uint32) slpc_get_sysclk_count(slpc_from_gas));
  gl1_wmgr_frames_remaining_to_sleep = gl1_wmgr_time_to_canceled_act_sclks / NUM_GSM_CLKS_PER_FRAME;

  /* note that gl1_wmgr_current_GFN can be more than gl1_hw_wkupmgr_wakeup_point_fn[] if we have slept for longer than programmed sleep time */
  gl1_wmgr_current_GFN = SUB_FN(gl1_hw_wkupmgr_wakeup_point_fn[gas_id], gl1_wmgr_frames_remaining_to_sleep);

  MSG_GERAN_MED_2_G( "MS Calc GRM gl1_wmgr_current_GFN frame: %d, wkup_FN: %d", gl1_wmgr_current_GFN, gl1_hw_wkupmgr_wakeup_point_fn[gas_id]);

  return(gl1_wmgr_current_GFN);

}
/*===========================================================================

FUNCTION  gpl1_multi_sim_convert_frames_to_sclcks

DESCRIPTION
  This function determines the requested GSM frames period in sclcks.

DEPENDENCIES
  None

RETURN VALUE
  Requested GSM frames period request in sclcks.

SIDE EFFECTS
===========================================================================*/
uint32 gpl1_multi_sim_convert_frames_to_sclcks( uint32 Gframes )
{
  /* Number of whole frames in scaled sclcks */
  uint32 sclk_duration   =
    timetick_cvt_to_sclk( ( Gframes * FRAME_SCALING_FACTOR ), T_MSEC );

  /* Finally scale back to correct period */
  sclk_duration        >>= FRAME_SCALING_SHIFT;

  return ( sclk_duration );
}

/*===========================================================================

FUNCTION  gl1_ms_trm_get_resource

DESCRIPTION
   This function returns the resource to be used with TRM based on the
   Multi-Sim Mode in operation and the gas_id

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
trm_resource_enum_t gl1_ms_get_trm_resource( gas_id_t gas_id , trm_reason_enum_t reason)
{
#if defined(FEATURE_SGLTE_DUAL_SIM ) || defined(FEATURE_SVLTE_DUAL_SIM ) || defined(FEATURE_GSM_RX_TX_SPLIT)|| defined(FEATURE_DUAL_DATA)

  switch(reason)
  {
     case TRM_TRAFFIC:
     case TRM_BG_TRAFFIC:
     case TRM_ACCESS:
       return TRM_RX_BEST;
  case TRM_DEMOD_PAGE:
    return TRM_RX_BEST_POSSIBLE_MOD;
    /*SR reason*/
     case TRM_ACQUISITION:
     case TRM_ACQUISITION_INV:
       return TRM_RX_BEST;
    default:
    return TRM_RX_BEST_POSSIBLE_MOD_2;
  }
#else
  if ( ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE )&&
       ( ( gas_id == GERAN_ACCESS_STRATUM_ID_2 )
#if defined ( FEATURE_TRIPLE_SIM )
         || ( gas_id == GERAN_ACCESS_STRATUM_ID_3 )
#endif /* FEATURE_TRIPLE_SIM */
       ) )
  {
    return TRM_RXTX_BEST_SMODE;
  }
  else
  {
    return TRM_RX_BEST;
  }
#endif /*FEATURE_SGLTE_DUAL_SIM or FEATURE_SVLTE_DUAL_SIM */
}

/*===========================================================================

FUNCTION gl1_set_gfw_to_active

DESCRIPTION
The requirement is that GFW should only enter ACTIVE mode when going
dedicated/packet transfer. This is because a MPCM_UNLOAD/LOAD is required.
The gonly sub should always enter GFW state MEAS_ACTIVE and only enter ACTIVE
mode when going dedicated/packet transfer.
This is because the multimode sub will generally need to be in GFW state
ACTIVE in order to do G2W searches and hence it is more desirable to be
in ACTIVE state immediately than jump around between gfw states.
PARAMS
  Active sub

DEPENDENCIES

RETURN VALUE


===========================================================================*/
boolean  gl1_set_gfw_to_active(gas_id_t gas_id)
{
#if 0 /* below if from Nikel DSDS, don't need this leave for reference */
  gas_id_t other_gas_id = GERAN_ACCESS_STRATUM_ID_2;
  if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    other_gas_id = GERAN_ACCESS_STRATUM_ID_1;
  }
  /*
  If Both GL1 subs are active AND the other sub is not in G2W BPLMN
  OR
  this sub is not GSM Only
  then can move GFW to Active
  */
  if(( ((l1_tsk_buffer[GERAN_DATA_SPACE_INDEX_1].sync_status == DS_ASYNCHRONISED) ||
        (l1_tsk_buffer[GERAN_DATA_SPACE_INDEX_2].sync_status == DS_ASYNCHRONISED)) &&
          l1_tsk_buffer[other_gas_id].l1_state != L1_SUSPEND_MODE )
       || ( FALSE == l1_sc_wcdma_get_gsm_only_mode(gas_id)))
  {
    return TRUE;
  }else
  {
    return FALSE;
  }
#else
 return TRUE;
#endif
}

/*===========================================================================

FUNCTION  gl1_check_any_other_client_has_lock

DESCRIPTION
   This function is necessary for DSDS to indicate if other sim has lock.
   For SS and DSDA the function always returns FALSE

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_check_any_other_client_has_lock( gas_id_t gas_id )
{
  int     i;
  int     as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );
  boolean gsm_client_has_lock = FALSE;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( i != as_idx )
    {
      if ( ( grm_get_trm_status(l1_tsk_buffer[i].client_id, GERAN_MAP_DATA_SPACE_INDEX_TO_GAS_ID(i)) == GL1_TRM_GRANTED ) ||
           ( grm_get_trm_status(l1_tsk_buffer[i].client_id, GERAN_MAP_DATA_SPACE_INDEX_TO_GAS_ID(i)) == GL1_TRM_RETAINED_FOR_ACCESS ) )
      {
        gsm_client_has_lock = TRUE;

        break;
      }
    }
  }

  return ( gsm_client_has_lock );
}

/*===========================================================================

FUNCTION gl1_is_any_sub_in_transfer

DESCRIPTION
         TRUE if any active SUB is in TRANSFER mode

PARAMS
  None

DEPENDENCIES

RETURN VALUE


===========================================================================*/
boolean gl1_is_any_sub_in_transfer( void )
{
  return ( gl1_check_any_subs_l1_state( L1_TRANSFER_MODE ) );
}

/*===========================================================================

FUNCTION gl1_check_any_subs_l1_state

DESCRIPTION
  Returns TRUE if any subs have that l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_any_subs_l1_state( byte l1_state )
{
  int     i;
  boolean l1_state_set = FALSE;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( l1_tsk_buffer[i].l1_state == l1_state )
    {
      l1_state_set = TRUE;

      break;
    }
  }

  return ( l1_state_set );
}

/*===========================================================================

FUNCTION gl1_check_all_other_subs_l1_state

DESCRIPTION
  Returns TRUE if all other SUBs subs have that l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_all_other_subs_l1_state( gas_id_t gas_id, byte l1_state  )
{
  int     i;
  boolean l1_state_set = TRUE;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( ((gas_id_t)i!= gas_id ) && (l1_tsk_buffer[i].l1_state != l1_state) )
    {
      l1_state_set = FALSE;

      break;
    }
  }

  return ( l1_state_set );
}

/*===========================================================================

FUNCTION gl1_check_gonly_mode_active

DESCRIPTION
  Returns if G only mode is active in Multi SIM environment

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_gonly_mode_active( void )
{
  boolean gonly_active = FALSE;

  switch ( gl1_msg_get_multi_sim_sys_mode())
  {
    case SYS_MODEM_DEVICE_MODE_SINGLE_SIM:
      gonly_active = TRUE;
      break;

    case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY:
    case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE:
      /* Inactive GSM SUBs = 0 : This is G + G */
      /* Inactive GSM SUBs = 1 : This is X + G */
      /* Inactive GSM SUBs = 2 : This is X + X */
      if( gl1_get_num_inactive_subs() == 0 )
      {
        gonly_active = TRUE;
      }
      break;

    case SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY:
      gonly_active = (!gl1_check_any_subs_l1_state( L1_INACTIVE_MODE ));
      break;

    default:
      break;
  }

  return gonly_active;
}

/*===========================================================================

FUNCTION gl1_get_gas_id_of_other_idle_sub

DESCRIPTION
  Returns the gas id of the other idle SUB in WGG mode

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
gas_id_t gl1_get_gas_id_of_other_idle_sub(gas_id_t gas_id)
{
  int      i;
  gas_id_t other_idle_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if( ( (gas_id_t)i!= gas_id ) && ( l1_tsk_buffer[i].l1_state == L1_MULTI_SIM_IDLE ) )
    {
      other_idle_gas_id = (gas_id_t)i;
      break;
    }
  }

  return other_idle_gas_id;
}

/*===========================================================================

FUNCTION gl1_get_client_id_of_other_idle_sub

DESCRIPTION
  Returns the client id of the other idle Sub

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
grm_client_enum_t gl1_get_client_id_of_other_idle_sub(gas_id_t gas_id)
{
   grm_client_enum_t client_id;

   gas_id_t gas_id_r =  gl1_get_gas_id_of_other_idle_sub(gas_id);

   client_id = gl1_map_gas_id_to_client_id( gas_id_r );

   return client_id;
}

/*===========================================================================

FUNCTION gl1_get_gas_id_of_other_g_sub

DESCRIPTION
  Returns gas_id of other GSM sub

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
gas_id_t gl1_get_gas_id_of_other_g_sub( gas_id_t gas_id  )
{
  int      i;
  gas_id_t other_g_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( ((gas_id_t)i!= gas_id ) && (l1_tsk_buffer[i].l1_state != L1_INACTIVE_MODE) )
    {
      other_g_gas_id = (gas_id_t)i;

      break;
    }
  }

  return ( other_g_gas_id );
}

/*===========================================================================

FUNCTION gl1_get_gas_id_of_other_g_sub_in_transfer

DESCRIPTION
  Returns gas_id of other GSM sub

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
gas_id_t gl1_get_gas_id_of_other_g_sub_in_transfer( gas_id_t gas_id  )
{
  int     i;
  gas_id_t other_g_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( ((gas_id_t)i!= gas_id ) && (l1_tsk_buffer[i].l1_state == L1_TRANSFER_MODE))
    {
      other_g_gas_id = (gas_id_t)i;

      break;
    }
  }

  return ( other_g_gas_id );
}

/*===========================================================================

FUNCTION gl1_check_any_other_sub_in_bg_traffic

DESCRIPTION
  checks if any other sub is in BG TRAFFIC

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/

boolean gl1_check_any_other_sub_in_bg_traffic( gas_id_t gas_id )
{
  int     i;
  boolean bg_traffic = FALSE;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    /*
     * Can use grm_get_reason to allow for different client request
     * that updates the data store for the active gas_id only
     */
    if ( ( i != gas_id ) &&
         ( TRM_BG_TRAFFIC
           == grm_gl1_get_current_trm_reason(gl1_map_gas_id_to_client_id(i)
                                             ,gas_id ) )
       )
    {
      bg_traffic = TRUE;

      break;
    }
  }

  return bg_traffic;
}

/*===========================================================================

FUNCTION gl1_check_any_subs_trm_status

DESCRIPTION
  Returns TRUE if any subs have that trm_status

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_any_subs_trm_status( gl1_trm_state_t trm_status )
{
  int     i;
  boolean trm_status_set = FALSE;

  for ( i = 0; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( grm_get_trm_status(l1_tsk_buffer[i].client_id, GERAN_MAP_DATA_SPACE_INDEX_TO_GAS_ID(i)) == trm_status )
    {
      trm_status_set = TRUE;

      break;
    }
  }

  return ( trm_status_set );
}

/*===========================================================================

FUNCTION gl1_check_num_inactive_subs

DESCRIPTION
  Returns number of idle GSM SUBS

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
uint8 gl1_get_num_inactive_subs( void )
{
  int   i;
  uint8 num_inactive_g_subs = 0;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( l1_tsk_buffer[i].l1_state == L1_INACTIVE_MODE)
    {
      num_inactive_g_subs++;
    }
  }

  return ( num_inactive_g_subs );
}


/*===========================================================================

FUNCTION gl1_check_any_other_subs_l1_state

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_any_other_subs_l1_state( gas_id_t gas_id,
                                           byte     l1_state )
{
  int     i;
  int     as_idx;
  boolean l1_state_set = FALSE;

  if (gas_id > GERAN_ACCESS_STRATUM_ID_MAX)
  {
    MSG_GERAN_ERROR_1("Invalid gas id: %d", gas_id);
    return FALSE;
  }
  as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( i != as_idx )
    {
      if ( l1_tsk_buffer[i].l1_state == l1_state )
      {
        l1_state_set = TRUE;

        break;
      }
    }
  }

  return ( l1_state_set );
}

/*===========================================================================

FUNCTION gl1_check_any_other_subs_active_state

DESCRIPTION
  Returns TRUE if any other client has going_active set TRUE

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_any_other_subs_active_state( gas_id_t gas_id )
{
  int     i;
  int     as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );
  boolean l1_active_set = FALSE;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( i != as_idx )
    {
      if ( l1_tsk_buffer[i].going_active )
      {
        l1_active_set = TRUE;

        break;
      }
    }
  }

  return ( l1_active_set );
}

/*===========================================================================

  FUNCTION  gl1_is_l1_state_ptm

  DESCRIPTION
  This function returns status of L1_TRANSFER_MODE STATE
  DEPENDENCIES
  None

  RETURN VALUE
  Boolean TRUE/FALSE

  SIDE EFFECTS


===========================================================================*/
boolean gl1_is_l1_state_ptm(gas_id_t gas_id)
{ 
  return( l1_tsk_buffer[gas_id].l1_state == L1_TRANSFER_MODE );
}
/*===========================================================================

  FUNCTION  gl1_is_l1_state_dtm

  DESCRIPTION
  This function returns status of L1_DTM_MODE STATE
  DEPENDENCIES
  None

  RETURN VALUE
  Boolean TRUE/FALSE

  SIDE EFFECTS


===========================================================================*/
boolean gl1_is_l1_state_dtm(gas_id_t gas_id)
{ 
  return( l1_tsk_buffer[gas_id].l1_state == L1_DTM_MODE );
}

/*===========================================================================

  FUNCTION  gl1_is_l1_state_msim_idle

  DESCRIPTION
  This function returns status of L1 MULTISIM IDLE STATE
  DEPENDENCIES
  None

  RETURN VALUE
  Boolean TRUE/FALSE

  SIDE EFFECTS


===========================================================================*/
boolean gl1_is_l1_state_msim_idle(gas_id_t gas_id)
{ 
#ifdef FEATURE_DUAL_SIM
  return( l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_IDLE ) ;
#else
  return FALSE;
#endif
}

#ifdef FEATURE_GPRS_GBTA

/*===========================================================================

FUNCTION gl1_grm_is_gbta_allowed

DESCRIPTION
  Returns TRUE if GBTA is ALLOWED

PARAMS
  None

DEPENDENCIES

RETURN TRUE/FALSE

===========================================================================*/
boolean gl1_grm_is_gbta_allowed(void)
{
  boolean gbta_allowed = FALSE;
  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  gbta_allowed = (gbta_state == GRM_GBTA_ALLOWED) ? TRUE : FALSE;
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
  return gbta_allowed;
}

/*===========================================================================

FUNCTION gl1_grm_is_gbta_blocked

DESCRIPTION
  Returns TRUE if GBTA is BLOCKED

PARAMS
  None

DEPENDENCIES

RETURN TRUE/FALSE

===========================================================================*/
boolean gl1_grm_is_gbta_blocked(void)
{
  boolean gbta_blocked = FALSE;
  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  gbta_blocked = (gbta_state == GRM_GBTA_BLOCKED) ? TRUE : FALSE;
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
  return gbta_blocked;
}

/*===========================================================================

FUNCTION gl1_grm_is_gbta_active

DESCRIPTION
  Returns TRUE if GBTA is ACTIVE

PARAMS
  None

DEPENDENCIES

RETURN TRUE/FALSE

===========================================================================*/
boolean gl1_grm_is_gbta_active(void)
{
  boolean gbta_active = FALSE;
  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  gbta_active = (gbta_state == GRM_GBTA_ACTIVE) ? TRUE : FALSE;
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
  return gbta_active;
}

/*===========================================================================

FUNCTION gl1_grm_is_gbta_inative

DESCRIPTION
  Returns TRUE if GBTA is INACTIVE

PARAMS
  None

DEPENDENCIES

RETURN TRUE/FALSE

===========================================================================*/
boolean gl1_grm_is_gbta_inative(void)
{
  boolean gbta_inactive = FALSE;
  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  gbta_inactive = (gbta_state == GRM_GBTA_NONE) ? TRUE : FALSE;
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
  return gbta_inactive;
}

/*===========================================================================

FUNCTION gl1_grm_is_gbta_allowed_or_active

DESCRIPTION
  Returns TRUE if GBTA is ALLOWED Or ACTIVE

PARAMS
  None

DEPENDENCIES

RETURN TRUE/FALSE

===========================================================================*/
boolean gl1_grm_is_gbta_allowed_or_active(void)
{
  boolean gbta_allowed_or_active = FALSE;
  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  gbta_allowed_or_active = ((gbta_state == GRM_GBTA_ALLOWED) ||   (gbta_state == GRM_GBTA_ACTIVE)) ? TRUE : FALSE;
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
  return gbta_allowed_or_active;
}

/*===========================================================================

FUNCTION gl1_grm_is_set_gbta_status

DESCRIPTION
   Set GBTA state

PARAMS
  grm_gbta_state_t

DEPENDENCIES

RETURN NONE

===========================================================================*/
void gl1_grm_is_set_gbta_status(grm_gbta_state_t gbta_status)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH

  int   i;
  gas_id_t idle_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
   
   
  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if( l1_tsk_buffer[i].l1_state == L1_MULTI_SIM_IDLE )
   {
      idle_gas_id = (gas_id_t)i;
      break;
   }
  }
  if(idle_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED)
  {
   if(gbta_status == GRM_GBTA_NONE)
   {
     gl1_qsh_event_notify(idle_gas_id,GL1_QSH_EVENT_GBTA_NONE);
   }
   else if(gbta_status == GRM_GBTA_ALLOWED)
   {
     gl1_qsh_event_notify(idle_gas_id,GL1_QSH_EVENT_GBTA_ALLOWED);
   }
   else if(gbta_status == GRM_GBTA_ACTIVE)
   {
     gl1_qsh_event_notify(idle_gas_id,GL1_QSH_EVENT_GBTA_ACTIVE);
   }
   else
   {
     gl1_qsh_event_notify(idle_gas_id,GL1_QSH_EVENT_GBTA_BLOCKED);
   }
  }
#endif

  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  gbta_state = gbta_status;
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
}

/*===========================================================================

FUNCTION gl1_grm_get_gbta_status

DESCRIPTION
  get GBTA state

PARAMS
  None

DEPENDENCIES

RETURN grm_gbta_state_t

===========================================================================*/
grm_gbta_state_t gl1_grm_get_gbta_status(void)
{
  grm_gbta_state_t temp_gbta_state;
  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  temp_gbta_state = gbta_state;
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
  return temp_gbta_state;
}

/*===========================================================================

FUNCTION gl1_check_any_other_subs_idle_evt_power_mon

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_any_other_subs_idle_evt_power_mon( gas_id_t gas_id ,gas_id_t *idle_gas_id_ptr)
{
  int     i;
  int     as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );
  boolean idle_mon_scheduled = FALSE;

  GL1_MULTI_SIM_MODE_GBTA_LOCK();

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( i != as_idx )
    {
      if (grm_gbta_info[i].idle_event == GBTA_EV_IDLE_PWR_MON)
      {
        idle_mon_scheduled = TRUE;
        *idle_gas_id_ptr  = (gas_id_t)i;

        break;
      }
    }
  }

  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();

  return ( idle_mon_scheduled );
}

/*===========================================================================

FUNCTION gl1_check_other_subs_idle_event_status

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_other_subs_idle_event_status(grm_gbta_idle_event_status_t stat,
                                               gas_id_t gas_id,gas_id_t *idle_gas_id_ptr )
{
  int     i;
  int     as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );
  boolean idle_event_status_active = FALSE;

  GL1_MULTI_SIM_MODE_GBTA_LOCK();
  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( (i != as_idx)&& (grm_gbta_info[i].idle_event_status == stat))
    {
      idle_event_status_active = TRUE;
      *idle_gas_id_ptr = (gas_id_t)i;
      break;
    }
  }
  GL1_MULTI_SIM_MODE_GBTA_UNLOCK();

  return idle_event_status_active;
}

/*===========================================================================

FUNCTION gl1_check_other_sub_release_pending

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_other_sub_release_pending(void)
{
  gas_id_t gas_id = gl1_get_gas_id_for_ptm_client();

  return (gpl1_gprs_leaving_transfer(gas_id));
}

/*===========================================================================

FUNCTION gl1_check_other_sub_reconfig_progress

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_other_sub_reconfig_progress( uint32 frames_required )
{
  gas_id_t gas_id = gl1_get_gas_id_for_ptm_client();

  return ( gpl1_gprs_reconfig_progress(frames_required, gas_id));
}

/*===========================================================================

FUNCTION gl1_check_other_sub_sc_op_progress

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_other_sub_sc_op_progress( void )
{
  gas_id_t gas_id = gl1_get_gas_id_for_ptm_client();

  return (l1_sci_is_op_in_progress_gbta(gas_id));
}

/*===========================================================================

FUNCTION gl1_check_other_sub_id_search_progress

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_other_sub_id_search_progress( void )
{
  gas_id_t gas_id = gl1_get_gas_id_for_ptm_client();

  return (l1_sc_irat_state_machine_active(gas_id));
}

/*===========================================================================

FUNCTION gl1_check_other_sub_irat_progress

DESCRIPTION
  Returns TRUE if other SUB is doing IRAT

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_other_sub_irat_progress( void )
{
  gas_id_t gas_id = gl1_get_gas_id_for_ptm_client();
  return (l1_sc_irat_get_gbta_not_allowed(gas_id));
}


/*===========================================================================

FUNCTION gl1_check_other_sub_ncell_meas_progress

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_other_sub_ncell_meas_progress( void)
{
   boolean ncell_meas_in_prog = FALSE;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
   gpl1_g2x_ta_priority_t pri = G2X_TA_PRI_MAX;
#endif

   gas_id_t gas_id = gl1_get_gas_id_for_ptm_client();

   if( l1_sci_meas_in_progress_gbta( gas_id))
   {
     ncell_meas_in_prog = TRUE;
   }
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
   else if( gpl1_g2x_ta_is_activty_running(&pri, gas_id) &&
           (pri == G2X_TA_PRI_SERV_BCCH) )
   {
     ncell_meas_in_prog = TRUE;
   }
#endif

  return ncell_meas_in_prog;
}
/*===========================================================================

FUNCTION gl1_check_other_sub_in_tbf_susp

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_other_sub_in_tbf_susp(void)
{
  gas_id_t gas_id = gl1_get_gas_id_for_ptm_client();
  if( GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id )
  {
    return FALSE;
  }
  else if ( l1_tsk_buffer[gas_id].current_params.L1Data.pTransfer_data != NULL )
  {
    return ((l1_tsk_buffer[gas_id].current_params.L1Data.pTransfer_data->disable_rx_tx));
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION gl1_other_sub_check_tbf_susp_planned

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_other_sub_check_tbf_susp_planned(void)
{
  gas_id_t gas_id = gl1_get_gas_id_for_ptm_client();
  return (gpl1_gbta_check_tbf_susp_planned(gas_id));
}
#endif

/*===========================================================================

FUNCTION gl1_check_any_other_subs_fw_load

DESCRIPTION
  Returns TRUE if any other client has the requested l1_state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_any_other_subs_fw_load( gas_id_t gas_id )
{
  int     i;
  int     as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );
  boolean l1_fw_load_set = FALSE;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( i != as_idx )
    {
      if ( l1_tsk_buffer[i].firmware_loaded )
      {
        l1_fw_load_set = TRUE;

        break;
      }
    }
  }

  return ( l1_fw_load_set );
}

/*===========================================================================

FUNCTION gl1_get_gas_id_for_ptm_client

DESCRIPTION
  Returns array index corresponding to requested client

PARAMS
  Desired client

DEPENDENCIES

RETURN VALUE

===========================================================================*/
gas_id_t gl1_get_gas_id_for_ptm_client( void )
{
  gas_id_t gas_id, i;

  gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( l1_tsk_buffer[i].l1_state == L1_TRANSFER_MODE )
    {
      gas_id = (gas_id_t)i;
      break;
    }
  }

  return ( gas_id );
}

/*===========================================================================

FUNCTION gl1_get_peer_gas_id_for_ptm_client

DESCRIPTION
  Returns array index corresponding to requested client

PARAMS
  Desired client

DEPENDENCIES

RETURN VALUE

===========================================================================*/
gas_id_t gl1_get_peer_gas_id_for_ptm_client( void )
{
  gas_id_t gas_id, i;

  gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( (l1_tsk_buffer[i].l1_state != L1_TRANSFER_MODE) &&
         (grm_get_trm_status(l1_tsk_buffer[i].client_id, GERAN_MAP_DATA_SPACE_INDEX_TO_GAS_ID(i)) == GL1_TRM_GRANTED))
    {
      gas_id = (gas_id_t)i;
      break;
    }
  }

  return ( gas_id );
}


/*===========================================================================

FUNCTION gl1_other_sub_check_missed_page

DESCRIPTION
  Returns a boolean flag to indicate if another SUB missed a page

PARAMS
  Desired client

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_other_sub_check_missed_page( gas_id_t gas_id )
{
   int     i;
   int     as_idx = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX( gas_id );
   boolean l1_missed_page = FALSE;

   GL1_MULTI_SIM_MODE_GBTA_LOCK();

   for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
   {
     if ( i != as_idx )
     {
       if ( grm_gbta_info[i].missed_page )
       {
         l1_missed_page = TRUE;

         break;
       }
     }
   }

   GL1_MULTI_SIM_MODE_GBTA_UNLOCK();
   
   return ( l1_missed_page );
}



/*===========================================================================

FUNCTION get_ms_background_hplmn_state

DESCRIPTION
  Returns the status of bplmn

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
background_hplm_state_t gl1_ms_background_hplmn_state( gas_id_t gas_id )
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  return ( l1_multi_sim_data_ptr->background_hplm_state );
}

/*===========================================================================

FUNCTION gl1_multi_sim_reset_trm_request_state

DESCRIPTION
  resets the trm_request_state to TRM_REQ_NULL

PARAMS
  None

DEPENDENCIES

RETURN
  NONE

===========================================================================*/
void gl1_multi_sim_reset_trm_request_state( gas_id_t gas_id )
{
  l1_multi_sim_t *l1_multi_sim_data_ptr = &l1_multi_sim[gas_id];

  l1_multi_sim_data_ptr->trm_request_state = TRM_REQ_NULL;
}

/*===========================================================================

FUNCTION gl1_check_possible_isr_overrun

DESCRIPTION
  Check if we are too much into frame during normal ISR runs to avoid issuing commands to FW too close

PARAMS
  None

DEPENDENCIES

RETURN
  NONE

===========================================================================*/

boolean gl1_check_possible_isr_overrun(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  sys_modem_as_id_e_type as_id  = geran_map_gas_id_to_nas_id( gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* If its rude wake and FW RF is being initialised in this frame, then skip scheduling any
   * commands to FW in this frame, but only if its first RF init after rude wake up
   */
  if(gl1_hw_get_rf_in_deep_sleep_whilst_forced_wakeup(gas_id))
  {
     MSG_GERAN_ERROR_0_G( "First activity after the rude wake up" );
     /* During forced wake up only bring RF out of deep sleep once */
     gl1_hw_set_rf_in_deep_sleep_whilst_forced_wakeup(gas_id, FALSE);
	 /* set the req next tick, so that we dont go to sleep in the same frame in SS mode */
     gl1_drx_require_next_tick(gas_id);
     /* If this is not from QTA GAP then return, if QTA GAP then fall through to next check */
     if(l1_tskisr_blk->qta_in_progress == FALSE && gl1_msg_get_multi_sim_mode())
     {
       return TRUE;
     }
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  if (l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr == FALSE)
  {
     /* Only for MSIM case and
      * (if GSTMR ISR not pending and we are close to frame boundary or if short frame number has changed)
      */
     if (gl1_msg_get_multi_sim_mode() &&
          ( (!gstmr_is_int_pending(gas_id) && (gstmr_rd_qsymbol_count(as_id) >= 4000))
        || (get_vstmr_FN_modMaxFN(as_id) != vstmr_rd_modMaxFN_frame_num(gas_id))
        ))
     {
       MSG_GERAN_MED_2_G( "Possible ISR overrun int_pending:%d qs:%d",!gstmr_is_int_pending(gas_id), gstmr_rd_qsymbol_count(geran_map_gas_id_to_nas_id(gas_id)));
       return TRUE;
     }
  }
#if defined ( FEATURE_QTA )
  else if (gl1_check_qta_active_in_warmup(gas_id))
  {
      /* gl1_check_qta_active_in_warmup(gas_id) == TRUE */
      /* If we wake up in QTA gap, this will be first instance, therefore force missing first burst of CCCH */
      /* Prevent FM from releasing in the same frame */
      l1_fm_set_suspend_trm_release(SUSPENSION_FRAME_DELAY,FM_PRI_RESERVED,gas_id);
      l1_tskisr_blk->firmware_loaded = FALSE;
      MSG_GERAN_MED_1_G(GL1_SLEEP_HDR "Possible ISR overrun whilst qta_in_progress:%d ",l1_tskisr_blk->qta_in_progress);
      return TRUE;
  }
#endif

  return FALSE;
}

/*===========================================================================

FUNCTION gl1_concman_scenario_config_request

DESCRIPTION
This function will call MCPM api and decides wheather iRat activity can be go ahead based on
MCPM return status

PARAMS


DEPENDENCIES
To be Called Only for Irat Activities in dedicated Mode

RETURN VALUE
Activity Accepted/Rejected by MCPM concurrency manager

===========================================================================*/

cmgr_scenario_status_e gl1_concman_scenario_config_request(gl1_concman_config_params_type scenario_params, gas_id_t gas_id)
{
	cmgr_scenario_status_e status;
	cmgr_tech_type tech = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GERAN_TECH : CMGR_GERAN1_TECH);
	
	cmgr_params input;
	memset(&input,0,sizeof(cmgr_params));
	input.scenario_e = scenario_params.scenario;
	input.priority = scenario_params.priority;
	input.flow_ctrl_time_msec = scenario_params.flow_ctrl_time_msec;
	input.duration_msec = scenario_params.duration_msec;
	input.start_timestamp = scenario_params.start_timestamp;
	input.type = scenario_params.type;
	
    status = cmgr_scenario_config(
            tech,
            &input
            );
	MSG_GERAN_HIGH_4_G("conc_mngr: Status:%d Type:%d Scenario:%d priority:%d",
                            status, 
		     				scenario_params.type,
					     	scenario_params.scenario,
		     				scenario_params.priority);
	if (status == CMGR_ACCEPTED || status == CMGR_REJECTED)
    {
      return status;
    }
    else
    {
      ERR_GERAN_FATAL_1_G("conc_mngr: Incorrect Value %d returned from Concurrency Manager",status);
	  return status;
    }

}



/*===========================================================================

FUNCTION grm_check_ptm_irat_sc_activity_ok

DESCRIPTION
Checks the surround cell irat activity scheduled for next frame is ok to run.
Sends BAND information to trm

PARAMS


DEPENDENCIES
To be Called Only for Irat Activities in dedicated Mode

RETURN VALUE
boolean TRUE if activity can  run

===========================================================================*/
boolean grm_check_ptm_irat_sc_activity_ok(uint16 freq_offset,
                                          uint32 frames_required,
                                          uint32 cmngr_scenario,
                                          gas_id_t gas_id)
{

#ifdef FEATURE_WCDMA
  l1_freq_scan_band_enum_type w_band;
#endif
  grm_client_enum_t meas_client = GRM_MAX_CLIENTS;
  sys_band_class_e_type trm_band = SYS_BAND_CLASS_NONE;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_info_t *info_ptr = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id, gas_id);
  boolean irat_allowed = TRUE;
#ifdef FEATURE_GSM_TO_LTE
  lte_earfcn_t dl_earfcn;
  sys_sband_lte_e_type  lte_band;
#endif
  l1_sc_wcdma_data_t* l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
#ifdef DEBUG_DRDSDS
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
#endif
  uint32   uarfcn = 0;
  gl1_concman_config_params_type gl1_concman_config_params;
  cmgr_scenario_status_e status;
  cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

  if( !l1_sc_wcdma_data_ptr )
  {
    MSG_GERAN_ERROR_0_G(GL1_DRDS_DBG_HDR"grm_check_ptm_irat_sc_activity_ok: l1_sc_wcdma_data_ptr == NULL");
    return FALSE;
  }

  if( !info_ptr )
  {
    MSG_GERAN_ERROR_0_G(GL1_DRDS_DBG_HDR"grm_check_ptm_irat_sc_activity_ok: info_ptr == NULL");
    return FALSE;
  }
  if(cmngr_scenario != G2X_TA_SEQ)
  {
  	gl1_concman_config_params.type = CMGR_TRANSIENT_START;
  	gl1_concman_config_params.scenario = cmngr_scenario;
  	gl1_concman_config_params.update_scenario = 0;
  	gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_IRAT_MEASUREMENT);
  	gl1_concman_config_params.start_timestamp = 0;
  	gl1_concman_config_params.duration_msec = 0;
  	gl1_concman_config_params.flow_ctrl_time_msec = 0;

  	status = gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);

  	if (status == CMGR_REJECTED)
  	{
  		MSG_GERAN_HIGH_1_G("conc_mngr: iRat activity denied by concurrency manager Status:%d",
                            status);
		return FALSE;
  	}
  	if (status == CMGR_ACCEPTED)
  	{
  		MSG_GERAN_HIGH_1_G("conc_mngr: iRat activity accepted by concurrency manager Status:%d",
                            status);
  	}
  }

  /*Check which RAT is using this frame and register BAND with TRM*/
#ifdef FEATURE_WCDMA
  if( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_offset].rat == RR_L1_IRAT_RAT_WCDMA)
  {
    uarfcn = (uint32)l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_offset].arfcn.UARFCN;
    w_band = wl1_trm_get_band(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_offset].arfcn.UARFCN);
    trm_band = trm_map_l1_band_type[w_band].band;
    meas_client = WCDMA_CLIENT_FOR_THIS_SUB(gas_id);
    /* For G2W search, WFW may have a LMEM concurrency issue with 1X paging on the other SUB,
       so GL1 is asked to wait till WFW is finished processing its Offline Searches as well
       So Further Extend Frames_required for Band Registration*/
    frames_required = MAX_FRAMES_FOR_G2W_DED_SEARCH;
    MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR" Band registration duration updated for Ded G2W:%d", frames_required);
  }
#endif
#ifdef FEATURE_GSM_TO_LTE
  else if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_offset].rat == RR_L1_IRAT_RAT_LTE)
  {
    dl_earfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_offset].arfcn.euarfcn;
    uarfcn = (uint32)dl_earfcn;
    (void)lte_ml1_common_band_get_band_from_dl_earfcn(dl_earfcn, &lte_band);
    trm_band = lte_ml1_convert_lte_band_to_sys_band(lte_band);
    meas_client =  LTE_CLIENT_FOR_THIS_SUB(gas_id);
  }
#endif /*FEATURE_GSM_TO_LTE*/
#ifdef FEATURE_GSM_TDS
 else
 {
   if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_offset].rat == RR_L1_IRAT_RAT_UTRAN_TDD)
   {
     uarfcn   =       (uint32)l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_offset].arfcn.UARFCN;
       trm_band = tdrm_get_band(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_offset].arfcn.UARFCN);
     meas_client = GRM_TDSCDMA;
   }
 }
#endif /* FEATURE_GSM_TDS */

  /*build and send TRM band request*/
  if (trm_band != SYS_BAND_CLASS_NONE)
  {

   irat_allowed = grm_check_irat_band_reg(meas_client, trm_band,frames_required, FALSE, gas_id);
   /*band request denied by TRM so idle frame can not be used for IRAT measurements*/
   if(FALSE == irat_allowed)
   {
     MSG_GERAN_HIGH_3_G(GL1_DRDS_DBG_HDR"IRAT idle frame meas on client %d DENIED by TRM for band %d for uarfcn=%d",
                                        meas_client,
                                        trm_band,
                                        uarfcn );

#ifdef DEBUG_DRDSDS
     if( l1_sc_globals_ptr)
     {
       MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"TRM denied IRAT activity: Active Rat %d, W search %d",
                                          l1_sc_globals_ptr->active_irat,
                                          l1_sc_globals_ptr->wcdma_srch_in_progress);
     }
#endif
   }
   else
   {
     MSG_GERAN_HIGH_3_G(GL1_DRDS_DBG_HDR"IRAT idle frame meas on client %d GRANTED by TRM for band %d for uarfcn=%d",
                                         meas_client,
                                         trm_band,
                                         uarfcn);
   }
 }
 else
 {
   MSG_GERAN_ERROR_0_G(GL1_DRDS_DBG_HDR"not able to match freq to TRM band ");
 }
 return irat_allowed;
}

gl1_sc_activities_t map_fm_priority_to_sc_activity(l1_fm_priority_T pri)
{
  switch (pri)
  {
    case FM_PRI_NCELL_BCCH:     return SC_BCCH;
    case FM_PRI_NCELL_ACQ:      return SC_FCH_SCH;
    case FM_PRI_NCELL_SCH:      return SC_FCH_SCH;
    case FM_PRI_WCDMA_IDSEARCH: return SC_IRAT;
    case FM_PRI_WCDMA_RECONF:   return SC_IRAT;

#ifdef FEATURE_GSM_TDS
    case FM_PRI_TDS_ACQ:        return SC_IRAT;
#endif /* FEATURE_GSM_TDS */

#ifdef FEATURE_GSM_TO_LTE
    case FM_PRI_LTE_IDSEARCH:   return SC_IRAT;
#endif

    default: return SC_MAX_ACTIVITIES;
  }
}
/*===========================================================================

FUNCTION grm_check_idle_sc_activity_ok

DESCRIPTION
Checks the surround cell activity booked into frame manager is ok to run.
Sends BAND information to trm

PARAMS


DEPENDENCIES

RETURN VALUE
boolean TRUE if activity can run

===========================================================================*/
boolean grm_check_idle_sc_activity_ok(l1_fm_priority_T pri,
                                      uint32 frames_required,
                                      uint32 reserved_fn,
                                      gas_id_t gas_id)
{
  /*For this feature all suround cell  activities on non-serving cell bands must be reported to TRM
   TRM may deny the requested BAND in which case clear out the activity from FM*/

  l1_sc_wcdma_data_t    *wcdma_ptr  = &l1_sc_wcdma_data[gas_id];
#ifdef FEATURE_WCDMA
  l1_freq_scan_band_enum_type w_band;
#endif
  sys_band_class_e_type trm_band = SYS_BAND_CLASS_NONE;
#ifdef FEATURE_GSM_TO_LTE
  lte_earfcn_t earfcn;
  sys_sband_lte_e_type  lte_band;
#endif
  boolean trm_request_required = FALSE;
  boolean activity_can_run = TRUE;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_info_t *info_ptr = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id, gas_id);
  /*Initialize meas-client to GRM_MAX_CLIENTS*/
  grm_client_enum_t meas_client = GRM_MAX_CLIENTS;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  grm_info_t *sec_info_ptr = NULL;
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

  gl1_sc_activities_t sc_activity = map_fm_priority_to_sc_activity(pri);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  sec_info_ptr = grm_get_info_ptr_from_client(
                              grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id,gas_id),
                              gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

  if (sc_activity == SC_MAX_ACTIVITIES)
  {
    MSG_GERAN_LOW_2_G(GL1_DRDS_DBG_HDR"Not A DR-DSDS SC_ACTIVITY %d pri %d",sc_activity,pri);
    return TRUE;
  }

  switch (pri)
  {
    case FM_PRI_WCDMA_IDSEARCH:
#ifdef FEATURE_WCDMA
    {
       /*do not send TRM set client state within QTA gap*/
       if(!gl1_hw_qta_gap_active(gas_id))
       {
         /*trm set client state only sent in all modes but not during QTA gap */
         w_band = wl1_trm_get_band(wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.UARFCN);
         trm_band = trm_map_l1_band_type[w_band].band;
         /*adding measured client info for TRM IRAT band reg.*/
         meas_client          = WCDMA_CLIENT_FOR_THIS_SUB(gas_id);
         trm_request_required = TRUE;
       }
    }
#endif
      break;

    case FM_PRI_WCDMA_RECONF:
#ifdef FEATURE_WCDMA
    {
       /*do not send TRM set client state within QTA gap*/
       if(!gl1_hw_qta_gap_active(gas_id))
       {
         w_band = wl1_trm_get_band(wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_reconfirm_freq_offset].arfcn.UARFCN);
         trm_band = trm_map_l1_band_type[w_band].band;
         /*adding measured client info for TRM IRAT band reg.*/
         meas_client          = WCDMA_CLIENT_FOR_THIS_SUB(gas_id);
         trm_request_required = TRUE;
       }
    }
#endif
    break;

#ifdef FEATURE_GSM_TDS
    case FM_PRI_TDS_ACQ:
      {
        /*do not send TRM set client state within QTA gap*/
        if(!gl1_hw_qta_gap_active(gas_id))
        {
          if(wcdma_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
          {
            if (wcdma_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM)
            {
              trm_band = tdrm_get_band(wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_reconfirm_freq_offset].arfcn.UARFCN);
            }
          else
          {
            trm_band = tdrm_get_band(wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.UARFCN);
          }
          /*adding measured client info for TRM IRAT band reg.*/
          meas_client  = GRM_TDSCDMA;
          trm_request_required = TRUE;
         }
       }
      }
      break;
#endif /* FEATURE_GSM_TDS */

#ifdef FEATURE_GSM_TO_LTE
    case FM_PRI_LTE_IDSEARCH:
    {
       /*do not send TRM set client state within QTA gap*/
       if(!gl1_hw_qta_gap_active(gas_id))
       {
        /*For LTE set client state always sent. L does not use IRAT reservation and reserves G band before sending IRAT request*/
        earfcn = wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.euarfcn;
        (void)lte_ml1_common_band_get_band_from_dl_earfcn(earfcn, &lte_band);
        trm_band = lte_ml1_convert_lte_band_to_sys_band(lte_band);
        /*adding measured client info for TRM IRAT band reg.*/
          meas_client =  LTE_CLIENT_FOR_THIS_SUB(gas_id);
        trm_request_required = TRUE;
    }
    }
      break;
#endif

    default:
        MSG_GERAN_ERROR_2_G(GL1_DRDS_DBG_HDR" Not an IRAT DR-DSDS SC_ACTIVITY %d pri %d",sc_activity,pri);
      break;
   }

  MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"SC Activity %d on trm_band %d", sc_activity, trm_band);


  if(trm_band != SYS_BAND_CLASS_NONE)
  {

    if (trm_request_required && info_ptr)
    {
      if(wcdma_ptr->rr_params.fastSearchState & FAST_SEARCH_ACTIVE )
      {
        if(info_ptr)
        {
          /*
          ** re-regsiter, release previous band registration else next fails and measurement is skipped
          */
          if(info_ptr->grm_activity_buf[SC_IRAT][GRM_ACT_IRAT].update_in_progress)
          {
            grm_transaction_release(SC_IRAT, &info_ptr->grm_activity_buf[SC_IRAT][GRM_ACT_IRAT], l1_tskisr_blk->client_id, gas_id);
          }
        }
      }
      /* primary */
      activity_can_run = grm_check_irat_band_reg(meas_client, trm_band,frames_required,FALSE, gas_id);
    }

  }

  return activity_can_run;
}

/*===========================================================================

FUNCTION grm_check_w_t_reconf_in_pg_block_ok

DESCRIPTION
before scheduling a W or T reconf search within a paging block first check the
required frequenncy band is OK to use on the currently held RF chain.

PARAMS


DEPENDENCIES

RETURN VALUE
boolean TRUE if activity can run

===========================================================================*/
boolean grm_check_w_t_reconf_in_pg_block_ok(uint32 frames_required,
                                            uint32 freq_index,
                                            gas_id_t gas_id)
{
#ifdef FEATURE_WCDMA
 l1_freq_scan_band_enum_type w_band;
#endif
  grm_client_enum_t meas_client = GRM_MAX_CLIENTS;
 sys_band_class_e_type trm_band = SYS_BAND_CLASS_NONE;
 volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_info_t *info_ptr = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id,
                                                      gas_id);

 boolean activity_can_run = TRUE;
 l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
 l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

 if ( !info_ptr )
 {
    MSG_GERAN_ERROR_0_G(GL1_DRDS_DBG_HDR"grm_check_w_t_reconf_in_pg_block_ok: "
                                       "info_ptr == NULL invalid ");
   return FALSE;
 }

 /*Check which RAT is using this paging block and register BAND with TRM*/
#ifdef FEATURE_WCDMA
  if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].rat)
 {
   w_band = wl1_trm_get_band(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].arfcn.UARFCN);
   trm_band = trm_map_l1_band_type[w_band].band;
    meas_client = WCDMA_CLIENT_FOR_THIS_SUB(gas_id);
   /* For G2W search, WFW may have a LMEM concurrency issue with 1X paging on the other SUB,
      so GL1 is asked to wait till WFW is finished processing its Offline Searches as well
      Hence Further Extend Frames_required for Band Registration*/
   frames_required = MAX_FRAMES_FOR_G2W_DED_SEARCH;
   MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR" Band registration duration updated for ReConf Ded G2W:%d", frames_required);
 }
 else /*T reconf*/
#endif
 {
#ifdef FEATURE_GSM_TDS
    if(RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].rat)
       {
         trm_band = tdrm_get_band(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].arfcn.UARFCN);
      meas_client = GRM_TDSCDMA;
   }
#endif /* FEATURE_GSM_TDS */
 }

  /*build and send TRM band request*/
  if (trm_band != SYS_BAND_CLASS_NONE )
  {
      if (grm_check_irat_band_reg(meas_client,trm_band, frames_required, FALSE, gas_id))
    {
      MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"grm_check_w_t_reconf_in_pg_block_ok: "
                                         "activity_can_run %d for band %d ",
                         activity_can_run, trm_band );
    }
    else
    {
      /*band request denied by TRM so idle frame can not be used for IRAT measurements*/
#ifdef DEBUG_DRDSDS
      MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"IRAT idle frame meas on client %d denied by TRM for band %d",
        meas_client, trm_band);
#endif
      /*Don't allow IRAT to proceed in this idle frame*/
      activity_can_run = FALSE;

        MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"TRM denied IRAT activity: Active Rat %d, W search %d",
                           l1_sc_globals_ptr->active_irat,
                           l1_sc_globals_ptr->wcdma_srch_in_progress);
      }
    }
  else
  {
     MSG_GERAN_ERROR_0_G(GL1_DRDS_DBG_HDR"not able to match freq to TRM band ");
     activity_can_run = FALSE;
  }

  return activity_can_run;
}

/*===========================================================================

FUNCTION gl1_get_client_id_of_other_sub_in_transfer

DESCRIPTION
  Returns the client id of the other sub_in_transfer

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
grm_client_enum_t gl1_get_client_id_of_other_sub_in_transfer(gas_id_t gas_id)
{
   grm_client_enum_t client_id;

   gas_id_t gas_id_r =  gl1_get_gas_id_of_other_g_sub_in_transfer(gas_id);

   client_id = gl1_map_gas_id_to_client_id( gas_id_r );

   return client_id;
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION gl1_check_num_idle_G_subs

DESCRIPTION
  Returns number of idle GSM SUBS

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
uint8 gl1_get_num_idle_subs( void )
{
  int  i;
  uint8 num_idle_g_subs = 0;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if ( gl1_cell_valid_state_check( l1_tsk_buffer[i].l1_state ) )
    {
      num_idle_g_subs++;
    }
  }

  return ( num_idle_g_subs );
}

/*===========================================================================

FUNCTION gl1_check_qta_active_in_warmup

DESCRIPTION
  Returns TRUE if qta gap active in warmup ISR or else FALSE

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_qta_active_in_warmup(gas_id_t gas_id)
{
#ifdef FEATURE_SLEEP_FROM_QTA
#ifdef FEATURE_QTA
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /*Is Warm up ISR? && QTA gap active*/
   if( (l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr == TRUE) &&
         gl1_hw_qta_gap_active(gas_id))
   {
      return TRUE;
   }
#endif /* FEATURE_QTA */
#endif /* FEATURE_SLEEP_FROM_QTA */

   return FALSE;
}

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
/*===========================================================================

FUNCTION gl1_check_if_sub_in_two_phase_access

DESCRIPTION
  Returns TRUE if sub is in 2 phase access state

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/

boolean gl1_check_if_sub_in_two_phase_access(gas_id_t gas_id)
{
  boolean ret = FALSE;

  if( (l1_transfer_data[(gas_id)].current_alloc_type == L1_SINGLE_BLOCK_ALLOC)  && 
      (l1_transfer_data[(gas_id)].single_block_tbf_ptr->two_phase_access == TRUE))
  {
    ret = TRUE;
  }

  return ret;
}

#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER

/*===========================================================================

  FUNCTION  gl1_get_current_trm_client_id

  DESCRIPTION
  This function gets the correct trm client id (as that might have changed for pch in tranfer)

  DEPENDENCIES
  None

  RETURN VALUE
  trm client id

  SIDE EFFECTS


===========================================================================*/
grm_client_enum_t gl1_get_current_trm_client_id( grm_client_enum_t client )
{
  gas_id_t gas_id = gl1_map_client_id_to_gas_id(client);
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  MSG_GERAN_HIGH_1_G("gl1_get_current_trm_client_id %d", l1_tskisr_blk->client_id);
  return l1_tskisr_blk->client_id;
}
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

#if defined(FEATURE_SGLTE_DUAL_SIM) || defined(FEATURE_SVLTE_DUAL_SIM)
/*===========================================================================

FUNCTION l1_handle_device_mode_cause_plmn

DESCRIPTION


PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void l1_handle_device_mode_cause_plmn(const gas_id_t gas_id)
{
  grm_client_enum_t          client_id;
  trm_grant_return_enum_type grm_request_grant;
  trm_reason_enum_t          reason;
  sys_proc_type_e_type       subreason;
  geran_procedure_t          geran_procedure = GERAN_CONN_PROCEDURE;
  volatile ISRTIM_CMD_BLK    *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  boolean                     rach_for_ps = FALSE;

  GL1_ISR_LOCK(gas_id);

  MSG_GERAN_HIGH_3_G("Handle BPLMN stop state %d main command %d trm status %d", l1_tskisr_blk->l1_state,
                     l1_tskisr_blk->main_command, grm_get_trm_status(l1_tskisr_blk->client_id, gas_id));

  client_id = l1_tskisr_blk->client_id;

  geran_get_traffic_reason(&rach_for_ps, gas_id);
  if (rach_for_ps)
  {
      reason = grm_get_connection_priority (GRM_PS_CONNECTION,gas_id);
      geran_procedure = GERAN_GPRS_PROCEDURE;
      subreason = gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id);
  }
  else
  {
      reason = grm_get_connection_priority (GRM_CS_CONNECTION,gas_id);
      subreason = gl1_get_specific_sys_proc (GERAN_CONN_PROCEDURE, gas_id);
  }

  if ( TRM_DENIAL != grm_get_trm_grant_status(client_id, gas_id))
  {
    rfm_device_enum_type rf_device =
      gl1_hw_get_gsm_rf_id(client_id, gas_id );

    MSG_GERAN_HIGH_2_G("RF Device %d rach_for_ps=%d", rf_device, (uint8)rach_for_ps);
    /* @@TODO: needs to be revisited if SGLTE is required or removed if it is not*/
    if ( rach_for_ps && ( rf_device != RFM_DEVICE_0 ) )
    {
      if (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
      {
#ifdef FEATURE_SGLTE_DUAL_SIM
        /*   client id, event , unlock by, gas id*/
        gta_handle_abort_for_l_or_t_callback( client_id, TRM_UNLOCK_IMMEDIATELY, 1, gas_id );
#endif
      } else if (l1_tskisr_blk->l1_state == L1_MULTI_SIM_DECOUPLED)
      {
        l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
      } else if (l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE)
      {
        /* tell GRR to trigger a PS abort and give the 1 sec connection time to get the lock */
        L1_send_MPH_L1_DS_ABORT_IND(gas_id, RR_DS_ABORT_TRANSFER, 0x3E8 );
        l1_tskisr_blk->main_command = L1_MULTI_SIM_IDLE_COMMAND;
      }
    }
  } else
  {
    /* TRM Not granted */
    if ((grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_CALLBACK_PENDING) &&
        (l1_tskisr_blk->l1_state == L1_MULTI_SIM_DECOUPLED))
    {
      /* pending TRM */
      /* used to cancel the pending reservation */
      grm_release( client_id, gas_id );

      grm_request_grant = grm_request( client_id,
                                       GRM_ACCESS_DURATION_SCLKS ,
                                       reason,
                                       subreason,
                                       TRM_RX_BEST_POSSIBLE_MOD_2,
                                       GRM_DIVRX_TRM_REQ_INVALID );

      /*Update local grant status*/
      grm_set_trm_grant_status(grm_request_grant, client_id, gas_id);

      if ( grm_request_grant != TRM_DENIAL )
      {
        grm_set_trm_status(GL1_TRM_RETAINED_FOR_ACCESS, client_id, gas_id);

          l1_tskisr_blk->main_command = L1_ACCESS_COMMAND;
      }
      else
      {
        /* set up the retry with TRM on MM Device*/
        grm_setup_access_retry(FALSE, PS_ACCESS_MM_DEVICE, reason, GRM_DIVRX_TRM_REQ_INVALID,geran_procedure, GRM_ACCESS_DURATION_SCLKS, gas_id);
        /* Request the lock now for this using the asynchronous TRM interface */

      }
    }
  }

  GL1_ISR_UNLOCK(gas_id);
}
#endif

/*===========================================================================

FUNCTION gl1_check_svlte_mode_active

DESCRIPTION
  Get if SVLTE Mode is Active from TRM.

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/

boolean gl1_check_svlte_mode_active(void)
{
#if defined(FEATURE_SVLTE_DUAL_SIM)
   trm_get_info_input_type input;
   trm_get_info_return_type output;

   input.client_id = TRM_GSM1;
   input.info_type = TRM_SIMUL_CAPABILITY;

   trm_get_info(&input, &output);

   if ( output.info.simul_cap_info & ( TRM_SVLTE_IS_ENABLED))
   {
     return TRUE;
   }
#endif
   return FALSE;
}


#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE

/*===========================================================================

FUNCTION gpl1_grm_rx_sec_supported_reason

DESCRIPTION


PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gpl1_grm_rx_sec_supported_reason(l1_fm_priority_T pri)
{
  boolean ret_value = FALSE;

  if (pri == FM_PRI_CCCH ||
      pri == FM_PRI_NCELL_BCCH ||
      pri == FM_PRI_NCELL_ACQ ||
      pri == FM_PRI_NCELL_SCH)
  {
    ret_value = TRUE;
  }

  return ret_value;
}
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */


/*===========================================================================

FUNCTION gl1_setup_request_and_notify_for_bplmn

DESCRIPTION


PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void gl1_setup_request_and_notify_for_bplmn(grm_client_enum_t dest_client, uint32 duration, gas_id_t gas_id )
{

  /* If the BPLMN mechanism can not run now as the next activity is too close,
     ensure that this next activity is booked into TRM too.*/
  /* add request and notify */
  /* setup all the data required to make the request to trm for the lock */
  grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
  grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

  grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

  /* Ask for the amount of time required for a complete reselection */
  grm_request_and_notify_enhanced_data_p->client_id = dest_client;

  //Update The Bands for BPLMN Now
  gpl1_select_band_to_req_trm(bplmn_idle_band_bitmap[gas_id], dest_client, gas_id);

  switch (L1_get_current_background_HPLMN_search_activity(gas_id))
  {
     case POWER_SCAN:
  grm_request_and_notify_enhanced_data_p->reason    = TRM_ACQUISITION;
       break;
     case ACQ_BCCH_LIST:
       grm_request_and_notify_enhanced_data_p->reason    = TRM_ACQUISITION;
       break;
     default:
       MSG_GERAN_ERROR_0_G("background_HPLMN_search_activity unknown");
  grm_request_and_notify_enhanced_data_p->reason    = TRM_ACQUISITION;
       break;
  }

  grm_request_and_notify_enhanced_data_p->sub_reason = SYS_PROC_TYPE_BPLMN_SEARCH;
  grm_request_and_notify_enhanced_data_p->resource  =
     gl1_ms_get_trm_resource(gas_id, grm_request_and_notify_enhanced_data_p->reason);
  grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(duration, T_MSEC);
  grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_ACQUISITION;
  grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, dest_client, gas_id);
  call_in_task_grm_request_and_notify_enhanced(grm_request_and_notify_enhanced_data_p);
}
/*===========================================================================

FUNCTION gl1_cancel_bplmn_request

DESCRIPTION
  Cancels the pending or granted trm , clears the local trm status store
  and goes back to initial BPLMN enter state

PARAMS
  None

DEPENDENCIES

RETURN VALUE
None
===========================================================================*/
void gl1_cancel_bplmn_request(grm_client_enum_t client , gas_id_t gas_id)
{
  grm_set_trm_status(GL1_TRM_NULL, client, gas_id);
  /* Trm has denied the lock, re issue the request by starting again */
  l1_multi_sim[gas_id].background_hplm_state = L1_MULTI_SIM_BGRND_HPLMN_NULL;
  L1_background_HPLMN_search_suspend_cb(gas_id);
}
#endif /*FEATURE_DUAL_SIM*/
/*===========================================================================

FUNCTION gl1_check_if_getting_deactivated

DESCRIPTION
  Returns true if gl1_getting_deactivated

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean gl1_check_if_getting_deactivated(gas_id_t gas_id)
{
   boolean ret_value = FALSE;
   volatile ISRTIM_CMD_BLK    *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   if ((l1_tskisr_blk->main_command== L1_DEACTIVATE_COMMAND)||
        (l1_tskisr_blk->l1_state == L1_INACTIVE_MODE) || (l1_tskisr_blk->l1_state == L1_SUSPEND_MODE))
  {

      ret_value = TRUE;
  }
   return(ret_value);
}

#ifdef FEATURE_DUAL_DATA
/*===========================================================================

FUNCTION gl1_set_data_sub_priority

DESCRIPTION
  Sets specific Subscription to assigned priority as sets other subscriptions
  to GERAN_PRIORITY_NOMINAL

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void gl1_set_data_sub_priority(geran_priority_t priority,gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint16 i=0;

  l1_tskisr_blk->priority = priority;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if (gas_id != i)
  {
      l1_tsk_buffer[i].priority = GERAN_PRIORITY_NOMINAL;

      /* Revert Multislot Class other subs in case it was
       * reduced during MMS transfer */
      if (priority == GERAN_PRIORITY_NOMINAL)
    {
        MSG_GERAN_HIGH_1_G("MMS-DATA Reverting MSC on gas_id: %d", i);
        L1_send_MPH_COEX_MSC_CHANGE_IND(0, 0, i);
        gl1_cfcm_data.gl1_mms_cfcm_state = GL1_GFCM_NULL;
        gl1_cfcm_data.gl1_mms_gfn_timer = L1_GPRS_INVALID_FN;
      }
    }
  }
  MSG_GERAN_HIGH_1_G ("MMS-DATA pri %d",priority);
    }

/*===========================================================================

FUNCTION gl1_get_data_sub_priority

DESCRIPTION
  Returns the priority of subscription

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
geran_priority_t gl1_get_data_sub_priority(gas_id_t gas_id)
        {
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   return(l1_tskisr_blk->priority);
}

/*===========================================================================

FUNCTION gl1_get_low_priority_data_sub

DESCRIPTION
  Returns the priority of low priority data subscription if one sub is found
  with GERAN_PRIORITY_INVERTED.

  If no sub is found with GERAN_PRIORITY_INVERTED data priority then will
  return GERAN_ACCESS_STRATUM_ID_UNDEFINED

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
gas_id_t gl1_get_low_priority_data_sub(void)
{
   gas_id_t gl1_mms_low_priority_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

   /* Check if we are in Data+MMS mode and assign gas_id for DDS sub*/
   if (gl1_get_data_sub_priority(GERAN_ACCESS_STRATUM_ID_1) == GERAN_PRIORITY_INVERTED)
   {
     gl1_mms_low_priority_gas_id = GERAN_ACCESS_STRATUM_ID_2;
   }
   else if (gl1_get_data_sub_priority(GERAN_ACCESS_STRATUM_ID_2) == GERAN_PRIORITY_INVERTED)
   {
     gl1_mms_low_priority_gas_id = GERAN_ACCESS_STRATUM_ID_1;
   }

   return(gl1_mms_low_priority_gas_id);
}
#endif /* FEATURE_DUAL_DATA */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  l1_isr_multi_sim_inactive

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_multi_sim_inactive( gas_id_t gas_id )
{

   if (l1_sc_irat_state_machine_active(gas_id))
   {
     l1_sci_tick_wcdma(gas_id);
   }

}

/*===========================================================================

FUNCTION  gpl1_determine_rach_reasons

DESCRIPTION
  Determines TRM reason and sub-reason for a new RACH attempt

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_determine_rach_reasons(const grm_conn_type_t  conn_type,
                                             trm_reason_enum_t     *reason_ptr,
                                             sys_proc_type_e_type  *subreason_ptr,
                                             gas_id_t               gas_id)
{
  trm_reason_enum_t      rach_reason;

  if (GRM_CS_CONNECTION == conn_type)
  {
    /* connection type is circuit-switched, RACH priority is based on MPH_SET_PRIORITY_IND */
    rach_reason = grm_get_connection_priority(conn_type, gas_id);
    if (subreason_ptr != NULL)
    {
      *subreason_ptr = gl1_get_specific_sys_proc (GERAN_CONN_PROCEDURE, gas_id);
    }
  }
  else
  {
    /* connection type is packet-switched, RACH always uses ACCESS priority */
    rach_reason = TRM_ACCESS;
    if (subreason_ptr != NULL)
    {
      *subreason_ptr = gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id);
    }
  }

  /* update the output parameters if provided */
  if (reason_ptr != NULL)
  {
    *reason_ptr = rach_reason;
  }

  return;
}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/* EOF */
