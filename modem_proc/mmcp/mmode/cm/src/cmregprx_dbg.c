/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

           C M    R E G P R O X Y   D E B U G   M O D U L E

GENERAL DESCRIPTION
  This module contains the debug related functionality of CM REG Proxy.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 1991 - 2014 by Qualcomm Technologies INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/cm/src/cmregprx_dbg.c#1 $

===========================================================================*/

/*lint -save -e656 -e641
** Turn off enum to int / operationuses compatible enums
*/

/**--------------------------------------------------------------------------
** Includes
** --------------------------------------------------------------------------
*/

#include "cmregprx_dbg.h"

#include "mmcp_variation.h"
#include "customer.h"
#include "cm.h"
#include "cm_v.h"
#include "cmi.h"
#include "comdef.h"
#include <string.h>
#include <stringl/stringl.h>

#include "cmdbg.h"

#if (defined(FEATURE_MMODE_CDMA_800) || defined(FEATURE_MMODE_CDMA_1900))
#include "cmxll.h"
#include "mc.h"
#include "mc_v.h"
#endif

#ifdef CM_GW_SUPPORTED
#include "cmwll.h"
#endif

#ifdef CM_DEBUG
#error code not present
#endif

#include "cmtaski.h"
#include "cmsimcoord.h"
#include "mmgsdilib_common.h"

#include "cmefs.h"

/**--------------------------------------------------------------------------
** Defines
** --------------------------------------------------------------------------
*/

extern cmregprx_stack_info_s_type *cmregprx_map_as_id_to_stack_info
(
  cmregprx_info_s_type            *cmregprx_info_ptr,
  /* Pointer to RegProxy's information.
  */

  sys_modem_as_id_e_type          as_id,

  sys_modem_stack_id_e_type       stack_id,

  cm_name_type                    rpt
);


/**--------------------------------------------------------------------------
** Datatypes
** --------------------------------------------------------------------------
*/

#if (defined(CM_GW_SUPPORTED) || defined(FEATURE_CM_LTE)||defined(FEATURE_TDSCDMA))


#ifdef MMODE_ADDITIONAL_DEBUG_INFO
static cmregprx_dbg_buffer_s_type cmregprx_dbg_buffer;


/**--------------------------------------------------------------------------
** Functions
** --------------------------------------------------------------------------
*/

/*===========================================================================
FUNCTION cmregprx_dbg_ptr

DESCRIPTION
  Return a pointer to the one and only CM REGPRX debug buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
#ifdef FEATURE_QSH_MDUMP 
QSH_MDUMP_FN_ATTR
#endif
cmregprx_dbg_buffer_s_type  *cmregprx_dbg_ptr( void )
{
  return &cmregprx_dbg_buffer;
} /* cmdbg_ptr() */

/*===========================================================================
FUNCTION cmregprx_dbg_init

DESCRIPTION
  Initialize the CM REGPRX Debug Buffer

  This function must be called before any CM submodule init,
  in order to capture the commands

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_init( void )
{
  cmregprx_dbg_buffer_s_type   *dbg_ptr = cmregprx_dbg_ptr();

  uint8 idx1 = 0, idx2 = 0, stk_idx = 0;

  dbg_ptr->dbg_buf_idx            = 0xFF;

  cmregprx_dbg_set_stack_id( SYS_MODEM_AS_ID_MAX, SYS_MODEM_STACK_ID_MAX );

  for (idx1 = 0; idx1 < CMREGPRX_DEBUG_BUFFER_SIZE; idx1++)
  {
    dbg_ptr->dbg_buf_msg[idx1].timestamp         =  0;
    dbg_ptr->dbg_buf_msg[idx1].idx               =  0;

    for(idx2 = 0 ; idx2 < MAX_SIMS; idx2++)
    {
      for (stk_idx = 0; stk_idx < CMREGPRX_NUM_MAX_STACKS; stk_idx ++)
      {
        dbg_ptr->dbg_buf_msg[idx1].cmregprx_state[idx2][stk_idx] = CMREGPRX_STATE_MAX;
        dbg_ptr->dbg_buf_msg[idx1].cmregprx_substate[idx2][stk_idx] = CMREGPRX_SUBSTATE_MAX;
      }
    }

    dbg_ptr->dbg_buf_msg[idx1].mm_id.asubs_id = SYS_MODEM_AS_ID_NONE;
    dbg_ptr->dbg_buf_msg[idx1].mm_id.stk_id = MM_STACK_NONE;
    dbg_ptr->dbg_buf_msg[idx1].msg_type = CMREGPRX_BUFF_TYPE_NONE;
    dbg_ptr->dbg_buf_msg[idx1].msg_name.rpt = 0;
    dbg_ptr->dbg_buf_msg[idx1].msg_payload = NULL;

  }
}

/*===========================================================================


FUNCTION cmregprx_dbg_set_stack_id

DESCRIPTION
  This function stores the value of stack_id and as_id received in current
  report so it can be used to determine the ss on which report is received,
  at time of adding the report to debug buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void                    cmregprx_dbg_set_stack_id
(

  sys_modem_as_id_e_type                 as_id,

  sys_modem_stack_id_e_type              stack_id

)
{

  cmregprx_dbg_buffer.asubs_id_for_current_report = as_id;
  cmregprx_dbg_buffer.stack_id_for_current_report = stack_id;

}/*cmregprx_dbg_buf_set_stack_id()*/


/*===========================================================================
FUNCTION cmregprx_dbg_add_mmoc_cmd_to_buffer

DESCRIPTION
  Add MMOC command to Debug Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_mmoc_cmd_to_buffer
(
  const cmregprx_cmd_msg_s_type  *cmd_ptr
)
{
  if(cmregprx_dbg_if_same_mmoc_cmd(cmd_ptr))
  {
    cmregprx_dbg_buffer.dbg_buf_msg[cmregprx_dbg_buffer.dbg_buf_idx].timestamp = time_get_uptime_secs();
    return;
  }

  switch (cmd_ptr->name)
  {
    case PROT_CMD_ACTIVATE:
    {
      CMREGPRX_DBG_MEM_ALLOC(act_prot_dbg_buf_msg, prot_act_s_type)

      act_prot_dbg_buf_msg->reason = cmd_ptr->param.act_prot.reason;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)act_prot_dbg_buf_msg);
      break;
    }
    case PROT_CMD_DEACTIVATE:
    {
      CMREGPRX_DBG_MEM_ALLOC(deact_prot_dbg_buf_msg, prot_deact_s_type)

      deact_prot_dbg_buf_msg->reason = cmd_ptr->param.deact_prot.reason;
      deact_prot_dbg_buf_msg->trans_id = cmd_ptr->param.deact_prot.trans_id;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)deact_prot_dbg_buf_msg);
      break;
    }
    case PROT_CMD_GENERIC:
    {
      CMREGPRX_DBG_MEM_ALLOC(gen_cmd_dbg_buf_msg, cmregprx_dbg_prot_gen_cmd_s_type)

      gen_cmd_dbg_buf_msg->cmd_type = cmd_ptr->param.gen_cmd.cmd_type;

      gen_cmd_dbg_buf_msg->mode_pref = SD_SS_MODE_PREF_NONE;
      gen_cmd_dbg_buf_msg->network_list_type = SD_NETWORK_LIST_NONE;
      gen_cmd_dbg_buf_msg->pref_reas = SD_SS_PREF_REAS_MAX;
      gen_cmd_dbg_buf_msg->orig_mode = SD_SS_ORIG_MODE_NONE;
      gen_cmd_dbg_buf_msg->domain_pref = SD_SS_SRV_DOMAIN_PREF_NONE;
      gen_cmd_dbg_buf_msg->pref_update_reas = SD_SS_PREF_UPDATE_REASON_NONE;

      if(gen_cmd_dbg_buf_msg->cmd_type == PROT_GEN_CMD_GET_NETWORKS_GW)
      {
        gen_cmd_dbg_buf_msg->mode_pref = cmd_ptr->param.gen_cmd.param.gw_get_net.mode_pref;
        gen_cmd_dbg_buf_msg->network_list_type = cmd_ptr->param.gen_cmd.param.gw_get_net.network_list_type;
      }
      else if(gen_cmd_dbg_buf_msg->cmd_type == PROT_GEN_CMD_PREF_SYS_CHGD)
      {
        gen_cmd_dbg_buf_msg->mode_pref = cmd_ptr->param.gen_cmd.param.pref_sys_chgd.mode_pref;
        gen_cmd_dbg_buf_msg->pref_reas = cmd_ptr->param.gen_cmd.param.pref_sys_chgd.pref_reas;
        gen_cmd_dbg_buf_msg->orig_mode = cmd_ptr->param.gen_cmd.param.pref_sys_chgd.orig_mode;
        gen_cmd_dbg_buf_msg->domain_pref = cmd_ptr->param.gen_cmd.param.pref_sys_chgd.domain_pref;
        gen_cmd_dbg_buf_msg->pref_update_reas = cmd_ptr->param.gen_cmd.param.pref_sys_chgd.pref_update_reas;
      }
      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)gen_cmd_dbg_buf_msg);

      break;
    }

    case PROT_CMD_PH_STAT_CHGD:
    {
      CMDBG_MEM_ALLOC(gw_ph_stat_cmd_dbg_buf_msg, cmregprx_ph_stat_chgd_s_type)

      memscpy( gw_ph_stat_cmd_dbg_buf_msg,sizeof(cmregprx_ph_stat_chgd_s_type),
               &(cmd_ptr->param.gw_ph_stat_cmd), sizeof(cmregprx_ph_stat_chgd_s_type));

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)gw_ph_stat_cmd_dbg_buf_msg);
      break;
    }
    case PROT_CMD_DS_STAT_CHGD:
    {
      CMDBG_MEM_ALLOC(ds_stat_chgd_cmd_dbg_buf_msg, prot_ds_stat_chgd_s_type)

      memscpy( ds_stat_chgd_cmd_dbg_buf_msg,sizeof(prot_ds_stat_chgd_s_type),
               &(cmd_ptr->param.ds_stat_chgd_cmd), sizeof(prot_ds_stat_chgd_s_type));

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)ds_stat_chgd_cmd_dbg_buf_msg);
      break;
    }
    case PROT_CMD_PS_DETACH:
    {
      CMREGPRX_DBG_MEM_ALLOC(ps_detach_cmd_dbg_buf_msg, cmregprx_ps_detach_s_type)

      ps_detach_cmd_dbg_buf_msg->type = cmd_ptr->param.ps_detach_cmd.type;
      ps_detach_cmd_dbg_buf_msg->trans_id = cmd_ptr->param.ps_detach_cmd.trans_id;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)ps_detach_cmd_dbg_buf_msg);
      break;
    }
    case PROT_CMD_SRV_REQ_PROCEED:
    {
      CMDBG_MEM_ALLOC(srv_req_proceed_cmd_dbg_buf_msg, cmregprx_srv_req_response_s_type)

      srv_req_proceed_cmd_dbg_buf_msg->srv_req_proceed = cmd_ptr->param.srv_req_proceed_cmd.srv_req_proceed;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)srv_req_proceed_cmd_dbg_buf_msg);
      break;
    }
    case PROT_CMD_UE_MODE_SWITCH:
    {
      CMDBG_MEM_ALLOC(ue_mode_switch_cmd_dbg_buf_msg, cmregprx_ue_mode_switch_s_type)

      ue_mode_switch_cmd_dbg_buf_msg->trans_id = cmd_ptr->param.ue_mode_switch_cmd.trans_id;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)ue_mode_switch_cmd_dbg_buf_msg);
      break;
    }
    case PROT_CMD_MULTIMODE_SUBS_CHGD:
    {
      CMDBG_MEM_ALLOC(multimode_subs_chgd_cmd_dbg_buf_msg, prot_subs_cap_chgd_s_type)

      memscpy( multimode_subs_chgd_cmd_dbg_buf_msg,sizeof(prot_subs_cap_chgd_s_type),
               &(cmd_ptr->param.multimode_subs_chgd_cmd), sizeof(prot_subs_cap_chgd_s_type));

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE, ((uint16)cmd_ptr->name), cmd_ptr->mm_id, (void *)multimode_subs_chgd_cmd_dbg_buf_msg);
      break;
    }

    default:
      CMREGPRX_MSG_LOW_1("DBG_BUFF: Ignored MMOC Cmd %d", cmd_ptr->name);
  }
}

/*===========================================================================
FUNCTION cmregprx_dbg_add_rpt_to_buffer

DESCRIPTION
  Add CM report to Debug Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_rpt_to_buffer
(
  const cm_rpt_type           *cm_rpt_ptr
)
{
  cmregprx_stack_info_s_type *current_stack_info_ptr = NULL;
  cmregprx_info_s_type       *cmregprx_info_ptr = cmregprx_get_info_ptr();

  current_stack_info_ptr = cmregprx_map_as_id_to_stack_info(cmregprx_info_ptr,
                           cmregprx_dbg_buffer.asubs_id_for_current_report,
                           cmregprx_dbg_buffer.stack_id_for_current_report,
                           cm_rpt_ptr->hdr.cmd);

  if (current_stack_info_ptr == NULL)
  {
    CMREGPRX_MSG_HIGH_1("ERROR: current_stack_info_ptr NULL PTR, sub %d",
                        cmregprx_dbg_buffer.asubs_id_for_current_report);
    return;
  }

  switch(cm_rpt_ptr->hdr.cmd)
  {
    case CM_SERVICE_CNF:
    {
      CMREGPRX_DBG_MEM_ALLOC(service_cnf_dbg_buf_msg, cmregprx_dbg_cm_service_cnf_s_type)

      service_cnf_dbg_buf_msg->transaction_id   = cm_rpt_ptr->cmd.service_cnf.transaction_id;
      service_cnf_dbg_buf_msg->service_status   = cm_rpt_ptr->cmd.service_cnf.service_state.service_status;
      service_cnf_dbg_buf_msg->active_rat       = cm_rpt_ptr->cmd.service_cnf.service_state.active_rat;
      service_cnf_dbg_buf_msg->acq_status       = cm_rpt_ptr->cmd.service_cnf.service_state.acq_status;
      service_cnf_dbg_buf_msg->scan_status      = cm_rpt_ptr->cmd.service_cnf.scan_status;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)service_cnf_dbg_buf_msg);
      break;
    }

    case CM_SERVICE_IND:
    {
      CMREGPRX_DBG_MEM_ALLOC(service_ind_dbg_buf_msg, cmregprx_dbg_cm_service_ind_s_type)

      service_ind_dbg_buf_msg->service_status   = cm_rpt_ptr->cmd.service_ind.service_state.service_status;
      service_ind_dbg_buf_msg->active_rat       = cm_rpt_ptr->cmd.service_ind.service_state.active_rat;
      service_ind_dbg_buf_msg->acq_status       = cm_rpt_ptr->cmd.service_ind.service_state.acq_status;
      service_ind_dbg_buf_msg->no_service_cause = cm_rpt_ptr->cmd.service_ind.service_state.no_service_cause;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)service_ind_dbg_buf_msg);
      break;
    }

    case CM_NETWORK_LIST_CNF:
    {
      CMREGPRX_DBG_MEM_ALLOC(net_list_cnf_dbg_buf_msg, cmregprx_dbg_cm_network_list_cnf_s_type)

      net_list_cnf_dbg_buf_msg->transaction_id   = cm_rpt_ptr->cmd.net_list_cnf.transaction_id;
      net_list_cnf_dbg_buf_msg->length           = cm_rpt_ptr->cmd.net_list_cnf.found_plmn_list.plmn_list.length;
      net_list_cnf_dbg_buf_msg->rplmn            = cm_rpt_ptr->cmd.net_list_cnf.rplmn;
      net_list_cnf_dbg_buf_msg->status           = cm_rpt_ptr->cmd.net_list_cnf.status;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)net_list_cnf_dbg_buf_msg);
      break;
    }

    case CM_SIM_AVAILABLE_CNF:
    case CM_SIM_NOT_AVAILABLE_CNF:
    case CM_STOP_MODE_CNF:
    {
      CMREGPRX_DBG_MEM_ALLOC(generic_cnf_dbg_buf_msg, cmregprx_dbg_generic_cnf_s_type)
      reg_cm_transaction_id_type transaction_id = 0;

      if(cm_rpt_ptr->hdr.cmd == CM_SIM_AVAILABLE_CNF)
        transaction_id = cm_rpt_ptr->cmd.sim_available_cnf.transaction_id;
      else if(cm_rpt_ptr->hdr.cmd == CM_SIM_NOT_AVAILABLE_CNF)
        transaction_id = cm_rpt_ptr->cmd.sim_not_available_cnf.transaction_id;
      else if(cm_rpt_ptr->hdr.cmd == CM_STOP_MODE_CNF)
        transaction_id = cm_rpt_ptr->cmd.stop_mode_cnf.transaction_id;

      generic_cnf_dbg_buf_msg->transaction_id = transaction_id;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)generic_cnf_dbg_buf_msg);
      break;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_PH_STATUS_CHGD_CNF:
    {
      CMREGPRX_DBG_MEM_ALLOC(ph_status_chgd_cnf_dbg_buf_msg, cmregprx_dbg_cm_ph_status_chgd_cnf_s_type)

      ph_status_chgd_cnf_dbg_buf_msg->transaction_id   = cm_rpt_ptr->cmd.ph_status_chgd_cnf.transaction_id;
      ph_status_chgd_cnf_dbg_buf_msg->ph_status_chgd_status   = cm_rpt_ptr->cmd.ph_status_chgd_cnf.ph_status_chgd_status;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)ph_status_chgd_cnf_dbg_buf_msg);
      break;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_CAMPED_IND:
    {
      CMREGPRX_DBG_MEM_ALLOC(camped_ind_dbg_buf_msg, cmregprx_dbg_cm_camped_ind_s_type)

      camped_ind_dbg_buf_msg->plmn     = cm_rpt_ptr->cmd.camped_ind.plmn;
      camped_ind_dbg_buf_msg->active_rat  = cm_rpt_ptr->cmd.camped_ind.active_rat;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)camped_ind_dbg_buf_msg);
      break;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    case CM_MM_PS_DATA_SUSPEND_IND:
    case CM_STACK_DEACT_IND:
    case CM_PDP_GRACEFUL_ABORT_CNF:

#ifdef FEATURE_MMODE_DUAL_SIM
    case CM_DS_STAT_CHGD_CNF:
    case CM_MULTIMODE_SUBS_CHGD_CNF:
#endif

#if defined(FEATURE_LTE) || defined(FEATURE_WRLF_SYSTEM_SEL)
    case CM_SUITABLE_SEARCH_END_IND:
#endif
    {
      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)NULL);
      break;
    }

    case CM_SIM_STATE_UPDATE_IND:
    {
      CMREGPRX_DBG_MEM_ALLOC(sim_state_update_ind_dbg_buf_msg, cmregprx_dbg_cm_sim_state_update_ind_s_type)

      sim_state_update_ind_dbg_buf_msg->sim_state   = cm_rpt_ptr->cmd.sim_state_update_ind.sim_state;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)sim_state_update_ind_dbg_buf_msg);
      break;
    }

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if defined(FEATURE_WRLF_SYSTEM_SEL)
    case CM_DISABLE_BPLMN_IND:
    {
      CMREGPRX_DBG_MEM_ALLOC(disable_bplmn_ind_dbg_buf_msg, cmregprx_dbg_cm_disable_bplmn_ind_s_type)

      disable_bplmn_ind_dbg_buf_msg->status   = cm_rpt_ptr->cmd.disable_bplmn_ind.status;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)disable_bplmn_ind_dbg_buf_msg);
      break;
    }
#endif /* FEATURE_WRLF_SYSTEM_SEL */

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    case CM_BEARER_CTXT_TRANSFER_IND:
    {
#if defined(FEATURE_CM_LTE) && defined(FEATURE_CM_GWL_CALL_OBJECT_TRANSFER)
      CMREGPRX_DBG_MEM_ALLOC(bearer_ctxt_transfer_ind_dbg_buf_msg, cmregprx_dbg_cm_bearer_ctxt_transfer_ind_s_type)

      bearer_ctxt_transfer_ind_dbg_buf_msg->rat   = cm_rpt_ptr->cmd.bearer_ctxt_transfer_ind.rat;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)bearer_ctxt_transfer_ind_dbg_buf_msg);
#endif
    }
    break;

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    /* Only expected when feature_mode = sglte */
    case CM_SERVICE_STAT_IND:
    {
      CMREGPRX_DBG_MEM_ALLOC(service_stat_ind_dbg_buf_msg, cmregprx_dbg_cm_service_ind_s_type)

      service_stat_ind_dbg_buf_msg->service_status   = cm_rpt_ptr->cmd.service_stat_ind.service_state.service_status;
      service_stat_ind_dbg_buf_msg->active_rat      = cm_rpt_ptr->cmd.service_stat_ind.service_state.active_rat;
      service_stat_ind_dbg_buf_msg->acq_status      = cm_rpt_ptr->cmd.service_stat_ind.service_state.acq_status;
      service_stat_ind_dbg_buf_msg->no_service_cause = cm_rpt_ptr->cmd.service_stat_ind.service_state.no_service_cause;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)service_stat_ind_dbg_buf_msg);
      break;
    }
    case CM_CSG_NW_SEL_MODE_UPDATE_IND:
    {

      CMREGPRX_DBG_MEM_ALLOC(csg_nw_sel_mode_update_ind_dbg_buf_msg, cmregprx_dbg_cm_csg_nw_sel_mode_update_ind_s_type)

      csg_nw_sel_mode_update_ind_dbg_buf_msg->nw_sel_mode   = cm_rpt_ptr->cmd.csg_nw_sel_mode_update_ind.nw_sel_mode;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_RPT_TYPE, ((uint16)cm_rpt_ptr->hdr.cmd), current_stack_info_ptr->mm_id, (void *)csg_nw_sel_mode_update_ind_dbg_buf_msg);

      break;
    }

    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    default:
      CMREGPRX_MSG_LOW_1("DBG_BUFF: Ignored RPT %d", cm_rpt_ptr->hdr.cmd);

      /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  }
  cmregprx_dbg_set_stack_id(SYS_MODEM_AS_ID_MAX, SYS_MODEM_STACK_ID_MAX );
}

/*===========================================================================
FUNCTION cmregprx_dbg_add_msg_rtr_to_buffer

DESCRIPTION
    Add MSG RTR command to Debug Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_msg_rtr_to_buffer
(
  const cm_msgr_cmd_s_type   *rpt_ptr,
  sys_modem_as_id_e_type      asubs_id
)
{
  /*As of now, only 1 Msg Rtr message is being processed by CM REG PRX.
   Also, since it's infrequent, not adding the check for same message*/

#ifdef FEATURE_MMODE_LTE_RESEL
  if (CM_MSGR_LTE_IRAT_RESEL(rpt_ptr->cmd.lte_sys.lte_actd_ind.msg_hdr.id))
  {
    CMREGPRX_DBG_MEM_ALLOC(lte_actd_ind_dbg_buf_msg, cmregprx_dbg_lte_rrc_to_CM_act_ind_s_type)
    mm_sub_stk_id_s_type mm_id;
    mm_id.asubs_id = asubs_id;
    mm_id.stk_id = cmph_determine_stk_per_rat(mm_id.asubs_id, SYS_SYS_MODE_LTE);

    lte_actd_ind_dbg_buf_msg->act_reason = rpt_ptr->cmd.lte_sys.lte_actd_ind.act_reason;
    lte_actd_ind_dbg_buf_msg->src_rat = rpt_ptr->cmd.lte_sys.lte_actd_ind.src_rat;

    cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_MSG_RTR_TYPE, ((uint16)CMREGPRX_BUFF_MSG_RTR_LTE_RRC_TO_CM_ACTIVATION_IND),
                               mm_id, (void *)lte_actd_ind_dbg_buf_msg);
  }
#endif

}

/*===========================================================================
FUNCTION cmregprx_dbg_add_nas_cmd_to_buffer

DESCRIPTION
  Add NAS command to Debug Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_nas_cmd_to_buffer
(
  cm_to_reg_cmd_type    msg_type,
  mm_sub_stk_id_s_type  mm_id,
  const void           *msg_payload
)
{
  switch (msg_type)
  {
    case CM_SERVICE_REQ:
    {
      cm_service_req_s_type *cm_service_req = (cm_service_req_s_type *)msg_payload;
      CMREGPRX_DBG_MEM_ALLOC(cm_service_req_dbg_buf_msg, cmregprx_dbg_cm_service_req_s_type)

      cm_service_req_dbg_buf_msg->network_selection_mode = cm_service_req->network_selection_mode;
      cm_service_req_dbg_buf_msg->mode_pref              = cm_service_req->mode_pref;
      cm_service_req_dbg_buf_msg->req_service_domain     = cm_service_req->req_service_domain;
      cm_service_req_dbg_buf_msg->scan_scope             = cm_service_req->scan_scope;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_TO_NAS_TYPE, ((uint16)msg_type), mm_id, (void *)cm_service_req_dbg_buf_msg);
      break;
    }
    case CM_NETWORK_LIST_REQ:
    {
      cm_network_list_req_s_type *cm_network_list_req = (cm_network_list_req_s_type *)msg_payload;
      CMREGPRX_DBG_MEM_ALLOC(cm_network_list_req_dbg_buf_msg, cmregprx_dbg_cm_network_list_req_s_type)

      cm_network_list_req_dbg_buf_msg->mode_pref           =  cm_network_list_req->mode_pref;
      cm_network_list_req_dbg_buf_msg->req_service_domain  =  cm_network_list_req->req_service_domain;
      cm_network_list_req_dbg_buf_msg->list_search_type    =  cm_network_list_req->list_search_type;
      cm_network_list_req_dbg_buf_msg->scan_scope          =  cm_network_list_req->scan_scope;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_TO_NAS_TYPE, ((uint16)msg_type), mm_id, (void *)cm_network_list_req_dbg_buf_msg);
      break;
    }
    case CM_STOP_MODE_REQ:
    {
      cm_stop_mode_req_s_type *cm_stop_mode_req = (cm_stop_mode_req_s_type *)msg_payload;
      CMREGPRX_DBG_MEM_ALLOC(cm_stop_mode_req_dbg_buf_msg, cmregprx_dbg_cm_stop_mode_req_s_type)

      cm_stop_mode_req_dbg_buf_msg->transaction_id   =  cm_stop_mode_req->transaction_id;
      cm_stop_mode_req_dbg_buf_msg->stop_mode_reason =  cm_stop_mode_req->stop_mode_reason;

      cmregprx_dbg_add_new_entry(CMREGPRX_BUFF_CMD_TO_NAS_TYPE, ((uint16)msg_type), mm_id, (void *)cm_stop_mode_req_dbg_buf_msg);
      break;
    }

    default:
      CMREGPRX_MSG_LOW_1("DBG_BUFF: Ignored NAS Cmd %d", msg_type);
  }


}

/*===========================================================================
FUNCTION cmregprx_dbg_create_new_entry

DESCRIPTION
  This function creates new entry in CM REGPRX Debug Buffer and initializes this entry

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_create_new_entry(void)
{
  uint8 buf_idx, sub_idx = 0, stk_idx = 0;

  cmregprx_dbg_buffer.dbg_buf_idx++;

  if(cmregprx_dbg_buffer.dbg_buf_idx >= CMREGPRX_DEBUG_BUFFER_SIZE)
  {
    cmregprx_dbg_buffer.dbg_buf_idx = 0;
  }

  buf_idx = cmregprx_dbg_buffer.dbg_buf_idx;

  cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].timestamp   =  time_get_uptime_secs();
  cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].idx         =  cmdbg_get_unique_idx();

  /* Storing state and substate of each stack to current debug buffer entry */
  for(sub_idx = 0 ; sub_idx < MAX_SIMS; sub_idx ++)
  {
    for (stk_idx = 0; stk_idx < CMREGPRX_NUM_MAX_STACKS; stk_idx ++)
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = (sys_modem_as_id_e_type)sub_idx;
      mm_id.stk_id = cmregprx_map_to_mm_stk(stk_idx);

      cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].cmregprx_state[sub_idx][stk_idx] = cmregprx_get_state(mm_id);
      cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].cmregprx_substate[sub_idx][stk_idx] = cmregprx_get_substate(mm_id);
    }
  }

  cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].mm_id.asubs_id = SYS_MODEM_AS_ID_NONE;
  cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].mm_id.stk_id = MM_STACK_NONE;
  cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].msg_type = CMREGPRX_BUFF_TYPE_NONE;
  cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].msg_name.rpt = 0;

  if(cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].msg_payload)
  {
    cm_mem_free(cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].msg_payload);
    cmregprx_dbg_buffer.dbg_buf_msg[buf_idx].msg_payload = NULL;
  }

}

/*===========================================================================
FUNCTION cmregprx_dbg_add_new_entry

DESCRIPTION
  This function adds new Debug buffer Entry in CM REGPRX Buffer

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none
===========================================================================*/
void cmregprx_dbg_add_new_entry
(
  cmregprx_buffer_type_e_type msg_type, /* Type of Buffer to be used */
  uint16                      msg_name, /* Specific cmd/report inside buffer mentioned above */
  mm_sub_stk_id_s_type        mm_id,  /* Sub/stack on which this new dbg msg has come/gone*/
  void                       *msg_payload /* Actual payload */
)
{
  cmregprx_dbg_create_new_entry();

  cmregprx_dbg_buffer.dbg_buf_msg[cmregprx_dbg_buffer.dbg_buf_idx].msg_type = msg_type;
  cmregprx_dbg_buffer.dbg_buf_msg[cmregprx_dbg_buffer.dbg_buf_idx].msg_payload = msg_payload;
  cmregprx_dbg_buffer.dbg_buf_msg[cmregprx_dbg_buffer.dbg_buf_idx].mm_id = mm_id;

  switch(msg_type)
  {
    case CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE:
      cmregprx_dbg_buffer.dbg_buf_msg[cmregprx_dbg_buffer.dbg_buf_idx].msg_name.cmd_from_mmoc   = (prot_cmd_e_type)msg_name;
      break;
    case CMREGPRX_BUFF_RPT_TYPE:
      cmregprx_dbg_buffer.dbg_buf_msg[cmregprx_dbg_buffer.dbg_buf_idx].msg_name.rpt  = (cm_name_type)msg_name;
      break;
    case CMREGPRX_BUFF_MSG_RTR_TYPE:
      cmregprx_dbg_buffer.dbg_buf_msg[cmregprx_dbg_buffer.dbg_buf_idx].msg_name.msgr_rpt  = (cmregprx_dbg_buff_msg_rtr_e_type)msg_name;
      break;
    case CMREGPRX_BUFF_CMD_TO_NAS_TYPE:
      cmregprx_dbg_buffer.dbg_buf_msg[cmregprx_dbg_buffer.dbg_buf_idx].msg_name.cmd_to_nas  = (cm_to_reg_cmd_type)msg_name;
      break;

    case CMREGPRX_BUFF_TYPE_NONE:
    case CMREGPRX_BUFF_TYPE_MAX:
    default:
      CMREGPRX_MSG_LOW_2("DBG_BUFF: Invalid Msg Type %d, Msg Name %d received", msg_type, msg_name);
  }
}


/*===========================================================================
FUNCTION cmregprx_dbg_if_same_mmoc_cmd

DESCRIPTION
  This function returns a boolean indicating whether the new MMOC command is identical to the most recently
  filled Debug message in the CMREGPRX debug buffer.

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.
===========================================================================*/
boolean cmregprx_dbg_if_same_mmoc_cmd
(
  const cmregprx_cmd_msg_s_type   *cmd_ptr
)
{
  uint8 idx = (cmregprx_dbg_buffer.dbg_buf_idx == 0xFF) ? (CMREGPRX_DEBUG_BUFFER_SIZE - 1) : cmregprx_dbg_buffer.dbg_buf_idx;
  cmregprx_dbg_buffer_message_s_type *prev_dbg_msg;
  uint8 sub_idx = 0, stk_idx = 0;

  if(idx >= CMREGPRX_DEBUG_BUFFER_SIZE)
  {
    CMREGPRX_MSG_HIGH_1("cmregprx_dbg_if_same_mmoc_cmd: invalid dbg_buf_idx %d", idx);
    return FALSE;
  }

  prev_dbg_msg = &cmregprx_dbg_buffer.dbg_buf_msg[idx];

  // If required, we can check for payload of MMOC commands selectively to compare whether they are same.
  if((prev_dbg_msg->msg_type != CMREGPRX_BUFF_CMD_FROM_MMOC_TYPE)
      ||(prev_dbg_msg->msg_name.cmd_from_mmoc != cmd_ptr->name)
      ||(prev_dbg_msg->mm_id.asubs_id != cmd_ptr->mm_id.asubs_id ||
         prev_dbg_msg->mm_id.stk_id!= cmd_ptr->mm_id.stk_id))
  {
    return FALSE;
  }

  for(sub_idx = 0 ; sub_idx < MAX_SIMS; sub_idx ++)
  {
    for (stk_idx = 0; stk_idx < CMREGPRX_NUM_MAX_STACKS; stk_idx ++)
    {
      mm_sub_stk_id_s_type mm_id;
      mm_id.asubs_id = (sys_modem_as_id_e_type)sub_idx;
      mm_id.stk_id = cmregprx_map_to_mm_stk(stk_idx);

      if(prev_dbg_msg->cmregprx_state[sub_idx][stk_idx] != cmregprx_get_state(mm_id)
          || prev_dbg_msg->cmregprx_substate[sub_idx][stk_idx] != cmregprx_get_substate(mm_id))
      {
        return FALSE;
      }
    }
  }

  return TRUE;
}




/*===========================================================================

FUNCTION cmregprx_dbg_buf_print_info

DESCRIPTION
  This function should be invoked by the MMOC when sanity timer expired to
  print value of cmregprx_debug_buffer

DEPENDENCIES

RETURN VALUE


SIDE EFFECTS
  None.
===========================================================================*/
EXTERN  void                        cmregprx_dbg_buf_print_info(
  void
)
{

#ifdef MMODE_ADDITIONAL_DEBUG_INFO_EXT

  int i = 0;
  uint8 sub_idx = 0, stk_idx = 0;

  CMREGPRX_MSG_HIGH_0("---------CMREGPRX DEBUG BUFFER---------");
  CMREGPRX_MSG_HIGH_1("Current_index : %d", cmregprx_dbg_buffer.dbg_buf_idx);

  for(i = 0 ; i < CMREGPRX_DEBUG_BUFFER_SIZE ; i++ )
  {
    if(cmregprx_dbg_buffer.dbg_buf_msg[i].msg_type != CMREGPRX_BUFF_TYPE_NONE)
    {
      if(cmregprx_dbg_buffer.dbg_buf_msg[i].is_cmd == TRUE)
      {
        CMREGPRX_MSG_HIGH_4("Command[%d]->%d | sub = %d stk = %d",
                            i,
                            cmregprx_dbg_buffer.dbg_buf_msg[i].msg_type,
                            cmregprx_dbg_buffer.dbg_buf_msg[i].mm_id.asubs_id,
                            cmregprx_dbg_buffer.dbg_buf_msg[i].mm_id.stk_id);
      }
      else
      {
        CMREGPRX_MSG_HIGH_4("Report[%d]->%d | sub = %d stk = %d",
                            i,
                            cmregprx_dbg_buffer.dbg_buf_msg[i].msg_type,
                            cmregprx_dbg_buffer.dbg_buf_msg[i].mm_id.asubs_id,
                            cmregprx_dbg_buffer.dbg_buf_msg[i].mm_id.stk_id);
      }

      for(sub_idx = 0 ; sub_idx < MAX_SIMS; sub_idx ++)
      {
        for (stk_idx = 0; stk_idx < CMREGPRX_NUM_MAX_STACKS; stk_idx ++)
        {
          mm_sub_stk_id_s_type mm_id;
          mm_id.asubs_id = (sys_modem_as_id_e_type)sub_idx;
          mm_id.stk_id = cmregprx_map_to_mm_stk(stk_idx);

          CMREGPRX_MSG_HIGH_4("   PROT sub[%d] stk[%d]:: State:%d  Substate:%d",
                              sub_idx, stk_idx
                              cmregprx_dbg_buffer.dbg_buf_msg[i].cmregprx_state[sub_idx][stk_idx],
                              cmregprx_dbg_buffer.dbg_buf_msg[i].sub_state[sub_idx][stk_idx]);
        }
      }
    }
  }
  CMREGPRX_MSG_HIGH_0("---------CMREGPRX DEBUG BUFFER---------");

#endif /* MMODE_ADDITIONAL_DEBUG_INFO_EXT */

}/*cmregprx_dbg_buf_print_info()*/


#endif
#endif

/*lint -restore */
