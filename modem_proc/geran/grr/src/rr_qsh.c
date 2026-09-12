/*!
 * \file rr_qsh.c 
 *  
 * This module contains functionality to interface to QSH. 
 *  
 *              Copyright (c) 2015 Qualcomm Technologies, Inc.
 *              All Rights Reserved.
 *              Qualcomm Confidential and Proprietary
 */
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_qsh.c#3 $ */
/* $DateTime: 2020/04/28 03:46:30 $$Author: pwbldsvc $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"

#if defined (FEATURE_QSH_DUMP) || defined (FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)|| defined(FEATURE_QSH_MDUMP)

#include "qsh.h"
#include "rr_qsh.h"
#include "grr_qsh_ext.h"
#include "rr_qsh_dump.h"
#include "rr_qsh_metric.h"
#include "rr_qsh_event.h"
#include "rr_qsh_event_hdlr.h"
#include "rr_defs.h"
#ifdef FEATURE_QSH_MDUMP
#include "rr_control.h"
#include "rr_grr.h"
#include "rr_gprs_debug.h"
#include "rr_cell_acq.h"
#include "rr_resel.h"
#include "rr_select_bcch.h"
#include "rr_candidate_cell_db.h"
#include "rr_mode.h"
#include "rr_ps_access.h"
#include "rr_l1_idle_mode.h"
#include "rr_plmn_list.h"
#ifdef FEATURE_GSM_DTM
#include "rr_dtm.h"
#endif /* FEATURE_GSM_DTM */
#include "rr_conn_establish.h"
#include "rr_conn_release.h"
#include "rr_cell_reestablish.h"
#include "rr_cell_selection.h"
#include "rr_intra_ho.h"
#include "rr_inter_ho.h"
#if defined(FEATURE_WCDMA) || defined(FEATURE_LTE) || defined(FEATURE_GSM_TDS)
#include "rr_x2g_resel.h"
#include "rr_w2g_service_redirection.h"
#endif /* FEATURE_WCDMA || FEATURE_LTE || FEATURE_GSM_TDS */
#ifdef FEATURE_WCDMA
#include "rr_g2w_handover.h"
#endif /* FEATURE_WCDMA */
#endif /* FEATURE_QSH_MDUMP */
/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR static void grr_qsh_mdump_collect(qsh_client_cb_params_s *param);
#endif /* FEATURE_QSH_MDUMP */

/*!
 * \brief QSH call-back function
 * 
 * \param params_ptr (in)
 */
#ifndef FEATURE_QSH_MDUMP
__attribute__((section(".uncompressible.text")))
#else
QSH_MDUMP_FN_ATTR
#endif
void rr_qsh_cb_fn(qsh_client_cb_params_s *params_ptr)
{
  RR_NULL_CHECK_RETURN_VOID(params_ptr);

  switch(params_ptr->action)
  {
#ifdef FEATURE_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
    {
      rr_qsh_dump_perform_cb_action(params_ptr);
      break;
    }
#endif // FEATURE_QSH_DUMP

#ifdef FEATURE_QSH_EVENT_METRIC
    case QSH_ACTION_METRIC_CFG:
    {
      rr_send_imsg_qsh_metric_config_ind(params_ptr);
      break;
    }

    case QSH_ACTION_METRIC_TIMER_EXPIRY:
    {
      rr_send_imsg_qsh_metric_timer_expiry_ind(params_ptr);
      break;
    }
#endif // FEATURE_QSH_EVENT_METRIC

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    case QSH_ACTION_CFG:
    {
      rr_send_imsg_qsh_event_config_ind(params_ptr);
      break;
    }
#endif // FEATURE_QSH_EVENT_NOTIFY_TO_QSH

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif 
#ifdef FEATURE_QSH_MDUMP
    case QSH_ACTION_MDUMP:
    {
      grr_qsh_mdump_collect(params_ptr);
      break;
    }
#endif /*FEATURE_QSH_DUMP*/

    default:
    {
      break;
    }
  }

  return;
}

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/

#ifdef FEATURE_QSH_EVENT_METRIC
/*!
 * \brief Process function for RR_IMSG_METRIC_CONFIG_IND
 * 
 * \param msg_ptr (in)
 */
__attribute__((section(".uncompressible.text")))
void rr_qsh_process_metric_config_ind(rr_imsg_qsh_metric_config_ind_t *msg_ptr)
{
  rr_qsh_metric_config_perform_cb_action(&msg_ptr->cb_params);

  return;
}

/*!
 * \brief Process function for RR_IMSG_METRIC_TIMER_EXPIRY_IND
 * 
 * \param msg_ptr (in)
 */
__attribute__((section(".uncompressible.text")))
void rr_qsh_process_metric_timer_expiry_ind(rr_imsg_qsh_metric_timer_expiry_ind_t *msg_ptr)
{
  rr_qsh_metric_timer_expiry_perform_cb_action(&msg_ptr->cb_params);

  return;
}
#endif // FEATURE_QSH_EVENT_METRIC

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*!
 * \brief Process function for RR_IMSG_EVENT_CONFIG_IND
 * 
 * \param msg_ptr (in)
 */
__attribute__((section(".uncompressible.text")))
void rr_qsh_process_event_config_ind(rr_imsg_qsh_event_config_ind_t *msg_ptr)
{
  rr_qsh_event_config_perform_cb_action(&msg_ptr->cb_params);

  return;
}
#endif // FEATURE_QSH_EVENT_NOTIFY_TO_QSH

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif // FEATURE_QSH_EVENT_NOTIFY_HANDLER

/*!
 * \brief Initialisation function called once at RR task start.
 */
__attribute__((section(".uncompressible.text")))
void rr_qsh_task_start_init(void)
{
  qsh_client_reg_s  client_reg;
  qsh_action_mask_t cb_action_support_mask = 0;

  // Initialise QSH registration structure
  qsh_client_reg_init(&client_reg);

#ifdef FEATURE_QSH_DUMP
  // Initialise the QSH dump module
  client_reg.dump_info = rr_qsh_dump_init();

  // Setup the call-back support mask for dumps
  cb_action_support_mask |= QSH_ACTION_DUMP_COLLECT;
#endif // FEATURE_QSH_DUMP

#ifdef FEATURE_QSH_EVENT_METRIC
  // Initialise the QSH metric module
  client_reg.metric_info = rr_qsh_metric_init();

  // Setup the call-back support mask for metrics
  cb_action_support_mask |= (QSH_ACTION_METRIC_CFG | QSH_ACTION_METRIC_TIMER_EXPIRY);
#endif // FEATURE_QSH_EVENT_METRIC

#if defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)
  /* Initialize the QSH event module */
  rr_qsh_event_init();
  cb_action_support_mask |= (QSH_ACTION_CFG | QSH_ACTION_EVENT_NOTIFY);
#endif

#ifdef FEATURE_QSH_MDUMP
  cb_action_support_mask |= QSH_ACTION_MDUMP;
#endif /* FEATURE_QSH_MDUMP */

  client_reg.client = QSH_CLT_GRR;
  client_reg.client_cb_ptr = rr_qsh_cb_fn;
  client_reg.major_ver = GRR_QSH_MAJOR_VER;
  client_reg.minor_ver = GRR_QSH_MINOR_VER;
  client_reg.cb_action_support_mask = cb_action_support_mask;

  // Register with QSH
  qsh_client_reg(&client_reg);

  return;
}

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR 
static void grr_qsh_mdump_collect(qsh_client_cb_params_s *param)
{
  gas_id_t gas_id;

  for (gas_id = 0; gas_id < NUM_GERAN_DATA_SPACES; gas_id++)
  {
    grr_mdump_rr_state(gas_id);

    switch(get_rr_state(gas_id))
    {
      case RR_INACTIVE:
        break;

      case RR_GOING_ACTIVE:
      case RR_GOING_INACTIVE:
      {
        grr_mdump_rr_mode(gas_id);
        grr_mdump_rr_l1_idle(gas_id);
        break;
      }

      case RR_PLMN_LIST_SEARCH:
      {
        grr_mdump_rr_gprs_debug(gas_id);
        grr_mdump_rr_plc(gas_id);
        grr_mdump_rr_control(gas_id);
        grr_mdump_rr_plmn_list_req(gas_id);
        break;
      }

      case RR_CELL_SELECTION:
      {
        grr_mdump_rr_gprs_debug(gas_id);
        grr_mdump_rr_control(gas_id);
        grr_mdump_rr_cell_selection(gas_id);
        grr_mdump_rr_cell_acq(gas_id);
        grr_mdump_rr_select_bcch(gas_id);
        break;
      }

      case RR_GRR:
      {
        grr_mdump_rr_gprs_debug(gas_id);
        grr_mdump_rr_control(gas_id);
        grr_mdump_grr_control(gas_id);
        grr_mdump_rr_cell_acq(gas_id);
        grr_mdump_rr_resel(gas_id);
        grr_mdump_rr_select_bcch(gas_id);
        grr_mdump_rr_decode_bcch(gas_id);
#if defined(FEATURE_WCDMA) || defined(FEATURE_LTE) || defined(FEATURE_GSM_TDS)
        grr_mdump_rr_x2g_resel(gas_id);
        grr_mdump_rr_x2g_redir(gas_id);
#endif
        grr_mdump_rr_mode(gas_id);
        grr_mdump_rr_ps_access(gas_id);
        break;
      }

      case RR_CONNECTION_PENDING:
      {
        grr_mdump_rr_gprs_debug(gas_id);
        grr_mdump_rr_conn_est(gas_id);
        break;
      }

      case RR_CELL_REESTABLISH:
      {
        grr_mdump_rr_gprs_debug(gas_id);
        grr_mdump_rr_conn_est(gas_id);
        grr_mdump_rr_select_bcch(gas_id);
        grr_mdump_rr_decode_bcch(gas_id);
        grr_mdump_rr_reest(gas_id);
        break;
      }

      case RR_DATA_TRANSFER:
      {
#ifdef FEATURE_GSM_DTM
        grr_mdump_rr_dtm(gas_id);
#endif
        grr_mdump_rr_conn_rel(gas_id);
        grr_mdump_rr_ps_access(gas_id);
        grr_mdump_rr_intra_ho(gas_id);
        grr_mdump_rr_control(gas_id);
        grr_mdump_rr_inter_ho(gas_id);
        break;
      }

      case RR_NO_CHANNELS:
      {
        grr_mdump_rr_mode(gas_id);
        break;
      }

      case RR_CONNECTION_RELEASE:
      {        
        grr_mdump_rr_conn_rel(gas_id);
        break;
      }

#if defined(FEATURE_WCDMA) || defined(FEATURE_LTE) || defined(FEATURE_GSM_TDS)
      case RR_X2G_RESEL:
      case RR_X2G_RESEL_ABORTED:
#endif /* FEATURE_WCDMA || FEATURE_LTE || FEATURE_GSM_TDS */
#if defined(FEATURE_GTOW_REDIRECTION_AFTER_CONN_RELEASE) || defined(FEATURE_LTE) || defined(FEATURE_GSM_TDS_G2T_REDIRECTION)
      case RR_G2X_REDIR:
#endif /* FEATURE_GTOW_REDIRECTION_AFTER_CONN_RELEASE || FEATURE_LTE || FEATURE_GSM_TDS_G2T_REDIRECTION*/
#ifdef FEATURE_LTE_TO_GSM_CGI
      case RR_X2G_CGI:
#endif /* FEATURE_LTE_TO_GSM_CGI */
      case RR_X2G_CCO_FAILED:
      case RR_X2G_CCO_ABORTED:
      case RR_X2G_CCO_FAILED_ABORTED:
      case RR_X2G_REDIR:
      case RR_W2G_SERVICE_REDIRECTION_IN_PROGRESS:
      case RR_W2G_INTERRAT_HANDOVER_PROGRESS:
      case RR_W2G_INTERRAT_CC_ORDER_PROGRESS:
      case RR_G2W_INTERRAT_HANDOVER_PROGRESS:
      {
        grr_mdump_rr_gprs_debug(gas_id);
#if defined(FEATURE_WCDMA) || defined(FEATURE_LTE) || defined(FEATURE_GSM_TDS)
        grr_mdump_rr_x2g_resel(gas_id);
        grr_mdump_rr_x2g_redir(gas_id);
#endif
#ifdef FEATURE_WCDMA
        grr_mdump_rr_g2w_handover(gas_id);
#endif /* FEATURE_WCDMA */
        grr_mdump_rr_inter_ho(gas_id);
        grr_mdump_rr_control(gas_id);
        grr_mdump_grr_control(gas_id);
        grr_mdump_rr_cell_acq(gas_id);
        grr_mdump_rr_resel(gas_id);
        grr_mdump_rr_select_bcch(gas_id);
        grr_mdump_rr_decode_bcch(gas_id);
        grr_mdump_rr_mode(gas_id);
        break;
      }

      default:
      {
        grr_mdump_rr_gprs_debug(gas_id);
        break;
      }
    }

  }

  qsh_mdump_prioritize_client(QSH_CLT_GRR, QSH_CLT_GL1);
  qsh_mdump_prioritize_client(QSH_CLT_GRR, QSH_CLT_GMAC);
}
#endif /* FEATURE_QSH_MDUMP */

#endif /* FEATURE_QSH_DUMP || FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_EVENT_NOTIFY_TO_QSH || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)|| defined(FEATURE_QSH_MDUMP) */

/* EOF */

