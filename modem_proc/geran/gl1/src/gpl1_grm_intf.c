/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    L 1 GRM GENERAL PROCESSING FILE

GENERAL DESCRIPTION
This module handles GERAN-to-TRM general purpose functions

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_grm_intf.c#6 $
$DateTime: 2021/08/18 02:53:44 $ $Author: pwbldsvc $

when      who       what, where, why
--------  --------  ----------------------------------------------------------
15/03/21   rks      CR2870853 fixing compiler warnings, caused by compiler upgrade.
09/12/16   sk       CR1099227 Ensure explicit TRM release happens after an empty QTA gap
28/01/20   rks      CR2463005 To fix the GL1 stuck issue while generating MINIDUMP.
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
07/06/19   rks      CR2464129 Resolving compilation issues for new compiler upgrade on HA1.0
22/10/18   rks      CR2311819 Do not do TRM reserve at for idle activities when STOP_GSM_MODE_REQ received for reselection.
06/04/18   akm      CR2216412 Moving G2X de-init in QTA cleanup
3/22/18    sk       CR2210950 Always reset rx_tx_status of the GSM client on grm_Release
11/04/17   rv       CR2031827 Excessive F3 log reduction for Oppo
12/06/17   cws      CR2058343 Register G sub2 Tx cilent async cb in single sim mode
06/07/17   sp       CR2071477 Push loosing client's TRM request of PCHxFER to Task to avoid load on ISR.
09/06/17   sn       CR2058997 Send DR change request to transfer sub only when Device config is DRDSDS
23/01/18   rv       CR2019399: Define G2T IRAT as LTA activity
25/01/18   km       CR2178965 Add grm_info mutex protection in grm_transaction_manager
31/10/17   sp       CR2125524 Update frame duration in task context when unlock_cancell is received
03/10/17   sp       CR2115832 Avoid MCPM State transitions in FIND_BCCH if EMBMS is Active on other sub (to utilize available gaps effectively)
06/09/17   sn       CR2103687 TRM Reason should be not cleared during releasing of PCHXFER
11/07/17   nsa      CR2138558 Adding additional cases for TRM acq priority so that priority not gets lowered incorrectly
08/06/17   sp       CR2056683: Populate ta_info in ReqAndNotify.
20/09/17   asr      CR2112996 Passing CBCH channel for determining trm band
23/01/18   rrp      CR2124956 Cancel reservation to clean QTA gap only if QTA startup is initialised.
12/01/18   rv       CR2170840: Fix for GSM TRM priority is lower then 1x priority during ACQUISITION 
23/03/16   smd      CR992904 Added a debug crash if GL1 holds tx device without rx device.
09/08/16   cws      CR1050184 Do not release Tx chain in single sim mode
08/09/16   snjv     CR1058525 Dont release TRM when calling RnN with updated reason
23/08/16   snjv     CR1057836 Read freq info from bit mask set when handling mph set prio ind 
09/09/16   sn       CR1062272 TRM RESERVE AT during IDLE to transfer
20/12/17   sk       CR2030816 GERAN changes for PBR Randomization algorithm
23/05/16   sp/snjv  CR1017531 When Div is disabled dynamically in MSim Idle release granted RxD when Reserving Prx
12/12/17   sk       CR2157443 Ensure correct DRX cycle value is sent in the ELS update
15/12/17   ap       CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
12/06/17   sn       CR2054908 Handling to back to back DR mode change requests
14/03/16   sn       CR982754 offload TRM exchange operation from ISR to L1_TASK
07/06/16   snjv     CR1019245 Do RnN instead of TRM Req(ISR) and do a grouped release when suitable
08/09/17   sn       CR1061730 FR43082: TA during PS Procedures- GL1 changes
29/03/16   mn       CR995137 Access tune away support: Allowing other tech's paging at GPRS access
21/04/16   db       CR999886 tx trm status is not reset at the time of sending start_gsm_mode_req in single SIM mode
13/04/17   zf       CR2033453: Fix KW error
29/10/17   km       CR2133917 GL1 support for L+L
06/12/16   snjv     CR1097939 When Res./Req. TRM update ta_info considering GBTA Only in G Plus G
21/11/16   snjv     CR1092709 L+L: G2L in QTA: Perform Reserve At for Div. for G2L
24/10/16   snjv     CR1081627 Gl1 to call grouped Reserve At instead of separate calls to DRx and tx clients
23/06/17   br       CR2064558 Don't install drx unlock cb handler in multisim idle mode though trm reason BG_TRAFFIC
02/02/17   br       CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
23/05/16   snjv     CR1012850 Release Bands bef clearing Band Reg strct & also extend trm properly
15/12/16   sk       CR1101677 Do not call qta_fw_rf_deinit from sch handler if QBTA is not supported
08/02/16   ng       CR1048864 Do not do TRM reservation for an already aborted IRAT activity
04/05/16   snjv     CR1010404 Drop Tx Chain when resource lock is released in Acq.
14/07/16   zf       CR1042228: Set trm_grant_status if TRM callback pending 
15/07/16   gmr      CR1039177 Avoid multiple declarations of TRM callback function 
12/07/16   tsk      CR987216 For SSIM Idle mode RxD should be gated by Idle mode RxD NV.
12/07/16   tsk      CR978200 Enable dynamic Rxd with SSIM and WLAN Antenna Sharing configuration.
18/05/16   sp       CR945002 Accept unlock callbacks with Reselection tag also.
17/05/16   rv       CR1016827: Fix for Build error in QTF mob
12/05/16    rv      CR998395: Excessive F3 logging in  GERAN L1 
03/05/16   bg       CR998612 TRM request with proper G band for the priority CBCH reception
28/04/16   hd       CR998694 Do not release the TRM in X+G configuration for extended paging while booking
11/03/16   sk       CR987445 If we already have lock with the same reason, no need to tell TRM the reason change
23/02/16   sk       CR973190 Excessive F3 logging reduction
22/4/16    sn       CR1006445 Correcting the CR1000581 chnages
11/4/16    sn       CR1000581 Releasing TX chain when resource is going inactive.
17/03/16   am       CR988102 Empty QTA gap when NBCCH and page activity are in collision
09/03/16   sk       CR986058 Klockwork error removal
01/02/15   js       CR909145 FR 27391: GL1 should allow QBTA/QTA gaps in find_BCCH state to avoid LTA gaps
01/02/16   bg       CR951395 Skip QTA if the allocated device does not support band
22/01/16   sp       CR957208 Fetch bands supported for Diversity Chain on the granted device.
22/01/16   sp       CR961535 Dont consider RxDiversity grant/denial as partial grant in GroupedTRM ReqAndNotify Callback
22/12/15   hd       CR953597 Use TRM lock while exchanging the priority and reserving TRM for CBCH activity
16/12/15   js       CR952188 Avoid exchange lock while releasing lock in QTA gap
12/16/15   tsk      CR951099 TRM grant status should consider secondary client id for GBTA during TRM release.
21/12/15   sk       CR938638 Reducing no of F3s
10/12/15   sp       CR948656 Check and update trm reserve reason if CB prio change is triggered by RR.
02/12/15   ap       CR939311 Change reason from ACCESS to TRAFFIC only when DRDSDS disabled
02/12/15   cah      CR943472 Debug F3 for Persistent denial. 
24/11/15   hd       CR942132 Release TRM before doing a reserve_at() in GTA gap
26/11/15   sp       CR943905 Update tuneaway info in Diversity request also.
20/11/15   pg       CR935148 Request TRM for DRx chain only for supported bands 
19/11/15   nm       CR939724 Ignore unlock cb for Diversity chain on Tabasco Modem(as DR is not supported)
18/11/15   cah      CR900214 - [WLAN+WAN] Failed in acquiring the Rxd chain in Single SIM Mode during Voice Call Attempt
16/11/15   sp       CR939632 Clear transaction manager entitys for all IFS activities
06/10/15   sp       CR914315: Update frame duration when Unlock Cancelled received for ACQuisition
11/11/15   smd      CR929829 Check the correct flag for BPLMN active
28/10/15   smd      CR929775 Disable DR temporarily when there is partial grant for RACH
05/11/15   cah      CR934800 call DR disable in task context when PG is received from ACCESS
28/09/15   cjl      CR929471 Delay sleep after geran lock release in idle
16/10/15   og       CR922137 Prevent unnecessary band release on the destination client
                    during the TRM lock exchange from client 7 to client 22
20/10/15   snjv     CR927604 Update duration for DR Band registration in G2W Ded search
15/10/15   js       CR923205 Reset W-Reconf flag if TRM lock is granted
02/10/15   og       CR917123 Prevent unnecessary band release on the destination client during TRM lock exchange.
14/10/15   cc       CR917262 Delay release IRAT band to search done for G2W dedicated search
06/10/15   sp       CR914315 Update frame duration when Unlock Cancelled received for ACQuisition
30/09/15   sk       CR916227 Disable frequent QSH events and add PCH rsv handler
10/09/15   sk       CR899847 GERAN changes to trigger QSH events and handler for debugging
01/10/15   cah      CR915732 DR-DSDS: Fix bug in Band req for DIVRX request
05/10/15   js       CR915574 Setting QTA initialized after bulding RF scripts in QTA gap 
18/09/15   nm       CR908307 Back out CR888325
23/09/15   sp       CR910405 Set trm_status to Callback pending after grm request and notify.
22/09/15   cah      CR905135 DR-DSDS. Ensure active G client releases IFS/IRAT band registration in TRM.
16/09/15   cjl      CR908527 improve system procedures used for TRM access
11/09/15   js       CR905493 Using gas_id based on requested client ID for acquring mutex lock
09/09/15   mko      CR904584 Backout CR899509 changes
14/09/15   snjv     CR904027 In Forced RnN if Tag isnt for reason Access Call Req. Notify Enhanced instead
15/09/15   zf       CR903269 Release Tx chain only when Tx client holds a chain or has a pending request
15/09/15   cah      CR906726 DR-DSDS: Add support for diversity chain release during CS registration procedure
11/09/15   ap       CR905373 GERAN missing pages whilst WCDMA in DATA due to incorrect trm resoursce usage 
02/09/15   zf       CR905869 Set trm_status to NULL after grm release
09/09/15   mko      CR904584: Backout CR899509 changes
09/09/15   mko      CR904584: Backout CR899509 changes
08/09/15   cah      CR892575: DR DSDS: Add support for diversity chain unlock in transfer mode
02/09/15   ap       CR900997 Populate the TuneAway Info as part of the grouped trm request
20/08/15   pa       CR893756:Grouped TRM apis for the diversity chain.
31/08/15   zf       CR899509: Do not release TRM for Tx if not holding TRM lock on Tx chain
01/09/15   ab       CR898944 - PCHXFER during TRM exchange should not release Diversity Chain
20/08/15   nm       CR888325 Improve Debug F3 if idle sub page collides with transfer sub Ncell SCH/FCCH
26/08/15   smd      CR896555 Don't start BPLMN in QTA gap.
27/08/15   smd      CR897074: Don't trigger persistent denial algorithm for GRM_DECREMENT_DENIED_REG_COUNT in grm_reserve_at.
12/08/15   pa       CR887572: RxD: Schedule DRX wakeup after Rx/Tx and request TRM in task context.
26/08/15   mko      CR895384 Clear the pending deferred wmgr event before putting reserve_at from GTS
27/08/15   cah      CR89423: Request for drx chain not populated with correct frequency information.
26/08/15   ws       CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
25/08/15   og       CR876393. Adding more comments.
13/08/15   smd      CR890960: Don't set Skip IFS flag if the  returned RF device is invalid
14/07/15   nm       CR865391 Move the TRM reserve for DRx chain before PRx chain
12/08/15   jk       CR882186:Don't request DRx device if in warm up ISR,LMM and SAIC is disabled
15/08/15   nm       CR890695 Do not call band release for IRAT bands during PCHXFER
14/08/14   ws       CR889273 Fix Static declarations of variables which are not Dual data spaced
14/08/15   cah      CR888912: Stale persistent denial PM band registartion in GRM. Causes flooring on X tech
13/08/15   pa       CR885541: Release TX, DRX and then PRX chain.
12/08/15   nm       CR888345 check for trm status "Retained_for_access" also while callign trm apis for TX client
10/08/15   sp       CR884488: Correctly Resume BCCH List in BPLMN state machine after page decode
05/08/15   hd       CR884936: Handle QTA STARTUP and CLEANUP messages in MSGR IF
05/08/15   cja      CR881765 Use grm cb startup/cleanup to determine if in qta gap for grm
05/08/15   cws      CR881986 Use only gsm1/gsm2 client for trm asdiv request
04/08/15   cah      CR881058:DR-DSDS update to persistent denial algorithm to check supported G bands for device.
17/07/15   ab       CR 871981 Address Static Analysis Warning.
29/07/15   zf       CR877814: Set TA perference for diversity chain
28/07/15   smd      CR845682 Skip IFS if GSM is granted a new device than the current device
27/07/15   pa       CR879827: Remove TRM reservation for the diversity chain.
21/07/15   cjl      CR876903 Use system procedure types for TRM subreasons
25/07/15   nk       CR877656 Dont release tx client when TRM resource lock is ON
23/07/15   fan/akh  CR865360 Use correct TRM reservation period when doing wakeup cancelation
23/07/15   akh      CR876576 Avoid TRM adjustment to include warmup when already included
24/07/15   amr      CR867369 Fix incorrect IRAT frame duration
23/07/15   fan/akh  CR853300 FR19329: PBR priority incorrectly incremented during W+G CBCH
23/07/15   cjl      CR878351 fix compiler warning
22/07/15   ap       CR877707: Use TRM_RX_BEST/TRM_RX_ANY/TRM_TX_BEST instead of TRM_RXTX_ANY and TRM_RXTX_BEST
21/07/15   cws      CR873163 fix asdiv/pchxfer compile warnings
21/07/15   cjl      CR876741 Replace TRM subreasons with system procedure types
17/07/15   sjw      CR874699: remove DUAL_WCDMA featurisation for inter-RAT client
15/07/15   fan/akh  CR857332 FR19329: WMGR support for rescheduling non-PCH activities
15/07/15   pa       CR872886: Fill frequency info for grouped trm request.
13/07/15   pa       CR870620: Don't release chains during the start of Single SIM initialisation.
12/07/15   nm       CR861458 Fix the interworking issues between PCHXFER, DRDSDS, RX/TX split
10/07/15   pa       CR866748: Circular buffer for grouped TRM input structures.
17/06/15   cjl      CR853555 Use dynamic connected mode reasons for CS/PS traffic
09/07/15   cah       CR811835: FR 26893: Band-by-Band OOS and BPLMN Search - GL1
06/07/15   sp       CR862778: Reduce F3 logging in Idle as its impacting QTA timeline
24/06/15   ap       CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR
03/07/15   pa       CR865455: Don't clear grm frequency info during Single SIM initialisation.
02/07/15   aga      CR860197 NULL2L workaround Reserve and request serving band for LTE IRAT
29/06/15   pg       CR862412 Remove conditional check on grant status before DRx chain release
26/06/15   zf       CR862683: Clear freq info when release TRM
06/28/15   smd      CR862917 Reserve/request the correct band for BCCH activity.
30/06/15   ws       CR852519 - Resolve GL1 enum conversion compile warnings
25/06/15   pg       CR861516 Reduce GRM function stack usage to avoid GL1 ISR stack overflows
19/06/15   fan/akh  CR851771 FR19329: Crash due to vstmr access during sleep
19/06/15   kb/ak/fa CR854009 FR19329: GSM making wrong TRM request
04/06/15   fan/akh  CR848455 Fix for multiple handlings of same cancelation request
17/06/15   ab       CR847473 : unify usage of gl1_map_gas_id_to_client_id for TX_ANT_DIV/DR_DSDS/RX_DIV/PCHXFER/AS_DIV
06/01/15   js       CR814275 GL1 changes for ROTA interface during extended paging
18/06/15   cah      CR748477: DR-DSDS: Reduce F3 load
18/06/15   cah      CR856002: handle partial grant for IFS/IRAT frequency band registrations
10/06/15   sjw      CR850737: variable connection priority for TRM-Unification
09/06/15   pa       CR846421: Partial grant, release chains outside the scope of callback context.
10/06/15   cjl      CR851285 Fix RACH from idle state machine during RR procedure lock
29/05/15   fan      CR843017 Correct handling of pending_deferred_wmgr_event flag and gl1_hw_wkupmgr_wakeup_point_fn
02/06/15   ar       CR847207 TRM_UNLOCK_BAND_INCOMPATIBLE not handled by gl1
03/06/15   aga      CR847218 Set preference for QTA gap when reserving Channel Maintenance activity
03/06/15   cah      CR847720 Remove conditional check on grant status before TX chain release
22/04/15   am       CR803948 TA in idle mode when mm_non_drx is enabled
25/05/15   sjv      CR843437 Make sure to add PD bands to TRM reserve_at , if next PCH block includes PM on denied bands
22/05/15   cah      CR841674 GL1 passed RFGSM invalid device ID 11 for UMID 0x0603022c on sub 2]
22/05/15   hd       CR841697 Release the secondary chain if reserve_at() doesn't happen
20/05/15   cgc      CR840322 add grm_transaction_release() to grm_register_additional_irat_bands()
29/05/15   pa       CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD.
03/03/15   zc       CR815093: Cancel reservation for W ncell activity if list is empty
19/05/15   cjl      CR840652 Apply GL1 procedure lock to tx client state in split rx/tx
20/05/15   aga      CR840879 Remove obsolete code that bypasses the clearing of the bands registration information
19/05/15   cjl      CR840425 Set trm status after issue of Request and Notify when resource lock active
18/05/15   ap       CR839705 Set trm status after issue of Request and Notify
15/05/15   pa       CR838867: Check Tx grant status and allocated RF device for complete grant.
20/04/15   sjv      CR823566 Set number of TRM denials/Xcch failures count through debug NV
07/05/15   smd/sjw  CR8nnnnn: inter-RAT lock exchange support for TX clients
15/04/15   pa       CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
08/05/15   sjw      CR835041: support for TRM_UMTS2 in grm_update_client_state_info()
07/05/15   fan/akh  CR834237 FR19329 Full G wake-up cancellation + code cleanup
05/05/15   smd      CR832564: Set GBTA_ALLOW when DR is disabled
06/05/15   mc       CR833524 Do not register TRM_GPRS* clients for chain hopping events
01/05/15   aga      CR831392 Use the appropriate WCDMA client ID gased on static AS_ID mapping
30/04/15   cja      CR830934 Ignore QTA startup with invalid RF device, indicate as empty gap
30/04/15   cja      CR800657 QTA gap for G2W IRAT
30/04/15   ap       CR811057 FR22272 - TRM Unification  - Logging Changes
23/04/15   cjl      CR822231 Cross WTR IRAT reselection changes to GL1 exchange mechanism
21/04/15   sjw      CR822898: removal of deprecated ACQUISITION reasons
20/04/15   ip       CR811868 Dual Multisim mode code changes + debug overhaul
16/04/15   smd      CR822202 Dual space data for IRAT and QTA
16/04/15   smd      CR822202 Replace TRM_TUNEAWAY_NONE with TRM_LONG_TUNEAWAY
16/04/15   aga      CR822202 TRM interface for QTA
03/02/15   cjl      CR790433 Create separate TRM activity for WCDMA reconfirm when needed
07/04/15   cjl      CR811729 FR22272 - TRM Unification - GL1 changes to allow lock retention using GRM
10/04/15   cgc      CR820213 Add grm_register_additional_irat_bands() for G2L fast search.
10/04/15   cah      CR819666 Problem in the logic for adding the denied bands to trm_reserve_at and trm_request
10/04/15   pa       CR820866:Incorrect return type of grm request for diversity chain
07/04/15   cja      CR800650 Optimise irat state machine
01/04/15   cah      CR816447: DR-DSDS ensure PM skipping does not occur when DR is disabled.
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
30/03/15   ap       CR811753 FR22272 - TRM Unification - GL1 changes to support
                                        SysInfo priority inversion during early camping
26/03/15   cs       CR813680 Remove polling mechanism for modify chain state
24/03/15   ap       CR811053:FR22272 - TRM Unification  - Access and Traffic attempts
23/03/15   ap       CR811046:FR22272 - TRM Unification  - Removal of _HI reasons/UNLOCK_IMMEDIATELY
25/03/15   ap       CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
24/03/15   sjw      CR812407: update for new MPH_SET_PRIORITY_IND payload
23/03/15   smd      CR812018 Don't request second_preferred resource in trm request except in svlte mode.
03/05/15   cah/sjv  CR800489 Handling grm_retain_lock_for_acquisition when change reason is made pending or denied by TRM
12/03/15   pa       CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
13/10/15   pg       CR805823 GSM diversity client grm_info memory corruption
03/03/15   ws       CR802410 Remove depreacated API trm_get_reason
25/02/14   cah      CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good serving cell conditions
24/02/15   js       CR749012: GL1 changes for LTE removal
20/02/15   sjw      CR793380: adaptive TRM durations for acquisition mode
20/02/15   jj       CR 798186  IRAT enum duplication clean up
15/01/15   cah      CR781585: GL1 registers invalid TRM band for TD booking.
20/02/15   js       CR693771: W+T removal. Resolving compilation errors
18/02/15   cah      CR780986: TRM_PENDING not handled by grm_change_reason api.
18/02/15   cah      CR777857: propagtion of Bolt CR to remove chipset names from Gl1.
18/02/15   cah      CR777304: check other sub gas_id is valid before use.
16/12/14   js       CR771759 Release TRM explicitly in case if W+G QTA
16/02/15   sp       CR772306: Update TRM of gbta grant for PBR to work
13/02/15   pa       CR792105: Allow either GBTA or IRAT.
13/02/15   js       CR782437 Providing API to WL1 for ROTA enhancement
13/02/15   pa       CR756156:G2X RF device handling using TRM apis.
04/02/15   sjv      CR790894: Disable DR->SR Fallback using mask set for NV 65770 (l1_debug)
29/01/15   pa       CR785021: Disable DR on TRM denial to activate GBTA on next PCH decode.
21/01/15   npt      CR783249 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled
21/01/15   pa       CR758792:RxD support for multi-sim.
19/01/15   mc       CR769958 : Removing Deprecated TRM APIs in GERAN
12/01/15   sjv      CR756154 Enhance Band Registration for Activities
06/01/14   jk       CR773695:DRx device Id not requested for all bands
05/01/15   npt      CR770381 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled
23/12/14   ggu      CR772195: No need to continue check gbta_allowed status when there is no any G sub is under TRANS state
22/12/14   pa       CR773689: Set RF device to Invalid on RF exit.
22/12/14   jj       CR774326  fix Kw warnings
19/12/14   nm       CR761630 check the bcch in trans seq state also for allowing gbta
15/12/14   smd      CR771282 Update aps_state only if there is a change.
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   dv       CR768715 in a good scell, ensure Persistant denial stays on
10/12/14   ws       CR768376 Q6 compile warning fixes
04/12/14   sjv      CR766784 Backing out CR759568
03/12/14   npt      CR759467 Fix compiler warnings
25/11/14   zf/zc/sj CR759568 grm_info ptr retrieved in grm_change_reason should use client_id,
06/11/14   nm       CR751692 Fix merge Errors for feature PCHXFER
25/11/14   pg       CR762645 Out of array bounds error for Rx Diversity for single-SIM builds
24/11/14   sjv      CR748431 Update DR mode flags after Multi_sim_mode has been set on receiving start_gsm_mode
24/11/14   sjv      CR762334 KW fix, for uninitialised band information in dev_info returned in grm_get_irat_rf_dev_from_grm_info
24/11/14   ws       CR761216 P1 Klocwork Fix, bounds check gas_id before use
19/11/14   sjv      CR759964 Update bands supported by TRM based on return data from TRM
18/11/14   sp       CR758079: Update internal GRM reason to TRM_NUM_REASONS when TRM is denied.
20/10/14   sk       CR735697 Ensure GL1 doesnt toggle between SI3/SI4 on TRM denial, and does explicit TRM release before making reservation in non-G only mode
13/11/14   am       CR754217 Keep GRM internal reason aligned with latest TRM function
13/11/14   npt      CR746180 Remove KW errors
03/11/14   sjv      CR750056 IRAT activities need to be aborted/cleared when an update_list is received
30/10/14   cws      CR737486  Update req_n_notify_reason properly and do retain lock check after local reason is updated
29/10/14   cah      CR710699: Do not update RSSI averaging for skipped ARFCN's
29/10/14   cah      CR747188  incorrect bands reported to TRM/ DR mode not enabled correctly.
29/10/14   ab       CR745429  Featurization correction for PCHXFER for SS / CR merges.
28/10/14   sjv      CR747201  Handle multiple calls for grm_reserve from task message queue (made from l1_fm_grm_reserve)
28/10/14   ss       CR746380  Set TRM extention flag outside of grm intialisation function.
27/10/14   sjv      CR746579  make sure to release pending activities as well in grm_band_release_if_required
25/10/14   sjv      CR745877  Update Crash Condition for Irat trm band reg.
24/10/14   sjv/cah  CR744418  Get serving cell band in freq info for demod_page
24/10/14   zf       CR745120: Fix unbalanced grm mutex lock/unlock.
23/10/14   sjv      CR744425  Removing redundant debug F3s for PM_ACT_DBG
23/10/14   pa       CR744906: Allow one IRAT DRDSDS activity at a time.
22/10/14   cah      CR737592: optimisation for persistent denial based on Scell RSSI and SnR
22/10/14   ws       CR744361 Data+MMS reduce MSC if commanded by CFCM
20/10/14   pa       CR742979: Mutex lock grm_info on releasing IRAT band registration.
17/10/14   pa       CR739626: In G2X TA, perform trm band registration on actual scheduling of BCCH.
17/10/14   sjv      CR741361 Clear grm freq_store when releasing TRM if persistent denial is not enabled
17/10/14   cs       CR735918 Assign supported bands for initial ACQ based on UE support
15/10/14   pa       CR739469: Add band information in trm request for NCELL activities.
14/10/14   pa       CR738110 Store requested TRM reason in the local structure on Request and Notify/Enhanced.
09/10/14   ws       CR 736562 P1 klocwork fixes
07/10/14   sjv      CR734254  G2L rf devices retrieval for primary and diversity
07/10/14   cah      CR734500; fix for GINT crash caused by gl1 trm status being out of sync with TRM.
03/10/14   aga      CR732455 Changes required to tune away framework for TH 1.0 single sim build
02/10/14   sjv      CR733766 Get RF Device from grm_info instead of querying TRM
30/09/14   pa       CR732428: Mutex lock grm api to prevent ISR and Task to write grm_info at the same time.
29/09/14   cs       CR726567 Always use zero min_duration in grm change reason
28/09/14   sjv      CR731407: grm_add_arfcn_to_trm_freq_input_type to update bands appropriately
24/09/14   cah      CR729408: Updates to reduce number of TRM API calls in Gl1.
22/09/14   cah       CR723144: Bolt 2.0 DRDSDS - W+G - G2G interband ncell monitor activity denied while in W+G FC mode
22/09/14   ap       CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
17/09/14   pa       CR725876 Release existing TRM IRAT Reg activity before creating next.
17/09/14   sjv      CR724558 For PM, TRM band-reg and release at frame level granularity for perf. optimization
17/09/14   pa       CR725876 Release existing TRM IRAT Reg activity before creating next.
16/09/14   cs       CR725029 Update TRM state IRAT API usage with new field
15/09/14   ws       CR 724529 Resolve remaining P1 Klocwork issues
12/09/14   cs       CR723714 Improve band registration handling in DR-DSDS
11/09/14   ws       CR723099 Resolve GL1 compile warnings
11/09/14   am       CR677361 Addressing KW GL1 P1 errors
10/09/14   cs       CR722344 Remove old deprecated TRM API usage
09/09/14   cos      CR720965 adding support for DR-DSDS - rfdev 2 and 3 handling wtr to RxD and calibration
27/08/14   sk       CR712706 Do explicit trm release before making a trm reservation only when inside the QTA gap
02/09/14   cah      CR717580 trm_client.cpp:5900   Assertion TRM_IS_PRIMARY_RF_DEVICE(activity_ptr->primary_device) failed
28/08/14   sjv      CR717166 pending activities are organized in an array for trm band release to avoid overwrite in case of diff. activities
27/08/14   cah      CR713711 P1 KW error: possible de-reference of null pointer
27/08/14   cah      CR713711 P1 KW error: possible de-reference of null pointer
29/08/14   cah      CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
27/08/14   cah      CR713711 P1 KW error: possible de-reference of null pointer
26/08/14   pa       CR715814 IFS measured client id is not updated from GL1 before passing to TRM
21/08/14   dv       CR711417 Ensure DR mode is enabled when camping on cell
20/08/14   sjv      CR711061  grm_send_band_release_if_required to be modified to clear bands for all activities
19/08/14   sjv      CR711666 measured client id is not updated from GL1 before passing to TRM
12/08/14   cah      CR707445 Overlap between PM's in PTM mode causes transaction id corruption
11/08/14   ssh      CR705064: when doing a redundant TRM reservation always use the stored TRM reserve reason
11/08/14   pa       CR707696: Band registration release is required if TRM is granted otherwise not.
08/08/14   ka       CR706866 FR18739: DR-DSDS RxD Idle, part 2 - new TRM clients
08/09/14   cah      CR704964: not handling TRM request pending correctly when attempting to change TRM reason to access urgent.
07/08/14   smd      CR706415 Fixed the event mask to register WTR hopping callback.
06/08/14   ka       CR699964 Implement Idle RXD support in GL1
05/08/14   zc       CR701993 grm_get_info passing wrong client into trm_get_info for device mapping
05/08/14   pa       CR704815: Fill band info during PTM Ncell activity.
05/08/14   pjr      CR703401 MSC capability reverted on wrong sub when MMS is complete
31/07/14   dv       CR702665 fix bug in grm_send_band_release_if_required
31/07/14   cos      CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
29/07/14   ap       CR695234: Changes for GL1's New Rach interface
28/07/14   cah      CR701236 BOLT 2.0 DR-DSDS: IFS band not released after SCH decode.
24/07/14   ka       CR692214 TRM APS feature changes required with new TRM api's
28/07/14   cah      CR700769: MODEM - trm_interface.cpp:2252   Unhandled feature 0.. client: 0
27/07/14   dv       CR700582 Ensure peending IFS/IRAT activity cleared if trm extension denied
25/07/14   cs       CR697245 New GRM API support for WTR Hopping
22/07/14   cah      CR697586 trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
16/07/14   ssh      CR689277: On wake up cancel do trm release only if TRM status is denied and TRM/GRM reserve reasons match
16/07/14   aga      CR695332 PCH scheduling  is not initiated, GBTA is not stated
15/07/14   pjr      CR694682 Revert MSC on other sub when MMS is complete
14/07/14   cs       CR693967 Get DR-DSDS mode during startup
11/07/14   zf       CR692262: Check missed frames when request TRM at warmup
10/07/14   cws      CR690009 Each sub check its own TRM lock only and add check before updating grm_info
02/07/14   cs       CR688003 First version
19/06/14   ssh      CR679781: Clear any pending deferred WMGR events if trm reservation done prior to sleep
21/05/14   ssh      CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub

===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "qsh.h"
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "ms.h"


#include "gpl1_grm_intf.h"

#include "l1.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "geran_multi_sim.h"

#include "l1_os.h"
#include "gpl1_gprs_isr.h"
#include "l1_drx.h"
#include "l1_sc.h"
#include "l1_fm.h"
#include "gl1_msg_wcdma_meas.h"

#include "rfm_device_types.h"

#include "gl1_hw_g.h"

#ifdef FEATURE_GSM_COEX
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX */

#include "gpl1_dual_sim.h"

#ifdef FEATURE_GSM_WTR_HOP
#include "gl1_msg_wtr_hop.h"
#endif /* FEATURE_GSM_WTR_HOP */

#include "gl1_msg_g.h"

#include <stringl.h>
#include "gl1_hw_sleep_ctl.h"
#include "gl1_mutex.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

/*API's and types for converting T, W & L frequencies into TRM system bands*/
#include "l1rrcif.h"
#include "lte_as.h"
#include "wl1trm.h"

#ifdef  FEATURE_GSM_TO_LTE
#include "lte_ml1_ext_api.h"
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif 


extern boolean gl1_hw_sleep_wmgr_cancel_after_prog[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif
/*===========================================================================

           Data Definitions

===========================================================================*/
grm_device_configuration_t grm_device_config = DEVICE_CONFIG_NONE;

/* The full set of possible GSM bands that can be requested for ACQ etc
   removed BAND PGSM from the list as it is still treated as a common band
   with EGSM900
*/
trm_band_t grm_valid_gsm_sys_bands[NUM_GERAN_DATA_SPACES][MAX_BAND_INFO_GRM];

uint16     grm_valid_gsm_sys_bands_cnt[NUM_GERAN_DATA_SPACES] =
             { INITIAL_VALUE( 0 ) };

trm_band_t grm_valid_gsm_sys_bands_divrx[NUM_GERAN_DATA_SPACES][MAX_BAND_INFO_GRM];

uint16     grm_valid_gsm_sys_bands_cnt_divrx[NUM_GERAN_DATA_SPACES] =
             { INITIAL_VALUE( 0 ) };

static grm_info_t grm_info[MAX_NUM_GRM_CLIENTS];

static geran_priority_t grm_acq_priority[NUM_GERAN_DATA_SPACES] =
                           { INITIAL_VALUE( GERAN_PRIORITY_NOMINAL ) };

static trm_reason_enum_t grm_ded_reason[NUM_GERAN_DATA_SPACES] =
                           { INITIAL_VALUE( GRM_CS_DEFAULT ) };


static trm_reason_enum_t grm_ptm_reason[NUM_GERAN_DATA_SPACES] =
                           { INITIAL_VALUE( GRM_PS_DEFAULT ) };


static grm_resource_lock_t grm_resource_lock_state[ NUM_GERAN_DATA_SPACES ] =
                 { INITIAL_VALUE( GRM_RESOURCE_LOCK_INACTIVE ) };

boolean gl1_grm_qta_gap_empty = FALSE;

static boolean gl1_drds_debug_set[ NUM_GERAN_DATA_SPACES ] =
                 { INITIAL_VALUE( TRUE ) };

static int32 reserve_at_count[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 0 ) };

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static boolean grm_current_aps_state[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };
/*keep track of activity priority (activity type in FM) passed in grm_reserve_at_enhanced,
  this is called only by l1_handle_messages for messages of type L1_GRM_RESERVE_AT
  This is to be used only in between, grm_reserve_at_enhanced and grm_reserve_at*/
static l1_fm_priority_T res_at_enh_pri[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(FM_PRI_MAX)};

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
/* Inform if ELS is ACTIVE i.e. LTE is in DATA */
static boolean els_active = FALSE;
/* ELS state info to be updated with set client state */
static grm_set_client_state_info_t grm_set_trm_els_state_info[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };
extern uint32 gl1_hw_sleep_wmgr_els_warmup_dur_in_sclk[];
#endif /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

/*Pending DR state transition */
grm_pending_dr_state               pending_dr_state;

/*GRM QTA State */
static boolean   grm_qta_state [ NUM_GERAN_DATA_SPACES ] =
                 { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_GSM_RX_TX_SPLIT
/*Tx Info structure for each SUB*/
static grm_tx_info_t     grm_tx_info[NUM_GERAN_DATA_SPACES];
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
/*===========================================================================

           Function  Declaration

===========================================================================*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

static void grm_register_async_callback(grm_client_enum_t  client_id,
                                        gas_id_t           gas_id);

static void grm_register_async_event_cb(
              uint8                         num_of_clients,
              grm_client_enum_t             *client_ids,
              uint32                        async_events,
              gas_id_t                      gas_id);

static void grm_request_and_notify_enh_async_info_cb(
              trm_req_n_not_enh_return_data *rane_info_p,
              grm_client_enum_t              granted_client,
              grm_tag_t                      grm_tag );

static void grm_modify_chain_state_async_info_cb(
              trm_modify_chain_state_return_data *modify_chain_state_info_p,
              grm_client_enum_t                   client_id,
              grm_tag_t                           grm_tag );

static void grm_unlock_chain_async_info_cb(
              trm_unlock_callback_data *unlock_info_p,
              grm_client_enum_t         client_id,
              grm_tag_t                 grm_tag );

static void grm_dual_receive_info_cb(
              trm_dual_rec_info_return_data *dual_receive_info_p,
              grm_client_enum_t              requested_client,
              grm_tag_t                      grm_tag );

static void grm_access_callback(
              grm_client_enum_t          client_id,
              trm_grant_return_enum_type grant_status );

static void grm_default_unlock(
              trm_unlock_callback_data *unlock_cb_data_p );



static void grm_limited_acq_unlock(
              trm_unlock_callback_data *unlock_data_p );

static void grm_request_and_notify_async_info_cb(
              trm_req_n_not_return_data *ran_info_p,
              grm_client_enum_t          granted_client,
              grm_tag_t                  grm_tag );

#ifdef FEATURE_GSM_WTR_HOP
static void grm_connected_mode_hop_async_info_cb(
              trm_connected_mode_hop_callback_data *connected_mode_hop_info_p,
              grm_client_enum_t                     client_id);
#endif /* FEATURE_GSM_WTR_HOP */

static void grm_qta_async_info_cb(
              trm_qta_event_callback_data *unlock_info_p,
              trm_client_enum_t         client,
              grm_tag_t                 grm_tag );

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
static void grm_els_async_info_cb(
   trm_els_state_callback_data *els_event_info_p,
   trm_client_enum_t         client
   ); 
#endif /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */


static boolean grm_get_info(grm_client_enum_t  client_id,
                            trm_get_info_data *grm_get_info_data_p,
                            grm_tag_t          grm_tag,
                            gas_id_t           gas_id );

static void grm_reset_paging_info(grm_info_t *grm_info_p,
                                  uint8       bs_pa_mfrms );

static void grm_clear_request_data(grm_client_enum_t  client_id,
                                   gas_id_t           gas_id );

static void grm_clear_sc_activity_data(grm_client_enum_t  client_id,
                                       gas_id_t           gas_id);

static void grm_process_client_state_update_result(
              grm_sc_activity_state_info_t  *result_data,
              gl1_sc_activities_t           activity,
              grm_client_enum_t             client_id,
              gas_id_t                      gas_id);


static boolean grm_is_valid_gsm_sysband (const trm_band_t band, const gas_id_t gas_id );

#ifdef FEATURE_GSM_RX_TX_SPLIT
static void grm_modify_to_traffic_reason_callback(grm_client_enum_t          granted_client,
                            trm_grant_return_enum_type grant_status );
#endif 

static void grm_coll_async_info_cb(
              trm_idle_coll_event_data  *idle_coll_event_info,
              trm_client_enum_t         client,
              grm_tag_t                 grm_tag );


/*===========================================================================

           Functions  Definition

===========================================================================*/

/*===========================================================================

FUNCTION     gl1_map_gas_id_to_client_id

DESCRIPTION  gas ID to trm client mapping function

RETURN VALUE trm client

===========================================================================*/
grm_client_enum_t gl1_map_gas_id_to_client_id(gas_id_t gas_id)
{
  grm_client_enum_t grm_client = GRM_NO_CLIENT;

  switch ( gas_id )
  {
    case GERAN_ACCESS_STRATUM_ID_1:
      /* In TA 1.0, client_id is set on receiving MPH_START_GSM_MODE_REQ, therefore if
      this function is called before START_GSM, then return TRM_GSM1 */
      if( !((l1_tsk_buffer[gas_id].client_id == GRM_GSM1) || (l1_tsk_buffer[gas_id].client_id == GRM_GPRS1)))
      {
        grm_client = GRM_GSM1;
      }
      else
      {
        grm_client = l1_tsk_buffer[gas_id].client_id;
      }
      break;

#if defined ( FEATURE_DUAL_SIM )
    case GERAN_ACCESS_STRATUM_ID_2:
      /* In TA 1.0, client_id is set on receiving MPH_START_GSM_MODE_REQ, therefore if
      this function is called before START_GSM, then return TRM_GSM1 */
      if( !((l1_tsk_buffer[gas_id].client_id == GRM_GSM2) || (l1_tsk_buffer[gas_id].client_id == GRM_GPRS2)))
      {
        grm_client = GRM_GSM2;
      }
      else
      {
        grm_client = l1_tsk_buffer[gas_id].client_id;
      }
      break;
#endif /* FEATURE_DUAL_SIM */

#if defined ( FEATURE_TRIPLE_SIM )
    case GERAN_ACCESS_STRATUM_ID_3:
      grm_client = GRM_GSM3;
      break;
#endif /* FEATURE_TRIPLE_SIM */

    default:
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_3(" Unknown gas_id = %d trm_client invalid %d. TRM_LAST_CLIENT: %d",
                        gas_id,
                        grm_client,
                        TRM_LAST_CLIENT);
      grm_client = GRM_GSM1;
      break;
  }

  ASSERT(IS_VALID_GRM_CLIENT(grm_client));

  return ( grm_client );
}

/*===========================================================================

  FUNCTION  gl1_get_current_grm_client_id

  DESCRIPTION
  This function gets the currect grm client id
  DEPENDENCIES
  None

  RETURN VALUE
  trm client id

  SIDE EFFECTS


===========================================================================*/
grm_client_enum_t gl1_get_current_grm_client_id(gas_id_t gas_id )
{
  return (l1_tsk_buffer[gas_id].client_id);
}

/*===========================================================================

FUNCTION     gl1_map_gas_id_to_client_id_pchxfer

DESCRIPTION  gas id to trm client mapping function for PCHXFER only.
             Usage for PCHXFER ONLY.
             PCHXFER reads BCCH and PCH as well.

RETURN VALUE grm_client_enum_t

===========================================================================*/
grm_client_enum_t gl1_map_gas_id_to_client_id_pchxfer(const gas_id_t gas_id)
{
  grm_client_enum_t grm_client = GRM_NO_CLIENT;

  switch ( gas_id )
  {
    case GERAN_ACCESS_STRATUM_ID_1:

      grm_client = GRM_GSM1;
      break;

#if defined ( FEATURE_DUAL_SIM )
    case GERAN_ACCESS_STRATUM_ID_2:

      grm_client = GRM_GSM2;
      break;
#endif /* FEATURE_DUAL_SIM */


    default:
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_3(" Unknown gas_id = %d trm_client invalid %d. GRM_LAST_CLIENT: %d", gas_id,grm_client, GRM_LAST_CLIENT);
      grm_client = GRM_GSM1;
      break;
  }

  return ( grm_client );
}
/*===========================================================================

  FUNCTION  gl1_map_client_id_to_gas_id

  DESCRIPTION
  This function gets the gas_id from client id

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS


===========================================================================*/
gas_id_t gl1_map_client_id_to_gas_id(grm_client_enum_t client_id )
{
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  switch ( client_id )
  {
    case GRM_GSM1:
#ifdef FEATURE_GSM_RX_TX_SPLIT
    case GRM_GSM1_TX:
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
#ifdef FEATURE_DUAL_SIM
    case GRM_GPRS1:
#ifdef FEATURE_GSM_RX_TX_SPLIT
    case GRM_GPRS1_TX:
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
#endif /* FEATURE_DUAL_SIM */

      gas_id = GERAN_ACCESS_STRATUM_ID_1;
    break;

#ifdef FEATURE_DUAL_SIM
    case GRM_GSM2:
    case GRM_GPRS2:
#ifdef FEATURE_GSM_RX_TX_SPLIT
    case GRM_GSM2_TX:
    case GRM_GPRS2_TX:
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

      gas_id = GERAN_ACCESS_STRATUM_ID_2;
    break;
#endif /* FEATURE_DUAL_SIM */
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    case GRM_GSM1_SECONDARY:
      gas_id = GERAN_ACCESS_STRATUM_ID_1;
    break;
#if defined ( FEATURE_DUAL_SIM )
    case GRM_GSM2_SECONDARY:
      gas_id = GERAN_ACCESS_STRATUM_ID_2;
    break;
#endif
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

#if defined ( FEATURE_TRIPLE_SIM )
    case GRM_GSM3:
      gas_id = GERAN_ACCESS_STRATUM_ID_3;
    break;
#endif /* FEATURE_TRIPLE_SIM */

    default:
      MSG_GERAN_ERROR_1_G( "Unknown client_id:%d", client_id );
    break;
  }
  return ( gas_id );
}

/*===========================================================================

  FUNCTION  gl1_drdsds_enabled

  DESCRIPTION
  This function indicates whether DR-DSDS mode is enabled for the chosen
  GSM SUB.

  DEPENDENCIES
  None

  RETURN VALUE
  dr_enabled status from the appropriate grm_info data structure.

  SIDE EFFECTS

===========================================================================*/
boolean gl1_drdsds_enabled(gas_id_t gas_id )
{
  boolean     drdsds_enabled = FALSE;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(
                          gl1_map_gas_id_to_client_id(gas_id),
                          gas_id);

  if ( grm_info_p )
  {
    drdsds_enabled = grm_info_p->dr_enabled;
  }

  return ( drdsds_enabled );
}

/*===========================================================================

FUNCTION  gl1_drds_debug

DESCRIPTION
  Enable/disable extra debug for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_drds_debug( const gas_id_t gas_id )
{
  return ( gl1_drds_debug_set[ gas_id ] );
}

/*===========================================================================

FUNCTION  grm_get_info_ptr_from_client/gas_id

DESCRIPTION
          Returns a pointer to the MS info structure for the specified client/
          gas_id

DEPENDENCIES

RETURN VALUE
  Pointer to MS info structure

SIDE EFFECTS
  None

===========================================================================*/
grm_info_t *grm_get_info_ptr_from_client(grm_client_enum_t client_id,
                                         gas_id_t gas_id )
{
  grm_info_t *grm_info_p = NULL;

  switch ( client_id )
  {
    case GRM_GSM1:
#ifdef FEATURE_GSM_RX_TX_SPLIT
    /* Tx and Rx share same grm_info*/
    case GRM_GSM1_TX:
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
       grm_info_p = &grm_info[GERAN_DATA_SPACE_INDEX_1];
       break;

#ifdef FEATURE_DUAL_SIM
    case GRM_GPRS1:
#ifdef FEATURE_GSM_RX_TX_SPLIT
    case GRM_GPRS1_TX:
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
      if (  l1_idle_data_store[gas_id].g_bplmn_running_state == BPLMN_ACTIVE )
      {
        grm_info_p = &grm_info[GERAN_BPLMN_SPACE_INDEX_1];
      }
      else
#endif /* FEATURE_DUAL_SIM */
      {
        grm_info_p = &grm_info[GERAN_DATA_SPACE_INDEX_1];
      }

    break;
#ifdef FEATURE_DUAL_SIM
    case GRM_GSM2:
#ifdef FEATURE_GSM_RX_TX_SPLIT
    case GRM_GSM2_TX:
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
       grm_info_p = &grm_info[GERAN_DATA_SPACE_INDEX_2];
       break;
    case GRM_GPRS2:
#ifdef FEATURE_GSM_RX_TX_SPLIT
    case GRM_GPRS2_TX:
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
      if (  l1_idle_data_store[gas_id].g_bplmn_running_state == BPLMN_ACTIVE  )
      {
        grm_info_p = &grm_info[GERAN_BPLMN_SPACE_INDEX_2];
      }
      else
      {
        grm_info_p = &grm_info[GERAN_DATA_SPACE_INDEX_2];
      }
    break;
#endif /* FEATURE_DUAL_SIM */


#ifdef FEATURE_GSM_RX_DIVERSITY
    case GRM_GSM1_SECONDARY:
      grm_info_p = &grm_info[GERAN_RX_DIV_DATA_INDEX_1];
    break;
#ifdef FEATURE_DUAL_SIM
    case GRM_GSM2_SECONDARY:
      grm_info_p = &grm_info[GERAN_RX_DIV_DATA_INDEX_2];
    break;
#endif
#endif /* FEATURE_GSM_RX_DIVERSITY */

#ifdef FEATURE_TRIPLE_SIM
    case GRM_GSM3:
      grm_info_p = &grm_info[GERAN_DATA_SPACE_INDEX_3];
    break;
#endif /* FEATURE_TRIPLE_SIM */

    case GRM_NO_CLIENT:
      break;
    default:
      /*Return grm_info based upon gas_id*/
      {
        int ds = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
        grm_info_p = &grm_info[ds];
      }
    break;
  }
  return ( grm_info_p );
}


#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif /* FEATURE_QSH_MDUMP */

grm_info_t* get_grm_info_ptr(void)
{
   grm_info_t *grm_info_p;
   grm_info_p = grm_info;
   return grm_info_p;
}



/*===========================================================================

FUNCTION      gl1_map_gsm_band_to_sys_band

DESCRIPTION   Maps sys_band_T to sys_band_class_e_type

RETURN VALUE  sys_band_class_e_type

===========================================================================*/
sys_band_class_e_type gl1_map_gsm_band_to_sys_band(sys_band_T band)
{
  sys_band_class_e_type sys_band_e = SYS_BAND_CLASS_NONE;

  switch( band )
  {
    case SYS_BAND_DCS_1800:
      sys_band_e = SYS_BAND_GSM_DCS_1800;
    break;

    case SYS_BAND_PCS_1900:
      sys_band_e = SYS_BAND_GSM_PCS_1900;
    break;

    case SYS_BAND_CELL_850:
      sys_band_e = SYS_BAND_GSM_850;
    break;

    case SYS_BAND_PGSM_900:
      sys_band_e = SYS_BAND_GSM_PGSM_900;
    break;

    case SYS_BAND_EGSM_900:
      sys_band_e = SYS_BAND_GSM_EGSM_900;
    break;

    default:
      MSG_GERAN_ERROR_1( "Unsupported RF band:%d",
                         band );
    break;
  }

  return ( sys_band_e );
}

/*===========================================================================

FUNCTION      gl1_map_sys_band_to_gsm_band

DESCRIPTION   Maps sys_band_class_e_type to sys_band_T

RETURN VALUE  sys_band_T

===========================================================================*/
sys_band_T gl1_map_sys_band_to_gsm_band(sys_band_class_e_type band_e)
{
  sys_band_T sys_band = SYS_BAND_NONE;

  switch( band_e )
  {
    case SYS_BAND_GSM_DCS_1800:
      sys_band = SYS_BAND_DCS_1800;
    break;

    case SYS_BAND_GSM_PCS_1900:
      sys_band = SYS_BAND_PCS_1900;
    break;

    case SYS_BAND_GSM_850:
      sys_band = SYS_BAND_CELL_850;
    break;

    case SYS_BAND_GSM_PGSM_900:
      sys_band = SYS_BAND_PGSM_900;
    break;

    case SYS_BAND_GSM_EGSM_900:
      sys_band = SYS_BAND_EGSM_900;
    break;

    default:
      MSG_GERAN_ERROR_1( "Unsupported RF Sys band:%d",
                         band_e );
    break;
  }

  return ( sys_band );
}

/*===========================================================================

FUNCTION  grm_check_valid_freq_info

DESCRIPTION
  Determines whether a GRM client data is valid and if not
  auto-initialise.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
boolean grm_check_valid_freq_info(grm_info_t *grm_info_p,
                                  gas_id_t    gas_id )
{
  boolean auto_add_band_info = FALSE;
  ARFCN_T serving_cell = gl1_get_serving_cell_arfcn(gas_id);

  if ( grm_info_p )
  {
    /* Use by both ISR and Task*/
    gl1_grm_info_mutex_lock(gas_id);
    if ( grm_info_p->grm_freq_info.num_bands > MAX_BAND_INFO_GRM )
    {
      MSG_GERAN_ERROR_1_G( "grm_check_valid_freq_info called with invalid num_bands:%d in grm_info",
                           grm_info_p->grm_freq_info.num_bands );

      grm_info_p->grm_freq_info.num_bands = 0;
    }

    /* check to see if request is being called without any valid band information */
    if ( grm_info_p->grm_freq_info.num_bands == 0 )
    {
      if ( gl1_drds_debug( gas_id ) )
      {
        MSG_GERAN_LOW_0_G( GL1_DRDS_DBG_HDR"grm_check_valid_freq_info auto-init" );
      }

      grm_info_p->grm_freq_info.num_bands     = 1;
      grm_info_p->grm_freq_info.bands[0].band =
        gl1_map_gsm_band_to_sys_band( serving_cell.band );

      auto_add_band_info = TRUE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      /*make sure the serving cell band is always registered*/
      if(serving_cell.band != INVALID_BAND)
      {
       grm_info_p->persistent_denial.denied_band_info[serving_cell.band].band_registered = TRUE;
      }
#endif
    }
    else
    {
      MSG_GERAN_LOW_2_G( GL1_DRDS_DBG_HDR"grm_check_valid_freq_info:%d num_bands:%d",
                          grm_info_p->grm_freq_info.bands[0].band,
                          grm_info_p->grm_freq_info.num_bands );
    }
    gl1_grm_info_mutex_unlock(gas_id);
  }

  return ( auto_add_band_info );
}

/*===========================================================================

FUNCTION  grm_release

DESCRIPTION
     Releases the chain held by a client and chains held by its associated
	 TX and DIV clients.

	 An Explicit TRM release is required when the TRM request is denied.

DEPENDENCIES
  None

RETURN VALUE
  boolean TRUE indicates TRM requested to release chain

SIDE EFFECTS

===========================================================================*/
void grm_release(grm_client_enum_t client_id, gas_id_t gas_id)
{
  trm_release_chain_input_info *grm_release_chain_input_info_data_p;
  /* Always clear to avoid spurious data */
  trm_release_chain_input_info  grm_release_chain_input_info_data = { 0 };
#ifdef FEATURE_GSM_RX_TX_SPLIT
  grm_client_enum_t tx_client_id = grm_map_rx_to_tx_client_id(client_id, gas_id);
  trm_get_info_input_type   input_data = { 0 };
  trm_get_info_input_type  *input_data_p = &input_data;
  trm_get_info_return_type  output_data = { 0 };
  trm_get_info_return_type *output_data_p = &output_data;
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  grm_grouped_request_bitmap_t client_bitmap;
  gl1_trm_state_t divrx_trm_status;


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  boolean persistent_denial_enabled =  FALSE;

  if (grm_get_resource_lock_state(gas_id))
  {
    MSG_GERAN_HIGH_0_G( GL1_DRDS_DBG_HDR"grm_release ignored - TRM resource lock is active" );
    return;
  }

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  /*Initializing Fields*/
  client_bitmap.txRequest  = FALSE;
  client_bitmap.drxRequest = FALSE;
  
  /*Order of releasing chains, TX, DRX and PRX.
    Since DRX is always associated with PRX chain, therefore on releasing PRX chain first before DRX
    TRM can grant DRX chain to other client whist G is still holding DRX chain.
    */

 /*Det whether Tx Chain is to be released*/
#ifdef FEATURE_GSM_RX_TX_SPLIT
  if (tx_client_id != GRM_NO_CLIENT)
  {
    input_data_p->client_id = grm_map_grm_client_id_to_trm_client_id(tx_client_id);
    input_data_p->info_type = TRM_CLIENT_INFO;
    input_data_p->input_data.client_info.requested_client_id = input_data_p->client_id;

    /* retrieve the TX client's current status */
    trm_get_info( input_data_p, output_data_p );

    /* release TX client only when it holds a chain or has pending request */
    if ( output_data_p->info.client_info.reason != TRM_NO_REASON )
    {
      client_bitmap.txRequest = TRUE;
    }
    else
    {
      client_bitmap.txRequest = FALSE;
      /* clear trm_status for tx client. In case of trm_exchange(), since GL1 does not own
         tx device GL1 won't go into the above brance to release tx chain. So tx_trm_status
         is left as trm_granted still. Need to clear it here.
       */
       grm_set_trm_status(GL1_TRM_NULL, tx_client_id, gas_id);
  }
  }/*if valid Tx client*/

#endif /* FEATURE_GSM_RX_TX_SPLIT */

  /* Determine if Diversity chain is to be released*/
  divrx_trm_status = grm_get_trm_status(grm_map_prx_to_divrx_client_id(client_id, gas_id), gas_id);

  if((GL1_TRM_GRANTED == divrx_trm_status)
      || (GL1_TRM_CALLBACK_PENDING == divrx_trm_status)
      || (GL1_TRM_RETAINED_FOR_ACCESS == divrx_trm_status)
    )
  {
    client_bitmap.drxRequest = TRUE;
  }
  else
  {
    client_bitmap.drxRequest = FALSE;
  }

  /*If Tx or Div to be released then grouped release*/
  if (client_bitmap.drxRequest || client_bitmap.txRequest)
  {
      grm_grouped_trm_release(client_id, client_bitmap, gas_id);
  }
  else
  {
  grm_release_chain_input_info_data_p         =
    &grm_release_chain_input_info_data;

  grm_release_chain_input_info_data_p->client =
                         grm_map_grm_client_id_to_trm_client_id(client_id);

  grm_release_chain_input_info_data_p->tag    = (trm_tag_type)GRM_RELEASE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /*if we have had an abort check and release any ongoing activity band registration in TRM*/
  grm_send_band_release_if_required(client_id, gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"GL1 call to trm_release_chain client_id:%d curr sclk:%d",
                      client_id, timetick_get_safe() );

  /* Release explicitly here calling underlying trm api */
  trm_release_chain( grm_release_chain_input_info_data_p );
  }/*Else no Tx or DivRx rel reqd*/

#ifdef FEATURE_GSM_RX_TX_SPLIT
  /* always reset the status of the RX client */
  grm_set_rx_tx_status(GRM_RX_TX_NULL, client_id, gas_id);
  grm_set_rx_tx_status(GRM_RX_TX_NULL, l1_tsk_buffer[gas_id].client_id, gas_id);
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  grm_gl1_set_current_trm_reason( TRM_NUM_REASONS, SYS_PROC_TYPE_NONE, client_id, gas_id );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /*Clear the freq_band_info,
    if persistent denial is not enabled */
  persistent_denial_enabled = grm_persistent_denial_monitoring(GRM_CHECK_PERSISTENT_DENIAL_FLAG,
                                                               client_id,
                                                               GRM_NULL_COMMAND,
                                                               gas_id);
  if (!persistent_denial_enabled)
  {
    MSG_GERAN_LOW_0_G(GL1_DRDS_DBG_HDR"Clear grm freq_store as persistent denial is not enabled");
    grm_clear_freq_info(client_id, gas_id);
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  grm_set_trm_status(GL1_TRM_NULL, client_id, gas_id);

  return;
}

/*===========================================================================

FUNCTION  grm_release_client_only

DESCRIPTION
     Releases the chain held by a only given client

DEPENDENCIES
  None

RETURN VALUE
  boolean TRUE indicates TRM requested to release chain

SIDE EFFECTS

===========================================================================*/
boolean grm_release_client_only(grm_client_enum_t client_id, gas_id_t gas_id )
{
  trm_release_chain_input_info *grm_release_chain_input_info_data_p;
  /* Always clear to avoid spurious data */
  trm_release_chain_input_info  grm_release_chain_input_info_data = { 0 };

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  boolean persistent_denial_enabled =  FALSE;

  if (grm_get_resource_lock_state(gas_id))
  {
      MSG_GERAN_HIGH_0_G( GL1_DRDS_DBG_HDR"grm_release ignored - TRM resource lock is active" );
      return FALSE;
  }

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  grm_release_chain_input_info_data_p         =
    &grm_release_chain_input_info_data;

  grm_release_chain_input_info_data_p->client =
                         grm_map_grm_client_id_to_trm_client_id(client_id);

  grm_release_chain_input_info_data_p->tag    = (trm_tag_type)GRM_RELEASE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /*if we have had an abort check and release any ongoing activity band registration in TRM*/
  grm_send_band_release_if_required(client_id, gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"GL1 call to trm_release_chain client_id:%d curr sclk:%d",
                      client_id, timetick_get_safe() );

  /* Release explicitly here calling underlying trm api */
  trm_release_chain( grm_release_chain_input_info_data_p );

  grm_gl1_set_current_trm_reason( TRM_NUM_REASONS, SYS_PROC_TYPE_NONE, client_id, gas_id );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /*Clear the freq_band_info,
    if persistent denial is not enabled */
  persistent_denial_enabled = grm_persistent_denial_monitoring(GRM_CHECK_PERSISTENT_DENIAL_FLAG,
                                                               client_id,
                                                               GRM_NULL_COMMAND,
                                                               gas_id);
  if (!persistent_denial_enabled)
  {
    MSG_GERAN_LOW_0_G(GL1_DRDS_DBG_HDR"Clear grm freq_store as persistent denial is not enabled");
    grm_clear_freq_info(client_id, gas_id);
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
#ifdef FEATURE_GSM_RX_TX_SPLIT
    /* always reset the status of the RX client */
    grm_set_rx_tx_status(GRM_RX_TX_NULL, client_id, gas_id);
    grm_set_rx_tx_status(GRM_RX_TX_NULL, l1_tsk_buffer[gas_id].client_id, gas_id);
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  return TRUE;
}

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
/*===========================================================================

FUNCTION  grm_release_pchxfer

DESCRIPTION
     Releases the RX chain held by a client

DEPENDENCIES
  None

RETURN VALUE
  boolean TRUE indicates TRM requested to release chain

SIDE EFFECTS

===========================================================================*/
boolean grm_release_pchxfer(grm_client_enum_t client_id, gas_id_t gas_id )
{
  trm_release_chain_input_info *grm_release_chain_input_info_data_p;
  /* Always clear to avoid spurious data */
  trm_release_chain_input_info  grm_release_chain_input_info_data = { 0 };

  grm_release_chain_input_info_data_p         =
    &grm_release_chain_input_info_data;

  grm_release_chain_input_info_data_p->client =
                         grm_map_grm_client_id_to_trm_client_id(client_id);

  grm_release_chain_input_info_data_p->tag    = (trm_tag_type)GRM_RELEASE;

  MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"GL1 call to trm_release_chain client_id:%d curr sclk:%d",
                      client_id, timetick_get_safe() );

  /* Release explicitly here calling underlying trm api */
  trm_release_chain( grm_release_chain_input_info_data_p );

  return TRUE;
}
#endif
/*===========================================================================

FUNCTION  grm_change_reason/priority

DESCRIPTION
  Set the reason/priority info for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_change_reason(grm_client_enum_t client_id,
                          trm_reason_enum_t new_reason,
                          sys_proc_type_e_type  sub_reason,
                          gas_id_t          gas_id )
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_modify_data *grm_modify_reason_data_p = NULL;
  trm_modify_chain_state_return_data *grm_modify_state_info_p = NULL;
  boolean return_result = FALSE;

  if ( grm_get_resource_lock_state(gas_id))
  {
     trm_reason_enum_t current_reason = grm_gl1_get_current_trm_reason(client_id,gas_id);
     MSG_GERAN_HIGH_2_G("grm_change_reason %d to %d blocked by resource lock", current_reason, new_reason );
  }
  else
  {
    if ( grm_info_p )
    {
      grm_modify_reason_data_p = grm_get_modify_chain_state_data(client_id, gas_id);

      if (grm_modify_reason_data_p)
      {
        grm_modify_reason_data_p->modify_reason.new_reason = new_reason;

        /* Always set to 0 sclks for GERAN */
        grm_modify_reason_data_p->modify_reason.min_duration = 0;

        grm_modify_reason_data_p->modify_reason.sub_reason = sub_reason;
        /* Wakeup Identifier needs to be set with change reason for PBR redesign */
#ifdef FEATURE_PBR_RANDOMIZATION 
        grm_modify_reason_data_p->modify_reason.wakeup_identifier = gl1_get_FN(gas_id);
        grm_info_p->start_frame = gl1_get_FN(gas_id);
#endif		

        grm_modify_chain_state(client_id, GRM_MODIFY_REASON, gas_id);

        grm_modify_state_info_p = grm_get_modify_state_info(client_id,gas_id);

        if (grm_modify_state_info_p)
        {
          /* Only ever gives back denied/granted immediately */
          if ( TRM_GRANTED == grm_modify_state_info_p->modify_info.mod_reason_info.result )
          {
            return_result = TRUE;
          }
        }
        else
        {
          MSG_GERAN_ERROR_1_G("Null grm_modify_state_info_p! Invalid client_id %d", client_id );
        }
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null grm_modify_reason_data_p! Invalid client_id %d", client_id );
      }
    }
    else
    {
      MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
    }
  }

  return return_result;
}
/*===========================================================================

FUNCTION  grm_change_reason/priority

DESCRIPTION
  Set the reason/priority info for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_change_priority(grm_client_enum_t client_id,
                            trm_reason_enum_t new_priority,
                            sys_proc_type_e_type  sub_reason,
                            gas_id_t          gas_id )
{
    grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );
    
    if ( grm_info_p )
    {
        trm_reason_enum_t current_priority = grm_info_p->reason;
        /* If we already have the lock with this reason, no need to tell TRM the reason change */
        if ( current_priority != new_priority )
        {
  return (grm_change_reason( client_id, new_priority,sub_reason, gas_id ));
}
        else
        {
           MSG_GERAN_MED_1_G("New & current TRM_reasons are same %d", current_priority); 
           return TRUE;
        }
    }
    return FALSE;

}

/*===========================================================================

FUNCTION  grm_check_connected_reason

DESCRIPTION
  Checks that the current connected reason matches the expected connected
  reason and updates the reason and callback if it does not.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_check_connected_reason(grm_client_enum_t client_id,
                                   grm_conn_type_t   connected_type,
                                   gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if (grm_info_p)
  {
    trm_reason_enum_t expected_reason = grm_get_connection_priority(connected_type, gas_id);
    trm_reason_enum_t current_reason = grm_info_p->reason;
    sys_proc_type_e_type expected_subreason;
    sys_proc_type_e_type current_subreason = grm_info_p->sub_reason;
    grm_client_enum_t tx_client_id = GRM_NO_CLIENT;

    if (connected_type == GRM_CS_CONNECTION)
    {
      expected_subreason = gl1_get_specific_sys_proc(GERAN_CONN_PROCEDURE, gas_id);
    }
    else
    {
      expected_subreason = gl1_get_specific_sys_proc(GERAN_GPRS_PROCEDURE, gas_id);
    }

    if ((grm_info_p->trm_grant_status != TRM_DENIAL) &&
        ((current_reason != expected_reason) ||
         (current_subreason != expected_subreason)))
    {
      grm_unlock_callback_t old_callback = grm_info_p->grm_unlock_callback;
      grm_unlock_callback_t new_callback = grm_info_p->grm_unlock_callback;

      if (GRM_CS_CONNECTION == connected_type)
      {
        if (expected_reason == TRM_TRAFFIC)
        {
          new_callback = (grm_unlock_callback_t)&grm_cs_traffic_abort_callback;
        }
        else /* expected_reason == TRM_ACCESS */
        {
          new_callback = (grm_unlock_callback_t)&grm_cs_access_abort_callback;
        }
      }
      else /* connected_type == GRM_PS_CONNECTION */
      {
        if (expected_reason == TRM_TRAFFIC)
        {
          new_callback = (grm_unlock_callback_t)&grm_ps_traffic_abort_callback;
        }
        else if (expected_reason == TRM_ACCESS)
        {
          new_callback = (grm_unlock_callback_t)&grm_ps_access_abort_callback;
        }
        else  /* expected_reason == TRM_BG_TRAFFIC */
        {
          new_callback = (grm_unlock_callback_t)&grm_ps_bg_traffic_abort_callback;
        }
      }

      if ((expected_reason == TRM_BG_TRAFFIC) ||
          ((expected_reason == TRM_ACCESS) && (current_reason == TRM_TRAFFIC)))
      /* Callback could fire immediately so install it first */
      {
        if (new_callback != old_callback)
        {
        grm_retain_lock (client_id,new_callback,gas_id);
        }
        if (!grm_change_reason(client_id,expected_reason,expected_subreason,gas_id))
        {
          if (new_callback != old_callback)
        {
          /* and replace the original callback if we were unable to change the reason */
          grm_retain_lock (client_id,old_callback,gas_id);
          return; /* Connected mode unlock callback and reason are unchanged */
        }
      }
      }
      else if ((expected_reason == TRM_TRAFFIC) && (current_reason == TRM_ACCESS))
      {
#ifdef FEATURE_GSM_RX_TX_SPLIT
        if( grm_is_a_tx_client(client_id) )
        { 
         /* This implies that the RaN callback for TX client TRM_TRAFFIC has happened.
          * Need to map tx to rx for use in grm_change_reason.
         */
          tx_client_id = client_id;
          client_id = grm_map_tx_to_rx_client_id(client_id,gas_id);
        }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

        if ( !grm_change_reason(client_id, expected_reason, expected_subreason, gas_id) &&
              tx_client_id == GRM_NO_CLIENT )
        {
          /* Modify reason from ACCESS to TRAFFIC failed  */
          MSG_GERAN_HIGH_3_G("connected mode reason change failed! %d->%d procedure=%d",
                             (int)current_reason,
                             (int)expected_reason,
                             (int)expected_subreason);
#ifdef FEATURE_GSM_RX_TX_SPLIT

          /*Grouped Request*/
          grm_grouped_request_and_notify(client_id,
                                         GRM_ACCESS_DURATION_SCLKS,
                                         expected_reason,
                                         expected_subreason,
                                         GRM_DIVRX_TRM_REQ_INVALID,
                                         GRM_RAN_MODIFY_TO_TRAFFIC_REASON,
                                         gas_id);

#else/*FEATURE_GSM_RX_TX_SPLIT*/
#ifdef FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY
          /* cant handle change reason failure when RX/TX Split not supported
           * abort the CS call
           */
          L1_send_PH_CS_ABORT_IND(GL1_CS_ABORT_UNKNOWN, DCCH, gas_id);
#endif /* FEATURE_GERAN_RF_TIMELINE_ERROR_RECOVERY */

#endif /*FEATURE_GSM_RX_TX_SPLIT*/

          return;
        } else 
        {
          if ( tx_client_id != GRM_NO_CLIENT )
          {
             /* if here implies that Rx client will be with ACCESS reason and
              *  Tx client with TRAFFIC
              */
             MSG_GERAN_HIGH_0_G("Change reason denied after R&N for Tx client");
          }
          /* reason has been modified , retain lock with traffic cb and enable DRDSDS*/
          if ( new_callback != old_callback )
          {
            grm_retain_lock(client_id, new_callback, gas_id);
          }
        }

      }
      else /* Change to higher priority reason before installing the new callback */
      {
        if (grm_change_reason(client_id,expected_reason,expected_subreason,gas_id))
        {
          if (new_callback != old_callback)
        {
          grm_retain_lock (client_id,new_callback,gas_id);
        }
        }
        else
        {
          return; /* Connected mode unlock callback and reason are unchanged */
        }
      }

      MSG_GERAN_HIGH_3_G("connected mode reason changed %d->%d procedure=%d",
                         (int)current_reason,
                         (int)expected_reason,
                         (int)expected_subreason);
    }
  }
  return;
}


/*===========================================================================

FUNCTION  grm_request

DESCRIPTION
  Forwards a GSM client request to the TRM layer.

  In single sim mode, GSM L1 will request the TRM lock when processing the
  START GSM MODE REQ and will release it when processing the STOP GSM MODE REQ.

  GSM L1 registers call backs with TRM when processing the START GSM MODE REQ,
  deregistration of these call backs never occurs.

  On the tx chain side, TRM should not assign two tx devices to two techs at
  the same time, regardless of whether the two devices are identical or not.

  TRM is still aware of clients even when GSM L1 does not register call backs
  for them.Example: In dual sim mode, if GSM1 RX SECONDARY requests a retain
  lock from TRM then a callback will be triggered to GSM1 TX.


DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  May intercept requests for TRM_DEMOD_PAGE and apply priority inversion

===========================================================================*/
trm_grant_return_enum_type grm_request(grm_client_enum_t   client_id,
                                       trm_duration_t      duration,
                                       trm_reason_enum_t   reason,
                                       sys_proc_type_e_type sub_reason,
                                       trm_resource_enum_t resource,
                                       grm_divrx_request_type req_type)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
  boolean non_valid_freq_info;

  gas_id_t gas_id = gl1_map_client_id_to_gas_id( client_id );
  trm_reason_enum_t req_reason = reason;
  trm_resource_enum_t req_resource = resource;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );
  trm_grant_return_enum_type  grm_request_grant = TRM_DENIAL;
#ifdef FEATURE_GPRS_GBTA
  gas_id_t tbf_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
#endif
  rfm_device_enum_type rf_device = RFM_INVALID_DEVICE;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;
  gl1_trm_state_t tx_trm_status = GL1_TRM_GRANTED;

#ifdef FEATURE_GSM_RX_TX_SPLIT
  grm_client_enum_t tx_client_id = grm_map_rx_to_tx_client_id(client_id, gas_id);
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if ( !grm_info_p || ( gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED ) )
  {
    MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"GRM Request with no valid grm_info for client_id:%d grant:%d",
                         client_id, grm_request_grant );

    return ( grm_request_grant );
  }

  GL1_TRM_LOCK();

#ifdef FEAT_DIFF_SIG_PATH_PER_GSM_BAND
  /* Request for all GSM bands in case reason is ACCESS or TRAFFIC or BG_TRAFFIC */
  if (grm_is_tx_reason(req_reason) && (sub_reason != SYS_PROC_TYPE_PAGE))
  {
    grm_set_valid_acquisition_bands(client_id,gas_id);
    grm_clear_freq_info(client_id, gas_id);

    for(idx=0;idx<grm_valid_gsm_sys_bands_cnt[gas_id];idx++)
    {
      grm_add_trm_band_to_freq_input_type(grm_valid_gsm_sys_bands[gas_id][idx], client_id, gas_id);
    }
  }
#endif

  non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

#ifdef FEATURE_GSM_RX_TX_SPLIT
  /* Check GL1 needs TX chain
    In Single SIM: reason is TRAFFIC and resource is RXTX_ANY */
  if (FALSE == gl1_msg_get_multi_sim_mode())
  {
    if ((TRM_TRAFFIC == reason) &&
        (TRM_RX_ANY ==  resource) &&
        (tx_client_id != GRM_NO_CLIENT))
    {
      tx_trm_status = grm_request_tx_chain(tx_client_id,
                                           duration,
                                           reason,
                                           sub_reason,
                                           TRM_TX_ANY,
                                           gas_id);
       /*update RF device on each trm request either grant or denial. */
      rf_device  = grm_get_rf_device(tx_client_id,
                                     gas_id );

      /*update internal HW device ID*/
      gl1_hw_set_gsm_rf_id (rf_device, tx_client_id, FALSE, gas_id);

      /* Reset to Invalid for Rx Chain*/
      rf_device = RFM_INVALID_DEVICE;
    }
    /*Change resource for Rx*/
    req_resource = TRM_RX_ANY;
  }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  if (GL1_TRM_GRANTED == tx_trm_status)
  {
    trm_status = grm_get_trm_status(client_id, gas_id);

    if ( ( trm_status == GL1_TRM_GRANTED ) ||
       ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS ) )
    {
      grm_change_priority( client_id, reason, sub_reason, gas_id );

      if ( non_valid_freq_info )
      {
        grm_clear_freq_info( client_id, gas_id );
      }

    GL1_TRM_UNLOCK();

    return ( grm_get_trm_grant_status(client_id, gas_id) );
  }

#if defined (FEATURE_DUAL_SIM) || defined (FEATURE_TRIPLE_SIM)
  if (grm_get_resource_lock_state(gas_id))
  {
    GL1_TRM_UNLOCK();
    MSG_GERAN_HIGH_0_G( GL1_DRDS_DBG_HDR"grm_request returns existing grant status only - "
                                        "TRM resource lock is active" );
    return grm_get_trm_grant_status(client_id, gas_id);
  }

  if (IS_URGENT_ACCESS(req_reason))
  {
    /* if GSM call setup occurs just after WCDMA has opened a QTA or GTA*/
    /* gap, then this check should allow their client to clean up */
    /* grm_release clears frequency information. Don't call in Single SIM*/
    if (grm_get_trm_grant_status(client_id, gas_id) == TRM_DENIAL &&
        gl1_hw_qta_gap_active(gas_id) &&
        gl1_msg_get_multi_sim_mode())
    {
      /* perform a release here since we don't hold the lock */
      /* but may have a previous reservation pending in TRM  */
      grm_release( client_id, gas_id );
    }
  }
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

#ifdef FEATURE_GPRS_GBTA
  tbf_gas_id = gl1_get_gas_id_for_ptm_client();

  if ( OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)  &&
       gl1_msg_get_multi_sim_standby_mode() &&
      (client_id == gpl1_gprs_tbf_abort_cb_store_data[OTHER_G_SUB_IN_TRANSFER(gas_id)].last_client) &&
      (!grm_info_p->wreconf) &&
      (!gl1_drdsds_enabled(gas_id)) &&
       gl1_gbta_check_gbta_allowed(req_reason,L1_SC_DURATION_GBTA,gas_id)
     )
  {
    gas_id_t other_g_gas_id  = gl1_get_gas_id_of_other_g_sub(gas_id);
    /* above check valid only in dsds, hence always chain_0 */
    grm_info_p->grm_request_data.grant = grm_request_grant = TRM_GRANTED;

    SET_GBTA_STATUS(GRM_GBTA_ACTIVE);
    MSG_GERAN_HIGH_2_G("GL1 GBTA ACTIVE FN:%d reason:%d",gl1_get_FN(gas_id),req_reason);

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_GBTA);
#endif

    /* stop SCE to avoid clash */
    if(tbf_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED)
    {
      OTHER_SUB_STOP_SC_TICK_TRANS(tbf_gas_id);
    }

    /*Get RF device of other SUB for GBTA*/
    rf_device =  gl1_hw_get_gsm_rf_id(gl1_map_gas_id_to_client_id(other_g_gas_id),
                                      other_g_gas_id);

    /*update the HW device */
    gl1_hw_set_gsm_rf_id(rf_device, client_id, FALSE, gas_id);

#if defined ( FEATURE_GSM_GPRS_READ_CCCH_IN_XFER) || defined (FEATURE_PBR_RANDOMIZATION)
#ifdef FEATURE_PBR_RANDOMIZATION
    if ( req_reason == TRM_DEMOD_PAGE ) 
#endif
    {
    grm_inform_grant_event(client_id,
                           resource,
                           duration,
                           req_reason,
                           TRM_GRANTED_CHAIN0
                           );
    }
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

  }
  else
#endif /* FEATURE_GPRS_GBTA */
  {

#if ((defined(FEATURE_SGLTE_DUAL_SIM ) || defined(FEATURE_SVLTE_DUAL_SIM )) && defined(FEATURE_QTA))
    if ( (
#ifdef FEATURE_SGLTE_DUAL_SIM
           ( gl1_hw_get_sglte_mode(gas_id) &&
             (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
           )
#endif /*FEATURE_SGLTE_DUAL_SIM*/
#if defined(FEATURE_SGLTE_DUAL_SIM ) && defined(FEATURE_SVLTE_DUAL_SIM )
           ||
#endif /*FEATURE_SGLTE_DUAL_SIM && FEATURE_SVLTE_DUAL_SIM*/
#ifdef FEATURE_SVLTE_DUAL_SIM
           ( gl1_check_svlte_mode_active() )
#endif /*FEATURE_SVLTE_DUAL_SIM*/
        ) &&
        ( gpl1_get_grm_qta_state(gas_id) && (geran_map_nas_id_to_gas_id(gl1_hw_qta_return_cm_as_id(gas_id)) == gas_id) ) )
    {
      req_resource = TRM_RX_SECOND_PREFERRED;
    }
#endif /*(FEATURE_SGLTE_DUAL_SIM || FEATURE_SVLTE_DUAL_SIM) && FEATURE_QTA*/

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /*Call band release before request call to TRM*/
    grm_send_band_release_if_required(client_id, gas_id);
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

    grm_request_grant = grm_handle_trm_request(client_id,
                                               duration,
                                               req_reason,
                                               sub_reason,
                                               req_resource,
                                               req_type,
                                               gas_id);

											   
    if(grm_request_grant == TRM_GRANTED) 
    { 
#ifdef FEATURE_PBR_RANDOMIZATION 
      if ( req_reason == TRM_DEMOD_PAGE )
      {
        grm_inform_grant_event(client_id,
                           req_resource,
                           duration,
                           req_reason,
                           TRM_GRANTED_CHAIN0
                           );
      }
#endif
      MSG_GERAN_HIGH_1_G( "W RECONF %d",grm_info_p->wreconf); 
      grm_info_p->wreconf = FALSE; 
    }  
    /*update RF device on each trm request either grant or denial. */
    rf_device  = grm_get_rf_device(client_id,
                                   gas_id );

    /*update internal HW device ID*/
    gl1_hw_set_gsm_rf_id (rf_device, client_id, FALSE, gas_id);
  }

  /* Only return rf device info if not denied */
  if ( grm_request_grant != TRM_DENIAL )
  {
    MSG_GERAN_HIGH_3_G( "MS: reserve_at_count:%d granted chain:%d rf_device:%d",
                        reserve_at_count[gas_id], grm_request_grant, rf_device );

    /*Update local stored trm_reason*/
    grm_gl1_set_current_trm_reason(req_reason, sub_reason, client_id, gas_id);
  }
  else
  {
#ifdef FEATURE_PBR_RANDOMIZATION 
    if ( req_reason == TRM_DEMOD_PAGE ) 
    {
      grm_inform_grant_event(client_id,
                             req_resource,
                             duration,
                             req_reason,
                             TRM_DENIED
                             );
    }
#endif  
    MSG_GERAN_HIGH_2_G( "MS: reserve_at_count:%d grm_request_grant:%d",
                        reserve_at_count[gas_id], grm_request_grant );
    grm_gl1_set_current_trm_reason(TRM_NUM_REASONS, SYS_PROC_TYPE_NONE, client_id, gas_id);
  }


#ifdef FEATURE_QTA
  /*
   ** LTE_GENERIC_QTA This now checks other tech has initiated the QTA gap by calling gl1_hw_qta_startup(), gl1_qta_wcdma_has_initialised.
   */
#ifdef FEATURE_SGLTE
  if ( gl1_hw_get_sglte_mode(gas_id) &&
       ( gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ) )
  {
    if ( gpl1_get_grm_qta_state(gas_id) && (geran_map_nas_id_to_gas_id(gl1_hw_qta_return_cm_as_id(gas_id)) == gas_id))
    {
      if ( grm_request_grant != TRM_DENIAL )
      {
        l1_tskisr_blk->qta_in_progress = TRUE;
      }
      else
      {
        MSG_GERAN_HIGH_0_G( "Other Tech initiated qta gap but TRM not granted" );
      }
    }
  }
  else
#endif /*FEATURE_SGLTE*/
  {
    l1_tskisr_blk->qta_in_progress =
     ( gpl1_get_grm_qta_state(gas_id) && ( grm_request_grant != TRM_DENIAL ) );
  }
#endif /* FEATURE_QTA */

#if defined(FEATURE_QBTA)
  if((l1_tskisr_blk->qta_in_progress) && (!IS_QBTA_REASON(reason, l1_fm_get_reserve_sub_reason(gas_id))))
  {
    gl1_hw_set_QBTA_flag(gas_id,FALSE);
  }
#endif /*FEATURE_QBTA*/

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  if(l1_tskisr_blk->qta_in_progress)
  {
    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_QTA);
  }
#endif
  /* Single SIM: Seperate TRM call for DRX chain
     MSIM: Grouped*/
  if ((FALSE == gl1_msg_get_multi_sim_mode())
      && (TRM_GRANTED == grm_request_grant)
      && (req_type != GRM_DIVRX_TRM_REQ_INVALID))
  {
    (void)grm_request_diversity_chain(grm_map_prx_to_divrx_client_id(client_id, gas_id),
                                      duration,
                                      grm_request_grant,
                                      req_type,
                                      gas_id);
  }

#ifdef FEATURE_GSM_RX_TX_SPLIT
  if ((FALSE == gl1_msg_get_multi_sim_mode()) && (TRM_GRANTED != grm_request_grant))
  {
    /* TRM not granted for Rx Chain. Release all Chains (Rx, Tx, Diversity)
     * Ideally, GL1 should not execute code under if statement because in single sim configuration TRM
     * is bound to grant the Rx and Tx chains.
     * If Rx is not granted, then release all chains.
     */
    grm_release(client_id, gas_id);
  }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  }

  if ( non_valid_freq_info )
  {
    grm_clear_freq_info( client_id, gas_id );
  }

  GL1_TRM_UNLOCK();

  return ( grm_request_grant );
} /* grm_request */

/*===========================================================================

FUNCTION  grm_request_rx_sec

DESCRIPTION
  Forwards a GSM client request to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  May intercept requests for TRM_DEMOD_PAGE and apply priority inversion

===========================================================================*/
trm_grant_return_enum_type grm_request_rx_sec(grm_client_enum_t   divrx_client_id,
                                              trm_duration_t      duration,
                                              trm_reason_enum_t   reason,
                                              sys_proc_type_e_type sub_reason,
                                              trm_resource_enum_t resource )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
  boolean non_valid_freq_info;

  gas_id_t gas_id = gl1_map_client_id_to_gas_id(divrx_client_id);
  trm_reason_enum_t req_reason = reason;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(divrx_client_id, gas_id);
  grm_info_t *grm_prx_info_p = NULL;
  trm_grant_return_enum_type  grm_request_grant = TRM_DENIAL;
  trm_request_input_info      *gl1_trm_request_input_info_p;
  trm_get_info_data           *gl1_trm_get_info_input_info_p;
    /* Always clear to avoid spurious data */
  trm_request_input_info  gl1_trm_request_input_info  = { 0 };
  trm_get_info_data       gl1_trm_get_info_input_info = { {0} };
  gl1_trm_state_t         trm_status = grm_get_trm_status(divrx_client_id, gas_id);
  rfm_device_enum_type    rf_device = RFM_INVALID_DEVICE;
  gl1_hw_rxdiv_data_t     *rxdiv_data_ptr = gl1_hw_get_gsm_rxdiv_data_ptr(gas_id);

  int32 i;

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if ( !grm_info_p || ( gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED ) )
  {
    MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"grm_request_rx_sec with no valid "
                                         "grm_info for client_id:%d grant:%d",
                         divrx_client_id, grm_request_grant );

    return ( grm_request_grant );
  }

  GL1_TRM_LOCK();

  non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

  if ( ( trm_status == GL1_TRM_GRANTED ) ||
       ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS ) )
  {
    /* No need to change the reason of diversity chain as there is only one reason for
       diversity which is TRM_DIVERSITY*/
    if ( non_valid_freq_info )
    {
      grm_clear_freq_info( divrx_client_id, gas_id );
    }

    GL1_TRM_UNLOCK();

    return ( grm_get_trm_grant_status(divrx_client_id, gas_id) );
  }

#if defined (FEATURE_DUAL_SIM) || defined (FEATURE_TRIPLE_SIM)
  if (grm_get_resource_lock_state(gas_id))
  {
    GL1_TRM_UNLOCK();
    MSG_GERAN_HIGH_0_G( GL1_DRDS_DBG_HDR"grm_request_rx_sec return existing grant "
                                        "status only - TRM resource lock is active" );
    return grm_get_trm_grant_status(divrx_client_id, gas_id);
  }
#endif

  grm_info_p->wreconf = FALSE;

  gl1_trm_request_input_info_p  = &gl1_trm_request_input_info;
  gl1_trm_get_info_input_info_p = &gl1_trm_get_info_input_info;

  /* ensure trm request for secondary chain is populated with same (or subset) of
     frequency information as that for the primary chain request*/
  grm_prx_info_p = grm_get_info_ptr_from_client(grm_info_p->prx_client, gas_id);

  /* If PRx made multi-band request, then get DRx device mapping before making DRx request
     in order to avoid requesting for unsupported DRx bands */
    /* Get diversity device band mapping */
  gl1_trm_get_info_input_info_p->band_mapping_info.resource = TRM_RX_DIVERSITY;
       
  /* Request all default supported bands */
  for ( i = 0; i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id]; i++ )
  {
    gl1_trm_get_info_input_info_p->band_mapping_info.band[i] = grm_valid_gsm_sys_bands_divrx[gas_id][i];
  }

  gl1_trm_get_info_input_info_p->band_mapping_info.num_bands = grm_valid_gsm_sys_bands_cnt_divrx[gas_id];

  rf_device = grm_get_device_mapping( divrx_client_id, gl1_trm_get_info_input_info_p, gas_id );

  /* clear supported RxD band mask */
  rxdiv_data_ptr->gl1_hw_DivRx_band_supported = 0;

  if ( grm_info_p )
  {
    for (i = 0; ((i < gl1_trm_get_info_input_info_p->band_mapping_info.num_bands) && (i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id])); i++)
    {
      if (grm_info_p->grm_get_info_data.info.band_mapping_info.device[i] >= RFM_MAX_DEVICES)
      {
        MSG_GERAN_ERROR_1_G( "gl1_hw_trm_init_DivRx: INVALID DEVICE detected for band %d",
                            gl1_trm_get_info_input_info_p->band_mapping_info.band[i] );
        rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band = 0;
      }
      else
      {
        rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band = 
           gl1_trm_get_info_input_info_p->band_mapping_info.band[i];

        rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].device = 
           grm_info_p->grm_get_info_data.info.band_mapping_info.device[i];

        rxdiv_data_ptr->gl1_hw_DivRx_band_supported |=
           gl1_hw_map_sys_band_to_gsm_rxdiv_band(rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band,gas_id);
      }
    }
  }

  /* preset num_bands */
  gl1_trm_request_input_info_p->freq_info.num_bands = 0;

  if(grm_prx_info_p)
  {
    for(i=0; ((i<grm_prx_info_p->grm_request_data.num_bands) && (i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id])); i++)
    {
      if(gl1_hw_gsm_trm_band_supported(grm_prx_info_p->grm_request_data.band_alloc[i].band.band, gas_id))
      {
         gl1_trm_request_input_info_p->freq_info.bands[gl1_trm_request_input_info_p->freq_info.num_bands].band 
            = grm_prx_info_p->grm_request_data.band_alloc[i].band.band;
         gl1_trm_request_input_info_p->freq_info.num_bands++;
      }
    }
    MSG_GERAN_HIGH_2_G(" grm_request_rx_sec: Map drx freq info to supported prx info, num bands %d, first band %d",
                       gl1_trm_request_input_info_p->freq_info.num_bands,gl1_trm_request_input_info_p->freq_info.bands[0].band);
  }

  /* If device mapping returned some supported bands, but there are no bands common with PRx request, then request
     for all supported DRx bands */
  if((gl1_trm_request_input_info_p->freq_info.num_bands == 0) && (rxdiv_data_ptr->gl1_hw_DivRx_band_supported != 0))
  {
    for (i = 0; ((i < gl1_trm_get_info_input_info_p->band_mapping_info.num_bands) && (i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id])); i++)
    {
      if(gl1_hw_gsm_trm_band_supported(rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band, gas_id))
      {
         gl1_trm_request_input_info_p->freq_info.bands[gl1_trm_request_input_info_p->freq_info.num_bands].band 
            = rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band;
         gl1_trm_request_input_info_p->freq_info.num_bands++;
      }
    }
    MSG_GERAN_HIGH_2_G(" grm_request_rx_sec: No valid PRx bands, request for all diversity bands, num bands %d, first band %d",
                       gl1_trm_request_input_info_p->freq_info.num_bands,gl1_trm_request_input_info_p->freq_info.bands[0].band);
  }

  /*If we have no valid bands to request, then don't make the request*/
  if(gl1_trm_request_input_info_p->freq_info.num_bands == 0)
  {
    MSG_GERAN_ERROR_0_G("grm_request_rx_sec: no bands supported, so no request made!!");
  }
  else
  {
    gl1_trm_request_input_info_p->client_id  = grm_map_grm_client_id_to_trm_client_id(divrx_client_id);
    gl1_trm_request_input_info_p->resource   = resource;
    gl1_trm_request_input_info_p->duration   = duration;
    gl1_trm_request_input_info_p->reason     = req_reason;
    gl1_trm_request_input_info_p->sub_reason = sub_reason;
    gl1_trm_request_input_info_p->tag        = (trm_tag_type)GRM_REQUEST;

    /* forward the request to TRM */
    trm_request( gl1_trm_request_input_info_p, &grm_info_p->grm_request_data );

    /* Detect tag mismatch */
    if ( grm_info_p->grm_request_data.tag != gl1_trm_request_input_info_p->tag )
    {
      MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"grm_request_rx_sec Tag mismatch in:%d out:%d",
                           gl1_trm_request_input_info_p->tag,
                           grm_info_p->grm_request_data.tag );
    }

    grm_request_grant = grm_info_p->grm_request_data.grant;

    /* Only return rf device info if not denied */
    if ( grm_request_grant != TRM_DENIAL )
    {

      rf_device = grm_get_rf_device(divrx_client_id,gas_id);
      /*Update local stored trm_reason*/
      grm_gl1_set_current_trm_reason(req_reason, sub_reason, divrx_client_id, gas_id);

      if (rf_device != RFM_INVALID_DEVICE)
      {
        gl1_hw_set_gsm_rf_id(rf_device, divrx_client_id, TRUE, gas_id);
      }

      MSG_GERAN_HIGH_2_G( "MS: grm_request_rx_sec reserve_at_count:%d granted rf device:%d ",
                          reserve_at_count[gas_id], rf_device );
    }
    else
    {
      /* clear supported RxD band mask */
      rxdiv_data_ptr->gl1_hw_DivRx_band_supported = 0;

      MSG_GERAN_HIGH_2_G( "MS: grm_request_rx_sec reserve_at_count:%d grm_request_grant:%d",
                          reserve_at_count[gas_id], grm_request_grant );
    }

    if ( non_valid_freq_info )
    {
      grm_clear_freq_info( divrx_client_id, gas_id );
    }
  }
  GL1_TRM_UNLOCK();

  return ( grm_request_grant );
} /* grm_request_rx_sec */
/*===========================================================================

FUNCTION grm_validate_diversity_chain_request

DESCRIPTION


PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
boolean grm_validate_diversity_chain_request(
                            trm_grant_return_enum_type  prx_trm_grant_return,
                                            grm_divrx_request_type req_type,
                                            gas_id_t gas_id)
{
  boolean         valid = FALSE;

#ifdef FEATURE_GSM_RX_DIVERSITY
  boolean supported_reason = TRUE, send_trm_req = FALSE;
  l1_fm_priority_T next_pri = FM_PRI_RESERVED;

  switch (req_type)
  {
    case GRM_DIVRX_TRM_REQ_INIT:
    {
      /* hw_init call -
      * we are requesting the rx secondary on start up
      * hence we have no FM activity to run - so we
      * don't need to check the FM reason for the request
      * - so hard set to TRUE
      */
      MSG_GERAN_HIGH_0_G (GL1_RXD"Stack init");
      if(gl1_get_rxd_ctl_nv(gas_id))
      {
        send_trm_req = TRUE;
      }
      break;
    }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    case GRM_DIVRX_TRM_REQ_IDLE:
    {
        /* Get the next priority from FM */
        next_pri = l1_get_fm_trm_priority(gas_id);

      if (next_pri == FM_PRI_LTE_IDSEARCH)
      {
        send_trm_req = TRUE;
      }
      else if (gl1_get_IMRD_ctl_nv(gas_id) && (!gl1_hw_is_saic_dis_in_lmm_mode(gas_id)) && gl1_hw_is_rxdiv_allowed(req_type, gas_id)) 
      {
        supported_reason = gpl1_grm_rx_sec_supported_reason(next_pri);
        send_trm_req = TRUE;
      }
      break;
    }
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
    case GRM_DIVRX_TRM_REQ_DED:
    {
      if(gl1_get_rxd_ctl_nv(gas_id) && gl1_hw_is_rxdiv_allowed(req_type, gas_id))
      {
        send_trm_req = TRUE;
      }
      break;
    }

    case GRM_DIVRX_TRM_REQ_DATA:
    {
      if(gl1_get_MSRD_ctl_nv(gas_id) && gl1_hw_is_rxdiv_allowed(req_type, gas_id))
      {
        send_trm_req = TRUE;
      }
      break;
    }

    default:
      MSG_GERAN_ERROR_1_G (GL1_RXD"Unknown req type %d", req_type);
      break;
  }
  if ((prx_trm_grant_return != TRM_DENIAL) && send_trm_req)
  {
    /*
    * We've got the Primary - see if
    * the pri is supported (or we called from init of stack)
    * We do not request secondary when in DSDA or if SGLTE is active
    */

    if ( supported_reason
         && !(gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE
#ifdef FEATURE_SGLTE
         || gl1_hw_get_sglte_mode(gas_id)
#endif /*FEATURE_SGLTE*/
              )
         )
    {
      valid = TRUE;
    }
  }

  MSG_GERAN_HIGH_3_G(GL1_RXD"grm_validate_diversity_chain_request valid %d "
                            "next_pri %d req_type %d",
                     valid, next_pri, req_type);
#endif /* FEATURE_GSM_RX_DIVERSITY */

  return valid;
}
/*===========================================================================

FUNCTION grm_request_diversity_chain

DESCRIPTION


PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
gl1_trm_state_t grm_request_diversity_chain(
                            grm_client_enum_t           divrx_client_id,
                            trm_duration_t              duration,
                            trm_grant_return_enum_type  prx_trm_grant_return,
                            grm_divrx_request_type      req_type,
                            gas_id_t                    gas_id)
{
  gl1_trm_state_t grm_divrx_chain_gl1_trm_state = GL1_TRM_NULL;

#ifdef FEATURE_GSM_RX_DIVERSITY
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(divrx_client_id, gas_id);
  trm_grant_return_enum_type divrx_grant;

  if (grm_validate_diversity_chain_request(prx_trm_grant_return, req_type, gas_id))
    {
      if (gl1_get_rxd_ctl_nv(gas_id))
      {
      MSG_GERAN_LOW_1_G(GL1_RXD"gpl1_grm_request_rx_sec: req_type %d",
                        req_type);
      }

      /* set the freq info */
      (void)grm_check_valid_freq_info(grm_info_p,gas_id);

      /* Request for div resource */
      divrx_grant = grm_request_rx_sec(divrx_client_id,
                                       duration,
                                       TRM_DIVERSITY,
                                       SYS_PROC_TYPE_DIVERSITY_CHAIN,
                                       TRM_RX_DIVERSITY);

      grm_set_trm_grant_status(divrx_grant, divrx_client_id, gas_id);

      if (divrx_grant != TRM_DENIAL)
      {
        MSG_GERAN_LOW_1_G(GL1_RXD"granted RX_SEC trm grant %d",
                           divrx_grant);
      grm_set_trm_status(GL1_TRM_GRANTED, divrx_client_id, gas_id);
        grm_divrx_chain_gl1_trm_state = GL1_TRM_GRANTED;
      }
      else
      {
        MSG_GERAN_LOW_1_G (GL1_RXD"denied RX_SEC trm grant %d",
                            divrx_grant);
      grm_set_trm_status(GL1_TRM_NULL, divrx_client_id, gas_id);
        grm_divrx_chain_gl1_trm_state = GL1_TRM_NOT_GRANTED;
      }
    }

  MSG_GERAN_HIGH_3_G(GL1_RXD"gpl1_grm_request_rx_sec "
                     "prx trm status %d divrx trm status %d req_type %d",
                     prx_trm_grant_return,
                     grm_get_trm_status(divrx_client_id, gas_id),
                     req_type);
#endif /* FEATURE_GSM_RX_DIVERSITY */
  return grm_divrx_chain_gl1_trm_state;
}

/*===========================================================================

FUNCTION grm_map_band_support_diversity_chain

DESCRIPTION
  Update global RxDiv ptr with all bands supported on Chain granted for Secondary Chain.

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/

void grm_map_band_support_diversity_chain(grm_client_enum_t  divrx_client_id, gas_id_t gas_id)
{

#ifdef FEATURE_GSM_RX_DIVERSITY
      /*Update All bands supported on this device*/
      grm_info_t *grm_info_p = grm_get_info_ptr_from_client(divrx_client_id, gas_id);
      rfm_device_enum_type  rf_device = RFM_INVALID_DEVICE;
      gl1_hw_rxdiv_data_t     *rxdiv_data_ptr = gl1_hw_get_gsm_rxdiv_data_ptr(gas_id);
      int32 i;
      trm_get_info_data           *gl1_trm_get_info_input_info_p;
      trm_get_info_data  gl1_trm_get_info_input_info= { {0} };
      
      gl1_trm_get_info_input_info_p = &gl1_trm_get_info_input_info;
      gl1_trm_get_info_input_info_p->band_mapping_info.resource = TRM_RX_DIVERSITY;

      /* Request all default supported bands */
       for ( i = 0; i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id]; i++ )
       {
         gl1_trm_get_info_input_info_p->band_mapping_info.band[i] = grm_valid_gsm_sys_bands_divrx[gas_id][i];
       }
      
       gl1_trm_get_info_input_info_p->band_mapping_info.num_bands = grm_valid_gsm_sys_bands_cnt_divrx[gas_id];
      
       rf_device = grm_get_device_mapping( divrx_client_id, gl1_trm_get_info_input_info_p, gas_id );
      
       /* clear supported RxD band mask */
       rxdiv_data_ptr->gl1_hw_DivRx_band_supported = 0;
      
       if ( grm_info_p )
       {
         for (i = 0; ((i < gl1_trm_get_info_input_info_p->band_mapping_info.num_bands) && (i < grm_valid_gsm_sys_bands_cnt_divrx[gas_id])); i++)
         {
           if (grm_info_p->grm_get_info_data.info.band_mapping_info.device[i] >= RFM_MAX_DEVICES)
           {
             MSG_GERAN_ERROR_1_G( "gl1_hw_trm_init_DivRx: INVALID DEVICE detected for band %d",
                                 gl1_trm_get_info_input_info_p->band_mapping_info.band[i] );
             rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band = 0;
           }
           else
           {
             rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band = 
                gl1_trm_get_info_input_info_p->band_mapping_info.band[i];
      
             rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].device = 
                grm_info_p->grm_get_info_data.info.band_mapping_info.device[i];
      
             rxdiv_data_ptr->gl1_hw_DivRx_band_supported |=
                gl1_hw_map_sys_band_to_gsm_rxdiv_band(rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[i].band,gas_id);
           }
         }
       }

       MSG_GERAN_HIGH_1_G("RxDiv Bands Supported Mask: %d", rxdiv_data_ptr->gl1_hw_DivRx_band_supported); 
#endif       
}
/*===========================================================================

FUNCTION  grm_trm_request_api

DESCRIPTION : API to make TRM_REQ ( for known denial case) for PBR handling.


DEPENDENCIES
  ***** ONLY TO BE USED BY PCHXFER and not for other DUAL_SIM use cases ****

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
trm_grant_return_enum_type  grm_trm_request_api(grm_client_enum_t   client_id,
                                                trm_duration_t      duration,
                                                trm_reason_enum_t   reason,
                                                sys_proc_type_e_type subreason,
                                                trm_resource_enum_t resource)
{
  gas_id_t gas_id = gl1_map_client_id_to_gas_id(client_id);
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );
  trm_grant_return_enum_type  grm_request_grant = TRM_DENIAL;

  if ( !grm_info_p || ( gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED ) )
  {
    MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"GRM TRM Request with no valid grm_info for "
                                         "client_id:%d grant:%d",
                         client_id, grm_request_grant );
    return ( grm_request_grant );
  }

  GL1_TRM_LOCK();

  /* forward the request to TRM */
  grm_request_grant = grm_handle_trm_request(client_id,
                                             duration,
                                             reason,
                                             subreason,
                                             resource,
                                             GRM_DIVRX_TRM_REQ_INVALID,
                                             gas_id);

  GL1_TRM_UNLOCK();
  return grm_request_grant;
}
/*===========================================================================

FUNCTION  grm_inform_grant_event

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void grm_inform_grant_event(grm_client_enum_t   client_id,
                            trm_resource_enum_t resource,
                            trm_duration_t      duration,
                            trm_reason_enum_t   reason,
                            trm_grant_event_enum_t          grant_event)
{
  gas_id_t gas_id = gl1_map_client_id_to_gas_id( client_id );
  grm_info_t *grm_info_p = NULL;

  trm_inform_grant_event_input_info  *req_info;
  trm_inform_grant_event_input_info  gl1_trm_grant_info;// = { 0 };

  trm_inform_grant_event_return_data *ret_data;
  trm_inform_grant_event_return_data gl1_event_return_data;// = { 0 };

  gas_id = check_gas_id(gas_id);
  grm_info_p = grm_get_info_ptr_from_client( client_id , gas_id);

  GL1_TRM_LOCK();

  req_info = &gl1_trm_grant_info;
  ret_data = &gl1_event_return_data;

  req_info->client_id = grm_map_grm_client_id_to_trm_client_id(client_id);
  req_info->resource  = resource;
  req_info->duration  = duration;
  req_info->reason    = reason;
  req_info->wakeup_identifier = 0;

  if ( grm_info_p )
  {
    req_info->freq_info = grm_info_p->grm_freq_info;
    /* Unique identifier which allows TRM to differentiate between redundant
       and non-redundant bookings  */
    req_info->wakeup_identifier = grm_info_p->start_frame;
  }

  req_info->tag       = (trm_tag_type)GRM_INFORM_GRANT_EVENT_INFO;

  req_info->grant_event = grant_event;

  MSG_GERAN_HIGH_5_G( "grm_inform_grant_1  client:%d, resource:%d, dur:%d, reason:%d, wkup:%d ",
                       req_info->client_id,
                       req_info->resource,
                       req_info->duration,
                       req_info->reason,
                       req_info->wakeup_identifier);

  MSG_GERAN_HIGH_2_G( "grm_inform_grant_2  tag:%d, grant_event:%d",
                       req_info->tag,
                       req_info->grant_event);

  trm_inform_grant_event(req_info, ret_data);

  MSG_GERAN_HIGH_2_G("grm_inform_grant_return grant=%d, tag=%d",
                       ret_data->grant, ret_data->tag);
  GL1_TRM_UNLOCK();
  return;
}

/*===========================================================================

FUNCTION  grm_get_acquisition_priority

DESCRIPTION
  Determines whether a GRM client should override TRM_ACQUISITION

DEPENDENCIES
  None

RETURN VALUE
  GERAN_PRIORITY_NOMINAL = 0,
  GERAN_PRIORITY_INVERTED,
  GERAN_PRIORITY_NOT_CHANGED

SIDE EFFECTS
  None

===========================================================================*/
geran_priority_t grm_get_acquisition_priority(gas_id_t gas_id)
{
  geran_priority_t priority;

  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    priority = grm_acq_priority[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)];
  }
  else
  {
    priority = GERAN_PRIORITY_NOMINAL;
  }

  return(priority);
}

/*===========================================================================

FUNCTION  grm_set_acquisition_priority

DESCRIPTION
  Stores the next TRM reason priority the client should use for an
  acquisition activity

DEPENDENCIES
  None

RETURN VALUE
  VOID

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_acquisition_priority(geran_priority_t acq_priority,
                                gas_id_t          gas_id )
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    if ((GERAN_PRIORITY_NOMINAL != acq_priority) &&
        (GERAN_PRIORITY_INVERTED     != acq_priority))
    {
      /* revert to nominal/lowest priority */
      grm_acq_priority[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] = GERAN_PRIORITY_NOMINAL;
    }
    else
    {
      grm_acq_priority[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] = acq_priority;
    }
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_acquisition_reason

DESCRIPTION
  returns the next new TRM reason code should use for an acquisition activity

DEPENDENCIES
  None

RETURN VALUE
  trm_reason_enum_t new acquisition reason if it has been inverted

SIDE EFFECTS
  None

===========================================================================*/
trm_reason_enum_t grm_get_acquisition_reason(trm_reason_enum_t curr_acq_reason, gas_id_t gas_id)
{
   geran_priority_t  priority = grm_get_acquisition_priority( gas_id);
   trm_reason_enum_t next_acq_reason;
   if ( priority == GERAN_PRIORITY_INVERTED )
   {
    switch ( curr_acq_reason )
    {
       case TRM_ACQUISITION_CELL_SELECT:
       case TRM_ACQUISITION_CELL_SELECT_INV:
         next_acq_reason = TRM_ACQUISITION_CELL_SELECT_INV;
         break;
       case TRM_ACQUISITION:
       case TRM_ACQUISITION_INV:
         next_acq_reason = TRM_ACQUISITION_INV;
         break;
       default:
         MSG_GERAN_ERROR_1_G("curr_acq_reason %d unknown",curr_acq_reason);
         next_acq_reason = TRM_ACQUISITION;
         break;
    }
  }else
  {
    next_acq_reason = curr_acq_reason;
  }
  return next_acq_reason;
}

/*===========================================================================

FUNCTION  grm_set_connection_priority

DESCRIPTION
  Stores the next TRM reason code the client should use for a connected activity

DEPENDENCIES
  None

RETURN VALUE
  VOID

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_connection_priority(const grm_conn_type_t conn_type,
                                 trm_reason_enum_t     conn_reason,
                                 gas_id_t              gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    if (GRM_CS_CONNECTION == conn_type)
    {
      grm_ded_reason[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] = conn_reason;
    }
    else
    {
      grm_ptm_reason[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] = conn_reason;
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_get_connection_priority

DESCRIPTION
  Retrieves the TRM reason to be used for CS or PS connections

DEPENDENCIES
  None

RETURN VALUE
  TRM reason enum

SIDE EFFECTS
  None

===========================================================================*/
trm_reason_enum_t grm_get_connection_priority(const grm_conn_type_t conn_type,
                                              gas_id_t              gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    if (GRM_CS_CONNECTION == conn_type)
    {
      return(grm_ded_reason[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)]);
    }
    else
    {
      return(grm_ptm_reason[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)]);
    }
  }

  MSG_GERAN_ERROR_1("Invalid GAS ID 0x%02X, use default connection priority", (int)gas_id);
  return((GRM_PS_CONNECTION == conn_type) ? GRM_PS_DEFAULT : GRM_CS_DEFAULT);
}

/*===========================================================================

FUNCTION  grm_get_rf_device

DESCRIPTION
  Get the requested RF Device TRM info for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type grm_get_rf_device(grm_client_enum_t  client_id,
                                       gas_id_t           gas_id )
{
  rfm_device_enum_type grm_rf_device = RFM_INVALID_DEVICE;

  /*Mutex lock grm api/grm_info structure as both ISR and L1 Task can call this function
    at the same time*/
  gl1_grm_info_mutex_lock(gas_id);

  if ( grm_get_info(client_id, NULL, GRM_GET_CHAIN_HOLDER, gas_id) )
  {
    grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

    if ( grm_info_p )
    {
      (void)grm_get_granted( client_id, &grm_info_p->grm_get_info_data, gas_id );

      grm_rf_device = grm_info_p->grm_get_info_data.info.client_info.dev;

      if ( grm_rf_device == RFM_MAX_DEVICES )
      {
         MSG_GERAN_ERROR_0_G( GL1_DRDS_DBG_HDR"grm_get_info returned RFM_MAX_DEVICES" );

         grm_rf_device = RFM_INVALID_DEVICE;
      }

      if ( gl1_drds_debug( gas_id ) )
      {
        MSG_GERAN_LOW_2_G( GL1_DRDS_DBG_HDR"grm_get_rf_device:%d for client_id:%d",
                            grm_rf_device, client_id );
      }
    }
  }

  gl1_grm_info_mutex_unlock(gas_id);

  return ( grm_rf_device );
}

/*===========================================================================

FUNCTION  grm_get_device_mapping

DESCRIPTION
  Get RF device info for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type grm_get_device_mapping(grm_client_enum_t  requested_client_id,
                                            trm_get_info_data *input_data_p,
                                            gas_id_t           gas_id )
{
  grm_info_t *grm_info_p  = grm_get_info_ptr_from_client(requested_client_id, gas_id);
  rfm_device_enum_type  return_device = RFM_INVALID_DEVICE;

  if ( !input_data_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_get_device_mapping no valid input_data_p "
                                         "requested_client_id:%d",
                         requested_client_id );

    return ( return_device );
  }

  /* Input information for the API */
  if ( grm_info_p )
  {
    if ( grm_get_info( requested_client_id, input_data_p, GRM_GET_DEVICE_MAPPING, gas_id ) )
    {
      return_device = grm_info_p->grm_get_info_data.info.band_mapping_info.device[0];

      if ( return_device == RFM_MAX_DEVICES )
      {
        return_device = RFM_INVALID_DEVICE;
      }

      if ( gl1_drds_debug( gas_id ) )
      {
        MSG_GERAN_LOW_3_G( GL1_DRDS_DBG_HDR"grm_get_device_mapping BAND[0]:%d device:%d bands req:%d",
                            input_data_p->band_mapping_info.band[0],
                            return_device,
                            input_data_p->band_mapping_info.num_bands );
      }
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_get_device_mapping Invalid client ID:%d",
                         requested_client_id );
  }

  return ( return_device );
}

/*===========================================================================

FUNCTION  grm_get_rat_access_info

DESCRIPTION
  Get bitmask of RAT that are in connected mode. 

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
uint32 grm_get_rat_access_info( gas_id_t gas_id )
{
  uint32 rat_bit_mask = 0;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  grm_info_t *grm_info_p  = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id, gas_id);

  if ( grm_info_p )
  {
  /* Input information for the API */
  if ( grm_get_info( l1_tskisr_blk->client_id, NULL, GRM_GET_RAT_ACCESS_INFO, gas_id ) )
  {
    rat_bit_mask = grm_info_p->grm_get_info_data.info.rat_access.rat_bit_mask;

    MSG_GERAN_HIGH_1_G( GL1_DRDS_DBG_HDR"grm_get_rat_access_info rat_bit_mask:0x%x", rat_bit_mask);
  }
  }
  else
  {
    MSG_GERAN_ERROR_0_G("dereferencing NULL ptr ");
  }

  return ( rat_bit_mask );
}

/*===========================================================================

FUNCTION  grm_is_alt_sub_in_data

DESCRIPTION
  Check if alternative sub is in data sub. This function calls grm_get_rat_access_info()
  to get bimask from TRM and simply return FALSE or TRUE to caller. 
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_is_alt_sub_in_data( gas_id_t gas_id )
{
  boolean is_alt_sub_in_data = FALSE;
  uint32 rat_bit_mask = 0;

  trm_rat_group_type trm_rat_group = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? TRM_RAT_GROUP_GERAN_SUB1 : TRM_RAT_GROUP_GERAN_SUB2);

  rat_bit_mask = grm_get_rat_access_info(gas_id);

  if((rat_bit_mask != 0) &&
     ((rat_bit_mask ^ (1 << trm_rat_group)) != 0))
  {
    is_alt_sub_in_data = TRUE;
  }

  MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"grm_is_alt_sub_in_data: rat_bit_mask: %d, alt_sub_in_data: %d",
                     rat_bit_mask,
                     is_alt_sub_in_data);

  return ( is_alt_sub_in_data );
}

/*===========================================================================

FUNCTION  grm_get_granted

DESCRIPTION
  Get the requested TRM granted info for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Grant status for client

SIDE EFFECTS
  None

===========================================================================*/
trm_grant_return_enum_type grm_get_granted(grm_client_enum_t         client_id,
                                           trm_get_info_return_type *output_data_p,
                                           gas_id_t                  gas_id )
{
  trm_get_info_input_type    *input_data_p;

  /* Always clear to avoid spurious data */
  trm_get_info_input_type     input_data = { 0 };
  trm_grant_return_enum_type  result     = TRM_DENIAL;
  boolean is_divrx_client = grm_is_a_secondary_client(client_id);
#ifdef FEATURE_GSM_RX_TX_SPLIT
  boolean is_tx_client = grm_is_a_tx_client(client_id);
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  grm_client_enum_t grm_client_id;

  if ( !output_data_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_get_granted no valid output_data_p client_id:%d",
                         client_id );

    return ( result );
  }


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if (  (l1_idle_data_store[gas_id].g_bplmn_running_state == BPLMN_ACTIVE ) && gl1_is_client_for_bplmn(client_id))
  {
     grm_client_id = client_id;
  }
  else
#endif /* defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )*/
  {
    if (is_divrx_client
#ifdef FEATURE_GSM_RX_TX_SPLIT
        || is_tx_client
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
        )
    {
      grm_client_id = client_id;
    }
    else
    {
      grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
    }
  }
  gl1_grm_info_mutex_lock(gas_id);

  input_data_p = &input_data;

  input_data_p->client_id                                  =
      grm_map_grm_client_id_to_trm_client_id(grm_client_id);
  input_data_p->info_type                                  =
    TRM_CLIENT_INFO;
  input_data_p->input_data.client_info.requested_client_id =
    grm_map_grm_client_id_to_trm_client_id(client_id);

  /* Always clear so data is refreshed with latest from TRM */
  memset( output_data_p, 0, sizeof( trm_get_info_return_type ) );

  trm_get_info( input_data_p, output_data_p );

  if ( output_data_p->info.client_info.dev <= grm_get_max_valid_rf_device_for_client( client_id ) )
  {
    result = TRM_GRANTED;

    /* Only update grm info data for GSM clients */
    if ( ( client_id == GRM_GSM1 ) || ( client_id == GRM_GSM2 )
#if defined ( FEATURE_DUAL_SIM )
         || gl1_is_client_for_bplmn(client_id) || ( client_id == GRM_GPRS1 ) || ( client_id == GRM_GPRS2 )
#endif /* FEATURE_TRIPLE_SIM */
#if defined ( FEATURE_TRIPLE_SIM )
         || ( client_id == GRM_GSM3 )
#endif /* FEATURE_TRIPLE_SIM */
       )
    {
      if ( gl1_map_client_id_to_gas_id( client_id ) != gas_id )
      {
        /* Do not update grm info if client_id does not matched with gas_id */
        MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_get_granted client_id:%d is not matched with gas_id",
                             client_id );
      }
      else
      {
        grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id );

        if ( grm_info_p )
        {
          grm_info_p->grm_get_info_data = *output_data_p;
        }
        else
        {
          MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_get_granted invalid grm_info_p for requested client_id:%d",
                               client_id );
        }

        /*update HW device ID */
        gl1_hw_set_gsm_rf_id(output_data_p->info.client_info.dev, client_id, FALSE, gas_id);
      }
    }
  }
  else
  {
    output_data_p->info.client_info.dev = RFM_INVALID_DEVICE;
  }

    MSG_GERAN_DEBUG_OPT_3_G( GL1_DRDS_DBG_HDR"grm_get_granted requested_client_id:%d grant:%d device:%d",
                        client_id, result, output_data_p->info.client_info.dev );

  gl1_grm_info_mutex_unlock(gas_id);
  return ( result );
}

/*===========================================================================

FUNCTION  grm_get_info

DESCRIPTION
  Get the requested TRM info for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
static boolean grm_get_info(grm_client_enum_t  requested_client_id,
                            trm_get_info_data *grm_get_info_data_p,
                            grm_tag_t          grm_tag,
                            gas_id_t           gas_id )
{
  boolean     valid_grm_tag        = TRUE;
  boolean     grm_get_info_success = FALSE;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(requested_client_id, gas_id);
  gas_id_t    requested_gas_id; 

  /*get gas_id based on requested client id and use the same for mutex lock*/
  requested_gas_id = gl1_map_client_id_to_gas_id(requested_client_id);
  if (requested_gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED)
  {
    requested_gas_id = gas_id;
  }

  /* Client ID refers to who is making the request.
   * Requested Client ID is the SUB for which you want to make request for
   */

  gl1_grm_info_mutex_lock(requested_gas_id);

  if ( grm_info_p )
  {
    trm_get_info_input_type *grm_get_info_input_data_p;

    /* Always clear to avoid spurious data */
    trm_get_info_input_type  grm_get_info_input_data = { 0 };

    grm_get_info_input_data_p                                             =
      &grm_get_info_input_data;

    /* If valid input data exists then copy in before filling in other data fields */
    if ( grm_get_info_data_p )
    {
      grm_get_info_input_data_p->input_data                               =
        *grm_get_info_data_p;
    }

    /* Doing this setting of requested_client_id
     * screws up RXD cal + Idle RXD
     * as we corrupt the device mapping info
     * so don't allow it if we are performing a device mapping
     */
    if ( (grm_tag != GRM_GET_DEVICE_MAPPING) && (grm_tag != GRM_GET_DEVICE_SUPPORT_INFO) )
    {
      grm_get_info_input_data_p->input_data.client_info.requested_client_id =
        grm_map_grm_client_id_to_trm_client_id(requested_client_id);
    }

    grm_get_info_input_data_p->client_id                                  =
      grm_map_grm_client_id_to_trm_client_id(requested_client_id);

    switch ( grm_tag )
    {
      case GRM_GET_REASON:
        grm_get_info_input_data_p->info_type = TRM_CLIENT_INFO;
      break;

      case GRM_GET_AS_ID_MAPPING:
        grm_get_info_input_data_p->info_type = TRM_ASID_MAPPING;
      break;

      case GRM_GET_CHAIN_HOLDER:
        grm_get_info_input_data_p->info_type = TRM_DEVICE_ALLOCATION_INFO;
      break;

      case GRM_GET_DEVICE_MAPPING:
        grm_get_info_input_data_p->info_type = TRM_DEVICE_MAPPING_INFO;
        grm_get_info_input_data_p->client_id = grm_map_grm_client_id_to_trm_client_id(requested_client_id);
      break;

      case GRM_GET_DR_CAPABILITY:
        grm_get_info_input_data_p->info_type = TRM_DR_CAPABILITY;
      break;
      case GRM_GET_DEVICE_CONFIG:
        grm_get_info_input_data_p->info_type = TRM_SIMUL_CAPABILITY;
      break;

      case GRM_GET_DEVICE_SUPPORT_INFO:
        grm_get_info_input_data_p->info_type = TRM_DEVICE_SUPPORT_INFO;
      break;

      case GRM_GET_RAT_ACCESS_INFO:
        grm_get_info_input_data_p->info_type = TRM_RAT_ACCESS_INFO;
        grm_get_info_input_data_p->input_data.rat_access.client_id = 
          grm_get_info_input_data_p->input_data.client_info.requested_client_id;
      break;

      default:
        valid_grm_tag = FALSE;

        MSG_GERAN_ERROR_3_G( GL1_DRDS_DBG_HDR"grm_get_info client_id:%d request for "
                                             "requested_client_id:%d unsupported grm_tag:%d",
                             grm_get_info_input_data_p->client_id, requested_client_id, grm_tag );
      break;
    }

    if ( valid_grm_tag )
    {
      /* Issue the Sync call request to TRM */
      trm_get_info( grm_get_info_input_data_p, &grm_info_p->grm_get_info_data );

      if ( grm_info_p->grm_get_info_data.info_type != grm_get_info_input_data_p->info_type )
      {
        MSG_GERAN_ERROR_4_G( GL1_DRDS_DBG_HDR"grm_get_info client_id:%d grm_tag:%d data type "
                                             "mismatch for grm_info_type in:%d out:%d",
                             grm_get_info_input_data_p->client_id, grm_tag,
                             grm_get_info_input_data_p->info_type,
                             grm_info_p->grm_get_info_data.info_type );
      }
      else
      {
          MSG_GERAN_DEBUG_OPT_4_G( GL1_DRDS_DBG_HDR"grm_get_info client_id:%d requested_client_id:%d "
                                              "grm_tag:%d grm_info_type:%d",
                              grm_get_info_input_data_p->client_id, requested_client_id, grm_tag,
                              grm_get_info_input_data_p->info_type );

        grm_get_info_success = TRUE;
      }
    }
  }
  gl1_grm_info_mutex_unlock(requested_gas_id);

  return ( grm_get_info_success );
}

/*===========================================================================

FUNCTION  grm_modify_chain_state

DESCRIPTION
  Set the chain state info for a particular DR-DSDS client. Copies the active
  chain state info from the pre-loaded grm_info store.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_modify_chain_state(grm_client_enum_t client_id,
                            grm_tag_t         grm_tag,
                            gas_id_t          gas_id )
{
  boolean                   valid_grm_tag = TRUE;
  grm_info_t                *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_modify_data           *grm_modify_chain_data_p = NULL;
  uint8                     num_requests = 1, i = 0;
  grm_client_enum_t         tx_client_id = client_id;
  gl1_trm_state_t           trm_status = GL1_TRM_NULL;
  grm_client_enum_t         modify_client_id = client_id;
  trm_modify_duration_info  tx_modify_duration = {0};
  trm_modify_reason_info    tx_modify_reason = {0};
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk   = &l1_tsk_buffer[gas_id];

#ifdef FEATURE_GSM_RX_TX_SPLIT
  tx_client_id = grm_map_rx_to_tx_client_id(client_id, gas_id);

  if (GRM_NO_CLIENT != tx_client_id)
  {
  trm_status = grm_get_trm_status(tx_client_id, gas_id);
  }

  /*In MSim Find Bcch  or idle state , Dont modify Tx Chain stat as not in use
    as Resource Lock might have been active and we will be releasing Tx if not done already*/
  if ((GRM_NO_CLIENT != tx_client_id)
      && (TRUE == gl1_msg_get_multi_sim_mode())
      && (l1_tskisr_blk->l1_state != L1_MULTI_SIM_FIND_BCCH)
	  && (l1_tskisr_blk->l1_state != L1_MULTI_SIM_IDLE)
      && ((GL1_TRM_GRANTED == trm_status) || (GL1_TRM_RETAINED_FOR_ACCESS == trm_status)))
  {
    /*Grouped modify is not supported by TRM*/
    //grm_grouped_modify_chain_state(client_id, grm_tag, gas_id);
    /*Rx and TX*/
    num_requests = 2;
   /*Partial grant is not supported for the time being*/
    grm_set_rx_tx_status(GRM_COMPLETE_GRANT, client_id, gas_id);
  }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  if ( grm_info_p )
  {
      for (i = 0; i < num_requests; i++)
      {
        trm_modify_chain_state_input_info *grm_modify_chain_state_input_info_p;

        /* Always clear to avoid spurious data */
        trm_modify_chain_state_input_info  grm_modify_chain_state_input_info = { 0 };

        grm_modify_chain_state_input_info_p = &grm_modify_chain_state_input_info;

        if (i == 1)
        {
          modify_client_id = tx_client_id;
        }

        grm_modify_chain_data_p = grm_get_modify_chain_state_data(modify_client_id, gas_id);

        if (grm_modify_chain_data_p)
        {
          switch ( grm_tag )
          {
            case GRM_MODIFY_BAND:
            {
              grm_modify_chain_state_input_info_p->modify_type = TRM_MODIFY_BAND;
            }
            break;

            case GRM_MODIFY_DURATION:
            {
              grm_modify_chain_state_input_info_p->modify_type = TRM_MODIFY_DURATION;
              if (0 == i)
              {
                /*Save RX releated params*/
                tx_modify_duration = grm_modify_chain_data_p->modify_duration;
              }
              else
              {
                /* Assign TX from RX*/
                grm_modify_chain_data_p->modify_duration = tx_modify_duration;
                if ( gl1_drds_debug( gas_id ) )
                {
                  MSG_GERAN_DEBUG_OPT_1_G( GL1_DRDS_DBG_HDR"TX Modify duration %d",
                                     grm_modify_chain_data_p->modify_duration.duration);
                }
              }
            }
            break;

            case GRM_MODIFY_REASON:
            {
              grm_modify_chain_state_input_info_p->modify_type = TRM_MODIFY_REASON;
              if (0 == i)
              {
                /*Save RX releated params*/
                tx_modify_reason = grm_modify_chain_data_p->modify_reason;
              }
              else
              {
                /* Assign TX from RX*/
                grm_modify_chain_data_p->modify_reason = tx_modify_reason;
                if ( gl1_drds_debug( gas_id ) )
                {
                  MSG_GERAN_MED_1_G( GL1_DRDS_DBG_HDR"TX Modify Reason %d",
                                     grm_modify_chain_data_p->modify_reason.new_reason);
                }
              }
            }
            break;

            default:
            {
              valid_grm_tag = FALSE;

              MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"grm_modify_chain_state for client_id:%d "
                                                   "request for unsupported grm_tag:%d",
                                   modify_client_id, grm_tag );
            }
            break;
          }

          if ( valid_grm_tag )
          {
            /* Copy data from grm_info data store into input data */

            grm_modify_chain_state_input_info_p->client_id =
              grm_map_grm_client_id_to_trm_client_id(modify_client_id);

            grm_modify_chain_state_input_info_p->modify_info = *grm_modify_chain_data_p;
            grm_modify_chain_state_input_info_p->tag         = (trm_tag_type)grm_tag;

            /* Synchronous callback with Granted/Denied or Pending */
            trm_modify_chain_state( grm_modify_chain_state_input_info_p );
          }
        }
        else
        {
          MSG_GERAN_ERROR_1_G("Null grm_modify_chain_data_p! Invalid client_id %d",
                              modify_client_id );
        }
      }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}

/*===========================================================================

FUNCTION  grm_retain_chain_lock

DESCRIPTION
  Retain lock for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_retain_chain_lock(grm_client_enum_t     client_id,
                           grm_unlock_callback_t unlock_callback,
                           gas_id_t              gas_id )
{
  trm_retain_chain_lock_input_info *retain_lock_input_info_p;

  /* Always clear to avoid spurious data */
  trm_retain_chain_lock_input_info  retain_lock_input_info = { 0 };

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  retain_lock_input_info_p             = &retain_lock_input_info;

  retain_lock_input_info_p->client     = grm_map_grm_client_id_to_trm_client_id(client_id);
  retain_lock_input_info_p->tag        = (trm_tag_type)GRM_RETAIN_LOCK;
  retain_lock_input_info_p->unlock_tag = retain_lock_input_info_p->tag;

  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_3_G( GL1_DRDS_DBG_HDR"grm_retain_lock for client:%d "
                                        "unlock_callback address:0x%x unlock_tag:%d",
                        client_id,
                        unlock_callback,
                        retain_lock_input_info_p->unlock_tag );
  }

  if ( grm_info_p )
  {
    grm_info_p->grm_unlock_callback = unlock_callback;

    trm_retain_chain_lock( retain_lock_input_info_p );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_retain_lock

DESCRIPTION
  Retain lock for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_retain_lock(grm_client_enum_t     client_id,
                     grm_unlock_callback_t unlock_callback,
                     gas_id_t              gas_id )
{
#ifdef FEATURE_GSM_RX_TX_SPLIT
  grm_client_enum_t tx_client_id = grm_map_rx_to_tx_client_id(client_id, gas_id);
  gl1_trm_state_t  tx_trm_status = grm_get_trm_status(tx_client_id, gas_id);
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  grm_client_enum_t divrx_client;
  grm_grouped_request_bitmap_t grp_bitmap = {0};

  if(!grm_is_a_secondary_client(client_id))
  {
      divrx_client = grm_map_prx_to_divrx_client_id(client_id, gas_id);
  }
  else
  {
      divrx_client = client_id;
  }



  grp_bitmap.prxRequest = TRUE;

#ifdef FEATURE_GSM_RX_DIVERSITY
  if(GL1_TRM_GRANTED == grm_get_trm_status(divrx_client, gas_id))
  {
    grp_bitmap.drxRequest = TRUE;
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_GSM_RX_TX_SPLIT
  if ((GL1_TRM_GRANTED == tx_trm_status) ||
      (GL1_TRM_RETAINED_FOR_ACCESS == tx_trm_status))
  {
    grp_bitmap.txRequest = TRUE;

    if (FALSE == gl1_msg_get_multi_sim_mode())
    {
      /*Tx Chain allocated, Retain lock for Traffic (Single SIM) else release chain */
      if (grm_get_connection_priority(GRM_CS_CONNECTION,gas_id) == grm_gl1_get_current_trm_reason(client_id, gas_id))
      {
        grm_retain_chain_lock(client_id, unlock_callback, gas_id);

        grm_retain_chain_lock(tx_client_id,
                              (grm_unlock_callback_t)&grm_cs_traffic_abort_callback,
                              gas_id );
#ifdef FEATURE_GSM_RX_DIVERSITY
        if(GL1_TRM_GRANTED == grm_get_trm_status(divrx_client, gas_id))
        {
          grm_retain_chain_lock(divrx_client, unlock_callback, gas_id);
        }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
      }

    }
    else
    {
      /*Grouped Request*/
      grm_grouped_retain_chain_lock(client_id, unlock_callback, grp_bitmap, gas_id);
    }
  }
  else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  {
    if (grp_bitmap.drxRequest)
    {
      /*Grouped Request*/
      grm_grouped_retain_chain_lock(client_id, unlock_callback, grp_bitmap, gas_id);
    }
    else
    {
    grm_retain_chain_lock(client_id, unlock_callback, gas_id);
  }
  }
  return;
}
/*===========================================================================

FUNCTION  grm_exchange_lock

DESCRIPTION
  Exchanges the trm lock between source and destination clients

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None
===========================================================================*/
void grm_exchange_lock(grm_client_enum_t dest_client_id,
                       grm_client_enum_t source_client_id,
                       gas_id_t          gas_id )
{
  if(dest_client_id != source_client_id )
  {
    trm_client_exchange_input_info info;

    info.client_1 = grm_map_grm_client_id_to_trm_client_id(dest_client_id);
    info.client_2 = grm_map_grm_client_id_to_trm_client_id(source_client_id);

    trm_client_exchange(&info);
  }
  MSG_GERAN_HIGH_2_G( "grm_exchange_lock - Source Client:%d Dest:%d",
                       source_client_id, dest_client_id );
}


/*===========================================================================

FUNCTION  grm_clear_freq_info

DESCRIPTION
  Resets the GRM band info within the structure provided

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_clear_freq_info(grm_client_enum_t client_id,
                         gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = NULL;

  grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id );

  if ( grm_info_p )
  {
    gl1_grm_info_mutex_lock( gas_id );

    /* nuke the frequency info store */
    memset( &grm_info_p->grm_freq_info, 0, sizeof( grm_info_p->grm_freq_info ) );

    grm_info_p->grm_freq_info.num_bands = 0;

    /*following only used for diversity clients for syncing freq info in trm requests for prx and drx*/
    grm_info_p->prx_client = GRM_NO_CLIENT;

    gl1_grm_info_mutex_unlock( gas_id );
  }
  return;
}

/*===========================================================================

FUNCTION  grm_get_max_valid_rf_device_for_client

DESCRIPTION
  Get the max valid RF device for a particular DR-DSDS client.
  Can be used to distinguish Primary/Secondary cleints etc.

DEPENDENCIES
  None

RETURN VALUE
  Max valid RF device id for the selected client

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type grm_get_max_valid_rf_device_for_client(
                       grm_client_enum_t client_id )
{
  rfm_device_enum_type max_valid_rf_device_for_client =
                         MAX_VALID_GERAN_RFM_DEVICE;

  switch ( client_id )
  {
    case GRM_GSM1:
    case GRM_GSM2:
#ifdef FEATURE_DUAL_SIM
    case GRM_GPRS1:
    case GRM_GPRS2:
#endif /*FEATURE_DUAL_SIM*/

#ifdef FEATURE_GSM_RX_TX_SPLIT
    case GRM_GSM1_TX:
    case GRM_GSM2_TX:
#ifdef FEATURE_DUAL_SIM
    case GRM_GPRS1_TX:
    case GRM_GPRS2_TX:
#endif /*FEATURE_DUAL_SIM*/
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

#if defined ( FEATURE_TRIPLE_SIM )
    case GRM_GSM3:
#endif /* FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
    case GRM_GSM1_SECONDARY:
    case GRM_GSM2_SECONDARY:
#if defined ( FEATURE_TRIPLE_SIM )
    case GRM_GSM3_SECONDARY:
#endif      
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
      max_valid_rf_device_for_client = MAX_VALID_GERAN_RFM_DEVICE;
    break;

    case GRM_UMTS:
    case GRM_UMTS2:
    case GRM_UMTS_SECONDARY:
    case GRM_UMTS_SECONDARY2:
      max_valid_rf_device_for_client = MAX_VALID_WCDMA_RFM_DEVICE;
    break;

    case GRM_LTE:
    case GRM_LTE_SECONDARY:
    case  GRM_LTE_SUB2 :
    case  GRM_LTE_SUB2_SECONDARY:
      max_valid_rf_device_for_client = MAX_VALID_LTE_RFM_DEVICE;
    break;

    /* May need to add TDSCDMA etc */
    case GRM_TDSCDMA:
    default:
        MSG_GERAN_ERROR_2( GL1_DRDS_DBG_HDR"grm_get_max_valid_rf_device_for_client "
                                           "client_id:%d No valid MAX RF device use:%d",
                           client_id, max_valid_rf_device_for_client );
    break;
  }

  return ( max_valid_rf_device_for_client );
}

/*===========================================================================

FUNCTION  grm_is_a_secondary_client

DESCRIPTION
  Return true if client is a secondary.
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_is_a_secondary_client (grm_client_enum_t client_id)
{
  boolean ret_value = FALSE;

#ifdef FEATURE_GSM_RX_DIVERSITY
  if (client_id == GRM_GSM1_SECONDARY  
#ifdef FEATURE_DUAL_SIM  
      || client_id == GRM_GSM2_SECONDARY 
#ifdef FEATURE_TRIPLE_SIM
      || client_id == GRM_GSM3_SECONDARY
#endif
#endif
      )
  {
    ret_value = TRUE;
  }
#endif /* FEATURE_GSM_RX_DIVERSITY */

  return ret_value;
}
/*===========================================================================

FUNCTION grm_init_valid_gsm_sys_bands

DESCRIPTION
  Initialise grm_valid_gsm_sys_bands array for all the gsm sys bands

PARAMS
  gas_id

DEPENDENCIES

RETURN VALUE
None

===========================================================================*/
void grm_init_valid_gsm_sys_bands(
       const gas_id_t gas_id )
{
  uint16 valid_init_band_cnt = 0;
  uint16 valid_init_band_cnt_divrx = 0;

  grm_valid_gsm_sys_bands[gas_id][valid_init_band_cnt++] = SYS_BAND_GSM_EGSM_900;
  grm_valid_gsm_sys_bands[gas_id][valid_init_band_cnt++] = SYS_BAND_GSM_DCS_1800;
  grm_valid_gsm_sys_bands[gas_id][valid_init_band_cnt++] = SYS_BAND_GSM_PCS_1900;
  grm_valid_gsm_sys_bands[gas_id][valid_init_band_cnt++] = SYS_BAND_GSM_850;

  grm_valid_gsm_sys_bands_cnt[gas_id] = valid_init_band_cnt;

  grm_valid_gsm_sys_bands_divrx[gas_id][valid_init_band_cnt_divrx++] = SYS_BAND_GSM_EGSM_900;
  grm_valid_gsm_sys_bands_divrx[gas_id][valid_init_band_cnt_divrx++] = SYS_BAND_GSM_DCS_1800;
  grm_valid_gsm_sys_bands_divrx[gas_id][valid_init_band_cnt_divrx++] = SYS_BAND_GSM_PCS_1900;
  grm_valid_gsm_sys_bands_divrx[gas_id][valid_init_band_cnt_divrx++] = SYS_BAND_GSM_850;

  grm_valid_gsm_sys_bands_cnt_divrx[gas_id] = valid_init_band_cnt_divrx;

  if ( valid_init_band_cnt > MAX_BAND_INFO_GRM )
  {
    MSG_GERAN_ERROR_1_G( "grm_init_valid_gsm_sys_bands needs updating valid_init_band_cnt:%d",
                         valid_init_band_cnt );
  }

  if ( valid_init_band_cnt_divrx > MAX_BAND_INFO_GRM )
  {
    MSG_GERAN_ERROR_1_G( "grm_init_valid_gsm_sys_bands_divrx needs updating valid_init_band_cnt_divrx:%d",
                         valid_init_band_cnt_divrx );
  }
}

/*===========================================================================

FUNCTION grm_is_valid_gsm_sysband

DESCRIPTION
  checks if G band is supported

PARAMS
  band
  gas_id

DEPENDENCIES

RETURN VALUE
boolean TRUE if band is supported

===========================================================================*/
static boolean grm_is_valid_gsm_sysband (const trm_band_t band, const gas_id_t gas_id )
{
 int i;

 for(i = 0; i < grm_valid_gsm_sys_bands_cnt[gas_id]; i++)
 {
    if (grm_valid_gsm_sys_bands[gas_id][i] == band)
    {
      return TRUE;
    }
 }
 return FALSE;

}
/*===========================================================================

FUNCTION  grm_initialise

DESCRIPTION
  Called to initialise a GRM client (GRM_GSM1, GRM_GSM2 or GRM_GSM3)
  Reset all counters, bitmasks and priority inversion patterns.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_initialise(grm_client_enum_t client_id, gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  if ( grm_info_p )
  {
    grm_reset_paging_info( grm_info_p, GRM_INITIALISED_MFRMS );

    grm_info_p->reason                      = TRM_NUM_REASONS;

    grm_set_req_and_notify_duration(INVALID_SCLKS, client_id, gas_id);

    grm_info_p->grm_unlock_callback         = NULL;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    (void)grm_persistent_denial_monitoring(GRM_RESET_PERSISTENT_DENIAL_MONITORING,
                                           client_id,
                                           GRM_NULL_COMMAND,
                                           gas_id);

    /* Interrogate TRM to discover if device supports DRDS */
    grm_set_device_configuration( gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

    /* Check for status at startup */
    grm_set_dr_enabled_status(client_id, gas_id );

    /* Force to be clear so any GRM calls will auto-init */
    grm_clear_freq_info( client_id, gas_id );
    grm_clear_request_data( client_id, gas_id);
    grm_clear_sc_activity_data( client_id, gas_id );

    pending_dr_state = PENDING_DR_NULL;


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /*Initializing the Band registration Buffers*/
    grm_activity_queue_init(client_id, gas_id);
    /*initialize the count of TRm denials and bad pch blocks to be considered*/
    grm_info_p->trm_den_or_bad_page_cnt_set = gl1_drdsds_get_denial_or_fail_count(gas_id);
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  }
  else
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_initialise no valid grm_info for client_id:%d",
                         client_id );
  }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  grm_register_async_callback(client_id, gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GSM_RX_TX_SPLIT
  /*Single SIM, Register TX client*/
  if ((FALSE == gl1_msg_get_multi_sim_mode())
      && ((client_id == GRM_GSM1)||(client_id == GRM_GSM2)))
  {
    grm_register_async_callback(grm_map_rx_to_tx_client_id(client_id,gas_id), gas_id);
  }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  return;
}

/*===========================================================================

FUNCTION  grm_set_dr_enabled_status

DESCRIPTION
  Sets the dr_enabled status from TRM for a particular GSM client,
  called during initialisation and every time enter idle called to ensure
  that the latest status is reflected during runtime.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_dr_enabled_status(grm_client_enum_t client_id,
                               gas_id_t    gas_id )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  grm_info_t        *grm_info_p;
  gas_id_t           other_g_gas_id;
  grm_client_enum_t  other_g_client_id;

  grm_info_t *grm_info_other_g_sub_p = NULL;

  grm_info_p     = grm_get_info_ptr_from_client(client_id, gas_id);

  /* Only valid for DR-DSDS so not in G+G hence quick return */
  if ( !grm_device_config_is_drdsds() )
  {
    if (grm_info_p)
    {
      grm_info_p->dr_enabled = FALSE;
    }
    return;
  }

  other_g_gas_id = OTHER_G_SUB(gas_id);

  if ( other_g_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED )
  {
    other_g_client_id = gl1_map_gas_id_to_client_id(other_g_gas_id);

    if(other_g_client_id != GRM_NO_CLIENT)
    {
      grm_info_other_g_sub_p = grm_get_info_ptr_from_client(other_g_client_id, other_g_gas_id);
    }
  }


  /* Only valid for G+G mode and if UE is configured as supporting DR-DSDS */
  if ( grm_info_p && grm_info_other_g_sub_p )
  {
    grm_info_p->dr_enabled =
      ( TRM_DUAL_RECEIVE_ENABLED == grm_get_dr_capability( client_id, gas_id ) );

    /* Based upon dr enabled status, set hybrid tuneaway state machine*/
    gl1_drdsds_set_hybrid_tuneaway_state( client_id, gas_id );

    /*if the sub has been in transfer mode there maybe a pending DR transition*/
    if (pending_dr_state != PENDING_DR_NULL)
    {
      if (grm_info_p->dr_enabled && (pending_dr_state == PENDING_DR_DISABLE))
      {
        (void)grm_enable_disable_dual_receive( FALSE, client_id, gas_id );
      }
      else if (!grm_info_p->dr_enabled && (pending_dr_state == PENDING_DR_ENABLE))
      {
        (void)grm_enable_disable_dual_receive( TRUE, client_id, gas_id );
      }
      else
      {
        grm_info_p->dr_enabled = pending_dr_state;
      }

      MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"Client %d Pending DR State Transition Detected:%d",
                                client_id, pending_dr_state );

      pending_dr_state = PENDING_DR_NULL;

      /*sync DR setting across both subs in G+G case*/
      if (!CHECK_ANY_OTHER_GSM_CLIENT_IS_INACTIVE(gas_id))
      {
        grm_info_other_g_sub_p->dr_enabled = grm_info_p->dr_enabled;

        /* Based upon dr enabled status, set hybrid tuneaway state machine*/
        gl1_drdsds_set_hybrid_tuneaway_state(other_g_client_id, other_g_gas_id );
      }
    }

    if ( gl1_drds_debug( gas_id ) )
    {
      MSG_GERAN_HIGH_4_G( GL1_DRDS_DBG_HDR"grm_set_dr_enabled_status: gas_id %d DR State %d gas_id %d DR state %d",
                          gas_id,
                          grm_info_p->dr_enabled ,
                          other_g_gas_id,
                          grm_info_other_g_sub_p->dr_enabled );

    }
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  return;
}

/*===========================================================================

FUNCTION  grm_set_valid_acquisition_bands

DESCRIPTION
  Sets the default list of bands dependent on the bands
  supported by the UE RF card.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_valid_acquisition_bands(grm_client_enum_t  client_id,
                                     gas_id_t           gas_id )
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id , gas_id);

  if ( grm_info_p )
  {
    uint16                i;
    trm_get_info_data    *grm_req_p;
    rfm_device_enum_type  rf_device;
    trm_get_info_data  grm_req   = {{0}};

    grm_req_p = &grm_req;

    /* Reset band request data to default */
    grm_init_valid_gsm_sys_bands( gas_id );

    /* Dummy request for all bands just to see what is supported */
    grm_req_p->band_mapping_info.resource  = TRM_RX_ANY;
    grm_req_p->band_mapping_info.num_bands = grm_valid_gsm_sys_bands_cnt[gas_id];

    /* Copy in dummy init band info */
    for ( i = 0; i < grm_req_p->band_mapping_info.num_bands; i++ )
    {
      grm_req_p->band_mapping_info.band[i] =
        grm_valid_gsm_sys_bands[gas_id][i];
    }

    /* Dummy request with all possible bands to detect what bands are supported */
    rf_device =
      grm_get_device_mapping( client_id, grm_req_p, gas_id );

    /*
     *Update bands suported based on return data from TRM
     */
    {
      sys_band_class_e_type trm_band_info;
      rfm_device_enum_type  rf_device_info;
      uint16                valid_band_cnt = 0;

      /* Clear any band information*/
      for ( i = 0; i < grm_valid_gsm_sys_bands_cnt[gas_id]; i++ )
      {
        /* Grab band and device info from TRM data */
        trm_band_info  =
          grm_req_p->band_mapping_info.band[i];
        rf_device_info =
          grm_info_p->grm_get_info_data.info.band_mapping_info.device[i];

        /* Only add bands supported by device */
        if ( ( rf_device_info != RFM_MAX_DEVICES ) &&
             ( rf_device_info != RFM_INVALID_DEVICE )
           )
        {
          grm_valid_gsm_sys_bands[gas_id][valid_band_cnt++] =
            trm_band_info;
        }

        if ( gl1_drds_debug( gas_id ) )
        {
          MSG_GERAN_LOW_2_G( GL1_DRDS_DBG_HDR"grm_set_valid_acquisition_bands: trm_band_info:%d rf_device_info:%d",
                             trm_band_info, rf_device_info );
        }
      }

      if ( gl1_drds_debug( gas_id ) )
      {
        MSG_GERAN_LOW_2_G( GL1_DRDS_DBG_HDR"grm_set_valid_acquisition_bands: client_id:%d valid_band_cnt:%d",
                           client_id, valid_band_cnt );
      }

      /* Update valid band count */
      grm_valid_gsm_sys_bands_cnt[gas_id] = valid_band_cnt;

      /* Clean up remaining entries */
      for ( ; valid_band_cnt < MAX_BAND_INFO_GRM; valid_band_cnt++ )
      {
        grm_valid_gsm_sys_bands[gas_id][valid_band_cnt] = SYS_BAND_CLASS_NONE;
      }
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_reset_paging_info

DESCRIPTION
  Resets the GRM paging info within the structure provided

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
static void grm_reset_paging_info(grm_info_t *grm_info_p,
                                  uint8       bs_pa_mfrms )
{
  if ( grm_info_p )
  {
    grm_info_p->curr_bs_pa_mfrms   = bs_pa_mfrms;

    MSG_GERAN_HIGH_1( "PBR grm_reset_paging_info bs_pa_mfrms:%d",
                      bs_pa_mfrms );
  }
  return;
}

/*===========================================================================

FUNCTION  grm_clear_request_data

DESCRIPTION
  Resets the GRM request info within the structure provided

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
static void grm_clear_request_data(grm_client_enum_t  client_id,
                                   gas_id_t           gas_id )
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  if ( grm_info_p )
  {
    gl1_grm_info_mutex_lock(gas_id);
    /* nuke the frequency info store */
    memset( &grm_info_p->grm_request_data, 0, sizeof( grm_info_p->grm_request_data ) );
    gl1_grm_info_mutex_unlock(gas_id);
  }
  return;
}

/*===========================================================================

FUNCTION  grm_clear_sc_activity_data

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
static void grm_clear_sc_activity_data(grm_client_enum_t  client_id,
                                       gas_id_t           gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  if ( grm_info_p )
  {
    gl1_grm_info_mutex_lock(gas_id);
    /* nuke the surround cell activity store */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    memset( &grm_info_p->persistent_denial.denied_band_info,
            0,
            sizeof( grm_info_p->persistent_denial.denied_band_info ) );
#endif
    memset( &grm_info_p->irat_band_registered,
            0,
            sizeof( grm_info_p->irat_band_registered ) );
    memset( &grm_info_p->current_band_info,
            0,
            sizeof( grm_info_p->current_band_info ) );


    gl1_grm_info_mutex_unlock(gas_id);
  }
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

#ifdef FEATURE_PERSISTENT_DENIAL_DEBUG
/*===========================================================================

FUNCTION  grm_denied_arfcn_processing

DESCRIPTION
  API should be called after when scheduling PM's in idle mode and when receiving
  the results . API counts the number of denied arfcns when scheduling and decrements
  the count when we receive results back.


DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void  grm_denied_arfcn_processing (ARFCN_T                            *arfcn,
                                   uint32                             count,
                                   grm_persistent_denial_sm_actions_t action,
                                   grm_client_enum_t                  client_id,
                                   gas_id_t                           gas_id)
{
  int i = 0;
  int j = 0;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if (!grm_info_p || !arfcn)
  {
     MSG_GERAN_ERROR_0_G("grm_denied_arfcn_processing: NULL pointer check fail!");
     return;
  }

  /*we only process the arfcns when persistent denial is enabled, this is to ensure we
    only process one batch of denied ARFCNS at a time*/
  if(grm_info_p->persistent_denial.persistent_denial_enabled)
  {
    switch (action)
    {
     case GRM_COUNT_DENIED_ARFCNS :
       for (i = 0; i < count ; i++)
       {
         if(arfcn[i].band != INVALID_BAND)
         {
           if (grm_info_p->persistent_denial.denied_band_info[arfcn[i].band].band_tune_result
               == TRM_BAND_TUNE_DENIED)
           {
             /*increment arfcn per band count*/
             grm_info_p->persistent_denial.denied_band_info[arfcn[i].band].arfcn_count++;

             if(gl1_drds_debug( gas_id ))
             {
               MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"grm_denied_arfcn_processing COUNT: arfcn %d from band %d",
                                  arfcn[i].num,
                                  arfcn[i].band);
             }

             j++;
           }
         }
       }
       MSG_GERAN_MED_1_G(GL1_DRDS_DBG_HDR"grm_denied_arfcn_processing: %d arfcns from unregistered bands", j);

      break;

     case GRM_PROCESS_DENIED_ARFCNS:

          if(arfcn[i].band != INVALID_BAND)
          {
            if (grm_info_p->persistent_denial.denied_band_info[arfcn[i].band].band_tune_result == TRM_BAND_TUNE_DENIED &&
                grm_info_p->persistent_denial.denied_band_info[arfcn[i].band].arfcn_count > 0)
             {
               /*decrement arfcn per band count*/
               grm_info_p->persistent_denial.denied_band_info[arfcn[i].band].arfcn_count--;

               if(gl1_drds_debug( gas_id ))
               {
                MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"grm_denied_arfcn_processing PROCESS: arfcn %d from band %d",
                                   arfcn[i].num,
                                   arfcn[i].band);
               }
               if(grm_info_p->persistent_denial.denied_band_info[arfcn[i].band].arfcn_count == 0)
               {

                MSG_GERAN_MED_1_G(GL1_DRDS_DBG_HDR"grm_denied_arfcn_processing: all arfcns measured for BAND %d",
                arfcn[i].band);

               }
             }
          }


      break;

     default:
         MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"grm_denied_arfcn_processing: Invalid action %d",action );
      break;

    }
  }

}
#endif /*FEATURE_PERSISTENT_DENIAL_DEBUG*/

/*===========================================================================

FUNCTION  grm_persistent_denial_monitoring

DESCRIPTION
  API should be called after avery trm_request for TRM_DEMOD_PAGE, detection of a denied IFS request and everytime idle mode
  is entered.
  API manages all actions assciated with persitant denial monitioring.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_persistent_denial_monitoring(grm_persistent_denial_sm_actions_t   action,
                                             grm_client_enum_t                    client_id,
                                             grm_pd_command_t                     request,
                                             gas_id_t                             gas_id)

{
  grm_info_t *grm_info_p = NULL;
  boolean return_result = FALSE;
  
  /*Persistent denial is not applicable for diversity client */
  if(TRUE == grm_is_a_secondary_client(client_id))
  {
    return return_result;
  }

  grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if(grm_info_p)
  {
    switch (action)
    {
      case GRM_START_PERSISTENT_DENIAL_MONITORING:
      {
        if(grm_info_p->persistent_denial.meas_cycle == GRM_QUICK_MEAS_CYCLE)
        {
          grm_info_p->persistent_denial.persistent_denial_enabled = TRUE;
        }
        else /*slow meas cycle and good serving cell conditions so we can live with more denials*/
        {
          if(grm_info_p->persistent_denial.request_denied_count > 0)
          {
            grm_info_p->persistent_denial.request_denied_count--;
          }

          if((grm_info_p->persistent_denial.request_denied_count == 0) &&
                 !grm_info_p->persistent_denial.persistent_denial_enabled)
          {
            grm_info_p->persistent_denial.persistent_denial_enabled= TRUE;
          }
        }
        return_result = grm_info_p->persistent_denial.persistent_denial_enabled;
      }
      break;

      case GRM_DECREMENT_DENIED_REG_COUNT:
      {
        if(grm_info_p->persistent_denial.persistent_denial_enabled
           && grm_info_p->persistent_denial.pch_ticks_to_include_denied_bands)
        {
          grm_info_p->persistent_denial.pch_ticks_to_include_denied_bands--;
        }

        if (grm_info_p->persistent_denial.pch_ticks_to_include_denied_bands == 0)
        {
          grm_info_p->persistent_denial.persistent_denial_enabled = FALSE;
          memset( &grm_info_p->persistent_denial.denied_band_info,
                  TRM_BAND_TUNE_OK,
                  sizeof( grm_info_p->persistent_denial.denied_band_info ) );
          grm_initialise_pm_denial_count(client_id, gas_id);
          grm_info_p->persistent_denial.pch_ticks_to_include_denied_bands = DEFAULT_DEMOD_PAGE_COUNT;
          grm_clear_registered_bands(client_id, gas_id);
          MSG_GERAN_MED_0_G(GL1_DRDS_DBG_HDR"GRM_DECREMENT_DENIED_REG_COUNT: RESET PD");
        }
        return_result =  (grm_info_p->persistent_denial.pch_ticks_to_include_denied_bands !=0);
      }
      break;

      case GRM_CHECK_PERSISTENT_DENIAL_FLAG:
        return_result= grm_info_p->persistent_denial.persistent_denial_enabled;
        break;

      case GRM_ADD_DENIED_BANDS_TO_FREQ_INFO:
      {
        int i;
        
        /*first make sure that the serving cell band is added to freq_info*/
        ARFCN_T serving_cell = gl1_get_serving_cell_arfcn(gas_id);

        grm_add_trm_band_to_freq_input_type(gl1_map_gsm_band_to_sys_band(serving_cell.band),
                                            client_id,
                                            gas_id);

        /*make sure serving cell band is always registered*/
        grm_info_p->persistent_denial.denied_band_info[serving_cell.band].band_registered = TRUE;

          /*next add the denied bands to the request*/
          for (i = 0; i < INVALID_BAND; i++ )
          {
            /*we should only add the band to the TRM request if the band is to be measured in next PCH block and its a valid band for the device*/
            if (grm_info_p->persistent_denial.denied_band_info[i].band_tune_result == TRM_BAND_TUNE_DENIED &&
                grm_info_p->persistent_denial.denied_band_info[i].next_pch_block &&
                grm_is_valid_gsm_sysband(gl1_map_gsm_band_to_sys_band(i), gas_id))
            {
              MSG_GERAN_MED_1_G(GL1_DRDS_DBG_HDR"grm_persistent_denial_monitoring: Add denied band %d to freq_info",i);

              grm_add_trm_band_to_freq_input_type(gl1_map_gsm_band_to_sys_band(i),
                                                  client_id,
                                                  gas_id);

              /*flag use to ensure that skipped measurements don't get added to the running average
                         registered bands are cleared when the pm results callback is received*/
              grm_info_p->persistent_denial.denied_band_info[i].band_registered = TRUE;

              /*flag is only valid for one PCH block and once the grm_request is made for demod page with all denied bands present then
                         flag can be cleared. Interband PM look ahead will be peformed again at the end of the paging block*/
              if(request == GRM_REQUEST_CLEAR)
              {
                grm_info_p->persistent_denial.denied_band_info[i].next_pch_block = FALSE;
              }
            }
           else
           {
             MSG_GERAN_HIGH_3_G(GL1_DRDS_DBG_HDR"grm_persistent_denial_monitoring: denied band %d, to be measured %d, valid sys band %d",
                grm_info_p->persistent_denial.denied_band_info[i].band_tune_result,
                grm_info_p->persistent_denial.denied_band_info[i].next_pch_block,
                grm_is_valid_gsm_sysband(gl1_map_gsm_band_to_sys_band(i), gas_id));         
           }
          }
        
          return_result = grm_info_p->persistent_denial.persistent_denial_enabled;
        }
        break;

      case GRM_RESET_PERSISTENT_DENIAL_MONITORING:
      {
        grm_info_p->persistent_denial.persistent_denial_enabled = FALSE;
        memset( &grm_info_p->persistent_denial.denied_band_info,
                TRM_BAND_TUNE_OK,
                sizeof( grm_info_p->persistent_denial.denied_band_info ) );
        grm_info_p->persistent_denial.pch_ticks_to_include_denied_bands = DEFAULT_DEMOD_PAGE_COUNT;
        return_result = FALSE;
        grm_initialise_pm_denial_count(client_id, gas_id);
        MSG_GERAN_DEBUG_OPT_0_G(GL1_DRDS_DBG_HDR"grm_persistent_denial_monitoring: RESET");
      }
      break;

      case GRM_GOOD_SERVING_CELL_CHECK:
      {
        if(grm_info_p->rssi >= GRM_PERSISTENT_DENIAL_RSSI_THRESHOLD &&
           grm_info_p->Scell_SNR >= GRM_PERSISTENT_DENIAL_SNR_THRESHOLD)
        {
          grm_info_p->persistent_denial.meas_cycle = GRM_SLOW_MEAS_CYLE;
          MSG_GERAN_MED_0_G(GL1_DRDS_DBG_HDR"grm_persistent_denial_monitoring: "
                                            "Good serving cell set slow PD meas cycle");
        }
        else
        {
          grm_info_p->persistent_denial.meas_cycle = GRM_QUICK_MEAS_CYCLE;
          MSG_GERAN_MED_0_G(GL1_DRDS_DBG_HDR"grm_persistent_denial_monitoring: "
                                            "Serving cell below RSSI+SNR thresholds set quick PD meas cycle");
        }
        return_result= grm_info_p->persistent_denial.persistent_denial_enabled;
      }
      break;

      case GRM_INIT_SERVING_CELL_LEVELS:
      {
        grm_info_p->rssi = GRM_PERSISTENT_DENIAL_RSSI_THRESHOLD;
        grm_info_p->Scell_SNR = GRM_PERSISTENT_DENIAL_SNR_THRESHOLD;
        grm_initialise_pm_denial_count(client_id, gas_id);
        grm_info_p->persistent_denial.meas_cycle = GRM_SLOW_MEAS_CYLE;
        return_result= grm_info_p->persistent_denial.persistent_denial_enabled;
      }
      break;

      default:
        MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"grm_persistent_denial_monitoring: invalid action %d",action);
        break;
      }

      if(grm_info_p->persistent_denial.persistent_denial_enabled)
      {
        MSG_GERAN_HIGH_4_G(GL1_DRDS_DBG_HDR"grm_persistent_denial_monitoring: Action: %d, Enabled %d, monitoring count %d , denials %d",action,
                grm_info_p->persistent_denial.persistent_denial_enabled,
                grm_info_p->persistent_denial.pch_ticks_to_include_denied_bands,
                grm_info_p->persistent_denial.request_denied_count);
      }
    }
    return return_result;
}

/*===========================================================================

FUNCTION  grm_reserve_at

DESCRIPTION
  Forwards a GSM client reservation to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  May intercept reservations for TRM_DEMOD_PAGE and apply priority inversion

===========================================================================*/
void grm_reserve_at(grm_client_enum_t   client_id,
                          trm_time_t            when,
                          trm_duration_t        duration,
                          trm_reason_enum_t     reason,
                          sys_proc_type_e_type  sub_reason,
                    trm_resource_enum_t resource,
                    boolean rxdiv_chain )
{
  gas_id_t                    gas_id = gl1_map_client_id_to_gas_id( client_id );

  grm_grouped_request_bitmap_t client_bitmap;

  client_bitmap.prxRequest = TRUE;
  client_bitmap.drxRequest = FALSE;
  client_bitmap.txRequest  = FALSE;

  if((grm_is_tx_reason(reason)) && (sub_reason != SYS_PROC_TYPE_PAGE))
  {
    client_bitmap.txRequest  = TRUE;    
  }

  /*Place DRx reserve_at only if allowed in MSim Idle mode 
    And Release if Reservation is not Reqd. but DRx is granted*/
  if ((rxdiv_chain) && 
       gl1_is_l1_state_msim_idle(gas_id))
  {
    if (grm_validate_diversity_chain_request( TRM_GRANTED, GRM_DIVRX_TRM_REQ_IDLE, gas_id))
    {
      client_bitmap.drxRequest = TRUE;
    }
    else if (grm_get_trm_status(grm_map_prx_to_divrx_client_id(client_id, gas_id), gas_id) == GL1_TRM_GRANTED)
    {
      grm_release_rx_sec(grm_map_prx_to_divrx_client_id(client_id, gas_id), gas_id);
    }/*if grm validate diversity chain request*/
  }/*If Rxd Allowed in mSim Idle */

  grm_grouped_trm_reserve(client_id, 
                          client_bitmap,
                       when,
                       duration,
                       reason,
                       sub_reason,
                          resource,
                            gas_id);

  return;
}

/*===========================================================================

FUNCTION  grm_reserve_at_enhanced

DESCRIPTION
  Forwards a GSM client reservation to the TRM layer. This also stores
  important information, that can be used to determine which client will win
  contention for the lock upon the next wakeup.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  May intercept reservations for TRM_DEMOD_PAGE and apply priority inversion

===========================================================================*/
void grm_reserve_at_enhanced(grm_client_enum_t   client_id,
                             trm_time_t          when,
                             trm_duration_t      duration,
                             trm_reason_enum_t   reason,
                             sys_proc_type_e_type   sub_reason,
                             uint32              required_frame,
                             uint32              frame_duration,
                             trm_resource_enum_t resource,
                             l1_fm_priority_T    pri,
                             gas_id_t            gas_id )
{
  grm_info_t              *grm_info_p;

  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  trm_reason_enum_t        rsv_reason    = reason;
  boolean                  qta_running   = FALSE;
  idle_data_T             *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
#if defined (FEATURE_QBTA)
  if(pri == FM_PRI_CCCH)
  {
     gl1_hw_fill_next_qta_gap_params( when ,gas_id);
  }
#endif

  /* CR2311819 do not request trm reservation as STOP_GSM_MODE is received and reselection is going to happen */
  if((L1_RAT_CHANGE_COMMAND == l1_tskisr_blk->command)&&
     (GERAN_EXCHANGE_GTOX == geran_get_exchange_on_release( gas_id ))&&
     (TRUE == l1_tskisr_blk->g2x_resel_in_progress))
     {
         MSG_GERAN_HIGH_0_G( "Preventing TRM reservation as reselection is ongoing");
         return;
     }

#ifdef FEATURE_QTA
  if ( gl1_hw_qta_gap_active( gas_id ) )
  {
    mdsp_sleep_reset_mdsp_intf(gas_id);

    /* reset the QTA flags just before giving up the lock */
    l1_tskisr_blk->qta_in_progress = FALSE;


    qta_running = TRUE;
  }
#endif /* FEATURE_QTA */

  MSG_GERAN_HIGH_4_G( "qta_running %d paging_mode %d paging_block_index %d, BS_CCCH_SDCCH_COMB:%d",
                      qta_running,
                      l1_idle_data_store[gas_id].paging_data.paging_mode,
                      l1_idle_data_store[gas_id].paging_data.paging_block_index,
                      l1_idle_data_store[gas_id].campedon_cell_data.BS_CCCH_SDCCH_COMB
                     );

  /* Get the correct pointer to the reservation store for this client */
  grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  if ( grm_info_p )
  {
    /* Store TRM when value for debugging */
    gl1_hw_sleep_wmgr_trm_when_cur[gas_id] = when;

    /* Store away the values for this booking */
    grm_info_p->duration          = duration;
    grm_info_p->when              = when;
    grm_info_p->start_frame       = required_frame;

    /* This is a deliberate action to prevent any difficult hyperframe wrap-arounds calculations
       in the decisions that follow */
    grm_info_p->end_frame      = ( required_frame + frame_duration );
    grm_info_p->resource       = resource;
    grm_info_p->frame_duration = frame_duration;

    MSG_GERAN_MED_3_G( "GSM booking. client_id %d dur %d @ %u",
                       client_id,
                       grm_info_p->duration,
                       grm_info_p->when );

    MSG_GERAN_MED_3_G( "GSM booking. ts %u start FN %d end FN %d",
                       timetick_get_safe(),
                       grm_info_p->start_frame,
                       grm_info_p->end_frame );

    grm_info_p->rsv_reason = rsv_reason;
    grm_info_p->rsv_sub_reason = sub_reason;
  }
  else
  {
    MSG_GERAN_ERROR_1_G( "grm_info_p returned as NULL pointer, Invalid client ID %d!",
                         client_id );
  }

  /* Call grm_release() if
   * 1. QTA is active (to tell other sub that they can start their activity)
   *  OR
   * 2. We are not in "G+G" and new activity is "ccch" but not in extended paging. 
   * So that PBR count stays up.
   */
  if( ( (!GONLY_OPERATION_IS_ACTIVE() && (((pri == FM_PRI_CCCH) && 
          (idle_data_ptr->paging_data.paging_mode != EXTENDED_PAGING))
         || (l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH && 
             l1_tskisr_blk->sub_state == L1_MULTI_SIM_RESELECTION)
         )
        )
#ifdef FEATURE_DUAL_SIM
      /* want to call reserve at with bplmn so as to be granted the correct duration*/
      && (l1_idle_data_store[gas_id].g_bplmn_running_state != BPLMN_ACTIVE)
#endif
      )
#if defined (FEATURE_QTA)
      || qta_running
#endif
      || OTHER_SUB_IN_TBF_SUSP()
      
#ifdef FEATURE_DUAL_SIM
      ||(grm_get_resource_lock_enum(gas_id) == GRM_RESOURCE_LOCK_GOING_INACTIVE)
#endif

  )
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif
    {
      grm_release( client_id, gas_id );
    }
  }
 
#ifdef FEATURE_DUAL_SIM
   /*  Resource lock will be made INACTIVE on releasing of activity*/
  if(grm_get_resource_lock_enum(gas_id) == GRM_RESOURCE_LOCK_GOING_INACTIVE)
  {
     grm_set_resource_lock_enum(GRM_RESOURCE_LOCK_INACTIVE,gas_id);
  }
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /*if activity is cleared prior in DR Mode, just return*/
  if((pri == FM_PRI_RESERVED) && gl1_drdsds_enabled(gas_id))
  {
    MSG_GERAN_HIGH_0_G( "update_list: has been received and Irat Activity has been cleared, so skip grm_reserve_at");
    return;
  }

  /* if gl1_task received and processed a new frequency list with zero neighbors recently 
     or else if search is aborted because of some other reason and srch state was set to NULL, 
     check if Irat Activities are getting sched and do not proceed with TRM reservation */
  if ((l1_sc_get_wcdma_list_num_cached_idle_freqs(gas_id) == 0) || (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL) || (l1_sc_wcdma_data_ptr->wcdma_srch.abort))
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
       l1_fm_reset_pri_res(gas_id);
       l1_fm_clear_required_frames( pri, gas_id );
       /*Fall through to return*/
      case FM_PRI_RESERVED:
       MSG_GERAN_HIGH_3_G( "skipped grm_reserve_at: Irat Activity Cleared: %d, srch state %d, abort flag %d", pri,l1_sc_wcdma_data_ptr->wcdma_srch.srch_state, l1_sc_wcdma_data_ptr->wcdma_srch.abort);
       return;

      default:
       break;
    }
  }
  /*set the static var before calling reserve_at*/
  res_at_enh_pri[gas_id] = pri;
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
  grm_reserve_at( client_id, when, duration, rsv_reason, sub_reason, resource, TRUE );

  if (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
  {
    if (0 == reserve_at_count[gas_id])
    {
      reserve_at_count[gas_id]++;
    }

    MSG_GERAN_MED_3_G("MSIM: reserve_at_cnt[%d] %d fn %d",
                       gas_id, reserve_at_count[gas_id],
                       GSTMR_GET_FN_GERAN(gas_id));
    /* gl1_hw_sleep_wmgr_cancel_in_prog[gas_id] = FALSE; */
  
  }
  return;
}

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION  grm_reserve_at_ccch_in_trans

DESCRIPTION
  Forwards a GSM client reservation to the TRM layer.
  This is only used for TRM reserve for own CCCH during transfer

DEPENDENCIES
  None

RETURN VALUE
  boolean TRUE indicates reserve was passed to TRM

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_reserve_at_ccch_in_trans(grm_client_enum_t    client_id,
                                  trm_time_t              when,
                                  trm_duration_t          duration,
                                  trm_reason_enum_t       reason,
                                  uint32                  required_frame,
                                  uint32                  frame_duration,
                                  trm_resource_enum_t     resource,
                                  gas_id_t                gas_id )
{
  trm_reserve_at_input_info *grm_reserve_at_input_info_p = NULL;
  grm_info_t                *grm_info_p = NULL;
  boolean                   non_valid_freq_info;

  trm_reserve_at_input_info  grm_reserve_at_input_info = { 0 };

  if (grm_get_resource_lock_state(gas_id))
  {
    MSG_GERAN_HIGH_0_G( GL1_DRDS_DBG_HDR"grm_reserve_at_ccch_in_trans blocked - TRM resource lock is active" );
    return FALSE;
  }

  grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  if ( grm_info_p )
  {
    /* Store away the values for this booking */
    grm_info_p->duration          = duration;
    grm_info_p->when              = when;
    grm_info_p->start_frame       = required_frame;

    /* This is a deliberate action to prevent any difficult hyperframe wrap-arounds calculations
       in the decisions that follow */
    grm_info_p->end_frame      = ( required_frame + frame_duration );
    grm_info_p->resource       = resource;
    grm_info_p->frame_duration = frame_duration;

    MSG_GERAN_MED_3_G( "GSM booking. client_id %d dur %d @ %u",
                       client_id,
                       grm_info_p->duration,
                       grm_info_p->when );

    MSG_GERAN_MED_3_G( "GSM booking. ts %d start FN %d end FN %d",
                       timetick_get_safe(),
                       grm_info_p->start_frame,
                       grm_info_p->end_frame );

    grm_info_p->rsv_reason = reason;
  }
  else
  {
    MSG_GERAN_ERROR_1_G( "grm_info_p returned as NULL pointer, Invalid client ID %d!",
                         client_id );
  }

  non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

  grm_reserve_at_input_info_p = &grm_reserve_at_input_info;

  grm_reserve_at_input_info_p->client_id   =
              grm_map_grm_client_id_to_trm_client_id(client_id);
  grm_reserve_at_input_info_p->resource    = resource;
  grm_reserve_at_input_info_p->when        = when;
  grm_reserve_at_input_info_p->duration    = duration;
  grm_reserve_at_input_info_p->reason      = reason;

  grm_reserve_at_input_info_p->sub_reason = SYS_PROC_TYPE_PAGE;

  /* Initialise to zero (to avoid uninitialised value to TRM), but it is updated below anyways. */
  grm_reserve_at_input_info_p->wakeup_identifier = 0;

  /* Tag with expected GL1 callback identifier */
  grm_reserve_at_input_info_p->tag = (trm_tag_type)GRM_RESERVE_AT;

  if ( grm_info_p )
  {
    /* Active band info for client */
    grm_reserve_at_input_info_p->freq_info = grm_info_p->grm_freq_info;

    /* Unique identifier which allows TRM to differentiate between redundant and non-redundant bookings  */
    grm_reserve_at_input_info_p->wakeup_identifier = grm_info_p->start_frame;
  }

  trm_reserve_at( grm_reserve_at_input_info_p );

  if ( non_valid_freq_info )
  {
    grm_clear_freq_info( client_id, gas_id );
  }

  return TRUE;
}
#endif /*  FEATURE_DUAL_SIM */
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

/*===========================================================================

FUNCTION  grm_get_cur_reservation

DESCRIPTION
  TRM booking has been done in grm_reserve_at_enhanced().
  Get pointer for current reservation.

DEPENDENCIES
  None

RETURN VALUE
  grm_info_t: pointer to current GRM/TRM reservation

===========================================================================*/
grm_info_t * grm_get_cur_reservation(uint32 wakeup_fn, gas_id_t gas_id )
{
  grm_client_enum_t        client_id;
  grm_info_t              *grm_info_p;
  trm_reason_enum_t        trm_reason;
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  if(gl1_get_trm_global_overide_status(gas_id))
  {
    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_SLEEP_EARLY_CAMP);
  }
#endif

  /* Update the reservation in sync with FM reservation */
  if ( !l1_fm_check_reserved_trm_activity(gas_id, wakeup_fn) )
  {
    MSG_GERAN_MED_1_G( "Update TRM booking for Activity: %d to resolve empty QTA gap",l1_get_fm_trm_priority(gas_id) );
  }

  /* Update booking with sleep warmup duration. */
  client_id  = l1_tskisr_blk->client_id;
  grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  trm_reason = grm_gl1_get_current_trm_reason(client_id, gas_id );

  if ( TRM_NUM_REASONS == trm_reason)
  {
    MSG_GERAN_ERROR_0_G("no reason booked for TRM!!!");

    return NULL;
  }

  return grm_info_p;
  }


/*===========================================================================

FUNCTION  grm_store_cur_reservation

DESCRIPTION
  TRM booking has been done in grm_reserve_at_enhanced().
  Store current reservation.

DEPENDENCIES
  None

RETURN VALUE
  void

===========================================================================*/
void grm_store_cur_reservation(grm_info_t *grm_info_p, gas_id_t gas_id )
{
  if(grm_info_p)
  {
    /* Store TRM when value for debugging */
    gl1_hw_sleep_wmgr_trm_when_at_gts[gas_id]       = grm_info_p->when;
    gl1_hw_sleep_wmgr_trm_when_cur[gas_id]          = grm_info_p->when;
    gl1_hw_sleep_wmgr_trm_res_fn_at_gts[gas_id]     = l1_fm_get_trm_reserved_fn(gas_id);
    gl1_hw_sleep_wmgr_trm_when_at_gts_valid[gas_id] = TRUE;
  }
  else
  {
    gl1_hw_sleep_wmgr_trm_when_at_gts_valid[gas_id] = FALSE;
  }

  return;
}

/*===========================================================================

FUNCTION  grm_reserve_at_gts

DESCRIPTION
  TRM booking has been done in grm_reserve_at_enhanced().
  If goes to sleep, it needs to extend TRM reservation to include
  sleep warmup duration.

DEPENDENCIES
  None

RETURN VALUE
  Void

===========================================================================*/
void grm_reserve_at_gts(int32 wup_duration, uint32 wakeup_fn, gas_id_t gas_id)
{
  grm_client_enum_t         client_id;
  grm_info_t               *grm_info_p;
  trm_reason_enum_t         trm_reason;
  volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint32                    trm_fn;

  grm_info_p = grm_get_cur_reservation(wakeup_fn, gas_id);

  client_id  = l1_tskisr_blk->client_id;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  if(gl1_get_trm_global_overide_status(gas_id))
  {
    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_SLEEP_EARLY_CAMP);
  }
#endif

  if ( grm_info_p )
  {

    /* We are updating the same booking so rsv reason can't change */
    trm_reason                = grm_info_p->rsv_reason;

    /* check if we have already performed TRM adjustment to include warmup */
    trm_fn = l1_fm_get_trm_reserved_fn(gas_id);

    if( trm_fn != gl1_hw_sleep_wmgr_warmup_incl_in_trm_fn[gas_id] )
    {
      /* not a duplicate reservation/not already reserved, so go ahead */

      /* TRM reservation done here will include warmup time (store FN) */
      gl1_hw_sleep_wmgr_warmup_incl_in_trm_fn[gas_id] = trm_fn;

      grm_info_p->duration     += wup_duration;
      grm_info_p->when         -= wup_duration;

     /*first reservation for any activity is done from frame manager,
       We may see the wakeup conflict if it is colliding with other subs reservation and pending wmgr event is set from wmgr callback
       2nd reservation is done from GTS with warmup duration added, With updated duration TRM might not see collision with other subs activity,
       so we should defer the wakeup only based on latest reservatoin*/

      /*clear pending deferred flag before putting reservation from GTS */
      gl1_hw_sleep_wmgr_mutex_lock(gas_id);

      gl1_hw_sleep_clear_any_pending_deferred_wmgr_event(gas_id);

      gl1_hw_sleep_wmgr_mutex_unlock(gas_id);

    grm_reserve_at( client_id,
                    grm_info_p->when,
                    grm_info_p->duration,
                    trm_reason,
                    grm_info_p->rsv_sub_reason,
                    grm_info_p->resource,
                    TRUE );

    grm_set_trm_status(GL1_TRM_NULL, client_id, gas_id);

    gl1_multi_sim_reset_trm_request_state( gas_id );

      MSG_GERAN_MED_5_G("GSM updated TRM booking at gts reason %d dur %d @ %u wupd: %d trm_fn: %d",
                        trm_reason,
                        grm_info_p->duration,
                        grm_info_p->when,
                        wup_duration,
                        trm_fn);
    }
    else
    {
      /* TRM was already updated with warmup duration, so don't update it again */
      MSG_GERAN_MED_5_G("TRM booking NOT updated at gts reason %d dur %d @ %u wupd: %d trm_fn: %d",
                      trm_reason,
                      grm_info_p->duration,
                      grm_info_p->when,
                        wup_duration,
                        trm_fn);
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G( "grm_info_p returned as NULL pointer, client ID: %d!", client_id );
  }

  /* store GRM/TRM reservation for use in wkup cancellation */
  grm_store_cur_reservation(grm_info_p, gas_id );
}

/*===========================================================================

FUNCTION  grm_update_reserve_reason

DESCRIPTION
  TRM booking has been done in grm_reserve_at_enhanced().
  If RR updates a priority change for an activity last reserved before going to sleep.
  Update the reservation with new TRM prio.

DEPENDENCIES
  None

RETURN VALUE
  Void

===========================================================================*/

void grm_update_reserve_reason(l1_fm_priority_T pri, gas_id_t gas_id)
{
    grm_info_t               *grm_info_p;
    volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    grm_info_p = grm_get_info_ptr_from_client( l1_tskisr_blk->client_id, gas_id );

    if ( grm_info_p )
    {
        /* Use TRM lock so that no parallel TRM request happens from warmup callback */
        GL1_TRM_LOCK();
        if (grm_get_trm_status (l1_tskisr_blk->client_id, gas_id) == GL1_TRM_NULL)
        {
            /*Check for new reason based on new FM priority*/
            grm_info_p->rsv_reason    = l1_fm_map_pri_to_trm(pri, gas_id);

            grm_reserve_at( l1_tskisr_blk->client_id,
                            grm_info_p->when,
                            grm_info_p->duration,
                            grm_info_p->rsv_reason,
                            grm_info_p->rsv_sub_reason,
                            grm_info_p->resource,
                            TRUE );

            grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);

            MSG_GERAN_MED_1_G("GSM updated TRM booking reason %d",
                            grm_info_p->rsv_reason);
        }
        GL1_TRM_UNLOCK();
    }
    else
    {
        MSG_GERAN_ERROR_1_G( "grm_info_p returned as NULL pointer, client ID: %d!", l1_tskisr_blk->client_id );
    }
}


/*===========================================================================
FUNCTION  gl1_check_trm_grm_reasons_match

DESCRIPTION
  This function checks TRM modules reservation reason with GRM data stores reservation reason

DEPENDENCIES
  None

RETURN VALUE
  TRUE if TRM/GRM reasons match or else FALSE

===========================================================================*/
boolean gl1_check_trm_grm_reasons_match(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t client_id = l1_tskisr_blk->client_id;
  grm_info_t *grm_info_gsm_ptr;
  boolean ret_val = FALSE;

  grm_info_gsm_ptr = grm_get_info_ptr_from_client( client_id, gas_id );

  if(grm_info_gsm_ptr)
  {
    if(grm_info_gsm_ptr->rsv_reason == grm_get_reason(l1_tskisr_blk->client_id,
                                                      gas_id))
    {
      ret_val = TRUE;
    }
  }
  return (ret_val);
}

/*===========================================================================

FUNCTION  grm_release_or_exchange

DESCRIPTION
     Releases the chain held by a client, or exchanges it with some other

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
void grm_release_or_exchange(grm_client_enum_t client_id, gas_id_t gas_id )
{

#if defined (FEATURE_GSM_GPRS_READ_CCCH_IN_XFER) || defined (FEATURE_QTA)
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

  /* query whether GRR wants GL1 to exchange the lock, or release it */
  geran_exchange_direction_t exchange_direction =
    geran_get_exchange_on_release( gas_id );

#if defined (FEATURE_QTA)
  if((l1_tskisr_blk->qta_in_progress) && (exchange_direction != GERAN_EXCHANGE_NULL))
  {
    geran_set_exchange_on_release( GERAN_EXCHANGE_NULL , gas_id );
    exchange_direction = GERAN_EXCHANGE_NULL;
    MSG_GERAN_HIGH_1_G( "Avoiding exchange in QTA gap:%d ", exchange_direction);
  }
#endif

  if ( GERAN_EXCHANGE_NULL != exchange_direction )
  {
    /* for G2X exchange, perform the band release first (on the GSM source client) */
    if ( GERAN_EXCHANGE_GTOX == exchange_direction )
  {
      grm_send_band_release_if_required(client_id, gas_id);
    }

    /* obey the exchange instruction instead of releasing */
    if ( geran_exchange_lock( exchange_direction, gas_id ) )
    {
      MSG_GERAN_HIGH_2_G( "GL1 call to geran_set_exchange_on_release client:%d "
                          "curr sclk:%d",
                          client_id, timetick_get_safe() );

      geran_set_exchange_on_release( GERAN_EXCHANGE_NULL, gas_id );
    }
    else
    {
      /* exchange failed, release explicitly here, just to be safe */
      MSG_GERAN_HIGH_2_G( "GL1 call to grm_release client:%d curr sclk:%d",
                          client_id, timetick_get_safe() );

      grm_release( client_id, gas_id );
    }
  }
  else
  {
    MSG_GERAN_MED_2_G( "GL1 call to grm_release client:%d curr sclk:%d",
                        client_id, timetick_get_safe() );

    /* issue the TRM release for the specified client */
    grm_release( client_id, gas_id );

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    l1_tskisr_blk->client_id = gl1_map_gas_id_to_client_id_pchxfer(gas_id);
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

  }

  return;
}

/*===========================================================================

FUNCTION  grm_request_at_warmup

DESCRIPTION
  Forwards a GSM client request to the TRM layer at sleep warmup

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  May intercept requests for TRM_DEMOD_PAGE and apply priority inversion

===========================================================================*/
void grm_request_at_warmup( gas_id_t gas_id )
{
  grm_info_t                 *grm_info_p;
  trm_grant_return_enum_type  req_result;
  uint32                      fn_at_warmup;
  uint8                       missed_frames;
  trm_reason_enum_t           grm_reason;
  grm_client_enum_t           client_id;
  boolean                     non_valid_freq_info;
  gl1_trm_state_t trm_status = GL1_TRM_NULL;

  volatile ISRTIM_CMD_BLK *l1_tskisr_blk_p = &l1_tsk_buffer[gas_id];

  client_id  = l1_tskisr_blk_p->client_id;
  req_result = TRM_DENIAL;

  grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

  trm_status = grm_get_trm_status(client_id, gas_id);
  if ( ( trm_status == GL1_TRM_GRANTED ) ||
       ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS ) )
  {
    MSG_GERAN_MED_2_G( "Don't request TRM at WARMUP trm_status %d main_command %d",
                       trm_status, l1_tskisr_blk_p->main_command );

    if ( non_valid_freq_info )
    {
      grm_clear_freq_info( client_id, gas_id );
    }

    return;
  }


  missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);
  fn_at_warmup  = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), 1);
  fn_at_warmup  = SUB_FN(fn_at_warmup, missed_frames);

  if ( !l1_fm_check_reserved_trm_activity( gas_id, fn_at_warmup ) )
  {
    MSG_GERAN_MED_2_G( "Don't request TRM at warmup FN %d missed frames %d",
                       fn_at_warmup, missed_frames );

    if ( non_valid_freq_info )
    {
      grm_clear_freq_info( client_id, gas_id );
    }

    return;
  }

  grm_reason = grm_gl1_get_current_trm_reason(client_id, gas_id);

  if ( TRM_NUM_REASONS == grm_reason)
  {
    MSG_GERAN_ERROR_0_G("no reason booked for TRM, probably Request being sent before Reservation time!!!");
  }

  if ( grm_info_p )
  {
    req_result = grm_request( client_id,
                              grm_info_p->duration,
                              grm_info_p->rsv_reason,
                              grm_info_p->rsv_sub_reason,
                              grm_info_p->resource,
                              GRM_DIVRX_TRM_REQ_IDLE );

    grm_set_trm_grant_status(req_result, client_id, gas_id);
    grm_set_trm_requested_at_warmup(TRUE, client_id, gas_id);

    if ( req_result != TRM_DENIAL )
    {
      grm_set_trm_status(GL1_TRM_GRANTED, client_id, gas_id);
      grm_set_trm_grant_fn(gl1_get_FN( gas_id ), client_id, gas_id);
      grm_set_trm_grant_frames(grm_info_p->frame_duration,
                               client_id,
                               gas_id);
    }
    else
    {
      grm_set_trm_status(GL1_TRM_NOT_GRANTED, client_id, gas_id);
      grm_set_trm_grant_fn(NOT_A_FRAME_NUMBER, client_id, gas_id);
      grm_set_trm_grant_frames(0, client_id, gas_id);
    }
  }
  MSG_GERAN_MED_3_G(GL1_SLEEP_HDR" GERAN Warm up request TRM lock %d grant_fn:%d grant_frames:%d",
                     grm_get_trm_status(client_id, gas_id),
                     grm_get_trm_grant_fn(client_id, gas_id),
                     grm_get_trm_grant_frames(client_id, gas_id));

  if ( non_valid_freq_info )
  {
    grm_clear_freq_info( client_id, gas_id);
  }
  return;
}

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
/*===========================================================================
FUNCTION  GRM_REQUEST_DIVRX_AT_WARMUP

DESCRIPTION
  Send TRM request and notify to the TRM layer for the diveristy client. 
  This function is called from sleep warmup callback. The duration is set to
  10ms and diversity client is mapped correctly from primary client.

DEPENDENCIES
  This module should only be called during sleep wakeup in idle mode.

RETURN VALUE
  Void

SIDE EFFECTS
===========================================================================*/
void grm_request_divrx_at_warmup(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk_p = &l1_tsk_buffer[gas_id];
  grm_client_enum_t divrx_client_id = GRM_NO_CLIENT;
  gl1_trm_state_t divrx_trm_status = GL1_TRM_NULL;

  /* Retrieve diversity client id from primary client and send a TRM request and
     notify using Drx client. Ensure that the client used is valid */
  divrx_client_id = grm_map_prx_to_divrx_client_id(l1_tskisr_blk_p->client_id, gas_id);
  if (divrx_client_id < GRM_MAX_CLIENTS)
  {
    /* Send request and notify only when the current status is other than TRM_GRANTED
       to avoid sending mutliple request and notify to TRM */
    divrx_trm_status = grm_get_trm_status(divrx_client_id, gas_id);
    if ((divrx_trm_status != GL1_TRM_GRANTED)
        && (divrx_trm_status != GL1_TRM_CALLBACK_PENDING))
    {
      /* Send GRM request and notify with static duration of 10ms for the secondary client */
      grm_request_and_notify(divrx_client_id,
        TRM_RX_SECONDARY,
        GRM_IDLEWARMUP_TRM_ACCESS_DURATION_SCLKS,
        TRM_DIVERSITY,
        GRM_DIVRX_TRM_REQ_IDLE,
        SYS_PROC_TYPE_DIVERSITY_CHAIN,
        GRM_RAN_ACCESS_RETRY);
    }
  }

  MSG_GERAN_HIGH_2_G(GL1_RXD"ANT_SHARING: During sleep warmup try TRM R&N for (Client, Status): (%d, %d)",
    divrx_client_id, divrx_trm_status);

  return;
}
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

/*===========================================================================

FUNCTION  grm_request_and_notify_enhanced

DESCRIPTION
  Forwards a GSM request and notify to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  boolean TRUE indicates request forwarded to TRM

SIDE EFFECTS
  May intercept reservations for TRM_ACQUISITION and apply priority inversion

===========================================================================*/
boolean grm_request_and_notify_enhanced(grm_client_enum_t   client_id,
                                     trm_resource_enum_t resource,
                                     trm_duration_t      duration,
                                     trm_reason_enum_t   reason,
                                     sys_proc_type_e_type sub_reason,
                                     grm_tag_t           grm_tag )
{
  boolean     non_valid_freq_info;
  boolean     rane_status = FALSE;

  gas_id_t    gas_id     = gl1_map_client_id_to_gas_id( client_id );
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if (grm_get_resource_lock_state(gas_id) || ( gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED ))
  {
    MSG_GERAN_HIGH_0_G( GL1_RXD"grm_request_and_notify_enhanced ignored - "
                               "TRM resource lock is active" );
    return FALSE;
  }
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

  non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

#ifdef FEATURE_GSM_RX_TX_SPLIT
  if (grm_is_tx_reason(reason) && (FALSE == grm_is_a_secondary_client(client_id)))
  {
    /*Grouped Request*/
    grm_grouped_enhanced_request_and_notify(client_id,
                                            resource,
                                            duration,
                                            reason,
                                            sub_reason,
                                            grm_tag,
                                            gas_id);
  }
  else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  {
    if ( grm_info_p )
    {
      trm_request_notify_enh_input_info *gl1_trm_request_notify_enh_input_info_p = NULL;

      trm_request_notify_enh_input_info  gl1_trm_request_notify_enh_input_info = { 0 };

      trm_reason_enum_t rane_reason = reason;

      /* Override for acquisition */
      if ((reason == TRM_ACQUISITION) || (reason == TRM_ACQUISITION_CELL_SELECT))
      {
        rane_reason = grm_get_acquisition_reason(reason, gas_id);
      }

      grm_set_req_and_notify_duration(duration, client_id, gas_id);

      /*update local store with RnN reason*/
      grm_gl1_set_current_trm_reason(rane_reason, sub_reason, client_id, gas_id);

      gl1_trm_request_notify_enh_input_info_p = &gl1_trm_request_notify_enh_input_info;

      gl1_trm_request_notify_enh_input_info_p->client_id =
                             grm_map_grm_client_id_to_trm_client_id(client_id);
      gl1_trm_request_notify_enh_input_info_p->resource  = resource;
      gl1_trm_request_notify_enh_input_info_p->duration  = duration;
      gl1_trm_request_notify_enh_input_info_p->reason    = rane_reason;

      gl1_trm_request_notify_enh_input_info_p->ta_info    = TRM_LONG_TUNEAWAY;
#ifdef FEATURE_GL1_ENABLE_QTA_IN_PSCAN
      if(IS_X_PLUS_G() && 
         IS_QTA_REASON_RANE(rane_reason, l1_tsk_buffer[gas_id].l1_state, l1_tsk_buffer[gas_id].sub_state) &&
         (duration <= (trm_duration_t)(L1_QTA_DURATION_GAP_MAX_IN_SCLKS)))
      {
         gl1_trm_request_notify_enh_input_info_p->ta_info    =    TRM_QUICK_TUNEAWAY;
      }
#endif

      /* Tag with expected GL1 callback identifier */
      gl1_trm_request_notify_enh_input_info_p->tag         = (trm_tag_type)grm_tag;
      gl1_trm_request_notify_enh_input_info_p->unlock_tag  = gl1_trm_request_notify_enh_input_info_p->tag;

      gl1_trm_request_notify_enh_input_info_p->sub_reason = sub_reason;
      /* Active band info for client , and also when receiving set prio ind. 
         we want to read the freq info again 
         RnN for set prio ind should be triggered only for acquisition in Foreground & background*/
      if ( gl1_trm_request_notify_enh_input_info_p->tag == GRM_RANE_ACQUISITION )
      {
        uint8 i;

        for ( i = 0;
            ( ( i < MAX_BAND_INFO_GRM ) &&
              ( i < ARR_SIZE( gl1_trm_request_notify_enh_input_info_p->freq_info.bands ) ) );
            i++ )
        {
          if ( grm_valid_gsm_sys_bands[gas_id][i] == SYS_BAND_CLASS_NONE )
          {
            break;
          }

          gl1_trm_request_notify_enh_input_info_p->freq_info.bands[i].band =
            grm_valid_gsm_sys_bands[gas_id][i];
        }

        /* for acquisition report all possible valid bands */
        gl1_trm_request_notify_enh_input_info_p->freq_info.num_bands = i;
      }
      else
      {
        gl1_trm_request_notify_enh_input_info_p->freq_info = grm_info_p->grm_freq_info;
      }

      MSG_GERAN_HIGH_5_G( "L1 call to grm_request_and_notify_enhanced reason:%d "
                          "client:%d dur:%d grm_tag:%d, unlock_tag:%d",
                          (int)rane_reason, (int)client_id, (int)duration, grm_tag,
                          gl1_trm_request_notify_enh_input_info_p->unlock_tag );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      /*Call Band Release before request and notify to TRM*/
      grm_send_band_release_if_required(client_id, gas_id);
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
	  grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, client_id, gas_id);


#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
      if ( els_active )
      {
        grm_els_state_update_aperiodic(rane_reason,
                                       sub_reason,
                                       duration,
                                       client_id,
                                       gas_id);
      }else
      {
        MSG_GERAN_HIGH_0_G("!els_active"); 
      }
#endif /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */
      trm_request_and_notify_enhanced( gl1_trm_request_notify_enh_input_info_p );

      /* nuke the frequency info store for RANE_ACQUISITION other cases leave alone */
      if ( !non_valid_freq_info
           && (gl1_trm_request_notify_enh_input_info_p->tag == GRM_RANE_ACQUISITION) )
      {
        grm_clear_freq_info( client_id, gas_id );
      }
      rane_status = TRUE;
    }
    else
    {
      MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
    }

    if ( non_valid_freq_info )
    {
      grm_clear_freq_info( client_id, gas_id );
    }
  }
  return rane_status;
}

/*===========================================================================

FUNCTION  grm_request_and_notify

DESCRIPTION
  Forwards a GSM request and notify to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  boolean TRUE indicates request forwarded to TRM

SIDE EFFECTS

===========================================================================*/
boolean grm_request_and_notify(grm_client_enum_t   client_id,
                            trm_resource_enum_t resource,
                            trm_duration_t      duration,
                            trm_reason_enum_t   reason,
                            grm_divrx_request_type    req_type,
                            sys_proc_type_e_type  sub_reason,
                            grm_tag_t           grm_tag )
{
  boolean     non_valid_freq_info;
  gas_id_t    gas_id     = gl1_map_client_id_to_gas_id( client_id );
  grm_info_t  *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );
  boolean     ran_status  = FALSE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if (grm_get_resource_lock_state(gas_id) || ( gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED ))
  {
    MSG_GERAN_HIGH_0_G( GL1_RXD"grm_request_and_notify ignored - TRM resource lock is active" );
    return FALSE;
  }
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

  non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

  GL1_TRM_LOCK();

#ifdef FEATURE_GSM_RX_TX_SPLIT
  if (grm_is_tx_reason(reason))
  {
    /*Grouped Request*/
    grm_grouped_request_and_notify(client_id,
                                   duration,
                                   reason,
                                   sub_reason,
                                   req_type,
                                   grm_tag,
                                   gas_id);
  }
  else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  {
    if ( grm_info_p )
    {
      trm_request_notify_input_info *grm_request_notify_input_info_p;

      trm_request_notify_input_info  grm_request_notify_input_info = { 0 };

      grm_request_notify_input_info_p = &grm_request_notify_input_info;

      grm_request_notify_input_info_p->client_id =
                      grm_map_grm_client_id_to_trm_client_id(client_id);
      grm_request_notify_input_info_p->resource  = resource;
      grm_request_notify_input_info_p->duration  = duration;
      grm_request_notify_input_info_p->reason    = reason;
      grm_request_notify_input_info_p->sub_reason = sub_reason;
      grm_request_notify_input_info_p->ta_info = TRM_LONG_TUNEAWAY;
      /*update local store with RnN reason*/
      grm_gl1_set_current_trm_reason(reason, sub_reason, client_id, gas_id);

      /* Tag with expected GL1 callback identifier */
      grm_request_notify_input_info_p->tag       = (trm_tag_type)grm_tag;

      /* Active band info for client */
      grm_request_notify_input_info_p->freq_info = grm_info_p->grm_freq_info;

      MSG_GERAN_HIGH_4_G( "L1 call to grm_request_and_notify resource:%d dur:%d reason:%d grm_tag:%d",
                          (int)resource, (int)duration, (int)reason, grm_tag );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      /*Call band release before request and notify call to TRM*/
      grm_send_band_release_if_required(client_id, gas_id);
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

	  grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, client_id, gas_id);
#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
      if (els_active &&  !grm_is_a_secondary_client (client_id) )
      {
        grm_els_state_update_aperiodic(reason,
                                       sub_reason,
                                       duration,
                                       client_id,
                                       gas_id);
      }else
      {
        MSG_GERAN_HIGH_0_G("!els_active"); 
      }
#endif /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */
      /* Request the lock now for this using the asynchronous TRM interface */
      trm_request_and_notify( grm_request_notify_input_info_p );
      ran_status = TRUE;
    }
    else
    {
      MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
    }

    if ( non_valid_freq_info )
    {
      grm_clear_freq_info( client_id, gas_id);
    }
  }

  GL1_TRM_UNLOCK();
  return ran_status;
}

/*===========================================================================

FUNCTION  grm_force_request_and_notify

DESCRIPTION
  Disables any GRM resource lock before GSM request and notify to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS

===========================================================================*/
void grm_force_request_and_notify(grm_client_enum_t   client_id,
                                  trm_resource_enum_t resource,
                                  trm_duration_t      duration,
                                  trm_reason_enum_t   reason,
                                  sys_proc_type_e_type sub_reason,
                                  grm_tag_t           grm_tag )
{
  gas_id_t gas_id;
  boolean resource_lock_active;

  gas_id = gl1_map_client_id_to_gas_id ( client_id );
  resource_lock_active = grm_get_resource_lock_state(gas_id);

  /* If GRM resources have been locked by resource lock procedure */
  if (resource_lock_active)
  {
    /* Release lock for the duration of this request and notify */
    grm_set_resource_lock_state(FALSE,gas_id);
  }

  grm_set_trm_status (GL1_TRM_CALLBACK_PENDING, client_id, gas_id);

  if (grm_tag == GRM_RAN_ACCESS_RETRY)
  {
    grm_request_and_notify (client_id, resource, duration, reason,GRM_DIVRX_TRM_REQ_INVALID,  sub_reason, grm_tag);
  }
  else//Should be a RnN Enhanced Reason, As GL1 uses RnN only for Access Retry .
  {
    grm_request_and_notify_enhanced(client_id,resource,duration,reason,sub_reason,grm_tag);
  }

  if (resource_lock_active)
  {
    /* Restore resource lock */
    grm_set_resource_lock_state(TRUE,gas_id);
  }
  return;
}

/*===========================================================================

FUNCTION  grm_set_drx_cycle

DESCRIPTION
  Called when computing the paging cycle
  TRM needs DRx Cycle to compute Page Block Rate.
  GL1 need to update as and when DRx Cycle changes

DEPENDENCIES

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_drx_cycle(uint32             paging_mframes,
                       grm_client_enum_t  client_id,
                       gas_id_t           gas_id )
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
    /* check whether the paging cycle has changed */
    if ( grm_info_p->curr_bs_pa_mfrms != paging_mframes )
    {
      grm_register_pbr_info( client_id,
                             FRAMES_TO_MSECS( paging_mframes * MULTIFRAME_51 ) );

      grm_reset_paging_info( grm_info_p, paging_mframes );

      grm_initialise_pm_denial_count(client_id, gas_id);
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_register_pbr_info

DESCRIPTION
  Called during GRM initialisation to register for modes that needs Markov Resolution.
  can call this api multiple times, if deemed necessary.

  PBR Modes is a bit-map of TRM modes.

DEPENDENCIES

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_register_pbr_info(grm_client_enum_t client_id,
                           uint32            drx_cycle )
{
  trm_register_feature_input_info *pbr_input_info_p;

  trm_register_feature_input_info  pbr_input_info = { 0 };

  gas_id_t    gas_id             = gl1_map_client_id_to_gas_id( client_id );

  pbr_input_info_p               = &pbr_input_info;

  pbr_input_info_p->client_id    = grm_map_grm_client_id_to_trm_client_id(client_id);

  pbr_input_info_p->feature_info = TRM_FEATURE_PBR;

  pbr_input_info_p->register_info.new_pbr_info.type =
    TRM_MODIFY_PBR_MODES;

  pbr_input_info_p->register_info.new_pbr_info.new_modes.client =
    grm_map_grm_client_id_to_trm_client_id( client_id );
  pbr_input_info_p->register_info.new_pbr_info.new_modes.pbr_modes =
    (uint64)( 1 << TRM_DEMOD_PAGE );

  trm_register_feature( pbr_input_info_p );

  /* Clear old data for fresh info */
  memset( pbr_input_info_p, 0, sizeof( pbr_input_info ) );

  pbr_input_info_p->client_id    = grm_map_grm_client_id_to_trm_client_id(client_id);

  pbr_input_info_p->feature_info = TRM_FEATURE_PBR;

  pbr_input_info_p->register_info.new_pbr_info.type =
    TRM_MODIFY_PBR_DRX_CYCLE;

  pbr_input_info_p->register_info.new_pbr_info.new_drx_cycle.client =
    grm_map_grm_client_id_to_trm_client_id( client_id );
  pbr_input_info_p->register_info.new_pbr_info.new_drx_cycle.drx_cycle =
    drx_cycle;

  trm_register_feature( pbr_input_info_p );

  MSG_GERAN_HIGH_2_G( "PBR grm_register_pbr_info client_id:%d drx_cycle:%d",
                      client_id, drx_cycle );

  return;
}
/*===========================================================================

FUNCTION  grm_enable_disable_dual_receive

DESCRIPTION
  Called to indicate current DR enabled status and to allow for changes
  to be requested.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if DR-DSDS is enabled, FALSE if not

SIDE EFFECTS

===========================================================================*/
boolean grm_enable_disable_dual_receive(boolean           enable,
                                        grm_client_enum_t client_id,
                                        gas_id_t          gas_id)
{

  trm_dual_receive_enum_type dual_receive_status;

  /*Var To Store DR Enable Disable Data */
  static grm_sc_activity_state_info_t grm_enable_disable_dual_receive_data[NUM_GERAN_DATA_SPACES] =
                                        { INITIAL_VALUE( NULL) };

  /*If Debug NV is set, do not allow DR Disable in any case*/
  if (!enable && GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_DISABLE_HYBRID_FB_FRM_DR))
  {
      MSG_GERAN_MED_0_G(GL1_DRDS_DBG_HDR"DR Disable Not Allowed when NV 65770 is set");
      return TRUE;
  }

  if ( !enable )
  {
    grm_enable_disable_dual_receive_data[gas_id].client_state_info.data.dr_data.dr_vote = TRM_DUAL_RECEIVE_DISABLED;
  }
  else
  {
    grm_enable_disable_dual_receive_data[gas_id].client_state_info.data.dr_data.dr_vote = TRM_DUAL_RECEIVE_ENABLED;
  }

  grm_enable_disable_dual_receive_data[gas_id].client_state_info.client_id    = grm_map_grm_client_id_to_trm_client_id(client_id);
  grm_enable_disable_dual_receive_data[gas_id].client_state_info.client_state = TRM_DUAL_RECEIVE;

  trm_set_client_state( &(grm_enable_disable_dual_receive_data[gas_id].client_state_info), 
                        &(grm_enable_disable_dual_receive_data[gas_id].trm_return_data) );

  dual_receive_status = grm_get_dr_capability(
                         grm_map_trm_client_id_to_grm_client_id(grm_enable_disable_dual_receive_data[gas_id].client_state_info.client_id),
                         gas_id );

  MSG_GERAN_HIGH_3_G(GL1_DRDS_DBG_HDR"grm_enable_disable_dual_receive: DR enable:%d dr_vote:%d dual_receive_status:%d ",
                      enable,
                      grm_enable_disable_dual_receive_data[gas_id].client_state_info.data.dr_data.dr_vote,
                      dual_receive_status);
  /* Sanity check that TRM returns correct DR setting */
  if ( dual_receive_status != grm_enable_disable_dual_receive_data[gas_id].client_state_info.data.dr_data.dr_vote )
  {
    MSG_GERAN_ERROR_3_G( GL1_DRDS_DBG_HDR"DR disable delayed by TRM assume: TRM report "
                                         "status %d and requested %d for client_id:%d",
                         dual_receive_status,
                         grm_enable_disable_dual_receive_data[gas_id].client_state_info.data.dr_data.dr_vote,
                         grm_enable_disable_dual_receive_data[gas_id].client_state_info.client_id );
  }

  return ( TRM_DUAL_RECEIVE_ENABLED == dual_receive_status );
}

/*===========================================================================

FUNCTION grm_send_client_state_update_start

DESCRIPTION
 Registers frequency  bands set with TRM and updates Info store with any denied
 bands. This information will be used by arbitrator  to skip PM measurements on
 denied GSM bands. Information also used by GL1 scheduler to decide whether to
 skip IRAT or NCELL FCH+SCH and NBCCH activities.

PARAMS
 grm_sc_activity_state_info_t* pointer to info store for sub performing the
 measurement activity  (non-serving band Ncell activities, IRAT)

DEPENDENCIES

RETURN VALUE
void

===========================================================================*/
void grm_update_client_state_info(grm_sc_activity_state_info_t  *activity_info,
                                  gl1_sc_activities_t           activity,
                                  grm_client_enum_t             client_id,
                                  gas_id_t                      gas_id)
{
  grm_info_t *grm_info_ptr = NULL;

  if (NULL == activity_info)
  {
    MSG_GERAN_ERROR_0_G(GRM_TM_HDR"activity_info is NULL");
    return;
  }

  grm_info_ptr = grm_get_info_ptr_from_client(
     grm_map_trm_client_id_to_grm_client_id(activity_info->client_state_info.client_id), gas_id);

  gl1_grm_info_mutex_lock(gas_id);

  if(activity_info->update_in_progress
     && (activity_info->client_state_info.client_state == TRM_IRAT_STATE)
     && (activity_info->client_state_info.data.irat_data.operation == TRM_START_STATE))
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0_G(GRM_TM_HDR"Error: Already ongoing IRAT activity");
  }



  /*Populate is_diversity_needed flag for LTE IRAT, this should populate the divesity fields from TRM Side*/
  if ((activity_info->client_state_info.client_state) == TRM_IRAT_STATE)
  {
    MSG_GERAN_MED_1_G(GRM_TM_HDR"G2X_div: DBG: Irat activity on measured client: %d",
                       activity_info->client_state_info.data.irat_data.measured_client);

    switch (activity_info->client_state_info.data.irat_data.measured_client)
    {
      case TRM_LTE:
      case TRM_LTE_SUB2:
      {
        MSG_GERAN_MED_0_G(GRM_TM_HDR"G2X_div: DBG: is diversity needed is TRUE with TRM for LTE!");
        activity_info->client_state_info.data.irat_data.is_diversity_needed = TRUE;
        break;
      }

      case TRM_TDSCDMA:
      {
        MSG_GERAN_MED_0_G(GRM_TM_HDR"G2X_div: DBG: is diversity needed is FALSE with TRM for TDSCDMA!");
        activity_info->client_state_info.data.irat_data.is_diversity_needed = FALSE;
        break;
      }

      case TRM_UMTS:
      case TRM_UMTS2:
      {
        MSG_GERAN_MED_0_G(GRM_TM_HDR"G2X_div: DBG: is diversity needed is FALSE with TRM for UMTS!");
        activity_info->client_state_info.data.irat_data.is_diversity_needed = FALSE;
        break;
      }

      default:
      {
        MSG_GERAN_ERROR_1_G(GRM_TM_HDR"G2X_div: ERROR: DBG: G2X IRAT On unknown client %d ",activity_info->client_state_info.data.irat_data.measured_client);
        activity_info->client_state_info.data.irat_data.is_diversity_needed = FALSE;
        break;
      }
    }
  }

  trm_set_client_state(&activity_info->client_state_info,&activity_info->trm_return_data);

  grm_process_client_state_update_result(activity_info,
                                         activity,
                                         client_id,
                                         gas_id);

  gl1_grm_info_mutex_unlock(gas_id);

  MSG_GERAN_MED_3_G(GRM_TM_HDR"grm_update_client_state_info: client_id %d, trans_id %d update_in_progress %d",
                   activity_info->client_state_info.client_id ,
                   activity_info->trm_return_data.transaction_id,
                   activity_info->update_in_progress);
}


/*===========================================================================

FUNCTION grm_clear_stale_client_state_information

DESCRIPTION
  nuke any stored frequency band information. Should be done before building
  a new trm_set_client_state_req
  ahead of IRAT or inter-band FCH+SCH, NBCCH

PARAMS
  client_id

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void grm_clear_stale_client_state_information(
                                  grm_client_enum_t             client_id,
                                  grm_sc_activity_state_info_t  *act_info_ptr,
                                  gas_id_t                       gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  if(grm_info_p)
  {
    gl1_grm_info_mutex_lock(gas_id);
    memset( act_info_ptr, 0, sizeof(grm_sc_activity_state_info_t) );
    gl1_grm_info_mutex_unlock(gas_id);
  }
  else
  {
    MSG_GERAN_ERROR_0_G(GRM_TM_HDR"NULL GRM Info Pointer!!");
  }
  return;
}

/*===========================================================================

FUNCTION grm_send_band_release_if_required

DESCRIPTION
  clears any running IFS/IRAT activity

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void grm_send_band_release_if_required(grm_client_enum_t client_id,
                                       gas_id_t          gas_id )
{
  grm_info_t *grm_info_p = NULL;

  /*Current RxD scope: Band release is not applicable */
  if(TRUE == grm_is_a_secondary_client(client_id))
  {
    return;
  }

  /* Cover the following cases:
   * We don't want to issue a band release if the requested client does not
   * map to the one currently represented by the gas id.
   *
   * We don't want to release band in the case of reading PCH during packet transfer
   * because it involves exchanging GSM1 <-> GPRS1 and GSM2 <-> GPRS2.
   */
  if( ( client_id != gl1_map_gas_id_to_client_id(gas_id) )
    &&( !( (client_id == GRM_GPRS1)  || (client_id == GRM_GPRS2) ))
    )
  {
    MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"band release ignored src client %d dest %d",
                        gl1_map_gas_id_to_client_id(gas_id),
                        client_id
                      );

    return;
  }

  grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  MSG_GERAN_LOW_0_G(GL1_DRDS_DBG_HDR"Calling grm_band_release_if_required");

  if ( grm_info_p )
  {
    int i;

    gl1_grm_info_mutex_lock(gas_id);

    for (i = 0; i < SC_MAX_ACTIVITIES; i++)
    {
      grm_transaction_manager(GRM_TM_FLUSH, i, client_id, gas_id);
    }
    gl1_grm_info_mutex_unlock(gas_id);
  }
}

/*===========================================================================

FUNCTION grm_add_sys_band_type_to_trm_freq_input_type

DESCRIPTION
  Adds new freq_info band information to to active list for client if required

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void grm_add_trm_band_to_freq_input_type(trm_band_t          band,
                                         grm_client_enum_t   client_id,
                                         gas_id_t            gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  int i;
  sys_band_T gsm_band;
  if (grm_info_p == NULL)
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! client %d",client_id);
    return;
  }

    if( band == SYS_BAND_CLASS_NONE)
    {
      MSG_GERAN_ERROR_0("grm_add_trm_band_to_freq_input_type: band == SYS_BAND_CLASS_NONE!!");
      return;
    }


    gl1_grm_info_mutex_lock(gas_id);
    /* First valid band entry */
    if ( grm_info_p->grm_freq_info.num_bands == 0)
    {
      grm_info_p->grm_freq_info.num_bands++;
      grm_info_p->grm_freq_info.bands[0].band = band;
    }
    else
    {
      if (grm_info_p->grm_freq_info.num_bands > MAX_BAND_INFO)
      {
        MSG_GERAN_ERROR_1(" num_bands out of bounds %d",grm_info_p->grm_freq_info.num_bands);
      }
      else
      {
        /* Scan and see if new band needs adding to pending request */
        for ( i = 0; i < grm_info_p->grm_freq_info.num_bands; i++ )
        {
          if ( band == grm_info_p->grm_freq_info.bands[i].band ||
             i == INVALID_BAND)
          {
            gl1_grm_info_mutex_unlock(gas_id);
            return;
          }
        }
        /* Bounds check to ensure we are not overwriting */
        if ( i < MAX_BAND_INFO)
        {
          grm_info_p->grm_freq_info.bands[i].band = band;
          grm_info_p->grm_freq_info.num_bands++;
          gsm_band = gl1_map_sys_band_to_gsm_band( (sys_band_class_e_type) band);

          if (gsm_band != SYS_BAND_NONE)
          {
            grm_info_p->persistent_denial.denied_band_info[gsm_band].band_registered = TRUE;
          }
        }
      }

    }
    gl1_grm_info_mutex_unlock(gas_id);

  MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_add_trm_band_to_freq_input_type: "
                                      "Add TRM band to freq_info_store for client:%d new_band:%d ",
                      client_id,
                      band);
}
/*===========================================================================

FUNCTION grm_add_sys_band_type_to_trm_freq_input_type

DESCRIPTION
  Adds new freq_info band information to to active list for client if required

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void grm_add_sys_band_type_to_trm_freq_input_type(trm_band_t          band,
                                                  grm_client_enum_t   client_id,
                                                  gl1_sc_activities_t sc_activity,
                                                  gas_id_t gas_id)
{
  trm_freq_input_type *band_info_ptr;

  uint16               i          = 0;
  grm_info_t          *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  sys_band_T gsm_band;

  if ( grm_info_p )
  {
    gl1_grm_info_mutex_lock(gas_id);
    if(sc_activity == SC_IRAT)
    {
      band_info_ptr = &grm_info_p->irat_band_registered;
    }
    else
    {
      band_info_ptr = &grm_info_p->current_band_info;
    }

    /* First valid band entry */
    if ( band_info_ptr->num_bands == 0)
    {
      band_info_ptr->bands[band_info_ptr->num_bands].band = band;

      band_info_ptr->num_bands++;
    }
    else
    {

      if (band_info_ptr->num_bands > MAX_BAND_INFO)
      {
        MSG_GERAN_ERROR_1_G(" num_bands out of bounds %d",band_info_ptr->num_bands );
      }
      else
      {
        /* Scan and see if new band needs adding to pending request */
        for ( i = 0; i < band_info_ptr->num_bands; i++ )
        {
          if ( band == band_info_ptr->bands[i].band ||
               i == INVALID_BAND)
          {
            gl1_grm_info_mutex_unlock(gas_id);
            return;
          }
        }

        /* Bounds check to ensure we are not overwriting */
        if ( i < MAX_BAND_INFO)
        {
          band_info_ptr->bands[i].band = band;
          band_info_ptr->num_bands++;

          if (sc_activity != SC_IRAT )
          {
            gsm_band = gl1_map_sys_band_to_gsm_band( (sys_band_class_e_type) band);
            if (gsm_band != SYS_BAND_NONE)
            {
              grm_info_p->persistent_denial.denied_band_info[gsm_band].band_registered = TRUE;
            }
          }
        }
      }
    }

    gl1_grm_info_mutex_unlock(gas_id);
    MSG_GERAN_MED_3_G( GL1_DRDS_DBG_HDR"Add band to TRM current_band_info for client:%d "
                                        "new_band:%d num_bands:%d",
                        client_id,
                        band,
                        band_info_ptr->num_bands );
  }
  else
  {
    MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"Invalid Client ID %d enable to add band %d "
                                         "to frequency store",
                       client_id, band );
  }

  return;
}

/*===========================================================================

FUNCTION  grm_dual_receive_info_cb

DESCRIPTION
  This function handles the TRM unlock callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the GL1 task queue to action the abort in that context

===========================================================================*/
static void grm_dual_receive_info_cb(
              trm_dual_rec_info_return_data *dual_receive_info_p,
              grm_client_enum_t              requested_client,
              grm_tag_t                      grm_tag )
{
  gas_id_t    gas_id     = gl1_map_client_id_to_gas_id( requested_client );
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( requested_client, gas_id );

  if ( !dual_receive_info_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_dual_receive_info_cb no valid "
                                         "dual_receive_info_p grm_tag:%d",
                         grm_tag );

    return;
  }


  if ( !grm_info_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_dual_receive_info_cb no valid grm_info "
                                         "for requested_client:%d",
                         requested_client );

    return;
  }

  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_3_G( GL1_DRDS_DBG_HDR"grm_dual_receive_info_cb dual_rec_info:%d "
                                        "grm_tag:%d, requested_client:%d",
                        dual_receive_info_p->dual_rec_info, grm_tag, requested_client );
  }


  /*
   * Use the unlock tagging info to determine which GL1 call back we use
   * as any subsequent request will overwrite the grm_tag
   */
  switch ( requested_client )
  {
    /* Only update grm info data for GSM clients */
    case GRM_GSM1:
    case GRM_GSM2:
#ifdef FEATURE_DUAL_SIM
    case GRM_GPRS1:
    case GRM_GPRS2:
#endif /* FEATURE_DUAL_SIM */
#if defined ( FEATURE_TRIPLE_SIM )
    case GRM_GSM3:
#endif /* FEATURE_TRIPLE_SIM */
      grm_info_p->dr_enabled = (TRM_DUAL_RECEIVE_ENABLED == dual_receive_info_p->dual_rec_info);

#ifdef FEATURE_GPRS_GBTA
      if ( grm_info_p->dr_enabled == FALSE )
      {
         if( gl1_gbta_get_nv_gbta_support(gas_id) &&
             GBTA_ALLOWED_CAN_BE_SET(gas_id) &&
             gl1_msg_get_multi_sim_standby_mode()&&
             ( ! OTHER_SUB_RELEASE_PENDING())&&
             OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)
           )
        {
           SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
           MSG_GERAN_HIGH_0_G( "grm_dual_receive_info_cb: DR=0, set GBTA_ALLOWED when the other G is in transfer");
        }
      }


#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
      if( gl1_msg_get_multi_sim_standby_mode() &&
          gpl1_check_if_ccch_in_transfer_supported(OTHER_GAS_ID(gas_id)) &&
         (OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) == TRUE) &&
         (!OTHER_SUB_RELEASE_PENDING()) &&
         (grm_device_config_is_drdsds())
         )
      {
        if(grm_info_p->dr_enabled == FALSE)
        {
          MSG_GERAN_HIGH_0_G( "grm_dual_receive_info_cb:Set GRM_PCHXFER_DR_OFF_PENDING");
          gl1_grm_set_pchxfer_dr_status(GRM_PCHXFER_DR_OFF_PENDING);
          call_in_task_grm_gprs_lock_exchange(GRM_PCHXFER_DR_OFF_PENDING ,OTHER_GAS_ID(gas_id));
        }
        else if(GET_PCHXFER_DR_STATE() != GRM_PCHXFER_DR_ON)
        {
          MSG_GERAN_HIGH_0_G( "grm_dual_receive_info_cb:Set GRM_PCHXFER_DR_ON_PENDING");
          gl1_grm_set_pchxfer_dr_status(GRM_PCHXFER_DR_ON_PENDING);
          call_in_task_grm_gprs_lock_exchange(GRM_PCHXFER_DR_ON_PENDING,OTHER_GAS_ID(gas_id));
        }
      }
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/
#endif /* FEATURE_GPRS_GBTA */

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
      /* Based upon dr enabled status, set hybrid tuneaway state machine*/
      gl1_drdsds_set_hybrid_tuneaway_state(requested_client, gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      MSG_GERAN_HIGH_1_G( "grm_dual_receive_info_cb new DR setting: %d",
                          grm_info_p->dr_enabled );
    break;

    case GRM_GSM1_SECONDARY:
    case GRM_GSM2_SECONDARY:
#if defined ( FEATURE_TRIPLE_SIM )
    case GRM_GSM3_SECONDARY:
#endif /* FEATURE_TRIPLE_SIM */
#ifdef FEATURE_GSM_RX_TX_SPLIT
    case GRM_GSM1_TX:
    case GRM_GSM2_TX:
    case GRM_GPRS1_TX:
    case GRM_GPRS2_TX:
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    break;

    default:
      MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"Unsupported requested_client:%d "
                                           "identifier in grm_dual_receive_info_cb",
                           requested_client );
    break;
  }

  return;
}

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM

/*===========================================================================

FUNCTION  gl1_grm_call_lock_exchange_in_task()

DESCRIPTION
  This function sends the message the L1 TASK which will exchange the TRM clients.
DEPENDENCIES
 
RETURN VALUE
  VOID

SIDE EFFECTS
  None

===========================================================================*/	
void call_in_task_grm_gprs_lock_exchange(grm_pchxfer_dr_state_t dr_status,gas_id_t  gas_id)
{

     /* post a message to the GL1 task to complete the processing */
     grm_gprs_lock_exchnage_t *msg_p;
     grm_gprs_lock_exchnage_t  msg;

     MSG_GERAN_HIGH_0_G( "gl1_grm_call_lock_exchange_in_task");
     msg_p = &msg;

     memset(&msg,NULL,sizeof(grm_gprs_lock_exchnage_t) );

     
     msg_p->dr_status = dr_status;

      /* set the inter-task header */
     msg_p->header.message_set = MS_L1_L1;
     msg_p->header.message_id  = L1_GRM_GPRS_LOCK_EXCHANGE;
     msg_p->gas_id = gas_id;

     PUT_IMH_LEN( sizeof( grm_gprs_lock_exchnage_t ) - sizeof( IMH_T ), msg_p );

     (void)L1_isr_send_message( l1_queues[gas_id], msg_p, FALSE, gas_id );

}
	
#endif /* FEATURE_DUAL_SIM */
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/


/*===========================================================================

FUNCTION  grm_abort_callback

DESCRIPTION
  This function handles the TRM unlock callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the GL1 task queue to action the abort in that context

===========================================================================*/
void grm_abort_callback(
       trm_unlock_callback_data *unlock_callback_data_p )
{
  gas_id_t          gas_id;
  trm_client_enum_t aborted_client;
  grm_client_enum_t grm_client_id = GRM_NO_CLIENT;
  trm_reason_enum_t       reason;
  trm_unlock_event_enum_t event;

  if ( !unlock_callback_data_p )
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"grm_abort_callback no valid unlock_callback_data_p" );
    return;
  }

  aborted_client = unlock_callback_data_p->unlock_client;
  grm_client_id = grm_map_trm_client_id_to_grm_client_id(aborted_client);
  reason         = unlock_callback_data_p->winning_client_info.reason;
  event          = unlock_callback_data_p->event;
  gas_id  = gl1_map_client_id_to_gas_id( grm_client_id );

  switch (event)
  {
    case TRM_UNLOCK_CANCELLED:
      MSG_GERAN_HIGH_1_G( "TRM_UNLOCK_CANCELLED received for abort_client:%d",
                          (int)aborted_client );
      /* nothing further to do */
    break;

    case TRM_UNLOCK_REQUIRED:
    case TRM_UNLOCK_BAND_INCOMPATIBLE:
      MSG_GERAN_HIGH_3_G( "TRM_UNLOCK_REQUIRED/TRM_UNLOCK_BAND_INCOMPATIBLE received for abort_client:%d reason %d event %d",
                          (int)aborted_client, reason ,(int)event);
      if ( !IS_URGENT_ACCESS(reason ) )
      {
        break;
      }

    default:
    {
      /* post a message to the GL1 task to complete the processing */
      gl1_grm_abort_callback_t *abort_cb_msg_p;

      gl1_grm_abort_callback_t  abort_cb_msg;

      abort_cb_msg_p = &abort_cb_msg;

      memset(&abort_cb_msg,NULL,sizeof(gl1_grm_abort_callback_t) );

      MSG_GERAN_HIGH_3_G( GL1_DRDS_DBG_HDR"grm_abort_callback event:%d unlock_by_sclk:%d for abort_client:%d",
                          unlock_callback_data_p->event, unlock_callback_data_p->unlock_by_sclk, aborted_client );

      abort_cb_msg_p->client         = grm_client_id;
      abort_cb_msg_p->event          = event;
      abort_cb_msg_p->unlock_by_sclk = unlock_callback_data_p->unlock_by_sclk;
      abort_cb_msg_p->gas_id         = gas_id;

      /* set the inter-task header */
      abort_cb_msg_p->header.message_set = MS_L1_L1;
      abort_cb_msg_p->header.message_id  = L1_GRM_ABORT_CALLBACK;
      PUT_IMH_LEN( sizeof( gl1_grm_abort_callback_t ) - sizeof( IMH_T ), abort_cb_msg_p );

      (void)L1_isr_send_message( l1_queues[gas_id], abort_cb_msg_p, FALSE, gas_id );
    }
    break;
  }
}

#if defined( FEATURE_G2X_IDLE_TUNEAWAY) || defined(FEATURE_G2X_REORG_TUNEAWAY)
/*===========================================================================

FUNCTION  grm_idle_abort_callback

DESCRIPTION
  This function handles an abort callback in IDLE state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_idle_abort_callback( trm_unlock_callback_data *unlock_data_p )
{
  gas_id_t  gas_id;
   grm_client_enum_t winning_client_id;
  trm_reason_enum_t winning_reason;
  trm_unlock_event_enum_t unlock_event;
  idle_data_T *idle_data_ptr = NULL;

  if ( !unlock_data_p )
  {
    MSG_GERAN_ERROR_0("grm_idle_abort_callback no valid unlock_data_p" );
    return;
  }

  gas_id = gl1_map_client_id_to_gas_id( grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client) );
  gas_id = check_gas_id(gas_id);

  idle_data_ptr = l1_tsk_buffer[gas_id].current_params.L1Data.pIdle_data;
  
  unlock_event      = unlock_data_p->event;
  winning_reason    = unlock_data_p->winning_client_info.reason;
   winning_client_id = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->winning_client_info.client_id);

  MSG_GERAN_HIGH_4_G("grm_idle_abort_callback event:%d winning_client_id:%d winning_reason %d unlock_by_sclk:%d",
                        unlock_data_p->event, winning_client_id, winning_reason, unlock_data_p->unlock_by_sclk);

   /* accept TA for All client with PAGE */
   if ( ( GERAN_TA_SUPPORTED == geran_nv_g2x_ta_supported(winning_client_id))&&
#ifdef FEATURE_G2X_REORG_TUNEAWAY
            (gl1_hw_is_g2x_reorg_ta_enabled()|| gl1_hw_is_g2x_non_drx_ta_enabled() )&&
#endif
#ifdef FEATURE_G2X_IDLE_TUNEAWAY
            (!idle_data_ptr->non_updated_lai_in_nmo1)&&
#endif
          (gl1_is_geran_tuneaway_allowed(gas_id))&&
         
        ( ( ( unlock_event == TRM_UNLOCK_BY) && 
            ( winning_reason == TRM_DEMOD_PAGE ) && 
            ( l1_idle_is_ta_changed( unlock_data_p->unlock_by_sclk, gas_id ) )
           ) || 
           ( unlock_event == TRM_UNLOCK_CANCELLED)
              )
           )
  {
    /* post a message to the GL1 task to complete the processing */
    gl1_grm_abort_callback_t *abort_cb_msg_p;
    gl1_grm_abort_callback_t  abort_cb_msg;

    abort_cb_msg_p = &abort_cb_msg;

    memset(&abort_cb_msg,NULL,sizeof(gl1_grm_abort_callback_t) );

       abort_cb_msg_p->client         = winning_client_id;
    abort_cb_msg_p->event          = unlock_event;
    abort_cb_msg_p->unlock_by_sclk = unlock_data_p->unlock_by_sclk;
    abort_cb_msg_p->gas_id         = gas_id;

    /* set the inter-task header */
    abort_cb_msg_p->header.message_set = MS_L1_L1;
    abort_cb_msg_p->header.message_id  = L1_GRM_IDLE_ABORT_CALLBACK;
    PUT_IMH_LEN( sizeof( gl1_grm_abort_callback_t ) - sizeof( IMH_T ), abort_cb_msg_p );

    MSG_GERAN_LOW_0_G("L1_GRM_IDLE_ABORT_CALLBACK");

    (void)L1_isr_send_message( l1_queues[gas_id], abort_cb_msg_p, FALSE, gas_id );
  }
}
#endif /* (FEATURE_G2X_IDLE_TUNEAWAY) ||(FEATURE_G2X_REORG_TUNEAWAY)*/

/*===========================================================================

FUNCTION  grm_register_async_event_cb

DESCRIPTION
  Called during GRM initialisation to register single asynchronous
  callback handler for TRM interaction.

  Use tag information etc to identify TRM modes.

DEPENDENCIES

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
static void grm_register_async_event_cb(
              uint8                         num_of_clients,
              grm_client_enum_t             *client_ids,
              uint32                        async_events,
              gas_id_t                      gas_id)
{
#ifdef FEATURE_GSM_RX_TX_SPLIT
  int i = 0;
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  trm_async_event_cb_input_type *async_event_cb_input_info_p;

  trm_async_event_cb_input_type  async_event_cb_input_info = { 0 };

  trm_async_operation_enum_type async_operation = TRM_ASYNC_CB_REGISTER;

  async_event_cb_input_info_p = &async_event_cb_input_info;

  if (client_ids)
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (num_of_clients > 1)
    {
      /*Grouped registration*/
      async_operation = TRM_ASYNC_GROUPED_CB_REGISTER;

      i = 0;
      async_event_cb_input_info_p->client_id = grm_map_grm_client_id_to_trm_client_id(*(client_ids + i));

      async_event_cb_input_info_p->data.grouped_reg_info.num_of_clients = num_of_clients;

      for (i = 0; (i < num_of_clients) && (i < TRM_GROUPED_CLIENTS_MAX); i++)
      {
        async_event_cb_input_info_p->data.grouped_reg_info.client_ids[i] =
                grm_map_grm_client_id_to_trm_client_id(*(client_ids + i));

      }

      async_event_cb_input_info_p->data.grouped_reg_info.callback_ptr =
                grm_async_grouped_cb_event_handler;

      async_event_cb_input_info_p->data.grouped_reg_info.events_bitmask = async_events;
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
      async_event_cb_input_info_p->client_id =
                    grm_map_grm_client_id_to_trm_client_id(*client_ids);

      async_event_cb_input_info_p->data.reg_info.callback_ptr =
                    grm_async_cb_event_handler;

      async_event_cb_input_info_p->data.reg_info.events_bitmask = async_events;

    }

    async_event_cb_input_info_p->operation = async_operation;

  trm_handle_async_event_cb( async_event_cb_input_info_p );
  }
  else
  {
    MSG_GERAN_ERROR_0_G("Null Client Id Information");
  }

  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_LOW_2_G( GL1_DRDS_DBG_HDR"grm_register_async_event_cb "
                                        "num_of_clients %d async_events:0x%x ",
                        num_of_clients, async_events );
  }

  return;
}

/*===========================================================================

FUNCTION  grm_gl1_get_current_trm_reason

DESCRIPTION
  Get the current TRM reason used by GL1.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
trm_reason_enum_t grm_gl1_get_current_trm_reason(grm_client_enum_t client_id,
                                                 gas_id_t gas_id)
{
  grm_info_t        *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_reason_enum_t reason = TRM_NUM_REASONS;

  if(grm_info_p)
  {
    MSG_GERAN_LOW_2_G("grm_gl1_get_current_trm_reason: %d for G client %d",
                        grm_info_p->reason,
                        client_id);
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (grm_is_a_tx_client(client_id))
    {
      if (grm_info_p->tx_info_p)
      {
        reason = grm_info_p->tx_info_p->tx_reason;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
      }
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
    reason = grm_info_p->reason;
   }
   }
   else
   {
    MSG_GERAN_ERROR_1_G("grm_gl1_get_current_trm_reason: Invalid gas_id %d", gas_id);
   }
   return reason;
}

/*===========================================================================

FUNCTION  grm_get_reason

DESCRIPTION
  Get the requested TRM reason info for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
trm_reason_enum_t grm_get_reason(grm_client_enum_t requested_client_id,
                                 gas_id_t          gas_id )
{
  /* Default out of range value */
  trm_reason_enum_t  grm_reason = TRM_NUM_REASONS;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(requested_client_id, gas_id);

  if ( grm_info_p )
  {
    if ( grm_get_info( requested_client_id, NULL, GRM_GET_REASON, gas_id ) )
    {
      grm_reason = grm_info_p->grm_get_info_data.info.client_info.reason;

      if ( gl1_drds_debug( gas_id ) )
      {
        MSG_GERAN_LOW_2_G( GL1_DRDS_DBG_HDR"grm_get_reason:%d for client:%d",
                            grm_reason, requested_client_id );
      }
    }
  }

  return ( grm_reason );
}
#ifdef FEATURE_PBR_RANDOMIZATION	
/*===========================================================================

FUNCTION  grm_get_wakeup_identifier

DESCRIPTION
  Get the last indicated wakeup identifier to TRM

DEPENDENCIES
  None

RETURN VALUE
  Frame Number

SIDE EFFECTS
  None

===========================================================================*/
uint32 grm_get_wakeup_identifier(grm_client_enum_t requested_client_id,
                                 gas_id_t          gas_id )
{
  /* Default out of range value */
  uint32  wakeup_identifier = NOT_A_FRAME_NUMBER;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(requested_client_id, gas_id);

  if ( grm_info_p )
  {
    wakeup_identifier = grm_info_p->start_frame;
  }
  
  return ( wakeup_identifier );
}

#endif
/*===========================================================================

FUNCTION  grm_get_dr_capability

DESCRIPTION
  Get the requested DR Capability TRM info for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
trm_dual_receive_enum_type grm_get_dr_capability(grm_client_enum_t client_id,
                                                 gas_id_t          gas_id )
{
  trm_dual_receive_enum_type  grm_dr_capability = TRM_DUAL_RECEIVE_MAX;
  grm_info_t                 *grm_info_p        =
                                grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p && grm_get_info( client_id, NULL, GRM_GET_DR_CAPABILITY, gas_id ) )
  {
    grm_dr_capability = grm_info_p->grm_get_info_data.info.dual_rec_info.dual_rec_info;

    if ( gl1_drds_debug( gas_id ) )
    {
      MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_get_dr_capability:%d for client_id:%d",
                          grm_dr_capability, client_id );
    }
  }

  return ( grm_dr_capability );
}

/*===========================================================================

FUNCTION  grm_update_aps_support

DESCRIPTION
  Enable/disable Adaptive Page Skipping support

DEPENDENCIES
  Requires API support for APS toggle from TRM

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_update_aps_support(const boolean           aps_state,
                            gas_id_t                gas_id)
{

  trm_set_client_state_input_type   client_state_info;
  trm_set_client_state_output_data  trm_return_data;

  if ( grm_current_aps_state[gas_id] == aps_state )
  {
    /* We don't need to update TRM if there is no change of aps_state. GRR would
       call this function in every MPH_BLOCK_QUALITY_IND. Don't need to update TRM
       so frequently
    */
    return;
  }

  grm_current_aps_state[gas_id] = aps_state;
  MSG_GERAN_HIGH_1_G( "APS gl1 update TRM about aps state:%d", aps_state);

  client_state_info.client_id = grm_map_grm_client_id_to_trm_client_id( gl1_map_gas_id_to_client_id( gas_id ) );
  client_state_info.client_state = TRM_APS;

  client_state_info.data.aps_data.aps_type = TRM_APS_SUPPORTED;

  if (aps_state)
  {
    client_state_info.data.aps_data.operation = TRM_START_STATE;
  }
  else
  {
    client_state_info.data.aps_data.operation = TRM_END_STATE;
  }

  trm_set_client_state(&client_state_info,&trm_return_data);

  return;
}

/*===========================================================================

FUNCTION grm_register_aps_info

DESCRIPTION
  This function indicates to TRM which reasons to use APS (adaptive page
  skipping)

DEPENDENCIES
  Requires the l1_tsk_buffer[gas_id].client_id to be valid before call

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_register_aps_info(grm_client_enum_t client_id, gas_id_t gas_id )
{
  trm_register_feature_input_info aps_register_input_info;

  aps_register_input_info.client_id = grm_map_grm_client_id_to_trm_client_id(client_id);
  aps_register_input_info.feature_info = TRM_FEATURE_APS;
  aps_register_input_info.register_info.new_aps_info.reason_mask =
      (uint64)( 1 << TRM_DEMOD_PAGE );

  trm_register_feature( &aps_register_input_info );

  MSG_GERAN_HIGH_1_G( "APS gl1_register_aps_reasons client:%d",
                      client_id );

  /* Initialize aps_state as FALSE */
  grm_update_aps_support( FALSE, gas_id );
}

/*===========================================================================

FUNCTION  grm_change_duration/grm_extend_duration

DESCRIPTION
  Set the duration info for a particular DR-DSDS client.
  Only ever gives back denied/granted immediately.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_change_duration(grm_client_enum_t client_id,
                            trm_duration_t    duration,
                            gas_id_t          gas_id )
{
  boolean     return_result = FALSE;
  grm_info_t *grm_info_p    = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_modify_data *grm_modify_duration_data_p = NULL;
  trm_modify_chain_state_return_data *grm_modify_state_info_p = NULL;

  if ( grm_info_p )
  {
    grm_modify_duration_data_p = grm_get_modify_chain_state_data(client_id, gas_id);

    if (grm_modify_duration_data_p)
    {
    grm_modify_duration_data_p->modify_duration.duration = duration;

    grm_modify_chain_state( client_id, GRM_MODIFY_DURATION, gas_id );

      grm_modify_state_info_p = grm_get_modify_state_info(client_id,gas_id);

      if (grm_modify_state_info_p)
      {
    /* Only ever gives back denied/granted immediately */
        if ( TRM_GRANTED == grm_modify_state_info_p->modify_info.mod_duration_info.result )
    {
      return_result = TRUE;
#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
          if (els_active &&  !grm_is_a_secondary_client (client_id) )
          {
            grm_els_state_update_aperiodic(grm_info_p->reason,
                                           grm_info_p->sub_reason,
                                           duration,
                                           client_id,
                                           gas_id);
          }
          else
          {
            MSG_GERAN_HIGH_0_G("!els_active");
          }
#endif /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */
    }
  }
  else
  {
         MSG_GERAN_ERROR_1_G("Null grm_modify_state_info_p! Invalid client_id %d", client_id );
  }
 }
 else
 {
      MSG_GERAN_ERROR_1_G("Null grm_modify_duration_data_p! Invalid client_id %d", client_id );
 }
}
  return ( return_result );
}

/*===========================================================================

FUNCTION  grm_extend_duration

DESCRIPTION


DEPENDENCIES


RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
boolean grm_extend_duration(grm_client_enum_t client_id,
                            trm_duration_t    duration,
                            gas_id_t          gas_id )
{
  return ( grm_change_duration( client_id, duration, gas_id ) );
}

/*===========================================================================

FUNCTION  grm_async_cb_event_handler

DESCRIPTION
  Async call-back to communicate from TRM to L1 on any async operation
  requested by the L1s

DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
void grm_async_cb_event_handler(
       trm_async_callback_information_type *grm_async_cb_event_data_p )
{
  gas_id_t                       gas_id;
  grm_tag_t                      grm_tag;
  trm_client_enum_t              async_cb_client;
  trm_async_callback_type_enum_t async_cb_type;
  grm_client_enum_t              grm_client_id = GRM_NO_CLIENT;

  if ( !grm_async_cb_event_data_p )
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"grm_async_cb_event_handler no valid "
                                       "grm_async_cb_event_data_p" );
    return;
  }

  async_cb_client = grm_async_cb_event_data_p->client;
  grm_client_id = grm_map_trm_client_id_to_grm_client_id(async_cb_client);
  gas_id          = gl1_map_client_id_to_gas_id( grm_client_id );
  grm_tag         = (grm_tag_t)grm_async_cb_event_data_p->tag;
  async_cb_type   = grm_async_cb_event_data_p->cb_type;

  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_3_G( GL1_DRDS_DBG_HDR"grm_async_cb_event_handler cb_type:%d grm_tag:%d client_id:%d",
                        async_cb_type, grm_tag, async_cb_client );
  }

  switch ( async_cb_type )
  {
    /* Req and Notify Return Information */
    case TRM_REQUEST_AND_NOTIFY_ASYNC_INFO:
      grm_request_and_notify_async_info_cb(
        (trm_req_n_not_return_data *)&grm_async_cb_event_data_p->data,
        grm_client_id,
        grm_tag );
    break;

    /* Enh Req and Notify Return Information */
    case TRM_REQUEST_AND_NOTIFY_ENH_ASYNC_INFO:
      grm_request_and_notify_enh_async_info_cb(
        (trm_req_n_not_enh_return_data *)&grm_async_cb_event_data_p->data,
        grm_client_id,
        grm_tag );
    break;

    /* Modify Chain State Return Information */
    case TRM_MODIFY_CHAIN_STATE_ASYNC_INFO:
      grm_modify_chain_state_async_info_cb(
        (trm_modify_chain_state_return_data *)&grm_async_cb_event_data_p->data,
        grm_client_id,
        grm_tag );
    break;

    /* Unlock Call-back Return Information */
    case TRM_UNLOCK_CHAIN_ASYNC_INFO:
      grm_unlock_chain_async_info_cb(
        (trm_unlock_callback_data *)&grm_async_cb_event_data_p->data,
        grm_client_id,
        grm_tag );
    break;

    case TRM_DUAL_RECEIVE_INFO:
      grm_dual_receive_info_cb(
        (trm_dual_rec_info_return_data *)&grm_async_cb_event_data_p->data,
        grm_client_id,
        grm_tag );
    break;

    case TRM_CONNECTED_MODE_HOP_ASYNC_INFO:
#ifdef FEATURE_GSM_WTR_HOP
      grm_connected_mode_hop_async_info_cb(
        (trm_connected_mode_hop_callback_data *)&grm_async_cb_event_data_p->data,
        grm_client_id );
    break;
#endif /* FEATURE_GSM_WTR_HOP */

    case TRM_QTA_ASYNC_INFO:
      grm_qta_async_info_cb(
        (trm_qta_event_callback_data *)&grm_async_cb_event_data_p->data,
        async_cb_client,
        grm_tag );
    break;
#if defined ( FEATURE_DUAL_SIM ) && defined ( FEATURE_CRAT_MSIM_TEST_FRAMEWORK )

    case TRM_ELS_STATE_ASYNC_INFO:
      grm_els_async_info_cb(
         (trm_els_state_callback_data *)&grm_async_cb_event_data_p->data,
         async_cb_client
         );
      break;

#endif /* FEATURE_DUAL_SIM && FEATURE_CRAT_MSIM_TEST_FRAMEWORK */    

case TRM_IDLE_COLL_ASYNC_INFO:

 MSG_GERAN_HIGH_3_G("grm_async_cb_event_handler cb_type:%d grm_tag:%d client_id:%d",
                        async_cb_type, grm_tag, async_cb_client );
      grm_coll_async_info_cb(
        (trm_idle_coll_event_data *) &grm_async_cb_event_data_p->data,
         async_cb_client,
         grm_tag 
         );  
    break;

    default:
       MSG_GERAN_ERROR_3_G( GL1_DRDS_DBG_HDR"Unhandled TRM Async CB type:%d tag:%d for client:%d",
                            async_cb_type,
                            grm_tag,
                            async_cb_client );
    break;
  }
}

/*===========================================================================

FUNCTION  grm_request_and_notify_async_info_cb

DESCRIPTION
  This function handles the TRM unlock callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
static void grm_request_and_notify_async_info_cb(trm_req_n_not_return_data *ran_info_p,
                                                 grm_client_enum_t          granted_client,
                                                 grm_tag_t                  grm_tag )
{
  gas_id_t    gas_id     = gl1_map_client_id_to_gas_id( granted_client );
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( granted_client, gas_id );

  if ( !ran_info_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_request_and_notify_async_info_cb no valid ran_info_p grm_tag:%d",
                         grm_tag );

    return;
  }

  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_4_G( GL1_DRDS_DBG_HDR"grm_request_and_notify_async_info_cb grant:%d num_bands:%d grm_tag:%d, granted_client:%d",
                        ran_info_p->grant, ran_info_p->num_bands, grm_tag, granted_client );
  }

  if ( !grm_info_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_request_and_notify_async_info_cb no valid grm_info for granted_client:%d",
                         granted_client );

    return;
  }

  /* Use the grm tagging info to determine which GL1 call back we use */
  switch ( grm_tag )
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
     case GRM_RAN_MODIFY_TO_TRAFFIC_REASON:
        grm_modify_to_traffic_reason_callback( granted_client,
                             ran_info_p->grant );
#endif /* FEATURE_GSM_RX_TX_SPLIT */
    case GRM_RAN_ACCESS_RETRY:
      grm_access_callback( granted_client,
                           ran_info_p->grant );
    break;

    default:
      MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"Unsupported grm_tag:%d identifier in grm_request_and_notify_async_info_cb",
                           grm_tag );
    break;
  }

  return;
}

/*===========================================================================

FUNCTION  grm_request_and_notify_enh_async_info_cb

DESCRIPTION
  This function handles the TRM unlock callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the GL1 task queue to action the abort in that context

===========================================================================*/
static void grm_request_and_notify_enh_async_info_cb(
              trm_req_n_not_enh_return_data *rane_info_p,
              grm_client_enum_t              granted_client,
              grm_tag_t                      grm_tag )
{
  gas_id_t    gas_id     = gl1_map_client_id_to_gas_id( granted_client );
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( granted_client, gas_id );

  if ( !rane_info_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_request_and_notify_enh_async_info_cb no "
                                         "valid rane_info_p grm_tag:%d",
                         grm_tag );

    return;
  }

  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_5_G( GL1_DRDS_DBG_HDR"grm_request_and_notify_enh_async_info_cb grant:%d "
                                        "num_bands:%d grm_tag:%d unlock_tag:%d, granted_client:%d",
                        rane_info_p->grant,
                        rane_info_p->num_bands,
                        grm_tag, rane_info_p->unlock_tag,granted_client  );
  }

  if ( !grm_info_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_request_and_notify_enh_async_info_cb no "
                                         "valid grm_info for granted_client:%d",
                         granted_client );

    return;
  }

  /*
   * Use the unlock tagging info to determine which GL1 call back we use
   * as any subsequent request will overwrite the grm_tag
   */
  switch ( (grm_tag_t)rane_info_p->unlock_tag )
  {
    case GRM_RANE_DENIED:
    case GRM_RANE_ACQUISITION:
    case GRM_RANE_RESELECTION:
    case GRM_RANE_GPRS_PRIORITY_IND:
      grm_enh_grant_callback( granted_client,
                              rane_info_p->grant,
                              rane_info_p->duration );
    break;

    case GRM_RANE_PANIC_RESET:
      grm_enh_grant_panic_reset_callback( granted_client,
                                          rane_info_p->grant,
                                          rane_info_p->duration );
    break;

    case GRM_RANE_TBF_SUSPEND:
      grm_tbf_enh_grant_callback( granted_client,
                                  rane_info_p->grant,
                                  rane_info_p->duration );
    break;

    default:
      MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"Unsupported grm_tag:%d identifier in "
                                           "grm_request_and_notify_enh_async_info_cb",
                           grm_tag );
    break;
  }
  return;
}
#ifdef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  grm_modify_to_traffic_reason_callback

DESCRIPTION
  This function handles the TRM grant callback for GSM Tx client.
  It posts a message to the GSM Layer 1 task to complete the processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void grm_modify_to_traffic_reason_callback(grm_client_enum_t          granted_client,
                            trm_grant_return_enum_type grant_status )
{
  gas_id_t    gas_id     = gl1_map_client_id_to_gas_id( granted_client );
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( granted_client, gas_id );

  if ( grm_info_p )
  {
    gl1_grm_grant_callback_t *grant_cb_msg_p;
    gl1_grm_grant_callback_t  grant_cb_msg;
    grant_cb_msg_p = &grant_cb_msg;

    memset(&grant_cb_msg,NULL,sizeof(gl1_grm_grant_callback_t));

    MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_enh_grant_callback grant_status:%d duration:%d for granted_client:%d",
                        grant_status, granted_client );

    grm_info_p->req_n_notify_duration = INVALID_SCLKS;

    grant_cb_msg_p->client         = granted_client;
    grant_cb_msg_p->grant_status   = grant_status;
    grant_cb_msg_p->duration       = GRM_ACCESS_DURATION_SCLKS;
    grant_cb_msg_p->gas_id         = gas_id;

    /* set the inter-task header */
    grant_cb_msg_p->header.message_set = MS_L1_L1;
    grant_cb_msg_p->header.message_id  = L1_GRM_GRANT_MODIFY_TO_TRAFFIC_CALLBACK;
    PUT_IMH_LEN( sizeof( gl1_grm_grant_callback_t ) - sizeof( IMH_T ), grant_cb_msg_p );

    (void)L1_isr_send_message( l1_queues[gas_id], grant_cb_msg_p, FALSE, gas_id );
  }
  else
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_enh_grant_callback no valid grm_info for granted_client:%d",
                         granted_client );
  }
}
#endif /* FEATURE_GSM_RX_TX_SPLIT */
/*===========================================================================

FUNCTION  grm_enh_grant_callback

DESCRIPTION
  This function handles the TRM grant callback for GSM clients.
  It posts a message to the GSM Layer 1 task to complete the processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_enh_grant_callback(grm_client_enum_t          granted_client,
                            trm_grant_return_enum_type grant_status,
                            trm_duration_t             duration )
{
  gas_id_t    gas_id     = gl1_map_client_id_to_gas_id( granted_client );
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( granted_client, gas_id );

  if ( grm_info_p )
  {
    gl1_grm_grant_callback_t *grant_cb_msg_p;
    gl1_grm_grant_callback_t  grant_cb_msg;
    grant_cb_msg_p = &grant_cb_msg;

    memset(&grant_cb_msg,NULL,sizeof(gl1_grm_grant_callback_t));

    MSG_GERAN_HIGH_3_G( GL1_DRDS_DBG_HDR"grm_enh_grant_callback grant_status:%d duration:%d for granted_client:%d",
                        grant_status, duration, granted_client );

    grm_info_p->req_n_notify_duration = INVALID_SCLKS;

    grant_cb_msg_p->client       = granted_client;
    grant_cb_msg_p->grant_status = grant_status;
    grant_cb_msg_p->duration     = duration;
    grant_cb_msg_p->gas_id       = gas_id;

    /* set the inter-task header */
    grant_cb_msg_p->header.message_set = MS_L1_L1;
    grant_cb_msg_p->header.message_id  = L1_GRM_GRANT_CALLBACK;
    PUT_IMH_LEN( sizeof( gl1_grm_grant_callback_t ) - sizeof( IMH_T ), grant_cb_msg_p );

    (void)L1_isr_send_message( l1_queues[gas_id], grant_cb_msg_p, FALSE, gas_id );
  }
  else
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_enh_grant_callback no valid grm_info for granted_client:%d",
                         granted_client );
  }
}
/*===========================================================================

FUNCTION  grm_access_callback

DESCRIPTION


DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS


===========================================================================*/
static void grm_access_callback(grm_client_enum_t          client,
                                trm_grant_return_enum_type grant_status )
{
  /* just call the enhanced callback with the fixed access duration */
  grm_enh_grant_callback( client, grant_status, GRM_ACCESS_DURATION_SCLKS );

  return;
}

/*===========================================================================

FUNCTION  grm_modify_chain_state_async_info_cb

DESCRIPTION
  This function handles the TRM chain state info callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the GL1 task queue to action the abort in that context

===========================================================================*/
static void grm_modify_chain_state_async_info_cb(
              trm_modify_chain_state_return_data *modify_chain_state_info_p,
              grm_client_enum_t                   client_id,
              grm_tag_t                           grm_tag )
{
  volatile ISRTIM_CMD_BLK    *l1_tskisr_blk;

  trm_modify_state_enum_t    trm_modify_chain_state_type;
  trm_grant_return_enum_type trm_modify_chain_state_result;

  boolean     valid_trm_modify_state = TRUE;
  gas_id_t    gas_id                 = gl1_map_client_id_to_gas_id( client_id );
  grm_info_t *grm_info_p             = grm_get_info_ptr_from_client( client_id, gas_id );

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if ( !modify_chain_state_info_p )
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_modify_chain_state_async_info_cb no "
                                         "valid modify_chain_state_info_p grm_tag:%d",
                         grm_tag );

    return;
  }
  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_modify_chain_state_async_info_cb "
                                        "grm_tag:%d client_id:%d",
                        					grm_tag, client_id  );
  }

  trm_modify_chain_state_type = modify_chain_state_info_p->modify_type;

  switch ( trm_modify_chain_state_type )
  {
    case TRM_MODIFY_BAND:
      trm_modify_chain_state_result =
        modify_chain_state_info_p->modify_info.mod_band_info.result;

      MSG_GERAN_HIGH_3_G( "TRM_MODIFY_BAND: cb received for client_id:%d result:%d band:%d",
                          client_id,
                          trm_modify_chain_state_result,
                          gl1_map_sys_band_to_gsm_band(
                            modify_chain_state_info_p->modify_info.mod_band_info.freq_info.bands[0].band ) );
    break;

    case TRM_MODIFY_DURATION:
      trm_modify_chain_state_result =
        modify_chain_state_info_p->modify_info.mod_duration_info.result;

      MSG_GERAN_HIGH_2_G( "TRM_MODIFY_DURATION: cb received for client_id:%d result:%d",
                          client_id,
                          trm_modify_chain_state_result );
    break;

    case TRM_MODIFY_REASON:
      trm_modify_chain_state_result =
        modify_chain_state_info_p->modify_info.mod_reason_info.result;

      MSG_GERAN_HIGH_2_G( "TRM_MODIFY_REASON: cb received for client_id:%d result:%d",
                          client_id,
                          trm_modify_chain_state_result );

#ifdef FEATURE_GSM_RX_TX_SPLIT
      if( grm_info_p && 
          ( trm_modify_chain_state_result == TRM_DENIAL ) && 
          ( grm_gl1_get_current_trm_reason(client_id, gas_id) == TRM_ACCESS ) &&
          ( modify_chain_state_info_p->modify_info.mod_reason_info.reason_requested == TRM_TRAFFIC )
        )
      {
        /* chain reason to TRAFFIC from ACCESS has been denied
           When call establishment is done and GL1 is ready to do change reason from TRM_ACCESS to TRM_TRAFFIC,
           GL1 puts a req&not for Tx client with TRM_TRAFFIC.
           TRM_TRAFFIC has the highest priority so TRM will unlock X tech
           During this time GL1 does not own Tx device. However TRM won’t grant it to anyone else since GL1 has requested
           it with the highest priority. GL1 could continue its voice call with Rx and Tx.
           When unlock is done, TRM grants Tx to GL1.
           GL1 changes reason from TRM_ACCESS to TRM_TRAFFIC for PRX and DRX pops client.
         */

        /* Do not alter the GL1 local states of trm grant */
        valid_trm_modify_state = FALSE;
      }
      else
#endif /* FEATURE_GSM_RX_TX_SPLIT */
      if(trm_modify_chain_state_result == TRM_GRANTED &&
        grm_info_p)
      {
        grm_gl1_set_current_trm_reason(modify_chain_state_info_p->modify_info.mod_reason_info.reason_requested,
                                       grm_info_p->sub_reason,
                                       client_id,
                                       gas_id);
      }

    break;

    default:
    {
      valid_trm_modify_state = FALSE;

      MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"Unsupported modify_type:%d identifier in grm_modify_chain_state_async_info_cb",
                           modify_chain_state_info_p->modify_type );
    }
    break;
  }

  if ( grm_info_p && valid_trm_modify_state )
  {
    switch ( trm_modify_chain_state_result )
    {
      /* When TRM_PENDING is no longer supported just remove this case handling */
      case TRM_PENDING:
        /* Copy info to local structure so we handle Pending response correctly*/
        grm_set_modify_state_info(modify_chain_state_info_p, client_id, gas_id);

        /* Update TRM grant status as if denied here */
        grm_set_trm_grant_status( TRM_DENIAL, client_id, gas_id);
      break;

      /* Either valid responses so data can be copied immediately to grm store */
      case TRM_DENIAL:
      case TRM_GRANTED:
        /* Copy info to local structure */
        grm_set_modify_state_info(modify_chain_state_info_p, client_id, gas_id);

        /*Update TRM grant status*/
        grm_set_trm_grant_status(trm_modify_chain_state_result, client_id, gas_id);
      break;

      default:

        MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_modify_chain_state_async_info_cb unsupported result:%d",
                             trm_modify_chain_state_result );
      break;
    }

    if ( gl1_drds_debug( gas_id ) )
    {
      MSG_GERAN_LOW_1_G( GL1_DRDS_DBG_HDR"grm_modify_chain grm_tag:%d",
                          grm_tag );
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_unlock_chain_async_info_cb

DESCRIPTION
  This function handles the TRM unlock callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the GL1 task queue to action the abort in that context

===========================================================================*/
static void grm_unlock_chain_async_info_cb(
              trm_unlock_callback_data *unlock_info_p,
              grm_client_enum_t         client,
              grm_tag_t                 grm_tag )
{
  gas_id_t    gas_id;
  grm_info_t *grm_info_p = NULL;

  if ( !unlock_info_p )
  {
    MSG_ERROR( GL1_DRDS_DBG_HDR"grm_unlock_chain_async_info_cb no valid unlock_info_p grm_tag:%d",
                         grm_tag,0,0 );
    return;
  }

  gas_id = gl1_map_client_id_to_gas_id(
     grm_map_trm_client_id_to_grm_client_id(unlock_info_p->unlock_client) );
  grm_info_p = grm_get_info_ptr_from_client( client, gas_id );

  if ( !grm_info_p )
  {
    MSG_ERROR( GL1_DRDS_DBG_HDR"grm_unlock_chain_async_info_cb no valid grm_info for "
                               "unlock_client:%d grm_tag:%d unlock_tag:%d",
               unlock_info_p->unlock_client,
               grm_tag,
               unlock_info_p->unlock_tag );

    return;
  }

#ifdef FEATURE_GSM_RX_TX_SPLIT
  if (grm_is_a_tx_client(client))
  {
    /*Ignore in case of Tx. Rx unlock will handle Rx and TX.*/
    MSG_GERAN_HIGH_5_G( GL1_DRDS_DBG_HDR"grm_unlock_chain_async_info_cb event:%d "
                                        "client:%d for unlock_client:%d "
                                        "grm_tag:%d unlock_tag:%d",
                        unlock_info_p->event, client,
                        unlock_info_p->unlock_client, grm_tag, unlock_info_p->unlock_tag );
    return;
  }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

  if (grm_is_a_secondary_client(client))
  {
    MSG_GERAN_HIGH_5_G( GL1_DRDS_DBG_HDR"grm_unlock_chain_async_info_cb for DRX event:%d "
                                        "client:%d for unlock_client:%d "
                                        "grm_tag:%d unlock_tag:%d",
                        unlock_info_p->event, client,
                        unlock_info_p->unlock_client, grm_tag, unlock_info_p->unlock_tag );  

#ifdef FEATURE_TABASCO_MODEM
    MSG_GERAN_HIGH_0_G("Ignore unlock cb for secondary chain");
    return;
#endif

  }

  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_5_G( GL1_DRDS_DBG_HDR"grm_unlock_chain_async_info_cb event:%d "
                                        "unlock_by_sclk:%d for unlock_client:%d "
                                        "grm_tag:%d unlock_tag:%d",
                        unlock_info_p->event, unlock_info_p->unlock_by_sclk,
                        unlock_info_p->unlock_client, grm_tag, unlock_info_p->unlock_tag );
  }

  /*
   * Use the unlock tagging info to determine which GL1 call back we use
   * as any subsequent request will overwrite the grm_tag
   */
  switch ( (grm_tag_t)unlock_info_p->unlock_tag )
  {
    case GRM_RANE_DENIED:
    case GRM_RANE_ACQUISITION:
    case GRM_RANE_RESELECTION:
    case GRM_RANE_GPRS_PRIORITY_IND:
    case GRM_RANE_PANIC_RESET:
      /* Copy over data to grm_info store */
      grm_info_p->grm_unlock_cb_data = *unlock_info_p;

      grm_abort_callback( &grm_info_p->grm_unlock_cb_data);
    break;

    case GRM_RAN_ACCESS_RETRY:
    case GRM_RANE_TBF_SUSPEND:
      /* Nothing here to do maybe call default */
    break;

    case GRM_RETAIN_LOCK:
      /* Copy over data to grm_info store */
      grm_info_p->grm_unlock_cb_data = *unlock_info_p;

      if ( gl1_drds_debug( gas_id ) )
      {
        MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_unlock_chain_async_info_cb "
                                            "GRM_RETAIN_LOCK for client:%d unlock_client:%d",
                            client, grm_info_p->grm_unlock_cb_data.unlock_client );
      }

      if ((int8) client == (int8)grm_info_p->grm_unlock_cb_data.unlock_client )
      {
        /* If valid registered callback function exists then execute here */
        if ( grm_info_p->grm_unlock_callback )
        {
          grm_info_p->grm_unlock_callback( &grm_info_p->grm_unlock_cb_data );
        }
      }
    break;

    default:
      MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"Unsupported unlock_tag %d identifier in "
                                           "grm_unlock_chain_async_info_cb",
                           unlock_info_p->unlock_tag );
    break;
  }

  return;
}

#ifdef FEATURE_GSM_WTR_HOP
/*===========================================================================

FUNCTION  grm_connected_mode_hop_async_info_cb

DESCRIPTION
  This function handles the TRM WTR Hop request for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Starts the WTR Hop state machine.

===========================================================================*/
static void grm_connected_mode_hop_async_info_cb(
              trm_connected_mode_hop_callback_data *connected_mode_hop_info_p,
              grm_client_enum_t                     client_id )
{
  trm_conn_mode_hop_enum_type conn_mode_hop_info_type;

  gas_id_t    gas_id                 = gl1_map_client_id_to_gas_id( client_id );
  grm_info_t *grm_info_p             = grm_get_info_ptr_from_client( client_id, gas_id );

  if ( !connected_mode_hop_info_p )
  {
    MSG_GERAN_ERROR_0_G( GL1_DRDS_DBG_HDR"grm_connected_mode_hop_async_info_cb no "
                                         "valid connected_mode_hop_info_p" );
    return;
  }

  conn_mode_hop_info_type = connected_mode_hop_info_p->hop_info;

  if ( !grm_info_p )
  {
    MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"grm_connected_mode_hop_async_info_cb "
                                         "no valid grm_info for client_id:%d comm_mode_hop_type:%d",
                         client_id, conn_mode_hop_info_type );

    return;
  }
  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_1_G( GL1_DRDS_DBG_HDR"grm_connected_mode_hop_async_info_cb client_id:%d", client_id );
  }
  switch ( conn_mode_hop_info_type )
  {
    case TRM_CONN_MODE_HOP_DEVICE_HOP_REQ:
    {
      wtr_hop_start_result_t wtr_hop_start_status;

      MSG_GERAN_HIGH_3_G( "TRM_CONN_MODE_HOP_DEVICE_HOP_REQ: cb received for client_id:%d "
                          "from_device:%d to_device:%d",
                          client_id,
                          connected_mode_hop_info_p->rf_device_hop_from,
                          connected_mode_hop_info_p->rf_device_hop_to
                         );

      wtr_hop_start_status =
        gl1_msg_hop_start( connected_mode_hop_info_p->rf_device_hop_to, gas_id );

      /* Call the connected mode HOP callback here */
      if ( WTR_HOP_START_SUCCESS != wtr_hop_start_status )
      {
        MSG_GERAN_HIGH_3_G( "TRM_CONN_MODE_HOP_DEVICE_HOP_REQ: cb received for client_id:%d "
                            "start_failed_status:%d to_device:%d",
                            client_id,
                            wtr_hop_start_status,
                            connected_mode_hop_info_p->rf_device_hop_to
                           );
      }
    }
    break;

    default:
    {
      MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"Unsupported conn_mode_hop_info_type:%d identifier in "
                                           "grm_connected_mode_hop_async_info_cb",
                           conn_mode_hop_info_type );
    }
    break;
  }

  return;
}

/*===========================================================================

FUNCTION  grm_send_wtr_hop_async_rsp

DESCRIPTION
  This function handles the TRM WTR Hop response for any GSM client.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  None.

===========================================================================*/
void grm_send_wtr_hop_async_rsp(boolean  wtr_hop_success,
                                gas_id_t gas_id )
{
  trm_async_event_response_data_type *resp_data_p;
  trm_async_event_response_data_type  resp_data = { 0 };
  volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  resp_data_p = &resp_data;

  resp_data_p->client_id = grm_map_grm_client_id_to_trm_client_id(l1_tskisr_blk->client_id);
  resp_data_p->event     = TRM_CONNECTED_MODE_HOP_ASYNC_INFO;

  if ( wtr_hop_success )
  {
    resp_data_p->data.dev_hop_req.result = TRM_ASYNC_EVENT_RESPONSE_SUCCESS;
  }
  else
  {
    resp_data_p->data.dev_hop_req.result = TRM_ASYNC_EVENT_RESPONSE_FAIL_NOT_READY;
  }

  MSG_GERAN_HIGH_3_G( "Reporting WTR hop status:%d result to TRM:%d for client_id:%d",
                      wtr_hop_success, resp_data_p->data.dev_hop_req.result,
                      resp_data_p->client_id );

  trm_async_event_response( &resp_data );
}

#endif /* FEATURE_GSM_WTR_HOP */

/*===========================================================================

FUNCTION  gpl1_set_grm_qta_state
DESCRIPTION
  Sets grm_qta_state, to indicate whether startup or cleanup

===========================================================================*/
static void gpl1_set_grm_qta_state(boolean qta_state, gas_id_t gas_id)
{
    grm_qta_state[gas_id] = qta_state;
}
/*===========================================================================

FUNCTION  gpl1_get_grm_qta_state
DESCRIPTION
  Returns grm_qta_state, to indicate whether startup or cleanup

===========================================================================*/
boolean gpl1_get_grm_qta_state(gas_id_t gas_id)
{
    return (grm_qta_state[gas_id]);
}

/*===========================================================================

FUNCTION  grm_qta_async_info_cb

DESCRIPTION
  This function handles the TRM unlock callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the GL1 task queue to action the abort in that context

===========================================================================*/
static void grm_qta_async_info_cb(
              trm_qta_event_callback_data *qta_info_p,
              trm_client_enum_t         client,
              grm_tag_t                 grm_tag )
{
  gas_id_t    gas_id;

  if ( !qta_info_p )
  {
    MSG_ERROR( GL1_DRDS_DBG_HDR"grm_qta_async_info_cb no valid qta_info_p grm_tag:%d",
                         grm_tag,0,0 );

    return;
  }

  gas_id = gl1_map_client_id_to_gas_id(grm_map_trm_client_id_to_grm_client_id(client));
  gas_id = check_gas_id(gas_id);
  
  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_qta_async_info_cb  "
                                        " grm_tag:%d client:%d",
                        					grm_tag, client  );
  }
  if (TRM_START_STATE == qta_info_p->operation)
  {
     gl1_hw_cm_set_qta_gap_for_g2x_irat(FALSE, gas_id);
     gpl1_set_grm_qta_state (TRUE, gas_id);
  }
  else
  {
    gpl1_set_grm_qta_state (FALSE, gas_id);

  if(TRM_END_STATE == qta_info_p->operation)
  {
    if(!gl1_hw_qta_gap_active(gas_id))
    {
       MSG_GERAN_HIGH_0_G( GL1_DRDS_DBG_HDR" TRM QTA cleanup, but QTA gap not active");
       gl1_grm_qta_gap_empty = TRUE;
    }
    else
    {
       gl1_grm_qta_gap_empty = gl1_hw_qta_empty_gap(gas_id);
    }
    /*Set the empty gap flag under the context of the source tech*/
    qta_info_p->qta_result->qta_gap_empty = gl1_grm_qta_gap_empty;
  }
  }

  l1_send_qta_startup_or_cleanup_msg(qta_info_p, gas_id);


  return;
} /*grm_qta_async_info_cb*/


/*===========================================================================

FUNCTION grm_gl1_get_dr_status

DESCRIPTION
  return setting of dr_enabled flag

PARAMS
  gas_id

DEPENDENCIES

RETURN VALUE boolean value of dr_enabled flag

===========================================================================*/
boolean grm_gl1_get_dr_status(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id, gas_id);

  if (grm_info_p)
  {
    return grm_info_p->dr_enabled;
  }
  else
  {
    MSG_GERAN_ERROR_0_G("grm_gl1_get_dr_status: grm_info_p is NULL ");
  }
  return FALSE;
}
/*===========================================================================

FUNCTION grm_add_arfcn_to_trm_freq_input_type

DESCRIPTION
  Adds new arfcn info band to active list for client if required

PARAMS
  None

DEPENDENCIES

RETURN VALUE boolean to indicate if freq store was updated

===========================================================================*/
boolean grm_add_arfcn_to_trm_freq_input_type(ARFCN_T           gl1_freq_info,
                                             grm_client_enum_t client_id,
                                             gas_id_t          gas_id)
{
  boolean     grm_freq_store_updated = FALSE;
  grm_info_t *grm_info_p             =
                grm_get_info_ptr_from_client( client_id, gas_id );

  if ( grm_info_p )
  {
    gl1_grm_info_mutex_lock(gas_id);
    /* First valid band entry */
    if ( !grm_info_p->grm_freq_info.num_bands )
    {
      grm_info_p->grm_freq_info.bands[grm_info_p->grm_freq_info.num_bands].band =
        gl1_map_gsm_band_to_sys_band( gl1_freq_info.band );

      grm_info_p->grm_freq_info.num_bands++;

      grm_freq_store_updated = TRUE;
    }
    else
    {
      uint16      i;

      /* Scan and see if new band needs adding to active list */
      for ( i = 0; i < grm_info_p->grm_freq_info.num_bands; i++ )
      {
        if ( i >= MAX_BAND_INFO_GRM )
        {
          MSG_GERAN_ERROR_2( GL1_DRDS_DBG_HDR"new_band:%d can not be added as the band store is full:%d",
                               gl1_freq_info.band,
                               grm_info_p->grm_freq_info.num_bands );
          gl1_grm_info_mutex_unlock(gas_id);
          return ( grm_freq_store_updated );
        }

        if ( gl1_map_gsm_band_to_sys_band( gl1_freq_info.band ) == grm_info_p->grm_freq_info.bands[i].band)
        {
          if (gl1_drds_debug( gas_id ))
          {
            MSG_GERAN_MED_4_G( GL1_DRDS_DBG_HDR"Add band to TRM freq_info for client_id:%d new_band:%d "
                                                "same as band at %d num_bands:%d",
                                client_id,
                                grm_info_p->grm_freq_info.bands[i].band,i,
                                grm_info_p->grm_freq_info.num_bands );
          }
          gl1_grm_info_mutex_unlock(gas_id);
          return ( grm_freq_store_updated );
        }
      }

      grm_info_p->grm_freq_info.bands[i].band = gl1_map_gsm_band_to_sys_band( gl1_freq_info.band );

      grm_info_p->grm_freq_info.num_bands++;
      grm_freq_store_updated = TRUE;
    }

    gl1_grm_info_mutex_unlock(gas_id);

    if ( grm_freq_store_updated && gl1_drds_debug( gas_id ) )
    {
      MSG_GERAN_MED_3( GL1_DRDS_DBG_HDR"Add band to TRM freq_info for client_id:%d new_band:%d num_bands:%d",
                         client_id,
                         grm_info_p->grm_freq_info.bands[grm_info_p->grm_freq_info.num_bands-1].band,
                         grm_info_p->grm_freq_info.num_bands );
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_add_arfcn_to_trm_freq_input_type no valid "
                                         "grm_info for client_id:%d",
                         client_id );
  }

  return ( grm_freq_store_updated );
}

/*===========================================================================

FUNCTION  grm_retain_lock_for_acquisition

DESCRIPTION
  Installs an appropriate unlock callback based on current acquisition priority

DEPENDENCIES
  Must only be called when client already holds a lock

RETURN VALUE
  None

SIDE EFFECTS
  TRM API usage in this function may trigger unlock callbacks in the same context

===========================================================================*/
void grm_retain_lock_for_acquisition(grm_client_enum_t acq_client,
                                     grm_unlock_mode_t unlock_mode,
                                     trm_reason_enum_t curr_acq_reason,
                                     sys_proc_type_e_type subreason,
                                     gas_id_t          gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  trm_reason_enum_t next_acq_reason;
  boolean change_prio_stat = FALSE;

  /* invalidate any existing unlock callback while changes are made */
  grm_retain_lock( acq_client, NULL, gas_id );

  next_acq_reason = grm_get_acquisition_reason(curr_acq_reason, gas_id);
  /* change priority to the next acquisition reason */
  change_prio_stat = grm_change_priority( acq_client, next_acq_reason, subreason, gas_id );

  if ( grm_get_resource_lock_state(gas_id))
  {
     change_prio_stat = TRUE;
     MSG_GERAN_HIGH_0_G("grm_retain_lock_for_acquisition: force change_prio_set to TRUE" );
  }
  if (change_prio_stat)
  {
    /* install the appropriate unlock callback (which may fire immediately) */
    if (GRM_IGNORE_UNLOCK_REQUIRED == unlock_mode)
    {
      /* install the limited callback that does not respond to UNLOCK_REQUIRED */
      grm_retain_lock( acq_client,
                       (grm_unlock_callback_t)&grm_limited_acq_unlock,
                       gas_id );
    }
    else
    {
      /* install the normal callback here so that UNLOCK_REQUIRED is handled */
      grm_retain_lock( acq_client,
                       (grm_unlock_callback_t)&grm_normal_acq_unlock,
                       gas_id );
    }
  }
  else
  {
    grm_set_trm_status(GL1_TRM_ABORTING, l1_tskisr_blk->client_id, gas_id);
    MSG_GERAN_HIGH_0_G(GL1_DRDS_DBG_HDR"grm_change_priority:Denied OR Pending "
                                       "So Retain Lock For Acquisition not performed!");
  }
  return;
}
/*===========================================================================

FUNCTION
  gl1_msim_reset_reserve_at_count()
DESCRIPTION
..this function shall be called when status of async sub changes or
  entering idle mode


DEPENDENCIES

RETURN VALUE
  None
SIDE EFFECTS
===========================================================================*/
void gl1_msim_reset_reserve_at_count( gas_id_t gas_id )
{
  reserve_at_count[gas_id] = 0;
}

/*===========================================================================

FUNCTION  grm_limited_acq_unlock

DESCRIPTION
  This wrapper function handles an abort callback during the FIND_BCCH state.
  It behaves exactly as grm_normal_acq_unlock() above, but does NOT
  respond to UNLOCK_REQUIRED events, by design

DEPENDENCIES
  Implemented with TRM "advanced" unlock callback mechanism

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void grm_limited_acq_unlock(
              trm_unlock_callback_data *unlock_data_p )
{
  if ( unlock_data_p )
  {
    /* this callback handles UNLOCK_REQUIRED as a special case */
    /* by deliberately ignoring it without triggering an abort */
    if ( ((TRM_UNLOCK_REQUIRED != unlock_data_p->event) && (TRM_UNLOCK_BAND_INCOMPATIBLE != unlock_data_p->event)) ||
         ( ((TRM_UNLOCK_REQUIRED == unlock_data_p->event) || (TRM_UNLOCK_BAND_INCOMPATIBLE == unlock_data_p->event)) && IS_URGENT_ACCESS(unlock_data_p->winning_client_info.reason) ))
    {
      grm_normal_acq_unlock( unlock_data_p );
    }
    else
    {
      grm_client_enum_t grm_client_id =
        grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);

      gas_id_t gas_id = gl1_map_client_id_to_gas_id(grm_client_id);

      MSG_GERAN_HIGH_3_G( "grm_limited_acq_unlock originated by client %d ignored by GSM client %d event %d",
                          (int)(unlock_data_p->winning_client_info.client_id),
                          (int)(unlock_data_p->unlock_client), (int)unlock_data_p->event );
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_default_unlock

DESCRIPTION
  Default unlock callback that does nothing. May be used in calls to
  trm_retain_lock() instead of a NULL function pointer. Must NOT be used
  in calls to trm_retain_lock_advanced() due to parameter differences

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Ignores all unlock events by design

===========================================================================*/
static void grm_default_unlock(
              trm_unlock_callback_data *unlock_callback_data_p )
{
  if ( unlock_callback_data_p )
  {
    if ( TRM_UNLOCK_CANCELLED != unlock_callback_data_p->event )
    {
      grm_client_enum_t grm_client_id =
        grm_map_trm_client_id_to_grm_client_id(unlock_callback_data_p->unlock_client);

      gas_id_t gas_id = gl1_map_client_id_to_gas_id(grm_client_id);

      /* output debug only for UNLOCK_BY, UNLOCK_REQUIRED, UNLOCK_IMMEDIATELY etc. */
      MSG_GERAN_HIGH_3_G( "grm_default_unlock Client:%d unlock event:%d by %d",
                          (int)unlock_callback_data_p->unlock_client,
                          (int)unlock_callback_data_p->event,
                          (int)unlock_callback_data_p->unlock_by_sclk );
    }
  }

  /* NOTE: if this callback is ever seen to be firing and causes any sort of */
  /* permanent lock denial scenarios, consider enhancing the code here with: */
  /* trm_abort_callback(client_id, unlock_ev, unlock_by); to allow an unlock */

  return;
}

/*===========================================================================

FUNCTION  grm_normal_acq_unlock

DESCRIPTION
  This function handles unlock events for all GSM clients during acquisition
  when UNLOCK_REQUIRED events are required to be supported

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the GL1 task queue to action the abort in that context

===========================================================================*/
void grm_normal_acq_unlock(
              trm_unlock_callback_data *unlock_data_p )
{
  if ( unlock_data_p )
  {
    /* resolve the GAS ID from the TRM client ID */
    grm_client_enum_t grm_client_id =
        grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);

    gas_id_t gas_id = gl1_map_client_id_to_gas_id(grm_client_id);
    gl1_grm_abort_callback_t *abort_cb_msg_p;
    gl1_grm_abort_callback_t  abort_cb_msg;

    abort_cb_msg_p = &abort_cb_msg;
    memset(&abort_cb_msg,NULL,sizeof(gl1_grm_abort_callback_t));

    if (TRM_UNLOCK_CANCELLED == unlock_data_p->event)
    {
	  /*Intention is to set to MAX duration, and if there is any premption later on, the Frame dur gets updated
		  via Unlock_By/Unlock_required event */
	
         MSG_GERAN_HIGH_3_G("TRM_UNLOCK_CANCELLED received for GRM Client:%d TRM Client:%d, Updated frame_duration=%d",
                         grm_client_id,
                         (int)(unlock_data_p->unlock_client),
						 RETAIN_UNLOCK_CANCEL_FRAME_DURATION);
    }
      /* post a message to the GL1 task to complete the processing */

      /* initialise client details */
      abort_cb_msg_p->client = grm_map_trm_client_id_to_grm_client_id( unlock_data_p->unlock_client );
      abort_cb_msg_p->gas_id = gas_id;

    if(TRM_UNLOCK_CANCELLED == unlock_data_p->event)
    {
         abort_cb_msg_p->event          = unlock_data_p->event;
         abort_cb_msg_p->frames_grant   = RETAIN_UNLOCK_CANCEL_FRAME_DURATION;
    }
      /* convert UNLOCK_REQUIRED events into time-bounded pre-emptions, depending on the client causing the unlock */
    else if ( (TRM_UNLOCK_REQUIRED == unlock_data_p->event) || (TRM_UNLOCK_BAND_INCOMPATIBLE == unlock_data_p->event) )
      {
        abort_cb_msg_p->event          = TRM_UNLOCK_BY;
        abort_cb_msg_p->unlock_by_sclk = ( timetick_get_safe() + ACQ_STATE_UNLOCK_BY_SCLCKS);

        MSG_GERAN_HIGH_3_G("TRM_UNLOCK_REQUIRED/TRM_UNLOCK_BAND_INCOMPATIBLE originated by client:%d for GSM client:%d",
                           (int)(unlock_data_p->winning_client_info.client_id),
                           (int)(unlock_data_p->unlock_client),(int)unlock_data_p->event);
      }
      else
      {
        /* TRM_UNLOCK_BY and TRM_UNLOCK_IMMEDIATELY are handled here */
        abort_cb_msg_p->event          = unlock_data_p->event;
        abort_cb_msg_p->unlock_by_sclk = unlock_data_p->unlock_by_sclk;
      }

      /* set the inter-task header */
      abort_cb_msg_p->header.message_set = MS_L1_L1;
      abort_cb_msg_p->header.message_id  = L1_GRM_ABORT_CALLBACK;
      PUT_IMH_LEN(sizeof(gl1_grm_abort_callback_t) - sizeof(IMH_T), abort_cb_msg_p);

      /* post the message to the task */
      (void)L1_isr_send_message(l1_queues[gas_id], abort_cb_msg_p, FALSE, gas_id);
    }
  else
  {
    MSG_GERAN_ERROR_0("Bad unlock callback, unable to detect client");
  }

  return;
}
boolean grm_is_embms_active(gas_id_t gas_id)
{
    boolean use_minimum = FALSE;
  
#ifdef FEATURE_LTE
    /* if EMBMS is active, use minimum durations to try to fit into the service gaps */
    if (LTE_ACTIVITY_EMBMS == (LTE_ACTIVITY_EMBMS & geran_get_lte_activity_status(gas_id)))
    {
      use_minimum = TRUE;
    }
#endif /* FEATURE_LTE */

 return use_minimum;
}
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  grm_get_acquisition_duration

DESCRIPTION
  Returns the maximum amount of time (in milliseconds) that a GERAN
  client should use in req-and-notify calls for GSM acquisition

DEPENDENCIES
  Only valid for power scan and BCCH parallel decode operations

RETURN VALUE
  Time in milliseconds

SIDE EFFECTS
  None

===========================================================================*/
timetick_type grm_get_acquisition_duration(gas_id_t gas_id)
{
  boolean use_minimum;

#ifdef FEATURE_LTE
  /* if EMBMS is active, use minimum durations to try to fit into the service gaps */
  if (LTE_ACTIVITY_EMBMS == (LTE_ACTIVITY_EMBMS & geran_get_lte_activity_status(gas_id)))
  {
    use_minimum = TRUE;
  }
  else
#endif /* FEATURE_LTE */
  {
    use_minimum = FALSE;
  }

  /* differentiate the duration based on the current L1 state */
  /* since power scans can make use of smaller chunks of time */
  if (L1_MULTI_SIM_POWER_SCAN == l1_tsk_buffer[gas_id].sub_state)
  {
    /* return the appropriate min/max durarion for power scan operation */
    return(use_minimum ? GRM_PSCN_MIN_REQUEST_MS : GRM_PSCN_MAX_REQUEST_MS);
  }

  /* return the appropriate min/max durarion for parallel BCCH decoding */
  return(use_minimum ? GRM_BCCH_MIN_REQUEST_MS : GRM_BCCH_MAX_REQUEST_MS);
}
#endif
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION grm_process_client_state_update_result

DESCRIPTION
processes the result structure populated by TRM when calling trm_set_client_state().
TRM_GRANTED implies valid Trans Id and TRM expects GL1 to release Trans Id.
In the current implemntation, TRM grants either valid RF devcie or INVALID RF
devcie for all requested bands. Therefore, the scenario where GL1 requests for
two bands (example) and gets valid RF device for one band and INVALID RF device
for other band is not supported.

  TRM returns grant as GRANTED if all the requested bands get valid RF device and
  expects GL1 to release Trans Id after GL1 finishes with the activity.

  TRM returns grant as DENIED and does not expect GL1 to release the Trans Id.
  All the requested bands get INVALID RF device.

In future, TRM can return mix of INVALID RF device and VALID RF device.
In case of mix valid RF device and invalid RF device, grant will be DENIED and
TRM will not expect GL1 to release Trans Id. However, TRM will expect GL1 to send
request to TRM again only with the band info which got valid RF devices before and
expects GL1 to release Trans Id after GL1 finishes with the activity

PARAMS
 grm_info_t* pointer to info store for sub performing the measurements
 activity  (non-serving band Ncell activities, IRAT)

DEPENDENCIES

RETURN VALUE
frequency registration result

===========================================================================*/
static void grm_process_client_state_update_result(
            grm_sc_activity_state_info_t  *result_data,
            gl1_sc_activities_t           activity,
            grm_client_enum_t             client_id,
            gas_id_t                      gas_id)
{
  int i;
  sys_band_T gsm_band;
  grm_info_t* info_ptr = NULL;
  int band_count = 0;
  int denied_bands = 0;
  boolean ifs_on_new_device = FALSE;

  if ( !result_data )
  {
    MSG_GERAN_ERROR_0_G(GRM_TM_HDR"Unable to process client state output data: "
                                  "result_data == NULL");
    return;
  }

  info_ptr = grm_get_info_ptr_from_client(client_id, gas_id );

  /*sanity checks*/
  if( !info_ptr )
  {
    MSG_GERAN_ERROR_2_G(GRM_TM_HDR"Unable to process client state output data: "
                                  "Invalid grm client ID %d TRM client Id %d",
                        client_id,
                        result_data->client_state_info.client_id);
    return;
  }

  /* band_count : Numbers of bands requested by GL1 in the band registration*/
  if(result_data->trm_return_data.set_state_type == TRM_IFS_STATE)
  {
    if(result_data->client_state_info.data.ifs_data.freq_info.num_bands > MAX_BAND_INFO_GRM)
    {
      MSG_GERAN_HIGH_1_G(GRM_TM_HDR"WARNING: IFS result_data num_bands > MAX_BAND_INFO_GRM %d",
                         result_data->client_state_info.data.ifs_data.freq_info.num_bands);
      band_count = MAX_BAND_INFO_GRM;
    }
    else
    {
      band_count = result_data->client_state_info.data.ifs_data.freq_info.num_bands;
    }
  }
  else
  {
    if(result_data->client_state_info.data.irat_data.freq_info.num_bands > MAX_BAND_INFO_GRM)
    {
      MSG_GERAN_HIGH_1_G(GRM_TM_HDR"WARNING: IFS result_data num_bands > MAX_BAND_INFO_GRM %d",
                         result_data->client_state_info.data.irat_data.freq_info.num_bands);
      band_count = MAX_BAND_INFO_GRM;
    }
    else
    {
      band_count = result_data->client_state_info.data.irat_data.freq_info.num_bands;
    }
  }

#ifdef DEBUG_DRDSDS
  MSG_GERAN_HIGH_5_G(GRM_TM_HDR"TRM BAND REG State Type %d State Op %d, Trans Id %d, START: %d, STOP %d ",
                                    result_data->trm_return_data.set_state_type,
                                    result_data->trm_return_data.state_oper,
                                    result_data->trm_return_data.transaction_id,
                                    result_data->start_frame,
                                    (result_data->start_frame + result_data->duration));
#endif

  /*establish what request was made*/
  if (result_data->trm_return_data.set_state_type == TRM_IFS_STATE)
  {
    /*is this a start or stop request*/
    if (result_data->trm_return_data.state_oper == TRM_START_STATE)
    {
      rfm_device_enum_type curr_rf_device = gl1_hw_get_gsm_rf_id(client_id, gas_id );

      /*dont clear denied band info if persistent denial is active as these bands are added to trm request
        bands will be cleared when persistent denial process ends i.e. 4 paging blocks*/
      if(!grm_persistent_denial_monitoring(GRM_CHECK_PERSISTENT_DENIAL_FLAG,
                                           client_id,
                                           GRM_NULL_COMMAND,
                                           gas_id))
      {
        /*clear out previously stored TRM denied band information*/
        memset(info_ptr->persistent_denial.denied_band_info,
               TRM_BAND_TUNE_OK,
               sizeof(info_ptr->persistent_denial.denied_band_info));
      }

      /*Process the result structure and identify any denied BANDS*/
      for (i = 0; i < band_count; i++)
      {
        if ( ( result_data->trm_return_data.result.ifs_result.band_dev_info[i].primary_device != RFM_INVALID_DEVICE ) &&
             ( result_data->trm_return_data.result.ifs_result.band_dev_info[i].primary_device != RFM_MAX_DEVICES ) &&
             ( result_data->trm_return_data.result.ifs_result.band_dev_info[i].primary_device != curr_rf_device )
           )
        {
           /* TRM has granted a valid device for IFS but the device is different from the current RF device for serving cell */
           ifs_on_new_device = TRUE;

           MSG_GERAN_HIGH_2(GRM_TM_HDR"skip IFS because of new rf dev %d, curr dev %d",
                            result_data->trm_return_data.result.ifs_result.band_dev_info[i].primary_device,
                            curr_rf_device);
        }
        /*if the device is invalid then we can not tune the RF to this band*/
        if(result_data->trm_return_data.result.ifs_result.band_dev_info[i].primary_device== RFM_INVALID_DEVICE ||
         result_data->trm_return_data.result.ifs_result.band_dev_info[i].primary_device == RFM_MAX_DEVICES)
        {
          gsm_band = gl1_map_sys_band_to_gsm_band(result_data->trm_return_data.result.ifs_result.band_dev_info[i].band);

          /*set the band tune to DENIED*/
          if (gsm_band < INVALID_BAND)
          {
            info_ptr->persistent_denial.denied_band_info[gsm_band].band_tune_result = TRM_BAND_TUNE_DENIED;
            info_ptr->persistent_denial.denied_band_info[gsm_band].band_registered = FALSE;
            MSG_GERAN_HIGH_1(GRM_TM_HDR"RF TUNE DENIED FOR BAND: %d",result_data->trm_return_data.result.ifs_result.band_dev_info[i].band);
            denied_bands++;
          }
        }
        else
        {
          gsm_band = gl1_map_sys_band_to_gsm_band(result_data->trm_return_data.result.ifs_result.band_dev_info[i].band);

          if (gsm_band < INVALID_BAND)
          {
            /* band is registered but TRM may give a different device. */
            info_ptr->persistent_denial.denied_band_info[gsm_band].band_on_new_dev = ifs_on_new_device;

            if ( TRUE == ifs_on_new_device)
            {
               info_ptr->persistent_denial.denied_band_info[gsm_band].band_tune_result = TRM_BAND_TUNE_DENIED;
            }
            info_ptr->persistent_denial.denied_band_info[gsm_band].band_registered = TRUE;

            /*update list of registered bands. */
          grm_add_sys_band_type_to_trm_freq_input_type (result_data->trm_return_data.result.ifs_result.band_dev_info[i].band,
                                                        grm_map_trm_client_id_to_grm_client_id( result_data->client_state_info.client_id ),
                                                        activity,
                                                        gas_id);
        }
       }
      }

      /* In the current implementation, either num_denied_bands should be 0 or equal to band_count */
      /*A frequency band registration has been made for this activity so set update in progress
      flag based upon the TRM grant*/
      if (TRM_GRANTED == result_data->trm_return_data.grant ||
         TRM_PARTIAL_GRANT == result_data->trm_return_data.grant)
      {
        /*A frequency band registration has been made for this activity so set update in progress flag*/
        result_data->update_in_progress = TRUE;
        result_data->trans_id_act = result_data->trm_return_data.transaction_id;

        MSG_GERAN_MED_3_G(GRM_TM_HDR" IFS meas result %d: requested bands %d denied bands %d",result_data->trm_return_data.grant, band_count, denied_bands );

      }
      else
      {
        /*If there are no IRAT bands successfully registered then we do not need to send a STOP to de-register any  BANDS */
        result_data->update_in_progress = FALSE;

#ifdef ABORT_F3_REDUCTION
        MSG_GERAN_HIGH_1_G(GRM_TM_HDR"surround cell activity denied by TRM clear trans id: %d",
                                          result_data->trm_return_data.transaction_id); 
#endif   
        result_data->trm_return_data.transaction_id = 0;
      }
    }
    else /*TRM_END_STATE*/
    {
      MSG_GERAN_MED_2_G(GRM_TM_HDR"Client state update END called for TRM IFS meas client %d FN %d",
                       result_data->client_state_info.data.ifs_data.measured_client,
                       gl1_get_FN(gas_id ));
    }
  }
  else if (result_data->trm_return_data.set_state_type == TRM_IRAT_STATE )
  {
    MSG_GERAN_MED_1_G(GRM_TM_HDR"IRAT Measured Client %d",
                     result_data->client_state_info.data.irat_data.measured_client);

    /*Is this an update start or stop request?*/
    if (result_data->trm_return_data.state_oper == TRM_START_STATE)
    {
      /*clear out previously stored TRM denied band information*/
      memset(&info_ptr->irat_denied_bands, 0, sizeof(info_ptr->irat_denied_bands));

       /* In the current implementation, either irat_denied_bands.num_bands should be 0 or equal to band_count */
      /*A frequency band registration has been made for this activity so set update in progress flag based upon the TRM grant*/
      if (TRM_GRANTED == result_data->trm_return_data.grant  ||
        TRM_PARTIAL_GRANT == result_data->trm_return_data.grant)
      {
        result_data->update_in_progress = TRUE;
        result_data->trans_id_act = result_data->trm_return_data.transaction_id;

      /*Process the result structure and identify any denied BANDS*/
      for (i = 0; i < band_count; i++)
      {
        /*if the device is invalid then we can not tune the RF to this band*/

        if((result_data->trm_return_data.result.irat_result.band_dev_info[i].primary_device== RFM_INVALID_DEVICE)
                || (result_data->trm_return_data.result.irat_result.band_dev_info[i].primary_device== RFM_MAX_DEVICES))
        {
          info_ptr->irat_denied_bands.bands[i].band = result_data->trm_return_data.result.irat_result.band_dev_info[i].band;
          info_ptr->irat_denied_bands.num_bands++;

          MSG_GERAN_HIGH_1_G(GRM_TM_HDR"RF TUNE DENIED FOR IRAT BAND %d", info_ptr->irat_denied_bands.bands[i].band);
          denied_bands++;
        }
        else
        {
          /*add IRAT frequency to list of registered BANDS this is required for when we send the STOP */
          grm_add_sys_band_type_to_trm_freq_input_type (result_data->trm_return_data.result.irat_result.band_dev_info[i].band,
                                                        client_id,
                                                        activity,
                                                        gas_id);
        }
      }

        MSG_GERAN_MED_3_G(GRM_TM_HDR" IRAT meas result %d: requested bands %d denied bands %d",result_data->trm_return_data.grant, band_count, denied_bands );
      }
      else
      {
        /*If there are no IRAT bands successfully registered then we do not need to send a STOP to de-register any  BANDS */
        result_data->update_in_progress = FALSE;
        result_data->trm_return_data.transaction_id = 0;
        MSG_GERAN_HIGH_1_G(GRM_TM_HDR"surround cell activity denied by TRM clear trans id: %d",
                                          result_data->trm_return_data.transaction_id);
      }

    }
    else /*TRM_END_STATE*/
    {
#ifdef DEBUG_DRDSDS
      MSG_GERAN_MED_2_G(GRM_TM_HDR"Client state update END called for TRM meas client %d FN %d",
                         result_data->client_state_info.data.irat_data.measured_client,
                         gl1_get_FN(gas_id ));
#endif
      /*Clear update in progress flag for this activity*/
      result_data->update_in_progress = FALSE;
    }
  }
  else
  {
    /*No processing required but capture for debug purposes*/
    MSG_GERAN_LOW_0_G(GRM_TM_HDR"CLIENT STATE UPDATE RESULT NOT PROCESSED");
  }
  return;
}

#endif

/*===========================================================================

FUNCTION grm_is_denied_band

DESCRIPTION
  Returns whether band is denied by TRM or not.

PARAMS
  None

DEPENDENCIES

RETURN VALUE
TRUE: Band Denied
FALSE: Tuned OK.

===========================================================================*/
boolean grm_is_denied_band(sys_band_T         band,
                           grm_client_enum_t  client_id,
                           gas_id_t           gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  boolean denied_band = FALSE;

  if (grm_info_p && gl1_drdsds_enabled(gas_id))
  {
   /*
     If persistent denial monitoring is enabled then denied bands are added to the
     registered bands via trm_request for DEMOD page If we have successfully registered
     the band with TRM then its OK to measure but will it remain tagged as denied so
     it can be added to the TRM request. If persistent denial is disabled then the
     band_registered flag will always be false
      */
   if(TRM_BAND_TUNE_DENIED == grm_info_p->persistent_denial.denied_band_info[band].band_tune_result)
   {
     denied_band = TRUE;
     MSG_GERAN_MED_1_G(GL1_DRDS_DBG_HDR"Denied Band %d", band);
   }
  }
  return denied_band;
}
/*===========================================================================

FUNCTION grm_set_device_configuration

DESCRIPTION
  In order to have asymmetric band support (SGLTE+G DSDA feature) i.e.
  2 WTR’s which don’t support the same G bands – 2nd WTR only supports a subset of 1st WTR.
  GL1 needs to bypass the DRDSDS code and execute asymmetric band support code
  Currently DRDSDS and SGLTE+G are mutually exclusive
  Note that in future there is a fall-back to DR-DSDS planned on SGLTE+G so this
  will need to be revised


PARAMS
  gas_id

DEPENDENCIES

RETURN VALUE
None

===========================================================================*/
void grm_set_device_configuration( gas_id_t gas_id )
{
  grm_client_enum_t requested_client_id = GRM_GSM1;

  grm_device_config = DEVICE_CONFIG_NON_DRDSDS;

  /* For non multisim always set NON DRDSDS no reason to interrogate GRM */
  if ( !gl1_msg_get_multi_sim_mode() )
  {
    return;
  }

  if ( grm_get_info( requested_client_id, NULL, GRM_GET_DEVICE_CONFIG, gas_id ) )
  {
    grm_info_t *grm_info_p = grm_get_info_ptr_from_client(requested_client_id, gas_id);

    if ( grm_info_p )
    {
      if ( ( TRM_DR_DSDS_IS_ENABLED & grm_info_p->grm_get_info_data.info.simul_cap_info ) &&
           !( TRM_SGLTE_SGTDS_IS_ENABLED & grm_info_p->grm_get_info_data.info.simul_cap_info ) )
      {
        /* set Device Configuration*/
        grm_device_config = DEVICE_CONFIG_DRDSDS;

#ifdef ABORT_F3_REDUCTION
         MSG_GERAN_HIGH_0_G( "Device is DRDSDS configured" ); 
#endif
      }
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G( "grm_set_device_configuration Invalid client ID:%d",
                         requested_client_id );
  }
  return;
}
/*===========================================================================

FUNCTION grm_device_config_is_drdsds

DESCRIPTION
  Returns TRUE if HW is configured for DRDSDS else
  returns false


PARAMS
None

DEPENDENCIES

RETURN VALUE
  Returns TRUE if HW is configured for DRDSDS else
  returns false

===========================================================================*/
boolean grm_device_config_is_drdsds( void )
{
  return ( grm_device_config == DEVICE_CONFIG_DRDSDS );
}

/*==========================================================================
FUNCTION grm_get_irat_rf_dev_from_grm_info

DESCRIPTION
       After Doing TRM band registration. i.e. calling trm_set_client_state
       TRM suggests the RF device in the return Data Structure
       which is used in this function below to get the appropriate RF Device
       suggested by TRM for IRAT

DEPENDENCIES
    should be called only after doing band registration (or grm_update_client_state_info)

PARAMS
    Band on which Measurement is to be performed

RETURNS
    the device info structure, eith members as the band, primary device, diversity device
============================================================================*/
trm_set_irat_band_device_info grm_get_irat_rf_dev_from_grm_info(sys_band_class_e_type bandin,
                                                                grm_client_enum_t client_id,
                                                                gas_id_t gas_id)
{
  int8 i;
  sys_band_class_e_type band_check;
  trm_set_irat_band_device_info dev_info;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  rfm_device_enum_type curr_device;
  /*Initialize dev_info structure, with measured band and invalid devices*/

  dev_info.band = bandin;
  dev_info.primary_device   = RFM_INVALID_DEVICE;
  dev_info.diversity_device = RFM_INVALID_DEVICE;

  if(grm_info_p)
  {
    if(grm_info_p->grm_activity_buf[SC_IRAT][GRM_ACT_IRAT].update_in_progress)
    {
      /*Try to get the device only if Band registration is done*/
      for(i = 0;
          i < grm_info_p->grm_activity_buf[SC_IRAT][GRM_ACT_IRAT].client_state_info.data.irat_data.freq_info.num_bands;
          i++)
      {
        band_check = grm_info_p->grm_activity_buf[SC_IRAT][GRM_ACT_IRAT].trm_return_data.result.irat_result.band_dev_info[i].band;

        if(band_check == bandin)
        {
          dev_info = grm_info_p->grm_activity_buf[SC_IRAT][GRM_ACT_IRAT].trm_return_data.result.irat_result.band_dev_info[i];

          if (gl1_drds_debug(gas_id))
          {
            MSG_GERAN_HIGH_3_G(GL1_DRDS_DBG_HDR"grm_get_irat_rf_dev_from_grm_info: For Band: %d Primary "
                                               "device %d and diversity device: %d",
                               bandin, dev_info.primary_device, dev_info.diversity_device);
          }

          return dev_info;
        }
        else
        {
          /*Ideally First index should match, as 1 band at a time is registered for IRAT*/
          if (gl1_drds_debug(gas_id))
          {
            MSG_GERAN_ERROR_3_G(GL1_DRDS_DBG_HDR"grm_get_irat_rf_dev_from_grm_info: at index %d trm "
                                                "band return:%d and bandin :%d",i,band_check,bandin);
          }
        }
      }/*for loop*/

      MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"No Band Match Found for Band %d so returning invalid device!!",bandin);

    }/*if update_in_progress*/
    else
    {
      /*No Transaction registered with TRM, so We may have regsitered through TRM request
          IRAT measurement request is always sent for LTE client. if update_in_progress is FALSE for LTE then this is an error
          All other IRAT clients can use primary */

      curr_device = gl1_hw_get_gsm_rf_id(client_id, gas_id);

      if((curr_device == RFM_INVALID_DEVICE)||(curr_device == RFM_MAX_DEVICES))
      {
        MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"BAND_REG Update_in_progress FALSE Invalid Device: %d",curr_device);
      }
      else
      {
        dev_info.primary_device = curr_device;

        MSG_GERAN_HIGH_3_G(GL1_DRDS_DBG_HDR"grm_get_irat_rf_dev_from_grm_info: For Band: %d Primary "
                                           "device %d and diversity device: %d",
                           bandin, dev_info.primary_device, dev_info.diversity_device);
      }

    }/*else update_in_progress*/
  }/*if grm_info NULL*/
  else
  {
    MSG_GERAN_ERROR_0_G("ERROR: GRM_INFO is NULL!");
  }

  return dev_info;
}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION  grm_gl1_set_current_trm_reason

DESCRIPTION
  Set the current TRM reason used by GL1.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_gl1_set_current_trm_reason(trm_reason_enum_t reason,
                                    sys_proc_type_e_type subreason,
                                    grm_client_enum_t client_id,
                                    gas_id_t gas_id)
{
  grm_info_t  *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_get_resource_lock_state(gas_id))
  {
     trm_reason_enum_t current_reason = grm_gl1_get_current_trm_reason(client_id,gas_id);
     MSG_GERAN_HIGH_3_G("grm_gl1_set_current_trm_reason %d to %d subreason %d blocked by resource lock", current_reason, reason, subreason );
  }
  else
  {
    if(grm_info_p)
    {
#ifdef FEATURE_GSM_RX_TX_SPLIT
      if (grm_is_a_tx_client(client_id))
      {
        if(grm_info_p->tx_info_p)
        {
          grm_info_p->tx_info_p->tx_reason = reason;
        }
        else
        {
          MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
        }
      }
      else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
      {
        grm_info_p->reason = reason;
        grm_info_p->sub_reason = subreason;
      }
    }
    else
    {
      MSG_GERAN_ERROR_1_G("grm_gl1_set_current_trm_reason: Invalid client_id %d", client_id);
    }
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_trm_status

DESCRIPTION
  Get the GL1 TRM  status.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
gl1_trm_state_t grm_get_trm_status(grm_client_enum_t client_id,
                                   gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  gl1_trm_state_t status = GL1_TRM_NULL;

  if ( grm_info_p )
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (grm_is_a_tx_client(client_id))
    {
      if (grm_info_p->tx_info_p)
      {
        status = grm_info_p->tx_info_p->tx_trm_status;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
      }
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
    status = grm_info_p->trm_status;
  }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return status;
}
/*===========================================================================

FUNCTION  grm_set_trm_status

DESCRIPTION
  Set the GL1 TRM status.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_trm_status(gl1_trm_state_t trm_status,
                        grm_client_enum_t client_id,
                        gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  gl1_trm_state_t *trm_status_p = NULL;

  if ( grm_info_p )
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (grm_is_a_tx_client(client_id))
    {
      if(grm_info_p->tx_info_p)
      {
        trm_status_p = &grm_info_p->tx_info_p->tx_trm_status;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
      }
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
      trm_status_p = &grm_info_p->trm_status;
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }

  if (trm_status_p != NULL)
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /* TRM status can only change after a callback while lock is active */
    if ( grm_get_resource_lock_state(gas_id) &&
         GL1_TRM_CALLBACK_PENDING != *trm_status_p )
    {
      MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"resource lock active - grm_set_trm_status %d ignored for client %d",trm_status,client_id);
    }
    else
#endif
    {
      *trm_status_p = trm_status;
      MSG_GERAN_HIGH_2_G("grm_set_trm_status: trm_status %d client_id %d",trm_status, client_id);
    }
  }

  return;
}
/*===========================================================================

FUNCTION  grm_get_trm_grant_status

DESCRIPTION
  Get the TRM grant status.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
trm_grant_return_enum_type grm_get_trm_grant_status(grm_client_enum_t client_id,
                                                    gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_grant_return_enum_type grant_status = TRM_RETURN_MAX;

  if ( grm_info_p )
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (grm_is_a_tx_client(client_id))
    {
      if(grm_info_p->tx_info_p)
      {
        grant_status = grm_info_p->tx_info_p->tx_trm_grant_status;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
      }
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
      grant_status = grm_info_p->trm_grant_status;
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return grant_status;
}
/*===========================================================================

FUNCTION  grm_set_trm_grant_status

DESCRIPTION
  Set the  TRM status.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_trm_grant_status(trm_grant_return_enum_type trm_grant_status,
                              grm_client_enum_t client_id,
                              gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if (grm_get_resource_lock_state(gas_id) &&
        grm_get_trm_status(client_id, gas_id) != GL1_TRM_CALLBACK_PENDING)
    {
#ifdef FEATURE_GSM_RX_TX_SPLIT
      if(grm_is_a_tx_client(client_id))
      {
        if (grm_info_p->tx_info_p)
        {
          MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_set_trm_grant_status %d ignored tx_status=%d",trm_grant_status,grm_info_p->tx_info_p->tx_trm_grant_status);
    }
    else
        {
          MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
        }
      }
      else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
      {
        MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_set_trm_grant_status %d ignored status=%d",trm_grant_status,grm_info_p->trm_grant_status);
      }
    }
    else
#endif /*defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )*/
    {
#ifdef FEATURE_GSM_RX_TX_SPLIT
      if (grm_is_a_tx_client(client_id))
      {
        if(grm_info_p->tx_info_p)
        {
          grm_info_p->tx_info_p->tx_trm_grant_status = trm_grant_status;
        }
        else
        {
          MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
        }
      }
      else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
      {
    grm_info_p->trm_grant_status = trm_grant_status;
  }
  }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_trm_release_required

DESCRIPTION
  Get the TRM release required.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_get_trm_release_required(grm_client_enum_t client_id,
                                     gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  boolean release_required = FALSE;

  if ( grm_info_p )
  {
    release_required = grm_info_p->trm_release_required;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return release_required;
}
/*===========================================================================

FUNCTION  grm_set_trm_release_required

DESCRIPTION
  Set the TRM release required.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_trm_release_required(boolean release_required,
                                  grm_client_enum_t client_id,
                                  gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if (!grm_get_resource_lock_state(gas_id))
#endif
    {
    grm_info_p->trm_release_required = release_required;
  }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_trm_requested_at_warmup

DESCRIPTION
  Get the TRM request at warm up.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_get_trm_requested_at_warmup(grm_client_enum_t client_id,
                                        gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  boolean trm_request_at_warmup = FALSE;

  if ( grm_info_p )
  {
    trm_request_at_warmup =  grm_info_p->trm_requested_at_warmup;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return trm_request_at_warmup;
}
/*===========================================================================

FUNCTION  grm_set_trm_requested_at_warmup

DESCRIPTION
  Set the TRM request at warm up.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_trm_requested_at_warmup(boolean trm_request,
                                     grm_client_enum_t client_id,
                                     gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
    grm_info_p->trm_requested_at_warmup = trm_request;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_trm_grant_fn

DESCRIPTION
  Get the TRM grant fn.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
uint32 grm_get_trm_grant_fn(grm_client_enum_t client_id,
                            gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  uint32 grant_fn = NOT_A_FRAME_NUMBER;

  if ( grm_info_p )
  {
    grant_fn = grm_info_p->trm_grant_fn;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return grant_fn;
}
/*===========================================================================

FUNCTION  grm_set_trm_grant_fn

DESCRIPTION
  Set the TRM grant fn.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_trm_grant_fn(uint32 trm_grant_fn,
                          grm_client_enum_t client_id,
                          gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if (!grm_get_resource_lock_state(gas_id))
#endif
    {
    grm_info_p->trm_grant_fn = trm_grant_fn;
  }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_trm_grant_frames

DESCRIPTION
  Get the TRM grant frames number.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
uint32 grm_get_trm_grant_frames(grm_client_enum_t client_id,
                                gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  uint32 grant_frames = 0;

  if ( grm_info_p )
  {
    grant_frames = grm_info_p->trm_grant_frames;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return grant_frames;
}
/*===========================================================================

FUNCTION  grm_set_trm_grant_frames

DESCRIPTION
  Set the TRM grant frames number.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_trm_grant_frames(uint32 trm_grant_frames,
                              grm_client_enum_t client_id,
                              gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if (!grm_get_resource_lock_state(gas_id))
#endif
    {
    grm_info_p->trm_grant_frames = trm_grant_frames;
  }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_frame_duration

DESCRIPTION
  Get frame duration.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
uint32 grm_get_frame_duration(grm_client_enum_t client_id,
                              gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  uint32 frame_duration = 0;

  if ( grm_info_p )
  {
    frame_duration = grm_info_p->frame_duration;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return frame_duration;
}
/*===========================================================================

FUNCTION  grm_set_frame_duration

DESCRIPTION
  Set frame duration.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_frame_duration(uint32            frame_duration,
                            grm_client_enum_t client_id,
                            gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if ( frame_duration > 0 || !grm_get_resource_lock_state(gas_id))
#endif
    {
    grm_info_p->frame_duration = frame_duration;
  }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_trm_reserve_fn

DESCRIPTION
  Get frame duration.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
uint32 grm_get_post_decrement_frame_duration(grm_client_enum_t client_id,
                                             gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  uint32 frame_duration = 0;

  if ( grm_info_p )
  {
    frame_duration = grm_info_p->frame_duration;

    if ( frame_duration > 1 )
    {
      grm_set_frame_duration(frame_duration - 1, client_id, gas_id);
    }
    else
    {
      grm_set_frame_duration(0, client_id, gas_id);
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return frame_duration;
}

/*===========================================================================

FUNCTION  grm_get_ps_access_mode

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
ps_access_mode_t grm_get_ps_access_mode(grm_client_enum_t  client_id,
                                        gas_id_t           gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  ps_access_mode_t ps_access_mode = PS_ACCESS_NONE;

  if ( grm_info_p )
  {
    ps_access_mode = grm_info_p->ps_access_mode;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return ps_access_mode;
}
/*===========================================================================

FUNCTION  grm_set_ps_access_mode

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_ps_access_mode(ps_access_mode_t    ps_access_mode,
                            grm_client_enum_t   client_id,
                            gas_id_t            gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
    grm_info_p->ps_access_mode = ps_access_mode;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}

/*===========================================================================

FUNCTION  grm_get_req_and_notify_duration

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
uint32 grm_get_req_and_notify_duration(grm_client_enum_t  client_id,
                                       gas_id_t           gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  uint32 duration = INVALID_SCLKS;

  if ( grm_info_p )
  {
    duration = grm_info_p->req_n_notify_duration;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return duration;
}
/*===========================================================================

FUNCTION  grm_set_req_and_notify_duration

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_req_and_notify_duration(uint32              duration,
                                     grm_client_enum_t   client_id,
                                     gas_id_t            gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
    grm_info_p->req_n_notify_duration = duration;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_modify_state_info

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
trm_modify_chain_state_return_data *grm_get_modify_state_info(grm_client_enum_t  client_id,
                                                              gas_id_t           gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_modify_chain_state_return_data *modify_state_info = NULL;

  if ( grm_info_p )
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (grm_is_a_tx_client(client_id))
    {
      if(grm_info_p->tx_info_p)
      {
        modify_state_info = &(grm_info_p->tx_info_p->tx_grm_modify_state_info);
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
      }
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
       modify_state_info = &(grm_info_p->grm_modify_state_info);
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return modify_state_info;
}
/*===========================================================================

FUNCTION  grm_set_modify_state_info

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_modify_state_info(trm_modify_chain_state_return_data   *modify_state_info,
                               grm_client_enum_t                    client_id,
                               gas_id_t                             gas_id)
{
  grm_info_t  *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if(grm_info_p && modify_state_info)
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (grm_is_a_tx_client(client_id))
    {
      if(grm_info_p->tx_info_p)
      {
        grm_info_p->tx_info_p->tx_grm_modify_state_info = *modify_state_info;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
      }
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
      grm_info_p->grm_modify_state_info = *modify_state_info;
    }
  }
  else
  {
    MSG_GERAN_ERROR_3_G("grm_set_modify_state_info: client_id %d "
                        "grm_info_p  %p modify_state_info %p",
                        client_id, (void*)grm_info_p, (void*)modify_state_info);
  }
  return;
}

/*===========================================================================

FUNCTION  grm_get_modify_chain_state_data

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
trm_modify_data *grm_get_modify_chain_state_data(grm_client_enum_t  client_id,
                                                 gas_id_t           gas_id)
{
  grm_info_t      *grm_info_p   = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_modify_data *modify_data  = NULL;

  if ( grm_info_p )
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (grm_is_a_tx_client(client_id))
    {
      if(grm_info_p->tx_info_p)
      {
        modify_data = &(grm_info_p->tx_info_p->tx_grm_modify_chain_state_data);
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
      }
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
       modify_data = &(grm_info_p->grm_modify_chain_state_data);
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return modify_data;
}
/*===========================================================================

FUNCTION  grm_set_modify_chain_state_data

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_modify_chain_state_data(trm_modify_data    *modify_data,
                                     grm_client_enum_t  client_id,
                                     gas_id_t           gas_id)
{
  grm_info_t  *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if(grm_info_p && modify_data)
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (grm_is_a_tx_client(client_id))
    {
      if(grm_info_p->tx_info_p)
      {
        grm_info_p->tx_info_p->tx_grm_modify_chain_state_data = *modify_data;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null tx_info_p! Invalid client_id %d", client_id );
      }
    }
    else
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
      grm_info_p->grm_modify_chain_state_data = *modify_data;
    }
  }
  else
  {
    MSG_GERAN_ERROR_3_G("grm_set_modify_chain_state_data: client_id %d "
                        "grm_info_p  %p modify_data %p",
                        client_id, (void*)grm_info_p, (void*)modify_data);
  }
  return;
}

/*===========================================================================

FUNCTION  grm_gl1_get_trm_sub_reason

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
sys_proc_type_e_type grm_gl1_get_trm_sub_reason(grm_client_enum_t  client_id,
                                                 gas_id_t           gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  sys_proc_type_e_type sub_reason = SYS_PROC_TYPE_NONE;

  if ( grm_info_p )
  {
    sub_reason = grm_info_p->sub_reason;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p! Invalid client_id %d", client_id );
  }
  return sub_reason;
}
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

/*GRM Transaction management Functions*/

/*===========================================================================

FUNCTION  grm_transaction_manager

DESCRIPTION
   Called at the end of ISR
   Checks grm_activity_buf for active transactions and
   does a band release on them if the end of activity is reached
   or
   if all active transactions have to be flushed

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
=============================================================================*/
void grm_transaction_manager(grm_tm_event_t       event,
                             gl1_sc_activities_t  act,
                             grm_client_enum_t    client_id,
                             gas_id_t             gas_id)
{
  boolean flush = FALSE;
  uint8 idx;
  /*local var for tracking ending FN for an activity*/
  uint32 act_FN;
  uint32 diff;
  /*Get the current FN*/
  uint32 current_FN = gl1_get_FN(gas_id);
  grm_sc_activity_state_info_t act_info;
  grm_info_t  *grm_info_p;

  gl1_grm_info_mutex_lock(gas_id);

  /*Get the GRM Info for the gas_id*/
  grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  /*Check to see ptr is valid*/
  if (!grm_info_p)
  {
    MSG_GERAN_ERROR_0_G(GRM_TM_HDR"ERROR: grm_info_p is NULL");
    gl1_grm_info_mutex_unlock(gas_id);
    return ;
  }

  switch (event)
  {
    case GRM_TM_FLUSH:
    {
      /*Set the Flush Flag,to release all transactions*/
      flush = TRUE;
    }
    /*Fall through*/
    case GRM_TM_CHECK:
    {
      /* Check for all active activities*/
      for (idx = 0; idx < GRM_ACT_MAX; idx++)
      {
        act_info = grm_info_p->grm_activity_buf[act][idx];

         /*Check if activity is active or not*/
         if(act_info.update_in_progress )
         {
           /*Adding considering Hyperframe as well*/
           act_FN = ADD_FN(act_info.start_frame, act_info.duration );

           MSG_GERAN_MED_5_G(GRM_TM_HDR"Activity:%d at index:%d Curr._FN:%d Completion_FN:%d Trans_id:%d",
                 act, idx, current_FN, act_FN, act_info.trans_id_act);

           /*get the difference between the current FN and the act_FN*/
           diff = SUB_FN(act_FN, current_FN);

           /*If current FN is equal to the scheduled completion FN,
                     or activity to be flushed
                     or difference between current Fn and scheduled FN is > than duration decided
                     , Release the activity*/
           if ((current_FN == act_FN)||flush || (diff > act_info.duration))
           {
              MSG_GERAN_MED_3_G(GRM_TM_HDR"Activity %d at index:%d reached its End, "
                                          "Call Band Release Flush:%d", act, idx, flush);
              grm_transaction_release(act,
                                      &grm_info_p->grm_activity_buf[act][idx],
                                      client_id,
                                      gas_id );
           }
         }/*if act_info.update_in_progress*/
      }/*for loop*/
   }
   break;

  default:
      MSG_GERAN_ERROR_0_G(GRM_TM_HDR" grm_transaction_manager: Unknown Event Type!!");

  }/*end Switch*/

  gl1_grm_info_mutex_unlock(gas_id);

}/*End of grm_transaction_manager*/

/*===========================================================================
  FUNCTION  grm_activity_queue_init

  DESCRIPTION
  Initialize the GRM Activity Queue

  DEPENDENCIES
  Should only be called at starting of GSM or Starting of Idle or Transfer
  state

  RETURN
  None
============================================================================*/
void grm_activity_queue_init(grm_client_enum_t    client_id,
                             gas_id_t             gas_id)
{
  /*Get the GRM Info for the gas_id*/
  grm_info_t  *grm_info_ptr = grm_get_info_ptr_from_client(client_id,gas_id);

  if (grm_info_ptr)
  {

    /*Ideally we should have released all transaction at this point 
      but still as a safety check Release All transaction before MemSet
      As In Case Resource Lock is Active then We dont happen to send Band Release
      Cause TRM Isnt Released as expected*/
    grm_send_band_release_if_required(client_id, gas_id);

    memset(&grm_info_ptr->grm_activity_buf,
           0,
           GRM_ACT_MAX * SC_MAX_ACTIVITIES * sizeof(grm_sc_activity_state_info_t));

    MSG_GERAN_MED_1_G(GRM_TM_HDR"Initializing the Queue Indexes for GRM_ACT for client %d",
                      client_id);

  }
  else
  {
    MSG_GERAN_ERROR_0_G(GRM_TM_HDR"ERROR: grm_info_ptr is NULL");
  }
  return;
}
/*=======================================================================
 FUNCTION:
  grm_find_next_indx_in_act_buf

 DESCRIPTION:
  Function to find next available index in the array for that activity
  in grm_activity_buf in grm_info

 DEPENDENCIES


 RETURN
  None

========================================================================*/
int8 grm_find_next_indx_in_act_buf(gl1_sc_activities_t  act,
                                   grm_client_enum_t    client_id,
                                   gas_id_t             gas_id)
{
  int8 index =  GRM_INVALID_ACT_INDEX;
  /*Get the GRM Info for the gas_id*/
  grm_info_t  *grm_info_ptr = grm_get_info_ptr_from_client(client_id,gas_id);

  /*Check to see ptr is valid*/
  if (grm_info_ptr)
  {
    /*Find Next available place for this activity*/
    for (index = 0; index < GRM_ACT_MAX; index++)
    {
      MSG_GERAN_LOW_3_G(GRM_TM_HDR"index: %d activity :%d update_in_progress : %d ",
                        index,
                        act,
                        grm_info_ptr->grm_activity_buf[act][index].update_in_progress );

      /*if activity stored is no longer valid, then we can use that index*/
      if (!grm_info_ptr->grm_activity_buf[act][index].update_in_progress)
      {
        MSG_GERAN_MED_2_G(GRM_TM_HDR"Found available index: %d for activity: %d",index, act);
        break;
      }
    }

    /*No available index has been found*/
  }
  else
  {
    MSG_GERAN_ERROR_0_G(GRM_TM_HDR"ERROR: grm_info_ptr is NULL");
  }

  /*If No Space available then return invalid index*/
  if (index == GRM_ACT_MAX)
  {
    index = GRM_INVALID_ACT_INDEX;
  }
  return index; 
}

/*=======================================================================
  FUNCTION:
    grm_transaction_release

  DESCRIPTION:
    Release The transaction

  DEPENDENCIES


  RETURN
  None

========================================================================*/
void grm_transaction_release(gl1_sc_activities_t            activity,
                             grm_sc_activity_state_info_t   *act_info_ptr,
                             grm_client_enum_t              client_id,
                             gas_id_t                       gas_id)
{
  int i,j;
  grm_info_t *info_ptr        = NULL;
  boolean     erase_all_bands = TRUE;

  /*Current RxD scope: Band release is not applicable */
  if(TRUE == grm_is_a_secondary_client(client_id))
  {
      MSG_GERAN_ERROR_0_G(GRM_TM_HDR"Secondary Client Band release attempted!");
      return;
  }

  /*Get the GRM Info for the gas_id*/
  info_ptr = grm_get_info_ptr_from_client(client_id,gas_id);

  if (info_ptr && (activity < SC_MAX_ACTIVITIES))
  {
    MSG_GERAN_MED_4_G( GRM_TM_HDR"SCE: Activity: %d Calling band_release: stored client_id %d "
                                  "gas_client_id %d Trans Id %d",
                        activity,
                        act_info_ptr->client_state_info.client_id,
                        client_id,
                        act_info_ptr->trm_return_data.transaction_id);

    gl1_grm_info_mutex_lock(gas_id);

    /*Check to see if active G client data matches stored client data. This check is for TRM G lient exchange during PCH in Xfer*/
    if(act_info_ptr->client_state_info.client_id != grm_map_grm_client_id_to_trm_client_id(client_id) )
    {
      act_info_ptr->client_state_info.client_id = grm_map_grm_client_id_to_trm_client_id(client_id);
    }

    if (activity == SC_IRAT)
    {
      /*only clear the bands registered once*/
      act_info_ptr->client_state_info.client_state = TRM_IRAT_STATE;
      act_info_ptr->client_state_info.data.irat_data.freq_info = info_ptr->irat_band_registered;

      act_info_ptr->client_state_info.data.irat_data.operation = TRM_END_STATE;
      act_info_ptr->client_state_info.data.irat_data.transaction_id = act_info_ptr->trm_return_data.transaction_id;

      grm_update_client_state_info(act_info_ptr, activity, client_id, gas_id);

      act_info_ptr->update_in_progress = FALSE;

      /*clear transaction ID. If a new start measurement occurs and transation id is non
        zero we can detect pending IFS stop and ensure correct trans id is cleared*/
      act_info_ptr->trm_return_data.transaction_id = 0;
      act_info_ptr->client_state_info.data.irat_data.transaction_id = 0;

      MSG_GERAN_MED_0_G(GRM_TM_HDR"Erase IRAT bands");
      memset(&info_ptr->irat_band_registered, 0, sizeof(info_ptr->irat_band_registered));
      grm_clear_stale_client_state_information(client_id, act_info_ptr, gas_id);
    }
    else
    {
      /*only clear the bands registered once*/
      act_info_ptr->client_state_info.client_state = TRM_IFS_STATE;
      act_info_ptr->client_state_info.data.ifs_data.freq_info = info_ptr->current_band_info;
      act_info_ptr->client_state_info.data.ifs_data.operation = TRM_END_STATE;
      act_info_ptr->client_state_info.data.ifs_data.transaction_id = act_info_ptr->trm_return_data.transaction_id;

      grm_update_client_state_info(act_info_ptr ,activity, client_id, gas_id);

      act_info_ptr->update_in_progress = FALSE;

      /*Check to see if they are ongoing updates before clearing the registered bands*/
      for (i= 0; i <= SC_FCH_SCH; i++)
      {
        for (j=0; j< GRM_ACT_MAX; j++)
        {
          if (info_ptr->grm_activity_buf[i][j].update_in_progress)
          {
            erase_all_bands = FALSE;
          }
        }
      }

      if(erase_all_bands)
      {
        MSG_GERAN_MED_0_G(GRM_TM_HDR"Erase IFS bands");
        memset(&info_ptr->current_band_info, 0, sizeof(info_ptr->current_band_info));
      }

      grm_clear_stale_client_state_information(client_id, act_info_ptr, gas_id);
    }

    gl1_grm_info_mutex_unlock(gas_id);
  }
  else
  {
    MSG_GERAN_ERROR_2_G(GL1_DRDS_DBG_HDR"Invalid Client ID %d or "
                                        "invalid sc activity %d",
                        client_id, activity);
  }
  return;
}

/*====================================================================
  FUNCTION:
    grm_check_ifs_band_reg

   DESCRIPTION:
     Function Which performs IFS Band Registration with TRM

   DEPENDENCIES:
     Should be used only in DR Mode

   RETURNS:
     -TRUE if Transaction is created with TRM Successfully
     -FALSE if Transaction is not created and denied.
=======================================================================*/
boolean grm_check_ifs_band_reg(gl1_sc_activities_t sc_activity,
                               sys_band_T sc_band,
                               uint32 frames_required,
                               boolean div_needed,
                               gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  grm_info_t *info_ptr = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id,gas_id);
  boolean activity_can_run = TRUE;
  int i = 0;
  sys_band_T gsm_band = SYS_BAND_NONE ;

  /*Find the next available index*/
  int8 idx = grm_find_next_indx_in_act_buf(sc_activity, l1_tskisr_blk->client_id, gas_id);
  /*check for valid index and info_ptr*/
  if (info_ptr && (idx != -1))
  {
    /*get the pointer*/
    grm_sc_activity_state_info_t* act_info_ptr = &info_ptr->grm_activity_buf[sc_activity][idx];

    MSG_GERAN_MED_3_G(GRM_TM_HDR"GRM_TM: IFS Reg called for Activity: %d To be stored at index:%d for %d Frames",
                       sc_activity, idx, frames_required);
    /* Update denied band by sending existing registered band info*/
    if (sc_activity == SC_PM)
    {
       act_info_ptr->client_state_info.data.ifs_data.freq_info = info_ptr->current_band_info;
    }
    else
    {
       /* Add new band registration */
       act_info_ptr->client_state_info.data.ifs_data.freq_info.num_bands = 1;
       act_info_ptr->client_state_info.data.ifs_data.freq_info.bands[0].band = gl1_map_gsm_band_to_sys_band(sc_band);
    }

    act_info_ptr->client_state_info.client_id = grm_map_grm_client_id_to_trm_client_id( gl1_map_gas_id_to_client_id(gas_id) );
    act_info_ptr->client_state_info.client_state = TRM_IFS_STATE;
    act_info_ptr->client_state_info.data.ifs_data.operation = TRM_START_STATE;
    act_info_ptr->start_frame = gl1_get_FN( gas_id );
    act_info_ptr->duration = frames_required;
    /*Check if duration is less than MAX allowed*/
    if(act_info_ptr->duration > MAX_DURATION_GRM_ACT)
    {
      MSG_GERAN_HIGH_2_G(GRM_TM_HDR"Duration:%d Greater than MAX Allowed:%d, "
                                   "hence Resetting to Max",
                         act_info_ptr->duration,
                         MAX_DURATION_GRM_ACT);
      act_info_ptr->duration = MAX_DURATION_GRM_ACT;
    }
    act_info_ptr->client_state_info.data.ifs_data.duration = gpl1_multi_sim_convert_frames_to_sclcks((act_info_ptr->duration));
    act_info_ptr->client_state_info.data.ifs_data.measured_client = act_info_ptr->client_state_info.client_id;
    /*Populate the diversity needed based on input*/
    act_info_ptr->client_state_info.data.ifs_data.is_diversity_needed = div_needed;

    MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR" num_bands: %d sc_band %d",
                          act_info_ptr->client_state_info.data.ifs_data.freq_info.num_bands, sc_band);

    grm_update_client_state_info(act_info_ptr,sc_activity, l1_tskisr_blk->client_id, gas_id);

    if(sc_activity == SC_PM)
    {
       for(i = 0; i < act_info_ptr->client_state_info.data.ifs_data.freq_info.num_bands; i++)
       {
         gsm_band = gl1_map_sys_band_to_gsm_band(act_info_ptr->trm_return_data.result.ifs_result.band_dev_info[i].band);

         /*set the band tune to DENIED*/
         if (gsm_band < INVALID_BAND)
         {
            if (info_ptr->persistent_denial.denied_band_info[gsm_band].band_tune_result== TRM_BAND_TUNE_DENIED)
            {
              /*Start Persistent Denial Only in Idle Mode*/
              if( l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
              {
                /*denied band detected, set persitent denial flag which will cause all
                  denied bands to be added to TRM request*/
                MSG_GERAN_MED_1_G(GL1_DRDS_DBG_HDR"PM idle persistent band denial condition met for GSM BAND %d",gsm_band);
                (void)grm_persistent_denial_monitoring(GRM_START_PERSISTENT_DENIAL_MONITORING,
                                                       l1_tskisr_blk->client_id,
                                                       GRM_NULL_COMMAND,
                                                       gas_id);
              }
              activity_can_run = FALSE;
              break;
            }
         }

       }
    }
    else
    {
        activity_can_run =  (info_ptr->persistent_denial.denied_band_info[sc_band].band_tune_result != TRM_BAND_TUNE_DENIED);
    }

    MSG_GERAN_HIGH_5_G(GRM_TM_HDR" IFS Reg activity type: %d, activity_can_run %d, "
                                 "Trans Id: %d index: %d duration: %d",
                       sc_activity,
                       activity_can_run,
                       act_info_ptr->trans_id_act,
                       idx, act_info_ptr->duration);

  }
  else
  {

    MSG_GERAN_ERROR_1_G(GRM_TM_HDR"ERROR: Index Found: %d: IFS Band Registration FAILED!",idx);
    activity_can_run = FALSE;
  }

 return activity_can_run;
}


/*===========================================================================
 FUNCTION grm_check_irat_band_reg

 DESCRIPTION
  For performing Band registration for IRat Activities

 PARAMS


 DEPENDENCIES

 RETURNS:
     -TRUE if Transaction is created with TRM Successfully
     -FALSE if Transaction is not created and denied.
=============================================================================*/
boolean grm_check_irat_band_reg (grm_client_enum_t meas_client,
                                 sys_band_class_e_type trm_band,
                                 uint32 frames_required,
                                 boolean div_needed,
                                 gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  grm_info_t *info_ptr = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id,gas_id);

  boolean activity_can_run = FALSE;

  /*Only 1 IRAT activity possible at a time, so index is hard coded to 0 here
       i.e. the first element of array*/
  int8 idx = GRM_ACT_IRAT;

  /*if valid info_ptr and no other IRAT activity in progress*/
  if (info_ptr && (!(info_ptr->grm_activity_buf[SC_IRAT][idx].update_in_progress)))
  {
    /*get the pointer*/
    grm_sc_activity_state_info_t* act_info_ptr = &info_ptr->grm_activity_buf[SC_IRAT][idx];

    act_info_ptr->start_frame = gl1_get_FN(gas_id);

    /*IRAT Frame Duration*/
    act_info_ptr->duration = (frames_required);

    /*Check if duration is less than MAX allowed*/
    if(act_info_ptr->duration > MAX_DURATION_GRM_ACT)
    {
      MSG_GERAN_HIGH_2_G(GRM_TM_HDR"Duration:%d Greater than MAX Allowed:%d, "
                                   "hence Resetting to Max",
                         act_info_ptr->duration,MAX_DURATION_GRM_ACT);
      act_info_ptr->duration = MAX_DURATION_GRM_ACT;
    }
    /*Always have primary client, if diversity is needed then flag will be set*/
    act_info_ptr->client_state_info.client_id =
                grm_map_grm_client_id_to_trm_client_id(l1_tskisr_blk->client_id);

    act_info_ptr->client_state_info.client_state = TRM_IRAT_STATE;
    /*measured client id is updated in grm_check_idle_sc_activity_ok function*/
    act_info_ptr->client_state_info.data.irat_data.operation = TRM_START_STATE;
    /*search is done 1 band at a time for IRAT*/
    act_info_ptr->client_state_info.data.irat_data.freq_info.num_bands = 1;
    act_info_ptr->client_state_info.data.irat_data.freq_info.bands[0].band = trm_band;
    /*update measured client*/
    act_info_ptr->client_state_info.data.irat_data.measured_client =
      grm_map_grm_client_id_to_trm_client_id(meas_client);

    act_info_ptr->client_state_info.data.irat_data.duration =
            gpl1_multi_sim_convert_frames_to_sclcks(act_info_ptr->duration);
   /* Always set to FALSE for GERAN as we never call change reason during IRAT */
    act_info_ptr->client_state_info.data.irat_data.should_change_reason_to_irat = FALSE;
   /*Set Diversity flag appropriately, for LTE, flag is set inside
     grm_update_client_state_info Function explicitly! */
   act_info_ptr->client_state_info.data.irat_data.is_diversity_needed = div_needed;
#ifdef DEBUG_DRDSDS

   MSG_GERAN_MED_2_G(GRM_TM_HDR"IRAT ACTIVITY current fn %d duration %d",
                     gl1_get_FN(gas_id),
                     frames_required);
#endif

    /*send TRM BAND request*/
    grm_update_client_state_info(act_info_ptr,SC_IRAT, l1_tskisr_blk->client_id, gas_id);

    /*Successful band registration means update_in_progress == TRUE*/
    activity_can_run = (act_info_ptr->update_in_progress == TRUE);

    MSG_GERAN_HIGH_5_G(GRM_TM_HDR" IRAT Reg Meas Client: %d, activity_can_run %d, "
                                 "Trans Id: %d index: %d duration: %d",
                       meas_client,
                       activity_can_run,
                       act_info_ptr->trans_id_act,
                       idx,
                       act_info_ptr->duration);
  }
  else
  {
    MSG_GERAN_ERROR_0_G(GRM_TM_HDR"ERROR: IRAT Activity Already in Progress!");
    activity_can_run = FALSE;
  }
  return activity_can_run;
}
/*===========================================================================

FUNCTION  grm_initialise_pm_denial_count

DESCRIPTION
  API sets the number of band tune denials for PM's that GL1 will live with before
  enabling persistent denial. This will be set based on DRX cycle and serving cell
  conditions.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_initialise_pm_denial_count(grm_client_enum_t client_id,
                                    gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if (grm_info_p)
  {
    if(grm_info_p->persistent_denial.meas_cycle == GRM_SLOW_MEAS_CYLE)
    {
      grm_info_p->persistent_denial.request_denied_count = 2;
    }
    else /*quick measurement cycle*/
    {
      grm_info_p->persistent_denial.request_denied_count = 1;
    }
  }
  return;
}
/*===========================================================================

FUNCTION grm_set_denied_band_in_next_mon_scan_flag

DESCRIPTION


PARAMS
  G band

DEPENDENCIES

RETURN VALUE
void

===========================================================================*/
void grm_set_denied_band_in_next_mon_scan_flag (sys_band_T        band,
                                                grm_client_enum_t client_id,
                                                gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if (grm_info_p && gl1_drdsds_enabled(gas_id) && band < INVALID_BAND)
  {
    grm_info_p->persistent_denial.denied_band_info[band].next_pch_block = TRUE;
  }
  return;
}

/*===========================================================================
 FUNCTION grm_clear_registered_bands

 DESCRIPTION
  At the end of a idle mon scan period clear the TRM registred bands. This is
  part of persistent denial mechanism. API should be called by idle power monitor
  call back APIs. e.g. l1_sci_monscan_rxlev_cb_gprs_idle

 PARAMS


 DEPENDENCIES

 RETURNS:

=============================================================================*/
void grm_clear_registered_bands(grm_client_enum_t client_id,
                                gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  int i;
  sys_band_T band = gl1_map_gsm_band_to_sys_band( gl1_get_serving_cell_arfcn(gas_id).band );

  if (grm_info_p )
  {
    for (i = 0; i < INVALID_BAND ; i++)
    {
      grm_info_p->persistent_denial.denied_band_info[i].band_registered = FALSE;
    }

    /*make sure serving band is always registered */
    if (band < INVALID_BAND)
    {
      grm_info_p->persistent_denial.denied_band_info[band].band_registered = TRUE;
    }
  }
  return;
}
/*===========================================================================
 FUNCTION grm_is_band_registered

 DESCRIPTION
  API to check if the G band has been registered with TRM.

 PARAMS


 DEPENDENCIES

 RETURNS:
 boolean TRUE is band is registered.

=============================================================================*/
boolean grm_is_band_registered(sys_band_T         band,
                               grm_client_enum_t  client_id,
                               gas_id_t           gas_id)
{
  grm_info_t *grm_info_p = NULL;
  boolean return_result = TRUE;
  ARFCN_T serving_cell = gl1_get_serving_cell_arfcn(gas_id);

  if (GERAN_ACCESS_STRATUM_ID_UNDEFINED != gas_id)
  {
    if(client_id == GRM_NO_CLIENT)
    {
      return_result = FALSE;
    }
  }

  if (return_result)
  {
    grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
    /*Ensure band reg check is only performed if DR is enabled. For SR we don't register bands
      This is to ensure inter band Power monitor skipping is not performed in SR mode*/
    /* when GL1 is doing power scan for background bplmn, we don't want to do band registration.
       Force the function to return TRUE
    */
    if ( grm_info_p && gl1_drdsds_enabled(gas_id) &&
         !( ( gpl1_get_multi_sim_bplmn_state(gas_id) == L1_MULTI_SIM_BGRND_HPLMN_RUNNING ) 
           &&(L1_get_current_background_HPLMN_search_activity(gas_id) == POWER_SCAN )
          )  
       )
    {
      /*serving(PCH) band is always registered  */
      if (serving_cell.band != band)
      {
        if ( (!grm_info_p->persistent_denial.denied_band_info[band].band_registered) ||
             ( grm_info_p->persistent_denial.denied_band_info[band].band_on_new_dev)
           )
        {
          /* Skip power monitor if band is allowed on a different device or if band is denied */
          MSG_GERAN_MED_3_G(GL1_DRDS_DBG_HDR"Serving Band is %d, G Band %d is not registered,or on_new_dev %d",
                            serving_cell.band,
                            band,
                            grm_info_p->persistent_denial.denied_band_info[band].band_on_new_dev);
          return_result = FALSE;
        }
      }
    }
    else if (!grm_info_p)
    {
      MSG_GERAN_ERROR_0_G("grm_is_band_registered: grm_info_p is NULL");
      return_result = FALSE;
    }
  }
  return return_result;
}
/*====================================================================
  FUNCTION:
    grm_tick_transaction_manager

   DESCRIPTION:
     Function releases Band Registration with TRM

   DEPENDENCIES:
     None

   RETURNS:
     None.
=======================================================================*/
void grm_tick_transaction_manager(grm_client_enum_t    client_id,
                                  gas_id_t             gas_id)
{
    gl1_sc_activities_t i;
    for (i = 0; i < SC_MAX_ACTIVITIES; i++)
    {
      grm_transaction_manager(GRM_TM_CHECK, i, client_id, gas_id);
    }
  }

/*===========================================================================

FUNCTION  grm_get_other_g_rf_device

DESCRIPTION
  Get the RF Device allocated to other G sub.

DEPENDENCIES
  None

RETURN VALUE
  RF device Id.

SIDE EFFECTS
  None

===========================================================================*/
rfm_device_enum_type grm_get_other_g_rf_device(gas_id_t          gas_id )
{
  rfm_device_enum_type rf_device = RFM_INVALID_DEVICE;
  gas_id_t other_g_gas_id = OTHER_G_SUB(gas_id);
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = NULL;

  if ( other_g_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED )
  {
    l1_tskisr_blk = &l1_tsk_buffer[other_g_gas_id];
    if(l1_tskisr_blk->client_id != GRM_NO_CLIENT)
    {
      rf_device = grm_get_rf_device(l1_tskisr_blk->client_id,
                                    other_g_gas_id);
    }
  }
  return rf_device;
}
/*===========================================================================
 FUNCTION grm_set_resource_lock_state

 DESCRIPTION
  This function invokes the TRM resource locking procedure used by RR
  to prevent interruption during protected procedures e.g. L1->GSM CSFB

 PARAMS
  enabled - TRUE stops release of held TRM resources
            FALSE allows release of held TRM resources

 DEPENDENCIES

 RETURN VALUE

=============================================================================*/
void grm_set_resource_lock_state( boolean state, gas_id_t gas_id )
{
  if (TRUE == state)
  {
    grm_set_resource_lock_enum(GRM_RESOURCE_LOCK_ACTIVE, gas_id);
  }
  else
  {
    grm_set_resource_lock_enum(GRM_RESOURCE_LOCK_INACTIVE, gas_id);
  }
}

/*===========================================================================
 FUNCTION grm_get_resource_lock_state

 DESCRIPTION
  This function returns the current state of the TRM resource lock procedure

 PARAMS
  none

 DEPENDENCIES

 RETURN VALUE
   TRUE - TRM resources are locked
   FALSE - TRM resources are not locked

=============================================================================*/
boolean grm_get_resource_lock_state( gas_id_t gas_id )
{
  return (GRM_RESOURCE_LOCK_ACTIVE == grm_resource_lock_state[gas_id]);
}

/*===========================================================================
 FUNCTION grm_set_resource_lock_enum

 DESCRIPTION
  This function invokes the TRM resource locking procedure used by RR
  to prevent interruption during protected procedures e.g. L1->GSM CSFB

 PARAMS
  state - GRM_RESOURCE_LOCK_INACTIVE
          GRM_RESOURCE_LOCK_ACTIVE
          GRM_RESOURCE_LOCK_GOING_INACTIVE

 DEPENDENCIES

 RETURN VALUE

=============================================================================*/
void grm_set_resource_lock_enum( grm_resource_lock_t state, gas_id_t gas_id )
{
  if (grm_resource_lock_state[gas_id] != state)
  {
    grm_resource_lock_state[gas_id] = state;
    MSG_GERAN_HIGH_1_G("TRM resource lock=%d",grm_resource_lock_state[gas_id]);
  }
}

/*===========================================================================
 FUNCTION grm_get_resource_lock_enum

 DESCRIPTION
  This function returns the current state of the TRM resource lock procedure

 PARAMS
  none

 DEPENDENCIES

 RETURN VALUE
  state - GRM_RESOURCE_LOCK_INACTIVE
          GRM_RESOURCE_LOCK_ACTIVE
          GRM_RESOURCE_LOCK_GOING_INACTIVE

=============================================================================*/
grm_resource_lock_t grm_get_resource_lock_enum( gas_id_t gas_id )
{
  return grm_resource_lock_state[gas_id];
}

/*===========================================================================

FUNCTION grm_register_additional_irat_bands

DESCRIPTION
         release existing G2L band registration
         registers next band for G2L during fast search

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/

boolean grm_register_additional_irat_bands( uint32 lte_band, uint32 num_frames, gas_id_t gas_id)
{
  boolean                lte_band_valid = FALSE ;

#ifdef FEATURE_GSM_TO_LTE
  sys_band_class_e_type  trm_band;

  grm_client_enum_t      client_id = l1_tsk_buffer[gas_id].client_id;

  grm_info_t           *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if(grm_info_p)
  {
     /*
     ** re-regsiter, release previous band registration else next fails and measurement is skipped
     */
     if(grm_info_p->grm_activity_buf[SC_IRAT][GRM_ACT_IRAT].update_in_progress)
     {
        grm_transaction_release(SC_IRAT, &grm_info_p->grm_activity_buf[SC_IRAT][GRM_ACT_IRAT], client_id, gas_id);
     }
  }
  else
  {
    MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"Null grm_info_p! Invalid client_id %d", client_id );
  }

  trm_band = lte_ml1_convert_lte_band_to_sys_band((sys_sband_lte_e_type)lte_band);

  lte_band_valid = grm_check_irat_band_reg( LTE_CLIENT_FOR_THIS_SUB(gas_id) , trm_band , num_frames, TRUE , gas_id );

#endif

  return (lte_band_valid);
}

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION grm_release_rx_sec

DESCRIPTION

  This function sends a TRM release for the secondary (diversity) rx chain
  of the client.
  This function is not client aware, the caller must ensure the correct diversity
  client is passed in the function argument.

  Note that this function gets called when L1 deactivates.

PARAMS
  None

DEPENDENCIES

RETURN VALUE
  boolean - TRUE = secondary receiver chain was released

===========================================================================*/
boolean grm_release_rx_sec(grm_client_enum_t client_id, gas_id_t gas_id)
{
#ifdef FEATURE_GSM_RX_DIVERSITY

  boolean release_status = TRUE;
  gl1_trm_state_t divrx_trm_status = GL1_TRM_NULL;

  if (GRM_NO_CLIENT != client_id)
  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if (grm_get_resource_lock_state(gas_id))
    {
      MSG_GERAN_HIGH_0_G( GL1_RXD"grm_release_rx_sec ignored - TRM resource lock is active" );
      release_status =  FALSE;
    }
    else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    {
      /* No reservation for the diversity chain so no need to clear TRM interface.
         Release the chain if granted.*/
      divrx_trm_status = grm_get_trm_status(client_id, gas_id);

      if ((GL1_TRM_GRANTED == divrx_trm_status)
          || (GL1_TRM_CALLBACK_PENDING == divrx_trm_status)
          || (GL1_TRM_RETAINED_FOR_ACCESS == divrx_trm_status)
        )
      {
        trm_release_chain_input_info *grm_release_chain_input_info_data_p;
        /* Always clear to avoid spurious data */
        trm_release_chain_input_info  grm_release_chain_input_info_data = { 0 };

        MSG_GERAN_LOW_1_G(GL1_RXD"gpl1_grm_release_rx_sec for client_id:%d",
                            client_id );

        grm_release_chain_input_info_data_p         =
          &grm_release_chain_input_info_data;

        grm_release_chain_input_info_data_p->client =
          grm_map_grm_client_id_to_trm_client_id(client_id);

        grm_release_chain_input_info_data_p->tag    = (trm_tag_type)GRM_RELEASE;

        MSG_GERAN_HIGH_2_G( GL1_RXD"GL1 call to trm_release_chain client_id:%d curr sclk:%d",
                            client_id, timetick_get_safe() );

        /* Release explicitly here calling underlying trm api */
        trm_release_chain( grm_release_chain_input_info_data_p );

        grm_reset_trm_reason_stat_unlck_and_others(client_id, gas_id);
      }/*if Rxd is granted*/
    }/*Else Resource lock isnt active*/
  }/*valid client id*/
#endif /* FEATURE_GSM_RX_DIVERSITY */

  return release_status;
}

/*===========================================================================

FUNCTION grm_reset_trm_reason_stat_unlck_and_others

DESCRIPTION

  This function resets the unlock cb data
       freq info
       trm status

PARAMS
  None

DEPENDENCIES

RETURN VALUE
  None

===========================================================================*/
void grm_reset_trm_reason_stat_unlck_and_others(grm_client_enum_t client_id, gas_id_t gas_id)
{
    grm_info_t *grm_info_p = NULL;
        grm_gl1_set_current_trm_reason( TRM_NUM_REASONS, SYS_PROC_TYPE_NONE, client_id, gas_id );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        /*Clear the freq_band_info*/
    MSG_GERAN_MED_0_G(GL1_DRDS_DBG_HDR"Clear grm freq_store");
        grm_clear_freq_info(client_id, gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

        grm_set_trm_status(GL1_TRM_NULL, client_id, gas_id);
      }
/*===========================================================================

FUNCTION grm_map_grm_client_id_to_trm_client_id

DESCRIPTION


PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
trm_client_enum_t grm_map_grm_client_id_to_trm_client_id(grm_client_enum_t grm_client_id)
{
  return (trm_client_enum_t)grm_client_id;
}
/*===========================================================================

FUNCTION grm_map_grm_client_id_to_trm_client_id

DESCRIPTION


PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
grm_client_enum_t grm_map_trm_client_id_to_grm_client_id(trm_client_enum_t trm_client_id)
{
  return (grm_client_enum_t)trm_client_id;
}
/*===========================================================================

FUNCTION grm_get_trm_band_from_g_pri

DESCRIPTION

Takes the priority and gas id and works out what TRM band will be used for activity.

PARAMS
l1_fm_priority_T pri
gas_id_t gas_id


DEPENDENCIES

RETURN VALUE
TRM band to be used for channel maintainence activity.

===========================================================================*/
sys_band_class_e_type grm_get_trm_band_from_g_pri (l1_fm_priority_T pri,gas_id_t gas_id)
{
  sys_band_class_e_type trm_band = SYS_BAND_CLASS_NONE;
  ARFCN_T ncell_arfcn;
  sys_band_T sc_band = SYS_BAND_NONE;
  l1_sc_wcdma_data_t        *wcdma_ptr     = &l1_sc_wcdma_data[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];  
  idle_data_T               *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
#ifdef FEATURE_WCDMA
  l1_freq_scan_band_enum_type w_band;
#endif
#ifdef FEATURE_GSM_TO_LTE
  lte_earfcn_t earfcn;
  sys_sband_lte_e_type  lte_band;
#endif
  ARFCN_T serving_cell;
    /* GSM serving cell */

  switch (pri)
  {
    case FM_PRI_CCCH:
    {
      serving_cell = gl1_get_serving_cell_arfcn(gas_id);
      trm_band=gl1_map_gsm_band_to_sys_band( serving_cell.band );
    }
    break;

    case FM_PRI_CBCH:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    case FM_PRI_MSIM_CBCH:
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    {
      if (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB)
      {
        trm_band = gl1_map_gsm_band_to_sys_band(gl1_get_serving_cell_arfcn(gas_id).band);
      }
      else
      {              
        trm_band = gl1_map_gsm_band_to_sys_band((idle_data_ptr->campedon_cell_data.cbch_channel_information.frequency_list.channel[0]).band);                
      }
    }
    break;

    case FM_PRI_NCELL_BCCH:
    {
      ncell_arfcn = get_next_bcch_arfcn(gas_id);
      sc_band = gl1_get_band(ncell_arfcn);
      trm_band = gl1_map_gsm_band_to_sys_band(sc_band);
    }
    break;

    case FM_PRI_NCELL_ACQ:
    {
      ncell_arfcn = get_next_fcb_arfcn(gas_id);
      sc_band = gl1_get_band(ncell_arfcn);
      trm_band = gl1_map_gsm_band_to_sys_band(sc_band);
    }
    break;

    case FM_PRI_NCELL_SCH:
    {
      ncell_arfcn = get_next_sch_arfcn(gas_id);
      sc_band = gl1_get_band(ncell_arfcn);
      trm_band = gl1_map_gsm_band_to_sys_band(sc_band);
    }
    break;

    case FM_PRI_WCDMA_IDSEARCH:
    {
#ifdef FEATURE_WCDMA
      w_band = wl1_trm_get_band(wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.UARFCN);
      trm_band = trm_map_l1_band_type[w_band].band;

      if(trm_band == SYS_BAND_CLASS_NONE)
      {
        MSG_GERAN_ERROR_2_G("trm_band == SYS_BAND_CLASS_NONE, uarfcn %d, index %d",
                            wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.UARFCN,
                            wcdma_ptr->wcdma_srch.curr_id_freq_offset);
      }
#endif /*FEATURE_WCDMA*/
    }
    break;

    case FM_PRI_WCDMA_RECONF:
    {
#ifdef FEATURE_WCDMA
      w_band = wl1_trm_get_band(wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_reconfirm_freq_offset].arfcn.UARFCN);
      trm_band = trm_map_l1_band_type[w_band].band;

      if(trm_band == SYS_BAND_CLASS_NONE)
      {
        MSG_GERAN_ERROR_2_G("trm_band == SYS_BAND_CLASS_NONE, uarfcn %d, index %d",
                            wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_reconfirm_freq_offset].arfcn.UARFCN,
                            wcdma_ptr->wcdma_srch.curr_reconfirm_freq_offset);
      }
#endif /*FEATURE_WCDMA*/
    }
    break;

#ifdef FEATURE_GSM_TDS
    case FM_PRI_TDS_ACQ:
    {
      if(wcdma_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
      {
         if (wcdma_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM)
         {
           trm_band = tdrm_get_band(wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_reconfirm_freq_offset].arfcn.UARFCN);

           if(trm_band == SYS_BAND_CLASS_NONE)
           {
              MSG_GERAN_ERROR_2_G("trm_band == SYS_BAND_CLASS_NONE, uarfcn %d, index %d",
                                   wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_reconfirm_freq_offset].arfcn.UARFCN,
                                   wcdma_ptr->wcdma_srch.curr_reconfirm_freq_offset);
           }
         }
         else
         {
           trm_band = tdrm_get_band(wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.UARFCN);

           if(trm_band == SYS_BAND_CLASS_NONE)
           {
             MSG_GERAN_ERROR_2_G("trm_band == SYS_BAND_CLASS_NONE, uarfcn %d, index %d",
                                  wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.UARFCN,
                                  wcdma_ptr->wcdma_srch.curr_id_freq_offset);
           }
         }
      }
      else
      {
        MSG_GERAN_ERROR_0_G("grm_get_trm_band_from_g_pri - search state is NULL ");
      }
    }
    break;
#endif /* FEATURE_GSM_TDS */

#ifdef FEATURE_GSM_TO_LTE
    case FM_PRI_LTE_IDSEARCH:
    {
      earfcn = wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.euarfcn;
      (void)lte_ml1_common_band_get_band_from_dl_earfcn(earfcn, &lte_band);
      trm_band = lte_ml1_convert_lte_band_to_sys_band(lte_band);

      if(trm_band == SYS_BAND_CLASS_NONE)
      {
        MSG_GERAN_ERROR_2_G("trm_band == SYS_BAND_CLASS_NONE, euarfcn %d, index %d",
                            wcdma_ptr->rr_params.freq_list[wcdma_ptr->wcdma_srch.curr_id_freq_offset].arfcn.euarfcn,
                            wcdma_ptr->wcdma_srch.curr_id_freq_offset);
      }
    }
    break;
#endif
    case FM_PRI_DL_TBF:
    {
         serving_cell = gl1_get_serving_cell_arfcn(gas_id);
         trm_band=gl1_map_gsm_band_to_sys_band( serving_cell.band );
    }
	break;
    default:
        MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR" Not a VALID DR-DSDS SC_ACTIVITY band check pri %d",pri);
      break;
   }
  return trm_band;
}
/*===========================================================================

FUNCTION     grm_map_prx_to_divrx_client_id

DESCRIPTION  PRX to DIVRX client id mapping

RETURN VALUE grm diversity client

===========================================================================*/
grm_client_enum_t grm_map_prx_to_divrx_client_id(grm_client_enum_t prx_client_id,
                                                 gas_id_t gas_id)
{
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
  grm_info_t *grm_info_p = NULL;

  switch (prx_client_id)
  {
  case GRM_GSM1:
  case GRM_GPRS1:
    divrx_client = GRM_GSM1_SECONDARY;
    break;

#ifdef FEATURE_DUAL_SIM
  case GRM_GSM2:
  case GRM_GPRS2:
    divrx_client = GRM_GSM2_SECONDARY;
    break;
#endif /*FEATURE_DUAL_SIM*/

  case GRM_IRAT:
  case GRM_IRAT2:
    break;

  default:
    MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"No Diversity client for client %d",
                        prx_client_id);
    break;
  }

  if(divrx_client != GRM_NO_CLIENT)
  {
     grm_info_p = grm_get_info_ptr_from_client(divrx_client,gas_id);

     /* Tag the DRX info with the associated Prx client
           this is used for syncing frequency information between prx and drx clients when requesting for Drx chain*/
     if(grm_info_p)
     {
       grm_info_p->prx_client = prx_client_id;
     }
  }
  return divrx_client;
}
#ifdef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  grm_configure_tx_info

DESCRIPTION
  Configure Tx data structure.

DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_configure_tx_info(grm_client_enum_t  client_id,
                           gas_id_t           gas_id)
{
#ifdef FEATURE_GSM_RX_DIVERSITY
  grm_client_enum_t divrx_client = GRM_NO_CLIENT;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  if (grm_info_p)
  {
    if (grm_info_p->tx_info_p == NULL)
    {
      grm_info_p->tx_info_p = &(grm_tx_info[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)]);
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"Null grm_info_p! Invalid client_id %d",
                        client_id );
  }

#ifdef FEATURE_GSM_RX_DIVERSITY
  /* For diversity client, no Tx info required.*/
  divrx_client = grm_map_prx_to_divrx_client_id(client_id, gas_id);
  grm_info_p = grm_get_info_ptr_from_client(divrx_client, gas_id);
  if (grm_info_p)
  {
    grm_info_p->tx_info_p = NULL;
  }
  else
  {
    MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"Null grm_info_p! Diversity client_id %d",
                        divrx_client );
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/

  grm_info_p = NULL;

#ifdef FEATURE_DUAL_SIM
  /* For bplmn client, no Tx info required.*/
  switch (client_id)
  {
  case GRM_GSM1:
  case GRM_GPRS1:
    grm_info_p = &grm_info[GERAN_BPLMN_SPACE_INDEX_1];
    break;

  case GRM_GSM2:
  case GRM_GPRS2:
    grm_info_p = &grm_info[GERAN_BPLMN_SPACE_INDEX_2];
    break;
  default:
    break;
  }

  if (grm_info_p)
  {
    grm_info_p->tx_info_p = NULL;
  }
#endif /*FEATURE_DUAL_SIM*/
  return;
}
/*===========================================================================

FUNCTION  grm_is_a_tx_client

DESCRIPTION
  Return true if client is a Tx Client.
DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_is_a_tx_client (grm_client_enum_t client_id)
{
  boolean ret_value = FALSE;

  if ((client_id == GRM_GSM1_TX) ||
      (client_id == GRM_GSM2_TX) ||
      (client_id == GRM_GPRS1_TX) ||
      (client_id == GRM_GPRS2_TX))
  {
    ret_value = TRUE;
  }
  return ret_value;
}
/*===========================================================================

FUNCTION     grm_map_rx_to_tx_client_id

DESCRIPTION  RX to Tx client id mapping

RETURN VALUE Tx Client Id

===========================================================================*/
grm_client_enum_t grm_map_rx_to_tx_client_id(grm_client_enum_t rx_client_id,
                                                 gas_id_t gas_id)
{
  grm_client_enum_t tx_client = GRM_NO_CLIENT;

  switch(rx_client_id)
  {
  case GRM_GSM1:
    tx_client = GRM_GSM1_TX;
    break;

  case GRM_GPRS1:
    tx_client = GRM_GPRS1_TX;
    break;

#ifdef FEATURE_DUAL_SIM
  case GRM_GSM2:
    tx_client = GRM_GSM2_TX;
    break;

  case GRM_GPRS2:
    tx_client = GRM_GPRS2_TX;
    break;

  case GRM_IRAT:
    tx_client = GRM_IRAT_TX;
    break;

  case GRM_IRAT2:
    tx_client = GRM_IRAT2_TX;
    break;
#endif /* FEATURE_DUAL_SIM */

#ifdef FEATURE_GSM_RX_DIVERSITY
  case GRM_GSM1_SECONDARY:
#ifdef FEATURE_DUAL_SIM
  case GRM_GSM2_SECONDARY:
#endif /*FEATURE_DUAL_SIM*/
    break;
#endif /* FEATURE_GSM_RX_DIVERSITY */

  default:
    MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"No Tx client for client %d",
                        rx_client_id);
    break;
  }
  return tx_client;
}

/*===========================================================================

FUNCTION     grm_map_tx_to_rx_client_id

DESCRIPTION  TX to Rx client id mapping

RETURN VALUE Rx Client Id

===========================================================================*/
grm_client_enum_t grm_map_tx_to_rx_client_id(grm_client_enum_t tx_client_id,
                                                 gas_id_t gas_id)
{
  grm_client_enum_t rx_client = GRM_NO_CLIENT;

  switch(tx_client_id)
  {
  case GRM_GSM1_TX :
    rx_client = GRM_GSM1;
    break;

#ifdef FEATURE_DUAL_SIM
  case GRM_GSM2_TX :
    rx_client = GRM_GSM2;
    break;

#endif /* FEATURE_DUAL_SIM */

  default:
    MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"No Rx client for client %d",
                        tx_client_id);
    break;
  }
  return rx_client;
}
/*===========================================================================

FUNCTION     grm_release_tx_chain

DESCRIPTION  Release Tx Chain

RETURN VALUE None

===========================================================================*/
void grm_release_tx_chain(grm_client_enum_t client_id, gas_id_t gas_id)
{
  trm_release_chain_input_info *grm_release_chain_input_info_data_p;

    /* Always clear to avoid spurious data */
  trm_release_chain_input_info  grm_release_chain_input_info_data = { 0 };

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if (grm_get_resource_lock_state(gas_id))
  {
    MSG_GERAN_HIGH_0_G( GL1_RXD"grm_release_tx_chain ignored - TRM resource lock is active" );
    return;
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  grm_release_chain_input_info_data_p         =
      &grm_release_chain_input_info_data;

  grm_release_chain_input_info_data_p->client =
                           grm_map_grm_client_id_to_trm_client_id(client_id);

  grm_release_chain_input_info_data_p->tag    = (trm_tag_type)GRM_RELEASE;


  MSG_GERAN_MED_2_G( GL1_DRDS_DBG_HDR"GL1 call to trm_release_chain client_id:%d curr sclk:%d",
                        client_id, timetick_get_safe() );

  /* Release explicitly here calling underlying trm api */
  trm_release_chain( grm_release_chain_input_info_data_p );

  grm_set_trm_status(GL1_TRM_NULL, client_id, gas_id);


  return;
}
/*===========================================================================

FUNCTION  grm_request_tx_chain

DESCRIPTION
  Forwards a GSM client request to the TRM layer. The reason for not calling
  grm_request for Tx Chain is grm_request modifies other variables which
  are not required by Tx chain.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
gl1_trm_state_t grm_request_tx_chain(grm_client_enum_t   client_id,
                                     trm_duration_t      duration,
                                     trm_reason_enum_t   reason,
                                     sys_proc_type_e_type sub_reason,
                                     trm_resource_enum_t resource,
                                     gas_id_t            gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk;

  trm_reason_enum_t req_reason = reason;
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_request_input_info *gl1_trm_request_input_info_p = NULL;
  trm_grant_return_enum_type grm_request_grant = TRM_RETURN_MAX;
    /* Always clear to avoid spurious data */
  trm_request_input_info  gl1_trm_request_input_info = { 0 };
  gl1_trm_state_t trm_status = grm_get_trm_status(client_id, gas_id);

  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if ( !grm_info_p || ( gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED ) )
  {
    MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"grm_request_rx_sec with no valid grm_info for client_id:%d grant:%d",
                         client_id, grm_request_grant );

    return ( GL1_TRM_NULL );
  }

  GL1_TRM_LOCK();

  if ( ( trm_status == GL1_TRM_GRANTED ) ||
       ( trm_status == GL1_TRM_RETAINED_FOR_ACCESS ) )
  {
    grm_change_priority( client_id, reason, sub_reason, gas_id );

    GL1_TRM_UNLOCK();

    return ( grm_get_trm_status(client_id, gas_id) );
  }

  gl1_trm_request_input_info_p = &gl1_trm_request_input_info;

  gl1_trm_request_input_info_p->client_id = grm_map_grm_client_id_to_trm_client_id(client_id);
  gl1_trm_request_input_info_p->resource  = resource;
  gl1_trm_request_input_info_p->duration  = duration;
  gl1_trm_request_input_info_p->reason    = req_reason;
  gl1_trm_request_input_info_p->sub_reason    = sub_reason;
  gl1_trm_request_input_info_p->freq_info = grm_info_p->grm_freq_info;

  gl1_trm_request_input_info_p->tag       = (trm_tag_type)GRM_REQUEST;

  /* forward the request to TRM */
  trm_request( gl1_trm_request_input_info_p, &grm_info_p->grm_request_data );

  /* Detect tag mismatch */
  if ( grm_info_p->grm_request_data.tag != gl1_trm_request_input_info_p->tag )
  {
    MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"grm_request_tx_chain Tag mismatch in:%d out:%d",
                           gl1_trm_request_input_info_p->tag,
                           grm_info_p->grm_request_data.tag );
  }

  grm_request_grant = grm_info_p->grm_request_data.grant;

  /* Only return rf device info if not denied */
  if ( grm_request_grant == TRM_GRANTED )
  {
    MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"MS: grm_request_tx_chain reserve_at_count:%d granted chain:%d ",
                        reserve_at_count[gas_id], grm_request_grant );
    trm_status = GL1_TRM_GRANTED;
  }
  else
  {
    MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"MS: grm_request_tx_chain grm_request_tx_chain:%d grm_request_grant:%d",
                        reserve_at_count[gas_id], grm_request_grant );
  }

  grm_set_trm_status(trm_status, client_id, gas_id);

  GL1_TRM_UNLOCK();

  return ( trm_status );
} /* grm_request_tx_chain */
/*===========================================================================

FUNCTION  grm_async_grouped_cb_event_handler

DESCRIPTION
  Grouped async call-back to communicate from TRM to L1 on any async operation
  requested by the L1s

DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
void grm_async_grouped_cb_event_handler(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p )
{
  int i = 0;
  boolean partial_grant_or_unlock = FALSE;
  uint8 tx_client_idx = TRM_GROUPED_CLIENTS_MAX;
  uint8 drx_client_idx = TRM_GROUPED_CLIENTS_MAX;
  grm_client_enum_t grm_client_id =  GRM_NO_CLIENT;
  gas_id_t  gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
  volatile ISRTIM_CMD_BLK        *l1_tskisr_blk = NULL;

  if (grm_async_grouped_cb_event_data_p)
  {
    /*One to one mapping between callback type and callback. For one callback type
      there may be multiple clients*/
    /*Check for Partial Grant or Partial Release*/

    partial_grant_or_unlock = grm_is_partial_grant_or_unlock(grm_async_grouped_cb_event_data_p);

    MSG_GERAN_HIGH_2(GL1_DRDS_DBG_HDR" grm_async_grouped_cb_event_handler num_of_entries %d partial_grant_or_unlock %d ",
                     grm_async_grouped_cb_event_data_p->num_of_entries, partial_grant_or_unlock);

    if (!partial_grant_or_unlock)
    {
      /* TRM ensures synchronous calls of callbacks. */
      /*First process Tx client callback*/
      for (i = 0;
           (i < grm_async_grouped_cb_event_data_p->num_of_entries) && (i < TRM_GROUPED_CLIENTS_MAX);
           i++)
      {
        grm_client_id = grm_map_trm_client_id_to_grm_client_id(
                    grm_async_grouped_cb_event_data_p->data[i].client);
        if (grm_is_a_tx_client(grm_client_id))
        {
          tx_client_idx = i;
          MSG_GERAN_HIGH_2(GL1_DRDS_DBG_HDR" grm_async_grouped_cb_event_handler tx_client_idx %d "
                                           "grm_client_id %d",
                           tx_client_idx, grm_client_id);
        }
        if (grm_is_a_secondary_client(grm_client_id))
        {
          drx_client_idx = i;
          MSG_GERAN_HIGH_2(GL1_DRDS_DBG_HDR" grm_async_grouped_cb_event_handler drx_client_idx %d "
                                           "grm_client_id %d",
                           drx_client_idx, grm_client_id);
        }
      }

      if (tx_client_idx != TRM_GROUPED_CLIENTS_MAX)
      {
        grm_async_cb_event_handler(&(grm_async_grouped_cb_event_data_p->data[tx_client_idx]));
      }

      if (drx_client_idx != TRM_GROUPED_CLIENTS_MAX)
      {
        grm_async_cb_event_handler(&(grm_async_grouped_cb_event_data_p->data[drx_client_idx]));
      }

      for (i = 0;
           (i < grm_async_grouped_cb_event_data_p->num_of_entries) && (i < TRM_GROUPED_CLIENTS_MAX);
           i++)
      {
        if ((i != tx_client_idx) && (i != drx_client_idx))
        {
          grm_client_id = grm_map_trm_client_id_to_grm_client_id(
                    grm_async_grouped_cb_event_data_p->data[i].client);

          MSG_GERAN_LOW_2(GL1_DRDS_DBG_HDR" grm_async_grouped_cb_event_handler rx_client_idx %d "
                                           "grm_client_id %d",
                           i, grm_client_id);
          grm_async_cb_event_handler(&(grm_async_grouped_cb_event_data_p->data[i]));
        }
      }
    }
    else
    {
      /* GL1 receives partial grant */
      grm_client_id = grm_map_trm_client_id_to_grm_client_id(
                        grm_async_grouped_cb_event_data_p->data[0].client);

      gas_id          = gl1_map_client_id_to_gas_id( grm_client_id );
      gas_id          = check_gas_id(gas_id);
      l1_tskisr_blk   = &l1_tsk_buffer[gas_id];

      if ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_DECOUPLED ) 
      {
        /* If GSM receives partial grant for RACH, disable DR mode. This is a workaround otherwise GL1
           may stuck in the state waiting for TRM lock forever.CR929775
        */
        MSG_GERAN_HIGH_1(GL1_DRDS_DBG_HDR"Partial grant for RACH,fallback to SR client_id %d",grm_client_id);
        call_in_task_grm_enable_disable_dual_receive(  l1_tskisr_blk->client_id, FALSE);
      }
    }
  }
  else
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"grm_async_grouped_cb_event_handler no valid "
                                       "grm_async_grouped_cb_event_data_p" );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_is_partial_grant_or_unlock

DESCRIPTION


DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
boolean grm_is_partial_grant_or_unlock(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p )
{
  boolean partial_grant_or_unlock = FALSE;
  trm_async_callback_type_enum_t async_cb_type = TRM_ASYNC_EVENT_MAX;

  if (grm_async_grouped_cb_event_data_p)
  {
    /*Get Callback type.*/
    if (grm_async_grouped_cb_event_data_p->num_of_entries != 0)
    {
      async_cb_type = grm_async_grouped_cb_event_data_p->data[0].cb_type;
    }

    switch (async_cb_type)
    {
    case TRM_REQUEST_AND_NOTIFY_ASYNC_INFO:
      partial_grant_or_unlock = grm_validate_request_and_notify_cb(grm_async_grouped_cb_event_data_p);
      break;

    case TRM_REQUEST_AND_NOTIFY_ENH_ASYNC_INFO:
      partial_grant_or_unlock = grm_validate_request_and_notify_enh_cb(grm_async_grouped_cb_event_data_p);
      break;

    case TRM_MODIFY_CHAIN_STATE_ASYNC_INFO:
      partial_grant_or_unlock = grm_validate_modify_chain_cb(grm_async_grouped_cb_event_data_p);
      break;

    case TRM_UNLOCK_CHAIN_ASYNC_INFO:
      partial_grant_or_unlock = grm_validate_unlock_chain_cb(grm_async_grouped_cb_event_data_p);
      break;

    case TRM_DUAL_RECEIVE_INFO:
    case TRM_CONNECTED_MODE_HOP_ASYNC_INFO:
    case TRM_QTA_ASYNC_INFO:
      break;

    default:
       MSG_GERAN_ERROR_1( GL1_DRDS_DBG_HDR"Unhandled Grouped TRM Async CB type:%d",
                          async_cb_type);
      break;
    }
  }
  else
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"grm_is_partial_grant_or_unlock no valid "
                                       "grm_async_grouped_cb_event_data_p" );
  }


  return partial_grant_or_unlock;
}
/*===========================================================================

FUNCTION  grm_validate_request_and_notify_cb

DESCRIPTION


DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
boolean grm_validate_request_and_notify_cb(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p )
{
  boolean                        partial_grant = FALSE;
  gas_id_t                       gas_id;
  trm_client_enum_t              async_cb_client;
  grm_client_enum_t              grm_client_id = GRM_NO_CLIENT;
  grm_client_enum_t              rx_client_id = GRM_NO_CLIENT;
  grm_client_enum_t              tx_client_id = GRM_NO_CLIENT;
  grm_client_enum_t              drx_client_id = GRM_NO_CLIENT;
  trm_grant_return_enum_type     rx_trm_grant_status = TRM_DENIAL;
  trm_grant_return_enum_type     tx_trm_grant_status = TRM_DENIAL;
  grm_info_t                     *grm_info_p = NULL;
  volatile ISRTIM_CMD_BLK        *l1_tskisr_blk = NULL;
  trm_reason_enum_t              reason = TRM_NUM_REASONS;
  int                            i = 0;
  grm_rx_tx_status_t             rx_tx_status = GRM_RX_TX_NULL;
  uint8                          rx_client_idx = TRM_GROUPED_CLIENTS_MAX;
  uint8                          tx_client_idx = TRM_GROUPED_CLIENTS_MAX;
  uint8                          drx_client_idx = TRM_GROUPED_CLIENTS_MAX;


  if (grm_async_grouped_cb_event_data_p)
  {
    if (grm_async_grouped_cb_event_data_p->num_of_entries != 0)
    {
      /*Get first Client Information*/
      async_cb_client = grm_async_grouped_cb_event_data_p->data[0].client;
      grm_client_id   = grm_map_trm_client_id_to_grm_client_id(async_cb_client);

      gas_id          = gl1_map_client_id_to_gas_id( grm_client_id );
      gas_id          = check_gas_id(gas_id);

      l1_tskisr_blk   = &l1_tsk_buffer[gas_id];
      rx_client_id    = l1_tskisr_blk->client_id;

      grm_info_p      = grm_get_info_ptr_from_client(rx_client_id, gas_id);

      rx_tx_status = grm_get_rx_tx_status(rx_client_id, gas_id);

      if (GRM_RX_TX_REQUESTED == rx_tx_status)
      {
        tx_client_id    = grm_map_rx_to_tx_client_id(rx_client_id, gas_id);
        drx_client_id   = grm_map_prx_to_divrx_client_id(rx_client_id, gas_id);

        for (i = 0;
             (i < grm_async_grouped_cb_event_data_p->num_of_entries) && (i < TRM_GROUPED_CLIENTS_MAX);
             i++)
        {
          if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(rx_client_id))
          {
            rx_trm_grant_status = grm_async_grouped_cb_event_data_p->data[i].data.chain_req_n_not_info.grant;
            rx_client_idx = i;
          }
          if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(tx_client_id))
          {
            tx_trm_grant_status = grm_async_grouped_cb_event_data_p->data[i].data.chain_req_n_not_info.grant;
            tx_client_idx = i;
          }
          if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(drx_client_id))
          {
            drx_client_idx = i;
          }
        }

        MSG_GERAN_MED_5_G(GL1_DRDS_DBG_HDR"grm_validate_request_and_notify_cb num %d "
                                          "RX Client %d Grant %d "
                                          "Tx Client %d Grant %d",
                          grm_async_grouped_cb_event_data_p->num_of_entries,
                          rx_client_id, rx_trm_grant_status,
                          tx_client_id, tx_trm_grant_status);

        if ((TRM_GROUPED_CLIENTS_MAX == rx_client_idx) && (TRM_GROUPED_CLIENTS_MAX == tx_client_idx))
        {
          /*Received call back for DRX, Not for PRX and TX. Ignore it. Ignore by individual callback
            as well.*/
            MSG_GERAN_HIGH_0_G("No CB for Prx/Tx Chain - Dont consider as Partial Grant");
        }
        else 
        if ((TRM_GRANTED != rx_trm_grant_status) || (TRM_GRANTED != tx_trm_grant_status))
        {
          /*Partial Grant*/
          partial_grant = TRUE;
          grm_set_rx_tx_status(GRM_PARTIAL_GRANT, rx_client_id, gas_id);
          reason = grm_gl1_get_current_trm_reason(rx_client_id, gas_id);

          /*treat partial grant as complete denial access timer
            will handle access retry attempts */
          call_in_task_grm_release_in_callback(rx_client_id);
        }
        else
        {
          grm_set_rx_tx_status(GRM_COMPLETE_GRANT, rx_client_id, gas_id);
        }
      }
      else if (GRM_PARTIAL_GRANT == rx_tx_status)
      {
        /*In the middle of handling partial grant.
          Scenario:  Requested Rx and Tx. Received Rx. Set rx_tx_status
          to GRM_PARTIAL_GRANT. Call grm_release in task context. Before releasing
          the chain, received Tx callback. Ignore it.
          Assumption: TRM will not send callback after releasing the chain.*/
        MSG_GERAN_MED_0_G(GL1_DRDS_DBG_HDR"Received callback during the handling of partial"
                                          "grant");
        partial_grant = TRUE;
      }
    }
  }
  else
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"grm_validate_request_and_notify_cb no valid "
                                       "grm_async_grouped_cb_event_data_p" );
  }

  return partial_grant;
}
/*===========================================================================

FUNCTION  grm_validate_request_and_notify_enh_cb

DESCRIPTION


DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
boolean grm_validate_request_and_notify_enh_cb(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p )
{
  boolean                        partial_grant = FALSE;
  gas_id_t                       gas_id;
  trm_client_enum_t              async_cb_client;
  grm_client_enum_t              grm_client_id = GRM_NO_CLIENT;
  grm_client_enum_t              rx_client_id = GRM_NO_CLIENT;
  grm_client_enum_t              tx_client_id = GRM_NO_CLIENT;
  grm_client_enum_t              drx_client_id = GRM_NO_CLIENT;
  trm_grant_return_enum_type     rx_trm_grant_status = TRM_DENIAL;
  trm_grant_return_enum_type     tx_trm_grant_status = TRM_DENIAL;
  grm_info_t                     *grm_info_p = NULL;
  volatile ISRTIM_CMD_BLK        *l1_tskisr_blk = NULL;
  grm_tag_t                      grm_tag = GRM_TAG_MAX;
  trm_reason_enum_t              reason = TRM_NUM_REASONS;
  sys_proc_type_e_type           sub_reason = SYS_PROC_TYPE_NONE;
  int                            i = 0;
  grm_rx_tx_status_t             rx_tx_status = GRM_RX_TX_NULL;
  uint8                          rx_client_idx = TRM_GROUPED_CLIENTS_MAX;
  uint8                          tx_client_idx = TRM_GROUPED_CLIENTS_MAX;
  uint8                          drx_client_idx = TRM_GROUPED_CLIENTS_MAX;

  if (grm_async_grouped_cb_event_data_p)
  {
    if (grm_async_grouped_cb_event_data_p->num_of_entries != 0)
    {
      /*Get first Client Information*/
      async_cb_client = grm_async_grouped_cb_event_data_p->data[0].client;
      grm_client_id   = grm_map_trm_client_id_to_grm_client_id(async_cb_client);

      gas_id          = gl1_map_client_id_to_gas_id( grm_client_id );
      gas_id          = check_gas_id(gas_id);

      l1_tskisr_blk   = &l1_tsk_buffer[gas_id];
      rx_client_id    = l1_tskisr_blk->client_id;

      grm_info_p      = grm_get_info_ptr_from_client(rx_client_id, gas_id);

      rx_tx_status = grm_get_rx_tx_status(rx_client_id, gas_id);

      if (GRM_RX_TX_REQUESTED == rx_tx_status)
      {
        tx_client_id    = grm_map_rx_to_tx_client_id(rx_client_id, gas_id);
        drx_client_id   = grm_map_prx_to_divrx_client_id(rx_client_id, gas_id);

        for (i = 0;
             (i < grm_async_grouped_cb_event_data_p->num_of_entries) && (i < TRM_GROUPED_CLIENTS_MAX);
             i++)
        {
          if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(rx_client_id))
          {
            rx_trm_grant_status = grm_async_grouped_cb_event_data_p->data[i].data.chain_req_n_not_enh_info.grant;
            grm_tag = grm_async_grouped_cb_event_data_p->data[i].tag;
            rx_client_idx = i;
          }
          if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(tx_client_id))
          {
            tx_trm_grant_status = grm_async_grouped_cb_event_data_p->data[i].data.chain_req_n_not_enh_info.grant;
            tx_client_idx = i;
          }
          else if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(drx_client_id))
          {
            drx_client_idx = i;
          }          

        }

        MSG_GERAN_MED_5_G(GL1_DRDS_DBG_HDR"grm_validate_request_and_notify_enh_cb num %d "
                                          "RX Client %d Grant %d "
                                          "Tx Client %d Grant %d",
                          grm_async_grouped_cb_event_data_p->num_of_entries,
                          rx_client_id, rx_trm_grant_status,
                          tx_client_id, tx_trm_grant_status);

        if ((TRM_GROUPED_CLIENTS_MAX == rx_client_idx) && (TRM_GROUPED_CLIENTS_MAX == tx_client_idx))
        {
          /*Received call back for DRX, Not for PRX and TX. Ignore it. Ignore by individual callback
            as well.*/
            MSG_GERAN_HIGH_0_G("No CB for Prx/Tx Chain - Dont consider as Partial Grant");
        }
        else if ((TRM_GRANTED != rx_trm_grant_status) || (TRM_GRANTED != tx_trm_grant_status))
        {
          /*Partial Grant*/
          partial_grant = TRUE;
          grm_set_rx_tx_status(GRM_PARTIAL_GRANT, rx_client_id, gas_id);
          reason = grm_gl1_get_current_trm_reason(rx_client_id, gas_id);
          sub_reason = grm_gl1_get_trm_sub_reason(rx_client_id, gas_id);

          /*treat partial grant as complete denial access timer
            will handle access retry attempts */
          call_in_task_grm_release_in_callback(rx_client_id);
        }
        else
        {
          grm_set_rx_tx_status(GRM_COMPLETE_GRANT, rx_client_id, gas_id);
        }
      }
      else if (GRM_PARTIAL_GRANT == rx_tx_status)
      {
        /*In the middle of handling partial grant.
          Scenario:  Requested Rx and Tx. Received Rx. Set rx_tx_status
          to GRM_PARTIAL_GRANT. Call grm_release in task context. Before releasing
          the chain, received Tx callback. Ignore it.
          Assumption: TRM will not send callback after releasing the chain.*/
        MSG_GERAN_MED_0_G(GL1_DRDS_DBG_HDR"Received callback during the handling of partial"
                                          "grant");
        partial_grant = TRUE;
      }
    }
  }
  else
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"grm_validate_request_and_notify_enh_cb no valid "
                                       "grm_async_grouped_cb_event_data_p" );
  }
  return partial_grant;
}
/*===========================================================================

FUNCTION  grm_validate_modify_chain_cb

DESCRIPTION


DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
boolean grm_validate_modify_chain_cb(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p )
{
  boolean                               partial_grant = FALSE;
  gas_id_t                              gas_id;
  trm_client_enum_t                     async_cb_client;
  grm_client_enum_t                     grm_client_id = GRM_NO_CLIENT;
  grm_client_enum_t                     rx_client_id = GRM_NO_CLIENT;
  grm_client_enum_t                     tx_client_id = GRM_NO_CLIENT;
  trm_grant_return_enum_type            rx_trm_grant_status = TRM_DENIAL;
  trm_grant_return_enum_type            tx_trm_grant_status = TRM_DENIAL;
  grm_info_t                            *grm_info_p = NULL;
  volatile ISRTIM_CMD_BLK               *l1_tskisr_blk = NULL;
  trm_async_callback_information_type   trm_async_cb =  {0};
  uint8                                   i = 0, rx_client_idx = TRM_GROUPED_CLIENTS_MAX;

  if (grm_async_grouped_cb_event_data_p)
  {
    if (grm_async_grouped_cb_event_data_p->num_of_entries != 0)
    {
      /*Get first Client Information*/
      async_cb_client = grm_async_grouped_cb_event_data_p->data[0].client;
      grm_client_id   = grm_map_trm_client_id_to_grm_client_id(async_cb_client);

      gas_id          = gl1_map_client_id_to_gas_id( grm_client_id );
      gas_id          = check_gas_id(gas_id);

      l1_tskisr_blk   = &l1_tsk_buffer[gas_id];
      rx_client_id    = l1_tskisr_blk->client_id;

      grm_info_p      = grm_get_info_ptr_from_client(rx_client_id, gas_id);

      if (GRM_RX_TX_REQUESTED == grm_get_rx_tx_status(rx_client_id, gas_id))
      {
        tx_client_id    = grm_map_rx_to_tx_client_id(rx_client_id, gas_id);

        for (i = 0;
             (i < grm_async_grouped_cb_event_data_p->num_of_entries) && (i < TRM_GROUPED_CLIENTS_MAX);
             i++)
        {
          if (TRM_MODIFY_REASON == grm_async_grouped_cb_event_data_p->data[i].data.chain_modify_state_info.modify_type)
          {
            if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(rx_client_id))
            {
              rx_trm_grant_status = grm_async_grouped_cb_event_data_p->data[i].data.chain_modify_state_info.modify_info.mod_reason_info.result;
              rx_client_idx = i;
            }
            if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(tx_client_id))
            {
              tx_trm_grant_status = grm_async_grouped_cb_event_data_p->data[i].data.chain_modify_state_info.modify_info.mod_reason_info.result;
            }
          }
        }

        MSG_GERAN_MED_5_G(GL1_DRDS_DBG_HDR"grm_validate_modify_chain_cb num %d "
                                          "RX Client %d Grant %d "
                                          "Tx Client %d Grant %d",
                          grm_async_grouped_cb_event_data_p->num_of_entries,
                          rx_client_id, rx_trm_grant_status,
                          tx_client_id, tx_trm_grant_status);

        if ((TRM_GRANTED != rx_trm_grant_status) || (TRM_GRANTED != tx_trm_grant_status))
        {
          /*Partial Grant*/
          partial_grant = TRUE;
          grm_set_rx_tx_status(GRM_PARTIAL_GRANT, rx_client_id, gas_id);
          /*If any one is not granted, treat Rx as Denial*/
          if (rx_client_idx != TRM_GROUPED_CLIENTS_MAX)
          {
            trm_async_cb = grm_async_grouped_cb_event_data_p->data[rx_client_idx];
            trm_async_cb.data.chain_modify_state_info.modify_info.mod_reason_info.result = TRM_DENIAL;
            grm_async_cb_event_handler(&trm_async_cb);
          }
          else
          {
            MSG_GERAN_ERROR_0_G(GL1_DRDS_DBG_HDR"Invalid Rx client Idx");
          }
        }
        else
        {
          grm_set_rx_tx_status(GRM_COMPLETE_GRANT, rx_client_id, gas_id);
        }
      }
    }
  }
  else
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"grm_validate_modify_chain_cb no valid "
                                       "grm_async_grouped_cb_event_data_p" );
  }
  return partial_grant;
}
/*===========================================================================

FUNCTION  grm_validate_unlock_chain_cb

DESCRIPTION
  Handle PRX, TX and DRX unlock callback.
  GL1 doesn't support unlock callback for DRX. On receiving unlock for PRX,
  GL1 release/handles PRX and DRX.

DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
boolean grm_validate_unlock_chain_cb(
   trm_grouped_async_callback_information_type *grm_async_grouped_cb_event_data_p )
{
  boolean                               partial_unlock = FALSE;
  gas_id_t                              gas_id;
  trm_client_enum_t                     async_cb_client;
  grm_client_enum_t                     grm_client_id = GRM_NO_CLIENT;
  grm_client_enum_t                     rx_client_id = GRM_NO_CLIENT;
  grm_client_enum_t                     tx_client_id = GRM_NO_CLIENT;
  grm_client_enum_t                     drx_client_id = GRM_NO_CLIENT;
  trm_client_enum_t                     trm_client_id = TRM_NO_CLIENT;
  grm_info_t                            *grm_info_p = NULL;
  volatile ISRTIM_CMD_BLK               *l1_tskisr_blk = NULL;
  trm_async_callback_information_type   trm_async_cb =  {0};
  uint8                                 i = 0;
  uint8                                 rx_client_idx = TRM_GROUPED_CLIENTS_MAX;
  uint8                                 tx_client_idx = TRM_GROUPED_CLIENTS_MAX;
  uint8                                 drx_client_idx = TRM_GROUPED_CLIENTS_MAX;

  if (grm_async_grouped_cb_event_data_p)
  {
    if (grm_async_grouped_cb_event_data_p->num_of_entries != 0)
    {
      /*Get first Client Information*/
      async_cb_client = grm_async_grouped_cb_event_data_p->data[0].client;
      grm_client_id   = grm_map_trm_client_id_to_grm_client_id(async_cb_client);

      gas_id          = gl1_map_client_id_to_gas_id( grm_client_id );
      gas_id          = check_gas_id(gas_id);

      l1_tskisr_blk   = &l1_tsk_buffer[gas_id];
      rx_client_id    = l1_tskisr_blk->client_id;

      grm_info_p      = grm_get_info_ptr_from_client(rx_client_id, gas_id);

      if ( grm_get_resource_lock_state(gas_id))
      {
        MSG_GERAN_HIGH_0_G("unlock_chain_cb ignored due to resource lock");
      }
      else if (GRM_COMPLETE_GRANT == grm_get_rx_tx_status(rx_client_id, gas_id))
      {
        /* Holding PRX, TX and/or DRX.
           Could receive callback for PRX, DRX and TX. Ignore DRX callback.*/
        tx_client_id    = grm_map_rx_to_tx_client_id(rx_client_id, gas_id);
        drx_client_id   = grm_map_prx_to_divrx_client_id(rx_client_id, gas_id);

        for (i = 0;
             (i < grm_async_grouped_cb_event_data_p->num_of_entries) && (i < TRM_GROUPED_CLIENTS_MAX);
              i++)
        {
          if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(rx_client_id))
          {
            rx_client_idx = i;
          }
          else if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(tx_client_id))
          {
            tx_client_idx = i;
          }
          else if (grm_async_grouped_cb_event_data_p->data[i].client == grm_map_grm_client_id_to_trm_client_id(drx_client_id))
          {
            drx_client_idx = i;
          }
        }

        MSG_GERAN_MED_5_G(GL1_DRDS_DBG_HDR"grm_validate_unlock_chain_cb num %d "
                                          "RX Client %d Rx Idx %d "
                                          "Tx Client %d Tx Idx %d",
                          grm_async_grouped_cb_event_data_p->num_of_entries,
                          rx_client_id, rx_client_idx,
                          tx_client_id, tx_client_idx);

        if (TRM_GROUPED_CLIENTS_MAX != drx_client_id)
        {
          MSG_GERAN_MED_2_G(GL1_DRDS_DBG_HDR"grm_validate_unlock_chain_cb DRX Client %d DRx Idx %d ",
                            drx_client_id, drx_client_idx);
        }

        if ((TRM_GROUPED_CLIENTS_MAX == rx_client_idx) && (TRM_GROUPED_CLIENTS_MAX == tx_client_idx))
        {
          /*Received call back for DRX, Not for PRX and TX. Ignore it. Ignore by individual callback
            as well.*/
        }
        else
        {
          /* Holding PRX, TX and/or DRX.
             Received unlock callback. PRX+TX+DRX, PRX+TX, PRX+DRX, DRX+TX, PRX, TX.
             Ignore DRX callback in individual callback handler.*/
        if ((TRM_GROUPED_CLIENTS_MAX != rx_client_idx) && (TRM_GROUPED_CLIENTS_MAX != tx_client_idx))
        {
          grm_set_rx_tx_status(GRM_COMPLETE_UNLOCK, rx_client_id, gas_id);
        }
        else if (rx_client_idx != TRM_GROUPED_CLIENTS_MAX)
        {
          /*Rx Unlock*/
          partial_unlock = TRUE;
          trm_async_cb = grm_async_grouped_cb_event_data_p->data[rx_client_idx];
        }
        else if (tx_client_idx != TRM_GROUPED_CLIENTS_MAX)
        {
          partial_unlock = TRUE;
          trm_async_cb = grm_async_grouped_cb_event_data_p->data[tx_client_idx];
        }

        /*If either Rx or TX Unlock received, treat Unlock for RX*/
        if (partial_unlock)
        {
          grm_set_rx_tx_status(GRM_PARTIAL_UNLOCK, rx_client_id, gas_id);
          trm_client_id = grm_map_grm_client_id_to_trm_client_id(rx_client_id);
          trm_async_cb.client = trm_client_id;
          trm_async_cb.data.unlock_info.unlock_client = trm_client_id;
          grm_async_cb_event_handler(&trm_async_cb);
        }
      }
    }
      /* For PRX+DRX callback, individual callback handler will handle. */
    }
  }
  else
  {
    MSG_GERAN_ERROR_0( GL1_DRDS_DBG_HDR"grm_validate_unlock_chain_cb no valid "
                                       "grm_async_grouped_cb_event_data_p" );
  }
  return partial_unlock;
}
/*===========================================================================

FUNCTION  grm_set_rx_tx_status

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
void grm_set_rx_tx_status(grm_rx_tx_status_t          status,
                          grm_client_enum_t           client_id,
                          gas_id_t                    gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
    grm_info_p->rx_tx_status = status;
  }
  else
  {
    MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"Null grm_info_p! Invalid client_id %d",
                        client_id );
  }
  return;
}
/*===========================================================================

FUNCTION  grm_get_rx_tx_status

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
  trm reason

SIDE EFFECTS
  None

===========================================================================*/
grm_rx_tx_status_t grm_get_rx_tx_status(grm_client_enum_t client_id,
                                        gas_id_t          gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  grm_rx_tx_status_t status = GRM_RX_TX_NULL;

  if ( grm_info_p )
  {
    status = grm_info_p->rx_tx_status;
  }
  else
  {
    MSG_GERAN_ERROR_1_G(GL1_DRDS_DBG_HDR"Null grm_info_p! Invalid client_id %d",
                        client_id );
  }
  return status;
}
/*===========================================================================

FUNCTION  grm_handle_tx_grant_callback

DESCRIPTION
  On TX grant callback, save grant status, Rf device and set trm status
  so that Rx grant callback can use them. RX grant callback is the main one
  which configures other paramneters and decides further actions.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_handle_tx_grant_callback(grm_client_enum_t          client,
                                  trm_grant_return_enum_type grant_status,
                                  gas_id_t                   gas_id )
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client, gas_id);
  rfm_device_enum_type rf_device = RFM_INVALID_DEVICE;

  if (grm_info_p)
  {
    /*Save TRM grant status*/
    grm_set_trm_grant_status(grant_status, client, gas_id);

    if (gl1_drds_debug(gas_id))
    {
      MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR "grm_handle_tx_grant_callback client %d grant %d",
                         client, grant_status);
    }

    if (TRM_GRANTED == grant_status)
    {
      /*Get Rf device*/
      rf_device = grm_get_rf_device( client, gas_id );

      if (RFM_INVALID_DEVICE != rf_device)
      {
        /*Save what is returned from TRM*/
        gl1_hw_set_gsm_rf_id(rf_device, client, FALSE, gas_id);
        grm_set_trm_status(GL1_TRM_GRANTED, client, gas_id);
      }
      else
      {
        grm_set_trm_status(GL1_TRM_NOT_GRANTED, client, gas_id);
      }
    }
    else
    {
      grm_set_trm_status(GL1_TRM_NOT_GRANTED, client, gas_id);
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", client);
  }
  return;
}

/*===========================================================================

FUNCTION  grm_grouped_trm_request

DESCRIPTION
  Forwards a GSM client request to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS


===========================================================================*/
trm_grant_return_enum_type grm_grouped_trm_request(
                                        grm_client_enum_t             client_id,
                                                   trm_duration_t      duration,
                                                   trm_reason_enum_t   reason,
                                                   sys_proc_type_e_type sub_reason,
                                                   trm_resource_enum_t resource,
                                        grm_divrx_request_type        req_type,
                                        grm_grouped_request_bitmap_t  req_bitmap,
                                                   gas_id_t            gas_id)
{
  trm_grant_return_enum_type grant = TRM_DENIAL;

  grm_client_enum_t rx_grm_client_id = client_id;
  trm_client_enum_t rx_trm_client_id = TRM_NO_CLIENT;
  trm_grant_return_enum_type rx_grant = TRM_DENIAL;
  uint8 rx_grant_idx = TRM_GROUPED_CLIENTS_MAX;
  uint8 drx_grant_idx = TRM_GROUPED_CLIENTS_MAX;

  grm_client_enum_t tx_grm_client_id = GRM_NO_CLIENT;
  trm_client_enum_t tx_trm_client_id = TRM_NO_CLIENT;
  trm_grant_return_enum_type tx_grant = TRM_DENIAL;
  uint8 tx_grant_idx = TRM_GROUPED_CLIENTS_MAX;

  grm_client_enum_t divrx_grm_client_id = GRM_NO_CLIENT;
  trm_client_enum_t divrx_trm_client_id = TRM_NO_CLIENT;
  trm_grant_return_enum_type divrx_grant = TRM_DENIAL;
  trm_tuneaway_enum_type     pri_ta_info = TRM_TUNEAWAY_NONE;

  uint8  i = 0;
  uint32 j;

  rfm_device_enum_type rf_device = RFM_INVALID_DEVICE;

  trm_grouped_request_input_info  *gl1_trm_grouped_request_input_info_p = NULL;
  trm_grouped_request_return_data *gl1_trm_grouped_request_return_data_p = NULL;

  /* For each SUB, seperate memory for ISR and Task to avoid data corruption.
     Circular buffer of dimension 2 (ISR and Task)*/
  static trm_grouped_request_input_info
    gl1_trm_grouped_request_input_info[GRM_ISR_TASK_DATA_SPACE][NUM_GERAN_DATA_SPACES];
  static trm_grouped_request_return_data
    gl1_trm_grouped_request_return_data[GRM_ISR_TASK_DATA_SPACE][NUM_GERAN_DATA_SPACES];
  /* Index to find next circular buffer*/
  static uint8 circular_buff_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
  uint8 buf_idx = 0;
  boolean non_valid_freq_info = FALSE;

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  gl1_hw_rxdiv_data_t     *rxdiv_data_ptr = gl1_hw_get_gsm_rxdiv_data_ptr(gas_id);

  if (grm_info_p)
  {
    /*Find Circular Buffer Idx*/
    gl1_grm_info_mutex_lock(gas_id);
    buf_idx = circular_buff_idx[gas_id]++;
    circular_buff_idx[gas_id] %= GRM_ISR_TASK_DATA_SPACE;
    gl1_grm_info_mutex_unlock(gas_id);

    gl1_trm_grouped_request_input_info_p = &gl1_trm_grouped_request_input_info[buf_idx][gas_id];
    gl1_trm_grouped_request_return_data_p = &gl1_trm_grouped_request_return_data[buf_idx][gas_id];

    memset(&gl1_trm_grouped_request_input_info[buf_idx][gas_id], 0, sizeof(trm_grouped_request_input_info));
    memset(&gl1_trm_grouped_request_return_data[buf_idx][gas_id], 0, sizeof(trm_grouped_request_return_data));

    /* grm_request fills frequency information. But in a scenario(TRM denial -> TRM Request for
       ACCESS/TRAFFIC, clears frequency information*/
    non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

    /*First Fill Rx*/
    i = gl1_trm_grouped_request_input_info_p->num_of_entries = 0;

    if (req_bitmap.prxRequest)
    {
    gl1_trm_grouped_request_input_info_p->request_info[i].client_id =
                                                     rx_trm_client_id = grm_map_grm_client_id_to_trm_client_id(client_id);
      gl1_trm_grouped_request_input_info_p->request_info[i].resource = gl1_ms_get_trm_resource( gas_id, reason );
    gl1_trm_grouped_request_input_info_p->request_info[i].duration = duration;
    gl1_trm_grouped_request_input_info_p->request_info[i].reason = reason;
    gl1_trm_grouped_request_input_info_p->request_info[i].sub_reason = sub_reason;
    gl1_trm_grouped_request_input_info_p->request_info[i].freq_info = grm_info_p->grm_freq_info;
    gl1_trm_grouped_request_input_info_p->request_info[i].tag = (trm_tag_type)GRM_REQUEST;

#ifdef FEATURE_QTA
    if ((IS_QTA_REASON(reason, sub_reason) &&
       (!l1_is_fm_trm_priority_g2t(gas_id)) &&
      (duration <= (gpl1_multi_sim_convert_frames_to_sclcks(L1_QTA_DURATION_GAP_MAX))))
        ||
         (!IS_X_PLUS_G() &&
         ( l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_IDLE ) && 
         IS_GBTA_REASON(reason, sub_reason)))
    {
      gl1_trm_grouped_request_input_info_p->request_info[i].ta_info = TRM_QUICK_TUNEAWAY;  
	  pri_ta_info = TRM_QUICK_TUNEAWAY;
    }
    else
#endif
    {
      gl1_trm_grouped_request_input_info_p->request_info[i].ta_info = TRM_LONG_TUNEAWAY;
	  pri_ta_info = TRM_LONG_TUNEAWAY;
	}

    i++;
    }/*If Prx Request*/

    if (req_bitmap.txRequest)
    {
    /*Fill TX*/
    tx_grm_client_id = grm_map_rx_to_tx_client_id(client_id, gas_id);

    gl1_trm_grouped_request_input_info_p->request_info[i].client_id =
                                                   tx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(tx_grm_client_id);

    gl1_trm_grouped_request_input_info_p->request_info[i].resource = TRM_TX_BEST;
    gl1_trm_grouped_request_input_info_p->request_info[i].duration = duration;
    gl1_trm_grouped_request_input_info_p->request_info[i].reason = reason;
    gl1_trm_grouped_request_input_info_p->request_info[i].sub_reason = sub_reason;
    gl1_trm_grouped_request_input_info_p->request_info[i].freq_info = grm_info_p->grm_freq_info;
    gl1_trm_grouped_request_input_info_p->request_info[i].tag = (trm_tag_type)GRM_REQUEST;

    i++;
    }

    /* Diversity chain is requested if PRX is granted. Assume PRX is granted for grouped request*/
    if (req_bitmap.drxRequest)
    {
      /*Fill DRX*/
      divrx_grm_client_id = grm_map_prx_to_divrx_client_id(rx_grm_client_id, gas_id);

        gl1_trm_grouped_request_input_info_p->request_info[i].client_id =
                                                       divrx_trm_client_id =
            grm_map_grm_client_id_to_trm_client_id(divrx_grm_client_id);

        gl1_trm_grouped_request_input_info_p->request_info[i].resource = TRM_RX_DIVERSITY;
        gl1_trm_grouped_request_input_info_p->request_info[i].duration = duration;
        gl1_trm_grouped_request_input_info_p->request_info[i].reason = TRM_DIVERSITY;
        gl1_trm_grouped_request_input_info_p->request_info[i].sub_reason = SYS_PROC_TYPE_DIVERSITY_CHAIN;
        gl1_trm_grouped_request_input_info_p->request_info[i].freq_info = grm_info_p->grm_freq_info;
        gl1_trm_grouped_request_input_info_p->request_info[i].tag = (trm_tag_type)GRM_REQUEST;
        gl1_trm_grouped_request_input_info_p->request_info[i].ta_info = pri_ta_info;

        i++;
      }

    gl1_trm_grouped_request_input_info_p->num_of_entries = i;

    /* forward the request to TRM */
    trm_grouped_request( gl1_trm_grouped_request_input_info_p,
                         gl1_trm_grouped_request_return_data_p );

    /* Process output. Find Rx, Tx and DivRx grant index*/
    for (i = 0;
         (i < gl1_trm_grouped_request_return_data_p->num_of_entries) && (i < TRM_GROUPED_CLIENTS_MAX) ;
         i++)
    {
      if (gl1_trm_grouped_request_return_data_p->return_info[i].client_id == rx_trm_client_id)
      {
        rx_grant_idx = i;
        rx_grant = gl1_trm_grouped_request_return_data_p->return_info[i].grant;
      }
      else if (gl1_trm_grouped_request_return_data_p->return_info[i].client_id == tx_trm_client_id)
      {
        tx_grant_idx = i;
        tx_grant = gl1_trm_grouped_request_return_data_p->return_info[i].grant;
        grm_set_trm_grant_status(tx_grant, tx_grm_client_id, gas_id);
      }
      else if (gl1_trm_grouped_request_return_data_p->return_info[i].client_id == divrx_trm_client_id)
      {
        divrx_grant = gl1_trm_grouped_request_return_data_p->return_info[i].grant;

        grm_set_trm_grant_status(divrx_grant, divrx_grm_client_id, gas_id);

        if (divrx_grant != TRM_DENIAL)
        {
          drx_grant_idx = i;

          grm_set_trm_status(GL1_TRM_GRANTED, divrx_grm_client_id, gas_id);

          rf_device = grm_get_rf_device(divrx_grm_client_id,gas_id);

          /*Update local stored trm_reason*/
          grm_gl1_set_current_trm_reason(TRM_DIVERSITY,
                                         SYS_PROC_TYPE_DIVERSITY_CHAIN,
                                         divrx_grm_client_id,
                                         gas_id);

          if (rf_device != RFM_INVALID_DEVICE)
          {
            gl1_hw_set_gsm_rf_id(rf_device, divrx_grm_client_id, TRUE, gas_id);

            rxdiv_data_ptr->gl1_hw_DivRx_band_supported = 0;

            grm_info_p->grm_request_data.num_bands = gl1_trm_grouped_request_return_data_p->return_info[drx_grant_idx].num_bands;
            for (j = 0; (j < grm_info_p->grm_request_data.num_bands) && (j < MAX_BAND_INFO_GRM); j++)
            {
              rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[j].band = gl1_trm_grouped_request_return_data_p->return_info[drx_grant_idx].band_alloc[j].band.band;
              rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[j].device = rf_device;
              rxdiv_data_ptr->gl1_hw_DivRx_band_supported |= gl1_hw_map_sys_band_to_gsm_rxdiv_band(rxdiv_data_ptr->gl1_hw_trm_rf_devices_DivRx[j].band,gas_id);
            }
          }

          MSG_GERAN_MED_2_G(GL1_RXD"granted RX_SEC trm grant %d rf device %d",
                            divrx_grant, rf_device);
        }
        else
        {
          MSG_GERAN_MED_1_G (GL1_RXD"denied RX_SEC trm grant %d",
                             divrx_grant);
          grm_set_trm_status(GL1_TRM_NULL, divrx_grm_client_id, gas_id);
          rxdiv_data_ptr->gl1_hw_DivRx_band_supported = 0;
        }
      }
    }

    MSG_GERAN_MED_3_G(GL1_DRDS_DBG_HDR"grm_grouped_trm_request num %d "
                                          "RX Client %d Rx Grant %d ",
                          gl1_trm_grouped_request_return_data_p->num_of_entries,
                          rx_trm_client_id, rx_grant);

    if (req_bitmap.txRequest)
    {
      MSG_GERAN_MED_2_G(GL1_DRDS_DBG_HDR"grm_grouped_trm_request "
                                          "Tx Client %d Tx Grant %d",
                          tx_trm_client_id, tx_grant);
    }

    /* Status: Granted (G), Denied/Not Requested (D)
       PRX Status, Tx Status, DRX status
       Case 1: GGG, Case 2: DDD, Case 3: GGD, Case 4 GDG
       Case 5: DGG, Case 6: GDD, Case 7: DGD, Case 8: DDG*/
    if ((TRM_GRANTED == rx_grant) && (TRM_GRANTED == tx_grant))
    {
      grant = TRM_GRANTED;

      /*Save return result for RX*/
      grm_info_p->grm_request_data.grant = gl1_trm_grouped_request_return_data_p->return_info[rx_grant_idx].grant;
      grm_info_p->grm_request_data.tag = gl1_trm_grouped_request_return_data_p->return_info[rx_grant_idx].tag;
      grm_info_p->grm_request_data.tune_info = gl1_trm_grouped_request_return_data_p->return_info[rx_grant_idx].tune_info;
      grm_info_p->grm_request_data.num_bands = gl1_trm_grouped_request_return_data_p->return_info[rx_grant_idx].num_bands;

      for (i = 0; (i < grm_info_p->grm_request_data.num_bands) && (i < MAX_BAND_INFO); i++)
      {
        grm_info_p->grm_request_data.band_alloc[i] = gl1_trm_grouped_request_return_data_p->return_info[rx_grant_idx].band_alloc[i];
      }

      /*Save Tx Info. Rx info will handled by parent function.*/
      /*update RF device on each trm request either grant or denial. */
      rf_device  = grm_get_rf_device(tx_grm_client_id, gas_id );

      /*update internal HW device ID*/
      gl1_hw_set_gsm_rf_id (rf_device, tx_grm_client_id, FALSE, gas_id);

      if (RFM_INVALID_DEVICE != rf_device)
      {
        /*Save what is returned from TRM*/
        gl1_hw_set_gsm_rf_id(rf_device, tx_grm_client_id, FALSE, gas_id);
        grm_set_trm_status(GL1_TRM_GRANTED, tx_grm_client_id, gas_id);
      }
      else
      {
        /*Tx status is granted but device is Invalid. Treat as Denial*/
        grant = TRM_DENIAL;
        tx_grant = TRM_DENIAL;
        grm_set_trm_status(GL1_TRM_NOT_GRANTED, tx_grm_client_id, gas_id);
      }

      /*Update local stored trm_reason*/
      grm_gl1_set_current_trm_reason(reason, sub_reason, tx_grm_client_id, gas_id);

      /*Case 1: GGG Case 3: GGD
        If DRX is granted use it, otherwise don't use*/
    }
    else if ((TRM_GRANTED == rx_grant) && (TRM_GRANTED != tx_grant))
    {
      /* Case 4 GDG, Case 6: GDD*/
      /*If Tx Requested, then it's a Partial Grant. Release Rx, DRX.
        else in case of RX and DRX, If DRX is granted use it, otherwise don't use */
      if (req_bitmap.txRequest)
      {
      grm_release(rx_grm_client_id, gas_id);
    }
      else
      {
        /*PRX+DRX*/
        grant = TRM_GRANTED;
      }
    }
    else if ((TRM_GRANTED != rx_grant) && (TRM_GRANTED == tx_grant))
    {
      /*Case 5: DGG,  Case 7: DGD*/
      /*Partial Grant. Release allocated one and treat Rx grant as denial*/
      grm_set_trm_status(GL1_TRM_GRANTED, tx_grm_client_id, gas_id);
      grm_release_tx_chain(tx_grm_client_id, gas_id);
       /* Rx not granted, clear DRX*/
      grm_release_rx_sec(divrx_grm_client_id, gas_id);

    }
    else
    {
      /* Case 2: DDD, Case 8: DDG*/
      /* Rx not granted, clear DRX*/
      MSG_GERAN_HIGH_2_G("Not Granted Rx Grant %d Rx Index %d",
                           rx_grant, rx_grant_idx);

      if (req_bitmap.txRequest)
      {
        MSG_GERAN_HIGH_2_G("Not Granted Tx Grant %d Tx Index %d",
                           tx_grant, tx_grant_idx);
      }

      grm_release_rx_sec(divrx_grm_client_id, gas_id);
    }

    if ( non_valid_freq_info )
    {
      grm_clear_freq_info( rx_grm_client_id, gas_id );
    }

    if ( non_valid_freq_info )
    {
      grm_clear_freq_info( divrx_grm_client_id, gas_id );
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", client_id);
  }
  return grant;
}
/*===========================================================================

FUNCTION  grm_grouped_request_and_notify

DESCRIPTION
  Forwards a GSM request and notify to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
void grm_grouped_request_and_notify(grm_client_enum_t   client_id,
                                           trm_duration_t      duration,
                                           trm_reason_enum_t   reason,
                                           sys_proc_type_e_type  sub_reason,
                                           grm_divrx_request_type  req_type,
                                           grm_tag_t           grm_tag,
                                           gas_id_t            gas_id)
{
  boolean     non_valid_freq_info;
  int i = 0;

  /* For each SUB, seperate memory for ISR and Task to avoid data corruption.
     Circular buffer of dimension 2 (ISR and Task)*/
  static trm_grouped_request_notify_input_info
    grm_grouped_request_notify_input_info[GRM_ISR_TASK_DATA_SPACE][NUM_GERAN_DATA_SPACES];
  /* Index to find next circular buffer*/
  static uint8 circular_buff_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
  uint8 buf_idx = 0;

  grm_client_enum_t tx_grm_client_id = GRM_NO_CLIENT;
  grm_client_enum_t rx_grm_client_id = client_id;

  trm_client_enum_t tx_trm_client_id = TRM_NO_CLIENT;
  trm_client_enum_t rx_trm_client_id = TRM_NO_CLIENT;
  grm_client_enum_t divrx_grm_client_id = GRM_NO_CLIENT;
  trm_client_enum_t divrx_trm_client_id = TRM_NO_CLIENT;
  grm_info_t *divrx_grm_info_p = NULL;
  

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

  if ( grm_info_p )
  {
    trm_grouped_request_notify_input_info *grm_grouped_request_notify_input_info_p = NULL;

    /*Find Circular Buffer Idx*/
    gl1_grm_info_mutex_lock(gas_id);
    buf_idx = circular_buff_idx[gas_id]++;
    circular_buff_idx[gas_id] %= GRM_ISR_TASK_DATA_SPACE;
    gl1_grm_info_mutex_unlock(gas_id);

    grm_grouped_request_notify_input_info_p = &grm_grouped_request_notify_input_info[buf_idx][gas_id];
    memset(&grm_grouped_request_notify_input_info[buf_idx][gas_id], 0, sizeof(trm_grouped_request_notify_input_info));

    
    i = grm_grouped_request_notify_input_info_p->num_of_entries = 0;

#ifdef FEATURE_GSM_RX_TX_SPLIT
    if ( grm_tag != GRM_RAN_MODIFY_TO_TRAFFIC_REASON  )
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {

      /*First Fill Rx*/
    grm_grouped_request_notify_input_info_p->request_info[i].client_id =
                                                   rx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(rx_grm_client_id);

    grm_grouped_request_notify_input_info_p->request_info[i].resource = gl1_ms_get_trm_resource( gas_id, reason );
    grm_grouped_request_notify_input_info_p->request_info[i].duration = duration;
    grm_grouped_request_notify_input_info_p->request_info[i].reason = reason;
    grm_grouped_request_notify_input_info_p->request_info[i].sub_reason = sub_reason;
    grm_grouped_request_notify_input_info_p->request_info[i].freq_info = grm_info_p->grm_freq_info;
    grm_grouped_request_notify_input_info_p->request_info[i].tag = (trm_tag_type)grm_tag;
    grm_grouped_request_notify_input_info_p->request_info[i].ta_info = TRM_LONG_TUNEAWAY;

    /*update local store with RnN reason*/
    grm_gl1_set_current_trm_reason(reason, sub_reason, rx_grm_client_id, gas_id);

    grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, rx_grm_client_id, gas_id);


    i++;
    }

    /*Fill TX*/
    tx_grm_client_id = grm_map_rx_to_tx_client_id(rx_grm_client_id, gas_id);

    grm_grouped_request_notify_input_info_p->request_info[i].client_id =
                                                   tx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(tx_grm_client_id);

    grm_grouped_request_notify_input_info_p->request_info[i].resource = TRM_TX_BEST;
    grm_grouped_request_notify_input_info_p->request_info[i].duration = duration;
    grm_grouped_request_notify_input_info_p->request_info[i].reason = reason;
    grm_grouped_request_notify_input_info_p->request_info[i].sub_reason = sub_reason;
    grm_grouped_request_notify_input_info_p->request_info[i].freq_info = grm_info_p->grm_freq_info;
    grm_grouped_request_notify_input_info_p->request_info[i].tag = (trm_tag_type)grm_tag;
    grm_grouped_request_notify_input_info_p->request_info[i].ta_info = TRM_LONG_TUNEAWAY;

#ifdef FEATURE_GSM_RX_TX_SPLIT 
    if ( grm_tag != GRM_RAN_MODIFY_TO_TRAFFIC_REASON  )
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    {
 	  grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, tx_grm_client_id, gas_id);
    }

    i++;
    /*Fill DRX*/
    
    /* Diversity chain is requested */
    if (req_type != GRM_DIVRX_TRM_REQ_INVALID )
    {
      /*Fill DRX*/
      divrx_grm_client_id = grm_map_prx_to_divrx_client_id(rx_grm_client_id, gas_id);

      divrx_grm_info_p = grm_get_info_ptr_from_client(divrx_grm_client_id, gas_id);

      if (divrx_grm_info_p)
      {
        non_valid_freq_info = grm_check_valid_freq_info(divrx_grm_info_p, gas_id);

        divrx_trm_client_id = grm_map_grm_client_id_to_trm_client_id(divrx_grm_client_id);

        grm_grouped_request_notify_input_info_p->request_info[i].client_id = divrx_trm_client_id;
        grm_grouped_request_notify_input_info_p->request_info[i].resource = TRM_RX_DIVERSITY;
        grm_grouped_request_notify_input_info_p->request_info[i].duration = duration;
        grm_grouped_request_notify_input_info_p->request_info[i].reason = TRM_DIVERSITY;
        grm_grouped_request_notify_input_info_p->request_info[i].sub_reason = SYS_PROC_TYPE_DIVERSITY_CHAIN;
        grm_grouped_request_notify_input_info_p->request_info[i].freq_info = divrx_grm_info_p->grm_freq_info;
        grm_grouped_request_notify_input_info_p->request_info[i].tag = (trm_tag_type)grm_tag;
        grm_grouped_request_notify_input_info_p->request_info[i].ta_info = TRM_LONG_TUNEAWAY;

        i++;
        /*set callback pending for the DRX client also*/
        grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, divrx_grm_client_id, gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", divrx_grm_client_id);
      }
    }


    grm_grouped_request_notify_input_info_p->num_of_entries = i;

    MSG_GERAN_HIGH_3_G( "L1 call to grm_grouped_request_and_notify  dur:%d reason:%d grm_tag:%d",
                         (int)duration, (int)reason, grm_tag );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /*Call band release before request and notify call to TRM*/
    grm_send_band_release_if_required(client_id, gas_id);
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

    grm_set_rx_tx_status(GRM_RX_TX_REQUESTED, rx_grm_client_id, gas_id);

    /* Request the lock now for this using the asynchronous TRM interface */
    trm_grouped_request_and_notify( grm_grouped_request_notify_input_info_p );
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", client_id);
  }

  if ( non_valid_freq_info )
  {
    grm_clear_freq_info( client_id, gas_id);
  }

  return;
}


/*===========================================================================

FUNCTION  grm_grouped_enhanced_request_and_notify

DESCRIPTION
  Forwards a GSM request and notify to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS

===========================================================================*/
void grm_grouped_enhanced_request_and_notify(grm_client_enum_t   client_id,
                                             trm_resource_enum_t resource,
                                             trm_duration_t      duration,
                                             trm_reason_enum_t   reason,
                                             sys_proc_type_e_type sub_reason,
                                             grm_tag_t           grm_tag,
                                             gas_id_t            gas_id)
{
  boolean     non_valid_freq_info;
  int i = 0;

  /* For each SUB, seperate memory for ISR and Task to avoid data corruption.
     Circular buffer of dimension 2 (ISR and Task)*/
  static trm_grouped_request_notify_enh_input_info
    gl1_grouped_trm_request_notify_enh_input_info[GRM_ISR_TASK_DATA_SPACE][NUM_GERAN_DATA_SPACES];
  /* Index to find next circular buffer*/
  static uint8 circular_buff_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
  uint8 buf_idx = 0;

  grm_client_enum_t tx_grm_client_id = GRM_NO_CLIENT;
  grm_client_enum_t rx_grm_client_id = client_id;

  trm_client_enum_t tx_trm_client_id = TRM_NO_CLIENT;
  trm_client_enum_t rx_trm_client_id = TRM_NO_CLIENT;

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  non_valid_freq_info = grm_check_valid_freq_info( grm_info_p, gas_id );

  if ( grm_info_p )
  {
    trm_grouped_request_notify_enh_input_info *gl1_trm_grouped_request_notify_enh_input_info_p = NULL;

    /*Find Circular Buffer Idx*/
    gl1_grm_info_mutex_lock(gas_id);
    buf_idx = circular_buff_idx[gas_id]++;
    circular_buff_idx[gas_id] %= GRM_ISR_TASK_DATA_SPACE;
    gl1_grm_info_mutex_unlock(gas_id);

    gl1_trm_grouped_request_notify_enh_input_info_p =
      &gl1_grouped_trm_request_notify_enh_input_info[buf_idx][gas_id];

    memset(&gl1_grouped_trm_request_notify_enh_input_info[buf_idx][gas_id],
           0,
           sizeof(trm_grouped_request_notify_enh_input_info));

    grm_set_req_and_notify_duration(duration, rx_grm_client_id, gas_id);

    /*update local store with RnN reason*/
    grm_gl1_set_current_trm_reason(reason, sub_reason, rx_grm_client_id, gas_id);

    /*First Fill Rx*/
    i = gl1_trm_grouped_request_notify_enh_input_info_p->num_of_entries = 0;

    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].client_id =
                                                   rx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(rx_grm_client_id);

    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].resource = gl1_ms_get_trm_resource( gas_id, reason );
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].duration = duration;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].reason = reason;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].sub_reason = sub_reason;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].freq_info = grm_info_p->grm_freq_info;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].tag = (trm_tag_type)grm_tag;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].unlock_tag = (trm_tag_type)grm_tag;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].ta_info = TRM_LONG_TUNEAWAY;
	grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, rx_grm_client_id, gas_id);

    i++;

    /*Fill TX*/
    tx_grm_client_id = grm_map_rx_to_tx_client_id(rx_grm_client_id, gas_id);

    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].client_id =
                                                   tx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(tx_grm_client_id);

    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].resource = TRM_TX_BEST;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].duration = duration;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].reason = reason;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].sub_reason = sub_reason;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].freq_info = grm_info_p->grm_freq_info;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].tag = (trm_tag_type)grm_tag;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].unlock_tag = (trm_tag_type)grm_tag;
    gl1_trm_grouped_request_notify_enh_input_info_p->req_not_info[i].ta_info = TRM_LONG_TUNEAWAY;
	grm_set_trm_status(GL1_TRM_CALLBACK_PENDING, tx_grm_client_id, gas_id);

    i++;

    gl1_trm_grouped_request_notify_enh_input_info_p->num_of_entries = i;

    MSG_GERAN_HIGH_4_G( "L1 call to grm_grouped_enhanced_request_and_notify reason:%d client:%d dur:%d grm_tag:%d",
                        (int)reason, (int)client_id, (int)duration, grm_tag );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /*Call Band Release before request and notify to TRM*/
    grm_send_band_release_if_required(client_id, gas_id);
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

    grm_set_rx_tx_status(GRM_RX_TX_REQUESTED, rx_grm_client_id, gas_id);

    trm_grouped_request_and_notify_enhanced( gl1_trm_grouped_request_notify_enh_input_info_p );
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", client_id);
  }

  if ( non_valid_freq_info )
  {
    grm_clear_freq_info( client_id, gas_id );
  }

  return;
}

/*===========================================================================

FUNCTION  grm_grouped_retain_chain_lock

DESCRIPTION
  Retain lock for a particular DR-DSDS client.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_grouped_retain_chain_lock(grm_client_enum_t     client_id,
                                   grm_unlock_callback_t unlock_callback,
                                   grm_grouped_request_bitmap_t   req_bitmap,
                                   gas_id_t              gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  int i = 0;

  grm_client_enum_t tx_grm_client_id = GRM_NO_CLIENT;
  grm_client_enum_t rx_grm_client_id = client_id;
  grm_client_enum_t drx_grm_client_id = GRM_NO_CLIENT;

  trm_client_enum_t tx_trm_client_id = TRM_NO_CLIENT;
  trm_client_enum_t rx_trm_client_id = TRM_NO_CLIENT;
  trm_client_enum_t drx_trm_client_id = TRM_NO_CLIENT;

  trm_grouped_retain_lock_input_info *grouped_retain_lock_input_info_p;

  /* Always clear to avoid spurious data */
  trm_grouped_retain_lock_input_info  grouped_retain_lock_input_info = { 0 };
  grm_info_t *grm_divrx_info_p;

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(rx_grm_client_id, gas_id);
  trm_reason_enum_t prx_reason = grm_gl1_get_current_trm_reason(rx_grm_client_id, gas_id);

  grouped_retain_lock_input_info_p             = &grouped_retain_lock_input_info;

  i = grouped_retain_lock_input_info_p->num_of_entries = 0;

  if (req_bitmap.prxRequest)
  {

  grouped_retain_lock_input_info_p->retain_info[i].client =
                                         rx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(rx_grm_client_id);


  grouped_retain_lock_input_info_p->retain_info[i].tag = (trm_tag_type)GRM_RETAIN_LOCK;
  grouped_retain_lock_input_info_p->retain_info[i].unlock_tag = (trm_tag_type)GRM_RETAIN_LOCK;


  i++;
  }

  if (req_bitmap.txRequest)
  {
   /*Fill TX*/
   tx_grm_client_id = grm_map_rx_to_tx_client_id(rx_grm_client_id, gas_id);


   grouped_retain_lock_input_info_p->retain_info[i].client =
                                          tx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(tx_grm_client_id);

  grouped_retain_lock_input_info_p->retain_info[i].tag = (trm_tag_type)GRM_RETAIN_LOCK;
  grouped_retain_lock_input_info_p->retain_info[i].unlock_tag = (trm_tag_type)GRM_RETAIN_LOCK;

  i++;
  }

  if (req_bitmap.drxRequest)
  {
   /*Fill DRX*/
    drx_grm_client_id = grm_map_prx_to_divrx_client_id(rx_grm_client_id, gas_id);


    grouped_retain_lock_input_info_p->retain_info[i].client =
                                          drx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(drx_grm_client_id);

    grouped_retain_lock_input_info_p->retain_info[i].tag = (trm_tag_type)GRM_RETAIN_LOCK;
    grouped_retain_lock_input_info_p->retain_info[i].unlock_tag = (trm_tag_type)GRM_RETAIN_LOCK;

    i++;
    /*install drx unlock cb handler if we're in transfer mode or cs registration DRX can now be unlocked individually  */
    /* No need to install drx unlock callback handler if GL1 state is multisim idle mode though trm reason is BG traffic
       reason is that with TA in Reorg mode feature, GL1 changes the trm reason to BG traffic in Page reorg mode and in multisim idle mode no need to unlcok DRX only */
    grm_divrx_info_p = grm_get_info_ptr_from_client(drx_grm_client_id, gas_id);
    if(((prx_reason == TRM_BG_TRAFFIC)&& !(l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)) ||
       prx_reason == TRM_ACCESS || 
       prx_reason == TRM_TRAFFIC)
    {      
      if(grm_divrx_info_p)
      {
       grm_divrx_info_p->grm_unlock_callback = (grm_unlock_callback_t)&grm_rx_diversity_unlock_cb;
      }

      MSG_GERAN_HIGH_1_G( GL1_DRDS_DBG_HDR"Install drx unlock callback client:%d ", drx_grm_client_id);
    }
    else
    {
      if(grm_divrx_info_p)
      {
        grm_divrx_info_p->grm_unlock_callback = unlock_callback;
      }
    }
  }

  grouped_retain_lock_input_info_p->num_of_entries = i;


  if ( gl1_drds_debug( gas_id ) )
  {
    MSG_GERAN_HIGH_2_G( GL1_DRDS_DBG_HDR"grm_grouped_retain_chain_lock for client:%d "
                                        "unlock_callback address:0x%x ",
                        client_id,
                        &unlock_callback);
  }

  if ( grm_info_p )
  {
    grm_info_p->grm_unlock_callback = unlock_callback;

    //grm_set_rx_tx_status(GRM_RX_TX_REQUESTED, rx_grm_client_id, gas_id);

    trm_grouped_retain_lock( grouped_retain_lock_input_info_p );
  }

  return;
}

/*===========================================================================

FUNCTION  grm_grouped_modify_chain_state

DESCRIPTION
  Set the chain state info for a particular DR-DSDS client. Copies the active
  chain state info from the pre-loaded grm_info store.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_grouped_modify_chain_state(grm_client_enum_t client_id,
                                    grm_tag_t         grm_tag,
                                    gas_id_t          gas_id )
{
  grm_info_t *grm_info_p    = grm_get_info_ptr_from_client(client_id, gas_id);
  trm_modify_data *grm_modify_chain_data_p = NULL;

  /* For each SUB, seperate memory for ISR and Task to avoid data corruption.
     Circular buffer of dimension 2 (ISR and Task)*/
  static trm_grouped_modify_state_input_info
    grm_grouped_modify_chain_state_input_info[GRM_ISR_TASK_DATA_SPACE][NUM_GERAN_DATA_SPACES];
  /* Index to find next circular buffer*/
  static uint8 circular_buff_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
  uint8 buf_idx = 0;
  int i = 0;

  grm_client_enum_t tx_grm_client_id = GRM_NO_CLIENT;
  grm_client_enum_t rx_grm_client_id = client_id;

  trm_client_enum_t tx_trm_client_id = TRM_NO_CLIENT;
  trm_client_enum_t rx_trm_client_id = TRM_NO_CLIENT;

  if ( grm_info_p )
  {
    trm_grouped_modify_state_input_info *grm_grouped_modify_chain_state_input_info_p = NULL;

    /*Find Circular Buffer Idx*/
    gl1_grm_info_mutex_lock(gas_id);
    buf_idx = circular_buff_idx[gas_id]++;
    circular_buff_idx[gas_id] %= GRM_ISR_TASK_DATA_SPACE;
    gl1_grm_info_mutex_unlock(gas_id);

    grm_grouped_modify_chain_state_input_info_p = &grm_grouped_modify_chain_state_input_info[buf_idx][gas_id];
    /* Always clear to avoid spurious data */
    memset(&grm_grouped_modify_chain_state_input_info[buf_idx][gas_id], 0, sizeof(trm_grouped_modify_state_input_info));

    grm_grouped_modify_chain_state_input_info_p->modify_type = TRM_MODIFY_MAX;

    switch ( grm_tag )
    {
      case GRM_MODIFY_BAND:
        grm_grouped_modify_chain_state_input_info_p->modify_type = TRM_MODIFY_BAND;
      break;

      case GRM_MODIFY_DURATION:
        grm_grouped_modify_chain_state_input_info_p->modify_type = TRM_MODIFY_DURATION;
      break;

      case GRM_MODIFY_REASON:
        grm_grouped_modify_chain_state_input_info_p->modify_type = TRM_MODIFY_REASON;
      break;

      default:

        MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"grm_grouped_modify_chain_state for client_id:%d request "
                                             "for unsupported grm_tag:%d",
                             client_id, grm_tag );
      break;
    }

    if ( grm_grouped_modify_chain_state_input_info_p->modify_type != TRM_MODIFY_MAX)
    {
      i = grm_grouped_modify_chain_state_input_info_p->num_of_entries = 0;

      grm_grouped_modify_chain_state_input_info_p->modify_info[i].client_id =
                                         rx_trm_client_id =
        grm_map_grm_client_id_to_trm_client_id(rx_grm_client_id);

      /* Copy data from grm_info data store into input data */
      grm_modify_chain_data_p = grm_get_modify_chain_state_data(rx_grm_client_id, gas_id);

      if (grm_modify_chain_data_p)
      {
        grm_grouped_modify_chain_state_input_info_p->modify_info[i].modify_data = *grm_modify_chain_data_p;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null grm_modify_chain_data_p! Invalid client_id %d", client_id );
      }

      grm_grouped_modify_chain_state_input_info_p->modify_info[i].tag         = (trm_tag_type)grm_tag;

      i++;

      /*Fill TX*/
      tx_grm_client_id = grm_map_rx_to_tx_client_id(rx_grm_client_id, gas_id);

      grm_grouped_modify_chain_state_input_info_p->modify_info[i].client_id =
                                                           tx_trm_client_id =
         grm_map_grm_client_id_to_trm_client_id(tx_grm_client_id);

      /* Copy data from grm_info data store into input data */
      grm_modify_chain_data_p = grm_get_modify_chain_state_data(tx_grm_client_id, gas_id);

      if (grm_modify_chain_data_p)
      {
        grm_grouped_modify_chain_state_input_info_p->modify_info[i].modify_data = *grm_modify_chain_data_p;
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Null grm_modify_chain_data_p! Invalid client_id %d", client_id );
      }

      grm_grouped_modify_chain_state_input_info_p->modify_info[i].tag         = (trm_tag_type)grm_tag;

      i++;

      grm_grouped_modify_chain_state_input_info_p->num_of_entries = i;

      grm_set_rx_tx_status(GRM_RX_TX_REQUESTED, rx_grm_client_id, gas_id);

      MSG_GERAN_HIGH_2_G( "L1 call to grm_grouped_modify_chain_state client:%d grm_tag:%d",
                        (int)client_id, grm_tag );

      /* Synchronous callback with Granted/Denied or Pending */
      trm_grouped_modify_chain_state(grm_grouped_modify_chain_state_input_info_p);
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", client_id);
  }
  return;
}

/*===========================================================================

FUNCTION  grm_is_tx_reason

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
boolean grm_is_tx_reason(trm_reason_enum_t reason)
{
  boolean tx_reason = FALSE;

  switch (reason)
  {
    case TRM_TRAFFIC:
    case TRM_BG_TRAFFIC:
    case TRM_ACCESS:
      tx_reason = TRUE;
      break;

    default:
      break;
  }
  return tx_reason;
}
#endif /*FEATURE_GSM_RX_TX_SPLIT*/

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  grm_register_async_callback

DESCRIPTION
  Register callback handler with TRM.
  Diversity Client registers with TRM for individual callback.
  Rx, Tx and BPLMN clients registers with TRM for grouped callback.
  During Rx client initialisation, Rx client registers all three (Rx, Tx and BPLMN)
  with TRM.
DEPENDENCIES

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void grm_register_async_callback(grm_client_enum_t  client_id,
                                 gas_id_t           gas_id)
{
  boolean           grouped_callback  = TRUE;
  boolean           is_reg_required   = TRUE;
  uint8             num_of_clients    = 0;
  grm_client_enum_t bplmn_client_id   = gl1_convert_gas_id_to_bplmn_client(gas_id);
  boolean           is_bplmn_client   = gl1_is_client_for_bplmn(client_id);

  grm_client_enum_t client[MAX_NUM_GRM_CLIENTS] = { INITIAL_VALUE( GRM_NO_CLIENT ) };

  if (gl1_msg_get_multi_sim_mode())
  {
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (is_bplmn_client || grm_is_a_secondary_client(client_id))
    {
      /* BPLMN/DRX client is already registered with TRM along with Rx and TX*/
      is_reg_required = FALSE;
    }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
  }
  else
    {
    grouped_callback = FALSE;
  }

  if (is_reg_required)
  {
    if (grouped_callback)
    {
      /*Rx*/
      client[num_of_clients++] = client_id;
      /*Tx*/
      client[num_of_clients++] = grm_map_rx_to_tx_client_id(client_id, gas_id);
      /*BPLMN*/
      client[num_of_clients++] = bplmn_client_id;
      /*BPLMN Tx*/
      client[num_of_clients++] = grm_map_rx_to_tx_client_id(bplmn_client_id, gas_id);
      /*DRX client. NV item setting is not updated yet, therefore can't control by NV item.
        NV item setting is updated in gl1_hw_multi_sim_init which is called after grm_initialise.*/
      client[num_of_clients++] = grm_map_prx_to_divrx_client_id(client_id, gas_id);
    }
    else
    {
      client[num_of_clients++] = client_id;
    }
    if ( gl1_drds_debug( gas_id ) )
    {
      MSG_GERAN_MED_2_G( GL1_DRDS_DBG_HDR"grm_register_async_callback "
				  "num_of_clients %d client_id %d ", num_of_clients, client_id );
    }
	
      /* Now register single TRM async callback handler */
#ifdef FEATURE_GSM_WTR_HOP
    grm_register_async_event_cb( num_of_clients,
                                 client,
                                 ( TRM_AYSNC_CONN_HOP_INFO_REQ | TRM_AYSNC_DR_INFO_REQ | TRM_ASYNC_IDLE_COLL_EVENT_INFO ),
                                 gas_id);
#else
    grm_register_async_event_cb( num_of_clients,
                                 client,
                                 TRM_AYSNC_DR_INFO_REQ | TRM_ASYNC_IDLE_COLL_EVENT_INFO,
                                 gas_id);
#endif /* FEATURE_WTR_HOP */
  }
  else
  {
    MSG_GERAN_MED_1_G( GL1_DRDS_DBG_HDR"TRM callback registration is not required for client %d ",
                       client_id);
  }
  return;
}

#ifdef  FEATURE_CRAT_MSIM_TEST_FRAMEWORK
/*===========================================================================

FUNCTION  grm_els_async_info_cb

DESCRIPTION
  This function handles the TRM unlock callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client

DEPENDENCIES
  Do NOT use GERAN-owned mutexes within this function, must execute quickly

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the GL1 task queue to action the abort in that context

===========================================================================*/
static void grm_els_async_info_cb(
   trm_els_state_callback_data *els_event_info_p,
   trm_client_enum_t         client
   )
{
  trm_client_enum_t  els_data_tech;
  gas_id_t gas_id = gl1_map_client_id_to_gas_id(grm_map_trm_client_id_to_grm_client_id(client));
  gas_id = check_gas_id(gas_id);

  if (!els_event_info_p)
  {
    MSG_GERAN_ERROR_0_G("grm_els_async_info_cb no valid els_event_info_p");

    return;
  }


  els_data_tech = els_event_info_p->data_tech;

  if (els_event_info_p->els_cb_activity == TRM_ELS_ACTIVATION)
  {
    /* To register that UE is ELS capable
     * ELS on/off notification to idle tech
     */
    /* Info used for ELS on/off notification from data->idle tech
        1. Data tech ID 
        2. Indication whether DDS has turned ELS On (TRUE) or Off (FALSE)
    */
    if (els_event_info_p->els_active)
    {
      els_active = TRUE;
      MSG_GERAN_MED_1_G("ELS ACTIVE by Data Tech %d", els_data_tech );

    }
    else
    {
      els_active = FALSE;
      MSG_GERAN_MED_1_G("ELS INACTIVE by Data Tech %d", els_data_tech );
    }
  }

  return;

} /* grm_els_async_info_cb */

/*===========================================================================

FUNCTION  grm_els_state_registration

DESCRIPTION
  ELS feature registration, activation, and state information updates will
  be achieved through the trm_set_client_state() interface.


DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS

===========================================================================*/
void grm_els_state_registration(
   grm_client_enum_t grm_client_id,
   gas_id_t          gas_id)
{

  memset(&grm_set_trm_els_state_info[(uint8)gas_id], 0,  sizeof(grm_set_client_state_info_t));


  /* ELS State Input Data */
  grm_set_trm_els_state_info[gas_id].client_state_info.client_id = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
  grm_set_trm_els_state_info[gas_id].client_state_info.client_state = TRM_ELS_STATE;
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.request  =  TRM_ELS_REGISTRATION;
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.operation = TRM_START_STATE;

  trm_set_client_state(&grm_set_trm_els_state_info[gas_id].client_state_info, &grm_set_trm_els_state_info[gas_id].trm_return_data);


  if (grm_set_trm_els_state_info[gas_id].trm_return_data.grant == TRM_GRANTED)
  {
    MSG_GERAN_MED_0_G("ELS REGISTRATION GRANTED");
  }
  else
  {
    MSG_GERAN_ERROR_0_G("ELS REGISTRATION NOT GRANTED");
  }

}

/*===========================================================================

FUNCTION  grm_els_state_update_aperiodic

DESCRIPTION
  Any TRM request that does not fall into the periodic gap pattern (i.e. paging)
  shall be assumed to be aperiodic. 


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void grm_els_state_update_aperiodic(trm_reason_enum_t reason,
                                    sys_proc_type_e_type sub_reason,
                                    trm_duration_t            duration,
                                    grm_client_enum_t grm_client_id,
                                    gas_id_t          gas_id)
{
  /*have a Static Var. to save Stack Space*/
  memset(&grm_set_trm_els_state_info[(uint8)gas_id], 0,  sizeof(grm_set_client_state_info_t));


  /* ELS State Input Data */
  grm_set_trm_els_state_info[gas_id].client_state_info.client_id = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
  grm_set_trm_els_state_info[gas_id].client_state_info.client_state = TRM_ELS_STATE;
  /* For idle tech to update its ELS state to send to DDS */
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.request  =  TRM_ELS_STATE_UPDATE;
  /* N/A for TRM_ELS_STATE_UPDATE requests */
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.operation = TRM_MAX_STATE_OPERATION;

  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.gap_type = TRM_ELS_APERIODIC_GAP;
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.client_id = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.reason = reason;
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.duration = duration;
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.activity = convert_sub_reason_to_els_activity(sub_reason,gas_id);

  trm_set_client_state(&grm_set_trm_els_state_info[gas_id].client_state_info, &grm_set_trm_els_state_info[gas_id].trm_return_data);


  if (grm_set_trm_els_state_info[gas_id].trm_return_data.grant == TRM_GRANTED)
  {
    MSG_GERAN_MED_0_G("ELS APEROIDIC GRANTED");
  }
  else
  {
    MSG_GERAN_ERROR_0_G("ELS APEROIDIC NOT GRANTED");
  }

}


/*===========================================================================

FUNCTION  grm_els_state_update

DESCRIPTION
  Entering Idle state triggers Periodic Events to TRM.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
 
ensure that client is always 7 or 8 for els 
probably dont want to call this all the time only when drx cycle changes 
but to begin with call always (for BU) 
 
===========================================================================*/
void grm_els_state_update(trm_time_t            when,
                          trm_reason_enum_t reason,
                          sys_proc_type_e_type sub_reason,
                          trm_duration_t            duration,
                          grm_client_enum_t grm_client_id,
                          gas_id_t          gas_id)
{
  /*have a Static Var. to save Stack Space*/
  memset(&grm_set_trm_els_state_info[(uint8)gas_id], 0,  sizeof(grm_set_client_state_info_t));

  /* ELS State Input Data */
  grm_set_trm_els_state_info[gas_id].client_state_info.client_id = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
  grm_set_trm_els_state_info[gas_id].client_state_info.client_state = TRM_ELS_STATE;
  /* For idle tech to update its ELS state to send to DDS */
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.request  =  TRM_ELS_STATE_UPDATE;
  /* N/A for TRM_ELS_STATE_UPDATE requests */
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.operation = TRM_MAX_STATE_OPERATION;


  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.client_id = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.reason = reason;
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.duration = duration;
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.gap_start = when;
  grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.activity = convert_sub_reason_to_els_activity(sub_reason,gas_id);

  if (reason == TRM_DEMOD_PAGE)
  {
    /* This is Periodic gap */

    grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.gap_type = TRM_ELS_PERIODIC_GAP;
    
    grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.drx_cycle = grm_get_drx_cycle(grm_client_id, gas_id);

    /* Include warmup duration and extra gsm frame to calculet over the air rx burst start position */
    grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.page_frame_start = 
                                                       when+NUM_SCLKS_PER_GSM_FRAME+gl1_hw_sleep_wmgr_els_warmup_dur_in_sclk[gas_id];
    /* For GERAN page_frame_start is equal to page_frame_position */
    grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.page_frame_position = 
        grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.page_frame_start;

    MSG_GERAN_MED_2_G("ELS PEROIDIC drx %d ms wup_dur %d", grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.drx_cycle,
                      gl1_hw_sleep_wmgr_els_warmup_dur_in_sclk[gas_id]);

    gl1_hw_sleep_wmgr_els_warmup_dur_in_sclk[gas_id] = 0;

  }
  else
  {
    /* all non DEMOD_PAGE is treated as Aperiodic */
    grm_set_trm_els_state_info[gas_id].client_state_info.data.els_data.new_els_state.gap_type = TRM_ELS_APERIODIC_GAP;
    MSG_GERAN_MED_0_G("ELS APEROIDIC");

  }

  trm_set_client_state(&grm_set_trm_els_state_info[gas_id].client_state_info, &grm_set_trm_els_state_info[gas_id].trm_return_data);


  if (grm_set_trm_els_state_info[gas_id].trm_return_data.grant == TRM_GRANTED)
  {
    MSG_GERAN_MED_0_G("ELS STATE UPDATE GRANTED");
  }
  else
  {
    MSG_GERAN_ERROR_0_G("ELS STATE UPDATE NOT GRANTED");
  }

}

/*===========================================================================

FUNCTION  grm_get_drx_cycle 

DESCRIPTION
  ELS feature requires the drx cycle for periodic gaps in ms


DEPENDENCIES
  None

RETURN VALUE
  ELS activity

SIDE EFFECTS

===========================================================================*/
uint32  grm_get_drx_cycle(grm_client_enum_t  client_id,  gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  if ( grm_info_p )
  {
    if ( grm_info_p->curr_bs_pa_mfrms == GRM_INITIALISED_MFRMS )
    {
      grm_info_p->curr_bs_pa_mfrms = l1_idle_data_store[gas_id].campedon_cell_data.BS_PA_MFRMS;
      MSG_GERAN_MED_1_G("drx value restored from idle data store: updated drx value %d ", grm_info_p->curr_bs_pa_mfrms);
    }
    if (!(grm_info_p && grm_info_p->curr_bs_pa_mfrms > 1))
    {
      MSG_GERAN_ERROR_1_G("Unexpected condition logic: drx value %d", grm_info_p->curr_bs_pa_mfrms);
      grm_info_p->curr_bs_pa_mfrms = 2;
    }
    return FRAMES_TO_MSECS(grm_info_p->curr_bs_pa_mfrms * MULTIFRAME_51);
  }
  return 0;
}


/*===========================================================================

FUNCTION  grm_els_state_update_G2X_resel 

DESCRIPTION
  Update ELS about a G2X reselction: Activity shall be notified as TRM_ELS_IRAT, 
  TRM reason can be INVALID, duration can be any value. 


DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS

===========================================================================*/

void grm_els_state_update_G2X_resel ( gas_id_t gas_id )
{
  if(  els_active  )
  {
    grm_client_enum_t client_id     = gl1_map_gas_id_to_client_id(gas_id);
    trm_duration_t    duration      = timetick_cvt_to_sclk( RESELECTION_DURATION_MS, T_MSEC );
    grm_els_state_update_aperiodic(TRM_NUM_REASONS,
                                   SYS_PROC_TYPE_IRAT_CELL_RESELECTION,
                                   duration,
                                   client_id,
                                   gas_id);
  }
  else
  {
    MSG_GERAN_HIGH_0_G("!els_active"); 
  }
}
/*===========================================================================

FUNCTION  grm_clear_els_active

DESCRIPTION
  Clear the ELS active state 


DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS

===========================================================================*/

void grm_clear_els_active ( void )
{
 els_active = FALSE;
}
#endif  /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */

#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
/*===========================================================================

FUNCTION  grm_handle_trm_request

DESCRIPTION
  Forwards a GSM client request to the TRM layer

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS


===========================================================================*/
trm_grant_return_enum_type grm_handle_trm_request(grm_client_enum_t   client_id,
                                                  trm_duration_t      duration,
                                                  trm_reason_enum_t   reason,
                                                  sys_proc_type_e_type sub_reason,
                                                  trm_resource_enum_t resource,
                                                  grm_divrx_request_type  req_type,
                                                  gas_id_t            gas_id)
{
  trm_request_input_info *gl1_trm_request_input_info_p = NULL;
  trm_grant_return_enum_type grant = TRM_DENIAL;
  /* Always clear to avoid spurious data */
  trm_request_input_info  gl1_trm_request_input_info = { 0 };
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  grm_grouped_request_bitmap_t grp_bitmap = {0};
  boolean grouped_request = FALSE;

  if (grm_info_p)
  {
    /* Single SIM - No grouped TRM call*/
#ifdef FEATURE_GSM_RX_TX_SPLIT
    if (gl1_msg_get_multi_sim_mode())
    {
      /*Grouped Request*/
      grp_bitmap.prxRequest = TRUE;
      grp_bitmap.txRequest = ((grm_is_tx_reason(reason) && (sub_reason != SYS_PROC_TYPE_PAGE)) ? TRUE : FALSE);

      /* Diversity chain is requested if PRX is granted. Assume PRX is granted for grouped request*/
      if ((req_type != GRM_DIVRX_TRM_REQ_INVALID)
          && grm_validate_diversity_chain_request(TRM_GRANTED, req_type, gas_id))
      {
        grp_bitmap.drxRequest = TRUE;
      }

      if (grp_bitmap.txRequest || grp_bitmap.drxRequest)
      {
        grouped_request = TRUE;
      grant = grm_grouped_trm_request(client_id,
                                      duration,
                                      reason,
                                      sub_reason,
                                      resource,
                                        req_type,
                                        grp_bitmap,
                                      gas_id);
    }
    }
#endif /*FEATURE_GSM_RX_TX_SPLIT*/
    if (FALSE == grouped_request)
    {
      gl1_trm_request_input_info_p = &gl1_trm_request_input_info;

      gl1_trm_request_input_info_p->client_id = grm_map_grm_client_id_to_trm_client_id(client_id);
      gl1_trm_request_input_info_p->resource  = resource;
      gl1_trm_request_input_info_p->duration  = duration;
      gl1_trm_request_input_info_p->reason    = reason;
      gl1_trm_request_input_info_p->sub_reason = sub_reason;
      gl1_trm_request_input_info_p->freq_info = grm_info_p->grm_freq_info;

      gl1_trm_request_input_info_p->tag       = (trm_tag_type)GRM_REQUEST;

#ifdef FEATURE_QTA
      if(IS_QTA_REASON(reason, sub_reason) &&
         (!l1_is_fm_trm_priority_g2t(gas_id)) &&
         (duration <= (gpl1_multi_sim_convert_frames_to_sclcks(L1_QTA_DURATION_GAP_MAX))))
      {
        gl1_trm_request_input_info_p->ta_info    = TRM_QUICK_TUNEAWAY;
      }
      else
#endif
      {
        gl1_trm_request_input_info_p->ta_info    = TRM_LONG_TUNEAWAY;
      }

      /* forward the request to TRM */
      trm_request( gl1_trm_request_input_info_p, &grm_info_p->grm_request_data );

      /* Detect tag mismatch */
      if ( grm_info_p->grm_request_data.tag != gl1_trm_request_input_info_p->tag )
      {
        MSG_GERAN_ERROR_2_G( GL1_DRDS_DBG_HDR"GRM_Request Tag mismatch in:%d out:%d",
                             gl1_trm_request_input_info_p->tag,
                             grm_info_p->grm_request_data.tag );
      }
      grant = grm_info_p->grm_request_data.grant;
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", client_id);
  }
  return grant;
}

#ifdef FEATURE_GSM_QSC_TX_DIV
/*===========================================================================

FUNCTION  check_asdiv_grm_client

DESCRIPTION
Make sure gl1 makes any TRM ASDIV request with GRM_GSM1 or GRM_GSM2

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/

grm_client_enum_t check_asdiv_grm_client(grm_client_enum_t grm_client)
{
  if((grm_client == GRM_GSM1) ||
     (grm_client == GRM_GPRS1) ||
     (grm_client == GRM_GSM1_TX) ||
     (grm_client == GRM_GPRS1_TX))
    {
      return GRM_GSM1;
    }

  else if((grm_client == GRM_GSM2) ||
     (grm_client == GRM_GPRS2) ||
     (grm_client == GRM_GSM2_TX) ||
     (grm_client == GRM_GPRS2_TX))
    {
      return GRM_GSM2;
    }
  else
    {
    return GRM_GSM1;
    }

};

#endif /*FEATURE_GSM_QSC_TX_DIV*/

/*===========================================================================

FUNCTION  grm_check_device_supported

DESCRIPTION
  Check if the allocated RF device supports the band.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
boolean grm_check_device_supported (
                                   grm_client_enum_t    requested_client_id,
                                   trm_band_t           requested_band,
                                   rfm_device_enum_type allocated_device,
                                   gas_id_t             gas_id )
{
  grm_info_t *grm_info_p  = grm_get_info_ptr_from_client(requested_client_id, gas_id);

  boolean               band_is_supported_by_allocated_dev = FALSE;
    /* whether the requested band is supported by the allocated device */
  trm_get_info_data     grm_get_device_support_input;
    /* input date structure for device support info */
  uint8                 num_of_device = 0;
    /* number of devices which support the requested band*/
  uint8                 i;
    /* looping index */

  grm_get_device_support_input.dev_support_info.band = requested_band;
    /* Input information for the API */

  if ( grm_info_p )
  {
    if ( grm_get_info( requested_client_id, &grm_get_device_support_input, GRM_GET_DEVICE_SUPPORT_INFO, gas_id ) )
    {
      num_of_device = grm_info_p->grm_get_info_data.info.dev_support_info.num_of_device; 
      for (i=0; i<num_of_device; i++)
      {
        if ( grm_info_p->grm_get_info_data.info.dev_support_info.rfm_device[i].device == allocated_device)
        {
          /* The allocated device is in the array of rf devices which support the input band */
          band_is_supported_by_allocated_dev = TRUE;
          break;
        }
      }
      
      MSG_GERAN_MED_3_G("grm_get_device_support: allocated device:%d band requested:%d, supported? %d ",
                         allocated_device,
                         grm_get_device_support_input.dev_support_info.band,
                         (uint8)band_is_supported_by_allocated_dev
                        );

    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G( GL1_DRDS_DBG_HDR"grm_get_device_support Invalid client ID:%d",
                         requested_client_id );
  }

  return ( band_is_supported_by_allocated_dev );
}

/**
 * @brief Performs a Grouped Reserve At Based on BitMap Requested 
 * When holding Lock with multiple Clients its best to Perform a Grouped Release 
 * To avoid Race Conditions where outcome depends on which Client Releases/Reserves First
 * 
 * @param client_id 
 * @param bitmap 
 * @param gas_id 
 */
void grm_grouped_trm_reserve(
                             grm_client_enum_t  client_id,
                             grm_grouped_request_bitmap_t bitmap,
                             trm_time_t            when,
                             trm_duration_t        duration,
                             trm_reason_enum_t     reason,
                             sys_proc_type_e_type  sub_reason,
                             trm_resource_enum_t   resource, 
                             gas_id_t gas_id)
{
  
  int i = 0;
  uint8 buf_idx = 0;

  /* For each SUB, seperate memory for ISR and Task to avoid data corruption.
     Circular buffer of dimension 2 (ISR and Task)*/
  static trm_grouped_reserve_at_input_info
    grm_grouped_reserve_at_input[GRM_ISR_TASK_DATA_SPACE][NUM_GERAN_DATA_SPACES];

  /* Index to find next circular buffer*/
  static uint8 circular_buff_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
  
  trm_tuneaway_enum_type grm_ta_info    = TRM_LONG_TUNEAWAY;
  sys_band_class_e_type  trm_band       = SYS_BAND_CLASS_NONE ;
  grm_client_enum_t rx_grm_client_id    = client_id;
  grm_client_enum_t tx_grm_client_id    = grm_map_rx_to_tx_client_id(rx_grm_client_id, gas_id);
  grm_client_enum_t divrx_grm_client_id = grm_map_prx_to_divrx_client_id(rx_grm_client_id, gas_id);

  grm_info_t *grm_info_p                = grm_get_info_ptr_from_client( client_id, gas_id );

  if (grm_get_resource_lock_state(gas_id)||
     (GERAN_EXCHANGE_GTOX == geran_get_exchange_on_release( gas_id )))
  {
    MSG_GERAN_HIGH_0_G( GL1_DRDS_DBG_HDR"grm_grouped_reserve_at ignored - TRM resource lock is active" );
    return;
  }

  if ( grm_info_p )
  {
    trm_grouped_reserve_at_input_info *grm_grouped_reserve_at_input_p = NULL;

    /*Find Circular Buffer Idx*/
    gl1_grm_info_mutex_lock(gas_id);
    buf_idx = circular_buff_idx[gas_id]++;
    circular_buff_idx[gas_id] %= GRM_ISR_TASK_DATA_SPACE;
    gl1_grm_info_mutex_unlock(gas_id);

    grm_grouped_reserve_at_input_p = &grm_grouped_reserve_at_input[buf_idx][gas_id];
    memset(&grm_grouped_reserve_at_input[buf_idx][gas_id], 0, sizeof(trm_grouped_reserve_at_input_info));

    /*Decide Quick Tune Away Appl.*/
    if(IS_X_PLUS_G() && 
       IS_QTA_REASON(reason, sub_reason)&&
       (!l1_is_fm_trm_priority_g2t(gas_id))&&
       (duration <= (gpl1_multi_sim_convert_frames_to_sclcks(L1_QTA_DURATION_GAP_MAX))))
    {
      grm_ta_info    = TRM_QUICK_TUNEAWAY;
    }
    /*we are setting ta info as QTA for GBTA(only for Idle state activities ) - to re-use the existing TRM enum*/
    else if(!IS_X_PLUS_G() && 
            ( l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_IDLE ) && 
          IS_GBTA_REASON(reason, sub_reason))
    {
      grm_ta_info    = TRM_QUICK_TUNEAWAY;
    }

    if (bitmap.txRequest) 
    {
      /*Fill TX*/
      grm_grouped_reserve_at_input_p->reserve_at_info[i].client_id  = grm_map_grm_client_id_to_trm_client_id(tx_grm_client_id);
      grm_grouped_reserve_at_input_p->reserve_at_info[i].reason     = reason;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].sub_reason = sub_reason;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].resource   = TRM_TX_BEST;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].ta_info    = TRM_LONG_TUNEAWAY;
      i++;
    }

    if (bitmap.drxRequest)
    {
      /*Fill DRX*/
      grm_grouped_reserve_at_input_p->reserve_at_info[i].client_id  = grm_map_grm_client_id_to_trm_client_id(divrx_grm_client_id);
      grm_grouped_reserve_at_input_p->reserve_at_info[i].reason     = TRM_DIVERSITY;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].sub_reason = SYS_PROC_TYPE_DIVERSITY_CHAIN;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].resource   = TRM_RX_DIVERSITY;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].ta_info    = grm_ta_info;
      /* Set TRM status to Reserved for diversity client */
      grm_set_trm_status(GL1_TRM_NULL, divrx_grm_client_id, gas_id);
      i++;
    }

    /* Fill PRx*/
    grm_grouped_reserve_at_input_p->reserve_at_info[i].client_id  = grm_map_grm_client_id_to_trm_client_id(rx_grm_client_id);
    grm_grouped_reserve_at_input_p->reserve_at_info[i].reason     = reason;
    grm_grouped_reserve_at_input_p->reserve_at_info[i].sub_reason = sub_reason;
    grm_grouped_reserve_at_input_p->reserve_at_info[i].resource   = resource;
    grm_grouped_reserve_at_input_p->reserve_at_info[i].ta_info    = grm_ta_info;
  
    grm_grouped_reserve_at_input_p->num_of_entries = i+1;

#ifndef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
    //AsDiv Appl. Only for PRx Client ID
    MSG_GERAN_HIGH_2_G(GL1_ASDIV"grm_grouped_reserve_chain_at: reason %d switch_at_wakeup %d",reason,gl1_asdiv_data.switch_at_wakeup[gas_id]);

    if (reason == TRM_DEMOD_PAGE)
    {
      if (gl1_asdiv_data.control_mask & DISABLE_IDLE_ANT_SWITCH)
      {
        grm_grouped_reserve_at_input_p->reserve_at_info[i].ant_switch_info.request_type = TRM_ANT_SWITCH_STATE_REQUEST_DEFAULT;
      }
      else if (gl1_asdiv_enable(GL1_TRM_ASDIV_TYPE1, gas_id))
      { 
        if (gl1_asdiv_data.switch_at_wakeup[gas_id])
        {
          grm_grouped_reserve_at_input_p->reserve_at_info[i].ant_switch_info.request_type = TRM_ANT_SWITCH_STATE_REQUEST_REVERSE;
        }
        else
        {
          grm_grouped_reserve_at_input_p->reserve_at_info[i].ant_switch_info.request_type = TRM_ANT_SWITCH_STATE_REQUEST_CURRENT;
        }
      }
      else if (gl1_asdiv_enable(GL1_TRM_ASDIV_TYPE2, gas_id))
      {
        grm_grouped_reserve_at_input_p->reserve_at_info[i].ant_switch_info.request_type = gl1_asdiv_type2_idle_pingpong_thresh_check(gas_id);
      }
    
       
         //Source client is used only for the first TRM request after HO/reselection
         //clear the source clients in reserve chain
      
      gl1_asdiv_data.X2G_source_client[gas_id].prx_client = GRM_NO_CLIENT;
      gl1_asdiv_data.X2G_source_client[gas_id].tx_client  = GRM_NO_CLIENT;	
    }//If reason is for Demod Page
    grm_grouped_reserve_at_input_p->reserve_at_info[i].ant_switch_info.source_client_id = TRM_NO_CLIENT;
#endif // FEATURE_GSM_TX_DIV_ANTNA_SWITCHING 

    /*Get the freq Info Data */
    if ( l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_FIND_BCCH )
    {
       /*Dont clear freq info, Correct Band info is already added in FIND BCCH State machine.*/
    }
    else if (reason == TRM_DEMOD_PAGE)
    {
      grm_clear_freq_info(client_id, gas_id);

      /*clear out the previously registered bands with TRM for DR-DSDS op mode*/
      if(gl1_drdsds_enabled( gas_id))
      {
        grm_clear_registered_bands(client_id,gas_id);
      }

      /*auto init to add serving band*/
      (void)grm_check_valid_freq_info(grm_info_p,gas_id);

      if(grm_persistent_denial_monitoring(GRM_CHECK_PERSISTENT_DENIAL_FLAG, client_id, GRM_NULL_COMMAND, gas_id)
       && gl1_drdsds_enabled(gas_id))
      {
        l1_sc_monscan_idle_check_for_inter_band_cells( gas_id );
        (void)grm_persistent_denial_monitoring(GRM_ADD_DENIED_BANDS_TO_FREQ_INFO,
                                             client_id,
                                             GRM_NULL_COMMAND,
                                             gas_id);
      }
    }
#ifdef FEAT_DIFF_SIG_PATH_PER_GSM_BAND
    else if (grm_is_tx_reason(reason) && (sub_reason != SYS_PROC_TYPE_PAGE))
    /* Request for all GSM bands in case reason is ACCESS or TRAFFIC or BG_TRAFFIC */
    {
       /*RX client*/
       grm_set_valid_acquisition_bands(rx_grm_client_id,gas_id);
       grm_clear_freq_info(rx_grm_client_id, gas_id);

       for(idx=0;idx<grm_valid_gsm_sys_bands_cnt[gas_id];idx++)
       {
         grm_add_trm_band_to_freq_input_type(grm_valid_gsm_sys_bands[gas_id][idx], rx_grm_client_id, gas_id);
       }

       /*TX client*/
       grm_set_valid_acquisition_bands(tx_grm_client_id,gas_id);
       grm_clear_freq_info(tx_grm_client_id, gas_id);

       for(idx=0;idx<grm_valid_gsm_sys_bands_cnt[gas_id];idx++)
       {
          grm_add_trm_band_to_freq_input_type(grm_valid_gsm_sys_bands[gas_id][idx], tx_grm_client_id, gas_id);
       }
    }
#endif
    else
    {
      grm_clear_freq_info(client_id, gas_id);
      if (res_at_enh_pri[gas_id] != FM_PRI_MAX)
      {
        trm_band =  grm_get_trm_band_from_g_pri(res_at_enh_pri[gas_id], gas_id);
        /*reset the static var*/
        res_at_enh_pri[gas_id] = FM_PRI_MAX;
      }
      else
      {
        trm_band =  grm_get_trm_band_from_g_pri(l1_get_fm_trm_priority(gas_id ), gas_id);
      }
      grm_add_trm_band_to_freq_input_type(trm_band, client_id, gas_id);
    }

    for (i = 0;i< grm_grouped_reserve_at_input_p->num_of_entries;i++)
    {
      grm_grouped_reserve_at_input_p->reserve_at_info[i].duration  = duration;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].when      = when;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].tag       = (trm_tag_type)GRM_RESERVE_AT;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].freq_info = grm_info_p->grm_freq_info;
      grm_grouped_reserve_at_input_p->reserve_at_info[i].wakeup_identifier = grm_info_p->start_frame;
      grm_gl1_set_current_trm_reason(grm_grouped_reserve_at_input_p->reserve_at_info[i].reason, 
                                     grm_grouped_reserve_at_input_p->reserve_at_info[i].sub_reason, 
                                     grm_map_trm_client_id_to_grm_client_id(grm_grouped_reserve_at_input_p->reserve_at_info[i].client_id), 
                                     gas_id);
    }/*For Loop*/

    MSG_GERAN_HIGH_1_G("L1 call to grm_grouped_reserve_at client:%d", rx_grm_client_id); 

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /*Call band release before request and notify call to TRM*/
    grm_send_band_release_if_required(rx_grm_client_id, gas_id);
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

#if defined ( FEATURE_DUAL_SIM ) && defined ( FEATURE_CRAT_MSIM_TEST_FRAMEWORK )
    if (els_active )
    {
      grm_els_state_update(when,
                           reason,
                           sub_reason,
                           duration,
                           rx_grm_client_id,
                           gas_id);
    }else
    {
      MSG_GERAN_HIGH_0_G("!els_active"); 
    }
#endif /* FEATURE_CRAT_GERAN_TEST_FRAMEWORK && FEATURE_DUAL_SIM */
    /* Request the lock now for this using the asynchronous TRM interface */
    trm_grouped_reserve_at( grm_grouped_reserve_at_input_p );

    /*Reset Trm stat for Tx Chain*/
    if (bitmap.txRequest)
    {
      grm_set_trm_status(GL1_TRM_NULL, tx_grm_client_id, gas_id);
	  
    }

    grm_set_trm_status(GL1_TRM_NULL, rx_grm_client_id, gas_id);

  }
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", client_id);
  }

  return;
}/*grm grouped trm reserve at*/

/*===========================================================================

FUNCTION call_in_task_grm_request

DESCRIPTION
 Request TRM for specified client

===========================================================================*/
void call_in_task_grm_request(grm_request_data_t *params_p)
{
  grm_request_data_t msg;
  gas_id_t           gas_id;

  if (params_p)
  {
    gas_id = params_p->gas_id;
    /* load the outgoing message with the supplied parameters */
    memscpy( &msg,
             sizeof(grm_request_data_t),  /* destination size */
             params_p,
             sizeof(grm_request_data_t) ); /* source size */

    msg.header.message_set = MS_L1_L1;
    msg.header.message_id  = L1_GRM_REQUEST;

    PUT_IMH_LEN( sizeof( grm_request_data_t ) - sizeof( IMH_T ), &msg );

    msg.gas_id             = gas_id;

    MSG_GERAN_MED_0_G("call_in_task_grm_request");

    gl1_drx_task_active( gas_id );

    ( void )L1_isr_send_message( l1_queues[gas_id], &msg, TRUE, gas_id );
  }
  else
  {
    MSG_GERAN_ERROR_0("call_in_task_grm_request no valid params_p"  );
  }
  return;
}


#ifdef FEATURE_DUAL_DATA
/*===========================================================================

FUNCTION grm_set_data_sub_priority

DESCRIPTION
  Sets specific Subscription to assigned priority as sets other subscriptions
  to GERAN_MMS_PRIORITY_NOMINAL

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
void grm_set_data_sub_priority(geran_priority_t priority,gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint16 i=0;

  l1_tskisr_blk->priority = priority;

  for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
  {
    if (gas_id != i)
{
      l1_tsk_buffer[i].priority = GERAN_MMS_PRIORITY_NOMINAL;

      /* Revert Multislot Class other subs in case it was
       * reduced during MMS transfer */
      if (priority == GERAN_MMS_PRIORITY_NOMINAL)
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

FUNCTION grm_get_data_sub_priority

DESCRIPTION
  Returns the priority of subscription

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
geran_priority_t grm_get_data_sub_priority(gas_id_t gas_id)
  {
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   return(l1_tskisr_blk->priority);
  }
  
/*===========================================================================

FUNCTION grm_get_low_priority_data_sub

DESCRIPTION
  Returns the priority of low priority data subscription if one sub is found
  with GERAN_MMS_PRIORITY_MAXIMAL.

  If no sub is found with GERAN_MMS_PRIORITY_MAXIMAL priority then will
  return GERAN_ACCESS_STRATUM_ID_UNDEFINED

PARAMS
  None

DEPENDENCIES

RETURN VALUE

===========================================================================*/
gas_id_t grm_get_low_priority_data_sub(void)
{
   gas_id_t gl1_mms_low_priority_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

   /* Check if we are in Data+MMS mode and assign gas_id for DDS sub*/
   if(grm_get_data_sub_priority(GERAN_ACCESS_STRATUM_ID_1) == GERAN_MMS_PRIORITY_MAXIMAL)
   {
     gl1_mms_low_priority_gas_id = GERAN_ACCESS_STRATUM_ID_2;
   }
   else if (grm_get_data_sub_priority(GERAN_ACCESS_STRATUM_ID_2)== GERAN_MMS_PRIORITY_MAXIMAL)
  {
     gl1_mms_low_priority_gas_id = GERAN_ACCESS_STRATUM_ID_1;
  }

   return(gl1_mms_low_priority_gas_id);
}
#endif /* FEATURE_DUAL_DATA */
/*===========================================================================

FUNCTION     grm_grouped_trm_release

DESCRIPTION  Perform a Grouped Release on clients set in input bitmap

DEPENDENCIES Resets trm status for Tx and Div Rx CLients only, and not for PRx client
             At the moment expected to be called only from grm release
          
RETURN VALUE None

===========================================================================*/
void grm_grouped_trm_release(grm_client_enum_t client_id, grm_grouped_request_bitmap_t bitmap, gas_id_t gas_id)
{
  int i = 0;

  /* For each SUB, seperate memory for ISR and Task to avoid data corruption.
     Circular buffer of dimension 2 (ISR and Task)*/
  static trm_grouped_release_input_type
    grm_grouped_release_input[GRM_ISR_TASK_DATA_SPACE][NUM_GERAN_DATA_SPACES];
  /* Index to find next circular buffer*/
  static uint8 circular_buff_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
  uint8 buf_idx = 0;

  grm_client_enum_t rx_grm_client_id = client_id;
  grm_client_enum_t tx_grm_client_id = grm_map_rx_to_tx_client_id(rx_grm_client_id, gas_id);
  grm_client_enum_t divrx_grm_client_id = grm_map_prx_to_divrx_client_id(rx_grm_client_id, gas_id);

  grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client_id, gas_id );

  if ( grm_info_p )
  {
    trm_grouped_release_input_type *grm_grouped_release_input_p = NULL;

    /*Find Circular Buffer Idx*/
    gl1_grm_info_mutex_lock(gas_id);
    buf_idx = circular_buff_idx[gas_id]++;
    circular_buff_idx[gas_id] %= GRM_ISR_TASK_DATA_SPACE;
    gl1_grm_info_mutex_unlock(gas_id);

    grm_grouped_release_input_p = &grm_grouped_release_input[buf_idx][gas_id];
    memset(&grm_grouped_release_input[buf_idx][gas_id], 0, sizeof(trm_grouped_release_input_type));

    if (bitmap.txRequest) 
    {
      /*Fill TX*/
      grm_grouped_release_input_p->rel_info[i].client = grm_map_grm_client_id_to_trm_client_id(tx_grm_client_id);
      grm_grouped_release_input_p->rel_info[i].tag    = (trm_tag_type)GRM_RELEASE; 
      i++;
    }

    if (bitmap.drxRequest)
    {
      /*Fill DRX*/
      grm_grouped_release_input_p->rel_info[i].client = grm_map_grm_client_id_to_trm_client_id(divrx_grm_client_id);
      grm_grouped_release_input_p->rel_info[i].tag    = (trm_tag_type)GRM_RELEASE;
      i++;
    }

    /* Fill Rx*/
    grm_grouped_release_input_p->rel_info[i].client = grm_map_grm_client_id_to_trm_client_id(rx_grm_client_id);
    grm_grouped_release_input_p->rel_info[i].tag    = (trm_tag_type)GRM_RELEASE;
    i++;

    /*Release All Appl*/    
    grm_grouped_release_input_p->num_of_entries = i;
    
    MSG_GERAN_HIGH_1_G( "L1 call to grm_grouped_release client:%d", rx_grm_client_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /*Call band release before request and notify call to TRM*/
    grm_send_band_release_if_required(rx_grm_client_id, gas_id);
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

    /* Request the lock now for this using the asynchronous TRM interface */
    trm_grouped_release( grm_grouped_release_input_p );

    /*If RxD chain released then Reset its info as well */
    if (bitmap.drxRequest)
    {
      grm_reset_trm_reason_stat_unlck_and_others(divrx_grm_client_id, gas_id);
    }
    /*Reset Trm stat for Tx Chain*/
    if (bitmap.txRequest)
    {
      grm_set_trm_status(GL1_TRM_NULL, tx_grm_client_id, gas_id);
    }
  }/*If Valid grm info pointer*/
  else
  {
    MSG_GERAN_ERROR_1_G("Null grm_info_p for client %d", client_id);
  }

  return;
}

/*===========================================================================

FUNCTION     grm_validate_tx_chain_grant_status

DESCRIPTION  force an err_fatal if GSM holds Tx chain without Rx chain

RETURN VALUE None

===========================================================================*/
void grm_validate_tx_chain_grant_status(grm_client_enum_t client_id, gas_id_t gas_id)
{
  gl1_trm_state_t rx_trm_status = GL1_TRM_NULL;
  gl1_trm_state_t tx_trm_status = GL1_TRM_NULL;  
  grm_client_enum_t tx_client_id = grm_map_rx_to_tx_client_id(client_id, gas_id);

  if (GRM_NO_CLIENT != client_id)
  {
    rx_trm_status = grm_get_trm_status(client_id, gas_id);
  }
  if (GRM_NO_CLIENT != tx_client_id)
  {
    tx_trm_status = grm_get_trm_status(tx_client_id, gas_id);
  }
  
  if ( ( GL1_TRM_GRANTED == tx_trm_status ) && 
       ( ( GL1_TRM_NOT_GRANTED == rx_trm_status ) ||( GL1_TRM_NULL == rx_trm_status ) )
     )
  {
    /* GL1 should never holds Tx device without Rx device. If it happens, something is wrong.
       Force a crash to capture the problem for debugging.
    */
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_2_G("Crash for debug,holding tx chain(status %d) without rx (status %d)",
			  (uint8)tx_trm_status, (uint8)rx_trm_status);
  }
  return;
}

/*===========================================================================

FUNCTION  grm_coll_async_info_cb

DESCRIPTION
  This function handles the TRM coll event callback for both GSM clients.
  This asynchronous call may happen from the context of any TRM client.
  If the event is not intended for this SUb. It is ignored.
DEPENDENCIES
 none

RETURN VALUE
  VOID

SIDE EFFECTS
  Posts a message to the RR task queue.

===========================================================================*/
static void grm_coll_async_info_cb(
              trm_idle_coll_event_data  *idle_coll_event_info,
              trm_client_enum_t         client,
              grm_tag_t                 grm_tag )
{
  gas_id_t   gas_id = gl1_map_client_id_to_gas_id(grm_map_trm_client_id_to_grm_client_id(client));

  if(idle_coll_event_info == NULL)
  {
    MSG_GERAN_ERROR_1_G("grm_coll_async_info_cb no valid idle_coll_event_info grm_tag:%d", grm_tag);
    return;
  }

  if(idle_coll_event_info->action_info.asid != geran_map_gas_id_to_nas_id( gas_id))
  {
    MSG_GERAN_HIGH_0_G("MPH_IDLE_COLL_FALLBACK_IND not send to RR. AS_ID mismatch, not intended for this RAT ");
  }
  else
  {
    gas_id = check_gas_id(gas_id);
  
    if (L1_ftm_mode_enabled())
    {
      MSG_GERAN_HIGH_0_G("FTM is enabled, MPH_IDLE_COLL_FALLBACK_IND not sent to GRR");   
    }   
    else  
    {  
    l1_send_coll_msg_to_rr( idle_coll_event_info, gas_id);
  
      MSG_GERAN_HIGH_1_G("MPH_IDLE_COLL_FALLBACK_IND sent to RR. fallback_type = %d", idle_coll_event_info->data.idle_fallback_data.fallback_type); 
    }
  }
  return;
}

/* EOF */
