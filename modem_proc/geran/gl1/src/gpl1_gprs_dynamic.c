/*=========================================================================
L 1   T R A N S F E R   D Y N A M I C   A L L O C A T I O N  L O G I C A L

C H A N N E L S

GENERAL DESCRIPTION
   This module contains the procedures to handle the ISR level processing
   of the Dynamic allocation locaical channels

EXTERNALIZED FUNCTIONS
  L1_control_transfer

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_dynamic.c#3 $
$DateTime: 2020/02/10 05:49:45 $ $Author: pwbldsvc $

when       who      what, where, why 
--------   ---      ----------------------------------------------------------
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
15/04/19   rks      CR2304170 Checking WCDMA search state before aborting TBF suspend.
02/03/18   sp       CR2199093 Allow G2x Tuneaway for LTE Tx Client for CHannel Maintainance reason
09/06/17   dg       CR2058835 Remove unwanted clock boost in PTM by changing order of switch case statement
19/02/18   km       CR2191380 Increment suspension_timer by one if G2L srch is active in PTM to delay RF shutdown in case of TRM_UNLOCK_REQ
06/07/17   sp       CR2071477 Push loosing client's TRM request of PCHxFER to Task to avoid load on ISR.
27/10/17   rc       CR2130584 Skipping initialisation of tuneaway and ncell variables if main command is L1_IDLE_COMMAND
05/01/17   sn       CR2166914 Update  TRX_state after GTA gap closed
21/12/17   smd      CR1007478 Add one frame at GTA TBF suspending to finish G2W aborting.
21/07/16   sp       CR1039087: Align number of measurements to be done in GPRS be same in both SSIM and MSIM
01/08/16   og       CR1038345. UE does not get RAU accept after SRVCC.
14/03/16   sn       CR982754 offload TRM exchange operation from ISR to L1_TASK
08/09/17   sn       CR1061730 FR43082: TA during PS Procedures- GL1 changes
05/10/16   mn       CR1074007: Optimization to improve Page decode success rate for G2L tuneaway 
11/03/15   bg       CR983597 FR30575: SR-DSDS G+L with G to L tuneway - GL1 changes
19/09/17   rrp      CR2111352 Restart SCE after transitioning from SB to UL TBF
16/03/17   mn        CR2020307 FR40846:Packet transfer Error Recovery mechanism.
22/08/17   nm       CR2096887 Reset tuneaway and ncell data when leaving transfer
08/08/17   nm       CR2040229 Enable PCH read in 2 phase access for both Idle sub and data sub using GBTA
29/09/16   dg       CR1071675 Don't check for sc tick  while sending MVCS request to MCPM for G2L search
29/06/16   sn       CR1035075 L1 moving to IDLE during GTA gap inactivity timer expiry only if FULL RELEASE is pending.
8/05/2016  nm       CR1021140 Using IS FN LATER MACRO instead of using >= , <= operators for FN comparisons.
24/05/16   sn       CR1016717:Removing Excess F3 logging for packet transfer
30/03/16   dg       CR990072 Call MCPM API for BIMC boost before starting G2L search in packet transfer mode with AEQ ON
19/08/15   mko      CR893112 When idle sub is in page reorg mode & GBTA is active then don't start SC tick after time slot adjustment
22/02/16   bg       CR975060 Decoupled PCHxFER scheduling from PTM NCell Schedule
21/12/15   sk       CR938638 Reducing no of F3s
21/10/15   wei/snjv CR926595 AsDiv: After LTA in Xfer Set Preferred Config as well with Mode
30/10/15   nm       CR921073 Release trm for pchxfer acitivity while opening G2X_TA gap and Resv again when gap closes
21/10/14   ws       CR927509 Corrected TBF config error trap regression issue with CR918202
28/09/15   mko      CR914802 While moving from DTM to transfer mode set the GBTA status to ALLOWED before retaining the lock
07/10/15   ws       CR918202 Add Debug to trap illegal DL TBF Null pointer
28/09/15   snjv     CR913042 Re-Enable DR if disabled when leaving Idle mode, So no need to re-enable in Xfer Init
10/09/15   mk       CR904625 Open G2x TA gap based on winning client
16/09/15   cjl      CR908527 improve system procedures used for TRM access
18/09/15   cws      CR907895 Change unnecessary F3 to LOW 
02/09/15   zf       CR905869 Set trm_status to NULL after grm release
26/08/15   ws       CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
14/08/14    ws      CR889273 Fix Static declarations of variables which are not Dual data spaced
06/08/15   nm       CR885742 even for GTA gap check for sufficient suspension Frames
03/08/15   nm/ab    CR883804 Do not exchange client for DR-Transition until SUB has entered Transfer
03/08/15   cjl      CR883566 Fix logic error causing crash in PCH read during xfer
21/07/15   cjl      CR876903 Use system procedure types for TRM subreasons 
22/06/15   mko      CR857424 Open GTA gap only when last winning client is GSM client.
27/07/15   js       CR838001 Avoid suspending TBF when Idle sub is already inactive
03/06/14   mko      CR847610: Gl1 should set stop_sc_tick_trans to TRUE when suspension starts
25/06/15   sp       CR829915 Corner case where lower timeslot change was missed did not casue data call abort
21/07/15   cws      CR873163 fix asdiv/pchxfer compile warnings
21/07/15   cjl      CR876741 Replace TRM subreasons with system procedure types
16/07/15   sp       CR832567: Reset disable_rx_tx when BCCH Sequencer is aborted for TS change sequencer
16/07/15   sp       CR836700 Reset GBTA status to ALLOWED on aborting WID search activity at trm band denial
13/07/15   cja      CR868167 For dynamic EGPRS AEq reduce clock bump after block is complete
12/07/15   nm       CR861458 Fix the interworking issues between PCHXFER, DRDSDS, RX/TX split
09/07/15   aga      CR864745 Register G2X sequencer in case idle sub is G camped 
17/06/15   cjl      CR853555 Use dynamic connected mode reasons for CS/PS traffic
12/05/15   mko      CR831541: G21x Tuneaway Optimization - during T3168/T3190 & PDAN G21x TA allowed with NV control
20/05/15   zc       CR841303 Change asdiv mode for LTA and remove debug crash from 829145
29/06/15   smd      CR860078 Don't disable GBTA at TRANSFER_INIT
30/06/15   ws       CR852519 - Resolve GL1 enum conversion compile warnings
30/06/15   cjl      CR863267 Treat GL1_TRM_RETAINED_FOR_ACCESS as GL1_TRM_GRANTED in *_TRM_WAIT states
22/06/15   smd      CR857943 GL1 to release and request diversity chain at GTA and RACH tune away
17/06/15   ab       CR847473 : unify usage of gl1_map_gas_id_to_client_id for TX_ANT_DIV/DR_DSDS/RX_DIV/PCHXFER/AS_DIV
11/06/15   ab       CR852677 : GBTA-GL1 performing first PCH decode as GTA due to ISR/TASK parallel execution during TBF INIT.
02/06/15   ar	    CR847207 TRM_UNLOCK_BAND_INCOMPATIBLE not handled by gl1
05/06/15   ab       CR848697 : GBTA not allowed after fall back and UE does GTA instead 
04/06/15   ab       CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
22/05/15   smd      CR842012 Enable DR when entering tranfer mode 
26/05/15   akh      CR838149 Ncell pwr levels for idle sub are not updated during GBTA (G Tr HMSC + G Idle)
11/05/15   ab       CR 830983 Ncell of IDLE sub is not getting detected during data transfer
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
01/05/15   aga      CR831392 Add support for second WCDMA TRM client in case of G2X tune away
30/04/15   ap       CR811057 FR22272 - TRM Unification  - Logging Changes
21/04/15   shr      CR806878 GPRS DSDS Power Control and WWCoex Power Info do not log the Tx Power in use
03/13/15   nk       CR807528 Abort WID in TBF, if still ACTIVE, when deregistering the sequencer due to duration being elapsed
03/04/15   ab       CR809082 Do not schedule PWR_MON_EVENT for missed page ,
                    when Transfer Sub is Releasing TBF.
07/04/15   shr      CR808932: COEX support for DTM
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
16/03/15   zc       CR802323: Reset USF Detect Flag if FT handler hasn't been added in EXT_UTBF_NO_DATA
24/03/15   ap       CR811053:FR22272 - TRM Unification  - Access and Traffic attempts 
23/03/15   ap       CR811046:FR22272 - TRM Unification  - Removal of _HI reasons/UNLOCK_IMMEDIATELY 
01/03/15   nm       CR800906 Check if sched bcch/pch in transfer was successful
24/02/15   nm       CR798878 Do a DS abort if lowest tn change got missed when in FW load wait state inside G2X TA gap
04/03/15   br       CR801533:Call l1_sci_reset_state_machine function only if sc_tick_trans is already stopped
03/04/15   br       CR763774 GL1 logging improvement to ease debugging
17/11/14   dg       CR758006 IRAT improvements to ease debugging
16/01/15   aga      CR780157: Remove transition from ACTIVE_GAP to ACTIVE for data call abort use cases
16/02/15   mko      CR771860: Clear the TRM extension flag when TA is blocked by upperlayers
16/02/15   sp       CR772306: Update TRM of gbta grant for PBR to work
13/02/15   pa       CR756156:G2X RF device handling using TRM apis. 
29/12/14   sp/aga   CR774163: Update GFW to active_gap when opening GTA gap
12/02/15   ab       CR773792: Perform header decodes only during DL tbf release, reduces TX Blanking
11/02/15   ggu      CR785913 For G2X TA check, if condition1 is FALSE, no need to check condition2 as it will delete the RRBP
29/01/15   ggu      CR787789: GPL1 should enable the RX_TX when NCell activity is denied by TRM
16/01/15   ap       CR766617: Attempt to Unsuspend Transfer in GTA before aborting PS
02/02/15   aga      CR789501 Extend W ID search on PTM sub gap using W ID search starting point as reference
07/01/15   jj       CR778334 Don't send set tx band when hw tx off
16/12/14   aga      CR770827 In case of imminent TS change abort sequencer sooner and abort any ongoing receives
15/12/14   pg       CR630281 implementation of RxD support for data/MSRD
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
28/11/14   mko      CR764165 Stop SC tick when Wid search/BCCH sequencer starts running
12/05/14   sk       CR767085 - For allowing G2T TA when clashing with EPDAN with CR & FAI set to 1
11/12/14   ws       CR769901 Compile warning cleanup for Tabasco modem
28/11/14   sk       CR762376 Allow G2T TuneAway while T3168/T3192 are running
18/11/14   ab       CR758820 PCHXFER : Correct Featurization and Enable for FR21036 : Own in PCH in Transfer for SS.
07/11/14   pjr      CR751950 Ensure wtr hopping is inactive when state changes from transfer to idle.
03/11/14   mko      CR720074 G->1x TA support 
28/10/14   mko      CR746792 Stop TBF suspension if last unlock event is unlock_cancelled or unlock_immediate
30/10/14   pa       CR748275: In DRDSDS (Transfer Mode): allow IRAT if band registration is successful.   
29/10/14   ab       CR745429 - Featurization correction for PCHXFER for SS / CR merges.
20/10/14   nm       CR729402 check if gbta can be allowed during transfer mode by checking other sub' state
19/10/14   nm       CR729445 Check for gbta active before allowing own pch in transfer
17/10/14   pa       CR739626: In G2X TA, perform trm band registration on actual scheduling of BCCH. 
16/10/14   ab/pa    CR740336: Clear GBTA idle pwr mon event if pwr mons are not scheduled.
03/10/14   aga      CR732455 Changes required to tune away framework for TH 1.0 single sim build 
30/09/14   mko      CR728501 If TS adjustment is missed then send DS_ABORT & move to idle when TRM is granted back
29/09/14   aga      CR727164 Fix regression introduced in the timeslot change sequencer
29/09/14   smd      CR724540 Start SC tick when deregistering TS_CHANGE activity   
23/09/14   ab       CR727993 : GBTA - update Power mon state when callback are installed.
20/08/14   sk       CR708206 Ensure GBTA is not allowed in the frame where lowest time slot changes
12/09/14   aga      CR722387 Reduce aborting time by running sequncer an extra time
24/09/14   aga      CR728730 Remove the veto check from data sub activities that require TBF suspension 
23/09/14   js       CR723532:Deregister the sequencer for priority TRM UNLOCK and check for transfer mode.
22/09/14   aga      CR725474 Allow TRM_UMTS_SECONDARY client to suspend TBF 
19/09/14   pa       CR72726 Perform TRM band registration on scheduling IRAT/IFS measurement.
12/09/14   nm       CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
05/09/14   aga      CR717978 [G2X TA]Fix corner case in which state is overwritten right away after aborting
05/09/14   mko      CR716832:Start SC tick while aborting serving bcch in RXTX_NULL state
02/09/14   cah      CR717580 trm_client.cpp:5900   Assertion TRM_IS_PRIMARY_RF_DEVICE(activity_ptr->primary_device) failed
03/09/14   sp        CR706174 Allow 200ms for TBF reestablishment with DS Abort in GTA gaps.
03/09/14   sp        CR715190: Gl1 now pass PT sub client in Restart SC tick, So use same gas_id to restart SC Statemachine
22/08/14   mk        CR712000 Shorten abort period for serving BCCH normal TBF suspension to avoid missing TS change
                     and handle abort when suspension timer is running
23/08/14   sk       CR714221 Ensure monscan aborts properly while timeslot change happening on PT sub
07/08/14   am       CR705970 Abort SCE if sce tick is missed due to packet time slot re-configuration
29/08/14   cah      CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
23/08/14   pa/ggu   CR687349:DSDS: GL1 should guarentee at least one frame eariler before start check TBF suspension for G2W TA case
23/08/14   sp       CR692209 Use Hyperframe macro at conditional checks.
22/08/14   aga      CR713731 Updates to F3s and other recent changes
13/08/14   ggu      CR698506:Should update g2w TA flag before reschedule TBF suspension
13/08/14   ggu      CR687349:DSDS: GL1 should guarentee at least one frame eariler before start check TBF suspension for G2W TA case
08/08/14   nm       CR706305 - Fix the regression of CR 697096 by starting monscan again afer lowest tn change
07/08/14   aga      CR689162: Abort tune away gap mechanism triggered if lower TS change is imminent
22/07/14   sp       CR697096: Suspend Pwr meas inside a TBF Suspension gap/reconfig
25/07/14   aga      CR698326 - avoid scheduling monitors when TBF suspension is imminent
19/05/14   br       CR667202 Issuing ACQ command in G2L measurment gap in PTM.
20/07/14   mko      CR620365 Abort currently pending wcdma ded srch when starting TBF suspension
11/07/14   sjw      Standardise use of gl1_is_geran_tuneaway_allowed()
04/07/14   cs       CR688003 GRM functions moved to gpl1_grm_intf.c
08/07/14   aga      CR687195 Abort ongoing PTCCH in the Ncell BCCH sequencer and WID search sequencer
02/07/14   aga      CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
20/06/14   pjr      DATA+MMS feature - USF utilisation calculation
30/05/14   ws       CR657674 Data + MMS Phase 1
17/06/14   cs/ch    CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa    CR660831 Use new trm_status definitions specific to GL1
22/05/14   cah      Bolt 2.0: FEATURE_GERAN_DRDSDS: NCELL & IRAT measurement changes
14/05/14   nm       CR643032 Call G2W reschedule TBF correctly (only when TBF abort client is WCDMA)
08/05/14   sp       CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
08/05/14   br       CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
08/05/14   nk       CR660831: GSM Layer 1 compares wrong enum values for internal TRM_DENIAL status. Change TRM_DENIAL enum to TRM_NOT_GRANTED to aoid any confusion going forward.
01/05/14   ab       CR657605 : GBTA : Transfer Sub should not manage GBTA transition
24/04/14   sp       CR645327 - Crash at boundary checks on PDTCH Timeslots.
16/04/14   ws       CR 645067 - Increase Q6 clk speed when AEQ2A is enabled in EGPRS mode
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF (and others already mainlined)
04/04/14   sai      CR638388: Delay TBF re-establish in DSDS if G2W resel ongoing on other sub.
21/03/14   ab       CR634448 - GBTA - Error Handling : Restart Trans SCE when GBTA Resume
19/03/14   ws       CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
17/03/14   ws       CR614664 - Back out!
14/03/14   ab       CR 458061  DSDS : GL1 should abort TBF suspension when just initiated to pending PS activity
13/03/14   sai      CR614664: Call trm_retain_lock when enabling G2W TA
03/03/14   am       CR630494 Don't release FW in the same frame as FW setup
06/03/14   br        CR626780, RACE condition between TRANSFER G sub and IDLE G sub, issuing SCH in G2x gap.
2/17/14    ab       CR612460 Fix race condition for power monitor update
13/02/14   nm       CR593995 FW is not loaded in case g2w_tbf_abort_callback_received is reset from MAC_l1_rel
                            and at the same time trm grant call back is received inside G2W TA gap
21/01/14   mko      CR596574: Reset HW schedule events when TBF is suspended for G2W TA
23/01/14   cgc      CR606502 rework CR589523 add l1_sc_set_skip_g2x_pkt_txfr()
08/01/13   mko      CR587454: Don't suspend the TBF if blocked by upper layers
06/01/14   sk       CR590618 Ensure GBTA state set to allowed after TBF suspension has been aborted
26/12/13   sp       CR593796: Correct Tx Band config at TBF Resumption after a G2W TA gap
17/12/13   ap       CR566007: W is not getting the lock for Manual PLMN search when Sub2 is in data call
11/12/13   ap       CR586661 GPRS suspension during SI read after W2G reselection
06/12/13   sp       CR585243: Set Cnf_pending when releasing in inactivity timer.
17/12/13   ap       CR566007: W is not getting the lock for Manual PLMN search when Sub2 is in data call
13/12/13   ap       CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
10/12/13   ws       CR 587586 - Remove DUAL_SIM featurisation in API
26/11/13   ssh      CR581870: Force idle G sub to wake-up (enter non-drx) when W/G enters BG traffic
21/11/13   sp       CR579556 Handle TBF Release received when TBF is suspended for long time
                    only in W + G mode.
21/10/13   sai      CR563915: Facilitate G2W TA after TBF suspend for BCCH decode.
03/12/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
12/02/13   mko      CR572907 G2W_TA: Requesting for MCPM/ Hw Tx On while resuming the TBF in G2W scenarios
12/02/13   sai      CR 571606: Multispace G2W abort tracking variable,ensure G2W TA abort happens
                    only in W + G mode.
22/11/13   cja      CR455527 Remove featurisation from files in API directory
31/10/13   sai      CR567424: Enhance G2W TA suspension mechanism.
24/10/13   sai      CR565751: Fix reference FN for G2W TA suspension reschedule mechanism.
18/10/13   sp       CR546281 Delay RF Tx Configuration to Unsuspending TBF after GTA gap
17/10/13   js       CR549516 Abort pwr meas hanlder and then change gbta state.
15/10/13   mko      G2W_TA: Reschedule the TBF suspension if WL1 request for TRM when rx/tx is paused for TBF reconfig
10/10/13   mko      G2W_TA: if TBF susp is cancelled or aborted then Reschedule the TBF suspension for TRM_UNLOCK_REQUIRED events
07/10/13   js       CR554760 Ensure set Tx band and Tx cmd doesnt go in the same frame after G2W gap resumed
26/09/13   aga      CR551311 - G2W - missing feature flag causing compilation error
25/09/13   aga      CR547585 - G2W - avoid trm_gta_preemption_callback processing while suspended / suspend in ISR
25/09/13   aga      CR550212 - G2W GFW crash  - Power measurements are cancelled one frame before
24/09/13   gk       CR549082:Sync from 1.1 and 1.0 to 2.0
24/09/13   sk       CR549191 Set GBTA state to Allowed at the end of a GTA gap
01/08/13   ap       CR518483 518483 GTA TBF Suspension abort incorrectly handled
14/09/13   ap/aga   CR545423 - G2W Tune Away Feature
23/08/13   ab       CR533381 Schedule idle sub's power mons in the Transfer Sub idle Frames
14/08/13   sp       CR529521: Move req_notify_enh_data from static to local
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
20/08/13   sk       CR530970 Ensure GBTA works properly in DSDS config
21/06/13   ws       CR498325 - Fix gas ID with grlc_service_block_timers() and
                    grlc_dl_set_oper_win_size()
15/08/13   sk       CR524039 GBTA bringup changes
08/05/13   cgc      CR453758 add FEATURE_CM_G2L search functionality FR-3009
07/16/13   zc       CR470197 Replace memcpy with memscpy
16/07/13   mko      CR509649MODEM - rr_grr51_grr52_control.c:4770   EV_RACH_FOR_PS_FAILURE no response to rach
12/07/13   am       [Propagation] CR381497 Call gpl1_gprs_disable_rx_tx after GTA gap
03/07/13   cs       CR508216 Ensure TBF suspension does not occur if all subs INACTIVE
02/07/13   sk       CR507513 TRM advanced call back integration to Triton-TSTS
21/06/13   cs       Major Triton TSTS Syncup
11/06/13   am       CR422299 Cancel TBF suspension for GTA if GTA is blocked by upper layers
20/05/13   am       CR476324 TBF release and RRBP received in GTA gap
22/02/13   ab       CR 455513 -Reset monscan in progress when terminating pwr meas during TBF suspension gap.
20/03/13   ap       CR465389 DSDS W toG  BPLMN search happening in GTA
06/06/13   cs       TSTS Updates
15/04/13   am       CR473834 Avoid TRM release while FW loading is in progress
29/04/13   ws       CR481359 enable GTA support
06/03/12   pg       CR457891: Trigger Tx band change earlier during transfer reconfig
14/02/13   og       CR443965. dynamically set the HMSC flag to RF.
11/02/13   og       CR451559. More changes to distinguish the correct gas id.
26/02/13   ws       CR455036 - X+G DSDA: Crash in GL1 when testing Test mode B loopback mode
14/02/13   pjr      CR452272 added support to set multisim mode at activation time
16/02/13   pg       CR453105: Schedule any required Tx band change during PTM init
07/12/12   jv       CR438196: Using RF Task API gl1_hw_rf_idle_frame_processing() for performing idle frame processing.
22/11/12   cja      CR414098 Tidy-up F3 for MCPM builds
21/11/12   jj       CR323035 GSM temp comp not working in data call
17/08/12   ws       CR387294 - Avoid g2X cleanup and aborting TBF suspension on same frame.
                    Can potentially result in cleanup and sync Rx being sent with same offset to FW.
30/07/12   pa       CR383547: Don't schedule G2T if TBF starts during BLk21-24
04/07/12   ws       CR 375550 - Allow power measurements NCELL and SCELL to be sceduled
                    test modes.
03/07/12   pa       CR371273: Don't schedule G2T if TS reconfigure before idle frame.
18/05/12   ab       CR#: 352933 L1 - MAC is out of sync after reception of Down ass
                    during DL TBF release.
13/04/12   ws       CR332457 - updated schedulingto allow SCELL BCCH on FN 0,5,9 FNmod13.
                    8 NCELL and 3 SCELL measurements scheduled every 13 frames.
12/04/12   ws       CR332457 - schedule BCCH power monitors on frames 0,4,8 mod 13 in PTM
                    add check to stop power meas being scheduled in tbf suspend mode
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
13/03/12   ws       CR335057 eSACCH buffer flush and restore after inter-cell handovers
18/10/11   jj       CR310766 Support of GSM thermal Read in idle Frame
06/06/11   og       Increase AHB speed to 128 MHz in GERAN data (for CSFB Fusion)
                    CR286343
28/04/11   pg       Fix compiler warnings
17/03/11   cja      CR279518 Add single slot FTM power store for GPRS/EDGE
15/10/10   cja      CR259253 When aborting TBF suspension add frame delay to allow Rx activity to complete
15/09/10   og       CR255563. Clear RRBP messages that are going to expire when
                    disable rx tx is TRUE.
06/09/10   ab       ENH Add TEST MODE B Reconfig and add asymetric TS support.
21/06/10   og       Abort power measurements when changing the timebase during
                    PSHO. Resolves CR234932.
18/06/10   nt       CR:243318 ensure that SCE isn't stuck upon TBF suspend abort
06/05/10   dv       CR237642 - Add ASRB funtionality
09/04/10   ab       CR: 234805 - Data connection cannot be established after switching
                    between test modes without resetting the FFA
06/04/10   cja      Select of AEq by NV change GL1 to match MDSP
26/03/10   ab       CR: 218543 - [TFR-EU] Enhancement request to reduce TBF suspension
                    time for neighbor BCCH reading in GPRS/EGPRS only mode.
17/03/10   ws       Added featurisation to seperate Data mover code from FEATURE_GSM_MDSP_IR
14/01/10   ab       CR 220326 :Clash between TBF restart and the new assignment
                    getting connected.
02/12/09   ws       Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                    Improvements
11/06/09   og       CR184417: Esnure a test mode stop indication is sent in GPRS mode.
29/05/09   ws       FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
19/05/09   nt       CR178773: Added check when scheduling scell bcch that it occurs
                    after TBF resumption, if TBF is currently suspended.
24/04/09   ws       Backout CR 166945 since this causes Rx/TX to be tasked during
                    a timeslot change - CR 180318
20/05/09   og       CR175530: Ensure the burst and data memory buffers are static.
12/03/09   ws       CR 173134 Added Test mode A to L1 interface and calculate
                    RLA_P for test modes from PDCH
11/03/09   og       CR173804 - Ignore downlink mac mode for Rel 6.
28/01/09   og       Do not apply the Enhanced Ext TBF criterea in single block
                    TBF. Resolves CR170320.
18/12/08   ap       CR 166854 Pre-Sync HO Case Fails- MS does not send HO Access Bursts
17/12/08   ap       CR 166945 L1 does not send rel confirm when PTR and PUAN with FAI=1 arrives
02/12/08   cs       Added in new gl1_hw_clk_ctl.h include from gl1_hw.h
25/11/08   ap       CR 163610 PSHO UE schedules but does not send out PCCF after PSHO failure
24/11/08   og       FEATURE_GSM_GPRS_MSC33 support of high clock speed setting
                    in GPRS when the highest to lowest timeslot span is == 5.
09/03/07   og       FEATURE_GSM_GPRS_MSC33 GPL1 changes.
02/10/08   cs       Lint fixes
15/09/08   cs       CR130550 Add in the ability to request a higher/lower clock speed if req'd
                    under the FEATURE_GPRS_USES_HIGH_ARM9_CLOCK
20/08/08   og       CR155903 Open SRB loopback as late as possible - TC21.8 / 21.9 BER fix.
22/08/08   nt       Removed compiler warnings
15/08/08   ap       Corrections to previous check-in for RRBP issues
17/08/08   ap       Add FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
04/08/08   ap       CR 153325 Ensure the number of dynamic scheduled messages is equal
                    to the number of allocated slots. The change is specific to
                    Extended Uplink TBF because in all other cases dummy control blocks
                    will ensure this problem is never encountered.
21/04/08    ws      CR 144248 Enable and disable ESAIC in gl1_msg_set_egprs_mode() and
                    improve F3 for SAICS status
26/03/08   og       Worldphone 7501 target changes to ensure the correct clock
                    speed is used in GPRS.
07/03/08   ip       Added PSHO changes under FEATURE_GPRS_PS_HANDOVER
15/01/08   ws       Added suport for ESIAC logging and SAIC AFC in PTM
17/01/08   cs       Lint fix
13/12/07   agv      CR 131982 Fix for EGPRS DM causing sleep nacks.
24/09/07   og       Handle the error fatal case where EGPRS DL TBF is released
                    but queued nodes still exist.
06/06/07   ws       CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                    is called when we reselect to PBCCH cell stopping NC2 reporting
05/03/07   cs       Making CR112989 calculation consistent with sc updates
27/01/07   og       Implement UL ctrl msg deletion in L1.
23/01/07   og       Resolve RVCT2.1 compiler warnings, merge from the
                    gpl1_hummer.10.00.43.01 branch.
01/12/06   og       Rewind back the fn counter by one when dropping from DTM mode
                    to transfer mode and cs ts < TS2.
28/11/06   dv       Lint cleanup
14/11/06   cs       CR102226 Fix the Uplink Power Logging to use unslammed slot
30/10/06   og       DTM enhanced release code drop.
15/09/06   ws       Keep track of sched_tx_count variable when there is no data to send
                    Ensure dynamic_trbf_ptr->sched_tx_count only reflects Tx scheduled
14/08/06   ws       Allow dedicated to be timeslot slammed to lowest received tn
                    and also to place the CS timeslot on TN 2
01/08/06   ws       Don't Transmit dynamic PDCH if we have a NULL pointer - CR 97066
31/07/06   av       CR85810 NB AB TSC, and TA fix.
28/07/06   ws       For Test mode B and SRB loopback disable SCE
07/06/06   og       Workaround to invalid rx ts tasking. Resolves CR 94869.
12/04/06   og       Reset the downlink start time l1 transfer variable.
17/03/06   og       Take downlink assignment mac mode into account. This is
                    required by release 4 ETSI spec.
09/03/06   og       Extra initialisation of highest tx timeslot, resolves CR 89298.
01/03/06   ws       Abort RLC UL FIFO when leaving Packet transfer CR 88157
26/01/06   og       Neighbour cell acquisition changes to handle extended dynamic.
06/01/05   og       Fix to SRB loopback reconfiguration handling, resolves CR 85231.
15/11/05   og       Fix to USF granularity handling, resolves CR 82339, 82341.
03/11/05   nt       changed values in the array IS_PDCH_CHAN_ABORTED so that the index
                    cannot equal the return to prevent aborts in one thread.
02/11/05   nt       added function to reset static flags/data gpl1_dynamic_data_init()
28/10/05   nt       CR77078: Changed the frame count after releaseing from 10 to 9.
                    This will prevent any DL messages on the block after the final
                    RRBP from getting processed.
26/09/05   og       Allow USF tx tasking when RRBP multislot class violation is
                    detected.
21/09/05   og       Fix to the restructuring of the rlc deferred blocks handling
                    in PL1.
16/09/05   og       Send a sysinfo read failure when transitioning from transfer
                    back to idle.
06/09/05   og       Correctly handle Tra/Trb clashes.
05/09/05   og       Multislot class 12 support.
01/09/05   ws       Linted and code cleanup for FEATURE_GSM_DTM
26/08/05   og       Removal of DEBUG_TRANSFER compiler switch and associated
                    code.
23/08/05   ws       Fix for CR 72124 - Don't disable TX's unless we are connecting
                    an UL TBF.
22/08/05   og       Add the setting of mac mode to NPL1.
19/08/05   og       Removal of downlink block FIFO.
19/08/05   ws       Added AFC_TT_MODE setting for DTM Feature and cater for
                    wraparound TN for DTM cases
08/08/05   og       Restructuring PL1 before addition of DTM.
02/08/05   og       Merge change 223019 from the raven branch.
07/29/05   gfr      Support for NPL1 DTM interface changes
05/07/05   og       Change the FNmod13 error fatal calls to msg error.
04/07/05   og       Make use of the NPL1 camp timeslot buffering scheme.
23/06/05   og       Lint error/warnings removal.
23/06/05   og       Code changes to accomodate the uplink tbf data block count
                    log packet.
17/06/05   og       Ensure the test mode start ind msg is sent when l1 enters
                    test mode B.
17/06/05   og       Increment the ptcch dl count only when the camp timeslot goes
                    from high to low.
13/06/05   ws       Don't suspend if out we don't have a valid TA since we may
                    miss PPCTA message (CR 65736)
13/06/05   og       Increment fn count of ptcch dl handler in long tdma frames.
08/06/05   og       Addition of FNmod13 validity check.
06/06/05   ws       Corrected MSG_MED for DF TFI mismatch
02/06/05   og       Removal of header-header includes.
01/06/05   og       Add validity flag check to the metrics callback routines.
16/05/05   og       Additional mean bep reporting changes.
09/05/05   og       Addition of BLER sliding window reference to mean bep.
09/05/05   nt       Removed all conditions from gpl1_gprs_awaiting_release() function
                    for case L1_PARTIAL_RELEASE_UL and L1_FULL_RELEASE_UL.
03/05/05   ws       Removed test for UL RRBP type in gpl1_gprs_awaiting_release()
                    PCA_UL_FAI may not be the last UL RRBP to be sent on UL
27/04/05   og       Allow PBCCH reading in pkt transfer on timeslots lower than
                    the camp timeslot.
25/04/05   nt       Added function gpl1_gprs_awaiting_release() to check if release
                    is pending after RRBP has been transmitted. This is a functional
                    replacement for old 'if' statement, but has the additional checking
                    that the last RRBP that was transmitted was a PCA_UL_FAI_MSG before
                    allowing a normal release.
20/04/05   og       Addition of filtered bep params to the msg metrics log packet.
18/04/05   og       Initialise the instantiated usf dl tfi structure correctly.
15/04/05   ws       Added DEBUG_UL_PACCH to help debug UL PACCH Tx's
12/04/05   og       Change the filling of alloc type so it is aware of discontinuous
                    allocations.
11/04/05   og       Correctly handle discontinuous timeslot allocations.
06/04/05   og       Lint warning Removal.
05/04/05   ws       Check loopback TN for SRB, if loopback tn is not an
                    allocated timeslot then loopback lowest allocated TN
4/4/05     og       Supply RLC with cs1-4 data blocks only in gprs tbf. Distinguish
                    between gprs and egprs tfi.
21/3/05    og       Lint error removal.
18/03/05   og       Fix to PDAN vs TBF suspension clash handling.
17/3/05    og       Updates to srb loopback handling following integration
                    on raven target.
11/3/05    og       Changes to egprs dl tfi and IR handling.
7/03/05    og       Allow BCCH read attempts during packet transfer even when
                    TBF is suspended.
04/03/05   ws       If DL FIFO is full then Free allocated DSM items
04/03/05   ws       Sanity testing length of received data blocks before passing up to RLC
03/01/05   kf       Changes for wcdma id searches in transfer.
17/02/05   og       Merge to fix from saber 4x branch ( no associated CR )
                    introduced by change 166619.
17/02/05   og       Adjust the tbf release conf process to make it aware of long
                    TDMA frames resulting from MS timebase adjustement.
16/02/05   ws       Ensured dl_st is set to invalid FN when de-queing PDAS
11/02/05   ws       Added setting of active window size for DL IR for EGPRS
11/02/05   ws       Added Flag for DL release handler to ensure PDAS or PTR megs
                    are not de-queued erroneously
11/02/05   og       Remove raw bep resolution adjustement.
03/02/05   gfr      Removed call to gl1_msg_IR_config
03/02/05   ws       Test mode B. Don't discard data block rcvd if the FBI bit is set
02/02/05   og       Merge to fix from saber 4x branch for bug report 55687.
21/01/05   og       Re-instated num rx timeslots msg medium debug.
20/01/05   ws       Send Single_blk_sent when we tx PRR on UL PACCH
17/01/05   ws       Lint updates
14/01/05   ws       Don't Create a EPDAN_REQ if we are doing DL UNACK mode and either
                    FBI bit is set, a PCA is sent instead
14/01/05   ws       Updated TT and AFC interfaces for GMSK and 8PSK Modualtion types
13/01/05   ws       Don't check for padding removal for 2nd Payload and take account
                    of Padding (if present) when shifting out fbi_e bits and creating
                    dsm item
13/01/05   og       MS timebase adjustements confined to radio block gap.
11/10/05   og       Spliting of assignment/release processing functions
                    to allow more flexibility.
11/10/05   og       Added/modified ptcch functional interfaces.
11/12/04   ws       Updates for TEST mode B for EGPRS
06/12/04   ws       Updates for TEST mode B for GPRS
20/12/04   og       Update the relative timeslot of NPL1s ptcch dl handlers
                    in between dl burst reads.
13/12/04   og       Removal of single block flag from gprs change timeslot function.
10/12/04   og       Fix to reconfiguring of the ptcch handler when MS timebase
                    is adjusted.
03/12/04   og       Change the type of the mdsp beps src pointer to uint16.
01/12/04   ws       Subtract 1 when construct dsm item for MCS RLC data block
                    this is due to fbi_e being shifted out
01/12/04   ws       Turn EGPRS off when returning back to idle
19/11/04   ws       Test Mode B addition for sending ph_data_ind to RLC every block
10/11/04   og       Fix and optimisation to egprs raw bep -> scell bep mapping.
2/11/04    og       Addition of egprs srb test mode capability.
28/10/04   ws       Lint tidy up
28/10/04   ws       Added EGPRS logging of burst and message metrics
26/10/04   og       Simplification of code, re-arrangement of variables.
25/10/04   og       Re-arrangement of private/public functions.
25/10/04   og       Changed check for 4th burst in the dynamic metrics call back
                    to use the supplied burst number.
17/10/04   ws       Changed gpl1_isr_soft_buf to static
13/10/04   kf       Changes for time slot slamming in transfer.
08/10/04   ws       Removed code to stop adjusting time tracking if we are decoding
                    NCELL BCCH. Functionality now in NPL1,
30/09/04   ws       Corrected bug in scheduling gl1_msg_dch_tt_adjust(). Now called
                    every 104 frames.
28/09/04   ws       Modified TBF suspension checking to abort TBF suspension if PTCCH/U
                    needs to be scheduled.
27/09/04   ws       Added ptcch_bad_snr_valid flag - Fix for CR 48942
21/09/04   gfr      Do not apply TT adjustments when an ncell BCCH  may be active.
15/09/04   ws       Remvoed shifting of MCS header as it's now implemeted in NPL1
01/09/04   ws       Temp added shifting of MCS header to account for missing USF field
26/08/04   kf       changes to ncell sync's for ts slamming.
17/08/04   ws       Cleaned up EGPRS code and linted
05/08/04   ws       Added test for MODE_RUN around transmitting RRBP
30/07/04   ws       Removed creating dsm item for rlc header in pdtch data callback
                    to speed up processing in RLC of payload
28/07/04   ws       Removed setting USF list when no receives are in progress
                    since this has been fixed in Firmware
02/08/04   ws       Don't block suspension if the PTCCH/D is in the way unless it is our
                    last decode attempt, then schedule PTCCH/D channel.
13/07/04   ws       Don't transmit RRBP's if coding scheme is normal bursts and we don't
                    have a valid TA value
09/07/04   ws       Modified release mechanism for abnormal cases to ensure PCA and
                    and PDAN_w_FAI RRBP's are sent
24/06/04   ws       Let AGC use rxlev sample if payload type is RLC and our USF is detected
23/06/04   ws       Reduced margin in waiting for MAC to respond to last PACCH block
                    of UL from 4 to 3 frames from data callback
26/05/04   sv       Changed "l1_isrtsk_blk" structure parameters for better
                    code readability.
09/07/04   ws       Updates to MCS data cb handler
30/06/04   og       updates following integration using simulator.
23/06/04   og       Egprs Qual measurements handling.
22/06/04   og       Make sure FN variable is updated in gpl1_gprs_control_dynamic_tbf()
                    once Frame counter values are incremented when the lowest
                    downlink timeslot decreases.
05/05/04   kf       Changes for ts slamming.
27/04/04   rm       Replaced PTCCH/TA and RRBP functionalities with new function calls
18/04/04   ws       Increased margin in waiting for MAC to respond to last PACCH block
                    of UL from 2 to 4 frames from data callback
23/04/04   ws       Corrected Lint Error's and Warninggs
19/03/04   ws       Added support for bad snr detection on PTCCH/D decodes
09/03/04   ws       Updates to stop PACCH rx's after final PCA on ul has been txd and
                    to stop tx's after final PCA tx.
02/17/04   gfr      AFC cleanup
13/02/04   ws       Added PTCCH tn checking in reconfig and init sub states, also
                    added additional check for CS1 when passing up PACCH data to MAC
13/02/04   ws       moved abort_once_only var in release procedure to avoid transfer lockup
03/02/04   ws       Backed out new abort procedure when leaving transfer mode
23/01/04   kf       added code for cheking if reconfig is too close to idle frame.
22/01/04   ws       added test arround l1_sci_go() is called when we are in transfer mode
14/01/04   ws       Removed waiting for DL RRBP's when releasing and returning to idle
08/01/04   ws       Modified abort procedure to allow for sb abort driver change
05/12/03   ws       Extended period to receive PACCH during release phase to allow
                    PDA to be received and handled on same block as PCA Tx.
26/11/03   ws       Lint cleanup
06/11/03   gfr      Support for quad-band.
05/11/03   kf       added the use of SIZE_OF_RRBP_BUFF
26/09/03   kf       Fix for power control on pdch
16/09/03   ws       Corrected bug with rrbp confirmation buffer
16/09/03   ws       Modified TBF suspension to stop suspension if PTCCH/U or
                    PTCCH/D is required
10/09/03   ws       Ensured TBF suspend does not happen if T3168 is running
09/09/03   ws       Modified reconfiguring PTCCH channels only when a param
                    change is detected
08/09/03   kf       Added abort function for serv cell psi1 reads.
03/09/03   ws       Added confirmation of RRBP transmision
20/08/03   pjr      Copy gprs serving cell measurements to l1 measurements if
                    returning to 51 idle. Ensures out of date values are not
                    used if we have been in transfer for any length of time.
08/21/03   gw       Modified aborting of ncell BCCH receive when TBF suspend
                    is aborted.
15/08/03   kf       added rrbp deletion for ncell bcch reading.
13/08/03   kf       added MSG_GERAN_HIGH_0_G("already_suspended");
12/08/03   ws       Lint changes
08/08/03   ws       Added checking for psi1_repeat_period = zero
31/07/03   bk       Moved l1_sc_balist_gprs_set_sync from ISR to task context.
22/07/03   ws       Added aborting of TBF SUSPEND if we are releasing back to idle
14/07/03   gw       Replaced call to l1_sci_monsan_go() with l1_sci_go().
11/07/03   ws       Added check for incorrect states and deleted RRBP if they
                    have passed current FN
10/07/03   kf       modified trans.bcch_in_progress
08/07/03   gw       GRR/L1 SCE cleanup.
03/07/03   ws       Removed initializing release_pending in TRANSFER_INIT state.
                    Now done in l1_task
26/06/03   ws       wait until PCA is txd before releasing TBF
23/06/03   gfr      Compiler warning cleanup.
20/06/03   ag       Abort TBF suspension for reconfigure.
19/06/03   gw       Changes to starting/stopping SCE.
17/06/03   ws       Added PTCCH/D retry if CRC fails as per 05.10
10/06/03   ws       Fixed bug with RLC UL FIFO handling.
30/05/03   ag       Added acquire serving cell PSI 1.
29/05/03   npr      adjusted "TN changed serv" MSG level from ERROR to MED
28/05/03   ws       adjusted MSG level's from LOW to MED
23/05/03   ws       Updates to BSIC transfer feature
19/05/03   ws       Fixed rlc fifo bugs and initialised disable_rx_tx var
15/05/03   kf       removed idle = TRUE. when leaving transfer.
14/05/03   pjr      Modified handler of RRBP so if recieved in idle51 sent on
                    allocated TBF.
02/05/03   gw       Merged in power transfer changes from branch.
01/05/03   ag       Updates to ncell pbcch reading.
29/04/03   ws       Disabled tx for 1 block after reconfiguring tbf to fix test case 42_3_1_1_4
25/04/03   pjr      Modified the switch statement in the gpl1_gprs_measurements function.
24/04/03   ag       Only abort ncell bcch reading if handler is loaded
24/04/03   kf       modified sceduling of rxlev measurements. and temoved redundant function.
17/04/03   pjr      Modified filter to determine when failed decode should be indicated
16/04/03   bk       Added work around to suspend PSBIs in transfer mode.
15/04/03   pjr      Added work around for no rx but still have monitors.
                    This is a temporary fix awaiting mDSP change. All added code
                    is between DEBUG - PJR comments.
09/04/03   ws       General updates to eliminate mdsp error issues
03/04/03   pjr      Added filter to determine when failed decode likely to be due
                    to burst not being transmitted by network.
04/04/03   gw       Changes to reflect modifications in monscan module.
17/03/03   ws       Removed dl abort for sending RRBP that was received in idle
12/03/03   ws       Enabled USF Granularity code and added call to gl1_drx_require_TX()
07/03/03   ws       Modified clash detection of RRBP to iclude Tra check
07/03/03   pjr      Modified sending of release confirm to MAC
05/03/03   tlx      Added starting of T3166 by L1.
28/02/03   hv       Added TXA_PENDING_PACK where TXA_NACK was referenced.
28/02/03   ag       Delete RRBPs of type PDAN_MSG if conflict with a TBF suspension
27/02/03   pjr      Removed redundant prototype gpl1_gprs_rxlev_serv_cell
27/02/03   pjr      Added definition of gpl1_gprs_meas_mode to TBF suspension files.
27/02/03   pjr      Moved gpl1_gprs_send_l1_mac_usf_detected from scheduled
                    callback to metrics callback.
26/02/03   npr      Changed instances of INVALID to GPL1_INVALID
26/02/03   ws       Added FEATURE_GSM_GPRS_USF_GRAN and moved PTCCH back to 2
                    before actual frame
21/02/03   ws       Added code to detect tn clashes with RRBP
20/02/03   pjr      Added handling for RRBP received in IDLE but sent in transfer.
17/02/03   ag       Update for FEATURE_GPRS_TBF_SUSPEND
17/02/03   kf       Changes for running average in transfer.
14/02/03   pjr      Separated frame dependant neighbour cell measurements into
                    function.
13/02/03   ws       Added Downlink Power control
12/02/03   ws       Delay leaving transfer by additional 1 frame to allow PCA
                    to be txd correctly
07/02/03   ws       Renamed gpl1_log.h to gpl1_gprs_log.h in include path
07/02/03   ag       Updates to FEATURE_GPRS_TBF_SUSPEND
06/02/03   ws       Added MSG_LOW's for debuging multislot. Also set decode unsucc..
                    to MSG_LOW
05/02/03   ws       Added PCA_UL_FAI to RRBP ul msg count
05/02/03   ws       Modified scheduling of UL and DL channels for schedule change
23/01/03   ws       Changed PH_READY_TO_SEND sending to pass R10_Q62 test case
23/01/03   kf       Improvements to acquisition and NCELL BCCH reading
17/01/03   ws       Disabled Acquire's in TRANSFER and adjusted FN's
                    for 1_sci_mon_scan_next_period_trans()
16/01/03   ws       Added call to l1_sci_mon_scan_next_period_trans()
16/01/03   ws       Removed aborts for sb and fcb as per kf request
16/01/03   pjr      Added setting of gprs_serv_cell_meas.suspended_tbf
10/01/03   pjr      Added calls to gpl1_gprs_sc_trans_tick() and
                    l1_sci_tick_receive()
09/01/03   ws       Added pdtch arfcn to afc_init
19/12/02   ws       Removed Invalidating DL PTCCH schedule on every
                    TBF RECONFIGURE
19/12/02   pjr      Removed call to gl1_msg_abort_power_meas.
16/12/02   pjr      Removed calls to gpl1_gprs_sc_trans_tick() and
                    l1_sci_tick_receive()
06/12/02   pjr      Removed serving cell measurement debug.
05/12/02   ws       Added gl1_msg_reset to INIT sub state
05/12/02   DLH      Modified params. in gpl1_gprs_calculate_arfcns call.
04/12/02   ws       Modified burst_metrics log packet and move memset() in sched cb
02/02/02   ws       Changed sending of rlc_ph_ready_to_send to when number of
                    of rlc blocks in ul fifo = number of timeslots
02/12/02   ws       Stopped set_signal_info() being called neadlessly, Added
                    Additional handling for scheduling sb burst during packet
                    transfer, Addded abort handling of sb and fcb handlers
27/11/02   ws       Changed afc_tt_init call for dual mode builds
25/11/02   pjr      Minor modification to debug message
25/11/02   ws       Added GPRS agc logging
22/11/02   ag       General improvements to NCELL BCCH reading under switch
                    FEATURE_GPRS_TBF_SUSPEND
15/11/02   ws       removed flag for starting T3192 on the first instance of a
                    PDAN with FAI or PCA with FAI set
12/11/02   ag       Added PL1 GPRS TBF suspension for NCELL BCCH reading code
                    under switch FEATURE_GPRS_TBF_SUSPEND.
11/11/02   ws       Updated agc to ignore UL data blocks if usf is not detected
11/11/02   pjr      Changed temporary serving cell measurement timeslot monitor
                    from fixed value to lowest timeslot.
08/11/02   ws       Reverted removal of gl1_msg_terminate_int_meas().
07/11/02   he       Removed gl1_msg_terminate_int_meas().
06/11/02   ws       Updated RELEASE_CONFIRM messages to add tbf type
05/11/02   ws       Added logging support for message and burst metrics
04/11/02   pjr      Merge Neighbour cell measurements under feature switch
01/11/02   pjr      Added serving cell measurements under feature switch
10/21/02   he       Replaced FN by gl1_get_FN(), tx_signal_strength by TXLEV.
10/09/02   ws       Removed = in comparison for rx multislot failure
10/07/02   DLH      Modified gpl1_gprs_calculate_arfcns call
27/09/02   ws       If moving back to 51 mfrm only abort on frame 2
18/09/02   ws       Added support of PCA_DL_WITH_FAI to RRBP handler
18/09/02   ws       Added PCA_DL_WITH_FAI to trigger starting of timer T3192
13/09/02   pjr      Modified confirm_pending process to use enum
11/09/02   pjr      Added confirm_pending process and changed to PDAN_WITH_FAI
6/09/02    ws       Added mod MULTIFRAME when setting received FN for the downlink
3/09/02    ws       Added support for T3192 timer start
8/08/02    ws       Corrected Lint error's and Warning's
1/08/02    ws       Added AFC/TT code to dynamic allocation

1/08/02    ws       Added code to wait for rrbp's to be tx'd if ul and/or dl
                    tbf is being released.
                    Added switch around all MSG loging to stop for target build
                    Changed FEATURE_GPRS to FEATURE_GSM_GPRS_L1.
                    Added ta_valid==TRUE test for scheduling tx's
                    Corrected PTCCH error for frame 38
29/07/02   ws       Added changes to rlc/pl1 interface by adding defer buffer for
                    storing get inforamtion that cannot be sent to RLC when ul fifo
                    is busy. corrected frequency hoping errors for PTCCH
19/07/02   DLH      Added calls to freq. hopping function
18/07/02   ws       Fixed bug that stops downlink tn's being allocated on non zero
                    timeslots
15/07/02   ws       Modified handling parameters for continuous TA, various changes
                    to sub states and starting time for transfer tbf reallocation
02/07/02   ws       Added radio block gap if tn changed between tbf reconfiguration
                    added L1_MAC_1ST_RADIO_BLOCK_SENT.
31/06/02   ws       Added L1_TRANSFER_RECONFIG sub_state
27/05/02   ws       Added Dowlink allocation code for Concurrent TBF
29/11/01   ws       Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"
#include "memory.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_msg_g.h"
#include "gl1_msg_pdch.h"
#include "gl1_sys_algo_gprs.h"
#include "l1_isr.h"
#include "gpl1_gprs_isr.h"
#include "grlculfifo.h"
#include "grlcdlfifo.h"
#include "l1_task.h" // for setting the bsic for access burst tx'x
#include "geran_dual_sim.h"

#include "gpl1_grm_intf.h"

#include <stringl/stringl.h>

#include "geran_msgs.h"
#include "grlcl1.h"
#include "gmacl1.h"
#include "gmactimers.h"
#include "l1_utils.h" /* for gl1_get_FN() */
#include "l1_sc_int.h" /* SCE L1 API's */

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gpl1_gprs_log.h"

#include "geran_eng_mode_info.h"

#include "l1_drx.h"
#include "gl1_msgi.h" /* for gl1_msg_abort_pbcch() */
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_ptcch.h"
#include "gpl1_gprs_test.h"



#include "l1_sc.h"


#include "l1_sc_int.h"

#ifdef FEATURE_GSM_WTR_HOP
#include "gl1_msg_wtr_hop.h"
#endif

#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_utils.h"

#ifdef FEATURE_GPRS_PS_HANDOVER
#include "gpl1_gprs_task.h"
#endif

#ifdef FEATURE_G2X_TUNEAWAY 
#include "gpl1_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "slpc.h"
#include "gl1_hw_sleep_ctl.h"
#include "gl1_hwi.h"
#define G2W_TUNEAWAY_DSDS_TIMEOUT 71 // 325 ms
#define G2W_TUNEAWAY_TSTS_TIMEOUT 87 // 400 ms /* G2W TSTS Back-to-back */
#endif /* FEATURE_G2X_TUNEAWAY  */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/*========================== #defines =====================================*/

/* #define GPL1_DM_DEBUG */

/* Used to show what BSNs are being received and transmitted */
//#define DEBUG_SHOW_PL1_BSN

/* Used to define the number of blocks in a PDCH 52 multiframe
 * for use in look up arrays.
 */
#define  GPL1_NUM_PDCH_MF_BLKS  (12U)

#ifdef DEBUG_SHOW_PL1_BSN
#error code not present
#endif

extern  gpl1_gprs_meas_mode_T gpl1_gprs_meas_mode[];

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
extern gpl1_gprs_tbf_abort_cb_store_data_t gpl1_gprs_tbf_abort_cb_store_data[NUM_GERAN_DATA_SPACES];
extern trans_T  trans[NUM_GERAN_DATA_SPACES];
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

#ifdef FEATURE_G2X_TUNEAWAY
#define TBF_MIN_DURATION_MS 40
static byte suspension_timer[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(SUSPENSION_FRAME_DELAY) };
static byte fw_rf_setting_up[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(RF_FW_SETUP_FRAME_DELAY) };
#endif /* FEATURE_G2X_TUNEAWAY */

#ifdef  FEATURE_GSM_GPRS_MSC33
#define  GPL1_TS_GAP_REQUIRES_HSPEED_FW_CLK  (5U)
#endif

#define RETRY_FRAMES_BEFORE_TIMEOUT 10 

/*========================== Local types/enums ============================*/

/*========================== Local function declarations ==================*/
typedef enum
{
   NO_PENDING_ABORT_TBF_SUSPEND = 0,
   ABORT_TBF_SUSPEND_THIS_FRAME,
   ABORT_TBF_SUSPEND_NEXT_FRAME
} abort_tbf_suspend_status_type;

/* This function is used to manage TBF rx/tx activity suspension due to
 * BCCH/PBCCH reading or configuration changes which lead to the lowest
 * downlink timeslot changing.
 */
#ifdef FEATURE_G2X_TUNEAWAY
#define GUARD_FRAME_TBF_RECONF_OR_REL           1
static void gpl1_gprs_unsuspend_tbf(transfer_data_T *transfer_data_ptr, uint32 tbf_suspend_gta_start_frame , gas_id_t gas_id);
boolean gpl1_g2x_ta_check_rrbp(boolean tdscdma_client, transfer_data_T *transfer_data_ptr,gpl1_g2x_ta_priority_t pri, gas_id_t gas_id);
#endif /* FEATURE_G2X_TUNEAWAY */

#ifdef FEATURE_GPRS_PS_HANDOVER
static void gpl1_gprs_psho_AB_on_pacch( frequency_information_T*  freq_info_ptr,
                                        uint8                     ul_timeslot,
                                        uint8                     access_burst_type,
                                        int16                     txlev, gas_id_t gas_id
                                        );

/* @@OG: A new callback function */
static void  gpl1_access_burst_tx_cb( gl1_msg_tx_scheduled_data_type  msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
                                      uint8                           num_msg_txd,
                                      gas_id_t gas_id  );

/* Associated discarded callback function */
static void  gpl1_access_burst_discarded_tx_cb( gl1_msg_tx_discarded_data_type  msgs_discarded[ GL1_DEFS_MAX_UL_MSGS ],
                                                uint8                           num_msg_discarded,
                                                gas_id_t                        gas_id );

/* This is used to finish off setting up the UE to rx/tx on the new cell
 * Once the timing updates have been completed.
 */
static void  gpl1_xfer_complete_psho( gas_id_t gas_id );

#endif /* FEATURE_GPRS_PS_HANDOVER */

extern void L1_sync_to_current ( l1_sc_bs_T *current_cell, gas_id_t gas_id );


extern gpl1_g2x_ta_data_t  gpl1_g2x_ta_data[NUM_GERAN_DATA_SPACES];

boolean gpl1_g2x_ta_block_bond( uint32 start_fn, tbf_suspend_T *suspension_time_ptr,  gas_id_t gas_id );
void gpl1_g2x_ta_seq_no_act(gas_id_t gas_id);
void gpl1_g2x_ta_seq_low_ts_chage(gas_id_t gas_id);
void gpl1_g2x_ta_seq_wid_search(gas_id_t gas_id);
void gpl1_g2x_ta_seq_serv_bcch(gas_id_t gas_id);
boolean gpl1_g2x_ta_register_seq(gpl1_g2x_ta_priority_t pri, gas_id_t gas_id);
boolean gpl1_g2x_ta_deregister_seq(gas_id_t gas_id);
void (*gpl1_g2x_ta_seq[NUM_GERAN_DATA_SPACES])(gas_id_t)={INITIAL_VALUE(&gpl1_g2x_ta_seq_no_act)};
gpl1_g2x_ta_disable_rx_tx_state g2x_ta_seq_trm_state[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(G2X_TA_DISABLE_RXTX_NULL)};
boolean gpl1_g2x_ta_common_checks_before_GAP(gpl1_g2x_ta_priority_t pri, gas_id_t gas_id);
gpl1_g2x_ta_data_sub_act_info_t tbf_susp;
boolean trigger_ts_change[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_G2X_TUNEAWAY
extern void gl1_hw_tcxo_inact( boolean deep_sleep, gas_id_t gas_id );
extern void gl1_hw_tbf_G2W_tune_back_RF(gas_id_t gas_id);
void gpl1_g2x_ta_seq_trm_unlock_for_G(gas_id_t gas_id);
void gpl1_g2x_ta_seq_trm_unlock_for_X(gas_id_t gas_id);
#endif /* FEATURE_G2X_TUNEAWAY */

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
void gpl1_g2x_ta_seq_serv_ccch(gas_id_t gas_id);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern void gl1_hw_switch_antenna_tx_div (gas_id_t gas_id);
#endif  
/*======================== Local Data =====================================*/

/* Used to store the MS timebase timeslot to be
 * in the radio block gap of inactivity.
 */
static uint8  delayed_current_timeslot[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(GPL1_INVALID) };

/* Provides client with the PDCH block number for a particular
 * fn mod52 value.
 * Used for scheduling TBF suspension when reading BCCH/PBCCH
 * on the neighbour/serving cell during packet transfer.
 */
static const uint8  GPL1_FN_MOD52_TO_BLK[ MULTIFRAME_52 ] =
{
    0,0,0,0,   1,1,1,1,    2,2,2,2,    2,

    3,3,3,3,   4,4,4,4,    5,5,5,5,    5,

    6,6,6,6,   7,7,7,7,    8,8,8,8,    8,

    9,9,9,9, 10,10,10,10, 11,11,11,11, 11
};

/* Provides client with the integer division result for a
 * particular fn mod52 value.
 * Used for scheduling TBF suspension when reading BCCH/PBCCH
 * on the neighbour/serving cell during packet transfer.
 */
static const uint8  GPL1_FN_MOD52_DIV_13[ MULTIFRAME_52 ] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

/* Provides client with the integer division result
 * for the block number supplied.
 * Used for scheduling BCCH/PBCCH reads in packet
 * transfer modes.
 * Accepted value range 0 - 11.
 */
static const uint8  GPL1_MF52_BLK_NUM_DIV_3[ GPL1_NUM_PDCH_MF_BLKS ] =
{
 /* 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 */
    0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3 , 3
};

/*Make sure that the frame number passed in can never be that returned
therefore

IS_PDCH_CHAN_ABORTED[3] = 3 should be
IS_PDCH_CHAN_ABORTED[3] = 4

IS_PDCH_CHAN_ABORTED[7] = 7 should be
IS_PDCH_CHAN_ABORTED[7] = 8,

IS_PDCH_CHAN_ABORTED[12] = 12 should be
IS_PDCH_CHAN_ABORTED[12] = 0

This reason is so that the abort can never happen on the first call to this check,
it will always be at least on the next frame preventing the call to abort the PDTCH
and the call to  gl1_msg_set_egprs_mode(FALSE); in the same thread.
*/
static const uint8 IS_PDCH_CHAN_ABORTED[13] =
{
  /* current FNmod13 : 0, 1, 2, 3 */
                       3, 3, 3, 4,
  /* current FNmod13 : 4, 5, 6, 7 */
                       7, 7, 7, 8,
  /* current FNmod13 : 8, 9, 10, 11,12 */
                      12, 12, 12, 12, 0
};

/* Controls the release process allowing the setup of variable for only
the first pass through the releasing loop */
static boolean abort_once_only[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(TRUE) };

static tbf_suspend_T  tbf_suspend[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE_S(0) };

static uint32         ncell_bcch_fn[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };
static boolean        check_for_rrbp[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
static boolean        tbf_suspend_ncell_bcch[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };


#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM
static boolean  decrement_fn_by_one[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
#endif
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

boolean  stop_sc_tick_trans[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

/* test mode B local storage */
gpl1_gprs_tm_b_store_t gpl1_gprs_tm_b_store[NUM_GERAN_DATA_SPACES];

/* Added for AB update; we need to store */
/* the tsc so a RRBP does not overwrite. */
uint8   gl1_active_pdch_tsc[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };

static boolean  gl1_open_srb_loopback[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

/* Setup Npl1s rx\tx structures containing function addresses */
static const  gl1_defs_rx_pkt_cb_type  dynamic_rx_cb =
{
     gpl1_gprs_rx_dyn_data_cb,
     gpl1_gprs_rx_dyn_metrics_cb,
    (gl1_defs_rx_pkt_discarded_callback)  NULL,
    (gl1_defs_rx_pkt_log_callback)        NULL
};

static gl1_msg_tx_pkt_cb_type   dynamic_tx_cb =
{
    (gl1_msg_scheduled_data_callback)  gpl1_gprs_tx_dyn_rrbp_sched_cb,
    (gl1_msg_discarded_data_callback)  gpl1_gprs_tx_dyn_rrbp_disc_cb,
      (gl1_msg_usf_utilisation_callback) gpl1_gprs_tx_dyn_rrbp_usf_util_cb
};

/*========================= Local function Prototypes =====================*/

#ifdef FEATURE_GPRS_PS_HANDOVER

/* @@OG: A new callback function */
static void  gpl1_access_burst_tx_cb( gl1_msg_tx_scheduled_data_type  msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
                                      uint8                           num_msg_txd,
                                      gas_id_t gas_id  )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* @@OG: Check if something was transmitted.
   * TODO: Make the checks more strict and compare payloads.
   */
  if( 0 != num_msg_txd )
  {

    transfer_data_T*  transfer_data_ptr = &l1_transfer_data[gas_id];

    /* Print an F3 message since this is an important event */
    MSG_GERAN_HIGH_1_G("Access burst txd fn %d",
             GSTMR_GET_FN_GERAN( gas_id ) );

    /* Work out which sub state and sub state to set to */
    switch( transfer_data_ptr->current_alloc_type )
    {
    case  L1_DOWNLINK_ALLOC:

      l1_tskisr_blk->sub_state = L1_DOWNLINK_1;
      break;

    case  L1_DYNAMIC_ALLOC:

      l1_tskisr_blk->sub_state = L1_DYNAMIC_1;
      break;

    case  L1_DYNAMIC_DOWN_ALLOC:

      l1_tskisr_blk->sub_state = L1_DYNAMIC_1;
      break;

    default:
      MSG_GERAN_ERROR_1_G("Pl1: Incorrect allocation type %d",transfer_data_ptr->current_alloc_type);
      break;
    } /* End of swtich on current allocation type */

    /* @@OG: Reconfigure the correct transmission callbacks here */
    gl1_msg_cfg_ul_pacch_pdtch( transfer_data_ptr->frequency_info_ptr->tsc,  /* repeat current freq_info_ptr->tsc config here */
                                TS0,                                         /* Access Burst TSC */
                                l1_idle_data->campedon_cell_BSIC,             /* This is right, swapped bsic by this point */
                                &dynamic_tx_cb,gas_id );

    GPL1_ENABLE_TX( gas_id );
  }
  else
  {
    MSG_GERAN_HIGH_0_G("0 Access bursts txed");
  }
}

static void  gpl1_access_burst_discarded_tx_cb( gl1_msg_tx_discarded_data_type  msgs_discarded[ GL1_DEFS_MAX_UL_MSGS ],
                                                uint8                           num_msg_discarded,
                                                gas_id_t                        gas_id )
{

  if( 0 != num_msg_discarded )
  {
    MSG_GERAN_HIGH_0_G("0 Access bursts txed");
  }

  /* @@OG: Reconfigure the tx callbacks to normal NB */

}

static void  gpl1_xfer_complete_psho( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
    /* Target Cell Pointer is not used anywhere in the code. Commenting out for now to fix compiler warning
    const cell_T*  target_cell_ptr =
       l1_sc_balist_get_cell_by_arfcn( gpl1_psho_data.target_cell_details.arfcn );*/

    /* Update Idle ARFCN and BSIC to that of target cell*/
    l1_idle_data->campedon_cell_ARFCN = gpl1_psho_data.target_cell_details.arfcn;
    l1_idle_data->campedon_cell_BSIC  = gpl1_psho_data.target_cell_details.bsic;

    /* Initialise the band and the pdm value */
    gl1_msg_acq_afc_init( gpl1_psho_data.target_cell_details.arfcn,gas_id );

    /* @@OG: Need to prime the agc also */
    sys_algo_agc_init( &l1_idle_data->campedon_cell_agc,
                       (dBx16_T) gpl1_psho_data.target_cell_details.rx_power );

    sys_algo_agc_init(&l1_transfer_data[gas_id].packet_transfer_agc,
                       (dBx16_T) gpl1_psho_data.target_cell_details.rx_power );
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    sys_algo_agc_init(&l1_transfer_data[gas_id].packet_transfer_div_agc,
                       (dBx16_T) gpl1_psho_data.target_cell_details.rx_power );
#endif
    /* @@OG: What is going to happen to the CCH tt and afc ??
     * PDCH tt is initialised in the PSHO reconfig state handling.
     */
}
#endif

/*========================= External function Prototypes =====================*/

#ifdef FEATURE_GPRS_PS_HANDOVER
extern boolean get_l1_psho_revert_command( gas_id_t gas_id );
extern void set_l1_psho_revert_command(boolean l1_psho_revert_command,gas_id_t gas_id);
/* @@OG: This is being used as global data at the moment, need to consider ways to
 * get rid of it.
 */
psho_data_T  gpl1_psho_data;

/* Flag to indicate if need to return
 * to current cell during PSHO. This is set
 * when MAC_L1_PSHO_STATUS indicates failure.
 */

#endif

void  gl1_egprs_open_srb_loopback( gas_id_t gas_id )
{
  /* Set this flag */
  gl1_open_srb_loopback[gas_id] = TRUE;

}

/*===========================================================================

FUNCTION  gpl1_gprs_control_dynamic_tbf

DESCRIPTION
  This function controls transfer mode for fixed allocation and concurrent
  downlink allocation together with the associated RRBP operation.

DEPENDENCIES
  pointer to dedicated mode data


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/


boolean gpl1_gprs_control_dynamic_tbf(
                                     dynamic_tbf_information_T  *dynamic_tbf_ptr,
                                     frequency_information_T    *freq_info_ptr,
                                     l1_mode_command_T    mode, gas_id_t gas_id
                                     )
{

  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
  l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);

 uint8    pacch_pdtch_ul_blk_start_cnt = GPL1_INVALID;
 uint8    pacch_pdtch_dl_blk_start_cnt = GPL1_INVALID;
  uint8           num_rrbp_msg_txd             = 0;
  static boolean  abort_pending[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
  gl1_defs_coding_type txed_coding_scheme[GL1_DEFS_MAX_ASSIGNED_UL_TS] = {0};

  static downlink_timeslot_info_t  downlink_ts_info[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE_S(GPL1_INVALID_TN) };


  /* This flag is used to allow setting of the tbf mode
   * to NPL1 as late as possible before the reception/transmission
   * of radio blocks commences.
   */
  static boolean  l1_transfer_init[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };


#ifdef FEATURE_GPRS_PS_HANDOVER

  static boolean  l1_perform_ps_hover[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
  static boolean  l1_psho_initial_transition_delay[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };
#endif

  /* EDGE DM clock switch Off flag. */
  boolean abort_tbf = TRUE;

  boolean  idle               = FALSE;
#ifdef FEATURE_GPRS_GBTA
  boolean gbta_go_idle = FALSE;
#endif /* FEATURE_GPRS_GBTA */

  uint32   FN   = gl1_get_FN( gas_id );
  uint8    i;

  /* Pointer used to read the packet switched variables from
   * the transfer/dtm common store.
   */
  const gpl1_ps_store_T*  gpl1_store_ptr = NULL;

  grlc_ul_fifo_access_block_t   rlc_config_params;

  transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  gpl1_g2x_ta_priority_t pri_fire=G2X_TA_PRI_RESERVED;

  /*** ERROR Check allocation and UL,DL config pointers*/
  switch (transfer_data_ptr->current_alloc_type)
  {
    case L1_DOWNLINK_ALLOC:
    case L1_DYNAMIC_DOWN_ALLOC:
    {
      /* Must have a downlink Allocated check pointers are not NULL*/
      if ( ( (transfer_data_ptr->dl_tbf_ptr == NULL) && (transfer_data_ptr->pending_dl_tbf_ptr == NULL) ) || 
           ( (transfer_data_ptr->dynamic_tbf_ptr == NULL ) && (transfer_data_ptr->pending_dynamic_tbf_ptr == NULL) )|| 
           ( (transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr == NULL ) && (transfer_data_ptr->pending_dl_tbf_ptr == NULL) &&
             (transfer_data_ptr->pending_dynamic_tbf_ptr == NULL) ) )
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_FATAL("GL1: DL TBF PTR NULL",0,0,0);
      }
     break;
    }

    case L1_DYNAMIC_ALLOC:
    {
      if ( (transfer_data_ptr->dynamic_tbf_ptr == NULL ) && (transfer_data_ptr->pending_dynamic_tbf_ptr == NULL) )
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_FATAL("GL1: UL TBF PTR NULL",0,0,0);
      }
      break;
    }

    default:
    {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_FATAL("GL1: bad alloc type %d",transfer_data_ptr->current_alloc_type,0,0 );
    }
  }

  /* Added for AB update. We need to store as */
  /* a RRBP must not trash the tsc of the NB. */
  gl1_active_pdch_tsc[gas_id] = (uint8)freq_info_ptr->tsc;

  /* @@Warning: This is temporary validity check to track the issue of
   * FNmod13 counter being out of step with the absolute frame counter.
   * This check should be removed once the issue is resolved.
   */
  if( frame_counters[gas_id].FNmod13 != (FN % 13) )
  {
    /* Report an error since transfer would be running on the
     * wrong radio block boundaries.
     */
    MSG_GERAN_ERROR_3_G(" FNmod13 %u out stepped abs FN %u modulo 13. FNmod52 %u",
              frame_counters[gas_id].FNmod13,
              FN,
              frame_counters[gas_id].FNmod52 );
  }
  /* Check if we need to reconfigure the TBF */
  if( l1_tskisr_blk->sub_state == L1_TRANSFER_RECONFIG )
  {
    /* Setup the usf allocation array */
    gl1_defs_rx_pkt_alloc_type  alloc_type[ GL1_DEFS_MAX_ASSIGNED_DL_TS ]=
    {
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 }
    };

    uint8                     dl_timeslot_alloc = 0;

    /* Take a copy of mac_mode */
    uint8                     mac_mode = dynamic_tbf_ptr->mac_mode;

    /* If we are connecting a downlink assignment then its mac_mode
     * must be ignored (Rel 6).
     */

    /* Setup egprs incremental redundancy.
     * This function updates the rlc mode.
     */
    gpl1_reconfig_egprs_incr_redundancy( transfer_data_ptr, FN ,gas_id);


#ifdef FEATURE_GPRS_PS_HANDOVER

    /* Re-instate the correct NB tx cb functions */
    dynamic_tx_cb.scheduled_callback = gpl1_gprs_tx_dyn_rrbp_sched_cb;
    dynamic_tx_cb.discarded_callback = gpl1_gprs_tx_dyn_rrbp_disc_cb;
    dynamic_tx_cb.usf_utilisation_callback = gpl1_gprs_tx_dyn_rrbp_usf_util_cb;
#endif

    /* configure NPL1s tx handler */
    gl1_msg_cfg_ul_pacch_pdtch( freq_info_ptr->tsc,
                                TS0, /* Access Burst TSC */
                                l1_idle_data->campedon_cell_BSIC,
                                &dynamic_tx_cb,gas_id );

    if( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
    {
        dl_timeslot_alloc = dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;
    }

    /* Work out the timeslot information resulting
     * from the received assignment message.
     */
    gpl1_calc_dyn_dl_ts_info( dl_timeslot_alloc,                  /* In  */
                              &( dynamic_tbf_ptr->usf_value[0] ), /* In  */
                              &( downlink_ts_info[gas_id] ),              /* Out */
                              &( alloc_type[0] ), gas_id );               /* Out */

    delayed_current_timeslot[gas_id] = downlink_ts_info[gas_id].lowest_dl_timeslot;

    if( downlink_ts_info[gas_id].lowest_dl_timeslot == l1_get_current_timeslot(gas_id) )
    {
        /* This is called regardless of whether the current timeslot has changed
         * or not. This is a requirement from the surround cell engine.
         */
        gpl1_gprs_change_timeslot( downlink_ts_info[gas_id].lowest_dl_timeslot, gas_id );

        /* Reset delayed current timeslot to prevent it from
         * getting used incase the radio block gap was
         * incorrectly set.
         */
        delayed_current_timeslot[gas_id] = GPL1_INVALID;
    }
    else
    {
        /* Print out warning, for debug purposes */
        MSG_GERAN_HIGH_3_G("delayed camping on ts %u FNmod13 %u  STmod13 %u",
                 delayed_current_timeslot[gas_id],
                 frame_counters[gas_id].FNmod13,
                 ADD_MOD( frame_counters[gas_id].FNmod13,
                          SUB_FN( transfer_data_ptr->starting_time, FN ),
                          13 )
                );

        /* Inform future ptcch rx\tx operation of
         * the change in the lowest downlink timeslot.
         */
        gpl1_gprs_ptcch_reconfig(gas_id );
    }

    /* Check the supplied mac mode */
    switch( mac_mode )
    {
    case  MAC_L1_MAC_MODE_DYNAMIC:

      /* Set the mac mode into l1 transfer data */
     l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_DYNAMIC;

      /* Inform NPL1 */
      gl1_msg_set_medium_access_mode( GL1_DEFS_DYNAMIC_ALLOC,gas_id );

      MSG_GERAN_MED_0_G("mac mode DYNAMIC");
      break;

    case  MAC_L1_MAC_MODE_EXTENDED_DYNAMIC:

      /* Set the mac mode into l1 transfer data */
     l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_EXTENDED_DYNAMIC;

      /* Inform NPL1 */
      gl1_msg_set_medium_access_mode( GL1_DEFS_EXT_DYNAMIC_ALLOC,gas_id );

      MSG_GERAN_MED_0_G("mac mode EXT DYNAMIC");
      break;

    default:
      /* The mac mode is invalid/unavailable, don't
       * do anything.
       */
      break;
    }

    /* Configure NPL1s rx handler */
    gl1_msg_cfg_dl_pacch_pdtch( &( alloc_type[ 0 ] ),
                                downlink_ts_info[gas_id].num_dl_pdch,
                                freq_info_ptr->tsc,
                                dynamic_tbf_ptr->usf_granularity,
                                &dynamic_rx_cb,
                                gas_id);

#ifdef PL1_FW_SIM
    /* Configure uplink and/or downlink tbf */
    MSG_GERAN_MED_0_G("PL1: DYNAMIC reconfig");
#endif

    /* Update the FN variable after the gpl1_gprs_change_timeslot( ) is called
     * incase lowest DL timeslot decreased and caused the frame counters
     * of the MS to be incremented by one. This ensures FN is the
     * most up-to-date value.
     */
    FN = gl1_get_FN( gas_id );

    gpl1_gprs_config_ta_param( FALSE,
                               freq_info_ptr->tsc,
                               downlink_ts_info[gas_id].lowest_dl_timeslot, gas_id );

    /* Configure AGC for dl power control ( if present) */
    gpl1_gprs_config_agc(gas_id);

    /* Set AGC valid on first RX block */
    gpl1_gprs_update_first_rx(TRUE, gas_id);

#if defined (FEATURE_G2X_TUNEAWAY)
    if (gpl1_gprs_get_gta_config(gas_id))
    {
      /*initiate RF Tx Band change on TBF resumption*/
      MSG_GERAN_MED_1_G("Delay Tx Band Change:%d",gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]));
    }
    else
#endif
    {
      /* initiate RF Tx band change - only gets actioned if band has actually changed */
      gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]), gas_id);
    }

    /************************************************************************************
     **
     ** If we are configuring or reconfiguring an Uplink TBF then stop the first TX
     ** from being scheduled since we have not yet received the USF on the new assignment
     **
     ** enable_tx is re-enabled after the first PDCH Tx attempt
     **
     ** Fix for CR 72124
     ***********************************************************************************/
    if( (!transfer_data_ptr->starting_time_elapsed) &&
        ( (transfer_data_ptr->tbf_est_type == UPLINK_TBF_ESTABLISHED) ||
          (transfer_data_ptr->tbf_est_type == UPLINK_DOWNLINK_TBF_ESTABLISHED)
        ) )
    {
       GPL1_DISABLE_TX( gas_id );
    }

    /* Work out if sch can go ahead or recongig to close to idle frame */
    switch (frame_counters[gas_id].FNmod52)
    {
    case 20:
    case 21:
    case 22:
    case 46:
    case 47:
    case 48:
        trans[gas_id].reconfig_two_close = TRUE;
        MSG_GERAN_MED_0_G("RECONFIG_TOO_CLOSE");
        break;

    case 25:
    case 51:
        gl1_hw_rf_idle_frame_processing(gas_id);
         break;

    default:
        trans[gas_id].reconfig_two_close = FALSE;
        break;
    }

    /* Work out which sub state and sub state to set to */
    switch( transfer_data_ptr->current_alloc_type )
    {
    case  L1_DOWNLINK_ALLOC:

      if (transfer_data_ptr->dl_tbf_ptr == &(transfer_data_ptr->dl_tbf_1))
      {
        l1_tskisr_blk->sub_state = L1_DOWNLINK_1;
      } else
      {
        l1_tskisr_blk->sub_state = L1_DOWNLINK_2;
      }
      /* only single tbf so set sub sub state to no dl tbf */
      l1_tskisr_blk->sub_sub_state= L1_NO_DL;
      break;

    case  L1_DYNAMIC_ALLOC:

      if (transfer_data_ptr->dynamic_tbf_ptr == &(transfer_data_ptr->dynamic_tbf_1))
      {
        l1_tskisr_blk->sub_state = L1_DYNAMIC_1;
      } else
      {
        l1_tskisr_blk->sub_state = L1_DYNAMIC_2;
      }

      /* only single tbf so set sub sub state to no dl tbf */
      l1_tskisr_blk->sub_sub_state= L1_NO_DL;
      break;

    case  L1_DYNAMIC_DOWN_ALLOC:

      /* setup sub states */
      if (transfer_data_ptr->dynamic_tbf_ptr == &(transfer_data_ptr->dynamic_tbf_1))
      {
        l1_tskisr_blk->sub_state = L1_DYNAMIC_1;
      } else
      {
        l1_tskisr_blk->sub_state = L1_DYNAMIC_2;
      }

      /* setup sub sub states */
      if (transfer_data_ptr->dl_tbf_ptr == &(transfer_data_ptr->dl_tbf_1))
      {
        l1_tskisr_blk->sub_sub_state = L1_DL_1;
      } else
      {
        l1_tskisr_blk->sub_sub_state = L1_DL_2;
      }
      break;

    default:
      MSG_GERAN_ERROR_1_G("Pl1: Incorrect allocation type %d",transfer_data_ptr->current_alloc_type);
      break;
    }

      switch( transfer_data_ptr->dynamic_tbf_ptr->test_mode )
      {
      case  TEST_MODE_SRB:
      {
        /* Warning: Do we have to wait for receiving a block
         * before tasking DSP for transmission ?
         */
        MSG_GERAN_MED_2_G("SRB ON FNmod13 %u tbf_1.srb %d",
                frame_counters[gas_id].FNmod13,l1_transfer_data[gas_id].dynamic_tbf_1.srb_mode);

        /* Inform RR so that no BCCH/PBCCH read requests
         * are sent, which require tbf suspension thus
         * disrupting loopback procedures.
         */
        switch( transfer_data_ptr->l1_test_mode )
        {
        case  L1_TEST_MODE_OFF:
          gpl1_l1_test_mode_start_ind( gas_id);

          /* Stop FCCH and SCH activity for test modes */
          stop_sc_tick_trans[gas_id] = TRUE;

          /* Ensure that the M-bit value( see 44014, sec.5.5.4 ) is set.
           * 0 - MS loopback blockon UL using either GMSK or 8PSK dependant
           *     on the received modulation.
           * 1 - MS loops back block on the UL using GMSK only.
           */
          gl1_srb_set_ul_mod_mode(transfer_data_ptr->dynamic_tbf_ptr->srb_mode,gas_id);


        /*lint -fallthrough*/
        case  L1_TEST_MODE_SRB_OFF:

          gl1_msg_pdch_set_loopback( TRUE,gas_id );
          break;

        case  L1_TEST_MODE_A:
          MSG_GERAN_ERROR_0_G( "cant start SRB while in test mode A");
          break;

        case  L1_TEST_MODE_B:
          MSG_GERAN_ERROR_0_G( "cant start SRB while in test mode B");

        /*lint -fallthrough */
        case  L1_TEST_MODE_SRB_ON:
          /* current l1 test mode is srb ON
           * in which case no action is required.
           */
          MSG_GERAN_MED_1_G("No change to Test mode FNmod13 %u",frame_counters[gas_id].FNmod13);
          break;

        default:
          /* Should never happen, report an error */
          MSG_GERAN_ERROR_1_G( "Invalid l1 test mode %u",
                     transfer_data_ptr->l1_test_mode );
          break;
        }

        transfer_data_ptr->l1_test_mode = L1_TEST_MODE_SRB_ON;

      } /* End of test mode srb case */
        break;

      case  TEST_MODE_A:
      {
        switch( transfer_data_ptr->l1_test_mode )
        {
        case  L1_TEST_MODE_OFF:

          /* Send the test mode start ind to RR */
          gpl1_l1_test_mode_start_ind(gas_id );

          /* Stop FCCH and SCH activity for test modes */
          stop_sc_tick_trans[gas_id] = TRUE;

          MSG_GERAN_MED_1_G("Test mode A FNmod13 %u",frame_counters[gas_id].FNmod13);

          /* Set the layer 1 test mode */
          transfer_data_ptr->l1_test_mode = L1_TEST_MODE_A;

          break;

        case  L1_TEST_MODE_SRB_OFF:
        case  L1_TEST_MODE_SRB_ON:

          MSG_GERAN_ERROR_0_G( "cant start test mode A while in SRB");
          break;

        case  L1_TEST_MODE_A:
          /* L1 is already in test mode A */

          MSG_GERAN_MED_1_G("Already in in test mode A",frame_counters[gas_id].FNmod13);

          break;

        case  L1_TEST_MODE_B:
          /* L1 is already in test mode A, so can't test mode B */

          MSG_GERAN_MED_1_G("cant start test mode B while in test mode A",frame_counters[gas_id].FNmod13);

          break;


        default:
          /* Invalid test mode, printout an error msg */
          MSG_GERAN_ERROR_1_G("invalid l1 test mode %u",
                    transfer_data_ptr->l1_test_mode );
          break;
        }  /* End of switch on l1 test mode */

      }
      break;


      case  TEST_MODE_B:
      {
        switch( transfer_data_ptr->l1_test_mode )
        {
        case  L1_TEST_MODE_OFF:

          /* Send the test mode start ind to RR */
          gpl1_l1_test_mode_start_ind(gas_id );

          /* Stop FCCH and SCH activity for test modes */
          stop_sc_tick_trans[gas_id] = TRUE;

          MSG_GERAN_MED_1_G("Test mode B FNmod13 %u",frame_counters[gas_id].FNmod13);

          /* Set the layer 1 test mode */
          transfer_data_ptr->l1_test_mode = L1_TEST_MODE_B;
          transfer_data_ptr->l1_test_mode_tn =
             transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn;

          /* Setup timeslots to loopback for Test mode B */
          gpl1_gprs_setup_test_B(gas_id);
          break;

        case  L1_TEST_MODE_SRB_OFF:
        case  L1_TEST_MODE_SRB_ON:

          MSG_GERAN_ERROR_0_G( "cant start test mode B while in SRB");
          break;

        case  L1_TEST_MODE_A:
           MSG_GERAN_ERROR_0_G( "cant start test mode A while in SRB");
           break;

        case  L1_TEST_MODE_B:
          /* L1 is already in test mode B */
          gpl1_gprs_tm_b_store[gas_id].ul_nts  = transfer_data_ptr->dynamic_tbf_ptr->nts;
          MSG_GERAN_MED_2_G("Test mode B reconfigure FNmod13 %u nts %d",
                  frame_counters[gas_id].FNmod13,transfer_data_ptr->dynamic_tbf_ptr->nts);

          /* Setup timeslots to loopback for Test mode B */
          gpl1_gprs_setup_test_B(gas_id);
          break;

        default:
          /* Invalid test mode, printout an error msg */
          MSG_GERAN_ERROR_1_G("invalid l1 test mode %u",
                    transfer_data_ptr->l1_test_mode );
          break;
        }  /* End of switch on l1 test mode */

      }
        break;

      case  TEST_MODE_OFF:
      {
        /* Check current l1 test mode */
        switch( transfer_data_ptr->l1_test_mode )
        {
        case  L1_TEST_MODE_OFF:
          MSG_GERAN_MED_1_G("Test mode OFF FNmod13 %u",frame_counters[gas_id].FNmod13);
          break;

        case  L1_TEST_MODE_SRB_ON:
            gl1_srb_set_ul_mod_mode(transfer_data_ptr->dynamic_tbf_ptr->srb_mode,gas_id);
            gl1_msg_pdch_set_loopback( FALSE,gas_id );

        /*lint -fallthrough*/
        case  L1_TEST_MODE_SRB_OFF:
        case  L1_TEST_MODE_A:
        case  L1_TEST_MODE_B:
          /* Inform RR so that bcch/pbcch reading
           * procedures during pkt xfer are resumed.
           */
            gpl1_l1_test_mode_stop_ind(gas_id );

            /* Resume FCCH and SCH activity for test modes */
            stop_sc_tick_trans[gas_id] = FALSE;

            MSG_GERAN_MED_1_G("Test mode OFF FNmod13 %u",frame_counters[gas_id].FNmod13);

            transfer_data_ptr->l1_test_mode = L1_TEST_MODE_OFF;
            transfer_data_ptr->dynamic_tbf_ptr->test_mode = TEST_MODE_OFF;

            /* Required as during reconfiguration as both structures are used */
           l1_transfer_data[gas_id].dynamic_tbf_1.test_mode = TEST_MODE_OFF;
           l1_transfer_data[gas_id].dynamic_tbf_2.test_mode = TEST_MODE_OFF;
          break;

        default:
          /* Should never happen, report an error */
          MSG_GERAN_ERROR_1_G( "Invalid l1 test mode %u",
                     transfer_data_ptr->l1_test_mode);
          break;
        } /* End of switch on l1 test mode */

      } /* End of default case */
        break;

      default:
        /* Ignore invalid value */
        MSG_GERAN_MED_1_G("No change to Test mode FNmod13 %u",frame_counters[gas_id].FNmod13);

        break;
      } /* End of switch on dynamic ptr test mode */

  } /* End of if L1_TRANSFER_RECONFIG... */
  else if( l1_tskisr_blk->sub_state ==L1_TRANSFER_INIT )
  {
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    /* Setup the usf allocation array */
    gl1_defs_rx_pkt_alloc_type  alloc_type[ GL1_DEFS_MAX_ASSIGNED_DL_TS ]=
    {
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 }
    };

    uint8                       dl_timeslot_alloc  = 0;

    uint8    tx_timeslot = GL1_DEFS_SLOTS_IN_FRAME;
    boolean *tx_ts_ptr   = &( gprs_serv_cell_meas_ptr->tx_in_progress
                              [ (GL1_DEFS_SLOTS_IN_FRAME - 1) ] );

    /* Take a copy of mac_mode */
    uint8  mac_mode = dynamic_tbf_ptr->mac_mode;

    /* If we are connecting a downlink assignment then its mac_mode
     * must be ignored (Rel 6).
    */


    /* Set the transfer init flag to TRUE for the purpose
     * of setting the tbf mode on the coming radio block
     * boundary.
     */
    l1_transfer_init[gas_id] = TRUE;


   /* Kick off SCE */

    if (l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
    {
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
      if (!(trans[gas_id].tuneaway_and_ncell_data_initialized) ||
          !l1_sc_get_monscan_active(gas_id))
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
      {
      l1_sci_go(gas_id);
      }
    } else
    {
      MSG_GERAN_HIGH_1_G("WRONG_SURROUND_MODE = %d",l1_sc_globals_ptr->mode);
    }

    gl1_hw_set_ftm_power_store_init(TRUE,gas_id);

    /* Test mode B Initialisation */

    gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status = 0;
    gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status = 0;
    gpl1_gprs_tm_b_store[gas_id].ul_tn[0] = 0xff;
    gpl1_gprs_tm_b_store[gas_id].ul_tn[1] = 0xff;

#ifdef FEATURE_GSM_GPRS_MSC33
    gpl1_gprs_tm_b_store[gas_id].ul_tn[2] = 0xff;
#endif /* FEATURE_GSM_GPRS_MSC33 */

    gpl1_gprs_tm_b_store[gas_id].ul_nts = 0;


    /* Initialise the ta_meas_in_progress, tx_in_progress
     * which is used by the serving cell measurement
     * module to avoid scheduling interference
     * measurements on timeslots that clash with
     * the ptcch timeslot, rrbp, usf ul.
     */
    gprs_serv_cell_meas_ptr->ta_meas_in_prog = NO_PTCCH;

    while( tx_timeslot-- > 0 )
    {
      *( tx_ts_ptr-- ) = FALSE;
    }
    tx_ts_ptr++;

    GPL1_RESET_USF_DETECT( gas_id );

    /* Initialise mac mode to invalid */
   l1_transfer_data[gas_id].mac_mode = L1_MAC_MODE_INVALID;

    /* Check the supplied mac mode */
    switch( mac_mode )
    {
    case  MAC_L1_MAC_MODE_DYNAMIC:

      /* Set the mac mode into l1 transfer data */
     l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_DYNAMIC;

      /* Inform NPL1 */
      gl1_msg_set_medium_access_mode( GL1_DEFS_DYNAMIC_ALLOC ,gas_id);

      MSG_GERAN_MED_0_G("mac mode DYNAMIC");
      break;

    case  MAC_L1_MAC_MODE_EXTENDED_DYNAMIC:

      /* Set the mac mode into l1 transfer data */
     l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_EXTENDED_DYNAMIC;

      /* Inform NPL1 */
      gl1_msg_set_medium_access_mode( GL1_DEFS_EXT_DYNAMIC_ALLOC,gas_id );

      MSG_GERAN_MED_0_G("mac mode EXT DYNAMIC");
      break;

    default:
      /* The mac mode is invalid/unavailable, don't
       * do anything.
       */
      break;
    } /* End of switch on mac mode */


    /* Setup egprs incremental redundancy.
     * This function sets the rlc mode.
     */
    gpl1_init_egprs_incr_redundancy( transfer_data_ptr, FN, gas_id );

    /* Set the loopback test mode to OFF */
    transfer_data_ptr->l1_test_mode = L1_TEST_MODE_OFF;

    MSG_GERAN_MED_1_G("Test mode OFF FNmod13 %u",frame_counters[gas_id].FNmod13);

    /* Preset SRB OPEN LOPPBACK to OFF */
    gl1_open_srb_loopback[gas_id] = FALSE;

#ifdef FEATURE_GPRS_PS_HANDOVER

    /* Re-instate the correct NB tx cb functions */
    dynamic_tx_cb.scheduled_callback = gpl1_gprs_tx_dyn_rrbp_sched_cb;
    dynamic_tx_cb.discarded_callback = gpl1_gprs_tx_dyn_rrbp_disc_cb;
    dynamic_tx_cb.usf_utilisation_callback = gpl1_gprs_tx_dyn_rrbp_usf_util_cb;
#endif

    /* configure NPL1s tx handler */
    gl1_msg_cfg_ul_pacch_pdtch( freq_info_ptr->tsc,
                                TS0, /* Access Burst TSC */
                                l1_idle_data->campedon_cell_BSIC,
                                &dynamic_tx_cb,gas_id );

    if( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
    {
        dl_timeslot_alloc = dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;
    }

    /* Work out the timeslot information resulting
     * from the received assignment message.
     */
    gpl1_calc_dyn_dl_ts_info( dl_timeslot_alloc,                  /* In  */
                              &( dynamic_tbf_ptr->usf_value[0] ), /* In  */
                              &( downlink_ts_info[gas_id] ),              /* Out */
                              &( alloc_type[0] ), gas_id );               /* Out */

    if( downlink_ts_info[gas_id].lowest_dl_timeslot != l1_get_current_timeslot(gas_id) )
    {
        /* Inform future ptcch rx\tx operation of
         * the change in the lowest downlink timeslot.
         */
        gpl1_gprs_ptcch_reconfig(gas_id );

        /* Update any active NPl1 ptcch handlers immediately */
        gpl1_gprs_ptcch_set_lowest_dl_timeslot( downlink_ts_info[gas_id].lowest_dl_timeslot,gas_id );
    }

    /* This is called regardless of whether the current timeslot has changed
     * or not. This is a requirement from the surround cell engine.
     */
    gpl1_gprs_change_timeslot( downlink_ts_info[gas_id].lowest_dl_timeslot, gas_id );

    /* Configure NPL1s rx handler */
    gl1_msg_cfg_dl_pacch_pdtch( &( alloc_type[ 0 ] ),
                                downlink_ts_info[gas_id].num_dl_pdch,
                                freq_info_ptr->tsc,
                                dynamic_tbf_ptr->usf_granularity,
                                &dynamic_rx_cb,
                                gas_id);

    /* Update the FN variable after the gpl1_gprs_change_timeslot( ) is called
     * incase lowest DL timeslot decreased and caused the frame counters
     * of the MS to be incremented by one. This ensures FN is the
     * most up-to-date value.
     */
    FN = gl1_get_FN( gas_id );

    gpl1_gprs_config_ta_param( TRUE,
                               freq_info_ptr->tsc,
                               downlink_ts_info[gas_id].lowest_dl_timeslot, gas_id );

    /* no current tbf present so allow rx and tx's to be scheduled */
    transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap = FALSE;

    if (transfer_data_ptr->dl_tbf_ptr)
    {
      transfer_data_ptr->dl_tbf_ptr->allow_one_rb_gap = FALSE;
    }

#ifdef FEATURE_GSM_DTM
    /* only doing packet channels here setup afc_tt mode */
    gl1_msg_set_afc_tt_mode(AFC_TT_MODE_PDCH, gas_id);
#endif

    /* Initialise AFC/TT for Packet transfer based on the frequency band */
    gl1_msg_pdch_afc_tt_init(freq_info_ptr->frequency_list.channel[0],gas_id);

    /* Configure AGC for dl power control ( if present) */
    gpl1_gprs_config_agc(gas_id);

    /* Set AGC valid on first RX block */
    gpl1_gprs_update_first_rx(TRUE, gas_id);

    /* initiate RF Tx band change - only gets actioned if band has actually changed */
    gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]),gas_id);

    /***
     *** General Initialisation
     ***/

#ifdef FEATURE_GSM_MDSP_ESAIC
    /* indicate no inter-cell handover as this is packet transfer */
    gl1_msg_clear_esaic_iir_filter(FALSE,FALSE, gas_id);
#endif
    /* setup metrics and log data for downlink */
    gpl1_gprs_init_metrics_cb_store( gas_id );

    /*Reset rrbp tx counter */
    gpl1_gprs_reset_num_rrbp_msg_txd(gas_id );
    l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

    /* Initialise abort callbacks */
    GPL1_RESET_ABORT_CB( gas_id );

    l1_transfer_data[gas_id].abort_receives = FALSE;

    /* Initialize count of scheduled Tx messages
     * Used to control scheduling of PACCH_PDTCH
     * messages to NPL1
     */
    dynamic_tbf_ptr->sched_msgs = 0;

    /* disable the first tx from being scheduled */
    GPL1_DISABLE_TX( gas_id);

    /* used to indicate that the pca has been transmitted
     * and therefore the tbf can be released
     */
    transfer_data_ptr->pca_sent = FALSE;

    /* Initialsise first_tx_block. Used to send first rlc_data_block_ind
     * to mac */
    transfer_data_ptr->first_tx_block=TRUE;

    /* initialise downlink buffer pointers to begining of fifo */
    gpl1_gprs_init_dl_fifo(gas_id);

    /* Reset the deferred buffers contents and associated
     * boolean flag.
     */
    GPL1_RESET_DEFER_BUFF( gas_id );
    GPL1_CLEAR_ABORT_FLAG( gas_id );

    /* preset wait for pca vars */
    transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
    transfer_data_ptr->wait_for_pca_valid = FALSE;


     /* Set the usf granularity information */
     gpl1_gprs_set_usf_gran_info( 0,      /* granularity block count */
                                  TRUE,   /* start granularity block count */
                                  gas_id );

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
     gpl1_gprs_init_tuneaway_and_ncell_data(gas_id);
#else

#ifdef FEATURE_GPRS_GBTA
    gpl1_set_gbta_status(gas_id);
#endif  /*FEATURE_GPRS_GBTA */

    /* initialise TBF suspension variables */
    gpl1_gprs_init_tbf_suspension(gas_id);

#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

    /* work out which sub state to set up */
    switch (transfer_data_ptr->current_alloc_type)
    {
    case L1_DOWNLINK_ALLOC:
      l1_tskisr_blk->sub_state = L1_DOWNLINK_1;
      /* send reset as we have just come from the 51 multiframe */
      gl1_msg_reset(gas_id);

#ifdef PL1_FW_SIM
      MSG_GERAN_MED_0_G("PL1: sub state = L1_DOWNLINK_1");
#endif
      break;

    case L1_DYNAMIC_ALLOC:
      l1_tskisr_blk->sub_state = L1_DYNAMIC_1;
      l1_tskisr_blk->sub_sub_state = L1_NO_DL;

#ifdef PL1_FW_SIM
      MSG_GERAN_MED_0_G("PL1: sub state = L1_DYNAMIC_1");
#endif
      break;

    default:
      MSG_GERAN_ERROR_1_G("PL1: Incorrect allocation type %d",transfer_data_ptr->current_alloc_type);
      break; /* do nothing */
    }

    /* Reset the uplink number of blocks structure */
    gpl1_reset_uplink_tbf_blk_count( gas_id );

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

    decrement_fn_by_one[gas_id] = FALSE;

#endif
#endif

  } /* End of if sub_state == L1_TRANSFER_INIT */

  switch (l1_tskisr_blk->sub_state)
  {
#ifdef FEATURE_GPRS_PS_HANDOVER
  case  L1_PSHO_RECONFIG:
  {
    /* This is a copy of transfer config to isolate changes */
    /* Setup the usf allocation array */
    gl1_defs_rx_pkt_alloc_type  alloc_type[ GL1_DEFS_MAX_ASSIGNED_DL_TS ]=
    {
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 },
      { (gl1_defs_usf_type) GL1_DEFS_PDCH_UNALLOCATED, (gl1_defs_tn_type) 0 }
    };

    uint8                     dl_timeslot_alloc = 0;

    /* Take a copy of mac_mode */
    uint8                     mac_mode = dynamic_tbf_ptr->mac_mode;


    MSG_GERAN_MED_1_G("gpl1_gprs_control_dynamic_tbf sub_state = %d",l1_tskisr_blk->sub_state);
    MSG_GERAN_MED_0_G("Reconfigure to new cell");

    /* @@OG: Setup the access burst callback functions */
    dynamic_tx_cb.scheduled_callback = gpl1_access_burst_tx_cb;
    dynamic_tx_cb.discarded_callback = gpl1_access_burst_discarded_tx_cb;
    dynamic_tx_cb.usf_utilisation_callback = NULL;


    /* Make sure no measurements are in progress when the timebase change occurs */
    gl1_msg_abort_pwr_meas(gas_id);

    if (l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
    {
      /* SC is not stopped when L1_PSHO_RECONFIG is recived but in the
      abort_pwr_meas function l1_sc_globals_ptr->monscan_in_progress flag is set to flase(Should not happen).
      This is corrected below(CR509649)*/
      l1_sc_globals_ptr->monscan_in_progress = TRUE;
      L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress ,L1_FNAME_GPL1_GPRS_DYNAMIC,L1_VAR_MONSCAN_IN_PROGRESS, gas_id);
    }
    else
    {
      MSG_GERAN_HIGH_1_G(" WRONG_SURROUND_MODE = %d",l1_sc_globals_ptr->mode);
    }

    /* If we are connecting a downlink assignment then its mac_mode
     * must be taken into consideration.
     * Incase of a timeslot reconfigure we rely on the dynamic
     * part to deliver the mac mode change hence the checks below
     * should suffice.
     */
    if(  ( DOWNLINK_TBF_ESTABLISHED == transfer_data_ptr->tbf_est_type )
       &&( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
       &&(  ( L1_DOWNLINK_ALLOC == transfer_data_ptr->current_alloc_type )
          ||( L1_DYNAMIC_DOWN_ALLOC == transfer_data_ptr->current_alloc_type )
         )
      )
    {
      mac_mode = dynamic_tbf_ptr->dl_tbf_ptr->mac_mode;
    }

    /* Setup egprs incremental redundancy.
     * This function updates the rlc mode.
     */
    gpl1_reconfig_egprs_incr_redundancy( transfer_data_ptr, FN ,gas_id);


    /* configure NPL1s tx handler */
    gl1_msg_cfg_ul_pacch_pdtch( freq_info_ptr->tsc,
                                TS0, /* Access Burst TSC */
                                gpl1_psho_data.target_cell_details.bsic,
                                &dynamic_tx_cb,gas_id );

    if( NULL != dynamic_tbf_ptr->dl_tbf_ptr )
    {
        dl_timeslot_alloc = dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;
    }

    /* Work out the timeslot information resulting
     * from the received assignment message.
     */
    gpl1_calc_dyn_dl_ts_info( dl_timeslot_alloc,                  /* In  */
                              &( dynamic_tbf_ptr->usf_value[0] ), /* In  */
                              &( downlink_ts_info[gas_id] ),              /* Out */
                              &( alloc_type[0] ), gas_id );               /* Out */

    delayed_current_timeslot[gas_id] = downlink_ts_info[gas_id].lowest_dl_timeslot;

    if( downlink_ts_info[gas_id].lowest_dl_timeslot == l1_get_current_timeslot(gas_id) )
    {
        /* This is called regardless of whether the current timeslot has changed
         * or not. This is a requirement from the surround cell engine.
         */
        gpl1_gprs_change_timeslot( downlink_ts_info[gas_id].lowest_dl_timeslot, gas_id );

        /* Reset delayed current timeslot to prevent it from
         * getting used incase the radio block gap was
         * incorrectly set.
         */

#if 0 /* @@OG: This is not necessary in PSHO, we need the delayed current ts to be always set
       * because it needs to be applied after we've synced up to the target.
       */
        delayed_current_timeslot[gas_id] = GPL1_INVALID;

#endif

    }
    else
    {
        /* Print out warning, for debug purposes */
        MSG_GERAN_HIGH_3_G("delayed camping on ts %u FNmod13 %u  STmod13 %u",
                 delayed_current_timeslot[gas_id],
                 frame_counters[gas_id].FNmod13,
                 ADD_MOD( frame_counters[gas_id].FNmod13,
                          SUB_FN( transfer_data_ptr->starting_time, FN ),
                          13 )
                );

        /* Inform future ptcch rx\tx operation of
         * the change in the lowest downlink timeslot.
         */
        gpl1_gprs_ptcch_reconfig(gas_id );
    }


    /* Check the supplied mac mode */
    switch( mac_mode )
    {
    case  MAC_L1_MAC_MODE_DYNAMIC:

      /* Set the mac mode into l1 transfer data */
     l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_DYNAMIC;

      /* Inform NPL1 */
      gl1_msg_set_medium_access_mode( GL1_DEFS_DYNAMIC_ALLOC,gas_id );

      MSG_GERAN_MED_0_G("mac mode DYNAMIC");
      break;

    case  MAC_L1_MAC_MODE_EXTENDED_DYNAMIC:

      /* Set the mac mode into l1 transfer data */
     l1_transfer_data[gas_id].mac_mode = MAC_L1_MAC_MODE_EXTENDED_DYNAMIC;

      /* Inform NPL1 */
      gl1_msg_set_medium_access_mode( GL1_DEFS_EXT_DYNAMIC_ALLOC,gas_id );

      MSG_GERAN_MED_0_G("mac mode EXT DYNAMIC");
      break;

    default:
      /* The mac mode is invalid/unavailable, don't
       * do anything.
       */
      break;
    } /* End of switch on mac mode */

    /* Configure NPL1s rx handler */
    gl1_msg_cfg_dl_pacch_pdtch( &( alloc_type[ 0 ] ),
                                downlink_ts_info[gas_id].num_dl_pdch,
                                freq_info_ptr->tsc,
                                dynamic_tbf_ptr->usf_granularity,
                                &dynamic_rx_cb,
                                gas_id);

#ifdef PL1_FW_SIM
    /* Configure uplink and/or downlink tbf */
    MSG_GERAN_MED_0_G("PL1: DYNAMIC reconfig");
#endif

    /* Update the FN variable after the gpl1_gprs_change_timeslot( ) is called
     * incase lowest DL timeslot decreased and caused the frame counters
     * of the MS to be incremented by one. This ensures FN is the
     * most up-to-date value.
     */
    FN = gl1_get_FN( gas_id );

    gpl1_gprs_config_ta_param( FALSE,
                               freq_info_ptr->tsc,
                               downlink_ts_info[gas_id].lowest_dl_timeslot, gas_id );


    /* @@OG: Initialise AFC/TT for Packet transfer based on the frequency band */
    gl1_msg_pdch_afc_tt_init(freq_info_ptr->frequency_list.channel[0],gas_id);

    /* Configure AGC for dl power control ( if present) */
    gpl1_gprs_config_agc(gas_id);

    /* Set AGC valid on first RX block */
    gpl1_gprs_update_first_rx(TRUE, gas_id);

    /* initiate RF Tx band change - only gets actioned if band has actually changed */
    gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]),gas_id);

//ITP PSHO reset rrbp an data queue before the switch and
// then post PACCH AB to send on first detection of USF
   // gpl1_gprs_reset_pacch_fifo();
   // gpl1_gprs_reset_rrbp_enteries();

// would like to flust RLC data queue but not sure how to implement this yet


#if 0 /* @@OG: We should not require this at all, the sc is not stopped when
       * the PSHO request is received anymore.
       */

    /* Kick off SCE as part of the reconfigure */
    /* not sure of the protocol here !! */
    l1_sc_start( L1SCModeGprsTrans, gas_id );

    if (l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
    {
      MSG_GERAN_HIGH_0_G("PSHO Start SCE");
      l1_sci_go(gas_id);
    }
    else
    {
      MSG_GERAN_HIGH_1_G("WRONG_SURROUND_MODE = %d",l1_sc_globals_ptr->mode);
    }

#endif

    /************************************************************************************
     **
     ** If we are configuring or reconfiguring an Uplink TBF then stop the first TX
     ** from being scheduled since we have not yet received the USF on the new assignment
     **
     ** enable_tx is re-enabled after the first PDCH Tx attempt
     **
     ** Fix for CR 72124
     ***********************************************************************************/
    if( (!transfer_data_ptr->starting_time_elapsed) &&
        ( (transfer_data_ptr->tbf_est_type == UPLINK_TBF_ESTABLISHED) ||
          (transfer_data_ptr->tbf_est_type == UPLINK_DOWNLINK_TBF_ESTABLISHED)
        ) )
    {
       GPL1_DISABLE_TX( gas_id);
    }

    /* Work out if sch can go ahead or recongig to close to idle frame */
    switch (frame_counters[gas_id].FNmod52)
    {
    case 20:
    case 21:
    case 22:
    case 46:
    case 47:
    case 48:
        trans[gas_id].reconfig_two_close = TRUE;
        MSG_GERAN_MED_0_G("RECONFIG_TOO_CLOSE");
        break;

    default:
        trans[gas_id].reconfig_two_close = FALSE;
        break;
    }

    l1_tskisr_blk->sub_state = L1_PSHO_IN_PROGRESS;

    /* @@OG: This is used when applying the timebase change */
    l1_perform_ps_hover[gas_id] = TRUE;
    l1_psho_initial_transition_delay[gas_id] = TRUE;

  } /* End of L1_PSHO_RECONFIG */

  /*lint -fallthrough*/
  case  L1_PSHO_IN_PROGRESS:
  {
    /*
     * 44.060 8.10.3.2, The target BSS expects one or more PS HANDOVER ACCESS
     * messages from the mobile station whenever Handover Reference information
     * is included in the PS HANDOVER COMMAND message
     */
    if( FALSE != gpl1_psho_data.handover_reference_present )
    {
     if (l1_psho_initial_transition_delay[gas_id] == FALSE)
     {
      /* @@OG: We need the psho reconfig state to persist
       * and leep on trying to send access bursts till the handover succeeds
       * and then we can change state.
       * Prevent any Normal burst activity since the timing advance is not known yet.
       */
      GPL1_DISABLE_TX( gas_id);

      if( FALSE == transfer_data_ptr->disable_rx_tx )
      {
        /* @@OG: Sending of the Access bursts is radio block based
         * as opposed to frame by frame.
         * Hence add a check on FNmod13.
         */
        switch( frame_counters[gas_id].FNmod13 )
        {
          case  12:
          case  3:
          case  7:
          {
            uint8  timeslot = 0;

            while( timeslot < 8 )
            {
              if( GPL1_INVALID != transfer_data_ptr->dynamic_tbf_ptr->usf_value[timeslot] )
              {
                /* @@OG: the frequency configuration is called inside this function */
                gpl1_gprs_psho_AB_on_pacch( freq_info_ptr,
                                            timeslot,
                                            gpl1_psho_data.psho_access_burst_type,
                                            (int16) gprs_serv_cell_meas_ptr->pch[timeslot], gas_id );

                pacch_pdtch_ul_blk_start_cnt = 0;

                break; /* @@OG: Allow only one timeslot tx to occur at the moment */
              }

              timeslot++;

            }/* End of while loop */
          }
          break;

          default:
            /* Nothing to be done here */
          break;

        } /* End of switch statement */

      } /* End of if disable rx tx == FALSE */
     }
     else
     {
      l1_psho_initial_transition_delay[gas_id] = FALSE;
     }
    }   /* End of if handover present && PSHO NON SYNC */
    else
    {
      /* Restore the sub state and the tx schedule/discard function pointers */
      dynamic_tx_cb.scheduled_callback = gpl1_gprs_tx_dyn_rrbp_sched_cb;
      dynamic_tx_cb.discarded_callback = gpl1_gprs_tx_dyn_rrbp_disc_cb;
      dynamic_tx_cb.usf_utilisation_callback = gpl1_gprs_tx_dyn_rrbp_usf_util_cb;

      /* We can call this multiple times per tdma frame */
      gl1_msg_cfg_ul_pacch_pdtch( transfer_data_ptr->frequency_info_ptr->tsc,  /* repeat current freq_info_ptr->tsc config here */
                                  TS0,                                         /* Access Burst TSC */
                                  gpl1_psho_data.target_cell_details.bsic,
                                  &dynamic_tx_cb,gas_id );

      /* Work out which sub state and sub state to set to */
      switch( transfer_data_ptr->current_alloc_type )
      {
      case  L1_DOWNLINK_ALLOC:

          l1_tskisr_blk->sub_state = L1_DOWNLINK_1;
        break;

      case  L1_DYNAMIC_ALLOC:

          l1_tskisr_blk->sub_state = L1_DYNAMIC_1;
        break;

      case  L1_DYNAMIC_DOWN_ALLOC:

          l1_tskisr_blk->sub_state = L1_DYNAMIC_1;
        break;

      default:
        MSG_GERAN_ERROR_1_G("Pl1: Incorrect allocation type %d",transfer_data_ptr->current_alloc_type);
        break;
      } /* End of swtich on current allocation type */
    } /* End of else statement */
  }   /* End of case L1_PSHO_IN_PROGRESS */
#endif

  /*lint -fallthrough*/
  case L1_DYNAMIC_1:
  case L1_DYNAMIC_2:
  case L1_DOWNLINK_1:
  case L1_DOWNLINK_2:
    {

      switch (frame_counters[gas_id].FNmod13)
      {
      case 0:

        /* Setup new window info if we have a DL EGPRS TBF for RLC ACK mode */
        if ((dynamic_tbf_ptr->dl_tbf_ptr) &&
           (transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS) &&
           (transfer_data_ptr->rlc_mode == RLC_ACK) )
        {
          e_grlc_dl_window_info_t win_info;

          /* IR is enabled for the EGPRS DL TBF so setup WS */
          if(e_grlc_dl_get_window_info(gas_id, &win_info))
          {
            gl1_msg_IR_set_active_window(win_info.start_bsn, win_info.end_bsn,gas_id);
            MSG_GERAN_HIGH_3_G("update dl win st_bsn %d end_bsn %d [%d]",win_info.start_bsn, win_info.end_bsn,gl1_get_FN( gas_id ));
          }else
          {
            /* Setup default params as RLC hasn't been confiured for DL TBF yet
             * No changes required for MSC30 - 34.
             */
            gl1_msg_IR_set_active_window(0,1024,gas_id);

            MSG_GERAN_HIGH_0_G("No win info from RLC");
          }

        }

      /*lint -fallthrough*/
      case 4:
      case 8:
        {
          if (transfer_data_ptr->rlc_svc_timer_active)
          {
            /* RLC has data so kick service block timer on frame 1
            * of each radio block
            */
#ifdef PL1_FW_SIM
            MSG_GERAN_LOW_0_G("PL1: sending RLC service block timer");
#endif
            grlc_service_block_timers(gas_id);

          }
          break;
        }

      default:
        break;
      }

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
     #error code not present
#endif

    pri_fire=gpl1_g2x_ta_fire_sequencer(gas_id);
    if(G2X_TA_PRI_RESERVED == pri_fire )
      {
      MSG_GERAN_LOW_1_G("G2X_TA FIRE SEQUENCER: NOTHING TO DO,TBF FN: %d",gl1_get_FN(gas_id));
      }
      else
      {
      MSG_GERAN_HIGH_2_G("G2X_TA FIRE SEQUENCER: FIRE SEQ:%d ,TBF FN: %d", pri_fire, gl1_get_FN(gas_id));
      if(gpl1_g2x_ta_register_seq(pri_fire, gas_id))
      {
        gpl1_g2x_ta_set_ongoing(pri_fire, gas_id);
      }
    }
    gpl1_g2x_ta_seq[gas_id](gas_id);



      if( FALSE != trigger_ts_change[gas_id] )
      {
          trigger_ts_change[gas_id] = FALSE;
          if( GPL1_INVALID != delayed_current_timeslot[gas_id] )
          {
            uint32 fnmod26_before_ts_change = frame_counters[gas_id].FNmod26;
#ifdef FEATURE_GPRS_PS_HANDOVER
            if( FALSE == l1_perform_ps_hover[gas_id] )
            {
#endif
              if( delayed_current_timeslot[gas_id] < l1_get_current_timeslot(gas_id) )
              {
                  if( 11 == frame_counters[gas_id].FNmod13 )
                  {
                      /* PL1 will miss the ptcch due to the long tdma
                       * frame. Abort any ptcch downlink handlers
                       * active.
                       * This is a requirement from NPL1.
                       */
                      gl1_msg_abort_dl_ptcch(gas_id);
                  }
                  else
                  {
                      /* Compensate for the lost tdma frame
                       * by updating the fn count for the dl ptcch
                       * handler.
                       */
                      gl1_msg_inc_dl_ptcch_frame_count( gas_id );
                  }
              }

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM
              /* @@OG TODO
               * Make sure that frame number is not incremented
               * when the transfer reconfig happens as a result of
               * a dtm enhanced release. This happens when dtm cs
               * timeslot was 0 or 1.
               */
              if( FALSE != decrement_fn_by_one[gas_id]  )
              {
                /* Rewind the MS TDMA frame counter by one */
                GSTMR_DEC_FN_BY_ONE( gas_id );

                /* Update Neighbour cell positions */
                l1_sc_set_sync( 0, (int16) (-5000), gas_id);

                /* Reset the boolean to FALSE */
                decrement_fn_by_one[gas_id]  = FALSE;
              }
#endif
#endif
              /* Task the lower layers to change the timebase
               * A requirement from the surround cell engine is
               * to call this regardless of whether the current
               * timeslot has changed or not.
               */
              gpl1_gprs_change_timeslot( delayed_current_timeslot[gas_id], gas_id );

              if ( (fnmod26_before_ts_change == 23 ) && (frame_counters[gas_id].FNmod26 == 24) )
              {
                MSG_GERAN_HIGH_0_G("Idle frame missed due to change in timeslot - abort sce rx");
                abort_receive(gas_id);
              }
#ifdef FEATURE_GPRS_PS_HANDOVER
            } /* End of l1_perform_ps_hover is FALSE */
            else
            {

              /* check if we are reverting to the original cell following
               * an unsuccesful HO
               */
              if( get_l1_psho_revert_command(gas_id) != TRUE)
              {
                /* Sync to the target cell
                 * This also sets the new lowest rx timeslot and updates the BA list.
                 */
                gpl1_gprs_sync_to_target( downlink_ts_info[gas_id].lowest_dl_timeslot,
                                          &gpl1_psho_data.current_cell_details,
                                          &gpl1_psho_data.target_cell_details,
                                          &gpl1_psho_data.previous_cell_details,
                                          gas_id );
              }else
              {
                /* revert back to current cell */
                L1_sync_to_current(&gpl1_psho_data.current_cell_details, gas_id);
              }

              set_l1_psho_revert_command(FALSE, gas_id);
              /* Finish off setting up the AFC and AGC
               * Beware of serving cell measurements whose results come back
               * immediately after the handover.
               * These should not be added as part of the new serving cell.
               */
              gpl1_xfer_complete_psho( gas_id);

              /* Reset the boolean */
              l1_perform_ps_hover[gas_id] = FALSE;
            }
#endif
            /* Update the lowest dl timeslot information for
             * the ptcch module.
             * Update any active NPl1 ptcch handlers immediately
             */
            gpl1_gprs_ptcch_set_lowest_dl_timeslot( delayed_current_timeslot[gas_id],gas_id );

            /* Update the FN variable after the gpl1_gprs_change_timeslot( ) is called
             * incase lowest DL timeslot decreased and caused the frame counters
             * of the MS to be incremented by one. This ensures FN is the
             * most up-to-date value.
             */
            FN = gl1_get_FN( gas_id );

            /* Reset the stored current timeslot to prevent misuse */
            delayed_current_timeslot[gas_id] = GPL1_INVALID;
          } /* End of if delayed current timeslot != invalid */

          /*start the monitors again as they were stopped during lowest tn change*/
          if (l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
          {
            MSG_GERAN_MED_0_G("Start SC monscan again after lowest tn change");
            l1_sci_go(gas_id);
          }
          else
          {
            MSG_GERAN_MED_1_G( "SCE mode changed %d",l1_sc_globals_ptr->mode);
          }
      }   /* End of if adjust ms timebase is TRUE */


      switch( frame_counters[gas_id].FNmod13 )
      {
        case 0:

          if( 0 == frame_counters[gas_id].FNmod104 )
          {
            gl1_msg_pdch_tt_adjust(gas_id);
          }
          break;

        case 3:   /* Schedule for Block 1 */
        case 7:   /* schedule for Block 2 */
        case 12:  /* schedule for Block 3 */
        {

          /* Initialise the highest receive timeslot to an invalid value.
           * This is conveyed to neighbour cell scheduling routines when
           * approaching the IDLE frame.
           */
          uint8  highest_rx_timeslot = 0xFF;

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM
          const q_mac_l1_delete_ul_ctrl_msg_t*  msg_ptr = gpl1_read_mac_l1_delete_ul_ctrl_msg(gas_id);

          if( NULL != msg_ptr )
          {
            /* Do we have a pending pacch fifo delete event */
            if( FALSE != msg_ptr->valid )
            {
              /* Search for the msg type in the PACCH FIFO and remove them
               * This is safe at this point because scheduled messages should
               * be zero and only pending messages are set.
               * Hence no interaction with lower layers to worry about at this
               * point.
               */
              uint8 csn_msg_type = msg_ptr->msg.message_type;

              MSG_GERAN_MED_2_G(" ul ctrl del FN %u type %u ", FN, csn_msg_type);

              /* Search through the pacch fifo and remove pending messages */
              gpl1_pacch_fifo_delete_msg( csn_msg_type, gas_id );

              /* Search through the RRBP queue and remove pending messages
               * Be careful of removing messages which the pending release is
               * awaiting transmission of.
               */
              gpl1_rrbp_delete_msg( csn_msg_type,gas_id );

              /* Reset queued message to invalid when completed */
              gpl1_remove_mac_l1_delete_ul_ctrl_msg( gas_id );

            } /* End of if queued delete msg is valid */
          }
          else
          {
            /* Print an error message */
            MSG_GERAN_ERROR_1_G("NULL msg ptr FNmod13 %u ", frame_counters[gas_id].FNmod13);
          }
#endif
#endif

          switch (frame_counters[gas_id].FNmod52)
          {
          case 25:
          case 51:
            gl1_hw_rf_idle_frame_processing(gas_id);
          break;

          default:
            break;
          }

          /* Check whether or not the SRB loopback needs to be opened */
          if( FALSE != gl1_open_srb_loopback[gas_id] )
          {
              transfer_data_ptr->l1_test_mode = L1_TEST_MODE_SRB_OFF;
              gl1_msg_pdch_set_loopback( FALSE,gas_id  );

              gl1_open_srb_loopback[gas_id] = FALSE;
          }

          /* stop rx and tx's from being scheduled if lowest numbered tn chages
          * while tbf is being reconfigured 05.02 sec. 6.4.2.2
          */

          if (!transfer_data_ptr->disable_rx_tx)
          {
            /* Used to cancel RX/TX for two block period boundaries */
            static uint8  tra_trb_cancel_ts_alloc[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0x0) };

            /* Used to cancel RX/TX for this block boundary */
            uint8    l1_cancel_ts_alloc = 0x0;

            /* Used to task receives for downlink tfi timeslots */
            uint8  dl_tn_alloc    = 0;

            /* Initialise index into metrics/log/data/ buffer */
            uint8  rlc_mac_dl_info_index = 0;


            /* This is the first receive/transmit tasking
             * following the transfer init state.
             */
            if( FALSE != l1_transfer_init[gas_id] )
            {
                /* Set the correct tbf mode to NPL1 */
                if( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
                {

                  /* Enable SAIC if AEQ is defined for EGPRS */
                  if(gl1_msg_is_aeq_egprs_enabled(gas_id))
                  {
                    gl1_hw_cfg_saic(TRUE, gas_id);

                    /* increase speed for EGPRS mode */
                    g11_hw_egprs_mcpm_change(TRUE,gas_id);
                    mdsp_enable_aeq_8psk_dynamic(TRUE ,gas_id);
                  }
                  gl1_msg_set_egprs_mode( TRUE,TRUE,gas_id );

                }else
                {

#ifdef FEATURE_GSM_MDSP_ESAIC
                  /* Enable SAIC only if EGPRS is not enabled */
                  gl1_hw_cfg_saic(TRUE, gas_id);
#endif
                }

                /* Reset the transfer init boolean */
                l1_transfer_init[gas_id] = FALSE;
            }

            gpl1_gprs_calculate_arfcns(l1_get_gprs_arfcns(gas_id), freq_info_ptr, NORM,gas_id);

            /* if an additional downlink allocation is configured
             * then assign timeslot allocation
             */
            if (dynamic_tbf_ptr->dl_tbf_ptr)
            {
              dl_tn_alloc = dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;
            }

            /* Fetch current value of num_rx_tn */
            gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

            if( 0x0 != tra_trb_cancel_ts_alloc[gas_id] )
            {
              /* This is the second block boundary after the flag
               * was set so reset to FALSE.
               */
              l1_cancel_ts_alloc |= tra_trb_cancel_ts_alloc[gas_id];

              /* preset clash_tn_found for detection of pdtch and RRBP */
              tra_trb_cancel_ts_alloc[gas_id] = 0x0;
            }

            /* Check the RRBP msg queue for any clashes with the receive allocation
             * (downlink and usf monitor).
             * If any clashes are found then temporarly adjust block rx/tx for 1
             * or 2 block periods.
             * | OR the flag to ensure the settings from the last block period
             * are not over-written thus potentially causing a timeslot clash.
             */
            gpl1_gprs_clean_rrbp_enteries( FN,
                                           gpl1_store_ptr->num_rx_tn,
                                           &( tra_trb_cancel_ts_alloc[gas_id] ),
                                           &( l1_cancel_ts_alloc ), gas_id );

            if( L1_MODE_RUN == mode )
            {
              /***
               *** Schedule DL PACCH_PDTCH TN(s)
               ***/

              /***********************************************************************
               ***
               *** schedule Allocated DL TN's
               ***
               ***********************************************************************/

              /* Update the camp timeslot */
              gl1_msg_pdch_set_camp_tn( (gl1_defs_tn_type) l1_get_current_timeslot(gas_id),gas_id );

              for (i=0;i< MAX_TN;i++)
              {
                /* We schedule all dl timeslots every block therefore we should
                 * have at least one tn either from the DL tbf and/or UL TBF
                 */
                if( (  ( dynamic_tbf_ptr->usf_value[i] != GPL1_INVALID )
                     ||( 0 != ( dl_tn_alloc & 0x1 ) )
                    )
                  &&( 0 == ( ( l1_cancel_ts_alloc >> i ) & 0x01) )
                  )
                {
                  /* Make sure the absolute timeslot is higher than or equal to
                   * camp timeslot. This assumes that PL1 slams the frame interrupt
                   * to lowest receive timeslot.
                   */
                  if( i >= l1_get_current_timeslot(gas_id) )
                  {
                    /* Setup DL data buffer */
                    gpl1_gprs_init_data_cb_store(rlc_mac_dl_info_index, transfer_data_ptr->rlc_mac_dl_buff.in_ptr, gas_id);

                    /* Schedule Downlink PDCH */
                    gl1_msg_rx_pacch_pdtch( (gl1_defs_tn_type) ( (uint8) i - l1_get_current_timeslot(gas_id) ),
                                            l1_get_gprs_arfcns ( gas_id),
                                            &transfer_data_ptr->packet_transfer_agc,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                                            &transfer_data_ptr->packet_transfer_div_agc,
#endif
                                            &gpl1_store_ptr->rlc_mac_dl_info[rlc_mac_dl_info_index++],gas_id );


                    /* Set the highest receive timeslot
                     * This needs to be done only on the boundary
                     * of the radio block preceding the IDLE frame.
                     */
                    switch( frame_counters[gas_id].FNmod52 )
                    {
                      case  20:
                      case  46:
                        highest_rx_timeslot = i;
                      break;

                      default:
                      break;
                    }

                    /* Check the total number of receive timeslots
                     * tasked so far against the RX parameter.
                     */
                    if( rlc_mac_dl_info_index > PL1_MULTISLOT_RX )
                    {
                      /* too many TS allocated, so shout */
                      MSG_GERAN_ERROR_0_G("PL1: Dynamic MS class failure");
                    }

                    /* Increment in pointer to dl fifo for next dl schedule
                     * reset to beginning if end of fifo is reached
                     */

                    if (transfer_data_ptr->rlc_mac_dl_buff.in_ptr >= &(gpl1_store_ptr->gpl1_gprs_dyn_dl_data[MAX_NUM_DL_MSGS]) )
                    {
                      transfer_data_ptr->rlc_mac_dl_buff.in_ptr = (gl1_defs_rx_pkt_data_type*)&(gpl1_store_ptr->gpl1_gprs_dyn_dl_data[0]);
                    }
                  } /* End of if abs ts >= camp ts */
                  else
                  {
                    /* Prevent invalid relative timeslot values from reaching NPL1
                     * and hence cancel receive tasking and prin a message error.
                     */
                    MSG_GERAN_ERROR_2_G( "rx cancelled abs TS %u less than camp TS %u",
                               i, l1_get_current_timeslot(gas_id ) );
                  }/* End of if abs ts < camp ts */
                }  // end of if(tn_alloc..

                /* move DL alloc onto the next timeslot */
                dl_tn_alloc>>=1;

              } //end of for(i.. MAX_TN
            } /* End of if l1 mode = run */

            /* DL BLOCK start for PACCH or PDTCH to be set in 1 frame */
            pacch_pdtch_dl_blk_start_cnt = 0;

            /****************************************
            ***
            *** Check if we need to transmit an RRBP
            ***
            ****************************************/

            /* invalidate which RRBP message to tx
             * in case no RRBP are in the upcomming block
             */
            i = SIZE_OF_RRBP_BUFF;

            /* check if rrbp is comming up in the next block
             * or if rrbp expired in gta gap
             */
            if ( gpl1_gprs_get_num_rrbp_msg(gas_id)
#ifdef FEATURE_G2X_TUNEAWAY
                 || gpl1_expired_rrbp_msg_in_gta(gas_id)
#endif /* FEATURE_G2X_TUNEAWAY */
               )
            {
              uint32  fn_num = ADD_FN( FN, NPL1_FN_OFFSET );

              while ( (mode == L1_MODE_RUN) &&
                      ((i = gpl1_gprs_get_matching_rrbp(fn_num, fn_num, GPL1_INVALID_TN, gas_id)) < SIZE_OF_RRBP_BUFF) )
              {
                /* Store away the message type of the RRBP we about to transmit to help us
                * in deciding which message to transmit if our next RRBP has a clash
                */

                transfer_data_ptr->last_rrbp_msg_type = gpl1_gprs_get_rrbp_msg_type(i, gas_id);

                /* We are about to TX an RRBP, NPL1 should schedule the RRBP and save
                * the pdch if the USF was detected in the previous block and transmit
                * it when the usf is detected again.
                *
                * Need to check if RLC data pointer is deleted for the RRBP tn or
                * if the pointer is saved until the next USF is detected
                */


                /* Check if we are sending a PDAN with FAI=1 for the first time
                * or for DL unack mode we are sending a PCA with FAI=1
                */
                switch( transfer_data_ptr->last_rrbp_msg_type )
                {
                case  PDAN_MSG_WITH_FAI:
                case  PCA_DL_FAI_MSG:

                  /* start timer 3192 */
                  grlc_dl_start_t3192(gas_id);
                  /* just started T3192 so clear out the flag */
                  transfer_data_ptr->first_t3192_start = FALSE;
                  MSG_GERAN_MED_0_G("PL1: START_T3192");
#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
                  gpl1_gprs_reset_tfi_when_T3192_running(gas_id);
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */
                  break;

                default:
                  /* No action required here */
                  break;
                } /* End of switch statement on last rrbp msg type */

                /* Only schedule RRBP if we have a valid ta for txing normal bursts or access bursts*/
                transfer_data_ptr->last_rrbp_msg_type = gpl1_gprs_transfer_tx_rrbp( i, gas_id );

                pacch_pdtch_ul_blk_start_cnt = 0;

              } /* (i < SIZE_OF_RRBP_BUFF) && ...*/

#ifdef FEATURE_G2X_TUNEAWAY
              if(gpl1_expired_rrbp_msg_in_gta(gas_id))
              {
                pacch_pdtch_ul_blk_start_cnt = 0;
              }
#endif /* FEATURE_G2X_TUNEAWAY */
              /* Check to see if there is a release pending */
              if ( gpl1_gprs_awaiting_release(gas_id) )
              {

                /* Check if the routes to release have resulted in the approriate RRBP
                * msgs being txd.
                * if ul is being release have all ul rrbp been txd
                * if ul and dl is being released have all rrbp been txd
                * Partial releases do not result in a return to idle mode
                * Full releases result in a return to idle
                */
                if ((transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_UL) ||
                    (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
                    (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL))
                {

                  MSG_GERAN_LOW_1_G("UL TBF DEAD [%d]",FN);
                  /*  Reset ul_desc pointer so any previous READS discarded. */
                  memset(&transfer_data_ptr->rlc_ul_status,0,sizeof(rlc_ul_status_T) );
                  transfer_data_ptr->rlc_ul_status.ul_desc_ptr  = &transfer_data_ptr->rlc_ul_status.ul_desc[0];

                  /* clear the deffered buffer flag and asociated flag */
                  GPL1_RESET_DEFER_BUFF( gas_id );
                  GPL1_CLEAR_ABORT_FLAG( gas_id );

                  /* check that RLC FIFO is disabled, it is done when the UL Release is rcvd */
                  if (transfer_data_ptr->rlc_ul_data_available)
                  {
                    MSG_GERAN_MED_1_G("RLC UL FIFO still enabled!! [%d]",FN);
                    transfer_data_ptr->rlc_ul_data_available = FALSE;
                  }

                } /* (transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_UL) || */

                /* Have we just scheduled our last RRBP for the TBF*/
                if (!pacch_pdtch_ul_blk_start_cnt)
                {
                  /* Ensure we only set the  pca fn once!! */
                  if (!transfer_data_ptr->wait_for_pca_valid)
                  {
                    /* wait for any new assignments on the PACCH
                     * last block to be processed is block when PCA is sent
                     */
                    transfer_data_ptr->wait_for_pca_fn =  ADD_FN(FN,9);

                    transfer_data_ptr->wait_for_pca_valid = TRUE;
                    MSG_GERAN_LOW_2_G("OLD leaving idle FN [%d] leave fn %d",FN,transfer_data_ptr->wait_for_pca_fn);
                  } else
                  {
                    MSG_GERAN_LOW_1_G("**** pca_valid still true [%d]",FN);
                  }
#ifdef FEATURE_G2X_TUNEAWAY
                  gpl1_set_expired_rrbp_msg_in_gta(FALSE, gas_id);
#endif /* FEATURE_G2X_TUNEAWAY */
                }

              } /*(transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_UL) && */

            } /* gpl1_gprs_get_num_rrbp_msg() */

            /***
             *** Schedule UL PACCH_PDTCH TN(s)
             ***/

            /* disable sending tx's for dynamic allocation
             * for first block as USF's haven't been detected
             * yet so the tx's will just be discarded
             *
             * Also if dynamic UL tbf is not present then the
             * number of timeslots (nts) will be zero so don't
             * schedule dynamic UL
             *
             * Finally check that we have a valid ta otherwise we can't tx
             * normal bursts
             */

            /* Fetch current value of enable_tx */
            gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

            if (   ( gpl1_store_ptr->enable_tx )
                 &&( dynamic_tbf_ptr->nts )
                 &&( gpl1_gprs_timing_advance_valid(gas_id) )
                 &&( mode == L1_MODE_RUN )
                 &&( !l1_transfer_data[gas_id].wait_for_pca_valid ) )
            {
              boolean               update_sig_info = FALSE;
              gl1_defs_coding_type  cs              = GL1_DEFS_CS1_CODING;

              /* schedule usf specific msgs based on the nts
              * available less any rrbp mesgs which are scheduled
              */

                /* Determine what data to transmit and
                * which coding scheme to use
                */
                switch( transfer_data_ptr->l1_test_mode )
                {
                case  L1_TEST_MODE_SRB_ON:
                {
                    /* Set this to TRUE to ensure
                     * gl1_msg_tx_pacch_pdtch_loopback() gets called
                     * on this radio block boundary.
                     */
                    update_sig_info = TRUE;
                }
                    break;

                case  L1_TEST_MODE_B:
                {
                  /* Read loop back data out of RLC into local structure */
                  gpl1_gprs_get_loop_ul_data(gas_id);

                  /* no break fall down */
                }
                /*lint -fallthrough*/
                case  L1_TEST_MODE_A:
                case  L1_TEST_MODE_SRB_OFF: /*lint !e616 */
                case  L1_TEST_MODE_OFF:
                {
                  uint8 sched_tx_count = 0;
                  geran_pdu_priority_t  priority = GPDU_LOW_PRIORITY ;

                  num_rrbp_msg_txd = gpl1_gprs_get_num_rrbp_msg_txd(gas_id);
                  while( dynamic_tbf_ptr->sched_msgs <
                         (dynamic_tbf_ptr->nts - num_rrbp_msg_txd)
                       )
                  {
                    /* Determine what data to transmit and
                     * which coding scheme to use
                     */
                    uint8* payload_data[3];

                    gl1_defs_tx_pkt_data_type  pkt_data;
                    gl1_defs_puncturing_type   ps[2] = { GL1_DEFS_PUNCTURING_1,
                                                         GL1_DEFS_PUNCTURING_1 };

                    /* Schedule EGPRS TBF MCS 1 - 9 or GPRS CS 1-4
                     * if we have RLC data.
                     * If not schedule UL Dummy CS1.
                     */
                    if (transfer_data_ptr->l1_test_mode == L1_TEST_MODE_B)
                    {
                      gpl1_gprs_set_loop_b_data(&cs,
                                                 ps,
                                                 payload_data, gas_id);
                      priority = GPDU_LOW_PRIORITY;

                    }else
                    {
                      priority = gpl1_gprs_get_egprs_ul_data( &cs,
                                                   ps,
                                                   payload_data,
                                                   gas_id,
                                                   FALSE        /* single block alloc */
                                                 );
                    }

                    /* Schedule dynamic UL pdch */

                    /* for GPRS write tx_data to msg_data item 0
                     * for EGPRS write header to msg_data item 0
                     * write msg1 to imsg_data item 1
                     * and write msg2 to msg_data item 2
                     */
#ifdef DEBUG_SHOW_PL1_BSN
                    #error code not present
#endif
                    pkt_data.egprs.hdr           = payload_data[0];
                    pkt_data.egprs.data[0]       = payload_data[1];
                    pkt_data.egprs.data[1]       = payload_data[2];
                    pkt_data.egprs.puncturing[0] = ps[0];
                    pkt_data.egprs.puncturing[1] = ps[1];

                    if( cs > GL1_DEFS_MCS9_CODING )
                    {
                      MSG_GERAN_ERROR_2_G("cs error %d [%d]",cs,gl1_get_FN( gas_id ));

                    }else if(payload_data[0] == NULL)
                    {
                      /* For EGPRS and GPRS ensure at least Payload_data[0]
                       * is valid as this is either the EGPRS header pointer
                       * or the GPRS data pointer
                       */
                      MSG_GERAN_LOW_1_G("No data to send [%d]",gl1_get_FN( gas_id ));

                    }else
                    {

                      gl1_msg_tx_pacch_pdtch_dynamic( &pkt_data,
                                                      cs,priority,gas_id );

                       txed_coding_scheme[sched_tx_count]= cs;
                      /* Update NPL1 message count */
                      sched_tx_count++;

                      /* Allow signal info to be called */
                      update_sig_info = TRUE;
                    }

                    /* Update NPL1 message count */
                    dynamic_tbf_ptr->sched_msgs++;

                  } /* End of while loop */

                  /*If FT handler hasn't been added, and ext_utbf_nodata fllag has been set, then there will be no TxMsgs to Transmit, 
                                so to avoid T3180 timer expiry, L1 must reset the USF_DETECT flag*/
                  if(!update_sig_info && (l1_idle_data->layer1_extra_gprs_sys_info.ext_utbf_nodata == TRUE))
                  {
                    GPL1_RESET_USF_DETECT(gas_id);
                  }

                  /* Ensure that sched_msgs relates to number of
                   * Tx scheduled.
                   * There may be a difference if the coding scheme
                   * is out of the range cs1 < cs > MCS9 or there is not
                   * data to send
                   */
                  dynamic_tbf_ptr->sched_msgs = sched_tx_count;

                  if (transfer_data_ptr->l1_test_mode == L1_TEST_MODE_B)
                  {
                    gpl1_gprs_tm_b_store[gas_id].rx_ctrl_status = 0;
                    gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status = 0;
                  }
                }
                    break;

                default:
                    /* Invalid test mode, flag an error */
                    MSG_GERAN_ERROR_0_G(" Invalid L1 test mode ");
                    break;
                } /* End of switch on l1_test_mode */

              /***
              *** update tx power for each assigned timeslot
              ***/
              if (update_sig_info)
              {
                /* temp place holder for TX PWR to write Eng Mode Info */
                int16 tmp_tx_pwr[8] = {0};

                /* Initialise pdch count */
                uint8  num_pdch = 0;

#ifdef  FEATURE_GSM_GPRS_MSC33
                gl1_msg_ul_pacch_pdtch_signal_type  signal_info[PL1_MULTISLOT_TX] =
                {
                  { GL1_DEFS_TN_0, 0x7fff },
                  { GL1_DEFS_TN_0, 0x7fff }
#if  (PL1_MULTISLOT_TX == (4))
                 ,{ GL1_DEFS_TN_0, 0x7fff }
                 ,{ GL1_DEFS_TN_0, 0x7fff }
#endif
#if  (PL1_MULTISLOT_TX == (5))
                 ,{ GL1_DEFS_TN_0, 0x7fff }
#endif
                };
#else
                gl1_msg_ul_pacch_pdtch_signal_type  signal_info[4] =
                {
                  { GL1_DEFS_TN_0, 0x7fff },
                  { GL1_DEFS_TN_0, 0x7fff },
                  { GL1_DEFS_TN_0, 0x7fff },
                  { GL1_DEFS_TN_0, 0x7fff }
                };
#endif

                /* loop around all 8 timeslots */
                for (i=0;i< MAX_TN;i++)
                {
                  /* a timeslot allocation has been found */
                  if (dynamic_tbf_ptr->usf_value[i]!=GPL1_INVALID)
                  {
                    /*  Serving Cell Measurements */
                    /*  Power control value */
                    uint8  slammed_slot = gpl1_gprs_ts_conv(i, gas_id);
                    signal_info[num_pdch].TXLEV = (int16)gprs_serv_cell_meas_ptr->pch[i];

                    switch ( frame_counters[gas_id].FNmod13 )
                    {
                    case  7:
                      gpl1_gprs_scell_disable_int_meas( (gl1_defs_tn_type) i , gas_id);
                      break;
                    default:
                      break;
                    }

                    signal_info[num_pdch].tn = (gl1_defs_tn_type) slammed_slot;
                    num_pdch++;
                  }

                } /* End of for loop */

                if(  ( num_pdch == 0 )
                   ||( num_pdch > PL1_MULTISLOT_TX )
                  )
                {
                  /* Report an F3 error message */
                  MSG_GERAN_ERROR_1_G(" PL1 transfer: invalid num ul pdch %u ",num_pdch);

                  /* Return from the dynamic control function */
                  return  FALSE;
                }

                /* Configure initial power of frequency values for assigned pdch's
                *  Need to investigate this more, do we need to specify power and arfcn
                * every time new data is set to NPL1 ????
                *
                * This should be the place where we update power and frequency  for the comming
                * radio block
                */
                /*lint -save -e645 fixed by above return */
                if( L1_TEST_MODE_SRB_ON == transfer_data_ptr->l1_test_mode )
                {
                    uint32  pdch = 0;
                    uint32  loop_from_timeslot;

                    /* adjust test mode timeslot to lowest allocated timeslot re 04.14 sec 5.5.5 */
                    if ( (transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn < downlink_ts_info[gas_id].lowest_dl_timeslot) ||
                        (transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn > downlink_ts_info[gas_id].highest_dl_timeslot))
                    {
                      transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn = downlink_ts_info[gas_id].lowest_dl_timeslot;
                    }

                    loop_from_timeslot = (uint32) transfer_data_ptr->dynamic_tbf_ptr->test_mode_tn;

                    /* Make sure loop_from_timeslot is correct */
                    while( loop_from_timeslot > downlink_ts_info[gas_id].highest_dl_timeslot )
                    {
                        loop_from_timeslot = ( loop_from_timeslot + downlink_ts_info[gas_id].lowest_dl_timeslot )
                                           - ( downlink_ts_info[gas_id].highest_dl_timeslot + 1 );
                    }

                    /* Must transmit on all uplink timeslots (ETSI 04.14)
                     * Wrap the timeslot-to-loop-from around max receive
                     * timeslot.
                     */
                    while( pdch < num_pdch )
                    {
                      gl1_msg_tx_pacch_pdtch_loopback( &( signal_info[ pdch ] ),
                                                       l1_get_gprs_arfcns (gas_id ),
                                                       (gl1_defs_tn_type) (loop_from_timeslot - l1_get_current_timeslot(gas_id)),gas_id );

                      /* update the loop_from_timeslot */
                      if( loop_from_timeslot < downlink_ts_info[gas_id].highest_dl_timeslot )
                      {
                        loop_from_timeslot = ADD_MOD( loop_from_timeslot,
                                                      1,
                                                      GL1_DEFS_SLOTS_IN_FRAME );
                      }
                      else
                      {
                        loop_from_timeslot = downlink_ts_info[gas_id].highest_dl_timeslot;
                      }

                      pdch++;
                    } /* End of while statement for tasking npl1 */
                  }
                  else
                  {
                    gl1_msg_set_pacch_pdtch_dynamic_signal_info( signal_info,
                                                                 (num_rrbp_msg_txd+dynamic_tbf_ptr->sched_msgs),
                                                                 l1_get_gprs_arfcns (gas_id ),
                                                                 gas_id );

                  }

                for (i=0; (i<num_pdch && i<dynamic_tbf_ptr->nts && i<dynamic_tbf_ptr->sched_msgs); i++)
                {
                  /* Report using original TS allocation and not slammed */
                  gpl1_log_gprs_power_control( GL1_MSG_UL_PACCH_PDTCH,
                                               (gl1_defs_tn_type) gpl1_gprs_ts_conv_unslam( signal_info[i].tn,gas_id ),
                                               signal_info[i].TXLEV,
                                               txed_coding_scheme[i], gas_id );
                }

                for (i=0; (i<num_pdch && i<dynamic_tbf_ptr->nts && i<dynamic_tbf_ptr->sched_msgs); i++)
                {
                  tmp_tx_pwr[i] = signal_info[i].TXLEV;
                }
                geran_eng_mode_data_write(ENG_MODE_TX_PWR, tmp_tx_pwr, TRUE, gas_id);

                /*lint -restore */
                /* UL BLOCK start for PACCH or PDTCH to be set in 1 frame */
                pacch_pdtch_ul_blk_start_cnt = 0;

              } /* end of if(update_sig_info) .. */

            } /* end of (enable_first_tx && dynamic_tbf_ptr->nts)*/

            /* first block has passed so allow transmissions */
            GPL1_ENABLE_TX( gas_id );

          } /* End of disable_rx_tx == FALSE */
          else
          {
              /* Setup the compare FN so that it is one frame after the rrbp tx FN */
              uint32  compare_FN = ADD_FN( FN, (NPL1_FN_OFFSET + 1) );

              MSG_GERAN_HIGH_1_G( "disabled rx tx tasking FNmod13 %u",frame_counters[gas_id].FNmod13);

              /* Check for RRBP messages due to be transmitted on the next frame and remove them
               * Waiting for the scheduler to detect expired RRBPs can lead to lockup problems whereby
               * no release confirm is sent to MAC.
         */
              gpl1_gprs_delete_oldest_frame_num( compare_FN, gas_id );
          }

          /* Check for the boundary of the radio block preceding
           * the IDLE frame.
           */
          switch( frame_counters[gas_id].FNmod52 )
          {
          case  20:
          case  46:

            /* Inform the Ncell scheduling routines of the highest receive timeslot
             * for this block boundary.
             * Note: Need to do this even for invalid values of the timeslot to ensure
             * no residual information from previous block boundaries is incorrectly
             * eventually used.
             */
            gpl1_pkt_xfer_set_highest_rx_timeslot( highest_rx_timeslot, gas_id );

            /* Make sure the highest tx timeslot is initalised to an invalid value
             * just incase the tx callback never sets it. This avoids conflict scenarios
             * due to expired rogue values.
             */
            gpl1_pkt_xfer_set_highest_tx_timeslot( 0xFF, gas_id );

            break;

          default:
            break;
          }

          break;
        } /* end of case */

        case  10:
        {
          switch( frame_counters[gas_id].FNmod52 )
          {
          case  10:
          case  36:
          {

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
               gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_PTCCH_FRAME_IN_PTM);
#endif
 
              /* Schedule PTCCH */

              /* stop rx and tx's from being scheduled if lowest numbered tn chages
               * while tbf is being reconfigured 05.02 sec. 6.4.2.2
               */
              if ((!transfer_data_ptr->disable_rx_tx)  && (mode == L1_MODE_RUN) )
              {
#ifdef PL1_FW_SIM
                MSG_GERAN_LOW_1_G("FN mod 416 = %d",frame_counters[gas_id].FNmod416);
#endif

                /* Need to pass pointer of data to transmit based on access burst type */
                gpl1_gprs_calculate_arfcns(l1_get_gprs_arfcns ( gas_id), freq_info_ptr, PTCCH,gas_id);

                gpl1_gprs_schedule_ptcch_tx(l1_get_gprs_arfcns_value(0,gas_id),
                                            l1_idle_data->layer1_extra_gprs_sys_info.access_burst_type, gas_id);

                gpl1_gprs_schedule_ptcch_rx(gas_id);

#ifdef FEATURE_GPRS_GBTA
                gpl1_gbta_schedule_idle_pwr_mon(gas_id);
#endif /*   FEATURE_GPRS_GBTA */

              } /* end of disable_rx_tx */
              else
              {
                MSG_GERAN_HIGH_0_G("disabled ptcch rx tx tasking FNmod13 10");
              }
              break;
          } /* End of case 10, case 36 */
          default:
          /* do nothing */
              break;
          } /* End of switch statement */
          break;
        } /* End of case 10 */

#ifdef FEATURE_GPRS_GBTA
          case  11 :
          {
            gas_id_t idle_gas_id;

            {
              switch( frame_counters[gas_id].FNmod52 )
              {
                case  11:
                case  37:
                {
                  /* Checks other gas_id Event status and it is not leaving Idle */
                  if(  gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_ACTIVE,gas_id,&idle_gas_id))
                  {
                    idle_gas_id =  check_gas_id(idle_gas_id);
                    if (l1_sc_get_monscan_active(idle_gas_id))  // Referring to IDLE SUB l1_sc
                    {

                      gl1_gbta_scheduling_idle_event(GBTA_EV_IDLE_PWR_MON,idle_gas_id);

                      /* change state only Transfer sub managed to load the handler */  
                      if(l1_get_idle_monitors_in_progress(idle_gas_id))
                      {
                        MSG_GERAN_MED_2_G("GL1 GBTA Idle Power monitor scheduled FN %d idle_gas_id %d",FN,idle_gas_id);
                        gl1_set_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED,idle_gas_id);
                      }
                      else
                      {
                        MSG_GERAN_HIGH_3_G("GL1 GBTA Idle Power monitor abandoned FN %d idle_gas_id %d IS_SUB_LEAVING_IDLE %d",
                                           FN,idle_gas_id,IS_SUB_LEAVING_IDLE(idle_gas_id));         
                        /* Reset when moving to idle  */
                        grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
                      }
                    }
                    else
                    {
                      MSG_GERAN_HIGH_2_G("GL1 GBTA Idle Power monitor abandoned moving to IDLE FN %d idle_gas_id %d ", FN,idle_gas_id);         
                      /* Reset when moving to idle  */
                      grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
                    }
                  }
                }
                break;

              /* Idle FRAME */
              case 24 :
              case 50 :
                {

             
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_IDLE_FRAME_IN_PTM);
#endif
                  /*SATHISH*/
                  /* check if idle frame free and mon canbe scheduled */
                  if (gprs_serv_cell_meas_ptr->idle_frame_free)
                  {
                    gpl1_gbta_schedule_idle_pwr_mon(gas_id);

                    if(  gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_ACTIVE,gas_id,&idle_gas_id))
                    {
                      idle_gas_id =  check_gas_id(idle_gas_id);
                      if (l1_sc_get_monscan_active(idle_gas_id))  // Referring to IDLE SUB l1_sc
                      {

                        gl1_gbta_scheduling_idle_event(GBTA_EV_IDLE_PWR_MON,idle_gas_id);

                        /* change state only Transfer sub managed to load the handler */  
                        if(l1_get_idle_monitors_in_progress(idle_gas_id))
                        { 
                          gl1_set_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED,idle_gas_id);
                          MSG_GERAN_MED_2_G("GL1 GBTA Idle Power monitor scheduled in IDLE FRAME FN %d idle_gas_id %d", FN,idle_gas_id);
                          gprs_serv_cell_meas_ptr->idle_frame_free  = FALSE;              
                        }
                        else
                        {
                          MSG_GERAN_HIGH_3_G("GL1 GBTA Idle Power monitor abandoned FN %d idle_gas_id %d IS_SUB_LEAVING_IDLE %d",
                                              FN,idle_gas_id,IS_SUB_LEAVING_IDLE(idle_gas_id));         
                          /* Reset when moving to idle  */
                          grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
                        }
                      }
                      else
                      {
                        MSG_GERAN_HIGH_2_G("GL1 GBTA Idle Power monitor abandoned moving to IDLE FN %d idle_gas_id %d ", FN,idle_gas_id);         
                        /* Reset when moving to idle  */
                        grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
                      }
                    }
                  }
                }
                break;

              default :
                /* do nothing */
                break;
              } /* End of case 11, case 37 */

            }/*end */

            break;
          } /* End of case 11 */
#endif /*   FEATURE_GPRS_GBTA */
        default:
          /* do nothing */
          break;
      } /* end of switch */

#ifdef FEATURE_G2X_TUNEAWAY
     if ( gl1_msg_get_multi_sim_mode() )
     {
        /* In Dual sim operation only perform the frame tick when firmware has been downloaded
        this implies that the lock is granted too */
        if ( l1_tskisr_blk->firmware_loaded == TRUE )
        {
          /*  Do Neighbour cell measurement and sync processing */
          if (gpl1_gprs_measurements(mode, gas_id))
          {
            /*
             * Set downlink block flag in next frame
             * if power measurements have been scheduled
             */
            pacch_pdtch_dl_blk_start_cnt = 0;
          }
        }
        else
        {
          MSG_GERAN_HIGH_1_G("No FW gprs_measurements FN %d",FN);
        }
      }
      else
#endif /* FEATURE_G2X_TUNEAWAY */
      /*  Do Neighbour cell measurement and sync processing */
      if (gpl1_gprs_measurements(mode , gas_id))
      {
        /*
         * Set downlink block flag in next frame
         * if power measurements have been scheduled
         */
        pacch_pdtch_dl_blk_start_cnt = 0;
      }

      /* default return value as TRANSFER is not commanded to be stopped */
      idle = FALSE;

      break;
    } /* case DYNAMIC_1, DYNAMIC_2, DOWNLINK_1 and DOWNLINK_2 */

  default:
    MSG_GERAN_ERROR_0_G("PL1: Transfer Bad Sub State!");
    break;

  } /* end of l1_tskisr_blk switch */

  if (mode == L1_MODE_STOP)
  {

    static boolean       sce_idle[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FALSE) };

    /* To avoid Tx's clashing with RX CCCH's in Idle only abort dl on frames 2,3 or 4
     * and then wait for 2 frames before returning to idle
     */
    switch (frame_counters[gas_id].FNmod13)
    {
    case 1: //frame 2
    case 2: //frame 3
    case 3: //frame 4
    case 5: //frame 2
    case 6: //frame 3
    case 7: //frame 4
    case 9: //frame 2
    case 10: //frame 3
    case 11: //frame 4
      {
        if (abort_once_only[gas_id])
        {
          abort_pending[gas_id] = TRUE;
          /*Give SCE a chance to abort any receives in progress. */
          l1_sci_tick_receive(gas_id);

          MSG_GERAN_MED_3_G("Abort DL PDTCH FN[%d][%d][%d]",FN, frame_counters[gas_id].FNmod52,frame_counters[gas_id].FNmod13);
          gl1_msg_abort_dl_pacch_pdtch(gas_id);
          gl1_msg_abort_dl_ptcch(gas_id);

          gl1_msg_abort_int_meas(gas_id);

          /* Store current FN when abort was called */
          transfer_data_ptr->abort_fn = frame_counters[gas_id].FNmod13;

          /* Stop the tx callbacks from being actioned*/
          GPL1_SET_ABORT_CB( gas_id );

          /* Send Abort mode to RLC to ensure FIFO is not left in
           * a waiting for GET/ABORT state
           */
          rlc_config_params.access_mode = ABORT_MODE;
          (void)grlc_ul_fifo_access_block(gas_id, &rlc_config_params);

          /* Clear wait for pca vars */
          transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
          transfer_data_ptr->wait_for_pca_valid = FALSE;

#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_abort(FALSE, gas_id);
#endif

          abort_once_only[gas_id] = FALSE;
        }
        break;
      }

    default:
      /* do nothing, wait for required frame */
      break;
    }

    if (abort_pending[gas_id])
    {
#ifdef FEATURE_GPRS_GBTA
      gas_id_t idle_gas_id;
#endif /* FEATURE_GPRS_GBTA*/

      sce_idle[gas_id] = !l1_sci_is_op_in_progress(gas_id);

#ifdef FEATURE_GPRS_GBTA
       gbta_go_idle = TRUE;
      /* First Sub */
      /* Idle Sub has set Event for Transfer Sub for Power Monitor*/
      if(gl1_check_any_other_subs_idle_evt_power_mon(gas_id,&idle_gas_id))
      {
        /* If power monitors of idle sub is not scheduled then clear the monitor event.
        Otherwise it will be cleared during pwr meas callback.*/
        idle_gas_id =  check_gas_id(idle_gas_id);
        if (! IS_IDLE_EV_PWR_MON_SCHEDULED(idle_gas_id))
        {
           MSG_GERAN_HIGH_1_G("GL1 GBTA IDLE Power monitor abandoned , Transfer Sub going to Idle. idle_sub %d",idle_gas_id);

           /* Reset , otherwise when the same sub goes in transfer it would start scheduling the Mon when when not required */        
           grm_notify_event(GBTA_EV_NONE,FALSE,idle_gas_id);
           gbta_go_idle = TRUE;
        }
        else
        {
           gbta_go_idle = FALSE; 
           MSG_GERAN_HIGH_1_G("GL1 GBTA IDLE Power monitor in progress , Transfer Sub going to Idle. idle_sub %d",idle_gas_id);
        }
      }
#endif /* FEATURE_GPRS_GBTA */

      /*Check if GL1 is stuck in this state because of unfinished IRAT activity*/
      gl1_check_if_irat_state_stuck(gas_id);

      /* Read Value of abort call_backs from isr store */
      gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
      /* Check to see if we can abort the EGPRS TBF. */
      if (transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS)
      {
        if (!gl1_msg_dm_complete(gas_id))
        {
          abort_tbf = FALSE;
        }
        else
        {
          abort_tbf = TRUE;
        }
      }

#ifdef GPL1_DM_DEBUG
      #error code not present
#endif
#endif /*(FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)*/

      /*
        Check to see if the SCE has aborted all operations and if a valid abort FN
        has been reached on this frame. If so then the abort is complete and L1 can
        transition to IDLE
      */
      /* Make sure for EDGE clock is not switched Off until DM complete.       */
      /* Check if wtr hop is in progress on current gas_id. Transfer           */
      /* mode can not move to idle while there is a hop in progress            */
      if ((sce_idle[gas_id] == TRUE) &&
          (transfer_data_ptr->abort_fn != L1_GPRS_INVALID_FN) &&
#ifdef FEATURE_GPRS_GBTA
          (gbta_go_idle == TRUE)  &&
#endif 
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_get_gas_to_hop() != gas_id &&
#endif
          (IS_PDCH_CHAN_ABORTED[transfer_data_ptr->abort_fn] == frame_counters[gas_id].FNmod13) &&
           abort_tbf == TRUE)
      {
        abort_pending[gas_id] = FALSE;

        /*  If measurements have been taken in transfer, update the 51 idle value.
            this overcomes the problem with being in transfer for along time and
            the idle serving cell measurement being out of date.  */
        if ((l1_running_avg_get_avg(&(gprs_serv_cell_meas_ptr->rla_p_buffer))))
        {
          l1_serving_cell_meas_ptr->rx_power = l1_running_avg_get_avg(&(gprs_serv_cell_meas_ptr->rla_p_buffer));
          l1_serving_cell_meas_ptr->no_of_measurements = 1;
        }

        abort_once_only[gas_id] = TRUE;
        idle = TRUE;
        sce_idle[gas_id] = FALSE;

        /* L1 leaves packet transfer mode after transmitting the
         * Packet Control Acknowledgement hence it is safer to
         * empty the rrbp queue on dropping back into idle as
         * opposed to when starting packet transfer.
         */
        gpl1_gprs_reset_rrbp_enteries(gas_id );

        /* Returning back to idle, so if we are doing an EGPRS TBF then turn EGPRS off */
        if (l1_transfer_data[gas_id].egprs_or_gprs_tbf == TBF_MODE_EGPRS)
        {
          /* Disable EGPRS mode and deregister EDGE CLK */
          gl1_msg_set_egprs_mode(FALSE,TRUE,gas_id);
          g11_hw_egprs_mcpm_change(FALSE,gas_id);
        }

        gpl1_transfer_service_pending_sysinfo_reqs(gas_id );

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

        decrement_fn_by_one[gas_id]  = FALSE;

        /* Remove the queued message */
        gpl1_remove_mac_l1_delete_ul_ctrl_msg( gas_id );

#endif
#endif

      }

    }

  } /* end of mode stop */

  /* Tell the L1 ISR to send the release confirmation
   * msg to MAC since the waiting period ( ~10 tdma
   * frames ) passed following PCA/PDAN transmission
   * tasking to NPL1.
   *
   * Allow for the case where the pca fn occurs 1 tdma
   * frame after the start time of a high->low camp
   * timeslot assignment. This is done by checking
   * for the long frame in the radio block gap.
   */
  if( ( FALSE != transfer_data_ptr->wait_for_pca_valid )
    &&( ( FN == transfer_data_ptr->wait_for_pca_fn )
      ||( FN == ADD_FN( transfer_data_ptr->wait_for_pca_fn, 1 ) )
      )
    )
  {
    transfer_data_ptr->pca_sent      = TRUE;
   l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

      switch(l1_transfer_data[gas_id].release_pending )
      {
          case L1_FULL_RELEASE_UL:
          case L1_PARTIAL_RELEASE_UL:

              /* Reset the UL ST */
             l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
              break;

          default:
              /* No action required here */
              break;
      } /* End of switch statement */
  }

  /* Send Frame tick notification to NPL1 */
  gpl1_gprs_transfer_send_ftn(
                             &pacch_pdtch_ul_blk_start_cnt,
                             &pacch_pdtch_dl_blk_start_cnt,
                             mode, gas_id );
  /*  Tickign the PCHXFER scheduling function every frame in transfer mode, so that
   *  we schedule PCH as soon as we are done with the previous one. Earlier it was
   *  ticked only 1 frame before Idle frame.
   */
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  gpl1_schedule_pch_in_transfer(gas_id);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

  /* During the radio block period preceding the IDLE frame,
   * need to wait till the transmit callback has been trigerred
   * NPL1 so as to convey the transmit timeslot information to
   * the neighbour cell scheduling routines.
   *
   * This needs to happen on the second tdma frame of the block
   * since this is the frame at which transmit callback happens
   * and at which the voting on the next neighbour cell to be
   * acquired occurs.
   *
   * Note: Order is vital, must happen after the frame tick
   * notification to the lower layers. This funcionality is
   * extracted out of the gpl1_gprs_measurements( ) function.
   */
  switch( frame_counters[gas_id].FNmod52 )
  {

  /*Change order of cases 9/35 and 21/47 to avoid clock boosts during 9/35 case*/
  
  /*1 frames before calling trans tick, check if next index to be scheduled is LTE one*/ 
  case 21:
  case 47:
   {
     if (mode == L1_MODE_RUN)
     {
         /*Check if Gl1 is going to schedule G2L search at next frame*/
         l1_sc_check_if_next_cm_search_is_lte(gas_id);
     }
   }
   break;

#ifdef FEATURE_G2X_TUNEAWAY
  case 9:
  case 35:
     if ( !gl1_msg_get_multi_sim_mode() )
     {
        /* Do nothing here in single sim mode when FNmod52 equals 9 or 35 */
       break;
     }
      /* fall through */
#endif

  case 22:
  case 48:
    {
      if (mode == L1_MODE_RUN)
      {
        if (!stop_sc_tick_trans[gas_id])
        {
#ifdef FEATURE_G2X_TUNEAWAY
         if ( gl1_msg_get_multi_sim_mode() )
         {
            if( gl1_check_any_other_subs_l1_state( gas_id, L1_SUSPEND_MODE ) )
            {
              MSG_GERAN_HIGH_0_G("Don't Abort TBF in GtoW mode");
              /*Check if MVCS boost req for G2L sent in the last frame, if yes sent de-boost now as no G2X will happen on the next idle frame*/
              l1_sc_check_for_g2x_mcvs_deboost_req(gas_id);
            }
            else
            {
              gpl1_gprs_sc_trans_tick(gas_id);
            }
         }else
#endif
         {
           gpl1_gprs_sc_trans_tick(gas_id);
         }

        } else
        {
          /* Need to preserve prediction of next NCELL SCH */
          gpl1_gprs_predict_next_sch(gas_id);
          /*Check if MVCS boost req for G2L sent in the last frame, if yes sent de-boost now as no G2X will happen on the next idle frame*/
          l1_sc_check_for_g2x_mcvs_deboost_req(gas_id);
          MSG_GERAN_MED_1_G("SCE stopped [%d]",gl1_get_FN( gas_id ));
        }

      /*Below check allows GBTA for scenario when transfer init happened
         *before other G sub actualy moved from INACTIVE_MODE to NULL/Idle but 
         *moved to IDLE after that*/
#ifdef FEATURE_GPRS_GBTA
      if( gl1_msg_get_multi_sim_standby_mode() &&
          IS_GBTA_INACTIVE() && 
          GBTA_ALLOWED_CAN_BE_SET(gas_id) &&
          gl1_gbta_get_nv_gbta_support(gas_id) &&
          (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) &&
	  (!gl1_drdsds_enabled(OTHER_GAS_ID(gas_id))) // TRM now has tech specfic dr status
        )
      {
        SET_GBTA_STATUS(GRM_GBTA_ALLOWED);

        /* For this SUB we are not interested in IDLE state vars */
        grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);

        MSG_GERAN_MED_0_G("GL1 GBTA : GBTA ALLOWED other G sub came out of INACTIVE_MODE");
      }
#endif  /*FEATURE_GPRS_GBTA */

      }
    }
    break;

  case 23:
  case 49:

    /* Reset flag to activate G2X  */
    /* Reason for resetting flag here is functions which schedules G2X activity
       l1_sc_wcdma_xfer_ready called multiple times at FN = 22 */
    /* FNmod26 =22, GL1 schedules IRAT, reinitialise l1_tds_modified_ts_fn on next frame*/

       l1_sc_set_skip_g2x_pkt_txfr(FALSE,gas_id);  /* allow G2X after TS is reconfigured */

    break;
    default:
    break;
    }

  /* @@Warning: This is temporary validity check to track the issue of
   * FNmod13 counter being out of step with the absolute frame counter.
   * This check should be removed once the issue is resolved.
   */
  if( frame_counters[gas_id].FNmod13 != (FN % 13) )
  {
    /* Report an error since transfer would be running on the
     * wrong radio block boundaries.
     */
    MSG_GERAN_ERROR_3_G("FNmod13 %u out stepped abs FN %u modulo 13. FNmod52 %u",
              frame_counters[gas_id].FNmod13,
              FN,
              frame_counters[gas_id].FNmod52 );
  }

  if ( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
  {
    /* Local fn value store, used to help identify a long frame
     * due to the lowest dl timeslot decreasing.
     */
    static uint32  previous_fn[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(GPL1_INVALID_FN) };

    /* Make sure a valid DL TBF is currently active */
    if( NULL != transfer_data_ptr->dl_tbf_ptr )
    {
      /* Take a copy of the downlink tbf timeslot allocation */
      const uint8  dl_timeslot_alloc =
        transfer_data_ptr->dl_tbf_ptr->ts_allocation;

      /* Inform the queued nodes of the frame tick.
       * Check if any of them are due to be converted
       * quality reports on this frame.
       * Warning: Use the function call to get the latest FN
       * value incase this is a long frame since FN
       * variable is set before any potential timing
       * changes take effect.
       */
      gpl1_egprs_search_qual_meas_queue( dl_timeslot_alloc, gas_id );

      if ( 2 == SUB_FN( FN, previous_fn[gas_id] ) )
      {
        /* This is a long frame because the timing
         * device omitted a frame interrupt.
         * Simulate the lost frame interrupt by
         * searching the queue again.
         */
        gpl1_egprs_search_qual_meas_queue( dl_timeslot_alloc , gas_id);
      }
    }
    else
    {
      /* Prevent EPDAN requests, from the previous DL TBF,
       * from being generated incase a new DL TBF is assigned.
       */
      gpl1_egprs_empty_qual_meas_queue(gas_id );
    }

    if ( FALSE != idle )
    {
      /* We are leaving packet transfer mode,
       * configure the qual meas module to switch off
       * the bep and interference measurements.
       */
      gpl1_egprs_serv_cell_qual_meas_reset(gas_id );

      /* Clear any possibly remaining queued quality
       * measurement nodes.
       */
      gpl1_egprs_empty_qual_meas_queue(gas_id );

      /* Need to make sure to avoid searching twice
       * on normal-length frame
       */
      previous_fn[gas_id] = GPL1_INVALID_FN;

      /* Turn OFF Incremental Redundancy when
       * leaving egprs pkt transfer.
       */
      gl1_msg_IR_enable( FALSE,gas_id );

      /* Reset the FULL BSN range for DL IR */
      gl1_msg_IR_clear_memory(0,2048,gas_id);

      MSG_GERAN_HIGH_1_G("IR OFF %d mem cleared",FN);

      transfer_data_ptr->rlc_mode = RLC_MODE_INVALID;
    } else
    {
      /* Update copy of current fn for comparison
       * purposes in the frames to follow.
       */
      previous_fn[gas_id] = FN;
    }
  }

  if ( FALSE != idle )
  {

    /* Reset the loopback test mode */
    switch( transfer_data_ptr->l1_test_mode )
    {
    case  L1_TEST_MODE_OFF:
    {

    }
      break;

    case  L1_TEST_MODE_SRB_ON:
    {
        gl1_msg_pdch_set_loopback( FALSE,gas_id  );

    }
    /*lint -fallthrough*/
    case  L1_TEST_MODE_SRB_OFF:
    case  L1_TEST_MODE_B:
    case  L1_TEST_MODE_A:
    {
        gpl1_l1_test_mode_stop_ind(gas_id );

        /* Resume FCCH and SCH activity for test modes */
        stop_sc_tick_trans[gas_id] = FALSE;
    }
      break;

    default:
    {
      /* Invalid l1 test mode value
       * printout debug error msg.
       */
      MSG_GERAN_ERROR_1_G( "Invalid l1 test mode %u",
                 transfer_data_ptr->l1_test_mode );
    }
      break;

    } /* End of switch statement */

    transfer_data_ptr->l1_test_mode = L1_TEST_MODE_OFF;
    transfer_data_ptr->dynamic_tbf_ptr->test_mode = TEST_MODE_OFF;

    MSG_GERAN_MED_1_G("Test mode OFF FNmod13 %u",frame_counters[gas_id].FNmod13);

    /* Required as during reconfiguration as both structures are used */
   l1_transfer_data[gas_id].dynamic_tbf_1.test_mode = TEST_MODE_OFF;
   l1_transfer_data[gas_id].dynamic_tbf_2.test_mode = TEST_MODE_OFF;

   memset(&l1_transfer_data[gas_id].dynamic_tbf_1.usf_value[0],  GPL1_INVALID, (sizeof(uint8)*GL1_DEFS_SLOTS_IN_FRAME));
   memset(&l1_transfer_data[gas_id].dynamic_tbf_2.usf_value[0],  GPL1_INVALID, (sizeof(uint8)*GL1_DEFS_SLOTS_IN_FRAME));
   memset(&transfer_data_ptr->dynamic_tbf_ptr->usf_value[0],     GPL1_INVALID, (sizeof(uint8)*GL1_DEFS_SLOTS_IN_FRAME));
  }


  if(trans[gas_id].tick_wcdma)
  {
    l1_sci_tick_receive(gas_id);
    l1_sci_tick_wcdma_id_in_tbf(30,gas_id);
  }

  /* End of each block period send an indication to GRLC */
  switch (frame_counters[gas_id].FNmod52)
  {
    case  3:
    case  7:
    case 11:
    case 16:
    case 20:
    case 24:
    case 29:
    case 33:
    case 37:
    case 42:
    case 46:
    case 50:
      grlc_send_block_end_ind(gas_id);
      break;

    default:
      break;
  }

  return(idle);
} /* end of function */



/*===========================================================================

FUNCTION  gpl1_gprs_measurements

DESCRIPTION
  This function controls neighbour cell measurements in dynamic transfer mode.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_measurements(l1_mode_command_T mode, gas_id_t gas_id)
{
  uint8                                    num_pwr_meas_to_schedule;
  uint8                                    num_pwr_meas_per_frame;
  static sint7                             meas_in_prog[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };
  boolean                                  ncell_meas_fixed = FALSE;
  boolean                                  pwr_meas_scheduled = FALSE;
  transfer_data_T*                         transfer_data_ptr = &l1_transfer_data[gas_id];
  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);

  switch (frame_counters[gas_id].FNmod52)
  {

  case 3:   /* Schedule for Block 1 */
  case 7:   /* schedule for Block 2 */
  case 12:  /* schedule for Block 3 */
  case 16:  /* schedule for Block 4 */
  case 20:  /* schedule for Block 5 */
  case 25:  /* schedule for Block 6 */
  case 29:  /* schedule for Block 7 */
  case 33:  /* schedule for Block 8 */
  case 38:  /* schedule for Block 9 */
  case 42:  /* schedule for Block 10 */
  case 46:  /* schedule for Block 11 */
  case 51:  /* schedule for Block 0 */
    {
      /* Don't schedule Power monitors if TBF SUSPENDED, otherwise read and write pointers
       * in l1_sc_pwr.c will get out of step
       * If we are in Test mode then schedule power measurements as SCELL measurements are needed
       * for C value calulations
       */
      if ( ( (mode == L1_MODE_RUN) && (stop_sc_tick_trans[gas_id]== FALSE)) ||
           ( (mode == L1_MODE_RUN) && (transfer_data_ptr->l1_test_mode != L1_TEST_MODE_OFF) ))
      {
        /*****************************************************************************************
        * schedule on the idle or PTCCH frame for measurements to start one frame
        * afer the idle or PTCCH and stop one frames before the idle or PTCCH frame. upto 11
        * measurements but only one per frame in transfer.
        *****************************************************************************************/
        if( ( 12 == frame_counters[gas_id].FNmod13 )
          &&( 0 == meas_in_prog[gas_id] ) )
        {
            /* 7 NCELL power monitors and 3 BCCH power mons */
            num_pwr_meas_to_schedule = L1_SC_TRFR_MAX_MEAS_GBTA;

          num_pwr_meas_per_frame = 1;
          l1_sci_monscan_do_rxlev_trans(num_pwr_meas_to_schedule, num_pwr_meas_per_frame, gas_id );
          meas_in_prog[gas_id] = (sint7) num_pwr_meas_to_schedule;
          MSG_GERAN_LOW_2_G("power measurenents sceduled %d %d",num_pwr_meas_to_schedule,num_pwr_meas_per_frame);
          pwr_meas_scheduled = TRUE;
        }

        meas_in_prog[gas_id] = (meas_in_prog[gas_id]-4);

        if (meas_in_prog[gas_id] <= 0)
        {
          meas_in_prog[gas_id] = 0;
        }
      }

      break;
    } /* end of case */

  case 23:
  case 49:
    {
      if (mode == L1_MODE_RUN)
      {
        if (!stop_sc_tick_trans[gas_id])
        {
          ncell_meas_fixed = gpl1_gprs_meas_mode_ptr->load_handler;
          if (ncell_meas_fixed)
          {
            l1_sci_tick_receive(gas_id);
          }
        }else
        {
          MSG_GERAN_MED_1_G("SCE stopped [%d]",gl1_get_FN( gas_id ));
        }
      }
      break;

    }

  default:
    break;
    /* do nothing */

  } /* end of switch */

  /* return flag indicating if any power measurements are scheduled or not */
  return(pwr_meas_scheduled);
}


/*===========================================================================

FUNCTION gpl1_gprs_tbf_susp_range

DESCRIPTION
  A look-up table to return the frame range for the NCELL BCCH decode gap

  ENAHNCEMENT : 26 march 2010.
  Reduction in the TBF suspension window under different boundary conditions.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_tbf_susp_range ( uint32 multiframe_start,
                                uint16 frame_index,
                                tbf_suspend_T *ncell_frame_struct,
                                uint8  bcch_fn_mod52, gas_id_t gas_id)
{
  /* find block number of next NCELL BCCH */
  const uint32  block_num      = (uint32) GPL1_FN_MOD52_TO_BLK[ bcch_fn_mod52 ];

  /* block start frame = mf start + (4 * block_num) + (block_num / 3) */
  uint32  block_starting_frame =
      ADD_FN( multiframe_start,
              ( ( block_num << 2 ) + ( GPL1_MF52_BLK_NUM_DIV_3[ block_num ] ) ) );

  ncell_frame_struct->block_num = (uint8) block_num;

  MSG_GERAN_HIGH_3_G("GL1 multiframe_start=%d,frame_index=%d,bcch_fn_mod52=%d",
            multiframe_start,frame_index,bcch_fn_mod52);

  MSG_GERAN_HIGH_2_G("GL1 block_num=%d,block_starting_frame=%d",
            block_num,block_starting_frame);

  switch( block_num )
  {
  case 2:
  case 5:
  case 8:
  case 11:
    {
      switch (frame_index)
      {
      case 0:
      case 1:
        /* Idle and PTCCH frame reports the block number of the previous
           block and frame_index 0, but TBF on a different set of three blocks */
        if (bcch_fn_mod52 == 12 || bcch_fn_mod52 == 25 || bcch_fn_mod52 == 38 || bcch_fn_mod52 == 51)
        {
          /*
          ** block_starting_frame coressponds to start current block frame
          ** already margin of 4 frames available before ncell_bcch_fn
          */
          ncell_frame_struct->start_frame_num = block_starting_frame;
        }
        else
        {
          ncell_frame_struct->start_frame_num = SUB_FN(block_starting_frame, 4);
        }

        if(!trans[gas_id].wcdma_cell_id_sceduled)
        {
          ncell_frame_struct->number_of_frames = 13;
        }
        else
        {
          ncell_frame_struct->number_of_frames = 20 + 4;
        }

        break;
      case 2:
      case 3:
        {
        /*
        ** Have 2 frame boundary hence no need to go back one block 4 frames
        */
        ncell_frame_struct->start_frame_num = block_starting_frame;

        if(!trans[gas_id].wcdma_cell_id_sceduled)
        {
          ncell_frame_struct->number_of_frames = 9;
        }
        else
        {
          ncell_frame_struct->number_of_frames = 20 + 4;
        }
        }
        break;

      default:
        /* All Other frame index's not handled for block num's 2,5,8 & 11 */
        break;
      }

      break;
    }

  case 0:
  case 3:
  case 6:
  case 9:
    {
      switch (frame_index)
      {
      case 0:
        {
        ncell_frame_struct->start_frame_num = SUB_FN(block_starting_frame, 5);

        if(!trans[gas_id].wcdma_cell_id_sceduled)
        {
          ncell_frame_struct->number_of_frames = 9 + 4;
        }
        else
        {
          ncell_frame_struct->number_of_frames = 20 + 4;
        }
        }
        break;

      case 1:
      case 2:
      case 3:
        {
        /*
        ** Have 2 frame  diffence due to ptcch frame , hence nogoing back one block period
        */
        ncell_frame_struct->start_frame_num = SUB_FN(block_starting_frame, 1);

        if(!trans[gas_id].wcdma_cell_id_sceduled)
        {
          /*
          ** if the qsymbols exceeds 3750 symbols duration , mdsp has not finsihed post
          ** processing of previous block and hence needs 2 slots duration for switchover,
          ** hence allow one extra block duration.
          */
          if (frame_index == 3)
          {
            ncell_frame_struct->number_of_frames = 13;
          }
          else
          {
            /*
            ** Enough time 2frames for mdsp to switch rx /tx
            */
            ncell_frame_struct->number_of_frames = 9;
          }
        }
        else
        {
          ncell_frame_struct->number_of_frames = 20 + 4;
        }
        }
        break;
      default:
        /* All Other frame index's not handled for block num's 0,3,6 & 9 */
        break;
      }
      break;
    }

  case 1:
  case 4:
  case 7:
  case 10:
    {
      switch (frame_index)
      {
      case 0:
      case 1:
        {
        /*
        ** allow enough 4 frames for before read ncell_bcch
        */
        ncell_frame_struct->start_frame_num = SUB_FN(block_starting_frame, 4);

        if(!trans[gas_id].wcdma_cell_id_sceduled)
        {
          /*
          ** suspend 1 frame less not 13 , to allow ptcch/meas read
          */
          ncell_frame_struct->number_of_frames = 12;
        }
        else
        {
          ncell_frame_struct->number_of_frames = 20 + 4;
        }
        }
        break;

      case 2:
      case 3:
        {
        ncell_frame_struct->start_frame_num = block_starting_frame;

        if(!trans[gas_id].wcdma_cell_id_sceduled)
        {
         if (frame_index == 2)
         {
          /*
          ** suspend 1 frame less not 9 , to allow ptcch/meas read
          */
           ncell_frame_struct->number_of_frames = 8;
         }
         else
         {
           ncell_frame_struct->number_of_frames = 9;
         }
        }
        else
        {
          ncell_frame_struct->number_of_frames = 20 + 4;
        }
        }
        break;
      default:
        /* All Other frame index's not handled for block num's 1,4,7 & 10 */
        break;
      }
      break;
    }

  default:
    /* Since there are only 12 blocks this case should not happen */
    MSG_GERAN_ERROR_1_G("Incorrect block number %d",ncell_frame_struct->block_num);

  }   // switch

#ifdef FEATURE_G2X_TUNEAWAY
    {
      //CR687349: Guarentee at least one frame eariler before start check TBF suspension for G2W TA case.
      uint32 pch_suspension_FN = (multiframe_start + bcch_fn_mod52);
      if(SUB_FN(pch_suspension_FN, ncell_frame_struct->start_frame_num) < (SUSPENSION_FRAME_DELAY + GUARD_FRAME_TBF_RECONF_OR_REL))
      {
        MSG_GERAN_HIGH_2_G("gpl1_gprs_tbf_susp_range:Update susp startFN:%d, pch_susp_FN:%d", ncell_frame_struct->start_frame_num, pch_suspension_FN);
        ncell_frame_struct->start_frame_num = SUB_FN(ncell_frame_struct->start_frame_num, 4); //Make it align the block boundary
        ncell_frame_struct->number_of_frames += 4;//Accordingly update the TBF suspended frames
      }
    }
#endif
}

/*===========================================================================

FUNCTION gpl1_gprs_check_tbf_suspension

DESCRIPTION
  Checks whether TBF suspension can happen.

DEPENDENCIES
  ncell_bcch structure
  tbf_suspend structure
  transfer_data_ptr structure
  ptcch_ul_lookup and ptcch_dl_lookup

RETURN VALUE
  allowed_to_go

SIDE EFFECTS
  None

===========================================================================*/
boolean  gpl1_gprs_check_tbf_suspension( uint32   start_frame,
                                         uint8    no_of_frames,
                                         boolean  check_ptcch,
                                         gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    /* Setup pointer to access the current transfer data parameters */
    transfer_data_T*  transfer_data_ptr =
        l1_tskisr_blk->current_params.L1Data.pTransfer_data;

    /* Copy in the tbf suspension strt/end frames locally */
    uint32  tbf_suspend_end = ADD_FN( start_frame, no_of_frames);

    /* Initialise the allowed to go flag */
    boolean  allowed_to_go = TRUE;

    /* Check if the TBF suspension period contains a PTCCH UL logical
     * channel, in which case this suspension is cancelled.
     */
    if( FALSE != check_ptcch )
    {
        allowed_to_go = gpl1_gprs_find_next_tai_during_suspension( start_frame,
                                                                   tbf_suspend_end , gas_id);
    }

    /* Check pending uplink assignment */
     if(  ( transfer_data_ptr->ul_st != L1_GPRS_INVALID_FN )
         &&( IS_FRAME_NUM_LATER_OR_EQUAL( transfer_data_ptr->ul_st, start_frame ))
         &&( IS_FRAME_NUM_LATER_OR_EQUAL(tbf_suspend_end, transfer_data_ptr->ul_st )))
    {
        allowed_to_go = FALSE;

        MSG_GERAN_HIGH_3_G("apprchin ul assign blks tbf susp FN=%d start_frame=%d tbf_suspend_end=%d",
                  gl1_get_FN( gas_id ),start_frame,tbf_suspend_end );
    }

    /* Check pending downlink assignment */
    if(  ( transfer_data_ptr->dl_st != L1_GPRS_INVALID_FN )
         &&( IS_FRAME_NUM_LATER_OR_EQUAL( transfer_data_ptr->dl_st, start_frame ))
         &&( IS_FRAME_NUM_LATER_OR_EQUAL(tbf_suspend_end, transfer_data_ptr->dl_st )))
    {
        allowed_to_go = FALSE;

        MSG_GERAN_HIGH_3_G("apprchin dl assign blks tbf susp FN=%d start_frame=%d tbf_suspend_end=%d",
                  gl1_get_FN( gas_id ),start_frame,tbf_suspend_end );
    }

    /* Check any pending tbf release */
    switch( transfer_data_ptr->release_pending )
    {
    case  L1_FULL_RELEASE_UL:
    case  L1_FULL_RELEASE_DL:
    case  L1_FULL_RELEASE_UL_DL:

      MSG_GERAN_HIGH_1_G( "tbf susp stopped release pending %d",
                transfer_data_ptr->release_pending );

      allowed_to_go = FALSE;
      break;

    default:
        /* No action to be taken here
         * Don't set the flag to TRUE.
         */
        break;
    }

    /* Return value to client */
    return  allowed_to_go;
}

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
/*===========================================================================

FUNCTION gpl1_gprs_init_tuneaway_and_ncell_data

DESCRIPTION
  This function initialises tuneaway and ncell variables.

DEPENDENCIES
  stop_sc_tick_trans
  gpl1_gprs_meas_mode
  gprs_serv_cell_meas
  ncell_bcch_fn
  tbf_suspend

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_init_tuneaway_and_ncell_data(gas_id_t gas_id)
{
  transfer_data_T                         *transfer_data_ptr;
  grm_client_enum_t        client_id;
  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* assign shortcut to general transfer_data_params */
  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  client_id = l1_tskisr_blk->client_id;

  if(trans[gas_id].tuneaway_and_ncell_data_initialized == FALSE && l1_tskisr_blk->main_command != L1_IDLE_COMMAND)
  {
    /* Restart calls to gpl1_gprs_sc_trans_tick() on idle frames */
    stop_sc_tick_trans[gas_id] = FALSE;
    transfer_data_ptr->disable_rx_tx = FALSE;
    gpl1_gprs_meas_mode_ptr->allowed_to_go = FALSE;
  
    gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;
  
    ncell_bcch_fn[gas_id] = 0;
    tbf_suspend[gas_id].block_num = 0;
    tbf_suspend[gas_id].start_frame_num = 0;
    tbf_suspend[gas_id].number_of_frames = 0;
    tbf_suspend_ncell_bcch[gas_id] = FALSE;
  
    MSG_GERAN_HIGH_0_G("DBG DS G TA init_tbf_susp");
  
    trans[gas_id].new_bcch_fn = L1_GPRS_INVALID_FN;
    
#ifdef FEATURE_GPRS_GBTA
    if( gl1_gbta_get_nv_gbta_support(gas_id) &&
        GBTA_ALLOWED_CAN_BE_SET(gas_id) &&
        (gl1_msg_get_multi_sim_standby_mode()) &&
        (!gl1_drdsds_enabled(OTHER_GAS_ID(gas_id)))) // TRM now has tech specfic dr status
    {
      SET_GBTA_STATUS(GRM_GBTA_ALLOWED);

      /* For this SUB we are not interested in IDLE state vars */
      grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);

      MSG_GERAN_MED_0_G("GBTA : GBTA ALLOWED ");
    }
    else
    {
      SET_GBTA_STATUS(GRM_GBTA_NONE);
      MSG_GERAN_MED_2_G("GBTA: GBTA NOT ALLOWED  mode %d gbta allowed %d",
                          gl1_msg_get_multi_sim_standby_mode(),GBTA_ALLOWED_CAN_BE_SET(gas_id));

      /* For this SUB we are not interested in IDLE state vars */
      grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);
    }

    trans[gas_id].gbta_is_tbf_pch_suspension = FALSE;
    trans[gas_id].gbta_current_pch_suspension_FN = L1_GPRS_INVALID_FN;
#endif /* FEATURE_GPRS_GBTA */
  
    
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
  
    /*PCHXFER - Exchange TRM clients as we will use GPRS_X for data related activities
        * and GSM_X for Reserving TRM for PCH. No need to exchange clients if DR_DSDS is enabled*/
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

    trans[gas_id].tuneaway_and_ncell_data_initialized = TRUE;
  }
  else
  {
    MSG_GERAN_HIGH_0_G("Tuneaway and Ncell data already initialized, skipping it");
  }

  if( gl1_msg_get_multi_sim_standby_mode()
  #ifdef FEATURE_SGLTE
     || (gl1_hw_get_sglte_mode(gas_id) && (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))
  #endif
  #ifdef FEATURE_SVLTE_DUAL_SIM
     || (gl1_check_svlte_mode_active() == TRUE)
  #endif
    )
  {
 
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
    #ifdef FEAT_DIFF_SIG_PATH_PER_GSM_BAND
    #ifdef FEATURE_RF_USES_RFM_CONTEXT_INTERFACE
          rfm_context_type rfm_context = gl1_hw_get_any_granted_rfm_context(client_id, gas_id );
    #else
          rfm_path_type rf_path = gl1_hw_get_any_granted_rfm_path(client_id, gas_id );
    #endif /* FEATURE_RF_USES_RFM_CONTEXT_INTERFACE */
    #else
    #ifdef FEATURE_RF_USES_RFM_CONTEXT_INTERFACE
          rfm_context_type rfm_context = gl1_hw_get_gsm_rfm_context(client_id, gas_id );
    #else
          rfm_path_type rf_path = gl1_hw_get_gsm_rf_path(client_id, gas_id );
    #endif /* FEATURE_RF_USES_RFM_CONTEXT_INTERFACE */
    #endif
        /* @@TODO IF/ELSE cases below are identical but retained until sglte+g is cleaned up */
    #ifdef FEATURE_RF_USES_RFM_CONTEXT_INTERFACE
          if (gl1_hw_get_valid_rfm_device(rfm_context, gas_id) != RFM_DEVICE_0)
    #else
          if (rf_path.rfm_device != RFM_DEVICE_0)
    #endif /* FEATURE_RF_USES_RFM_CONTEXT_INTERFACE */
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
      grm_change_priority( client_id,
                           grm_get_connection_priority(GRM_PS_CONNECTION,gas_id),
                           gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id),
                           gas_id );
    }
  #if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     grm_retain_lock( client_id,
                     (grm_unlock_callback_t)&grm_gta_preemption_callback,
                      gas_id );
   
     /* Need to regitster an abort callback with TRM. This allows any booking by any other client to
        invoke this callback and pass the time when the booking is required */
     MULTI_SIM_FW_LOAD_FRAMES = MULTI_SIM_FW_LOAD_FRAMES_GPAGE_GTA;
  #endif
  }

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif

}


/*===========================================================================

FUNCTION gpl1_gprs_reset_tuneaway_and_ncell_data

DESCRIPTION
  This function resets the TA and ncell initialized flag variables.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_gprs_reset_tuneaway_and_ncell_data(gas_id_t gas_id )
{
  trans[gas_id].tuneaway_and_ncell_data_initialized = FALSE;
  MSG_GERAN_HIGH_0_G("reset_tuneaway_and_ncell_data");
}

#else

/*===========================================================================

FUNCTION gpl1_gprs_init_tbf_suspension

DESCRIPTION
  This function initialises TBF suspension variables.

DEPENDENCIES
  stop_sc_tick_trans
  gpl1_gprs_meas_mode
  gprs_serv_cell_meas
  ncell_bcch_fn
  tbf_suspend

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_init_tbf_suspension(gas_id_t gas_id)
{
  transfer_data_T                         *transfer_data_ptr;
  gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* assign shortcut to general transfer_data_params */
  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  /* Restart calls to gpl1_gprs_sc_trans_tick() on idle frames */
  stop_sc_tick_trans[gas_id] = FALSE;
  transfer_data_ptr->disable_rx_tx = FALSE;
  gpl1_gprs_meas_mode_ptr->allowed_to_go = FALSE;
//    l1_sc_globals_ptr->schedule_next_read = FALSE;

  gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;

  ncell_bcch_fn[gas_id] = 0;
  tbf_suspend[gas_id].block_num = 0;
  tbf_suspend[gas_id].start_frame_num = 0;
  tbf_suspend[gas_id].number_of_frames = 0;
  tbf_suspend_ncell_bcch[gas_id] = FALSE;

  MSG_GERAN_HIGH_0_G("DBG DS G TA init_tbf_susp");

  gpl1_gprs_ds_init_pch_suspension(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif


}

#endif /*# FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
/*===========================================================================

FUNCTION gpl1_gprs_restart_ncell_sch

DESCRIPTION
  This function Restart ncell sch after ncell bcch decode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_restart_ncell_sch(gas_id_t gas_id)
{
  /* Restart ncell sch after ncell bcch decode */
  check_for_rrbp[gas_id] = FALSE;
  stop_sc_tick_trans[gas_id] = FALSE;

}


/* kf time slot slamming */


/*===========================================================================

FUNCTION gpl1_gprs_ts_conv

DESCRIPTION
  Converts an Absoloute Timeslot vale and returns the relative timeslot
  based on the camped current timeslot
DEPENDENCIES

unslammed_tn - Absoloute timeslot value

RETURN VALUE

 slammed_tn - relative timeslot value

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_ts_conv(uint8 un_slammed, gas_id_t gas_id )
{
  uint8 slammed = 0;
  uint8 curr_ts = l1_get_current_timeslot(gas_id);

  if ((un_slammed >= GL1_DEFS_SLOTS_IN_FRAME || curr_ts >= GL1_DEFS_SLOTS_IN_FRAME))
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_2_G("Pdtch timeslots not in range -un_slammed:%d, curr_ts:%d",un_slammed,curr_ts);
  }
  /* account for un_slammed_tn > current_tn */
  slammed = ( ((un_slammed - curr_ts) + GL1_DEFS_SLOTS_IN_FRAME) %
                    GL1_DEFS_SLOTS_IN_FRAME);

  return slammed;
}
/*===========================================================================

FUNCTION gpl1_gprs_ts_conv_unslam

DESCRIPTION
 Converts a slammed relative timeslot based from the camped timeslot
 to the absoloute timeslot value

DEPENDENCIES

slammed_tn - relative timeslot

RETURN VALUE

un_slammed_tn - Absoloute timeslot value

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_ts_conv_unslam(uint8 slammed, gas_id_t gas_id )
{
  uint8 un_slammed = 0;
  uint8 curr_ts    = l1_get_current_timeslot(gas_id);

  if ((slammed >= GL1_DEFS_SLOTS_IN_FRAME) || (curr_ts >= GL1_DEFS_SLOTS_IN_FRAME))
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_2_G("Pdtch timeslots not in range -slammed:%d, curr_ts:%d",slammed,curr_ts);
  }

  /* Account for slammed + camped tn > 8 */
  un_slammed = ( (slammed + curr_ts + GL1_DEFS_SLOTS_IN_FRAME) %
                  GL1_DEFS_SLOTS_IN_FRAME);


  return un_slammed;
}


/*===========================================================================

FUNCTION gpl1_dynamic_data_init

DESCRIPTION
  Resets any flags and variables critical to dynamic transfer operation.

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void gpl1_dynamic_data_init( gas_id_t gas_id  )
{
  abort_once_only[gas_id] = TRUE;

  /* Reset the downlink start time variable */
 l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

 /* Make sure these are reset */
 l1_transfer_data[gas_id].dynamic_tbf_1.ul_tfi_st = L1_GPRS_INVALID_FN;
 l1_transfer_data[gas_id].dynamic_tbf_2.ul_tfi_st = L1_GPRS_INVALID_FN;

  /* Make sure these are reset */
 l1_transfer_data[gas_id].dl_tbf_1.dl_tfi_st = L1_GPRS_INVALID_FN;
 l1_transfer_data[gas_id].dl_tbf_2.dl_tfi_st = L1_GPRS_INVALID_FN;

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM
    decrement_fn_by_one[gas_id]  = FALSE;
#endif
#endif

}

#ifdef FEATURE_GPRS_PS_HANDOVER  // scope error fix
/*===========================================================================

FUNCTION gpl1_gprs_psho_AB_on_pacch

DESCRIPTION
  Schedules Access bursts on PACCH
DEPENDENCIES

RETURN VALUE

===========================================================================*/
static void gpl1_gprs_psho_AB_on_pacch( frequency_information_T*  freq_info_ptr,
                                        uint8                     ul_timeslot,
                                        uint8                     access_burst_type,
                                        int16                     txlev, gas_id_t gas_id
                                        )
{
  gl1_msg_ul_pacch_pdtch_signal_type  signal_info;
  gl1_defs_coding_type                coding_scheme = GL1_DEFS_ACCESS_CODING;
  gl1_defs_tx_pkt_data_type           AB_data;

  ARFCN_T                             arfcn[4];

  gpl1_gprs_calculate_arfcns( &(arfcn[0]),
                              freq_info_ptr,
                              NORM ,gas_id);

  MSG_GERAN_HIGH_1_G("Send AB on PACCH burst type %d", access_burst_type);

  /*  Set up coding scheme, cheat EIGHT_BIT = 0 */
  if( 0 != access_burst_type )
  {
    coding_scheme = GL1_DEFS_EXT_ACCESS_CODING;
  }

  /* Load the handover reference */
  AB_data.gprs.data = &gpl1_psho_data.handover_reference;

  /* Ensure the timeslot passed in is in the rane 0 - 7 */
  signal_info.tn = (gl1_defs_tn_type) SUB_MOD( ul_timeslot, l1_get_current_timeslot(gas_id), 8 );

  /*  Power control value, just satisfy for now */
  signal_info.TXLEV = txlev;

  gl1_msg_tx_psho_pacch_pdtch_fixed( (gl1_defs_tx_pkt_data_type *)(&(AB_data)),
                                     &signal_info,
                                     &(arfcn[0]),
                                     coding_scheme,gas_id );
}
#endif

/* Used by the DTM control function to effectively tell dynamic control that
 * the frame counter is currently over incremented by one and needs to be rewinded
 * back.
 */
#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

/*===========================================================================

FUNCTION gpl1_decrement_fn_on_next_tbase_change

DESCRIPTION
  Decrements fn on next timebase change

DEPENDENCIES

RETURN VALUE

===========================================================================*/

void  gpl1_decrement_fn_on_next_tbase_change( gas_id_t gas_id )
{
  /* Set the boolean to TRUE */
  decrement_fn_by_one[gas_id] = TRUE;
}

/* This function searches through the pacch fifo and
 * removes any pending messages.
 * @@WARNING: This is to be called only radio block
 * boundaries.
 */
void  gpl1_pacch_fifo_delete_msg( uint8  csn_msg_type, gas_id_t gas_id )
{
  /* Assign a pointer */
  mac_ul_ctrl_buffer_T* const  mac_ul_pacch_ptr =
   l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr;


  if(  ( 0 != mac_ul_pacch_ptr->pending_msgs )
     &&( FALSE !=l1_transfer_data[gas_id].mac_ul_ctrl_available )
    )
  {
    /* Take a local copy of pending messages for use as a counter */
    uint32  pending_msgs = mac_ul_pacch_ptr->pending_msgs;

    /* Start at the out ptr and end at the in ptr */
    pacch_msg_buffer_T*  search_ptr = mac_ul_pacch_ptr->pacch_data_out_ptr;

    MSG_GERAN_MED_3_G("pend msgs %u srch_ptr 0x%p in_ptr 0x%p", pending_msgs, search_ptr, mac_ul_pacch_ptr->pacch_data_in_ptr );

    /* Search through the buffer */
    while(  ( 0 != pending_msgs )
          &&( search_ptr != mac_ul_pacch_ptr->pacch_data_in_ptr )
         )
    {
      /* Compare csn message types */
      if(  ( ((search_ptr->control_block[ 1 ] & 0xFC)>>2U) == csn_msg_type )
         &&( FALSE != search_ptr->valid )
        )
      {
        /* Remove msg but check its position also since
         * since pacch fifo is designed so that ctrl msg positions
         * in queue are always contiguous.
         *
         * Over-write this block with a dummy ctrl block.
         */
        memscpy( &( search_ptr->control_block[0] ),
                sizeof(search_ptr->control_block),
                &(l1_transfer_data[gas_id].dummy_control_block[0] ),
                SIZE_OF_CONTROL_RADIO_BLOCK );

        /* Print an F3 debug message */
        MSG_GERAN_HIGH_3_G("Del f pacch msg %u on addr 0x%p FNmod13 %u",
                 csn_msg_type,
                 search_ptr,
                 frame_counters[gas_id].FNmod13 );

        /* Delete the msg effectively by setting the valid flag to FALSE */
        search_ptr->valid = FALSE;

        /* Make sure the counters and flags are altered to reflect that */
        if( mac_ul_pacch_ptr->pending_msgs > (1U) )
        {
          mac_ul_pacch_ptr->pending_msgs--;
        }
        else
        {
          /* Set the ctrl data available boolean to FALSE */
         l1_transfer_data[gas_id].mac_ul_ctrl_available = FALSE;

          /* reset the number of pending msgs to zero */
          mac_ul_pacch_ptr->pending_msgs = 0;
        }

        /* Decrement counter */
        if( 0 != pending_msgs )
        {
        pending_msgs--;
        }

      } /* End of if del msg type == search msg type */
      else
      {
        MSG_GERAN_MED_2_G("cmp failure. msg type %u valid %u", csn_msg_type, search_ptr->valid);
      }

      /* Move pointer towards pacch_data_in_ptr */
      if( search_ptr >= &( mac_ul_pacch_ptr->pacch_data[(MAX_NUM_UL_PACCH_MSGS-1)] ) )
      {
        search_ptr = &( mac_ul_pacch_ptr->pacch_data[0] );
      }
      else
      {
        ++search_ptr;
      }

    } /* End of while loop */
  }   /* End of if pending_msgs && mac ul ctrl available */
  else
  {
    MSG_GERAN_HIGH_3_G("No fifo PACCH msgs found %u pend msgs %u avail %u",
             frame_counters[gas_id].FNmod13,mac_ul_pacch_ptr->pending_msgs,l1_transfer_data[gas_id].mac_ul_ctrl_available );
  }
}

#endif
#endif /* FEATURE_GSM_DTM */

/*===========================================================================

FUNCTION gpl1_get_tbf_resumption_fn

DESCRIPTION
  Returns the status of the disable_rx_tx which is indicative of the state
  of the TBF suspension. The pointer *resumption_frame_nbr is populated with
  the resumption frame number if the TBF is currently suspended.

DEPENDENCIES

RETURN VALUE
  TRUE - TBF currently suspended
  FALSE - TBF NOT suspended
  *resumption_frame_nbr - The frame that the TBF will resume.

===========================================================================*/
boolean gpl1_get_tbf_resumption_fn(uint32 *resumption_frame_nbr, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];


  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  gpl1_g2x_ta_priority_t pri;

  if (resumption_frame_nbr)
  {
    if ( l1_tskisr_blk->current_params.L1Data.pTransfer_data->disable_rx_tx )
    {

      if(gpl1_g2x_ta_is_suspension_ongoing(gas_id,&pri))
      {
        *resumption_frame_nbr = ADD_FN( data_ptr ->table[pri].frame_number, data_ptr ->table[pri].duration );
      }

    }
  }
  else
  {
    MSG_GERAN_ERROR_0_G("NULL PTR in gpl1_get_tbf_resumption_fn()");
  }
  return l1_tskisr_blk->current_params.L1Data.pTransfer_data->disable_rx_tx;
}

/*===========================================================================

FUNCTION gpl1_reset_stop_sc_tick_trans

DESCRIPTION
  Resets the boolean variable stop_sc_tick_trans

DEPENDENCIES

RETURN VALUE
===========================================================================*/
void gpl1_reset_stop_sc_tick_trans( gas_id_t gas_id )
{
  if ( stop_sc_tick_trans[gas_id] )
  {
    MSG_GERAN_HIGH_0("stop_sc_tick_trans needs resetting");
  }

  stop_sc_tick_trans[gas_id] = FALSE;
}

/*===========================================================================

FUNCTION gpl1_reset_stop_sc_tick_trans

DESCRIPTION
  Resets the boolean variable stop_sc_tick_trans

DEPENDENCIES

RETURN VALUE
===========================================================================*/
void gpl1_stop_sc_tick_trans( gas_id_t gas_id )
{
  stop_sc_tick_trans[gas_id] = TRUE;
  MSG_GERAN_MED_0_G("stop_sc_tick_trans");


#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_SC_ACTIVITY_STOP);
#endif
}

/*===========================================================================

FUNCTION gpl1_start_sc_tick_trans

DESCRIPTION
  Kicks of PTM SCE.

DEPENDENCIES

RETURN VALUE
===========================================================================*/
void gpl1_start_sc_tick_trans( gas_id_t gas_id )
{
  if(stop_sc_tick_trans[gas_id] == TRUE)
  {
     l1_sci_reset_state_machine(gas_id);
  }
  stop_sc_tick_trans[gas_id] = FALSE;
  MSG_GERAN_MED_0_G("start_sc_tick_trans");
 
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_SC_ACTIVITY_START);
#endif
}
/*===========================================================================

FUNCTION gpl_get_stop_sc_tick_trans

DESCRIPTION
  Resets the boolean variable stop_sc_tick_trans

DEPENDENCIES

RETURN VALUE
===========================================================================*/
boolean gpl1_get_stop_sc_tick_trans(gas_id_t gas_id)
{
  return(stop_sc_tick_trans[gas_id]);
}
#ifdef FEATURE_G2X_TUNEAWAY
/*===========================================================================

FUNCTION gpl1_gprs_unsuspend_tbf

DESCRIPTION This function should be called when exiting suspended tbf


DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_unsuspend_tbf(transfer_data_T *transfer_data_ptr, uint32 tbf_suspend_gta_start_frame, gas_id_t gas_id )
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  uint32 FN = GSTMR_GET_FN_GERAN( gas_id);
  /*Local Copy of curr antenna*/
  int curr_antenna = antenna_to_use_for_tx[gas_id]; 
  /* Allow interference measurements to be scheduled */
  gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;
  /* Allow TBF rx/tx activities again */
  transfer_data_ptr->disable_rx_tx  = FALSE;

  /* stop tx's for first radio block as USF's haven't been detected yet */
  GPL1_DISABLE_TX(gas_id);
    if (l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
    {
      l1_sci_go(gas_id);
    }else
  {
      MSG_GERAN_MED_1_G( "SCE mode changed %d",l1_sc_globals_ptr->mode);
    }

   gpl1_start_sc_tick_trans(gas_id);

  if ( trans[gas_id].tick_wcdma)
      {
     gpl1_gprs_restart_ncell_sch(gas_id);
  }

  trans[gas_id].tick_wcdma = FALSE;

   tbf_suspend_ncell_bcch[gas_id] = FALSE;

#ifdef FEATURE_GPRS_GBTA
    /* Allow GBTA after TBF suspension/resume */
   if ( IS_GBTA_BLOCKED())
        {
     SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
   }
#endif /* FEATURE_GPRS_GBTA */

   MSG_GERAN_MED_3_G( "G2W_TA: TBF RESTART FN = %d GTA Gap %d num_rrbp_msgs = %d",
            FN, SUB_FN( FN, tbf_suspend_gta_start_frame ),gpl1_gprs_get_num_rrbp_msg(gas_id));
#ifdef FEATURE_GSM_QSC_TX_DIV
   /*Restore Traffic asdiv settings on TBF resume*/
   gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXTX, gas_id);

   if (gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE1))
   {
      MSG_GERAN_MED_1_G("GL1_AsDiv: Curr Antenna: %d Query and Reset for TBF ReStart", curr_antenna);
      /*Switch the Global Antenna Var. so that We can switch if different config from TRM*/
      switch (curr_antenna) 
      {
        case 1:
          antenna_to_use_for_tx[gas_id] = 2;
          gl1_hw_switch_antenna_tx_div(gas_id);
          break;

        case 2:
          antenna_to_use_for_tx[gas_id] = 1;
          gl1_hw_switch_antenna_tx_div(gas_id);
          break;

       default:
           MSG_GERAN_ERROR_1_G("Gl1_AsDiv: Invalid Init Antenna Setting for Type1:%d", curr_antenna);
       }
      /*Reset the antenna GLobal Var*/
      antenna_to_use_for_tx[gas_id] = curr_antenna;
   }
    

#endif
}

/*===========================================================================

FUNCTION gpl1_g2x_ta_check_rrbp

DESCRIPTION This function should be called when exiting suspended tbf


DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_g2x_ta_check_rrbp(boolean tdscdma_client, transfer_data_T *transfer_data_ptr,gpl1_g2x_ta_priority_t pri, gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  boolean  abort_tbf_suspension = FALSE;

if ( (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
              (transfer_data_ptr->release_pending == L1_FULL_RELEASE_DL)  ||
              (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL) )
            {
    abort_tbf_suspension = TRUE;

    MSG_GERAN_HIGH_1_G("REL PEND ABORT TBF SUSP %d",transfer_data_ptr->release_pending);
  } else if ( 0 != gpl1_gprs_get_num_rrbp_msg(gas_id ) )
  {
    abort_tbf_suspension =
    gpl1_gprs_rrbp_to_abort_tbf_suspension( tdscdma_client, ADD_FN( data_ptr ->table[pri].frame_number,
                                                    data_ptr ->table[pri].duration ), gas_id );
  }

  /* TBF suspension has started already but needs
   * to be cancelled because either a tbf assignment
   * is due or a tbf release is due or a PCA needs
   * to be transmitted.
            */
  return(abort_tbf_suspension);
}
#endif /* FEATURE_G2X_TUNEAWAY */
/*===========================================================================

FUNCTION gpl1_gprs_get_gta_config

DESCRIPTION


DEPENDENCIES
 None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_get_gta_config(gas_id_t gas_id)
{
  gpl1_g2x_ta_priority_t pri_ongoing;

  if(GERAN_ACCESS_STRATUM_ID_UNDEFINED == gas_id)
    return FALSE;
  else
    return gpl1_g2x_ta_is_suspension_ongoing(gas_id,&pri_ongoing);
}

#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION gpl1_gbta_schedule_idle_pwr_mon

DESCRIPTION
  Schedules Idle pwr monitor for idle Sub in transfer SUB PTCCH Frame.

DEPENDENCIES

RETURN VALUE
===========================================================================*/

void  gpl1_gbta_schedule_idle_pwr_mon(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  gas_id_t idle_gas_id = GERAN_ACCESS_STRATUM_ID_1;
 #ifdef FEATURE_G2X_TUNEAWAY
  gpl1_g2x_ta_priority_t pri=G2X_TA_PRI_RESERVED;
  uint32 next_suspension_FN, next_resume_FN;
#endif /* FEATURE_G2X_TUNEAWAY */


  /*  check if there is a need to schedule Idle Power Moniter
   ** only when allowed and not in ACTIVE  */
  if(IS_GBTA_ALLOWED())
  {
    /* check of Idle event set for other gas_id and not in progress */
    if (  gl1_check_any_other_subs_idle_evt_power_mon(gas_id,&idle_gas_id))
    {
      idle_gas_id =  check_gas_id(idle_gas_id);
      if (! IS_IDLE_EV_PWR_MON_SCHEDULED(idle_gas_id))
      {
        /* check for clash  as dont want to thrash the idle power moniter */
        if( !(gpl1_g2x_ta_is_activty_running( &pri,gas_id) 
#ifndef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
           && ( L1_SINGLE_BLOCK_ALLOC != transfer_data_ptr->current_alloc_type ) 
#endif /*#ifndef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
          ))
        {
           /* Check to see if TBF suspension is imminent */
          if(gpl1_g2x_ta_next_planned_suspension(&next_suspension_FN, &next_resume_FN, gas_id) &&
            (!IS_FRAME_NUM_LATER(next_suspension_FN,ADD_FN(gl1_get_FN(gas_id),3)))
            )
         {
            MSG_GERAN_HIGH_1_G("GBTA Power Monitor Schedule not allowed.tune away activity planned at FN: %d",
                             next_suspension_FN);
            return;
         }


                             /* Idle Mon schedule allowed */
          gl1_set_idle_event_status(GBTA_STATUS_PWR_MON_ACTIVE,idle_gas_id);
          MSG_GERAN_MED_0_G("GBTA GBTA_STATUS_PWR_MON_ACTIVE");

        }
        else
        {
          MSG_GERAN_HIGH_1_G("GBTA Power Monitor Schedule not allowed ONGOING TBF suspension pri:%d",
                             pri);
        }
      }
    }
  }
  else
  {
    //MSG_GERAN_HIGH_0_G("GL1 GBTA not allowed or active");
  }
} /* gpl1_gbta_schedule_idle_pwr_mon */

/*===========================================================================

FUNCTION gpl1_set_gbta_status

DESCRIPTION
  setting GBTA status based on NV/l1_state..etc

DEPENDENCIES

RETURN VALUE
===========================================================================*/

void gpl1_set_gbta_status(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  if( gl1_msg_get_multi_sim_standby_mode() &&
      IS_GBTA_INACTIVE() && 
      GBTA_ALLOWED_CAN_BE_SET(gas_id) &&
      gl1_gbta_get_nv_gbta_support(gas_id) &&
      (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) &&
      (!gl1_drdsds_enabled(OTHER_GAS_ID(gas_id))) // TRM now has tech specfic dr status
    )
  {
     SET_GBTA_STATUS(GRM_GBTA_ALLOWED);

     /* For this SUB we are not interested in IDLE state vars */
     grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);

     MSG_GERAN_MED_0_G("GL1 GBTA : GBTA is set ALLOWED");
  }
  else
  {
    SET_GBTA_STATUS(GRM_GBTA_NONE);
    MSG_GERAN_MED_3_G("GBTA: GBTA NOT ALLOWED gbta_state %d mode %d gbta allowed %d",
                        IS_GBTA_INACTIVE(),gl1_msg_get_multi_sim_standby_mode(),GBTA_ALLOWED_CAN_BE_SET(gas_id));
    /* For this SUB we are not interested in IDLE state vars */
    grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);
  }
}

#endif /*   FEATURE_GPRS_GBTA */


/*===========================================================================

FUNCTION gpl1_gprs_schedule_tbf_suspension

DESCRIPTION
  Using bcch_fn determines the TBF suspension period and whether
  NCELL BCCH reading can occur during this period.

  (This function is designed to be called from NCELL code.)

DEPENDENCIES
  ncell_bcch structure
  tbf_suspend structure
  transfer_data_ptr structure
  ptcch_ul_lookup and ptcch_dl_lookup

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_g2x_ta_block_bond( uint32 start_fn, tbf_suspend_T *suspension_time_ptr,  gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    /* assign shortcut to general transfer_data_params */
    transfer_data_T* transfer_data_ptr =
        l1_tskisr_blk->current_params.L1Data.pTransfer_data;


    int l1_transfer_sub_idx;

      transfer_data_ptr = NULL;

      /* Scan through and find which SUB is in active TRANSFER */
      for ( l1_transfer_sub_idx = GERAN_DATA_SPACE_INDEX_1; l1_transfer_sub_idx < NUM_GERAN_DATA_SPACES;
            l1_transfer_sub_idx++ )
      {
        if ( l1_tsk_buffer[l1_transfer_sub_idx].l1_state == L1_TRANSFER_MODE )
        {
          transfer_data_ptr =
            l1_tsk_buffer[l1_transfer_sub_idx].current_params.L1Data.pTransfer_data;

          gpl1_gprs_meas_mode[l1_transfer_sub_idx].allowed_to_go = TRUE;

                break;
         }
      }


    if( NULL != transfer_data_ptr )
    {
        uint8 frame_index;
        uint8 ncell_bcch_fn_mod_52;

        uint32   pre_frame_index;
        uint32 multiframe_start;

        /* ncell_bcch_fn is the frame number with respect
         * to serving cell where the NCELL BCCH occurs
         */
        multiframe_start     = (start_fn / MULTIFRAME_52) * MULTIFRAME_52;
        ncell_bcch_fn_mod_52 = start_fn % MULTIFRAME_52;
       // POP ncell_bcch_fn        = start_fn;

        MSG_GERAN_HIGH_1_G( " Initial FN before block boundary %d ", start_fn);

        pre_frame_index = ncell_bcch_fn_mod_52
                          - GPL1_FN_MOD52_DIV_13[ ncell_bcch_fn_mod_52 ];

        /* frame_index = pre_frame_index % 4 */
        frame_index     = (uint8)
            ( pre_frame_index - ( ( pre_frame_index >> 2 ) << 2 ) );

        /* Find the range of frame numbers used for BCCH decoding */
        gpl1_gprs_tbf_susp_range( multiframe_start,
                                  frame_index,
                                  suspension_time_ptr,
                                  ncell_bcch_fn_mod_52, gas_id );
        return TRUE;
    }
    else
    {
        MSG_GERAN_ERROR_0_G("Null transfer data ptr");
        return FALSE;
    }

}

void gpl1_g2x_ta_seq_no_act(gas_id_t gas_id)
{
  MSG_GERAN_LOW_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_no_act at FN=%d",gl1_get_FN(gas_id));
}

void gpl1_g2x_ta_seq_low_ts_chage(gas_id_t gas_id)
{

   uint32   FN   = gl1_get_FN( gas_id );
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   transfer_data_T*  transfer_data_ptr =
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_low_ts_chage at FN=%d",gl1_get_FN(gas_id));


#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif

   if( FALSE != transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap )
   {

      if( !IS_FRAME_NUM_LATER(FN, transfer_data_ptr->dynamic_tbf_ptr->starting_time))
      {
         /* disable rx and tx's for 1 radio block */
         transfer_data_ptr->disable_rx_tx = TRUE;
         MSG_GERAN_HIGH_0_G("disable_rx_tx T");

         if ( l1_sc_globals_ptr->monscan_in_progress )
         {
            MSG_GERAN_MED_0_G( "Suspending Mon in lowest_ts_change gap" );
            l1_sc_monscan_abort_gprs(gas_id);
         }
      }
      else
      {
         trigger_ts_change[gas_id] = TRUE;
         transfer_data_ptr->disable_rx_tx = FALSE;
         transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap = FALSE;

         if( NULL != transfer_data_ptr->dl_tbf_ptr )
         {
           transfer_data_ptr->dl_tbf_ptr->allow_one_rb_gap = FALSE;
         }

         /* stop tx's for first radio block as USF's haven't been detected yet */
         GPL1_DISABLE_TX(gas_id);
         gpl1_g2x_ta_deregister_seq(gas_id);
        /*idle sub can be in page reorg mode and decoding the pages in GBTA so don't start the sc tick when GBTA is active*/
		if(!IS_GBTA_ACTIVE())
		{
         gpl1_start_sc_tick_trans(gas_id);
      }
        else
        {
          MSG_GERAN_HIGH_0_G("GBTA is active, don't start sc tick");
        }
   }
   }
   else if( NULL != transfer_data_ptr->dl_tbf_ptr )
   {
      if (transfer_data_ptr->dl_tbf_ptr->allow_one_rb_gap)
      {

         if( !IS_FRAME_NUM_LATER( FN,transfer_data_ptr->dl_tbf_ptr->tbf_start_time))
         {
            transfer_data_ptr->disable_rx_tx = TRUE;

            MSG_GERAN_HIGH_0_G("disable_rx_tx T");
            if ( l1_sc_globals_ptr->monscan_in_progress )
            {
               MSG_GERAN_MED_0_G( "Suspending Mon in lowest_ts_change gap" );
               l1_sc_monscan_abort_gprs(gas_id);
            }
         }
         else
         {
            trigger_ts_change[gas_id] = TRUE;
            transfer_data_ptr->disable_rx_tx = FALSE;
            transfer_data_ptr->dl_tbf_ptr->allow_one_rb_gap = FALSE;
            transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap = FALSE;

            /* stop tx's for first radio block as USF's haven't been detected yet */
            GPL1_DISABLE_TX(gas_id);
            gpl1_g2x_ta_deregister_seq(gas_id);
            /*idle sub can be in page reorg mode and decoding the pages in GBTA so don't start the sc tick when GBTA is active*/
		    if(!IS_GBTA_ACTIVE())
		    {
            gpl1_start_sc_tick_trans(gas_id);
         }
            else
           {
             MSG_GERAN_HIGH_0_G("GBTA is active, don't start sc tick");
           }
      }
   }
   }

   if( trigger_ts_change[gas_id])
   {
      MSG_GERAN_MED_0_G( "Do not allow GBTA for this frame for lowest ts change" );
      l1_tskisr_blk->time_slot_change_pending = TRUE;
   }

}

void gpl1_g2x_ta_seq_wid_search(gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   // l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   transfer_data_T *transfer_data_ptr=l1_tskisr_blk->current_params.L1Data.pTransfer_data;
   gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
   uint32   FN = gl1_get_FN( gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   boolean trm_band_req_ok = TRUE;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_wid_search at FN=%d",gl1_get_FN(gas_id));
 
   switch( g2x_ta_seq_trm_state[gas_id] )
   {
      case G2X_TA_DISABLE_RXTX_NULL :
      {
         if (gpl1_g2x_ta_common_checks_before_GAP(G2X_TA_PRI_WID_SEARCH,gas_id) 
#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
             && !IS_TWO_PHASE_ACCESS(gas_id) /*do not allow TBF suspension in 2 phase access*/
#endif /*#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
             ) 
         {
            /* Start the suspension by disabling tbf rx/tx
            * activity and setting the serving cell
             * suspension flag inorder to prevent
             * interference measurements from being scheduled.
             */
            transfer_data_ptr->disable_rx_tx  = TRUE;
            gprs_serv_cell_meas_ptr->suspended_tbf = TRUE;
            gl1_msg_abort_dl_ptcch(gas_id);
            g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_SUSPENDING;

#ifdef FEATURE_GPRS_GBTA
            /* Block GBTA during TBF suspension */
            if ( IS_GBTA_ALLOWED())
            {
               SET_GBTA_STATUS(GRM_GBTA_BLOCKED);
            }
#endif /* FEATURE_GPRS_GBTA */
            gpl1_stop_sc_tick_trans(gas_id);
            MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_wid_search START at FN=%d",gl1_get_FN(gas_id));
         }
         else
         {
            (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
            gpl1_start_sc_tick_trans(gas_id);
            gpl1_g2x_ta_deregister_seq(gas_id);
         }
      }
      break;
      case G2X_TA_DISABLE_RXTX_SUSPENDING:
      {
         /*Checks while awaiting BCCH to start*/
         boolean  abort_tbf_suspension = FALSE;

         MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_wid_search G2X_TA_DISABLE_RXTX_SUSPENDING at FN=%d",gl1_get_FN(gas_id));

         if( (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
               (transfer_data_ptr->release_pending == L1_FULL_RELEASE_DL)  ||
               (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL)
             )
         {
               abort_tbf_suspension = TRUE;

               MSG_GERAN_HIGH_1_G("REL PEND ABORT TBF SUSP %d",transfer_data_ptr->release_pending);
         }
         else if(gpl1_g2x_ta_is_TS_act_imminent(gas_id))
         {
           #ifdef FEATURE_GPRS_GBTA
           /* Allow GBTA after TBF suspension aborted*/
           if ( IS_GBTA_BLOCKED())
           {
               SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
           }
           #endif /* FEATURE_GPRS_GBTA */
           /* Allow interference measurements to be scheduled */
           gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;
           gpl1_start_sc_tick_trans(gas_id);
           gpl1_g2x_ta_deregister_seq(gas_id);
           MSG_GERAN_MED_1_G("ABORT TBF SUSPEND FN = %d",FN);
           break;
         }
         else if( 0 != gpl1_gprs_get_num_rrbp_msg(gas_id ) )
         {
           abort_tbf_suspension =
               gpl1_gprs_rrbp_to_abort_tbf_suspension( FALSE, ADD_FN( data_ptr ->table[G2X_TA_PRI_WID_SEARCH].frame_number,
                                                               data_ptr ->table[G2X_TA_PRI_WID_SEARCH].duration ), gas_id );
         }

         if(abort_tbf_suspension)
         {
            g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_ABORTING;
            break;
         }

     if(FN == SUB_FN(data_ptr->non_trm_act.wIDsearch_fn, 1 ))
     {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       if(FALSE == grm_check_ptm_irat_sc_activity_ok(l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_in_tbf_freq_offset,
                                                     data_ptr ->table[G2X_TA_PRI_WID_SEARCH].duration,
                                                     G2X_TA_SEQ,
                                                     gas_id))
       {
         trm_band_req_ok = FALSE;
       }
      
       if (FALSE == trm_band_req_ok) 
       {
#ifdef FEATURE_GPRS_GBTA
         if ( IS_GBTA_BLOCKED())
         {
           SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
         }
#endif /* FEATURE_GPRS_GBTA */ 
         (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
         gpl1_start_sc_tick_trans(gas_id);
         gpl1_g2x_ta_deregister_seq(gas_id);
       }
       else
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
       {
         l1_sc_wcdma_receive_id_in_tbf(wcdma_done_trans, gas_id);
         trans[gas_id].tick_wcdma = FALSE;
         g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_SUSPENDED;
       }
     }
    }
    break;
    case G2X_TA_DISABLE_RXTX_SUSPENDED:
    {
     /*Checks while awaiting BCCH to start*/
     boolean  abort_tbf_suspension = FALSE;
    MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_wid_search G2X_TA_DISABLE_RXTX_SUSPENDED at FN=%d",gl1_get_FN(gas_id));
    if( (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
               (transfer_data_ptr->release_pending == L1_FULL_RELEASE_DL)  ||
               (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL)
              )
         {
             abort_tbf_suspension = TRUE;

             MSG_GERAN_HIGH_1_G("REL PEND ABORT TBF SUSP %d",transfer_data_ptr->release_pending);
         }
         else if(gpl1_g2x_ta_is_TS_act_imminent(gas_id))
         {
            l1_sc_wcdma_abort_id_tbf(gas_id);
            #ifdef FEATURE_GPRS_GBTA
             /* Allow GBTA after TBF suspension aborted*/
            if ( IS_GBTA_BLOCKED())
            {
             SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
            }
            #endif /* FEATURE_GPRS_GBTA */
            /* Allow interference measurements to be scheduled */
            gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;
            gpl1_start_sc_tick_trans(gas_id);
            gpl1_g2x_ta_deregister_seq(gas_id);
            MSG_GERAN_MED_1_G("ABORT TBF SUSPEND FN = %d",FN);
            break; 
         }
         else if( 0 != gpl1_gprs_get_num_rrbp_msg(gas_id ) )
         {
           abort_tbf_suspension =
               gpl1_gprs_rrbp_to_abort_tbf_suspension( FALSE, ADD_FN( data_ptr ->table[G2X_TA_PRI_WID_SEARCH].frame_number,
                                                               data_ptr ->table[G2X_TA_PRI_WID_SEARCH].duration ), gas_id );
         }

         if(abort_tbf_suspension)
         {
            l1_sc_wcdma_abort_id_tbf(gas_id);
            g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_ABORTING;
            break;
         }

         if(FN == data_ptr->non_trm_act.wIDsearch_fn)
         {
            trans[gas_id].tick_wcdma = TRUE;
            l1_sci_tick_receive(gas_id);
            l1_sci_tick_wcdma_id_in_tbf(22, gas_id);
            trans[gas_id].wcdma_cell_id_sceduled = FALSE;
            trans[gas_id].wcdma_cell_id_go   = FALSE;
         }
		 else if(FN == SUB_FN(ADD_FN( data_ptr->non_trm_act.wIDsearch_fn,  data_ptr ->table[G2X_TA_PRI_WID_SEARCH].duration),NPL1_FN_OFFSET))
         {
            /* Allow interference measurements to be scheduled */
            gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;

            /* Allow TBF rx/tx activities again */
            transfer_data_ptr->disable_rx_tx  = FALSE;

            /* stop tx's for first radio block as USF's haven't been detected yet */
            GPL1_DISABLE_TX(gas_id);

            if (trans[gas_id].tick_wcdma)
            {
               gpl1_gprs_restart_ncell_sch(gas_id);
            }

            trans[gas_id].tick_wcdma = FALSE;

#ifdef FEATURE_GPRS_GBTA
            /* Allow GBTA after TBF suspension/resume */
            if ( IS_GBTA_BLOCKED())
            {
               SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
            }
#endif /* FEATURE_GPRS_GBTA */

            MSG_GERAN_MED_2_G( "G2X_TA SEQ gpl1_g2x_ta_seq_wid_search TBF RESTART FN = %d num_rrbp_msgs = %d", gl1_get_FN(gas_id), gpl1_gprs_get_num_rrbp_msg(gas_id) );

            if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE)
            {
              l1_sc_wcdma_abort_id_tbf(gas_id);
              g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_ABORTING;
              break;
            }
            gpl1_g2x_ta_deregister_seq(gas_id);

         }
      }
      break;
      case G2X_TA_DISABLE_RXTX_ABORTING :
      {
         MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_wid_search G2X_TA_DISABLE_RXTX_ABORTING at FN=%d",gl1_get_FN(gas_id));
         if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
        {
            MSG_GERAN_HIGH_1_G( "Wait Here do not proceed	 WCDMA srch state : %d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);
            break;
        }

         if( FALSE == transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap )
         {
            if ( NULL != transfer_data_ptr->dl_tbf_ptr )
            {
               if ( FALSE == transfer_data_ptr->dl_tbf_ptr->allow_one_rb_gap )
               {
				  /* TBF suspension is to be cancelled so re-allow
				   * tbf rx/tx activities.
				   */
                  transfer_data_ptr->disable_rx_tx = FALSE;
                  GPL1_DISABLE_TX(gas_id);
               }
            }
            else
            {
			   /* TBF suspension is to be cancelled so re-allow
			    * tbf rx/tx activities.
			    */
               transfer_data_ptr->disable_rx_tx = FALSE;
               GPL1_DISABLE_TX(gas_id);
            }
         }

#ifdef FEATURE_GPRS_GBTA
         /* Allow GBTA after TBF suspension aborted*/
         if ( IS_GBTA_BLOCKED())
         {
            SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
         }
#endif /* FEATURE_GPRS_GBTA */

         /* Allow interference measurements to be scheduled */
         gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;
         gpl1_start_sc_tick_trans(gas_id);
         gpl1_g2x_ta_deregister_seq(gas_id);
         MSG_GERAN_MED_1_G("ABORT TBF SUSPEND FN = %d",FN);    
      }
      break;
      
      default:
      MSG_GERAN_ERROR_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_wid_search Unexpected state %d ",g2x_ta_seq_trm_state[gas_id]);
      break;
  }
}
/*===========================================================================

FUNCTION gpl1_g2x_ta_seq_serv_bcch

DESCRIPTION
  Perform BCCH search.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_g2x_ta_seq_serv_bcch(gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   transfer_data_T *transfer_data_ptr=l1_tskisr_blk->current_params.L1Data.pTransfer_data;
   gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
   uint32   FN = gl1_get_FN( gas_id );
  
   MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_serv_bcch at FN=%d",FN);

   switch( g2x_ta_seq_trm_state[gas_id] )
   {
	  case G2X_TA_DISABLE_RXTX_NULL :
      {
		if (gpl1_g2x_ta_common_checks_before_GAP(G2X_TA_PRI_SERV_BCCH,gas_id))
		{
		   /* Start the suspension by disabling tbf rx/tx
            * activity and setting the serving cell
            * suspension flag inorder to prevent
            * interference measurements from being scheduled.
            */
		   transfer_data_ptr->disable_rx_tx  = TRUE;
		   gprs_serv_cell_meas_ptr->suspended_tbf = TRUE;
		   gl1_msg_abort_dl_ptcch(gas_id);
		   g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_SUSPENDING;

#ifdef FEATURE_GPRS_GBTA
		   /* Block GBTA during TBF suspension */
		   if ( IS_GBTA_ALLOWED())
		   {
			  SET_GBTA_STATUS(GRM_GBTA_BLOCKED);
		   }
#endif /* FEATURE_GPRS_GBTA */
           gpl1_stop_sc_tick_trans(gas_id);
		   MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_serv_bcch START at FN=%d",gl1_get_FN(gas_id));
		}
		else
        {
		   (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
		   gpl1_g2x_ta_deregister_seq(gas_id);
		   gpl1_start_sc_tick_trans(gas_id);
		}
	  }
	  break;
      case G2X_TA_DISABLE_RXTX_SUSPENDING:
      {
		 /*Checks while awaiting BCCH to start*/
		 boolean  abort_tbf_suspension = FALSE;

		 MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_serv_bcch G2X_TA_DISABLE_RXTX_SUSPENDING at FN=%d",gl1_get_FN(gas_id));
		 if( (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
			 (transfer_data_ptr->release_pending == L1_FULL_RELEASE_DL)  ||
         (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL)
			 )
		 {
			abort_tbf_suspension = TRUE;
			MSG_GERAN_HIGH_1_G("REL PEND ABORT TBF SUSP %d",transfer_data_ptr->release_pending);
		 }
     else if(gpl1_g2x_ta_is_TS_act_imminent(gas_id))
     {
       #ifdef FEATURE_GPRS_GBTA
       /* Allow GBTA after TBF suspension/resume */
       if ( IS_GBTA_BLOCKED())
       {
          SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
       }
       #endif /* FEATURE_GPRS_GBTA */
       gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;       
       transfer_data_ptr->disable_rx_tx  = FALSE;      
       gpl1_start_sc_tick_trans(gas_id);
       gpl1_g2x_ta_deregister_seq(gas_id);
       MSG_GERAN_MED_1_G("ABORT TBF SUSPEND FN = %d",FN);
       break;
     }
		 else if( 0 != gpl1_gprs_get_num_rrbp_msg(gas_id ) )
		 {
			abort_tbf_suspension =
                  gpl1_gprs_rrbp_to_abort_tbf_suspension( FALSE, ADD_FN( data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number,
                                                                  data_ptr ->table[G2X_TA_PRI_SERV_BCCH].duration ), gas_id );
		 }

		 if(abort_tbf_suspension)
		 {
			g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_ABORTING;
			break;
		 }

		 if(FN == SUB_FN(data_ptr->non_trm_act.nbcch_fn, 1 ))
		 {
        if (gpl1_gprs_sc_bcch_sched_cb_trans(gas_id)) 
        {
          trans[gas_id].tick_wcdma = FALSE;
          g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_SUSPENDED;
        }
        else
        {
          /* Else part should not be hit in the single sim case*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
          (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
          /*CR787789: GPL1 should enable the RX_TX when NCell activity is denied by TRM*/
          g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_ABORTING;
#endif  /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
			}
		 }
	  }
	  break;
	  case G2X_TA_DISABLE_RXTX_SUSPENDED:
	  {
		 /*Checks while awaiting BCCH to start*/
		 boolean  abort_tbf_suspension = FALSE;
		 MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_serv_bcch G2X_TA_DISABLE_RXTX_SUSPENDED at FN=%d",gl1_get_FN(gas_id));

		 if( (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
			 (transfer_data_ptr->release_pending == L1_FULL_RELEASE_DL)  ||
               (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL)
			 )
		 {
			abort_tbf_suspension = TRUE;
			MSG_GERAN_HIGH_1_G("REL PEND ABORT TBF SUSP %d",transfer_data_ptr->release_pending);
		 }
            else if(gpl1_g2x_ta_is_TS_act_imminent(gas_id))
            {
              /* Abort any currently pending NCELL BCCH reads */
              l1_sc_abort_receive(gas_id);
              l1_sci_tick_receive(gas_id);
               #ifdef FEATURE_GPRS_GBTA
               /* Allow GBTA after TBF suspension/resume */
               if ( IS_GBTA_BLOCKED())
               {
                  SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
               }
               #endif /* FEATURE_GPRS_GBTA */
               gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;
               gpl1_start_sc_tick_trans(gas_id);
               gpl1_g2x_ta_deregister_seq(gas_id);
               MSG_GERAN_MED_1_G("ABORT TBF SUSPEND FN = %d",FN);
               break;      
		 }
		 else if( 0 != gpl1_gprs_get_num_rrbp_msg(gas_id ) )
		 {
			abort_tbf_suspension =
                  gpl1_gprs_rrbp_to_abort_tbf_suspension( FALSE, ADD_FN( data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number,
                                                                  data_ptr ->table[G2X_TA_PRI_SERV_BCCH].duration ), gas_id );
		 }

		 if(abort_tbf_suspension)
		 {
			/* Abort any currently pending NCELL BCCH reads */
			l1_sc_abort_receive(gas_id);
			l1_sci_tick_receive(gas_id);
			g2x_ta_seq_trm_state[gas_id]=G2X_TA_DISABLE_RXTX_ABORTING;
			break;
		 }

		 if(FN == data_ptr->non_trm_act.nbcch_fn)
		 {
			l1_sci_tick_receive(gas_id);
			trans[gas_id].tick_wcdma = FALSE;
		 }
		 else if(FN == SUB_FN(ADD_FN( data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number,  data_ptr ->table[G2X_TA_PRI_SERV_BCCH].duration),NPL1_FN_OFFSET))
		 {
			/* Allow interference measurements to be scheduled */
			gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;

			/* Allow TBF rx/tx activities again */
			transfer_data_ptr->disable_rx_tx  = FALSE;

			/* stop tx's for first radio block as USF's haven't been detected yet */
			GPL1_DISABLE_TX(gas_id);
#ifdef FEATURE_GPRS_GBTA
			/* Allow GBTA after TBF suspension/resume */
			if ( IS_GBTA_BLOCKED())
			{
			   SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
			}
#endif /* FEATURE_GPRS_GBTA */

			MSG_GERAN_MED_2_G( "G2X_TA SEQ gpl1_g2x_ta_seq_serv_bcch TBF RESTART FN = %d num_rrbp_msgs = %d",
                             ADD_FN( data_ptr ->table[G2X_TA_PRI_SERV_BCCH].frame_number, data_ptr ->table[G2X_TA_PRI_SERV_BCCH].duration ),
                             gpl1_gprs_get_num_rrbp_msg(gas_id) );
      gpl1_g2x_ta_deregister_seq(gas_id);
		 }
	  }
	  break;
	  case G2X_TA_DISABLE_RXTX_ABORTING :
	  {
		 MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_serv_bcch G2X_TA_DISABLE_RXTX_ABORTING at FN=%d",gl1_get_FN(gas_id));
		 if( FALSE == transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap )
		 {
			if ( NULL != transfer_data_ptr->dl_tbf_ptr )
			{
			   if ( FALSE == transfer_data_ptr->dl_tbf_ptr->allow_one_rb_gap )
			   {
				  /* TBF suspension is to be cancelled so re-allow
				   * tbf rx/tx activities.
				   */
				  transfer_data_ptr->disable_rx_tx = FALSE;
				  GPL1_DISABLE_TX(gas_id);
			   }
			}
			else
			{
			   /* TBF suspension is to be cancelled so re-allow
			    * tbf rx/tx activities.
			    */
			   transfer_data_ptr->disable_rx_tx = FALSE;
			   GPL1_DISABLE_TX(gas_id);
			}
		 }

#ifdef FEATURE_GPRS_GBTA
		 /* Allow GBTA after TBF suspension aborted*/
		 if ( IS_GBTA_BLOCKED())
		 {
			SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
		 }
#endif /* FEATURE_GPRS_GBTA */

		 /* Allow interference measurements to be scheduled */
		 gprs_serv_cell_meas_ptr->suspended_tbf = FALSE;
		 gpl1_start_sc_tick_trans(gas_id);
		 gpl1_g2x_ta_deregister_seq(gas_id);
		 MSG_GERAN_MED_1_G("ABORT TBF SUSPEND FN = %d",FN);    
	  }
	  break;

   default: 
       MSG_GERAN_ERROR_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_serv_bcch Unexpected state %d ",g2x_ta_seq_trm_state[gas_id]);
       break;
   }
  return;
}

#ifdef FEATURE_G2X_TUNEAWAY

void gpl1_g2x_ta_seq_trm_unlock_for_G(gas_id_t gas_id)
{

 volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
 // trm_freq_request_and_notify_enhanced_data_t request_and_notify_enhanced_data;
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  static rfgsm_band_type tx_band_at_susp[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE((rfgsm_band_type) 0xff) };
  transfer_data_T *transfer_data_ptr=l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  frequency_information_T *freq_info_ptr = (transfer_data_ptr->frequency_info_ptr);
  rfgsm_band_type curr_tx_band;

  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  boolean allowed_to_go;
  boolean tbf_susp_not_allowed = FALSE;
  boolean geran_tuneaway_allowed = FALSE;
  boolean stop_tbf_suspension = FALSE;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];


  static int8     inactivity_timer[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(MULTIFRAME_52) };
  static uint32   tbf_suspend_gta_start_frame[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };



  MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_G at FN=%d",gl1_get_FN(gas_id));

  switch( g2x_ta_seq_trm_state[gas_id] )
   {
      case G2X_TA_DISABLE_RXTX_NULL :
      {
          trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
          if ( (trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
          {
          /* query the API for the upper layer veto status */
          geran_tuneaway_allowed = gl1_is_geran_tuneaway_allowed(gas_id);

            MSG_GERAN_HIGH_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_G veto=%d",!geran_tuneaway_allowed);

            if ((FALSE != geran_tuneaway_allowed) && (gpl1_g2x_ta_common_checks_before_GAP(G2X_TA_PRI_TRM_UNLOCK,gas_id)))
            {
              /* Start the suspension by disabling tbf rx/tx
                        * activity and setting the serving cell
                        * suspension flag inorder to prevent
                        * interference measurements from being scheduled.
                        */
               tbf_suspend_gta_start_frame[gas_id] = gl1_get_FN( gas_id );
               transfer_data_ptr->disable_rx_tx = TRUE;
               gprs_serv_cell_meas_ptr->suspended_tbf = TRUE;

#ifdef FEATURE_GPRS_GBTA
               /* Block GBTA during TBF suspension */
               if ( IS_GBTA_ALLOWED())
               {
                  SET_GBTA_STATUS(GRM_GBTA_BLOCKED);
               }
#endif /* FEATURE_GPRS_GBTA */

               MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_G G2X_TA_DISABLE_RXTX_NULL exit at FN=%d",gl1_get_FN(gas_id));

            /* Added 1 frame as the decode usually on 2nd frame and as we have margin allowing us
            ** more time to decide if we want to continue tbf suspension.
            */
               suspension_timer[gas_id] = SUSPENSION_FRAME_DELAY + GUARD_FRAME_TBF_RECONF_OR_REL;
               if ( l1_sc_globals_ptr->monscan_in_progress )
               {
                  MSG_GERAN_LOW_0_G( "l1_sc_monscan_abort" );
                  l1_sc_monscan_abort(gas_id);
               }

               if ( l1_sc_globals_ptr->wcdma_srch_in_progress )
               {
                  l1_sc_wcdma_abort_ded_search(gas_id);
               }
               /* Abort any currently pending NCELL BCCH reads */
               l1_sc_abort_receive(gas_id);
               l1_sci_tick_receive(gas_id);
               gl1_msg_abort_dl_ptcch(gas_id);

               g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_SUSPENDING;
            }
            else
            {
               gpl1_start_sc_tick_trans(gas_id);
               (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
               gpl1_g2x_ta_deregister_seq(gas_id);
            }
         } /*if ( (l1_tskisr_blk->trm_status == GL1_TRM_GRANTED) || (l1_tskisr_blk->trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )*/
         break;
   }
   case G2X_TA_DISABLE_RXTX_SUSPENDING:
   {
      MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_G G2X_TA_DISABLE_RXTX_SUSPENDING at FN=%d",gl1_get_FN(gas_id));
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if ( trm_status == GL1_TRM_NOT_GRANTED )
      {
          call_in_task_grm_release( l1_tskisr_blk->client_id );
          (void)gpl1_g2x_ta_abort(G2X_TA_TRM_NOT_GRANTED,gas_id);
          g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
          /* Stay here to allow the suspension to provide the msg/hw layers with a couple of frames to terminate */
      } else
      {
          if (--suspension_timer[gas_id] == 0)
          {

              int     as_idx;
              boolean active_non_trans_sub = FALSE ;
            /* About to release TRM. Check again for any RRBP msg's.
             * This may have occured whilst decoding the block before the tbf suspend occured.
             * If so then abort the tbf suspension here
             */
            //adjust_ms_timebase = gpl1_gprs_check_rrbp(transfer_data_ptr, gas_id);
             tbf_susp_not_allowed = gpl1_g2x_ta_check_rrbp(FALSE, transfer_data_ptr, G2X_TA_PRI_TRM_UNLOCK,gas_id);

            /* Final Check before releasing TRM for any clashes with:
             * or pending tbf assignment
             * or pending tbf release.
             */
            allowed_to_go = gpl1_gprs_check_tbf_suspension( data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].frame_number,
                                                            data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].duration,
                                                                          FALSE, gas_id );


            /* check upper layer control switch (protects GPRS signalling operations) */
            if (!gl1_is_geran_tuneaway_allowed(gas_id))
            {
              /* API has indicated that the TBF cannot be pre-empted  - don't abort */
              MSG_GERAN_HIGH_0_G("G2X is blocked by upper layers");
              //Set allowed to go flag to false so that TBF suspend is aborted.
              allowed_to_go = FALSE;
      }
            if(gpl1_g2x_ta_is_TS_act_imminent(gas_id))
            {
              MSG_GERAN_HIGH_0_G("G2X_TA: Lower TS change occurred, TBF suspend aborted");
              allowed_to_go = FALSE;
            }

           /*   Code to be checked */
           /* Scan and set tbf_susp_not_allowed if ALL non-transfer SUBs are INACTIVE */
            for ( as_idx = GERAN_DATA_SPACE_INDEX_1; as_idx < NUM_GERAN_DATA_SPACES; as_idx++ )
            {
              if ( ( l1_tskisr_blk->client_id != l1_tsk_buffer[as_idx].client_id ) &&
                   ( l1_tsk_buffer[as_idx].l1_state != L1_INACTIVE_MODE ) )
              {
                /* Active non-transfer SUB found */
                active_non_trans_sub = TRUE;
                break;
              }
            }
            //if sequencer is already fired and Got unlock cancel then we need need to stop TBF suspension
            //With New rach interface  we get unlock cancel followed by unlock_immediately (so added both events)
            /*If winning client is not G client then don't open GTA gap*/
            if( (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event == TRM_UNLOCK_CANCELLED) 
                || (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event == TRM_UNLOCK_IMMEDIATELY)
                || (((gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event == TRM_UNLOCK_REQUIRED) ||
                (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event == TRM_UNLOCK_BAND_INCOMPATIBLE)) &&
                    ( IS_URGENT_ACCESS(gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_unlock_by_reason)))
                ||  (!IS_CLIENT_G(gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client))
                )
            {
              stop_tbf_suspension = TRUE;
            }

            if( allowed_to_go && ( !tbf_susp_not_allowed ) && (!stop_tbf_suspension) && (active_non_trans_sub) )
            {
              grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
              grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;

              grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;

              gl1_msg_abort_pwr_meas(gas_id);
              gpl1_stop_sc_tick_trans(gas_id);
              inactivity_timer[gas_id] = MULTIFRAME_52;


               /* Ask for the amount of time required for a complete reselection */
               grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
               grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(TBF_MIN_DURATION_MS, T_MSEC);
               grm_request_and_notify_enhanced_data_p->reason    = grm_get_connection_priority (GRM_PS_CONNECTION, gas_id);
               grm_request_and_notify_enhanced_data_p->sub_reason = gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id);

               grm_request_and_notify_enhanced_data_p->resource  =
                 gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );

               grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_TBF_SUSPEND;

               grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);
               g2x_ta_seq_trm_state[gas_id] = G2G_TA_DISABLE_RXTX_TRM_WAIT_SET_FW_ACTIVE_GAP;
               tx_band_at_susp[gas_id] = gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]);

               //call_in_task_trm_freq_request_and_notify_enhanced(request_and_notify_enhanced_data);
               call_in_task_grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p );

               /*release the diversity chain too*/
               grm_release_rx_sec(grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id);

              MSG_GERAN_HIGH_1_G("L1_DS_DISABLE_RXTX_SUSPENDED go Wait for TRM %d",gl1_get_FN(gas_id));



            }
             else
             {
              /* ABORT treatment */
               g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
               MSG_GERAN_HIGH_1_G("Abort TBF Suspension %d",gl1_get_FN(gas_id));
               gpl1_gprs_unsuspend_tbf(transfer_data_ptr, tbf_suspend_gta_start_frame[gas_id], gas_id);
               (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
               gpl1_g2x_ta_deregister_seq(gas_id);
             }
          }
          else
          {
            MSG_GERAN_HIGH_1_G("suspension timer running %d",gl1_get_FN(gas_id));
            if(gpl1_g2x_ta_is_TS_act_imminent(gas_id))
            {
              MSG_HIGH("G2X_TA Lower TS change occurred, TBF suspend aborted  ",0,0,0);
              /* ABORT treatment */
              g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
              MSG_GERAN_HIGH_1_G("Abort TBF Suspension %d",gl1_get_FN(gas_id));
              gpl1_gprs_unsuspend_tbf(transfer_data_ptr, tbf_suspend_gta_start_frame[gas_id], gas_id);
              (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
              gpl1_g2x_ta_deregister_seq(gas_id);
            }

          }
        }
        }
        break;

        case G2G_TA_DISABLE_RXTX_TRM_WAIT_SET_FW_ACTIVE_GAP:
        {
          /* Put GFW back into meas mode when deactivating Sub after TRM lock is released - need to revisit */
          gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_ACTIVE_GAP, geran_map_gas_id_to_nas_id(gas_id));
          g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_TRM_WAIT;
        }
        /* Fall trough please keep TRM_WAIT_SET_FW_ACTIVE_GAP above TRM_WAIT state */
        case G2X_TA_DISABLE_RXTX_TRM_WAIT:
        {
        MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_G G2X_TA_DISABLE_RXTX_TRM_WAIT at FN=%d",gl1_get_FN(gas_id));
        trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if ( trm_status == GL1_TRM_NOT_GRANTED  )
        {
          call_in_task_grm_release( l1_tskisr_blk->client_id );
          (void)gpl1_g2x_ta_abort(G2X_TA_TRM_NOT_GRANTED,gas_id);
          g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
          gpl1_g2x_ta_deregister_seq(gas_id);
        } 
        else if( ((trm_status == GL1_TRM_GRANTED)||(trm_status == GL1_TRM_RETAINED_FOR_ACCESS)) && !gpl1_g2x_ta_is_ts_change_missed(gas_id))
        {

          curr_tx_band = gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]);

          if( tx_band_at_susp[gas_id]  != 0xff && curr_tx_band != tx_band_at_susp[gas_id])
          {
            /*Delay TBF unsuspension by 1 frame to allow Tx band Configuration*/
             MSG_GERAN_MED_2_G("Delay TBF Restart for Tx Band Change New:%d Old: %d",curr_tx_band, tx_band_at_susp[gas_id]);

             gl1_hw_set_idle_mode(FALSE,gas_id);
             gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]),gas_id);
             tx_band_at_susp[gas_id] = curr_tx_band;
          }
          else
          {
            /* Check for the expired rrbp in GTA gap */
            if ( !gpl1_expired_rrbp_msg_in_gta(gas_id) && gpl1_gprs_awaiting_release(gas_id)
                 && gpl1_expired_rrbp_msg(gl1_get_FN(gas_id), gas_id) )
            {
              gpl1_set_expired_rrbp_msg_in_gta(TRUE, gas_id);
            }

            /* Need to load the RF and FW now, and only move to runnning once ready, this will be
                     indicated by the flag l1_tskisr_blk->firmware_loaded */
            gl1_hw_async_intf_app_mode_config(GFW_APP_MODE_ACTIVE, geran_map_gas_id_to_nas_id(gas_id));
            /* Inform gdrivers we are entering TRANSFER mode */
            gl1_hw_set_idle_mode(FALSE,gas_id);
            g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
            gpl1_gprs_unsuspend_tbf(transfer_data_ptr,tbf_suspend_gta_start_frame[gas_id], gas_id );
            tx_band_at_susp[gas_id] = 0xff;

            /* This call is only to now make sure that this client allows pre-emption to take place
                     as this doesn't take place within the minimum duration time specified using the RANE */
            grm_retain_lock( l1_tskisr_blk->client_id,
                             (grm_unlock_callback_t)&grm_gta_preemption_callback,
                             gas_id );

            if( grm_get_connection_priority(GRM_PS_CONNECTION,gas_id) != grm_get_reason( l1_tskisr_blk->client_id, gas_id ))
            {
              inactivity_timer[gas_id] = MULTIFRAME_52;
              grm_change_priority( l1_tskisr_blk->client_id, 
                                   grm_get_connection_priority(GRM_PS_CONNECTION,gas_id),
                                   gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id), 
                                   gas_id );

            }

            gpl1_g2x_ta_deregister_seq(gas_id);
          }

        }
        else if( --inactivity_timer[gas_id] <= 0 ||
                ( gpl1_g2x_ta_is_ts_change_missed(gas_id)) )
        {

          inactivity_timer[gas_id] = MULTIFRAME_52;
         /* Need to abort the TBF and perform an abnormal release */
          MSG_GERAN_HIGH_2_G("L1 send RR_DS_ABORT_TIMEDOUT fn %d inact timer %d", gl1_get_FN( gas_id ),inactivity_timer[gas_id]);

          gpl1_gprs_reset_rrbp_enteries(gas_id );

          /* Incase of pending release, MAC will ignore DS Abort Ind, L1 must release TBF and enter Idle*/
          if (transfer_data_ptr->release_pending)
          {
            l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
            transfer_data_ptr->confirm_pending = SEND_IN_ISR;
            /* Stop SCE as L1_DS_ABORT_IND is not sent , Help to release TRM and Sleep */
            l1_sc_stop(gas_id);
            MSG_GERAN_HIGH_1_G("Release Pending %d - Enter Idle", transfer_data_ptr->release_pending);
          }
          else
          {

            gas_id_t alt_gas_id;
            boolean g2x_resel_in_progress = FALSE;

            for(alt_gas_id = 0;alt_gas_id < NUM_GERAN_DATA_SPACES;alt_gas_id++)
            {
               if((alt_gas_id != gas_id)
               &&(l1_tsk_buffer[alt_gas_id].g2x_resel_in_progress)
               &&( l1_tsk_buffer[alt_gas_id].l1_state == L1_INACTIVE_MODE )
               )
               {
                  MSG_GERAN_HIGH_1_G("G2X resel in progress on gas_id:%d, postpone TBF abort by 940 ms",alt_gas_id);
                  g2x_resel_in_progress = TRUE;
                  break;
               }
            }

            if(( TRUE == gl1_is_granted_geran_sub_doing_reselection(l1_tskisr_blk->client_id) )
                || (g2x_resel_in_progress))
            {
              /* WtoG reselection is active on multimode sub
               * Do not restart data connection for 940 ms to
               * allow reselection to conclude
               */
               L1_send_MPH_L1_DS_ABORT_IND(gas_id,RR_DS_ABORT_TIMEDOUT,940);
            }else
            {
               L1_send_MPH_L1_DS_ABORT_IND(gas_id,RR_DS_ABORT_TIMEDOUT,200);
            }
            (void)gpl1_g2x_ta_abort(DS_ABORT_TIMEDOUT,gas_id);
            gpl1_g2x_ta_deregister_seq(gas_id);
            //  call_in_task_trm_release(l1_tskisr_blk->client_id, gas_id);
            call_in_task_grm_release( l1_tskisr_blk->client_id );

         }
        }
        else if( inactivity_timer[gas_id] == RETRY_FRAMES_BEFORE_TIMEOUT )
        {
          /* Attempt to unsuspend Transfer before Aborting PS */
          grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
          grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;
          grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;
          grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
          grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(TBF_MIN_DURATION_MS, T_MSEC);
          grm_request_and_notify_enhanced_data_p->reason    = grm_get_connection_priority(GRM_PS_CONNECTION,gas_id);
          grm_request_and_notify_enhanced_data_p->sub_reason = gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id);

          grm_request_and_notify_enhanced_data_p->resource  =
             gl1_ms_get_trm_resource(gas_id, grm_request_and_notify_enhanced_data_p->reason);

          grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_TBF_SUSPEND;
          grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id); 

          call_in_task_grm_request_and_notify_enhanced(grm_request_and_notify_enhanced_data_p);

          /*release the diversity chain too*/
          grm_release_rx_sec(grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id,gas_id),gas_id);

     }
     break;
   }

   default:
   MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_G INVALID state at FN=%d",gl1_get_FN(gas_id));
   break;
   } /* switch( g2x_ta_seq_trm_state[gas_id] ) */
  gl1_drx_require_next_tick(gas_id);


}
void gpl1_g2x_ta_seq_trm_unlock_for_X(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  //trm_freq_request_and_notify_enhanced_data_t request_and_notify_enhanced_data;
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  static rfgsm_band_type tx_band_at_susp[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE((rfgsm_band_type) 0xff) };

  transfer_data_T *transfer_data_ptr=l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  frequency_information_T *freq_info_ptr = (transfer_data_ptr->frequency_info_ptr);

  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  boolean allowed_to_go;
  boolean tbf_susp_not_allowed = FALSE;
  boolean geran_tuneaway_allowed = FALSE;
  boolean stop_tbf_suspension = FALSE;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;

  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

  static int8 inactivity_timer[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };
  static uint32   tbf_suspend_gta_start_frame[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };



  switch( g2x_ta_seq_trm_state[gas_id] )
  {
    case G2X_TA_DISABLE_RXTX_NULL :
    {
       MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X  G2X_TA_DISABLE_RXTX_NULL at FN=%d",gl1_get_FN(gas_id));

      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
      if ( (trm_status == GL1_TRM_GRANTED) || (trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )
      {
        /* query the API for the upper layer veto status */
        geran_tuneaway_allowed = gl1_is_geran_tuneaway_allowed(gas_id);
        
        MSG_GERAN_HIGH_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X veto=%d",!geran_tuneaway_allowed);
        
        if(data_ptr ->trm_unlock.winning_client_id != gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client)
        {
          /*winning client is changed don't allow suspension*/
          geran_tuneaway_allowed = FALSE;
          MSG_GERAN_HIGH_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X winning client changed tuneaway_allowed = %d",!geran_tuneaway_allowed);
        }
        
        if ((FALSE != geran_tuneaway_allowed)&&(gpl1_g2x_ta_common_checks_before_GAP(G2X_TA_PRI_TRM_UNLOCK,gas_id)))
        {
          /* Start the suspension by disabling tbf rx/tx
          * activity and setting the serving cell
          * suspension flag inorder to prevent
          * interference measurements from being scheduled.
          */
          tbf_suspend_gta_start_frame[gas_id] = gl1_get_FN( gas_id );
          transfer_data_ptr->disable_rx_tx  = TRUE;
          gprs_serv_cell_meas_ptr->suspended_tbf = TRUE;
          
#ifdef FEATURE_GPRS_GBTA
          /* Block GBTA during TBF suspension */
          if ( IS_GBTA_ALLOWED())
          {
            SET_GBTA_STATUS(GRM_GBTA_BLOCKED);
          }
#endif /* FEATURE_GPRS_GBTA */
          
          MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X G2X_TA_DISABLE_RXTX_NULL exit at FN=%d",gl1_get_FN(gas_id));
          gpl1_stop_sc_tick_trans(gas_id);
          /* Added 1 frame as the decode usually on 2nd frame and as we have margin allowing us
          ** more time to decide if we want to continue tbf suspension.
          */
          suspension_timer[gas_id] = SUSPENSION_FRAME_DELAY + GUARD_FRAME_TBF_RECONF_OR_REL;
          if ( l1_sc_globals_ptr->monscan_in_progress )
          {
            MSG_GERAN_LOW_0_G( "l1_sc_monscan_abort" );
            l1_sc_monscan_abort(gas_id);
          }
          
          if ( l1_sc_globals_ptr->wcdma_srch_in_progress )
          {
            /* Add one more frame to take care of G2W abort. In rare case, WL1 returns G2W_SRCH_ABORT_CNF or G2W_RF_EXIT_CNF
               later than usual, GL1 needs one more frame to finish aborting. GPL1 layer won't send frame tick notification
               to message layer when FW is unloaded at G2X_TA_DISABLE_RXTX_SUSPENDING. Then wcdma_ded_srch_handler() won't
               have the chance finish G2W aborting. G2W state will be stuck at GTOW_SRCH_STATE_ABORTING. Later when transiting
               out of transfer mode, GL1 could not do the transition because G2W state is still active.
         
               Since GL1 does GTA at the block boundary, usually there is still one or two extra frames before the other
               tech requesting TRM. Adding one more frame in suspension_timer() won't block the other tech. Aborting G2W before
               GTA does not happen often, plus the reserved time of the other tech aligns with GSM block boundary does not happen
               often either. Only if both happen, there is a possibility that GL1 releases TRM lock too late. It should be a rare
               case.
            */
            if (   l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_WCDMA 
                || l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE  )
            {
              suspension_timer[gas_id] += 1;
            }
            l1_sc_wcdma_abort_ded_search(gas_id);
          }
          /* Abort any currently pending NCELL BCCH reads */
          l1_sc_abort_receive(gas_id);
          l1_sci_tick_receive(gas_id);
          gl1_msg_abort_dl_ptcch(gas_id);
          
          g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_SUSPENDING;
        }
        else
        {
          gpl1_start_sc_tick_trans(gas_id);
          (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
          gpl1_g2x_ta_deregister_seq(gas_id);
        }
        
      } /*if ( (l1_tskisr_blk->trm_status == GL1_TRM_GRANTED) || (l1_tskisr_blk->trm_status == GL1_TRM_RETAINED_FOR_ACCESS) )*/
      
      break;
    }
    case G2X_TA_DISABLE_RXTX_SUSPENDING:
    {
      MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X G2X_TA_DISABLE_RXTX_SUSPENDING at FN=%d",gl1_get_FN(gas_id));
      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);

      if ( trm_status == GL1_TRM_NOT_GRANTED )
      {
        //call_in_task_trm_release(l1_tskisr_blk->client_id,gas_id);
        call_in_task_grm_release( l1_tskisr_blk->client_id );
        (void)gpl1_g2x_ta_abort(G2X_TA_TRM_NOT_GRANTED,gas_id);
        g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
        /* Stay here to allow the suspension to provide the msg/hw layers with a couple of frames to terminate */
      } 
      else
      {
        if (--suspension_timer[gas_id] == 0)
        {
          boolean tdscdma_client = FALSE;
          if((data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].client_id == GRM_TDSCDMA )
                || (data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].client_id == GRM_1X && CHECK_G21X_TA_ENABLE_RRBP())
                || (data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].client_id == GRM_LTE) || (data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].client_id == GRM_LTE_SUB2)) 
          {
               MSG_GERAN_HIGH_0_G("G2X_TA Prioritise G2T/1X/LTE TA over RRBP: PDAN_MSG/PDAN_MSG_WITH_FAI/PDAN_MSG_WITH_CRD");
            tdscdma_client = TRUE;
          }
          
          /* About to release TRM. Check again for any RRBP msg's.
          * This may have occured whilst decoding the block before the tbf suspend occured.
          * If so then abort the tbf suspension here
          */
          //adjust_ms_timebase = gpl1_gprs_check_rrbp(transfer_data_ptr, gas_id);
          tbf_susp_not_allowed = gpl1_g2x_ta_check_rrbp(tdscdma_client, transfer_data_ptr, G2X_TA_PRI_TRM_UNLOCK,gas_id);
          
          
          /* Final Check before releasing TRM for any clashes with:
          * or pending tbf assignment
          * or pending tbf release.
          */
          allowed_to_go = gpl1_gprs_check_tbf_suspension( data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].frame_number,
          data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].duration,
          FALSE, gas_id );

          if(FALSE == allowed_to_go)
          {
            MSG_GERAN_HIGH_0_G("G2X_TA not allowed as TBF assign pending");
          }

          /* check upper layer control switch (protects GPRS signalling operations) */
          if (!gl1_is_geran_tuneaway_allowed(gas_id))
          {
            /* API has indicated that the TBF cannot be pre-empted  - don't abort */
            MSG_GERAN_HIGH_0_G("G2X is blocked by upper layers");
            //Set allowed to go flag to false so that TBF suspend is aborted.
            allowed_to_go = FALSE;
          }
          
          if(gpl1_g2x_ta_is_TS_act_imminent(gas_id))
          {
            MSG_GERAN_HIGH_0_G("G2X_TA: Lower TS change occurred, TBF suspend aborted");
            allowed_to_go = FALSE;
          }
          
          if(data_ptr ->trm_unlock.winning_client_id != gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client)
          {
             /*winning client is changed don't allow suspension*/
             allowed_to_go = FALSE;
             MSG_GERAN_HIGH_0_G("G2X_TA winning client changed, don't allow suspesion ");
          }

          if(gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_event == TRM_UNLOCK_CANCELLED)
          {
            stop_tbf_suspension = TRUE;
          }
          
          if( allowed_to_go && ( !tbf_susp_not_allowed ) && (!stop_tbf_suspension))
          {
            grm_request_and_notify_enhanced_data_t  grm_request_and_notify_enhanced_data;
            grm_request_and_notify_enhanced_data_t *grm_request_and_notify_enhanced_data_p;
            
            grm_request_and_notify_enhanced_data_p = &grm_request_and_notify_enhanced_data;
            
            gl1_hw_schedule_reset_events(gas_id);
            gpl1_stop_sc_tick_trans(gas_id);
            if(SYS_MODEM_DS_PREF_TRIPLE_STANDBY == geran_get_sys_multi_sim_pref())
            {
              inactivity_timer[gas_id] = G2W_TUNEAWAY_TSTS_TIMEOUT;  // 400 ms /* G2W TSTS Back-to-back */
            }
            else
            {
              inactivity_timer[gas_id] = G2W_TUNEAWAY_DSDS_TIMEOUT; // 325 ms
            }
            
            /* Ask for the amount of time required for a complete reselection */
            grm_request_and_notify_enhanced_data_p->client_id = l1_tskisr_blk->client_id;
            grm_request_and_notify_enhanced_data_p->duration  = timetick_cvt_to_sclk(TBF_MIN_DURATION_MS, T_MSEC);
            grm_request_and_notify_enhanced_data_p->reason    = grm_get_connection_priority (GRM_PS_CONNECTION, gas_id);
            
            grm_request_and_notify_enhanced_data_p->resource  =
            gl1_ms_get_trm_resource( gas_id, grm_request_and_notify_enhanced_data_p->reason );
            
            grm_request_and_notify_enhanced_data_p->grm_tag   = GRM_RANE_TBF_SUSPEND;
            
            grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, l1_tskisr_blk->client_id, gas_id);
            g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_TRM_WAIT;
            tx_band_at_susp[gas_id] = gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]);
            
			/*Store current antenna position before LTA*/
#ifdef FEATURE_GSM_QSC_TX_DIV
            gl1_trm_asd_set_mode_and_config(TRM_CLIENT_MODE_SLEEP, TRM_ANT_SWITCH_DIV_CONFIG_CURRENT, gas_id);
#endif				 
            gl1_hw_multi_sim_deact_g2x(gas_id);

            /*release the diversity chain too*/
            grm_release_rx_sec(grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id,gas_id),gas_id);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
            /*if pchxfer activity is planned in future, release trm here so that we get it back to transfer sub once gap closes*/
            gpl1_check_rel_resv_ccch_xfer(TRUE, gas_id);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

            grm_request_and_notify_enhanced( grm_request_and_notify_enhanced_data_p->client_id,
                                             grm_request_and_notify_enhanced_data_p->resource,
                                             grm_request_and_notify_enhanced_data_p->duration,
                                             grm_request_and_notify_enhanced_data_p->reason,
                                             gl1_get_specific_sys_proc (GERAN_GPRS_PROCEDURE, gas_id),
                                             grm_request_and_notify_enhanced_data_p->grm_tag );
            
            l1_tskisr_blk->firmware_loaded = FALSE;
            
            MSG_GERAN_HIGH_1_G("L1_DS_DISABLE_RXTX_SUSPENDED go Wait for TRM %d",gl1_get_FN(gas_id));
          
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TBF_SUSPEND);
#endif
          
          }
          else
          {
            /* ABORT treatment */
            g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
            MSG_GERAN_HIGH_1_G("Abort TBF Suspension %d",gl1_get_FN(gas_id));
            gpl1_gprs_unsuspend_tbf(transfer_data_ptr, tbf_suspend_gta_start_frame[gas_id], gas_id);
            (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
            gpl1_g2x_ta_deregister_seq(gas_id);
          }
        }
        else
        {
          MSG_GERAN_HIGH_1_G("suspension timer running %d",gl1_get_FN(gas_id));
          gl1_msg_abort_pwr_meas(gas_id);
          if(gpl1_g2x_ta_is_TS_act_imminent(gas_id))
          {
            MSG_HIGH("G2X_TA Lower TS change occurred, TBF suspend aborted  ",0,0,0);
            /* ABORT treatment */
            g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
            MSG_GERAN_HIGH_1_G("Abort TBF Suspension %d",gl1_get_FN(gas_id));
            gpl1_gprs_unsuspend_tbf(transfer_data_ptr, tbf_suspend_gta_start_frame[gas_id], gas_id);
            (void)gpl1_g2x_ta_abort(T3192_RUNNING,gas_id);
            gpl1_g2x_ta_deregister_seq(gas_id);
          }
        }
      }
      break;
    }
    case G2X_TA_DISABLE_RXTX_TRM_WAIT:
    {
      MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X G2X_TA_DISABLE_RXTX_TRM_WAIT at FN=%d",gl1_get_FN(gas_id));
      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
      if ( trm_status == GL1_TRM_NOT_GRANTED  )
      {
        call_in_task_grm_release( l1_tskisr_blk->client_id );
        (void)gpl1_g2x_ta_abort(G2X_TA_TRM_NOT_GRANTED,gas_id);
        g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
        gpl1_g2x_ta_deregister_seq(gas_id);
      }
      else if( ((trm_status == GL1_TRM_GRANTED)||(trm_status == GL1_TRM_RETAINED_FOR_ACCESS)) && !gpl1_g2x_ta_is_ts_change_missed(gas_id))
      {
#ifdef FEATURE_G2L_TUNEAWAY
        /* Storing the FN at the time of G2X gap end. This will be used to check the
         * requirement where 2 back to back G2X gaps are not opened if duration between 
         * them is less than 100ms
         */
        gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN = gl1_get_FN(gas_id);
        MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X G2X_TA_DISABLE_RXTX_TRM_WAIT:prev_g2x_end_FN=%d",gpl1_g2x_ta_data[gas_id].trm_unlock.prev_g2x_end_FN);
#endif /* FEATURE_G2L_TUNEAWAY */

        if( l1_tskisr_blk->firmware_loaded == FALSE )
        {
          /* For G2W TA want to enter the FW load state whereas for GTA dont need to */
          g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_FW_LOAD_WAIT;
          fw_rf_setting_up[gas_id] = RF_FW_SETUP_FRAME_DELAY;
          gl1_multi_sim_fw_rf_load(l1_tskisr_blk->client_id, gas_id);
          break;
        }
      }
      else if( --inactivity_timer[gas_id] <= 0 ||
                ( gpl1_g2x_ta_is_ts_change_missed(gas_id)) )
      {
        
        if(SYS_MODEM_DS_PREF_TRIPLE_STANDBY == geran_get_sys_multi_sim_pref())
        {
          inactivity_timer[gas_id] = G2W_TUNEAWAY_TSTS_TIMEOUT;  // 400 ms /* G2W TSTS Back-to-back */
        }
        else
        {
          inactivity_timer[gas_id] = G2W_TUNEAWAY_DSDS_TIMEOUT; // 325 ms
        }
        /* Need to abort the TBF and perform an abnormal release */
        MSG_GERAN_HIGH_2_G("L1 send RR_DS_ABORT_TIMEDOUT fn %d inact timer %d", gl1_get_FN( gas_id ),inactivity_timer[gas_id]);
        
        gpl1_gprs_reset_rrbp_enteries(gas_id );
        
        /* Incase of pending release, MAC will ignore DS Abort Ind, L1 must release TBF and enter Idle*/
        if (gpl1_gprs_leaving_transfer(gas_id))
        {
          l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
          transfer_data_ptr->confirm_pending = SEND_IN_ISR;
          /* Stop SCE as L1_DS_ABORT_IND is not sent , Help to release TRM and Sleep */
          l1_sc_stop(gas_id);
          MSG_GERAN_HIGH_1_G("Release Pending %d - Enter Idle", transfer_data_ptr->release_pending);
        }
        else
        {
          
          gas_id_t alt_gas_id;
          boolean g2x_resel_in_progress = FALSE;
          
          for(alt_gas_id = 0;alt_gas_id < NUM_GERAN_DATA_SPACES;alt_gas_id++)
          {
            if((alt_gas_id != gas_id)
            &&(l1_tsk_buffer[alt_gas_id].g2x_resel_in_progress)
            &&( l1_tsk_buffer[alt_gas_id].l1_state == L1_INACTIVE_MODE )
            )
            {
              MSG_GERAN_HIGH_1_G("G2X resel in progress on gas_id:%d, postpone TBF abort by 940 ms",alt_gas_id);
              g2x_resel_in_progress = TRUE;
              break;
            }
          }
          
          if(( TRUE == gl1_is_granted_geran_sub_doing_reselection(l1_tskisr_blk->client_id) )
          || (g2x_resel_in_progress)
          )
          {
            /* WtoG reselection is active on multimode sub
            * Do not restart data connection for 940 ms to
            * allow reselection to conclude
            */
            L1_send_MPH_L1_DS_ABORT_IND(gas_id,RR_DS_ABORT_TIMEDOUT,940);
          }
          else
          {
            L1_send_MPH_L1_DS_ABORT_IND(gas_id,RR_DS_ABORT_TIMEDOUT,200);
          }
      
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
           gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_DS_ABORT);
#endif

          (void)gpl1_g2x_ta_abort(DS_ABORT_TIMEDOUT,gas_id);
          gpl1_g2x_ta_deregister_seq(gas_id);
          // call_in_task_trm_release(l1_tskisr_blk->client_id, gas_id);
          call_in_task_grm_release( l1_tskisr_blk->client_id );
          
        }
        
      }
      break;
    }
    case G2X_TA_DISABLE_RXTX_FW_LOAD_WAIT:
    {
      MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X G2X_TA_DISABLE_RXTX_TRM_WAIT at FN=%d",gl1_get_FN(gas_id));
      trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
    
      if ( fw_rf_setting_up[gas_id] > 0 )
      {
        fw_rf_setting_up[gas_id]--;
      }
    
      /* Check for the expired rrbp in GTA gap */
      if ( !gpl1_expired_rrbp_msg_in_gta(gas_id) && gpl1_gprs_awaiting_release(gas_id)
      && gpl1_expired_rrbp_msg(gl1_get_FN(gas_id), gas_id) )
      {
        gpl1_set_expired_rrbp_msg_in_gta(TRUE, gas_id);
      }
    
      if( l1_tskisr_blk->firmware_load_status == FW_LOAD_IN_PROGRESS )
      {
        MSG_GERAN_HIGH_0_G("FW_LOAD_IN_PROGRESS");
      }
      /* Time is now ticking keep watch for an abort */
      /* as this may happen at any time during this state too */
      else if( (trm_status == GL1_TRM_ABORTING) ||
      l1_tskisr_blk->main_command == L1_DEACTIVATE_COMMAND )
      {
        if( fw_rf_setting_up[gas_id] == 0 )
        {
          g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
            call_in_task_grm_release( l1_tskisr_blk->client_id );
          MSG_GERAN_HIGH_0_G("DS TBF CCCH Process Aborted");
        }
        else
        {
          MSG_GERAN_HIGH_1_G("Delay trm release FN = %d",gl1_get_FN( gas_id ));
        }
      } 
      else if( l1_tskisr_blk->firmware_loaded == TRUE )
      {
        if(gpl1_g2x_ta_is_ts_change_missed(gas_id))
        {
          if(SYS_MODEM_DS_PREF_TRIPLE_STANDBY == geran_get_sys_multi_sim_pref())
          {
            inactivity_timer[gas_id] = G2W_TUNEAWAY_TSTS_TIMEOUT;  // 400 ms /* G2W TSTS Back-to-back */
          }
          else
          {
            inactivity_timer[gas_id] = G2W_TUNEAWAY_DSDS_TIMEOUT; // 325 ms
          }
  
         /* Need to abort the TBF and perform an abnormal release */
          MSG_GERAN_HIGH_2_G("L1 send RR_DS_ABORT_TIMEDOUT fn %d inact timer %d", gl1_get_FN( gas_id ),inactivity_timer[gas_id]);
  
          gpl1_gprs_reset_rrbp_enteries(gas_id );
  
          /* Incase of pending release, MAC will ignore DS Abort Ind, L1 must release TBF and enter Idle*/
          if (transfer_data_ptr->release_pending)
          {
            l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
            transfer_data_ptr->confirm_pending = SEND_IN_ISR;
            /* Stop SCE as L1_DS_ABORT_IND is not sent , Help to release TRM and Sleep */
            l1_sc_stop(gas_id);
            MSG_GERAN_HIGH_1_G("Release Pending %d - Enter Idle", transfer_data_ptr->release_pending);
          }
          else
          {
            gas_id_t alt_gas_id;
            boolean g2x_resel_in_progress = FALSE;
        
            for(alt_gas_id = 0;alt_gas_id < NUM_GERAN_DATA_SPACES;alt_gas_id++)
            {
              if((alt_gas_id != gas_id)
                 &&(l1_tsk_buffer[alt_gas_id].g2x_resel_in_progress)
                 &&( l1_tsk_buffer[alt_gas_id].l1_state == L1_INACTIVE_MODE )
                )
               {
                 MSG_GERAN_HIGH_1_G("G2X resel in progress on gas_id:%d, postpone TBF abort by 940 ms",alt_gas_id);
                 g2x_resel_in_progress = TRUE;
                 break;
               }
            }
    
            if(( TRUE == gl1_is_granted_geran_sub_doing_reselection(l1_tskisr_blk->client_id) )
               || (g2x_resel_in_progress)
              )
            {
              /* WtoG reselection is active on multimode sub
               * Do not restart data connection for 940 ms to
               * allow reselection to conclude
               */
              L1_send_MPH_L1_DS_ABORT_IND(gas_id,RR_DS_ABORT_TIMEDOUT,940);
            }else
            {
              L1_send_MPH_L1_DS_ABORT_IND(gas_id,RR_DS_ABORT_TIMEDOUT,200);
            }
  
 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_DS_ABORT);
#endif
   
  
            (void)gpl1_g2x_ta_abort(DS_ABORT_TIMEDOUT,gas_id);
            gpl1_g2x_ta_deregister_seq(gas_id);
            call_in_task_grm_release( l1_tskisr_blk->client_id );
          }
        }

        else
        {
        /* Inform gdrivers we are entering TRANSFER mode */
          gl1_hw_set_idle_mode(FALSE,gas_id);
          /* initiate RF Tx band change - only gets actioned if band has actually changed */
          if( is_hw_tx_on (gas_id ))
          {
            gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]),gas_id);
          }
          tx_band_at_susp[gas_id] = 0xff;
          gl1_hw_turn_tx_on(gas_id);
          /*In gl1_hw_multi_sim_deact_g2x(gas_id), TRM state is updated TX off before doing RANE. After TRM is granted TRX 
          state has to be updated accordingly*/
          gl1_set_tx_on_always (gas_id);
          MSG_GERAN_HIGH_0_G("waiting for rf/fw warmup time");
          
          g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ; 
          gpl1_g2x_ta_deregister_seq(gas_id);
          gpl1_gprs_unsuspend_tbf(transfer_data_ptr,tbf_suspend_gta_start_frame[gas_id], gas_id);
    	  
          /* Do we need to suspend TBF for NCELL,SCELL BCCH or WCDMA ID searches */
          /* This call is only to now make sure that this client allows pre-emption to take place
          as this doesn't take place within the minimum duration time specified using the RANE */
          //  trm_retain_lock_advanced(l1_tskisr_blk->client_id, (trm_unlock_callback_advanced_t)&trm_gta_preemption_callback);
          grm_retain_lock( l1_tskisr_blk->client_id,(grm_unlock_callback_t)&grm_gta_preemption_callback,gas_id );

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
          /*Reserve TRM if required for PCH in transfer. We release trm when we open the gap so need to resv again*/
          gpl1_check_rel_resv_ccch_xfer(FALSE, gas_id);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
     
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TBF_RESUME);
#endif
        }  
      }
      break;
    }

    default:
    MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_seq_trm_unlock_for_X INVALID state at FN=%d",gl1_get_FN(gas_id));
    break;
  } /* switch( g2x_ta_seq_trm_state[gas_id] ) */
  
  gl1_drx_require_next_tick(gas_id);

}

boolean gpl1_g2x_ta_is_seq_registered_not_G(gas_id_t gas_id)
{
   if(&gpl1_g2x_ta_seq_trm_unlock_for_X == gpl1_g2x_ta_seq[gas_id])
  {
     return TRUE;
  }else
  {
     return FALSE;
    }
  }


#endif /* FEATURE_G2X_TUNEAWAY */

boolean gpl1_g2x_ta_register_seq(gpl1_g2x_ta_priority_t pri, gas_id_t gas_id)
{
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];

  /* Sanity check first check to see that no sequencer is register*/
  if(&gpl1_g2x_ta_seq_no_act != gpl1_g2x_ta_seq[gas_id])
  {
    if(G2X_TA_PRI_TBF_TS_CHANGE == pri)
    {
    if( (&gpl1_g2x_ta_seq_wid_search == gpl1_g2x_ta_seq[gas_id])
       ||(&gpl1_g2x_ta_seq_serv_bcch == gpl1_g2x_ta_seq[gas_id])    
#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
       ||(&gpl1_g2x_ta_seq_serv_ccch == gpl1_g2x_ta_seq[gas_id])
#endif     
      )
      {
        MSG_GERAN_ERROR_0_G("G2X_TA REGISTER TS change SEQ trying to register while PCH sequencer running, stopping pch seq");
        /*run sequencer for pch..this shud be its last run*/
        gpl1_g2x_ta_seq[gas_id](gas_id);
        
        /*if still pch sequencer is regisitered, ERR FATAL*/
        if(&gpl1_g2x_ta_seq_no_act != gpl1_g2x_ta_seq[gas_id])
        {
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
          ERR_GERAN_FATAL_1_G("G2X_TA REGISTER TS change SEQ trying to register another sequencer still running %X ",gpl1_g2x_ta_seq[gas_id]);
        }
      }
#ifdef FEATURE_G2X_TUNEAWAY
      else if ( (&gpl1_g2x_ta_seq_trm_unlock_for_G == gpl1_g2x_ta_seq[gas_id]) || (&gpl1_g2x_ta_seq_trm_unlock_for_X == gpl1_g2x_ta_seq[gas_id]) )
      {
       /*TS adjustment missed, need to do DS_ABORT when TRM is granted back to transfer sub*/
        MSG_GERAN_ERROR_1_G("G2X_TA REGISTER SEQ trying to register while sequencer running %X, TS adjustement missed",gpl1_g2x_ta_seq[gas_id]);
        return FALSE;
      }
#endif
      else
      {
#ifdef FEATURE_QSH_MDUMP
          QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_1_G("G2X_TA REGISTER TS change SEQ trying to register while sequencer running %X",gpl1_g2x_ta_seq[gas_id]);
      }
    }
    else
    {
      MSG_GERAN_ERROR_1_G("G2X_TA REGISTER SEQ trying to register while sequencer running %X",gpl1_g2x_ta_seq[gas_id]);
      return FALSE;
    }
  }  
    
  switch(pri)
  {
    case G2X_TA_PRI_TBF_TS_CHANGE:
    {
      MSG_GERAN_HIGH_1_G("G2X_TA REGISTER SEQ for activity G2X_TA_PRI_TBF_TS_CHANGE:  at FN %d  ",gl1_get_FN(gas_id));
      gpl1_g2x_ta_seq[gas_id] = &gpl1_g2x_ta_seq_low_ts_chage;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
       gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_LOW_TS_CHHANGE);
#endif

    }
    break;
    case G2X_TA_PRI_WID_SEARCH:
    {
      MSG_GERAN_HIGH_1_G("G2X_TA REGISTER SEQ for activity G2X_TA_PRI_WID_SEARCH: at FN %d ",gl1_get_FN(gas_id));
      gpl1_g2x_ta_seq[gas_id] = &gpl1_g2x_ta_seq_wid_search;
    }
    break;
    case G2X_TA_PRI_SERV_BCCH:
    {
      MSG_GERAN_HIGH_1_G("G2X_TA REGISTER SEQ for activity G2X_TA_PRI_SERV_BCCH:  at FN %d ",gl1_get_FN(gas_id));
#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if(data_ptr ->table[G2X_TA_PRI_SERV_BCCH].async_ccch_type == ASYNC_ACTIVITY_GBTA)
      {
        gpl1_g2x_ta_seq[gas_id] = &gpl1_g2x_ta_seq_serv_ccch;
      }
      else
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
      {
        gpl1_g2x_ta_seq[gas_id] = &gpl1_g2x_ta_seq_serv_bcch;
      }
    }
    break;

#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
     case  G2X_TA_PRI_SERV_PCH :
     {
       MSG_GERAN_HIGH_1_G("G2X_TA REGISTER SEQ for activity G2X_TA_PRI_SERV_PCH:  at FN %d ",gl1_get_FN(gas_id));
       gpl1_g2x_ta_seq[gas_id] = &gpl1_g2x_ta_seq_serv_ccch;
     }
     break;
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
#ifdef FEATURE_G2X_TUNEAWAY
    case G2X_TA_PRI_TRM_UNLOCK:
    {
        gpl1_g2x_ta_seq[gas_id] = &gpl1_g2x_ta_seq_trm_unlock_for_X;

      MSG_GERAN_HIGH_2_G("G2X_TA REGISTER SEQ for activity G2X_TA_PRI_TRM_UNLOCK registered sequencer %X at FN:%d ", gpl1_g2x_ta_seq[gas_id], gl1_get_FN(gas_id));

    } /*case G2X_TA_PRI_TRM_UNLOCK:*/
    break;
#endif /* FEATURE_G2X_TUNEAWAY */
    default:
      MSG_GERAN_ERROR_1_G("G2X_TA REGISTER SEQ: Unexpected pri: %d ",pri);
      return FALSE;


  } /*switch(pri) */
 return TRUE;
        }
boolean gpl1_g2x_ta_deregister_seq(gas_id_t gas_id)
{
  if(&gpl1_g2x_ta_seq_no_act == gpl1_g2x_ta_seq[gas_id])
  {
    MSG_GERAN_MED_0_G("G2X_TA DE-REGISTER SEQ: Trying to de-register NO ACT SEQ ");
    return FALSE;
  } else
        {
    MSG_GERAN_HIGH_1_G("G2X_TA DE-REGISTER SEQ: Sequencer %X de-registered",gpl1_g2x_ta_seq);
    gpl1_g2x_ta_seq[gas_id] = &gpl1_g2x_ta_seq_no_act;
    g2x_ta_seq_trm_state[gas_id] = G2X_TA_DISABLE_RXTX_NULL ;
    gpl1_g2x_ta_reset_ongoing(gas_id);
    gpl1_g2x_ta_scheduler(gas_id);
    return TRUE;
        }
      }


boolean gpl1_g2x_ta_common_checks_before_GAP(gpl1_g2x_ta_priority_t pri, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  boolean allowed_to_go=TRUE;

  boolean tdscdma_client = FALSE;

  transfer_data_T*  transfer_data_ptr =
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  rex_timer_cnt_type time_left = 0;

  rm_timer_state_t rm_timer_state;
  gmac_timers_t *gmac_timers_struct = gmac_get_timers_ptr(gas_id);
  MSG_GERAN_MED_1_G("G2X_TA SEQ gpl1_g2x_ta_common_checks_before_GAP at FN=%d",gl1_get_FN(gas_id));

   /* 1st FRAME*/
      /* Check for any clashes with:
       * UL ptcch
       * or pending tbf assignment
       * or pending tbf release.
       */

     if(((data_ptr ->table[pri].client_id == GRM_TDSCDMA) || (data_ptr ->table[pri].client_id == GRM_1X && CHECK_G21X_TA_ENABLE_RRBP()) || (data_ptr ->table[pri].client_id == GRM_LTE)|| (data_ptr ->table[pri].client_id == GRM_LTE_SUB2)) 
                  && (pri == G2X_TA_PRI_TRM_UNLOCK))
     {
       tdscdma_client = TRUE;
     }
      allowed_to_go &= gpl1_gprs_check_tbf_suspension( data_ptr ->table[pri].frame_number,
                                                          data_ptr ->table[pri].duration,
                                                                           FALSE, gas_id );
      if(FALSE == allowed_to_go)
      {
        MSG_GERAN_HIGH_0_G("G2X_TA not allowed as TBF assign pending");
        return FALSE;
      }

      allowed_to_go &= gpl1_gprs_find_rrbp_during_suspension( tdscdma_client, data_ptr ->table[pri].frame_number,
                                                              ADD_FN( data_ptr ->table[pri].frame_number,
                                                                      (data_ptr ->table[pri].duration - 1) ), gas_id ) ;
      if(FALSE == allowed_to_go)
      {
        MSG_GERAN_HIGH_0_G("G2X_TA not allowed as collision with higher RRBP ctrl msg");
        return FALSE;
      }
      
      rm_timer_state = grlc_dl_t3192_status(gas_id );
      time_left = rex_get_timer( &gmac_timers_struct->timer_t3168 ) ;

      if(!CHECK_G21X_TA_ENABLE_RRBP())
      {
        MSG_GERAN_HIGH_0_G("G21X_TA_RRBP flag is NOT set");
      }
      if( ( (0 == time_left) || (pri == G2X_TA_PRI_TRM_UNLOCK) )
        &&( (TMR_ACTIVE != rm_timer_state) || ( pri == G2X_TA_PRI_TRM_UNLOCK ) )
        &&( L1_TEST_MODE_OFF == transfer_data_ptr->l1_test_mode )
        && (FALSE == gpl1_g2x_ta_is_TS_act_imminent(gas_id))
        )
       {
          if( (0 != time_left) || (TMR_ACTIVE == rm_timer_state) )
          {
            MSG_GERAN_HIGH_2_G("G2X_TA is allowed when T3192/T3168 is running t3168_rem=%d, t3192_state=%d",
                              time_left,rm_timer_state);
          }
          else
          {
          MSG_GERAN_HIGH_3_G("G2X_TA is allowed - t3168_rem=%d, t3192_state=%d,l1_test_mode=%d",
                              time_left,rm_timer_state, transfer_data_ptr->l1_test_mode);
          }
         return TRUE;
       }
        else
        {
            MSG_GERAN_HIGH_3_G("G2X_TA SEQ gpl1_g2x_ta_common_checks_before_GAP t3168_rem=%d, t3192_state=%d,l1_test_mode=%d",
                                time_left,rm_timer_state, transfer_data_ptr->l1_test_mode);
        }

  return FALSE;
}

#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER

#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION  gl1_check_if_winning_client

DESCRIPTION
  Performs Winning client check.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean gl1_check_if_winning_client(grm_client_enum_t self_trm_client_id,gas_id_t gas_id)
{
  sys_modem_device_mode_e_type device_mode = gl1_msg_get_multi_sim_sys_mode();

  if ( (device_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ) || 
       (device_mode == SYS_MODEM_DEVICE_MODE_SINGLE_SIM) ||
        gl1_drdsds_enabled(OTHER_GAS_ID(gas_id)))
  {
    return TRUE;
  }
  else  // DSDS Mode.
  {
   if ( self_trm_client_id == gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client)
   {
     return TRUE;
   }
   else
   {
      trm_resource_enum_t           resource = gl1_ms_get_trm_resource(gas_id,TRM_DEMOD_PAGE);
      uint32                        duration = gpl1_multi_sim_convert_frames_to_sclcks( L1_SC_DURATION_BCCH );
      grm_request_data_t            req_data = {0};
      
      MSG_GERAN_HIGH_2_G("PCHXFER not the winning client ,self_client_id %d , winning_client_id %d",
                         self_trm_client_id,gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client );

      req_data.client_id         = self_trm_client_id;
      req_data.duration          = duration;
      req_data.reason            = TRM_DEMOD_PAGE;
      req_data.sub_reason        = SYS_PROC_TYPE_PAGE;
      req_data.resource          = resource;
      req_data.divrx_req         = GRM_DIVRX_TRM_REQ_INVALID;
      req_data.gas_id            = gas_id;

      call_in_task_grm_request(&req_data);
     return FALSE;
   }
  }
}
#endif /*  FEATURE_DUAL_SIM */
/*===========================================================================

FUNCTION  gpl1_g2x_ta_seq_serv_ccch

DESCRIPTION
  Sequencer for running BCCH and PCH in SS - using GBTA.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_g2x_ta_seq_serv_ccch(gas_id_t gas_id)
{
  transfer_data_T*  transfer_data_ptr = &l1_transfer_data[gas_id];
  gpl1_g2x_ta_data_t * data_ptr = &gpl1_g2x_ta_data[gas_id];
  uint32   FN   = gl1_get_FN( gas_id );
  gpl1_g2x_ta_priority_t pri;

  if (FALSE == gpl1_g2x_ta_is_activty_running( &pri,gas_id))
  {
    MSG_GERAN_HIGH_0_G("PCHXFER Handler just reregistered." );
    return;
  }
 
  /*first check if lowest tn change activity will collide with our activity
    if yes, we need to abort our activity immediately */
  if(TRUE == gpl1_g2x_ta_is_TS_act_imminent(gas_id))
  {
    if (pri == G2X_TA_PRI_SERV_PCH)
    {
      MSG_GERAN_HIGH_2_G("PCHXFER Missed PCH due to lowest tn change collision ..aborting c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
      L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
    }
    else if (pri ==  G2X_TA_PRI_SERV_BCCH)
    {
      MSG_GERAN_HIGH_2_G("PCHXFER Missed BCCH due to lowest tn change collision ..aborting c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
    }

    gpl1_g2x_ta_deregister_seq(gas_id);
    l1_sc_abort_receive(gas_id);
    l1_sci_tick_receive(gas_id);
    gpl1_start_sc_tick_trans(gas_id);
    return;
  }

  if ( FN ==  SUB_FN(data_ptr ->table[pri].frame_number,NPL1_FN_OFFSET))
  {
    boolean conflict = transfer_data_ptr->disable_rx_tx;

#ifdef FEATURE_DUAL_SIM
    if ((!conflict) && (pri == G2X_TA_PRI_SERV_PCH))
    {
      conflict = ! (gl1_check_if_winning_client(gl1_map_gas_id_to_client_id_pchxfer(gas_id),gas_id));

      /*if we are not the winning client , check if TRM_UNLOCK act is the winner.*/
      /*we can still go ahead and read own PCH if that activity is after PCH*/
      if(conflict &&
           (gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TRM_UNLOCK, gas_id)) &&
           (gpl1_gprs_tbf_abort_cb_store_data[gas_id].last_client == data_ptr ->trm_unlock.winning_client_id) &&
           ((data_ptr ->trm_unlock.event == TRM_UNLOCK_REQUIRED) || (data_ptr ->trm_unlock.event == TRM_UNLOCK_BAND_INCOMPATIBLE))&&
           (IS_FRAME_NUM_LATER(data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].frame_number, 
                               ADD_FN(data_ptr ->table[G2X_TA_PRI_SERV_PCH].frame_number ,data_ptr ->table[G2X_TA_PRI_SERV_PCH].duration))))
      {
        conflict = FALSE;
        MSG_GERAN_HIGH_1_G("PCHXFER Decode PCH since winning client act is after PCH, winner act 5 at FN %d",
                              data_ptr ->table[G2X_TA_PRI_TRM_UNLOCK].frame_number);
      }
    }
#endif /*  FEATURE_DUAL_SIM */
    if(!conflict)
    {
      conflict = gpl1_conflict_check_for_ccch(gas_id);

#ifdef FEATURE_DUAL_SIM
      /*in case of serv BCCH using GBTA mech, check if GBTA is already active or not*/
      /*we cant allow 2 GBTAs together*/
      if(!conflict)
      {
        conflict = IS_GBTA_ACTIVE();
        MSG_GERAN_HIGH_2_G("PCHXFER : B/C CCH check conflict %d gbta state %d", conflict,GET_GBTA_STATE());
      }
#endif /* FEATURE_DUAL_SIM */
    }

    if (!conflict)
    {
      gpl1_stop_sc_tick_trans(gas_id);
      if(TRUE == gpl1_gprs_sc_bcch_sched_cb_trans(gas_id))
      {
        trans[gas_id].tick_wcdma = FALSE;
      }
      else
      {
        if (pri == G2X_TA_PRI_SERV_PCH)
        {
          MSG_GERAN_HIGH_2_G("PCHXFER Missed PCH c_fn %d activity_fn = %d ..sched fail",gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
          L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
          gpl1_g2x_ta_deregister_seq(gas_id);
        }
        else if (pri ==  G2X_TA_PRI_SERV_BCCH)
        {
          MSG_GERAN_HIGH_2_G("PCHXFER Missed BCCH reschedule c_fn %d activity_fn = %d  sched fail",gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
          gpl1_g2x_ta_deregister_seq(gas_id);
        }
      }
    }
    else if (pri == G2X_TA_PRI_SERV_PCH)
    {
      MSG_GERAN_HIGH_2_G("PCHXFER Missed PCH c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
      L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
      gpl1_g2x_ta_deregister_seq(gas_id);
    }
    else if (pri ==  G2X_TA_PRI_SERV_BCCH)
    {
      MSG_GERAN_HIGH_2_G("PCHXFER Missed BCCH reschedule c_fn %d activity_fn = %d",gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
      gpl1_g2x_ta_deregister_seq(gas_id);
    }
  }
  else
  {
    if( ( FALSE == transfer_data_ptr->disable_rx_tx )  &&
        ( FN == data_ptr ->table[pri].frame_number )
      )
    {
      l1_sci_tick_receive(gas_id);
      trans[gas_id].tick_wcdma = FALSE;
    }
    else if ( FN == data_ptr ->table[pri].frame_number )
    {
      MSG_GERAN_ERROR_3_G("PCHXFER CCCH ABORT ### Missed CCCH Event %d  c_fn %d activity_fn = %d",pri,gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
      L1_send_UNKNOWN_CCCH_MPH_BLOCK_QUALITY_IND(gas_id);
      gpl1_g2x_ta_deregister_seq(gas_id);
      l1_sc_abort_receive(gas_id);
      l1_sci_tick_receive(gas_id);
    }
    else
    {
      MSG_GERAN_LOW_3_G("PCHXFER### Nothing to do Event %d  c_fn %d activity_fn = %d",pri,gl1_get_FN(gas_id),data_ptr ->table[pri].frame_number);
    }
  }
  return;
} /* gpl1_g2x_ta_seq_serv_ccch */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */


