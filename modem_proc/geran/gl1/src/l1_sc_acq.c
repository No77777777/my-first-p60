/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           SCE: ACQUISITION

GENERAL DESCRIPTION
   This module contains functions for SCE FCCH and SCH detection.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_acq.c#2 $
$DateTime: 2020/02/10 05:49:45 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
18/01/16   zf        CR961805: Set first burst flag in x2g_startup cmd
28/02/18   rc        CR2190693 Recalculating offset to ncell acq start frame number when in collision with other sub page and SCH timing is not known
03/04/17   ap        CR2023080: GERAN Ncells not getting detected by Idle SUB during Data on other SUB
31/03/17   mk        CR2025712: SENS algorithm need to be extended for SDCCH mode also
12/08/16   og        CR1050088. Sanity check the PTM FCCH tone results.
03/25/16   zf        CR995061: Check if activity reserved before starting ncell activity
14/02/17   mk        CR2004610 : FR 40251 (SeNS) - Skip reporting NCELL if SCH reconfirmation failed with good GDET score
03/02/16   zf        CR971154: Don't block FCCH booking in QTA.
13/10/16   mk        CR1071720: Optimization for 3rd SCH reconf to consider it as failed if RSSI is more than -90dBm and SNR is less than 6 dB
13/05/16   sk        CR1013011 Clear DRX_REF variable of sb/fcb/bcch if the activity's start_FN is skipped/missed.
09/05/16   sm        CR1002861 Added QSH GL1 handler : PCH collision with Ncell activity and continuous Ncell activity failure  .  
01/03/16   br        CR981662 GL1 to give sufficent time  for X-tech to open QTA for NcellSCH( activity6) activity
12/01/16   zf        CR961419: Rework 948783
18/11/15   cjl       CR939889 Fix GSM stuck in QTA gap booking blocked ncell activity
24/04/15   am        CR825716 Delay SCH scheduling after 4 TRM denials in idle mode
06/10/15   nk        CR918217 Removing unnecessary checks before NCELL SCH Rx
05/10/15   sp        CR898457: Ignore next tick req for SCH/FCCH state configurations in next_cell_bcch
30/09/15   cjl       CR914688 Abort blocked acquisition attempts when leaving idle
16/09/15   hd        CR905810 Check if drx_ref is already set before registering for NCELL activity
07/09/15   br        CR899284 GL1 to schedule ACQ immediately after 14 frames in case of TRM denial, if other sub is in data transfer.
19/08/15   mc        CR874662 CXM Freq ID support for X2G startup and X2G preload
04/08/15   hd        CR883709 Pass the gas_id properly to the grm_get_reason() while checking collision with other sub
15/07/15   sjv       CR869294 Book in DRX manager for Msim Idle NCell activity only while reserving TRM 
29/07/15   cws       CR879244 Do only one ncell sch attempt in QTA gap
28/07/15   js        CR841536 Changes for NCELL SCH as QBTA activity.
07/07/15   am        CR864051 Ncell SCH in PTM sometimes fails in desense 
27/05/15   am        CR843429 Take missed frames by sleep recovery into account for ACQ
30/04/15   am        CR830972 Use individual subs for storing afc
20/07/15   nk        CR874714 Do not reserve TRM just one frame (NCELL FCH) before when other SUB is in Data call
22/05/15   ws        CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
30/04/15   ip        CR799995 FR26290 Sleep when X tech is in connected state
16/04/15   smd       CR822202 Dual space data for IRAT and QTA
16/03/15   zf        CR809443: Don't clear RX state when retry SCH in QTA.
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
17/03/15   xz        CR801386 sovle the address word aligment issue for scheduled_status of SCH
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
19/02/15   fm        CR791018 GL1 checks SIM mode for NCell SCH Rx before starting it
18/02/15   js        CR792938 Avoid schedulling of Ncell ACQ ahead of PCH to avoid unnecessary GTA gap.
12/02/15   am        CR771762 BCCH Decode Failures at Good RSSI when the NCell is drifted by 1.5KHz
13/01/15   sp/js     CR782085 Rebook FCCH/SCH activity if QTA init fails
12/01/15   sjv       CR756154 Enhance Band Registration for Activities
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   ws        CR768376 Q6 compile warning fixes
04/12/14   sjv       CR766784 Backing out CR759568
25/11/14   zf/zc/sj  CR759568 start_frame_offset is updated, for calculating FCB start frame if QTA supported
26/09/14   zf        CR732436: Delay starting ACQ when RF wakes up late
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build
10/10/13   cja       CR555489 GL1: rejecting the tone when the offset > 0 in the idle frame
13/08/13   dg        CR485683: In dedicated & packet transfer mode,Reject tone if its offset
                     is less than starting of FCCH window
11/09/14   am        CR677361 Addressing KW GL1 P1 errors
04/09/14   zf        CR705256: Remove one wait frame for waiting to start FCB acquisition
14/08/14   js        CR614857:Decoupling PCH and ACQ
29/08/14   cah      CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
03/09/14   sp        CR715190 Function call to reset GBTA state from Active to Allowed/None
21/08/14   sjv       CR712391  Start Frame for FCB when GSM is in QTA from W, is postponed further as W wont release lock within 28msec after a QTA gap
06/08/14   df        CR536059  Allow Asynchronous sleep for all idle activities
31/07/14   pa        CR702665: TRM band registration release during NCELL Tone detection in transfer mode.
30/07/14   sp        CR676031: In TCH mode, use Delayed abort for aborting NCELL SCH.
28/07/14   ssh       CR700687: Handle X2G start up from warm up ISR
28/07/14   cah       CR701236  BOLT 2.0 DR-DSDS: IFS band not released after SCH decode.
22/07/14   cah       CR697586  trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
10/07/14   js        CR593074- ROTA/QBTA Feature implementation
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
30/05/14   ws        CR657674 Data + MMS Phase 1
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS: Scell measurement changes
08/05/14   nk        CR660831: GSM Layer 1 compares wrong enum values for internal TRM_DENIAL status. Change TRM_DENIAL enum to TRM_NOT_GRANTED to aoid any confusion going forward.
08/04/14   cs        CR638933 Allow equal priority event to be booked when suspend is set
17/03/14   sp        CR633653:Reschedule NBCCH and NSCH activities if the resrved occassion is missed due to a deferred wakeup from W
19/03/14   ws        CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
03/03/14   am        CR630494 Don't release FW in the same frame as FW setup
07/03/14   ssh       CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
24/02/14   js        CR621831:CM start up when BCCH followed by NCELL SCH
28/02/14   ap        CR624060 Further changes to FEATURE_PBR_MARKOV applied
07/02/14   dv        CR612952 delay TRM release by 1 frame for ncell ACQ abort
04/02/14   am        CR604532 Don't release FW in the same frame as FW setup
15/01/13   sp        CR600563: Correct the Regression of 592577 which resulted in no ncell acq in Single Sim mode.
19/12/13   jk        CR503359 - GL1 changes to support Rx diversity
03/12/13   mko       Adding CR537478 changes which are removed in power opt
03/12/13   js        CR574803 TRM extended for Ncell Acquisitions after PCH decode leading to missed page on next SUB
28/11/13   js        CR580273 Activity 5 getting blocked as extend duration was denied
19/11/13   ssh/ip    CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   nm        CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
07/11/13   zf        CR537690: Reduce G wakeup timeline for MS mode
11/27/13   sk        CR571001 Ensure Acq always gets booked along with PCH when W is QTA suppored mode
21/10/13   sk        CR563878 Ensure Acq gets to run inside the GTA gap with GBTA framework
18/10/13   sk        CR563250 Ensure NBCCH booked with QTA over head and issue CM startup only on the startup frame
17/10/13   js        CR534835: TRM lock due to QTA can't be handled:: wrong FN Calculation
11/10/13   sk        CR557161 Remove the duplicate trm release at the end of the GBTA gap
05/10/13   sk        CR554665 Backout the power opt temperorily(CR537690)
27/09/13   ab        CR550563 TR 1.1 - GBTA - Make Pseudo TRM_GRANT NULL after activity over.
24/09/13   gk        CR549082:Sync from 1.1 and 1.0 to 2.0
09/09/13   ab        CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
10/09/13   sk        CR539332 Ensure to clear the FM in the case of FCB abort when the state machine is not configured for Rx yet
29/08/13   gk        CR496693:The drx reference should be removed when the reservation FN has been reached
15/08/13   sk        CR524039 GBTA bringup changes
25/06/13   cs        Major Triton TSTS Syncup
22/05/13   ap        CR485912: Revised GFW state transitions
07/06/13   cs        TSTS Updates
08/02/13   zf        CR439530: Book ncell acq to after paging when W sub is acquiring.
20/05/13   npt/zf    CR410922: Offset missed frames for acq FCB due to sleep error recovery
17/05/13   kb/npt    CR385548 - Decode PCH after sleep wakeup error recovery.
21/04/13   am        CR478082 Don't abort current sch/fch until ongoing FW loading is finished
07/05/13   am        CR484187 Clear frame while abondoning SCH
19/04/13   mc        CR477448 DSDS: QTA Support : Initial code changes & Triton Bring-Up - fix build errors
13/04/13   mc        CR471759 DSDS: QTA Support : Initial code changes & Triton Bring-Up
04/04/13   ss        CR470525 Check TRM only if Dual Sim while aborting NCELL ACQ
25/02/13   ap        CR 455601 DSDS Don't issue NCELL ACQ  commands to the msg layer when QTA init fail
11/02/13   ap        CR448602 DSDS GTA: UE not able to finish FTP and stuck in bad state
11/02/13   ap        CR451539 additional fix to CR#440375
11/02/13   og        CR451271.Don't issue SCH read commands to the msg layer when QTA init fails.
07/02/13   ap        CR449859 Always enter GFW state MEAS_ACTIVE for gonly sub else
                             enter GFW ACTIVE state
18/01/13   ap        CR445826 UE not able to do any NCELL activity and stuck in doing FCCH for only one cell
18/01/13   mc        CR437838
17/01/12   zf        CR440375: Adding back FB_DUAL_SIM states which removed during merge
12/12/12   mc        CR431452 NCELL SCH and BCCH start FN corrections for QTA
26/11/12   zf        CR417719: Optimization of Ncell acquisition
03/09/12   zf        CR341843: Opt16: Re-use SCH timing for re-acquiring Ncell.
16/07/12   jj        CR374445 UE is not including report quantity for arfcn 20 in enhanced measurement report
08/05/12   jj        CR353379 fix for scheduling clash between NCell SCH and Rx
24/05/12   jj        CR 357359 MST WP8 Apollo: BCCH Decode failures during cell reselection
14/06/11   cja       CR264690 Acquisition frequency capture range larger
26/04/11   ap        CR283781 DSDS GL1 is stuck in idle abort
11/02/11   cs        CR274909 DSDS ncell afc not applying correct frequency error
14/01/11   nt        DSDS CR269953 - When aborting ncell SCH allow mDSP more time before disable
27/04/10   cs        CR236460 Latest AFC\XO handling updates
10-03-16   tjw       Renamed UNUSED to __UNUSED_ARG to avoid name clash.
08/02/10   cja       XO Update
25/01/10   cja       CR224507. For XO correct freq error setting for NCell.
09-10-22   tjw       avoid compiler and lint warnings about unused variables when MSG
                     is defined away to nothing
17/04/09   nt        CR179739 - DTM/CS uses different acq window sizes
26/05/09   cs        CR167848 fix for expanded afc_freq I/F to mDSP
09/04/09   og        Seperate the dedicated and transfer SCH scheduling decisions
                     CR174438.
18/08/08   nt        CR153766: Added initialisation of freq_error in l1_scell_afc_info
20/02/08   og        CR139102-Ensure the TCH->TCH non-blind handover uses the frequency
                     error for the target cell before camping on it.
03/10/07   og        Upgrade the FEATURE_GSM_L1_HFREQ_ERR_NCELL_SCH_DECODE to
                     include packet transfer NCELL SCH attempts.
26/06/07   nt        Corrected previous integration error
24/05/07   og        Prevent afc checks during SDCCH8/SDCCH4 multiframes when the
                     afc is off. Prevents vctcxo corruption. Resolves CR119645.
12/06/07   og        Upgrade the FEATURE_GSM_L1_HFREQ_ERR_NCELL_SCH_DECODE to
                     include dedicated TCH NCELL SCH attempts.
29/03/07   nt        CR:113459 Changed the number of frames required for FCCH/SCH
                     acuisition on SDCCH, so the frame manager will allow the task.
06/12/06   og        Fix pch block decode failures due to interruption by BPLMN
                     search. Fixes CR107369.
09/11/06   og        Fix to CR92174, handle IDLE NCELL SCH / BCCH decode at high frequency
                     error conditions.
14/08/06   ws        Allow dedicated to be timeslot slammed to lowest received tn
                     and also to place the CS timeslot on TN 2
16/03/06   og        Shrink the allowed FB/SB acquisition window size by 2 timeslots
                     to the TDMA frame tick. Resolves CR 88025.
02/01/06   ip        CR85687 & CR85686 FCH and SCH corrections to receiver window
                     size in DTM mode
26/01/06   og        Neighbour cell acquisition changes to handle extended dynamic.
23/01/06   og        Removal of QLint high errors.
23/11/05   og        Offset the last tn by 3 to temporarly accomodate changes in
                     gl1_msg_rx_ncell_fcch( ).
17/11/05   nt        Added setting of rx_tn and tx_tn to appropriate value for
                     use by the function call gl1_msg_rx_ncell_fcch().
01/09/05   nt        Uses the 'real' tx_tn and rx_tn values for the call to
                     gl1_msg_rx_ncell_fcch, this prevents FCCH scheduling conflicts
08/12/05   ws        Added initial FEATURE_GSM_DTM support
06/16/05   sv        Fixed lint error due to header file include.
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
03/07/05   sv        Tick the DRX Manager while issuing an ncell abort.
02/11/05   sv        Fixed Idle Abort problem due to issuing multiple aborts.
02/05/05   bm        BG PLMN scan optimizations
01/26/05   sv        Add support for Frame Manager.
11/01/04   yhong     Changed NCell Abort API to take a parameter
10/20/04   sv        Optimization of Ncell SCE awake time
08/26/04   sv        Lint cleanup
07/21/04   bk        Merged in changes for RRBP repartioning
07/15/04   gfr       Log ncell bcch/sch burst and message metrics.
06/22/04   dp        Lint cleanup
06/21/04   gsc       Pass in an extra argument to l1_sci_set_required_frames to
                     indicate the type of NCELL activity.
06/17/04   dp        Handled ncell corner cases during background PLMN search
05/26/04   sv        Lint Cleanup.
05/14/04   gw        Yet another refinement of the aborting mechanism.
05/20/04   gfr       Mainline FEATURE_COMMON_PWR_HANDLER
05/12/04   gw        Make sure that Idle Mode BSIC decodes are scheduled far enough
                     in the future to allow a sleep cycle before they occur.
04/05/04   dp        Fixed bug in background HPLMN aborting
03/31/04   dp        Fixed compiler warnings
03/16/04   gw        Modified receive module design to be compatible with
                     background PLMN search.
02/25/04   gw        Lint clean up.
02/19/04   gfr       Compile fix for no background HPLMN searching
02/09/04   dp        Added support for background HPLMN searching
11/05/03   gfr       Support for quad-band.
08/28/03   gw        Modified result reported for SB receives to include blocked
                     as a possibility.
08/24/03   dp        Put in check to see if FCCH offset is within the TCH
                     acquisition window
30/07/03   kf        replaced l1_sc_abort_receive_callback_gprs with l1_sc_abort_receive_callback.
07/14/03   gw        Removed ASSERTs.
07/11/03   gfr       Lint cleanup
07/09/03   bk        Need to pass the Base station reference for Tx and not OTA ref
07/08/03   bk        When the tone is detected check if the SCH can be decoded at the
                     end of the acq window, if not, move it to the beginning of the acq window
06/23/03   gfr       Compiler warning cleanup.
06/12/03   gw        Rationalization of SCE constants.
06/11/03   gw        Changed #define SCH_FRAMES to make sense w.r.t FCCH_FRAMES.
05/20/03   gfr       Common power handler support.
05/02/03   gw        Merged in power transfer changes from branch.
04/09/03   ws        Modified receive abort callback for gprs
04/04/03   gw        Removed obsolete debug code.
04/03/03   pjr       Modified calling of l1_sc_start_receive.
03/10/03   dp        Cetecom hack to decrease SCH decode offset by 90 to
                     increase tune time
01/16/03   ws        Modified gpl1_gprs_sci_start_sb_trans() as per KF request
01/10/03   gw        Changed gpl1_gprs_sc_receive_fcb_trans() to use lowest tn
                     instead of highest tn per request from ws.
01/02/03   gw        Added function l1_sci_is_fb_in_progress().
12/03/02   mk        General GPRS featurization cleanup.
12/02/02   ws        Updated SB burst scheduling for GPRS
11/25/02   mk        Updated GPRS baseline.
11/15/02   mk        Updated GPRS baseline with release 2.0 functionality.
10/17/02   JC        Acquisition AGC support; Disabled for NCELL for now.
10/16/02   dp        Fix for Ncell SCH scheduling bug which was putting us
                     into a long sleep
10/08/02   kf        Added PL1 GPRS measurement code hooks under switch
                     FEATURE_GPRS_MEAS.
10/01/02   ATM       Added central get and set FN routines
09/24/02   dp        Workaround for mdsp SBI errors when Ncell SCH offset lies
                     near FTSM.  Problem is due to large ZIF tune time.  Will
                     be pulled out when ZIF tune time is fixed
09/17/02   ATM       Moved logging for Combined ACQ to gl1_msg_acq.c.
08/13/02   dp        Interface change in gl1_msg_rx_fcch_sch for parallel decodes
08/23/02   plm       Removed the declaration of local variable frame_delay in the
                     function l1_sci_fcb_tch_cnf() that was declared but not used
                     to eliminate compiler warning.
08/21/02   gw        Moved call to gl1_msg_rx_fcch_sch() from task to ISR to
                     fix bug when aborter aborts.
07/31/02   ATM       Added logging for NCELL ACQ.
07/11/02   gw        Changed reference to power_dBm_x16 in acq report to
                     pwr_dBm_x16.
06/27/02   dp        gl1_msg_rx_ncell_fcch interface change for GPRS
05/30/02   dp        Truncated Ncell tone detect debug msgs
05/21/02   gw        Removed MSG_MED and gl1_dBm_to_l1pwr from l1_sc_read_sb.
03/22/02   JC        Added AGC support into NCELL acquisition and replaced
                     GSM_DEBUG macros w/ MSG_MED macros.
02/27/02   gw        Fixed bug in SCH abort.
02/20/02   dp        Cleaned up some TCH related SC functions
02/14/02   gw        Changes to match new message layer interface for AGC.
02/07/02   gw        Moved functions for FCB and SB reception from l1_sc_rcv.c.
                     Improved mechanism for aborting receives.
12/21/01   gw        Changes to support DRX.  Modified mechanism for scheduling
                     neighbor cell SCH reception.
12/18/01   dp        Changes to support SC acquisition using the new acq API.
                     Also cleaned up some of the fcb and sb functions.
12/10/01   thh       Hw_*() functions are obsolete.  Use the new interface
                     gl1_hw*() functions.
10/31/01   dp        Incorporated changes due to new SCH decode_rpt format
10/24/01   dp        Modified ncell_sch_data_callback for SC operation on TCH.
                     Added ncell_fcch_data_callback for as part of msg/frame
                     layer implementation of SC tone detection on the TCH
10/16/01   gw        Mainlined feature FINDBCCH_DEBUG.  Cleaned up SB code.
                     Changed remaining CCWW types to QCT types.
10/10/01   gw        Modified SCH reception to use message layer functions.
                     Mainlined feature MON_TMP.
09/25/01   mk/gw     General SCE cleanup. Mainlined MONITOR_DEBUG, added
                     PScan/Ncell support using FINDBCCH_DEBUG.
08/31/01   JC        Added COMMENT block around Pu_ICT variables.
08/20/01   mk/gw     Added Ncell support, featurized via MONITOR_DEBUG.
08/20/01   mk        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "l1_drx.h"
#include "gl1_msg_g.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "l1_isr.h"
#include "l1_sc.h"
#include "msg.h"
#include "l1_log.h"
#include "l1_utils.h"   /* for ADD_FN() macro */
#include "l1_task.h"
#include "l1_fm.h"
#include "l1_os.h"

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"

#include "gpl1_dual_sim.h"

#include "gpl1_grm_intf.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gl1_hw.h"
#endif
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define MAX_FB_ABORT_COUNT 10
#define DELAY_SCH_SCHEDULING 433
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  /* Maximum offset at which to start the state machine, this guarantees the timeline
   is as expected, otherwise an offset too large may cause results to not be available
   for an extra frame*/
#define MAX_NCELL_SCH_OFFSET  (6250 + (GL1_HW_SCH_SEARCH_WIDTH_DEFAULT * 4 / 2))

#define NCELL_SCH_TCH_RSSI_THRESH -80*16
#define NCELL_SCH_TCH_GDET_THRESH 665               // 2.6 in Q8 format

uint32 paging_collision_fn[NUM_GERAN_DATA_SPACES];
uint8 collision_sub[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GPRS_GBTA
#define L1_ACQ_GTA_SUSP_GAP 16
#endif /* FEATURE_GPRS_GBTA */

#define L1_NCELL_ACQ_GAP_TO_GTA     11
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

/*
 * Static Variable Declarations
 * ----------------------------
 */

typedef struct
{
   enum
   {
      FB_NULL,
      FB_INIT,
      FB_WAIT,
      FB_ACTIVE,
      FB_RAN_OUT_OF_FRAMES,
      FB_ABORTING
   } state;

   ARFCN_T              arfcn;
   sys_algo_agc_T      *agc;

   boolean              success;

   uint32               start_fn;
   int16                start_qbit;

   boolean              sch_decoded;
   uint8                sch_data[4];

   boolean              do_AFC;
   boolean              known_sch_timing;
   uint8                fcb_detection_duration;
   uint8                fcch_frames;
   uint8                sch_frames;
   uint8                sch_attempts;

   gl1_msg_rx_rpt       rx_rpt;
   gl1_msg_rx_cb_type   rx_cb;
   gl1_msg_decode_rpt   decode_rpt;

   uint8                drx_ref;

   l1_ncell_afc_info_t *afc_info_ptr;

} fcb_struct;


typedef struct
{
   enum
   {
      SB_NULL,
      SB_INIT,
      SB_WAIT,
      SB_ACTIVE,
      SB_RAN_OUT_OF_FRAMES,
      SB_ABORTING
   } state;

   ARFCN_T              arfcn;
   uint32               fn_lag;
   sys_algo_agc_T      *agc;
#ifdef FEATURE_GSM_RX_DIVERSITY
   sys_algo_agc_T      *agc_divrx;
#endif /*FEATURE_GSM_RX_DIVERSITY*/
   dword                start_fn;
   int16                start_qbit;
   byte                 mask;
   byte                 tries;
   byte                 trm_denials;

   l1_sc_rcv_result_T   result;

   boolean              data_done;      /* Need to make sure both callbacks have */
   boolean              metrics_done;   /* occured before we go to StateDone     */

   gl1_msg_rx_rpt       rx_rpt;
   gl1_msg_decode_rpt   decode_rpt;
   gl1_msg_metrics_rpt  metrics_rpt;
   gl1_msg_rx_cb_type   rx_cb;

   uint8                drx_ref;

   l1_ncell_afc_info_t *afc_info_ptr;
} sb_struct;

static const uint8 sb_map[] =
{
   0,0x01,0,0,0,0,0,0,0,0,
   0,0x02,0,0,0,0,0,0,0,0,
   0,0x04,0,0,0,0,0,0,0,0,
   0,0x08,0,0,0,0,0,0,0,0,
   0,0x10,0,0,0,0,0,0,0,0,0
};

#undef  DED_SC_TONE_DETECT_DEBUG
#undef  DED_SC_SCH_DECODE_DEBUG

#ifdef DED_SC_TONE_DETECT_DEBUG
#error code not present
#endif

#ifdef DED_SC_SCH_DECODE_DEBUG
#error code not present
#endif

typedef struct
{
  int        last_sch_offset_delta ;
  fcb_struct fcb;
  sb_struct  sb;
  boolean    ncell_acq_retry ;
  boolean    ncell_acq_pending ;
  boolean    ncell_acq_in_progress ;
  boolean    ncell_sch_in_progress ;
}l1_sc_acq_data_t;

static l1_sc_acq_data_t  l1_sc_acq_data[NUM_GERAN_DATA_SPACES];
l1_ncell_afc_info_t      l1_scell_afc_info[NUM_GERAN_DATA_SPACES];

extern dedicated_data_T l1_dedicated_data[];

/*
 * Prototypes for local Functions
 * ------------------------------
 */


static void fcb_aborted( void* dummy, gas_id_t gas_id );
static void ncell_fcch_sch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void ncell_fcch_sch_abort_callback(gas_id_t gas_id);
static void ncell_fcch_tch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void ncell_fcch_tch_abort_callback( gas_id_t gas_id );
static void l1_sc_calc_fcb_start_fn( gas_id_t gas_id );
static void l1_sc_init_idle_fcb_acq( gas_id_t gas_id );
static void l1_sc_start_idle_fcb_acq( gas_id_t gas_id );
static uint16 fcb_detection_duration( gas_id_t gas_id );

static void sb_aborted( void* dummy, gas_id_t gas_id );
static void ncell_sch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void ncell_sch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );
static void ncell_sch_abort_callback(gas_id_t gas_id);

static void ncell_sch_tch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void ncell_sch_tch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );
static void ncell_sch_tch_abort_callback( gas_id_t gas_id );

static uint32 next_sb_fn( uint32 serv_fn, gas_id_t gas_id );

static void ncell_sch_trans_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );
static void gpl1_gprs_sci_fcb_trans_cnf( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void gpl1_gprs_ncell_sch_trans_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );

#ifdef FEATURE_GSM_SENS_SUPPORT
void l1_sc_run_SENS(l1_sc_acq_data_t *l1_sc_acq_data_ptr, gas_id_t gas_id);
#endif

/*helper functions for DR-DSDS*/
ARFCN_T get_next_sch_arfcn(gas_id_t gas_id)
{
  return l1_sc_acq_data[gas_id].sb.arfcn;
}
ARFCN_T get_next_fcb_arfcn(gas_id_t gas_id)
{
  return l1_sc_acq_data[gas_id].fcb.arfcn;
}

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )

/*===========================================================================

FUNCTION gl1_page_collision_with_x_sub

DESCRIPTION
       Determines paging collision with other sub.

PARAMS Client_id,, priority , gas_id

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
boolean gl1_page_collision_with_x_sub( grm_client_enum_t client,
                                       l1_fm_priority_T priority,
                                       gas_id_t gas_id )
{
  gas_id_t other_gas_id;

  l1_sc_acq_data_t *l1_sc_acq_data_ptr;
  volatile ISRTIM_CMD_BLK *tskisr_ptr = NULL;
  trm_reason_enum_t trm_reason;
  uint8  duration = 0;
  uint32 diff_sclks = 0;
  uint32 num_of_frames = 0;
  uint32 current_fn = 0;

  other_gas_id = gl1_map_client_id_to_gas_id(client);

  if (other_gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED)
  {
     return FALSE;
  }

  tskisr_ptr = &l1_tsk_buffer[other_gas_id];

  l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

  if (priority == FM_PRI_CCCH)
  {
    duration = L1_DURATION_CCCH;
  }
  else if (priority == FM_PRI_NCELL_ACQ)
  {
    duration = l1_sc_acq_data_ptr->fcb.fcb_detection_duration;
  }

  trm_reason = grm_get_reason( client, other_gas_id);

  if ((TRM_NUM_REASONS != trm_reason) && (GRM_PAGING_REASON(trm_reason)))
  {
    grm_info_t *grm_info_p = grm_get_info_ptr_from_client( client, gas_id );

    if ( tskisr_ptr && grm_info_p )
    {
       diff_sclks = ( grm_info_p->when - timetick_get_safe() );

      if (diff_sclks > 0)
      {
          num_of_frames =  MSECS_TO_FRAMES(timetick_cvt_from_sclk(diff_sclks, T_MSEC));
          current_fn = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), num_of_frames);
          paging_collision_fn[other_gas_id] = current_fn;
          collision_sub[gas_id] = other_gas_id;
          if (SUB_FN(current_fn, GSTMR_GET_FN_GERAN(gas_id)) <= duration)
          {
            MSG_GERAN_HIGH_3_G("collision_sub[gas_id]=%d paging_collision_fn=%d current_fn=%d ", collision_sub[gas_id], paging_collision_fn[collision_sub[gas_id]], current_fn);
            return TRUE;
          }
      }
    }
    else
    {
      MSG_GERAN_MED_1_G( "Invalid Client ID %d!!", client );
    }
  }

  return FALSE;
}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*
 *     Function name :  l1_sc_fcb_init
 *     -------------------------------
 *
 *     Description :
 *     ---------------
 *     Init static data in this file.
 *
 */

void l1_sc_fcb_init( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   l1_sc_acq_data_ptr->fcb.state = FB_NULL;
}

/*
 *     Function name :  l1_sc_fcb_shutdown
 *     -----------------------------------
 *
 *     Description :
 *     ---------------
 *     Clean up static data in this file.
 *
 */
void l1_sc_fcb_shutdown( void )
{
}

/* This function is designed to be called from the task level
 * with interrupts locked.
 */
void l1_sc_start_fcb( cell_T* cell, gas_id_t gas_id )
{
    l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
    if ( l1_sc_acq_data_ptr->fcb.state != FB_NULL )
    {
       MSG_GERAN_ERROR_0_G("ncell FCCH already in progress");
    }

    /* Store the data we'll need to start the receive. */
    l1_sc_acq_data_ptr->fcb.arfcn            = cell->bs.arfcn;
    l1_sc_acq_data_ptr->fcb.agc              = &cell->bs.agc;

#ifdef FEATURE_GSM_POWER_OPTION16
    l1_sc_acq_data_ptr->fcb.known_sch_timing = cell->bs.bsic_known;
#else /* FEATURE_GSM_POWER_OPTION16 */
    l1_sc_acq_data_ptr->fcb.known_sch_timing = FALSE;
#endif /* FEATURE_GSM_POWER_OPTION16 */
    if ( l1_sc_acq_data_ptr->fcb.known_sch_timing )
    {
        l1_sc_acq_data_ptr->sb.fn_lag        = cell->bs.fn_lag;
        l1_sc_acq_data_ptr->sb.start_qbit    = cell->bs.qbit_lag;
        l1_sc_acq_data_ptr->sb.mask          = 0x1F;
    }
    else
    {
        l1_sc_acq_data_ptr->sb.fn_lag        = 0;
        l1_sc_acq_data_ptr->sb.start_qbit    = 0;
        l1_sc_acq_data_ptr->sb.mask          = 0x1F;
    }

    /* Move state machine to first state. */
    l1_sc_acq_data_ptr->fcb.state            = FB_INIT;

    l1_sc_acq_data_ptr->fcb.afc_info_ptr        = &( cell->afc_info );
    l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid = FALSE;


    l1_sc_acq_data_ptr->ncell_acq_retry = FALSE;
    l1_sc_acq_data_ptr->ncell_acq_pending = FALSE;
    l1_sc_acq_data_ptr->ncell_sch_in_progress = FALSE;

}

/*
 *     Function name :  l1_sci_abort_fcb
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Abort an FCB search. Call with interrupts locked.
 *
 */
void l1_sci_abort_fcb( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   switch (l1_sc_acq_data_ptr->fcb.state)
   {
      case FB_WAIT:
         l1_fm_clear_required_frames(FM_PRI_NCELL_ACQ, gas_id);
         /*intentional fall through */

      case FB_INIT:
         l1_sc_acq_data_ptr->fcb.state = FB_NULL;

         /* msg layer is inactive so we can abort immediately */
         l1_sc_abort_receive_callback(gas_id);
         break;

      case FB_ACTIVE:
         l1_sc_acq_data_ptr->fcb.state = FB_ABORTING;

         /* need to abort the msg layer */
         gl1_msg_abort_rx_ncell_fcch_sch(DELAYED_ABORT, gas_id);
         break;

      case FB_RAN_OUT_OF_FRAMES:
         l1_sc_acq_data_ptr->fcb.state = FB_ABORTING;
         break;

      case FB_ABORTING:
         /* do nothing */
         MSG_GERAN_ERROR_0_G("Multiple aborts.");
         break;

      case FB_NULL:
      default:
         l1_sc_acq_data_ptr->fcb.state = FB_NULL;

         /* this should never happen */
         l1_sc_abort_receive_callback(gas_id);
         l1_fm_clear_required_frames(FM_PRI_NCELL_ACQ, gas_id);
         MSG_GERAN_ERROR_1_G("Unexpected state %d", l1_sc_acq_data_ptr->fcb.state);
         break;
   }

   l1_sc_acq_data_ptr->ncell_acq_retry = FALSE;
   l1_sc_acq_data_ptr->ncell_acq_pending = FALSE;
   l1_sc_acq_data_ptr->ncell_acq_in_progress = FALSE;
   gl1_drx_require_next_tick(gas_id);


}


static void ncell_fcch_sch_abort_callback(gas_id_t gas_id)
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];


   /* Work out why we aborted */
   if (( l1_sc_acq_data_ptr->fcb.state == FB_RAN_OUT_OF_FRAMES ) ||
       ( l1_sc_acq_data_ptr->fcb.state == FB_WAIT ))
   {
       /* We ran out of frames.  Set up to
        * try again.
        */
         l1_sc_acq_data_ptr->fcb.state = FB_INIT;
   }
   else if ( l1_sc_acq_data_ptr->fcb.state == FB_ABORTING )
   {
      /* All SC receives are being aborted */
      /* - let the SCE know we're done     */
      l1_sc_abort_receive_callback(gas_id);
      l1_sc_acq_data_ptr->fcb.state = FB_NULL;
   }
   else
   {
      MSG_GERAN_ERROR_1_G("Unexpected abort callback. %d", l1_sc_acq_data_ptr->fcb.state);
      l1_sc_abort_receive_callback(gas_id);
      l1_sc_acq_data_ptr->fcb.state = FB_NULL;
   }



   l1_sc_acq_data_ptr->ncell_acq_retry = FALSE;
   l1_sc_acq_data_ptr->ncell_acq_pending = FALSE;
   l1_sc_acq_data_ptr->ncell_acq_in_progress = FALSE;
   l1_fm_clear_required_frames(FM_PRI_NCELL_ACQ, gas_id);

}

/*
 *     Function name :  l1_sci_tick_fcb
 *     --------------------------------
 *
 *     Description :
 *     ---------------
 *     Tick the FCB search state machine.
 *     After calling l1_sci_start_fcb() this function must be called every frame
 *     until l1_sci_tick_fcb() returns TRUE or the search is aborted via
 *     l1_sci_abort_fcb().
 *     The first call to l1_sci_tick_fcb() must be done in the frame following the
 *     frame where l1_sci_start_fcb() has been called.
 *
 *     Returns :
 *     -----------
 *     FALSE when l1_sci_tick_fcb() should be called again.
 *     TRUE when done.
 *
 */
void l1_sci_tick_fcb( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   ASSERT_INT();

   switch ( l1_sc_acq_data_ptr->fcb.state )
   {
     case FB_INIT:
         l1_sc_init_idle_fcb_acq( gas_id );

         /* fall through to next case */

     case FB_WAIT:
     {
          uint32 current_FN;
          uint32 missed_current_FN;
          uint8  missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);

          current_FN = gl1_get_FN(gas_id);
          /* We may wakeup a few frames late due to sleep error recovery. */
          missed_current_FN = SUB_FN(current_FN, missed_frames);

         if ( l1_sc_acq_data_ptr->fcb.start_fn == missed_current_FN )
         {
#ifdef FEATURE_TRIPLE_SIM
          if(!gl1_detect_page_collision_with_other_sub(gas_id))
#endif   /* FEATURE_TRIPLE_SIM */
           {
             l1_sc_acq_data_ptr->fcb.drx_ref = 0;
             if ( l1_fm_is_activity_reserved(FM_PRI_NCELL_ACQ, gas_id) &&
                  l1_fm_are_frames_available(FM_PRI_NCELL_ACQ,
                  (l1_sc_acq_data_ptr->fcb.fcb_detection_duration - missed_frames), fcb_aborted, 0, gas_id) )
             {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                 /* Make sure that FW/RF are initialised and if this is "NOT" first QTA gap after wake up from sleep*/
                 if ( gl1_msg_get_multi_sim_mode() &&
                     ( l1_tskisr_blk->firmware_loaded == FALSE ) &&
                     ( gl1_check_qta_active_in_warmup(gas_id) == FALSE ))
                 {
                     if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_NOT_GRANTED )
                     {
                         l1_tskisr_blk->firmware_loaded = TRUE;
#ifdef FEATURE_QTA
                         if ( l1_tskisr_blk->qta_in_progress )
                         {
                             MSG_GERAN_HIGH_1_G("DSDSDBG FN = %d NCELL ACQ - QTA in progress",
                                     gl1_get_FN(gas_id));

                             if(!gl1_hw_qta_fw_rf_init(FALSE,GL1_FW_QBTA_CM_START_UP_OFFSET,FALSE,l1_sc_acq_data_ptr->fcb.arfcn,FALSE,TRUE,gas_id))
                             {
                                 l1_sc_acq_data_ptr->ncell_acq_retry = TRUE;
                                 l1_sc_init_idle_fcb_acq( gas_id );
                                 l1_tskisr_blk->firmware_loaded = FALSE;
                                 break;
                             }
                             else
                             {
                                 /* Prevent FM from releasing in the same frame */
                                 l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                                FM_PRI_NCELL_ACQ, gas_id );
                             }
                         }
                         else
#endif /* FEATURE_QTA */
                         {
                             /* Issue the call to load up the firmware */
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
                                                            FM_PRI_NCELL_ACQ, gas_id );
                         }
                     }
                 }

                if (gl1_check_possible_isr_overrun(gas_id))
                   {
                       /* re-attempt this FCB so book up another time. */
                   l1_sc_acq_data_ptr->ncell_acq_retry = TRUE;
                   l1_sc_init_idle_fcb_acq( gas_id );
                   MSG_GERAN_MED_1_G( GL1_ACQ_HDR"FCCH (ARFCN=%d) retry in state FB_WAIT, possible ISR overrun", l1_sc_acq_data_ptr->fcb.arfcn.num);
                }
                else
                {
                  if (  (!gl1_msg_get_multi_sim_mode()) || ( l1_tskisr_blk->firmware_loaded == TRUE))
                   {
#if defined (FEATURE_QBTA)
                     if((l1_tskisr_blk->qta_in_progress) && (!gl1_hw_get_qta_cm_fw_rf_init_status(gas_id)))
                     {
                           gl1_hw_qta_fw_rf_init(FALSE,GL1_FW_QBTA_CM_START_UP_OFFSET,TRUE,l1_sc_acq_data_ptr->fcb.arfcn,FALSE,TRUE,gas_id);
                     }
#endif
                       /* add missed number of frames */
                       l1_sc_update_fcb_start_fn( missed_frames, gas_id );

                       /*Start FCB Acq if Single Sim Mode or only if Firmware loaded in multisim*/
                       l1_sc_start_idle_fcb_acq( gas_id );

                   }
                   else if (l1_fm_leaving_idle_mode (gas_id) == TRUE)
                   {
                     /* abandon this attempt as we need to leave idle mode */
                     MSG_GERAN_HIGH_3_G(GL1_ACQ_HDR"FCCH (ARFCN=%d) abandoned - leaving idle mode",
                                       l1_sc_acq_data_ptr->fcb.arfcn.num, l1_sc_acq_data_ptr->fcb.start_fn, current_FN );
                     ncell_fcch_sch_abort_callback( gas_id );
                   }
                   else
                   {
                       /* re-attempt this FCB so book up another time. */
                       l1_sc_acq_data_ptr->ncell_acq_retry = TRUE;
                       l1_sc_init_idle_fcb_acq( gas_id );
                       MSG_GERAN_HIGH_3_G(GL1_ACQ_HDR"FCCH (ARFCN=%d) retry in state FB_WAIT. Waiting for start FN %d curr FN %d",
                                l1_sc_acq_data_ptr->fcb.arfcn.num, l1_sc_acq_data_ptr->fcb.start_fn, current_FN );
                   }
                }
#else
             /* add missed number of frames */
             l1_sc_update_fcb_start_fn( missed_frames, gas_id );

             /* Single sim case */
             l1_sc_start_idle_fcb_acq( gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
        }
        else
        {
          /* re-attempt this FCB so book up another time. */
          l1_sc_acq_data_ptr->ncell_acq_retry = TRUE;
          l1_sc_init_idle_fcb_acq( gas_id );
          MSG_GERAN_HIGH_3_G(GL1_ACQ_HDR"FCCH (ARFCN=%d) retry in state FB_WAIT. Waiting for start FN %d curr FN %d",
                             l1_sc_acq_data_ptr->fcb.arfcn.num, l1_sc_acq_data_ptr->fcb.start_fn, current_FN );
        }
         }
#ifdef FEATURE_TRIPLE_SIM
          else
         {
            /* re-attempt this FCB so book up another time. */
            l1_sc_acq_data_ptr->ncell_acq_retry = TRUE;
            l1_sc_init_idle_fcb_acq(gas_id);
            MSG_HIGH(GL1_ACQ_HDR"FCCH (ARFCN=%d) retry in state FB_WAIT. Waiting for start FN %d curr FN %d",
            l1_sc_acq_data_ptr->fcb.arfcn.num, l1_sc_acq_data_ptr->fcb.start_fn, current_FN );
         }
#endif /* FEATURE_TRIPLE_SIM */
       }
         else if ( !(IS_FRAME_NUM_LATER(l1_sc_acq_data_ptr->fcb.start_fn, missed_current_FN)) )
         {
             /* abandon this attempt - something has gone wrong with the scheduling the frame number has passed */
             MSG_GERAN_HIGH_3_G(GL1_ACQ_HDR"FCCH (ARFCN=%d) abandoned in state FB_WAIT. Frame has passed. start FN %d curr FN %d",
                      l1_sc_acq_data_ptr->fcb.arfcn.num, l1_sc_acq_data_ptr->fcb.start_fn, current_FN );
             /* Our tick is skipped so drx_ref is no longer valid. */
             l1_sc_acq_data_ptr->fcb.drx_ref = 0;             
             ncell_fcch_sch_abort_callback( gas_id );
         }
     }
     break;

      case FB_ACTIVE:
         /* We take care of this state in l1_sci_post_tick_fcb() */
         break;

      case FB_RAN_OUT_OF_FRAMES:
      case FB_ABORTING:
         gl1_drx_require_next_tick(gas_id);
         break;

      case FB_NULL:
         /* Should never happen, but makes lint happy */
         break;
   }

}

void l1_sci_post_tick_fcb( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_INT();

   if (l1_sc_acq_data_ptr->fcb.state == FB_ACTIVE)
   {
         gl1_drx_require_next_tick(gas_id);
   }
}

/* Call with interrrupts locked. */
void l1_sc_start_fcb_tch( cell_T* cell, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   l1_sc_acq_data_ptr->fcb.arfcn            = cell->bs.arfcn;
   l1_sc_acq_data_ptr->fcb.agc              = &cell->bs.agc;
   l1_sc_acq_data_ptr->fcb.state            = FB_INIT;

   l1_sc_acq_data_ptr->fcb.afc_info_ptr        = &( cell->afc_info );
   l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid = FALSE;
}

void l1_sci_tick_fcb_tch( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
   dedicated_data_T *l1_dedicated_data_ptr;
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;

   ASSERT_INT();
   l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
   gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[gas_id];

   if (l1_sc_acq_data_ptr->fcb.state == FB_INIT)
   {
      gl1_defs_tn_type  rx_tn;
      gl1_defs_tn_type  tx_tn;

      /* Set the transmit boolean to FALSE so that timing advance
       * is not taken into account when adjusting the acq window.
       */
      gl1_defs_tn_struct  rx_tn_info = { GL1_DEFS_TN_0, FALSE };
      gl1_defs_tn_struct  tx_tn_info = { GL1_DEFS_TN_0, FALSE };

      l1_sc_acq_data_ptr->fcb.rx_cb.data_cb    = ncell_fcch_tch_data_callback;
      l1_sc_acq_data_ptr->fcb.rx_rpt.data      = &l1_sc_acq_data_ptr->fcb.decode_rpt;
      l1_sc_acq_data_ptr->fcb.rx_cb.metrics_cb = NULL;
      l1_sc_acq_data_ptr->fcb.rx_cb.log_cb     = NULL;
      l1_sc_acq_data_ptr->fcb.rx_cb.abort_cb   = ncell_fcch_tch_abort_callback;

      gl1_msg_cfg_ncell_fcch( &l1_sc_acq_data_ptr->fcb.rx_cb,gas_id );

      l1_sc_acq_data_ptr->fcb.start_fn = l1_scdrv_get_fn( gas_id );

      /*
        Add 1 since we're triggering SC from l1_tch one frame earlier
        than the CCWW code.  This is called from the ISR since we need
        start_fn to be accurate.  Its used later to predict when to
        expect the SCH
       */
      l1_sc_acq_data_ptr->fcb.start_fn   = ADD_FN( l1_sc_acq_data_ptr->fcb.start_fn, 1 );

#ifdef FEATURE_GSM_DTM
      /* For DTM and Dedicated we slam to place the CS TN on relative
       * TN 2
       */
      if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
      {
        if (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
        {
          tx_tn = GL1_DEFS_TN_2;
          rx_tn = GL1_DEFS_TN_2;

        }else
        {
          rx_tn = GL1_DEFS_TN_0;
          tx_tn = GL1_DEFS_TN_0;
        }
      }
      else
      {
        tx_tn = (gl1_defs_tn_type)gprs_serv_cell_meas_ptr->highest_tn;
        rx_tn = GL1_DEFS_TN_0;
      }
#else
      /*
         The 0 is the sliding window to accomodate the rx slots in GPRS.
         Set to 0 for GSM
       */
      rx_tn = GL1_DEFS_TN_0;
      tx_tn = GL1_DEFS_TN_0;
#endif

      /* Fill in the structure */
      rx_tn_info.tn = rx_tn;
      tx_tn_info.tn = (gl1_defs_tn_type) ADD_MOD( (uint32) tx_tn, 3, GL1_DEFS_SLOTS_IN_FRAME );

      gl1_msg_rx_ncell_fcch ( &( rx_tn_info ),
                              &( tx_tn_info ), /* Tx TS rx equivalent */
                              l1_sc_acq_data_ptr->fcb.arfcn,
                              l1_sc_acq_data_ptr->fcb.agc,
                              &l1_sc_acq_data_ptr->fcb.rx_rpt,
                              gas_id );

      gl1_set_ncell_block_start(TRUE,gas_id);

      l1_sc_acq_data_ptr->fcb.state = FB_ACTIVE;
   }
   else
   {
      MSG_GERAN_ERROR_1_G("Unexpected call to l1_sci_tick_fcb_tch %d", l1_sc_acq_data_ptr->fcb.state);
   }
}

/*
 *     Function name :  l1_sci_abort_fcb_tch
 *     -------------------------------------
 *
 *     Description :
 *     ---------------
 *     Abort an FCB search. Called with interrupts locked.
 *     Aborts FCB receives started in TCH or Trans.
 */
void l1_sci_abort_fcb_tch( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

   ASSERT_INT();

   switch (l1_sc_acq_data_ptr->fcb.state)
   {
      case FB_INIT:
         l1_sc_acq_data_ptr->fcb.state = FB_NULL;

         /* msg layer is inactive so we can abort immediately */
         l1_sc_abort_receive_callback(gas_id);
         break;

      case FB_ACTIVE:
         l1_sc_acq_data_ptr->fcb.state = FB_ABORTING;

         /* need to abort the msg layer */
         gl1_msg_abort_rx_ncell_fcch(DELAYED_ABORT, gas_id);
         break;

      case FB_ABORTING:
         /* do nothing */
         MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"Multiple aborts.");
         break;

      case FB_NULL:
      case FB_RAN_OUT_OF_FRAMES:
      default:
         l1_sc_acq_data_ptr->fcb.state = FB_NULL;

         /* this should never happen */
         l1_sc_abort_receive_callback(gas_id);
         MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Unexpected state %d",
                    l1_sc_acq_data_ptr->fcb.state);
         break;

   }
}

static void ncell_fcch_sch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

   l1_fm_clear_required_frames(FM_PRI_NCELL_ACQ, gas_id);

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif

   //If tone was detected or acquistion was successful
   if (rpt->acq.success)
   {
      l1_sc_acq_data_ptr->fcb.success        = TRUE;
      l1_sc_acq_data_ptr->fcb.sch_decoded    = TRUE;
#ifdef FEATURE_GSM_POWER_OPTION16
      rpt->acq.time_offset                  +=  l1_sc_acq_data_ptr->fcb.start_qbit;
#endif /* FEATURE_GSM_POWER_OPTION16 */
      l1_sc_acq_data_ptr->fcb.start_fn       = ADD_FN( l1_sc_acq_data_ptr->fcb.start_fn, (uint32)(rpt->acq.time_offset / 5000) );
      l1_sc_acq_data_ptr->fcb.start_qbit     = (int16)(rpt->acq.time_offset % 5000);
      l1_sc_acq_data_ptr->fcb.sch_data[0]    = rpt->acq.sch_data[0];
      l1_sc_acq_data_ptr->fcb.sch_data[1]    = rpt->acq.sch_data[1];
      l1_sc_acq_data_ptr->fcb.sch_data[2]    = rpt->acq.sch_data[2];
      l1_sc_acq_data_ptr->fcb.sch_data[3]    = rpt->acq.sch_data[3];

      MSG_GERAN_MED_3_G( GL1_ACQ_HDR"Ncell FB SB success: ARFCN=%d Offset=%d RxLVL=%d",
               rpt->acq.arfcn.num,
               rpt->acq.time_offset,
               rpt->acq.pwr_dBm_x16 );

      /* Update the rotator value stored for this NCELL */
      if( NULL != l1_sc_acq_data_ptr->fcb.afc_info_ptr )
      {

        l1_sc_acq_data_ptr->fcb.afc_info_ptr->afc_freq   = (int32)rpt->acq.afc_freq;
        l1_sc_acq_data_ptr->fcb.afc_info_ptr->freq_error = gl1_hw_normxofreqtofreq( rpt->acq.arfcn  ,
                     ((int32)rpt->acq.afc_freq - gl1_hw_get_xo_acc_freq_err(gas_id)), gas_id );

        l1_sc_acq_data_ptr->fcb.afc_info_ptr->freq_error = gl1_hw_normxofreqtofreq( rpt->acq.arfcn  ,
                     ((int32)rpt->acq.afc_freq - gl1_hw_get_xo_acc_freq_err(gas_id)), gas_id );

        l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid = TRUE;
        MSG_GERAN_MED_2_G( GL1_ACQ_HDR"Ncell FB/SB afc_freq %dXO and freq_err %dHz retained for acq use",
                 l1_sc_acq_data_ptr->fcb.afc_info_ptr->afc_freq, l1_sc_acq_data_ptr->fcb.afc_info_ptr->freq_error);
      }
      else
      {
        MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"NULL l1_sc_acq_data_ptr->fcb.afc_info_ptr");
      }

   } //if tone not detected
   else
   {
      l1_sc_acq_data_ptr->fcb.success      = FALSE;
      l1_sc_acq_data_ptr->fcb.sch_decoded  = FALSE;

      /* Update the rotator value stored for this NCELL */
      if( NULL != l1_sc_acq_data_ptr->fcb.afc_info_ptr )
      {
        l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid = FALSE;
      }
      else
      {
        MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"NULL fcb.afc_info_ptr");
      }

      MSG_GERAN_MED_1_G( GL1_ACQ_HDR"Ncell FB SB failed: ARFCN=%d",
               l1_sc_acq_data_ptr->fcb.arfcn.num);
   }



   l1_sc_acq_data_ptr->ncell_acq_retry       = FALSE;
   l1_sc_acq_data_ptr->ncell_acq_pending     = FALSE;
   l1_sc_acq_data_ptr->ncell_acq_in_progress = FALSE;
   l1_sc_acq_data_ptr->fcb.state             = FB_NULL;
   l1_sci_receive_done(gas_id);
}

static void ncell_fcch_tch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
   l1_sc_globals_T  *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);
   ASSERT_INT();

   if (rpt->fcch.tone_detected)
   {
    uint16 start_window_offset = 0;

    start_window_offset = gl1_get_ncell_start_acq_offset(gas_id); /*Get the value of start acq offset of NCELL FCCH window*/

    if (rpt->fcch.tone_offset < 0)  /*If the detected tone offset is less than 0*/
    {

     if(rpt->fcch.tone_offset < -(4 * QS_PER_SLOT)) /*If the tone offset is less than -2500 Qs then ignore it*/
     {
       rpt->fcch.tone_detected = FALSE;
       MSG_GERAN_HIGH_3_G( GL1_ACQ_HDR"Ncell TCH SC Tone Ignored:start window off=%d Offset=%d less than -2500 Qs",
       start_window_offset, (int16)rpt->fcch.tone_offset, 0 );
     }
     /*If the starting offset of ncell fcch tone window is greater than the offset of detected tone then ignore it*/
     else if(start_window_offset > ((rpt->fcch.tone_offset) + 5000))
     {
       rpt->fcch.tone_detected = FALSE;
       MSG_GERAN_HIGH_3_G( GL1_ACQ_HDR"Ncell TCH SC Tone Ignored:start window off=%d Offset=%d less than 0",
       start_window_offset, (int16)rpt->fcch.tone_offset, 0 );
     }

    }

   }

   //If tone was detected and lies within the acquisition window
   if (rpt->fcch.tone_detected)
   {
      gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, FALSE };
      gl1_defs_tn_struct  highest_tn_info = { GL1_DEFS_TN_3, FALSE };
      int16 ncell_sch_pos_in_acq_window;

       /*
          If tone lies outside acq window, ignore it.   It will fall at the
          beginning of the window 2 idle frames later
       */

       l1_sc_highest_lowest_timslot(&(lowest_tn_info), &(highest_tn_info), gas_id);
       highest_tn_info.tn = (gl1_defs_tn_type) ADD_MOD( (uint32) highest_tn_info.tn, 3, GL1_DEFS_SLOTS_IN_FRAME );

       /*The constraints apllied to the acquisition window differ for dedicated mode*/
       if ( l1_sc_globals_ptr->mode == L1SCModeDediTCh)
       {
         ncell_sch_pos_in_acq_window = gl1_msg_ncell_sch_pos_in_acq_window_dedic(
                                                 (int16)rpt->fcch.tone_offset,
                                                 &(lowest_tn_info),
                                                 &(highest_tn_info),
                                                 gas_id);
       }
       else
       {
         ncell_sch_pos_in_acq_window = gl1_msg_ncell_sch_pos_in_acq_window(
                                                 (int16)rpt->fcch.tone_offset,
                                                 &(lowest_tn_info),
                                                 &(highest_tn_info),
                                                 gas_id);
       }

       if ( ncell_sch_pos_in_acq_window == 0 )
       {
        l1_sc_acq_data_ptr->fcb.success = TRUE;

        MSG_GERAN_MED_2_G( GL1_ACQ_HDR"Ncell TCH SC Tone Ok:ARFCN=%d Offset=%d",
                 l1_sc_acq_data_ptr->fcb.arfcn.num, (int16)rpt->fcch.tone_offset);

#ifdef DED_SC_TONE_DETECT_DEBUG
        #error code not present
#endif

        /* Save the frequency offset information for later use in the
         * SCH decoding in dedicated.
         */
        if( NULL != l1_sc_acq_data_ptr->fcb.afc_info_ptr )
        {
          l1_sc_acq_data_ptr->fcb.afc_info_ptr->afc_freq   = (int32)rpt->fcch.afc_freq;

          l1_sc_acq_data_ptr->fcb.afc_info_ptr->freq_error = (int32)rpt->fcch.fine_freq;

          l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid = TRUE;

          MSG_GERAN_MED_2_G( GL1_ACQ_HDR"Ncell TCH FB afc_freq %dXO retained for acq use ff err %dHz",
                   l1_sc_acq_data_ptr->fcb.afc_info_ptr->afc_freq, l1_sc_acq_data_ptr->fcb.afc_info_ptr->freq_error);
        }
        else
        {
          MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"NULL afc_info_ptr");
        }
        /* Store qbit lag for later reference */
        l1_sc_acq_data_ptr->fcb.start_qbit     = (int16)rpt->fcch.tone_offset;
       }
       else
       {
           l1_sc_acq_data_ptr->fcb.success = FALSE;

           MSG_GERAN_HIGH_2_G(GL1_ACQ_HDR"Ncell TCH SC Tone:ARFCN=%d Offset=%d outside acq window",
                    l1_sc_acq_data_ptr->fcb.arfcn.num,(int16)rpt->fcch.tone_offset);
       }
   } //if tone not detected
   else
   {
        l1_sc_acq_data_ptr->fcb.success = FALSE;

        MSG_GERAN_MED_1_G( GL1_ACQ_HDR"Ncell TCH SC Tone failed: ARFCN=%d",
                 l1_sc_acq_data_ptr->fcb.arfcn.num);

#ifdef DED_SC_TONE_DETECT_DEBUG
        #error code not present
#endif

        if( NULL != l1_sc_acq_data_ptr->fcb.afc_info_ptr )
        {
          l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid = FALSE;
        }
   }



   l1_sc_acq_data_ptr->fcb.state = FB_NULL;
   l1_sci_receive_done(gas_id);

	/* inform MCPM that PTM/DED Ncell scenario is now over (FR54762)*/
    gl1_concman_config_params.type = CMGR_TRANSIENT_END;
    gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
    gl1_concman_config_params.update_scenario = 0;
    gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
    gl1_concman_config_params.start_timestamp = 0;
    gl1_concman_config_params.duration_msec = 0;
    gl1_concman_config_params.flow_ctrl_time_msec = 0;
    gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
    MSG_GERAN_HIGH_0_G("***sending scenario complete to MCPM***");

}

/*
 *     Function name :  fcb_aborted
 *     ----------------------------
 *
 *     Description :
 *     ---------------
 *     Callback to be called when an FCB search has to be aborted because
 *     layer1 needs frames that were previously available to the surround
 *     cell engine.
 *
 *     Parameters :
 *     ---------------
 *     IN - dummy - Not used.
 *
 */
static void fcb_aborted( void* dummy , gas_id_t gas_id)
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   NOTUSED( dummy );
   ASSERT_INT();

   if((l1_sc_acq_data_ptr->fcb.state != FB_ABORTING) )
   {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if ( (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_GRANTED) && gl1_msg_get_multi_sim_mode() )
      {
        MSG_GERAN_ERROR_0_G("Trying to abort FCB ACQ, when the SUB doesnt hold TRM, Clear the FM");
        l1_fm_clear_required_frames(FM_PRI_NCELL_ACQ, gas_id);
      }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      /* Abort the message layer only if a Ncell ACQ is in progress */
      if(l1_sc_acq_data_ptr->ncell_acq_in_progress)
      {
         /* Prepare to try again */
         l1_sc_acq_data_ptr->fcb.state = FB_RAN_OUT_OF_FRAMES;
         gl1_msg_abort_rx_ncell_fcch_sch(DELAYED_ABORT, gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         MSG_GERAN_HIGH_0_G("Delay release of TRM lock by 1 frame to allow cleanup to execute");

         l1_fm_set_suspend_trm_release( 1,
                                        FM_PRI_NCELL_ACQ, gas_id );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      }
      else
      {
         /* Prepare to try again */
         l1_sc_acq_data_ptr->fcb.state = FB_INIT;
         l1_fm_clear_required_frames(FM_PRI_NCELL_ACQ, gas_id);
      }
   }
   else
   {
      MSG_GERAN_MED_0_G( GL1_ACQ_HDR"Ncell ACQ Abort already in progress!");
   }

}

static void ncell_fcch_tch_abort_callback( gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
  gl1_concman_config_params_type gl1_concman_config_params;
  cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

   if (l1_sc_acq_data_ptr->fcb.state != FB_ABORTING)
   {
      MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Unexpected fcb_tch abort callback state %d",
                 l1_sc_acq_data_ptr->fcb.state);
   }

   l1_sc_acq_data_ptr->fcb.state = FB_NULL;
   l1_sc_abort_receive_callback(gas_id);

	/* inform MCPM that PTM/DED Ncell scenario is now over (FR54762)*/
     gl1_concman_config_params.type = CMGR_TRANSIENT_END;
     gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
     gl1_concman_config_params.update_scenario = 0;
     gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
     gl1_concman_config_params.start_timestamp = 0;
     gl1_concman_config_params.duration_msec = 0;
     gl1_concman_config_params.flow_ctrl_time_msec = 0;
     gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
     MSG_GERAN_HIGH_0_G("***sending scenario complete to MCPM***");

}


/*
 *     Function name :  l1_sc_read_fcb
 *     --------------------------------
 *
 *     Description :
 *     ---------------
 *     Read results of an FCB search.
 *     Must only be called after a call to l1_sci_tick_fcb() returned TRUE.
 *
 *     Parameters :
 *     ---------------
 *     OUT - start_fn    - Frame number where l1_scdrv_fcb_req() could have
 *                         been called the latest and still got the same FCB.
 *                         Only set if this function returns TRUE.
 *     OUT - start_qbit  - Quarter bit position of the FCB burst in the frame.
 *                         (0..4999)
 *                         Only set if this function returns TRUE.
 *
 *     Returns :
 *     -----------
 *     TRUE when an FCB has been found.
 *     FALSE if not.
 *
 */
boolean l1_sc_read_fcb( uint32* start_fn, int16* start_qbit, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_TASK();

   if ( l1_sc_acq_data_ptr->fcb.success )
   {
      *start_fn   = l1_sc_acq_data_ptr->fcb.start_fn;
      *start_qbit = l1_sc_acq_data_ptr->fcb.start_qbit;
   }
   return l1_sc_acq_data_ptr->fcb.success;
}


/*
 *     Function name :  l1_sc_read_fcbsb
 *     ----------------------------------
 *
 *     Description :
 *     ---------------
 *     Read results of a combined FCB and SB search.
 *     Must only be called after a call to l1_sci_tick_fcb() returned TRUE.
 *
 *     Parameters :
 *     ---------------
 *     OUT - sb_data      - Data of the received SB.
 *                          Only set if this function returns TRUE.
 *     OUT - start_fn     - Frame number where SB receive was requested
 *                          been called or one more, if start_qbit passed by
 *                          l1_sci_sb_cnf() was greater than 4999.
 *                          Only set if this function returns TRUE.
 *     OUT - timing_error - The quarter bit offset of the burst in the frame.
 *                          Only set if this function returns TRUE.
 *
 *     Returns :
 *     -----------
 *     TRUE when an FCB has been found and SCH has been decoded.
 *     FALSE if no FCB has been found, if the SCH was not decoded, or if the
 *     receive was aborted because no frames were available.
 *
 */
boolean l1_sc_read_fcbsb( uint8 sb_data[4], uint32* start_fn, int16* timing_error, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_TASK();

   if ( l1_sc_acq_data_ptr->fcb.success )
   {
      sb_data[0]  = l1_sc_acq_data_ptr->fcb.sch_data[0];
      sb_data[1]  = l1_sc_acq_data_ptr->fcb.sch_data[1];
      sb_data[2]  = l1_sc_acq_data_ptr->fcb.sch_data[2];
      sb_data[3]  = l1_sc_acq_data_ptr->fcb.sch_data[3];
      if ( l1_sc_acq_data_ptr->fcb.start_qbit >= 5000 )
      {
         *start_fn = ADD_FN( l1_sc_acq_data_ptr->fcb.start_fn, 1 );
      }
      else
      {
         *start_fn = l1_sc_acq_data_ptr->fcb.start_fn;
      }
      *timing_error = l1_sc_acq_data_ptr->fcb.start_qbit;
   }
   return l1_sc_acq_data_ptr->fcb.success;
}

/* The retry gap shall be tuned to minimise the number of retry. */
/* It shall be at least 18 for QTA compatible. */
#define L1_SC_FCB_RETRY_GAP     (18)

boolean l1_sc_is_ncell_acq_pending( gas_id_t gas_id )
{
    l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
    return (l1_sc_acq_data_ptr->ncell_acq_pending);
}

/* calculate fcb start frame number */
static void l1_sc_calc_fcb_start_fn( gas_id_t gas_id )
{
    l1_sc_globals_T  *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
    uint32 current_FN;
    uint32 load_frames = 0;
    uint32 start_frame_offset = 1;
    uint32 gap_to_next_activity = 0;

    current_FN = gl1_get_FN( gas_id );

    MSG_GERAN_MED_3_G(GL1_ACQ_HDR"Calculating FCH Start FN when Current FN:%d Mode:%d known_sch_timing:%d",current_FN,l1_sc_globals_ptr->mode,l1_sc_acq_data_ptr->fcb.known_sch_timing);

    /* Do not do Opt 16 in SDCCH mode. */
    if (l1_sc_globals_ptr->mode == L1SCModeDediSDCCh)
    {
        l1_sc_acq_data_ptr->fcb.known_sch_timing = FALSE;
    }
    else
    {
        if (l1_sc_is_idle_rx_in_progress( gas_id ) == FALSE)
        {
            start_frame_offset += load_frames;

            if (l1_sc_acq_data_ptr->ncell_acq_retry)
            {
                start_frame_offset += L1_SC_FCB_RETRY_GAP;
            }

            gap_to_next_activity = l1_sc_get_gap_to_next_activity(gas_id);

            MSG_GERAN_MED_3_G(GL1_ACQ_HDR"idle_rx_is_not_in_progress and gap_to_next_activity:%d start_frame_offset:%d fcb_duration:%d",
                              gap_to_next_activity, start_frame_offset, l1_sc_acq_data_ptr->fcb.fcb_detection_duration );

/* If gap to the next RX activity is too close, try FCB after that. */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if (((gap_to_next_activity > start_frame_offset) &&
                 (gap_to_next_activity < (l1_sc_acq_data_ptr->fcb.fcb_detection_duration + start_frame_offset + 1 )) &&
                  (!l1_sc_acq_data_ptr->ncell_acq_retry )) ||
                  ((OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)) || ( gl1_msg_get_multi_sim_standby_mode() && (gl1_get_num_inactive_subs() > 0 ))))
#else /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            if ((gap_to_next_activity > start_frame_offset) &&
                (gap_to_next_activity < (l1_sc_acq_data_ptr->fcb.fcb_detection_duration + start_frame_offset)))
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

            {
                start_frame_offset = gap_to_next_activity + load_frames;
                if ( l1_sc_acq_data_ptr->fcb.known_sch_timing )
                {
                    start_frame_offset += L1_DURATION_CCCH;
                }
                else
                {
                    /* frame can be available earlier for SBD and earlier decode. */
                    start_frame_offset += L1_DURATION_CCCH - 3;
                }
#ifdef FEATURE_GPRS_GBTA
                if ( ( ( OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)) || 
                       ( gl1_msg_get_multi_sim_standby_mode() && (gl1_get_num_inactive_subs() > 0 ) ) )
                )
                {
                  start_frame_offset = L1_ACQ_GTA_SUSP_GAP;
                  MSG_GERAN_MED_1_G(GL1_ACQ_HDR"start_frame_offset Revised Again:%d",start_frame_offset);
                }
#endif /* FEATURE_GPRS_GBTA */
            }
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            else
            {
              if(gl1_detect_page_collision_with_other_sub(gas_id))
              {
                start_frame_offset = ADD_FN(SUB_FN(paging_collision_fn[collision_sub[gas_id]],current_FN),L1_DURATION_CCCH);
                MSG_GERAN_HIGH_3_G("JS : MULTISIM: GAS ID 0x%02X collision_sub[gas_id]=%d paging_collision_fn=%d start_frame_offset=%d ", collision_sub[gas_id],paging_collision_fn[collision_sub[gas_id]],start_frame_offset );
              }
            }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
        }

    }

    if ( l1_sc_acq_data_ptr->fcb.known_sch_timing )
    {
        start_frame_offset++;
        load_frames++;

        if (start_frame_offset < L1_SC_FCB_RETRY_GAP + load_frames)
        {
            start_frame_offset = L1_SC_FCB_RETRY_GAP + load_frames;
        }
        l1_sc_acq_data_ptr->fcb.start_fn = next_sb_fn( ADD_FN(current_FN, start_frame_offset), gas_id );

        /* Open FCB scan window 1 timeslot before FCB burst. */
        if ( l1_sc_acq_data_ptr->sb.start_qbit >= L1_SC_DURATION_FCB_GUARD_QS )
        {
            l1_sc_acq_data_ptr->fcb.start_qbit  = l1_sc_acq_data_ptr->sb.start_qbit - L1_SC_DURATION_FCB_GUARD_QS;
        }
        else
        {
            l1_sc_acq_data_ptr->fcb.start_qbit  = l1_sc_acq_data_ptr->sb.start_qbit + 5000 - L1_SC_DURATION_FCB_GUARD_QS;
            l1_sc_acq_data_ptr->fcb.start_fn    = SUB_FN(l1_sc_acq_data_ptr->fcb.start_fn, 1);
        }
    }
    else
    {
#ifdef FEATURE_GPRS_GBTA
      if (((OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)) ||
           (gl1_msg_get_multi_sim_standby_mode() && (gl1_get_num_inactive_subs() > 0)))
          )
      {
        /* G in Data or X tech will not be able to suspend activity in time
         * for FCB in next frame to occur so schedule for L1_ACQ_GTA_SUSP_GAP
         */
        start_frame_offset = L1_ACQ_GTA_SUSP_GAP;
        MSG_GERAN_MED_1_G(GL1_ACQ_HDR"start_frame_offset Revised:%d", start_frame_offset);

        if(gl1_detect_page_collision_with_other_sub(gas_id))
        {
          start_frame_offset = ADD_FN(SUB_FN(paging_collision_fn[collision_sub[gas_id]],current_FN),L1_DURATION_CCCH);
          if( OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
            {
              start_frame_offset= start_frame_offset + L1_NCELL_ACQ_GAP_TO_GTA;
            }
           MSG_GERAN_HIGH_1_G("Start_frame_offset Revised =%d ",start_frame_offset );
        }

      }
#endif /* FEATURE_GPRS_GBTA */
        l1_sc_acq_data_ptr->fcb.start_fn   = ADD_FN(current_FN, start_frame_offset);

        l1_sc_acq_data_ptr->fcb.start_qbit = 0;
    }

#ifdef DEBUG_SCE_FCB_MSG
    MSG_GERAN_MED_3_G( GL1_ACQ_HDR"SC ACQ starts at FN %u in %d frames. CCCH gap %d",
             l1_sc_acq_data_ptr->fcb.start_fn, start_frame_offset, gap_to_next_activity );
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif
}

/* Update FCB start FN with the given frames */
void l1_sc_update_fcb_start_fn( uint8 frames, gas_id_t gas_id )
{
    if ( frames != 0)
    {
        l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

        l1_sc_acq_data_ptr->fcb.start_fn = ADD_FN(l1_sc_acq_data_ptr->fcb.start_fn, frames);
        MSG_GERAN_HIGH_2_G("add %d frames, updated FCB start FN %d",frames, l1_sc_acq_data_ptr->fcb.start_fn);
    }
}

/* Initialise fcb sch acquisition for idle. */
static void l1_sc_init_idle_fcb_acq( gas_id_t gas_id )
{
    l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
    l1_sc_globals_T  *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    uint32 current_FN;

    current_FN = gl1_get_FN( gas_id );

    l1_sc_acq_data_ptr->fcb.fcb_detection_duration  = fcb_detection_duration( gas_id );

    MSG_GERAN_MED_3_G( GL1_ACQ_HDR"FN = %d SC ACQ init, requires %d frames GAS %d",
             current_FN, l1_sc_acq_data_ptr->fcb.fcb_detection_duration, gas_id );

    l1_sc_calc_fcb_start_fn( gas_id );

    /* The number of frames available for FCCH/SCH acquisition on SDCCH and IDLE modes differs.
       Check which mode of operation is active prior to scheduling for this acquisition. */
    if (l1_sc_globals_ptr->mode == L1SCModeDediSDCCh)
    {
        l1_sc_acq_data_ptr->fcb.do_AFC              = FALSE;
        l1_sc_acq_data_ptr->fcb.fcch_frames         = SYS_PARAM_MAX_FRAMES_FOR_NCELL_FCCH_DETECTION;
        l1_sc_acq_data_ptr->fcb.sch_frames          = SYS_PARAM_MAX_FRAMES_FOR_SCH_NCELL_DECODING;
        l1_sc_acq_data_ptr->fcb.sch_attempts        = SYS_PARAM_MAX_ATTEMPTS_FOR_NCELL_SCH_DECODING;
    }
    else
    {
        l1_sc_acq_data_ptr->fcb.do_AFC              = TRUE;
        /* If re-use known SCH timing, reduce FCCH/SCH acquisition duration. */
        if ( l1_sc_acq_data_ptr->fcb.known_sch_timing )
        {
            l1_sc_acq_data_ptr->fcb.fcch_frames     = SYS_PARAM_MAX_FRAMES_FOR_FCCH_DETECTION_KNOWN_SCH_TIMING;
            l1_sc_acq_data_ptr->fcb.sch_frames      = SYS_PARAM_MAX_FRAMES_FOR_SCH_DECODING_KNOWN_SCH_TIMING;
            l1_sc_acq_data_ptr->fcb.sch_attempts    = SYS_PARAM_MAX_ATTEMPTS_FOR_SCH_DECODING_KNOWN_SCH_TIMING;
        }
        else
        {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#if defined (FEATURE_QTA) || defined (FEATURE_GPRS_GBTA)
            if( gl1_msg_get_multi_sim_standby_mode() && (( gl1_get_num_inactive_subs() > 0 )||OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)) )
            {
                l1_sc_acq_data_ptr->fcb.fcch_frames     = SYS_PARAM_MAX_FRAMES_FOR_FCCH_DETECTION_QTA;
                l1_sc_acq_data_ptr->fcb.sch_frames      = SYS_PARAM_MAX_FRAMES_FOR_SCH_DECODING_QTA;
                l1_sc_acq_data_ptr->fcb.sch_attempts    = SYS_PARAM_MAX_ATTEMPTS_FOR_SCH_DECODING_QTA;
            }
            else
#endif /* FEATURE_QTA | FEATURE_GPRS_GBTA */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            {
            l1_sc_acq_data_ptr->fcb.fcch_frames     = SYS_PARAM_MAX_FRAMES_FOR_FCCH_DETECTION;
            l1_sc_acq_data_ptr->fcb.sch_frames      = SYS_PARAM_MAX_FRAMES_FOR_SCH_DECODING;
            l1_sc_acq_data_ptr->fcb.sch_attempts    = SYS_PARAM_MAX_ATTEMPTS_FOR_SCH_DECODING;
        }
    }
    }

#ifdef DEBUG_SCE_FCB_MSG
    MSG_GERAN_MED_3_G( GL1_ACQ_HDR"SC FCB ACQ requires %d frames at FN=%u start_qbit %d",
             l1_sc_acq_data_ptr->fcb.fcb_detection_duration,
             l1_sc_acq_data_ptr->fcb.start_fn,
             l1_sc_acq_data_ptr->fcb.start_qbit );
#endif

    if(!gl1_msg_get_multi_sim_mode())
    {
      l1_sc_acq_data_ptr->fcb.drx_ref = gl1_drx_require_tick( l1_sc_acq_data_ptr->fcb.start_fn, gas_id );
    }
    l1_fm_set_required_frame_number( FM_PRI_NCELL_ACQ,
                                     l1_sc_acq_data_ptr->fcb.start_fn,
                                     l1_sc_acq_data_ptr->fcb.fcb_detection_duration,
                                     gas_id );
    l1_sc_acq_data_ptr->fcb.state = FB_WAIT;

    if ( l1_sc_acq_data_ptr->fcb.known_sch_timing == FALSE )
    {
        l1_sc_acq_data_ptr->ncell_acq_pending = TRUE;
    }
}

/* kick off an fcb sch acq for idle */
static void l1_sc_start_idle_fcb_acq( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

   MSG_GERAN_MED_2_G(GL1_ACQ_HDR"FN = %d SC FCB ACQ starts GAS %d",gl1_get_FN( gas_id ),gas_id);

   /* Set up stuff required by the gl1_msg layer */
   l1_sc_acq_data_ptr->fcb.rx_rpt.data      = &l1_sc_acq_data_ptr->fcb.decode_rpt;
   l1_sc_acq_data_ptr->fcb.rx_cb.metrics_cb = NULL;
   l1_sc_acq_data_ptr->fcb.rx_cb.log_cb     = NULL;
   l1_sc_acq_data_ptr->fcb.rx_cb.data_cb    = ncell_fcch_sch_data_callback;
   l1_sc_acq_data_ptr->fcb.rx_cb.abort_cb   = ncell_fcch_sch_abort_callback;

   gl1_msg_cfg_ncell_fcch_sch( &l1_sc_acq_data_ptr->fcb.rx_cb, l1_sc_acq_data_ptr->fcb.do_AFC, gas_id );

   /* Start the acquisition */
   gl1_msg_rx_fcch_sch( l1_sc_acq_data_ptr->fcb.arfcn,                 /* ARFCN */
                        l1_sc_acq_data_ptr->fcb.agc,                   /* AGC parameters */
                        l1_sc_acq_data_ptr->fcb.start_qbit,            /* start offset */
                        l1_sc_acq_data_ptr->fcb.fcch_frames,           /* FCCH frames  */
                        l1_sc_acq_data_ptr->fcb.sch_frames,            /* SCH frames   */
                        l1_sc_acq_data_ptr->fcb.sch_attempts,          /* SCH attempts */
                        l1_sc_acq_data_ptr->fcb.do_AFC,                /* do AFC */
                        FALSE,                                         /* do AGC */
                        FALSE,                                         /* early_callback */
                        &l1_sc_acq_data_ptr->fcb.rx_rpt,               /* report */
                        GSM_INVALID_BSIC,
                        gas_id );

   gl1_set_ncell_block_start(TRUE, gas_id);
   l1_sc_acq_data_ptr->fcb.start_fn = l1_scdrv_get_fn( gas_id );
   l1_sc_acq_data_ptr->fcb.state = FB_ACTIVE;
   l1_sc_acq_data_ptr->ncell_acq_pending = FALSE;
   l1_sc_acq_data_ptr->ncell_acq_in_progress = TRUE;
   gl1_drx_require_next_tick(gas_id);
}

/* Get FCB detection duration. */
static uint16 fcb_detection_duration( gas_id_t gas_id )
{
    l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
    l1_sc_globals_T  *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

    /* The number of frames available for FCCH/SCH acquisition on SDCCH and IDLE modes differs. Check which mode of
    operation is active prior to scheduling for this acquisition. */
    if (l1_sc_globals_ptr->mode == L1SCModeDediSDCCh)
    {
        return L1_SC_DURATION_FCB_SDCCH;
    }
    else
    {
        if ( l1_sc_acq_data_ptr->fcb.known_sch_timing )
        {
            return L1_SC_DURATION_FCB_IDLE_KNOWN_SCH_TIMING;
        }
        else
        {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#if defined (FEATURE_QTA) || defined (FEATURE_GPRS_GBTA)
            if( gl1_msg_get_multi_sim_standby_mode() && ( ( gl1_get_num_inactive_subs() > 0 ) || OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)) )
            {
                return L1_SC_DURATION_FCB_IDLE_QTA;
            }
            else
#endif /* FEATURE_QTA|FEATURE_GPRS_GBTA*/
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            {
            return L1_SC_DURATION_FCB_IDLE;
        }
    }
    }
}

/***
 * Module:
 * -------
 * Contains state machine to receive SB.
 *
 ***/

/*
 *     Function name :  l1_sc_sb_init
 *     ------------------------------
 *
 *     Description :
 *     ---------------
 *     Init static data in this file.
 *
 */
void l1_sc_sb_init( gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   l1_sc_acq_data_ptr->sb.state = SB_NULL;
}

/*
 *     Function name :  l1_sc_sb_shutdown
 *     ----------------------------------
 *
 *     Description :
 *     ---------------
 *     Clean up static data in this file.
 *
 */
void l1_sc_sb_shutdown( gas_id_t gas_id )
{
}

/*
 *     Function name :  l1_sc_start_sb
 *     --------------------------------
 *
 *     Description :
 *     ---------------
 *     Setup the SB state machine to do a receive.
 *     The SB state machine waits until its time to receive an SB so its
 *     not necessary to call l1_sci_start_sb() at a particular frame in the multiframe.
 *     When no frames are available to receive the SB, the state machine tries
 *     again until L1_SC_SB_MAX_TRIES tries have been made, at which point
 *     the state machine terminated and returns failure to get the SB.
 *     After calling l1_sci_start_sb(), l1_sci_tick_sb() must be called every frame to
 *     perform the search until l1_sci_tick_sb() returns TRUE or the search is
 *     aborted via l1_sci_abort_sb().
 *     The first call to l1_sci_tick_sb() must be done in the frame following the
 *     frame where l1_sci_start_sb() has been called.
 *
 *
 */
/* call with interrupts locked */
void l1_sc_start_sb( cell_T* cell, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_TASK();

   if ( l1_sc_acq_data_ptr->sb.state != SB_NULL )
   {
      MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"ncell SCH already in progress");
   }

   /* store the info that we need */
   l1_sc_acq_data_ptr->sb.arfcn          = cell->bs.arfcn;
   l1_sc_acq_data_ptr->sb.agc            = &cell->bs.agc;
#ifdef FEATURE_GSM_RX_DIVERSITY
   l1_sc_acq_data_ptr->sb.agc_divrx      = &cell->bs.agc_divrx;
#endif
   l1_sc_acq_data_ptr->sb.fn_lag         = cell->bs.fn_lag;
   l1_sc_acq_data_ptr->sb.start_qbit     = cell->bs.qbit_lag;
   l1_sc_acq_data_ptr->sb.tries          = 0;
   l1_sc_acq_data_ptr->sb.trm_denials    = 0;
   l1_sc_acq_data_ptr->sb.mask           = cell->bs.bsic_known ? 0x1F : 0x01;

   l1_sc_acq_data_ptr->sb.state          = SB_INIT;

   l1_sc_acq_data_ptr->sb.afc_info_ptr   = &( cell->afc_info );
}

/*
 *     Function name :  l1_sci_tick_sb
 *     -------------------------------
 *
 *     Description :
 *     ---------------
 *     Tick the SB state machine.
 *     After calling l1_sci_start_sb() this function must be called every frame
 *     until l1_sci_tick_sb() returns TRUE or the search is aborted via
 *     l1_sci_abort_sb().
 *     The first call to l1_sci_tick_sb() must be done in the frame following the
 *     frame where l1_sci_start_sb() has been called.
 *
 *     Returns :
 *     -----------
 *     FALSE when l1_sci_tick_sb() should be called again.
 *     TRUE when done.
 *
 */
void l1_sci_tick_sb( gas_id_t gas_id )
{
   gl1_msg_rx_cb_type rx_cb;
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   boolean skip_sch_scheduling = FALSE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   ASSERT_INT();

   switch ( l1_sc_acq_data_ptr->sb.state )
   {
      case SB_INIT:
      {
         uint32 current_FN;
         uint32 abs_sb_start_fn;
         uint32 frames_reqd;
         /* Clear the flags so we know when both callbacks have been called */
         l1_sc_acq_data_ptr->sb.data_done      = FALSE;
         l1_sc_acq_data_ptr->sb.metrics_done   = FALSE;

         /* Set up stuff required by the gl1_msg layer             */
         /* Some of this doesn't really need to be done every time */
         l1_sc_acq_data_ptr->sb.rx_rpt.data       = &l1_sc_acq_data_ptr->sb.decode_rpt;
         l1_sc_acq_data_ptr->sb.rx_rpt.metrics[0] = &l1_sc_acq_data_ptr->sb.metrics_rpt;

         /* Make sure that next rx is far enough into future that
          * L1 can sleep in the meantime.  This turns out to save
          * more pwoer than making the rx sooner and staying awake.
          *
          * For background PLMN search the rx must be at least 4
          * frames in the future to allow the background PLMN
          * search to abort if required.
          */
         current_FN  = gl1_get_FN( gas_id );
#if defined (FEATURE_G2X_TUNEAWAY) || defined (FEATURE_GPRS_GBTA)
         if ( gl1_msg_get_multi_sim_standby_mode())
         {
           l1_sc_acq_data_ptr->sb.start_fn = next_sb_fn( ADD_FN(current_FN, 18), gas_id );
         }
         else
#endif /* FEATURE_G2X_TUNEAWAY */
         {
           l1_sc_acq_data_ptr->sb.start_fn = next_sb_fn( ADD_FN(current_FN, 8), gas_id );
         }

         if ( (l1_sc_acq_data_ptr->sb.start_qbit + 5000) >= MAX_NCELL_SCH_OFFSET)
         {
           frames_reqd = L1_SC_DURATION_SB + 1;
         }
         else
         {
           frames_reqd = L1_SC_DURATION_SB;
         }

         if(!gl1_msg_get_multi_sim_mode())
         {
           l1_sc_acq_data_ptr->sb.drx_ref = gl1_drx_require_tick( l1_sc_acq_data_ptr->sb.start_fn, gas_id );
         }

         l1_fm_set_required_frame_number( FM_PRI_NCELL_SCH, l1_sc_acq_data_ptr->sb.start_fn, frames_reqd, gas_id);

         l1_sc_acq_data_ptr->sb.state = SB_WAIT;

         abs_sb_start_fn = SUB_FN(l1_sc_acq_data_ptr->sb.start_fn,current_FN);

         MSG_GERAN_MED_1_G(GL1_ACQ_HDR"%d frames to SCH receive",
                           abs_sb_start_fn);
      }
      /* fall through to next case */
      case SB_WAIT:
         if ( l1_sc_acq_data_ptr->sb.start_fn == gl1_get_FN( gas_id ) )
         {
            uint32 frames_reqd;
            l1_sc_acq_data_ptr->sb.drx_ref = 0;
            if ( (l1_sc_acq_data_ptr->sb.start_qbit + 5000) >= MAX_NCELL_SCH_OFFSET)
            {
              frames_reqd = L1_SC_DURATION_SB + 1;
            }
            else
            {
              frames_reqd = L1_SC_DURATION_SB;
            }

              if ( l1_fm_is_activity_reserved(FM_PRI_NCELL_SCH, gas_id) &&
                   l1_fm_are_frames_available(FM_PRI_NCELL_SCH, frames_reqd, sb_aborted, 0, gas_id))
              {
               int32 freq_error = 0;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               boolean qta_rf_init_failed = FALSE;

               /* Make sure that FW/RF are initialised and if this is "NOT" first QTA gap after wake up from sleep*/
               if ( gl1_msg_get_multi_sim_mode() &&
                   ( l1_tskisr_blk->firmware_loaded == FALSE ) &&
                       ( gl1_check_qta_active_in_warmup(gas_id) == FALSE ))
               {
                   if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_NOT_GRANTED )
                   {
                       l1_tskisr_blk->firmware_loaded = TRUE;
#ifdef FEATURE_QTA
                       if ( l1_tskisr_blk->qta_in_progress )
                       {
                           uint16 offset;
                           MSG_GERAN_HIGH_1_G("FN = %d NCELL SCH - QTA in progress",gl1_get_FN( gas_id ));
#ifdef FEATURE_QBTA                              
#ifdef ZIF_TUNE_TIME_FIXED
                           #error code not present
#else
                           /* GFW defines SCH RF trigger at half search width before Rx burst. */
                           offset = l1_sc_acq_data_ptr->sb.start_qbit + 5000 - (GL1_HW_SCH_SEARCH_WIDTH_DEFAULT * 4 / 2);
#endif /* ZIF_TUNE_TIME_FIXED */
#else
                           offset = GL1_FW_QBTA_CM_START_UP_OFFSET ;                              
#endif  
                           if ( !gl1_hw_qta_fw_rf_init(FALSE,offset,FALSE,l1_sc_acq_data_ptr->sb.arfcn,FALSE,TRUE,gas_id) )
                           {
                               /* give up on this SB */
                               qta_rf_init_failed = TRUE;
                               l1_tskisr_blk->firmware_loaded = FALSE;
                           }
                           else
                           {
                               /* Prevent FM from releasing in the same frame */
                               l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                              FM_PRI_NCELL_SCH, gas_id );
                           }
                       }
                       else
#endif /* FEATURE_QTA*/
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
                                                          FM_PRI_NCELL_SCH, gas_id );
                       }
                   }
               }

               /* We would expect that the RF activity would have finished and firward loaded.
                            But there is possibility that ISR may have overrun or we may be very close to the frame boundary,
                            hence to avoid 2 sync RX issued to GFW we tick over and don't schedule commands in this frame.  */
               if (qta_rf_init_failed || gl1_check_possible_isr_overrun(gas_id))
               {
                 skip_sch_scheduling = TRUE;
                 /*Dont consider this as an SB attempt*/
                 if (l1_sc_acq_data_ptr->sb.tries)
                    l1_sc_acq_data_ptr->sb.tries--;
                 MSG_GERAN_MED_2_G( GL1_ACQ_HDR"SCH (ARFCN=%d) skipped, possible ISR overrun",!gstmr_is_int_pending(gas_id), gstmr_rd_qsymbol_count(geran_map_gas_id_to_nas_id(gas_id)));
               }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               if ( ((!gl1_msg_get_multi_sim_mode()) || ( l1_tskisr_blk->firmware_loaded == TRUE))
                && (!skip_sch_scheduling))
#else
               if ( !gl1_msg_get_multi_sim_mode() )
#endif
               {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#if defined (FEATURE_QBTA) && defined (FEATURE_WCDMA)
                 if((gl1_hw_check_in_QBTA(gas_id)) && (!gl1_hw_get_qta_cm_fw_rf_init_status(gas_id)))
                 {
                   gl1_hw_set_QBTA_flag(gas_id,FALSE);
                   gl1_hw_qta_fw_rf_init(FALSE,GL1_FW_QBTA_CM_START_UP_OFFSET,TRUE,l1_sc_acq_data_ptr->sb.arfcn,FALSE,TRUE,gas_id);
                 }
#endif
#endif
               rx_cb.data_cb    = ncell_sch_data_callback;
               rx_cb.metrics_cb = ncell_sch_metrics_callback;
               rx_cb.log_cb     = NULL;
               rx_cb.abort_cb   = ncell_sch_abort_callback;

               /* Apply the afc freq from the fcch attempt if found to
                * be valid.
                */
               if(  ( NULL != l1_sc_acq_data_ptr->sb.afc_info_ptr )
                  &&( FALSE != l1_sc_acq_data_ptr->sb.afc_info_ptr->valid )
                 )
               {
                 freq_error = (int32)l1_sc_acq_data_ptr->sb.afc_info_ptr->freq_error;

                 MSG_GERAN_MED_1_G( GL1_ACQ_HDR"Ncell SB apply freq_error delta %dHz",
                          freq_error);
               }

               gl1_msg_cfg_ncell_sch( &rx_cb,gas_id);

               /* Uncomment once ZIF tune time is reduced */
#ifdef ZIF_TUNE_TIME_FIXED
               #error code not present
#else
               /*
                  ZIF_WORKAROUND
                  Wake up 2 frames before SCH burst.  Workaround for
                  SBI errors due to large ZIF tune time.  Will be
                  pulled out after ZIF tune time is reduced.  Increase
                  qbit_lag by 5000 since we're waking up 2 frames in advance
               */
               gl1_msg_rx_ncell_sch ( l1_sc_acq_data_ptr->sb.arfcn,
                                      l1_sc_acq_data_ptr->sb.agc,
#ifdef FEATURE_GSM_RX_DIVERSITY
                                      l1_sc_acq_data_ptr->sb.agc_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                                      (uint16)(l1_sc_acq_data_ptr->sb.start_qbit + 5000),
                                      freq_error,
                                      &l1_sc_acq_data_ptr->sb.rx_rpt,gas_id );
#endif
               gl1_set_ncell_block_start(TRUE,gas_id);

               l1_sc_acq_data_ptr->sb.state = SB_ACTIVE;

               l1_sc_acq_data_ptr->ncell_sch_in_progress = TRUE;
               gl1_drx_require_next_tick(gas_id);
           }
           else if (( ++l1_sc_acq_data_ptr->sb.tries == L1_SC_SB_MAX_TRIES) 
#ifdef FEATURE_QTA		   
		   || ( l1_tskisr_blk->qta_in_progress )
#endif
		   )
           {
              l1_sc_acq_data_ptr->sb.result = L1_SC_BLOCKED;
              l1_sc_acq_data_ptr->sb.state  = SB_NULL;

              l1_sci_receive_done(gas_id);
              l1_fm_clear_required_frames(FM_PRI_NCELL_SCH, gas_id);
              gl1_drx_require_next_tick(gas_id);
              MSG_GERAN_HIGH_2_G( GL1_ACQ_HDR"SCH (ARFCN=%d) abandoned (%d tries)",
                        l1_sc_acq_data_ptr->sb.arfcn.num, l1_sc_acq_data_ptr->sb.tries);
           }
           else
           {
              l1_sc_acq_data_ptr->sb.start_fn = next_sb_fn( gl1_get_FN( gas_id ) , gas_id);

              if(!gl1_msg_get_multi_sim_mode())
              {
                l1_sc_acq_data_ptr->sb.drx_ref  = gl1_drx_require_tick( l1_sc_acq_data_ptr->sb.start_fn, gas_id );
              }

               if ( (l1_sc_acq_data_ptr->sb.start_qbit + 5000) >= MAX_NCELL_SCH_OFFSET)
               {
            frames_reqd = L1_SC_DURATION_SB + 1;
           }
           else
           {
            frames_reqd = L1_SC_DURATION_SB;
           }

              l1_fm_set_required_frame_number( FM_PRI_NCELL_SCH, l1_sc_acq_data_ptr->sb.start_fn, frames_reqd, gas_id);
           }
        }
            else if ( (++l1_sc_acq_data_ptr->sb.tries == L1_SC_SB_MAX_TRIES) ||
                      (l1_fm_leaving_idle_mode(gas_id) == TRUE) )
            {
               l1_sc_acq_data_ptr->sb.result = L1_SC_BLOCKED;
               l1_sc_acq_data_ptr->sb.state  = SB_NULL;

               l1_sci_receive_done(gas_id);
               l1_fm_clear_required_frames(FM_PRI_NCELL_SCH, gas_id);
               gl1_drx_require_next_tick(gas_id);
               MSG_GERAN_HIGH_2_G( GL1_ACQ_HDR"SCH (ARFCN=%d) abandoned (%d tries)",
                         l1_sc_acq_data_ptr->sb.arfcn.num, l1_sc_acq_data_ptr->sb.tries);
            }
            else
            {
              uint32 serv_fn = gl1_get_FN( gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
              if( gl1_msg_get_multi_sim_mode() && l1_fm_allowed_but_blocked_by_trm( FM_PRI_NCELL_SCH, gas_id ))
              {
                if ( ++l1_sc_acq_data_ptr->sb.trm_denials > 3 )
                {
                  /* delay by 2 second after 4 consecutive trm denials */
                  serv_fn = ADD_FN(serv_fn, DELAY_SCH_SCHEDULING);
                }
              }
#endif
               l1_sc_acq_data_ptr->sb.start_fn = next_sb_fn( serv_fn , gas_id);
               if(!gl1_msg_get_multi_sim_mode())
               {
                 l1_sc_acq_data_ptr->sb.drx_ref  = gl1_drx_require_tick( l1_sc_acq_data_ptr->sb.start_fn, gas_id );
               }

            if ( (l1_sc_acq_data_ptr->sb.start_qbit + 5000) >= MAX_NCELL_SCH_OFFSET)
            {
             frames_reqd = L1_SC_DURATION_SB + 1;
            }
            else
            {
             frames_reqd = L1_SC_DURATION_SB;
            }

               l1_fm_set_required_frame_number( FM_PRI_NCELL_SCH, l1_sc_acq_data_ptr->sb.start_fn, frames_reqd, gas_id);
            }
          }
         else if (IS_FRAME_NUM_LATER(gl1_get_FN( gas_id ), l1_sc_acq_data_ptr->sb.start_fn))
         {
            /*Rebook SB activity if earlier occassion is missed in any case -
                       a case where G-Wakeup is deferred due to W activity*/
            uint32 frames_reqd = 0;

            /* Our tick is skipped so drx_ref is no longer valid. */
            l1_sc_acq_data_ptr->sb.drx_ref = 0;
            MSG_GERAN_HIGH_1_G( GL1_ACQ_HDR"SCH (ARFCN=%d) - Rebook tick, as current booking at %d elapsed", l1_sc_acq_data_ptr->sb.start_fn);
            l1_sc_acq_data_ptr->sb.start_fn = next_sb_fn( gl1_get_FN( gas_id ) , gas_id);

            if(!gl1_msg_get_multi_sim_mode())
            {
              l1_sc_acq_data_ptr->sb.drx_ref  = gl1_drx_require_tick( l1_sc_acq_data_ptr->sb.start_fn, gas_id );
            }

            if ( (l1_sc_acq_data_ptr->sb.start_qbit + 5000) >= MAX_NCELL_SCH_OFFSET)
            {
             frames_reqd = L1_SC_DURATION_SB + 1;
            }
            else
            {
             frames_reqd = L1_SC_DURATION_SB;
            }
            l1_fm_set_required_frame_number( FM_PRI_NCELL_SCH, l1_sc_acq_data_ptr->sb.start_fn, frames_reqd, gas_id);
         }
         break;

      case SB_ACTIVE:
         /* We take care of this state in l1_sci_post_tick_sb() */
         break;

      case SB_RAN_OUT_OF_FRAMES:
      case SB_ABORTING:
         /* Make sure we don't go to sleep */
         gl1_drx_require_next_tick(gas_id);
         break;

      case SB_NULL:
         /* Should never happen, but makes lint happy */
         break;

   }
}

void l1_sci_post_tick_sb( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_INT();

   if (l1_sc_acq_data_ptr->sb.state == SB_ACTIVE)
   {
         gl1_drx_require_next_tick(gas_id);
   }
}

/*
 *     Function name :  l1_sci_abort_sb
 *     --------------------------------
 *
 *     Description :
 *     ---------------
 *     Abort an SB receive.  Call with interrupts locked.
 */
void l1_sci_abort_sb( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_INT();

   switch (l1_sc_acq_data_ptr->sb.state)
   {
      case SB_INIT:
         l1_sc_acq_data_ptr->sb.state = SB_NULL;

         /* msg layer is not active - no need to abort */
         l1_sc_abort_receive_callback(gas_id);
      break;

      case SB_WAIT:
         l1_sc_acq_data_ptr->sb.state = SB_NULL;

         /* msg layer is not active - no need to abort */
         l1_sc_abort_receive_callback(gas_id);
         l1_fm_clear_required_frames(FM_PRI_NCELL_SCH, gas_id);
         break;

      case SB_ACTIVE:
         l1_sc_acq_data_ptr->sb.state = SB_ABORTING;

         /* need to abort msg layer */
         gl1_msg_abort_rx_ncell_sch(DELAYED_ABORT, gas_id);
         break;

      case SB_RAN_OUT_OF_FRAMES:
         l1_sc_acq_data_ptr->sb.state = SB_ABORTING;
         break;

      case SB_ABORTING:
         /* do nothing */
         MSG_GERAN_ERROR_0_G("Multiple aborts.");
         break;

      case SB_NULL:
      default:
         l1_sc_acq_data_ptr->sb.state = SB_NULL;

         /* this should never happen */
         l1_sc_abort_receive_callback(gas_id);

         l1_fm_clear_required_frames(FM_PRI_NCELL_SCH, gas_id);

         MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Unexpected state %d",
                    l1_sc_acq_data_ptr->sb.state);
         break;
   }

   gl1_drx_require_next_tick(gas_id);
}

/* call with interrupts locked */
void l1_sc_start_sb_tch( cell_T* cell, gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   l1_sc_acq_data_ptr->sb.start_qbit        = cell->bs.qbit_lag;

   l1_sc_acq_data_ptr->sb.arfcn             = cell->bs.arfcn;
   l1_sc_acq_data_ptr->sb.agc               = &cell->bs.agc;
#ifdef FEATURE_GSM_RX_DIVERSITY
   l1_sc_acq_data_ptr->sb.agc_divrx         = &cell->bs.agc_divrx;
#endif
   /* Clear the flags so we know when both callbacks have been called */
   l1_sc_acq_data_ptr->sb.data_done         = FALSE;
   l1_sc_acq_data_ptr->sb.metrics_done      = FALSE;

   l1_sc_acq_data_ptr->sb.state             = SB_INIT;

   /* Setup the afc info ptr */
   l1_sc_acq_data_ptr->sb.afc_info_ptr = &( cell->afc_info );

   /* @@WARNING: Do not reset the afc info validity flag
    * here !
    */
}

void l1_sci_tick_sb_tch( gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

   if (l1_sc_acq_data_ptr->sb.state == SB_INIT)
   {
      int16 start_qbit;

      int32 freq_error     = 0;

      l1_sc_acq_data_ptr->sb.rx_cb.data_cb     = ncell_sch_tch_data_callback;
      l1_sc_acq_data_ptr->sb.rx_rpt.data       = &l1_sc_acq_data_ptr->sb.decode_rpt;
      l1_sc_acq_data_ptr->sb.rx_rpt.metrics[0] = &l1_sc_acq_data_ptr->sb.metrics_rpt;
      l1_sc_acq_data_ptr->sb.rx_cb.metrics_cb  = ncell_sch_tch_metrics_callback;
      l1_sc_acq_data_ptr->sb.rx_cb.log_cb      = NULL;
      l1_sc_acq_data_ptr->sb.rx_cb.abort_cb    = ncell_sch_tch_abort_callback;

      gl1_msg_cfg_ncell_sch( &l1_sc_acq_data_ptr->sb.rx_cb,gas_id );

      l1_sc_acq_data_ptr->sb.start_fn          = l1_scdrv_get_fn( gas_id );

      /*
       Add 1 since we're calling it a frame
       earlier than original code. This is called from the ISR
       since start_fn needs to be accurate
       */
      l1_sc_acq_data_ptr->sb.start_fn          = ADD_FN( l1_sc_acq_data_ptr->sb.start_fn, 1 );

      /*
       Add 5000 since the SCH handler requires offsets to be
       specified relative to "idle - 1"
      */
      start_qbit           = l1_sc_acq_data_ptr->sb.start_qbit;
      start_qbit          += QS_PER_FRAME;

      /* Apply the afc freq from the fcch attempt if found to
       * be valid.
       */
      if(  ( NULL != l1_sc_acq_data_ptr->sb.afc_info_ptr )
         &&( FALSE != l1_sc_acq_data_ptr->sb.afc_info_ptr->valid )
        )
      {
        freq_error = (int32)l1_sc_acq_data_ptr->sb.afc_info_ptr->freq_error;

        MSG_GERAN_MED_1_G( GL1_ACQ_HDR"TCH Ncell SB apply freq_error delta %dHz",
                 freq_error);
      }

      gl1_msg_rx_ncell_sch ( l1_sc_acq_data_ptr->sb.arfcn,
                             l1_sc_acq_data_ptr->sb.agc,
#ifdef FEATURE_GSM_RX_DIVERSITY
                             l1_sc_acq_data_ptr->sb.agc_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                             (uint16)start_qbit,
                             freq_error,
                             &l1_sc_acq_data_ptr->sb.rx_rpt,gas_id );

     /* Don;t worry about restoring the freq base.
      * because the firmware is supposed to do that in the
      * async rx task.
      */

      gl1_set_ncell_block_start(TRUE,gas_id);

      l1_sc_acq_data_ptr->sb.state = SB_ACTIVE;
   }
   else
   {
      MSG_GERAN_ERROR_1_G("Unexpected call to l1_sci_tick_sb_tch %d ", l1_sc_acq_data_ptr->sb.state);
   }
}


/* call with interrupts locked */
void l1_sci_abort_sb_tch( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_INT();

   switch (l1_sc_acq_data_ptr->sb.state)
   {
      case SB_INIT:
         l1_sc_acq_data_ptr->sb.state = SB_NULL;

         /* msg layer is inactive so we can abort immediately */
         l1_sc_abort_receive_callback(gas_id);
         break;

      case SB_ACTIVE:
         l1_sc_acq_data_ptr->sb.state = SB_ABORTING;

         /* need to abort the msg layer */
         gl1_msg_abort_rx_ncell_sch(DELAYED_ABORT,gas_id);
         break;

      case SB_ABORTING:
         /* do nothing */
         MSG_GERAN_ERROR_0_G("Multiple aborts.");
         break;

      case SB_NULL:
      case SB_WAIT:
      case SB_RAN_OUT_OF_FRAMES:
      default:
         l1_sc_acq_data_ptr->sb.state = SB_NULL;

         /* this should never happen */
         l1_sc_abort_receive_callback(gas_id);
         MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Unexpected state %d",
                    l1_sc_acq_data_ptr->sb.state);
         break;
   }

}


/*
 *     Function name :  sb_aborted
 *     ---------------------------
 *
 *     Description :
 *     ---------------
 *     Callback to be called when an SB search has to be aborted because
 *     layer1 needs frames that were previously available to the surround
 *     cell engine.
 *
 *     Parameters :
 *     ---------------
 *     IN - dummy - Not used.
 *
 */
static void sb_aborted( void* dummy, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   NOTUSED( dummy );
   ASSERT_INT();

   if(l1_sc_acq_data_ptr->sb.state != SB_ABORTING)
   {
      /* Abort the message layer only if a Ncell SCH is in progress */
      if(l1_sc_acq_data_ptr->ncell_sch_in_progress)
      {
         l1_sc_acq_data_ptr->sb.state = SB_RAN_OUT_OF_FRAMES;
         gl1_msg_abort_rx_ncell_sch(DELAYED_ABORT, gas_id);
      }
   }
   else
   {
      MSG_GERAN_MED_0_G( GL1_ACQ_HDR"Ncell SCH Abort already in progress!");
   }

}

static void ncell_sch_abort_callback(gas_id_t gas_id)
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
#ifdef FEATURE_GPRS_GBTA
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_GPRS_GBTA */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* If aborting the ncell SCH allow SUSPENSION_FRAME_DELAY frames before disabling the FW */
   if ( gl1_msg_get_multi_sim_mode() )
   {
     l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                    FM_PRI_NCELL_SCH, gas_id );
   }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   l1_fm_clear_required_frames(FM_PRI_NCELL_SCH, gas_id);

#ifdef FEATURE_GPRS_GBTA
   /* After SCH decode , reset back to GBTA_ALLOWED */
   if (IS_GBTA_ACTIVE())
   {
     gpl1_gbta_reset_active_state(gas_id);

     /* as we never got the lock but if it gets extended then usefull to know if that was gbta extension */
     grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);
   }
#endif /* FEATURE_GPRS_GBTA */


   /* Work out why we aborted */
   if ( l1_sc_acq_data_ptr->sb.state == SB_RAN_OUT_OF_FRAMES )
   {
      /* We just ran out of frames   */

      /* Prepare to try agian. */
      if ( ++l1_sc_acq_data_ptr->sb.tries == L1_SC_SB_MAX_TRIES )
      {
         /* Time to give up */
         l1_sc_acq_data_ptr->sb.state  = SB_NULL;
         l1_sc_acq_data_ptr->sb.result = L1_SC_BLOCKED;
         l1_sci_receive_done(gas_id);
         MSG_GERAN_HIGH_2_G( GL1_ACQ_HDR"SCH (ARFCN=%d) abandoned (%d tries)",
                   l1_sc_acq_data_ptr->sb.arfcn.num, L1_SC_SB_MAX_TRIES);
      }
      else
      {
         l1_sc_acq_data_ptr->sb.start_fn = next_sb_fn( gl1_get_FN( gas_id ), gas_id );

         if(!gl1_msg_get_multi_sim_mode())
         {
           l1_sc_acq_data_ptr->sb.drx_ref  = gl1_drx_require_tick( l1_sc_acq_data_ptr->sb.start_fn , gas_id);
         }

         l1_fm_set_required_frame_number( FM_PRI_NCELL_SCH, l1_sc_acq_data_ptr->sb.start_fn, L1_SC_DURATION_SB, gas_id);
         l1_sc_acq_data_ptr->sb.state    = SB_WAIT;
      }
   }
   else if (l1_sc_acq_data_ptr->sb.state == SB_ABORTING)
   {
      /* All SC receives are being aborted */
      /* - let the SCE know we're done     */
      l1_sc_abort_receive_callback(gas_id);
      l1_sc_acq_data_ptr->sb.state = SB_NULL;
   }
   else
   {
      MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Unexpected abort callback state %d",
                 l1_sc_acq_data_ptr->sb.state);

      l1_sc_abort_receive_callback(gas_id);
      l1_sc_acq_data_ptr->sb.state = SB_NULL;
   }

   l1_sc_acq_data_ptr->ncell_sch_in_progress = FALSE;

}

/*
 *     Function name :  l1_sc_read_sb
 *     -------------------------------
 *
 *     Description :
 *     ---------------
 *     Read results of an SB search.
 *     Must only be called after a call to l1_sci_tick_sb() returned TRUE.
 *
 *     Parameters :
 *     ---------------
 *     OUT - sb_data      - Data of the received SB.
 *                          Only set if this function returns TRUE.
 *     OUT - start_fn     - Frame number where SB receive was requested
 *                          or one more, if start_qbit passed by
 *                          l1_sci_sb_cnf() was greater than 4999.
 *                          Only set if this function returns TRUE.
 *     OUT - timing_error - The quarter bit timing error of the burst. Positive
 *                          means the burst has been received later than expected.
 *                          Only set if this function returns TRUE.
 *
 *     Returns :
 *     -----------
 *     L1_SC_DECODE_SUCCESS when the SB CRC passes.
 *     L1_SC_DECODE_FAILURE when the SB CRC fails.
 *     L1_SC_BLOCKED when the SB cannot be received because
 *     no frames are available.
 *
 */
l1_sc_rcv_result_T l1_sc_read_sb( uint8 sb_data[4], uint32* start_fn, int16* timing_error, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_TASK();

   if ( l1_sc_acq_data_ptr->sb.result == L1_SC_DECODE_SUCCESS )
   {
      sb_data[0]  = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[0];
      sb_data[1]  = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[1];
      sb_data[2]  = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[2];
      sb_data[3]  = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[3];
      if ( l1_sc_acq_data_ptr->sb.start_qbit >= 5000 )
      {
         *start_fn = ADD_FN( l1_sc_acq_data_ptr->sb.start_fn, 1 );
      }
      else
      {
         *start_fn = l1_sc_acq_data_ptr->sb.start_fn;
      }
      *timing_error = l1_sc_acq_data_ptr->sb.metrics_rpt.timing_offset;
   }

   return l1_sc_acq_data_ptr->sb.result;
}

l1_sc_rcv_result_T l1_sc_read_sb_tch( byte sb_data[4], dword* start_fn, sword* timing_error, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_TASK();

   if ( l1_sc_acq_data_ptr->sb.result == L1_SC_DECODE_SUCCESS )
   {
      sb_data[0] = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[0];
      sb_data[1] = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[1];
      sb_data[2] = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[2];
      sb_data[3] = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[3];
      *start_fn = l1_sc_acq_data_ptr->sb.start_fn;

      *timing_error = (int16)(l1_sc_acq_data_ptr->sb.metrics_rpt.timing_offset - l1_sc_acq_data_ptr->last_sch_offset_delta);
   }
   return l1_sc_acq_data_ptr->sb.result;
}



l1_sc_rcv_result_T l1_sc_decide_about_sb_tch(uint8 try_no,  gas_id_t gas_id)
{
    l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

    if(l1_sc_acq_data_ptr->sb.result == L1_SC_DECODE_SUCCESS )
    {
        if(try_no == 2)
        {
            if( l1_sc_acq_data_ptr->sb.metrics_rpt.pwr_dBm_x16 > (-90 * 16) && l1_sc_acq_data_ptr->sb.metrics_rpt.snr < 1019)
            {
                l1_sc_acq_data_ptr->sb.result = L1_SC_DECODE_FAILURE;
                MSG_GERAN_HIGH_2_G("Ignoring the decoded SCH power is %i SNR is %d",l1_sc_acq_data_ptr->sb.metrics_rpt.pwr_dBm_x16,l1_sc_acq_data_ptr->sb.metrics_rpt.snr);
                return L1_SC_DECODE_FAILURE;
            }
            else
            {
                MSG_GERAN_HIGH_0_G("Accepting the decoding the SCH");
                return L1_SC_DECODE_SUCCESS;
            }
        }
    }

    return (l1_sc_acq_data_ptr->sb.result);

}

#ifdef FEATURE_GSM_SENS_SUPPORT
/*===========================================================================

FUNCTION  l1_sc_run_SENS

DESCRIPTION
  This function Runs SENS algorithm on standalone SCH results and update SB result based 
  on GDET score
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_sc_run_SENS( l1_sc_acq_data_t *l1_sc_acq_data_ptr, gas_id_t gas_id )
{

    if( l1_sc_acq_data_ptr->sb.metrics_rpt.pwr_dBm_x16 > NCELL_SCH_TCH_RSSI_THRESH)
    {
      uint32 decstat_avg;
      uint16 decstat1, decstat2;
    
      decstat1 = l1_sc_acq_data_ptr->sb.metrics_rpt.gfwGdetResults.decStat1; 
      decstat2 = l1_sc_acq_data_ptr->sb.metrics_rpt.gfwGdetResults.decStat2; 
      decstat_avg = (decstat1 + decstat2)/2;
    
      if( decstat_avg  < NCELL_SCH_TCH_GDET_THRESH)
      {
        l1_sc_acq_data_ptr->sb.result = L1_SC_DEC_FAIL_WITH_GOOD_GDET;
        MSG_GERAN_MED_4_G(GL1_ACQ_HDR"SCH Decode failed with good GDET, pwr: %d, decstat1: %d, decstat2: %d, avg: %d", l1_sc_acq_data_ptr->sb.metrics_rpt.pwr_dBm_x16, decstat1, decstat2, decstat_avg);
      }
      else
      {
        MSG_GERAN_MED_4_G(GL1_ACQ_HDR"SCH Decode failed with poor GDET, pwr: %d, decstat1: %d, decstat2: %d, avg: %d", l1_sc_acq_data_ptr->sb.metrics_rpt.pwr_dBm_x16, decstat1, decstat2, decstat_avg);
      }
    }
    else
    {
      MSG_GERAN_MED_1_G(GL1_ACQ_HDR"SCH Decode failed with low pwr: %d", l1_sc_acq_data_ptr->sb.metrics_rpt.pwr_dBm_x16);
    }

}
#endif
static void ncell_sch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
#ifdef FEATURE_GPRS_GBTA
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_GPRS_GBTA */

   ASSERT_INT();

   /* Log */
   l1_log_ncell_sch_msg_metrics(&rpt->sch, gas_id);
   l1_fm_clear_required_frames(FM_PRI_NCELL_SCH, gas_id);

   l1_sc_acq_data_ptr->sb.data_done = TRUE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif


   if ( rpt->sch.hdr.good_data )
   {
      l1_sc_acq_data_ptr->sb.result = L1_SC_DECODE_SUCCESS;
   }
   else
   {
#ifdef FEATURE_GPRS_GBTA
     MSG_GERAN_MED_1_G("GBTA sch_data_callback scheduled_status (1 << 16) %d ",rpt->sch.hdr.scheduled_status);
#endif


#ifdef FEATURE_GPRS_GBTA
      if (rpt->sch.hdr.scheduled_status == GPL1_SCH_SCHDULE_FAIL)
      {
         l1_sc_acq_data_ptr->sb.result = L1_SC_SCHED_FAIL;
      }
      else
#endif /* FEATURE_GPRS_GBTA */
      {
       l1_sc_acq_data_ptr->sb.result = L1_SC_DECODE_FAILURE;
       
#ifdef FEATURE_GSM_SENS_SUPPORT
        if( (L1_DEDICATED_MODE == l1_tsk_buffer[gas_id].l1_state) && (TRUE == gl1_hw_get_sens_fr_ctrl()))
        {
          l1_sc_run_SENS(l1_sc_acq_data_ptr, gas_id);
        }
#endif
      }
   }

   if (l1_sc_acq_data_ptr->sb.metrics_done)
   {
      /* Since we call l1_sci_receive_done() state
       * actually needs to be set to SB_NULL.
       */
      l1_sc_acq_data_ptr->sb.state = SB_NULL;
      l1_sci_receive_done(gas_id);
   }


#ifdef FEATURE_GPRS_GBTA
   /* After SCH decode , reset back to GBTA_ALLOWED */
   if (IS_GBTA_ACTIVE())
   {
     gpl1_gbta_reset_active_state(gas_id);
     /* as we never got the lock but if it gets extended then usefull to know if that was gbta extension */
     grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);
   }
#endif /* FEATURE_GPRS_GBTA */


   /* The rest of this is just debug code and
    * diagnostic messages.
    */
   if ( rpt->sch.hdr.good_data )
   {
      uint8 BSIC;

      /* Update the rotator value stored for this NCELL */
      if( NULL != l1_sc_acq_data_ptr->sb.afc_info_ptr )
      {
        l1_sc_acq_data_ptr->sb.afc_info_ptr->valid = TRUE;

        /* Snapshot the current XO + freq_error for later restoration during ncell bcch decode */
        l1_sc_acq_data_ptr->sb.afc_info_ptr->afc_freq =
          ( gl1_hw_freqtonormxofreq( l1_sc_acq_data_ptr->sb.arfcn,
                                     l1_sc_acq_data_ptr->sb.afc_info_ptr->freq_error,
                                     gas_id ) +
            gl1_hw_get_xo_acc_freq_err(gas_id));
      }
      else
      {
        MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"NULL l1_sc_acq_data_ptr->sb.afc_info_ptr ");
      }

      BSIC = (l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[0] & 0xfc) >> 2;

      MSG_GERAN_MED_3_G( GL1_ACQ_HDR"Ncell SCH OK: ARFCN=%d NCC=%d BCC=%d",
               l1_sc_acq_data_ptr->sb.arfcn.num, (BSIC>>3), (BSIC & 0x07) );

      /* avoid compiler and lint warnings about unused variables when MSG
         is defined away to nothing */
      (void)BSIC;

      MSG_GERAN_MED_1_G( GL1_ACQ_HDR"Ncell SCH OK at FN: %d",
               gl1_get_FN( gas_id ));
   }
   else
   {
      MSG_GERAN_MED_2_G( GL1_ACQ_HDR"Ncell SCH Failed (ARFCN=%d) at FN:%d...",
               l1_sc_acq_data_ptr->sb.arfcn.num, gl1_get_FN( gas_id ));
   }

   l1_sc_acq_data_ptr->ncell_sch_in_progress = FALSE;


}

static void ncell_sch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   /* Log - only a single burst */
   l1_log_ncell_sch_burst_metrics(rpt[0], gas_id);

   l1_sc_acq_data_ptr->sb.metrics_done = TRUE;
   if (l1_sc_acq_data_ptr->sb.data_done)
   {
      /* Since we call l1_sci_receive_done() state
       * actually needs to be set to SN_NULL.
       */
      l1_sc_acq_data_ptr->sb.state = SB_NULL;

      l1_sci_receive_done(gas_id);
   }
}

static void ncell_sch_tch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];

   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

   ASSERT_INT();

   /* Log */
   l1_log_ncell_sch_msg_metrics(&rpt->sch, gas_id);

   if ( rpt->sch.hdr.good_data )
   {
      l1_sc_acq_data_ptr->sb.result = L1_SC_DECODE_SUCCESS;
   }
   else
   {
      l1_sc_acq_data_ptr->sb.result = L1_SC_DECODE_FAILURE;
#ifdef FEATURE_GSM_SENS_SUPPORT
      if(TRUE == gl1_hw_get_sens_fr_ctrl())
      {
          l1_sc_run_SENS(l1_sc_acq_data_ptr, gas_id);
      }
#endif
   }

   l1_sc_acq_data_ptr->sb.data_done = TRUE;

   if (l1_sc_acq_data_ptr->sb.metrics_done)
   {
      l1_sc_acq_data_ptr->sb.state = SB_NULL;
      l1_sci_receive_done(gas_id);
   }

   /* The rest of this is just debug code and
    * diagnostic messages.
    */
   if ( rpt->sch.hdr.good_data )
   {
      uint8 BSIC;

      BSIC = (l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[0] & 0xfc) >> 2;

      MSG_GERAN_MED_3_G( GL1_ACQ_HDR"Ncell TCH SCH OK: ARFCN=%d NCC=%d BCC=%d",
               l1_sc_acq_data_ptr->sb.arfcn.num, (BSIC>>3), (BSIC & 0x07) );

      /* avoid compiler and lint warnings about unused variables when MSG
         is defined away to nothing */
      (void)BSIC;

#ifdef DED_SC_SCH_DECODE_DEBUG
      #error code not present
#endif

      /* Do not change the stored afc_freq since it seems good eough
       * to guarantee a successful decode.
       */
   }
   else
   {

#ifdef DED_SC_SCH_DECODE_DEBUG
      #error code not present
#endif

      /* Set the stored afc freq to invalid */
      if( NULL != l1_sc_acq_data_ptr->sb.afc_info_ptr )
      {
        l1_sc_acq_data_ptr->sb.afc_info_ptr->valid = FALSE;
      }

      MSG_GERAN_MED_1_G( GL1_ACQ_HDR"Ncell TCH SCH Failed (ARFCN=%d)...",
               l1_sc_acq_data_ptr->sb.arfcn.num);
   }

	/* inform MCPM that PTM/DED Ncell scenario is now over (FR54762)*/
    gl1_concman_config_params.type = CMGR_TRANSIENT_END;
    gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
    gl1_concman_config_params.update_scenario = 0;
    gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
    gl1_concman_config_params.start_timestamp = 0;
    gl1_concman_config_params.duration_msec = 0;
    gl1_concman_config_params.flow_ctrl_time_msec = 0;
    gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
    MSG_GERAN_HIGH_0_G("***sending scenario complete to MCPM***");

}

static void ncell_sch_tch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   /* Log - only a single burst */
   l1_log_ncell_sch_burst_metrics(rpt[0], gas_id);

   l1_sc_acq_data_ptr->sb.metrics_done = TRUE;
   if (l1_sc_acq_data_ptr->sb.data_done)
   {
      l1_sc_acq_data_ptr->sb.state = SB_NULL;
      l1_sci_receive_done(gas_id);
   }
}


static void ncell_sch_tch_abort_callback( gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
  gl1_concman_config_params_type gl1_concman_config_params;
  cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);
   if (l1_sc_acq_data_ptr->sb.state != SB_ABORTING)
   {
      MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Unexpected sb_tch abort callback. %d",
                 l1_sc_acq_data_ptr->sb.state);
   }

   l1_sc_acq_data_ptr->sb.state = SB_NULL;
   l1_sc_abort_receive_callback(gas_id);

	/* inform MCPM that PTM/DED Ncell scenario is now over (FR54762)*/
     gl1_concman_config_params.type = CMGR_TRANSIENT_END;
     gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
     gl1_concman_config_params.update_scenario = 0;
     gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
     gl1_concman_config_params.start_timestamp = 0;
     gl1_concman_config_params.duration_msec = 0;
     gl1_concman_config_params.flow_ctrl_time_msec = 0;
     gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
     MSG_GERAN_HIGH_0_G("***sending scenario complete to MCPM***");

}

/*
 *     Function name :  next_sb_fn
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Calculates the serving cell frame number corresponding to the next SCH
 *     burst on a neighbor cell.  This functions uses values stored in the
 *     sb struct, specifically: fn_lag and mask.  These must be set to values
 *     corresponding to the neighbor cell in question.
 *
 *     Parameters :
 *     ---------------
 *     IN  - serv_fn      - The current serving cell frame number.
 *
 *     Returns :
 *     -----------
 *     The number of the serving cell frame on which to start the SCH reception.
 *
 */

static uint32 next_sb_fn( uint32 serv_fn, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint32 fn;
   uint16 fn_mod_51;

   /* for idle mode update qbit drift */
   if( l1_sc_globals_ptr->mode == L1SCModeIdle )
   {
     if( l1_sc_apply_timing_drift(gas_id) )
     {
       l1_sc_acq_data_ptr->sb.fn_lag = l1_sc_globals_ptr->receiving_cell->bs.fn_lag;
       l1_sc_acq_data_ptr->sb.start_qbit = l1_sc_globals_ptr->receiving_cell->bs.qbit_lag;
       MSG_GERAN_HIGH_3_G("SCH arfcn %d updated fn_lag %d qbit_lag %d",l1_sc_acq_data_ptr->sb.arfcn.num,l1_sc_acq_data_ptr->sb.fn_lag,l1_sc_acq_data_ptr->sb.start_qbit);
     }
   }

   /* +2 is included since we need to start receive two
    * frames in advance for GPRS.
    */
   if(l1_sc_globals_ptr->mode == L1SCModeGprsIdle)
   {
     fn = FRAMES_PER_HYPERFRAME + serv_fn - l1_sc_acq_data_ptr->sb.fn_lag + 2;
   }
   else
   {
#ifdef ZIF_TUNE_TIME_FIXED
      #error code not present
#else
      /*
         Wake up 2 frames before SCH burst.  Workaround for
         SBI errors due to large ZIF tune time.  Will be
         pulled out after ZIF tune time is reduced
      */
      /* Uncomment once ZIF tune time is reduced */
      fn = FRAMES_PER_HYPERFRAME + serv_fn - l1_sc_acq_data_ptr->sb.fn_lag + 2;
#endif
   }

   fn_mod_51 = fn%51;

   do
   {
      if ( ++fn_mod_51 == 51 )
      {
         fn_mod_51 = 0;
      }

      if ( ++serv_fn == FRAMES_PER_HYPERFRAME )
      {
         serv_fn = 0;
      }
   }
   while ( ( sb_map[ fn_mod_51 ] & l1_sc_acq_data_ptr->sb.mask ) == 0 );
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif
   return serv_fn;
}

boolean l1_sci_is_fb_in_progress( gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   return (l1_sc_acq_data_ptr->fcb.state != FB_NULL);
}


/***********************************************************
* GPRS MONITOR FUNCTIONS BELOW
************************************************************/

static void gpl1_gprs_sci_fcb_trans_cnf( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

   ASSERT_INT();
   MSG_GERAN_LOW_0_G("**CALLBACK_FCB_TRANS**");
   //If tone was detected

   /* Sanity check the tone detection result */
   if( FALSE != rpt->fcch.tone_detected )
   {
     if(  ( rpt->fcch.tone_offset < 0 )
        &&( rpt->fcch.tone_offset < (-(2*QS_PER_FRAME)) )
       )
     {
       rpt->fcch.tone_detected = FALSE;

       MSG_GERAN_ERROR_1_G(" invalid PTM FCCH time offset %d ",rpt->fcch.tone_offset );
     }
     if(  ( rpt->fcch.tone_offset > 0 )
        &&( rpt->fcch.tone_offset > (2*QS_PER_FRAME) )
       )
     {
       rpt->fcch.tone_detected = FALSE;

       MSG_GERAN_ERROR_1_G(" invalid PTM FCCH time offset %d ",rpt->fcch.tone_offset );
     }
   }

   if (rpt->fcch.tone_detected)
   {
    uint16 start_window_offset = 0;

    start_window_offset = gl1_get_ncell_start_acq_offset(gas_id); /*Get the value of start acq offset of NCELL FCCH window*/

    if (rpt->fcch.tone_offset < 0)  /*If the detected tone offset is less than 0*/
    {

     if(rpt->fcch.tone_offset < -(4 * QS_PER_SLOT)) /*If the tone offset is less than -2500 Qs then ignore it*/
     {
       rpt->fcch.tone_detected = FALSE;
       MSG_GERAN_HIGH_3_G( GL1_ACQ_HDR"Ncell PTM FB SC Tone Ignored:start window off=%d Offset=%d less than -2500 Qs",
       start_window_offset, (int16)rpt->fcch.tone_offset, 0 );
     }
     /*If the starting offset of ncell fcch tone window is greater than the offset of detected tone then ignore it*/
     else if(start_window_offset > ((rpt->fcch.tone_offset) + 5000))
     {
       rpt->fcch.tone_detected = FALSE;
       MSG_GERAN_HIGH_3_G( GL1_ACQ_HDR"Ncell PTM FB SC Tone Ignored:start window off=%d Offset=%d less than 0",
       start_window_offset, (int16)rpt->fcch.tone_offset, 0 );
     }

    }

   }

   if (rpt->fcch.tone_detected)
   {
      l1_sc_acq_data_ptr->fcb.success = TRUE;

      /* Store qbit lag for later reference */
      l1_sc_acq_data_ptr->fcb.start_qbit     = (int16)rpt->fcch.tone_offset;
      MSG_GERAN_HIGH_2_G( GL1_ACQ_HDR"Ncell PTM SC Tone Ok:ARFCN=%d Offset=%d",
                l1_sc_acq_data_ptr->fcb.arfcn.num, (int16)rpt->fcch.tone_offset);

      /* Save the frequency offset information for later use in the
       * SCH decoding in xfer.
       */
      if( NULL != l1_sc_acq_data_ptr->fcb.afc_info_ptr )
      {
        /* Pick up from the fcch in norm XO/rotator units */
        l1_sc_acq_data_ptr->fcb.afc_info_ptr->afc_freq   = (int32)rpt->fcch.afc_freq;

        l1_sc_acq_data_ptr->fcb.afc_info_ptr->freq_error = gl1_hw_normxofreqtofreq( l1_sc_acq_data_ptr->fcb.arfcn,
                     ((int32)rpt->fcch.afc_freq - gl1_hw_get_xo_acc_freq_err(gas_id)), gas_id );

        l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid      = TRUE;

        MSG_GERAN_HIGH_2_G( GL1_ACQ_HDR"Ncell PTM FB afc freq %dXO and ff %dHz retained for acq use",
                 l1_sc_acq_data_ptr->fcb.afc_info_ptr->afc_freq, l1_sc_acq_data_ptr->fcb.afc_info_ptr->freq_error);
      }
      else
      {
        MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"NULL afc_info_ptr");
      }

   } //if tone not detected
   else
   {
      l1_sc_acq_data_ptr->fcb.success = FALSE;

      MSG_GERAN_LOW_1_G( GL1_ACQ_HDR"SC Tone failed: ARFCN=%d",
               l1_sc_acq_data_ptr->fcb.arfcn.num);

      if( NULL != l1_sc_acq_data_ptr->fcb.afc_info_ptr )
      {
        /* Reset the afc validity flag */
        l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid = FALSE;
      }

   }
   l1_sc_acq_data_ptr->fcb.state = FB_NULL;
   l1_sci_receive_done(gas_id);

	/* inform MCPM that PTM/DED Ncell scenario is now over (FR54762)*/
    gl1_concman_config_params.type = CMGR_TRANSIENT_END;
    gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
    gl1_concman_config_params.update_scenario = 0;
    gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
    gl1_concman_config_params.start_timestamp = 0;
    gl1_concman_config_params.duration_msec = 0;
    gl1_concman_config_params.flow_ctrl_time_msec = 0;
    gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
    MSG_GERAN_HIGH_0_G("***sending scenario complete to MCPM***");

}


static void gpl1_gprs_ncell_sch_trans_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);
   ASSERT_INT();

   /* Log */
   l1_log_ncell_sch_msg_metrics(&rpt->sch, gas_id);

   if ( rpt->sch.hdr.good_data )
   {
      l1_sc_acq_data_ptr->sb.result = L1_SC_DECODE_SUCCESS;
   }
   else
   {
      l1_sc_acq_data_ptr->sb.result = L1_SC_DECODE_FAILURE;
   }

   MSG_GERAN_LOW_0_G("***CALLBACK_SB_TRANS***");
   l1_sc_acq_data_ptr->sb.data_done = TRUE;
   if (l1_sc_acq_data_ptr->sb.metrics_done)
   {
      l1_sc_acq_data_ptr->sb.state = SB_NULL;
      l1_sci_receive_done(gas_id);
   }

   /* The rest of this is just debug code and
    * diagnostic messages.
    */
   if ( rpt->sch.hdr.good_data )
   {
      MSG_GERAN_HIGH_1_G( GL1_ACQ_HDR"Ncell PTM SCH OK: (ARFCN=%d)...",
                l1_sc_acq_data_ptr->sb.arfcn.num);
   }
   else
   {
      /* Set the stored afc freq to invalid */
      if( NULL != l1_sc_acq_data_ptr->sb.afc_info_ptr )
      {
        l1_sc_acq_data_ptr->sb.afc_info_ptr->valid = FALSE;
      }

      MSG_GERAN_MED_1_G( GL1_ACQ_HDR"Ncell PTM SCH Failed (ARFCN=%d)...",
               l1_sc_acq_data_ptr->sb.arfcn.num);
   }

	/* inform MCPM that PTM/DED Ncell scenario is now over (FR54762)*/
    gl1_concman_config_params.type = CMGR_TRANSIENT_END;
    gl1_concman_config_params.scenario = CMGR_GERAN_NCELL_ACQ_GAP;
    gl1_concman_config_params.update_scenario = 0;
    gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
    gl1_concman_config_params.start_timestamp = 0;
    gl1_concman_config_params.duration_msec = 0;
    gl1_concman_config_params.flow_ctrl_time_msec = 0;
    gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
    MSG_GERAN_HIGH_0_G("***sending scenario complete to MCPM***");

}

/* call with interrupts locked */
void gpl1_gprs_sc_start_fcb_trans( cell_T* cell, gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   l1_sc_acq_data_ptr->fcb.arfcn            = cell->bs.arfcn;
   l1_sc_acq_data_ptr->fcb.agc              = &cell->bs.agc;

   l1_sc_acq_data_ptr->fcb.rx_cb.data_cb    = gpl1_gprs_sci_fcb_trans_cnf;
   l1_sc_acq_data_ptr->fcb.rx_rpt.data      = &l1_sc_acq_data_ptr->fcb.decode_rpt;
   l1_sc_acq_data_ptr->fcb.rx_cb.metrics_cb = NULL;
   l1_sc_acq_data_ptr->fcb.rx_cb.log_cb     = NULL;
   l1_sc_acq_data_ptr->fcb.rx_cb.abort_cb   = ncell_fcch_tch_abort_callback;
   l1_sc_acq_data_ptr->fcb.state            = FB_INIT;

    /* Get a pointer to the cells freq correction
     * structure.
     */
    l1_sc_acq_data_ptr->fcb.afc_info_ptr        = &( cell->afc_info );
    l1_sc_acq_data_ptr->fcb.afc_info_ptr->valid = FALSE;

   /*
   Message layer functions invoked in the task.
   The handler is ticked from within l1_tch in the ISR
   */
   MSG_GERAN_LOW_1_G("**RECEIVE_FCB_TRANS %d ARFCN**",l1_sc_acq_data_ptr->fcb.arfcn.num);

   gl1_msg_cfg_ncell_fcch( &l1_sc_acq_data_ptr->fcb.rx_cb,gas_id );
}

/* call with interrupts locked */
void gpl1_gprs_sc_start_sb_trans( cell_T* cell, gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   int16 start_qbit;

   /* Don't change these lines without checking
    * that result will always be =ve.  The value
    * passed to the msg layer must be +ve.
    */
   start_qbit    = cell->bs.qbit_lag;
   start_qbit   += QS_PER_FRAME;
   l1_sc_acq_data_ptr->sb.start_qbit = start_qbit;

   l1_sc_acq_data_ptr->sb.arfcn      = cell->bs.arfcn;

   l1_sc_acq_data_ptr->sb.agc        = &cell->bs.agc;
#ifdef FEATURE_GSM_RX_DIVERSITY
   l1_sc_acq_data_ptr->sb.agc_divrx  = &cell->bs.agc_divrx;
#endif
   /*
    Add 5000 since the SCH handler requires offsets to be
    specified relative to "idle - 1"
   */

   /* Clear the flags so we know when both callbacks have been called */
   l1_sc_acq_data_ptr->sb.data_done      = FALSE;
   l1_sc_acq_data_ptr->sb.metrics_done   = FALSE;


   l1_sc_acq_data_ptr->sb.rx_cb.data_cb     = gpl1_gprs_ncell_sch_trans_data_callback;
   l1_sc_acq_data_ptr->sb.rx_rpt.data       = &l1_sc_acq_data_ptr->sb.decode_rpt;
   l1_sc_acq_data_ptr->sb.rx_rpt.metrics[0] = &l1_sc_acq_data_ptr->sb.metrics_rpt;
   l1_sc_acq_data_ptr->sb.rx_cb.metrics_cb  = ncell_sch_trans_metrics_callback;
   l1_sc_acq_data_ptr->sb.rx_cb.log_cb      = NULL;
   l1_sc_acq_data_ptr->sb.rx_cb.abort_cb    = ncell_sch_tch_abort_callback;

   l1_sc_acq_data_ptr->sb.state             = SB_INIT;

   /* Setup the afc info ptr */
   l1_sc_acq_data_ptr->sb.afc_info_ptr = &( cell->afc_info );

   /* @@WARNING: Do not reset the afc info validity flag
    * here !
    */

   /*
    Message layer functions invoked in the task.
    The handler is ticked from within l1_tch in the ISR
   */
   MSG_GERAN_LOW_3_G( GL1_ACQ_HDR"SB_TRANS %d %d %d",
            l1_sc_acq_data_ptr->sb.arfcn.num,
            l1_sc_acq_data_ptr->sb.start_qbit,
            ( cell->last_tone_fn % 52 ) );

   gl1_msg_cfg_ncell_sch( &l1_sc_acq_data_ptr->sb.rx_cb,gas_id );
}

static void ncell_sch_trans_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   /* Log - only a single burst */
   l1_log_ncell_sch_burst_metrics(rpt[0], gas_id);

   l1_sc_acq_data_ptr->sb.metrics_done = TRUE;
   if (l1_sc_acq_data_ptr->sb.data_done)
   {
      l1_sc_acq_data_ptr->sb.state = SB_NULL;
      l1_sci_receive_done(gas_id);
   }
}

void gpl1_gprs_sci_tick_fcb_trans( gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_INT();

   if (l1_sc_acq_data_ptr->fcb.state == FB_INIT)
   {
      /* Instantiate and initialise the lowest and highest timeslot
       * information structures.
       */
      const gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, /* relative timeslot */
                                                    FALSE          /* transmit boolean */
                                                  };

      gl1_defs_tn_struct        highest_tn_info = { GL1_DEFS_TN_0, /* relative timeslot */
                                                    FALSE          /* transmit boolean */
                                                  };
      /* Absolute highest tx timeslot */
      uint32  l1_highest_tx_timeslot;

      /* Get the absolute highest rx */
      uint32  l1_highest_rx_timeslot;

      /* Initialise the highest relative rx or transmit timeslot
       * to the value of the current absolute lowest receive timeslot.
       */
      uint32  highest_rel_rxtx_ts = l1_get_current_timeslot(gas_id);

      /* Get the highest rx and tx timeslots */
      gpl1_pkt_xfer_get_highest_timeslots( &l1_highest_rx_timeslot,
                                           &l1_highest_tx_timeslot,
                                           gas_id );

      /* Find the highest occupied timeslot set by
       * previous block periods activities.
       */
      if( 0xFF != l1_highest_tx_timeslot )
      {
        /* The highest timeslot is a transmit timeslot.
         * Find the receive equivalent of the transmit timeslot
         */
        highest_rel_rxtx_ts = ADD_MOD( l1_highest_tx_timeslot,
                                       3,
                                       GL1_DEFS_SLOTS_IN_FRAME );

        /* Set the transmit flag to TRUE to allow for the timing advance
         * to be taken into account by NPL1 when sizing the FB window.
         */
        highest_tn_info.transmit = TRUE;
      }
      else if( 0xFF != l1_highest_rx_timeslot )
      {
        /* The highest timeslot is a receive timeslot */
        highest_rel_rxtx_ts = l1_highest_rx_timeslot;
      }

      /* Convert it to relative terms. */
      highest_rel_rxtx_ts = SUB_MOD( highest_rel_rxtx_ts,
                                     l1_get_current_timeslot(gas_id),
                                     GL1_DEFS_SLOTS_IN_FRAME );

      /* Fill the highest tn info structure */
      highest_tn_info.tn = (gl1_defs_tn_type) highest_rel_rxtx_ts;

      l1_sc_acq_data_ptr->fcb.start_fn = l1_scdrv_get_fn( gas_id );

      /*
        Add 1 since we're triggering SC from l1_tch two frame earlier
        than the original code. This is called from the ISR since we need
        start_fn to be accurate. Its used later to predict when to
        expect the SCH
       */
      l1_sc_acq_data_ptr->fcb.start_fn   = ADD_FN( l1_sc_acq_data_ptr->fcb.start_fn, 0 );

      gl1_msg_rx_ncell_fcch ( &( lowest_tn_info ),
                              &( highest_tn_info ),
                              l1_sc_acq_data_ptr->fcb.arfcn,
                              l1_sc_acq_data_ptr->fcb.agc,
                              &l1_sc_acq_data_ptr->fcb.rx_rpt,
                              gas_id );

      gl1_set_ncell_block_start(TRUE,gas_id);

      l1_sc_acq_data_ptr->fcb.state = FB_ACTIVE;
   }
   else
   {
      MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Unexpected call to gpl1_gprs_sci_tick_fcb_trans %d",
                 l1_sc_acq_data_ptr->fcb.state);
   }
}

void gpl1_gprs_sci_tick_sb_trans( gas_id_t gas_id )
{
  l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   if (l1_sc_acq_data_ptr->sb.state == SB_INIT)
   {

      /* local variable to hold the freq error info
       * Make sure it is initialised to zero.
       */
      int32 freq_error = 0;

      l1_sc_acq_data_ptr->sb.start_fn = l1_scdrv_get_fn( gas_id );

      l1_sc_acq_data_ptr->sb.start_fn = ADD_FN( l1_sc_acq_data_ptr->sb.start_fn, 0 );

      MSG_GERAN_LOW_2_G( GL1_ACQ_HDR"SB start fn %d start qbit %d",
               ( l1_sc_acq_data_ptr->sb.start_fn % 52 ),
               l1_sc_acq_data_ptr->sb.start_qbit);

      /* Apply the afc freq from the fcch attempt if found to
       * be valid.
       */
      if(  ( NULL != l1_sc_acq_data_ptr->sb.afc_info_ptr )
         &&( FALSE != l1_sc_acq_data_ptr->sb.afc_info_ptr->valid )
        )
      {
        freq_error = (int32)l1_sc_acq_data_ptr->sb.afc_info_ptr->freq_error;

        MSG_GERAN_MED_1_G( GL1_ACQ_HDR"PTM Ncell SB apply freq_error delta %dHz",
                 freq_error);
      }

      gl1_msg_rx_ncell_sch ( l1_sc_acq_data_ptr->sb.arfcn,
                             l1_sc_acq_data_ptr->sb.agc,
#ifdef FEATURE_GSM_RX_DIVERSITY
                             l1_sc_acq_data_ptr->sb.agc_divrx,
#endif
                             (uint16)l1_sc_acq_data_ptr->sb.start_qbit,
                             freq_error,
                             &l1_sc_acq_data_ptr->sb.rx_rpt,gas_id );

      gl1_set_ncell_block_start(TRUE,gas_id);

      l1_sc_acq_data_ptr->sb.state = SB_ACTIVE;
   }
   else
   {
      MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Unexpected call to gpl1_gprs_sci_tick_sb_trans %d",
                 l1_sc_acq_data_ptr->sb.state);
   }
}

l1_sc_rcv_result_T gpl1_gprs_sc_read_sb_trans( byte sb_data[4], dword* start_fn, sword* timing_error, gas_id_t gas_id )
{
   l1_sc_acq_data_t *l1_sc_acq_data_ptr = &l1_sc_acq_data[gas_id];
   ASSERT_TASK();

   if ( l1_sc_acq_data_ptr->sb.result == L1_SC_DECODE_SUCCESS )
   {
      sb_data[0] = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[0];
      sb_data[1] = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[1];
      sb_data[2] = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[2];
      sb_data[3] = l1_sc_acq_data_ptr->sb.decode_rpt.sch.data[3];
      *start_fn = l1_sc_acq_data_ptr->sb.start_fn;
      *timing_error = l1_sc_acq_data_ptr->sb.metrics_rpt.timing_offset;
   }
   return l1_sc_acq_data_ptr->sb.result;
}

/* 
 *     --------------------------------- 
 *     Function name :  gl1_set_sb_drx_ref
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Sets the drx ref for sb struct
 *
 *     Parameters :
 *     ---------------
 *     drx_ref, gas_id
 *
 *     Returns :
 *     -----------
 *     None
 *
 */
void gl1_set_sb_drx_ref(uint8 drx_ref, gas_id_t gas_id)
{

  l1_sc_acq_data[gas_id].sb.drx_ref = drx_ref;
  MSG_GERAN_MED_1_G("gl1_set_sb_drx_ref: set to %d", drx_ref);
}  

/* 
 *     --------------------------------- 
 *     Function name :  gl1_get_sb_drx_ref
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Get the drx ref for sb struct
 *
 *     Parameters :
 *     ---------------
 *     gas_id
 *
 *     Returns :
 *     -----------
 *     None
 *
 */
uint8 gl1_get_sb_drx_ref(gas_id_t gas_id)
{

  return l1_sc_acq_data[gas_id].sb.drx_ref;
}  

/* 
 *     --------------------------------- 
 *     Function name :  gl1_set_fb_drx_ref
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Sets the drx ref for fb struct
 *
 *     Parameters :
 *     ---------------
 *     drx_ref, gas_id
 *
 *     Returns :
 *     -----------
 *     None
 *
 */
void gl1_set_fb_drx_ref(uint8 drx_ref, gas_id_t gas_id)
{

  l1_sc_acq_data[gas_id].fcb.drx_ref = drx_ref;
  MSG_GERAN_MED_1_G("gl1_set_fb_drx_ref: set to %d", drx_ref);
}

/* 
 *     --------------------------------- 
 *     Function name :  gl1_get_fb_drx_ref
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Get the drx ref for fb struct
 *
 *     Parameters :
 *     ---------------
 *     gas_id
 *
 *     Returns :
 *     -----------
 *     None
 *
 */
uint8 gl1_get_fb_drx_ref(gas_id_t gas_id)
{

  return l1_sc_acq_data[gas_id].fcb.drx_ref;
}

