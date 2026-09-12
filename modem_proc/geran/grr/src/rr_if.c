/*! \file rr_if.c
 
  This module contains the functionality to send RR-IMSGs into RR, as defined by
  the API in rr_if.h.
 
                Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_if.c#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "rr_seg_load.h"
#include "rr_if.h"
#include "rr_rr.h"
#include "rr_gprs_debug.h"
#include "geran_multi_sim.h"
#include "stringl/stringl.h"
#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)
#include "qsh.h"
#endif // FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_EVENT_NOTIFY_TO_QSH || FEATURE_QSH_EVENT_NOTIFY_HANDLER


/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Static Function Declarations and Definitions
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Externalized Function Definitions
 * -------------------------------------------------------------------------*/

const char * rr_imsg_name(const rr_imsg_e imsg)
{
  switch (imsg)
  {
    case RR_IMSG_PS_ACCESS_IND:                     return "RR_IMSG_PS_ACCESS_IND";
#ifdef FEATURE_LTE
    case RR_IMSG_INDIVIDUAL_PRIORITIES_UPDATED_IND: return "RR_IMSG_INDIVIDUAL_PRIORITIES_UPDATED_IND";
    case RR_IMSG_IRAT_ALLOWED_UPDATED_IND:          return "RR_IMSG_IRAT_ALLOWED_UPDATED_IND";
    case RR_IMSG_IRAT_SYSTEM_INFORMATION_UPDATED_IND: return "RR_RESEL_IMSG_IRAT_SYSTEM_INFORMATION_UPDATED_IND";
#endif /* FEATURE_LTE */
    case RR_IMSG_RR_EVENT_IND:                      return "RR_IMSG_RR_EVENT_IND";
    case RR_IMSG_SAVED_MSG_IND:                     return "RR_IMSG_SAVED_MSG_IND";
    case RR_IMSG_PS_ACCESS_REQ:                     return "RR_IMSG_PS_ACCESS_REQ";
    case RR_IMSG_NO_PS_ACCESS_REQ:                  return "RR_IMSG_NO_PS_ACCESS_REQ";
    case RR_IMSG_GPRS_IA_IND:                       return "RR_IMSG_GPRS_IA_IND";
    case RR_IMSG_MSC_CHANGE_IND:                    return "RR_IMSG_MSC_CHANGE_IND";
    case RR_IMSG_X2G_CCO_COMPLETE_IND:              return "RR_IMSG_X2G_CCO_COMPLETE_IND";
    case RR_IMSG_PAGE_IND:                          return "RR_IMSG_PAGE_IND";
    case RR_IMSG_PAGE_MODE_IND:                     return "RR_IMSG_PAGE_MODE_IND";
    case RR_IMSG_L1_IDLE_MODE_CHANGE_REQUIRED:      return "RR_IMSG_L1_IDLE_MODE_CHANGE_REQUIRED";
#ifdef FEATURE_SGLTE
    case RR_IMSG_SGLTE_SWITCH_TO_CS_ONLY_GSM_ONLY_REQ: return "RR_IMSG_SGLTE_SWITCH_TO_CS_ONLY_GSM_ONLY_REQ";
#endif /* FEATURE_SGLTE */
#ifdef FEATURE_MODEM_CONFIG_REFRESH
    case RR_IMSG_GERAN_NV_REFRESH_IND:              return "RR_IMSG_GERAN_NV_REFRESH_IND";
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
    case RR_IMSG_CSI_MULTIPLE_INSTANCES_SIS_RETRIEVED_IND: return "RR_IMSG_CSI_MULTIPLE_INSTANCES_SIS_RETRIEVED_IND";
#ifdef FEATURE_LOCK_ARFCN
    case  RR_IMSG_SELECTION_START_LOCKING_IND:     return "RR_IMSG_SELECTION_START_LOCKING_IND" ;
    case  RR_IMSG_SELECTION_START_UNLOCKING_IND:   return "RR_IMSG_SELECTION_START_UNLOCKING_IND";
    case  RR_IMSG_SELECTION_NO_LOCKING_REQUIRED:   return "RR_IMSG_SELECTION_NO_LOCKING_REQUIRED";
#endif /*FEATURE_LOCK_ARFCN*/
#ifdef FEATURE_QSH_EVENT_METRIC
    case RR_IMSG_QSH_METRIC_CONFIG_IND:             return "RR_IMSG_QSH_METRIC_CONFIG_IND";
    case RR_IMSG_QSH_METRIC_TIMER_EXPIRY_IND:       return "RR_IMSG_QSH_METRIC_TIMER_EXPIRY_IND";
#endif // FEATURE_QSH_EVENT_METRIC
#ifdef FEATURE_GERAN_HST_OPT
    case RR_IMSG_FREQ_LIST_UPDATE_REQUIRED:         return "RR_IMSG_FREQ_LIST_UPDATE_REQUIRED";
#endif /* FEATURE_GERAN_HST_OPT */
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif // FEATURE_QSH_EVENT_NOTIFY_HANDLER_TO_QSH
    default:                                        return "?";
  }
}


void rr_send_imsg(
  rr_imsg_e imsg,
  const gas_id_t gas_id
)
{
  rr_message_header_t header;

  memset(&header, 0, sizeof(header));

  rr_rr_send(
    &header,
    (uint8) imsg,
    sizeof(header),
    RR_IMSG,
    gas_id
  );
}

void rr_send_priority_imsg_rr_event_ind(
  rr_event_T rr_event,
  const gas_id_t gas_id
)
{
  rr_imsg_rr_event_ind_t message;

#ifndef FEATURE_GERAN_REDUCED_SPRINTF
  MSG_SPRINTF_1(MSG_SSID_DFLT,
                MSG_LEGACY_HIGH,
                "Sending RR_IMSG_RR_EVENT_IND with %s",
                rr_event_name(rr_event));
#else
  MSG_GERAN_HIGH_1_G("Sending RR_IMSG_RR_EVENT_IND with %d", rr_event);
#endif /* FEATURE_GERAN_REDUCED_SPRINTF */

  message.rr_event = rr_event;
  message.priority_over_other_imsg = TRUE;

  rr_rr_send(
    &message.header,
    (uint8) RR_IMSG_RR_EVENT_IND,
    sizeof(message),
    RR_IMSG,
    gas_id
  );
}

void rr_send_imsg_rr_event_ind(
  rr_event_T rr_event,
  const gas_id_t gas_id
)
{
  rr_imsg_rr_event_ind_t message;
  message.rr_event = rr_event;
  message.priority_over_other_imsg = FALSE;

#ifndef FEATURE_GERAN_REDUCED_SPRINTF
  MSG_SPRINTF_1(MSG_SSID_DFLT,
                MSG_LEGACY_HIGH,
                "Sending RR_IMSG_RR_EVENT_IND with %s",
                rr_event_name(rr_event));
#else
    MSG_GERAN_HIGH_1_G("Sending RR_IMSG_RR_EVENT_IND with %d", rr_event);
#endif /* FEATURE_GERAN_REDUCED_SPRINTF */

  rr_rr_send(
    &message.header,
    (uint8) RR_IMSG_RR_EVENT_IND,
    sizeof(message),
    RR_IMSG,
    gas_id
  );
}

void rr_send_imsg_saved_msg_ind(
  mm_rr_msgs_u *msg,
  const gas_id_t gas_id
)
{
  rr_imsg_saved_msg_ind_t message;

#ifndef FEATURE_GERAN_REDUCED_SPRINTF
  MSG_SPRINTF_1(MSG_SSID_DFLT,
                MSG_LEGACY_HIGH,
                "Sending RR_IMSG_SAVED_MSG_IND with %s",
                rr_gprs_message_name(
                  (rr_cmd_bdy_type *)msg,
                  msg->header.message_set,
                  msg->header.message_id,
                  NULL
                )
               );
#else
      MSG_GERAN_HIGH_2_G("Sending RR_IMSG_SAVED_MSG_IND with (%d, %d)", 
                         msg->header.message_set,
                         msg->header.message_id);
#endif /* FEATURE_GERAN_REDUCED_SPRINTF */

  memscpy(&message.msg,sizeof(message.msg), msg, sizeof(message.msg));

  rr_rr_send(
    &message.header,
    (uint8) RR_IMSG_SAVED_MSG_IND,
    sizeof(message),
    RR_IMSG,
    gas_id
  );
}

void rr_send_imsg_ps_access_req(
  boolean purge_gmm_signalling,
  rr_cb_fn_t confirm_cb_fn,
  const gas_id_t gas_id
)
{
  rr_imsg_ps_access_req_t message;

  MSG_GERAN_HIGH_1_G("RR_IMSG_PS_ACCESS_REQ: purge_gmm=%d", purge_gmm_signalling);

  message.purge_gmm_signalling = purge_gmm_signalling;
  message.confirm_cb_fn = confirm_cb_fn;

  rr_rr_send(
    &message.header,
    (uint8) RR_IMSG_PS_ACCESS_REQ,
    sizeof(message),
    RR_IMSG,
    gas_id
  );
}

void rr_send_imsg_no_ps_access_req(
  boolean del_all_pdus,
  rr_cb_fn_t confirm_cb_fn,
  const gas_id_t gas_id
)
{
  rr_imsg_no_ps_access_req_t message;

  message.del_all_pdus = del_all_pdus;
  message.confirm_cb_fn = confirm_cb_fn;

  rr_rr_send(
    &message.header,
    (uint8) RR_IMSG_NO_PS_ACCESS_REQ,
    sizeof(message),
    RR_IMSG,
    gas_id
  );
}

void rr_send_imsg_gprs_ia_ind(rr_gprs_ia_params_t params, rr_gprs_ia_cb_fn_t cb_fn, const gas_id_t gas_id)
{
  rr_imsg_gprs_ia_ind_t msg;

  msg.params = params;
  msg.cb_fn = cb_fn;

  rr_rr_send(
    &msg.header,
    (uint8) RR_IMSG_GPRS_IA_IND,
    sizeof(msg),
    RR_IMSG,
    gas_id
  );
}

void rr_send_imsg_page_ind(
  rr_page_info_t *page_info_ptr,
  const gas_id_t gas_id
)
{
  rr_imsg_page_ind_t msg;

  RR_NULL_CHECK_RETURN_VOID(page_info_ptr);

  msg.page_info = (*page_info_ptr);

  rr_rr_send(
    &msg.header,
    (uint8) RR_IMSG_PAGE_IND,
    sizeof(msg),
    RR_IMSG,
    gas_id
  );
}

void rr_send_imsg_page_mode_ind(
  paging_mode_T old_page_mode,
  paging_mode_T new_page_mode,
  const gas_id_t gas_id
)
{
  rr_imsg_page_mode_ind_t msg;

  msg.old_page_mode = old_page_mode;
  msg.new_page_mode = new_page_mode;

  rr_rr_send(
    &msg.header,
    (uint8) RR_IMSG_PAGE_MODE_IND,
    sizeof(msg),
    RR_IMSG,
    gas_id
  );
}
#ifdef FEATURE_LOCK_ARFCN
void rr_send_imsg_sel_locking_ind( ARFCN_T locking_ARFCN, const gas_id_t gas_id)
{
  rr_imsg_sel_start_locking_ind_t msg;

  msg.locking_ARFCN = locking_ARFCN;
  rr_rr_send(
       &msg.header,
       (uint8) RR_IMSG_SELECTION_START_LOCKING_IND,
       sizeof(msg),
       RR_IMSG,
       gas_id
       );
}
void rr_send_imsg_sel_unlocking_ind(const gas_id_t gas_id)
{
  rr_imsg_sel_start_unlocking_ind_t msg;
  rr_rr_send(
       &msg.header,
       (uint8) RR_IMSG_SELECTION_START_UNLOCKING_IND,
       sizeof(msg),
       RR_IMSG,
       gas_id
       );
}

void rr_send_imsg_sel_nolocking_required(const gas_id_t gas_id)
{
  rr_imsg_sel_nolocking_required_ind_t msg;
  rr_rr_send(
       &msg.header,
       (uint8) RR_IMSG_SELECTION_NO_LOCKING_REQUIRED,
       sizeof(msg),
       RR_IMSG,
       gas_id
       );
}
#endif /*FEATURE_LOCK_ARFCN*/

#ifdef FEATURE_MODEM_CONFIG_REFRESH
void rr_send_imsg_geran_nv_refresh_ind(
  const gas_id_t gas_id
)
{
  rr_imsg_geran_nv_refresh_ind_t message;

  rr_rr_send(
    &message.header,
    (uint8) RR_IMSG_GERAN_NV_REFRESH_IND,
    sizeof(message),
    RR_IMSG,
    gas_id
  );
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

#ifdef FEATURE_QSH_EVENT_METRIC
/*!
 * \brief Sends RR_IMSG_QSH_METRIC_CONFIG_IND to RR.
 * 
 * \param cb_params_ptr
 */
void rr_send_imsg_qsh_metric_config_ind(qsh_client_cb_params_s *cb_params_ptr)
{
  rr_imsg_qsh_metric_config_ind_t msg;

  RR_NULL_CHECK_RETURN_VOID(cb_params_ptr);

  msg.cb_params = *cb_params_ptr;

  rr_rr_send(
    &msg.header,
    (uint8) RR_IMSG_QSH_METRIC_CONFIG_IND,
    sizeof(msg),
    RR_IMSG,
    GERAN_ACCESS_STRATUM_ID_1
  );
}

/*!
 * \brief Sends RR_IMSG_QSH_METRIC_TIMER_EXPIRY_IND to RR.
 * 
 * \param cb_params_ptr
 */
void rr_send_imsg_qsh_metric_timer_expiry_ind(qsh_client_cb_params_s *cb_params_ptr)
{
  rr_imsg_qsh_metric_timer_expiry_ind_t msg;

  RR_NULL_CHECK_RETURN_VOID(cb_params_ptr);

  msg.cb_params = *cb_params_ptr;

  rr_rr_send(
    &msg.header,
    (uint8) RR_IMSG_QSH_METRIC_TIMER_EXPIRY_IND,
    sizeof(msg),
    RR_IMSG,
    GERAN_ACCESS_STRATUM_ID_1
  );
}
#endif // FEATURE_QSH_EVENT_METRIC

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*!
 * \brief Sends RR_IMSG_QSH_EVENT_CONFIG_IND to RR.
 * 
 * \param cb_params_ptr
 */
void rr_send_imsg_qsh_event_config_ind(qsh_client_cb_params_s *cb_params_ptr)
{
  rr_imsg_qsh_event_config_ind_t msg;

  RR_NULL_CHECK_RETURN_VOID(cb_params_ptr);

  msg.cb_params = *cb_params_ptr;

  rr_rr_send(
    &msg.header,
    (uint8) RR_IMSG_QSH_EVENT_CONFIG_IND,
    sizeof(msg),
    RR_IMSG,
    GERAN_ACCESS_STRATUM_ID_1
  );
}
#endif /* FEATURE_QSH_EVENT_NOTIFY_TO_QSH */

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif //FEATURE_QSH_EVENT_NOTIFY_HANDLER

/* EOF */

