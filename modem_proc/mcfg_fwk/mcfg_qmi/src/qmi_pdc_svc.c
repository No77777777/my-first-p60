/*==============================================================================

    Q M I   P E R S I S T E N T   D E V I C E   C O N F I G   S E R V I C E

GENERAL DESCRIPTION
  Persistent Device Configuration (PDC) QMI service implementation

Copyright (c) 2012-2015 by QUALCOMM Technologies Incorporated.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_qmi/src/qmi_pdc_svc.c#2 $ 
$DateTime: 2021/08/05 08:47:13 $ 
$Author: pwbldsvc $ 
$Change: 32223212 $ 

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
01/28/13      Added support to query commands/TLVs supported by service 
04/30/10 mj   Initial creation

==============================================================================*/

#include "comdef.h"

#include "msg.h"

#include "qmi_csi.h"
#include "qmi_sap.h"

#include "persistent_device_configuration_v01.h"
#include "qmi_pdc_msg.h"
#include "persistent_device_configuration_impl_v01.h"

#include "mcfg_osal.h"
#include "mcfg_int.h"
#include "mcfg_utils.h"
#include "mcfg_common.h"
#include "mcfg_trl.h"
#include "mcfg_validate.h"
#include "mcfg_refresh_i.h"
#include "mcfg_uim.h"
#include "mcfg_setting.h"
#include "mcfg_client.h"

/* Added for dummy XML*/
#include "fs_public.h"                          /* For handling EFS files */
#include "fs_sys_types.h"
/* -------------------*/
#include <string.h>
#include "mqcsi_log.h"
#include "mcfg_svc.h"

/*==============================================================================

                  DEFINITIONS AND DECLARATIONS FOR MODULE

==============================================================================*/
/*==============================================================================
  Constants and Macros
==============================================================================*/
#ifdef _WIN32
#define UNUSED_ATTRIBUTE 
#else
#define UNUSED_ATTRIBUTE __attribute__((unused))
#endif

/* Macro to validate config type */
#define IS_VALID_PDC_CONFIG_TYPE(type) ((type) == PDC_CONFIG_TYPE_MODEM_PLATFORM_V01 || \
                                        (type) == PDC_CONFIG_TYPE_MODEM_SW_V01)
/* Macro to validate subscription ID */
#define IS_VALID_MCFG_SUBSCRIPTION_ID(sub) (MCFG_SUB_ID_FIRST <= (sub) && (sub) < MCFG_NUM_OF_SUB_ID)

/*==============================================================================
  Typedefs
==============================================================================*/
/* request handler fucntion prototype */
typedef qmi_csi_cb_error (* const pdc_service_req_handler)
                         (void *connection_handle, qmi_req_handle req_handle, unsigned int msg_id, void *req_c_struct, unsigned int req_c_struct_len, void *service_cookie);

/*==============================================================================
  Variables
==============================================================================*/


/*============================================================================== 
 
                 PRIVATE FUNCTION FORWARD DECLARATIONS
 
==============================================================================*/
/* message handler functions */
static qmi_csi_cb_error pdc_handle_reset_req(void *, qmi_req_handle, unsigned int, void *, unsigned int, void *)UNUSED_ATTRIBUTE;
static qmi_csi_cb_error pdc_handle_req(void *, qmi_req_handle, unsigned int, void *, unsigned int, void *)UNUSED_ATTRIBUTE;
static qmi_csi_cb_error pdc_handle_indication_register_req(void *, qmi_req_handle, unsigned int, void *, unsigned int, void *)UNUSED_ATTRIBUTE;
static qmi_csi_cb_error pdc_handle_deactivate_config_req(void *, qmi_req_handle, unsigned int, void *, unsigned int, void *)UNUSED_ATTRIBUTE;
static qmi_csi_cb_error pdc_handle_get_config_limits_req(void *, qmi_req_handle, unsigned int, void *, unsigned int, void *)UNUSED_ATTRIBUTE;
static qmi_csi_cb_error pdc_handle_get_default_config_info_req(void *, qmi_req_handle, unsigned int, void *, unsigned int, void *)UNUSED_ATTRIBUTE;
static void pdc_send_deactivate_config_ind(pdc_client_type *, pdc_deactivate_config_req_msg_v01 *)UNUSED_ATTRIBUTE;
static void pdc_send_get_config_limits_ind(pdc_client_type *, pdc_get_config_limits_req_msg_v01 *)UNUSED_ATTRIBUTE;
static void pdc_send_get_default_config_info_ind(pdc_client_type *, pdc_get_default_config_info_req_msg_v01 *)UNUSED_ATTRIBUTE;

/* Table to handle the dispatch of request handler functions. */
static pdc_service_req_handler pdc_service_req_handle_table[] =
{
#ifndef REMOVE_QMI_PDC_RESET_V01
  pdc_handle_reset_req,                    /* Request handler for message ID 0x0000 */
  pdc_handle_reset_req,                    /* Request handler for message ID 0x0001 */  // TODO: remove!
  //NULL,                                  /* Request handler for message ID 0x0001 */  // TODO: put back!
#else
  NULL,
  NULL,
#endif
  NULL,                                    /* Request handler for message ID 0x0002 */
  NULL,                                    /* Request handler for message ID 0x0003 */
  NULL,                                    /* Request handler for message ID 0x0004 */
  NULL,                                    /* Request handler for message ID 0x0005 */
  NULL,                                    /* Request handler for message ID 0x0006 */
  NULL,                                    /* Request handler for message ID 0x0007 */
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, /* Request handler for message ID 0x0008 - 0x000F*/
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, /* Request handler for message ID 0x0010 - 0x0017*/
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, /* Request handler for message ID 0x0018 - 0x001F*/
#ifndef REMOVE_QMI_PDC_INDICATION_REGISTER_V01
  pdc_handle_indication_register_req,      /* Request handler for message ID 0x0020 */
#else
  NULL,                                    /* Request handler for message ID 0x0020 */
#endif
  NULL,                                    /* Request handler for message ID 0x0021 */
#ifndef REMOVE_QMI_PDC_GET_SELECTED_CONFIG_V01
  pdc_handle_req,                          /* Request handler for message ID 0x0022 */
#else
  NULL,                                    /* Request handler for message ID 0x0022 */
#endif
#ifndef REMOVE_QMI_PDC_SET_SELECTED_CONFIG_V01
  pdc_handle_req,                          /* Request handler for message ID 0x0023 */
#else
  NULL,                                    /* Request handler for message ID 0x0023 */
#endif
#ifndef REMOVE_QMI_PDC_LIST_CONFIGS_V01
  pdc_handle_req,                          /* Request handler for message ID 0x0024 */
#else
  NULL,                                    /* Request handler for message ID 0x0024 */
#endif
#ifndef REMOVE_QMI_PDC_DELETE_CONFIG_V01
  pdc_handle_req,                        /* Request handler for message ID 0x0025 */
#else
  NULL,                                    /* Request handler for message ID 0x0025 */
#endif
#ifndef REMOVE_QMI_PDC_LOAD_CONFIG_V01
  pdc_handle_req,                          /* Request handler for message ID 0x0026 */
#else
  NULL,                                    /* Request handler for message ID 0x0026 */
#endif
#ifndef REMOVE_QMI_PDC_ACTIVATE_CONFIG_V01
  pdc_handle_req,                          /* Request handler for message ID 0x0027 */
#else
  NULL,                                    /* Request handler for message ID 0x0027 */
#endif
#ifndef REMOVE_QMI_PDC_GET_CONFIG_INFO_V01
  pdc_handle_req,                          /* Request handler for message ID 0x0028 */
#else
  NULL,                                    /* Request handler for message ID 0x0028 */
#endif
#ifndef REMOVE_QMI_PDC_GET_CONFIG_LIMITS_V01
  pdc_handle_get_config_limits_req,        /* Request handler for message ID 0x0029 */
#else
  NULL,                                    /* Request handler for message ID 0x0029 */
#endif
#ifndef REMOVE_QMI_PDC_GET_DEFAULT_CONFIG_INFO_V01
  pdc_handle_get_default_config_info_req,  /* Request handler for message ID 0x002A */
#else
  NULL,                                    /* Request handler for message ID 0x002A */
#endif
#ifndef REMOVE_QMI_PDC_DEACTIVATE_CONFIG_V01
  pdc_handle_deactivate_config_req,        /* Request handler for message ID 0x002B */
#else
  NULL,                                    /* Request handler for message ID 0x002B */
#endif
#ifndef REMOVE_QMI_PDC_VALIDATE_CONFIG_V01
  pdc_handle_req,                          /* Request handler for message ID 0x002C */
#else
  NULL,                                    /* Request handler for message ID 0x002C */
#endif
#ifndef REMOVE_QMI_PDC_GET_FEATURE_V01
  pdc_handle_req,                          /* Request handler for message ID 0x002D */
#else
  NULL,                                    /* Request handler for message ID 0x002D */
#endif
#ifndef REMOVE_QMI_PDC_SET_FEATURE_V01
  pdc_handle_req,                          /* Request handler for message ID 0x002E */
#else
  NULL,                                    /* Request handler for message ID 0x002E */
#endif
};


/*==============================================================================

                     INTERNAL FUNCTION DECLARATIONS

==============================================================================*/

/*==============================================================================

  FUNCTION pdc_connect_cb

  DESCRIPTION
    Callback when a client connects to the server

  DEPENDENCIES
    None

  PARAMETERS
    client_handle     [in]  - handle used by QCSI to idenfity services
    service_cookie    [in]  - cookie registered with QCSI during qmi_csi_register
    connection_handle [out] - handle for QMI service connection
 
  RETURN VALUE
    Status of connect

  SIDE EFFECTS
    None

==============================================================================*/
static qmi_csi_cb_error pdc_connect_cb
(
  qmi_client_handle  client_handle,
  void              *service_cookie,
  void             **connection_handle
)
{
  uint8 i;
  pdc_service_type *pdc_svc;
  pdc_client_type  *pdc_client;

  /*-----------------------------------------------------------------------*/

  pdc_client = NULL;
  pdc_svc = (pdc_service_type *) service_cookie;
  
  if ( !pdc_svc )
  {
    MSG_ERROR( "pdc_connect_cb(): pdc_svc NULL", 0, 0, 0 );
    return QMI_CSI_CB_INTERNAL_ERR;
  }
  
 if ( !connection_handle )
  {
    MSG_ERROR( "pdc_connect_cb(): connection_handle NULL", 0, 0, 0 );
    return QMI_CSI_CB_INTERNAL_ERR;
  }

  /* Find empty client slot */
  for ( i = 0; i < PDC_CLIENTS_MAX; i++ )
  {
    if (pdc_svc->pdc_clients[i].connected == FALSE)
    {
      /* Found empty slot */
      pdc_client = &pdc_svc->pdc_clients[i];
      break;
    }
  }

  if ( !pdc_client )
  {
    MSG_ERROR( "pdc_connect_cb(): No empty client slots found", 0, 0, 0 );
    return QMI_CSI_CB_NO_MEM;
  }

  /* Clear out client struct memory */
  memset( pdc_client, 0, sizeof(pdc_client_type) );

  /* Fill out client slot data with default values */
  pdc_client->connected                 = TRUE;
  pdc_client->qmi_client_handle         = client_handle;
  pdc_client->event_reg.selected_config = FALSE;

  /* Return client handle */
  *connection_handle = pdc_client;
  
  return QMI_CSI_CB_NO_ERR;

} /* pdc_connect_cb() */


/*==============================================================================

  FUNCTION pdc_disconnect_cb

  DESCRIPTION
    Callback when a client disconnects from the server

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    connection_handle [in] - handle for QMI client connection
    service_cookie    [in] - cookie registered with QCSI during qmi_csi_register

  RETURN VALUE
    None

  SIDE EFFECTS
    None

==============================================================================*/
static void pdc_disconnect_cb
(
  void  *connection_handle,
  void  *service_cookie
)
{
  pdc_client_type  *pdc_client;
  pdc_service_type *service;
  pdc_list_config_cache_type *list_config_cache = NULL;

  /*-----------------------------------------------------------------------*/

  pdc_client = (pdc_client_type *) connection_handle;
  
  if ( pdc_client )
  {
    pdc_client = (pdc_client_type *) connection_handle;

    /* Free client slot from being connected */
    pdc_client->connected = FALSE;

    /* reset config cache if client handle matches */
    service  = (pdc_service_type *) service_cookie;
    if (pdc_client->qmi_client_handle == service->load_config_cache.qmi_client_handle)
    {
      mcfg_svc_default_load_config_cache(&service->load_config_cache);
    }

    mcfg_svc_get_list_config_cache(service, (mcfg_svc_client_type *)&pdc_client->qmi_client_handle, &list_config_cache);
    mcfg_svc_default_list_config_cache(list_config_cache);
    
  } else {
    MSG_ERROR( "pdc_disconnect_cb(): pdc_client NULL", 0, 0, 0 );
  }

} /* pdc_disconnect_cb() */


/*==============================================================================

  FUNCTION pdc_process_req_cb

  DESCRIPTION
    Handles PDC client requests

  DEPENDENCIES
    Service must be initialized

  PARAMETERS
    connection_handle [in] - handle for QMI service connection
    req_handle        [in] - handle used to identify the transaction and client
    msg_id            [in] - message ID for this message
    req_c_struct      [in] - C data structure for this response
    req_c_struct_len  [in] - C data structure length
    service_cookie    [in] - cookie registered with QCSI during qmi_csi_register

  RETURN VALUE
    Status of QMI request

  SIDE EFFECTS
    None

==============================================================================*/
qmi_csi_cb_error pdc_process_req_cb
(
  void           *connection_handle,
  qmi_req_handle  req_handle,
  unsigned int    msg_id,
  void           *req_c_struct,
  unsigned int    req_c_struct_len,
  void           *service_cookie
)
{
  qmi_csi_cb_error retval = QMI_CSI_CB_NO_ERR;
  pdc_service_type *service;
  qmi_response_type_v01 *response;

  /*-----------------------------------------------------------------------*/
  
  if(!connection_handle || !service_cookie)
  {
    MSG_ERROR( "pdc_process_req_cb(): NULL pointer", 0, 0, 0 );
    return QMI_CSI_CB_INTERNAL_ERR;
  }
  
  service  = (pdc_service_type *)       service_cookie;
  response = (qmi_response_type_v01 *) &service->pdc_resp;

  MSG_MED( "pdc_process_req_cb(): processing MSG ID %d", msg_id, 0, 0 );
  if ((msg_id < ((int) sizeof(pdc_service_req_handle_table) / sizeof(*pdc_service_req_handle_table)))
      &&
      (pdc_service_req_handle_table[msg_id]))
  {
    /* If the jump table has a valid entry at this message ID, call the handler function */
    retval = pdc_service_req_handle_table[msg_id] (connection_handle,req_handle,msg_id,req_c_struct,req_c_struct_len,service_cookie);
  } else {
    /* if the message ID doesn't exist format an error response */
    MSG_ERROR( "pdc_process_req_cb(): msg_id unknown (%d)", msg_id, 0, 0 );

    /* Clear the response memory */
    memset( response, 0, sizeof(qmi_response_type_v01) );

    /* Send the response now (there is no message handler to send the resp) */
    response->result = QMI_RESULT_FAILURE_V01;
    response->error  = QMI_ERR_INVALID_MESSAGE_ID_V01;

    qmi_csi_send_resp( req_handle,
                       msg_id,
                       response,
                       sizeof(qmi_response_type_v01) );
  }

  MSG_MED( "pdc_process_req_cb(): returning %d", retval, 0, 0 );
  
  return retval;

} /* pdc_process_req_cb() */


/*==============================================================================

  FUNCTION pdc_handle_reset_req

  DESCRIPTION
    Handles the request to reset PDC client

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    connection_handle [in] - handle for QMI service connection
    req_handle        [in] - handle used to identify the transaction and client
    msg_id            [in] - message ID for this message
    req_c_struct      [in] - C data structure for this response
    req_c_struct_len  [in] - C data structure length
    service_cookie    [in] - cookie registered with QCSI during qmi_csi_register

  RETURN VALUE
    Status of QMI request

  SIDE EFFECTS
    None

==============================================================================*/
static qmi_csi_cb_error pdc_handle_reset_req
(
  void           *connection_handle,
  qmi_req_handle  req_handle,
  unsigned int    msg_id,
  void           *req_c_struct,
  unsigned int    req_c_struct_len,
  void           *service_cookie
)
{
  pdc_service_type *service;
  pdc_client_type  *client;
  /* there is no request TLV's */
  pdc_reset_resp_msg_v01 *response;
  pdc_list_config_cache_type *list_config_cache = NULL;

  /*-----------------------------------------------------------------------*/
  
  service  = (pdc_service_type *) service_cookie;
  client   = (pdc_client_type *)  connection_handle;
  (void) req_c_struct;
  response = (pdc_reset_resp_msg_v01 *) &service->pdc_resp;
  
  /* connection_handle and service_cookie have already been checked by pdc_process_req_cb() */
  
  /* Request is not used in this function so no need to check it */
  mcfg_svc_default_load_config_cache(&service->load_config_cache);
  mcfg_svc_get_list_config_cache(service, (mcfg_svc_client_type *)&client->qmi_client_handle, &list_config_cache);
  mcfg_svc_default_list_config_cache(list_config_cache);

  /* Init response data */
  memset( response, 0, sizeof(pdc_reset_resp_msg_v01) );

  /* Validate request */
  
  /* Perform requested action */
  client->event_reg.selected_config = FALSE;
  
  response->resp.result = QMI_RESULT_SUCCESS_V01;
  response->resp.error = QMI_ERR_NONE_V01;

  /* Send the response */
  qmi_csi_send_resp( req_handle,
                     msg_id,
                     response,
                     sizeof(pdc_reset_resp_msg_v01) );

  return QMI_CSI_CB_NO_ERR;

} /* pdc_handle_reset_req() */


/*==============================================================================

  FUNCTION pdc_handle_indication_register_req

  DESCRIPTION
    Handles the request to register for QMI_PDC indications

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    connection_handle [in] - handle for QMI service connection
    req_handle        [in] - handle used to identify the transaction and client
    msg_id            [in] - message ID for this message
    req_c_struct      [in] - C data structure for this response
    req_c_struct_len  [in] - C data structure length
    service_cookie    [in] - cookie registered with QCSI during qmi_csi_register

  RETURN VALUE
    Status of QMI request

  SIDE EFFECTS
    Registers a client indication reporting and causes an indication to be sent
    ever time it is enabled.

==============================================================================*/
static qmi_csi_cb_error pdc_handle_indication_register_req
(
  void           *connection_handle,
  qmi_req_handle  req_handle,
  unsigned int    msg_id,
  void           *req_c_struct,
  unsigned int    req_c_struct_len,
  void           *service_cookie
)
{
  pdc_service_type *service;
  pdc_client_type  *client;
  pdc_indication_register_req_msg_v01  *request;
  pdc_indication_register_resp_msg_v01 *response;
  
  qmi_csi_error qmi_error;
  boolean send_config_change_ind;

  /*-----------------------------------------------------------------------*/
  
  service  = (pdc_service_type *) service_cookie;
  client   = (pdc_client_type *)  connection_handle;
  request  = (pdc_indication_register_req_msg_v01 *)   req_c_struct;
  response = (pdc_indication_register_resp_msg_v01 *) &service->pdc_resp;
  
  qmi_error = QMI_CSI_NO_ERR;
  send_config_change_ind = FALSE;
  
  /* connection_handle and service_cookie have already been checked by pdc_process_req_cb() */
  
  /* Check that the request data is present */
  if (!request)
  {
    MSG_ERROR( "pdc_handle_indication_register_req(): request pointer NULL", 0, 0, 0 );
    return QMI_CSI_CB_INTERNAL_ERR;
  }

  /* Init response data */
  memset( response, 0, sizeof(pdc_indication_register_resp_msg_v01) );
  response->resp.error = QMI_ERR_NONE_V01;

  /* Validate request */
  
  /* Perform requested action */
  if (request->reg_config_change_valid)
  {
    MSG_MED( "pdc_handle_indication_register_req(): handling reg_config_change TLV", 0, 0, 0 );
    client->event_reg.selected_config = request->reg_config_change;
    if (client->event_reg.selected_config)
    {
      send_config_change_ind = TRUE;
    }
  }

  MSG_MED( "pdc_handle_indication_register_req(): handling refresh event TLV", 0, 0, 0 );
  client->event_reg.refresh_event = TRUE;

  /* Set the response result based on the response error */  
  response->resp.result = QMI_RESULT_FROM_ERROR(response->resp.error);
  
  /* Send the response */
  qmi_csi_send_resp( req_handle,
                     msg_id,
                     response,
                     sizeof(pdc_indication_register_resp_msg_v01) );

  if (send_config_change_ind)
  {
    /* Send the IND to the client when the reporting is enabled using the cached
       IND that is stored in the service info */
    if (service->pdc_ind.config_change_valid)
    {
      MSG_MED( "pdc_handle_indication_register_req(): sending config_change IND", 0, 0, 0 );
      qmi_error = qmi_csi_send_ind(client->qmi_client_handle,
                                   QMI_PDC_CONFIG_CHANGE_IND_V01,
                                   &service->pdc_ind.config_change,
                                   service->pdc_ind.config_change_len);
    
      if ( qmi_error != QMI_CSI_NO_ERR )
      {
        MSG_ERROR( "pdc_handle_indication_register_req(): send_ind failed (%d)", qmi_error, 0, 0 );
        client->event_reg.selected_config = FALSE;
        response->resp.error = QMI_ERR_INTERNAL_V01;
      }
    } else {
      MSG_MED( "pdc_handle_indication_register_req(): no cached IND to send", 0, 0, 0 );
    }
  }
  
  return QMI_CSI_CB_NO_ERR;
} /* pdc_handle_indication_register_req() */


/*==============================================================================

  FUNCTION pdc_handle_deactivate_config_req

  DESCRIPTION
    Handles the request to deactivate the currently active device configuration

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    connection_handle [in] - handle for QMI service connection
    req_handle        [in] - handle used to identify the transaction and client
    msg_id            [in] - message ID for this message
    req_c_struct      [in] - C data structure for this response
    req_c_struct_len  [in] - C data structure length
    service_cookie    [in] - cookie registered with QCSI during qmi_csi_register

  RETURN VALUE
    Status of QMI request

  SIDE EFFECTS
    Resets the device

==============================================================================*/
static qmi_csi_cb_error pdc_handle_deactivate_config_req
(
  void           *connection_handle,
  qmi_req_handle  req_handle,
  unsigned int    msg_id,
  void           *req_c_struct,
  unsigned int    req_c_struct_len,
  void           *service_cookie
)
{
  pdc_service_type *service;
  pdc_client_type  *client;
  pdc_deactivate_config_req_msg_v01  *request;
  pdc_deactivate_config_resp_msg_v01 *response;

  /*-----------------------------------------------------------------------*/
  
  service  = (pdc_service_type *) service_cookie;
  client   = (pdc_client_type *)  connection_handle;
  request  = (pdc_deactivate_config_req_msg_v01 *)   req_c_struct;
  response = (pdc_deactivate_config_resp_msg_v01 *) &service->pdc_resp;
  
  /* connection_handle and service_cookie have already been checked by pdc_process_req_cb() */
  
  /* Check that the request data is present */
  if (!request)
  {
    MCFG_MSG_ERROR( "pdc_handle_deactivate_config_req(): request pointer NULL");
    return QMI_CSI_CB_INTERNAL_ERR;
  }

  /* Init response data */
  memset( response, 0, sizeof(pdc_deactivate_config_resp_msg_v01) );

  /* Validate request */
  if (!IS_VALID_PDC_CONFIG_TYPE(request->config_type))
  {
    MCFG_MSG_ERROR_1( "pdc_handle_deactivate_config_req(): invalid type %d", 
                      request->config_type );
    response->resp.error = QMI_ERR_NOT_SUPPORTED_V01;
  }
  else if (request->subscription_id_valid == TRUE && 
      !IS_VALID_MCFG_SUBSCRIPTION_ID(request->subscription_id))
  {
    MCFG_MSG_ERROR_1("pdc_handle_deactivate_config_req(): invalid subscription %lu", 
                     request->subscription_id);
    response->resp.error = QMI_ERR_NOT_SUPPORTED_V01;
  }
  else
  {
    response->resp.error = QMI_ERR_NONE_V01;
  }
  
  /* Set the response result based on the response error */  
  response->resp.result = QMI_RESULT_FROM_ERROR(response->resp.error);
  
  /* Send the response */
  qmi_csi_send_resp( req_handle,
                     msg_id,
                     response,
                     sizeof(pdc_deactivate_config_resp_msg_v01) );

  /* Perform requested action and send indication for valid request */
  if (response->resp.error == QMI_ERR_NONE_V01)
  {
    pdc_send_deactivate_config_ind(client, request);
  }

  return QMI_CSI_CB_NO_ERR;
} /* pdc_handle_deactivate_config_req() */


/*==============================================================================

  FUNCTION pdc_handle_get_config_limits_req

  DESCRIPTION
    Handles the request to query limits for storing configurations on the device

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    connection_handle [in] - handle for QMI service connection
    req_handle        [in] - handle used to identify the transaction and client
    msg_id            [in] - message ID for this message
    req_c_struct      [in] - C data structure for this response
    req_c_struct_len  [in] - C data structure length
    service_cookie    [in] - cookie registered with QCSI during qmi_csi_register

  RETURN VALUE
    Status of QMI request

  SIDE EFFECTS
    None

==============================================================================*/
static qmi_csi_cb_error pdc_handle_get_config_limits_req
(
  void           *connection_handle,
  qmi_req_handle  req_handle,
  unsigned int    msg_id,
  void           *req_c_struct,
  unsigned int    req_c_struct_len,
  void           *service_cookie
)
{
  pdc_service_type *service;
  pdc_client_type  *client;
  pdc_get_config_limits_req_msg_v01  *request;
  pdc_get_config_limits_resp_msg_v01 *response;

  /*-----------------------------------------------------------------------*/
  
  service  = (pdc_service_type *) service_cookie;
  client   = (pdc_client_type *)  connection_handle;
  request  = (pdc_get_config_limits_req_msg_v01 *)   req_c_struct;
  response = (pdc_get_config_limits_resp_msg_v01 *) &service->pdc_resp;
  
  /* connection_handle and service_cookie have already been checked by pdc_process_req_cb() */
  
  /* Check that the request data is present */
  if (!request)
  {
    MCFG_MSG_ERROR( "pdc_handle_get_config_limits_req(): request pointer NULL" );
    return QMI_CSI_CB_INTERNAL_ERR;
  }

  /* Init response data */
  memset( response, 0, sizeof(pdc_get_config_limits_resp_msg_v01) );

  /* Validate request */
  if (!IS_VALID_PDC_CONFIG_TYPE(request->config_type))
  {
    MCFG_MSG_ERROR_1( "pdc_handle_get_config_limits_req(): invalid type %d", 
                      request->config_type );
    response->resp.error = QMI_ERR_NOT_SUPPORTED_V01;
  }
  else
  {
    response->resp.error = QMI_ERR_NONE_V01;
  }
  
  /* Set the response result based on the response error */  
  response->resp.result = QMI_RESULT_FROM_ERROR(response->resp.error);
  
  /* Send the response */
  qmi_csi_send_resp( req_handle,
                     msg_id,
                     response,
                     sizeof(pdc_get_config_limits_resp_msg_v01) );

  /* Perform requested action and send indication for valid request */
  if (response->resp.error == QMI_ERR_NONE_V01)
  {
    pdc_send_get_config_limits_ind(client, request);
  }

  return QMI_CSI_CB_NO_ERR;
} /* pdc_handle_get_config_limits_req() */


/*==============================================================================

  FUNCTION pdc_handle_get_default_config_info_req

  DESCRIPTION
    Handles the request to get more info for a default device configuration

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    connection_handle [in] - handle for QMI service connection
    req_handle        [in] - handle used to identify the transaction and client
    msg_id            [in] - message ID for this message
    req_c_struct      [in] - C data structure for this response
    req_c_struct_len  [in] - C data structure length
    service_cookie    [in] - cookie registered with QCSI during qmi_csi_register

  RETURN VALUE
    Status of QMI request

  SIDE EFFECTS
    None

==============================================================================*/
static qmi_csi_cb_error pdc_handle_get_default_config_info_req
(
  void           *connection_handle,
  qmi_req_handle  req_handle,
  unsigned int    msg_id,
  void           *req_c_struct,
  unsigned int    req_c_struct_len,
  void           *service_cookie
)
{
  pdc_service_type *service;
  pdc_client_type  *client;
  pdc_get_default_config_info_req_msg_v01  *request;
  pdc_get_default_config_info_resp_msg_v01 *response;

  /*-----------------------------------------------------------------------*/
  
  service  = (pdc_service_type *) service_cookie;
  client   = (pdc_client_type *)  connection_handle;
  request  = (pdc_get_default_config_info_req_msg_v01 *)   req_c_struct;
  response = (pdc_get_default_config_info_resp_msg_v01 *) &service->pdc_resp;
  
  /* connection_handle and service_cookie have already been checked by
     pdc_process_req_cb() */
  
  /* Check that the request data is present */
  if (!request)
  {
    MCFG_MSG_ERROR( "get_default_config_info_req(): request pointer NULL" );
    return QMI_CSI_CB_INTERNAL_ERR;
  }

  /* Init response data */
  memset( response, 0, sizeof(pdc_get_default_config_info_resp_msg_v01) );

  /* Validate request */
  if (!IS_VALID_PDC_CONFIG_TYPE(request->config_type))
  {
    MCFG_MSG_ERROR_1("get_default_config_info_req(): invalid type (%d)", 
                     request->config_type);
    response->resp.error = QMI_ERR_NOT_SUPPORTED_V01;
  }
  else
  {
    response->resp.error = QMI_ERR_NONE_V01;
  }
  
  /* Set the response result based on the response error */  
  response->resp.result = QMI_RESULT_FROM_ERROR(response->resp.error);
  
  /* Send the response */
  qmi_csi_send_resp( req_handle,
                     msg_id,
                     response,
                     sizeof(pdc_get_default_config_info_resp_msg_v01) );

  /* Perform requested action and send indication for valid request */
  if (response->resp.error == QMI_ERR_NONE_V01)
  {
    pdc_send_get_default_config_info_ind(client, request);
  }

  return QMI_CSI_CB_NO_ERR;
} /* pdc_handle_get_default_config_info_req() */


static qmi_error_type_v01 pdc_err_map_mcfg_to_qmi
(
  mcfg_error_e_type error
)
{
  if (error < MCFG_ERR_NON_QMI)
  {
    return (qmi_error_type_v01)error;
  }
  return QMI_ERR_INTERNAL_V01;
  }

/*==============================================================================

  FUNCTION pdc_handle_req

  DESCRIPTION
    Handles a pdc request

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    connection_handle [in] - handle for QMI service connection
    req_handle        [in] - handle used to identify the transaction and client
    msg_id            [in] - message ID for this message
    req_c_struct      [in] - C data structure for this response
    req_c_struct_len  [in] - C data structure length
    service_cookie    [in] - cookie registered with QCSI during qmi_csi_register

  RETURN VALUE
    Status of QMI request

  SIDE EFFECTS
    None

==============================================================================*/
static qmi_csi_cb_error pdc_handle_req
(
  void           *connection_handle,
  qmi_req_handle  req_handle,
  unsigned int    msg_id,
  void           *req_c_struct,
  unsigned int    req_c_struct_len,
  void           *service_cookie
)
{
  void  *request = NULL;
  pdc_service_type *service = NULL;
  pdc_client_type  *client = NULL;
  const mcfg_svc_cmd_entry_type *svc = NULL;
  mcfg_svc_post_cmd_type post_cmd;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/
  
  service  = (pdc_service_type *) service_cookie;
  client   = (pdc_client_type *)  connection_handle;
  
  svc = (const mcfg_svc_cmd_entry_type *) mcfg_svc_cmd_get_handler(msg_id);
  if (!svc)
  {
    MCFG_MSG_ERROR( "pdc_handle_req(): msg_id unknown" );
    return QMI_CSI_CB_NO_MEM;
  }

  request = mcfg_malloc(svc->size_of_req_type);
  if (!request)
  {
    MCFG_MSG_ERROR( "pdc_handle_req(): malloc failed" );
    return QMI_CSI_CB_NO_MEM;
  }
  memscpy(request, svc->size_of_req_type,
          req_c_struct, req_c_struct_len);

  /* connection_handle and service_cookie have already been checked by pdc_process_req_cb() */
  
  post_cmd.data.pdc_post.client = (pdc_client_type *) connection_handle;
  post_cmd.data.pdc_post.req_handle = req_handle;
  post_cmd.data.pdc_post.msg_id = msg_id;
  post_cmd.type = QMI_PDC;

  error = svc->handler_ptr(post_cmd, 
                               request, 
                               req_c_struct_len, 
                               &service->pdc_resp,
                   svc->size_of_resp_type);

  return QMI_CSI_CB_NO_ERR;
} /* pdc_handle_req() */


/*============================================================================== 
 
                    PUBLIC FUNCTION DECLARATIONS FOR MODULE
 
==============================================================================*/

/*===========================================================================

  FUNCTION pdc_send_resp

  DESCRIPTION
    This function sends QMI_PDC response after a req is processed.

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    None 
 
  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void pdc_send_resp
( 
  qmi_req_handle     req_handle,
  unsigned int       msg_id,
  void               *response,
  unsigned int       size
)
{
  /*-----------------------------------------------------------------------*/

  if (!req_handle)
  {
    MCFG_MSG_ERROR("client pointer NULL");
    return;
  }

  if (!response)
  {
    MCFG_MSG_ERROR("response pointer NULL");
    return;
  }

  /* Send the response */
  qmi_csi_send_resp( req_handle,
                     msg_id,
                     response,
                     size );

} /* pdc_send_resp() */


/*===========================================================================

  FUNCTION pdc_send_ind

  DESCRIPTION
    This function sends QMI_PDC indications after a req/response is processed.

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    None 
 
  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void pdc_send_ind
( 
  qmi_client_handle   qmi_client_handle,
  uint16_t            message_id,
  void                *p_src,
  uint32_t            src_len
)
{
  uint8 retry = 0;
  qmi_csi_error qmi_error = QMI_CSI_NO_ERR;
  /*-----------------------------------------------------------------------*/

  if (!p_src)
  {
    MCFG_MSG_ERROR("indication pointer NULL");
    return;
  }

  retry = 0;
  do
  {
    /* send indication message */
    qmi_error = qmi_csi_send_ind(qmi_client_handle,
                                 message_id,
                                 p_src,
                                 src_len);

    if (qmi_error != QMI_CSI_NO_ERR)
    {
      MCFG_MSG_ERROR_2("Failure to send indication: %d, wait 20ms and retry%d ",qmi_error, retry);
      timer_sleep(20, T_MSEC, FALSE);
    }
    else
    {
      MCFG_MSG_LOW("indication is sent");
      break;
    }

    retry ++;
  }while(retry < 10);
} /* pdc_send_ind() */


/*===========================================================================

  FUNCTION pdc_send_deactivate_config_ind

  DESCRIPTION
    This function sends QMI_PDC indications after deactivate config
    req/response are processed.

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    client    [in] pdc client
    request   [in] list configs request

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void pdc_send_deactivate_config_ind
( 
  pdc_client_type               *client,
  pdc_deactivate_config_req_msg_v01 *request
)
{
  pdc_deactivate_config_ind_msg_v01 deactivate_config_ind;
  qmi_csi_error qmi_error;
  mcfg_sub_id_type_e_type sub;
#ifdef FEATURE_MCFG_HW_CONFIG_REQUIRED
  mcfg_config_id_s_type config_id;
#endif
 
  /*-----------------------------------------------------------------------*/
  
  /* Check pass pointers */
  if (!client)
  {
    MCFG_MSG_ERROR("pdc_send_deactivate_config_ind(): client pointer NULL");
    return;
  }
  
  if (!request)
  {
    MCFG_MSG_ERROR("pdc_send_deactivate_config_ind(): request pointer NULL");
    return;
  }
  
  if (request->subscription_id_valid == FALSE)
  {
    sub = MCFG_SUB_ID_FIRST;
  }
  else
  {
    sub = request->subscription_id;
  }

  /* init indication message */
  memset(&deactivate_config_ind, 0, sizeof(pdc_deactivate_config_ind_msg_v01));
  deactivate_config_ind.ind_token_valid = request->ind_token_valid;  
  deactivate_config_ind.ind_token = request->ind_token;

#ifdef FEATURE_MCFG_HW_CONFIG_REQUIRED
  if (request->config_type == MCFG_TYPE_HW &&
      (mcfg_utils_get_active_config(MCFG_TYPE_SW, &config_id, MCFG_SUB_ID_FIRST) || 
       mcfg_utils_get_selected_config(MCFG_TYPE_SW, &config_id, MCFG_SUB_ID_FIRST)))
  {
    MCFG_MSG_ERROR("SW/Carrier MBN has to be deactivated first");
    deactivate_config_ind.error = QMI_ERR_INVALID_OPERATION_V01;
  }
  else 
#endif
  if(!mcfg_utils_deactivate_config(request->config_type, sub, FALSE))
  {
    deactivate_config_ind.error = QMI_ERR_INTERNAL_V01;
  }
  else
  {
    deactivate_config_ind.error = QMI_ERR_NONE_V01;
  }

  qmi_error = qmi_csi_send_ind(client->qmi_client_handle,
                               QMI_PDC_DEACTIVATE_CONFIG_IND_V01,
                               &deactivate_config_ind,
                               sizeof(pdc_deactivate_config_ind_msg_v01));

  if (qmi_error != QMI_CSI_NO_ERR)
  {
    MCFG_MSG_ERROR_1("Failure to deactivate config indication: %d", qmi_error);
  }
  else
  {
    MCFG_MSG_LOW("QMI_PDC_DEACTIVATE_CONFIG_IND_V01 is sent");
  }
} /* pdc_send_deactivate_config_ind() */

/*===========================================================================

  FUNCTION pdc_send_get_config_limits_ind

  DESCRIPTION
    This function sends QMI_PDC indications after get config limits
    req/response are processed.

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    client    [in] pdc client
    request   [in] get selected config request

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void pdc_send_get_config_limits_ind
( 
  pdc_client_type                   *client,
  pdc_get_config_limits_req_msg_v01 *request
)
{
  pdc_get_config_limits_ind_msg_v01 config_limits_ind;
  qmi_csi_error qmi_error;
  
  /*-----------------------------------------------------------------------*/
  
  /* Check pass pointers */
  if (!client)
  {
    MCFG_MSG_ERROR("pdc_send_get_config_limits_ind(): client pointer NULL");
    return;
  }
  
  if (!request)
  {
    MCFG_MSG_ERROR("pdc_send_get_config_limits_ind(): request pointer NULL");
    return;
  }

  /* req handler suppose already verified input parameter: config type */
  
  /* init indication message */
  memset(&config_limits_ind, 0, sizeof(pdc_get_config_limits_ind_msg_v01));
  config_limits_ind.ind_token_valid = request->ind_token_valid;  
  config_limits_ind.ind_token = request->ind_token;

  if (mcfg_utils_get_config_limits(request->config_type, 
                                   &config_limits_ind.max_config_size,
                                   &config_limits_ind.curr_config_size))
  {
    config_limits_ind.max_config_size_valid = TRUE;
    config_limits_ind.curr_config_size_valid = TRUE;
    config_limits_ind.error = QMI_ERR_NONE_V01;
  }
  else
  {
    config_limits_ind.error = QMI_ERR_INTERNAL_V01;
  }

  /* send indication message */
  qmi_error = qmi_csi_send_ind(client->qmi_client_handle,
                               QMI_PDC_GET_CONFIG_LIMITS_IND_V01,
                               &config_limits_ind,
                               sizeof(pdc_get_config_limits_ind_msg_v01));

  if (qmi_error != QMI_CSI_NO_ERR)
  {
    MCFG_MSG_ERROR_1("Failure to send indication: %d", qmi_error);
  }
  else
  {
    MCFG_MSG_LOW("QMI_PDC_GET_CONFIG_LIMITS_IND_V01 is sent");
  }
} /* pdc_send_get_config_limits_ind() */

/*===========================================================================

  FUNCTION pdc_send_get_default_config_info_ind

  DESCRIPTION
    This function sends a QMI_PDC_GET_DEFAULT_CONFIG_INFO_IND indication
    with a binary blob containing the config info trailer of default config

  DEPENDENCIES
    QMI_PDC must be initialized

  PARAMETERS
    client    [in] pdc client
    request   [in] get selected config request

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
static void pdc_send_get_default_config_info_ind
( 
  pdc_client_type                         *client,
  pdc_get_default_config_info_req_msg_v01 *request
)
{
  pdc_get_default_config_info_ind_msg_v01 config_info_ind;
  mcfg_config_info_s_type config_info;
  qmi_csi_error qmi_error;
  
  /*-----------------------------------------------------------------------*/
  
  /* Check the input pointers */
  if (!client)
  {
    MCFG_MSG_ERROR("get_default_config_info_ind(): client pointer NULL");
    return;
  }
  
  if (!request)
  {
    MCFG_MSG_ERROR("get_default_config_info_ind(): request pointer NULL");
    return;
  }

  /* The request handler should have already validated the input parameters
     (the config type and ID) */
  
  /* Initialize the indication message */
  memset(&config_info_ind, 0, sizeof(pdc_get_default_config_info_ind_msg_v01));
  config_info_ind.ind_token_valid = request->ind_token_valid;  
  config_info_ind.ind_token = request->ind_token;

  /* Verify the config ID exists then build the indication */
  if (mcfg_utils_get_default_config_info(request->config_type, &config_info))
  {
    config_info_ind.config_size_valid = TRUE;
    config_info_ind.config_size = config_info.config_len;

    if (mcfg_trl_decode_tlv(
          &config_info, MCFG_TRL_TLV_ID_CARRIER_NAME, 
          config_info_ind.config_desc, MCFG_TRL_CARRIER_NAME_MAX_LEN+1))
    {
      config_info_ind.config_desc_len = 
        strlen((char *)config_info_ind.config_desc);
      if (config_info_ind.config_desc_len > MCFG_TRL_CARRIER_NAME_MAX_LEN)
      {
        config_info_ind.config_desc_len = MCFG_TRL_CARRIER_NAME_MAX_LEN;
      }
      config_info_ind.config_desc_valid = TRUE;
    }

    if (mcfg_trl_decode_tlv(&config_info, MCFG_TRL_TLV_ID_CONFIG_VERSION, 
                            &config_info_ind.config_version, 
                            sizeof(config_info_ind.config_version)))
    {
      config_info_ind.config_version_valid = TRUE;
    }

    config_info_ind.error = QMI_ERR_NONE_V01;
  }
  else
  {
    config_info_ind.error = QMI_ERR_INTERNAL_V01;
  }

  /* Send the indication message */
  qmi_error = qmi_csi_send_ind(client->qmi_client_handle,
                               QMI_PDC_GET_DEFAULT_CONFIG_INFO_IND_V01,
                               &config_info_ind,
                               sizeof(pdc_get_default_config_info_ind_msg_v01));

  if (qmi_error != QMI_CSI_NO_ERR)
  {
    MCFG_MSG_ERROR_1("Failure to send indication: %d", qmi_error);
  }
  else
  {
    MCFG_MSG_LOW("QMI_PDC_GET_DEFAULT_CONFIG_INFO_IND_V01 is sent");
  }
} /* pdc_send_get_default_config_info_ind() */

/*==============================================================================

  FUNCTION pdc_svc_init

  DESCRIPTION
    This function registers the PDC service with the QCSI framework.  The service
    is also registered with QSAP so that it is available via QMUX framework.

  DEPENDENCIES
    QCSI/QSAP must be initialized

  PARAMETERS
    os_params [in] - task parameters for the service

  RETURN VALUE
    Service handle from the QCSI framework

  SIDE EFFECTS
    Registers with the QCSI framework and QSAP

==============================================================================*/
qmi_csi_service_handle pdc_svc_init
( 
  qmi_csi_os_params *os_params
)
{
  qmi_idl_service_object_type  pdc_service_object = NULL;
  qmi_csi_error                qmi_csi_error;
  qmi_sap_client_handle        pdc_sap_handle;
  qmi_sap_error                qmi_sap_error;
  
  pdc_service_type            *pdc_svc_state;
  qmi_csi_options              options;
  
  /*-----------------------------------------------------------------------*/

  if (NULL == os_params)
  {
    MCFG_MSG_ERROR( "pdc_svc_init(): os_params NULL" );
    return NULL;
  }

  pdc_svc_state = mcfg_svc_get_service_state();
  memset(pdc_svc_state, 0, sizeof(*pdc_svc_state));
  if (!pdc_svc_state)
  {
    MCFG_MSG_ERROR( "pdc_svc_init(): service state NULL" );
    return NULL;
  }

  mcfg_svc_default_load_config_cache(&pdc_svc_state->load_config_cache);
  
  pdc_service_object = pdc_get_service_object_v01();

  /*-------------------------------------------------------------------------
    Register QMI PDC service with QMI SI for logging of supported interfaces
  -------------------------------------------------------------------------*/
  (void) qmi_si_register_object( pdc_service_object,
                                 0, /* Service Instance */
                                 pdc_get_service_impl_v01() );

  /* Register with QCSI so clients can connect using IPCRouter */
  QMI_CSI_OPTIONS_INIT(options);
  (void)mqcsi_register_log_options(&options);
  qmi_csi_error = qmi_csi_register_with_options( pdc_service_object,
                                    pdc_connect_cb, 
                                    pdc_disconnect_cb,
                                    pdc_process_req_cb,
                                    pdc_svc_state,
                                    os_params,
                                    &options,
                                    &pdc_svc_state->pdc_service_handler );

  if ( qmi_csi_error == QMI_CSI_NO_ERR)
  {
    /* Register with QSAP so clients can connect using QMUX */
    qmi_sap_error = qmi_sap_register( pdc_service_object,
                                      NULL,
                                      &pdc_sap_handle );
    
    if ( qmi_sap_error != QMI_SAP_NO_ERR)
    {
      MCFG_MSG_ERROR_1( "pdc_svc_init(): QSAP error: %d", qmi_sap_error );
    }
  }
  else
  {
    MCFG_MSG_ERROR_1( "pdc_svc_init(): QCSI error: %d", qmi_csi_error );
  }

  return pdc_svc_state->pdc_service_handler;

} /* pdc_svc_init() */



