/*===========================================================================

                         D S _ Q M I _ DPM . C

DESCRIPTION

  The Data Services QMI Data Port Mapper service source file.

EXTERNALIZED FUNCTIONS

  qmi_dpm_init()
    Initialize the QMI DPM service

Copyright (c) 2013-2017 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
===========================================================================
===========================================================================


===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "amssassert.h"
#include "qmi_modem_task_svc.h"
#include "qmi_modem_task_cmd.h"

#include "ds_qmi_dpm.h"
#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"

#include "qmi_idl_lib.h"
#include "qmi_idl_lib_internal.h"
#include "qmi_csi.h"
#include "qmi_csi_target_ext.h"
#include "mqcsi_log.h"
#include "common_v01.h"

#include "data_port_mapper_v01.h"
#include "data_common_v01.h"
#include "qmi_sap.h"
#include "queue.h"
#include "qmi_framework.h"
#include "ds_qmux.h"
#include "ds_qmi_svc.h"
#include "ps_clat.h"
#include <stringl/stringl.h>
#include "smd_sio_api.h"
#include "ds_rmnet_xport.h"

#include "ttl_map.h"

/*---------------------------------------------------------------------------
  Service configuration
  - maximum number of clients per DPM instance = ???
  - DPM service version is 1.0
---------------------------------------------------------------------------*/
#define DPMI_BASE_VER_MAJOR  (1)
#define DPMI_BASE_VER_MINOR  (0)

#define DPMI_ADDENDUM_VER_MAJOR  (0)
#define DPMI_ADDENDUM_VER_MINOR  (0)


#define DPMI_MAX_CLIENTS      5
#define DPMI_QCSI_OS_SIGNAL ((rex_sigs_type)(1 << QMI_QCSI_SERVICE_DPM_SIGNAL))

#define DS_QMI_DPM_ASSERT(expression) \
         ds_qmi_dpm_assert_wrapper(__LINE__, expression)

/*===========================================================================

                                DATA TYPES

===========================================================================*/

/*---------------------------------------------------------------------------
  QMI DPM Client state info
---------------------------------------------------------------------------*/
typedef struct {
  q_link_type       link; /*< Data Structure required by the queue 
                              library to add this as part of a queue */
  qmi_client_handle clnt;

}qmi_dpmi_client_info_type;



/*---------------------------------------------------------------------------
  QMI DPM Service state info
---------------------------------------------------------------------------*/

typedef struct {
  q_type                 dpmi_client_q;  /*<Information about all DPM currently registered client
                                            of type qmi_dpmii_client_info_type*/
  qmi_csi_service_handle service_handle;
#ifdef QMI_DPM_DEBUG
  qmi_sap_client_handle  sap_client_handle;
#endif
  uint16                 num_of_clients;
}qmi_dpmi_service_info_type;

/*---------------------------------------------------------------------------
  DPM commands
---------------------------------------------------------------------------*/
typedef enum
{
  QMI_DPMI_CMD_MIN_VAL = -1,
  QMI_DPMI_CMD_MAX_VAL = 0x7FFFFFFF
} qmi_dpmi_cmd_type;


static qmi_dpmi_service_info_type    qmi_dpm_svc;
static qmi_csi_os_params             os_params;
static boolean                       qmi_dpmi_inited = FALSE;


static qmi_csi_cb_error  qmi_dpmi_open_port(
  qmi_dpmi_client_info_type        *clnt_info,
  qmi_req_handle                   req_handle,
  int                              msg_id,
  void                             *req_c_struct,
  int                              req_c_struct_len,
  void                             *service_handle
);


static qmi_csi_cb_error  qmi_dpmi_close_port(
  qmi_dpmi_client_info_type        *clnt_info,
  qmi_req_handle                   req_handle,
  int                              msg_id,
  void                             *req_c_struct,
  int                              req_c_struct_len,
  void                             *service_handle
);

static qmi_csi_cb_error  qmi_dpmi_get_supported_capabilities(
  qmi_dpmi_client_info_type        *clnt_info,
  qmi_req_handle                   req_handle,
  int                              msg_id,
  void                             *req_c_struct,
  int                              req_c_struct_len,
  void                             *service_handle
);

static boolean qmi_dpmi_msg_hdlr_cb
(
  qmi_sig_enum_type  sig,
  void              *svc_ptr
);

static qmi_csi_cb_error qmi_dpmi_svc_connect_cb
(
  qmi_client_handle          client_handle,
  void                      *service_handle,
  void                     **connection_handle
);

static void qmi_dpmi_svc_disconnect_cb
(
  void                      *connection_handle,
  void                      *service_handle
);

static qmi_csi_cb_error qmi_dpmi_handle_client_req_cb
(
  void                     *connection_handle,
  qmi_req_handle           req_handle,
  unsigned int             msg_id,
  void                     *req_c_struct,
  unsigned int             req_c_struct_len,
  void                     *service_handle
);

static boolean qmi_dpmi_convert_ep_id_to_int 
( 
  data_ep_type_enum_v01 device_type,
  uint32 interface,
  uint32 *ep_id
);


/*
TODO: Check if we need commands. 
*/ 


/* Define service handle table for DPM messages */
static qmi_csi_cb_error (* const req_handle_table[])
(
 qmi_dpmi_client_info_type        *clnt_info,
 qmi_req_handle           req_handle,
 int                      msg_id,
 void                    *req_c_struct,
 int                      req_c_struct_len,
 void                     *service_handle
) = 
{
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, /* Request handler for message ID 0x00 - 0x07*/
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, /* Request handler for message ID 0x08 - 0x0F*/
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, /* Request handler for message ID 0x10 - 0x17*/
  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL, /* Request handler for message ID 0x18 - 0x1F*/
  qmi_dpmi_open_port,                      /* Request handler for message ID 0x20 */
  qmi_dpmi_close_port,                     /* Request handler for message ID 0x21 */
  qmi_dpmi_get_supported_capabilities      /* Request handler for message ID 0x22 */
};


extern rex_tcb_type qmi_modem_tcb;
extern boolean rmnet_port_open_ipa_init_done;

static int dpmi_compare_qmi_client_handle
( 
  void *item_ptr,
  void *compare_val 
)
{
  qmi_dpmi_client_info_type *clnt_info = (qmi_dpmi_client_info_type*)item_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return ( (clnt_info->clnt == 
                          (qmi_client_handle)compare_val) ? 1 : 0 );
}


static int dpmi_compare_connection_info_handle
( 
  void *p_ItemPtr,
  void *p_CompareVal 
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* item_ptr is an element of the Client Q i.e. Connection Handle, 
     and compare val is also a Connection Handle */
  return ( (p_ItemPtr == p_CompareVal) ? 1 : 0 );
}

/*===========================================================================

FUNCTION DS_QMI_DPM_ASSERT_WRAPPER()

DESCRIPTION
  Wrapper function for DS_QMI_DPM ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_qmi_dpm_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DS_QMI_DPM_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* ds_qmi_dpm_assert_wrapper */


/*===========================================================================

                       EXTERNAL FUNCTION DEFINTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_DPM_INIT()

  DESCRIPTION
    Register the DPM service with QCSI 

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_dpm_init
(
  void
)
{
  qmi_csi_error rc = 0;
  qmi_csi_options       options;
  LOG_MSG_INFO1_1("QMI DPM Service init State %d", qmi_dpmi_inited);
  if (!qmi_dpmi_inited)
  {
    /*-------------------------------------------------------------------------
      Setting QMI MODEM TASK signal handler for handling QMI messages
    -------------------------------------------------------------------------*/
    (void)qmi_set_sig_handler(QMI_QCSI_SERVICE_DPM_SIGNAL, 
                              qmi_dpmi_msg_hdlr_cb, &qmi_dpm_svc);

    qmi_enable_sig(QMI_QCSI_SERVICE_DPM_SIGNAL);

    /* Set the signal to init again */
    qmi_set_signal(QMI_QCSI_SERVICE_DPM_SIGNAL);
    return;
  }
  os_params.tcb = &qmi_modem_tcb;
  os_params.sig = DPMI_QCSI_OS_SIGNAL;
  QMI_CSI_OPTIONS_INIT(options);
  (void)mqcsi_register_log_options(&options);
  /*-------------------------------------------------------------------------
  Register QMI DPM service with QCSI
  -------------------------------------------------------------------------*/
  rc = qmi_csi_register_with_options(dpm_get_service_object_v01(),
                        qmi_dpmi_svc_connect_cb,
                        qmi_dpmi_svc_disconnect_cb, 
                        qmi_dpmi_handle_client_req_cb, 
                        &qmi_dpm_svc, &os_params, 
                        &options,
                        &qmi_dpm_svc.service_handle);   
  if(rc != QMI_CSI_NO_ERR)
  {
    ERR_FATAL("Unable to register DPM service! Error %d", rc, 0, 0);
    DS_QMI_DPM_ASSERT(0);
    return;
  }
  LOG_MSG_INFO1_1("qmi_csi_register success %d", rc);
#ifdef QMI_DPM_DEBUG
  /*-------------------------------------------------------------------------
  Register QMI DPM service with QSAP
  -------------------------------------------------------------------------*/
  src = qmi_sap_register(dpm_get_service_object_v01(), NULL, &qmi_dpm_svc.sap_client_handle);
  if (src != QMI_SAP_NO_ERR)
  {
    ERR_FATAL("Unable to register DPM service with QSAP! Error %d", src, 0, 0);
    DS_QMI_DPM_ASSERT(0);
    return;
  }
#endif
  /* Initialize the queue which will hold all handles of clients */
  (void) q_init(&qmi_dpm_svc.dpmi_client_q);
  qmi_dpm_svc.num_of_clients = 0;

  //TODO: Check if we need to process command.

}/*qmi_dpm_init */

/*===========================================================================
  FUNCTION QMI_DPMI_SVC_CONNECT_CB()

  DESCRIPTION
    CallBack function to be registered with QCSI for connecting a client

  PARAMETERS
    Client Handle
    Service Handle
    Connection Handle

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dpmi_svc_connect_cb
(
  qmi_client_handle          client_handle,
  void                      *service_handle,
  void                     **connection_handle
)
{
  qmi_dpmi_client_info_type    *clnt_info_ptr;
  qmi_dpmi_service_info_type   *dpm_svc_ptr;
  qmi_csi_cb_error             rc;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_QMI_DPM_ASSERT(connection_handle && service_handle);

  dpm_svc_ptr = (qmi_dpmi_service_info_type *) service_handle;
 
  if ( NULL != q_linear_search( &dpm_svc_ptr->dpmi_client_q,
                                dpmi_compare_qmi_client_handle,
                                client_handle ) )
  {
    LOG_MSG_ERROR_1("dpmi_svc_connect_cb: DPM client 0x%p present",
                    client_handle);
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }

  if (DPMI_MAX_CLIENTS == dpm_svc_ptr->num_of_clients)
  {
    LOG_MSG_ERROR_1("dpmi_svc_connect_cb: Maximum number of clients already allocated %d",
                    DPMI_MAX_CLIENTS);
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC(clnt_info_ptr, sizeof(qmi_dpmi_client_info_type),
                           qmi_dpmi_client_info_type*);
  if(!clnt_info_ptr)
  {
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }
  memset(clnt_info_ptr, 0, sizeof(qmi_dpmi_client_info_type));
  (void) q_link ( clnt_info_ptr, &clnt_info_ptr->link );
  clnt_info_ptr->clnt = client_handle;
  /* Add it to the queue of clients */
  q_put( &(dpm_svc_ptr->dpmi_client_q), &(clnt_info_ptr->link) );
  dpm_svc_ptr->num_of_clients++;

  *connection_handle = clnt_info_ptr;
  LOG_MSG_INFO1_1("dpmi_svc_connect_cb: Alloc client 0x%p", clnt_info_ptr);
  rc = QMI_CSI_CB_NO_ERR;
bail:
  return rc;
} /* qmi_dpmi_svc_connect_cb() */


/*===========================================================================
  FUNCTION QMI_DPMI_SVC_DISCONNECT_CB()

  DESCRIPTION
    CallBack function to be registered with QCSI for disconnecting a Client

  PARAMETERS
    Client Handle
    Service Handle
    Connection Handle

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_dpmi_svc_disconnect_cb
(
  void                      *connection_handle,
  void                      *service_handle
)
{
  qmi_dpmi_service_info_type    *dpm_svc_ptr;
  qmi_dpmi_client_info_type     *clnt_info_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DPM_ASSERT(connection_handle && service_handle);
  dpm_svc_ptr = (qmi_dpmi_service_info_type *)service_handle;

  LOG_MSG_INFO1_1("dpmi_svc_disconnect_cb: Releasing client 0x%p",
                  connection_handle);

  clnt_info_ptr = q_linear_search( &dpm_svc_ptr->dpmi_client_q,
                                   dpmi_compare_connection_info_handle,
                                   connection_handle );
  if (NULL == clnt_info_ptr)
  {
    LOG_MSG_ERROR_1("dpmi_svc_connect_cb: DPM client 0x%p not found",
                    connection_handle);
    return;

  }

  if ( TRUE != q_delete_ext( &dpm_svc_ptr->dpmi_client_q,
                             &clnt_info_ptr->link ) )
  {
    LOG_MSG_ERROR_1("dpmi_svc_connect_cb: DPM client 0x%p could not be deleted from queue",
                    clnt_info_ptr);
    return;
  }
  PS_SYSTEM_HEAP_MEM_FREE(clnt_info_ptr);
  dpm_svc_ptr->num_of_clients--;

} /* qmi_dpmi_svc_disconnect_cb() */

/*===========================================================================
  FUNCTION QMI_DPMI_HANDLE_CLIENT_REQ_CB()

  DESCRIPTION
    CallBack function to be registered with QCSI for handling client request

  PARAMETERS
    Client Handle
    Service Handle
    Connection Handle

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_dpmi_handle_client_req_cb
(
  void                     *connection_handle,
  qmi_req_handle           req_handle,
  unsigned int             msg_id,
  void                     *req_c_struct,
  unsigned int             req_c_struct_len,
  void                     *service_handle
)
{
  qmi_csi_cb_error              rc;
  qmi_dpmi_client_info_type     *clnt_info_ptr;
  qmi_dpmi_service_info_type    *dpm_svc_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_DPM_ASSERT(connection_handle && service_handle);

  rc = QMI_CSI_CB_NO_ERR;
  dpm_svc_ptr = (qmi_dpmi_service_info_type*) service_handle;

  clnt_info_ptr = q_linear_search( &dpm_svc_ptr->dpmi_client_q,
                                   dpmi_compare_connection_info_handle,
                                   connection_handle );
  if (NULL == clnt_info_ptr)
  {
    LOG_MSG_ERROR_1("dpmi_handle_req_cb: Invalid clnt 0x%p",
                    connection_handle);
    rc = QMI_CSI_CB_INTERNAL_ERR;
    return rc;
  }

  if(msg_id < (sizeof(req_handle_table) / sizeof(*req_handle_table)))
  {
    if(req_handle_table[msg_id])
    {
      rc = req_handle_table[msg_id] (clnt_info_ptr, req_handle, msg_id,
                                     req_c_struct, req_c_struct_len,
                                     service_handle);
    }
    else
    {
      LOG_MSG_ERROR_1("dpmi_handle_req_cb: NULL message ID handler: %d",
                      msg_id);
      rc = QMI_CSI_CB_UNSUPPORTED_ERR;
    }
  }
  else
  {
    LOG_MSG_ERROR_1("dpmi_handle_req_cb: Invalid message ID: %d", msg_id);
    rc = QMI_CSI_CB_UNSUPPORTED_ERR;
  }

  return rc;
} /* qmi_dpmi_handle_client_req_cb() */

/*===========================================================================
  FUNCTION QMI_DPMI_MSG_HDLR_CB()

  DESCRIPTION
    CallBack function to be registered with QCSI for handling Messages related to DPM service.

  PARAMETERS
    Client Handle
    Service Handle
    Connection Handle

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_dpmi_msg_hdlr_cb
(
  qmi_sig_enum_type  sig,
  void               *svc_ptr
)
{
  qmi_dpmi_service_info_type *dpm_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_DPM_ASSERT((sig == QMI_QCSI_SERVICE_DPM_SIGNAL) && svc_ptr);

  if (!qmi_dpmi_inited)
  {
    qmi_dpmi_inited = TRUE;
    /* Call init again to register with QCSI now
     * Wait for all tasks to be inited */
    qmi_dpm_init();
    return TRUE;
  }

  dpm_sp = (qmi_dpmi_service_info_type *) svc_ptr;

  qmi_csi_handle_event(dpm_sp->service_handle, &os_params);
  return TRUE;
} /* qmi_dpmi_msg_hdlr_cb */


/*===========================================================================
  FUNCTION QMI_DPMI_OPEN_PORT()

  DESCRIPTION
    Process the request to open ports dynamically

  PARAMETERS
    clnt_info     :          Client info requesting the message.
    service_handle:          QMI_DPM's service instance state pointer for this qmi link
    req_handle    :          Request handle
    msg_id        :          Message Id  for the corresponding message.
    req_c_struct  :          Request structure with details for the message.
 
  RETURN VALUE
 
  DEPENDENCIES
    qmi_dpm_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_dpmi_open_port
(
  qmi_dpmi_client_info_type        *clnt_info,
  qmi_req_handle                   req_handle,
  int                              msg_id,
  void                             *req_c_struct,
  int                              req_c_struct_len,
  void                             *service_handle
)
{
  boolean err_code = TRUE;
  dpm_open_port_req_msg_v01        *req_msg;
  dpm_open_port_resp_msg_v01       resp_msg;
  uint32 iter = 0;
  uint32 ep_id = 0;
  uint32 aggr_ep_id = 0;
  int    aggr_iter = 0;
  rmnet_phys_xport_buffer_info_type rmnet_buffer_info;

  TTLBOOTUP(STARTUP_QMUX_DPM_OPEN);
  DS_QMI_DPM_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && service_handle);

  req_msg = (dpm_open_port_req_msg_v01 *)req_c_struct;
  LOG_MSG_INFO1_1 ("Client 0x%p requesting Open port", clnt_info);

  memset(&resp_msg, 0, sizeof(dpm_open_port_resp_msg_v01));
  memset(&rmnet_buffer_info, 0 , sizeof(rmnet_phys_xport_buffer_info_type));

  LOG_ULOG_MSG_INFO_1("qmi_dpmi_open_port: rmnet_port_open_ipa_init_done %d", rmnet_port_open_ipa_init_done);

  if (TRUE == rmnet_port_open_ipa_init_done)
  {
    if (req_msg->software_data_port_list_valid == TRUE)
    {
      for (iter = 0; iter < req_msg->software_data_port_list_len; iter++)
      {
        if (FALSE == qmi_dpmi_convert_ep_id_to_int(req_msg->software_data_port_list[iter].ep_id.ep_type,
                                                   req_msg->software_data_port_list[iter].ep_id.iface_id,
                                                   &ep_id))
        {
          LOG_MSG_ERROR_2("Unknown value of End point id received with ep-type %d,iface_id %d ",
                          req_msg->software_data_port_list[iter].ep_id.ep_type,
                          req_msg->software_data_port_list[iter].ep_id.iface_id);
          continue;
        }
        err_code = qmux_add_data_port(smd_sio_get_sio_id_from_name(req_msg->software_data_port_list[iter].port_name), 
                                      ep_id, 0, 0, &rmnet_buffer_info);
        if (err_code == FALSE)
        {
          LOG_MSG_ERROR_1("Could not add data port with ep_id %d", ep_id);
        }
      }
    }

    if (req_msg->hardware_data_port_list_valid == TRUE)
    {
      for (iter = 0; iter < req_msg->hardware_data_port_list_len; iter++)
      {
        if (FALSE == qmi_dpmi_convert_ep_id_to_int(req_msg->hardware_data_port_list[iter].ep_id.ep_type,
                                                   req_msg->hardware_data_port_list[iter].ep_id.iface_id,
                                                   &ep_id))
        {
          LOG_MSG_ERROR_2("Unknown value of End point id received with ep-type %d,iface_id %d ",
                          req_msg->hardware_data_port_list[iter].ep_id.ep_type,
                          req_msg->hardware_data_port_list[iter].ep_id.iface_id);
          continue;
        }
        if (req_msg->data_port_buffer_info_list_valid == TRUE) 
        {
          if (req_msg->data_port_buffer_info_list_len != req_msg->hardware_data_port_list_len)
          {
            LOG_MSG_ERROR_2("Wrong length for data port agg %d and hardware data port len %d", 
                            req_msg->data_port_buffer_info_list_len, req_msg->hardware_data_port_list_len);
            DS_QMI_DPM_ASSERT(0);
          }
          for (aggr_iter = 0; (uint32)aggr_iter < req_msg->data_port_buffer_info_list_len; aggr_iter++) 
          {
            if (FALSE == qmi_dpmi_convert_ep_id_to_int(req_msg->data_port_buffer_info_list[aggr_iter].ep_id.ep_type,
                                                       req_msg->data_port_buffer_info_list[aggr_iter].ep_id.iface_id,
                                                       &aggr_ep_id)) 
            {
              LOG_MSG_ERROR_2("Unknown value of End point id received with ep-type %d,iface_id %d ",
                              req_msg->data_port_buffer_info_list[aggr_iter].ep_id.ep_type,
                              req_msg->data_port_buffer_info_list[aggr_iter].ep_id.iface_id);
              continue;
            }
            if (ep_id == aggr_ep_id)
            {
              break;
            }
          }
        }
        else
        {
          aggr_iter = -1;
        }

        if (aggr_iter >= 0 && 
            (uint32)aggr_iter < req_msg->data_port_buffer_info_list_len)
        {
          rmnet_buffer_info.ul_fifo_size = 
            req_msg->data_port_buffer_info_list[aggr_iter].ul_fifo_size;
          rmnet_buffer_info.dl_fifo_size = 
            req_msg->data_port_buffer_info_list[aggr_iter].dl_fifo_size;
          rmnet_buffer_info.dl_buf_size = 
            req_msg->data_port_buffer_info_list[aggr_iter].dl_buf_size;     
        }

        err_code = qmux_add_data_port(SIO_PORT_NULL, 
                                      ep_id,
                                      req_msg->hardware_data_port_list[iter].hardware_ep_pair.consumer_pipe_num, 
                                      req_msg->hardware_data_port_list[iter].hardware_ep_pair.producer_pipe_num,
                                      &rmnet_buffer_info);
        if (err_code == FALSE)
        {
          LOG_MSG_ERROR_1("Could not add data port with ep_id %d", ep_id);
        }
      }
    }

    if (req_msg->control_port_list_valid == TRUE)
    {
      for (iter = 0; iter < req_msg->control_port_list_len; iter++)
      {
        if (FALSE == qmi_dpmi_convert_ep_id_to_int(req_msg->control_port_list[iter].default_ep_id.ep_type,
                                                   req_msg->control_port_list[iter].default_ep_id.iface_id,
                                                   &ep_id))
        {
          LOG_MSG_ERROR_2("Unknown value of End point id received with ep-type %d,iface_id %d ",
                          req_msg->control_port_list[iter].default_ep_id.ep_type,
                          req_msg->control_port_list[iter].default_ep_id.iface_id);
          continue;
        }
        err_code = qmux_add_control_port(smd_sio_get_sio_id_from_name(req_msg->control_port_list[iter].port_name),
                                         ep_id);
        if (err_code == FALSE)
        {
          LOG_MSG_ERROR_1("Could not add control port with ep_id %d", ep_id);
        }
      }
    }
  
    resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
    resp_msg.resp.error = QMI_ERR_NONE_V01;
  }
  else
  {
    /* Return failure if IPA is not initialized yet */
    LOG_ULOG_MSG_INFO_0("qmi_dpmi_open_port: Could not open data/control port, "
                       "as IPA is not initialized");

    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
  }

  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(dpm_open_port_resp_msg_v01));

  return QMI_CSI_CB_NO_ERR;

}/*qmi_dpmi_open_port*/


/*===========================================================================
  FUNCTION QMI_DPMI_CLOSE_PORT()

  DESCRIPTION
    Process the request to close ports dynamically

    PARAMETERS
    clnt_info     :          Client info requesting the message.
    service_handle:          QMI_DPM's service instance state pointer for this qmi link
    req_handle    :          Request handle
    msg_id        :          Message Id  for the corresponding message.
    req_c_struct  :          Request structure with details for the message.
 
  RETURN VALUE
 
  DEPENDENCIES
    qmi_dpm_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_dpmi_close_port
(
  qmi_dpmi_client_info_type        *clnt_info,
  qmi_req_handle                   req_handle,
  int                              msg_id,
  void                             *req_c_struct,
  int                              req_c_struct_len,
  void                             *service_handle
)
{
  boolean err_code = TRUE;
  dpm_close_port_req_msg_v01        *req_msg;
  dpm_close_port_resp_msg_v01       resp_msg;
  uint32 iter = 0;
  uint32 ep_id = 0;
  DS_QMI_DPM_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && service_handle);
  req_msg = (dpm_close_port_req_msg_v01 *)req_c_struct;
  LOG_MSG_INFO1_1 ("Client 0x%p requesting close port", clnt_info);

  memset(&resp_msg, 0, sizeof(dpm_close_port_resp_msg_v01));

  if (req_msg->control_port_list_valid == TRUE)
  {
    for (iter = 0; iter < req_msg->control_port_list_len; iter++)
    {
      err_code = qmux_remove_control_port(smd_sio_get_sio_id_from_name(req_msg->control_port_list[iter].port_name));
    }
  }
  if (req_msg->data_port_list_valid == TRUE)
  {
    for (iter = 0; iter < req_msg->data_port_list_len; iter++)
    {
      if (FALSE == qmi_dpmi_convert_ep_id_to_int(req_msg->data_port_list[iter].ep_type, req_msg->data_port_list[iter].iface_id, &ep_id))
      {
        LOG_MSG_ERROR_2("Unknown value of End point id received with ep-type %d,iface_id %d ",
                        req_msg->data_port_list[iter].ep_type,
                        req_msg->data_port_list[iter].iface_id);
        continue;
      }
      err_code = qmux_remove_data_port(ep_id);
       if (err_code == FALSE)
       {
         LOG_MSG_ERROR_1("Could not remove data port with ep_id %d", ep_id);
       }
    }
  }
 
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(dpm_close_port_resp_msg_v01));

  return QMI_CSI_CB_NO_ERR;
}

/*===========================================================================
  FUNCTION QMI_DPMI_GET_SUPPORTED_CAPABILITIES()

  DESCRIPTION
    Process the request to query supported capabilities

    PARAMETERS
    clnt_info     :          Client info requesting the message.
    service_handle:          QMI_DPM's service instance state pointer for this qmi link
    req_handle    :          Request handle
    msg_id        :          Message Id  for the corresponding message.
    req_c_struct  :          Request structure with details for the message.
 
  RETURN VALUE
 
  DEPENDENCIES
    qmi_dpm_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_dpmi_get_supported_capabilities
(
  qmi_dpmi_client_info_type        *clnt_info,
  qmi_req_handle                   req_handle,
  int                              msg_id,
  void                             *req_c_struct,
  int                              req_c_struct_len,
  void                             *service_handle
)
{
  dpm_get_supported_capabilities_resp_msg_v01       resp_msg;
  DS_QMI_DPM_ASSERT(clnt_info && service_handle);
  LOG_MSG_INFO1_1 ("Client 0x%p querying supported capabilities", clnt_info);

  memset(&resp_msg, 0, sizeof(dpm_get_supported_capabilities_resp_msg_v01));

  resp_msg.is_shim_supported_valid = TRUE;
  resp_msg.is_shim_supported = TRUE; /* Return TRUE unconditionally */

  resp_msg.is_rev_ip_sync_supported_valid = TRUE;
  resp_msg.is_rev_ip_sync_supported = TRUE; 
  
#ifdef FEATURE_DATA_PS_464XLAT 
  resp_msg.is_clat_supported_valid = TRUE;
  resp_msg.is_clat_supported = ps_clat_is_enabled();   
#endif /* FEATURE_DATA_PS_464XLAT */

  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(dpm_get_supported_capabilities_resp_msg_v01));

  return QMI_CSI_CB_NO_ERR;
} /* qmi_dpmi_get_supported_capabilities */


/*===========================================================================
  FUNCTION QMI_DPMI_CONVERT_EP_ID_TO_INT()

  DESCRIPTION
    Internal function to convert ep id to integer.

  PARAMETERS
   device_type : The enum identifying the device
   interface   : Interface number
   ep_id       : The return value is stored in this

  RETURN VALUE

  DEPENDENCIES

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_dpmi_convert_ep_id_to_int 
( 
  data_ep_type_enum_v01 device_type,
  uint32                interface,
  uint32                *ep_id
)
{

  if (device_type > 255) 
  {
    return FALSE;
  }
  if ((interface & 0xFF000000) > 0) 
  {
    return FALSE;
  }
  *ep_id = (((device_type & 0xff) << 24) | (interface & 0x00FFFFFF)); 
  return TRUE;
}
