/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            L 1   T R A N S F E R   M O D E   T A S K  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events while the L1
   task is in the L1 TRANSFER mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_transfer_mode
    Procedure to handle external events while the L1 task is in the
    L1 transfer mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_transfer_if.c#2 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
15/06/18   ng        CR2261488 Remove 'l1_rcvd_non_drx_ind' related handling, it is no more needed
21/09/17   rc        CR2048297 Handling of MAC_L1_RELEASE when there is a TS change is ongoing
25/05/17   hd        CR2051434 Make the variable l1_rcvd_non_drx_ind dual spaced
08/08/17   sn        CR2074497 Handling of MAC_L1_RELEASE during TRANSFER to EDTM Transition
16/11/17   rc        CR2144408 Check for appending DL_timeslots/UL_timeslots while handling MAC_L1_DYNAMIC_CONFIG/MAC_L1_DL_CONFIG respectively.
19/09/17   sn        CR2111248 Handle of UL TBF RELEASE during RACH to TRANSFER Transition State
09/07/16   sn        CR 1056833 State mismatch between GL1 and MAC during transfer during handling of PDCH REL+ PACKET TBF RELEASE
07/12/16   sn        CR1098212 Reset DL ST during DL TBF RELEASE
24/03/17   km        CR2024222 Cancel lowest_ts_change, if planned as part of PDCH_TS_REL, before releasing DL_TBF
09/05/17   mn        CR2043013 Reset the Wait_For_PCA FN/Valid variable when UL_DL_TBF_FULL_RELEASE is recieved.
23/05/17   sn        CR2049735 Handling of Back to Back TBF configure messages in TRANSFER mode.
08/05/17   sn        CR2043910 Low TS change handling during Processing of new Config messages
14/06/16   nm        CR1017234 Do not wait for sending rel cnf after DL RRBP(if pending) for Release type DL TBF NORMAL 
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
16/03/17   mn        CR2020307 FR40846:Packet transfer Error Recovery mechanism.
31/08/17   nm        CR2098538 Abort monitors and own PCH/BCCH when transition from Single block to UL TBF
22/08/17   nm        CR2096887 Reset tuneaway and ncell data when leaving transfer
01/08/16   sn        CR1047524 Handling of PACKET PDCH RELEASE during FULL RELEASE
28/06/16   sn        CR1034621 Correcting pending alloc in DTM state.
18/5/16    sm        CR1017901 Added GL1 idle mode QSH events 
18/5/16    sn        CR1007565 PACKET Transfer DTF Events/Handlers Phase1
02/03/16   sn        CR981034 Handling Packet TBF release of DL while wait for PTSR re-configuration
07/11/15   zc        CR817452 Do not change downlink alloc to null on TBF release if there is a pending main_command
29/09/15   hd        CR914052 Starting time is made up when MAC_L1_PDCH_REL_TS is received
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
01/05/15   nm        CR812477 Check for pending lowest tn change before calling gpl1_gprs_lowest_tn_changed for DL TBF CFG
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr       CR818535 FR25951 Changes to support Single Voice Session 
07/04/15   shr       CR808932: COEX support for DTM
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
05/12/14   sp        CR767083: Override paging multiframe to 2, for PDT tests
17/02/15   nk        CR784206 Release ISR mutext after moving main command to IDLE
17/02/15   br        CR769516 Change dtm event to  L1_DTM_TRANSFER_RECONFIG during MAC_L1_RELEASE only if previous dtm evtn is not DTM_STOP
18/11/14   nm        CR739472 Handle UL TBF Rel when current and pending alloc is Dynamic_DOWLINK_ALLOC
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
05/11/14   ws        CR748118 -Respond to MAC_L1_RELEASE if received when transitioning to IDLE 
                     mode from SINGLE BLOCK Allocation
10/10/14   aga       CR731661 gas_id being incorrectly set in some circumstances 
06/10/14   pg        CR734405 Thor compilation errors 
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build 
01/09/14   br        CR694598 Update starttime in pending DL TBF pointers also with MAC_L1_PDCH_REL_TS
28/05/14   ap        CR671971: Change SGLTE mode on reception of UE_MODE_CHANGE_REQ
08/08/14   sp        CR676015: Update dl tbf startime also in handling MAC_L1_TS_RECONFIG.
21/02/14   nk        CR611257 Handling for rrbp msg_type PCA_UL_FAI_MSG while adjusting starting time
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
04/06/14   sp        CR673305: Update pending tbf ptrs also with Starting time, when adjusting startTime during TBF Release handling.
02/07/14   dg        CR622796 Combine UL and DL normal TBF release if GL1 receives UL/DL TBF release
                     when already DL/UL tbf release is pending because of ongoing G2T search
02/07/14   aga       CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
30/05/14   ws        CR657674 Data + MMS Phase 1
08/05/14   br        CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
01/05/14   ab        CR657605 : GBTA : Transfer Sub should not manage GBTA transition  
08/05/14   br        CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
08/05/14   br        CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
01/05/14   ab        CR657605 : GBTA : Transfer Sub should not manage GBTA transition
28/04/14   ws        CR641591 - EDTM Don't connect Pkt CS assignemt if reconfigured before DTM_CHANNEL_RELEAS_REQ
                     is received
06/02/14   smd       CR548408: Remove sending MCS_CXM_STX messages.
25/10/2013 nk        CR567965: Set GBTA status to none when releasing tbf only when the GBTA is not active
24/07/13   ss        CR515896  Set wcdma_list_update_inprogress to TRUE in msg_handler routine
27/09/13   sp        CR551580: Add NV logging of outgoing L1 messages
14/09/13   ap/aga    CR545423 - G2W Tune Away Feature
15/08/13   sk        CR524039 GBTA bringup changes
08/08/13   dv        CR525765 Changes for DSDS T/G IRAT bringup
07/16/13   zc        CR470197 Replace memcpy with memscpy
21/06/13   cs        Major Triton TSTS Syncup
03/12/12   cgc       CR419997 DTM state change during channel modify, add L1_DTM_INIT
29/04/13   ws        CR481359 enable GTA support
15/02/13   ws        CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
31/05/12   ab        CR 415136 RX and TX Time Power averaging update using msg router intf.
12/09/12   ab        CR410361 Incorrect starttime var used during PCA vs starttime clash
12/09/12   ip        CR384421 Disable GSM sleep during CCO activity
18/05/12   ws        CR360865: do not release to IDLE if pending PDAN with FAI present
04/09/12   ab        CR 380314 Handling of back to back UL or DL release when already
                     releasing UL_DL_TBF in L1 is in progress
30/07/12   pa        CR383547: Don't schedule G2T if TBF starts during BLk21-24
03/07/12   pa        CR375720: Don't delay TBF release in the idle frame.
03/07/12   pa        CR371273: Don't schedule G2T if TS reconfigure before idle frame.
22/06/12   pa        CR371580: Delay TBF release during G2T activity.
19/06/12   ab        CR 370547 :GL1 not sending REL_CNF when releasing UL TBF
                     with existing UL+DL and UL being released.
27/04/12   ab        CR356700 -L1 to obey back to back TBF release,to come out of release state.
31/05/12   ab        CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
12/06/2012 ws        CR 361498 Move UL or DL starting time if PCA needs to be transmitted after
                     start-time
06/12/11   ab        CR 324793:MS sending REL_CNF immediately without sending RRBP
                     for uplink (PTR startime comes before RRBP ). RRBP ignored when TS changes
25/04/12   pa        CR355881: Removed warning.
28/03/12   pa        CR 346062 Workaround for G2T Abort.
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
08/02/12   ab        CR: 181344 - 3GPP Spec Update: Receive filter reset upon change of frequency
18/01/11   ab        CR 331856 MS not considering RRBP_TYPE PCA_SRB,thus not sending RRBP for PKT TBF REL.
06/12/11   ab        CR 319219 Crash during pending Packet timeslot reconfigure followed by uplink release
07/11/11   ab        CR 316230 - L1 sends UL + DL connect ind when UL is about to be released.
                     and doing reconfig due to PTR when RRBP is just sent out.
20/10/11   ws        CR 313643 - Ensure start time for DTM_CHANNEL_REQ is after any suspension that
                     may be still in progress
27/09/11   ab        CR 308576 L1 forgets existing DL rel , followed by uplink rel in DTM
27/09/11   ab        CR 305068,do not wait to send rrbp for DL for Data other than polled messages.
27/09/11   ab        CR 307685 - L1 does not send REL CNF while rel UL and doing reconfig due to PTR
09/06/2011 ab        CR 289734 GL1 is trying to transmit a PCA on a timeslot that
                     has been superseded by a TS reconfiguration leading to crash
08/06/11   ab        CR285370 - Check for pending RRBP before bringing forward start time.
29/01/11   ky        CR272959:In UL+DL TBF, Adjusting the pending dl start time such
                     that UL release conf goes first to mac and then gl1 send
                     sends connect ind to gmac
27/01/11   dmb       The following RR<->L1 messages are removed (not used):
                     MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
17/06/08   ap        CR 149306 - L1 missing the first block for the downlink TBF
06/09/10   ab        ENH Add TEST MODE B Reconfig and add asymetric TS support.
17/09/10   ws        CR255424 - Don't set L1_DTM_CANCEL_PS_RECONFIG dtm event if
                     we still have a pending starting time.
05/07/10   og        Remove extra mutex unlocks. CR245080.
14/06/10   ab        CR: 235405 - 3GPP: Values of power control parameters for the
                     update and assignment of timeslots
06/05/10   dv        CR237642 - Add ASRB funtionality
03/03/10   ab        Code Featurization for FEATURE_GSM_DTM and FEATURE_GSM_EDTM
13/01/10   ab        CR 216148 GPRS L1 - EDTM - CS call released followed by UL/DL Rel
                     causes lock up as pending alloc state made NULL during TBF Rel.
11/01/10   ab        CR:222962 - GERAN L1 - Peform NULL Pointer check for single_blk_ptr
23/12/09   ab        CR 221081:GPRS L1 – does not consider pending release when
                     deciding on future pending allocation state.
11/12/09   ab        CR 220464 Add REL_IND/REL_CNF to DL_SINGLE_BLOCK to prevent L1 and MAC
                     go out of sync.
11/12/09   ab        CR: 217101 - L1 is not connecting to DL TBF after UL TBF is released
27/09/09   ab        REVERT CR CR:217101 : L1 is not connecting to DL TBF after UL TBF is released
                     as submiited by mistake before testing and review.
27/09/09   ab        CR:217101 : L1 is not connecting to DL TBF after UL TBF is released
13/08/09   ap        CR 166945 L1 does not send rel confirm when PTR and PUAN with FAI=1 arrives

09/07/09   ab        CR 183589 Incorrect handling of RLC mode change request during extend
                     UL TBF operation
24/04/09   ws        Backout CR 166945 since this causes Rx/TX to be tasked during
                     a timeslot change - CR 180318
12/03/09   ws        CR 173134 Added Test mode A to L1 interface and calculate
                     RLA_P for test modes from PDCH
11/03/09   og        CR173804 - Ignore downlink mac mode for Rel 6.
01/19/09   rc        Moved INVALID_ARFCN and ARFCNS_EQUAL in to gprsdef.h
25/11/08   ap        CR 166945 L1 does not send rel confirm when PTR and PUAN with FAI=1 arrives
25/11/08   ap        CR 163610 PSHO UE schedules but does not send out PCCF after PSHO failure
03/11/08   ap        Add L1 handling of MAC_L1_PSHO_CANCEL_PENDING_CONFIG.
04/11/08   nt        CR160641 - L1 needs to ensure that the release cnf is sent to MAC
02/10/08   cs        Lint fixes
22/08/08   nt        Removed compiler warnings
28/07/98   hv        Added KxMutex support
25/06/08   ap        CR 145857 L1 should ignore the additional abnormal release if we are
                     already handling an UL or DL release
10/04/08   ap        CR 113202 Don't set main commands in the DTM mode for MAC_L1_PDCH_REL_TS
14/03/08   ip        Added PSHO changes under FEATURE_GPRS_PS_HANDOVER
17/01/08   cs        Lint fixes
06/06/07   ws        CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                     is called when we reselect to PBCCH cell stopping NC2 reporting
01/05/07   og        Ensure the starting time elapsed flag is set to TRUE when
                     releasing an uplink single block alloc. Resolves CR 117521.
02/03/07   cs        Add support for new CGPS timetag
06/02/07   cs        Fix E/DTM compile issue
26/01/07   og        Implement UL ctrl msg deletion in L1.
23/01/07   og        Fix to compilation problems.
23/01/07   og        Resolve RVCT2.1 compiler warnings, merge from the
                     gpl1_hummer.10.00.43.01 branch.
23/01/07   og        Added handling of the RR->L1 MPH_DTM_L1PARAM_UPDATE_REQ message.
12/01/07   og        Improve reaction time of the packet uplink assignment by 2
                     TDMA frames.
05/01/07   og        Withdraw CHANGE 409840. Resolves CR108896.
28/11/06   dv        Lint cleanup. Replaced multiple calls of gl1_get_FN()
                     with a single call at top of function
22/11/06   og        Remove the pending UL assignment part of connect IND when
                     an UL release is received. Resolves CR106527.
07/11/06   og        Merge fix to CR89511 from the raven 3.0 branch.
06/11/06   og        Don't reset the dtm enhanced release command event when a
                     PTR is received.
31/10/06   og        Ensure the AMR multirate configuration is updated in the
                     L1_EDTM_INIT.
30/10/06   og        DTM enhanced release code drop.
19/09/06   og        EDTM updates to handle dtm assignments with concurrent UL
                     DL TBF.
15/09/06   og        EDTM updates.
08/09/06   og        Remove the dummy ctrl blk restriction of connecting a DTM
07/09/06   og        EDTM updates following testing on Agilent 8960.
31/08/06   og        EDTM updates following host test simulator testing.
24/08/06   og        EDTM updates following host test simulator testing.
17/08/06   og        Basic featured-off EDTM code drop.
28/07/06   ws        CR94182 - only ignore PDCH_REL_TS when our main_command is set
                     and we don't have a release pending
19/07/06   nt        CR93487 - L1 ignoring release from MAC. Have stopped the main command
                     getting set to TRANSFER_COMMAND for a PTR in DTM MODE.
25/05/06   og        Improve PTR reaction time, resolves CR90515.
05/05/06   og        Merge fix to CR93146 / SR822243 from the raven 3.0 branch.
17/03/06   og        Take downlink assignment mac mode into account. This is
                     required by release 4 ETSI spec.
09/03/06   ws        If we are in concurrency and receive abnormal UL release
                     ensure pending_dl_tbf_ptr is setup CR 89483
10/02/06   ws        Fixed missing parenthesis in previous checkin
03/02/06   rm        Addressing CR85685 Removing Continous timing Advance in DTM
20/01/05   og        Add an invalid test mode value, resolves Lint
                     Error 64( Type mismatch (assignment) (int/enum).
13/01/06   ws        Addded NulL pointer check in MAC_L1_PDCH_REL_TS handler CR 85981
06/01/06   og        Fix to SRB loopback reconfiguration handling, resolves CR 85231.
17/11/05   ws        Ensure RRBP's are not added to RRBP store during SRB loopback
08/11/05   nt        When cancelling a pending reconfig in DTM issue the event
                     L1_DTM_CANCEL_PS_RECONFIG to the DTM FSM to move back to DTM_RUN
28/10/05   og        Merge fix to CR 80685 from the raven branch.
19/09/05   nt        Multiple changes to support the use of MAC_L1_REALEASE,
                     MAC_L1_TS_RECONFIG, MAC_L1_DYNAMIC_TBF_CONFIG, MAC_L1_DL_TBF_CONFIG
                     for DTM. Also found and fixed releasing bug in
                     gpl1_gprs_handle_tbf_release();
15/09/05   ws        Added FEATURE_GSM_DTM around mac mode defs in MAC->L1 MSGs
14/09/05   og        Fix to the interruption of a queued UL release by a PDCH
                     release while MS is in concurrent TBF.
22/08/05   og        Add the setting of mac mode to NPL1.
17/08/05   og        Removal of dummy ctrl block dual buffer.
22/07/05   og        Fix to CR70508. Ensure the ul release confirm is sent to MAC
                     when the release msg is followed immediately by an ul assignment.
27/06/05   og        Fix to CR67703. Prevent the queued uplink release from
                     upsetting the pending packet timeslot reconfigure.
08/06/05   og        Added MSG ERROR printouts to track start time issue seen
                     in CR62533.
02/06/05   og        Removal of header-header includes.
31/05/05   ws        CMU200 Test mode B specific: if test_mode_tn is 0 then loopback lowest tn
26/05/05   ws        CR 64279 If waiting for DL PCA to perform DL release and we receive UL
                     Release then relase confirm DL and wait for UL PCA before release
12/05/05   nt        CR63848 The pending allocation should be kept as L1_DYNAMIC_DOWN_ALLOC
                     when releasing an uplink.
25/04/05   nt        changed gpl1_gprs_handle_tbf_release to alter a pending alloc
                     rather than deleting it, depending upon the release request from MAC.
21/04/05   og        Fix to the queuing a downlink assignment and an uplink release
                     at the same time.
21/04/05   ws        Ensure rel_alloc_ts is not overwritten in MAC_L1_PDCH_REL_TS
11/4/05    og        Added protection against multiple ul/dl normal release messages.
21/3/05    og        Updates to handle dl release immediately followed by a
                     queued uplink release.
17/3/05    og        Updates to srb loopback handling following integration
                     on raven target.
11/03/05   sv        Mainlined FEATURE_GSM_CB.
11/3/05    og        Changes to egprs dl tfi and IR handling.
18/02/05   og        Reset the ul_st when the main command is nulled due to
                     an uplink tbf release message from MAC.
17/02/05   og        Merge to fix from saber 4x branch ( no associated CR )
                     introduced by change 166619.
16/2/05    og        Add more detail/robustness to tbf assignment in progress
                     checks when a MAC L1 RELEASE message is received.
17/1/05    og        Fix to timeslot alloc bitmap use in while loops.
17/01/05   ws        Lint updates
14/01/05   ws        Reinstated setting of test mode B ul_nts
13/1/05    og        MS timebase adjustements confined to radio block gap.
06/12/04   ws        Updates for TEST mode B for GPRS
16/12/04   og        Merge of fix from the saber 4x branch.
25/11/04   ws        Added check for PCA tx time if it occurs after reception
                     of PTR, PUAS or PDAS with no Start time then switch to new
                     channel 2 radio blocks after PCA has been sent.
10/11/04   ws        Removed setting of tbf_mode for reassignment of TBF
02/11/04   og        Addition of egprs srb test mode capability.
28/10/04   ws        Lint tidy up
28/10/04   og        Fix to de-referencing null single_block_tbf_ptr.
19/10/04   og        Added single block bep period2 handling.
15/10/04   og        Removed Fixed TBF allocation related source code.
15/10/04   ws        Moved preservation of sched_msgs to ISR Context
08/10/04   ws        Release handler updates for not sending PH_CONNECT_IND if
                     we are releasing pending assignment.
08/10/04   ws        Don't overwrite sched_msgs in DYNAMIC and PTR handlers.
                     Results in RLC UL FIFO getting out of sync with PL1
05/10/04   dlh       Added handling for MPH_PBCCH_DESCRIPTION_IND.
17/09/04   ws        Don't check frequency params if reuse_sb_freq is set as
                     frequency list is not valid
02/08/04   ws        If we are releasing Uplink TBF while waiting for PUA starting
                     time to expire, force main commad to NULL (CR 46665)
21/07/04   ws        Ignore MAC_L1_PDCH_REL_TS msg if we are waiting for starting time or
                     a release to happen
09/07/04   ws        Modified release mechanism for abnormal cases to ensure PCA and
                     and PDAN_w_FAI RRBP's are sent
23/06/04   og        Egprs Qual measurements handling.
15/06/04   ws        Corrected typo in setting pending_alloc in DL release
10/06/04   kf        added L1_FULL_RELEASE_UL to release DL_TBF_NORMAL
08/06/04   kf        added the handeling of MPH_UE_MODE_CHANGE_REQ
07/06/04   ws        Reverted release confirm for UL+DL release to UL+DL cause
03/06/04   kf        if FAI UL PCA going out before start time go to dl only.
02/06/04   sv        Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
02/06/04   ws        backed out previous release_pending change and instead send DL release cnf
                     if an UL tbf release is pending
18/05/04   ws        if PDAS is rxd when release_pending is UL+DL then only release UL and
                     wait for PDAS starting time
10/05/04   ws        Modified release handler for UL abnormal to only release UL if
                     in concurrency release and for UL + DL abnormal to release confirm
                     what TBF is active.
07/05/04   og        Removal of Lint warnings.
28/04/04   rm        Replaced existing ta and rrbp codes with function calls to
                     rrbp and ptcch modules. Also cleaned up lint errors
26/04/04   ws        Check that no release and no starting time is pending before
                     reconfiguring due to MAC_L1_POWER_CONTROL
23/04/04   ws        Corrected Lint Error's
20/04/04   ws        Added check to only connect DL TBF in PTR handler if UL is to
                     be released
02/04/04   gw        Added processing of messages from CB task.
01/04/04   ws        Fixed compile warnings in gpl1_gprs_check_freq_params()
23/03/04   ws        Added tbf_freq_cnf_t handling to MAC_L1_FREQ_CONFIG handler and
                     modified L1_send_l1_mac_freq_ass_conf() to include tbf_freq_cnf_t
11/03/04   ws        Added Clearing of ta param pointer is null to stop cont. TA params
                     swapping to UL
09/03/04   ws        Added additional check to ensure we don't overwrite disable_rx_tx
                     when handling MAC_L1_PDCH_TS_REL msg
09/03/04   ws        Added adjustment of Starting time for assignments if PCA tx is
                     scheduled subsequent to assignment
09/03/04   ws        Added min reaction starting time to assignmnets with no starting times
02/02/04   kf        Added MPH_READ_SYS_INFO_REQ & MPH_STOP_READ_SYS_INFO_REQ.
27/01/04   kf        setting of band for ul power control
14/01/04   ws        Removed waiting for DL RRBP when attempting to release DL TBF
12/01/04   kf        Added power control to second phase of two phase access.
07/01/04   ws        Added reseting main_command to NULL when we release UL+DL to
                     ensure race condition does not occur from a previous UL TBF release
07/01/04   ws        Added sending of RRBP_FAILURE_IND to MAC when PDAN with CRD RRBP
                     can't be sent
10/12/03   ws        Added Intlock around responding to MAC_L1_FREQ_CONFIG to avoid
                     assignment issues in MAC
05/12/03   ws        Added case to handle establishing DL TBF during full release of ul
26/11/03   ws        Lint cleanup
24/11/03   ws        Added handling of starting time expirey's in PUA's PDA's and PTR's
06/11/03   gfr       Support for quad-band.
05/11/03   ws        Added additional check to MAC_L1_POWER_CONTROL handler to not
                     ignore received TA value if we currently don't have a valid TA value
05/11/03   kf        Added the use of SIZE_OF_RRBP_BUFF
29/10/03   kf        Fix for decrementing the num of rrbp msgs
20/10/03   kf        Removed reseting of alpha and gamma parameters for power control
14/10/03   npr       Removed gamma clearance for non-referenced TNs in handling
                     power control info from MAC in TS Reconfig and Power Ctrl Msgs
01/10/03   ws        Check if TAV has changed in MAC_L1_POWER_CONTROL handler
01/10/03   ws        Removed deleting RRBP's for PDA,PUA and PTR if blocks gap
                     is detected
26/09/03   dlh       Modified Aquire PSI message slightly.
19/09/03   ws        Added starting case if both ul and dl starting times are
                     invalid to PTR
19/09/03   pjr       Lint tidy
18/09/03   pjr       Added code to handle starting time for assignment whilst
                     another assignement is currently in progress.
12/09/03   pjr       Added code to handle starting time for assignment whilst
                     release in progress
11/09/03   ws        Added call to l1_sc_stop() to MPH_STOP_GSM_MODE_REQ handler
05/09/03   pjr       Modified handling of starting time in RECONFIG message handler.
04/09/03   sw        Added message handler for MPH_WCDMA_CELL_UPDATE_LIST_REQ
03/09/03   ws        Modified lowest tn chaged function to return true only if
                     the pending lowest tn is less than lowest current tn.
02/09/03   dlh       Modified some F3's.
02/09/03   ws        Fixed bug with pending_dl_tbf_ptr overwriting current
                     dl_tbf_ptr in PTR handler
22/08/03   ws        Fixed handling of cont ta params in PTR
22/08/03   dlh       Added MPH_STOP_PSI_ACQUIRE_REQ message handling.
21/08/03   ws        Added debug for cont ta params in PTR
20/08/03   pjr       If no power control values are contained in TBF_CONFIG signals
                     added clearing of gamma_tn and alpha values to force PMAX tx power
15/08/03   ws        Added handling of single block reuse flag for frequency handling
30/07/03   pjr       Added check for in progress PDA when an uplink release is received.
24/07/03   pjr       Modified dynamic config to handle expired starting times.
22/07/03   ws        Stopped MAC_L1_PDCH_REL_TS from reconfiguring if a release is pending
14/07/03   bk        Moved gpl1_gprs_setup_start_idle() out of gpl1_gprs_handle_tbf_release() as
                     this function needs to be executed without interrupts locked.
11/07/03   ws        Forced sub_state to DOWNLINK_1 or DOWNLINK_2 to allow return to idle
10/07/03   bk        Changed the prototype for gpl1_gprs_setup_start_idle so that it
                     can be called from the task context. Moved l1_sc_start out of critical
                     section.
10/07/03   ws        Check added to ensure we don't stop SCE if we are leaving single block
08/07/03   kf        changed surround update req
08/07/03   gw        GRR/L1 SCE cleanup.
27/06/03   ws        Set starting time to invalid Fn when releasing ul+dl abnormally
26/06/03   ws        When releasing Ul TBF and a pending ul tbf is waiting for start
                     time. Starting time is cleared to avoid PH_CONNECT_IND being sent
                     to MAC.
25/06/03   gw        Use gpl1_gprs_setup_start_idle() to go to Idle Mode.
23/06/03   gfr       Compiler warning cleanup.
20/06/03   ws        Ensure we are not trying to reconfigure tbf when releasing tn's
20/06/03   pjr       Added recording of rrbp frame number in quality report requests.
20/06/03   ws        Added RRBP FN checking to MAC_L1_PDCH_REL_TN.
                     Added tbf suspend check to ul/dl allocation handlers to avoid
                     transfer mode being left in a permenant suspend state.
06/19/03   gw        Changes for starting/stopping SCE going in/out of
                     52 Idle Mode.
19/06/03   gw        Changes to starting and stopping of SCE.
18/06/03   pjr       Added call to function gpl1_gprs_calc_rf_pwr() to update
                     uplink power control whenever the power control parameters
                     are updated.
17/06/03   ws        Ensure we don't have a release pending before actioning new params in
                     MAC_L1_POWER_CONTROL
17/06/03   ws        Added Clearing of starting time if an assignment is
                     pending while attemting to release ul
11/06/03   gw        Removed call to gpl1_gprs_update_bcch_stop() from handler
                     for MPH_GPRS_SURROUND_UPDATE_REQ.
10/06/03   pjr       Added handling of PDA whilst waiting for PUA starting time
                     and PUA whilst waiting for PDA starting time.
                     Modified MPH_GPRS_L1PARAM_UPDATE_REQ handler.
10/06/03   ws        Removed clearing out rlc_fifo_status on contension resoloution
                     and cs change. Disabled RLC UL FIFO when ul tbf is released.
30/05/03   ag        Added MPH_ACQUIRE_PSI_REQ.
29/05/03   kf        ncell pbcch read changse
29/05/03   ws        Fixed missing dl tfi from MAC_L1_PDCH_REL_TS signal handler
28/05/03   ws        Corrected pending_dynamic_tbf_ptr in MAC_L1_RELEASE_TS
23/05/03   ws        Updated BSIC TRANSFER feature
23/05/03   ws        Added missing num_rrbp_msgs--
20/05/03   ws        Modifications to use correct starting time calculation
15/05/03   ws        Added MPH_GSM_STOP_MODE_REQ, added check for ignoring
                     cont. TA params if they have not changed.
14/05/03   pjr       Changed quality report handling to increment the number of
                     quality reports to send.
14/05/03   pjr       Modified QUALITY REPORT TIME to handle multiple timeslots
05/12/03   dp        Added code to handle BSIC transfer with timeslot
                     reconfiguration
02/05/03   gw        Merged in power transfer changes from branch.
02/05/03   pjr       Moved suspension of interference measurements to
                     gpl1_gprs_transfer.c
01/05/03   ag        Updates to ncell pbcch reading.
30/04/03   pjr       Added suspension of interference measurements due to reconfigure.
28/04/03   gw        Modified INTLOCK'ing.
02/04/03   gw        Changes to reflect modifications in monsan module.
17/04/03   DLH       Added handling for MPH_GPRS_L1PARAM_UPDATE_REQ
04/17/03   DLH       Added fix for page mode SAME_AS_BEFORE case
09/04/03   pjr       Added support for setting start time after pending RRBP when
                     moving into dynamic from single block.
28/02/03   kf        #ifdefed ncell pbcch reading
27/02/03   pjr       Corrected bug in UL_TBF_ABNORMAL release in setting of
                     l1_tskisr_blk->main_command
26/02/03   npr       Changed instances of INVALID to GPL1_INVALID
21/02/03   DLH       Modified mac_l1_page_mode and mm_non_drx_mode functions.
20/02/03   pjr       Added handling of rrbp scheduled in idle for transmission
                     in transfer.
13/02/03   ws        Updates for DL power control
06/02/03   ws        Corrected Bug with ignoring TS reconfig if received during
                     downlink TBF only.
06/02/03   ws        Added support for deleting non-important RRBP if TS reconfigure
                     gap is needed. Also added Debug msgs for multislot
05/02/03   ws        Added PCA_UL_FAI to RRBP ul msg count
05/02/03   ws        Adjusted min reaction time to TEMP_DELAY_OS for schedule change
17/01/03   ws        Added RRBP scanning to DYNAMIC and DL TBF configs
16/01/03   ws        Added RRBP scanning for Timeslot reconfigure when a block's gap is needed
08/01/03   pjr       Added setting of gprs_serv_cell_meas.gamma_gsm for dual band.
07/01/02   ws        Added correct band checking of ARFCN if FEATURE_MULTIBAND is defined
19/12/02   pjr       Removed storing of power control parameters in l1_transfer_data[gas_id]
                     structure.
12/02/02   ws        Added handling of MPH_START_MM_NON_DRX_IND and MPH_STOP_MM_NON_DRX_IND
11/22/02   ws        Added stopping of T3192 for DL ASS and PTR ASS
11/22/02   pjr       Added L1_SINGLE_BLOCK_ALLOC to UL_TBF_NORMAL release
11/20/02   DLH       Added MPH_START_MM_NON_DRX_IND and MPH_STOP_MM_NON_DRX_IND
14/11/02   ws        Added test for which idle mode to return back to.
09/11/02   ws        Added case to modify pending release if we are in concurrency
                     and we get an ul release followed by a dl relelase
08/11/02   pjr       Made quality reporting time an array to handle pending
                     ack/nacks. Modified timing advance for single block
08/11/02   ws        Reset sched_msgs in dynamic struct to zero in
                     MAC_L1_DYN_TBF_CONFIG
06/11/02   ws        Updated tbf release mechanism to support full release and
                     abnormal release of ul and dl tbf
04/11/02   pjr       Merge Neighbour cell measurements under feature switch
01/11/02   pjr       Added serving cell measurements under feature switch
28/10/02   ws        Ensured release_cnf message is sent if returning to idle
                     without waiting for ul or dl tbf and additional testing for
                     concurrency alloc type when receiving MAC_L1_RELEASE
10/21/02   he        Replaced FN by gl1_get_FN().
16/10/02   pjr       Changed start time for no supplied start time to FN+4
11/10/02   ws        Re-enabled CS change flag
10/10/02   DLH       Removed MAC_L1_PAGE_MODE handler
10/10/02   DLH       Added extra param to mac access function
09/09/02   ws        Moved INTLOCK to stop too many RRBP's being droped
20/09/02   ws        moved setting of Main_command in MAC_L1_DYNAMIC_TBF_CONFIG
18/09/02   DLH       Added mph_gprs_l1param_update_req_t handler.
08/28/02   DLH       Added page mode handling while in transfer mode
13/09/02   pjr       Temporary removal of CS reconfiguration handling.
12/09/02   pjr       Removed MAC_PH_RANDOM_ACCESS_REQ as no longer required in this
                     mode.
11/09/02   pjr       Added PDAN_WITH_FAI an PDAN_WITH_CRD handling and clearing of
                     buffers in CONTENTION_RESOLUTION handling.
06/09/02   ws        Added temporary soloution for receiving RRBP after the FN has
                     passed and cleared out RRBP buffer for Abnormal releases
05/09/02   pjr       Added handling of MAC_L1_DUMMY_CTRL_BLOCK signal and modified
                     freq_conf, dynamic_conf and single_block_conf to allow transition
                     from single block to dynamic.
3/09/02    ws        Removed setting of sub state when returning to IDLE from TRANSFER
08/08/02   ws        Added PRACH handler and corrected Lint errors and warnings
01/08/02   ws        Added changes to RRBP handler for PCA_UL and PCA_DL
                     Added changes to switch cont. ta params if tbf is released
                     Changed FEATURE_GPRS to FEATURE_GSM_GPRS_L1
24/07/02   ws        Changes relating to handling releases for PDCH_TS_REL
22/07/02   ws        corrected bug in handling of MAC_L1_FREQ_CONFIG where data was
                     being written to wrong data structure
18/07/02   ws        Modified setting allow_one_rb_gap to false if lowest numbered
                     does not change between allocation
15/07/02   ws        Added gpl1_gprs_chk_blk_bndry() to adjust RRBP FN if it does
                     not lie on the first frame of a radio block
15/07/02   ws        Added support for signals to allow reconfiguration of the tbf
02/07/02   ws        Added gpl1_gprs_lowest_tn_changed(), and added tbf_est_ind
                     to allow MAC_PH_CONNECT_IND to be configured correctly
27/06/02   ws        Added support of gpl1_gprs_get_starting_time()
11/06/02   pjr       Added handling of MAC_L1_CONTENTION_RES and
                     MAC_L1_QUALITY_RPT signals.
31/05/02   ws        Added MAC_L1_DL_TBF_CONFIG code for TBF reconfiguration
27/11/01   ws        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"
#include "gl1_msg_g.h"
#include "gpl1_gprs_task.h"
#include "gpl1_gprs_if.h"
#include "l1_isr.h"
#include "gpl1_gprs_isr.h"
#include "gmacl1.h"
#include "grlcl1.h"
#include "l1_sc_int.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "ms.h"
#include "sys_stru.h"
#include "memory.h" // For memcpy()
#include <stringl/stringl.h>
#include "msg.h" //for f3 message
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_ptcch.h"
#include "gprsdef.h"
#include "gprsdef_g.h"


#include "l1_sc.h"



#include "gpl1_gprs_log.h"


#include "l1_smscb.h"
#include "cbl1if.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_utils.h"
#include "l1i.h"

#ifdef FEATURE_GPRS_PS_HANDOVER
#include "gl1_msg_pdch.h" /* @@OG: Need to access the timing advance */
#endif
#include "geran_eng_mode_info.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_cmdi.h"
#include "gplt_ctl.h"
#endif /*FEATURE_GL1_GPLT*/

#include "gpl1_grm_intf.h"

#ifdef FEATURE_GPRS_GBTA
#include "gpl1_dual_sim.h"
#endif /* FEATURE_GPRS_GBTA */

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
#include "geran_dual_sim_g.h"
#endif

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */

#include "l1_task.h"

extern dedicated_data_T l1_dedicated_data[];
#ifdef FEATURE_GSM_TDS
extern boolean l1_sc_irat_performing_acq_meas(gas_id_t gas_id);
extern int     l1_g2t_delay_tbf_rel;
extern boolean l1_sc_irat_performing_ded_meas(gas_id_t gas_id);
extern boolean mdsp_decrement_cleanup_time_offset_flag[];
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/* #defines */

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

/* @@OG: EDTM design */

/* Used to define number of possible TBF types present
 * NO_TBF, UL_TBF, DL_TBF, UL_DL_TBF.
 */
#define  GPL1_NUM_TBF_TYPES  (4)

#endif  /* FEATURE_GSM_EDTM && FEATURE_GSM_DTM */

#define  GPL1_WAITING_PERIOD_FOR_UL_REL_CNF  (9)
#define  GPL1_NO_OF_FRAMES_IN_BLOCK  (4)
/* Local function declaration */

/* This functions is used to work out the lowest timeslot from the
 * supplied timeslot allocation. compare it with the current timeslot
 * and return TRUE/FALSE if the it is different/not.
 */
static boolean  gpl1_gprs_lowest_tn_changed( uint8  timeslot_alloc,
                                             uint8  current_timeslot );

/* These functions are used in packet transfer to check for the error condition
 * whereby L1 is releasing dl/ul and it receives an extra release message
 * from MAC.
 */
static boolean  gpl1_assess_dl_tbf_release( const transfer_data_T*  transfer_ptr, gas_id_t gas_id );
static boolean  gpl1_assess_ul_tbf_release( const transfer_data_T*  transfer_ptr, gas_id_t gas_id );

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

/* @@OG: EDTM design */
/* Lookup table to analyse value of dtm_tbf_type_t enum
 * Used to indicate the presence of a DL tbf in the dtm assignment
 * message.
 */
static const boolean  GPL1_IS_DL_TBF_PRESENT[ GPL1_NUM_TBF_TYPES ] =
{
  FALSE,
  FALSE,
  TRUE,
  TRUE
};
/* @@OG: EDTM design */
/* Lookup table to analyse value of dtm_tbf_type_t enum
 * Used to indicate the presence of a UL tbf in the dtm assignment
 * message.
 */
static const boolean  GPL1_IS_UL_TBF_PRESENT[ GPL1_NUM_TBF_TYPES ] =
{
  FALSE,
  TRUE,
  FALSE,
  TRUE
};
#endif  /* FEATURE_GSM_DTM && FEATURE_GSM_EDTM */
#ifdef FEATURE_G2X_TUNEAWAY
void gpl1_gprs_delete_rrbp_with_FAI(gas_id_t gas_id);
#endif
#ifdef FEATURE_GPRS_PS_HANDOVER
boolean get_l1_psho_revert_command( gas_id_t gas_id );
void set_l1_psho_revert_command(boolean l1_psho_revert_command,gas_id_t gas_id);

typedef struct
{

/* Flag to indicate if need to return
 * to current cell during PSHO. This is set
 * when MAC_L1_PSHO_STATUS indicates failure.
 */
 boolean l1_psho_revert_command;

}gpl1_gprs_transfer_if_data_t;

/* Local function prototypes */
static gpl1_gprs_transfer_if_data_t  gpl1_gprs_transfer_if_data[NUM_GERAN_DATA_SPACES];

#endif /* FEATURE_GPRS_PS_HANDOVER */
/*===========================================================================

FUNCTION gpl1_gprs_lowest_tn_changed

DESCRIPTION
  This function finds the lowest timeslot in the supplied allocation and
  compares it to the supplied current timeslot.

DEPENDENCIES
  None

RETURN VALUE
  TRUE   : if the lowest numbered timeslot has changed
  FALSE  : if the lowest numbered timeslot has not changed

SIDE EFFECTS
  None

===========================================================================*/
static boolean  gpl1_gprs_lowest_tn_changed( uint8  pl1_timeslot_alloc,     /* reverse of etsi spec timeslot alloc */
                                             uint8  slammed_timeslot )
{
    uint32  timeslot        = 0;
    uint32  lowest_timeslot = GPL1_INVALID;

    while( timeslot < GL1_DEFS_SLOTS_IN_FRAME )
    {
        if( 0 != ( pl1_timeslot_alloc & ( 0x0001 << timeslot ) ) )
        {
            lowest_timeslot = timeslot;
            break;
        }
        ++timeslot;
    }

    if( ( lowest_timeslot != slammed_timeslot )
      &&( GPL1_INVALID != lowest_timeslot ) )
    {
        MSG_GERAN_HIGH_0(" lowest_tn_change ");

        return  TRUE;
    }
    else
    {
        MSG_GERAN_HIGH_0(" lowest_tn_no_change ");
        return  FALSE;
    }
}

/*===========================================================================

FUNCTION  gpl1_assess_dl_tbf_release

DESCRIPTION
  This function assesses the validity of the release tbf command received
  from MAC.

DEPENDENCIES
  None

RETURN VALUE
  boolean  accept_release

SIDE EFFECTS
  None

===========================================================================*/
static boolean  gpl1_assess_dl_tbf_release( const transfer_data_T *transfer_ptr, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    boolean  accept_release = TRUE;

    /* Check here for extra dl release messages
     * and ignore them.
     */
    switch( transfer_ptr->release_pending )
    {
      case  L1_FULL_RELEASE_DL:
      case  L1_PARTIAL_RELEASE_DL:
      {
        /* if already in release but ask to release with with no rrbp , perform release.*/
        if (! gpl1_gprs_get_dl_num_rrbp_msg(gas_id))
        {
          MSG_GERAN_HIGH_0_G( "dl release duplicate honoured as NO RRBP");
        }
        else
        {
          /*  dl release is already in progress, report an error and
          * ignore the msg.
          */
          MSG_GERAN_ERROR_0_G( "dl release ignored, release in progress ");

          /* Exit function
          * make sure the return to idle flag is FALSE.
          */
          accept_release = FALSE;
        }
      }
      break; /*lint !e527 : unreachable break statement */

      case  L1_FULL_RELEASE_UL_DL :

         /*  ul_dl release is already in progress, report an error and
         * ignore the msg.
         */
          MSG_GERAN_ERROR_0_G( "dl release ignored, release in progress ");

         /* Exit function
         * make sure the return to idle flag is FALSE.
         */
         accept_release = FALSE;

      break;

      default:
        /* No action required here */
      break;
    }

    if( FALSE != accept_release )
    {
      switch( transfer_ptr->current_alloc_type )
      {
        case  L1_DYNAMIC_ALLOC:
        {
          MSG_GERAN_ERROR_0_G( "dl release ignored, ul tbf only present ");

          /* Exit function
           * Make sure the return to idle flag is FALSE.
           */
          accept_release = FALSE;
        }
        break;

        case  L1_DYNAMIC_DOWN_ALLOC:
        {
          if( ( transfer_ptr->pending_alloc_type == L1_DYNAMIC_ALLOC )
              ||( transfer_ptr->pending_alloc_type == L1_NULL_ALLOC ) )
          {
            MSG_GERAN_ERROR_0_G( " dl release ignored ");

            /* Exit function
             * Make sure the return to idle flag is FALSE.
             */
            accept_release = FALSE;
          }
        }
        break;

      default:

        switch( l1_tskisr_blk->main_command )
        {
          case  L1_IDLE_COMMAND:

            if(transfer_ptr->current_alloc_type != L1_SINGLE_BLOCK_ALLOC)
            {
              MSG_GERAN_ERROR_0_G( " dl release ignored ");

              /* Exit function
               * Make sure the return to idle flag is FALSE.
               */
              accept_release = FALSE;
            }

          break; /*lint !e527 : unreachable break statement */

          default:
            /* No action required here */
          break;
        } /* End of switch statement on main command */

        break;
      } /* End of switch on current alloc type */
    }   /* End of if accept_release != FALSE */

    return  accept_release;

} /* End of gpl1_filter_extra_dl_release */

/*===========================================================================

FUNCTION  gpl1_assess_ul_tbf_release

DESCRIPTION
   This function assesses the validity of the release tbf command received
  from MAC.

DEPENDENCIES
  None

RETURN VALUE
  boolean  accept_release

SIDE EFFECTS
  None

===========================================================================*/
static boolean  gpl1_assess_ul_tbf_release( const transfer_data_T *transfer_ptr, gas_id_t gas_id )
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    boolean  accept_release = TRUE;

    switch( transfer_ptr->release_pending )
    {
    case  L1_FULL_RELEASE_UL:
    case  L1_PARTIAL_RELEASE_UL:

     /* if already in release but ask to release with with no rrbp , perform release.*/
     if (! gpl1_gprs_get_ul_num_rrbp_msg(gas_id))
     {
       MSG_GERAN_HIGH_0_G( "ul release duplicate honoured as NO RRBP");
     }
     else
     {
       /*  ul release is already in progress, report an error and
       * ignore the msg.
       */
       MSG_GERAN_ERROR_0_G( " ul release ignored, release in progress ");

       /* Exit function
       * make sure the return to idle flag is FALSE.
       */
       accept_release =  FALSE;
     }
     break;

     case  L1_FULL_RELEASE_UL_DL :

       /*  ul_dl release is already in progress, report an error and
       * ignore the msg.
       */
       MSG_GERAN_ERROR_0_G( "ul release ignored, release in progress ");

       /* Exit function
       * make sure the return to idle flag is FALSE.
       */
       accept_release =  FALSE;

      break;

    default:
      /* No action required here */
      break;
    } /* End of switch on release pending */

    if( FALSE != accept_release )
    {
      switch( transfer_ptr->current_alloc_type )
      {
      case  L1_DOWNLINK_ALLOC:
      {
        MSG_GERAN_ERROR_0_G( " ul release ignored, dl tbf only present ");

        /* Exit function
         * Make sure the return to idle flag is FALSE.
         */
        return  FALSE;
      }

      case  L1_DYNAMIC_DOWN_ALLOC:
      {
        if( ( transfer_ptr->pending_alloc_type == L1_DOWNLINK_ALLOC )
          ||( transfer_ptr->pending_alloc_type == L1_NULL_ALLOC ) )
        {
          MSG_GERAN_ERROR_0_G( " ul release ignored ");

          /* Exit function
           * Make sure the return to idle flag is FALSE.
           */
          return  FALSE;
        }
      }
      break;

      default:

        switch( l1_tskisr_blk->main_command )
        {
        case  L1_IDLE_COMMAND:

          if(transfer_ptr->current_alloc_type != L1_SINGLE_BLOCK_ALLOC)
          {
            MSG_GERAN_ERROR_0_G( " ul release ignored ");

            /* Exit function
             * Make sure the return to idle flag is FALSE.
             */
            return  FALSE;
          }

        default:
          /* No action required here */
          break;
        } /* End of switch statement on main command */

        break;
      } /* End of switch on current alloc type */
    }

    return  accept_release;

} /* End of gpl1_filter_extra_dl_release */

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

/*===========================================================================

FUNCTION  gpl1_gprs_handle_edtm_release

DESCRIPTION
   This function handles the  functionality of moving from TRANS TO IDLE
   duirng EDTM race condition case.

DEPENDENCIES
  None

RETURN VALUE
  boolean  accept_release

SIDE EFFECTS
  None

===========================================================================*/

static void gpl1_gprs_handle_edtm_release(gas_id_t  gas_id)
{

      volatile ISRTIM_CMD_BLK* l1_tskisr_blk;

     /* This is race condition during EDTM case, Gl1 received DTM_CHANNEL_ASSIGN_REQ.
      And before Gl1 connected to EDTM Gl1 received MAC_L1_RELEASE In this case Gl1 releases the ongoing TBF
      and goes to IDLE and clears EDTM resources
      All below Cases are covered
      DL+UL XFER->EDTM  + UL REL
      DL+UL XFER->EDTM  + DL REL
      UL XFER-> (UL+DL) EDTM  + UL REL
      UL XFER-> (UL+DL) EDTM  + DL REL
      UL XFER-> (UL+DL) EDTM  + (UL+DL) REL
      DL XFER-> (UL+DL) EDTM  + UL REL
      DL XFER-> (UL+DL) EDTM  + DL REL
      DL XFER-> (UL+DL) EDTM  + (UL+DL) REL
   
    */

     l1_tskisr_blk = &l1_tsk_buffer[gas_id];
     gpl1_set_dtm_assig_status(FALSE, gas_id);
     /* turn off RLC UL FIFO as all data has been sucessfully sent */
     l1_transfer_data[gas_id].rlc_ul_data_available = FALSE;
     
     /* Gl1 to send release confirmation to MAC with TBF type same as TBF type received
      in DTM CHANNEL ASSIGNMENT*/
     switch(l1_transfer_data[gas_id].tbf_est_type)
     {
        case UPLINK_TBF_ESTABLISHED:
        {
           l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL;
        }
        break;
        case DOWNLINK_TBF_ESTABLISHED:
        {
           l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_DL;
        }
        break;
        case UPLINK_DOWNLINK_TBF_ESTABLISHED:
        {
           l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL_DL;
        }
        break;
        default:
        MSG_GERAN_MED_1_G("Unexpected TBF EST Type",l1_transfer_data[gas_id].tbf_est_type);
     }
     
     /* do we have a pending starting time for a ul or dl tbf ? */

     l1_transfer_data[gas_id].starting_time = L1_GPRS_INVALID_FN;
     l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
     l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
     
     l1_tskisr_blk->main_command = L1_NULL_COMMAND;
     l1_tskisr_blk->command = L1_NULL_COMMAND;
     
     /*Reset the Wait For PCA FN/Valid variables.to ignore prior waiting RRBP*/
     l1_transfer_data[gas_id].wait_for_pca_fn = L1_GPRS_INVALID_FN;
     l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;
     
     MSG_GERAN_MED_0_G("EDTM Race condition. Going to IDLE");
     
     /* Ensure that the release confirm will be sent */
     l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
     
     /* Set starting time valid to false as we need to release ASAP */
     l1_transfer_data[gas_id].starting_time_valid = FALSE;
     
     /* set starting time elapsed to true as we do not want the MAC_PH_CONNECT_IND
     * to be sent when we restart transfer_mode
     */
     l1_transfer_data[gas_id].starting_time_elapsed  = TRUE;
       
     l1_sc_stop(gas_id);


}
#endif

/*===========================================================================

FUNCTION  gpl1_gprs_handle_transfer_mode

DESCRIPTION
  This function handles the L1 Task in Transfer mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#define  GL1_PKT_XFER_MAX_WAIT_FRAMES  (4)


void gpl1_gprs_handle_transfer_mode(IMH_T *msg_header)
{
  gas_id_t gas_id;
  inter_task_BCCH_allocation_T  ba_buffer;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
  gas_id_t gas_id_recovery = l1_multi_sim_determine_gas_id(msg_header);
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */

  if( NULL == msg_header )
  {
    MSG_GERAN_ERROR_0( " NULL msg_header ptr ");
    return;
  }

  if (msg_header->message_set == MS_MAC_L1)
  {

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
/*Ignore mac_msgs once recovery started*/
    if( GPL1_RECOVERY_STARTED_STATE == gpl1_gprs_get_recovery_sub_state(gas_id_recovery))
    {
      MSG_GERAN_HIGH_1( "Ignore msg_id=%d pckt recovery started ",msg_header->message_id);
      return;
    }
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */

    switch (msg_header->message_id)
    {


    /*******************************************************
    *                                                      *
    *                MAC_L1_FREQ_CONFIG                    *
    *                                                      *
    ********************************************************/
    case MAC_L1_FREQ_CONFIG:
      {
        mac_l1_freq_config_t  *message_ptr;
        boolean error;
        tbf_freq_cnf_t tbf_type;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_freq_config_t *) msg_header;
        gas_id = message_ptr->gas_id;
        /*lint +e740 */
        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);

        /* Check for un supported frequencies in ma list that are not in the
           supported bands ONLY if we are not re-using our current freq params
         */

        if(message_ptr->reuse_sb_frq_alloc == FALSE)
        {
          error = gpl1_gprs_check_frequency_params(
                                                   &(message_ptr->frequency_information.frequency_list),
                                                   message_ptr->frequency_information.hopping_flag,
                                                    message_ptr->gas_id
                                                  );
        }else
        {
          /* we are re-using the existing frequency desc. so confirm frequency params */
          error = FALSE;
        }

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

        /* @@OG: EDTM design */

        if ( message_ptr->dtm_freq_param_present )
        {
          /* check for un supported frequencies in ma list that
           * are not in the supported bands
           */
          error = gpl1_gprs_check_frequency_params
                            (
                              &(message_ptr->frequency_information.frequency_list),
                              message_ptr->frequency_information.hopping_flag, message_ptr->gas_id
                            );

          /*
           * Copy in the frequency info into the frequency_info_1 structure, there is no
           * TBF at present as L1 is in the dedicated state.
           */
          if( !error )
          {
            l1_transfer_data[gas_id].frequency_info_1 = message_ptr->frequency_information;
          }

          /* Mark MAC_L1_FREQ_CONFIG as received */
          SET_FREQ_CONFIG_RECV(gas_id);
        }
#endif
        /*
        ** CgC moved before msg send
        */
        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

        /* read out tbf type from msg */
        tbf_type = (tbf_freq_cnf_t)message_ptr->tbf_freq_cnf;

        /* send signal to mac to confirm or fail frequency params */
        L1_send_l1_mac_freq_ass_conf(error,tbf_type, message_ptr->gas_id);

        MSG_GERAN_MED_2_G("PL1: MAC_L1_FREQ_CONFIG Rcvd reuse %d sngl blk %d",message_ptr->reuse_sb_frq_alloc,l1_transfer_data[gas_id].l1_called_from_sngl_blk);

        /* if no error then continue and pass new frequency parameters into transfer data*/
        if (error != TRUE)
        {
          if (l1_transfer_data[gas_id].l1_called_from_sngl_blk)
          {
            /* check if the frequency params for the PUA should be reused from the
             * single block assignment
             */
            if (message_ptr->reuse_sb_frq_alloc)
            {
              l1_transfer_data[gas_id].pending_frequency_info_ptr = &l1_transfer_data[gas_id].frequency_info_1;
            } else
            {
              /* Use the frequency params in the message */
              l1_transfer_data[gas_id].frequency_info_2 = message_ptr->frequency_information;
              l1_transfer_data[gas_id].pending_frequency_info_ptr = &l1_transfer_data[gas_id].frequency_info_2;
            }
            /* Mark MAC_L1_FREQ_CONFIG as received */
            SET_FREQ_CONFIG_RECV(gas_id);

          } else
          {
            /* Setup frequency parameters in pending structure */
            if (l1_transfer_data[gas_id].frequency_info_ptr == &l1_transfer_data[gas_id].frequency_info_1)
            {
              l1_transfer_data[gas_id].frequency_info_2 = message_ptr->frequency_information;
              l1_transfer_data[gas_id].pending_frequency_info_ptr = &l1_transfer_data[gas_id].frequency_info_2;

            } else if (l1_transfer_data[gas_id].frequency_info_ptr == &l1_transfer_data[gas_id].frequency_info_2)
            {
              l1_transfer_data[gas_id].frequency_info_1 = message_ptr->frequency_information;
              l1_transfer_data[gas_id].pending_frequency_info_ptr = &l1_transfer_data[gas_id].frequency_info_1;
            }

            /* in transfer mode we will always receive either MAC_L1_DYNAMIC_TBF_CONFIG,
             * MAC_L1_DL_TBF_CONFIG or MAC_L1_TS_CONFIG after we receive the MAC_L1_FREQ_CONFIG
             * where the reallocation of the tbf is handled, hence no reason to do it here
             */
          }

        } /* End of if error */


        break;
      }

      /*******************************************************
      *                                                      *
      *                MAC_L1_DUMMY_CTRL_BLOCK               *
      *                                                      *
      ********************************************************/
    case MAC_L1_DUMMY_CTRL_BLOCK:
      {

        mac_l1_dummy_ctrl_block_t  *message_ptr;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_dummy_ctrl_block_t *) msg_header;
        gas_id = message_ptr->gas_id;
        /*lint +e740 */

        MSG_GERAN_MED_0_G("MAC_L1_DUMMY_CONTROL ");

        memscpy(
              l1_transfer_data[gas_id].dummy_control_block,
              sizeof(l1_transfer_data[gas_id].dummy_control_block),
              message_ptr->dummy_ctrl_block,
              sizeof(message_ptr->dummy_ctrl_block)
              );

        /* Mark mac_l1_dummy_ctrl_block received */
        SET_DUMMY_CTRL_RECV(gas_id);

        break;
      } /* end of case */


      /*******************************************************
      *                                                      *
      *                MAC_L1_DYNAMIC_TBF_CONFIG             *
      *                                                      *
      ********************************************************/
    case MAC_L1_DYNAMIC_TBF_CONFIG:
      {
        mac_l1_dynamic_tbf_config_t  *message_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
        dynamic_tbf_information_T *dynamic_msg_dest_ptr;
        uint8 i;
        /* Initialise the timing advance to zero */
        uint8 timing_advance_value = 0;
        grm_client_enum_t g_client;
       /* This is to know where low ts change has been updated or not. if not updated then we can check for lowest ts change.
        if it is true, low ts activity updation taken care and no need to touch that part again*/
        boolean low_ts_change_uptd = FALSE;  
        uint8  wait_for_isr_count = 0;

        message_ptr = ( mac_l1_dynamic_tbf_config_t *) msg_header;
        gas_id = message_ptr->gas_id;
        g_client = gl1_map_gas_id_to_client_id(gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[message_ptr->gas_id];
        dynamic_msg_dest_ptr=NULL;

        MSG_GERAN_MED_0_G("PL1: MAC_L1_DYNAMIC_TBF_CONFIG Rcvd");


        /*-------------------------------------------------------------------------------
        Start the SCE for the 2nd part of 2 phase access. Start the SCE outside the
        INTLOCKS as the function is very CPU intensive.
        --------------------------------------------------------------------------------*/

        if (l1_transfer_data[gas_id].l1_called_from_sngl_blk)
        {
          l1_sc_start( L1SCModeGprsTrans, message_ptr->gas_id );

          /* Access the single block tbf pointer only if the uplink
           * tbf assignment is received following a single block allocation.
           */
          timing_advance_value = l1_transfer_data[gas_id].single_block_tbf_ptr->
                                                  timing_advance_params.ta_value_ie.ta_value;
        }

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);

         while(( gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TBF_TS_CHANGE, gas_id) == ONGOING )
               && (  wait_for_isr_count <= GL1_PKT_XFER_MAX_WAIT_FRAMES )
             )
        {
          GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
          (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
          (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );

          MSG_GERAN_HIGH_1_G(" wait for isr to get FN, count %d ", wait_for_isr_count );

          wait_for_isr_count++;

          GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
        }

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
        switch( get_queued_mac_l1_edtm_cs_rel_tbf_alloc(message_ptr->gas_id ))
        {
        case  NO_TBF_PRESENT:
        case  DL_TBF_PRESENT:
          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id);
          break;

        case  UL_TBF_PRESENT:

          MSG_GERAN_MED_0_G(" Cancelled enh alloc UL TBF ");

          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id);
          break;

        case  UL_DL_TBF_PRESENT:

          MSG_GERAN_MED_0_G(" Cancelled enh alloc UL TBF, only DL remain. ");

          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id);

          break;

        default:

          MSG_GERAN_ERROR_1_G( " Unknown enh alloc tbf type %u ",
                     get_queued_mac_l1_edtm_cs_rel_tbf_alloc(message_ptr->gas_id ));
          break;
        }
#endif

        /*lint -e740 ccww legacy code */

        /*lint +e740  */

        gpl1_gprs_add_ul_ta_params(message_ptr->dynamic_tbf.timing_advance_params,
                                   l1_transfer_data[gas_id].l1_called_from_sngl_blk,
                                   timing_advance_value,message_ptr->gas_id );

        if (l1_transfer_data[gas_id].l1_called_from_sngl_blk)
        {

         /* For debugging purposes */
         uint8  ul_ts_alloc = 0;


          /* Setup mode to determine whether this TBF is EGPRS or GPRS */
          l1_transfer_data[gas_id].egprs_or_gprs_tbf = message_ptr->dynamic_tbf.tbf_mode;

          /* Setup test mode for EGPRS
           * either TEST_MODE_OFF, TEST_MODE_B or TEST_MODE_SRB
           */
          l1_transfer_data[gas_id].dynamic_tbf_1.test_mode    = message_ptr->dynamic_tbf.test_mode;
          l1_transfer_data[gas_id].dynamic_tbf_1.test_mode_tn = message_ptr->dynamic_tbf.ts_offset;
          l1_transfer_data[gas_id].dynamic_tbf_1.srb_mode     = message_ptr->dynamic_tbf.srb_mode;

          if ( FALSE != message_ptr->dynamic_tbf.bep_period2.present )
          {
            l1_transfer_data[gas_id].dynamic_tbf_1.bep_period2 =
            message_ptr->dynamic_tbf.bep_period2.bep_period2;
          } else
          {
            l1_transfer_data[gas_id].dynamic_tbf_1.bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
          }



#ifdef FEATURE_GSM_DTM
        /* Copy in the mac mode */
          l1_transfer_data[gas_id].dynamic_tbf_1.mac_mode = message_ptr->dynamic_tbf.mac_mode;
#else
          l1_transfer_data[gas_id].dynamic_tbf_1.mac_mode = MAC_L1_MAC_MODE_DYNAMIC;
#endif

          l1_transfer_data[gas_id].dynamic_tbf_2.mac_mode = L1_MAC_MODE_INVALID;

          l1_transfer_data[gas_id].dynamic_tbf_1.usf_granularity = message_ptr->dynamic_tbf.usf_granularity;

          /* setup downlink power control parameter's */
          if (message_ptr->dynamic_tbf.dl_ctrl_param.params_valid)
          {
            l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
            l1_transfer_data[gas_id].dl_power_control.po = message_ptr->dynamic_tbf.dl_ctrl_param.po;
          } else
          {
            l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
          }

          /* starting time not always present in dynamic allocation so check */
          if (message_ptr->dynamic_tbf.tbf_start_time_valid)
          {
            MSG_GERAN_MED_0_G("ST present");
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
            /*  Determine if starting time present and if it needs adjusting  */
            /* NOTE this can only be a 52 style starting time */
            if (gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].dynamic_tbf_1.starting_time,
                                            message_ptr->dynamic_tbf.starting_time,
                                            PCCCH_52, message_ptr->gas_id))
            {


#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
              /*  Starting time has expired, set as if no starting time present */
              /*We want to make sure we have alteast 4 Frames to run ISR before ST*/
              message_ptr->dynamic_tbf.starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME);
#else
              /*Starting time has expired, set as if no starting time present */
              message_ptr->dynamic_tbf.starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

              /*  Re - calculate with new starting time */
              (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].dynamic_tbf_1.starting_time,
                                                message_ptr->dynamic_tbf.starting_time,
                                                PCCCH_52, message_ptr->gas_id);
            }

            l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].dynamic_tbf_1.starting_time;
          } else
          {
            /*  If RRBP pending set start time to block after RRBP has been sent, else set
                starting time to the current frame plus the temp delay. */

            /*  Search rrbp array and set starting time accordingly */
            i= gpl1_gprs_check_for_rrbp_msg(TRUE,message_ptr->gas_id);

            if (i < SIZE_OF_RRBP_BUFF)
            {
              /*  Determine frame number, at this time assume only one rrbp will be outstanding  */
              message_ptr->dynamic_tbf.starting_time = (gpl1_gprs_get_rrbp_frame(i, message_ptr->gas_id) + 8);
            } else
            {

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
              /*We want to make sure we have alteast 4 Frames to run ISR before ST*/
              message_ptr->dynamic_tbf.starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME);
#else
              message_ptr->dynamic_tbf.starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);
#endif/*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/
            }

            message_ptr->dynamic_tbf.tbf_start_time_valid = TRUE;
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
            /*  Determine if starting time present and if it needs adjusting  */
            /* NOTE this can only be an 52 style starting time */
            (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].dynamic_tbf_1.starting_time,
                                              message_ptr->dynamic_tbf.starting_time,
                                              PCCCH_52, message_ptr->gas_id);

            l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].dynamic_tbf_1.starting_time;
          }


          /* Map MAC description of USF values and timeslots into L1_transfer_data */

          /*
            * Mapping element 0 = usf value for TN0
            *         element 7 = usf value for TN7
            *
            * USF value range 0 to 7
            * USF value = 0xff TN not supported
            */

          l1_transfer_data[gas_id].dynamic_tbf_1.nts = 0;
          l1_transfer_data[gas_id].dynamic_tbf_1.sched_msgs = 0;
          for (i=0;i< MAX_TN;i++)
          {
            if (message_ptr->dynamic_tbf.usf[i].present)
            {
              /* For debug purposes */
              ul_ts_alloc |= ( 0x80 >> i );

              l1_transfer_data[gas_id].dynamic_tbf_1.usf_value[i] = message_ptr->dynamic_tbf.usf[i].usf_value;
              l1_transfer_data[gas_id].dynamic_tbf_1.nts++;
            } else
              /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
              l1_transfer_data[gas_id].dynamic_tbf_1.usf_value[i] = GPL1_INVALID;

          }

          if (message_ptr->dynamic_tbf.power_control_params.present)
          {
            uint8 index;
            power_control_gamma_T *pwr_cntrl_ptr;

            /*  Update Alpha value  */
            gprs_serv_cell_meas_ptr->alpha = message_ptr->dynamic_tbf.power_control_params.alpha;

            /*  Extract gamma_tn values  */
            pwr_cntrl_ptr = &(message_ptr->dynamic_tbf.power_control_params.gamma_TN0);
            index = 0;

            do
            {
              if (pwr_cntrl_ptr->present)
              {
                gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
              }


              /*  Increment pointer  */
              pwr_cntrl_ptr++;
              index++;

            }while (index < 8);

          }


          /*  Update uplink power based on the modified power control parameters. */
          gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

          /* Mark mac_l1_dynamic_tbf_config received */
          SET_DYNAMIC_TBF_RECV(gas_id);

          if (DYNAMIC_TBF_ALLOC_COMP(gas_id))
          {
            /* If all signals have been received to fully
              * describe a DYNAMIC UL allocation then
              * move to L1_TRANSFER state and service it
              */
            /* setup tbf establish type  */
            if (l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN)
            {
              l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;
            }
            l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_ALLOC;
            l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;

            RESET_BITMAP(gas_id);

            l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
            l1_tskisr_blk->command = L1_NULL_COMMAND;

            switch( l1_tskisr_blk->main_command )
            {
            case  L1_IDLE_COMMAND:
              /* The idle command has been setup as part of handling an
               * downlink/uplink release message.
               */

              if(  ( SEND_IN_ISR == l1_transfer_data[gas_id].confirm_pending )
                 &&( L1_FULL_RELEASE_UL == l1_transfer_data[gas_id].release_pending ) )
              {
                /* This covers the case where an uplink tbf release is
                 * received during uplink packet transfer and is followed immediately
                 * by an uplink assignment on the same task context switch.
                 */

                /* Clear wait for pca vars */
                l1_transfer_data[gas_id].wait_for_pca_fn    = L1_GPRS_INVALID_FN;
                l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;

                MSG_GERAN_MED_0_G("rel confirm IDLE CMD ");

                /* Send the release confirm message to upper stack */
                gpl1_gprs_send_l1_mac_tbf_rel_confirm( L1_FULL_RELEASE_UL, message_ptr->gas_id );

                /* reset relelase type and rel confirm status*/
                l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
                l1_transfer_data[gas_id].confirm_pending = SENT;

              }
              break;

            default:
              /* No action required here */
              break;
            } /* End of switch on main command */
#ifdef FEATURE_GSM_DTM
            /* Don't set main commands in the DTM mode */
            if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
            {
              /* signal to L1 ISR that a TBF reconfig is pending */
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
            }
#else
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif
          }

          MSG_GERAN_MED_1_G(" ul_ts_alloc  0x%X ", ul_ts_alloc);

          /* ul_ts_alloc used as debug  */
          NOTUSED(ul_ts_alloc);
        } else
        {
         /* For debugging purposes */
         uint8  ul_ts_alloc = 0;
         uint8  l1_ul_ts_alloc = 0;

#ifdef FEATURE_GSM_DTM
          if( (l1_tskisr_blk->l1_state == L1_DTM_MODE) &&
              (l1_transfer_data[gas_id].dynamic_tbf_ptr == &(l1_transfer_data[gas_id].dynamic_tbf_1)) )
          {
            /* Reconfigure dynamic tbf or add to existing DL tbf */
            dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

            /* Setup pending dynamic structure */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;
          }
          else
          if( (l1_tskisr_blk->l1_state == L1_DTM_MODE) &&
              (l1_transfer_data[gas_id].dynamic_tbf_ptr == &(l1_transfer_data[gas_id].dynamic_tbf_2)) )
          {
            /* reconfigure dynamic tbf */
            dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

            /* Setup pending dynamic structure */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

          }
          else
#endif /*FEATURE_GSM_DTM*/
         /* Based on the current sub state work out where to
          * copy new data to
          */
          if ((l1_tskisr_blk->sub_state == L1_DYNAMIC_1) ||
              (l1_tskisr_blk->sub_state == L1_DOWNLINK_1) ||
              (l1_tskisr_blk->sub_state == L1_DOWNLINK_2) )
          {
            /* Reconfigure dynamic tbf or add to existing DL tbf */
            dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

            /* Setup pending dynamic structure */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

          } else if ( l1_tskisr_blk->sub_state == L1_DYNAMIC_2)
          {
            /* reconfigure dynamic tbf */
            dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

            /* Setup pending dynmaic structure */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
          }

          if (!dynamic_msg_dest_ptr)
          {
            GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
            /* something has gone wrong, report error and return */
            MSG_GERAN_ERROR_0_G("PL1: Transfer bad sub_state! ");
            return;
          }

#ifdef FEATURE_GSM_DTM
         /* Copy in the mac mode */
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode = message_ptr->dynamic_tbf.mac_mode;
#else
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode = MAC_L1_MAC_MODE_DYNAMIC;
#endif


          /* Setup mode to determine whether this TBF is EGPRS or GPRS */

          //l1_transfer_data[gas_id].egprs_or_gprs_tbf = message_ptr->dynamic_tbf.tbf_mode;

          /* Setup test mode for EGPRS
           * either TEST_MODE_OFF, TEST_MODE_B or TEST_MODE_SRB
           */
          dynamic_msg_dest_ptr->test_mode = message_ptr->dynamic_tbf.test_mode;
          dynamic_msg_dest_ptr->test_mode_tn = message_ptr->dynamic_tbf.ts_offset;
          dynamic_msg_dest_ptr->srb_mode     = message_ptr->dynamic_tbf.srb_mode;

          /* If the ts offset is zero then only loopback lowest allocated Rx */
          if(message_ptr->dynamic_tbf.ts_offset == 0)
          {
            dynamic_msg_dest_ptr->test_mode_tn = gpl1_gprs_ts_conv_unslam(gprs_serv_cell_meas_ptr->lowest_tn.tn,message_ptr->gas_id);
          }


          if ( FALSE != message_ptr->dynamic_tbf.bep_period2.present )
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->bep_period2 =
            message_ptr->dynamic_tbf.bep_period2.bep_period2;
          } else
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
          }

          /* set up pending allocation type */
          if ((l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC ) ||
              (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) )
          {
            if (l1_transfer_data[gas_id].release_pending == L1_PARTIAL_RELEASE_DL)
            {
              l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;
            }
            else
            if (l1_transfer_data[gas_id].release_pending == L1_FULL_RELEASE_DL)
            {
              l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;
              l1_transfer_data[gas_id].release_pending = L1_PARTIAL_RELEASE_DL;
            } else
            {
              /* moving from dl, or dynamic + dl tbf to dl + dynamic ul tbf */
              l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
            }
          } else
          {
            /* reconfiguring dynamic allocation, still single ul tbf */
            l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;
          }

          /* initialise number of timeslots to zero */
          dynamic_msg_dest_ptr->nts = 0;

          for (i=0;i< MAX_TN;i++)
          {
            if (message_ptr->dynamic_tbf.usf[i].present)
            {
              /* For debug purposes */
              ul_ts_alloc |= ( 0x80 >> i );

              l1_ul_ts_alloc |= (0x0001 << i );

              dynamic_msg_dest_ptr->usf_value[i] = message_ptr->dynamic_tbf.usf[i].usf_value;
              dynamic_msg_dest_ptr->nts++;

              if ((uint8)dynamic_msg_dest_ptr->test_mode == L1_TEST_MODE_B)
              {
                /* Setup test mode B tn allocation */
                gpl1_gprs_tm_b_store[gas_id].ul_nts++;
              }

            } else
              /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
              dynamic_msg_dest_ptr->usf_value[i] = GPL1_INVALID;

          }

          /* Setup new usf granularity FALSE = 1 block tx , TRUE = 4 block tx */
          dynamic_msg_dest_ptr->usf_granularity = message_ptr->dynamic_tbf.usf_granularity;

          /* setup downlink power control parameter's */
          if (message_ptr->dynamic_tbf.dl_ctrl_param.params_valid)
          {
            l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
            l1_transfer_data[gas_id].dl_power_control.po = message_ptr->dynamic_tbf.dl_ctrl_param.po;
          } else
          {
            l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
          }
          /* CR2144408:Check for the case where MAC_L1_RELEASE for DL has been received but Reconfig is not done*/
          if( NULL != l1_transfer_data[gas_id].dl_tbf_ptr && l1_transfer_data[gas_id].current_alloc_type != L1_DYNAMIC_ALLOC )
          {
              /* Make sure the 1rb gap is correctly
               * set to TRUE only when necessary.
               */
              l1_ul_ts_alloc |= l1_transfer_data[gas_id].dl_tbf_ptr->ts_allocation;
          }

          /* check if lowest numbered tn has changed */
          if(gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TBF_TS_CHANGE, gas_id))
          {
              /*check if Pending DL TBF is present*/
              if(l1_transfer_data[gas_id].pending_dl_tbf_ptr!= NULL)
              {
                   /*check new UL allocation + Pending DL allocation with current low TS 
                      if there is lowest TS change then add low TS change at TBF starting time
                     Because low TS change and TBf reconfig will happen at the same time*/
                  if (gpl1_gprs_lowest_tn_changed((l1_transfer_data[gas_id].pending_dl_tbf_ptr->ts_allocation| l1_ul_ts_alloc ), 
                                                      l1_get_current_timeslot(message_ptr->gas_id) ))
                   {
                      l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap =TRUE;
                      low_ts_change_uptd = TRUE;
                   }
                   else
                   {
                     /*no low TS change. We need to cancel the low TS change activity from G2X_TA table*/
                      l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
                      l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;
                      gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_TBF_TS_CHANGE,gas_id);
                      low_ts_change_uptd = TRUE;
                   }
              }
              else
              {
                 /* check lowest TS change with new alloc and current low TS.
                   if there is no low TS change , we need to cancel the lowest timeslot as pending config
                    is overwritten with new UL config*/
                if(!gpl1_gprs_lowest_tn_changed( l1_ul_ts_alloc , l1_get_current_timeslot(gas_id) ) )
                {
                   l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
                   gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_TBF_TS_CHANGE,gas_id);
                   low_ts_change_uptd = TRUE;
                }
              }
          }

          if((!low_ts_change_uptd) && gpl1_gprs_lowest_tn_changed( l1_ul_ts_alloc , l1_get_current_timeslot(message_ptr->gas_id) ) )
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;


            /* Check if there is a PCA or PDAN w FAI or PDAN w CRD RRBP */

            /* Only create a starting time if we haven't got one */
            if (!message_ptr->dynamic_tbf.tbf_start_time_valid)
            {
              message_ptr->dynamic_tbf.tbf_start_time_valid =
              gpl1_gprs_transfer_find_rrbp_type(&message_ptr->dynamic_tbf.starting_time,message_ptr->gas_id);
            }

            else
            {
              /* The start time is brought forward 4 frames so l1 reacts at startime.
                This allows for TBF rx/tx activity suspension when lowest
                downlink timeslot changes.NOTE the start time needs to be checked against
                the block boundary. This is currently checked later if start time is valid.
               */

              boolean bring_starttime_forward = TRUE;
              uint8   rrbp_index = SIZE_OF_RRBP_BUFF;
              uint32  rrbp_fn;
              uint8   diff;

              if (message_ptr->dynamic_tbf.starting_time%13 != 0)
              {
                diff  = GL1_DEFS_FRAMES_IN_BLK_PERIOD;
              }
              else
              {
                diff = GL1_DEFS_FRAMES_IN_BLK_PERIOD + 1;
              }

              rrbp_index = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

              if (rrbp_index < SIZE_OF_RRBP_BUFF)
              {
                rrbp_fn = gpl1_gprs_get_rrbp_frame(rrbp_index, message_ptr->gas_id);
                /* rrbp fn and startime clash hence move starttime by 4 frames
                */
                if ( rrbp_fn == message_ptr->dynamic_tbf.starting_time)
                {
                  bring_starttime_forward = FALSE;
                  message_ptr->dynamic_tbf.starting_time = ADD_FN(message_ptr->dynamic_tbf.starting_time,GL1_DEFS_FRAMES_IN_BLK_PERIOD);
                  MSG_GERAN_HIGH_2_G("Start time delayed 4 frames, start time %d rrbp_fn %d",message_ptr->dynamic_tbf.starting_time,rrbp_fn);
                }
                /* rrbp fn and startime fn with 1 block gap , hence no need to -4 frames
                ** othwerwise it will cancel rrbp tx
                */
                else if ( ADD_FN(rrbp_fn,diff) >=  message_ptr->dynamic_tbf.starting_time)
                {
                  bring_starttime_forward = FALSE;
                  MSG_GERAN_HIGH_3_G("Start time not moved forward %d frames, start time %d rrbp_fn %d",diff,message_ptr->dynamic_tbf.starting_time,rrbp_fn);
                }
              }

              if ( bring_starttime_forward )
              {
                message_ptr->dynamic_tbf.starting_time=SUB_FN( message_ptr->dynamic_tbf.starting_time, diff );
                MSG_GERAN_HIGH_2_G("Bring Start time forward %d frames, start time %d",diff,message_ptr->dynamic_tbf.starting_time);
              }
            }

          } else
          {
            /* Is the TBF currently suspended ? */
            if (l1_transfer_data[gas_id].disable_rx_tx)
            {
              /* Ensure we don't overwrite the gap and st var's if we are currently suspended */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap =
              l1_transfer_data[gas_id].dynamic_tbf_ptr->allow_one_rb_gap;

              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time =
              l1_transfer_data[gas_id].dynamic_tbf_ptr->starting_time;

            } else
            {
              if(!low_ts_change_uptd)
              {
                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
              }
            }

            MSG_GERAN_LOW_1_G(" FALSE nts %d",dynamic_msg_dest_ptr->nts);
          }

          if (message_ptr->dynamic_tbf.power_control_params.present)
          {
            uint8 index;
            power_control_gamma_T *pwr_cntrl_ptr;

            /*  Update Alpha value  */
            gprs_serv_cell_meas_ptr->alpha = message_ptr->dynamic_tbf.power_control_params.alpha;

            /*  Extract gamma_tn values  */
            pwr_cntrl_ptr = &(message_ptr->dynamic_tbf.power_control_params.gamma_TN0);
            index = 0;

            do
            {
              if (pwr_cntrl_ptr->present)
              {
                gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
              }


              /*  Increment pointer  */
              pwr_cntrl_ptr++;
              index++;

            }while (index < 8);

          }


          /*  Update uplink power based on the modified power control parameters. */
          gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

          /* Only create a starting time if we haven't got one */
          if (!message_ptr->dynamic_tbf.tbf_start_time_valid)
          {
            uint32 pca_fn;
            uint32 current_fn = gl1_get_FN( message_ptr->gas_id );

            /* Save the RRBP so move the suspend block to after RRBP frame */
            message_ptr->dynamic_tbf.tbf_start_time_valid = TRUE;
            message_ptr->dynamic_tbf.starting_time  = ADD_FN( current_fn, 2 );

            /* Do we have any pending PCA RRBP's to be sent */
            i = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

            if (i < SIZE_OF_RRBP_BUFF)
            {
              /* We have found a PCA */
              pca_fn = gpl1_gprs_get_rrbp_frame(i,message_ptr->gas_id);

              /* Is the Frame number of the PCA before our PTR starting time ? */
              if ( gpl1_gprs_is_frame_num_later(pca_fn,message_ptr->dynamic_tbf.starting_time))
              {
                message_ptr->dynamic_tbf.starting_time =  ((pca_fn + 8) % FRAMES_IN_HYPERFRAME);
              }

            }

            MSG_GERAN_MED_1_G("ST made up %d",message_ptr->dynamic_tbf.starting_time);
          }

          /* check to see if a start time has been specified */
          if (message_ptr->dynamic_tbf.tbf_start_time_valid)
          {
            /*  Determine if starting time present and if it needs adjusting  */
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
            /* Note this can only be a 52 style starting time */
            if (gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                            message_ptr->dynamic_tbf.starting_time,
                                            PCCCH_52, message_ptr->gas_id))
            {

              /* Starting time has expired, set as if no starting time present */
              message_ptr->dynamic_tbf.starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);
              /*  Re - calculate with new starting time */
              (void)gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                message_ptr->dynamic_tbf.starting_time,
                                                PCCCH_52, message_ptr->gas_id);
            }

            l1_transfer_data[gas_id].ul_st = dynamic_msg_dest_ptr->starting_time;

            if(  ( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
               &&( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].dl_st )
              )
            {
              /* @@Warning: This is no functional change but is used
               * to track the issue experienced in CR 62533 where PL1
               * is unable to connect assignments in particular
               * scenarios. This is because a new UL assignment must
               * wait for an old DL assignment before it can be connected
               * and vice versa.
               */
              if( l1_transfer_data[gas_id].ul_st < l1_transfer_data[gas_id].dl_st )
              {
                MSG_GERAN_ERROR_3(" PL1 unable to connect UL tbf st %u before DL tbf st %u ",
                          l1_transfer_data[gas_id].ul_st,
                          l1_transfer_data[gas_id].dl_st,
                          0 );
              }
            }
          } else
          {
            /*  If an PDA is in progress then do not clear start time variables.  */
            if (l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN)
            {
              l1_transfer_data[gas_id].starting_time_valid = FALSE; // move into transfer ASAP.
              l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
            }
          }
          MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap,dynamic_msg_dest_ptr->starting_time);
          if((FALSE != l1_transfer_data[gas_id].starting_time_valid))
          {
            if(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap)
            {
              (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,l1_transfer_data[gas_id].starting_time,2, gas_id);
              gpl1_g2x_ta_scheduler(gas_id);
            }
          }

          /* set up tbf establish type for send PH_CONNECT_IND to MAC */
          if (l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN)
          {
            l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;
          }
#ifdef FEATURE_GSM_DTM
          /* Don't set main commands in the DTM mode */
          if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
          {
            /* signal to L1 ISR that a TBF reconfig is pending */
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
          }
#else
          /* signal to L1 ISR that a TBF reconfig is pending */
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          if(l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
          { 
            gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TBF_RECONFIG);
          }
#endif

          MSG_GERAN_MED_1_G(" ul_ts_alloc  0x%X ", ul_ts_alloc);

          /* ul_ts_alloc used for debug */
          NOTUSED(ul_ts_alloc);

        }

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
        gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT,l1_transfer_data[gas_id].ul_st, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
        break;

      }
      /*******************************************************
       *                                                      *
       *                MAC_L1_FIXED_TBF_CONFIG               *
       *                                                      *
       ********************************************************/
    case MAC_L1_FIXED_TBF_CONFIG :
      {
        mac_l1_fixed_tbf_config_t  *message_ptr;
        message_ptr = ( mac_l1_fixed_tbf_config_t *) msg_header;
        gas_id = message_ptr->gas_id;
        MSG_GERAN_MED_0_G("PL1: MAC_L1_FIXED_TBF_CONFIG Rcvd");
        break;
      }
      /*******************************************************
       *                                                      *
       *                MAC_L1_DL_TBF_CONFIG                  *
       *                                                      *
       ********************************************************/
    case MAC_L1_DL_TBF_CONFIG:
      {
        /*lint -e740 ccww legacy code */
        mac_l1_dl_tbf_config_t*      message_ptr     = ( mac_l1_dl_tbf_config_t *) msg_header;
        /*lint +e740 */
        downlink_tbf_information_T *dl_msg_dest_ptr=NULL;

        uint8  l1_dl_timeslot_alloc = 0;

        uint8 bkup_sub_state = 0;
        uint8 bkup_sub_sub_state = 0;

        uint8  wait_for_isr_count = 0;

        transfer_allocation_T bkup_current_alloc_type = L1_NULL_ALLOC;
        transfer_allocation_T bkup_pending_alloc_type = L1_NULL_ALLOC;
        boolean low_ts_change_uptd = FALSE;

        volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
        gprs_pl1_serving_cell_meas_T *gprs_serv_cell_meas_ptr;
        grm_client_enum_t g_client;

        gas_id = message_ptr->gas_id;
        g_client = gl1_map_gas_id_to_client_id(gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[message_ptr->gas_id];

        MSG_GERAN_MED_3_G("PL1: MAC_L1_DL_TBF_CONFIG Rcvd [%d] ts_alloc 0x%X",
                gl1_get_FN( message_ptr->gas_id ),
                message_ptr->ts_allocation,
                0);

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);

        while( ( gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TBF_TS_CHANGE, gas_id) == ONGOING )
               && (  wait_for_isr_count <= GL1_PKT_XFER_MAX_WAIT_FRAMES )
             )
        {
          GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
          (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
          (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );

          MSG_GERAN_HIGH_1_G(" wait for isr to get FN, count %d ", wait_for_isr_count );

          wait_for_isr_count++;

          GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
        }

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
        switch( get_queued_mac_l1_edtm_cs_rel_tbf_alloc(message_ptr->gas_id ) )
        {
        case  NO_TBF_PRESENT:
        case  UL_TBF_PRESENT:
          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id);

          break;

        case  DL_TBF_PRESENT:

          MSG_GERAN_MED_0_G(" Cancelled enh alloc DL TBF ");

          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id) ;
          break;

        case  UL_DL_TBF_PRESENT:

          MSG_GERAN_MED_0_G(" Cancelled enh alloc DL TBF, only UL remain. ");

          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id);

          break;

        default:

          MSG_GERAN_ERROR_1_G( " Unknown enh alloc tbf type %u ",
                     get_queued_mac_l1_edtm_cs_rel_tbf_alloc(message_ptr->gas_id ));
          break;
        }
#endif

#ifdef FEATURE_G2X_TUNEAWAY
        gpl1_gprs_delete_rrbp_with_FAI(message_ptr->gas_id);
#endif

        /* stop T3192 as we have just received a PDA
         * GSM 04.60 sec 13.1
         */
        grlc_dl_stop_t3192(message_ptr->gas_id);
        MSG_GERAN_HIGH_0_G("PL1: stop T3192");

        if(  ( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
           &&( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].ul_st ) )
        {
          MSG_GERAN_MED_0_G("PUA IN PROGRESS");

          /*  Temporarily change some parameters, as PDA setup before PUA starting
              time has elapsed but PDA should be setup based on PUA being active. */
          /*  Parameters to be changed:- sub_state, sub_sub_state in l1_isrtsk_blk.
              current_alloc_type and pending_alloc_type in l1_transfer_data[gas_id].  */
          bkup_sub_state = l1_tskisr_blk->sub_state;
          bkup_sub_sub_state = l1_tskisr_blk->sub_sub_state;
          bkup_current_alloc_type = l1_transfer_data[gas_id].current_alloc_type;
          bkup_pending_alloc_type = l1_transfer_data[gas_id].pending_alloc_type;

          l1_tskisr_blk->sub_state = L1_DYNAMIC_2;
          l1_tskisr_blk->sub_sub_state = L1_DL_1;
          l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
          l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;

        }

        /* Based on the current sub state work out where to
         * copy new data to
         */
        switch (l1_tskisr_blk->sub_state)
        {
        case L1_DYNAMIC_1:
        case L1_DYNAMIC_2:

          if (l1_tskisr_blk->sub_sub_state == L1_DL_1)
          {
            /* Reconfigure downlink as part of concurrent tbf */
            dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

            /* Setup pending downlink structure */
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

          } else if ( (l1_tskisr_blk->sub_sub_state == L1_DL_2) ||
                      (l1_tskisr_blk->sub_sub_state == L1_NO_DL) )
          {
            /* new downlink with dynamic allocation or
             * reconfigure of downlink part or concurrent tbf */
            dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

            /* Setup pending downlink structure */
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;
          }

          /* added by ADG */
          if (l1_transfer_data[gas_id].release_pending == L1_PARTIAL_RELEASE_UL)
          {
            l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
          } else if (l1_transfer_data[gas_id].release_pending == L1_FULL_RELEASE_UL)
          {
            l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
            l1_transfer_data[gas_id].release_pending = L1_PARTIAL_RELEASE_UL;

          } else
          {
            /* set up pending allocation type */
            l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
          }

          break;

        case L1_DOWNLINK_1:

          /* Reconfigure downlink tbf, no concurrency */
          dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

          /* Setup pending downlink structure */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

          /* set up pending allocation type */
          l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;

          break;

        case L1_DOWNLINK_2:

          /* reconfigure downlink tbf, no concurrency */
          dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

          /* Setup pending downlink structure */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

          if (l1_transfer_data[gas_id].ul_st == L1_GPRS_INVALID_FN)
          {
            /* set up pending allocation type */
            l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
          }

          break;

#ifdef FEATURE_GSM_DTM
        case L1_DTM_INIT:          /*CgC during Channel modify DTM state changes for 1 frame */
        case L1_DTM_RUNNING:

          /* Sort out which of the DL data pointers to use */
          if ( l1_transfer_data[gas_id].dl_tbf_ptr == &(l1_transfer_data[gas_id].dl_tbf_1) )
          {
            /* Reconfigure downlink as part of concurrent tbf */
            dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

            /* Setup pending downlink structure */
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
          }
          else
          {
            /* Reconfigure downlink as part of concurrent tbf */
            dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

            /* Setup pending downlink structure */
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;
          }

          /***************************************************************/
          /* Check if in UL at the moment and if so whether a release is */
          /* about to take place, this will affect the setting of the    */
          /* pending allocation type.                                    */
          /***************************************************************/
          if ( (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_ALLOC) ||
               (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) )
          {
            if (l1_transfer_data[gas_id].release_pending == L1_PARTIAL_RELEASE_UL)
            {
              l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
            }
            else
            if (l1_transfer_data[gas_id].release_pending == L1_FULL_RELEASE_UL)
            {
              l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
              l1_transfer_data[gas_id].release_pending = L1_PARTIAL_RELEASE_UL;

            } else
            {
              /* set up pending allocation type */
              l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
            }
          }
          else if((l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC)&&
                  (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC))
          {
             l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
          }
          else
          {
            l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
          }

          break;
#endif

        default:
          /* something has gone wrong report error */
          MSG_GERAN_ERROR_0("PL1: Transfer Bad Sub_state");
          break;

        } /* End of switch */

        if (!dl_msg_dest_ptr)
        {
          GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
          /* something has gone wrong report error and return */
          MSG_GERAN_ERROR_0("PL1: Transfer Bad Sub_sub_state");
          return;
        }

        /* Ignore the downlink assignment mac mode (Rel 6) */
        dl_msg_dest_ptr->mac_mode = L1_MAC_MODE_INVALID;

        /* store new tn allocation in pending structure */
        dl_msg_dest_ptr->nts = gpl1_gprs_reorder_tn( &message_ptr->ts_allocation,
                                                     &dl_msg_dest_ptr->ts_allocation);

        /* Setup mode to determine whether this TBF is EGPRS or GPRS */
        //l1_transfer_data[gas_id].egprs_or_gprs_tbf = message_ptr->tbf_mode;

        /* Copy in the rlc mode */
        l1_transfer_data[gas_id].pending_dl_tbf_ptr->rlc_mode = message_ptr->rlc_mode;

        l1_transfer_data[gas_id].pending_dl_tbf_ptr->link_qual_meas_mode =
        message_ptr->link_qual_meas_mode;

        if ( FALSE != message_ptr->bep_period2.present )
        {
          l1_transfer_data[gas_id].pending_dl_tbf_ptr->bep_period2 =
          message_ptr->bep_period2.bep_period2;
        } else
        {
          l1_transfer_data[gas_id].pending_dl_tbf_ptr->bep_period2 =
          GPRS_PL1_BEP_PERIOD2_INVALID;
        }

        l1_dl_timeslot_alloc = dl_msg_dest_ptr->ts_allocation;
        /* CR2144408:Check for the case where MAC_L1_RELEASE for UL has been received but Reconfig is not done*/
        if( NULL != l1_transfer_data[gas_id].dynamic_tbf_ptr && l1_transfer_data[gas_id].current_alloc_type != L1_DOWNLINK_ALLOC)
        {
            uint32 timeslot = 0;
            uint8  *usf_ptr = &( l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[ 0 ] );

            while( timeslot < 8 )
            {
                if( GPL1_INVALID != *usf_ptr )
                {
                    l1_dl_timeslot_alloc |= (0x0001 << timeslot);
                }
                ++timeslot;
                usf_ptr++;
            }
        }

        /*if no change in lowest tn is required, check if there is already some pending tn change*/
         if(gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TBF_TS_CHANGE, gas_id))
         {
              /*check if Pending UL TBF is present*/
            if(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr != NULL)
            {
               uint8  *usf_ptr = &( l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->usf_value[0] );
               uint8  l1_pending_ul_timeslot_alloc = 0;
               uint8  timeslot = 0;

               while( timeslot < 8 )
        {
                   if( *usf_ptr != GPL1_INVALID )
          {
                       l1_pending_ul_timeslot_alloc |= (0x0001 << timeslot );
                   }
                   ++timeslot;
                   usf_ptr++;
          }
                 /*check new DL allocation + Pending uL allocation with current low TS 
                 if there is lowest TS change then add low TS change at TBF starting time
                 Because low TS change and TBf reconfig will happen at the same time*/
               if (gpl1_gprs_lowest_tn_changed((l1_pending_ul_timeslot_alloc | l1_dl_timeslot_alloc ), 
                                                      l1_get_current_timeslot(message_ptr->gas_id) ))
          {
                      l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap =TRUE;
                      low_ts_change_uptd = TRUE;
               }
               else
              {
                     /*no low TS change. We need to cancel the low TS change activity from G2X_TA table*/
                      l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
                      l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;
                      gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_TBF_TS_CHANGE,gas_id);
                      low_ts_change_uptd = TRUE;
             }
          }
            else
            {
                 /* check lowest TS change with new alloc and current low TS.
                   if there is no low TS change , we need to cancel the lowest timeslot as pending config
                    is overwritten with new UL config*/
                if(!gpl1_gprs_lowest_tn_changed( l1_dl_timeslot_alloc , l1_get_current_timeslot(gas_id) ) )
          {
                   l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;
                   gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_TBF_TS_CHANGE,gas_id);
                   low_ts_change_uptd = TRUE;
                }
          }
        }

        /* check if lowest numbered tn has changed ..need to check again as it might have changes coz of above code*/
        if ((!low_ts_change_uptd)&& gpl1_gprs_lowest_tn_changed( l1_dl_timeslot_alloc , l1_get_current_timeslot(message_ptr->gas_id) ) )

        {
          l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = TRUE;

          /* Check if there is a PCA or PDAN w FAI or PDAN w CRD RRBP */
          if (!message_ptr->tbf_start_time_valid)
          {
            message_ptr->tbf_start_time_valid = gpl1_gprs_transfer_find_rrbp_type(&message_ptr->tbf_start_time,message_ptr->gas_id);
          }
          else
          {
            /* In order to immediately start decoding the downlink PDCH for a
              given TBF start time, the start time is brought forward 4 frames.
              This allows for TBF rx/tx activity suspension when lowest
              downlink timeslot changes.NOTE the start time needs to be checked against
              the block boundary. This is currently checked later if start time is valid.
            */

            boolean bring_starttime_forward = TRUE;
            uint8   rrbp_index = SIZE_OF_RRBP_BUFF;
            uint32  rrbp_fn;
            uint8   diff;

            if (message_ptr->tbf_start_time%13 != 0)
            {
              diff  = GL1_DEFS_FRAMES_IN_BLK_PERIOD;
            }
            else
            {
              diff = GL1_DEFS_FRAMES_IN_BLK_PERIOD + 1;
            }

            rrbp_index = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

            if (rrbp_index < SIZE_OF_RRBP_BUFF)
            {
              rrbp_fn = gpl1_gprs_get_rrbp_frame(rrbp_index,message_ptr->gas_id);

              /* rrbp fn and startime clash hence move starttime by 4 frames
              */
              if ( rrbp_fn == message_ptr->tbf_start_time)
              {
                bring_starttime_forward = FALSE;
                message_ptr->tbf_start_time = ADD_FN(message_ptr->tbf_start_time,GL1_DEFS_FRAMES_IN_BLK_PERIOD);
                MSG_GERAN_HIGH_2_G("Start time delayed 4 frames, start time %d rrbp_fn %d",message_ptr->tbf_start_time,rrbp_fn);
              }
              /* rrbp fn and startime fn with 1 block gap , hence no need to -4 frames
              ** othwerwise it will cancel rrbp tx
              */
              else if ( ADD_FN(rrbp_fn,diff) >=  message_ptr->tbf_start_time)
              {
                bring_starttime_forward = FALSE;
                MSG_GERAN_HIGH_3_G("Start time not moved forward %d frames, start time %d rrbp_fn %d",diff,message_ptr->tbf_start_time,rrbp_fn);
              }
            }

            if ( bring_starttime_forward )
            {
              message_ptr->tbf_start_time=SUB_FN( message_ptr->tbf_start_time, diff );
              MSG_GERAN_HIGH_2_G("Bring Start time forward %d frames, start time %d",diff,message_ptr->tbf_start_time);
            }
          }
          MSG_GERAN_LOW_1_G("TRUE nts %d", dl_msg_dest_ptr->nts);
        } else
        {
          /* Is the TBF currently suspended ? */
          if ( (l1_transfer_data[gas_id].disable_rx_tx) && (l1_transfer_data[gas_id].dl_tbf_ptr != NULL) )
          {
            /* If we are currently suspended, don't overwrite th gap or st var's */
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap =
            l1_transfer_data[gas_id].dl_tbf_ptr->allow_one_rb_gap;

            l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time =
            l1_transfer_data[gas_id].dl_tbf_ptr->tbf_start_time;
          } 
          else
          {
            if(!low_ts_change_uptd)
          {
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;
          }
          }
          MSG_GERAN_LOW_1_G(" FALSE nts %d", dl_msg_dest_ptr->nts);
        }

        if (message_ptr->power_control.present)
        {
          uint8 index;
          power_control_gamma_T *pwr_cntrl_ptr;

          /*  Update Alpha value  */
          gprs_serv_cell_meas_ptr->alpha = message_ptr->power_control.alpha;

          /*  Extract gamma_tn values  */
          pwr_cntrl_ptr = &(message_ptr->power_control.gamma_TN0);
          index = 0;

          do
          {
            if (pwr_cntrl_ptr->present)
            {
              gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
            }

            /*  Increment pointer  */
            pwr_cntrl_ptr++;
            index++;

          }while (index < 8);

        }


        /*  Update uplink power based on the modified power control parameters. */
        gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

        /* setup downlink power control parameter's */
        if (message_ptr->downlink_pwr.params_valid)
        {
          l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
          l1_transfer_data[gas_id].dl_power_control.po = message_ptr->downlink_pwr.po;
        } else
        {
          l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
        }

        gpl1_gprs_add_to_pending_dl_ta_params (message_ptr->ta_params,message_ptr->gas_id);

        if (!message_ptr->tbf_start_time_valid)
        {

          uint32 pca_fn;
          uint16 i;

          /* Save the RRBP so move the suspend block to after RRBP frame */
          message_ptr->tbf_start_time_valid = TRUE;
          message_ptr->tbf_start_time  = ((gl1_get_FN( message_ptr->gas_id ) + 4) % FRAMES_IN_HYPERFRAME);


          /* Do we have any pending UL PCA RRBP's to be sent */
          i = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

          if (i < SIZE_OF_RRBP_BUFF)
          {
            /* We have found a PCA */
            pca_fn = gpl1_gprs_get_rrbp_frame((uint8)i,message_ptr->gas_id);

            /* Is the Frame number of the PCA before our PTR starting time ? */
            if ( gpl1_gprs_is_frame_num_later(pca_fn,message_ptr->tbf_start_time))
            {
              message_ptr->tbf_start_time =  ((pca_fn + 8) % FRAMES_IN_HYPERFRAME);
            }

          }

          MSG_GERAN_MED_1_G("ST made up %d",message_ptr->tbf_start_time);
        }

        /* check to see if a start time has been specified */
        if (message_ptr->tbf_start_time_valid)
        {
          MSG_GERAN_MED_0_G("ST present");
          /*  Determine if starting time present and if it needs adjusting  */
          /* Note this can only be a 52 style starting time */
          l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
          if (gpl1_gprs_get_starting_time(&dl_msg_dest_ptr->tbf_start_time,
                                          message_ptr->tbf_start_time,
                                          PCCCH_52, message_ptr->gas_id))
          {
            /*  Starting time has expired, set as if no starting time present */
            message_ptr->tbf_start_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);
            /*  Re - calculate with new starting time */
            (void)gpl1_gprs_get_starting_time(&dl_msg_dest_ptr->tbf_start_time,
                                              message_ptr->tbf_start_time,
                                              PCCCH_52, message_ptr->gas_id);
          }

          l1_transfer_data[gas_id].dl_st = dl_msg_dest_ptr->tbf_start_time;

          if(  ( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
             &&( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].ul_st )
            )
          {
            /* @@Warning: This is no functional change but is used
             * to track the issue experienced in CR 62533 where PL1
             * is unable to connect assignments in particular
             * scenarios. This is because a new UL assignment must
             * wait for an old DL assignment before it can be connected
             * and vice versa.
             */
            if( l1_transfer_data[gas_id].dl_st < l1_transfer_data[gas_id].ul_st )
            {
                MSG_GERAN_ERROR_2_G(" PL1 unable to connect DL tbf st %u before UL tbf st %u ",
                          l1_transfer_data[gas_id].dl_st,
                          l1_transfer_data[gas_id].ul_st);
            }
          }
        } else
        {
          /*  If an PUA is in progress then do not clear start time variables.  */
          if (l1_transfer_data[gas_id].ul_st == L1_GPRS_INVALID_FN)
          {
            l1_transfer_data[gas_id].starting_time_valid = FALSE; // move into transfer ASAP.
            l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
          }
        }
        
          MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,l1_transfer_data[gas_id].pending_dl_tbf_ptr,l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap,dl_msg_dest_ptr->tbf_start_time);
          if((FALSE != l1_transfer_data[gas_id].starting_time_valid))
          {
            if(l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap)
          {
              (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,l1_transfer_data[gas_id].starting_time,2, gas_id);
              gpl1_g2x_ta_scheduler(gas_id);
          }
        }
                

        if (message_ptr->dl_tfi_present)
        {
          dl_msg_dest_ptr->dl_tfi_present = TRUE;
          dl_msg_dest_ptr->dl_tfi = message_ptr->dl_tfi;
        } else
        {
          /* no tfi present so use current tfi
           * must be a reallocation as we need a dl tfi for
           * the dl assignment
           * Check that dl_tbf_ptr is valid just in case
           */
          if (l1_transfer_data[gas_id].dl_tbf_ptr)
          {
            dl_msg_dest_ptr->dl_tfi_present  = TRUE;
            dl_msg_dest_ptr->dl_tfi = l1_transfer_data[gas_id].dl_tbf_ptr->dl_tfi;
          } else
          {
            /* this is an assignment error!!! */
            MSG_GERAN_ERROR_0("PL1: TRANSFER No DL Tfi present");
          }
        }

        /* set up tbf establish type for send PH_CONNECT_IND to MAC */
        if (l1_transfer_data[gas_id].ul_st == L1_GPRS_INVALID_FN)
        {
          l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;
        }
        else if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
        {
          /*  Temporarily change some parameters, as PDA setup before PUA starting
              time has elapsed but PDA should be setup based on PUA being active. */
          /*  Parameters to be changed:- sub_state, sub_sub_state in l1_isrtsk_blk.
              current_alloc_type and pending_alloc_type in l1_transfer_data[gas_id].  */
          l1_tskisr_blk->sub_state = bkup_sub_state;
          l1_tskisr_blk->sub_sub_state = bkup_sub_sub_state;
          l1_transfer_data[gas_id].current_alloc_type = bkup_current_alloc_type;
          l1_transfer_data[gas_id].pending_alloc_type = bkup_pending_alloc_type;
        }

#ifdef FEATURE_GSM_DTM
        /* Don't set main commands in the DTM mode */
        if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
        {
          /* signal to L1 ISR that a TBF reconfig is pending */
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
        }
#else
        /* signal to L1 ISR that a TBF reconfig is pending */
        l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif
    #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          if(l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
          { 
            gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TBF_RECONFIG);
          }
#endif

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
       gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT, l1_transfer_data[gas_id].dl_st, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

        break;
      }


      /********************************************************
       *                                                      *
       *                MAC_L1_SINGLE BLOCK_CONFIG            *
       *                                                      *
       ********************************************************/

       /* Required for ARAC procedure as defined in GSM 04.60 7.1.3.2.1
        * currently only required for EGPRS support
        */

      case MAC_L1_SINGLE_BLOCK_CONFIG:
      {
        boolean  starting_time_elapsed;
        uint32   temp_starting_time;
        single_blk_information_T *single_blk_dest_ptr = NULL;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

        /*  Set up for single block config  */
        mac_l1_single_block_config_t *message_ptr;

        /*lint -e740 ccww legacy code */
        message_ptr = (mac_l1_single_block_config_t *) msg_header;
        /*lint +e740 */
        gas_id = message_ptr->gas_id;
        l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];

        /* We don't know if we are GPRS or EGPRS here, so setup mode to GPRS */
        l1_transfer_data[gas_id].egprs_or_gprs_tbf = TBF_MODE_GPRS;

        MSG_GERAN_MED_1_G("PL1: MAC_L1_SINGLE_BLOCK_CONFIG Rcvd ts%d",message_ptr->sb_tbf.ts_number);

        /* Work out where to copy single_block config data to */

        if(l1_transfer_data[gas_id].single_block_tbf_ptr == &l1_transfer_data[gas_id].single_block_tbf_1)
        {
          single_blk_dest_ptr = &l1_transfer_data[gas_id].single_block_tbf_2;
        }else
        {
          single_blk_dest_ptr = &l1_transfer_data[gas_id].single_block_tbf_1;
        }

        /* setup pending single block pointer */
        l1_transfer_data[gas_id].pending_single_tbf_ptr = single_blk_dest_ptr;

        /* setup number of blocks to transmit */
        single_blk_dest_ptr->num_rb_alloc = message_ptr->sb_tbf.num_rb_alloc;


        if((!single_blk_dest_ptr->num_rb_alloc) || (single_blk_dest_ptr->num_rb_alloc > 1))
        {
          /* The number of blocks to transmit should only be 1 here. Shout if not!! */
          MSG_GERAN_ERROR_1("num rb block incorrect %d",l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc);
          return;
        }


        l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;

        /*  Reset ul_desc pointer so any previous READS discarded. */
        memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
        l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  = &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];

        /* ensure the Downlink TBF pointer is null */
        l1_transfer_data[gas_id].dl_tbf_ptr=NULL;

        /*  Extract the information from the message  */
        /*  Access type, single phase or two phase. This determines which state
            PL1 returns to after transferring the single block, access state for
            two phase access, idle state for everything else. */
        single_blk_dest_ptr->two_phase_access = message_ptr->sb_tbf.two_phase_access;

        /* We should only be doing 2 phase at his point i.e. no measurement reports etc */
        if(single_blk_dest_ptr->two_phase_access != TRUE)
        {
          MSG_GERAN_ERROR_0_G("PL1: 2 phase access not set in single blk reconfig");
          return;
        }

        /* setup single block type to uplink as this is a single ul block */
        single_blk_dest_ptr->sb_ul_dl_type = SINGLE_UL_BLOCK;


        /*  timeslot number */
        single_blk_dest_ptr->ts_number = message_ptr->sb_tbf.ts_number;

        /*  If present, alpha value */
        single_blk_dest_ptr->alpha_present = message_ptr->sb_tbf.alpha_present;

        if (single_blk_dest_ptr->alpha_present)
        {
          single_blk_dest_ptr->alpha = message_ptr->sb_tbf.alpha;
        }

        /*  Gamma_tn  */
        single_blk_dest_ptr->gamma_tn = message_ptr->sb_tbf.gamma_tn;

        /*  Downlink power struct */
        single_blk_dest_ptr->downlink_pwr = message_ptr->sb_tbf.downlink_pwr;

        /* setup downlink power control parameter's */
        l1_transfer_data[gas_id].dl_power_control = message_ptr->sb_tbf.downlink_pwr;

        /* Copy the bep period 2, required by the egprs bep quality
         * measurements module.
         */
        if ( FALSE != message_ptr->sb_tbf.bep_period2.present )
        {
          single_blk_dest_ptr->bep_period2 =
              message_ptr->sb_tbf.bep_period2.bep_period2;
        } else
        {
          single_blk_dest_ptr->bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
        }

        /*  Starting time, also perform some checks */

        /*  Now check if the starting time is the first frame of a block, if
            not move to next block boundary. Function will return modified
            starting time, If starting time has elapsed function will return
            TRUE. */
        starting_time_elapsed = gpl1_gprs_get_starting_time(&temp_starting_time,
                                                            message_ptr->sb_tbf.starting_time,
                                                            PCCCH_52, gas_id);

        if (!starting_time_elapsed)
        {
          if (gpl1_gprs_adjust_mod_num(gl1_get_FN( message_ptr->gas_id ), 3, FRAMES_IN_HYPERFRAME) > (uint32)temp_starting_time)
          {
            starting_time_elapsed = TRUE;

          }
        }

        /* Set up Timing advance for transmission */
        single_blk_dest_ptr->timing_advance_params = message_ptr->sb_tbf.timing_advance_params;

        /* clear NC drx related params as we don't expect to TX measurement reports from here */
        single_blk_dest_ptr->nc_non_drx_params.nc_non_drx_valid  = FALSE;
        single_blk_dest_ptr->nc_non_drx_params.meas_report       = FALSE;

        /*  Check if starting time has elapsed, if so, inform MAC, stop process
            and return to IDLE  */
        if (starting_time_elapsed)
        {
          /*  Inform MAC  */
          l1_mac_sig_t            message_buffer,*message;

          MSG_GERAN_MED_0_G("PL1: SNGLE BLCK ST elapsed ");



          /* clear out the PACCH FIFO as we have noy sent it */
          gpl1_gprs_reset_pacch_fifo(gas_id);

          /* Align the pointers */
          message = &message_buffer;

          /* Set up the message header */
          message->task_id = MS_MAC_L1;
          message->sig_id = L1_MAC_51_SB_FAILURE;
          message->msg.sb_51_failure.gas_id = gas_id;

          /* Send the message, NULL used for MAC_PH_DATA_IND */
          L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);


          /* Clear bitmap and wait for new assignment */
          RESET_BITMAP(gas_id);


          MSG_GERAN_HIGH_1_G("Resetting Transfer mode [%d]",gl1_get_FN( gas_id ));
          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
          l1_transfer_data[gas_id].starting_time_valid = FALSE;
        }

        /*  If single block config has been previously received but not sent,
            (waiting for starting time) and another single block config is
            received, then clear Freq config and uplink data valid flags
            as new ones will be sent */
        if (FREQ_CONFIG_RECV(gas_id) && SINGLE_BLOCK_RECV(gas_id) && UPLINK_DATA_VALID(gas_id))
        {
          /*  Clear freq config and uplink data flags  */
          CLEAR_FREQ_CONFIG_RECV(gas_id);
          CLEAR_UPLINK_DATA_VALID(gas_id);

        }

        /* Mark mac_l1_single_block_config received */
        SET_SINGLE_BLOCK_RECV(gas_id);

        if (SINGLE_BLOCK_ALLOC_COMP(gas_id))
        {

          /* setup tbf establish type  */
          l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;

          l1_transfer_data[gas_id].pending_alloc_type = L1_SINGLE_BLOCK_ALLOC;
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
          RESET_BITMAP(gas_id);

          l1_tskisr_blk->command = L1_NULL_COMMAND;
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
          gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, temp_starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

        }

        break;
      }

      /*******************************************************
      *                                                      *
      *                MAC_L1_TS_RECONFIG                    *
      *                                                      *
      ********************************************************/

    case MAC_L1_TS_RECONFIG:
      {
        mac_l1_ts_reconfig_t  *message_ptr;
        downlink_tbf_information_T *dl_msg_dest_ptr=NULL;
        dynamic_tbf_information_T *dynamic_msg_dest_ptr=NULL;
        uint8 i;
        boolean connect_dl_only = FALSE;

        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
        grm_client_enum_t g_client;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_ts_reconfig_t *) msg_header;
        /*lint +e740 */
        gas_id = message_ptr->gas_id;
        l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[message_ptr->gas_id];
        g_client = gl1_map_gas_id_to_client_id(gas_id);
        MSG_GERAN_MED_3_G( "PL1: MAC_L1_TS_RECONFIG Rcvd [%d] dl present %u ",
                 gl1_get_FN( message_ptr->gas_id ),
                 message_ptr->dl_tfi_present,
                 0 );

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
        switch( get_queued_mac_l1_edtm_cs_rel_tbf_alloc(message_ptr->gas_id ) )
        {
        case  NO_TBF_PRESENT:
          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id);
          break;

        case  DL_TBF_PRESENT:

          MSG_GERAN_MED_0_G(" Cancelled enh alloc DL TBF ");

          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id) ;
          break;

        case  UL_TBF_PRESENT:

          MSG_GERAN_MED_0_G(" Cancelled enh alloc UL TBF ");

          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id);
          break;

        case  UL_DL_TBF_PRESENT:

          MSG_GERAN_MED_0_G(" Cancelled enh alloc UL & DL TBF ");

          set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,message_ptr->gas_id);
          break;

        default:

          MSG_GERAN_ERROR_1_G( " Unknown enh alloc tbf type %u ",
                     get_queued_mac_l1_edtm_cs_rel_tbf_alloc(message_ptr->gas_id));
          break;
        }
#endif
#ifdef FEATURE_G2X_TUNEAWAY
        gpl1_gprs_delete_rrbp_with_FAI(message_ptr->gas_id);
#endif
        /* stop T3192 as we have just received a PTR
         * GSM 04.60 sec 13.1
         */
        grlc_dl_stop_t3192(message_ptr->gas_id);

        MSG_GERAN_HIGH_1_G( "PL1: stop T3192 dl alloc 0x%X ",
                  message_ptr->dl_ts_allocation);

        /****************************************************************
         ***** store downlink allocation parameters in pending structure
         ****************************************************************/

#ifdef FEATURE_GSM_DTM
        if ( (l1_tskisr_blk->l1_state == L1_DTM_MODE) &&
             (l1_transfer_data[gas_id].dl_tbf_ptr == &(l1_transfer_data[gas_id].dl_tbf_1)) )
        {
          /* Reconfigure downlink as part of concurrent tbf */
          dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

          /* Setup pending downlink structure */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
        }
        else
        if ( (l1_tskisr_blk->l1_state == L1_DTM_MODE) &&
             (l1_transfer_data[gas_id].dl_tbf_ptr != &(l1_transfer_data[gas_id].dl_tbf_1)) )
        {
          /* new downlink with dynamic allocation or
           * reconfigure of downlink part or concurrent tbf */
          dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

          /* Setup pending downlink structure */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;
        }
        else
#endif /*FEATURE_GSM_DTM*/
        if ((l1_tskisr_blk->sub_sub_state == L1_DL_1) ||
            (l1_tskisr_blk->sub_state == L1_DOWNLINK_1))
        {
          /* Reconfigure downlink as part of concurrent tbf */
          dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

          /* Setup pending downlink structure */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

          /* double check we are not writing into our active dl structure */
          if (l1_transfer_data[gas_id].dl_tbf_ptr)
          {
            if (l1_transfer_data[gas_id].dl_tbf_ptr == &l1_transfer_data[gas_id].dl_tbf_2)
            {
              MSG_GERAN_ERROR_0("pending_dl_tbf_ptr setup incorrectly");
              /* Modify pointer's to ensure we don't overwrite our active data */

              /* Reconfigure downlink as part of concurrent tbf */
              dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

              /* Setup pending downlink structure */
              l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;
            }
          }

        } else if ( (l1_tskisr_blk->sub_sub_state == L1_DL_2) ||
                    (l1_tskisr_blk->sub_sub_state == L1_NO_DL)||
                    (l1_tskisr_blk->sub_state == L1_DOWNLINK_2))
        {
          /* new downlink with dynamic allocation or
           * reconfigure of downlink part or concurrent tbf */
          dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

          /* Setup pending downlink structure */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

          /* double check we are not writing into our active dl structure */
          if (l1_transfer_data[gas_id].dl_tbf_ptr)
          {
            if (l1_transfer_data[gas_id].dl_tbf_ptr == &l1_transfer_data[gas_id].dl_tbf_1)
            {
              MSG_GERAN_ERROR_0("pending_dl_tbf_ptr setup incorrectly");
              /* Modify pointer's to ensure we don't overwrite our active data */

              /* Reconfigure downlink as part of concurrent tbf */
              dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

              /* Setup pending downlink structure */
              l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
            }
          }

        }

        if (!dl_msg_dest_ptr)
        {
          GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
          /* Something has gone wrongm report error and return */
          MSG_GERAN_ERROR_0_G("PL1: TRANSFER Bad sub_sub_state");
          return;
        }

        /* set up pending allocation type */
        l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;

        /* store new tn allocation in pending structure */
        dl_msg_dest_ptr->nts = gpl1_gprs_reorder_tn( &message_ptr->dl_ts_allocation,
                                                     &dl_msg_dest_ptr->ts_allocation);

        /* setup downlink power control parameter's */
        if (message_ptr->dl_ctrl_param.params_valid)
        {
          l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
          l1_transfer_data[gas_id].dl_power_control.po = message_ptr->dl_ctrl_param.po;
        } else
        {
          l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
        }

        if (message_ptr->dl_tfi_present)
        {
          dl_msg_dest_ptr->dl_tfi_present = TRUE;
          dl_msg_dest_ptr->dl_tfi = message_ptr->dl_tfi;
        } else
        {
          /* no tfi present so use current tfi
           * must be a reallocation as we need a dl tfi for
           * the dl assignment
           * Check that dl_tbf_ptr is valid just in case
           */
          if (l1_transfer_data[gas_id].dl_tbf_ptr)
          {
            dl_msg_dest_ptr->dl_tfi_present  = TRUE;
            dl_msg_dest_ptr->dl_tfi = l1_transfer_data[gas_id].dl_tbf_ptr->dl_tfi;
          } else
          {
            /* this is an assignment error!!! */
            MSG_GERAN_ERROR_0_G("PL1: TRANSFER No DL Tfi present");
          }
        }

        /* Copy mac mode into the pending downlinktbf structure */
        dl_msg_dest_ptr->mac_mode = message_ptr->uplink_tbf_type;

        if(  ( MAC_L1_MAC_MODE_DYNAMIC == message_ptr->uplink_tbf_type )
           ||( MAC_L1_MAC_MODE_EXTENDED_DYNAMIC == message_ptr->uplink_tbf_type )
          )
        {
          uint8  ul_ts_alloc = 0;
          uint8  l1_ul_ts_alloc = 0;

          /****************************************************************
           ***** store dynamic allocation parameters in pending structure
           ****************************************************************/
#ifdef FEATURE_GSM_DTM
          if( (l1_tskisr_blk->l1_state == L1_DTM_MODE) &&
              (l1_transfer_data[gas_id].dynamic_tbf_ptr == &(l1_transfer_data[gas_id].dynamic_tbf_1)) )
          {
            /* Reconfigure dynamic tbf or add to existing DL tbf */
            dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

            /* Setup pending dynamic structure */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;
          }
          else
          if( (l1_tskisr_blk->l1_state == L1_DTM_MODE) &&
              (l1_transfer_data[gas_id].dynamic_tbf_ptr == &(l1_transfer_data[gas_id].dynamic_tbf_2)) )
          {
            /* reconfigure dynamic tbf */
            dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

            /* Setup pending dynamic structure */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

          }
          else
#endif /*FEATURE_GSM_DTM*/

          if ((l1_tskisr_blk->sub_state == L1_DYNAMIC_1)||
              (l1_tskisr_blk->sub_state == L1_DOWNLINK_1) ||
              (l1_tskisr_blk->sub_state == L1_DOWNLINK_2) )
          {
            /* Reconfigure dynamic tbf or add to existing DL tbf */
            dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

            /* Setup pending dynamic structure */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

          } else if ( l1_tskisr_blk->sub_state == L1_DYNAMIC_2)
          {
            /* reconfigure dynamic tbf */
            dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

            /* Setup pending dynamic structure */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
          }


          if (!dynamic_msg_dest_ptr)
          {
            GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
            /* Something has gone wrong report error and return */
            MSG_GERAN_ERROR_0_G("PL1: Transfer Bad sub_state");
            return;
          }

          /* Copy in the mac mode */
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode = message_ptr->uplink_tbf_type;

          dynamic_msg_dest_ptr->nts = 0;

          for (i=0;i< MAX_TN;i++)
          {
            if (message_ptr->usf[i].present)
            {
              ul_ts_alloc |= (0x80 >> i);
              l1_ul_ts_alloc |= ( 0x0001 << i );

              dynamic_msg_dest_ptr->usf_value[i] = message_ptr->usf[i].usf_value;
              dynamic_msg_dest_ptr->nts++;
            } else
              /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
              dynamic_msg_dest_ptr->usf_value[i] = GPL1_INVALID;

          }

          /* ul_ts_alloc used for debug only */
          NOTUSED(ul_ts_alloc);

          MSG_GERAN_MED_1_G("ts reconf ul ts alloc 0x%X",ul_ts_alloc);

          /* Setup new usf granularity FALSE = 1 block tx , TRUE = 4 block tx */
          dynamic_msg_dest_ptr->usf_granularity = message_ptr->usf_granularity;

          l1_transfer_data[gas_id].pending_dl_tbf_ptr->link_qual_meas_mode =
          message_ptr->link_qual_meas_mode;

          l1_transfer_data[gas_id].pending_dl_tbf_ptr->bep_period2 =
          GPRS_PL1_BEP_PERIOD2_INVALID;

          /* Copy in rlc mode */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr->rlc_mode      = message_ptr->rlc_mode;

          /* Set test mode for EGPRS as invalid since the test mode
           * variable is not part of the timeslot reconfigure message.
           */
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->test_mode = TEST_MODE_INVALID;

          /* Update the test mode params to get reflected in dynamic config */
          if( l1_transfer_data[gas_id].l1_test_mode == L1_TEST_MODE_B)
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->test_mode = TEST_MODE_B;
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->test_mode_tn = l1_transfer_data[gas_id].l1_test_mode_tn;
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->nts = dynamic_msg_dest_ptr->nts;
          }

          if( ( NULL != l1_transfer_data[gas_id].dynamic_tbf_ptr )
            &&( 0 == l1_ul_ts_alloc ) )
          {
              /* If the TS reconfig msg does not contain any ul timeslots
               * and an uplink tbf is in progress then make sure
               * the uplink tbfs timeslots are included when deciding
               * on the 1 rb gap.
               */
              uint32 timeslot = 0;
              uint8  *usf_ptr = &( l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[ 0 ] );

              while( timeslot < 8 )
              {
                  if( GPL1_INVALID != *usf_ptr )
                  {
                      l1_ul_ts_alloc |= (0x0001 << timeslot);
                  }
                  ++timeslot;
                  usf_ptr++;
              }
          }

          if( ( NULL != l1_transfer_data[gas_id].dl_tbf_ptr )
            &&( 0 == dl_msg_dest_ptr->ts_allocation ) )
          {
              /* If the TS reconfig msg does not contain any dl timeslots
               * and a downlink tbf is in progress then make sure
               * the downlink tbfs timeslots are included when deciding
               * on the 1 rb gap.
               */
              l1_ul_ts_alloc |= l1_transfer_data[gas_id].dl_tbf_ptr->ts_allocation;
          }
          /* check if lowest numbered tn has changed or */
          if (gpl1_gprs_lowest_tn_changed( ( dl_msg_dest_ptr->ts_allocation | l1_ul_ts_alloc ), l1_get_current_timeslot(message_ptr->gas_id) ) )

          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = TRUE;

            /* Check if there is a PCA or PDAN w FAI or PDAN w CRD RRBP */
            if (!message_ptr->starting_time_present)
            {
              message_ptr->starting_time_present = gpl1_gprs_transfer_find_rrbp_type(&message_ptr->starting_time,message_ptr->gas_id);
            }
            else
            {
              /* In order to immediately start decoding the downlink PDCH for a
                given TBF start time, the start time is brought forward 4 frames.
                This allows for TBF rx/tx activity suspension when lowest
                downlink timeslot changes.NOTE the start time needs to be checked against
                the block boundary. This is currently checked later if start time is valid.
              */

              boolean bring_starttime_forward = TRUE;
              uint8   rrbp_index = SIZE_OF_RRBP_BUFF;
              uint32  rrbp_fn;
              uint8   diff;

              if (message_ptr->starting_time%13 != 0)
              {
                diff  = GL1_DEFS_FRAMES_IN_BLK_PERIOD;
              }
              else
              {
                diff = GL1_DEFS_FRAMES_IN_BLK_PERIOD + 1;
              }

              rrbp_index = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

              if (rrbp_index < SIZE_OF_RRBP_BUFF)
              {
                rrbp_fn = gpl1_gprs_get_rrbp_frame(rrbp_index,message_ptr->gas_id);

                /* rrbp fn and startime clash hence move starttime by 4 frames
                */
                if ( rrbp_fn == message_ptr->starting_time)
                {
                  bring_starttime_forward = FALSE;
                  message_ptr->starting_time = ADD_FN(message_ptr->starting_time,GL1_DEFS_FRAMES_IN_BLK_PERIOD);
                  MSG_GERAN_HIGH_2_G("Start time delayed 4 frames, start time %d rrbp_fn %d",message_ptr->starting_time,rrbp_fn);
                }
                /* rrbp fn and startime fn with 1 block gap , hence no need to -4 frames
                ** othwerwise it will cancel rrbp tx
                */
                else if ( ADD_FN(rrbp_fn,diff) >=  message_ptr->starting_time)
                {
                  bring_starttime_forward = FALSE;
                  MSG_GERAN_HIGH_3_G("Start time not moved forward %d frames, start time %d rrbp_fn %d",diff,message_ptr->starting_time,rrbp_fn);
                }
              }

              if ( bring_starttime_forward )
              {
                message_ptr->starting_time=SUB_FN( message_ptr->starting_time, diff );
                MSG_GERAN_HIGH_2_G("Bring Start time forward %d frames, start time %d",diff,message_ptr->starting_time);
              }
            }
            MSG_GERAN_LOW_0_G(" ts gap = TRUE");
          } else
          {
            if (l1_transfer_data[gas_id].disable_rx_tx)
            {
              /* Make sure the dl_tbf_ptr is set before using it */
              if( NULL != l1_transfer_data[gas_id].dl_tbf_ptr )
              {
                l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap =
                l1_transfer_data[gas_id].dl_tbf_ptr->allow_one_rb_gap;

                l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time =
                l1_transfer_data[gas_id].dl_tbf_ptr->tbf_start_time;
              }
              else
              {
                l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;

                MSG_GERAN_MED_0_G(" NULL dl_tbf_ptr ");
              }

              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap =
              l1_transfer_data[gas_id].dynamic_tbf_ptr->allow_one_rb_gap;

              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time =
              l1_transfer_data[gas_id].dynamic_tbf_ptr->starting_time;
            } else
            {
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
              l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;
            }
            MSG_GERAN_LOW_0_G(" ts gap = FALSE");
          }


        } else
        {
          GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
          /*** add fixed allocation bits here if we ever need it */
          MSG_GERAN_ERROR_0_G("PL1: Transfer Fixed alloc not supp.");
          return;
        }
        /****************************************************************
         ***** Setup power control and TA parameters
         ****************************************************************/
        if (message_ptr->power_control_params.present)
        {
          uint8 index;
          power_control_gamma_T *pwr_cntrl_ptr;

          /*  Update Alpha value  */
          gprs_serv_cell_meas_ptr->alpha = message_ptr->power_control_params.alpha;

          /*  Extract gamma_tn values  */
          pwr_cntrl_ptr = &(message_ptr->power_control_params.gamma_TN0);
          index = 0;

          do
          {
            if (pwr_cntrl_ptr->present)
              gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;

            /*  Increment pointer  */
            pwr_cntrl_ptr++;
            index++;

          }while (index < 8);

          /*  Update uplink power based on the modified power control parameters. */
          gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

        }

        gpl1_gprs_add_global_ta_param(message_ptr->global_packet_ta,message_ptr->gas_id);

        if (!message_ptr->starting_time_present)
        {
          uint32 pca_fn;
          uint32 current_fn = gl1_get_FN( message_ptr->gas_id );

          /* Save the RRBP so move the suspend block to after RRBP frame */
          message_ptr->starting_time_present = TRUE;

          /* Improve the reaction time of L1
           * Only account for the task to ISR context switch and hence add 1 tdma frame.
           */
          message_ptr->starting_time  = ADD_FN( current_fn, 2 );

          /* Do we have any pending PCA RRBP's to be sent */
          i = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

          if (i < SIZE_OF_RRBP_BUFF)
          {
            /* We have found a PCA */
            pca_fn = gpl1_gprs_get_rrbp_frame(i,message_ptr->gas_id);

            /* Is the Frame number of the PCA before our PTR starting time ? */
            if ( gpl1_gprs_is_frame_num_later(pca_fn,message_ptr->starting_time))
            {
              message_ptr->starting_time =  ((pca_fn + 8) % FRAMES_IN_HYPERFRAME);
            }

          }

         MSG_GERAN_MED_1_G("ST made up %d",message_ptr->starting_time);

        }

        /* check to see if a start time has been specified */
        if (message_ptr->starting_time_present)
        {
          MSG_GERAN_MED_0_G("ST present");

          /*  Determine if starting time present and if it needs adjusting  */
          /* Note this can only be a 52 style starting time */
          if ( (l1_transfer_data[gas_id].ul_st == l1_transfer_data[gas_id].dl_st) &&
               (l1_transfer_data[gas_id].ul_st != L1_GPRS_INVALID_FN))
          {
            /* implies we have already configured once */
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
            l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

            if (gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                            message_ptr->starting_time,
                                            PCCCH_52, message_ptr->gas_id))
            {

              /*  Starting time has expired, set as if no starting time present */
              message_ptr->starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

              /*  Re - calculate with new starting time */
              (void)gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                message_ptr->starting_time,
                                                PCCCH_52, message_ptr->gas_id);
            }
            l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].starting_time;
            l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].starting_time;
            dl_msg_dest_ptr->tbf_start_time = l1_transfer_data[gas_id].dl_st;
          }
          else if (l1_transfer_data[gas_id].ul_st != L1_GPRS_INVALID_FN)
          {
            /* Waiting for start time associated with uplink config */
            /*  Update with the re-config starting time */
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

            if (gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                            message_ptr->starting_time,
                                            PCCCH_52, message_ptr->gas_id))
            {

              /*  Starting time has expired, set as if no starting time present */
              message_ptr->starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

              /*  Re - calculate with new starting time */
              (void)gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                message_ptr->starting_time,
                                                PCCCH_52, message_ptr->gas_id);
            }

            l1_transfer_data[gas_id].ul_st = dynamic_msg_dest_ptr->starting_time;

          } else if (l1_transfer_data[gas_id].dl_st != L1_GPRS_INVALID_FN)
          {
            /* Waiting for start time associated with dwnlink config */
            /*  Update with the re-config starting time */
            l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

            if (gpl1_gprs_get_starting_time(&dl_msg_dest_ptr->tbf_start_time,
                                            message_ptr->starting_time,
                                            PCCCH_52, message_ptr->gas_id))
            {
              /*  Starting time has expired, set as if no starting time present */
              message_ptr->starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

              /*  Re - calculate with new starting time */
              (void)gpl1_gprs_get_starting_time(&dl_msg_dest_ptr->tbf_start_time,
                                                message_ptr->starting_time,
                                                PCCCH_52, message_ptr->gas_id);

            }
            l1_transfer_data[gas_id].dl_st = dl_msg_dest_ptr->tbf_start_time;

          } else
          {
            /* no ul or dl starting currently active so set up normal case */

            if (gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                            message_ptr->starting_time,
                                            PCCCH_52, message_ptr->gas_id))
            {
              /*  Starting time has expired, set as if no starting time present */
              message_ptr->starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

              /*  Re - calculate with new starting time */
              (void)gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                message_ptr->starting_time,
                                                PCCCH_52, message_ptr->gas_id);
            }

          }

        } else
        {
          l1_transfer_data[gas_id].starting_time_valid = FALSE; // move into transfer ASAP.
          l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
        }
        
          MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap,dynamic_msg_dest_ptr->starting_time);
          if((FALSE != l1_transfer_data[gas_id].starting_time_valid))
          {
            if(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap)
            {
              (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,dynamic_msg_dest_ptr->starting_time,2, gas_id);
              gpl1_g2x_ta_scheduler(gas_id);
            }
        }

        /***
         *** Work out if an exiting uplink TBF is to be released before
         *** PTR is to be connected
         ***/
        if ((l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) ||
            (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_ALLOC))
        {
          /* We are either in concurency or Ul only */
          if ((l1_transfer_data[gas_id].release_pending == L1_PARTIAL_RELEASE_UL) ||
              (l1_transfer_data[gas_id].release_pending == L1_FULL_RELEASE_UL))
          {
            /* Search for theFN of the FAI UL PCA to determine when UL will be released */
            connect_dl_only = FALSE;

            /* do we have to wait to transmit pca before going to dl only */
            if (l1_transfer_data[gas_id].wait_for_pca_valid == FALSE)
            {
              i = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

              if (i < SIZE_OF_RRBP_BUFF)
              {
                /* Is the Frame number of the PCA before our PTR starting time ? */
                if ( l1_transfer_data[gas_id].starting_time > gpl1_gprs_get_rrbp_frame(i,message_ptr->gas_id) )
                {
                  connect_dl_only = TRUE;
                }
              }
            } else
            {
              /* has the pca gone out before starting time if so go to dl only */
              /* One has to use the actual FN when PCA was sent rather that wait for PCA FN
                 which is adjusted by +9 to send REL_CNF as we already have DL ,
                 and not going to Idle , doing a REL_CNF on PCA TX boundary should be fine
              */
              if (SUB_FN(l1_transfer_data[gas_id].wait_for_pca_fn,GPL1_WAITING_PERIOD_FOR_UL_REL_CNF) <
                  l1_transfer_data[gas_id].starting_time)
              {
                /* As PCA is sent  make it false as we will be sending REL_CNF for UL below */
                l1_transfer_data[gas_id].pca_sent = FALSE;

                MSG_GERAN_MED_2_G(" pca_fn [%d] st %d pca tx , send ul rel cnf as pca sent",
                         l1_transfer_data[gas_id].wait_for_pca_fn,l1_transfer_data[gas_id].starting_time);

                l1_transfer_data[gas_id].wait_for_pca_fn    = L1_GPRS_INVALID_FN;
                l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;

                connect_dl_only = TRUE;
              } else
              {
                connect_dl_only = FALSE;
              }
            }

          }
        }

        if (connect_dl_only)
        {
          /* reset the MAC mode associated with the released
           * uplink tbf.
           */
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode =
              L1_MAC_MODE_INVALID;

          MSG_GERAN_HIGH_0_G(" UL released connect DL only");

          l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;

          /* set up tbf establish type for send PH_CONNECT_IND to MAC */
          l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

          /* Now tell MAC that we have cancelled the pending uplink */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_UL, message_ptr->gas_id);

          /* reset relelase type and rel confirm status*/
          l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].starting_time;
          dl_msg_dest_ptr->tbf_start_time = l1_transfer_data[gas_id].dl_st;
          l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
          l1_transfer_data[gas_id].confirm_pending = SENT;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
          gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT, l1_transfer_data[gas_id].dl_st, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

        } else
        {
          /* set up tbf establish type for send PH_CONNECT_IND to MAC */
          l1_transfer_data[gas_id].tbf_est_type = UPLINK_DOWNLINK_TBF_ESTABLISHED;

          /* Set the ul and dl st so that the release handlers are
           * aware of the packet timeslot reconfigure.
           */

          dynamic_msg_dest_ptr->starting_time = l1_transfer_data[gas_id].ul_st;
          dl_msg_dest_ptr->tbf_start_time = l1_transfer_data[gas_id].dl_st;
          l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].starting_time;
          l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].starting_time;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
          gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, l1_transfer_data[gas_id].ul_st, gas_id);
          gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT, l1_transfer_data[gas_id].dl_st, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

        }

#ifdef FEATURE_GSM_DTM
        /* Don't set main commands in the DTM mode */
        if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
        {
          /* reset transfer mode to start with ul tbf only */
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
        }
        else
        {

#ifdef  FEATURE_GSM_EDTM
          if( L1_DTM_ENHANCED_RELEASE != l1_get_dtm_command_event(message_ptr->gas_id) )
          {
#endif

            l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG,message_ptr->gas_id);

#ifdef  FEATURE_GSM_EDTM

          }
#endif


        }
#else
        /* signal to L1 ISR that a TBF reconfig is pending */
        l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TBF_RECONFIG);
#endif

        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
        break;
      }


#ifdef FEATURE_GPRS_PS_HANDOVER

      /*******************************************************
      *                                                      *
      *                MAC_L1_PSHO_CANCEL_PENDING_CONFIG     *
      *                                                      *
      ********************************************************/
      case  MAC_L1_PSHO_CANCEL_PENDING_CONFIG:
      {
        mac_l1_psho_cancel_pending_config_t* message_ptr = ( mac_l1_psho_cancel_pending_config_t* )msg_header;
        volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        uint32 current_frame;

        gas_id = message_ptr->gas_id;

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
        current_frame = GSTMR_GET_FN_GERAN( message_ptr->gas_id );

        /* Only cancel the pending TBF if we have not started to
         * reconfigure in expectation of its start. Else, the TBF will
         * be started.
         */

        if( (l1_transfer_data[gas_id].starting_time != L1_GPRS_INVALID_FN)
         && (current_frame < SUB_FN(l1_transfer_data[gas_id].starting_time,1)))
        {
          MSG_GERAN_MED_0_G( "PL1: MAC_L1_PSHO_CANCEL_PENDING_CONFIG ");

          l1_tskisr_blk->main_command = L1_NULL_COMMAND;

          /* Reset the start time related variables.
           * This ensures a connect indication is never sent.
           */

          l1_transfer_data[gas_id].ul_st                 = L1_GPRS_INVALID_FN;
          l1_transfer_data[gas_id].dl_st                 = L1_GPRS_INVALID_FN;
          l1_transfer_data[gas_id].starting_time         = L1_GPRS_INVALID_FN;
          l1_transfer_data[gas_id].starting_time_valid   = FALSE;
          l1_transfer_data[gas_id].starting_time_elapsed = TRUE;

          /* Set pending alloc type to current. If pending is set to NULL_ALLOC
           * then an error occurs for the following scenario:
           * Current allocation is DYNAMIC_DOWN_ALLOC, then MAC_L1_PSHO_CANCEL_PENDING_CONFIG
           * occurs which sets pending alloc to NULL. MAC_L1_RELEASE occurs,resulting in an error,
           * where the validity of releasing DL where pending alloc is NULL.
           * (see gpl1_assess_dl_tbf_release).
           */

          l1_transfer_data[gas_id].pending_alloc_type    = l1_transfer_data[gas_id].current_alloc_type;
        }
        else
        {
          MSG_GERAN_HIGH_0_G( "PL1: TOO LATE TO CANCEL PSHO PENDING CONFIG ");
        }

        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
        break;
      }

      /*******************************************************
      *                                                      *
      *                MAC_L1_PSHO_REQ                       *
      *                                                      *
      ********************************************************/

    case MAC_L1_PSHO_REQ:
      {
        /* Use a local pointer to read the message contents */
        mac_l1_psho_req_t* const  message_ptr = ( mac_l1_psho_req_t *) msg_header;
        idle_data_T               *l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];
        volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[message_ptr->gas_id];
        grm_client_enum_t g_client;
        l1_sc_bs_T*  target_cell_details_ptr = NULL;

        /* Get a copy of the current frame number */
        uint32  current_fn;

        gas_id = message_ptr->gas_id;
        MSG_GERAN_MED_3_G( "PL1: MAC_L1_PSHO_REQ Rcvd to cell %d bsic %d type %d",
                 message_ptr->psho_arfcn.num,
                 message_ptr->psho_bsic,
                 message_ptr->psho_type );
        g_client = gl1_map_gas_id_to_client_id(gas_id);
        /* Quick check on layer 1 state, ensure we are not in DTM mode
           or start time specified */

        if(
           ( ( message_ptr->ul_tbf.tbf_start_time_valid )
              &&( PSHO_UL_TBF == message_ptr->psho_tbf_type )
           )
#ifdef FEATURE_GSM_DTM
           ||
           ( l1_tskisr_blk->l1_state == L1_DTM_MODE )
#endif /* FEATURE_GSM_DTM */
          )
        {
           MSG_GERAN_ERROR_0_G( "PSHO handover received in DTM state or Start time valid");

           gpl1_gprs_send_l1_mac_psho_cnf(PSHO_FAILURE, message_ptr->gas_id);

           return;
        }

        if(  ( PSHO_GTOG == message_ptr->psho_type )
           &&( PSHO_NON_SYNC == message_ptr->psho_sync_type )
           &&( FALSE == message_ptr->handover_reference_present )
          )
        {
           MSG_GERAN_ERROR_0_G(" Hover ref not present for GTOG NON_SYNC case ");

           return;
        }

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);

        current_fn = gl1_get_FN( message_ptr->gas_id );

        target_cell_details_ptr = l1_sc_balist_get_cell_data( message_ptr->psho_arfcn,message_ptr->gas_id );

        /* Quick check to ensure we are synchronised to the target cell */

        if ( NULL != target_cell_details_ptr )
        {
            MSG_GERAN_MED_3_G( "Target cell qbit lag=%d fn_lag=%d bsic_known %d",
                     target_cell_details_ptr->qbit_lag,
                     target_cell_details_ptr->fn_lag,
                     target_cell_details_ptr->bsic_known );

            MSG_GERAN_MED_3_G( "Target cell bsic %d ts %d Curr_ts %d",
                     target_cell_details_ptr->bsic,
                     target_cell_details_ptr->TN,
                     l1_get_current_timeslot(message_ptr->gas_id) );
        }
        else
        {
           MSG_GERAN_ERROR_0_G( "PSHO handover to none synchronised cell");

           gpl1_gprs_send_l1_mac_psho_cnf(PSHO_FAILURE, message_ptr->gas_id);

           GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
           return;
        }

        /* @@OG: It might be worth loading these into the dedicated data structure instead */
        gpl1_psho_data.target_cell_details = *target_cell_details_ptr;

        gpl1_psho_data.current_cell_details.arfcn = l1_idle_data->campedon_cell_ARFCN;
        gpl1_psho_data.current_cell_details.bsic  = l1_idle_data->campedon_cell_BSIC;
        gpl1_psho_data.current_cell_details.bsic_known = TRUE;
        /* store current cell info to previous
           for failure case and resort to original info */
        gpl1_psho_data.current_cell_details.TN = l1_get_current_timeslot(message_ptr->gas_id);

        /* This is because we're currently slammed to current_timeslot and
         * our own SCH is on timeslot 0
         */

        /* @@OG: I am not sure this is required */
        gpl1_psho_data.previous_cell_details.arfcn = l1_idle_data->campedon_cell_ARFCN;
        gpl1_psho_data.previous_cell_details.bsic  = l1_idle_data->campedon_cell_BSIC;
        gpl1_psho_data.previous_cell_details.bsic_known = TRUE;
        gpl1_psho_data.previous_cell_details.qbit_lag   = (int16)(( 0 - l1_get_current_timeslot(message_ptr->gas_id) ) * 625); /* @@OG: Not necessarily true */
        gpl1_psho_data.previous_cell_details.fn_lag     = 0;

        /* If qbit_lag is 0, don't adjust qbit_lag, otherwise subtract 1
         * because ncell qbit_lag would be -ve relative to our timeslot
         */
        if( 0 != gpl1_psho_data.previous_cell_details.qbit_lag  )
        {
          gpl1_psho_data.previous_cell_details.fn_lag =
          SUB_FN( gpl1_psho_data.previous_cell_details.fn_lag, 1);
        }


        MSG_GERAN_MED_1_G( "Prev cell arfcn %d",
                 gpl1_psho_data.previous_cell_details.arfcn.num);

        MSG_GERAN_MED_3_G( "Prev cell qbit lag=%d fn_lag=%d bsic_known %d",
                 gpl1_psho_data.previous_cell_details.qbit_lag,
                 gpl1_psho_data.previous_cell_details.fn_lag,
                 gpl1_psho_data.previous_cell_details.bsic_known );

        MSG_GERAN_MED_3_G( "Prev cell bsic %d ts %d Curr_ts %d",
                 gpl1_psho_data.previous_cell_details.bsic,
                 gpl1_psho_data.previous_cell_details.TN,
                 l1_get_current_timeslot(message_ptr->gas_id) );

        /* Store previous channel information, need to null a few parameters */
        /* Frequency info contained within this message */
        gpl1_psho_data.psho_backup_l1_transfer_data  = l1_transfer_data[gas_id];

        /* Store a copy of the TA and PTCCH info, check valid flag
           to see if required on fall back */
        gpl1_psho_data.psho_backup_gpl1_gprs_ta_info = l1_get_gpl1_gprs_ta_info(message_ptr->gas_id) ;

        /* Store the gamma and alpha parameters currently being used */
        /* These are required for handover failure case */
        gpl1_psho_data.psho_backup_gamma_alpha.alpha = gprs_serv_cell_meas_ptr->alpha;
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_gsm = gprs_serv_cell_meas_ptr->gamma_gsm;
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[0] = gprs_serv_cell_meas_ptr->gamma_tn[0];
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[1] = gprs_serv_cell_meas_ptr->gamma_tn[1];
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[2] = gprs_serv_cell_meas_ptr->gamma_tn[2];
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[3] = gprs_serv_cell_meas_ptr->gamma_tn[3];
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[4] = gprs_serv_cell_meas_ptr->gamma_tn[4];
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[5] = gprs_serv_cell_meas_ptr->gamma_tn[5];
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[6] = gprs_serv_cell_meas_ptr->gamma_tn[6];
        gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[7] = gprs_serv_cell_meas_ptr->gamma_tn[7];
        gpl1_psho_data.psho_backup_gamma_alpha.pmax = gprs_serv_cell_meas_ptr->pmax;
        gpl1_psho_data.psho_backup_gamma_alpha.ul_power_band = gprs_serv_cell_meas_ptr->ul_power_band;

        /* Store the PSHO sync type for use in isr level */
        gpl1_psho_data.psho_sync_type = message_ptr->psho_sync_type;

        switch( message_ptr->psho_type )
        {
          case  PSHO_GTOG:
          {
            int16  ps_handover_ta;

            /* Lots of stuff to do in here when moved to isr */
            MSG_GERAN_MED_0_G("PSHO_GTOG ");

            switch( message_ptr->psho_sync_type )
            {
              case PSHO_NON_SYNC:
              {
                /* Rach based on parameters provided on new channel structure
                 * ensure the handover has a valid reference as this mandatory for non sync PSHO
                 * By performing error checks earlier and returning if error is present.
                 * As per 45010-700, section 6.6, Non synd PSHO uses TA = 0 until PI is received
                 */
                ps_handover_ta = 0;
              }
              break;

              case PSHO_SYNC:
              {
                /* Calculate TA for use on new channel as per 45010-700, annex A  */
                /* this is the same calculation as Circuit Switch */
                int16  real_qbit_lag;
                uint8  timing_advance_in_use;

                int16  source_qbit = (8 - l1_get_current_timeslot(message_ptr->gas_id)) * 625;
                int16  otd         = ( gpl1_psho_data.target_cell_details.qbit_lag - source_qbit );

                while (otd < 0)
                {
                  otd = otd + 5000;
                }

                real_qbit_lag         = (word) otd;
                timing_advance_in_use = gl1_msg_pdch_retrieve_ta(message_ptr->gas_id);

                if ( real_qbit_lag > 4999 )
                {
                  real_qbit_lag -= 5000;
                  ps_handover_ta = (int16) ((real_qbit_lag/2 + timing_advance_in_use)%256);
                }
                /* This computation is a re-arrangement of the formula given in 45.010
                 * of OTD = RTD + t1 - t0.
                 */
                else if ( real_qbit_lag > 1250 )
                {
                  ps_handover_ta = (int16) ((timing_advance_in_use - (5000 - real_qbit_lag)/2)%256); /* @@OG Check if the compiler is optimising */
                }
                else
                {
                  ps_handover_ta = (int16) ((real_qbit_lag/2 + timing_advance_in_use)%256);
                }

                /* ps_handover_ta is signed, so check for negative is needed */
                if ( ps_handover_ta > 229  || ps_handover_ta < 0  )
                {
                  /* This is a negative timing advance - GSM 05.10 section A.3.3 */
                  ps_handover_ta = 0;
                }
                else if ( ps_handover_ta >  63 )
                {
                  /* Timing advance is too big - for NCI see GSM 04.08 table 10.55*/
                  if ( message_ptr->normal_cell_indication == 0 )
                  {
                    /* Ignore calculated value - use default */
                    ps_handover_ta = 63;
                  }
                }
                MSG_GERAN_MED_3_G("PSHO: PSHO_SYNC TA calc TA [%d] in use [%d] real_qbit_lag %d",ps_handover_ta,timing_advance_in_use,real_qbit_lag);
              }
              break;

              case PSHO_PRE_SYNC:
              {
                /*
                  If the timing advance is not present, use the default of 1 as specified
                  in 45010-700, section 6.6.
                */
                if ( message_ptr->pkt_ext_ta.pkt_ext_ta_present )
                {
                  ps_handover_ta = (uint8) message_ptr->pkt_ext_ta.pkt_ext_ta;
                }
                else
                {
                  ps_handover_ta = 1;
                }

                MSG_GERAN_MED_1_G("PSHO: PSHO_PRE_SYNC TA used [%d]",ps_handover_ta);
              }
              break;

              case  PSHO_NO_SYNC:
              default:
              {
                /* Incorrect PSHO sync type received, Dump F3,
                   signal status failure to MAC and continue */
                MSG_GERAN_ERROR_1_G( "PSHO sync type incorrect %d ",message_ptr->psho_sync_type);

                gpl1_gprs_send_l1_mac_psho_cnf(PSHO_FAILURE, message_ptr->gas_id);

                MSG_GERAN_MED_2_G("PL1: L1_MAC_PSHO_CNF [%d] [%d]",PSHO_FAILURE,current_fn);

                GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
                return;
              }
            } /* End of switch statement on PSHO sync type */

            /* Is handover present, then we are asked to send four access bursts with TA = 0 on PACCH */
            /* Send four AB on the PACCH */
            gpl1_psho_data.handover_reference_present = message_ptr->handover_reference_present;

            /* For 11 bit Access Bursts the top 3 bits are 000, hence we only get
               sent an uint8 handover reference in the interface message */
            gpl1_psho_data.handover_reference     = (uint16) message_ptr->handover_reference;
            gpl1_psho_data.psho_access_burst_type = message_ptr->psho_access_burst_type;

            /* Okay so far for GTOG PSHO, now decipher GPRS structures for new channel */

          } /* case  PSHO_GTOG */
          break;

          case  PSHO_GTOW:
          {
            MSG_GERAN_MED_1_G("PSHO_GTOW %d, this is not coded currently ", message_ptr->psho_type);
            GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
            return;

            /* @@OG: We are likely to just deactivate here */
            /*no break needed as the return terminates this switch statement*/
          }

          case  PSHO_WTOG:
          case  PSHO_NONE:
          default:
          {

            GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

            /* Incorrect PSHO type received, Dump F3,
               signal status failure to MAC and continue */

            gpl1_gprs_send_l1_mac_psho_cnf(PSHO_FAILURE, message_ptr->gas_id);

            MSG_GERAN_MED_3_G("PL1: L1_MAC_PSHO_CNF [%d] [%d] type %d ",PSHO_FAILURE,current_fn,message_ptr->psho_type);

            return;
          }
        } /* End of switch processing on PSHO type */

        /* stop the surround cell engine - seems like a common thing to do !! */
#if 0
        l1_sc_stop();   /* @@OG: I am not really sure we need to that in Packet transfer mode */
#endif

        /* now check the tbf type */
        switch( message_ptr->psho_tbf_type)
        {
          case PSHO_UL_TBF:
          {
              dynamic_tbf_information_T *dynamic_msg_dest_ptr = NULL;
              uint8  ul_ts_alloc = 0;
              uint8  l1_ul_ts_alloc = 0;
              uint8  i;

              MSG_GERAN_MED_0_G("PL1: PSHO_UL_TBF ");

              /* Use current sub state to determine current allocation */
              if ((l1_tskisr_blk->sub_state == L1_DYNAMIC_1) ||
                  (l1_tskisr_blk->sub_state == L1_DOWNLINK_1) ||
                  (l1_tskisr_blk->sub_state == L1_DOWNLINK_2) )
              {
                /* Reconfigure dynamic tbf */
                dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

                /* Setup pending dynamic structure */
                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;
              }
              else if ( l1_tskisr_blk->sub_state == L1_DYNAMIC_2)
              {
                /* Reconfigure dynamic tbf */
                dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

                /* Setup pending dynmaic structure */
                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
              }

              if (!dynamic_msg_dest_ptr)
              {
                GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

                /* something has gone wrong, report error and return */
                MSG_GERAN_ERROR_1_G("PL1: PSHO bad sub_state! %d ",l1_tskisr_blk->sub_state);
                gpl1_gprs_send_l1_mac_psho_cnf(PSHO_FAILURE, message_ptr->gas_id);
                return;
              }

#ifdef FEATURE_GSM_DTM
              /* Copy in the mac mode */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode = message_ptr->ul_tbf.mac_mode;
#else
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode = MAC_L1_MAC_MODE_DYNAMIC;
#endif


              /* Setup test mode for EGPRS
               * either TEST_MODE_OFF, TEST_MODE_B or TEST_MODE_SRB
               */
              dynamic_msg_dest_ptr->test_mode = message_ptr->ul_tbf.test_mode;
              dynamic_msg_dest_ptr->test_mode_tn = message_ptr->ul_tbf.ts_offset;

              /* If the ts offset is zero then only loopback lowest allocated Rx */
              if(message_ptr->ul_tbf.ts_offset == 0)
              {
                // note this is going to change in PSHO
                dynamic_msg_dest_ptr->test_mode_tn = gpl1_gprs_ts_conv_unslam(gprs_serv_cell_meas_ptr->lowest_tn.tn,message_ptr->gas_id);
              }


              if ( FALSE != message_ptr->ul_tbf.bep_period2.present )
              {
                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->bep_period2 =
                message_ptr->ul_tbf.bep_period2.bep_period2;
              }
              else
              {
                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
              }
#if 0
              /* set up pending allocation type */
              if ((l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC ) ||
                  (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) )
              {
                /* moving from dl, or dynamic + dl tbf to dl + dynamic ul tbf */
                l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;

              } else if (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_ALLOC )
              {
                /* reconfiguring dynamic allocation, still single ul tbf */
                l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;
              }
#endif
              /* This replaces the above code, here we are dealing with Uplink only
                 and it is not relevent to the source configuration */

              /* reconfiguring dynamic allocation, still single ul tbf */
              l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;

              /* initialise number of timeslots to zero */
              dynamic_msg_dest_ptr->nts = 0;

              for (i=0;i< MAX_TN;i++)
              {
                if (message_ptr->ul_tbf.usf[i].present)
                {
                  /* For debug purposes */
                  ul_ts_alloc |= ( 0x80 >> i );

                  l1_ul_ts_alloc |= (0x0001 << i );

                  dynamic_msg_dest_ptr->usf_value[i] = message_ptr->ul_tbf.usf[i].usf_value;
                  dynamic_msg_dest_ptr->nts++;

                  if ((uint8)dynamic_msg_dest_ptr->test_mode == L1_TEST_MODE_B)
                  {
                    /* Setup test mode B tn allocation */
                    gpl1_gprs_tm_b_store[gas_id].ul_nts++;
                  }

                } else
                  /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
                  dynamic_msg_dest_ptr->usf_value[i] = GPL1_INVALID;

              }

              /* Setup new usf granularity FALSE = 1 block tx , TRUE = 4 block tx */
              dynamic_msg_dest_ptr->usf_granularity = message_ptr->ul_tbf.usf_granularity;

              /* setup downlink power control parameter's */
              if (message_ptr->ul_tbf.dl_ctrl_param.params_valid)
              {
                l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
                l1_transfer_data[gas_id].dl_power_control.po = message_ptr->ul_tbf.dl_ctrl_param.po;
              } else
              {
                l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
              }

#if 0
              /* Removed as current pending allocation is Uplink only */
              if( NULL != l1_transfer_data[gas_id].dl_tbf_ptr )
              {
                  /* Make sure the 1rb gap is correctly
                   * set to TRUE only when necessary.
                   */

                 // Okay, ITP this might change if the dl is reconfigured !!!!!
                  l1_ul_ts_alloc |= l1_transfer_data[gas_id].dl_tbf_ptr->ts_allocation;
              }
#endif
              /* To be reomoved at a later PSHO stage */
              /* check if lowest numbered tn has changed */
              if( gpl1_gprs_lowest_tn_changed( l1_ul_ts_alloc , l1_get_current_timeslot(message_ptr->gas_id) ) )
              {
                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;


                /* Check if there is a PCA or PDAN w FAI or PDAN w CRD RRBP */

                /* Only create a starting time if we haven't got one */
                if (!message_ptr->ul_tbf.tbf_start_time_valid)
                {
                  message_ptr->ul_tbf.tbf_start_time_valid =
                  gpl1_gprs_transfer_find_rrbp_type(&message_ptr->ul_tbf.starting_time,message_ptr->gas_id);
                }


              } else
              {
                /* Is the TBF currently suspended ? */
                if (l1_transfer_data[gas_id].disable_rx_tx)
                {
                  /* Ensure we don't overwrite the gap and st var's if we are currently suspended */
                  l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap =
                  l1_transfer_data[gas_id].dynamic_tbf_ptr->allow_one_rb_gap;

                  l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time =
                  l1_transfer_data[gas_id].dynamic_tbf_ptr->starting_time;

                } else
                {
                  l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
                }

                MSG_GERAN_LOW_1_G(" FALSE nts %d",dynamic_msg_dest_ptr->nts);
              }

              /* @@OG: We need the radio block gap to be always set
               * in the PSHOVER case.
               */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;


              if (message_ptr->ul_tbf.power_control_params.present)
              {
                uint8 index;
                power_control_gamma_T *pwr_cntrl_ptr;

                /*  Update Alpha value  */
                gprs_serv_cell_meas_ptr->alpha = message_ptr->ul_tbf.power_control_params.alpha;

                /*  Extract gamma_tn values  */
                pwr_cntrl_ptr = &(message_ptr->ul_tbf.power_control_params.gamma_TN0);
                index = 0;

                do
                {
                  if (pwr_cntrl_ptr->present)
                  {
                    gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
                  }
                  else
                  {
                    /* As we reselecting to new cell in pkt transfer mode , use default values for gamma_tn
                    ** If no power control parameters, use PMAX power (GSM5.05 10.2.1)
                    */
                    gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
                  }

                  /*  Increment pointer  */
                  pwr_cntrl_ptr++;
                  index++;

                }while (index < 8);

              }
              else
              {
                uint8 index;
                /*  If no power control parameters, use PMAX power (GSM5.05 10.2.1) */
                /*  Easiest way is to set gamma_tn to 0, and alpha to 0 */
                for(index=0;index<8;index++)
                {
                  gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
                }
                gprs_serv_cell_meas_ptr->alpha = 0;
              }

              // ITP PSHO
              // Not sure this can be done until we receive the new serving cell info
              /*  Update uplink power based on the modified power control parameters. */
              gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

              /* Only create a starting time if we haven't got one */
              if (!message_ptr->ul_tbf.tbf_start_time_valid)
              {

                uint32 pca_fn;

                /* Save the RRBP so move the suspend block to after RRBP frame */
                message_ptr->ul_tbf.tbf_start_time_valid = TRUE;
                message_ptr->ul_tbf.starting_time  = ADD_FN( current_fn, 2 );


                /* Do we have any pending RRBP's to be sent */
                i = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

                if (i < SIZE_OF_RRBP_BUFF)
                {
                  /* We have found a PCA */
                  pca_fn = gpl1_gprs_get_rrbp_frame(i,message_ptr->gas_id);

                  /* Is the Frame number of the PCA before our PTR starting time ? */
                  if ( gpl1_gprs_is_frame_num_later(pca_fn,message_ptr->ul_tbf.starting_time))
                  {
                    message_ptr->ul_tbf.starting_time =  ((pca_fn + 8) % FRAMES_IN_HYPERFRAME);
                  }

                }

                MSG_GERAN_MED_1_G("ST made up %d",message_ptr->ul_tbf.starting_time);
              }

              /* check to see if a start time has been specified */
             // if (message_ptr->ul_tbf.tbf_start_time_valid)
             // {
             //  MSG_GERAN_ERROR_0_G( "PSHO invalid start time !!!");
             //  gpl1_gprs_send_l1_mac_psho_cnf(PSHO_FAILURE);
             //  return;
             // }

              /* check to see if a start time has been specified */
              if (message_ptr->ul_tbf.tbf_start_time_valid)
              {
                /*  Determine if starting time present and if it needs adjusting  */
                l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
                /* Note this can only be a 52 style starting time */
                if (gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                message_ptr->ul_tbf.starting_time,
                                                PCCCH_52, message_ptr->gas_id))
                {

                  /* Starting time has expired, set as if no starting time present */
                  message_ptr->ul_tbf.starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);
                  /*  Re - calculate with new starting time */
                  (void)gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                    message_ptr->ul_tbf.starting_time,
                                                    PCCCH_52, message_ptr->gas_id);
                }

                l1_transfer_data[gas_id].ul_st = dynamic_msg_dest_ptr->starting_time;

                if(  ( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
                   &&( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].dl_st )
                  )
                {
                  /* @@Warning: This is no functional change but is used
                   * to track the issue experienced in CR 62533 where PL1
                   * is unable to connect assignments in particular
                   * scenarios. This is because a new UL assignment must
                   * wait for an old DL assignment before it can be connected
                   * and vice versa.
                   */
                  if( l1_transfer_data[gas_id].ul_st < l1_transfer_data[gas_id].dl_st )
                  {
                    MSG_GERAN_ERROR_3_G(" PL1 unable to connect UL tbf st %u before DL tbf st %u ",
                              l1_transfer_data[gas_id].ul_st,
                              l1_transfer_data[gas_id].dl_st,
                              0 );
                  }
                }
              }
              else
              {
                /*  If an PDA is in progress then do not clear start time variables.  */
                if (l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN)
                {
                  l1_transfer_data[gas_id].starting_time_valid = FALSE; // move into transfer ASAP.
                  l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
                }
              }
              
          MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap,dynamic_msg_dest_ptr->starting_time);
          if((FALSE != l1_transfer_data[gas_id].starting_time_valid))
          {
            if(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap)
            {
              (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,dynamic_msg_dest_ptr->starting_time,2, gas_id);
              gpl1_g2x_ta_scheduler(gas_id);
            }
          }

              /* set up tbf establish type for send PH_CONNECT_IND to MAC */
              if (l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN)
              {
                l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;
              }

              MSG_GERAN_MED_1_G(" ul_ts_alloc  0x%X ", ul_ts_alloc);

              /* ul_ts_alloc used for debug */
              NOTUSED(ul_ts_alloc);
              /* signal to L1 ISR that a PSHO is pending */
              l1_tskisr_blk->main_command = L1_PSHO_COMMAND;

          }
          break;

          case PSHO_UL_DL_TBF:
          {
              downlink_tbf_information_T *dl_msg_dest_ptr = NULL;
              dynamic_tbf_information_T *dynamic_msg_dest_ptr=NULL;
              uint8 i;
              boolean connect_dl_only = FALSE;
              uint8  ul_ts_alloc = 0;
              uint8  l1_ul_ts_alloc = 0;

              MSG_GERAN_MED_0_G("PL1: PSHO_UL_DL_TBF ");

            if ((l1_tskisr_blk->sub_sub_state == L1_DL_1) ||
                (l1_tskisr_blk->sub_state == L1_DOWNLINK_1))
            {
              /* Reconfigure downlink as part of concurrent tbf */
              dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

              /* Setup pending downlink structure */
              l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

              /* double check we are not writing into our active dl structure */
              if (l1_transfer_data[gas_id].dl_tbf_ptr)
              {
                if (l1_transfer_data[gas_id].dl_tbf_ptr == &l1_transfer_data[gas_id].dl_tbf_2)
                {
                  MSG_GERAN_ERROR_0_G("pending_dl_tbf_ptr setup incorrectly");
                  /* Modify pointer's to ensure we don't overwrite our active data */

                  /* Reconfigure downlink as part of concurrent tbf */
                  dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

                  /* Setup pending downlink structure */
                  l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;
                }
              }

            } else if ( (l1_tskisr_blk->sub_sub_state == L1_DL_2) ||
                        (l1_tskisr_blk->sub_sub_state == L1_NO_DL)||
                        (l1_tskisr_blk->sub_state == L1_DOWNLINK_2))
            {
              /* new downlink with dynamic allocation or
               * reconfigure of downlink part or concurrent tbf */
              dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

              /* Setup pending downlink structure */
              l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

              /* double check we are not writing into our active dl structure */
              if (l1_transfer_data[gas_id].dl_tbf_ptr)
              {
                if (l1_transfer_data[gas_id].dl_tbf_ptr == &l1_transfer_data[gas_id].dl_tbf_1)
                {
                  MSG_GERAN_ERROR_0_G("pending_dl_tbf_ptr setup incorrectly");
                  /* Modify pointer's to ensure we don't overwrite our active data */

                  /* Reconfigure downlink as part of concurrent tbf */
                  dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

                  /* Setup pending downlink structure */
                  l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
                }
              }

            }

            if (!dl_msg_dest_ptr)
            {
              GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
              /* Something has gone wrongm report error and return */
              MSG_GERAN_ERROR_0_G("PL1: TRANSFER Bad sub_sub_state");
              return;
            }

            /* set up pending allocation type */
            l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;

            /* store new tn allocation in pending structure */
            dl_msg_dest_ptr->nts = gpl1_gprs_reorder_tn( &message_ptr->dl_tbf.ts_allocation,
                                                         &dl_msg_dest_ptr->ts_allocation);

            /* setup downlink power control parameter's */
            if (message_ptr->ul_tbf.dl_ctrl_param.params_valid)
            {
              l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
              l1_transfer_data[gas_id].dl_power_control.po = message_ptr->ul_tbf.dl_ctrl_param.po;
            } else
            {
              l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
            }

            if (message_ptr->dl_tbf.dl_tfi_present)
            {
              dl_msg_dest_ptr->dl_tfi_present = TRUE;
              dl_msg_dest_ptr->dl_tfi = message_ptr->dl_tbf.dl_tfi;
            } else
            {
              /* no tfi present so use current tfi
               * must be a reallocation as we need a dl tfi for
               * the dl assignment
               * Check that dl_tbf_ptr is valid just in case
               */
              if (l1_transfer_data[gas_id].dl_tbf_ptr)
              {
                dl_msg_dest_ptr->dl_tfi_present  = TRUE;
                dl_msg_dest_ptr->dl_tfi = l1_transfer_data[gas_id].dl_tbf_ptr->dl_tfi;
              } else
              {
                /* this is an assignment error!!! */
                MSG_GERAN_ERROR_0_G("PL1: TRANSFER No DL Tfi present");
              }
            }

            /****************************************************************
             ***** store dynamic allocation parameters in pending structure
             ****************************************************************/
#ifdef FEATURE_GSM_DTM
              /* Copy mac mode into the pending downlinktbf structure */
            dl_msg_dest_ptr->mac_mode = message_ptr->ul_tbf.mac_mode;

            if( (l1_tskisr_blk->l1_state == L1_DTM_MODE) &&
                (l1_transfer_data[gas_id].dynamic_tbf_ptr == &(l1_transfer_data[gas_id].dynamic_tbf_1)) )
            {
              /* Reconfigure dynamic tbf or add to existing DL tbf */
              dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

              /* Setup pending dynamic structure */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;
            }
            else
            if( (l1_tskisr_blk->l1_state == L1_DTM_MODE) &&
                (l1_transfer_data[gas_id].dynamic_tbf_ptr == &(l1_transfer_data[gas_id].dynamic_tbf_2)) )
            {
              /* reconfigure dynamic tbf */
              dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

              /* Setup pending dynamic structure */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

            }
            else
#endif /*FEATURE_GSM_DTM*/

            if ((l1_tskisr_blk->sub_state == L1_DYNAMIC_1)||
                (l1_tskisr_blk->sub_state == L1_DOWNLINK_1) ||
                (l1_tskisr_blk->sub_state == L1_DOWNLINK_2) )
            {
              /* Reconfigure dynamic tbf or add to existing DL tbf */
              dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

              /* Setup pending dynamic structure */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;

            } else if ( l1_tskisr_blk->sub_state == L1_DYNAMIC_2)
            {
              /* reconfigure dynamic tbf */
              dynamic_msg_dest_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;

              /* Setup pending dynamic structure */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
            }


            if (!dynamic_msg_dest_ptr)
            {
              GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
              /* Something has gone wrong report error and return */
              MSG_GERAN_ERROR_0_G("PL1: Transfer Bad sub_state");
              return;
            }

#ifdef FEATURE_GSM_DTM
            /* Copy in the mac mode */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode = message_ptr->ul_tbf.mac_mode;
#endif /* FEATURE_GSM_DTM */

            dynamic_msg_dest_ptr->nts = 0;

            for (i=0;i< MAX_TN;i++)
            {
              if (message_ptr->ul_tbf.usf[i].present)
              {
                ul_ts_alloc |= (0x80 >> i);
                l1_ul_ts_alloc |= ( 0x0001 << i );

                dynamic_msg_dest_ptr->usf_value[i] = message_ptr->ul_tbf.usf[i].usf_value;
                dynamic_msg_dest_ptr->nts++;
              } else
                /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
                dynamic_msg_dest_ptr->usf_value[i] = GPL1_INVALID;

            }

            /* ul_ts_alloc used for debug only */
            NOTUSED(ul_ts_alloc);

            MSG_GERAN_MED_1_G("ts reconf ul ts alloc 0x%X",ul_ts_alloc);

            /* Setup new usf granularity FALSE = 1 block tx , TRUE = 4 block tx */
            dynamic_msg_dest_ptr->usf_granularity = message_ptr->ul_tbf.usf_granularity;

            l1_transfer_data[gas_id].pending_dl_tbf_ptr->link_qual_meas_mode =
            message_ptr->dl_tbf.link_qual_meas_mode;

            l1_transfer_data[gas_id].pending_dl_tbf_ptr->bep_period2 =
            GPRS_PL1_BEP_PERIOD2_INVALID;

            /* Copy in rlc mode */
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->rlc_mode      = message_ptr->dl_tbf.rlc_mode;

            /* Set test mode for EGPRS as invalid since the test mode
             * variable is not part of the timeslot reconfigure message.
             */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->test_mode = TEST_MODE_INVALID;

            if( ( NULL != l1_transfer_data[gas_id].dynamic_tbf_ptr )
              &&( 0 == l1_ul_ts_alloc ) )
            {
                /* If the TS reconfig msg does not contain any ul timeslots
                 * and an uplink tbf is in progress then make sure
                 * the uplink tbfs timeslots are included when deciding
                 * on the 1 rb gap.
                 */
                uint32 timeslot = 0;
                uint8  *usf_ptr = &( l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[ 0 ] );

                while( timeslot < 8 )
                {
                    if( GPL1_INVALID != *usf_ptr )
                    {
                        l1_ul_ts_alloc |= (0x0001 << timeslot);
                    }
                    ++timeslot;
                    usf_ptr++;
                }
            }

            if( ( NULL != l1_transfer_data[gas_id].dl_tbf_ptr )
              &&( 0 == dl_msg_dest_ptr->ts_allocation ) )
            {
                /* If the TS reconfig msg does not contain any dl timeslots
                 * and a downlink tbf is in progress then make sure
                 * the downlink tbfs timeslots are included when deciding
                 * on the 1 rb gap.
                 */
                l1_ul_ts_alloc |= l1_transfer_data[gas_id].dl_tbf_ptr->ts_allocation;
            }
            /* check if lowest numbered tn has changed or */
            if (gpl1_gprs_lowest_tn_changed( ( dl_msg_dest_ptr->ts_allocation | l1_ul_ts_alloc ), l1_get_current_timeslot(message_ptr->gas_id) ) )

            {
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;
              l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = TRUE;

              /* Check if there is a PCA or PDAN w FAI or PDAN w CRD RRBP */
              // Changed to use tbf_start_time_valid for PSHO, this will not be valid.
              // if (!message_ptr->starting_time_present)
              if (!message_ptr->dl_tbf.tbf_start_time_valid)
              {
                message_ptr->dl_tbf.tbf_start_time_valid = gpl1_gprs_transfer_find_rrbp_type(&message_ptr->dl_tbf.tbf_start_time,message_ptr->gas_id);
              }


              MSG_GERAN_LOW_0_G(" ts gap = TRUE");
            } else
            {
              if (l1_transfer_data[gas_id].disable_rx_tx)
              {
                /* Make sure the dl_tbf_ptr is set before using it */
                if( NULL != l1_transfer_data[gas_id].dl_tbf_ptr )
                {
                  l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap =
                  l1_transfer_data[gas_id].dl_tbf_ptr->allow_one_rb_gap;

                  l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time =
                  l1_transfer_data[gas_id].dl_tbf_ptr->tbf_start_time;
                }
                else
                {
                  l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;

                  MSG_GERAN_MED_0_G(" NULL dl_tbf_ptr ");
                }

                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap =
                l1_transfer_data[gas_id].dynamic_tbf_ptr->allow_one_rb_gap;

                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time =
                l1_transfer_data[gas_id].dynamic_tbf_ptr->starting_time;
              } else
              {
                l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
                l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;
              }
              MSG_GERAN_LOW_0_G(" ts gap = FALSE");
            }

            /* @@OG: We need the radio block gap to be always set
             * in the PSHOVER case.
             */
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = TRUE;
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;

             /****************************************************************
             ***** Setup power control and TA parameters
             ****************************************************************/
             /* supplied in dynamic and dl structures for PSHO */
            if (message_ptr->ul_tbf.power_control_params.present)
            {
              uint8 index;
              power_control_gamma_T *pwr_cntrl_ptr;

              /*  Update Alpha value  */
              gprs_serv_cell_meas_ptr->alpha = message_ptr->ul_tbf.power_control_params.alpha;

              /*  Extract gamma_tn values  */
              pwr_cntrl_ptr = &(message_ptr->ul_tbf.power_control_params.gamma_TN0);
              index = 0;

              do
              {
                if (pwr_cntrl_ptr->present)
                {
                  gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
                }
                else
                {
                  /* As we reselecting to new cell in pkt transfer mode , use default values for gamma_tn
                  ** If no power control parameters, use PMAX power (GSM5.05 10.2.1)
                  */
                  gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
                }

                /*  Increment pointer  */
                pwr_cntrl_ptr++;
                index++;

              }while (index < 8);
            }
            else
            {
              uint8 index;
              /*  If no power control parameters, use PMAX power (GSM5.05 10.2.1) */
              /*  Easiest way is to set gamma_tn to 0, and alpha to 0 */
              for(index=0;index<8;index++)
              {
                gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
              }
              gprs_serv_cell_meas_ptr->alpha = 0;
            }

            /*  Update uplink power based on the modified power control parameters. */
            gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

            /* Removed for PSHO gpl1_gprs_add_global_ta_param(message_ptr->global_packet_ta); */

            if (!message_ptr->dl_tbf.tbf_start_time_valid)
            {
              uint32 pca_fn;
              uint32 psho_current_fn = gl1_get_FN( message_ptr->gas_id );

              /* Save the RRBP so move the suspend block to after RRBP frame */
              message_ptr->dl_tbf.tbf_start_time_valid = TRUE;

              /* Improve the reaction time of L1
               * Only account for the task to ISR context switch and hence add 1 tdma frame.
               */
              message_ptr->dl_tbf.tbf_start_time  = ADD_FN( psho_current_fn, 2 );

              /* Do we have any pending RRBP's to be sent */
              i = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

              if (i < SIZE_OF_RRBP_BUFF)
              {
                /* We have found a PCA */
                pca_fn = gpl1_gprs_get_rrbp_frame(i,message_ptr->gas_id);

                /* Is the Frame number of the PCA before our PTR starting time ? */
                if ( gpl1_gprs_is_frame_num_later(pca_fn,message_ptr->dl_tbf.tbf_start_time))
                {
                  message_ptr->dl_tbf.tbf_start_time =  ((pca_fn + 8) % FRAMES_IN_HYPERFRAME);
                }

              }

             MSG_GERAN_MED_1_G("ST made up %d",message_ptr->dl_tbf.tbf_start_time);

            }

            /* check to see if a start time has been specified */
            if (message_ptr->dl_tbf.tbf_start_time_valid)
            {
              MSG_GERAN_MED_0_G("ST present");

              /*  Determine if starting time present and if it needs adjusting  */
              /* Note this can only be a 52 style starting time */

              if (l1_transfer_data[gas_id].ul_st != L1_GPRS_INVALID_FN)
              {
                /* Waiting for start time associated with uplink config */
                /*  Update with the re-config starting time */
                l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

                if (gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                message_ptr->dl_tbf.tbf_start_time,
                                                PCCCH_52, message_ptr->gas_id))
                {

                  /*  Starting time has expired, set as if no starting time present */
                  message_ptr->dl_tbf.tbf_start_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

                  /*  Re - calculate with new starting time */
                  (void)gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                    message_ptr->dl_tbf.tbf_start_time,
                                                    PCCCH_52, message_ptr->gas_id);
                }

                l1_transfer_data[gas_id].ul_st = dynamic_msg_dest_ptr->starting_time;

              } else if (l1_transfer_data[gas_id].dl_st != L1_GPRS_INVALID_FN)
              {
                /* Waiting for start time associated with dwnlink config */
                /*  Update with the re-config starting time */
                l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

                if (gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                message_ptr->dl_tbf.tbf_start_time,
                                                PCCCH_52, message_ptr->gas_id))
                {
                  /*  Starting time has expired, set as if no starting time present */
                  message_ptr->dl_tbf.tbf_start_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

                  /*  Re - calculate with new starting time */
                  (void)gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                    message_ptr->dl_tbf.tbf_start_time,
                                                    PCCCH_52, message_ptr->gas_id);

                }
                l1_transfer_data[gas_id].dl_st = dynamic_msg_dest_ptr->starting_time;

              } else
              {
                /* no ul or dl starting currently active so set up normal case */

                if (gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                message_ptr->dl_tbf.tbf_start_time,
                                                PCCCH_52, message_ptr->gas_id))
                {
                  /*  Starting time has expired, set as if no starting time present */
                  message_ptr->dl_tbf.tbf_start_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

                  /*  Re - calculate with new starting time */
                  (void)gpl1_gprs_get_starting_time(&dynamic_msg_dest_ptr->starting_time,
                                                    message_ptr->dl_tbf.tbf_start_time,
                                                    PCCCH_52, message_ptr->gas_id);
                }

              }

            } else
            {
              l1_transfer_data[gas_id].starting_time_valid = FALSE; // move into transfer ASAP.
              l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
            }

          MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap,dynamic_msg_dest_ptr->starting_time);
          if((FALSE != l1_transfer_data[gas_id].starting_time_valid))
          {
            if(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap)
            {
              (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,dynamic_msg_dest_ptr->starting_time,2, gas_id);
              gpl1_g2x_ta_scheduler(gas_id);
            }
          }
            /***
             *** Work out if an exiting uplink TBF is to be released before
             *** PTR is to be connected
             ***/
            if ((l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) ||
                (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_ALLOC))
            {
              /* We are either in concurency or Ul only */
              if ((l1_transfer_data[gas_id].release_pending == L1_PARTIAL_RELEASE_UL) ||
                  (l1_transfer_data[gas_id].release_pending == L1_FULL_RELEASE_UL))
              {
                /* Search for theFN of the FAI UL PCA to determine when UL will be released */
                connect_dl_only = FALSE;

                /* do we have to wait to transmit pca before going to dl only */
                if (l1_transfer_data[gas_id].wait_for_pca_valid == FALSE)
                {
                  i = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

                  if (i < SIZE_OF_RRBP_BUFF)
                  {
                    /* Is the Frame number of the PCA before our PTR starting time ? */
                    if ( l1_transfer_data[gas_id].starting_time > gpl1_gprs_get_rrbp_frame(i,message_ptr->gas_id) )
                    {
                      connect_dl_only = TRUE;
                    }
                  }
                } else
                {
                  /* has the pca gone out before starting time if so go to dl only */
                  /* One has to use the actual FN when PCA was sent rather that wait for PCA FN
                    which is adjusted by +9 to send REL_CNF as we already have DL ,
                    and not going to Idle , doing a REL_CNF on PCA TX boundary should be fine
                  */
                  if (SUB_FN(l1_transfer_data[gas_id].wait_for_pca_fn,GPL1_WAITING_PERIOD_FOR_UL_REL_CNF) <
                      l1_transfer_data[gas_id].starting_time)
                  {
                    /* As PCA is sent  make it false as we will be sending REL_CNF for UL below */
                    l1_transfer_data[gas_id].pca_sent = FALSE;

                    MSG_GERAN_MED_2_G(" pca_fn [%d] st %d pca tx , send ul rel cnf as pca sent.",
                              l1_transfer_data[gas_id].wait_for_pca_fn,l1_transfer_data[gas_id].starting_time);

                    l1_transfer_data[gas_id].wait_for_pca_fn    = L1_GPRS_INVALID_FN;
                    l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;

                    connect_dl_only = TRUE;
                  } else
                  {
                    connect_dl_only = FALSE;
                  }
                }

              }
            }

            if (connect_dl_only)
            {
              /* reset the MAC mode associated with the released
               * uplink tbf.
               */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode =
                  L1_MAC_MODE_INVALID;

              MSG_GERAN_HIGH_0_G(" UL released connect DL only");

              l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;

              /* set up tbf establish type for send PH_CONNECT_IND to MAC */
              l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

              /* Now tell MAC that we have cancelled the pending uplink */
              gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_UL, message_ptr->gas_id);

              /* reset relelase type and rel confirm status*/
              l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
              l1_transfer_data[gas_id].confirm_pending = SENT;
            } else
            {
              /* set up tbf establish type for send PH_CONNECT_IND to MAC */
              l1_transfer_data[gas_id].tbf_est_type = UPLINK_DOWNLINK_TBF_ESTABLISHED;

              /* Set the ul and dl st so that the release handlers are
               * aware of the packet timeslot reconfigure.
               */
              l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].starting_time;
              l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].starting_time;
            }
            break;
          }

          case PSHO_NO_TBF: /* @@OG: Why is this an error condition ?
                             * I think its because we always an uplink TBF to successfully
                             * complete the handover, especially in non-sync or is it always?
                             */
          default:
          {
              GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

              MSG_GERAN_MED_0_G("PL1: Invalid PSHO TBF type ");
            return;
          }
        } /* Switch on tbf type */

        /* signal to L1 ISR that a PSHO is pending */
        l1_tskisr_blk->main_command = L1_PSHO_COMMAND;    /* @@OG: This will over-write the main command set by UL/DL above
                                                           * Which might not be needed in this case...
                                                           */
      GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
      break;

      } /* End of MAC_L1_PSHO_REQ */

      /*******************************************************
      *                                                      *
      *                MAC_L1_PSHO_STATUS                    *
      *                                                      *
      ********************************************************/
      case MAC_L1_PSHO_STATUS:
      {

        mac_l1_psho_status_t*  message_ptr = ( mac_l1_psho_status_t *) msg_header;
        volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[message_ptr->gas_id];
        gpl1_gprs_transfer_if_data_t *gpl1_gprs_transfer_if_data_ptr = &gpl1_gprs_transfer_if_data[message_ptr->gas_id];
        uint32 current_fn = gl1_get_FN( message_ptr->gas_id );

        switch (message_ptr->psho_status)
        {
            case PSHO_SUCCESS:
            {
                gas_id = message_ptr->gas_id;
                MSG_GERAN_MED_1_G("PL1: MAC_L1_PSHO_STATUS PSHO_SUCCESS [%d]",message_ptr->psho_status);

                /* AGV REM Make sure the GPRS Sliding Avg is initialised properly. */
                gprs_serv_cell_meas_ptr->state = SCM_INIT;

                /* Do the initialisation for the serve cell structures */
                (void)gpl1_gprs_serving_cell_meas(0,message_ptr->gas_id);

                /* Reset the flag which allows c value calculations
                 * to run on ccch during cs idle.
                 */
                gprs_serv_cell_meas_ptr->cal_c_value_51_idle = FALSE;

                /* Convey nv item information to the egprs quality measurements
                 * module.
                 */
                gpl1_egprs_scell_qual_meas_nv_item_config( gl1_is_anite_gcf_flag_set(message_ptr->gas_id), message_ptr->gas_id );


            }
            break;

            case PSHO_FAILURE:
            {
               /*
                * This is similar to the handling of the MAC_L1_PSHO_REQ
                * message except that we don't need to save backups this time.
                */
                gas_id = message_ptr->gas_id;
                MSG_GERAN_MED_1_G("PL1: MAC_L1_PSHO_STATUS PSHO_FAILURE [%d]",message_ptr->psho_status);

                GL1_ISR_SAVE_LOCK(message_ptr->gas_id);

                /* Re Store backup info after handover failure */

                /* Re Store the TA and PTCCH info if previously valid  */
                if (gpl1_psho_data.psho_backup_gpl1_gprs_ta_info.ta_valid)
                {
                  l1_set_gpl1_gprs_ta_info (gpl1_psho_data.psho_backup_gpl1_gprs_ta_info,message_ptr->gas_id);
                }

                /* Re Store the gamma and alpha parameters */
                gprs_serv_cell_meas_ptr->alpha = gpl1_psho_data.psho_backup_gamma_alpha.alpha;
                gprs_serv_cell_meas_ptr->gamma_gsm = gpl1_psho_data.psho_backup_gamma_alpha.gamma_gsm;
                gprs_serv_cell_meas_ptr->gamma_tn[0] = gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[0];
                gprs_serv_cell_meas_ptr->gamma_tn[1] = gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[1];
                gprs_serv_cell_meas_ptr->gamma_tn[2] = gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[2];
                gprs_serv_cell_meas_ptr->gamma_tn[3] = gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[3];
                gprs_serv_cell_meas_ptr->gamma_tn[4] = gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[4];
                gprs_serv_cell_meas_ptr->gamma_tn[5] = gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[5];
                gprs_serv_cell_meas_ptr->gamma_tn[6] = gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[6];
                gprs_serv_cell_meas_ptr->gamma_tn[7] = gpl1_psho_data.psho_backup_gamma_alpha.gamma_tn[7];
                gprs_serv_cell_meas_ptr->pmax = gpl1_psho_data.psho_backup_gamma_alpha.pmax;
                gprs_serv_cell_meas_ptr->ul_power_band = gpl1_psho_data.psho_backup_gamma_alpha.ul_power_band;

#if 0 /* @@OG I am not sure thats the best way to do it for l1_transfer_data[gas_id] */

                l1_transfer_data[gas_id]  = gpl1_psho_data.psho_backup_l1_transfer_data;
#endif

                /* @@OG: What about the ptcch configuration ?? Are we sure that is saved as well?
                 *
                 */

                /* Find which one of the frequency_info_2 and frequency_info_1 configurations we need to use */
                if( NULL != gpl1_psho_data.psho_backup_l1_transfer_data.frequency_info_ptr )
                {
                  if( &l1_transfer_data[gas_id].frequency_info_1 == l1_transfer_data[gas_id].frequency_info_ptr )
                  {
                    l1_transfer_data[gas_id].pending_frequency_info_ptr = &l1_transfer_data[gas_id].frequency_info_1;
                  }
                  else
                  {
                    l1_transfer_data[gas_id].pending_frequency_info_ptr = &l1_transfer_data[gas_id].frequency_info_2;
                  }

                  /* The pointer is setup, its safe to copy into it now */
                  *(l1_transfer_data[gas_id].pending_frequency_info_ptr) =
                    *(gpl1_psho_data.psho_backup_l1_transfer_data.frequency_info_ptr);
                }
                else
                {
                  l1_transfer_data[gas_id].pending_frequency_info_ptr = NULL;
                }

                /* Need to perform the handover back to the old current cell
                 * By performing the following:
                 *
                 * Swap current and target cells.
                 */
                gpl1_psho_data.target_cell_details = gpl1_psho_data.current_cell_details;

                /* Don't worry about the idle data here, this will be swapped back when completing this
                 * handover in the dynamic control function.
                 */

                gpl1_psho_data.handover_reference_present = FALSE; /* Make sure we don't send access bursts when
                                                                    * falling back on the old channel and cell.
                                                                    */

                /* Swap current and pending tbf configuration pointers and data using
                 * the backed-up packet transfer configurations.
                 * Issue the L1_PSHO_COMMAND at the end.
                 *
                 */
                l1_transfer_data[gas_id].pending_alloc_type = gpl1_psho_data.psho_backup_l1_transfer_data.current_alloc_type;

                /* Setup the tbf establishment type */
                l1_transfer_data[gas_id].tbf_est_type = gpl1_psho_data.psho_backup_l1_transfer_data.tbf_est_type;

                /* Calculate the start time for this handover here */
                l1_transfer_data[gas_id].starting_time_valid   = TRUE;
                l1_transfer_data[gas_id].starting_time_elapsed = FALSE;

                l1_transfer_data[gas_id].starting_time = ADD_FN( current_fn, 2 );

                /* Make sure we are aligned correctly */
                gpl1_gprs_chk_blk_bndry( &(l1_transfer_data[gas_id].starting_time) );

                /* Find which one of the dl_tbf_2 and dl_tbf_1 configurations we need to use */
                if( NULL != gpl1_psho_data.psho_backup_l1_transfer_data.dl_tbf_ptr )
                {
                  if( &l1_transfer_data[gas_id].dl_tbf_1 == l1_transfer_data[gas_id].dl_tbf_ptr )
                  {
                    l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;
                  }
                  else
                  {
                    l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
                  }

                  /* The pointer is setup, its safe to copy into it now */
                  *(l1_transfer_data[gas_id].pending_dl_tbf_ptr) =
                    *(gpl1_psho_data.psho_backup_l1_transfer_data.dl_tbf_ptr);

                  /* Alow one radio block gap */
                  l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = TRUE;

                  l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time   = l1_transfer_data[gas_id].starting_time;

                  /* Setup the dl start time */
                  l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].starting_time;
                }
                else
                {
                  l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
                }

                /* Find which one of the dynamic_tbf_1 and dynamic_tbf_2 configurations we need to use */
                /* Find which one of the dl_tbf_2 and dl_tbf_1 configurations we need to use */
                if( NULL != gpl1_psho_data.psho_backup_l1_transfer_data.dynamic_tbf_ptr )
                {
                  if( &l1_transfer_data[gas_id].dynamic_tbf_1 == l1_transfer_data[gas_id].dynamic_tbf_ptr )
                  {
                    l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
                  }
                  else
                  {
                    l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;
                  }

                  /* The pointer is setup, its safe to copy into it now */
                  *(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr) =
                    *(gpl1_psho_data.psho_backup_l1_transfer_data.dynamic_tbf_ptr);

                  /* Alow one radio block gap */
                  l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;

                  l1_transfer_data[gas_id].dynamic_tbf_ptr->starting_time = l1_transfer_data[gas_id].starting_time;

                  /* Setup the dl and ul start times */
                  l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].starting_time;

                }
                else
                {
                  l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
                }

                /*
                 * The power control parameters? Need to prime the scell ones again.
                 * alpha, gamma also.
                 * Update the rf power.
                 */
                gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

                /* Setup the starting time and radio block gap flag */
                l1_tskisr_blk->main_command = L1_PSHO_REVERT_COMMAND;
                gpl1_gprs_transfer_if_data_ptr->l1_psho_revert_command = TRUE;
                GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
            }
            break;

            default:
            {
                /* Print a debug message */
                gas_id = message_ptr->gas_id;
                MSG_GERAN_MED_1_G("PL1: MAC_L1_PSHO_STATUS Unknown statue [%d]",message_ptr->psho_status);

                /* What is the best action to be done here ?
                 * Should we assume this is successful or treat suspiciously as a bad case and revert back
                 * anyways.
                 */
            }
            break;
        }  /* End of PSHO status switch */

      break;
      }

#endif  /* End of FEATURE_GPRS_PS_HANDOVER */

      /*******************************************************
      *                                                      *
      *                MAC_L1_PACCH_RRBP                     *
      *                                                      *
      ********************************************************/
      case MAC_L1_PACCH_RRBP:
      {
        mac_l1_pacch_rrbp_t *message_ptr;
        boolean rrbp_and_config_ul_clash=FALSE;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_pacch_rrbp_t *) msg_header;
        gas_id = message_ptr->gas_id;

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);


        /* ETSI 04.14: When in Radio Block Loopback Sub-mode ON,
         * MS should not send any control messages or data from
         * RLC to its own physical Link layer for transmission.
         */
        if( ( L1_TEST_MODE_SRB_ON != l1_transfer_data[gas_id].l1_test_mode )
            ||(  ( L1_TEST_MODE_SRB_ON == l1_transfer_data[gas_id].l1_test_mode )
            &&( PCA_SRB == message_ptr->rrbp_msg ) ) )
        {
#ifdef FEATURE_G2X_TUNEAWAY
          if( l1_transfer_data[gas_id].dl_tbf_ptr != NULL )
          {

            if( (l1_transfer_data[gas_id].dl_tbf_ptr->dl_tbf_with_t3192_active == TRUE) && ((message_ptr->rrbp_msg == PCA_DL_FAI_MSG  ) || (message_ptr->rrbp_msg == PDAN_MSG_WITH_FAI)) )
            {
              /* Gl1 has stopped the t3192 timer and does not want to start it again as it is about to connect new PDA
               * therefore don't add following rrbp types to que as they will restart the timer
               */
        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
              MSG_GERAN_HIGH_0_G("PACCH_RRBP ignored to prevent start of T3192");
              break;
            }
          }
#endif
          gpl1_gprs_transfer_add_rrbp_msg(( mac_l1_pacch_rrbp_t *) message_ptr);

          /* Check if this RRBP message is a PCA and it is due to be scheduled when a block's gap is required
           * due to lowest timeslot changing
           */
          if (l1_transfer_data[gas_id].starting_time_valid)
          {
            switch (message_ptr->rrbp_msg)
            {
              case PCA_UL_MSG:
              case PCA_DL_MSG:
              case PCA_UL_FAI_MSG:
              case PCA_DL_FAI_MSG:
              case PCA_SRB:
              {

                  if (l1_transfer_data[gas_id].pending_dynamic_tbf_ptr)
                  {

                  /* Uplink allocation start time is before PCA FN or it's aligned with
                   * blocks gap as lowest TN changed, ST needs moving
                       */

                   if ( ((l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap) &&
                         (l1_transfer_data[gas_id].ul_st != L1_GPRS_INVALID_FN)  &&
                         (l1_transfer_data[gas_id].ul_st == message_ptr->absolute_fn)) ||
                        ((l1_transfer_data[gas_id].ul_st != L1_GPRS_INVALID_FN) &&
                         (IS_FRAME_NUM_LATER(message_ptr->absolute_fn,l1_transfer_data[gas_id].ul_st)) )
                      )
                      {
                        uint32 temp_st;

                        /* block's gap needs moving to block after this PCA */
                        MSG_GERAN_ERROR_1_G(" PCA clash UL ST moved!! [%d]",gl1_get_FN( message_ptr->gas_id ));
                        temp_st  = ((message_ptr->absolute_fn + 4) % FRAMES_IN_HYPERFRAME);

                        /*  Re - calculate with new starting time */
                        l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

                        /* Set flag to TRUE. This is required in case there is also a
                         * pending dl config.
                         */
                        rrbp_and_config_ul_clash = TRUE;
                        (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time,
                                                          temp_st,
                                                          PCCCH_52, message_ptr->gas_id);
                        l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time;
                      }

                    }

                  if (l1_transfer_data[gas_id].pending_dl_tbf_ptr)
                  {

                    if ( ((l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap) &&
                          (l1_transfer_data[gas_id].dl_st != L1_GPRS_INVALID_FN)  &&
                          (l1_transfer_data[gas_id].dl_st == message_ptr->absolute_fn)) ||
                         ((l1_transfer_data[gas_id].dl_st != L1_GPRS_INVALID_FN) &&
                          (IS_FRAME_NUM_LATER(message_ptr->absolute_fn,l1_transfer_data[gas_id].dl_st)) )
                       )
                    {
                      /* A downlink allocation requires a block's gap, does this allign with
                       * our PCA tx?
                       */

                        uint32 temp_st;
                        /* block's gap needs moving to block after this PCA */
                        MSG_GERAN_HIGH_1_G(" PCA clash DL ST moved!! [%d]",gl1_get_FN( message_ptr->gas_id ));
                        temp_st  = ((message_ptr->absolute_fn + 4) % FRAMES_IN_HYPERFRAME);

                        if(rrbp_and_config_ul_clash)
                        {
                          /* There is also a pending ul config.
                           * In such case the starting time needs to be adjusted
                           * within function, gpl1_gprs_get_starting_time.
                           */
                          l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
                          l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
                          (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time,
                                                            temp_st,
                                                            PCCCH_52, message_ptr->gas_id);

                          l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time;
                        }
                        else
                        {
                          /*  Re - calculate with new starting time */
                          l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

                          (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time,
                                                          temp_st,
                                                          PCCCH_52, message_ptr->gas_id);

                          l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time;
                        }
                    }
                  }

              }    /* End of case statement */
              break;

              default:
              break;
            } /* end of switch statement */

          } /* end if starting_time_valid */
        }  /* End of if statement */
        else
        {
          MSG_GERAN_HIGH_0_G("RRBP Ignored in SRB mode");
        }
        gpl1_gprs_check_for_rrbp_duplicates(message_ptr->gas_id);
        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
        break;
      }

      /*******************************************************
      *                                                       *
      *                MAC_L1_CS_RECONFIG                     *
      *                                                       *
      ********************************************************/
    case MAC_L1_CS_RECONFIG:
      {
        /* No handling in GL1 RLC controls access to FIFO */

        break;
      }

      /*******************************************************
      *                                                       *
      *                MAC_L1_CONTENTION_RES                  *
      *                MAC_L1_STOP_RLC_TX                     *
      ********************************************************/
    case MAC_L1_STOP_RLC_TX:
    case MAC_L1_CONTENTION_RES:
      {
        /* No handling in GL1 RLC controls access to FIFO */
        break;
      }

      /*******************************************************
      *                                                       *
      *                MAC_L1_POWER_CONTROL                   *
      *                                                       *
      ********************************************************/
    case MAC_L1_POWER_CONTROL:
      {
        mac_l1_pwr_ctrl_t   *message_ptr;
        boolean ignore_reconfigure = FALSE;
        volatile ISRTIM_CMD_BLK* l1_tskisr_blk;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
        message_ptr = ( mac_l1_pwr_ctrl_t *) msg_header;

        gas_id = message_ptr->gas_id;
        MSG_GERAN_MED_0_G("MAC_L1_POWER_CONTROL Rcvd");

        l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[message_ptr->gas_id];

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);

        if (message_ptr->global_packet_ta_present)
        {
          ignore_reconfigure = gpl1_gprs_config_global_ta_param(message_ptr->global_packet_ta,message_ptr->gas_id);
        }
        else
        {
          /* No Global TA present so ignore the reconfigure */
          ignore_reconfigure = TRUE;
        }

        if (message_ptr->power_control.present)
        {
          /* handle power control bits */
          uint8 index;
          power_control_gamma_T *pwr_cntrl_ptr;

          /*  Update Alpha value  */
          gprs_serv_cell_meas_ptr->alpha = message_ptr->power_control.alpha;

          /*  Extract gamma_tn values  */
          pwr_cntrl_ptr = &(message_ptr->power_control.gamma_TN0);
          index = 0;

          do
          {
            if (pwr_cntrl_ptr->present)
              gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;

            /*  Increment pointer  */
            pwr_cntrl_ptr++;
            index++;

          }while (index < 8);

          /*  Update uplink power based on the modified power control parameters. */
          gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);
        }

        if (!ignore_reconfigure)
        {
          /* only force a reconfigure if we have no release pending and
           * we are not waiting for a starting time
           */
          if ((l1_transfer_data[gas_id].release_pending == L1_NO_RELEASE) &&
              (l1_transfer_data[gas_id].starting_time_valid != TRUE))
          {
            /* setup pending allocation type to current allocation type */
            l1_transfer_data[gas_id].pending_alloc_type = l1_transfer_data[gas_id].current_alloc_type;

            if(gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TBF_TS_CHANGE, gas_id) == ONGOING)
            {
                /*if lowest TS change is running, wait for it to completed*/
                while(gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TBF_TS_CHANGE, gas_id) == ONGOING)
                {
                   GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
                   (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
                   (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
                   GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
                }
            }
            /* signal to isr to change power control and/or timing advance */
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
          }
        }
          GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

        break;
      }
      /*******************************************************
      *                                                       *
      *                MAC_L1_PDCH_REL_TS                     *
      *                                                       *
      ********************************************************/
    case MAC_L1_PDCH_REL_TS:
      {
        mac_l1_pdch_rel_ts_t *message_ptr;
        uint8                rel_ts_alloc=0;
        uint8                stored_rel_ts_alloc = 0;
        uint8                dl_ts_alloc=0;
        uint8                pending_dl_ts_alloc=0;
        uint8                *usf_value=NULL;
        uint8                pending_usf[MAX_TN];
        boolean              dl_tbf_present=FALSE;
        boolean              dynamic_tbf_present=FALSE;
        boolean              dynamic_ts_changed = FALSE;
        boolean              dl_ts_changed = FALSE;
        uint8                rel_nts=0;
        uint8                dynamic_nts=0;
        uint8                dl_nts=0;
        uint8                i=0;
        boolean              starting_time_present = FALSE;
        uint32               starting_time= L1_GPRS_INVALID_FN;
        grm_client_enum_t    g_client;
        volatile ISRTIM_CMD_BLK* l1_tskisr_blk;
        uint8                rrbp_index;
        boolean              go_to_idle = FALSE;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_pdch_rel_ts_t *) msg_header;

        gas_id = message_ptr->gas_id;
        MSG_GERAN_MED_0_G("PL1: MAC_L1_PDCH_REL_TS Rcvd");
        g_client = gl1_map_gas_id_to_client_id(gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        /*lint +e740 */

        /* Ignore this message if our main_command event is set
         * and we don't have a pending release
         * This is required in cases where a timeslot gets released
         * which releases a TBF and reduces the number of tn's on a
         * seperate TBF - CR 94182
         */
        if ( (l1_tskisr_blk->main_command != L1_NULL_COMMAND) &&
             (l1_transfer_data[gas_id].release_pending == L1_NO_RELEASE) )
        {
          MSG_GERAN_HIGH_0_G(" L1 change of state in progress, ignore msg");
          return;
        }

        GL1_ISR_SAVE_LOCK(gas_id);

        /* setup dl tn allocation if a dl tbf is present */
        if (l1_transfer_data[gas_id].dl_tbf_ptr)
        {
          dl_ts_alloc = l1_transfer_data[gas_id].dl_tbf_ptr->ts_allocation;
          dl_nts = l1_transfer_data[gas_id].dl_tbf_ptr->nts;
          dl_tbf_present = TRUE;
        }

        /* test if dynamic tbf is being used and setup shortcut pointers */
        if (l1_transfer_data[gas_id].dynamic_tbf_ptr)
        {
          usf_value = &(l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[0]);
          dynamic_nts = l1_transfer_data[gas_id].dynamic_tbf_ptr->nts;

          if (dynamic_nts)
          {
            dynamic_tbf_present = TRUE;
          }
        }



        /* re-order the timeslots to TN0 in bit 0 and TN7 in bit 7 */
        rel_nts = gpl1_gprs_reorder_tn(&message_ptr->ts_allocation,
                                       &rel_ts_alloc);


        /* store rel_ts_alloc for checking lowest tn later on */
        stored_rel_ts_alloc = rel_ts_alloc;

        if (!rel_nts)
        {
          /* mac should not of sent PDCH_REL_TS as it should
           * send an abnormal release
           */
          MSG_GERAN_ERROR_0_G("PL1: TRANSFER MAC_ERROR");
          GL1_ISR_SAVE_UNLOCK(gas_id);
          return;
        }

        MSG_GERAN_MED_2_G(" dl_ts_alloc 0x%X dl_nts %d ",dl_ts_alloc,dl_nts);
        MSG_GERAN_MED_2_G(" rel_ts_alloc 0x%X rel_nts %d ",rel_ts_alloc,rel_nts);

        for (i=0;i< MAX_TN ;i++)
        {
          /* Preset pending usf to invalid */
          pending_usf[i] = GPL1_INVALID;

          /* check if dynamic timeslot is allocated and setup alloc_type struct */
          if (dynamic_tbf_present)
          {
            if( NULL != usf_value )
            {
            if (usf_value[i] != GPL1_INVALID)
            {
              if (!(rel_ts_alloc & 0x1))
              {
                /* ts is being released */
                dynamic_ts_changed = TRUE;
                /* If an UL TS is released, delete the RRBPs assigned to that TS               */
                /* ------------------ Refer to 3GPP 44.060 -> Section 8.2 ------------------   */
                /* When a mobile station receives a PACKET PDCH RELEASE message,               */
                /* it shall immediately stop transmitting and receiving on all assigned PDCHs, */
                /* which are indicated as not present in the TIMESLOTS_AVAILABLE field,        */
                /* remove those PDCHs from its list of assigned PDCHs.                         */

                rrbp_index = gpl1_gprs_get_matching_rrbp(GPL1_INVALID_FN, GPL1_INVALID_FN, i, gas_id);

                if(rrbp_index < SIZE_OF_RRBP_BUFF )
                {
                  gpl1_gprs_delete_rrbp_entry(rrbp_index,gas_id);
                }

                /* dynamic allocated tn is being deallocated */
                if (--dynamic_nts == 0)
                {
                  /* no dynamic allocated tn's left so release dynamic tbf */
                  MSG_GERAN_ERROR_0_G("NO DYNAMIC allocated TNs left");
                }

              } else
              {
                /* timeslot not being released so copy current usf into pending store */
                pending_usf[i] = usf_value[i];
              }

            }  /* End of if(usf_value .. */
            } /* End of if usf_value is not NULL */
            else
            {
              MSG_GERAN_ERROR_0_G(" NULL usf_value ptr ");
            }
          }  /* End of if (dynmaic_tbf_present) */



          if ( (dl_tbf_present) && (dl_ts_alloc & 0x1) )
          {
            if (!(rel_ts_alloc & 0x1))
            {
              /* timeslot is being released */
              dl_ts_changed = TRUE;

              if (--dl_nts == 0)
              {
                /* no allocated dl timeslots left so release downlink tbf */
                MSG_GERAN_ERROR_0_G("NO DL allocated TNs left");
              } else
              {
                /* store new un-allocation tn  in pending store */
                //pending_dl_ts_alloc &= (0 <<i);
              }
            } else
            {
              /* timeslot not being released so copy dl tn into pending store */
              pending_dl_ts_alloc |= ((dl_ts_alloc & 0x1)<<i);
            }

          }

          /* Move onto next timeslot */
          dl_ts_alloc >>=1;
          rel_ts_alloc >>=1;

        } /* End of for loop */

        MSG_GERAN_MED_1_G(" pending_dl_ts_alloc 0x%X ",pending_dl_ts_alloc);

        
        switch(l1_transfer_data[gas_id].release_pending)
        {
          case L1_FULL_RELEASE_UL:
          {
            if(!gpl1_gprs_get_ul_num_rrbp_msg(gas_id) && (l1_tskisr_blk->main_command!= L1_IDLE_COMMAND))
            {
              go_to_idle = TRUE;
            }
          }
          break;
        
          case L1_FULL_RELEASE_DL:
          {
            gpl1_gprs_calc_num_ab_dl_rrbp(gas_id);
            if(!gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id) && (l1_tskisr_blk->main_command!= L1_IDLE_COMMAND))
            {
              go_to_idle = TRUE;
            }
          }
          break;
          case L1_FULL_RELEASE_UL_DL:
          {
             gpl1_gprs_calc_num_ab_dl_rrbp(gas_id);
             if(!(gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id)+gpl1_gprs_get_ul_num_rrbp_msg(gas_id)) && (l1_tskisr_blk->main_command!= L1_IDLE_COMMAND))
             {
                go_to_idle = TRUE;
             }
          }
          break;
          default:
          MSG_GERAN_LOW_1_G(" Do Nothing %d ",l1_transfer_data[gas_id].release_pending);
          break;
        }
        
        if(go_to_idle)
        {
          MSG_GERAN_MED_0_G(" Going to Idle ");
#ifdef FEATURE_GSM_DTM
          /* Release is Waiting for RRBP. If RRBP is deleted , start Release process.*/
          if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
#endif
          {
             l1_sc_stop(gas_id);
          }
          gpl1_gprs_setup_start_idle((void *)0,gas_id);
          GL1_ISR_SAVE_UNLOCK(gas_id);
          return;
        }

        if (dynamic_ts_changed)
        {
          /*  work out which dynmaic structure to copy new configuration into */
          if (l1_transfer_data[gas_id].dynamic_tbf_ptr == &l1_transfer_data[gas_id].dynamic_tbf_1)
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;
          } else
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
          }

          if (l1_transfer_data[gas_id].disable_rx_tx)
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time = l1_transfer_data[gas_id].dynamic_tbf_ptr->starting_time;
          }

          /* update new number of assigned timeslots */
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->nts = dynamic_nts;
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->usf_granularity =
          l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_granularity;

          /* copy new allocated timeslots into the pending dynamic structure */
          memscpy(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->usf_value,
                 sizeof(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->usf_value),
                 pending_usf,
                 sizeof(pending_usf) );

          gpl1_gprs_reset_pending_ta_params(gas_id);

          /* This message does not carry any EGPRS quality
           * parameters for use by the scell measurements
           * module, hence set them to invalid in whichever
           * structure the pending ptr is set to.
           */
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->bep_period2 =
          GPRS_PL1_BEP_PERIOD2_INVALID;

           l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->test_mode =
              TEST_MODE_OFF;

          /* Set the mac mode for the benefit of the
           * reconfiguration routine.
           */
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode =
              L1_MAC_MODE_INVALID;
        }

        if (dl_ts_changed)
        {
          /* only timeslots will change so copy current dynamic structure
           * into pending one
           */
          if (l1_transfer_data[gas_id].dl_tbf_ptr == &l1_transfer_data[gas_id].dl_tbf_2)
          {
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;
          } else
          {
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
          }


          if (l1_transfer_data[gas_id].disable_rx_tx)
          {
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = TRUE;
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time = l1_transfer_data[gas_id].dl_tbf_ptr->tbf_start_time;
          }
          /* update new number of assigned timeslots */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr->nts = dl_nts;

          /* copy new allocated timeslots into the pending dynamic structure */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr->ts_allocation = pending_dl_ts_alloc;

          gpl1_gprs_reset_pending_ta_params(gas_id);

          if (l1_transfer_data[gas_id].dl_tbf_ptr->dl_tfi_present)
          {
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->dl_tfi_present=TRUE;
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->dl_tfi = l1_transfer_data[gas_id].dl_tbf_ptr->dl_tfi;
          } else
          {
            MSG_GERAN_ERROR_0_G("No DL TFI");
          }

          /* This message does not carry any EGPRS quality
           * parameters for use by the scell measurements
           * module, hence set them to invalid in whichever
           * structure the pending ptr is set to.
           */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr->link_qual_meas_mode =
          INVALID_MEAS_MODE;

          l1_transfer_data[gas_id].pending_dl_tbf_ptr->bep_period2 =
          GPRS_PL1_BEP_PERIOD2_INVALID;

          l1_transfer_data[gas_id].pending_dl_tbf_ptr->rlc_mode =
              RLC_MODE_INVALID;

          if (l1_transfer_data[gas_id].pending_dynamic_tbf_ptr != NULL)
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->test_mode =
                TEST_MODE_OFF;
          }

          /* Set the mac mode for the benefit of the
           * reconfiguration routine.
           */
          if (l1_transfer_data[gas_id].pending_dynamic_tbf_ptr != NULL)
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode =
                                                  L1_MAC_MODE_INVALID;
          }

          /* Set the mac in the pending downlink tbf structure
           * to invalid.
           */
          l1_transfer_data[gas_id].pending_dl_tbf_ptr->mac_mode = L1_MAC_MODE_INVALID;
        }

        /* If either dynamic or downlink require new allocation
         * changes then reconfigure  otherwise do nothing
         * ensure there is no reconfigure pending!!
         */
        if (( (dl_ts_changed) ||
              (dynamic_ts_changed) ) &&
            (l1_tskisr_blk->main_command != L1_TRANSFER_COMMAND))
        {
          if (gpl1_gprs_lowest_tn_changed( stored_rel_ts_alloc, l1_get_current_timeslot(gas_id) ) )
          {
            if (dl_ts_changed)
            {
              l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = TRUE;
              MSG_GERAN_HIGH_0_G("dl ts changed");
            } else if (dynamic_ts_changed)
            {
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;
              MSG_GERAN_HIGH_0_G("dyn ts changed");
            }

            /* Check if there is an RRBP within the next 6 frames */
            starting_time_present = gpl1_gprs_transfer_find_rrbp( ((gl1_get_FN(gas_id) + 6) % FRAMES_IN_HYPERFRAME),
                                                                  &starting_time, gas_id);

            if (starting_time_present == FALSE)
            {
              starting_time_present = TRUE;
            
              /* Improve the reaction time of L1
               * Only account for the task to ISR context switch and hence add 1 tdma frame.
               */
              starting_time  = ADD_FN( gl1_get_FN( gas_id ), 2 );

              MSG_GERAN_MED_1_G("ST made up %d",starting_time);
            
            }
          }/* End of if lowest_tn_changed */

          switch( l1_transfer_data[gas_id].release_pending )
          {
          case  L1_PARTIAL_RELEASE_UL:

            /* If there is a pending UL release then delay actioning
             * the PDCH release so it happens when the UL release is
             * confirmed.
             */
            if(  ( L1_DYNAMIC_DOWN_ALLOC == l1_transfer_data[gas_id].current_alloc_type )
               &&( L1_DOWNLINK_ALLOC == l1_transfer_data[gas_id].pending_alloc_type )
              )
            {
              starting_time_present = FALSE;
            }
            break;

          case  L1_NO_RELEASE:
            /* If we are waiting on a release of a tbf wait for it before
             * releasing the TS
             */
            l1_transfer_data[gas_id].pending_alloc_type = l1_transfer_data[gas_id].current_alloc_type;

            break;

          default:
            /* Don't do anything here, the case statements can be
             * expanded to include any potential future problems.
             * These could potentially include partial dl release
             * full uplink release and full downlink release.
             */
            break;
          }

          /* if there is an RRBP in the way of our ts release then wait until RRBP
           * has been txd
           */
          if (starting_time_present)
          {
            l1_transfer_data[gas_id].starting_time_valid = TRUE;

            /* work out which structure to write starting time to */
            if (dl_ts_changed)
            {
              (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time,
                                                starting_time,
                                                PCCCH_52, gas_id);

              MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,
              l1_transfer_data[gas_id].pending_dl_tbf_ptr,l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap,l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time);
              if(l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap)
              {
                (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time,2, gas_id);
                gpl1_g2x_ta_scheduler(gas_id);
              }

            } else if (dynamic_ts_changed)
            {
              (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time,
                                                starting_time,
                                                PCCCH_52, gas_id);

              MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time);
              if(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap)
              {
                (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time,2, gas_id);
                gpl1_g2x_ta_scheduler(gas_id);
              }

            }

            MSG_GERAN_MED_2_G(" ST present %d [%d]",l1_transfer_data[gas_id].starting_time,gl1_get_FN( gas_id ));
          } else
          {
            /* no starting time */
            l1_transfer_data[gas_id].starting_time_valid =FALSE;
          }

          /* set starting time elapsed to true as we do not want the MAC_PH_CONNECT_IND
            * to be sent when we restart transfer_mode
            */
          l1_transfer_data[gas_id].starting_time_elapsed  = TRUE;

          if (l1_transfer_data[gas_id].release_pending == L1_NO_RELEASE)
          {
            /* If we are waiting on a release of a tbf wait for it before
             * releasing the TS
             */
#ifdef FEATURE_GSM_DTM
            /* Don't set main commands in the DTM mode */
            if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
            {
              /* signal to L1 ISR that a TBF reconfig is pending */
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
            } else
            {
              l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG, gas_id);
            }
#else
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif
          }
        }

        GL1_ISR_SAVE_UNLOCK(gas_id);
        break;
      }
      /*******************************************************
      *                                                       *
      *                MAC_L1_RELEASE                         *
      *                                                       *
      ********************************************************/
    case MAC_L1_RELEASE:
      {
        mac_l1_release_t  *message_ptr;
        boolean  return_to_idle = FALSE;
        uint8  wait_for_isr_count = 0;
        volatile ISRTIM_CMD_BLK* l1_tskisr_blk;

        message_ptr = (mac_l1_release_t *)msg_header;

        gas_id = message_ptr->gas_id;
        

#ifdef FEATURE_GSM_TDS
        if( l1_sc_get_tds_irat_active(gas_id) )
        {
          /* CR375720: ISR has higher priority than task, if GL1 is in the
          idle frame then mdsp_decrement_cleanup_time_offset_flag is set to FALSE, no need to delay TBF release*/
          if(mdsp_decrement_cleanup_time_offset_flag[gas_id])
          {
            if ( (l1_g2t_delay_tbf_rel == UL_TBF_NORMAL)&&((( mac_l1_release_t *) msg_header)->release_tbf == DL_TBF_NORMAL))
            {
              l1_g2t_delay_tbf_rel = (int) UL_AND_DL_TBF_NORMAL;
            }
            else if ( (l1_g2t_delay_tbf_rel == DL_TBF_NORMAL)&&((( mac_l1_release_t *) msg_header)->release_tbf == UL_TBF_NORMAL))
            {
              l1_g2t_delay_tbf_rel = (int) UL_AND_DL_TBF_NORMAL;
            }
            else
            {
            l1_g2t_delay_tbf_rel = (int)((( mac_l1_release_t *) msg_header)->release_tbf);
            }
            MSG_GERAN_HIGH_1_G("TDS: GL1 postpones TBF release l1_g2t_delay_tbf_rel %d", l1_g2t_delay_tbf_rel);
            break;
          }
        }
#endif

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
        
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        if(l1_tskisr_blk->sub_state == L1_TRANSFER_INIT)
        {
         MSG_GERAN_HIGH_0_G("Delay by one frame");
         GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
         (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
         (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
         GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
        }

        while(( gpl1_g2x_ta_get_act_status(G2X_TA_PRI_TBF_TS_CHANGE, gas_id) == ONGOING )
                && (	wait_for_isr_count <= 4 ))
        {
          GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
          (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
          (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
		
          MSG_GERAN_HIGH_1_G(" wait for isr to get FN, count %d ", wait_for_isr_count );
		
          wait_for_isr_count++;
		
          GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
        }
        /* release the aprropriate TBF and inform the ISR to
         * swap over to the new tbf parameters
         */
#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
        if((l1_tskisr_blk->command == L1_DEDICATED_COMMAND) && (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE))
        {
          gpl1_gprs_handle_edtm_release(gas_id);
          return_to_idle = TRUE;
        }
       else
#endif
        {
           return_to_idle = gpl1_gprs_handle_tbf_release( (( mac_l1_release_t *) msg_header)->release_tbf, message_ptr->gas_id );
        }
 
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TBF_RELEASE);
#endif

        /*---------------------------------------------------------------------------
           Return to idle without the interrupts being locked as the functions within
           are CPU intensive.
        -----------------------------------------------------------------------------*/
        if( FALSE != return_to_idle )
        {
          gpl1_gprs_setup_start_idle((void *)0, message_ptr->gas_id);
        }

        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
        break;

      }

      /*******************************************************
      *                                                       *
      *                MAC_PH_DATA_REQ                        *
      *                                                       *
      ********************************************************/
    case MAC_PH_DATA_REQ:
      {
        mac_ph_data_req_t  *message_ptr;
        mac_ul_ctrl_buffer_T *mac_data_ptr;
       /*lint -e740 ccww legacy code */
        message_ptr = ( mac_ph_data_req_t *) msg_header;
        /*lint +e740 */
        gas_id = message_ptr->gas_id;

        MSG_GERAN_MED_0_G("PL1: MAC_PH_DATA_REQ Rcvd");

        if( L1_TEST_MODE_SRB_ON == l1_transfer_data[gas_id].l1_test_mode )
        {
          /* ETSI 04.14: When in Radio Block Loopback Sub-mode ON,
           * MS should not send any control messages or data from
           * RLC to its own physical Link layer for transmission.
           */
          break;
        }

        mac_data_ptr = l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr;


        /* check there is space to copy new message into L1 buffer */
        if (( mac_data_ptr->pending_msgs + mac_data_ptr->sched_msgs) < MAX_NUM_UL_PACCH_MSGS )
        {

          /* Copy PACCH message into L1 buffer */
          memscpy(
                mac_data_ptr->pacch_data_in_ptr->control_block,
                sizeof(mac_data_ptr->pacch_data_in_ptr->control_block),
                message_ptr->dl_ctrl_block,
                SIZE_OF_CONTROL_RADIO_BLOCK
                );

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
          /* Set the valid boolean */
          mac_data_ptr->pacch_data_in_ptr->valid = TRUE;
#endif

          /*
           * Increment in pointer for next access.
           * Check if end of buffer has been reached,
           * and set to first element if it has
           */
          if ( ++(mac_data_ptr->pacch_data_in_ptr) > &(mac_data_ptr->pacch_data[MAX_NUM_UL_PACCH_MSGS -1]) )
          {
            mac_data_ptr->pacch_data_in_ptr = mac_data_ptr->pacch_data;
          }

          /* Data now available in L1 buffer for transmission */
          mac_data_ptr->data_valid = TRUE;

          /* Increment the number of messages */
          mac_data_ptr->pending_msgs++;

          /* PACCH UL CONTROL available */
          l1_transfer_data[gas_id].mac_ul_ctrl_available = TRUE;
        } else
        {
          MSG_GERAN_ERROR_0_G("PL1: ERROR MAC UL PACCH BUFFER OVERFLOW");
        }



        break;
      }

      /*******************************************************
      *                                                      *
      *                MAC_L1_QUALITY_RPT                    *
      *                                                      *
      ********************************************************/
    case MAC_L1_QUALITY_RPT:
      {
        mac_l1_quality_rpt_t  *message_ptr;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;

        message_ptr = (mac_l1_quality_rpt_t *)msg_header;
        gas_id = message_ptr->gas_id;
        gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[message_ptr->gas_id];

        MSG_GERAN_LOW_2_G("PL1: MAC_L1_QUALITY_RPT Rcvd %d %d ",gl1_get_FN( message_ptr->gas_id ),message_ptr->rrbp_value);

        if (message_ptr->rrbp_value != L1_GPRS_INVALID_FN)
        {
          /*  Set up quality reporting time to be starting time */
          if ((uint32)gpl1_gprs_adjust_mod_num(message_ptr->rrbp_value, -(int32)gl1_get_FN( message_ptr->gas_id ),
                                               FRAMES_IN_HYPERFRAME) < (uint32)8)
          {
            /* Increment the number of quality reports to send */
            gprs_serv_cell_meas_ptr->quality_report_count++;

            /*  Indicate associated rrbp_fn in response */
            gprs_serv_cell_meas_ptr->qrt = message_ptr->rrbp_value;

            /*  Sent quality report immediately */
            gpl1_gprs_build_quality_report(message_ptr->gas_id);

          } else
          {
            gprs_serv_cell_meas_ptr->quality_report_time[gprs_serv_cell_meas_ptr->quality_report_time_index++] = message_ptr->rrbp_value;
            if (gprs_serv_cell_meas_ptr->quality_report_time_index > (MAX_NUM_QUAL_RPRTS - 1))
            {
              gprs_serv_cell_meas_ptr->quality_report_time_index = 0;
            }
          }

        } else
        {
          /* Increment the number of quality reports to send */
          gprs_serv_cell_meas_ptr->quality_report_count++;

          /*  Indicate associated rrbp_fn in response */
          gprs_serv_cell_meas_ptr->qrt = message_ptr->rrbp_value;

          /*  Sent quality report immediately */
          gpl1_gprs_build_quality_report(message_ptr->gas_id);

        }
        break;
      }


#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

          /* @@OG: EDTM design */
      /******************************************************
      * MAC_L1_DTM_CHANNEL_ASSIGN_REQ                       *
      * -----------------------------                       *
      * Provides L1 with the DTM assignment alloactions.    *
      * It may contain and UL and or DL alloaction.         *
      *******************************************************/
      case MAC_L1_DTM_CHANNEL_ASSIGN_REQ:
      {
        mac_l1_dtm_channel_assign_req_t* const  message_ptr       = ( mac_l1_dtm_channel_assign_req_t *) msg_header;
        gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(message_ptr->gas_id);
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[message_ptr->gas_id];
        idle_data_T               *l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[message_ptr->gas_id];
        const gprs_l1_cell_opt_t*               msg_cell_opts_ptr = &( message_ptr->gprs_l1_cell_opt );
        uint32 current_fn = gl1_get_FN( message_ptr->gas_id );
        grm_client_enum_t g_client;
        boolean  set_main_cmd = FALSE;

        gas_id = message_ptr->gas_id;
        MSG_GERAN_HIGH_2_G( "xfer MAC_L1_DTM_CHANNEL_ASSIGN_REQ CS present:%d tbf %u ",
                  message_ptr->cs_present,
                  message_ptr->dtm_tbf_type);
        g_client = gl1_map_gas_id_to_client_id(gas_id);
        gpl1_set_dtm_assig_status(TRUE, gas_id);
        /* read and use the cell options structure */
        gprs_l1_idle_data->gprs_campedon_cell_data.power_control.alpha        = msg_cell_opts_ptr->alpha;
        gprs_l1_idle_data->gprs_campedon_cell_data.power_control.t_avg_w      = msg_cell_opts_ptr->t_avg_w;
        gprs_l1_idle_data->gprs_campedon_cell_data.power_control.t_avg_t      = msg_cell_opts_ptr->t_avg_t;
        gprs_l1_idle_data->gprs_campedon_cell_data.power_control.pc_meas_chan = msg_cell_opts_ptr->pc_meas_chan;
        gprs_l1_idle_data->gprs_campedon_cell_data.power_control.n_avg_i      = msg_cell_opts_ptr->n_avg_i;
        gprs_l1_idle_data->gprs_campedon_cell_data.gprs_ms_txpwr_max_cch      = msg_cell_opts_ptr->gprs_ms_txpwr_max_cch;
        gprs_serv_cell_meas_ptr->pmax                                             = msg_cell_opts_ptr->gprs_ms_txpwr_max_cch;

        l1_idle_data->layer1_extra_gprs_sys_info.access_burst_type  = msg_cell_opts_ptr->access_burst_type;
        gprs_l1_idle_data->gprs_campedon_cell_data.drx_timer_max    = msg_cell_opts_ptr->drx_timer_max;

        /* Read the broadcast bep period from the gprs options structure and
         * use it to configure the egprs quality measurements module.
         * This is really vital in W2G tch handover cases where the MS starts up in
         * W and the EGPRS downlink assignment does not contain a valid bep period2.
         */
        gpl1_egprs_scell_qual_meas_broadcast_config( msg_cell_opts_ptr->bep_period, message_ptr->gas_id );

        if( FALSE != message_ptr->cs_present )
        {
          /* temp place holder for TX PWR to write Eng Mode Info */
          int16 tmp_tx_pwr[8] = {0};

          /* Setup a destination pointer
           * @@TODO: To follow up with whats done in rach_if, the dedic configuration should
           * ideally be copied to the "channel_specification" instead of the new_channel_specification.
           */
          channel_spec_T* const  dedic_ptr = &( l1_dedicated_data_ptr->new_channel_specification );

          /* Setup a source pointer to the current transfer freq information */
          const frequency_information_T*  xfer_freq_ptr = &(l1_transfer_data[gas_id].frequency_info_1);

          /* Copy the dedicated data and set the command (not main command)
           * @@OG: EDTM design: Is this really correct? we need the freq info of the message
           * so this assumes we got a frequency config before dtm chan assign req.
           * Make sure we are picking the correct dtm frequency config parameters.
           */
          dedic_ptr->channel_info_1_after.frequency_list = xfer_freq_ptr->frequency_list;
          dedic_ptr->channel_info_1_after.hopping_flag   = xfer_freq_ptr->hopping_flag;
          dedic_ptr->channel_info_1_after.HSN            = xfer_freq_ptr->hsn;
          dedic_ptr->channel_info_1_after.MAIO           = xfer_freq_ptr->maio;
          dedic_ptr->channel_info_1_after.TSC            = xfer_freq_ptr->tsc;

          dedic_ptr->channel_info_1_after.subchannel   = message_ptr->cs_desc.cs_chan_info.subchannel;
          dedic_ptr->channel_info_1_after.channel_type = message_ptr->cs_desc.cs_chan_info.channel_type;
          dedic_ptr->channel_info_1_after.TN           = message_ptr->cs_desc.cs_chan_info.TN;

          //dedic_ptr->channel_info_1_after.

          dedic_ptr->channel_mode_1          = message_ptr->cs_desc.channel_mode;
          dedic_ptr->channel_mode_2          = message_ptr->cs_desc.channel_mode;

          switch( message_ptr->cs_desc.channel_mode )
          {
          case SIGNALLING_ONLY:

            MSG_GERAN_HIGH_0_G(" chan mode SIGNALLING_ONLY ");
            break;

          case SPEECH_V1:
            MSG_GERAN_HIGH_0_G(" chan mode SPEECH_V1 ");
            break;

          case SPEECH_V2:
            MSG_GERAN_HIGH_0_G(" chan mode SPEECH_V2 ");
            break;

          case SPEECH_V3:
            MSG_GERAN_HIGH_0_G(" chan mode SPEECH_V3 ");
            break;

          case DATA_9600:
            MSG_GERAN_HIGH_0_G(" chan mode DATA_9600 ");
            break;

          case DATA_4800:
            MSG_GERAN_HIGH_0_G(" chan mode DATA_4800 ");
            break;

          case DATA_2400:
            MSG_GERAN_HIGH_0_G(" chan mode DATA_2400 ");
            break;

          case DATA_14400:
            MSG_GERAN_HIGH_0_G(" chan mode DATA_14400 ");
            break;

          default:
            MSG_GERAN_ERROR_1_G(" Unknown channel mode %u ",message_ptr->cs_desc.channel_mode);
            break;
          }

          /* Copy channel info 1 into 2 */
          dedic_ptr->channel_info_2_after = dedic_ptr->channel_info_1_after;

          dedic_ptr->DTX_indicator           = message_ptr->cs_desc.DTX_indicator;
          dedic_ptr->power_level             = message_ptr->cs_desc.power_level;

          dedic_ptr->before_parameters_valid = FALSE;
          dedic_ptr->starting_time_valid     = FALSE;

          dedic_ptr->cipher_flag      = l1_dedicated_data_ptr->channel_specification.cipher_flag;
          dedic_ptr->cipher_algorithm = l1_dedicated_data_ptr->channel_specification.cipher_algorithm;

          /* Currently not present in message
           * Make sure all variables in the msg are copied.
           */
          dedic_ptr->number_of_ded_channels = 0;

          dedic_ptr->starting_time_valid     = FALSE;
          dedic_ptr->before_parameters_valid = FALSE;

          l1_dedicated_data_ptr->previous_chnl_type = SDCCH;

          /* Set all the possible transmit level variables */
          l1_dedicated_data_ptr->ms_power_level_in_use  = message_ptr->cs_desc.power_level;

          tmp_tx_pwr[0] = l1_dedicated_data_ptr->ms_power_level_in_use;
          geran_eng_mode_data_write(ENG_MODE_TX_PWR, tmp_tx_pwr, TRUE, message_ptr->gas_id);

          l1_dedicated_data_ptr->ms_power_level_to_use  = message_ptr->cs_desc.power_level;
          l1_dedicated_data_ptr->ordered_ms_power_level = message_ptr->cs_desc.power_level;

          l1_dedicated_data_ptr->freq_redef_imminent = FALSE;

          l1_dedicated_data_ptr->timing_advance_in_use  = gl1_msg_retrieve_ta(message_ptr->gas_id);
          geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&(l1_dedicated_data_ptr->timing_advance_in_use),TRUE, message_ptr->gas_id);
          l1_dedicated_data_ptr->timing_advance_to_use  = l1_dedicated_data_ptr->timing_advance_in_use;
          l1_dedicated_data_ptr->ordered_timing_advance = l1_dedicated_data_ptr->timing_advance_in_use;

#ifdef FEATURE_GSM_AMR

          l1_dedicated_data_ptr->new_multirate_configuration = message_ptr->cs_desc.multi_rate_config;

#endif

          l1_dedicated_data_ptr->L1_confirm_msg              = (byte) MPH_DTM_CHANNEL_ASSIGN_CNF;

          /* Reset the l1 tx data buffers */
          l1_dcch_s0_buffer[message_ptr->gas_id].data_valid = FALSE;
          l1_sacch_s0_buffer[message_ptr->gas_id].data_valid = FALSE;

          /* The following is a cut and paste source code for handling the immediate
           * assignment request. This is to make sure we have not forgotten to set / init
           * any parameter.
           */
#if 0
          //L1_remove_oob_ba(&ba_buffer,&l1_idle_data.campedon_cell_data.BA_list);
          //l1_sc_set_ba_list(&ba_buffer, 0xFF);
#endif
        } /* End of if cs present is TRUE ) */
        else
        {
          MSG_GERAN_ERROR_0_G(" DTM CHAN ASSIGN without cs present ");

          /* Exit function */
        }

        /* The SCE needs to be stopped to make this transition to the DTM
         * @@WARNING: Maybe its better to do this in the ISR ???
         */
        l1_sc_stop(message_ptr->gas_id);

        /* Lock interrupts to perform atomic processes */
        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);

        if( ( l1_transfer_data[gas_id].release_pending == L1_NO_RELEASE )
          &&( l1_transfer_data[gas_id].starting_time_valid != TRUE )
          )
        {
          /* setup pending allocation type to current allocation type */
          l1_transfer_data[gas_id].pending_alloc_type = l1_transfer_data[gas_id].current_alloc_type;
        }

        if(( message_ptr->dtm_tbf_type == NO_TBF_PRESENT ) ||
           ( message_ptr->dtm_tbf_type == UL_TBF_PRESENT ) ||
           ( message_ptr->dtm_tbf_type == DL_TBF_PRESENT ) ||
           ( message_ptr->dtm_tbf_type == UL_DL_TBF_PRESENT )
          )
        {
          /* Handle the case of no TBF present seperately.
           * Because we need to keep the current TBF configuration
           * alone.
           * Double check what needs to happen pending releases.
           */
          if( NO_TBF_PRESENT == message_ptr->dtm_tbf_type )
          {
            /* Remove any pending DL/UL TBF assignments */
            if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
            {
               if( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].dl_st )
               {
                 l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
               }

               if( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].ul_st )
               {
                 l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
               }

               /* Null the main command */
               l1_tskisr_blk->main_command = L1_NULL_COMMAND;

               l1_transfer_data[gas_id].starting_time_valid   = FALSE;
               l1_transfer_data[gas_id].starting_time_elapsed = TRUE;

               /* This is a major event so print an F3 message */
               MSG_GERAN_ERROR_0_G(" Removing pending assignments ");
            }

            if( L1_NO_RELEASE != l1_transfer_data[gas_id].release_pending )
            {
              l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
            }

            /* Change pending alloc to be same as current */
            l1_transfer_data[gas_id].pending_alloc_type = l1_transfer_data[gas_id].current_alloc_type;
          }

          /* Store the TBF parameters depending upon the type of TBF */

          /* Is downlink TBF present ? */
          if( FALSE != GPL1_IS_DL_TBF_PRESENT[ message_ptr->dtm_tbf_type ] )
          {
            mac_downlink_tbf_t*          dl_tbf_config_ptr =  &( message_ptr->dl_tbf );
            downlink_tbf_information_T*  dl_msg_dest_ptr   = NULL;

            uint8  l1_dl_timeslot_alloc = 0;
            uint8  bkup_sub_state       = 0;
            uint8  bkup_sub_sub_state   = 0;

            transfer_allocation_T  bkup_current_alloc_type = L1_NULL_ALLOC;
            transfer_allocation_T  bkup_pending_alloc_type = L1_NULL_ALLOC;

            /***************************************************************/
            /* @@WARNING: Check for any pending DL TBF release and stop it */
            /***************************************************************/

            /* stop T3192 as we have just received a PDA
             * GSM 04.60 sec 13.1
             */
            grlc_dl_stop_t3192(message_ptr->gas_id);
            MSG_GERAN_HIGH_0_G("PL1: stop T3192");

            /* @@OG: EDTM Design
             * Probably need to remove queued ul tbf at this point?
             */
            if(  ( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
               &&( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].ul_st ) )
            {
              MSG_GERAN_MED_0_G("PUA IN PROGRESS");

              /*  Temporarily change some parameters, as PDA setup before PUA starting
                  time has elapsed but PDA should be setup based on PUA being active. */
              /*  Parameters to be changed:- sub_state, sub_sub_state in l1_isrtsk_blk.
                  current_alloc_type and pending_alloc_type in l1_transfer_data[gas_id].  */
              bkup_sub_state          = l1_tskisr_blk->sub_state;
              bkup_sub_sub_state      = l1_tskisr_blk->sub_sub_state;
              bkup_current_alloc_type = l1_transfer_data[gas_id].current_alloc_type;
              bkup_pending_alloc_type = l1_transfer_data[gas_id].pending_alloc_type;

              l1_tskisr_blk->sub_state            = L1_DYNAMIC_2;
              l1_tskisr_blk->sub_sub_state        = L1_DL_1;
              l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
              l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
            }

            /* Based on the current sub state work out where to
             * copy new data to
             */
            switch (l1_tskisr_blk->sub_state)
            {
            case L1_DYNAMIC_1:
            case L1_DYNAMIC_2:

              if (l1_tskisr_blk->sub_sub_state == L1_DL_1)
              {
                /* Reconfigure downlink as part of concurrent tbf */
                dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

                /* Setup pending downlink structure */
                l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

              } else if ( (l1_tskisr_blk->sub_sub_state == L1_DL_2) ||
                          (l1_tskisr_blk->sub_sub_state == L1_NO_DL) )
              {
                /* new downlink with dynamic allocation or
                 * reconfigure of downlink part or concurrent tbf */
                dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

                /* Setup pending downlink structure */
                l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;
              }

              /* added by ADG */
              if (l1_transfer_data[gas_id].release_pending == L1_PARTIAL_RELEASE_UL)
              {
                l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
              } else if (l1_transfer_data[gas_id].release_pending == L1_FULL_RELEASE_UL)
              {
                l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
                l1_transfer_data[gas_id].release_pending = L1_PARTIAL_RELEASE_UL;

              } else
              {
                /* set up pending allocation type */
                l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
              }

              break;

            case L1_DOWNLINK_1:

              /* Reconfigure downlink tbf, no concurrency */
              dl_msg_dest_ptr = &l1_transfer_data[gas_id].dl_tbf_2;

              /* Setup pending downlink structure and alloc type */
              l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_2;
              l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
              break;

            case L1_DOWNLINK_2:

              /* reconfigure downlink tbf, no concurrency, set pending ptr */
              dl_msg_dest_ptr                     = &l1_transfer_data[gas_id].dl_tbf_1;
              l1_transfer_data[gas_id].pending_dl_tbf_ptr = &l1_transfer_data[gas_id].dl_tbf_1;

              if (l1_transfer_data[gas_id].ul_st == L1_GPRS_INVALID_FN)
              {
                /* set up pending allocation type */
                l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
              }
              break;

            default:
              /* something has gone wrong report error */
              MSG_GERAN_ERROR_0_G("PL1: Transfer Bad Sub_state");
              break;
            } /* End of switch */

            if (!dl_msg_dest_ptr)
            {
              GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
              /* something has gone wrong report error and return */
              MSG_GERAN_ERROR_0_G("PL1: Transfer Bad Sub_sub_state");
              return;
            }

            /* Ignore the downlink assignment mac mode (Rel 6) */
            dl_msg_dest_ptr->mac_mode = L1_MAC_MODE_INVALID;

            /* store new tn allocation in pending structure */
            dl_msg_dest_ptr->nts = gpl1_gprs_reorder_tn( &dl_tbf_config_ptr->ts_allocation,
                                                         &dl_msg_dest_ptr->ts_allocation);
            /* Copy in the rlc mode */
            l1_transfer_data[gas_id].pending_dl_tbf_ptr->rlc_mode = dl_tbf_config_ptr->rlc_mode;

            l1_transfer_data[gas_id].pending_dl_tbf_ptr->link_qual_meas_mode =
              dl_tbf_config_ptr->link_qual_meas_mode;

            if ( FALSE != dl_tbf_config_ptr->bep_period2.present )
            {
              l1_transfer_data[gas_id].pending_dl_tbf_ptr->bep_period2 =
                dl_tbf_config_ptr->bep_period2.bep_period2;
            }
            else
            {
              l1_transfer_data[gas_id].pending_dl_tbf_ptr->bep_period2 =
              GPRS_PL1_BEP_PERIOD2_INVALID;
            }

            l1_dl_timeslot_alloc = dl_msg_dest_ptr->ts_allocation;

            /* No need to consider the currently allocated uplink timeslots
             * since the uplink TBF is to be removed or replaced anyways.
             */
            /* Check if there is a PCA or PDAN w FAI or PDAN w CRD RRBP */
            if (!dl_tbf_config_ptr->tbf_start_time_valid)
            {
              /* @@TODO: This test should ideally check the DL related RRBP messages only. */
              dl_tbf_config_ptr->tbf_start_time_valid =
                 gpl1_gprs_transfer_find_rrbp_type(&dl_tbf_config_ptr->tbf_start_time,message_ptr->gas_id);
            }

            /* @@WARNING: The handling of lowest timeslot calculations here needs to change
             * since the DTM chan assign is not handled as an addition to the ones previous.
             * Hence only consider the contents of the message and be wary of the CS timeslot.
             * @@TODO this still needs to be done.
             */

            /* check if lowest numbered tn has changed */
            if (gpl1_gprs_lowest_tn_changed( l1_dl_timeslot_alloc , l1_get_current_timeslot(message_ptr->gas_id) ) )
            {
              l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = TRUE;

              /* Check if there is a PCA or PDAN w FAI or PDAN w CRD RRBP */
              if (!dl_tbf_config_ptr->tbf_start_time_valid)
              {
                /* @@TODO: This test should ideally check the DL related RRBP messages only. */
                dl_tbf_config_ptr->tbf_start_time_valid =
                  gpl1_gprs_transfer_find_rrbp_type(&dl_tbf_config_ptr->tbf_start_time,message_ptr->gas_id);
              }
            }
            else
            {
              /* Is the TBF currently suspended ? */
              if ( (l1_transfer_data[gas_id].disable_rx_tx) && (l1_transfer_data[gas_id].dl_tbf_ptr != NULL) )
              {
                /* If we are currently suspended, don't overwrite th gap or st var's */
                l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap =
                  l1_transfer_data[gas_id].dl_tbf_ptr->allow_one_rb_gap;

                l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time =
                  l1_transfer_data[gas_id].dl_tbf_ptr->tbf_start_time;
              }
              else
              {
                l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap = FALSE;
              }
            }

            if (dl_tbf_config_ptr->power_control.present)
            {
              uint8 index = 0;
              power_control_gamma_T *pwr_cntrl_ptr = &(dl_tbf_config_ptr->power_control.gamma_TN0);

              /*  Update Alpha value  */
              gprs_serv_cell_meas_ptr->alpha = dl_tbf_config_ptr->power_control.alpha;

              /*  Extract gamma_tn values  */
              do
              {
                if (pwr_cntrl_ptr->present)
                {
                  gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
                }

                /*  Increment pointer  */
                pwr_cntrl_ptr++;
                index++;

              }while (index < 8);
            }

            /*  Update uplink power based on the modified power control parameters. */
            gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

            /* setup downlink power control parameter's */
            if (dl_tbf_config_ptr->downlink_pwr.params_valid)
            {
              l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
              l1_transfer_data[gas_id].dl_power_control.po           = dl_tbf_config_ptr->downlink_pwr.po;
            }
            else
            {
              l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
            }

            gpl1_gprs_add_to_pending_dl_ta_params (dl_tbf_config_ptr->ta_params,message_ptr->gas_id);

            /* @@OG: EDTM Design
             * Take the DL st in the msg into account when present.
             */
            if( FALSE == dl_tbf_config_ptr->tbf_start_time_valid )
            {
              dl_tbf_config_ptr->tbf_start_time_valid = TRUE;
              dl_tbf_config_ptr->tbf_start_time       = ADD_FN( current_fn, (NPL1_FN_OFFSET+1) );
            }

            /* These two lines are necessary before calling the get starting time
             * function.
             */
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
            l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

            /* Make sure this function checks for start time
             * alignment on a radio block boundary.
             * This sets the l1_transfer_data[gas_id].starting_time variable.
             */
            (void)gpl1_gprs_get_starting_time( &dl_msg_dest_ptr->tbf_start_time,
                                               dl_tbf_config_ptr->tbf_start_time,
                                               PCCCH_52, message_ptr->gas_id );


            l1_transfer_data[gas_id].dl_st    = dl_msg_dest_ptr->tbf_start_time;

            /* dl_tbf_config_ptr->tbf_start_time used later to compare with Dedicated St time 
            and adjusted Starttime for EDTM accordingly. This should have updated Starttime Value*/

            dl_tbf_config_ptr->tbf_start_time = dl_msg_dest_ptr->tbf_start_time;

          MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,l1_transfer_data[gas_id].pending_dl_tbf_ptr,l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap,dl_msg_dest_ptr->tbf_start_time);
          if((FALSE != l1_transfer_data[gas_id].starting_time_valid))
          {
            if(l1_transfer_data[gas_id].pending_dl_tbf_ptr->allow_one_rb_gap)
            {
              (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,dl_msg_dest_ptr->tbf_start_time,2, gas_id);
              gpl1_g2x_ta_scheduler(gas_id);
            }
          }
   
            
            if (dl_tbf_config_ptr->dl_tfi_present)
            {
              dl_msg_dest_ptr->dl_tfi_present = TRUE;
              dl_msg_dest_ptr->dl_tfi         = dl_tbf_config_ptr->dl_tfi;
            }
            else
            {
              /* no tfi present so use current tfi
               * must be a reallocation as we need a dl tfi for
               * the dl assignment
               * Check that dl_tbf_ptr is valid just in case
               */
              if (l1_transfer_data[gas_id].dl_tbf_ptr)
              {
                dl_msg_dest_ptr->dl_tfi_present  = TRUE;
                dl_msg_dest_ptr->dl_tfi = l1_transfer_data[gas_id].dl_tbf_ptr->dl_tfi;
              } else
              {
                /* this is an assignment error!!! */
                MSG_GERAN_ERROR_0_G("PL1: TRANSFER No DL Tfi present");
              }
            }

            /* set up tbf establish type for send PH_CONNECT_IND to MAC */
            if (l1_transfer_data[gas_id].ul_st == L1_GPRS_INVALID_FN)
            {
              l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;
            }
            else if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
            {
              /*  Temporarily change some parameters, as PDA setup before PUA starting
                  time has elapsed but PDA should be setup based on PUA being active. */
              /*  Parameters to be changed:- sub_state, sub_sub_state in l1_isrtsk_blk.
                  current_alloc_type and pending_alloc_type in l1_transfer_data[gas_id].  */
              l1_tskisr_blk->sub_state            = bkup_sub_state;
              l1_tskisr_blk->sub_sub_state        = bkup_sub_sub_state;
              l1_transfer_data[gas_id].current_alloc_type = bkup_current_alloc_type;
              l1_transfer_data[gas_id].pending_alloc_type = bkup_pending_alloc_type;
            }

            if( FALSE != FREQ_CONFIG_RECV(gas_id) )
            {
              /* Use the same handling as per the DL TBF CONFIG */
#if 0 /* Doing this so I can have a non biased picture of transfer
       * when doing the implicit release.
       */
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif
              set_main_cmd = TRUE;
            }
            else
            {
              MSG_GERAN_ERROR_0_G(" DL TBF present but no FREQ CFG ");
            }

#if 1 /* needs enabling */
            if( FALSE == GPL1_IS_UL_TBF_PRESENT[ message_ptr->dtm_tbf_type ] )
            {
              /* Do any necessary work to release ul tbf */
              switch( l1_transfer_data[gas_id].pending_alloc_type )
              {
              case  L1_DYNAMIC_DOWN_ALLOC:
                /* The only problem with that is the isr scheduler which would
                 * recognize this msg as an uplink release and hence, in egprs, not copy
                 * the bep period and link qual meas mode to the qual meas module.
                 */
                l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;

                MSG_GERAN_MED_0_G(" UL TBF implicit release no conf to MAC ");
                break;

              default:
                break;
              }

              if( FALSE != gpl1_gprs_get_ul_num_rrbp_msg(message_ptr->gas_id) )
              {
                /* Change the dedicated start time to happen after the UL PCA
                 * has been transmitted.
                 * This is necessary incase the PCA DL tx fn and PCA UL tx fn
                 * have different values.
                 */
                uint8  rrbp_index = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

                if( SIZE_OF_RRBP_BUFF != rrbp_index )
                {
                  uint32 rrbp_fn = gpl1_gprs_get_rrbp_frame( rrbp_index,message_ptr->gas_id );
                  uint32 tx_fn   = ADD_FN( rrbp_fn, 4 );

                  gpl1_gprs_chk_blk_bndry( &tx_fn );

                  /* Only replace the value if it is higher, otherwise
                   * the start time allows the RRBP msg to be transmitted
                   * already.
                   */
                  if( tx_fn > dl_msg_dest_ptr->tbf_start_time )
                  {
                    dl_msg_dest_ptr->tbf_start_time    = tx_fn;
                    l1_transfer_data[gas_id].dl_st             = tx_fn;
                    l1_transfer_data[gas_id].starting_time     = tx_fn;
                    dl_tbf_config_ptr->tbf_start_time = tx_fn;
                  }
                }

                /* Test again for a PCA UL message */
                rrbp_index = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

                if( SIZE_OF_RRBP_BUFF != rrbp_index )
                {
                  uint32 rrbp_fn = gpl1_gprs_get_rrbp_frame( rrbp_index,message_ptr->gas_id );
                  uint32 tx_fn   = ADD_FN( rrbp_fn, 4 );

                  gpl1_gprs_chk_blk_bndry( &tx_fn );

                  /* Only replace the value if it is higher, otherwise
                   * the start time allows the RRBP msg to be transmitted
                   * already.
                   */
                  if( tx_fn > dl_msg_dest_ptr->tbf_start_time )
                  {
                    dl_msg_dest_ptr->tbf_start_time    = tx_fn;
                    l1_transfer_data[gas_id].dl_st             = tx_fn;
                    l1_transfer_data[gas_id].starting_time     = tx_fn;
                    dl_tbf_config_ptr->tbf_start_time = tx_fn;
                  }
                }

              }

              /* Is there a pending uplink TBF still being queued?
               * Remove it since the Implicit UL release will replace it.
               */
              if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
              {
                /* Null the main command */
                l1_tskisr_blk->main_command = L1_NULL_COMMAND;

                /* Disable the Ul start time */
                l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

                /* Any pending allocation work required ? */
              }

              /* Are there any current uplink TBF operations running?
               * If yes then should L1:
               * Release immediately?
               * or
               * Queue a Release?
               */
            } /* End of if ul tbf is not present in the message */
#endif
          } /* End of if dl tbf present */

          /* Is uplink TBF present ? */
          if( FALSE != GPL1_IS_UL_TBF_PRESENT[ message_ptr->dtm_tbf_type ] )
          {
            /* Initialise the timing advance to zero
             * alloc variables are for debug purposes.
             */
            uint8  i;
            uint8  ul_ts_alloc          = 0;
            uint8  l1_ul_ts_alloc       = 0;

            mac_dynamic_tbf_information_T*  ul_tbf_config_ptr    = &( message_ptr->ul_tbf );
            dynamic_tbf_information_T*      dynamic_msg_dest_ptr = NULL;

            /***************************************************************/
            /* @@WARNING: Check for any pending UL TBF release and stop it */
            /***************************************************************/

           /* Based on the current sub state work out where to
            * copy new data to. Removed checks for state - DTM.
            */
           switch( l1_tskisr_blk->sub_state )
           {
           case  L1_DYNAMIC_1:
           case  L1_DOWNLINK_1:
           case  L1_DOWNLINK_2:

              /* Reconfigure dynamic tbf or add to existing DL tbf */
              dynamic_msg_dest_ptr                     = &l1_transfer_data[gas_id].dynamic_tbf_2;
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;
             break;

           case  L1_DYNAMIC_2:

              /* reconfigure dynamic tbf and pend dyn ptr */
              dynamic_msg_dest_ptr                     = &l1_transfer_data[gas_id].dynamic_tbf_1;
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
             break;

           default:

             GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

             /* something has gone wrong, report error and return */
             MSG_GERAN_ERROR_0_G("PL1: Transfer bad sub_state! ");
             return;
           }

           /* Copy in the mac mode */
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode = ul_tbf_config_ptr->mac_mode;

          dynamic_msg_dest_ptr->test_mode    = ul_tbf_config_ptr->test_mode;
          dynamic_msg_dest_ptr->test_mode_tn = ul_tbf_config_ptr->ts_offset;

          /* If the ts offset is zero then only loopback lowest allocated Rx */
          if(ul_tbf_config_ptr->ts_offset == 0)
          {
            dynamic_msg_dest_ptr->test_mode_tn = gpl1_gprs_ts_conv_unslam(gprs_serv_cell_meas_ptr->lowest_tn.tn,message_ptr->gas_id);
          }

          if ( FALSE != ul_tbf_config_ptr->bep_period2.present )
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->bep_period2 =
              ul_tbf_config_ptr->bep_period2.bep_period2;
          }
          else
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
          }
          /* set up pending allocation type */
          if ((l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC ) ||
              (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) )
          {
            /* moving from dl, or dynamic + dl tbf to dl + dynamic ul tbf */
            l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;

          }
          else if (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_ALLOC )
          {
            /* reconfiguring dynamic allocation, still single ul tbf */
            l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;
          }

          /* initialise number of timeslots to zero */
          dynamic_msg_dest_ptr->nts = 0;

          for (i=0;i< MAX_TN;i++)
          {
            if (ul_tbf_config_ptr->usf[i].present)
            {
              /* For debug purposes */
              ul_ts_alloc |= ( 0x80 >> i );

              l1_ul_ts_alloc |= (0x0001 << i );

              dynamic_msg_dest_ptr->usf_value[i] = ul_tbf_config_ptr->usf[i].usf_value;
              dynamic_msg_dest_ptr->nts++;

              if ((uint8)dynamic_msg_dest_ptr->test_mode == L1_TEST_MODE_B)
              {
                /* Setup test mode B tn allocation */
                gpl1_gprs_tm_b_store[gas_id].ul_nts++;
              }

            } else
              /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
              dynamic_msg_dest_ptr->usf_value[i] = GPL1_INVALID;
          }

          /* Setup new usf granularity FALSE = 1 block tx , TRUE = 4 block tx */
          dynamic_msg_dest_ptr->usf_granularity = ul_tbf_config_ptr->usf_granularity;

          /* setup downlink power control parameter's */
          if (ul_tbf_config_ptr->dl_ctrl_param.params_valid)
          {
            l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
            l1_transfer_data[gas_id].dl_power_control.po           = ul_tbf_config_ptr->dl_ctrl_param.po;
          }
          else
          {
            l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
          }

          /* No need to take the timeslots of the current downlink assignment
           * into account since this DTM assignment does a complete
           * replacement.
           */

          /* check if lowest numbered tn has changed */
          if( gpl1_gprs_lowest_tn_changed( l1_ul_ts_alloc , l1_get_current_timeslot(message_ptr->gas_id) ) )
          {
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;

            /* Check if there is a PCA or PDAN w FAI or PDAN w CRD RRBP */
            /* Only create a starting time if we haven't got one */
            if (!ul_tbf_config_ptr->tbf_start_time_valid)
            {
              ul_tbf_config_ptr->tbf_start_time_valid =
              gpl1_gprs_transfer_find_rrbp_type(&ul_tbf_config_ptr->starting_time,message_ptr->gas_id);
            }
          }
          else
          {
            /* Is the TBF currently suspended ? */
            if (l1_transfer_data[gas_id].disable_rx_tx)
            {
              /* Ensure we don't overwrite the gap and st var's if we are currently suspended */
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap =
                l1_transfer_data[gas_id].dynamic_tbf_ptr->allow_one_rb_gap;

              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->starting_time =
                l1_transfer_data[gas_id].dynamic_tbf_ptr->starting_time;
             }
             else
             {
               l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
             }
          }

          if (ul_tbf_config_ptr->power_control_params.present)
          {
            uint8 index = 0;
            const power_control_gamma_T*  pwr_cntrl_ptr = &(ul_tbf_config_ptr->power_control_params.gamma_TN0);

            /*  Update Alpha value  */
            gprs_serv_cell_meas_ptr->alpha = ul_tbf_config_ptr->power_control_params.alpha;

            /*  Extract gamma_tn values  */
            do
            {
              if (pwr_cntrl_ptr->present)
              {
                gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
              }

              /*  Increment pointer  */
              pwr_cntrl_ptr++;
              index++;

            }while (index < 8);
          }

          /*  Update uplink power based on the modified power control parameters. */
          gpl1_gprs_calc_rf_pwr( message_ptr->gas_id);

            /* Only create a starting time if we haven't got one */
            if (!ul_tbf_config_ptr->tbf_start_time_valid)
            {
              /* Save the RRBP so move the suspend block to after RRBP frame */
              ul_tbf_config_ptr->tbf_start_time_valid = TRUE;
              ul_tbf_config_ptr->starting_time        = ((gl1_get_FN( message_ptr->gas_id ) + 4) % FRAMES_IN_HYPERFRAME);

              /* Do we have any pending RRBP's to be sent */
              i = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id);

              if (i < SIZE_OF_RRBP_BUFF)
              {
                /* We have found a PCA */
                uint32  pca_fn = gpl1_gprs_get_rrbp_frame(i,message_ptr->gas_id);

                /* Is the Frame number of the PCA before our PTR starting time ? */
                if ( gpl1_gprs_is_frame_num_later(pca_fn,ul_tbf_config_ptr->starting_time))
                {
                  ul_tbf_config_ptr->starting_time =  ((pca_fn + 8) % FRAMES_IN_HYPERFRAME);
                }
              }

              MSG_GERAN_MED_1_G("ST made up %d",ul_tbf_config_ptr->starting_time);
            } /* End if msg start time is valid */

            /* check to see if a start time has been specified */
            if (ul_tbf_config_ptr->tbf_start_time_valid)
            {
              /*  Determine if starting time present and if it needs adjusting  */
              l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

              /* Note this can only be a 52 style starting time */
              if( gpl1_gprs_get_starting_time( &dynamic_msg_dest_ptr->starting_time,
                                               ul_tbf_config_ptr->starting_time,
                                               PCCCH_52, message_ptr->gas_id ) )
              {
                /* Starting time has expired, set as if no starting time present */
                ul_tbf_config_ptr->starting_time = ((gl1_get_FN( message_ptr->gas_id )+TEMP_DELAY_OS)% FRAMES_IN_HYPERFRAME);

                /*  Re - calculate with new starting time */
                (void)gpl1_gprs_get_starting_time( &dynamic_msg_dest_ptr->starting_time,
                                                   ul_tbf_config_ptr->starting_time,
                                                   PCCCH_52, message_ptr->gas_id );
              }

              l1_transfer_data[gas_id].ul_st           = dynamic_msg_dest_ptr->starting_time;
              ul_tbf_config_ptr->starting_time = dynamic_msg_dest_ptr->starting_time;

              /* @@OG: EDTM Design
               * do not think these checks are valid in this case
               */
              if(  ( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
                 &&( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].dl_st )
                )
              {
                /* @@Warning: This is no functional change but is used
                 * to track the issue experienced in CR 62533 where PL1
                 * is unable to connect assignments in particular
                 * scenarios. This is because a new UL assignment must
                 * wait for an old DL assignment before it can be connected
                 * and vice versa.
                 */
                if( l1_transfer_data[gas_id].ul_st < l1_transfer_data[gas_id].dl_st )
                {
                  MSG_GERAN_ERROR_3_G(" PL1 unable to connect UL tbf st %u before DL tbf st %u ",
                            l1_transfer_data[gas_id].ul_st,
                            l1_transfer_data[gas_id].dl_st,
                            0 );
                }
              }
            }
            else if (l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN)
            {
              /*  If an PDA is in progress then do not clear start time variables. */
              l1_transfer_data[gas_id].starting_time_valid = FALSE; // move into transfer ASAP.
              l1_transfer_data[gas_id].starting_time_elapsed = FALSE;
            }
            
          MSG_GERAN_HIGH_4_G("G2X_TA TS check starting_time_valid:%d, l1_transfer_data.pending_dynamic_tbf_ptr:%X, allow:%d, tbf_start_time:%d",l1_transfer_data[gas_id].starting_time_valid,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr,l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap,dynamic_msg_dest_ptr->starting_time);
          if((FALSE != l1_transfer_data[gas_id].starting_time_valid))
          {
            if(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap)
            {
              (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,dynamic_msg_dest_ptr->starting_time,2, gas_id);
              gpl1_g2x_ta_scheduler(gas_id);
            }
          }

            /* set up tbf establish type for send PH_CONNECT_IND to MAC */
            if (l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN)
            {
              l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;
            }

           /* @@OG: EDTM Design, removed checks for dtm state here
            * before executing the next couple of lines.
            */
#if 0 /* Doing this so I can have a non biased picture of transfer
       * when doing the implicit release.
       */
            /* signal to L1 ISR that a TBF reconfig is pending */
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif
            set_main_cmd = TRUE;

            MSG_GERAN_MED_1_G(" ul_ts_alloc  0x%X ", ul_ts_alloc);

            if( FALSE != FREQ_CONFIG_RECV(gas_id) )
            {
              /* Use the same handling as per the DYNAMIC TBF CONFIG */
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;

              set_main_cmd = TRUE;
            }
            else
            {
              MSG_GERAN_ERROR_0_G(" UL TBF present but no FREQ CFG present");
            }

            if( FALSE == GPL1_IS_DL_TBF_PRESENT[ message_ptr->dtm_tbf_type ] )
            {
              /* Do any necessary work to release dl tbf */
              switch( l1_transfer_data[gas_id].pending_alloc_type )
              {
              case  L1_DYNAMIC_DOWN_ALLOC:
                /* The only problem with that is the isr scheduler which would
                 * recognize this msg as an downlink release and hence, in egprs, not copy
                 * the bep period and link qual meas mode to the qual meas module.
                 */
                l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;

                MSG_GERAN_MED_0_G(" DL TBF implicit release no conf to MAC ");
                break;

              default:
                break;
              }

              if( FALSE != gpl1_gprs_get_dl_num_rrbp_msg(message_ptr->gas_id) )
              {
                /* Change the dedicated start time to happen after the DL PCA
                 * has been transmitted.
                 * This is necessary incase the PCA UL tx fn and PCA DL tx fn
                 * have different values.
                 */
                uint8  rrbp_index = gpl1_gprs_get_matching_rrbp_msg_type( PCA_DL_FAI_MSG,message_ptr->gas_id );

                if( SIZE_OF_RRBP_BUFF != rrbp_index )
                {
                  uint32 rrbp_fn = gpl1_gprs_get_rrbp_frame( rrbp_index,message_ptr->gas_id );
                  uint32 tx_fn   = ADD_FN( rrbp_fn, 4 );

                  gpl1_gprs_chk_blk_bndry( &tx_fn );

                  /* Only replace the value if it is higher, otherwise
                   * the start time allows the RRBP msg to be transmitted
                   * already.
                   */
                  if( tx_fn > dynamic_msg_dest_ptr->starting_time )
                  {
                    dynamic_msg_dest_ptr->starting_time = tx_fn;
                    l1_transfer_data[gas_id].ul_st              = tx_fn;
                    l1_transfer_data[gas_id].starting_time      = tx_fn;
                    ul_tbf_config_ptr->starting_time    = tx_fn;
                  }
                }

                /* Test again for a PCA message */
                rrbp_index = gpl1_gprs_get_mathching_rrbp_index(message_ptr->gas_id );

                if( SIZE_OF_RRBP_BUFF != rrbp_index )
                {
                  uint32 rrbp_fn = gpl1_gprs_get_rrbp_frame( rrbp_index,message_ptr->gas_id );
                  uint32 tx_fn   = ADD_FN( rrbp_fn, 4 );

                  gpl1_gprs_chk_blk_bndry( &tx_fn );

                  /* Only replace the value if it is higher, otherwise
                   * the start time allows the RRBP msg to be transmitted
                   * already.
                   */
                  if( tx_fn > dynamic_msg_dest_ptr->starting_time )
                  {
                    dynamic_msg_dest_ptr->starting_time = tx_fn;
                    l1_transfer_data[gas_id].ul_st              = tx_fn;
                    l1_transfer_data[gas_id].starting_time      = tx_fn;
                    ul_tbf_config_ptr->starting_time    = tx_fn;
                  }
                }

                /* @@TODO: Test for more messages */
              }

              /* Is there a pending downlink TBF still being queued?
               * Remove it since the Implicit DL release will replace it.
               */
              if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
              {
                /* Null the main command */
                l1_tskisr_blk->main_command = L1_NULL_COMMAND;

                /* Disable the Ul start time */
                l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

                /* Any pending allocation work required ? */
              }

              /* Are there any current downlink TBF operations running?
               * If yes then should L1:
               * Release immediately?
               * or
               * Queue a Release?
               */

            } /* End of if dl tbf is not present in the message */
            else
            {
              /* Ensure pending alloc is setup correctly */
              l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_DOWN_ALLOC;
              l1_transfer_data[gas_id].tbf_est_type       = UPLINK_DOWNLINK_TBF_ESTABLISHED;
            }
          } /* End of if ul tbf present */
        }
        else
        {
          MSG_GERAN_ERROR_3_G( "Unsupported dtm_tbf_t:%d in DTM Assignment",
                     message_ptr->dtm_tbf_type,
                     0,
                     0 );
        }

        /* Check the main command flag and set it to transfer if it
         * is true.
         */
        if( FALSE != set_main_cmd )
        {
          /* Set the main command to transfer command inorder to queue the
           * TBF configuration included in this dtm assignment message.
           */
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;

          /* Reset the flag */
          set_main_cmd = FALSE;
        }

        /* Check that the freq config message was received prior
         * to the dtm channel assignment.
         * @@TODO: Check if we really need to impose this condition?
         */
        if( FALSE != FREQ_CONFIG_RECV(gas_id) )
        {
          static const uint8  BLK_BOUNDARY_FN_OFFSET[ 13 ]=
          {
            /* 0  1  2  3  4  5  6  7  8  9  10 11 12 */
               0, 3, 2, 1, 0, 3, 2, 1, 0, 4, 3, 2, 1
          };
          static uint32 resumption_frame_nbr[NUM_GERAN_DATA_SPACES];

          /* Set the command to L1_DEDICATED_COMMAND
           * Set the starting time
           */
          l1_tskisr_blk->command          = L1_DEDICATED_COMMAND;

          /* Check if we are TBF suspended, ensure starting time is after
           * suspension completes
           */
          if ( gpl1_get_tbf_resumption_fn(&resumption_frame_nbr[gas_id], gas_id) )
          {
            l1_dedicated_data_ptr->starting_time  =  ADD_MOD( resumption_frame_nbr[gas_id], (NPL1_FN_OFFSET+1), 42432 );
          }
          else
          {
            l1_dedicated_data_ptr->starting_time  =
              ADD_MOD( frame_counters[gas_id].FNmod42432, (NPL1_FN_OFFSET+1), 42432 );
          }
          /* Align on a block boundary */
          l1_dedicated_data_ptr->starting_time =
            ADD_MOD( l1_dedicated_data_ptr->starting_time,
                     BLK_BOUNDARY_FN_OFFSET[ (l1_dedicated_data_ptr->starting_time%13) ],
                                                     42432 );

          /* @@TODO match the packet and cs start times
           * Might need to extend the cs st based on the packet
           * especially in cases of polled dl/ul assignments.
           * Generalise this more to use the transfer data start time.
           */
          switch( message_ptr->dtm_tbf_type )
          {
          case  DL_TBF_PRESENT:

            if( FALSE != message_ptr->dl_tbf.tbf_start_time_valid )
            {
              if(gpl1_gprs_is_frame_num_later(message_ptr->dl_tbf.tbf_start_time,l1_dedicated_data_ptr->starting_time ))
              {
                l1_dedicated_data_ptr->starting_time  = message_ptr->dl_tbf.tbf_start_time % 42432;
              }

              /* Assumes tbf start time is already block aligned
               * so no extra checking is done here.
               */
               MSG_GERAN_MED_3_G(" DTM chan assign start time %u dl_st %u FN %u",
                       l1_dedicated_data_ptr->starting_time,
                message_ptr->dl_tbf.tbf_start_time,
                    GSTMR_GET_FN_GERAN( message_ptr->gas_id) );
            }
            else
            {

              MSG_GERAN_MED_3_G(" DTM chan assign start time %u FN %u FN42432 %u ",
                l1_dedicated_data_ptr->starting_time,
            GSTMR_GET_FN_GERAN( message_ptr->gas_id),
            frame_counters[message_ptr->gas_id].FNmod42432 );

            }
            break;

          case  UL_TBF_PRESENT:

            if( FALSE != message_ptr->ul_tbf.tbf_start_time_valid )
            {
              if(gpl1_gprs_is_frame_num_later(message_ptr->ul_tbf.starting_time,l1_dedicated_data_ptr->starting_time ))
              {
                l1_dedicated_data_ptr->starting_time  = message_ptr->ul_tbf.starting_time % 42432;
              }
            }
            else
            {
              MSG_GERAN_MED_3_G(" DTM chan assign start time %u FN %u FN42432 %u ",
                l1_dedicated_data_ptr->starting_time,
            GSTMR_GET_FN_GERAN( message_ptr->gas_id),
            frame_counters[message_ptr->gas_id].FNmod42432 );
            }
            break;

          case  UL_DL_TBF_PRESENT:

            if( FALSE != message_ptr->dl_tbf.tbf_start_time_valid )
            {
        if(gpl1_gprs_is_frame_num_later(message_ptr->dl_tbf.tbf_start_time,l1_dedicated_data_ptr->starting_time))
              {
                l1_dedicated_data_ptr->starting_time  = message_ptr->dl_tbf.tbf_start_time % 42432;
                /* Aligining the Transfer ST time with dedicated time */
                l1_transfer_data[gas_id].starting_time = l1_transfer_data[gas_id].dl_st;
              }
              /* Assumes tbf start time is already block aligned
               * so no extra checking is done here.
               */
               MSG_GERAN_MED_3_G(" DTM chan assign start time %u dl_st %u FN %u",
                       l1_dedicated_data_ptr->starting_time,
                message_ptr->dl_tbf.tbf_start_time,
                    GSTMR_GET_FN_GERAN( message_ptr->gas_id) );
            }

            if( FALSE != message_ptr->ul_tbf.tbf_start_time_valid )
            {
              if( gpl1_gprs_is_frame_num_later((message_ptr->ul_tbf.starting_time % 42432),l1_dedicated_data_ptr->starting_time  ))
              {
                l1_dedicated_data_ptr->starting_time = message_ptr->ul_tbf.starting_time % 42432;
                /* Aligining the Transfer ST time with dedicated time */
                l1_transfer_data[gas_id].starting_time = l1_transfer_data[gas_id].ul_st;
              }
            }

            break;

          case  NO_TBF_PRESENT:

             MSG_GERAN_MED_3_G(" DTM chan assign start time %u FN %u FN42432 %u ",
               l1_dedicated_data_ptr->starting_time,
           GSTMR_GET_FN_GERAN( message_ptr->gas_id),
           frame_counters[message_ptr->gas_id].FNmod42432 );

            break;

          default:

             MSG_GERAN_MED_3_G(" DTM chan assign start time %u FN %u FN42432 %u ",
               l1_dedicated_data_ptr->starting_time,
           GSTMR_GET_FN_GERAN( message_ptr->gas_id),
           frame_counters[message_ptr->gas_id].FNmod42432 );

             MSG_GERAN_ERROR_1_G(" Unknown TBF type %u ", message_ptr->dtm_tbf_type);
            break;
          }

          /* @@WARNING: Although some upper sections of the code are int-locked,
           * the calculation of the frames could, by this point, be out
           * of step between dedic and transfer. Thus either intlock
           * the whole thing or find another safer way.
           */

          RESET_BITMAP(gas_id);
        }
        else
        {
          /* Report an error
           * Might want to combine this assignment with a frequency configure
           * that arrives after this point.
           */
          MSG_GERAN_ERROR_0_G( " No dtm freq config received, connecttion abandoned ");
        }

        /* Unlock interrupts */
        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

      } /* case MAC_L1_DTM_CHANNEL_ASSIGN_REQ */
      break;

      case  MAC_L1_DELETE_UL_CTRL_MSG:
      {
        const mac_l1_delete_ul_ctrl_msg_t* message_ptr =
          (mac_l1_delete_ul_ctrl_msg_t*) msg_header;

        GL1_ISR_LOCK(message_ptr->gas_id);

        gas_id = message_ptr->gas_id;
        MSG_GERAN_MED_2_G(" MAC_L1_DELETE_UL_CTRL_MSG rcvd FN %u type %u ", GSTMR_GET_FN_GERAN( message_ptr->gas_id ),message_ptr->message_type);
        if (l1_transfer_data[gas_id].current_alloc_type == L1_SINGLE_BLOCK_ALLOC  )
        {
          /* clear out the PACCH UL FIFO if we have any msgs remaining from transfer */
          gpl1_gprs_reset_pacch_fifo(gas_id);
        }
        else
        {
        gpl1_queue_mac_l1_delete_ul_ctrl_msg( message_ptr->message_type, gas_id );
        }

       GL1_ISR_UNLOCK(message_ptr->gas_id);
      }
      break;

#endif  /* FEATURE_GSM_EDTM && FEATURE_GSM_DTM */

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
      case  MAC_L1_RECOVERY_REQ:
          {
            mac_l1_recovery_req_t *message_ptr;
            message_ptr = (mac_l1_recovery_req_t *)msg_header;
            gas_id = message_ptr->gas_id;
            gpl1_gprs_set_recovery_state(GPL1_RECOVERY_STARTED_STATE,gas_id);
            /* Ensure that the recovery confirm need be sent */
            gpl1_gprs_set_recovery_cnf_pending(TRUE,gas_id);
            gpl1_gprs_handle_recovery_req(message_ptr->gas_id);
          }
          break;
#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */

    default:
      MSG_GERAN_ERROR_0("PL1 TRANSFER Bad message received");
      break;

    } /* End of switch */

  } else if (msg_header->message_set == MS_RR_L1)
  {
    switch ( msg_header->message_id )
    {
    /* Enable Sleep indication */
  case MPH_ENABLE_SLEEP_IND:
  {
    L1_handle_ENABLE_SLEEP_IND ( msg_header );
      break;
    }

    /********************************************************
    *                                                      *
    *                MPH_START_MM_NON_DRX_IND:             *
    *                                                      *
    ********************************************************/
    case MPH_START_MM_NON_DRX_IND:
      {
        mph_start_mm_non_drx_ind_t* message_ptr = ( mph_start_mm_non_drx_ind_t* )msg_header;
        idle_data_T               *l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];

        gas_id = message_ptr->gas_id;
        MSG_GERAN_HIGH_1_G("MPH_START_MM_NON_DRX_IND %d",message_ptr->non_updated_lai_in_nmo1);
        /* Enable the MM non-drx mode period */
        l1_idle_data->mm_non_drx_mode = TRUE;
        l1_idle_data->non_updated_lai_in_nmo1 = message_ptr->non_updated_lai_in_nmo1;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_NON_DRX_START_IND);
#endif
       
        break;
      }

      /********************************************************
      *                                                      *
      *                MPH_STOP_MM_NON_DRX_IND:              *
      *                                                      *
      ********************************************************/
    case MPH_STOP_MM_NON_DRX_IND:
      {
        mph_stop_mm_non_drx_ind_t* message_ptr = ( mph_stop_mm_non_drx_ind_t* )msg_header;
        idle_data_T               *l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];

        gas_id = message_ptr->gas_id;
        MSG_GERAN_MED_0_G("MPH_STOP_MM_NON_DRX_IND");
        /* Disable the MM non-drx mode period */
        l1_idle_data->mm_non_drx_mode = FALSE;
        l1_idle_data->non_updated_lai_in_nmo1 = FALSE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_NON_DRX_STOP_IND);
#endif
        break;
      }

      /********************************************************
       *                                                      *
       *                MPH_NC_MEASUREMENT_REQ                *
       *                                                      *
       ********************************************************/
    case MPH_NC_MEASUREMENTS_REQ:
      {
        mph_nc_measurements_req_t *message_ptr;
        gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;
        /*lint -e740*/
        message_ptr = ( mph_nc_measurements_req_t *) msg_header;
        /*lint +e740*/
        gas_id = message_ptr->gas_id;
        gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(message_ptr->gas_id);
        gpl1_gprs_meas_mode_ptr->nc_mode=TRUE;
        L1_remove_oob_ba(&ba_buffer,&message_ptr->balist);
        l1_sc_set_ba_list(&ba_buffer, 0xFF,message_ptr->gas_id);

        MSG_GERAN_LOW_0_G("***MPH_NC_MEASUREMENTS_REQ***");

        break;
      }


      /********************************************************
       *                                                      *
       *                MPH_STOP_NC_MEASUREMENT_REQ           *
       *                                                      *
       ********************************************************/
    case MPH_STOP_NC_MEASUREMENTS_REQ:
      {
        mph_stop_nc_measurements_req_t *message_ptr;
        gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;
        /*lint -e740*/
        message_ptr = ( mph_stop_nc_measurements_req_t *) msg_header;
        /*lint +e740*/
        gas_id = message_ptr->gas_id;
        gpl1_gprs_meas_mode_ptr =
                   gl1_ms_switch_meas_mode_data(message_ptr->gas_id);

        gpl1_gprs_meas_mode_ptr->nc_mode=FALSE;
        L1_remove_oob_ba(&ba_buffer,&message_ptr->balist);
        l1_sc_set_ba_list(&ba_buffer, 0xFF,message_ptr->gas_id);

        MSG_GERAN_LOW_0_G("***MPH_STOP_NC_MEASUREMENTS_REQ***");

        break;
      }

      /********************************************************
       *                                                      *
       *            MPH_GPRS_SURROUND_UPDATE_REQ              *
       *                                                      *
       ********************************************************/
    case MPH_GPRS_SURROUND_UPDATE_REQ:
      {
        mph_gprs_surround_update_req_t *message_ptr;

        /*lint -e740*/
        message_ptr = (mph_gprs_surround_update_req_t *)msg_header;
        /*lint +e740*/
        gas_id = message_ptr->gas_id;
        if( L1_TEST_MODE_OFF == l1_transfer_data[gas_id].l1_test_mode )
        {
          gpl1_gprs_update_bcch( message_ptr->gprs_surround_data.bcch_params.bcch_arfcn,
                                 message_ptr->gprs_surround_data.bcch_params.si_requested,
                                 message_ptr->gprs_surround_data.bcch_params.si13_normal_or_ext,
                                 message_ptr->gas_id );
        } /* End of if l1 is in test mode */
        else
        {
          MSG_GERAN_ERROR_0( " MPH_GPRS_SURROUND_UPDATE_REQ ignored: L1 in test mode ");
        }
        break;
      }

      /********************************************************
       *                                                      *
       *            MPH_STOP_SURROUND_UPDATE_REQ              *
       *                                                      *
       ********************************************************/
    case MPH_STOP_SURROUND_UPDATE_REQ:
      {
        mph_stop_surround_update_req_t* message_ptr = ( mph_stop_surround_update_req_t* )msg_header;
        /* This function stop sboth bcch and pbcch updates. */
        gpl1_gprs_update_bcch_stop( FALSE, message_ptr->gas_id );
        break;
      }



      /********************************************************
      *                                                      *
      *                MPH_READ_SYS_INFO_REQ                 *
      *                                                      *
      ********************************************************/

    case MPH_READ_SYS_INFO_REQ:

      {
        /* Only accept sys info read requests when L1 is not
         * in test mode inorder to prevent tbf suspensions
         * from occuring.
         */
        if( L1_TEST_MODE_OFF == l1_transfer_data[((mph_read_sys_info_req_T *)msg_header)->gas_id].l1_test_mode )
        {
          L1_process_READ_SYS_INFO_REQ_TRANS( (mph_read_sys_info_req_T *)msg_header );
        }
        else
        {
          MSG_GERAN_ERROR_0( " MPH_READ_SYS_INFO_REQ ignored: L1 in test mode ");
        }
        break;
      }

      /********************************************************
       *                                                      *
       *                MPH_GPRS_L1_PARAM_UPDATE_REQ          *
       *                                                      *
       ********************************************************/


    case MPH_GPRS_L1PARAM_UPDATE_REQ:
      {
        MSG_GERAN_MED_2("MPH_GPRS_L1PARAM_UPDATE_REQ", 0 , 0 );

        /*  Load the parameters into the pending store  */
        gpl1_gprs_L1_params_pending(msg_header);

        break;
      }

      /********************************************************
       *                                                      *
       *                MPH_STOP_GSM_MODE_REQ                 *
       *                                                      *
       ********************************************************/

    case MPH_STOP_GSM_MODE_REQ:
      {
        mph_stop_gsm_mode_req_T *message_ptr;

        message_ptr = (mph_stop_gsm_mode_req_T *)msg_header;
        gas_id = message_ptr->gas_id;
        MSG_GERAN_MED_0_G("MPH_STOP_MODE_REQ");

        /* Stop the SCE */
        l1_sc_stop(message_ptr->gas_id);
        /* Move to NULL state */
        L1_setup_DEACTIVATE( msg_header );
        break;
      }



      /********************************************************
      *                                                       *
      *      MPH_WCDMA_CELL_UPDATE_LIST_REQ                   *
      *                                                       *
      ********************************************************/

    case MPH_WCDMA_CELL_UPDATE_LIST_REQ:
      {
        mph_wcdma_cell_update_list_req_T* msg;

        msg = ( mph_wcdma_cell_update_list_req_T* )msg_header;
        l1_sc_set_wcdma_list_update_inprogress( TRUE , msg->gas_id );
        l1_sc_wcdma_update_list( msg, msg->gas_id );
        l1_sc_set_wcdma_list_update_inprogress( FALSE , msg->gas_id );
        break;
      }


      /********************************************************
       *                                                      *
       *                MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ     *
       *                                                      *
       ********************************************************/

    case MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ:
       {
          mph_specific_irat_freq_update_req_T* msg;
          msg = ( mph_specific_irat_freq_update_req_T* )msg_header;  
 
          l1_sc_specific_irat_freq_update(msg, msg->gas_id);
          break;
       }


      /********************************************************
       *                MPH_UE_MODE_CHANGE_REQ                *
       *                                                      *
       ********************************************************/

    case MPH_UE_MODE_CHANGE_REQ:
      {
        mph_ue_mode_change_req_T  *message_ptr;
        message_ptr = ( mph_ue_mode_change_req_T *) msg_header; /*lint !e740 */
#ifdef FEATURE_SGLTE
        /* Set SGLTE mode */
        gl1_hw_set_sglte_mode(message_ptr->sglte_mode_active, message_ptr->gas_id);        
#endif
        l1_sc_wcdma_set_mode(message_ptr->gsm_only,message_ptr->gas_id);
        break;
      }

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

      /**********************************************************
      *                                                         *
      *                MPH_DTM_CHANNEL_RELEASE_REQ              *
      *                                                         *
      ***********************************************************/
      case MPH_DTM_CHANNEL_RELEASE_REQ:
      {
        /* Setup a pointer to read msg content */
        const mph_dtm_channel_release_req_t*  message_ptr =
            (mph_dtm_channel_release_req_t *) msg_header;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
#ifdef FEATURE_GSM_TRANSFER_DED_SERVING_CELL_INFO
        dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[message_ptr->gas_id];
#endif
        gas_id = message_ptr->gas_id;

        if( L1_DEDICATED_COMMAND == l1_tskisr_blk->command )
        {

          idle_data_T               *l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];

          MSG_GERAN_MED_0_G(" MPH_DTM_CHANNEL_RELEASE_REQ ");

          /* NULL the command */
          l1_tskisr_blk->command = L1_NULL_COMMAND;

          /* */
          RESET_BITMAP(gas_id);

          /* Not sure whats the best way to cancel the TBF assignments
           * associated with the dtm channel assignment.
           */

#ifdef FEATURE_GSM_TRANSFER_DED_SERVING_CELL_INFO
          /* Previous cell applies only to inter-cell handovers */
          l1_dedicated_data_ptr->previous_cell_details.bsic_known = FALSE;
#endif
          /* Copy over any data */
          if (message_ptr->valid_bcch_carrier)
          {
            l1_idle_data->campedon_cell_ARFCN = message_ptr->serving_bcch_carrier;
            l1_idle_data->campedon_cell_BSIC = message_ptr->BSIC.BS_colour_code;

            l1_idle_data->campedon_cell_BSIC +=
                                            (byte)(message_ptr->BSIC.PLMN_colour_code * 8);

            /* Setup default BCCH decoding mask in case we go to Idle Mode */
            l1_idle_data->si_mask        = RR_L1_SI_TYPE_ALL_NORM;
            l1_idle_data->BCCH_mask      = gl1_compute_BCCH_mask(l1_idle_data->si_mask);
            l1_idle_data->ext_BCCH_mask  = gl1_compute_ext_BCCH_mask(l1_idle_data->si_mask);
          }
          else
          {
            l1_idle_data->campedon_cell_ARFCN.num = INVALID_ARFCN;

            /* Tell CGPS we are leaving this cell */
            gl1_timetag_cgps(L1_NULL_MODE, CGPS_TIMETAG_PRE_CELL_CHANGE,message_ptr->gas_id);
          }

        }   /* End of if command == L1_DEDICATED_COMMAND */
        else
        {
          MSG_GERAN_ERROR_0_G(" Unable to handle MPH_DTM_CHANNEL_RELEASE_REQ ");
        }

      }
      break;

    case  MPH_DTM_L1PARAM_UPDATE_REQ:
      {
        const mph_dtm_l1param_update_req_t*  msg_ptr =
          (const mph_dtm_l1param_update_req_t*)  msg_header;
        idle_data_T               *l1_idle_data=&l1_idle_data_store[msg_ptr->gas_id];
          /* Config the egprs quality measurements module using the bep period
           * broadcast on the BCCH.
           */
          gpl1_egprs_scell_qual_meas_broadcast_config
          ( msg_ptr->l1_system_info_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.bep_period, msg_ptr->gas_id );
          l1_idle_data->campedon_cell_data = msg_ptr->l1_system_info_params.ccch_idle_mode_params.layer1_sys_info; /* Struct to struct copy */
          l1_idle_data->campedon_cell_data.BS_PA_MFRMS = l1_override_bs_pa_mfrms(l1_idle_data->campedon_cell_data.BS_PA_MFRMS, msg_ptr->gas_id);


      } /* End of case MPH_DTM_L1PARAM_UPDATE_REQ */
      break;

#endif /* FEATURE_GSM_EDTM && FEATURE_GSM_DTM  */

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
      /********************************************************
      *                                                       *
      *           MPH_REDUCED_NCELL_MEAS_IND                  *
      *                                                       *
      ********************************************************/
      case MPH_REDUCED_NCELL_MEAS_IND:
      {
        mph_reduced_ncell_meas_ind_T *message_ptr = (mph_reduced_ncell_meas_ind_T *) msg_header;
        gas_id = check_gas_id(message_ptr->gas_id);  
        l1_sc_config_reduced_ncell_list_rr( message_ptr->reduced_ncell_list, gas_id );
        break;
      }
#endif

    /*******************************************************
    *                                                      *
    *                MPH_SET_PRIORITY_IND                  *
    *                                                      *
    ********************************************************/
    case MPH_SET_PRIORITY_IND:
      {
        mph_set_priority_ind_T* message_ptr = (mph_set_priority_ind_T* )msg_header;
        volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        gas_id = check_gas_id(message_ptr->gas_id);

        if (message_ptr->procedure_type == GERAN_GPRS_PROCEDURE)
        {
          grm_check_connected_reason(
            l1_tskisr_blk->client_id,
            GRM_PS_CONNECTION,
            gas_id);
        }
        break;
      }

    default:
      MSG_GERAN_ERROR_1("Invalid MS_RR_L1 message id %d",msg_header->message_id);

      break;
    } /* End of switch  */
  }
  /*******************************************************
  *                                                      *
  *                   GSM CB-L1 messages                 *
  *                                                      *
  ********************************************************/
  else if ( msg_header->message_set == MS_CB_L1)
  {
    if (msg_header->message_id == (uint8) CB_SCHEDULING_REQ)
    {
      cb_scheduling_req_type *message_ptr;
      message_ptr = (cb_scheduling_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_scheduling_info
      ( message_ptr->channel_ind,
        message_ptr->scheduling_mode,
        message_ptr->scheduling_descriptor.scheduling_period_fn_list,
        message_ptr->scheduling_descriptor.scheduling_period_length,
        geran_map_nas_id_to_gas_id(message_ptr->as_id)
      );
    } else if (msg_header->message_id == (uint8) CB_SKIP_REQ)
    {
      cb_skip_req_type *message_ptr;
      message_ptr = (cb_skip_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_skip_info
      ( message_ptr->channel_ind,
        message_ptr->scheduling_descriptor.scheduling_period_fn_list,
        message_ptr->scheduling_descriptor.scheduling_period_length,
        message_ptr->skip_ind,
        geran_map_nas_id_to_gas_id(message_ptr->as_id)
      );
    } else if (msg_header->message_id == (uint8) CB_DEACTIVATE_REQ )
    {
      cb_deactivate_req_type *message_ptr;
      message_ptr = (cb_deactivate_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_deactivate_info( message_ptr->channel_ind, geran_map_nas_id_to_gas_id(message_ptr->as_id) );
    }
  }
} /* End of Function */

/*===========================================================================

FUNCTION   gpl1_gprs_handle_tbf_release

DESCRIPTION
  This function check the current allocation type and depending on the which
  tbf UL, DL or UL & DL requires releasing will set up the pending tbf structures
  and inform the ISR to swap over to the new parameters

  This function is called directly from receoving the MAC_L1_TBF_RELEASE or by
  MAC_L1_PDCH_REL when all timeslots on the respective tbf(s) have been released

DEPENDENCIES
  l1_transfer_data[gas_id]

RETURN VALUE
  Returns TRUE if entering idle else FALSE

SIDE EFFECTS
  None

===========================================================================*/

boolean gpl1_gprs_handle_tbf_release(release_tbf_t   release_tbf, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean normal_release_transfer=FALSE;
  boolean return_to_idle = FALSE;

  l1_transfer_data[gas_id].tbf_release_req_type = release_tbf;

  switch (release_tbf)
  {
  /*************** DL RELEASE ORDERED *****************/
  case DL_TBF_NORMAL:
  case DL_TBF_ABNORMAL: /*lint !e616 */
    {

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
     #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/


      if( FALSE == gpl1_assess_dl_tbf_release( &( l1_transfer_data[gas_id] ), gas_id ) )
      {
         return  FALSE;
      }
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
      gpl1_gprs_register_release_recovery_event(release_tbf,gas_id);
#endif
#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
      switch( get_queued_mac_l1_edtm_cs_rel_tbf_alloc(gas_id ) )
      {
      case  NO_TBF_PRESENT:
      case  UL_TBF_PRESENT:
        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id);

        break;

      case  DL_TBF_PRESENT:

        MSG_GERAN_MED_0_G(" Cancelled enh alloc DL TBF ");

        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id);
        break;

      case  UL_DL_TBF_PRESENT:

        MSG_GERAN_MED_0_G(" Cancelled enh alloc DL TBF, only UL remain. ");

        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id);

        break;

      default:

        MSG_GERAN_ERROR_3_G( " Unknown enh alloc tbf type %u ",
                   get_queued_mac_l1_edtm_cs_rel_tbf_alloc(gas_id ),
                   0,
                   0 );
        break;
      }
#endif

      l1_transfer_data[gas_id].release_in_progress = FALSE;

      /*  Determine if a release has been received between an assignment and
          its associated startime. If so ensure the pending alloc etc handled
          correctly.  */
#ifdef FEATURE_GSM_DTM
      if ( DTM_PS_RECONFIG_PENDING(gas_id) || GPRS_PS_RECONFIG_PENDING )
#else
      if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
#endif /*FEATURE_GSM_DTM*/
      {
        if( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].dl_st )
        {
          /* Downlink Assignment in progress */
          MSG_GERAN_MED_3_G( "DL assignment in progress fn %u st %u",
                   gl1_get_FN( gas_id ),
                   l1_transfer_data[gas_id].dl_st,
                   0 );

          /* Clear out the pending downlink assignment
           * Do not alter the current/pending alloc type variables here
           * since they are checked later in this function.
           */
          if(  ( L1_GPRS_INVALID_FN == l1_transfer_data[gas_id].ul_st )
             &&( DOWNLINK_TBF_ESTABLISHED == l1_transfer_data[gas_id].tbf_est_type )
            )
          {
            /* NULL the main command, this removes the
             * pending downlink assignment.
             */
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;

#ifdef FEATURE_GSM_DTM
            /* In the DTM_PS_RECONFIG state - return to the DTM_RUN state */
            l1_set_dtm_command_event(L1_DTM_CANCEL_PS_RECONFIG,gas_id);
#endif
            /* Reset the start time related variables.
             * This ensures a connect indication is never sent.
             */
            l1_transfer_data[gas_id].dl_st                 = L1_GPRS_INVALID_FN;
            l1_transfer_data[gas_id].starting_time_valid   = FALSE;
            l1_transfer_data[gas_id].starting_time_elapsed = TRUE;

            /* Print out an F3 message */
            MSG_GERAN_MED_0_G( "DL assignment removed ");

            /* @@Warning: In the case where an uplink release was
             * received before the downlink release then nulling
             * the main command can be disastrous since it will
             * cancel the "non-rrbp pending" uplink release.
             */
          }

          l1_transfer_data[gas_id].release_in_progress = TRUE;

        } /* End of if dl st is valid */

        if( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].ul_st )
        {
          /* Uplink Assignment in progress */
          MSG_GERAN_MED_3_G( "UL assignment in progress fn %u st %u",
                   gl1_get_FN( gas_id ),
                   l1_transfer_data[gas_id].ul_st,
                   0 );

          l1_transfer_data[gas_id].release_in_progress = TRUE;

        } /* End of if ul st is valid */
      }   /* End of if main command is L1 TRANSFER COMMAND */

      if( l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC )
      {
        /* setup pending ul dynamic pointer */
        if( l1_transfer_data[gas_id].dynamic_tbf_ptr == &l1_transfer_data[gas_id].dynamic_tbf_1 )
        {
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_2;
        }
        else
        {
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = &l1_transfer_data[gas_id].dynamic_tbf_1;
        }
      }

      /* if there is only a downlink tbf running
       * return to IDLE
       */

      MSG_GERAN_MED_0_G("PL1: RELEASE DL TBF");
      MSG_GERAN_MED_2_G("current alloc = %d pending_alloc = %d",
              l1_transfer_data[gas_id].current_alloc_type,
              l1_transfer_data[gas_id].pending_alloc_type);

      /* check if we are using the dl cont. ta params, swap to ul if we are!!
         * dosen't matter if we are using ul cont. parameters
         */

      gpl1_gprs_swap_dl_cont_ta_to_ul(gas_id);

      /* Calculate number of downlink rrbp's which need to be scheduled before
       * we release ( Note: this may not include all dlownlink RRBP's)
       */
      gpl1_gprs_calc_num_ab_dl_rrbp(gas_id);


      /***********************************/
      /*  L1 is currently in just UL tbf */
      /***********************************/
      if(l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_ALLOC)
      {

#ifdef FEATURE_GSM_DTM
        if ( ( DTM_PS_RECONFIG_PENDING(gas_id) || GPRS_PS_RECONFIG_PENDING ) &&
             ( l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
           )
#else
        if ( (l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND) &&
             (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
           )
#endif /*FEATURE_GSM_DTM*/
        {
          /************************************************************/
          /*  There is both a pending UL and DL so just cancel the DL */
          /*  downlink allocation                                     */
          /************************************************************/

          MSG_GERAN_MED_0_G("Clear only pending DL alloaction - keep UL");

          /* Ensure any pending starting time for the DL doesn't expire before Dl is released */
          l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

          /* The pending allocation is now just for UL */
          l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;

          l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;

          /* Now tell MAC that we have cancelled the pending DL */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_DL, gas_id);

        }

      }

      /* Do we need to returnm to idle?
       * 1. Currently assigned DL TBF and are not waiting for a pending UL TBF
       *
       * 2 Currently assigned UL + DL TBF. Previously received and UL release
       *   and now received THIS DL release
       */
      if ( ( (l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC) &&
             (l1_transfer_data[gas_id].pending_alloc_type != L1_DYNAMIC_DOWN_ALLOC)
           )||
           ( (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) &&
             (l1_transfer_data[gas_id].pending_alloc_type == L1_DOWNLINK_ALLOC)
           ) ||
           ( (l1_transfer_data[gas_id].single_block_tbf_ptr != NULL ) &&
             (l1_transfer_data[gas_id].single_block_tbf_ptr->sb_ul_dl_type == SINGLE_DL_BLOCK ) &&
             (l1_transfer_data[gas_id].current_alloc_type == L1_SINGLE_BLOCK_ALLOC )
           )
         )
      {
         normal_release_transfer = TRUE;
         /* Two possible cases are handled are here          
         1.PACKET PDCH RELEASE causing TS change and yet to be connected          
         2.New DL Config received and yet to be connected*/
        if ( (l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND) &&
               (l1_transfer_data[gas_id].pending_alloc_type == L1_DOWNLINK_ALLOC) &&
               (l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC))
        {
             
             MSG_GERAN_MED_1_G("clear DL st [%d]",l1_transfer_data[gas_id].starting_time);
             
             /* Ensure any pending starting time for the dl doesn't expire before dl is released */
             l1_transfer_data[gas_id].starting_time = L1_GPRS_INVALID_FN;
             l1_transfer_data[gas_id].dl_st   = L1_GPRS_INVALID_FN;
             l1_tskisr_blk->main_command  = L1_NULL_COMMAND;
            if(gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TBF_TS_CHANGE, gas_id))
            {
              gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_TBF_TS_CHANGE, gas_id);
            }
        }
      }
        else if((l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC)&&
          (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)&&
            (l1_tskisr_blk->main_command  == L1_TRANSFER_COMMAND)&&
            (l1_transfer_data[gas_id].ul_st != L1_GPRS_INVALID_FN))
      {
           /* If there is concurrent tbf and pending UL: then release the downlink and
              * re-config with new Ul TBF asap.
              */
        uint32 FN = GSTMR_GET_FN_GERAN( gas_id);
        uint32  timeslot = 0;
        uint8  *usf_ptr = &( l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->usf_value[0] );
        uint8  l1_ul_timeslot_alloc = 0;
        uint32 tbf_starting_time ,dl_rrbp_fn,ul_rrbp_fn;

        while( timeslot < 8 )
        {
            if( *usf_ptr != GPL1_INVALID )
            {
                l1_ul_timeslot_alloc |= (0x0001 << timeslot );
            }
            ++timeslot;
            usf_ptr++;
        }

        /*Cancel TS change acitivity if anything planned as part of PTSR. 
         As part of gpl1_gprs_handle_reconfig, if low TS chnage is there 
         G2X_TA_PRI_TBF_TS_CHANGE activity will be added */

        if(gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TBF_TS_CHANGE, gas_id))
        {
          gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_TBF_TS_CHANGE, gas_id);
        }
         
        if (gpl1_gprs_lowest_tn_changed( l1_ul_timeslot_alloc , l1_get_current_timeslot(gas_id) ) )
        {
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;
        }
        else
        {
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
        }

        l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;

        l1_transfer_data[gas_id].tbf_est_type= UPLINK_TBF_ESTABLISHED;

        l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

        if(gpl1_gprs_get_ul_num_rrbp_msg(gas_id))
        {
            /*UL RRBP for assignment is present */

            ul_rrbp_fn = gpl1_gprs_get_ul_rrbp_msg_fn(gas_id);

            if(gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id))
            {
              if(release_tbf == DL_TBF_ABNORMAL)
              {
               /*DL RRBP for TBF release is present. */

              dl_rrbp_fn = gpl1_gprs_get_dl_ab_rrbp_msg_fn(gas_id);
              if (IS_FRAME_NUM_LATER_OR_EQUAL(dl_rrbp_fn,ul_rrbp_fn))
              {
                  tbf_starting_time = ADD_FN(dl_rrbp_fn,GPL1_NO_OF_FRAMES_IN_BLOCK);
              }
              else
              {
                 tbf_starting_time = ADD_FN(ul_rrbp_fn,GPL1_NO_OF_FRAMES_IN_BLOCK);
                }
              }
              else
              {
                MSG_GERAN_HIGH_0_G("DL RRBP present but TBF rel type is DL NORMAL, Delete DL RRBPs");
                gpl1_gprs_delete_dl_rrbp_entries(gas_id);
              }
            }
            else
            {
               tbf_starting_time = ADD_FN(ul_rrbp_fn,GPL1_NO_OF_FRAMES_IN_BLOCK);
            }

            gpl1_gprs_chk_blk_bndry(&tbf_starting_time);

            l1_transfer_data[gas_id].release_pending = L1_PARTIAL_RELEASE_DL;

            l1_transfer_data[gas_id].starting_time = tbf_starting_time;

            l1_transfer_data[gas_id].ul_st = tbf_starting_time;

            l1_transfer_data[gas_id].confirm_pending= SEND_IN_ISR;

            MSG_GERAN_HIGH_1_G("REL DL+ Config new UL tbf_starting_time %d", tbf_starting_time);

        }
        else if(gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id) && 
               (release_tbf == DL_TBF_ABNORMAL))
        {
            /*DL RRBP for TBF release is present. */
            MSG_GERAN_HIGH_1_G("waiting for dl rrbp's %d",gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id));

            dl_rrbp_fn = gpl1_gprs_get_dl_ab_rrbp_msg_fn(gas_id);

            tbf_starting_time = ADD_FN(dl_rrbp_fn,GPL1_NO_OF_FRAMES_IN_BLOCK);

            l1_transfer_data[gas_id].release_pending = L1_PARTIAL_RELEASE_DL;

            gpl1_gprs_chk_blk_bndry(&tbf_starting_time);

            l1_transfer_data[gas_id].starting_time = tbf_starting_time;
            l1_transfer_data[gas_id].ul_st = tbf_starting_time;

            l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
            MSG_GERAN_HIGH_1_G("REL DL+ Config new UL tbf_starting_time = %d", tbf_starting_time);
         
         }
         else
         {
             /*UL RRBP for PTR is not present. RRBO for DL release also not present. 
              Move the new resources asap.
             Tbf starting time = cur_fn+1 to do re-config asap */

            /* Re-Config will happen when TBF_start_time == cur_fn+1 , Next ISR will add one more frame*/
            tbf_starting_time = ADD_FN(FN, NPL1_FN_OFFSET*2);

            gpl1_gprs_chk_blk_bndry(&tbf_starting_time);

            if(!IS_FRAME_NUM_LATER(tbf_starting_time,l1_transfer_data[gas_id].starting_time))
            {
               l1_transfer_data[gas_id].starting_time = tbf_starting_time;
               l1_transfer_data[gas_id].ul_st = tbf_starting_time;
            }
            
            /* inform MAC that the DL has been released */
            gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_DL, gas_id);
            l1_transfer_data[gas_id].confirm_pending = SENT;
            l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
            MSG_GERAN_HIGH_1_G("REL DL+ Config new UL tbf_starting_time = FN+2 %d", tbf_starting_time);
            if(gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id))
            {
              MSG_GERAN_HIGH_0_G("DL RRBP present but TBF rel type is DL NORMAL, Delete DL RRBPs");
              gpl1_gprs_delete_dl_rrbp_entries(gas_id);
            }
        }

      }
      else if (l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
      {
      	 /* If there is concurrent tbf then release the downlink and
      	 * continue with the uplink tbf
       	 */
     

        uint32  timeslot = 0;
        uint8  *usf_ptr = &( l1_transfer_data[gas_id].dynamic_tbf_ptr->usf_value[0] );
        uint8  l1_ul_timeslot_alloc = 0;

        while( timeslot < 8 )
        {
            if( *usf_ptr != GPL1_INVALID )
            {
                l1_ul_timeslot_alloc |= (0x0001 << timeslot );
            }
            ++timeslot;
            usf_ptr++;
        }

        l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;
        l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;


        /* Copy current dynamic config into the pending structure */
        *l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = *l1_transfer_data[gas_id].dynamic_tbf_ptr;

        /* Set pending test mode to off if it's not test mode A.*/
        if(l1_transfer_data[gas_id].dynamic_tbf_ptr->test_mode != TEST_MODE_A)
        {
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->test_mode =
              TEST_MODE_OFF;
        }
        /* Set the mac mode for the benefit of the
         * reconfiguration routine.
         */
        l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->mac_mode =
            L1_MAC_MODE_INVALID;

         /*Cancel TS change acitivity (as this may be planned because of DL ts alloc change)
         if planned as part of PDCH_TS_REL because DL_TBF is to be released anyways.
         As part of gpl1_gprs_handle_reconfig, if low TS change is there (because of UL ts alloc change) 
         G2X_TA_PRI_TBF_TS_CHANGE activity will be added */

        if(gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_TBF_TS_CHANGE, gas_id))
        {
          gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_TBF_TS_CHANGE, gas_id);
        }
         
        if (gpl1_gprs_lowest_tn_changed( l1_ul_timeslot_alloc , l1_get_current_timeslot(gas_id) ) )
        {
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = TRUE;
        }
        else
        {
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap = FALSE;
        }

        /* Set starting time valid to false as we need to release ASAP */
        l1_transfer_data[gas_id].starting_time_valid = FALSE;

        /* set starting time elapsed to true as we do not want the MAC_PH_CONNECT_IND
         * to be sent when we restart transfer_mode
         */
        l1_transfer_data[gas_id].starting_time_elapsed  = TRUE;


        /* Signal to L1 ISR that a TBF reconfig is pending if there are no dl rrbp
         * messages awaiting to been txd
         */
        if (gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id) &&
           (release_tbf == DL_TBF_ABNORMAL))
        {
          MSG_GERAN_HIGH_1_G("waiting for dl rrbp's %d",gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id));
          l1_transfer_data[gas_id].release_pending = L1_PARTIAL_RELEASE_DL;

        } else
        {

#ifdef FEATURE_GSM_DTM
          /* Don't set main commands in the DTM mode */
          if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
          {
            /* reset transfer mode to start with ul tbf only */
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
          }
          else
          {
            if(l1_get_dtm_command_event(gas_id) != L1_DTM_STOP)
            {
              l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG,gas_id);
            }
          }
#else
          /* reset transfer mode to start with ul tbf only */
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif

          /* inform MAC that the DL has been released */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_DL, gas_id);
          l1_transfer_data[gas_id].confirm_pending = SENT;
          l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
          if(gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id)) 
          {
            MSG_GERAN_HIGH_0_G("DL RRBP present but TBF rel type is DL NORMAL, Delete DL RRBPs");
            gpl1_gprs_delete_dl_rrbp_entries(gas_id);
          }
        }

      }
      else if((l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC) &&
              (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC))
      {
        /* We are about to connect UL + DL TBF but the DL is now being released
         *
         * Convert pending allocation to UL TBF only when starting time elapses
         */
        /* Ensure any pending starting time for the DL doesn't expire before Dl is released */
        l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

        l1_transfer_data[gas_id].pending_alloc_type = L1_DYNAMIC_ALLOC;
        l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;

        /* inform MAC that the DL has been released */
        gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_DL, gas_id);

      }

    } /* End of case dl tbf normal/abnormal */
      break;

    /*************** UL RELEASE ORDERED *****************/

  case UL_TBF_NORMAL:
  case UL_TBF_ABNORMAL: /*lint !e616 */
    {
      MSG_GERAN_MED_0_G("PL1: RELEASE UL TBF");
      MSG_GERAN_MED_2_G("current alloc = %d pending_alloc = %d",
              l1_transfer_data[gas_id].current_alloc_type,
              l1_transfer_data[gas_id].pending_alloc_type);

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

      if( FALSE == gpl1_assess_ul_tbf_release( &( l1_transfer_data[gas_id] ), gas_id ) )
      {
         return  FALSE;
      } 
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
      gpl1_gprs_register_release_recovery_event(release_tbf,gas_id);
#endif
#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
      switch( get_queued_mac_l1_edtm_cs_rel_tbf_alloc(gas_id ) )
      {
      case  NO_TBF_PRESENT:
      case  DL_TBF_PRESENT:
        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id);
        break;

      case  UL_TBF_PRESENT:

        MSG_GERAN_MED_0_G(" Cancelled enh alloc UL TBF ");

        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id);
        break;

      case  UL_DL_TBF_PRESENT:

        MSG_GERAN_MED_0_G(" Cancelled enh alloc UL TBF, only DL remain. ");

        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id);

        break;

      default:

        MSG_GERAN_ERROR_3_G( " Unknown enh alloc tbf type %u ",
                   get_queued_mac_l1_edtm_cs_rel_tbf_alloc(gas_id ),
                   0,
                   0 );
        break;
      }
#endif

      l1_transfer_data[gas_id].release_in_progress = FALSE;

      /*  Determine if a release has been received between an assignment and
          its associated startime. If so ensure the pending alloc etc handled
          correctly.  */

#ifdef FEATURE_GSM_DTM
      if ( DTM_PS_RECONFIG_PENDING(gas_id) || GPRS_PS_RECONFIG_PENDING )
#else
      if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
#endif /*FEATURE_GSM_DTM*/
      {
        if( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].dl_st )
        {
          /* Downlink Assignment in progress */
          MSG_GERAN_MED_3_G( "DL assignment in progress fn %u st %u",
                   gl1_get_FN( gas_id ),
                   l1_transfer_data[gas_id].dl_st,
                   0 );

          l1_transfer_data[gas_id].release_in_progress = TRUE;

        } /* End of if dl st is valid */

        if( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].ul_st )
        {
          /* Uplink Assignment in progress */
          MSG_GERAN_MED_3_G( "UL assignment in progress fn %u st %u",
                   gl1_get_FN( gas_id ),
                   l1_transfer_data[gas_id].ul_st,
                   0 );

          l1_transfer_data[gas_id].release_in_progress = TRUE;
        } /* End of if ul st is valid */
      }   /* End of if main command is L1 TRANSFER COMMAND */

      /* turn off RLC UL FIFO as all data has been sucessfully sent */
      l1_transfer_data[gas_id].rlc_ul_data_available = FALSE;

      gpl1_gprs_swap_ul_cont_ta_to_dl(gas_id);

      switch( l1_transfer_data[gas_id].current_alloc_type )
      {
      case  L1_DOWNLINK_ALLOC:
      {
        /* Check if we have an UL assignment pending
         * cancel the UL starting time
         */
#ifdef FEATURE_GSM_DTM
        if ( (DTM_PS_RECONFIG_PENDING(gas_id) || GPRS_PS_RECONFIG_PENDING) &&
             (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
           )
#else
        if ( (l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND) &&
             (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
           )
#endif /*FEATURE_GSM_DTM*/
        {

          /************************************************************/
          /*  There is both a pending UL and DL so just cancel the UL */
          /*  allocation                                              */
          /************************************************************/

          MSG_GERAN_MED_0_G("Clear only pending UL alloaction - keep DL");

          /* Ensure any pending starting time for the DL doesn't expire before Dl is released */
          l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

          /* The pending allocation is now just for DL */
          l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;

          l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

          /* Now tell MAC that we have cancelled the pending PUA */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_UL,gas_id);

        }

      } /* End of case L1_DOWNLINK_ALLOC */
        break;

      case  L1_DYNAMIC_ALLOC:
      case  L1_SINGLE_BLOCK_ALLOC:
      {
        /* for all single_block_alloc combinations */
        normal_release_transfer = TRUE;

        /* If we only have ul tbf and we are asked to release it then
         * we need to return to idle mode
         */

        /* do we have a pending starting time for a ul tbf ? */
#ifdef FEATURE_GSM_DTM
        if ( ((DTM_PS_RECONFIG_PENDING(gas_id) || GPRS_PS_RECONFIG_PENDING) &&
              (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_ALLOC))
              ||
             ((l1_transfer_data[gas_id].tbf_est_type == UPLINK_TBF_ESTABLISHED) &&
              (!l1_transfer_data[gas_id].starting_time_elapsed)
             )
           )
#else
        if ( ( (l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND) &&
               (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_ALLOC)
             ) ||
             ( (l1_transfer_data[gas_id].tbf_est_type == UPLINK_TBF_ESTABLISHED) &&
               (!l1_transfer_data[gas_id].starting_time_elapsed)
             )
           )
#endif /*FEATURE_GSM_DTM*/
        {
          MSG_GERAN_MED_1_G("clear st [%d]",gl1_get_FN( gas_id ));

          /* Ensure any pending starting time for the ul doesn't expire before ul is released */
          l1_transfer_data[gas_id].starting_time = L1_GPRS_INVALID_FN;
          l1_transfer_data[gas_id].ul_st         = L1_GPRS_INVALID_FN;
          l1_tskisr_blk->main_command    = L1_NULL_COMMAND;

          if( L1_SINGLE_BLOCK_ALLOC == l1_transfer_data[gas_id].current_alloc_type )
          {
            /* Set starting time elapsed to true as we do not want the MAC_PH_CONNECT_IND
             * to be sent before the uplink release has completed properly.
             */
            l1_transfer_data[gas_id].starting_time_elapsed = TRUE;
          }
        }

#ifdef FEATURE_GSM_DTM
        else if ( (DTM_PS_RECONFIG_PENDING(gas_id) || GPRS_PS_RECONFIG_PENDING) &&
                  (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
                )
#else
        else if ( (l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND) &&
                  (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
                )
#endif /*FEATURE_GSM_DTM*/
        {
          /************************************************************/
          /*  There is both a pending UL and DL so just cancel the UL */
          /*  allocation                                              */
          /************************************************************/
          /******************************************************************************************/
          /* CR63848                                                                                */
          /* The pending allocation should be kept as L1_DYNAMIC_DOWN_ALLOC. This is                */
          /* because it is referenced later in the releasing code in gpl1_gprs_transfer_send_ftn()  */
          /* to deicide how to proceed when releasing the uplink with a pending downlink allocation */
          /******************************************************************************************/

          MSG_GERAN_MED_0_G("Clear only pending UL alloaction - keep DL");

          /* Ensure any pending starting time for the DL doesn't expire before Dl is released */
          l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

          l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

          /* If no poll , example Packet TBF REL without poll , send REL_CNF
          */
          if ( ! gpl1_gprs_get_ul_num_rrbp_msg(gas_id))
          {
            /* Now tell MAC that we have cancelled the pending PUA/PTR-for UL */
             gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_UL, gas_id);

             /* was set TRUE , at the beginning , so make it FALSE */
             normal_release_transfer = FALSE;
          }
          else // normal_release_transfer is still TRUE.
          {
            uint32 pca_ul_msg_frame_num;
            uint32 ul_rel_conf_frame_num;

            /* if the pending alloc is both UL + DL and there is a PCA UL FAI for UL
               is pending then we need to adjust the starting time of DL such that connect
               ind goes to gmac after the ul release confirm */
            if(L1_DYNAMIC_DOWN_ALLOC == l1_transfer_data[gas_id].pending_alloc_type)
            {
              if(l1_transfer_data[gas_id].dl_st != L1_GPRS_INVALID_FN)
              {
                transfer_data_T          *transfer_data_ptr =
                                             l1_tskisr_blk->current_params.L1Data.pTransfer_data;

                transfer_data_ptr->starting_time = l1_transfer_data[gas_id].dl_st;
                pca_ul_msg_frame_num = gpl1_gprs_get_ul_rrbp_msg_fn(gas_id);
                MSG_GERAN_MED_2_G("pca_ul_msg_frame_num %d dl_st %d",pca_ul_msg_frame_num,l1_transfer_data[gas_id].dl_st);
                if(pca_ul_msg_frame_num != L1_GPRS_INVALID_FN)
                {
                  /* calculate the ul release confirmation frame no */
                  ul_rel_conf_frame_num = ((pca_ul_msg_frame_num + (GPL1_WAITING_PERIOD_FOR_UL_REL_CNF )) % FRAMES_IN_HYPERFRAME);

                  if( (ul_rel_conf_frame_num == l1_transfer_data[gas_id].dl_st) ||
                      (IS_FRAME_NUM_LATER(ul_rel_conf_frame_num,l1_transfer_data[gas_id].dl_st)))
                  {

                    transfer_data_ptr->starting_time =  ((ul_rel_conf_frame_num + GPL1_NO_OF_FRAMES_IN_BLOCK) % FRAMES_IN_HYPERFRAME);

                    gpl1_gprs_chk_blk_bndry(&transfer_data_ptr->starting_time);

                    /* update the dl_st copy */
                    l1_transfer_data[gas_id].dl_st = transfer_data_ptr->starting_time;

                    MSG_GERAN_MED_2_G( "Adj the dl st after blk_bndry to %d to send pca at %d ",
                             l1_transfer_data[gas_id].dl_st,pca_ul_msg_frame_num);

                    if (l1_transfer_data[gas_id].pending_dl_tbf_ptr)
                    {
                      l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time= l1_transfer_data[gas_id].dl_st;
                    }

                  }
                }
              } // if(l1_transfer_data[gas_id].dl_st != L1_GPRS_INVALID_FN)
            }   // if(L1_DYNAMIC_DOWN_ALLOC == l1_transfer_data[gas_id].pending_alloc_type)
          }  // else
        }
      } /* End of case dynamic alloc or single alloc */
        break;

      case  L1_DYNAMIC_DOWN_ALLOC:
      {
        if( L1_DYNAMIC_ALLOC == l1_transfer_data[gas_id].pending_alloc_type )
        {
          normal_release_transfer = TRUE;
        }
        else
        {
          uint32 pca_ul_msg_frame_num;
          uint32 ul_rel_conf_frame_num;
          const uint8  num_ul_rrbp_msgs = gpl1_gprs_get_ul_num_rrbp_msg(gas_id );

          /* if the pending alloc is both UL + DL and there is a PCA UL FAI for UL
             is pending then we need to adjust the starting time of DL such that connect
             ind goes to gmac after the ul release confirm */
          if(L1_DYNAMIC_DOWN_ALLOC == l1_transfer_data[gas_id].pending_alloc_type)
          {
            if(l1_transfer_data[gas_id].dl_st != L1_GPRS_INVALID_FN)
            {
              transfer_data_T          *transfer_data_ptr =
                                           l1_tskisr_blk->current_params.L1Data.pTransfer_data;

              l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
              l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;
              transfer_data_ptr->starting_time = l1_transfer_data[gas_id].dl_st;

              pca_ul_msg_frame_num = gpl1_gprs_get_ul_rrbp_msg_fn(gas_id);

              MSG_GERAN_MED_2_G("pca_ul_msg_frame_num %d dl_st %d",pca_ul_msg_frame_num,l1_transfer_data[gas_id].dl_st);
              if(pca_ul_msg_frame_num != L1_GPRS_INVALID_FN)
              {
                 /* calculate the ul release confirmation frame no */
                ul_rel_conf_frame_num = ((pca_ul_msg_frame_num + (GPL1_WAITING_PERIOD_FOR_UL_REL_CNF )) % FRAMES_IN_HYPERFRAME);
                if( (ul_rel_conf_frame_num == l1_transfer_data[gas_id].dl_st) ||
                    (IS_FRAME_NUM_LATER(ul_rel_conf_frame_num,l1_transfer_data[gas_id].dl_st)))
                {
                 transfer_data_T          *transfer_data_ptr =
                                           l1_tskisr_blk->current_params.L1Data.pTransfer_data;

                  transfer_data_ptr->starting_time =  ((ul_rel_conf_frame_num + GPL1_NO_OF_FRAMES_IN_BLOCK) % FRAMES_IN_HYPERFRAME);

                  gpl1_gprs_chk_blk_bndry(&transfer_data_ptr->starting_time);

                  /* update the dl_st copy */
                  l1_transfer_data[gas_id].dl_st = transfer_data_ptr->starting_time;

                  MSG_GERAN_MED_2_G( "Adj the dl st after blk_bndry to %d to send pca at %d ",
                           l1_transfer_data[gas_id].dl_st,pca_ul_msg_frame_num);


                  if (l1_transfer_data[gas_id].pending_dl_tbf_ptr)
                  {
                    l1_transfer_data[gas_id].pending_dl_tbf_ptr->tbf_start_time = l1_transfer_data[gas_id].dl_st;
                  }                  

                }
              }
            }
          }

          /* If there is concurrent tbf then release the uplink and
           * continue with the downlink tbf
           */

          /* Clear out the pending uplink assignment
           * Do not alter the current/pending alloc type variables here
           * since they are checked later in this function.
           */
          switch( l1_transfer_data[gas_id].tbf_est_type )
          {
          case  UPLINK_DOWNLINK_TBF_ESTABLISHED:

            /* Reset the uplink start time */
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

            /* Ensure the connect indication message does
             * not mention the uplink TBF to MAC.
             */
            l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

            /* Print out an F3 message */
            MSG_GERAN_MED_0_G( "UL assignment connect ind removed, DL remaining");

            break;

          default:
            break;
          }

          /* If there is a queued packet timeslot reconfigure then
           * ensure it is correctly connected by not changing the
           * the pending allocation type.
           */
          if( !(  ( FALSE != l1_transfer_data[gas_id].release_in_progress )
                &&( L1_DYNAMIC_DOWN_ALLOC == l1_transfer_data[gas_id].pending_alloc_type )
                &&( 0 != num_ul_rrbp_msgs )
                &&( UPLINK_DOWNLINK_TBF_ESTABLISHED == l1_transfer_data[gas_id].tbf_est_type )
               )
            )
          {
            l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
          }

          if (l1_transfer_data[gas_id].dl_st == L1_GPRS_INVALID_FN)
          {
            /*  If dl_st is set then we are waiting for a downlink assignment to be
                actioned. By not changing the l1_transfer_data[gas_id].starting time flags
                the release confirmation will be delayed until the downlink
                assignment starting time. */

            /* Warning: Nulling the main command would also remove any
             * uplink assignment due in the future.
             */
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;

            /* Reset the ul start time */
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

            /* Set starting time valid to false as we need to release ASAP */
            l1_transfer_data[gas_id].starting_time_valid = FALSE;

            /* set starting time elapsed to true as we do not want the MAC_PH_CONNECT_IND
            * to be sent when we restart transfer_mode
            */
            l1_transfer_data[gas_id].starting_time_elapsed  = TRUE;
          }

          /* Signal to L1 ISR that a TBF reconfig is pending if there are no ul rrbp
           * messages awaiting to been txd
           *
           * If there are rrbp awaiting txing on the ul the pending reconfig will be
           * issued once the last dl rrbp has been txd
           */
          MSG_GERAN_MED_3_G(" ul rrbp = %d fn %d [%d] ",
                    num_ul_rrbp_msgs,
                    gpl1_gprs_get_rrbp_frame(0,gas_id),
                    gl1_get_FN( gas_id  ) );

          if( 0 != num_ul_rrbp_msgs )
          {
            l1_transfer_data[gas_id].release_pending = L1_PARTIAL_RELEASE_UL;

          } else
          {
#ifdef FEATURE_GSM_DTM
            /* Don't set main commands in the DTM mode */
            if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
            {
                /* reset transfer mode to start with ul tbf only */
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
            }
            else
            {
              l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG,gas_id);
            }
#else
            /* reset transfer mode to start with ul tbf only */
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif

            /* inform MAC that the UL has been released */
            gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_UL, gas_id);
            l1_transfer_data[gas_id].confirm_pending = SENT;
            l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
          }

        } /* End of else pending alloc is not dynamic alloc */
      }   /* End of case dynamic downlink */
        break;

      default:
        /* No action required here */
        break;
      } /* End of switch on current alloc type */


      break;
    } /* End of case ul release normal/abnormal */

    /***************UL & DL RELEASE ORDERED *****************/
  case UL_AND_DL_TBF_NORMAL:
  case UL_AND_DL_TBF_ABNORMAL: /*lint !e616 */
    {
     /* Check here for extra ul_dl release messages
      * and ignore them.
      */
      
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER 
	  #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/ 
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

      switch( l1_transfer_data[gas_id].release_pending )
      {
      case  L1_FULL_RELEASE_UL_DL:

       /* if already in release but ask to release with with no rrbp , perform release.*/
       if ( ! (gpl1_gprs_get_ul_num_rrbp_msg(gas_id) + gpl1_gprs_get_dl_num_rrbp_msg(gas_id)) )
       {
         MSG_GERAN_HIGH_0_G( "ul / dl release duplicate honoured as NO RRBP");
       }
       else
       {
         /*  ul release is already in progress, report an error and
          * ignore the msg.
         */
         MSG_GERAN_ERROR_0_G( " ul dl release ignored, release in progress ");

         /* Exit function */
         return  FALSE;
       }

      default:
        /* No action required here */
        break;
      }
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
       gpl1_gprs_register_release_recovery_event(release_tbf,gas_id);
#endif

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
      switch( get_queued_mac_l1_edtm_cs_rel_tbf_alloc(gas_id ) )
      {
      case  NO_TBF_PRESENT:
        break;

      case  DL_TBF_PRESENT:

        MSG_GERAN_MED_0_G(" Cancelled enh alloc DL TBF ");

        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id) ;
        break;

      case  UL_TBF_PRESENT:

        MSG_GERAN_MED_0_G(" Cancelled enh alloc UL TBF ");

        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id);
        break;

      case  UL_DL_TBF_PRESENT:

        MSG_GERAN_MED_0_G(" Cancelled enh alloc UL & DL TBF ");

        set_queued_mac_l1_edtm_cs_rel_tbf_alloc(NO_TBF_PRESENT,gas_id);
        break;

      default:

        MSG_GERAN_ERROR_3_G( " Unknown enh alloc tbf type %u ",
                   get_queued_mac_l1_edtm_cs_rel_tbf_alloc(gas_id ),
                   0,
                   0 );
        break;
      }
#endif

         l1_transfer_data[gas_id].release_in_progress = FALSE;

        /*  Determine if a release has been received between an assignment and
            its associated startime. If so ensure the pending alloc etc handled
            correctly.  */

#ifdef FEATURE_GSM_DTM
        if ( DTM_PS_RECONFIG_PENDING(gas_id) || GPRS_PS_RECONFIG_PENDING )
#else
        if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
#endif /*FEATURE_GSM_DTM*/
        {
          if( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].dl_st )
          {
            /* Downlink Assignment in progress */
            MSG_GERAN_MED_3_G( "DL assignment in progress fn %u st %u",
                     gl1_get_FN( gas_id ),
                     l1_transfer_data[gas_id].dl_st,
                     0 );

            l1_transfer_data[gas_id].release_in_progress = TRUE;
            l1_transfer_data[gas_id].starting_time_elapsed = TRUE;
			
          } /* End of if dl st is valid */

          if( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].ul_st )
          {
            /* Uplink Assignment in progress */
            MSG_GERAN_MED_3_G( "UL assignment in progress fn %u st %u",
                     gl1_get_FN( gas_id ),
                     l1_transfer_data[gas_id].ul_st,
                     0 );

            l1_transfer_data[gas_id].release_in_progress = TRUE;
            l1_transfer_data[gas_id].starting_time_elapsed = TRUE;

          } /* End of if ul st is valid */
        }   /* End of if main command is L1 TRANSFER COMMAND */

      /* turn off RLC UL FIFO as all data has been sucessfully sent */
      l1_transfer_data[gas_id].rlc_ul_data_available = FALSE;
      /* do we have a pending starting time for a ul or dl tbf ? */

      MSG_GERAN_MED_1_G("clear st [%d]",gl1_get_FN( gas_id ));
      /* Ensure any pending starting time for the ul doesn't expire before ul is released */
      l1_transfer_data[gas_id].starting_time = L1_GPRS_INVALID_FN;
      l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
      l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

      /* Calculate number of downlink rrbp's which need to be scheduled before
       * we release ( Note: this may not include all downlink RRBP's)
       */

      gpl1_gprs_calc_num_ab_dl_rrbp(gas_id);

      /******************************************************************************************/
      /* CR63848  - extended for CR 216148 , as release can be processed appropriately.         */
      /* The pending allocation should be kept as it is .                                       */
      /* because it is referenced later in the releasing code in gpl1_gprs_transfer_send_ftn()  */
      /* to deicide how to proceed when releasing the uplink with a pending downlink allocation */
      /******************************************************************************************/

      l1_tskisr_blk->main_command = L1_NULL_COMMAND;

      /*Reset the Wait For PCA FN/Valid variables.to ignore prior waiting RRBP*/
      l1_transfer_data[gas_id].wait_for_pca_fn = L1_GPRS_INVALID_FN;
      l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;


      MSG_GERAN_MED_0_G("PL1: RELEASE UL AND DL TBF");
      normal_release_transfer = TRUE;

      break;
    }


  default:

    MSG_GERAN_ERROR_0_G("Incorrect release cause");

    break;
  }

  if (normal_release_transfer)
  {
    /********************************************************
     **** need to check if there are any pending
     **** RRBP's. If there are then delay release until
     **** they have been sent
     ****
     ********************************************************/

    switch (release_tbf)
    {
    case UL_TBF_NORMAL:
    case UL_TBF_ABNORMAL:
      {
        /* Checkif there are any ul rrbp's */
        if (gpl1_gprs_get_ul_num_rrbp_msg(gas_id))
        {
          /**** CR 64279 ***/

          /* Because release_pending is always set to L1_NO_RELEASE
           * in l1_isr.c if we have a DL release pending due to
           * outstanding ab_dl_rrbp count then
           *
           * 1. Cancel the main_command TRANSFER_COMMAND event
           * 2. Release confirm the DL to MAC
           * 3. Return to Idle when PCA_FAI has been txd
           */
          if(l1_transfer_data[gas_id].release_pending == L1_PARTIAL_RELEASE_DL)
          {

            /* Inform MAC that the UL has been released */
            gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_DL, gas_id);
            l1_transfer_data[gas_id].confirm_pending = SENT;
            l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;

            l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;

            MSG_GERAN_HIGH_1_G(" Cancel wait for DL PCA w Ul REL [%d]",gl1_get_FN( gas_id ));

          }

          l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL;
          MSG_GERAN_MED_2_G("pdch rel pending UL [%d] [%d] ",gl1_get_FN( gas_id ),frame_counters[gas_id].FNmod52);

        } else
        {
          MSG_GERAN_MED_2_G("pdch return idle UL[%d] [%d]",gl1_get_FN( gas_id ),frame_counters[gas_id].FNmod52);

          /* Because release_pending is always set to L1_NO_RELEASE
           * in l1_isr.c if we have a DL release pending due to
           * outstanding ab_dl_rrbp count then
           *
           * 1. Cancel the main_command TRANSFER_COMMAND event
           * 2. Release confirm the DL to MAC
           * 3. Return to Idle when PCA_FAI has been txd
           */
          if(l1_transfer_data[gas_id].release_pending == L1_PARTIAL_RELEASE_DL)
          {
            MSG_GERAN_HIGH_2_G(" Cancel wait for DL PCA w Ul REL [%d] wait_for_pca_valid %d",
                       gl1_get_FN( gas_id ),l1_transfer_data[gas_id].wait_for_pca_valid );

            /* Inform MAC that the UL has been released */
            gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_PARTIAL_RELEASE_DL, gas_id);
            l1_transfer_data[gas_id].confirm_pending = SENT;
            l1_transfer_data[gas_id].wait_for_pca_valid = FALSE;

            l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          }

          /* set release type for release confirm which is sent we
           * get back to IDLE mode
           */
          l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL;

          /* Ensure that the release confirm will be sent */
          l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

#ifdef FEATURE_GSM_DTM
          /* We don't want to kill the SCE on transitions back to dedicated */
          if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
#endif
          {
            l1_sc_stop(gas_id);
          }

          return_to_idle = TRUE;

        }

        break;
      }

    case DL_TBF_NORMAL:
    case DL_TBF_ABNORMAL:
      {
        /* Check if there any pending dl rrbp's  */
        if (((l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) &&
             (l1_transfer_data[gas_id].pending_alloc_type == L1_DOWNLINK_ALLOC)))
        {
          /* We have previously received an ul release and are awiting for the PCA
           * on the UL to be relelased until we move to DL, but a DL release has
           * been ordered as T3192 has expired
           */
          /* inform MAC that the DL has been released */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(L1_FULL_RELEASE_DL, gas_id);

          l1_transfer_data[gas_id].release_pending    = L1_FULL_RELEASE_UL;

          /*If the main command is a transfer command, L1 will be put into transfer reconfig next frame, so don't need to clear the pending_alloc here*/
          if(l1_tskisr_blk->main_command != L1_TRANSFER_COMMAND)
          {
            l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
          }

          if (!gpl1_gprs_get_ul_num_rrbp_msg(gas_id))
          {
            /* clear pending allocation and main command in case there is a race condition between
             * this downlink release and a previous uplink release
             */
            l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;

            MSG_GERAN_HIGH_2_G("pdch return idle UL+DL[%d] [%d]",gl1_get_FN( gas_id ),frame_counters[gas_id].FNmod52);

            /* inform MAC that the UL has been released */
            l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL;

            /* Ensure that the release confirm will be sent */
            l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

#ifdef FEATURE_GSM_DTM
            /* We don't want to kill the SCE on transitions back to dedicated */
            if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
#endif
            {
              l1_sc_stop(gas_id);
            }

            return_to_idle = TRUE;

          }

        } else if( (l1_transfer_data[gas_id].current_alloc_type == L1_SINGLE_BLOCK_ALLOC) &&
                   (l1_transfer_data[gas_id].pending_alloc_type == L1_NULL_ALLOC))
        {
          /* No UL TBF Present, so return back to IDLE when we have sent all
           * important ul rrbp's
           */
          if (gpl1_gprs_get_ul_num_rrbp_msg(gas_id))
          {
            /* set release type for release confirm which is sent we
             * get back to IDLE mode
             */
            l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_DL;

            MSG_GERAN_MED_2_G("pdch rel pending DL_SINGLE_BLOCK rrbp present[%d] [%d] ",gpl1_gprs_get_ul_num_rrbp_msg(gas_id),gl1_get_FN( gas_id ));

          } else
          {
            MSG_GERAN_MED_2_G("pdch return idle DL_SINGLE_BLOCK [%d] [%d]",gl1_get_FN( gas_id ),frame_counters[gas_id].FNmod52);

            /* set release type for release confirm which is sent we
             * get back to IDLE mode
             */
            l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_DL;

            /* Ensure that the release confirm will be sent */
            l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

#ifdef FEATURE_GSM_DTM
            /* We don't want to kill the SCE on transitions back to dedicated */
            if ( (l1_tskisr_blk->l1_state != L1_DTM_MODE) )
#endif
            {
              /* No need to stop SCE as SCE is not running during DL_SINGLE_BLOCK */
              return_to_idle = TRUE;
            }
          }
        } else
        {

          /* No UL TBF Present, so return back to IDLE when we have sent all
           * important dl rrbp's for any messages polled or blocks with FAI set.
           */
          if (gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id) )
          {
            /* set release type for release confirm which is sent we
             * get back to IDLE mode
             */
            l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_DL;

            MSG_GERAN_MED_2_G("pdch rel pending DL [%d] [%d] ",gpl1_gprs_get_num_rrbp_msg(gas_id),gl1_get_FN( gas_id ));

          } else
          {

            MSG_GERAN_MED_2_G("pdch return idle DL[%d] [%d]",gl1_get_FN( gas_id ),frame_counters[gas_id].FNmod52);

            /* set release type for release confirm which is sent we
             * get back to IDLE mode
             */
            l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_DL;

            /* Ensure that the release confirm will be sent */
            l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

            /* Set starting time valid to false as we need to release ASAP */
            l1_transfer_data[gas_id].starting_time_valid = FALSE;

            /* set starting time elapsed to true as we do not want the MAC_PH_CONNECT_IND
            * to be sent when we restart transfer_mode
            */
            l1_transfer_data[gas_id].starting_time_elapsed  = TRUE;

#ifdef FEATURE_GSM_DTM
            /* We don't want to kill the SCE on transitions back to dedicated */
            if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
#endif
            {
              l1_sc_stop(gas_id);
            }

            return_to_idle = TRUE;
          }
        }

        break;
      } /* End of case statement */

    case UL_AND_DL_TBF_NORMAL:
    case UL_AND_DL_TBF_ABNORMAL:
      {
        /* Check if there are any ul rrbp's */
        /* dl rrbp only related to msg specific and FAI set */
        if ( (gpl1_gprs_get_ul_num_rrbp_msg(gas_id)) +
             (gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id))
             +
             (gpl1_gprs_get_matching_rrbp_msg_type(PCA_SRB, gas_id) < SIZE_OF_RRBP_BUFF)
            )
        {
          l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL_DL;

          MSG_GERAN_MED_3_G("pdch rel pending UL+DL [%d] ul %d dl %d ",gl1_get_FN( gas_id ),gpl1_gprs_get_ul_num_rrbp_msg(gas_id),gpl1_gprs_get_num_rrbp_msg(gas_id));

        } else
        {
          MSG_GERAN_MED_3_G("pdch ret idle UL+DL [%d] ul %d dl %d ",gl1_get_FN( gas_id ),gpl1_gprs_get_ul_num_rrbp_msg(gas_id),gpl1_gprs_get_num_rrbp_msg(gas_id));

          /* set release type for release confirm which is sent we
           * get back to IDLE mode
           */
          l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL_DL;

          /* Ensure that the release confirm will be sent */
          l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;

          /* Set starting time valid to false as we need to release ASAP */
          l1_transfer_data[gas_id].starting_time_valid = FALSE;

          /* set starting time elapsed to true as we do not want the MAC_PH_CONNECT_IND
          * to be sent when we restart transfer_mode
          */
          l1_transfer_data[gas_id].starting_time_elapsed  = TRUE;

#ifdef FEATURE_GSM_DTM
          /* We don't want to kill the SCE on transitions back to dedicated */
          if ( l1_tskisr_blk->l1_state != L1_DTM_MODE )
#endif
          {
            l1_sc_stop(gas_id);
          }

          return_to_idle = TRUE;
        }

        break;

      } /* end of case */

    default:
      MSG_GERAN_ERROR_1_G("Invalid release cause %d",release_tbf);

      break;
    } /* end of switch */

  } /* end of if (normal_release_transfer) */

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
   if(return_to_idle == TRUE)
   {
     gpl1_gprs_reset_tuneaway_and_ncell_data(gas_id);
   }
#endif /* FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

  return return_to_idle;

}

/*===========================================================================

FUNCTION  L1_SEND_L1_MAC_FREQ_ASS_CONF

DESCRIPTION
  Creates the above named message message and sends to the GS_QUEUE_MAC
  queue.

DEPENDENCIES
  error

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_l1_mac_freq_ass_conf(byte error,tbf_freq_cnf_t tbf_type, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
      return;
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_L1_MAC_FREQ_ASS_CNF;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      if (error)
      {
        gplt_cmd_ptr->mac_freq_ass_conf.param.band_supported = FALSE;
      }
      else
      {
        gplt_cmd_ptr->mac_freq_ass_conf.param.band_supported = TRUE;
      }
      gplt_cmd_ptr->mac_freq_ass_conf.param.tbf_freq_cnf = tbf_type;

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_L1_MAC_FREQ_ASS_CNF");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
  /* Locals */

  l1_mac_sig_t            message_buffer,*message;


  /* Align the pointers */

  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = L1_MAC_FREQ_ASS_CNF;


  /* fill the data part of the message */
  /* if an error has been detected set band supported
   * as false otherwise set as TRUE
   */
  if (error)
    message->msg.tbf_freq_ass_cnf.band_supported = FALSE;
  else
    message->msg.tbf_freq_ass_cnf.band_supported = TRUE;

  /* return tbf type which was received in the freq config msg */
  message->msg.tbf_freq_ass_cnf.tbf_freq_cnf = tbf_type;
  message->msg.tbf_freq_ass_cnf.gas_id = gas_id;

  /* Send the message, NULL used for MAC_PH_DATA_IND */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);
#endif /* FEATURE_GL1_GPLT*/
} /* End of L1_send_l1_mac_freq_ass_conf */


/*===========================================================================

FUNCTION gpl1_gprs_check_frequency_params

DESCRIPTION
  This function checks for invalid frequencies not supprted by the hardware.
DEPENDENCIES
  None

RETURN VALUE
  TRUE   : if an ivalid frequency has been detected.
  FALSE  : if all frequencies are valid.

SIDE EFFECTS
  None

===========================================================================*/

boolean gpl1_gprs_check_frequency_params(
                                        frequency_list_T *frequency_list_ptr,
                                        boolean hopping_flag, gas_id_t gas_id
                                        )
{

  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  uint8 ma_list_length,i;
  ARFCN_T assigned_arfcn;
  boolean error;

  if( NULL == frequency_list_ptr )
  {
    MSG_GERAN_ERROR_0_G( " NULL frequency_list_ptr ");

    return  TRUE;
  }

  /* if Frequency Hopping is not used */
  if (!hopping_flag)
  {
    assigned_arfcn = frequency_list_ptr->channel[0];
    geran_eng_mode_data_write(ENG_MODE_DED_OR_PDCH_ARFCN, &(frequency_list_ptr->channel[0]), TRUE, gas_id);

    /* Check that ARFCN is a valid for EGSM or DCS bands
     * i.e. is within 0 -> 124 975 -> 1023 for EGSM
     * or within 512 -> 885 for DCS
     */
    if ( (assigned_arfcn.num <= EGSM_MAX_ARFCN_LOWER) ||
         ((assigned_arfcn.num >= EGSM_MIN_ARFCN_UPPER)  && (assigned_arfcn.num <= EGSM_MAX_ARFCN_UPPER)) ||
         ((assigned_arfcn.num >= DCS_BASE_ARFCN) && (assigned_arfcn.num < (DCS_BASE_ARFCN + DCS_MAXNO_ARFCNS)))
         ||
         ((assigned_arfcn.num >= PCS_BASE_ARFCN) && (assigned_arfcn.num < (PCS_BASE_ARFCN + PCS_MAXNO_ARFCNS))) ||
         ((assigned_arfcn.num >= GSM_850_BASE_ARFCN) && (assigned_arfcn.num < (GSM_850_BASE_ARFCN + GSM_850_MAXNO_ARFCNS)))
       )
    {
      /* Frequency is within range therefor no error */
      error = FALSE;
      MSG_GERAN_LOW_1_G("freq [%d]",assigned_arfcn.num);
    } else
    {
      /* Frequency is not in range therefore report error */
      error = TRUE;
    }

  } else
  {

    /*
   * Check for invalid Hopping frequencies( i.e. those not in supported rf band) in MA list
   */
    /* intialise while vars */
    i=0;
    error=FALSE;

    /* Setup the length of the MA list*/
    ma_list_length = frequency_list_ptr->no_of_items;
    while (error == FALSE)
    {
      assigned_arfcn = frequency_list_ptr->channel[i];


      /* Check that ARFCN is a valid for EGSM or DCS bands
        * i.e. is within 0 -> 124 975 -> 1023 for EGSM
        * or within 512 -> 885 for DCS
      */
      if ( (assigned_arfcn.num <= EGSM_MAX_ARFCN_LOWER) ||
           ((assigned_arfcn.num >= EGSM_MIN_ARFCN_UPPER)  && (assigned_arfcn.num <= EGSM_MAX_ARFCN_UPPER)) ||
           ((assigned_arfcn.num >= DCS_BASE_ARFCN) && (assigned_arfcn.num < (DCS_BASE_ARFCN + DCS_MAXNO_ARFCNS)))
           ||
           ((assigned_arfcn.num >= PCS_BASE_ARFCN) && (assigned_arfcn.num < (PCS_BASE_ARFCN + PCS_MAXNO_ARFCNS))) ||
           ((assigned_arfcn.num >= GSM_850_BASE_ARFCN) && (assigned_arfcn.num < (GSM_850_BASE_ARFCN + GSM_850_MAXNO_ARFCNS)))
         )
      {
        /* Frequency is within range therefor no error */
        error = FALSE;
        MSG_GERAN_LOW_1_G("freq [%d]",assigned_arfcn.num);
      } else
      {
        /* Frequency is not in range therefore report error */
        error = TRUE;
      }
      /* increment to next frequency in hopping list */
      i++;

      /* If we have reached the end of the frequency list then stop */
      if (i >= ma_list_length)
        break;
    } /* End of While */


  } /* end of else */

  if (!error)
  {
    if(hopping_flag)
    {
      geran_eng_mode_data_write(ENG_MODE_HOPPING_FREQUENCY_LIST,frequency_list_ptr,TRUE, gas_id);

    }

    {
      eng_mode_hopping_status_t eng_mode_hopping = {0};

      eng_mode_hopping.hopping_status = hopping_flag;

      geran_eng_mode_data_write(ENG_MODE_HOPPING_STATUS, (void *)&eng_mode_hopping, TRUE, gas_id);
    }

    /*  If the frequency is valid.  */
    /*  Set the serving cell band dependant gamma value to the correct value. */
    switch (assigned_arfcn.band)
    {
    case SYS_BAND_EGSM_900:
      /*  GSM frequencies so gamma = 39dBm  */
      gprs_serv_cell_meas_ptr->gamma_gsm = GAMMA_GSM900;
      gprs_serv_cell_meas_ptr->ul_power_band      = SYS_BAND_EGSM_900;
      break;
    case SYS_BAND_PGSM_900:
      /*  GSM frequencies so gamma = 39dBm  */
      gprs_serv_cell_meas_ptr->gamma_gsm = GAMMA_GSM900;
      gprs_serv_cell_meas_ptr->ul_power_band      = SYS_BAND_PGSM_900;
      break;
    case SYS_BAND_DCS_1800:
      /*  DCS frequencies so gamma = 36dBm  */
      gprs_serv_cell_meas_ptr->gamma_gsm = GAMMA_GSM1800;
      gprs_serv_cell_meas_ptr->ul_power_band      = SYS_BAND_DCS_1800;
      break;
    case SYS_BAND_PCS_1900:
      /*  PCS frequencies so gamma = 36dBm  */
      gprs_serv_cell_meas_ptr->gamma_gsm = GAMMA_GSM1900;
      gprs_serv_cell_meas_ptr->ul_power_band      = SYS_BAND_PCS_1900;
      break;
    case SYS_BAND_CELL_850:
      /*  GSM 850 frequencies so gamma = 39dBm  */
      gprs_serv_cell_meas_ptr->gamma_gsm = GAMMA_GSM850;
      gprs_serv_cell_meas_ptr->ul_power_band      = SYS_BAND_CELL_850;
      break;

    case INVALID_BAND:
    default:

      MSG_GERAN_ERROR_1_G("incorrect band %d",assigned_arfcn.band);
      break;

    }
  }

  return(error);
}

#ifdef FEATURE_G2X_TUNEAWAY
void gpl1_gprs_delete_rrbp_with_FAI(gas_id_t gas_id)
{
  if( grlc_dl_t3192_status( gas_id ) == TMR_ACTIVE )
  {
    if(l1_transfer_data[gas_id].dl_tbf_ptr != NULL )
    {
      uint8  rrbp_index;

      if( l1_transfer_data[gas_id].dl_tbf_ptr->rlc_mode == RLC_ACK )
      {
        do
        {
           /* Do we have any pending PDAN_MSG_WITH_FAI RRBP's to be sent */
            rrbp_index = gpl1_gprs_get_matching_rrbp_msg_type(PDAN_MSG_WITH_FAI, gas_id);
           if (rrbp_index < SIZE_OF_RRBP_BUFF)
           {
             /* We have found a PDAN_MSG_WITH_FAI */
              gpl1_gprs_delete_rrbp_entry(rrbp_index, gas_id);
              MSG_GERAN_MED_0_G("RLC ACK delete_rrbp_entry");
          }
        }
        while(rrbp_index < SIZE_OF_RRBP_BUFF);

        if(gpl1_gprs_get_serv_cell_head_ptr(gas_id) != NULL )
        {
          MSG_GERAN_HIGH_0_G("EPDAN_REQ pending in queue ");
        }
        gpl1_egprs_empty_qual_meas_queue(gas_id);
      }
      else if( l1_transfer_data[gas_id].dl_tbf_ptr->rlc_mode == RLC_UNACK )
      {
        do
        {
           /* Do we have any pending PCA_DL_FAI_MSG RRBP's to be sent */
           rrbp_index = gpl1_gprs_get_matching_rrbp_msg_type(PCA_DL_FAI_MSG, gas_id);
           if (rrbp_index < SIZE_OF_RRBP_BUFF)
           {
             /* We have found a PDAN_MSG_WITH_FAI */
              gpl1_gprs_delete_rrbp_entry(rrbp_index, gas_id);
              MSG_GERAN_MED_0_G("RLC UNACK delete_rrbp_entry");
          }
        }
        while(rrbp_index < SIZE_OF_RRBP_BUFF);

        if(gpl1_gprs_get_serv_cell_head_ptr(gas_id) != NULL )
        {
          MSG_GERAN_HIGH_0_G("EPDAN_REQ pending in queue");
        }
        gpl1_egprs_empty_qual_meas_queue(gas_id);
      }
      else
      {
        MSG_GERAN_MED_0_G("RLC_MODE_INVALID");
      }
    }
    else
    {
       MSG_GERAN_MED_0_G("dl ptr == null");
       return;
    }
    l1_transfer_data[gas_id].dl_tbf_ptr->dl_tbf_with_t3192_active = TRUE;
    MSG_GERAN_MED_0_G("T3192 TMR_ACTIVE");
  }
}
#endif

#ifdef FEATURE_GPRS_PS_HANDOVER
boolean get_l1_psho_revert_command( gas_id_t gas_id )
{
  gpl1_gprs_transfer_if_data_t *gpl1_gprs_transfer_if_data_ptr = &gpl1_gprs_transfer_if_data[gas_id];
  return gpl1_gprs_transfer_if_data_ptr->l1_psho_revert_command;
}

void set_l1_psho_revert_command(boolean l1_psho_revert_command,gas_id_t gas_id)
{
  gpl1_gprs_transfer_if_data_t *gpl1_gprs_transfer_if_data_ptr = &gpl1_gprs_transfer_if_data[gas_id];
  gpl1_gprs_transfer_if_data_ptr->l1_psho_revert_command = l1_psho_revert_command;
}
#endif /* FEATURE_GPRS_PS_HANDOVER */
