/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             L 1    T R A N S F E R    I S R

GENERAL DESCRIPTION
   This module handles the L1 TRANSFER mode isr processing.
gl1_gbta_get_nv_gbta_support
EXTERNALIZED FUNCTIONS
  L1_control_transfer
    Procedure to service the tdma interrupt when in transfer mode.




INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_transfer.c#2 $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18/12/17   sn        CR2157525 Correct/Update TBF EST TYPE in case of Transfer Reconfig
10/06/17   rc        CR2120964 Reset tuneaway_and_ncell_data_initialized flag while releasing TBF when PCA RRBP has been sent
29/06/17   mn        CR2066103 Don't change pending_allocation if valid DL_ST or UL_ST is present.
16/03/17   mn        CR2020307 FR40846:Packet transfer Error Recovery mechanism.
17/06/16   ng        CR1030473 Send MCVS request of 384 MHZ at the time of initiating G2X search when GL1 is in Transfer mode (modify fix of CR 1021833 & 1025293)
31/05/16   dg        CR1021833 Send MCVS request of 384 MHZ at the time of initiating G2L search with HMSC ON
18/5/16    sn        CR1007565 PACKET Transfer DTF Events/Handlers Phase1
29/09/15   cja       CR915689 Add check for lowest_tn in range for KW
22/09/15   cja       CR912029 Fix KW error for slammed ul alloc array overflow
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
08/07/15   nm        CR836549 set main command to transfer command if ul/dl connect is pending after dl/ul connect
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
05/06/15   ab        CR848697 : GBTA not allowed after fall back and UE does GTA instead 
04/06/15   ab        CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
21/04/15   sjw       CR822898: removal of deprecated ACQUISITION reasons
07/04/15   shr       CR808932: COEX support for DTM
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
24/03/15   sjw       CR812407: update for new MPH_SET_PRIORITY_IND payload
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
16/02/15   mko       CR779465 Don't call grm_retain_lock when sending PH_CONNECT_IND
                               Retain lock is already done from gpl1_gprs_ds_init_pch_suspension, no need to do again when sending PH_CONNECT_IND
16/02/15   mko       CR775889: Don't set TRM extension flag for LTA SUPPORTED in DTM mode
16/02/15   mko       CR771860: Clear the TRM extension flag when TA is blocked by upperlayers
25/07/14   ab        CR773792: Perform header decodes only during DL tbf release, reduces TX Blanking
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs. 
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
07/10/14   pg        CR730132 F3 cleanup 
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build 
12/09/14   fm        CR718734 to avoid RX window is stuck in close state, open rx window when GL1 is in L1_TRANSFER_INIT state 
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
14/07/14   dg        CR685818 Continue with G2L search only if no block adjustment is pending
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
30/07/14   pjr       CR701243 Handling MMS priority indication to block tune away during MMS
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
26/06/14   ab        CR 682000 - COEX - GL1 to update ARFCN if they are not registered when start start is next radio block
20/07/14   mko       CR689023 Register G2x TA sequence to suspend TBF for Time slot adjustment
11/07/14   sjw       Standardise use of gl1_is_geran_tuneaway_allowed()
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
30/05/14   ws        CR657674 Data + MMS Phase 1
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
16/04/14   aga       CR644999 - G stuck in data transfer mode update required to CR341208
03/04/14   ab        CR 639723 : Notify PS - to register ARFCN/Freq Id when starttime has elapsed when TRM lock is not acquired Single Shot
21/03/14   sai       CR614664: Call trm_retain_lock when enabling G2W TA
19/03/13   ws        CR627983 - Correct Previous TFI handling if we Move from UL only TBF to UL+DL TBF
17/03/14   ws        CR614664 - Back out!
13/03/14   sai       CR614664: Call trm_retain_lock when enabling G2W TA
13/03/14   mc        CR602368: COEX : Support for Frequency Hopping over large ARFCN lists
13/02/14   nm        CR600645 use MAX_NUM_UL_PACCH_MSGS for checking wrap around of ul_desc_ptr under 
                              compilation flag FEATURE_GSM_GPRS_MSC33
31/01/14   mko       CR487642: DSDS Dont schedule rx and tx commands, when FW has not been conditioned
23/01/14   cgc       CR606502 rework CR589523 changed to l1_sc_set_skip_g2x_pkt_txfr()
15/01/14   cgc       CR589523 fix to skip xfer g2l
30/12/13   mko       CR 579095: Block G2W TA only if both device mode and device pref indicate TSTS
                     mode of operation.
26/11/13   ab        CR581794: DSDS GBTA: Do now allow GBTA during DL Esbalishtment is full completed and startime is elapsed.
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
17/10/13   js        CR 534791 Added gas_id for grlc_is_xfer_active() and grlc_ul_data_detect ()
05/10/13   sp        CR552188: Corrections upon CR542963
05/10/13   gk        CR542963: Removed Reset of release pending at gprs idle mode req.
27/09/13   sp        CR551580: Add NV logging of outgoing L1 messages
25/09/13   aga       CR551779 - G2W - Avoid going in G2W tuneaway gap for TSTS
14/09/13   ap/aga    CR545423 - G2W Tune Away Feature
20/08/13   sk        CR530970 Ensure GBTA works properly in DSDS config
15/08/13   sk        CR524039 GBTA bringup changes
10/07/13   pjr       CR507544 use pending_frequency_info_ptr when notifying ps assignment to arbitratror
08/05/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
29/04/13   ws        CR481359 enable GTA support
05/04/13   mc        CR470155: COEX Support - GERAN arbitrator framework - PS integration
15/02/13   ws        CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
30/07/12   pa        CR383547: Don't schedule G2T if TBF starts during BLk21-24
27/06/12   pg        CR320137: Ensure ul_desc_ptr remains within the bounds of ul_desc[] buffer
08/06/12   pa        Added support for GPLT.
26/03/12   jj        CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
29/09/11   ab        CR299297 : Incorrect Handling of Control messages during TBF
                     reconfiguration boundary
19/08/11   cgc       CR289295 Add NULL ptr check, side effects from CR 277664
22/07/11   ab        CR 297022 Fix null ptr dereferencing during DL release.
21/03/11   ky        CR277664:Ignoring the DL data block received with correct DL TFI but
                     on a different timeslot which is not allocated in the current downlink tbf
17/03/11   cja       CR279518 Add single slot FTM power store for GPRS/EDGE
27/01/11   ap        CR 272988 Fix potential null ptr dereferencing
24/12/10   cja       CR 235173 - (merge)fixed null ptr during UL release and DL being established
01/12/10   ap        CR 263337 Incorrect TFI being set for Dynamic Downlink timeslot
                     reconfigure case
07/11/10   ab        CR 262876 - Panic Reset - await_ass guard timer expires
                     if T3192 being stopped incorrectly.
24/09/10   cja       Fix KlocWork warnings
07/06/10   cja       Fix Klockwork error
13/04/10   pg        CR#234356: Check for NULL dl_tbf_ptr during PS handover handling
25/01/10   ab        CR#: 223317 - update current_alloc type during release procedure
                     to enable L1 to have the right pending_alloc state.
02/12/09   ws        Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                     Improvements
29/09/09   ap        CR 193147 Incorrect handling of TFI
23/09/09   ps        Resolve Klocwork errors
30/01/09   ab        CR#: 170232 - IOT-Alcatel- Enh Ext UL TBF -
                     PUDCB send during transfer as RLC is doing reprocess and
                     block UL FIFO.
28/01/09   og        Do not apply the Enhanced Ext TBF criterea in single block
                     TBF. Resolves CR170320.
24/11/08   og        FEATURE_GSM_GPRS_MSC33 GPL1 changes.
13/10/08   ab        CR#: 159780 - IOT-Alcatel- Enh Ext UL TBF -
                     Extra PUDCB send during cell selection
30/09/08   ap        Resubmission of CR150107, due to perforce integration issue.
30/09/08   ap        Resolve CR150107. Fix the handling of simultaneous TS Reconfigure
                     (UL and DL) and L1 Downlink Release.  When handling this message
                     combination the resulting dl tbf pointer is incorrectly assigned.
14/03/08   ip        Added PSHO changes under FEATURE_GPRS_PS_HANDOVER
16/07/07   og        Fix the handling of EGPRS multiblock PACCH allocations when
                     pending msgs > 1. Resolves CR 121577.
27/03/07   og        Resolve CR114347, Ensure start time wrap around condition
                     around the hyperframe is handled correctly.
22/03/07   ws        CR 114678 - Fix for PACCH/U buffer handling when EDTM FEATURE
                     is undefined
06/02/07   cs        Fix compile if E/DTM not enabled
26/01/07   og        Implement UL ctrl msg deletion in L1.
25/08/06   og        EDTM addition of the mac_ph_cs_connect_ind message and
                     typedef.
16/08/06   ws        Added Exhanced Extended uplink tbf feature. Don't tx dummy
                     control blocks if RLC fifo is empty
25/05/06   og        Improve PTR reaction time, resolves CR90515.
16/01/05   og        Merge of change 280067 from the raven 3.0 branch, resolves
                     SR 791123.
18/11/05   og        Correction to suppression of QLint 8.0 errors.
11/11/05   og        Lint 8.0 AMSS_std_critical_errors removal.
21/09/05   og        Fix to the restructuring of the rlc deferred blocks handling
                     in PL1.
26/08/05   og        Removal of DEBUG_TRANSFER compiler switch and associated
                     code.
22/08/05   og        Add the setting of mac mode to NPL1.
17/08/05   og        Removal of dummy ctrl block dual buffer.
12/08/05   ws        Updated for initial DTM release
08/08/05   og        Restructuring PL1 before addition of DTM.
07/29/05   gfr       Support for NPL1 DTM interface changes
23/06/05   og        Code changes to accomodate the uplink tbf data block count
                     log packet.
10/06/05   og        Set the link qual meas mode to invalid when releasing downlink
                     tbf in concurrency.
10/06/05   og        Remove the forcing of egprs link quality measurement mode
                     to 1 ( INTERF_MEASUREMENTS).
02/06/05   og        Removal of header-header includes.
01/06/05   og        Add validity flag check to the metrics callback routines.
31/05/05   og        Inform the egprs qual meas module of the broadcast bep period
                     directly.
27/04/05   og        Allow PBCCH reading in pkt transfer on timeslots lower than
                     the camp timeslot.
21/04/05   og        Fix to the queuing a downlink assignment and an uplink release
                     at the same time.
05/04/05   nt       Added the processing of the L1_PARAM_UPDATE within the main function
                    gpl1_gprs_control_transfer.
11/3/05    og       Changes to egprs dl tfi and IR handling.
11/02/05   ws       Added setting of active window size for DL IR for EGPRS
11/02/05   ws       Added Flag for DL release handler to ensure PDAS or PTR msgs
06/01/05   ws        Only reset starting time when sending connect_ind if we are
                     not doing single block allocation
16/12/04   og        Fix to typing error.
16/12/04   og        Merge of fix from the saber 4x branch.
12/11/04   og        Added code to handle BCCH/PBCCH BEP PERIOD field reserved case.
11/11/04   og        Copy in BCCH/PBCCH bep_period into L1 CCCH/PCCCH data structures.
21/10/04   ws        Corrected bug in preserving sched_msgs across UL TBF reconfigure
19/10/04   og        Added single block bep period2 handling.
18/10/04   og        Fix to the configuration of interf meas module in egprs pkt
                     transfer.
15/10/04   og        Removed Fixed TBF allocation related source code.
15/10/04   ws        Moved preservation of sched_msgs form PUA and PTR handlers to
                     TBF_RECONFIG sub_state
14/10/04   og        Fixes to the handling of inteference measurements while
                     in egprs packet transfer.
12/10/04   ws        Added Multiblock allocation for EGPRS
23/08/04   ws        When sending rel confirm for DL or UL+DL stop t3192 if running
19/08/04   ws        Removed Puncturing scheme offset PS1 = 0 in RLC and frame layer
25/06/04   kf        Added check for meas buffer full before stuffing.
02/06/04   sv        Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
02/06/04   ws        backed out previous UL+DL release case
18/05/04   ws        Added handling of PDAS when UL+DL are to be released
05/05/04   og        Removal of Lint warnings.
25/03/04   ws        Moved setting up of dynamic_tbf_ptr in DOWNLINK_ALLOCATION in
                     TRANSFER_INIT sub state
11/03/04   ws        If an initial tav is received in transfer and we don't have a
                     valid tav then apply new value else ignore message
19/02/04   ws        Add test for Starting time <= Fn in case SCE is slow aborting
                     receives in progress in IDLE, enusres PH_CONNECT_IND is sent
20/01/04   ws        Added Validation for partial releases of UL & DL TBF's
26/11/03   ws        Lint cleanup
20/11/02   ws        Added deleting of RRBP's when initialising SINGLE_BLOCK
20/11/03   ws        Added fuctionality to inform MAC that PDAN w CRD can't
                     be sent
05/11/03   kf        added the use of SIZE_OF_RRBP_BUFF
01/10/03   hv        Removed rlc no data message.
12/09/03   pjr       Added code to handle starting time for assignment whilst
                     release in progress
10/09/03   ws        Setup correct structure for  Cont. TA if not present when
                     we first enter transfer mode
09/09/03   ws        Modified reconfiguring PTCCH channels only when a param
                     change is detected
04/09/03   ws        Removed gpl1_log_gprs_air_interface_summary() to l1_task.c
22/08/03   ws        Fixed bug with clearing ul + dl ptcch valid flag for PTR
12/08/03   ws        Removed ncell_meas_fixed from gpl1_gprs_send_ftn()
12/08/03   pjr       Only initialise serving cell measurements when coming from
                     52-idle if some measurements exist.
17/07/03   ws        Downgraded MSG's as no longer required
11/07/03   ws        Added MSG_ERROR's for incorrect state's in PL1
10/07/03   bk        Changed the call to gpl1_gprs_setup_start_idle to be called from
                     the task context.
09/07/03   pjr       Added initialisation of serving cell RLA_P to INIT.
27/06/03   ws        Deleted PDAN RRBP's when reassigning DL TBF
26/06/03   ws        wait until PCA is txd before releasing TBF
25/06/03   gw        Use gprs_gpl1_setup_start_idle() to go to Idle Mode.
23/06/03   gfr       Compiler warning cleanup.
20/06/03   ws        Further improvements to Cont. PTCCH to stop parameters being
                     invalidated incorrectly.
20/06/03   pjr       Initialise gprs serving cell measurements in transfer init.
19/06/03   gw        Changes for starting/stopping SCE going in/out of
                     52 Idle Mode.
19/06/03   gw        Changes to starting and stopping of SCE.
17/06/03   ws        Added code for PTCCH/D retry as per GSM 05.10
16/06/03   pjr       Modified serving cell interference measurement scheduling
                     when PTCCH present.
10/06/03   pjr       Added handling of PDA whilst waiting for PUA starting time
                     and PUA whilst waiting for PDA starting time.
04/06/03   ws        Added DL PTCCH abort if tai parameters change
28/05/03   kf        removed ba list copy.
23/03/03   ws        Added missing num_rrbp_msg--
19/03/03   ws        Implemented defered abort handling for rlc ul fifo
15/03/03   ws        Added grlc_ul_data_detect(), Fixed Ptcch invalidation
                     bug and implemented toggle buffer for ul dummy data
12/05/03   dp        Fixed bugs in transferring Ncell BSIC
02/05/03   dp        Changes to preserve Ncell BSIC across transitions
02/05/03   gw        Merged in power transfer changes from branch.
02/05/03   pjr       Added suspension of interference measurements to
                     L1_TRANSFER_RECONFIG
09/04/03   ws        Moved starting of surround cell Engine to original position
03/04/03   pjr       Moved starting of surround cell engine from L1_TRANSFER_INIT
                     to PH_CONNECT_IND. This ensures the surround cell engine
                     will not start before the starting time elapses.
02/04/03   gw        Changes to reflect modifications in monscan module.
17/03/02   ws        Corrected missing comment
12/03/02   ws        Added deletion of RRBP when moving to DL tranfer
07/03/03   hv        Made "rlc no data" message conditional to RLC being in xfer
26/02/03   ws        Disabled USF granularity for DL TBF only cases
26/02/03   npr       Changed instances of INVALID to GPL1_INVALID
21/02/03   ws        Modification's for starting time for block's gap
17/02/03   kf        Changes for running average in transfer.
14/02/03   pjr       Modified mode 52 starting times.
10/02/03   ws        Added stopping T3192 when a DL or UL+DL TBF is connected and
                     deletion of DL_FAI RRBP from the previous DL TBF.
07/02/03   ws        Added sending RLC_PH_READY_TO_SEND if RLC has no data to send
07/02/03   ws        Renamed gpl1_log.h to gpl1_gprs_log.h in include path
06/02/03   ws        Modified gap for timeslot reconfigure to align with new scheduling
20/01/03   ws        Corrected bug relating to setting dl_tbf_ptr to NULL
16/01/03   ws        Corrected bug in leaving a block's gap if lowest TN changes
19/12/02   ws        Added check for TAI or TA TN changing for invalidating PTCCH/D
19/12/02   pjr       Removed storing of power control parameters in l1_transfer_data[gas_id]
                     structure.
17/12/02   pjr       Removed call to function gl1_set_ncell_block_start(TRUE) when
                     next_frame_is_acq_ncell_block_start flag set by serving cell
                     measurements.
16/12/02   pjr       Modified setting of next_frame_is_acq_ncell_block_start flag.
06/12/02   pjr       Modified calling of serving cell measurements.
29/11/02   pjr       Modified reconfigure and dynamic alloc cases to ensure
                     ul_cont_ta_valid is cleared.
25/11/02   pjr       Modified serving cell interference measurements.
22/11/02   pjr       Set downlink pointer to NULL when not required.
14/11/02   ws        Updated transition to idle mode based on new L1_GPRS_IDLE state
13/11/02   ws        Reset starting_time_valid flag when PH_CONNECT_IND is sent
11/11/02   ws        Updated Release process if rrbp is pending
06/11/02   ws        Added release type testing in gpl1_gprs_send_l1_mac_tbf_rel_confirm()
04/11/02   pjr       Merge Neighbour cell measurements under feature switch
01/11/02   pjr       Added serving cell measurements under feature switch
10/21/02   he        Replaced FN by gl1_get_FN()
28/10/02   ws        Added check to L1_DYNAMIC_DOWN_ALLOC reconfig to ensure that a dl
                     tbf is present before assigning it to the pending_dl_ptr
09/09/02   ws        Added L1_SINGLE_BLK_RX sub state handling
13/09/02   pjr       Reset pending release flag in send ftn function. Reset downlink
                     pending ptr when used in dynamic.
11/09/02   pjr       Added function gpl1_gprs_send_l1_mac_tbf_rel_confirm
3/09/02    ws        Removed setting of sub state when returning to IDLE from TRANSFER
8/08/02    ws        Corrected Lint error's and Warning's
2/08/02    ws        Added call to gpl1_gprs_control_dl_tbf() if mac mode is fixed alloc
1/08/02    ws        Added PL1_FW_SIM arround all MSG_XX calls
                     Changed FEATURE_GSM to FEATURE_GSM_GPRS_L1
                     Added PARTIAL and FULL RELEASE functionality
                     Added ta_ptr to RECONFIG state

24/07/02   pjr       Added debug LED test setting under switch
                     DEBUG_GSM_GPRS_LED_TEST.
24/07/02   ws        Changes relating to imposing a radio block gap if no starting
                     time is present
22/07/02   ws        Restored dl tbf ptr in dynamic structure if dynamic tbf has been
                     reconfigured
18/07/02   ws        Fixed bug that stops substates DYNAMIC_2 and DOWNLINK_2
                     calling the aprropriate isr function.
15/07/02   ws        Added changes to support sending MAC_PH_CONNECT_IND when no
                     starting time specified. General changes for transfer reconfig.
09/07/02   pjr       Changes to gpl1_gprs_transfer_send_ftn to used
                     waiting_for_ul_start from transfer_data struct.
02/07/02   ws        Added changes to support sending MAC_PH_CONNECT_IND correctly,
                     modified gpl1_gprs_get_ul_data() to support concurrent tbf,
                     and various changes to allow dynamic and downlink tbf's to be
                     reconfigured
11/06/02   pjr       Added changing to access mode to gpl1_gprs_transfer_send_ftn
                     function.
31/05/02   ws        Added L1_TRANSFER_RECONFIG state code
27/05/02   ws        changed dynamic and fixed alloc types for downlink alloc.
09/05/02   pjr       Added single block allocation
1/11/01    ws        Initial version .
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "customer.h"


#include "comdef.h"
#include "memory.h" /* for memset() */
#include "l1_task.h"
#include "l1_isr.h"
#include "gl1_msg_g.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_task.h"
#include "gmacl1sig.h"
#include "gmacl1sig_g.h"
#include "gmacl1.h"
#include "ms.h"
#include "msg.h"
#include "l1_sc.h"
#include "l1_sc_int.h"
#include "grlculfifo.h"
#include "grlcl1.h"
#include "l1_utils.h" /* for gl1_get_FN() */

#include "gpl1_gprs_log.h"


#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_ptcch.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_utils.h"

#ifdef DEBUG_GSM_GPRS_LED_TEST
#error code not present
#endif
#ifdef FEATURE_G2X_TUNEAWAY
#include "gl1_msg_pdch.h"
#include "gpl1_dual_sim.h"
#endif /* FEATURE_G2X_TUNEAWAY */

#include "gl1_hw_g.h"
#include "gpl1_grm_intf.h"

#include "gl1_msg_wtr_hop.h"


#ifdef FEATURE_GL1_GPLT
#include "gplt_cmdi.h"
#include "gplt_ctl.h"
#endif /*FEATURE_GL1_GPLT*/

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#include "gl1_arbitrator_cxm.h"

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */

#ifdef FEATURE_GSM_COEX
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX */

extern dedicated_data_T l1_dedicated_data[NUM_GERAN_DATA_SPACES];
#endif /* FEATURE_GSM_COEX_SW_CXM */


/* This structure is used to store the total number of
 * rlc data blocks and mac dummy control blocks during
 * the lifetime of an uplink tbf.
 * This is used to generate the uplink statistics
 * log packet.
 */
static ul_data_blk_count_t  gpl1_uplink_tbf_data_blk_count[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE_S( 0 ) };
static boolean coex_update_params[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION      gpl1_get_dtm_assig_status

DESCRIPTION   gets the value of the DTM assignment flag

RETURN VALUE  TRUE if dtm assignment recieved

===========================================================================*/
static boolean gpl1_get_dtm_assig_status(gas_id_t gas_id);

#endif /* FEATURE_GSM_DTM */

/*===========================================================================

FUNCTION  gpl1_gprs_send_L1_MAC_PH_CONNECT_IND

DESCRIPTION
  Creates the above named message and sends to the GS_QUEUE_MAC
  queue or GS_QUEUE_TEST.

  The function sets the tbf_type_ind parameter to either
  UPLINK_TBF_ESTABLISHED or DOWNLINK_TBF_ESTABLISHED depending
  on which TBF is to be established.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_send_l1_mac_ph_connect_ind(uint16 tbf_type, gas_id_t gas_id)
{
#ifdef FEATURE_GL1_GPLT
  gplt_cmd_type *gplt_cmd_ptr;    /* ptr to pkt idle mode */

    if ( (gplt_cmd_ptr = (gplt_cmd_type *) q_get( &gplt_l1_cmd_free_q )) == NULL )
    {
      MSG_GERAN_HIGH_0_G( "No buffers on plt_cmd_free_q");
        /* No command buffers available */
    }else
    {
      gplt_cmd_ptr->hdr.gas_id = gas_id;
      gplt_cmd_ptr->hdr.cmd = GPLT_MAC_PH_CONNECT_IND;   /* Set command type */
      gplt_cmd_ptr->hdr.cmd_hdr.task_ptr = rex_self();        /* Reply to task */
      gplt_cmd_ptr->hdr.cmd_hdr.done_q_ptr = &gplt_l1_cmd_free_q; /* Free queue */
      gplt_cmd_ptr->hdr.cmd_hdr.sigs = GPLT_CMD_RSP_SIG;       /* Signal back */

      gplt_cmd_ptr->mac_ph_connect_ind.param.tbf_type_ind = tbf_type;

      MSG_GERAN_HIGH_0_G( "GPLT DBG: Sending GPLT_MAC_PH_CONNECT_IND");
      gplt_cmd( gplt_cmd_ptr );
      /* Send the PLT command to GPLT*/
    }

#else
  l1_mac_sig_t   message_buffer,*message;

  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = MAC_PH_CONNECT_IND;
  message->msg.connect_ind.tbf_type_ind = (uint8)tbf_type;
  message->msg.connect_ind.gas_id = gas_id;
  message->msg.connect_ind.connect_status = MAC_L1_CON_STAT_NO_CAUSE;

  /* Send the message */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);
#endif /* FEATURE_GL1_GPLT*/
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  gpl1_gprs_send_l1_mac_ph_connect_ind_trm_failed

DESCRIPTION
  Creates the above named message and sends to the GS_QUEUE_MAC
  queue or GS_QUEUE_TEST.

  The function sets the tbf_type_ind parameter to either
  UPLINK_TBF_ESTABLISHED or DOWNLINK_TBF_ESTABLISHED depending
  on which TBF is to be established.

  This is specifically designed for use when TRM lock has been denied

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_send_l1_mac_ph_connect_ind_trm_failed(uint16 tbf_type, gas_id_t gas_id )
{

  l1_mac_sig_t   message_buffer,*message;

  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = MAC_PH_CONNECT_IND;
  message->msg.connect_ind.tbf_type_ind = (uint8)tbf_type;
  message->msg.connect_ind.gas_id = gas_id;
  message->msg.connect_ind.connect_status = MAC_L1_CON_STAT_TRM_LOCK_FAIL_DL;

  /* Send the message */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);
}
#endif

/*===========================================================================

FUNCTION  gpl1_gprs_send_MAC_PH_DATA_IND()

DESCRIPTION
  Creates the above named message and sends to the GS_QUEUE_MAC
  queue or GS_QUEUE_TEST

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_send_mac_ph_data_ind(
                                   uint32 frame_number,
                                   uint8 timeslot,
                                   uint8 *control_data, gas_id_t gas_id
                                   )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];

  l1_mac_sig_t   message_buffer,*message;

  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = MAC_PH_DATA_IND;


  /* Set up the data in the message body */
  message->msg.data_ind.frame_no = frame_number;
  message->msg.data_ind.timeslot = timeslot;
  message->msg.data_ind.bcch_arfcn = l1_idle_data->campedon_cell_ARFCN;

  message->msg.data_ind.gprs_l1_page_group_flag = FALSE;
   message->msg.data_ind.gas_id = gas_id;
  /* Send the message */
  L1_send_gprs_gmac_message(message, control_data,gl1_get_FN(gas_id), gas_id);

}
#ifdef FEATURE_GPRS_PS_HANDOVER
/*===========================================================================

FUNCTION  gpl1_gprs_send_L1_MAC_PSHO_CNF

DESCRIPTION
  Creates the above named message and sends to the GS_QUEUE_MAC
  queue or GS_QUEUE_TEST.

  The function sets the psho status accordingly and sends the
  singnal to MAC

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_send_l1_mac_psho_cnf(psho_status_t psho_status, gas_id_t gas_id)
{

  l1_mac_sig_t   message_buffer,*message;

  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = L1_MAC_PSHO_CNF;

  /* Set the status */
  message->msg.psho_cnf.psho_status = psho_status;
  message->msg.psho_cnf.gas_id = gas_id;
  /* Send the message */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);
}
#endif

/*===========================================================================

FUNCTION  GPL1_GPRS_NOTIFY_START()

DESCRIPTION
  Controls notification of GPRS start to external components (MAC_PH_CONNECT_IND
  to MAC and GL1 arbitrator notification) and resetting RRBP and RLC timers
  based on the allocation UL,DL or UL and DL
DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_notify_start(transfer_data_T *transfer_data_ptr, gas_id_t gas_id)
{
  const uint32  FN = gl1_get_FN( gas_id );
  const uint32  st_fn_diff = SUB_FN( transfer_data_ptr->starting_time, FN );
  uint8 num_ul_slots = 0, num_dl_slots = 0, lowest_dl_OTA_timeslot = 0;
  boolean slammed_ps_dl_alloc[GL1_DEFS_SLOTS_IN_FRAME], slammed_ps_ul_alloc[GL1_DEFS_SLOTS_IN_FRAME];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  frequency_list_T freq_params = {0};
  dynamic_tbf_information_T  *updated_valid_tbf_dynamic_ptr = transfer_data_ptr->dynamic_tbf_ptr;
  downlink_tbf_information_T *updated_valid_tbf_dl_ptr      = transfer_data_ptr->dl_tbf_ptr;

  /* Send MAC_PH_CONNECT_IND to MAC, if starting time is this frame
     */
  if (!transfer_data_ptr->starting_time_elapsed)
  {
    if(  ( 0 == st_fn_diff )
       ||( st_fn_diff > 31623 )
       ||( FALSE == transfer_data_ptr->starting_time_valid )
      )
    {
      gpl1_log_gprs_transfer_summary(gas_id);	  
#if defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER ) && defined ( FEATURE_GERAN_PTM_ERR_RECOVERY )
      #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH && FEATURE_GERAN_PTM_ERR_RECOVERY*/
      gpl1_gprs_send_l1_mac_ph_connect_ind(transfer_data_ptr->tbf_est_type, gas_id);
      MSG_GERAN_HIGH_2_G("PL1: PH_CONNECT_IND [%d] [%d]",transfer_data_ptr->tbf_est_type,FN);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TBF_CONNECT);
#endif	  


      /* Don't reset if we are allocated a single block */
      if(transfer_data_ptr->current_alloc_type != L1_SINGLE_BLOCK_ALLOC)
      {
        /* Reset the starting time since the assignment is connected */
        transfer_data_ptr->starting_time = L1_GPRS_INVALID_FN;
      }

      if (transfer_data_ptr->release_in_progress &&
          transfer_data_ptr->release_in_progress_delay)
      {
        transfer_data_ptr->release_in_progress = FALSE;
        transfer_data_ptr->release_in_progress_delay = FALSE;
      }

      switch( transfer_data_ptr->tbf_est_type )
      {
          case  DOWNLINK_TBF_ESTABLISHED:

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
              gpl1_gprs_recovery_deregister_event(GPL1_DL_CONNECT_EVENT, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

              transfer_data_ptr->first_t3192_start = TRUE;

#ifdef FEATURE_G2X_TUNEAWAY
              if(transfer_data_ptr->dl_tbf_ptr != NULL)
              {
                transfer_data_ptr->dl_tbf_ptr->dl_tbf_with_t3192_active = FALSE;
              }
              else
              { /* transfer_data_ptr->dl_tbf_ptr is NULL, clear any residual setting */
                 transfer_data_ptr->dl_tbf_1.dl_tbf_with_t3192_active = FALSE;
                 transfer_data_ptr->dl_tbf_2.dl_tbf_with_t3192_active = FALSE;                 
              }
#endif

              /* Delete any RRBP's which have PDAN_FAI, PCA_DL_FAI or PDAN as
               * they are no longer valid
               */
              gpl1_gprs_delete_dl_rrbp_entries(gas_id);

              if(transfer_data_ptr->dl_tbf_ptr !=NULL )
              {
                 transfer_data_ptr->dl_tbf_ptr->dl_tfi_st = FN;
                 num_dl_slots = transfer_data_ptr->dl_tbf_ptr->nts;
                 updated_valid_tbf_dl_ptr = transfer_data_ptr->dl_tbf_ptr;
              }

              /* Determine if uplink awaiting for starting time */
              if( transfer_data_ptr->ul_st != L1_GPRS_INVALID_FN )
              {
                  /*  Uplink is waiting */
                  transfer_data_ptr->starting_time = transfer_data_ptr->ul_st;
                  transfer_data_ptr->tbf_est_type  = UPLINK_TBF_ESTABLISHED;
                  transfer_data_ptr->ul_st         = L1_GPRS_INVALID_FN;
                  if(transfer_data_ptr->pending_alloc_type != L1_NULL_ALLOC)
                  {
                    l1_tskisr_blk->main_command	   = L1_TRANSFER_COMMAND;
                    MSG_GERAN_HIGH_0_G("ul st valid..set main command to transfer cmd");
                  }
                  else
                  {
                    MSG_GERAN_ERROR_0_G("ERROR: pending alloc NULL but ul st is not invalid");
                  }
              }
              else
              {
                  transfer_data_ptr->starting_time_elapsed = TRUE;

                  /* ensure starting time is disabled
                   * before contiuing with transfer mode
                   */
                  transfer_data_ptr->starting_time_valid = FALSE;
              }

              transfer_data_ptr->dl_st = L1_GPRS_INVALID_FN;

              if ((transfer_data_ptr->rlc_mode == RLC_ACK) &&
                  (transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS)
                 )
              {
                e_grlc_dl_window_info_t win_info;

                /* IR is enabled for the EGPRS DL TBF so setup WS */
                if(e_grlc_dl_get_window_info(gas_id, &win_info))
                {
                  gl1_msg_IR_set_active_window(win_info.start_bsn, win_info.end_bsn,gas_id);
                  MSG_GERAN_HIGH_3_G("update dl win st_bsn %d end_bsn %d",win_info.start_bsn, win_info.end_bsn,gl1_get_FN( gas_id ));
                }else
                {
                  /* Setup default params as RLC hasn't been confiured for DL TBF yet
                   * No changes required for MSC30 - 34.
                   */
                  gl1_msg_IR_set_active_window(0,1024,gas_id);

                  MSG_GERAN_HIGH_0_G("No win info from RLC");
                }
              }

              break;

          case  UPLINK_TBF_ESTABLISHED:

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
              gpl1_gprs_recovery_deregister_event(GPL1_UL_CONNECT_EVENT, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

              /* Determine if downlink awaiting for starting time */
              if( transfer_data_ptr->dl_st != L1_GPRS_INVALID_FN )
              {
                  /* Uplink is waiting */
                  transfer_data_ptr->starting_time = transfer_data_ptr->dl_st;
                  transfer_data_ptr->tbf_est_type  = DOWNLINK_TBF_ESTABLISHED;
                  transfer_data_ptr->dl_st         = L1_GPRS_INVALID_FN;
                  if(transfer_data_ptr->pending_alloc_type != L1_NULL_ALLOC)
                  {
                    l1_tskisr_blk->main_command	   = L1_TRANSFER_COMMAND;
                    MSG_GERAN_HIGH_0_G("dl st valid..set main command to transfer cmd");
                  }
                  else
                  {
                    MSG_GERAN_ERROR_0_G("ERROR: pending alloc NULL but dl st is not invalid");
                  }
              }
              else
              {
                  transfer_data_ptr->starting_time_elapsed = TRUE;

                 /* ensure starting time is disabled before contiuing with transfer mode */
                 transfer_data_ptr->starting_time_valid = FALSE;
              }

              if(transfer_data_ptr->dynamic_tbf_ptr != NULL )
              {
                transfer_data_ptr->dynamic_tbf_ptr->ul_tfi_st = FN;
                num_ul_slots = transfer_data_ptr->dynamic_tbf_ptr->nts;
                updated_valid_tbf_dynamic_ptr = transfer_data_ptr->dynamic_tbf_ptr;
              }

              transfer_data_ptr->ul_st = L1_GPRS_INVALID_FN;

              break;

          case  UPLINK_DOWNLINK_TBF_ESTABLISHED:

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
             gpl1_gprs_recovery_deregister_event(GPL1_UL_CONNECT_EVENT, gas_id);
             gpl1_gprs_recovery_deregister_event(GPL1_DL_CONNECT_EVENT, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

              transfer_data_ptr->first_t3192_start = TRUE;
#ifdef FEATURE_G2X_TUNEAWAY
              if(transfer_data_ptr->dl_tbf_ptr != NULL)
              {
                transfer_data_ptr->dl_tbf_ptr->dl_tbf_with_t3192_active = FALSE;
              }
			  else
              { /* transfer_data_ptr->dl_tbf_ptr is NULL, clear any residual setting */
                 transfer_data_ptr->dl_tbf_1.dl_tbf_with_t3192_active = FALSE;
                 transfer_data_ptr->dl_tbf_2.dl_tbf_with_t3192_active = FALSE;                 
              }
#endif
              /* Delete any RRBP's which have PDAN_FAI,
               * PCA_DL_FAI or PDAN as they are no longer valid
               */
              gpl1_gprs_delete_dl_rrbp_entries(gas_id);

              transfer_data_ptr->starting_time_elapsed = TRUE;

              /* ensure starting time is disabled before contiuing with transfer mode */
              transfer_data_ptr->starting_time_valid = FALSE;

              transfer_data_ptr->dl_st = L1_GPRS_INVALID_FN;
              transfer_data_ptr->ul_st = L1_GPRS_INVALID_FN;

              if(transfer_data_ptr->dl_tbf_ptr !=NULL )
              {
                 transfer_data_ptr->dl_tbf_ptr->dl_tfi_st = FN;
                 num_dl_slots = transfer_data_ptr->dl_tbf_ptr->nts;
                 updated_valid_tbf_dl_ptr = transfer_data_ptr->dl_tbf_ptr;
              }

              if(transfer_data_ptr->dynamic_tbf_ptr != NULL )
              {
                transfer_data_ptr->dynamic_tbf_ptr->ul_tfi_st = FN;
                num_ul_slots = transfer_data_ptr->dynamic_tbf_ptr->nts;
                updated_valid_tbf_dynamic_ptr = transfer_data_ptr->dynamic_tbf_ptr;
              }

              if ((transfer_data_ptr->rlc_mode == RLC_ACK) &&
                  (transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS)
                 )
              {
                e_grlc_dl_window_info_t win_info;

                /* IR is enabled for the EGPRS DL TBF so setup WS */
                if(e_grlc_dl_get_window_info(gas_id, &win_info))
                {
                  gl1_msg_IR_set_active_window(win_info.start_bsn, win_info.end_bsn,gas_id);
                  MSG_GERAN_HIGH_3_G("update dl win st_bsn %d end_bsn %d",win_info.start_bsn, win_info.end_bsn,gl1_get_FN( gas_id ));
                }else
                {
                  /* Setup default params MAX Window size as RLC hasn't been confiured for DL TBF yet
                   * No changes required for MSC30 - 34.
                   */
                  gl1_msg_IR_set_active_window(0,1024,gas_id);

                  MSG_GERAN_HIGH_0_G("No win info from RLC");
                }
              }

              break;

          default:
#ifdef FEATURE_G2X_TUNEAWAY
              if(transfer_data_ptr->dl_tbf_ptr != NULL)
              {
                transfer_data_ptr->dl_tbf_ptr->dl_tbf_with_t3192_active = FALSE;
              }
              else
              { /* transfer_data_ptr->dl_tbf_ptr is NULL, clear any residual setting */
                 transfer_data_ptr->dl_tbf_1.dl_tbf_with_t3192_active = FALSE;
                 transfer_data_ptr->dl_tbf_2.dl_tbf_with_t3192_active = FALSE;                 
              }
#endif
              transfer_data_ptr->starting_time_elapsed = TRUE;

              /* ensure starting time is disabled before contiuing with transfer mode */
              transfer_data_ptr->starting_time_valid = FALSE;

              break;
      } /* End of switch statement on tbf_est_type */

#ifdef FEATURE_GSM_COEX_SW_CXM
      if(updated_valid_tbf_dl_ptr != NULL)
      {
        gl1_get_slammed_ps_dl_alloc(&slammed_ps_dl_alloc[0], updated_valid_tbf_dl_ptr, updated_valid_tbf_dynamic_ptr, &lowest_dl_OTA_timeslot, gas_id);
      }

      if(updated_valid_tbf_dynamic_ptr != NULL)
      {
        gl1_get_slammed_ps_ul_alloc(&slammed_ps_ul_alloc[0], updated_valid_tbf_dynamic_ptr, &lowest_dl_OTA_timeslot, gas_id);
      }

      if(!coex_update_params[gas_id])
      {
        if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
        {
          if(transfer_data_ptr->frequency_info_ptr != NULL)
          {
            freq_params = transfer_data_ptr->frequency_info_ptr->frequency_list;
          }
          else if(transfer_data_ptr->pending_frequency_info_ptr != NULL)
          {
            freq_params = transfer_data_ptr->pending_frequency_info_ptr->frequency_list;
          }
        }
        garb_intf_notify_ps_assignment(
          freq_params,
          num_ul_slots,
          num_dl_slots,
          gpl1_gprs_get_arfcns,
          &slammed_ps_ul_alloc[0],
          &slammed_ps_dl_alloc[0],
          l1_tskisr_blk->l1_state,
          gas_id
          );

      }
      coex_update_params[gas_id] = FALSE;
#endif /* FEATURE_GSM_COEX_SW_CXM */
    }
#ifdef FEATURE_GSM_COEX_SW_CXM
    else if(1 == st_fn_diff) /* Else of if st <= fn OR st is invalid - Notify the arbitrator of the PS assignment one frame prior to the start time */
    {
      coex_update_params[gas_id] =  TRUE;
      if(transfer_data_ptr->dl_tbf_ptr!=NULL)
      {
        num_dl_slots = transfer_data_ptr->dl_tbf_ptr->nts;
        updated_valid_tbf_dl_ptr=transfer_data_ptr->dl_tbf_ptr;
      }

      if(transfer_data_ptr->dynamic_tbf_ptr!=NULL)
      {
         num_ul_slots = transfer_data_ptr->dynamic_tbf_ptr->nts;
         updated_valid_tbf_dynamic_ptr=transfer_data_ptr->dynamic_tbf_ptr;
      }

      if( ( (transfer_data_ptr->tbf_est_type==DOWNLINK_TBF_ESTABLISHED) ||
             (transfer_data_ptr->tbf_est_type==UPLINK_DOWNLINK_TBF_ESTABLISHED) ) &&
            (transfer_data_ptr->pending_dl_tbf_ptr!=NULL))
      {
         num_dl_slots = transfer_data_ptr->pending_dl_tbf_ptr->nts;
         updated_valid_tbf_dl_ptr=transfer_data_ptr->pending_dl_tbf_ptr;
      }

      if( ( (transfer_data_ptr->tbf_est_type==UPLINK_TBF_ESTABLISHED) ||
            (transfer_data_ptr->tbf_est_type==UPLINK_DOWNLINK_TBF_ESTABLISHED) ) &&
            (transfer_data_ptr->pending_dynamic_tbf_ptr!=NULL))
      {
         num_ul_slots = transfer_data_ptr->pending_dynamic_tbf_ptr->nts;
         updated_valid_tbf_dynamic_ptr=transfer_data_ptr->pending_dynamic_tbf_ptr;
      }

      if(updated_valid_tbf_dl_ptr!=NULL)
      {
        gl1_get_slammed_ps_dl_alloc(&slammed_ps_dl_alloc[0], updated_valid_tbf_dl_ptr, updated_valid_tbf_dynamic_ptr, &lowest_dl_OTA_timeslot, gas_id );
      }

      if(updated_valid_tbf_dynamic_ptr!=NULL)
      {
        gl1_get_slammed_ps_ul_alloc(&slammed_ps_ul_alloc[0], updated_valid_tbf_dynamic_ptr, &lowest_dl_OTA_timeslot, gas_id );
      }

      if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
      {
        if(transfer_data_ptr->pending_frequency_info_ptr != NULL)
        {
          freq_params = transfer_data_ptr->pending_frequency_info_ptr->frequency_list;
        }
        else if(transfer_data_ptr->frequency_info_ptr != NULL)
        {
          freq_params = transfer_data_ptr->frequency_info_ptr->frequency_list;
        }
        else
        {
          MSG_GERAN_HIGH_0_G("pending_frequency_info_ptr and frequency_info_ptr == NULL");
          coex_update_params[gas_id] =  FALSE;
        }
      }

         garb_intf_notify_ps_assignment(
           freq_params,
           num_ul_slots,
           num_dl_slots,
           gpl1_gprs_get_arfcns,
           &slammed_ps_ul_alloc[0],
           &slammed_ps_dl_alloc[0],
           l1_tskisr_blk->l1_state,
           gas_id
         );
    } /* End of if st <= fn OR st is invalid */
#endif
  }     /* End of if start time has not elapsed    */

}

/*===========================================================================

FUNCTION  GPL1_GPRS_HANDLE_RECONFIG()

DESCRIPTION
  Controls reallocating UL, DL or UL and DL TBF's due to establishment or
  TBF release
DEPENDENCIES


RETURN VALUE
  Idle. TRUE - Drivers aborted sucessfully
        FALSE - Still waiting for channel drivers to abort

SIDE EFFECTS
  None

===========================================================================*/

boolean gpl1_gprs_handle_reconfig(transfer_data_T *transfer_data_ptr,
                                       l1_mode_command_T  mode,
                                       uint8 l1_state, gas_id_t gas_id )
{
  boolean  idle               = FALSE;
  const uint32 FN             = gl1_get_FN( gas_id );
  grm_client_enum_t g_client = gl1_map_gas_id_to_client_id(gas_id);


  switch (transfer_data_ptr->pending_alloc_type)
  {
  case L1_DOWNLINK_ALLOC:
    {
      boolean check_pending_dl_tbf = FALSE;

      if ( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
      {
        /* New timeslots are allocated for the downlink, Reset the filter values
         * specific to that timeslot.
         */
        switch ( transfer_data_ptr->current_alloc_type )
        {
        case L1_DOWNLINK_ALLOC:
          {
            if( NULL != transfer_data_ptr->pending_dl_tbf_ptr )
            {
              uint8  new_dl_ts_alloc = 0x0;

              uint8 *pend_ts_alloc_ptr =
              &( transfer_data_ptr->pending_dl_tbf_ptr->ts_allocation );

              uint8 *current_ts_alloc_ptr;

              /* Can occur when UL is released and DL getting connected for first time */
              if (transfer_data_ptr->dl_tbf_ptr == NULL)
              {
                current_ts_alloc_ptr  = &new_dl_ts_alloc;
                MSG_GERAN_HIGH_0_G("GL1 DL be assigned first time with UL being released");
              }
              else
              {
                current_ts_alloc_ptr = &( transfer_data_ptr->dl_tbf_ptr->ts_allocation );
              }

              /* Perform ( A | B ) ^ B to find purely new dl timeslos
               * where A = new timeslot allocation
               *       B = old timeslot allocation
               */
              new_dl_ts_alloc = ( (*current_ts_alloc_ptr) | (*pend_ts_alloc_ptr) )
                                ^(*current_ts_alloc_ptr);

              gpl1_egprs_scell_qual_meas_pacch_config( transfer_data_ptr->pending_dl_tbf_ptr->link_qual_meas_mode,
                                                        new_dl_ts_alloc,
                                                        transfer_data_ptr->pending_dl_tbf_ptr->bep_period2, gas_id );

              /* Configure the inerference measurements module */
              gpl1_egprs_scell_interf_meas_config( transfer_data_ptr->pending_dl_tbf_ptr->link_qual_meas_mode, gas_id );

            } /* End of if pending dl tbf ptr is not NULL */
            else
            {
              /* Major assignment/release scheduling error has occured
               * Printout debug msg error.
               */
              MSG_GERAN_ERROR_1_G(" NULL (pending) dl tbf ptr FNmod13 %u",frame_counters[gas_id].FNmod13);

            } /* End of else pending dl tbf ptr is NULL */
          }   /* End of case L1_DOWNLINK_ALLOC */
          break;

        default:
          {
            /* We are releasing the uplink tbf, does not make a difference for
             * quality measurment module so dont re-configure.
             */
            check_pending_dl_tbf = TRUE;
          }
          break;
        } /* End of switch on current allocation type */
      } /* End of if tbf is EGPRS */
      /* Moving to downlink so assume this involves
       * reconfiguring an already exisiting dl tbf
       */
      /* swap over to new downlink parameters */
      transfer_data_ptr->current_alloc_type = transfer_data_ptr->pending_alloc_type;

      /* Check to see that we have a pending dl */
      if (transfer_data_ptr->pending_dl_tbf_ptr)
      {
        if(transfer_data_ptr->dl_tbf_ptr !=NULL )
        {
          transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_tfi = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
          transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_ts_alloc = transfer_data_ptr->dl_tbf_ptr->ts_allocation;

        }
        transfer_data_ptr->dl_tbf_ptr = transfer_data_ptr->pending_dl_tbf_ptr;

        if (!transfer_data_ptr->starting_time_valid)
        {
          /* If no starting time is specified setup starting time for dynamic so a
           * radio block gap can be left (if needed)
           */
          switch (frame_counters[gas_id].FNmod13)
          {
          case 0:
          case 4:
          case 8:
            /* just missed programing rx,tx for NPL1 extend to next block */
            transfer_data_ptr->dl_tbf_ptr->tbf_start_time=((FN+4) % FRAMES_IN_HYPERFRAME);

            break;

          default:
            transfer_data_ptr->dl_tbf_ptr->tbf_start_time = FN;
            break;
          }

          gpl1_gprs_chk_blk_bndry(&transfer_data_ptr->dl_tbf_ptr->tbf_start_time);


        }


        /* Clear pending downlink pointer.
         * Check if there is a pending downlink assignment
         * getting connected because the pending dl tbf ptr
         * should not be nulled yet if that is the case.
         */
        if(  ( DOWNLINK_TBF_ESTABLISHED == transfer_data_ptr->tbf_est_type )
           &&( FALSE != check_pending_dl_tbf )
           &&( L1_GPRS_INVALID_FN != l1_transfer_data[gas_id].dl_st ) )
          {
            /* Don't NULL the pending pointer because that would prevent
             * the quality measurement module from receiving the
             * link qual meas mode and bep period. It also leads
             * de-referencing of a NULL pending pointer.
             */
          }
          else
          {
            transfer_data_ptr->pending_dl_tbf_ptr = NULL;
          }
      }

      gpl1_gprs_setup_cont_ta_params(gas_id);

      /* Check to see if frequency parameters have changed as part of this reallocation */
      if (transfer_data_ptr->pending_frequency_info_ptr)
      {
        transfer_data_ptr->frequency_info_ptr = transfer_data_ptr->pending_frequency_info_ptr;
        transfer_data_ptr->pending_frequency_info_ptr=NULL;
      }

      /* preset all USF's to unallocated since we are dl tbf only */
      memset((transfer_data_ptr->dynamic_tbf_1.usf_value),GPL1_INVALID,sizeof(uint8[8]) );

      /* Disable USF Granularity as we are DL TBF only */
      transfer_data_ptr->dynamic_tbf_ptr->usf_granularity=FALSE;
      /* preset number of timeslots to zero to stop dynamic tx's being scheduled */
      transfer_data_ptr->dynamic_tbf_1.nts = 0;

      /* Ensure sched_msgs is preset to zero. Only used for UL TBF's */
      transfer_data_ptr->dynamic_tbf_1.sched_msgs = 0;

      /* set up dynamic pointer to access downlink allocation */
      transfer_data_ptr->dynamic_tbf_ptr  = &(transfer_data_ptr->dynamic_tbf_1);
      /* set up downlink pointer within dynamic structure */
      transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr = transfer_data_ptr->dl_tbf_ptr;

      if(l1_state == L1_TRANSFER_MODE)
      {
        MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_dynamic_tbf from gpl1_gprs_handle_reconfig");
#ifdef FEATURE_GSM_WTR_HOP
        gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
        idle = gpl1_gprs_control_dynamic_tbf(
                                            transfer_data_ptr->dynamic_tbf_ptr,
                                            transfer_data_ptr->frequency_info_ptr,
                                            mode, gas_id
                                            );

        MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
        gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif
      }

    } /* End of case downlink alloc */
    break;


  case L1_DYNAMIC_ALLOC:
  case L1_DYNAMIC_DOWN_ALLOC:
    {
      /* Assume this is either reconfiguring an exisiting dynamic+down tbf
       * or an existing downlink tbf is reconfigured to down+dynamic tbf
       * or a dynamic is reconfigured to a dynmic+down tbf or
       * a single dynamic tbf is being reconfigured
       */
      if ( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
      {
        uint8  bep_period          = GPRS_PL1_BEP_PERIOD2_INVALID;
        uint8  link_qual_meas_mode = INVALID_MEAS_MODE;
        uint8  new_dl_ts_alloc     = 0x0;

        switch ( transfer_data_ptr->pending_alloc_type )
        {
        case L1_DYNAMIC_ALLOC:
          {
            switch ( transfer_data_ptr->current_alloc_type )
            {
            case L1_DYNAMIC_DOWN_ALLOC:
              {
                /* We are releasing the downlink tbf,
                 * reconfigure the qual meas module to
                 * switch off bep measurements and
                 * interference measurements.
                 */
                link_qual_meas_mode = INVALID_MEAS_MODE;
                bep_period          = GPRS_PL1_BEP_PERIOD2_INVALID;

                if( transfer_data_ptr->dl_tbf_ptr != NULL )
                {
                   /* change is to dynamic so no need  to check TFI */
                   transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi=GPL1_TFI_INVALID;
                   transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc = GPL1_INVALID_TN;
                }

              }
              break;

            default:
              {
                /* We are reconfiguring the uplink tbf,
                 * re-configure the quality measurment module
                 * with the new bep period.
                 */
                link_qual_meas_mode = INVALID_MEAS_MODE;

                if( NULL != transfer_data_ptr->pending_dynamic_tbf_ptr )
                {
                  bep_period = transfer_data_ptr->pending_dynamic_tbf_ptr->bep_period2;
                }
              }
              break;
            }
          } /* Case pending alloc is dynamic */
          break;

        case L1_DYNAMIC_DOWN_ALLOC:
          {
            switch ( transfer_data_ptr->current_alloc_type )
            {
            case L1_DYNAMIC_DOWN_ALLOC:
              {
                /* It could be that reconfiguration
                 * for either downlink or uplink is taking place.
                 * Check the pending ptrs to get a clearer picture
                 * of what exactly is happenning.
                 */
                if ( ( NULL != transfer_data_ptr->pending_dynamic_tbf_ptr )
                     &&( NULL == transfer_data_ptr->pending_dl_tbf_ptr ) )
                {
                  /* We have received an uplink assignment,
                   * re-configure the quality measurment module
                   * with the new bep period.
                   */
                  link_qual_meas_mode = INVALID_MEAS_MODE;

                  bep_period          = transfer_data_ptr->
                                        pending_dynamic_tbf_ptr->bep_period2;

                  if(transfer_data_ptr->dl_tbf_ptr !=NULL )
                  {
                      /* change is to dynamic so no need  to check TFI */
                      transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi=GPL1_TFI_INVALID;
                      transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc = GPL1_INVALID_TN;
                  }

                } else if ( ( NULL != transfer_data_ptr->pending_dl_tbf_ptr )
                            &&( NULL == transfer_data_ptr->pending_dynamic_tbf_ptr ) )
                {
                  /* A downlink tbf assignment is received,
                   * re-configure the quality measurment module
                   * with the new bep period/link qual meas mode.
                   */
                  uint8 *pend_ts_alloc_ptr =
                  &( transfer_data_ptr->pending_dl_tbf_ptr->ts_allocation );

                  uint8 *current_ts_alloc_ptr =
                  &( transfer_data_ptr->dl_tbf_ptr->ts_allocation );

                  link_qual_meas_mode = transfer_data_ptr->
                                        pending_dl_tbf_ptr->link_qual_meas_mode;

                  bep_period          = transfer_data_ptr->
                                        pending_dl_tbf_ptr->bep_period2;

                  /* Perform ( A | B ) ^ B to find purely new dl timeslos
                   * where A = new timeslot allocation
                   *       B = old timeslot allocation
                   */
                  new_dl_ts_alloc = ( (*current_ts_alloc_ptr) | (*pend_ts_alloc_ptr) )
                                    ^(*current_ts_alloc_ptr);

                  if(transfer_data_ptr->dl_tbf_ptr !=NULL )
                  {
                     transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_tfi
                                            = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
                     transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi
                                            = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
                     transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_ts_alloc
                                            = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
                     transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc
                                            = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
                  }

                } else if ( ( NULL != transfer_data_ptr->pending_dl_tbf_ptr )
                            &&( NULL != transfer_data_ptr->pending_dynamic_tbf_ptr ) )
                {
                  /* A ts reconfigure message is received, update the
                   * link quality measurement mode only
                   */
                  uint8 *pend_ts_alloc_ptr =
                  &( transfer_data_ptr->pending_dl_tbf_ptr->ts_allocation );

                  uint8 *current_ts_alloc_ptr =
                  &( transfer_data_ptr->dl_tbf_ptr->ts_allocation );

                  link_qual_meas_mode = transfer_data_ptr->
                                        pending_dl_tbf_ptr->link_qual_meas_mode;

                  bep_period          = GPRS_PL1_BEP_PERIOD2_INVALID;

                  /* Perform ( A | B ) ^ B to find purely new dl timeslos
                   * where A = new timeslot allocation
                   *       B = old timeslot allocation
                   */
                  new_dl_ts_alloc = ( (*current_ts_alloc_ptr) | (*pend_ts_alloc_ptr) )
                                    ^(*current_ts_alloc_ptr);

                  if(transfer_data_ptr->dl_tbf_ptr !=NULL )
                  {
                     transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_tfi
                                      = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
                     transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi
                                      = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
                     transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_ts_alloc
                                      = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
                     transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc
                                      = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
                  }
                }
              }
              break;

            case L1_DYNAMIC_ALLOC:
              {
                uint8  pend_ts_alloc = 0x0;

                if( NULL != transfer_data_ptr->pending_dl_tbf_ptr )
                {
                  pend_ts_alloc =
                    transfer_data_ptr->pending_dl_tbf_ptr->ts_allocation;

                  /* A downlink tbf assignment is received,
                   * re-configure the quality measurment module
                   * with the new bep period/link qual meas mode.
                   */
                  link_qual_meas_mode = transfer_data_ptr->pending_dl_tbf_ptr->link_qual_meas_mode;
                  /* no current downlink */
                  transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_tfi=GPL1_TFI_INVALID;
                  if( transfer_data_ptr->dl_tbf_ptr != NULL )
                  {
                    /* change is to dynamic so no need  to check TFI */
                    transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi=GPL1_TFI_INVALID;
                    transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_tfi=GPL1_TFI_INVALID;
                    transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_ts_alloc = GPL1_INVALID_TN;
                    transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc = GPL1_INVALID_TN;
                  }

                }

                if ( NULL == transfer_data_ptr->pending_dynamic_tbf_ptr )
                {
                  /* A valid bep period is only supplied in the pkt dl assignment
                   * msg and not in the ts reconfigure
                   */
                  if( NULL != transfer_data_ptr->pending_dl_tbf_ptr )
                  {
                    bep_period = transfer_data_ptr->pending_dl_tbf_ptr->bep_period2;
                  }
                } else
                {
                  /* This is a ts reconfigure msg, it does not have bep period2 */
                  bep_period = GPRS_PL1_BEP_PERIOD2_INVALID;
                }
                /* Perform ( A | B ) ^ B to find purely new dl timeslos
                 * where A = new timeslot allocation
                 *       B = old timeslot allocation
                 */
                new_dl_ts_alloc = pend_ts_alloc;
              }
              break;

            case L1_DOWNLINK_ALLOC:
              {
                /* We are reconfiguring the uplink tbf,
                 * re-configure the quality measurment module
                 * with the new bep period.
                 */
                link_qual_meas_mode = INVALID_MEAS_MODE;
                bep_period          = GPRS_PL1_BEP_PERIOD2_INVALID;

                if ( NULL != transfer_data_ptr->pending_dl_tbf_ptr )
                {
                  /* A tbf ts reconfig is received with possibly new dl timeslots,
                   * re-configure the quality measurment module
                   * with the link qual meas mode.
                   */
                  uint8 *pend_ts_alloc_ptr =
                  &( transfer_data_ptr->pending_dl_tbf_ptr->ts_allocation );

                  uint8 *current_ts_alloc_ptr =
                  &( transfer_data_ptr->dl_tbf_ptr->ts_allocation );

                  /* Perform ( A | B ) ^ B to find purely new dl timeslos
                   * where A = new timeslot allocation
                   *       B = old timeslot allocation
                   */
                  new_dl_ts_alloc = ( (*current_ts_alloc_ptr) | (*pend_ts_alloc_ptr) )
                                    ^(*current_ts_alloc_ptr);

                  link_qual_meas_mode = transfer_data_ptr->
                                        pending_dl_tbf_ptr->link_qual_meas_mode;

                  if(transfer_data_ptr->dl_tbf_ptr !=NULL )
                  {
                     transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_tfi
                                        = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
                     transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi
                                        = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
                     transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_ts_alloc
                                        = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
                     transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc
                                        = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
                  }

                }
                else if( NULL != transfer_data_ptr->pending_dynamic_tbf_ptr )
                {
                  /* @@Warning: Do not combine both if( ) checks into && statement
                   * since this order of checking must be guaranteed.
                   */
                  if( GPRS_PL1_BEP_PERIOD2_INVALID != transfer_data_ptr->
                                                      pending_dynamic_tbf_ptr->
                                                      bep_period2 )
                  {
                    /* This is an uplink assignment msg with a valid
                     * bep period value to update the meas module with.
                     */
                    bep_period  = transfer_data_ptr->pending_dynamic_tbf_ptr->bep_period2;
                    if(transfer_data_ptr->dl_tbf_ptr !=NULL )
                    {
                      transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi
                                        = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
                      transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc
                                        = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
                    }
                  }
                }
              }
              break;

            default:
              break;
            } /* End of switch on current alloc type */
          } /* Case pending alloc is dynamic + downlink */
          break;

        default:
          break;
        } /* End of switch on pending alloc type */

        /* Configure the EGPRS quality measurements capability */
         gpl1_egprs_scell_qual_meas_pacch_config( link_qual_meas_mode,
                                                  new_dl_ts_alloc,
                                                  bep_period, gas_id );

        /* Configure the inerference measurements module */
        gpl1_egprs_scell_interf_meas_config( link_qual_meas_mode, gas_id );
      } /* End of if tbf is EGPRS */

      /* Ensure sched_msgs is setup properly. Makes sure it is preserved across
       * reconfiguration of UL TBF only
       */
      if( (transfer_data_ptr->current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) ||
          (transfer_data_ptr->current_alloc_type == L1_DYNAMIC_ALLOC))
      {
        if (transfer_data_ptr->pending_dynamic_tbf_ptr)
        {
          transfer_data_ptr->pending_dynamic_tbf_ptr->sched_msgs = transfer_data_ptr->dynamic_tbf_ptr->sched_msgs;
        }

      }else
      {
        if (transfer_data_ptr->pending_dynamic_tbf_ptr)
        {
          transfer_data_ptr->pending_dynamic_tbf_ptr->sched_msgs=0;
        }
      }

      /*As part of Re-config, Gl1 connects both UL and DL Configs if there is any pending Structure.
       Gl1 also needs to send the PH_CONNECT_IND accordingly*/

      if((transfer_data_ptr->dl_st != L1_GPRS_INVALID_FN) && 
         (transfer_data_ptr->ul_st != L1_GPRS_INVALID_FN)&&
         (transfer_data_ptr->pending_dynamic_tbf_ptr)&&
         (transfer_data_ptr->pending_dl_tbf_ptr)&&
         (transfer_data_ptr->tbf_est_type != UPLINK_DOWNLINK_TBF_ESTABLISHED))
      {
            MSG_GERAN_HIGH_1_G("RECONFIG: TBF EST %d", transfer_data_ptr->tbf_est_type);
            transfer_data_ptr->tbf_est_type = UPLINK_DOWNLINK_TBF_ESTABLISHED;
      }

      /* check if the uplink has also changed i.e by receiving ts reconfig or dynamic*/
      if (transfer_data_ptr->pending_dynamic_tbf_ptr)
      {
        transfer_data_ptr->dynamic_tbf_ptr = transfer_data_ptr->pending_dynamic_tbf_ptr;

        /* Restore current dl_tbf pointer if there is one */
        if (transfer_data_ptr->pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
        {
          if(transfer_data_ptr->dl_tbf_ptr != NULL )
          {
            if(transfer_data_ptr->current_alloc_type == L1_DYNAMIC_ALLOC)
            {
            /* Ensure previous DL-TFI and TN are reset as only applicable if 
               we are currently assigned a DL
             */
              transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi
                                 = GPL1_TFI_INVALID;
              transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc
                                 = GPL1_INVALID_TN;
            }
            else
            {
              transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi
                        = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
              transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc
                        = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
            }

          }
          transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr = transfer_data_ptr->dl_tbf_ptr;
        } else if (transfer_data_ptr->pending_alloc_type == L1_DYNAMIC_ALLOC)
        {
          /* reconfiguring to UL TBF only so clear dl tbf pointer */
          transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr = NULL;
          /*
           * Need to clear the pending_dl_tbf_ptr. There shouldn't
           * be a pending_dl_tbf_ptr for a pending Dynamic Allocation.
           */
          transfer_data_ptr->pending_dl_tbf_ptr = NULL;
        }

        /* Improve the reaction time of L1
         * Add 1 tdma frame.
         */
        transfer_data_ptr->dynamic_tbf_ptr->starting_time = ADD_FN( FN, 1 );

        /* Align to next block boundary */
        gpl1_gprs_chk_blk_bndry(&transfer_data_ptr->dynamic_tbf_ptr->starting_time);
		
        if(transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap)
        {
          (void)gpl1_g2x_ta_update_table_field( G2X_TA_PRI_TBF_TS_CHANGE, g_client,transfer_data_ptr->dynamic_tbf_ptr->starting_time,2, gas_id);           
          gpl1_g2x_ta_scheduler(gas_id);
        }



        /* clear out pending dynamic pointer */
        transfer_data_ptr->pending_dynamic_tbf_ptr = NULL;

      }

      /* check if downlink has changed  */
      if (transfer_data_ptr->pending_dl_tbf_ptr)
      {
        if( transfer_data_ptr->dl_tbf_ptr != NULL )
        {
          if(transfer_data_ptr->current_alloc_type == L1_DYNAMIC_ALLOC)
          {
            /* Ensure previous DL-TFI and TN are reset as only applicable if 
               we are currently assigned a DL
             */
            transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_tfi
                               = GPL1_TFI_INVALID;
            transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc
                               = GPL1_INVALID_TN;

          }
          else
          {
            transfer_data_ptr->pending_dl_tbf_ptr->previous_dl_tfi
                               = transfer_data_ptr->dl_tbf_ptr->dl_tfi;
            transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc
                               = transfer_data_ptr->dl_tbf_ptr->ts_allocation;
          }

        }
        transfer_data_ptr->dl_tbf_ptr = transfer_data_ptr->pending_dl_tbf_ptr;
        /* setup downlink pointer within dynamic structure */
        transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr = transfer_data_ptr->dl_tbf_ptr;

        /* Improve the reaction time of L1
         * Add 1 tdma frame.
         */
        transfer_data_ptr->dl_tbf_ptr->tbf_start_time = ADD_FN( FN, 1 );

        /* Align to next block boundary */
        gpl1_gprs_chk_blk_bndry(&transfer_data_ptr->dl_tbf_ptr->tbf_start_time);


        /* clear out pending downlink pointer */
        transfer_data_ptr->pending_dl_tbf_ptr = NULL;


      }

      gpl1_gprs_setup_cont_ta_params(gas_id);

      /* swap over to new downlink parameters & maybe uplink params */
      transfer_data_ptr->current_alloc_type = transfer_data_ptr->pending_alloc_type;


      /* Check to see if frequency parameters have changed as part of this reallocation */
      if (transfer_data_ptr->pending_frequency_info_ptr)
      {
        transfer_data_ptr->frequency_info_ptr = transfer_data_ptr->pending_frequency_info_ptr;
        transfer_data_ptr->pending_frequency_info_ptr=NULL;
      }

      if (transfer_data_ptr->current_alloc_type == L1_DYNAMIC_ALLOC)
      {
        if (transfer_data_ptr->dl_tbf_ptr)
        {
          transfer_data_ptr->dl_tbf_ptr = NULL;
          MSG_GERAN_LOW_0_G("PL1 - set dl_tbf_ptr to NULL");
        }
      }

      if(l1_state == L1_TRANSFER_MODE)
      {
        /* process dynamic allocation */
        MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_dynamic_tbf from gpl1_gprs_handle_reconfig");
#ifdef FEATURE_GSM_WTR_HOP
        gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
        (void)gpl1_gprs_control_dynamic_tbf(
                                             transfer_data_ptr->dynamic_tbf_ptr,
                                             transfer_data_ptr->frequency_info_ptr,
                                             mode, gas_id
                                           );
        MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
        gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif
      }

    } /* End of case dynamic or dynamic + downlink */
    break;


    /* case for doing only single block re-allocation UL
     * Required for ARAC procedures in EGPRS
     */
    case L1_SINGLE_BLOCK_ALLOC:
    {

      /* Check that the RRBP's are deleted */
      gpl1_gprs_reset_rrbp_enteries(gas_id);

      if(transfer_data_ptr->pending_single_tbf_ptr)
      {

        /* swap over to new allocation */
        transfer_data_ptr->current_alloc_type = transfer_data_ptr->pending_alloc_type;

        /*swap over to new single block allocation */
        transfer_data_ptr->single_block_tbf_ptr = transfer_data_ptr->pending_single_tbf_ptr;

        transfer_data_ptr->pending_single_tbf_ptr = NULL;
        transfer_data_ptr->pending_alloc_type = L1_NULL_ALLOC;

        /* sanity check to ensure we are not setting up a NULL pointer */
        if(transfer_data_ptr->pending_frequency_info_ptr)
        {
          /* set frequency info ptr to point at frequency info_1  */
          transfer_data_ptr->frequency_info_ptr = transfer_data_ptr->pending_frequency_info_ptr;

          transfer_data_ptr->pending_frequency_info_ptr = NULL;
        }else
        {
          MSG_GERAN_ERROR_0_G(" PL1: Null pend freq_ptr!! ");

          /* keep current frequency defn */
        }

        if(l1_state == L1_TRANSFER_MODE)
        {
          MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_single_blk from gpl1_gprs_handle_reconfig");
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
          (void)gpl1_gprs_control_single_blk(transfer_data_ptr->single_block_tbf_ptr,
                                           transfer_data_ptr->frequency_info_ptr,
                                           mode, gas_id );
          MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif
        }

        /* Configure the EGPRS quality measurements module.
         * This is due to a SINGLE BLOCK CONFIG MSG hence only
         * bep period 2 is supplied.
         */
        gpl1_egprs_scell_qual_meas_pacch_config( INVALID_MEAS_MODE,
                                                 0x0,
                                                 transfer_data_ptr->
                                                 single_block_tbf_ptr->bep_period2, gas_id );
      }else
      {
        MSG_GERAN_ERROR_0_G("PL1: Null pend single_tbf_ptr!!");
        transfer_data_ptr->pending_alloc_type = L1_NULL_ALLOC;
      }

    break;
    } /* End of case single block alloc */


  default:
    MSG_GERAN_ERROR_1_G("PL1: Incorrect pending alloc type %d",transfer_data_ptr->pending_alloc_type);
    break; /* Do nothing */

  }   /* end of switch on pending allocation type */

  return(idle);
}


/*===========================================================================

FUNCTION  GPL1_GPRS_HANDLE_TBF_COMPLETE()

DESCRIPTION
  Controls releasing TBF's when PCA RRBP has been sent
DEPENDENCIES


RETURN VALUE
 None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_tbf_complete(transfer_data_T *transfer_data_ptr, gas_id_t gas_id)

{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /*  Ensure the PCA has been transmitted before we release the TBF */
  if (transfer_data_ptr->pca_sent)
  {

    MSG_GERAN_HIGH_2_G(" rel [%d] [%d]",transfer_data_ptr->release_pending,transfer_data_ptr->pending_alloc_type);
    /* Is a full or partial release pending */
    if ((transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL) ||
        (transfer_data_ptr->release_pending == L1_FULL_RELEASE_DL) ||
        (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL_DL))
    {

      if ((transfer_data_ptr->pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC) &&
          (transfer_data_ptr->release_pending == L1_FULL_RELEASE_UL))
      {
        /* Issue command to release ul  */
        transfer_data_ptr->pending_alloc_type = L1_DOWNLINK_ALLOC;
#ifdef FEATURE_GSM_DTM
        if(l1_tskisr_blk->l1_state == L1_DTM_MODE)
        {
          /* ----------------------------------------------------------------------------
             Set event for Reconfiguring the Packet TBF(s)
             --------------------------------------------------------------------------*/
          l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG,gas_id);

        }else if(l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
        {
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
        }
#else
        l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif
      } else if ((transfer_data_ptr->pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC) &&
                 (transfer_data_ptr->release_pending == L1_FULL_RELEASE_DL))
      {
        /* Issue command to release downlink */
        transfer_data_ptr->pending_alloc_type = L1_DYNAMIC_ALLOC;

#ifdef FEATURE_GSM_DTM
        if(l1_tskisr_blk->l1_state == L1_DTM_MODE)
        {
          /* ----------------------------------------------------------------------------
             Set event for Reconfiguring the Packet TBF(s)
             --------------------------------------------------------------------------*/
          l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG,gas_id);

        }else if(l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
        {
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
        }
#else
        l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif

      } else
      {
#ifdef FEATURE_GSM_DTM
        if(l1_tskisr_blk->l1_state == L1_DTM_MODE)
        {
          /* ----------------------------------------------------------------------------
             Set event for Returning to L1_DEDICATED_MODE as
             All TBF's are now finished
             --------------------------------------------------------------------------*/
          l1_set_dtm_command_event(L1_DTM_TRANSFER_STOP,gas_id);
#ifdef  FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
             gpl1_gprs_reset_tuneaway_and_ncell_data( gas_id );
#endif /* FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

        }else if(l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
        {
        /*----------------------------------------------------------------------------
           Stop SCE can be called in ISR context, but start SCE has to be called in
          task context since it is CPU intensive
          ----------------------------------------------------------------------------*/
          l1_sc_stop(gas_id);
          l1_scdrv_call_in_task( gpl1_gprs_setup_start_idle, (void *)0 , gas_id );
        }
#else
        /*----------------------------------------------------------------------------
           Stop SCE can be called in ISR context, but start SCE has to be called in
          task context since it is CPU intensive
          ----------------------------------------------------------------------------*/
        l1_sc_stop(gas_id);
        l1_scdrv_call_in_task( gpl1_gprs_setup_start_idle, (void *)0 , gas_id );
#endif


      }

      transfer_data_ptr->pca_sent = FALSE;

    } else if ((transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_DL) ||
               (transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_UL))
    {
      /**** Validate the partial release. ****/

      /* If we are releasing the DL then ensure the pending_alloc_type is set to
       * dynamic allocation.
       *
       * If we are releasing the UL then ensure the pending_alloc_type is set to
       * downlink allocation.
       */
      if (transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_DL)
      {
        if ((transfer_data_ptr->pending_alloc_type != L1_DYNAMIC_ALLOC) && (transfer_data_ptr->dl_st == L1_GPRS_INVALID_FN))
        {
          MSG_GERAN_HIGH_2_G("alloc type chgd. rel_p %d p_alloc %d",transfer_data_ptr->release_pending,transfer_data_ptr->pending_alloc_type);
          transfer_data_ptr->pending_alloc_type = L1_DYNAMIC_ALLOC;
        }
      } else if (transfer_data_ptr->release_pending == L1_PARTIAL_RELEASE_UL)
      {
        if (transfer_data_ptr->pending_alloc_type != L1_DOWNLINK_ALLOC)
        {
          MSG_GERAN_HIGH_2_G("alloc type chgd. rel_p %d p_alloc %d",transfer_data_ptr->release_pending,transfer_data_ptr->pending_alloc_type);
          transfer_data_ptr->pending_alloc_type = L1_DOWNLINK_ALLOC;
        }
      }

      /* Issue command to release ul or downlink and clear flag */
#ifdef FEATURE_GSM_DTM
        if(l1_tskisr_blk->l1_state == L1_DTM_MODE)
        {
          /* ----------------------------------------------------------------------------
             Set event for Reconfiguring the Packet TBF(s)
             --------------------------------------------------------------------------*/
          l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG,gas_id);

        }else if(l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
        {
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
        }
#else
        l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#endif
      transfer_data_ptr->pca_sent = FALSE;
    }
  }

}

/*===========================================================================

FUNCTION  GPL1_GPRS_CONTROL_TRANSFER

DESCRIPTION
  This function controls transfer mode. Depending upon which allocation type
  has beem ordered by MAC, this function calls l1_control_single_tbf() for
  scheduling NPL1 for fixed allocation

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_control_transfer(l1_mode_command_T  mode, gas_id_t gas_id)
{
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

  transfer_data_T *transfer_data_ptr;
  boolean         idle = FALSE;
  running_avg_struct *ra_ptr;
  idle_data_T * idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  ra_ptr        = &(l1_idle_data->campedon_cell_pwr_meas_avg);

  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

    if((((transfer_data_ptr->dl_st != L1_GPRS_INVALID_FN) || (transfer_data_ptr->ul_st != L1_GPRS_INVALID_FN))
         ||((transfer_data_ptr->dynamic_tbf_ptr != NULL)&&(transfer_data_ptr->dynamic_tbf_ptr->allow_one_rb_gap == TRUE))
         ||((transfer_data_ptr->dl_tbf_ptr != NULL)&&(transfer_data_ptr->dl_tbf_ptr->allow_one_rb_gap == TRUE))
         ||((l1_transfer_data[gas_id].pending_dynamic_tbf_ptr != NULL)&&(l1_transfer_data[gas_id].pending_dynamic_tbf_ptr->allow_one_rb_gap == TRUE)))
        && (frame_counters[gas_id].FNmod26 == 21))

    {
      /* dl_st and ul_st represents starting time which is always at block boundary
         starting time  = Curr_FN+4+align to block boundary
         If there is starting time because of TBF establishment or
         TS reconfiguration at Blk 21-24, then don't schedule G2X
      */

      l1_sc_set_skip_g2x_pkt_txfr(TRUE,gas_id);
    }

  /* send MAC_CONNECT_IND to MAC at start time */
  gpl1_gprs_notify_start(transfer_data_ptr, gas_id);

  /***************************************************/
  /* Stateless handling of L1_UPDATE_PARAMS from GRR */
  /***************************************************/
  if( gprs_l1_idle_data->update_pending )
  {
    /* The signal MPH_GPRS_L1PARAM_UPDATE_REQ has been received update now */
    gpl1_gprs_update_L1_params(gas_id);

    /* Clear flag now update has occurred  */
    gprs_l1_idle_data->update_pending = FALSE;
  }

  switch (l1_tskisr_blk->sub_state)
  {

  case L1_TRANSFER_INIT:
    {
        /* If some measurements have been taken in 51 then initialise with the results  */
        if (l1_running_avg_is_full(ra_ptr))
        {
          gl1_power_meas_type pwr_meas_type = PRX_MEAS;
          dBx16_T divrx_power = GL1_INVALID_POWER_LEVEL;
          /*  Initialise with the serving cell measurements collected in 51 IDLE  */
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
          if (gl1_get_MSRD_ctl_nv(gas_id) || gl1_get_IMRD_ctl_nv(gas_id)) 
          {
            pwr_meas_type = PRX_DIVRX_MEAS;
            divrx_power = (dBx16_T)l1_serving_cell_meas_ptr->divrx_power;
          }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

          l1_running_avg_stuff_buf(&(gprs_serv_cell_meas_ptr->rla_p_buffer),
                                   (dBx16_T)l1_serving_cell_meas_ptr->rx_power,
                                   divrx_power,
                                   pwr_meas_type,
                                   gas_id);
        }

      /*  Set for TRANSFER mode measurements */
      l1_running_avg_set_avg_len(&gprs_serv_cell_meas_ptr->rla_p_buffer, GPRS_SERV_CELL_TRANSFER_AVG_LEN, gas_id);

      gl1_hw_set_ftm_power_store_init(TRUE,gas_id);

      /*  Set up reporting period for 5 seconds to initialise */
      gprs_serv_cell_meas_ptr->rla_p_count_transfer = GPRS_SC_RLA_P_COUNT_TRANSFER_CONT;

      /* Initialize PTM AGC, prime with current value from BCCH AGC
       * and enable Slew rate limiting
       */
      sys_algo_agc_init(&transfer_data_ptr->packet_transfer_agc,idle_data_ptr->campedon_cell_agc.pwr_dBm_x16);
      sys_algo_agc_enable_slew_rate_limiting(&transfer_data_ptr->packet_transfer_agc);
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
      sys_algo_agc_init(&transfer_data_ptr->packet_transfer_div_agc,idle_data_ptr->campedon_cell_agc_divrx.pwr_dBm_x16);
      sys_algo_agc_enable_slew_rate_limiting(&transfer_data_ptr->packet_transfer_div_agc);
#endif
      MSG_GERAN_HIGH_2_G(" PTM AGC INIT %d [%d]",idle_data_ptr->campedon_cell_agc.pwr_dBm_x16/16,gl1_get_FN( gas_id ));

#ifdef FEATURE_G2X_TUNEAWAY
      /* to avoid RX window is stuck in closed state, open rx window here */
      gl1_msgi_pdch_open_rx_win(gas_id);
      gl1_msgi_ptcch_open_rx_win(gas_id);
#endif

      switch (transfer_data_ptr->current_alloc_type)
      {
      /* case for doing only dynamic allocation UL */
      case L1_DYNAMIC_ALLOC:
        {

          /* set fixed_tbf_ptr to point at fixed_tbf_1 (current structure)*/
          transfer_data_ptr->dynamic_tbf_ptr = &(transfer_data_ptr->dynamic_tbf_1);

          /* Set downlink tbf_pointer to NULL as this is a single UL TBF */
          transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr = NULL;

          /* set frequency info ptr to point at frequency info_1 (current structure)*/
          transfer_data_ptr->frequency_info_ptr = &(transfer_data_ptr->frequency_info_1);

          MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_dynamic_tbf from gpl1_gprs_control_transfer");
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
          idle = gpl1_gprs_control_dynamic_tbf(
                                              transfer_data_ptr->dynamic_tbf_ptr,
                                              transfer_data_ptr->frequency_info_ptr,
                                              mode, gas_id
                                              );
          MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif

          if ( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
          {
            /* Configure the EGPRS qual meas module.
             * bep period2 is supplied in the msg, the qual
             * meas module would not pay attention to it unless bep
             * meas are enabled.
             */
             gpl1_egprs_scell_qual_meas_pacch_config( INVALID_MEAS_MODE,
                                                      0x0,
                                                      transfer_data_ptr->
                                                      dynamic_tbf_ptr->bep_period2, gas_id );

            /* Configure the inerference measurements module */
            gpl1_egprs_scell_interf_meas_config( INVALID_MEAS_MODE, gas_id );
          }
          break;
        } /* End of case Dynamic alloc */

        /* case for doing only single block allocation UL */
      case L1_SINGLE_BLOCK_ALLOC:
        {

          /* Check that the RRBP's are deleted */
          gpl1_gprs_reset_rrbp_enteries(gas_id);

          /* set frequency info ptr to point at frequency info_1  */
          transfer_data_ptr->frequency_info_ptr =
          &(transfer_data_ptr->frequency_info_1);

          /* setup single block tbf ptr to point to single block tbf 1 */
          transfer_data_ptr->single_block_tbf_ptr =
                                       &(transfer_data_ptr->single_block_tbf_1);

          MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_single_blk from gpl1_gprs_control_transfer");
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
          (void)gpl1_gprs_control_single_blk(transfer_data_ptr->single_block_tbf_ptr,
                                             transfer_data_ptr->frequency_info_ptr,
                                             mode, gas_id);
          MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif

          /* Configure the EGPRS quality measurements module.
           * This is due to a SINGLE BLOCK CONFIG MSG hence only
           * bep period 2 is supplied.
           */
           gpl1_egprs_scell_qual_meas_pacch_config( INVALID_MEAS_MODE,
                                                    0x0,
                                                    transfer_data_ptr->
                                                    single_block_tbf_ptr->bep_period2, gas_id );
          break;
        } /* End of case single block alloc */

      case L1_DOWNLINK_ALLOC:
        {

          /* set dl_tbf_ptr to point at dl_tbf_1 (current structure)*/
          transfer_data_ptr->dl_tbf_ptr = &(transfer_data_ptr->dl_tbf_1);

          /* Udpate the dl_st that can be used for conflict mgmt in gbta */
          if( transfer_data_ptr->starting_time != L1_GPRS_INVALID_FN )
          {
            transfer_data_ptr->dl_st = transfer_data_ptr->starting_time;
          }

          /* set frequency info ptr to point at frequency info_1 (current structure)*/
          transfer_data_ptr->frequency_info_ptr = &(transfer_data_ptr->frequency_info_1);

          /* set up dynamic pointer to access downlink allocation */
          transfer_data_ptr->dynamic_tbf_ptr  = &(transfer_data_ptr->dynamic_tbf_1);

          /* preset all USF's to unallocated since we are dl tbf only */
          memset((transfer_data_ptr->dynamic_tbf_1.usf_value),GPL1_INVALID,sizeof(uint8[8]) );

          /* Disable USF granulairty as we are DL TBF only */
          transfer_data_ptr->dynamic_tbf_ptr->usf_granularity=FALSE;

          /* preset number of timeslots to zero to stop dynamic tx's being scheduled */
          transfer_data_ptr->dynamic_tbf_1.nts = 0;

          /* set up downlink pointer within dynamic structure */
          transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr = transfer_data_ptr->dl_tbf_ptr;

          /* Check that all RRBP's are in the future */
          gpl1_gprs_delete_oldest_frame_num(gl1_get_FN( gas_id ), gas_id);

          /* reset the previous dl tfi */
          transfer_data_ptr->dl_tbf_ptr->previous_dl_tfi= GPL1_TFI_INVALID;
          transfer_data_ptr->dl_tbf_ptr->previous_dl_ts_alloc = GPL1_INVALID_TN;


          MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_dynamic_tbf from gpl1_gprs_control_transfer");
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
          idle = gpl1_gprs_control_dynamic_tbf(
                                              transfer_data_ptr->dynamic_tbf_ptr,
                                              transfer_data_ptr->frequency_info_ptr,
                                              mode, gas_id
                                              );
          MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
          gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif

          if ( TBF_MODE_EGPRS == transfer_data_ptr->egprs_or_gprs_tbf )
          {
            /* Configure the EGPRS quality measurements module.
             * This is due to a DL TBF CONFIG MSG hence both
             * link qual meas mode and bep period are supplied.
             */
             gpl1_egprs_scell_qual_meas_pacch_config( transfer_data_ptr->dl_tbf_ptr->link_qual_meas_mode,
                                                      transfer_data_ptr->dl_tbf_ptr->ts_allocation,
                                                      transfer_data_ptr->dl_tbf_ptr->bep_period2, gas_id );

            /* Configure the inerference measurements module */
            gpl1_egprs_scell_interf_meas_config( transfer_data_ptr->dl_tbf_ptr->link_qual_meas_mode, gas_id );
          } /* End of if tbf assignment is EGPRS */
          break;
        } /* End of case downlink allocation */

      default:
        MSG_GERAN_ERROR_1_G("PL1: Incorrect alloc type %d",transfer_data_ptr->current_alloc_type);
        break;
      } // end of switch on current allocation type

      break;
    } // end of case for TRANSFER_INIT

  case L1_TRANSFER_RECONFIG:
    {
      /* work out what the pending tbf will be */
      idle = gpl1_gprs_handle_reconfig(transfer_data_ptr,
                                            mode,
                                            l1_tskisr_blk->l1_state, gas_id );
#ifdef FEATURE_GPRS_GBTA
      /* Useful when coming out of DTM */
      if( !idle  &&
         (gl1_msg_get_multi_sim_standby_mode()) &&
          gl1_gbta_get_nv_gbta_support(gas_id) &&
          GBTA_ALLOWED_CAN_BE_SET(gas_id) &&
          (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) &&
          IS_GBTA_INACTIVE() &&
         (!gl1_drdsds_enabled(OTHER_GAS_ID(gas_id))) // TRM now has tech specfic dr status
         )
      {
        SET_GBTA_STATUS(GRM_GBTA_ALLOWED);
        /* For this SUB we are not interested in IDLE state vars */
        grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);

        MSG_GERAN_MED_0_G("GL1 GBTA : GBTA ALLOWED L1_TRANSFER_RECONFIG");
      }
      else
      {
         MSG_GERAN_MED_1_G("GL1 GBTA : L1_TRANSFER_RECONFIG gbta state %d",GET_GBTA_STATE());
      }
#endif  /*FEATURE_GPRS_GBTA */

      break;
    } /* End of case L1_RECONFIG */

#ifdef FEATURE_GPRS_PS_HANDOVER
  case L1_PSHO_RECONFIG: /* 75 */
    {
      MSG_GERAN_MED_1_G("ITP PSHO RECONFIG l1 state %d",l1_tskisr_blk->l1_state);

      /* work out what the pending tbf will be */
      idle = gpl1_gprs_handle_reconfig(transfer_data_ptr,
                                       mode,
                                       l1_tskisr_blk->l1_state, gas_id );

      break;
    } /* End of case L1_PSHO_RECONFIG */
#endif

#ifdef FEATURE_GPRS_PS_HANDOVER

  case  L1_PSHO_IN_PROGRESS: /* @@OG */

#endif

  case L1_DYNAMIC_1:
  case L1_DYNAMIC_2:
    {
      MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_dynamic_tbf from gpl1_gprs_control_transfer");
#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
      idle = gpl1_gprs_control_dynamic_tbf(transfer_data_ptr->dynamic_tbf_ptr, transfer_data_ptr->frequency_info_ptr,mode,
                                           gas_id);
      MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif
      break;
    }


  case L1_DOWNLINK_1:
  case L1_DOWNLINK_2:
    {

      MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_dynamic_tbf from gpl1_gprs_control_transfer");
#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
      idle = gpl1_gprs_control_dynamic_tbf(transfer_data_ptr->dynamic_tbf_ptr, transfer_data_ptr->frequency_info_ptr,mode,
                                           gas_id);
      MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif

      break;
    } /* End of substate case downlink1 or downlink2 */

  case  L1_SINGLE_BLK_TX:
    {
      MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_single_blk from gpl1_gprs_control_transfer");
#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
      idle = gpl1_gprs_control_single_blk(transfer_data_ptr->single_block_tbf_ptr,
                                        transfer_data_ptr->frequency_info_ptr,
                                        mode, gas_id );
      MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif
      break;
    }

  case L1_SINGLE_BLK_RX:
    {
      MSG_GERAN_LOW_0_G("GPRS_HOP_DBG Execute gpl1_gprs_control_single_blk from gpl1_gprs_control_transfer");
#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
#endif
      idle = gpl1_gprs_control_single_blk(transfer_data_ptr->single_block_tbf_ptr,
                                        transfer_data_ptr->frequency_info_ptr,
                                        mode, gas_id );
      MSG_GERAN_LOW_1_G("GPRS_HOP_DBG FN mod 26: %d", frame_counters[gas_id].FNmod26);
#ifdef FEATURE_GSM_WTR_HOP
      gl1_msg_wtr_hop_state_machine(NULL, gas_id);
#endif
      break;
    }



  default:
    MSG_GERAN_ERROR_1_G(" Incorrect PL1 substate %d",l1_tskisr_blk->sub_state);
    break; // do nothing

  } // end of switch on l1_tskisr_blk->sub_state

  return(idle);
}

/*===========================================================================

FUNCTION  GPL1_GPRS_GET_EGPRS_UL_DATA

DESCRIPTION
  This function checks the RLC UL buffer, the MAC UL buffer and the UL
  dummy control buffer and  returns a pointer to the message which has the
  highest priority. The Priority is ordered from highest to lowest as MAC,
  RLC and the uplink dummy control.

  The function also determines the coding scheme to use for transmitting the
  UL message.

DEPENDENCIES
  None

RETURN VALUE
  pointer to the message to transmit

SIDE EFFECTS
  None

===========================================================================*/
geran_pdu_priority_t gpl1_gprs_get_egprs_ul_data( gl1_defs_coding_type*     coding_scheme,
                                  gl1_defs_puncturing_type  puncturing_scheme[2],
                                  uint8*                    msg_data[3],
                                  gas_id_t                  gas_id,
                                  boolean                   single_block_alloc)
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean           check_data_fifo   = TRUE;
  transfer_data_T*  transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  geran_pdu_priority_t           priority = GPDU_LOW_PRIORITY;

  /* First check to see if L1 has queued any MAC PACCH radio blocks
   * to be sent.
   */
#ifdef FEATURE_GPRS_GBTA
   if(IS_GBTA_ACTIVE() &&
      FALSE != transfer_data_ptr->mac_ul_ctrl_available)
   {
     MSG_GERAN_MED_0_G("GL1 GBTA PACCH DELAYED");
   }
#endif /*FEATURE_GPRS_GBTA */

  if(  ( FALSE != transfer_data_ptr->mac_ul_ctrl_available )
     &&( NULL != transfer_data_ptr->mac_ul_ctrl_buff_ptr)
 #ifdef FEATURE_GPRS_GBTA
     && (! IS_GBTA_ACTIVE())   // not schedule PACCH when GBTA is active to avoid cancellation.
 #endif /*FEATURE_GPRS_GBTA */
    )
  {
    mac_ul_ctrl_buffer_T*  mac_ul_buff_ptr = transfer_data_ptr->mac_ul_ctrl_buff_ptr;

    if(  ( mac_ul_buff_ptr->data_valid )
       &&( mac_ul_buff_ptr->pending_msgs )
      )
    {
#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
      while(  ( mac_ul_buff_ptr->pacch_data_out_ptr != mac_ul_buff_ptr->pacch_data_in_ptr )
            &&( 0 != mac_ul_buff_ptr->pending_msgs )
            &&( FALSE != check_data_fifo )
           )
      {
        /* Look for valid messages to hand in to the msg layer */
        if( FALSE != mac_ul_buff_ptr->pacch_data_out_ptr->valid )
        {


#endif
          /* PACCH FIFO now loaded, so stop lower priority RLC data overwriting data
           * to be Txd
           */

          check_data_fifo = FALSE;


////// LLOKS WRONG TO ME
          priority            =  TRUE;

          /* Assign pointer to next PACCH message for transmission */
          msg_data[0] = mac_ul_buff_ptr->pacch_data_out_ptr->control_block;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
          #error code not present
#endif
#endif

          /* if we are using dynamic allocation, update the scheduled message count
           * as this is used to resubmit pacch msgs which are not transmitted
           */
          switch( transfer_data_ptr->current_alloc_type )
          {
          case  L1_DYNAMIC_ALLOC:
          case  L1_DYNAMIC_DOWN_ALLOC:

            mac_ul_buff_ptr->sched_msgs++;
            break;

          default:
            mac_ul_buff_ptr->sched_msgs=0;
            break;
          }

          /* descrement pending count as a pacch message has now been scheduled for
           * transmission
           */
          if( 0 != mac_ul_buff_ptr->pending_msgs )
          {
            mac_ul_buff_ptr->pending_msgs--;
          }

          if( 0 == mac_ul_buff_ptr->pending_msgs )
          {
            /* if no more mac data available to transmit then unset mac data
             * availability flag
             */
            transfer_data_ptr->mac_ul_ctrl_available = FALSE;

            mac_ul_buff_ptr->data_valid = FALSE;
          }

          /* set coding scheme to coding scheme 1 as this is a PACCH */
          *coding_scheme = GL1_DEFS_CS1_CODING;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

        } /* End of if out ptr->valid is not FALSE */

#endif

        /*
         * Increment out pointer for next access.
         * Check if end of buffer has been reached,
         * and set to first element if it has
         */
        if ( ++(mac_ul_buff_ptr->pacch_data_out_ptr) >
             &(mac_ul_buff_ptr->pacch_data[(MAX_NUM_UL_PACCH_MSGS -1)]) )
        {
          mac_ul_buff_ptr->pacch_data_out_ptr = mac_ul_buff_ptr->pacch_data;
        }

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

      } /* End of while out ptr != in ptr && pend msgs != 0 && check_data_fifo != FALSE */

#endif

    } /* End of if mac_ul_buff_ptr->data_valid is TRUE
       * && pending msgs != 0.
       */
  }   /* End of if mac_ul_ctrl_available is not FALSE
       * && mac_ul_ctrl_buff_ptr is not NULL.
       */

  if( FALSE != check_data_fifo )
  {

    if( transfer_data_ptr->rlc_ul_data_available == TRUE )
    {
      grlc_ul_fifo_access_block_t  rlc_config_params;
      grlc_ul_fifo_op_result_t     rlc_result;

      /* assign shortcut pointer's for use in function */
      rlc_ul_status_T*  rlc_ul_status_ptr = &(transfer_data_ptr->rlc_ul_status);

      /* Read value of defer_abort_flag from pl1_store */
      const gpl1_ps_store_T*  gpl1_store_ptr = gpl1_read_xfer_isr_store( gas_id );

      /* Check if a deferred abort is required and access to the fifo is allowed, i.e. no
       * CS or TLLI change is in progress
       */
      if (gpl1_store_ptr->defer_abort_flag)
      {
        /* handle deferred abort's here as scheduled tx's have not been transmitted*/
        rlc_config_params.access_mode = ABORT_MODE;

        if ( grlc_ul_fifo_access_block(gas_id, &rlc_config_params) == UL_FIFO_ACCESS_OK)
        {
          GPL1_CLEAR_ABORT_FLAG( gas_id );
        }

      } /* end of defer flag */

      /* Check to see if RLC has any data to send */
      if ((transfer_data_ptr->current_alloc_type == L1_DYNAMIC_ALLOC) ||
          (transfer_data_ptr->current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) )
      {
        /* Only nead to do a read mode as we are doing dynamic alloc */
        rlc_config_params.access_mode = READ_MODE;
      }

      rlc_result = grlc_ul_fifo_access_block(gas_id, &rlc_config_params);

      if ( rlc_result == UL_FIFO_ACCESS_OK)
      {
        /* Inform RLC of data present condition
        */
        grlc_ul_data_detect(gas_id,TRUE);

        if ((transfer_data_ptr->current_alloc_type ==L1_DYNAMIC_ALLOC) ||
            (transfer_data_ptr->current_alloc_type == L1_DYNAMIC_DOWN_ALLOC) )
        {
          /* only store ack state if we are doing dynamic allocation */
          rlc_ul_status_ptr->ul_desc_ptr->ack_state = rlc_config_params.ack_state;
        }

        rlc_ul_status_ptr->ul_desc_ptr->blk_ptr = rlc_config_params.blk_ptr;

        /* Check tbf mode to work out where data to tx is GPRS or EGPRS */
        if (transfer_data_ptr->egprs_or_gprs_tbf == TBF_MODE_EGPRS)
        {
          /* Assign pointer to current data block */
          msg_data[0] = rlc_config_params.blk_ptr->elm.egprs.hdr;
          msg_data[1] = rlc_config_params.blk_ptr->elm.egprs.msg1;
          msg_data[2] = rlc_config_params.blk_ptr->elm.egprs.msg2;

          /* Assign coding scheme value and puncturing schemes  */
          *coding_scheme = (gl1_defs_coding_type)(rlc_config_params.blk_ptr->elm.egprs.mcs + MSC_CS_OFFSET);

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
#endif

          if( NULL != puncturing_scheme )
          {
            puncturing_scheme[0] =  (gl1_defs_puncturing_type)(rlc_config_params.blk_ptr->elm.egprs.ps1);
            puncturing_scheme[1] =  (gl1_defs_puncturing_type)(rlc_config_params.blk_ptr->elm.egprs.ps2);
          }
          else
          {
            MSG_GERAN_ERROR_0_G(" NULL puncturing_scheme pointer ");
          }

        } else
        {
          /* Assign pointer to current data block */
          msg_data[0] = rlc_config_params.blk_ptr->elm.gprs.block;

          /* Assign coding scheme value  */
          *coding_scheme = (gl1_defs_coding_type)rlc_config_params.blk_ptr->elm.gprs.cs;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
#endif

          /* No need to set puncturing schemes for CS1-4, NPL1 only needs them for MCS1-9 */
        }

        /* only priotise First and Retx , Unack mode which are always 1st Tx */
        if ( ((rlc_config_params.ack_state == TXA_PTX) || (rlc_config_params.ack_state == TXA_NACK)) ||
             ((transfer_data_ptr->rlc_mode == RLC_UNACK) && (rlc_config_params.ack_state == TXA_PACK)) 
           )
        {
           priority = rlc_config_params.pdu_prio; 
        }


        /* Increment pointer for next RLC message */
        rlc_ul_status_ptr->ul_desc_ptr++;

#ifdef  FEATURE_GSM_GPRS_MSC33
        if(rlc_ul_status_ptr->ul_desc_ptr > &rlc_ul_status_ptr->ul_desc[MAX_NUM_UL_PACCH_MSGS - 1])
#else
        if(rlc_ul_status_ptr->ul_desc_ptr > &rlc_ul_status_ptr->ul_desc[3])
#endif
        {
          rlc_ul_status_ptr->ul_desc_ptr = &rlc_ul_status_ptr->ul_desc[0];
          //MSG_GERAN_ERROR_0_G("rlc_ul_status_ptr->ul_desc_ptr out of bounds - reset pointer");
        }

        /* Increment the number of RLC messages for this Radio block */
#ifdef  FEATURE_GSM_GPRS_MSC33
        if (++rlc_ul_status_ptr->num_msgs > PL1_MULTISLOT_TX)
#else
        if (++rlc_ul_status_ptr->num_msgs > 4)
#endif
        {
          MSG_GERAN_ERROR_0_G("rlc ul status fifo overflow!");
        }

      } /* End of if rlc_result == UL_FIFO_ACCESS_OK */
      else if (rlc_result == UL_FIFO_EMPTY)
      {
        if( (l1_idle_data->layer1_extra_gprs_sys_info.ext_utbf_nodata == TRUE)
           && ( FALSE == single_block_alloc ))
        {
          /* Check if we don't have to send dummy blocks when RLC FIFO is empty */
          msg_data[0] = NULL;
          *coding_scheme = GL1_DEFS_CS1_CODING;
          MSG_GERAN_HIGH_0_G("EXT ULTBF NO DATA ! ");

        }
        else
        {
          if (grlc_is_xfer_active(gas_id))
          {
            /* Inform RLC of no data condition
            */
            grlc_ul_data_detect(gas_id,FALSE);
          }

          /* Send RLC_PH_READY_TO_SEND */
          grlc_send_ph_ready_to_send_ind(gas_id);

          /* FIFO has problem or is empty assign pointer to UL dummy control data */
          msg_data[0] = transfer_data_ptr->dummy_control_block;

          /* Set coding scheme to coding scheme 1 as this is a PACCH */
          *coding_scheme = GL1_DEFS_CS1_CODING;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
#endif

        }
      } /* End of if rlc_result == UL_FIFO_EMPTY */
      else
      {
        if((l1_idle_data->layer1_extra_gprs_sys_info.ext_utbf_nodata == TRUE)
            && ( FALSE == single_block_alloc ))
        {
          /* Check if we don't have to send dummy blocks when RLC FIFO is busy */
          msg_data[0] = NULL;
          *coding_scheme = GL1_DEFS_CS1_CODING;
          MSG_GERAN_HIGH_1_G("EXT ULTBF NO DATA ! ul_fifo_state=%d",rlc_result);

        }
        else
        {
          if (grlc_is_xfer_active(gas_id))
          {
            /* Inform RLC of no data condition
            */
            grlc_ul_data_detect(gas_id,FALSE);
          }

          /* Send RLC_PH_READY_TO_SEND */
          grlc_send_ph_ready_to_send_ind(gas_id);

          /* FIFO has problem or is empty assign pointer to UL dummy control data */
          msg_data[0] = transfer_data_ptr->dummy_control_block;

          /* Set coding scheme to coding scheme 1 as this is a PACCH */
          *coding_scheme = GL1_DEFS_CS1_CODING;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
#endif
        }
      } /* End of else rlc_result != UL_FIFO_ACCESS_OK / UL_FIFO_EMPTY */
    }   /* transfer_data_ptr->rlc_ul_data_available == TRUE */
    else
    {
     if( (l1_idle_data->layer1_extra_gprs_sys_info.ext_utbf_nodata == TRUE)
          && ( FALSE == single_block_alloc ))
      {
        /* Check if we don't have to send dummy blocks when RLC FIFO is empty */
        msg_data[0] = NULL;
        *coding_scheme = GL1_DEFS_CS1_CODING;
        MSG_GERAN_HIGH_0_G("EXT ULTBF NO DATA ! ");
      }
      else
      {
        /* Otherwise send a dummy UL control message as no MAC or RLC data */
        msg_data[0] = transfer_data_ptr->dummy_control_block;

        /* Set coding scheme to coding scheme 1 as this is a PACCH */
        *coding_scheme = GL1_DEFS_CS1_CODING;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
#if  DEBUG_GPL1_PACCH_FIFO_DELETION
#error code not present
#endif
#endif
      }
    } /* End of else transfer_data_ptr->rlc_ul_data_available != TRUE */
  }   /* End of if check_data_fifo is TRUE */

  return priority;
}


/*===========================================================================

FUNCTION  GPL1_GPRS_REORDER_TN

DESCRIPTION
  This function accepts the timeslot bitmap description from MAC which is
  in the format TN0 = bit7 and TN7 = bit 0 and re orders them into a format
  which PL1 can use where TN0  = bit0 and TN7 = bit7
DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/

uint8 gpl1_gprs_reorder_tn(uint8 *mac_tn, uint8 *pl1_tn)
{
  uint16 i;
  uint8  nts = 0;

  *pl1_tn = 0;
  /* change order of TN from MAC, old order TN0 = Bit 7
   * new order TN0 = Bit 0
   */
  for (i=0; i< MAX_TN; i++)
  {
    *pl1_tn |= (((*mac_tn & 0x80)>> 7) << i);

    /* increment timeslot count if allocated */
    if ( ((*mac_tn & 0x80) >> 7) ==1)
    {
      nts++;
    }

    *mac_tn<<=1;
  }

  return(nts);

}

/*===========================================================================

FUNCTION  gpl1_gprs_transfer_send_ftn

DESCRIPTION
  This function configures a set of counter's and flags for formatting
  the parameters for sending the function gl1_frame_tick_notification() to NPL1.

  The parameters to this function are pointers to static variables defined in
  l1_control_fixed_tbf(), l1_control_dl_tbf() & l1_control_dynamic_tbf()


DEPENDENCIES
  l1_control_fixed_tbf() l1_control_dl_tbf() & l1_control_dynamic_tbf()


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_transfer_send_ftn(
                                uint8 *pacch_pdtch_ul_blk_start_cnt,
                                uint8 *pacch_pdtch_dl_blk_start_cnt,
                                l1_mode_command_T mode, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gl1_msg_frame_tick_type  ftn;

  transfer_data_T *transfer_data_ptr =
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  ftn.next_frame_is_ul_block_start = FALSE;
  ftn.next_frame_is_dl_block_start = FALSE;

  if (*pacch_pdtch_ul_blk_start_cnt == 0)
  {
    ftn.next_frame_is_ul_pdch_block_start = TRUE;

    /* set counter to invalid as block start has elapsed */
    *pacch_pdtch_ul_blk_start_cnt = GPL1_INVALID;
  }
  else
  {
    ftn.next_frame_is_ul_pdch_block_start = FALSE;
  }

  if (*pacch_pdtch_dl_blk_start_cnt == 0)
  {
    ftn.next_frame_is_dl_pdch_block_start = TRUE;

    /* set counter to invalid as block start has elapsed */
    *pacch_pdtch_dl_blk_start_cnt = GPL1_INVALID;
  }
  else
  {
    ftn.next_frame_is_dl_pdch_block_start = FALSE;
  }

  gpl1_gprs_update_ptcch_ftn(&ftn, gas_id);

  /* Set block starts for surround cell activities */
  ftn.next_frame_is_acq_ncell_block_start = gl1_is_ncell_block_start(gas_id);
  ftn.next_frame_is_pwr_meas_block_start = gl1_is_pwr_meas_block_start(gas_id);

  /* Perform serving cell measurements */
  ftn.next_frame_is_int_meas_block_start = gpl1_gprs_serving_cell_meas(mode,gas_id);

  /*
   * Notify NPL1 of frame tick.
   */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( gl1_msg_get_multi_sim_mode() )
  {
    /* In Dual sim operation only perform the frame tick when firmware has been downloaded
       this implies that the lock is granted too */
    if ( l1_tskisr_blk->firmware_loaded == TRUE )
    {
      gl1_msg_frame_tick_notification ( &ftn,gas_id );
    }
    else
    {
      MSG_GERAN_MED_0_G("No FW so no FTN");
    }
   }
   else
   {
     gl1_msg_frame_tick_notification ( &ftn,gas_id );
   }
#else
   gl1_msg_frame_tick_notification ( &ftn,gas_id );
#endif

  /* Clear flags */
  gl1_set_ncell_block_start(FALSE,gas_id);
  gl1_set_pwr_meas_block_start(FALSE,gas_id);

  /*
   * Decrement all block start counters
   */

  if (*pacch_pdtch_ul_blk_start_cnt != GPL1_INVALID)
    (*pacch_pdtch_ul_blk_start_cnt)--;

  if (*pacch_pdtch_dl_blk_start_cnt != GPL1_INVALID)
    (*pacch_pdtch_dl_blk_start_cnt)--;

  gpl1_gprs_handle_tbf_complete(transfer_data_ptr, gas_id );

}   /* End of function */

/*===========================================================================

FUNCTION  gpl1_gprs_send_L1_MAC_TBF_REL_CONFIRM

DESCRIPTION
  Creates the above named message and sends to the GS_QUEUE_MAC
  queue or GS_QUEUE_TEST.

  The function sets the tbf_release_type parameter to either
  UPLINK_TBF_RELEASE_CONF or DOWNLINK_TBF_RELEASE_CONF depending
  on which TBF is to be release.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_send_l1_mac_tbf_rel_confirm(release_T tbf_type, gas_id_t gas_id)
{

  l1_mac_sig_t   message_buffer,*message;

  message = &message_buffer;

#if defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER ) && defined ( FEATURE_GERAN_PTM_ERR_RECOVERY )
    #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH && FEATURE_GERAN_PTM_ERR_RECOVERY*/

  switch (tbf_type)
  {
  case L1_FULL_RELEASE_UL:
  case L1_PARTIAL_RELEASE_UL:

    if (l1_transfer_data[gas_id].release_in_progress)
    {
      if (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_DOWN_ALLOC)
      {
        l1_transfer_data[gas_id].pending_alloc_type = L1_DOWNLINK_ALLOC;
        l1_transfer_data[gas_id].release_in_progress_delay = TRUE;
      } else
      {
        l1_transfer_data[gas_id].release_in_progress = FALSE;
      }
    }

    /* update the current_alloc type premature , to avoid reassigment
    ** changing the pending alloc type to dyn_down_alloc, even though there
    ** is no ul assignment CR#: 223317
    */
    if (l1_transfer_data[gas_id].pending_alloc_type == L1_DOWNLINK_ALLOC)
    {
      l1_transfer_data[gas_id].current_alloc_type = L1_DOWNLINK_ALLOC;
    }

    message->msg.tbf_released.tbf_released = UL_TBF_RELEASE_CONF;
    MSG_GERAN_MED_0_G(" REL_CONFIRM UL ");

    /* Generate the log packet */
    gpl1_log_uplink_tbf_data_block_count( &( gpl1_uplink_tbf_data_blk_count[gas_id] ), gas_id );

    /* Reset the contents of the uplink tbf block count */
    gpl1_reset_uplink_tbf_blk_count( gas_id);

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
    gpl1_gprs_recovery_deregister_event(GPL1_UL_RELEASE_EVENT, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

    break;

  case L1_FULL_RELEASE_DL:
  case L1_PARTIAL_RELEASE_DL:

    message->msg.tbf_released.tbf_released =  DL_TBF_RELEASE_CONF;
#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
    MSG_GERAN_MED_1_G(" REL_CONFIRM DL t3192_tfi_match_count %d",l1_transfer_data[gas_id].t3192_tfi_match_count);
#else
    MSG_GERAN_MED_0_G(" REL_CONFIRM DL ");
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */
    /* update the current_alloc type premature , to avoid reassigment
    ** changing the pending alloc type to dyn_down_alloc, even though there
    ** is no dl assignment CR#: 223317
    */
    if (l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_ALLOC)
    {
      l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_ALLOC;
    }

    /* If T3192 is still running then stop it as DL is now released */
    if(grlc_dl_t3192_status(gas_id) == TMR_ACTIVE)
    {
      grlc_dl_stop_t3192(gas_id);
      MSG_GERAN_MED_0_G("PL1: stop T3192");
    }
#ifdef FEATURE_G2X_TUNEAWAY
    if(l1_transfer_data[gas_id].dl_tbf_ptr != NULL)
    {
      l1_transfer_data[gas_id].dl_tbf_ptr->dl_tbf_with_t3192_active = FALSE;
    }
#endif

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
    gpl1_gprs_recovery_deregister_event(GPL1_DL_RELEASE_EVENT, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

    break;

  case L1_FULL_RELEASE_UL_DL:

    /* Generate the log packet */
    gpl1_log_uplink_tbf_data_block_count( &( gpl1_uplink_tbf_data_blk_count[gas_id] ), gas_id );

    /* Reset the contents of the uplink tbf block count */
    gpl1_reset_uplink_tbf_blk_count( gas_id );

    message->msg.tbf_released.tbf_released =  UL_DL_TBF_RELEASE_CONF;
#ifdef FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY
    MSG_GERAN_MED_1_G(" REL_CONFIRM UL+DL t3192_tfi_match_count = %d",l1_transfer_data[gas_id].t3192_tfi_match_count);
#else
    MSG_GERAN_MED_0_G(" REL_CONFIRM UL+DL ");
#endif /* FEATURE_GSM_GPRS_DL_REL_HDR_DEC_ONLY */
    /* If T3192 is still running then stop it as DL is now released */
    if(grlc_dl_t3192_status(gas_id) == TMR_ACTIVE)
    {
      grlc_dl_stop_t3192(gas_id);
      MSG_GERAN_MED_0_G("PL1: stop T3192");
    }
#ifdef FEATURE_G2X_TUNEAWAY
    if(l1_transfer_data[gas_id].dl_tbf_ptr != NULL)
    {
      l1_transfer_data[gas_id].dl_tbf_ptr->dl_tbf_with_t3192_active = FALSE;
    }
#endif

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
    gpl1_gprs_recovery_deregister_event(GPL1_UL_RELEASE_EVENT, gas_id);
    gpl1_gprs_recovery_deregister_event(GPL1_DL_RELEASE_EVENT, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

    break;

  default:
    MSG_GERAN_ERROR_0_G("PL1: REL CNF ERROR");
  }

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id = L1_MAC_TBF_REL_CONFIRM;
  message->msg.tbf_released.gas_id = gas_id;
  /* Send the message */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);

}

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
/*===========================================================================

FUNCTION  gpl1_conflict_check_for_ccch
DESCRIPTION
  Perform conflict if PCH/ CCCH can be scheduled.
  as with this feature both can be scheduled simultaneously
DEPENDENCIES


RETURN VALUE
  TRUE - if allowed

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_conflict_check_for_ccch (gas_id_t gas_id)
{
   if ( !( ! l1_sci_is_op_in_progress_gbta(gas_id) &&
           ! l1_sci_meas_in_progress(gas_id) &&
            gpl1_gprs_check_tbf_suspension( gl1_get_FN(gas_id),L1_SC_DURATION_GBTA,FALSE,gas_id)))
   {
     MSG_GERAN_HIGH_2_G("PCHXFER - TA not allowed sc_op %d sci_meas %d",l1_sci_is_op_in_progress_gbta(gas_id),l1_sci_meas_in_progress(gas_id));
     return TRUE;
   }

   return FALSE; // No Conflict
} /* gpl1_conflict_check_for_ccch */

#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

/*===========================================================================

FUNCTION  gpl1_gprs_reset_pacch_fifo
DESCRIPTION
  Clears the FIFO in PL1 for storing PACCH UL control blocks
DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_reset_pacch_fifo(gas_id_t gas_id)
{
  if(l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->pending_msgs + l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->sched_msgs)
  {
#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
    uint32  pacch_msg = MAX_NUM_UL_PACCH_MSGS;
#endif

    l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->pending_msgs =0;
    l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->sched_msgs = 0;

    /* clear flag as we have no PACCH data */
    l1_transfer_data[gas_id].mac_ul_ctrl_available = FALSE;

    /* set pointer's to point to beginning of PACCH FIFO */
    l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->pacch_data_in_ptr =
    l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->pacch_data_out_ptr =
    l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->last_pacch_txd_ptr = l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->pacch_data;

#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
    /* Reset the validity boolean */
    while( (pacch_msg--) != 0 )
    {
      l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr->pacch_data[ pacch_msg ].valid = FALSE;
    }
#endif

  }
  MSG_GERAN_LOW_1_G(" PL1: UL PACCH FIFO RESET [%d]",gl1_get_FN( gas_id ));

}
/*===========================================================================

FUNCTION  gpl1_reset_uplink_tbf_blk_count( )

DESCRIPTION
  Used for gathering log packet statistics.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void  gpl1_reset_uplink_tbf_blk_count( gas_id_t gas_id )
{
    /* Set the number of data and dummy blocks to zero */
    gpl1_uplink_tbf_data_blk_count[gas_id].number_rlc_data_blks   = 0;
    gpl1_uplink_tbf_data_blk_count[gas_id].number_dummy_ctrl_blks = 0;
}


/*===========================================================================

FUNCTION  gpl1_increment_uplink_tbf_data_blk_count( )

DESCRIPTION
  Used for gathering log packet statistics.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void  gpl1_increment_uplink_tbf_data_blk_count( gas_id_t gas_id )
{
    /* Increment the number of rlc data blocks transmitted by one */
    gpl1_uplink_tbf_data_blk_count[gas_id].number_rlc_data_blks++;
}


/*===========================================================================

FUNCTION  gpl1_increment_uplink_tbf_dummy_blk_count( )

DESCRIPTION
  Used for gathering log packet statistics.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void  gpl1_increment_uplink_tbf_dummy_blk_count( gas_id_t gas_id )
{
    /* Increment the number of mac dummy blocks transmitted by one */
    gpl1_uplink_tbf_data_blk_count[gas_id].number_dummy_ctrl_blks++;
}


#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )
/*===========================================================================

FUNCTION  gpl1_send_mac_ph_cs_connect_ind

DESCRIPTION
  Creates the MAC_PH_CS_CONNECT_IND message and sends it to the GS_QUEUE_MAC
  queue or GS_QUEUE_TEST.

  Fills in the absolute circuit switched dtm timeslot.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_send_mac_ph_cs_connect_ind( uint8  cs_timeslot,gas_id_t gas_id )
{
  /* Instantiate a message buffer */
  l1_mac_sig_t  message_buffer, *message;

  message = &message_buffer;

  /* Set up the message header and type */
  message->task_id = MS_MAC_L1;
  message->sig_id  = MAC_PH_CS_CONNECT_IND;
  /* Fix Klocwork initialisation error */
  message->link.next_ptr = NULL;
  message->link.prev_ptr = NULL;

  /* Fill in the cs_timeslot information,
   * This is needed by MAC for multislot class checking.
   */
  message->msg.cs_connect_ind.cs_timeslot = cs_timeslot;
  message->msg.cs_connect_ind.gas_id = gas_id;
  /* Send the message */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);

}
#endif

#ifdef FEATURE_GSM_COEX
#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION  gpl1_gprs_get_arfcns

DESCRIPTION
  This function determines the ARFCNs to be used on the specified frames
  according to the defined frequency hopping parameters.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_gprs_get_arfcns(int16 start, uint16 increment, uint16 num_arfcns, ARFCN_T arfcns[], gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  frequency_information_T* channel_data = l1_tskisr_blk->current_params.L1Data.pTransfer_data->frequency_info_ptr;
  uint32 i, fn = ADD_FN(gl1_get_FN(gas_id), start);

  if(channel_data!=NULL)
  {
    for(i=0;i<num_arfcns;i++)
    {
      arfcns[i] = gpl1_gprs_get_ARFCN(channel_data, 0, ADD_FN(fn ,i) );
    }
  }
}

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION      gpl1_set_dtm_assig_status

DESCRIPTION   sets the flag true(when entering DTM) or false(when existing DTM).

RETURN VALUE  None

===========================================================================*/
void gpl1_set_dtm_assig_status(boolean dtm_assignemnt, gas_id_t gas_id)
{
  l1_transfer_data[gas_id].slam_dtm_assign    = dtm_assignemnt;
}

/*===========================================================================

FUNCTION      gpl1_get_dtm_assig_status

DESCRIPTION   gets the value of the DTM assignment flag

RETURN VALUE  TRUE if DTM assignment message received 

===========================================================================*/
static boolean gpl1_get_dtm_assig_status(gas_id_t gas_id)
{
  return l1_transfer_data[gas_id].slam_dtm_assign;
}
#endif /* FEATURE_GSM_DTM */


/*===========================================================================

FUNCTION  gl1_get_slammed_ps_dl_alloc

DESCRIPTION
function computes slammed downlink PS timeslot indices for DTM and TRANSFER MODE, using the downlink slot bit map and the usf array and stores it in a dl array 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_get_slammed_ps_dl_alloc(boolean *slammed_ps_dl_alloc, downlink_tbf_information_T *tbf_dl, dynamic_tbf_information_T *tbf_dynamic, 
                               uint8 *lowest_dl_OTA_timeslot, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  uint8 ps_tn,lowest_tn=0,lowest_tn_found=FALSE;
  uint8 slot_index=0,dl_tn_alloc=0,cs_tn;
  transfer_data_T *transfer_data;
  transfer_data = l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  dl_tn_alloc   = tbf_dl->ts_allocation;

  memset(slammed_ps_dl_alloc, 0, sizeof(boolean)*GL1_DEFS_SLOTS_IN_FRAME);

  if(l1_dedicated_data_ptr != NULL)
  {
    cs_tn = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TN;
  }

  for(ps_tn=0;ps_tn<GL1_DEFS_SLOTS_IN_FRAME;ps_tn++)
  {
    if( ((tbf_dynamic !=NULL) && (tbf_dynamic->usf_value[ps_tn] != GPL1_INVALID)) || (0 != ( dl_tn_alloc & 0x1 )))
    {
      if(lowest_tn_found == FALSE)
      {
        lowest_tn                =  ps_tn;
        lowest_tn_found          =  TRUE;
        *lowest_dl_OTA_timeslot  = lowest_tn;
      }

      if(gpl1_get_dtm_assig_status(gas_id))
      {
        lowest_tn = SUB_MOD(cs_tn, GL1_DEFS_TN_2, GL1_DEFS_SLOTS_IN_FRAME);
      }
      slot_index              = SUB_MOD(ps_tn, lowest_tn, GL1_DEFS_SLOTS_IN_FRAME);
      slammed_ps_dl_alloc[slot_index] = TRUE;
    }
    dl_tn_alloc>>=1;
  }
}

/*===========================================================================

FUNCTION  gpl1_set_coex_params_update

DESCRIPTION
function sets the coex params update flag

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gpl1_set_coex_params_update(boolean coex_params_update, gas_id_t gas_id)
{
  coex_update_params[gas_id] = coex_params_update;
}

/*===========================================================================

FUNCTION  gl1_get_slammed_ps_ul_alloc

DESCRIPTION
function computes slammed  uplink PS timeslot indices for DTM and TRANSFER MODE using the usf array, and stores them in a ul array 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_get_slammed_ps_ul_alloc(boolean *slammed_ps_ul_alloc, dynamic_tbf_information_T* tbf_dynamic, uint8 *lowest_dl_OTA_timeslot, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  uint8 ps_tn;
  uint8 slammed_slot=0, lowest_tn = 0, cs_tn = 0;
  transfer_data_T *transfer_data;
  transfer_data = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  memset(slammed_ps_ul_alloc, 0, sizeof(boolean)*GL1_DEFS_SLOTS_IN_FRAME);

  if(l1_dedicated_data_ptr != NULL)
  {
    cs_tn = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.TN;
  }

  for(ps_tn=0;ps_tn<GL1_DEFS_SLOTS_IN_FRAME;ps_tn++)
  {
    if(tbf_dynamic->usf_value[ps_tn] != GPL1_INVALID)
    {
      if(gpl1_get_dtm_assig_status(gas_id))
      {
        lowest_tn = SUB_MOD(cs_tn, GL1_DEFS_TN_2, GL1_DEFS_SLOTS_IN_FRAME);
      }
      else
      {
        lowest_tn = *lowest_dl_OTA_timeslot;
      }

      slammed_slot              = SUB_MOD(ps_tn, lowest_tn, GL1_DEFS_SLOTS_IN_FRAME);

      /* Add check to prevent buffer overflow */
      if (slammed_slot > (GL1_DEFS_SLOTS_IN_FRAME - 1))
      {
         MSG_GERAN_ERROR_3_G("slammed slot out of range, %d, %d, %d", slammed_slot, lowest_tn, cs_tn);
         slammed_slot = 0;
      }

      slammed_ps_ul_alloc[slammed_slot] = TRUE;
    }
  }
}
#endif /* FEATURE_GSM_COEX_SW_CXM*/
#endif /*FEATURE_GSM_COEX*/
/*===========================================================================

FUNCTION  gl1_check_no_dl_rx_slots

DESCRIPTION
Check if no of total slots >3

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

boolean gl1_check_no_dl_rx_slots(gas_id_t gas_id)
{
   transfer_data_T*  transfer_data_ptr = &l1_transfer_data[gas_id];

  if(transfer_data_ptr != NULL)
  {

    if(transfer_data_ptr->dynamic_tbf_ptr != NULL)
    { /*For UL TBF*/
      if((transfer_data_ptr->dynamic_tbf_ptr->nts) > 3)
      {  
        return TRUE;
      }

      /*For UL +DL TBF*/
      if((transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr != NULL) && 
        ((transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->nts) + (transfer_data_ptr->dynamic_tbf_ptr->nts) > 3))
        {
          return TRUE;
        }       
    }

    /*For DL TBF*/
    if((transfer_data_ptr->dl_tbf_ptr != NULL) && ((transfer_data_ptr->dl_tbf_ptr->nts) > 3))
    {
       return TRUE;
    }

  }

  return FALSE;
}

