/*===========================================================================

                         D S _ Q M I _ O T T . C

DESCRIPTION

  The Data Services QMI ***

OTT service provides tools to operators to differentiate subscription offers
and introduce new ways to monetize the increased data demand.

EXTERNALIZED FUNCTIONS

  qmi_ott_init()
    Initialize the QMI OTTservice

Copyright (c) 2013-2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/src/ds_qmi_ott.c#2 $
  $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
04/06/17    ss     Return appropriate error code when message is not suppoted
08/13/13    svj    Created module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "amssassert.h"
#include "dcc_task_defs.h"
#include "dcc_task_svc.h"

#include "ds_qmi_ott.h"
#include "over_the_top_v01.h"

#include "ds_Utils_DebugMsg.h"
#include "ps_system_heap.h"

#include "ps_sys.h"
#include "ps_sys_conf.h"
#include "ps_sys_event.h"
#include "ps_sys_ioctl.h"
#include "qmi_idl_lib.h"
#include "qmi_idl_lib_internal.h"
#include "qmi_csi.h"
#include "qmi_csi_target_ext.h"
#include "mqcsi_log.h"
#include "common_v01.h"
#include "qmi_sap.h"
#include "queue.h"
#include "qmi_framework.h"
#include "stringl/stringl.h"


/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Service management
---------------------------------------------------------------------------*/
#define OTT_MAX_CLIDS    (QMI_FRAMEWORK_SVC_MAX_CLIENTS - 1)
#define OTT_INVALID_INSTANCE    -1

/*---------------------------------------------------------------------------
  Major and Minor Version Nos for OTT_
---------------------------------------------------------------------------*/
#define OTT_BASE_VER_MAJOR    (1)
#define OTT_BASE_VER_MINOR    (1)

#define OTT_ADDENDUM_VER_MAJOR  (0)
#define OTT_ADDENDUM_VER_MINOR  (0)


/*---------------------------------------------------------------------------
  Macro used in command handlers (common)
---------------------------------------------------------------------------*/
#define CHECK_RETVAL()  if (FALSE == retval) { dsm_free_packet(&response); \
                                               return NULL; }

#define OTTI_QCSI_OS_SIGNAL ((rex_sigs_type)(1 << DCC_OTT_RDR_SIGNAL))

#define OTTI_MAX_TIMER_INTERVAL (0xFFFFFFFF)

#define OTTI_MAX_THRPUT_RAT_COUNT 2

#define OTTI_MASK_UL_THROUGHPUT_REPORT 0x01
#define OTTI_MASK_DL_THROUGHPUT_REPORT 0x02

#define DS_QMI_OTT_ASSERT(expression) \
         ds_qmi_ott_assert_wrapper(__LINE__, expression)

/*===========================================================================

                                DATA TYPES

===========================================================================*/


/*---------------------------------------------------------------------------
  QMI OTTClient state info
---------------------------------------------------------------------------*/
typedef struct {
  q_link_type                  link; /*< Data Structure required by the queue 
                                         library to add this as part of a 
                                         queue */
  qmi_client_handle clnt;

  uint32  subscription_id;
  uint32  uplink_report_interval;
  uint32  downlink_report_interval;
  boolean report_ul_queue_size;   
  struct
  {
    boolean  report_ul_thrpt_reporting_status_change;
    boolean  report_ul_thrpt_info;
    boolean  report_dl_thrpt_reporting_status_change;
    boolean  report_dl_thrpt_info;
  } report_status;

}qmi_otti_client_info_type;

/*---------------------------------------------------------------------------
  QMI OTT Subscription type
---------------------------------------------------------------------------*/
typedef enum 
{
  QMI_OTTI_SUBS_DEFAULT     = OTT_DEFAULT_SUBS_V01,     /* Default Data Subscription */
  QMI_OTTI_SUBS_PRIMARY     = OTT_PRIMARY_SUBS_V01,     /* Primary */
  QMI_OTTI_SUBS_SECONDARY   = OTT_SECONDARY_SUBS_V01,   /* Secondary */
  QMI_OTTI_SUBS_TERTIARY    = OTT_TERTIARY_SUBS_V01 ,   /* Tertiary */
  QMI_OTTI_SUBS_MAX,                                    /* Max supp subs */
  QMI_OTTI_SUBS_DONT_CARE   = OTT_DONT_CARE_SUBS_V01    /* Don't care subs */
} qmi_otti_subs_enum_type;

/*---------------------------------------------------------------------------
  QMI OTTService state info
---------------------------------------------------------------------------*/

typedef enum{
  QMI_OTTI_CURRENT_REPORT_INVALID      = 0x00,
  QMI_OTTI_CURRENT_REPORT_BY_TIME      = 0x01,
  QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD = 0x02
}qmi_otti_thrput_report_type;

typedef struct{
  qmi_client_handle            reporting_clnt;
  qmi_otti_thrput_report_type report_type;
}qmi_otti_thrput_clnt_info_type;

typedef struct
{
  q_type                 client_q; /*< Information about all the clients that
                                       have connected to the service. This is 
                                       queue of qmi_otti_client_info_type */
  qmi_csi_service_handle service_handle;
  qmi_sap_client_handle  sap_client_handle;
  uint16                 num_of_clients;
  
  qmi_otti_thrput_clnt_info_type ul_thrput_clnt_info[QMI_OTTI_SUBS_MAX];
  qmi_otti_thrput_clnt_info_type dl_thrput_clnt_info[QMI_OTTI_SUBS_MAX];

  ps_sys_ul_thrput_actual_thres_criteria_type  ul_client_request_config[OTTI_MAX_THRPUT_RAT_COUNT];
  ps_sys_dl_thrput_actual_thres_criteria_type  dl_client_request_config[OTTI_MAX_THRPUT_RAT_COUNT];
} qmi_otti_state_info_type;

typedef enum
{
  QMI_OTTI_CMD_UL_THRPT_STATUS_CHANGE = 0,
  QMI_OTTI_CMD_UL_THRPT_INFO = 1,
  QMI_OTTI_CMD_DL_THRPT_STATUS_CHANGE = 2,
  QMI_OTTI_CMD_DL_THRPT_INFO = 3,
  QMI_OTTI_CMD_MAX
} qmi_otti_cmd_type;

static qmi_otti_state_info_type      qmi_ott_state;
static qmi_csi_os_params             os_params;
static boolean                       qmi_otti_inited = FALSE;


/* This is the default QMI_OTTcmd buffer. Make sure that it does not grow
 * beyond 512 bytes. In case you need a command buffer larger than 512 bytes,
 * declare a separate structure. */
typedef struct
{
  uint16  cmd_id; /* qmi_cmd_id_e_type */
  uint32 subscription_id;
  union
  {
    ott_uplink_throughput_reporting_status_ind_msg_v01    ul_thrpt_status;  
    ott_downlink_throughput_reporting_status_ind_msg_v01  dl_thrpt_status; 
    ott_uplink_throughput_info_ind_msg_v01    ul_thrpt_info;
    ott_downlink_throughput_info_ind_msg_v01  dl_thrpt_info;
  } data; 
} qmi_otti_cmd_buf_type;



/*===========================================================================

                               INTERNAL DATA

===========================================================================*/

/*---------------------------------------------------------------------------
  QMI service command handlers
  forward declarations & cmd handler dispatch table definition
---------------------------------------------------------------------------*/

static boolean qmi_otti_msg_hdlr_cb
(
  dcc_sig_enum_type  sig,
  void              *svc_ptr
);

static qmi_csi_cb_error qmi_otti_svc_connect_cb
(
  qmi_client_handle          client_handle,
  void                      *service_handle,
  void                     **connection_handle
);

static void qmi_otti_svc_disconnect_cb
(
  void                      *connection_handle,
  void                      *service_handle
);

static qmi_csi_cb_error qmi_otti_handle_client_req_cb
(
  void                     *connection_handle,
  qmi_req_handle           req_handle,
  unsigned int             msg_id,
  void                     *req_c_struct,
  unsigned int             req_c_struct_len,
  void                     *service_handle
);

static void qmi_otti_process_cmd
(
  dcc_cmd_enum_type  cmd,
  void              *cmd_ptr
);

static void *qmi_otti_get_cmd_buf
(
  qmi_otti_cmd_type cmd_id
);

static void qmi_otti_reg_sys_events
( 
  void
);

static qmi_csi_cb_error  qmi_otti_bind_subscription(
  qmi_otti_client_info_type        *clnt_info,
  qmi_req_handle           req_handle,
  int                      msg_id,
  void                    *req_c_struct,
  int                      req_c_struct_len,
  void                     *service_handle
);

static qmi_csi_cb_error  qmi_otti_get_bind_subscription(
  qmi_otti_client_info_type        *clnt_info,
  qmi_req_handle           req_handle,
  int                      msg_id,
  void                    *req_c_struct,
  int                      req_c_struct_len,
  void                     *service_handle
);

uint32 qmi_otti_resolve_client_subs
(
  void         * cl_sp
);

static boolean qmi_otti_verify_client_subs
(
  qmi_otti_client_info_type         * cl_sp,
  uint32                               event_subs_id,
  boolean                              call_specific
);

static qmi_csi_cb_error  qmi_otti_indication_register(
  qmi_otti_client_info_type        *clnt_info,
  qmi_req_handle           req_handle,
  int                      msg_id,
  void                    *req_c_struct,
  int                      req_c_struct_len,
  void                     *service_handle
);

static qmi_csi_cb_error  qmi_otti_configure_uplink_throughupt_info_settings(
  qmi_otti_client_info_type        *clnt_info,
  qmi_req_handle           req_handle,
  int                      msg_id,
  void                    *req_c_struct,
  int                      req_c_struct_len,
  void                     *service_handle
);

int16 qmi_otti_update_time_based_throughput_config
(
  qmi_otti_client_info_type  * cl_sp_in,
  boolean                      uplink_reporting,
  boolean                      client_inited,
  int16                      * ps_errno
);

static void otti_ul_thrpt_status_change_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
);

static void qmi_otti_generate_ul_thrpt_status_ind
(
  ott_uplink_throughput_reporting_status_ind_msg_v01   * thrpt_status,
  ps_sys_subscription_enum_type                         subscription_id
);

static void qmi_otti_ul_thrpt_status_ind
(
  void                                                 * client,
  ott_uplink_throughput_reporting_status_ind_msg_v01   * thrpt_status
);

static qmi_csi_cb_error   qmi_otti_query_uplink_throughput_reporting_status
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
);

static qmi_csi_cb_error   qmi_otti_query_uplink_throughput_info
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
);

static void otti_ul_thrpt_info_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
);

static void qmi_otti_generate_ul_thrpt_info_ind
(
  ott_uplink_throughput_info_ind_msg_v01   * thrpt_info,
  ps_sys_subscription_enum_type              subscription_id
);

static void qmi_otti_ul_thrpt_info_ind
(
  void                                     * client,
  ott_uplink_throughput_info_ind_msg_v01   * thrpt_info
);

static qmi_csi_cb_error  qmi_otti_configure_downlink_throughupt_info_settings(
  qmi_otti_client_info_type        *clnt_info,
  qmi_req_handle           req_handle,
  int                      msg_id,
  void                    *req_c_struct,
  int                      req_c_struct_len,
  void                     *service_handle
);

static void otti_dl_thrpt_status_change_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
);

static void qmi_otti_generate_dl_thrpt_status_ind
(
  ott_downlink_throughput_reporting_status_ind_msg_v01   * thrpt_status,
  ps_sys_subscription_enum_type                         subscription_id
);

static void qmi_otti_dl_thrpt_status_ind
(
  void                                                   * client,
  ott_downlink_throughput_reporting_status_ind_msg_v01   * thrpt_status
);

static qmi_csi_cb_error   qmi_otti_query_downlink_throughput_reporting_status
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
);

static void otti_dl_thrpt_info_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
);

static void qmi_otti_generate_dl_thrpt_info_ind
(
  ott_downlink_throughput_info_ind_msg_v01   * thrpt_info,
  ps_sys_subscription_enum_type              subscription_id
);

static void qmi_otti_dl_thrpt_info_ind
(
  void                                       * client,
  ott_downlink_throughput_info_ind_msg_v01   * thrpt_info
);

static qmi_csi_cb_error   qmi_otti_query_downlink_throughput_info
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
);

int16 qmi_otti_update_threshold_throughput_config
(
  qmi_otti_client_info_type  *  cl_sp_in,
  boolean                       uplink_reporting,
  boolean                       client_inited,
  int16                       * ps_errno,
  int16                       * threshold_err,
  void                        *req_ptr
);

static void otti_ul_thrpt_threshold_status_change_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
);

static void otti_dl_thrpt_threshold_status_change_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
);

static qmi_csi_cb_error   qmi_otti_get_capabilities
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
);


/*===========================================================================

                       FORWARD FUNCTION DECLARATIONS 

===========================================================================*/


/* Define service handle table for OTTmessages */
static qmi_csi_cb_error (* const req_handle_table[])
(
 qmi_otti_client_info_type        *clnt_info,
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
  qmi_otti_bind_subscription,            /* Request handler for message ID 0x20 */
  qmi_otti_get_bind_subscription,        /* Request handler for message ID 0x21 */
  qmi_otti_indication_register,          /* Request handler for message ID 0x22 */
  qmi_otti_configure_uplink_throughupt_info_settings,  /* Request handler for message ID 0x23 */
  NULL,                                  /* Request handler for message ID 0x24 */
  qmi_otti_query_uplink_throughput_reporting_status,  /* Request handler for message ID 0x25 */ 
  NULL,                                  /* Request handler for message ID 0x26 */ 
  qmi_otti_query_uplink_throughput_info,  /* Request handler for message ID 0x27 */ 
  qmi_otti_configure_downlink_throughupt_info_settings, /* Request handler for message ID 0x28 */ 
  NULL,                                               /* Request handler for message ID 0x29 */ 
  qmi_otti_query_downlink_throughput_reporting_status, /* Request handler for message ID 0x2A */
  NULL,                                                /* Request handler for message ID 0x2B */
  qmi_otti_query_downlink_throughput_info,              /* Request handler for message ID 0x2C */
  qmi_otti_get_capabilities                            /* Request handler for message ID 0x02D */
};

extern rex_tcb_type dcc_tcb;

// Comparison function for Client handle
static int otti_compare_qmi_client_handle
( 
  void *item_ptr,            // Queue Item
  void *compare_val          // Comparison value
)
{
  qmi_otti_client_info_type *clnt_info = (qmi_otti_client_info_type*)item_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return ( (clnt_info->clnt == 
                          (qmi_client_handle)compare_val) ? 1 : 0 );
}

// Comparison function for connection info
static int otti_compare_connection_info_handle
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

FUNCTION DS_QMI_OTT_ASSERT_WRAPPER()

DESCRIPTION
  Wrapper function for DS_QMI_OTT ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_qmi_ott_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DS_QMI_OTT_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* ds_qmi_ott_assert_wrapper */

/*===========================================================================

                       EXTERNAL FUNCTION DEFINTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_OTT_INIT()

  DESCRIPTION
    Registers the OTT service with QCSI 

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_ott_init
(
   void
)
{
  qmi_csi_error                      rc = QMI_CSI_NO_ERR;
  qmi_sap_error                      src = QMI_SAP_NO_ERR;
  qmi_csi_options                    options;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_INFO1_1("QMI OTT service init %d", qmi_otti_inited);

  if (!qmi_otti_inited)
  {
    /*-------------------------------------------------------------------------
      Setting DCC signal handler for handling QMI messages
    -------------------------------------------------------------------------*/
    (void)dcc_set_sig_handler(DCC_OTT_RDR_SIGNAL, 
                              qmi_otti_msg_hdlr_cb, &qmi_ott_state);
    dcc_enable_sig(DCC_OTT_RDR_SIGNAL);

    /* Set the signal to init again */
    DCC_SET_SIGNAL(DCC_OTT_RDR_SIGNAL);
    return;
  }

  os_params.tcb = &dcc_tcb;
  os_params.sig = OTTI_QCSI_OS_SIGNAL;
  QMI_CSI_OPTIONS_INIT(options);
  (void)mqcsi_register_log_options(&options);

  /*-------------------------------------------------------------------------
    Register QMI OTT service with QCSI
  -------------------------------------------------------------------------*/
  rc = qmi_csi_register_with_options(ott_get_service_object_v01(),
                        qmi_otti_svc_connect_cb,
                        qmi_otti_svc_disconnect_cb, 
                        qmi_otti_handle_client_req_cb, 
                        &qmi_ott_state, &os_params, 
                        &options,
                        &qmi_ott_state.service_handle); 
 
  if(rc != QMI_CSI_NO_ERR)
  {
    ERR_FATAL("Unable to register OTT service! Error %d", rc, 0, 0);
    DS_QMI_OTT_ASSERT(0);
    return;
  }

  LOG_MSG_INFO1_1("QMI OTT service registered with QMI Framework, rc = %d ",
                rc);

  //register with SAP
  src = qmi_sap_register(ott_get_service_object_v01(), NULL, 
                         &qmi_ott_state.sap_client_handle);
  if (src != QMI_SAP_NO_ERR)
  {
    ERR_FATAL("Unable to register OTT service with QSAP! Error %d", src, 0, 0);
    DS_QMI_OTT_ASSERT(0);
    return;
  }
 
  /*-------------------------------------------------------------------------
    Register at power up for interested Events.
  -------------------------------------------------------------------------*/
  qmi_otti_reg_sys_events();

  /* Initialize the queue which will hold all handles of clients */
  (void) q_init(&qmi_ott_state.client_q);
  qmi_ott_state.num_of_clients = 0;

  /*-------------------------------------------------------------------------
    initialize uplink and downlink threshold reporting client to unused
  -------------------------------------------------------------------------*/
  memset(qmi_ott_state.ul_thrput_clnt_info, 0 , sizeof(qmi_ott_state.ul_thrput_clnt_info));
  memset(qmi_ott_state.dl_thrput_clnt_info, 0 , sizeof(qmi_ott_state.dl_thrput_clnt_info));
  memset(&qmi_ott_state.ul_client_request_config, 0, sizeof(qmi_ott_state.ul_client_request_config));
  memset(&qmi_ott_state.dl_client_request_config, 0, sizeof(qmi_ott_state.dl_client_request_config));

  /*-------------------------------------------------------------------------
    Setting QMI OTT service command handler with DCC task process
  -------------------------------------------------------------------------*/
  dcc_set_cmd_handler(DCC_OTT_SVC_CMD, qmi_otti_process_cmd);
}/* qmi_ott_init */

/*===========================================================================
  FUNCTION QMI_OTTI_MSG_HDRL_CB()

  DESCRIPTION
    Registers the callback for each message handler

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_otti_msg_hdlr_cb
(
  dcc_sig_enum_type  sig,
  void              *svc_ptr
)
{
  qmi_otti_state_info_type *ott_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_OTT_ASSERT(sig == DCC_OTT_RDR_SIGNAL && svc_ptr);

  if (!qmi_otti_inited)
  {
    qmi_otti_inited = TRUE;
    /* Call init again to register with QCSI now
     * Wait for all tasks to be inited */
    qmi_ott_init();
    return TRUE;
  }

  ott_sp = (qmi_otti_state_info_type*) svc_ptr;

  qmi_csi_handle_event(ott_sp->service_handle, &os_params);
  return TRUE;
} /* qmi_otti_msg_hdlr_cb */

/*===========================================================================
  FUNCTION QMI_OTTI_SVC_CONNECT_CB()

  DESCRIPTION
    Allocates the given client

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_otti_svc_connect_cb
(
  qmi_client_handle          client_handle,
  void                      *service_handle,
  void                     **connection_handle
)
{
  qmi_otti_client_info_type    *clnt_info_ptr;
  qmi_otti_state_info_type     *ott_svc_ptr;
  qmi_csi_cb_error             rc = QMI_CSI_CB_NO_ERR;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_QMI_OTT_ASSERT(connection_handle && service_handle);

  ott_svc_ptr = (qmi_otti_state_info_type *) service_handle;
 
  if ( NULL != q_linear_search( &ott_svc_ptr->client_q,
                                otti_compare_qmi_client_handle,
                                client_handle ) )
  {
    LOG_MSG_ERROR_1("otti_svc_connect_cb: OTT client 0x%p present",
                  client_handle);
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }

  if (OTT_MAX_CLIDS == ott_svc_ptr->num_of_clients)
  {
    LOG_MSG_ERROR_1("otti_svc_connect_cb: Maximum number of clients already "
                  "allocated %d", OTT_MAX_CLIDS);
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC(clnt_info_ptr,
                sizeof(qmi_otti_client_info_type),
                qmi_otti_client_info_type*); 

  if(!clnt_info_ptr)
  {
    rc = QMI_CSI_CB_CONN_REFUSED;
    goto bail;
  }

  memset(clnt_info_ptr, 0, sizeof(qmi_otti_client_info_type));
  (void) q_link ( clnt_info_ptr, &clnt_info_ptr->link );
  clnt_info_ptr->clnt = client_handle;
  /* Add it to the queue of clients */
  q_put( &(ott_svc_ptr->client_q), &(clnt_info_ptr->link) );
  ott_svc_ptr->num_of_clients++;

  *connection_handle = clnt_info_ptr;
  LOG_MSG_INFO1_1("otti_svc_connect_cb: Alloc client 0x%p", 
                clnt_info_ptr);
  rc = QMI_CSI_CB_NO_ERR;
bail:
  return rc;
} /* qmi_otti_svc_connect_cb() */

/*===========================================================================
  FUNCTION QMI_OTTI_SVC_DISCONNECT_CB()

  DESCRIPTION
    Releases the given client

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_otti_svc_disconnect_cb
(
  void                      *connection_handle,
  void                      *service_handle
)
{
  qmi_otti_state_info_type     *ott_svc_ptr;
  qmi_otti_client_info_type    *clnt_info_ptr;
  int16                         ps_errno = DS_ENOERR;
  int16                         return_val = DSS_SUCCESS;
  ps_sys_ul_thrput_thres_criteria_type ul_threshold;
  ps_sys_dl_thrput_thres_criteria_type dl_threshold;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(connection_handle && service_handle);

  memset(&ul_threshold, 0, sizeof(ps_sys_ul_thrput_thres_criteria_type));
  memset(&dl_threshold, 0, sizeof(ps_sys_dl_thrput_thres_criteria_type));

  ott_svc_ptr = (qmi_otti_state_info_type *)service_handle;

  LOG_MSG_INFO1_1("otti_svc_disconnect_cb: Releasing client 0x%p", 
                   connection_handle);

  clnt_info_ptr = q_linear_search( &ott_svc_ptr->client_q,
                                   otti_compare_connection_info_handle,
                                   connection_handle );
  if (NULL == clnt_info_ptr)
  {
    LOG_MSG_ERROR_1("otti_svc_disconnect_cb: OTT client 0x%p not found",
                  connection_handle);
    return;

  }

  //Update uplink configurations before releasing client
  qmi_otti_update_time_based_throughput_config(clnt_info_ptr , TRUE,
                                      FALSE, &ps_errno);

  //Update downlink interval before releasing client
  qmi_otti_update_time_based_throughput_config(clnt_info_ptr , FALSE,
                                      FALSE, &ps_errno);


  if(clnt_info_ptr->subscription_id < QMI_OTTI_SUBS_MAX)
  {
    if( clnt_info_ptr->clnt == qmi_ott_state.ul_thrput_clnt_info[clnt_info_ptr->subscription_id].reporting_clnt)
    {
      //Update uplink threshold client before releasing client
      qmi_ott_state.ul_thrput_clnt_info[clnt_info_ptr->subscription_id].reporting_clnt = QMI_SVC_CLID_UNUSED;
      qmi_ott_state.ul_thrput_clnt_info[clnt_info_ptr->subscription_id].report_type = QMI_OTTI_CURRENT_REPORT_INVALID;
  
      ul_threshold.reset_to_defaults = TRUE;
      return_val = ps_sys_ioctl_ex(PS_SYS_TECH_3GPP,
                                   PS_SYS_IOCTL_3GPP_SET_UL_THRPUT_THRES_CRITERIA,
                                   (ps_sys_subscription_enum_type) clnt_info_ptr->subscription_id,
                                   &ul_threshold,
                                   &ps_errno);
    }
 
    if( clnt_info_ptr->clnt == qmi_ott_state.dl_thrput_clnt_info[clnt_info_ptr->subscription_id].reporting_clnt)
    {
       //update downlink threshold client before releasing client
      qmi_ott_state.dl_thrput_clnt_info[clnt_info_ptr->subscription_id].reporting_clnt = QMI_SVC_CLID_UNUSED;
      qmi_ott_state.dl_thrput_clnt_info[clnt_info_ptr->subscription_id].report_type = QMI_OTTI_CURRENT_REPORT_INVALID;
  
      dl_threshold.reset_to_defaults = TRUE;
      return_val = ps_sys_ioctl_ex(PS_SYS_TECH_3GPP,
                                   PS_SYS_IOCTL_3GPP_SET_DL_THRPUT_THRES_CRITERIA,
                                   (ps_sys_subscription_enum_type) clnt_info_ptr->subscription_id,
                                   &dl_threshold,
                                   &ps_errno);
    }
  }
  
  if ( TRUE != q_delete_ext( &ott_svc_ptr->client_q,
                             &clnt_info_ptr->link ) )
  {
    LOG_MSG_ERROR_1("OTT client 0x%p could not be deleted from queue",
                  clnt_info_ptr);
    return;
  }
  PS_SYSTEM_HEAP_MEM_FREE(clnt_info_ptr);
  ott_svc_ptr->num_of_clients--;
} /* qmi_otti_svc_disconnect_cb() */

/*===========================================================================
  FUNCTION QMI_OTTI_HANDLE_CLIENT_REQ_CB()

  DESCRIPTION
    Registers callback for client request

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_otti_handle_client_req_cb
(
  void                     *connection_handle,
  qmi_req_handle           req_handle,
  unsigned int             msg_id,
  void                     *req_c_struct,
  unsigned int             req_c_struct_len,
  void                     *service_handle
)
{
  qmi_csi_cb_error               rc = QMI_CSI_CB_NO_ERR;
  qmi_otti_client_info_type     *clnt_info_ptr;
  qmi_otti_state_info_type      *ott_svc_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(connection_handle && service_handle);

  rc = QMI_CSI_CB_NO_ERR;
  ott_svc_ptr = (qmi_otti_state_info_type*) service_handle;

  clnt_info_ptr = q_linear_search( &ott_svc_ptr->client_q,
                                   otti_compare_connection_info_handle,
                                   connection_handle );
  if (NULL == clnt_info_ptr)
  {
    LOG_MSG_ERROR_1("otti_handle_req_cb: Invalid clnt 0x%p",
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
      LOG_MSG_ERROR_1("otti_handle_req_cb: NULL message ID handler: %d",
                    msg_id);
      rc = QMI_CSI_CB_UNSUPPORTED_ERR;
    }
  }
  else
  {
    LOG_MSG_ERROR_1("otti_handle_req_cb: Invalid message ID: %d",
                   msg_id);
    rc = QMI_CSI_CB_UNSUPPORTED_ERR;
  }

  return rc;
} /* qmi_otti_handle_client_req_cb() */

/*===========================================================================
  FUNCTION QMI_OTTI_GET_CMD_BUF()

  DESCRIPTION
    Allocate and assign a QMI OTT command buffer from the PS MEM heap
    based on the QMI CMD type

  PARAMETERS
    cmd_id - QMI command type

  RETURN VALUE
    cmd_buf_ptr - Pointer to the allocated command buffer

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void *qmi_otti_get_cmd_buf
(
  qmi_otti_cmd_type cmd_id
)
{
/*-------------------------------------------------------------------------*/
  if(cmd_id < QMI_OTTI_CMD_MAX)
  {
    qmi_otti_cmd_buf_type *cmd_buf_ptr = NULL;
    PS_SYSTEM_HEAP_MEM_ALLOC(cmd_buf_ptr,
                  sizeof(qmi_otti_cmd_buf_type),
                  qmi_otti_cmd_buf_type*); 
    return ((void*)cmd_buf_ptr);
  }
  else
  {
    LOG_MSG_ERROR_INT_1("Unknown QMI OTT command [d]",(int32)cmd_id);
    return NULL;
  }
} /* qmi_otti_get_cmd_buf */

/*===========================================================================
  FUNCTION qmi_otti_process_cmd()

  DESCRIPTION
    This function processes a QMI OTT command

    It is called by the QMI command handler and will dispatch the
    associated command/event handler function.

  PARAMETERS
    cmd:      DCC cmd to be processed
    cmd_ptr:  private data buffer containing the QMI OTT command
              information.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI OTT must already have been initialized

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_otti_process_cmd
(
  dcc_cmd_enum_type  cmd,
  void              *cmd_ptr
)
{
  qmi_otti_cmd_buf_type *cmd_buf_ptr = NULL;
/*-------------------------------------------------------------------------*/

  DS_QMI_OTT_ASSERT(cmd_ptr != NULL);
  cmd_buf_ptr = (qmi_otti_cmd_buf_type *)cmd_ptr;

  LOG_MSG_INFO1_2("qmi_otti_process_cmd: cmdbuf's cmd_id:%d  cmd %d", 
                 cmd_buf_ptr->cmd_id, cmd);

  switch(cmd_buf_ptr->cmd_id)
  {
    case QMI_OTTI_CMD_UL_THRPT_STATUS_CHANGE:
      qmi_otti_generate_ul_thrpt_status_ind
      (
         &cmd_buf_ptr->data.ul_thrpt_status, 
         cmd_buf_ptr->subscription_id
      );
      break;

    case QMI_OTTI_CMD_UL_THRPT_INFO:
      qmi_otti_generate_ul_thrpt_info_ind
      (
         &cmd_buf_ptr->data.ul_thrpt_info,
         cmd_buf_ptr->subscription_id
      );
      break;

    case QMI_OTTI_CMD_DL_THRPT_STATUS_CHANGE:
      qmi_otti_generate_dl_thrpt_status_ind
      (
         &cmd_buf_ptr->data.dl_thrpt_status,
         cmd_buf_ptr->subscription_id
      );
      break;

    case QMI_OTTI_CMD_DL_THRPT_INFO:
      qmi_otti_generate_dl_thrpt_info_ind
      (
         &cmd_buf_ptr->data.dl_thrpt_info,
         cmd_buf_ptr->subscription_id
      );
      break;

    default:
      LOG_MSG_ERROR_INT_1("Unexpected cmd(d) recd in QMI OTT cmd handler",
                    cmd_buf_ptr->cmd_id);
      DS_QMI_OTT_ASSERT(0);
      
  }

  PS_SYSTEM_HEAP_MEM_FREE(cmd_ptr);
  return;
} /* qmi_otti_process_cmd() */

/*===========================================================================
  FUNCTION QMI_OTTI_REG_SYS_EVENTS()

  DESCRIPTION
    Registers for PS system level events

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_otti_reg_sys_events
( 
  void
)
{
  int                      sub_ind = 0;
  ps_sys_event_enum_type   event_id   = PS_SYS_EVENT_MIN;
  int16                    result = 0;
  int16                    dss_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for (sub_ind = PS_SYS_PRIMARY_SUBS; sub_ind < PS_SYS_SUBS_MAX; sub_ind++)
  {
     result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                  PS_SYS_EVENT_UL_THROUGHPUT_INTERVAL_STATUS_CHANGE,
                                  (ps_sys_subscription_enum_type) sub_ind,
                                  otti_ul_thrpt_status_change_cb,
                                  NULL,
                                  &dss_errno);
      if(result < 0)
      {
        event_id = PS_SYS_EVENT_UL_THROUGHPUT_INTERVAL_STATUS_CHANGE;
        break;
      }

      result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                    PS_SYS_EVENT_UL_THROUGHPUT_INFO,
                                    (ps_sys_subscription_enum_type) sub_ind,
                                    otti_ul_thrpt_info_cb,
                                    NULL,
                                    &dss_errno);
      if(result < 0)
      {
        event_id = PS_SYS_EVENT_UL_THROUGHPUT_INFO;
        break;
      }

     result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                  PS_SYS_EVENT_UPDATED_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE,
                                  (ps_sys_subscription_enum_type) sub_ind,
                                  otti_dl_thrpt_status_change_cb,
                                  NULL,
                                  &dss_errno);
      if(result < 0)
      {
        event_id = PS_SYS_EVENT_UPDATED_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE;
        break;
      }

      result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                    PS_SYS_EVENT_DL_THROUGHPUT_INFO,
                                    (ps_sys_subscription_enum_type) sub_ind,
                                    otti_dl_thrpt_info_cb,
                                    NULL,
                                    &dss_errno);
      if(result < 0)
      {
        event_id = PS_SYS_EVENT_DL_THROUGHPUT_INFO;
        break;
      }
      
      result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                    PS_SYS_EVENT_UL_THRPUT_ACTUAL_THRES_CRITERIA,
                                    (ps_sys_subscription_enum_type) sub_ind,
                                    otti_ul_thrpt_threshold_status_change_cb,
                                    NULL,
                                    &dss_errno);
      if(result < 0)
      {
        event_id = PS_SYS_EVENT_UL_THRPUT_ACTUAL_THRES_CRITERIA;
        break;
      }
      
      result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                    PS_SYS_EVENT_DL_THRPUT_ACTUAL_THRES_CRITERIA,
                                    (ps_sys_subscription_enum_type) sub_ind,
                                    otti_dl_thrpt_threshold_status_change_cb,
                                    NULL,
                                    &dss_errno);
      if(result < 0)
      {
        event_id = PS_SYS_EVENT_DL_THRPUT_ACTUAL_THRES_CRITERIA;
        break;
      }
  } /* end for */

  if(result < 0)
  {
    LOG_MSG_ERROR_INT_2("Couldn't reg for ps sys event_id = d Err : (d)",
                    event_id, dss_errno);
    DS_QMI_OTT_ASSERT(0);
    return;
  }
}/* qmi_otti_reg_sys_events */

/*===========================================================================
  FUNCTION QMI_OTTI_BIND_SUBSCRIPTION()

  DESCRIPTION
    Binds user given subscription to this client

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer
 
  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_otti_bind_subscription
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
)
{
  ott_bind_subscription_req_msg_v01      *req_ptr;
  ott_bind_subscription_resp_msg_v01      resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && 
         service_handle);

  req_ptr = (ott_bind_subscription_req_msg_v01 *)req_c_struct;

  LOG_MSG_INFO1_2 ("Client 0x%p subscription %d", clnt_info, 
                   req_ptr->subscription);

  memset(&resp_msg, 0, sizeof(ott_bind_subscription_resp_msg_v01));

  if (( req_ptr->subscription > OTT_TERTIARY_SUBS_V01 || 
        req_ptr->subscription < OTT_DEFAULT_SUBS_V01) &&
        req_ptr->subscription != OTT_DONT_CARE_SUBS_V01 )
  {
    resp_msg.resp.error = QMI_ERR_INVALID_ARG_V01;
  }

  if ( QMI_ERR_NONE_V01 != resp_msg.resp.error )
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                      sizeof(ott_bind_subscription_resp_msg_v01));
    return QMI_CSI_CB_NO_ERR;
  }

  clnt_info->subscription_id =
    (ott_bind_subscription_enum_v01)req_ptr->subscription;

  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_bind_subscription_resp_msg_v01));

  return QMI_CSI_CB_NO_ERR;

} /* qmi_otti_bind_subscription() */

/*===========================================================================
  FUNCTION QMI_OTTI_GET_BIND_SUBSCRIPTION()

  DESCRIPTION
    Returns already bound subscription to this client

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error   qmi_otti_get_bind_subscription
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
)
{
  ott_get_bind_subscription_resp_msg_v01  resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && 
         service_handle);

  memset(&resp_msg, 0, sizeof(ott_get_bind_subscription_resp_msg_v01));

  LOG_MSG_INFO1_2("Client 0x%p Subs id %d",clnt_info, 
                   clnt_info->subscription_id);
 
  resp_msg.subscription_valid = TRUE;
  resp_msg.subscription = 
    (ott_bind_subscription_enum_v01)clnt_info->subscription_id;
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_get_bind_subscription_resp_msg_v01));

  return QMI_CSI_CB_NO_ERR;
}/* qmi_otti_get_bind_subscription */

/*===========================================================================
  FUNCTION QMI_OTTI_RESOLVE_CLIENT_SUBS()

  DESCRIPTION
    This function resolves the client subscription.
 
  PARAMETERS
    cl_sp         : client state pointer

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
uint32 qmi_otti_resolve_client_subs
(
  void         * cl_sp
)
{
  qmi_otti_client_info_type *       client_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_OTT_ASSERT(cl_sp != NULL);

  client_sp = (qmi_otti_client_info_type *)cl_sp;

  if (client_sp->subscription_id == OTT_DONT_CARE_SUBS_V01 || 
      client_sp->subscription_id == OTT_DEFAULT_SUBS_V01)
  {
    return ps_sys_get_default_data_subscription();
  }
  else
  {
    return(client_sp->subscription_id);
  }
}/* qmi_otti_resolve_client_subs */


/*===========================================================================
  FUNCTION QMI_OTTI_VERIFY_CLIENT_SUBS()

  DESCRIPTION
    This function verifies if the client subscription is applicable to
    the event subscription.
 
  PARAMETERS
    cl_sp         : client state pointer
    event_subs_id : Subscription of the event
    call_specific : Identifies if the request is related to a data call

  RETURN VALUE
   TRUE : if the event is applicable to client
   FALSE: otherwise

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_otti_verify_client_subs
(
  qmi_otti_client_info_type         * cl_sp,
  uint32                               event_subs_id,
  boolean                              call_specific
)
{
  boolean                             compare_result = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_OTT_ASSERT(cl_sp != NULL);

  if (event_subs_id > OTT_TERTIARY_SUBS_V01 || 
      event_subs_id <= OTT_DEFAULT_SUBS_V01)
  {
    //event subs should be valid for resolving
     LOG_MSG_ERROR_INT_1( "Cannot resolve: Invalid subs passed: d", event_subs_id );
     return FALSE;
  }

  if (cl_sp->subscription_id == event_subs_id)
  {
    //Client subs and event subs match
    compare_result = TRUE;
    goto send_result;
  }

  if (event_subs_id == ps_sys_get_default_data_subscription())
  {
    if (cl_sp->subscription_id == OTT_DEFAULT_SUBS_V01 ||
        cl_sp->subscription_id == OTT_DONT_CARE_SUBS_V01)
    {
      //Send to DDS and Don't care clients
      compare_result = TRUE;
      goto send_result;
    }
  }
  else
  {
    if (cl_sp->subscription_id == OTT_DONT_CARE_SUBS_V01 &&
        call_specific == TRUE) 
    {
      //Send all call specific events to don't care clients
      compare_result = TRUE;
      goto send_result;
    }
  }

send_result:
  LOG_MSG_INFO1_3( "Cl subs %d, event_subs %d compare_result %d", 
                   cl_sp->subscription_id, event_subs_id, compare_result );
  return compare_result;
}/* qmi_otti_verify_client_subs */

/*===========================================================================
  FUNCTION QMI_OTTI_INDICATION_REGISTER()

  DESCRIPTION
    This function registers the control point for different indications

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer
 
  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error qmi_otti_indication_register
(
   qmi_otti_client_info_type *clnt_info,
   qmi_req_handle             req_handle,
   int                        msg_id,
   void                      *req_c_struct,
   int                        req_c_struct_len,
   void                      *service_handle
)
{
   ott_indication_register_req_msg_v01      *req_ptr;
   ott_indication_register_resp_msg_v01      resp_msg;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && 
         service_handle);

  req_ptr = (ott_indication_register_req_msg_v01 *)req_c_struct;
  memset(&resp_msg, 0, sizeof(ott_indication_register_resp_msg_v01));

  if ( TRUE ==  req_ptr->report_ul_thrpt_reporting_status_change_valid )
  {
    clnt_info->report_status.report_ul_thrpt_reporting_status_change = 
      req_ptr->report_ul_thrpt_reporting_status_change;
  }

  if ( TRUE ==  req_ptr->report_ul_thrpt_info_valid )
  {
    clnt_info->report_status.report_ul_thrpt_info = 
      req_ptr->report_ul_thrpt_info;
  }

  if ( TRUE ==  req_ptr->report_dl_thrpt_reporting_status_change_valid )
  {
    clnt_info->report_status.report_dl_thrpt_reporting_status_change = 
      req_ptr->report_dl_thrpt_reporting_status_change;
  }

  if ( TRUE ==  req_ptr->report_dl_thrpt_info_valid )
  {
    clnt_info->report_status.report_dl_thrpt_info = 
      req_ptr->report_dl_thrpt_info;
  }

  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_indication_register_resp_msg_v01));

  return QMI_CSI_CB_NO_ERR;
}/* qmi_otti_indication_register */

/*===========================================================================
  FUNCTION QMI_OTTI_CONFIGURE_UPLINK_THROUGHPUT_INFO_SETTINGS()

  DESCRIPTION
    Configures the uplink throughput info settings

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer
 
  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_otti_configure_uplink_throughupt_info_settings(
  qmi_otti_client_info_type        *clnt_info,
  qmi_req_handle           req_handle,
  int                      msg_id,
  void                    *req_c_struct,
  int                      req_c_struct_len,
  void                     *service_handle
)
{
  ott_configure_uplink_throughput_settings_req_msg_v01  *req_ptr;
  ott_configure_uplink_throughput_settings_resp_msg_v01 resp_msg;
  int16                                 return_val;
  int16                                 ps_errno = 0;
  qmi_csi_cb_error                      rc = QMI_CSI_CB_NO_ERR;
  ps_sys_ul_thrput_thres_criteria_type  ul_threshold;
  uint32                                subscription_id = 0;
  int16                                 err_no = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && 
         service_handle);

  subscription_id = qmi_otti_resolve_client_subs(clnt_info);

  if(subscription_id >= QMI_OTTI_SUBS_MAX)
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error = QMI_ERR_NO_SUBSCRIPTION_V01;
    goto send_result;
  }
  req_ptr = 
    (ott_configure_uplink_throughput_settings_req_msg_v01 *)req_c_struct;
  memset(&resp_msg, 0, 
         sizeof(ott_configure_uplink_throughput_settings_resp_msg_v01));
  
  /*-----------------------------------------------------------
  if none of the optional TLVs are present, fail the request
  ------------------------------------------------------------*/
  if (!(req_ptr->ul_throughput_report_period_valid ||
      req_ptr->rat_type_valid || req_ptr->ul_throughput_hyst_mag_chng_size_valid || 
      req_ptr->ul_throughput_hyst_time_valid || req_ptr->ul_throughput_threshold_list_valid || 
      req_ptr->ul_threshold_action_valid))
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error  = QMI_ERR_MISSING_ARG_V01;
    goto send_result;
  }

  /*-----------------------------------------------------------
  if both time based and threshold based reporting are present
  fail the request
  ------------------------------------------------------------*/
  if (req_ptr->ul_throughput_report_period_valid && 
      (req_ptr->rat_type_valid || req_ptr->ul_threshold_action_valid || 
      req_ptr->ul_throughput_hyst_mag_chng_size_valid || req_ptr->ul_throughput_hyst_time_valid ||
      req_ptr->ul_throughput_threshold_list_valid))
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error  = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }
  
  /*-------------------------------------------------------
  Time based reporting
  ---------------------------------------------------------*/
  if ( req_ptr->ul_throughput_report_period_valid )
  {
    /*-------------------------------------------------------
    Check if Threshold based reporting is on going
    -----------------------------------------------------------*/

    if( QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD == qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type )
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_INVALID_OPERATION_V01;
    goto send_result;
  }
    
    clnt_info->uplink_report_interval = req_ptr->ul_throughput_report_period;

    if ( req_ptr->report_ul_queue_size_valid )
    {
      clnt_info->report_ul_queue_size = req_ptr->report_ul_queue_size;
    }

  //Update timer here
    return_val = qmi_otti_update_time_based_throughput_config( clnt_info, TRUE, 
                                                      TRUE, &ps_errno );

   if (return_val != DSS_SUCCESS)
   {
     LOG_MSG_ERROR_2("ps_sys_conf_set ret [%d] errno [%d]", 
                     return_val, ps_errno);

     switch (ps_errno)
     {
       case DS_NOMEMORY:
         resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
         resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
         break;

       default:
         resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
         resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
         break;
     }
  }
    
    qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type = QMI_OTTI_CURRENT_REPORT_BY_TIME;

    goto send_result;
  } /* end time based reporting */ 

  /*-------------------------------------------------------
  Threshold based reporting
  ---------------------------------------------------------*/
  else
  {
    /*----------------------------------------------------------
    check for on going time based reporting
    -------------------------------------------------------------*/
    if( QMI_OTTI_CURRENT_REPORT_BY_TIME == qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type )
    {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_INVALID_OPERATION_V01;
    goto send_result;
  }

    /*----------------------------------------------------------
    check for first client on threshold reporting
    -------------------------------------------------------------*/

    if((qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD && 
       (qmi_ott_state.ul_thrput_clnt_info[subscription_id].reporting_clnt != clnt_info->clnt)) ||
        qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_TIME)
  {
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_NOT_SUPPORTED_V01;
      goto send_result;
    }
    /*---------------------------------------------------------------
    RAT must be present if other threshold criteria are present
    --------------------------------------------------------------*/
    if(!req_ptr->rat_type_valid && (req_ptr->ul_throughput_hyst_mag_chng_size_valid || 
        req_ptr->ul_throughput_hyst_time_valid || req_ptr->ul_throughput_threshold_list_valid))
    {
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
    }

    /*----------------------------------------------------------
    Set payload, also store in clnt_info for tracking client request purpose
    -------------------------------------------------------------*/
    memset(&ul_threshold, 0 , sizeof(ps_sys_ul_thrput_thres_criteria_type));

    ul_threshold.is_valid_rat = req_ptr->rat_type_valid;
    if( req_ptr->rat_type_valid )
    {
      switch(req_ptr->rat_type)
      {
        case OTT_RAT_WCDMA_V01: 
          ul_threshold.rat = PS_SYS_RAT_EX_3GPP_WCDMA;
          break;
        case OTT_RAT_LTE_V01: 
          ul_threshold.rat = PS_SYS_RAT_EX_3GPP_LTE;
          break;
        default:
          resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
          resp_msg.resp.error = QMI_ERR_INVALID_ARG_V01;
          goto send_result;
      }
    }
    
    ul_threshold.is_valid_hyst_time = req_ptr->ul_throughput_hyst_time_valid;
    if( req_ptr->ul_throughput_hyst_time_valid )
    {
      ul_threshold.hyst_time = req_ptr->ul_throughput_hyst_time;
    }
    
    ul_threshold.is_valid_hyst_mag_chng_size = req_ptr->ul_throughput_hyst_mag_chng_size_valid;
    if( req_ptr->ul_throughput_hyst_mag_chng_size_valid )
    {
      ul_threshold.hyst_mag_chng_size = req_ptr->ul_throughput_hyst_mag_chng_size;
    }

    ul_threshold.is_valid_threshold_list = req_ptr->ul_throughput_threshold_list_valid;
    if( req_ptr->ul_throughput_threshold_list_valid )
    {
      ul_threshold.threshold_list_len= req_ptr->ul_throughput_threshold_list_len;
      memscpy(ul_threshold.threshold_list, 
              PS_SYS_THRESHOLD_LIST_LEN * sizeof(ul_threshold.threshold_list[0]),
              req_ptr->ul_throughput_threshold_list, 
              QMI_OTT_UL_THRESHOLD_LIST_LEN_V01 * sizeof(req_ptr->ul_throughput_threshold_list[0]));
    }

    if( req_ptr->ul_threshold_action_valid )
    {
      switch(req_ptr->ul_threshold_action)
      {
        case OTT_THRESHOLD_ACTION_STOP_V01:
         ul_threshold.action = PS_SYS_THRESHOLD_ACTION_STOP;
         break;
        case OTT_THRESHOLD_ACTION_START_V01:
          ul_threshold.action = PS_SYS_THRESHOLD_ACTION_START;
         break;
        default:
          resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
          resp_msg.resp.error = QMI_ERR_INVALID_ARG_V01;
          goto send_result;
      }
    }
    else
    {
      ul_threshold.action = PS_SYS_THRESHOLD_ACTION_INVALID;
    }
    return_val = ps_sys_ioctl_ex(PS_SYS_TECH_3GPP,
                                 PS_SYS_IOCTL_3GPP_SET_UL_THRPUT_THRES_CRITERIA,
                                 (ps_sys_subscription_enum_type) subscription_id,
                                 &ul_threshold,
                                 &err_no);
   if (return_val != DSS_SUCCESS)
   {
      LOG_MSG_ERROR_2("ps_sys_ioctl ret [%d] errno [%d]", 
                     return_val, ps_errno);

     switch (ps_errno)
     {
       case DS_NOMEMORY:
         resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
         resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
         break;

       default:
         resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
         resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
         break;
     }
     goto send_result;
   }
    
    /*----------------------------------------------------------
    Set first threshold reporting client
    -------------------------------------------------------------*/
    qmi_ott_state.ul_thrput_clnt_info[subscription_id].reporting_clnt = clnt_info->clnt;
    qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type = QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD;

    /*------------------------------------------------------------
      Update QMI cache for the request
    -------------------------------------------------------------*/
    if(req_ptr->rat_type_valid)
    {
      qmi_ott_state.ul_client_request_config[req_ptr->rat_type - 1].rat = ul_threshold.rat;
  
      if(req_ptr->ul_throughput_hyst_time_valid)
      {
        qmi_ott_state.ul_client_request_config[req_ptr->rat_type - 1].hyst_time = ul_threshold.hyst_time;
      }
      if(req_ptr->ul_throughput_hyst_mag_chng_size_valid)
      {
        qmi_ott_state.ul_client_request_config[req_ptr->rat_type - 1].hyst_mag_chng_size = 
            ul_threshold.hyst_mag_chng_size;
      }
      if(req_ptr->ul_throughput_threshold_list_valid)
      {
        qmi_ott_state.ul_client_request_config[req_ptr->rat_type - 1].threshold_list_len = 
          ul_threshold.threshold_list_len;
        memscpy(&qmi_ott_state.ul_client_request_config[req_ptr->rat_type - 1].threshold_list,
                PS_SYS_THRESHOLD_LIST_LEN * sizeof(ul_threshold.threshold_list[0]),
                &ul_threshold.threshold_list, 
                ul_threshold.threshold_list_len * sizeof(ul_threshold.threshold_list[0]));
      }
    }
  }/* end threshold based reporting */

  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  rc = QMI_CSI_CB_NO_ERR;

send_result:
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_configure_uplink_throughput_settings_resp_msg_v01));

  return rc;

}/* qmi_otti_configure_uplink_throughupt_info_settings */

/*===========================================================================
  FUNCTION qmi_otti_update_time_based_throughput_config()

  DESCRIPTION
    Sends indication to clients registered for downlink throughput info 

  PARAMETERS
    cl_sp            : client state pointer
    uplink_reporting : TRUE - uplink request FALSE - downlink request
    client_inited    : TRUE- if client inited, FALSE-if due to client release
    ps_errno         : Err no

  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
int16 qmi_otti_update_time_based_throughput_config
(
  qmi_otti_client_info_type  * cl_sp_in,
  boolean                       uplink_reporting,
  boolean                       client_inited,
  int16                       * ps_errno
)
{
  qmi_otti_state_info_type    * ott_sp;
  qmi_otti_client_info_type *   cl_sp;
  uint32                        subscription_id = 0;
  uint32                        min_timer = 0;
  ps_sys_report_ul_throughput_config_type   ul_info;
  int16                         return_val = 0;
  int16                         err_no = 0;
  q_type                      * client_q_ptr = NULL;
  void                        * client_buf_ptr;
  void                        * next_client_buf_ptr = NULL;
  uint32                        cl_sp_in_report_interval = 0;
  uint32                        curr_report_interval = 0;
  ps_sys_conf_enum_type         conf_to_set = PS_SYS_CONF_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  subscription_id = qmi_otti_resolve_client_subs(cl_sp_in);
  ott_sp = &qmi_ott_state;
  client_q_ptr   = &ott_sp->client_q;
  client_buf_ptr = q_check(client_q_ptr);
  cl_sp_in_report_interval =
     (uplink_reporting == TRUE) ?  cl_sp_in->uplink_report_interval :
                                   cl_sp_in->downlink_report_interval;

  if ( cl_sp_in_report_interval == 0 )
  {
    //Disable timer
    min_timer = 0;
    ul_info.report_ul_queue_size = FALSE;
  }
  else
  {
    if (!client_inited)
    {
      min_timer = OTTI_MAX_TIMER_INTERVAL; //initialise to large val
    }
    else
    {
      min_timer = cl_sp_in_report_interval;
    }

    /*-------------------------------------------------------------------------
      Find minimum timer rquest of all client requets so far(including current)
      If the update is due to client release, find min among remaining clients
    ------------------------------------------------------------------------*/
    while(client_buf_ptr != NULL)
    {
      cl_sp = (qmi_otti_client_info_type *) client_buf_ptr;
      curr_report_interval = (uplink_reporting == TRUE) ?
        cl_sp->uplink_report_interval : cl_sp->downlink_report_interval;

      ul_info.report_ul_queue_size = cl_sp_in->report_ul_queue_size;

      if (( curr_report_interval != 0) &&
          ( subscription_id == qmi_otti_resolve_client_subs(cl_sp) ) &&
            cl_sp != cl_sp_in)
      {
         min_timer = MIN(curr_report_interval, min_timer);
      }
      
      next_client_buf_ptr = 
        q_next(client_q_ptr, &((q_generic_item_type *) client_buf_ptr)->link);
      client_buf_ptr = next_client_buf_ptr;
    }

    if (min_timer == OTTI_MAX_TIMER_INTERVAL) 
    {
      /*-------------------------------------------------------------------------
        No more clients with min reporting interval found; Disable the timer
      ------------------------------------------------------------------------*/
      min_timer = 0;
    }
  } /* cl_sp_in_report_interval != 0 */

  ul_info.report_ul_timer = min_timer;

  conf_to_set = (uplink_reporting == TRUE) ? PS_SYS_CONF_UL_THROUGHPUT_CONFIG :
                                             PS_SYS_CONF_DL_THROUGHPUT_INFO_TIMER;

  return_val = ps_sys_conf_set_ex(PS_SYS_TECH_ALL,
                                  conf_to_set,
                                  (ps_sys_subscription_enum_type) subscription_id,
                                  &ul_info,
                                  &err_no);

  /*---------------------------------------------------
  Reset the report time type if time based is disabled
  -----------------------------------------------------*/
  if( min_timer == 0 && subscription_id < QMI_OTTI_SUBS_MAX)
  {
    if( uplink_reporting == TRUE )
    {
      if(qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_TIME)
      {
        qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type = QMI_OTTI_CURRENT_REPORT_INVALID;
      }
    }
    else
    {
      if(qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_TIME)
      {
        qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type = QMI_OTTI_CURRENT_REPORT_INVALID;
      }
    }
  }
  
  *ps_errno = err_no;
  return return_val;
}/* qmi_otti_update_time_based_throughput_config */

/*===========================================================================
  FUNCTION OTTI_UL_THRPT_STATUS_CHANGE_CB()

  DESCRIPTION
    This the callback that we have registered with PS Sys layer for
    uplink throughput status

  PARAMETERS
    tech_type      : technology for which system status has changed
    event_name     : Event name
    subscription_id: Subscription id
    event_info_ptr : System status event info
    user_data_ptr  : Optional user data to be sent.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void otti_ul_thrpt_status_change_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
)
{
  qmi_otti_cmd_buf_type                                * cmd_ptr = NULL;
  ott_uplink_throughput_reporting_status_ind_msg_v01   * ul_thrpt_status;
  ps_sys_thrpt_status_type                             * ps_thrpt_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_OTT_ASSERT(event_info_ptr != NULL);

  ps_thrpt_status = (ps_sys_thrpt_status_type *)event_info_ptr;
  cmd_ptr = (qmi_otti_cmd_buf_type *)qmi_otti_get_cmd_buf(
                      (qmi_otti_cmd_type)QMI_OTTI_CMD_UL_THRPT_STATUS_CHANGE);

  if( cmd_ptr == NULL)
  {
    return;
  }
  
  memset(cmd_ptr,0,sizeof(qmi_otti_cmd_buf_type));  

  ul_thrpt_status = &cmd_ptr->data.ul_thrpt_status;
  ul_thrpt_status->reporting_status_valid = TRUE;
  ul_thrpt_status->reporting_status =
     (ott_thrpt_status_reason_enum_v01)ps_thrpt_status->thrpt_status;
  ul_thrpt_status->actual_interval_valid = TRUE;
  ul_thrpt_status->actual_interval = ps_thrpt_status->actual_interval;

  cmd_ptr->cmd_id = QMI_OTTI_CMD_UL_THRPT_STATUS_CHANGE;
  cmd_ptr->subscription_id = subscription_id;

  dcc_send_cmd_ex (DCC_OTT_SVC_CMD, cmd_ptr);
} /* otti_ul_thrpt_status_change_cb() */

/*===========================================================================
FUNCTION  QMI_WDSI_GENERATE_UL_THRPT_STATUS_CHANGE_IND

DESCRIPTION
  This function generated uplink throughput status indications

PARAMETERS
  thrpt_status     - Uplink Thrpt status information
  subscription_id  - Subscription identifier

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void qmi_otti_generate_ul_thrpt_status_ind
(
  ott_uplink_throughput_reporting_status_ind_msg_v01   * thrpt_status,
  ps_sys_subscription_enum_type                         subscription_id
)
{
  qmi_otti_client_info_type                   * cl_sp;
  q_type                                      * client_q_ptr = NULL;
  void                                        * client_buf_ptr;
  void                                        * next_client_buf_ptr;
  qmi_otti_state_info_type                    * ott_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(thrpt_status == NULL)
  {
    DS_QMI_OTT_ASSERT(0);
  }

  ott_sp         = &qmi_ott_state;
  client_q_ptr   = &ott_sp->client_q;
  client_buf_ptr = q_check(client_q_ptr);
  
  while(client_buf_ptr != NULL)
  {
    cl_sp = (qmi_otti_client_info_type *) client_buf_ptr;
        
    if ( (TRUE == cl_sp->report_status.report_ul_thrpt_reporting_status_change) &&
         (TRUE == qmi_otti_verify_client_subs(cl_sp, 
                                              (uint32)subscription_id,
                                              FALSE)) )
    {
        qmi_otti_ul_thrpt_status_ind(cl_sp, thrpt_status);
    }
      
    next_client_buf_ptr = 
      q_next(client_q_ptr, &((q_generic_item_type *) client_buf_ptr)->link);
    client_buf_ptr = next_client_buf_ptr;
  }
  
} /* qmi_otti_generate_ul_thrpt_status_ind */

/*===========================================================================
  FUNCTION QMI_OTTI_THRPT_STATUS_IND()

  DESCRIPTION
    This function sends thrpt status indication to registered control point.

  PARAMETERS
    clid            : clientID
    thrpt_status    : thrpt status

  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_otti_ul_thrpt_status_ind
(
  void                                                 * client,
  ott_uplink_throughput_reporting_status_ind_msg_v01    * thrpt_status
)
{
  qmi_otti_client_info_type               * client_info;
  qmi_csi_error                             rc;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  rc = QMI_CSI_NO_ERR;
  client_info = (qmi_otti_client_info_type *) client;
  if (client_info == NULL)
  {
    DS_QMI_OTT_ASSERT(0);
    return;
  }
            
  rc = qmi_csi_send_ind(client_info->clnt, 
                        QMI_OTT_UPLINK_THROUGHPUT_REPORTING_STATUS_IND_V01,
                        thrpt_status, 
                        sizeof(ott_uplink_throughput_reporting_status_ind_msg_v01));
  
  if ( rc != QMI_CSI_NO_ERR )
  {
    LOG_MSG_ERROR_1("qmi_otti_ul_thrpt_status_ind error %d", rc);
  }
} /* qmi_otti_ul_thrpt_status_ind() */

/*===========================================================================
  FUNCTION OTTI_UL_THRPT_THRESHOLD_STATUS_CHANGE_CB()

  DESCRIPTION
    This the callback that we have registered with PS Sys layer for
    uplink throughput threshold status

  PARAMETERS
    tech_type      : technology
    event_name     : Event name
    subscription_id: Subscription id
    event_info_ptr : Uplink threshold status info
    user_data_ptr  : Optional user data to be sent.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void otti_ul_thrpt_threshold_status_change_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
)
{
  qmi_otti_cmd_buf_type                                * cmd_ptr = NULL;
  ott_uplink_throughput_reporting_status_ind_msg_v01   * ul_thrpt_status;
  ps_sys_ul_thrput_actual_thres_criteria_type          * ps_thrpt_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_OTT_ASSERT(event_info_ptr != NULL);

  ps_thrpt_status = (ps_sys_ul_thrput_actual_thres_criteria_type *)event_info_ptr;
  cmd_ptr = (qmi_otti_cmd_buf_type *)qmi_otti_get_cmd_buf(
                      (qmi_otti_cmd_type)QMI_OTTI_CMD_UL_THRPT_STATUS_CHANGE);

  if( cmd_ptr == NULL)
  {
    return;
  }
  
  memset(cmd_ptr,0,sizeof(qmi_otti_cmd_buf_type));  

  ul_thrpt_status = &cmd_ptr->data.ul_thrpt_status;
  ul_thrpt_status->reporting_status_valid = TRUE;
  ul_thrpt_status->reporting_status =
     (ott_thrpt_status_reason_enum_v01)ps_thrpt_status->thrput_status;
  
  if(ps_thrpt_status->thrput_status == PS_SYS_THRPT_REPORT_ENABLED)
  {
  ul_thrpt_status->rat_type_valid= TRUE;
  ul_thrpt_status->actual_ul_throughput_hyst_time_valid = TRUE;
  ul_thrpt_status->actual_ul_throughput_hyst_mag_chng_size_valid = TRUE;
  ul_thrpt_status->actual_ul_throughput_threshold_list_valid = TRUE;

  switch(ps_thrpt_status->rat)
  {
    case(PS_SYS_RAT_EX_3GPP_WCDMA):
      ul_thrpt_status->rat_type = OTT_RAT_WCDMA_V01;
      break;
    case(PS_SYS_RAT_EX_3GPP_LTE):
      ul_thrpt_status->rat_type = OTT_RAT_LTE_V01;
      break;
    default:
      LOG_MSG_ERROR_1("otti_ul_thrpt_threshold_status_change_cb invalid RAT type %d",
                       ps_thrpt_status->rat);
      return;
  }

  ul_thrpt_status->actual_ul_throughput_hyst_time = ps_thrpt_status->hyst_time;
  ul_thrpt_status->actual_ul_throughput_hyst_mag_chng_size = 
    ps_thrpt_status->hyst_mag_chng_size;
  ul_thrpt_status->actual_ul_throughput_threshold_list_len = ps_thrpt_status->threshold_list_len;
    memscpy(ul_thrpt_status->actual_ul_throughput_threshold_list,
            QMI_OTT_UL_THRESHOLD_LIST_LEN_V01 * sizeof(ul_thrpt_status->actual_ul_throughput_threshold_list[0]),
            ps_thrpt_status->threshold_list,
            ps_thrpt_status->threshold_list_len * sizeof(ps_thrpt_status->threshold_list[0]));

    
  }
  cmd_ptr->cmd_id = QMI_OTTI_CMD_UL_THRPT_STATUS_CHANGE;
  cmd_ptr->subscription_id = subscription_id;

  dcc_send_cmd_ex (DCC_OTT_SVC_CMD, cmd_ptr);
} /* otti_ul_thrpt_threshold_status_change_cb() */

/*===========================================================================
  FUNCTION QMI_OTTI_QUERY_UPLINK_THRT_REPORTING_STATUS()

  DESCRIPTION
    Queries uplink throughput reporting status

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error   qmi_otti_query_uplink_throughput_reporting_status
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
)
{
  ott_query_uplink_throughput_reporting_status_resp_msg_v01  resp_msg;
  int16                                 return_val = DSS_SUCCESS;
  qmi_error_type_v01                    errval;
  ps_sys_thrpt_status_type              report_status;
  ps_sys_ul_thrput_actual_thres_criteria_type thres_report_status;
  uint32                                subscription_id = 0;
  int16                                 ps_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && service_handle);

  errval = QMI_ERR_NONE_V01;
  memset(&resp_msg,0,sizeof(resp_msg));
  memset(&report_status, 0, sizeof(ps_sys_thrpt_status_type));
  memset(&thres_report_status, 0, sizeof(ps_sys_ul_thrput_actual_thres_criteria_type));
    
  subscription_id = qmi_otti_resolve_client_subs(clnt_info);

  /*-------------------------------------------------------------------------
    Retrieve ul thrpt status information by calling into PS Sys Layer
  -------------------------------------------------------------------------*/

  if(subscription_id < QMI_OTTI_SUBS_MAX &&
     qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_TIME)
  {
  return_val = ps_sys_conf_get_ex(PS_SYS_TECH_ALL,
                                  PS_SYS_CONF_UL_THROUGHPUT_INTERVAL_STATUS_CHANGE,
                                  (ps_sys_subscription_enum_type) subscription_id,
                                  (void *)&report_status,
                                  &ps_errno);

  if (return_val != DSS_SUCCESS)
  {
    LOG_MSG_ERROR_INT_2("ps_sys_conf_get_ex ret [d] errno [d]", return_val, ps_errno);
    switch (ps_errno)
    {
      case DS_NOMEMORY:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
        break;
      default:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
        break;
    }
    goto send_result;
  }

  resp_msg.reporting_status_valid = TRUE;
  resp_msg.reporting_status = 
    (ott_thrpt_status_reason_enum_v01)report_status.thrpt_status;
  
  resp_msg.actual_interval_valid = TRUE;
  resp_msg.actual_interval = report_status.actual_interval;

  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  }
  
  else if(subscription_id < QMI_OTTI_SUBS_MAX &&
          qmi_ott_state.ul_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD)
  {
    return_val = ps_sys_conf_get_ex(PS_SYS_TECH_ALL,
                                    PS_SYS_CONF_UL_THRPUT_ACTUAL_THRES_CRITERIA,
                                    (ps_sys_subscription_enum_type) subscription_id,
                                    (void *)&thres_report_status,
                                    &ps_errno);
    if (return_val != DSS_SUCCESS)
    {
      LOG_MSG_ERROR_2("ps_sys_conf_get_ex ret [%d] errno [%d]", return_val, ps_errno);
      switch (ps_errno)
      {
        case DS_NOMEMORY:
          resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
          resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
          break;
        default:
          resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
          resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
          break;
      }
      goto send_result;
    }
    resp_msg.reporting_status_valid = TRUE;
    resp_msg.reporting_status = 
      (ott_thrpt_status_reason_enum_v01)thres_report_status.thrput_status;
    resp_msg.rat_type_valid = TRUE;
    switch(thres_report_status.rat)
    {
      case(PS_SYS_RAT_EX_3GPP_WCDMA):
        resp_msg.rat_type = OTT_RAT_WCDMA_V01;
        break;
      case(PS_SYS_RAT_EX_3GPP_LTE):
        resp_msg.rat_type = OTT_RAT_LTE_V01;
        break;
      default:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
        goto send_result;
    }
    resp_msg.actual_ul_throughput_hyst_mag_chng_size_valid = TRUE;
    resp_msg.actual_ul_throughput_hyst_mag_chng_size = thres_report_status.hyst_mag_chng_size;
    resp_msg.actual_ul_throughput_hyst_time_valid = TRUE;
    resp_msg.actual_ul_throughput_hyst_time = thres_report_status.hyst_time;
    
    resp_msg.actual_ul_throughput_threshold_list_valid = TRUE;
    resp_msg.actual_ul_throughput_threshold_list_len = thres_report_status.threshold_list_len;
    
    memscpy(resp_msg.actual_ul_throughput_threshold_list, 
            QMI_OTT_UL_THRESHOLD_LIST_LEN_V01 * sizeof(resp_msg.actual_ul_throughput_threshold_list[0]),
            thres_report_status.threshold_list, 
            thres_report_status.threshold_list_len * sizeof(thres_report_status.threshold_list[0]));
  }
  
  else{
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error = QMI_ERR_INFO_UNAVAILABLE_V01;
  }

send_result:
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_query_uplink_throughput_reporting_status_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

}/* qmi_otti_query_uplink_throughput_reporting_status */

/*===========================================================================
  FUNCTION OTTI_UL_THRPT_INFO_CB()

  DESCRIPTION
    This the callback that we have registered with PS Sys layer for
    uplink throughput info

  PARAMETERS
    tech_type      : technology for which system status has changed
    event_name     : Event name
    subscription_id: Subscription id
    event_info_ptr : System status event info
    user_data_ptr  : Optional user data to be sent.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void otti_ul_thrpt_info_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
)
{
  qmi_otti_cmd_buf_type                   * cmd_ptr = NULL;
  ott_uplink_throughput_info_ind_msg_v01  * ul_thrpt_info;
  ps_sys_ul_throughput_info_type          * ps_thrpt_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_OTT_ASSERT(event_info_ptr != NULL);

  ps_thrpt_info = (ps_sys_ul_throughput_info_type *)event_info_ptr;
  cmd_ptr = (qmi_otti_cmd_buf_type *)qmi_otti_get_cmd_buf(
                      (qmi_otti_cmd_type)QMI_OTTI_CMD_UL_THRPT_INFO);

  if( cmd_ptr == NULL)
  {
    return;
  }
  
  memset(cmd_ptr,0,sizeof(qmi_otti_cmd_buf_type));  

  ul_thrpt_info = &cmd_ptr->data.ul_thrpt_info;
  ul_thrpt_info->uplink_rate_valid = TRUE;
  ul_thrpt_info->uplink_rate = ps_thrpt_info->uplink_allowed_rate;

  ul_thrpt_info->confidence_level_valid = TRUE;
  ul_thrpt_info->confidence_level = ps_thrpt_info->confidence_level;

  ul_thrpt_info->uplink_queue_size_valid = TRUE;
  ul_thrpt_info->uplink_queue_size= ps_thrpt_info->uplink_queue_size;

  cmd_ptr->cmd_id = QMI_OTTI_CMD_UL_THRPT_INFO;
  cmd_ptr->subscription_id = subscription_id;

  dcc_send_cmd_ex (DCC_OTT_SVC_CMD, cmd_ptr);
} /* otti_ul_thrpt_info_cb() */

/*===========================================================================
FUNCTION  QMI_WDSI_GENERATE_THRPT_INFO_IND

DESCRIPTION
  This callback is called to notify clients about thrpt info changes

PARAMETERS
  thrpt_status - Thrpt status information
  subscription_id - Subscription identifier

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void qmi_otti_generate_ul_thrpt_info_ind
(
  ott_uplink_throughput_info_ind_msg_v01   * thrpt_info,
  ps_sys_subscription_enum_type              subscription_id
)
{
  qmi_otti_client_info_type                   * cl_sp;
  q_type                                      * client_q_ptr = NULL;
  void                                        * client_buf_ptr;
  void                                        * next_client_buf_ptr;
  qmi_otti_state_info_type                    * ott_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(thrpt_info == NULL)
  {
    DS_QMI_OTT_ASSERT(0);
  }

  ott_sp         = &qmi_ott_state;
  client_q_ptr   = &ott_sp->client_q;
  client_buf_ptr = q_check(client_q_ptr);
  
  while(client_buf_ptr != NULL)
  {
    cl_sp = (qmi_otti_client_info_type *) client_buf_ptr;
        
    if ((TRUE == cl_sp->report_status.report_ul_thrpt_info) &&
        (TRUE == qmi_otti_verify_client_subs(cl_sp, 
                                             (uint32)subscription_id,
                                              FALSE)) )
    {
        qmi_otti_ul_thrpt_info_ind(cl_sp, thrpt_info);
    }
      
    next_client_buf_ptr = 
      q_next(client_q_ptr, &((q_generic_item_type *) client_buf_ptr)->link);
    client_buf_ptr = next_client_buf_ptr;
  }
} /* qmi_otti_generate_ul_thrpt_info_ind */

/*===========================================================================
  FUNCTION QMI_OTTI_UL_THRPT_INFO_IND()

  DESCRIPTION
    This function sends thrpt info indication to registered control point.

  PARAMETERS
    clid          : clientID
    thrpt_info    : thrpt info

  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_otti_ul_thrpt_info_ind
(
  void                                     * client,
  ott_uplink_throughput_info_ind_msg_v01   * thrpt_info
)
{
  qmi_otti_client_info_type               * client_info;
  qmi_csi_error                             rc;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  rc = QMI_CSI_NO_ERR;
  client_info = (qmi_otti_client_info_type *) client;
  if (client_info == NULL)
  {
    DS_QMI_OTT_ASSERT(0);
    return;
  }

  if ( FALSE == client_info->report_ul_queue_size ) 
  {
    thrpt_info->uplink_queue_size_valid = FALSE;
  }      
  else
  {
    thrpt_info->uplink_queue_size_valid = TRUE;
  }

  rc = qmi_csi_send_ind(client_info->clnt, 
                        QMI_OTT_UPLINK_THROUGHPUT_INFO_IND_V01,
                        thrpt_info, 
                        sizeof(ott_uplink_throughput_info_ind_msg_v01));
  
  if ( rc != QMI_CSI_NO_ERR )
  {
    LOG_MSG_ERROR_INT_1("qmi_otti_ul_thrpt_info_ind error: d", rc);
  }
} /* qmi_otti_ul_thrpt_info_ind() */

/*===========================================================================
  FUNCTION QMI_OTTI_QUERY_UPLINK_THROUGHPUT_INFO()

  DESCRIPTION
    Queries uplink throughput information

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error   qmi_otti_query_uplink_throughput_info
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
)
{
  ott_query_uplink_throughput_info_resp_msg_v01  resp_msg;
  ott_query_uplink_throughput_info_req_msg_v01   *req_ptr;
  int16                                 return_val = DSS_SUCCESS;
  qmi_error_type_v01                    errval;
  ps_sys_ul_throughput_info_type        thrpt_info;
  uint32                                subscription_id = 0;
  int16                                 ps_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && service_handle);

  req_ptr = (ott_query_uplink_throughput_info_req_msg_v01 *)req_c_struct;

  errval = QMI_ERR_NONE_V01;
  memset(&resp_msg,0,sizeof(resp_msg));
  memset(&thrpt_info, 0, sizeof(ps_sys_ul_throughput_info_type));
    
  subscription_id = qmi_otti_resolve_client_subs(clnt_info);

  /*-------------------------------------------------------------------------
    Retrieve dl thrpt status information by calling into PS Sys Layer
  -------------------------------------------------------------------------*/
  return_val = ps_sys_conf_get_ex(PS_SYS_TECH_ALL,
                                  PS_SYS_CONF_UL_THROUGHPUT_INFO,
                                  (ps_sys_subscription_enum_type) subscription_id,
                                  (void *)&thrpt_info,
                                  &ps_errno);

  if (return_val != DSS_SUCCESS)
  {
    LOG_MSG_ERROR_INT_2("ps_sys_conf_get_ex ret [d] errno [d]", return_val, ps_errno);
    switch (ps_errno)
    {
      case DS_NOMEMORY:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
        break;
      default:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
        break;
    }
    goto send_result;
  }

  resp_msg.uplink_rate_valid = TRUE;
  resp_msg.uplink_rate       = thrpt_info.uplink_allowed_rate;
  
  resp_msg.confidence_level_valid = TRUE;
  resp_msg.confidence_level       = thrpt_info.confidence_level;

  if (req_ptr->report_ul_queue_size_valid)
  {
    if (req_ptr->report_ul_queue_size)
    {
      resp_msg.uplink_queue_size_valid = TRUE;
      resp_msg.uplink_queue_size = thrpt_info.uplink_queue_size;
    }
  }
  
  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;

send_result:
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_query_uplink_throughput_info_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

}/* qmi_otti_query_uplink_throughput_info */

/*===========================================================================
  FUNCTION QMI_OTTI_CONFIGURE_DOWNLINK_THROUGHPUT_INFO_SETTINGS ()

  DESCRIPTION
    This function registers the control point for different indications

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer
 
  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error  qmi_otti_configure_downlink_throughupt_info_settings(
  qmi_otti_client_info_type        *clnt_info,
  qmi_req_handle           req_handle,
  int                      msg_id,
  void                    *req_c_struct,
  int                      req_c_struct_len,
  void                     *service_handle
)
{
  ott_configure_downlink_throughput_settings_req_msg_v01  *req_ptr;
  ott_configure_downlink_throughput_settings_resp_msg_v01 resp_msg;
  int16                                 return_val;
  int16                                 ps_errno = 0;
  qmi_csi_cb_error                      rc = QMI_CSI_CB_NO_ERR;
  ps_sys_dl_thrput_thres_criteria_type  dl_threshold;
  uint32                                subscription_id = 0;
  int16                                 err_no = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && 
         service_handle);

  subscription_id = qmi_otti_resolve_client_subs(clnt_info);

  if(subscription_id >= QMI_OTTI_SUBS_MAX)
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error = QMI_ERR_NO_SUBSCRIPTION_V01;
    goto send_result;
  }

  req_ptr = 
    (ott_configure_downlink_throughput_settings_req_msg_v01 *)req_c_struct;
  memset(&resp_msg, 0, 
         sizeof(ott_configure_downlink_throughput_settings_resp_msg_v01));

  /*-----------------------------------------------------------
  if none of the optional TLVs are present, fail the request
  ------------------------------------------------------------*/
  if ((req_ptr->dl_throughput_report_period_valid || 
      req_ptr->rat_type_valid || req_ptr->dl_throughput_hyst_mag_chng_size_valid || 
      req_ptr->dl_throughput_hyst_time_valid || req_ptr->dl_throughput_threshold_list_valid || 
      req_ptr->dl_threshold_action_valid) == 0 )
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error  = QMI_ERR_MISSING_ARG_V01;
    goto send_result;
  }

  /*-----------------------------------------------------------
  if both time based and threshold based reporting are present
  fail the request
  ------------------------------------------------------------*/
  if (req_ptr->dl_throughput_report_period_valid && 
      (req_ptr->rat_type_valid || req_ptr->dl_threshold_action_valid || 
      req_ptr->dl_throughput_hyst_mag_chng_size_valid || req_ptr->dl_throughput_hyst_time_valid ||
      req_ptr->dl_throughput_threshold_list_valid))
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
    resp_msg.resp.error  = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }
  
  /*-------------------------------------------------------
  Time based reporting
  ---------------------------------------------------------*/
  if ( req_ptr->dl_throughput_report_period_valid )
  {
    clnt_info->downlink_report_interval = 
      req_ptr->dl_throughput_report_period;

    /*-------------------------------------------------------
    Check if Threshold based reporting is on going
    -----------------------------------------------------------*/
    if( QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD == qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type )
  {
    resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_INVALID_OPERATION_V01;
    goto send_result;
  }
  
  qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type = QMI_OTTI_CURRENT_REPORT_BY_TIME;

  //Update timer here
   return_val = qmi_otti_update_time_based_throughput_config( clnt_info, 
                                                     FALSE, 
                                                     TRUE,
                                                     &ps_errno );

   if (return_val != DSS_SUCCESS)
   {
     switch (ps_errno)
     {
       case DS_NOMEMORY:
         resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
         resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
         break;
       default:
         resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
         resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
         break;
     }
     goto send_result;
   }
  }/* end of timer based reporting*/
  
  /*-------------------------------------------------------
  Threshold based reporting
  ---------------------------------------------------------*/
  else
  {
    /*----------------------------------------------------------
    check for on going time based reporting
    -------------------------------------------------------------*/
    if( QMI_OTTI_CURRENT_REPORT_BY_TIME == qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type )
    {
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_INVALID_OPERATION_V01;
      goto send_result;
    }

    /*----------------------------------------------------------
    check for first client on threshold reporting
    -------------------------------------------------------------*/
   if((qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD && 
      (qmi_ott_state.dl_thrput_clnt_info[subscription_id].reporting_clnt != clnt_info->clnt)) ||
       qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_TIME)

    {
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_NOT_SUPPORTED_V01;
      goto send_result;
    }

    /*---------------------------------------------------------------
    RAT must be present if other threshold criteria are present
    --------------------------------------------------------------*/
    if(!req_ptr->rat_type_valid && (req_ptr->dl_throughput_hyst_mag_chng_size_valid || 
        req_ptr->dl_throughput_hyst_time_valid || req_ptr->dl_throughput_threshold_list_valid))
    {
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
    }

    /*----------------------------------------------------------
    Set payload, also store in clnt_info for tracking client request purpose
    -------------------------------------------------------------*/
    memset(&dl_threshold, 0 , sizeof(ps_sys_dl_thrput_thres_criteria_type));

    dl_threshold.is_valid_rat = req_ptr->rat_type_valid;
    if( req_ptr->rat_type_valid )
    {
      switch(req_ptr->rat_type)
      {
        case OTT_RAT_WCDMA_V01: 
          dl_threshold.rat = PS_SYS_RAT_EX_3GPP_WCDMA;
          break;
        case OTT_RAT_LTE_V01: 
          dl_threshold.rat = PS_SYS_RAT_EX_3GPP_LTE;
          break;
        default:
          resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
          resp_msg.resp.error = QMI_ERR_INVALID_ARG_V01;
          goto send_result;
      }
    }
    dl_threshold.is_valid_hyst_time = req_ptr->dl_throughput_hyst_time_valid;
    if( req_ptr->dl_throughput_hyst_time_valid )
    {
      dl_threshold.hyst_time = req_ptr->dl_throughput_hyst_time;
    }
    
    dl_threshold.is_valid_hyst_mag_chng_size = req_ptr->dl_throughput_hyst_mag_chng_size_valid;
    if( req_ptr->dl_throughput_hyst_mag_chng_size_valid )
    {
      dl_threshold.hyst_mag_chng_size = req_ptr->dl_throughput_hyst_mag_chng_size;
    }

    dl_threshold.is_valid_threshold_list = req_ptr->dl_throughput_threshold_list_valid;
    if( req_ptr->dl_throughput_threshold_list_valid )
    {
      dl_threshold.threshold_list_len= req_ptr->dl_throughput_threshold_list_len;
      memscpy(dl_threshold.threshold_list, 
              PS_SYS_THRESHOLD_LIST_LEN * sizeof(dl_threshold.threshold_list[0]),
              req_ptr->dl_throughput_threshold_list, 
              QMI_OTT_UL_THRESHOLD_LIST_LEN_V01 * sizeof(req_ptr->dl_throughput_threshold_list[0]));
    }

    if( req_ptr->dl_threshold_action_valid )
    {
      switch(req_ptr->dl_threshold_action)
      {
        case OTT_THRESHOLD_ACTION_STOP_V01:
         dl_threshold.action = PS_SYS_THRESHOLD_ACTION_STOP;
         break;
        case OTT_THRESHOLD_ACTION_START_V01:
          dl_threshold.action = PS_SYS_THRESHOLD_ACTION_START;
         break;
        default:
          resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
          resp_msg.resp.error = QMI_ERR_INVALID_ARG_V01;
          goto send_result;
      }
    }
    else
    {
      dl_threshold.action = PS_SYS_THRESHOLD_ACTION_INVALID;
    }
    return_val = ps_sys_ioctl_ex(PS_SYS_TECH_3GPP,
                                 PS_SYS_IOCTL_3GPP_SET_DL_THRPUT_THRES_CRITERIA,
                                 (ps_sys_subscription_enum_type) subscription_id,
                                 &dl_threshold,
                                 &err_no);
    if (return_val != DSS_SUCCESS)
    {
      LOG_MSG_ERROR_2("ps_sys_ioctl ret [%d] errno [%d]", 
                     return_val, ps_errno);

     switch (ps_errno)
     {
       case DS_NOMEMORY:
         resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
         resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
         break;
       default:
         resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
         resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
         break;
     }
     goto send_result;
   }

    /*----------------------------------------------------------
    Set first threshold reporting client
    -------------------------------------------------------------*/
    qmi_ott_state.dl_thrput_clnt_info[subscription_id].reporting_clnt = clnt_info->clnt;
    qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type = QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD;

    /*------------------------------------------------------------
      Update QMI cache for the request
    -------------------------------------------------------------*/
    if(req_ptr->rat_type_valid)
    {
      qmi_ott_state.dl_client_request_config[req_ptr->rat_type - 1].rat = dl_threshold.rat;
    
      if(req_ptr->dl_throughput_hyst_time_valid)
      {
        qmi_ott_state.dl_client_request_config[req_ptr->rat_type - 1].hyst_time = dl_threshold.hyst_time;
      }
      if(req_ptr->dl_throughput_hyst_mag_chng_size_valid)
      {
        qmi_ott_state.dl_client_request_config[req_ptr->rat_type - 1].hyst_mag_chng_size = 
            dl_threshold.hyst_mag_chng_size;
      }
      if(req_ptr->dl_throughput_threshold_list_valid)
      {
        qmi_ott_state.dl_client_request_config[req_ptr->rat_type - 1].threshold_list_len = 
          dl_threshold.threshold_list_len;
        memscpy(&qmi_ott_state.dl_client_request_config[req_ptr->rat_type - 1].threshold_list,
                PS_SYS_THRESHOLD_LIST_LEN * sizeof(dl_threshold.threshold_list[0]),
                &dl_threshold.threshold_list, 
                dl_threshold.threshold_list_len * sizeof(dl_threshold.threshold_list[0]));
      }
    }
  }/* end threshold based reporting */

  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;
  rc = QMI_CSI_CB_NO_ERR;

send_result:
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_configure_downlink_throughput_settings_resp_msg_v01));

  return rc;
}/* qmi_otti_configure_downlink_throughupt_info_settings */

/*===========================================================================
  FUNCTION OTTI_DL_THRPT_STATUS_CHANGE_CB()

  DESCRIPTION
    This the callback that we have registered with PS Sys layer for
    downlink throughput status

  PARAMETERS
    tech_type      : technology for which system status has changed
    event_name     : Event name
    subscription_id: Subscription id
    event_info_ptr : System status event info
    user_data_ptr  : Optional user data to be sent.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void otti_dl_thrpt_status_change_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
)
{
  qmi_otti_cmd_buf_type                                * cmd_ptr = NULL;
  ott_downlink_throughput_reporting_status_ind_msg_v01 * dl_thrpt_status;
  ps_sys_thrpt_status_type                             * ps_thrpt_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_4("otti_dl_thrpt_status_change_cb tech_type %d event_name %d"
                  " event_info_ptr 0x%x Subscription id %d",
                  tech_type, event_name, event_info_ptr,subscription_id);
                 
  DS_QMI_OTT_ASSERT(event_info_ptr != NULL);

  ps_thrpt_status = (ps_sys_thrpt_status_type *)event_info_ptr;
  cmd_ptr = (qmi_otti_cmd_buf_type *)qmi_otti_get_cmd_buf(
                      (qmi_otti_cmd_type)QMI_OTTI_CMD_DL_THRPT_STATUS_CHANGE);

  if( cmd_ptr == NULL)
  {
    return;
  }
  
  memset(cmd_ptr,0,sizeof(qmi_otti_cmd_buf_type));  

  dl_thrpt_status = &cmd_ptr->data.dl_thrpt_status;
  dl_thrpt_status->reporting_status_valid = TRUE;
  dl_thrpt_status->reporting_status = (ott_thrpt_status_reason_enum_v01)ps_thrpt_status->thrpt_status;
  dl_thrpt_status->actual_interval_valid = TRUE;
  dl_thrpt_status->actual_interval = ps_thrpt_status->actual_interval;

  cmd_ptr->cmd_id = QMI_OTTI_CMD_DL_THRPT_STATUS_CHANGE;
  cmd_ptr->subscription_id = subscription_id;

  dcc_send_cmd_ex (DCC_OTT_SVC_CMD, cmd_ptr);
} /* otti_dl_thrpt_status_change_cb() */

/*===========================================================================
FUNCTION  QMI_WDSI_GENERATE_DL_THRPT_STATUS_CHANGE_IND

DESCRIPTION
  This callback is called to notify clients about dl thrpt status changes

PARAMETERS
  thrpt_status - Thrpt status information
  subcription_id - Subscription identifier

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void qmi_otti_generate_dl_thrpt_status_ind
(
  ott_downlink_throughput_reporting_status_ind_msg_v01   * thrpt_status,
  ps_sys_subscription_enum_type                         subscription_id
)
{
  qmi_otti_client_info_type                   * cl_sp;
  q_type                                      * client_q_ptr = NULL;
  void                                        * client_buf_ptr;
  void                                        * next_client_buf_ptr;
  qmi_otti_state_info_type                    * ott_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(thrpt_status == NULL)
  {
    DS_QMI_OTT_ASSERT(0);
  }

  ott_sp         = &qmi_ott_state;
  client_q_ptr   = &ott_sp->client_q;
  client_buf_ptr = q_check(client_q_ptr);
  
  while(client_buf_ptr != NULL)
  {
    cl_sp = (qmi_otti_client_info_type *) client_buf_ptr;
        
    if ( (TRUE == cl_sp->report_status.report_dl_thrpt_reporting_status_change) &&
         (TRUE == qmi_otti_verify_client_subs(cl_sp, 
                                              (uint32)subscription_id,
                                              FALSE)) )
    {
        qmi_otti_dl_thrpt_status_ind(cl_sp, thrpt_status);
    }
      
    next_client_buf_ptr = 
      q_next(client_q_ptr, &((q_generic_item_type *) client_buf_ptr)->link);
    client_buf_ptr = next_client_buf_ptr;
  }
} /* qmi_otti_generate_dl_thrpt_status_ind */

/*===========================================================================
  FUNCTION QMI_OTTI_DL_THRPT_STATUS_IND()

  DESCRIPTION
    This function sends dl thrpt status indication to registered control point.

  PARAMETERS
    client          : ptr to client info
    thrpt_status    : thrpt status

  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_otti_dl_thrpt_status_ind
(
  void                                                    * client,
  ott_downlink_throughput_reporting_status_ind_msg_v01    * thrpt_status
)
{
  qmi_otti_client_info_type               * client_info;
  qmi_csi_error                             rc;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  rc = QMI_CSI_NO_ERR;
  client_info = (qmi_otti_client_info_type *) client;
  if (client_info == NULL)
  {
    DS_QMI_OTT_ASSERT(0);
    return;
  }
            
  rc = qmi_csi_send_ind(client_info->clnt, 
                        QMI_OTT_DOWNLINK_THROUGHPUT_REPORTING_STATUS_IND_V01,
                        thrpt_status, 
                        sizeof(ott_downlink_throughput_reporting_status_ind_msg_v01));
  
  if ( rc != QMI_CSI_NO_ERR )
  {
    LOG_MSG_ERROR_1("qmi_otti_dl_thrpt_status_ind error %d", rc);
  }
} /* qmi_otti_dl_thrpt_status_ind() */

/*===========================================================================
  FUNCTION OTTI_DL_THRPT_THRESHOLD_STATUS_CHANGE_CB()

  DESCRIPTION
    This the callback that we have registered with PS Sys layer for
    downlink throughput threshold status

  PARAMETERS
    tech_type      : technology
    event_name     : Event name
    subscription_id: Subscription id
    event_info_ptr : Downlink threshold status info
    user_data_ptr  : Optional user data to be sent.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void otti_dl_thrpt_threshold_status_change_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
)
{
  qmi_otti_cmd_buf_type                                * cmd_ptr = NULL;
  ott_downlink_throughput_reporting_status_ind_msg_v01 * dl_thrpt_status;
  ps_sys_dl_thrput_actual_thres_criteria_type          * ps_thrpt_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_OTT_ASSERT(event_info_ptr != NULL);

  ps_thrpt_status = (ps_sys_dl_thrput_actual_thres_criteria_type *)event_info_ptr;
  cmd_ptr = (qmi_otti_cmd_buf_type *)qmi_otti_get_cmd_buf(
                      (qmi_otti_cmd_type)QMI_OTTI_CMD_DL_THRPT_STATUS_CHANGE);

  if( cmd_ptr == NULL)
  {
    return;
  }
  
  memset(cmd_ptr,0,sizeof(qmi_otti_cmd_buf_type));  

  dl_thrpt_status = &cmd_ptr->data.dl_thrpt_status;
  dl_thrpt_status->reporting_status_valid = TRUE;
  dl_thrpt_status->reporting_status =
     (ott_thrpt_status_reason_enum_v01)ps_thrpt_status->thrput_status;
  if(ps_thrpt_status->thrput_status == PS_SYS_THRPT_REPORT_ENABLED)
  {
  dl_thrpt_status->rat_type_valid= TRUE;
  dl_thrpt_status->actual_dl_throughput_hyst_time_valid = TRUE;
  dl_thrpt_status->actual_dl_throughput_hyst_mag_chng_size_valid = TRUE;
  dl_thrpt_status->actual_dl_throughput_threshold_list_valid = TRUE;

  switch(ps_thrpt_status->rat)
  {
    case(PS_SYS_RAT_EX_3GPP_WCDMA):
      dl_thrpt_status->rat_type = OTT_RAT_WCDMA_V01;
      break;
    case(PS_SYS_RAT_EX_3GPP_LTE):
      dl_thrpt_status->rat_type = OTT_RAT_LTE_V01;
      break;
    default:
      LOG_MSG_ERROR_1("otti_dl_thrpt_threshold_status_change_cb invalid RAT type %d",
                       ps_thrpt_status->rat);
      return;
  }

  dl_thrpt_status->actual_dl_throughput_hyst_time = ps_thrpt_status->hyst_time;
  dl_thrpt_status->actual_dl_throughput_hyst_mag_chng_size = 
    ps_thrpt_status->hyst_mag_chng_size;
  dl_thrpt_status->actual_dl_throughput_threshold_list_len = ps_thrpt_status->threshold_list_len;
    memscpy(dl_thrpt_status->actual_dl_throughput_threshold_list,
            QMI_OTT_UL_THRESHOLD_LIST_LEN_V01 * sizeof(dl_thrpt_status->actual_dl_throughput_threshold_list[0]),
            ps_thrpt_status->threshold_list,
            ps_thrpt_status->threshold_list_len * sizeof(ps_thrpt_status->threshold_list[0]));
  }
  cmd_ptr->cmd_id = QMI_OTTI_CMD_DL_THRPT_STATUS_CHANGE;
  cmd_ptr->subscription_id = subscription_id;

  dcc_send_cmd_ex (DCC_OTT_SVC_CMD, cmd_ptr);
} /* otti_dl_thrpt_threshold_status_change_cb() */

/*===========================================================================
  FUNCTION QMI_OTTI_QUERY_DOWNLINK_THRT_REPORTING_STATUS()

  DESCRIPTION
    Queries downlink throughput reporting status

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error   qmi_otti_query_downlink_throughput_reporting_status
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
)
{
  ott_query_downlink_throughput_reporting_status_resp_msg_v01  resp_msg;
  int16                                 return_val = DSS_SUCCESS;
  qmi_error_type_v01                    errval;
  ps_sys_thrpt_status_type              report_status;
  uint32                                subscription_id = 0;
  int16                                 ps_errno = 0;
  ps_sys_ul_thrput_actual_thres_criteria_type thres_report_status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && service_handle);

  errval = QMI_ERR_NONE_V01;
  memset(&resp_msg,0,sizeof(resp_msg));
  memset(&report_status, 0, sizeof(ps_sys_thrpt_status_type));
    
  subscription_id = qmi_otti_resolve_client_subs(clnt_info);
  
  if(subscription_id < QMI_OTTI_SUBS_MAX &&
     qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_TIME)
  {
  /*-------------------------------------------------------------------------
    Retrieve dl thrpt status information by calling into PS Sys Layer
  -------------------------------------------------------------------------*/
  return_val = ps_sys_conf_get_ex(PS_SYS_TECH_ALL,
                                  PS_SYS_CONF_DL_THROUGHPUT_INTERVAL_STATUS_CHANGE,
                                  (ps_sys_subscription_enum_type) subscription_id,
                                  (void *)&report_status,
                                  &ps_errno);

  if (return_val != DSS_SUCCESS)
  {
    LOG_MSG_ERROR_INT_2("ps_sys_conf_get_ex ret [d] errno [d]", return_val, ps_errno);
    switch (ps_errno)
    {
      case DS_NOMEMORY:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
        break;
      default:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
        break;
    }
    goto send_result;
  }

  resp_msg.reporting_status_valid = TRUE;
  resp_msg.reporting_status = (ott_thrpt_status_reason_enum_v01)report_status.thrpt_status;
  
  resp_msg.actual_interval_valid = TRUE;
  resp_msg.actual_interval = report_status.actual_interval;

}/*end time based */

else if(subscription_id < QMI_OTTI_SUBS_MAX && 
        qmi_ott_state.dl_thrput_clnt_info[subscription_id].report_type == QMI_OTTI_CURRENT_REPORT_BY_THRESHOLD)
{
  return_val = ps_sys_conf_get_ex(PS_SYS_TECH_ALL,
                                  PS_SYS_CONF_DL_THRPUT_ACTUAL_THRES_CRITERIA,
                                  (ps_sys_subscription_enum_type) subscription_id,
                                  (void *)&thres_report_status,
                                  &ps_errno);
  if (return_val != DSS_SUCCESS)
  {
    LOG_MSG_ERROR_2("ps_sys_conf_get_ex ret [%d] errno [%d]", return_val, ps_errno);
    switch (ps_errno)
    {
      case DS_NOMEMORY:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
        break;
      default:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
        break;
    }
    goto send_result;
  }
  resp_msg.reporting_status_valid = TRUE;
  resp_msg.reporting_status = 
    (ott_thrpt_status_reason_enum_v01)thres_report_status.thrput_status;
  resp_msg.rat_type_valid = TRUE;
  switch(thres_report_status.rat)
  {
    case(PS_SYS_RAT_EX_3GPP_WCDMA):
      resp_msg.rat_type = OTT_RAT_WCDMA_V01;
      break;
    case(PS_SYS_RAT_EX_3GPP_LTE):
      resp_msg.rat_type = OTT_RAT_LTE_V01;
      break;
    default:
      resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
      resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
      goto send_result;
  }
  resp_msg.actual_dl_throughput_hyst_mag_chng_size_valid = TRUE;
  resp_msg.actual_dl_throughput_hyst_mag_chng_size = thres_report_status.hyst_mag_chng_size;
  resp_msg.actual_dl_throughput_hyst_time_valid = TRUE;
  resp_msg.actual_dl_throughput_hyst_time = thres_report_status.hyst_time;
  resp_msg.actual_dl_throughput_threshold_list_valid = TRUE;
  resp_msg.actual_dl_throughput_threshold_list_len = thres_report_status.threshold_list_len;
  memscpy(resp_msg.actual_dl_throughput_threshold_list, 
          QMI_OTT_UL_THRESHOLD_LIST_LEN_V01 * sizeof(resp_msg.actual_dl_throughput_threshold_list[0]),
          thres_report_status.threshold_list, 
          thres_report_status.threshold_list_len * sizeof(thres_report_status.threshold_list[0]));
}

resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
resp_msg.resp.error = QMI_ERR_NONE_V01;

send_result:
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_query_downlink_throughput_reporting_status_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

}/* qmi_otti_query_downlink_throughput_reporting_status */

/*===========================================================================
  FUNCTION OTTI_DL_THRPT_INFO_CB()

  DESCRIPTION
    This the callback that we have registered with PS Sys layer for
    downlink throughput info

  PARAMETERS
    tech_type      : technology for which system status has changed
    event_name     : Event name
    subscription_id: Subscription id
    event_info_ptr : System status event info
    user_data_ptr  : Optional user data to be sent.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void otti_dl_thrpt_info_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,  
  void                                         * user_data_ptr
)
{
  qmi_otti_cmd_buf_type                     * cmd_ptr = NULL;
  ott_downlink_throughput_info_ind_msg_v01  * dl_thrpt_info;
  ps_sys_dl_throughput_info_type            * ps_thrpt_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_4("otti_dl_thrpt_info_cb tech_type %d event_name %d"
                  " event_info_ptr 0x%x Subscription id %d",
                  tech_type, event_name, event_info_ptr,subscription_id);
                 
  DS_QMI_OTT_ASSERT(event_info_ptr != NULL);

  ps_thrpt_info = (ps_sys_dl_throughput_info_type *)event_info_ptr;
  cmd_ptr = (qmi_otti_cmd_buf_type *)qmi_otti_get_cmd_buf(
                      (qmi_otti_cmd_type)QMI_OTTI_CMD_DL_THRPT_INFO);

  if( cmd_ptr == NULL)
  {
    return;
  }
  
  memset(cmd_ptr,0,sizeof(qmi_otti_cmd_buf_type));  

  dl_thrpt_info = &cmd_ptr->data.dl_thrpt_info;
  dl_thrpt_info->downlink_rate_valid = TRUE;
  dl_thrpt_info->downlink_rate = ps_thrpt_info->downlink_allowed_rate;

  dl_thrpt_info->confidence_level_valid = TRUE;
  dl_thrpt_info->confidence_level = ps_thrpt_info->confidence_level;

  cmd_ptr->cmd_id = QMI_OTTI_CMD_DL_THRPT_INFO;
  cmd_ptr->subscription_id = subscription_id;

  dcc_send_cmd_ex (DCC_OTT_SVC_CMD, cmd_ptr);
} /* otti_dl_thrpt_info_cb() */

/*===========================================================================
FUNCTION  QMI_WDSI_GENERATE_DL_THRPT_INFO_IND

DESCRIPTION
  This function generated downlink throughput info indications

PARAMETERS
  thrpt_status - Thrpt status information
  subscription_id - subscription identifier

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void qmi_otti_generate_dl_thrpt_info_ind
(
  ott_downlink_throughput_info_ind_msg_v01   * thrpt_info,
  ps_sys_subscription_enum_type              subscription_id
)
{
  qmi_otti_client_info_type                   * cl_sp;
  q_type                                      * client_q_ptr = NULL;
  void                                        * client_buf_ptr;
  void                                        * next_client_buf_ptr;
  qmi_otti_state_info_type                    * ott_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(thrpt_info == NULL)
  {
    DS_QMI_OTT_ASSERT(0);
  }

  ott_sp         = &qmi_ott_state;
  client_q_ptr   = &ott_sp->client_q;
  client_buf_ptr = q_check(client_q_ptr);
  
  while(client_buf_ptr != NULL)
  {
    cl_sp = (qmi_otti_client_info_type *) client_buf_ptr;
        
    if ((TRUE == cl_sp->report_status.report_dl_thrpt_info) &&
        (TRUE == qmi_otti_verify_client_subs(cl_sp, 
                                             (uint32)subscription_id,
                                              FALSE)) )
    {
        qmi_otti_dl_thrpt_info_ind(cl_sp, thrpt_info);
    }
      
    next_client_buf_ptr = 
      q_next(client_q_ptr, &((q_generic_item_type *) client_buf_ptr)->link);
    client_buf_ptr = next_client_buf_ptr;
  }
} /* qmi_otti_generate_dl_thrpt_info_ind */

/*===========================================================================
  FUNCTION QMI_OTTI_THRPT_INFO_IND()

  DESCRIPTION
    This function sends thrpt info indication to registered control point.

  PARAMETERS
    clid          : clientID
    thrpt_info    : thrpt info

  RETURN VALUE
    void

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_otti_dl_thrpt_info_ind
(
  void                                     * client,
  ott_downlink_throughput_info_ind_msg_v01   * thrpt_info
)
{
  qmi_otti_client_info_type               * client_info;
  qmi_csi_error                             rc;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  rc = QMI_CSI_NO_ERR;
  client_info = (qmi_otti_client_info_type *) client;
  if (client_info == NULL)
  {
    DS_QMI_OTT_ASSERT(0);
    return;
  }
            
  rc = qmi_csi_send_ind(client_info->clnt, 
                        QMI_OTT_DOWNLINK_THROUGHPUT_INFO_IND_V01,
                        thrpt_info, 
                        sizeof(ott_downlink_throughput_info_ind_msg_v01));
  
  if ( rc != QMI_CSI_NO_ERR )
  {
    LOG_MSG_ERROR_INT_1("qmi_otti_dl_thrpt_info_ind error d", rc);
  }
} /* qmi_otti_dl_thrpt_info_ind() */

/*===========================================================================
  FUNCTION QMI_OTTI_QUERY_DOWNLINK_THROUGHPUT_INFO()

  DESCRIPTION
    Queries downlink throughput information

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error   qmi_otti_query_downlink_throughput_info
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
)
{
  ott_get_downlink_throughput_info_resp_msg_v01  resp_msg;
  int16                                 return_val = DSS_SUCCESS;
  qmi_error_type_v01                    errval;
  ps_sys_dl_throughput_info_type        thrpt_info;
  uint32                                subscription_id = 0;
  int16                                 ps_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && service_handle);

  errval = QMI_ERR_NONE_V01;
  memset(&resp_msg,0,sizeof(resp_msg));
  memset(&thrpt_info, 0, sizeof(ps_sys_dl_throughput_info_type));
    
  subscription_id = qmi_otti_resolve_client_subs(clnt_info);

  /*-------------------------------------------------------------------------
    Retrieve dl thrpt status information by calling into PS Sys Layer
  -------------------------------------------------------------------------*/
  return_val = ps_sys_conf_get_ex(PS_SYS_TECH_ALL,
                                  PS_SYS_CONF_DL_THROUGHPUT_INFO,
                                  (ps_sys_subscription_enum_type) subscription_id,
                                  (void *)&thrpt_info,
                                  &ps_errno);

  if (return_val != DSS_SUCCESS)
  {
    LOG_MSG_ERROR_INT_2("ps_sys_conf_get_ex ret [d] errno [d]", return_val, ps_errno);
    switch (ps_errno)
    {
      case DS_NOMEMORY:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_NO_MEMORY_V01;
        break;
      default:
        resp_msg.resp.result = QMI_RESULT_FAILURE_V01;
        resp_msg.resp.error = QMI_ERR_INTERNAL_V01;
        break;
    }
    goto send_result;
  }

  resp_msg.downlink_rate_valid = TRUE;
  resp_msg.downlink_rate       = thrpt_info.downlink_allowed_rate;
  
  resp_msg.confidence_level_valid = TRUE;
  resp_msg.confidence_level       = thrpt_info.confidence_level;

  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;

send_result:
  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_get_downlink_throughput_info_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;

}/* qmi_otti_query_downlink_throughput_info */

/*===========================================================================
  FUNCTION QMI_OTTI_GET_CAPABILITIES_REQ()

  DESCRIPTION
    Queries OTT capabilities

  PARAMETERS
    clnt_info     : client state pointer
    req_handle    : Request message handle
    msg_id        : Message ID
    req_c_struct  : Request structure
    req_c_struct_len : Length of request
    service_handle : Service state pointer

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_csi_cb_error   qmi_otti_get_capabilities
(
  qmi_otti_client_info_type *clnt_info,
  qmi_req_handle             req_handle,
  int                        msg_id,
  void                      *req_c_struct,
  int                        req_c_struct_len,
  void                      *service_handle
)
{
  ott_get_capabilities_req_msg_v01     *req_ptr;
  ott_get_capabilities_resp_msg_v01     resp_msg;
  qmi_error_type_v01                    errval;
  ps_sys_dl_throughput_info_type        thrpt_info;
  uint32                                subscription_id = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_OTT_ASSERT(req_c_struct && (req_c_struct_len > 0) && clnt_info && service_handle);

  req_ptr = 
    (ott_get_capabilities_req_msg_v01 *)req_c_struct;
  subscription_id = qmi_otti_resolve_client_subs(clnt_info);

  errval = QMI_ERR_NONE_V01;
  memset(&resp_msg,0,sizeof(resp_msg));
  memset(&thrpt_info, 0, sizeof(ps_sys_dl_throughput_info_type));
    
  /*-------------------------------------------------------------------------
    Retrieve dl thrpt status information by calling into PS Sys Layer
  -------------------------------------------------------------------------*/
  if(req_ptr->req_capability_mask & OTTI_MASK_UL_THROUGHPUT_REPORT)
  {
    resp_msg.ul_capability_valid = TRUE;
    resp_msg.ul_capability = OTT_UL_THROUGHPUT_REPORT_BY_THRESHOLD_V01;
  }
  if(req_ptr->req_capability_mask & OTTI_MASK_DL_THROUGHPUT_REPORT)
  {
    resp_msg.dl_capability_valid = TRUE;
    resp_msg.dl_capability = OTT_DL_THROUGHPUT_REPORT_BY_THRESHOLD_V01;
  }

  resp_msg.resp.result = QMI_RESULT_SUCCESS_V01;
  resp_msg.resp.error = QMI_ERR_NONE_V01;

  qmi_csi_send_resp(req_handle, msg_id, &resp_msg, 
                    sizeof(ott_get_capabilities_resp_msg_v01));
  return QMI_CSI_CB_NO_ERR;
}

