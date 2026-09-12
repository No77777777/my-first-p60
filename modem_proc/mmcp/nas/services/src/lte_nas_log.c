
/*===========================================================================
 
                    Callflow Analysis Logging Source File
 
DESCRIPTION
   Required for NAS logging.
 
Copyright (c) 2000, 2001 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "mmcp_variation.h"
#include<customer.h>
#ifdef FEATURE_LTE
#include "comdef.h"
#include "amssassert.h"
#include "naslog_v.h"
#include "lte_log_codes.h"
#include "lte_nas_log.h"
#include "err.h"
#include "event.h"
#include "event_defs.h"
#include <stringl/stringl.h>
#include "ULogFront.h"
#include "lte_rrc_ext_msg.h"
#include "mm_v.h"
#include "sm_v.h"
#include "emm_utility.h"
/*===========================================================================

                           DATA DECLARATIONS

===========================================================================*/
#define LTE_NAS_DIAG_LOG_VERSION 1
#define LTE_NAS_DIAG_RRC_SRV_REQ_VERSION 32
#define LTE_NAS_DIAG_UPDATED_LOG_VERSION 2
#define LOG_PTR_SIZE sizeof(log_hdr_type)

/*===========================================================================



                      EDIT HISTORY FOR FILE

$Header: //components/rel/mmcp.mpss/7.9.0/nas/services/src/lte_nas_log.c#1 $
$Author: pwbldsvc $
$DateTime: 2021/06/15 22:40:27 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/05/09   hnam    Added support for forbidden tracking are list & GUTI in EMM STATE log packet
24/06/09   hnam    Changed the function definition param name for 
                    send_emm_usimcard_mode_log_info
06/19/09   hnam     Initial Revision
===========================================================================*/

extern ULogHandle mm_get_ulog_ota_handle(void);


/*---------------------------------------------------------------------
                    ESM LOGGING FUNCTIONS
---------------------------------------------------------------------*/

void send_esm_bearer_ctxt_state_log_info
(
  lte_nas_esm_bearer_context_state_T  *esm_bearer_ctxt_state_log_info
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  byte pkt_size;

  ASSERT(esm_bearer_ctxt_state_log_info != NULL);

  /*Add the DIAG log version*/
  esm_bearer_ctxt_state_log_info->log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_esm_bearer_context_state_T);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_ESM_BEARER_CONTEXT_STATE_LOG_C, pkt_size, sm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0E4 not logged\n");
    return;
  }
  
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,sizeof(lte_nas_esm_bearer_context_state_T),esm_bearer_ctxt_state_log_info, sizeof(lte_nas_esm_bearer_context_state_T));
  log_commit((void*)diag_log_ptr);
  
}

void send_esm_bearer_ctxt_log_info
(
  lte_nas_esm_bearer_context_info_T  *esm_bearer_ctxt_log_info
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size;

  ASSERT(esm_bearer_ctxt_log_info != NULL);

  /*Add the DIAG log version*/
  esm_bearer_ctxt_log_info->log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_esm_bearer_context_info_T);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_ESM_BEARER_CONTEXT_INFO_LOG_C, pkt_size, sm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0E5 not logged\n");
    return;
  }
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,sizeof(lte_nas_esm_bearer_context_info_T),esm_bearer_ctxt_log_info, sizeof(lte_nas_esm_bearer_context_info_T));
  log_commit((void*)diag_log_ptr);
}

void send_esm_proc_state_log_info
(
  lte_nas_esm_procedure_state_T  *esm_proc_state_log_info
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size;

  ASSERT(esm_proc_state_log_info != NULL);

  /*Add the DIAG log version*/
  esm_proc_state_log_info->log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_esm_procedure_state_T);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_ESM_PROCEDURE_STATE_LOG_C, pkt_size, sm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0E6 not logged\n");
    return;
  }
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,sizeof(lte_nas_esm_procedure_state_T),esm_proc_state_log_info, sizeof(lte_nas_esm_procedure_state_T));
  log_commit((void*)diag_log_ptr);
}


/*---------------------------------------------------------------------
                    EMM LOGGING FUNCTIONS
---------------------------------------------------------------------*/

void send_emm_state_log_info
(
  byte                emm_state,
  byte                emm_substate, /*Contradicts -> emm_substate is of type 'word" in emm_database.h*/
  sys_plmn_id_s_type     *plmn,
  boolean                guti_valid,
  lte_nas_emm_guti_type  *guti  
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size;
  lte_nas_emm_state_type emm_state_log;
  //Initializing guti to FF (Invalid guti)
  //memcpy(&emm_state_log.guti,0xFF,sizeof(lte_nas_emm_guti_log_type));
  //byte guti_id = 6;/*6 is the enum value of GUTI ID*/

  /*Initialize plmn*/
  memset(&emm_state_log.plmn,0xFF, sizeof(sys_plmn_id_s_type));

  /*Add the DIAG log version*/
  emm_state_log.log_version = LTE_NAS_DIAG_UPDATED_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_emm_state_type);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_EMM_STATE_LOG_C, pkt_size,mm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0EE not logged\n");
    return;
  }
  //emm_state_log.log_version = emm_val;
  emm_state_log.emm_state = emm_state;
  emm_state_log.emm_substate = emm_substate;
  memscpy(&emm_state_log.plmn.identity[0],sizeof(sys_plmn_id_s_type),plmn,3);
  if(guti_valid)
  {
    //memcpy(&emm_state_log.guti,guti,sizeof(lte_nas_emm_guti_log_type));
    /*************************************************************************
     UE_ID is an enum type, so it takes 4 bytes (i.e. three extra bytes are 
     padded into the *guti structure, so memcpy()'ing this structure to the 
     logging function will contain 3 extra bytes in the middle and GUTI 
     parameters following UE_ID will be unrecognized
    **************************************************************************/
    emm_state_log.guti.ue_id            = (byte)guti->ue_id;
    emm_state_log.guti.plmn.identity[0] = guti->plmn.identity[0];
    emm_state_log.guti.plmn.identity[1] = guti->plmn.identity[1];
    emm_state_log.guti.plmn.identity[2] = guti->plmn.identity[2];
    emm_state_log.guti.mme_group_id[0]  = guti->mme_group_id[0];
    emm_state_log.guti.mme_group_id[1]  = guti->mme_group_id[1];
    emm_state_log.guti.mme_code         = guti->mme_code;
    emm_state_log.guti.m_tmsi[0]        = guti->m_tmsi[0];
    emm_state_log.guti.m_tmsi[1]        = guti->m_tmsi[1];
    emm_state_log.guti.m_tmsi[2]        = guti->m_tmsi[2];
    emm_state_log.guti.m_tmsi[3]        = guti->m_tmsi[3];
    emm_state_log.guti_valid            = (byte)guti_valid;
  }
  //if guti_valid == FALSE, fill emm_state_log with invalid info
  else
  {
    emm_state_log.guti.ue_id            = (byte)0xFF;
    emm_state_log.guti.plmn.identity[0] = 0xFF;
    emm_state_log.guti.plmn.identity[1] = 0xFF;
    emm_state_log.guti.plmn.identity[2] = 0xFF;
    emm_state_log.guti.mme_group_id[0]  = 0xFF;
    emm_state_log.guti.mme_group_id[1]  = 0xFF;
    emm_state_log.guti.mme_code         = 0xFF;
    emm_state_log.guti.m_tmsi[0]        = 0xFF;
    emm_state_log.guti.m_tmsi[1]        = 0xFF;
    emm_state_log.guti.m_tmsi[2]        = 0xFF;
    emm_state_log.guti.m_tmsi[3]        = 0xFF;
    emm_state_log.guti_valid            = (byte)guti_valid;
  }
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  if(temp_ptr != NULL)
  {
    memscpy(temp_ptr,sizeof(lte_nas_emm_state_type),&emm_state_log, sizeof(lte_nas_emm_state_type));
  }
  else
  {
    MSG_ERROR_0("NAS: LOG_LTE_NAS_EMM_STATE_LOG_C log packet could not be logged");
  }
  log_commit((void*)diag_log_ptr);
}

void send_emm_usimcard_mode_log_info
(
  lte_nas_emm_usim_card_mode_type  *emm_usim_card_mode_info
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size;

  ASSERT(emm_usim_card_mode_info != NULL);

  /*Add the DIAG log version*/
  emm_usim_card_mode_info->log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_emm_usim_card_mode_type);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_EMM_USIM_CARD_MODE_LOG_C, pkt_size, mm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0EF not logged\n");
    return;
  }
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,sizeof(lte_nas_emm_usim_card_mode_type),emm_usim_card_mode_info, sizeof(lte_nas_emm_usim_card_mode_type));
  log_commit((void*)diag_log_ptr);
}


void send_emm_current_ctxt_sec_log_info
(
  lte_nas_emm_current_sec_context_type  *emm_current_sec_ctxt_log
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size;

  ASSERT(emm_current_sec_ctxt_log != NULL);

  /*Add the DIAG log version*/
  emm_current_sec_ctxt_log->log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_emm_current_sec_context_type);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_EMM_CURRENT_SECUIRY_CONTEXT_LOG_C, pkt_size, mm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0F2 not logged\n");
    return;
  }
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,sizeof(lte_nas_emm_current_sec_context_type),emm_current_sec_ctxt_log, sizeof(lte_nas_emm_current_sec_context_type));
  log_commit((void*)diag_log_ptr);
}

void send_emm_ps_domain_int_cipher_log_info
(
  lte_nas_emm_ps_domain_int_cipher_keys  *emm_ps_domain_int_cipher_log
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size;

  ASSERT(emm_ps_domain_int_cipher_log != NULL);

  /*Add the DIAG log version*/
  emm_ps_domain_int_cipher_log->log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_emm_ps_domain_int_cipher_keys);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_EMM_CKIK_FOR_PS_DOMAIN_LOG_C, pkt_size , mm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0F3 not logged\n");
    return;
  }
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,sizeof(lte_nas_emm_ps_domain_int_cipher_keys),emm_ps_domain_int_cipher_log, sizeof(lte_nas_emm_ps_domain_int_cipher_keys));
  log_commit((void*)diag_log_ptr);
}

void send_emm_native_sec_ctxt_log_info
(
  lte_nas_emm_native_sec_context_type  *emm_native_sec_ctxt_log
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size;

  ASSERT(emm_native_sec_ctxt_log != NULL);

  /*Add the DIAG log version*/
  emm_native_sec_ctxt_log->log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_emm_native_sec_context_type);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_EMM_NATIVE_SECURITY_CONTEXT_LOG_C, pkt_size, mm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0F4 not logged\n");
    return;
  }
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,sizeof(lte_nas_emm_native_sec_context_type),emm_native_sec_ctxt_log, sizeof(lte_nas_emm_native_sec_context_type));
  log_commit((void*)diag_log_ptr);
}

void send_emm_usim_serv_table_log_info
(
  lte_nas_emm_usim_serv_table_type  *emm_usim_serv_table_log
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size;

  ASSERT(emm_usim_serv_table_log != NULL);

  /*Add the DIAG log version*/
  emm_usim_serv_table_log->log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + sizeof(lte_nas_emm_usim_serv_table_type);
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_EMM_USIM_SERVICE_TABLE_LOG_C, pkt_size, mm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0F5 not logged\n");
    return;
  }
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,sizeof(lte_nas_emm_usim_serv_table_type),emm_usim_serv_table_log, sizeof(lte_nas_emm_usim_serv_table_type));
  log_commit((void*)diag_log_ptr);
}

void log_forbidden_tracking_area_info
(
  lte_nas_tai_lst2_type *forbidden_for_service_list,
  lte_nas_tai_lst2_type *rrc_forbidden_list
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size = 0;
  lte_nas_emm_forbidden_tailist_type forbidden_list_log;
  byte index = 0, struct_size = 0;

  ASSERT(forbidden_for_service_list != NULL);
  ASSERT(rrc_forbidden_list != NULL);

  /*
  1 -> Log header
  (rrc_forbidden_list->tai_lst_length * 5 + 1) =  RRC Forbidden TAI length * ((PLMN (3 bytes) + TAC (2 bytes)) 
                                                  + Length octet of RRC forbidden tai list (1 byte)
  (forbidden_for_service_list->tai_lst_length * 5 + 1) = Forbidden TAI length for service * ((PLMN (3 bytes) + TAC (2 bytes)) 
                                                  + Length octet of forbidden tai list for service (1 byte)                                                
  */
  struct_size = ((rrc_forbidden_list->tai_lst_length * 5) + 1) + ((forbidden_for_service_list->tai_lst_length * 5) + 1) + 1;

  memset(&forbidden_list_log,0,sizeof(lte_nas_emm_forbidden_tailist_type));
  /*Add the DIAG log version*/
  forbidden_list_log.log_version = LTE_NAS_DIAG_LOG_VERSION;

  pkt_size = LOG_PTR_SIZE + struct_size;
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_EMM_FORBIDDEN_TRACKING_AREA_LIST_LOG_C, pkt_size, mm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0F6 not logged\n");
    return;
  }
  /*===============================================================================
     lte_nas_tai_lst2_type structure is not a PACK structure, so extra bytes 
     are padded,hence memcpy() of this structure will lead to extra bytes...
     so the following method is used to copy all the data into the logging buffer
  ================================================================================*/
  /*******************************************************
         Log FORBIDDEN TRACKING AREA LIST FOR ROAMING
  *******************************************************/
  forbidden_list_log.forbidden_roaming_list.length = rrc_forbidden_list->tai_lst_length;
  for(index = 0; index < rrc_forbidden_list->tai_lst_length; index++)
  {
    memscpy(&forbidden_list_log.forbidden_roaming_list.diff_plmn[index].plmn,
           sizeof(sys_plmn_id_s_type),&rrc_forbidden_list->diff_plmn[index].plmn,
           sizeof(sys_plmn_id_s_type));
    forbidden_list_log.forbidden_roaming_list.diff_plmn[index].tac = rrc_forbidden_list->diff_plmn[index].tac;
                           //((rrc_forbidden_list->diff_plmn[index].tac >> 8) | 
                            //(rrc_forbidden_list->diff_plmn[index].tac << 8));
  }

  /*******************************************************
         Log FORBIDDEN TRACKING AREA LIST FOR RSERVICE
  *******************************************************/
  forbidden_list_log.forbidden_service_list.length = forbidden_for_service_list->tai_lst_length;
  for(index = 0; index < forbidden_for_service_list->tai_lst_length; index++)
  {
    memscpy(&forbidden_list_log.forbidden_service_list.diff_plmn[index].plmn,
           sizeof(sys_plmn_id_s_type),&forbidden_for_service_list->diff_plmn[index].plmn,
            sizeof(sys_plmn_id_s_type));
    forbidden_list_log.forbidden_service_list.diff_plmn[index].tac = forbidden_for_service_list->diff_plmn[index].tac;
                     //((forbidden_for_service_list->diff_plmn[index].tac << 8) |
                      //(forbidden_for_service_list->diff_plmn[index].tac >> 8));
  }
  /*Get a DIAG PTR*/
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  /*Copy Log version number*/
  memscpy(temp_ptr,struct_size,&forbidden_list_log.log_version, sizeof(byte));
  temp_ptr ++;
  /*Copy "rrc forbidden tracking area list" length octet*/
  memscpy(temp_ptr,struct_size-1,&forbidden_list_log.forbidden_roaming_list.length, sizeof(byte));
  temp_ptr ++;
  /*Copy "rrc forbidden tracking area list"*/
  memscpy(temp_ptr,struct_size-2,forbidden_list_log.forbidden_roaming_list.diff_plmn, forbidden_list_log.forbidden_roaming_list.length * 5);
  temp_ptr += forbidden_list_log.forbidden_roaming_list.length * 5;
  /*Copy "forbidden tracking area list for service" length octet*/
  memscpy(temp_ptr,struct_size- 2 - (forbidden_list_log.forbidden_roaming_list.length * 5),&forbidden_list_log.forbidden_service_list.length, sizeof(byte));
  temp_ptr ++;
  /*Copy "forbidden tracking area list for service"*/
  memscpy(temp_ptr,struct_size-3- (forbidden_list_log.forbidden_roaming_list.length * 5),forbidden_list_log.forbidden_service_list.diff_plmn, forbidden_list_log.forbidden_service_list.length * 5);
  temp_ptr += forbidden_list_log.forbidden_service_list.length * 5;

  log_commit((void*)diag_log_ptr);  
}

void log_lte_rrc_service_request_info
(
  lte_rrc_service_req_s *rrc_service_req_ptr,
  emm_ctrl_data_type    *emm_ctrl_data_ptr
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;
  word pkt_size = 0, struct_size = 0, tmp_size = 0;
  lte_nas_emm_rrc_sevice_request_type rrc_service_req_log;
  byte index = 0;
  sys_plmn_id_s_type  sys_plmn;

  ASSERT(rrc_service_req_ptr != NULL);
  ASSERT(emm_ctrl_data_ptr != NULL);

  memset((void *)&rrc_service_req_log, 0, sizeof(rrc_service_req_log));

  /*Add the DIAG log version*/
  rrc_service_req_log.log_version = LTE_NAS_DIAG_RRC_SRV_REQ_VERSION;
  struct_size += sizeof(rrc_service_req_log.log_version);
  rrc_service_req_log.as_id = mm_as_id;
  struct_size += sizeof(rrc_service_req_log.as_id);
  rrc_service_req_log.trans_id = rrc_service_req_ptr->trans_id;
  struct_size += sizeof(rrc_service_req_log.trans_id);
  rrc_service_req_log.network_select_mode = (byte)rrc_service_req_ptr->network_select_mode;
  struct_size += sizeof(rrc_service_req_log.network_select_mode);
  rrc_service_req_log.req_plmn_info_is_valid = rrc_service_req_ptr->req_plmn_info_is_valid;
  struct_size += sizeof(rrc_service_req_log.req_plmn_info_is_valid);
  rrc_service_req_log.rplmn_info_is_valid = rrc_service_req_ptr->rplmn_info_is_valid;
  struct_size += sizeof(rrc_service_req_log.rplmn_info_is_valid);
  rrc_service_req_log.hplmn_info_is_valid = rrc_service_req_ptr->hplmn_info_is_valid;
  struct_size += sizeof(rrc_service_req_log.hplmn_info_is_valid);
  rrc_service_req_log.scan_is_new = rrc_service_req_ptr->scan_is_new;
  struct_size += sizeof(rrc_service_req_log.scan_is_new);
  rrc_service_req_log.use_timer = rrc_service_req_ptr->use_timer;
  struct_size += sizeof(rrc_service_req_log.use_timer);
  rrc_service_req_log.lte_scan_time = rrc_service_req_ptr->lte_scan_time;
  struct_size += sizeof(rrc_service_req_log.lte_scan_time);
  rrc_service_req_log.req_type = rrc_service_req_ptr->req_type;
  struct_size += sizeof(rrc_service_req_log.req_type);
  rrc_service_req_log.csg_id = rrc_service_req_ptr->csg_id;
  struct_size += sizeof(rrc_service_req_log.csg_id);
  rrc_service_req_log.t_timeout = rrc_service_req_ptr->t_timeout;
  struct_size += sizeof(rrc_service_req_log.t_timeout);
  rrc_service_req_log.ehplmn_camping_allowed = rrc_service_req_ptr->ehplmn_camping_allowed;
  struct_size += sizeof(rrc_service_req_log.ehplmn_camping_allowed);
  rrc_service_req_log.scan_scope = (byte)rrc_service_req_ptr->scan_scope;
  struct_size += sizeof(rrc_service_req_log.scan_scope);
  rrc_service_req_log.emc_srv_pending = rrc_service_req_ptr->emc_srv_pending;
  struct_size += sizeof(rrc_service_req_log.emc_srv_pending);
  sys_plmn = emm_convert_rrc_plmn_id_to_nas_plmn_id(rrc_service_req_ptr->req_plmn);

  /*******************************************************
         Log Requested PLMN
  *******************************************************/
  if( rrc_service_req_ptr->req_plmn_info_is_valid == TRUE) 
  {
    memscpy(&rrc_service_req_log.req_plmn.identity[0],sizeof(sys_plmn_id_s_type),&sys_plmn,sizeof(sys_plmn_id_s_type));
    struct_size += sizeof(sys_plmn_id_s_type);
  }

  /*******************************************************
         Log RPLMN
  *******************************************************/
  if( rrc_service_req_ptr->rplmn_info_is_valid== TRUE) 
  {
    memscpy(&rrc_service_req_log.rplmn.identity[0],sizeof(sys_plmn_id_s_type),&emm_ctrl_data_ptr->rplmn_info.rplmn,sizeof(sys_plmn_id_s_type));
    struct_size += sizeof(sys_plmn_id_s_type);
  }

  /*******************************************************
         Log HPLMN
  *******************************************************/
  if( rrc_service_req_ptr->hplmn_info_is_valid== TRUE) 
  {
    memscpy(&rrc_service_req_log.hplmn.identity[0],sizeof(sys_plmn_id_s_type),&emm_ctrl_data_ptr->emm_home_plmn,sizeof(sys_plmn_id_s_type));
    struct_size += sizeof(sys_plmn_id_s_type);
  }

  /*******************************************************
         Log EHPLMN LIST 
  *******************************************************/
  rrc_service_req_log.ehplmn_list.length =(byte) rrc_service_req_ptr->ehplmn_list.num_plmns;
  struct_size ++;
  for( index = 0; index < rrc_service_req_log.ehplmn_list.length; index++ )
  { 
    sys_plmn = emm_convert_rrc_plmn_id_to_nas_plmn_id(rrc_service_req_ptr->ehplmn_list.plmn[index]);
    memscpy(&rrc_service_req_log.ehplmn_list.plmn[index],
            sizeof(sys_plmn_id_s_type),&sys_plmn,
            sizeof(sys_plmn_id_s_type));
    struct_size += sizeof(sys_plmn_id_s_type);
  }

   /*******************************************************
         Log FORBIDDEN TRACKING AREA LIST 
  *******************************************************/
  rrc_service_req_log.forbidden_ta_list.length = emm_ctrl_data_ptr->rrc_forbidden_list_ptr->tai_lst_length;
  struct_size ++;
  for( index = 0; index < emm_ctrl_data_ptr->rrc_forbidden_list_ptr->tai_lst_length; index++ )
  { 
    memscpy(&rrc_service_req_log.forbidden_ta_list.diff_plmn[index].plmn,
            sizeof(sys_plmn_id_s_type),&emm_ctrl_data_ptr->rrc_forbidden_list_ptr->diff_plmn[index].plmn,
            sizeof(sys_plmn_id_s_type));
    rrc_service_req_log.forbidden_ta_list.diff_plmn[index].tac = emm_ctrl_data_ptr->rrc_forbidden_list_ptr->diff_plmn[index].tac;
    struct_size += sizeof(sys_plmn_id_s_type)+ sizeof(word);
  }

  /*******************************************************
         Log FORBIDDEN MANUAL TRACKING AREA LIST 
  *******************************************************/
  rrc_service_req_log.forbidden_manual_ta_list.length = emm_ctrl_data_ptr->manual_ftai_list_ptr->tai_lst_length;
  struct_size ++;
  for( index = 0; index < emm_ctrl_data_ptr->manual_ftai_list_ptr->tai_lst_length; index++ )
  { 
    memscpy(&rrc_service_req_log.forbidden_manual_ta_list.diff_plmn[index].plmn,
            sizeof(sys_plmn_id_s_type),&emm_ctrl_data_ptr->manual_ftai_list_ptr->diff_plmn[index].plmn,
            sizeof(sys_plmn_id_s_type));
    rrc_service_req_log.forbidden_manual_ta_list.diff_plmn[index].tac = emm_ctrl_data_ptr->manual_ftai_list_ptr->diff_plmn[index].tac;
    struct_size += sizeof(sys_plmn_id_s_type)+ sizeof(word);
  }

  /*******************************************************
         Log EPLMN LIST 
  *******************************************************/
  rrc_service_req_log.eplmn_list.length = emm_ctrl_data_ptr->emm_equivalent_PLMN_list.length_of_plmn_lst_contents;
  struct_size ++;
  for( index = 0; index < emm_ctrl_data_ptr->emm_equivalent_PLMN_list.length_of_plmn_lst_contents; index++ )
  { 
    memscpy(&rrc_service_req_log.eplmn_list.plmn[index],
            sizeof(sys_plmn_id_s_type),&emm_ctrl_data_ptr->emm_equivalent_PLMN_list.plmn[index],
            sizeof(sys_plmn_id_s_type));
    struct_size += sizeof(sys_plmn_id_s_type);
  }

  /*******************************************************
         Log RAT PRIORITY LIST 
  *******************************************************/
  rrc_service_req_log.rat_pri_list.num_items = rrc_service_req_ptr->rat_pri_list.num_items;
  struct_size +=sizeof(rrc_service_req_log.rat_pri_list.num_items);
  rrc_service_req_log.rat_pri_list.next_acq_sys_index = rrc_service_req_ptr->rat_pri_list.next_acq_sys_index;
  struct_size += sizeof(rrc_service_req_log.rat_pri_list.next_acq_sys_index);
  rrc_service_req_log.rat_pri_list.scan_type.new_scan = rrc_service_req_ptr->rat_pri_list.scan_type.new_scan;
  struct_size += sizeof(rrc_service_req_log.rat_pri_list.scan_type.new_scan);
  rrc_service_req_log.rat_pri_list.scan_type.use_timer = rrc_service_req_ptr->rat_pri_list.scan_type.use_timer;
  struct_size += sizeof(rrc_service_req_log.rat_pri_list.scan_type.use_timer);
  for (index = 0; index < rrc_service_req_ptr->rat_pri_list.num_items; index++) 
  {
    rrc_service_req_log.rat_pri_list.priority_list_info[index].acq_sys_mode = (byte)rrc_service_req_ptr->rat_pri_list.priority_list_info[index].acq_sys_mode;
    struct_size ++;
    rrc_service_req_log.rat_pri_list.priority_list_info[index].acq_sys_time_interval = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].acq_sys_time_interval;
    struct_size += sizeof(uint32);
    rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_rat_acq_required = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].bst_rat_acq_required ;
    struct_size ++;
    if(rrc_service_req_log.rat_pri_list.priority_list_info[index].acq_sys_mode == 9) 
    {
      struct_size += sizeof(uint64)*4;
#if(LTE_BAND_NUM == 256)
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_1_64 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_1_64;
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_65_128 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_65_128;
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_129_192 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_129_192;
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_193_256 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_193_256;
#else
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_1_64 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].band_cap.lte_band_cap;
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_65_128 = 0;
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_129_192 = 0;
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap.bits_193_256 = 0;
#endif
      if(rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_rat_acq_required == TRUE ) 
      {
        struct_size += sizeof(uint64)*4;
#if(LTE_BAND_NUM == 256)
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_1_64 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_1_64;
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_65_128 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_65_128;
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_129_192 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_129_192;
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_193_256 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_193_256;
#else
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_1_64 = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].band_cap.lte_band_cap;
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_65_128 = 0;
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_129_192 = 0;
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap.bits_193_256 = 0;
#endif
      }
    }
    else
    {
      struct_size += sizeof(uint64);
      rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.chgwt_band_cap = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].band_cap.chgwt_band_cap;
      if(rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_rat_acq_required == TRUE ) 
      {
        struct_size += sizeof(uint64);
        rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.chgwt_band_cap = rrc_service_req_ptr->rat_pri_list.priority_list_info[index].bst_band_cap.chgwt_band_cap;
      }
    }
  }

  pkt_size = LOG_PTR_SIZE + struct_size;
  diag_log_ptr = nas_log_alloc(LOG_LTE_NAS_EMM_LTE_RRC_SERVICE_REQUEST_LOG_C, pkt_size, mm_message_as_id);
  if(diag_log_ptr == NULL)
  {
    MSG_ERROR_0("Diag returned a NULL log pointer, log packet B0CE not logged\n");
    return;
  }

  /*Get a DIAG PTR*/
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;

  /*Copy Log version number*/
  memscpy(temp_ptr,struct_size,&rrc_service_req_log.log_version, sizeof(rrc_service_req_log.log_version));
  temp_ptr += sizeof(rrc_service_req_log.log_version);
  tmp_size += sizeof(rrc_service_req_log.log_version);

  /*Copy as_id*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.as_id, sizeof(rrc_service_req_log.as_id));
  temp_ptr += sizeof(rrc_service_req_log.as_id);
  tmp_size += sizeof(rrc_service_req_log.as_id);

  /*Copy trans_id*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.trans_id, sizeof(rrc_service_req_log.trans_id));
  temp_ptr += sizeof(rrc_service_req_log.trans_id);
  tmp_size += sizeof(rrc_service_req_log.trans_id);


  /*Copy network_select_mode*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.network_select_mode, sizeof(rrc_service_req_log.network_select_mode));
  temp_ptr += sizeof(rrc_service_req_log.network_select_mode);
  tmp_size += sizeof(rrc_service_req_log.network_select_mode);

  /*Copy req_plmn*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.req_plmn_info_is_valid, sizeof(rrc_service_req_log.req_plmn_info_is_valid));
  temp_ptr += sizeof(rrc_service_req_log.req_plmn_info_is_valid);
  tmp_size += sizeof(rrc_service_req_log.req_plmn_info_is_valid);
  if(rrc_service_req_log.req_plmn_info_is_valid == TRUE) 
  {
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.req_plmn, sizeof(sys_plmn_id_s_type)*(rrc_service_req_ptr->req_plmn_info_is_valid));
    temp_ptr += sizeof(sys_plmn_id_s_type)*(rrc_service_req_ptr->req_plmn_info_is_valid);
    tmp_size += sizeof(sys_plmn_id_s_type)*(rrc_service_req_ptr->req_plmn_info_is_valid);
  }

  /*Copy rplmn*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rplmn_info_is_valid, sizeof(rrc_service_req_log.rplmn_info_is_valid));
  temp_ptr += sizeof(rrc_service_req_log.rplmn_info_is_valid);
  tmp_size += sizeof(rrc_service_req_log.rplmn_info_is_valid);
  if(rrc_service_req_log.rplmn_info_is_valid == TRUE) 
  {
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rplmn, sizeof(sys_plmn_id_s_type));
    temp_ptr += sizeof(sys_plmn_id_s_type) ;
    tmp_size += sizeof(sys_plmn_id_s_type) ;
  }

  /*Copy hplmn*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.hplmn_info_is_valid, sizeof(rrc_service_req_log.hplmn_info_is_valid));
  temp_ptr += sizeof(rrc_service_req_log.hplmn_info_is_valid);
  tmp_size += sizeof(rrc_service_req_log.hplmn_info_is_valid);
  if(rrc_service_req_log.hplmn_info_is_valid == TRUE) 
  {
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.hplmn, sizeof(sys_plmn_id_s_type));
    temp_ptr += sizeof(sys_plmn_id_s_type) ;
    tmp_size += sizeof(sys_plmn_id_s_type) ;
  }

  /*Copy ehplmn list length octet*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.ehplmn_list.length, sizeof(rrc_service_req_log.ehplmn_list.length));
  temp_ptr += sizeof(rrc_service_req_log.ehplmn_list.length);
  tmp_size += sizeof(rrc_service_req_log.ehplmn_list.length);
  /*Copy ehplmn list*/
  for( index = 0; index < rrc_service_req_log.ehplmn_list.length; index++ )
  { 
    /*Copy ehplmn list*/
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.ehplmn_list.plmn[index], sizeof(sys_plmn_id_s_type));
    temp_ptr += sizeof(sys_plmn_id_s_type);
    tmp_size += sizeof(sys_plmn_id_s_type);
  }

  /*Copy forbidden tracking area list length octet*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.forbidden_ta_list.length, sizeof(rrc_service_req_log.forbidden_ta_list.length));
  temp_ptr += sizeof(rrc_service_req_log.forbidden_ta_list.length);
  tmp_size += sizeof(rrc_service_req_log.forbidden_ta_list.length);
  /*Copy "forbidden tracking area list "*/
  if(rrc_service_req_log.forbidden_ta_list.length > 0) 
  {
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.forbidden_ta_list.diff_plmn, rrc_service_req_log.forbidden_ta_list.length * 5);
    temp_ptr += rrc_service_req_log.forbidden_ta_list.length * 5;
    tmp_size += rrc_service_req_log.forbidden_ta_list.length * 5 ;
  }

  /*Copy forbidden manual tracking area list length octet*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.forbidden_manual_ta_list.length, sizeof(rrc_service_req_log.forbidden_manual_ta_list.length));
  temp_ptr += sizeof(rrc_service_req_log.forbidden_manual_ta_list.length);
  tmp_size += sizeof(rrc_service_req_log.forbidden_manual_ta_list.length);
  /*Copy "forbidden manual tracking area list "*/
  if(rrc_service_req_log.forbidden_manual_ta_list.length > 0) 
  {
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.forbidden_manual_ta_list.diff_plmn, rrc_service_req_log.forbidden_manual_ta_list.length * 5);
    temp_ptr += rrc_service_req_log.forbidden_manual_ta_list.length * 5;
    tmp_size += rrc_service_req_log.forbidden_manual_ta_list.length * 5;
  }

  /*Copy eplmn list length octet*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.eplmn_list.length, sizeof(rrc_service_req_log.eplmn_list.length));
  temp_ptr += sizeof(rrc_service_req_log.eplmn_list.length);
  tmp_size += sizeof(rrc_service_req_log.eplmn_list.length);
  /*Copy eplmn list*/
  for( index = 0; index < rrc_service_req_log.eplmn_list.length; index++ )
  { 
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.eplmn_list.plmn[index], sizeof(sys_plmn_id_s_type));
    temp_ptr += sizeof(sys_plmn_id_s_type);
    tmp_size += sizeof(sys_plmn_id_s_type);
  }

  /*Copy scan_is_new */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.scan_is_new, sizeof(rrc_service_req_log.scan_is_new));
  temp_ptr += sizeof(rrc_service_req_log.scan_is_new) ;
  tmp_size += sizeof(rrc_service_req_log.scan_is_new);

  /*Copy use_timer */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.use_timer, sizeof(rrc_service_req_log.use_timer));
  temp_ptr += sizeof(rrc_service_req_log.use_timer);
  tmp_size += sizeof(rrc_service_req_log.use_timer);

  /*Copy lte_scan_time */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.lte_scan_time, sizeof(rrc_service_req_log.lte_scan_time));
  temp_ptr += sizeof(rrc_service_req_log.lte_scan_time);
  tmp_size += sizeof(rrc_service_req_log.lte_scan_time);

  /*Copy req_type */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.req_type, sizeof(rrc_service_req_log.req_type));
  temp_ptr += sizeof(rrc_service_req_log.req_type);
  tmp_size += sizeof(rrc_service_req_log.req_type);

  /*Copy csg_id */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.csg_id, sizeof(rrc_service_req_log.csg_id));
  temp_ptr += sizeof(rrc_service_req_log.csg_id);
  tmp_size += sizeof(rrc_service_req_log.csg_id);

  /*Copy t_timeout */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.t_timeout, sizeof(rrc_service_req_log.t_timeout));
  temp_ptr += sizeof(rrc_service_req_log.t_timeout);
  tmp_size += sizeof(rrc_service_req_log.t_timeout);

  /*Copy ehplmn_camping_allowed */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.ehplmn_camping_allowed, sizeof(rrc_service_req_log.ehplmn_camping_allowed));
  temp_ptr += sizeof(rrc_service_req_log.ehplmn_camping_allowed);
  tmp_size += sizeof(rrc_service_req_log.ehplmn_camping_allowed);

   /*Copy scan_scope */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.scan_scope, sizeof(rrc_service_req_log.scan_scope));
  temp_ptr += sizeof(rrc_service_req_log.scan_scope);
  tmp_size += sizeof(rrc_service_req_log.scan_scope);

  /*Copy emc_srv_pending */
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.emc_srv_pending, sizeof(rrc_service_req_log.emc_srv_pending));
  temp_ptr += sizeof(rrc_service_req_log.emc_srv_pending);
  tmp_size += sizeof(rrc_service_req_log.emc_srv_pending);

  /*Copy rat priority list*/
  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.num_items, sizeof(rrc_service_req_log.rat_pri_list.num_items));
  temp_ptr += sizeof(rrc_service_req_log.rat_pri_list.num_items);
  tmp_size += sizeof(rrc_service_req_log.rat_pri_list.num_items);

  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.next_acq_sys_index, sizeof(rrc_service_req_log.rat_pri_list.next_acq_sys_index));
  temp_ptr += sizeof(rrc_service_req_log.rat_pri_list.next_acq_sys_index);
  tmp_size += sizeof(rrc_service_req_log.rat_pri_list.next_acq_sys_index);

  for (index = 0; index < rrc_service_req_log.rat_pri_list.num_items; index++)
  {
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.priority_list_info[index].acq_sys_mode, sizeof(byte));
    temp_ptr ++;
    tmp_size ++;
    if (rrc_service_req_log.rat_pri_list.priority_list_info[index].acq_sys_mode == 9) 
    {
      memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.lte_band_cap, sizeof(uint64)*4);
      temp_ptr += sizeof(uint64)*4;
      tmp_size += sizeof(uint64)*4;
      memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_rat_acq_required, sizeof(boolean));
      temp_ptr ++;
      tmp_size ++;
      if (rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_rat_acq_required == TRUE)
      {
        memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_band_cap.lte_band_cap, sizeof(uint64)*4);
        temp_ptr += sizeof(uint64)*4;
        tmp_size += sizeof(uint64)*4;
      }
    }
    else
    {
      memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.chgwt_band_cap, sizeof(uint64));
      temp_ptr += sizeof(uint64);
      tmp_size += sizeof(uint64);
      memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_rat_acq_required, sizeof(boolean));
      temp_ptr ++;
      tmp_size ++;
      if (rrc_service_req_log.rat_pri_list.priority_list_info[index].bst_rat_acq_required == TRUE)
      {
        memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.priority_list_info[index].band_cap.chgwt_band_cap, sizeof(uint64));
        temp_ptr += sizeof(uint64);
        tmp_size += sizeof(uint64);
      }
    }
    memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.priority_list_info[index].acq_sys_time_interval, sizeof(uint32));
    temp_ptr += sizeof(uint32);
    tmp_size += sizeof(uint32);
  }

  memscpy(temp_ptr,struct_size - tmp_size,&rrc_service_req_log.rat_pri_list.scan_type, sizeof(lte_nas_eoos_gwl_scan_log_type));
  temp_ptr += sizeof(lte_nas_eoos_gwl_scan_log_type);
  tmp_size += sizeof(lte_nas_eoos_gwl_scan_log_type);

  log_commit((void*)diag_log_ptr);

}

void send_nas_ota_msg_log_packet
(
  word log_code,
  word nas_emm_msg_size,
  byte *nas_ota_msg_ptr
)
{
  byte   *diag_log_ptr;
  word   pkt_size;
  byte   *temp_ptr;
  byte   log_version;
  byte   std_version;
  byte   std_major_version;
  byte   std_minor_version;

  ASSERT(nas_ota_msg_ptr != NULL);

  if (mm_get_ulog_ota_handle() != NULL)
  {
    (void) ULogFront_RawLog(mm_get_ulog_ota_handle(), (const char*) nas_ota_msg_ptr, nas_emm_msg_size);
  }

  pkt_size = LOG_PTR_SIZE + nas_emm_msg_size + sizeof(log_version) +sizeof(std_version)
                          + sizeof(std_major_version) +sizeof(std_minor_version);

  switch(log_code)
  {
  case LOG_LTE_NAS_ESM_SEC_OTA_IN_MSG_LOG_C:    
  case LOG_LTE_NAS_ESM_SEC_OTA_OUT_MSG_LOG_C:
  case LOG_LTE_NAS_ESM_OTA_IN_MSG_LOG_C:
  case LOG_LTE_NAS_ESM_OTA_OUT_MSG_LOG_C:
  case LOG_LTE_NAS_EMM_SEC_OTA_IN_MSG_LOG_C:
  case LOG_LTE_NAS_EMM_SEC_OTA_OUT_MSG_LOG_C:
  case LOG_LTE_NAS_EMM_OTA_IN_MSG_LOG_C:
  case LOG_LTE_NAS_EMM_OTA_OUT_MSG_LOG_C:

    /* Allocate log packet */
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);    
    /* Check to see if log packet allocated */
    if (diag_log_ptr == NULL)
    {
      /* Could not allocate buffer */
      MSG_ERROR_1("Diag could not allocate the buffer for LOG : %d \n",log_code);
      return;
    }
    break;
  default:
    MSG_ERROR_0("Unknown DIAG packet received\n");
    return;
  } 
  /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      Featurization or Log versioning is done in this section
      Based on the spec used, std_major_version values are changed.
      As of now...
      FOR DECEMBER spec, we follow these values...
          std_major_version = 0;
          std_minro_version = 0;
      For MARCH SPEC....we would be using...
          std_major_version = 1;
          std_minor_version = 0;
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  log_version = LTE_NAS_DIAG_LOG_VERSION;
  std_version = 8;
  std_major_version = 2;
#ifdef FEATURE_LTE_REL9
  std_version = 9;
  std_major_version = 5;
#endif
  std_minor_version = 0;

/*+++++++++++++++++++++  END OF LONG VERSIONING +++++++++++++++++++++++++++*/
  /* Copy the log data into the logging memory space */
  /*lint -e644 */
  temp_ptr = diag_log_ptr + LOG_PTR_SIZE;
  memscpy(temp_ptr,pkt_size-LOG_PTR_SIZE, &log_version, sizeof(byte));
  temp_ptr++;
  memscpy(temp_ptr,pkt_size-LOG_PTR_SIZE-1, &std_version, sizeof(byte));
  temp_ptr++;
  memscpy(temp_ptr,pkt_size-LOG_PTR_SIZE-2, &std_major_version, sizeof(byte));
  temp_ptr++;
  memscpy(temp_ptr,pkt_size-LOG_PTR_SIZE-3, &std_minor_version, sizeof(byte));
  temp_ptr++;
  memscpy(temp_ptr,pkt_size-LOG_PTR_SIZE-4, nas_ota_msg_ptr, nas_emm_msg_size); 
  /*lint +e644 */
  /* Send the packet */
  log_commit((void*)diag_log_ptr); 
}


#if defined (OLD_NAS_LOGGING_DESIGN)

void send_nas_esm_log_packet
(
  word log_code    
)
{

  byte *diag_log_ptr;
  byte *temp_ptr;

  lte_nas_esm_bearer_context_state_T esm_bearer_ctxtstate_log;
  lte_nas_esm_bearer_context_info_T  esm_bearer_ctxt_info_log;
  lte_nas_esm_procedure_state_T      esm_proc_state_log;

  byte esm_val = 1;
  word pkt_size = 0;

  ASSERT(log_code != NULL);

  /*------------------------------------------------
         Logging based on log code received
  -------------------------------------------------*/
  
  switch(log_code)
  {
  case LOG_LTE_NAS_ESM_BEARER_CONTEXT_STATE_LOG_C:
    pkt_size = sizeof(log_hdr_type) + sizeof(lte_nas_esm_bearer_context_state_T);
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);
    /*---------------------------------------------------
            Fill in the values for the structure
    ---------------------------------------------------*/
    esm_bearer_ctxtstate_log.log_version = esm_val;
    esm_bearer_ctxtstate_log.bearer_id = esm_val;
    esm_bearer_ctxtstate_log.bearer_state = esm_val;
    esm_bearer_ctxtstate_log.connection_id = esm_val;

    temp_ptr = diag_log_ptr + sizeof(log_hdr_type);
    memscpy(temp_ptr,sizeof(lte_nas_esm_bearer_context_state_T),&esm_bearer_ctxtstate_log, sizeof(lte_nas_esm_bearer_context_state_T));
    break;
  case LOG_LTE_NAS_ESM_BEARER_CONTEXT_INFO_LOG_C:
    pkt_size = sizeof(log_hdr_type) + sizeof(lte_nas_esm_bearer_context_info_T);
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);
    /*---------------------------------------------------
            Fill in the values for the structure
    ---------------------------------------------------*/
    esm_bearer_ctxt_info_log.log_version = esm_val;
    esm_bearer_ctxt_info_log.context_type = esm_val;
    esm_bearer_ctxt_info_log.bearer_id = esm_val;
    esm_bearer_ctxt_info_log.bearer_state = esm_val;
    esm_bearer_ctxt_info_log.connection_id = esm_val;
    esm_bearer_ctxt_info_log.sdf_id = esm_val;
    memset(&esm_bearer_ctxt_info_log.sdf_id,&esm_val,LTE_NAS_OTA_MSG_MAX_SIZE);

    temp_ptr = diag_log_ptr + sizeof(log_hdr_type);
    memscpy(temp_ptr,sizeof(lte_nas_esm_bearer_context_info_T),&esm_bearer_ctxt_info_log, sizeof(lte_nas_esm_bearer_context_info_T));
    break;
  case LOG_LTE_NAS_ESM_PROCEDURE_STATE_LOG_C:
    pkt_size = sizeof(log_hdr_type) + sizeof(lte_nas_esm_procedure_state_T);
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);
    /*---------------------------------------------------
            Fill in the values for the structure
    ---------------------------------------------------*/
    esm_proc_state_log.log_version = esm_val;
    esm_proc_state_log.instance_id = esm_val;
    esm_proc_state_log.proc_state = esm_val;
    esm_proc_state_log.pti = esm_val;
    esm_proc_state_log.sdf_id = esm_val;
    esm_proc_state_log.pending_msg_id = esm_val;

    temp_ptr = diag_log_ptr + sizeof(log_hdr_type);
    memscpy(temp_ptr, sizeof(lte_nas_esm_procedure_state_T),&esm_proc_state_log, sizeof(lte_nas_esm_procedure_state_T));
    break;
  }
  log_commit((void*)diag_log_ptr);
}

void send_nas_emm_log_packet
(
  word                log_code,
  emm_ctrl_data_type* emm_log_info
)
{
  byte *diag_log_ptr;
  byte *temp_ptr;

  lte_nas_emm_state_type                emm_state_log;
  lte_nas_emm_usim_card_mode_type       emm_usim_card_mode_log;
  lte_nas_emm_current_sec_context_type  emm_current_sec_context_log;
  lte_nas_emm_ps_domain_int_cipher_keys ps_domain_cipher_log;
  lte_nas_emm_native_sec_context_type   emm_native_sec_context_log;

  byte emm_val = 1;
  word pkt_size = 0;

  ASSERT(log_code != NULL);  
  /*------------------------------------------------
         Logging based on log code received
  -------------------------------------------------*/
  
  /*lint -e564 */

  switch(log_code)
  {
  case LOG_LTE_NAS_EMM_STATE_LOG_C:
    memset(&emm_state_log,0,sizeof(lte_nas_emm_state_type));
    pkt_size = sizeof(log_hdr_type) + sizeof(lte_nas_emm_state_type);
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);
    /*---------------------------------------------------
            Fill in the values for the structure
    ---------------------------------------------------*/
    emm_state_log.log_version = emm_val;
    emm_state_log.emm_state = emm_log_info->emm_state;
    emm_state_log.emm_substate = emm_log_info->emm_substate;
    memset(&emm_state_log.plmn,emm_log_info->plmn_service_state.plmn,PLMN_ID_LEN);

    temp_ptr = diag_log_ptr + sizeof(log_hdr_type);
    memscpy(temp_ptr,sizeof(lte_nas_emm_state_type),&emm_state_log, sizeof(lte_nas_emm_state_type));
    break;
  case LOG_LTE_NAS_EMM_USIM_CARD_MODE_LOG_C:
    memset(&emm_usim_card_mode_log,0,sizeof(lte_nas_emm_usim_card_mode_type));
    pkt_size = sizeof(log_hdr_type) + sizeof(lte_nas_emm_usim_card_mode_type);
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);
    /*---------------------------------------------------
            Fill in the values for the structure
    ---------------------------------------------------*/
    emm_usim_card_mode_log.log_version = esm_val;
    emm_usim_card_mode_log.usim_card_mode = mm_sim_card_mode;
    emm_usim_card_mode_log.lte_service_supported = TRUE; /*Hariprasad shall revisit this, to update this value, 
                                                         by calling a funtion that gets lte_supported from usim service table*/
    memset(emm_usim_card_mode_log.imsi,emm_log_info->imsi,sizeof(IMSI_LEN));
    memset(emm_usim_card_mode_log.epsloci,emm_log_info->imsi,sizeof(IMSI_LEN));

    temp_ptr = diag_log_ptr + sizeof(log_hdr_type);
    memscpy(temp_ptr,sizeof(lte_nas_emm_usim_card_mode_type),&esm_bearer_ctxtstate_log, sizeof(lte_nas_emm_usim_card_mode_type));
    break;
  case LOG_LTE_NAS_EMM_CURRENT_SECUIRY_CONTEXT_LOG_C:
    memset(&emm_current_sec_context_log,0,sizeof(lte_nas_emm_current_sec_context_type));
    pkt_size = sizeof(log_hdr_type) + sizeof(lte_nas_emm_current_sec_context_type);
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);
    /*---------------------------------------------------
            Fill in the values for the structure
    ---------------------------------------------------*/
    emm_current_sec_context_log.log_version = 
    memset(&emm_current_sec_context_log.nas_ul_cnt,emm_log_info>context_info.nas_ul_cnt[0],NAS_UL_DL_COUNT); 
    memset(&emm_current_sec_context_log.nas_dl_cnt,emm_log_info>context_info.nas_dl_cnt[0],NAS_UL_DL_COUNT); 
    memset(&emm_current_sec_context_log.nas_integrity_algo_key,emm_log_info>context_info.nas_int_key[0],NAS_INT_CIPHER_KEY_LEN); 
    memset(&emm_current_sec_context_log.nas_cipher_algo_key,emm_log_info>context_info.nas_enc_key[0],NAS_UL_DL_COUNT); 
    emm_current_sec_context_log.nas_int_algo = emm_log_info>context_info.security_alg.integrity_protect_alg;
    emm_current_sec_context_log.ciphering_alg = emm_log_info>context_info.security_alg.ciphering_alg;
    emm_current_sec_context_log.nas_eksi = emm_log_info>context_info.nasKSI;   
    memset(&emm_current_sec_context_log.nas_kasme,emm_log_info>context_info.nas_kasme,KASME_LEN);     

    temp_ptr = diag_log_ptr + sizeof(log_hdr_type);
    memscpy(temp_ptr,sizeof(lte_nas_emm_current_sec_context_type),&emm_current_sec_context_log, sizeof(lte_nas_emm_current_sec_context_type));
    break;
  case LOG_LTE_NAS_EMM_CKIK_FOR_PS_DOMAIN_LOG_C:
    memset(&ps_domain_cipher_log,0,sizeof(lte_nas_emm_ps_domain_int_cipher_keys));
    pkt_size = sizeof(log_hdr_type) + sizeof(lte_nas_emm_ps_domain_int_cipher_keys);
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);
    /*---------------------------------------------------
            Fill in the values for the structure
    ---------------------------------------------------*/
    ps_domain_cipher_log.log_version = emm_val;
    ps_domain_cipher_log.keyset_id_ksips = emm_val;
    memset(&ps_domain_cipher_log.cipher_keys_ckps,&mm_auth_get_ck(),NAS_INT_CIPHER_KEY_LEN);     
    memset(&ps_domain_cipher_log.int_keys,&mm_auth_get_ik(),NAS_INT_CIPHER_KEY_LEN);     

    temp_ptr = diag_log_ptr + sizeof(log_hdr_type);
    memscpy(temp_ptr,sizeof(lte_nas_emm_ps_domain_int_cipher_keys),&ps_domain_cipher_log, sizeof(lte_nas_emm_ps_domain_int_cipher_keys));
    break;
  case LOG_LTE_NAS_EMM_NATIVE_SECURITY_CONTEXT_LOG_C:
    memset(&emm_native_sec_context_log,0,sizeof(lte_nas_emm_native_sec_context_type));
    pkt_size = sizeof(log_hdr_type) + sizeof(lte_nas_emm_native_sec_context_type);
    diag_log_ptr = nas_log_alloc(log_code, pkt_size, mm_message_as_id);
    /*---------------------------------------------------
            Fill in the values for the structure
    ---------------------------------------------------*/
    emm_native_sec_context_log.log_version             = emm_val;
    emm_native_sec_context_log.eps_nas_sec_context_len = emm_val;
    emm_native_sec_context_log.eps_nas_sec_context_tag = emm_val;
    emm_native_sec_context_log.ksi_tag                 = emm_val;
    emm_native_sec_context_log.ksi_len                 = emm_val;
    emm_native_sec_context_log.ksi_value               = emm_val;
    emm_native_sec_context_log.kasme_tag               = emm_val;
    memset(&emm_native_sec_context_log.kasme_value,&emm_val,MAX_KASME_VALUE_LEN);
    emm_native_sec_context_log.ul_nas_count_tag        = emm_val;
    emm_native_sec_context_log.ul_nas_count_len        = emm_val;
    memset(&emm_native_sec_context_log.ul_nas_count_value,&emm_val,NAS_COUNT_LEN); 
    emm_native_sec_context_log.dl_nas_count_tag        = emm_val;
    emm_native_sec_context_log.dl_nas_count_len        = emm_val;
    memset(&emm_native_sec_context_log.dl_nas_count_value,&emm_val,NAS_COUNT_LEN); 
    emm_native_sec_context_log.nas_algo_tag            = emm_val;
    emm_native_sec_context_log.nas_algo_len            = emm_val;
    emm_native_sec_context_log.nas_algo_value          = emm_val;   
    temp_ptr = diag_log_ptr + sizeof(log_hdr_type);
    memscpy(temp_ptr,sizeof(lte_nas_emm_native_sec_context_type),&emm_native_sec_context_log, sizeof(lte_nas_emm_native_sec_context_type));
    break;
  }
  /*-----------------------------------
            Send the packet 
  -----------------------------------*/
  log_commit((void*)diag_log_ptr);
}
#endif

#endif /*FEATURE_LTE*/


