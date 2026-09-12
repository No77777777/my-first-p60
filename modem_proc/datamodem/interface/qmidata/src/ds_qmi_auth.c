/*===========================================================================

                         D S _ Q M I _ A U T H . C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

EXTERNALIZED FUNCTIONS

  qmi_auth_init()
    Register the AUTH service with QMUX for all applicable QMI links
    

Copyright (c) 2004-2015,2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/src/ds_qmi_auth.c#3 $ $DateTime: 2020/04/24 05:33:29 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/12/13    vrk    Corrected variable intialization in bind subscription
12/07/13    vrk    Added EAP Notificaton code Indication support
07/20/13    gk     Corrected method_mask TLV validation
07/08/13    gk     Modified the subscription validation properly
06/11/13    gk     Fixed KW errors 
06/06/13    gk     Added support for subscription binding and multisim support
08/27/12    gk     Added support for EAP-AKA-PRIME method in start_eap_session_message
08/06/12    gk     Added support for QMI service to log what messages/TLVs  
                   are supported on this branch.
07/26/12    sb     Fixed KW Errors.
06/04/12    wc     Fix futex leaking in client deallocation
04/25/11    sa     Fix to avoid crash because of incompatible auth command id type.
12/17/10    kk     Removing DCC command buffer dependency from QMI services.
11/08/10    mct    Support for AKAv2 QMI messages.
02/19/09    am     DS Task De-coupling effort and introduction of DCC task.
09/12/08    ks     Created module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include <stringl/stringl.h>
#include "ps_sys.h"
#include "ps_sys_conf.h"
#include "ps_sys_event.h"
#include "ps_sys_ioctl.h"

#ifdef FEATURE_DATA_QMI_EAP
#include <stdio.h>
#include <string.h>

#include "dsm.h"
#include "nv.h"
#include "msg.h"
#include "err.h"
#include "amssassert.h"
#include "ds_qmi_defs.h"
#include "ds_qmi_svc.h"
#include "ds_qmi_task.h"
#include "ds_qmux_ext.h"
#include "ds_qmux.h"
#include "ds_qmi_svc_ext.h"
#include "ps_eap.h"
#include "dcc_task_defs.h"
#include "dcc_task_svc.h"

#include "ps_eap_sim.h"
#include "ps_eap_aka.h"
#include "ps_eap_aka_prime.h"
#include "ps_eap_sim_aka.h"
#include "ps_eapi.h"
#include "ps_utils_aka.h"
#include "ps_eap_client_config.h"

#ifdef FEATURE_MMGSDI_SESSION_LIB
  #include "mmgsdilib_common.h"
#endif /* FEATURE_MMGSDI_SESSION_LIB */

#include "qmi_svc_defs.h"
#include "qmi_svc_utils.h"
#include "qmi_framework.h"
#include "ds_qmi_auth.h"
#include "ds_qmi_fw_common.h"
#include "authentication_service_v01.h"

#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "qmi_si.h"
#include "qmi_idl_lib.h"
#include "ps_utils.h"

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Service management
---------------------------------------------------------------------------*/
#define AUTHI_MAX_CLIDS    (QMI_FRAMEWORK_SVC_MAX_CLIENTS - 1)
#define AUTHI_INVALID_INSTANCE    -1

/*---------------------------------------------------------------------------
  Major and Minor Version Nos for AUTH
---------------------------------------------------------------------------*/
#define AUTHI_BASE_VER_MAJOR    (1)
#define AUTHI_BASE_VER_MINOR    (3)

#define AUTHI_ADDENDUM_VER_MAJOR  (0)
#define AUTHI_ADDENDUM_VER_MINOR  (0)

/*---------------------------------------------------------------------------
  QMI_AUTH_START_EAP_SESSION TLV Message IDs
---------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_EAP_METHOD_MASK   (0x10)
#define AUTHI_PRM_TYPE_EAP_USER_ID       (0x11)
#define AUTHI_PRM_TYPE_EAP_META_IDENTITY (0x12)
#define AUTHI_PRM_TYPE_EAP_SIM_AKA_ALGO  (0x13)

/*---------------------------------------------------------------------------
  QMI_AUTH_SEND_IMSI_PUBLIC_KEY TLV Message IDs
---------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_EAP_KEY_ID       (0x10)
#define AUTHI_PRM_TYPE_MCC_MNC          (0x11)
#define AUTHI_PRM_TYPE_EXPIRY_TIME      (0x12)
/*---------------------------------------------------------------------------
  AKA Run Algorithm Parameters
-----------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_AKA_PARAMS        (0x10)

/*---------------------------------------------------------------------------
  AKA Algorithm Result Data
-----------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_AKA_RESULT_DATA        (0x10)

/*---------------------------------------------------------------------------
  bind_subscription TLV definitions
---------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_BIND_SUBSCRIPTION (0x10)

/*---------------------------------------------------------------------------
  Indication_register TLV definitions
---------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_EAP_NOTIFICATION_CODE (0x10)
#define AUTHI_PRM_TYPE_EAP_CLIENT_ERR_CODE (0x11)
#define AUTHI_PRM_TYPE_EAP_AUTH_REJ (0x12)
#define AUTHI_PRM_TYPE_EAP_IMSI_PUBLIC_KEY_REQ (0x13)
#define AUTHI_PRM_TYPE_RETR_CA_CERTIFICATE_REQ (0x14)

/*---------------------------------------------------------------------------
  QMI_AUTH_START_EAP_SESSION TLV Message IDs
---------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_EAP_AUTH_CRED_MSK   (0x10)
#define AUTHI_PRM_TYPE_EAP_AUTH_CRED_EMSK   (0x11)
#define AUTHI_PRM_TYPE_EAP_AUTH_CRED_SESSION_ID   (0x12)

/*---------------------------------------------------------------------------
  QMI_AUTH_RETRIEVE_CA_CERTIFICATE_IND TLV Message IDs
-----------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_CA_CERT_DN        (0x02)

/*---------------------------------------------------------------------------
  QMI_AUTH_SEND_CA_CERTIFICATE TLV Message IDs
-----------------------------------------------------------------------------*/
#define AUTHI_PRM_TYPE_NUM_CERTIFICATES        (0x10)
#define AUTHI_PRM_TYPE_CERT_INDEX              (0x11)
#define AUTHI_PRM_TYPE_CA_CERTIFICATE          (0x12)
#define AUTHI_PRM_TYPE_NUM_SEGMENTS            (0x13)
#define AUTHI_PRM_TYPE_SEQUENCE_NUM            (0x14)
#define AUTHI_PRM_TYPE_FAILURE_REASON          (0x15)


/*---------------------------------------------------------------------------
  Control Cap BIT MASK
---------------------------------------------------------------------------*/
typedef enum
{
  AUTHI_EAP_METHOD_NONE = 0x0,
  AUTHI_EAP_METHOD_SIM  = 0x1,
  AUTHI_EAP_METHOD_AKA       = 0x2,
  AUTHI_EAP_METHOD_AKA_PRIME = 0x4
} authi_control_cap_e_type;

typedef enum
//EAP Authentication Credential Mask
{
  AUTHI_EAP_CRED_MSK         = 0x1,
  /** EAP Master Session Key */
  AUTHI_EAP_CRED_EXT_MSK     = 0x2,
  /** EAP Extended Master Session Key*/
  AUTHI_EAP_CRED_SESSION_ID  = 0x4
  /** Session ID */
} auth_eap_cred_e_type;

/*---------------------------------------------------------------------------
  Macro used in command handlers (common)
---------------------------------------------------------------------------*/
#define CHECK_RETVAL()  if (FALSE == retval) { dsm_free_packet(&response); \
                                               return NULL; }

#define DS_QMI_AUTH_ASSERT(expression) \
         ds_qmi_auth_assert_wrapper(__LINE__, expression)


/*===========================================================================

                                DATA TYPES

===========================================================================*/


/*---------------------------------------------------------------------------
  AUTH Command enum type - not equal to the actual command values!
  mapping is in qmi_auth_cmd_callbacks table

  DO NOT REORDER THIS ENUM!
---------------------------------------------------------------------------*/
typedef enum
{
  AUTHI_CMD_MIN                          = 0,
  AUTHI_CMD_RESET                        = AUTHI_CMD_MIN,
  AUTHI_CMD_START_EAP_SESSION            ,
  AUTHI_CMD_SEND_EAP_PACKET              ,
  AUTHI_CMD_GET_EAP_SESSION_KEYS         ,
  AUTHI_CMD_END_EAP_SESSION              ,
  AUTHI_CMD_RUN_AKA_ALGO                 ,
  AUTHI_CMD_SET_SUBSCRIPTION_BINDING     ,
  AUTHI_CMD_GET_BIND_SUBSCRIPTION        ,
  AUTHI_CMD_INDICATION_REGISTER          ,
  AUTHI_CMD_SEND_EAP_PACKET_EXT          ,
  AUTHI_CMD_EAP_GET_AUTH_CREDS           ,
  AUTHI_CMD_SEND_IMSI_PUBLIC_KEY         ,
  AUTHI_CMD_RETR_CA_CERT_IND             ,
  AUTHI_CMD_MAX, 
  AUTHI_CMD_WIDTH                    = 0xFFFF                        
} qmi_authi_cmd_e_type;

typedef enum
{
  AUTHI_CMD_VAL_RESET                         = 0x0000,
  AUTHI_CMD_VAL_INDICATION_REGISTER           = 0x0003,
  AUTHI_CMD_VAL_START_EAP_SESSION             = 0x0020,
  AUTHI_CMD_VAL_SEND_EAP_PACKET               = 0x0021,
  AUTHI_CMD_VAL_EAP_SESSION_RESULT_IND        = 0x0022,
  AUTHI_CMD_VAL_GET_EAP_SESSION_KEYS          = 0x0023,
  AUTHI_CMD_VAL_END_EAP_SESSION               = 0x0024,
  AUTHI_CMD_VAL_RUN_AKA_ALGO                  = 0x0025,
  AUTHI_CMD_VAL_AKA_ALGO_RESULT_IND           = 0x0026,
  AUTHI_CMD_VAL_SET_SUBSCRIPTION_BINDING      = 0x0027,
  AUTHI_CMD_VAL_GET_BIND_SUBSCRIPTION         = 0x0028,
  AUTHI_CMD_VAL_EAP_NOTIFICATION_CODE_IND     = 0x0029,
  AUTHI_CMD_VAL_EAP_CLIENT_ERR_IND            = 0x002A,
  AUTHI_CMD_VAL_EAP_AUTH_REJ_IND              = 0x002B,
  AUTHI_CMD_VAL_SEND_EAP_PACKET_EXT           = 0x002C,
  AUTHI_CMD_VAL_EAP_GET_AUTH_CREDS            = 0x002D,
  AUTHI_CMD_VAL_SEND_IMSI_PUBLIC_KEY          = 0x002E,
  AUTHI_CMD_VAL_SEND_IMSI_PUBLIC_KEY_REFRESH_IND = 0x002F,
  AUTHI_CMD_VAL_RETR_CA_CERT_IND              = 0x0030,
  AUTHI_CMD_VAL_SEND_CA_CERT_RESULT           = 0x0031,

/* QC EXTERNAL QMI COMMAND RANGE IS 0x0000 - 0x5555. 
   Add the next external QMI Command here */

/* VENDOR SPECIFIC QMI COMMAND RANGE IS 0x5556 - 0xAAAA.
   IMPORTANT!
   Add the vendor specific QMI Commands within this range only to avoid 
   conflicts with QC QMI commands that would get released in future */

/* RESERVED QC QMI COMMAND RANGE IS 0xAAAB - 0xFFFE */
  AUTHI_CMD_VAL_WIDTH                      = 0xFFFF
} qmi_authi_cmd_val_e_type;

/*---------------------------------------------------------------------------
  AUTH client sunscription preference enum
---------------------------------------------------------------------------*/
typedef enum
{
  AUTHI_BIND_SUBS_PRIMARY   = 0x01,
  AUTHI_BIND_SUBS_SECONDARY = 0x02,
  AUTHI_BIND_SUBS_TERTIARY  = 0x03,
  AUTHI_BIND_SUBS_MAX
} qmi_authi_client_subs_e_type;
/*---------------------------------------------------------------------------
  QMI AUTH instance state definition &
  AUTH client state definition
---------------------------------------------------------------------------*/
#define QMI_AUTH_MAX_AKA_RAND_LEN_SIZE   255
#define QMI_AUTH_MAX_AKA_AUTN_LEN_SIZE   255
#define QMI_AUTH_MAX_AKA_DIGEST_LEN_SIZE 255
#define QMI_AUTH_MAX_AKA_DATA_LEN_SIZE   255
#define QMI_EAP_MAX_PRE_MASTER_KEY_SIZE  64
#define QMI_EAP_MAX_EXT_MASTER_KEY_SIZE  64
#define QMI_AUTH_MAX_MNC_MCC_SIZE        4
typedef struct 
{
  qmi_common_client_state_type common; // must be first since we alias
  int16     instance;  
  int16     service_id;  
  struct
  {
    eap_handle_type              eap_handle;
    qmi_cmd_buf_type           * pending_req_cmd_buf;
    eap_result_enum_type         result;
    uint8                        msk[QMI_EAP_MAX_PRE_MASTER_KEY_SIZE];
    uint16                       msk_len;
    /*Extended Master Session key */
    uint16                       emsk_len;
    uint8                        emsk[QMI_EAP_MAX_EXT_MASTER_KEY_SIZE];
   /*Session ID */
    uint16                       session_id_len;
    uint8                        session_id[EAP_SIM_AKA_MAX_SESSION_ID];
    boolean                      eap_result_ready;
  }eap_info;

  qmi_authi_client_subs_e_type subscription_id;
  struct 
  {
    boolean  report_eap_notification_code;
    boolean  report_eap_err_ind_code;
    boolean  report_eap_auth_rej;
    boolean  report_imsi_public_key_req;
    boolean  report_retr_ca_cert_req;
  } report_status;

  q_type ca_cert_tx_q;

} qmi_authi_client_state_type;

/*---------------------------------------------------------------------------
  Watermark queue to hold deferred responses
---------------------------------------------------------------------------*/
typedef qmi_authi_client_state_type* qmi_authi_client_state_ptr_type; 

typedef struct
{
  uint16                           num_qmi_instances;  
  qmi_authi_client_state_ptr_type  client[AUTHI_MAX_CLIDS];
} qmi_authi_state_type;

typedef struct
{
  uint16 cmd_id;
  union
  {
    struct
    {
      uint16    num_instances;
    } init_cb;

    struct
    {
      qmi_framework_common_msg_hdr_type    common_msg_hdr;
    } clid_cb;

    struct
    {
      qmi_framework_msg_hdr_type    msg_hdr;
      dsm_item_type             *   sdu_in;
    } cmd_hdlr_cb;

    struct
    {
      eap_sim_aka_task_srv_req_cb_type req_cb;
      void *                           user_data;
    } eap_sim_aka;

    struct
    {
      void           * user_data; 
      dsm_item_type  * pkt;
    } eap_resp;
    struct
    {
      eap_handle_type    eap_handle; 
      eap_sim_aka_indication_enum_type ind_type;
      eap_sim_aka_ind_union_type  value;
    } eap_indication_cb;
    struct
    {
      ps_sys_event_imsi_public_key_action_type action;
      uint32 subscription_id;
    }eap_imsi_public_key_info_cb;
    struct{
      ps_sys_event_get_ca_certificate get_ca_cert;
      uint32 subscription_id;
    }retr_ca_cert_cb;
  } data;
} qmi_authi_cmd_buf_type;

typedef struct
{
  uint16                cmd_id;
  uint8                 pmk[QMI_EAP_MAX_PRE_MASTER_KEY_SIZE];
  uint16                pmk_len;
  uint8                 emsk[QMI_EAP_MAX_EXT_MASTER_KEY_SIZE];
  uint16                emsk_len;
  uint8                 session_id[EAP_SIM_AKA_MAX_SESSION_ID];
  uint16                session_id_len;
  eap_handle_type       handle;
  eap_result_enum_type  result;
  void *                user_data;
} qmi_authi_eap_result_cmd_buf_type;

typedef struct
{
  uint16                         cmd_id;
  ps_utils_aka_handle_type       handle;
  ps_utils_aka_status_enum_type  status;
  uint8                          digest[QMI_AUTH_MAX_AKA_DIGEST_LEN_SIZE];
  uint8                          digest_len;
  uint8                          aka_data[QMI_AUTH_MAX_AKA_DATA_LEN_SIZE];
  uint8                          aka_data_len;
  void                          *user_data;
} qmi_authi_aka_result_cmd_buf_type;

typedef struct
{
  uint16   cmd_id;
  uint32   tx_id;
  uint16   num_certificates;
  uint16   cert_index;
  dsm_item_type   *ca_cert_payload;
  uint16    ca_cert_len;
  uint8    num_segments;
  uint8    sequence_number;
  uint32   failure_reason;
  uint32   subs_id;
  qmi_authi_client_state_type *auth_cl_sp;
} qmi_authi_ca_cert_cmd_buf_type;

/*---------------------------------------------------------------------------
  QMI AUTH Service configuration 
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_framework_svc_config_type    fw_cfg;
  qmi_svc_cmd_hdlr_type *          cmd_hdlr_array;   
  uint16                           cmd_num_entries;  
  void *                           sp;               
  boolean                          registered;   /* Service registration statu   */ 
} qmi_auth_svc_config_type;

typedef struct
{
  q_link_type    link;
  uint32         tx_id;
  uint8          num_cert;
  q_type         ca_cert_record_q;
}qmi_authi_ca_cert_tx_link_type;

/* Data type to store CA certificate record.
   each node represents a complete record of CA certificate */
typedef struct
{
  q_link_type   link;
  uint8         cert_index;
  uint8         num_segments;
  boolean       is_cert_complete;
  q_type        ca_cert_segment_q;
}qmi_authi_ca_cert_record_link_type;

/* Data type to store CA certificate record.
   each node represents a segment of CA certificate */
typedef struct
{
  q_link_type     link;
  uint8           sequence_num;
  dsm_item_type  *ca_cert_seg;
}qmi_authi_ca_cert_segment_link_type;

/*===========================================================================

                               INTERNAL DATA

===========================================================================*/

/*---------------------------------------------------------------------------
  QMI service command handlers
  forward declarations & cmd handler dispatch table definition
---------------------------------------------------------------------------*/

static dsm_item_type* qmi_authi_reset(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_authi_start_eap_session(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_authi_send_eap_packet(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_authi_get_eap_session_keys(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_authi_get_eap_auth_credentials(void* ,void* ,void* ,dsm_item_type **);
static dsm_item_type* qmi_authi_end_eap_session(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_authi_run_aka_algo(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_authi_set_subscription_binding(void *,void *,void *,dsm_item_type ** );
static dsm_item_type* qmi_authi_get_bind_subscription(void *,void *,void *,dsm_item_type ** );
static dsm_item_type* qmi_authi_indication_register(void*, void*, void*, dsm_item_type **);
static dsm_item_type * qmi_authi_send_imsi_public_key(void *, void *, void *,dsm_item_type **);
static dsm_item_type * qmi_authi_send_ca_certificate(void *, void *, void *,dsm_item_type **);


#define AUTH_HDLR(a,b)  QMI_SVC_HDLR( a, (qmi_svc_hdlr_ftype)b )

static qmi_svc_cmd_hdlr_type  qmi_authi_cmd_callbacks[AUTHI_CMD_MAX] =
{   
  AUTH_HDLR( AUTHI_CMD_VAL_RESET,
                qmi_authi_reset),
  AUTH_HDLR( AUTHI_CMD_VAL_START_EAP_SESSION,
                qmi_authi_start_eap_session),
  AUTH_HDLR( AUTHI_CMD_VAL_SEND_EAP_PACKET,
                qmi_authi_send_eap_packet),
  AUTH_HDLR( AUTHI_CMD_VAL_GET_EAP_SESSION_KEYS,
                qmi_authi_get_eap_session_keys),
  AUTH_HDLR( AUTHI_CMD_VAL_END_EAP_SESSION,
                qmi_authi_end_eap_session),
  AUTH_HDLR( AUTHI_CMD_VAL_RUN_AKA_ALGO,
                qmi_authi_run_aka_algo),
  AUTH_HDLR( AUTHI_CMD_VAL_SET_SUBSCRIPTION_BINDING,
                qmi_authi_set_subscription_binding),
  AUTH_HDLR( AUTHI_CMD_VAL_GET_BIND_SUBSCRIPTION,
                qmi_authi_get_bind_subscription),
  AUTH_HDLR( AUTHI_CMD_VAL_INDICATION_REGISTER,
		qmi_authi_indication_register),
  AUTH_HDLR( AUTHI_CMD_VAL_SEND_EAP_PACKET_EXT,
                qmi_authi_send_eap_packet),
  AUTH_HDLR( AUTHI_CMD_VAL_EAP_GET_AUTH_CREDS,
                qmi_authi_get_eap_auth_credentials),
  AUTH_HDLR( AUTHI_CMD_VAL_SEND_IMSI_PUBLIC_KEY,
                qmi_authi_send_imsi_public_key),
  AUTH_HDLR( AUTHI_CMD_VAL_SEND_CA_CERT_RESULT,
                qmi_authi_send_ca_certificate)
};

static qmi_authi_state_type  qmi_auth_state;

/*---------------------------------------------------------------------------
  QMI callback definition
---------------------------------------------------------------------------*/

static boolean qmi_authi_alloc_clid_cb
(
  qmi_framework_common_msg_hdr_type * svc_common_hdr  
);

static void qmi_authi_dealloc_clid_cb
(
  qmi_framework_common_msg_hdr_type * svc_common_hdr 
); 

static void qmi_authi_init_cb
(
  uint16 num_instances
);

static void qmi_authi_cmd_hdlr_cb
( 
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type ** sdu 
);

static qmi_authi_client_state_type * qmi_authi_alloc_cl_sp
(
  uint8 clid
);

static boolean qmi_authi_dealloc_cl_sp
(
  uint8 clid
);

static void    qmi_authi_reset_client(void *);
static void    qmi_authi_initialize_client(qmi_authi_client_state_type *);
static boolean qmi_auth_eap_task_srv_fct
(
  eap_sim_aka_task_srv_req_cb_type req_cb,
  void * user_data
);

static void qmi_authi_imsi_public_key_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
);

/*---------------------------------------------------------------------------
  QMI Service (QMI_AUTH) configuration definition
---------------------------------------------------------------------------*/
static qmi_auth_svc_config_type  qmi_authi_cfg;

static struct
{ 
  boolean inited;
  eap_sim_aka_meta_info_type    sim_aka_meta_info;
  boolean ca_cert_supported;
  void   *ca_client_ptr;
} qmi_authi_global = {FALSE,};

/*===========================================================================

                       FORWARD FUNCTION DECLARATIONS 

===========================================================================*/

static void qmi_authi_process_eap_sim_aka_task_switch
(
  qmi_authi_cmd_buf_type*
);

static void qmi_authi_send_eap_resp
(
  qmi_authi_cmd_buf_type*
);

static void qmi_authi_send_eap_result_ind
(
  qmi_authi_eap_result_cmd_buf_type*
);

static void qmi_authi_send_aka_result_ind
(
  qmi_authi_aka_result_cmd_buf_type*
);

static void qmi_authi_process_cmd(void *);

static void qmi_authi_process_svc_init
(
  uint16 num_instances
);

static void qmi_authi_process_alloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
);

static void qmi_authi_process_dealloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
);

static void qmi_authi_process_cmd_hdlr
(
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type * sdu 
);

static void qmi_authi_dispatch_transaction
(
  qmi_transaction_type *  x_p
);

static boolean qmi_authi_input
(
  qmi_cmd_buf_type *             cmd_buf_p,
  dsm_item_type **               sdu_in
);

static boolean qmi_authi_send_response
(
  qmi_authi_client_state_type * cl_sp, 
  qmi_cmd_buf_type *cmd_buf_p, 
  dsm_item_type *  msg_ptr
);

static boolean qmi_authi_queue_response
( 
  qmi_cmd_buf_type **  cmd_buf_p,
  dsm_item_type **     msg
);

static boolean qmi_authi_send_indication
(
  void * msg_hdr_p, 
  uint16 cmd_type, 
  dsm_item_type * ind 
);

static qmi_cmd_buf_type *  qmi_authi_get_transaction_cmd_buf
(
  qmi_transaction_type *  x_p
);

static void qmi_authi_free_transaction_cmd_buf
(
  qmi_cmd_buf_type **  cmd_buf_handle
);

static qmi_transaction_type * qmi_authi_get_transaction
(
  qmi_authi_state_type        *   svc_sp,
  qmi_authi_client_state_type *   cl_sp
);

static void qmi_authi_free_transaction
(
  qmi_transaction_type **  x_p
);

static void *qmi_authi_get_cmd_buf(qmi_cmd_id_e_type,uint16);

static void qmi_authi_send_eap_indcation
(
   uint8 clid,
   eap_sim_aka_indication_enum_type  ind_type,
   eap_sim_aka_ind_union_type value
);

static void qmi_authi_process_eap_indicaiton
(
   qmi_authi_cmd_buf_type *cmd_ptr
);

static void qmi_authi_send_retrieve_imsi_public_key_ind
(
   qmi_authi_cmd_buf_type *cmd_ptr
);

static void qmi_authi_get_svc_info_cb
(  
  uint8  clid,
  qmi_svc_info_type * svc_info
);

static void qmi_auth_retr_ca_cert_ind_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
);

static void qmi_authi_retr_ca_cert_ind
(
   qmi_authi_cmd_buf_type *cmd_ptr
);

static void qmi_authi_process_ca_cert
(
  qmi_authi_ca_cert_cmd_buf_type *cmd_ptr
);

static boolean qmi_authi_process_ca_cert_tx
(
  qmi_authi_ca_cert_tx_link_type * ca_cert_tx_link_ptr,
  uint16                           cert_index,
  dsm_item_type                  * ca_cert_payload,
  uint8                            num_segments,
  uint8                            sequence_number,
  uint8                          * num_complete_cert
);

static boolean qmi_authi_process_ca_cert_record
(
  qmi_authi_ca_cert_record_link_type * ca_cert_record_link_ptr,
  dsm_item_type                       *ca_cert_payload,
  uint8                                sequence_number
);

static boolean qmi_authi_process_ca_cert_segment
(
  q_type           *segment_q_ptr,
  dsm_item_type    *ca_cert_payload,
  uint8              sequence_number
);

static boolean prepare_and_send_root_cert_result
(
  ds_sys_ca_cert_result_enum_type      result,
  qmi_authi_ca_cert_tx_link_type      *ca_tx_link_ptr,
  uint32                               subs_id
);

static boolean qmi_authi_process_ca_cert_order
(
  qmi_authi_ca_cert_record_link_type  *ca_cert_record_link_ptr,
  ds_sys_ca_cert_records_q_type       *ds_sys_record_ptr
);
static void qmi_authi_ca_cert_free_tx
(
  q_type                       *tx_q_ptr,
  uint32                        tx_id,
  boolean                       remove_ca_cert_buf
);
static void qmi_authi_ca_cert_free_record
(
  q_type  *ca_cert_record_q,
  boolean  remove_ca_cert_buf
);
static void qmi_authi_ca_cert_free_segment
(
  q_type  *ca_cert_segment_q,
  boolean  remove_ca_cert_buf
);

static boolean prepare_and_send_root_cert_failure
(
  ds_sys_ca_cert_result_enum_type  result,
  uint32                           subs_id,
  uint32                           tx_id
);


static void qmi_authi_reg_sys_events(void);

#define qmi_authi_free_cmd_buf(buf_ptr) PS_SYSTEM_HEAP_MEM_FREE(buf_ptr)

/*===========================================================================
  FUNCTION QMI_AUTH_INIT()

  DESCRIPTION
    Register the AUTH service with QMI Framework 

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_auth_init
(
  void
)
{
  qmi_framework_err_e_type  reg_result = QMI_FRAMEWORK_ERR_NONE;
  uint32                 idl_major_ver = AUTHI_BASE_VER_MAJOR;
  uint32                 idl_minor_ver = AUTHI_BASE_VER_MINOR;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0("QMI AUTH service init");

  /*-------------------------------------------------------------------------
    Setting QMI AUTH service command handler with dcc task process
  -------------------------------------------------------------------------*/
  qmi_task_set_svc_cmd_handler(QMUX_SERVICE_AUTH,qmi_authi_process_cmd);

  /*-------------------------------------------------------------------------
    QMI Auth service configuration setup
  -------------------------------------------------------------------------*/
  /* Retrieving IDL major and minor version for QMI AUTH service*/
#ifndef TEST_FRAMEWORK  
  qmi_idl_get_idl_version(auth_get_service_object_v01(),&idl_major_ver);
  qmi_idl_get_idl_minor_version(auth_get_service_object_v01(),&idl_minor_ver);
#endif /* TEST_FRAMEWORK */
  qmi_authi_cfg.fw_cfg.base_version.major     = idl_major_ver;
  qmi_authi_cfg.fw_cfg.base_version.minor     = idl_minor_ver;

  qmi_authi_cfg.fw_cfg.addendum_version.major = AUTHI_ADDENDUM_VER_MAJOR;
  qmi_authi_cfg.fw_cfg.addendum_version.minor = AUTHI_ADDENDUM_VER_MINOR;

  qmi_authi_cfg.fw_cfg.cbs.alloc_clid         = qmi_authi_alloc_clid_cb;
  qmi_authi_cfg.fw_cfg.cbs.dealloc_clid       = qmi_authi_dealloc_clid_cb;
  qmi_authi_cfg.fw_cfg.cbs.init_cback         = qmi_authi_init_cb;
  qmi_authi_cfg.fw_cfg.cbs.cmd_hdlr           = qmi_authi_cmd_hdlr_cb;
  qmi_authi_cfg.fw_cfg.cbs.get_svc_info       = qmi_authi_get_svc_info_cb;

  qmi_authi_cfg.cmd_hdlr_array               = qmi_authi_cmd_callbacks;
  qmi_authi_cfg.cmd_num_entries               = AUTHI_CMD_MAX;
  qmi_authi_cfg.sp                            = &qmi_auth_state;

  /*-------------------------------------------------------------------------
    Calling QMI Framework API to register the service with Framework
  -------------------------------------------------------------------------*/
  reg_result = qmi_framework_reg_service( QMUX_SERVICE_AUTH,
                                          &qmi_authi_cfg.fw_cfg );

  if (reg_result != QMI_FRAMEWORK_ERR_NONE)
  {
    return;
  }
  
  /*-------------------------------------------------------------------------
    Register at power up for interested System Events.
  -------------------------------------------------------------------------*/
  qmi_authi_reg_sys_events();

  qmi_authi_cfg.registered  = TRUE;
} /* qmi_auth_init */

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_EAP_SIM_AKA_TASK_SWITCH()

  DESCRIPTION
    Process the task switch req

  PARAMETERS
    cmd_ptr

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_process_eap_sim_aka_task_switch
(
  qmi_authi_cmd_buf_type *cmd_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmd_ptr->data.eap_sim_aka.req_cb(cmd_ptr->data.eap_sim_aka.user_data);
} /* qmi_authi_process_eap_sim_aka_task_switch */


/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION DS_QMI_AUTH_ASSERT_WRAPPER()

DESCRIPTION
  Wrapper function for DS_QMI_AUTH ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_qmi_auth_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DS_QMI_AUTH_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* ds_qmi_auth_assert_wrapper */

/*===========================================================================
  FUNCTION QMI_AUTHI_REG_SYS_EVENTS()

  DESCRIPTION
    Registers for PS system level events

  PARAMETERS
    None

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_reg_sys_events
( 
  void
)
{
  int                      sub_ind = 0;
  int16                    result = 0;
  int16                    dss_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for (sub_ind = PS_SYS_PRIMARY_SUBS; sub_ind < PS_SYS_SUBS_MAX; sub_ind++)
  {  

    result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                  PS_SYS_EVENT_RETRIEVE_IMSI_PUBLIC_KEY_INFO,
                                  (ps_sys_subscription_enum_type) sub_ind,
                                  qmi_authi_imsi_public_key_cb,
                                  NULL,
                                  &dss_errno);
    if(result < 0)
    {
      LOG_MSG_ERROR_INT_3("Couldn't reg for ps sys event_id = d Err : (d) subs_id = d",
                       PS_SYS_EVENT_RETRIEVE_IMSI_PUBLIC_KEY_INFO, dss_errno, sub_ind);
    }

    result = ps_sys_event_reg_ex (PS_SYS_TECH_ALL,
                                  PS_SYS_EVENT_GET_CA_CERTIFICATE,
                                  (ps_sys_subscription_enum_type) sub_ind,
                                  qmi_auth_retr_ca_cert_ind_cb,
                                  NULL,
                                  &dss_errno);
    if(result < 0)
    {
      LOG_MSG_ERROR_3("Couldn't reg for ps sys event_id = %d Err : (%d) subs_id = %d",
                       PS_SYS_EVENT_GET_CA_CERTIFICATE, dss_errno, sub_ind);
    }
  }
}/* qmi_authi_reg_sys_events */

/*===========================================================================
  FUNCTION QMI_AUTH_EAP_TASK_SRV_FCT()

  DESCRIPTION
    Cback function registered with EAP SIM/AKA for task switch

  PARAMETERS
    req_cb - cback to be called after switching tasks
    user_data

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_auth_eap_task_srv_fct
(
  eap_sim_aka_task_srv_req_cb_type req_cb,
  void * user_data
)
{
  qmi_authi_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(
                      QMI_CMD_EAP_SIM_AKA_TASK_SWITCH_EV,__LINE__);
  if( cmd_ptr == NULL)
  {
    return FALSE;
  }

  memset(cmd_ptr, 0, sizeof(qmi_authi_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_EAP_SIM_AKA_TASK_SWITCH_EV;
  cmd_ptr->data.eap_sim_aka.req_cb = req_cb;
  cmd_ptr->data.eap_sim_aka.user_data = user_data;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);

  return TRUE;
} /* qmi_auth_eap_task_srv_fct() */

/*===========================================================================
FUNCTION qmi_authi_get_svc_info_cb()

DESCRIPTION
  Function returns service info associated with client id.

PARAMETERS
  clid  : Client id
  svc_info : Service info

RETURN VALUE
  Service information
  
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static void qmi_authi_get_svc_info_cb
(  
  uint8  clid,
  qmi_svc_info_type * svc_info
)
{
  qmi_authi_client_state_type * cl_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (clid > 0 && clid <= AUTHI_MAX_CLIDS) 
  {
    cl_sp = (qmi_authi_client_state_type *) qmi_auth_state.client[clid - 1];
    if (cl_sp && svc_info) 
    {
      svc_info->subscription_id = cl_sp->subscription_id;
    }                 
  }
  
  return;
} /* qmi_authi_get_svc_info_cb() */



/*===========================================================================
  FUNCTION QMI_EAP_TRP_TX_CBACK()

  DESCRIPTION
    Tx cback registered with EAP to get EAP response packets

  PARAMETERS
    user_data
    eap_resp_pkt

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_eap_trp_tx_cback
(
  void           * user_data, 
  dsm_item_type ** eap_resp_pkt
)
{
  qmi_authi_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(user_data != NULL);

  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(QMI_CMD_SEND_EAP_RESP,__LINE__);
  if( cmd_ptr == NULL)
  {
    return;
  }

  memset(cmd_ptr, 0, sizeof(qmi_authi_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_SEND_EAP_RESP;
  cmd_ptr->data.eap_resp.pkt = *eap_resp_pkt;
  cmd_ptr->data.eap_resp.user_data = user_data;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
}


/*===========================================================================
  FUNCTION QMI_AUTH_SENDI_EAP_RESP()

  DESCRIPTION
    Process the eap resp cback and send EAP response

  PARAMETERS
    cmd_ptr

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_send_eap_resp
(
  qmi_authi_cmd_buf_type * cmd_ptr
)
{
  qmi_authi_client_state_type * auth_cl_sp;
  qmi_cmd_buf_type            * cmd_buf_p;
  dsm_item_type *              eap_resp_pkt;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  uint16   tlv_len;
  uint8  tlv_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  eap_resp_pkt = cmd_ptr->data.eap_resp.pkt;
  auth_cl_sp = (qmi_authi_client_state_type *) cmd_ptr->data.eap_resp.user_data;

  DS_QMI_AUTH_ASSERT(auth_cl_sp != NULL);
  cmd_buf_p = auth_cl_sp->eap_info.pending_req_cmd_buf;

  /* clear the cmd_buf since now sending response */
  auth_cl_sp->eap_info.pending_req_cmd_buf = NULL;

  if(cmd_buf_p == NULL)
  {
    LOG_MSG_ERROR_INT_0("got eap_trp_tx_cback but no REQ pending!");
    dsm_free_packet(&eap_resp_pkt);
    return;
  }

  errval = QMI_ERR_NONE;
 
  tlv_len = (uint16) dsm_length_packet(eap_resp_pkt);
  tlv_type = QMI_TYPE_REQUIRED_PARAMETERS;

  if ( (QMI_SVC_PKT_PUSH(&eap_resp_pkt, &tlv_len, sizeof(uint16))) &&
       (QMI_SVC_PKT_PUSH(&eap_resp_pkt, &tlv_type, sizeof(uint8))) 
       == FALSE )
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&eap_resp_pkt);
  }

  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);

  if(FALSE == qmi_svc_put_result_tlv(&eap_resp_pkt, result, errval))
  {
    dsm_free_packet(&eap_resp_pkt);
    qmi_authi_free_transaction_cmd_buf( &cmd_buf_p );
    return;
  }

  if(FALSE == qmi_authi_send_response(auth_cl_sp, cmd_buf_p, eap_resp_pkt) )
  {
    LOG_MSG_ERROR_INT_0 ("Unable to send response for QMI EAP SEND PKT");
    dsm_free_packet(&eap_resp_pkt);
    qmi_authi_free_transaction_cmd_buf(&cmd_buf_p);
  }
} /* qmi_authi_send_eap_resp() */


/*===========================================================================
  FUNCTION QMI_EAP_RESULT_IND_CBACK()

  DESCRIPTION
    Process the task switch req

  PARAMETERS
    handle - eap handle
    user data
    result - result of EAP 
    supp_auth_info_ptr - EAP session information

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_eap_result_ind_cback
(
    
  eap_handle_type                  handle,
  void                            *supp_result_ind_user_data,
  eap_result_enum_type  result,
  eap_sim_aka_supp_auth_mask_type  supp_auth_info_mask,
  uint16                           supp_auth_info_size,
  void                            *supp_auth_info_ptr
)
{
  eap_sim_aka_auth_cred_type        *auth_cred = NULL;
  qmi_authi_eap_result_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* check that result_ind_user_data is valid in case ASSERT is removed */
  if(supp_result_ind_user_data == NULL)
  {
    LOG_MSG_ERROR_INT_0("Result IND user data NULL");
    return;
  }

  if( supp_auth_info_size <= sizeof(eap_sim_aka_auth_cred_type) )
  {
    LOG_MSG_INFO1_2("supp_auth_info_size %d sizeof %d",
                    supp_auth_info_size, sizeof(eap_sim_aka_auth_cred_type));
  }

  DS_QMI_AUTH_ASSERT(supp_result_ind_user_data && (supp_auth_info_size <= sizeof(eap_sim_aka_auth_cred_type)));


  if(supp_auth_info_mask == EAP_SUPP_AUTH_INFO_CRED)
  {
    auth_cred = (eap_sim_aka_auth_cred_type *)supp_auth_info_ptr;
   // Process MSK, EMSK and Session-ID
  }

  if(NULL == auth_cred)
  {
    LOG_MSG_ERROR_INT_0("auth_cred ptr NULL");
    return;
  }

    /* check that master_key_len is valid in case ASSERT is removed */
  if( (auth_cred->msk_len > QMI_EAP_MAX_PRE_MASTER_KEY_SIZE) ||
      (auth_cred->emsk_len > QMI_EAP_MAX_EXT_MASTER_KEY_SIZE) ||
      (auth_cred->session_id_len > EAP_SIM_AKA_MAX_SESSION_ID))
  {
    LOG_MSG_ERROR_3("Pre master key len (%d) Ext Master Key len (%d)"
                    "Session Id Len (%d)", 
                    auth_cred->msk_len, 
                    auth_cred->emsk_len, 
                    auth_cred->session_id_len);
    return;
  }

  
  cmd_ptr = (qmi_authi_eap_result_cmd_buf_type *)qmi_authi_get_cmd_buf(
  	            QMI_CMD_SEND_EAP_RESULT,__LINE__);
  if( cmd_ptr == NULL)
  {
    return;
  }

  memset(cmd_ptr, 0, sizeof(qmi_authi_eap_result_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_SEND_EAP_RESULT;
  cmd_ptr->handle = handle;
  cmd_ptr->result = result;
  cmd_ptr->pmk_len = auth_cred->msk_len;
  cmd_ptr->emsk_len = auth_cred->emsk_len;
  cmd_ptr->session_id_len = auth_cred->session_id_len;
  cmd_ptr->user_data = supp_result_ind_user_data;
  
  memscpy(cmd_ptr->pmk, QMI_EAP_MAX_PRE_MASTER_KEY_SIZE, auth_cred->msk, auth_cred->msk_len);
  memscpy(cmd_ptr->emsk, QMI_EAP_MAX_EXT_MASTER_KEY_SIZE, auth_cred->emsk, auth_cred->emsk_len);
  memscpy(cmd_ptr->session_id, EAP_SIM_AKA_MAX_SESSION_ID, 
          auth_cred->session_id, auth_cred->session_id_len);
 
  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
}/* qmi_eap_result_ind_cback */

/*===========================================================================
  FUNCTION QMI_AUTH_EAP_INDICATION_CBACK()

  DESCRIPTION
     indication callback function from EAP module

  PARAMETERS
     eap_handle - handle of the eap session
     ind_type - type of the indication 
     payload - data related to the indication type

  RETURN VALUE
     None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_auth_eap_indication_cback
(
   eap_handle_type  eap_handle,
   eap_sim_aka_indication_enum_type ind_type,
   eap_sim_aka_ind_union_type* payload
)
{
  qmi_authi_cmd_buf_type *cmd_ptr;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( (ind_type >= EAP_SIM_AKA_IND_MAX) || (ind_type < EAP_SIM_AKA_IND_NOTIFICATION_CODE) ||
      ((ind_type != EAP_SIM_AKA_IND_AUTH_REJECT) && (payload == NULL)) )
  {
    LOG_MSG_ERROR_1("qmi_auth_eap_indication_cback. Invalid ind_type received (%d)", ind_type);
    return;
  }

  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(
                    QMI_CMD_EAP_INDICATION_CB,__LINE__);
  if( cmd_ptr == NULL)
  {
    return;
  }

  memset(cmd_ptr, 0, sizeof(qmi_authi_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_EAP_INDICATION_CB;
  cmd_ptr->data.eap_indication_cb.eap_handle = eap_handle;
  cmd_ptr->data.eap_indication_cb.ind_type = ind_type;
  if(payload != NULL)
  {
     memscpy(&cmd_ptr->data.eap_indication_cb.value,
            sizeof(eap_sim_aka_ind_union_type),
            payload,
            sizeof(eap_sim_aka_ind_union_type));
  }

  /*Post command to DCC task*/
  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
}

/*===========================================================================
  FUNCTION QMI_AUTHI_SEND_EAP_RESULT_IND()

  DESCRIPTION
    Process the eap resp cback and send EAP response

  PARAMETERS
    cmd_ptr

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_send_eap_result_ind
(
  qmi_authi_eap_result_cmd_buf_type *  cmd_ptr
)
{
  dsm_item_type               *  ind;
  qmi_authi_client_state_type * auth_cl_sp;
  qmi_framework_msg_hdr_type    framework_msg_hdr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_AUTH_ASSERT(cmd_ptr != NULL);

  auth_cl_sp = (qmi_authi_client_state_type *) cmd_ptr->user_data;

  if(auth_cl_sp->eap_info.eap_handle != cmd_ptr->handle)
  {
    LOG_MSG_ERROR_2("Eap Handle in result_ind_cb(%d) does not match client's handle(%d)",
                    cmd_ptr->handle, auth_cl_sp->eap_info.eap_handle);
    return;
  } 

  ind = NULL;
  auth_cl_sp->eap_info.result = cmd_ptr->result;

  DS_QMI_AUTH_ASSERT(cmd_ptr->pmk_len <= QMI_EAP_MAX_PRE_MASTER_KEY_SIZE);

  /* check that pre_master_key_len is valid in case ASSERT is removed */
  if(cmd_ptr->pmk_len > QMI_EAP_MAX_PRE_MASTER_KEY_SIZE)
  {
    LOG_MSG_ERROR_INT_1("PMK len invalid (d)", cmd_ptr->pmk_len);
    return;
  }
  
  auth_cl_sp->eap_info.msk_len = cmd_ptr->pmk_len;
  auth_cl_sp->eap_info.emsk_len = cmd_ptr->emsk_len;
  auth_cl_sp->eap_info.session_id_len = cmd_ptr->session_id_len;

  memscpy(auth_cl_sp->eap_info.msk, 
          QMI_EAP_MAX_PRE_MASTER_KEY_SIZE,
         cmd_ptr->pmk, 
         cmd_ptr->pmk_len);

  memscpy(auth_cl_sp->eap_info.emsk, 
          QMI_EAP_MAX_EXT_MASTER_KEY_SIZE,
          cmd_ptr->emsk, 
          cmd_ptr->emsk_len);
    
  memscpy(auth_cl_sp->eap_info.session_id, 
          EAP_SIM_AKA_MAX_SESSION_ID,
          cmd_ptr->session_id, 
          cmd_ptr->session_id_len);

  auth_cl_sp->eap_info.eap_result_ready = TRUE;

  if( FALSE == qmi_svc_put_param_tlv(&ind, 
                                     QMI_TYPE_REQUIRED_PARAMETERS,
                                     sizeof (uint8),
                                     &(auth_cl_sp->eap_info.result)) )
  {
    LOG_MSG_INFO2_0("Unable to generate EAP Result indication!");
    dsm_free_packet(&ind);
    return;
  }

  /*-----------------------------------------------------------------------
    Fill the QMI Framework message header before sending Indication to
    Framework.
  -----------------------------------------------------------------------*/
  framework_msg_hdr.common_hdr.client_id = auth_cl_sp->common.clid;
  /* if unicast indication, qmi framework map the instance and no need to fill*/
  framework_msg_hdr.common_hdr.qmi_instance = AUTHI_INVALID_INSTANCE;
  framework_msg_hdr.common_hdr.service = QMUX_SERVICE_AUTH;
  /* for indication the transaction ID can be ignored */
  framework_msg_hdr.common_hdr.transaction_id = 0;
  framework_msg_hdr.msg_ctl_flag = QMI_FLAG_MSGTYPE_IND;
  framework_msg_hdr.msg_len = (uint16) dsm_length_packet(ind);

  if( FALSE == qmi_authi_send_indication( &framework_msg_hdr, 
                                          AUTHI_CMD_VAL_EAP_SESSION_RESULT_IND, 
                                          ind))
  {
    LOG_MSG_ERROR_INT_0("Unable to send qmi_authi_send_eap_result_ind!");
    dsm_free_packet(&ind);
  }
} /* qmi_authi_send_eap_result_ind */


/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_EAP_INDICATION()

  DESCRIPTION
    Process the cmd posted for sending eap indications

  PARAMETERS
    cmd_ptr : data passed from command

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_process_eap_indicaiton
(
   qmi_authi_cmd_buf_type *cmd_ptr
)
{
  qmi_authi_client_state_type * cl_sp = NULL;
  eap_handle_type eap_handle;
  int index = 0;
  eap_sim_aka_indication_enum_type ind_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   DS_QMI_AUTH_ASSERT(cmd_ptr != NULL);

  eap_handle = (eap_handle_type)cmd_ptr->data.eap_indication_cb.eap_handle;
  ind_type   = (eap_sim_aka_indication_enum_type) cmd_ptr->data.eap_indication_cb.ind_type;
  LOG_MSG_INFO1_2("qmi_authi_process_eap_indicaiton: Process ind_type: %d for eap_handle: 0x%x",
                   ind_type, eap_handle);  

  if((ind_type >= EAP_SIM_AKA_IND_MAX) || (ind_type < EAP_SIM_AKA_IND_NOTIFICATION_CODE))
  {
    return;
  }

  /*loop through all clients and send ind to clients registered*/
  for( index = 0; index < AUTHI_MAX_CLIDS; index++ )
  {
    cl_sp = (qmi_authi_client_state_type *) qmi_auth_state.client[index];
    if( cl_sp && (cl_sp->common.clid != QMI_SVC_CLID_UNUSED) && 
       (cl_sp->eap_info.eap_handle == eap_handle) && 
       ( (ind_type == EAP_SIM_AKA_IND_NOTIFICATION_CODE && cl_sp->report_status.report_eap_notification_code) ||
         (ind_type == EAP_SIM_AKA_IND_CLIENT_ERROR_CODE && cl_sp->report_status.report_eap_err_ind_code) ||
         (ind_type == EAP_SIM_AKA_IND_AUTH_REJECT && cl_sp->report_status.report_eap_auth_rej)) )
     {
       qmi_authi_send_eap_indcation(cl_sp->common.clid, 
                                     ind_type,
                                     cmd_ptr->data.eap_indication_cb.value);
     }
   }
}

/*===========================================================================
  FUNCTION QMI_AUTHI_SEND_EAP_INDICATION() 

  DESCRIPTION
    Send the related eap indication to clients

  PARAMETERS
    clid : id of client registered for indications
    ind_type : type of the indication
    value : error/notification code value

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_send_eap_indcation
(
   uint8 clid,
    eap_sim_aka_indication_enum_type  ind_type,
    eap_sim_aka_ind_union_type        value
)
{
  dsm_item_type *  ind = NULL;
  qmi_framework_common_msg_hdr_type common_hdr;
  uint8 cmd_type;
  boolean v_out1_set = FALSE;
  PACKED struct PACKED_POST
  {
     uint16 value;
  } v_out1;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset(&v_out1,0,sizeof(v_out1));

  switch(ind_type)
  {
    case EAP_SIM_AKA_IND_NOTIFICATION_CODE:
       v_out1.value  = (uint16)value.notification_code;
       v_out1_set = TRUE;
       cmd_type = (uint8)AUTHI_CMD_VAL_EAP_NOTIFICATION_CODE_IND;
     break;
    case EAP_SIM_AKA_IND_CLIENT_ERROR_CODE:
       v_out1.value  = (uint16)value.client_error;
       v_out1_set = TRUE;
       cmd_type = (uint8)AUTHI_CMD_VAL_EAP_CLIENT_ERR_IND;
     break;
    case EAP_SIM_AKA_IND_AUTH_REJECT:
       cmd_type = (uint8)AUTHI_CMD_VAL_EAP_AUTH_REJ_IND;
     break;
    default:
       LOG_MSG_INFO2_2("qmi_authi_send_eap_indcation: %d to registered client: %d",
                        ind_type,clid);
       return;
     break;
   }

  if( v_out1_set && ( FALSE == qmi_svc_put_param_tlv(&ind, 
                                          QMI_TYPE_REQUIRED_PARAMETERS,
                                          sizeof(v_out1),
                                          &v_out1)) )
  {
    dsm_free_packet(&ind);
    return;
  }

  /*-----------------------------------------------------------------------
  Fill the QMI message header before sending Indication to Framework.
  -----------------------------------------------------------------------*/
  common_hdr.client_id = clid;
  /* if unicast indication, qmi framework map the instance and no need to fill*/
  common_hdr.qmi_instance = AUTHI_INVALID_INSTANCE;
  common_hdr.service = QMUX_SERVICE_AUTH;
  /* for indication the transaction ID can be ignored */
  common_hdr.transaction_id = 0;

  if( FALSE == ds_qmi_fw_send_ind( &common_hdr,
                                   cmd_type,
                                   ind ) )
  {
    LOG_MSG_ERROR_INT_0("Unable to send eap_indication!");
    dsm_free_packet(&ind);
  }
}

/*===========================================================================
  FUNCTION QMI_AUTHI_RESET()

  DESCRIPTION
    Reset the issuing AUTH client's state
    
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_reset
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *              response;
  boolean                      retval;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  /*-------------------------------------------------------------------------
    Reset the client's state
  -------------------------------------------------------------------------*/
  qmi_authi_reset_client(cl_sp);

  /*-------------------------------------------------------------------------
    Build the response from back to front
    No error possible in reset, so result is success
  -------------------------------------------------------------------------*/
  response = NULL;

  retval = qmi_svc_put_result_tlv(&response,
                                  QMI_RESULT_SUCCESS,
                                  QMI_ERR_NONE); 
  CHECK_RETVAL();

  return response;

} /* qmi_authi_reset() */

/*===========================================================================
  FUNCTION QMI_AUTHI_START_EAP_SESSION()

  DESCRIPTION
    Start the EAP session
  
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_start_eap_session
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *               response;
  qmi_authi_client_state_type * auth_cl_sp;
  eap_info_type                eap_req_params;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean                      retval;
  boolean  algo_tlv_set = FALSE;
  uint32   eap_sim_aka_algo = EAP_AKA_ALGO_NONE;
  uint32                       eap_method_mask;
  uint8    type;
  uint16   len;
  void *   value;
  uint16   expected_len;
  uint32 checkcode_setting_mask;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  errval = QMI_ERR_NONE;

  memset( &eap_req_params, 0, sizeof(eap_req_params) );
  /* Initialise the global variables */
  memset( &(qmi_authi_global.sim_aka_meta_info.eap_meta_identity), 0,
            sizeof(qmi_authi_global.sim_aka_meta_info.eap_meta_identity));
  memset( &(qmi_authi_global.sim_aka_meta_info.id), 0,
            sizeof(qmi_authi_global.sim_aka_meta_info.id));
  memset( &(qmi_authi_global.sim_aka_meta_info.sim_aka_algo_type), 0,
            sizeof(qmi_authi_global.sim_aka_meta_info.sim_aka_algo_type));
  qmi_authi_global.sim_aka_meta_info.new_client = FALSE;

  auth_cl_sp = (qmi_authi_client_state_type *) cl_sp;
  /* default allow SIM or AKA or AKA_PRIME(AKA') */
  eap_method_mask = AUTHI_EAP_METHOD_SIM | AUTHI_EAP_METHOD_AKA | AUTHI_EAP_METHOD_AKA_PRIME;

  while (*sdu_in)
  {
    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    // special value 0 = variable length or don't care (unrecognzied TLV)
    expected_len = 0;
    value = NULL;
    switch (type)
    {
      case AUTHI_PRM_TYPE_EAP_METHOD_MASK:
        expected_len = sizeof(eap_method_mask);
        value = (void *) &eap_method_mask;
        break;

      /* expected_len is still 0 so that we will go past the check */
      case AUTHI_PRM_TYPE_EAP_USER_ID:
        if(len <= (sizeof(eap_req_params.user_id.name)+sizeof(uint8)))
        {
          value = (void *) &eap_req_params.user_id.len;

          /* copy first byte to length field.The structure here is as below
             =========================================
              type | Length | user_id length| user_id
             ========================================= */
          if(PS_UTILS_DSM_PULLUP( sdu_in, value, sizeof(uint8)) != sizeof(uint8))
          {
            errval = QMI_ERR_MALFORMED_MSG;
            goto send_result;
          }
          if((eap_req_params.user_id.len != (len -= sizeof(uint8))) || 
            (eap_req_params.user_id.len == 0))
          {
            errval = QMI_ERR_MALFORMED_MSG;
            /* TODO need to failover here */
            goto send_result;
          }  
          value = (void *) &eap_req_params.user_id.name;
        }
        else
        {
          errval = QMI_ERR_ARG_TOO_LONG;
          goto send_result;
        }
        break;

      case AUTHI_PRM_TYPE_EAP_META_IDENTITY:
        if(len <= (sizeof(qmi_authi_global.sim_aka_meta_info.eap_meta_identity.name) + sizeof(uint8)))
        {
          value = (void *) &qmi_authi_global.sim_aka_meta_info.eap_meta_identity.len;
          /* copy first byte to length field.The structure here is as below
                 ========================================
                 type | Length | meta_id length| meta_id
                 ======================================== */
          if(PS_UTILS_DSM_PULLUP( sdu_in, value, sizeof(uint8)) != sizeof(uint8))
          {
            errval = QMI_ERR_MALFORMED_MSG;
            goto send_result;
          }
          if((qmi_authi_global.sim_aka_meta_info.eap_meta_identity.len != (len -= sizeof(uint8))) ||
            (qmi_authi_global.sim_aka_meta_info.eap_meta_identity.len == 0))
          {
            errval = QMI_ERR_MALFORMED_MSG;
            /* TODO need to failover here */
            goto send_result;
          }  
          value = (void *) &qmi_authi_global.sim_aka_meta_info.eap_meta_identity.name;
        }
        else
        {
          errval = QMI_ERR_ARG_TOO_LONG;
          goto send_result;
        }
        break;

      case AUTHI_PRM_TYPE_EAP_SIM_AKA_ALGO:
        expected_len = sizeof(eap_sim_aka_algo);
        value = (void *) &eap_sim_aka_algo;
        algo_tlv_set = TRUE;
        break;

     default:
        LOG_MSG_INFO2_1 ("Unrecognized TLV type (%d)", type);
        break;
    }
    if(!qmi_svc_validate_type_length( type,
                                          len,
                                          expected_len,
                                          FILE_ID_DS_QMI_AUTH,__LINE__))
    {
      errval = QMI_ERR_MALFORMED_MSG;
      goto send_result;
    }

    /*-----------------------------------------------------------------------
      If type is unrecognized, value will be NULL, and dsm_pullup will 
      free the unrecognized value, i.e. we skip past unrecognized TLVs
    -----------------------------------------------------------------------*/
    if(len != PS_UTILS_DSM_PULLUP( sdu_in, value, len ))
    {
      errval = QMI_ERR_MALFORMED_MSG;
      goto send_result;
    }
  }


  if(!((eap_method_mask > AUTHI_EAP_METHOD_NONE)
      && (eap_method_mask <= (AUTHI_EAP_METHOD_SIM | AUTHI_EAP_METHOD_AKA | AUTHI_EAP_METHOD_AKA_PRIME))))
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }
  if((algo_tlv_set == TRUE) && !(eap_sim_aka_algo < EAP_SIM_AKA_ALGO_MAX))
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }    

  if(auth_cl_sp->eap_info.eap_handle != EAP_INVALID_HANDLE)
  {
    errval = QMI_ERR_INVALID_OPERATION;
    goto send_result;
  }

  /* populate the eap_info_type struct used to create eap instance */
  if(algo_tlv_set == TRUE)
  {
    /* if eap_method_mask is set  to AUTHI_EAP_METHOD_SIM
       then acceptable eap_sim_aka_algo types are EAP_SIM_ALGO_GSM or EAP_SIM_ALGO_USIM_GSM
       or EAP_SIM_ALGO_NONE */
    if((eap_method_mask & AUTHI_EAP_METHOD_SIM) && !((eap_method_mask & AUTHI_EAP_METHOD_AKA) ||
                                                     (eap_method_mask & AUTHI_EAP_METHOD_AKA_PRIME)))
    {
      if((eap_sim_aka_algo == EAP_SIM_ALGO_GSM) ||
         (eap_sim_aka_algo == EAP_SIM_ALGO_USIM_GSM) ||
         (eap_sim_aka_algo == EAP_AKA_ALGO_NONE))
      {
        eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_SIM_METHOD);
      }
      else
      {
        errval = QMI_ERR_INVALID_ARG;
        goto send_result;
      }
    }

    /* if eap_method_mask is set  to AUTHI_EAP_METHOD_AKA
       then acceptable eap_sim_aka_algo types are EAP_AKA_ALGO_SHA1 or EAP_AKA_ALGO_MILENAGE
       or EAP_AKA_ALGO_CAVE or EAP_SIM_ALGO_NONE */
    else if((eap_method_mask & AUTHI_EAP_METHOD_AKA) && !((eap_method_mask & AUTHI_EAP_METHOD_SIM) ||
                                                         (eap_method_mask & AUTHI_EAP_METHOD_AKA_PRIME)))
    {
      if((eap_sim_aka_algo == EAP_AKA_ALGO_SHA1) ||
        (eap_sim_aka_algo == EAP_AKA_ALGO_MILENAGE) ||
        (eap_sim_aka_algo == EAP_AKA_ALGO_CAVE)  ||
        (eap_sim_aka_algo == EAP_AKA_ALGO_NONE))
      {
        eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_AKA_METHOD);
      }
      else
      {
        errval = QMI_ERR_INVALID_ARG;
        goto send_result;
      }
    }
    else if((eap_method_mask & AUTHI_EAP_METHOD_AKA_PRIME) && 
          !((eap_method_mask & AUTHI_EAP_METHOD_SIM) || (eap_method_mask & AUTHI_EAP_METHOD_AKA)))
    {
      if((eap_sim_aka_algo == EAP_AKA_ALGO_MILENAGE) ||
         (eap_sim_aka_algo == EAP_AKA_ALGO_NONE))
      {
        eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_AKA_PRIME_METHOD);
      }
      else
      {
        errval = QMI_ERR_INVALID_ARG;
        goto send_result;
      }
    }
    /* if eap_method_mask is not set then eap_auth_prot_mask is set as per eap_sim_aka_algo type */
    else if(eap_method_mask & (AUTHI_EAP_METHOD_SIM | AUTHI_EAP_METHOD_AKA | 
                                                 AUTHI_EAP_METHOD_AKA_PRIME))
    {
      if((eap_sim_aka_algo == EAP_SIM_ALGO_GSM) ||
        (eap_sim_aka_algo == EAP_SIM_ALGO_USIM_GSM))
      {
        eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_SIM_METHOD);
      }
      else if((eap_sim_aka_algo == EAP_AKA_ALGO_SHA1) ||
              (eap_sim_aka_algo == EAP_AKA_ALGO_CAVE))
      {
        eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_AKA_METHOD);
      }
      else if(eap_sim_aka_algo == EAP_AKA_ALGO_MILENAGE)
      {
        eap_req_params.eap_auth_prot_mask |= ((0x1 << EAP_AKA_METHOD) | 
                                              (0x1 << EAP_AKA_PRIME_METHOD));
      }
      /* since we have already checked the validity of eap_sim_aka_algo this will obviously fall 
         into EAP_AKA_ALGO_NONE case */
      else
      {
        eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_SIM_METHOD | 0x1 << EAP_AKA_METHOD |
                                              0x1 << EAP_AKA_PRIME_METHOD);
      }
    }
  }
  else
  {
    if(eap_method_mask & AUTHI_EAP_METHOD_SIM)
      eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_SIM_METHOD);
    if(eap_method_mask & AUTHI_EAP_METHOD_AKA) 
      eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_AKA_METHOD);
    if(eap_method_mask & AUTHI_EAP_METHOD_AKA_PRIME)
      eap_req_params.eap_auth_prot_mask |= (0x1 << EAP_AKA_PRIME_METHOD);
  }

  eap_req_params.authenticator_rsp_required = FALSE;
  eap_req_params.raw_mode = FALSE;
  eap_req_params.support_eap_ext = FALSE;
  eap_req_params.supp_result_ind_f_ptr = (void *) qmi_eap_result_ind_cback;
  eap_req_params.supp_result_ind_user_data= cl_sp;
  eap_req_params.trp_tx_f_ptr = qmi_eap_trp_tx_cback;
  eap_req_params.trp_tx_user_data = cl_sp;
  
  qmi_authi_global.sim_aka_meta_info.supp_result_auth_info_mask = EAP_SUPP_AUTH_INFO_CRED;

  /* init SIM and AKA methods. */
  eap_sim_init();
  eap_aka_init();
  /* init AKA PRIME method */
  if( eap_req_params.eap_auth_prot_mask & (0x1 << EAP_AKA_PRIME_METHOD) )
  {
    eap_aka_prime_init();
  }

  auth_cl_sp->eap_info.eap_handle = eap_create_instance(&eap_req_params);

  if(auth_cl_sp->eap_info.eap_handle == EAP_INVALID_HANDLE)
  {
    LOG_MSG_ERROR_INT_1("EAP create instance failed for client: d",
                    auth_cl_sp->common.clid);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }
  /* populate the eap_sim_aka_meta_info_type struct used to set meta_info 
     for SIM AKA and meta_info */
  if(eap_req_params.user_id.len > 0)
  {
    qmi_authi_global.sim_aka_meta_info.id.provided_id_len = (uint8)eap_req_params.user_id.len;
    memscpy(qmi_authi_global.sim_aka_meta_info.id.provided_id, 
            eap_req_params.user_id.len,
            eap_req_params.user_id.name,
           eap_req_params.user_id.len);

    /* Since user_id TLV is optional we need this to set this to identify if it is 
       being dealt as a client which is supposed to be using user_id  */
    qmi_authi_global.sim_aka_meta_info.new_client = TRUE;
  }

  /*Save the Algo received in global*/
  /*EAP_AKA_ALGO_NONE will be saved/passed if no user preference*/
  if(algo_tlv_set == TRUE)
  {
    qmi_authi_global.sim_aka_meta_info.sim_aka_algo_type = 
                          (eap_sim_aka_algo_enum_type)eap_sim_aka_algo;
  }

#if (defined FEATURE_DUAL_SIM || defined FEATURE_TRIPLE_SIM)
/*Device is in multsim mode, get the correct session type mapped to subs_id*/
  if( auth_cl_sp->subscription_id >= AUTHI_BIND_SUBS_MAX)
  {
     errval = QMI_ERR_INTERNAL;
     goto send_result;
  }

  checkcode_setting_mask = EAP_CLIENT_CHECKCODE_VERIFICATION_SETTING | EAP_CLIENT_SEND_CHECKCODE_SETTING;
  
  /* Set QMI AUTH as EAP client and set client params */
  eap_set_auth_client_id( auth_cl_sp->eap_info.eap_handle,
                          EAP_CLIENT_QMI_AUTH,
                          checkcode_setting_mask);

  qmi_authi_global.sim_aka_meta_info.eap_session_is_set = TRUE;
  qmi_authi_global.sim_aka_meta_info.eap_session_type = 
           eap_client_config_get_session_type
            ( (uint8)auth_cl_sp->subscription_id,
              eap_sim_aka_algo ); 

  LOG_MSG_INFO1_3("subscription type %d algo_type %d & session set to %d", 
                  auth_cl_sp->subscription_id,
                  eap_sim_aka_algo,
                  qmi_authi_global.sim_aka_meta_info.eap_session_type );
#endif

 /*qmi_authi_global.sim_aka_meta_info.notification_rcv_cb_fct = qmi_auth_eap_notification_cback;*/
 /*including notification code, error codes and Auth rej indications will be sent from this new call back*/
  qmi_authi_global.sim_aka_meta_info.indication_rcv_cb_fct = qmi_auth_eap_indication_cback;

  /* set the meta_info for SIM AKA */
  eap_set_meta_info(auth_cl_sp->eap_info.eap_handle, (void *) &(qmi_authi_global.sim_aka_meta_info));
send_result:
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response,
                                  result,
                                  errval); 
  CHECK_RETVAL();

  return response;

} /* qmi_authi_start_eap_session() */



/*===========================================================================
  FUNCTION QMI_AUTHI_SEND_EAP_PACKET()

  DESCRIPTION
    Gives the name of the device manufacturer.
    
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_send_eap_packet
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *    response;
  qmi_authi_client_state_type  * auth_cl_sp;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean            retval;
  uint8    type;
  uint16   len;
  boolean  got_v_in_reqd;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);
  
  response = NULL;
  errval = QMI_ERR_NONE;
  auth_cl_sp = (qmi_authi_client_state_type *) cl_sp;

  while (*sdu_in)
  {
    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    // special value 0 = variable length or don't care (unrecognzied TLV)
    switch (type)
    {
      case QMI_TYPE_REQUIRED_PARAMETERS:
        got_v_in_reqd = TRUE;
        break;

     default:
        LOG_MSG_INFO2_1 ("Unrecognized TLV type (%d)", type);

        /* No other TLVs expected in this req */
        errval = QMI_ERR_MALFORMED_MSG;
        goto send_result;
    }

    if(got_v_in_reqd == TRUE)
    {
      /* since don't want to parse more or else the value protion is pulled out */
      break;
    }

    /* don't pull up, since need to send dsm_item to eap */
  }

  if( (auth_cl_sp->eap_info.eap_handle == EAP_INVALID_HANDLE) ||
      (auth_cl_sp->eap_info.pending_req_cmd_buf != NULL) )
  {
    LOG_MSG_ERROR_1("Got EAP req packet with Invalid handle or another pending req: 0x%x. Reject",
                    (auth_cl_sp->eap_info.pending_req_cmd_buf == NULL)?0:auth_cl_sp->eap_info.pending_req_cmd_buf);
    dsm_free_packet(sdu_in);
    errval = QMI_ERR_INVALID_OPERATION;
    goto send_result;
  }

  eap_input(auth_cl_sp->eap_info.eap_handle, sdu_in);
  auth_cl_sp->eap_info.pending_req_cmd_buf = cmd_buf_p;

  return QMI_SVC_RESPONSE_PENDING;

send_result: 
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();
  
  return response;
  
} /* qmi_authi_send_eap_packet() */


/*===========================================================================
  FUNCTION QMI_AUTHI_GET_EAP_SESSION_KEYS()

  DESCRIPTION
    To get the session keys.
        
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_get_eap_session_keys
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *    response;
  qmi_authi_client_state_type * auth_cl_sp;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean            retval;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  errval = QMI_ERR_NONE;

  auth_cl_sp = (qmi_authi_client_state_type *) cl_sp;

  if(auth_cl_sp->eap_info.result != EAP_RESULT_SUCCESS)
  {
    errval = QMI_ERR_INVALID_OPERATION;
    goto send_result;
  }

  if( FALSE == qmi_svc_put_param_tlv(&response, 
                                     QMI_TYPE_REQUIRED_PARAMETERS,
                                     auth_cl_sp->eap_info.msk_len,
                                     auth_cl_sp->eap_info.msk) )
  {
    LOG_MSG_INFO2_0("Unable to generate EAP Result indication!");
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }
 
send_result:  
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();
  
  return response;
} /* qmi_authi_get_eap_session_keys() */



/*===========================================================================
  FUNCTION QMI_AUTHI_END_EAP_SESSION

  DESCRIPTION
    To end the EAP session.
        
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_end_eap_session
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *    response;
  qmi_authi_client_state_type  *  auth_cl_sp;
  qmi_transaction_type *  x_p;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean            retval;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  errval = QMI_ERR_NONE;

  auth_cl_sp = (qmi_authi_client_state_type *) cl_sp;

  if(auth_cl_sp->eap_info.eap_handle == EAP_INVALID_HANDLE)
  {
    errval = QMI_ERR_INVALID_OPERATION;
    goto send_result;
  }

  eap_delete_instance(auth_cl_sp->eap_info.eap_handle);

  /* Free pending req if any */
  if(auth_cl_sp->eap_info.pending_req_cmd_buf != NULL)
  {
    x_p = auth_cl_sp->eap_info.pending_req_cmd_buf->x_p;
    qmi_authi_free_transaction(&x_p);
  }

  /* Clear Eap info */
  memset(&(auth_cl_sp->eap_info), 0, sizeof(auth_cl_sp->eap_info));
  auth_cl_sp->eap_info.eap_handle = EAP_INVALID_HANDLE;
  auth_cl_sp->eap_info.result = EAP_RESULT_FAILURE;


send_result:  
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();
  
  return response;
} /* qmi_authi_end_eap_session() */

/*===========================================================================
  FUNCTION QMI_AUTH_AKA_RESULT_IND_CBACK()

  DESCRIPTION
    Process the AKA callback and send the command to QMI taks to send the
    indication to the client.

  PARAMETERS
    aka_handle   - aka handle
    status       - result of the aka algorithm
    digest       - Digest data
    digest_len   - the length of the digest data
    aka_data     - the AKA response data
    aka_data_len - the length of the AKA response data
    user_data    - the user_data passed into the start_aka_algo fcn

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_auth_aka_result_ind_cback
(
  ps_utils_aka_handle_type       aka_handle,
  ps_utils_aka_status_enum_type  status,
  uint8                         *digest,
  uint8                          digest_len,
  uint8                         *aka_data,
  uint8                          aka_data_len,
  void                          *user_data
)
{
  qmi_authi_aka_result_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(user_data != NULL);

  cmd_ptr = (qmi_authi_aka_result_cmd_buf_type *)qmi_authi_get_cmd_buf(
  	          QMI_CMD_SEND_AKA_ALGO_RESULT,__LINE__);
  if( cmd_ptr == NULL)
  {
    return;
  }

  memset(cmd_ptr, 0, sizeof(qmi_authi_aka_result_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_SEND_AKA_ALGO_RESULT;
  cmd_ptr->handle        = aka_handle;
  cmd_ptr->status        = status;
  cmd_ptr->digest_len    = digest_len;
  cmd_ptr->aka_data_len  = aka_data_len;
  cmd_ptr->user_data     = user_data;  

  memscpy(cmd_ptr->digest, digest_len, digest, digest_len);
  memscpy(cmd_ptr->aka_data, aka_data_len, aka_data, aka_data_len);

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
} /* qmi_auth_aka_result_ind_cback() */

/*===========================================================================
  FUNCTION QMI_AUTHI_RUN_AKA_ALGO()

  DESCRIPTION
    Gives the name of the device manufacturer.
    
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_run_aka_algo
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  void *             value;
  dsm_item_type *    response = NULL;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean            retval;
  ps_utils_aka_ver_enum_type    aka_ver = 0;
  uint8    aka_params[2*sizeof(uint8)+
                      QMI_AUTH_MAX_AKA_RAND_LEN_SIZE +
	              QMI_AUTH_MAX_AKA_AUTN_LEN_SIZE] = {0};
  uint8    autn_len_offset;
  uint8    autn_offset;
  ps_utils_aka_handle_type aka_handle;
  uint8    type;
  uint16   len = 0, aka_params_len = 0;
  boolean  got_v_in_reqd = FALSE;
  boolean  got_v_in_aka_params = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);
  
  errval = QMI_ERR_NONE;

  while (*sdu_in)
  {
    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    // special value 0 = variable length or don't care (unrecognzied TLV)
    switch (type)
    {
      case QMI_TYPE_REQUIRED_PARAMETERS:
	value = (void *) &aka_ver;
        if(sizeof(aka_ver) != len)
        {
          LOG_MSG_INFO2_2("Invalid TLV len (%d) for type (%d)", len, type);
          errval = QMI_ERR_MALFORMED_MSG;
          goto send_result;
        }
        got_v_in_reqd = TRUE;
        break;

      case AUTHI_PRM_TYPE_AKA_PARAMS:
        if(len > sizeof(aka_params))
        {
          LOG_MSG_INFO2_2("TLV len (%d) too long for type (%d)", len, type);
          errval = QMI_ERR_ARG_TOO_LONG;
          goto send_result;
        }
        value = (void *) aka_params;
        aka_params_len = len;
        got_v_in_aka_params = TRUE;
        break;

     default:
        LOG_MSG_INFO2_1 ("Unrecognized TLV type (%d)", type);

        /* No other TLVs expected in this req */
        errval = QMI_ERR_MALFORMED_MSG;
        goto send_result;
    }

    /*-----------------------------------------------------------------------
      If type is unrecognized, value will be NULL, and dsm_pullup will 
      free the unrecognized value, i.e. we skip past unrecognized TLVs
    -----------------------------------------------------------------------*/
    if(len != PS_UTILS_DSM_PULLUP( sdu_in, value, len))
    {
      errval = QMI_ERR_MALFORMED_MSG;
      goto send_result;
    }
  } /* end while */


  if(got_v_in_reqd != TRUE)
  {
    /* Missing the required TLV, fail */
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  if(aka_ver >= PS_UTILS_AKA_MAX_VER)
  {
    LOG_MSG_INFO2_1 ("Invalid AKA version: %d in TLV", aka_ver);
    errval = QMI_ERR_INVALID_ARG;
    goto send_result; 
  }

  if(aka_ver == PS_UTILS_AKA_V1 || aka_ver == PS_UTILS_AKA_V2)
  {
    if (got_v_in_aka_params != TRUE)
    {
      /* This optional TLV must be present for AKA_V1 and AKA_V2 */
      LOG_MSG_INFO2_0 ("Missing AKA Params TLV");
      errval = QMI_ERR_MISSING_ARG;
      goto send_result;
    }
    /* Determine offsets for parameters in TLV */
    /*------------------------------------------------
     * | rand_len |   rand   | autn_len |   autn   |
     * |  uint8   | variable |  uint8   | variable |
     *-----------------------------------------------*/
    autn_len_offset =   sizeof(uint8) + aka_params[0];
    autn_offset     = 2*sizeof(uint8) + aka_params[0];    
 
    /* Validate some length values */
    if (aka_params_len != (2 * sizeof(uint8) + aka_params[0] + aka_params[autn_len_offset]))
    {
      LOG_MSG_INFO2_3 ("TLV Length %d does not match rand_len %d and autn_len %d",
                       aka_params_len,
                       aka_params[0],
                       aka_params[autn_len_offset]);
      errval = QMI_ERR_MALFORMED_MSG;
      goto send_result;
    }
 
    /* Call AKA processing function */
    if(-1 == (aka_handle = ps_utils_run_aka_algo
                           (
                             aka_ver,
                             &aka_params[1], /* rand     */
                             aka_params[0],  /* rand_len */
                             &aka_params[autn_offset],
                             aka_params[autn_len_offset],
                             (ps_utils_aka_callback_type) qmi_auth_aka_result_ind_cback,
                             cl_sp
                           )))
    {
      LOG_MSG_INFO2_0 ("Unknown QMI AKA processing error!");
      errval = QMI_ERR_INTERNAL;
      goto send_result;
    }

    /* Return AKA_V1/V2 handle to client */
    if( FALSE == qmi_svc_put_param_tlv(&response, 
                                       QMI_TYPE_REQUIRED_PARAMETERS,
                                       (uint16) sizeof(aka_handle),
                                       (void*)  &aka_handle))
    {
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
    }
  }

send_result: 
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();
  
  return response;
  
} /* qmi_authi_run_aka_algo() */

/*===========================================================================
  FUNCTION QMI_AUTHI_SEND_AKA_RESULT_IND()

  DESCRIPTION
    Send the AKA result to the client.

  PARAMETERS
    cmd_ptr

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_send_aka_result_ind
(
  qmi_authi_aka_result_cmd_buf_type *  cmd_ptr
)
{
  dsm_item_type  *  ind = NULL;
  uint8             tlv_type;
  uint16            tlv_len;
  uint16            opt_tlv_len;
  qmi_authi_client_state_type * auth_cl_sp = NULL;
  qmi_framework_msg_hdr_type    framework_msg_hdr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(cmd_ptr != NULL);
  auth_cl_sp = (qmi_authi_client_state_type *)cmd_ptr->user_data;

  /* Only populate the optional TLV if the status is success */
  if(cmd_ptr->status == PS_UTILS_AKA_SUCCESS)
  {
    /* Push down the optional TLV for AKAv1/v2 */
    opt_tlv_len = sizeof(cmd_ptr->digest_len) +
                  sizeof(cmd_ptr->aka_data_len) +
                  cmd_ptr->aka_data_len +
                  cmd_ptr->digest_len;

    tlv_type = AUTHI_PRM_TYPE_AKA_RESULT_DATA;

    if ( (QMI_SVC_PKT_PUSH(&ind, cmd_ptr->aka_data, 
                           cmd_ptr->aka_data_len)) &&
         (QMI_SVC_PKT_PUSH(&ind, &cmd_ptr->aka_data_len, sizeof(uint8))) &&
         (QMI_SVC_PKT_PUSH(&ind, cmd_ptr->digest, 
                           cmd_ptr->digest_len)) &&
         (QMI_SVC_PKT_PUSH(&ind, &cmd_ptr->digest_len, sizeof(uint8))) &&
         (QMI_SVC_PKT_PUSH(&ind, &opt_tlv_len, sizeof(uint16))) &&
         (QMI_SVC_PKT_PUSH(&ind, &tlv_type, sizeof(uint8))) 
         == FALSE )
    {
      LOG_MSG_INFO2_0("Unable to generate AKA Result indication data!");
      dsm_free_packet(&ind);
      return;
    }
  } /* opt TLV */

  /* Push down the two required params: aka_handle (4 bytes) and status (1 byte) */
  tlv_len = (uint16) (sizeof(uint8)+sizeof(uint32));
  tlv_type = QMI_TYPE_REQUIRED_PARAMETERS;

  if ( (QMI_SVC_PKT_PUSH(&ind, &cmd_ptr->status, sizeof(uint8))) &&
       (QMI_SVC_PKT_PUSH(&ind, &cmd_ptr->handle, sizeof(uint32))) &&
       (QMI_SVC_PKT_PUSH(&ind, &tlv_len, sizeof(uint16))) &&
       (QMI_SVC_PKT_PUSH(&ind, &tlv_type, sizeof(uint8))) 
       == FALSE )
  {
    LOG_MSG_INFO2_0("Unable to generate AKA Result indication req params!");
    dsm_free_packet(&ind);
    return;
  }

  /*-----------------------------------------------------------------------
    Fill the QMI Framework message header before sending Indication to
    Framework.
  -----------------------------------------------------------------------*/
  framework_msg_hdr.common_hdr.client_id = auth_cl_sp->common.clid;
  /* if unicast indication, qmi framework map the instance and no need to fill*/
  framework_msg_hdr.common_hdr.qmi_instance = AUTHI_INVALID_INSTANCE;
  framework_msg_hdr.common_hdr.service = QMUX_SERVICE_AUTH;
  /* for indication the transaction ID can be ignored */
  framework_msg_hdr.common_hdr.transaction_id = 0;
  framework_msg_hdr.msg_ctl_flag = QMI_FLAG_MSGTYPE_IND;
  framework_msg_hdr.msg_len = (uint16) dsm_length_packet(ind);
  if(FALSE == qmi_authi_send_indication( &framework_msg_hdr, 
                                         AUTHI_CMD_VAL_AKA_ALGO_RESULT_IND, 
                                         ind))
  {
    LOG_MSG_ERROR_INT_0("Unable to send qmi_authi_send_aka_result_ind!");
    dsm_free_packet(&ind);
  }
} /* qmi_authi_send_aka_result_ind */

/*===========================================================================
  FUNCTION QMI_AUTHI_SET_SUBSCRIPTION_BINDING()

  DESCRIPTION
    Binds user given subscription to this client

  PARAMETERS
    sp           : service state pointer
    cl_sp        : client state pointer
    cmd_buf_p    : the current command buffer
    sdu_in       : dsm_item containing incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_set_subscription_binding
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *    response;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean            retval;
#if (defined FEATURE_DUAL_SIM || defined FEATURE_TRIPLE_SIM)
  uint8    type;
  uint16   len;
  uint16   expected_len;
  uint32   subs_id;
  void *   value;
  qmi_authi_client_state_type *  auth_client_sp;
  boolean  got_v_in_reqd = FALSE;
#endif
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);
  
  response = NULL;
  errval = QMI_ERR_NONE;

#if (defined FEATURE_DUAL_SIM || defined FEATURE_TRIPLE_SIM)
  auth_client_sp = (qmi_authi_client_state_type *)cl_sp;

  while (*sdu_in)
  {
    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    // special value 0 = variable length or don't care (unrecognzied TLV)
    expected_len = 0;
    value = NULL;
    switch (type)
    {
      case QMI_TYPE_REQUIRED_PARAMETERS:
        expected_len = sizeof(subs_id);
        value = &subs_id;
        got_v_in_reqd = TRUE;
        break;

      default:
        LOG_MSG_INFO2_1( "Unrecognized TLV type (%d)",type);
        break;
    }
    if(!qmi_svc_validate_type_length( type,
                                          len,
                                          expected_len,
                                          FILE_ID_DS_QMI_AUTH,__LINE__) )
    {
      errval = QMI_ERR_MALFORMED_MSG;
      goto send_result;
    }

    /*-----------------------------------------------------------------------
      If type is unrecognized, value will be NULL, and dsm_pullup will
      free the unrecognized value, i.e. we skip past unrecognized TLVs
    -----------------------------------------------------------------------*/
    if(len != PS_UTILS_DSM_PULLUP( sdu_in, value, len))
    {
      errval = QMI_ERR_MALFORMED_MSG;
      goto send_result;
    }
  }
  
  if(!got_v_in_reqd)
  {
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  if(!(subs_id < AUTHI_BIND_SUBS_MAX))
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }
  
  #ifndef FEATURE_TRIPLE_SIM
    if(subs_id == AUTHI_BIND_SUBS_TERTIARY)
    {
      errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
      goto send_result;
    }
  #endif
  auth_client_sp->subscription_id = (qmi_authi_client_subs_e_type)subs_id ;

#else
  errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
  goto send_result;
#endif  
    
  send_result:
    result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                     : QMI_RESULT_FAILURE);

    retval = qmi_svc_put_result_tlv(&response, result, errval);
    CHECK_RETVAL();

    return response;

} /* qmi_authi_bind_subscription */

/*===========================================================================
  FUNCTION QMI_AUTHI_GET_BIND_SUBSCRIPTION()

  DESCRIPTION
    Returns already bound  subscription to this client

  PARAMETERS
    sp           : service state pointer
    cl_sp        : client state pointer
    cmd_buf_p    : the current command buffer
    sdu_in       : dsm_item containing incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_get_bind_subscription
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *    response;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean            retval; 

#if (defined FEATURE_DUAL_SIM || defined FEATURE_TRIPLE_SIM)
  qmi_authi_client_state_type *  auth_client_sp;
  uint32 subscription_id;
#endif
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  errval = QMI_ERR_NONE;

#if (defined FEATURE_DUAL_SIM || defined FEATURE_TRIPLE_SIM)
  auth_client_sp = (qmi_authi_client_state_type *)cl_sp;

  if(auth_client_sp == NULL)
  {
    LOG_MSG_ERROR_INT_0("client state pointer is NULL");
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }
  if( auth_client_sp->subscription_id >= AUTHI_BIND_SUBS_MAX )
  {
    LOG_MSG_ERROR_INT_1("Invalid subscription id stored for client d",
                     auth_client_sp->common.clid);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }
  subscription_id = auth_client_sp->subscription_id;
  /*-------------------------------------------------------------------------
    Build the response from back to front
  -------------------------------------------------------------------------*/
  if(FALSE == qmi_svc_put_param_tlv(&response, 
                                    AUTHI_PRM_TYPE_BIND_SUBSCRIPTION,
                                    sizeof(subscription_id),
                                    &subscription_id))
  {
    errval = QMI_ERR_NO_MEMORY;
    dsm_free_packet(&response);
  }
#else
  errval = QMI_ERR_OP_DEVICE_UNSUPPORTED;
  goto send_result;
#endif
  
  send_result:
   result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                    : QMI_RESULT_FAILURE);
   retval = qmi_svc_put_result_tlv(&response, result, errval);
   CHECK_RETVAL();
       
   return response;

}/* qmi_authi_get_bind_subscription */


/*===========================================================================
  FUNCTION QMI_AUTHI_INDICATION_REGISTER()

  DESCRIPTION
    To set state for indicaiton registeration

  PARAMETERS
    sp           : service state pointer
    cmd_buf_p    : the current command buffer
    cl_sp        : client state pointer
    sdu_in       : dsm_item containing incoming request

  RETURN VALUE
    ptr to response

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_indication_register
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
    dsm_item_type *    response = NULL;
    qmi_error_e_type   errval   = QMI_ERR_NONE;
    qmi_result_e_type  result;
    boolean 	       retval; 
    qmi_authi_client_state_type * auth_client_sp = NULL;
    PACKED struct PACKED_POST
    {
      boolean report_eap_notification_code;
    } v_in1;
    PACKED struct PACKED_POST
    {
      boolean report_eap_client_err_code;
    } v_in2;
    PACKED struct PACKED_POST
    {
      boolean report_eap_auth_rej;
    } v_in3;
    PACKED struct PACKED_POST
    {
      boolean report_imsi_public_key_req;
    } v_in4;
    PACKED struct PACKED_POST
    {
      boolean report_retr_ca_cert_req;
    } v_in5;

    uint8    type;
    uint16   len;
    uint16   expected_len;
    void *   value;
    boolean  got_v_in1 = FALSE;
    boolean  got_v_in2 = FALSE;
    boolean  got_v_in3 = FALSE;
    boolean  got_v_in4 = FALSE;
    boolean  got_v_in5 = FALSE;
    ps_sys_event_ca_cert_retrieval_support  ca_cert_support;
    int16      ps_errno;
    int16       ps_result;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
    DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

    auth_client_sp = (qmi_authi_client_state_type *)cl_sp;
    while (*sdu_in)
    {
       if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
       {
         continue;
       }

       expected_len = 0;
       value = NULL;
       switch (type)
       {
         case AUTHI_PRM_TYPE_EAP_NOTIFICATION_CODE:
            expected_len = sizeof(v_in1);
            value = &v_in1;
            got_v_in1 = TRUE;
          break;

          case AUTHI_PRM_TYPE_EAP_CLIENT_ERR_CODE:
             expected_len = sizeof(v_in2);
             value = &v_in2;
             got_v_in2 = TRUE;
           break;

           case AUTHI_PRM_TYPE_EAP_AUTH_REJ:
              expected_len = sizeof(v_in3);
              value = &v_in3;
              got_v_in3 = TRUE;
            break;

           case AUTHI_PRM_TYPE_EAP_IMSI_PUBLIC_KEY_REQ:
              expected_len = sizeof(v_in4);
              value = &v_in4;
              got_v_in4 = TRUE;
            break;

           case AUTHI_PRM_TYPE_RETR_CA_CERTIFICATE_REQ:
              expected_len = sizeof(v_in5);
              value = &v_in5;
              got_v_in5 = TRUE;
            break;

          default:
             LOG_MSG_INFO2_1( "Unrecognized TLV type (%d)",type);
           break;
       }
	
       if(!qmi_svc_validate_type_length( type,
                                         len,
                                         expected_len,
                                         FILE_ID_DS_QMI_AUTH,__LINE__) )

       {
          errval = QMI_ERR_MALFORMED_MSG;
          goto send_result;
       }

      /*-----------------------------------------------------------------------
        If type is unrecognized, value will be NULL, and dsm_pullup will
        free the unrecognized value, i.e. we skip past unrecognized TLVs
        -----------------------------------------------------------------------*/
       if(len != PS_UTILS_DSM_PULLUP( sdu_in, value, len))
       {
          errval = QMI_ERR_MALFORMED_MSG;
          goto send_result;
       }
    }

    /*-------------------------------------------------------------------------
     If ALL optional TLVs are absent return error or else carry on and
      process the request.
    -------------------------------------------------------------------------*/	 
    if(!(got_v_in1 || got_v_in2 || got_v_in3 || got_v_in4 || got_v_in5))
    {
      errval = QMI_ERR_MISSING_ARG;
      goto send_result;
    }

    if(got_v_in1)
    {
    /*Process the report eap notification code value*/
    if(v_in1.report_eap_notification_code == TRUE)
    {
      auth_client_sp->report_status.report_eap_notification_code = TRUE;
    }
    else if(v_in1.report_eap_notification_code == FALSE)
    {
      auth_client_sp->report_status.report_eap_notification_code = FALSE;
    }
    else
    {
      errval = QMI_ERR_INVALID_ARG;
      goto send_result;		  
    }
    }
    if(got_v_in2)
    {
      /*Process the report eap client error code */
      if(v_in2.report_eap_client_err_code == TRUE)
      {
        auth_client_sp->report_status.report_eap_err_ind_code = TRUE;
      }
      else if(v_in2.report_eap_client_err_code == FALSE)
      {
        auth_client_sp->report_status.report_eap_err_ind_code = FALSE;
      }
      else
      {
        errval = QMI_ERR_INVALID_ARG;
        goto send_result;
      }
    }
    if(got_v_in3)
    {
      /*Process the report eap auth rej ind*/
      if(v_in3.report_eap_auth_rej == TRUE)
      {
        auth_client_sp->report_status.report_eap_auth_rej = TRUE;
      }
      else if(v_in3.report_eap_auth_rej == FALSE)
      {
        auth_client_sp->report_status.report_eap_auth_rej = FALSE;
      }
      else
      {
        errval = QMI_ERR_INVALID_ARG;
        goto send_result;
      }
    }
    if(got_v_in4)
    {
      /*Process the report IMSI public key request */
      if(v_in4.report_imsi_public_key_req == TRUE)
      {
        auth_client_sp->report_status.report_imsi_public_key_req = TRUE;
      }
      else if(v_in4.report_imsi_public_key_req == FALSE)
      {
        auth_client_sp->report_status.report_imsi_public_key_req = FALSE;
      }
      else
      {
        errval = QMI_ERR_INVALID_ARG;
        goto send_result;
      }
    }
    if(got_v_in5)
    {
      /*Process the report CA cert request */
      if(v_in5.report_retr_ca_cert_req == TRUE)
      {
        memset(&ca_cert_support, 0, sizeof(ps_sys_event_ca_cert_retrieval_support));

        /* Mark Auth service supports CA certificate*/
        auth_client_sp->report_status.report_retr_ca_cert_req = TRUE;
        qmi_authi_global.ca_cert_supported = TRUE;
        qmi_authi_global.ca_client_ptr= (void *)auth_client_sp;

        /* Invoke event to notify modem clients */
        ca_cert_support = TRUE;  
        ps_result = ps_sys_event_ind_ex(PS_SYS_TECH_ALL,
                                        PS_SYS_EVENT_CA_CERT_RETRIEVAL_SUPPORT,
                                        (ps_sys_subscription_enum_type)auth_client_sp->subscription_id,
                                        (void *)&ca_cert_support,
                                        &ps_errno);
        if ( ps_result != 0)
        {
          errval = QMI_ERR_INTERNAL;
          goto send_result;
        }
      }
      else if(v_in5.report_retr_ca_cert_req == FALSE)
      {
        auth_client_sp->report_status.report_retr_ca_cert_req = FALSE;
      }
      else
      {
        errval = QMI_ERR_INVALID_ARG;
        goto send_result;
      }
    }


send_result:
    result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
	                                 : QMI_RESULT_FAILURE);

    retval = qmi_svc_put_result_tlv(&response, result, errval);
    CHECK_RETVAL();

    return response;
}

/*===========================================================================
FUNCTION QMI_AUTHI_INIT_CB()

  DESCRIPTION
  Function gets called from framework whenever the QMI AUTH service registers with
  framework. This function posts a DCC command for the service to handle this
  callback.
    
  PARAMETERS
  num_instances : getting Num of QMI instances at run time.

  RETURN VALUE
  None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_init_cb
(
  uint16 num_instances
)
{
  qmi_authi_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(QMI_CMD_AUTH_INIT_CB,__LINE__);
  if( cmd_ptr == NULL)
  {
    return;
  }

  memset(cmd_ptr, 0, sizeof(qmi_authi_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_AUTH_INIT_CB;
  cmd_ptr->data.init_cb.num_instances = num_instances;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);

} /* qmi_authi_init_cb() */

/*===========================================================================
FUNCTION QMI_AUTHI_ALLOC_CLID_CB()

DESCRIPTION
  Function gets called from framework whenever a clid is allocated by the
  framework for QMI AUTH service. This function posts a DCC command for the
  service to handle this callback.

PARAMETERS
  common_msg_hdr  : QMI Framework common msg hdr

RETURN VALUE
  Boolean : TRUE if the clid set properly
          : FALSE, otherwise
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static boolean qmi_authi_alloc_clid_cb
(  
  qmi_framework_common_msg_hdr_type*   common_msg_hdr  
)
  {
  qmi_authi_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_AUTH_ASSERT(common_msg_hdr != NULL);

  LOG_MSG_INFO1_2("QMI AUTH alloc clid cb: clid:%d, QMI Instances:%d",
                  common_msg_hdr->client_id, common_msg_hdr->qmi_instance);

  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(QMI_CMD_AUTH_ALLOC_CLID_CB,__LINE__);
  if( cmd_ptr == NULL)
    {
    return FALSE;
    }

  /*-------------------------------------------------------------------------
    copy the framework common message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_authi_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_AUTH_ALLOC_CLID_CB;
  cmd_ptr->data.clid_cb.common_msg_hdr.service = common_msg_hdr->service;
  cmd_ptr->data.clid_cb.common_msg_hdr.client_id = common_msg_hdr->client_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.transaction_id = common_msg_hdr->transaction_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.qmi_instance = common_msg_hdr->qmi_instance;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
  return TRUE;
} /* qmi_authi_alloc_clid_cb */

/*===========================================================================
FUNCTION QMI_AUTHI_DEALLOC_CLID_CB()

DESCRIPTION 
  Function gets called from framework whenever a clid is deallocated by the
  framework for QMI AUTH service. This function posts a DCC command for the
  service to handle this callback.

PARAMETERS
  common_msg_hdr  : QMI Framework common msg hdr

RETURN VALUE
  None
 
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static void qmi_authi_dealloc_clid_cb
(  
  qmi_framework_common_msg_hdr_type*   common_msg_hdr  
)
{
  qmi_authi_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(common_msg_hdr != NULL);
  LOG_MSG_INFO1_2("QMI AUTH dealloc clid cb: clid:%d, QMI Instances:%d",
                  common_msg_hdr->client_id, common_msg_hdr->qmi_instance);

  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(QMI_CMD_AUTH_DEALLOC_CLID_CB,__LINE__);
  if( cmd_ptr == NULL)
  {
    return;
  }

  /*-------------------------------------------------------------------------
    copy the framework common message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_authi_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_AUTH_DEALLOC_CLID_CB;
  cmd_ptr->data.clid_cb.common_msg_hdr.service = common_msg_hdr->service;
  cmd_ptr->data.clid_cb.common_msg_hdr.client_id = common_msg_hdr->client_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.transaction_id = common_msg_hdr->transaction_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.qmi_instance = common_msg_hdr->qmi_instance;

  /*-------------------------------------------------------------------------
    Positing command to QMI Service task to handle the clid callback
  -------------------------------------------------------------------------*/
  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
} /* qmi_authi_dealloc_clid_cb */

/*===========================================================================
  FUNCTION FRAMEWORK_AUTHI_CMD_HDLR_CB()

  DESCRIPTION
    Function gets called from framework whenever a new QMI AUTH request
    message is received. This function posts a DCC command for the
    service to handle this callback.

  PARAMETERS
    msg_hdr  :  msg hdr
    sdu : dsm item
 
  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void  qmi_authi_cmd_hdlr_cb
( 
  qmi_framework_msg_hdr_type * msg_hdr,
  dsm_item_type             ** sdu 
)
{
  qmi_authi_cmd_buf_type *cmd_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_AUTH_ASSERT(msg_hdr != NULL);
  LOG_MSG_INFO1_3("qmi_authi_cmd_hdlr_cb: clid:%d, QMI Instances:%d, tx_id: %d",
                  msg_hdr->common_hdr.client_id,
                  msg_hdr->common_hdr.qmi_instance,
                  msg_hdr->common_hdr.transaction_id);
  
  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(QMI_CMD_AUTH_CMD_HDLR_CB,__LINE__);
  if( cmd_ptr == NULL)
  {
    return;
  }

  /*-------------------------------------------------------------------------
    copy the framework message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_authi_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_AUTH_CMD_HDLR_CB;
  cmd_ptr->data.cmd_hdlr_cb.msg_hdr.common_hdr.service = 
                                        msg_hdr->common_hdr.service;
  cmd_ptr->data.cmd_hdlr_cb.msg_hdr.common_hdr.client_id = 
                                        msg_hdr->common_hdr.client_id;
  cmd_ptr->data.cmd_hdlr_cb.msg_hdr.common_hdr.transaction_id = 
                                        msg_hdr->common_hdr.transaction_id;
  cmd_ptr->data.cmd_hdlr_cb.msg_hdr.common_hdr.qmi_instance = 
                                        msg_hdr->common_hdr.qmi_instance;
  cmd_ptr->data.cmd_hdlr_cb.msg_hdr.msg_ctl_flag = msg_hdr->msg_ctl_flag; 
  cmd_ptr->data.cmd_hdlr_cb.msg_hdr.msg_len = msg_hdr->msg_len; 
  cmd_ptr->data.cmd_hdlr_cb.sdu_in = *sdu;

  /*-------------------------------------------------------------------------
    Positing command to QMI Service task to handle the service request
  -------------------------------------------------------------------------*/
  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
} /* qmi_authi_cmd_hdlr_cb */

/*===========================================================================
FUNCTION QMI_AUTHI_ALLOC_CL_SP()

  DESCRIPTION
  This function gets called whenever a client state need to be allocated.
  The client state has allocated and deallocated dynamically at runtime.
    
  PARAMETERS
  clid: client ID

  RETURN VALUE
  qmi_authi_client_state_type * : ptr to QMI AUTH client state.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_authi_client_state_type * qmi_authi_alloc_cl_sp
(
  uint8 clid
)
{
  qmi_authi_client_state_type *  cl_sp = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_1( "qmi_authi_alloc_cl_sp clid :%d", clid );

  if ((clid == QMI_SVC_CLID_UNUSED ) ||
      (clid >= QMI_FRAMEWORK_SVC_MAX_CLIENTS ))
  {
    LOG_MSG_INFO2_1( "Invalid client ID clid :%d", clid );
    return cl_sp;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(cl_sp, sizeof(qmi_authi_client_state_type),
                                  qmi_authi_client_state_type*);

  qmi_auth_state.client[clid - 1] = cl_sp;
  return cl_sp;
} /* qmi_authi_alloc_cl_sp */

/*===========================================================================
FUNCTION QMI_AUTHI_DEALLOC_CL_SP()

DESCRIPTION
  This function gets called whenever a client state need to be deallocated.
  The client state is allocated and deallocated dynamically at runtime.

PARAMETERS
    clid: client ID

RETURN VALUE
  TRUE: client Id deallocated otherwise error.
 
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static boolean qmi_authi_dealloc_cl_sp
(
  uint8 clid
)
  {
  qmi_authi_client_state_type *  cl_sp = NULL;
  int16                          ps_result = DSS_SUCCESS;
  int16                          ps_errno = DS_ENOERR;
  boolean                        ca_cert_support = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_1( "qmi_authi_dealloc_cl_sp clid :%d", clid );

  if ((clid == QMI_SVC_CLID_UNUSED ) ||
      (clid >= QMI_FRAMEWORK_SVC_MAX_CLIENTS ))
  {
    LOG_MSG_INFO2_1( "Invalid client ID clid :%d", clid );
    return FALSE;
  }

  cl_sp = (qmi_authi_client_state_type *) qmi_auth_state.client[clid - 1];
  if( cl_sp == (qmi_authi_client_state_type *)qmi_authi_global.ca_client_ptr)
  {
    qmi_authi_global.ca_cert_supported = FALSE;
    qmi_authi_global.ca_client_ptr= NULL;
    ca_cert_support = FALSE;
    ps_result = ps_sys_event_ind_ex(PS_SYS_TECH_ALL,
                                    PS_SYS_EVENT_CA_CERT_RETRIEVAL_SUPPORT,
                                    (ps_sys_subscription_enum_type)cl_sp->subscription_id,
                                    (void *)&ca_cert_support,
                                    &ps_errno);
    if ( ps_result != 0)
    {
      LOG_MSG_INFO1_2("qmi_authi_dealloc_cl_sp removing CA support ERROR! result %d ps_errno %d",
                      ps_result,ps_errno);
    }
  }
  if(cl_sp)
  {
    q_destroy(&cl_sp->common.x_free_q);
    q_destroy(&cl_sp->common.cmd_free_q);
    q_destroy(&cl_sp->ca_cert_tx_q);

    PS_SYSTEM_HEAP_MEM_FREE(cl_sp);
    qmi_auth_state.client[clid - 1] = NULL;
  }
  else
  {
    LOG_MSG_ERROR_INT_0("Client state pointer is already NULL");
    return FALSE;
  }
  return TRUE;
} /* qmi_authi_dealloc_cl_sp */


/*===========================================================================
  FUNCTION QMI_AUTHI_INITIALIZE_CLIENT()

  DESCRIPTION
    Initialize the new client state for the given client state ptr.
    
  PARAMETERS
    cl_sp :  client state pointer
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_initialize_client
(
  qmi_authi_client_state_type * cl_sp
)
{
  qmi_authi_state_type *  auth_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(cl_sp != NULL);
  auth_sp = &qmi_auth_state;
  memset( cl_sp , 0, sizeof(qmi_authi_client_state_type) );

  LOG_MSG_INFO2_0( "qmi_authi_initialize_client " );
  cl_sp->instance     = AUTHI_INVALID_INSTANCE;
  cl_sp->common.clid  = QMI_SVC_CLID_UNUSED;
  cl_sp->service_id   = QMUX_SERVICE_AUTH;

  ds_qmi_fw_common_cl_init(auth_sp, &cl_sp->common);

  cl_sp->eap_info.eap_handle = EAP_INVALID_HANDLE;
  cl_sp->eap_info.result = EAP_RESULT_FAILURE;

#if (defined FEATURE_DUAL_SIM || defined FEATURE_TRIPLE_SIM)
  cl_sp->subscription_id = AUTHI_BIND_SUBS_PRIMARY;
#endif

/*------------------------------------------------------------------------
  Initialize CA certificate queue
-------------------------------------------------------------------------*/
(void)q_init(&(cl_sp->ca_cert_tx_q));

} /* qmi_authi_initialize_client() */


/*===========================================================================
  FUNCTION QMI_AUTHI_RESET_CLIENT()

  DESCRIPTION
    Resets the state for the given client
    
  PARAMETERS
    cl_sp :  client state pointer
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_reset_client
(
  void * cl_sp
)
{
  qmi_authi_client_state_type * auth_cl_sp;
  int i;
  qmi_transaction_type *  x_p;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  auth_cl_sp = (qmi_authi_client_state_type *) cl_sp;

  LOG_MSG_INFO2_0( "qmi_authi_reset_client " );

  if(auth_cl_sp->eap_info.eap_handle != EAP_INVALID_HANDLE)
  {
    LOG_MSG_INFO1_1("Delete EAP instance since client (%d) reset",
                    auth_cl_sp->common.clid);
    eap_delete_instance(auth_cl_sp->eap_info.eap_handle);
  }

  if(auth_cl_sp->eap_info.pending_req_cmd_buf != NULL)
  {
    x_p = auth_cl_sp->eap_info.pending_req_cmd_buf->x_p;
    qmi_authi_free_transaction(&x_p);
  }

  /* Clear Eap info */
  memset(&(auth_cl_sp->eap_info), 0, sizeof(auth_cl_sp->eap_info));
  auth_cl_sp->eap_info.eap_handle = EAP_INVALID_HANDLE;
  auth_cl_sp->eap_info.result = EAP_RESULT_FAILURE;

  /*-------------------------------------------------------------------------
    Free resources for client
  -------------------------------------------------------------------------*/
  /*-------------------------------------------------------------------------
    q_init() is used here as a way to clear the queue and not as a way to 
    initialize the queue.
  -------------------------------------------------------------------------*/ 
  (void) q_init( &auth_cl_sp->common.x_free_q );
  for( i = 0; i < MAX_PENDING_TRANSACTIONS; i++ )
  {
    x_p = &auth_cl_sp->common.transactions[i];
    qmi_authi_free_transaction( &x_p );
  }
   
  /*-------------------------------------------------------------------------
    clear event reporting
  -------------------------------------------------------------------------*/
} /* qmi_authi_reset_client() */

/*===========================================================================
  FUNCTION QMI_ATI_PROCESS_CMD()

  DESCRIPTION
    This function processes a QMI AT command or event.

    It is called by the QMI command handler and will dispatch the
    associated command/event handler function.

  PARAMETERS
    cmd_ptr:  private data buffer containing the QMI AT command
              information.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI AT must already have been initialized

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_process_cmd
(
  void *cmd_ptr
)
{
  qmi_authi_cmd_buf_type *cmd_buf_ptr = NULL;
/*-------------------------------------------------------------------------*/

  DS_QMI_AUTH_ASSERT(cmd_ptr != NULL);
  cmd_buf_ptr = (qmi_authi_cmd_buf_type *)cmd_ptr;

  switch(cmd_buf_ptr->cmd_id)
  {
    case QMI_CMD_EAP_SIM_AKA_TASK_SWITCH_EV:
    {
      qmi_authi_process_eap_sim_aka_task_switch(cmd_buf_ptr);
      break;
    }

    case QMI_CMD_SEND_EAP_RESP:
    {
      qmi_authi_send_eap_resp(cmd_buf_ptr);
      break;
    }

    case QMI_CMD_AUTH_INIT_CB:
    {
      qmi_authi_process_svc_init(cmd_buf_ptr->data.init_cb.num_instances);
      break;
    }

    case QMI_CMD_AUTH_ALLOC_CLID_CB:
    {
      qmi_authi_process_alloc_clid(&cmd_buf_ptr->data.clid_cb.common_msg_hdr);
      break;
    }

    case QMI_CMD_AUTH_DEALLOC_CLID_CB:
    {
      qmi_authi_process_dealloc_clid(&cmd_buf_ptr->data.clid_cb.common_msg_hdr);
      break;
    }

    case QMI_CMD_AUTH_CMD_HDLR_CB:
    {
      qmi_authi_process_cmd_hdlr(&cmd_buf_ptr->data.cmd_hdlr_cb.msg_hdr,
                                    cmd_buf_ptr->data.cmd_hdlr_cb.sdu_in );
      break;
    }

    case QMI_CMD_SEND_EAP_RESULT:
    {
      qmi_authi_eap_result_cmd_buf_type *eap_cmd_buf_ptr =
        (qmi_authi_eap_result_cmd_buf_type *)cmd_ptr;
      qmi_authi_send_eap_result_ind(eap_cmd_buf_ptr);
      memset(eap_cmd_buf_ptr, 0, sizeof(qmi_authi_eap_result_cmd_buf_type));
      break;
    }
      
    case QMI_CMD_SEND_AKA_ALGO_RESULT:
    {
      qmi_authi_aka_result_cmd_buf_type *aka_cmd_buf_ptr =
        (qmi_authi_aka_result_cmd_buf_type *)cmd_ptr;
      qmi_authi_send_aka_result_ind(aka_cmd_buf_ptr);
      break;
    }
    case QMI_CMD_EAP_INDICATION_CB:
    { 
      qmi_authi_process_eap_indicaiton(cmd_buf_ptr);
      break;
    }
    case QMI_CMD_AUTH_RETRIEVE_IMSI_PUBLIC_KEY:
    {
      qmi_authi_send_retrieve_imsi_public_key_ind(cmd_buf_ptr);
      break;
    }
    
    case QMI_CMD_AUTH_RETR_CA_CERT_EVT:
    {
      qmi_authi_retr_ca_cert_ind(cmd_buf_ptr);
      break;
    }

    case QMI_CMD_AUTH_CA_CERT_PROCESS_INTERNAL:
    {
      qmi_authi_ca_cert_cmd_buf_type *ca_cmd_buf_ptr =
        (qmi_authi_ca_cert_cmd_buf_type *)cmd_ptr;
      qmi_authi_process_ca_cert(ca_cmd_buf_ptr);
      break;
    }

    default:
      LOG_MSG_ERROR_INT_1("Unexpected cmd(d) recd in QMI AT cmd handler",
                      cmd_buf_ptr->cmd_id);
      DS_QMI_AUTH_ASSERT(0);
  }
  qmi_authi_free_cmd_buf(cmd_ptr);
  return;
} /* qmi_authi_process_cmd() */

/*===========================================================================
  FUNCTION QMI_AUTHI_GET_CMD_BUF()

  DESCRIPTION
    Allocate and assign a QMI AT command buffer from the PS MEM heap
    based on the QMI CMD type
    
  PARAMETERS
    cmd - QMI command type
    line_num - to indicate the line num of caller.
   
  RETURN VALUE
    cmd_buf_ptr - Pointer to the allocated command buffer

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void *qmi_authi_get_cmd_buf
(
  qmi_cmd_id_e_type cmd_id,
  uint16 line_num
)
{
  LOG_MSG_INFO1_2("cmd_id = %d line_num = %d",cmd_id,line_num);
  switch (cmd_id)
  {
    case QMI_CMD_EAP_SIM_AKA_TASK_SWITCH_EV:
    case QMI_CMD_SEND_EAP_RESP:
    case QMI_CMD_AUTH_INIT_CB:
    case QMI_CMD_AUTH_ALLOC_CLID_CB:
    case QMI_CMD_AUTH_DEALLOC_CLID_CB:
    case QMI_CMD_AUTH_CMD_HDLR_CB:
    case QMI_CMD_EAP_INDICATION_CB:
    case QMI_CMD_AUTH_RETRIEVE_IMSI_PUBLIC_KEY:
    case QMI_CMD_AUTH_RETR_CA_CERT_EVT:
    { 
      qmi_authi_cmd_buf_type *cmd_buf_ptr;
      PS_SYSTEM_HEAP_MEM_ALLOC(cmd_buf_ptr, sizeof(qmi_authi_cmd_buf_type),
                               qmi_authi_cmd_buf_type*);
      return ((void *)cmd_buf_ptr);
    }

    case QMI_CMD_SEND_EAP_RESULT:
    {
      qmi_authi_eap_result_cmd_buf_type *eap_cmd_buf_ptr;
      PS_SYSTEM_HEAP_MEM_ALLOC(eap_cmd_buf_ptr, sizeof(qmi_authi_eap_result_cmd_buf_type),
                               qmi_authi_eap_result_cmd_buf_type*);
      return ((void *)eap_cmd_buf_ptr);
    }

    case QMI_CMD_SEND_AKA_ALGO_RESULT:
    {
      qmi_authi_aka_result_cmd_buf_type *aka_cmd_buf_ptr;
      PS_SYSTEM_HEAP_MEM_ALLOC(aka_cmd_buf_ptr, sizeof(qmi_authi_aka_result_cmd_buf_type),
                               qmi_authi_aka_result_cmd_buf_type*);
      return ((void *)aka_cmd_buf_ptr);
    }

    case QMI_CMD_AUTH_CA_CERT_PROCESS_INTERNAL:
    {
      qmi_authi_ca_cert_cmd_buf_type *ca_cert_cmd_buf_ptr;
      PS_SYSTEM_HEAP_MEM_ALLOC(ca_cert_cmd_buf_ptr, sizeof(qmi_authi_ca_cert_cmd_buf_type),
                               qmi_authi_ca_cert_cmd_buf_type*);
      return ((void *)ca_cert_cmd_buf_ptr);
    }
    default:
      break;
  }
  return NULL;
} /* qmi_authi_get_cmd_buf */

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_SVC_INIT()

  DESCRIPTION
    This function processes a init callback request and intialize the service
    related infos.

  PARAMETERS
    num_instances:  nnumber of QMI Instances

  RETURN VALUE
    None

  DEPENDENCIES
    QMI AUTH must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_process_svc_init
(
  uint16 num_instances
)
{
  qmi_authi_state_type *         auth_sp;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_0( "qmi_authi init callback " );
  auth_sp = &qmi_auth_state;
  memset( auth_sp, 0, sizeof(qmi_authi_state_type) );


  /*-------------------------------------------------------------------------
    Max QMI Instances
  -------------------------------------------------------------------------*/
  auth_sp->num_qmi_instances = num_instances;
  /*-------------------------------------------------------------------------
    initialize client state non-zero fields
  -------------------------------------------------------------------------*/
  if(qmi_authi_global.inited == FALSE)
  {
    memset(&(qmi_authi_global.sim_aka_meta_info), 0, sizeof(eap_sim_aka_meta_info_type));
    qmi_authi_global.sim_aka_meta_info.id.provided_id_len = 0;
    qmi_authi_global.sim_aka_meta_info.task_srv_fct = qmi_auth_eap_task_srv_fct;

    qmi_authi_global.inited = TRUE;
    qmi_authi_global.ca_cert_supported = FALSE;
    qmi_authi_global.ca_client_ptr = NULL;
    
  }
} /* qmi_authi_process_svc_init */

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_ALLOC_CLID()

  DESCRIPTION
    This function process the client alloc callback request. This function
    allocates and initialize the new client state for the allocated client ID.

  PARAMETERS
    common_msg_hdr:  private data buffer containing the clid alloc request information.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI AUTH must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_process_alloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
)
{
  qmi_authi_client_state_type   *  cl_sp;  
  qmi_result_e_type                result = QMI_RESULT_FAILURE; 
  qmi_error_e_type                 errval = QMI_ERR_NONE;
/*-------------------------------------------------------------------------*/

  DS_QMI_AUTH_ASSERT(common_msg_hdr != NULL);
  cl_sp = NULL;

  LOG_MSG_INFO2_0( "qmi_authi process clid allocation" );
  cl_sp = qmi_authi_alloc_cl_sp(common_msg_hdr->client_id);

  if(cl_sp)
  {
    qmi_authi_initialize_client(cl_sp);
    cl_sp->common.clid = common_msg_hdr->client_id;
    cl_sp->instance    = (uint16)common_msg_hdr->qmi_instance;
    cl_sp->service_id  = common_msg_hdr->service ;

    LOG_MSG_INFO2_1( "qmi_authi clid set to local client state ptr: clid%d",
                     cl_sp->common.clid );
    result = QMI_RESULT_SUCCESS;
  }
  else
  {
    LOG_MSG_INFO1_0 ("No available service clids!" );
    errval = QMI_ERR_INTERNAL;
  }

  /* Notifying the clid allocation back to client via QMI Framework*/
  qmi_framework_svc_send_alloc_clid_result_ex(result, common_msg_hdr, errval);

} /* qmi_authi_process_alloc_clid */

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_DEALLOC_CLID()

  DESCRIPTION
    This function process the client dealloc callback request. This function
    reset and deallocates the new client state for the allocated client ID.

  PARAMETERS
    common_msg_hdr:  private data buffer containing the clid dealloc
    request information.
 
  RETURN VALUE
    None

  DEPENDENCIES
    QMI AUTH must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_process_dealloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
)
{
  qmi_authi_client_state_type   *  cl_sp;   
  qmi_result_e_type                result = QMI_RESULT_FAILURE; 
  qmi_error_e_type                 errval = QMI_ERR_NONE;
/*-------------------------------------------------------------------------*/

  DS_QMI_AUTH_ASSERT(common_msg_hdr != NULL);

  cl_sp = NULL;
  LOG_MSG_INFO2_0( "qmi_authi process clid deallocation" );

  cl_sp = (qmi_authi_client_state_type *) 
          qmi_auth_state.client[common_msg_hdr->client_id  - 1];

  if (cl_sp != NULL && cl_sp->common.clid == common_msg_hdr->client_id)
  {
    qmi_authi_reset_client(cl_sp);
    qmi_authi_dealloc_cl_sp(common_msg_hdr->client_id);
    LOG_MSG_INFO2_1( "qmi_authi clid free to local client state ptr: clid%d",
                     common_msg_hdr->client_id );
    result = QMI_RESULT_SUCCESS;
  }
  else
  {
    errval = QMI_ERR_INVALID_CLIENT_ID;
    LOG_MSG_ERROR_INT_1( "Can't find clid: d!", common_msg_hdr->client_id);
  }

  /* Notifying the clid deallocation back to client via QMI Framework*/
  qmi_framework_svc_send_dealloc_clid_result_ex(result, common_msg_hdr, errval);
} /* qmi_authi_process_dealloc_clid */

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_CMD_HDLR()

  DESCRIPTION
    This function process the service command handler request callback.
    This function creates a transaction and dispatches to the appropriate
    message handler
    
  PARAMETERS
    msg_hdr:  Framework message header for the request.
    sdu_in : request

  RETURN VALUE
    None

  DEPENDENCIES
    QMI AUTH must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_process_cmd_hdlr
(
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type * sdu_in 
)
{
  qmi_authi_state_type    * svc_sp;
  qmi_authi_client_state_type * cl_sp;
  int32                         temp;
  uint16                        cmd_type;
  uint16                        sdu_len;
  uint16                        remaining_bytes;
  dsm_item_type *               next_sdu;
  qmi_transaction_type *  x_p;
  qmi_cmd_buf_type *   cmd_buf_p;
  
/*-------------------------------------------------------------------------*/

  DS_QMI_AUTH_ASSERT(msg_hdr && sdu_in);
  LOG_MSG_INFO2_3( "Process QMI AUTH svc commoan handlr callback function: clid%d, tx_id:%d, ctl_flag:%d ",
                   msg_hdr->common_hdr.client_id,
                   msg_hdr->common_hdr.transaction_id,
                   msg_hdr->msg_ctl_flag );

  /*-------------------------------------------------------------------------
    Get a service state and client state pointer
  -------------------------------------------------------------------------*/
  svc_sp = (qmi_authi_state_type *)&qmi_auth_state;
  cl_sp = (qmi_authi_client_state_type *) 
             qmi_auth_state.client[msg_hdr->common_hdr.client_id - 1];
  
  DS_QMI_AUTH_ASSERT(cl_sp != NULL);

  /*-------------------------------------------------------------------------
    Get a transaction buffer to track all contained commands in the bundle
  -------------------------------------------------------------------------*/
  x_p = qmi_authi_get_transaction(svc_sp, cl_sp);
  if (x_p == NULL)
  {
    LOG_MSG_INFO1_1 ("No transaction container!  Discarding service %x message",
                     svc_sp);
    dsm_free_packet(&sdu_in);
    return; 
  }

  x_p->ctl  = msg_hdr->msg_ctl_flag;
  x_p->x_id = msg_hdr->common_hdr.transaction_id;

  /*-------------------------------------------------------------------------                  
    Parse the incoming service message
  -------------------------------------------------------------------------*/
  next_sdu = NULL;
  remaining_bytes = (uint16) dsm_length_packet(sdu_in);
  while (sdu_in)
  {
    /*-----------------------------------------------------------------------
      Extract service message header
    -----------------------------------------------------------------------*/
    temp = dsm_pull16( &sdu_in ); 
    if (temp == -1)
    {
      LOG_MSG_INFO1_1 ("Short QMI AUTH svc header!  Discarding transaction %d",
                       x_p->x_id);
      dsm_free_packet( &sdu_in );
      qmi_authi_free_transaction( &x_p );
      return;
    }
    cmd_type = ps_ntohs(temp);

    temp = dsm_pull16( &sdu_in );
    if (temp == -1)
    {
      LOG_MSG_INFO1_1 ("Short QMI AUTH svc header!  Discarding transaction %d",
                       x_p->x_id);
      dsm_free_packet( &sdu_in );
      qmi_authi_free_transaction( &x_p );
      return;
    }
    sdu_len = ps_ntohs(temp);
    remaining_bytes -= 4;

    if (sdu_len > remaining_bytes)
    {
      LOG_MSG_INFO1_1 ("Short QMI AUTH svc payload!  Discarding service %x message",
                       svc_sp);
      dsm_free_packet( &sdu_in );
      qmi_authi_free_transaction( &x_p );
      return;
    }
    else if (sdu_len < remaining_bytes)
    {
      /*---------------------------------------------------------------------
        save subsequent SDUs and isolate the current by trimming,
        unbundling individual commands into separate dsm chains
      ---------------------------------------------------------------------*/
      DS_QMI_AUTH_ASSERT (next_sdu == NULL);
      temp = PS_UTILS_DSM_DUP_PACKET( &next_sdu, sdu_in, sdu_len, 65535);
      if (0 == temp)
      {
        dsm_free_packet( &next_sdu ); // is this needed?
        dsm_free_packet( &sdu_in );
        qmi_authi_free_transaction( &x_p );
        return;
      }
      dsm_trim_packet( &sdu_in, sdu_len );
    }

    /*-----------------------------------------------------------------------
      sdu_in now contains a single command.
      Get a QMI command buffer to hold the command
    -----------------------------------------------------------------------*/
    DS_QMI_AUTH_ASSERT (x_p != NULL);
    cmd_buf_p = qmi_authi_get_transaction_cmd_buf( x_p );
    if (cmd_buf_p == NULL)
    {
      LOG_MSG_INFO1_1 ("No QMI command buffer!  Discarding service %x message",
                       svc_sp);
      dsm_free_packet(&sdu_in);
      qmi_authi_free_transaction( &x_p );
      return;
    }

    cmd_buf_p->cmd_type = cmd_type;
    x_p->req_list[x_p->n_cmds-1] = sdu_in;

    /*-----------------------------------------------------------------------
      move to next sdu in bundle, if present
    -----------------------------------------------------------------------*/
    sdu_in = next_sdu;
    next_sdu = NULL;
    remaining_bytes -= sdu_len;
  }
  LOG_MSG_INFO2_3 ("QMI Auth Dispatch the transaction(%x) with (%d cmds) ctl_flag= %x",
                   x_p->x_id, x_p->n_cmds, x_p->ctl);
  qmi_authi_dispatch_transaction(x_p);
} /* qmi_authi_process_cmd_hdlr */

/*===========================================================================
  FUNCTION QMI_AUTHI_DISPATCH_TRANSACTION()

  DESCRIPTION
    Dispatches all QMI AUTH commands in a passed transaction.
    This should be called after the comman handlr function has unbundled the request.

  PARAMETERS
    x_p : pointer to transaction containing commands to dispatch

  RETURN VALUE
    None

  DEPENDENCIES
    QMI Auth service must be initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_dispatch_transaction
(
  qmi_transaction_type *  x_p
)
{
  qmi_cmd_buf_type *  cmd_buf_p;
  dsm_item_type *     msg;
  int                 i;
  boolean             free_t = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Dispatch each of the commands in the transaction
  -------------------------------------------------------------------------*/
  for( i = 0; i < x_p->n_cmds; i++ )
  {
    cmd_buf_p = x_p->cmd_list[i];
    msg = x_p->req_list[i];
    x_p->req_list[i] = NULL;

    /*-----------------------------------------------------------------------
      Dispatch the current SDU/msg transaction
    -----------------------------------------------------------------------*/
    if( FALSE == qmi_authi_input( cmd_buf_p, &msg ) )
    {
      LOG_MSG_INFO2_3 ("Transaction %x Command #%d (%x) processing failed",
                       x_p, i, cmd_buf_p);
      free_t = TRUE;
     //  we should ensure the transaction doesn't stall here.
    }

    /*-----------------------------------------------------------------------
      Ensure the input SDU is freed.  Command handlers don't need to worry
      about freeing the input message.
    -----------------------------------------------------------------------*/
    dsm_free_packet ( &msg );
  }
  if(TRUE == free_t)
  {
    // this indicates that qmi_authi_input() failed for one of the bundled 
    // commands in the transaction. This failure could be either problem
    // with assembling response or invalid command type was sent to the
    // service. In either case right thing to do is to cleanup transaction
    // at this point, otherwise it will never be cleaned up.
    qmi_authi_free_transaction( &x_p );
  }
} /* qmi_authi_dispatch_transaction() */

/*===========================================================================
  FUNCTION QMI_AUTHI_INPUT()

  DESCRIPTION
    Process the incoming QMI message and send the response back.
    This function is registered with QMUX.

  PARAMETERS
    cmd_buf_p_in : command buffer pointer
    sdu_in    : message to be processed

  RETURN VALUE
    TRUE:   message processed successfully
    FALSE:  invalid message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_authi_input
(
  qmi_cmd_buf_type *             cmd_buf_p,
  dsm_item_type **               sdu_in
)
{
  qmi_authi_state_type *    svc_sp;
  qmi_authi_client_state_type * cl_sp;
  dsm_item_type *          response_ptr = NULL;
  qmi_svc_cmd_hdlr_type *  cmd_hdlr;
  uint16                   cmd;
  boolean                  retval;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT (cmd_buf_p && sdu_in);

  LOG_MSG_INFO3_2( "Got QMI Service cmd type %d (%d bytes)",
                   cmd_buf_p->cmd_type, dsm_length_packet(*sdu_in));

  svc_sp = (qmi_authi_state_type *)&qmi_auth_state;
  cl_sp =  (qmi_authi_client_state_type *)cmd_buf_p->x_p->cl_sp;

  DS_QMI_AUTH_ASSERT (svc_sp && cl_sp);

  /*-------------------------------------------------------------------------
    Checking to see if a cmd hndler is registerd for the input command
  -------------------------------------------------------------------------*/
  cmd_hdlr = qmi_authi_cfg.cmd_hdlr_array;
  for (cmd = 0; cmd < qmi_authi_cfg.cmd_num_entries; cmd++, cmd_hdlr++)
  {
    if (cmd_buf_p->cmd_type == cmd_hdlr->cmd_type)
    {
      break; 
    }
  }

  if( cmd == qmi_authi_cfg.cmd_num_entries )
  {
    LOG_MSG_INFO2_2( "Unrecognized type (=%d) for service (=%d)! Return error",
                     cmd_buf_p->cmd_type, cl_sp->service_id );

    retval = qmi_svc_put_result_tlv( &response_ptr,
                                     QMI_RESULT_FAILURE,
                                     QMI_ERR_INVALID_QMI_CMD );
    if (FALSE == retval)
    {
      dsm_free_packet(&response_ptr);
      response_ptr = NULL;
    }
  }
  else
  {
    cmd_buf_p->in_progress = TRUE;
    if((cmd_buf_p->x_p->ctl & QMI_FLAG_MASK_MSGTYPE) == QMI_FLAG_MSGTYPE_CMD)
    {
      MSG_SPRINTF_1( MSG_SSID_DS, MSG_LEGACY_MED, "Handling %s", cmd_hdlr->msg_desc);
      response_ptr = cmd_hdlr->request_hdlr( svc_sp, cmd_buf_p, cl_sp, sdu_in );
    }
    else
    {
      LOG_MSG_INFO2_2 ("Unhandled (%d cmd) msg type (%d)",
                       cmd_buf_p->x_p->ctl&QMI_FLAG_MASK_MSGTYPE,
                       cmd_buf_p->cmd_type);
      response_ptr = NULL;
    }
  }

  cmd_buf_p->in_progress = FALSE;
  /*-------------------------------------------------------------------------
    send response if ready.  Check for need to queue this command in 
    pending queue.
  -------------------------------------------------------------------------*/
  if (response_ptr == NULL)
  {
    LOG_MSG_INFO2_0 ("(Service generated no response)");
    qmi_authi_free_transaction_cmd_buf(&cmd_buf_p);
    return FALSE;
  }
  else if (response_ptr == QMI_SVC_RESPONSE_PENDING)
  {
    LOG_MSG_INFO2_1 ("Response pending for cmd_buf_p %p", cmd_buf_p);
    // command buffer will be freed later when response is completed
    return TRUE;
  }
  return qmi_authi_send_response( cl_sp, cmd_buf_p, response_ptr );
} /* qmi_authi_input() */

/*===========================================================================
FUNCTION QMIAUTHI_SEND_RESPONSE()

DESCRIPTION
  This function calls QMI Framework API to send a QMI AUTH Service response to a
  clients.

PARAMETERS 
  cl_sp    : client state pointer
  cmd_buf_p: command buffer
  msg_ptr : dsm item

RETURN VALUE
  TRUE: If responses send to Framework
  FALSE: otherwise

DEPENDENCIES
  QMI Auth service must be initialized and registered with Framework

SIDE EFFECTS
  None
===========================================================================*/
static boolean qmi_authi_send_response
(
  qmi_authi_client_state_type *    cl_sp,
  qmi_cmd_buf_type *               cmd_buf_p,
  dsm_item_type *                  msg_ptr
)
{
  qmi_transaction_type * x_p;
  int                          i;
  qmi_framework_msg_hdr_type   msg_hdr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_QMI_AUTH_ASSERT (cl_sp && cmd_buf_p && msg_ptr);

  if(FALSE == qmi_svc_prepend_msg_hdr(&msg_ptr, cmd_buf_p->cmd_type)) 
  {
    LOG_MSG_INFO2_1 ("Unable to send Response for cmd %d",
                     cmd_buf_p->cmd_type);
    /*-----------------------------------------------------------------------
      Note - no need to free msg_ptr, taken care of in 
             qmi_authi_prepend_msg_hdr()
    -----------------------------------------------------------------------*/
    qmi_authi_free_transaction_cmd_buf(&cmd_buf_p);
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    find this outstanding transaction and save the response.
    If all responses are present, generate the response bundle and send
  -------------------------------------------------------------------------*/
  x_p = cmd_buf_p->x_p;
  DS_QMI_AUTH_ASSERT (x_p != NULL);
  
  /*-------------------------------------------------------------------------
    Queue the response (ok if freed above - need to queue regardless for 
    accounting purposes)
  -------------------------------------------------------------------------*/
  if( FALSE == qmi_authi_queue_response( &cmd_buf_p, &msg_ptr ) )
  {
    DS_UTILS_ERROR_FATAL("Unable to push pending response to send queue");
    DS_QMI_AUTH_ASSERT (msg_ptr == NULL); // msg_ptr is freed in queue_response
    return FALSE;
  }

  DS_QMI_AUTH_ASSERT ((cmd_buf_p == NULL) && (msg_ptr == NULL));

  /*-------------------------------------------------------------------------
    If other responses are pending, return now.  Transmission of the bundled
    response will be defered until all responses are "sent" by command hdlrs
  -------------------------------------------------------------------------*/
  for( i = 0; i < x_p->n_cmds; i++ )
  {
    if( QMI_SVC_RESPONSE_PENDING == x_p->resp_list[i] )
    {
      LOG_MSG_INFO2_0 ("qmi_authi_response pending");
      return TRUE;
    }
  }

  /*-------------------------------------------------------------------------
    If all responses are present, generate the bundle by appending all 
    responses and send the whole out
    dequeue off bundle_resp_q and verify that cmd buffers are correct
  -------------------------------------------------------------------------*/
  for( i = 0; i < x_p->n_cmds; i++ )
  {
    DS_QMI_AUTH_ASSERT( x_p->resp_list[i] != QMI_SVC_RESPONSE_PENDING );
    if( x_p->resp_list[i] != QMI_SVC_RESPONSE_NONE )
    {
        dsm_append( &msg_ptr, &(x_p->resp_list[i]) );
    }
  }

  if (msg_ptr)
  {

    msg_hdr.common_hdr.client_id      = cl_sp->common.clid; 
    msg_hdr.common_hdr.qmi_instance   = cl_sp->instance;  
    msg_hdr.common_hdr.service        = (qmux_service_e_type)cl_sp->service_id;  
    msg_hdr.common_hdr.transaction_id = x_p->x_id;

    msg_hdr.msg_ctl_flag = QMI_FLAG_MSGTYPE_RESP;
    if( x_p->n_cmds > 1 )
    {
      msg_hdr.msg_ctl_flag |= QMI_FLAG_MASK_COMPOUND;
    }
    msg_hdr.msg_len  = (uint16) dsm_length_packet(msg_ptr);
    qmi_framework_svc_send_response( &msg_hdr, msg_ptr );
  }

  /*-------------------------------------------------------------------------
    free the transaction buffer
  -------------------------------------------------------------------------*/
  qmi_authi_free_transaction( &x_p );  
  return TRUE;

} /* qmi_authi_send_response */

/*===========================================================================
  FUNCTION QMI_AUTHIQUEUE_RESPONSE()

  DESCRIPTION
    Schedule the response to be sent back to the client The bundled response
    will be sent when all responses have been queued.

  PARAMETERS
    cmd_buf_p :  The command buffer for the request for which the response
                 is being queued.
    msg       :  A DSM chain containing the response

  RETURN VALUE
    TRUE  -  Success
    FALSE -  Failure

  DEPENDENCIES
    none

  SIDE EFFECTS
    The command buffer pointed to by cmd_buf_p will be freed.
    The dsm chain referenced by *msg is handled herein.  Caller should 
    never reference the pointer again after this call.
===========================================================================*/
static boolean qmi_authi_queue_response
( 
  qmi_cmd_buf_type **  cmd_buf_p,
  dsm_item_type **     msg
)
{
  qmi_transaction_type *  x_p;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT (cmd_buf_p && *cmd_buf_p && ((*cmd_buf_p)->x_p) && msg);

  /*-------------------------------------------------------------------------
    find this outstanding transaction and save the response.
    If all responses are present, generate the response bundle and send
  -------------------------------------------------------------------------*/
  x_p = (*cmd_buf_p)->x_p;
  
  /*-------------------------------------------------------------------------
    check if cmd buffer was one of the cmds in the transaction.
  -------------------------------------------------------------------------*/
  if( (*cmd_buf_p) != x_p->cmd_list[(*cmd_buf_p)->cmd_no] )
  {
    /*-----------------------------------------------------------------------
      If cmd was not part of the associated transaction, bad news!
    -----------------------------------------------------------------------*/
    DS_UTILS_ERROR_FATAL("Command is linked to transaction it doesn't belong to! discarding");
    dsm_free_packet( msg );
    qmi_authi_free_transaction_cmd_buf( cmd_buf_p );
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    If so, save the response dsm pointer in the transaction
    If response dsm pointer is null, the free command buffer call below
    will set the pointer to 1 to indicate that processing is complete but
    no response was generated.
  -------------------------------------------------------------------------*/
  x_p->cmd_list[(*cmd_buf_p)->cmd_no]  = NULL;
  x_p->resp_list[(*cmd_buf_p)->cmd_no] = *msg;
  *msg = NULL;

  /*-------------------------------------------------------------------------
    The command buffer is no longer needed so free it up for reuse
  -------------------------------------------------------------------------*/
  qmi_authi_free_transaction_cmd_buf( cmd_buf_p );
  return TRUE;

} /* qmi_authi_queue_response() */

/*===========================================================================
  FUNCTION QMI_AUTHI_SEND_INDICATION()

  DESCRIPTION
    This function is calls the QMI Framework API to send out the AUTH service
    indication to client.

  PARAMETERS
    msg_hdr_p : QMI Framework message header
    cmd_type : type of indication
    ind : dsm item
 
  RETURN VALUE
    TRUE if ind sent to Framework sucessfully
    FALSE otherwise
 
  DEPENDENCIES
    QMI Auth service must be initialized and registered with Framework
 
  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_authi_send_indication
(
  void *           msg_hdr_p,
  uint16           cmd_type,
  dsm_item_type *  ind
)
{
  qmi_framework_msg_hdr_type   * framework_msg_hdr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_AUTH_ASSERT(msg_hdr_p && ind);  
      
  framework_msg_hdr = (qmi_framework_msg_hdr_type*) msg_hdr_p;

  LOG_MSG_INFO2_3( "Sending QMI AUTH Indication to QMI Framework: clid:%d, tx_id:%d, ind_id:%d",
                   framework_msg_hdr->common_hdr.client_id,
                   framework_msg_hdr->common_hdr.transaction_id,
                   cmd_type );

  if(FALSE == qmi_svc_prepend_msg_hdr(&ind, cmd_type)) 
  {
    return FALSE;
  }

  if(TRUE != qmi_framework_svc_send_ind(framework_msg_hdr, ind))  
  {
    LOG_MSG_ERROR_INT_0( "Unable to send QMI AUTH indication to QMI Framework" );
    return FALSE;
  }
  return TRUE;
} /* qmi_authi_send_indication */

/*===========================================================================
  FUNCTION QMI_AUTHI_GET_TRANSACTION_CMD_BUF()

  DESCRIPTION
    Allocate a QMI AUTH service command buffer

  PARAMETERS
    x_p : QMI Transaction that the command buf will be added to

  RETURN VALUE
    pointer to allocated command buffer

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_cmd_buf_type *  qmi_authi_get_transaction_cmd_buf
(
  qmi_transaction_type *  x_p
)
{
  qmi_cmd_buf_type *     cmd_buf_p;
  q_type *               q_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT (x_p != NULL);
  if (x_p->n_cmds >= MAX_MSGS_IN_BUNDLE)
  {
    LOG_MSG_INFO1_0 ("Too many commands in transaction");
    return NULL;
  }

  if (x_p->cl_sp)  // cl_sp != NULL means this transaction is client specific
  {
    q_ptr = &x_p->cl_sp->cmd_free_q;
    cmd_buf_p = q_get(q_ptr);
    if (NULL == cmd_buf_p)
    {
      LOG_MSG_INFO1_0 ("No command buffers available!");
      return NULL;
    }
  }
  else
  {
      LOG_MSG_INFO1_0 ("Invalid state pointer !");
      return NULL;
  }

  cmd_buf_p->x_p    = x_p;
  cmd_buf_p->cmd_no = x_p->n_cmds;
  cmd_buf_p->in_progress = FALSE;
  x_p->cmd_list[x_p->n_cmds] = cmd_buf_p;
  x_p->resp_list[x_p->n_cmds] = QMI_SVC_RESPONSE_PENDING;
  x_p->n_cmds++;
  return cmd_buf_p;

} /* qmi_authi_get_transaction_cmd_buf() */

/*===========================================================================
  FUNCTION QMI_AUTHI_FREE_TRANSACTION_CMD_BUF()

  DESCRIPTION
    Frees a previously allocated QMI AUTH transaction command buffer

  PARAMETERS
    cmd_buf_handle : Handle to command buf pointer which points to the cmd
                     buffer to be freed.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI Auth service must be initialized and registered with Framework
 
  SIDE EFFECTS
    Removes reference to this command buf from the associated transaction
    structure.  
===========================================================================*/
static void qmi_authi_free_transaction_cmd_buf
(
  qmi_cmd_buf_type **  cmd_buf_handle
)
{
  qmi_authi_client_state_type *  cl_sp;
  qmi_cmd_buf_type *            freeing_cmd_buf_p;
  q_type *                      q_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT (cmd_buf_handle != NULL);
  /* get the pointer locally so after we modify the x_p below we don't 
  dereference the null x_p->cmd_list[0] */
  freeing_cmd_buf_p = *cmd_buf_handle; 
  *cmd_buf_handle = NULL; // free the callers reference immediately

  LOG_MSG_INFO3_1 ("Freeing QMI command buffer %x", freeing_cmd_buf_p);

  if (freeing_cmd_buf_p)
  {
    DS_QMI_AUTH_ASSERT (freeing_cmd_buf_p->x_p != NULL);
    cl_sp = (qmi_authi_client_state_type *)freeing_cmd_buf_p->x_p->cl_sp;

    /* remove reference to this command buffer from transaction */
    DS_QMI_AUTH_ASSERT(freeing_cmd_buf_p->cmd_no < MAX_MSGS_IN_BUNDLE);
    LOG_MSG_INFO3_3 ("About to clear reference to cmd_buf_p (%x) from x_p(%x:%d)",
                     freeing_cmd_buf_p,
                     freeing_cmd_buf_p->x_p,
                     freeing_cmd_buf_p->cmd_no );

    /* clear the reference to this cmd buffer in transaction to handle
       the case where the response hasn't yet been queued (this would
       also clear the reference) */
    freeing_cmd_buf_p->x_p->cmd_list[freeing_cmd_buf_p->cmd_no] = NULL;

    /* if cmd buffer is freed before response is generated, need to update
       the containing transaction accounting to ensure response is not 
       withheld on account of this freed response */
       
    LOG_MSG_INFO3_3 ("Check if need to clear pending response (%x) from x_p(%x:%d)",
                     freeing_cmd_buf_p,
                     freeing_cmd_buf_p->x_p,
                     freeing_cmd_buf_p->cmd_no );

    if( freeing_cmd_buf_p->x_p->resp_list[freeing_cmd_buf_p->cmd_no] 
        == QMI_SVC_RESPONSE_PENDING )
    {
      /* indicate that no response is forthcoming for this command */
      freeing_cmd_buf_p->x_p->resp_list[freeing_cmd_buf_p->cmd_no] = 
        (dsm_item_type *) QMI_SVC_RESPONSE_NONE;
    }

    memset( freeing_cmd_buf_p, 0, sizeof (qmi_cmd_buf_type) );
    (void) q_link( freeing_cmd_buf_p, &(freeing_cmd_buf_p->link) );
    if (cl_sp)
    {
      q_ptr = &cl_sp->common.cmd_free_q;
      q_put( q_ptr, &(freeing_cmd_buf_p->link) );
    }
    else
    {
      LOG_MSG_INFO2_0("Freeing cmd_buf for global transaction");
    }
  }
  else
  {
    LOG_MSG_INFO2_0 ("Attempt to free NULL command buffer!");
  }
} /* qmi_authi_free_transaction_cmd_buf() */

/*===========================================================================
  FUNCTION QMI_AUTHI_GET_TRANSACTION()

  DESCRIPTION
    Get a transaction buffer for the QMI Auth service 
    by the client state pointer provided

  PARAMETERS
    svc_sp :  pointer to service state from which the transaction is allocated
    cl_sp :  client state pointer for client to get transaction buffer for
             or NULL if sending a broadcast indication

  RETURN VALUE
    NULL if no transaction buffer is available
    otherwise, pointer to transaction buffer

  DEPENDENCIES
    QMI Auth service must be initialized and registered with Framework
 
  SIDE EFFECTS
    service global transaction might be allocated.
===========================================================================*/
static qmi_transaction_type *  qmi_authi_get_transaction
(
  qmi_authi_state_type *  svc_sp,
  qmi_authi_client_state_type *   cl_sp
)
{
  qmi_transaction_type *  x_p;
  uint8 local_index = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(svc_sp != NULL);
  x_p = NULL;

  if (cl_sp)
  {
    x_p = q_get( &cl_sp->common.x_free_q );
    /* We must initialise the req_list and the resp_list to NULL as we have
       multiple corruption and crashes in the free transaction*/
    if (x_p)
    {
      for(local_index = 0; local_index < MAX_MSGS_IN_BUNDLE; local_index++)
      {
        x_p->req_list[local_index] = NULL;
        x_p->resp_list[local_index] = NULL;
      }
    }
  }
  else /* if no client sp, must be a broadcast indication */
  {
    LOG_MSG_ERROR_0("Invalid client state pointer!");
    return NULL;
  }

  if (x_p)
  {
    x_p->cl_sp = (qmi_common_client_state_type *)cl_sp;
    x_p->svc_sp =  svc_sp;
  }
  return x_p;
} /* qmi_authi_get_transaction() */

/*===========================================================================
  FUNCTION QMI_AUTHI_FREE_TRANSACTION()

  DESCRIPTION
    Frees the passed transaction buffer

  PARAMETERS
    x_p :  handle to transaction pointer pointing to transaction buffer to
           be freed.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI Auth service must be initialized and registered with Framework

  SIDE EFFECTS
    Nulls out the caller's transaction buffer pointer
===========================================================================*/
static void qmi_authi_free_transaction
(
  qmi_transaction_type **  x_p
)
{
  qmi_authi_state_type *         svc_sp;
  qmi_authi_client_state_type *  cl_sp;
  int i;
  boolean free_x;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT (x_p != NULL);
  if (*x_p == NULL)
  {
    LOG_MSG_INFO1_0 ("Attempt to free a NULL transaction pointer!");
    return;
  }
  svc_sp = (qmi_authi_state_type *) (*x_p)->svc_sp; 
  cl_sp = (qmi_authi_client_state_type * )(*x_p)->cl_sp;

  free_x = TRUE;
  if ((*x_p)->n_cmds > 0)
  {
    for( i = 0; (i < MAX_MSGS_IN_BUNDLE && i < (*x_p)->n_cmds); i++ )
    {
      if( (*x_p)->resp_list[i] != (dsm_item_type *) QMI_SVC_RESPONSE_NONE &&
          (*x_p)->resp_list[i] != (dsm_item_type *) QMI_SVC_RESPONSE_PENDING )
      {
        dsm_free_packet( &((*x_p)->resp_list[i]));
      }
      if( (*x_p)->req_list[i] )
      {
        dsm_free_packet( &((*x_p)->req_list[i]) );
      }
      if ((*x_p)->cmd_list[i])
      {
        if( FALSE == (*x_p)->cmd_list[i]->in_progress )
        {
          qmi_authi_free_transaction_cmd_buf( &((*x_p)->cmd_list[i]) );
        }
        else
        {
          /*-------------------------------------------------------------------
            don't free cmd buf if it's in process (i.e. currently executing,
            called free_transaction as part of processing, e.g. reset, abort)
          -------------------------------------------------------------------*/
          free_x = FALSE;
          LOG_MSG_INFO2_0 ("Freeing transaction while command in progress - deferred.");
          /* ensure partially freed transaction doesn't get dispatched in this
             case - queued response for pending command should be freed after
             processing is complete. */
        }
      }
    }
  }

  if (free_x)
  {
    memset( *x_p, 0, sizeof(qmi_transaction_type) );

    (void) q_link( *x_p, &((*x_p)->link) );
    (*x_p)->cl_sp = (qmi_common_client_state_type *)cl_sp;
    (*x_p)->svc_sp = svc_sp;
    if (cl_sp)
    {
      q_put( &cl_sp->common.x_free_q, &((*x_p)->link) );
    }
    *x_p = NULL;
  }
  else
  {
    LOG_MSG_INFO2_0 ("Transaction in process, "
                     "will be freed once current cmd is completed");
  }
} /* qmi_authi_free_transaction() */


/*===========================================================================
  FUNCTION QMI_AUTHI_GET_EAP_AUTH_CREDENTIALS()

  DESCRIPTION
    To get the EAP authentication credential information.
        
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_get_eap_auth_credentials
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *    response;
  qmi_authi_client_state_type * auth_cl_sp;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean            retval;
  uint32             eap_cred_req;
  uint8    type;
  uint16   len;
  void *   value;
  uint16   expected_len;
  boolean got_v_in_reqd = FALSE;
  uint16 opt_tlv_len;
  uint8  tlv_type;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  errval = QMI_ERR_NONE;

  auth_cl_sp = (qmi_authi_client_state_type *) cl_sp;

  eap_cred_req = 0;
    
  if(auth_cl_sp->eap_info.result != EAP_RESULT_SUCCESS)
  {
    errval = QMI_ERR_INVALID_OPERATION;
    goto send_result;
  }

   while (*sdu_in)
  {
    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    // special value 0 = variable length or don't care (unrecognzied TLV)
    expected_len = 0;
    value = NULL;
    switch (type)
    {
      case QMI_TYPE_REQUIRED_PARAMETERS:
        expected_len = sizeof(eap_cred_req);
        value = (void *) &eap_cred_req;
        got_v_in_reqd = TRUE;
        break;
    
      default:
        LOG_MSG_INFO2_1 ("Unrecognized TLV type (%d)", type);
        break;
    }
    if(!qmi_svc_validate_type_length( type,
                                        len,
                                        expected_len,
                                        FILE_ID_DS_QMI_AUTH,__LINE__))
      {
        errval = QMI_ERR_MALFORMED_MSG;
        goto send_result;
      }
    
      /*-----------------------------------------------------------------------
       If type is unrecognized, value will be NULL, and dsm_pullup will 
       free the unrecognized value, i.e. we skip past unrecognized TLVs
       ------------------------------------------------------------------------*/
      if(len != PS_UTILS_DSM_PULLUP( sdu_in, value, len ))
      {
        errval = QMI_ERR_MALFORMED_MSG;
        goto send_result;
      }
    }

  if(got_v_in_reqd != TRUE)
  {
    /* Missing the required TLV, fail */
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  if((eap_cred_req > (AUTHI_EAP_CRED_MSK | AUTHI_EAP_CRED_EXT_MSK | AUTHI_EAP_CRED_SESSION_ID))||
     (eap_cred_req <= (AUTHI_EAP_CRED_MSK & AUTHI_EAP_CRED_EXT_MSK & AUTHI_EAP_CRED_SESSION_ID)))
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  if(eap_cred_req & AUTHI_EAP_CRED_MSK)
  {
    /* Push down the optional TLV for MSK */
    opt_tlv_len = sizeof(auth_cl_sp->eap_info.msk_len) +
                  auth_cl_sp->eap_info.msk_len;

    tlv_type =  AUTHI_PRM_TYPE_EAP_AUTH_CRED_MSK;

    if ( (QMI_SVC_PKT_PUSH(&response, auth_cl_sp->eap_info.msk, 
                           auth_cl_sp->eap_info.msk_len)) &&
         (QMI_SVC_PKT_PUSH(&response, &auth_cl_sp->eap_info.msk_len, sizeof(uint16))) &&
         (QMI_SVC_PKT_PUSH(&response, &opt_tlv_len, sizeof(uint16))) &&
         (QMI_SVC_PKT_PUSH(&response, &tlv_type, sizeof(uint8))) 
         == FALSE )
    {
      LOG_MSG_INFO2_0("Unable to generate EAP MSK!");
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
      goto send_result;
    }
  }

  if(eap_cred_req & AUTHI_EAP_CRED_EXT_MSK)
  {
    opt_tlv_len = sizeof(auth_cl_sp->eap_info.emsk_len) +
                  auth_cl_sp->eap_info.emsk_len;

    tlv_type =  AUTHI_PRM_TYPE_EAP_AUTH_CRED_EMSK;

    if ( (QMI_SVC_PKT_PUSH(&response, auth_cl_sp->eap_info.emsk, 
                           auth_cl_sp->eap_info.emsk_len)) &&
         (QMI_SVC_PKT_PUSH(&response, &auth_cl_sp->eap_info.emsk_len, sizeof(uint16))) &&
         (QMI_SVC_PKT_PUSH(&response, &opt_tlv_len, sizeof(uint16))) &&
         (QMI_SVC_PKT_PUSH(&response, &tlv_type, sizeof(uint8))) 
         == FALSE )
    {
      LOG_MSG_INFO2_0("Unable to generate EAP EMSK!");
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
      goto send_result;
    }
  }
  if(eap_cred_req & AUTHI_EAP_CRED_SESSION_ID)
  {
    opt_tlv_len = sizeof(auth_cl_sp->eap_info.session_id_len) +
                  auth_cl_sp->eap_info.session_id_len;

    tlv_type =  AUTHI_PRM_TYPE_EAP_AUTH_CRED_SESSION_ID;

    if ( (QMI_SVC_PKT_PUSH(&response, auth_cl_sp->eap_info.session_id, 
                           auth_cl_sp->eap_info.session_id_len)) &&
         (QMI_SVC_PKT_PUSH(&response, &auth_cl_sp->eap_info.session_id_len, sizeof(uint16))) &&
         (QMI_SVC_PKT_PUSH(&response, &opt_tlv_len, sizeof(uint16))) &&
         (QMI_SVC_PKT_PUSH(&response, &tlv_type, sizeof(uint8))) 
         == FALSE )
    {
      LOG_MSG_INFO2_0("Unable to generate EAP session ID!");
      errval = QMI_ERR_NO_MEMORY;
      dsm_free_packet(&response);
      goto send_result;
    }
  }

send_result:  
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();
  
  return response;
} /* qmi_authi_get_eap_auth_credentials() */
/*===========================================================================
  FUNCTION QMI_AUTH_SEND_IMSI_PUBLIC_KEY()

  DESCRIPTION
    Send IMSI public key and associated information
  
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_send_imsi_public_key
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                  response;
  qmi_authi_client_state_type *    auth_cl_sp;
  ps_sys_imsi_public_key_info_type   imsi_public_key_info;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean                          retval;
  uint8    type;
  uint16   len = 0;
  void *   value;
  uint16   expected_len;
  boolean  got_v_in_reqd = FALSE;
  int16                                 ps_errno = DS_ENOERR;
  int16                                 return_val = DSS_SUCCESS;
  uint32  subs_id = 0;
  boolean  mcc_mnc_tlv_set = FALSE;
  boolean  ex_time_tlv_set = FALSE;
  uint8    mcc_mnc_param[2*sizeof(uint8)+QMI_AUTH_MAX_MNC_MCC_SIZE + QMI_AUTH_MAX_MNC_MCC_SIZE] = {0};
  int8     mcc_len = 0, mnc_len = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  errval = QMI_ERR_NONE;

  memset( &imsi_public_key_info, 0, sizeof(imsi_public_key_info));

  auth_cl_sp = (qmi_authi_client_state_type *) cl_sp;
  subs_id = auth_cl_sp->subscription_id;

  while (*sdu_in)
  {
    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    // special value 0 = variable length or don't care (unrecognzied TLV)
    expected_len = 0;
    value = NULL;
    switch (type)
    {
      /* expected_len is 0 so that we will go past the check */
      case QMI_TYPE_REQUIRED_PARAMETERS:
        if(len <= (sizeof(imsi_public_key_info.imsi_public_key)+sizeof(uint16)))
        {
          value = (void *) &imsi_public_key_info.imsi_public_key_size;

          /* copy first byte to length field.The structure here is as below
             =========================================================
              type | Length | IMSI public key length| IMSI public key
             ========================================================= */
          if(PS_UTILS_DSM_PULLUP( sdu_in, value, sizeof(uint16)) != sizeof(uint16))
          { 
            errval = QMI_ERR_MALFORMED_MSG;
            goto send_result;
          }
          if((imsi_public_key_info.imsi_public_key_size != (len -= sizeof(uint16))) || 
            (imsi_public_key_info.imsi_public_key_size == 0))
          {
            errval = QMI_ERR_MALFORMED_MSG;
            /* TODO need to failover here */
            goto send_result;
          }  
          value = (void *) &imsi_public_key_info.imsi_public_key;
        }
        else
        {
          errval = QMI_ERR_ARG_TOO_LONG;
          goto send_result;
        }
        got_v_in_reqd = TRUE;
        break;

      case AUTHI_PRM_TYPE_EAP_KEY_ID:
        if(len <= (sizeof(imsi_public_key_info.key_identifier)+sizeof(uint16)))
        {
          value = (void *) &imsi_public_key_info.key_identifier_len;
        
          /* copy first byte to length field.The structure here is as below
             =========================================
              type | Length | user_id length| user_id
             ========================================= */
          if(PS_UTILS_DSM_PULLUP( sdu_in, value, sizeof(uint16)) != sizeof(uint16))
          {
            errval = QMI_ERR_MALFORMED_MSG;
            goto send_result;
          }
          if((imsi_public_key_info.key_identifier_len != (len -= sizeof(uint16))) || 
            (imsi_public_key_info.key_identifier_len== 0))
          {
            errval = QMI_ERR_MALFORMED_MSG;
            /* TODO need to failover here */
            goto send_result;
          }  
          value = (void *) &imsi_public_key_info.key_identifier;
        }
        else
        {
          errval = QMI_ERR_ARG_TOO_LONG;
          goto send_result;
        }
        break;

      case AUTHI_PRM_TYPE_MCC_MNC:
        expected_len = sizeof(ps_sys_mcc_mnc_type);
        if(len > expected_len)
        {
          LOG_MSG_INFO2_3("TLV len (%d) expected len %d too long for type (%d)", len, expected_len, type);
          errval = QMI_ERR_ARG_TOO_LONG;
          goto send_result;
        }

        value = (void *)mcc_mnc_param;
        mcc_mnc_tlv_set = TRUE;
        break;

      case AUTHI_PRM_TYPE_EXPIRY_TIME:
        expected_len = sizeof(imsi_public_key_info.expiry_time);
        value = (void *) &imsi_public_key_info.expiry_time;
        ex_time_tlv_set = TRUE;
        break;
        
      default:
        LOG_MSG_INFO2_1 ("Unrecognized TLV type (%d)", type);
        break;
      }

      /*-----------------------------------------------------------------------
        If type is unrecognized, value will be NULL, and dsm_pullup will 
        free the unrecognized value, i.e. we skip past unrecognized TLVs
      -----------------------------------------------------------------------*/
      if(len != PS_UTILS_DSM_PULLUP( sdu_in, value, len ))
      {
        errval = QMI_ERR_MALFORMED_MSG;
        goto send_result;
      }
    }/* end of while*/
    
  if(!got_v_in_reqd)
  {
    /* Missing the required TLV, fail */
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
    }

/*
                       Fill MCC/MNC parameters
  ====================================================================
  mcc_len       | mcc             | mnc_len                   | mnc                    |
  1 byte        | var             | 1 byte                    | var                    |
mcc_mnc_param[0]|mcc_mnc_param[1] | mcc_mnc_param[mcc_len + 1] |mcc_mnc_param[mcc_len + 2]|
 =====================================================================*/

  mcc_len = (int)mcc_mnc_param[0];
  if(mcc_len > PS_SYS_MCC_MNC_SIZE - 1)
  {
    LOG_MSG_INFO2_2("MCC length %d expected max len %d", mcc_len, PS_SYS_MCC_MNC_SIZE - 1 );
    errval = QMI_ERR_ARG_TOO_LONG;
    goto send_result;
  }
  memscpy(imsi_public_key_info.mcc_mnc_info.mcc, PS_SYS_MCC_MNC_SIZE - 1,
           &mcc_mnc_param[1], mcc_len);

  mnc_len = mcc_mnc_param[mcc_len + 1];
  if(mnc_len > PS_SYS_MCC_MNC_SIZE - 1)
  {
    LOG_MSG_INFO2_2("MNC length %d expected max len %d", mnc_len, PS_SYS_MCC_MNC_SIZE - 1 );
    errval = QMI_ERR_ARG_TOO_LONG;
    goto send_result;
  }
  memscpy(imsi_public_key_info.mcc_mnc_info.mnc, PS_SYS_MCC_MNC_SIZE - 1,
           &mcc_mnc_param[mcc_len + 2], mnc_len);

  imsi_public_key_info.mcc_mnc_valid = mcc_mnc_tlv_set;
  imsi_public_key_info.expiry_time_valid = ex_time_tlv_set;

  return_val = ps_sys_conf_set_ex(PS_SYS_TECH_ALL,
                                  PS_SYS_CONF_IMSI_PUBLIC_KEY_INFO,
                                  (ps_sys_subscription_enum_type) subs_id,
                                  &imsi_public_key_info,
                                  &ps_errno);

  if (return_val != DSS_SUCCESS)
  {
    LOG_MSG_ERROR_INT_2("ps_sys_conf_set ret [d] errno [d]", 
                    return_val, ps_errno);

    switch (ps_errno)
    {
      case DS_NOMEMORY:
        errval = QMI_ERR_NO_MEMORY;
        break;
      default:
        errval = QMI_ERR_INTERNAL;
        break;
    }
  }

  send_result:
    result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                     : QMI_RESULT_FAILURE);
  
    retval = qmi_svc_put_result_tlv(&response,
                                    result,
                                    errval); 
    CHECK_RETVAL();
  
    return response;
  
}/* qmi_authi_send_imsi_public_key */

/*===========================================================================
  FUNCTION QMI_AUTHI_IMSI_PUBLIC_KEY_CB()

  DESCRIPTION
    This is the callback that we have registered with PS Sys layer to 
    send retrieve IMSI key indication to AP. 

  PARAMETERS
    tech_type      : technology for which system status has changed
    event_name     : Event name
    event_info_ptr : IMSI public key action
    user_data_ptr  : Optional user data to be sent.

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_imsi_public_key_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
)
{
  qmi_authi_cmd_buf_type *cmd_ptr = NULL;
  ps_sys_event_imsi_public_key_retrieve_info* imsi_refresh;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(event_info_ptr != NULL);

  imsi_refresh = (ps_sys_event_imsi_public_key_retrieve_info *)event_info_ptr;
  
  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(
                    QMI_CMD_AUTH_RETRIEVE_IMSI_PUBLIC_KEY,__LINE__);
  if( cmd_ptr == NULL)
  {
    return;
  }
      
  cmd_ptr->cmd_id = QMI_CMD_AUTH_RETRIEVE_IMSI_PUBLIC_KEY;
  cmd_ptr->data.eap_imsi_public_key_info_cb.action= imsi_refresh->action;
  cmd_ptr->data.eap_imsi_public_key_info_cb.subscription_id = (uint32) subscription_id;

  dcc_send_cmd_ex (DCC_QMI_CMD, cmd_ptr);
} /* qmi_authi_imsi_public_key_cb() */

/*===========================================================================
  FUNCTION QMI_AUTHI_SEND_RETRIEVE_IMSI_PUBLIC_KEY_IND()

  DESCRIPTION
    Process the cmd posted for sending retrieve IMSI key indications

  PARAMETERS
    cmd_ptr : data passed from command

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_send_retrieve_imsi_public_key_ind
(
   qmi_authi_cmd_buf_type *cmd_ptr
)

{
  dsm_item_type  *  ind = NULL;
  qmi_authi_client_state_type * cl_sp = NULL;
  qmi_framework_msg_hdr_type    framework_msg_hdr;
  auth_imsi_public_key_action_enum_v01 action;
  int index = 0;
  dsm_item_type*                     dup_ind = NULL;
  uint16                             len = 0;
  boolean                            ret = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(cmd_ptr != NULL);

  if(cmd_ptr->data.eap_imsi_public_key_info_cb.action == PS_SYS_EVENT_IMSI_PUBLIC_KEY_RETRIEVE)
  {
    action = AUTH_IMSI_PUBLIC_KEY_RETRIEVE_V01;
  }
  else if(cmd_ptr->data.eap_imsi_public_key_info_cb.action == PS_SYS_EVENT_IMSI_PUBLIC_KEY_REFRESH)
  {
    action = AUTH_IMSI_PUBLIC_KEY_REFRESH_V01;
  }

  if( FALSE == qmi_svc_put_param_tlv(&ind, 
                                      QMI_TYPE_REQUIRED_PARAMETERS,
                                      sizeof(uint32),
                                      &action))
  {
    LOG_MSG_INFO2_0("Unable to generate IMSI key indication!");
    dsm_free_packet(&ind);
    return;
  }

  /*-----------------------------------------------------------------------
    Fill the QMI Framework message header before sending Indication to
    Framework.
  -----------------------------------------------------------------------*/

  /*loop through all clients and send ind to clients registered*/
  for( index = 0; index < AUTHI_MAX_CLIDS; index++ )
  {
    cl_sp = (qmi_authi_client_state_type *) qmi_auth_state.client[index];
      
    //check for subscription and report indication
    if( (NULL != cl_sp) &&
          (cl_sp->subscription_id == (qmi_authi_client_subs_e_type)cmd_ptr->data.eap_imsi_public_key_info_cb.subscription_id)&&
          (cl_sp->report_status.report_imsi_public_key_req == TRUE))
    {
      framework_msg_hdr.common_hdr.client_id = cl_sp->common.clid;
      /* if unicast indication, qmi framework map the instance and no need to fill*/
      framework_msg_hdr.common_hdr.qmi_instance = cl_sp->instance;
      framework_msg_hdr.common_hdr.service = QMUX_SERVICE_AUTH;
      /* for indication the transaction ID can be ignored */
      framework_msg_hdr.common_hdr.transaction_id = 0;
    
      if( cl_sp && (cl_sp->common.clid != QMI_SVC_CLID_UNUSED))
      { 
        len = (uint16)dsm_length_packet(ind);
        dup_ind = NULL;

        if (PS_UTILS_DSM_DUP_PACKET(&dup_ind, ind, 0, len) == len)
        {
            ret = ds_qmi_fw_send_ind( &framework_msg_hdr.common_hdr,
                                         AUTHI_CMD_VAL_SEND_IMSI_PUBLIC_KEY_REFRESH_IND,
                                         dup_ind );
        } 
        else
        {
            ret = FALSE;
        }

        if(ret == FALSE)
        {
          LOG_MSG_ERROR_INT_0("Unable to send qmi_authi_send_retrieve_imsi_public_key_ind!");
          break;
        }
      }
    }
  }
  dsm_free_packet(&ind);
} /* qmi_authi_send_retrieve_imsi_public_key_ind */

/*===========================================================================
  FUNCTION QMI_AUTH_RETR_CA_CERT_IND_CB()

  DESCRIPTION
    Process the retreive CA certificate callback and send the command to 
    QMI take to send the indication to the client.

  PARAMETERS
    aka_handle   - aka handle
    status       - result of the aka algorithm
    digest       - Digest data
    digest_len   - the length of the digest data
    aka_data     - the AKA response data
    aka_data_len - the length of the AKA response data
    user_data    - the user_data passed into the start_aka_algo fcn

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_auth_retr_ca_cert_ind_cb
(
  ps_sys_tech_enum_type                          tech_type,
  ps_sys_event_enum_type                         event_name,
  ps_sys_subscription_enum_type                  subscription_id,
  void                                         * event_info_ptr,
  void                                         * user_data_ptr
)
{
  qmi_authi_cmd_buf_type              *cmd_ptr = NULL;
  ps_sys_event_get_ca_certificate     *retr_ca_cert_ptr;
  int16                                ps_errno = 0;
  ps_sys_ioctl_send_ca_cert_info_type  send_ca_cert;
  qmi_authi_ca_cert_tx_link_type      *ca_tx_link_ptr = NULL;
  qmi_authi_client_state_type         *cl_sp = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(event_info_ptr != NULL);

  retr_ca_cert_ptr = (ps_sys_event_get_ca_certificate *)event_info_ptr;
  
  cmd_ptr = (qmi_authi_cmd_buf_type *)qmi_authi_get_cmd_buf(
                    QMI_CMD_AUTH_RETR_CA_CERT_EVT,__LINE__);
  if( cmd_ptr == NULL)
  {
    LOG_MSG_ERROR_INT_0("qmi_auth_retr_ca_cert_ind_cb(): NULL cmd_ptr!");
    return;
  }

  /*-----------------------------------------------------
    Check if AP CA support is present, 
    if not present, send PS_SYS_IOCTL_SEND_CA_CERTIFICATE
    with failure reason
  ------------------------------------------------------*/
  if(qmi_authi_global.ca_cert_supported == FALSE )
  {
    memset(&send_ca_cert, 0, sizeof(ps_sys_ioctl_send_ca_cert_info_type));
    send_ca_cert.failure_info = DS_SYS_CA_CERT_RESULT_INFO_UNAVAILABLE;
    send_ca_cert.tx_id = retr_ca_cert_ptr->tx_id;
    
    ps_sys_ioctl_ex(PS_SYS_TECH_ALL,
                    PS_SYS_IOCTL_SEND_CA_CERTIFICATE,
                    subscription_id,
                    &send_ca_cert,
                    &ps_errno);
    return;
  }
  /*-----------------------------------------------------
    Store tx_id to client q
  ------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(ca_tx_link_ptr, sizeof(qmi_authi_ca_cert_tx_link_type),
                           qmi_authi_ca_cert_tx_link_type*);

  //Init part
  (void)q_link(ca_tx_link_ptr, &ca_tx_link_ptr->link);

  //do memset here
  memset(ca_tx_link_ptr, 0, sizeof(qmi_authi_ca_cert_tx_link_type));
  ca_tx_link_ptr->tx_id = retr_ca_cert_ptr->tx_id;
  (void)q_init(&(ca_tx_link_ptr->ca_cert_record_q));
  
  /* Add it to the queue of tx_id */
  cl_sp = (qmi_authi_client_state_type *) qmi_authi_global.ca_client_ptr;
  q_put(&(cl_sp ->ca_cert_tx_q), &(ca_tx_link_ptr->link));

  /*-----------------------------------------------------
    Copy payload and post command
  ------------------------------------------------------*/
  cmd_ptr->cmd_id = QMI_CMD_AUTH_RETR_CA_CERT_EVT;
  cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.cert_dn_length = 
     retr_ca_cert_ptr->cert_dn_length;

  memscpy(cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.cert_dn, DS_SYS_CERT_DN_MAX, 
          retr_ca_cert_ptr->cert_dn, retr_ca_cert_ptr->cert_dn_length);

  cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.tx_id = retr_ca_cert_ptr->tx_id;
  cmd_ptr->data.retr_ca_cert_cb.subscription_id = (uint32) subscription_id;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
} /* qmi_auth_retr_ca_cert_ind_cb() */

/*===========================================================================
  FUNCTION QMI_AUTHI_RETI_CA_CERT_IND()

  DESCRIPTION
    Process the cmd posted for retrieving CA certificate indications

  PARAMETERS
    cmd_ptr : data passed from command

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_retr_ca_cert_ind
(
   qmi_authi_cmd_buf_type *cmd_ptr
)
{
  dsm_item_type  *  ind = NULL;
  qmi_authi_client_state_type * cl_sp = NULL;
  qmi_framework_msg_hdr_type    framework_msg_hdr;
  uint8             tlv_type;
  uint16            tlv_len;
  int index = 0;
  qmi_authi_ca_cert_tx_link_type  *ca_cert_tx_ptr;
  uint8             error_no = 0;
  uint16            local_cert_dn_length = 0;
  dsm_item_type*                     dup_ind = NULL;
  uint16                             len = 0;
  boolean                            ret = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(cmd_ptr != NULL);

  /* Push down the mandatory TLV for cert DN. uint16 is for cert_dn_length*/
  local_cert_dn_length = cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.cert_dn_length;
  tlv_len = sizeof(uint16) + cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.cert_dn_length;

  tlv_type = AUTHI_PRM_TYPE_CA_CERT_DN;

  if ((QMI_SVC_PKT_PUSH(&ind, &cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.cert_dn, 
                         cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.cert_dn_length)) &&
      (QMI_SVC_PKT_PUSH(&ind, &local_cert_dn_length,
                         sizeof(uint16))) &&
      (QMI_SVC_PKT_PUSH(&ind, &tlv_len, sizeof(uint16))) &&
      (QMI_SVC_PKT_PUSH(&ind, &tlv_type, sizeof(uint8))) 
       == FALSE )
  {
    error_no = 1;
    goto failure_result;
  }

  /* Push down the tx_id TLV */
  if( FALSE == qmi_svc_put_param_tlv(&ind, 
                                      QMI_TYPE_REQUIRED_PARAMETERS,
                                      sizeof(uint32),
                                      &cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.tx_id))
  {
    error_no = 2;
    goto failure_result;
  }

  /*-----------------------------------------------------------------------
    Fill the QMI Framework message header before sending Indication to
    Framework.
  -----------------------------------------------------------------------*/

  /*loop through all clients and send ind to clients registered*/
  for( index = 0; index < AUTHI_MAX_CLIDS; index++ )
  {
    cl_sp = (qmi_authi_client_state_type *) qmi_auth_state.client[index];
      
    //check for subscription and report indication
    if( (NULL != cl_sp) &&
          (cl_sp->subscription_id == (qmi_authi_client_subs_e_type)cmd_ptr->data.retr_ca_cert_cb.subscription_id)&&
          (cl_sp->report_status.report_retr_ca_cert_req == TRUE))
    {
      framework_msg_hdr.common_hdr.client_id = cl_sp->common.clid;
      /* if unicast indication, qmi framework map the instance and no need to fill*/
      framework_msg_hdr.common_hdr.qmi_instance = cl_sp->instance;
      framework_msg_hdr.common_hdr.service = QMUX_SERVICE_AUTH;
      /* for indication the transaction ID can be ignored */
      framework_msg_hdr.common_hdr.transaction_id = 0;
    
      if( cl_sp && (cl_sp->common.clid != QMI_SVC_CLID_UNUSED))
      {
        len = (uint16)dsm_length_packet(ind);

        dup_ind = NULL;
        if (PS_UTILS_DSM_DUP_PACKET(&dup_ind, ind, 0, len) == len)
        {
            ret = ds_qmi_fw_send_ind( &framework_msg_hdr.common_hdr,
                                         AUTHI_CMD_VAL_RETR_CA_CERT_IND,
                                         dup_ind);
        } 
        else
        {
            ret = FALSE;
        }

        if(ret == FALSE)
        {
          error_no = 3;
          goto failure_result;
        }
        
      }
    }
  }

  // Free all memory allocated for this indication
  dsm_free_packet(&ind);
  return;
  
  failure_result:

  LOG_MSG_ERROR_INT_1("qmi_authi_retr_ca_cert_ind() error %d", error_no);

  // Free all memory allocated for this indication
  dsm_free_packet(&ind);

  for( index = 0; index < AUTHI_MAX_CLIDS; index++ )
  {
    cl_sp = (qmi_authi_client_state_type *) qmi_auth_state.client[index];
    ca_cert_tx_ptr = (qmi_authi_ca_cert_tx_link_type *)q_check(&(cl_sp->ca_cert_tx_q));
    while(ca_cert_tx_ptr != NULL)
    {
      if(ca_cert_tx_ptr->tx_id == cmd_ptr->data.retr_ca_cert_cb.get_ca_cert.tx_id)
      {
        PS_SYSTEM_HEAP_MEM_FREE(ca_cert_tx_ptr);
      }
    }
    ca_cert_tx_ptr = (qmi_authi_ca_cert_tx_link_type *) q_next(&(cl_sp->ca_cert_tx_q), &(ca_cert_tx_ptr->link));
  }
}/* qmi_authi_retr_ca_cert_ind */

/*===========================================================================
  FUNCTION QMI_AUTH_SEND_CA_CERTIFICATE()

  DESCRIPTION
    Send CA certificate information to modem clients
  
  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_authi_send_ca_certificate
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                  response;
  qmi_authi_client_state_type *    auth_cl_sp;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  dsm_item_type     *temp_ca_cert = NULL;
  boolean  retval;
  uint8    type;
  uint16   len = 0;
  void *   value;
  uint16   expected_len;
  uint8    error_no = 0;
  boolean  got_v_in_reqd = FALSE;
  boolean  num_cert_set = FALSE;
  boolean  cert_index_set = FALSE;
  boolean  ca_cert_set = FALSE;
  boolean  num_segments_set = FALSE;
  boolean  sequence_number_set = FALSE;
  boolean  failure_reason_set = FALSE;
  qmi_authi_ca_cert_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  errval = QMI_ERR_NONE;

  auth_cl_sp = (qmi_authi_client_state_type *) cl_sp;
  
  cmd_ptr = (qmi_authi_ca_cert_cmd_buf_type *)qmi_authi_get_cmd_buf(
  	         QMI_CMD_AUTH_CA_CERT_PROCESS_INTERNAL,__LINE__);
  if( cmd_ptr == NULL)
  {
    goto send_result;
  }
  memset(cmd_ptr, 0, sizeof(qmi_authi_ca_cert_cmd_buf_type));
  cmd_ptr->subs_id = auth_cl_sp->subscription_id;

  while (*sdu_in)
  {
    if( !qmi_svc_get_tl( sdu_in, &type, &len ) )
    {
      continue;
    }

    // special value 0 = variable length or don't care (unrecognzied TLV)
    expected_len = 0;
    value = NULL;
    switch (type)
    {
      /* expected_len is 0 so that we will go past the check */
      case QMI_TYPE_REQUIRED_PARAMETERS:
        expected_len = sizeof(cmd_ptr->tx_id);
        value = (void *) &cmd_ptr->tx_id;
        got_v_in_reqd = TRUE;
        break;

      case AUTHI_PRM_TYPE_NUM_CERTIFICATES:
        expected_len = sizeof(cmd_ptr->num_certificates);
        value = (void *) &cmd_ptr->num_certificates;
        num_cert_set = TRUE;
        break;

      case AUTHI_PRM_TYPE_CERT_INDEX:
        expected_len = sizeof(cmd_ptr->cert_index);
        value = (void *) &cmd_ptr->cert_index;
        cert_index_set = TRUE;
        break;
        
      case AUTHI_PRM_TYPE_CA_CERTIFICATE:
        value = (void *) &cmd_ptr->ca_cert_len;
      
        /* copy first byte to length field.The structure here is as below
           =========================================================
            type | Length | CA Cert length| CA Certificate
           ========================================================= */
        if(PS_UTILS_DSM_PULLUP( sdu_in, value, sizeof(uint16)) != sizeof(uint16))
        { 
          errval = QMI_ERR_MALFORMED_MSG;
          goto send_result;
        }
        if((cmd_ptr->ca_cert_len != (len -= sizeof(uint16))) || 
           (cmd_ptr->ca_cert_len == 0))
        {
          errval = QMI_ERR_MALFORMED_MSG;
          /* TODO need to failover here */
          goto send_result;
        }  

        //ToDO . do we need to allocate memory for DSM item or DUP api do itself
        //Provide correct offset DUP
        //invoke DUP API
        //Use Split
        dsm_split_packet(sdu_in, &temp_ca_cert, cmd_ptr->ca_cert_len, DSM_DS_LARGE_ITEM_POOL);
        cmd_ptr->ca_cert_payload = *sdu_in;
        *sdu_in = temp_ca_cert;        
        ca_cert_set = TRUE;
        break;

      case AUTHI_PRM_TYPE_NUM_SEGMENTS:
        expected_len = sizeof(cmd_ptr->num_segments);
        value = (void *) &cmd_ptr->num_segments;
        num_segments_set = TRUE;
        break;

      case AUTHI_PRM_TYPE_SEQUENCE_NUM:
        expected_len = sizeof(cmd_ptr->sequence_number);
        value = (void *) &cmd_ptr->sequence_number;
        sequence_number_set = TRUE;
        break;
        
      case AUTHI_PRM_TYPE_FAILURE_REASON:
          expected_len = sizeof(uint32);
          value = (void *) &cmd_ptr->failure_reason;
          failure_reason_set = TRUE;
          break;

      default:
        LOG_MSG_INFO1_1 ("Unrecognized TLV type (%d)", type);
        break;
      }

      /*-----------------------------------------------------------------------
        If type is unrecognized, value will be NULL, and dsm_pullup will 
        free the unrecognized value, i.e. we skip past unrecognized TLVs
      -----------------------------------------------------------------------*/
      if(type != AUTHI_PRM_TYPE_CA_CERTIFICATE)
      {
        if(len != PS_UTILS_DSM_PULLUP( sdu_in, value, len ))
        {
          errval = QMI_ERR_MALFORMED_MSG;
          goto send_result;
        }
      }
    }/* end of while*/
    
  if(!got_v_in_reqd)
  {
    /* Missing the required TLV, fail */
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
    }

  /*---------------------------------------------------------------------------
    Check for wrong TLV set configuration
    1. if failure reason is sent, no other optional TLV should be present
    2. CA cert, num_cert, segment need to be present if CA cert is present
  -----------------------------------------------------------------------------*/
  if(failure_reason_set && 
    (ca_cert_set || cert_index_set || num_segments_set || num_cert_set))
  {
    errval = QMI_ERR_INVALID_ARG;
    error_no = 1;
    goto send_result;
  }
  if(!failure_reason_set && (!(ca_cert_set && num_segments_set && num_cert_set && cert_index_set)))
  {
    errval = QMI_ERR_INVALID_ARG;
    error_no = 2;
    goto send_result;
  }
  if(cmd_ptr->num_segments > 1 && (!sequence_number_set))
  {
    errval = QMI_ERR_INVALID_ARG;
    error_no = 3;
    goto send_result;
  }

  if(cmd_ptr->cert_index > cmd_ptr->num_certificates)
  {
    errval = QMI_ERR_INVALID_ARG;
    error_no = 4;
    goto send_result;
  }

  /* Post command to QMI to further process the CA certificate
     Instead of doing it here (non-blocking) */
  cmd_ptr->cmd_id = QMI_CMD_AUTH_CA_CERT_PROCESS_INTERNAL;
  cmd_ptr->auth_cl_sp              = auth_cl_sp;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
  
  send_result:

    LOG_MSG_INFO1_1("qmi_authi_send_ca_certificate(): error_no %d ", error_no);

    result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                     : QMI_RESULT_FAILURE);
    if(errval != QMI_ERR_NONE)
    {
      PS_SYSTEM_HEAP_MEM_FREE(cmd_ptr);
    }
  
    retval = qmi_svc_put_result_tlv(&response,
                                    result,
                                    errval); 
    CHECK_RETVAL();

    return response;
  
}/* qmi_authi_send_ca_certificate */

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_CA_CERT()

  DESCRIPTION
    Process CA certificates recieved from AP
  
  PARAMETERS

    cmd_buf_p : ptr to cmd buffer

  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_process_ca_cert
(
  qmi_authi_ca_cert_cmd_buf_type *cmd_ptr
)
{
  ds_sys_ca_cert_result_enum_type      result = DS_SYS_CA_CERT_RESULT_SUCCESS;
  int16                                error_code = 0;
  int16                                ps_errno = DS_ENOERR;
  uint8                                num_complete_cert = 0;
  qmi_authi_ca_cert_tx_link_type      *ca_tx_link_ptr = NULL;
  q_type                              *tx_q_ptr = NULL;
  boolean                              tx_id_match = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*----------------------------------------------------------------
Steps:
1. check for failure reason
  1.1 use ps_sys_ioctl to convey the failure reason.
2. Initialize the queue for ps_sys_ioctl
3. Process CA certificate
4. once all CA certs are present, order them in order
-----------------------------------------------------------------*/
  DS_QMI_AUTH_ASSERT(cmd_ptr != NULL);

/* 1. check for failure reason */
  if( cmd_ptr->failure_reason != 0 )
  {
    switch(cmd_ptr->failure_reason)
    {
      case 1:
        result = DS_SYS_CA_CERT_RESULT_GENERAL_FAILURE;
        break;
        
      case 2:
        result = DS_SYS_CA_CERT_RESULT_CERT_NOT_FOUND;
        break;
      default:
        error_code = 1;
        break;
    }

    if(!(prepare_and_send_root_cert_failure(result, cmd_ptr->subs_id, cmd_ptr->tx_id)))
    {
      error_code = 1;
      goto failure_result;
    }
  }/* end if( cmd_ptr->failure_reason != 0 ) */

  //Loop though queue and figure out ca_tx_link_ptr for this tx_id
  //Num_cert updated here


  /* If tx_id match any of the q node,
     1. copy num_cert
     2. initialize ca_cert_record_q if needed
     3. call qmi_authi_process_ca_cert_record to procces each CA cert record level
     4. Order all segments if all CA certicates are received
   */
  tx_q_ptr   = &(cmd_ptr->auth_cl_sp->ca_cert_tx_q);
  ca_tx_link_ptr = (qmi_authi_ca_cert_tx_link_type *)q_check(tx_q_ptr);

  while (ca_tx_link_ptr != NULL)
  {
    if(cmd_ptr->tx_id == ca_tx_link_ptr->tx_id)
    { 
      tx_id_match = TRUE;
      break;
    }
    ca_tx_link_ptr = (qmi_authi_ca_cert_tx_link_type *)q_next(tx_q_ptr, &(ca_tx_link_ptr->link));
  }
  /* 3. Process CA certificate */

  if ( TRUE == tx_id_match )
  {
    ca_tx_link_ptr->num_cert = cmd_ptr->num_certificates;
        
    if(!qmi_authi_process_ca_cert_tx(ca_tx_link_ptr, 
                                 cmd_ptr->cert_index, 
                                 cmd_ptr->ca_cert_payload,
                                 cmd_ptr->num_segments,
                                 cmd_ptr->sequence_number,
                                 &num_complete_cert))
    {
      error_code = 3;
      goto failure_result;
    }
  }
  else
  {
    error_code = 4;
    goto failure_result;
  }

  /* 4. if all CA certs are present, call ps_sys_ioctl */
  if( num_complete_cert == cmd_ptr->num_certificates )
  {
    if(!(prepare_and_send_root_cert_result(result, ca_tx_link_ptr, cmd_ptr->subs_id)))
    {
      error_code = 5;
      goto failure_result;
    }

    /* 5. Free memory but not the ca cert dsm buffer memory */
    qmi_authi_ca_cert_free_tx(&(cmd_ptr->auth_cl_sp->ca_cert_tx_q), 
                                cmd_ptr->tx_id,
                                FALSE); 
  }
   
  return;  
  failure_result:

  LOG_MSG_ERROR_INT_3("qmi_authi_process_ca_cert error %d ps_errno %d &(cmd_ptr->auth_cl_sp->ca_cert_tx_q) %d", error_code, ps_errno, &(cmd_ptr->auth_cl_sp->ca_cert_tx_q));
    
    //Free QMI client related queue and dsm item for certificate
    qmi_authi_ca_cert_free_tx(&(cmd_ptr->auth_cl_sp->ca_cert_tx_q),cmd_ptr->tx_id, TRUE);
    return;
}/* qmi_authi_process_ca_cert */

/*===========================================================================
  FUNCTION prepare_and_send_root_cert_failure()

  DESCRIPTION
    Process a failed transaction and notify modem clients about the failure reason
  
  PARAMETERS
    result  : result of the CA certificate retrieval
    subs_id : subscription ID
    tx_id   : transaction ID

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean prepare_and_send_root_cert_failure
(
  ds_sys_ca_cert_result_enum_type  result,
  uint32                           subs_id,
  uint32                           tx_id
)
{
  ps_sys_ioctl_send_ca_cert_info_type   send_ca_cert;
  uint16                                error_code = 0;
  int16                                 ps_errno = DS_ENOERR;
/*--------------------------------------------------------*/
  memset(&send_ca_cert, 0 , sizeof(ps_sys_ioctl_send_ca_cert_info_type));
  send_ca_cert.failure_info = (ps_sys_ca_cert_result_enum_type)result;
  send_ca_cert.tx_id = tx_id;
  
  /* 1.1 use ps_sys_ioctl to convey the failure reason. */
  if( 0 != ps_sys_ioctl_ex(PS_SYS_TECH_ALL,
                             PS_SYS_IOCTL_SEND_CA_CERTIFICATE,
                             (ps_sys_subscription_enum_type)subs_id,
                             &send_ca_cert,
                             &ps_errno))
  {
    error_code = 1;
    goto failure_result;
  }
failure_result:
  LOG_MSG_ERROR_2("prepare_and_send_root_cert_failure err %d ps err %d",error_code,ps_errno);
  return FALSE;
}/* prepare_and_send_root_cert_failure */

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_CA_CERT_TX()

  DESCRIPTION
    Process a particular transaction ID (TX) for CA certificates recieved from AP
  
  PARAMETERS
  * ca_cert_tx_link_ptr   : tx queue ptr
  cert_index              : current certificate index 
  * ca_cert_payload       : certificate
  num_segments            : total number of segments in a certificate
  sequence_number         : current segment sequence
  * num_complete_cert     : number of complete certificate

  eg: if AP is sending 5 CA cert with the 3nd certificate has 2 segments (due to large size).
      if we are currently processing the first segment of the 2 segmensts for the 3nd certificate
      cert_index             : 3 
      num_segments           : 2
      sequence_number        : 1

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_authi_process_ca_cert_tx
(
  qmi_authi_ca_cert_tx_link_type * ca_cert_tx_link_ptr,
  uint16                           cert_index,
  dsm_item_type                  * ca_cert_payload,
  uint8                            num_segments,
  uint8                            sequence_number,
  uint8                          * num_complete_cert
)
{
  boolean                              cert_index_found           = FALSE;
  qmi_authi_ca_cert_record_link_type * ca_cert_record_link_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AUTH_ASSERT(ca_cert_tx_link_ptr != NULL );
    
  ca_cert_record_link_ptr = (qmi_authi_ca_cert_record_link_type *)
                             q_check(&(ca_cert_tx_link_ptr->ca_cert_record_q));
  while (ca_cert_record_link_ptr != NULL)
  {
    if(cert_index == ca_cert_record_link_ptr->cert_index)
    {
      cert_index_found = TRUE;
      break;
    }
    ca_cert_record_link_ptr = 
         (qmi_authi_ca_cert_record_link_type *)q_next(&(ca_cert_tx_link_ptr->ca_cert_record_q), 
                                                      &(ca_cert_record_link_ptr->link));
  }/* end while */

/*-------------------------------------------------------
2. If no match for the cert_index OR there's no node in the queue
--------------------------------------------------------*/
  if( FALSE == cert_index_found)
  {
    PS_SYSTEM_HEAP_MEM_ALLOC(ca_cert_record_link_ptr, sizeof(qmi_authi_ca_cert_record_link_type),
                               qmi_authi_ca_cert_record_link_type*);
    if(ca_cert_record_link_ptr == NULL)
    {
      return FALSE;
    }
    /* 2.1 Create a node and initialize the segment queue   */
    memset(ca_cert_record_link_ptr, 0, sizeof(qmi_authi_ca_cert_record_link_type));
    (void)q_link(ca_cert_record_link_ptr, &ca_cert_record_link_ptr->link);
    ca_cert_record_link_ptr->cert_index = cert_index;
    ca_cert_record_link_ptr->num_segments = num_segments;
    ca_cert_record_link_ptr->is_cert_complete = FALSE;
    
    (void)q_init(&(ca_cert_record_link_ptr->ca_cert_segment_q));
    
    /* Add it to the queue of record */
    q_put(&(ca_cert_tx_link_ptr->ca_cert_record_q), &(ca_cert_record_link_ptr->link));
  }
  
  if(!( qmi_authi_process_ca_cert_record(ca_cert_record_link_ptr,
                                         ca_cert_payload,
                                         sequence_number)))
  {
    return FALSE;
  }
  
  ca_cert_record_link_ptr = (qmi_authi_ca_cert_record_link_type*)q_check(&(ca_cert_tx_link_ptr->ca_cert_record_q));
  while (ca_cert_record_link_ptr != NULL)
  {
    if(ca_cert_record_link_ptr->is_cert_complete)
    {
      *num_complete_cert = (*num_complete_cert) + 1;
    }
    ca_cert_record_link_ptr = 
        (qmi_authi_ca_cert_record_link_type*)q_next(&(ca_cert_tx_link_ptr->ca_cert_record_q),
                                                    &(ca_cert_record_link_ptr->link));

  }/* end while */
  return TRUE;
}/* qmi_authi_process_ca_cert_tx */

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_CA_CERT_RECORD()

  DESCRIPTION
    Process a particular record within a transaction ID (TX) for CA certificates recieved from AP
  
  PARAMETERS
  ca_cert_record_link_ptr : record queue ptr
  * ca_cert_payload       : certificate
  sequence_number         : current segment sequence

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_authi_process_ca_cert_record
(
  qmi_authi_ca_cert_record_link_type * ca_cert_record_link_ptr,
  dsm_item_type                       *ca_cert_payload,
  uint8                                sequence_number
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  LOG_MSG_INFO1_1("qmi_authi_process_ca_cert_record() sequence number %d", sequence_number);
  //store the certificate
  if(!qmi_authi_process_ca_cert_segment(&(ca_cert_record_link_ptr->ca_cert_segment_q),
                                          ca_cert_payload,
                                          sequence_number))
  {
    return FALSE;
  }
  
  /* Check if all segments for that CA cert record are presents*/
  if(q_cnt(&(ca_cert_record_link_ptr->ca_cert_segment_q)) == ca_cert_record_link_ptr->num_segments)
  {
    ca_cert_record_link_ptr->is_cert_complete = TRUE;
    return TRUE;
  }
  
  return TRUE;
}/* qmi_authi_process_ca_cert_record */

/*===========================================================================
  FUNCTION QMI_AUTHI_PROCESS_CA_CERT_SEGMENT()

  DESCRIPTION
    Store CA certificate into segment queue
  
  PARAMETERS
  *segment_q_ptr          : Segment queue ptr
  *ca_cert_payload        : CA certificate
  sequence_number         : current segment sequence

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_authi_process_ca_cert_segment
(
  q_type           *segment_q_ptr,
  dsm_item_type    *ca_cert_payload,
  uint8              sequence_number
)
{
  qmi_authi_ca_cert_segment_link_type * ca_cert_segment_link_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_1("qmi_authi_process_ca_cert_segment() sequence number %d", sequence_number);
  /*------------------------------------------
Steps:
1. Create a segment node
2. copy the node payload
    2.1 point the dsm_item ca_cert_payload to the node
    2.2 copy the sequence number
------------------------------------------*/

/* 1 Create a segment node   */
  PS_SYSTEM_HEAP_MEM_ALLOC(ca_cert_segment_link_ptr, sizeof(qmi_authi_ca_cert_segment_link_type),
                            qmi_authi_ca_cert_segment_link_type*);
  if(ca_cert_segment_link_ptr == NULL)
  {
    LOG_MSG_INFO1_0("qmi_authi_process_ca_cert_segment() allocation failed!");
    return FALSE;
  }
  memset(ca_cert_segment_link_ptr, 0, sizeof(qmi_authi_ca_cert_segment_link_type));
  //copy payload
  (void)q_link(ca_cert_segment_link_ptr, &ca_cert_segment_link_ptr->link);
  ca_cert_segment_link_ptr->ca_cert_seg = ca_cert_payload;
  ca_cert_segment_link_ptr->sequence_num = sequence_number;
  /* Add it to the queue of record */
  q_put(segment_q_ptr, &(ca_cert_segment_link_ptr->link));
  return TRUE;
}/* qmi_authi_process_ca_cert_segment */

/*===========================================================================
  FUNCTION PREPARE_AND_SEND_ROOT_CERT_RESULT()

  DESCRIPTION
    this function construct ps sys payload and 
   calls ps_sys_ioctl to send all processed CA cert to clients.
  
  PARAMETERS
  ds_sys_ca_cert_result_enum_type      result,
  qmi_authi_ca_cert_tx_link_type      *ca_tx_link_ptr,
  uint32                               subs_id

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean prepare_and_send_root_cert_result
(
  ds_sys_ca_cert_result_enum_type      result,
  qmi_authi_ca_cert_tx_link_type      *ca_tx_link_ptr,
  uint32                               subs_id
)
{
  ps_sys_ioctl_send_ca_cert_info_type   send_ca_cert;
  qmi_authi_ca_cert_record_link_type   *ca_record_ptr = NULL;
  ds_sys_ca_cert_records_q_type        *ds_sys_record_ptr = NULL;
  int16                                 error_code = 0;
  int16                                 ps_errno = DS_ENOERR;
/*----------------------------------------------------------------------------*/
  memset(&send_ca_cert, 0 , sizeof(ps_sys_ioctl_send_ca_cert_info_type));

  PS_SYSTEM_HEAP_MEM_ALLOC( send_ca_cert.ca_cert_q_ptr, sizeof(q_type),q_type*);

  if(send_ca_cert.ca_cert_q_ptr == NULL)
  {
    LOG_MSG_INFO1_0("prepare_and_send_root_cert_result() ca_cert_q_ptr allocation failed!");
    return FALSE;
  }
  memset(send_ca_cert.ca_cert_q_ptr, 0 , sizeof(q_type));
  (void)q_init(send_ca_cert.ca_cert_q_ptr);

  ca_record_ptr = (qmi_authi_ca_cert_record_link_type*)q_check(&(ca_tx_link_ptr->ca_cert_record_q));
  send_ca_cert.tx_id = ca_tx_link_ptr->tx_id;
  while(ca_record_ptr != NULL)
  {
    //allocate memory for node send cert send_ca_cert
    PS_SYSTEM_HEAP_MEM_ALLOC(ds_sys_record_ptr, sizeof(ds_sys_ca_cert_records_q_type),
                             ds_sys_ca_cert_records_q_type*);
    if(ds_sys_record_ptr == NULL)
    {
      LOG_MSG_INFO1_0("prepare_and_send_root_cert_result() ds_sys_record_ptr allocation failed!");
      return FALSE;
    }
    memset(ds_sys_record_ptr,0,sizeof(ds_sys_ca_cert_records_q_type));
    
    if(!qmi_authi_process_ca_cert_order(ca_record_ptr, ds_sys_record_ptr))
    {
      error_code = 1;
      goto failure_result;
    }
  
    q_put(send_ca_cert.ca_cert_q_ptr , &(ds_sys_record_ptr->link));
    
    ca_record_ptr = 
        (qmi_authi_ca_cert_record_link_type*)q_next(&(ca_tx_link_ptr->ca_cert_record_q),&(ca_record_ptr->link));
  }        
  if( 0 != ps_sys_ioctl_ex(PS_SYS_TECH_ALL,
                           PS_SYS_IOCTL_SEND_CA_CERTIFICATE,
                           (ps_sys_subscription_enum_type)subs_id,
                           &send_ca_cert,
                           &ps_errno))
  {
    error_code = 2;
    goto failure_result;
  }
  return TRUE;

failure_result:
  LOG_MSG_ERROR_2("prepare_and_send_root_cert_result err %d ps err %d",error_code ,ps_errno);
  
  //free memory allocated for the PS_SYS_IOCTL_SEND_CA_CERTIFICATE
  ds_sys_record_ptr =(ds_sys_ca_cert_records_q_type*) q_check(send_ca_cert.ca_cert_q_ptr);
  while(ds_sys_record_ptr != NULL)
  {
    dsm_free_packet(ds_sys_record_ptr->ca_cert_ptr);

    ds_sys_record_ptr =
      (ds_sys_ca_cert_records_q_type*)q_next(send_ca_cert.ca_cert_q_ptr, &(ds_sys_record_ptr->link));
  }

  PS_SYSTEM_HEAP_MEM_FREE(ds_sys_record_ptr);
  q_destroy(send_ca_cert.ca_cert_q_ptr);
  PS_SYSTEM_HEAP_MEM_FREE(send_ca_cert.ca_cert_q_ptr);
  return FALSE;
}/* prepare_and_send_root_cert_result */

/*===========================================================================
  FUNCTION QMI_AUTHII_PROCESS_CA_CERT_ORDER()

  DESCRIPTION
    this function is called when all segments and certificates are received from AP.
    It process all the stored certificates and sort them in order.
  
  PARAMETERS
  *ca_cert_record_link_ptr : qmi stored CA cert
  *ds_sys_record_ptr       : framework payload

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_authi_process_ca_cert_order
(
  qmi_authi_ca_cert_record_link_type  *ca_cert_record_link_ptr,
  ds_sys_ca_cert_records_q_type       *ds_sys_record_ptr
)
{
  uint16                                seq_index = 1;
  qmi_authi_ca_cert_segment_link_type * ca_cert_segment_link_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

//loop through each record, use the sequence to sort them in order
  for(seq_index = 1; seq_index <= ca_cert_record_link_ptr->num_segments ; seq_index++ )
  {
    ca_cert_segment_link_ptr = 
        (qmi_authi_ca_cert_segment_link_type *)q_check(&(ca_cert_record_link_ptr->ca_cert_segment_q));
   
    while (ca_cert_segment_link_ptr != NULL )
    {
      if( seq_index == ca_cert_segment_link_ptr->sequence_num )
      {
        //for the first segment in each record
        if(ds_sys_record_ptr->ca_cert_ptr == NULL)
        {
          ds_sys_record_ptr->ca_cert_ptr = &(ca_cert_segment_link_ptr->ca_cert_seg);
        }
        else
        {
          dsm_append(ds_sys_record_ptr->ca_cert_ptr, &(ca_cert_segment_link_ptr->ca_cert_seg));
        }
        break;
      }
      ca_cert_segment_link_ptr =
        (qmi_authi_ca_cert_segment_link_type *)q_next(&(ca_cert_record_link_ptr->ca_cert_segment_q), 
                                                      &(ca_cert_segment_link_ptr->link));
    }
  }
  LOG_MSG_INFO1_1("qmi_authi_process_ca_cert_order() done processing ds_sys_record_ptr->ca_cert_ptr %p", 
                   ds_sys_record_ptr->ca_cert_ptr);
  return TRUE;
}/* qmi_authi_process_ca_cert_order */


/*===========================================================================
  FUNCTION QMI_AUTHI_CA_CERT_FREE_TX()

  DESCRIPTION
   Free CA certificate transaction
    
  PARAMETERS

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_ca_cert_free_tx
(
  q_type                       *tx_q_ptr,
  uint32                       tx_id,
  boolean                      remove_ca_cert_buf
)
{
  boolean   tx_id_match = FALSE;
  uint8     error = 0;
  qmi_authi_ca_cert_tx_link_type * ca_cert_tx_link_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ca_cert_tx_link_ptr = (qmi_authi_ca_cert_tx_link_type *)q_check(tx_q_ptr);
    
  while (ca_cert_tx_link_ptr != NULL)
  {
    /* If tx_id match any of the q node, free the record node
       Free the current node as well
     */
    if(tx_id == ca_cert_tx_link_ptr->tx_id)
    { 
      tx_id_match = TRUE;

      //free the record
      qmi_authi_ca_cert_free_record(&(ca_cert_tx_link_ptr->ca_cert_record_q), remove_ca_cert_buf);

      /* Delete the node */
      if (TRUE != q_delete_ext(tx_q_ptr, &(ca_cert_tx_link_ptr->link))) 
      {
        error = 1;
        goto failure_result;
      }
      
      PS_SYSTEM_HEAP_MEM_FREE(ca_cert_tx_link_ptr);
      break;
    }
    ca_cert_tx_link_ptr =
      (qmi_authi_ca_cert_tx_link_type *)q_next(tx_q_ptr, &(ca_cert_tx_link_ptr->link));
  }

failure_result:
  LOG_MSG_INFO1_2("qmi_authi_ca_cert_free_tx error %d tx_id %d",error, tx_id);
  return;
}/* qmi_authi_ca_cert_free_tx */

/*===========================================================================
  FUNCTION QMI_AUTHI_CA_CERT_FREE_RECORD()

  DESCRIPTION
   Free CA certificate record
  
  PARAMETERS

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_ca_cert_free_record
(
  q_type  *ca_cert_record_q,
  boolean remove_ca_cert_buf
)
{
  qmi_authi_ca_cert_record_link_type * ca_cert_record_link_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Go through each node, free the linked queue and itself */
  ca_cert_record_link_ptr = (qmi_authi_ca_cert_record_link_type *)q_check(ca_cert_record_q);
  
  /* 1. check if there's matching cert_index node, mark TRUE if found */
  while (ca_cert_record_link_ptr != NULL)
  {
    qmi_authi_ca_cert_free_segment(&ca_cert_record_link_ptr->ca_cert_segment_q, remove_ca_cert_buf);

    /* Delete the node */
    if (TRUE != q_delete_ext(ca_cert_record_q,
                             &(ca_cert_record_link_ptr->link))) 
    {
      LOG_MSG_ERROR_0("qmi_authi_ca_cert_free_record ERROR!");
    }
    ca_cert_record_link_ptr =
      (qmi_authi_ca_cert_record_link_type *)q_next(ca_cert_record_q, &(ca_cert_record_link_ptr->link));
  }/* end while */
  
  PS_SYSTEM_HEAP_MEM_FREE(ca_cert_record_link_ptr);
    q_destroy(ca_cert_record_q);

} /* qmi_authi_ca_cert_free_record */

/*===========================================================================
  FUNCTION QMI_AUTHI_CA_CERT_FREE_SEGMENT()

  DESCRIPTION
   Free CA certificate segment
  
  PARAMETERS

  RETURN VALUE
    TRUE if process success
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_authi_ca_cert_free_segment
(
  q_type  *ca_cert_segment_q,
  boolean  remove_ca_cert_buf
)
{
  qmi_authi_ca_cert_segment_link_type  *ca_cert_segment_link_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ca_cert_segment_link_ptr = (qmi_authi_ca_cert_segment_link_type *)q_check(ca_cert_segment_q);

  /* 1. check if there's matching cert_index node, mark TRUE if found */
  while (ca_cert_segment_link_ptr != NULL)
  {
    /* 4. Delete the buffer only if failure */
    if(remove_ca_cert_buf)
    {
      dsm_free_buffer(ca_cert_segment_link_ptr->ca_cert_seg);
    }
    if (TRUE != q_delete_ext(ca_cert_segment_q,
                              &ca_cert_segment_link_ptr->link))
    {
      LOG_MSG_ERROR_0("qmi_authi_ca_cert_free_segment ERROR!");
    }

    ca_cert_segment_link_ptr =
      (qmi_authi_ca_cert_segment_link_type *)q_next(ca_cert_segment_q, &(ca_cert_segment_link_ptr->link));
  }/* end while */
  
  PS_SYSTEM_HEAP_MEM_FREE(ca_cert_segment_link_ptr);
  q_destroy(ca_cert_segment_q);
}/* qmi_authi_ca_cert_free_segment */

#endif /* FEATURE_DATA_QMI_EAP */
