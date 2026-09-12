/*==============================================================================

                M C F G   S E R V I C E S

GENERAL DESCRIPTION
  MCFG service implementation

Copyright (c) 2018 by QUALCOMM Technologies Incorporated.
==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_utils/src/mcfg_svc.c#2 $ 
$DateTime: 2021/08/05 08:47:13 $ 
$Author: pwbldsvc $ 
$Change: 32223212 $ 

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
01/09/18      Initial creation

==============================================================================*/

#include "mcfg_utils.h"
#include "mcfg_uim.h"
#include "mcfg_validate.h"
#include "mcfg_trl.h"
#include "mcfg_setting.h"
#include "mcfg_refresh_i.h"
#include "mcfg_int.h"
#include "diagpkt.h"
#include "qmi_pdc_svc.h"
#ifdef FEATURE_MCFG_DIAG_SUPPORT
	#include "mcfg_diag.h"
#endif
#include "qmi_pdc_msg.h"
#include "mcfg_osal.h"


/*==============================================================================
  DEFINITIONS AND DECLARATIONS FOR MODULE
==============================================================================*/

/*==============================================================================
  Constants and Macros
==============================================================================*/
#ifdef FEATURE_MCFG_DIAG_SUPPORT
	#define DIAG_PACKETS_IN_TRANSIT_QUEUE   5
	#define DIAG_WAIT_INTERVAL              5   /* unit: milliseconds */
#endif
#define PDC_QMI_PACKET_MAX_LIMIT        2048  /* 2K */


/*==============================================================================
  Variables and Structures
==============================================================================*/

/*
 * MCFG Service client handle
 */
qmi_client_handle DIAG_CLIENT_HANDLE;

/*==============================================================================
  Local Data
==============================================================================*/
/* 
 * The dispatch table for the MCFG service handlers.
*/
static const mcfg_svc_cmd_entry_type mcfg_svc_cmd_handler_tbl[] =
{
  {MCFG_SVC_CMD_INDICATION_REGISTER,    sizeof(pdc_indication_register_req_msg_v01),    sizeof(pdc_indication_register_resp_msg_v01),   mcfg_svc_indication_register_handler},
  {MCFG_SVC_CMD_GET_SELECTED_CONFIG,    sizeof(pdc_get_selected_config_req_msg_v01),    sizeof(pdc_get_selected_config_resp_msg_v01),   mcfg_svc_get_selected_config_handler},
  {MCFG_SVC_CMD_SET_SELECTED_CONFIG,    sizeof(pdc_set_selected_config_req_msg_v01),    sizeof(pdc_set_selected_config_resp_msg_v01),   mcfg_svc_set_selected_config_handler},
  {MCFG_SVC_CMD_LIST_CONFIG,            sizeof(pdc_list_configs_req_msg_v01),           sizeof(pdc_list_configs_resp_msg_v01),          mcfg_svc_list_configs_handler},
  {MCFG_SVC_CMD_DELETE_CONFIG,          sizeof(pdc_delete_config_req_msg_v01),          sizeof(pdc_delete_config_resp_msg_v01),         mcfg_svc_delete_config_handler},
  {MCFG_SVC_CMD_LOAD_CONFIG,            sizeof(pdc_load_config_req_msg_v01),            sizeof(pdc_load_config_resp_msg_v01),           mcfg_svc_load_config_handler},
  {MCFG_SVC_CMD_ACTIVATE_CONFIG,        sizeof(pdc_activate_config_req_msg_v01),        sizeof(pdc_activate_config_resp_msg_v01),       mcfg_svc_activate_config_handler},
  {MCFG_SVC_CMD_GET_CONFIG_INFO,        sizeof(pdc_get_config_info_req_msg_v01),        sizeof(pdc_get_config_info_resp_msg_v01),       mcfg_svc_get_config_info_handler},
  {MCFG_SVC_CMD_VALIDATE_CONFIG,        sizeof(pdc_validate_config_req_msg_v01),        sizeof(pdc_validate_config_resp_msg_v01),       mcfg_svc_validate_config_handler},
  {MCFG_SVC_CMD_GET_FEATURE,            sizeof(pdc_get_feature_req_msg_v01),            sizeof(pdc_get_feature_resp_msg_v01),           mcfg_svc_get_feature_handler},
  {MCFG_SVC_CMD_SET_FEATURE,            sizeof(pdc_set_feature_req_msg_v01),            sizeof(pdc_set_feature_resp_msg_v01),           mcfg_svc_set_feature_handler},
  {MCFG_SVC_CMD_GET_CONFIG,             sizeof(pdc_get_config_req_msg_v01),             sizeof(pdc_get_config_resp_msg_v01),            mcfg_svc_get_config_handler},
};


/*==============================================================================

  FUNCTION mcfg_svc_get_service_state

  DESCRIPTION
    Returns a pointer to the global cache data for mcfg services

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    Pointer to the cache data stored globally

  SIDE EFFECTS
    None

==============================================================================*/
pdc_service_type * mcfg_svc_get_service_state
(
  void
)
{
  return &global_mcfg_svc;
} /* END mcfg_svc_get_service_state */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_get_handler

  DESCRIPTION
    lookup function for the mcfg_svc_cmd_handler_tbl.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
const mcfg_svc_cmd_entry_type* mcfg_svc_cmd_get_handler
(
   mcfg_svc_cmd_e_type cmd_code
)
{
  int size_of_entry_table = 0;
  int index = 0;
  /*-----------------------------------------------------------------------*/

  size_of_entry_table = (sizeof(mcfg_svc_cmd_entry_type) > 0) ? (sizeof(mcfg_svc_cmd_handler_tbl) / sizeof(mcfg_svc_cmd_entry_type)) : 0;

  for (index=0; index<size_of_entry_table; index++)
  {
    if(cmd_code == mcfg_svc_cmd_handler_tbl[index].cmd_code)
    {
      return &mcfg_svc_cmd_handler_tbl[index];
    }
  }

  return NULL;
} /* END mcfg_svc_cmd_get_handler */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_client_match

  DESCRIPTION
    checks if the same client is making the sub sequential requests

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_svc_cmd_client_match
(
  mcfg_svc_post_cmd_type post_cmd,
  void *cache,
  uint16_t message_id
)
{
  mcfg_svc_client_type    *client = NULL;
  /*-----------------------------------------------------------------------*/

  client = (mcfg_svc_client_type *)cache;

  if (post_cmd.type == QMI_PDC)
  {
    if (client->qmi_client_handle != post_cmd.data.pdc_post.client->qmi_client_handle)
    {
      MCFG_MSG_HIGH_2( "qmi handle %p mismatch %p", 
                        post_cmd.data.pdc_post.client->qmi_client_handle, client->qmi_client_handle);

      return FALSE;
    }
  }
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  else if (post_cmd.type == DIAG_CMD)
  {
    if (client->qmi_client_handle != (struct qmi_client_handle_struct *)&DIAG_CLIENT_HANDLE)
    {
      MCFG_MSG_HIGH_2( "diag handle %p mismatch %p", 
                        (struct qmi_client_handle_struct *)&DIAG_CLIENT_HANDLE, client->qmi_client_handle);

      return FALSE;
    }
  }
#endif
  else
  {
    /* invalid cmd type */
    return FALSE;
  }

  return TRUE;
} /* END mcfg_svc_cmd_client_match */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_cache_update

  DESCRIPTION
    updates the client cache information

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_svc_cmd_cache_update
(
  mcfg_svc_post_cmd_type post_cmd,
  void *cache,
  uint16_t message_id
)
{
  mcfg_svc_client_type    *client = NULL;
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  diag_client_type        *diag_client = NULL;
#endif
  /*-----------------------------------------------------------------------*/

  if (post_cmd.type == QMI_PDC)
  {
    client = (mcfg_svc_client_type *)cache;
    client->qmi_client_handle = post_cmd.data.pdc_post.client->qmi_client_handle;

    MCFG_MSG_LOW_2("client qmi handle %d, cached qmi handle %d", 
                    post_cmd.data.pdc_post.client->qmi_client_handle,
                    client->qmi_client_handle);

    return TRUE;
  }
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  else if (post_cmd.type == DIAG_CMD)
  {
    client = (mcfg_svc_client_type *)cache;
    client->qmi_client_handle = (struct qmi_client_handle_struct *)&DIAG_CLIENT_HANDLE;

    diag_client = (diag_client_type *)mcfg_diag_cmd_get_client_cache(message_id);
    if (!diag_client)
    {
      MCFG_MSG_ERROR_3("invalid message_id %d, failed to cache delayed rsp id %d for client %p", 
                       message_id, 
                       post_cmd.data.diag_post.delayed_rsp_id,
                       client->qmi_client_handle);

      return FALSE;
    }

    diag_client->delayed_rsp_id = post_cmd.data.diag_post.delayed_rsp_id;
    MCFG_MSG_LOW_2("cached diag handle %d, cached delayed rsp id %d", 
                    client->qmi_client_handle,
                    diag_client->delayed_rsp_id);

    return TRUE;
  }
#endif
  else
  {
    /* invalid cmd type */
    return FALSE;
  }

  return FALSE;
} /* END mcfg_svc_cmd_cache_update */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_generate_post_cmd

  DESCRIPTION
    generates a post_cmd type to be used for sending cached indications

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
#ifdef FEATURE_MCFG_DIAG_SUPPORT
qmi_error_type_v01 mcfg_svc_cmd_generate_post_cmd
(
  mcfg_svc_post_cmd_type *post_cmd,
  qmi_client_handle  client_handle,
  diag_client_type *diag_client
)
#else
qmi_error_type_v01 mcfg_svc_cmd_generate_post_cmd
(
  mcfg_svc_post_cmd_type *post_cmd,
  qmi_client_handle  client_handle
)
#endif
{
  pdc_service_type *svc = NULL;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  svc = mcfg_svc_get_service_state();

  if(client_handle == NULL)
  {
    MCFG_MSG_ERROR("unexpected NULL pointer error");
    err = QMI_ERR_MALFORMED_MSG_V01;
    return err;
  }
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  if (client_handle == svc->pdc_clients[DIAG_CLIENT_INDEX].qmi_client_handle)
  {
    if (!diag_client)
    {
      MCFG_MSG_ERROR("unexpected NULL pointer error");
      err = QMI_ERR_INTERNAL_V01;
      return err;
    }
    post_cmd->type = DIAG_CMD;
    post_cmd->data.diag_post.delayed_rsp_id = diag_client->delayed_rsp_id;
  }
  else
#endif
  {
    post_cmd->data.pdc_post.client = mcfg_malloc(sizeof(pdc_client_type));
    if(!post_cmd->data.pdc_post.client)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      return err;
    }
    memset(post_cmd->data.pdc_post.client, 0, sizeof(pdc_client_type));

    post_cmd->type = QMI_PDC;
    post_cmd->data.pdc_post.client->qmi_client_handle = client_handle;
  }

  return err;
} /* END mcfg_svc_cmd_generate_post_cmd */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_resp

  DESCRIPTION
    sends a response for the QMI / DIAG command in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_resp
(
  mcfg_svc_post_cmd_type post_cmd,
  qmi_idl_type_of_message_type message_type,
  uint16_t message_id,
  void *resp,
  uint32_t resp_size,
  qmi_error_type_v01 error
)
{
  /*-----------------------------------------------------------------------*/

  MCFG_MSG_LOW_2("Sending response type %d message_id 0x%02x", post_cmd.type, message_id);
  if (post_cmd.type == QMI_PDC)
  {
    pdc_send_resp(post_cmd.data.pdc_post.req_handle,
                  post_cmd.data.pdc_post.msg_id,
                  resp,
                  resp_size);
  }
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  else if (post_cmd.type == DIAG_CMD)
  {
    mcfg_diag_cmd_resp(post_cmd.data.diag_post.delayed_rsp,
                       message_type,
                       message_id,
                       resp,
                       resp_size,
                       error);

  }
#endif
  else
  {
    MCFG_MSG_ERROR_1("Error %d", QMI_ERR_NOT_PROVISIONED_V01);
  }
} /* END mcfg_svc_cmd_resp */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_ind

  DESCRIPTION
    sends an indication for the QMI / DIAG command in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_ind
(
  mcfg_svc_post_cmd_type post_cmd,
  uint16 rsp_cnt,
  qmi_idl_type_of_message_type message_type,
  uint16_t message_id,
  void *p_src,
  uint32_t src_len,
  uint32_t buffer_size,
  qmi_error_type_v01 err
)
{
  qmi_client_handle  qmi_client_handle;
  /*-----------------------------------------------------------------------*/

  MCFG_MSG_LOW_2("Sending indication type %d message_id 0x%02x", post_cmd.type, message_id);
  if (post_cmd.type == QMI_PDC)
  {
    qmi_client_handle = post_cmd.data.pdc_post.client->qmi_client_handle;
    pdc_send_ind(qmi_client_handle,
                 message_id,
                 p_src,
                 src_len);
  }
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  else if (post_cmd.type == DIAG_CMD)
  {
    mcfg_diag_cmd_ind(post_cmd.data.diag_post.delayed_rsp_id,
                      rsp_cnt,
                      message_type,
                      message_id,
                      p_src,
                      src_len,
                      buffer_size,
                      err);

  }
#endif
  else
  {
    MCFG_MSG_ERROR_1("Error %d", QMI_ERR_NOT_PROVISIONED_V01);
  }
} /* END mcfg_svc_cmd_ind */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_proc

  DESCRIPTION
    Processes a mcfg svc command in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_proc
(
  mcfg_task_cmd_s_type *cmd
)
{
  mcfg_svc_cmd_data_s_type *mcfg_svc_cmd_data = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MCFG_CHECK_NULL_PTR_RET(cmd);
  MCFG_CHECK_NULL_PTR_RET(cmd->data);
  
  mcfg_svc_cmd_data = (mcfg_svc_cmd_data_s_type *) cmd->data;
  MCFG_CHECK_NULL_PTR_RET(mcfg_svc_cmd_data->request);

  switch (mcfg_svc_cmd_data->cmd_id)
  {
    case MCFG_SVC_CMD_INDICATION_REGISTER:
      mcfg_svc_cmd_indication_register(mcfg_svc_cmd_data->post_cmd, (pdc_indication_register_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_GET_SELECTED_CONFIG:
      mcfg_svc_cmd_get_selected_config(mcfg_svc_cmd_data->post_cmd, (pdc_get_selected_config_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_SET_SELECTED_CONFIG:
      mcfg_svc_cmd_set_selected_config(mcfg_svc_cmd_data->post_cmd, (pdc_set_selected_config_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_LIST_CONFIG:
      mcfg_svc_cmd_list_config(mcfg_svc_cmd_data->post_cmd, (pdc_list_configs_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_DELETE_CONFIG:
      mcfg_svc_cmd_delete_config(mcfg_svc_cmd_data->post_cmd, (pdc_delete_config_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_LOAD_CONFIG:
      mcfg_svc_cmd_load_config(mcfg_svc_cmd_data->post_cmd, (pdc_load_config_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_ACTIVATE_CONFIG:
      mcfg_svc_cmd_activate_config(mcfg_svc_cmd_data->post_cmd, (pdc_activate_config_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_GET_CONFIG_INFO:
      mcfg_svc_cmd_get_config_info(mcfg_svc_cmd_data->post_cmd, (pdc_get_config_info_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_VALIDATE_CONFIG:
      mcfg_svc_cmd_validate_config(mcfg_svc_cmd_data->post_cmd, (pdc_validate_config_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_GET_FEATURE:
      mcfg_svc_cmd_get_feature(mcfg_svc_cmd_data->post_cmd, (pdc_get_feature_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_SET_FEATURE:
      mcfg_svc_cmd_set_feature(mcfg_svc_cmd_data->post_cmd, (pdc_set_feature_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    case MCFG_SVC_CMD_GET_CONFIG:
      mcfg_svc_cmd_get_config(mcfg_svc_cmd_data->post_cmd, (pdc_get_config_req_msg_v01 *) mcfg_svc_cmd_data->request);
      break;

    default:
      MCFG_MSG_ERROR_1("Unexpected mcfg svc command 0x%02x", mcfg_svc_cmd_data->cmd_id);
  }
} /* END mcfg_svc_cmd_proc */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_post

  DESCRIPTION
    This function sends mcfg svc commands for processing. 

  DEPENDENCIES
    None

  PARAMETERS

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_post
(
  mcfg_svc_cmd_e_type cmd_type,
  mcfg_svc_post_cmd_type post_cmd,
  void *request
)
{
  mcfg_task_cmd_s_type *cmd = NULL;
  mcfg_svc_cmd_data_s_type *mcfg_svc_cmd_data = NULL;
  uint32 path_length = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  MCFG_CHECK_NULL_PTR_RET(request);

  cmd = mcfg_task_cmd_alloc(MCFG_TASK_CMD_SVC, sizeof(mcfg_svc_cmd_data_s_type));
  MCFG_CHECK_NULL_PTR_RET(cmd);
  MCFG_CHECK_NULL_PTR_RET(cmd->data);
    
  mcfg_svc_cmd_data = (mcfg_svc_cmd_data_s_type *) cmd->data;
  mcfg_svc_cmd_data->cmd_id = cmd_type;
  mcfg_svc_cmd_data->post_cmd = post_cmd;
  mcfg_svc_cmd_data->request = request;

  if (!mcfg_task_cmd_send(cmd))
  {
    MCFG_MSG_ERROR_1("mcfg svc command 0x%02x post failed", mcfg_svc_cmd_data->cmd_id);
    mcfg_task_cmd_free(cmd);
  }
  else
  {
    MCFG_MSG_LOW_1("mcfg svc command 0x%02x post successful", mcfg_svc_cmd_data->cmd_id);
  }
} /* END mcfg_svc_cmd_post */


/*===========================================================================

  FUNCTION mcfg_svc_indication_register_handler

  DESCRIPTION
    Handles a mcfg svc command packet for refresh indication registeration in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_indication_register_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_indication_register_resp_msg_v01 *pdc_rsp = NULL;
  pdc_indication_register_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_indication_register_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_indication_register_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_indication_register_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_INDICATION_REGISTER);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_INDICATION_REGISTER, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_INDICATION_REGISTER,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_indication_register_handler */


/*===========================================================================

  FUNCTION mcfg_svc_get_selected_config_handler

  DESCRIPTION
    Handles a mcfg svc command packet for get selected config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_get_selected_config_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_get_selected_config_resp_msg_v01 *pdc_rsp = NULL;
  pdc_get_selected_config_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_get_selected_config_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_get_selected_config_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_get_selected_config_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_PDC_CONFIG_TYPE(pdc_req->config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->config_type);
      break;
    }
    if (pdc_req->subscription_id_valid == TRUE && 
        !IS_VALID_MCFG_SUBSCRIPTION_ID(pdc_req->subscription_id))
    {
      error = QMI_ERR_NONE_V01;
      MCFG_MSG_ERROR_1("invalid sub %lu", pdc_req->subscription_id);
      break;
    }
    if (pdc_req->slot_id_valid == TRUE && 
      !IS_VALID_MCFG_SLOT_ID(pdc_req->slot_id))
    {
      error = QMI_ERR_NONE_V01;
      MCFG_MSG_ERROR_1("invalid slot %lu", pdc_req->slot_id);
      break;
    }
  }while(0);

  /* PDC tool checks for sub 2 always; to facilitate this without throwing error, mcfg_svc_cmd_post
     for get_selected_config is outside do-while() loop */
  if(error == QMI_ERR_NONE_V01)
  {
    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_GET_SELECTED_CONFIG);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_GET_SELECTED_CONFIG, post_cmd, pdc_req);
  }

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);

  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_GET_SELECTED_CONFIG,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_get_selected_config_handler */


/*===========================================================================

  FUNCTION mcfg_svc_set_selected_config_handler

  DESCRIPTION
    Handles a mcfg svc command packet for set selected config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_set_selected_config_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_set_selected_config_resp_msg_v01 *pdc_rsp = NULL;
  pdc_set_selected_config_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_set_selected_config_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_set_selected_config_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_set_selected_config_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_PDC_CONFIG_TYPE(pdc_req->new_config_info.config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->new_config_info.config_type);
      break;
    }
    if (!IS_VALID_PDC_CONFIG_ID_LEN(pdc_req->new_config_info.config_id_len))
    {
      error = QMI_ERR_INVALID_ID_V01;
      MCFG_MSG_ERROR_1("invalid config id length %lu", pdc_req->new_config_info.config_id_len);
      break;
    }
    if (pdc_req->subscription_id_valid == TRUE && 
        !IS_VALID_MCFG_SUBSCRIPTION_ID(pdc_req->subscription_id))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid sub %lu", pdc_req->subscription_id);
      break;
    }
    if (pdc_req->slot_id_valid == TRUE && 
      !IS_VALID_MCFG_SLOT_ID(pdc_req->slot_id))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid slot %lu", pdc_req->slot_id);
      break;
    }

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_SET_SELECTED_CONFIG);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_SET_SELECTED_CONFIG, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_SET_SELECTED_CONFIG,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_set_selected_config_handler */


/*===========================================================================

  FUNCTION mcfg_svc_list_configs_handler

  DESCRIPTION
    Handles a mcfg svc command packet to list configs in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_list_configs_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_list_configs_resp_msg_v01 *pdc_rsp = NULL;
  pdc_list_configs_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_list_configs_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_list_configs_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_list_configs_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (pdc_req->config_type_valid == TRUE && 
        !IS_VALID_PDC_CONFIG_TYPE(pdc_req->config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->config_type);
      break;
    }

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_LIST_CONFIG);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_LIST_CONFIG, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_LIST_CONFIG,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_list_configs_handler */


/*===========================================================================

  FUNCTION mcfg_svc_delete_config_handler

  DESCRIPTION
    Handles a mcfg svc command packet to delete config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_delete_config_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_delete_config_resp_msg_v01 *pdc_rsp = NULL;
  pdc_delete_config_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_delete_config_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_delete_config_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_delete_config_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_PDC_CONFIG_TYPE(pdc_req->config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->config_type);
      break;
    }
    if (pdc_req->config_id_valid && 
        !IS_VALID_PDC_CONFIG_ID_LEN(pdc_req->config_id_len))
    {
      error = QMI_ERR_INVALID_ID_V01;
      MCFG_MSG_ERROR_1("invalid config id length %lu", pdc_req->config_id_len);
      break;
    }

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_DELETE_CONFIG);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_DELETE_CONFIG, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_DELETE_CONFIG,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_delete_config_handler */


/*===========================================================================

  FUNCTION mcfg_svc_load_config_handler

  DESCRIPTION
    Handles a mcfg svc command packet to load config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_load_config_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_load_config_resp_msg_v01 *pdc_rsp = NULL;
  pdc_load_config_req_msg_v01 *pdc_req = NULL;
  pdc_load_config_cache_type *cache = NULL;
  pdc_service_type *svc = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_load_config_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_load_config_resp_msg_v01 *) rsp_ptr;
    svc = mcfg_svc_get_service_state();
    cache = &svc->load_config_cache;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_load_config_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_PDC_CONFIG_TYPE(pdc_req->load_config_info.config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->load_config_info.config_type);
      break;
    }
    if (!IS_VALID_PDC_CONFIG_ID_LEN(pdc_req->load_config_info.config_id_len))
    {
      error = QMI_ERR_INVALID_ID_V01;
      MCFG_MSG_ERROR_1("invalid config id length %lu", pdc_req->load_config_info.config_id_len);
      break;
    }
    if (pdc_req->load_config_info.total_config_size == 0 ||
      pdc_req->load_config_info.total_config_size > MCFG_CONFIG_SIZE_MAX || 
      pdc_req->load_config_info.config_frame_len == 0 ||
      pdc_req->load_config_info.config_frame_len > MCFG_CONFIG_FRAME_SIZE_MAX )
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_2( "invalid total size %lu frame len %lu", pdc_req->load_config_info.total_config_size, pdc_req->load_config_info.config_frame_len);
      break;
    }
    if (pdc_req->storage_valid == TRUE && 
             !IS_VALID_PDC_STORAGE_TYPE(pdc_req->storage))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1( "invalid storage type %d", pdc_req->storage );
      break;
    }

    do
    {
      if (cache->config_s.config_len != 0)
      {
        /* this is subsequential request, validate with cached information */
        if (!mcfg_svc_cmd_client_match(post_cmd, cache, MCFG_SVC_CMD_LOAD_CONFIG))
        {
          error = QMI_ERR_DEVICE_IN_USE_V01;
          /* Skip resetting the cache */
          break;
        }

        if (pdc_req->storage_valid == TRUE && 
                 pdc_req->storage != (pdc_storage_enum_v01) cache->storage)
        {
          MCFG_MSG_ERROR_1( "invalid storage type %d", 
                            pdc_req->storage );
          error = QMI_ERR_INVALID_ARG_V01;
        } 
        else if (pdc_req->load_config_info.config_type != (pdc_config_type_enum_v01) cache->type)
        {
          MCFG_MSG_ERROR_1( "invalid config type %d", 
                            pdc_req->load_config_info.config_type );
          error = QMI_ERR_INVALID_ARG_V01;
        }  
        else if (pdc_req->load_config_info.total_config_size != cache->config_s.config_len || 
                    !MCFG_SIZE_OFFSET_WITHIN_LIMIT(pdc_req->load_config_info.config_frame_len, 
                      cache->offset,pdc_req->load_config_info.total_config_size))
        {
          MCFG_MSG_ERROR_1( "invalid total size %lu", 
                            pdc_req->load_config_info.total_config_size );
          error = QMI_ERR_INVALID_ARG_V01;
        }  
        else if (pdc_req->load_config_info.config_id_len != cache->id_s.id_len ||
                 memcmp(pdc_req->load_config_info.config_id, 
                        cache->id_s.id, cache->id_s.id_len) !=0)
        {
          MCFG_MSG_ERROR_1( "invalid ID %lu", 
                            pdc_req->load_config_info.config_id_len );
          error = QMI_ERR_INVALID_ID_V01;
        }
      }
      /* this is first request, double check cached load config info*/
      else if (cache->qmi_client_handle != NULL ||
                cache->offset != 0 || 
                cache->id_s.id_len != 0)
      {
         MCFG_MSG_ERROR( "invalid handle");
         error = QMI_ERR_INTERNAL_V01;
      }

      /* Reset the cache for all errors except a QMI handle mismatch */
      if (error != QMI_ERR_NONE_V01)
      {
        mcfg_svc_default_load_config_cache(cache);
      }
    } while(0);
    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_LOAD_CONFIG);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_LOAD_CONFIG, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  /* A 'frame data reset' tlv is sent for all errors */
  if (pdc_rsp->resp.error != QMI_ERR_NONE_V01)
  {
    pdc_rsp->frame_data_reset = TRUE;
    pdc_rsp->frame_data_reset_valid = TRUE;
  }
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_LOAD_CONFIG,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_load_config_handler */


/*===========================================================================

  FUNCTION mcfg_svc_activate_config_handler

  DESCRIPTION
    Handles a mcfg svc command packet to activate config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_activate_config_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_activate_config_resp_msg_v01 *pdc_rsp = NULL;
  pdc_activate_config_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_activate_config_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_activate_config_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_activate_config_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_PDC_CONFIG_TYPE(pdc_req->config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->config_type);
      break;
    }
    if (pdc_req->subscription_id_valid == TRUE && 
        !IS_VALID_MCFG_SUBSCRIPTION_ID(pdc_req->subscription_id))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid sub %lu", pdc_req->subscription_id);
      break;
    }
    if (pdc_req->slot_id_valid == TRUE && 
      !IS_VALID_MCFG_SLOT_ID(pdc_req->slot_id))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid slot %lu", pdc_req->slot_id);
      break;
    }

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_ACTIVATE_CONFIG);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_ACTIVATE_CONFIG, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_ACTIVATE_CONFIG,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_activate_config_handler */


/*===========================================================================

  FUNCTION mcfg_svc_get_config_info_handler

  DESCRIPTION
    Handles a mcfg svc command packet to get config info in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_get_config_info_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_get_config_info_resp_msg_v01 *pdc_rsp = NULL;
  pdc_get_config_info_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_get_config_info_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_get_config_info_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_get_config_info_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_PDC_CONFIG_TYPE(pdc_req->new_config_info.config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->new_config_info.config_type);
      break;
    }
    if (!IS_VALID_PDC_CONFIG_ID_LEN(pdc_req->new_config_info.config_id_len))
    {
      error = QMI_ERR_INVALID_ID_V01;
      MCFG_MSG_ERROR_1("invalid config id length %lu", pdc_req->new_config_info.config_id_len);
      break;
    }

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_GET_CONFIG_INFO);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_GET_CONFIG_INFO, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_GET_CONFIG_INFO,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_get_config_info_handler */


/*===========================================================================

  FUNCTION mcfg_svc_validate_config_handler

  DESCRIPTION
    Handles a mcfg svc command packet for validate config request in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_validate_config_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_validate_config_resp_msg_v01 *pdc_rsp = NULL;
  pdc_validate_config_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_validate_config_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_validate_config_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_validate_config_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_PDC_CONFIG_TYPE(pdc_req->config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->config_type);
      break;
    }
    if (pdc_req->config_id_valid == TRUE && 
      !IS_VALID_PDC_CONFIG_ID_LEN(pdc_req->config_id_len))
    {
      error = QMI_ERR_INVALID_ID_V01;
      MCFG_MSG_ERROR_1("invalid config id length %lu", pdc_req->config_id_len);
      break;
    }
    if (pdc_req->subscription_id_valid == TRUE && 
        !IS_VALID_MCFG_SUBSCRIPTION_ID(pdc_req->subscription_id))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid sub %lu", pdc_req->subscription_id);
      break;
    }

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_VALIDATE_CONFIG);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_VALIDATE_CONFIG, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_VALIDATE_CONFIG,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_validate_config_handler */


/*===========================================================================

  FUNCTION mcfg_svc_get_feature_handler

  DESCRIPTION
    Handles a mcfg svc command packet to set feature in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_get_feature_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_get_feature_resp_msg_v01 *pdc_rsp = NULL;
  pdc_get_feature_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_get_feature_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_get_feature_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_get_feature_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_MCFG_SLOT_ID(pdc_req->slot_id))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid slot %lu", pdc_req->slot_id);
      break;
    }

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_GET_FEATURE);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_GET_FEATURE, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_GET_FEATURE,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_get_feature_handler */


/*===========================================================================

  FUNCTION mcfg_svc_set_feature_handler

  DESCRIPTION
    Handles a mcfg svc command packet to set feature in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_set_feature_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_set_feature_resp_msg_v01 *pdc_rsp = NULL;
  pdc_set_feature_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_set_feature_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_set_feature_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_set_feature_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_MCFG_SLOT_ID(pdc_req->slot_id))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid slot %lu", pdc_req->slot_id);
      break;
    }

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_SET_FEATURE);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_SET_FEATURE, post_cmd, pdc_req);
  }while(0);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_SET_FEATURE,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_set_feature_handler */


/*===========================================================================

  FUNCTION mcfg_svc_get_config_handler

  DESCRIPTION
    Handles a mcfg svc command packet to get config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
int32 mcfg_svc_get_config_handler
(
  mcfg_svc_post_cmd_type post_cmd,
  void *req_ptr,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
)
{
  pdc_get_config_resp_msg_v01 *pdc_rsp = NULL;
  pdc_get_config_req_msg_v01 *pdc_req = NULL;
  qmi_error_type_v01 error = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do{
    pdc_req = (pdc_get_config_req_msg_v01 *) req_ptr;
    pdc_rsp = (pdc_get_config_resp_msg_v01 *) rsp_ptr;

    /* Init response data */
    memset( pdc_rsp, 0, sizeof(pdc_get_config_resp_msg_v01) );

    if (!req_ptr)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR( "request pointer NULL");
      break;
    }

    /* Validate request */
    if (!IS_VALID_PDC_CONFIG_TYPE(pdc_req->config_type))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid config type %d", pdc_req->config_type);
      break;
    }
    if (pdc_req->config_id_valid == TRUE && 
        !IS_VALID_PDC_CONFIG_ID_LEN(pdc_req->config_id_len))
    {
      error = QMI_ERR_INVALID_ID_V01;
      MCFG_MSG_ERROR_1("invalid config id length %lu", pdc_req->config_id_len);
      break;
    }
    if (pdc_req->subscription_id_valid == TRUE && 
        !IS_VALID_MCFG_SUBSCRIPTION_ID(pdc_req->subscription_id))
    {
      error = QMI_ERR_NOT_SUPPORTED_V01;
      MCFG_MSG_ERROR_1("invalid sub %lu", pdc_req->subscription_id);
      break;
    }
    if (pdc_req->ind_token_valid != TRUE || pdc_req->frame_index_valid != TRUE)
    {
      error = QMI_ERR_MALFORMED_MSG_V01;
      MCFG_MSG_ERROR("invalid ind_token or frame_index");
      break;
    }

    if (pdc_req->ind_token == get_config_ind_token)  /* request for either next frame or first frame */
    {
      if (pdc_req->frame_index != get_config_frame_index)
      {
        get_config_ind_token = 0;
        get_config_frame_index = 0;

        MCFG_MSG_ERROR_1("invalid frame_index %lu", pdc_req->frame_index);
        error = QMI_ERR_INVALID_INDEX_V01;
        break;
      }
    }
    else  /* request for first frame */
    {
      if(pdc_req->frame_index != 0)
      {
        get_config_ind_token = 0;
        get_config_frame_index = 0;

        MCFG_MSG_ERROR_1("invalid frame_index %lu", pdc_req->frame_index);
        error = QMI_ERR_INVALID_INDEX_V01;
        break;
      }
    }
    get_config_ind_token = pdc_req->ind_token;
    get_config_frame_index = pdc_req->frame_index + 1;

    /*----------------------------------------------------------------------
    Send Request to MCFG context for processing
    ----------------------------------------------------------------------*/
    MCFG_MSG_LOW_1("Successfully sent to mcfg_svc proc 0x%02x", MCFG_SVC_CMD_GET_CONFIG);
    mcfg_svc_cmd_post(MCFG_SVC_CMD_GET_CONFIG, post_cmd, pdc_req);
  }while(0);

  MCFG_MSG_LOW_1("get_config_ind_token 0x%02x", get_config_ind_token);
  MCFG_MSG_LOW_1("get_config_frame_index 0x%02x", get_config_frame_index);

  /* Set the response result based on the response error */
  pdc_rsp->resp.error = error;
  pdc_rsp->resp.result = QMI_RESULT_FROM_ERROR(pdc_rsp->resp.error);
  
  mcfg_svc_cmd_resp(post_cmd,
                    QMI_IDL_RESPONSE,
                    MCFG_SVC_CMD_GET_CONFIG,
                    pdc_rsp,
                    rsp_len,
                    error);

  return error;
} /* END mcfg_svc_get_config_handler */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_indication_register

  DESCRIPTION
    Processes a mcfg svc command to register for refresh event indication in the context of the MCFG task.
    This function only handles DIAG client currently. In future, it could be made to handle all clients.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_indication_register
(
  mcfg_svc_post_cmd_type post_cmd,
  pdc_indication_register_req_msg_v01 *request
)
{
  mcfg_client_event_reg_s_type *event_reg_info = NULL;
  pdc_service_type *svc = NULL;
  pdc_client_type *client = NULL;
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  diag_client_type *diag_client = NULL;
#endif
  uint16 rsp_cnt = 1;
  mcfg_client_event_e_type event_reg_type = 0;
  mcfg_client_id_type client_id = 0;
  static boolean indication_registered = FALSE;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    svc = mcfg_svc_get_service_state();
    client = &svc->pdc_clients[DIAG_CLIENT_INDEX];
#ifdef FEATURE_MCFG_DIAG_SUPPORT
    diag_client = (diag_client_type *)mcfg_diag_cmd_get_client_cache(MCFG_SVC_CMD_INDICATION_REGISTER);
    if (!diag_client)
    {
      MCFG_MSG_ERROR_1("failed to get diag client cache for message_id %d", 
                       MCFG_SVC_CMD_INDICATION_REGISTER);
      err = QMI_ERR_INTERNAL_V01;
      break;
    }
#endif

    MCFG_MSG_LOW( "handling refresh event TLV");
    client->event_reg.refresh_event = TRUE;

    if(!indication_registered) /* avoid multiple registration */
    {
      MCFG_MSG_LOW_1("indication_registered %d", indication_registered);
      event_reg_info = (mcfg_client_event_reg_s_type *) mcfg_malloc(sizeof(mcfg_client_event_reg_s_type));
      if (!event_reg_info)
      {
        err = QMI_ERR_NO_MEMORY_V01;
        break;
      }
      memset(event_reg_info, 0, sizeof(mcfg_client_event_reg_s_type));

      client_id = MCFG_CLIENT_ID_MCFG;
      event_reg_type = (MCFG_CLIENT_EVENT_REFRESH_START | MCFG_CLIENT_EVENT_REFRESH_COMPLETE);
      event_reg_info->refresh_event_reg.type = MCFG_REFRESH_TYPE_SLOT_N_SUBS;
      event_reg_info->refresh_event_reg.slot_mask = MCFG_REFRESH_INDEX_MASK_ALL;
      event_reg_info->refresh_event_reg.sub_mask = MCFG_REFRESH_INDEX_MASK_ALL;
      event_reg_info->refresh_event_reg.refresh_event_cb = &mcfg_svc_cmd_refresh_event_ind;

      if(!mcfg_client_register(client_id, event_reg_type, event_reg_info)){
        err = QMI_ERR_ABORTED_V01;
        break;
      }
    }

    indication_registered = TRUE;
    MCFG_MSG_LOW("indication registration successful");

#ifdef FEATURE_MCFG_DIAG_SUPPORT
    /* cache client information after successful registration */
    diag_client->delayed_rsp_id = post_cmd.data.diag_post.delayed_rsp_id;
    diag_client->rsp_cnt = 1;
	rsp_cnt = diag_client->rsp_cnt;
#endif

    if (request->reg_config_change_valid)
    {
      client->event_reg.selected_config = request->reg_config_change;

      MCFG_MSG_LOW("handling reg_config_change TLV");
      if (request->reg_config_change)
      {
        /* Send the IND to the client when the reporting is enabled using the cached
           IND that is stored in the service info */
        if (svc->pdc_ind.config_change_valid)
        {
          MCFG_MSG_LOW("sending config_change IND");
          /* send indication message */
          mcfg_svc_cmd_ind(post_cmd,
                           rsp_cnt,
                           QMI_IDL_INDICATION,
                           QMI_PDC_CONFIG_CHANGE_IND_V01,
                           &svc->pdc_ind.config_change,
                           svc->pdc_ind.config_change_len,
                           svc->pdc_ind.config_change_len,
                           err);
        } 
        else 
        {
          MCFG_MSG_LOW("no cached config_change IND to send");
        }
      }
    }
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Refresh indication registration error %d", err);
  }

  mcfg_free(request);
  mcfg_free(event_reg_info);
} /* END mcfg_svc_cmd_indication_register */


/*===========================================================================
  FUNCTION mcfg_svc_cmd_generate_refresh_event_ind

  DESCRIPTION
    This function generates and sends QMI_PDC & DIAG_CMD indications to all clients that
    have registered for the refresh event registartion type(START/COMPLETE).

  DEPENDENCIES
    QMI_PDC / DIAG_CMD must be initialized

  PARAMETERS
    event_reg_type [in] mcfg_client_event_e_type
    slot_index     [in] slot ID for refresh event
    sub_index      [in] sub_ID for refresh event

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_generate_refresh_event_ind
(
  mcfg_client_event_e_type  event_reg_type,
  mcfg_refresh_index_type   slot_index,
  mcfg_refresh_index_type   sub_index
)
{
  mcfg_refresh_event_s_type *p_event = NULL;
  /*-----------------------------------------------------------------------*/

  p_event = mcfg_malloc(sizeof(mcfg_refresh_event_s_type)); 
  if (!p_event)
  {
    MCFG_MSG_ERROR_1("sending refresh event indication failed %d", QMI_ERR_NO_MEMORY_V01);
    return;
  }

  p_event->mcfg_event = event_reg_type;
  p_event->type = MCFG_REFRESH_TYPE_SLOT_N_SUBS;
  p_event->sub_index = sub_index;
  p_event->slot_index = slot_index;

  MCFG_MSG_LOW("sending refresh event indications to all clients");
  mcfg_svc_cmd_refresh_event_ind(p_event);

  mcfg_free(p_event);
} /* END mcfg_svc_cmd_generate_refresh_event_ind */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_refresh_event_ind

  DESCRIPTION
    send out refresh indications to registered client

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
boolean mcfg_svc_cmd_refresh_event_ind
(
  mcfg_refresh_event_s_type *p_event
)
{
  pdc_refresh_ind_msg_v01 *refresh_event_ind = NULL;
  pdc_service_type *svc = NULL;
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  diag_client_type *diag_client = NULL;
#endif
  mcfg_svc_post_cmd_type post_cmd;
  uint16 rsp_cnt = 1;
  uint32 ind_size = 0;
  int i = 0;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;  
  /*-----------------------------------------------------------------------*/

  MCFG_CHECK_NULL_PTR_RET_FALSE(p_event);
  svc = mcfg_svc_get_service_state();
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  diag_client = (diag_client_type *)mcfg_diag_cmd_get_client_cache(MCFG_SVC_CMD_INDICATION_REGISTER);
  rsp_cnt = (diag_client != NULL) ? diag_client->rsp_cnt : rsp_cnt;
#endif

  do{
    if (!IS_VALID_MCFG_SUBSCRIPTION_ID(p_event->sub_index))
    {
      err = QMI_ERR_NOT_SUPPORTED_V01;
      break;
    }
    if (!IS_VALID_MCFG_SLOT_ID(p_event->slot_index))
    {
      err = QMI_ERR_NOT_SUPPORTED_V01;
      break;
    }

    refresh_event_ind = mcfg_malloc(2 * sizeof(pdc_refresh_ind_msg_v01)); 
    if (!refresh_event_ind)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      break;
    }
    
    /* init indication message */
    memset(refresh_event_ind, 0, sizeof(pdc_refresh_ind_msg_v01));

    if(pdc_qmi_compose_refresh_event_ind(refresh_event_ind,
                                        &ind_size,
                                        p_event->mcfg_event,
                                        p_event->slot_index,
                                        p_event->sub_index) != 0)
    {
      err = QMI_ERR_INTERNAL_V01;
      break;
    }

    /* Send the refresh indication to all clients that are both connected and
       registered for the indication */
    for ( i = 0; i < (PDC_CLIENTS_MAX+1); i++ )
    {
      if ( svc->pdc_clients[i].connected == TRUE )
      {
        if ( svc->pdc_clients[i].event_reg.refresh_event == TRUE )
        {
#ifdef FEATURE_MCFG_DIAG_SUPPORT
          err = mcfg_svc_cmd_generate_post_cmd(&post_cmd, svc->pdc_clients[i].qmi_client_handle, diag_client);
#else
          err = mcfg_svc_cmd_generate_post_cmd(&post_cmd, svc->pdc_clients[i].qmi_client_handle);
#endif
          if(err != QMI_ERR_NONE_V01)
          {
            MCFG_MSG_ERROR_1("post_cmd generate failed with error %d", err);
            continue;
          }

          /* send indication message */
          mcfg_svc_cmd_ind(post_cmd,
                           rsp_cnt,
                           QMI_IDL_INDICATION,
                           QMI_PDC_REFRESH_IND_V01,
                           refresh_event_ind,
                           sizeof(pdc_refresh_ind_msg_v01),
                           2 * sizeof(pdc_refresh_ind_msg_v01),
                           err);

          if (post_cmd.type == QMI_PDC)
          {
            MCFG_MSG_LOW("releasing client memory");
            mcfg_free(post_cmd.data.pdc_post.client);
          }
        }
      }
    } 
  }while(0);

  mcfg_free(refresh_event_ind);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    return FALSE;
  }

  return TRUE;
} /* END mcfg_svc_cmd_refresh_event_ind */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_config_change_ind

  DESCRIPTION
    send out config change indication to registered client

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_config_change_ind
(
  mcfg_config_type_e_type config_type,
  mcfg_config_id_s_type  *config_id
)
{
  pdc_config_change_ind_msg_v01 *config_change_ind = NULL;
  pdc_service_type *svc = NULL;
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  diag_client_type *diag_client = NULL;
#endif
  mcfg_svc_post_cmd_type post_cmd;
  uint16 rsp_cnt = 1;
  uint32 ind_size = 0;
  int i = 0;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;  
  /*-----------------------------------------------------------------------*/

  MCFG_CHECK_NULL_PTR_RET(config_id);
  svc = mcfg_svc_get_service_state();
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  diag_client = (diag_client_type *)mcfg_diag_cmd_get_client_cache(MCFG_SVC_CMD_INDICATION_REGISTER);
  rsp_cnt = (diag_client != NULL) ? diag_client->rsp_cnt : rsp_cnt;
#endif

  do{
    config_change_ind = mcfg_malloc(sizeof(pdc_config_change_ind_msg_v01)); 
    if (!config_change_ind)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      break;
    }
    memset(config_change_ind, 0, sizeof(pdc_config_change_ind_msg_v01));
  
    /* Compose the IND payload */
    if(pdc_qmi_compose_config_change_ind(config_change_ind,
                                          &ind_size,
                                          config_type,
                                          config_id) != 0)
    {
      err = QMI_ERR_INTERNAL_V01;
      break;
    }

    /* Check that the IND has changed since the last IND of this type that was sent out */
    if ((svc->pdc_ind.config_change_len == ind_size) && 
        (0 == memcmp(&svc->pdc_ind.config_change, config_change_ind, ind_size)))
    {
      MCFG_MSG_LOW( "IND matches last, ignoring");
      err = QMI_ERR_NONE_V01;
      break;
    }
    
    /* Update the last IND stored by the service with the new IND payload */
    memscpy(&svc->pdc_ind.config_change, sizeof(pdc_config_change_ind_msg_v01), 
            config_change_ind, ind_size);
    svc->pdc_ind.config_change_len = ind_size;

    /* Send the config indication to all clients that are both connected and
       registered for the indication */
    for ( i = 0; i < (PDC_CLIENTS_MAX+1); i++ )
    {
      if ( svc->pdc_clients[i].connected == TRUE )
      {
        if ( svc->pdc_clients[i].event_reg.selected_config == TRUE )
        {
#ifdef FEATURE_MCFG_DIAG_SUPPORT
           err = mcfg_svc_cmd_generate_post_cmd(&post_cmd, svc->pdc_clients[i].qmi_client_handle, diag_client);
#else
           err = mcfg_svc_cmd_generate_post_cmd(&post_cmd, svc->pdc_clients[i].qmi_client_handle);
#endif
           if(err != QMI_ERR_NONE_V01)
           {
             MCFG_MSG_ERROR_1("post_cmd generate failed with error %d", err);
             continue;
           }

           /* send indication message */
           mcfg_svc_cmd_ind(post_cmd,
                            rsp_cnt,
                            QMI_IDL_INDICATION,
                            QMI_PDC_CONFIG_CHANGE_IND_V01,
                            config_change_ind,
                            sizeof(pdc_config_change_ind_msg_v01),
                            sizeof(pdc_config_change_ind_msg_v01),
                            err);

          if (post_cmd.type == QMI_PDC)
          {
            MCFG_MSG_LOW("releasing client memory");
            mcfg_free(post_cmd.data.pdc_post.client);
          }
        }
      }
    }
  }while(0);

  mcfg_free(config_change_ind);
  
  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
  }
} /* END mcfg_svc_cmd_config_change_ind */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_get_selected_config

  DESCRIPTION
    Processes a mcfg svc command to get selected config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_get_selected_config
(
  mcfg_svc_post_cmd_type post_cmd,
  pdc_get_selected_config_req_msg_v01 *request
)
{
  pdc_get_selected_config_ind_msg_v01 selected_config_ind;
  uint16 rsp_cnt = 1;
  mcfg_config_id_s_type config_id;
  mcfg_sub_id_type_e_type sub = 0;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/
  
  /* init indication message */
  memset(&selected_config_ind, 0, sizeof(pdc_get_selected_config_ind_msg_v01));

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    selected_config_ind.ind_token_valid = request->ind_token_valid;  
    selected_config_ind.ind_token = request->ind_token;

    if (request->subscription_id_valid == FALSE)
    {
      if (request->slot_id_valid == FALSE)
      {
        sub = MCFG_SUB_ID_FIRST;
      }
      else
      {
        sub = mcfg_uim_map_slot_index_to_sub_id(request->slot_id);
      }
    }
    else
    {
      sub = request->subscription_id;
    }

    /* generate indication message */
    if(IS_VALID_MCFG_SUBSCRIPTION_ID(sub))
    {
      if (mcfg_utils_get_active_config(request->config_type, &config_id, sub))
      {
        MCFG_MSG_LOW_1("active config id length %d", config_id.id_len);
        if (config_id.id_len > 0 && 
            config_id.id_len <= PDC_CONFIG_ID_SIZE_MAX_V01)
        {
          selected_config_ind.active_config_id_len = config_id.id_len;
          memscpy(selected_config_ind.active_config_id, PDC_CONFIG_ID_SIZE_MAX_V01,
                  config_id.id, config_id.id_len);
          selected_config_ind.active_config_id_valid = TRUE;
          MCFG_MSG_LOW_4("sub %d active config id %02x%02x%02x", 
                      sub, selected_config_ind.active_config_id[0], selected_config_ind.active_config_id[1], selected_config_ind.active_config_id[2]);
        }
      }

      if (mcfg_utils_get_selected_config(request->config_type, &config_id, sub))
      {
        MCFG_MSG_LOW_1("pending config id length %d", config_id.id_len);
        if (config_id.id_len > 0 && 
            config_id.id_len <= PDC_CONFIG_ID_SIZE_MAX_V01)
        {
          if (!selected_config_ind.active_config_id_valid 
              || config_id.id_len != selected_config_ind.active_config_id_len 
              || (memcmp(config_id.id, selected_config_ind.active_config_id, 
                         config_id.id_len) != 0))
          {
            selected_config_ind.pending_config_id_len = config_id.id_len;
            memscpy(selected_config_ind.pending_config_id, 
                    PDC_CONFIG_ID_SIZE_MAX_V01, config_id.id, config_id.id_len);
            selected_config_ind.pending_config_id_valid = TRUE;
            MCFG_MSG_LOW_4("sub %d pending config id %02x%02x%02x", 
              sub, selected_config_ind.pending_config_id[0], selected_config_ind.pending_config_id[1], selected_config_ind.pending_config_id[2]);
          }
        }
      }
    }

    selected_config_ind.error = (!selected_config_ind.active_config_id_valid && !selected_config_ind.pending_config_id_valid) ? QMI_ERR_NOT_PROVISIONED_V01 : QMI_ERR_NONE_V01;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_GET_SELECTED_CONFIG_IND_V01,
                     (void *)&selected_config_ind,
                     sizeof(pdc_get_selected_config_ind_msg_v01),
                     sizeof(pdc_get_selected_config_ind_msg_v01),
                     err);
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    selected_config_ind.error = err;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_GET_SELECTED_CONFIG_IND_V01,
                     (void *)&selected_config_ind,
                     sizeof(pdc_get_selected_config_ind_msg_v01),
                     sizeof(pdc_get_selected_config_ind_msg_v01),
                     err);
  }

  mcfg_free(request);
}/* END mcfg_svc_cmd_get_selected_config */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_set_selected_config

  DESCRIPTION
    Processes a mcfg svc command to set selected config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_set_selected_config
(
  mcfg_svc_post_cmd_type post_cmd,
  pdc_set_selected_config_req_msg_v01 *request
)
{
  pdc_set_selected_config_ind_msg_v01 selected_config_ind;
  uint16 rsp_cnt = 1;
  mcfg_config_id_s_type config_id;
  mcfg_sub_id_type_e_type sub = 0;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  /* init indication message */
  memset(&selected_config_ind, 0, sizeof(pdc_set_selected_config_ind_msg_v01));

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    selected_config_ind.ind_token_valid = request->ind_token_valid;  
    selected_config_ind.ind_token = request->ind_token;

    if (request->subscription_id_valid == FALSE)
    {
      if (request->slot_id_valid == FALSE)
      {
        sub = MCFG_SUB_ID_FIRST;
      }
      else
      {
        sub = mcfg_uim_map_slot_index_to_sub_id(request->slot_id);
      }
    }
    else
    {
      if (request->slot_id_valid == FALSE)
      {
        sub = request->subscription_id;
      }
      else if (request->subscription_id == mcfg_uim_map_slot_index_to_sub_id(request->slot_id))
      {
        sub = request->subscription_id;
      }
      else if (request->new_config_info.config_type == MCFG_TYPE_HW)
      {
        MCFG_MSG_ERROR("Cross mapping for HW MBN is not allowed");
        err = QMI_ERR_NOT_PROVISIONED_V01;
        break;
      }
      else if (mcfg_utils_get_active_config(request->new_config_info.config_type, &config_id, request->subscription_id) || 
               mcfg_utils_get_active_config(request->new_config_info.config_type, &config_id, mcfg_uim_map_slot_index_to_sub_id(request->slot_id)))
      {
        MCFG_MSG_ERROR_2("Deactivate existing mbn before changing mapping slot%lu sub%lu ", request->slot_id, request->subscription_id);
        err = QMI_ERR_NOT_PROVISIONED_V01;
        break;
      }
      else
      {
        MCFG_MSG_HIGH_2("set_selected_config changed mapping to slot%lu sub%lu ", request->slot_id, request->subscription_id);
        sub = request->subscription_id;
        mcfg_uim_process_mapping(request->slot_id, request->subscription_id, FALSE);
      }
    }

    /* generate indication message */
    if (request->new_config_info.config_id_len == 0 ||
        request->new_config_info.config_id_len > PDC_CONFIG_ID_SIZE_MAX_V01)
    {
      err = QMI_ERR_INVALID_ID_V01;
    }
    else
    {
      config_id.id_len = request->new_config_info.config_id_len;
      memscpy(config_id.id, PDC_CONFIG_ID_SIZE_MAX_V01, 
              request->new_config_info.config_id, config_id.id_len);
     
      if (mcfg_utils_is_config_id_unique(request->new_config_info.config_type, &config_id))
      {
        err = QMI_ERR_INVALID_ID_V01;
      }
      else if (mcfg_utils_set_selected_config(request->new_config_info.config_type, &config_id, sub))
      {
        err = QMI_ERR_NONE_V01;
      }
      else
      {
        err = QMI_ERR_INTERNAL_V01;
      }
    }

    selected_config_ind.error = err;

    /* send indication message */
    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_SET_SELECTED_CONFIG_IND_V01,
                     (void *)&selected_config_ind,
                     sizeof(pdc_set_selected_config_ind_msg_v01),
                     sizeof(pdc_set_selected_config_ind_msg_v01),
                     err);

    if (selected_config_ind.error == QMI_ERR_NONE_V01)
    {
      mcfg_svc_cmd_config_change_ind(request->new_config_info.config_type,
                                     &config_id);
    }
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    selected_config_ind.error = err;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_SET_SELECTED_CONFIG_IND_V01,
                     (void *)&selected_config_ind,
                     sizeof(pdc_set_selected_config_ind_msg_v01),
                     sizeof(pdc_set_selected_config_ind_msg_v01),
                     err);
  }

  mcfg_free(request);
}/* END mcfg_svc_cmd_set_selected_config */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_list_config

  DESCRIPTION
    Processes a mcfg svc command to list configs in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_list_config
(
  mcfg_svc_post_cmd_type post_cmd,
  pdc_list_configs_req_msg_v01 *request
)
{
  pdc_list_configs_ind_msg_v01 *p_list_configs_ind = NULL;
  pdc_list_config_cache_type *cache = NULL;
  mcfg_config_id_s_type *p_id_list = NULL;
  mcfg_config_id_s_type *p_id_list_i = NULL;
  pdc_config_type_enum_v01 type;
  boolean multi_support = TRUE;
  uint32_t list_ret = 0;
  uint16_t ind_count = 0;
  uint16_t num_of_sent = 0;
  uint16 rsp_cnt = 1;
  uint8_t i;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  uint32_t packed_size = 0;
  mcfg_svc_client_type    *client = NULL;
  /*-----------------------------------------------------------------------*/

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    p_list_configs_ind = mcfg_malloc(sizeof(pdc_list_configs_ind_msg_v01));
    if (!p_list_configs_ind)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      break;
    }

    p_id_list = mcfg_malloc(((multi_support)? MCFG_CONFIG_LIST_SIZE_MAX : PDC_CONFIG_LIST_SIZE_MAX_V01)
                            * sizeof(mcfg_config_id_s_type));
    if (!p_id_list)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      break;
    }

    /* init indication message */
    memset(p_list_configs_ind, 0, sizeof(pdc_list_configs_ind_msg_v01));
    memset(p_id_list, 0, sizeof(((multi_support)? MCFG_CONFIG_LIST_SIZE_MAX : PDC_CONFIG_LIST_SIZE_MAX_V01)
                            * sizeof(mcfg_config_id_s_type)));

    p_list_configs_ind->ind_token_valid = TRUE;  
    p_list_configs_ind->ind_token = request->ind_token;
    p_list_configs_ind->config_list_len = 0;
    type = request->config_type;

    client = mcfg_malloc(sizeof(mcfg_svc_client_type));
    if (!client)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      break;
    }

    if (post_cmd.type == QMI_PDC)
    {
      client->qmi_client_handle = post_cmd.data.pdc_post.client->qmi_client_handle;
    }
    else if (post_cmd.type == DIAG_CMD)
    {
      client->qmi_client_handle =  (struct qmi_client_handle_struct *)&DIAG_CLIENT_HANDLE;
    }

	mcfg_svc_get_list_config_cache(mcfg_svc_get_service_state(), client, &cache);
    if (!cache)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      break;
    }

    /* generate indication message */
    do
    {

      if (cache->config_list != NULL)
      {
        /* this is subsequential request, validate with cached information */
        if (!mcfg_svc_cmd_client_match(post_cmd, cache, MCFG_SVC_CMD_LIST_CONFIG) || 
            request->ind_token_valid == FALSE || 
            request->ind_token != cache->ind_token ||
            request->multi_support_valid == FALSE || 
            request->multi_support == FALSE || 
            request->config_type_valid == FALSE)
        {
          MCFG_MSG_LOW_1("reset config_list for cached token %d", cache->ind_token);
		  mcfg_svc_default_list_config_cache(cache);
        }
      }
      else
      {
        /* this is a new request, update the cache */
        if (request->config_type_valid && request->multi_support_valid && request->multi_support)
        {
          mcfg_svc_cmd_cache_update(post_cmd, cache, MCFG_SVC_CMD_LIST_CONFIG);
          cache->ind_token = request->ind_token;
          MCFG_MSG_LOW_1("ind_token %x", cache->ind_token);
        }

        list_ret = mcfg_utils_list_configs(request->config_type, MCFG_CONFIG_LIST_SIZE_MAX, p_id_list);
        MCFG_MSG_LOW_1("client multiple list_ret %d", list_ret);

        if (list_ret == 0)
        {
          err = QMI_ERR_NONE_V01;
          break;
        }
        cache->config_list = mcfg_malloc(list_ret * sizeof(mcfg_config_id_s_type));
        if (!cache->config_list)
        {
          err = QMI_ERR_INTERNAL_V01;
          break;
        }
        memscpy(cache->config_list, list_ret * sizeof(mcfg_config_id_s_type),
                p_id_list, list_ret * sizeof(mcfg_config_id_s_type));

        cache->total = list_ret;
        cache->num_of_sent = 0;
      }
      /* Consider all elements of 'pdc_list_configs_ind_msg_v01' structure except 'config_list'
         which will be calculated inside the for loop dynamically. */
      packed_size = sizeof(pdc_list_configs_ind_msg_v01) - sizeof(p_list_configs_ind->config_list);
      for(i=0; i<PDC_CONFIG_LIST_SIZE_MAX_V01 && ((cache->num_of_sent + i) < cache->total); i++)
      {
        p_id_list_i = cache->config_list + cache->num_of_sent + i;
        packed_size += sizeof(request->config_type) + sizeof(p_id_list_i->id_len) + p_id_list_i->id_len;
        if (packed_size > PDC_QMI_PACKET_MAX_LIMIT)
          break;
        p_list_configs_ind->config_list[i].config_type = request->config_type;
        p_list_configs_ind->config_list[i].config_id_len = p_id_list_i->id_len;
        memscpy(p_list_configs_ind->config_list[i].config_id, PDC_CONFIG_ID_SIZE_MAX_V01, 
                p_id_list_i->id, p_id_list_i->id_len);
        p_list_configs_ind->config_list_len ++;
      }

      cache->num_of_sent += p_list_configs_ind->config_list_len;
      if(cache->num_of_sent >= cache->total)
      {
        mcfg_svc_default_list_config_cache(cache);
        p_list_configs_ind->more_available_valid = TRUE;
        p_list_configs_ind->more_available = FALSE;
      }
      else
      {
        p_list_configs_ind->more_available_valid = TRUE;
        p_list_configs_ind->more_available = TRUE;    
      }

      MCFG_MSG_LOW_2("client multiple list_ret total %d num_of_sent %d", cache->total, cache->num_of_sent);
    }while(0);

    p_list_configs_ind->error = err;
    p_list_configs_ind->config_list_valid = (err == QMI_ERR_NONE_V01) ? TRUE : FALSE;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_LIST_CONFIGS_IND_V01,
                     p_list_configs_ind,
                     sizeof(pdc_list_configs_ind_msg_v01),
                     PDC_CONFIG_ID_SIZE_MAX_V01 * (PDC_CONFIG_LIST_SIZE_MAX_V01 + 1),
                     err);
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    if (p_list_configs_ind != NULL)
    {
      p_list_configs_ind->error = err;
      p_list_configs_ind->config_list_valid = FALSE;

      mcfg_svc_cmd_ind(post_cmd,
                       rsp_cnt,
                       QMI_IDL_INDICATION,
                       QMI_PDC_LIST_CONFIGS_IND_V01,
                       p_list_configs_ind,
                       sizeof(pdc_list_configs_ind_msg_v01),
                       PDC_CONFIG_ID_SIZE_MAX_V01 * (PDC_CONFIG_LIST_SIZE_MAX_V01 + 1),
                       err);
    }
  }

  mcfg_free(p_list_configs_ind);
  mcfg_free(p_id_list);
  mcfg_free(request);
  mcfg_free(client);
}/* END mcfg_svc_cmd_list_config */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_delete_config

  DESCRIPTION
    Processes a mcfg svc command to delete config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_delete_config
(
   mcfg_svc_post_cmd_type post_cmd,
   pdc_delete_config_req_msg_v01 *request
)
{
  pdc_delete_config_ind_msg_v01 delete_config_ind;
  mcfg_config_id_s_type  id;
  uint16 rsp_cnt = 1;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  mcfg_error_e_type mcfg_err = MCFG_ERR_NONE;
  /*-----------------------------------------------------------------------*/

  /* init indication message */
  memset(&delete_config_ind, 0, sizeof(pdc_delete_config_ind_msg_v01));

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    delete_config_ind.ind_token_valid = request->ind_token_valid;  
    delete_config_ind.ind_token = request->ind_token;

    /* generate indication message */
    if (request->config_id_valid == FALSE)
    {
      mcfg_err = mcfg_utils_delete_all(request->config_type, MCFG_STORAGE_MASK_ALL, FALSE);
      err = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;
    }
    else
    {
      id.id_len = request->config_id_len;
      memscpy(id.id, MCFG_CONFIG_ID_SIZE_MAX, request->config_id, id.id_len);

      if (mcfg_utils_is_config_id_unique(request->config_type, &id))
      {
        err = QMI_ERR_INVALID_ID_V01;
      }
      else
      {
        mcfg_err = mcfg_utils_delete_config(request->config_type, &id, MCFG_STORAGE_MASK_ALL);
        err = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;
      }
    }

    delete_config_ind.error = err;

    /* send indication message */
    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_DELETE_CONFIG_IND_V01,
                     (void *)&delete_config_ind,
                     sizeof(pdc_delete_config_ind_msg_v01),
                     sizeof(pdc_delete_config_ind_msg_v01),
                     err);
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    delete_config_ind.error = err;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_DELETE_CONFIG_IND_V01,
                     (void *)&delete_config_ind,
                     sizeof(pdc_delete_config_ind_msg_v01),
                     sizeof(pdc_delete_config_ind_msg_v01),
                     err);
  }

  mcfg_free(request);
}/* END mcfg_svc_cmd_delete_config */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_load_config

  DESCRIPTION
    Processes a mcfg svc command to load config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_load_config
(
   mcfg_svc_post_cmd_type post_cmd,
   pdc_load_config_req_msg_v01 *request
)
{
  pdc_load_config_ind_msg_v01 load_config_ind;
  pdc_load_config_cache_type *cache = NULL;
  pdc_service_type *svc = NULL;
  uint16 rsp_cnt = 1;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  mcfg_error_e_type mcfg_err = MCFG_ERR_NONE;
  /*-----------------------------------------------------------------------*/
  
  /* init indication message */
  memset(&load_config_ind, 0, sizeof(pdc_load_config_ind_msg_v01));

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    load_config_ind.ind_token_valid = request->ind_token_valid;  
    load_config_ind.ind_token = request->ind_token;

    svc = mcfg_svc_get_service_state();
    cache = &svc->load_config_cache;

    /* generate indication message */
    if (!cache->config_s.config_addr)
    {
      /* this is first request, allocate memory buffer */
      MCFG_MSG_LOW("config_buf NULL");
      mcfg_svc_cmd_cache_update(post_cmd, cache, MCFG_SVC_CMD_LOAD_CONFIG);

      cache->config_s.config_len = request->load_config_info.total_config_size;

      if (request->storage_valid == FALSE)
      {
         cache->storage = (mcfg_storage_mode_e_type) PDC_STORAGE_LOCAL_V01;
      }
      else 
      {
         cache->storage = request->storage;
      }

      cache->type = request->load_config_info.config_type;

      cache->id_s.id_len = request->load_config_info.config_id_len;
      memscpy(cache->id_s.id, MCFG_CONFIG_ID_SIZE_MAX, 
              request->load_config_info.config_id, 
              request->load_config_info.config_id_len);

      /* check ID and alloc memory buffer */
      if (!mcfg_utils_is_config_id_unique(request->load_config_info.config_type,
                                          &cache->id_s))
      {
        MCFG_MSG_ERROR_1( "id exists len %lu", 
                          request->load_config_info.config_id_len );
        err = QMI_ERR_INVALID_ID_V01;
      }
      else if (!mcfg_utils_alloc_config_buffer(request->load_config_info.config_type,
                                     &cache->config_s))
      {

        MCFG_MSG_ERROR_1( "mem alloc failed with size %d", 
                          cache->config_s.config_len );
        err = QMI_ERR_NO_MEMORY_V01;
      }
    }

    if (!MCFG_SIZE_OFFSET_WITHIN_LIMIT(request->load_config_info.config_frame_len, 
           cache->offset, cache->config_s.config_len) ||
         MCFG_INTERGER_OVERFLOWED(cache->config_s.config_addr, cache->offset) ||
         (request->load_config_info.config_frame_len > MCFG_CONFIG_FRAME_SIZE_MAX))
    {
      /* wrong sum, send err indication and reset cache */
      MCFG_MSG_ERROR_2( "offset %lu overflow config len %d", 
                        cache->offset, cache->config_s.config_len);
      err = QMI_ERR_INVALID_ARG_V01;
    }

    /* Add frame len check again to avoid klocwork warnings */
    if (err != QMI_ERR_NONE_V01 || 
        request->load_config_info.config_frame_len > MCFG_CONFIG_FRAME_SIZE_MAX)
    {
      load_config_ind.frame_data_reset = TRUE;
      load_config_ind.frame_data_reset_valid = TRUE;
      mcfg_svc_default_load_config_cache(cache);
    }
    else
    {
      memscpy((uint8 *)cache->config_s.config_addr + cache->offset,
              MCFG_CONFIG_SIZE_MAX,
              request->load_config_info.config_frame,
              request->load_config_info.config_frame_len );

      cache->offset += request->load_config_info.config_frame_len;

      load_config_ind.received_config_size = request->load_config_info.config_frame_len;
      load_config_ind.received_config_size_valid = TRUE;

      load_config_ind.remaining_config_size = cache->config_s.config_len - cache->offset;
      load_config_ind.remaining_config_size_valid = TRUE;

      if (cache->config_s.config_len > cache->offset)
      {
        /* send indication and wait for next frame */
        MCFG_MSG_LOW_1( "remaining %d and wait for next frame", 
                          load_config_ind.remaining_config_size );
      }
      else
      {
        /* no more frame, save config to efs and clear cache */
        MCFG_MSG_LOW("last frame, save and clear cache");

        mcfg_err = mcfg_utils_add_config(request->load_config_info.config_type, 
                                           &cache->id_s, &cache->config_s, cache->storage);

        err = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;
        if(err != QMI_ERR_NONE_V01)
        {
          MCFG_MSG_ERROR_1( "failed to save config, reset frame (0x%x)", mcfg_err);
          load_config_ind.frame_data_reset = TRUE;
          load_config_ind.frame_data_reset_valid = TRUE;
        }
        else
        {
          mcfg_utils_efs_sync();
        }

        mcfg_svc_default_load_config_cache(cache);
      }
    }

    load_config_ind.error = err;

    /* send indication message */
    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_LOAD_CONFIG_IND_V01,
                     (void *)&load_config_ind,
                     sizeof(pdc_load_config_ind_msg_v01),
                     sizeof(pdc_load_config_ind_msg_v01),
                     err);
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    load_config_ind.error = err;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_LOAD_CONFIG_IND_V01,
                     (void *)&load_config_ind,
                     sizeof(pdc_load_config_ind_msg_v01),
                     sizeof(pdc_load_config_ind_msg_v01),
                     err);
  }

  mcfg_free(request);
}/* END mcfg_svc_cmd_load_config */


/*==============================================================================

  FUNCTION mcfg_svc_default_load_config_cache

  DESCRIPTION
    This function initialize / reinitialize the cached load config
    information.

  DEPENDENCIES
    QCSI/QSAP must be initialized

  PARAMETERS
    cache [in] - load_config_cache

  RETURN VALUE
    None

  SIDE EFFECTS
    
==============================================================================*/
void mcfg_svc_default_load_config_cache
( 
  pdc_load_config_cache_type *cache
)
{
  
  /*-----------------------------------------------------------------------*/

  if (NULL == cache)
  {
    MCFG_MSG_ERROR( "cache NULL pointer error");
    return;
  }
  
  if (cache->config_s.config_addr)
  {
    mcfg_utils_free_config_buffer(&cache->config_s);
  }

  memset(cache, 0, sizeof(pdc_load_config_cache_type));

  MCFG_MSG_LOW( "cache is now reset");
  return;
} /* mcfg_svc_default_load_config_cache() */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_activate_config

  DESCRIPTION
    Processes a mcfg svc command to activate config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_activate_config
(
   mcfg_svc_post_cmd_type post_cmd,
   pdc_activate_config_req_msg_v01 *request
)
{
  pdc_activate_config_ind_msg_v01 activate_config_ind;
  uint16 rsp_cnt = 1;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  mcfg_error_e_type mcfg_error = MCFG_ERR_NONE;
#ifdef FEATURE_MCFG_REFRESH
  pdc_activate_cache_type *p_cache = NULL;
  pdc_service_type *svc = NULL;
  uint8 slot = 0;
#endif
  /*-----------------------------------------------------------------------*/
  
  /* init indication message */
  memset(&activate_config_ind, 0, sizeof(pdc_activate_config_ind_msg_v01));

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

  activate_config_ind.ind_token_valid = request->ind_token_valid;  
  activate_config_ind.ind_token = request->ind_token;

#ifdef FEATURE_MCFG_REFRESH
    if (request->activation_type_valid && 
        request->activation_type == PDC_ACTIVATION_REFRESH_ONLY_V01)
    {
      MCFG_MSG_LOW("refresh only");
      if (request->slot_id_valid == TRUE 
          && IS_VALID_MCFG_SLOT_ID(request->slot_id) 
          && request->subscription_id_valid == TRUE 
          && IS_VALID_MCFG_SUBSCRIPTION_ID(request->subscription_id)
          && mcfg_refresh_enabled(request->slot_id))
      {
        MCFG_MSG_LOW_1("refresh sloti=%d", request->slot_id);
        mcfg_error = mcfg_utils_activate_refreshonly(request->slot_id, request->subscription_id);
        err = (mcfg_error < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_error : QMI_ERR_INTERNAL_V01;
        if(err == QMI_ERR_NONE_V01)
        {
          activate_config_ind.error = err;
          svc = mcfg_svc_get_service_state();
          p_cache = &svc->activate_cache[request->slot_id];
          /* indication will be sent out later */
          p_cache->ind_token_valid = request->ind_token_valid;
          p_cache->ind_token = request->ind_token;
          mcfg_svc_cmd_cache_update(post_cmd, p_cache, MCFG_SVC_CMD_ACTIVATE_CONFIG);

          mcfg_free(request);
          return;
        }
      }
      else if (!request->slot_id_valid && !request->subscription_id_valid ) 
      {
        if(mcfg_refresh_is_ongoing(MCFG_REFRESH_INDEX_MASK_ALL))
        {
          MCFG_MSG_ERROR("APP centric refreshing all rejected with refresh ongoing");
          err =  QMI_ERR_DEVICE_IN_USE_V01;
        }
        else
        {
          MCFG_MSG_LOW("ACTIVATE refresh together");
          svc = mcfg_svc_get_service_state();
          memset(&svc->activate_cache[0], 0, (MCFG_NUM_OF_UIM_SLOT+1)*sizeof(pdc_activate_cache_type));

          p_cache = &svc->activate_cache[request->slot_id];
          /* indication will be sent out later */
          p_cache->ind_token_valid = request->ind_token_valid;
          p_cache->ind_token = request->ind_token;
          mcfg_svc_cmd_cache_update(post_cmd, p_cache, MCFG_SVC_CMD_ACTIVATE_CONFIG);

          mcfg_error = mcfg_refresh_all_by_apps();
          err = (mcfg_error < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_error : QMI_ERR_INTERNAL_V01;
          if(err == QMI_ERR_NONE_V01)
          {
            activate_config_ind.error = err;

            /* indication will be sent out later */
            mcfg_free(request);
            return;
          }
          else
          {
            memset(&svc->activate_cache[0], 0, (MCFG_NUM_OF_UIM_SLOT+1)*sizeof(pdc_activate_cache_type));
          }
        }
      }
      else
      {
        err = QMI_ERR_NOT_SUPPORTED_V01;
      }
    }
    else
#endif
    {
      mcfg_error = mcfg_utils_activate_config(request->config_type, MCFG_ACTIVATION_BY_PDC); 
      
      if(mcfg_error != MCFG_ERR_NONE)
      {
        err = (mcfg_error < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_error : QMI_ERR_INTERNAL_V01;
      }
      else
      {
#ifdef FEATURE_MCFG_REFRESH
        err = QMI_ERR_NONE_V01;
        svc = mcfg_svc_get_service_state();
  
        for (slot = MCFG_SUB_ID_FIRST; slot < MCFG_NUM_OF_UIM_SLOT; slot++)
        {
          p_cache = &svc->activate_cache[slot];
          /* indication will be sent out later */
          p_cache->ind_token_valid = request->ind_token_valid;
          p_cache->ind_token = request->ind_token;
          mcfg_svc_cmd_cache_update(post_cmd, p_cache, MCFG_SVC_CMD_ACTIVATE_CONFIG);
        }

        activate_config_ind.error = err;

        mcfg_free(request);
        return;
#endif
      }
    }

    activate_config_ind.error = err;

    /* send indication message */
    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_ACTIVATE_CONFIG_IND_V01,
                     (void *)&activate_config_ind,
                     sizeof(pdc_activate_config_ind_msg_v01),
                     sizeof(pdc_activate_config_ind_msg_v01),
                     err);
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    activate_config_ind.error = err;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_ACTIVATE_CONFIG_IND_V01,
                     (void *)&activate_config_ind,
                     sizeof(pdc_activate_config_ind_msg_v01),
                     sizeof(pdc_activate_config_ind_msg_v01),
                     err);
  }

  mcfg_free(request);
}/* END mcfg_svc_cmd_activate_config */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_cached_activate_config_ind

  DESCRIPTION
    sends out indications after activate refresh only req/response are processed.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_cached_activate_config_ind
(
  uint8 slot_index,
  mcfg_error_e_type mcfg_err
)
{
  pdc_activate_config_ind_msg_v01 activate_config_ind;
  pdc_activate_cache_type *p_cache = NULL;
  pdc_service_type *svc = NULL;
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  diag_client_type *diag_client = NULL;
#endif
  mcfg_svc_post_cmd_type post_cmd;
  uint16 rsp_cnt = 1;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/

  svc = mcfg_svc_get_service_state();
  p_cache = &svc->activate_cache[slot_index];
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  diag_client = (diag_client_type *)mcfg_diag_cmd_get_client_cache(MCFG_SVC_CMD_ACTIVATE_CONFIG);
  rsp_cnt = (diag_client != NULL) ? diag_client->rsp_cnt : rsp_cnt;
#endif

  do
  {
    MCFG_ASSERT_COND_RET(IS_VALID_MCFG_SLOT_ID(slot_index));

    /* init indication message */
    memset(&activate_config_ind, 0, sizeof(pdc_activate_config_ind_msg_v01));
    
    if (p_cache->qmi_client_handle)
    {
      activate_config_ind.ind_token_valid = p_cache->ind_token_valid;  
      activate_config_ind.ind_token = p_cache->ind_token ;
      activate_config_ind.error = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;
#ifdef FEATURE_MCFG_DIAG_SUPPORT
      err = mcfg_svc_cmd_generate_post_cmd(&post_cmd, p_cache->qmi_client_handle, diag_client);
#else
      err = mcfg_svc_cmd_generate_post_cmd(&post_cmd, p_cache->qmi_client_handle);
#endif
      if(err != QMI_ERR_NONE_V01)
      {
        /* failed to generate post_cmd */
        MCFG_MSG_ERROR_1("post_cmd generate failed with error %d", err);
        return;
      }

      /* send indication message */
      mcfg_svc_cmd_ind(post_cmd,
                       rsp_cnt,
                       QMI_IDL_INDICATION,
                       MCFG_SVC_CMD_ACTIVATE_CONFIG,
                       (void *)&activate_config_ind,
                       sizeof(pdc_activate_config_ind_msg_v01),
                       sizeof(pdc_activate_config_ind_msg_v01),
                       err);

      memset(p_cache, 0, sizeof(pdc_activate_cache_type));
    }
    else
    {
      p_cache->done = TRUE;
      p_cache->error = mcfg_err;

      for (slot_index = 0; slot_index < MCFG_NUM_OF_UIM_SLOT; slot_index++)
      {
        p_cache = &svc->activate_cache[slot_index];

        if (p_cache->done == FALSE)
        {
          MCFG_MSG_LOW_1("Refresh all is not yet completed for sloti %d, wait...", slot_index);
          return;
        }

        if (p_cache->error != MCFG_ERR_NONE)
        {
          activate_config_ind.error = (p_cache->error < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)p_cache->error : QMI_ERR_INTERNAL_V01;
        }
      }

      MCFG_MSG_LOW("Refresh all completed");

      p_cache = &svc->activate_cache[MCFG_NUM_OF_UIM_SLOT];

      activate_config_ind.ind_token_valid = p_cache->ind_token_valid;
      activate_config_ind.ind_token = p_cache->ind_token;
      activate_config_ind.error = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;
#ifdef FEATURE_MCFG_DIAG_SUPPORT
      err = mcfg_svc_cmd_generate_post_cmd(&post_cmd, p_cache->qmi_client_handle, diag_client);
#else
      err = mcfg_svc_cmd_generate_post_cmd(&post_cmd, p_cache->qmi_client_handle);
#endif
      if(err != QMI_ERR_NONE_V01)
      {
        /* failed to generate post_cmd */
        MCFG_MSG_ERROR_1("post_cmd generate failed with error %d", err);
        return;
      }

      /* send indication message */
      mcfg_svc_cmd_ind(post_cmd,
                       rsp_cnt,
                       QMI_IDL_INDICATION,
                       MCFG_SVC_CMD_ACTIVATE_CONFIG,
                       (void *)&activate_config_ind,
                       sizeof(pdc_activate_config_ind_msg_v01),
                       sizeof(pdc_activate_config_ind_msg_v01),
                       err);

      memset(&svc->activate_cache[0], 0, (MCFG_NUM_OF_UIM_SLOT + 1) * sizeof(pdc_activate_cache_type));
    }
  }while(0);

  if (post_cmd.type == QMI_PDC)
  {
    MCFG_MSG_LOW("releasing client memory");
    mcfg_free(post_cmd.data.pdc_post.client);
  }
} /* END mcfg_svc_cmd_cached_activate_config_ind */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_get_config_info

  DESCRIPTION
    Processes a mcfg svc command to get config info in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_get_config_info
(
   mcfg_svc_post_cmd_type post_cmd,
   pdc_get_config_info_req_msg_v01 *request
)
{
  pdc_get_config_info_ind_msg_v01 config_info_ind;
  uint16 rsp_cnt = 1;
  mcfg_config_id_s_type config_id;
  mcfg_config_info_s_type config_info;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/
  
  /* init indication message */
  memset(&config_info_ind, 0, sizeof(pdc_get_config_info_ind_msg_v01));
  memset(&config_info, 0, sizeof(mcfg_config_info_s_type));

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    config_info_ind.ind_token_valid = request->ind_token_valid;  
    config_info_ind.ind_token = request->ind_token;

    config_id.id_len = request->new_config_info.config_id_len;
    memscpy(config_id.id, MCFG_CONFIG_ID_SIZE_MAX,
            request->new_config_info.config_id, config_id.id_len);

    /* Verify the config ID exists then build the indication */
    if (mcfg_utils_is_config_id_unique(request->new_config_info.config_type,
                                       &config_id))
    {
      err = QMI_ERR_INVALID_ID_V01;
    }
    else
    {
      if (mcfg_utils_get_config_info(request->new_config_info.config_type, 
                                     &config_id, 
                                     &config_info))
      {
        config_info_ind.config_size_valid = TRUE;
        config_info_ind.config_size = config_info.config_len;

        if (mcfg_trl_decode_tlv(&config_info, 
                                MCFG_TRL_TLV_ID_CARRIER_NAME, 
                                config_info_ind.config_desc, 
                                MCFG_TRL_CARRIER_NAME_MAX_LEN+1))
        {
          config_info_ind.config_desc_len = strlen((char *)config_info_ind.config_desc);
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

        if (mcfg_trl_decode_tlv(&config_info, MCFG_TRL_TLV_ID_BASE_VERSION, 
                                &config_info_ind.base_version, 
                                sizeof(config_info_ind.base_version)))
        {
          config_info_ind.base_version_valid = TRUE;
        }

        config_info_ind.storage = (pdc_storage_enum_v01) config_info.storage;
        config_info_ind.storage_valid = TRUE;
        if (config_info_ind.storage == PDC_STORAGE_REMOTE_V01)
        {
           memscpy(config_info_ind.path, MCFG_CONFIG_PATH_SIZE_MAX,
                   config_info.path, MCFG_CONFIG_PATH_SIZE_MAX);
           config_info_ind.path_valid = TRUE;
           err = QMI_ERR_NONE_V01;
        }
        else if (config_info_ind.storage == PDC_STORAGE_LOCAL_V01)
        {
           config_info_ind.path_valid = FALSE;
           err = QMI_ERR_NONE_V01;
        }
        else 
        {
           config_info_ind.storage_valid = FALSE;
           err = QMI_ERR_INTERNAL_V01;
        }
      }
      else
      {
        err = QMI_ERR_INTERNAL_V01;
      }
    }

    config_info_ind.error = err;

    /* send indication message */
    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_GET_CONFIG_INFO_IND_V01,
                     (void *)&config_info_ind,
                     sizeof(pdc_get_config_info_ind_msg_v01),
                     sizeof(pdc_get_config_info_ind_msg_v01),
                     err);
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    config_info_ind.error = err;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_GET_CONFIG_INFO_IND_V01,
                     (void *)&config_info_ind,
                     sizeof(pdc_get_config_info_ind_msg_v01),
                     sizeof(pdc_get_config_info_ind_msg_v01),
                     err);
  }

  mcfg_free(request);
}/* END mcfg_svc_cmd_get_config_info */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_validate_config

  DESCRIPTION
    Processes a mcfg svc command to validate configs in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_validate_config
(
  mcfg_svc_post_cmd_type post_cmd,
  pdc_validate_config_req_msg_v01 *request
)
{
  pdc_validate_config_ind_msg_v01 *validate_config_ind = NULL;
  uint16 rsp_cnt = 0x8001;
  mcfg_config_id_s_type config_id;
  mcfg_config_type_e_type type;
  mcfg_sub_id_type_e_type sub;
#ifdef FEATURE_MCFG_DIAG_SUPPORT
  uint32 max_diagQ_size = 0;
  uint32 curr_diagQ_size = 0;
#endif
  uint8_t j = 0;
  uint8 pet_dog = 0;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  mcfg_error_e_type mcfg_err = MCFG_ERR_NONE;
  /*-----------------------------------------------------------------------*/
  // For XML reading
  /*-----------------------------------------------------------------------*/
  struct fs_stat     temp_buf;
  int                fd=-1;
  char               f_name[128]; /* Increase size from 120 to 128 for context*/
  fs_size_t          nbytes;
  fs_size_t          bytes_read;
  fs_size_t          read_bytes;
  fs_size_t          bytes_left;
  boolean            read_success;
  fs_ssize_t         result = 0;
  /*-----------------------------------------------------------------------*/

  validate_config_ind = mcfg_malloc(sizeof(pdc_validate_config_ind_msg_v01)); 
  if (!validate_config_ind)
  {
    MCFG_MSG_ERROR("failed to validate_config_ind");
    mcfg_free(request);
    return;
  }

  type = request->config_type;

  if (request->subscription_id_valid == FALSE)
  {
    sub = MCFG_SUB_ID_FIRST;
  }
  else
  {
    sub = request->subscription_id;
  }

  /* init indication message */
  memset(validate_config_ind, 0, sizeof(pdc_validate_config_ind_msg_v01));
  validate_config_ind->ind_token_valid = request->ind_token_valid;  
  validate_config_ind->ind_token = request->ind_token;

  validate_config_ind->result_format_valid = TRUE;
  validate_config_ind->frame_index_valid   = TRUE;
  validate_config_ind->result_frame_valid  = TRUE;
  // not needed since memset but listed here for clarity
  //validate_config_ind->result_format = 0;
  //validate_config_ind->frame_index   = 0;
  //validate_config_ind->result_frame  = {0};

  err = QMI_ERR_NONE_V01;

  /* generate indication message */
  if (request->config_id_valid == TRUE)
  {
    config_id.id_len = request->config_id_len;
    memscpy(config_id.id, PDC_CONFIG_ID_SIZE_MAX_V01, 
           request->config_id, config_id.id_len);

    MCFG_MSG_LOW_4("config_id sub%d %02x%02x%02x", 
            sub, config_id.id[0], config_id.id[1], config_id.id[2]);

    /* as long as the config is on the target the config id is valid */
    if (mcfg_utils_is_config_id_unique(type, &config_id))
    {
     MCFG_MSG_LOW("Config ID not present on target");
     err = QMI_ERR_INVALID_ID_V01;
    }
    else 
    {
      // Config exists on the device
    }
  }
  else
  {
    if (!mcfg_utils_get_active_config(type, &config_id, sub))
    {
      MCFG_MSG_ERROR_2("No config id found for cfg_type %d on subscription %d", type, sub);
      err = QMI_ERR_INVALID_ID_V01;
    }
  }

  do
  {
    if (err == QMI_ERR_NONE_V01)
    {
      mcfg_err = mcfg_validate_config(type, &config_id, sub);
      err = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;
    }

    /* If qmi error at this point then either no config id was passed and there 
       is no active config or a config id was passed but not present on the target */
    if (err != QMI_ERR_NONE_V01)
    {
      MCFG_MSG_LOW("Error validating config");
      validate_config_ind->frame_index_valid = FALSE;
      validate_config_ind->result_frame_valid = FALSE;

      validate_config_ind->error = err;

      mcfg_svc_cmd_ind(post_cmd,
                       (rsp_cnt & 0x7fff),
                       QMI_IDL_INDICATION,
                       QMI_PDC_VALIDATE_CONFIG_IND_V01,
                       validate_config_ind,
                       sizeof(pdc_validate_config_ind_msg_v01),
                       sizeof(pdc_validate_config_ind_msg_v01),
                       err);

      mcfg_free(validate_config_ind);
      mcfg_free(request);

      return;
    }

    (void)snprintf(f_name, sizeof(f_name), "/mcfg_test.xml");

    /* Check if the file is present or not. */
    if(efs_stat(f_name,&temp_buf) == -1) 
    {
      MCFG_MSG_ERROR("MCFG: /mcfg_test.xml not found");
      err = QMI_ERR_INTERNAL_V01;
      break;
    } 
    else if(temp_buf.st_size <= 1)
    {
       MCFG_MSG_ERROR("XML file is 0B");
       err = QMI_ERR_INTERNAL_V01;
       break;
    }
    else 
    {
       fd = efs_open(f_name, O_RDWR, 0777 );
       if (fd < 0)
       {
          MCFG_MSG_ERROR("error opening file");
          err = QMI_ERR_INTERNAL_V01;
          break;
       }
    }

    /* XML file successfully opened if this point is reached*/
#ifdef FEATURE_MCFG_DIAG_SUPPORT
    max_diagQ_size = ((uint32) diagpkt_max_delayed_rsp_size()) * DIAG_PACKETS_IN_TRANSIT_QUEUE;
#endif
    nbytes = temp_buf.st_size;
    read_success = TRUE;
    bytes_read = 0;
    while ((bytes_read < nbytes) && (TRUE == read_success))
    {
      bytes_left = nbytes - bytes_read;
      if (bytes_left <= PDC_CONFIG_DESC_SIZE_MAX_V01)
      {
         read_bytes = bytes_left;
         validate_config_ind->frame_index = 0xFFFFFFFF;
         rsp_cnt = rsp_cnt & 0x7fff;
      }
      else 
      {
         read_bytes = PDC_CONFIG_DESC_SIZE_MAX_V01;
      }
      result = efs_read (fd,
                         (void *) validate_config_ind->result_frame,
                         read_bytes);

      MCFG_MSG_LOW_5("fsize: %d, bRead: %d, bytes_left: %d, readB: %d, result: %d", 
                      nbytes, bytes_read, bytes_left, read_bytes, result);

      if (result <= 0)
      {
         MCFG_MSG_ERROR_1("read failure from file after %lu bytes read", bytes_read); 
         err = QMI_ERR_INTERNAL_V01;
         validate_config_ind->frame_index_valid = FALSE;
         validate_config_ind->result_frame_valid = FALSE;
         read_success = FALSE;
         break;
      }

      validate_config_ind->result_frame_len = result;
      validate_config_ind->error = err;

#ifdef FEATURE_MCFG_DIAG_SUPPORT
      curr_diagQ_size = (uint32) diagpkt_total_delayed_bytes_queued();

      pet_dog = 0;
      while (curr_diagQ_size >= max_diagQ_size)
      {
        MCFG_MSG_LOW_2("curr_diagQ_size: %d, max_diagQ_size: %d", curr_diagQ_size, max_diagQ_size); 
        timer_sleep(DIAG_WAIT_INTERVAL, T_MSEC, FALSE);
        curr_diagQ_size = (uint32) diagpkt_total_delayed_bytes_queued();
        pet_dog++;

        if(pet_dog >= 100)
        {
          mcfg_task_dog_hb_report();
          pet_dog = 0;
        }
      }
#endif
      /* send indication message */
      mcfg_svc_cmd_ind(post_cmd,
                       rsp_cnt,
                       QMI_IDL_INDICATION,
                       QMI_PDC_VALIDATE_CONFIG_IND_V01,
                       validate_config_ind,
                       sizeof(pdc_validate_config_ind_msg_v01),
                       sizeof(pdc_validate_config_ind_msg_v01),
                       err);

      /* This will overflow during last frame, but shouldn't matter
         since loop will not be entered again */
      validate_config_ind->frame_index++;
      bytes_read += (fs_size_t) result;
      rsp_cnt += 1;
    }
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    if (validate_config_ind != NULL)
    {
      validate_config_ind->error = err;

      mcfg_svc_cmd_ind(post_cmd,
                       rsp_cnt,
                       QMI_IDL_INDICATION,
                       QMI_PDC_VALIDATE_CONFIG_IND_V01,
                       validate_config_ind,
                       sizeof(pdc_validate_config_ind_msg_v01),
                       sizeof(pdc_validate_config_ind_msg_v01),
                       err);
    }
  }

  if (fd >= 0) 
  {
     efs_close(fd);
     efs_unlink(f_name);
  }

  mcfg_free(validate_config_ind);
  mcfg_free(request);
}/* END mcfg_svc_cmd_validate_config */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_get_feature

  DESCRIPTION
    Processes a mcfg svc command to get feature in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_get_feature
(
  mcfg_svc_post_cmd_type post_cmd,
  pdc_get_feature_req_msg_v01 *request
)
{
  pdc_get_feature_ind_msg_v01 feature_ind;
  uint16 rsp_cnt = 1;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  mcfg_error_e_type mcfg_err = MCFG_ERR_NONE;
  /*-----------------------------------------------------------------------*/
  
  /* init indication message */
  memset(&feature_ind, 0, sizeof(pdc_get_feature_ind_msg_v01));

  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    feature_ind.ind_token_valid = request->ind_token_valid;  
    feature_ind.ind_token = request->ind_token;

    /* generate indication message */
#ifdef FEATURE_MCFG_FEATURETAG
    mcfg_err = mcfg_get_feature(request->slot_id, MCFG_SETTING_CURR, &feature_ind);
#else
    mcfg_err = mcfg_get_feature(request->slot_id, &feature_ind);
#endif
    err = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;

    feature_ind.error = err;

    /* send indication message */
    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_GET_FEATURE_IND_V01,
                     (void *)&feature_ind,
                     sizeof(pdc_get_feature_ind_msg_v01),
                     sizeof(pdc_get_feature_ind_msg_v01),
                     err);
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    feature_ind.error = err;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_GET_FEATURE_IND_V01,
                     (void *)&feature_ind,
                     sizeof(pdc_get_feature_ind_msg_v01),
                     sizeof(pdc_get_feature_ind_msg_v01),
                     err);
  }

  mcfg_free(request);
}/* END mcfg_svc_cmd_get_feature */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_set_feature

  DESCRIPTION
    Processes a mcfg svc command to set feature in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_set_feature
(
   mcfg_svc_post_cmd_type post_cmd,
   pdc_set_feature_req_msg_v01 *request
)
{
  pdc_set_feature_ind_msg_v01 feature_ind;
  uint16 rsp_cnt = 1;
  mcfg_slot_index_type_e_type sloti;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  mcfg_error_e_type mcfg_err = MCFG_ERR_NONE;
  /*-----------------------------------------------------------------------*/
  
  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    /* init indication message */
    memset(&feature_ind, 0, sizeof(pdc_set_feature_ind_msg_v01));

    feature_ind.ind_token_valid = request->ind_token_valid;  
    feature_ind.ind_token = request->ind_token;

    if (request->slot_id != 0xFFFFFFFF)
    {
      /* generate indication message */
      mcfg_err = mcfg_set_feature(request);
      err = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;
    }
    else
    {
      for (sloti = 0; sloti < MCFG_NUM_OF_UIM_SLOT; sloti++)
      {
        /* generate indication message */
        request->slot_id = sloti;
        mcfg_err = mcfg_set_feature(request);
        err = (mcfg_err < MCFG_ERR_NON_QMI) ? (qmi_error_type_v01)mcfg_err : QMI_ERR_INTERNAL_V01;
        if (err != QMI_ERR_NONE_V01 &&
            err != QMI_ERR_NO_EFFECT_V01)
        {
          break;
        }
      }
    }

    feature_ind.error = err;

    /* send indication message */
    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_SET_FEATURE_IND_V01,
                     (void *)&feature_ind,
                     sizeof(pdc_set_feature_ind_msg_v01),
                     sizeof(pdc_set_feature_ind_msg_v01),
                     err);

  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    feature_ind.error = err;

    mcfg_svc_cmd_ind(post_cmd,
                     rsp_cnt,
                     QMI_IDL_INDICATION,
                     QMI_PDC_SET_FEATURE_IND_V01,
                     (void *)&feature_ind,
                     sizeof(pdc_set_feature_ind_msg_v01),
                     sizeof(pdc_set_feature_ind_msg_v01),
                     err);
  }

  mcfg_free(request);
}/* END mcfg_svc_cmd_set_feature */


/*===========================================================================

  FUNCTION mcfg_svc_cmd_get_config

  DESCRIPTION
    Processes a mcfg svc command to get config in the context of the MCFG task.

  DEPENDENCIES
    None

  PARAMETERS
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/
void mcfg_svc_cmd_get_config
(
   mcfg_svc_post_cmd_type post_cmd,
   pdc_get_config_req_msg_v01 *request
)
{
  pdc_get_config_ind_msg_v01 *get_config_ind = NULL;
  uint16 rsp_cnt = 1;
  mcfg_config_id_s_type config_id;
  mcfg_config_s_type config;
  mcfg_sub_id_type_e_type sub;
  int config_size = 0;
  uint32_t max_frame_len = PDC_CONFIG_FRAME_SIZE_MAX_V01/8;
  uint16_t ind_count = 0;
  uint16_t num_of_sent = 0;
  uint8_t i, j;
  qmi_error_type_v01 err = QMI_ERR_NONE_V01;
  /*-----------------------------------------------------------------------*/
  
  do
  {
    if (!request)
    {
      err = QMI_ERR_MALFORMED_MSG_V01;
      break;
    }

    get_config_ind = mcfg_malloc (sizeof(pdc_get_config_ind_msg_v01));
    if (!get_config_ind)
    {
      err = QMI_ERR_NO_MEMORY_V01;
      break;
    }

    /* init indication message */
    memset(get_config_ind, 0, sizeof(pdc_get_config_ind_msg_v01));
    memset(&config_id, 0, sizeof(mcfg_config_id_s_type));
    memset(&config, 0, sizeof(mcfg_config_s_type));
    
    sub = (request->subscription_id_valid == TRUE) ? request->subscription_id : MCFG_SUB_ID_FIRST;

    /* generate indication message */
    if(IS_VALID_MCFG_SUBSCRIPTION_ID(sub))
    {
      if(request->config_id_valid == TRUE)
      {
        config_id.id_len = request->config_id_len;
        memscpy(config_id.id, MCFG_CONFIG_ID_SIZE_MAX, request->config_id, request->config_id_len);
      }
      else
      {
        if (mcfg_utils_get_active_config(request->config_type, &config_id, sub))
        {
          MCFG_MSG_LOW_1("active config id len %d", config_id.id_len);
          if (config_id.id_len > 0 && config_id.id_len <= PDC_CONFIG_ID_SIZE_MAX_V01)
          {
            MCFG_MSG_LOW_4("sub %d config id %02x%02x%02x", sub, config_id.id[0], config_id.id[1], config_id.id[2]);
          }
          else
          {
            err = QMI_ERR_INVALID_ID_V01;
            break;
          }
        }
        else
        {
          err = QMI_ERR_NOT_PROVISIONED_V01;
          break;
        }
      }

      if(!mcfg_utils_get_config_size(request->config_type, &config_id, &config_size))
      {
        err = QMI_ERR_INTERNAL_V01;
        break;
      }
      config.config_len = config_size;

      if(!mcfg_utils_alloc_config_buffer(request->config_type, &config))
      {
        err = QMI_ERR_NO_MEMORY_V01;
        break;
      }

      if(!mcfg_utils_get_config(request->config_type, &config_id, &config))
      {
        err = QMI_ERR_INTERNAL_V01;
        break;
      }

      ind_count = config.config_len / max_frame_len;
      ind_count += ((config.config_len % max_frame_len) != 0) ? 1 : 0;
      MCFG_MSG_LOW_1("ind_count 0x%02x", ind_count);

      if(request->frame_index >= ind_count)
      {
        err = QMI_ERR_INVALID_INDEX_V01;
        break;
      }

      get_config_ind->ind_token_valid = request->ind_token_valid;
      get_config_ind->ind_token = request->ind_token;

      get_config_ind->frame_index_valid = request->frame_index_valid;
      get_config_ind->frame_index = request->frame_index;

      num_of_sent = max_frame_len * (get_config_frame_index - 1);

      get_config_ind->config_frame_len = ((config.config_len - num_of_sent) > max_frame_len) ? max_frame_len : (config.config_len - num_of_sent);
      memscpy(get_config_ind->config_frame, max_frame_len, 
                ((uint8_t *)config.config_addr + num_of_sent), get_config_ind->config_frame_len);
      MCFG_MSG_LOW_1("config_frame_len %d", get_config_ind->config_frame_len);

      num_of_sent += get_config_ind->config_frame_len;

      get_config_ind->config_frame_valid = TRUE;
      if(get_config_ind->frame_index == (ind_count - 1))  /* last frame */
      {
        get_config_ind->frame_index = 0xFFFFFFFF;
        get_config_frame_index = 0;
        get_config_ind_token = 0;
      }
      MCFG_MSG_LOW_1("cached ind token 0x%02x", get_config_ind_token);
      MCFG_MSG_LOW_1("cached frame index 0x%02x", get_config_frame_index);

      get_config_ind->error = err;

      /* send indication message */
      mcfg_svc_cmd_ind(post_cmd,
                       rsp_cnt,
                       QMI_IDL_INDICATION,
                       QMI_PDC_GET_CONFIG_IND_V01,
                       get_config_ind,
                       sizeof(pdc_get_config_ind_msg_v01),
                       sizeof(pdc_get_config_ind_msg_v01),
                       err);
    }
  }while(0);

  if(err != QMI_ERR_NONE_V01)
  {
    MCFG_MSG_ERROR_1("Error %d", err);
    if (get_config_ind != NULL)
    {
      get_config_ind->error = err;

      mcfg_svc_cmd_ind(post_cmd,
                       rsp_cnt,
                       QMI_IDL_INDICATION,
                       QMI_PDC_GET_CONFIG_IND_V01,
                       get_config_ind,
                         sizeof(pdc_get_config_ind_msg_v01),
                         sizeof(pdc_get_config_ind_msg_v01),
                         err);
    }
  }

  mcfg_free(get_config_ind);
  mcfg_free(request);
  mcfg_utils_free_config_buffer(&config);
}/* END mcfg_svc_cmd_get_config */

/*===========================================================================

  FUNCTION mcfg_svc_default_list_config_cache

  DESCRIPTION
    Resets the given list config cache

  DEPENDENCIES
    None

  PARAMETERS
    *cache   [in]     the list config cache

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/

void mcfg_svc_default_list_config_cache
(
  pdc_list_config_cache_type *cache
)
{
  MCFG_CHECK_NULL_PTR_RET(cache);
  if(cache->config_list)
  {
     mcfg_free(cache->config_list);
  }
  memset(cache, 0, sizeof(pdc_list_config_cache_type));
}

/*===========================================================================

  FUNCTION mcfg_svc_get_list_config_cache

  DESCRIPTION
    If there is a cache already assigned to the given *client, then that cache will be
    identified and returned. Otherwise the unassigned cache will be returned.

  DEPENDENCIES
    None

  PARAMETERS
    *svc      [in]      the service global where all those QMI/DIAG client and service details are stored
    *client   [in]      for which client we need to fetch the list config cache
    **cache   [out]     the fetched list config cache address

  RETURN VALUE
    None

  SIDE EFFECTS
    None

===========================================================================*/

void mcfg_svc_get_list_config_cache
(
  pdc_service_type *svc,
  mcfg_svc_client_type *client,
  pdc_list_config_cache_type **cache
)
{
  int i;

  MCFG_CHECK_NULL_PTR_RET(svc);
  MCFG_CHECK_NULL_PTR_RET(client);
  
  *cache = NULL; /* initialie the cache with NULL for the validation purpose */
  
  for(i=0; i<(PDC_CLIENTS_MAX+1); i++)
  {
    if(svc->pdc_clients[i].qmi_client_handle == client->qmi_client_handle)
    {
      *cache = &svc->list_config_cache[i]; /* list config cache index is same as the pdc_clients index */
	  break;
    }
  }
}

