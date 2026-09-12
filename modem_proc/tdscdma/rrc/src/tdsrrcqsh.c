/*!
  @file
  tdsrrcqsh.c

  @brief
  TDSRRC QSH (Qualcomm Sherlock Holmes) debug support

*/

/*===========================================================================

  Copyright (c) 2008 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcqsh.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/12/15   cdf     Initial version

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "tdsrrcqsh.h"
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcccm.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcsibproci.h"
#include "tdsrrcrcei.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcmcm.h"
#include "sys.h"

#ifdef FEATURE_QSH_EVENT_METRIC

/*===========================================================================

                         LOCAL VARIABLES

===========================================================================*/
tdsrrcqsh_global_struct_type tdsrrcqsh_global;

qsh_ext_metric_cfg_s  tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_MAX];

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/


/*===========================================================================

                                FUNCTIONS

===========================================================================*/
/*===========================================================================

FUNCTION TDSRRC_QSH_TC_METRICS_INIT

DESCRIPTION
  Initializing the metrics for test support
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrc_qsh_tc_metrics_init(void)
{

   qsh_client_metric_cfg_init(tdsrrcqsh_metric_cfg_arr, TDSRRC_QSH_METRIC_MAX);
   
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RESEL_CEL_SEL].id = TDSRRC_QSH_METRIC_RESEL_CEL_SEL;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RESEL_CEL_SEL].sampling_period_ms = 1000;   
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RESEL_CEL_SEL].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RESEL_CEL_SEL].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_resel_cel_sel_s);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RESEL_CEL_SEL].fifo.element_count_total = 10;
   
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RLF_OOS].id = TDSRRC_QSH_METRIC_RLF_OOS;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RLF_OOS].sampling_period_ms = 1000;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RLF_OOS].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RLF_OOS].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_rlf_oos_s);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RLF_OOS].fifo.element_count_total = 10;

   
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_HO].id = TDSRRC_QSH_METRIC_HO;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_HO].sampling_period_ms = 1000;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_HO].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_HO].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_ho_s);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_HO].fifo.element_count_total = 10;

   
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].id = TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].sampling_period_ms = 1000;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_tmr_and_const_s);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].fifo.element_count_total = 3;

   
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_SERVING].id = TDSRRC_QSH_METRIC_SERVING;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_SERVING].sampling_period_ms = 1000;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_SERVING].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_SERVING].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_serving_cell_s);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_SERVING].fifo.element_count_total = 10;

   
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RRC_STATE].id = TDSRRC_QSH_METRIC_RRC_STATE;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RRC_STATE].sampling_period_ms = 1000;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RRC_STATE].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RRC_STATE].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_rrc_state_s);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_RRC_STATE].fifo.element_count_total = 100;

   
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_OTA].id = TDSRRC_QSH_METRIC_OTA;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_OTA].sampling_period_ms = 1000;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_OTA].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_OTA].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_ota_s);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_OTA].fifo.element_count_total = 15;

   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CONN_END_INFO].id = TDSRRC_QSH_METRIC_CONN_END_INFO;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CONN_END_INFO].sampling_period_ms = 0;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CONN_END_INFO].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CONN_END_INFO].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_conn_end_info_s_type);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CONN_END_INFO].fifo.element_count_total = 10;

   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].id = TDSRRC_QSH_METRIC_MULTI_RAB_STATUS;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].sampling_period_ms = 0;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_multi_rab_status_s_type);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].fifo.element_count_total = 10;

   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].id = TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].sampling_period_ms = 0;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].subs_id = SYS_MODEM_AS_ID_1;
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].fifo.element_size_bytes = sizeof(tdsrrc_qsh_metric_cell_update_cause_s_type);
   tdsrrcqsh_metric_cfg_arr[TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].fifo.element_count_total = 5;
}


/*===========================================================================

FUNCTION        tdsrrcqsh_init

DESCRIPTION     TDSRRC QSH initialization. It will register a callback function
                to QSH module

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_init(void)
{
  /* Register TDSRRC QSH callback */
  qsh_client_reg_s client_reg;

  memset(&tdsrrcqsh_global, 0x00, sizeof(tdsrrcqsh_global_struct_type));

  qsh_client_reg_init(&client_reg);

  /*Adding some support for test.*/
  tdsrrc_qsh_tc_metrics_init();
  client_reg.metric_info.metric_cfg_arr_ptr = &tdsrrcqsh_metric_cfg_arr[0];
  client_reg.metric_info.metric_cfg_count = TDSRRC_QSH_METRIC_MAX;
        
  client_reg.client = QSH_CLT_TRRC;
  client_reg.client_cb_ptr = tdsrrcqsh_client_cb;
  client_reg.cb_action_support_mask = (QSH_ACTION_METRIC_TIMER_EXPIRY|QSH_ACTION_METRIC_CFG);
  client_reg.major_ver = TDSRRC_QSH_MAJOR_VER;
  client_reg.minor_ver = TDSRRC_QSH_MINOR_VER;
  
  qsh_client_reg(&client_reg);
}


/*===========================================================================

FUNCTION        tdsrrcqsh_commit_data

DESCRIPTION     This function commits data to QSH and update the next address.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_commit_data(qsh_metric_id_t metric_id, qsh_client_metric_log_reason_e log_reason)
{
  uint8 *next_addr = NULL;
  qsh_client_metric_log_done_s metric_log_done;
  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();
  
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
	return;
  }
  
  qsh_client_metric_log_done_init(&metric_log_done);

  metric_log_done.client = QSH_CLT_TRRC;
  metric_log_done.metric_id = metric_id;
  metric_log_done.metric_context_id = tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].metric_context_id;
  metric_log_done.log_reason = log_reason;

  next_addr = qsh_client_metric_log_done(&metric_log_done);

  if(next_addr == NULL)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR, "TRRC_QSH: Invalid next addr after qsh_client_metric_log_done,metric_id %d",metric_id);
  }
  else
  {
    tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].data_ptr = next_addr;
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH, "TRRC_QSH: metric_id %d, metric_context_id %d, log_reason %d", 
    metric_id, tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].metric_context_id, log_reason);

  switch(metric_id)
  {
    case TDSRRC_QSH_METRIC_RESEL_CEL_SEL:
     TDSRRC_MSG5(MSG_LEGACY_MED, "TRRC_QSH: METRIC_RESEL_CEL_SEL, num_t_to_l_resel %d, num_t_to_t_resel %d, num_t_to_g_resel %d, num_resel_fail %d, num_cel_sel_fail %d", 
       tdsrrcqsh_global.resel_cel_sel_metric.num_t_to_l_resel, 
       tdsrrcqsh_global.resel_cel_sel_metric.num_t_to_t_resel,
       tdsrrcqsh_global.resel_cel_sel_metric.num_t_to_g_resel,
       tdsrrcqsh_global.resel_cel_sel_metric.num_resel_fail,
       tdsrrcqsh_global.resel_cel_sel_metric.num_cel_sel_fail);
      break;

    case TDSRRC_QSH_METRIC_RLF_OOS:
     TDSRRC_MSG3(MSG_LEGACY_MED, "TRRC_QSH: METRIC_RLF_OOS, num_rlf %d, num_oos %d, num_con_reject %d", 
       tdsrrcqsh_global.rlf_oos_metric.num_rlf, 
       tdsrrcqsh_global.rlf_oos_metric.num_oos,
       tdsrrcqsh_global.rlf_oos_metric.num_con_reject);
      break;

    case TDSRRC_QSH_METRIC_HO:
     TDSRRC_MSG4(MSG_LEGACY_MED, "TRRC_QSH: METRIC_HO, num_t_to_l_ho %d, num_t_to_g_ho %d, num_t_to_t_ho %d, num_inter_rat_ho_fail %d", 
       tdsrrcqsh_global.ho_metric.num_t_to_l_ho, 
       tdsrrcqsh_global.ho_metric.num_t_to_g_ho,
       tdsrrcqsh_global.ho_metric.num_t_to_t_ho,
       tdsrrcqsh_global.ho_metric.num_inter_rat_ho_fail);
      break;

    case TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS:
     TDSRRC_MSG3(MSG_LEGACY_MED, "TRRC_QSH: METRIC_TIMERS_AND_CONSTANTS, T313 %d, T300 %d, T302 %d", 
       tdsrrcqsh_global.tmr_and_const.t313, 
       tdsrrcqsh_global.tmr_and_const.t300,
       tdsrrcqsh_global.tmr_and_const.t302);
     TDSRRC_MSG4(MSG_LEGACY_MED, "TRRC_QSH: METRIC_TIMERS_AND_CONSTANTS, N313 %d, N315 %d, N300 %d, N302 %d", 
       tdsrrcqsh_global.tmr_and_const.n313, 
       tdsrrcqsh_global.tmr_and_const.n315,
       tdsrrcqsh_global.tmr_and_const.n300,
       tdsrrcqsh_global.tmr_and_const.n302);
      break;

    case TDSRRC_QSH_METRIC_SERVING:
     TDSRRC_MSG4(MSG_LEGACY_MED, "TRRC_QSH: METRIC_SERVING, cell_id %d, drx_cycle %d, uarfcn %d, cpid %d", 
       tdsrrcqsh_global.serving_cell_metric.cell_id, 
       tdsrrcqsh_global.serving_cell_metric.drx_cycle,
       tdsrrcqsh_global.serving_cell_metric.uarfcn,
       tdsrrcqsh_global.serving_cell_metric.cpid);
     TDSRRC_MSG3(MSG_LEGACY_MED, "TRRC_QSH: METRIC_SERVING, rrc_state %d, dpch_ul_tdm_status %d, dpch_dl_tdm_status %d", 
       tdsrrcqsh_global.serving_cell_metric.rrc_state, 
       tdsrrcqsh_global.serving_cell_metric.dpch_ul_tdm_status,
       tdsrrcqsh_global.serving_cell_metric.dpch_dl_tdm_status);
      break;

    case TDSRRC_QSH_METRIC_RRC_STATE:
     TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: METRIC_RRC_STATE,rrc_state %d",tdsrrcqsh_global.rrc_state.rrc_state);
      break;

    case TDSRRC_QSH_METRIC_OTA:
     TDSRRC_MSG5(MSG_LEGACY_MED, "TRRC_QSH: METRIC_OTA, msg_id %d, param0 %d, param1 %d, param2 %d, param3 %d", 
       tdsrrcqsh_global.ota.msg_id,
       tdsrrcqsh_global.ota.param0,
       tdsrrcqsh_global.ota.param1,
       tdsrrcqsh_global.ota.param2,
       tdsrrcqsh_global.ota.param3);
      break;

    case TDSRRC_QSH_METRIC_CONN_END_INFO:  
      switch(tdsrrcqsh_global.conn_end_info.type)
        {
          case SYS_CALL_END_EVENT_HANDOVER_FAIL:
            TDSRRC_MSG2(MSG_LEGACY_MED, "TRRC_QSH: METRIC_CONN_END_INFO, conn_end_type %d, ho_fail_type %d", 
              tdsrrcqsh_global.conn_end_info.type,
              tdsrrcqsh_global.conn_end_info.cause.ho_fail_type
              );
            break;

          case SYS_CALL_END_EVENT_RLF_OR_WEAK_SIGNAL: 
            TDSRRC_MSG2(MSG_LEGACY_MED, "TRRC_QSH: METRIC_CONN_END_INFO, conn_end_type %d, rlf_weak_signal_type %d", 
              tdsrrcqsh_global.conn_end_info.type,
              tdsrrcqsh_global.conn_end_info.cause.rlf_weak_signal_type
              );
            break;

          case SYS_CALL_END_EVENT_RX_RRC_RELEASE:
            TDSRRC_MSG2(MSG_LEGACY_MED, "TRRC_QSH: METRIC_CONN_END_INFO, conn_end_type %d, conn_rel_cause %d", 
              tdsrrcqsh_global.conn_end_info.type,
              tdsrrcqsh_global.conn_end_info.cause.conn_rel_cause
              );
            break;
            
          default:
            TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: METRIC_CONN_END_INFO, wrong conn_end_type %d", 
              tdsrrcqsh_global.conn_end_info.type);
            break;
        }
      break;

    case TDSRRC_QSH_METRIC_MULTI_RAB_STATUS:
      TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: METRIC_RAB_STATUS, rab_status %d", 
                                  tdsrrcqsh_global.rab_status.rab_status);
      break;

    case TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE:
      TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: METRIC_CELL_UPDATE_CAUSE, cu_cause %d", 
                                  tdsrrcqsh_global.cell_update_cause.cell_update_cause);
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TRRC_QSH: Wrong metric_id %d",metric_id);
      break;

  }

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_client_cb

DESCRIPTION     Client cb from QSH.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_client_cb(qsh_client_cb_params_s *cb_params_p)
{
  if(cb_params_p)
  {
    switch(cb_params_p->action)
    {
      case QSH_ACTION_METRIC_CFG:
      case QSH_ACTION_METRIC_TIMER_EXPIRY:
        /* Post message to TRRC task */
        tdsrrcqsh_send_metric_cb_ind(cb_params_p);
        break;

      default:
        /* Not supported yet */
        TDSRRC_MSG1(MSG_LEGACY_ERROR, "TRRC_QSH: does not support this action currently,action: %d .",cb_params_p->action);
        break;
    }
  }
  return;
}


/*===========================================================================

FUNCTION        tdsrrcqsh_send_metric_cb_ind

DESCRIPTION     Post a message back to TDSRRC task from QSH callback context.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_send_metric_cb_ind
(
  qsh_client_cb_params_s *cb_params_p
)
{
  tdsrrc_cmd_type *int_cmd_ptr = NULL;

  int_cmd_ptr = tdsrrc_get_int_cmd_buf();

  if(int_cmd_ptr != NULL)
  {
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_QSH_CB_IND;
    int_cmd_ptr->cmd.qsh_cb_ind.cb_params = (*cb_params_p);

    TDSRRC_MSG0(MSG_LEGACY_MED,"TRRC_QSH: TDSRRC_QSH_CB_IND sent");
    tdsrrc_put_int_cmd(int_cmd_ptr);
  }

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_handle_metric_cb_ind

DESCRIPTION     Handle QSH callback indication.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_handle_metric_cb_ind
(
  tdsrrc_qsh_cb_ind_type *qsh_cb_ind_p
)
{
  qsh_client_action_done_s cb_done;

  switch(qsh_cb_ind_p->cb_params.action)
  {
    case QSH_ACTION_METRIC_CFG:
      tdsrrcqsh_update_metric_cfg(
        &(qsh_cb_ind_p->cb_params.action_params.metric_cfg));
      break;

    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      tdsrrcqsh_flush_periodic_metric(
        &(qsh_cb_ind_p->cb_params.action_params.metric_timer_expiry), 
        qsh_cb_ind_p->cb_params.context_id);
      break;

    default:
      break;
  }


  qsh_client_action_done_init(&cb_done);

  cb_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
  cb_done.cb_params_ptr = &(qsh_cb_ind_p->cb_params);
  
  qsh_client_action_done(&cb_done);

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_update_metric_cfg

DESCRIPTION     Update address to the buffer where metrics should be written to.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_update_metric_cfg
(
  qsh_client_metric_cfg_s           *qsh_metric_cfg_p
)
{
  uint8           *addr = NULL;
  qsh_metric_id_t metric_id;
  sys_modem_as_id_e_type sub_id, curr_sub_id;
  tdsrrc_state_e_type rrc_state;

  metric_id = qsh_metric_cfg_p->id;
  sub_id = qsh_metric_cfg_p->subs_id;

  if(metric_id >= TDSRRC_QSH_METRIC_MAX)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TRRC_QSH: invalid metric_id %d",metric_id);
    return;
  }

  if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"TRRC_QSH: invalid sub_id: %d, num_subs: %d",sub_id, TDSCDMA_NUM_SUBS);
    return;
  }

  if(qsh_metric_cfg_p->action == QSH_METRIC_ACTION_START)
  {
    addr = qsh_metric_cfg_p->start_addr;

    if(addr == NULL)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TRRC_QSH: invalid start address passed for metric_id %d",metric_id);
      return;
    }

    tdsrrcqsh_global.metric_cfg[sub_id][metric_id].data_ptr = addr;

    tdsrrcqsh_global.metric_cfg[sub_id][metric_id].size_bytes = qsh_metric_cfg_p->size_bytes;

    tdsrrcqsh_global.metric_cfg[sub_id][metric_id].started = TRUE;
  }
  else if(qsh_metric_cfg_p->action == QSH_METRIC_ACTION_STOP)
  {
    tdsrrcqsh_global.metric_cfg[sub_id][metric_id].data_ptr = NULL;

    tdsrrcqsh_global.metric_cfg[sub_id][metric_id].started = FALSE;
  }

  tdsrrcqsh_global.metric_cfg[sub_id][metric_id].metric_context_id = qsh_metric_cfg_p->metric_context_id;

  curr_sub_id = tdsrrc_get_as_id();

  if(sub_id == curr_sub_id)
  {
    tdsrrcqsh_initialize_metric_data_by_id(metric_id);
  }

  if(qsh_metric_cfg_p->action == QSH_METRIC_ACTION_START && sub_id == curr_sub_id)
  {
    /* update metric immediately after start */
    switch(metric_id)
    {
      case TDSRRC_QSH_METRIC_RRC_STATE:
        rrc_state = tdsrrc_get_state();
        tdsrrcqsh_rrc_state_update(rrc_state);
        break;

      case TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS:
        tdsrrcqsh_timer_and_constants_update();
        break;

      case TDSRRC_QSH_METRIC_SERVING:
        tdsrrcqsh_serving_cell_update();
        break;

      case TDSRRC_QSH_METRIC_MULTI_RAB_STATUS:
        tdsrrcqsh_rab_status_update();
        break;     

      default:
        break;
    }
  }
  
  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_flush_periodic_metric

DESCRIPTION     Flush periodic QSH metric and update new address.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_flush_periodic_metric
(
  qsh_client_metric_timer_expiry_s  *expiry_data_p,
  qsh_context_id_t                  ctx_id
)
{
  int i;
  qsh_metric_id_t metric_id;
  boolean commit_data = FALSE;
  sys_modem_as_id_e_type sub_id;
  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();
  
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
	return;
  }

  /* Do not report periodic metric in inactive mode*/
  if(TDSRRC_MODE_INACTIVE == tdsrrcmcm_get_rrc_mode())
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"TRRC_QSH: do not report periodic metric in inactive mode");
    return;
  }

  for(i = 0; i < expiry_data_p->metric_id_count; i++)
  {
    commit_data = FALSE;
    metric_id = expiry_data_p->params[i].metric_id;
    sub_id = expiry_data_p->params[i].subs_id;

    if(metric_id >= TDSRRC_QSH_METRIC_MAX)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TRRC_QSH: invalid metric_id %d",metric_id);
      continue;
    }

    if(sub_id != curr_sub_id)
    {
      TDSRRC_MSG2(MSG_LEGACY_MED,"TRRC_QSH: not for current sub, sub_id: %d, curr_sub_id: %d", sub_id, curr_sub_id);
      continue;
    }

    if((tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].started == FALSE)||
       (tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].data_ptr == NULL))
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"TRRC_QSH: Metric is not started by QSH for metric_id %d",metric_id);
      continue;
    }

    /*
     * Update metrics
     */
    switch(metric_id)
    {
      case TDSRRC_QSH_METRIC_RESEL_CEL_SEL:
        memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].data_ptr, tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].size_bytes,
                  (void *)&tdsrrcqsh_global.resel_cel_sel_metric, sizeof(tdsrrc_qsh_metric_resel_cel_sel_s));
        commit_data = TRUE;
        break;

      case TDSRRC_QSH_METRIC_RLF_OOS:
        memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].data_ptr, tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].size_bytes,
                  (void *)&tdsrrcqsh_global.rlf_oos_metric, sizeof(tdsrrc_qsh_metric_rlf_oos_s));
        commit_data = TRUE;
        break;

      case TDSRRC_QSH_METRIC_HO:
        memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].data_ptr, tdsrrcqsh_global.metric_cfg[curr_sub_id][metric_id].size_bytes,
                  (void *)&tdsrrcqsh_global.ho_metric, sizeof(tdsrrc_qsh_metric_ho_s));
        commit_data = TRUE;
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_MED,"TRRC_QSH: Unexpected periodic metric_id %d",metric_id);
        break;
    }

    if(commit_data)
    {
      /*
       * Commit and get next addr
       */
      tdsrrcqsh_commit_data(metric_id, QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY);
  
      tdsrrcqsh_initialize_metric_data_by_id(metric_id);
    }
  }

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_initialize_metric_data_by_id

DESCRIPTION     Reset metric data to initial state.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_initialize_metric_data_by_id
(
  qsh_metric_id_t  metric_id
)
{
  switch(metric_id)
  {
    case TDSRRC_QSH_METRIC_RESEL_CEL_SEL:
      memset(&tdsrrcqsh_global.resel_cel_sel_metric, 0x00, sizeof(tdsrrc_qsh_metric_resel_cel_sel_s));
      break;

    case TDSRRC_QSH_METRIC_RLF_OOS:
      memset(&tdsrrcqsh_global.rlf_oos_metric, 0x00, sizeof(tdsrrc_qsh_metric_rlf_oos_s));
      break;

    case TDSRRC_QSH_METRIC_HO:
      memset(&tdsrrcqsh_global.ho_metric, 0x00, sizeof(tdsrrc_qsh_metric_ho_s));
      break;

    case TDSRRC_QSH_METRIC_SERVING:
      memset(&tdsrrcqsh_global.serving_cell_metric, 0x00, sizeof(tdsrrc_qsh_metric_serving_cell_s));
      break;

    case TDSRRC_QSH_METRIC_RRC_STATE:
      memset(&tdsrrcqsh_global.rrc_state, 0x00, sizeof(tdsrrc_qsh_metric_rrc_state_s));
      break;

    case TDSRRC_QSH_METRIC_OTA:
      memset(&tdsrrcqsh_global.ota, 0x00, sizeof(tdsrrc_qsh_metric_ota_s));
      break;

    case TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS:
      memset(&tdsrrcqsh_global.tmr_and_const, 0x00, sizeof(tdsrrc_qsh_metric_tmr_and_const_s));
      break;

    case TDSRRC_QSH_METRIC_CONN_END_INFO:
      memset(&tdsrrcqsh_global.conn_end_info, 0x00, sizeof(tdsrrc_qsh_metric_conn_end_info_s_type));
      break;

    case TDSRRC_QSH_METRIC_MULTI_RAB_STATUS:
      memset(&tdsrrcqsh_global.rab_status, 0x00, sizeof(tdsrrc_qsh_metric_multi_rab_status_s_type));
      break;

    case TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE:
      memset(&tdsrrcqsh_global.cell_update_cause, 0x00, sizeof(tdsrrc_qsh_metric_cell_update_cause_s_type));
      break;

    default:
      break;
  }

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_metric_stats_update

DESCRIPTION     Update metric of given metric id.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_metric_stats_update(tdsrrcqsh_metric_enum_type metric_id)
{
  switch(metric_id)
  {
    case TDSRRC_QSH_T2L_RESEL_START:
        tdsrrcqsh_global.resel_cel_sel_metric.num_t_to_l_resel++;
        break;

    case TDSRRC_QSH_T2T_RESEL_START:
        tdsrrcqsh_global.resel_cel_sel_metric.num_t_to_t_resel++;
        break;

    case TDSRRC_QSH_T2G_RESEL_START:
        tdsrrcqsh_global.resel_cel_sel_metric.num_t_to_g_resel++;
        break;

    case TDSRRC_QSH_RESEL_FAILURE:
        tdsrrcqsh_global.resel_cel_sel_metric.num_resel_fail++;
        break;

    case TDSRRC_QSH_CEL_SEL_FAILURE:
        tdsrrcqsh_global.resel_cel_sel_metric.num_cel_sel_fail++;
        break;

    case TDSRRC_QSH_T2L_HO_START:
        tdsrrcqsh_global.ho_metric.num_t_to_l_ho++;
        break;

    case TDSRRC_QSH_T2G_HO_START:
        tdsrrcqsh_global.ho_metric.num_t_to_g_ho++;
        break;

    case TDSRRC_QSH_T2T_HO_START:
        tdsrrcqsh_global.ho_metric.num_t_to_t_ho++;
        break;

    case TDSRRC_QSH_IRAT_HO_FAILURE:
        tdsrrcqsh_global.ho_metric.num_inter_rat_ho_fail++;
        break;

    case TDSRRC_QSH_RLF:
        tdsrrcqsh_global.rlf_oos_metric.num_rlf++;
        break;

    case TDSRRC_QSH_OOS:
        tdsrrcqsh_global.rlf_oos_metric.num_oos++;
        break;

    case TDSRRC_QSH_CONN_REJ:
        tdsrrcqsh_global.rlf_oos_metric.num_con_reject++;
        break;

    default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"TRRC_QSH: Wrong metric %d",metric_id);
        break;
  }

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_timer_and_constants_update

DESCRIPTION     Update timer and constants and send event to QSH.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_timer_and_constants_update(void)
{
  tdsrrc_qsh_metric_tmr_and_const_s tmr_and_const_params;
  tdsrrc_T_300 t_300 = 0;
  tdsrrc_N_300 n_300 = 0;
  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();
  
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
    return;
  }

  if((tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].started == FALSE) 
      || (tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"TRRC_QSH: TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS is not started");
    return;
  }
  
  if(tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1) != NULL)
  {
    (void)tdsrrcrce_get_t300_from_serv_cell_db(&t_300);
    (void)tdsrrcrce_get_n300_from_serv_cell_db(&n_300);
  }

  memset(&tmr_and_const_params, 0x00 , sizeof(tdsrrc_qsh_metric_tmr_and_const_s));

  tmr_and_const_params.t313 = (uint8)TDSRRC_GET_T313();
  tmr_and_const_params.t300 = (uint8)t_300;
  tmr_and_const_params.t302 = (uint8)TDSRRC_GET_T302();
  tmr_and_const_params.n313 = (uint8)TDSRRC_GET_N313();
  tmr_and_const_params.n315 = (uint8)TDSRRC_GET_N315();
  tmr_and_const_params.n300 = (uint8)n_300;
  tmr_and_const_params.n302 = (uint8)TDSRRC_GET_N302();

  if(memcmp(&tmr_and_const_params, &tdsrrcqsh_global.tmr_and_const, sizeof(tdsrrc_qsh_metric_tmr_and_const_s)) != 0)
  {
    memscpy((void *)&tdsrrcqsh_global.tmr_and_const, sizeof(tdsrrc_qsh_metric_tmr_and_const_s),
            (void *)&tmr_and_const_params, sizeof(tdsrrc_qsh_metric_tmr_and_const_s));

    memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr, 
            tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].size_bytes,
            (void *)&tdsrrcqsh_global.tmr_and_const, sizeof(tdsrrc_qsh_metric_tmr_and_const_s));

    tdsrrcqsh_commit_data((qsh_metric_id_t)TDSRRC_QSH_METRIC_TIMERS_AND_CONSTANTS, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  } 
}

/*===========================================================================

FUNCTION        tdsrrcqsh_serving_cell_update

DESCRIPTION     Update serving cell information and send event to QSH for the
                update.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_serving_cell_update(void)
{
  tdsrrc_state_e_type rrc_state;
  tdsl1_common_ts_info_struct_type *common_ts_info_ptr = NULL;
  uint16 freq = 0;
  uint8 cpid = 0;
  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();
  
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
	return;
  }

  if((tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_SERVING].started == FALSE)||
     (tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_SERVING].data_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TRRC_QSH: TDSRRC_QSH_METRIC_SERVING is not started");
    return;
  }

  if(TDSRRC_NOT_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_QSH))
  {
    /* Do not need to update serving cell metric if not camping on. */
    return;
  }
  
  rrc_state = tdsrrc_get_state();
  
  tdsrrcqsh_global.serving_cell_metric.rrc_state = (uint8)rrc_state;

  if(rrc_state == TDSRRC_STATE_DISCONNECTED)
  {
    tdsrrcqsh_global.serving_cell_metric.drx_cycle = (uint16)tdsrrcllc_get_drx_info();
  }
  else
  {
    tdsrrcqsh_global.serving_cell_metric.drx_cycle = (uint16)tdsutran_drx_cycle_info.utran_drx_cycle_length.length;
  }

  if(TDSRRC_STATE_CELL_DCH == rrc_state)
  {
    if(tdsl1_get_reference_cell_info(&freq, &cpid))
    {
      tdsrrcqsh_global.serving_cell_metric.uarfcn = freq;
      tdsrrcqsh_global.serving_cell_metric.cpid = (uint16)cpid;
    }
    else
    {
      /*should not enter here*/
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TRRC_QSH: TDSRRC_QSH_METRIC_SERVING: can not get freq and cpid form L1.");
      return;
    }

    if ((cpid == tdsordered_config_ptr->cell_param_id) &&
      (tdscell_id_per_rl[0] != TDSINVALID_CELL_ID_OTA))
    {
      /* cell_id is unknown in CELL_DCH state */
      tdsrrcqsh_global.serving_cell_metric.cell_id = tdscell_id_per_rl[0];
    }
    else if(tdscell_id_per_rl[0] == TDSINVALID_CELL_ID_OTA
            && tdsrrcqsh_global.previous_serving_cell_metric.cpid == cpid
            && tdsrrcqsh_global.previous_serving_cell_metric.uarfcn == freq)
    {
      tdsrrcqsh_global.serving_cell_metric.cell_id = tdsrrcqsh_global.previous_serving_cell_metric.cell_id;
    }
    else
    {
      tdsrrcqsh_global.serving_cell_metric.cell_id = TDSINVALID_CELL_ID_OTA;
    }

    common_ts_info_ptr = &tdscurrent_config_ptr->l1_ul_chan_parms.dpch_cfg.common_ts_info;

    /* TDM is configured except following two cases:
     * 1. Repetition period = 1 indicate continuous allocation.
     *    (See 25.331, 10.3.6.10 Common timeslot info)
     * 2. Repetition period = Repetition length. 
     *    Some network vendor may configure it. It is not consistent with protocol. 
     *    But it shall be considered as the continuous allocation.
     */
    if((common_ts_info_ptr->repetition_period != TDSL1_DPCH_REPEAT_PRD_MIN) &&
       (common_ts_info_ptr->repetition_period != common_ts_info_ptr->repetition_length))
    {
      tdsrrcqsh_global.serving_cell_metric.dpch_ul_tdm_status = TRUE;
    }
    else
    {
      tdsrrcqsh_global.serving_cell_metric.dpch_ul_tdm_status = FALSE;
    }

    common_ts_info_ptr = &tdscurrent_config_ptr->l1_dl_chan_parms.dpch.dl_cctrch_list.common_ts_info;

    if((common_ts_info_ptr->repetition_period != TDSL1_DPCH_REPEAT_PRD_MIN) &&
       (common_ts_info_ptr->repetition_period != common_ts_info_ptr->repetition_length))
    {
      tdsrrcqsh_global.serving_cell_metric.dpch_dl_tdm_status = TRUE;
    }
    else
    {
      tdsrrcqsh_global.serving_cell_metric.dpch_dl_tdm_status = FALSE;
    }
  }
  else
  {
    tdsrrcqsh_global.serving_cell_metric.uarfcn = tdsrrcccm_get_curr_camped_freq();
 
    tdsrrcqsh_global.serving_cell_metric.cpid = tdsrrcccm_get_curr_camped_cpid();

    tdsrrcqsh_global.serving_cell_metric.cell_id = tdsrrcccm_get_curr_camped_cell_id();

    tdsrrcqsh_global.serving_cell_metric.dpch_ul_tdm_status = FALSE;

    tdsrrcqsh_global.serving_cell_metric.dpch_dl_tdm_status = FALSE;
  }

  if((tdsrrcqsh_global.serving_cell_metric.uarfcn != tdsrrcqsh_global.previous_serving_cell_metric.uarfcn) ||
     (tdsrrcqsh_global.serving_cell_metric.cpid != tdsrrcqsh_global.previous_serving_cell_metric.cpid) ||
     (tdsrrcqsh_global.serving_cell_metric.cell_id != tdsrrcqsh_global.previous_serving_cell_metric.cell_id) ||
     (tdsrrcqsh_global.serving_cell_metric.dpch_ul_tdm_status != tdsrrcqsh_global.previous_serving_cell_metric.dpch_ul_tdm_status) ||
     (tdsrrcqsh_global.serving_cell_metric.dpch_dl_tdm_status != tdsrrcqsh_global.previous_serving_cell_metric.dpch_dl_tdm_status) ||
     (tdsrrcqsh_global.serving_cell_metric.rrc_state != tdsrrcqsh_global.previous_serving_cell_metric.rrc_state) ||
     (tdsrrcqsh_global.serving_cell_metric.drx_cycle != tdsrrcqsh_global.previous_serving_cell_metric.drx_cycle))
  {
    memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_SERVING].data_ptr, tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_SERVING].size_bytes,
              (void *)&tdsrrcqsh_global.serving_cell_metric, sizeof(tdsrrc_qsh_metric_serving_cell_s));
  
    /*
     * Commit and get next addr
     */
    tdsrrcqsh_commit_data((qsh_metric_id_t)TDSRRC_QSH_METRIC_SERVING, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

    tdsrrcqsh_global.previous_serving_cell_metric = tdsrrcqsh_global.serving_cell_metric;

    tdsrrcqsh_initialize_metric_data_by_id(TDSRRC_QSH_METRIC_SERVING);
  }

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_rrc_state_update

DESCRIPTION     Send event to QSH if RRC state is updated.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_rrc_state_update(tdsrrc_state_e_type new_state)
{
  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();
  
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
	return;
  }

  if((tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_RRC_STATE].started == FALSE)||
     (tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_RRC_STATE].data_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TRRC_QSH: TDSRRC_QSH_METRIC_RRC_STATE is not started");
    return;
  }

  tdsrrcqsh_global.rrc_state.rrc_state = (uint8)new_state;

  if(tdsrrcqsh_global.rrc_state.rrc_state != tdsrrcqsh_global.previous_rrc_state.rrc_state)
  {
    memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_RRC_STATE].data_ptr, tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_RRC_STATE].size_bytes,
              (void *)&tdsrrcqsh_global.serving_cell_metric, sizeof(tdsrrc_qsh_metric_rrc_state_s));
  
    /*
     * Commit and get next addr
     */
    tdsrrcqsh_commit_data((qsh_metric_id_t)TDSRRC_QSH_METRIC_RRC_STATE, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

    tdsrrcqsh_global.previous_rrc_state = tdsrrcqsh_global.rrc_state;
  }

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_save_ota

DESCRIPTION     This function saves the passed OTA and send to QSH.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_save_ota
(
  uint8                     *sdu,
  uint16                    sdu_length,
  tdsrrcqsh_ota_pdu_type_e  pdu_type,
  uint8                     msg_type
)
{
  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();
  
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
	return;
  }

  if((tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_OTA].started == FALSE)||
     (tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_OTA].data_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TRRC_QSH: TDSRRC_QSH_METRIC_OTA is not started");
    return;
  }

  /* First 3 bits from MSB indicate pdu type. Next 5 bits indicate msg type */
  tdsrrcqsh_global.ota.msg_id = ((pdu_type<<5)|(msg_type));
  /* Other fields in OTA params will be later populated */

  if(tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_OTA].data_ptr, tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_OTA].size_bytes,
              (void *)&tdsrrcqsh_global.ota, sizeof(tdsrrc_qsh_metric_ota_s));
  
    /*
     * Commit and get next addr
     */
    tdsrrcqsh_commit_data((qsh_metric_id_t)TDSRRC_QSH_METRIC_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

    tdsrrcqsh_initialize_metric_data_by_id(TDSRRC_QSH_METRIC_OTA);
  }

  return;
}

/*===========================================================================

FUNCTION        tdsrrcqsh_conn_end_info_update

DESCRIPTION       
  This function updates RLF/RA failure/L2 failure/RRC release/HO failure to qsh

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_conn_end_info_update(tdsrrc_qsh_metric_conn_end_info_s_type * conn_end_info_ptr)
{

  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();
  
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
	return;
  }

  if((tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_CONN_END_INFO].started == FALSE)||
     (tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_CONN_END_INFO].data_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TRRC_QSH: TDSRRC_QSH_METRIC_CONN_END_INFO is not started");
    return;
  }

  tdsrrcqsh_global.conn_end_info.type = conn_end_info_ptr->type;

  switch(conn_end_info_ptr->type)
  {
    case SYS_CALL_END_EVENT_HANDOVER_FAIL: 
      tdsrrcqsh_global.conn_end_info.cause.ho_fail_type= conn_end_info_ptr->cause.ho_fail_type;
      break;   

    case SYS_CALL_END_EVENT_RLF_OR_WEAK_SIGNAL:
      tdsrrcqsh_global.conn_end_info.cause.rlf_weak_signal_type = conn_end_info_ptr->cause.rlf_weak_signal_type;
      break; 

    case SYS_CALL_END_EVENT_RX_RRC_RELEASE:
      tdsrrcqsh_global.conn_end_info.cause.conn_rel_cause = conn_end_info_ptr->cause.conn_rel_cause;
      break;
      
    default:
      TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: METRIC_CONN_END_INFO, wrong conn_end_type %d", 
        conn_end_info_ptr->type);
      break;
  }

  memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_CONN_END_INFO].data_ptr, 
          tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_CONN_END_INFO].size_bytes,
          (void *)&tdsrrcqsh_global.conn_end_info, 
          sizeof(tdsrrc_qsh_metric_conn_end_info_s_type));

  tdsrrcqsh_commit_data((qsh_metric_id_t)TDSRRC_QSH_METRIC_CONN_END_INFO, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
    
  return;
}

/*===========================================================================

FUNCTION tdsrrcrce_qsh_update_rlf_or_weak_signal_failure

DESCRIPTION
  This function updates RLF/RA failure/L2 failure to qsh
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrcqsh_update_rlf_or_weak_signal_failure(sys_rlf_weak_signal_e_type type)
{
  tdsrrc_qsh_metric_conn_end_info_s_type conn_end_info;
  conn_end_info.type = SYS_CALL_END_EVENT_RLF_OR_WEAK_SIGNAL;
  conn_end_info.cause.rlf_weak_signal_type = type;
  tdsrrcqsh_conn_end_info_update(&conn_end_info);
}

/*===========================================================================
FUNCTION tdsrrcrce_qsh_update_connection_release

DESCRIPTION
  This function updates rrc connection release cause to qsh
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcqsh_update_connection_release(tdsrrc_conn_rel_cause_e_type rel_cause)
{
  tdsrrc_qsh_metric_conn_end_info_s_type conn_end_info;
  conn_end_info.type = SYS_CALL_END_EVENT_RX_RRC_RELEASE;
  conn_end_info.cause.conn_rel_cause = (uint8)rel_cause;
  tdsrrcqsh_conn_end_info_update(&conn_end_info);
}

/*===========================================================================
FUNCTION tdsrrcrce_qsh_update_connection_release

DESCRIPTION
  This function updates ho failure type to qsh
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrcqsh_update_ho_failure(sys_handover_failure_e_type ho_fail_type)
{
  tdsrrc_qsh_metric_conn_end_info_s_type conn_end_info;
  conn_end_info.type = SYS_CALL_END_EVENT_HANDOVER_FAIL;
  conn_end_info.cause.ho_fail_type = ho_fail_type;
  tdsrrcqsh_conn_end_info_update(&conn_end_info);
}

/*===========================================================================

FUNCTION        tdsrrcqsh_rab_status_update

DESCRIPTION     Send event to QSH if rab_status changes.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_rab_status_update(void)
{
  sys_rab_status_e_type curr_rab_status;
  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();
  
  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
	return;
  }

  if((tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].started == FALSE)||
     (tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].data_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TRRC_QSH: TDSRRC_QSH_METRIC_MULTI_RAB_STATUS is not started");
    return;
  }

  curr_rab_status = (sys_rab_status_e_type)tdsrrc_query_mac_rab_status();

  if(tdsrrcqsh_global.rab_status.rab_status != curr_rab_status)
  {
    tdsrrcqsh_global.rab_status.rab_status  = curr_rab_status;
    
    memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].data_ptr, 
            tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_MULTI_RAB_STATUS].size_bytes,
            (void *)&tdsrrcqsh_global.rab_status, 
            sizeof(tdsrrc_qsh_metric_multi_rab_status_s_type));
  
    tdsrrcqsh_commit_data((qsh_metric_id_t)TDSRRC_QSH_METRIC_MULTI_RAB_STATUS, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  }
  
  return;
}


/*===========================================================================

FUNCTION        tdsrrcqsh_cu_cause_update

DESCRIPTION     Send event to QSH if cu_cause changes.

DEPENDENCIES
                None

RETURN VALUE
                None.

SIDE EFFECTS
                None

===========================================================================*/
void tdsrrcqsh_cu_cause_update(uint8 cu_cause)
{
  sys_modem_as_id_e_type curr_sub_id = tdsrrc_get_as_id();

  if((curr_sub_id < SYS_MODEM_AS_ID_1) || (curr_sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
  {
    TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC_QSH: invalid sub_id %d", curr_sub_id);
    return;
  }

  if((tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].started == FALSE)||
     (tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].data_ptr == NULL))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TRRC_QSH: TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE is not started");
    return;
  }

  tdsrrcqsh_global.cell_update_cause.cell_update_cause = cu_cause;
  
  memscpy(tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].data_ptr, 
          tdsrrcqsh_global.metric_cfg[curr_sub_id][TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE].size_bytes,
          (void *)&tdsrrcqsh_global.cell_update_cause, 
          sizeof(tdsrrc_qsh_metric_cell_update_cause_s_type));

  tdsrrcqsh_commit_data((qsh_metric_id_t)TDSRRC_QSH_METRIC_CELL_UPDATE_CAUSE, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  
  return;
}

#endif /* FEATURE_QSH_EVENT_METRIC */

