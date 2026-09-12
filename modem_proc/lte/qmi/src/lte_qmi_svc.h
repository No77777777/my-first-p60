#ifndef LTE_QMI_SVC_H
#define LTE_QMI_SVC_H
/*==============================================================================

                LTE_QMI_SVC.H

DESCRIPTION
  This file contains functionality that is related to the LTE QMI service

Copyright (c) 2013 - 2015 Qualcomm Technologies Incorporated. 
All Rights Reserved.
Qualcomm Confidential and Proprietary
==============================================================================*/

/*==============================================================================

  $Header: 
  $DateTime: 
  $Author: 
 
  when        who    what, where, why
  --------    ---    -------------------------------------------------------------
  07/18/15    psv    created file.
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/

#include "comdef.h"
#include "rex.h"
#include "qmi_csi.h"
#include "qmi_sap.h"
#include <msgr.h>
#include <msgr_umid.h>

#include "lte_v01.h"

#ifdef FEATURE_LTE_DISCOVERY
#include "prose_ext_msg.h"
#include <ds_lted_ext_msg.h>
#endif

#include "lte.h"
#include "lte_qmi_task.h"
#include "lte_qmi_utils.h"

#include "lte_rrc_ext_msg.h"
#include "lte_rrc_diag.h"
#include "lte_rrc_mem_util.h"


/*==============================================================================

                           EXTERNAL MACRO DEFINITIONS

==============================================================================*/



/*! Signals component of the OS */
typedef rex_sigs_type lte_qmi_signal_t;

/*! TCB Component of the OS */
typedef rex_tcb_type lte_qmi_tcb_t;


/*! @brief LTE QMI Signal struct for other tasks to raise signal */
typedef struct
{
  lte_qmi_tcb_t     *tcb;
  lte_qmi_signal_t  signal_id;
} lte_qmi_signal_s;

/*==============================================================================

                           EXTERNAL FUNCTION DEFINITIONS

==============================================================================*/

/*==============================================================================
FUNCTION LTE_QMI_SVC_INIT()

DESCRIPTION
  This function is used to initialize any data structures that we maintain for
  the LTE QMI service.
 
  This is called as a part of the init thread and should contain minimal
  processing
 
PARAMETERS
  None

RETURN VALUE
  None

DEPENDENCIES
  This function has to be called during LTE task initialization.
 
SIDE EFFECTS
  None

==============================================================================*/
void lte_qmi_svc_init(void);

/*==============================================================================
FUNCTION LTE_QMI_SVC_START()

DESCRIPTION
  This function is used to kick start the service?.
 
PARAMETERS
  None

RETURN VALUE
  Signals that this module expects to be set on LTE TCB

DEPENDENCIES
  This function has to be called during LTE task start.
 
SIDE EFFECTS
  None

==============================================================================*/
void lte_qmi_svc_start(void);

/*==============================================================================
FUNCTION LTE_QMI_SVC_SEND_IND()

DESCRIPTION
  This function is used to send an indication to the client on the apps proc.
  If the client handle is unavailable, this function does not cache the ind
  to be sent at a later point.
 

PARAMETERS
  msg_id       - Message identifier
  msg_ptr      - Pointer to structure containing the message
  msg_len      - Length of the message (size of the message structure)
 
RETURN VALUE
  E_SUCCESS on success
  Failure codes otherwise

DEPENDENCIES
  Currently, it will send the indication from the context of the
  caller
 
SIDE EFFECTS
  None

==============================================================================*/
int lte_qmi_svc_send_ind
(
  unsigned int                    msg_id,
  void                            *msg_ptr,
  unsigned int                    msg_len
);

/*==============================================================================
FUNCTION LTE_QMI_SVC_PROCESS_SERVER_SIG()

DESCRIPTION
  This function is used to process the signal LTE_TASK_QMI_SERVER_SIG that is
  set on the LTE TCB. This signal is set whenever the server needs to respond
  to any request

PARAMETERS
  None
 
RETURN VALUE
  None

DEPENDENCIES
  This function can be called only from LTE QMI task
 
SIDE EFFECTS
  None

==============================================================================*/
void lte_qmi_svc_process_server_sig(void);

/*==============================================================================
FUNCTION LTE_QMI_SVC_REGISTER_HANDLER()

DESCRIPTION
  This function is used to register a callback with the QMI server. When the
  client sends a message the corresponding call back is called by the server.
 
  The callback invoked by the server handles the request and sends a response
  appropriately

PARAMETERS
  msg_id       - Message identifier for which handler is being registered
  handler_fn   - Pointer to the handler function 
 
RETURN VALUE
  None

DEPENDENCIES
  This function can be called only from LTE task
 
SIDE EFFECTS
  None

==============================================================================*/
int lte_qmi_svc_register_handler
(
  unsigned int                        msg_id,
  void                                *handler_fn
);

/*==============================================================================
FUNCTION LTE_QMI_SVC_SEND_RESPONSE()

DESCRIPTION
  This function is used to send a response to a request sent by the client on
  the remote processor. This is a wrapper around the QMI framework.

PARAMETERS 
  req_handle   - The handle to the request initiated by the client 
  msg_id       - Message identifier for which response is being sent
  resp_ptr     - Pointer to the response message
  resp_len     - Length of the response
 
RETURN VALUE
  LTE_SUCCESS on success
  Failure codes otherwise

DEPENDENCIES
  None
 
SIDE EFFECTS
  None

==============================================================================*/
qmi_csi_error lte_qmi_svc_send_response
(
  void                     *req_handle,
  unsigned int             msg_id,
  void                     *resp_ptr,
  unsigned int             resp_len
);


qmi_req_handle lte_qmi_svc_get_req_handle(void);


/*============================================================================== 
  Handler function declaration
==============================================================================*/

qmi_csi_cb_error lte_qmi_handle_indication_reg_req
(
  qmi_req_handle req_handle,
  qmi_lte_indication_reg_req_msg_v01 *ind_reg_req
);

#ifdef FEATURE_LTE_DISCOVERY
qmi_csi_cb_error lte_qmi_handle_set_lted_cfg_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_set_lted_config_req_msg_v01 *lted_cfg_req
);

qmi_csi_cb_error lte_qmi_handle_get_lted_cfg_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_get_lted_config_req_msg_v01 *lted_cfg_req
);

qmi_csi_cb_error lte_qmi_handle_set_lted_cat_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_set_lted_category_req_msg_v01 *set_lted_cat_req
);

qmi_csi_cb_error lte_qmi_handle_get_lted_cat_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_get_lted_category_req_msg_v01 *get_lted_cat_req
);

qmi_csi_cb_error lte_qmi_handle_lted_terminate_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_terminate_req_msg_v01 *lted_term_req
);

qmi_csi_cb_error lte_qmi_handle_lted_get_service_status_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_get_service_status_req_msg_v01 *get_serv_status_req
);

qmi_csi_cb_error lte_qmi_handle_lted_publish_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_publish_req_msg_v01 *publish_req
);

qmi_csi_cb_error lte_qmi_handle_lted_subscribe_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_subscribe_req_msg_v01 *subscribe_req
);

qmi_csi_cb_error lte_qmi_handle_lted_cancel_publish_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_cancel_publish_req_msg_v01 *can_publish_req
);

qmi_csi_cb_error lte_qmi_handle_lted_cancel_subscribe_req
(
  qmi_req_handle req_handle,
  qmi_lte_disc_cancel_subscribe_req_msg_v01 *can_subscribe_req
);
#endif /* FEATURE_LTE_DISCOVERY */


qmi_csi_cb_error lte_qmi_handle_bind_subscription_req
(
  qmi_req_handle req_handle,
  qmi_lte_bind_subscription_req_msg_v01 *bind_subscription_req
);

qmi_csi_cb_error lte_qmi_handle_get_subscription_info_req
(
  qmi_req_handle req_handle,
  qmi_lte_get_subscription_info_req_msg_v01 *get_bind_subs_req
);


#endif /* LTE_QMI_SVC_H */
