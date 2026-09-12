#ifndef GPL1_DUAL_SIM_H
#define GPL1_DUAL_SIM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     L1 DUAL SIM GENERAL PROCESSING HEADER FILE

GENERAL DESCRIPTION
This module handles the NULL mode from TASK context, when in DUAL SIM mode of
operation.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gpl1_dual_sim.h#3 $
$DateTime: 2020/07/28 11:01:41 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
24/07/20   sal      CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
28/08/18   rv        CR2303543: Fix for Transfer SUB not allowing TA for other SUB requests with priority ACCESS_LOW
01/02/18   ap       CR2022526 X tech SUB1 page denied by GSM SUB2 when reading PCH in Transfer
02/01/17   sk       CR2001022  Delay RANE for RACH till we get QTA Cleanup
19/08/16   dp       CR1055018 Make FIND_BCCH_MODE QTA's check for invalid device granted from TRM
12/06/17   sn        CR2054908 Handling to back to back DR mode change requests
20/07/16   sp        CR1040702: Allow TA during 2Phase access
29/03/16   mn        CR995137 Access tune away support: Allowing other tech's paging at GPRS access
29/10/17   km        CR2133917 GL1 support for TA.3.0 L+L
01/09/16   bg        CR1061674 Updated the scaling factor to 3ms to open the GTA gap 1 frame early
06/02/17   km        CR1040468 Fix for other sub X tech page miss during G2X TA
04/09/17   sn        CR2102884 Handling of Single block TBF structure before its intilalized
08/08/17   nm        CR2040229 Enable PCH read in 2 phase access for both Idle sub and data sub using GBTA
15/12/16   sk        CR1101677 Do not call qta_fw_rf_deinit from sch handler if QBTA is not supported
18/11/15   cah       CR900214 - [WLAN+WAN] Failed in acquiring the Rxd chain in Single SIM Mode during Voice Call Attempt
20/10/15   ap        CR923515 Do not extend duration of diversity chain if denied by TRM 
15/10/15   ap        CR923459 GSM logging procedure for Attach seen as CS instead of PS registration
28/09/15   mko       CR914802 While moving from DTM to transfer mode set the GBTA status to ALLOWED before retaining the lock
06/10/15   sp        CR914315 Update frame duration when Unlock Cancelled received for ACQuisition
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
02/09/15   nm        CR897078 Change the reason from Access to TRAFFIC if RACH for CS is received
21/09/15   nm        CR767743 Using actual duration in trm unlock cb for unlock by event
04/09/15   cjl       CR892689 Ensure BPLMN client does not retain TRM chain when aborted
07/09/15   js        CR903209 Backing out CR891616
08/09/15   cah       CR892575: DR DSDS: Add support for diversity chain unlock in transfer mode
02/09/15   ap        CR897976 Reduce the duration of TRM resource when requesting for Reselection
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
26/08/15   js        CR891616 Setting correct duration to tune away table 
20/08/15   fan       CR893599 Remove flag indicating that other SUB is going active check during cancelation
20/08/15   ap        CR849948 Race condition between handling MPH_SET_PRIORITY_IND and TRM grant callback 
13/08/15   nm        CR889817 add TRM_IRAT_MEAS reason to GTA compatible reasons
05/08/15   zf        CR885743: Set IRAT measurement as QTA reason
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types
15/07/15   fan/akh   CR857332 FR19329: WMGR support for rescheduling non-PCH activities
12/07/15   nm        CR861458 Fix the interworking issues between PCHXFER, DRDSDS, RX/TX split
09/07/15   cah       CR811835: FR 26893: Band-by-Band OOS and BPLMN Search - GL1
30/06/15   am        CR863251 Change REDUCE_FRAMES_TASK_TO_ISR_LATENCY back to 3
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
24/06/15   ap        CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR 
23/06/15   fan/akh   CR859092 Adapt WMGR Cancel handling to RACH Rescheduling in DSDA, DR-DSDS
06/01/15   js       CR814275 GL1 changes for ROTA interface during extended paging
29/05/15   sjv       CR845608 When panic reset is triggered by L1 and is actioned by L1 Task after ISR finishes, Irat SM abort is not handled
05/06/15   ab        CR848697 : GBTA not allowed after fall back and UE does GTA instead 
04/06/15   ab        CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
03/06/15   aga       CR847218 Add Channel Maintenance as QTA reason
15/04/15   pa        CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
07/05/15   fan/akh   CR834237 FR19329 Full G wake-up cancellation + code cleanup
06/05/15   cjl       CR831599 Resolve persistent BCCH denial issue
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
21/04/15   sjw       CR822898: removal of deprecated ACQUISITION reasons
03/04/15   ab        CR809082 Do not schedule PWR_MON_EVENT for missed page ,
                     when Transfer Sub is Releasing TBF.
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
30/03/15   ap        CR811753 FR22272 - TRM Unification - GL1 changes to support
                                        SysInfo priority inversion during early camping
30/03/15   nm        CR765891 Allowing TA for TDS during PS RACH
27/03/15   ap        CR811054 FR22272 - TRM Unification  - Extended Paging with PBR
24/03/15   ap        CR811053:FR22272 - TRM Unification  - Access and Traffic attempts
23/03/15   ap        CR811046:FR22272 - TRM Unification  - Removal of _HI reasons/UNLOCK_IMMEDIATELY
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
24/03/15   sjw       CR812407: update for new MPH_SET_PRIORITY_IND payload
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
17/02/15   br        CR796211 QTA:Don't  start QTA from GL1, if gl1 deactivation is going on.
20/02/15   sjw       CR793380: adaptive TRM durations for acquisition mode
04/02/15   nk        CR767734 Allowing TA during 2-phase access
13/02/15   pa        CR792105: Allow either GBTA or IRAT.
20/02/15   cjl       CR789907 FR24151 Reduce frequent tuneaways from LTE for CHANNEL_MAINTENANCE
19/02/15   sp        CR776381 Ensure DEMOD_PAGE_INV_HI is also a GBTA reason
24/01/15   mko       CR785110 GL1 must not register unlock callback if DS_ABORT is received during the FW_LOAD state
27/01/15   nm        CR786009 Fix the merge error from CR668029
16/01/15   aga       CR780157: Remove transition from ACTIVE_GAP to ACTIVE for data call abort use cases
13/02/15   js        CR782437 Providing API to WL1 for ROTA enhancement
13/02/15   pa        CR756156:G2X RF device handling using TRM apis.
29/12/14   sp/aga    CR774163: Update GFW to active_gap when opening GTA gap
16/10/14   aga       CR735726  Remove obsolete gta_set_wcdma_blacklisted functionality
27/01/15   am        CR787063 Increase RESEL_EXITS_COUNTDOWN_THRESHOLD
21/01/15   pa        CR758792:RxD support for multi-sim.
12/01/15   sjv       CR756154 Enhance Band Registration for Activities
06/01/15   am        CR390109 Improved DSDS cell reselection
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   ws        CR768376 Q6 compile warning fixes
24/07/14   nm        CR698030 Use correct trm client id to query other sub trm reason when pch in transfer is enabled
25/11/14   sjv       CR759816 Remove reason checks for G2T and G2W TA for TBF unlock_by
29/10/14   ab        CR745429 - Featurization correction for PCHXFER for SS / CR merges.
23/10/14   sjv/aga   CR744425 using correct gas_id and TRM reasons in gl1_is_granted_geran_sub_doing_reselection
22/10/14   ws        CR744361 Data+MMS reduce MSC if commanded by CFCM
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build
30/09/14   mko       CR728501 If TS adjustment is missed then send DS_ABORT & move to idle when TRM is granted back
22/09/14   ap        CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
17/09/14   mko       CR725121:Add TRM_CHANNEL_MAINTENANCE &TRM_RESELECT reasons as G2T compatible reasons
20/05/14   ap        CR668029 Handling Serving Cell BCCH reads during Early Camp when X Rat is in Data Traffic
22/08/14   mk        CR712000 Shorten abort period for serving BCCH normal TBF suspension to avoid missing TS change
                     and handle abort when suspension timer is running
28/08/14   sjv       CR717166 pending activities are organized in an array for trm band release to avoid overwrite in case of diff. activities
31/07/14   xz        CR697883 Not allow GBTA if current idle frame is doing TDS IRAT in xfer mode
22/08/14   aga       CR713731 Updates to F3s and other recent changes
11/08/14   sjw       CR706459: use L1_COMPLETE_DS_ABORT for race condition between abort and release
07/08/14   aga       CR689162: Abort tune away gap mechanism triggered if lower TS change is imminent
06/08/14   ka        CR699964 Implement Idle RXD support in GL1
04/08/14   nm        CR663403 Use TRM_BROADCAST_ACCESS trm reason in page reorg mode instead of DEMOD_PAGE_MAX_SENS
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
25/07/14   aga       CR698326 - avoid scheduling monitors when TBF suspension is imminent
16/06/14   nk        CR678832 Override TRM reason to QTA compatible reason one and avoid updating TRm reservation for TA
10/07/14   js        CR593074- ROTA/QBTA Feature implementation
16/07/14   aga       CR695332 PCH scheduling  is not initiated, GBTA is not stated
11/07/14   sjw       Standardise use of gl1_is_geran_tuneaway_allowed()
14/07/14   cs        CR693967 Get DR-DSDS mode during startup
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
02/07/14   aga       CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution
30/05/14   ws        CR657674 Data + MMS Phase 1
27/06/14   cs        CR685966 Add DR INFO async callback support
24/06/14   cs        CR684714 Add back in functionality removed for initial updated TRM API support
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
25/04/14   dg        CR654695 Back-out CR649702 to avoid regression
18/03/14   ap        CR629844 SGLTE BPLMN support when G1 in PS call in G+G - PS Hopping
18/03/14   ap        CR614949 FR 19574: Performing LTE/TDS measurements while GERAN is in connected mode using the second WTR
03/03/14   am        CR630494 Don't release FW in the same frame as FW setup
12/03/14   ssh       CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
06/03/14   br        CR626780, RACE condition between TRANSFER G sub and IDLE G sub, issuing SCH in G2x gap.
28/02/14   ap        CR624060 Further changes to FEATURE_PBR_MARKOV applied
28/02/14   sjw       CR620569: adaptive unlock callback for acquisition
21/02/14   ap        CR620029 Ensure FEATURE_PBR_MARKOV is correctly applied
19/02/14   ap        CR600310 GSM L1 to send channel Id to GSM FW corresponding to the primary RF device
2/17/14    ab        CR612460 Fix race condition for power monitor update
09/01/14   zf        CR598637: DRX only updates TRM booking if DRX activity is matching with the TRM booking
24/01/14   mko       CR593859:Extend the TRM if SI read is in less than 11 frames after the paging in reselection state(HDR in BG Traffic)
13/02/14   ap        CR612587: Ensure call RF exit GSM mode when releasing TRM in DSDA mode
17/01/13   sk        CR600574 Ensure GBTA state is reset from Active if TRM released/reserved without the activity being run
24/12/13   ssh/sp    CR600563: Check for ISR overrun after Fw Init before issuing commands to GFW
03/02/14   gk        CR591181 Change WARM_UP_OVERHEAD to 150 instead of 100
19/12/13   js        CR581840 IDLE SUBs releasing TRM immediately after GRANT leading to unnecessary GTA Gaps
11/12/13   ap        CR586661 GPRS suspension during SI read after W2G reselection
17/12/13   sk        CR586741 Update TRM reservation to QTA compatible one if WCDMA is in QTA mode
16/12/13   dv        CR569122 High missed page rate due to W ID search
12/01/14   sjw       CR592154: use new TRM reason for ACQUISITION_MAX
07/01/14   sjw       CR596709: created new grm_release_or_exchange() function
24/12/13   ap        CR592011: FR 17482: Page Blocking Management in Multi-SIM Single Radio Devices amendment
14/12/13   sjw       CR589502: Install different unlock callback for change of acq priority
13/12/13   ap        CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
04/12/13   ap        CR584084 Addressing WCDMA reconfirmation search whilst GERAN in Transfer
04/12/13   aga       CR549390 G2W_TA IRAT Enable W ID search and reconf in case the other G sub is in data call
03/12/13   js        CR574803 TRM extended for Ncell Acquisitions after PCH decode leading to missed page on next SUB
01/12/13   sk        CR583361 Backout CR549390
27/11/13   aga       CR549390 G2W_TA IRAT Enable W ID search and reconf in case the other G sub is in data call
19/11/13   ssh/ip    CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
14/11/13   zc        CR569943:Use correct resource for trm_freq_request
15/11/13   nm        CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
11/11/13   sk        CR573517 Ensure correct sys mode used by GL1 Arb while giving coex priority
31/10/13   sai       CR567424: Enhance G2W TA suspension mechanism.
24/10/13   sai       CR565751: Fix reference FN for G2W TA suspension reschedule mechanism.
21/10/13   sai       CR563915: Facilitate G2W TA after TBF suspend for BCCH decode.
05/12/13   sjw       CR583608: Use TRM lock acquired using inter-RAT exchange
12/02/13   sk        CR573517 Ensure correct sys mode used by GL1 Arb while giving coex priority
28/11/13   sjw       CR578873: Priority Inversion Management for Multi-SIM (PIMMS)
19/10/13   sk        CR563877 Ensure BCCH reads are given priority over PCH when doing reselection and consider early wakeup time into TRM reservation
31/10/13   sai       CR567424: Enhance G2W TA suspension mechanism.
24/10/13   sai       CR565751: Fix reference FN for G2W TA suspension reschedule mechanism.
13/11/13   ap        CR562878 FR 17482: Page Blocking Management in Multi-SIM Single Radio Devices
11/10/13   sk        CR556552 Ensure idle sub doesn’t invert the priority when other sub using global override for reselection in QTA scenario.
20/09/13   js        CR522426 Multi SIM CBCH priority decision to be taken only when we have more than one sub active
05/10/13   sk        CR554665 Backout the power opt temperorily(CR537690)
14/09/13   sp        CR524905 Update Minimum frames required for BPLMN activity
21/08/13   zf        CR524993: DSDA, when W in BG traffic, XO does not shutdown.
25/09/13   aga       CR547585 - G2W - avoid trm_gta_preemption_callback processing while suspended
24/09/13   zf        CR537690: Reduce G wakeup timeline for MS mode
26/09/13   cgc       CR548645 Generic QTA for 3.0 DSDS bringup changes
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
14/09/13   ap/aga    CR545423 - G2W Tune Away Feature
02/09/13   nm        propagation of CR511322 Reset trm request state if RACH is aborted
26/08/13   gk        CR484897:W2G handover using exchange of TRM lock
10/09/13   cs        CR532298 Integrate all wakeup-mgr changes for Multi-SIM
20/08/13   sk        CR530970 Ensure GBTA works properly in DSDS config
15/08/13   sk        CR524039 GBTA bringup changes
07/08/13   gk        CR509196 :Abort the previous power scan, in case of reception of back-to-back power scan request
01/08/13   sk        CR521884 Priority inversion not working in different DRX combination
12/07/13   cs        CR509747 Apply frequency drift to TRM_DENIED SUB(s)
05/07/13   sk        CR509202 Priority inversion and alternate page skipping changes
02/07/13   sk        CR507513 TRM advanced call back integration to Triton-TSTS
01/07/13   ip        CR506628 - FC power code changes
21/06/13   cs        Major Triton TSTS Syncup
13/06/13   pjr       CR489588: Added check in IS_WCDMA_QTA_SUPPORTED() to return false in SS and DSDA
06/06/13   cs        Further TSTS Updates
28/05/13   ap        CR461268: DSDS XO management + async time drift problem
21/05/13   cs        TSTS Updates
17/05/13   zf        CR488883: Do not use QTA enabled TRM reason before W supports QTA.
27/03/13   am/ap     CR432888 Adjust BPLMN frames for acqusition of BCCH in DSDS mode
26/04/13   zf        CR477992: Fix G2W IRAT issue
29/04/13   ws        CR481359 enable GTA support
25/04/13   zf        CR479905: Reduce default FW load frames to 2
06/04/13   sv        CR462075: Corrected the logic when ignoring GRR commands in GTW PLMN cases.
07/03/13   pjr       CR460494 added macro OTHER_GSM_CLIENT_IS_INACTIVE
12/02/13   ap        CR450842 DSDS: PCH not being decode on SUB2 even though TRM is granted
07/02/13   ap        CR449859 Always enter GFW state MEAS_ACTIVE for gonly sub else
                              enter GFW ACTIVE state
07/02/13   ap        CR449348 DSDS W/G+G IRAT MO Voice: HO from W2G requires attention
18/05/12   mc        DSDS CR359187: LTA activity is granted TRM when GLite FW is still loaded - recovery code
14/02/13   pjr       CR452272 added support to set multisim mode at activation time
09/05/11   sjw       DSDS frame overhead reduction for SCMM.
02/03/11   njt       DSDS CR276931 - BPLMN suspension changes
25/01/11   nt        Added suspension state to L1_multi_sim_acquiring.
10/01/11   ap        DSDS CR270377 SYNC SIM is disabling L1 ISR while ASYNC SIM is still
                     active when consecutive start/stop GSM mode req
23/11/10   nt        DSDS CR250375 Changed the REDUCE_FRAMES_TASK_TO_ISR_LATENCY value
22/11/10   nt        DSDS CR261791 Perform post idle init is other SIM has the lock
13/10/10   nt        DSDS - CR258203 - Missing pages in C+G and W+G modes
24/09/10   nt        DSDS CR253591 Introduced MULTI_SIM_CCCH_FW_LOAD_GUARD for CCCH timeouts
13/08/10   nt        DSDS CR249783 fix to prevent premature release
06/08/10   nt        DSDS CR249192 - Lock out using TRM for FEE. Use FAIR estimate
28/07/10   nt        DSDS - Fixes for GplusG mode
20/07/10   nt        DSDS - Enable GpluG operating mode
09/07/10   nt        Move ASYNC to SYNC sim swap to ISR context
30/06/10   sjw       Introduction of GRM framework
12/05/10   nt        CR232162 - DS delayed BPLMN abort conf's until fully completed
12/04/10   nt        CR232391 - Add the changes to handle iRAT MPH_RECONNECT_REQ
12/04/10   nt        CR229761 - changes to reduce standby current of DS G+G
23/03/10   nt        Changes to prevent BPLMN mechanism from getting into the wrong state
===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "ms.h"
#include "l1_fm.h"

#include "gpl1_grm_intf.h"
#include "rfm_device_types.h"
#include "cmgr_api.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "l1i.h"
#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "gl1_msg_g.h"

#if defined ( FEATURE_QSH_EVENT_NOTIFY_TO_QSH ) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
#include "l1_log_qsh.h"
#endif 

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/* Used for frame based scaling */
#define FRAME_SCALING_FACTOR    (4726) /* 4.615 * 1024     */
#define QSYM_SCLK_SCALING_NUM   (12)   /* One QSYM = 12 / 13 usecs */
#define QSYM_SCLK_SCALING_DENOM (13)
#define FRAME_SCALING_SHIFT     (10)   /* divide by 1024 */
#define TRM_AD_INFINITUM        (0x7FFFFFFF)
#define SCALING_FACTOR_MS       (3072) /* 3 * 1024 : (3ms)    */

/* Convert msecs into GSM 4.615msecs frame ticks */
#define MSECS_TO_FRAMES( msecs_duration ) \
          ( ( msecs_duration << FRAME_SCALING_SHIFT ) / FRAME_SCALING_FACTOR )

#define FRAMES_TO_MSECS( gsm_frames ) \
          ( ( gsm_frames * FRAME_SCALING_FACTOR ) >> FRAME_SCALING_SHIFT )

#define MODULO_FRAME_SCALING_FACTOR( msecs_duration ) \
          ( ( msecs_duration << FRAME_SCALING_SHIFT ) % FRAME_SCALING_FACTOR )

#define GL1_DRDS_DBG_HDR "GL1_DRDS:"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

#define COMPLETE_PROCEDURE_DURATION 0xFFFFFFFF

#define PTM_IDLE_FRAME_FREQ_REGISTRATION_DURATION 3
#define PTM_SUSP_FREQ_REGISTRATION_DURATION 12
#define GPRS_IDLE_AWAKE_FREQ_REGISTRATION_DURATION 4
#define GPRS_IDLE_PM_REGISTARTION_DURATION 3


/*======================TRM DURATIONS========================*/
#define BCCH_DURATION_MS                   37 /* 8 frames for 1 BCCH read */
#define RESELECTION_DURATION_MS            100
#define RETAIN_LOCK_DEFAULT_FRAME_DURATION 150

/*As per curr PIMMS design, the Acq Inv is working on maximum of 1sec duration 
  - so retaining lock for a maximum of 204frames when unlock cancelled is received*/
#define RETAIN_UNLOCK_CANCEL_FRAME_DURATION 204 /*FIND_BCCH RR max timer for inversion is 1sec*/

/* To cover extended bcch minimum frames left are 40 e.g. extended bcch
   at frame 5 (+ 6 bcch read) finishes at frame 11, then for the next
   bcch FN i.e. 52  we get 40 frames. */
#define RESEL_ENTER_COUNTDOWN_THRESHOLD 40

/* 13 is the worst case frame required for idle sub to do the TRM request & notify, if it is too less then idle sub could miss the BCCH occasion
   13 = 1+5+2+3+2
   1 - for cell reselection state machine to back to other state
   5 - is the worst case overhead for transfer sub to align the radio block for GTA gap including 1 idle frame
   2 - transfer sub to release TRM
   3 - idle sub to get TRM and wait for rf/fw warmup time from ISR
   2 - to give some margin
*/
#define RESEL_EXITS_COUNTDOWN_THRESHOLD  13

#define IS_RESEL_OR_ACQ_REASON(rsn) (\
((rsn) == TRM_CHANNEL_MAINTENANCE)          || \
((rsn) == TRM_CHANNEL_MAINTENANCE_INV) || \
((rsn) == TRM_ACQUISITION_CELL_SELECT) || \
((rsn) == TRM_ACQUISITION_CELL_SELECT_INV)       || \
((rsn) == TRM_ACQUISITION)       || \
((rsn) == TRM_ACQUISITION_INV)      \
)

#define IS_ACQ_REASON(rsn) (\
((rsn) == TRM_ACQUISITION_CELL_SELECT) || \
((rsn) == TRM_ACQUISITION_CELL_SELECT_INV)       || \
((rsn) == TRM_ACQUISITION)       || \
((rsn) == TRM_ACQUISITION_INV)      \
)
#define IS_URGENT_ACCESS(rsn) (\
((rsn) == TRM_ACCESS)          || \
((rsn) == TRM_ACCESS_LOW)      || \
((rsn) == TRM_TRAFFIC)            \
)

/*----------------Call in task definitions----------------*/
/* This defines the structures and function prototypes
for the TRM interaction within the task, called via ISR */

/*~~~~~~~~~~~multi_sim_rf_fw_load_t~~~~~~~~~~~*/
typedef struct{
  IMH_T                           header;
  grm_client_enum_t               client_id;
  gas_id_t                        gas_id;
} multi_sim_rf_fw_load_t;

/*~~~~~~~~~~~multi_sim_panic_reset_t~~~~~~~~~~~*/
typedef struct{
  IMH_T                           header;
  gas_id_t                        gas_id;
  l1_panic_reset_type_T           error_code;
  rr_l1_message_id_enum_T         reset_response;
} multi_sim_panic_reset_t;

/*~~~~~~~~~~~multi_sim_gonly_deactivate_t~~~~~~~~~~~*/
typedef struct{
  IMH_T                           header;
  gas_id_t                        gas_id;
} multi_sim_gonly_deactivate_t;

/*~~~~~~~~~~~~~~gl1_complete_ds_abort_t~~~~~~~~~~~~~*/
typedef struct
{
  IMH_T                   header;
  grm_client_enum_t       client;
  rr_l1_message_id_enum_T msg_id;
  gas_id_t                gas_id;
} gl1_complete_ds_abort_t;

typedef enum
{
  CS_RACH,  /* RACH for CS Traffic */
  PS_RACH,  /* RACH for PS Traffic*/
  NO_RACH   /* INVALID value */
} rach_type_t;

void gl1_set_rach_type(rach_type_t rach_type, gas_id_t gas_id);

rach_type_t gl1_get_rach_type(gas_id_t gas_id);


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
                                             gas_id_t               gas_id);



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
void gta_handle_abort_2phase_access_callback(grm_client_enum_t       client,
                               trm_unlock_event_enum_t event,
                               uint32                  unlock_by_sclk,
                               gas_id_t                gas_id);

void grm_gta_2phase_access_preemption_callback(trm_unlock_callback_data *unlock_data_ptr);


#endif /*FEATURE_GTA_2PHASE_ACCESS*/

#ifdef FEATURE_GTA_DURING_PS_RACH
/*===========================================================================

FUNCTION     gta_handle_abort_RACH_callback

DESCRIPTION  L1 task handler for GTA abort callback which has been triggered from
             Other tech calling the GTA unlock callback  during RACH state (for PS services)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void gta_handle_abort_RACH_callback(grm_client_enum_t       winning_client,
                                                 trm_reason_enum_t       winning_client_reason,
                                                 trm_unlock_event_enum_t event,
                                                 uint32                  unlock_by_sclk,
                                                 gas_id_t                gas_id);

#endif /* FEATURE_GTA_DURING_PS_RACH */
extern void gl1_handle_abort_callback(
              grm_client_enum_t       grm_client_id,
              trm_unlock_event_enum_t event,
              uint32                  unlock_by_sclk,
              gas_id_t                gas_id );

extern void gl1_handle_grant_callback(
              grm_client_enum_t          grm_client_id,
              trm_duration_t             duration,
              gas_id_t                   gas_id );

#ifdef FEATURE_GSM_RX_DIVERSITY
void gl1_handle_drx_unlock(grm_client_enum_t drx_client, gas_id_t gas_id);
void gl1_hw_handle_idle_rxd_rf_fw_exit(  grm_client_enum_t divrx_client_id, gas_id_t gas_id );
#endif

#ifdef FEATURE_G2X_TUNEAWAY
void g2x_ta_handle_abort_callback(grm_client_enum_t       client,
                               trm_unlock_event_enum_t event,
                               uint32                  unlock_by_sclk,
                               uint32                  winning_client_duration,
                               trm_tuneaway_enum_type  ta_info,
                               gas_id_t                gas_id);
#endif /* FEATURE_G2X_TUNEAWAY */
#ifdef FEATURE_SGLTE
extern void gta_handle_abort_for_l_or_t_callback(
              grm_client_enum_t       client,
              trm_unlock_event_enum_t event,
              uint32                  unlock_by_sclk,
              gas_id_t                gas_id);
#endif /* FEATURE_SGLTE */

#ifdef FEATURE_G2X_TUNEAWAY
/* These variables are used to store the last winning client, reason for the last winning client etc.
 * during the GTA gap. These are initialized to invalid values at the end of the GTA gap, inside
 * dsds_init_pch_suspension function. These are updated at the start of the gta_preemption_callback
 * function. The purpose is, we don't want the unlock callback to be called again, if the last winning client
 * and the current winning client are calling this callback for the same reason. The reason to avoid this repeated
 * callback is to avoid delaying the tbf abort time repeatedly.
 */
typedef struct {
  grm_client_enum_t           last_client;
  trm_unlock_event_enum_t     last_event;
  uint32                      last_unlock_by_sclk;
  trm_reason_enum_t           last_unlock_by_reason;
} gpl1_gprs_tbf_abort_cb_store_data_t;

extern gpl1_gprs_tbf_abort_cb_store_data_t gpl1_gprs_tbf_abort_cb_store_data[];
#ifdef FEATURE_GTA_2PHASE_ACCESS

typedef struct {
  uint32    pch_suspension_FN;
  boolean   tick_susp_state_machine;
  boolean   disable_rx;
} gta_2p_susp_info_t;

extern gta_2p_susp_info_t gta_2p_susp_info[];

#endif /*FEATURE_GTA_2PHASE_ACCESS*/
#endif


boolean  gl1_set_gfw_to_active(gas_id_t gas_id);

boolean gl1_detect_page_collision_with_other_sub( gas_id_t gas_id );

#if defined FEATURE_QBTA
boolean gl1_check_qbta_allowed(gas_id_t gas_id);
#endif
grm_client_enum_t gl1_get_client_id_of_other_sub_in_transfer(gas_id_t gas_id);

#define G2X_TA_SEQ  0xFFFFFFFF

typedef struct
{  
  cmgr_scenario_type_e type;
  uint32 scenario;
  uint32 update_scenario;
  uint32 priority;
  uint32 flow_ctrl_time_msec;
  uint64 start_timestamp;
  uint32 duration_msec;
}gl1_concman_config_params_type;


extern cmgr_scenario_status_e gl1_concman_scenario_config_request(gl1_concman_config_params_type scenario_params, gas_id_t gas_id);


/*===========================================================================

FUNCTION  l1_isr_find_bcch

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_multi_sim_find_bcch( gas_id_t gas_id );

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
void l1_isr_multi_sim_null(gas_id_t gas_id);

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
void l1_isr_multi_sim_idle( gas_id_t gas_id );
/*===========================================================================

FUNCTION  l1_isr_multi_sim_decoupled

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_multi_sim_decoupled( gas_id_t gas_id );
/*===========================================================================

FUNCTION  l1_isr_handle_multi_sim_find_sch_command

DESCRIPTION
   Handle the main command mentioned in the function name in various modes.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_handle_multi_sim_find_sch_command( gas_id_t gas_id );

/*===========================================================================

FUNCTION  l1_isr_handle_multi_sim_find_bcch_command

DESCRIPTION
 used for dual sim acqusition activities and reselection
DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_isr_handle_multi_sim_find_bcch_command(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gl1_handle_grm_requests

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
extern boolean gl1_handle_grm_requests( grm_conn_type_t connection_type,
                                        boolean  reset_state,
                                        ARFCN_T           arfcn_info,
                                        grm_divrx_request_type req_type,
                                        gas_id_t gas_id );

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
void l1_init_multi_sim_data( gas_id_t gas_id );

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

  +++++ This must occur in task context due to the call to trm_release +++++

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void l1_swap_multi_sim_sync_state_or_deactivate( gas_id_t gas_id );

/*===========================================================================

FUNCTION   call_in_task_multi_sim_panic_reset

DESCRIPTION
   Sends a message to the layer1 task telling it to action the panic reset
   mechanism

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void call_in_task_multi_sim_panic_reset( gas_id_t gas_id );

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
void call_in_task_gplusg_deactivate( gas_id_t gas_id );

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
extern void l1_set_global_override_trm_reason( trm_reason_enum_t reason, gas_id_t gas_id );

extern void l1_clear_global_override_trm_reason( gas_id_t gas_id );

extern boolean gl1_get_trm_global_overide_status( gas_id_t gas_id );
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
void gpl1_set_invert_sys_info_trm_priority(boolean invert_sys_info_priority, gas_id_t gas_id );

boolean gpl1_sys_info_trm_priority_inverted(gas_id_t gas_id);

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
trm_reason_enum_t l1_get_global_override_trm_reason( gas_id_t gas_id );

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
void gl1_multi_sim_fw_rf_load( grm_client_enum_t client_id, gas_id_t gas_id );

/*===========================================================================

FUNCTION  grm_abort_callback

DESCRIPTION
  This function handles the trm abort callback for both GSM clients.
  And changes the trm_status within the clients global data, this is picked
  up by the ISR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void gpl1_gprs_complete_ds_abort(grm_client_enum_t       client_id,
                                        rr_l1_message_id_enum_T pending_msg,
                                        gas_id_t                gas_id);


#ifdef FEATURE_G2X_TUNEAWAY
/*===========================================================================

FUNCTION  grm_gta_preemption_callback

DESCRIPTION
  This function handles the TRM abort callback for the GSM client
  that is doing the Packet transfer
  And changes the trm_status within the clients global data, this is picked
  up by the ISR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void grm_gta_preemption_callback(
              trm_unlock_callback_data *unlock_data_p );

/*===========================================================================

FUNCTION  grm_tbf_enh_grant_callback

DESCRIPTION
  This function handles the enhanced GRANT callback for the GSM client
  that is doing the Packet transfer . It changes
  the trm_status within the clients global data, this is picked up by the ISR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_tbf_enh_grant_callback
(
  grm_client_enum_t               client,
  trm_grant_return_enum_type      grant_status,
  trm_duration_t                  duration
);





boolean gpl1_gprs_get_gta_config(gas_id_t gas_id);
boolean gpl1_gprs_is_this_the_active_tbf_id(gas_id_t current_gas_id);
boolean gl1_page_collision_with_x_sub( grm_client_enum_t client,
                                       l1_fm_priority_T priority,
                                       gas_id_t gas_id  );



#endif /* FEATURE_G2X_TUNEAWAY */



/*===========================================================================

FUNCTION  gpl1_multi_sim_calculate_grm_reservation_time

DESCRIPTION
  This function determines the required TRM request period in sclcks,
  factoring in the time duringt he frame at which this is called.

DEPENDENCIES
  None

RETURN VALUE
  TRM period request in sclcks.

SIDE EFFECTS
===========================================================================*/
uint32 gpl1_multi_sim_calculate_grm_reservation_time( uint32   requested_FN,
                                                      gas_id_t gas_id );

/*===========================================================================

FUNCTION  gl1_wmgr_calculate_grm_reservation_time

DESCRIPTION
  This function determines the required TRM request period in sclcks,
  factoring in the time during the frame at which this is called.

DEPENDENCIES
  None

RETURN VALUE
  TRM period request in sclcks.

SIDE EFFECTS
===========================================================================*/
uint32 gl1_wmgr_calculate_grm_reservation_time(gas_id_t gas_id);

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
uint32 gpl1_multi_sim_convert_frames_to_sclcks( uint32 Gframes );

/*===========================================================================

FUNCTION  GPL1_GPRS_HANDLE_MULTI_SIM_FIND_BCCH

DESCRIPTION
  This function handles the L1 Task in dual sim find bcch mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_multi_sim_find_bcch(IMH_T *msg_header);

/*===========================================================================

FUNCTION  gpl1_gprs_handle_multi_sim_idle

DESCRIPTION
  This function handles the L1 Task in idle mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_multi_sim_idle(IMH_T *msg_header, gas_id_t gas_id);

/*===========================================================================

FUNCTION  gpl1_gprs_handle_multi_sim_null

DESCRIPTION
  This function handles the L1 Task in dual sim null mode, and the messages
  received within the task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_multi_sim_null( IMH_T * msg_header );
/*===========================================================================

FUNCTION  gpl1_gprs_handle_multi_sim_decoupled

DESCRIPTION
  This function handles the L1 Task in dual sim decoupled mode, and the messages
  received within the task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_multi_sim_decoupled(IMH_T *msg_header);


/*===========================================================================

FUNCTION  l1_ds_isr_inactive_mode

DESCRIPTION
  Do top level ISR processing for mode mentioned in function name.
  This mimicks l1_isr_inactive_mode but it does not tear down the gstmr handler
  as there is still a sim active.

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_ds_isr_inactive_mode( gas_id_t gas_id );

/*===========================================================================

FUNCTION  L1_send_PH_RANDOM_ACCESS_CNF_with_result

DESCRIPTION
  This function sends the PH_RANDOM_ACCESS_CNF to L2.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_PH_RANDOM_ACCESS_CNF_with_result(const gas_id_t gas_id,
                                              const geran_result_t rach_result);

/*===========================================================================

FUNCTION gl1_ms_switch_multi_sim_data

DESCRIPTION
             Only used for Dual SIM/Dual data space.  Switches the working
             data pointer to the data space indicated by gas_id.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void gl1_ms_switch_multi_sim_data(gas_id_t gas_id);

/*===========================================================================

FUNCTION  gpl1_multi_sim_bplmn_decode_bcch_inactive

DESCRIPTION
  Rteurns the status of the parrallel bcch decode state machine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_multi_sim_bplmn_decode_bcch_inactive( gas_id_t gas_id  );

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
                                 gas_id_t                gas_id );


typedef enum{
  L1_MULTI_SIM_BGRND_HPLMN_NULL,
  L1_MULTI_SIM_BGRND_HPLMN_FW_LOADING,
  L1_MULTI_SIM_BGRND_HPLMN_RUNNING,
  L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING,
  L1_MULTI_SIM_BGRND_HPLMN_TRM_WAIT,
  L1_MULTI_SIM_BGRND_HPLMN_END
}background_hplm_state_t;

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
void l1_multi_sim_bplmn_abort ( void* dummy, gas_id_t gas_id );

/*===========================================================================

FUNCTION  gpl1_get_multi_sim_bplmn_state

DESCRIPTION
  returns the variable background_hplm_state
===========================================================================*/
background_hplm_state_t gpl1_get_multi_sim_bplmn_state( gas_id_t gas_id );
void gpl1_set_multi_sim_bplmn_state( background_hplm_state_t background_hplm_state, gas_id_t gas_id );

int8 gl1_check_num_active_G_subs( void );
extern boolean l1_get_starting_time_elapsed( gas_id_t gas_id );
extern void l1_set_starting_time_elapsed( boolean,gas_id_t gas_id );
extern void l1_set_access_isr_send_mac_sb_failure( boolean value,gas_id_t gas_id );
/* EXTERNS used in gpl1_dual_sim.c */
extern void gl1_load_current_cell_data( gas_id_t gas_id );
extern void gl1_begin_INTERRAT_Reconnect(gas_id_t gas_id);

/* global define for the time to allow the FW/RF to load up on a dual sim build */
#ifdef T_MSM7x27
#define MULTI_SIM_FW_LOAD_FRAMES_DEFAULT 5
#else
#define MULTI_SIM_FW_LOAD_FRAMES_DEFAULT 0
#endif /*T_MSM7x27*/

#define L1_MIN_HDR_INTER_GAP 11 /* Minimum seperation between consecutive QTA gaps */

#ifdef FEATURE_QTA
/*
** 3.0 DSDS for generic QTA now 1 frame earlier to allow for either QTA or LTA, was 0
*/
#define MULTI_SIM_FW_LOAD_FRAMES_QTA 1
#endif


#define ZERO_MULTI_SIM_FW_LOAD_FRAMES_DEFAULT 0
#define SHORT_MULTI_SIM_FW_LOAD_FRAMES_DEFAULT 0
#define MULTI_SIM_CCCH_FW_LOAD_GUARD 10
#define L1_MIN_WCDMA_INTER_QTA_GAP 11 /* Minimum seperation between consecutive QTA gaps */

typedef struct{
  boolean           fw_loaded_and_ready;
  grm_client_enum_t active_client;
  boolean           three_subs_page_colliding;
} GOnly_mode_settings_t;

extern GOnly_mode_settings_t GOnly_mode_settings;


#ifdef FEATURE_GPRS_GBTA

typedef enum {
  GBTA_EV_NONE,
  GBTA_EV_IDLE_PWR_MON  /* event set during PCH Decode */
}grm_gbta_idle_event_t;

typedef enum {
  GBTA_STATUS_NONE,
  GBTA_STATUS_PWR_MON_ACTIVE,      /* Idle Power Monitor in process of being scheduled */
  GBTA_STATUS_PWR_MON_SCHEDULED    /* Idle Power Monitor Scheduled */
}grm_gbta_idle_event_status_t;

typedef enum {
  GRM_GBTA_NONE,
  GRM_GBTA_ALLOWED,
  GRM_GBTA_ACTIVE,
  GRM_GBTA_BLOCKED
}grm_gbta_state_t;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
typedef enum {
  GRM_PCHXFER_DR_NONE,
  GRM_PCHXFER_DR_ON,
  GRM_PCHXFER_DR_OFF,
  GRM_PCHXFER_DR_ON_PENDING,
  GRM_PCHXFER_DR_OFF_PENDING,
}grm_pchxfer_dr_state_t;


typedef struct
{
  IMH_T                   header;
  grm_pchxfer_dr_state_t dr_status;
  gas_id_t                gas_id;
} grm_gprs_lock_exchnage_t;

void call_in_task_grm_gprs_lock_exchange(grm_pchxfer_dr_state_t dr_status,gas_id_t  gas_id);

#endif
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/


typedef struct {
  boolean                          leaving_idle_mode; /* allows not to schedule pwr monitor */
  boolean                          missed_page; /*indicates whether last page was missed*/
  grm_gbta_idle_event_t            idle_event;
  grm_gbta_idle_event_status_t     idle_event_status;
}grm_gbta_info_t;

extern grm_gbta_info_t grm_gbta_info[NUM_GERAN_DATA_SPACES];
extern grm_gbta_state_t            gbta_state;

#endif /*FEATURE_GPRS_GBTA */

extern uint16 MULTI_SIM_FW_LOAD_FRAMES;
#define REDUCE_FRAMES_TASK_TO_ISR_LATENCY 3
#define RF_FW_SETUP_FRAME_DELAY           2
#define RF_FW_WARMUP_FRAME_DELAY          1
#define ACQ_STATE_UNLOCK_BY_SCLCKS        (GRM_SCLKS_SCALING_FACTOR * 5 )/*frames for unlock by*/

#define CHECK_ANY_GSM_CLIENT_HAS_LOCK() \
( !gl1_check_any_subs_trm_status( GL1_TRM_GRANTED ) && \
  !gl1_check_any_subs_trm_status( GL1_TRM_RETAINED_FOR_ACCESS ) )

#define CHECK_ANY_GSM_CLIENT_IS_ABORTING_LOCK() \
( !gl1_check_any_subs_trm_status( GL1_TRM_ABORTING ) )

#define GONLY_OPERATION_IS_ACTIVE() \
( gl1_check_gonly_mode_active() )

#define CHECK_ANY_OTHER_GSM_CLIENT_IS_INACTIVE( gas_id ) \
( gl1_check_any_other_subs_l1_state( gas_id, L1_INACTIVE_MODE ) )

#define GBTA_ALLOWED_CAN_BE_SET(gas_id) \
( !gl1_check_all_other_subs_l1_state( gas_id, L1_INACTIVE_MODE ) )


#define OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) \
( gl1_check_any_other_subs_l1_state(gas_id, L1_TRANSFER_MODE))


#define OTHER_GSM_CLIENT_IN_SINGLE_BLOCK_ALLOC(gas_id) \
( \
  (l1_transfer_data[(gas_id)].current_alloc_type == L1_SINGLE_BLOCK_ALLOC) \
)

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
boolean gl1_check_if_sub_in_two_phase_access(gas_id_t gas_id);

#define IS_TWO_PHASE_ACCESS(gas_id) \
( \
  (l1_transfer_data[(gas_id)].current_alloc_type == L1_SINGLE_BLOCK_ALLOC)  && \
  (l1_transfer_data[(gas_id)].single_block_tbf_ptr != NULL)  && \
  (l1_transfer_data[(gas_id)].single_block_tbf_ptr->two_phase_access == TRUE) \
)
#endif /*FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

#ifdef FEATURE_GPRS_GBTA

/* Definition for GBTA */

#define IS_SUB_LEAVING_IDLE(gas_id) \
(\
  grm_gbta_info[gas_id].leaving_idle_mode \
)

/*
** GBTA STATE MACROS
*/

boolean gl1_grm_is_gbta_allowed(void);
boolean gl1_grm_is_gbta_blocked(void);
boolean gl1_grm_is_gbta_active(void);
boolean gl1_grm_is_gbta_inative(void);
boolean gl1_grm_is_gbta_allowed_or_active(void);
void gl1_grm_is_set_gbta_status(grm_gbta_state_t gbta_status);
grm_gbta_state_t gl1_grm_get_gbta_status(void);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
void gl1_grm_set_pchxfer_dr_status(grm_pchxfer_dr_state_t gbta_status);
grm_pchxfer_dr_state_t gl1_grm_get_pchxfer_dr_status(void);
#endif
#endif

#define SET_GBTA_STATUS(gbta_status)  \
  (gl1_grm_is_set_gbta_status(gbta_status))

#define GET_GBTA_STATE() \
  (gl1_grm_get_gbta_status())


#define IS_GBTA_ALLOWED() \
  (gl1_grm_is_gbta_allowed())


#define IS_GBTA_BLOCKED() \
  (gl1_grm_is_gbta_blocked())

#define IS_GBTA_ACTIVE() \
  (gl1_grm_is_gbta_active())


#define IS_GBTA_INACTIVE() \
  (gl1_grm_is_gbta_inative())

#define IS_GBTA_ALLOWED_OR_ACTIVE() \
  (gl1_grm_is_gbta_allowed_or_active())


#define IS_IDLE_EV_PWR_MON_ACTIVE(gas_id) \
  (grm_gbta_info[gas_id].idle_event_status == GBTA_STATUS_PWR_MON_ACTIVE)


#define IS_IDLE_EV_PWR_MON_SCHEDULED(gas_id) \
  (grm_gbta_info[gas_id].idle_event_status == GBTA_STATUS_PWR_MON_SCHEDULED)

#define IS_IDLE_EV_PWR_MON_SET(gas_id) \
  (grm_gbta_info[gas_id].idle_event == GBTA_EV_IDLE_PWR_MON)

#define OTHER_SUB_RELEASE_PENDING() \
 ( gl1_check_other_sub_release_pending( ))

#define OTHER_SUB_RECONFIG_PROGRESS(frames_required) \
(gl1_check_other_sub_reconfig_progress (frames_required))

#define OTHER_SUB_START_SC_TICK_TRANS(gas_id) \
(gpl1_start_sc_tick_trans(gas_id))

#define OTHER_SUB_STOP_SC_TICK_TRANS(gas_id) \
( gpl1_stop_sc_tick_trans(gas_id))

#define OTHER_SUB_SC_OP_IN_PROGRESS() \
(gl1_check_other_sub_sc_op_progress())

#define OTHER_SUB_WCDMA_ID_SEARCHING_IN_TBF(gas_id) \
(gl1_check_other_sub_id_search_progress( ))

#define OTHER_SUB_IRAT_IN_IDLE_FRAME(gas_id) \
(gl1_check_other_sub_irat_progress( ))

#define OTHER_SUB_NCELL_MEAS_IN_PROGRESS(gas_id) \
(gl1_check_other_sub_ncell_meas_progress())
/* GBTA Assocaited Functions */

#define OTHER_SUB_IN_TBF_SUSP() \
(gl1_check_other_sub_in_tbf_susp())

#define OTHER_SUB_SUSP_PLANNED(gas_id) \
(gl1_other_sub_check_tbf_susp_planned())

#define OTHER_SUB_MISSED_LAST_PAGE(gas_id) \
(gl1_other_sub_check_missed_page(gas_id))

#define SET_MISSED_PAGE_STATUS(missed_page_status,gas_id) \
(\
  (grm_gbta_info[gas_id].missed_page = missed_page_status) \
)

#define OTHER_G_SUB(gas_id) \
(gl1_get_gas_id_of_other_g_sub( gas_id ))

#define OTHER_GAS_ID(gas_id) ((gas_id) != GERAN_ACCESS_STRATUM_ID_1 ? GERAN_ACCESS_STRATUM_ID_1 : GERAN_ACCESS_STRATUM_ID_2)

#define OTHER_G_SUB_IN_TRANSFER(gas_id) \
(gl1_get_gas_id_of_other_g_sub_in_transfer( gas_id ))

/* GBTA Assocaited Functions */

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM

#define SET_PCHXFER_DR_STATUS(dr_status)  \
(gl1_grm_set_pchxfer_dr_status(dr_status))

#define GET_PCHXFER_DR_STATE() \
  (gl1_grm_get_pchxfer_dr_status())
#endif
#endif

void grm_notify_event(grm_gbta_idle_event_t event,boolean leaving_idle,gas_id_t gas_id );
boolean gl1_gbta_schedule_idle_pwr_mon_if_allowed(gas_id_t gas_id);
void    gl1_gbta_scheduling_idle_event(grm_gbta_idle_event_t event,gas_id_t gas_id);

gas_id_t gl1_gbta_get_transfer_sub(gas_id_t gas_id);
boolean gl1_is_gbta_reason(trm_reason_enum_t reason,gas_id_t gas_id);

void    gpl1_gbta_schedule_idle_pwr_mon(gas_id_t gas_id);
void    gpl1_set_gbta_status(gas_id_t gas_id);
boolean gl1_gbta_check_gbta_allowed(trm_reason_enum_t req_reason,uint32 frames_required,gas_id_t gas_id);
boolean gpl1_check_no_transfer_sub_conflict(uint32 frames_req,gas_id_t gas_id);

boolean gl1_gbta_schedule_idle_per_mon_if_allowed(gas_id_t gas_id);
boolean gpl1_gbta_check_tbf_susp_planned(gas_id_t gas_id );
void    gpl1_gbta_reset_active_state(gas_id_t gas_id);


#endif  /*FEATURE_GPRS_GBTA */

#ifdef L1_HOST_TEST
#error code not present
#endif/*L1_HOST_TEST*/

/*QTA reason  =  PCH + all channel Maintenance reasons (Ncell FCCH/SCH/BCCH, IRAT Searches) */
#define IS_QTA_REASON(rsn,sub_reason) (\
((rsn) == TRM_DEMOD_PAGE)  || \
((rsn) == TRM_CHANNEL_MAINTENANCE) || \
((rsn) == TRM_IRAT_MEASUREMENT) || \
(((rsn) == TRM_CHANNEL_MAINTENANCE_INV)  && (((sub_reason) == SYS_PROC_TYPE_SYSTEM_INFO_READING) || ((sub_reason) == SYS_PROC_TYPE_ACQUISITION))))


#define IS_X_PLUS_G()( \
gl1_msg_get_multi_sim_standby_mode() && (gl1_get_num_inactive_subs() > 0) )

/*GBTA reason  =  PCH, (N)BCCH, Page Rorg & Ncell SCH*/
#define IS_GBTA_REASON(rsn, sub_reason) (\
((rsn) == TRM_DEMOD_PAGE)  || \
((((rsn) == TRM_CHANNEL_MAINTENANCE) || ((rsn) == TRM_CHANNEL_MAINTENANCE_INV))  && ((sub_reason) == SYS_PROC_TYPE_SYSTEM_INFO_READING)) ||\
(((rsn) == TRM_CHANNEL_MAINTENANCE_INV) && ((sub_reason) == SYS_PROC_TYPE_PAGE))|| \
(((rsn) == TRM_CHANNEL_MAINTENANCE) && ((sub_reason) == SYS_PROC_TYPE_INTER_FCELL_MEASUREMENT)))

#if defined FEATURE_QBTA
#define IS_QBTA_REASON(rsn, sub_reason) (\
((rsn) == TRM_DEMOD_PAGE)      || \
((((rsn) == TRM_CHANNEL_MAINTENANCE) || ((rsn) == TRM_CHANNEL_MAINTENANCE_INV))  && ((sub_reason) == SYS_PROC_TYPE_SYSTEM_INFO_READING))|| \
(((rsn) == TRM_CHANNEL_MAINTENANCE) && ((sub_reason) == SYS_PROC_TYPE_INTER_FCELL_MEASUREMENT)))
#endif
#ifdef FEATURE_GL1_ENABLE_QTA_IN_PSCAN
#define IS_QTA_REASON_RANE(rsn, state, sub_state) (\
	((rsn) == TRM_ACQUISITION_INV) && ((state) == L1_MULTI_SIM_FIND_BCCH) && ((sub_state) == L1_MULTI_SIM_POWER_SCAN))
#endif

#ifdef FEATURE_G2X_TUNEAWAY

#define MULTI_SIM_FW_LOAD_FRAMES_GPAGE_GTA 0

boolean gpl1_is_client_g(grm_client_enum_t client_id);
#define IS_CLIENT_G(client) (gpl1_is_client_g(client))

boolean gpl1_g2x_ta_is_ts_change_missed(gas_id_t gas_id);
boolean gpl1_g2x_ta_is_seq_registered_not_G(gas_id_t gas_id);

#ifdef  FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
#ifdef FEATURE_DUAL_SIM
void gl1_trans_grm_reserve_at_self_ccch( uint32 required_frame_nbr, gas_id_t gas_id );
#endif /* FEATURE_DUAL_SIM */
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

#endif /* FEATURE_G2X_TUNEAWAY */


#define IS_WCDMA_IN_BG_TRAFFIC() ((trm_get_extension_flag(TRM_UMTS, TRM_BG_TRAFFIC, TRM_PRIORITY_INV_ENABLED)) && \
                                   gl1_msg_get_multi_sim_mode() )

#define WCDMA_QTA_SUPPORTED_MASK (TRM_PRIORITY_INV_ENABLED + TRM_QUICK_TA_SUPPORTED)

#define IS_WCDMA_VOTED_PAGE_SKIP() (trm_get_extension_flag(TRM_UMTS, TRM_BG_TRAFFIC, TRM_ALTERNATE_PAGE_ENABLED))

#define WCDMA_LTA_PAGE_SKIP_MASK (TRM_PRIORITY_INV_ENABLED + TRM_ALTERNATE_PAGE_ENABLED)


#define IS_WCDMA_QTA_SUPPORTED() ( ((trm_get_extension_mask(TRM_UMTS, TRM_BG_TRAFFIC) & WCDMA_QTA_SUPPORTED_MASK) == WCDMA_QTA_SUPPORTED_MASK) && \
                                   gl1_msg_get_multi_sim_mode() )

#if defined ( FEATURE_TRIPLE_SIM )
#define IS_MULTI_SIM_MODE( mode ) ( ( (mode) == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY ) \
                                    || ( (mode) == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY ) \
                                    || ( (mode) == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ) \
                                  )
#else
#define IS_MULTI_SIM_MODE( mode ) ( ( (mode) == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY ) \
                                    || ( (mode) == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ) \
                                  )
#endif /* FEATURE_TRIPLE_SIM */

boolean grm_check_idle_sc_activity_ok(l1_fm_priority_T pri,
                                      uint32 frames_required,
                                      uint32 reserved_fn,
                                      gas_id_t gas_id);

boolean grm_check_w_t_reconf_in_pg_block_ok(uint32 frames_required,
                                            uint32 freq_index,
                                            gas_id_t gas_id);

boolean grm_check_ptm_irat_sc_activity_ok(uint16 freq_offset,
                                          uint32 frames_required,
                                          uint32 cmngr_scenario,
                                          gas_id_t gas_id);


gl1_sc_activities_t map_fm_priority_to_sc_activity(l1_fm_priority_T pri);

/* GRM definitions start here */

/* extern void grm_complete_x2g_drdsds_ho_operation(sys_band_T band); */
extern void gpl1_multi_sim_setup_afc_tt( gas_id_t gas_id );

extern void grm_request_at_warmup( gas_id_t gas_id );

extern void grm_enh_grant_panic_reset_callback(
             grm_client_enum_t          grm_client_id,
             trm_grant_return_enum_type grant_status,
             trm_duration_t             duration
);

void grm_setup_access_retry(boolean           pending_gl1_event, 
                            ps_access_mode_t  access_for_ps, 
                            trm_reason_enum_t reason,
                            grm_divrx_request_type    req_type, 
                            geran_procedure_t geran_procedure, 
                            trm_duration_t    duration,
                            gas_id_t          gas_id);


uint32 l1_bcch_current_bcch_fn( gas_id_t gas_id );

extern boolean gl1_is_granted_geran_sub_doing_reselection(grm_client_enum_t active_client);
extern trm_resource_enum_t gl1_ms_get_trm_resource(gas_id_t gas_id, trm_reason_enum_t reason);

extern boolean gl1_is_any_sub_in_transfer( void );

extern boolean gl1_check_any_subs_l1_state( byte l1_state );
extern boolean gl1_check_any_other_subs_l1_state( gas_id_t gas_id, byte l1_state );

grm_client_enum_t  gl1_get_client_id_of_other_idle_sub(gas_id_t gas_id);
void gl1_store_grm_sub_reason(sys_proc_type_e_type sub_reason, gas_id_t gas_id);

extern boolean gl1_check_any_other_subs_active_state( gas_id_t gas_id );
boolean gl1_is_l1_state_ptm(gas_id_t gas_id);
boolean gl1_is_l1_state_dtm(gas_id_t gas_id);

boolean gl1_is_l1_state_msim_idle(gas_id_t gas_id);

#ifdef FEATURE_GPRS_GBTA
extern boolean gl1_check_any_other_subs_idle_evt_power_mon( gas_id_t gas_id,gas_id_t *idle_gas_id_ptr );
extern void gl1_set_idle_event_status( grm_gbta_idle_event_status_t ev_stat, gas_id_t gas_id );
extern boolean gl1_check_other_subs_idle_event_status(grm_gbta_idle_event_status_t stat, gas_id_t gas_id ,gas_id_t *idle_gas_id_ptr);
extern boolean gl1_check_other_sub_release_pending( void );
extern boolean gl1_check_other_sub_reconfig_progress( uint32 frames_required );
extern boolean gl1_check_other_sub_sc_op_progress( void );
extern boolean gl1_check_other_sub_id_search_progress( void );
extern boolean gl1_check_other_sub_irat_progress( void );
extern boolean gl1_check_other_sub_ncell_meas_progress( void );
extern boolean gl1_check_other_sub_in_tbf_susp( void );
extern boolean gl1_other_sub_check_tbf_susp_planned(void);
extern boolean gl1_other_sub_check_missed_page( gas_id_t gas_id );
extern boolean gl1_check_all_other_subs_l1_state( gas_id_t gas_id, byte l1_state  );
extern boolean gl1_check_gonly_mode_active( void );
#endif

extern boolean gl1_check_any_other_subs_fw_load( gas_id_t gas_id );

extern boolean gl1_check_any_subs_trm_status( gl1_trm_state_t trm_status );

extern gas_id_t gl1_get_gas_id_for_ptm_client( void );

gas_id_t gl1_get_peer_gas_id_for_ptm_client( void );

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
grm_client_enum_t gl1_get_current_trm_client_id( grm_client_enum_t client );
#endif

extern boolean gl1_check_any_other_client_has_lock( gas_id_t gas_id );
extern boolean l1_sc_wcdma_get_gsm_only_mode( gas_id_t gas_id );

extern gas_id_t gl1_get_gas_id_of_other_g_sub( gas_id_t gas_id  );
extern gas_id_t gl1_get_gas_id_of_other_g_sub_in_transfer( gas_id_t gas_id  );

extern uint8 grm_num_clashing_subs( void );
extern uint8 gl1_get_num_inactive_subs( void );
extern gas_id_t gl1_get_gas_id_of_other_idle_sub(gas_id_t gas_id);
extern boolean gl1_check_any_other_sub_in_bg_traffic( gas_id_t gas_id );
background_hplm_state_t gl1_ms_background_hplmn_state( gas_id_t gas_id );


void gl1_multi_sim_reset_trm_request_state( gas_id_t gas_id );
extern boolean gl1_check_possible_isr_overrun(gas_id_t gas_id);
extern boolean gl1_check_qta_active_in_warmup(gas_id_t gas_id);
boolean gl1_check_if_getting_deactivated(gas_id_t gas_id);

#if defined(FEATURE_SGLTE_DUAL_SIM) || defined(FEATURE_SVLTE_DUAL_SIM)
void l1_handle_device_mode_cause_plmn(const gas_id_t gas_id);
#endif

#ifdef FEATURE_DUAL_DATA
void gl1_set_data_sub_priority(geran_priority_t priority,gas_id_t gas_id);
geran_priority_t gl1_get_data_sub_priority(gas_id_t gas_id);
gas_id_t gl1_get_low_priority_data_sub(void);
#endif /* FEATURE_DUAL_DATA */

boolean gl1_check_svlte_mode_active(void);

void l1_isr_multi_sim_inactive( gas_id_t gas_id );

sys_band_T L1_multi_sim_bcch_get_reservation_band(gas_id_t gas_id);

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
boolean gpl1_grm_rx_sec_supported_reason(l1_fm_priority_T pri);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
#endif /*FEATURE_DUAL_SIM*/
#endif /* GPL1_DUAL_SIM_H */
