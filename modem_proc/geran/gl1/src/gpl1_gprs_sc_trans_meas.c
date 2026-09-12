
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           SCE: L1 DEDICATED MODE

GENERAL DESCRIPTION
   This module contains SCE code for L1 Dedicated mode, SDCCH and TCH
   measurements.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_sc_trans_meas.c#3 $
$DateTime: 2020/02/10 05:49:45 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
21/06/16   hd        CR1031512 Exchange TRM client while scheduling PCH in XFER if it dosn't happen at the time of PTM init
03/07/18   rc        CR2203950 Not to fire G2X TA sequencer on or after activity scheduling frame.
26/03/18   rc........CR2210144 Calling Inform_grant with TRM DENIAL after grm_request from task when PCHXFER is not the winning client
23/03/18   sn        CR2210037 Honor TRM_UNLOCK_REQD where there is change in Client_id
22/03/18   sn        CR2206198 To Ensure PCHXFER activity and TRM_UNLOCK activity conflict at TRM when TRM_UNLOCK is received. 
14/03/18   rc        CR2189833 To prioritize unlock over WID search when T3168/T3192 timer is running
31/01/17   km        CR1094189 Do not reset GTA winning client 
11/01/16   km        CR1102445 Resolving collision check criteria for pri1 and pri2 in G2X_TA schedular
01/02/18   ap       CR2022526 X tech SUB1 page denied by GSM SUB2 when reading PCH in Transfer
01/02/18   sn        CR2181500 Don't Ignore TRM_UNLOCK if Planned acitvity is staled one
01/02/18   km        CR2011569 Check for current client-id (GPRS1 or GPRS2) before exchanging it after TBF is established
18/09/17   rc        CR2108045 To increase minimum gap to scheduling PCH n transfer 
01/03/17   sn        CR2164797 Reschedule Activity 5 w.r.to updated new PCHXFER duration
11/08/17   rc        CR2138578 ML logging - GL1 Changes
31/08/16   sk        CR1061117 Do not abort PCH/BCCH of serving cell on getting BA_list updated
22/08/16   snjv      CR1054401 Allow TRM Exchange in PCH in xfer even if Resource Lock is active
28/09/16   snjv      CR1070057 Check PCHXFER DR STATE before trigerring Release for PCHinXfer Client
21/11/16   snjv      CR1088881 G2X: Dont term activity as stale if on current frame 
09/11/16   sn        CR1086032 Adding Valid condition to avoid reservations for PCH in xfer while handling DTM CHANNEL ASSIGNMENT_REQ
12/09/16   mn        CR1060328 Ignore Unlock Event while leaving transfer mode due to DL full release.
20/12/17   sk        CR2030816 GERAN changes for PBR Randomization algorithm
27/06/16   sn        CR1033756 clearing the TDS Pch SuspFn when TDS does RANE.
09/09/16   bg        CR1035830:Reset last winning client details when activity 5 can not be updated in g2x TA table
20/07/16   sp        CR1040702: Allow TA during 2Phase access
08/09/17   sn       CR1061730 FR43082: TA during PS Procedures- GL1 changes
05/10/16   mn        CR1074007: Optimization to improve Page decode success rate for G2L tuneaway 
08/07/16   bg        CR1033899 Added mutex lock for G2X Tuneaway to avoid race conditions
09/09/16   bg        CR1042761:Check if higher priority activity collides with current activity
01/09/16   bg        CR1061674 Updated the scaling factor to 3ms to open the GTA gap 1 frame early
06/02/17   km        CR1040468 Fix for other sub X tech page miss during G2X TA
08/07/16   bg        CR1025344 Allow G2L TA only for LTE page in case LTE asks for back to back gaps within 100ms.
11/03/15   bg        CR983597 FR30575: SR-DSDS G+L with G to L tuneway - GL1 changes
16/02/17   cc        CR1010614 G2L idle time transfer and MEAS only support
02/02/17   br        CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
08/09/17   nm        CR2103941 Do not schedule IRAT when in 2 phase access
22/08/17   nm        CR2096887 Reset tuneaway and ncell data when leaving transfer
08/08/17   nm        CR2040229 Enable PCH read in 2 phase access for both Idle sub and data sub using GBTA
03/01/16   mn        CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
29/09/16   dg        CR1071675 Don't check for sc tick  while sending MVCS request to MCPM for G2L search
22/06/16   mn        CR1032754 Protecting g2x_ta activity status Under GL1_ISR_LOCK
21/06/16   hd        CR1031512 Exchange TRM client while scheduling PCH in XFER if it dosn't happen at the time of PTM init
10/06/16   nm        CR1025785 Set ignore_xcch in case ncell ACQ on PTM Yields PCH on idle sub
24/05/16   sn        CR1016717:Removing Excess F3 logging for packet transfer
25/01/16   nm        CR963816 cancel pchxfer activity before releasing trm
31/03/16   sn        CR904815 SCH reconf not happening in GPRS Tr mode when ncell is switched off and then on again
23/03/16   sn        CR993078  Removing cancelling of  PCHXFER activity from fire sequencer in tbf suspended state.
22/02/16   bg        CR975060 Decoupled PCHxFER scheduling from PTM NCell Schedule
21/12/15	   sk		 CR938638 Reducing no of F3s
15/12/15   nm        CR951738 use the update_table_field function for updating any G2X TA activity for proper block bound calculation
08/12/15   og        CR929672. Missing gl1_trans_grm_reserve_at_self_ccch( ) in
                     gpl1_gprs_compute_ccch_event_qbit_lag( ).
12/11/15   nm        CR936310 Cancel table for both pch and bcch in transfer activity before leaving trans(regression from CR/899669)
12/11/15   nm        CR932874 Do not reset the 2 phase access G2T TA state machine on gettign unlock cancelled
12/11/15   aga       CR928385 Apply 10 frame delay in case of 1x TA data unlock required as done for default cases
02/11/15   aga       CR933783 GL1 P1 KW errors around Pointer 'grm_info_1'
02/11/15   cjl       CR930113 Block PCHXFER when resource lock is active
27/10/15   nm        CR922518 Decouple the WReconf search from Page when PCHXFER is enabled
30/10/15   nm        CR921073 Release trm for pchxfer acitivity while opening G2X_TA gap and Resv again when gap closes
10/05/15   mko       CR909257 If FN adjustment for neighbour cells is not done then don't compensate while leaving the transfer mode
09/09/15   nm        CR899669 Handle abort of PCHXFER properly if leaving transfer mode
16/09/15   cjl       CR908527 improve system procedures used for TRM access
23/09/15   sp        CR903927: Update default tbf susp start frame for Unlock Required Case.
21/09/15   nm        CR767743 Using actual duration in trm unlock cb for unlock by event
02/09/15   zf        CR905869: Set trm_status to NULL after grm release
07/09/15   js        CR903209 Backing out CR891616
01/09/15   ab        CR898944 - PCHXFER during TRM exchange should not release Diversity Chain
26/08/15   nm        CR890755 corect the next nbcch/sbcch fn calculation in transfer state
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
26/08/15   js        CR891616 Setting correct duration to tune away table  
14/08/14   ws        CR889273 Fix Static declarations of variables which are not Dual data spaced
11/08/15   hd        CR873848 Ignore the unlock callback for CBCH activity during PTM
03/08/15   nm/ab     CR883804 Do not exchange client for DR-Transition until SUB has entered Transfer
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons 
24/07/15   zf        CR876435: Get RF device in TBF TRM grant CB
21/07/15   cws       CR873163 fix asdiv/pchxfer compile warnings
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types
25/06/15   nm        CR859825 Correct the reservation time for PCHXFER so as to match Request time 
18/05/15   dg        CR838227 Remove frame_slaming_flag as block level checks are already there
                     to ensure no G2X irat is triggered if TS adjustment is pending
12/07/15   nm        CR861458 Fix the interworking issues between PCHXFER, DRDSDS, RX/TX split
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
25/06/15   nm        CR859724 Correct the RxD code which was preventing grm_change_priority to be called
17/06/15   ab        CR847473 : unify usage of gl1_map_gas_id_to_client_id for TX_ANT_DIV/DR_DSDS/RX_DIV/PCHXFER/AS_DIV
18/06/15   ap        CR856107: Fix GL1 deadlock with TRM : grm_tbf_enh_grant_callback
12/06/15   sp        CR845398: Fix GL1 deadlock with TRM, bringback CR780458
02/06/15   ar        CR847207 TRM_UNLOCK_BAND_INCOMPATIBLE not handled by gl1
28/05/15   pa        CR844276: Handling of tbf grant callback for Tx Client.
19/05/15   ar        CR840298 check the trm_grant_status against rx client ID in grm_tbf_enh_grant_callback.
08/05/15   ws        CR834838 Correct as_id,gas_id calling fn parameter bugs
01/05/15   aga       CR831392 Add support for second WCDMA TRM client in case of G2X tune away
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
22/04/15   nm        CR806841 Correct F3 related to G2X_TA
21/04/15   sjw       CR822898: removal of deprecated ACQUISITION reasons
31/03/15   cja       CR800650 Optimise irat state machine
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
27/03/15   ap        CR816435 FR22272 - TRM Unification  - Correct removal of trm continuation reason
27/03/15   ap        CR811054 FR22272 - TRM Unification  - Extended Paging with PBR
23/03/15   ap        CR811046:FR22272 - TRM Unification  - Removal of _HI reasons/UNLOCK_IMMEDIATELY
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
24/03/15   sjw       CR812407: update for new MPH_SET_PRIORITY_IND payload
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
04/03/15   br        CR802631: G2X TA: Handling RACE condition between W id in TBF search and MAC_L1 Release
30/01/15   sp        CR788423: Ignore TRM demod broadcast reason for Unlock Required events
06/02/14   mko       CR767744 De-prioritization of Ncell SI read in packet transfer mode if it is colliding with TDS page (TDS page miss OPT#9)
04/02/14   nk        CR767734 Allowing TA during 2-phase access
09/02/14   nm        CR793018 correct the hyperframe wrap around logic for bcch fn
11/02/15   am        CR790280 Return to normal page mode on back to back extended page in xfer mode
18/02/15   nk        CR773487 Release TRM inline when doing it from tbf_enh_grant_callback
12/02/15   am        CR771762 BCCH Decode Failures at Good RSSI when the NCell is drifted by 1.5KHz
03/02/15   zf        CR780458: Adding L1 task grm_release handling for callback
18/02/15   nm        CR796941 Fix KW error in gpl1_g2x_ta_cancel_table_field which got introduced from CR 772306
17/02/15   br        CR772280 G2X_TA: IGNORE TRM_UNLOCK_REQUIRED for SCI_PAGE reason
17/02/15   mko       CR767531 Set/Clear TRM extension flag for LTA_SUPPORTED correctly
                           clear when moving out of transfer mode
17/02/15   mko       CR776353 Suspend all G2x_TA table fields after de-registering the sequencer
16/01/15   aga       CR780157: Remove transition from ACTIVE_GAP to ACTIVE for data call abort use cases
16/02/15   sp        CR772306: Update TRM of gbta grant for PBR to work
13/02/15   pa        CR756156:G2X RF device handling using TRM apis.
29/12/14   sp/aga    CR774163: Update GFW to active_gap when opening GTA gap
16/01/15   ak      CR781336: In case of winning client is not GSM1/2/3, GL1 bypasses the logic to check GBTA
11/02/14   am        CR719452 Addressing KW GL1 P1 errors
11/02/15   npt       CR791237 Revert workaround for G-G data transfer reselection issue
16/10/14   aga       CR735726 Remove obsolete gta_set_wcdma_blacklisted functionality
04/02/15   npt       CR791105 Workaround for G to G cell reselection issue
26/01/15   npt       CR786002 Fix compilation errors when FEATURE_DUAL_SIM is enabled
21/01/15   pa        CR758792:RxD support for multi-sim.
16/01/15   nm        CR765703 Align the bcch in xfer TS read with PCH in XFER (same as CCCH TN)
12/01/15   sjv       CR756154 Enhance Band Registration for Activities
05/01/15   npt       CR770381 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled
04/12/14   ab        CR766511 Perform Serving Cell and Neighbour Cell BCCH Read in Packet Transfer using Burst Tuneaway
12/12/14   zf        CR770728: In TRM TBF callback function, calling in task GRM release without requesting GL1 lock
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   ws        CR768376 Q6 compile warning fixes
05/12/14   fm        CR766852 Schedule the next G2X TA action, when the scheduled one is cancelled
03/12/14   npt       CR759467 Fix compiler warnings
02/12/14   ak        CR748234 Problems with legacy SCE Opt 16/17/20
01/12/14   aga       CR760146 Avoid cancelling ongoing suspension activity if gap already started
20/11/14   og        CR760473. Call grm_release in the GSM L1 task context.
29/10/14   jj        CR725942 Do Force ncell SCH in first attempt
13/11/14   nm        CR750236 Dont Schedule PCHXFER when leaving transfer (UL_DL_REL pending)
24/11/14   cws       CR762135 Release lock if TRM notifies to unlock immediately in SxLTE+G
25/11/14   sjv       CR759816 Remove reason checks for G2T and G2W TA for TBF unlock_by
19/11/14   aga       CR755567 Restart SCE after BCCH stopped
18/11/14   ab        CR758820 PCHXFER : Correct Featurization and Enable for FR21036 : Own in PCH in Transfer for SS.
06/10/14   dg        CR727901 Do Force Acq. on Max re-tries only if SCH lies before the start of acq window
14/11/14   aga       CR753308: Allow unlock immediate request while G data call is ongoing
12/11/14   nm        CR722187 Allow gbta suspension to be *scheduled* even when current gbta state is blocked
31/10/14   mko       CR746875 In wcdma_done_trans(), don't set allowed_to_go to false if BCCH to be scheduled
03/11/14   mko       CR720074 G->1x TA support
01/11/14   nm        CR746639 PCHXFER - Incorrect pch fn calculation when ccch ts is other than 0
30/10/14   sjv       CR748415 Do not perform band registrations for SCH/FCH if receive in progress
29/10/14   ab        CR745429 - Featurization correction for PCHXFER for SS / CR merges.
20/10/14   nm        CR727981 Cancel waiting PCH activity if we dont have TRM lock and it is within 40 ms( RANE duration)
20/10/14   nm        CR727855 while moving Act 5 after PCH, calculate the new fn considering the block boundary
20/10/14   mko       CR725933:Sending UNKNOWN_BLOCK_QUALITY when page is missed due to conflict with RRBP transmission, sothat page sharing works
19/09/14   nm        CR720652 update pch in transfer frames from 6 to 8 and correst lowest tn correct imminent logic
17/10/14   nm        CR741161 Enable feature PCH in transfer on Jolokia and fix a merge error
25/09/14   nm        CR727855 while moving Act 5 after PCH, calculate the new fn considering the block boundary
20/10/14   cws       CR734670 Prevent WTR hop for PS non-MMS call and enable WTR hop in SVLTE+G
17/10/14   pa        CR739626: In G2X TA, perform trm band registration on actual scheduling of BCCH.
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build
24/09/14   js        CR727907 : No need to exchange if Client is either TRM_GPRS1 or TRM_GPRS2 when moving back from DTM to transfer.
25/09/13   mko       CR728501 If TS adjustment is missed then send DS_ABORT & move to idle when TRM is granted back
29/09/14   nm        CR729351 Deregsiter PCH sequencer if sc recive abort done on getting new BA list
23/09/14   js        CR723532:Deregister the sequencer for priority TRM UNLOCK and check for transfer mode.
23/09/14   aga       CR717115 Shorten the first execution time for TRM_UNLOCK_REQUIRED event for G case to 7 frames
22/09/14   aga       CR725474 Allow TRM_UMTS_SECONDARY client to suspend TBF
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
11/09/14   ws        CR723099 Resolve GL1 compile warnings
29/08/14   cah       CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
22/08/14   mko       CR708801: Replace L1_send_PH_DATA_IND with L1_send_sysinfo_PH_DATA_IND so that TC value sent to RR
20/08/14   pa        CR712553: Perform band registration on scheduling SCH.
22/08/14   aga       CR713731 Updates to F3s and other recent changes
19/08/14   cah       CR710771: IFS band release not called when max attempts is reached for tone detect
08/08/14   nk        CR705945 Check for GERAN_ACCESS_STRATUM_ID_UNDEFINED to avoid memory read crash
05/08/14   ap        CR704438 Dont allow GTA whilst in SGLTE+G DSDA
13/08/14   ap        CR676077 Do not hold secondary chain when entering PS in SGLTE+G mode
13/08/14   ggu       CR687349:DSDS: GL1 should guarentee at least one frame eariler before start check TBF suspension for G2W TA case
07/08/14   aga       CR689162: Abort tune away gap mechanism triggered if lower TS change is imminent
05/08/14   mko       CR698626: When TBF suspension is aborted reschedule w/G activities for unlock required reasons
28/05/14   dg        CR668060 G2W ID search is not aborted properly before leaving packet transfer mode
25/07/14   am        CR698669 Transfer mode SCE should not be triggered until all g2l_timed_searchs are completed
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
30/07/14   pjr       CR701243 Handling MMS priority indication to block tune away during MMS
25/07/14   aga       CR698326 - avoid scheduling monitors when TBF suspension is imminent
16/07/14   aga       CR695332 PCH scheduling  is not initiated, GBTA is not stated
11/07/14   pjr       CR692400 Fix KW error
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
08/07/14   cah       CR690526: DR_DSDS: Fix for NCELL selection causing crash in gprs_next_cell_trans.
07/07/14   aga       CR675214: [G2X TA] Fixed legacy KW issue
02/07/14   aga       CR685484: [G2X TA] use logic in gpl1_gprs_tbf_susp_range(..) to determine NcBCCH/WID duration
02/07/14   aga       CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
30/05/14   ws        CR657674 Data + MMS Phase 1
19/06/14   cah       CR682757 BOLT 2.0. GL1 use of incorrect TRM API blocking MCS check in
18/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
21/05/14   aga       CR644507 Removing the 2 frame delay, legacy from GTA to reduce the G2W overhead time
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS: Ncell & irat measurement scheduling changes
08/05/14   nk        CR660831: GSM Layer 1 compares wrong enum values for internal TRM_DENIAL status. Change TRM_DENIAL enum to TRM_NOT_GRANTED to aoid any confusion going forward.
01/05/14   ab        CR657605 : GBTA : Transfer Sub should not manage GBTA transition
01/05/14   cgc       CR646598 Remove invalid GTA code that prevents sc ticks
11/04/14   pg        CR645875 Fix Bolt multisim compilation errors
01/04/14   aga       CR624117 double check if the client requesting the lock is W and set g2w_tbf_abort_callback_received accordingly
24/03/14   cgc       CR637417 Removed deprecated code,l1_sc_wcdma_xfer_ready(FALSE), merge missed from 2.1
24/03/14   ap        CR638181 Add extra case for Ignoring the winning client id when event is UNLOCK_CANCEL
24/03/14   ap        CR637442 Ignore the winning client id when event is UNLOCK_CANCEL
18/03/14   ap        CR636650 When Sub1 does G2x measurements & Sub2 in data transfer G Sub1 misses many PCH decode
18/03/14   ap        CR629844 SGLTE BPLMN support when G1 in PS call in G+G - PS Hopping
19/03/14   ws        CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
18/03/14   ap        CR614949 FR 19574: Performing LTE/TDS measurements while GERAN is in connected mode using the second WTR
17/03/14   aga       CR631890, Remove check for unlock imediatly, let it go trough the G2W tbf abort callback
06/03/14   br        CR626780, RACE condition between TRANSFER G sub and IDLE G sub, issuing SCH in G2x gap.
04/03/14   ap        CR626016 UE is not doing GERAN to LTE dedicated mode measurement in during Voice call
28/02/14   ap        CR624060 Further changes to FEATURE_PBR_MARKOV applied
12/02/14   dg        CR612047 Set flag send_gprs_trans_tick while returning from function gpl1_gprs_next_cell_trans
23/01/14   cgc       CR606502 rework CR589523 add l1_sc_ts_changed()
20/01/13   ap        CR599098: check for compatable RF device for IRAT
23/12/13   sk        CR590887 Ensure Ncell Acq doesnt get ticked just before a GBTA gap
06/01/14   sk        CR596201 Ensure PCH suspension frame correctly updated for Acq in TSTS mode
30/12/13   mko       CR 579095: Block G2W TA only if both device mode and device pref indicate TSTS
                     mode of operation.
19/12/13   js        CR581840 IDLE SUBs releasing TRM immediately after GRANT leading to unnecessary GTA Gaps
17/12/13   ap        CR566007: W is not getting the lock for Manual PLMN search when Sub2 is in data call
17/12/13   nm        CR587488 ncell bcch to use trm reason as demod page when other sub in transfer
16/12/13   ap        CR590162: FR 15265: Temporarily remove the DSDA to DSDS dynamic mode switching from GL1
13/12/13   ap        CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
15/10/13   mko       G2W_TA: Reschedule the TBF suspension if WL1 request for TRM when rx/tx is paused for TBF reconfig
10/10/13   mko       G2W_TA: if TBF susp is cancelled or aborted then Reschedule the TBF suspension for TRM_UNLOCK_REQUIRED events
21/11/13   sai       CR 576784: Clear G2W abort tracking variable prior to handling TRM request for GTA.
27/11/13   sk        CR579562 Ensure GBTA registrations are made when in GBTA Active state
12/02/13   sai       CR 571606: Multispace G2W abort tracking variable,ensure G2W TA abort happens
                     only in W + G mode.
21/10/13   sai       CR563915: Facilitate G2W TA after TBF suspend for BCCH decode.
15/11/13   ap        CR575548 FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS
31/10/13   sai       CR567424: Enhance G2W TA suspension mechanism.
24/10/13   sai       CR565751: Fix reference FN for G2W TA suspension reschedule mechanism.
05/11/13   sk        CR566307 Do not schedule PCH suspension when TBF already suspended
01/11/13   sai       CR569985: Set G2W TA at higher priority as compared to Ncell FCCH/SCH decode
23/10/13   sk        CR565563 Correcting TRM event for secondary chain requests
21/10/13   ap        CR564181 GL1 should not attempt to schedule a TBF suspension for 1x
15/08/13   sk        Remove GTA stale check
15/10/13   mko/sai   CR561014: G2W_TA: Reschedule the TBF suspension if WL1 request for TRM when rx/tx is paused for TBF reconfig
10/10/13   mko/sai   CR555133: G2W_TA: if TBF susp is cancelled or aborted then Reschedule the TBF suspension for TRM_UNLOCK_REQUIRED events
30/09/13   aga       CR547585 - G2W - g2w_tbf_abort_callback_received not reset at TRAN->IDLE transition
25/09/13   aga       CR551779 - G2W - go back to GTA legacy subtract 2 frames from the suspension frame number
                                avoid going in G2W tuneaway gap for TSTS
25/09/13   aga       CR547585 - G2W - avoid trm_gta_preemption_callback processing while suspended / suspend in ISR
14/09/13   ap/aga    CR545423 - G2W Tune Away Feature
02/09/13   nm        CR530500 - propagation of below CRs from Nikel
                     CR481271 DSDS: G2W activity incorrectly allowed in GTA gap causing crash
                     CR472764: DSDS GTA don't configure any handlers if we are is suspension mode
                     CR458894 DSDS Prevent G2W measurement and ncell acquisition simultaneously
21/08/13   am        CR530958 Ensure tbf release is processed when TBF abort is triggered for RACH on idle sub
20/08/13   ab        CR 529794 GBTA Enhancement: facilitate PAGE decode on Idle SUB
                      when Other SUB is in GBTA and doing NCell decode
15/08/13   sk        CR524039 GBTA bringup changes
17/06/13   cgc       CR486358 3G_SEARCH_PRIO is now deprecated
08/05/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
02/07/13   sk        CR507513 TRM advanced call back integration to Triton-TSTS
21/06/13   cs        Major Triton TSTS Syncup
14/06/13   cs        TSTS Changes for DS/TS mode detect
06/06/13   cs        Further TSTS Updates
22/05/13   cs        TSTS Updates
28/05/13   ap        CR461268: DSDS XO management + async time drift problem
21/04/13   am        CR476324 TBF release and RRBP received in GTA gap
30/04/13   ws        CR481359 - GTA reduce PCH FN for suspension by 2 frames to ensure
                     PTM is set to BG_TRAFFIC before PCH trm_request()
29/04/13   ws        CR481359 enable GTA support
26/03/13   cja       CR448231 Correct CR398356 for trans mode NCell SCH outside gap
11/02/13   og        CR451559. More changes to distinguish the correct gas id.
27/10/11   ap        DSDS CR315426: separate timing adjustments for TS changes
29/01/12   ss        CR398356  Adjust qbit lag and fn lag when qbit lag is negetive or greater than 5000QS
21/03/12    jj      CR340963 Avoid scheduling g2x gap when frame is slammed
26/03/12   pa        CR347049 Added gpl1_gprs_calc_pkt_txfr_gap.
14/10/11   ab        CR 295030 L1 in packet transfer should not autonomously clear bcch_mask
                     used to acquire SI set.
29/06/11   nt        DSDS CR289454: ASYNC should ignore timing adjustments for CS and PS connections
15/04/11   ws        CR280277 -  Make sure Ncell BCCH FN is not before End of current suspension
                     window. Otherwise can result in FW error
22/03/11   ap        CR279430 DSDS Timing value wraps casusing frame misalignment
27/01/11   dmb       The following RR<->L1 messages are removed (not used):
                     MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
11/01/11   cgc       Fix compiler warning add cast #2411
03/08/10   cja       CR247307 Prevent attempt to stop BCCH read once TBF is alreay suspended
30/06/10   cja       CR243878 Correct scheduling of SCH if not 52 frames after FCCH detection
18/06/10   nt        CR:243318 ensure that SCE isn't stuck upon TBF suspend abort
10-03-16   tjw       Renamed UNUSED to __UNUSED_ARG to avoid name clash.
19/05/09   nt        CR178773: Added check when scheduling scell bcch that it occurs
                     after TBF resumption, if TBF is currently suspended.
24/02/09   ap        CR 167896 When l1 cancels serving cell sys info reads it also
                     cancels any previously scheduled serving cell sys info reads.
13/02/09   ap        Lint fixes for high Warning
25/11/08   ap        CR 163610 PSHO UE schedules but does not send out PCCF after PSHO failure
02/10/08   cs        Lint fixes
17/09/08   nt        Re-Introduce the CR88026
22/08/08   ap        Fix for FEATURE_GSM_GPRS_READ_CCCH_IN_XFER checkin
17/08/08   ap        Add FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
28/07/98   hv        Added KxMutex support
22/04/08   og        Back out NCELL SCH changes CR 88026.
11/04/08   og        NCELL SCH changes to support a wider range of offsets. CR 88026.
07/03/08   ip        Added PSHO changes under FEATURE_GPRS_PS_HANDOVER
06/06/07   ws        CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                     is called when we reselect to PBCCH cell stopping NC2 reporting
05/03/07   cs        Making CR112989 calculation consistent with sc updates
28/02/07   dv        Fix for CR94002:unnecessary scheduling of FB searchs.
26/05/06   og        Enforce the 10 seconds bsic validity criterea, addresses CR81029.
04/05/06   nt        CR66476 and CR92528 - Cell ID tick is now updated in transfer
                     mode using the id_tick that gets ticked on the IDLE frames of the
                     PDTCH. The TBF suspend for ID is also scheduled off the idle ID Tick
                     to keep IDLE and TRANSFER mode cell ID's correctly spaced at 25secs.
26/01/06   og        Neighbour cell acquisition changes to handle extended dynamic.
16/11/05   og        Re-organise and regroup data and functions.
16/09/05   og        Send a sysinfo read failure when transitioning from transfer
                     back to idle.
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
06/04/05   og        Lint warning removal.
03/01/05   kf        Changes for wcdma id searches in transfer.
17/01/05   ws        Lint updates
13/1/05    og        MS timebase adjustements confined to radio block gap.
20/12/04   ws        When leaving transfer inform GRR if we have SCELL BCCH read in progress
                     CR 53476
13/12/04   og        Removal of single block flag from gprs change timeslot function.
28/10/04   ws        Lint tidy up
25/10/04   bm        Fix for L1 Idle abort, in a race condition resulted in SCE STRAT, STOP
                     couple of times in same frame
13/10/04   kf        changes to ncell sync's for ts slamming.
26/08/04   kf        changes to ncell sync's for ts slamming.
14/07/04   gw        Added function gpl1_gprs_sc_trans_post_sort_process().
08/03/04   dp        Fixed bug in computing deltafnlost
21/07/04   kf        removed code that was setting ncell fn incorrectly in
                     gpl1_sc_adjust_lag_trans().
08/06/04   kf        changed gpl1_gprs_schedule_sch_if_allowed(). for the case
                     where ncell always clashes.
25/05/04   ws        Added adjustment of fn_lag in gpl1_sc_adjust_lag_trans()
24/05/04   ws        modifications to gpl1_gprs_schedule_sch_if_allowed() to fix
                     BSIC reconfirmation issues
23/06/04   og        Egprs Qual measurements handling.
13/05/04   rm        Updated lint warnings and clean up merge problem
12/05/04   gw        Send SURROUND_UPDATE_FAILURE_IND if BSIC changes while SU
                     is pending.
07/05/04   og        Removal of Lint warnings.
07/05/04   kf        Removed 4 sec delay if sch is blocked.
28/04/04   rm        Replaced RRBP codes with new RRBP function calls to RRBP module
09/03/04   ws        Added clearing of trans.bcch_in_progress when processing
                     read sys info request
02/25/04   gw        Changes to support GSM L1 lint clean up.
02/02/04   kf        lowered debug msg's
02/02/04   kf        Added support for serve cell bcch reading transfer.
23/01/04   kf        Added check for new or old to be used parameters from ts reconfig.
26/11/03   ws        Lint cleanup
07/11/03   gfr       Quad band support.
05/11/03   kf        added the use of SIZE_OF_RRBP_BUFF
03/09/03   sw        Enhancements to WCDMA meas during transfer
29/08/03   kf        Changes for sceduling of fch/sch.
08/28/03   gw        Changes to support new result type returned by l1_sc_read_sb
                     functions.
18/08/03   sw        Added WCDMA measurements during transfer
15/08/03   kf        added reseting of gpl1_gprs_meas_mode.allowed_to_go in
                     ncell bcch/pbcch callbacks.
12/08/03   kf        Removed sch_timing_error.
05/08/03   kf        Added code for adjusting ncell fn in sch callback.
31/07/03   bk        Changed the mechanism for detecting RRBP conflicts with SCH.
14/07/03   gw        Removed ASSERTs.
14/07/03   kf        added cell->pbcch == TRUE check to pbcch_done,
                     added fh arfcn for  ncell pbcch.
11/07/03   bk        Undid the changes but still check against 1 TS less than before.
11/07/03   bk        Checking the NCELL SCH clash against the highest Rx instead of highest Tx. This will
                     widen the window to decode the SCH.
11/07/03   rr        don't knock off Tx burst for SCH reconfirmations
10/07/03   kf        allow fch/sch to continue while bcch sceduled
10/07/03   kf        added field to L1_send_MPH_SURROUND_UPDATE_FAILURE_IND
10/07/03   gw        Changed #defines for time intervals to reflect length of
                     reporting period.
09/07/03   gw        Added sending of MPH_SURROUND_FAILURE_IND.
08/07/03   bk        Added support to not reconfirm the BSIC when the SCH clashes with the
                     Tx
08/07/03   bk        Moved SCH to the beginning of the window if it clashed with the Rx
                     following the idle frame and cleaned the "sch_allowed" function
08/07/03   kf        Changed max running av length
08/07/03   gw        GRR/L1 SCE cleanup. Changes to ncell BCCH decoding.  Changed order of clauses
                     in gpl1_gprs_next_cell_trans() cells in top six are done
                     first.  Added logging of acq. start. for cells not in top
                     six.
23/06/03   gfr       Compiler warning cleanup.
10/06/03   kf        changed running average size and number of sorted cells
28/05/03   npr       MSG_HIGH in gpl1_gprs_ncell_bcch_read() changed to MSG_LOW
                     "SCH scheduling error" msg now MSG_LOW from ERROR.
                     MSG_ERROR in gpl1_gprs_sc_bad_sb_trans() changed to LOW
27/05/03   dp        Further optimized Ncell SCH windows
23/05/03   ws        Update for BSIC transfer feature
19/05/03   kf        bug fix clean up.
15/05/03   kf        changed TEN_SECONDS to 75.
14/05/03   kf        sch fix.
05/12/03   dp        Added additional info to Ncell SCH debug msgs
04/29/03   dp        Changes to preserve Ncell BSIC across transitions
02/05/03   gw        Merged in power transfer changes from branch.
02/05/03   ag        F3 Messaging in gpl1_gprs_next_cell_trans() reduced from HIGH
                     to LOW - reinstated as overwritten in previous check-in.
01/05/03   ag        Updates to ncell pbcch reading.
28/04/03   nr        F3 Messaging in gpl1_gprs_next_cell_trans() reduced from HIGH
                     to LOW, and number of handled cells restricted to 16 from
                     maximum of 32, in order to reduce ISR loading.
28/04/03   gw        Changes to preserve ncell power measure info.  Added
                     acquisition logging.
24/04/03   ag        Remove call to l1_sci_abort_bcch from
                     gpl1_gprs_update_bcch_trans
24/04/03   kf        modified surround cell ticking.
09/04/03   ws        Increased size of rxlev average buffer and tidy up of surround
                     cell engine to avoid possible lockup scenarios
04/09/03   gw        Added support for transferring ncell power measurement info
                     between L1 modes.
04/04/03   gw        Added code to explicitly reset ncell info when entering and
                     leaving Transfer Mode.  Later this will be replaced with
                     code to support transferring ncell info between modes.
02/04/03   gw        Changes to reflect modifications in monscan module.
26/03/03   pjr       Started tidy up of SB scheduling and adjusted window size.
19/03/03   ws        Fixed bug with scheduling SB's
18/03/03   ws        Modified window for scheduling SB
17/02/03   kf        Changes for running average in transfer. Also changes to stop
                     sch conflicting with Rx.
07/02/03   ag        Updates to FEATURE_GPRS_TBF_SUSPEND
03/02/03   ws        Merged SC changes from branch
16/01/02   ws        Updates to sc fcb detection as per KF request
10/01/03   gw        Changed some SCE function calls to reflect renaming of some
                     BA list functions done as part of SCE clean up.
18/12/02   ag        Handle gprs_surround_bcch_info.si_requested under switch
                     FEATURE_GPRS_TBF_SUSPEND
16/12/02   pjr       Added setting of gprs_serv_cell_meas.idle_slot_free flag.
13/12/02   ws        Fixed Null pointer with strong_sch_unknown_cell
02/12/02   ws        Updated scheduling of sb bursts
22/11/02   ag        General improvements to NCELL BCCH reading under switch
                     FEATURE_GPRS_TBF_SUSPEND
12/11/02   ag        Added PL1 GPRS TBF suspension for NCELL BCCH reading code
                     hooks under switch FEATURE_GPRS_TBF_SUSPEND.
08th Oct 2002 Keith fallows  Initial Version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/* Alphabetical Order */
#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif
#include "l1_isr.h"
#include "l1_log.h"
#include "l1_sc.h"
#include "l1_task.h"
#include "l1_sc_int.h"
#include "l1_sc_irat.h"
#include "l1_utils.h"
#include "gpl1_gprs_isr.h"
#include "l1_task.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gmactimers.h"

#ifdef FEATURE_GPRS_PS_HANDOVER
#include "gpl1_gprs_task.h"
#endif

#include "gpl1_gprs_utils.h"
#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"

#include "gpl1_grm_intf.h"

#include "geran_multi_sim_api.h"

#include "gpl1_dual_sim.h"
#include "geran_multi_sim.h"
#include "mcfg_nv_api.h"
#ifdef FEATURE_G2X_TUNEAWAY
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_rrbp.h"
#include "geran_nv.h"
#endif
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/


/*========================== #defines =====================================*/

/* Reporting period in transfer is 13 frames. */
/* Making values multiples of 4 for reasons
 * that are not fully understood yet. GW
 */

/* (10000 / 4.615 / 13) */ // kf was 164
#define TEN_SECONDS  164

/* (4000 / 4.615 / 13) */
#define FOUR_SECONDS  64

#define MAX_NCELLS_TO_DECODE  10
#define MAX_TRIES             4


#define NUM_ID_TICKS  252

#define TBF_MIN_DURATION_MS 40
#define FRAMES_UNLOCK_REQ_FIRST_SCHEDULE 10
#define NOT_A_FRAME_DURATION 0xFFFF

#define DEFAULT_DUR_TRM_UNLOCK 20
#define TBF_RESUME_GUARD_FRAMES 2

#define MIN_GAP_FOR_SCHEDULING_PCH_IN_TRANSFER  10

#ifdef FEATURE_G2L_TUNEAWAY  
  /* This macro indicates the duration between the previous G2X gap end time to current 
   * G2X gap start time, which will be used to check the requirement where 2 back to back
   * G2X gaps are not opened if duration between them is less than 100ms.
   * 100ms = ~21.66 GSM frames, so used the rounded value 22.
   */
#define MIN_DURATION_BETWEEN_G2X_GAPS  22
#endif /* FEATURE_G2L_TUNEAWAY */

#ifdef FEATURE_GTA_2PHASE_ACCESS
#define GTA_TDS_INIT { L1_GPRS_INVALID_FN, FALSE, FALSE }
gta_2p_susp_info_t gta_2p_susp_info[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(GTA_TDS_INIT) };
extern boolean gpl1_gprs_get_single_block_receive(gas_id_t gas_id);
#endif /*FEATURE_GTA_2PHASE_ACCESS*/
extern gpl1_g2x_ta_disable_rx_tx_state g2x_ta_seq_trm_state[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GPRS_GBTA
extern boolean ignore_xcch;
#endif /* FEATURE_GPRS_GBTA */

/*========================== External Funcs ============================*/
void grm_gta_preemption_callback(
       trm_unlock_callback_data *unlock_data_p );

/*========================== Macros =======================================*/

/* None are defined so far */

/*========================== Local types/enums ============================*/

typedef struct
{
  cell_T* serve_cell;
} gpl1_gprs_serv_cell_T;

/*========================== Local function declarations ==================*/

static uint8  gpl1_gprs_calc_running_avg_length_trans( uint8  cell_count );
static void   gpl1_gprs_next_cell_trans( gas_id_t gas_id );

static void   gpl1_gprs_schedule_sch_if_allowed ( cell_T*  cell, gas_id_t gas_id );

static void   gpl1_gprs_fcb_done_trans( cell_T*  cell, gas_id_t gas_id );

static void   gpl1_gprs_sb_done_trans( cell_T*  cell, gas_id_t gas_id );

static void   gpl1_gprs_bcch_done_trans( cell_T*  cell, gas_id_t gas_id );

static void   gpl1_sc_adjust_lag_trans( l1_sc_bs_T*  bs,
                                        sword        timing_error, gas_id_t gas_id );
extern gpl1_g2x_ta_data_sub_act_info_t tbf_susp;
extern boolean gpl1_g2x_ta_block_bond( uint32 start_fn, tbf_suspend_T *suspension_time_ptr,  gas_id_t gas_id );
gpl1_g2x_ta_data_t  gpl1_g2x_ta_data[NUM_GERAN_DATA_SPACES];

extern boolean gpl1_g2x_ta_deregister_seq(gas_id_t gas_id);

extern  gpl1_gprs_meas_mode_T gpl1_gprs_meas_mode[];

extern void (*gpl1_g2x_ta_seq[NUM_GERAN_DATA_SPACES])(gas_id_t);

#ifdef FEATURE_G2X_TUNEAWAY
/* 2frames offset is needed to add sequencer for time slot change */
#define FRAME_OFFSET_TS_CHANGE 2

static void grm_tbf_G2X_abort_callback(
  grm_client_enum_t               tbf_client_id,
  grm_client_enum_t               winning_client_id,
  trm_unlock_event_enum_t         event,
  uint32                          unlock_in_sclk,
  trm_reason_enum_t               winning_client_reason,
  uint32                          winning_client_duration,
  trm_tuneaway_enum_type          ta_info
);

void gpl1_g2x_ta_update_trm_callback_data(
  grm_client_enum_t               tbf_client_id,
  grm_client_enum_t               winning_client_id,
  trm_unlock_event_enum_t         event,
  uint32                          trm_suspension_fn, 
  trm_reason_enum_t               winning_client_reason,
  gas_id_t gas_id
);

extern void gl1_hw_tbf_G2W_tune_back_RF(gas_id_t gas_id);

#endif /* FEATURE_G2X_TUNEAWAY */

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
extern void gpl1_g2x_ta_seq_serv_ccch(gas_id_t gas_id);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER  */

#ifdef FEATURE_GPRS_GBTA
static boolean gl1_gbta_handle_abort_callback(
  grm_client_enum_t               tbf_client_id,
  grm_client_enum_t               winning_client_id,
  trm_unlock_event_enum_t         event,
  uint32                          unlock_in_sclk,
  trm_reason_enum_t               winning_client_reason,
  gas_id_t                        gas_id
 );
#endif /* FEATURE_GPRS_GBTA */

uint32  calculate_gap_to_next_page( idle_data_T  *idle_data_ptr,byte   paging_mode,gas_id_t gas_id );
boolean gpl1_g2x_prioritze_unlock_over_wid_search(gas_id_t gas_id);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
ARFCN_T NULL_ARFCN_VAL = {0};

void gl1_disable_ccch_in_transfer(gas_id_t gas_id);
void gpl1_gprs_compute_ccch_event_qbit_lag(gas_id_t gas_id);

void gpl1_get_cell_information(int16 *qbit_lag,ARFCN_T *arfcn,gpl1_g2x_ta_priority_t pri,gas_id_t gas_id);

static byte page_mode_pchxfer[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NORMAL_PAGING)};

#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

/*======================== Local Data =====================================*/

/*
 * How many idle slots do we have to wait before an SB comes along.
 * Indexed by the neighbours frame number % 51.
 */
static const byte  gpl1_gprs_next_chance_to_get_sb[ ] =
{
  1,0,9,8,7,6,5, 4,3,2,
  1,0,9,8,7,6,5, 4,3,2,
  1,0,9,8,7,6,5, 4,3,2,
  1,0,9,8,7,6,5, 4,3,2,
  1,0,10,9,8,7, 6,5,4,3,2
};

/* This is used to allow adjustment of the FCCH/SCH window
 * size depending on the highest transmit timeslot in the
 * block period preceding the IDLE frame.
 */
static uint32  l1_highest_rx_timeslot[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0xFF) };
static uint32  l1_highest_tx_timeslot[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0xFF) };

static gpl1_gprs_serv_cell_T  gpl1_gprs_serv_cell[NUM_GERAN_DATA_SPACES];

boolean  bcch_mask_ext[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_G2X_TUNEAWAY
#define TBF_ABORT_INIT { GRM_NO_CLIENT, TRM_UNLOCK_CANCELLED, 1,  TRM_NUM_REASONS }

gpl1_gprs_tbf_abort_cb_store_data_t gpl1_gprs_tbf_abort_cb_store_data[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(TBF_ABORT_INIT) };

#endif

#define TBF_MIN_DURATION_FRAMES 8


static uint16  id_tick[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
grm_pchxfer_dr_state_t pchxfer_dr_state = GRM_PCHXFER_DR_NONE;
#endif
#endif


/* This structure is externed for use by other PL1 modules
 * both at the GPRS and GSM levels.
 */
trans_T  trans[NUM_GERAN_DATA_SPACES];

/*======================== Imported global data ===========================*/

extern const uint16                 bcch_mask_clear[];

/*========================= Local function Prototypes =====================*/

/* This function calculates the number of power monitors that need to be performed
 * on a particular neighbour cell during packet transfer before the acquisition
 * attempts can be performed.
 */
static uint8  gpl1_gprs_calc_running_avg_length_trans( uint8  cell_count )
{
  uint8 samples = 0;

  if (cell_count <= 8)
  {
    samples = 84;
  } else if ((cell_count > 8) && (cell_count <= 12))
  {
    samples = 74;
  } else if ((cell_count > 12) && (cell_count <= 16))
  {
    samples = 63;
  } else if ((cell_count > 16) && (cell_count <= 20))
  {
    samples = 52;
  } else if ((cell_count > 20) && (cell_count <= 24))
  {
    samples = 41;
  } else if ((cell_count > 24) && (cell_count <= 28))
  {
    samples = 30;
  } else if ((cell_count > 28) && (cell_count <= 32))
  {
    samples = 19;
  }

  return samples;
}

/* This function inspects the neighbour cell list and makes the decision
 * on which one the acquisition attempts should be performed on next.
 */
static void gpl1_gprs_next_cell_trans( gas_id_t gas_id )
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[gas_id];


  dword   pre_idle_frame_no_mod_52 = 23;
  cell_T *cell_above_pwr_threshold;
  dword   next_pre_idle_fn;
  cell_T *cell;
  word    fn_mod_51;
  word    n;
  uint8   num_cells;
  int32   deltafnlost = 0;
  int32   max_deltafnlost_top3 = 0;
  int32   min_deltafnlost_bottom3 = 0x7fffffff;
  int32   max_deltafnlost_bottom3 = 0;
  int32   min_deltafnlost_outOftop6 = 0x7fffffff;
  int32   max_deltafnlost_outOftop6 = 0;
  boolean tone_in_progress = FALSE;
  uint32  fn_lag, gprs_gpl1_sc_fn, next_sb_fn;
  uint32  delta_sb_to_cur_fn;

  cell_T *strong_tone_unknown_cell_top3 = NULL;
  cell_T *strong_tone_unknown_cell_bottom3 = NULL;
  cell_T *strong_sch_unknown_cell = NULL;
  cell_T *tone_unknown_cell_outOftop6 = NULL;
  cell_T *sch_unknown_cell = NULL;
  cell_T *known_confirmed_cell = NULL;
  cell_T *known_unconfirmed_cell = NULL;
  cell_T *known_fading_cell = NULL;

  boolean wcdma_srch_scheduled = FALSE;
  boolean wcdma_srch_pending = FALSE;
  boolean tbf_already_suspended = FALSE;
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  boolean b_ccch_in_xfer_as_gbta = FALSE;
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
  uint32 next_suspension_FN, next_resume_FN;
  gpl1_g2x_ta_priority_t pri;
  gpl1_g2x_ta_data_t *data_ptr = &gpl1_g2x_ta_data[gas_id];

  boolean g2l_timed_search_active = FALSE;
  
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
  boolean is_two_phase_access = FALSE;
#endif

  idle_data_T *l1_idle_data = get_ptr_to_idle_data(gas_id);

  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
  grm_client_enum_t g_client = gl1_map_gas_id_to_client_id(gas_id);
  gpl1_gprs_meas_mode_ptr->load_handler = FALSE;

  /* Disable serving cell interference measurements
   * on the IDLE frame which is 3 frames ahead.
   */
  gprs_serv_cell_meas_ptr->idle_frame_free = FALSE;

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
  is_two_phase_access = IS_TWO_PHASE_ACCESS(gas_id);
#endif

  ASSERT_TASK();

  /* Clip the number of cells to decode to  MAX_NCELLS_TO_DECODE (10) */
  num_cells = (l1_sc_globals_ptr->sorted_cell_count > MAX_NCELLS_TO_DECODE) ?
     MAX_NCELLS_TO_DECODE :
     l1_sc_globals_ptr->sorted_cell_count;

  /* Check for a tone detect in progress and clear out tone detects for weaker cells or cells
     outside the top MAX_NCELLS_TO_DECODE
  */
  for (n = 0; n < l1_sc_globals_ptr->sorted_cell_count; ++n)
  {
    cell = l1_sc_globals_ptr->sorted_cells[n];

    if (n >= MAX_NCELLS_TO_DECODE || !l1_sc_rx_power_acceptable(cell->bs.rx_power))
    {
      if (cell->try_no > 0)
      {
        cell->try_no = 0;
        l1_log_ncell_acq(L1_LOG_NCELL_ACQ_FAIL_FCCH,
                         cell->rank,
                         cell->bs.arfcn,
                         cell->bs.rx_power, gas_id);

      }
    } else if ( cell->try_no > 0 && !tone_in_progress && cell->bs.bsic_known == FALSE  )
    {
      tone_in_progress = TRUE;
      if (cell->rank < 3)
      {
        strong_tone_unknown_cell_top3 = cell;
      } else if ( cell->rank < 6 )
      {
        strong_tone_unknown_cell_bottom3 = cell;
      } else
      {
        tone_unknown_cell_outOftop6 = cell;
      }
    }
  }

  /* Check for the boundary of the id_tick */
  if (id_tick[gas_id]++ >= NUM_ID_TICKS)
  {
    id_tick[gas_id] = 0;
  }

  /* Tick the ID counters of the WCDMA cells */
  if (
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
      (is_two_phase_access == FALSE) && 
#endif
      l1_sc_wcdma_tick_idle_id_in_tbf(id_tick[gas_id], gas_id))
  {
    /* No need to check Rf device as Gl1 won't perfrom IRAT
       if band registration fails later. */
    wcdma_srch_pending = TRUE;
    MSG_GERAN_HIGH_1_G("OK to Start ID search in TBF - %d", gl1_get_FN(gas_id));

    trans[gas_id].wcdma_cell_id_go = TRUE; //l1_sc_wcdma_start_id_in_tbf();
  }


  if (trans[gas_id].wcdma_cell_id_go)
  {
    uint32 cell_id_fn;

    trans[gas_id].wcdma_cell_id_sceduled = TRUE;
    cell_id_fn = gl1_get_FN(gas_id) + 15;



    (void)gpl1_g2x_ta_update_table_field(G2X_TA_PRI_WID_SEARCH, g_client, cell_id_fn, NOT_A_FRAME_DURATION, gas_id);
    data_ptr->non_trm_act.wIDsearch_fn = cell_id_fn;
    gpl1_g2x_ta_scheduler(gas_id);

    MSG_GERAN_HIGH_2_G("sched cell_id %d af FN:%d", cell_id_fn, gl1_get_FN(gas_id));


  }

  else if (!gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_BCCH, gas_id))
  {
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    b_ccch_in_xfer_as_gbta =  TRUE;
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
    if (gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans == FALSE)
    {
      uint32 new_bcch_fn;

      for (n = 0; n < num_cells; ++n)
      {
        /* Dont need to check l1_sc_globals.autonomous_bcch since
         * we know it will be FALSE in Transfer Mode.
         */
        cell = l1_sc_globals_ptr->sorted_cells[n];
        if (cell->sb_count_to_bcch == BCCH_DO_NOW)
        {

          new_bcch_fn = gpl1_gprs_get_bcch_fn(cell->bs.fn_lag, cell->bs.qbit_lag, cell->bcch_mask, gas_id);
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
          if (b_ccch_in_xfer_as_gbta)
          {
            gpl1_gprs_meas_mode[gas_id].allowed_to_go = FALSE; /*  will prevent disable_rx_tx susp interferance */
            data_ptr->non_trm_act.nbcch_fn = L1_GPRS_INVALID_FN;  // as we are not suspending
          }
          else
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
          {
            data_ptr->non_trm_act.nbcch_fn = new_bcch_fn;
          }

          (void)gpl1_g2x_ta_update_table_field(G2X_TA_PRI_SERV_BCCH,
                                               gl1_map_gas_id_to_client_id_pchxfer(gas_id),
                                               new_bcch_fn, NOT_A_FRAME_DURATION, gas_id);
          data_ptr->non_trm_act.nbcch_fn = new_bcch_fn;
          gpl1_g2x_ta_scheduler(gas_id);


          MSG_GERAN_MED_4_G("sched nbcch %d SURROUND ARFCN %d qbit_lag %d c_fn[%d]", new_bcch_fn, cell->bs.arfcn.num, cell->bs.qbit_lag, gl1_get_FN(gas_id));

          trans[gas_id].bcch_cell        = cell;
          trans[gas_id].new_bcch_fn      = new_bcch_fn;
          /* Only do one cell at a time. */
          break;
        }
      }
    }
    else if (gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans == TRUE)
    {

      uint32 new_bcch_fn;
      uint8 ccch_tn;
      sword qbit_decrease;


      ccch_tn = (uint8)(l1_idle_data->paging_data.ccch_group << 1);
      qbit_decrease = (sword)((ccch_tn - l1_get_current_timeslot(gas_id)) * 625);

      if (qbit_decrease < 0)
      {
        qbit_decrease += QS_PER_FRAME;
      }
      if (ccch_tn >= l1_get_current_timeslot(gas_id))
      {
        gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag  = (uint32) - 1; //-2 + 1
      }
      else
      {
        gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag     = (uint32) - 2;
      }
      gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag  = (int16)qbit_decrease;


      new_bcch_fn = gpl1_gprs_get_bcch_fn(gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag,
                                          gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag, gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask,
                                          gas_id);

      if (bcch_mask_ext[gas_id] == TRUE)
      {
        new_bcch_fn += 4;
      }
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if (b_ccch_in_xfer_as_gbta)
      {
        gpl1_gprs_meas_mode[gas_id].allowed_to_go = FALSE; /*  will prevent disable_rx_tx susp interferance */
        data_ptr->non_trm_act.nbcch_fn = L1_GPRS_INVALID_FN; // as we are not suspending
      }
      else
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
      {
        data_ptr->non_trm_act.nbcch_fn = new_bcch_fn;
      }
      (void)gpl1_g2x_ta_update_table_field(G2X_TA_PRI_SERV_BCCH,
                                           gl1_map_gas_id_to_client_id_pchxfer(gas_id),
                                           new_bcch_fn, NOT_A_FRAME_DURATION, gas_id);
      data_ptr->non_trm_act.nbcch_fn = new_bcch_fn;
      gpl1_g2x_ta_scheduler(gas_id);

      MSG_GERAN_HIGH_3_G("sched scell bcch %d [%d] tbf_already_suspended:%d", new_bcch_fn, gl1_get_FN(gas_id), tbf_already_suspended);
      MSG_GERAN_MED_3_G("SERVING ARFCN bs.fn_lag bs.qbit_lag %d %d %d ", gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn.num, gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag, gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag);
      trans[gas_id].bcch_cell        = gpl1_gprs_serv_cell[gas_id].serve_cell;
      trans[gas_id].new_bcch_fn      = new_bcch_fn;
    }

  }

#if defined (FEATURE_G2X_TUNEAWAY) || defined (FEATURE_GPRS_GBTA)
  /* Keep same behavior as FEATURE_DUAL_SIM_GERAN_TUNEAWAY */
  if ((frame_counters[gas_id].FNmod52 ==  9) || (frame_counters[gas_id].FNmod52 ==  35))
  {
    /* Repeat scheduling for BCCH read and
       Idle sub's CCCH read is already achieved above. Don't
       want to schedule Idle frame activity as we are not close
       to the Idle frame. Need to set send_gprs_trans_tick so
       next cell trans can schedule again
     */
    l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;

    MSG_GERAN_HIGH_1_G("gpl1_gprs_next_cell_trans(): GTA early exit stop_sc_tick_trans=%d",  gpl1_get_stop_sc_tick_trans(gas_id));

    /*Check if MVCS boost req for G2L sent in the last frame, if yes sent de-boost now as no G2X will happen on the next idle frame*/
    l1_sc_check_for_g2x_mcvs_deboost_req(gas_id);
    return;
  }
#endif /* FEATURE_G2X_TUNEAWAY / FEATURE_GPRS_GBTA*/
  if (frame_counters[gas_id].FNmod52 == 22)
  {
    pre_idle_frame_no_mod_52 = 23;
  } else if (frame_counters[gas_id].FNmod52 == 48)
  {
    pre_idle_frame_no_mod_52 = 49;
  }

  gprs_gpl1_sc_fn = l1_sc_get_fn(gas_id);
  /*add 26+FRAMES_PER_HYPERFRAME so subtract of bs.fn_lag won't generate negative value   */

  next_pre_idle_fn = gprs_gpl1_sc_fn + 52 + FRAMES_PER_HYPERFRAME;
  next_pre_idle_fn -= (next_pre_idle_fn - pre_idle_frame_no_mod_52)MODULO 52;


  /* find next cell */

  for (n = 0; n < num_cells; ++n)
  {
    cell = l1_sc_globals_ptr->sorted_cells[n];

    /* If Ncell rx power < -112 dBm don't do a Ncell decode */
    if (l1_sc_rx_power_acceptable(cell->bs.rx_power))
    {
      cell_above_pwr_threshold = cell;
    } else
    {
      cell_above_pwr_threshold = NULL;
    }


    {

      if (cell->bs.bsic_known)
      {

        /* If cell is below absolute threshold or if the current rx lvl
           has dropped by more than 10dB from the last known level,
           start SCH decodes immediately.  This is to help us drop the
           cell faster
         */
        if ((cell_above_pwr_threshold == NULL) ||
            (!l1_sc_rx_power_drop_acceptable(cell->bs.rx_power - cell->bs.prev_rx_power)))
        {
          MSG_GERAN_LOW_2_G("Cell %d drop=%d Trig SCH", cell->bs.arfcn.num, cell->bs.rx_power - cell->bs.prev_rx_power);
          /* Decode at next available opportunity */
          cell->next_tick = l1_sc_globals_ptr->surround_tick;
          cell_above_pwr_threshold = cell;
          cell->in_fade = TRUE;
        }

        fn_lag = cell->bs.fn_lag;

        fn_mod_51 = (next_pre_idle_fn - fn_lag)MODULO 51;

        if (gpl1_gprs_next_chance_to_get_sb[fn_mod_51] == 0)
        {
           MSG_GERAN_HIGH_1_G("SCH scheduling fn_mod_51 %d",fn_mod_51);
          /* BSIC known but cell not confirmed within last 10 secs*/
          if (cell->next_tick <= l1_sc_globals_ptr->surround_tick)
          {
            if (cell->in_fade)
            {
              if (known_fading_cell == NULL)
              {
                known_fading_cell = cell_above_pwr_threshold;
              }
            } else
            {
              if (known_unconfirmed_cell == NULL)
              {
                known_unconfirmed_cell =  cell_above_pwr_threshold;
              }
            }

          } else
          {
            /* BSIC known and cell confirmed within the last 10 secs */
            if (known_confirmed_cell == NULL)
            {
              known_confirmed_cell =  cell_above_pwr_threshold;

            }
          }
        }
      } else if (cell->bs.tone_detected)
      {
        /*
         * Check if 
         * 1. SCH was expected in the past (gprs_gpl1_sc_fn > next_sb_fn)
         * 2. Or if SCH is expected in the future, but in the idle frame 
         *    of the current 52 multiframe (delta_sb_to_cur_fn < MULTIFRAME_26)
         * => then schedule it now
         */
        next_sb_fn = ADD_FN(cell->last_tone_fn, cell->next_sb_expected);
        delta_sb_to_cur_fn = SUB_FN(next_sb_fn, gprs_gpl1_sc_fn);

        if (IS_FRAME_NUM_LATER(gprs_gpl1_sc_fn, next_sb_fn) /* 1. sch was in the past     */
            || (delta_sb_to_cur_fn < MULTIFRAME_26))        /* 2. sch is in this 52 frame */
        {
          if (cell->rank < 6)
          {
            /* FCCH known, SCH unknown - cell in best 10 */
            if (strong_sch_unknown_cell == NULL)
            {
              strong_sch_unknown_cell = cell_above_pwr_threshold;
            }

          } else
          {
            /* FCCH known, SCH unknown - cell not in best 6 */
            if (sch_unknown_cell == NULL)
            {
              sch_unknown_cell = cell_above_pwr_threshold;
            }

          }

          /*
          Predict when the next SCH might be received
          in case the SCH is not successfully decoded
          Moved this here from good_sb and bad_sb because
          the cell is not guaranteed a SCH decode attempt
          could be pre-empted by a higher priority cell.  In that
          case, next_sb_expected would not be updated if it was in
          those functions.
          */
          if (cell->idle_frames_to_next_sb == 9)
          {
            cell->next_sb_expected += 2 * MULTIFRAME_26;
            cell->idle_frames_to_next_sb = 11;
          }
          else
          {
            cell->next_sb_expected += 9 * MULTIFRAME_26;
            if (cell->idle_frames_to_next_sb == 11)
            {
              cell->idle_frames_to_next_sb = 20;
            }
            else
            {
              cell->idle_frames_to_next_sb = 9;
            }
          }

        }
      } else if ( (cell_above_pwr_threshold != NULL ) && ( !tone_in_progress ) )
      {
        deltafnlost = (int32)SUB_FN(gprs_gpl1_sc_fn, (uint32)cell->fn_when_lost);


        if (cell->fn_when_lost > 0)
        {
          deltafnlost = (int32)SUB_FN(gprs_gpl1_sc_fn, (uint32)cell->fn_when_lost);
        } else
        {
          deltafnlost = (int32)ADD_FN(gprs_gpl1_sc_fn, (uint32)(-cell->fn_when_lost));
        }

        /* Find and schedule the unknown cell from each group that
           hasn't been searched in the longest time.  The groups are
           the top 3, bottom 3, and out of the top 6.  Also find the
           most recently searched unknown cell from the bottom 2 groups.
           Later we will choose when to cancel the higher power groups in
           favor of the lower power groups based on this information.
        */

        if (cell->rank < 3)
        {
          if (deltafnlost > max_deltafnlost_top3)
          {
            strong_tone_unknown_cell_top3 =  cell_above_pwr_threshold;
            max_deltafnlost_top3 = deltafnlost;
          }

        } else if (cell->rank < 6)
        {
          if (deltafnlost > max_deltafnlost_bottom3)
          {
            strong_tone_unknown_cell_bottom3 =  cell_above_pwr_threshold;
            max_deltafnlost_bottom3 = deltafnlost;
          }
          if (deltafnlost < min_deltafnlost_bottom3)
          {
            min_deltafnlost_bottom3 = deltafnlost;
          }

        } else
        {
          if (deltafnlost > max_deltafnlost_outOftop6)
          {
            tone_unknown_cell_outOftop6 =  cell_above_pwr_threshold;
            max_deltafnlost_outOftop6 = deltafnlost;
          }
          if (deltafnlost < min_deltafnlost_outOftop6)
          {
            min_deltafnlost_outOftop6 = deltafnlost;
          }

        }
      }

    } //if in ba_list


  } //for

  /* Cancel any scheduled top 3 tone unknown search if all cells from the top3 have been searched
     since the last bottom 3 search.  Keep in mind the default values if there are no unknown
     cells in a given group when editing this section.  If tone in progress there won't be multiple
     cells scheduled so this code doesn't change anything.
  */
  if (strong_tone_unknown_cell_bottom3 != NULL && min_deltafnlost_bottom3 > max_deltafnlost_top3)
  {
    strong_tone_unknown_cell_top3 = NULL;
  }

  /* Cancel any scheduled top 3 and bottom 3 tone unknown search if a cell out of the top 6
    needs to be searched.  The desired algorithm when all cells are unknown is:
            ABC D ABC E ABC F ABC G, repeat with H instead of G, etc
    where cell powers from strongest to weakest are A, B, etc.  If tone in progress there won't
    be multiple cells scheduled so this code doesn't change anything.
 */
  if (tone_unknown_cell_outOftop6 != NULL
      && min_deltafnlost_outOftop6 > max_deltafnlost_bottom3
      && min_deltafnlost_outOftop6 > max_deltafnlost_top3
      && min_deltafnlost_bottom3 > max_deltafnlost_top3)
  {
    strong_tone_unknown_cell_top3 = NULL;
    strong_tone_unknown_cell_bottom3 = NULL;
  }

  /* Attempt to schedule neighbour cell activities in the order
   * of priority. Start with the WCDMA since this is the highest
   * priority and finish off with FB acquisition on a NCELL which
   * is not one of the top six.
   *
   * If no acquisition attempt is scheduled then perform interference
   * measurement since they are the lowest priority measurements during
   * packet transfer mode.
   */

  /* For GTA we do not want to configure any  handlers if we are is suspension mode.
   * this prevents a handler configured for the idle sub being overwriiten by the transfer sub
   */


  if(gpl1_g2x_ta_is_suspension_ongoing(gas_id, &pri))
  {
    l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;

    MSG_GERAN_HIGH_1_G("gpl1_gprs_next_cell_trans(): GTA early exit stop_sc_tick_trans=%d",  gpl1_get_stop_sc_tick_trans(gas_id));

    /*Check if MVCS boost req for G2L sent in the last frame, if yes sent de-boost now as no G2X will happen on the next idle frame*/
    l1_sc_check_for_g2x_mcvs_deboost_req(gas_id);
    return;
  }

  if (gpl1_g2x_ta_next_planned_suspension(&next_suspension_FN, &next_resume_FN, gas_id))
  {

    if (IS_FRAME_NUM_LATER(ADD_FN(gl1_get_FN(gas_id), TBF_MIN_DURATION_FRAMES), next_suspension_FN))
    {
      l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;
      MSG_GERAN_HIGH_1_G("gpl1_gprs_next_cell_trans(): GTA early exit stop_sc_tick_trans=%d",  gpl1_get_stop_sc_tick_trans(gas_id));

    /*Check if MVCS boost req for G2L sent in the last frame, if yes sent de-boost now as no G2X will happen on the next idle frame*/
    l1_sc_check_for_g2x_mcvs_deboost_req(gas_id);

      return;
    }
#ifdef  FEATURE_G2X_TUNEAWAY
    /* If G2W TA reservation frame is less than 14 frames away,
    ** block Ncell FCCH tone detection
       To be reviewed
    */
    else if ((gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TRM_UNLOCK, gas_id))
             && ((gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_UMTS)
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_UMTS_SECONDARY)
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_UMTS2)
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_UMTS_SECONDARY2)
#ifdef FEATURE_G2_1X_TUNEAWAY
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_1X)
#endif
#ifdef FEATURE_G2L_TUNEAWAY
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE)
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SECONDARY)
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SUB2)
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SUB2_SECONDARY)
#endif /* FEATURE_G2L_TUNEAWAY */
                 || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_TDSCDMA))
             && (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event == TRM_UNLOCK_BY)
             && ((gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_sclk - timetick_get_safe()) <= 1968) /* 14 G frames */
             && (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason == TRM_DEMOD_PAGE)
             && (gl1_msg_get_multi_sim_standby_mode())
             && (GERAN_TA_SUPPORTED == geran_nv_g2w_ta_supported())
             )
    {
      MSG_GERAN_HIGH_3_G("gpl1_gprs_next_cell_trans: Block Ncell ACQ/BSIC/BSIC reconf for G2W TA, unlock_by:%d, curr_sclk:%d, reason:%d",
                         gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_sclk,
                         timetick_get_safe(),
                         gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason
                         );

      l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;
      trans[gas_id].reconfig_two_close = FALSE;

      /*Check if MVCS boost req for G2L sent in the last frame, if yes sent de-boost now as no G2X will happen on the next idle frame*/
      l1_sc_check_for_g2x_mcvs_deboost_req(gas_id);
      return;
    }
    else
    {
      MSG_GERAN_HIGH_3_G("gpl1_gprs_next_cell_trans: unlock_by:%d, curr_sclk:%d, reason:%d",
                         gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_sclk,
                         timetick_get_safe(),
                         gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason
                         );

      MSG_GERAN_HIGH_2_G("gpl1_gprs_next_cell_trans: last_client:%d, last_event:%d",
                         gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client,
                         gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event
                         );
    }
#endif /* FEATURE_G2X_TUNEAWAY */
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if (gl1_check_any_other_client_has_lock(gas_id) && (TRUE == gpl1_get_stop_sc_tick_trans(gas_id)))
  {
    l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;
    MSG_GERAN_HIGH_0_G("gpl1_gprs_next_cell_trans() early exit: other sub has the lock so don't schedule any irat or NCELL meas");

    /*Check if MVCS boost req for G2L sent in the last frame, if yes sent de-boost now as no G2X will happen on the next idle frame*/
    l1_sc_check_for_g2x_mcvs_deboost_req(gas_id);

    return;
  }
#endif  /* FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM */

  if (
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
      (is_two_phase_access == FALSE) && 
#endif
       l1_sc_wcdma_xfer_ready(TRUE, gas_id))
  {
    // 3G_SEARCH_PRIO is now deprecated, always behave as if TRUE

    // If 3G_SEARCH_PRIO is set by the network then the UE may
    // use 3 out of every 13 seconds of BSIC reconfirm time to
    // exclusively search for 3G cells.

    wcdma_srch_scheduled = TRUE;

    gpl1_gprs_meas_mode_ptr->load_handler = TRUE;
  }
  else
  {
    /*Check if MVCS boost req for G2L sent in the last frame, if yes sent de-boost now as no G2X will happen on the next idle frame*/
    l1_sc_check_for_g2x_mcvs_deboost_req(gas_id);
  }

#ifdef FEATURE_GSM_TO_LTE
  /* do not run SCE until all g2l_timed_searchs are completed*/
  if (l1_sc_g2l_timed_search_active(gas_id))
  {
    g2l_timed_search_active = TRUE;
    MSG_GERAN_HIGH_0_G("G2L search pending - do not run SCE");
  }
#endif

  /* Check that a Ncell acquisition handler has not been already
   * installed. If this is the case then attempt to do BSIC reconfirm
   * attempt.
   */
  if ((FALSE == gpl1_gprs_meas_mode_ptr->load_handler)
      && (known_fading_cell) && !g2l_timed_search_active
      )
  {
    gpl1_gprs_schedule_sch_if_allowed(known_fading_cell, gas_id);
  }

  /* Check that a Ncell acquisition handler has not been already
   * installed. If this is the case then attempt to do BSIC decode
   * attempt on the SCH channel.
   */
  if ((FALSE == gpl1_gprs_meas_mode_ptr->load_handler)
      && (strong_sch_unknown_cell) && !g2l_timed_search_active
      )
  {
    /*
      SCH unknown cells get high priority irrespective of whether they are
      in the top 6 or not.  Once you don't decode the SCH after the tone at
      the first available opportunity (2 idle frames) we fall out of sync
    */

    gpl1_gprs_schedule_sch_if_allowed(strong_sch_unknown_cell, gas_id);
  }

  /* Check that a Ncell acquisition handler has not been already
   * installed. If this is the case then attempt to do BSIC decode
   * attempt on the SCH channel.
   *
   * The difference with the previous SCH check is that this neighbour cell is
   * of lower receive power.
   */
  if ((FALSE == gpl1_gprs_meas_mode_ptr->load_handler)
      && (sch_unknown_cell) && !g2l_timed_search_active
      )
  {
    gpl1_gprs_schedule_sch_if_allowed(sch_unknown_cell, gas_id);
  }

  /* Check that a Ncell acquisition handler has not been already
   * installed. If this is the case then attempt to do BSIC reconfirm
   * attempt.
   */
  if ((FALSE == gpl1_gprs_meas_mode_ptr->load_handler)
      && (known_unconfirmed_cell) && !g2l_timed_search_active
      )
  {
    gpl1_gprs_schedule_sch_if_allowed(known_unconfirmed_cell, gas_id);
  }

  /*
  ** 3G_SEARCH_PRIO is now deprecated, always behave as if TRUE
  ** 2nd call to l1_sc_wcdma_xfer_ready(FALSE) removed
  */


  /* Check that a Ncell acquisition handler has not been already
   * installed. If this is the case then attempt to acquire FB of
   * a Ncell from the top 3 and ensure that it hasn't already been
   * acquired.
   */
  if ((FALSE == gpl1_gprs_meas_mode_ptr->load_handler)
      && (strong_tone_unknown_cell_top3) &&  !(strong_tone_unknown_cell_top3->bs.tone_detected)
      && !g2l_timed_search_active
      )
  {

    /* No FCCH decoded, cell in top 3 */

    /*
    Search 22 idle frames for the FCCH
    22 = 2 attempts * 11 since we are guaranteed to get a FCCH
    within 11 idle frames
    */
    if (
           (strong_tone_unknown_cell_top3->try_no <= MAX_TRANS_TONE_DETECT_ATTEMPTS)
        )
    {
      /* Trigger NCELL_ACQ_START only on first tone detect attempt */
      if (0 == strong_tone_unknown_cell_top3->try_no)
      {
        /* TAP POINT FOR NCELL_ACQ_START */
        l1_log_ncell_acq(L1_LOG_NCELL_ACQ_START,
                         strong_tone_unknown_cell_top3->rank,
                         strong_tone_unknown_cell_top3->bs.arfcn,
                         strong_tone_unknown_cell_top3->bs.rx_power, gas_id);

        MSG_GERAN_HIGH_2_G("Ncell Tone Start Cell: %d, Rank %d", strong_tone_unknown_cell_top3->bs.arfcn.num, strong_tone_unknown_cell_top3->rank);
      }

      strong_tone_unknown_cell_top3->try_no++;

      gpl1_gprs_sc_receive_fcb_trans(strong_tone_unknown_cell_top3, gpl1_gprs_fcb_done_trans, gas_id);

      gpl1_gprs_meas_mode_ptr->load_handler = TRUE;
    }
    else
    {
      /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
      l1_log_ncell_acq(L1_LOG_NCELL_ACQ_FAIL_FCCH,
                       strong_tone_unknown_cell_top3->rank,
                       strong_tone_unknown_cell_top3->bs.arfcn,
                       strong_tone_unknown_cell_top3->bs.rx_power, gas_id);

      strong_tone_unknown_cell_top3->try_no = 0;

      /* Save fn when cell was lost.  Used in scheduling */
      strong_tone_unknown_cell_top3->fn_when_lost = (int32)l1_sc_get_fn(gas_id);

      MSG_GERAN_MED_1_G("Ncell Tone detect failed: ARFCN = %d", strong_tone_unknown_cell_top3->bs.arfcn.num);

    }

  }

  /* Check that a Ncell acquisition handler has not been already
   * installed. If this is the case then attempt to acquire FB of
   * a Ncell from the bottom 3 and ensure that it hasn't already been
   * acquired.
   */
  if ((FALSE == gpl1_gprs_meas_mode_ptr->load_handler)
      && (strong_tone_unknown_cell_bottom3) && !(strong_tone_unknown_cell_bottom3->bs.tone_detected)
      && !g2l_timed_search_active
      )
  {

    /* No FCCH decoded, cell in bottom 3 */
    if (
         (strong_tone_unknown_cell_bottom3->try_no <= MAX_TRANS_TONE_DETECT_ATTEMPTS)
        )
    {
      /* Trigger NCELL_ACQ_START only on first tone detect attempt */
      if (0 == strong_tone_unknown_cell_bottom3->try_no)
      {
        /* TAP POINT FOR NCELL_ACQ_START */
        l1_log_ncell_acq(L1_LOG_NCELL_ACQ_START,
                         strong_tone_unknown_cell_bottom3->rank,
                         strong_tone_unknown_cell_bottom3->bs.arfcn,
                         strong_tone_unknown_cell_bottom3->bs.rx_power, gas_id);

        MSG_GERAN_HIGH_2_G("Ncell Tone Start Cell: %d, Rank %d", strong_tone_unknown_cell_bottom3->bs.arfcn.num, strong_tone_unknown_cell_bottom3->rank);
      }

      strong_tone_unknown_cell_bottom3->try_no++;

      gpl1_gprs_sc_receive_fcb_trans(strong_tone_unknown_cell_bottom3, gpl1_gprs_fcb_done_trans, gas_id);

      gpl1_gprs_meas_mode_ptr->load_handler = TRUE;
    }
    else
    {
      /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
      l1_log_ncell_acq(L1_LOG_NCELL_ACQ_FAIL_FCCH,
                       strong_tone_unknown_cell_bottom3->rank,
                       strong_tone_unknown_cell_bottom3->bs.arfcn,
                       strong_tone_unknown_cell_bottom3->bs.rx_power, gas_id);

      strong_tone_unknown_cell_bottom3->try_no = 0;

      /* Save fn when cell was lost.  Used in scheduling */
      strong_tone_unknown_cell_bottom3->fn_when_lost = (int32)gprs_gpl1_sc_fn;

      MSG_GERAN_MED_1_G("Ncell Tone detect failed: ARFCN = %d", strong_tone_unknown_cell_bottom3->bs.arfcn.num);

    }

  }

  /* Check that a Ncell acquisition handler has not been already
   * installed. If this is the case then attempt to acquire FB of
   * a Ncell out of the top 6 and ensure that it hasn't already been
   * acquired.
   */
  if ((FALSE == gpl1_gprs_meas_mode_ptr->load_handler)
      && (tone_unknown_cell_outOftop6) && !(tone_unknown_cell_outOftop6->bs.tone_detected)
      && !g2l_timed_search_active
      )
  {

    /* FCCH not decoded, cell out of top 6 */

    /*
      Search 22 idle frames for the FCCH
     22 = 2 attempts * 11 since we are guaranteed to get a FCCH
     within 11 idle frames
    */
    if (
         ( tone_unknown_cell_outOftop6->try_no <= MAX_TRANS_TONE_DETECT_ATTEMPTS)
        )
    {
      /* Trigger NCELL_ACQ_START only on first tone detect attempt */
      if (0 == tone_unknown_cell_outOftop6->try_no)
      {
        /* TAP POINT FOR NCELL_ACQ_START */
        l1_log_ncell_acq(L1_LOG_NCELL_ACQ_START,
                         tone_unknown_cell_outOftop6->rank,
                         tone_unknown_cell_outOftop6->bs.arfcn,
                         tone_unknown_cell_outOftop6->bs.rx_power, gas_id);

      }

      tone_unknown_cell_outOftop6->try_no++;

      gpl1_gprs_sc_receive_fcb_trans(tone_unknown_cell_outOftop6, gpl1_gprs_fcb_done_trans, gas_id);

      gpl1_gprs_meas_mode_ptr->load_handler = TRUE;

    }
    else
    {
      /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
      l1_log_ncell_acq(L1_LOG_NCELL_ACQ_FAIL_FCCH,
                       tone_unknown_cell_outOftop6->rank,
                       tone_unknown_cell_outOftop6->bs.arfcn,
                       tone_unknown_cell_outOftop6->bs.rx_power, gas_id);

      tone_unknown_cell_outOftop6->try_no = 0;

      /* Save fn when cell was lost.  Used in scheduling */
      tone_unknown_cell_outOftop6->fn_when_lost = (int32)gprs_gpl1_sc_fn;

      MSG_GERAN_MED_1_G("Ncell Tone detect failed: ARFCN = %d", tone_unknown_cell_outOftop6->bs.arfcn.num);

    }

  }


  /* Check that a Ncell acquisition handler has not been already
   * installed. If this is the case then attempt to perform
   * interference measurements.
   */
  if (FALSE == gpl1_gprs_meas_mode_ptr->load_handler)
  {
    /*  Allow serving cell interference measurements to be taken  */
    gprs_serv_cell_meas_ptr->idle_frame_free = TRUE;
  }

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
  if(is_two_phase_access == FALSE )
#endif
  {
    l1_sc_wcdma_xfer_tick(wcdma_srch_pending, wcdma_srch_scheduled, gas_id);
  }

  l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;
  trans[gas_id].reconfig_two_close           = FALSE;

}

/* This function checks the activities taking place before the IDLE frame
 * and then verifies whether or not it is possible to acquire a neighbour cell
 * based on the quarter bit offset.
 */
static void  gpl1_gprs_schedule_sch_if_allowed( cell_T*  cell, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);

      /* variables related to FR54762 */
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_scenario_status_e status;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

  /* Instantiate and initialise the lowest and highest timeslot
   * information structures.
   */
  const gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, /* Relative timeslot */
                                                FALSE          /* transmit boolean  */
                                              };
  gl1_defs_tn_struct        highest_tn_info = { GL1_DEFS_TN_0, /* Relative timeslot */
                                                FALSE          /* transmit boolean  */
                                              };
  /* Initialise the clash result */
  int  ncell_scell_ts_clash_result = 0;

  /* The lowest receive is always the current timeslot which is the camp timeslot.
   * Use the highest occupied timeslot which is either the highest receive or
   * the highest transmit.
   */
  uint32  highest_rel_rxtx_ts = l1_get_current_timeslot(gas_id);

  if( 0xFF != l1_highest_tx_timeslot[gas_id] )
  {
    /* Find the receive equivalent */
    highest_rel_rxtx_ts = ADD_MOD( l1_highest_tx_timeslot[gas_id],
                                   3,
                                   GL1_DEFS_SLOTS_IN_FRAME );

    /* Set the transmit boolean to TRUE so the timing advance
     * is taken into account when sizing the acquisition window.
     */
    highest_tn_info.transmit = TRUE;
  }
  else if( 0xFF != l1_highest_rx_timeslot[gas_id] )
  {
    highest_rel_rxtx_ts = l1_highest_rx_timeslot[gas_id];
  }

  /* @@Warning: Do not reset the l1 highest rxtx timeslot.
   * This is done on the next block boundary.
   */

  /* Convert the receive equivalent timeslot of the highest transmit
   * to be relative to the camp timeslot i.e. lowest absolute receive.
   */
  highest_rel_rxtx_ts = SUB_MOD( highest_rel_rxtx_ts,
                                 l1_get_current_timeslot(gas_id),
                                 GL1_DEFS_SLOTS_IN_FRAME );

  /* Set the highest tn info */
  highest_tn_info.tn = (gl1_defs_tn_type) highest_rel_rxtx_ts;

  /* allow SCH to knock off Tx burst */
  ncell_scell_ts_clash_result = gl1_msg_ncell_sch_pos_in_acq_window
                                (
                                cell->bs.qbit_lag,
                                &( lowest_tn_info ),
                                &( highest_tn_info ),
                                gas_id
                                );

  /* Check the clash result */

   /* FR54762 Inform MCPM about this scenario and check if we can go ahead with scheduling Ncell FCB in TCH */
   gl1_concman_config_params.type = CMGR_TRANSIENT_START;
   gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
   gl1_concman_config_params.update_scenario = 0;
   gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
   gl1_concman_config_params.start_timestamp = 0;
   gl1_concman_config_params.duration_msec = 0;
   gl1_concman_config_params.flow_ctrl_time_msec = 0;
   status = gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
   MSG_GERAN_HIGH_1_G("***sending scenario request to MCPM : status : %d", status);

  /* @@OG: Force scheduling of the MAX_TRY attempt */

  if(( cell->first_ncell_sch_attempt)||(  ( ncell_scell_ts_clash_result == 0 )
  ||( ( ncell_scell_ts_clash_result == -1 ) &&
    ( MAX_TRIES == cell->try_no )
  &&(FALSE == mcfg_gcf_nv_get_status())
     )
    ))
  {

    if(status == CMGR_ACCEPTED)
    {
    /* SCH is scheduled, the Tx could possibly been discarded */
    gpl1_gprs_sc_receive_sb_trans( cell, gpl1_gprs_sb_done_trans, gas_id );

    gpl1_gprs_meas_mode_ptr->load_handler = TRUE;
    cell->first_ncell_sch_attempt = FALSE;
   }
  else
    MSG_GERAN_HIGH_1_G("Ncell sch denied by MCPM for ARFCN : %d", cell->bs.arfcn.num);
   }
  else
  {
    /*----------------------------------------------------------------------------------------------
         If the BSIC is unknown then do label this as a bad SCH, instead assume that the timing info is
         valid and don't reconfirm.  Reschedule it after 4 seconds
      -----------------------------------------------------------------------------------------------*/
    if( cell->try_no < MAX_TRIES )
    {
      cell->try_no++;

      MSG_GERAN_MED_3_G( "SCH rescheduled later, lotn=%d hitn=%d ARFCN=%d",GL1_DEFS_TN_0, highest_rel_rxtx_ts, cell->bs.arfcn.num );

      MSG_GERAN_MED_3_G( " tone_detected %u bsic_known %u qbit_lag %d ", cell->bs.tone_detected, cell->bs.bsic_known, cell->bs.qbit_lag );

      cell->next_tick = l1_sc_globals_ptr->surround_tick + FOUR_SECONDS;
    }
    else
    {
      MSG_GERAN_MED_3_G("MARK SCH BAD, lotn=%d hitn=%d ARFCN=%d", GL1_DEFS_TN_0, highest_rel_rxtx_ts, cell->bs.arfcn.num );

      MSG_GERAN_MED_3_G( " tone_detected %u bsic_known %u qbit_lag %d ", cell->bs.tone_detected, cell->bs.bsic_known, cell->bs.qbit_lag );

      gpl1_gprs_sc_bad_sb_trans( cell, gas_id );
    }

  } /* End of else acquisition QB offset clashed with current timebase
     * configuration.
     */
}

/* This function updates the neighbour cell fn lag and qbit lag
 * based on the information provided by the FB acquisition
 * attempt.
 */
static void gpl1_gprs_fcb_done_trans( cell_T* cell, gas_id_t gas_id )
{

  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  dword     start_fn;
  int16     timing_error;

  ASSERT_TASK();


  if ( l1_sc_read_fcb(&start_fn, &timing_error, gas_id ) )
  {
    /* Instantiate and initialise the lowest and highest timeslot
     * information structures.
     */
    gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, /* Relative timeslot */
                                            FALSE          /* transmit boolean  */
                                          };
    gl1_defs_tn_struct  highest_tn_info = { GL1_DEFS_TN_0, /* Relative timeslot */
                                            FALSE          /* transmit boolean  */
                                          };
    if ( !cell->bs.tone_detected )
    {
      cell->bs.tone_detected = TRUE;

      //Reset try number for SCH decodes
      cell->try_no = 0;

    }

    /*
    FN lag is the frame number difference between the serving and surrounding cells.
    Cannot determine it unless the SCH is decoded.  Set to 0 for now
    */

    cell->bs.fn_lag = 0;
    cell->bs.qbit_lag     = timing_error;

    /*
    Store the last occurence of the tone to predict when to expect
    the SCH relative to the tone
    */
    cell->last_tone_fn = start_fn;

    cell->idle_frames_to_next_sb = 1;

    /*--------------------------------------------------------
       Check if the qbit is at the end of the acquisition
       window, then force the SCh to be decoded at the
       beginning of the acquisition to prevent the SCH
       from clashing with a Rx at the beginning of the frame.
       Since the Acq window is relative to the lowest Rx tn
       account for this in the calculation
    ----------------------------------------------------------*/

    /* Set the highest and lowest tn info */
    lowest_tn_info.tn  = (gl1_defs_tn_type) gprs_serv_cell_meas_ptr->lowest_tn.tn;
    highest_tn_info.tn = (gl1_defs_tn_type) gprs_serv_cell_meas_ptr->highest_tn;

    if ( gl1_msg_ncell_sch_pos_in_acq_window (
                                             cell->bs.qbit_lag,
                                             &( lowest_tn_info ),
                                             &( highest_tn_info ),
                                             gas_id
                                             ) == 1
       )
    {
      cell->next_sb_expected = (2 * MULTIFRAME_52);
      cell->bs.qbit_lag     -= QS_PER_FRAME;
    } else
    {
      cell->next_sb_expected = MULTIFRAME_52;
    }
    cell->first_ncell_sch_attempt = TRUE;
    cell->bs.fn_when_read = gl1_get_FN(gas_id);
    cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
  }

  
#ifdef FEATURE_GPRS_GBTA
  ignore_xcch = FALSE;
#endif /* FEATURE_GPRS_GBTA */

}

/* This function updates the neighbour cell fn lag and qbit lag
 * based on the information provided by the SB acquisition
 * attempt.
 */
static void  gpl1_gprs_sb_done_trans( cell_T*  cell, gas_id_t gas_id )
{
  byte               sb_data[4];
  dword              start_fn;
  int16              timing_error;
  l1_sc_rcv_result_T result;

  ASSERT_TASK();

  result = gpl1_gprs_sc_read_sb_trans( sb_data, &start_fn, &timing_error, gas_id );

  if ( result == L1_SC_DECODE_SUCCESS )
  {
    cell->bs.fn_when_read = gl1_get_FN(gas_id);
    cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
    gpl1_gprs_sc_good_sb_trans( cell, sb_data, start_fn, timing_error, gas_id );

    MSG_GERAN_LOW_3_G("SCH err=%d qbitlag=%d fnlag=%d",timing_error,cell->bs.qbit_lag,cell->bs.fn_lag);
  } else
  {
    gpl1_gprs_sc_bad_sb_trans( cell, gas_id );
  }


}

/* This function updates the neighbour cell fn lag and qbit lag
 * based on the information provided by the BCCH read
 * attempt.
 */
static void  gpl1_gprs_bcch_done_trans( cell_T*  cell, gas_id_t gas_id )
{
  uint8  *bcch_data;
  dBx16_T rx_power;
  int16   timing_error;
  uint8   tc;
  uint16  temp_bcch_mask;
  uint8 num_bursts = 0;
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  gpl1_g2x_ta_priority_t pri;
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif

  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr =
                    gl1_ms_switch_meas_mode_data(gas_id);


  ASSERT_TASK();
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  (void)gpl1_g2x_ta_is_activty_running(&pri,gas_id);
   /* First check for CCCH read */
  if( pri == G2X_TA_PRI_SERV_PCH )
  {
    byte                      page_mode = NORMAL_PAGING;
    boolean pch_read_result = l1_sc_read_bcch( &bcch_data, &rx_power, &timing_error, &tc, &num_bursts, gas_id );
    if (pch_read_result)
    {
      L1_send_PH_DATA_IND( TRUE, CCCH, TRUE, cell->bs.arfcn, bcch_data, (uint16)MAX_CCCH_OCTETS, gas_id );
      L1_send_MPH_BLOCK_QUALITY_IND(CCCH, GL1_BLOCK_QUALITY_GOOD, gas_id);
      MSG_GERAN_HIGH_3_G("PCH decoded (Arfcn=%d  fn=%d) rxp %d",
                          cell->bs.arfcn.num,gl1_get_FN( gas_id ),rx_power);

      if ( page_mode_pchxfer[gas_id] == EXTENDED_PAGING )
      {
        page_mode_pchxfer[gas_id] = NORMAL_PAGING;
        MSG_GERAN_HIGH_0_G("return to normal page mode");
      }
      else
      {
        page_mode = gpl1_read_page_mode(bcch_data,gas_id);
      }

      if ( page_mode == EXTENDED_PAGING )
      {
        uint32   new_bcch_fn;
        uint32   gap_to_next_activity = 0;
        uint8 ccch_tn;
        sword qbit_decrease;
        gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
        grm_client_enum_t g_client = data_ptr ->table[G2X_TA_PRI_SERV_PCH].client_id;
        idle_data_T   *idle_data_ptr;
        boolean b_ccch_in_xfer_as_gbta = FALSE;
        idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

        MSG_GERAN_HIGH_0_G("Page mode EXTENDED_PAGING during PCH Read in Transfer. recompute ext_page.");

        gap_to_next_activity  = calculate_gap_to_next_page(l1_tskisr_blk->current_params.L1Data.pIdle_data,EXTENDED_PAGING,gas_id);
        new_bcch_fn = ADD_FN(gl1_get_FN(gas_id),gap_to_next_activity);
        ccch_tn = (uint8)(idle_data_ptr->paging_data.ccch_group << 1);
        qbit_decrease = (sword)((ccch_tn - l1_get_current_timeslot(gas_id)) * 625);

        if (qbit_decrease < 0)
        {
           qbit_decrease += QS_PER_FRAME;
        }
        if( ccch_tn >= l1_get_current_timeslot(gas_id))
        {
          new_bcch_fn = ADD_FN(new_bcch_fn,1);
        }

        gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag = (int16)qbit_decrease;
        MSG_GERAN_MED_4_G("SERVING ARFCN bs.fn_lag bs.qbit_lag ccch_tn %d %d %d %d ",
                           gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn.num,
                           gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag,
                           gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag,
                           ccch_tn);

        trans[gas_id].new_bcch_fn      = new_bcch_fn;

        /*check if we are deregitering our own sequence for safety*/
        if(&gpl1_g2x_ta_seq_serv_ccch == gpl1_g2x_ta_seq[gas_id])
        {
          gpl1_g2x_ta_deregister_seq(gas_id);
        }

        gpl1_start_sc_tick_trans(gas_id);
        b_ccch_in_xfer_as_gbta =  gl1_check_ccch_in_transfer_permitted(gas_id);

        if(b_ccch_in_xfer_as_gbta) // make sure still in transfer
        {
          page_mode_pchxfer[gas_id] = EXTENDED_PAGING;

          (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_SERV_PCH, g_client, new_bcch_fn ,L1_SC_DURATION_BCCH, gas_id);
          data_ptr->non_trm_act.nbcch_fn = L1_GPRS_INVALID_FN;
          gpl1_g2x_ta_scheduler(gas_id);
#ifdef FEATURE_DUAL_SIM
          if ( gl1_msg_get_multi_sim_standby_mode() &&
               !gl1_drdsds_enabled(OTHER_GAS_ID(gas_id)))
          {
            gl1_trans_grm_reserve_at_self_ccch( SUB_FN(new_bcch_fn, 1), gas_id );
          }
#endif /* FEATURE_DUAL_SIM*/
        }
        else
        {
          MSG_GERAN_HIGH_0_G("PCHXFER### Missed PCH -EXT_PAGE:TA not allowed");
        }
      }
      else // NORMAL_PAGING
      {
        /*check if we are deregitering our own sequence for safety*/
        if(&gpl1_g2x_ta_seq_serv_ccch == gpl1_g2x_ta_seq[gas_id])
        {
          gpl1_g2x_ta_deregister_seq(gas_id);
        }
        gpl1_start_sc_tick_trans(gas_id);
      }
    }
    else
    {
      /* return to normal page mode on xcch failure */
      if ( page_mode_pchxfer[gas_id] == EXTENDED_PAGING )
      {
        page_mode_pchxfer[gas_id] = NORMAL_PAGING;
      }

      L1_send_PH_DATA_IND(FALSE,
                          CCCH,
                          TRUE,
                          NULL_ARFCN_VAL,
                          NULL,
                          0, gas_id);
      if(num_bursts > 1)
      {
        L1_send_MPH_BLOCK_QUALITY_IND(CCCH, GL1_BLOCK_QUALITY_BAD, gas_id);
        MSG_GERAN_HIGH_3_G("xCCH failed  (Arfcn=%d  fn=%d) rxp %d",
                            cell->bs.arfcn.num,gl1_get_FN( gas_id ),rx_power);
      }
      else
      {
        L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
        MSG_GERAN_HIGH_3_G("CCCH(0)  (Arfcn=%d  fn=%d) rxp %d",
                            cell->bs.arfcn.num,gl1_get_FN( gas_id ),rx_power);
      }
      /*check if we are deregitering our own sequence for safety*/
      if(&gpl1_g2x_ta_seq_serv_ccch == gpl1_g2x_ta_seq[gas_id])
      {
        gpl1_g2x_ta_deregister_seq(gas_id);
      }
      gpl1_start_sc_tick_trans(gas_id);
    }
  }
  else if ( pri == G2X_TA_PRI_SERV_BCCH )
#else
  if (cell->sb_count_to_bcch == BCCH_DO_NOW)
#endif
  {

    if ( l1_sc_read_bcch( &bcch_data, &rx_power, &timing_error, &tc, &num_bursts,gas_id ) )
    {
      //Replace L1_send_PH_DATA_IND with L1_send_sysinfo_PH_DATA_IND
      //so that TC value sent to RR and SI sharing works.
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if (cell->sb_count_to_bcch == BCCH_DO_NOW)
      {
         L1_send_sysinfo_PH_DATA_IND( TRUE, BCCH, FALSE, cell->bs.arfcn, tc, bcch_data, 23, gas_id );
      }
#else
      L1_send_sysinfo_PH_DATA_IND( TRUE, BCCH, FALSE, cell->bs.arfcn, tc, bcch_data, 23, gas_id );
#endif

      /* For serving cell bcch read , let RR clear the mask by sending MPH_SYS_INFO REQ
      ** with si mask = 0 . This is to avoid cases when BCCH read is successful but RR
      ** encounters a deoode error - allowing L1 to continuq acquiring the sys indo
      ** avoiding abnormal TBF release in case of SI 13.
      */
      if( gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans == FALSE )
      {
        /* if L1 is allowed to, clear the SIs for this TC value out of the mask */
        cell->bcch_mask &= ~(bcch_mask_clear[ tc ] & cell->bcch_clear_mask);
      }

      if ( cell->bcch_mask == 0 )
      {
        cell->sb_count_to_bcch = BCCH_DONT_DO;
        gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans = FALSE;
      }
    } else
    {
      /* See if this BCCH block would have cleared the mask.
       * If the mask would have been cleared then stop the receive
       * and send a failure indication.
       */
      temp_bcch_mask = cell->bcch_mask & ~(bcch_mask_clear[ tc ] & cell->bcch_clear_mask);

      if (temp_bcch_mask == 0)
      {
        if (gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans == FALSE)
        {
          /* Assume not autonomous BCCH */
          cell->sb_count_to_bcch = BCCH_DONT_DO;

          L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, FALSE, gas_id );
        } else
        {
          L1_send_MPH_READ_SYS_INFO_FAILURE_IND(FALSE, TRUE, gas_id);


          cell->sb_count_to_bcch = BCCH_DONT_DO;
          gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans = FALSE;

        }
      }
    }


#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  if ( data_ptr ->table[G2X_TA_PRI_SERV_BCCH].async_ccch_type == ASYNC_ACTIVITY_GBTA)
  {
    /*check if we are deregitering our own sequence for safety*/
    if(&gpl1_g2x_ta_seq_serv_ccch == gpl1_g2x_ta_seq[gas_id])
    {
      gpl1_g2x_ta_deregister_seq(gas_id);
    }
  }
#endif
    // Should start_sce
    gpl1_start_sc_tick_trans(gas_id);
  }

  gpl1_gprs_meas_mode_ptr->allowed_to_go = FALSE;

}

/*
 *
 */
static void  gpl1_sc_adjust_lag_trans( l1_sc_bs_T*  bs,
                                       sword        timing_error, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* Instantiate and initialise the lowest and highest timeslot
   * information structures.
   */
  gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, /* Relative timeslot */
                                          FALSE          /* transmit boolean  */
                                        };
  gl1_defs_tn_struct  highest_tn_info = { GL1_DEFS_TN_0, /* Relative timeslot */
                                          FALSE          /* transmit boolean  */
                                        };

  int16 qb = bs->qbit_lag + timing_error;

  gl1_defs_tn_type lowest_tn;
  gl1_defs_tn_type highest_tn;


  highest_tn =(gl1_defs_tn_type)gprs_serv_cell_meas_ptr->highest_tn;
  lowest_tn =  (gl1_defs_tn_type)gprs_serv_cell_meas_ptr->lowest_tn.tn;



  /* Calculate the highest possible occupied timeslot */
  /* +3 because e want over-the-air time */
  if (gprs_serv_cell_meas_ptr->highest_tn - (uint8)lowest_tn  == 3)
  {
    highest_tn = (gl1_defs_tn_type)(gprs_serv_cell_meas_ptr->highest_tn  + 2);
  } else
  {
    highest_tn = (gl1_defs_tn_type)(gprs_serv_cell_meas_ptr->highest_tn + 3);
  }

  /* Set the lowest and highest timeslot info */
  lowest_tn_info.tn  = lowest_tn;
  highest_tn_info.tn = highest_tn;

  if ( gl1_msg_ncell_sch_pos_in_acq_window( qb, &(lowest_tn_info), &(highest_tn_info), gas_id ) == 1 )
  {

    qb -= QS_PER_FRAME;

    /*
     * Since qbit offset of NCELL is adjusted by 1 frame (5000 qbits)
     * make corresponding change to fn lag
     */
    bs->fn_lag = ADD_FN( bs->fn_lag, 1 );

  } else if ( gl1_msg_ncell_sch_pos_in_acq_window( qb, &(lowest_tn_info), &(highest_tn_info), gas_id ) == -1 ) /* Rx and Tx TS */
  {

    qb += QS_PER_FRAME;
    bs->fn_lag = SUB_FN( bs->fn_lag, 1 );

  }

  bs->qbit_lag = qb;

}

/*========================= External function Prototypes =====================*/

/*===========================================================================

FUNCTION  gpl1_gprs_sc_enter_trans

DESCRIPTION

  enter gprs fixed mode gprs data call measurements

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_sc_enter_trans( gas_id_t gas_id )
{
  uint8 avg_len;

  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  ASSERT_TASK();

  trans[gas_id].report_period_timer          = 1; /* make timer expire on next tick */
  l1_sc_globals_ptr->max_bsic_age         = 216*20; /* ca 20 secs */
  l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;
  gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans = FALSE;
  gpl1_gprs_meas_mode_ptr->leave_trans_adjust_sync = FALSE;
  gpl1_gprs_meas_mode_ptr->enter_trans_adjust_sync =TRUE;

   id_tick[gas_id] = 0;
   trans[gas_id].wcdma_cell_id_sceduled = FALSE;
   trans[gas_id].wcdma_cell_id_go             = FALSE;
   trans[gas_id].tick_wcdma = FALSE;

  /* Preserve power measurements for cells in old BA list.
   * Use the info. we have at the moment to guess the
   * running average length.
   */
  avg_len = gpl1_gprs_calc_running_avg_length_trans( l1_sc_balist_get_num_cells(gas_id) );
  l1_sc_balist_set_ra_length( avg_len, gas_id );
  l1_sc_balist_transfer_power_info(gas_id);
//   l1_sc_monscan_set_req_passes( avg_len );

  /* Update monscan since BA list may have changed. */
  l1_sc_monscan_update(gas_id);

  /* Reset BSIC info */
  l1_sc_balist_reset_counters(gas_id);

  l1_sc_wcdma_enter_xfer(gas_id);
}

/*===========================================================================

FUNCTION  gpl1_gprs_sc_leave_trans

DESCRIPTION

    leave gprs fixed mode gprs data call measurements

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_sc_leave_trans( gas_id_t gas_id )
{
  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  ASSERT_TASK();
  gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
  /* Stop any ncell BCCH receives in progress. */
  if (!gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans)
  {
    gpl1_gprs_update_bcch_stop(TRUE, gas_id);
  }
  else if (gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans)
  {
    L1_send_MPH_READ_SYS_INFO_FAILURE_IND(TRUE, FALSE, gas_id);
  }

  if (l1_sc_globals_ptr->monscan_in_progress)
  {
    l1_sc_monscan_abort_gprs(gas_id);
  }

  if (l1_sc_globals_ptr->receive_in_progress)
  {
    if (l1_sc_irat_state_machine_active(gas_id) == FALSE)
    {
      l1_sc_abort_receive(gas_id);
    }
  }

  if (l1_sc_globals_ptr->wcdma_srch_in_progress)
  {
    l1_sc_wcdma_abort_xfer_search(gas_id);
  }

  // This can only happen when where TBf is just suspended for w id search( and g2w cfg is done) and at the same time MAC sent release request
  if ((g2x_ta_seq_trm_state[gas_id] == G2X_TA_DISABLE_RXTX_SUSPENDED) && (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT))
  {
    l1_sc_set_gtow_srch_state_null(gas_id);
  }


  l1_sc_wcdma_leave_xfer(gas_id);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  gpl1_g2x_ta_pchxfer_activity_cleanup(gas_id);

#ifdef FEATURE_DUAL_SIM
  SET_PCHXFER_DR_STATUS(GRM_PCHXFER_DR_NONE);
#endif
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

  /*leaving transfer so we should deregister the seq*/
  gpl1_g2x_ta_deregister_seq(gas_id);

  (void)gpl1_g2x_ta_suspend_all_table(gas_id);

  l1_sc_globals_ptr->send_gprs_trans_tick = FALSE;

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
  gpl1_gprs_reset_tuneaway_and_ncell_data(gas_id);
#endif /* FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

  /*if FN lag adjustment is not yet done, we do not need to compensate it while leave trans*/
  /*we set the enter_trans_adjust_sync to FALSE if adjustment happened in transfer state*/
  if (gpl1_gprs_meas_mode_ptr->enter_trans_adjust_sync == TRUE)
  {
    gpl1_gprs_meas_mode_ptr->leave_trans_adjust_sync = FALSE;
  }
  else
  {
    gpl1_gprs_meas_mode_ptr->leave_trans_adjust_sync = TRUE;
  }

  gpl1_gprs_meas_mode_ptr->enter_trans_adjust_sync = FALSE;
#ifdef FEATURE_G2L_TUNEAWAY
  gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN = GPL1_INVALID_FN;
#endif /* FEATURE_G2L_TUNEAWAY */
}

/*===========================================================================

FUNCTION  gpl1_gprs_sc_good_sb_trans

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_sc_good_sb_trans( cell_T*  cell,
                                  byte     sb_data[4],
                                  dword    start_fn,
                                  sword    timing_error, gas_id_t gas_id )
{
  dword bs_fn;
  byte  bsic;

  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  bs_fn = l1_sc_decode_sb( sb_data, &bsic );

  if ( !cell->bs.bsic_known )
  {
    cell->bs.bsic_known = TRUE;
    cell->bs.bsic = bsic;

    /* TAP POINT FOR NCELL_ACQ_SUCCESS */
    l1_log_ncell_acq( L1_LOG_NCELL_ACQ_SUCCESS,
                      cell->rank,
                      cell->bs.arfcn,
                      cell->bs.rx_power, gas_id );

  } else if ( cell->bs.bsic != bsic )
  {
    cell->bs.bsic = bsic;

    if (cell->sb_count_to_bcch == BCCH_DO_NOW)
    {
      L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, FALSE, gas_id);
      cell->sb_count_to_bcch = BCCH_DONT_DO;
      MSG_GERAN_MED_1_G("Ncell %d BSIC changed. SU cancelled.", cell->bs.arfcn.num);
    } else
    {
      MSG_GERAN_MED_1_G("Ncell %d BSIC changed.", cell->bs.arfcn.num);
    }
  }

  /*
  Once decoded don't decode for another 10 secs
  */
  cell->next_tick = l1_sc_globals_ptr->surround_tick + TEN_SECONDS;


  cell->bs.sb_data[ 0 ] = sb_data[ 0 ];
  cell->bs.sb_data[ 1 ] = sb_data[ 1 ];
  cell->bs.sb_data[ 2 ] = sb_data[ 2 ];
  cell->bs.sb_data[ 3 ] = sb_data[ 3 ];

  cell->bs.fn_lag = SUB_FN( start_fn, bs_fn );

  cell->last_bsic_fn = start_fn;
  cell->try_no = 0;
  cell->in_fade = FALSE;

  if ( cell->bs.fn_lag )
  {
    MSG_GERAN_MED_2_G("FN changed but not qbit, ARFCN=%d, FN_lag=%d", cell->bs.arfcn.num, cell->bs.fn_lag);
  }
  /*
     Set this to prevent the same cell from being accessed again till the
     next rank period
  */
  cell->bs.prev_rx_power = cell->bs.rx_power;

  gpl1_sc_adjust_lag_trans( &cell->bs, timing_error, gas_id );

   MSG_GERAN_LOW_3_G("SB GOOD Trans ARFCN=%d qbit_lag=%d fn_lag=%d",cell->bs.arfcn.num,cell->bs.qbit_lag,cell->bs.fn_lag);
}


/*===========================================================================

FUNCTION  gpl1_gprs_sc_bad_sb_trans

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_sc_bad_sb_trans( cell_T*  cell, gas_id_t gas_id )
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  ++cell->try_no;

  if ( cell->try_no >= MAX_TRIES )
  {

    if ( cell->bs.bsic_known )
    {
      cell->fn_when_lost = -32 * MAX_TRANS_TONE_DETECT_ATTEMPTS * MULTIFRAME_52;

      /* TAP POINT FOR NCELL_RECONFIRMATION_LOST */
      l1_log_ncell_acq( L1_LOG_NCELL_ACQ_RECONF_LOST,
                        cell->rank,
                        cell->bs.arfcn,
                        cell->bs.rx_power, gas_id );

      /* If there is a pending BCCH read then
       * tell GRR we're abandoning it.
       */
      if (cell->sb_count_to_bcch == BCCH_DO_NOW)
      {
        L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, FALSE, gas_id);
        cell->sb_count_to_bcch = BCCH_DONT_DO;
        MSG_GERAN_MED_1_G("Abandoning SU on %d.", cell->bs.arfcn.num);
      }
    } else
    {

      /* Save fn when cell was lost.  Used in scheduling */
      cell->fn_when_lost = (int32)l1_sc_get_fn(gas_id);

      /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason SCH) */
      l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_SCH,
                        cell->rank,
                        cell->bs.arfcn,
                        cell->bs.rx_power, gas_id );

    }

    cell->bs.bsic_known = FALSE;
    cell->bs.tone_detected = FALSE;
    cell->bs.bsic = 0;

    cell->try_no = 0;
    cell->in_fade = FALSE;

    /* TODO delete power measurements cause cell is lost */

  }

  else
  {
    /* If SCH fails, decode at next available opportunity*/
    cell->next_tick = l1_sc_globals_ptr->surround_tick;
    cell->in_fade = TRUE;
  }

  MSG_GERAN_LOW_3_G("SB BAD Trans ARFCN=%d qbit_lag=%d fn_lag=%d",cell->bs.arfcn.num,cell->bs.qbit_lag,cell->bs.fn_lag);
}


/*===========================================================================

FUNCTION  gpl1_sc_set_ba_list_trans

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_sc_set_ba_list_trans( inter_task_BCCH_allocation_T*  ba_list,
                                uint8                          ncc_permitted, gas_id_t gas_id )
{
  uint8 avg_len;

  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  gpl1_g2x_ta_priority_t pri;
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
#endif

  ASSERT_TASK();

  if ( l1_sc_balist_prepare_new( ba_list, ncc_permitted, gas_id ) )
  {
    if ( l1_sc_globals_ptr->receive_in_progress 
        && (l1_sc_globals_ptr->receiving_cell != gpl1_gprs_serv_cell[gas_id].serve_cell)
        && !l1_sc_globals_ptr->receiving_cell->in_new_ba_list )
    {

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      gpl1_g2x_ta_is_activty_running( &pri,gas_id);

      if (pri == G2X_TA_PRI_SERV_BCCH)
      {
        MSG_GERAN_HIGH_2_G("PCHXFER Missed BCCH due due to change in BA list ..aborting c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
        gpl1_g2x_ta_deregister_seq(gas_id);
        l1_sc_abort_receive(gas_id);
        l1_sci_tick_receive(gas_id);
      }
      else
      {
        l1_sc_abort_receive(gas_id);
      }

#else
      l1_sc_abort_receive(gas_id);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

      /* no receive pending so we must enable the tick */
      l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;
    }
    l1_sc_monscan_update(gas_id);

    avg_len = gpl1_gprs_calc_running_avg_length_trans( l1_sc_balist_get_num_cells(gas_id));
    l1_sc_monscan_set_req_passes( avg_len, gas_id );
    l1_sc_balist_set_ra_length( avg_len, gas_id );
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_report_period_done_trans

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_report_period_done_trans( gas_id_t gas_id )
{
  ASSERT_TASK();

  l1_sci_inc_surround_tick(1, gas_id);
}

/*===========================================================================

FUNCTION  gpl1_gprs_sc_trans_tick

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_sc_trans_tick( gas_id_t gas_id )
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  //ASSERT_TASK();

  /* if tick arrives just after disabling it, throw it away */
  if ( l1_sc_globals_ptr->send_gprs_trans_tick )
  {
    l1_sc_globals_ptr->send_gprs_trans_tick = FALSE;
    gpl1_gprs_next_cell_trans(gas_id);
  }
}

/*===========================================================================

FUNCTION  gpl1_pkt_xfer_set_highest_rx_timeslot

DESCRIPTION

This function allows the packet transfer tasking routines to
inform the neighbour cell scheduling entities of the highest receive
timeslot which is used on the block boundary before the IDLE frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_pkt_xfer_set_highest_rx_timeslot( uint8  highest_rx_timeslot, gas_id_t gas_id )
{
  l1_highest_rx_timeslot[gas_id] = (uint32) highest_rx_timeslot;
}

/*===========================================================================

FUNCTION  gpl1_pkt_xfer_set_highest_tx_timeslot

DESCRIPTION

This function allows the packet transfer tasking routines to
inform the neighbour cell scheduling entities of the highest transmit
timeslot which is used on the block boundary before the IDLE frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_pkt_xfer_set_highest_tx_timeslot( uint8  highest_tx_timeslot, gas_id_t gas_id )
{
  l1_highest_tx_timeslot[gas_id] = (uint32) highest_tx_timeslot;
}

/*===========================================================================

FUNCTION  gpl1_pkt_xfer_get_highest_timeslots

DESCRIPTION

This function allows the packet transfer tasking routines to
inform the neighbour cell scheduling entities of the highest transmit
timeslot which is used on the block boundary before the IDLE frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_pkt_xfer_get_highest_timeslots( uint32*  highest_rx_timeslot_ptr,
                                           uint32*  highest_tx_timeslot_ptr,
                                           gas_id_t gas_id )
{
  *highest_rx_timeslot_ptr = l1_highest_rx_timeslot[gas_id];

  *highest_tx_timeslot_ptr = l1_highest_tx_timeslot[gas_id];
}

/*===========================================================================

FUNCTION  gpl1_gprs_predict_next_sch

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_predict_next_sch( gas_id_t gas_id )
{
  word    n;
  cell_T* cell;

  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  /*
   * During the TBF suspension for NCELL BCCH reading
   * predict when the next SCH might be received
   * in case the SCH is not successfully decoded.
   * This is required because gpl1_gprs_next_cell_trans()
   * is not called during this period.
   */

  if ( l1_sc_globals_ptr->send_gprs_trans_tick )
  {
    l1_sc_globals_ptr->send_gprs_trans_tick = FALSE;

    for ( n = 0; (n < l1_sc_globals_ptr->sorted_cell_count) && (n < 16) ; ++n )
    {
      cell = l1_sc_globals_ptr->sorted_cells[ n ];

      if (cell->bs.tone_detected)
      {
        uint32  sc_fn = l1_sc_get_fn(gas_id);

        if ( SUB_FN( cell->last_tone_fn + cell->next_sb_expected, sc_fn ) < MULTIFRAME_52 )
        {
          if ( cell->idle_frames_to_next_sb == 9 )
          {
            cell->next_sb_expected += 2*MULTIFRAME_26;
            cell->idle_frames_to_next_sb = 11;
          }
          else
          {
             cell->next_sb_expected += 9*MULTIFRAME_26;
             if (cell->idle_frames_to_next_sb == 11)
             {
               cell->idle_frames_to_next_sb = 20;
             }
             else
             {
               cell->idle_frames_to_next_sb = 9;
             }
          }
        }
      }
    }
    l1_sc_globals_ptr->send_gprs_trans_tick = TRUE;
  }

}


/*===========================================================================

FUNCTION  gpl1_gprs_sc_bcch_sched_cb_trans

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  TRUE : Successful BCCH scheduled

SIDE EFFECTS
  None

===========================================================================*/
boolean  gpl1_gprs_sc_bcch_sched_cb_trans( gas_id_t gas_id )
{
  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
  boolean allowed_to_go = FALSE;
  gl1_msg_chan_type chan_type = GL1_MSG_BCCH;
  async_ccch_type_t  async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;
  cell_T* ccch_cell = trans[gas_id].bcch_cell;

  gpl1_g2x_ta_priority_t pri;
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
#endif

  (void)gpl1_g2x_ta_is_activty_running( &pri,gas_id);

#ifndef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  allowed_to_go = gpl1_gprs_meas_mode_ptr->allowed_to_go;
#else
  if ( data_ptr ->table[pri].async_ccch_type == ASYNC_ACTIVITY_GBTA)
  {
    allowed_to_go = TRUE;
    chan_type  = (pri == G2X_TA_PRI_SERV_PCH) ? GL1_MSG_CCCH: GL1_MSG_BCCH;
    if (pri == G2X_TA_PRI_SERV_PCH)
    {
      ccch_cell = gpl1_gprs_serv_cell[gas_id].serve_cell;
    }
    async_ccch_type = ASYNC_ACTIVITY_GBTA;
  }
  else
  {
    allowed_to_go = gpl1_gprs_meas_mode_ptr->allowed_to_go;
    async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;
  }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
  if (allowed_to_go)
  {
    MSG_GERAN_MED_1_G("Call gpl1_gprs_sc_receive_bcch_trans chan_type %d",chan_type);
    allowed_to_go = gpl1_gprs_sc_receive_bcch_trans( ccch_cell,
                                                     gpl1_gprs_bcch_done_trans,
                                                     chan_type,
                                                     async_ccch_type,
                                                     gas_id );
  }
  else
  {
    MSG_GERAN_HIGH_0_G("gpl1_gprs_sc_bcch_sched_cb_trans allowed to go is FALSE");
  }

  return allowed_to_go;
}

/*===========================================================================

FUNCTION  L1_process_READ_SYS_INFO_REQ_TRANS

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  L1_process_READ_SYS_INFO_REQ_TRANS( mph_read_sys_info_req_T *message_ptr )
{
  uint16 bcch_mask, ext_BCCH_mask;
  boolean decode;

  //gas_id_t gas_id = check_gas_id(message_ptr->gas_id);getting compile error.
  gas_id_t gas_id = message_ptr->gas_id;
  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  gprs_pl1_serving_cell_meas_T  *gpl1_sc = gprs_serv_cell_meas_ptr;

  bcch_mask_ext[gas_id] = FALSE;
  gpl1_gprs_serv_cell[gas_id].serve_cell                = l1_sc_balist_get_serving_cell_ptr(gas_id);
  gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn      = gl1_get_serving_cell_arfcn(gas_id);
  gpl1_gprs_serv_cell[gas_id].serve_cell->bs.rx_power   = gpl1_sc->rla_p_isr_avg;
  gpl1_gprs_serv_cell[gas_id].serve_cell->bs.bsic       = l1_idle_data->campedon_cell_BSIC;
  gpl1_gprs_serv_cell[gas_id].serve_cell->bs.bsic_known = TRUE;

  decode = FALSE;
  ASSERT_TASK();

  if ((message_ptr->si_mask == RR_L1_SI_TYPE_NONE) || (message_ptr->si_mask == RR_L1_SI_TYPE_1) || (message_ptr->si_mask == RR_L1_SI_TYPE_2)
      || (message_ptr->si_mask == RR_L1_SI_TYPE_2_bis) || (message_ptr->si_mask == RR_L1_SI_TYPE_2_ter)
      || (message_ptr->si_mask == RR_L1_SI_TYPE_2_quater_NORM) || (message_ptr->si_mask == RR_L1_SI_TYPE_3)
      || (message_ptr->si_mask == RR_L1_SI_TYPE_4) || (message_ptr->si_mask == RR_L1_SI_TYPE_9)
      || (message_ptr->si_mask == RR_L1_SI_TYPE_13_NORM) || (message_ptr->si_mask == RR_L1_SI_TYPE_13_alt_NORM)
      || (message_ptr->si_mask == RR_L1_SI_TYPE_15_NORM))
  {


    if (message_ptr->replace_prev_mask)
    {
      l1_idle_data->si_mask = message_ptr->si_mask;
    } else
    {
      l1_idle_data->si_mask |= message_ptr->si_mask; /*lint !e655 bit-wise operation uses enums */
    }

    bcch_mask             = (uint16) gl1_compute_BCCH_mask( l1_idle_data->si_mask );
    gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask      = bcch_mask;
    decode = TRUE;
    bcch_mask_ext[gas_id] = FALSE;
  } else if ((message_ptr->si_mask == RR_L1_SI_TYPE_2_quater_EXT)
             || (message_ptr->si_mask == RR_L1_SI_TYPE_7) || (message_ptr->si_mask == RR_L1_SI_TYPE_8)
             || (message_ptr->si_mask == RR_L1_SI_TYPE_13_EXT) || (message_ptr->si_mask == RR_L1_SI_TYPE_13_alt_EXT)
             || (message_ptr->si_mask == RR_L1_SI_TYPE_15_EXT) || (message_ptr->si_mask == RR_L1_SI_TYPE_16)
             || (message_ptr->si_mask == RR_L1_SI_TYPE_17) )
  {


    if (message_ptr->replace_prev_mask)
    {
      l1_idle_data->si_mask = message_ptr->si_mask;
    } else
    {
      l1_idle_data->si_mask |= message_ptr->si_mask;/*lint !e655 bit-wise operation uses enums */
    }


    ext_BCCH_mask         = (uint16) gl1_compute_ext_BCCH_mask( l1_idle_data->si_mask );
    gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask       = ext_BCCH_mask;
    decode = TRUE;
    bcch_mask_ext[gas_id] = TRUE;
  }

  if (decode == TRUE)
  {

    gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_clear_mask = gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask;



    if (gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask != 0)
    {
      /* get BCCh asap */
      gpl1_gprs_serv_cell[gas_id].serve_cell->sb_count_to_bcch = BCCH_DO_NOW;
      gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans = TRUE;
    } else
    {
      gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask            = SERV_BCCH_MASK_NONE;
      if (gprs_serv_cell_meas_ptr->suspended_tbf == FALSE)
      {
        /* If TBF not already suspended then stop any scheduled BCCH */
        gpl1_gprs_serv_cell[gas_id].serve_cell->sb_count_to_bcch     = BCCH_DONT_DO;

		/*Protecting g2x_ta activity status with ISR lock during cancelling of Planned activity because at some places it is also called from task context while at the same time ISR might be writing this structure.*/
        GL1_ISR_LOCK(gas_id);
		
        if( gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_BCCH,gas_id) &&
            ARFCNS_EQUAL(gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn , trans[gas_id].bcch_cell->bs.arfcn))
        {
          MSG_GERAN_MED_2_G("Cancel planned SBCCH activity ARFCN %d SI mask 0x%x",
                             gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn.num, gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask);
          gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_SERV_BCCH,gas_id);

          /* BCCH update is stopped, so GL1 should schedule the next action */
          gpl1_g2x_ta_scheduler(gas_id);
        }

		GL1_ISR_UNLOCK(gas_id);
        
        if( gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans != FALSE)
        {
          /* prevent a tbf suspension for an already
           * scheduled serving cell sys info read
           */
          gpl1_gprs_meas_mode_ptr->allowed_to_go = FALSE;
          gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans = FALSE;
          gpl1_reset_stop_sc_tick_trans(gas_id);
        }
      }
      else
      {
        /* reset si mask as told by RR after successfull deoode */
        gpl1_gprs_serv_cell[gas_id].serve_cell->sb_count_to_bcch       = BCCH_DONT_DO;
        gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans = FALSE;
      }
    }
  } else
  {
    L1_send_MPH_READ_SYS_INFO_FAILURE_IND(FALSE, TRUE, gas_id);
    gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask            = SERV_BCCH_MASK_NONE;
    gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_clear_mask      = SERV_BCCH_MASK_NONE;
    gpl1_gprs_serv_cell[gas_id].serve_cell->sb_count_to_bcch     = BCCH_DONT_DO;
    gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans = FALSE;
    MSG_GERAN_MED_0_G("MULTIPLE_MASK");
  }


  MSG_GERAN_MED_2_G("MPH_READ_SYS_INFO_REQ 0x%x, %d",
          message_ptr->si_mask,
          message_ptr->replace_prev_mask);
}

/*===========================================================================

FUNCTION gpl1_gprs_sc_trans_post_sort_process

DESCRIPTION
  Should be called each time the BA list is sorted in Transfer Mode.  This
  function looks for cells entering or leaving top six and takes
  appropriate action. E.g. marks BSIC as unkniown for cells re-entrering the
  top six after a long time out of it.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  gpl1_gprs_sc_trans_post_sort_process( gas_id_t gas_id )
{
  uint32  i;
  cell_T *cell;

  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  /* Loop through sorted cells */
  for (i=0; i<l1_sc_globals_ptr->sorted_cell_count; i++)
  {
    cell = l1_sc_globals_ptr->sorted_cells[i];

    GL1_ISR_LOCK(gas_id);

    /* Did it just enter top six or just
     * get strong enough to see?
     */
    if ( ( (cell->rank < 6) && (cell->prev_rank >= 6) ) ||
         ( (cell->bs.prev_rx_power < SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16) &&
           (cell->bs.rx_power >= SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 ) ) )
    {
      /* Have we already decoded the BSIC on this cell? */
      if (cell->bs.bsic_known)
      {
        /* Check how long cell has been out of top 6. */
        if (cell->next_tick < l1_sc_globals_ptr->surround_tick)
        {
          /* Impose the 10 seconds check here
           * before erasing the BSIC information.
           */
          if( (l1_sc_globals_ptr->surround_tick - cell->next_tick) >= TEN_SECONDS )
          {
            /* Cell has been out of best 6 for a long time -
             * mark for new acquisition.
             */
            cell->sb_count_to_bcch = BCCH_DONT_DO;
            cell->bs.bsic_known    = FALSE;

            /* Print an F3 message */
            MSG_GERAN_MED_3_G(" bsic erased arfcn %u tick %u global tick %u",
                    cell->bs.arfcn.num,
                    cell->next_tick,
                    l1_sc_globals_ptr->surround_tick );
          }
        }
      }
    }

    GL1_ISR_UNLOCK(gas_id);
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_change_timeslot

DESCRIPTION
  This function controls the change of timeslot

DEPENDENCIES
  new timeslot number

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_change_timeslot ( byte new_ts,  gas_id_t gas_id )
{
    int16  sync_info_ncell = 0;

    if( new_ts < l1_get_current_timeslot(gas_id) )
    {
        /* Locals */
        sword  qbit_decrease = (sword) ((new_ts - l1_get_current_timeslot(gas_id)) * 625);

        gl1_msg_update_g2l_slot_change( (int32)qbit_decrease, gas_id );
        if( qbit_decrease < 0 )
        {
            qbit_decrease = QS_PER_FRAME + qbit_decrease;
        }
        l1_sc_ts_changed(gas_id);

        MSG_GERAN_HIGH_2_G(" MS camped on ts %u FNmod13 %u ", new_ts, frame_counters[gas_id].FNmod13);

        gl1_msg_correct_timing( (int32)qbit_decrease,gas_id );

        /* The above line of code should shorten the next frame
        but instead lengthens it by adding 8. thus we have
        lost a frame and must compensate by incrementing the
        frame counters an extra time */
        GSTMR_INC_FN_BY_ONE(gas_id);
        sync_info_ncell = (int16) (( new_ts - l1_get_current_timeslot(gas_id) ) * 625);

        l1_set_current_timeslot(new_ts, gas_id);   /* update current TS */
    }
    else if( new_ts > l1_get_current_timeslot(gas_id) )
    {
        /* Locals */
        sword  qbit_decrease = (sword)((new_ts - l1_get_current_timeslot(gas_id)) * 625);
        gl1_msg_update_g2l_slot_change( (int32)qbit_decrease, gas_id );

        if( qbit_decrease < 0 )
        {
            qbit_decrease = QS_PER_FRAME + qbit_decrease;
        }
        l1_sc_ts_changed(gas_id);

        MSG_GERAN_HIGH_2_G(" MS camped on ts %u FNmod13 %u ", new_ts, frame_counters[gas_id].FNmod13);

        gl1_msg_correct_timing( (int32)qbit_decrease,gas_id );

        sync_info_ncell = (int16) (( new_ts - l1_get_current_timeslot(gas_id) ) * 625);

        l1_set_current_timeslot(new_ts, gas_id);   /* update current TS  */
    }

    /*
       Even if timeslot hasn't changes, check if Ncell offset is
       within bounds or if it needs to be wrapped around.  Especially
       for a SDCCH to TCH transition since the Ncell acq windows are
       different
     */
    l1_scdrv_call_in_task( l1_sc_balist_gprs_set_sync_in_task,
                           (void *)((int32)sync_info_ncell), gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
}

#ifdef FEATURE_GPRS_PS_HANDOVER

/*===========================================================================

FUNCTION  gpl1_gprs_sync_to_target

DESCRIPTION
  This function controls the change of synchronisation to the target cell
  for PSHO. It is based on the dedicated function l1_sync_to_target()

DEPENDENCIES
  new lowest downlink timeslot number, qbit offset and frame number offset

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_sync_to_target (byte new_ts,
                                l1_sc_bs_T *current_cell,
                                l1_sc_bs_T *target_cell,
                                l1_sc_bs_T *previous_cell,
                                gas_id_t gas_id )
{
    if (target_cell->bsic_known)
    {
      int16  ncell_qbit_lag;
      int16  total_qbit_lag;
      sword  ts_qbit_lag;

      /*save old timeslot number*/
      current_cell->TN = l1_get_current_timeslot(gas_id);
      /*
       * Add 1 to frame number lag because it is currently based
       * on when the sch aquisition was commanded and not executed
       * i.e it is a frame out.
       */
      target_cell->fn_lag = ADD_FN( target_cell->fn_lag, 1 );
      if (target_cell->qbit_lag < 0 )
      {
        ncell_qbit_lag = QS_PER_FRAME + target_cell->qbit_lag;
        /*
         * Increasing qbit lag by 5000 therefore decrease fn lag
         * by 1 frame
         */
        target_cell->fn_lag = SUB_FN( target_cell->fn_lag, 1 );
        /*
         * Change FN to that of target cell using fn lag
         * Store offset and fn lag to current cell to revert
         * back to original cell if handover fails
         */
        current_cell->fn_lag   = L1_change_FN( target_cell->fn_lag, gas_id );
        current_cell->qbit_lag = - target_cell->qbit_lag;

        /* Translate Ncell information w.r.t new cell */
        l1_sc_set_sync( ADD_FN(target_cell->fn_lag,1),
                        target_cell->qbit_lag, gas_id );

        MSG_GERAN_HIGH_3_G("target_cell->qbit_lag < 0, %d update ncell offsets, new ts %d curr ts %d ",target_cell->qbit_lag,new_ts,l1_get_current_timeslot(gas_id));

        /* Use previous cell info, so that we don't have to reacquire
         * if its in the new cell's BA list
         */
        l1_sc_ba_cell_set_sync( previous_cell,
                                ADD_FN(target_cell->fn_lag,1),
                                target_cell->qbit_lag, gas_id );
      }
      else
      {
        ncell_qbit_lag = target_cell->qbit_lag;

        /*
         * Change FN to that of target cell using fn lag
         * Store offset and fn lag to current cell to revert
         * back to original cell if handover fails
         */
        current_cell->fn_lag = L1_change_FN( target_cell->fn_lag , gas_id);
        current_cell->qbit_lag = QS_PER_FRAME - target_cell->qbit_lag;

        /* Translate Ncell information w.r.t new cell */
        l1_sc_set_sync( target_cell->fn_lag,
                        target_cell->qbit_lag, gas_id );

        MSG_GERAN_HIGH_3_G("target_cell->qbit_lag >= 0, %d update ncell offsets, new ts %d curr ts %d ",target_cell->qbit_lag,new_ts,l1_get_current_timeslot(gas_id));

        /* Transfer serving cell info separately */
        l1_sc_ba_cell_set_sync( previous_cell,
                                target_cell->fn_lag,
                                target_cell->qbit_lag, gas_id );
      }

      MSG_GERAN_HIGH_1_G("Sync Tgt Old TS = 0 New TS=%d",new_ts);

      /*
       * The surround cell offset is based on the surround cell's
       * tone and SCH which are in timeslot 0.  So we've synced up
       * to timeslot 0 after this we slam to ncell_qbit_lag.
       * So we have to compute ts_qbit_lag relative to TS 0
       */
      ts_qbit_lag = (sword)(new_ts * 625);
      /* Adjust for this timeslot difference */
      l1_sc_set_sync(0, (int16) ts_qbit_lag, gas_id);
      /* Transfer serving cell info separately */
      l1_sc_ba_cell_set_sync( previous_cell,0,ts_qbit_lag, gas_id );
      total_qbit_lag = ncell_qbit_lag + ts_qbit_lag;

      MSG_GERAN_HIGH_3_G("Slamming by %d + %d = %d",ncell_qbit_lag,ts_qbit_lag,total_qbit_lag);
      MSG_GERAN_HIGH_3_G("PSHO, return params qbit %d  fn %d tn %d ",current_cell->qbit_lag,current_cell->fn_lag,current_cell->TN);
      /* Adjust for both offsets together instead of in two stages earlier
       *  ncell_qbit_lag syncs up to timeslot 0 of new cell and ts_qbit_lag
       * then syncs up to new timeslot on new cell
       */
      if ( total_qbit_lag )
      {
        gl1_msg_correct_timing( (int32)total_qbit_lag ,gas_id);
        GSTMR_REWIND_FN ( 1, gas_id );

      }
      /* @@OG: Set the current timeslot */
      l1_set_current_timeslot(new_ts, gas_id);
    }
    else
    {
       MSG_GERAN_ERROR_0_G("Target cell details unavailable");
    }
}

#endif
/*===========================================================================

FUNCTION  l1_sc_balist_gprs_set_sync_in_task( )

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_balist_gprs_set_sync_in_task( void *sync_info_ncell, gas_id_t gas_id )
{
  int16 ncell_offset;

  ncell_offset = (int16) (uint32) (void *) sync_info_ncell;

  l1_sc_set_sync(0, ncell_offset, gas_id);
}


/*===========================================================================

FUNCTION  wcdma_done_trans( )

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  wcdma_done_trans( void* unused, gas_id_t gas_id )
{
   gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
   gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
   /* called in task context */
   NOTUSED( unused );
   // Set allowed_to_go to false only if table field is not updated with BCCH FN
   if( data_ptr->non_trm_act.nbcch_fn == NOT_A_FRAME_NUMBER )
   {
      gpl1_gprs_meas_mode_ptr->allowed_to_go = FALSE;
   }

   MSG_GERAN_MED_1_G("wcdma_done_trans, allowed_to_go %d ",gpl1_gprs_meas_mode_ptr->allowed_to_go);

   trans[gas_id].tick_wcdma = FALSE;
   gpl1_gprs_restart_ncell_sch(gas_id);
}


/*===========================================================================

FUNCTION  gpl1_transfer_service_pending_sysinfo_reqs

DESCRIPTION
  This function checks for any pending sysinfo requests on the
  serving cell and sends failure inds to upper layers when any
  are found.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_transfer_service_pending_sysinfo_reqs( gas_id_t gas_id )
{
  if( NULL != gpl1_gprs_serv_cell[gas_id].serve_cell )
  {
    /* Check for any Serving cell BCCH read request
     * which have not been serviced yet.
     */
    if( SERV_BCCH_MASK_NONE != gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask )
    {
      /* Send a sys info request failure indication */
      L1_send_MPH_READ_SYS_INFO_FAILURE_IND( TRUE,      /* state change */
                                             FALSE, gas_id );

    }
  }
  else
  {
    MSG_GERAN_ERROR_0_G(" NULL serve_cell ptr ");
  }
}

/******************************************************************************
 *
 *  Function name:  calculate_gap_to_next_page
 *  -----------------------------------------------
 *
 *  Description:
 *  ------------
 * Calculates the number of frames between the current frame and the frame
 * the next CCCH receive starts in (frame where message layer driver called)
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
 *
 *
 *  Returns:
 *  --------
 *  Gap to next activity as described above.  0 if no gap.
 *
 *****************************************************************************/
#define BLOCK_ALIGN_OFFSET 2 // do not change.

uint32 calculate_gap_to_next_page( idle_data_T  *idle_data_ptr, byte   paging_mode,gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

   const  uint8 paging_frames[9] = {6,12,16,22,26,32,36,42,46};
   const  uint8 ext_paging_frame[9] = {16,22,26,32,36,42,46,6,12};

   uint32 gap_to_next_activity = 0;
   uint32 current_frame_pos;
   uint32 paging_frame_pos;
   uint32 local_FN;
   uint32 paging_period;
   uint8  ext_pch_blk_index=0;
   uint32 local_FNmod51 = frame_counters[gas_id].FNmod51;

   gprs_pl1_serving_cell_meas_T  *gpl1_sc = gprs_serv_cell_meas_ptr;
   if ( idle_data_ptr != NULL)
   {
      MSG_GERAN_MED_4_G("PCHXFER calculate_gap_to_next_page pg MF %d  pgi %d SDCCH_COMB %d BS_AG_BLK_RES %d",
                                          idle_data_ptr->paging_data.paging_multiframe,
                                          idle_data_ptr->paging_data.paging_block_index,
                                          idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB,
                                          idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES);
      local_FN          = gl1_get_FN( gas_id );

      gpl1_gprs_serv_cell[gas_id].serve_cell                = l1_sc_balist_get_serving_cell_ptr(gas_id);
      gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn      = gl1_get_serving_cell_arfcn(gas_id);
      gpl1_gprs_serv_cell[gas_id].serve_cell->bs.rx_power   = gpl1_sc->rla_p_isr_avg;
      gpl1_gprs_serv_cell[gas_id].serve_cell->bs.bsic       = l1_idle_data->campedon_cell_BSIC;
      gpl1_gprs_serv_cell[gas_id].serve_cell->bs.bsic_known = TRUE;

      /*
       In case paging parameters are unknown i.e. if the SIM is invalid
       from the start, use default values
      */
      if ( idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 0 )
      {
        idle_data_ptr->campedon_cell_data.BS_PA_MFRMS = 2;
        MSG_GERAN_ERROR_0_G("BS_PA_MFRMS was 0.  Setting to 2.");
      }

      if (paging_mode == EXTENDED_PAGING )
      {
        if (!idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB)
        {
          if((idle_data_ptr->paging_data.paging_block_index >=7))
          {
            ext_pch_blk_index = idle_data_ptr->paging_data.paging_block_index + idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES;
            if(ext_pch_blk_index > 8)
            {
              ext_pch_blk_index-=9;
            }
            if(local_FNmod51 >= paging_frames[7])
            {
              gap_to_next_activity = (MULTIFRAME_51 - local_FNmod51) + ext_paging_frame[ext_pch_blk_index] - BLOCK_ALIGN_OFFSET;
            }
            else
            {
              gap_to_next_activity = ext_paging_frame[ext_pch_blk_index] - local_FNmod51 - BLOCK_ALIGN_OFFSET;
            }
          }
          else
          {
            ext_pch_blk_index = idle_data_ptr->paging_data.paging_block_index ;
            gap_to_next_activity = ext_paging_frame[ext_pch_blk_index] - local_FNmod51 - BLOCK_ALIGN_OFFSET;
          }
        }
        else
        {
           switch ( idle_data_ptr->paging_data.paging_block_index )
           {
             case 0:
               ext_pch_blk_index =0;
               gap_to_next_activity = ext_paging_frame[ext_pch_blk_index] - local_FNmod51 - BLOCK_ALIGN_OFFSET;
               break;
             case 1:
               ext_pch_blk_index =( 7 + idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES) ;
               if(local_FNmod51 > 12)
               {
                 gap_to_next_activity = (MULTIFRAME_51 - local_FNmod51) + ext_paging_frame[ext_pch_blk_index] - BLOCK_ALIGN_OFFSET;
               }
               else
               {
                 gap_to_next_activity =  ext_paging_frame[ext_pch_blk_index] - BLOCK_ALIGN_OFFSET - local_FNmod51 ;
               }
               break;
             case 2:
               ext_pch_blk_index =(8 + idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES)% 9;
               if(local_FNmod51 > 16)
               {
                 gap_to_next_activity = (MULTIFRAME_51 - local_FNmod51) + ext_paging_frame[ext_pch_blk_index] - BLOCK_ALIGN_OFFSET;
               }
               else
               {
                 gap_to_next_activity =  ext_paging_frame[ext_pch_blk_index] - BLOCK_ALIGN_OFFSET - local_FNmod51 ;
               }
               if ( idle_data_ptr->campedon_cell_data.BS_AG_BLKS_RES == 2)
               {
                 gap_to_next_activity = gap_to_next_activity + MULTIFRAME_51;
               }
               break;
            }
        } // sdcch_combined
      } //extened_paging
      else
      {
      /* Start by working out the gap to the next paging block.
       * This calculation is performed modulo BS_PA_MFRMS*51,
       * i.e. modulo the paging period.
       */
          paging_period     = idle_data_ptr->campedon_cell_data.BS_PA_MFRMS*MULTIFRAME_51;
          current_frame_pos = local_FN % paging_period;
          paging_frame_pos  = (idle_data_ptr->paging_data.paging_multiframe * MULTIFRAME_51) +
                          paging_frames[idle_data_ptr->paging_data.paging_block_index];

          /* required because of definition of gap */
          current_frame_pos += BLOCK_ALIGN_OFFSET;

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
           * multiple of BS_PA_MFRMS then the modulo BS_PA_MFRMS*51 calculation
           * above will give the wrong gap.
           */
          if ( (local_FN + gap_to_next_activity) >= (uint32)(FRAMES_IN_HYPERFRAME) )
          {
            /* The next paging block is after the FN wrap around. Do
            * calculation again, taking this into account.
            */
            gap_to_next_activity = (FRAMES_IN_HYPERFRAME + paging_frame_pos) - local_FN - BLOCK_ALIGN_OFFSET;
          }
      }
      MSG_GERAN_HIGH_3_G("PCHXFER gap_to_next_activity %d page_mode %d BS_CCCH_SDCCH_COMB %d",
                         gap_to_next_activity,paging_mode,idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB);
      return gap_to_next_activity;
   }
   else
   {
      MSG_GERAN_ERROR_0_G("idle_data_ptr == NULL");
      return 0;
   }
}
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
/*===========================================================================

FUNCTION  gl1_check_ccch_in_transfer_permitted

DESCRIPTION
   Checks if PCH in Transfer is permitted by RR and we are in Packet Transfer only.

DEPENDENCIES
  Resource Lock shouldnt be active 

RETURN VALUE
  TRUE -  when PCH/CCCH  can be decoded using COEX PS scheduling bit
  FALSE - Not permitted.

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_check_ccch_in_transfer_permitted(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /*Dont Allow TA for CCCH in Xfer if Resource Lock is active */
#ifdef FEATURE_DUAL_SIM
  if ( grm_get_resource_lock_state (gas_id) )
  {
#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG
    MSG_GERAN_HIGH_0_G("PCHXFER: Read_ccch_in_transfer blocked by resource lock");
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG */
  }
  else 
#endif /* FEATURE_DUAL_SIM */
  if ( gpl1_check_if_ccch_in_transfer_supported(gas_id) &&
       (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) &&
       (!gpl1_gprs_leaving_transfer(gas_id))&& 
       (l1_tskisr_blk->command != L1_DEDICATED_COMMAND)
     )
  {
    return  TRUE;
  }

  return FALSE;
} /* gl1_check_ccch_in_transfer_permitted */

/*===========================================================================

FUNCTION  gpl1_check_if_ccch_in_transfer_supported

DESCRIPTION
   If PCH in transfer is enabled , the calls the pkt scheduler to calculate PCH _FN

DEPENDENCIES
  None

RETURN VALUE
None
SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_check_if_ccch_in_transfer_supported(gas_id_t gas_id)
{
  boolean supported = FALSE;
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];

#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG
  if ( ( (TRUE == l1_idle_data->layer1_extra_gprs_sys_info.read_pch_in_transfer) ||
        (GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_FORCE_ENABLE_CCCH_IN_XFER) ) )
        &&
        /*by default disabled from the mainline builds currently, can be enabled through NV*/
        ( !GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_DISABLE_CCCH_IN_XFER))
        &&
        ( gl1_msg_get_multi_sim_sys_mode() != SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY)
     )
#else
  if ( ( (TRUE == l1_idle_data->layer1_extra_gprs_sys_info.read_pch_in_transfer )
        &&
        ( gl1_msg_get_multi_sim_sys_mode() != SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY)
     )
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG */
  {
    supported = TRUE;
  }
#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG
  
   MSG_GERAN_DEBUG_OPT_4_G("PCHXFER: Read_ccch_in_transfer_support %d      EFS enabled %d Msim mode %d Force enable %d", supported,
                        !GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_DISABLE_CCCH_IN_XFER),
                        gl1_msg_get_multi_sim_sys_mode(),
                        GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_FORCE_ENABLE_CCCH_IN_XFER));
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG */

  return supported;
}

/*===========================================================================

FUNCTION  gpl1_check_and_clear_ccch_xfer_resv

DESCRIPTION
   Check if CCCH in transfer activity needs to be released/reserved 
   Do a trm release if rel_or_resv = TRUE else do a TRM ResvAt for PCH_XFER
   PCHXFER design had a limitation that whenever PCH activity was scheduled after G2X TA gap and 
   within 40ms of closing the gap, it did not run as TRM did not return the TRM lock back to 
   Transfer sub in this case.
   Hence we need to do a TRM Release just before opening gap (to clear trm resv) and do 
   a reserve at again when gap is closed

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_check_rel_resv_ccch_xfer(boolean rel_or_resv, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T   *idle_data_ptr;
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  uint32 pch_resv_fn = 0;
  
  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

  if(gl1_msg_get_multi_sim_standby_mode()&&
     (!gl1_drdsds_enabled(OTHER_GAS_ID(gas_id))) &&
      gpl1_check_if_ccch_in_transfer_supported(gas_id) &&
      gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_PCH,gas_id))
  {
    if(rel_or_resv == TRUE)
    {
      grm_release_pchxfer( data_ptr ->table[G2X_TA_PRI_SERV_PCH].client_id, gas_id );
    }
    else
    {
      pch_resv_fn = SUB_FN(data_ptr ->table[G2X_TA_PRI_SERV_PCH].frame_number, 1);
        
      if((data_ptr ->table[G2X_TA_PRI_SERV_PCH].frame_number != L1_GPRS_INVALID_FN) && //extra safety check
          IS_FRAME_NUM_LATER(pch_resv_fn, gl1_get_FN(gas_id)))
      {
        gl1_trans_grm_reserve_at_self_ccch( pch_resv_fn, gas_id );
      }
    }
  }
      
}

/*===========================================================================

FUNCTION  gpl1_schedule_pch_in_transfer

DESCRIPTION
   computes and schedules PCH in Transfer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_schedule_pch_in_transfer(gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    idle_data_T   *idle_data_ptr;
    gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
    uint32 gap_to_next_activity = 0;
    uint8 ccch_tn;
    sword qbit_decrease;
    boolean b_ccch_in_xfer_as_gbta = FALSE;    

    gpl1_g2x_ta_act_status_t act_status = gpl1_g2x_ta_get_act_status(G2X_TA_PRI_SERV_PCH,gas_id);

    gpl1_g2x_ta_act_status_t unlock_act_status = gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TRM_UNLOCK,gas_id);

    if( gl1_check_ccch_in_transfer_permitted(gas_id) &&
      !((act_status == NEXT_ACT) || (act_status == WAITING) ||(act_status == ONGOING)))
    {
      /* Here removed the condition for scheduling PCHXFER only if gap to PCH is within 15 to 52 frames. */

      uint32 new_bcch_fn;
      /* check whether client exchange happened for PCH in transfer, this check is added  */
      /* as client exchange may not have happened in PTM init due to resource lock          */
      /* Do not exchange clients in case DR - DSDS                                                       */
      if((l1_tskisr_blk->client_id == GRM_GSM1) || (l1_tskisr_blk->client_id == GRM_GSM2))
      {
        MSG_GERAN_MED_2_G("PCHXFER: Client exchange didn't happen at PTM init, client: %d, drdsds: %d", l1_tskisr_blk->client_id
                                                                                                      , gl1_drdsds_enabled(OTHER_GAS_ID(gas_id)));
        if( gl1_msg_get_multi_sim_standby_mode() &&
            (!gl1_drdsds_enabled(OTHER_GAS_ID(gas_id))))
        {
          gpl1_ccch_xfer_exchange_trm_clients(gas_id);
        }
      }

      idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
      gap_to_next_activity = calculate_gap_to_next_page( idle_data_ptr,NORMAL_PAGING,gas_id );

	  
      if(gap_to_next_activity > MIN_GAP_FOR_SCHEDULING_PCH_IN_TRANSFER)
      {
        new_bcch_fn = ADD_FN(gl1_get_FN(gas_id),gap_to_next_activity);

        /* Work out what qbit we need to decode CCCH from slammed TN in PTM
             ** Also need to factor in CCCH GROUP TN
        */

        ccch_tn = (uint8)(idle_data_ptr->paging_data.ccch_group << 1);
        qbit_decrease = (sword)((ccch_tn - l1_get_current_timeslot(gas_id)) * 625);

        if (qbit_decrease < 0)
        {
          qbit_decrease += QS_PER_FRAME;
       }
       if( ccch_tn >= l1_get_current_timeslot(gas_id))
       {
         new_bcch_fn = ADD_FN(new_bcch_fn,1);
       }
       gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag = (int16)qbit_decrease;
       gpl1_gprs_meas_mode[gas_id].allowed_to_go = FALSE; /*  will prevent disable_rx_tx susp interferance */

       MSG_GERAN_MED_4_G("SERVING ARFCN bs.fn_lag bs.qbit_lag %d %d %d ccch tn %d",
                          gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn.num,
                          gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag,
                          gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag,
                          ccch_tn);

       trans[gas_id].new_bcch_fn      = new_bcch_fn;  // obselete now.

       page_mode_pchxfer[gas_id] = NORMAL_PAGING;

       (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_SERV_PCH,
                                             gl1_map_gas_id_to_client_id_pchxfer(gas_id), 
                                             new_bcch_fn ,
                                             L1_SC_DURATION_BCCH,
                                             gas_id);
       data_ptr->non_trm_act.nbcch_fn = L1_GPRS_INVALID_FN;
       gpl1_g2x_ta_scheduler(gas_id);

#ifdef FEATURE_DUAL_SIM
       if ( gl1_msg_get_multi_sim_standby_mode() && 
            !gl1_drdsds_enabled(OTHER_GAS_ID(gas_id)) && 
            (unlock_act_status != ONGOING))
       {
         gl1_trans_grm_reserve_at_self_ccch( SUB_FN(new_bcch_fn, 1), gas_id );
       }
#endif /* FEATURE_DUAL_SIM */
      }
    }
} /* gpl1_schedule_pch_in_transfer */

/*===========================================================================

FUNCTION  gl1_disable_ccch_in_transfer

DESCRIPTION
   gl1_disable_ccch_in_transfer when transition to DTM.

DEPENDENCIES
  None

RETURN VALUE
None
SIDE EFFECTS
  None

===========================================================================*/
void gl1_disable_ccch_in_transfer(gas_id_t gas_id)
{
   gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];

   /*if not ogoing then remove entry */
   if (gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_BCCH,gas_id))
   {
     gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_SERV_BCCH,gas_id);
     if(TRUE == gpl1_get_stop_sc_tick_trans(gas_id))
     {
       gpl1_start_sc_tick_trans(gas_id);
     }
   }
   else if(ONGOING == data_ptr ->table[G2X_TA_PRI_SERV_BCCH].status)
   {
     MSG_GERAN_HIGH_2_G("PCHXFER Missed BCCH due to disabling c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number);
     gpl1_g2x_ta_deregister_seq(gas_id);
     l1_sc_abort_receive(gas_id);
     l1_sci_tick_receive(gas_id);
     gpl1_start_sc_tick_trans(gas_id);
   }

   if (gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_PCH,gas_id))
   {
     gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_SERV_PCH,gas_id);
     if(TRUE == gpl1_get_stop_sc_tick_trans(gas_id))
     {
       gpl1_start_sc_tick_trans(gas_id);
     }
   }
   else if(ONGOING == data_ptr ->table[G2X_TA_PRI_SERV_PCH].status)
   {
     MSG_GERAN_HIGH_2_G("PCHXFER Missed PCH due to disabling c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[G2X_TA_PRI_SERV_PCH].frame_number);
     L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);

     gpl1_g2x_ta_deregister_seq(gas_id);
     l1_sc_abort_receive(gas_id);
     l1_sci_tick_receive(gas_id);
     gpl1_start_sc_tick_trans(gas_id);
   }
}

/*===========================================================================

FUNCTION  gpl1_gprs_compute_ccch_event_qbit_lag

DESCRIPTION
   Recomputes qbit_lag and adjust activity FN. when lowest TN changes

DEPENDENCIES
  None

RETURN VALUE
None
SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_compute_ccch_event_qbit_lag(gas_id_t gas_id)
{
  sword qbit_decrease;
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T *idle_data_ptr;
  uint32 gap_to_next_activity = 0;
  uint8 ccch_tn;
  uint32 new_bcch_fn;

  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  ccch_tn = (uint8)(idle_data_ptr->paging_data.ccch_group << 1);

  if (gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_PCH,gas_id))
  {
     gap_to_next_activity = calculate_gap_to_next_page( idle_data_ptr,page_mode_pchxfer[gas_id],gas_id );

     new_bcch_fn = ADD_FN(gl1_get_FN(gas_id), gap_to_next_activity);

     /* Work out what qbit we need to decode CCCH from slammed TN in PTM
       ** Also need to factor in CCCH GROUP TN
       */
     qbit_decrease = (sword)((ccch_tn - l1_get_current_timeslot(gas_id)) * 625);

     if (qbit_decrease < 0)
     {
       qbit_decrease += QS_PER_FRAME;
     }

     if( ccch_tn >= l1_get_current_timeslot(gas_id))
     {
       new_bcch_fn = ADD_FN(new_bcch_fn,1);
     }
     gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag = (int16)qbit_decrease;
     gpl1_gprs_meas_mode[gas_id].allowed_to_go = FALSE; /*  will prevent disable_rx_tx susp interferance */

     MSG_GERAN_MED_5_G("SERVING ARFCN bs.fn_lag bs.qbit_lag %d %d %d ccch tn %d page mode %d",
                        gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn.num,
                        gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag,
                        gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag,
                        ccch_tn,
                        page_mode_pchxfer[gas_id]);

     trans[gas_id].new_bcch_fn = new_bcch_fn;  // obselete now.

     if(data_ptr ->table[G2X_TA_PRI_SERV_PCH].frame_number != new_bcch_fn)
     {
       (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_SERV_PCH,
                                              gl1_map_gas_id_to_client_id_pchxfer(gas_id),
                                              new_bcch_fn ,
                                              L1_SC_DURATION_BCCH,
                                              gas_id);
       data_ptr->non_trm_act.nbcch_fn = L1_GPRS_INVALID_FN;
       gpl1_g2x_ta_scheduler(gas_id);

#ifdef FEATURE_DUAL_SIM
          if ( gl1_msg_get_multi_sim_standby_mode() &&
               !gl1_drdsds_enabled(OTHER_GAS_ID(gas_id)))
          {
            gl1_trans_grm_reserve_at_self_ccch( SUB_FN(new_bcch_fn, 1),gas_id );
          }
#endif
     }
  }

  if (gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_BCCH,gas_id))
  {
     uint32 new_bcch_fn;

     gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);

     if (gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans == FALSE)
     {
       new_bcch_fn = gpl1_gprs_get_bcch_fn(trans[gas_id].bcch_cell->bs.fn_lag, trans[gas_id].bcch_cell->bs.qbit_lag, trans[gas_id].bcch_cell->bcch_mask, gas_id);
       MSG_GERAN_MED_3_G("PCHXFER4 : re-calculate the FN NCELL (lowest_tn_change) before %d after %d qbit_lag %d",
                           data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number,
                           new_bcch_fn,trans[gas_id].bcch_cell->bs.qbit_lag);
     }
     else // scell bcch
     {
       qbit_decrease = (sword)((ccch_tn - l1_get_current_timeslot(gas_id)) * 625);

       if (qbit_decrease < 0)
       {
          qbit_decrease += QS_PER_FRAME;
       }
       if( ccch_tn >= l1_get_current_timeslot(gas_id))
       {
         gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag     = (uint32) -1;
       }
       else
       {
         gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag     = (uint32) -2;
       }

       gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag   = (int16)qbit_decrease;

       new_bcch_fn = gpl1_gprs_get_bcch_fn(gpl1_gprs_serv_cell[gas_id].serve_cell->bs.fn_lag,
                                           gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag, gpl1_gprs_serv_cell[gas_id].serve_cell->bcch_mask,
                                           gas_id);

       if (bcch_mask_ext[gas_id] == TRUE)
       {
         new_bcch_fn = ADD_FN(new_bcch_fn,4);
       }

       MSG_GERAN_MED_3_G("PCHXFER4 : re-calculate the FN SCELL (lowest_tn_change) before %d after %d qbit_lag %d",
                           data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number,
                           new_bcch_fn,gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag);
     }

     if( data_ptr ->table[G2X_TA_PRI_SERV_BCCH].async_ccch_type == ASYNC_ACTIVITY_GBTA)
     {
       if(data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number != new_bcch_fn)
       {
         (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_SERV_BCCH, 
                                               gl1_map_gas_id_to_client_id_pchxfer(gas_id),
                                               new_bcch_fn ,NOT_A_FRAME_DURATION, gas_id);
         gpl1_g2x_ta_scheduler(gas_id);
       }
     }
     else
     {
       //Call update_table_field to block align the suspension FN, needed for GTA
       if(data_ptr->non_trm_act.nbcch_fn != new_bcch_fn)
       {
         data_ptr->non_trm_act.nbcch_fn = new_bcch_fn;
         (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_SERV_BCCH, 
                                               gl1_map_gas_id_to_client_id_pchxfer(gas_id),
                                               new_bcch_fn ,NOT_A_FRAME_DURATION, gas_id);
         gpl1_g2x_ta_scheduler(gas_id);
       }
       else
       {
         MSG_GERAN_MED_0_G("G2X_TA No change in bcch FN after TS adjustment");
       }
     }
  }
}

#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION  gpl1_ccch_xfer_exchange_trm_clients

DESCRIPTION
  This function exchanges the trm between source and destination clients

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_ccch_xfer_exchange_trm_clients(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t dest_client = GRM_GPRS1;
  grm_client_enum_t source_client = l1_tskisr_blk->client_id;

#ifdef FEATURE_GSM_RX_TX_SPLIT
  grm_client_enum_t          src_tx_client;
  grm_client_enum_t          dst_tx_client;
  trm_client_exchange_input_info input;
#endif /* FEATURE_GSM_RX_TX_SPLIT */

  switch(source_client)
  {
    case GRM_GSM1:
      dest_client = GRM_GPRS1;
      break;
    
    case GRM_GSM2:
      dest_client = GRM_GPRS2;
      break;
      
    case GRM_GPRS1:
      dest_client = GRM_GSM1;
      break;
  
    case GRM_GPRS2:
      dest_client = GRM_GSM2;
      break;
  
    default:
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_1_G("ERROR : Unexpected client Id %d", l1_tskisr_blk->client_id);
      break;
  }
  
  
  /* clear down the state of the receiving clients */
  grm_release_pchxfer(dest_client, gas_id);
  
#ifdef FEATURE_GSM_RX_TX_SPLIT
  /* look up the correct TX client for each RX client */
  src_tx_client = grm_map_rx_to_tx_client_id(source_client, gas_id);
  dst_tx_client = grm_map_rx_to_tx_client_id(dest_client, gas_id);

  grm_release_pchxfer(dst_tx_client, gas_id);

  MSG_GERAN_HIGH_4_G("PCHXFER : TRM exchange src %d src_tx %d dst %d dst tx %d", 
                      source_client, 
                      src_tx_client,
                      dest_client, 
                      dst_tx_client);
                      
  GL1_TRM_LOCK();

  /* and call the TRM interface to complete the exchange for the RX clients */
  input.client_1 = grm_map_grm_client_id_to_trm_client_id(dest_client);
  input.client_2 = grm_map_grm_client_id_to_trm_client_id(source_client);
  trm_client_exchange(&input);

  /* if TX clients have also been specified, exchange them as well */
  input.client_1 = grm_map_grm_client_id_to_trm_client_id(dst_tx_client);
  input.client_2 = grm_map_grm_client_id_to_trm_client_id(src_tx_client);
  trm_client_exchange(&input);

  GL1_TRM_UNLOCK();
  
  /* clear down the state of the source tx client */
  grm_release_pchxfer(src_tx_client, gas_id);
  
#else
  
  /* and then swap the states, so dest_client is holding a chain */
  grm_exchange_lock(dest_client, l1_tskisr_blk->client_id, gas_id);
  
#endif /* FEATURE_GSM_RX_TX_SPLIT */
  
  /* clear down the state of the source client */
  grm_release_pchxfer(source_client, gas_id);
  
  l1_tskisr_blk->client_id = dest_client;
}
/*===========================================================================

FUNCTION gl1_grm_is_set_pchxfer_dr_status

DESCRIPTION
   set PCHXFER DR state

PARAMS
  grm_pchxfer_dr_state_t

DEPENDENCIES

RETURN NONE

===========================================================================*/
void gl1_grm_set_pchxfer_dr_status(grm_pchxfer_dr_state_t dr_status)
{
  pchxfer_dr_state = dr_status;
}


/*===========================================================================

FUNCTION gl1_grm_get_pchxfer_dr_status

DESCRIPTION
   get PCHXFER DR state

PARAMS
  

DEPENDENCIES

RETURN 
  grm_pchxfer_dr_state_t


===========================================================================*/
grm_pchxfer_dr_state_t gl1_grm_get_pchxfer_dr_status(void)
{
  return pchxfer_dr_state;
}

#endif /* FEATURE_DUAL_SIM*/

/*===========================================================================

FUNCTION  gpl1_get_cell_information

DESCRIPTION
  This function obtains the Cell qbit lag and arfcn for coex.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_get_cell_information(int16 *qbit_lag,ARFCN_T *arfcn,gpl1_g2x_ta_priority_t pri,gas_id_t gas_id)
{
   if (pri == G2X_TA_PRI_SERV_PCH)
   {
     *qbit_lag  =  gpl1_gprs_serv_cell[gas_id].serve_cell->bs.qbit_lag;
     *arfcn     =  gpl1_gprs_serv_cell[gas_id].serve_cell->bs.arfcn;
   }
   else  //G2X_TA_PRI_SERV_BCCH
   {
     *qbit_lag  =  trans[gas_id].bcch_cell->bs.qbit_lag;
     *arfcn     =  trans[gas_id].bcch_cell->bs.arfcn;
   }
}
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

/*===========================================================================

FUNCTION  gpl1_gprs_calc_pkt_txfr_gap

DESCRIPTION
  This function calculates position of the lowest rx slot and
  highest rx/tx slot.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_calc_pkt_txfr_gap(uint8 *lowest_rx_tn, uint8 *highest_tx_tn, gas_id_t gas_id)
{

  /* The lowest receive is always the current timeslot which is the camp timeslot.
   * Use the highest occupied timeslot which is either the highest receive or
   * the highest transmit.
   */
  uint32  highest_rel_rxtx_ts = l1_get_current_timeslot(gas_id);

#if defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG)
  MSG_GERAN_HIGH_3_G("l1_get_current_timeslot %d l1_highest_tx_timeslot %d l1_highest_rx_timeslot %d",
           highest_rel_rxtx_ts, l1_highest_tx_timeslot[gas_id], l1_highest_rx_timeslot[gas_id]);
#endif /*defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG)*/

  if( 0xFF != l1_highest_tx_timeslot[gas_id] )
  {
    /* Find the receive equivalent */
    highest_rel_rxtx_ts = ADD_MOD( l1_highest_tx_timeslot[gas_id],
                                   3,
                                   GL1_DEFS_SLOTS_IN_FRAME );
  }
  else if( 0xFF != l1_highest_rx_timeslot[gas_id] )
  {
    highest_rel_rxtx_ts = l1_highest_rx_timeslot[gas_id];
  }


  /* Convert the receive equivalent timeslot of the highest transmit
   * to be relative to the camp timeslot i.e. lowest absolute receive.
   */
  highest_rel_rxtx_ts = SUB_MOD( highest_rel_rxtx_ts,
                                 l1_get_current_timeslot(gas_id),
                                 GL1_DEFS_SLOTS_IN_FRAME );

#if defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG)
  MSG_GERAN_HIGH_1_G("highest_rel_rxtx_ts %d", highest_rel_rxtx_ts);
#endif /*defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG)*/

  /* Set the highest tn info */
  *highest_tx_tn = (uint8) highest_rel_rxtx_ts;
  *lowest_rx_tn = 0;
  return;
}


/*===========================================================================

FUNCTION  gl1_is_geran_tuneaway_allowed

    DESCRIPTION
   Determines if GPRS Tuneaway is allowed, or is being prevented by
    - PDP Context activation signalling;
    - GMM Non-DRX mode for GPRS Attach;
    - Network Assisted Cell Change (NACC);
    - Enhanced Dual Transfer Mode (EDTM);
    - RLC Unacknowledged Mode traffic;
    - etc.

    DEPENDENCIES
    None

    RETURN VALUE
  TRUE if GPRS Tuneaway is allowed

    SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_is_geran_tuneaway_allowed( gas_id_t gas_id )
{
  sys_modem_as_id_e_type as_id  = geran_map_gas_id_to_nas_id( gas_id );

  /* check upper layer control switch (protects GPRS signalling operations) */
  if (geran_api_get_tuneaway_blocked(as_id))
  {
    /* API has indicated that the TBF cannot be pre-empted  - don't abort */
    MSG_GERAN_HIGH_1_G("GTA/GBTA is blocked by upper layers on ds%1d", 1+(int)as_id);
    return FALSE;
  }
  return TRUE;
} /*  gl1_is_geran_tuneaway_allowed */


#ifndef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS

/*===========================================================================

FUNCTION  gpl1_gprs_ds_init_pch_suspension

DESCRIPTION


DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_ds_init_pch_suspension(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t        client_id;
  trans[gas_id].new_bcch_fn = L1_GPRS_INVALID_FN;

#ifdef FEATURE_GPRS_GBTA
  trans[gas_id].gbta_is_tbf_pch_suspension = FALSE;
  trans[gas_id].gbta_current_pch_suspension_FN = L1_GPRS_INVALID_FN;
#endif /* FEATURE_GPRS_GBTA */

  client_id = l1_tskisr_blk->client_id;

  /* These variables are used to store the last winning client, reason for the last winning client etc.
   * during the GTA gap. These are initialized to invalid values here. These are updated at the start of the
   * gta_preemption_callback function. The purpose is, we don't want the unlock callback to be called again,
   * if the last winning client and the current winning client are calling this callback for the same reason.
   */

#ifdef FEATURE_G2X_TUNEAWAY
  gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client           = GRM_NO_CLIENT;
  gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event            = TRM_UNLOCK_CANCELLED;
  gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_sclk   = 1;
  gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason = TRM_NUM_REASONS;
#endif

  gpl1_g2x_ta_data_init(gas_id);


  /* Only initialise GTA if we are in DSDS mode */
  if( gl1_msg_get_multi_sim_standby_mode()
#ifdef FEATURE_SGLTE
     || (gl1_hw_get_sglte_mode(gas_id) && (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))
#endif
#ifdef FEATURE_SVLTE_DUAL_SIM
     || (gl1_check_svlte_mode_active() == TRUE)
#endif
    )
  {
    /* Grab the secondary chain also here so that WCDMA wont get the lock when G is in PTM*/


#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
      /* dont exchange trm client if DR-DSDS is enabled*/
      if(gl1_msg_get_multi_sim_standby_mode() &&
         gpl1_check_if_ccch_in_transfer_supported(gas_id))
      {
        if(!gl1_drdsds_enabled(OTHER_GAS_ID(gas_id)))
        {
          /*In SR-DSDS mode exchange client only if current client is non-GPRS client*/
          if(l1_tskisr_blk->client_id != GRM_GPRS1 && l1_tskisr_blk->client_id != GRM_GPRS2)
          {
            gpl1_ccch_xfer_exchange_trm_clients(gas_id);
          }
          else
          {
             MSG_GERAN_HIGH_1_G("No need to exchange TRM client as it is already GPRS client %d",l1_tskisr_blk->client_id);
          }
          client_id = l1_tskisr_blk->client_id;
          gl1_grm_set_pchxfer_dr_status(GRM_PCHXFER_DR_OFF);
        }
        else
        {
          gl1_grm_set_pchxfer_dr_status(GRM_PCHXFER_DR_ON);
        }
      }
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      grm_retain_lock( client_id,
                      (grm_unlock_callback_t)&grm_gta_preemption_callback,
                       gas_id );

      /* Need to regitster an abort callback with TRM. This allows any booking by any other client to
         invoke this callback and pass the time when the booking is required */
      MULTI_SIM_FW_LOAD_FRAMES = MULTI_SIM_FW_LOAD_FRAMES_GPAGE_GTA;
#endif

    MSG_GERAN_HIGH_0_G("MS G TA ds_init_pch_sus");

#ifdef FEATURE_SGLTE
    if( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
    {
      trm_get_info_return_type get_granted;

      if ( TRM_GRANTED == grm_get_granted( client_id, &get_granted, gas_id )
#ifdef FEATURE_DUAL_DATA
           && grm_get_data_sub_priority(gas_id) != GERAN_PRIORITY_INVERTED
#endif
         )
      {
        rfm_device_enum_type rf_device = gl1_hw_get_gsm_rf_id(client_id, gas_id );

        /* @@TODO IF/ELSE cases below are identical but retained until sglte+g is cleaned up */
        if (rf_device != RFM_DEVICE_0)
        {
          grm_change_priority( client_id, 
                               grm_get_connection_priority(GRM_PS_CONNECTION,gas_id),
                               gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id), 
                               gas_id );
        }
        else
        {
          grm_change_priority( client_id, 
                               grm_get_connection_priority(GRM_PS_CONNECTION,gas_id),
                               gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id), 
                               gas_id );
        }
      }
    }
    else
#endif
    {
      /*Entering into packet transfer mode, RxD is not supported,
        release divrx */
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
      if (gl1_get_MSRD_ctl_nv(gas_id) == FALSE)
      {
        grm_release_rx_sec(grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id);
      }
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
#endif /*FEATURE_DUAL_SIM*/
      grm_change_priority( client_id,
                           grm_get_connection_priority(GRM_PS_CONNECTION,gas_id),
                           gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id),
                           gas_id );
    }
  }
}

#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

#ifdef FEATURE_G2X_TUNEAWAY
/*===========================================================================

FUNCTION     g2x_ta_handle_abort_callback

DESCRIPTION  L1 task handler for GTA abort callback

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void g2x_ta_handle_abort_callback(grm_client_enum_t       client,
                                  trm_unlock_event_enum_t event,
                               uint32                  unlock_by_sclk,
                                  uint32                  winning_client_duration,
                                  trm_tuneaway_enum_type  ta_info,
                                  gas_id_t                gas_id)
{
  boolean do_schedule_tbf_susp;
  uint32  gap_to_start_in_frames; /*no. of frames till the activity start FN*/

  grm_info_t *grm_info_p = NULL;

  grm_client_enum_t 			  other_g_client;
  trm_reason_enum_t 			  other_g_reason;
  boolean						  l_wreconf = FALSE;
#ifdef FEATURE_GPRS_GBTA
  boolean						  gbta_success = FALSE;
#endif
  uint32   suspension_FN = L1_GPRS_INVALID_FN;

  uint32   duration = DEFAULT_DUR_TRM_UNLOCK; /*in frames*/
  uint32   msecs_duration = 0;
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];

#ifdef FEATURE_G2L_TUNEAWAY
  uint32   gframe_duration = 0;
#endif /* FEATURE_G2L_TUNEAWAY */

 uint32 pchxfer_duration = L1_GPRS_INVALID_FN;

  GL1_ISR_LOCK(gas_id);
  
  other_g_client = gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client;
  /* check whether the other G client is pending the lock */
  other_g_reason = gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason;

  if (other_g_client != GRM_NO_CLIENT)
  {
    grm_info_p = grm_get_info_ptr_from_client(other_g_client, gas_id);
    if (grm_info_p)
    {
      l_wreconf = grm_info_p->wreconf;
    }
  }

  MSG_GERAN_HIGH_2_G("G2X_TA: info_ptr->wreconf %d other_g_client %d",
                     l_wreconf,
                      gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client);

  /* check whether a suspension gap is already scheduled */
  if (l1_tsk_buffer[gas_id].current_params.L1Data.pTransfer_data == NULL)
  {
    GL1_ISR_UNLOCK(gas_id);
    MSG_GERAN_ERROR_0_G("G2X_TA: Transfer data ptr is NULL");
    return;
  }

  if(gpl1_gprs_leaving_transfer(gas_id))
  {
    MSG_GERAN_HIGH_2_G("G2X_TA: Leaving Transfer: Ignore Unlock Event %d other_client %d",
                      event, gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client);
    GL1_ISR_UNLOCK(gas_id);
    return;
  }

  //Only if the winning client is GSM1/2/3 need to check GBTA logic
  if (IS_CLIENT_G(other_g_client))
  {
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef  FEATURE_DUAL_SIM
    /* no need to anything  as gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client updated a winning client */
    if (gl1_map_client_id_to_gas_id(other_g_client) == gl1_map_client_id_to_gas_id(client))
    {
#ifdef FEATURE_GPRS_GBTA
      gbta_success = TRUE;
      trans[gas_id].gbta_current_pch_suspension_FN = L1_GPRS_INVALID_FN;
      trans[gas_id].gbta_is_tbf_pch_suspension = FALSE;
#endif /* FEATURE_GPRS_GBTA */
      MSG_GERAN_HIGH_4_G("PCHXFER GBTA Reservation for slave client %d other_g_client %d winning_client %d evnt %d",
                         client, other_g_client, gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client, event);

      /* invoke the scheduler again to see if our activity can become next act if not already*/
      if (NEXT_ACT != gpl1_g2x_ta_get_act_status(G2X_TA_PRI_SERV_PCH, gas_id))
      {
        gpl1_g2x_ta_scheduler(gas_id);
      }
    }
    else
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/
      /* Decide if GBTA is the course of action */
#ifdef FEATURE_GPRS_GBTA
      if ((gl1_gbta_get_nv_gbta_support(gas_id))
          && (other_g_client != GRM_NO_CLIENT)
          && (ta_info == TRM_QUICK_TUNEAWAY) //in G+G , QTA maps to GBTA and LTA maps to GTA
          && (FALSE == l_wreconf)
          )
      {
        gbta_success = gl1_gbta_handle_abort_callback(client, other_g_client, event, unlock_by_sclk, other_g_reason, gas_id);
      }
#endif /* FEATURE_GPRS_GBTA */
  }


  /*  GBTA not supported or not execute proceed with a normal suspension gap*/
  if (!gbta_success)
  {
    /* default outcome is NOT to open GTA gap */
    do_schedule_tbf_susp = FALSE;
    switch (event)
    {
      case TRM_UNLOCK_CANCELLED:
      {
        /* cancel any pending TBF suspension*/
        if (gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TRM_UNLOCK, gas_id) != ONGOING)
        {
          (void)gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_TRM_UNLOCK, gas_id);
        }
        GL1_ISR_UNLOCK(gas_id);
        MSG_GERAN_HIGH_1_G("G2X_TA: TRM_UNLOCK_CANCELLED Abort for Client %d", client);
        return;
      }

      case TRM_UNLOCK_IMMEDIATELY:
      {
      MSG_GERAN_HIGH_2_G("G2X_TA: TRM_UNLOCK_IMMEDIATELY client %d L1 send RR_DS_ABORT_TRANSFER fn %d", client,gl1_get_FN(gas_id));

        /* L1 is leaving packet transfer mode abnormally.
        * Empty the rrbp queue to ensure L1 drops into Idle.
        */
        gpl1_gprs_reset_rrbp_enteries(gas_id);

        /* if tbf release is pending then ensure tbf release is processed */
        if (gpl1_gprs_awaiting_release(gas_id))
        {
          volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

          transfer_data_ptr->wait_for_pca_valid = TRUE;
          transfer_data_ptr->wait_for_pca_fn = ADD_FN(gl1_get_FN(gas_id), 1);
        }

        /* tell GRR to trigger a PS abort and give the CS connection time to get the lock */
      L1_send_MPH_L1_DS_ABORT_IND(gas_id, RR_DS_ABORT_TRANSFER, GL1_ACCESS_CONFIRM_TIMEOUT);
      }
        break;

      case TRM_UNLOCK_BY:
      {
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
        if(IS_TWO_PHASE_ACCESS(gas_id) && (other_g_reason != TRM_DEMOD_PAGE))
        {
          MSG_GERAN_HIGH_1_G("G2X_TA_2P_ACCESS- TA not allowed for reason %d in Two phase Access", other_g_reason);
          do_schedule_tbf_susp = FALSE;
        }
        else
#endif /* FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
        {
          unlock_by_sclk -= timetick_get_safe();
          msecs_duration = timetick_cvt_from_sclk(unlock_by_sclk, T_MSEC);
          gap_to_start_in_frames = MSECS_TO_FRAMES(msecs_duration);
  
          /* Following check is added to avoid other sub X tech page misses. In a boundary case, where we 
           * have only 3 frames to suspend TBF, we are not able to release the lock with in Unlock by time. 
           * Due to this, Other X tech is not getting the lock when required,leading to page miss. 
           * In those scenarios, we are reducing the gap to start by 1 frame so that lock would be released
           * in time by G data sub and there won't be missing pages from other sub X tech.*/
      if ((MODULO_FRAME_SCALING_FACTOR(msecs_duration)) < SCALING_FACTOR_MS )
          {
            gap_to_start_in_frames = gap_to_start_in_frames - 1;
            MSG_GERAN_HIGH_1_G("G2X_TA: TRM_UNLOCK_BY gap_to_start_in_frames %d", gap_to_start_in_frames);
          }
          /* Dual GSTMR architecture needs additional overhead such that Trm request for PCH sub happens after
          * PTM sub prioirty is reduced to BG traffic
          */
          suspension_FN = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), gap_to_start_in_frames);
  
  #ifdef FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK
          duration = MSECS_TO_FRAMES(timetick_cvt_from_sclk(winning_client_duration, T_MSEC));
          duration += TBF_RESUME_GUARD_FRAMES;
  #endif /*FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK*/
  
        MSG_GERAN_HIGH_5_G("TRM_UNLOCK_BY suspension_FN %d	  Winning Client %d from TRM, gap to start(sclks):%d gap to start(msec):%d gap_duration(frames) =%d",
          suspension_FN, gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client , unlock_by_sclk,msecs_duration,duration);
          do_schedule_tbf_susp = TRUE;
  
          MSG_GERAN_HIGH_3_G("G2W_TA: TRM_UNLOCK_BY gap to start(frames) %d; current frame: %d; TBF susp: %d", gap_to_start_in_frames, gl1_get_FN(gas_id), do_schedule_tbf_susp);

  #ifdef FEATURE_G2L_TUNEAWAY
          /* Currently allowing only for LTE, in future it may required to all techs*/
          if ((gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE)
              || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SECONDARY)
              || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SUB2)
              || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SUB2_SECONDARY)
              )
          {
            if ((gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN) != GPL1_INVALID_FN)
            {
              /* gframe_duration gives the duration between two G2X gaps */
              gframe_duration = SUB_FN(suspension_FN, gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN);
              if ((gframe_duration < MIN_DURATION_BETWEEN_G2X_GAPS) &&
                  (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason != TRM_DEMOD_PAGE))
              {
                /* Do not Suspend the TBF, if unlock by reason is not TRM_DEMOD_PAGE in back to back G2L TA scenario */
                do_schedule_tbf_susp = FALSE;
                suspension_FN = L1_GPRS_INVALID_FN;
              }
              MSG_GERAN_HIGH_4_G("G2X_TA: TRM_UNLOCK_BY unlock_by_reason: %d; prev_g2x_end_FN: %d; gframe_duration: %d; do_schedule_tbf_susp: %d", gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason, gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN, gframe_duration, do_schedule_tbf_susp);
            }
          }
  #endif /* FEATURE_G2L_TUNEAWAY */
        }
      }
      break;

      case TRM_UNLOCK_REQUIRED:
      case TRM_UNLOCK_BAND_INCOMPATIBLE:
      {
        if (other_g_reason == TRM_DEMOD_PAGE)
        {
          MSG_GERAN_HIGH_3_G("G2X_TA: IGNORE TRM_UNLOCK_REQUIRED/TRM_UNLOCK_BAND_INCOMPATIBLE for Paging reason %d w_c %d event %d", other_g_reason, gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client, (int)event);
        }
        else
        {
          if (IS_URGENT_ACCESS(other_g_reason))
          {
            /* TRM UNIFICATION
            * Replacement for UNLOCK_IMMEDIATELY
            */
          MSG_GERAN_HIGH_2_G("G2X_TA: TRM_UNLOCK_IMMEDIATELY client %d L1 send RR_DS_ABORT_TRANSFER fn %d", client, gl1_get_FN(gas_id));

            /* L1 is leaving packet transfer mode abnormally.
            * Empty the rrbp queue to ensure L1 drops into Idle.
            */
            gpl1_gprs_reset_rrbp_enteries(gas_id);

            /* if tbf release is pending then ensure tbf release is processed */
            if (gpl1_gprs_awaiting_release(gas_id))
            {
              volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
              transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

              transfer_data_ptr->wait_for_pca_valid = TRUE;
              transfer_data_ptr->wait_for_pca_fn = ADD_FN(gl1_get_FN(gas_id), 1);
            }
            /* tell GRR to trigger a PS abort and give the CS connection time to get the lock */
          L1_send_MPH_L1_DS_ABORT_IND(gas_id, RR_DS_ABORT_TRANSFER, GL1_ACCESS_CONFIRM_TIMEOUT);
          break;
        }
          /*Acitviy5 is already planned. No need to update it again.
           But check if Activity 5 is valid (activity which is planned can be staled)*/
     
          if((gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TRM_UNLOCK, gas_id)) &&
              (data_ptr->table[G2X_TA_PRI_TRM_UNLOCK].frame_number != NOT_A_FRAME_NUMBER) &&
              (data_ptr->trm_unlock.winning_client_id == other_g_client)&&
              (data_ptr->trm_unlock.event == TRM_UNLOCK_REQUIRED)&&
              (IS_FRAME_NUM_LATER(data_ptr->table[G2X_TA_PRI_TRM_UNLOCK].frame_number, gl1_get_FN(gas_id))))
          {
            gpl1_g2x_ta_scheduler(gas_id);
            MSG_GERAN_HIGH_0_G("TRM_UNLOCK_REQUIRED ,Activity is already planned.Avoid updating again");
            break;
          }
          /* possible sub x is doing PT and sub y is doing a power scan so has no
          specific time for unlock but still wants it
          Therefore suspend in arbitrary 7 frames time */
            suspension_FN = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), FRAMES_UNLOCK_REQ_FIRST_SCHEDULE);
          do_schedule_tbf_susp = TRUE;
          MSG_GERAN_HIGH_3_G("TRM_UNLOCK_REQUIRED/TRM_UNLOCK_BAND_INCOMPATIBLE Abort for Client %d suspension_FN %d event %d", client, suspension_FN, (int)event);
#ifdef FEATURE_G2L_TUNEAWAY
          /* Currently allowing only for LTE, in future it may required to all techs*/
          if ((gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE)
              || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SECONDARY)
              || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SUB2)
              || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SUB2_SECONDARY)
              || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_TX)
              || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SUB2_TX)
              )
          {
            if ((gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN) != GPL1_INVALID_FN)
            {
              /* gframe_duration gives the duration between two G2X gaps */
              gframe_duration = SUB_FN(suspension_FN, gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN);
              if (gframe_duration < MIN_DURATION_BETWEEN_G2X_GAPS)
              {
                suspension_FN = ADD_FN(suspension_FN, SUB_FN(MIN_DURATION_BETWEEN_G2X_GAPS, gframe_duration));
              }
              MSG_GERAN_HIGH_3_G("G2X_TA: TRM_UNLOCK_REQUIRED suspension_FN: %d; prev_g2x_end_FN: %d; gframe_duration: %d", suspension_FN, gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN, gframe_duration);
            }
          }
#endif /* FEATURE_G2L_TUNEAWAY */
        }
      }
        break;

      default:
        MSG_GERAN_ERROR_1_G("MULTISIM: unhandled unlock event %d", (int)event);
        break;
    }

    if (do_schedule_tbf_susp)
    {
      if (gpl1_g2x_ta_update_table_field(G2X_TA_PRI_TRM_UNLOCK, other_g_client, suspension_FN, duration, gas_id))
      {
        gpl1_g2x_ta_update_trm_callback_data(client, other_g_client, event, suspension_FN, other_g_reason, gas_id);
      /*Store PCHXFER Duration if PCHXFER is Planned and if TRM_UNLOCK is updated successfully*/
      if((other_g_reason == TRM_DEMOD_PAGE) &&
         (gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_PCH, gas_id)))
      {
         pchxfer_duration = data_ptr->table[G2X_TA_PRI_SERV_PCH].duration;
      }
 
      }
      else
      {
        MSG_GERAN_HIGH_3_G("G2X_TA: Updated Failed reset client details,last unlock event:%d,last_unlock_by_sclk:%d,last_unlock_by_reason %d",
                     gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event,
                     gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_sclk,
                     gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason);

        /* These variables are used to store the last winning client, reason for the last winning client etc.
         * during the GTA gap. These are initialized to invalid values here. These are updated at the start 
         * of the gta_preemption_callback function. The purpose is, when g2x ta table can not be updated in
         * the current function, we need to set invalid values for these variables which were updated in 
         * gta_preemption_callback. Otherwise, we would not be handling the repeated unlock event in gta_preemption_callback.
         * By setting invalid values here, we would be able to handle the repeated unlock event in gta_preemption_callback.
         */
        gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_sclk   = 1;
        gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason = TRM_NUM_REASONS;
      }
      gpl1_g2x_ta_scheduler(gas_id);
    /*Check if there is any change in PCHXFER duration due to TRM_UNLOCK and PCHXFER conflict
      if PCHXFER duration is changed.Try to reserve with new duration which will ensure conflict at TRM POV*/

     if((pchxfer_duration!= L1_GPRS_INVALID_FN )&&
       (pchxfer_duration!= data_ptr->table[G2X_TA_PRI_SERV_PCH].duration))
      {
        gpl1_check_rel_resv_ccch_xfer(FALSE,gas_id);
      }
    } /*if (do_schedule_tbf_susp) */

  } /* ! GBTA success */

  gpl1_g2x_ta_debug_table(gas_id);

  GL1_ISR_UNLOCK(gas_id);
  return;
}
#ifdef FEATURE_GTA_2PHASE_ACCESS
/*===========================================================================

FUNCTION     gta_for_tds_handle_abort_callback

DESCRIPTION  L1 task handler for GTA abort callback which has been triggered from
             TDSCDMA calling the GTA unlock callback

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gta_handle_abort_2phase_access_callback(grm_client_enum_t client,
                               trm_unlock_event_enum_t   event,
                               uint32                    unlock_by_sclk,
                               gas_id_t                  gas_id)
{
    uint32  frame_duration;
    uint32  suspension_FN;
    tbf_suspend_T suspension_time;

    GL1_ISR_LOCK(gas_id);

    switch(event)
    {
        case TRM_UNLOCK_CANCELLED:
            MSG_GERAN_ERROR_1_G("2PHASE_ACCESS TRM_UNLOCK_CANCELLED %d", (int)event);
            // Canel any pending suspension
            gta_2p_susp_info[gas_id].pch_suspension_FN = L1_GPRS_INVALID_FN;
            break;
        case TRM_UNLOCK_REQUIRED:
		case TRM_UNLOCK_BAND_INCOMPATIBLE:	
        case TRM_UNLOCK_IMMEDIATELY:

            MSG_GERAN_HIGH_3_G("2PHASE_ACCESS: TRM_UNLOCK_IMMEDIATELY client %d L1 send RR_DS_ABORT_TRANSFER fn %d event %d", client,gl1_get_FN(gas_id),(int)event);

            L1_send_MPH_L1_DS_ABORT_IND(gas_id, RR_DS_ABORT_TRANSFER, GL1_ACCESS_CONFIRM_TIMEOUT);
            break;

        case TRM_UNLOCK_BY:
            unlock_by_sclk -= timetick_get_safe();
            frame_duration = MSECS_TO_FRAMES(timetick_cvt_from_sclk(unlock_by_sclk, T_MSEC));

            suspension_FN = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), frame_duration );
            if(gpl1_g2x_ta_block_bond(suspension_FN, &suspension_time, gas_id))
            {
              gta_2p_susp_info[gas_id].pch_suspension_FN  = suspension_time.start_frame_num;
            }

            MSG_GERAN_HIGH_3_G("2PHASE_ACCESS: TRM_UNLOCK_BY suspension_FN %d  Abort for Client %d from TRM sclk:%d", gta_2p_susp_info[gas_id].pch_suspension_FN, client, unlock_by_sclk);
            break;

            default:
              MSG_GERAN_ERROR_1_G("MULTISIM: unhandled unlock event %d", (int)event);
            break;

    }
    GL1_ISR_UNLOCK(gas_id);
    return;
}

#endif /*FEATURE_GTA_2PHASE_ACCESS*/

#ifdef FEATURE_SGLTE

/*===========================================================================

FUNCTION     gta_handle_abort_for_l_or_t_callback

DESCRIPTION  L1 task handler for GTA abort callback which has been triggered from LTE or
             TDSCDMA calling the GTA unlock callback

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gta_handle_abort_for_l_or_t_callback(grm_client_enum_t       client,
                               trm_unlock_event_enum_t event,
                               uint32                  unlock_by_sclk,
                               gas_id_t                gas_id)

{
  MSG_GERAN_HIGH_2_G("TRM_UNLOCK_IMMEDIATELY client %d L1 send RR_DS_ABORT_TRANSFER fn %d", client, gl1_get_FN(gas_id));

  /* L1 is leaving packet transfer mode abnormally.
   * Empty the rrbp queue to ensure L1 drops into Idle.
   */
  gpl1_gprs_reset_rrbp_enteries(gas_id);

  /* if tbf release is pending then ensure tbf release is processed */
  if (gpl1_gprs_awaiting_release(gas_id))
  {
    volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

    transfer_data_ptr->wait_for_pca_valid = TRUE;
    transfer_data_ptr->wait_for_pca_fn = ADD_FN(gl1_get_FN(gas_id), 1);
  }

  /* tell GRR to trigger a PS abort and give the 1 sec connection time to get the lock */
  L1_send_MPH_L1_DS_ABORT_IND(gas_id, RR_DS_ABORT_TRANSFER, 0x3E8 );
}
#endif

/*===========================================================================

FUNCTION  grm_gta_preemption_callback

DESCRIPTION
 Pre-emption callback registered with TRM.
 Function:
   - FEATURE_SGLTE use case
   - Check winning client allow: G, W and T
   - Check if TA supported for W/T
   - Store information received from TRM
   - Call  trm_tbf_G2X_abort_callback

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_gta_preemption_callback( trm_unlock_callback_data *unlock_data_p )
{
  gas_id_t gas_id;
  grm_client_enum_t winning_client_id;
  grm_client_enum_t unlock_client_id;
  trm_reason_enum_t winning_reason;
  uint32 winning_client_duration = 0;

#if defined(FEATURE_SGLTE) || defined(FEATURE_SVLTE_DUAL_SIM)
  if (unlock_data_p)
  {
    gas_id = gl1_map_client_id_to_gas_id(grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client));
    gas_id = check_gas_id(gas_id);

    winning_client_id = grm_map_trm_client_id_to_grm_client_id(
       unlock_data_p->winning_client_info.client_id);

    /* doesnt matter what gas_id is passed to gl1_hw_get_sglte_mode
       should return sglte mode regardless*/
    if ((
#if defined(FEATURE_SGLTE)
         gl1_hw_get_sglte_mode(GERAN_ACCESS_STRATUM_ID_1) ||
#endif
         gl1_check_svlte_mode_active()) &&
        (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))
    {
      if ((winning_client_id == GRM_LTE) || (winning_client_id == GRM_LTE_SUB2) ||
          (winning_client_id == GRM_TDSCDMA) ||
          ((((unlock_data_p->event == TRM_UNLOCK_REQUIRED) || (unlock_data_p->event == TRM_UNLOCK_BAND_INCOMPATIBLE)) && (IS_URGENT_ACCESS(unlock_data_p->winning_client_info.reason)))
           || (unlock_data_p->event == TRM_UNLOCK_IMMEDIATELY)
           ))
      {
        gas_id_t                 l_abort_gas_id;
        /* post a message to the L1 task to complete the processing */
        gl1_grm_abort_callback_t  abort_cb_msg;
        gl1_grm_abort_callback_t *abort_cb_msg_p;
        grm_client_enum_t grm_client_id  = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);

        abort_cb_msg_p = &abort_cb_msg;
        l_abort_gas_id = gl1_map_client_id_to_gas_id(grm_client_id);

        MSG_GERAN_HIGH_2_G("unlock request for Client %d for event %d",
                           winning_client_id, (int)unlock_data_p->event);

        abort_cb_msg_p->client         = grm_client_id;
        abort_cb_msg_p->event          = unlock_data_p->event;
        abort_cb_msg_p->unlock_by_sclk = unlock_data_p->unlock_by_sclk;
        abort_cb_msg_p->gas_id         = l_abort_gas_id;

        /* set the inter-task header */
        abort_cb_msg_p->header.message_set = MS_L1_L1;
        abort_cb_msg_p->header.message_id  = L1_GTA_ABORT_FOR_L_OR_T_CALLBACK;
        PUT_IMH_LEN(sizeof(gl1_grm_abort_callback_t) - sizeof(IMH_T), abort_cb_msg_p);

         (void)L1_isr_send_message( GS_QUEUE_LAYER_1, abort_cb_msg_p, FALSE, l_abort_gas_id );

        return;
       }else if ((winning_client_id == GRM_GSM1) || (winning_client_id == GRM_GSM2))
      {
        MSG_GERAN_HIGH_2_G("unlock request for Client %d for event %d", winning_client_id, (int)unlock_data_p->event);
        return;
      }
    }
  }
#endif

  if (unlock_data_p)
  {
    gas_id = gl1_map_client_id_to_gas_id(grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client));

    winning_reason    =
       unlock_data_p->winning_client_info.reason;

    winning_client_id = grm_map_trm_client_id_to_grm_client_id(
       unlock_data_p->winning_client_info.client_id);

    gas_id = check_gas_id(gas_id);

    if ( (unlock_data_p->event != TRM_UNLOCK_CANCELLED) &&
        !(((unlock_data_p->event == TRM_UNLOCK_REQUIRED) || (unlock_data_p->event == TRM_UNLOCK_BAND_INCOMPATIBLE) ) && 
          (IS_URGENT_ACCESS (winning_reason) || (winning_reason==TRM_CHANNEL_MAINTENANCE))) &&
        (unlock_data_p->event != TRM_UNLOCK_IMMEDIATELY) &&
        ( winning_client_id != GRM_UMTS ) &&
        ( winning_client_id != GRM_UMTS_SECONDARY ) &&
        ( winning_client_id != GRM_UMTS2 ) &&
        ( winning_client_id != GRM_UMTS_SECONDARY2 ) &&
        ( winning_client_id != GRM_GSM1 ) &&
        ( winning_client_id != GRM_GSM2 ) &&
        ( winning_client_id != GRM_GPRS1 ) &&
        ( winning_client_id != GRM_GPRS2 )
#ifdef FEATURE_TRIPLE_SIM
        && ( winning_client_id != GRM_GSM3 )
#endif /* FEATURE_TRIPLE_SIM */
#ifdef FEATURE_G2X_TUNEAWAY
        &&(winning_client_id != GRM_TDSCDMA)
#ifdef FEATURE_G2_1X_TUNEAWAY
        &&(winning_client_id != GRM_1X)
#endif
#ifdef FEATURE_G2L_TUNEAWAY
        &&(winning_client_id != GRM_LTE)
        &&(winning_client_id != GRM_LTE_SECONDARY)
        &&(winning_client_id != GRM_LTE_SUB2)
        &&(winning_client_id != GRM_LTE_SUB2_SECONDARY)
        &&(winning_client_id != GRM_LTE_TX)
        &&(winning_client_id != GRM_LTE_SUB2_TX)
#endif /* FEATURE_G2L_TUNEAWAY */
#endif /* FEATURE_G2X_TUNEAWAY */ 
      )
    {
      MSG_GERAN_HIGH_2_G("G2X_TA: Ignoring unlock request for Client %d event %d", winning_client_id, (int)unlock_data_p->event);
      return;
    }

        /* Check if the preemption is for high priority CBCH on the other SUB, then ignore it */
        if ((winning_client_id == GRM_GSM1 || winning_client_id == GRM_GSM2) &&
            (winning_reason == TRM_CHANNEL_MAINTENANCE_INV) &&
            (l1_fm_get_reserve_sub_reason(OTHER_GAS_ID(gas_id)) == SYS_PROC_TYPE_IMS_SMS))
    {
      MSG_GERAN_HIGH_2_G("G2X_TA: Ignoring unlock request for CBCH activity Client %d, reason %d", winning_client_id, winning_reason);
      return;
    }

#ifdef FEATURE_G2X_TUNEAWAY
        if ((((unlock_data_p->event == TRM_UNLOCK_REQUIRED) || (unlock_data_p->event == TRM_UNLOCK_BAND_INCOMPATIBLE)) && IS_URGENT_ACCESS(winning_reason))
            || (unlock_data_p->event == TRM_UNLOCK_IMMEDIATELY)
            )
    {
      MSG_GERAN_HIGH_0_G("G2X_TA: TRM_UNLOCK_IMMEDIATELY received");
    }
     else

        if (((GRM_UMTS == winning_client_id)||(GRM_UMTS_SECONDARY == winning_client_id)||(GRM_UMTS2 == winning_client_id)||(GRM_UMTS_SECONDARY2 == winning_client_id))
                 && (GERAN_TA_SUPPORTED != geran_nv_g2w_ta_supported()))
        {
          MSG_HIGH("G2X_TA: Ignoring unlock request from WCDMA in device_mode: %d, multi_sim_pref:%d, geran_nv_g2w_ta_supported:%d",
                   gl1_msg_get_multi_sim_sys_mode(),
                   geran_get_sys_multi_sim_pref(),
                   geran_nv_g2w_ta_supported()
                   );
          return;
        }
        else if ((GRM_TDSCDMA == winning_client_id) && (GERAN_TA_SUPPORTED != geran_nv_g2t_ta_supported()))
        {
          MSG_HIGH("G2X_TA: Ignoring unlock request from TDSCDMA in device_mode: %d, multi_sim_pref:%d, geran_nv_g2t_ta_supported:%d",
                   gl1_msg_get_multi_sim_sys_mode(),
                   geran_get_sys_multi_sim_pref(),
                   geran_nv_g2t_ta_supported()
                   );
          return;
        }
#ifdef FEATURE_G2_1X_TUNEAWAY
        else if ((GRM_1X == winning_client_id) && (GERAN_TA_SUPPORTED != geran_nv_g21x_ta_supported()))
        {
          MSG_GERAN_HIGH_3_G("G2X_TA: Ignoring unlock request from 1X in device_mode: %d, multi_sim_pref:%d, geran_nv_g2t_ta_supported:%d",
                             gl1_msg_get_multi_sim_sys_mode(),
                             geran_get_sys_multi_sim_pref(),
                             geran_nv_g21x_ta_supported()
                             );
          return;
        }
#endif
#else
        if ((GRM_UMTS == winning_client_id) || (GRM_UMTS_SECONDARY == winning_client_id) ||
            (GRM_UMTS2 == winning_client_id) || (GRM_UMTS_SECONDARY2 == winning_client_id))
    {
      if ((!(((TRM_UNLOCK_REQUIRED == unlock_data_p->event) || (TRM_UNLOCK_BAND_INCOMPATIBLE == unlock_data_p->event)) && (IS_URGENT_ACCESS(winning_reason)))
           /* during GPRS, ignore pre-emption if WCDMA needs the lock for any non-urgent reason */
           && (TRM_UNLOCK_IMMEDIATELY != unlock_data_p->event)
           ))
      {
        MSG_GERAN_HIGH_1_G("Ignoring non-urgent unlock request event %d from WCDMA", (int)unlock_data_p->event);

        return;
      }
    }
#endif/* !FEATURE_G2X_TUNEAWAY */

        if (gas_id >= NUM_GERAN_DATA_SPACES)
    {
      MSG_GERAN_ERROR_1_G("G2X_TA: incorrect gas_id %d", gas_id);
      return;
    }
    else
    if( (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client           == winning_client_id) &&
                 (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason == winning_reason) &&
                 (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event            == unlock_data_p->event) &&
                 (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_sclk   == unlock_data_p->unlock_by_sclk))
        {
          MSG_GERAN_HIGH_0_G("No change in the winning client. Not processing preemption callback");

          return;
        }

        else
        {
          /* Store the last winning client */
          gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client            = winning_client_id;
          gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event             = unlock_data_p->event;
          gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_sclk    = unlock_data_p->unlock_by_sclk;
          gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason  = winning_reason;
        }
        MSG_HIGH("G2X_TA: TRM TBF Abort Req as GSM client %d wants TRM for reason %d (event %d)",
                 winning_client_id,
                 winning_reason,
                 unlock_data_p->event);

        if (gl1_msg_get_multi_sim_standby_mode())
    {
      unlock_client_id = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);

      if (unlock_data_p->event == TRM_UNLOCK_BY)
      {
        winning_client_duration = unlock_data_p->winning_client_info.duration;
      }

      MSG_GERAN_HIGH_2_G(" G2X_TA: TRM TBF Abort Req TBF unlocked_client:%d winning_client_duration=%d ", unlock_client_id, winning_client_duration);

      grm_tbf_G2X_abort_callback(unlock_client_id,
                                 winning_client_id,
                                 unlock_data_p->event,
                                 unlock_data_p->unlock_by_sclk,
                                 winning_reason,
                                 winning_client_duration,
                                 unlock_data_p->ta_info
                                 );
    }
  }

  return;
}

#ifdef FEATURE_GTA_2PHASE_ACCESS
/*===========================================================================

FUNCTION  grm_gta_for_tds_preemption_callback

DESCRIPTION
  This function handles the TRM abort callback for the GSM client
  during 2 Phase Access and TRM is required by TD-SCDMA
  And changes the trm_status within the clients global data, this is picked
  up by the ISR.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_gta_2phase_access_preemption_callback(trm_unlock_callback_data *unlock_data_ptr)
{
    if(unlock_data_ptr != NULL)
    {
        gas_id_t gas_id;
        gl1_grm_2phase_access_abort_callback_t abort_cb_msg;
        trm_reason_enum_t reason = unlock_data_ptr->winning_client_info.reason;
        trm_unlock_event_enum_t event = unlock_data_ptr->event;
        grm_client_enum_t winning_client_id;
        idle_data_T *l1_idle_data = NULL;

        gas_id = gl1_map_client_id_to_gas_id( grm_map_trm_client_id_to_grm_client_id(unlock_data_ptr ->unlock_client ));
        gas_id = check_gas_id(gas_id);
        l1_idle_data = &l1_idle_data_store[gas_id];
        winning_client_id =
          grm_map_trm_client_id_to_grm_client_id(unlock_data_ptr->winning_client_info.client_id);

        if ( ((GERAN_TA_SUPPORTED == geran_nv_g2x_ta_supported(winning_client_id)) && (l1_idle_data->non_updated_lai_in_nmo1 == FALSE) &&
             (
               (( TRM_UNLOCK_BY == event ) && ( TRM_DEMOD_PAGE == reason)) ||
               (( (TRM_UNLOCK_REQUIRED == event) || (TRM_UNLOCK_BAND_INCOMPATIBLE == event) ) && (IS_URGENT_ACCESS (reason))) ||
               ( TRM_UNLOCK_IMMEDIATELY == event )
              )
              ) ||
                (TRM_UNLOCK_CANCELLED == event )
           )
        {
            MSG_GERAN_HIGH_4_G("2PHASE_ACCESS: Unlock event :%d request for Client %d Winning Reason : %d NV_TA: %d",unlock_data_ptr->event,
                               unlock_data_ptr->winning_client_info.client_id, unlock_data_ptr->winning_client_info.reason, geran_nv_g2x_ta_supported(winning_client_id));
        }
        else
        {
            gta_2p_susp_info[gas_id].pch_suspension_FN = L1_GPRS_INVALID_FN;
            MSG_GERAN_HIGH_4_G("2PHASE_ACCESS: Ignoring unlock event :%d request for Client %d Winning Reason :%d, NV_TA: %d",unlock_data_ptr->event,
                                           unlock_data_ptr->winning_client_info.client_id, unlock_data_ptr->winning_client_info.reason, geran_nv_g2x_ta_supported(winning_client_id));

            return;
        }
        if (gl1_msg_get_multi_sim_standby_mode())
        {
            // Prepare the message to be sent to L1 task for further processing
            abort_cb_msg.tbf_client_id         = grm_map_trm_client_id_to_grm_client_id(unlock_data_ptr->unlock_client);
            abort_cb_msg.winning_client_id     = winning_client_id;
            abort_cb_msg.event                 = unlock_data_ptr->event;
            abort_cb_msg.unlock_in_sclk        = unlock_data_ptr->unlock_by_sclk;
            abort_cb_msg.gas_id                = gas_id;

            abort_cb_msg.header.message_set = MS_L1_L1;
            abort_cb_msg.header.message_id  = L1_GTA_FOR_2PHASE_ACCESS_ABORT_CALLBACK;
            PUT_IMH_LEN( sizeof( gl1_grm_2phase_access_abort_callback_t ) - sizeof( IMH_T ), &abort_cb_msg );

            (void)L1_isr_send_message(GS_QUEUE_LAYER_1, &abort_cb_msg, FALSE, gas_id);

            MSG_GERAN_MED_3_G("2PHASE_ACCESS: L1_GTA_FOR_2PHASE_ACCESS_ABORT_CALLBACK  tbf_client_id:%d ; event:%d, winning_client_id %d",abort_cb_msg.tbf_client_id, abort_cb_msg.event, abort_cb_msg.winning_client_id);

        }
      }
}
#endif  //FEATURE_GTA_2PHASE_ACCESS
/*===========================================================================

FUNCTION  grm_tbf_enh_grant_callback

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_tbf_enh_grant_callback(grm_client_enum_t           client,
                                trm_grant_return_enum_type  grant_status,
                                trm_duration_t              duration)
{
  uint32 frame_duration;
  volatile ISRTIM_CMD_BLK *l1_tsk_buffer_ptr;

  gas_id_t gas_id = gl1_map_client_id_to_gas_id( client );

  l1_tsk_buffer_ptr = &l1_tsk_buffer[gas_id];

  if (grm_get_trm_status(client, gas_id) == GL1_TRM_GRANTED)
  {
    MSG_GERAN_HIGH_1_G("Client %d already granted",client);
    return;
  }

#ifdef FEATURE_GSM_RX_TX_SPLIT
  if (grm_is_a_tx_client(client)) 
  {
    grm_enh_grant_callback(client,
                           grant_status,
                           duration);
  }
  else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  {
    /*The number of frames it takes of the allowed time, to tell ISR it has the TRM
    lock, and then again for the ISR to tell the task to release the lock. 1 Frame each way.*/
    if (grm_get_trm_status(client, gas_id) != GL1_TRM_CALLBACK_PENDING )
    {
      /* Issue the TRM release as we were not waiting for the lock, so give it back */
      MSG_GERAN_HIGH_1_G("Client %d no longer waiting for lock",client);
      if(l1_tsk_buffer_ptr->firmware_load_status != FW_PENDING_RELEASE)
      {
        call_in_task_grm_release_in_callback( client );
      }
      return;
    }

    if ( grant_status != TRM_DENIAL )
    {
      grm_enh_grant_callback(client, grant_status, duration);

      duration -= timetick_get_safe();
      frame_duration = MSECS_TO_FRAMES( timetick_cvt_from_sclk(duration, T_MSEC) );

      MSG_GERAN_HIGH_2_G("Grant for Client %d from TRM for %d frames",client,frame_duration);
    }
    else
    {
      grm_set_trm_status(GL1_TRM_NOT_GRANTED, client, gas_id);
      MSG_GERAN_HIGH_1_G("Denial for Client %d from TRM",client);
    }
  }
  return;
}

#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION  gpl1_gprs_set_pch_suspension

DESCRIPTION
        Can get updated during TRM EXTENSION example EXT PCH

DEPENDENCIES

RETURN VALUE
  TRUE - do not proceed with grant
  FALSE - do not proceed

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gbta_check_tbf_susp_planned(gas_id_t gas_id )
{
  boolean tbf_susp_planned =  FALSE;
  uint32 next_suspension_FN, next_resume_FN;
  if( GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id )
  {
    return TRUE;
  }
#ifdef FEATURE_G2X_TUNEAWAY
 if(gpl1_g2x_ta_next_planned_suspension(&next_suspension_FN, &next_resume_FN, gas_id))
  {
    if(IS_FRAME_NUM_LATER(next_suspension_FN, trans[gas_id].gbta_current_pch_suspension_FN))
    {
      if(!IS_FRAME_NUM_LATER(next_suspension_FN, trans[gas_id].gbta_current_pch_suspension_FN+L1_SC_DURATION_GBTA))
      {
        /* Overlapping detected between TBF suspension and GBTA */
        tbf_susp_planned =  TRUE;
      }
    }
    else
    {
      /*Next TBF suspension is sooner then GBTA start */
      if(IS_FRAME_NUM_LATER(next_resume_FN, trans[gas_id].gbta_current_pch_suspension_FN))
      {
        /* Overlapping detected between TBF suspension and GBTA */
        tbf_susp_planned =  TRUE;
       }
     }
   }
  else
  {
    MSG_GERAN_MED_1_G( "GBTA tbf_susp_planned FALSE (gbta OK ) gbta_suspension_FN %d",trans[gas_id].gbta_current_pch_suspension_FN);
  }

#endif

  return tbf_susp_planned;
}  /* gpl1_gbta_check_tbf_susp_planned */
#endif /*  FEATURE_GPRS_GBTA */


/*===========================================================================

FUNCTION  grm_tbf_G2W_abort_callback

DESCRIPTION  Handles the asynchronous callback from TRM when a GPRS Tuneaway
             gap needs to be opened as a result of activity on another client.
             Functionality:
              - set LTA extension flag  (legacy, might be removed)
              - check if reason is allowed
              - send message L1_GTA_ABORT_CALLBACK to task

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void grm_tbf_G2X_abort_callback(
  grm_client_enum_t               tbf_client_id,
  grm_client_enum_t               winning_client_id,
  trm_unlock_event_enum_t         event,
  uint32                          unlock_in_sclk,
  trm_reason_enum_t               winning_client_reason,
  uint32                          winning_client_duration,
  trm_tuneaway_enum_type          ta_info
)
{
  gas_id_t          l_active_tbf_gas_id;
  int               as_idx;
  gl1_grm_preemption_abort_callback_t  abort_cb_msg;
  gl1_grm_preemption_abort_callback_t  *abort_cb_msg_p;

  l_active_tbf_gas_id = gl1_map_client_id_to_gas_id( tbf_client_id );
  as_idx              = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX ( l_active_tbf_gas_id);
    /* post a message to the L1 task to complete the processing */
    abort_cb_msg_p = &abort_cb_msg;

    abort_cb_msg_p->tbf_client_id         = tbf_client_id;
    abort_cb_msg_p->winning_client_id     = winning_client_id;
    abort_cb_msg_p->event          = event;
    abort_cb_msg_p->unlock_in_sclk = unlock_in_sclk;
    abort_cb_msg_p->gas_id         = l_active_tbf_gas_id;
    abort_cb_msg_p->winning_client_duration = winning_client_duration;
    abort_cb_msg_p->ta_info = ta_info;

    /* set the inter-task header */
    abort_cb_msg_p->header.message_set = MS_L1_L1;
    abort_cb_msg_p->header.message_id  = L1_GTA_ABORT_CALLBACK;
    PUT_IMH_LEN( sizeof( gl1_grm_preemption_abort_callback_t ) - sizeof( IMH_T ), abort_cb_msg_p );

    (void)L1_isr_send_message(GS_QUEUE_LAYER_1, abort_cb_msg_p, FALSE, l_active_tbf_gas_id);

    MSG_HIGH("G2X_TA: L1_GTA_ABORT_CALLBACK  TBF tbf_client_id:%d ; event:%d, l_active_tbf_gas_id:%d",tbf_client_id,event,l_active_tbf_gas_id);

      return;
 }

boolean gpl1_is_client_g(grm_client_enum_t client_id)
{

 if (
       (GRM_GSM1 == client_id)
#if defined (FEATURE_DUAL_SIM)
    || (GRM_GSM2 == client_id)
#endif
#if defined (FEATURE_TRIPLE_SIM)
  || (GRM_GSM3 == client_id)
#endif
#ifdef FEATURE_DUAL_SIM
    || (GRM_GPRS1 == client_id)
    || (GRM_GPRS2 == client_id)
#endif /* FEATURE_DUAL_SIM */
  )
    {
    return TRUE;
    }
    else
    {
    return FALSE;
  }

    }
#endif /* FEATURE_G2X_TUNEAWAY */
/*===========================================================================
                        AGA: G2X TA DATA STRUCTURE PROCEDURES
===========================================================================*/

boolean gpl1_g2x_ta_update_table_field( gpl1_g2x_ta_priority_t pri, grm_client_enum_t client_id, uint32 frame_number, uint16 duration, gas_id_t gas_id)
{
  tbf_suspend_T suspension_time;
  boolean update_success = FALSE;
  gpl1_g2x_ta_data_t *data_ptr = &gpl1_g2x_ta_data[gas_id];
  GL1_G2X_TUNEAWAY_LOCK();
/* integrity checks before update*/
  if (pri < G2X_TA_PRI_MAX)
  {
    if (data_ptr->table[pri].status == ONGOING)
    {
      MSG_GERAN_ERROR_1_G("G2X_TA UPDATE: Ongoing activity:%d, can not be updated", pri);
      GL1_G2X_TUNEAWAY_UNLOCK();
      return FALSE;
    }
    else if (!IS_FRAME_NUM_LATER(frame_number, gl1_get_FN(gas_id)))
    {
      MSG_GERAN_ERROR_2_G("G2X_TA UPDATE:  Activity:%d, can not be updated current FN is later then %d ", pri, frame_number);
      GL1_G2X_TUNEAWAY_UNLOCK();
      return FALSE;
    }
  }

  data_ptr->table[pri].client_id    = client_id;
  data_ptr->table[pri].status       = WAITING; /* Awaiting scheduling */
  suspension_time.g2x_notGTA = FALSE;

  switch (pri)
  {
    case G2X_TA_PRI_TBF_TS_CHANGE:
       data_ptr ->table[pri].frame_number = frame_number;
       data_ptr ->table[pri].duration     = duration;
       data_ptr ->table[pri].async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;
       update_success = TRUE;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
       gpl1_gprs_recovery_register_event(GPL1_LOW_TS_EVENT,frame_number,gas_id);
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */
      break;

    case G2X_TA_PRI_WID_SEARCH:
    case G2X_TA_PRI_SERV_BCCH:
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    case G2X_TA_PRI_SERV_PCH:
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
    {
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if ((pri == G2X_TA_PRI_SERV_BCCH) || (pri == G2X_TA_PRI_SERV_PCH))
      {
        data_ptr->table[pri].frame_number = frame_number;
        data_ptr->table[pri].duration     = L1_SC_DURATION_BCCH;
        data_ptr->table[pri].async_ccch_type = ASYNC_ACTIVITY_GBTA;
        MSG_GERAN_MED_4_G("G2X_TA UPDATE:GBTA(CCCH):  Activity:%d, now at %d  for %d frames, STATUS:%d", pri, data_ptr->table[pri].frame_number, data_ptr->table[pri].duration, data_ptr->table[pri].status);
        update_success = TRUE;
      }
      else
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
        if (gpl1_g2x_ta_block_bond(frame_number, &suspension_time, gas_id))
        {
          data_ptr->table[pri].frame_number = suspension_time.start_frame_num;
          data_ptr->table[pri].duration     = suspension_time.number_of_frames;
          data_ptr->table[pri].async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;
          update_success = TRUE;
        }
    }
      break;

    case G2X_TA_PRI_TRM_UNLOCK:
    {
#ifdef FEATURE_G2X_TUNEAWAY
      if (!gpl1_is_client_g(data_ptr->table[pri].client_id))
      {
        suspension_time.g2x_notGTA = TRUE;
      }

      if (gpl1_g2x_ta_block_bond(frame_number, &suspension_time, gas_id))
      {

#ifdef FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK
        if (suspension_time.start_frame_num != frame_number)
        {
          duration  +=  SUB_FN(frame_number, suspension_time.start_frame_num);
          MSG_GERAN_HIGH_1_G("G2X_TA UPDATE: Updated duration after checking block boundary %d", duration);
        }
#endif /*FEATURE_USE_ACTUAL_TRM_DUR_UNLOCK*/

        data_ptr->table[pri].frame_number = suspension_time.start_frame_num;
        data_ptr->table[pri].duration     = duration;
        data_ptr->table[pri].async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;
        update_success = TRUE;
      }
#else
      update_success = FALSE;
      MSG_GERAN_ERROR_0_G("G2X_TA UPDATE: Unexpected pri in MonoSIM Only config pri:G2X_TA_PRI_TRM_UNLOCK ");
#endif /* FEATURE_G2X_TUNEAWAY */


    }
      break;

    default:
      MSG_GERAN_ERROR_1_G("G2X_TA UPDATE: Unexpected pri: %d ", pri);
      break;

  } /*switch(pri) */

  GL1_G2X_TUNEAWAY_UNLOCK();

  if (TRUE == update_success)
  {
    MSG_GERAN_HIGH_4_G("G2X_TA UPDATE:  Activity:%d, now at %d  for %d frames, STATUS:%d", pri, data_ptr->table[pri].frame_number, data_ptr->table[pri].duration, data_ptr->table[pri].status);
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}



void gpl1_g2x_ta_update_trm_callback_data(
  grm_client_enum_t               tbf_client_id,
  grm_client_enum_t               winning_client_id,
  trm_unlock_event_enum_t         event,
  uint32                          trm_suspension_fn, 
  trm_reason_enum_t               winning_client_reason,
  gas_id_t gas_id
)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];

  data_ptr ->trm_unlock.tbf_client_id=tbf_client_id;
  data_ptr ->trm_unlock.winning_client_id=winning_client_id;
  data_ptr ->trm_unlock.event=event;
  data_ptr ->trm_unlock.trm_suspension_fn=trm_suspension_fn;
  data_ptr ->trm_unlock.winning_client_reason=winning_client_reason;

}

boolean gpl1_g2x_ta_cancel_table_field( gpl1_g2x_ta_priority_t pri, gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t *data_ptr = &gpl1_g2x_ta_data[gas_id];

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif
#endif/* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

  GL1_G2X_TUNEAWAY_LOCK();
  switch (pri)
  {
    case G2X_TA_PRI_TRM_UNLOCK:
    {
        data_ptr ->trm_unlock.tbf_client_id = GRM_NO_CLIENT;
        data_ptr ->trm_unlock.winning_client_id = GRM_NO_CLIENT;
        data_ptr ->trm_unlock.event=TRM_UNLOCK_BAND_INCOMPATIBLE; 
        data_ptr ->trm_unlock.trm_suspension_fn=NOT_A_FRAME_NUMBER;
        data_ptr ->trm_unlock.winning_client_reason=TRM_NUM_REASONS; 
    } break;
    case G2X_TA_PRI_SERV_BCCH:
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    case G2X_TA_PRI_SERV_PCH:
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
      data_ptr->non_trm_act.nbcch_fn = NOT_A_FRAME_NUMBER;
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
      if ((pri == G2X_TA_PRI_SERV_PCH) &&
          (data_ptr->table[pri].async_ccch_type == ASYNC_ACTIVITY_GBTA) &&
          gl1_msg_get_multi_sim_standby_mode() &&
          (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) && // do not want trm release in DTM mode
          (GET_PCHXFER_DR_STATE() == GRM_PCHXFER_DR_OFF)) //PCHXFER DR State is DR Off means Client Exchange has happened
      {
         
        grm_client_enum_t client_id = data_ptr->table[pri].client_id;

        if ((client_id == gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client) &&
            !(gpl1_gprs_leaving_transfer(gas_id)))
        {
          grm_info_t  *grm_info_p = NULL;
          grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

          if (grm_info_p != NULL)
          {
            grm_inform_grant_event(client_id,
                                   gl1_ms_get_trm_resource(gas_id, grm_info_p->rsv_reason),
                                   grm_info_p->duration,
                                   grm_info_p->rsv_reason,
                                   TRM_GRANTED_CHAIN0
                                   );
          }
          else
          {
            MSG_GERAN_ERROR_1_G("GRM info NULL for client id %d", client_id);
          }
        }
        else
        {
          MSG_GERAN_MED_2_G("PCHXFER###: we are NOT Winning client , winn client %d leaving_transfer %d", gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client, gpl1_gprs_leaving_transfer(gas_id));
        }

        //clean up the table entry for PCH/BCCH in transfer activity before releasing trm to prevent any race condition due to trm cb
        data_ptr->table[pri].frame_number = NOT_A_FRAME_NUMBER;
        data_ptr->table[pri].duration 	= NOT_A_FRAME_DURATION;
        data_ptr->table[pri].status	 = NO_ACT;
        data_ptr->table[pri].async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;
        data_ptr->table[pri].client_id = GRM_NO_CLIENT;

        grm_release_pchxfer(client_id, gas_id);
      }
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
      break;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
      case G2X_TA_PRI_TBF_TS_CHANGE:
           gpl1_gprs_recovery_deregister_event(GPL1_LOW_TS_EVENT,gas_id);
      break;
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */

    break;
    case G2X_TA_PRI_WID_SEARCH:
      data_ptr->non_trm_act.wIDsearch_fn = NOT_A_FRAME_NUMBER;
      break;
    default:
      break;
  }

  if ((pri > G2X_TA_PRI_RESERVED) && (pri < G2X_TA_PRI_MAX))
  {
    MSG_GERAN_HIGH_4_G("G2X_TA CANCEL:  Activity:%d was cancelled , was at %d  for %d frames and STATUS:%d", pri, data_ptr->table[pri].frame_number, data_ptr->table[pri].duration, data_ptr->table[pri].status);
    data_ptr->table[pri].frame_number = NOT_A_FRAME_NUMBER;
    data_ptr->table[pri].duration     = NOT_A_FRAME_DURATION;
    data_ptr->table[pri].client_id    = GRM_NO_CLIENT;
    data_ptr->table[pri].status    = NO_ACT;
    data_ptr->table[pri].async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;
    GL1_G2X_TUNEAWAY_UNLOCK();
    return TRUE;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("G2X_TA CANCEL: Unexpected pri: %d ", pri);
    GL1_G2X_TUNEAWAY_UNLOCK();
    return FALSE;
  }


}


boolean gpl1_g2x_ta_suspend_table_field( gpl1_g2x_ta_priority_t pri, gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  /* integrity checks before update*/
  if(pri<G2X_TA_PRI_MAX)
    {
      if (ONGOING == data_ptr ->table[pri].status)
      {
          MSG_GERAN_ERROR_1_G("G2X_TA SUSPEND: Ongoing activity:%d,is not allowed",pri);
          return FALSE;
      }
      else if((NO_ACT == data_ptr ->table[pri].status)||(STOPPED == data_ptr ->table[pri].status))
      {
        MSG_GERAN_LOW_2_G("G2X_TA SUSPEND: Activity:%d,with status %d can't be suspended",pri,data_ptr ->table[pri].status);
        return FALSE;
      }
  }

  switch(pri)
  {
    case G2X_TA_PRI_TBF_TS_CHANGE:
    case G2X_TA_PRI_WID_SEARCH:
    case G2X_TA_PRI_SERV_BCCH:
#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    case G2X_TA_PRI_SERV_PCH:
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
    case G2X_TA_PRI_TRM_UNLOCK:
    {
        // KEEP frame_number, duration and client for rescheduling
        data_ptr ->table[pri].status = SUSPENDED;
        MSG_GERAN_HIGH_3_G("G2X_TA SUSPEND:  Activity:%d was cancelled , now at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        return TRUE;
    }

    break;


    default:
      MSG_GERAN_ERROR_1_G("G2X_TA SUSPEND: Unexpected pri: %d ",pri);
    break;

  } /*switch(pri) */

  return FALSE;

}

boolean gpl1_g2x_ta_suspend_all_table(gas_id_t gas_id)
{
  gpl1_g2x_ta_priority_t pri;
  boolean suspend_done = TRUE;

  MSG_GERAN_ERROR_1_G("G2X_TA SUSPEND ALL at FN:%d",gl1_get_FN(gas_id));

  for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++)
  {
    if(!gpl1_g2x_ta_suspend_table_field( pri,gas_id))
      suspend_done = FALSE;

  }

  return suspend_done;

  }


boolean gpl1_g2x_ta_stop_all_act(gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;

   for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++)
    {
      if (data_ptr ->table[pri].status == ONGOING)
  {
        MSG_GERAN_ERROR_1_G("G2X_TA STOP ALL: Ongoing activity:%d,is not allowed",pri);
        return FALSE;
      }
    }


  data_ptr ->trm_unlock.tbf_client_id = GRM_NO_CLIENT;
  data_ptr ->trm_unlock.winning_client_id = GRM_NO_CLIENT;
  data_ptr ->trm_unlock.event=TRM_UNLOCK_BAND_INCOMPATIBLE; 
  data_ptr ->trm_unlock.trm_suspension_fn=NOT_A_FRAME_NUMBER;
  data_ptr ->trm_unlock.winning_client_reason=TRM_NUM_REASONS; 

  for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++)
  {

        data_ptr ->table[pri].frame_number = NOT_A_FRAME_NUMBER;
        data_ptr ->table[pri].duration     = NOT_A_FRAME_DURATION;
        data_ptr ->table[pri].client_id    = GRM_NO_CLIENT;
        data_ptr ->table[pri].status    = NO_ACT;
        data_ptr ->table[pri].async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;


  }

  MSG_GERAN_HIGH_1_G("G2X_TA STOP ALL: Init G2X TA datat at ",gl1_get_FN(gas_id));
  return TRUE;

  }



boolean gpl1_g2x_ta_is_suspension_ongoing(gas_id_t gas_id, gpl1_g2x_ta_priority_t *pri_ongoing)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;
  *pri_ongoing = G2X_TA_PRI_RESERVED;

   for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++)
    {
      if ((data_ptr ->table[pri].status == ONGOING) && (data_ptr ->table[pri].async_ccch_type == ASYNC_ACTIVITY_SUSPENSION))
      {
        *pri_ongoing = pri;
        MSG_GERAN_HIGH_3_G("G2X_TA ONGOING?:  Activity:%d was cancelled , now at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        return TRUE;
}
    }
  return FALSE;

}


boolean gpl1_g2x_ta_is_activty_running(gpl1_g2x_ta_priority_t *pri_ongoing,gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;
  *pri_ongoing = G2X_TA_PRI_RESERVED;

  for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++)
  {
    if (data_ptr ->table[pri].status == ONGOING)
    {
      *pri_ongoing = pri;
      return TRUE;
    }
  }
  return FALSE;
} /* gpl1_g2x_ta_is_activty_ongoing */


boolean gpl1_g2x_ta_next_planned_suspension(uint32 *suspension_fn, uint32 *resume_fn ,gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;
  *suspension_fn=L1_GPRS_INVALID_FN;
  *resume_fn=L1_GPRS_INVALID_FN;

   for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++)
   {
      if (data_ptr ->table[pri].status == ONGOING)
      {
        MSG_GERAN_HIGH_3_G("G2X_TA PLANNED?:  Activity:%d is ONGOING, now at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        return FALSE;
      }
      else if(data_ptr ->table[pri].status == NEXT_ACT)
      {
        MSG_GERAN_HIGH_3_G("G2X_TA PLANNED?:  Activity:%d is NEXT, at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef  FEATURE_DUAL_SIM
        if(gl1_msg_get_multi_sim_standby_mode() &&
           (pri == G2X_TA_PRI_SERV_PCH) &&
           (data_ptr->table[pri].client_id != gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client))
        {
          MSG_GERAN_HIGH_0_G("G2X_TA PLANNED? : Activity 4, is not the winning client..so no susp planned");
          return FALSE;
        }
        else
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

        {
          *suspension_fn = data_ptr ->table[pri].frame_number;
          *resume_fn     = data_ptr ->table[pri].frame_number+data_ptr ->table[pri].duration;
          return TRUE;
        }
      }
    }
    return FALSE;
}



boolean gpl1_g2x_ta_is_act_planned(gpl1_g2x_ta_priority_t pri, gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];

      if (data_ptr->table[pri].status == ONGOING)
{

        MSG_GERAN_HIGH_3_G("G2X_TA IS ACT PLANNED?:  Activity:%d is ONGOING, now at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        return FALSE;
      }
      else if((data_ptr->table[pri].status == NEXT_ACT) || (data_ptr->table[pri].status == WAITING))
      {
        MSG_GERAN_HIGH_3_G("G2X_TA PLANNED?:  Activity:%d is NEXT or WAITING, at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        return TRUE;
      }
      else
      {
        return FALSE;
      }
}

boolean gpl1_g2x_ta_is_TS_act_imminent(gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t  pri;

  if((data_ptr->table[G2X_TA_PRI_TBF_TS_CHANGE].status == WAITING) || (data_ptr->table[G2X_TA_PRI_TBF_TS_CHANGE].status == NEXT_ACT))
  {
     if((gpl1_g2x_ta_is_activty_running( &pri, gas_id))&&
        (!IS_FRAME_NUM_LATER(SUB_FN(data_ptr->table[G2X_TA_PRI_TBF_TS_CHANGE].frame_number, 1), ADD_FN(data_ptr->table[pri].frame_number, data_ptr->table[pri].duration))))
       {
         MSG_GERAN_HIGH_3_G("G2X_TA Lower TS change act IMMINENT:  Activity:%d is ONGOING, at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
         return TRUE;
       }
   }
 return FALSE;
}


/*===========================================================================

FUNCTION  gpl1_g2x_ta_get_act_status

DESCRIPTION  returns the state of the activity

DEPENDENCIES
  None

RETURN VALUE
  State of the activity input

SIDE EFFECTS
  None

===========================================================================*/

gpl1_g2x_ta_act_status_t gpl1_g2x_ta_get_act_status(gpl1_g2x_ta_priority_t pri, gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  return (gpl1_g2x_ta_act_status_t)data_ptr ->table[pri].status;
}

#ifdef FEATURE_G2X_TUNEAWAY
/*===========================================================================

FUNCTION  gpl1_g2x_ta_is_ts_change_missed

DESCRIPTION

This function used to check if we missed to do time slot change, if missed we need to do DS_ABORT and move to idle

DEPENDENCIES
  None

RETURN VALUE
  TRUE - TS adjustment is missed
  FALSE - TS adjustment is not missed

SIDE EFFECTS
  None

===========================================================================*/

boolean gpl1_g2x_ta_is_ts_change_missed(gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t  pri = G2X_TA_PRI_TBF_TS_CHANGE;

  if( (data_ptr ->table[pri].frame_number != NOT_A_FRAME_NUMBER) && !IS_FRAME_NUM_LATER( SUB_FN(data_ptr ->table[pri].frame_number, FRAME_OFFSET_TS_CHANGE), gl1_get_FN(gas_id)) )
  {
    MSG_GERAN_HIGH_2_G("G2X_TA TS adjustement missed TS_CHANGE FN %d current FN %d ",data_ptr ->table[pri].frame_number, gl1_get_FN(gas_id));
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION  gpl1_g2x_ta_are_act_colliding

DESCRIPTION  check if two activities are colliding. i.ele they have overlapping times

DEPENDENCIES
  None

RETURN VALUE
  TRUE if activities collide else FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_g2x_ta_are_act_colliding(gpl1_g2x_ta_priority_t pri1, gpl1_g2x_ta_priority_t pri2, gas_id_t gas_id)
{
  boolean collision = FALSE;
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  if((data_ptr ->table[pri1].status == WAITING || data_ptr ->table[pri1].status == NEXT_ACT) &&
     (data_ptr ->table[pri2].status == WAITING || data_ptr ->table[pri2].status == NEXT_ACT))
  {
    if(IS_FRAME_NUM_LATER(data_ptr ->table[pri2].frame_number, data_ptr ->table[pri1].frame_number))
    {
      if(IS_FRAME_NUM_LATER_OR_EQUAL(ADD_FN(data_ptr ->table[pri1].frame_number, data_ptr ->table[pri1].duration ), data_ptr ->table[pri2].frame_number))
      {
        collision = TRUE;
        /* plus addition of three frames to ensure collision. */
        data_ptr ->table[pri1].duration = ADD_FN(SUB_FN(data_ptr->trm_unlock.trm_suspension_fn, data_ptr ->table[pri1].frame_number), 3);
      }
    }
    else
    {
      if(IS_FRAME_NUM_LATER_OR_EQUAL(ADD_FN(data_ptr ->table[pri2].frame_number, data_ptr ->table[pri2].duration ), data_ptr ->table[pri1].frame_number))
      {
        if ( data_ptr ->table[pri2].frame_number == data_ptr ->table[pri1].frame_number )
        {
          data_ptr ->table[pri1].duration = ADD_FN(SUB_FN(data_ptr->trm_unlock.trm_suspension_fn, data_ptr ->table[pri1].frame_number), 3);
        }
        collision = TRUE;
      }
    }
  }
  MSG_GERAN_MED_5_G("G2X_TA :PCHXFER : Act colliding : %d ,pri1 %d , pri2 %d, pri1fn [%d], pr2fn [%d]" ,
                      collision,
                      pri1,
                      pri2,
                      data_ptr ->table[pri1].frame_number,
                      data_ptr ->table[pri2].frame_number);
  return collision;
}
#endif /* FEATURE_G2X_TUNEAWAY */

void gpl1_g2x_ta_data_init(gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t *data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;

  data_ptr->trm_unlock.tbf_client_id = GRM_NO_CLIENT;
  data_ptr->trm_unlock.winning_client_id = GRM_NO_CLIENT;
  data_ptr->trm_unlock.event = TRM_UNLOCK_BAND_INCOMPATIBLE; 
  data_ptr->trm_unlock.trm_suspension_fn=NOT_A_FRAME_NUMBER;
  data_ptr->trm_unlock.winning_client_reason = TRM_NUM_REASONS;
#ifdef FEATURE_G2L_TUNEAWAY
  data_ptr->trm_unlock.prev_g2x_end_FN = GPL1_INVALID_FN;
#endif /* FEATURE_G2L_TUNEAWAY */
  data_ptr->non_trm_act.nbcch_fn = NOT_A_FRAME_NUMBER;
  data_ptr->non_trm_act.wIDsearch_fn = NOT_A_FRAME_NUMBER;
  for (pri = G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++)
  {

    data_ptr->table[pri].frame_number = NOT_A_FRAME_NUMBER;
    data_ptr->table[pri].duration     = NOT_A_FRAME_DURATION;
    data_ptr->table[pri].client_id    = GRM_NO_CLIENT;
    data_ptr->table[pri].status    = NO_ACT;
    data_ptr->table[pri].async_ccch_type = ASYNC_ACTIVITY_SUSPENSION;
  }

  gpl1_g2x_ta_deregister_seq(gas_id);

  MSG_GERAN_HIGH_1_G("G2X_TA INIT: Init G2X TA datat at %d", gl1_get_FN(gas_id));

}



void gpl1_g2x_ta_debug_table(gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;
  for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++ )
  {
    if (data_ptr->table[pri].status != NO_ACT)
    {
      switch(pri)
      {
        case G2X_TA_PRI_TBF_TS_CHANGE:
        {
          MSG_GERAN_LOW_4_G("G2X_TA DEBUG:Activity G2X_TA_PRI_TBF_TS_CHANGE: STATUS:%d, client:%d at FN %d  for %d frames",data_ptr ->table[pri].status,data_ptr ->table[pri].client_id,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        }
        break;
        case G2X_TA_PRI_WID_SEARCH:
        {
          MSG_GERAN_LOW_4_G("G2X_TA DEBUG:Activity G2X_TA_PRI_WID_SEARCH: STATUS:%d, client:%d at FN %d  for %d frames",data_ptr ->table[pri].status,data_ptr ->table[pri].client_id,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        }
        break;
        case G2X_TA_PRI_SERV_BCCH:
        {
          MSG_GERAN_LOW_4_G("G2X_TA DEBUG:Activity G2X_TA_PRI_SERV_BCCH: STATUS:%d, client:%d at FN %d  for %d frames",data_ptr ->table[pri].status,data_ptr ->table[pri].client_id,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        }
        break;
        case G2X_TA_PRI_TRM_UNLOCK:
        {
          MSG_GERAN_LOW_4_G("G2X_TA DEBUG:Activity G2X_TA_PRI_TRM_UNLOCK: STATUS:%d, client:%d at FN %d  for %d frames",data_ptr ->table[pri].status,data_ptr ->table[pri].client_id,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        }
        break;

#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
        case G2X_TA_PRI_SERV_PCH:
        {
          MSG_GERAN_LOW_4_G("G2X_TA DEBUG:Activity G2X_TA_PRI_SERV_PCH: STATUS:%d, client:%d at FN %d  for %d frames",data_ptr ->table[pri].status,data_ptr ->table[pri].client_id,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
        }
        break;
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

        default:
        MSG_GERAN_ERROR_1_G("G2X_TA DEBUG: Unexpected pri: %d ",pri);
        break;
      } /*switch(pri) */
    }
  }

  MSG_GERAN_LOW_5_G("G2X_TA DEBUG: tbf_client_id:%d; winning_client_id:%d; event %d; trm_suspension_fn:%d; winning_client_reason:%d",data_ptr ->trm_unlock.tbf_client_id,
  data_ptr ->trm_unlock.winning_client_id, data_ptr ->trm_unlock.event,data_ptr ->trm_unlock.trm_suspension_fn,data_ptr ->trm_unlock.winning_client_reason);
}


/**
 * @brief Schedules Next Activity in G2X TA State Machine and 
 *  	  updates related DSs
 * 
 * @param gas_id 
 */
void gpl1_g2x_ta_scheduler(gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t *data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri, pri_next;
  uint32 fn_next_act;
  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
  uint32 reschedule_fn;
  const uint8 BLK_BDRY_FN_OFFSET = 10;
#endif /* FEATURE_DUAL_SIM */
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

  GL1_G2X_TUNEAWAY_LOCK();
  for (pri = G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++)
  {
    if ((NO_ACT != data_ptr->table[pri].status) && (ONGOING != data_ptr->table[pri].status))
    {
      /* Sanity checks */
      if (NOT_A_FRAME_NUMBER == data_ptr->table[pri].frame_number)
      {
        MSG_GERAN_ERROR_1_G("G2X_TA SCHEDULER: Invalid FN for ongoing ACT: %d ", pri);
        continue;
      }

      /* Remove/reschedule stale activities */
      if( !IS_FRAME_NUM_LATER(data_ptr ->table[pri].frame_number, gl1_get_FN(gas_id)))
      {
        MSG_GERAN_HIGH_3_G("G2X_TA SCHEDULER: Stale activity detected: %d was scheduled at FN: %d current STATUS:%d", pri, data_ptr->table[pri].frame_number, data_ptr->table[pri].status);
        switch (pri)
        {
          case G2X_TA_PRI_TBF_TS_CHANGE:
          case G2X_TA_PRI_WID_SEARCH:
          case G2X_TA_PRI_SERV_BCCH:
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
          case G2X_TA_PRI_SERV_PCH:
            if (pri == G2X_TA_PRI_SERV_PCH)
            {
              MSG_GERAN_HIGH_2_G("G2X_TA SCHEDULER: Missed PCH c_fn %d activity_fn = %d", gl1_get_FN(gas_id), data_ptr->table[pri].frame_number);
              L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
            }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
            if (!gpl1_g2x_ta_cancel_table_field(pri, gas_id))
            {
              MSG_GERAN_ERROR_1_G("G2X_TA SCHEDULER: Can't cancel stale ACT: %d ", pri);
            }
            break;

          case G2X_TA_PRI_TRM_UNLOCK:
          {
            if ((TRM_UNLOCK_REQUIRED == data_ptr->trm_unlock.event) || (TRM_UNLOCK_BAND_INCOMPATIBLE == data_ptr->trm_unlock.event))
            {
              uint32 reschedule_fn = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), 16);
              data_ptr->table[pri].status       = WAITING;

              gpl1_g2x_ta_update_table_field(pri,
                                             data_ptr->table[pri].client_id,
                                             reschedule_fn,
                                             DEFAULT_DUR_TRM_UNLOCK,
                                             gas_id);

              MSG_GERAN_HIGH_3_G("G2X_TA SCHEDULER:  Activity:%d was rescheduled, now at %d  for %d frames", pri, data_ptr->table[pri].frame_number, data_ptr->table[pri].duration);

            }
            else
            {
              if (!gpl1_g2x_ta_cancel_table_field(pri, gas_id))
              {
                MSG_GERAN_ERROR_1_G("G2X_TA SCHEDULER: Can't cancel stale ACT: %d ", pri);
              }
            }
          }
            break;



          default:
            MSG_GERAN_ERROR_1_G("G2X_TA SCHEDULER: Unexpected pri: %d ", pri);
            break;
        } /*switch(pri) */

      }
      else /* Execution time still valid */
      {
        if (SUSPENDED == data_ptr->table[pri].status)
        {
          /* Resume activities that were suspended */
          data_ptr->table[pri].status       = WAITING;
          MSG_GERAN_HIGH_3_G("G2X_TA SCHEDULER:  Activity:%d was unsuspended, now at %d  for %d frames", pri, data_ptr->table[pri].frame_number, data_ptr->table[pri].duration);
        }
      }
    } /*if((NO_ACT != data_ptr ->table[pri].status) && (ONGOING != data_ptr ->table[pri].status)) */
  } /*for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++ )*/

  /* Decide which one is the next activity */
  pri_next = G2X_TA_PRI_RESERVED;
  fn_next_act = NOT_A_FRAME_NUMBER;
  pri = G2X_TA_PRI_MAX;

  while (pri > G2X_TA_PRI_RESERVED)
  {
    pri--;
    if (NOT_A_FRAME_NUMBER != data_ptr->table[pri].frame_number) /* Valid frame number to schedule */
    {
      if ((WAITING == data_ptr->table[pri].status) || (NEXT_ACT == data_ptr->table[pri].status))
      {
        MSG_GERAN_DEBUG_OPT_1_G("G2X_TA SCHEDULER0: pri%d considered", pri);
        if (NEXT_ACT == data_ptr->table[pri].status) data_ptr->table[pri].status = WAITING; /* Next act to be decided invalidate it for now*/

        if (NOT_A_FRAME_NUMBER == fn_next_act) /* Need to do this, for first valid act, because of IS_FRAME_NUM_LATER limitations*/
        {
          pri_next = pri;
          fn_next_act = data_ptr->table[pri].frame_number;
          MSG_GERAN_DEBUG_OPT_2_G("G2X_TA SCHEDULER1first: next_act %d at fn%d", pri_next, fn_next_act);
        }
        else if (IS_FRAME_NUM_LATER(fn_next_act, data_ptr->table[pri].frame_number))      /* Higher prio fn start is sooner for current prio*/
        {
          pri_next = pri;
          fn_next_act = data_ptr->table[pri].frame_number;
          MSG_GERAN_DEBUG_OPT_2_G("G2X_TA SCHEDULER1: next_act %d at fn%d", pri_next, fn_next_act);
        }
        else /* Check if higher prio activity collides with current   */
        {
          if (IS_FRAME_NUM_LATER_OR_EQUAL(ADD_FN(fn_next_act, data_ptr->table[pri_next].duration), data_ptr->table[pri].frame_number))
          {
            pri_next = pri;
            fn_next_act = data_ptr->table[pri].frame_number;
            MSG_GERAN_DEBUG_OPT_2_G("G2X_TA SCHEDULER2: next_act %d at fn%d", pri_next, fn_next_act);
          }
          else
          {
            MSG_GERAN_DEBUG_OPT_2_G("G2X_TA SCHEDULER4: next_act %d at fn%d", pri_next, fn_next_act);
          }
        }
      }
    }
  }
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
  if ((pri_next == G2X_TA_PRI_SERV_PCH) &&
      (gpl1_g2x_ta_are_act_colliding(G2X_TA_PRI_SERV_PCH, G2X_TA_PRI_TRM_UNLOCK, gas_id)) &&
      (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client != data_ptr->table[G2X_TA_PRI_SERV_PCH].client_id))
  {
    if ((data_ptr->trm_unlock.event == TRM_UNLOCK_REQUIRED) || (data_ptr->trm_unlock.event == TRM_UNLOCK_BAND_INCOMPATIBLE))
    {
      reschedule_fn = ADD_FN(ADD_FN(data_ptr->table[G2X_TA_PRI_SERV_PCH].frame_number, data_ptr->table[G2X_TA_PRI_SERV_PCH].duration), BLK_BDRY_FN_OFFSET);

      gpl1_g2x_ta_update_table_field(G2X_TA_PRI_TRM_UNLOCK,
                                     data_ptr->table[G2X_TA_PRI_TRM_UNLOCK].client_id,
                                     reschedule_fn,
                                     20,
                                     gas_id);
      MSG_GERAN_HIGH_1_G("G2X_TA SCHEDULER5: move ACT 5 after PCH activity, ACT 5 FN now is %d", data_ptr->table[G2X_TA_PRI_TRM_UNLOCK].frame_number);
    }
    else
    {
      MSG_GERAN_HIGH_0_G("G2X_TA SCHEDULER5: dont make SERV PCH as next act as it is not winning client");
      pri_next = G2X_TA_PRI_TRM_UNLOCK;
    }
  }
#endif /* FEATURE_DUAL_SIM */
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/
#ifdef FEATURE_DUAL_SIM
    /*De-prioritization of Ncell SI read if it is colliding with TDS/LTE page*/
    if((pri_next == G2X_TA_PRI_SERV_BCCH) && (gpl1_gprs_meas_mode_ptr->gprs_gpl1_do_serve_cell_bcch_trans == FALSE)
        && ((gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_TDSCDMA)
             ||(gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE)||(gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == GRM_LTE_SUB2))
        && (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event == TRM_UNLOCK_BY)
        && (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason == TRM_DEMOD_PAGE)
        && (gpl1_g2x_ta_are_act_colliding(G2X_TA_PRI_SERV_BCCH, G2X_TA_PRI_TRM_UNLOCK, gas_id)))
     {
        MSG_GERAN_HIGH_0_G("G2X_TA SCHEDULER6: Move TDS/LTE unlock_by activity as next act");
        pri_next = G2X_TA_PRI_TRM_UNLOCK;
     }
#endif /* FEATURE_DUAL_SIM */

  MSG_GERAN_HIGH_3_G("G2X_TA SCHEDULER3: next_act %d at fn%d cur_fn %d", pri_next, fn_next_act, gl1_get_FN(gas_id));
  if (G2X_TA_PRI_RESERVED == pri_next)
  {
    MSG_GERAN_ERROR_0_G("G2X_TA SCHEDULER: No activity was scheduled next");
    gpl1_g2x_ta_debug_table(gas_id);
    GL1_G2X_TUNEAWAY_UNLOCK();
    return;
  }

  data_ptr->table[pri_next].status = NEXT_ACT;
  gpl1_g2x_ta_debug_table(gas_id);
  GL1_G2X_TUNEAWAY_UNLOCK();

}



/* Decide which sequencer to start at that point might be changed later on to do the next act*/
gpl1_g2x_ta_priority_t gpl1_g2x_ta_fire_sequencer(gas_id_t gas_id)
{
 gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
 gpl1_g2x_ta_priority_t pri;
 boolean allowed_to_go=FALSE;

#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef   FEATURE_DUAL_SIM
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM */
#endif  /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

 for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++ )
  {
    if(NEXT_ACT == data_ptr ->table[pri].status)
    {
       /*PCHinXfer Requires G2X TA To Fire 1 frame before sched. FN to configure Handlers properly
         TRM Unlock By can be processed if fired 1 FN before*/
       if(SUB_FN( data_ptr ->table[pri].frame_number, NPL1_FN_OFFSET ) == GSTMR_GET_FN_GERAN(gas_id))
       {
         if((pri == G2X_TA_PRI_WID_SEARCH)&&(gpl1_g2x_prioritze_unlock_over_wid_search(gas_id)))
         {
           continue;
         }
         return pri;
       }
       else if(( SUB_FN( data_ptr ->table[pri].frame_number, ( NPL1_FN_OFFSET + TEMP_DELAY_OS )) == GSTMR_GET_FN_GERAN(gas_id)) &&
               (G2X_TA_PRI_TBF_TS_CHANGE != pri) &&
               (data_ptr ->table[pri].async_ccch_type != ASYNC_ACTIVITY_GBTA ))
       {
          /* might need to cancel ongoing AGA */
          /* Check for any clashes with:
           * or pending tbf assignment
           * or pending tbf release.
                    */
          allowed_to_go = gpl1_gprs_check_tbf_suspension( data_ptr ->table[pri].frame_number,
                                                          data_ptr ->table[pri].duration,
                                                                              FALSE, gas_id );
          MSG_GERAN_HIGH_2_G("G2X_TA FIRE: Five frames before at fn:%d allowed_to_go=%d",gl1_get_FN(gas_id),allowed_to_go);
          if( FALSE != allowed_to_go )
          {
            /* Stop calls to l1_sci_tick_receive on idle frames */
            gpl1_stop_sc_tick_trans(gas_id);
          }
          else
          {
            //allowed_to_go is false so we need to cancel or reschedule based on TRM reason
            if( (pri == G2X_TA_PRI_TRM_UNLOCK) && ((TRM_UNLOCK_REQUIRED == data_ptr ->trm_unlock.event) || 
				(TRM_UNLOCK_BAND_INCOMPATIBLE == data_ptr ->trm_unlock.event)))
            {
              data_ptr ->table[pri].frame_number = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), 16);
              data_ptr ->table[pri].status       = WAITING;
              MSG_GERAN_HIGH_3_G("G2X_TA FIRE: Activity:%d was rescheduled while aborting, now at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
              /*invoke the scheduler to select the NEXT_ACT*/
              gpl1_g2x_ta_scheduler(gas_id);
            }
            else
            {
              MSG_GERAN_HIGH_2_G("G2X_TA FIRE: Activity %d was cancelled 5 frames before start at fn%d",pri,gl1_get_FN(gas_id));
              (void)gpl1_g2x_ta_cancel_table_field( pri, gas_id);
            }
          }

        }
        else if(!IS_FRAME_NUM_LATER(data_ptr ->table[pri].frame_number, gl1_get_FN(gas_id)))
        {
          gpl1_g2x_ta_scheduler(gas_id);
        }
    }
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    else if((pri == G2X_TA_PRI_SERV_PCH) && (WAITING == data_ptr ->table[pri].status))
    {
        if(!IS_FRAME_NUM_LATER(data_ptr ->table[pri].frame_number, gl1_get_FN(gas_id)))
        {
           gpl1_g2x_ta_scheduler(gas_id);
        }
    }
    /*Debug crash to detect PCH/BCCH (gbta) activity overruning its reserved frames*/
    else if(( data_ptr ->table[pri].async_ccch_type == ASYNC_ACTIVITY_GBTA) && (ONGOING == data_ptr ->table[pri].status))
    {
        if(!IS_FRAME_NUM_LATER(ADD_FN(data_ptr ->table[pri].frame_number, ADD_FN(data_ptr ->table[pri].duration, 8)), gl1_get_FN(gas_id)))
        {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           /*crash here as this is unexpected*/
           ERR_GERAN_FATAL_0_G("PCH/BCCH Activity ONGOING even after its duration elapsed");
        }
    }
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/
  }
  return G2X_TA_PRI_RESERVED;
}

boolean gpl1_g2x_ta_set_ongoing(gpl1_g2x_ta_priority_t pri, gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  if(NEXT_ACT == data_ptr ->table[pri].status)
  {
    MSG_GERAN_HIGH_2_G("G2X_TA SET ONGOING: Activity %d set ONGOING at fn%d",pri,gl1_get_FN(gas_id));
    data_ptr ->table[pri].status = ONGOING;
    return TRUE;
  }
  else
  {
    MSG_GERAN_ERROR_0_G("G2X_TA SET ONGOING: activity set ongoing is not the next one");
    return FALSE;
  }

}

boolean gpl1_g2x_ta_reset_ongoing( gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;
  for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++ )
  {
    if(ONGOING == data_ptr ->table[pri].status)
    {
      (void)gpl1_g2x_ta_cancel_table_field( pri, gas_id);
    return TRUE;
    }
  }

  return FALSE;

}


boolean gpl1_g2x_ta_abort(gpl1_g2x_ta_abort_reason_t reason_abort, gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;

  MSG_GERAN_HIGH_1_G("G2X_TA ABORT: at fn%d",gl1_get_FN(gas_id));
  for( pri=G2X_TA_PRI_TBF_TS_CHANGE; pri < G2X_TA_PRI_MAX; pri++ )
     {
    if(ONGOING == data_ptr ->table[pri].status)
    {
      // While aborting we need to cancel the table or reschedule if trm reason is unlock_required (Ex: for W ACQ we need to reschedule)
      if( (pri == G2X_TA_PRI_TRM_UNLOCK) && ((TRM_UNLOCK_REQUIRED == data_ptr ->trm_unlock.event) 
	  	|| (TRM_UNLOCK_BAND_INCOMPATIBLE == data_ptr ->trm_unlock.event)) && (reason_abort == T3192_RUNNING))
      {
        uint32 reschedule_fn = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), 16);
		
        //first change the status to WAITING as ONGOING activity is not allowed to be updated
        data_ptr ->table[pri].status       = WAITING;
		
        //after calling gpl1_g2x_ta_abort function we call gpl1_g2x_ta_scheduler, so we don't need to call here after updating FN
        gpl1_g2x_ta_update_table_field( pri,
                                        data_ptr ->table[pri].client_id,
                                        reschedule_fn,
                                        DEFAULT_DUR_TRM_UNLOCK,
                                        gas_id);
		
        MSG_GERAN_HIGH_3_G("G2X_TA ABORT:  Activity:%d was rescheduled while aborting, now at %d  for %d frames",pri,data_ptr ->table[pri].frame_number,data_ptr ->table[pri].duration);
      }
      else
      {
      (void)gpl1_g2x_ta_cancel_table_field( pri, gas_id);
     }
     }
     else
     {
      (void)gpl1_g2x_ta_suspend_table_field(pri, gas_id);
    }
     }
  /* Do we need to reschedule now? or when? maybe it depends on the reason*/

     return TRUE;
  }


#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION  gl1_gbta_handle_abort_callback

DESCRIPTION

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
gl1_gbta_handle_abort_callback(client, other_g_client, event, unlock_by_sclk, other_g_reason, gas_id);
===========================================================================*/
static boolean gl1_gbta_handle_abort_callback(
  grm_client_enum_t               tbf_client_id,
  grm_client_enum_t               winning_client_id,
  trm_unlock_event_enum_t         event,
  uint32                          unlock_in_sclk,
  trm_reason_enum_t               winning_client_reason,
  gas_id_t                        gas_id
)
{
    grm_client_enum_t               other_idle_g_client =  GRM_NO_CLIENT;
    uint32  frame_duration;
    uint32 suspension_FN = L1_GPRS_INVALID_FN;
    boolean return_flag=FALSE;


   if ( !IS_GBTA_INACTIVE())   // optional , may be RACH events disallows.
   {

    switch (event)
    {
      case TRM_UNLOCK_BY:
         {
           unlock_in_sclk -= timetick_get_safe();
           frame_duration = MSECS_TO_FRAMES(timetick_cvt_from_sclk(unlock_in_sclk, T_MSEC));
           /* Dual GSTMR architecture needs additional overhead such that Trm request for PCH sub happens after
           * PTM sub prioirty is reduced to BG traffic
           */
             suspension_FN = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), (SUB_FN(frame_duration, 2) ) );
             trans[gas_id].gbta_client = winning_client_id;
             return_flag=TRUE;
          MSG_GERAN_HIGH_5_G("GBTA: TRM_UNLOCK_BY suspension_FN %d  Abort for TBF Client %d from TRM sclk:%d frame_dur %d , cur_fn %d"
                            ,suspension_FN
                            ,tbf_client_id
                            , unlock_in_sclk
                            ,frame_duration
                            ,gl1_get_FN(gas_id));
         }
        break;

      case TRM_UNLOCK_REQUIRED:
	  case TRM_UNLOCK_BAND_INCOMPATIBLE:
         {
            if( winning_client_reason == TRM_DEMOD_PAGE ) 
       {
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
         suspension_FN = GSTMR_GET_FN_GERAN(gas_id);
         trans[gas_id].gbta_client             = winning_client_id;
         return_flag = TRUE;

         MSG_GERAN_HIGH_5_G("GBTA: Dont IGNORE TRM_UNLOCK_REQUIRED/TRM_UNLOCK_BAND_INCOMPATIBLE for Paging reason %d w_c %d susp_FN %d gbta_curr_sus_fn %d event %d",
                             winning_client_reason,winning_client_id,suspension_FN, trans[gas_id].gbta_current_pch_suspension_FN,(int)event);
#endif /* FEATURE_DUAL_SIM */
#else
         MSG_GERAN_HIGH_2_G("GBTA: IGNORE TRM_UNLOCK_REQUIRED/TRM_UNLOCK_BAND_INCOMPATIBLE for Paging reason %d evt %d",winning_client_reason,(int)event);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
       }
       else
       {
        /* possible sub x is doing PT and sub y is doing a power scan so has no
           specific time for unlock but still wants it
           Therefore suspend in arbitrary 51 frames time */
         if( geran_get_sys_multi_sim_pref() == SYS_MODEM_DS_PREF_TRIPLE_STANDBY )
         {
             /* To be review and simplified, to check use case*/
                  
               grm_client_enum_t               other_client_id;
               grm_info_t                      *grm_info = NULL;
               grm_info_t                      *grm_info_1 = NULL;
               trm_reason_enum_t               trm_reason;
               gas_id_t                        l_active_tbf_gas_id;

               grm_info = grm_get_info_ptr_from_client(tbf_client_id, gas_id);
               other_client_id = gl1_get_client_id_of_other_idle_sub(gas_id);
               grm_info_1 = grm_get_info_ptr_from_client(other_client_id, gl1_map_client_id_to_gas_id( other_client_id ));
               l_active_tbf_gas_id = gl1_map_client_id_to_gas_id( tbf_client_id );


               if((other_client_id != GRM_NO_CLIENT) && (NULL != grm_info) && (NULL != grm_info_1))
               {
                trm_reason = grm_get_reason( other_client_id, gl1_map_client_id_to_gas_id( other_client_id ) );
                 if (((timetick_get_safe() + timetick_cvt_to_sclk(FRAMES_TO_MSECS(FRAMES_UNLOCK_REQ_FIRST_SCHEDULE), T_MSEC) + grm_info->req_n_notify_duration) > grm_info_1->when)
                          && (TRM_NUM_REASONS != trm_reason))
  {
                    suspension_FN = SUB_FN(MSECS_TO_FRAMES(timetick_cvt_from_sclk(grm_info_1->when - timetick_get_safe(), T_MSEC)), 2);
                    other_idle_g_client = gl1_get_client_id_of_other_idle_sub(l_active_tbf_gas_id);
                    gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client = other_idle_g_client;
                    return_flag=TRUE;
  }
                  }

               }
               MSG_GERAN_HIGH_3_G("GBTA: TRM_UNLOCK_REQUIRED/TRM_UNLOCK_BAND_INCOMPATIBLE Abort for  TBF Client %d suspension_FN %d event %d",tbf_client_id,suspension_FN,(int)event);
               }
           }
        break;

      default:
        MSG_GERAN_ERROR_1_G("GBTA: Unlock event %d not handled here", (int)event);
        break;
     }

   } /* IS_GBTA_ALLOWED() || IS_GBTA_ACTIVE()  */
   else
  {
        // gbta reason , but not allowed , dont book activity.
          MSG_GERAN_HIGH_0_G("GL1 GBTA not allowed , dont register activity");
  }


  if(!return_flag)
  {
    trans[gas_id].gbta_current_pch_suspension_FN = L1_GPRS_INVALID_FN;
    trans[gas_id].gbta_is_tbf_pch_suspension = FALSE;
  }
  else
  {

  /* Schedule GBTA */

          trans[gas_id].gbta_current_pch_suspension_FN = suspension_FN;
          trans[gas_id].gbta_is_tbf_pch_suspension = TRUE;
          trans[gas_id].gbta_suspension_reason =  winning_client_reason;

          MSG_GERAN_MED_3_G("GL1 GBTA REGISTER suspension_FN %d reason %d client %d ",
                               suspension_FN, winning_client_reason,trans[gas_id].gbta_client);
  }
  return return_flag;

}

#endif /* FEATURE_GPRS_GBTA */

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER

void gpl1_g2x_ta_pchxfer_activity_cleanup(gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];

  if (ONGOING == data_ptr ->table[G2X_TA_PRI_SERV_PCH].status)
  {
    MSG_GERAN_HIGH_2_G("PCHXFER Missed PCH due to Activity abort c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[G2X_TA_PRI_SERV_PCH].frame_number);
    L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
    gpl1_g2x_ta_deregister_seq(gas_id);
    l1_sc_abort_receive(gas_id);
    l1_sci_tick_receive(gas_id);
  }
  else if((ONGOING == data_ptr ->table[G2X_TA_PRI_SERV_BCCH].status) && 
          (data_ptr ->table[G2X_TA_PRI_SERV_BCCH].async_ccch_type == ASYNC_ACTIVITY_GBTA))
  {
    MSG_GERAN_HIGH_2_G("PCHXFER Missed BCCH due to Activity abort c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number);
    gpl1_g2x_ta_deregister_seq(gas_id);
    l1_sc_abort_receive(gas_id);
    l1_sci_tick_receive(gas_id);
  }
  gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_SERV_BCCH, gas_id);
  gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_SERV_PCH, gas_id);
}
  
#endif

boolean gpl1_g2x_prioritze_unlock_over_wid_search(gas_id_t gas_id)
{
 rex_timer_cnt_type time_left = 0;
 rm_timer_state_t rm_timer_state;
 gmac_timers_t *gmac_timers_struct = gmac_get_timers_ptr(gas_id);

 gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
 gpl1_g2x_ta_priority_t pri;

  if((WAITING == data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].status) && 
        (data_ptr ->table[G2X_TA_PRI_WID_SEARCH].frame_number ==data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].frame_number))
  {
    rm_timer_state = grlc_dl_t3192_status(gas_id );
    time_left = rex_get_timer( &gmac_timers_struct->timer_t3168 ) ;


    if((0 != time_left) || (TMR_ACTIVE == rm_timer_state))
    {
     MSG_GERAN_HIGH_2_G("Act5 can be prioritize over act2 as T3192/T3168 is running t3168_rem=%d, t3192_state=%d",
                            time_left,rm_timer_state);
data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].status = NEXT_ACT;
     return TRUE;
    }
  }

return FALSE;
}

