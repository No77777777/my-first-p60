/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            Q M I    U I M    H T T P    S T U B S


GENERAL DESCRIPTION

  This source file contains the stub functions for the QMI UIM HTTP service

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2016 QUALCOMM Technologies, Incorporated and its licensors.
All Rights Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_http_stubs.c#1 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ks      Adding stubs for the QMI UIM HTTP service

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "rex.h"
#include "qmi_idl_lib.h"
#include "common_v01.h"
#include "ipc_policy.h"

#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */
#include "lpalib.h"
#include "qmi_uim_http.h"

/*===========================================================================
  FUNCTION QMI_UIM_HTTP_SERVICE_INIT

  DESCRIPTION
    Called to start the QMI UIM HTTP service when the HTTP modem task is up

  PARAMETERS
    TCB and SIG

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_http_service_init (
  rex_tcb_type    * tcb_ptr,
  rex_sigs_type     sig
)
{
  (void) tcb_ptr;
  (void) sig;
} /* qmi_uim_http_service_init */


/*===========================================================================
  FUNCTION QMI_UIM_HTTP_HANDLE_QCSI_SIG

  DESCRIPTION
    Function is called when a QCSI signal is received by the QMI UIM HTTP
    service. It will result in the calling of one of the QMI UIM HTTP
    request handlers.

  PARAMETERS
    tcb
    sig

  RETURN VALUE
    int

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_http_handle_qcsi_sig (
  rex_tcb_type *tcb,
  rex_sigs_type sig
)
{
  (void) tcb;
  (void) sig;  
} /* qmi_uim_http_handle_qcsi_sig */


/*===========================================================================
   FUNCTION:      QMI_UIM_HTTP_CMD_Q_SIG_HANDLER

  DESCRIPTION:
    QMI UIM HTTP CMD Q signal handler

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     Returns TRUE if command queue is empty
     FALSE if there are more commands to process

   SIDE EFFECTS:
     None
===========================================================================*/
boolean qmi_uim_http_cmd_q_sig_handler (
  void
)
{
  return TRUE;
} /* qmi_uim_http_cmd_q_sig_handler */


/*===========================================================================
   FUNCTION:      QMI_UIM_HTTP_SERVICE_REGISTER

   DESCRIPTION:
     This function registers the calling task with the UIM HTTP service.
     The function returns immediately and the service sends events with status
     of the connection

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_http_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_http_result_type qmi_uim_http_service_register (
  qmi_uim_http_service_registry_id_type    *service_id
)
{
  (void) service_id;
  return QMI_UIM_HTTP_ERROR;
} /* qmi_uim_http_service_register */


/*===========================================================================
   FUNCTION:      QMI_UIM_HTTP_SEND_TRANSACTION_REQUEST

   DESCRIPTION:
     This function allows the calling task to send an http request via the QMI UIM
     HTTP service

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_http_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_http_result_type qmi_uim_http_send_transaction_request (
  const qmi_uim_http_transaction_req_data_type  * qmi_uim_http_transaction_ptr
)
{
  (void) qmi_uim_http_transaction_ptr; 
  return QMI_UIM_HTTP_ERROR;
} /* qmi_uim_http_send_transaction_request */


/*===========================================================================
   FUNCTION:      QMI_UIM_HTTP_SERVICE_RELEASE

   DESCRIPTION:
     This function un-registers the calling task from the QMI UIM HTTP
     service.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     qmi_uim_http_result_type

   SIDE EFFECTS:
     None
=============================================================================*/
qmi_uim_http_result_type qmi_uim_http_service_release (
  qmi_uim_http_service_registry_id_type    service_id
)
{
  (void) service_id;
  return QMI_UIM_HTTP_ERROR;
} /* qmi_uim_http_service_release */


/*=========================================================================== 
  FUNCTION QMI_UIM_HTTP_DEREG 
  
  DESCRIPTION 
    Called to deregister the QMI UIM http as QMI service and as SAP client on 
    reception of task stop signal 
  
  PARAMETERS 
    None 
 
  RETURN VALUE 
    None 
  
  DEPENDENCIES 
    None 
  
  SIDE EFFECTS 
    None 
===========================================================================*/ 
void qmi_uim_http_dereg( 
  void 
)
{
}/* qmi_uim_http_dereg */
