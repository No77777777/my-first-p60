/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     L 1 DUAL SIM MODE FIND BCCH STATE PROCESSING

GENERAL DESCRIPTION


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dual_sim_find_bcch_if.c#2 $
$DateTime: 2021/08/12 06:17:45 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
12/08/21   sal       CR3010819 Ensure ongoing MCPM state update in DPC is MCPM_INIT before setting main command to ENTER_NULL_COMMAND .
19/07/18   ng        CR2271185 Abort MCPM INIT when change IRAT neighbor list is obtained in BCCH state.
20/02/18   shm      Change Request 2192580 : KW P1 errors on TA.3.0 fix
07/02/18   hd        CR2181437 Report the BCCH as found when BSIC doesn't match after SCH decode
10/05/17   sk        CR2024440 Add an explicit intlock for DivRx meas buffer update
11/08/17   rc        CR2138578 ML logging - GL1 Changes
08/09/16   snjv      CR1058525 Dont release TRM when calling RnN with updated reason
12/09/16   zf        CR1058059 Abort MCPM INIT when change UE mode.
25/10/16  mn         CR1080163 Send MPH_ACQUISITION_STATE_IND when retained for nominal priority
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
03/04/17   br        CR2027088 Band based ASDIV support during ACQ
02/06/17   sk        CR1116408 Do not order RESCAN as a part of ASDIV probe if GL1 is indicated to abort or deactivate stack.
27/12/16   sk        CR1092325 GL1 to report zero frequencies when sending MPH_POWER_SCAN_CNF for DS Abort    
06/09/15   br        CR1063442 FR 38050: AsDiv in Acquisition for CSFB in multisimode.
26/02/16   js        CR981060 Rework on CR961866
02/02/16   js        CR970891 Reverting CR961866 & CR965869
27/02/16   sk        CR981473 Correction of handling DS ABORT INDICATION message, gpl1_gprs_handle_ds_abort_ind() called
14/01/16   js        CR961866: Check for internal GRM status while handling DS_ABORT
28/10/15   sk        CR922637 Ensure GL1 doesnt release-and-rerequest for SET_PRIORITY_IND if a GTA gap is in progress
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
27/09/15   dv        CR915710 For MPH_BECCH_DECODE_LIST_REQ ensure only 
                              required bands are registered with TRM 
02/09/15   zf        CR905869: Set trm_status to NULL after grm release
26/08/15   sjw       CR896531: send MPH_ACQUISITION_STATE_IND if already granted a chain
07/08/15   cjl       CR877672 Handle priority inversion for TRM_RESELECT use cases
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons 
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types
06/07/15   nk        CR866473 handle DS ABORT INDICATION message in Idle mode.
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
24/06/15   ap        CR866727 TRM U:support of Acquisition Cell Select as DR and Acquisition as SR 
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
30/03/15   ap        CR811753 FR22272 - TRM Unification - GL1 changes to support 
                                        SysInfo priority inversion during early camping 
11/03/15   zc        CR806341 Release asdiv switch on req_n_notify for G acquisition
20/02/15   sjw       CR793380: adaptive TRM durations for acquisition mode
19/02/15   mko       CR776845 Don't wait for ISR  tick when moving to inactive state
24/01/15   mko       CR785110 GL1 must not register unlock callback if DS_ABORT is received during the FW_LOAD state
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs. 
07/01/15   sp        CR764166: Ensure GRM status is granted only to allow release of lock in idle/decouple.
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   ws        CR768376 Q6 compile warning fixes
20/01/14   js        CR592998 Adding one more frame delay to process DS_ABORT_IND
16/10/14   ss        CR740692 call grm_get_granted instead of checking trm_grant_status in gpl1_gprs_handle_priority_ind
24/09/14   cah       CR729408 Updates to reduce number of TRM API calls in Gl1.   
02/09/14   cja       CR717966 G2L stuck in SCHEDULE LTE as update list had made index WCDMA
23/08/14   nk        CR713833 Do not move main_command to ENTER_NULL_COMMAND in complete_sds_abort() if is it already L1_DEACTIVATE_CMD 
                              as we are trying to process and send confirmation for STOP_MODE_GSM_REQ
20/06/14   jj        CR656872 fix critical gl1 kw error  
28/05/14   ap        CR671971: Change SGLTE mode on reception of UE_MODE_CHANGE_REQ
22/08/14   sp        CR685373: Maintain requested cell (BSIC) from RR and report failure if BSIC decoded is not matching in ISR SCH CNF.
11/08/14   sjw       CR706459: use L1_COMPLETE_DS_ABORT for race condition between abort and release
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
01/07/14   cs        CR688003 Runtime enable for DR-DSDS
30/05/14   ws        CR657674 Data + MMS Phase 1
20/05/14   smd       CR662851: Release and request RF device again for RACH in SGLTE+G
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
08/05/14   sp        CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
10/05/14   rc        CR662550: Back out changes made in CR655548
06/05/14   smd       CR655548: Release and request RF device again for RACH in SGLTE+G
15/04/14   ka        CR611185: Ensure information from the MPH_START_GPRS_IDLE_MODE_REQ is used in RACH
09/04/14   ap        CR640266 L1 is unable to sleep because X2G reselect in progress is always indicated
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
18/03/14   ap        CR629844 SGLTE BPLMN support when G1 in PS call in G+G - PS Hopping
18/03/14   ap        CR614949 FR 19574: Performing LTE/TDS measurements while GERAN is in connected mode using the second WTR
28/02/14   sjw       CR620569: adaptive unlock callback for acquisition
04/02/14   sjw       CR611206: corrected ordering of TRM calls within priority inversion handler
05/12/13   am        CR581771 Ensure frequency correction to re-selection cell is always applied
12/01/14   sjw       CR592154: use new TRM reason for ACQUISITION_MAX
08/01/14   cah       CR597414: fix for missing handling of L1_GSTMR_DEREG_SWAP_TO_VSTMR
14/12/13   sjw       CR589502: Install different unlock callback for change of acq priority
13/12/13   ap        CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
28/11/13   cgc       CR581186 Desense SYS_PARAM_MIN_ACCEPTABLE_RX_LVL in SGLTE mode                     
28/11/13   sjw       CR578873: Priority Inversion Management for Multi-SIM (PIMMS)
13/11/13   ap        CR562878 FR 17482: Page Blocking Management in Multi-SIM Single Radio Devices
24/07/13   ss        CR515896  Set wcdma_list_update_inprogress to TRUE in msg_handler routine
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
27/09/13   cgc       CR523552 add support for FR 16939 L+DSDS LTA/QTA qta_in_progress LTE_GENERIC_QTA
23/05/13   pg        CR460555: Enable 3GPP Rel10 RACH Power Reduciton feature
23/09/13   ab        CR543349 DSDS -  Perform TRM_RELEASE before doing RANE when handling L1_DS_ABORT_IND
17/09/13   sp        CR542963: Remove Reset of release pending at gprs idle mode req.
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
21/08/13   npt       CR531633 - Remove FEATURE_DIME_MODEM dependencies
21/06/13   cs        Major Triton TSTS Syncup
06/06/13   cs        TSTS Updates
25/05/13   am        CR488358 NULL check for l1_Frequency_List
01/05/13   pg        CR478903: read all normal and extended BCCH blocks during
                     MPH_SELECT_SPECIFIC_BCCH_REQ / MPH_GPRS_RESELECTION_REQ
18/02/13   ab        CR 453181 : don't reset Packet Transfer state variables if in DSDx mode
02/05/13   zf        CR480969: Fix G+G DSDS camping
01/04/13   am        CR469877 - CR413664+CR428401+CR415240+CR415240
15/03/13   ws        CR 463978 - Fix compile errors for QTF environment
18/02/13   ab        CR 453181 : AYSYNC SIM should not reset SYNC SIM Transfer state variables.
10/02/13    ip       CR448394
18/05/12   mc        DSDS CR359187:To avoid further collisions with QTA gaps, Set L1_DECOUPLE_MULTI_SIM_IDLE_COMMAND when
                     denying RACH due to QTA in progress.
14/02/13   pjr       CR452272 added support to set multisim mode at activation time
26/09/11   ap        CR 308923: DSDS GL1 Task can be stuck waiting on signal after MPH_DS_L1_ABORT_IND
21/09/11   ap        DSDS CR306482 - Ensure ISR runs and actions MPH_L1_DS_ABORT_IND before
                     continuing in task
27/01/11   dmb       The following RR<->L1 messages are removed (not used):
                     MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
09/07/10   nt        Move ASYNC to SYNC sim swap to ISR context
30/06/10   sjw       Introduction of GRM framework
19/05/10   njt       Ensure that L1 is awake for MPH_STOP_GSM_MODE_REQ processing
===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "ms.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "l1.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "l1_drx.h"
#include "l1_smscb.h"
#include "l1_os.h"
#include "l1_sc.h"

#include "ftm.h"

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_task.h"
#include "gpl1_gprs_serving_cell.h"

#include "gpl1_dual_sim.h"
#include "geran_dual_sim_g.h"

#include "gpl1_grm_intf.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif 

#ifdef FEATURE_GL1_ASDIV_ACQ
#include "gl1_asdiv.h"
#endif

/* Union of all the messages sent in response to MPH_L1_DS_ABORT_IND */
typedef union
{
  IMH_T                          message_header;
  mph_select_specific_bcch_cnf_T select_specific_bcch_cnf;
  mph_reselection_cnf_T          reselection_cnf;
  mph_decode_bcch_list_cnf_T     decode_bcch_list_cnf;
  mph_power_scan_cnf_T           power_scan_cnf;
} l1_ds_messages_u;

/*=======================EXTERNAL DATA=========================*/
extern boolean last_acq_was_list;

extern gpl1_g2x_ta_disable_rx_tx_state g2x_ta_seq_trm_state[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION     gpl1_gprs_generate_abort_response

DESCRIPTION  Creates and sends a FAILURE response that GRR needs from GL1

DEPENDENCIES GL1 must have been rcvd MPH_L1_DS_ABORT_IND(RR_DS_ABORT_TO_NULL)

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
void gpl1_gprs_generate_abort_response(const rr_l1_message_id_enum_T msg_id, const gas_id_t gas_id)
{
  l1_ds_messages_u l1_rr_cmd;
  size_t           msg_siz;
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];

  switch(msg_id)
  {
  case MPH_POWER_SCAN_CNF:
    l1_rr_cmd.power_scan_cnf.suspending = FALSE;
    l1_rr_cmd.power_scan_cnf.invalidate = TRUE;
    l1_rr_cmd.power_scan_cnf.gas_id     = gas_id;
    msg_siz = sizeof(mph_power_scan_cnf_T);
    l1_Frequency_List->results->num_frequencies = 0;
    break;

  case MPH_RESELECTION_CNF:
    l1_rr_cmd.reselection_cnf.BCCH_found    = FALSE;
    l1_rr_cmd.reselection_cnf.state_changed = FALSE;
    l1_rr_cmd.reselection_cnf.gas_id        = gas_id;
    msg_siz = sizeof(mph_reselection_cnf_T);
    break;

  case MPH_DECODE_BCCH_LIST_CNF:
    l1_rr_cmd.decode_bcch_list_cnf.retry_required = FALSE;
    l1_rr_cmd.decode_bcch_list_cnf.suspending     = FALSE;
    l1_rr_cmd.decode_bcch_list_cnf.gas_id         = gas_id;
    msg_siz = sizeof(mph_decode_bcch_list_cnf_T);
    break;

  case MPH_SELECT_SPECIFIC_BCCH_CNF:
    l1_rr_cmd.select_specific_bcch_cnf.BCCH_found = FALSE;
    l1_rr_cmd.select_specific_bcch_cnf.gas_id     = gas_id;
    memset(l1_rr_cmd.select_specific_bcch_cnf.SCH_block, 0, SCH_BLOCK_SIZE);
    msg_siz = sizeof(mph_select_specific_bcch_cnf_T);
    break;

  default:
    msg_siz = 0;
    break;
  }

  /* if a valid message type was detected, construct the primitive now */
  if (msg_siz >= sizeof(IMH_T))
  {
    /* create a L1->RR signal which will be handled by the RR state machines */
    l1_rr_cmd.message_header.message_set = MS_RR_L1;
    l1_rr_cmd.message_header.message_id  = (byte)msg_id;

    /* lint -e{778} */
    PUT_IMH_LEN((msg_siz - sizeof(IMH_T)), &(l1_rr_cmd.message_header));
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

    if (GS_SUCCESS != gs_send_message(GS_QUEUE_RR, (void *)&l1_rr_cmd, FALSE))
    {
      MSG_GERAN_ERROR_0_G("Unable to enqueue message for GRR");
    }
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Unable to generate message 0x%02X", (int)msg_id);
  }

  return;
}

/*===========================================================================

FUNCTION  GPL1_GPRS_COMPLETE_DS_ABORT

DESCRIPTION
  This function completes the processing of the MPH_L1_DS_ABORT_IND

DEPENDENCIES
  gpl1_gprs_handle_ds_abort_ind() must have been called already

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_complete_ds_abort(grm_client_enum_t client_id, 
                                 rr_l1_message_id_enum_T pending_msg, 
                                 gas_id_t gas_id)
{
  gl1_trm_state_t grm_status = grm_get_trm_status(client_id, gas_id);

  if ( GL1_TRM_GRANTED == grm_status ||
       GL1_TRM_ABORTING == grm_status|| 
       GL1_TRM_RETAINED_FOR_ACCESS ==  grm_status
     )
  {
    boolean retain_now;

    /* lock is already granted so retain and prevent further pre-emption */
    /* unless TRM_ABORTING state has already been set in the task buffer */
    GL1_ISR_LOCK(gas_id);
    if (GL1_TRM_ABORTING != grm_status)
    {
      grm_set_frame_duration(COMPLETE_PROCEDURE_DURATION, client_id, gas_id);
      retain_now = TRUE;
    }
    else
    {
      retain_now = FALSE;
    }
    GL1_ISR_UNLOCK(gas_id);

    if (retain_now)
    {
      /* GL1 is now expected to complete its current acquisition procedure */
      /* and then send a normal _CNF response to GRR when it has completed */
      grm_retain_lock(client_id, NULL, gas_id);
      return;
    }
    else
    {
      MSG_GERAN_HIGH_0_G("Completion of DS abort while GL1_TRM_ABORTING");
    }
  }
  else
  {
    /* explicitly wipe any TRM request pending for this client */
    grm_release(client_id, gas_id);
  }

  /* push the state machine back to NULL */
  GL1_ISR_LOCK(gas_id);
  if(l1_tsk_buffer[gas_id].main_command != L1_DEACTIVATE_COMMAND)
  {
    l1_tsk_buffer[gas_id].main_command = L1_ENTER_NULL_COMMAND;
  }
  GL1_ISR_UNLOCK(gas_id);

  /* whichever action took place, give GRR the confirmation it needs */
  gpl1_gprs_generate_abort_response(pending_msg, gas_id);

  if(l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE)
  {
    (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
    (void)rex_wait(GPL1_WAIT_FOR_ISR_TO_GET_FN);
  }

  if(l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE)
  {
    (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
    (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
  }

  return;
}

/*===========================================================================

FUNCTION  GPL1_GPRS_HANDLE_DS_ABORT_IND

DESCRIPTION
  This function handles MPH_L1_DS_ABORT_IND sent by GRR during acquisition

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_ds_abort_ind(mph_l1_ds_abort_ind_T *message_ptr)
{
  if (message_ptr != NULL)
  {
    gas_id_t gas_id = message_ptr->gas_id;

    MSG_GERAN_HIGH_2_G("MPH_L1_DS_ABORT_IND rcvd with cause %d, msg 0x%02X",
                       (int)(message_ptr->abort_type), (int)(message_ptr->pending_msg));

    if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
    {
      /*************************************************
       * RR_DS_ABORT_TO_NULL - GRR wants GL1 to cancel
       * or complete any pending procedure and respond
       * using the normal primitive for that procedure
       *************************************************/
      if (RR_DS_ABORT_TO_NULL == message_ptr->abort_type)
      {
        trm_get_info_return_type trm_info;
        grm_client_enum_t        client_id;
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_trm_state_t trm_status;
#endif
        client_id = gl1_map_gas_id_to_client_id(gas_id);

        if (GL1_TRM_GRANTED == grm_get_trm_status(client_id, gas_id))
        {
          /* client has already been granted the TRM lock for its activity */
          if (message_ptr->complete_granted)
          {
            gl1_complete_ds_abort_t complete_msg;

            /* GRR wants this primitive to run to completion, so */
            /* retain the lock now and avoid further pre-emption */
            GL1_ISR_LOCK(gas_id);
            grm_set_frame_duration(COMPLETE_PROCEDURE_DURATION, client_id, gas_id);
            GL1_ISR_UNLOCK(gas_id);
            grm_retain_lock(client_id, NULL, gas_id);

            /* post a message to the GL1 task to complete this abort processing, in */
            /* case an abort callback fired previously and a release is also queued */
            complete_msg.client = client_id;
            complete_msg.msg_id = message_ptr->pending_msg;
            complete_msg.gas_id = gas_id;

            /* set the inter-task header */
            complete_msg.header.message_set = MS_L1_L1;
            complete_msg.header.message_id  = L1_COMPLETE_DS_ABORT;
            PUT_IMH_LEN(sizeof(gl1_complete_ds_abort_t) - sizeof(IMH_T), &complete_msg);

            (void)L1_isr_send_message(l1_queues[gas_id], &complete_msg, FALSE, gas_id);

            /* exit now: L1_COMPLETE_DS_ABORT does any remaining work in the task */
            return;
          }
          else
          {
            /* simulate an unlock callback to abort the current activity */
            gl1_handle_abort_callback(client_id, TRM_UNLOCK_IMMEDIATELY, 0UL, gas_id);
          }
        }

        /* complete the processing by updating main state machine and responding to GRR */
        gpl1_gprs_complete_ds_abort(client_id, message_ptr->pending_msg, gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        trm_status = grm_get_trm_status(client_id, gas_id);
        if (trm_status == GL1_TRM_CALLBACK_PENDING)
        {
          gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TRM_LOCK_WAIT_TIMEOUT);
        }
#endif

      }
      else
      {
        MSG_GERAN_ERROR_0_G("MPH_L1_DS_ABORT_IND with unknown abort_type");
      }
    }
    else
    {
      MSG_ERROR("GAS ID 0x%02X out of range for MPH_L1_DS_ABORT_IND", (int)gas_id, 0, 0);
    }
  }

  return;
}

/*===========================================================================

FUNCTION  GPL1_GPRS_HANDLE_PRIORITY_IND

DESCRIPTION
  This function handles MPH_SET_PRIORITY_IND sent by GRR during acquisition

DEPENDENCIES
  New priority must have been passed to GRM already by l1_handle_message()

RETURN VALUE
  None

SIDE EFFECTS
  May replace the unlock callback registered for the affected GSM client

===========================================================================*/
static void gpl1_gprs_handle_priority_ind(mph_set_priority_ind_T *message_ptr)
{
  if (message_ptr != NULL)
  {
    trm_get_info_return_type get_granted;
    sys_proc_type_e_type sub_reason = message_ptr->sys_proc_type;
    gas_id_t gas_id = message_ptr->gas_id;

    if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
    {
      volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
      grm_client_enum_t        this_client;
      trm_reason_enum_t        curr_acq_reason;
      timetick_type            acq_duration = timetick_cvt_to_sclk(RESELECTION_DURATION_MS, T_MSEC);

      /* retrieve the TRM reason that the active client last booked, */
      /* which is independent of whether it has been granted a chain */
      l1_tskisr_blk   = &l1_tsk_buffer[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)];

      this_client     = l1_tskisr_blk->client_id;

      curr_acq_reason = grm_gl1_get_current_trm_reason(this_client, gas_id);

      if ( TRM_CHANNEL_MAINTENANCE == curr_acq_reason && 
           GERAN_PRIORITY_INVERTED == message_ptr->new_priority )
      {
        acq_duration = timetick_cvt_to_sclk(RESELECTION_DURATION_MS, T_MSEC);
        curr_acq_reason = TRM_CHANNEL_MAINTENANCE_INV;
      }
      else if (TRM_CHANNEL_MAINTENANCE_INV == curr_acq_reason && 
               GERAN_PRIORITY_NOMINAL == message_ptr->new_priority )
      {
        acq_duration = timetick_cvt_to_sclk(RESELECTION_DURATION_MS, T_MSEC);
        curr_acq_reason = TRM_CHANNEL_MAINTENANCE;
      }
      else if (GRM_ACQUISITION_REASON(curr_acq_reason))
      {
        acq_duration = timetick_cvt_to_sclk(grm_get_acquisition_duration(gas_id), T_MSEC);
        curr_acq_reason = TRM_ACQUISITION;
      }
      else if (GRM_ACQUISITION_DR_REASON(curr_acq_reason))
      {
        acq_duration = timetick_cvt_to_sclk(grm_get_acquisition_duration(gas_id), T_MSEC);
        curr_acq_reason = TRM_ACQUISITION_CELL_SELECT;
      }
      else if (TRM_CHANNEL_MAINTENANCE != curr_acq_reason)
      {
        /* grm_get_reason() has returned an unexpected value, drop out */
        MSG_GERAN_ERROR_1_G("Unexpected TRM reason %d", (int)curr_acq_reason);
        return;
      }

      /* now determine whether the client holds the lock, or is pending it */
      if ( TRM_DENIAL != grm_get_granted( this_client, &get_granted, gas_id ) )
      {
        /* holding a lock, so change the priority if it was for */
        /* acquisition, and no abort callback has been received */
        if ( (TRM_ACQUISITION == curr_acq_reason) || 
             (TRM_ACQUISITION_CELL_SELECT == curr_acq_reason))
        {
          if ((TRUE == l1_tskisr_blk->firmware_loaded) &&
              (GL1_TRM_ABORTING != grm_get_trm_status(this_client, gas_id)))
          {
            grm_retain_lock_for_acquisition(this_client, GRM_ALLOWS_UNLOCK_REQUIRED, curr_acq_reason, sub_reason, gas_id);

            /* if retaining for an inverted priority, update GRR with the state now */
            if ( grm_get_acquisition_priority(gas_id) != GERAN_PRIORITY_NOT_CHANGED) 
            {
               L1_send_MPH_ACQUISITION_STATE_IND(RR_L1_ACQ_RUNNING, grm_get_acquisition_priority(gas_id), gas_id);
            }
          }
          else
          {
            MSG_GERAN_HIGH_0_G("No need to change acquisition lock");
          }
        }
        else if ( TRM_CHANNEL_MAINTENANCE_INV == curr_acq_reason || 
                  TRM_CHANNEL_MAINTENANCE == curr_acq_reason )
        {
          grm_change_priority (this_client,curr_acq_reason,sub_reason,gas_id);
        }
      }
      else if (GL1_TRM_CALLBACK_PENDING == grm_get_trm_status(this_client, gas_id))
      {

        /* Check if PT sub is opening a GTA gap in parallel for this client
           if so do not update the RANE here */
        if( OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)
            &&( this_client== gpl1_gprs_tbf_abort_cb_store_data[OTHER_G_SUB_IN_TRANSFER(gas_id)].last_client ) 
            &&( g2x_ta_seq_trm_state[OTHER_G_SUB_IN_TRANSFER(gas_id)] == G2X_TA_DISABLE_RXTX_TRM_WAIT ) )
        {
          MSG_GERAN_HIGH_0_G("GTA gap is opened by PT sub, do not update the RANE here");
        }
        else		  
        {
			
        trm_resource_enum_t resource;

        /* determine resource and freq parameters */
        resource = gl1_ms_get_trm_resource(gas_id, curr_acq_reason);

#ifdef FEATURE_GSM_QSC_TX_DIV
        /* Request and notify assumes an implicit release, which is happening elsewhere. 
           When this happens, need to also change the ASDIV state when in FIND_BCCH */
        gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_INACTIVE ,gas_id);
#endif

        /* Dont do a release and perform RnN Directly with updated reason
           to preserve freq info used in between */
        grm_request_and_notify_enhanced( this_client,
                                         resource,
                                         acq_duration,
                                         curr_acq_reason,
                                         sub_reason,
                                         GRM_RANE_GPRS_PRIORITY_IND
                                        );

      }
      }
      else
      {
        /* neither holding a chain nor pending a response, so nothing to do */
        MSG_GERAN_MED_1_G("No further action required, TRM status %d",
                          (int)(grm_get_trm_status(this_client, gas_id)));
      }
    }
  }

  return;
}

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
void gpl1_gprs_handle_multi_sim_find_bcch(IMH_T *msg_header)
{
  gas_id_t gas_id;

   /* function l1_handle_idle_mode starts off this mode by initiating
   a power measurement scan of surrounding cells. this terminates
   when the ISR sends a ISR_SCAN_CNF message to the L1 task. */

   if (msg_header->message_set == MS_L1_L1)
   {
      uint16 num_satisfy_threshold_criteria = 0;
      uint16 i;

      switch (msg_header->message_id)
      {
        /********************************************************
         *                                                      *
         *                ISR_SCAN_CNF                          *
         *                                                      *
         ********************************************************/
         case ISR_SCAN_CNF:
         {
           isrtim_scan_cnf_T *message_ptr = (isrtim_scan_cnf_T *)msg_header;
           Frequency_List_T *l1_Frequency_List;
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

           gas_id = check_gas_id(message_ptr->gas_id);
           l1_Frequency_List = l1_Frequency_List_Store[gas_id];
           l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            /* A set of measurements exist in the l1_Frequency_List. */

           /* Check L1_Frequency_List pointer is valid, since MPH_STOP_MODE_REQ
           may have deallocated dynamic memory causing a potential page fault */
           if(l1_Frequency_List == NULL)
           {
             MSG_HIGH("l1_Frequency_List NULL!",0,0,0);
             break;
           }

            /* Calculate the signal strength averages for the list ... */
            L1_normalise_signal_strengths(l1_Frequency_List->length,
                                          l1_Frequency_List->no_of_measurement,
                                          l1_Frequency_List->power_dBm_x16,
                                          l1_Frequency_List->agc
#ifdef GL1_GMSK_MOD_DET
                                          ,l1_Frequency_List->decstat
#endif
                                          );

            /* ... sort it ... */
            num_satisfy_threshold_criteria = L1_sort_signal_strengths(gas_id);

            /* ... copy the results into RR's buffer ... */
            for (i=0; i<num_satisfy_threshold_criteria; i++)
            {
              l1_Frequency_List->results->meas[i].RXLEV_average =
                    L1_map_dBm_x16_to_rxlev(l1_Frequency_List->power_dBm_x16[l1_Frequency_List->sorted_array_indexes[i]]);

#ifdef DEBUG_PSCAN /* Disabled this F3 printout since it is adding 30 ms to our camp on time */
#error code not present
#endif
              l1_Frequency_List->results->meas[i].arfcn = l1_Frequency_List->ARFCN[l1_Frequency_List->sorted_array_indexes[i]];
            }

            l1_Frequency_List->results->num_frequencies = num_satisfy_threshold_criteria;

            l1_Frequency_List->results->num_frequencies = num_satisfy_threshold_criteria;

            /* ... transition to Null Mode ... */
            GL1_ISR_SAVE_LOCK(gas_id);

#ifdef FEATURE_GL1_ASDIV_ACQ              
            if (gl1_asdiv_acq_enabled(gas_id) &&                         /*Asdiv in Acquisition is enabled*/
                !gl1_asdiv_acq_check_rescan(gas_id) &&                   /*A Rescan for AsDiv in Acquisition is not attempted*/
                ((num_satisfy_threshold_criteria != 0) ||                /*If Some Valid Cells are found or Freq. length is within Limits*/
                (l1_Frequency_List->length <= GL1_ASDIV_ACQ_MAX_FREQ_LIST_LEN))&&
                ( l1_tskisr_blk->main_command == L1_NULL_COMMAND)) 
            {
              /* change sub state to decide AsDiv for Acquisition*/
              l1_tskisr_blk->sub_state = L1_ASDIV_ACQ_SWITCH;
              gl1_asdiv_acq_update_params( GL1_ASDIV_ACQ_EV_PSCAN_END,
                                           l1_Frequency_List->results->meas[0].arfcn,
                                           num_satisfy_threshold_criteria,
                                           l1_Frequency_List->length,
                                           gas_id);

              if(0 == num_satisfy_threshold_criteria)
              {
                  L1_send_ISRTIM_SCAN_REQ( L1_FIND_BCCH_COMMAND, gas_id );
              }                  
            }
            else
            {
              if (gl1_asdiv_acq_check_rescan(gas_id)) 
              {
                if(num_satisfy_threshold_criteria ==0)
                {
                  gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXACQ ,gas_id);
				  /*On Entering This State, If scenario is for ReScan then Switch*/
                  gl1_trm_ant_switch_div_set_config(gas_id);
                }
                gl1_asdiv_acq_reset(gas_id);
              }
#endif /*FEATURE_GL1_ASDIV_ACQ*/

              if ( l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND)
              {
                /* ... transition to Null Mode ... */
                l1_tskisr_blk->main_command = L1_ENTER_NULL_COMMAND;
              }
#ifdef FEATURE_GL1_ASDIV_ACQ              
            }
#endif /*FEATURE_GL1_ASDIV_ACQ*/

            l1_tskisr_blk->next_params.L1_confirm_ID = L1_POWER_SCAN_CNF;

            GL1_ISR_SAVE_UNLOCK(gas_id);

         }
         break;


        /********************************************************
         *                                                      *
         *                ISR_SCH_CNF                           *
         *                                                      *
         ********************************************************/

        /* The L1 ISR has found a SCH on the channel or not as indicated
         in  the ISRTIM_FIRST_FCB_CNF message to the L1 task. */

         case ISR_SCH_CNF:
         {
            l1_sch_cnf_T *msg_ptr;
            uint8         BSIC;
            idle_data_T               *l1_idle_data;
            Frequency_List_T          *l1_Frequency_List;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

           msg_ptr = (l1_sch_cnf_T *)msg_header;
           gas_id = check_gas_id(msg_ptr->gas_id);
           l1_idle_data=&l1_idle_data_store[gas_id];
           l1_Frequency_List = l1_Frequency_List_Store[gas_id];
           l1_tskisr_blk = &l1_tsk_buffer[gas_id];
           BSIC = get_BSIC_from_sch_data( msg_ptr->sch_block );

           if ( msg_ptr->success && 
                l1_tskisr_blk->sub_state != L1_ACQUIRING_DURING_RESELECTION && 
                l1_tskisr_blk->x2g_resel_in_progress == FALSE )
           {
             if (l1_tskisr_blk->next_params.bsic_expected != GSM_INVALID_BSIC && 
                 BSIC != l1_tskisr_blk->next_params.bsic_expected)
             {
               MSG_GERAN_ERROR_3_G("SCH decoded with different BSIC -- decoded:%2x, requested=%2x, substate=%d",
                                    BSIC,l1_tskisr_blk->next_params.bsic_expected,
                                    l1_tskisr_blk->sub_state);

               msg_ptr->success = FALSE;
             }
           }

            if (msg_ptr->success)
               {
                  /* A FCH/SCH combination has been found on the given ARFCN */
                  /* Inform upper layers */
                  if (l1_tskisr_blk->sub_state == L1_ACQUIRING_DURING_RESELECTION)
                  {
                       L1_send_MPH_RESELECTION_CNF( TRUE, gas_id );
                  }
                  else
                  {
                       L1_send_MPH_SELECT_SPECIFIC_BCCH_CNF( TRUE,
                                                             msg_ptr->sch_block, gas_id  );

                       /* In Multi-SIM need to have valid client RGS as lock given up */
                       gl1_msg_update_xo_rgs( FALSE, FALSE, gas_id );
                  }
                  /* Start decoding the CCH only */
                  
                  l1_idle_data->campedon_cell_BSIC = BSIC;

                  l1_idle_data->campedon_cell_ARFCN = l1_Frequency_List->ARFCN[l1_Frequency_List->length];
                  l1_idle_data->campedon_cell_agc   = l1_Frequency_List->agc[l1_Frequency_List->length];
                  sys_algo_agc_enable_slew_rate_limiting(&l1_idle_data->campedon_cell_agc);

                  /* Setup default BCCH decoding mask in case we go
                   * to Idle Mode.
                   */
                  l1_idle_data->si_mask        = RR_L1_SI_TYPE_ALL_NORM | RR_L1_SI_TYPE_2_quater_EXT | RR_L1_SI_TYPE_13_EXT;
                  l1_idle_data->BCCH_mask      = gl1_compute_BCCH_mask( l1_idle_data->si_mask );
                  l1_idle_data->ext_BCCH_mask  = gl1_compute_ext_BCCH_mask( l1_idle_data->si_mask );

                  L1_send_ISRTIM_CCH_UNI_DWN_REQ( BSIC,
                                                  l1_Frequency_List->ARFCN[l1_Frequency_List->length],
                                                  l1_idle_data->ext_BCCH_mask,
                                                  &l1_idle_data->campedon_cell_agc, gas_id );
               }
               else
               {
                  if (l1_tskisr_blk->sub_state == L1_ACQUIRING_DURING_RESELECTION)
                  {
                     L1_send_MPH_RESELECTION_CNF( FALSE, gas_id );
                  }
                  else
                  {
                     L1_send_MPH_SELECT_SPECIFIC_BCCH_CNF( FALSE, NULL, gas_id );
                  }
               }
               /* (Re)-Initialize the SCE */
               l1_sc_init(gas_id);
            }
            break;

         default:
            MSG_GERAN_ERROR_1("L1->L1(BCCH) message %d ignored.", msg_header->message_id);
            break;
      } /* end of switch (msg_header->message_id) */
   } /* end of if (msg_header->message_set == MS_L1_L1)  */

   else if (msg_header->message_set == MS_RR_L1)
   {
      switch (msg_header->message_id)
      {

        /********************************************************
         *                                                      *
         *                MPH_L1_DS_ABORT_IND                   *
         *                                                      *
         ********************************************************/
         case MPH_L1_DS_ABORT_IND:
         {
           gpl1_gprs_handle_ds_abort_ind((mph_l1_ds_abort_ind_T *)msg_header);
           break;
         }

        /********************************************************
         *                                                      *
         *                MPH_SET_PRIORITY_IND                  *
         *                                                      *
         ********************************************************/

         case MPH_SET_PRIORITY_IND:
         {
           gpl1_gprs_handle_priority_ind((mph_set_priority_ind_T *)msg_header);
           break;
         }

        /********************************************************
         *                                                      *
         *                MPH_STOP_GSM_MODE_REQ                 *
         *                                                      *
         ********************************************************/

         case MPH_STOP_GSM_MODE_REQ:
         {
            mph_stop_gsm_mode_req_T  *message_ptr;

            message_ptr = ( mph_stop_gsm_mode_req_T *) msg_header; /*lint !e740 */
            gas_id = check_gas_id(message_ptr->gas_id);

            /* Stop L1 ISR */
            L1_setup_DEACTIVATE( msg_header );

            /* Rude wakeup from SLEEP, so we respond faster */
            gl1_drx_require_next_tick(gas_id);

            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_UE_MODE_CHANGE_REQ                *
         *                                                      *
         ********************************************************/

         case MPH_UE_MODE_CHANGE_REQ:
           {
             mph_ue_mode_change_req_T  *message_ptr;

             message_ptr = (mph_ue_mode_change_req_T *)msg_header; /*lint !e740 */
             gas_id = check_gas_id(message_ptr->gas_id);
#ifdef FEATURE_SGLTE
             /* Set SGLTE mode */
             gl1_hw_set_sglte_mode(message_ptr->sglte_mode_active, gas_id);                          
#endif

             /* Abort MCPM INIT state before changing neighbour cells. */
             if( gl1_hw_mcpm_get_state(gas_id) == GL1_MCPM_GERAN_INIT ||
                 (gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id) && gl1_hw_check_ongoing_mcpm_update(MCPM_GERAN_INIT_REQ, gas_id)) )
             {
               /* push the state machine back to NULL */
               GL1_ISR_LOCK(gas_id);
               if(l1_tsk_buffer[gas_id].main_command != L1_DEACTIVATE_COMMAND)
               {
                 l1_tsk_buffer[gas_id].main_command = L1_ENTER_NULL_COMMAND;
               }
               GL1_ISR_UNLOCK(gas_id);
             }

             /* Wait until MCPM come out of INIT state. */
             while( gl1_hw_mcpm_get_state(gas_id) == GL1_MCPM_GERAN_INIT ||
                    gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id) )
             {
               MSG_GERAN_HIGH_0_G("Waiting for MCPM out of INIT state to handle UE mode change req");
               (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
               (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
             }

             l1_sc_wcdma_set_mode(message_ptr->gsm_only, gas_id);

             break;
           }

        /********************************************************
         *                                                      *
         *                MPH_POWER_SCAN_REQ                    *
         *                                                      *
         ********************************************************/

         case MPH_POWER_SCAN_REQ:
         {
            mph_power_scan_req_T *message_ptr;

            message_ptr = (mph_power_scan_req_T *)msg_header;
            gas_id = check_gas_id(message_ptr->gas_id);

            if( FALSE == L1_setup_PSCAN( msg_header ) )
            {
              /* Command the  the ISR to start measuring */
              GL1_ISR_LOCK(gas_id);
              L1_send_ISRTIM_SCAN_REQ( L1_FIND_BCCH_COMMAND,gas_id );
              GL1_ISR_UNLOCK(gas_id);

              /* Rude wakeup from SLEEP, the other SIM may be in IDLE so L1 is sleeping */
              gl1_drx_require_next_tick(gas_id);
            }
            else
            {
              /* Something is not right about the power scan request.
               * Send back a confirmation to avoid any upper layer
               * timeouts.
               */
              L1_send_MPH_POWER_SCAN_CNF( FALSE, gas_id ); /* suspending boolean */
            }

            break;
         }

        /********************************************************
         *                                                      *
         *            MPH_SELECT_SPECIFIC_BCCH_REQ              *
         *                                                      *
         ********************************************************/

         case MPH_SELECT_SPECIFIC_BCCH_REQ:
         {
            mph_select_specific_bcch_req_T *message_ptr;
            uint8 bsic_requested;
            sys_band_mask_type             band_mask;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
            /* align pointers */
            message_ptr = (mph_select_specific_bcch_req_T *)msg_header; /*lint !e740 */
            gas_id = check_gas_id(message_ptr->gas_id);

            l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            /* If we are about to camp on a 1800 band cell ensure we are running at
             * low clock speed, otherwise set some other band to clock at high speed
             */
            if(message_ptr->specific_channel_no.band == SYS_BAND_DCS_1800)
            {
              band_mask = SYS_BAND_MASK_GSM_DCS_1800;
            }else
            {
              band_mask = SYS_BAND_MASK_GSM_850;
            }

            gl1_set_band_group(band_mask, gas_id);


            GL1_ISR_LOCK(gas_id);
            l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
            bsic_requested = (message_ptr->bsic.PLMN_colour_code << 3) | message_ptr->bsic.BS_colour_code;

            L1_send_ISRTIM_ACQ_REQ(message_ptr->specific_channel_no, gas_id);
            l1_tskisr_blk->next_params.bsic_expected = bsic_requested;
            MSG_GERAN_MED_2_G("SpecificBCCH Req with ARFCN:%d, BSIC %2x",message_ptr->specific_channel_no.num, bsic_requested);
            GL1_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_COEX_SW_CXM
            garb_intf_notify_find_bcch_update_arfcn(message_ptr->specific_channel_no,gas_id);
            garb_intf_notify_find_bcch_update_rx_power(message_ptr->specific_channel_no,
              (int32)(l1_tskisr_blk->next_params.L1Data.pAgc_data->pwr_dBm_x16),gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

            /* Rude wakeup from SLEEP, the other SIM may be in IDLE so L1 is sleeping */
            gl1_drx_require_next_tick(gas_id);

            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_START_IDLE_REQ                    *
         *                                                      *
         ******************************************************/

         case MPH_START_IDLE_REQ:
         {
           mph_start_idle_req_T* message_ptr = ( mph_start_idle_req_T* )msg_header;
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

           gas_id = check_gas_id(message_ptr->gas_id);
           l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            L1_setup_START_IDLE( msg_header );

            /* Change the ISR mode */
            l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

            (void)l1_log_paging_data(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
             l1_clear_global_override_trm_reason(gas_id);
#endif			

            break;
         }


        /********************************************************
         *                                                      *
         *        MPH_DECODE_BCCH_LIST_ABORT_REQ                *
         *                                                      *
         ****************************************************** */

         case MPH_DECODE_BCCH_LIST_ABORT_REQ:
         {
           mph_decode_bcch_list_abort_req_T* message_ptr = ( mph_decode_bcch_list_abort_req_T* )msg_header;
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

           gas_id = check_gas_id(message_ptr->gas_id);
            /* Change the ISR mode */
            GL1_ISR_LOCK(gas_id);
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            l1_tskisr_blk->main_command = L1_ENTER_NULL_COMMAND;
            /*
               Indicate that a decode bcch list cnf is to be sent.
               Only sent when aborted by a MPH_DECODE_BCCH_LIST_ABORT_REQ
             */

            l1_tskisr_blk->next_params.L1_confirm_ID = L1_DECODE_BCCH_LIST_CNF;
            GL1_ISR_UNLOCK(gas_id);

            break;
         }


        /********************************************************
         *                                                      *
         *                MPH_EARLY_CAMPING                    *
         *                                                      *
         ******************************************************/

         case MPH_EARLY_CAMPING_REQ:
         {
            mph_early_camping_req_T* message_ptr = ( mph_early_camping_req_T* )msg_header;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

            gas_id = check_gas_id(message_ptr->gas_id);
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            L1_setup_EARLY_CAMPING( msg_header );

            /* Change the ISR mode */
            l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
			
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
            l1_set_global_override_trm_reason( TRM_CHANNEL_MAINTENANCE,gas_id );
#endif

            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_IGNORE_WCDMA_NEIGHBORS_REQ        *
         *                                                      *
         ********************************************************/

         case MPH_IGNORE_WCDMA_NEIGHBORS_REQ:
         {
            mph_ignore_wcdma_neighbors_req_T* msg;
            msg = ( mph_ignore_wcdma_neighbors_req_T* )msg_header; /*lint !e740 */
            l1_sc_wcdma_ignore_neighbors( msg );
            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_WCDMA_CELL_UPDATE_LIST_REQ        *
         *                                                      *
         ********************************************************/

         case MPH_WCDMA_CELL_UPDATE_LIST_REQ:
         {
            mph_wcdma_cell_update_list_req_T* msg;
            msg = ( mph_wcdma_cell_update_list_req_T* )msg_header; /*lint !e740 */

            gas_id = check_gas_id(msg->gas_id);
			
            /* Abort MCPM INIT state before changing neighbour cells..
               It can't enable/disable neighbour cells when MCPM is in INIT state */
            if( gl1_hw_mcpm_get_state(gas_id) == GL1_MCPM_GERAN_INIT ||
                (gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id) && gl1_hw_check_ongoing_mcpm_update(MCPM_GERAN_INIT_REQ, gas_id)) )
            {
              /* push the state machine back to NULL */
              GL1_ISR_LOCK(gas_id);
              if(l1_tsk_buffer[gas_id].main_command != L1_DEACTIVATE_COMMAND)
              {
                l1_tsk_buffer[gas_id].main_command = L1_ENTER_NULL_COMMAND;
              }
              GL1_ISR_UNLOCK(gas_id);
            }
            
            /* Wait until MCPM come out of INIT state. */
            while( gl1_hw_mcpm_get_state(gas_id) == GL1_MCPM_GERAN_INIT ||
                   gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id) )
            {
              MSG_GERAN_HIGH_0_G("Waiting for MCPM out of INIT state to handle IRAT neighbor update list");
              (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
              (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
            }

            l1_sc_set_wcdma_list_update_inprogress( TRUE , msg->gas_id );
            l1_sc_wcdma_update_list( msg, gas_id );
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
         *                                                      *
         *                MPH_READ_SYS_INFO_REQ                 *
         *                                                      *
         ********************************************************/

         case MPH_READ_SYS_INFO_REQ:

         {
            mph_read_sys_info_req_T* message_ptr = ( mph_read_sys_info_req_T* )msg_header;
           idle_data_T               *l1_idle_data;
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

           gas_id = check_gas_id(message_ptr->gas_id);
           l1_idle_data=&l1_idle_data_store[gas_id];
           l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            /* While L1 is in FindBCCH mode it will ignore the
             * mask RR sends for the normal BCCH and get all
             * normal BCCH blocks.  It will use the mask RR
             * provides on the extended BCCH.  Also, L1 will
             * remember the mask and apply it in Idle Mode.
             */
            L1_process_READ_SYS_INFO_REQ( (mph_read_sys_info_req_T *)msg_header ); /*lint !e740 */

            l1_tskisr_blk->current_params.extended_BCCH_mask = l1_idle_data->ext_BCCH_mask;
            break;
         }

        /********************************************************
         *                                                      *
         *            MPH_DECODE_BCCH_LIST_REQ                  *
         *                                                      *
         ********************************************************/
         case MPH_DECODE_BCCH_LIST_REQ:
         {
           mph_decode_bcch_list_req_T  *message_ptr;
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
           BCCH_List_T *l1_BCCH_List_ptr;

           /* Align pointers */
           message_ptr = ( mph_decode_bcch_list_req_T *)msg_header; /*lint !e740 */
           gas_id = check_gas_id(message_ptr->gas_id);
           l1_tskisr_blk = &l1_tsk_buffer[gas_id];
           l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
            /* Only process in L1_GETTING_BCCH sub state to avoid
             * this list decode interrupting a previous one that's
             * still in progress.
             */
            if ( (l1_tskisr_blk->sub_state == L1_MULTI_SIM_RESELECTION) ||
                 ( (l1_tskisr_blk->sub_state     == L1_MULTI_SIM_AQUIRING) &&
                   (l1_tskisr_blk->sub_sub_state == L1_ACQ_END)   &&
                   (last_acq_was_list            == FALSE)
                 )
               )
            {


               /* If RR sends a null list, just send back a cnf and do nothing */
               if ( message_ptr->frequency_list.num_frequencies == 0 )
               {
                  MSG_GERAN_ERROR_0_G("RR->L1 BCCH decode list empty");

                  /* Send CNF(ERROR) back to RR */
                  L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
               }
               else
               {
                  L1_setup_DECODE_BCCH_LIST( msg_header );
                  /*Only setup bands for use that are required.*/
                  gpl1_select_band_to_req_trm(message_ptr->requested_bands, l1_tskisr_blk->client_id, gas_id);

                  /* Don't handle background search in this state. */
                  MSG_GERAN_MED_0_G("Decode BCCH list initiated in foreground");

                  GL1_ISR_LOCK(gas_id);
                  l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
                  l1_tskisr_blk->command      = L1_ACQUIRE_BCCH_LIST_COMMAND;
                  L1_send_ISRTIM_ACQ_LIST_REQ( l1_BCCH_List_ptr->current_sch_index,gas_id );
                  GL1_ISR_UNLOCK(gas_id);

                  /* Rude wakeup from SLEEP, the other SIM may be in IDLE so L1 is sleeping */
                  gl1_drx_require_next_tick(gas_id);

               }
            }
            else
            {
              MSG_GERAN_ERROR_2_G("DECODE_BCCH_LIST_REQ ignored sub %d sub sub %d ", l1_tskisr_blk->sub_state,
              l1_tskisr_blk->sub_sub_state);
            }
            break;
         }
        /********************************************************
        *                                                      *
        *                MPH_START_GPRS_IDLE_REQ               *
        *                                                      *
        ********************************************************/

        case MPH_START_GPRS_IDLE_MODE_REQ:
          {
            mph_start_gprs_idle_mode_req_t *message_ptr;
            gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
            uint8 i;

            message_ptr = (mph_start_gprs_idle_mode_req_t *)msg_header;
            gas_id = check_gas_id(message_ptr->gas_id);

            gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

            gl1_handle_mph_start_gprs_idle_mode_req(message_ptr);

            /*  Initialise the tx power array to maximum allowable power  */
            for (i=0; i<8; i++)
            {
              gprs_serv_cell_meas_ptr->pch[i] = gprs_serv_cell_meas_ptr->pmax;
            }

            gprs_serv_cell_meas_ptr->quality_report_time_index = 0;

            /*  Initialise the running average structure  */
            /*  Initialize serving cell measurement averaging filter */
            gprs_serv_cell_meas_ptr->rla_p_count_idle = GPRS_SC_RLA_P_COUNT_IDLE;

            l1_running_avg_init(&gprs_serv_cell_meas_ptr->rla_p_buffer, 
                                gpl1_gprs_get_serv_cell_meas_buf(PRX_MEAS_BUFFER, gas_id),
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
                                gpl1_gprs_get_serv_cell_meas_buf(DIVRX_MEAS_BUFFER, gas_id),
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA || FEATURE_GSM_RX_DIVERSITY_DATA*/
                                (L1_GPRS_SERV_CELL_MEAS_BUF_LEN - 1),
                                gas_id);

            /*  Set for Idle 52 mode measurements */
            l1_running_avg_set_avg_len(&gprs_serv_cell_meas_ptr->rla_p_buffer, GPRS_SERV_CELL_IDLE_52_AVG_LEN,gas_id );

            /*************************************************************************/
            /* CR69217                                                               */
            /* ------------------                                                    */
            /* Clear any flags related to gprs that may still be set from a previous */
            /* gprs session.                                                         */
            /*************************************************************************/
            /* only update if nobody is in TRANSFER MODE */
            if ( !gl1_is_any_sub_in_transfer() )
            {
              l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
              l1_transfer_data[gas_id].confirm_pending = SENT;

              l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
              l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;
            }

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
		   /* Clear out any overrid reason */
           l1_clear_global_override_trm_reason(gas_id);  
#endif	

          }
          break;

#ifdef FEATURE_GERAN_DRDSDS
          /********************************************************
           *                                                      *
           *                MPH_DSC_THRESHOLD_IND                 *
           *                                                      *
           ********************************************************/
          case MPH_DSC_THRESHOLD_IND:
          {
            L1_handle_MPH_DSC_THRESHOLD_IND(msg_header);
          }
          break;

#endif

         case MPH_GPRS_L1PARAM_UPDATE_REQ:
         {
            gpl1_gprs_handle_grr_idle_mode(msg_header);
            break;
         }


         case MPH_GPRS_RESELECTION_REQ:
         {
            gpl1_gprs_handle_reselection_bcch_mode((mph_gprs_reselection_req_t *)msg_header);
            break;
         }

         default:
            MSG_GERAN_ERROR_1("RR->L1(BCCH) message %d ignored.", msg_header->message_id);
             break;

      } /* End of switch */
   } /* End of    else if (msg_header->message_set == MS_RR_L1) */
   else if (msg_header->message_set == MS_L2_L1)
   {
     /********************************************************
      *                                                      *
      *                PH_RANDOM_ACCESS_IND                  *
      *                                                      *
      ********************************************************/
      if (msg_header->message_id == PH_RANDOM_ACCESS_IND)
      {
         ph_random_access_ind_T *message_ptr = (ph_random_access_ind_T *)msg_header;
         gl1_trm_state_t trm_status;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         gas_id = check_gas_id(message_ptr->gas_id);

         if (message_ptr->rach_for_ps_traffic)
         {
           MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for PS");
         } else
         {
           MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for CS");
         }
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];
         trm_status = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
         if (trm_status == GL1_TRM_RETAINED_FOR_ACCESS)
         {
           /* this is to prevent frame manager from retaining the lock */
           grm_set_trm_status(GL1_TRM_GRANTED,l1_tskisr_blk->client_id, gas_id);
         }  
       /* start to move GL1 to Decoupled Mode*/
          l1_tskisr_blk->main_command = L1_DECOUPLE_MULTI_SIM_COMMAND;
      }
     /********************************************************
      *                                                      *
      *                PH_RANDOM_ACCESS_REQ                  *
      *                                                      *
      ********************************************************/
      else if (msg_header->message_id == PH_RANDOM_ACCESS_REQ)
      {
        MSG_GERAN_ERROR_0("Rcvd PH_RANDOM_ACCESS_REQ in wrong state");
      }
   }
   /*******************************************************
   *                                                      *
   *                   GSM CB-L1 messages                 *
   *                                                      *
   ********************************************************/
   else if ( msg_header->message_set == MS_CB_L1)
   {
      if (msg_header->message_id == CB_SCHEDULING_REQ) /*lint !e641 */
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
      }
      else if (msg_header->message_id == CB_SKIP_REQ) /*lint !e641 */
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
      }
      else if (msg_header->message_id == CB_DEACTIVATE_REQ )  /*lint !e641 */
      {
         cb_deactivate_req_type *message_ptr;
         message_ptr = (cb_deactivate_req_type *) msg_header; /*lint !e740 */

         l1_smscb_process_deactivate_info( message_ptr->channel_ind, geran_map_nas_id_to_gas_id(message_ptr->as_id) );
      }
   }
   else
   {
      MSG_GERAN_ERROR_0("L1(BCCH) - Unknown message set.");
   }
}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
