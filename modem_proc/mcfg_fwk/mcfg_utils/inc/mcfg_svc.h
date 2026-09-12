
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

$Header: //components/rel/mcfg_fwk.mpss/7.8.1.1/mcfg_utils/inc/mcfg_svc.h#2 $ 
$DateTime: 2021/08/05 08:47:13 $ 
$Author: pwbldsvc $ 
$Change: 32223212 $ 

when     who  what, where, why
-------- ---  ----------------------------------------------------------------
01/09/18      Initial creation

==============================================================================*/

#include "mcfg_task.h"
#include "persistent_device_configuration_v01.h"
#include "qmi_idl_lib.h"
#include "common_v01.h"
#include "mcfg_client.h"
#include "qmi_csi.h"
#include "mcfg_common.h"


/*==============================================================================
  DEFINITIONS AND DECLARATIONS FOR MODULE
==============================================================================*/

/*==============================================================================
  Constants and Macros
==============================================================================*/
/* Macro to return result based on a given qmi_error_type_v01 */
#define QMI_RESULT_FROM_ERROR(error) (error == QMI_ERR_NONE_V01 ? QMI_RESULT_SUCCESS_V01 \
                                                                : QMI_RESULT_FAILURE_V01)

/*
 * MCFG Service types
 */
#define QMI_PDC       1 /* request generated from QMI */
#define DIAG_CMD      2 /* request generated from DIAG */

/*
 * max number of clients per MCFG Service
 */
#define PDC_CLIENTS_MAX     8

/*
 * fixed index for Diag Client
 */
#define DIAG_CLIENT_INDEX   PDC_CLIENTS_MAX

/*
 * MCFG Service client handle
 */
extern qmi_client_handle DIAG_CLIENT_HANDLE;


/*==============================================================================
  Typedefs
==============================================================================*/

typedef enum mcfg_svc_cmd_e {
  MCFG_SVC_CMD_MIN = 0,

  MCFG_SVC_CMD_INDICATION_REGISTER = QMI_PDC_INDICATION_REGISTER_REQ_V01,
  MCFG_SVC_CMD_GET_SELECTED_CONFIG = QMI_PDC_GET_SELECTED_CONFIG_REQ_V01,
  MCFG_SVC_CMD_SET_SELECTED_CONFIG = QMI_PDC_SET_SELECTED_CONFIG_REQ_V01,
  MCFG_SVC_CMD_LIST_CONFIG = QMI_PDC_LIST_CONFIGS_REQ_V01,
  MCFG_SVC_CMD_DELETE_CONFIG = QMI_PDC_DELETE_CONFIG_REQ_V01,
  MCFG_SVC_CMD_LOAD_CONFIG = QMI_PDC_LOAD_CONFIG_REQ_V01,
  MCFG_SVC_CMD_ACTIVATE_CONFIG = QMI_PDC_ACTIVATE_CONFIG_REQ_V01,
  MCFG_SVC_CMD_GET_CONFIG_INFO = QMI_PDC_GET_CONFIG_INFO_REQ_V01,
  MCFG_SVC_CMD_VALIDATE_CONFIG = QMI_PDC_VALIDATE_CONFIG_REQ_V01,
  MCFG_SVC_CMD_GET_FEATURE = QMI_PDC_GET_FEATURE_REQ_V01,
  MCFG_SVC_CMD_SET_FEATURE = QMI_PDC_SET_FEATURE_REQ_V01,
  MCFG_SVC_CMD_GET_CONFIG = QMI_PDC_GET_CONFIG_REQ_V01,

  MCFG_SVC_CMD_MAX
} mcfg_svc_cmd_e_type;

/*
 *  Packet
 *
 * used to store cache for a diag command
 */
typedef struct diag_client {
  uint16                cmd_id;
  uint16                delayed_rsp_id;
  uint16                rsp_cnt;
} diag_client_type;

/*
 *  Packet
 *
 * used while posting the diag command request to mcfg_svc 
 */
typedef struct diag_post_cmd {
  uint16 delayed_rsp_id;
  void *delayed_rsp;
} diag_post_cmd_type;

/* PDC client info - event registration information */
typedef struct 
{
  boolean            selected_config;
  boolean            refresh_event;
} pdc_event_reg_type;

/* PDC client info */
typedef struct 
{
  qmi_client_handle  qmi_client_handle;
  boolean            connected;
  pdc_event_reg_type event_reg;
} pdc_client_type;

/*
 *  Packet
 *
 * used while posting the diag command request to mcfg_svc 
 */
typedef struct pdc_post_cmd {
  unsigned int msg_id;
  qmi_req_handle req_handle;
  pdc_client_type *client;
} pdc_post_cmd_type;

/* PDC responses */
typedef union
{
  qmi_response_type_v01                resp;
  pdc_reset_resp_msg_v01               reset;
  pdc_indication_register_resp_msg_v01 indication_register;
  pdc_get_selected_config_resp_msg_v01 get_selected_config;
  pdc_set_selected_config_resp_msg_v01 set_selected_config;
  pdc_list_configs_resp_msg_v01        list_configs;
  pdc_delete_config_resp_msg_v01       delete_config;
  pdc_load_config_resp_msg_v01         load_config;
  pdc_activate_config_resp_msg_v01     activate_config;
  pdc_get_config_info_resp_msg_v01     get_config_info;
  pdc_get_config_limits_resp_msg_v01   get_config_limits;
} pdc_general_resp_v01_type;

/* PDC cache of last-sent indications - used to remove duplicate IND's from
   being sent */
typedef struct
{
  boolean                              config_change_valid;
  pdc_config_change_ind_msg_v01        config_change;
  uint32                               config_change_len;
} pdc_last_ind_type;

/* PDC load config cached information */
typedef struct 
{
  qmi_client_handle     qmi_client_handle;

  mcfg_storage_mode_e_type storage;
  mcfg_config_type_e_type type;
  mcfg_config_id_s_type id_s;

  uint32                offset;
  mcfg_config_s_type    config_s;
} pdc_load_config_cache_type;

/* PDC activate cached information */
typedef struct 
{
  qmi_client_handle     qmi_client_handle;
  uint8_t               ind_token_valid;
  uint32_t              ind_token;
  mcfg_error_e_type     error;
  boolean               done;
} pdc_activate_cache_type;

/* PDC list config cached information */
typedef struct 
{
  qmi_client_handle     qmi_client_handle;

  mcfg_config_id_s_type *config_list;

  uint32                ind_token;
  uint16                total;
  uint16                num_of_sent;
} pdc_list_config_cache_type;

/* PDC service info */
typedef struct 
{
  /* Individual clients */
  pdc_client_type pdc_clients[PDC_CLIENTS_MAX + 1];   /* +1 is for a Diag Client */

  /* Service handler from QCSI */
  qmi_csi_service_handle pdc_service_handler;
  
  /* Allocate response an indication message bodies outside of stack space */
  pdc_general_resp_v01_type     pdc_resp;
  pdc_last_ind_type             pdc_ind;

  pdc_load_config_cache_type    load_config_cache;

  pdc_activate_cache_type       activate_cache[MCFG_NUM_OF_UIM_SLOT+1];

  pdc_list_config_cache_type    list_config_cache[PDC_CLIENTS_MAX + 1];

  diag_client_type              *diag_cache; /* pointer to diag cache if diag support is available */
} pdc_service_type;

/*
 * Post command Packet
 *
 * used by a client to queue queries under mcfg context
 */
typedef struct mcfg_svc_post_cmd {
  uint8 type; /* type = QMI_PDC  or  type = DIAG_CMD */
  union
  {
    diag_post_cmd_type diag_post;
    pdc_post_cmd_type pdc_post;
  }data;
} mcfg_svc_post_cmd_type;

/*
 *  Packet
 *
 * used to queue / dequeue svc command requests
 */
typedef struct mcfg_svc_cmd_data_s {
  mcfg_svc_cmd_e_type cmd_id;
  mcfg_svc_post_cmd_type post_cmd;
  void *request;
} mcfg_svc_cmd_data_s_type;

/*
 * Packet
 *
 * 
 */
typedef struct mcfg_svc_client {
  qmi_client_handle     qmi_client_handle;
} mcfg_svc_client_type;

/*
 * Table entry for mcfg services
 *
 * used by client to get mcfg service information
 */
typedef struct mcfg_svc_cmd_entry {
  mcfg_svc_cmd_e_type cmd_code;
  int size_of_req_type;
  int size_of_resp_type;
  int32 (*handler_ptr) (mcfg_svc_post_cmd_type post_cmd, void *request, uint32_t req_len, void *response, uint32_t rsp_len);
}
mcfg_svc_cmd_entry_type;


/*==============================================================================
  Variables and Structures
==============================================================================*/
/* global cache for all commands and clients */
static pdc_service_type global_mcfg_svc;

/* cache for get_config command */
static uint16 get_config_ind_token = 0;
static uint16 get_config_frame_index = 0;


/*==============================================================================

                    PUBLIC FUNCTION DECLARATIONS FOR MODULE

==============================================================================*/

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
);

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
   mcfg_svc_cmd_e_type svc_code
);


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
);


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
);

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
);
#else
qmi_error_type_v01 mcfg_svc_cmd_generate_post_cmd
(
  mcfg_svc_post_cmd_type *post_cmd,
  qmi_client_handle  client_handle
);
#endif

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
);


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
);


/*===========================================================================

  FUNCTION mcfg_svc_cmd_proc

  DESCRIPTION
    Processes a mcfg service command in the context of the MCFG task.

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
);


/*===========================================================================

  FUNCTION mcfg_svc_cmd_post

  DESCRIPTION
    This function sends QMI / DIAG commands for processing under MCFG context. 

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
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


/*===========================================================================

  FUNCTION mcfg_svc_get_feature_handler

  DESCRIPTION
    Handles a mcfg svc command packet to get feature in the context of the MCFG task.

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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


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
  void *request,
  uint32_t req_len,
  void *rsp_ptr,
  uint32_t rsp_len
);


/*===========================================================================

  FUNCTION mcfg_svc_cmd_indication_register

  DESCRIPTION
    Processes a mcfg svc command to register for refresh event indication in the context of the MCFG task.

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
);


#ifndef REMOVE_QMI_PDC_REFRESH_IND_V01
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
);
#endif


/*===========================================================================

  FUNCTION mcfg_svc_cmd_refresh_event_ind

  DESCRIPTION
    Processes a mcfg svc command to send indications for refresh event in the context of the MCFG task.

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
);


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
);


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
);


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
);


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
);


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
);


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
);


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
);


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
);


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
);


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
);


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
);


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
);


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
);


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
);

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
);

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
);

