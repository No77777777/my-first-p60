/*===========================================================================

  Copyright (c) 2018 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

#include <msgr.h>
#include "mm_v.h"
#include "emm_ssgccs.h"
#include "emm_ssgccs_api.h"
#include "emm_database.h"
#include "stringl.h"

#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
ssgccs_4g_emm_msg_handler emm_msg_handler = NULL;
#endif
/*========================================================================
  FUNCTION register_ssgccs_4g_emm_msg_handler

  DESCRIPTION
    Register callback handler for EMM info

  PARAMETERS
    msg_handler [in] - callback handler

  RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_4g_emm_msg_handler
(
  ssgccs_4g_emm_msg_handler msg_handler
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  emm_msg_handler = msg_handler;
#endif
} /* register_ssgccs_4g_emm_msg_handler() */


/*========================================================================
  FUNCTION emm_update_ssgccs_csfb_call_status

  DESCRIPTION
    update the csfb call status

  PARAMETERS
    call_status [in] - uint8

  RETURN VALUE
    None
===========================================================================*/
void emm_update_ssgccs_csfb_call_status
(
  uint8 call_status
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
    ssgccs_4g_emm_info_type emm_info;

    if (emm_msg_handler == NULL)
    {

        return;
    }

    memset(&emm_info, 0, sizeof(ssgccs_4g_emm_info_type));


    emm_info.call_status_info_present = TRUE;
    emm_info.csfb_call_status = call_status;
	emm_info.as_id = (sys_modem_as_id_e_type)mm_as_id;
    emm_msg_handler(&emm_info);
#endif
}


/*========================================================================
  FUNCTION emm_ssgccs_handle_incoming_msg

  DESCRIPTION
    Process EMM incoming message

  PARAMETERS
    incoming_msg [in] - EMM incoming message

  RETURN VALUE
    None
===========================================================================*/
void emm_ssgccs_handle_incoming_msg
(
  lte_nas_emm_incoming_msg_type *incoming_msg
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  ssgccs_4g_emm_info_type emm_info;
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  sys_plmn_id_s_type        selected_plmn;
  boolean  plmn_id_is_undefined;
  boolean  mnc_includes_pcs_digit;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);
  ASSERT(emm_ctrl_data_ptr->last_rrc_service_ind_ptr != NULL);

  selected_plmn = emm_convert_rrc_plmn_id_to_nas_plmn_id(
                               emm_ctrl_data_ptr->last_rrc_service_ind_ptr->camped_svc_info.selected_plmn);


  if (incoming_msg == NULL)
  {
    return;
  }
  if (emm_msg_handler == NULL)
  {
    return;
  }
  memset(&emm_info, 0, sizeof(ssgccs_4g_emm_info_type));
  emm_info.msg_id = incoming_msg->hdr.msg_id;
  emm_info.global_cell_id =  emm_ctrl_data_ptr->last_rrc_service_ind_ptr->camped_svc_info.cell_identity;
  emm_info.as_id=(sys_modem_as_id_e_type)mm_as_id;
  sys_plmn_get_mcc_mnc(selected_plmn,
                         &plmn_id_is_undefined,
                         &mnc_includes_pcs_digit,
                         &emm_info.mcc,
                         &emm_info.mnc);

  switch (incoming_msg->hdr.msg_id) {
    case ATTACH_REJECT:
      emm_info.attach_reject_cause = incoming_msg->attach_rej.emm_cause;
      emm_info.sec_header= incoming_msg->attach_rej.security_hdr;
      
      break;      

    case TRACKING_AREA_REJECT:
      emm_info.tau_reject_cause = incoming_msg->tau_rej.emm_cause;
      emm_info.sec_header = incoming_msg->tau_rej.security_hdr;
      
      break;
    case SERVICE_REJECT:
      emm_info.service_reject = incoming_msg->service_rej.emm_cause;
      emm_info.sec_header= incoming_msg->service_rej.security_hdr;
      
      break;
	case SECURITY_MODE_COMMAND:
	  emm_info.cipher_algo = incoming_msg->security_mode_cmd.nas_security_alg.ciphering_alg;
      emm_info.sec_header = incoming_msg->security_mode_cmd.security_hdr;
	  
      break;
	case EMM_IDENTITY_REQUEST:
      emm_info.mobile_id = incoming_msg->identity_req.mobile_identity;
      emm_info.sec_header= incoming_msg->identity_req.security_hdr;
	  
      break;
    case ATTACH_ACCEPT:
      (void)memscpy((void*)&(emm_info.tai_lst),
             sizeof(lte_nas_emm_tai_lst_info_type),
             (void*)&(incoming_msg->attach_accept.tai_lst),
             sizeof(lte_nas_emm_tai_lst_info_type));
       emm_info.sec_header = incoming_msg->attach_accept.security_hdr;
       
      break;
    case TRACKING_AREA_UPDATE_ACCEPT:
      (void)memscpy((void*)&(emm_info.tai_lst),
             sizeof(lte_nas_emm_tai_lst_info_type),
             (void*)&(incoming_msg->tau_accept.tai_lst),
             sizeof(lte_nas_emm_tai_lst_info_type));
       emm_info.sec_header= incoming_msg->tau_accept.security_hdr;
       
      break;
    default:
      
      break;
  }
  emm_msg_handler(&emm_info);
#endif
} /* emm_ssgccs_handle_incoming_msg() */

/*========================================================================
  FUNCTION emm_ssgccs_handle_outgoing_msg

  DESCRIPTION
    Process EMM outgoing message

  PARAMETERS
    outgoing_msg [in] - EMM outgoing message

  RETURN VALUE
    None
===========================================================================*/
void emm_ssgccs_handle_outgoing_msg
(
  lte_nas_emm_outgoing_msg_type* outgoing_msg
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  ssgccs_4g_emm_info_type emm_info;
  sys_plmn_id_s_type        selected_plmn;
  boolean  plmn_id_is_undefined;
  boolean  mnc_includes_pcs_digit;
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);
  ASSERT(emm_ctrl_data_ptr->last_rrc_service_ind_ptr != NULL);
  if (outgoing_msg == NULL)
  {
    return;
  }
  if (emm_msg_handler == NULL)
  {

    return;
  }
  memset(&emm_info, 0, sizeof(ssgccs_4g_emm_info_type));
  emm_info.msg_id = outgoing_msg->hdr.msg_id;
  emm_info.global_cell_id =  emm_ctrl_data_ptr->last_rrc_service_ind_ptr->camped_svc_info.cell_identity;
  emm_info.as_id=(sys_modem_as_id_e_type)mm_as_id;
 selected_plmn = emm_convert_rrc_plmn_id_to_nas_plmn_id(
                               emm_ctrl_data_ptr->last_rrc_service_ind_ptr->camped_svc_info.selected_plmn);

  sys_plmn_get_mcc_mnc(selected_plmn,
                         &plmn_id_is_undefined,
                         &mnc_includes_pcs_digit,
                         &emm_info.mcc,
                         &emm_info.mnc);

  if (outgoing_msg->hdr.msg_id == ATTACH_REQUEST)
  {
    emm_info.ue_id = outgoing_msg->attach_req.eps_mobile_id.ue_id;
    
  }
  else if (outgoing_msg->hdr.msg_id == TRACKING_AREA_UPADTE_REQUEST)
  {
    emm_info.ue_id = outgoing_msg->tau_req.eps_mobile_id.ue_id;
    
  }
  else if (outgoing_msg->hdr.msg_id == EMM_AUTHENTICATION_FAILURE)
  {
    emm_info.authentication_failure_cause = outgoing_msg->auth_fail.emm_cause;
    
  }
  
  emm_msg_handler(&emm_info);
#endif
} /* emm_ssgccs_handle_outgoing_msg() */


/*========================================================================
  FUNCTION emm_ssgccs_send_tau_failed_ind

  DESCRIPTION
    Process EMM outgoing message

  PARAMETERS
    outgoing_msg [in] - EMM outgoing message

  RETURN VALUE
    None
===========================================================================*/
void emm_ssgccs_send_tau_failed_ind(lte_emm_connection_cause_type                cause)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  emm_tau_failed_ind_type  tau_failed_ind;

  emm_ctrl_data_type  *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);

  memset(&tau_failed_ind, 0, sizeof(emm_tau_failed_ind_type));
   



  msgr_init_hdr((msgr_hdr_s *)&tau_failed_ind.msg_hdr,
                MSGR_NAS_EMM,
                NAS_EMM_TAU_FAILED_IND);

  tau_failed_ind.cause = cause;

  if(emm_msgr_send((msgr_hdr_s *)&tau_failed_ind,
               sizeof(emm_tau_failed_ind_type)) != E_SUCCESS)
  {
    MSG_ERROR_DS(MM_SUB, "=EMM= Failed to send NAS_EMM_TAU_FAILED_IND", 0,0,0);
  }
#endif
}

/*===========================================================================

FUNCTION  emm_ssgccs_send_attach_failed_ind

DESCRIPTION
  This function handles the case of connection is released before ATTACH complete

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void emm_ssgccs_send_attach_failed_ind(lte_emm_connection_cause_type                cause)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  emm_attach_failed_ind_type  attach_failed_ind;

  emm_ctrl_data_type  *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);
  memset(&attach_failed_ind, 0, sizeof(emm_attach_failed_ind_type));
  
   



  msgr_init_hdr((msgr_hdr_s *)&attach_failed_ind.msg_hdr,
                MSGR_NAS_EMM,
                NAS_EMM_ATTACH_FAILED_IND);

  attach_failed_ind.cause = cause;

  if(emm_msgr_send((msgr_hdr_s *)&attach_failed_ind,
               sizeof(emm_attach_failed_ind_type)) != E_SUCCESS)
  {
    MSG_ERROR_DS(MM_SUB, "=EMM= Failed to send NAS_EMM_ATTACH_FAILED_IND",0,0,0);
  }
#endif
}
/*===========================================================================

FUNCTION  emm_send_ssg_sec_ctxt_set_ind

DESCRIPTION
  This function handles the case of connection is released before ATTACH complete

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void emm_send_ssg_sec_ctxt_set_ind(emm_nas_ssg_context_type   ctxt_est)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  emm_sec_ctxt_set_ind_type  sec_ctxt_set_ind;

  emm_ctrl_data_type  *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);
  memset(&sec_ctxt_set_ind, 0, sizeof(emm_sec_ctxt_set_ind_type));
  
   



  msgr_init_hdr((msgr_hdr_s *)&sec_ctxt_set_ind.msg_hdr,
                MSGR_NAS_EMM,
                NAS_EMM_SEC_CTX_SET_IND);

  sec_ctxt_set_ind.ctxt_est = ctxt_est;

  if(emm_msgr_send((msgr_hdr_s *)&sec_ctxt_set_ind,
               sizeof(emm_sec_ctxt_set_ind_type)) != E_SUCCESS)
  {
    MSG_ERROR_DS(MM_SUB, "=EMM= Failed to send NAS_EMM_SEC_CTX_SET_IND",0,0,0);
  }
#endif
}
/*===========================================================================

FUNCTION  emm_ssgccs_send_sec_ctxt_del_ind

DESCRIPTION
  This function handles the case of connection is released before ATTACH complete

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void emm_ssgccs_send_sec_ctxt_del_ind(emm_nas_ssg_context_type        ctxt_est)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  emm_sec_ctxt_del_ind_type  sec_ctxt_del_ind;

  emm_ctrl_data_type  *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);

  memset(&sec_ctxt_del_ind, 0, sizeof(emm_sec_ctxt_del_ind_type));
   



  msgr_init_hdr((msgr_hdr_s *)&sec_ctxt_del_ind.msg_hdr,
                MSGR_NAS_EMM,
                NAS_EMM_SEC_CTX_DEL_IND);

  sec_ctxt_del_ind.ctxt_est = ctxt_est;

  if(emm_msgr_send((msgr_hdr_s *)&sec_ctxt_del_ind,
               sizeof(emm_sec_ctxt_del_ind_type)) != E_SUCCESS)
  {
    MSG_ERROR_DS(MM_SUB, "=EMM= Failed to send NAS_EMM_SEC_CTX_DEL_IND",0,0,0);
  }
#endif
}

/*===========================================================================

FUNCTION  emm_ssgccs_cell_change_ind

DESCRIPTION
  This function sends cell change indication

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void emm_ssgccs_cell_change_ind
(
  lte_rrc_global_cell_id_t cell_identity,
  sys_plmn_id_s_type     plmn
)
{
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
  emm_ssgccs_cell_change_ind_type  ssgccs_cell_change_ind;
  boolean  plmn_id_is_undefined;
  boolean  mnc_includes_pcs_digit;
  
  emm_ctrl_data_type  *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);
  
  memset(&ssgccs_cell_change_ind, 0, sizeof(emm_ssgccs_cell_change_ind_type));
  
  sys_plmn_get_mcc_mnc(plmn,
                       &plmn_id_is_undefined,
                       &mnc_includes_pcs_digit,
                       &ssgccs_cell_change_ind.mcc,
                       &ssgccs_cell_change_ind.mnc);


  msgr_init_hdr((msgr_hdr_s *)&ssgccs_cell_change_ind.msg_hdr,
                MSGR_NAS_EMM, 
                NAS_EMM_CELL_CHANGE_IND); 

  ssgccs_cell_change_ind.cell_identity = cell_identity;
  ssgccs_cell_change_ind.as_id = mm_as_id;

  if(emm_msgr_send((msgr_hdr_s *)&ssgccs_cell_change_ind,
               sizeof(emm_ssgccs_cell_change_ind_type)) != E_SUCCESS)
  {
    MSG_ERROR_DS(MM_SUB, "=EMM= Failed to send NAS_EMM_CELL_CHANGE_IND",0,0,0);
  } 
#endif
}

