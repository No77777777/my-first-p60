/*===========================================================================

                                  R R C R C R

GENERAL DESCRIPTION
 The RRCRCR module consists of RRC CONNECTION RELEASE PROCEDURE 
 implementation. The 3GPP TS 25.331 RRC Protocol specification is used as
 basis for implementation.

EXTERNALIZED FUNCTIONS

  tdsrrcrcr_init_procedure()
    This function will initialize the RRC Connection Release 
    procedure substate to TDSRRCRCR_INITIAL Substate. This will be called in
    the RRCTASK modules along with other Init Procedures.

  tdsrrcrcr_procedure_event_handler()

    This is the entry point for RRC Connection Release procedure. 
    All events for RCR come to this function from RRC Dispatcher. 
    Depending on the UE state of RRC Connection Release Procedure, it
    calls the corresponding state handler. These state handlers call
    substate handlers depending on the the current substate of RRC
    Connection Release procedure.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcrcr.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/19/12   bj      Delete NV release check when receive RRC connection release message.
10/21/11   yuh    Merged LTE code
03/10/11   bj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/18/10   ysh     Frequency info save. 3GPP-8.6.6.1(3.4.2)
                                  Save Primary freq when enter Non-CELL_DCH state.
11/11/10   yzh     (3GPP: 10.3.8.15)Add Rplmn information in RRC CONNECTION RELEASE message.
10/29/10   zwj     Added code to handle receive R4 RRCRleaseMessage.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdscdma_variation.h"
#include "rex.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "tdsrrcrcr.h"
#include "tdsrrcrcri.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrccmd_v.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcsend.h"
#include "tdsrrctmr.h"
#include "tdsrrcsibdb.h"
#include "tdsrrclcm.h"
#include "msg.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcmm_v.h"
#include "tdsrrccu.h"
#include "tdsrrcllc.h"
#include "tdsrrclogging.h"
#include "tdsrrccspi.h"
#include "tdsrrccsp.h"
#include "tdsrrcpg1.h"
#include "tdsrrcccm.h"
#include "tdsrrcmisc.h"
#include "tdsrrcasn1util.h"

/*#include "rr_rrc_if.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#include "tdsrrcmcm.h"

#ifdef FEATURE_TDSCDMA_TO_LTE
#include "lte_rrc_ext_api.h"
#endif

#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "mm_as_types.h"
#include "mm.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/
/*===========================================================================

      RRC CONNECTION RELEASE PROCEDURE SUBSTATE Variable        

===========================================================================*/
/* RCR Substate variable initialized to TDSRRCRCR_INITIAL during the RRC TASK 
   startup. */              
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrcrcr_substate_e_type  tdsrrcrcr_substate;  

static uint32 tdsrrcrcr_n_308;         /* Variable to store max number of 
                                      RRC CONNECTION RELEASE COMPLETE 
                                      retransmission */
static boolean tdsrrcrcr_cell_reselection_occured; 
                                   /* This indicates whether cell reselection
                                      occured during RCR procedure */
static tdsrrc_RRC_TransactionIdentifier tdsrrcrcr_transaction_identifier;
                                   /* This stores the transaction Id for
                                      RRC Connection Release message */
static tdsrrcrcr_error_indication_status_e_type tdsrrcrcr_error_ind; 
                                   /* This stores the information whether to 
                                      include error cause in the RRC
                                      Connection Release Complete Msg */
                                   
tdsrrc_redirect_req_type tdsrrcrcr_saved_redirect_info;
boolean tdsrrcrcr_redirection_info_present;
                                   
boolean tdsrrc_t2t_redir_due_to_rls;                /* The flag is record RCR with redirection info */
extern tdsrrc_cs_domain_error_count_type tdsrrc_cs_domain_error;

/*===========================================================================

FUNCTION tdsrrcrcr_init_procedure

DESCRIPTION
  This function will initialize the RRC Connection Release procedure
  substate to TDSRRCRCR_INITIAL Substate. This will be called in the RRCTASK
  modules along with other Init Procedures.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcrcr_init_procedure
(    
  void  
)
{
  /* Resets the RCR state machine */
  tdsrrcrcr_substate = TDSRRCRCR_INITIAL; 
  /* The following code is added since de-registration mechanism is not place.
     It should be deleted once de-registration mechanism in place in State
     change manager */
  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_RCR,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_DISCONNECTED);     
  /* For T2T redirection event */
  tdsrrc_t2t_redir_due_to_rls = FALSE;
} /* tdsrrcrcr_init_procedure */

/*===========================================================================

FUNCTION tdsrrcrcr_procedure_event_handler

DESCRIPTION
  This is the entry point for RRC Connection Release procedure. All
  events for RCR come to this function from RRC Dispatcher. Depending on the
  UE state of RRC Connection Release Procedure, it calls the corresponding
  state handler. These state handlers call substate handlers depending on the 
  the current substate of RRC Connection Release procedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  switch (rrc_state)
  {
    case TDSRRC_STATE_DISCONNECTED:
      tdsrrcrcr_process_disconnected_state(cmd_ptr);
      break;

    case TDSRRC_STATE_CELL_FACH:    
      tdsrrcrcr_process_cell_fach_state(cmd_ptr); 
      break;

    case TDSRRC_STATE_CELL_DCH:     
      tdsrrcrcr_process_cell_dch_state(cmd_ptr); 
      break;
  
    default:  
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Event forwarded in an invalid State: %d",rrc_state);
      break;
   }  
} /* tdsrrcrcr_procedure_event_handler */

/*===========================================================================

FUNCTION tdsrrcrcr_process_cell_fach_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in CELL_FACH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_cell_fach_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcrcr_substate)
  {
    case TDSRRCRCR_INITIAL: 
      tdsrrcrcr_process_rcr_initial_substate(cmd_ptr);
      break;
  
    case TDSRRCRCR_WAIT_FOR_CELL_UPDATE_COMP_CNF:
      tdsrrcrcr_process_rcr_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
      break;

    case TDSRRCRCR_WAIT_FOR_L2ACK_FOR_RRC_CONN_REL_COMPLETE:
      tdsrrcrcr_process_rcr_wait_for_l2ack_for_rrc_conn_rel_complete_substate(cmd_ptr);
      break;

    case TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF:
      tdsrrcrcr_process_rcr_wait_for_srb_release_cnf_substate(cmd_ptr);
      break;

    default:     
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid substate in CELL_FACH state: %d",tdsrrcrcr_substate);
     break;
  }  
} /* tdsrrcrcr_process_cell_fach_state */
/*===========================================================================

FUNCTION tdsrrcrcr_process_cell_dch_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in CELL_DCH state of Connected  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_cell_dch_state
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcrcr_substate)
  {
    case TDSRRCRCR_INITIAL: 
      tdsrrcrcr_process_rcr_initial_substate(cmd_ptr);
      break;

    case TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE:
      tdsrrcrcr_process_rcr_wait_for_t308_timer_expire_substate(cmd_ptr);
      break;

    case TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF:
      tdsrrcrcr_process_rcr_wait_for_srb_release_cnf_substate(cmd_ptr);
      break;

    default:     
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid substate in CELL_DCH state: %d",tdsrrcrcr_substate);
     break;
  }  
} /* tdsrrcrcr_process_cell_dch_state */
/*===========================================================================

FUNCTION tdsrrcrcr_process_disconnected_state

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in DISCONNECTED state of Idle  mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_disconnected_state     
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch (tdsrrcrcr_substate)
  {
    case TDSRRCRCR_WAIT_FOR_L2ACK_FOR_RRC_CONN_REL_COMPLETE:
      tdsrrcrcr_process_rcr_wait_for_l2ack_for_rrc_conn_rel_complete_substate(cmd_ptr);
      break;

    case TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE:
      tdsrrcrcr_process_rcr_wait_for_t308_timer_expire_substate(cmd_ptr);
      break;

    case TDSRRCRCR_WAIT_FOR_CELL_UPDATE_COMP_CNF:
      tdsrrcrcr_process_rcr_wait_for_cell_update_comp_cnf_substate(cmd_ptr);
      break;

    case TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF:
      tdsrrcrcr_process_rcr_wait_for_srb_release_cnf_substate(cmd_ptr);
      break;

    default:     
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"In Idle mode,Invalid cmd %d in RCR substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrcr_substate);
     break;
  }  
} /* tdsrrcrcr_process_disconnected_state */

/*===========================================================================

FUNCTION tdsrrcrcr_convert_t308_to_ms

DESCRIPTION
  This function converts the tdsrrc_T_308 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  int

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrcrcr_convert_t308_to_ms
(
  tdsrrc_T_308 rrcrcr_t_308                   /* enum for Timer T308 */
)
{
  uint32 timer_value;                         /* Timer Value in milli seconds */

  switch (rrcrcr_t_308)
  {
    case tdsrrc_T_308_ms40:
      timer_value = 40;
      break;

    case tdsrrc_T_308_ms80:
      timer_value = 80;
      break;    
  
    case tdsrrc_T_308_ms160:
      timer_value = 160;
      break;

    case tdsrrc_T_308_ms320:
      timer_value = 320;
      break;
  
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unknown Timer value rcvd from SIB1");
      timer_value = 40;  /* Set lowest timer value as a default */
      break;
  }
  return (timer_value);
} /* tdsrrcrcr_convert_t308_to_ms*/


/*===========================================================================
FUNCTION tdsrrcrcr_send_connection_rel_complete_msg

DESCRIPTION
  This function builds RRC CONNECTION RELEASE COMPLETE message and send the
  same to the SEND CHAIN to transmit either in Unacknowledged mode or 
  Acknowledged mode.
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrcr_status_e_type: It returns TDSRRCRCR_SUCCESS if it successfully places 
  RRC CONNECTION RELEASE COMPLETE SDU to the SEND CHAIN. Otherwise it returns
  TDSRRCRCR_FAILURE_MAJOR

SIDE EFFECTS
  None
===========================================================================*/
tdsrrcrcr_status_e_type tdsrrcrcr_send_connection_rel_complete_msg
(
  tdsrrc_RB_Identity rb_id,                    /* Radio bearer Id */ 
  uecomdef_logch_mode_e_type mode,          /* Mode of transmit */  
  tdsrrcrcr_error_indication_status_e_type error_ind_status
                                            /* Indicates whether to include
                                               Error Indication field in the 
                                               RRC Connection Rel comlete Msg*/                                         
)
{
  tdsrrcrcr_status_e_type status = TDSRRCRCR_FAILURE_MAJOR; 
                                            /* Status is initialized to
                                               Failure */
  tdsrrc_UL_DCCH_Message *msg_ptr;             /* Pointer to uplink DCCH message*/

  rlc_lc_id_type ul_lc_id;                  /* Logical Channel Id */
  
 /* Checks whether Logical Channel entity setup for DCCH logical
     Channel type */
  ul_lc_id = tdsrrclcm_check_ul_rlc_lc_id ( UE_LOGCHAN_DCCH,
                                         rb_id,                                           
                                         mode);
  /* If RLC Logical Channel Id is already setup, the send RRC Connection
     Release Complete message through SEND_CHAIN */
  if(ul_lc_id != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
  {
    /* Allocate a buffer for an uplink DCCH Message. This buffer should be 
       released by tdsrrcsend_send_ul_sdu() function using OSS compiler functions */

    msg_ptr = (tdsrrc_UL_DCCH_Message *) tdsrrc_malloc ( 
                                      sizeof(struct tdsrrc_UL_DCCH_Message));
    if (msg_ptr != NULL)
    {

      msg_ptr->message.t = T_tdsrrc_UL_DCCH_MessageType_rrcConnectionReleaseComplete;

      /* Initialize the bit mask for RRC Connection Release Complete */
      TDSRRC_RESET_MSG_IE_PRESENT(msg_ptr->message.u.rrcConnectionReleaseComplete);

      if(TDSRRCRCR_INCLUDE_ERROR_INDICATION == error_ind_status)
      {
       TDSRRC_MSG_COMMON_SET_BITMASK_IE_TYPE2(msg_ptr->message.u.rrcConnectionReleaseComplete,
          errorIndication);
        msg_ptr->message.u.rrcConnectionReleaseComplete.errorIndication.t =
          T_tdsrrc_FailureCauseWithProtErr_protocolError;
        msg_ptr->message.u.rrcConnectionReleaseComplete.errorIndication.u.protocolError = 
          rtxMemAllocTypeZ(&tdsenc_ctxt, tdsrrc_ProtocolErrorInformation);

        if(msg_ptr->message.u.rrcConnectionReleaseComplete.errorIndication.u.protocolError == NULL)
        {
          ERR_FATAL("memory alloc failure",0,0,0);
        }

        msg_ptr->message.u.rrcConnectionReleaseComplete.errorIndication.u.protocolError->
         diagnosticsType.t = tdsrrc_protocol_error_information.diagnosticsType.t;
        msg_ptr->message.u.rrcConnectionReleaseComplete.errorIndication.u.protocolError->
         diagnosticsType.u.type1 = tdsrrc_protocol_error_information.diagnosticsType.u.type1;
                /*log the protocol error*/
        tdsrrc_log_protocol_error((uint8) (tdsrrc_get_state()),
                                (uint8)TDSRRC_PROCEDURE_RCR,
                                (uint8)TDSRRCLOG_PROTOCOL_ERROR,
                                tdsrrc_log_map_protocol_cause(tdsrrc_protocol_error_information.diagnosticsType.u.type1->protocolErrorCause)
                                );
      }
    
      /* Set the Transaction Id */
      msg_ptr->message.u.rrcConnectionReleaseComplete.rrc_TransactionIdentifier =
        tdsrrcrcr_transaction_identifier;

 
      /* This places SDU into RLC watermark queue */
      if(rb_id == DCCH_UM_RADIO_BEARER_ID)
      {
        if(TDSRRCSEND_SUCCESS ==
           tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RCR,msg_ptr,ul_lc_id,
                            TDSRRCSEND_L2ACK_NOT_REQUIRED))
        {
          status = TDSRRCRCR_SUCCESS;        

          /* start the timer T308 */
          tdsrrctmr_start_timer(TDSRRCTMR_T_308_TIMER, 
                        tdsrrcrcr_convert_t308_to_ms(TDSRRC_GET_T308()) );  
        }

      }
      else if (rb_id == DCCH_AM_RADIO_BEARER_ID)
      {
        if(TDSRRCSEND_SUCCESS == 
           tdsrrcsend_send_ul_sdu(TDSRRC_PROCEDURE_RCR,msg_ptr,ul_lc_id,
           TDSRRCSEND_L2ACK_REQUIRED))
        {
          status = TDSRRCRCR_SUCCESS;

          /* Set the pointer to NULL as it is already freed in embedded buff */
          tdsrrc_protocol_error_information.diagnosticsType.u.type1 = NULL;

          /* Start the sanity timer for L2 ACK */
          tdsrrctmr_start_timer(TDSRRCTMR_RCR_L2ACK_TIMER, 
                             TDSRRCRCR_TIMER_VALUE_FOR_L2_ACK);
        }   
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid RB for RRC Conn Rel Complete");
        tdsrrcasn1_free_pdu(tdsrrc_UL_DCCH_Message_PDU, msg_ptr);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Malloc failed for RRC Conn Rel Comp Msg");
    }
  }
  else
  { 
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB: %d",rb_id);

  } 
  return (status);            
}/*tdsrrcrcr_send_connection_rel_complete_msg */

#if defined FEATURE_TDSCDMA_TO_LTE
/*========================================================================
FUNCTION tdsrrcrcr_save_lte_redirection_info

DESCRIPTION
  This function saves the LTE redirection info received in RRC
  Connection Release message. This info is then later passed on to
  CSP for further processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/
void tdsrrcrcr_save_lte_redirection_info
(
  tdsrrc_EUTRA_TargetFreqInfoList *EUTRA_target_freq_info_ptr
)
{
  tdsrrc_EUTRA_BlacklistedCellPerFreqList *blacklisted_cell_info_ptr = NULL;
  uint8 earfcn_count = 0;
  uint8 blacklisted_cells_count = 0;
  uint32 idx, idy;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving redirection info from Conn release message.");
  tdsrrcrcr_redirection_info_present = TRUE;

  tdsrrcrcr_saved_redirect_info.choice = TDSRRC_INTER_SYSTEM_LTE;
  tdsrrcrcr_saved_redirect_info.u.EUTRA_target_cell_info.earfcn_count = 0;

  idx = 0;
  while((EUTRA_target_freq_info_ptr->n > idx) &&
          (earfcn_count < LTE_RRC_IRAT_MAX_REDIR_FREQ))
  {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    if(!lte_rrc_earfcn_is_supported(tdsrrc_get_as_id(), (lte_earfcn_t)EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq, tdsrrccsp_get_lte_band_pref()))
#else
    if(!lte_rrc_earfcn_is_supported((lte_earfcn_t)EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq, tdsrrccsp_get_lte_band_pref()))
#endif
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSTOL: EARFCN %d not supported by UE",EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq);
      idx++;
      continue;
    }

    tdsrrcrcr_saved_redirect_info.u.EUTRA_target_cell_info.earfcn_info[earfcn_count].earfcn = (lte_earfcn_t)EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSTOL: target EARFCN %d",EUTRA_target_freq_info_ptr->elem[idx].dlEUTRACarrierFreq);
  
    /* Check if blacklisted cells also present for the current EARFCN.*/
    blacklisted_cells_count = 0;
    if(EUTRA_target_freq_info_ptr->elem[idx].m.eutraBlacklistedCellPerFreqListPresent)
    {
      idy = 0;
      blacklisted_cell_info_ptr = &(EUTRA_target_freq_info_ptr->elem[idx].eutraBlacklistedCellPerFreqList);
      while((blacklisted_cell_info_ptr->n > idy) &&
               (blacklisted_cells_count < LTE_RRC_IRAT_MAX_EUTRA_CELLS_PER_FREQ))
      {
        tdsrrcrcr_saved_redirect_info.u.EUTRA_target_cell_info.earfcn_info[earfcn_count].blacklisted_cells[blacklisted_cells_count++] = 
           blacklisted_cell_info_ptr->elem[idy].physicalCellIdentity;
        idy++;
      }
    }
    tdsrrcrcr_saved_redirect_info.u.EUTRA_target_cell_info.earfcn_info[earfcn_count++].blacklisted_cells_count = blacklisted_cells_count;
    idx++;
  }
  tdsrrcrcr_saved_redirect_info.u.EUTRA_target_cell_info.earfcn_count = earfcn_count;

  if(earfcn_count == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSTOL: No target EARFCN's supported. No redirection.");
    tdsrrcrcr_redirection_info_present = FALSE;
  }

}
#endif
#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================
FUNCTION tdsrrcrcr_process_860non_critical_ext

DESCRIPTION
  Processes 860 non critical extension of RRC Conn Rel OTA.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void tdsrrcrcr_process_860non_critical_ext
(
  tdsrrc_RRCConnectionRelease_v860ext_IEs * rcr_860ext
)
{
  if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(rcr_860ext,tdsrrc_RRCConnectionRelease_v860ext_IEs,redirectionInfo))
  {
#if defined FEATURE_TDSCDMA_TO_LTE
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: RedirectionInfo-v860ext present.");
    if(rcr_860ext->redirectionInfo.t == T_tdsrrc_RedirectionInfo_v860ext_interRATInfo)
    {
      if(rcr_860ext->redirectionInfo.u.interRATInfo->t == T_tdsrrc_InterRATInfo_v860ext_eutra)
      {
        tdsrrcrcr_save_lte_redirection_info(&(rcr_860ext->redirectionInfo.u.interRATInfo->u.eutra->eutra_TargetFreqInfoList));
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"GSM rat redirection info should be given in IE RedirectionInfo.or RedirectionInfo-r6");
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Inter-freq redirection info should be given in IE RedirectionInfo or RedirectionInfo-r6.");
    }
#else
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: RedirectionInfo-v860ext not supported.");
#endif
  }
}
#endif /*FEATURE_TDSCDMA_REL8*/

/*===========================================================================

FUNCTION tdsrrcrcr_update_release_cause

DESCRIPTION
  This function converts the release cause from ASN1 format into RRC-MM 
  interface format.
DEPENDENCIES
  None

RETURN VALUE
  rrc_rel_cause_e_type: 

SIDE EFFECTS
  None
===========================================================================*/
rrc_rel_cause_e_type tdsrrcrcr_update_release_cause
(
  tdsrrc_ReleaseCause release_cause 
)
{  
  rrc_rel_cause_e_type converted_rel_cause;

  switch(release_cause)
  {
    case tdsrrc_ReleaseCause_normalEvent:
      converted_rel_cause = RRC_REL_CAUSE_NORMAL;
      break;
    case tdsrrc_ReleaseCause_unspecified:
      converted_rel_cause = RRC_REL_CAUSE_UNSPEC;
      break;
    case tdsrrc_ReleaseCause_pre_emptiveRelease:
      converted_rel_cause = RRC_REL_CAUSE_PRE_EMPTIVE;
      break;
    case tdsrrc_ReleaseCause_congestion:
      converted_rel_cause = RRC_REL_CAUSE_CONGESTION;
      break;
    case tdsrrc_ReleaseCause_re_establishmentReject:
      converted_rel_cause = RRC_REL_CAUSE_RE_ESTABLISH_REJECT;
      break;
    case tdsrrc_ReleaseCause_userInactivity:
      converted_rel_cause = RRC_REL_USER_INACTIVITY;
      break;
    case tdsrrc_ReleaseCause_directedsignallingconnectionre_establishment:
      converted_rel_cause = RRC_DIRECTED_SIGNALLING_REESTABLISHMENT;
      break;
    default:
      /* For other unknown ASN1 release causes set to Unspecified */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Invalid release cause rcvd : %d",release_cause);
      converted_rel_cause = RRC_REL_CAUSE_UNSPEC;
      break; 
  }

  return (converted_rel_cause);
}/*tdsrrcrcr_update_release_cause */

/*========================================================================
FUNCTION tdsrrcrcr_save_redirection_info

DESCRIPTION
  This function saves the redirection info received in RRC
  Connection Release message. This info is then later passed on to
  CSP for further processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
========================================================================*/

void tdsrrcrcr_save_redirection_info
(
  tdsrrc_RedirectionInfo_r6 *redirection_info_ptr
)
{
  sys_band_T gsm_band;
  tdsrr_frequence_band_e_type freq_band;
  tdsrrc_GSM_TargetCellInfoList *GSM_target_cell_info_ptr = NULL;
  tdsrrclog_tdscdma_to_tdscdma_redir_start_event_type td2td_redir_event;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saving redirection info from Conn release message.");
  tdsrrcrcr_redirection_info_present = TRUE;

  switch(redirection_info_ptr->t)
  {
    case T_tdsrrc_RedirectionInfo_r6_frequencyInfo:
      tdsrrcrcr_saved_redirect_info.u.freq.uarfcn_UL_present = FALSE;
      tdsrrcrcr_saved_redirect_info.choice = TDSRRC_FREQ_REDIRECTION;
      tdsrrcrcr_saved_redirect_info.u.freq.uarfcn_DL = (uint16)redirection_info_ptr->u.frequencyInfo->modeSpecificInfo.u.tdd->uarfcn_Nt;
  
      /* For rrc connection release with redirection info Start Event*/
      tdsrrc_t2t_redir_due_to_rls = TRUE;
      td2td_redir_event.wait_timer_in_secs = 0;
      td2td_redir_event.redir_reason = TD2TD_REDIR_RELEASE;
      td2td_redir_event.freq = tdsrrcrcr_saved_redirect_info.u.freq.uarfcn_DL;
      event_report_payload(EVENT_TDSCDMA_TO_TDSCDMA_REDIRECTION_START, sizeof(td2td_redir_event),
                 (void *)&td2td_redir_event);
      break;
  
    case T_tdsrrc_RedirectionInfo_r6_interRATInfo:
      tdsrrcrcr_saved_redirect_info.choice = TDSRRC_INTER_SYSTEM_GSM;
      tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size = 0;
      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2_PTR(redirection_info_ptr->u.interRATInfo,gsm_TargetCellInfoList))
      {
        uint32 local_idx=0;
        GSM_target_cell_info_ptr = &redirection_info_ptr->u.interRATInfo->gsm_TargetCellInfoList;
       
        while ((GSM_target_cell_info_ptr->n > local_idx) && 
               (tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size < TDS_MAX_RRC_RR_GSM_CELL_LIST_SIZE))
        {
            /* Let first check the type of the cell to find if we
             * really need to give this cell to L1 */
          switch(GSM_target_cell_info_ptr->elem[local_idx].frequency_band)
          {
             case tdsrrc_Frequency_Band_dcs1800BandUsed:
               freq_band = TDSRR_DCS_1800;
               break;
    
             case tdsrrc_Frequency_Band_pcs1900BandUsed:
               freq_band = TDSRR_PCS_1900;
               break;
    
             default:
               TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unsupported Band %d rcvd",GSM_target_cell_info_ptr->elem[local_idx].frequency_band);
               freq_band = TDSRR_MAX_BANDS;
               break;
          }
    
          if((gsm_band
                  = tdsrr_get_arfcn_band_internal((uint16)GSM_target_cell_info_ptr->elem[local_idx].bcch_ARFCN, freq_band))
               != INVALID_BAND)
          {
             TDSRRC_MSG2(MSG_LEGACY_HIGH,"Adding ARFCN %d and band %d to the list to be sent to RR",GSM_target_cell_info_ptr->elem[local_idx].bcch_ARFCN,freq_band);
    
             /* Get bcch_ARFCN */
             tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.arfcns[tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size].num
                = (uint16) GSM_target_cell_info_ptr->elem[local_idx].bcch_ARFCN;
    
             tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.arfcns[tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size].band
                = gsm_band;
          }
          else
          {
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"Band %d for ARFCN %d is invalid. Not adding to list",(tdsrr_frequence_band_e_type)GSM_target_cell_info_ptr->elem[local_idx].frequency_band,(uint16) GSM_target_cell_info_ptr->elem[local_idx].bcch_ARFCN);
          }
          if(GSM_target_cell_info_ptr->elem[local_idx].m.bsicPresent)
          {
             tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.BSIC[tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size].BSIC_NCC 
                    = GSM_target_cell_info_ptr->elem[local_idx].bsic.ncc;
             tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.BSIC[tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size++].BSIC_BCC 
                    = GSM_target_cell_info_ptr->elem[local_idx].bsic.bcc;
          }
          else
          {
             tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.BSIC[tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size].BSIC_NCC 
                    = 0xFF;
             tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.BSIC[tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size++].BSIC_BCC 
                    = 0xFF;
          }
          local_idx++;
        }   
      }
      break;
   
    default:     
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"This should never happen");
      break;
  }
}
/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_initial_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in TDSRRCRCR_INITIAL substate of either CELL_FACH State or
  CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr = NULL;  /* Pointer to Downlink DCCH
                                                message */
  tdsrrc_DL_CCCH_Message *ccch_msg_ptr = NULL;  /* Pointer to Downlink CCCH
                                                message */
  rlc_lc_id_type lc_id;                      /* Local variable to store 
                                                RLC LC Id */
  tdsrrc_state_e_type current_state;            /* Local Varibale to store 
                                                the current state */
  tdsrrc_int_u_rnti_type u_rnti=0;                /* Local varibale to store U-RNTI 
                                                received through RCR message */ 
 
  tdsrrc_int_u_rnti_type u_rnti_local=0;          /* Local varibale to store U-RNTI 
                                                received from RRCLLC */

  uint8 message_choice;                      /* Local varibel to store messgae
                                                t */

  tdsrrc_Rplmn_Information_r4 *local_rplmn_information = NULL; /* Local structure for RPLMN info */
  tdsrrc_TDD_UMTS_Frequency_List * tdd_freq_list_ptr = NULL; /* Ptr to the freq list present in RRC Connection
                                                          * Release Message */

  boolean acq_db_updated = FALSE;            /* determines if acq db was updated or not
                                                when Connection Release Request from the 
                                                Network had a frequency information. This
                                                variable is subsequently used to determine
                                                if UE should try acq on the best cell in 
                                                acq db during DCH->Idle transition. If acq_db
                                                is updated then acquisition on the best cell in 
                                                active set is not attempted */

  boolean tdd_freq_list_contains_camped_freq = FALSE;  
                                              /* stores the state related to whether camped freq
                                                 is present in the fdd freq list or not */

  rrc_plmn_identity_type camped_plmn = tdsrrcccm_get_curr_camped_plmn();
                                              /* used to compare the camped plmn with Japan to determine
                                               * the priority order in which freq(s) present in
                                               * active set, rplmn freq list, acq db should be searched.
                                               * If MCC is Japan and RPLMN freq list is present then
                                               * the priority order is:
                                               *      RPLMN Freq List --> Rem. Freqs in ACQ DB.
                                               * Else, the priority order is 
                                               *      Camped Cell --> Camped Freq --> RPLMN Freq List --> Rem. Freqs in ACQ DB. 
                                               * 
                                               * Also, if last camped frequency is present in the RPLMN list
                                               * then it is scanned before any other entry in RPLMN List or ACQ DB.
                                               */

  tdsrrc_ReleaseCause     rrc_release_cause_recvd;

  tdsrrc_RedirectionInfo_r6 *local_redirect_info_ptr = NULL;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {     
    case TDSRRC_DOWNLINK_SDU_IND:
      /* Sets the cell reselection to false at the start of RRC Connection
         Release procedure */
      tdsrrcrcr_cell_reselection_occured = FALSE;

      /* Set boolean not to include error cause */
      tdsrrcrcr_error_ind = TDSRRCRCR_NOT_INCLUDE_ERROR_INDICATION;

      tdsrrc_protocol_error_reject = FALSE;

      /* Assert the receieved message is of type tdsrrc_DL_CCCH_Message */
      if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU)
      {
        message_choice = 
          tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

        if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
        {
          /* Get the OTA message pointer */
          dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *) 
                         cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
        }
        else
        { /* ASN1 decoding failed */
          tdsrrc_protocol_error_reject = TRUE;
        }

        if( message_choice == 
            T_tdsrrc_DL_DCCH_MessageType_rrcConnectionRelease)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rcvd RRC Connection Release DCCH");

          tdsrrcrcr_redirection_info_present = FALSE;

          if(tdsrrc_protocol_error_reject == TRUE) /* DCCH ASN1 decoding failed */
          {

  #if(!defined(T_WINNT))
             /*log the protocol error*/
             tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
                                     (uint8)TDSRRC_PROCEDURE_RCR,
                                     TDSRRCLOG_PROTOCOL_ERROR,
                                     TDSRRCLOG_ASN1_ENC_ERR
                                    );
  #endif

            lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                    UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                    DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                                the above Downlink Logical 
                                                Channel type. */
                    UE_MODE_ACKNOWLEDGED_DATA);
                                              /* Mode of transmission for the RLC 
                                                 service entity. */
            if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
            } 
            else /* Valid RLC ID found */
            {
              if(SUCCESS != tdsrrcsend_send_rrc_status_message(
                 T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError,
                                               // The protocol error cause to be 
                                               // placed in the message
                 0,                            // Set transaction id to 0 since
                                               // ASN1 decoding failed
                 tdsrrc_ReceivedMessageType_rrcConnectionRelease,     // Id  of the received message
                 TDSRRC_PROCEDURE_RCR,            // RRC procedure
                 lc_id,                        /* Logical Channel Id */
                 TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                         /* Indicates whether L2 Ack 
                                                  required or not */
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to transmit RRC STATUS message");
              }  
              /* Reset the state machine and procedure ends */
              tdsrrcrcr_substate = TDSRRCRCR_INITIAL;
            } /* Valid RLC ID found */
        
          }
          else /* DCCH ASN1 decoding is successful */
          {
            boolean n308_presence       = FALSE;

            boolean rel5_critical_extn_supported = FALSE;

            /*lint -save -e613 */
            if(dcch_msg_ptr->message.u.rrcConnectionRelease.t ==
              T_tdsrrc_RRCConnectionRelease_later_than_r3 )
            {
              /* Update Transaction Id */
              tdsrrcrcr_transaction_identifier = dcch_msg_ptr->message.u.
                   rrcConnectionRelease.u.later_than_r3.rrc_TransactionIdentifier;

              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR received with Critical Extensions");
              
              if(dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.t ==
                T_tdsrrc_RRCConnectionRelease_criticalExtensions_1_r4)
              {
                rel5_critical_extn_supported = TRUE;      
                if(dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                    criticalExtensions.u.r4.rrcConnectionRelease_r4.m.rplmn_informationPresent)
                {
                  /*lint -save -e740 */
                  local_rplmn_information = (tdsrrc_Rplmn_Information_r4 *)&(dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.rrcConnectionRelease_r4.rplmn_information);
                  /*lint -restore */
                }
                if(dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                    criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent)
                {
                  if(dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
					criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v690NonCriticalExtensionsPresent)
                  {
                    if(dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v690NonCriticalExtensions.
                        rrcConnectionRelease_v690ext.m.redirectionInfo_v690extPresent)
                    {
                      local_redirect_info_ptr = (tdsrrc_RedirectionInfo_r6 *)&(dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                        v690NonCriticalExtensions.rrcConnectionRelease_v690ext.redirectionInfo_v690ext);
                      if(local_redirect_info_ptr != NULL)
                      {
                         tdsrrcrcr_save_redirection_info(local_redirect_info_ptr);
                      }
                    }
                  }
                }
                /* Update RRC connection release cause */
                tdsrrc_release_cause = 
                  tdsrrcrcr_update_release_cause(dcch_msg_ptr->message.u.
                  rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.
                 rrcConnectionRelease_r4.releaseCause);

                if(dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.
			  	u.r4.rrcConnectionRelease_r4.m.n_308Present)
                {
                  tdsrrcrcr_n_308 = 
                   dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.
                  rrcConnectionRelease_r4.n_308;
                  n308_presence = TRUE;
                } 

          
                if((tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)
                  &&(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7))         
                {
                  if((dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
					criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
                      (dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                      criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.
                      v690NonCriticalExtensionsPresent))
                  {
                    tdsrrcrcr_process_690non_critical_ext(&dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.
                        v4d0NonCriticalExtensions.v690NonCriticalExtensions);
                  }
                }
                else
                {
                  tdsrrc_high_mobility_ind_in_rcr = FALSE;
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"High Mobility Ind set to FALSE.");                  
                }
#ifdef FEATURE_TDSCDMA_REL8
                if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
                {
                  if((dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions
					.u.r4.m.v4d0NonCriticalExtensionsPresent) &&
                    (dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.
                    u.r4.v4d0NonCriticalExtensions.m.v690NonCriticalExtensionsPresent) &&
                    (dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.
                    u.r4.v4d0NonCriticalExtensions.v690NonCriticalExtensions.m.v770NonCriticalExtensionsPresent) &&
                    (dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
                    v690NonCriticalExtensions.v770NonCriticalExtensions.m.v860NonCriticalExtensionsPresent))
                  {
                    tdsrrcrcr_process_860non_critical_ext(&dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.
                        v4d0NonCriticalExtensions.v690NonCriticalExtensions.v770NonCriticalExtensions.v860NonCriticalExtensions.rrcConnectionRelease_v860ext);
                  }
                }
#endif /*FEATURE_TDSCDMA_REL8*/
                if (tdsrrc_release_cause == RRC_REL_CAUSE_NORMAL)
                {
                  if (mm_per_subs_get_cs_domain_no_rsp_type(tdsrrc_get_as_id()) == CALL_STATE_MO_REJECT_CAUSE15)
                  {
                    tdsrrc_cs_domain_error.mo_error_count++;
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS_domain_error: CALL_STATE_MO_REJECT_CAUSE15 after calling NAS API after receiving RRCConnectionRelease, mo_error_count:%d", tdsrrc_cs_domain_error.mo_error_count);                                   
                  }
                  else if(mm_per_subs_get_cs_domain_no_rsp_type(tdsrrc_get_as_id()) == CALL_STATE_MT_PAGE_NO_SETUP)
                  {
                    tdsrrc_cs_domain_error.mt_error_count++;
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS_domain_error: CALL_STATE_MT_PAGE_NO_SETUP after calling NAS API after receiving RRCConnectionRelease, mt_error_count:%d", tdsrrc_cs_domain_error.mt_error_count);                                                       
                  }
				  else
			      {
				  	TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS_domain_error: reset error count"); 
			        tdsrrc_cs_domain_error.mo_error_count = 0;
                    tdsrrc_cs_domain_error.mt_error_count = 0;
			      }
                  if((tdsrrc_cs_domain_error.mo_error_count == TDSMAX_CS_DOMAIN_MO_ERROR_COUNT) ||
                       (tdsrrc_cs_domain_error.mt_error_count == TDSMAX_CS_DOMAIN_MT_ERROR_COUNT))
                  {
                    tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, (uint32)tds_cs_domain_bar_timer_length); 
                    tdsrrc_cs_domain_error.mo_error_count = 0;
                    tdsrrc_cs_domain_error.mt_error_count = 0;
                  }
                }

              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR Critical Extensions not supported");      
                /* Send RRC Connection Release Complete Message with Protocol Error
                and update the release cause to "unspecified" */
                tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;

                /* Set the Error Indication */
                tdsrrc_protocol_error_information.diagnosticsType.t = 
                T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

                tdsrrc_protocol_error_information.diagnosticsType.u.type1 = 
                  rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_ProtocolErrorInformation_type1);
                
                if(tdsrrc_protocol_error_information.diagnosticsType.u.type1 == NULL)
                {
                  ERR_FATAL("memory alloc failure",0,0,0);
                }
                
                tdsrrc_protocol_error_information.diagnosticsType.u.type1->protocolErrorCause = 
                  tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended ;

                tdsrrcrcr_n_308 = 0;
                tdsrrcrcr_error_ind = TDSRRCRCR_INCLUDE_ERROR_INDICATION;
              }
            } /* End of if of later than release 3 present */
            /* unsupport rel99 OTA */
            else
            {
              /* Send RRC Connection Release Complete Message with Protocol Error
                        and update the release cause to "unspecified" */
              tdsrrc_release_cause = RRC_REL_CAUSE_UNSPEC;

              /* Set the Error Indication */
              tdsrrc_protocol_error_information.diagnosticsType.t = 
              T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

              tdsrrc_protocol_error_information.diagnosticsType.u.type1 = 
                rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_ProtocolErrorInformation_type1);

              if(tdsrrc_protocol_error_information.diagnosticsType.u.type1 == NULL)
              {
                ERR_FATAL("memory alloc failure",0,0,0);
              }

              tdsrrc_protocol_error_information.diagnosticsType.u.type1->protocolErrorCause = 
                tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended ;

              tdsrrcrcr_n_308 = 0;
              tdsrrcrcr_error_ind = TDSRRCRCR_INCLUDE_ERROR_INDICATION;
            }

            if (local_rplmn_information != NULL)
            {
              uint32 local_idx=0;
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RPLMN Info in RRC Conn Rel msg");

              /* Validate the Frequency Information and then add it to
               * the frequency/frequencies to the acquisition database
               */
              if(local_rplmn_information->m.tdd128_UMTS_Frequency_ListPresent)
              {
              tdd_freq_list_ptr = &local_rplmn_information->tdd128_UMTS_Frequency_List;

            while(tdd_freq_list_ptr->n > local_idx)
              {
                /* Validate the uarfcn_Nt based on
                 * UE's band preference settings before adding it to the 
                 * acquisition database
                 */
                 /* add comment: this function tdsrrc_validate_tds_frequency will be modified begin*/
                if(tdsrrc_validate_tds_frequency(tdd_freq_list_ptr->elem[local_idx].uarfcn_Nt)
                     == FAILURE)
                /* add comment: this function tdsrrc_validate_tds_frequency will be modified end*/
                {
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Band for freq %d not supported",tdd_freq_list_ptr->elem[local_idx].uarfcn_Nt);
                }
                else
                {
                if(tdsrrcccm_get_curr_camped_freq() != (uint16)tdd_freq_list_ptr->elem[local_idx].uarfcn_Nt)
                  {
                    if(tdsrrccsp_update_acq_db((uint16)tdd_freq_list_ptr->elem[local_idx].uarfcn_Nt) == SUCCESS)
                    {
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"ACQ entry added to ACQ DB");
                      acq_db_updated = TRUE;
                    }
                    else
                    {
                      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ACQ entry not added to ACQ DB");
                      break;
                    }
                  }
                  else
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdd list contains curr camped freq");
                    tdd_freq_list_contains_camped_freq = TRUE;
                  }
                }

                local_idx++;  
              }
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring RPLMN info as tdd128_UMTS_Frequency_List is not present.");
              }

              /* this is to get curr camped freq at the top of acq db so that
               * if on tx to idle, acq fails on the cpid then CSP can still try
               * the camped freq before trying acq on other freqs in the acq db
               */
              if((tdd_freq_list_contains_camped_freq)
                 ||
                 (   (acq_db_updated)
                  && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC1)
                  && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC2)
                  && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC3)
                  && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC4)
                 )
                )
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Add curr camped freq to the ACQ DB");
                (void) tdsrrccsp_update_acq_db(tdsrrcccm_get_curr_camped_freq());
                acq_db_updated = TRUE;
              }
            } /* End of if: rplmn_info_presence */

            /* Gets the current state of the RRC */
            current_state = tdsrrc_get_state();

            /* Switch on RRC state */
            switch(current_state)
            {
              case TDSRRC_STATE_CELL_DCH:  
                /* reset the  counters */
 
                tdsrrc_v_308 = 0; 
                #ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
                tdsrrcrcr_cancel_reconfig_proc();
                #endif
                if(rel5_critical_extn_supported == FALSE &&
                    (dcch_msg_ptr->message.u.rrcConnectionRelease.t ==
                  T_tdsrrc_RRCConnectionRelease_later_than_r3))
                {
                  /* Build RRC Connection Release complete message and send the
                  same to the SEND CHAIN */ 

                  if(TDSRRCRCR_SUCCESS == 
                    tdsrrcrcr_send_connection_rel_complete_msg(
                    DCCH_UM_RADIO_BEARER_ID,
                    UE_MODE_UNACKNOWLEDGED,
                    tdsrrcrcr_error_ind) )
                  /* This function starts the Timer T308 */
                  {
                     TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Release Complete is sent");
                  
                     /* Increment the number of retransmission */
                     tdsrrc_v_308++; 

                     /* Also store the scr and pn info of the best cell in the 
                      * active set so that UE first attempts acquisition on this
                      * cell before trying ACQ DB 
                      */
                     if(   ((!acq_db_updated)
                        || ((TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC1)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC2)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC3)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC4)))
                        && (!tdsrrcrcr_redirection_info_present)
                       )
                     {
                       tdsrrccsp_get_aset_cpid_pn();
                     }

                     tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE;
                    if(TDSRRCCU_NOT_COMPLETED == 
                       tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_RCR) )
                    {

                        tdsrrccu_clear_pending_cu_procedure();
                                /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
                        if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
                        {
                          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");

                          /* RCR waits for the release confirmation from the RRC LLC */         
                          tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;    
                        }
                        else   /* Failed to send a internal command to RRC LLC.*/
                        {      
                          ERR_FATAL("Unable send internal cmd to RRC LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
                        }
                        tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;

                    }
                  }
                  else
                  {
                    /* It is OK even if UE fails to transmit RRC Connection
                    Release Complete. Eventually UTRAN will release the 
                    resources */
                    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to send RRC Conn Rel Complete");
                  }
                  return;
                }

                /* Check whether Max number of retransmissions field is present */
                if(n308_presence == TRUE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"N308 is %d",tdsrrcrcr_n_308);
                  
                  /* Build RRC Connection Release complete message and send the
                     same to the SEND CHAIN */              
                  if(TDSRRCRCR_SUCCESS == 
                       tdsrrcrcr_send_connection_rel_complete_msg(
                       DCCH_UM_RADIO_BEARER_ID,
                       UE_MODE_UNACKNOWLEDGED, 
                       tdsrrcrcr_error_ind) )

                  /* This function starts the Timer T308 */
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Release Complete is sent");

                    /* Increment the number of retransmission */
                    tdsrrc_v_308++; 

                    /* If Connection Release Request from the Network had a 
                     * frequency information then do not try acquisition on
                     * the best cell in the active set
                     */
                    if(   ((!acq_db_updated)
                       || ((TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC1)
                        && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC2)
                        && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC3)
                        && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC4)))
                       && (!tdsrrcrcr_redirection_info_present)
                      )
                    {
                      /* Since Connection Release Request from the Network did
                       * not come with frequency information, store the scr and
                       * pn info of the best cell in the active set so that UE 
                       * first attempts acquisition on this cell before trying 
                       * ACQ DB 
                       */       
                      tdsrrccsp_get_aset_cpid_pn();
                    }

                    tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE;
                      if(TDSRRCCU_NOT_COMPLETED == 
                       tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_RCR) )
                    {

                        tdsrrccu_clear_pending_cu_procedure();
                                /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
                        if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
                        {
                          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");

                          /* RCR waits for the release confirmation from the RRC LLC */         
                          tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;    
                        }
                        else   /* Failed to send a internal command to RRC LLC.*/
                        {      
                          ERR_FATAL("Unable send internal cmd to RRC LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
                        }
                        tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;

                    }
                  }
                  else
                  {
                    /* It is OK even if UE fails to transmit RRC Connection
                    Release Complete. Eventually UTRAN will release the 
                    resources */
                    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to send RRC Conn Rel Complete");
                  }
                }
                else
                {
                  // This is a race condition and  Cell Updated would have started . 
                  // RRC Connection Release procedure transmits RRC Connection
                  // Release Complete and includes error indication.               
                  TDSRRC_MSG0(MSG_LEGACY_LOW,"The msg do not conatin conditional field");

                  /* Set the Error Indication */
                  tdsrrc_protocol_error_information.diagnosticsType.t = 
                    T_tdsrrc_ProtocolErrorInformation_diagnosticsType_type1;

                  tdsrrc_protocol_error_information.diagnosticsType.u.type1 = 
                    rtxMemAllocTypeZ(&tdsenc_ctxt,tdsrrc_ProtocolErrorInformation_type1);
                  if(tdsrrc_protocol_error_information.diagnosticsType.u.type1 == NULL)
                  {
                    ERR_FATAL("memory alloc failure",0,0,0);
                  }

                  tdsrrc_protocol_error_information.diagnosticsType.u.type1->
                  protocolErrorCause = tdsrrc_ProtocolErrorCause_messageNotCompatibleWithReceiverState ;  

                  /* Build RRC Connection Release complete message and send the
                     same to the SEND CHAIN */              
                  if(TDSRRCRCR_SUCCESS == 
                       tdsrrcrcr_send_connection_rel_complete_msg(
                       DCCH_UM_RADIO_BEARER_ID,
                       UE_MODE_UNACKNOWLEDGED,
                       tdsrrcrcr_error_ind) )
                    /* This function starts the Timer T308 */
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Release Complete is sent");

                    /* Increment the number of retransmission */
                    tdsrrc_v_308++; 

                    /* If Connection Release Request from the Network had a 
                     * frequency information then do not try acquisition on
                     * the best cell in the active set
                     */
                    if(   ((!acq_db_updated)
                        || ((TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC1)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC2)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC3)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC4)))
                         && (!tdsrrcrcr_redirection_info_present)
                       )
                    {
                      /* Since Connection Release Request from the Network did
                       * not come with frequency information, store the scr and
                       * pn info of the best cell in the active set so that UE 
                       * first attempts acquisition on this cell before trying 
                       * ACQ DB 
                       */
                      tdsrrccsp_get_aset_cpid_pn();
                    }

                    tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE;
                  
                  }
                  else
                  {
                    /* It is OK even if UE fails to transmit RRC Connection
                    Release Complete. Eventually UTRAN will release the 
                    resources */
                    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to send RRC Conn Rel Complete");
                  }
                }
                break;

              case TDSRRC_STATE_CELL_FACH:

                /* Check whether Max number of retransmissions field is present */
                if(n308_presence == TRUE)
                {
                  // This is a race condition and  Cell Updated would have started . 
                  // RRC Connection Release procedure transmits RRC Connection
                  // Release Complete and includes error indication.          
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"N308 in rrcrcr in CELL_FACH. Ignoring");
                }

                /* Query if CELL UPDATE has a valid C-RNTI to tranmit in
                   CELL_FACH state */
                if(TDSRRCCU_NOT_COMPLETED == 
                     tdsrrccu_get_cell_update_complete_status(TDSRRC_PROCEDURE_RCR) )
                {
                  if(   ((!acq_db_updated)
                     || ((TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC1)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC2)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC3)
                         && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC4)))
                      && (!tdsrrcrcr_redirection_info_present)
                    )
                  {
                    /* Since Connection Release Request from the Network did
                     * not come with frequency information, store the scr and
                     * pn info of the best cell in the active set so that UE 
                     * first attempts acquisition on this cell before trying 
                     * ACQ DB 
                     */       
                    tdsrrccsp_get_aset_cpid_pn();
                  }

                  /* CELL UPDATE is in process of getting a valid C-RNTI. */
                  tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_CELL_UPDATE_COMP_CNF;
                    tdsrrccu_clear_pending_cu_procedure();
                              /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
                    if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
                    {
                      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");

                      /* RCR waits for the release confirmation from the RRC LLC */         
                      tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;    
                    }
                    else   /* Failed to send a internal command to RRC LLC.*/
                    {      
                      ERR_FATAL("Unable send internal cmd to RRC LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
                    }
                    tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;
                }
                else
                {
                  #ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
                  tdsrrcrcr_cancel_reconfig_proc();
                  #endif
                  /* Build RRC Connection Release complete message and send the
                     same to the SEND CHAIN */              
                  if(TDSRRCRCR_SUCCESS == 
                       tdsrrcrcr_send_connection_rel_complete_msg(
                       DCCH_AM_RADIO_BEARER_ID,
                       UE_MODE_ACKNOWLEDGED_DATA, 
                       tdsrrcrcr_error_ind) )
                  /* This function starts the Timer TDSRRCRCR_TIMER_VALUE_FOR_L2_ACK */
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Release Complete is sent");

                    if(   ((!acq_db_updated)
                      || ((TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC1)
                          && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC2)
                          && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC3)
                          && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC4)))
                      && (!tdsrrcrcr_redirection_info_present)
                     )
                    {
                      /* Since Connection Release Request from the Network did
                       * not come with frequency information, store the scr and
                       * pn info of the best cell in the active set so that UE 
                       * first attempts acquisition on this cell before trying 
                       * ACQ DB 
                       */       
                      tdsrrccsp_get_aset_cpid_pn();
                    }

                    tdsrrcrcr_substate = 
                      TDSRRCRCR_WAIT_FOR_L2ACK_FOR_RRC_CONN_REL_COMPLETE;
                  }
                  else
                  {
                    /* It is OK even if UE fails to transmit RRC Connection
                    Release Complete. Eventually UE will release the 
                    resources after L2 ACK expires */
                    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to send RRC Conn Rel Complete");
                  } 
                }
                break;

              default:
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Con Rel Msg rcvd in Invalid state:%d",current_state);
                break;
            }
            /*lint -restore */
          }/* DCCH ASN1 decoding is successful */
        }
        else /* Invalid message is routed to RCR procedure */
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid t: %d for RCR proc",message_choice);
        }
      }
      else if (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == 
                tdsrrc_DL_CCCH_Message_PDU)
      {

        //local var to see if group_release match was success/failure
        boolean group_release_match = FALSE;
        boolean group_release_present = FALSE;
        tdsrrc_RRCConnectionRelease_CCCH_groupIdentity * local_group_release_list;


        message_choice =
          tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                               cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

        if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
        {
          /* Get the OTA message pointer */
          ccch_msg_ptr = (tdsrrc_DL_CCCH_Message *) 
                       cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
        }
        else
        { /* ASN1 decoding failed */
          tdsrrc_protocol_error_reject = TRUE;
        }
      
        if(tdsrrc_protocol_error_reject != TRUE) /* CCCH ASN1 decoding successful */
        {
          if( message_choice  == 
             T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rcvd RRC Connection Release CCCH");
            tdsrrcrcr_redirection_info_present = FALSE;
            rrc_release_cause_recvd = tdsrrc_ReleaseCause_unspecified;

            /*lint -save -e613 */
            if(ccch_msg_ptr->message.u.rrcConnectionRelease.t ==
              T_tdsrrc_RRCConnectionRelease_CCCH_later_than_r3 )
            {
              /* Critical externsions go here */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Critical extensions specified");
              if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.t == 
                   T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_3_r4)
              {
                /* Translate the U-RNTI */
                tdsrrc_translate_urnti(&ccch_msg_ptr->message.u.rrcConnectionRelease.
                                   u.later_than_r3.u_RNTI,
                                   &u_rnti);

                rrc_release_cause_recvd = ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.rrcConnectionRelease_CCCH_r4.rrcConnectionRelease.releaseCause;
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rel cause is %d",rrc_release_cause_recvd);
                
		            if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
						            criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent)
						    {
						      if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
						         criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v690NonCriticalExtensionsPresent)
						      {
						        if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.v690NonCriticalExtensions.
						           rrcConnectionRelease_v690ext.m.redirectionInfo_v690extPresent)
						        {
						          local_redirect_info_ptr = (tdsrrc_RedirectionInfo_r6 *)&(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.r4.v4d0NonCriticalExtensions.
						              v690NonCriticalExtensions.rrcConnectionRelease_v690ext.redirectionInfo_v690ext);
						          if(local_redirect_info_ptr != NULL)
						          {
						             tdsrrcrcr_save_redirection_info(local_redirect_info_ptr);
						          }
						        }
						      }
						    }
#ifdef FEATURE_TDSCDMA_REL8
                if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
                {
                  if((ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
						            criticalExtensions.u.r4.m.v4d0NonCriticalExtensionsPresent)
                      && (ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
						         criticalExtensions.u.r4.v4d0NonCriticalExtensions.m.v690NonCriticalExtensionsPresent) &&
                      (ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
						         criticalExtensions.u.r4.v4d0NonCriticalExtensions.v690NonCriticalExtensions.
						         m.v860NonCriticalExtensionsPresent))
                  {
                     tdsrrcrcr_process_860non_critical_ext(&ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                           criticalExtensions.u.r4.v4d0NonCriticalExtensions.v690NonCriticalExtensions.v860NonCriticalExtensions.rrcConnectionRelease_v860ext);
                  }
                }
#endif /*FEATURE_TDSCDMA_REL8*/
              }

              if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
              {
               
                /* Translate the U-RNTI */
                tdsrrc_translate_urnti(&ccch_msg_ptr->message.u.rrcConnectionRelease.
                                   u.later_than_r3.u_RNTI,
                                   &u_rnti);
                //Check if Group-Release-IE's are received and see if a Group-Release Match is met
                if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.t == 
                   T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_3_criticalExtensions)
                {
                  if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.
                     criticalExtensions.m.groupIdentityPresent)
                  {
                    OSRTDListNode * grp_rel_info_ptr=NULL;
                    group_release_present = TRUE;
  
                    local_group_release_list = &ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                      criticalExtensions.u.criticalExtensions.groupIdentity;
                    grp_rel_info_ptr =  local_group_release_list->head;
                    
                    while(grp_rel_info_ptr!=NULL)
                    {
                      if(SUCCESS == tdsrrcpg1_compare_group_release_info(grp_rel_info_ptr->data))
                      {
                        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Group-Release Matched");
                        group_release_match = TRUE;
                        break;
                      }
                      grp_rel_info_ptr = grp_rel_info_ptr->next;
                    }
                  }
                  else
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"groupIdentity is NOT Present");
                  }
                }
  
                if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.t == 
                    T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_3_criticalExtensions)
                {
                  if (ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.
                      criticalExtensions.criticalExtensions.t == 
                      T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_1_r5)
                  {
                    rrc_release_cause_recvd = ccch_msg_ptr->message.u.rrcConnectionRelease.
                      u.later_than_r3.criticalExtensions.u.criticalExtensions.criticalExtensions.u.r5.
                      rrcConnectionRelease_CCCH_r5.rrcConnectionRelease.releaseCause;
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rel cause is %d",rrc_release_cause_recvd);

                    if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                        criticalExtensions.u.criticalExtensions.criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent)
                    {
                      if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.criticalExtensions.criticalExtensions.u.r5.
                            v690NonCriticalExtensions.rrcConnectionRelease_v690ext.m.redirectionInfo_v690extPresent)
                      {
                        local_redirect_info_ptr = (tdsrrc_RedirectionInfo_r6 *)&(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.
                           criticalExtensions.criticalExtensions.u.r5.v690NonCriticalExtensions.rrcConnectionRelease_v690ext.redirectionInfo_v690ext);
                        if(local_redirect_info_ptr != NULL)
                        {
                           tdsrrcrcr_save_redirection_info(local_redirect_info_ptr);
                        }
                      }
                    }
#ifdef FEATURE_TDSCDMA_REL8
                    if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
                    {
                      if((ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.criticalExtensions.
                        criticalExtensions.u.r5.m.v690NonCriticalExtensionsPresent)
                        && (ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.criticalExtensions.
                        criticalExtensions.u.r5.v690NonCriticalExtensions.m.v860NonCriticalExtensionsPresent))
                      {
                        tdsrrcrcr_process_860non_critical_ext(&ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                            criticalExtensions.u.criticalExtensions.criticalExtensions.u.r5.v690NonCriticalExtensions.v860NonCriticalExtensions.
                            rrcConnectionRelease_v860ext);
                      }
                    }
#endif /*FEATURE_TDSCDMA_REL8*/
                  }
                }
              }
            }

            //update the release cause
            tdsrrc_release_cause = tdsrrcrcr_update_release_cause(rrc_release_cause_recvd);

            if(TDSU_RNTI_NOT_PRESENT == tdsrrcllc_get_current_urnti(&u_rnti_local) )
            {
              ERR_FATAL("URNTI is not present",0,0,0);
            }

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"u_rnti_local %d, u_rnti %d, group_release_match %d is", u_rnti_local, u_rnti, group_release_match);

            //release connection if group release is present and if it was a match  or
            //if group release is absent and the urnti's matched
            if(((TRUE == group_release_present) &&  (TRUE == group_release_match)) ||
               ((FALSE == group_release_present) && (u_rnti_local == u_rnti)))
            {
              if (tdsrrc_release_cause == RRC_REL_CAUSE_NORMAL)
              {
                if (mm_per_subs_get_cs_domain_no_rsp_type(tdsrrc_get_as_id()) == CALL_STATE_MO_REJECT_CAUSE15)
                {
                  tdsrrc_cs_domain_error.mo_error_count++;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS_domain_error: CALL_STATE_MO_REJECT_CAUSE15 after calling NAS API after receiving RRCConnectionRelease, mo_error_count:%d", tdsrrc_cs_domain_error.mo_error_count);                                                     
                }
                else if(mm_per_subs_get_cs_domain_no_rsp_type(tdsrrc_get_as_id()) == CALL_STATE_MT_PAGE_NO_SETUP)
                {
                  tdsrrc_cs_domain_error.mt_error_count++;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CS_domain_error: CALL_STATE_MT_PAGE_NO_SETUP after calling NAS API after receiving RRCConnectionRelease, mt_error_count:%d", tdsrrc_cs_domain_error.mt_error_count);                                                     
                }
				else
			    {
			      TDSRRC_MSG0(MSG_LEGACY_HIGH,"CS_domain_error: reset error count"); 				
			      tdsrrc_cs_domain_error.mo_error_count = 0;
                  tdsrrc_cs_domain_error.mt_error_count = 0;
			    }
                if((tdsrrc_cs_domain_error.mo_error_count == TDSMAX_CS_DOMAIN_MO_ERROR_COUNT) ||
                    (tdsrrc_cs_domain_error.mt_error_count == TDSMAX_CS_DOMAIN_MT_ERROR_COUNT))
                {
                  tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, (uint32)tds_cs_domain_bar_timer_length); 
                  tdsrrc_cs_domain_error.mo_error_count = 0;
                  tdsrrc_cs_domain_error.mt_error_count = 0;
                }
              }
              if(!tdsrrcrcr_redirection_info_present)
              {
                /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
                if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
  
                  if(   (!acq_db_updated)
                      || ((TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC1)
                          && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC2)
                          && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC3)
                          && (TDSRRC_CSP_GET_INT_MCC(camped_plmn) != TDSJAPAN_MCC4))
                    )
                  {
                    /* Since Connection Release Request from the Network did
                     * not come with frequency information, store the scr and
                     * pn info of the best cell in the active set so that UE 
                     * first attempts acquisition on this cell before trying 
                     * ACQ DB 
                     */       
                    tdsrrccsp_get_aset_cpid_pn();
                  }
  
                  /* RCR waits for the release confimration from the RRC LLC */
                  tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;
                }
                else   /* Failed to send a internal command to RRC LLC.*/
                {      
                  ERR_FATAL("Unable send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
                }
              }
              else
              {
                tdsrrcrcr_send_redirection_req();
              }
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"U_RNTI Mismatch. Ignore");
            }
            /*lint -restore */
          } 
          else
          {
            /* Incompatible message is received. Hence Trash it */
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid msg: %d for SDU type: %d rcvd",message_choice,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type );
          }
        }/* CCCH ASN1 decoding successful */
        else
        {
  #if(!defined(T_WINNT))
            /*log the protocol error*/
            tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
                                  (uint8)TDSRRC_PROCEDURE_RCR,
                                    TDSRRCLOG_PROTOCOL_ERROR,
                                    TDSRRCLOG_ASN1_ENC_ERR
                                   );
  #endif
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid PDU type: %d is rcvd",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
      }
      break;
    case TDSRRC_STATE_CHANGE_IND:
      TDSRRC_MSG2(MSG_LEGACY_MED,"Unexpected Event:%d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrcr_substate);
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrcr_substate);
  }
 
} /* tdsrrcrcr_process_rcr_initial_substate */


/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_wait_for_t308_timer_expire_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE  substate of 
  CELL_DCH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_wait_for_t308_timer_expire_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {     
    case TDSRRC_DOWNLINK_SDU_IND:       
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR proc already started. Msg Ignored");
      break;

    case TDSRRC_STATE_CHANGE_IND:
      /* stop the timer T308 */
        tdsrrctmr_stop_timer(TDSRRCTMR_T_308_TIMER);
      
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
        /* Reset the RCE state machine */
        tdsrrcrcr_substate = TDSRRCRCR_INITIAL;

      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event: %d",cmd_ptr->cmd.state_change_ind.new_state ,0,0);
      }
      break;

     case TDSRRC_T308_EXPIRED_IND:
      if(tdsrrc_v_308 <= tdsrrcrcr_n_308)               
      {        
        /* Resend RRC connection Release Complete Message and this function 
           restarts the T308 Timer */
        if(TDSRRCRCR_SUCCESS == 
             tdsrrcrcr_send_connection_rel_complete_msg(
             DCCH_UM_RADIO_BEARER_ID,
             UE_MODE_UNACKNOWLEDGED,
             TDSRRCRCR_NOT_INCLUDE_ERROR_INDICATION))
        
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Release Complete is sent");

          tdsrrc_v_308++;               
        }
        else
        { 
          /* It is OK even if UE fails to transmit RRC Connection
          Release Complete. Eventually UTRAN will release the 
          resources */
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to send RRC Conn Rel Complete");
        }
      }
      else /* Reached maximum allowed re-tries. Wait for T308 time before
              releasing the Radio resources */
      {
        /* If saved redirection info from Conn Release is present, send the redirection req to CSP*/
        if(tdsrrcrcr_redirection_info_present)
        {
          tdsrrcrcr_send_redirection_req();
        }
        else
        {
          /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
          if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
            /* RCR waits for the release confimration from the RRC LLC */         
            tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;    
          }
          else    /* Failed to send a internal command to RRC LLC.*/
          {      
            ERR_FATAL("Unable send to internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
          }
        }
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrcr_substate);
      break;
  }          
} /* tdsrrcrcr_process_rcr_wait_for_t308_timer_expire_substate */

/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_wait_for_cell_update_comp_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in TDSRRCRCR_WAIT_FOR_CELL_UPDATE_COMP_CNF  substate of 
  CELL_FACH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_wait_for_cell_update_comp_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  tdsrrc_DL_CCCH_Message *ccch_msg_ptr;         /* Pointer to Downlink CCCH
                                                message */
  tdsrrc_int_u_rnti_type u_rnti = 0;            /* Local varibale to store U-RNTI */
 
  tdsrrc_int_u_rnti_type u_rnti_local;          /* Local varibale to store U-RNTI 
                                                received from RRCLLC */

  tdsrrc_ReleaseCause     rrc_release_cause_recvd = tdsrrc_ReleaseCause_unspecified;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {     
    case TDSRRC_DOWNLINK_SDU_IND:       

      if (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == 
          tdsrrc_DL_CCCH_Message_PDU)
      {
        //local var to see if group_release match was success/failure
        boolean group_release_match = FALSE;
        boolean group_release_present = FALSE;

        if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
        {                 /* CCCH ASN1 decoding successful */
          ccch_msg_ptr = (tdsrrc_DL_CCCH_Message *) 
                         cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

          if(ccch_msg_ptr->message.t == 
             T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Conn Rel received on CCCH");
            tdsrrcrcr_redirection_info_present = FALSE;
            if(ccch_msg_ptr->message.u.rrcConnectionRelease.t ==
              T_tdsrrc_RRCConnectionRelease_CCCH_later_than_r3)
            {
              if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
              {
                tdsrrc_RRCConnectionRelease_CCCH_groupIdentity * local_group_release_list;

                /* Translate the U-RNTI */
                tdsrrc_translate_urnti(&ccch_msg_ptr->message.u.rrcConnectionRelease.
                                   u.later_than_r3.u_RNTI,
                                   &u_rnti);


                //check if Group Release IE's are present and see if Group Release Match is met
                if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.t == 
                   T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_3_criticalExtensions)
                {
                  OSRTDListNode * grp_rel_info_ptr=NULL;
                  if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.
                     criticalExtensions.m.groupIdentityPresent)
                  {
                    group_release_present = TRUE;

                    local_group_release_list = &ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                      criticalExtensions.u.criticalExtensions.groupIdentity;
					grp_rel_info_ptr= local_group_release_list->head;

                    while(grp_rel_info_ptr != NULL)
                    {
                      if(SUCCESS == tdsrrcpg1_compare_group_release_info(grp_rel_info_ptr->data))
                      {
                        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Group-Release Matched");
                        group_release_match = TRUE;
                        break;
                      }
                      grp_rel_info_ptr = grp_rel_info_ptr->next;;
                    }
                  }
                }

                if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.t == 
                    T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_3_criticalExtensions)
                {
                  if (ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.
                      criticalExtensions.criticalExtensions.t == 
                      T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_1_r5)
                  {
                    rrc_release_cause_recvd = ccch_msg_ptr->message.u.rrcConnectionRelease.
                      u.later_than_r3.criticalExtensions.u.criticalExtensions.criticalExtensions.u.r5.
                      rrcConnectionRelease_CCCH_r5.rrcConnectionRelease.releaseCause;
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rel cause is %d",rrc_release_cause_recvd);
                  }
                }
              }
            }
            else
            {
                rrc_release_cause_recvd = ccch_msg_ptr->message.u.rrcConnectionRelease.
              u.r3.rrcConnectionRelease_CCCH_r3.rrcConnectionRelease.releaseCause;
               /* Translate the U-RNTI */
               tdsrrc_translate_urnti(&ccch_msg_ptr->message.u.rrcConnectionRelease.
                                 u.r3.rrcConnectionRelease_CCCH_r3.u_RNTI,
                                 &u_rnti); 
            }

            //update the release cause
            tdsrrc_release_cause = tdsrrcrcr_update_release_cause(rrc_release_cause_recvd);


            if(TDSU_RNTI_NOT_PRESENT == tdsrrcllc_get_current_urnti(&u_rnti_local) )
            {
              ERR_FATAL("URNTI is not present",0,0,0);
            }

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"u_rnti_local %d, u_rnti %d, group_release_match %d is", u_rnti_local, u_rnti, group_release_match);

            //release connection if group release is present and if it was a match  or
            //if group release is absent and the urnti's matched
            if(((TRUE == group_release_present) &&  (TRUE == group_release_match)) ||
               ((FALSE == group_release_present) && (u_rnti_local == u_rnti)))
            {
              if(!tdsrrcrcr_redirection_info_present)
              {
                /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
                if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
  
                  /* RCR waits for the release confimration from the RRC LLC */         
                  tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;    
                }
                else   /* Failed to send a internal command to RRC LLC.*/
                {      
                  ERR_FATAL("Unable send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
                }
              }
              else
              {
                tdsrrcrcr_send_redirection_req();
              }
            }
            else
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"U-RNTI Mismatch. Ignore");
            }
          } 

        }/* CCCH ASN1 decoding successful */
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to DECODE RCR message");
#if(!defined(T_WINNT))
          /*log the protocol error*/
          tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
                                (uint8)TDSRRC_PROCEDURE_RCR,
                                  TDSRRCLOG_PROTOCOL_ERROR,
                                  TDSRRCLOG_ASN1_ENC_ERR
                                 );
#endif
        }
      }
      else
      {
          /* Incompatible message is received. Hence Trash it */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Incompatible for SDU type: %d received",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type );
      }

      break;

    case TDSRRC_STATE_CHANGE_IND:
            
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {
        /* Reset the RCE state machine */
        tdsrrcrcr_substate = TDSRRCRCR_INITIAL;

      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event: %d",cmd_ptr->cmd.state_change_ind.new_state ,0,0);
      }
      break;

    case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
      if(cmd_ptr->cmd.cu_complete_cnf.procedure == TDSRRC_PROCEDURE_RCR)
      {                
        /* Send RRC connection Release Complete Message since RRC has valid
           C-RNTI */
        if(TDSRRCRCR_SUCCESS == 
             tdsrrcrcr_send_connection_rel_complete_msg(
             DCCH_UM_RADIO_BEARER_ID,
             UE_MODE_UNACKNOWLEDGED,
             tdsrrcrcr_error_ind))
        
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Connection Release Complete is sent");                                   

          tdsrrcrcr_substate = 
            TDSRRCRCR_WAIT_FOR_L2ACK_FOR_RRC_CONN_REL_COMPLETE;
        }
        else
        {
          ERR_FATAL("Unable to send RRC Conn Rel Complete",0,0,0);
        }
      }
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_CELL_UPDATE_COMPLETE_CNF's proc: %d",cmd_ptr->cmd.cu_complete_cnf.procedure);
      }
      
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrcr_substate);
      break;
  }          
} /* tdsrrcrcr_process_rcr_wait_for_cell_update_comp_cnf_substate */


/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_wait_for_l2ack_for_rrc_conn_rel_complete_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection 
  Release procedure in TDSRRCRCR_WAIT_FOR_L2ACK_FOR_RRC_CONN_REL_COMPLETE
  substate of CELL_FACH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_wait_for_l2ack_for_rrc_conn_rel_complete_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  tdsrrc_DL_CCCH_Message *ccch_msg_ptr;         /* Pointer to Downlink CCCH
                                                message */
  tdsrrc_int_u_rnti_type u_rnti = 0;            /* Local varibale to store U-RNTI */
 
  tdsrrc_int_u_rnti_type u_rnti_local;          /* Local varibale to store U-RNTI 
                                                received from RRCLLC */

  tdsrrc_ReleaseCause     rrc_release_cause_recvd = tdsrrc_ReleaseCause_unspecified;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {     
    case TDSRRC_DOWNLINK_SDU_IND:

      if (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == 
          tdsrrc_DL_CCCH_Message_PDU)
      {
        //local var to see if group_release match was success/failure
        boolean group_release_match = FALSE;
        boolean group_release_present = FALSE;

        if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
        {                 /* CCCH ASN1 decoding successful */
          ccch_msg_ptr = (tdsrrc_DL_CCCH_Message *) 
                         cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

          if(ccch_msg_ptr->message.t == 
             T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC Conn Rel received on CCCH");
            tdsrrcrcr_redirection_info_present = FALSE;

            if(ccch_msg_ptr->message.u.rrcConnectionRelease.t ==
              T_tdsrrc_RRCConnectionRelease_CCCH_later_than_r3)
            {
              if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
              {
                tdsrrc_RRCConnectionRelease_CCCH_groupIdentity * local_group_release_list;

                /* Translate the U-RNTI */
                tdsrrc_translate_urnti(&ccch_msg_ptr->message.u.rrcConnectionRelease.
                                   u.later_than_r3.u_RNTI,
                                   &u_rnti);


                //check if Group Release IE's are present and see if Group Release Match is met
                if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.t == 
                   T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_3_criticalExtensions)
                {
                  OSRTDListNode * grp_rel_info_ptr=NULL;
                  if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.
                     criticalExtensions.m.groupIdentityPresent)
                  {
                    group_release_present = TRUE;
  
                    local_group_release_list = &ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.
                      criticalExtensions.u.criticalExtensions.groupIdentity;
					grp_rel_info_ptr= local_group_release_list->head;
  
                    while(NULL != grp_rel_info_ptr)
                    {
                      if(SUCCESS == tdsrrcpg1_compare_group_release_info(grp_rel_info_ptr->data))
                      {
                        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Group-Release Matched");
                        group_release_match = TRUE;
                        break;
                      }
                      grp_rel_info_ptr = grp_rel_info_ptr->next;
                    }
                  }
                }
  
                if(ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.t == 
                    T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_3_criticalExtensions)
                {
                  if (ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.criticalExtensions.u.
                      criticalExtensions.criticalExtensions.t == 
                      T_tdsrrc_RRCConnectionRelease_CCCH_criticalExtensions_1_r5)
                  {
                    rrc_release_cause_recvd = ccch_msg_ptr->message.u.rrcConnectionRelease.
                      u.later_than_r3.criticalExtensions.u.criticalExtensions.criticalExtensions.u.r5.
                      rrcConnectionRelease_CCCH_r5.rrcConnectionRelease.releaseCause;
                    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Rel cause is %d",rrc_release_cause_recvd);
                  }
                }
              }
            }
            else
            {
                rrc_release_cause_recvd = ccch_msg_ptr->message.u.rrcConnectionRelease.
              u.r3.rrcConnectionRelease_CCCH_r3.rrcConnectionRelease.releaseCause;
               /* Translate the U-RNTI */
               tdsrrc_translate_urnti(&ccch_msg_ptr->message.u.rrcConnectionRelease.
                                 u.r3.rrcConnectionRelease_CCCH_r3.u_RNTI,
                                 &u_rnti); 
            }

            //update the release cause
            tdsrrc_release_cause = tdsrrcrcr_update_release_cause(rrc_release_cause_recvd);


            if(TDSU_RNTI_NOT_PRESENT == tdsrrcllc_get_current_urnti(&u_rnti_local) )
            {
              ERR_FATAL("URNTI is not present",0,0,0);
            }

            TDSRRC_MSG3(MSG_LEGACY_HIGH,"u_rnti_local %d, u_rnti %d, group_release_match %d is", u_rnti_local, u_rnti, group_release_match);

            //release connection if group release is present and if it was a match  or
            //if group release is absent and the urnti's matched
            if(((TRUE == group_release_present) &&  (TRUE == group_release_match)) ||
               ((FALSE == group_release_present) && (u_rnti_local == u_rnti)))
            {
              /* Stop the timer for L2 ACK  */
              tdsrrctmr_stop_timer(TDSRRCTMR_RCR_L2ACK_TIMER);

              if(!tdsrrcrcr_redirection_info_present)
              {
                /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
                if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
  
                  /* RCR waits for the release confimration from the RRC LLC */         
                  tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;    
                }
                else   /* Failed to send a internal command to RRC LLC.*/
                {      
                  ERR_FATAL("Unable send internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
                }
              }
              else
              {
                tdsrrcrcr_send_redirection_req();
              }
            }
            else
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"U-RNTI Mismatch. Ignore");
            }
          } 

        }/* CCCH ASN1 decoding successful */
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Failed to DECODE RCR message");
#if(!defined(T_WINNT))
          /*log the protocol error*/
          tdsrrc_log_protocol_error( (uint8) tdsrrc_get_state(),
                                (uint8)TDSRRC_PROCEDURE_RCR,
                                  TDSRRCLOG_PROTOCOL_ERROR,
                                  TDSRRCLOG_ASN1_ENC_ERR
                                 );
#endif
        }
      }
      else
      {
          /* Incompatible message is received. Hence Trash it */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Incompatible for SDU type: %d received",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type );
      }

      break;

    case TDSRRC_STATE_CHANGE_IND:
      /* Stop the timer for L2 ACK  */
      tdsrrctmr_stop_timer(TDSRRCTMR_RCR_L2ACK_TIMER);

      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED))
      {        
        /* Reset the RCR state machine */
        tdsrrcrcr_substate = TDSRRCRCR_INITIAL;

      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event:%d",cmd_ptr->cmd.state_change_ind.new_state ,0,0);
      }
      break; 

    case TDSRRC_NEW_CELL_IND:
      /* Stop the timer for L2 ACK  */
      tdsrrctmr_stop_timer(TDSRRCTMR_RCR_L2ACK_TIMER);

      tdsrrcrcr_cell_reselection_occured = TRUE;
      TDSRRC_MSG1(MSG_LEGACY_LOW,"New cell ind %d",tdsrrcrcr_cell_reselection_occured);
      break;

    case TDSRRC_TIMER_FOR_RCR_L2ACK_EXPIRED_IND:
      /* There is a serious problem with RLC for Acknowledged mode
         transmission. However inform the NAS  and Release radio 
         resources and reset the state machine */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RCR L2 ACK sanity Timer expired"); 

      /* If saved redirection info from Conn Release is present, send the redirection req to CSP*/
      if(tdsrrcrcr_redirection_info_present)
      {
        tdsrrcrcr_send_redirection_req();
      }
      else 
      {
        /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
        if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
  
          /* Reset the RCR state machine */
          tdsrrcrcr_substate = TDSRRCRCR_INITIAL;
  
          /* Update the release cause */
          tdsrrc_release_cause = RRC_REL_DEEP_FADE;
        }
        else    /* Failed to send a internal command to RRC LLC.*/
        {      
          ERR_FATAL("Unable send to internal cmd to LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
        }      
      }
      break;

    case TDSRRC_DOWNLINK_L2ACK_IND:
      /* Stop the timer for L2 ACK  */
      tdsrrctmr_stop_timer(TDSRRCTMR_RCR_L2ACK_TIMER);

      if(cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS)
      {
        /* If saved redirection info from Conn Release is present, send the redirection req to CSP*/
        if(tdsrrcrcr_redirection_info_present)
        {
          tdsrrcrcr_send_redirection_req();
        }
        else
        {
          /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
          if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
  
            /* RCR waits for the release confirmation from the RRC LLC */         
            tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;    
          }
          else   /* Failed to send a internal command to RRC LLC.*/
          {      
            ERR_FATAL("Unable send internal cmd to RRC LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
          }
        }
      }
      else /* Failed to recieve successful L2ACK */
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"L2ACK for RRC Conn Rel Comp Msg not rcvd");

        /* If saved redirection info from Conn Release is present, send the redirection req to CSP*/
        if(tdsrrcrcr_redirection_info_present)
        {
          tdsrrcrcr_send_redirection_req();
        }
        else
        {
          /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
          if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
  
            /* Reset the procedure. */   
            tdsrrcrcr_substate = TDSRRCRCR_INITIAL;
  
            /* Update the release cause */
            tdsrrc_release_cause = RRC_REL_DEEP_FADE;
          }
          else   /* Failed to send a internal command to RRC LLC.*/
          {      
            ERR_FATAL("Unable send internal cmd to RRC LLC: %d",TDSRRC_CHANNEL_CONFIG_REQ,0,0);
          }
        }
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrcr_substate);
      break;
  }          
} /* tdsrrcrcr_process_rcr_wait_for_l2ack_for_rrc_conn_rel_complete_substate */
/*===========================================================================

FUNCTION tdsrrcrcr_process_rcr_wait_for_srb_release_cnf_substate

DESCRIPTION
  This function processes all events that are dispatched to RRC Connection
  Release procedure in TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF substate of either
  CELL_DCH State or CELL_FACH State.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_rcr_wait_for_srb_release_cnf_substate
(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to RRC command */
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_STATE_CHANGE_IND:
      if((cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) &&
         ( tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED) )
      {        
        /* Reset the RCR state machine */
        tdsrrcrcr_substate = TDSRRCRCR_INITIAL;
      }
      else
      {
        ERR_FATAL("RRCSCMGR posted an unregistered event:%d",cmd_ptr->cmd.state_change_ind.new_state ,0,0);
      }
      break; 

    case TDSRRC_CHANNEL_CONFIG_CNF:
      /* It doesn't matter whether RRC LLC successfully releases Radio
         resources or not */
      if(cmd_ptr->cmd.chan_config_cnf.procedure == TDSRRC_PROCEDURE_RCR)
      {
        /* The RCR procedure ends */
        tdsrrcrcr_substate = TDSRRCRCR_INITIAL; 
      }
      break;

    default:
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d in the substate: %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrcrcr_substate);
      break;
  }

} /* tdsrrcrcr_process_rcr_wait_for_srb_release_cnf_substate */


/*===========================================================================

FUNCTION tdsrrcrcr_is_proc_active

DESCRIPTION
  Returns rcr sub-state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

boolean tdsrrcrcr_is_proc_active
(
  void
)
{
  
  /* Resets the RCR state machine */
  if (tdsrrcrcr_substate == TDSRRCRCR_INITIAL)
  {
    return FALSE;
  }
  else
  {
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCRCR st:%d,rrc_st:%d",tdsrrcrcr_substate,tdsrrc_get_state());
    return TRUE;
  }

}
/*===========================================================================

FUNCTION tdsrrcrcr_is_proc_active_sub

DESCRIPTION
  Indicates whether RCR is active or not.

DEPENDENCIES
  None

RETURN VALUE
  TRUE: If UE is Multi-SIM, and if RCR is in progress on as_id given by NAS
        If UE is single SIM and RCR is in progress.

  FALSE:If UE is in Multi-SIM and as_id doesn't match the ID on which TDS is active
        Else If RCR is not in progress at TDSRRC


SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcrcr_is_proc_active_sub
(
  sys_modem_as_id_e_type as_id
)
{
  boolean status = FALSE;
  if((!tdsrrc_is_sim_id_valid) || (as_id != tdsrrc_sim_id_recvd))
  {
    status = FALSE;
  }
  /* Resets the RCR state machine */
  if (tdsrrcrcr_substate == TDSRRCRCR_INITIAL)
  {
    status = FALSE;
  }
  else
  {
     status = TRUE;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS RCR in progress %d",status);
  return status;
}


/*===========================================================================

FUNCTION tdsrrcrcr_process_690non_critical_ext

DESCRIPTION
  Processes 690 non critical extension of RRC Conn Rel OTA.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_process_690non_critical_ext
(
  tdsrrc_RRCConnectionRelease_v690NonCriticalExtensions_1 * rcr_msg_ptr
)
{
  if((rcr_msg_ptr->m.v770NonCriticalExtensionsPresent) &&
      (rcr_msg_ptr->v770NonCriticalExtensions.rrcConnectionRelease_v770ext.m.
      ueMobilityStateIndicatorPresent))
  {
    tdsrrc_high_mobility_ind_in_rcr = TRUE;
  }
  else
  {
    tdsrrc_high_mobility_ind_in_rcr = FALSE;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"tdsrrc_high_mobility_ind_in_rcr = %d",tdsrrc_high_mobility_ind_in_rcr);
}

/*===========================================================================
FUNCTION tdsrrcrcr_send_redirection_req

DESCRIPTION
  This function builds the TDSRRC_REDIRECT_REQ command with all parameters
  received through RRC CONNECTION REJECT message and sends the command to
  Cell Selection Procedure
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrcrcr_status_e_type : Returns TDSRRCRCR_SUCCESS if it successfully sends
  TDSRRC_REDIRECT_REQ command. Otherwise returns TDSRRCRCR_FAILURE_MAJOR

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_send_redirection_req()
{
 
  tdsrrc_cmd_type *cmd_ptr;                     /* Pointer to RRC Command */
  uint32 i;
  tdsrrc_state_e_type tdsrrc_curr_state = tdsrrc_get_state();
                                             /* The status is initialized to
                                                Failure */
  
  switch(tdsrrcrcr_saved_redirect_info.choice)
  {
    case TDSRRC_FREQ_REDIRECTION: 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq redirection due to connection release");
  
      /* tdsrrc_validate_dl_arfcn->tdsrrc_validate_tds_frequency, old function also just hudge DL freq */
      if(FAILURE == tdsrrc_validate_tds_frequency(
                        tdsrrcrcr_saved_redirect_info.u.freq.uarfcn_DL
                        )
        )
      {
#if 0   /*Do not record redirect freq in DB due to we will try it individually*/
        if(tdsrrccsp_update_acq_db(tdsrrcrcr_saved_redirect_info.u.freq.uarfcn_DL) == SUCCESS)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Redirect frequency info added to ACQ DB");
        }
#endif
        tdsrrcrcr_redirection_info_present = FALSE;
      }
      /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
      if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
      }
      break;
  
    case TDSRRC_INTER_SYSTEM_GSM:
#ifdef FEATURE_TDSCDMA_TO_LTE
    case TDSRRC_INTER_SYSTEM_LTE:
#endif

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Interrat redirection due to connection rej");
  
      if(tdsrrcmcm_is_dualmode_enabled() && tdsrrcrcr_saved_redirect_info.choice == TDSRRC_INTER_SYSTEM_GSM
                 && ((tdsrrc_curr_state == TDSRRC_STATE_CELL_DCH) || (tdsrrc_curr_state == TDSRRC_STATE_CELL_FACH))
        )
      {
        if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
        {
          tdsrrccsp_cleanup_bplmn_state();
        }

        /* Allocates the buffer to RRC Internal command */
        if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        {     
          /* Fill in the command parameters to TDSRRC_REDIRECT_REQ Command */
          cmd_ptr->cmd_hdr.cmd_id = TDSRRC_REDIRECT_REQ;
          cmd_ptr->cmd.redirect_req.proc_id = TDSRRC_PROCEDURE_RCR;
          cmd_ptr->cmd.redirect_req.wait_time = 0;
          cmd_ptr->cmd.redirect_req.cpid = 128; /* initialize the cpid to an invalid value. TDD:0~127 */
          cmd_ptr->cmd.redirect_req.choice = tdsrrcrcr_saved_redirect_info.choice;
  
          if(tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size != 0)
          {
            for(i = 0;i < tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size;i++)
            {
                   /* Get bcch_ARFCN */
                   cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.arfcns[i].num
                      = tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.arfcns[i].num;
        
                   cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.arfcns[i].band
                      = tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.arfcns[i].band;
         
                   cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.BSIC[i].BSIC_NCC 
                      = tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.BSIC[i].BSIC_NCC;
        
                   cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.BSIC[i].BSIC_BCC
                      = tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.BSIC[i].BSIC_BCC;
            }
            cmd_ptr->cmd.redirect_req.u.GSM_cell_info_list.size = tdsrrcrcr_saved_redirect_info.u.GSM_cell_info_list.size;
          }
          /* sends the command to Cell Selection Procedure */
          tdsrrc_put_int_cmd(cmd_ptr);         
        }
        else 
        { 
          ERR_FATAL("Unable to post Internal Cmd to CSP: %d",TDSRRC_REDIRECT_REQ,0,0);
        } 
      }
      else
#ifdef FEATURE_TDSCDMA_TO_LTE
      if(tdsrrcmcm_is_lte_mode_enabled() && tdsrrcrcr_saved_redirect_info.choice == TDSRRC_INTER_SYSTEM_LTE
           && ((tdsrrc_curr_state == TDSRRC_STATE_CELL_DCH) || (tdsrrc_curr_state == TDSRRC_STATE_CELL_FACH))
       )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Send LTE redirection info");

        if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
        {
          tdsrrccsp_cleanup_bplmn_state();
        }

        /* Allocates the buffer to RRC Internal command */
        if( (cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        {
          /* Fill in the command parameters to RRC_REDIRECT_REQ Command */
          cmd_ptr->cmd_hdr.cmd_id = TDSRRC_REDIRECT_REQ;
          cmd_ptr->cmd.redirect_req.proc_id = TDSRRC_PROCEDURE_RCR;
          cmd_ptr->cmd.redirect_req.wait_time = 0;
          cmd_ptr->cmd.redirect_req.cpid = 128; /* initialize the cpid to an invalid value. TDD:0~127 */

          cmd_ptr->cmd.redirect_req.choice = tdsrrcrcr_saved_redirect_info.choice;

          cmd_ptr->cmd.redirect_req.u.EUTRA_target_cell_info = tdsrrcrcr_saved_redirect_info.u.EUTRA_target_cell_info;

          /* sends the command to Cell Selection Procedure */
          tdsrrc_put_int_cmd(cmd_ptr);         
        }
        else 
        { 
          ERR_FATAL("Unable to post Internal Cmd to CSP: %d",TDSRRC_REDIRECT_REQ,0,0);
        } 
      }
      else
#endif
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"iRAT Redirection rcvd when Interrat Mode is not enabled.or feature disabled Trans to disconn");
  #ifdef FEATURE_DISCONNECT_ON_UNSUPPORTED_IRAT_REDIRECTION
        if ((tdsrrc_establishment_cause == RRC_EST_EMERGENCY_CALL) &&
             tdsrrccsp_check_gsm_bands_supported())                               
        {
          if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR,
                   TDSRRC_TX_TO_DISCON_UNSUPPORTED_REDIRECTION))
          {                            
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
          }    
        }
        else
  #endif
        {
          /* Take UE to Idle via CSP by sending init cell sel with tx to discon */
          if(SUCCESS == tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_RCR, TDSRRC_TX_TO_DISCON_OTHER))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending init cell sel ind to CSP for tx to discon");
          }
        }
      } 

      break;
  
    default:     
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"This should never happen");
      break;
  }      
  
  tdsrrcrcr_substate = TDSRRCRCR_WAIT_FOR_SRB_RELEASE_CNF;
   
} /* tdsrrcrcr_send_redirection_req */   
#ifdef FEATURE_TDSCDMA_RCR_CANCEL_RECFG
/*===========================================================================
FUNCTION tdsrrcrcr_cancel_reconfig_proc

DESCRIPTION
  This function is used to check whether can cancel the reconfiguration procedure
  and try to cancel the reconfiguration procedure.
  
DEPENDENCIES
  None

RETURN VALUE
 void

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_cancel_reconfig_proc(void)
{
  tdsrrcllc_oc_set_status_e_type     oc_status;
  tdsrrc_proc_e_type proc_id;
  tdsrrcllc_oc_process_state_e_type  process_state; 
  tdsrrc_cmd_type *llc_cmd_ptr = NULL;
  oc_status = tdsrrcllc_get_ordered_config_state_and_proc(&proc_id, &process_state);
  if (oc_status != TDSOC_NOT_SET)
  {
    
    if(!(((tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)||(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)) && ((oc_status == TDSOC_SET_FOR_CELL_DCH))) ||
            !(tdsrrccu_check_proc_waiting_for_chan_config_cnf(proc_id) ||(oc_status == TDSOC_SET_FOR_DCH_FACH_TRANS)) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't Cancel pending L1 config ");
    }
    else if (TDSLLC_WAIT_CPHY_SETUP_CNF == tdsrrcllc_return_current_substate()
         /* revert procedure , activation time is now, so RRC can't cancel. */
        && (!tdsrrcllc_check_going_to_old_config())
        && (tdsrrcllc_check_sufficient_act_time())
        && (proc_id == TDSRRC_PROCEDURE_RBE ||
            proc_id == TDSRRC_PROCEDURE_RBRC ||
            proc_id == TDSRRC_PROCEDURE_RBR ||
            proc_id == TDSRRC_PROCEDURE_TCR ||
            proc_id == TDSRRC_PROCEDURE_PCR))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cancel pending L1 config ");
      if((llc_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
      {
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
        llc_cmd_ptr->cmd.rrc_llc_req.l1_req = 
                   (tdsl1_req_cmd *)tdsrrc_malloc(sizeof(tdsl1_req_cmd)); 
        if(llc_cmd_ptr->cmd.rrc_llc_req.l1_req  == NULL) 
        {
            ERR_FATAL("tdsl1_req_cmd not received",0,0,0); 
        }
#endif
      
        /* Initialize the LLC command header first */
        llc_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
        llc_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_RCR;
        llc_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
        llc_cmd_ptr->cmd.rrc_llc_req.l1_cmd.act_time_type = TDSL1_ACTIVATION_TIME_NONE;
        llc_cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_ACT_TIME_CANCEL_REQ;
        tdsrrc_put_int_cmd(llc_cmd_ptr); 
      }
      else
      {
        ERR_FATAL("RRC Unable to get cmd buf for RLC cmd",0,0,0);
      }
    }
  }
}
/*===========================================================================
FUNCTION tdsrrcrcr_is_wait_t308_state

DESCRIPTION
  This function is used to check RCR is at wait T308 state.
  
DEPENDENCIES
  None

RETURN VALUE
 TRUE: RCR is waiting T308 expire.
 FALSE: RCR is not waiting T308 expier.

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrcrcr_is_wait_t308_state(void)
{
  if (TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE == tdsrrcrcr_substate)
  {
    return TRUE;
  }
  return FALSE;
}
#endif

/*===========================================================================

FUNCTION tdsrrcrcr_is_interrat_redirection

DESCRIPTION 
  This function is used to check whether rcr msg indicating interrat redirection

DEPENDENCIES
    None.

RETURN VALUE
   TRUE:  rcr msg indicating interrat redirection
   FALSE: rcr msg not indicating interrat redirection

===========================================================================*/
boolean tdsrrcrcr_is_interrat_redirection(void)
{

  if(tdsrrcrcr_redirection_info_present
      && (tdsrrcrcr_saved_redirect_info.choice == TDSRRC_INTER_SYSTEM_GSM || tdsrrcrcr_saved_redirect_info.choice == TDSRRC_INTER_SYSTEM_LTE))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================
FUNCTION tdsrrcrcr_end_rcr_procedure

DESCRIPTION
  This function is used to end RCR procedure immediately.
  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcrcr_end_rcr_procedure(void)
{
  if (tdsrrcrcr_substate == TDSRRCRCR_WAIT_FOR_T308_TIMER_EXPIRE)
  {
    /*let TDSRRCTMR_T_308_TIMER expire immediately, and no need to send "RRC connection release complete" again. */
    tdsrrc_v_308 = tdsrrcrcr_n_308 + 1;
    if(tdsrrctmr_get_remaining_time(TDSRRCTMR_T_308_TIMER) != 0)
    {
      tdsrrctmr_stop_timer(TDSRRCTMR_T_308_TIMER);
      tdsrrctmr_start_timer(TDSRRCTMR_T_308_TIMER, 0);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"force T308 timer to expire immediately");
    }
  }
  else if(tdsrrcrcr_substate == TDSRRCRCR_WAIT_FOR_L2ACK_FOR_RRC_CONN_REL_COMPLETE)
  {
    /*let TDSRRCTMR_RCR_L2ACK_TIMER expire immediately, no need to wait for L2 ACK of "RRC connection release complete". */    
    if(tdsrrctmr_get_remaining_time(TDSRRCTMR_RCR_L2ACK_TIMER) != 0)
    {
      tdsrrctmr_stop_timer(TDSRRCTMR_RCR_L2ACK_TIMER);
      tdsrrctmr_start_timer(TDSRRCTMR_RCR_L2ACK_TIMER, 0);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"force RCR L2ACK timer to expire immediately");
    }
  }  
}
