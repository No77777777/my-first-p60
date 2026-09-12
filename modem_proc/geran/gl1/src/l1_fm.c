/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      L A Y E R  1  F R A M E  M A N A G E R

GENERAL DESCRIPTION
   This module contains procedures to support the Neighbor cell activity
   scheduling in Layer 1. Together these functions make up the Frame manager.

EXTERNALIZED FUNCTIONS
l1_fm_init
l1_fm_reset
l1_fm_tick
l1_fm_are_frames_available
l1_fm_set_required_frame_number
l1_fm_clear_required_frames
l1_fm_get_available_frames
l1_fm_extend_required_frames
l1_fm_log_event

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_fm.c#4 $
$DateTime: 2020/06/07 04:13:53 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
05/06/20   sc        CR2703314 TDS Removal on Agatti- Moving G2T/T2G code under TDS feature
20/03/17   ksb       CR2021239 Extend the TRM  in QTA for the near NBCCH irrespective of high priority flag
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
19/06/18   rc        CR2259553 Release the lock if trm_status is GL1_TRM_NOT_GRANTED.
13/06/18   sk        CR2250958 Do not change freq info if resource lock is active
16/02/18   sk        CR2191275 Suppress duplicate reservation when already one pending in the task
12/03/18   sp        CR2202251 Dont extend DRx chain beyond Prx Chain. Also Drx release can be inline
07/03/18   sn        CR2199105 Priortize DL TBF over CCCH based on Reserved Frame number
26/02/18   sp        CR2195726 Ensure to use right freq Band before trm request.
25/09/17   bg        CR2114113 Extend duration is not required in Page Reorg mode
09/11/17   ng        CR2140861 Do not allow Non Irat activity in QTA gap opened for IRAT activity
17/08/17   rc        CR2082810 Not to schedule TC=1 if TC = 0 is not decoded in CB non-drx-mode and update the priority of CB to low  in drx mode.
16/06/17   rc        CR2058052 To avoid supress wrong FM booking
31/01/18   rv        CR2042600: Fix for - Activity 4 is continuously aborted due to lack of frames when page is colliding in SSIM mode
10/02/17   akm       CR2000470 Two page miss consecutively after wakeup cancellation
23/01/18   rv        CR2019399: Define G2T IRAT as LTA activity
26/12/17   sn        CR2160741: Gl1 Coex timeline updation at Wakeup Cancel of Idle Sub
23/01/18   rrp       CR2124956 Perform extend duration only when we have lock.
04/01/18   sk        CR2167071 fix compiler error
23/01/17   ap        CR1080314 IFS Band registration overflows in Paging Re-Org mode
04/03/17   rc        CR2010767 Page miss indication is sent twice for the same page in dual sim case
14/11/16   sn        CR1087650 Adding proper condition to check if QTA started for releasing TRM
14/04/16   akm       CR999764 Extended page is prioritized over NBCCH during collision
05/01/16   zf        CR954362: After prioritized NBCCH cancelled CCCH, clear its required frames
03/25/16   zf        CR995061: Check if activity reserved before starting ncell activity
20/12/17   sk        CR2030816 GERAN changes for PBR Randomization algorithm
08/06/17   akm       CR2053712 CBCH is not scheduled after wakeup cancellation for page
25/04/16   sp        CR953124: Dont Prioritize CB over NBCCH
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
15/11/17   ng        CR1021532 Make G2L Null2X (and changes for all G2X as Null2X)
29/10/17   km        CR2133917 GL1 support for L+L
10/08/17   bg        CR2090176 Set change_reason to TRUE to request for extending the lock in page reorg mode
05/07/16   br        CR2063984 TA in Reorg-mode: Dont extend  TRM lock for acq activity while UE in page-reorg mode
05/07/17   br        CR2060300 TA in Reorg mode: Extended page misses on one sub during page reorg on other sub.
12/06/17   br        CR2058885 During G2X TA in page Reorg mode, dont change the prio while extending the lock
02/02/17   br        CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
07/12/16   ap        CR1096256 When granted TRM for IRAT do not attempt non IRAT operation
23/05/16   snjv      CR1012850 Release Bands bef clearing Band Reg strct & also extend trm properly
29/08/16   rv        CR1059564: Fix for-GL1 Stuck while aborting idle mode, FW load status set to FW_PENDING_RELEASE
26/08/16   mn        CR1059703 Remove debug print 
02/08/16   mn        CR1049025 Extend TRM lock if current activity is PCH and previous activity is SCH ,when QTA is under progress 
04/07/16   sk        CR1037263 Don't call IFS band registration in extend_duration() if band is same as earlier 
15/07/16   sn        CR1041906 CBCH activity proceeded with Invalid RF device
22/06/16   mn        CR1032142 Not allowing extension of TRM duration when GBTA is active
16/06/16   sk        CR1029565 No need to check drx_ref for stale activity as stale activities's tick release already taken care in gl1_drx_tick()
15/06/16   sn        CR1028121 Abort CBCH activity when it overlaps at the same frame number with any HP activity.
09/06/16    mn       CR1026351 Resolve linking error in QTF mob build
06/03/2016 ng        CR1022465 Clear drx_ref once IRAT activity gets scheduled
31/05/16   mn        CR1022253 Idle DTF/Handlers
06/05/16   sp        CR975500: Dont abort LP activity at FM request as both HP/LP cant run in that frame, If required it gets aborted through l1_fm_tick()
16/05/16   db        CR1009490 Move CBCH priority handling from GRR to GL1
09/05/16   sm        CR1002861 Added QSH GL1 handler : PCH collision with Ncell activity 
12/04/16   zf        CR1002584: Set DRX reference for W reconf when activity is reserved
03/05/16   bg        CR998612 TRM request with proper G band for the priority CBCH reception
03/05/16   bg        CR1002250 Do not extend the TRM lock in QTA gap when peer tech tx is not active
11/4/2016  sn        CR1000581 Releasing TX chain when resource is going inactive.
14/03/16   am        CR987997:NBCCH is not prioritized over page during collision when QTA is in progess
22/02/16   bg        CR978038 Don't extend the TRM lock when Gsub is holding resource lock
04/02/15   sk        CR972370/CR887629 Back to original behavior for prioritised NBCCH over CCCH 
19/01/16   zf        CR964732 Don't start NBCCH when CCCH is in progress
12/01/16   sk        CR960922 Ensure GBTA active frame count is reset at the end of the GBTA gap
06/01/16   sp        CR957494: Allow clear of CBCH DRx ref, but maintain next CBCH Occasion so that skip ind can be sent when X-tech in traffic.
30/12/15   js        CR956837 Set DRX reference only when IRAT activity is reserved
28/12/15   sp        CR955380: Apply only Low Prio CBCH when any tech in BG traffic / in X+G modes
29/12/15   mko       CR955880: Clear CBCH frames and remove Drx entry when other tech is in bg traffic
23/11/15   hd        CR953597 Check if the pending CBCH priority is not FM_PRI_MAX while checking if it is not active priority
21/12/15   ng        CR938638 Reducing no of F3s
03/12/15   hd        CR945450 Remove the setting of l1_ccch_blocked_by_nbcch while CCCH activity is blocked by NBCCH
10/12/15   sp        CR948656 Check and update trm reserve reason if CB prio change is triggered by RR.
23/11/15   cws       CR943080 Do not set asdiv when waking up for irat
26/26/15   mko       CR941539: After wakeup cancel, add correct FN offset while calculating  gap to next activity
24/11/15   sp        CR924033 Apply RR requested CB priority inversion, only on next TC=0 occassion.
24/04/15   am        CR825716 Delay SCH scheduling after 4 TRM denials in idle mode
19/11/15   sk        CR934976 Block low priority CB requests when other sub is doing W2G BPLMN search
16/11/15   sp        CR939632 Allow fix of CR/931668 work for SR-DSDS mode also.
10/11/15   ap        CR934796 GERAN Acquisition attempted in a LTE QTA gap aimed for IRAT measurement
28/10/15   snjv      CR931668 During TRM extend ensure band of ncell Acq. is registered 
04/10/15   cjl       CR934494 Set GRM state to GL1_TRM_GRANTED on delayed geran lock release
02/11/15   sk        CR932290 Ensure NBCCH is not aborted when prioritized over CCCH
28/09/15   cjl       CR929471 Delay sleep after geran lock release in idle
20/10/15   ap        CR923515 Do not extend duration of diversity chain if denied by TRM
13/10/15   zf        CR922465 Do not extend TRM after completed IRAT activity
14/10/15   sk        CR919643 Ensure ID search is given priority when the reservation is overlapped with low priority CBCH
14/10/15   sk        CR914199 Ensure NBCCH is prioritized over serving PCH when PCH Drx is matching with SI3 period
06/10/15   dv        CR914186 During TRM extend ensure band of ncell SCH is registered  
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
14/09/15   nk        CR899259 If BPLMN is active, check that GL1 is in NOT in 
                              non DRX mode and only then return FALSE for extend trm duration
30/09/15   cjl       CR914688 Abort blocked acquisition attempts when leaving idle
16/09/15   hd        CR905810 Check if drx_ref is already set before registering for NCELL activity
21/09/15   snjv      CR908747 If No Higher Prio. task scheduled then allow sceduled activity to go ahead
16/09/15   js        CR901570 Don't schedule CBCH if other tech tx is active
09/09/15   ap        CR901286 TRM lock is incorrectly retained for impending SCH activity in QTA GAP
07/08/15   am        CR899527 Ensure GL1 wakeup and reserve a high priority task
24/08/15   cjl       CR872907 Notify GRR of serving cell BCCH TRM denials
20/08/15   pa        CR893756:Grouped TRM apis for the diversity chain. 
26/08/15   ws       CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
19/08/15   snju      CR893264 Set AsDiv Idle Mode when Beginning BPLMN after suspension
12/08/15   hd        CR888214: Exchange the FM activity if SET_PRIORITY_IND has come to invert and a CBCH activity is already booked
11/08/15   hd        CR873848 Ignore the unlock callback for CBCH activity during PTM
11/08/15   zf        CR888623: Don't book FM activity at the current frame
11/08/15   fan/akh   CR871795 FR19329: Avoid cancelled CCCH booking after rude wakeup
06/08/15   cah       CR885274: GL1 booking W reconf using Channel Maintenance  reason instead of the new IRAT reason
16/07/15   mko       CR871748: When HPLMN_search_in_progres don't set suspend_trm_release() by one frame
                     after page decode if Ncell ACQ is pending.
31/07/15   cah       CR879928:  IRAT set client state  removed  for QTA   
15/07/15   sjv       CR869294 Book in DRX manager for Msim Idle NCell activity only while reserving TRM 
30/07/15   fan/akh   CR880067 FR19329: Disable support for non-CCCH activity cancellation
07/02/15   sjv       CR865682 Query TRM directly to check granted state instead of GL1 internal vars
26/06/15   sjv       CR862422 Rgrsn fix of CR860020: set l1_ccch_blocked_by_nbcch flag consdering trm_grant_status
23/06/15   sjv       CR860020 Dont Release TRM When CCCH is blocked prior, due to NBCCH
29/07/15   nk        CR880746 Dont attempt reservation for WID search if it is for the current frame number
28/07/15   js        CR857088 Release TRM lock while aborting idle when received DL_TBF in QTA gap.
27/07/15   ap        CR880017: BPLMN activity being aborted incorrectly
27/07/15   ap        CR877555: Reduction number of calls to TRM when doing BPLMN
04/06/15   mko       CR848332: In BG_TRAFFIC, clear drx tick entry when FM table for CBCH  is cleared
23/07/15   cjl       CR878351 fix compiler warning
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types
26/05/15   nm        CR842930 back out CR 768113 as it is causing a reserve at too close for Ncell SCH
15/07/15   fan/akh   CR857332 FR19329: WMGR support for rescheduling non-PCH activities
10/07/15   jk        CR866667:Don't request TRM for DRx chain when waking up for ACQ
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
08/07/15   df        CR815683 Avoid error recovery when G2W scripts are built during wakeup
06/07/15   sp        CR862778: Reduce F3 logging in Idle as its impacting QTA timeline
24/06/15   ap        CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR
02/07/15   aga       CR860197 NULL2L workaround Reserve and request serving band for LTE IRAT
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
26/06/15   js        CR860602 Reset FW load flag when lock is extended in QTA gap/correcting offset check.
23/06/15   smd       CR860261 Request IRAT band no matter DR is enabled or not.
23/06/15   sp        CR859168: Dont clear trm_requested_at_warmup from frame manager
13/06/15   hd        CR853527 Clear missed frames due to error recovery when doing a reserve_at
23/06/15   fan/akh   CR859092 Adapt WMGR Cancel handling to RACH Rescheduling in DSDA, DR-DSDS
04/06/15   fan/akh   CR848455 Fix for multiple handlings of same cancelation request
01/06/15   js        CR814275 GL1 changes for ROTA interface during extended paging
08/06/15   mc        CR837381 Do not allow IRAT activites to attempt to run in GBTA gaps
03/06/15   cws       CR846011 Clear all irat fm activity when srch state goes to NULL
03/06/15   og        CR844464. Ensure the GSM band is not part of the G2W TRM request.
04/06/15   ab        CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
22/04/15   am        CR803948 TA in idle mode when mm_non_drx is enabled
07/04/15   nm        CR816454 reset trm_requested_at_warmup when MSIM_CBCH activity doesn't run
28/04/15   br        CR806209 GL1 to extend the trm lock if ncell SCH activity is within 11 frames from the current FN
20/05/15   aga       CR840984 NULL2X IRAT changes for G2W IRAT
19/05/15   sjw       CR840271: clear inactive CBCH FM entries when priority is changed
15/05/15   aga       CR838339 Reset the value for the saved frame number if activity got cancelled
13/05/15   cah       CR836017: Do not change priority or extend duartion if trm status is not granted
04/05/15   zf        CR832554: Fix wrong parameters in calling grm_check_idle_sc_activity_ok function
08/04/15   mc        CR816376: Fix defect in solution for CR519643 : Missed Page due to SCH running in QTA gap
07/05/15   fan/akh   CR834237 FR19329 Full G wake-up cancellation + code cleanup
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
28/04/15   ap        CR824511: Gl1 is calculating an incorrect grant duration for bplmn.
                               GL1 is not assigning bplmn abort callback in trm grant cb.
24/04/15   fan       CR796481 Full G wakeup cancellation
21/04/15   sjw       CR826445: reinstate TRM_DEMOD_CBCH for nominal Cell Broadcast
10/04/15   fm        CR816312:GL1 extends TRM for the impending xCCH when LTE is acquiring NW.
16/04/15   smd       CR822202 Dual space data for IRAT and QTA
18/03/15   am        CR808945 Need to take 1 frame into account for reserving next task
03/02/15   cjl       CR790433 Create separate TRM activity for WCDMA reconfirm when needed
14/04/15   aga       CR799530 Reserve W/TD/L IRAT ID search with TRM in advance
10/04/15   cah       CR819666 Problem in the logic for adding the denied bands to trm_reserve_at and trm_request
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
30/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
30/03/15   ap        CR811753 FR22272 - TRM Unification - GL1 changes to support
                                        SysInfo priority inversion during early camping
27/03/15   dv/pa     CR813849: In X+G DR-DSDS, revert back to SR after 3 consequtive TRM denials
27/03/15   ap        CR811054 FR22272 - TRM Unification  - Extended Paging with PBR
23/03/15   ap        CR811046:FR22272 - TRM Unification  - Removal of _HI reasons/UNLOCK_IMMEDIATELY
26/03/15   sjw       CR813087: use CHANNEL_MAINTENANCE_INV for high-priority CBCH
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
24/03/15   sjw       CR811752: refactored priority inversion for CBCH
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
03/03/15   nk        CR802349 Dont allow G2X activity to run in GBTA gap
03/04/15   br        CR736774 GL1 logging improvement to ease debugging
04/03/14   cah       CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good serving cell conditions
18/02/15   zc        CR795392 Revert back to SR-DSDS on denial of TRM in DR-DSDS mode
04/02/15   nm        CR788432 Extend TRM lock after PCH if NBCCH is less than inter QTA gap inside a QTA gap
27/02/15   am        CR646584 WLAN coex
25/02/15   nm        CR783148 Check for TRM_NUM_REAONS before extending frames
24/02/15   js        CR749012: GL1 changes for LTE removal
15/01/15   cah       CR781585: GL1 registers invalid TRM band for TD booking.
20/02/14   cjl       CR789907 FR24151: Prevent frequent tuneaways for NCELL BCCH reading causing RLF on LTE
20/02/15   js        CR693771: W+T removal. Resolving compilation errors
11/02/15   nm        CR791093 Do not use DEMOD_PAGE_INV reason for NBCCH when prioritized inside QTA
19/02/15   sp        CR774887 Deprioritize NBCCH read over CCCH when BPLMN is running.
19/02/15   nk        CR787609 Defer TS change if there's a pending command at GFW.
16/01/14   nm        CR778945 Arbitrtor time entry was not added for the first frame of next PCH
19/02/15   fm        CR791018 GL1 should check SIM mode for NCell SCH Rx before starting it
19/02/15   cah       CR780986: TRM_PENDING not handled by grm_change_reason api.
19/02/15   js        CR793536 Clearing NBCCH while aborting idle and ensuring trm is released in QTA,correcting fix done in CR790660
18/02/15   js        CR784968 Correcting TRM reason code in in QTA for NBCCH
18/02/15   am        CR791181 Reset GBTA active status if GBTA is no longer required
18/02/15   am        CR791074 ISR overrun check for drx task sleep
13/01/15   zf        CR780369: Do not abort NBCCH when leave idle.
19/01/14   js        CR782723 Do not Abort SCE while clearing stale CBCH activity.
15/02/15   sp        CR729492 Remove unimportant logging
13/02/15   pa        CR756156:G2X RF device handling using TRM apis.
29/01/15   pa        CR785021: Disable DR on TRM denial to activate GBTA on next PCH decode.
28/01/15   fm        CR763710 GL1 checks if the current TRM reservation is for NCell SCH Rx before starting it
26/01/15   npt       CR786002 Fix compilation errors when FEATURE_DUAL_SIM is enabled
21/01/15   fm        CR783834 after completing one activity, if the incoming activity occurs in 11 frames and DRDSDS is enabled, GL1 shall extend the TRM grant rather than release/reserve it.
21/01/15   pa        CR758792:RxD support for multi-sim.
07/01/15   dp        CR771533 only check sched overhead if we're operating in legacy DR-mode
15/12/14   sk        CR758093 Do not request the lock again whe GBTA grant is available on missed frames during wakeup
12/12/14   sk        CR763710 Ensure NBCCH is read when consistently colliding with serving PCH or other sub's PCH
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   sjv       CR769423 Ensure l1_idle_mode_isr returns TRUE only when it is safe to release TRM
10/12/14   ws        CR768376 Q6 compile warning fixes
09/12/14   cws       CR726562 Setting suspend TRM release frames as gaps to CCCH activity
08/12/14   sjv       CR768113 If in QBTA, Do Not Extend TRM for any activity after attempting PCH dec.
03/12/14   npt       CR759467 Fix compiler warnings
21/11/14   am        CR758032 Missed_page for GBTA should be set only when GBTA is not blocked by upper layers
20/11/14   sk        CR760119 Correcting the porting error from CR735697
18/11/14   sp        CR758079: Block CBCH Requests when other sub is in Packet Transfer
20/10/14   sk        CR735697 Ensure GL1 doesnt toggle between SI3/SI4 on TRM denial, and does explicit TRM release before making reservation in non-G only mode
03/11/14   sjv       CR750056 IRAT activities need to be aborted/cleared when an update_list is received
01/11/14   df        CR748602 Rework and enable adaptive timeline
05/11/14   fan       CR751146 Fix wakeup error recovery mechanism on Thor
04/11/14   ap        CR749044 Mismatch between firmware_load_status and firmware_loaded  causing TRM to be not requested
29/10/14   cah       CR710799 DR-DSDS Ensure RSSI average is not updated for skipped arfcns in DR mode
13/10/14   nm        CR736379 Set the release_required correctly
29/09/14   sk        CR743686: Ensure Frames for the Cb activity are cleared when new scheduling/skip req received
28/10/14   zc        CR734183 Ensure that asdiv idle wakeup is only set in G idle mode
28/10/14   sjv       CR747201 Handle multiple calls for grm_reserve from task message queue (made from l1_fm_grm_reserve)
17/10/14   sk        CR743536 Ensure gap to CCCH is updated on rude wakeup before ISR runs
21/10/14   fan       CR734490 Rework the wake up recovery mechanism
15/10/14   pa        CR739469: Add band information in trm request for NCELL activities.
06/10/14   cah       CR733516: Missed PCH flag should not not be gating IRAT meas in DR mode.
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build
24/09/14   cah       CR729408: Updates to reduce number of TRM API calls in Gl1.
24/09/14   cah       CR729408: Updates to reduce number of TRM API calls in Gl1.
22/09/14   cah       CR723144: Bolt 2.0 DRDSDS - W+G - G2G interband ncell monitor activity denied while in W+G FC mode
12/09/14   fan       CR636420: Run Sleep commit after GSTMR ISR at wakeup
04/09/14   js        CR631839 Allowing BPLMN in DSDA by suspending TRM release by two frames
21/08/14   nk        CR712853 Avoid making reservation for CCCH even if theres a FM entry present when L1 is awaiting a TRM CALLBACK
29/08/14   ap        CR671108:During background PLMN search after G reads page it extends duration using page reason and not acquisition reason
29/08/14   ap        CR714059 Background PLMN search Acquisition collides with CCCH read
27/08/14   gk        CR664737 Reset GBTA_STATUS to NONE, during next l1_fm_trm_request; if idle sub is reading page during GBTA and the PT sub
                              releases the TBF
11/08/14   js        CR703909:QTA gap extend TRM lock for own page which is after NBCCH
11/07/14   miaof     CR689769 Correct the condition for TRM reservation in l1_multi_sim_trm_manager
22/08/14   nm       CR624617 set Reserve required correctly for next activity, if current activity clears frame in FM
24/06/14   nk        CR684761 Suspend TRM release when extending TRM for impending CCCH activity
05/08/14   nk        CR702494: Back out CR679299
14/08/14   cak       CR709296: SC activity clash with paging occasion does not clear TRM band reg
12/08/14   pa        CR708404: Keep awake till next tick after releasing TRM.
08/08/14   ka        CR706866 FR18739: DR-DSDS RxD Idle, part 2 - new TRM clients
06/08/14   ka        CR699964 Implement Idle RXD support in GL1
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
04/08/14   nm        CR662106 not releasing TRM and going to sleep during CB activity after receiving CBCH deact req
20/06/14   xz        CR679229 release TRM and deact normally if extend denied.
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
27/07/14   dv        CR700582 Ensure peending IFS/IRAT activity cleared if trm extension denied
22/07/14   cah       CR697586 trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
13/06/14   ka        CR675110:TRM is released in extended paging scenario
02/07/14   aga       CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
10/06/14   jj        CR 668965 merge ASDIV from DI.3.0 to BO 2.0
24/06/14   cs        CR684714 Add back in functionality removed for initial updated TRM API support
18/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS: Surround cell meas changes
21/05/14   ssh       CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub
08/05/14   sp        CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
08/05/14   nk        CR660831: GSM Layer 1 compares wrong enum values for internal TRM_DENIAL status. Change TRM_DENIAL enum to TRM_NOT_GRANTED to aoid any confusion going forward.
12/05/14   zc        CR660275: Do not set asdiv wakeup config unless granted TRM
06/05/14   sp        CR658177 Clear last reserved reasons from FM Data when an activity starts running.
25/04/14   ggu       CR652985 Incorrectly calculates the expiration of TRM booking and extend TRM continuously
24/04/14   mc        CR654156 Suspend TRM release must not be honored if TRM status has been cleared down to TRM_NOT_GRANTED. This is required because
                     we can no longer assume that we will be granted the same RF device on the next TRM request so need to de-init and re-init RF.
22/04/14   og        CR646704. Check the trm_status and the booking expiry frame number
                     in l1_fm_trm_extend_duration( )
17/01/14   xz        CR647598 extend TRM Lock in LTA is just applicable W/T TO G
16/04/14   sp        CR649249: Retain TRM lock if GBTA is active for page reorg/ext paging. Otherwise let grm_request() decide to acquire GBTA/normal TRM.
15/04/14   mko       CR639602 decrement suspend trm relese correctly
14/04/14   am        CR647631 Inhibit sleep if TRM is extended
09/04/14   cs        CR638933 Allow equal priority event to be booked when suspend is set
09/04/14   xz        CR645227 Make more accurancy for number of frames available to the current activity before the duration booked with TRM expires
09/04/14   sp        CR635810:Don't release TRM lock for access when the lock is granted after request_and_notify(), Correct/620698
08/04/14   ka        CR644062 If other tech has initialised QTA do not allow an LTA activity to run
04/04/14   dv        CR644062 Ensure FM check for PLMN search in SGLTE mode
15/03/14   xz        CR627175 Keep retain TRM lock if the activity is closer for LTA purpose
29/03/14   mc        CR639462 : GBTA : Ext BCCH is not read after reading BCCH
28/03/14   ssh       CR621230: Use extended warm up time in the event of G+G(+G) wake up collisions to avoid warm up ISR overrun
21/03/14   am        CR634992 Pass duration in sclk in trm_extend_duration()
12/03/14   smd       CR628843 Allow background PLMN during BG traffic in DSDA mode
12/03/14   ssh       CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
08/03/14   am        CR624825 If TRM is retained for TBF assignment then TRM release should happen on TBF release
25/0214    am        CR622100 CBCH should use high priority only in idle mode
25/02/14   sk        CR614910 Update G BPLMN handling inline with DSDA-OMRD implementation
21/02/14   sk/smd    CR620698:Don't release TRM lock for access when the lock is granted after request_and_notify()
20/02/14   ssh       CR613866: If TRM self-denied in the first QTA gap we should avoid running any [means all] activity
20/02/14   am        CR620509 Ensure CBCH read is not allowed during dedicated mode
09/01/14   zf        CR598637 DRX only updates TRM booking if DRX activity is matching with the TRM booking
24/01/14   mko       CR593859 Extend the TRM if SI read is in less than 11 frames after the paging in reselection state(HDR in BG Traffic)
31/10/13   sk        CR566283 Do not block non QTA TRM requests when W is in QTA supported mode
17/01/13   sk        CR600574 Ensure GBTA state is reset from Active if TRM released/reserved without the activity being run
11/02/13   sp        CR488596 To indicate CB Missing indication in dedicated mode too
27/01/14   ss        CR605762 Call abort_receive if stale boooking detected in frame manager
16/01/14   ka        CR600056 In l1_fm_trm_request() ensure qta gap is active to determine that its qta
06/01/14   sk        CR593161 Ensure CB reservations not made when other SUB is in Data
24/12/13   nm        CR589209 Dont re-request TRM inside GBTA for page reorg mode if already have the grant
19/12/13   js        CR581840 IDLE SUBs releasing TRM immediately after GRANT leading to unnecessary GTA Gaps
09/12/13   sk        CR586591 Ensure GBTA status is not reset when GBTA is active in TSTS mode
16/12/13   dv        CR569122 High missed page rate due to W ID search
06/12/13   nk        CR585368 Prevent trm manager state machine to run if called from sleep warmup cb
06/01/14   sp        CR596088 Retain GBTA Active State for Extended Paging.
06/01/14   sk        CR593161 Ensure CB reservations not made when other SUB is in Data
24/12/13   nm        CR589209 Dont re-request TRM inside GBTA for page reorg mode if already have the grant
19/12/13   js        CR581840 IDLE SUBs releasing TRM immediately after GRANT leading to unnecessary GTA Gaps
09/12/13   sk        CR586591 Ensure GBTA status is not reset when GBTA is active in TSTS mode
16/12/13   dv        CR569122 High missed page rate due to W ID search
06/12/13   nk        CR585368 Prevent trm manager state machine to run if called from sleep warmup cb
13/12/13   ap        CR588766 FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
10/12/13   ws        CR587586 Remove DUAL_SIM featurisation in API
04/12/13   aga       CR549390 G2W_TA IRAT Enable W ID search and reconf in case the other G sub is in data call
01/12/13   sk        CR583361 Backout the CR549390
22/11/13   nm        CR578915 Do not rely on trm_extn flag only to differentiate between QTA/LTA
19/11/13   ssh/ip    CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   js        CR572966:Reselections to the Ncell are still failing as SUB2 is not able to decode all the SIs leading to SYS_INFO_TIMER expiry
15/11/13   nm        CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
13/11/13   js        CR569507:Missed pages on sub2 when sub1 is in power scan mode
08/11/13   sk        CR572873 Ensure non QTA activities doesnt run when W is in QTA mode
31/10/13   sk        CR566283 Do not block non QTA TRM requests when W is in QTA supported mode
13/11/13   js        CR569507:Missed pages on sub2 when sub1 is in power scan mode
11/20/13   nk        CR 549654 : Preventing scheduling of bcch state machine when frame manager has aborted that activity
19/10/13   sk        CR563877 Ensure BCCH reads are given priority over PCH when doing reselection and consider early wakeup time into TRM reservation
21/10/13   sk        CR563878 Ensure Acq gets to run inside the GTA gap with GBTA framework
13/11/13   ap        CR562878 FR 17482: Page Blocking Management in Multi-SIM Single Radio Devices
04/11/13   ka        CR570733: FM_PRI_LTE_IDSEARCH case required in l1_fm_map_pri_to_trm
15/10/13   js        CR508118: Send missed block ind if wakeup for CCCH cancelled
20/09/13   js        CR522426 Multi SIM CBCH priority decision to be taken only when we have more than one sub active
05/10/13   gk        CR543637 Set trm_status = TRM_NULL after calling call_in_task_trm_release
04/10/13   gk        CR492165: Read Extended Page if the second block of the page is between 42-49 in 51 MF
20/09/12   nk        CR547421 Skipping reservation for any activity other than CCCH activity when BPLMN is active
02/10/13   js        CR545531:Partial QBTA for extended paging
01/10/13   sk        CR552259 CR sync from 1.0/1.1
27/09/13   ab        CR550563 TR 1.1 - GBTA - Make Pseudo TRM_GRANT NULL after activity over.
30/09/13   sk        CR551975 Ensure TRM release/reserve not triggered when trm suspension counter is non zero
27/09/13   cgc       CR523552 add support for FR 16939 L+DSDS LTA/QTA
25/09/13   sk        CR549801 Ensure QTA reservations not made when W is non QTA supported mode
24/09/13   gk        CR549082:Sync from 1.1 and 1.0 to 2.0
09/09/13   ab        CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
19/09/13   sk        CR547301 Ensure Ncell Acq is not reserved as GBTA compatible reason code
10/09/13   nm        CR537341 CBCH activity is using TRM_ACCESS instead of FM_PRI_DSDS_CBCH
29/08/13   us        CR519643 Avoid ncell acquisition or delay ncell scheduling when bcch schedule is within 48msec
15/08/13   sk        CR524039 GBTA bringup changes
05/08/13   sk        Ensure non QTA requests not made when W is in QTA supported mode
18/09/13   us        CR519643 Avoid ncell acquisition or delay ncell scheduling when bcch schedule is within 48msec
29/07/13   sk        CR519663 Partial QBTA bringup changes
24/06/13   cs        Major Triton TSTS Syncup
23/05/13   sai       CR489525: Block CBCH activity during WCDMA BG traffic.
04/04/13   ab        CR471628  DSDS-L1 should not suppress TRM reservation if TRM reservation is removed.
21/03/13   am        CR454872 Abort FM only if frame is currently used
28/03/13   ab        CR 467838 GL1 DSDS : When exiting IDLE for RACH or est DL  , TRM lock should not be released
16/05/13   am        CR462200 Do not supress TRM reserve for TRM_ABORTING
15/05/13   am        CR469031 Avoid unnecessary TRM release
17/05/13   zf        CR488883: Do not use QTA enabled TRM reason before W supports QTA.
02/05/13   zf        CR480969: Fix G+G DSDS camping
01/04/13   am        CR469877 - CR413664+CR428401+CR415240+CR415240
12/02/13   ap        CR450842 DSDS: PCH not being decode on SUB2 even though TRM is granted
06/02/13   cs        DSDS CR438977: merged support for CBCH in DSDS mode
31/08/12   mc        DSDS CR376315 Prevent new activities from running if a TRM release is pending due to L1 task latency.
19/01/13   mc        CR442753
18/01/13   mc        CR437838
09/01/13   cs        CR377479 Only move FEE state if FEE estimate started OK
20/12/12   rc        Back out CR417401 & CR-377479
13/12/12   cs        CR377479 Only move FEE state if FEE estimate started OK
12/12/12   mc        CR431063 Added FM_ACTION_CLEAR into dual sim trm manager
14/02/13   pjr       CR452272 added support to set multisim mode at activation time
26/11/12   zf        CR417719: Optimization of Ncell acquisition
29/10/12   ss        CR396846 : Process Read sys info request only if next BCCH read does not coincides with active G2X measurement
                     scheduled in frame manager
19/11/12   ip/zf     CR341843: Opt16: Re-use SCH timing for re-acquiring Ncell.
23/10/12   npt       CR413635 - Port sleep code for DIME modem
03/09/12   zf        CR341843: Opt16: Re-use SCH timing for re-acquiring Ncell.
20/06/11   cs        Add in CBCH for DSDS using individual priority
20/06/11   nt        DSDS CR261327 - Fix reservation of future SCE tasks
16/05/11   cs        DSDS CR282434 Reenable CBCH support on DSDS
11/03/11   nt        DSDS CR278173 - prevent ncell bookings into TRM during G2W BPLMN
02/03/11   nt        DSDS CR277336 - only allow page and BPLMN bookings during DSDS BPLMN
10/02/11   sjw       DSDS CR255895 - use timetick_get_safe() in ISR context
07/02/11   nt        Add back in removed code to keep TRM lock for FEE estimation.
08/12/10   cs        Fix the frame reservation for non DSDS targets
29/11/10   og        Adding G2L support.
19/11/10   nt        DSDS Enhancement to SCE scheduling during Tuneaway
23/05/08   ap        Klocwork fixes
05/09/07   og        Allow the max frames count to happen per fm table element,
                     resolves CR127568.
28/02/07   cs        Tidy up the FM logging
23/01/07   ws        CR 90771 - Fix frame manager issue with back to back CBCH and PCH
23/01/07   og        Resolve RVCT2.1 compiler warnings, merge from the
                     gpl1_hummer.10.00.43.01 branch.
10/18/05   sv        Add function to return gap to next ccch\sdcch block.
08/04/05   sv        Add logging when setting required FN.
05/23/05   sv        Remove function l1_fm_update_ccch_gap.
05/09/05   sv        Fix to correctly decrement frame counters (bug in previous checkin).
05/09/05   sv        Set "count_to_max_frames" to TRUE when default number of
                     frames have expired but the task needs more frames now.
04/11/05   sv        Fixed CBCH handling within Frame Manager. Added new function
                     that updates the CCCH gap in Frame Manager.
02/09/05   sv        Modified the code so that frames are not given to an activity
                     until all other activities are done aborting.
02/03/05   sv        Modify "l1_fm_get_available_frames" to return correct no of
                     available frames. Also, clear the Frame manager elements for
           a lower priority task when rejected for frames.
01/26/05   sv        Add support for Frame Manager.


===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "l1_fm.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1_log.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "l1_isr.h"
#include "gpl1_gprs_utils.h"
#include "l1_sc.h"
#include "gl1_hw_g.h"
#include "gl1_hw_sleep_ctl.h"

/*****************************************************

 LOCAL STORAGE

******************************************************/
#include "gpl1_dual_sim.h"
#include "l1_drx.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "l1_smscb.h"

#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#endif

#include "gl1_arbitrator_interface.h"

/* Define Absolute value */
#define  ABS( x ) ( ((x) < 0) ? -(x) : (x) )

#define NON_DRX_FRAME_DURATION 9


sys_proc_type_e_type reserve_sub_reason[NUM_GERAN_DATA_SPACES];
static boolean l1_fm_grm_request( l1_fm_priority_T pri, uint32 frames_required, gas_id_t gas_id );
static void l1_fm_grm_reserve_at( l1_fm_priority_T pri, uint32 frames_required, uint32 required_frame_nbr, gas_id_t gas_id );
void l1_fm_find_next_task_to_reserve_or_release( gas_id_t gas_id );

static boolean l1_fm_grm_extend_duration(uint32 frames_required, l1_fm_priority_T pri, gas_id_t gas_id);
static boolean l1_fm_suppress_grm_reserve_at(l1_fm_priority_T pri,trm_reason_enum_t reason, uint32 required_frame_nbr, uint32 frames_required, gas_id_t gas_id);
uint32 l1_fm_get_frames_until_trm_booking_expires(gas_id_t gas_id);
boolean l1_fm_check_reserved_trm_activity( gas_id_t gas_id, uint32 frames_required );

void l1_fm_set_suspend_trm_release( uint32 frames,
                                    l1_fm_priority_T suspend_release_event_pri,
                                    gas_id_t gas_id );

static boolean l1_fm_is_task_bookable_before_high_priority( l1_fm_priority_T current_task,
                                                            l1_fm_priority_T high_priority_task,
                                                            gas_id_t gas_id );
static boolean l1_fm_is_a_pri_to_apply_overhead( l1_fm_priority_T pri );
#else /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
static boolean l1_fm_pri_is_earlier_and_non_conflicting(l1_fm_priority_T pri_1, l1_fm_priority_T pri_2, gas_id_t gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
extern int64 rem_time_ms;
#endif 

static boolean l1_fm_table_element_copy( l1_fm_priority_T dest, l1_fm_priority_T source, gas_id_t gas_id);

extern void l1_clear_cbch_activity(gas_id_t gas_id);

l1_fm_data_t  l1_fm_data[NUM_GERAN_DATA_SPACES];

l1_fm_priority_T l1_fm_current_activity[NUM_GERAN_DATA_SPACES];

l1_fm_priority_T  cbch_active_pri[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FM_PRI_CBCH) };
l1_fm_priority_T  cbch_pending_pri[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FM_PRI_CBCH) };

extern void l1_idle_rebook_drx_tick(gas_id_t gas_id);

#if defined( FEATURE_G2T_IDLE_TUNEAWAY) ||defined( FEATURE_G2X_REORG_TUNEAWAY)
extern uint32 l1_get_reorg_sub_ccch_fn(gas_id_t gas_id);
extern boolean l1_is_idle_ta_prepared(gas_id_t gas_id);
extern void l1_add_ccch_drx_ref (uint32 FN, gas_id_t gas_id);
#endif

#ifdef FRAME_MANAGER_LOGGING
/*===========================================================================
FUNCTION l1_fm_log_event

DESCRIPTION
  Logging mechanism for the Frame Manager.

INPUTS
  Priority of the task, Frame Manager Event

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/

void l1_fm_log_event(l1_fm_priority_T pri, fm_event_T event, gas_id_t gas_id)
{
    l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
    l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];


    if (l1_nv_log_ptr->l1_fm_log_index >= L1_FM_LOG_LEN)
    {
      l1_nv_log_ptr->l1_fm_log_index = 0;
    }

    l1_nv_log_ptr->l1_fm_log_buf[l1_nv_log_ptr->l1_fm_log_index].FN             = gl1_get_FN( gas_id );
    l1_nv_log_ptr->l1_fm_log_buf[l1_nv_log_ptr->l1_fm_log_index].event          = event;
    l1_nv_log_ptr->l1_fm_log_buf[l1_nv_log_ptr->l1_fm_log_index].fm_task        = pri;
    l1_nv_log_ptr->l1_fm_log_buf[l1_nv_log_ptr->l1_fm_log_index].fm_state       = l1_fm_data_ptr->l1_fm_table[pri].state;
    l1_nv_log_ptr->l1_fm_log_buf[l1_nv_log_ptr->l1_fm_log_index].fm_frames_reqd = l1_fm_data_ptr->l1_fm_table[pri].required_frames;
    l1_nv_log_ptr->l1_fm_log_index++;
}

#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION l1_get_fm_trm_priority

DESCRIPTION
  Returns l1_fm_priority_T

DEPENDENCIES
  None.

SIDE EFFECTS
  None.

===========================================================================*/

l1_fm_priority_T l1_get_fm_trm_priority(gas_id_t gas_id )
{
   return l1_fm_data[gas_id].trm_reserved_pri;
}

/*===========================================================================

FUNCTION l1_fm_get_trm_duration

DESCRIPTION
  Returns duration for which trm was reserved (in num of frames)

DEPENDENCIES
  None.

SIDE EFFECTS
  None.

===========================================================================*/

uint32 l1_fm_get_trm_duration(gas_id_t gas_id )
{
   return l1_fm_data[gas_id].trm_reserved_duration;
}

/*===========================================================================

FUNCTION l1_fm_get_trm_reserved_fn

DESCRIPTION
Get the current TRM reserved FN

PARAMS

DEPENDENCIES

RETURN VALUE
  TRUE -- when the required frame is what TRM reserved at.
  FALSE -- otherwise.

===========================================================================*/
uint32 l1_fm_get_trm_reserved_fn( gas_id_t gas_id)
{
    return l1_fm_data[gas_id].trm_reserved_fn;
}

/*===========================================================================
FUNCTION l1_is_fm_trm_priority_g2t

DESCRIPTION
  Return TRUE if fm priority is G2T IRAT else FALSE

DEPENDENCIES
  None.

SIDE EFFECTS
  None.

===========================================================================*/

boolean l1_is_fm_trm_priority_g2t(gas_id_t gas_id )
{
#ifdef FEATURE_GSM_TDS

   if(l1_get_fm_trm_priority(gas_id) == FM_PRI_TDS_ACQ)
   {
     return TRUE;
   }
#endif
   return FALSE;
}

/*===========================================================================

FUNCTION l1_fm_map_pri_to_trm

DESCRIPTION
  Maps l1_fm_priority_T to a TRM reason code

DEPENDENCIES
  None.

SIDE EFFECTS
  None.

===========================================================================*/
trm_reason_enum_t l1_fm_map_pri_to_trm(l1_fm_priority_T pri, gas_id_t gas_id)
{
  trm_reason_enum_t reason = TRM_NUM_REASONS;

  switch(pri)
  {
    case FM_PRI_WCDMA_IDSEARCH:
    case FM_PRI_WCDMA_RECONF:
#ifdef FEATURE_GSM_TDS
    case FM_PRI_TDS_ACQ:
#endif /*FEATURE_GSM_TDS*/
#ifdef FEATURE_GSM_TO_LTE
      case FM_PRI_LTE_IDSEARCH:
#endif /*FEATURE_GSM_TO_LTE*/
      reason = TRM_IRAT_MEASUREMENT;
      reserve_sub_reason[gas_id] = SYS_PROC_TYPE_IRAT_MEASUREMENT;
      break;

    case FM_PRI_NCELL_ACQ:
    {
      reason = TRM_CHANNEL_MAINTENANCE;
      reserve_sub_reason[gas_id] = SYS_PROC_TYPE_ACQUISITION;
      break;
    }

    case FM_PRI_NCELL_SCH:
    {
      reason = TRM_CHANNEL_MAINTENANCE;
      reserve_sub_reason[gas_id] = SYS_PROC_TYPE_INTER_FCELL_MEASUREMENT;
     break;
    }

    case FM_PRI_NCELL_BCCH:
    {
      reserve_sub_reason[gas_id] = SYS_PROC_TYPE_SYSTEM_INFO_READING;
          reason = TRM_CHANNEL_MAINTENANCE;
        break;
    }

    case FM_PRI_CCCH:
      reason = TRM_DEMOD_PAGE;
      reserve_sub_reason[gas_id] = SYS_PROC_TYPE_PAGE;
      break;

    case FM_PRI_BACKGROUND_PLMN:
       switch (L1_get_current_background_HPLMN_search_activity(gas_id))
       {
          case POWER_SCAN:
            reason = TRM_ACQUISITION;
            break;
          case ACQ_BCCH_LIST:
            reason = TRM_ACQUISITION_CELL_SELECT;
            break;
          default:
            MSG_GERAN_ERROR_0_G("background_HPLMN_search_activity unknown");
            reason = TRM_ACQUISITION;
            break;
       }
      reserve_sub_reason[gas_id] = SYS_PROC_TYPE_BPLMN_SEARCH;
      break;

    case FM_PRI_CBCH:
      reason = TRM_CHANNEL_MAINTENANCE_INV;
      reserve_sub_reason[gas_id] = SYS_PROC_TYPE_IMS_SMS;
      break;

    case FM_PRI_MSIM_CBCH:
      reason = TRM_ACQUISITION;
      reserve_sub_reason[gas_id] = SYS_PROC_TYPE_IMS_SMS;
      break;

    case FM_PRI_RESERVED:
    default:
      MSG_GERAN_ERROR_1_G("Unexpected FM priority %d", pri);
      break;
  }

  /* Change the reason here if this is for a page read and there is an override reason */
  if(TRM_DEMOD_PAGE == reason)
  {
    reason = l1_get_global_override_trm_reason(gas_id);
  }

  if( ( pri == FM_PRI_NCELL_BCCH ) && l1_sc_bcch_use_high_priority(gas_id) )
  {
    reason = TRM_CHANNEL_MAINTENANCE_INV;
  }

  if ( pri == FM_PRI_BACKGROUND_PLMN)
  {
    reason = grm_get_acquisition_reason(reason, gas_id);
  }

  return reason;
}

/*===========================================================================

FUNCTION gl1_ms_fm_update_booking_frames

DESCRIPTION
        Only used for Dual SIM/Dual data space.  Updates the start times for
        the frame manager bookings. This will be as a result of
        the ASYNC sims offset frame number changing. So times booked into the
        DRX manager will not now correspond to the actual ASYNC frames unless
        this operation is performed.

PARAMS  frame_adjust - value to change the frame by

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void gl1_ms_fm_update_booking_frames( int32 frame_adjust, gas_id_t gas_id )
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
   uint32 i;

   for (i=0; i<FM_PRI_MAX; i++)               /*lint !e641 suppress "Converting enum to int"*/
   {
     if ( l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number != NOT_A_FRAME_NUMBER )
     {
       if (frame_adjust > 0)
       {
         l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number =
           ADD_FN(l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number, frame_adjust);
       }
       else
       {
         l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number =
           SUB_FN(l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number, (frame_adjust*-1));
       }
     }
   }
}

/*===========================================================================

FUNCTION l1_fm_check_reserved_trm_activity

DESCRIPTION
        Check if current frame is what TRM reserved at.

PARAMS  frames_required -- required frame number

DEPENDENCIES

RETURN VALUE
  TRUE -- when the required frame is what TRM reserved at.
  FALSE -- otherwise.

===========================================================================*/
boolean l1_fm_check_reserved_trm_activity( gas_id_t gas_id, uint32 frames_required )
{
    return (l1_fm_data[gas_id].trm_reserved_fn == frames_required);
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */


/*===========================================================================

FUNCTION l1_fm_init

DESCRIPTION
  Resets the Frame manager.  Should be called at init time and each time
  MS enters Idle Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  State of the Frame manager is reset.

===========================================================================*/

void l1_fm_init( gas_id_t gas_id )
{
   l1_fm_reset(gas_id);
}

/*===========================================================================

FUNCTION l1_fm_reset

DESCRIPTION
  Resets the Frame manager.  Should be called at init time and each time
  MS enters Idle Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  State of the Frame manager is reset.

===========================================================================*/

void l1_fm_reset( gas_id_t gas_id )
{
   l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
   uint32 i;

   for (i=0; i<FM_PRI_MAX; i++)                /*lint !e641 suppress "Converting enum to int"*/
   {
      l1_fm_data_ptr->l1_fm_table[i].state                 = FM_STATE_NULL;
      l1_fm_data_ptr->l1_fm_table[i].required_frames       = 0;
      l1_fm_data_ptr->l1_fm_table[i].max_required_frames   = 0;
      l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number = NOT_A_FRAME_NUMBER;
      l1_fm_data_ptr->l1_fm_table[i].abort_callback        = NULL;
      l1_fm_data_ptr->l1_fm_table[i].abort_data            = 0;

      l1_fm_data_ptr->count_to_max_frames[i]               = FALSE;
      l1_fm_data_ptr->leaving_idle_mode                    = FALSE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      l1_fm_data_ptr->l1_fm_table[i].action                = FM_ACTION_NONE;
      l1_fm_data_ptr->l1_fm_table[i].fw_load_frames        = 0;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   l1_fm_data_ptr->suspend_trm_release                     = 0;
   l1_fm_data_ptr->suspend_trm_release_pri                 = FM_PRI_RESERVED;
   l1_fm_data_ptr->is_irat_activity                        = FALSE;

   l1_fm_reset_pri_res(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}

void l1_fm_set_leaving_idle_mode(boolean leaving_idle_mode, gas_id_t gas_id )
{
  l1_fm_data[gas_id].leaving_idle_mode = leaving_idle_mode;
}

boolean l1_fm_leaving_idle_mode(gas_id_t gas_id )
{
  return l1_fm_data[gas_id].leaving_idle_mode;
}


/*===========================================================================

FUNCTION l1_fm_tick

DESCRIPTION
  Frame tick handler for the Frame manager.  It is called every frame in the
  GSM\GPRS Idle mode and SDCCH mode. It updates the "required_frames" element
  for every task (should be only 1) that is currently running. In case, the
  highest priority task - CCCH needs to run, this function will abort the
  currently running task.

INPUTS
  Gap to next CCCH activity

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void l1_fm_tick( uint32 gap_to_next_ccch_activity, gas_id_t gas_id )
{
   l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
   uint32 i;

   /* Work down table and abort any running (should
    * be only one) task that has run out of frames.
    */
   l1_fm_data_ptr->gap_to_ccch_activity = gap_to_next_ccch_activity;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if ( l1_fm_data_ptr->suspend_trm_release != 0 )
   {
     l1_fm_data_ptr->suspend_trm_release--;

     MSG_GERAN_MED_2_G( "Frame Manager:suspend_trm_release %d suspend_trm_release_pri %d",
                        l1_fm_data_ptr->suspend_trm_release, l1_fm_data_ptr->suspend_trm_release_pri );
   }

   /* no need to check for CCCH activity as this is always ok to run */
   for (i=FM_PRI_CCCH+1; i<FM_PRI_MAX; i++)        /*lint !e641 suppress "Converting enum to int"*/
#else
   for (i=0; i<FM_PRI_MAX; i++)                    /*lint !e641 suppress "Converting enum to int"*/
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   {
      if (l1_fm_data_ptr->l1_fm_table[i].state == FM_STATE_USING_FRAMES)
      {
         if((i == FM_PRI_NCELL_BCCH) && (l1_fm_data_ptr->leaving_idle_mode == FALSE) && l1_sc_bcch_use_high_priority(gas_id))
            {
              MSG_GERAN_MED_0_G("Not aborting  NBCCH read for CCCH");
            }
         else if((i == FM_PRI_NCELL_BCCH) && (l1_fm_data_ptr->leaving_idle_mode == TRUE))
         {
             MSG_GERAN_MED_0_G("Leave idle, do not abort NBCCH");
         }
         else
         /* @@OG: Fix to CR127568 */
         if( FALSE != l1_fm_data_ptr->count_to_max_frames[ i ] )
         {
            if (l1_fm_data_ptr->l1_fm_table[i].max_required_frames > l1_fm_data_ptr->gap_to_ccch_activity)
            {
              /* If we are currently scheduling CBCH then this is a special case
               * as CBCH and PCH are in adjacent blocks and so PCH and CBCH states
               * will be active at the same time since data call back from CBCH will be received when
               * PCH is being received
               */
              if ( i != l1_fm_get_active_CBCH_pri(gas_id) )
              {
                /* Not enough frames - time to abort. */
                MSG_GERAN_MED_3_G("Frame Manager:Aborting activity %d due to lack of frames(reqd frames:%d,CCCH gap:%d)..",
                        i, l1_fm_data_ptr->l1_fm_table[i].max_required_frames,
                        l1_fm_data_ptr->gap_to_ccch_activity);
                /* @@OG: Fix to CR127568 */
                l1_fm_data_ptr->count_to_max_frames[ i ] = FALSE;

                l1_fm_data_ptr->l1_fm_table[i].state = FM_STATE_CB_PENDING;
#ifdef FRAME_MANAGER_LOGGING
                l1_fm_log_event( (l1_fm_priority_T) i, EVE_ABORT_CURR_TASK, gas_id);
#endif
                l1_fm_data_ptr->l1_fm_table[i].abort_callback(l1_fm_data_ptr->l1_fm_table[i].abort_data, gas_id);
              }
            }

            if ( ( l1_fm_data_ptr->l1_fm_table[i].state == FM_STATE_USING_FRAMES )
                 && ( l1_fm_data_ptr->l1_fm_table[i].max_required_frames == 0 ) )
            {
               /* @@OG: Fix to CR127568 */
               l1_fm_data_ptr->count_to_max_frames[ i ] = FALSE;

               l1_fm_data_ptr->l1_fm_table[i].state = FM_STATE_CB_PENDING;
#ifdef FRAME_MANAGER_LOGGING
               l1_fm_log_event( (l1_fm_priority_T) i, EVE_ABORT_CURR_TASK, gas_id);
#endif
               MSG_GERAN_ERROR_2_G("Frame Manager:Activity %d did not complete within reqd no of frames(FN:%d)",i, gl1_get_FN( gas_id ));
               l1_fm_data_ptr->l1_fm_table[i].abort_callback(l1_fm_data_ptr->l1_fm_table[i].abort_data, gas_id);
            }
            if(l1_fm_data_ptr->l1_fm_table[i].max_required_frames > 0)
            {
              l1_fm_data_ptr->l1_fm_table[i].max_required_frames--;
            }
         }
         else
         {
           if (l1_fm_data_ptr->l1_fm_table[i].required_frames > l1_fm_data_ptr->gap_to_ccch_activity)
            {
               /* Not enough frames - time to abort. */
               MSG_GERAN_MED_3_G("Frame Manager:Aborting activity %d due to lack of frames(reqd frames:%d,CCCH gap:%d)..", i,
                                                                               l1_fm_data_ptr->l1_fm_table[i].required_frames,
                                                                                         l1_fm_data_ptr->gap_to_ccch_activity);
               l1_fm_data_ptr->l1_fm_table[i].state = FM_STATE_CB_PENDING;
#ifdef FRAME_MANAGER_LOGGING
               l1_fm_log_event( (l1_fm_priority_T) i, EVE_ABORT_CURR_TASK, gas_id);
#endif
               l1_fm_data_ptr->l1_fm_table[i].abort_callback(l1_fm_data_ptr->l1_fm_table[i].abort_data, gas_id);
            }

            if ( ( l1_fm_data_ptr->l1_fm_table[i].state == FM_STATE_USING_FRAMES )
                 && ( l1_fm_data_ptr->l1_fm_table[i].required_frames == 0 ) )
            {
              if (l1_fm_data_ptr->l1_fm_table[i].max_required_frames == 0)
              {
                 l1_fm_data_ptr->l1_fm_table[i].state = FM_STATE_CB_PENDING;
#ifdef FRAME_MANAGER_LOGGING
                 l1_fm_log_event( (l1_fm_priority_T) i, EVE_ABORT_CURR_TASK, gas_id);
#endif
                 MSG_GERAN_ERROR_2_G("Frame Manager:Activity %d did not complete within reqd no of frames(FN:%d)",i, gl1_get_FN( gas_id ));

                 if( NULL != l1_fm_data_ptr->l1_fm_table[i].abort_callback )
                 {
                   l1_fm_data_ptr->l1_fm_table[i].abort_callback(l1_fm_data_ptr->l1_fm_table[i].abort_data, gas_id);
                 }
                 else
                 {
                   MSG_GERAN_ERROR_2_G(" NULL abort callback for activity %d FN %d ", i, gl1_get_FN( gas_id ));
                 }

              }
              else
              {
                   /* @@OG: Fix to CR127568 */
                 l1_fm_data_ptr->count_to_max_frames[ i ] = TRUE;
              }
            }
            if( l1_fm_data_ptr->l1_fm_table[i].required_frames > 0 )
            {
              l1_fm_data_ptr->l1_fm_table[i].required_frames--;
            }
            if(l1_fm_data_ptr->l1_fm_table[i].max_required_frames > 0)
            {
              l1_fm_data_ptr->l1_fm_table[i].max_required_frames--;
            }
         }

#ifdef FRAME_MANAGER_DEBUG
         #error code not present
#endif

#ifdef FRAME_MANAGER_LOGGING
         l1_fm_log_event( (l1_fm_priority_T) i, EVE_FM_TICK, gas_id);
#endif
      }
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* FM tick has completed so reset the suspend release pri if set */
   if ( ( l1_fm_data_ptr->suspend_trm_release == 0 ) &&
        ( l1_fm_data_ptr->suspend_trm_release_pri != FM_PRI_RESERVED ) )
   {
     l1_fm_data_ptr->suspend_trm_release_pri = FM_PRI_RESERVED;
   }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION l1_drx_wakeup_cancel_for_CCCH

DESCRIPTION
  Check if wakeup cancellation is for CCCH decode

INPUTS
  Future wakeup frame number, GAS_ID

DEPENDENCIES
  None

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None.

===========================================================================*/

boolean l1_drx_wakeup_cancel_for_CCCH(uint32 reserved_frame_num,gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  int32 delta = 0;
  boolean ret_val = FALSE;

  if((l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number) != NOT_A_FRAME_NUMBER)
  {
    delta = reserved_frame_num - (l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number);

    if(ABS(delta) <= 4)
    {
      ret_val = TRUE;
    }

  }

  MSG_GERAN_HIGH_4_G("l1_drx_wakeup_cancel_for_CCCH: old_wakeup_point_fn:%d, CCCH_reserved_frame_num:%d new_wakeup_point_fn:%d ret_val:%d",
                     reserved_frame_num,
                     (l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number),
                     GSTMR_GET_FN_GERAN(gas_id),
                     ret_val
                     );

  return ret_val;
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION l1_drx_set_gap_to_CCCH_on_rude_wakeup

DESCRIPTION
  Set the gap to CCCH actiivity in rude wakeup

INPUTS
  GAS_ID

DEPENDENCIES
  None

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None.

===========================================================================*/

void l1_drx_set_gap_to_CCCH_on_rude_wakeup( gas_id_t gas_id )
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  uint32 current_fn            =  gl1_get_FN( gas_id );

  if( ( l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number != NOT_A_FRAME_NUMBER ) &&
        IS_FRAME_NUM_LATER(l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number,current_fn ))
   {
     l1_fm_data_ptr->gap_to_ccch_activity = SUB_FN(l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number,current_fn);
   }
}

/*===========================================================================

FUNCTION l1_fm_allowed_but_blocked_by_trm

DESCRIPTION
  This function tells if FM activity was allowed but blocked by TRM

INPUTS
  Priority of the task, Gas_id

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - if activity was denied by TRM
  FALSE - if activity was not denied by TRM

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_fm_allowed_but_blocked_by_trm( l1_fm_priority_T pri, gas_id_t gas_id )
{
  return ( l1_fm_data[gas_id].l1_fm_table[pri].allowed_but_trm_denial );
}

/*===========================================================================

FUNCTION l1_fm_are_frames_available

DESCRIPTION
  Determines if there are frames available for any task to complete its
  operation.

INPUTS
  Priority of the task, no of frames requested, abort handler for the task

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - if frames are available
  FALSE - if frames are unavailable

SIDE EFFECTS
  None.

===========================================================================*/

boolean l1_fm_are_frames_available( l1_fm_priority_T pri,
                                    uint32 frames_required,
                                    void ( *abort_callback )( void*, gas_id_t gas_id ),
                                    void* abort_data, gas_id_t gas_id )
{
   l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
   uint32 i;
   volatile ISRTIM_CMD_BLK *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
   idle_data_T *l1_idle_data=&l1_idle_data_store[gas_id];
   rfm_device_enum_type  l_rfm_device = RFM_INVALID_DEVICE;
   uint8 overhead = 0;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   boolean hp_cbch = FALSE;
   overhead = (garb_intf_gas_is_xpg_coex_victim(gas_id) && 
               l1_fm_is_a_pri_to_apply_overhead(pri)    &&
               !gl1_drdsds_enabled(gas_id)) ? QTA_OVERHEAD : 0; /* Minimum period between consecutive TA gaps */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */


   MSG_GERAN_HIGH_3_G("Activity %d requests %d frames at FN=%d",pri,frames_required,gl1_get_FN( gas_id ));
   l1_fm_current_activity[gas_id] = pri;

   l_rfm_device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id );

   MSG_GERAN_MED_3_G("mode: standby_mode %d multi_sim_mode %d, device id %d",
                      gl1_msg_get_multi_sim_standby_mode(),
                      gl1_msg_get_multi_sim_mode(),
                      l_rfm_device);

   /* if higher priority tasks are running, then no frames can
    * be provided.
    */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if(l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number == NOT_A_FRAME_NUMBER)
   {
      l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number = gl1_get_FN( gas_id );
   }
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* Add the time to load the RF and FW into the time required when in dual sim mode */
   if ( gl1_msg_get_multi_sim_mode() )
   {
      l1_fm_data_ptr->l1_fm_table[pri].allowed_but_trm_denial = FALSE;
       frames_required += l1_fm_data_ptr->l1_fm_table[pri].fw_load_frames;
     }

#ifdef FEATURE_NAS_CBS_DSDS
   /* check whether this request is for a high priority CBCH activity in DSDS mode */
   if ( ( FM_PRI_CBCH == pri ) && ( l1_fm_data_ptr->gap_to_ccch_activity > 0 )
        && ( l1_fm_data_ptr->gap_to_ccch_activity != NOT_A_FRAME_NUMBER )
      )
   {
     hp_cbch = TRUE;
   }
#endif /* FEATURE_NAS_CBS_DSDS */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   if(pri != FM_PRI_MAX)
   {
     /* Skip Check of CBCH Pri's when in Dedicated Mode */
     if ( ( (pri == FM_PRI_CBCH)
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            || ( pri == FM_PRI_MSIM_CBCH )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
          )
          && ( l1_tskisr_blk->l1_state== L1_DEDICATED_MODE || l1_tskisr_blk->l1_state == L1_TRANSFER_MODE
#ifdef FEATURE_GSM_DTM
              || l1_tskisr_blk->l1_state == L1_DTM_MODE
#endif
             )
        )
     {
       /* No CBCH booking as Dedicated mode is higher prio*/
       l1_fm_clear_required_frames(pri, gas_id);

       return FALSE;
     }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     if( l1_fm_prioritise_nbcch_over_ccch(gas_id) && ( pri == FM_PRI_CCCH ))
     {

        if(l1_sc_bcch_use_high_priority(gas_id))
        {
          MSG_GERAN_HIGH_0_G("Activity 1 (CCCH) blocked and NBCCH is prioritised");
        }
        l1_fm_clear_required_frames(pri, gas_id);

        return FALSE;
     }
#endif

     for (i=0; i<pri; i++)                      /*lint !e641 suppress "Converting enum to int"*/
     {
       if (l1_fm_data_ptr->l1_fm_table[i].state != FM_STATE_NULL)
       {
         MSG_GERAN_MED_3_G("Frame Manager:HP activity %d running!..No frames for activity %d(FN:%d)", i, pri, gl1_get_FN( gas_id ));
         l1_fm_clear_required_frames(pri, gas_id);

         return FALSE;
       }
     }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     if ( gl1_msg_get_multi_sim_mode() && L1_background_HPLMN_search_in_progress(gas_id) )
     {
       if ( pri != FM_PRI_CCCH )
       {
         MSG_GERAN_HIGH_1_G("Activity %d not allowed during BPLMN",(int)pri);
         l1_fm_clear_required_frames(pri, gas_id);

         return FALSE;
       }
     }

     if( ( (pri==FM_PRI_MSIM_CBCH) || (pri==FM_PRI_CBCH) ) &&
         ( OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) || (garb_intf_gas_is_xpg_coex_victim(gas_id)) ))
     {

        /*Block activity if OTHER_GSM_CLIENT_IN_TRANSFER
         */
        if ( gl1_msg_get_multi_sim_standby_mode() )
        {
          /* if here must be doing LTA  CBCH*/
          MSG_GERAN_HIGH_1_G("Activity %d not allowed when other client in transfer ", pri);

          if ( ( pri == FM_PRI_MSIM_CBCH ) || ( pri == FM_PRI_CBCH ) )
          {
            l1_clear_cbch_activity(gas_id);
          }
          else
          {
            l1_fm_clear_required_frames(pri, gas_id);
          }

          return FALSE;
        }
     }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     if (( pri == FM_PRI_MSIM_CBCH) && gl1_check_any_subs_l1_state( L1_WTOG_BPLMN_MODE))
     {
       MSG_GERAN_HIGH_0_G("Low Priority CB is not allowed during W2G PLMN search");
       l1_fm_clear_required_frames(pri, gas_id);
		 
       return FALSE;
     }	 
#endif

     if ( (pri != FM_PRI_CCCH) && (hp_cbch == FALSE) && ((frames_required + 1) > l1_fm_data_ptr->gap_to_ccch_activity))
#else
     if ( (frames_required + 1) > l1_fm_data_ptr->gap_to_ccch_activity)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
     {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        if( (l1_fm_data[gas_id].leaving_idle_mode || l1_idle_data->mm_non_drx_mode) ||
          !(( pri == FM_PRI_NCELL_BCCH ) && (l1_fm_prioritise_nbcch_over_ccch(gas_id)||(l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number == NOT_A_FRAME_NUMBER) )))
#else
        if( l1_fm_data[gas_id].leaving_idle_mode || l1_idle_data->mm_non_drx_mode)
#endif

       {
         l1_fm_clear_required_frames(pri, gas_id);

         return FALSE;
       }
     }

#if defined( FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
     /* Idle mode TA is opened when mm_non_drx mode is set,
      * block all activities other than CCCH
      */
     if (gl1_msg_get_multi_sim_mode() && l1_is_idle_ta_prepared(gas_id) )
     {
       MSG_GERAN_HIGH_1_G("Activity %d not allowed during IDLE TA",(int)pri);
       l1_fm_clear_required_frames(pri, gas_id);

       return FALSE;
     }
#endif /* (FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)*/

     {
       l1_fm_priority_T cbch_pri;

       /* determine the activity identifier for CBCH */
       cbch_pri = l1_fm_get_active_CBCH_pri( gas_id );

     /* Check whether the number of frames required will clash with the receive operation
      * of a higher priority task (scheduled for sometime in the near future). In that case,
      * it is advisable not to schedule the lower priority task in the first place. This will
      * save us the trouble of aborting this task anyway.
      */
       for (i=0; i<pri; i++)                   /*lint !e641 suppress "Converting enum to int"*/
       {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         if ( ( pri == FM_PRI_NCELL_BCCH ) && l1_fm_prioritise_nbcch_over_ccch(gas_id) )
         {
           
           if(l1_sc_bcch_use_high_priority(gas_id))
           {
             
             MSG_GERAN_HIGH_0_G("Prioritise Activity 3 (NBCCH) ahead of Activity 1 (CCCH) ");

           }
         }else
#endif
         if (l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number != NOT_A_FRAME_NUMBER)
         {
           if (IS_FRAME_NUM_LATER_OR_EQUAL(l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number, l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number))
           {
           /* Here the task is requesting for frames at a frame number that is higher than the FN at which a
            * higher priority is scheduled to run. Make sure that there is enough room for the high priority
            * task to complete; else do not schedule the low priority task
            */
             if ((SUB_FN(l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number,l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number)) < l1_fm_data_ptr->l1_fm_table[i].required_frames )
             {
               MSG_GERAN_MED_3_G("Frame Manager:HP activity %d will collide with activity %d!(FN:%d)",
                       i, pri, gl1_get_FN( gas_id ));

               l1_fm_clear_required_frames(pri, gas_id);

               return FALSE;
             }
           }
           else
           {
           /* Here the task is requesting for frames at a frame number that is lower than the FN at which a
            * higher priority is scheduled to run. Make sure that the low priority task will be able to complete
            * before the high priority  task can run; else do not schedule the low priority task
            */
             if ((SUB_FN(l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number,l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number)) < l1_fm_data_ptr->l1_fm_table[pri].required_frames + overhead)
             {
               /* Back to Back CBCH and PCH exception  */
               if ( (pri == cbch_pri) && (i == FM_PRI_CCCH) )
               {
#if ( defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM ) ) \
    && defined ( FEATURE_NAS_CBS_DSDS )
                   /* enable the "count to max frames" flag now, so that the CB activity runs to completion */
                   l1_fm_data_ptr->count_to_max_frames[pri] = TRUE;
#endif /* FEATURE_DUAL_SIM && FEATURE_NAS_CBS_DSDS */

                 MSG_GERAN_HIGH_3_G("CBCH exception needs %d frames for activity %d [FN %d]",
                          (int)frames_required, (int)pri, gl1_get_FN( gas_id ));
               }else
               {
                 MSG_GERAN_MED_3_G("Frame Manager:HP activity %d will collide with activity %d!(FN:%d)",
                         i, pri, gl1_get_FN( gas_id ));

                 l1_fm_clear_required_frames(pri, gas_id);
                 return FALSE;
               }
             }
           }
         }
       }
      /* Work down table and abort any running lower priorty task.
       *
       */
       for (i=pri+1; i<FM_PRI_MAX; i++)                /*lint !e641 suppress "Converting enum to int"*/
       {
         if (l1_fm_data_ptr->l1_fm_table[i].state == FM_STATE_USING_FRAMES)
         {
#if ( defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM ) ) \
    && defined ( FEATURE_NAS_CBS_DSDS )
           if ((pri == FM_PRI_CCCH) &&
               (i   == cbch_pri)    &&
               (l1_fm_data_ptr->count_to_max_frames[cbch_pri]))
           {
             /* CCCH activity is requesting frames, but back to back CBCH is still running */
             MSG_GERAN_MED_3_G("Frame Manager:Allowing CBCH exception to complete", 0, 0, 0);
             break;
           }
#endif /* ( FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM ) & FEATURE_NAS_CBS_DSDS */

           MSG_MED("Frame Manager:LP activity %d already running - reschedule HP activity !(FN:%d,CCCH gap:%d)",
                   i, gl1_get_FN( gas_id ), l1_fm_data_ptr->gap_to_ccch_activity);
             return FALSE;
           }
         else if (l1_fm_data_ptr->l1_fm_table[i].state == FM_STATE_CB_PENDING)
           {
             return FALSE;
           }
         }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       if(l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)
       {
          MSG_GERAN_MED_2_G("Frame Manager:Activity %d allowed but TRM release is pending FN=%d",
                 pri, gl1_get_FN(gas_id));

          l1_fm_clear_required_frames(pri, gas_id);
          return FALSE;
       }
       else
       /* Check that we are not still suspending if in multi sim mode for lower priority event
        * or if FM_PRI_RESERVED then always allow rebooking to go through as we just keep trm
        */
       if ( gl1_msg_get_multi_sim_mode() &&
            ( ( l1_fm_data_ptr->suspend_trm_release != 0 ) &&
              ( l1_fm_data_ptr->suspend_trm_release_pri != FM_PRI_RESERVED ) &&
              ( l1_fm_data_ptr->suspend_trm_release_pri != pri ) &&
              (!( (l1_tskisr_blk->reservation_for_bcch == FALSE) && (pri == FM_PRI_NCELL_BCCH) && (l1_fm_data_ptr->suspend_trm_release_pri == FM_PRI_CCCH) )) ))
       {
         MSG_GERAN_MED_2_G( "Frame Manager:Activity %d allowed but suspend_trm_release set! FN:%d",
                            pri, gl1_get_FN( gas_id ) );

         l1_fm_clear_required_frames(pri, gas_id);

         return FALSE;
       }
       /* For TA gaps do not permit any activity to run other than that which was reserved with TRM. This is to help prevent
        * any low priority tasks from sneeking in and hijacking a TA gap that was intended for a higher priority task. This
        * is only enforced for the first activity in the QTA gap (i.e. when TRM has not yet been granted) to allow futher activities
        * to run as part of a QTA gap extension */
#ifdef FEATURE_QTA
       else
       if( gl1_hw_qta_gap_active(gas_id) && (l1_fm_data_ptr->trm_reserved_pri != pri) && (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_GRANTED) )
       {
          MSG_GERAN_HIGH_2_G( "Frame Manager:Activity %d denied as QTA gap open for activity %d", pri, l1_fm_data_ptr->trm_reserved_pri);
          return FALSE;
       }
#endif /* FEATURE_QTA */
       else
       /* Check that TRM will allow this operation too if in dual sim mode */
       if ( gl1_msg_get_multi_sim_mode() && (l1_fm_grm_request( pri, frames_required, gas_id ) == FALSE) )
       {
         gas_id_t other_g_gas_id = OTHER_G_SUB_IN_TRANSFER(gas_id);

         MSG_GERAN_MED_2_G("Frame Manager:Activity %d allowed but blocked by TRM!FN:%d",
                 pri, gl1_get_FN( gas_id ));

         l1_fm_clear_required_frames(pri, gas_id);

         l1_fm_data_ptr->l1_fm_table[pri].allowed_but_trm_denial = TRUE;

         /* As TRM has been asked for the lock using a trm_request() this is an implied release, and will trash any
         future bookings. Ensure that any previous bookings get re-reserved back into TRM */
         l1_fm_data_ptr->l1_fm_table[FM_PRI_RESERVED].action = FM_ACTION_SET;

         /*Check to see if we were going to do a page.*/
         if ( ( pri == FM_PRI_CCCH)
              && gl1_msg_get_multi_sim_standby_mode()
              && !gl1_drdsds_enabled(gas_id)
            )
         {
           /* Set missed page so that PTM sub block scheduling of IRAT and suspend TBF for page decode on idle sub.
            * This should be checked only if GBTA is allowed from upper layers
            */
           if ( ( other_g_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED )
                && gl1_is_geran_tuneaway_allowed( other_g_gas_id )
              )
           {
             l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(other_g_gas_id);

             /*check to see if there is a IRAT search ongoing on other sub.*/
             if (l1_sc_globals_ptr->wcdma_srch_in_progress )
             {
               SET_MISSED_PAGE_STATUS(TRUE,gas_id);
               MSG_GERAN_MED_1_G("Missed page due to ID search on gas_id %d - Status TRUE ",other_g_gas_id);
             }
           }
         }
         return FALSE;
       }
       else
       {


         /*in idle mode all surround cell activities on non-serving cell bands must be reported to TRM when DR-DSDS is enabled
                TRM may deny the requested frequency bands in which case the activity can't run and booking in FM must be removed
                */
          if (( l1_tskisr_blk->l1_state == L1_IDLE_MODE ) ||
              ( l1_tskisr_blk->l1_state == L1_GPRS_IDLE_MODE ) ||
              ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE )
          )
          {
            if(!grm_check_idle_sc_activity_ok(pri,
                                              frames_required,
                                              l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number,
                                              gas_id) )
           {
                 l1_fm_clear_required_frames(pri, gas_id);
                 MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR"Activity %d denied by TRM, removing booking from L1FM", pri);
                 return FALSE;
           }
         }

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        if( (pri == FM_PRI_CCCH) && l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr)
        {
          gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_PAGE_WUP);
        }

        if( (pri == FM_PRI_CCCH) && l1_tskisr_blk->reservation_for_bcch && gl1_get_trm_global_overide_status(gas_id))
        {
          gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_EARLY_CAMP_BCCH_DECODE);
        }

        if( (pri == FM_PRI_WCDMA_IDSEARCH)
#ifdef FEATURE_GSM_TDS

        ||(pri == FM_PRI_TDS_ACQ )
#endif
|| (pri == FM_PRI_LTE_IDSEARCH))
        {
          gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_IRAT_MEAS_GAP);
        }
#endif

         MSG_GERAN_MED_3_G("Frame Manager:Activity %d will run now!(FN:%d,CCCH gap:%d)",
                              pri, gl1_get_FN( gas_id ), l1_fm_data_ptr->gap_to_ccch_activity);

         l1_fm_data_ptr->l1_fm_table[pri].state                 = FM_STATE_USING_FRAMES;
         l1_fm_data_ptr->l1_fm_table[pri].required_frames       = frames_required;
         l1_fm_data_ptr->l1_fm_table[pri].max_required_frames   = frames_required;
         l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number = NOT_A_FRAME_NUMBER;
         l1_fm_data_ptr->l1_fm_table[pri].abort_callback        = abort_callback;
         l1_fm_data_ptr->l1_fm_table[pri].abort_data            = abort_data;
         l1_fm_data_ptr->fm_active_pri = pri;

         /* RF device allocated for IRAT may not support GSM. The is_irat_activity flag is to prevent
          * FM to extend TRM lock acquired by an IRAT activity.
          */
         if(pri == FM_PRI_WCDMA_RECONF
            || pri == FM_PRI_WCDMA_IDSEARCH
#ifdef  FEATURE_GSM_TO_LTE
            || pri == FM_PRI_LTE_IDSEARCH
#endif
#ifdef FEATURE_GSM_TDS
            || pri == FM_PRI_TDS_ACQ
#endif
         )
         {
           l1_fm_data_ptr->is_irat_activity = TRUE;
         }
         else
         {
           l1_fm_data_ptr->is_irat_activity = FALSE;
         }

#ifdef FRAME_MANAGER_LOGGING
         l1_fm_log_event(pri, EVE_START_TASK, gas_id);
#endif
         /*Clear last reservation info here*/
         l1_fm_data_ptr->trm_reserved_reason = TRM_NUM_REASONS;
         l1_fm_data_ptr->trm_reserved_fn = NOT_A_FRAME_NUMBER;
         l1_fm_data_ptr->trm_reserved_pri = FM_PRI_MAX;

         /* A new TRM activity has been successfully booked so clear the resource lock exit state */
         if (GRM_RESOURCE_LOCK_GOING_INACTIVE == grm_get_resource_lock_enum(gas_id))
         {
         /* This could have been in any state when resources were locked but here 
              it's definitely granted so restore the known state to avoid disruption 
              to TRM release caused by GL1_TRM_RETAINED_FOR_ACCESS */
           grm_set_trm_status (GL1_TRM_GRANTED,l1_tskisr_blk->client_id,gas_id);
         }

#ifdef FEATURE_GSM_QSC_TX_DIV
         if((
#ifdef FEATURE_DUAL_SIM
     (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE) ||
#endif
     (l1_tskisr_blk->l1_state == L1_IDLE_MODE))&&
     (l1_fm_data_ptr->is_irat_activity == FALSE))
     {
      gl1_hw_set_asdiv_mode_config(gas_id, TRM_CLIENT_MODE_IDLE_WAKEUP);
     }
#endif /* FEATURE_GSM_QSC_TX_DIV */

#ifdef FEATURE_WLAN_COEX_SW_CXM
     /* inform COEX (WLAN) that GL1 is doing a high priority activity.
     *  Add 1 frame in the start as actual RX will happen in the next frame
     * currently making only ACQ and remaining all as PAGE high prio activity
     */
     if( pri == FM_PRI_NCELL_ACQ )
     {
       garb_cxm_high_priority_event_ind(ADD_FN(GSTMR_GET_FN_GERAN(gas_id),1),frames_required,CXM_HIGH_PRIO_ACQ,gas_id);
     }
     else
     {
       garb_cxm_high_priority_event_ind(ADD_FN(GSTMR_GET_FN_GERAN(gas_id),1),frames_required,CXM_HIGH_PRIO_PAGE,gas_id);
     }
#endif /* FEATURE_WLAN_COEX_SW_CXM */

         return TRUE;
       }
#else
       MSG_GERAN_MED_3_G("Frame Manager:Activity %d will run now!(FN:%d,CCCH gap:%d)",
                            pri, gl1_get_FN( gas_id ), l1_fm_data_ptr->gap_to_ccch_activity);

       l1_fm_data_ptr->l1_fm_table[pri].state                 = FM_STATE_USING_FRAMES;
       l1_fm_data_ptr->l1_fm_table[pri].required_frames       = frames_required;
       l1_fm_data_ptr->l1_fm_table[pri].max_required_frames   = frames_required;
       l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number = NOT_A_FRAME_NUMBER;
       l1_fm_data_ptr->l1_fm_table[pri].abort_callback        = abort_callback;
       l1_fm_data_ptr->l1_fm_table[pri].abort_data            = abort_data;

#ifdef FRAME_MANAGER_LOGGING
       l1_fm_log_event(pri, EVE_START_TASK, gas_id);
#endif

       return TRUE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
     }
   }
   else
   {
     /* For Klocwork */
     MSG_GERAN_ERROR_1_G("l1_fm_table[OOB] index fail %d", pri);

     return FALSE;
   }
}

/*===========================================================================

FUNCTION l1_fm_set_required_frame_number

DESCRIPTION
  Sets the frame number at which a task requires a frame.

INPUTS
  Priority of the task, frame number at which frames are required.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void l1_fm_set_required_frame_number(l1_fm_priority_T pri, uint32 frame_no, uint32 frames_reqd, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
   MSG_GERAN_HIGH_3_G("Activity %d requires %d frames at FN=%d",pri,frames_reqd,frame_no);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* Add the time to load the RF and FW into the time required when in dual sim mode */
  if ( gl1_msg_get_multi_sim_mode() )
  {
    /*CgC generic LTA/QTA startup is now the same time for both */
    frames_reqd += MULTI_SIM_FW_LOAD_FRAMES;
    l1_fm_data_ptr->l1_fm_table[pri].fw_load_frames = MULTI_SIM_FW_LOAD_FRAMES;
    /*
    If the activity being set is currently running then the action is set for
    a combined clear and set frames.
    */
    if(l1_fm_data_ptr->l1_fm_table[pri].state==FM_STATE_USING_FRAMES)
    {
      l1_fm_data_ptr->l1_fm_table[pri].action = FM_ACTION_CLEAR_AND_SET;
    }
    else
    /*
    The action event needs to be setup for the trm lock manager to process
    ensure that if the action was to clear an active task that this is still
    performed with the FM_ACTION_CLEAR_ACTIVE_TASK. The set action will be
    performed after this completed.
    */
    if ( ( l1_fm_data_ptr->l1_fm_table[pri].action != FM_ACTION_CLEAR_ACTIVE_TASK ) &&
         ( l1_fm_data_ptr->l1_fm_table[pri].action != FM_ACTION_DELAYED_RELEASE_FOR_TUNEAWAY )&&
         ( l1_fm_data_ptr->l1_fm_table[pri].action != FM_ACTION_DELAYED_RELEASE_FOR_BPLMN )
       )
    {
      l1_fm_data_ptr->l1_fm_table[pri].action = FM_ACTION_SET;
    }
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number = frame_no;
  l1_fm_data_ptr->l1_fm_table[pri].required_frames       = frames_reqd;

  /* ustmr_s_time */
  if(!gl1_hw_sleep_wmgr_cancel_in_prog[gas_id])
  {
    /* normal case
     * start time = vstmr tick + 1 TS
     */
  l1_fm_data_ptr->l1_fm_table[pri].ustmr_s_time = USTMR_ADD( gstmr_get_ota_ustmr_start_time(gas_id), SUB_FN(frame_no, gl1_get_FN(gas_id) ) * USTMR_PER_FRAME);
  }
  else
  {
    /* wkup mgr cancellation case
     * l1_fm_set_required_frame_number() called from gl1_hw_wakeup_conflict_rebook(),
     * so, we can program the ustmr_s_time by adding gap (gap is generally just a drx cycle)
     * (gap = new_wkpt - prev_wkpt)
     */
    l1_fm_data_ptr->l1_fm_table[pri].ustmr_s_time = USTMR_ADD( l1_fm_data_ptr->l1_fm_table[pri].ustmr_s_time, SUB_FN(frame_no, gl1_hw_wkupmgr_wakeup_point_fn[gas_id]) * USTMR_PER_FRAME);
  }

  /* ustmr_e_time */
  l1_fm_data_ptr->l1_fm_table[pri].ustmr_e_time = USTMR_ADD( l1_fm_data_ptr->l1_fm_table[pri].ustmr_s_time, USTMR_PER_FRAME * frames_reqd);

#ifdef FRAME_MANAGER_LOGGING
    l1_fm_log_event(pri, EVE_SET_REQD_FRAMES, gas_id);
#endif
}

/*===========================================================================

FUNCTION l1_fm_clear_required_frames

DESCRIPTION
  Clears the frame required for any task. Should be called at the end of any
  receive operation.

INPUTS
  Task Priority

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void l1_fm_clear_required_frames(l1_fm_priority_T pri, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* The action event needs to be setup for the trm lock manager to process. If the process
  was running set it up so that the task is release with the lock and FW. */
  if ( l1_fm_data_ptr->l1_fm_table[pri].state != FM_STATE_NULL )
  {
    l1_fm_data_ptr->l1_fm_table[pri].action = FM_ACTION_CLEAR_ACTIVE_TASK;
  }
  else
  /* If it was already setup for FM_ACTION_CLEAR_ACTIVE_TASK - DON'T CHANGE IT! the release
  still need to occur even if there have been two calls to clear out for some reason */
  if ( l1_fm_data_ptr->l1_fm_table[pri].action != FM_ACTION_CLEAR_ACTIVE_TASK )
  {
    l1_fm_data_ptr->l1_fm_table[pri].action = FM_ACTION_CLEAR;
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  l1_fm_data_ptr->l1_fm_table[pri].required_frames       = 0;
  l1_fm_data_ptr->l1_fm_table[pri].max_required_frames   = 0;
  l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number = NOT_A_FRAME_NUMBER;
  l1_fm_data_ptr->l1_fm_table[pri].state                 = FM_STATE_NULL;

  /* @@OG: Fix to CR127568 */
  l1_fm_data_ptr->count_to_max_frames[ pri ] = FALSE;

#ifdef FRAME_MANAGER_LOGGING
  l1_fm_log_event(pri, EVE_CLEAR_FRAMES, gas_id);
#endif
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION l1_fm_clear_required_frames_after_pch_decode

DESCRIPTION
  Clears the frames required for the actual receive of a page.
  receive operation.

INPUTS
  Task Priority

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void l1_fm_clear_required_frames_after_pch_decode(l1_fm_priority_T pri, gas_id_t gas_id)
{
#ifdef FEATURE_QTA
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_QTA */
  l1_fm_data_t            *l1_fm_data_ptr = &l1_fm_data[gas_id];

  /* The action event needs to be setup for the trm lock manager to process. If the process
  was running set it up so that the task is release with the lock and FW. */
  if( (l1_fm_data_ptr->l1_fm_table[pri].state != FM_STATE_NULL) && L1_background_HPLMN_search_in_progress(gas_id) && ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ) )
  {
    l1_fm_data_ptr->l1_fm_table[pri].action = FM_ACTION_DELAYED_RELEASE_FOR_BPLMN;
  }
  else
#ifdef FEATURE_GSM_POWER_OPTION16
  if (( l1_fm_data_ptr->l1_fm_table[pri].state != FM_STATE_NULL ) && l1_sc_is_ncell_acq_pending( gas_id ) && (!l1_tskisr_blk->qta_in_progress)
#ifdef FEATURE_GPRS_GBTA
  && !(OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
#endif
  && !L1_background_HPLMN_search_in_progress(gas_id)
  )
  {
    l1_fm_data_ptr->l1_fm_table[pri].action = FM_ACTION_DELAYED_RELEASE_FOR_TUNEAWAY;
  }
  else
#endif /* FEATURE_GSM_POWER_OPTION16 */
  if ( l1_fm_data_ptr->l1_fm_table[pri].state != FM_STATE_NULL )
  {
    l1_fm_data_ptr->l1_fm_table[pri].action = FM_ACTION_CLEAR_ACTIVE_TASK;
  }
  else
  /* If it was already setup for FM_ACTION_CLEAR_ACTIVE_TASK - DON'T CHANGE IT! the release
  still need to occur even if there have been two calls to clear out for some reason */
  if ( ( l1_fm_data_ptr->l1_fm_table[pri].action != FM_ACTION_CLEAR_ACTIVE_TASK ) &&
       ( l1_fm_data_ptr->l1_fm_table[pri].action != FM_ACTION_DELAYED_RELEASE_FOR_TUNEAWAY )
     )
  {
    l1_fm_data_ptr->l1_fm_table[pri].action = FM_ACTION_CLEAR;
  }

  l1_fm_data_ptr->l1_fm_table[pri].required_frames       = 0;
  l1_fm_data_ptr->l1_fm_table[pri].max_required_frames   = 0;
  l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number = NOT_A_FRAME_NUMBER;
  l1_fm_data_ptr->l1_fm_table[pri].state                 = FM_STATE_NULL;

    /* @@OG: Fix to CR127568 */
  l1_fm_data_ptr->count_to_max_frames[ pri ]             = FALSE;

#ifdef FRAME_MANAGER_LOGGING
    l1_fm_log_event(pri, EVE_CLEAR_FRAMES, gas_id);
#endif
}

/*===========================================================================

FUNCTION l1_fm_set_suspend_trm_release

DESCRIPTION
  This function programs a frame duration that must elapse before a job can
  be released, and the firmware deactivated.

INPUTS
  Task Priority

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void l1_fm_set_suspend_trm_release( uint32 frames,
                                    l1_fm_priority_T suspend_release_event_pri,
                                    gas_id_t gas_id )
{
  l1_fm_data_t *l1_fm_data_ptr            = &l1_fm_data[gas_id];

  l1_fm_data_ptr->suspend_trm_release     = frames;

  l1_fm_data_ptr->suspend_trm_release_pri = suspend_release_event_pri;
}

/*===========================================================================

FUNCTION l1_fm_get_suspend_trm_release

DESCRIPTION
  Returns the most recent value of the suspend duration

INPUTS
  GAS ID

RETURN VALUE
  Number of frames programmed

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_fm_get_suspend_trm_release(gas_id_t gas_id)
{
  return(l1_fm_data[gas_id].suspend_trm_release);
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION l1_fm_get_fm_table_state

DESCRIPTION
  Returns the state

INPUTS
  GAS ID, FM_PRIORITY

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
l1_fm_state_T l1_fm_get_fm_table_state( gas_id_t gas_id,  l1_fm_priority_T pri)
{
  return(l1_fm_data[gas_id].l1_fm_table[pri].state);
}

/*===========================================================================

FUNCTION l1_fm_get_available_frames

DESCRIPTION
  Returns the minimum number of frames available to a particular activity (ie.starting
  from the current frame upto the frame number at which a higher priority neighbor cell
  activity or CCCH block reception is scheduled.)

INPUTS
  Task Priority, current frame number

RETURN VALUE
  Number of frames available

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_fm_get_available_frames(l1_fm_priority_T pri, uint32 current_frame_number, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
   uint32 i, temp;
   uint32 no_available_frames = MAX_GAP;

   for(i=0;i<pri;i++)        /*lint !e641 suppress "Converting enum to int"*/
   {
      if((l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number != NOT_A_FRAME_NUMBER)) /*lint !e641 suppress "Converting enum to int"*/
      {
         if (IS_FRAME_NUM_LATER(current_frame_number, l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number))
         {
            temp = l1_fm_data_ptr->gap_to_ccch_activity -1;
            l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number = NOT_A_FRAME_NUMBER;
         }
         else
         {
            temp = SUB_FN(l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number, current_frame_number)-1;
         }
         no_available_frames = MIN(no_available_frames, temp);
      }
   }

#ifdef FRAME_MANAGER_LOGGING
   l1_fm_log_event(pri, EVE_GET_AVAIL_FRAMES, gas_id);
#endif
   return (no_available_frames);
}


/*===========================================================================

FUNCTION l1_fm_extend_required_frames

DESCRIPTION
  Sets the maximum number of frames that may be required by a task.

INPUTS
  Priority of the task, number of frames required.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void l1_fm_extend_required_frames(l1_fm_priority_T pri, uint32 frames_required, gas_id_t gas_id)
{
   l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
   l1_fm_data_ptr->l1_fm_table[pri].max_required_frames = frames_required;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if ( pri == FM_PRI_BACKGROUND_PLMN )
   {
      l1_fm_data_ptr->l1_fm_table[pri].state                 = FM_STATE_USING_FRAMES;
      l1_fm_data_ptr->l1_fm_table[pri].required_frames   = frames_required;
      l1_fm_data_ptr->l1_fm_table[pri].max_required_frames   = frames_required;
      l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number = NOT_A_FRAME_NUMBER;
      l1_fm_data_ptr->l1_fm_table[pri].abort_callback    = l1_multi_sim_bplmn_abort;
      l1_fm_data_ptr->l1_fm_table[pri].abort_data        = 0;
   }
#endif
}

/*===========================================================================

FUNCTION l1_fm_get_ccch_gap

DESCRIPTION
  Returns the gap to next CCCH\SDCCH block

INPUTS
  None

RETURN VALUE
  Number of frames to next CCCH\SDCCH block.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_fm_get_ccch_gap( gas_id_t gas_id )
{
   l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
   return l1_fm_data_ptr->gap_to_ccch_activity;
}


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION l1_fm_check_sch_activity_next

DESCRIPTION
  checks if next activity is SCH
INPUTS


DEPENDENCIES
  None

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_fm_check_sch_activity_next(gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];


  if ( l1_fm_data_ptr->trm_reserved_pri == FM_PRI_NCELL_SCH)
    return TRUE;
  else
   return FALSE;
} /* l1_fm_check_sch_activity_next */
#endif  /* FEATURE_GPRS_GBTA */

/*===========================================================================

FUNCTION l1_fm_grm_request

DESCRIPTION
  Sets the frame number at which a task requires a frame.

INPUTS
  Priority of the task, and the number of frames required to perform the task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static boolean l1_fm_grm_request( l1_fm_priority_T pri, uint32 frames_required, gas_id_t gas_id )
{
  trm_duration_t             duration;
  trm_reason_enum_t          reason;
  trm_grant_return_enum_type grm_grant_status;
  trm_resource_enum_t        resource;
  grm_client_enum_t          client_id;
  grm_divrx_request_type     req_type = GRM_DIVRX_TRM_REQ_INVALID;
  boolean                    band_reg_fail = FALSE;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  sys_band_class_e_type  trm_band = SYS_BAND_CLASS_NONE ;
#endif

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gl1_trm_state_t trm_status = GL1_TRM_NULL;

#ifdef FEATURE_QTA
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
#endif
  uint8 missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);
  ARFCN_T serving_cell;

  client_id = l1_tskisr_blk->client_id;

  /* Get the TRM reason here for this activity */
  reason = l1_fm_map_pri_to_trm(pri, gas_id);

  serving_cell = gl1_get_serving_cell_arfcn(gas_id);
#ifdef FEATURE_QTA
  /*
  For QTA we must only request the TRM reason for which we have previously reserved i.e
  despite QTA currently being supported our pending reservation may still be for an LTA
  activity if WCDMA transitioned into QTA support mode after we had made our reservation */
  if(gl1_hw_qta_gap_active(gas_id)  && (l1_fm_data_ptr->trm_reserved_pri != FM_PRI_RESERVED) )
  {
    if ( !IS_QTA_REASON(reason, reserve_sub_reason[gas_id]) )
    {
      /*
       * The other tech has initialised the QTA gap but we are attempting to run an activity which is
       * not a QTA reason. This can only end badly - so immediately return FALSE -
       * don't allow this activity to run.
       * This typically happens when we have booked for some QTA activity which is a comparatively
       * low priority activity e.g. FM_PRI_NCELL_ACQ but just before this the other tech releases
       * TRM and the higher non QTA activity gets in here (in this case FM_PRI_BACKGROUND_PLMN)
       */
      MSG_GERAN_HIGH_2_G("LTA in QTA Act:%i, res_reason:%i",pri,l1_fm_data_ptr->trm_reserved_reason);
      return FALSE;
    }

    else if((gl1_hw_get_qta_gap_for_g2x_irat(gas_id)) &&  (reason != TRM_IRAT_MEASUREMENT))
    {
      MSG_GERAN_HIGH_1_G("Non IRAT activity %d not allowed in QTA gap opened for IRAT",pri);
      return FALSE;
    }

    if(l1_fm_data_ptr->trm_reserved_reason != TRM_NUM_REASONS)
    {
      reason = l1_fm_data_ptr->trm_reserved_reason;
    }
    MSG_GERAN_MED_1_G("Request for TRM reason code %d in QTA support",reason);
  }
#endif /*FEATURE_QTA*/

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if (!grm_get_resource_lock_state(gas_id))
    {
       grm_clear_freq_info(client_id, gas_id);
   
       trm_band =  grm_get_trm_band_from_g_pri (pri, gas_id) ;
   
       if (trm_band == SYS_BAND_CLASS_NONE)
       {
         band_reg_fail = TRUE;
       }
       else
       {
         grm_add_trm_band_to_freq_input_type(trm_band, client_id, gas_id);
       }
    }
  /*If we have denied PM bands then add to trm request for DEMOD_PAGE*/
    if (grm_persistent_denial_monitoring(GRM_CHECK_PERSISTENT_DENIAL_FLAG, client_id,GRM_NULL_COMMAND, gas_id) &&
    gl1_drdsds_enabled(gas_id) &&
    (TRM_DEMOD_PAGE == l1_fm_map_pri_to_trm(pri,gas_id)))
   {
    (void)grm_persistent_denial_monitoring(GRM_ADD_DENIED_BANDS_TO_FREQ_INFO,
                                          client_id,
                                          GRM_REQUEST_CLEAR,
                                          gas_id);
   }
   else
   {
   /*if DR has been disabled since last paging occasion then clear the persistant denial condition since we're in legacy DSDS*/
      if(!gl1_drdsds_enabled(gas_id) &&
       (TRM_DEMOD_PAGE == l1_fm_map_pri_to_trm(pri,gas_id)))
      {
         (void)grm_persistent_denial_monitoring (GRM_RESET_PERSISTENT_DENIAL_MONITORING,
                                                 client_id,
                                                 GRM_NULL_COMMAND,
                                                 gas_id);
      }
   }
  MSG_GERAN_MED_4_G("l1_fm_grm_request reason %d pri %d frames_req %d band %d",
                    reason, pri, frames_required, trm_band);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

  duration = gpl1_multi_sim_convert_frames_to_sclcks( frames_required );

#ifdef FEATURE_GPRS_GBTA
  /* TRM reason codes not compatible with GBTA gaps should not be permitted to run inside a GBTA gap */
  if( !IS_GBTA_REASON(reason, reserve_sub_reason[gas_id]) && (GET_GBTA_STATE() == GRM_GBTA_ACTIVE) )
  {
    MSG_GERAN_MED_2_G("Don't run activity pri=%d reason=%d in GBTA gap", pri, reason);
    return FALSE;
  }

  /*dont request the TRM if we already have the grant inside GBTA for Page reorg-mode*/
  if( ( (l1_idle_data_store[gas_id].paging_data.paging_mode == PAGING_REORGANIZATION) ||
        (l1_idle_data_store[gas_id].paging_data.paging_mode == EXTENDED_PAGING) ||
        (l1_idle_data_store[gas_id].ext_BCCH_mask != 0)||
        ( missed_frames != 0 )
      ) &&
      (GET_GBTA_STATE() == GRM_GBTA_ACTIVE ) &&
      (grm_get_trm_status(client_id, gas_id) == GL1_TRM_GRANTED) )
  {
    if( !gl1_is_any_sub_in_transfer() )
    {
     /* We will come here, if the idle sub tries to read the PCH during the GBTA grant and meanwhile PT sub releases TRM
      * In the very next block, the idle sub again makes a TRM request. But now we need to make the GBTA status to NONE
      */
      SET_GBTA_STATUS(GRM_GBTA_NONE);
      grm_set_trm_grant_frames(0, client_id, gas_id);
      l1_reset_gbta_active_count(gas_id);
      return FALSE;
    }
    grm_set_trm_grant_fn(gl1_get_FN( gas_id ), client_id, gas_id);
    grm_set_trm_grant_frames(frames_required, client_id, gas_id);
    MSG_GERAN_LOW_0_G("GBTA already active and trm granted..do not request again");
    return TRUE;
  }

  /* Making sure that if getting new trm grant trm_status should be null */
  if ( ( TRM_DENIAL == grm_get_trm_grant_status(client_id, gas_id)) &&
       ( grm_get_trm_requested_at_warmup(client_id, gas_id) == FALSE ) )
  {
    grm_set_trm_status(GL1_TRM_NULL, client_id, gas_id);
  }
#endif /* FEATURE_GPRS_GBTA */

  trm_status = grm_get_trm_status(client_id, gas_id);
  if ( ( trm_status != GL1_TRM_RETAINED_FOR_ACCESS ) &&
       ( grm_get_trm_requested_at_warmup(client_id, gas_id) == FALSE ) )
  {
    /*
    If the lock hasn't been granted already then ask for it and return the result
    of the call with respect to the trm_status.*/
    if ( (trm_status != GL1_TRM_GRANTED) &&
         (trm_status != GL1_TRM_ABORTING) )
    {
      resource = gl1_ms_get_trm_resource(gas_id,reason);

#ifdef FEATURE_QTA
      if( l1_tskisr_blk->l1_state != L1_DEDICATED_MODE )
      {
        if((!IS_QTA_REASON(reason, reserve_sub_reason[gas_id])) &&  (gl1_hw_qta_gap_active(gas_id)) )
        {
          l1_fm_clear_required_frames(pri, gas_id);

          grm_set_trm_grant_frames(0, client_id, gas_id);

          MSG_GERAN_MED_3_G("Frame Manager:Activity %d at FN=%d reason=%d: Do not request with non QTA reason when W is in QTA mode",
                      pri, gl1_get_FN(gas_id), reason);

          return FALSE;
        }
      }
#endif /* FEATURE_QTA */
      if(!band_reg_fail)
      {
      /*To avoid stability issues due to delayed combined PRx DRx RF wake up processing, when G wakes up for ACQ ,
            *don't request TRM for DRx cahin .In case of delayed wake up  for CCH and NBCCH GFW has recovery mechanism.
            *For ACQ since we don't any way enable RxD we don't need to request TRM for DRx chain for ACQ.*/
      if((pri == FM_PRI_NCELL_ACQ) && (l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr == FALSE))
        {
        req_type = GRM_DIVRX_TRM_REQ_INVALID;
        }
      else
        {
        req_type = GRM_DIVRX_TRM_REQ_IDLE;
        }
      grm_grant_status = grm_request( client_id, duration, reason, reserve_sub_reason[gas_id], resource, req_type );
      }
      else
      {
        grm_grant_status = TRM_DENIAL;
      }

      grm_set_trm_grant_status(grm_grant_status, client_id, gas_id);

      if ( grm_grant_status != TRM_DENIAL )
      {
        grm_set_trm_status(GL1_TRM_GRANTED, client_id, gas_id);
        grm_set_trm_grant_fn(gl1_get_FN( gas_id ), client_id, gas_id);
        grm_set_trm_grant_frames(frames_required, client_id, gas_id);
      }
      else
      {
        grm_set_trm_grant_frames(0, client_id, gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

        /*if persistent denial contributed to the DEMOD_PAGE denial then disable persistent denial monitoring*/
        if (reason == TRM_DEMOD_PAGE &&
            grm_persistent_denial_monitoring(GRM_CHECK_PERSISTENT_DENIAL_FLAG,client_id, GRM_NULL_COMMAND, gas_id))
        {
          (void)grm_persistent_denial_monitoring(GRM_RESET_PERSISTENT_DENIAL_MONITORING, client_id,GRM_NULL_COMMAND,  gas_id);
        }
#endif
      }
    }
    /*
    Here the function has been called when the lock has already been granted,
    if this is the case then ask for an extension of the lock. Don't do this if L1
    is not in MULTI_SIM_IDLE mode as in dedicated SDCCH modes this request is not required
    as we have the lock for the */
    else if ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE )
    {
      if(GET_GBTA_STATE() == GRM_GBTA_ACTIVE)
      {
        MSG_GERAN_HIGH_0_G("GRM_GBTA:Extension of Lock is not required when GBTA is active");
      }
      else if (FALSE == l1_fm_grm_extend_duration(frames_required, pri, gas_id))
      {
        /*If resource lock isnt active only then Set FW Pending Release*/
        if ((l1_tskisr_blk->firmware_loaded == TRUE)&& (!grm_get_resource_lock_state(gas_id)))
        {
          /* TRM Extension is denied, so release the current lock first
            This will block any other activity doing a TRM Request.
            Only set this if fw is loaded otherwise there is potential
            for Gl1 never to request TRM
           */
          l1_tskisr_blk->firmware_load_status = FW_PENDING_RELEASE;
        }


          /*grm_release will release the chain and triggers sleep.
            Since GL1 has been denied by TRM for extended duration,
            therefore keep GL1 awake for the next tick so that GL1 can
            reserve chain for the next idle activity and prevents sleep
            to modify TRM booking without holding the chain*/
          gl1_drx_require_next_tick(gas_id);
      }
    }
    /* return the status of the lock */
  }

  /* reset trm_requested_at_warmup flag */
  grm_set_trm_requested_at_warmup(FALSE, l1_tskisr_blk->client_id, gas_id);
  trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
  return ( (trm_status == GL1_TRM_GRANTED) ||
           (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) );
}


/*===========================================================================

FUNCTION l1_fm_find_next_task_to_reserve

DESCRIPTION
  This function looks through the frame manager table to find the next activity
  that will run of type
  FM_PRI_CCCH
  FM_PRI_NCELL_BCCH
  FM_PRI_CBCH
  FM_PRI_NCELL_SCH
  FM_PRI_WCDMA_IDSEARCH
  FM_PRI_WCDMA_RECONF
  FM_PRI_MSIM_CBCH
  Then this will be booked into TRM

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void l1_fm_find_next_task_to_reserve_or_release(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  l1_fm_priority_T pri      = FM_PRI_RESERVED;
  uint32 FN_now             = gl1_get_FN( gas_id );
  boolean l1_fm_stale_booking = FALSE;  /* Call abort_recive at the end if TRUE */
  transfer_data_T *transfer_data_ptr = l1_tskisr_blk->next_params.L1Data.pTransfer_data;

  /* FM_PRI_CCCH - If there is a booking for CCCH then set this as the priority reservation of choice at this stage */
  if (l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number != NOT_A_FRAME_NUMBER)
  {
    pri = FM_PRI_CCCH;
  }

  /* FM_PRI_NCELL_BCCH - See if there is a booking at the next level below that could be booked */
  if (l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number != NOT_A_FRAME_NUMBER)
  {
    if ( l1_get_background_wcdma_plmn_scan_active(gas_id) )
    {
      MSG_GERAN_HIGH_0_G("G2W BPLMN active. No Ncell BCCH Only CCCH booking allowed");
    }
    else
    if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number ) )
    {

      MSG_GERAN_ERROR_2_G("Stale booking for FM_PRI_NCELL_BCCH booked=%d current=%d"
        ,l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number
        ,FN_now
        );

      l1_fm_stale_booking = TRUE;
      l1_fm_clear_required_frames(FM_PRI_NCELL_BCCH, gas_id);

    }
    else
    if (l1_fm_prioritise_nbcch_over_ccch(gas_id))
    {
        pri = FM_PRI_NCELL_BCCH;
    }
  }

  /* FM_PRI_CBCH - See if there is a booking at the next level below that could be booked */
  if ( l1_fm_data_ptr->l1_fm_table[FM_PRI_CBCH].reserved_frame_number != NOT_A_FRAME_NUMBER )
  {
    if ( l1_get_background_wcdma_plmn_scan_active( gas_id ) )
    {
      MSG_GERAN_HIGH_0_G( "G2W BPLMN active: no CBCH booking allowed");
    }
    else
    if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_CBCH].reserved_frame_number ) )
    {
      MSG_GERAN_ERROR_2_G( "Stale booking for FM_PRI_CBCH booked=%d current=%d",
                 l1_fm_data_ptr->l1_fm_table[FM_PRI_CBCH].reserved_frame_number,
                 FN_now);

      l1_fm_clear_required_frames( FM_PRI_CBCH, gas_id );
    }
#if defined (FEATURE_GSM_COEX)
    else if ( garb_intf_gas_is_xpg_coex_victim(gas_id) )/*If peer tech tx active means it is in data or connected mode so clear CBCH*/
    {
       MSG_GERAN_HIGH_0_G( " NO CBCH (FM_PRI_CBCH) booking allowed, When X tech tx is active" );
       l1_clear_cbch_activity(gas_id);

       /*release drx  tick for CBCH */
       l1_clear_cbch_drx_ref(gas_id);
     }
#endif /* FEATURE_QTA */
    else if ( (l1_fm_data_ptr->l1_fm_table[FM_PRI_CBCH].reserved_frame_number != NOT_A_FRAME_NUMBER) &&
               IS_FRAME_NUM_LATER( l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number,
                                  l1_fm_data_ptr->l1_fm_table[FM_PRI_CBCH].reserved_frame_number ) )
    {
      /* If pri is NBCCH and colliding with CBCH, then only prioritize NBCCH */
      if ( (pri == FM_PRI_CCCH) || 
           ((pri == FM_PRI_NCELL_BCCH) && l1_fm_is_task_bookable_before_high_priority(FM_PRI_CBCH, pri, gas_id)) )
      {
      /* when the pseudo-DRX feature is enabled, the collision is always resolved in favour of CBCH */
      MSG_GERAN_HIGH_1_G("MULTISIM: high priority CBCH allowed to book ahead of activity %d", (int)pri);

      /* this is now the next bookable activity */
      pri = FM_PRI_CBCH;

      /* ensure that no low priority CB activities are left booked */
      l1_fm_clear_required_frames( FM_PRI_MSIM_CBCH, gas_id );
    }
  }
  }

  /* FM_PRI_NCELL_SCH - See if there is a booking at the next level below that could be booked */
  if (l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].reserved_frame_number != NOT_A_FRAME_NUMBER)
  {
    if ( l1_get_background_wcdma_plmn_scan_active(gas_id) )
    {
      MSG_GERAN_HIGH_0_G("G2W BPLMN active. No SCH Only CCCH booking allowed");
    }
    else
    if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].reserved_frame_number ) )
    {

      MSG_GERAN_ERROR_2_G("Stale booking for FM_PRI_NCELL_SCH booked=%d current=%d"
        ,l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].reserved_frame_number
        ,FN_now
        );

      l1_fm_stale_booking = TRUE;
      l1_fm_clear_required_frames(FM_PRI_NCELL_SCH, gas_id);

    }
    else
    /* Check if FM_PRI_NCELL_SCH can be booked before pri */
    if ( l1_fm_is_task_bookable_before_high_priority(FM_PRI_NCELL_SCH, pri, gas_id) )
    {
      /* this is now the next bookable activity */
      pri = FM_PRI_NCELL_SCH;
    }
  }

  /* FM_PRI_NCELL_ACQ - See if there is a booking at the next level below that could be booked */
  if (l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_ACQ].reserved_frame_number != NOT_A_FRAME_NUMBER)
  {
    if ( l1_get_background_wcdma_plmn_scan_active(gas_id) )
    {
      MSG_GERAN_HIGH_0_G("G2W BPLMN active. No ACQ Only CCCH booking allowed");
    }
    else
    if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_ACQ].reserved_frame_number ) )
    {

      MSG_GERAN_ERROR_2_G("Stale booking for FM_PRI_NCELL_ACQ booked=%d current=%d"
        ,l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_ACQ].reserved_frame_number
        ,FN_now
        );
      l1_fm_stale_booking = TRUE;
      l1_fm_clear_required_frames(FM_PRI_NCELL_ACQ, gas_id);

    }
    else
    /* Check if FM_PRI_NCELL_ACQ can be booked before pri */
    if ( l1_fm_is_task_bookable_before_high_priority(FM_PRI_NCELL_ACQ, pri, gas_id) )
    {
      /* this is now the next bookable activity */
      pri = FM_PRI_NCELL_ACQ;
    }
  }


  /* FM_PRI_WCDMA_RECONF - See if there is a booking at the next level below that could be booked */
  if ( l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_RECONF].reserved_frame_number != NOT_A_FRAME_NUMBER )
  {
    MSG_GERAN_LOW_0_G( "FM_PRI_WCDMA_RECONF booking");
    if ( l1_get_background_wcdma_plmn_scan_active(gas_id) )
    {
      MSG_GERAN_HIGH_0_G( "G2W BPLMN active: no DSDS WCDMA reconf search booking allowed");
    }
    else
    if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_RECONF].reserved_frame_number ) )
    {
      MSG_GERAN_ERROR_2_G( "Stale booking for FM_PRI_WCDMA_RECONF booked=%d current=%d",
                 l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_RECONF].reserved_frame_number,
                 FN_now );

      l1_fm_stale_booking = TRUE;
      l1_fm_clear_required_frames( FM_PRI_WCDMA_RECONF, gas_id );
      l1_sc_wcdma_data[gas_id].scheduled_wcdma_reconf_fn           = NOT_A_FRAME_NUMBER;
      l1_sc_wcdma_data[gas_id].drx_ref                             = 0;
    }
    else
    {
      MSG_GERAN_LOW_3_G( "FM_PRI_WCDMA_RECONF: pri:%d; l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number: %d;l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_RECONF].reserved_frame_number:%d ",
                           pri,
                           l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number,
                           l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_RECONF].reserved_frame_number );

      /* Check if FM_PRI_WCDMA_RECONF can be booked before pri */
      if ( l1_fm_is_task_bookable_before_high_priority(FM_PRI_WCDMA_RECONF, pri, gas_id) )
      {
        /* this is now the next bookable activity */
        pri = FM_PRI_WCDMA_RECONF;
      }

      /* not sure why this is happening first time, to be checked*/
      if((0==pri)&&(NOT_A_FRAME_NUMBER==l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number))
      {
        pri = FM_PRI_WCDMA_RECONF;
      }
    }
  }

    /* FM_PRI_WCDMA_IDSEARCH - See if there is a booking at the next level below that could be booked */
    if ( l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_IDSEARCH].reserved_frame_number != NOT_A_FRAME_NUMBER )
    {
      MSG_GERAN_LOW_0_G( "FM_PRI_WCDMA_IDSEARCH booking");
      if ( l1_get_background_wcdma_plmn_scan_active(gas_id) )
      {
        MSG_GERAN_HIGH_0_G( "G2W BPLMN active: no DSDS WCDMA ID search booking allowed");
      }
      else
      if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_IDSEARCH].reserved_frame_number ) )
      {
        MSG_GERAN_ERROR_2_G( "Stale booking for FM_PRI_WCDMA_IDSEARCH booked=%d current=%d",
                   l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_IDSEARCH].reserved_frame_number,
                   FN_now );

        l1_fm_clear_required_frames( FM_PRI_WCDMA_IDSEARCH, gas_id );
        l1_sc_wcdma_data[gas_id].id_search_fn                        = L1_GPRS_INVALID_FN;
        l1_sc_wcdma_data[gas_id].l1_irat_reserve_at_sent             = FALSE;
        l1_sc_wcdma_data[gas_id].drx_ref                             = 0;
      }
      else
      {
        MSG_GERAN_LOW_3_G( "FM_PRI_WCDMA_IDSEARCH: pri:%d; l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number: %d;l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_IDSEARCH].reserved_frame_number:%d ",pri, l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number,l1_fm_data_ptr->l1_fm_table[FM_PRI_WCDMA_IDSEARCH].reserved_frame_number );
        /* Check if FM_PRI_WCDMA_IDSEARCH can be booked before pri */
        if ( l1_fm_is_task_bookable_before_high_priority(FM_PRI_WCDMA_IDSEARCH, pri, gas_id) )
        {
          /* this is now the next bookable activity */
          pri = FM_PRI_WCDMA_IDSEARCH;
        }

        /* not sure why this is happening first time, to be checked*/
        if((0==pri)&&(NOT_A_FRAME_NUMBER==l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number))
        {
          pri = FM_PRI_WCDMA_IDSEARCH;
        }
      }
    }
#ifdef FEATURE_GSM_TO_LTE
    /* FM_PRI_LTE_IDSEARCH - See if there is a booking at the next level below that could be booked */
    if ( l1_fm_data_ptr->l1_fm_table[FM_PRI_LTE_IDSEARCH].reserved_frame_number != NOT_A_FRAME_NUMBER )
    {
      MSG_GERAN_LOW_0_G( "FM_PRI_LTE_IDSEARCH booking");
      if ( l1_get_background_wcdma_plmn_scan_active(gas_id) )
      {
        MSG_GERAN_HIGH_0_G( "G2W BPLMN active: no DSDS LTE ID search booking allowed");
      }
      else
      if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_LTE_IDSEARCH].reserved_frame_number ) )
      {
        MSG_GERAN_ERROR_2_G( "Stale booking for FM_PRI_LTE_IDSEARCH booked=%d current=%d",
                   l1_fm_data_ptr->l1_fm_table[FM_PRI_LTE_IDSEARCH].reserved_frame_number,
                   FN_now );
        l1_fm_clear_required_frames( FM_PRI_LTE_IDSEARCH, gas_id );
        l1_sc_wcdma_data[gas_id].id_search_fn                        = L1_GPRS_INVALID_FN;
        l1_sc_wcdma_data[gas_id].l1_irat_reserve_at_sent             = FALSE;
        l1_sc_wcdma_data[gas_id].drx_ref                             = 0;
      }
      else
      {
        MSG_GERAN_LOW_3_G( "FM_PRI_LTE_IDSEARCH: pri:%d; l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number: %d;l1_fm_data_ptr->l1_fm_table[FM_PRI_LTE_IDSEARCH].reserved_frame_number:%d ",pri, l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number,l1_fm_data_ptr->l1_fm_table[FM_PRI_LTE_IDSEARCH].reserved_frame_number );

        /* Check if FM_PRI_LTE_IDSEARCH can be booked before pri */
        if ( l1_fm_is_task_bookable_before_high_priority(FM_PRI_LTE_IDSEARCH, pri, gas_id) )
        {
          /* this is now the next bookable activity */
          pri = FM_PRI_LTE_IDSEARCH;
        }

        /* not sure why this is happening first time, to be checked*/
        if((0==pri)&&(NOT_A_FRAME_NUMBER==l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number))
        {
          pri = FM_PRI_LTE_IDSEARCH;
        }
      }
    }
#endif
#ifdef FEATURE_GSM_TDS
    /* FM_PRI_TDS_ACQ - See if there is a booking at the next level below that could be booked */
    if ( l1_fm_data_ptr->l1_fm_table[FM_PRI_TDS_ACQ].reserved_frame_number != NOT_A_FRAME_NUMBER )
    {
      MSG_GERAN_LOW_0_G( "FM_PRI_TDS_ACQ booking");
      if ( l1_get_background_wcdma_plmn_scan_active(gas_id) )
      {
        MSG_GERAN_HIGH_0_G( "G2W BPLMN active: no DSDS WCDMA ID search booking allowed");
      }
      else
      if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_TDS_ACQ].reserved_frame_number ) )
      {
        MSG_GERAN_ERROR_2_G( "Stale booking for FM_PRI_TDS_ACQ booked=%d current=%d",
                   l1_fm_data_ptr->l1_fm_table[FM_PRI_TDS_ACQ].reserved_frame_number,
                   FN_now );

        l1_fm_clear_required_frames( FM_PRI_TDS_ACQ, gas_id );
        l1_sc_wcdma_data[gas_id].id_search_fn                        = L1_GPRS_INVALID_FN;
        l1_sc_wcdma_data[gas_id].l1_irat_reserve_at_sent             = FALSE;
        l1_sc_wcdma_data[gas_id].drx_ref                             = 0;
      }
      else
      {
        MSG_GERAN_LOW_3_G( "FM_PRI_TDS_ACQ: pri:%d; l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number: %d;l1_fm_data_ptr->l1_fm_table[FM_PRI_TDS_ACQ].reserved_frame_number:%d ",pri, l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number,l1_fm_data_ptr->l1_fm_table[FM_PRI_TDS_ACQ].reserved_frame_number );
        /* Check if FM_PRI_TDS_ACQ can be booked before pri */
        if ( l1_fm_is_task_bookable_before_high_priority(FM_PRI_TDS_ACQ, pri, gas_id) )
        {
          /* this is now the next bookable activity */
          pri = FM_PRI_TDS_ACQ;
        }
        /* not sure why this is happening first time, to be checked*/
        if((0==pri)&&(NOT_A_FRAME_NUMBER==l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number))
        {
          pri = FM_PRI_TDS_ACQ;
        }
      }
    }
#endif

  /* FM_PRI_MSIM_CBCH - See if there is a booking at the next level below that could be booked */
  if ( l1_fm_data_ptr->l1_fm_table[FM_PRI_MSIM_CBCH].reserved_frame_number != NOT_A_FRAME_NUMBER )
  {
    if ( l1_get_background_wcdma_plmn_scan_active(gas_id) )
    {
      MSG_GERAN_HIGH_0_G( "G2W BPLMN active: no DSDS CBCH booking allowed");
    }
    else
    if ( IS_FRAME_NUM_LATER_OR_EQUAL( FN_now, l1_fm_data_ptr->l1_fm_table[FM_PRI_MSIM_CBCH].reserved_frame_number ) )
    {
      MSG_GERAN_ERROR_2_G( "Stale booking for FM_PRI_MSIM_CBCH booked=%d current=%d",
                 l1_fm_data_ptr->l1_fm_table[FM_PRI_MSIM_CBCH].reserved_frame_number,
                 FN_now );

      l1_fm_clear_required_frames( FM_PRI_MSIM_CBCH, gas_id );
    }
    else if ( OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) || garb_intf_gas_is_xpg_coex_victim(gas_id))
    {
      MSG_GERAN_HIGH_0_G( "X-tech in BG TRAFFIC: NO CBCH (FM_PRI_MSIM_CBCH)booking allowed" );
      l1_clear_cbch_activity(gas_id);
      /*release drx  tick for CBCH */
      l1_clear_cbch_drx_ref(gas_id);
    }
    else
    /* Check if FM_PRI_MSIM_CBCH can be booked before pri */
    if ( l1_fm_is_task_bookable_before_high_priority(FM_PRI_MSIM_CBCH, pri, gas_id) )
    {
      /* this is now the next bookable activity */
      pri = FM_PRI_MSIM_CBCH;
    }
  }


  if(pri != FM_PRI_CCCH)
  {
     if ( gl1_msg_get_multi_sim_mode() && L1_background_HPLMN_search_in_progress(gas_id) )
     {
       if (l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number != NOT_A_FRAME_NUMBER)
       {
         MSG_GERAN_HIGH_1_G("BPLMN active. Only CCCH booking allowed. Other activity %d booking is not allowed", pri);
         pri = FM_PRI_CCCH;
       }
       else
       {
         pri = FM_PRI_RESERVED;
       }
     }
  }

#if defined(FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
  /* Idle mode TA is opened when mm_non_drx mode is set,
    * block all activities other than CCCH
    */
  if (gl1_msg_get_multi_sim_mode() && l1_is_idle_ta_prepared(gas_id ) )
  {
    if ( l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number != NOT_A_FRAME_NUMBER )
    {
        MSG_GERAN_HIGH_1_G("activity %d booking not allowed during IDLE TA", pri);

         gl1_drx_release_fn_tick(l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number , gas_id);
	     l1_fm_clear_required_frames(FM_PRI_CCCH, gas_id);

		l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number = l1_get_reorg_sub_ccch_fn(gas_id);

        /*Tell FM that we want new activity at that FN */
        l1_fm_set_required_frame_number(FM_PRI_CCCH, l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number , L1_SC_DURATION_CCCH, gas_id);
		l1_add_ccch_drx_ref(l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number,gas_id);
      
      pri = FM_PRI_CCCH;
	    gl1_drx_require_next_tick(gas_id);
    }
    else
    {
      pri = FM_PRI_RESERVED;
    }
  }
#endif /* (FEATURE_G2X_IDLE_TUNEAWAY || FEATURE_G2X_REORG_TUNEAWAY)*/

  if((l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND) && 
      (pri == FM_PRI_CCCH) &&
      (l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number != NOT_A_FRAME_NUMBER) &&
      (transfer_data_ptr->starting_time_valid)&&
      (IS_FRAME_NUM_LATER_OR_EQUAL(l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number,transfer_data_ptr->starting_time)))
     {
          MSG_GERAN_HIGH_1_G("DL TBF is priortized over Acitvity=%d ", pri);
          pri = FM_PRI_RESERVED;
     }
 
  if (l1_fm_stale_booking == TRUE )
  {
    abort_receive(gas_id);
    l1_fm_stale_booking = FALSE;
  }

  /*if update in progress and a Irat activity, then clear activity and do release*/
  if (l1_sc_get_wcdma_list_update_inprogress(gas_id)&& gl1_drdsds_enabled(gas_id))
  {
    switch (pri)
    {
      case FM_PRI_WCDMA_IDSEARCH:
      case FM_PRI_WCDMA_RECONF:
#ifdef FEATURE_GSM_TDS
      case FM_PRI_TDS_ACQ:
#endif
#ifdef FEATURE_GSM_TO_LTE
      case FM_PRI_LTE_IDSEARCH:
#endif
       l1_fm_clear_required_frames( pri, gas_id );
       pri = FM_PRI_RESERVED;
       MSG_GERAN_HIGH_1_G( "update_list: has been received, Irat Activity Cleared: %d and skip l1_fm_grm_reserve_at", pri);
      default:
       break;
    }
  }

    /* inform COEX-WLAN that activity is over */
#ifdef FEATURE_WLAN_COEX_SW_CXM
  garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif

  if ((pri != FM_PRI_RESERVED) && (l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number!=NOT_A_FRAME_NUMBER))
  {
    l1_fm_grm_reserve_at(
      pri,
      l1_fm_data_ptr->l1_fm_table[pri].required_frames,
      l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number, gas_id );
  }
  else
  {
    call_in_task_grm_release( l1_tskisr_blk->client_id );

    l1_fm_reset_pri_res(gas_id);

  }
}


/*===========================================================================

FUNCTION gl1_store_grm_sub_reason

DESCRIPTION
  Stores the sub trm reason for logging
  Need to add Qualifiers of TRM reasons.
  At the current stage, the qualifiers are not used to affect TRM decision.
  They are used for logging and analysis purposes.
INPUTS
 gas_id
 sub trm reason

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void gl1_store_grm_sub_reason(sys_proc_type_e_type sub_reason, gas_id_t gas_id)
{
   reserve_sub_reason[gas_id] = sub_reason ;
}

/*===========================================================================

FUNCTION l1_fm_grm_reserve_at

DESCRIPTION
  Books in a TRM reservation at the required frame number, for the required task.

INPUTS
  Priority of the task, frame number at which frames are required, and the
  number of frames required to perform the task..

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void l1_fm_grm_reserve_at( l1_fm_priority_T pri, uint32 frames_required,
                                  uint32 required_frame_nbr, gas_id_t gas_id )
{
  grm_reserve_at_data_t    grm_reserve_at_data;
  grm_reserve_at_data_t   *grm_reserve_at_data_p;
  trm_reason_enum_t        trm_booked_reason;
  grm_client_enum_t        client_id;

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_fm_data_t              *l1_fm_data_ptr = &l1_fm_data[gas_id];
  uint8 drx_ref = 0;
  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  timetick_type            acq_duration = 0;
  #endif
  client_id             = l1_tskisr_blk->client_id;

  grm_reserve_at_data_p = &grm_reserve_at_data;

  /* calculate the required parameters and pass in to TRM in the reservation request */
  grm_reserve_at_data_p->frames_required    = required_frame_nbr;
  grm_reserve_at_data_p->frame_duration     = frames_required;

  grm_reserve_at_data_p->reason             = l1_fm_map_pri_to_trm( pri, gas_id );
  grm_reserve_at_data_p->sub_reason         = reserve_sub_reason[gas_id];
  grm_reserve_at_data_p->pri                = pri;
  grm_reserve_at_data_p->client_id          = client_id;
  grm_reserve_at_data_p->gas_id             = gas_id;
  grm_reserve_at_data_p->resource           = gl1_ms_get_trm_resource( gas_id, grm_reserve_at_data_p->reason );

  grm_reserve_at_data_p->duration           =
    gpl1_multi_sim_convert_frames_to_sclcks( frames_required );


  /* Check if a DRX rebooking is needed, if so rebook here */
  if( pri == FM_PRI_CCCH )
  {
    l1_idle_rebook_drx_tick(gas_id);
  }

  /* L1 should not suppress TRM reservation if TRM reservation is removed. */
  trm_booked_reason = grm_gl1_get_current_trm_reason(client_id, gas_id);

  MSG_GERAN_MED_3_G( "TRM - grm_reserve_at-new reason:%d trm_booked_reason %d trm_status:%d",
             grm_reserve_at_data_p->reason, trm_booked_reason,grm_get_trm_status(client_id, gas_id) );

  if( ( ( trm_booked_reason == TRM_NUM_REASONS ) && ( l1_tskisr_blk->firmware_load_status != FW_PENDING_RELEASE ) ) ||
      grm_get_trm_release_required(client_id, gas_id) ||
      (!l1_fm_suppress_grm_reserve_at( pri,grm_reserve_at_data_p->reason, required_frame_nbr, frames_required, gas_id ) ) )
  {
    l1_fm_data_ptr->trm_reserved_reason   = grm_reserve_at_data_p->reason;
    l1_fm_data_ptr->trm_reserved_fn       = required_frame_nbr;
    l1_fm_data_ptr->trm_reserved_duration = frames_required;
    l1_fm_data_ptr->trm_reserved_pri      = pri;

    /* Clear the sleep missed_frames if it is set */
    gl1_hw_sleep_clear_missed_frames(gas_id);
	
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH

	if(l1_fm_data_ptr->trm_reserved_reason == TRM_DEMOD_PAGE)
	  { 
	    acq_duration=timetick_cvt_to_sclk(FRAMES_TO_MSECS(required_frame_nbr), T_MSEC);
		rem_time_ms = gl1_get_rem_time_in_ms((uint32)acq_duration);
     	gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_EXTENDED_PAGE_REM_TIME);
   	  }
	
#endif

    call_in_task_grm_reserve_at( grm_reserve_at_data );

    MSG_GERAN_HIGH_4_G( "FN:%d TRM - grm_reserve_at() - %d frames at FN:%d, pri:%d",
            gl1_get_FN( gas_id ), frames_required, required_frame_nbr, l1_fm_data_ptr->trm_reserved_pri );

    /*Book same FN in DRX Manager as well , so that TRM/GRM Booking and DRX manager are in sync*/
    if(gl1_msg_get_multi_sim_mode())
    {
      switch (pri) 
      {
        case FM_PRI_NCELL_ACQ:
          drx_ref = gl1_drx_require_tick( required_frame_nbr, gas_id );
          gl1_set_fb_drx_ref(drx_ref, gas_id);
          break;
  
        case FM_PRI_NCELL_SCH:
          drx_ref = gl1_drx_require_tick( required_frame_nbr, gas_id );
          gl1_set_sb_drx_ref(drx_ref, gas_id);
          break;

        case FM_PRI_NCELL_BCCH:
          drx_ref = gl1_drx_require_tick (required_frame_nbr, gas_id);
          gl1_set_bcch_drx_ref(drx_ref, gas_id);
          break;

        case FM_PRI_WCDMA_RECONF:
        case FM_PRI_WCDMA_IDSEARCH:

#ifdef  FEATURE_GSM_TO_LTE
        case FM_PRI_LTE_IDSEARCH:
#endif
#ifdef FEATURE_GSM_TDS
        case FM_PRI_TDS_ACQ: 
#endif
         drx_ref = gl1_drx_require_tick (required_frame_nbr, gas_id);
         l1_sc_wcdma_data[gas_id].drx_ref = drx_ref;
         break;
		 
        default:
          //do nothing
          break;
      }/*switch pri*/
     }/*if multisim*/
  }
  else
  {
    MSG_GERAN_HIGH_3_G( "FN:%d TRM - grm_reserve_at() - %d frames at FN:%d - suppressed duplicate",
              gl1_get_FN( gas_id ), frames_required, required_frame_nbr );
  }

}

/*===========================================================================

FUNCTION l1_multi_sim_trm_manager

DESCRIPTION
  This function is the main scheduler for the trm relate activities in IDLE mode.
  If controls the releasing and reserving of activities via calls to trm_shim_release
  and trm_shim_reserve_at in the L1 task.

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  boolean returns TRUE = No running activities
  boolean returns FALSE = One or more activities are still running or cleanup is still pending

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_multi_sim_trm_manager( gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T               *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  boolean release_required = FALSE;
  boolean reserve_required = FALSE;
  boolean fm_clear_release_required = FALSE;
  boolean ret_val = TRUE;
  uint16  task;
  trm_get_info_return_type get_granted;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
#if defined (FEATURE_QBTA)
  trm_time_t      when;
#endif

#ifdef FEATURE_QTA
  boolean nbcch_complete   = FALSE;
  boolean nsch_complete    = FALSE;
  boolean ccch_complete    = (l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].action == FM_ACTION_CLEAR_ACTIVE_TASK);
  nbcch_complete           = (l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].action == FM_ACTION_CLEAR_ACTIVE_TASK);
  nsch_complete            = (l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].action == FM_ACTION_CLEAR_ACTIVE_TASK);
#endif /* FEATURE_QTA */

  /* Must not honor suspended TRM release if trm_status has been cleared down to TRM_NOT_GRANTED. This can
  happen for e.g. when a TRM extend duration request is denied. Since it can no longer be assumed that
  any subsequent TRM request will allocate the same device as is currently held the TRM release *must*
  be initiated to ensure that RF is deinitialised for the current device and then re-initialised based
  on the new device ID allocated by TRM.*/
  trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
  if( (trm_status == GL1_TRM_NOT_GRANTED) && (l1_fm_data_ptr->suspend_trm_release != 0) )
  {
     l1_fm_data_ptr->suspend_trm_release = 0;
     MSG_GERAN_HIGH_1_G("l1_multi_sim_trm_manager trm_status == %d cannot honor suspend TRM release", trm_status);
  }

  if ( l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr )
  {
     // Don't run trm manager state machine just yet
     gl1_drx_require_next_tick(gas_id);

     return FALSE;
  }
  if ( gl1_msg_get_multi_sim_mode() && (l1_fm_data_ptr->suspend_trm_release == 0) )
  {
    /******************************************************************************************
    Go through all entries in the frame manager tables to decide what actions need processing
    *******************************************************************************************/
    for (task=0; task<FM_PRI_MAX; task++)
    {
#ifdef FRAME_MANAGER_DEBUG
      #error code not present
#endif
      switch ( l1_fm_data_ptr->l1_fm_table[task].action )
      {
        /* 1. Check first of all for any active task that is now completed and can be
        cleared and released. */
        case FM_ACTION_CLEAR_ACTIVE_TASK:
        {
          release_required = TRUE;

          /* reset to the default */
          l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_NONE;

          break;
        }

        /* 2. Check for any newly booked in future tasks that require a TRM reservation
        but only book them now if there is no TRM lock. If the lock exists then when
        this is released the task will be booked in, and this will be performed using the
        action FM_ACTION_CLEAR_ACTIVE_TASK */
        case FM_ACTION_SET:
        {
          trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
          if (( (task == FM_PRI_CCCH) || (task == FM_PRI_NCELL_BCCH) || (task == FM_PRI_NCELL_SCH)
              ||  (task == FM_PRI_NCELL_ACQ) || ( task == l1_fm_get_active_CBCH_pri(gas_id)
              || (task == FM_PRI_WCDMA_IDSEARCH)
              || (task == FM_PRI_WCDMA_RECONF)
#ifdef FEATURE_GSM_TO_LTE
              || (task == FM_PRI_LTE_IDSEARCH)
#endif
#ifdef FEATURE_GSM_TDS
              || (task == FM_PRI_TDS_ACQ)
#endif
            ) )
              &&  ( !l1_fm_any_activity_running(gas_id) ) )
          {
            reserve_required = TRUE;
          }
          else
          /* Check to see if something needs to be re-booked back into TRM */
          if (( task == FM_PRI_RESERVED ) && ( trm_status == GL1_TRM_NULL ))
          {
            reserve_required = TRUE;
          }

          /* reset to the default */
          l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_NONE;

          break;
        }

        /* 3. Can release, but a page has been decoded and if early decode has been used and we
        are within a WCDMA tuneaway period, its more efficient to hold the lock until the SCE
        has run and decided if it now needs to schedule any activities. If this is not done the
        lock is given back to WCDMA and SCE activities fail to get scheduled */
        case FM_ACTION_DELAYED_RELEASE_FOR_TUNEAWAY:
        {
          trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
          if(trm_status != GL1_TRM_NOT_GRANTED)
          {
             l1_fm_set_suspend_trm_release( 1, FM_PRI_RESERVED, gas_id );

             l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_CLEAR_ACTIVE_TASK;
          }
          else
          {
             release_required = TRUE;

             /* reset to the default */
             l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_NONE;
          }
          break;
        }

        case FM_ACTION_DELAYED_RELEASE_FOR_BPLMN:
        {
          trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
          if(trm_status != GL1_TRM_NOT_GRANTED)
          {
            l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY, FM_PRI_RESERVED, gas_id );

            l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_CLEAR_ACTIVE_TASK;
          }
          else
          {
            release_required = TRUE;

            /* reset to the default */
            l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_NONE;
          }
          break;
        }

        /* 4. Frames have been re-booked for the current running activity. This typically happens
        when a state machine is granted frames to run but then decides that it no longer has any
        work to do. This is a combined clear and set frames */
        case FM_ACTION_CLEAR_AND_SET:
        {
          /* reset to the default */
          l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_NONE;

          l1_fm_data_ptr->l1_fm_table[task].state = FM_STATE_NULL;

          release_required = TRUE;
          break;
        }

        /* 5. Frames are being cleared - Release TRM only when there is no activity which requires TRM reservation */
        case FM_ACTION_CLEAR:
        {
          fm_clear_release_required = TRUE;

          /* reset to the default */
            l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_NONE;

          break;
        }

        default:
          /* reset to the default */
          l1_fm_data_ptr->l1_fm_table[task].action = FM_ACTION_NONE;

      }

      if ( l1_fm_data_ptr->suspend_trm_release != 0 )
      {
        MSG_GERAN_HIGH_2_G( "Don't release - suspend_trm_release is set %d for event_pri %d",
                            l1_fm_data_ptr->suspend_trm_release,
                            l1_fm_data_ptr->suspend_trm_release_pri );

       gl1_drx_require_next_tick(gas_id);

       break;
     }
    }

    /* final check when release_required and reserve_required are not set
    * see if fm clear is required then go through all the FM entry and if
    * none of the fm enry is valid then set release required
    */
    for (task=0; fm_clear_release_required && (task<FM_PRI_MAX); task++)
    {
      if ( l1_fm_data_ptr->l1_fm_table[task].action == FM_ACTION_CLEAR_ACTIVE_TASK )
      {
        fm_clear_release_required = FALSE;
      }
    }

    if( fm_clear_release_required && !release_required && !reserve_required)
    {
      for (task=FM_PRI_CCCH; task<FM_PRI_MAX; task++)
      {
        if( (l1_fm_data_ptr->l1_fm_table[task].reserved_frame_number != NOT_A_FRAME_NUMBER)
             && !( IS_FRAME_NUM_LATER( gl1_get_FN(gas_id), l1_fm_data_ptr->l1_fm_table[task].reserved_frame_number ) )
          )
        {
          /*if we haven't reserved TRM for this activity in the FM table, reserve is required*/
          if ( (l1_fm_data_ptr->trm_reserved_fn != NOT_A_FRAME_NUMBER)
         && (IS_FRAME_NUM_LATER(l1_fm_data_ptr->l1_fm_table[task].reserved_frame_number, l1_fm_data_ptr->trm_reserved_fn))
             && (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_CALLBACK_PENDING))
          {
            MSG_GERAN_HIGH_2_G("TRM reservation required for activity %d ,curr trm res FN %d", task, l1_fm_data_ptr->trm_reserved_fn);
            reserve_required = TRUE;
            break;
          }
        }
      }

      if(task == FM_PRI_MAX )
      {
        MSG_GERAN_MED_0_G("l1_multi_sim_trm_manager: Release required is set ");
        release_required = TRUE;
      }
    }

    if( grm_get_trm_release_required(l1_tskisr_blk->client_id, gas_id) )
    {
      MSG_GERAN_MED_0_G("l1_multi_sim_trm_manager: TRM Release required is set ");
      release_required = TRUE;
    }

    MSG_GERAN_MED_4_G("l1_multi_sim_trm_manager trm_status %d main_command %d rsv_rqd=%d, release_required %d",
        grm_get_trm_status(l1_tskisr_blk->client_id, gas_id),l1_tskisr_blk->main_command,reserve_required,release_required);

    /*****************************************************************************
    Check that there is not intention to release if an activity is still running.
    If so then prevent the release from actioning
    *****************************************************************************/
    if ( l1_fm_any_activity_running(gas_id) && ( release_required || reserve_required ) )
    {
      MSG_GERAN_HIGH_1_G("Don't release - fm activity %d still running", l1_fm_data[gas_id].fm_active_pri);
    }
    else
    /*****************************************************************************
    Check for if access is imminent and if so hold on to the lock regardless
    ******************************************************************************/

    // lock must have been granted when these evaluate to TRUE
    if ( ((l1_tskisr_blk->main_command == L1_ACCESS_COMMAND) ||
           (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_RETAINED_FOR_ACCESS)) &&
#ifdef FEATURE_QTA
         ( l1_tskisr_blk->qta_in_progress == FALSE ) &&
#endif /* FEATURE_QTA */
         ( release_required || reserve_required ))
    {
      MSG_GERAN_HIGH_3_G("Don't release-retain lock for access : trm_status %d main_command %d release_required %d",
                         grm_get_trm_status(l1_tskisr_blk->client_id, gas_id),
                         l1_tskisr_blk->main_command,
                         release_required);
    }
    else
    if (  (l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND_ABORTING_IDLE) && (trm_status!= GL1_TRM_NOT_GRANTED) &&
#ifdef FEATURE_QTA
       ( !gl1_hw_qta_gap_active(gas_id)) &&
#endif /* FEATURE_QTA */
       ( release_required || reserve_required ))
    {
     MSG_GERAN_HIGH_4_G("Don't release-retain lock for access EST DL : trm_status %d main_command %d release_required %d, reserve_required %d",
                        grm_get_trm_status(l1_tskisr_blk->client_id, gas_id),
                        l1_tskisr_blk->main_command,
                        release_required,
                        reserve_required);
    }
    else
    {
      uint32 gap_to_ccch = NOT_A_FRAME_NUMBER;
      uint32 gap_to_nbcch = NOT_A_FRAME_NUMBER;
      uint32 gap_to_ncell_sch = NOT_A_FRAME_NUMBER;
      trm_reason_enum_t trm_reason = grm_gl1_get_current_trm_reason(l1_tskisr_blk->client_id, gas_id );

      if( l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number != NOT_A_FRAME_NUMBER )
      {
        gap_to_ccch = SUB_FN(l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number, gl1_get_FN( gas_id ));
      }

    if( l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number != NOT_A_FRAME_NUMBER )
      {
        gap_to_nbcch = SUB_FN(l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number, gl1_get_FN( gas_id ));
      }

    if( l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].reserved_frame_number != NOT_A_FRAME_NUMBER )
      {
        gap_to_ncell_sch = SUB_FN(l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].reserved_frame_number, gl1_get_FN( gas_id ));
      }

      /* when MM is non Drx mode and TRM is already granted , TRM is not released and lock is extended 
       when Gap to CCCH < 4 frames.
       There is possibilty of TRM reason getting changed. So check if TRM change reason is needed
        */
      if((idle_data_ptr->mm_non_drx_mode) &&
          (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_GRANTED)&&
          (gap_to_ccch != NOT_A_FRAME_NUMBER)&&
          (gap_to_ccch < GL1_DEFS_FRAMES_IN_BLK_PERIOD) && 
          (!l1_is_idle_ta_prepared(gas_id)))
      {
         trm_reason_enum_t new_reason;

        if ((TRUE == gl1_is_geran_tuneaway_allowed(gas_id)) &&
            (FALSE == idle_data_ptr->non_updated_lai_in_nmo1))
        {
          new_reason = TRM_ACCESS;
        }
        else
        {
          new_reason = TRM_CHANNEL_MAINTENANCE_INV;
        }

        if (grm_change_priority(l1_tskisr_blk->client_id, new_reason, SYS_PROC_TYPE_PAGE, gas_id ))     
        {
          MSG_GERAN_HIGH_2_G("TRM lock is retained MM non DRX mode gap_to_ccch = %d  Frames Required= %d",
                           gap_to_ccch, l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].required_frames);
          l1_fm_set_suspend_trm_release( gap_to_ccch, FM_PRI_CCCH, gas_id );
        }
      }

#ifdef FEATURE_QTA
      /******************************************************************************
      Check if the TRM lock needs to be retained for impending CCCH activity
      *******************************************************************************/
      else if( ( ccch_complete || nbcch_complete || nsch_complete) &&
          ( release_required || reserve_required ) &&
          ( l1_tskisr_blk->qta_in_progress ) &&
          ( gap_to_ccch < L1_MIN_WCDMA_INTER_QTA_GAP) &&
          ( l1_fm_grm_extend_duration( gap_to_ccch + l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].required_frames, FM_PRI_CCCH, gas_id) )
        )
      {
        l1_fm_set_suspend_trm_release( gap_to_ccch, FM_PRI_CCCH, gas_id );
        MSG_GERAN_HIGH_2_G("FN = %d TRM lock is retained for impending CCCH activity at FN = %d", gl1_get_FN( gas_id ), l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number);
#if defined FEATURE_QBTA
        {
          when =  gpl1_multi_sim_calculate_grm_reservation_time( l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number , gas_id ) +
                        timetick_get_safe() ;
          gl1_hw_fill_next_qta_gap_params( when , gas_id );
        }
#endif
      }


    else if(
          ( ccch_complete || nbcch_complete ) &&
          ( release_required || reserve_required ) &&
          ( l1_tskisr_blk->qta_in_progress ) &&
          ( gap_to_ncell_sch < L1_MIN_WCDMA_INTER_QTA_GAP) &&
          ( l1_fm_grm_extend_duration( gap_to_ncell_sch + l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].required_frames, FM_PRI_NCELL_SCH, gas_id) )
        )
      {
        l1_fm_set_suspend_trm_release( gap_to_ncell_sch, FM_PRI_NCELL_SCH, gas_id );
        l1_tskisr_blk->firmware_loaded = FALSE ;
        MSG_GERAN_HIGH_2_G("FN = %d TRM lock is retained for impending SCH activity at FN = %d", gl1_get_FN( gas_id ), l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].reserved_frame_number);
      }
      else
      /******************************************************************************
      Check if the TRM lock needs to be retained for impending CCCH activity When it is IN LTA GAP
      *******************************************************************************/
      if( ( gl1_msg_get_multi_sim_standby_mode() && (gl1_get_num_inactive_subs() > 0) ) &&
          (!IS_QTA_REASON(grm_gl1_get_current_trm_reason( l1_tskisr_blk->client_id, gas_id ), reserve_sub_reason[gas_id]) || gl1_drdsds_enabled(gas_id)) &&
          ( !gpl1_get_grm_qta_state(gas_id) ) && /* Not in a QTA gap */
          ( garb_intf_gas_is_xpg_coex_victim(gas_id) ) && /*If peer tech tx active, allow TRM extension in LTA gap */
          ( !l1_fm_data_ptr->is_irat_activity ) && /* Previous activity is not IRAT activity */
#if defined( FEATURE_G2T_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
          (!(l1_is_idle_ta_prepared(gas_id)))&&
#endif
          (trm_reason != TRM_NUM_REASONS) &&
          ( release_required || reserve_required ) &&
          ( gap_to_ccch < L1_MIN_WCDMA_INTER_QTA_GAP) &&
          ( l1_fm_grm_extend_duration( gap_to_ccch + l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].required_frames,FM_PRI_CCCH, gas_id) )
        )
      {
        l1_fm_set_suspend_trm_release( gap_to_ccch, FM_PRI_CCCH, gas_id );
        MSG_GERAN_HIGH_2_G("FN = %d TRM lock is retained for impending CCCH activity in LTA GAP at FN = %d", gl1_get_FN( gas_id ), l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number);
      }

    else
      /******************************************************************************
      Check if the TRM lock needs to be retained for impending SCH activity When it is IN LTA GAP
      *******************************************************************************/
      if( ( gl1_msg_get_multi_sim_standby_mode() && (gl1_get_num_inactive_subs() > 0) ) &&
          (trm_reason != TRM_NUM_REASONS)&&
          ( !gpl1_get_grm_qta_state(gas_id) ) && /* Not in a QTA gap */
          ( garb_intf_gas_is_xpg_coex_victim(gas_id) ) && /*If peer tech tx active, allow TRM extension in LTA gap */
          ( !l1_fm_data_ptr->is_irat_activity ) && /* Previous activity is not IRAT activity */
          (!IS_QTA_REASON(trm_reason, reserve_sub_reason[gas_id]) || gl1_drdsds_enabled(gas_id))&&
          ( release_required || reserve_required ) &&
#if defined( FEATURE_G2T_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
          (!(l1_is_idle_ta_prepared(gas_id)))&&
#endif
          ( gap_to_ncell_sch < L1_MIN_WCDMA_INTER_QTA_GAP) &&
          ( l1_fm_grm_extend_duration( gap_to_ncell_sch + l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].required_frames,FM_PRI_NCELL_SCH, gas_id) )
        )
      {
        l1_fm_set_suspend_trm_release( gap_to_ncell_sch, FM_PRI_NCELL_SCH, gas_id );
        MSG_GERAN_HIGH_2_G("FN = %d TRM lock is retained for impending SCH activity in LTA GAP at FN = %d", gl1_get_FN( gas_id ), l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].reserved_frame_number);
      }
      else
#endif /* FEATURE_QTA */
          if( ( ccch_complete ) &&
          ( release_required || reserve_required ) &&
          ( !l1_fm_data_ptr->is_irat_activity ) && /* Previous activity is not IRAT activity */
          ( gap_to_ccch < L1_MIN_HDR_INTER_GAP)
          /* @@TODO this will go wrong if HDR client also apply TRM-U connected mode priority changes */
          && (TRM_TRAFFIC == grm_get_reason( GRM_HDR, gas_id ))
          && ( l1_fm_grm_extend_duration( gap_to_ccch + l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].required_frames,FM_PRI_CCCH, gas_id) )
        )
      {
        l1_fm_set_suspend_trm_release( gap_to_ccch, FM_PRI_CCCH, gas_id );
        MSG_GERAN_HIGH_2_G("HDR BGtraffic, reselection FN = %d TRM lock is retained for impending CCCH activity at FN = %d", gl1_get_FN( gas_id ), l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number);
      }
    else
      /******************************************************************************
        Check if the TRM lock needs to be retained for impending NBCCH activity
        *******************************************************************************/
      if  (( ccch_complete || nbcch_complete ) &&
          ( release_required || reserve_required ) &&
          ( l1_tskisr_blk->qta_in_progress ) &&
          ( !l1_fm_data_ptr->is_irat_activity ) && /* Previous activity is not IRAT activity */
          ( gap_to_nbcch < L1_MIN_WCDMA_INTER_QTA_GAP) &&
          ( l1_fm_grm_extend_duration( gap_to_nbcch + l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].required_frames, FM_PRI_NCELL_BCCH, gas_id) )
        )
      {
        l1_fm_set_suspend_trm_release( gap_to_nbcch, FM_PRI_NCELL_BCCH, gas_id );
        l1_tskisr_blk->firmware_loaded = FALSE ;
        MSG_GERAN_HIGH_2_G("FN = %d TRM lock is retained for impending NBCCH activity at FN = %d", gl1_get_FN( gas_id ), l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number);
      }
      /******************************************************************************
        Check if the TRM lock needs to be extended for impending xCCH activity, when
        LTE is acquiring NW. This will avoid g sub missing BCCH continuously when it
        always conflicts with LTE acquistion.
        This isn't applied for DR-DSDS as this could cause problems if the initial TRM
        grant was for an RF device that supports the serving band only (e.g. a
        grant for BCCH with no monitors) and we then go ahead and extend for a PCH with
        non-serving band monitors.
      *******************************************************************************/
      else if( (gap_to_ccch != NOT_A_FRAME_NUMBER) &&
               (!gl1_drdsds_enabled(gas_id)) &&
               (release_required || reserve_required) &&
               ( !l1_fm_data_ptr->is_irat_activity ) && /* Previous activity is not IRAT activity */
               (gap_to_ccch < L1_MIN_WCDMA_INTER_QTA_GAP) &&
               (TRM_ACQUISITION == grm_get_reason(LTE_CLIENT_FOR_THIS_SUB(gas_id), gas_id)) &&               
               (l1_fm_grm_extend_duration(gap_to_ccch + l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].required_frames, FM_PRI_CCCH, gas_id))
             )
      {
        l1_fm_set_suspend_trm_release( gap_to_ccch, FM_PRI_CCCH, gas_id );
        MSG_GERAN_HIGH_2_G("LTE ACQ, FN = %d TRM lock is retained for impending CCCH activity at FN = %d",
        gl1_get_FN( gas_id ), l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number);
      }
      else
      /******************************************************************************
      Release or find the next booked activity to reserve with TRM
      *******************************************************************************/
      if ( release_required || reserve_required )
      {
        l1_fm_data_ptr->is_irat_activity = FALSE;
        l1_fm_find_next_task_to_reserve_or_release(gas_id);
      }
    }
  }

  if ( l1_fm_data_ptr->suspend_trm_release )
  {
    MSG_GERAN_HIGH_2_G( "Don't release - suspend_trm_release is set %d for event_pri %d",
                        l1_fm_data_ptr->suspend_trm_release,
                        l1_fm_data_ptr->suspend_trm_release_pri );

    gl1_drx_require_next_tick(gas_id);
    ret_val = FALSE;
  }

//TODO : Ideally we do not want to allow an IDLE abort whilst a TRM/FW cleanup is still
//       pending. However, as things stand various calls to l1_fm_clear_required_frames()
//       are being made on every ISR tick for which an IDLE abort is pending. This makes
//       it impossible to abort IDLE without having a pending FW/TRM transation. It is
//       important to clear all pending TRM reservations before exiting IDLE, however
//       this needs to be done via a SINGLE call to TRM release prior to IDLE abort.
//if(l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)
//{
//   MSG_GERAN_HIGH_0_G("IDLE abort not permitted as FW release is pending");
//   ret_val = FALSE;
//}

#ifdef FEATURE_WLAN_COEX_SW_CXM
  if( ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM ) && !l1_fm_any_activity_running(gas_id) )
  {
    /* inform COEX-WLAN that activity is over */
    garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
  }
#endif /* FEATURE_WLAN_COEX_SW_CXM */

  return ret_val;
}

#ifdef FEATURE_QTA
/*===========================================================================

FUNCTION l1_fm_qta_extension_permitted

DESCRIPTION
  Determines if an on-the-fly extension of a QTA gap can be accommodated
  depending on the activity and current gap length

INPUTS
  duration - number of frames for which the extension is required

DEPENDENCIES
  None

SIDE EFFECTS
  None.

===========================================================================*/
static boolean l1_fm_qta_extension_permitted(uint32 duration, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];

  boolean rc = FALSE;
  uint32 trm_grant_fn = grm_get_trm_grant_fn(l1_tskisr_blk->client_id, gas_id);

  if(SUB_FN(ADD_FN(gl1_get_FN( gas_id ),duration), trm_grant_fn) <= L1_QTA_DURATION_GAP_MAX)
  {
    rc = TRUE;
  }

  return rc;
}

#endif /* FEATURE_QTA */

/*===========================================================================

FUNCTION l1_fm_get_frames_until_trm_booking_expires

DESCRIPTION
  Returns the number of frames available to the current activity before the
  duration booked with TRM expires.

RETURN VALUE
  Number of frames available

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_fm_get_frames_until_trm_booking_expires(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint32 trm_grant_fn = grm_get_trm_grant_fn(l1_tskisr_blk->client_id, gas_id);
  uint32 trm_grant_frames = grm_get_trm_grant_frames(l1_tskisr_blk->client_id, gas_id);

  if((trm_grant_fn == NOT_A_FRAME_NUMBER) ||
       (trm_grant_frames == 0) ||
     (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_GRANTED))
  {
     return 0;
  }
  else
  {
     uint32 trm_expire_fn = ADD_FN(trm_grant_fn, trm_grant_frames);
     if( IS_FRAME_NUM_LATER(trm_expire_fn, gl1_get_FN( gas_id)) )
     {
     return SUB_FN(trm_expire_fn, gl1_get_FN( gas_id ));
  }
     else
     {
       return 0;
     }
  }
}

/*===========================================================================

FUNCTION l1_fm_grm_extend_duration

DESCRIPTION
  Determines if an on-the-fly extension of a TRM grant can be accommodated

INPUTS
  frames_required - number of frames for which the extension is required

DEPENDENCIES
  None

SIDE EFFECTS
  None.

===========================================================================*/
static boolean l1_fm_grm_extend_duration(uint32 frames_required,l1_fm_priority_T pri, gas_id_t gas_id)
{
  idle_data_T               *idle_data_ptr;
  boolean last_two_blocks_in_51_mf;
  uint8 i;
#ifdef FEATURE_GSM_RX_DIVERSITY
  grm_client_enum_t divrx_client_id;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean rc = TRUE;
  gl1_trm_state_t trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
  boolean div_req_reqd = FALSE;

  /*Get the GRM Info for the gas_id*/
  grm_info_t *info_ptr = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id,gas_id);

  /* No need to proceed if we do not have TRM lock */
  if (trm_status != GL1_TRM_GRANTED)
  {
    MSG_GERAN_HIGH_2_G("No TRM lock to extend duration for activity %d for %d frames", pri, frames_required);
    return FALSE;
  }

  if (grm_get_resource_lock_state(gas_id))
  {
    return TRUE;
  }

  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  last_two_blocks_in_51_mf = (42 <= ( GSTMR_GET_FN_GERAN(gas_id) % 51)) ;

#ifdef FEATURE_GSM_RX_DIVERSITY
  divrx_client_id =  grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

  if((GRM_NO_CLIENT != divrx_client_id)
     && gl1_get_IMRD_ctl_nv(gas_id) 
     && gpl1_grm_rx_sec_supported_reason(pri)
     && (GL1_TRM_GRANTED == grm_get_trm_status(divrx_client_id, gas_id)))
  {
    div_req_reqd = TRUE;
  }
#else
   div_req_reqd = DIVERSITY_FALSE;  
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  if (l1_idle_data_store[gas_id].g_bplmn_running_state == BPLMN_ACTIVE && (FALSE == idle_data_ptr->mm_non_drx_mode))
  {
    /* Dont extend for sc activity when BPLMN running
     */
    grm_set_trm_status(GL1_TRM_NOT_GRANTED, l1_tskisr_blk->client_id, gas_id);
    grm_set_trm_grant_fn(NOT_A_FRAME_NUMBER, l1_tskisr_blk->client_id, gas_id);
    grm_set_trm_grant_frames(0, l1_tskisr_blk->client_id, gas_id);
    return FALSE;
  }
  if((idle_data_ptr->mm_non_drx_mode)&&
      (idle_data_ptr->non_updated_lai_in_nmo1== FALSE)&&
      (gl1_is_geran_tuneaway_allowed(gas_id))&&
      ( trm_status == GL1_TRM_GRANTED )
   )
  {
     MSG_GERAN_HIGH_0_G("IDLE TA: grm_extend_duration() not required in MM NON DRX mode:");
     return TRUE;
  }
#ifdef FEATURE_QTA
  /* last line of defense belt and braces to stop on the fly grm_extend_duration() occuring in QTA. If this doesn't fit
     into the frames that remain for the current TRM reservation then it will be denied. */
  if((l1_tskisr_blk->qta_in_progress) && (!l1_fm_qta_extension_permitted(frames_required, gas_id)))
  {
    /* only a single activity can run in the QTA gap - the TRM reservation cannot be extended */
    grm_set_trm_status(GL1_TRM_NOT_GRANTED, l1_tskisr_blk->client_id, gas_id);
    grm_set_trm_grant_fn(NOT_A_FRAME_NUMBER, l1_tskisr_blk->client_id, gas_id);
    grm_set_trm_grant_frames(0, l1_tskisr_blk->client_id, gas_id);

    MSG_GERAN_HIGH_0_G("grm_extend_duration() denied - QTA in progress");

    rc = FALSE;
  }
  else
#endif /* FEATURE_QTA */
  if( (last_two_blocks_in_51_mf) &&
           (idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING) &&
           (!idle_data_ptr->first_ext_pch_block_read) )
  {
    /* The extra check for last_two_block_in_51_mf is done, considering the following scenario :
     * Consider the first page block of an extended page falls on last block of 51 MF boundary.
     * It would imply the extended page would be on FN 12-15 of next MF, if BCCH Ext is not present
     * or on FN 16-19, if BCCH ext is present. In this case, we would want to release the lock
     * and make a fresh reservation for the extended page block, otherwise this would imply retaing
     * the lock for a long duration of time. Similar thing holds good for the first page block of an
     * extended paging falling on FN 42-45
     */
    /* We are in extended paging mode with current FN falling between FN 42-49 and this is the first
     * of the two blocks of extended PCH.
     */
    grm_set_trm_status(GL1_TRM_NOT_GRANTED, l1_tskisr_blk->client_id, gas_id);
    grm_set_trm_grant_fn(NOT_A_FRAME_NUMBER, l1_tskisr_blk->client_id, gas_id);
    grm_set_trm_grant_frames(0, l1_tskisr_blk->client_id, gas_id);

     MSG_GERAN_HIGH_0_G( "grm_extend_duration() denied - Second block of extended paging falling on last 2 blocks of 51MF" );

    rc = FALSE;
  }
  else
  {
    trm_duration_t duration = gpl1_multi_sim_convert_frames_to_sclcks( frames_required );
    boolean change_reason = FALSE;
    boolean extend_duration = FALSE;
    boolean trm_band_req_ok = TRUE;
    boolean res_lock        = grm_get_resource_lock_state(gas_id);
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    trm_reason_enum_t reason = l1_fm_map_pri_to_trm(pri, gas_id);
    trm_reason_enum_t grm_current_reason = grm_gl1_get_current_trm_reason(l1_tskisr_blk->client_id, gas_id);

    /*Only change reason or extend duration if we have grant from TRM*/
    if (trm_status == GL1_TRM_GRANTED)
    {
       boolean band_reg_reqd = FALSE;
       ARFCN_T serving_cell  = gl1_get_serving_cell_arfcn(gas_id);
       sys_band_T other_band = serving_cell.band;
       /* This is required here because on BO if FM extends then its fine ncell */
       switch (pri) 
       {
         case FM_PRI_NCELL_BCCH:
           other_band = gl1_map_sys_band_to_gsm_band(grm_get_trm_band_from_g_pri(pri, gas_id));
           band_reg_reqd = TRUE;
           break;
         case FM_PRI_NCELL_ACQ:
         case FM_PRI_NCELL_SCH:
           other_band = l1_sc_globals_ptr->receiving_cell->bs.arfcn.band;
           band_reg_reqd = TRUE;
           break;
          case FM_PRI_LTE_IDSEARCH:
            trm_band_req_ok = TRUE;
            break;
         case FM_PRI_CCCH:
            /* 
             * In case of Non Drx Mode and Paging Re-org, Req will be only 
             * for CCCH and no Reg is reqd 
             * It also overflows the Band Registration Buffer 
             */ 
           if (!idle_data_ptr->mm_non_drx_mode && !idle_data_ptr->paging_data.all_paging_blocks )
           {
             band_reg_reqd = TRUE; 
           }
         default:
           break;
       }/*Switch Case*/
	   
       if(band_reg_reqd && info_ptr)
       {
         /* if num_bands = 0, it means earlier band used was serving_cell's only */
         if((info_ptr->grm_freq_info.num_bands == 0) && (other_band == serving_cell.band))
         { 
            band_reg_reqd = FALSE;	  
            MSG_GERAN_HIGH_1_G( "No need to do IFS as serving cell band %d is already registered", other_band );
         
         }
         else
         {
            for(i=0 ; (i < info_ptr->grm_freq_info.num_bands) && (i < MAX_BAND_INFO) ; i++)
            {
               if (gl1_map_gsm_band_to_sys_band(other_band) == info_ptr->grm_freq_info.bands[i].band)
               {
                   band_reg_reqd = FALSE;
                   MSG_GERAN_HIGH_1_G( "No need to do IFS as the extended activity band %d is already registered", other_band);
                   break;
               }
            }
         }
       }
	  
       if (band_reg_reqd)
       {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
             trm_band_req_ok = grm_check_ifs_band_reg(SC_FCH_SCH, 
                                                      other_band,
                                                      frames_required,
                                                      div_req_reqd,
                                                      gas_id );

#endif
       }/*if band reg reqd*/


      if (trm_band_req_ok)
      {
        if ( (grm_current_reason == TRM_IRAT_MEASUREMENT) && (reason != TRM_IRAT_MEASUREMENT)  )
        {
          /* Here we have requested and been granted for IRAT procedure but now want to
           * change to a non IRAT procedure. This happens for example when GL1 receives
           * a SYS_INFO_REQ during Warmup where the SI read now collides with IRAT 
           * procedure. In this scenario because G has reserved and been granted with 
           * X tech band GL1 needs to relinquish trm grant and not continue with the 
           * grm change priority process. 
           */ 
          MSG_GERAN_HIGH_1_G("Changing reason from IRAT_MEASUREMENT to %d Denied!",reason);
        }
        else if ((grm_current_reason == TRM_BG_TRAFFIC) && (reason ==TRM_CHANNEL_MAINTENANCE_INV) && ((l1_tskisr_blk->current_params.L1Data.pIdle_data->mm_non_drx_mode ) ||
                 (l1_tskisr_blk->current_params.L1Data.pIdle_data->paging_data.all_paging_blocks)))
        {
          /* We have changed the reason to TRM_BG_TRAFFIC to allow the tuneaways in Reorg mode,
           * so while extending the lock no need to do the change prio, just set the change_reason
           * to TRUE to request for extending the lock
           */
          change_reason = TRUE;
        }
        else
        {
          /*Change Reason to be Forced to TRUE in case of Resource Lock Active and is done prior to Band Reg.*/
        change_reason = (res_lock || 
                        grm_change_priority( l1_tskisr_blk->client_id, reason, reserve_sub_reason[gas_id], gas_id ));
        }
      }/*if trm band reg ok*/

#ifdef FEATURE_G2X_REORG_TUNEAWAY
      if ((change_reason)&&(l1_tskisr_blk->current_params.L1Data.pIdle_data->paging_data.all_paging_blocks)&& 
           gl1_is_geran_tuneaway_allowed(gas_id))
      {
        MSG_GERAN_HIGH_0_G("IDLE TA ::grm_extend_duration() not required - in Page ReOrg mode");
        return TRUE;
      }
#endif /* FEATURE_G2X_REORG_TUNEAWAY */

      if ( change_reason )
      {
        if (l1_idle_data_store[gas_id].mm_non_drx_mode)
        {
          /* re-evaluate duation when in non drx mode
           * To account for the possibilty of single burst decode 
           * need 9 frames for the duration for worse case 
           *        C|C|C|C|F|S|C|C|C|C|C|C etc.. 
           *  SBDecoded|8|7|6|5|4|3|2|1|Decoded
           */
          duration = gpl1_multi_sim_convert_frames_to_sclcks( NON_DRX_FRAME_DURATION );

        }/*if non drx mode*/

        if( res_lock || grm_extend_duration( l1_tskisr_blk->client_id, duration, gas_id ))
        {
          uint32 trm_grant_frames =  grm_get_trm_grant_frames(l1_tskisr_blk->client_id, gas_id);
          grm_set_trm_grant_frames((trm_grant_frames + frames_required), l1_tskisr_blk->client_id, gas_id);

          MSG_GERAN_HIGH_2_G("grm_extend_duration() - %d frames Resource Lock - %d",frames_required, res_lock);
          extend_duration = TRUE;
#ifdef FEATURE_PBR_RANDOMIZATION
          /* if the previous grant was for DEMOD PAGE, we sent grant event for this wakeup ID, dont send it again */		  
          if( ( reason == TRM_DEMOD_PAGE ) && (grm_current_reason != TRM_DEMOD_PAGE))
          {
            grm_inform_grant_event(l1_tskisr_blk->client_id,
                                   gl1_ms_get_trm_resource(gas_id, TRM_DEMOD_PAGE),
                                   duration,
                                   TRM_DEMOD_PAGE,
                                   TRM_GRANTED_CHAIN0
                                  );
          }
#endif
#ifdef FEATURE_GSM_RX_DIVERSITY
          if (div_req_reqd)
          {
            /* Check for Extension of diversity chain as well 
              Pass duration less than PRx to avoid race conditions of DivRx conflicting with other tech.
              In DSDS we need to avoid such denials as DRx only anyway is not useful for other sub */
            if( !grm_extend_duration( divrx_client_id, (duration-SCLKS_PER_MS), gas_id ) )
            {
              gl1_hw_handle_idle_rxd_rf_fw_exit( divrx_client_id , gas_id );
            }
          }/*if div req reqd*/
#endif /*FEATURE_GSM_RX_DIVERSITY*/
        }/*if res lock || grm extend duration*/
      }/*if change reason */
    }/*if trm status granted*/

    if(!change_reason || !extend_duration)
    {
#ifdef FEATURE_PBR_RANDOMIZATION 
      /* if the previous grant was for DEMOD PAGE, we sent grant event for this wakeup ID, dont send it again */			  
      if( ( reason == TRM_DEMOD_PAGE ) && ( grm_current_reason != TRM_DEMOD_PAGE ) )
      {
         grm_inform_grant_event(l1_tskisr_blk->client_id,
                                gl1_ms_get_trm_resource(gas_id, TRM_DEMOD_PAGE),
                                duration,
                                TRM_DEMOD_PAGE,
                                TRM_DENIED
                                );
      }
#endif
      grm_set_trm_status(GL1_TRM_NOT_GRANTED, l1_tskisr_blk->client_id, gas_id);
      grm_set_trm_grant_fn(NOT_A_FRAME_NUMBER, l1_tskisr_blk->client_id, gas_id);
      grm_set_trm_grant_frames(0, l1_tskisr_blk->client_id, gas_id);
      grm_set_trm_grant_status(TRM_DENIAL, l1_tskisr_blk->client_id, gas_id);

      MSG_GERAN_HIGH_1_G("grm_extend_duration() - %d frames denied",frames_required);
      rc = FALSE;
    }

  }/*End of else*/

  return rc;
}

/*===========================================================================

FUNCTION l1_fm_suppress_grm_reserve_at

DESCRIPTION
  Determines if a TRM reserve at should be suppressed due to duplicated
  Do not supress if TRM was in use

DEPENDENCIES
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static boolean l1_fm_suppress_grm_reserve_at(l1_fm_priority_T pri,trm_reason_enum_t reason, uint32 required_frame_nbr, uint32 frames_required, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK  *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  gl1_trm_state_t trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

  boolean rc = ( (l1_fm_data_ptr->trm_reserved_pri      != pri) ||
  	             (l1_fm_data_ptr->trm_reserved_reason   != reason) ||
                 (l1_fm_data_ptr->trm_reserved_fn       != required_frame_nbr) ||
                 (l1_fm_data_ptr->trm_reserved_duration != frames_required)    ||
                 ( (trm_status == GL1_TRM_GRANTED) && (l1_tskisr_blk->firmware_load_status != FW_PENDING_RELEASE)) ||
                 (trm_status == GL1_TRM_ABORTING) );

  return !rc;
}

/*===========================================================================

FUNCTION l1_fm_is_task_bookable_before_high_priority

DESCRIPTION
  The function check if a lower priority task can be booked before a higher
  priority task

INPUTS
  Low priority to book, high priority booked task

DEPENDENCIES
  None

RETURN VALUE
  TRUE if there are enough free frames before the high priority task to run
  the low priority task, FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/
static boolean l1_fm_is_task_bookable_before_high_priority( l1_fm_priority_T current_task,
                                                            l1_fm_priority_T high_priority_task,
                                                            gas_id_t gas_id )
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  uint8 overhead = 0; 

  /*If a valid high priority task isnt set, then return TRUE, No need to check further*/
  if (high_priority_task == FM_PRI_RESERVED)
  {
	  return TRUE;
  }

  /* If CCCH is high priority task then need to take 1 frame into account for reserving next task */
  if( high_priority_task == FM_PRI_CCCH )
  {
    /* Offset of 1 frame is required to check if this FM task
     * is allowed w.r.t. CCCH. When l1_fm_are_frames_available() is made
     * FM allows the access only if the required_frames is less
     * than or equal to the gap_to_ccch_activity
     */
    overhead = 1;
  }

  if ( l1_fm_is_a_pri_to_apply_overhead(current_task))
  {
    /* Minimum period between consecutive TA gaps */
    overhead += ( ( garb_intf_gas_is_xpg_coex_victim(gas_id) && !gl1_drdsds_enabled(gas_id) ) ? QTA_OVERHEAD : 0 );
  }

  /* Check that the low priority task is required to run before the high priority task */
  if (IS_FRAME_NUM_LATER(l1_fm_data_ptr->l1_fm_table[high_priority_task].reserved_frame_number,
        l1_fm_data_ptr->l1_fm_table[current_task].reserved_frame_number))
  {
    /* Here the task is requesting for frames at a frame number that is lower than the FN at which a
     * higher priority is scheduled to run. Make sure that there is enough room for the low priority
     * task to complete before the high priority; else do not book the low priority task
     */
    if ((SUB_FN(l1_fm_data_ptr->l1_fm_table[high_priority_task].reserved_frame_number,
                l1_fm_data_ptr->l1_fm_table[current_task].reserved_frame_number))
        >
        ( l1_fm_data_ptr->l1_fm_table[current_task].required_frames + overhead ))
    {
        return TRUE;
    }
  }
  return FALSE;
}

/*===========================================================================

FUNCTION l1_fm_is_a_pri_to_apply_overhead

DESCRIPTION
  
  Prevent low priority QTA actvity running, if a higher
  priority QTA activity would be denied due to the inability to setup back
  to back QTA gaps i.e. low priority QTA actvity but butts up to a higher
  priority QTA activity.
 
  This functions returns true if passed in current_task is one of the listed
  activities that QTA should be denied when butting up to a higher
  priority QTA activity.
 

INPUTS
  gas_id

DEPENDENCIES
  None

RETURN VALUE
  TRUE passed in current_task is one of the listed activities
  FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/
static boolean l1_fm_is_a_pri_to_apply_overhead( l1_fm_priority_T current_task )
{
  if ((current_task == FM_PRI_NCELL_SCH) || 
      (current_task == FM_PRI_NCELL_ACQ) || 
      (current_task == FM_PRI_MSIM_CBCH) )
  {
   return TRUE;
  }
  return FALSE;
}

boolean l1_fm_prioritise_nbcch_over_ccch(gas_id_t gas_id)
{
  static uint32 last_missed_page[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(NOT_A_FRAME_NUMBER)};	
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

  if( l1_fm_get_running_activity(gas_id) == FM_PRI_NCELL_BCCH )
  {
    return TRUE;
  }

  if ( L1_background_HPLMN_search_in_progress(gas_id) )
  {
     l1_sc_bcch_reset_high_priority_flag(gas_id);
     return FALSE;
  }
  if( l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number == NOT_A_FRAME_NUMBER ||
      l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number == NOT_A_FRAME_NUMBER )
  {
    return FALSE;
  }

  if (IS_FRAME_NUM_LATER(l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number,
                         l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number))
  {
    /* Here the task is requesting for frames at a frame number that is higher than the FN at which a
        * higher priority is scheduled to run. Make sure that there is enough room for the high priority
        * task to complete; else do not book the low priority task
        */
    if ((SUB_FN(l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number,
              l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number))
       >
       l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].required_frames + 1)
    {
       return TRUE;
    }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    else if( (l1_tsk_buffer[gas_id].reservation_for_bcch == FALSE) &&
             (idle_data_ptr->paging_data.paging_mode == NORMAL_PAGING) )
    {
       l1_sc_bcch_set_high_priority_flag(gas_id);
  
       return TRUE;
    }
#endif
  } else if (IS_FRAME_NUM_LATER_OR_EQUAL(l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number,
                                         l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number))

  {

    /* Here the task is requesting for frames at a frame number that is earlier than the FN at which a
        * higher priority is scheduled to run. prioritise NBCCH if high priority flag is set and this is not a
        * conflict with serving cell BCCH
        */
    if ((SUB_FN(l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number,
              l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number))
    >
       l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].required_frames)
    {
       return FALSE;
    }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    else if( (l1_tsk_buffer[gas_id].reservation_for_bcch == FALSE) &&
             (idle_data_ptr->paging_data.paging_mode == NORMAL_PAGING) )
    {
       l1_sc_bcch_set_high_priority_flag(gas_id);

      if(gl1_msg_get_multi_sim_mode())
      {   
        gl1_idle_release_currect_ccch_drx_tick(gas_id);
        /* send missed page indication to RR */
	if(last_missed_page[gas_id] != l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number)
	{
              L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
             last_missed_page[gas_id] = l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number;
        }
       /* Clear required frames for suppressed CCCH */
        l1_fm_clear_required_frames( FM_PRI_CCCH, gas_id );
      }
       return TRUE;
    }
#endif
  }

  return FALSE;
}
#endif /* ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) */


/*===========================================================================

FUNCTION l1_fm_get_active_CBCH_pri

DESCRIPTION
  This will return the active FM priority for the CBCH activity
When X tech in BG Traffic, then CB will be default to low prio.

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  Active FM priortiy for CBCH.

SIDE EFFECTS
  None.

===========================================================================*/
l1_fm_priority_T l1_fm_get_active_CBCH_pri( gas_id_t gas_id )
{

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
   if (gl1_msg_get_multi_sim_mode() && 
       (garb_intf_gas_is_xpg_coex_victim(gas_id) ||
        !GONLY_OPERATION_IS_ACTIVE())
       )
   {
      cbch_active_pri[gas_id] = FM_PRI_MSIM_CBCH;
    }
#endif

   return (cbch_active_pri[gas_id]);

}
/*===========================================================================

FUNCTION l1_fm_set_CBCH_pri

DESCRIPTION
  This will store the requested CBCH Pri from RR into Pending info.
  Which will later be applied on next TC=0 occassion

INPUTS
  None

DEPENDENCIES
  

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/

void l1_fm_set_CBCH_pri(boolean high_prio, gas_id_t gas_id)
{
  if (high_prio)
  {
    cbch_pending_pri[gas_id] = FM_PRI_CBCH;
  }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) 
  else
  {
    cbch_pending_pri[gas_id] = FM_PRI_MSIM_CBCH;  
  }
#endif
  MSG_GERAN_HIGH_1_G("new Pending CBCH prio:%d", cbch_pending_pri[gas_id]);
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) 
/*===========================================================================

FUNCTION l1_fm_toggle_CBCH_pri

DESCRIPTION
  This function will toggle the CBCH priority and store in pending priority
  It will be used in next CBCH cycle startion with TC=0. This functioin shall
  be called after setting priority for current CBCH cycle.i.e, after calling 
  l1_fm_updated_CBCH_pri()
  
INPUTS
  None

DEPENDENCIES
  

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void l1_fm_toggle_CBCH_pri(gas_id_t gas_id)
{
  if (cbch_active_pri[gas_id] == FM_PRI_MSIM_CBCH)
  {
    cbch_pending_pri[gas_id] = FM_PRI_CBCH;
  }
  else if (cbch_active_pri[gas_id] == FM_PRI_CBCH)
  {
    cbch_pending_pri[gas_id] = FM_PRI_MSIM_CBCH;
  }
  MSG_GERAN_HIGH_1_G("New Pending Toggled CBCH prio:%d", cbch_pending_pri[gas_id]);
}
#endif

/*===========================================================================

FUNCTION l1_fm_updated_CBCH_pri

DESCRIPTION
  This will Update the current CBCH Priority to be used.
  
INPUTS
  None

DEPENDENCIES
  

RETURN VALUE
  Active FM priortiy for CBCH.
  When X tech in BG Traffic, then CB will be default to low prio.

SIDE EFFECTS
  None.

===========================================================================*/

l1_fm_priority_T l1_fm_updated_CBCH_pri(gas_id_t gas_id)
{
  if (l1_smscb_use_high_priority(gas_id))
  {
    /*If Scheduling in CB DRX Mode, then always use High Pri CBCH read*/
    cbch_active_pri[gas_id] = FM_PRI_CBCH;
    cbch_pending_pri[gas_id] = FM_PRI_CBCH;
  }

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM) 
  if (gl1_msg_get_multi_sim_mode() && 
      (garb_intf_gas_is_xpg_coex_victim(gas_id) ||
       !GONLY_OPERATION_IS_ACTIVE()))
  {
     /*if X-tech is in traffic or in X+G Mode, CBCH must be with lower prio*/
    cbch_active_pri[gas_id] = FM_PRI_MSIM_CBCH;
    cbch_pending_pri[gas_id] = FM_PRI_MSIM_CBCH;     
  }
#endif

  if ((cbch_active_pri[gas_id] != cbch_pending_pri[gas_id]) &&
      (cbch_pending_pri[gas_id] != FM_PRI_MAX))
  {
    MSG_GERAN_HIGH_1_G("Using New CBCH prio:%d", cbch_pending_pri[gas_id]);
    cbch_active_pri[gas_id] = cbch_pending_pri[gas_id];
  }
  return cbch_active_pri[gas_id];
}

/*===========================================================================

FUNCTION  L1_FM_GET_REQUIRED_FRAMES_TO_COMPLETE

DESCRIPTION
  This function returns the number of frames required for the current running task to complete.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS


===========================================================================*/
uint32 l1_fm_get_required_frames_to_complete(l1_fm_priority_T pri,gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];

  if (l1_fm_data_ptr->l1_fm_table[pri].state == FM_STATE_NULL)
  {
      return 0;
  }
  else
  {
      return (l1_fm_data_ptr->l1_fm_table[pri].required_frames + FRAMES_REQUIRED_FM_ABORT);
  }

}

/******************************************************************************
 *
 *  Function name:  l1_clear_inactive_cbch_entries
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 *  Clears any inactive CBCH bookings from the Frame Manager
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
void l1_clear_inactive_cbch_entries(gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  if (l1_fm_data_ptr->l1_fm_table[FM_PRI_CBCH].state == FM_STATE_NULL)
  {
     l1_fm_clear_required_frames( FM_PRI_CBCH, gas_id );
  }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if (l1_fm_data_ptr->l1_fm_table[FM_PRI_MSIM_CBCH].state == FM_STATE_NULL)
  {
    l1_fm_clear_required_frames( FM_PRI_MSIM_CBCH, gas_id );
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}

/*===========================================================================

FUNCTION l1_fm_get_running_activity

DESCRIPTION
  This function looks through the frame manger table to cycle through the tasks
  and returns the current running activity

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  l1_fm_priority_T - FM_PRI_RESERVED if no activities are currently running

SIDE EFFECTS
  None.

===========================================================================*/
l1_fm_priority_T l1_fm_get_running_activity(gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  byte i;

  for (i=0; i<FM_PRI_MAX; i++)
  {
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
    if ( l1_fm_data_ptr->l1_fm_table[i].state != FM_STATE_NULL )
    {
      return i;
    }
#else
    if(l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number != NOT_A_FRAME_NUMBER)
    {
      return i;
    }
#endif
  }

  return FM_PRI_RESERVED;
}
/*===========================================================================
FUNCTION gl1_check_if_xcch_activity

DESCRIPTION
  Checks if current activity is xcch or not.

===========================================================================*/
boolean gl1_check_if_current_activity_is_xcch(gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  return (l1_fm_get_running_activity(gas_id) == FM_PRI_CCCH);
#else
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];

  return (l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number != NOT_A_FRAME_NUMBER);
#endif
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION l1_fm_any_activity_running

DESCRIPTION
  This function looks through the frame manger table to cycle through the tasks
  to ensure that none are still running and in need of the TRM lock or FW.

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  boolean returns TRUE = One or more activities are still running
  boolean returns FALSE = No activities are still running

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_fm_any_activity_running(gas_id_t gas_id)
{
  return(l1_fm_get_running_activity(gas_id)!=FM_PRI_RESERVED);
}
#else

/*===========================================================================

FUNCTION  l1_fm_pri_is_earlier_and_non_conflicting

DESCRIPTION
  This function determines if a lower priority activity (pri 1) is earlier
  and not in conflict with a higher priority activity (pri 2). This is used
  to help determine which activity will run next.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the activities are in conflict

SIDE EFFECTS
  None

===========================================================================*/
static boolean l1_fm_pri_is_earlier_and_non_conflicting(l1_fm_priority_T pri_1, l1_fm_priority_T pri_2, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  boolean rc = FALSE;

  if (l1_fm_data_ptr->l1_fm_table[pri_1].reserved_frame_number != NOT_A_FRAME_NUMBER)
  {
    if (IS_FRAME_NUM_LATER(l1_fm_data_ptr->l1_fm_table[pri_2].reserved_frame_number,
                           l1_fm_data_ptr->l1_fm_table[pri_1].reserved_frame_number) )
    {
      if ((SUB_FN(l1_fm_data_ptr->l1_fm_table[pri_2].reserved_frame_number,
                  l1_fm_data_ptr->l1_fm_table[pri_1].reserved_frame_number))
           >      l1_fm_data_ptr->l1_fm_table[pri_1].required_frames )
      {
         rc = TRUE;
      }
    }
  }

  return rc;
}

#endif /* FEATURE_DUAL_SIM */

#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION  l1_fm_get_next_activity

DESCRIPTION
  This function gets the FN and type of the next activity to run. If an
  activity is in progress then this will pass back the next FN.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
l1_fm_priority_T l1_fm_get_next_activity(l1_fm_next_activity_T* next_activity, gas_id_t gas_id)
{
  l1_fm_priority_T pri = l1_fm_get_running_activity(gas_id);
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];

  if(pri!=FM_PRI_RESERVED)
  {
    next_activity->fn = ADD_FN(gl1_get_FN(gas_id), 1);
  }
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  else if(l1_fm_data_ptr->trm_reserved_fn!=NOT_A_FRAME_NUMBER)
  {
    next_activity->fn = l1_fm_data_ptr->l1_fm_table[l1_fm_data_ptr->trm_reserved_pri].reserved_frame_number;
    next_activity->duration = l1_fm_data_ptr->l1_fm_table[l1_fm_data_ptr->trm_reserved_pri].required_frames;
    next_activity->activity = l1_fm_data_ptr->trm_reserved_pri;
    next_activity->ustmr_s_time = l1_fm_data_ptr->l1_fm_table[l1_fm_data_ptr->trm_reserved_pri].ustmr_s_time;
    next_activity->ustmr_e_time = l1_fm_data_ptr->l1_fm_table[l1_fm_data_ptr->trm_reserved_pri].ustmr_e_time;
  }
#else /* defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM) */
  else
  {
    /* FM_PRI_CCCH - If there is a booking for CCCH then set this as the next activity */
    if (l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number != NOT_A_FRAME_NUMBER)
    {
      next_activity->fn = l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number;
      next_activity->duration = l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].required_frames;
      next_activity->ustmr_s_time = l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].ustmr_s_time;
      next_activity->ustmr_e_time = l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].ustmr_e_time;
      next_activity->activity = FM_PRI_CCCH;
    }

    /* FM_PRI_NCELL_BCCH - If there is an earlier and non-conflicting booking for NCELL BCCH then set this as the next activity */
    if(l1_fm_pri_is_earlier_and_non_conflicting(FM_PRI_NCELL_BCCH, pri, gas_id))
    {
      next_activity->fn = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].reserved_frame_number;
      next_activity->duration = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].required_frames;
      next_activity->ustmr_s_time = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].ustmr_s_time;
      next_activity->ustmr_e_time = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_BCCH].ustmr_e_time;
      next_activity->activity = FM_PRI_NCELL_BCCH;
    }

    /* FM_PRI_NCELL_ACQ - If there is an earlier and non-conflicting booking for NCELL ACQ then set this as the next activity */
    if(l1_fm_pri_is_earlier_and_non_conflicting(FM_PRI_NCELL_ACQ, pri, gas_id))
    {
      next_activity->fn = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_ACQ].reserved_frame_number;
      next_activity->duration = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_ACQ].required_frames;
      next_activity->ustmr_s_time = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_ACQ].ustmr_s_time;
      next_activity->ustmr_e_time = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_ACQ].ustmr_e_time;
      next_activity->activity = FM_PRI_NCELL_ACQ;
    }

    /* FM_PRI_NCELL_SCH - If there is an earlier and non-conflicting booking for NCELL SCH then set this as the next activity */
    if(l1_fm_pri_is_earlier_and_non_conflicting(FM_PRI_NCELL_SCH, pri, gas_id))
    {
      next_activity->fn = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].reserved_frame_number;
      next_activity->duration = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].required_frames;
      next_activity->ustmr_s_time = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].ustmr_s_time;
      next_activity->ustmr_e_time = l1_fm_data_ptr->l1_fm_table[FM_PRI_NCELL_SCH].ustmr_e_time;
      next_activity->activity = FM_PRI_NCELL_SCH;
    }
  }
#endif /* defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM) */

  return next_activity->activity;

}

/*===========================================================================

Function
  l1_fm_get_active_pri

Description
  gets the priority of the running activity, which is currently running

Returns
  frame manager priority of running activty

=============================================================================*/
l1_fm_priority_T l1_fm_get_active_pri(gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  if(l1_fm_data_ptr->l1_fm_table[l1_fm_data_ptr->fm_active_pri].state == FM_STATE_USING_FRAMES)
    return l1_fm_data_ptr->fm_active_pri;
  else
    return FM_PRI_RESERVED;
}

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
/*===========================================================================

FUNCTION  l1_fm_reset_pri_res

DESCRIPTION
  This Function resets the reserve fn and activity data in FM

DEPENDENCIES
  None

RETURN VALUE
 Void

SIDE EFFECTS
  None

===========================================================================*/

void l1_fm_reset_pri_res(gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];

  MSG_GERAN_MED_4_G( "l1_fm_reset_pri_res: Reset FM Data for reserved activity:%d with reason:%d at FN:%d for duration:%d",
         l1_fm_data_ptr->trm_reserved_pri , l1_fm_data_ptr->trm_reserved_reason,
         l1_fm_data_ptr->trm_reserved_fn, l1_fm_data_ptr->trm_reserved_duration);

  l1_fm_data_ptr->trm_reserved_fn       = 0;
  l1_fm_data_ptr->trm_reserved_pri      = FM_PRI_RESERVED;
  l1_fm_data_ptr->trm_reserved_reason   = TRM_NUM_REASONS;
  l1_fm_data_ptr->trm_reserved_duration = 0;

}



/*===========================================================================

Function
  l1_fm_are_bplmn_frames_available

Description
  based on l1_fm_are_frames_available but soley used for checking the BPLMN activity

Returns
  True if activity can run
  False if activity cant run

=============================================================================*/
boolean l1_fm_are_bplmn_frames_available(
                                    uint32 frames_required,
                                    void ( *abort_callback )( void*, gas_id_t gas_id ),
                                    void* abort_data, gas_id_t gas_id )
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  uint32 i;
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];

  l1_fm_current_activity[gas_id] = FM_PRI_BACKGROUND_PLMN;

  MSG_GERAN_HIGH_2_G("Activity bplmn requests %d frames at FN=%d", frames_required, gl1_get_FN(gas_id));

  /* if higher priority tasks are running, then no frames can
   * be provided.
   */
  for (i = 0; i < FM_PRI_BACKGROUND_PLMN; i++)
  {
    if (l1_fm_data_ptr->l1_fm_table[i].state != FM_STATE_NULL)
    {
      MSG_GERAN_MED_2_G("Frame Manager:HP activity %d running!..No frames for BPLMN (FN:%d)", i, gl1_get_FN(gas_id));
      l1_fm_clear_required_frames(FM_PRI_BACKGROUND_PLMN, gas_id);

      return FALSE;
    }
  }

  /* Check whether the number of frames required will clash with the receive operation
   * of a higher priority task (scheduled for sometime in the near future). In that case,
   * it is advisable not to schedule the lower priority task in the first place. This will
   * save us the trouble of aborting this task anyway.
   */
  for (i = 0; i < FM_PRI_BACKGROUND_PLMN; i++)
  {

    if (l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number != NOT_A_FRAME_NUMBER)
    {
      if (IS_FRAME_NUM_LATER(l1_fm_data_ptr->l1_fm_table[FM_PRI_BACKGROUND_PLMN].reserved_frame_number, l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number))
      {
        /* Here the task is requesting for frames at a frame number that is higher than the FN at which a
         * higher priority is scheduled to run. Make sure that there is enough room for the high priority
         * task to complete; else do not schedule the low priority task
         */
        if ((SUB_FN(l1_fm_data_ptr->l1_fm_table[FM_PRI_BACKGROUND_PLMN].reserved_frame_number, l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number))
            < l1_fm_data_ptr->l1_fm_table[i].required_frames)
        {
          MSG_GERAN_HIGH_2_G("Frame Manager:HP activity %d will collide with activity bplmn!(FN:%d)", i, gl1_get_FN(gas_id));

          l1_fm_clear_required_frames(FM_PRI_BACKGROUND_PLMN, gas_id);

          return FALSE;
        }
      } else
      {
        /* Here the task is requesting for frames at a frame number that is lower than the FN at which a
         * higher priority is scheduled to run. Make sure that the low priority task will be able to complete
         * before the high priority  task can run; else do not schedule the low priority task
         */
        if ((SUB_FN(l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number, l1_fm_data_ptr->l1_fm_table[FM_PRI_BACKGROUND_PLMN].reserved_frame_number))
            < l1_fm_data_ptr->l1_fm_table[FM_PRI_BACKGROUND_PLMN].required_frames)
        {

          MSG_GERAN_HIGH_2_G("Frame Manager:HP activity %d will collide with bplmn activity!(FN:%d)",
                             i, gl1_get_FN(gas_id));

          l1_fm_clear_required_frames(FM_PRI_BACKGROUND_PLMN, gas_id);
          return FALSE;

        }
      }
    }
  }
  /* Work down table and abort any running lower priorty task.
   *
   */
  for (i = FM_PRI_BACKGROUND_PLMN + 1; i < FM_PRI_MAX; i++)                /*lint !e641 suppress "Converting enum to int"*/
  {
    if (l1_fm_data_ptr->l1_fm_table[i].state == FM_STATE_USING_FRAMES)
    {


      MSG_GERAN_HIGH_3_G("Frame Manager:Aborting LP activity %d!(FN:%d,CCCH gap:%d)",
                         i, gl1_get_FN(gas_id), l1_fm_data_ptr->gap_to_ccch_activity);

      l1_fm_data_ptr->l1_fm_table[i].state = FM_STATE_CB_PENDING;

#ifdef FRAME_MANAGER_LOGGING
      l1_fm_log_event((l1_fm_priority_T)i, EVE_ABORT_LP_TASK, gas_id);
#endif

      l1_fm_data_ptr->l1_fm_table[i].abort_callback(l1_fm_data_ptr->l1_fm_table[i].abort_data, gas_id);

      /* Check here if we get an immediate abort callback.
       * If yes, then frames can be provided to requesting task.
       * If FM_PRI_BACKGROUND_PLMN is attempting to run then dont allow to program
       * this frame as may clash with a sync rx even though frames have been cleared.
       * FM_PRI_BACKGROUND_PLMN will still attempt to run next frame so should be OK
       */
      if ((l1_fm_data_ptr->l1_fm_table[i].state != FM_STATE_NULL))
      {
        return FALSE;
      }
    } else if (l1_fm_data_ptr->l1_fm_table[i].state == FM_STATE_CB_PENDING)
    {
      return FALSE;
    }
  }

  if (l1_tskisr_blk->firmware_load_status == FW_PENDING_RELEASE)
  {
    MSG_GERAN_MED_1_G("Frame Manager:Activity bplmn allowed but TRM release is pending FN=%d",
                      gl1_get_FN(gas_id));

    l1_fm_clear_required_frames(FM_PRI_BACKGROUND_PLMN, gas_id);
    return FALSE;
  } 
  else if 
    (((l1_fm_data_ptr->suspend_trm_release != 0) &&
         (l1_fm_data_ptr->suspend_trm_release_pri != FM_PRI_RESERVED) &&
         (l1_fm_data_ptr->suspend_trm_release_pri != FM_PRI_BACKGROUND_PLMN)))
    /* Check that we are not still suspending if in multi sim mode for lower priority event
     * or if FM_PRI_RESERVED then always allow rebooking to go through as we just keep trm
     */
  {
      /* not applicable for BPLMN in DSDS mode only DSDA*/
      MSG_GERAN_HIGH_1_G("Frame Manager:Activity bplmn allowed but suspend_trm_release set! FN:%d",
                         gl1_get_FN(gas_id));

      l1_fm_clear_required_frames(FM_PRI_BACKGROUND_PLMN, gas_id);

      return FALSE;
  } else
  {

      MSG_GERAN_MED_2_G("Frame Manager:Activity bplmn can run now!(FN:%d,CCCH gap:%d)", gl1_get_FN(gas_id), l1_fm_data_ptr->gap_to_ccch_activity);

#ifdef FRAME_MANAGER_LOGGING
      l1_fm_log_event(FM_PRI_BACKGROUND_PLMN, EVE_START_TASK, gas_id);
#endif

      return TRUE;
    }

}

#endif /* defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM) */

/*===========================================================================

Function
  l1_fm_set_activity_running

Description
  sets the priority to a running activity

Returns
 None

=============================================================================*/
void l1_fm_set_activity_running(l1_fm_priority_T pri, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  l1_fm_data_ptr->l1_fm_table[pri].state = FM_STATE_USING_FRAMES;

}

/*===========================================================================

FUNCTION l1_fm_is_next_activity_IRAT

DESCRIPTION
  This function returns is the current running or the next reserved priority is an IRAT one

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  boolean - returns is the current running or the next reserved priority is an IRAT one

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_fm_is_next_activity_IRAT(gas_id_t gas_id)
{

  switch(l1_fm_get_running_activity(gas_id))
  {
     case FM_PRI_WCDMA_RECONF:
     case FM_PRI_WCDMA_IDSEARCH:
     case FM_PRI_LTE_IDSEARCH:
#ifdef FEATURE_GSM_TDS
     case FM_PRI_TDS_ACQ: /*Low Chip Rate TD-SCDMA*/
#endif
     {
       MSG_GERAN_HIGH_0_G("l1_fm_is_next_activity_IRAT: IRAT activity running");
     return TRUE;

     }break;
     default:
   {
     MSG_GERAN_LOW_0_G("l1_fm_is_next_activity_IRAT: No IRAT running");
     }break;
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   switch(l1_get_fm_trm_priority(gas_id))
   {
     case FM_PRI_WCDMA_RECONF:
     case FM_PRI_WCDMA_IDSEARCH:
     case FM_PRI_LTE_IDSEARCH:
#ifdef FEATURE_GSM_TDS
     case FM_PRI_TDS_ACQ: /*Low Chip Rate TD-SCDMA*/
#endif
     {
       MSG_GERAN_HIGH_0_G("l1_fm_is_next_activity_IRAT: IRAT activity reserved");
     return TRUE;

     }break;
     default:
   {
        MSG_GERAN_LOW_0_G("l1_fm_is_next_activity_IRAT: No IRAT reserved");
     }break;

   }
#endif
   return FALSE;
}

/*===========================================================================

FUNCTION l1_fm_clear_irat_activity

DESCRIPTION
  This function clears all irat activity in FM

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None.

===========================================================================*/
void l1_fm_clear_irat_activity(gas_id_t gas_id)
{

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  switch(l1_get_fm_trm_priority(gas_id))
  {
     case FM_PRI_WCDMA_RECONF:
     case FM_PRI_WCDMA_IDSEARCH:
     case FM_PRI_LTE_IDSEARCH:
#ifdef FEATURE_GSM_TDS
     case FM_PRI_TDS_ACQ: /*Low Chip Rate TD-SCDMA*/
#endif
     {
       l1_fm_reset_pri_res(gas_id);
     }break;
     default:
   ;
  }
#endif

  switch(l1_fm_data[gas_id].fm_active_pri)
  {
     case FM_PRI_WCDMA_RECONF:
     case FM_PRI_WCDMA_IDSEARCH:
     case FM_PRI_LTE_IDSEARCH:
#ifdef FEATURE_GSM_TDS
     case FM_PRI_TDS_ACQ: /*Low Chip Rate TD-SCDMA*/
#endif
     {
       l1_fm_data[gas_id].fm_active_pri = FM_PRI_RESERVED;
     }break;
     default:
   ;
  }

  l1_fm_clear_required_frames(FM_PRI_WCDMA_RECONF, gas_id);
  l1_fm_clear_required_frames(FM_PRI_WCDMA_IDSEARCH, gas_id);
  l1_fm_clear_required_frames(FM_PRI_LTE_IDSEARCH, gas_id);
#ifdef FEATURE_GSM_TDS
  l1_fm_clear_required_frames(FM_PRI_TDS_ACQ, gas_id);
#endif
}
#endif /* FEATURE_GSM_COEX_SW_CXM */

/*===========================================================================

FUNCTION l1_fm_is_fn_ccch

DESCRIPTION
  Checks if a FM_PRI_CCCH is planned at frame_fn.

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  TRUE if a PRI_CCCH is planned at frame_fn.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_fm_is_fn_ccch(uint32 frame_fn, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];

  if(frame_fn == l1_fm_data_ptr->l1_fm_table[FM_PRI_CCCH].reserved_frame_number)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}



#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)

/*===========================================================================

FUNCTION l1_fm_wmgr_rebook_activity

DESCRIPTION
  This function cancels the previously booked activity and books the next
  activity. It then returns gap to next activity or 0 if activity could not
  be cancelled.

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  returns gap_to_activity (0 if activity cannot be cancelled)

SIDE EFFECTS
  None.

===========================================================================*/
uint32 l1_fm_wmgr_rebook_activity(gas_id_t gas_id)
{
  uint32 gap_warmup_to_activity;
  uint32 this_FN;
  uint32 cancelled_activity_fn;
  boolean reprog_allowed;
  uint32 gap_to_activity;
  l1_fm_priority_T next_activity_pri;
  uint32 next_activity_fn;
  uint32 next_activity_dur;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#ifdef FEATURE_PBR_RANDOMIZATION  
  boolean page_wakeup_cancel = FALSE;
  uint8 trm_reserved_duration = l1_fm_get_trm_duration( gas_id );
  if (grm_get_reason( l1_tskisr_blk->client_id, gas_id) == TRM_DEMOD_PAGE) 
  {
    /* cancelled activity is PCH */
    page_wakeup_cancel = TRUE;
  }
#endif
  /* 1. find current activity FN which is being cancelled  */
  cancelled_activity_fn  = l1_fm_get_trm_reserved_fn(gas_id);

  /* 2. find if activity can be cancelled */
  reprog_allowed = l1_fm_wmgr_cancel_activity(gas_id);                              /* WMGR_CORE */

  /* 3. Find GAP to next activity */
  if(reprog_allowed)
  {
#ifdef FEATURE_PBR_RANDOMIZATION
  /* Indicate TRM about wakeup cancelled for page, before reserving for next activity
     TRM will increment/decrement PBR count based on this event*/
    if ( page_wakeup_cancel )
    {
       grm_inform_grant_event(l1_tskisr_blk->client_id,
                              gl1_ms_get_trm_resource(gas_id, TRM_DEMOD_PAGE),
                              trm_reserved_duration,
                              TRM_DEMOD_PAGE,
                              TRM_DENIED
                             );
    }
#endif
    /* 4. Run frame manager to schedule the next activity and also book TRM */
    l1_fm_find_next_task_to_reserve_or_release(gas_id);

    /* 5. Info on next activity is now available */
    next_activity_fn  = l1_fm_get_trm_reserved_fn(gas_id);
    next_activity_pri = l1_get_fm_trm_priority(gas_id);
    next_activity_dur = l1_fm_get_trm_duration(gas_id);

    this_FN = GSTMR_GET_FN_GERAN(gas_id);
    gap_warmup_to_activity = SUB_FN(cancelled_activity_fn, this_FN);

    /* 6. Update logging */
    /* a. pri */
    gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_BK_PRI(next_activity_pri);

    /* b. next activity */
    if(next_activity_pri == FM_PRI_CCCH)
    {
      if(l1_tskisr_blk->reservation_for_bcch == FALSE)
      {
        /* PCH */
        gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ACT_BK_PCH;
      }
      else
      {
        /* BCCH */
        gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ACT_BK_BCCH;
      }
    }
    else
    {
      /* Other activity */
      gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ACT_BK_OTHER;
    }

    /* 7. Take care of L1 */
#ifdef FEATURE_GSM_COEX_SW_CXM
    if((gl1_is_l1_state_msim_idle(gas_id)) &&  (OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)))
    {
      garb_intf_update_timeline_at_wakeup_cancel(gas_id);
    }
#endif

    gl1_hw_wmgr_drx_require_tick(next_activity_fn, gas_id);

    /* 8. Find gap_to_activity */
    gap_to_activity = SUB_FN(next_activity_fn, cancelled_activity_fn);

    /* 9. Add gap_warmup_to_activity */
    gap_to_activity += gap_warmup_to_activity - 1;
  }
  else
  {
    /* activity cannot be cancelled */
    gap_to_activity = 0;
  }

  return gap_to_activity;
}


/*===========================================================================

FUNCTION l1_fm_wmgr_cancel_activity

DESCRIPTION
  This function cancels the previously booked activity and books the next
  activity. It then returns gap to next activity or 0 if activity could not
  be cancelled.

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  returns reprog_allowed T if activity is present in frame table and its safe to reprog

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_fm_wmgr_cancel_activity(gas_id_t gas_id)
{
  uint32 gap_to_next_ccch;
  uint32 gap_warmup_to_activity;
  uint32 cancelled_activity_fn;
  uint32 this_FN;
  uint32 next_ccch_fn;
  uint32 ccch_activity_dur;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean next_ccch_is_pch;
  boolean reprog_allowed = TRUE;
  boolean cancelled_activity_is_pch;
  boolean prev_reservation_for_bcch;
  l1_fm_priority_T cancelled_activity_pri;
  l1_fm_priority_T pri_other_activity;
  idle_data_T *idle_data_ptr;

  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  /* 1. Find current activity that is going to be cancelled */
  cancelled_activity_pri = l1_get_fm_trm_priority(gas_id );
  cancelled_activity_fn  = l1_fm_get_trm_reserved_fn(gas_id);
  this_FN = GSTMR_GET_FN_GERAN(gas_id);
  gap_warmup_to_activity = SUB_FN(cancelled_activity_fn, this_FN);

  /* 2a. store pri for cancelled activity */
  gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_CAN_PRI(cancelled_activity_pri);

  /* 2b. Store current activity for logging */
  cancelled_activity_is_pch = FALSE;
  if(cancelled_activity_pri == FM_PRI_CCCH)
  {
    if(l1_tskisr_blk->reservation_for_bcch == FALSE)
    {
      /* cancelled activity is PCH */
      cancelled_activity_is_pch = TRUE;

      /* store cancelled activity */
      gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ACT_CAN_PCH;
    }
    else
    {
      /* cancelled activity is BCCH */

      /* store cancelled activity */
      gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ACT_CAN_BCCH;
    }
  }
  else
  {
    /* cancelled activity is not PCH/BCCH */

    /* store cancelled activity */
    gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ACT_CAN_OTHER;

    /* Cancelling non CCCH activity is not supported */
    reprog_allowed = FALSE;

    /* store error code */
    gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_UNSUPPORTED_ACT_CAN;

    MSG_GERAN_MED_1_G("WMGR_SLEEP: Non-CCCH cancellation not supported: 0x%x", gl1_hw_sleep_wmgr_result[gas_id]);

    return reprog_allowed;
  }

  /* 3. Check if there is any other valid activity in the frame table */
  pri_other_activity = l1_fm_get_activity_before_fn( cancelled_activity_pri,        /* WMGR_CORE */
            ADD_FN(this_FN, L1_FM_WMGR_CANCEL_NEXT_ACTIVITY_MIN_GAP), gas_id);
  if(pri_other_activity == FM_PRI_RESERVED)
  {
    /* There is no activity in frame table before start_fn. If we had some activity there,
     * then frame manager could have scheduled it when we remove this current activity,
     * but we possibly, we won't have time to schedule the next activity.
     */

    /* 4. If we are cancelling PCH/BCCH, then find next PCH/BCCH */
    if(cancelled_activity_pri == FM_PRI_CCCH)
    {
      /* 5. find GAP to next FM_PRI_CCCH - it may be a new activity which is not in frame mgr table */

       /*Set paging mode to normal if its extended paging */
      if(idle_data_ptr->paging_data.paging_mode == EXTENDED_PAGING)
           idle_data_ptr->paging_data.paging_mode = NORMAL_PAGING;
	  
      /* 5a. save it in case we need to restore it if cancellation cannot be done */
      prev_reservation_for_bcch = l1_tskisr_blk->reservation_for_bcch;

      /* 5b. Find next BCCH */
      /* using L1_FM_WMGR_CANCEL_CALC_GAP_TO_NEXT_ACTIVITY_FN_OFFSET instead of cancelled_activity_dur */
      /*Add 2frames FN offset, FN offset should be less than 4. If PCH(FN+4) is immediately after BCCH(FN) and wakeup is canceled for BCCH then gl1 should calc gap to PCH */
      gap_to_next_ccch = gl1_idle_get_gap_to_next_activity(L1_FM_WMGR_CANCEL_CALC_GAP_TO_NEXT_ACTIVITY_FN_OFFSET + gap_warmup_to_activity, gas_id );

      /* 5c. 1 frame has to be added to get correct gap (normally throughout the code) */
      gap_to_next_ccch += L1_FM_WMGR_CANCEL_CALC_GAP_TO_NEXT_ACTIVITY_FN_OFFSET + gap_warmup_to_activity + 1; /* 1 has to be added for getting new fn */

      MSG_GERAN_MED_3_G("WMGR_SLEEP: Calculated gap to next activity is: %d, gap_warmup: %d, FN_OFFSET: %d",
        gap_to_next_ccch, gap_warmup_to_activity, L1_FM_WMGR_CANCEL_CALC_GAP_TO_NEXT_ACTIVITY_FN_OFFSET);

      /* 6. Find activity/dur/next acitivity FN */

      /* a. check if next activity is PCH or BCCH */
      if(l1_tskisr_blk->reservation_for_bcch == FALSE)
      {
        /* PCH */
        next_ccch_is_pch = TRUE;
      }
      else
      {
        /* BCCH */
        next_ccch_is_pch = FALSE;
      }

      /* b. duration (BCCH and PCH have same duration) */
      ccch_activity_dur = L1_SC_DURATION_CCCH;

      /* c. Calculate next activity FN (gap returned was wrt this_FN) */
      next_ccch_fn = ADD_FN(this_FN, gap_to_next_ccch);

      /* d. This is the next earliest pt at which we are allowed to book TRM for CCCH */
      gl1_hw_sleep_next_allowed_ccch_fn[gas_id] = next_ccch_fn;

      /* 7. For PCH, check if we can cancel and book next pch.
       *    The plan is to allow cancelling of only one PCH in order to
       *    allow frame manager to run and schedule other NCELL activities.
       */
      if( (next_ccch_is_pch != TRUE) || gl1_hw_sleep_wmgr_pch_cancel_allowed[gas_id] )
      {

        /* 8. Finally allowed to cancel PCH/BCCH.
         *    Cancellation allowed only if another PCH/BCCH activity is present to be
         *    reprogrammed, or if another activity is present in FM to be scheduled.
         */

        /* a. As we are only allowing 1 PCH cancellation, mark it so that we
         *    don't cancel next PCH
         */
        if(cancelled_activity_is_pch == TRUE)
        {
          gl1_hw_sleep_wmgr_pch_cancel_allowed[gas_id] = FALSE;
        }
        else
        {
          /* Inform GRR that we cancelled an attempt to read BCCH using block quality UNKNOWN */
          L1_send_MPH_BLOCK_QUALITY_IND(BCCH, GL1_BLOCK_QUALITY_UNKNOWN, gas_id);
        }
        /* b. Clear FN for current activity from FM */
        l1_fm_clear_required_frames(cancelled_activity_pri, gas_id);

        /* c. We don't want tick at the next activity as it is cancelled.
         *    Clearing tick means nobody wants to use radio at that tick and
         *    we can sleep at that tick
         */
        gl1_drx_release_fn_tick(cancelled_activity_fn, gas_id);

        /* d. Tell FM that we want new activity at that FN */
        l1_fm_set_required_frame_number(FM_PRI_CCCH, next_ccch_fn, ccch_activity_dur, gas_id);
      }
      else
      {
        /* cannot proceed as cannot cancel pch activity again */
        reprog_allowed = FALSE;

        /* store error code */
        gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_PCH_CAN_NOT_ALLOWED;

        /* restore reservation_for_bcch */
        l1_tskisr_blk->reservation_for_bcch = prev_reservation_for_bcch;
      }
    }       /* if(cancelled_activity_pri == FM_PRI_CCCH) */
    else
    {
      /* current activity is not PCH/BCCH */

      /* 9. Check if there is any other valid activity in the frame table */
      pri_other_activity = l1_fm_get_other_activity(cancelled_activity_pri, gas_id);

      /* 10. Proceed with cancellation only if there is some other activity in frame table */
      if( pri_other_activity != FM_PRI_RESERVED )
      {
        /* some other activity is present in frame table,
         * so we cancel current activity and schedule next activity.
         */

        /* a. Clear current activity from FM */
        l1_fm_clear_required_frames(cancelled_activity_pri, gas_id);

        /* b. We don't want tick at the next PCH as it is cancelled.
         *    Clearing tick means nobody wants to use radio at that tick and
         *    we can sleep at that tick
         */
        gl1_drx_release_fn_tick(cancelled_activity_fn, gas_id);
      }
      else
      {
        /* we don't have any other activity to be run, so cannot remove
         * current activity as then we will never wakeup if we don't have next activity.
         * By waking up, we will run frame manager tick, and it will find the next activity.
         */
        reprog_allowed = FALSE;

        /* store error code */
        gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_NO_OTHER_ACTIVITY;
      }
    }   /* else - if(cancelled_activity_pri == FM_PRI_CCCH) */
  }     /* if(pri_other_activity != FM_PRI_RESERVED) */
  else
  {
    /* some other activity found before start_fn, so we cannot proceed with wkup cancellation
     * as we may not have enough time to reprog slpc
     */
    reprog_allowed = FALSE;

    /* store error code */
    gl1_hw_sleep_wmgr_result[gas_id] |= GL1_HW_SLEEP_WMGR_RES_ERR_ACTIVITY_INSUF_GAP;
  }     /* else - if(pri_other_activity == FM_PRI_RESERVED) */

  return reprog_allowed;
}

/*===========================================================================

FUNCTION l1_fm_get_activity_before_fn

DESCRIPTION
  Find first activity in the Frame Table before start_fn, and which is other
  than the cancelled activity

INPUTS
  Cancelled activity, Start FN

DEPENDENCIES
  None

RETURN VALUE
  l1_fm_priority_T - first activity that will be run
                     or FM_PRI_RESERVED if no activities were found

SIDE EFFECTS
  None.

===========================================================================*/
l1_fm_priority_T l1_fm_get_activity_before_fn(l1_fm_priority_T cancelled_activity_pri, uint32 start_fn, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  l1_fm_priority_T i;
  uint32 cur_activity_fn;
  uint32 next_activity_fn = NOT_A_FRAME_NUMBER;
  l1_fm_priority_T next_activity_pri = FM_PRI_RESERVED;

  /* find earliest activity after start_fn */
  for (i=0; i<FM_PRI_MAX; i++)
  {
    cur_activity_fn = l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number;
    if (  (i != cancelled_activity_pri) &&                  /* not same as cancelled activity */
          (cur_activity_fn != NOT_A_FRAME_NUMBER) &&        /* entry is valid */
          IS_FRAME_NUM_LATER_OR_EQUAL(start_fn, cur_activity_fn) )  /* some activity is present before start_fn */
    {
      if( (next_activity_fn == NOT_A_FRAME_NUMBER) || IS_FRAME_NUM_LATER(next_activity_fn, cur_activity_fn) )
      {
        /* new activity is before earlier found activity */
        next_activity_pri = i;
        next_activity_fn = cur_activity_fn;
      }
    }                                                       /* if (  (i != cancelled_activity_pri) */
  }                                                         /* for (i=0; i<FM_PRI_MAX; i++) */

  if(next_activity_pri != FM_PRI_RESERVED)
  {
    MSG_GERAN_MED_3_G( "WMGR_SLEEP: First activity before start_fn: pri: %d, fn: %d, start_fn: %d",
      next_activity_pri, next_activity_fn, start_fn);
  }

  return next_activity_pri;
}

/*===========================================================================

FUNCTION l1_fm_get_other_activity

DESCRIPTION
  This function looks through the frame manger table to find if there is any
  activity other than current activity which can be run

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  l1_fm_priority_T - first other activity which is other than current activity
                   or FM_PRI_RESERVED if no activities were found

SIDE EFFECTS
  None.

===========================================================================*/
l1_fm_priority_T l1_fm_get_other_activity(l1_fm_priority_T pri, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  l1_fm_priority_T i;
  l1_fm_priority_T pri_return = FM_PRI_RESERVED;

  for (i=0; i<FM_PRI_MAX; i++)
  {
    /* check if we have a valid frame in frame tbl which is other than pri */
    if ( (i != pri) && (l1_fm_data_ptr->l1_fm_table[i].reserved_frame_number != NOT_A_FRAME_NUMBER) )
    {
      pri_return = i;
      break;
    }
  }

  return pri_return;
}

/*===========================================================================

FUNCTION l1_fm_get_reserve_sub_reason

DESCRIPTION
  This function returns reserve_sub_reason

INPUTS
  gas_id_t gas_id

DEPENDENCIES
  None

RETURN VALUE
  sys_proc_type_e_type - reserve_sub_reason
SIDE EFFECTS
  None.

===========================================================================*/
sys_proc_type_e_type l1_fm_get_reserve_sub_reason(gas_id_t gas_id)
{
  return reserve_sub_reason[gas_id];
}


/*===========================================================================

FUNCTION l1_fm_exchange_cbch_priority

DESCRIPTION
  This function looks for if older TRM resrevation is there with different priority after receiving the SET_PRIORITY_IND 
  to alter the priority for CBCH. Exchange the reserved priority if there is some already reserved CBCH activity is there.

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void l1_fm_exchange_cbch_priority( gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  uint32        cbch_reserved_fn;
  uint8         cbch_tc;

  l1_fm_priority_T curr_cbch_pri = l1_fm_get_active_CBCH_pri(gas_id);
  l1_fm_priority_T pending_cbch_pri;

  if(l1_fm_data_ptr->l1_fm_table[curr_cbch_pri].reserved_frame_number != NOT_A_FRAME_NUMBER)
  {
    MSG_GERAN_HIGH_2_G("Curr Pri:%d, FN:%d",curr_cbch_pri, l1_fm_data_ptr->l1_fm_table[curr_cbch_pri].reserved_frame_number);
    /*Check if reservedFN maps to TC=0*/
    cbch_reserved_fn = l1_fm_data_ptr->l1_fm_table[curr_cbch_pri].reserved_frame_number;

    cbch_tc = (cbch_reserved_fn / MULTIFRAME_51) % 8;

    if (cbch_tc == 0)
	{
  
      pending_cbch_pri = l1_fm_updated_CBCH_pri(gas_id);	

      if (pending_cbch_pri != curr_cbch_pri)
      {
        l1_fm_table_element_copy(pending_cbch_pri, curr_cbch_pri, gas_id);
        l1_fm_clear_required_frames(curr_cbch_pri, gas_id);
        MSG_GERAN_HIGH_4_G("Copied from CurrPri:%d, NewPri:%d, CurrFN:%d, NewFN:%d",curr_cbch_pri, pending_cbch_pri, l1_fm_data_ptr->l1_fm_table[curr_cbch_pri].reserved_frame_number,
                            l1_fm_data_ptr->l1_fm_table[pending_cbch_pri].reserved_frame_number);		

        if (l1_fm_data_ptr->trm_reserved_pri == curr_cbch_pri)
        {
          grm_update_reserve_reason( pending_cbch_pri, gas_id);
        }
      }
    }
  }
}

/*===========================================================================

FUNCTION l1_fm_task_in_conflict

DESCRIPTION
  This function detetcts if input task is in conflict with low priority reserved task

INPUTS
  task priority and its start frame number

DEPENDENCIES
  None

RETURN VALUE
  TRUE  if the input task is in conflict with low priority reserved task
  FALSE otherwise

SIDE EFFECTS
  None.

===========================================================================*/
boolean l1_fm_task_in_conflict(l1_fm_priority_T task, uint32 task_fn, gas_id_t gas_id)
{
  boolean conflict = FALSE;
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];

  if ( ( l1_fm_data_ptr->trm_reserved_pri != FM_PRI_RESERVED )
        && ( l1_fm_data_ptr->trm_reserved_pri != FM_PRI_MAX )
        && ( l1_fm_data_ptr->trm_reserved_pri > task )
     )
  {
    /* where higher priority task is having conflict with low priority reserved task */
    if ( !( IS_FRAME_NUM_LATER( l1_fm_data_ptr->trm_reserved_fn, task_fn ) )
         && ( SUB_FN(task_fn,l1_fm_data_ptr->trm_reserved_fn) <= l1_fm_data_ptr->trm_reserved_duration )
       )
    {
      conflict = TRUE;
    }
  }

  return conflict;
}
#endif /* defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM) */

/*===========================================================================

FUNCTION l1_fm_table_element_copy

DESCRIPTION
  This function looks for if older TRM resrevation is there with different priority after receiving the SET_PRIORITY_IND 
  to alter the priority for CBCH. Exchange the reserved priority if there is some already reserved CBCH activity is there.

INPUTS
  None

DEPENDENCIES
  None

RETURN VALUE
  TRUE  if the source activity is already running
  FALSE if the source activity is not running

SIDE EFFECTS
  None.

===========================================================================*/
static boolean l1_fm_table_element_copy( l1_fm_priority_T dest, l1_fm_priority_T source, gas_id_t gas_id)
{
  l1_fm_table_element_T *dest_fm_ptr    = &l1_fm_data[gas_id].l1_fm_table[dest];
  l1_fm_table_element_T *source_fm_ptr  = &l1_fm_data[gas_id].l1_fm_table[source];
  boolean active = FALSE;

  if(l1_fm_get_running_activity(gas_id) == source)
  {
    MSG_GERAN_HIGH_1_G("Do not copy frame manager elements as activity %d is running", source);
    active = TRUE;
    return active;
  }

  MSG_GERAN_HIGH_2_G("Copy frame manager elements from activity %d to activity %d",source,dest);


  /* Copy contents of source data to destination */
  memscpy(dest_fm_ptr, sizeof(l1_fm_table_element_T), source_fm_ptr, sizeof(l1_fm_table_element_T));

#ifdef FRAME_MANAGER_LOGGING
  l1_fm_log_event(dest, EVE_SET_REQD_FRAMES, gas_id);
#endif

  return active;
}
/*===========================================================================

Function
  l1_fm_is_activity_reserved

Description
  Check if an activity has been reserved.

Returns
  TRUE - Activity reserved
  FALSE - Activity not reserved

=============================================================================*/
l1_fm_priority_T l1_fm_is_activity_reserved(l1_fm_priority_T pri, gas_id_t gas_id)
{
  l1_fm_data_t *l1_fm_data_ptr = &l1_fm_data[gas_id];
  if(l1_fm_data_ptr->l1_fm_table[pri].reserved_frame_number == NOT_A_FRAME_NUMBER)
    return FALSE;
  else
    return TRUE;
}


#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

#ifdef FEATURE_DUAL_SIM                                
boolean gl1_is_qbta_reason ( trm_reason_enum_t winning_reason,trm_client_enum_t winning_client_id )
{
#ifdef FEATURE_QBTA
   boolean is_qbta_reason = FALSE;
   gas_id_t gas_id = gl1_map_client_id_to_gas_id((grm_client_enum_t) winning_client_id);

   if (gas_id < GERAN_ACCESS_STRATUM_ID_UNDEFINED)
   {
      is_qbta_reason = IS_QBTA_REASON(winning_reason, reserve_sub_reason[gas_id]);

      MSG_GERAN_HIGH_3_G("IS_QBTA_REASON %d- reserved_reason=%d, sub=%d",is_qbta_reason, 
                     winning_reason,
                     reserve_sub_reason[gas_id]);      
   }
   return (is_qbta_reason);

#endif
   return FALSE;

}
#endif /*FEATURE_DUAL_SIM */

l1_fm_priority_T l1_fm_get_pending_CBCH_pri( gas_id_t gas_id )
{
   return (cbch_pending_pri[gas_id]);
}

/* EOF */

