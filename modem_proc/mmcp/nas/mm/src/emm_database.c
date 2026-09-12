

/*===========================================================================

  Copyright (c) 2008 Qualcomm Technologies Incorporated. All Rights Reserved

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


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mmcp.mpss/7.9.0/nas/mm/src/emm_database.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
============================================================================
10/06/09   vdr     Compiler warnings fixed
04/14/09    RI      Added emm_db_get_ctrl_data() for MM Authentication Server.
============================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmcp_variation.h"
#include<customer.h>
#ifdef FEATURE_LTE

#include "comdef.h"
#include "emm.h"
#include "emm_database.h"
#include "emm_utility.h"
#include "emm_rrc_if.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#if defined(FEATURE_TRIPLE_SIM)
emm_ctrl_data_type *emm_ctrl_data_ptr_sim[MAX_AS_IDS] = {NULL, NULL,NULL};
#elif defined FEATURE_DUAL_SIM
emm_ctrl_data_type *emm_ctrl_data_ptr_sim[MAX_AS_IDS] = {NULL, NULL};
#endif
emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_DATA)
emm_reg_service_report_type reg_service_report_sim[MAX_AS_IDS];
#define reg_service_report reg_service_report_sim[mm_as_id]
#else
emm_reg_service_report_type reg_service_report;
#endif

/*===========================================================================
 
FUNCTION    EMM_DB_ALLOC_MEMORY

DESCRIPTION
  This function allocates the memory for emm_ctrl_data pointer for two stacks for a dual SIM device.
  For single SIM device only one heap block is allocated

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void emm_db_alloc_memory()
{
  size_t  size = sizeof(emm_ctrl_data_type);
#ifdef FEATURE_DUAL_SIM
  mm_as_id_e_type  as_id;
  for(as_id = MM_AS_ID_1; as_id < (mm_as_id_e_type)MAX_AS_IDS; as_id++)
  {
    emm_ctrl_data_ptr_sim[as_id] = modem_mem_calloc(1, size, MODEM_MEM_CLIENT_NAS);
   
    mm_check_for_null_ptr((void*)emm_ctrl_data_ptr_sim[as_id] );
  }
#else
  emm_ctrl_data_ptr = modem_mem_calloc(1, size, MODEM_MEM_CLIENT_NAS);
  mm_check_for_null_ptr((void*)emm_ctrl_data_ptr);
#endif
}

 /*===========================================================================

FUNCTION    EMM_DB_GET_CTRL_DATA_PER_SUBS

DESCRIPTION
  This function returns a pointer to the heap memory block based on the as_id

DEPENDENCIES
  None

RETURN VALUE
  emm_ctrl_data_type*

SIDE EFFECTS
  None
===========================================================================*/

emm_ctrl_data_type *emm_db_get_ctrl_data_per_subs(mm_as_id_e_type as_id)
{
#ifdef FEATURE_DUAL_SIM
  return emm_ctrl_data_ptr_sim[as_id];
#else
  (void)as_id;
  return emm_ctrl_data_ptr;
#endif
}

/*===========================================================================

FUNCTION    EMM_DB_GET_CTRL_DATA

DESCRIPTION
  This function returns a pointer to the heap memory block based on the current subscription ID.

DEPENDENCIES
  None

RETURN VALUE
  emm_ctrl_data_type*

SIDE EFFECTS
  None
===========================================================================*/
emm_ctrl_data_type *emm_db_get_ctrl_data(void)
{
#ifdef FEATURE_DUAL_SIM
  return emm_ctrl_data_ptr_sim[mm_sub_id];
#else
  return emm_ctrl_data_ptr;
#endif
}
/*===========================================================================

FUNCTION    EMM_SET_CONNECTION_STATE

DESCRIPTION
  Sets Connection state to the passed value to this function.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_SET_CONNECTION_STATE(emm_connection_state_type emm_connection_state) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  MSG_HIGH_DS_1(MM_SUB,"=EMM= Set connection state %u",emm_connection_state);   
  emm_ctrl_data_ptr->emm_connection_state = emm_connection_state;
}

/*===========================================================================

FUNCTION    EMM_MOVES_TO_IDLE_STATE

DESCRIPTION
  Sets Connection State to IDLE

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_MOVES_TO_IDLE_STATE(void)
{
  EMM_SET_CONNECTION_STATE(EMM_IDLE_STATE); 
  emm_set_process_type(SYS_PROC_TYPE_NONE);
  auth_reset_cache_memory(FALSE, NO_TIMER_EXPIRED, RRC_PS_DOMAIN_CN_ID);
}

/*===========================================================================

FUNCTION    EMM_MOVES_TO_WAITING_FOR_RRC_CONFIRMATION_STATE

DESCRIPTION
  Sets Connection state to 'EMM_WAITING_FOR_RRC_CONFIRMATION_STATE'.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void  EMM_MOVES_TO_WAITING_FOR_RRC_CONFIRMATION_STATE(void)
{
  EMM_SET_CONNECTION_STATE(EMM_WAITING_FOR_RRC_CONFIRMATION_STATE);
}

/*===========================================================================

FUNCTION    EMM_MOVES_TO_CONNECTED_STATE_STATE

DESCRIPTION
  Sets Connection state to 'EMM_CONNECTED_STATE'.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void  EMM_MOVES_TO_CONNECTED_STATE_STATE(void ) 
{
  EMM_SET_CONNECTION_STATE(EMM_CONNECTED_STATE);
}
/*===========================================================================

FUNCTION    EMM_MOVES_TO_RELEASING_RRC_CONNECTION_STATE

DESCRIPTION
  Sets Connection state to EMM_MOVES_TO_RELEASING_RRC_CONNECTION_STATE.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void  EMM_MOVES_TO_RELEASING_RRC_CONNECTION_STATE(void) 
{
  EMM_SET_CONNECTION_STATE(EMM_RELEASING_RRC_CONNECTION_STATE);  
  emm_set_process_type(SYS_PROC_TYPE_NONE);
#ifdef FEATURE_DUAL_SIM
  mm_stop_timer(EMM_MT_PAGE_EXT_PRI_TIMER);
#endif
}
/*===========================================================================

FUNCTION    EMM_GET_CONNECTION_STATE

DESCRIPTION
  Returns the current connection state.

DEPENDENCIES
  None

RETURN VALUE
  Current connection state

SIDE EFFECTS
  None
===========================================================================*/

emm_connection_state_type  EMM_GET_CONNECTION_STATE(void) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  return emm_ctrl_data_ptr->emm_connection_state;
}
/*===========================================================================

FUNCTION    EMM_SET_STATE

DESCRIPTION
  Sets EMM state to the passed value to this function.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_SET_STATE(emm_state_type emm_state) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  MSG_HIGH_DS_1(MM_SUB,"=EMM= Set state %u",emm_state); 
  emm_ctrl_data_ptr->emm_state = emm_state;
#ifdef FEATURE_DUAL_SIM
  if(emm_state != EMM_REGISTERED)
  {
    mm_stop_timer(EMM_MT_PAGE_EXT_PRI_TIMER);
  }
#endif
}
/*===========================================================================

FUNCTION    EMM_SET_SUBSTATE

DESCRIPTION
  Sets EMM Substate state to the passed value to this function.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_SET_SUBSTATE(emm_substate_type emm_substate) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  MSG_HIGH_DS_1(MM_SUB,"=EMM= Set substate %u",emm_substate); 
  emm_ctrl_data_ptr->emm_substate = (emm_substate_type)emm_substate;
}
/*===========================================================================

FUNCTION    EMM_MOVES_TO_NULL_STATE

DESCRIPTION
  Sets EMM State to EMM_NULL

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void  EMM_MOVES_TO_NULL_STATE(void) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  EMM_SET_STATE(EMM_NULL);  
  send_emm_state_log_info((byte)emm_ctrl_data_ptr->emm_state, 
                          (byte)emm_ctrl_data_ptr->emm_substate,
                          &emm_ctrl_data_ptr->plmn_service_state->plmn,
                          emm_ctrl_data_ptr->emm_guti_valid,
                          &emm_ctrl_data_ptr->emm_guti);
                          auth_reset_cache_memory(FALSE, NO_TIMER_EXPIRED, RRC_PS_DOMAIN_CN_ID);
}
/*===========================================================================

FUNCTION    EMM_MOVES_TO_DEREGISTERED_STATE

DESCRIPTION
  Sets EMM State to EMM_DEREGISTERED

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_MOVES_TO_DEREGISTERED_STATE(emm_substate_type emm_substate) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  if (emm_ctrl_data_ptr->emm_state == EMM_DEREGISTERED_INITIATED) 
  {
    emm_send_diag_event(EVENT_NAS_MO_DETACH, (nas_event_end_result)PROC_END, 
               (nas_event_end_reason)PROC_END_ACCEPT, (lte_nas_emm_cause_type)LTE_NAS_CAUSE_NONE);
  }
   else if(emm_ctrl_data_ptr->emm_state == EMM_REGISTERED_INITIATED 
#ifdef FEATURE_DUAL_SIM  
  	&&!(emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr != NULL &&
	 emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr->cmd.hdr.message_id == MMR_STOP_MODE_REQ && 
	 emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH)
#endif	 
	 )
  {
	// DO no reset this flag if UE is moving to DEREG state because of DUAL SWITCH in between ATTACH, as we
    // need to ignore if NW sends same ATTACH ACCET message during re-attach.	
    emm_ctrl_data_ptr->ignore_attach_pti_mismatch = FALSE;
  }

  if (emm_ctrl_data_ptr->mt_detach_info.type != INVALID_MT_DETACH_TYPE)
  { 
    emm_send_diag_event(EVENT_NAS_MT_DETACH,(nas_event_end_result)PROC_END,
                (nas_event_end_reason)PROC_END_ACCEPT,(lte_nas_emm_cause_type)LTE_NAS_CAUSE_NONE);
  } 

  EMM_SET_STATE(EMM_DEREGISTERED) ;
  MSG_HIGH_DS_1(MM_SUB,"=EMM= Set substate %u",emm_substate);      
#if defined(FEATURE_DUAL_SIM) && defined (FEATURE_DUAL_DATA)
  emm_reset_esr_tau_pending_conn_rel(LTE_RRC_FORCE_ESR_ENDED);
#endif
  emm_ctrl_data_ptr->emm_substate = (emm_substate_type)emm_substate ;      
  emm_ctrl_data_ptr->rau_pending_ll_failure = FALSE;

  if (emm_ctrl_data_ptr->emm_substate != EMM_DEREGISTERED_ATTEMPTING_TO_ATTACH)
  {
    is_nas_emm_attach_tau_timeout = FALSE;
  }

  mm_stop_timer(TIMER_T3412);           
  mm_stop_timer(TIMER_T3423);
#ifndef FEATURE_HA_MAINLINE_ONLY_ENABLE
  mm_stop_timer(TIMER_DELAY_TAU);
#endif	
  mm_stop_timer(TIMER_RADIO_CAP_ENQ);     
  MSG_HIGH_DS_0(MM_SUB,"=EMM= T3412 stopped - Deregistered state");
  emm_ctrl_data_ptr->is_periodic_tau_required = FALSE;
  emm_ctrl_data_ptr->tau_cause = TAU_CAUSE_INVALID;  
  emm_ctrl_data_ptr->radio_cap_update = EMM_RADIO_CAP_UPDATE_NONE;
  emm_ctrl_data_ptr->is_syncup_tau_for_local_bear_deact_needed = FALSE;
  emm_ctrl_data_ptr->ue_initial_cap_enq_ratmask  = 0x00;
  emm_ctrl_data_ptr->nw_initial_cap_enq_ratmask  = 0x00;
  emm_ctrl_data_ptr->pdn_reject_cause_55_cnt = 0;
  emm_ctrl_data_ptr->esm_proc_retry_ind = FALSE;
  /* Reset the CS reject cause */
  emm_ctrl_data_ptr->combined_attach_cs_rej_cause = LTE_NAS_CAUSE_NONE;
  if(emm_ctrl_data_ptr->emm_tai_list_ptr != NULL)  
  {  
    emm_ctrl_data_ptr->emm_tai_list_ptr->tai_lst_length = 0;  
  }  
  emm_ctrl_data_ptr->last_tau_type = TAU_CAUSE_INVALID;
  memset((void*)&(emm_ctrl_data_ptr->eps_bearer_context_status),0, 
                        sizeof(lte_nas_eps_bearer_context_status_type)); 

  emm_ctrl_data_ptr->eps_bearer_context_status_updated = FALSE;
  emm_delete_mapped_context();   
  send_emm_state_log_info((byte)emm_ctrl_data_ptr->emm_state, 
                          (byte)emm_ctrl_data_ptr->emm_substate,
                          &emm_ctrl_data_ptr->plmn_service_state->plmn,
                          emm_ctrl_data_ptr->emm_guti_valid,
                          &emm_ctrl_data_ptr->emm_guti);

  if(emm_ctrl_data_ptr->rlf_state == FALSE)
  {
  gmm_multimode_set_state(GMM_DEREGISTERED, GMM_NO_CELL_AVAILABLE);
  }

  if((emm_ctrl_data_ptr->ps_detach_info.status != EMM_PS_DETACH_NONE) &&
     (emm_ctrl_data_ptr->mt_detach_info.type == INVALID_MT_DETACH_TYPE))
  {
    emm_send_ps_detach_cnf(emm_ctrl_data_ptr);
  }

  if ((is_lte_mode() == TRUE) &&
      ((*emm_ctrl_data_ptr->set_drx_req_pending_ptr == NAS_DRX_CFG_REQ_PROCESSING) ||
       (*emm_ctrl_data_ptr->set_drx_req_pending_ptr == NAS_DRX_CFG_REQ_PENDING)) &&
      (emm_get_gmm_drx_coff_client_rat() == SYS_RAT_MAX))
  {
    mm_send_mmr_set_drx_cnf(FALSE);
  }
  else if ((is_lte_mode() == TRUE) &&
           (*emm_ctrl_data_ptr->set_drx_req_pending_ptr == NAS_DRX_CFG_REQ_PROCESSING))
  {
    *emm_ctrl_data_ptr->set_drx_req_pending_ptr = NAS_DRX_CFG_REQ_PENDING;
  }

  emm_reset_emc_srv_state(emm_ctrl_data_ptr);

  if (emm_ctrl_data_ptr->emm_lte_mode_change_pending == TRUE) 
  {
    emm_ctrl_data_ptr->emm_lte_mode_change_pending = FALSE;
  }
  nas_emm_power_off_detach_type = INVALID_DETACH_TYPE;
  auth_reset_cache_memory(FALSE, NO_TIMER_EXPIRED, RRC_PS_DOMAIN_CN_ID);
#ifdef FEATURE_GSTK  
  if (emm_ctrl_data_ptr->send_idle_event_gstk == TRUE &&
  	  emm_ctrl_data_ptr->detach_reason == SWITCH_OFF)
  {
	if (emm_ctrl_data_ptr->last_rrc_service_ind_ptr->svc_status == LTE_RRC_SVC_STATUS_NOT_AVAILABLE) 
    {
	  emm_send_no_svc_to_gstk();	
	}
    else
    {		
      emm_send_limited_svc_to_gstk(emm_ctrl_data_ptr);
	}  
  }
#endif  
#ifdef FEATURE_DUAL_SIM
   if(emm_ctrl_data_ptr->dual_switch_pended == TRUE &&
	 emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr != NULL &&
	 emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr->cmd.hdr.message_id == MMR_STOP_MODE_REQ && 
	 emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH
	 )
   {
     emm_process_stop_mode_req(emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr,emm_ctrl_data_ptr);
   }
#endif    
  emm_ctrl_data_ptr->tau_pending_gw_attach = FALSE; 
}

/*===========================================================================

FUNCTION    EMM_MOVES_TO_REGISTERED_INITIATED_STATE

DESCRIPTION
  Sets EMM State to EMM_REGISTERED_INITIATED

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_MOVES_TO_REGISTERED_INITIATED_STATE(emm_substate_type emm_substate)  
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  EMM_SET_STATE(EMM_REGISTERED_INITIATED) ;        
  MSG_HIGH_DS_1(MM_SUB,"=EMM= Set substate %u",emm_substate);      
  if(emm_substate == ((emm_substate_type)EMM_REGISTERED_LIMITED_SERVICE))
  {
    emm_send_limited_svc_to_gstk(emm_ctrl_data_ptr);
  }
  emm_ctrl_data_ptr->emm_substate = (emm_substate_type)emm_substate ;  
  send_emm_state_log_info((byte)emm_ctrl_data_ptr->emm_state, 
                          (byte)emm_ctrl_data_ptr->emm_substate,
                          &emm_ctrl_data_ptr->plmn_service_state->plmn,
                          emm_ctrl_data_ptr->emm_guti_valid,
                          &emm_ctrl_data_ptr->emm_guti);
}
/*===========================================================================

FUNCTION    EMM_MOVES_TO_REGISTERED_STATE

DESCRIPTION
  Sets EMM State to EMM_REGISTERED

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_MOVES_TO_REGISTERED_STATE(emm_substate_type emm_substate) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

#if defined(FEATURE_DUAL_SIM) && defined (FEATURE_DUAL_DATA)
  if((EMM_GET_STATE() == EMM_SERVICE_REQUEST_INITIATED) &&
     (emm_ctrl_data_ptr->esr_tau_pending_conn_rel == TRUE))
  {
    emm_send_rrc_force_esr_ind(LTE_RRC_FORCE_ESR_ENDED,emm_ctrl_data_ptr);
  }
#endif

  EMM_SET_STATE(EMM_REGISTERED) ;
  MSG_HIGH_DS_1(MM_SUB,"=EMM= Set substate %u",emm_substate);
  emm_ctrl_data_ptr->emm_substate = (emm_substate_type)emm_substate ; 
  if( emm_ctrl_data_ptr->emm_substate == ((emm_substate_type)EMM_REGISTERED_NORMAL_SERVICE )) 
  { 
    emm_send_registered_evt_to_gstk(emm_ctrl_data_ptr); 
  } 
  if(emm_ctrl_data_ptr->emm_substate == EMM_REGISTERED_IMSI_DETACH_INITIATED)
  {
    emm_send_diag_event(EVENT_NAS_MO_DETACH, (nas_event_end_result)PROC_START, 
                (nas_event_end_reason)PROC_END_NO_REASON, (lte_nas_emm_cause_type)LTE_NAS_CAUSE_NONE);
  }
  send_emm_state_log_info((byte)emm_ctrl_data_ptr->emm_state, 
                          (byte)emm_ctrl_data_ptr->emm_substate, 
                          &emm_ctrl_data_ptr->plmn_service_state->plmn,
                          emm_ctrl_data_ptr->emm_guti_valid,
                          &emm_ctrl_data_ptr->emm_guti);
}
/*===========================================================================

FUNCTION    EMM_REGISTERED_CHANGE_SUB_STATE

DESCRIPTION
 In EMM_REGISTERED state, this function changes the substate to the passed value.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_REGISTERED_CHANGE_SUB_STATE(emm_substate_type emm_substate) 
{
  
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  EMM_SET_STATE(EMM_REGISTERED) ;        
  MSG_HIGH_DS_1(MM_SUB,"=EMM= Set substate %u",emm_substate);      
  emm_ctrl_data_ptr->emm_substate = (emm_substate_type)emm_substate ;   
}


/*===========================================================================

FUNCTION    EMM_MOVES_TO_TRACKING_AREA_UPDATING_INITIATED_STATE

DESCRIPTION
 Sets EMM State to EMM_TRACKING_AREA_UPDATING_INITIATED.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void EMM_MOVES_TO_TRACKING_AREA_UPDATING_INITIATED_STATE(void) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  EMM_SET_STATE(EMM_TRACKING_AREA_UPDATING_INITIATED);   
  send_emm_state_log_info((byte)emm_ctrl_data_ptr->emm_state, 
                          (byte)emm_ctrl_data_ptr->emm_substate, 
                          &emm_ctrl_data_ptr->plmn_service_state->plmn,
                          emm_ctrl_data_ptr->emm_guti_valid,
                          &emm_ctrl_data_ptr->emm_guti);
}
/*===========================================================================

FUNCTION    EMM_MOVES_TO_SERVICE_REQUEST_INITIATED_STATE

DESCRIPTION
  Sets EMM state to EMM_SERVICE_REQUEST_INITIATED.

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void  EMM_MOVES_TO_SERVICE_REQUEST_INITIATED_STATE(void) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  EMM_SET_STATE(EMM_SERVICE_REQUEST_INITIATED);  
  send_emm_state_log_info((byte)emm_ctrl_data_ptr->emm_state, 
                          (byte)emm_ctrl_data_ptr->emm_substate, 
                          &emm_ctrl_data_ptr->plmn_service_state->plmn,
                          emm_ctrl_data_ptr->emm_guti_valid,
                          &emm_ctrl_data_ptr->emm_guti);
}
/*===========================================================================

FUNCTION    EMM_MOVES_TO_DEREGISTERED_INITIATED_STATE

DESCRIPTION
  Sets EMM state to EMM_DEREGISTERED_INITIATED

DEPENDENCIES
  None

RETURN VALUE
  None*

SIDE EFFECTS
  None
===========================================================================*/

void  EMM_MOVES_TO_DEREGISTERED_INITIATED_STATE(void) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  EMM_SET_STATE(EMM_DEREGISTERED_INITIATED);  
#if defined(FEATURE_DUAL_SIM) && defined (FEATURE_DUAL_DATA)
  emm_reset_esr_tau_pending_conn_rel(LTE_RRC_FORCE_ESR_ENDED);
#endif
  send_emm_state_log_info((byte)emm_ctrl_data_ptr->emm_state, 
                          (byte)emm_ctrl_data_ptr->emm_substate, 
                          &emm_ctrl_data_ptr->plmn_service_state->plmn,
                          emm_ctrl_data_ptr->emm_guti_valid,
                          &emm_ctrl_data_ptr->emm_guti);
  emm_send_diag_event(EVENT_NAS_MO_DETACH, (nas_event_end_result)PROC_START, 
                (nas_event_end_reason)PROC_END_NO_REASON, (lte_nas_emm_cause_type)LTE_NAS_CAUSE_NONE);
}
/*===========================================================================

FUNCTION    EMM_GET_STATE

DESCRIPTION
  Returns current EMM State.

DEPENDENCIES
  None

RETURN VALUE
  current EMM state

SIDE EFFECTS
  None
===========================================================================*/            

emm_state_type EMM_GET_STATE(void)
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  return (emm_ctrl_data_ptr->emm_state);
}
/*===========================================================================

FUNCTION    EMM_GET_SUBSTATE

DESCRIPTION
  Returns current EMM substate.

DEPENDENCIES
  None

RETURN VALUE
  current EMM substate

SIDE EFFECTS
  None
===========================================================================*/

emm_substate_type EMM_GET_SUBSTATE(void) 
{
  emm_ctrl_data_type        *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  return((emm_substate_type)(emm_ctrl_data_ptr->emm_substate));
}

#ifdef FEATURE_QSH_MDUMP

QSH_MDUMP_FN_ATTR void emm_log_mdump_vars() 
{
#if defined FEATURE_DUAL_SIM
  mm_as_id_e_type temp_sub_id;
  temp_sub_id = mm_sub_id;
#endif

#if defined FEATURE_DUAL_SIM
  for (mm_sub_id = MM_AS_ID_1; mm_sub_id < (mm_as_id_e_type)MAX_NAS_STACKS; mm_sub_id++)
#endif
  {
    qsh_mdump_collect_high(&emm_ctrl_data_ptr_sim[mm_sub_id],sizeof(emm_ctrl_data_ptr));
    if (emm_ctrl_data_ptr != NULL)
    {
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id],sizeof(emm_ctrl_data_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->plmn_service_state,sizeof(sys_plmn_service_state_s_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->emm_tai_list_ptr,sizeof(lte_nas_emm_tai_lst_info_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->emm_location_area,sizeof(location_information_T));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->rrc_forbidden_list_ptr,sizeof(lte_nas_tai_lst2_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->forbidden_for_service_list_ptr,sizeof(lte_nas_tai_lst2_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->last_rrc_service_ind_ptr,sizeof(lte_rrc_service_ind_s));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->reg_req_pending_mm_cmd_ptr,sizeof(mm_cmd_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->pdn_conn_req_ptr,sizeof(lte_nas_esm_pdn_connectivity_req));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->nas_incoming_msg_ptr,sizeof(lte_nas_incoming_msg_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->emm_ota_message_ptr,sizeof(byte));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->pending_reg_cmd_ptr,sizeof(mm_cmd_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->reg_lte_pended_cs_domain_param_ptr,sizeof(sys_lte_cs_domain_param_s_type));
      qsh_mdump_collect_high(emm_ctrl_data_ptr_sim[mm_sub_id]->conn_mode_manual_search_ptr,sizeof(boolean));
    }
  }
#if defined FEATURE_DUAL_SIM
  mm_sub_id = temp_sub_id;
#endif
}

#endif

#endif /*FEATURE_LTE*/

