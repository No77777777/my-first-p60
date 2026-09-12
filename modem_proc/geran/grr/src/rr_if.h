#ifndef RR_IF_H
#define RR_IF_H

/*! \file rr_if.h 
 
  Signalling interface for the RR module.
 
                Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
*/
/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_if.h#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "rr_message_header.h"
#include "rr_defs.h"
#include "rr_events.h"
#include "mm_rr.h"
#include "csn_apt.h"
#include "rr_ce_paging.h"
#include "rr_l1.h"
#include "geran_multi_sim.h"
#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)
#include "qsh.h"
#endif // FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_EVENT_NOTIFY_TO_QSH

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/* Input messages. */
typedef enum
{
  RR_IMSG_PS_ACCESS_IND,
  RR_IMSG_INDIVIDUAL_PRIORITIES_UPDATED_IND,
  RR_IMSG_IRAT_ALLOWED_UPDATED_IND,
  RR_IMSG_RR_EVENT_IND,
  RR_IMSG_SAVED_MSG_IND,
  RR_IMSG_PS_ACCESS_REQ,
  RR_IMSG_NO_PS_ACCESS_REQ,
  RR_IMSG_GPRS_IA_IND,
  RR_IMSG_MSC_CHANGE_IND,
  RR_IMSG_X2G_CCO_COMPLETE_IND,
  RR_IMSG_PAGE_IND,
  RR_IMSG_PAGE_MODE_IND,
  RR_IMSG_IRAT_SYSTEM_INFORMATION_UPDATED_IND,
  RR_IMSG_L1_IDLE_MODE_CHANGE_REQUIRED,
  RR_IMSG_SGLTE_SWITCH_TO_CS_ONLY_GSM_ONLY_REQ,
  RR_IMSG_SGLTE_SWITCH_TO_CS_ONLY_USER_GSM_ONLY_REQ,
#ifdef FEATURE_MODEM_CONFIG_REFRESH
  RR_IMSG_GERAN_NV_REFRESH_IND,
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
  RR_IMSG_CSI_MULTIPLE_INSTANCES_SIS_RETRIEVED_IND,
#ifdef FEATURE_LOCK_ARFCN
  RR_IMSG_SELECTION_START_LOCKING_IND,
  RR_IMSG_SELECTION_START_UNLOCKING_IND,
  RR_IMSG_SELECTION_NO_LOCKING_REQUIRED,
#endif /*FEATURE_LOCK_ARFCN*/
#ifdef FEATURE_QSH_EVENT_METRIC
  RR_IMSG_QSH_METRIC_CONFIG_IND,
  RR_IMSG_QSH_METRIC_TIMER_EXPIRY_IND,
#endif // FEATURE_QSH_EVENT_METRIC
#ifdef FEATURE_GERAN_HST_OPT
  RR_IMSG_FREQ_LIST_UPDATE_REQUIRED,
#endif /* FEATURE_GERAN_HST_OPT */
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  RR_IMSG_QSH_EVENT_CONFIG_IND,
#endif // FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif // FEATURE_QSH_EVENT_NOTIFY_HANDLER_TO_QSH
  RR_IMSG_COUNT
} rr_imsg_e;

#ifdef FEATURE_LOCK_ARFCN

typedef struct
{
  rr_message_header_t header;
  ARFCN_T             locking_ARFCN;
  gas_id_t            gas_id;
} rr_imsg_sel_start_locking_ind_t;

typedef struct
{
  rr_message_header_t header;
  gas_id_t            gas_id;
} rr_imsg_sel_start_unlocking_ind_t;

typedef struct
{
  rr_message_header_t header;
  gas_id_t            gas_id;
} rr_imsg_sel_nolocking_required_ind_t;

#endif /*FEATURE_LOCK_ARFCN*/

#if defined (FEATURE_WCDMA) || defined (FEATURE_GSM_TDS) || defined(FEATURE_LTE)
typedef struct
{
  rr_message_header_t header;
  rr_cco_status_e     status;
} rr_imsg_x2g_cco_status_ind_t;

#endif /* FEATURE_WCDMA || FEATURE_GSM_TDS || FEATURE_LTE */

typedef struct
{
  rr_message_header_t header;
  rr_event_T          rr_event;
  boolean             priority_over_other_imsg;
} rr_imsg_rr_event_ind_t;

typedef struct
{
  rr_message_header_t header;
  mm_rr_msgs_u        msg;
} rr_imsg_saved_msg_ind_t;

typedef struct
{
  rr_message_header_t header;
  boolean purge_gmm_signalling;
  rr_cb_fn_t confirm_cb_fn;
} rr_imsg_ps_access_req_t;

typedef struct
{
  rr_message_header_t header;
  boolean del_all_pdus;
  rr_cb_fn_t confirm_cb_fn;
} rr_imsg_no_ps_access_req_t;

typedef struct
{
  rr_message_header_t header;
  rr_gprs_ia_params_t params;
  rr_gprs_ia_cb_fn_t cb_fn;
} rr_imsg_gprs_ia_ind_t;

typedef struct
{
  rr_message_header_t header;
} rr_imsg_msc_change_ind_t;

typedef struct
{
  rr_message_header_t header;
  rr_page_info_t page_info;
} rr_imsg_page_ind_t;

typedef struct
{
  rr_message_header_t header;
  paging_mode_T old_page_mode;
  paging_mode_T new_page_mode;
} rr_imsg_page_mode_ind_t;

#ifdef FEATURE_MODEM_CONFIG_REFRESH
typedef struct
{
  rr_message_header_t header;
} rr_imsg_geran_nv_refresh_ind_t;
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

#ifdef FEATURE_QSH_EVENT_METRIC
typedef struct
{
  rr_message_header_t header;
  qsh_client_cb_params_s cb_params;
} rr_imsg_qsh_metric_config_ind_t;

typedef struct
{
  rr_message_header_t header;
  qsh_client_cb_params_s cb_params;
} rr_imsg_qsh_metric_timer_expiry_ind_t;
#endif // FEATURE_QSH_EVENT_METRIC

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
typedef struct
{
  rr_message_header_t header;
  qsh_client_cb_params_s cb_params;
} rr_imsg_qsh_event_config_ind_t;
#endif // FEATURE_QSH_EVENT_NOTIFY_TO_QSH

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif // FEATURE_QSH_EVENT_NOTIFY_HANDLER

typedef union
{
  rr_message_header_t                  header;
  rr_imsg_rr_event_ind_t               rr_event_ind;
  rr_imsg_saved_msg_ind_t              saved_msg_ind;
  rr_imsg_ps_access_req_t              ps_access_req;
  rr_imsg_no_ps_access_req_t           no_ps_access_req;
  rr_imsg_gprs_ia_ind_t                gprs_ia_ind;
  rr_imsg_msc_change_ind_t             msc_change_ind;
  rr_imsg_page_ind_t                   page_ind;
  rr_imsg_page_mode_ind_t              page_mode_ind;
#ifdef FEATURE_MODEM_CONFIG_REFRESH
  rr_imsg_geran_nv_refresh_ind_t       geran_nv_refresh_ind;
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
#ifdef FEATURE_LOCK_ARFCN
  rr_imsg_sel_start_locking_ind_t      sel_start_locking_ind;
  rr_imsg_sel_start_unlocking_ind_t    sel_start_unlocking_ind;
  rr_imsg_sel_nolocking_required_ind_t sel_start_nolocking_required_ind;
#endif /*FEATURE_LOCK_ARFCN*/
#ifdef FEATURE_QSH_EVENT_METRIC
  rr_imsg_qsh_metric_config_ind_t        qsh_metric_config_ind;
  rr_imsg_qsh_metric_timer_expiry_ind_t  qsh_metric_timer_expiry_ind;
#endif // FEATURE_QSH_EVENT_METRIC
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  rr_imsg_qsh_event_config_ind_t         qsh_event_config_ind;
#endif // FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif // FEATURE_QSH_EVENT_NOTIFY_HANDLER
} rr_imsg_u;

/* Union of all messages. */
typedef union
{
  rr_imsg_u imsg;
} rr_msg_u;

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/

/*!
 * \brief Returns a text string representation of a RR IMSG.
 * 
 * \param imsg 
 * 
 * \return const char* 
 */
extern const char * rr_imsg_name(const rr_imsg_e imsg);

/*!
 * \brief Sends a message without payload to RR.
 * 
 * \param imsg - The unique message identifier for the message.
 * \param gas_id - GAS-ID.
 */
extern void rr_send_imsg(rr_imsg_e imsg, const gas_id_t gas_id);

/*!
 * \brief Sends RR_IMSG_RR_EVENT_IND to RR.
 * 
 * \param rr_event - The RR event to be included in the message.
 * \param gas_id - GAS-ID.
 */
extern void rr_send_priority_imsg_rr_event_ind(
  rr_event_T rr_event,
  const gas_id_t gas_id
);

/**
  @brief Sends RR_IMSG_RR_EVENT_IND to RR.

  @param rr_event The RR event to be included in the message.
 */
/*!
 * \brief Sends RR_IMSG_RR_EVENT_IND to RR.
 * 
 * \param rr_event - The RR event to be included in the message.
 * \param gas_id - GAS-ID.
 */
extern void rr_send_imsg_rr_event_ind(
  rr_event_T rr_event,
  const gas_id_t gas_id
);

/*!
 * \brief Sends RR_IMSG_RR_MSG_IND to RR.
 * 
 * \param msg - Message to be encapsulated and sent into RR-CONTROL.
 * \param gas_id - GAS-ID.
 */
extern void rr_send_imsg_saved_msg_ind(
  mm_rr_msgs_u *msg,
  const gas_id_t gas_id
);

/*!
 * \brief Sends RR_IMSG_PS_ACCESS_REQ to RR.
 * 
 * \param purge_gmm_signalling - TRUE if GMM signalling PDUs should be deleted; FALSE otherwise.
 * \param confirm_cb_fn - Call-back function to be called upon completion (NULL if not required).
 * \param gas_id - GAS-ID.
 */
extern void rr_send_imsg_ps_access_req(
  boolean purge_gmm_signalling,
  rr_cb_fn_t confirm_cb_fn,
  const gas_id_t gas_id
);

/*!
 * \brief RR_IMSG_NO_PS_ACCESS_REQ to RR. 
 * 
 * \param del_all_pdus - TRUE if all PDUs should be deleted; FALSE otherwise.
 * \param confirm_cb_fn - Call-back function to be called upon completion (NULL if not required).
 * \param gas_id - GAS-ID.
 */
extern void rr_send_imsg_no_ps_access_req(
  boolean del_all_pdus,
  rr_cb_fn_t confirm_cb_fn,
  const gas_id_t gas_id
);

/*!
 * \brief Sends RR_IMSG_GPRS_IA_IND to RR.
 * 
 * \param ia_params (in)
 * \param cb_fn (in)
 * \param gas_id (in)
 */
extern void rr_send_imsg_gprs_ia_ind(rr_gprs_ia_params_t ia_params, rr_gprs_ia_cb_fn_t cb_fn, const gas_id_t gas_id);

/*!
 * \brief Sends RR_IMSG_PAGE_IND to RR.
 * 
 * \param page_info_ptr - Container of information about the page.
 * \param gas_id - GAS-ID.
 */
extern void rr_send_imsg_page_ind(
  rr_page_info_t *page_info_ptr,
  const gas_id_t gas_id
);

/*!
 * \brief Sends RR_IMSG_PAGE_MODE_IND to RR.
 * 
 * \param old_page_mode - Previous Page Mode.
 * \param new_page_mode - New Page Mode.
 * \param gas_id - GAS-ID.
 */
extern void rr_send_imsg_page_mode_ind(
  paging_mode_T old_page_mode,
  paging_mode_T new_page_mode,
  const gas_id_t gas_id
);

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*!
 * \brief Sends RR_IMSG_GERAN_NV_REFRESH_IND to RR.
 * 
 * \param gas_id - GAS-ID.
 */
extern void rr_send_imsg_geran_nv_refresh_ind(
  const gas_id_t gas_id
);
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

#ifdef FEATURE_LOCK_ARFCN
/*!
 * \brief Sends RR_IMSG_SELECTION_START_LOCKING_IND to RR.
 * \param locking_ARFCN .
 * \param gas_id - GAS-ID.
 */

extern void rr_send_imsg_sel_locking_ind(
  ARFCN_T        locking_ARFCN,
  const gas_id_t gas_id
);

/*!
 * \brief Sends RR_IMSG_SELECTION_START_UNLOCKING_IND to RR.
 * \param gas_id - GAS-ID.
 */

extern void rr_send_imsg_sel_unlocking_ind(
  const gas_id_t gas_id
);

/*!
 * \brief Sends RR_IMSG_SELECTION_NO_LOCKING_REQUIRED to RR.
 * \param gas_id - GAS-ID.
 */

extern void rr_send_imsg_sel_nolocking_required(
  const gas_id_t gas_id
);

#endif /*FEATURE_LOCK_ARFCN*/

#ifdef FEATURE_QSH_EVENT_METRIC
/*!
 * \brief Sends RR_IMSG_QSH_METRIC_CONFIG_IND to RR.
 * 
 * \param metric_config_ptr
 */
extern void rr_send_imsg_qsh_metric_config_ind(qsh_client_cb_params_s *cb_params_ptr);

/*!
 * \brief Sends RR_IMSG_QSH_METRIC_TIMER_EXPIRY_IND to RR.
 * 
 * \param metric_timer_expiry_ptr
 */
extern void rr_send_imsg_qsh_metric_timer_expiry_ind(qsh_client_cb_params_s *cb_params_ptr);
#endif // FEATURE_QSH_EVENT_METRIC

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*!
 * \brief Sends RR_IMSG_QSH_EVENT_CONFIG_IND to RR.
 * 
 * \param cb_params_ptr
 */
extern void rr_send_imsg_qsh_event_config_ind(qsh_client_cb_params_s *cb_params_ptr);
#endif  //FEATURE_QSH_EVENT_NOTIFY_TO_QSH

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif  //FEATURE_QSH_EVENT_NOTIFY_HANDLER

#endif /* #ifndef RR_IF_H */

/* EOF */

