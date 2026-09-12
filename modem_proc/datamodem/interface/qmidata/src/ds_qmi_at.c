/*===========================================================================

                         D S _ Q M I _ A T . C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Atcop Service source
 file.

EXTERNALIZED FUNCTIONS

  qmi_at_init()
    Register the AT service with QMUX for all applicable QMI links
    

Copyright (c) 2009 - 2017 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/src/ds_qmi_at.c#2 $ $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/13/17    skc    Fixed encoding issue for legacy QMI AT message.
08/01/17    skc    Fixed issue related to AT FWD commands.
12/20/13    gk     Added support for extended URC/RSP
19/12/13    gk     Added support for qmi_ati_send_at_urc message
12/12/13    sah    fixed kw errors
23/10/13    gk     Delayed clid allocation toi make sure cmd registration
                   request comes only after ATCoP registration is one.
08/09/13    vrk    Modified strlcpy usage to comply with Security guidelines
08/01/13    gk     Fixed double client registration with ATCoP due to back to back client
                   id requests.
2/26/13     ab     Migrated QMI AT to new framework
12/04/12    rev    Fixed KW Errors.
07/26/12    sb     Fixed KW Errors.
05/25/12    sn     Correcting the minor version and rejecting get client id
                   based on global AT Fwd cmd Reg limit of multiple clients.
04/18/12    ua     Fixing a race condition in QMI AT between Client Registration
                   Client ID allocation and Reset of client between QMI modem task and 
                   DCC task.
02/17/12   sb      Fixed Compiler and KW Warnings.
01/08/12    ua     Fixed reseting of QMI AT Client ID in QMI RESET/QMI AT 
                   deregistration. 
12/23/11    ua     Multiple clients can register commands multiple number of times. 
12/13/11    ua     Enhanced to support multiple clients of AT. 
12/17/10    kk     Added log messages before ASSERTs.
12/17/10    kk     Removing DCC command buffer dependency from QMI services.
05/17/10    ua     Removing dependency of ATCoP Macro MAX_ARG.
06/17/10    ua     Touch screen control of CMEC is added as a new optional
                   TLV.
10/02/24    vs     Removed dependency on DS task
09/12/08    ks     Created module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_QMI_AT
#include <stdio.h>

#include "dsm.h"
#include "nv.h"
#include "msg.h"
#include "err.h"
#include "amssassert.h"
#include "ds_qmi_defs.h"
#include "ds_qmi_svc.h"
#include "ds_qmux_ext.h"
#include "ds_qmux.h"
#include "ds_qmi_svc_ext.h"
#include "ds_qmi_task.h"
#include "dcc_task_defs.h"
#include "dcc_task_svc.h"
#include "ps_in.h"
#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"

#include "qmi_svc_defs.h"
#include "qmi_svc_utils.h"
#include "qmi_framework.h"
#include "ds_qmi_at.h"
#include "ds_qmi_fw_common.h"
#include "access_terminal_service_v01.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "ps_utils.h"
#include <stringl/stringl.h>

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Service management
---------------------------------------------------------------------------*/
#define ATI_MAX_CLIDS    (3)
#define ATI_INVALID_CLIENT_INDEX 255
#define ATI_INVALID_INSTANCE    -1

/*---------------------------------------------------------------------------
  Major and Minor Version Nos for AT
---------------------------------------------------------------------------*/
#define ATI_BASE_VER_MAJOR    (1)
#define ATI_BASE_VER_MINOR    (4)

#define ATI_ADDENDUM_VER_MAJOR  (0)
#define ATI_ADDENDUM_VER_MINOR  (0)

/*---------------------------------------------------------------------------
  Macro used in command handlers (common)
---------------------------------------------------------------------------*/
#define CHECK_RETVAL()  if (FALSE == retval) { dsm_free_packet(&response); \
                                               return NULL; }

#define QMI_AT_FREE_CMD_REG_BUF(cmd_reg_buf)                  \
  do                                                          \
  {                                                           \
    if(cmd_reg_buf)                                           \
    {                                                         \
      if(cmd_reg_buf->at_cmd_list_ptr)                        \
      {                                                       \
        PS_SYSTEM_HEAP_MEM_FREE(cmd_reg_buf->at_cmd_list_ptr);\
        cmd_reg_buf->at_cmd_list_ptr = NULL;                  \
      }                                                       \
      PS_SYSTEM_HEAP_MEM_FREE(cmd_reg_buf);                   \
      cmd_reg_buf = NULL;                                     \
    }                                                         \
  }while(0);

#define QMI_ATI_DECODE_REQUEST_MSG( message_id, sdu_in, req_msg, req_size)\
        qmi_ati_decode_msg( message_id, sdu_in, req_msg, req_size);

#define QMI_ATI_ENCODE_RESPONSE_MSG( message_id, resp_msg, size, err_code, response) \
         qmi_ati_encode_msg( message_id, resp_msg, size, err_code, response)

#define DS_QMI_AT_ASSERT(expression) \
         ds_qmi_at_assert_wrapper(__LINE__, expression)

qmi_idl_service_object_type 	at_svc_obj;
/*===========================================================================

                                DATA TYPES

===========================================================================*/

/* TLV types */
  #define ATI_PRM_TYPE_AT_CMD_LIST  (0x01)
  #define ATI_PRM_TYPE_AT_CMD_INFO  (0x01)
  #define ATI_PRM_TYPE_STATE_INFO   (0x11)
  #define ATI_PRM_TYPE_TOKEN_INFO   (0x10)
  #define ATI_PRM_TYPE_TOKEN        (0x10)
  #define ATI_PRM_TYPE_STATE_CMEC_4 (0x19)
  #define ATI_PRM_TYPE_STATE_CMEC  (0x18)
  #define ATI_PRM_TYPE_STATE_CCUG  (0x17)
  #define ATI_PRM_TYPE_STATE_CMEE  (0x16)
  #define ATI_PRM_TYPE_STATE_COLP  (0x15)
  #define ATI_PRM_TYPE_STATE_CLIR  (0x14) 
  #define ATI_PRM_TYPE_STATE_S4    (0x13)
  #define ATI_PRM_TYPE_STATE_S3    (0x12)
  #define ATI_PRM_TYPE_STATE_Q     (0x11)
  #define ATI_PRM_TYPE_STATE_V     (0x10) 

/*---------------------------------------------------------------------------
  AT Command enum type - not equal to the actual command values!
  mapping is in qmi_ati_cmd_callbacks table

  DO NOT REORDER THIS ENUM!
---------------------------------------------------------------------------*/
typedef enum
{
  ATI_CMD_MIN                          = 0,
  ATI_CMD_RESET                        = ATI_CMD_MIN,
  ATI_CMD_REG_AT_CMD_FWD              ,
  ATI_CMD_FWD_AT_RESP                 ,
  ATI_CMD_FWD_AT_URC                  ,
  ATI_CMD_REG_AT_CMD_FWD_EX           ,
  ATI_CMD_MAX, 
  ATI_CMD_WIDTH                    = 0xFFFF                        
} qmi_ati_cmd_e_type;

typedef enum
{
  ATI_CMD_VAL_RESET                 = 0x0000,
  ATI_CMD_VAL_REG_AT_CMD_FWD        = 0x0020,
  ATI_CMD_VAL_FWD_AT_CMD_IND        = 0x0021,
  ATI_CMD_VAL_FWD_AT_RESP           = 0x0022,
  ATI_CMD_VAL_ABORT_AT_CMD_IND      = 0x0023,
  ATI_CMD_VAL_FWD_AT_URC            = 0x0024,
  ATI_CMD_VAL_REG_AT_CMD_FWD_EX     = 0x0025,
  ATI_CMD_VAL_FWD_AT_CMD_IND_EX     = 0x0026,
  
/* QC EXTERNAL QMI COMMAND RANGE IS 0x0000 - 0x5555. 
   Add the next external QMI Command here */

/* VENDOR SPECIFIC QMI COMMAND RANGE IS 0x5556 - 0xAAAA.
   IMPORTANT!
   Add the vendor specific QMI Commands within this range only to avoid 
   conflicts with QC QMI commands that would get released in future */

/* RESERVED QC QMI COMMAND RANGE IS 0xAAAB - 0xFFFE 
   Internal QMI Commands must be added in DECREASING ORDER from Below */
  ATI_CMD_VAL_WIDTH                 = 0xFFFF
} qmi_ati_cmd_val_e_type;

/*---------------------------------------------------------------------------
  AT client registartion state enum type 
  When there is a client registration still pending we move AT registration
  to PENDING.
---------------------------------------------------------------------------*/
typedef enum
{
  ATI_AT_REG_NONE ,
  ATI_AT_REG_PENDING ,
  ATI_AT_REG_DONE
}qmi_ati_at_reg_e_type;

typedef struct
{
  dsat_cmd_list_type *at_cmd_list_ptr;
  uint32 cnt;
  uint32 client;
  qmi_cmd_buf_type * pending_cmd_buf;
}qmi_ati_cmd_reg_buf_type;

/*---------------------------------------------------------------------------
  QMI AT instance state definition &
  AT client state definition
---------------------------------------------------------------------------*/
typedef struct qmi_ati_client_state_s
{
  qmi_common_client_state_type common; // must be first since we alias
  int16     instance;
  struct
  {
    dsat_cmd_list_type    reg_cmd_list[MAX_CMD_LIST];
    uint8                 cmd_cnt;
  }cmd_info;
  qmi_ati_cmd_reg_buf_type * pending_info;
  boolean is_legacy_reg;  // Flag indicating if registration is for legacy client
} qmi_ati_client_state_type;

/*---------------------------------------------------------------------------
  Watermark queue to hold deferred responses
---------------------------------------------------------------------------*/
typedef qmi_ati_client_state_type* qmi_ati_client_state_ptr_type; 

typedef struct
{
  uint16                           num_qmi_instances;  /*Not used at the moment. May be used in the future*/
  qmi_ati_client_state_ptr_type    client[ATI_MAX_CLIDS];
} qmi_ati_state_type;

typedef struct
{
  uint16 cmd_id; /* qmi_cmd_id_e_type */
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
      boolean  status;
      uint32   client_id;
      void *   user_data;
    } client_reg;

    struct
    {
      boolean  status;
      void *   user_data;
    } cmd_reg;

    struct
    {
      uint32  handle;
      void *  user_data;
    } cmd_abort;
  } data;
} qmi_ati_cmd_buf_type;

typedef struct
{
  uint16 cmd_id; /* qmi_cmd_id_e_type */
  byte args[MAX_LINE_SIZE];
  byte name[MAX_CMD_SIZE];
  dsat_cmd_state_type  at_state;
  uint8 num_args;
  unsigned int op;
  uint32  at_handle;
  void * user_data;
} qmi_ati_fwd_cmd_buf_type;

/*===========================================================================

                               INTERNAL DATA

===========================================================================*/


/*---------------------------------------------------------------------------
  QMI service command handlers
  forward declarations & cmd handler dispatch table definition
---------------------------------------------------------------------------*/

static dsm_item_type*  qmi_ati_reset(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_ati_reg_at_cmd_fwd(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_ati_send_at_repsonse(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_ati_send_at_urc(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_ati_reg_at_cmd_fwd_ex(void*, void*, void*, dsm_item_type **);

qmi_error_type_v01 qmi_ati_decode_msg
(
  uint16_t          message_id,
  dsm_item_type **  msg_ptr,
  void *            req_msg,
  uint32_t          req_size
);

boolean qmi_ati_encode_msg
(
  uint16_t             message_id,
  void *               resp_msg,
  uint32_t             size,
  qmi_error_type_v01     err_code,
  dsm_item_type **     response
);

#define AT_HDLR(a,b)  QMI_SVC_HDLR( a, (qmi_svc_hdlr_ftype)b )

static qmi_svc_cmd_hdlr_type  qmi_ati_cmd_callbacks[ATI_CMD_MAX] =
{   
  AT_HDLR( ATI_CMD_VAL_RESET,
             qmi_ati_reset),
  AT_HDLR( ATI_CMD_VAL_REG_AT_CMD_FWD,
                qmi_ati_reg_at_cmd_fwd),
  AT_HDLR( ATI_CMD_VAL_FWD_AT_RESP,
                qmi_ati_send_at_repsonse),
  AT_HDLR( ATI_CMD_VAL_FWD_AT_URC,
                qmi_ati_send_at_urc),
  AT_HDLR( ATI_CMD_VAL_REG_AT_CMD_FWD_EX,
                qmi_ati_reg_at_cmd_fwd_ex)
};

static qmi_ati_state_type  qmi_at_state;

/*---------------------------------------------------------------------------
  QMI callback definition
---------------------------------------------------------------------------*/
static boolean qmi_ati_alloc_clid_cb
(
  qmi_framework_common_msg_hdr_type * svc_common_hdr  
);

static void qmi_ati_dealloc_clid_cb
(
  qmi_framework_common_msg_hdr_type * svc_common_hdr 
); 

static void qmi_ati_init_cb
(
  uint16 num_instances
);

static void qmi_ati_cmd_hdlr_cb
( 
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type ** sdu 
);

static qmi_ati_client_state_type * qmi_ati_alloc_cl_sp
(
  uint8 clid
);

static boolean qmi_ati_dealloc_cl_sp
(
  uint8 clid
);

static void    qmi_ati_initialize_client(qmi_ati_client_state_type *);
static void     qmi_ati_reset_client(void *);

/*---------------------------------------------------------------------------
  QMI instance configurations
---------------------------------------------------------------------------*/
static ds_qmi_fw_svc_cfg_type  qmi_ati_cfg;

static struct
{ 
  boolean inited;
  qmi_ati_at_reg_e_type reg_at_client;
  uint32                      abort_client;
  uint32 atcop_cl_id;
} qmi_ati_global = {FALSE,};



/*===========================================================================

                       FORWARD FUNCTION DECLARATIONS 

===========================================================================*/

static uint32  qmi_ati_get_at_client_id_by_cl_sp (
                                qmi_ati_client_state_type *  cl_sp );

static qmi_ati_client_state_type *  qmi_ati_get_at_client_by_cmd_name (
                                     byte *cmd_name );

static void qmi_ati_process_at_client_reg_status( boolean, uint32, void* );

static void qmi_ati_process_at_cmd_reg_status( boolean, void* );

static void qmi_ati_fwd_at_cmd_ind( uint32, uint32, byte*,
              dsat_cmd_state_type, uint8, byte*, uint32 );
              
static void qmi_ati_fwd_at_cmd_ind_ex( uint32, uint32, byte*,
              dsat_cmd_state_type, uint8, byte*, uint32 );              

static void qmi_ati_send_at_cmd_abort_ind( uint32, void* );

static void qmi_ati_process_cmd(void *);

static void qmi_ati_process_svc_init
(
  uint16 num_instances
);

static void qmi_ati_process_alloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
);

static void qmi_ati_process_dealloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
);

static void qmi_ati_process_cmd_hdlr
(
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type * sdu 
);

static boolean qmi_ati_send_response
(
  qmi_cmd_buf_type *               cmd_buf_p,
  dsm_item_type *                  msg_ptr
);

static void *qmi_ati_get_cmd_buf(qmi_cmd_id_e_type);
static int qmi_ati_strncmp_ig_sp_case
(
  const byte *s1,             /*  Pointer to string 1 */
  const byte *s2,             /*  Pointer to string 2 */
  uint32      n               /* Number of characters to compare */
);
boolean  qmi_ati_validate_client_id
(
  uint32    client,
  uint16    line_num
);

boolean qmi_ati_check_valid_async_cmd_buf
(
  ds_qmi_fw_async_cmd_buf_type * async_cmd_buf
);

#define qmi_ati_free_cmd_buf(buf_ptr) PS_SYSTEM_HEAP_MEM_FREE(buf_ptr)

/*===========================================================================
  FUNCTION QMI_AT_INIT()

  DESCRIPTION
    Register the AT service with QMI Framework

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_at_init
(
  void
)
{
  qmi_framework_err_e_type  reg_result = QMI_FRAMEWORK_ERR_NONE;
  uint32                 idl_major_ver = ATI_BASE_VER_MAJOR;
  uint32                 idl_minor_ver = ATI_BASE_VER_MINOR;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0("QMI AT service init");

  /*-------------------------------------------------------------------------
    Setting QMI AT service command handler with dcc task process
  -------------------------------------------------------------------------*/
  qmi_task_set_svc_cmd_handler(QMUX_SERVICE_AT, qmi_ati_process_cmd);
                                                 
  /*-------------------------------------------------------------------------
    QMI AT service configuration setup
  -------------------------------------------------------------------------*/
  /* Retrieving IDL major and minor version for QMI AT service*/
#ifndef TEST_FRAMEWORK  
  qmi_idl_get_idl_version(at_get_service_object_v01(),&idl_major_ver);
  qmi_idl_get_idl_minor_version(at_get_service_object_v01(),&idl_minor_ver);
#endif /* TEST_FRAMEWORK */
  qmi_ati_cfg.fw_cfg.base_version.major     = idl_major_ver;
  qmi_ati_cfg.fw_cfg.base_version.minor     = idl_minor_ver;
                                                 
  qmi_ati_cfg.fw_cfg.addendum_version.major = ATI_ADDENDUM_VER_MAJOR;
  qmi_ati_cfg.fw_cfg.addendum_version.minor = ATI_ADDENDUM_VER_MINOR;
                                                 
  qmi_ati_cfg.fw_cfg.cbs.alloc_clid         = qmi_ati_alloc_clid_cb;
  qmi_ati_cfg.fw_cfg.cbs.dealloc_clid       = qmi_ati_dealloc_clid_cb;
  qmi_ati_cfg.fw_cfg.cbs.init_cback         = qmi_ati_init_cb;
  qmi_ati_cfg.fw_cfg.cbs.cmd_hdlr           = qmi_ati_cmd_hdlr_cb;

  qmi_ati_cfg.cmd_hdlr_array                = qmi_ati_cmd_callbacks;
  qmi_ati_cfg.cmd_num_entries               = ATI_CMD_MAX;

  /*-------------------------------------------------------------------------
    Calling QMI Framework API to register the service with Framework
  -------------------------------------------------------------------------*/
  reg_result = qmi_framework_reg_service( QMUX_SERVICE_AT,
                                          &qmi_ati_cfg.fw_cfg );

  if (reg_result != QMI_FRAMEWORK_ERR_NONE)
    {
    return;
  }
  qmi_ati_cfg.registered  = TRUE;
  
  at_svc_obj = at_get_service_object_v01();

} /* qmi_at_init() */

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

/*===========================================================================
  FUNCTION QMI_ATI_PROCESS_AT_CLIENT_REG_STATUS()

  DESCRIPTION
    

  PARAMETERS
    

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_ati_process_at_client_reg_status
(
  boolean status,
  uint32   client_id,
  void *userdata
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if((status == FALSE)||(NULL == userdata))
  {
    DS_UTILS_ERROR_FATAL("Unable to handle status req, AT svc not initialized");
  }

  qmi_ati_global.atcop_cl_id = client_id;
  qmi_ati_global.reg_at_client = ATI_AT_REG_DONE;

  LOG_MSG_ERROR_2("sending after client registration. Service Id:%d cmd_hdr %p",((qmi_framework_common_msg_hdr_type*)userdata)->service,userdata);
  /* Notifying the clid allocation back to client via QMI Framework*/
  qmi_framework_svc_send_alloc_clid_result_ex(QMI_RESULT_SUCCESS, 
      (qmi_framework_common_msg_hdr_type*)userdata,
       QMI_ERR_NONE);

  PS_SYSTEM_HEAP_MEM_FREE(userdata);
} /* qmi_ati_process_at_client_reg_status */

/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION DS_QMI_AT_ASSERT_WRAPPER()

DESCRIPTION
  Wrapper function for DS_QMI_AT ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_qmi_at_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DS_QMI_AT_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* ds_qmi_at_assert_wrapper */

/*===========================================================================
  FUNCTION QMI_ATI_DECODE_MSG()

  DESCRIPTION
    This function is a wrapper functions for qmi_svc_idl_message_decode
 
  PARAMETERS
    message_id : unique message ID.
    msg_ptr      : input buffer with all the TLV's.
    req_msg     : c struct to be filled
    req_size     : sizeof c structure

  RETURN VALUE
    qmi_error_type_v01

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_type_v01 qmi_ati_decode_msg
(
  uint16_t          message_id,
  dsm_item_type **  msg_ptr,
  void *            req_msg,
  uint32_t          req_size
)
{
  if(req_msg != NULL)
  {
    return (qmi_error_type_v01)qmi_svc_idl_message_decode(at_svc_obj,
               message_id,msg_ptr,req_msg,req_size);
  }
  else
  {
    return QMI_ERR_INTERNAL_V01;
  }
} /* qmi_ati_decode_msg */

/*===========================================================================
  FUNCTION QMI_ATI_ENCODE_MSG()

  DESCRIPTION
    This function is a wrapper functions for qmi_svc_idl_message_encode
    It assigns error and result codes to response and invokes idl encode function.
 
  PARAMETERS
    message_id : unique message ID.
    resp_msg   : input response buffer.
    size       : size of resp_msg
    err_code   : error code to be filled in the response
    response   : output dsm item pointer which has all the result TLV's

  RETURN VALUE
    TRUE is successful othewise FALSE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_ati_encode_msg
(
  uint16_t             message_id,
  void *               resp_msg,
  uint32_t             size,
  qmi_error_type_v01     err_code,
  dsm_item_type **     response
)
{
  qmi_result_type_v01 result;
  qmi_response_type_v01* res_ptr = NULL;
  /*---------------------------------------------------------------------------*/
  result = (err_code == QMI_ERR_NONE_V01? QMI_RESULT_SUCCESS_V01
                                     : QMI_RESULT_FAILURE_V01);
  res_ptr = (qmi_response_type_v01*)resp_msg;
  res_ptr->result = (qmi_result_type_v01)result;
  res_ptr->error = (qmi_error_type_v01)err_code;

  if( !qmi_svc_idl_message_encode(at_svc_obj,
      QMI_IDL_RESPONSE,message_id,resp_msg,size,response))
  {
    dsm_free_packet(response);
    *response = NULL;
    LOG_MSG_ERROR_INT_1("Encoding failed for message id: d",message_id);
    if(!qmi_svc_put_result_tlv(response,(qmi_result_e_type)QMI_RESULT_FAILURE_V01,
                              (qmi_error_e_type)QMI_ERR_INTERNAL_V01))
    {
      dsm_free_packet(response);
      return FALSE;
    }
  }
  return TRUE;
} /* qmi_ati_encode_msg */

/*===========================================================================
  FUNCTION QMI_ATI_REG_AT_CBACK()

  DESCRIPTION
    

  PARAMETERS
    

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_reg_at_cback
(
  boolean     cmd_status,
  uint32      client_id,
  void      * user_data   
)
{
  qmi_ati_cmd_buf_type *cmd_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd_ptr = (qmi_ati_cmd_buf_type *)qmi_ati_get_cmd_buf(QMI_CMD_AT_CLIENT_REG_STATUS);
  if( cmd_ptr == NULL)
  {
    return;
  }

  cmd_ptr->cmd_id = QMI_CMD_AT_CLIENT_REG_STATUS;
  cmd_ptr->data.client_reg.status = cmd_status;
  cmd_ptr->data.client_reg.client_id = client_id;
  cmd_ptr->data.client_reg.user_data = user_data;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
} /* qmi_ati_reg_at_cback */

/*===========================================================================
  FUNCTION QMI_ATI_REG_AT_CMD_CBACK()

  DESCRIPTION
    

  PARAMETERS
    

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_reg_at_cmd_cback
(
  boolean     cmd_status,
  uint32      client_id,
  void      * user_data   
)
{
  qmi_ati_cmd_buf_type *cmd_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (client_id != qmi_ati_global.atcop_cl_id)
  {
    LOG_MSG_ERROR_2( "Ignoring callback for cl_id %d, expected cl_id %d",
                     client_id, qmi_ati_global.atcop_cl_id );
    return;
  }

  cmd_ptr = (qmi_ati_cmd_buf_type *)qmi_ati_get_cmd_buf(QMI_CMD_AT_CMD_REG_STATUS);
  if( cmd_ptr == NULL)
  {
    return;
  }

  cmd_ptr->cmd_id = QMI_CMD_AT_CMD_REG_STATUS;
  cmd_ptr->data.cmd_reg.status = cmd_status;
  cmd_ptr->data.cmd_reg.user_data = user_data;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
}


/*===========================================================================
  FUNCTION QMI_ATI_PROCESS_AT_CMD_REG_STATUS()

  DESCRIPTION
    This function handles the response sent for command registration

  PARAMETERS
    status   - boolean , result of the command
    user_data  - cmd_buf sent in request

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_process_at_cmd_reg_status
(
  boolean status,
  void * user_data
)
{
  qmi_ati_client_state_type * at_cl_sp;
  qmi_cmd_buf_type            * cmd_buf_p;
  dsm_item_type *              response;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  uint32             client = 0;
  uint32             index;
  qmi_ati_cmd_reg_buf_type *cmd_reg_buf = NULL;
  ds_qmi_fw_async_cmd_buf_type *qmi_resp_buf_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  response = NULL;
  qmi_resp_buf_p = (ds_qmi_fw_async_cmd_buf_type *)user_data;
  LOG_MSG_INFO1_2("qmi_ati_process_at_cmd_reg_status:%d, ud:0x%x",status, user_data);
  
  DS_QMI_AT_ASSERT(qmi_resp_buf_p && qmi_resp_buf_p->cmd_buf);

  cmd_buf_p = (qmi_cmd_buf_type *)qmi_resp_buf_p->cmd_buf;

  if( (NULL == cmd_buf_p->x_p) || (NULL == cmd_buf_p->x_p->cl_sp) )
  {
    LOG_MSG_ERROR_INT_0("Invalid Transaction ptr or client state ptr, most likely client was released");
    ds_qmi_fw_free_async_cmd_buf(qmi_resp_buf_p);
    return;
  }
  /* check cmd_buf check against all clients avaiable */
  if(!qmi_ati_check_valid_async_cmd_buf(qmi_resp_buf_p))
  {
    /*cmd_buf is valid but not found in current client list.. assert*/
    DS_UTILS_ERROR_FATAL("Invalid resp_buf_ptr");
  }

  at_cl_sp = (qmi_ati_client_state_type *) cmd_buf_p->x_p->cl_sp;
  cmd_reg_buf = at_cl_sp->pending_info;
  client = qmi_ati_get_at_client_id_by_cl_sp(at_cl_sp);
  /*make way  for new commands as this handled correctly*/
  at_cl_sp->pending_info = NULL;

  if((cmd_reg_buf == NULL) || 
     (cmd_reg_buf->pending_cmd_buf == NULL) || 
     (cmd_reg_buf->pending_cmd_buf != cmd_buf_p) ||
     (cmd_reg_buf->at_cmd_list_ptr == NULL) ||
     !qmi_ati_validate_client_id(client, __LINE__))
  {
    LOG_MSG_ERROR_INT_1("Invalid pending_info stored at client: d", client);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  if(status == TRUE)
  {
    errval = QMI_ERR_NONE;
   /* Find out number of valid commands existing to update the new commands */
    index = at_cl_sp->cmd_info.cmd_cnt;
    if( (index >= MAX_CMD_LIST) ||
        (index + cmd_reg_buf->cnt > MAX_CMD_LIST) )
    {
      at_cl_sp->pending_info = NULL;
      errval = QMI_ERR_INTERNAL;
      goto send_result;
    }

    LOG_MSG_INFO2_1("Updating new command list from index [%d]", index);

    /* Save the commands to trace the client to send the forward indication to */
    memscpy(&at_cl_sp->cmd_info.reg_cmd_list[index],
           (sizeof(dsat_cmd_list_type ) * cmd_reg_buf->cnt),
           cmd_reg_buf->at_cmd_list_ptr,
           (sizeof(dsat_cmd_list_type ) * cmd_reg_buf->cnt));
    at_cl_sp->cmd_info.cmd_cnt += cmd_reg_buf->cnt;
  }
  else
  {
    errval = QMI_ERR_INVALID_ARG;
  }

 send_result:
  QMI_AT_FREE_CMD_REG_BUF(cmd_reg_buf);
  ds_qmi_fw_free_async_cmd_buf(qmi_resp_buf_p);
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);

  if( (FALSE == qmi_svc_put_result_tlv(&response, result, errval)) ||
      (FALSE == qmi_ati_send_response(cmd_buf_p, response)) )
  {
    LOG_MSG_INFO2_0 ("Unable to send response for QMI AT CMD REG Req");
    dsm_free_packet(&response);
    ds_qmi_fw_free_cmd_buf(&cmd_buf_p);
    return;
  }
} /* qmi_ati_process_at_cmd_reg_status() */

/*===========================================================================
  FUNCTION QMI_ATI_ABORT_AT_CMD_CBACK()

  DESCRIPTION
    

  PARAMETERS
    

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_abort_at_cmd_cback
(
  void *  user_info_ptr,
  uint32  at_handle
)
{
  qmi_ati_cmd_buf_type *cmd_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  cmd_ptr = (qmi_ati_cmd_buf_type *)qmi_ati_get_cmd_buf(QMI_CMD_AT_ABORT_CMD);
  if( cmd_ptr == NULL)
  {
    return;
  }

  cmd_ptr->cmd_id = QMI_CMD_AT_ABORT_CMD;
  cmd_ptr->data.cmd_abort.handle = at_handle;
  cmd_ptr->data.cmd_abort.user_data = user_info_ptr;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
}

/*===========================================================================
  FUNCTION QMI_ATI_SEND_AT_CMD_ABORT_IND()

  DESCRIPTION
    Fwd the AT cmd received 

  PARAMETERS
    cmd_ptr

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_send_at_cmd_abort_ind
(
  uint32 at_handle,
  void * user_data
  
)
{
  dsm_item_type             *  ind;
  qmi_ati_client_state_type * at_cl_sp;
  qmi_framework_common_msg_hdr_type    common_hdr;
  at_abort_at_cmd_ind_v01     ind_msg;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_1("qmi_ati_send_at_cmd_abort_ind handle %d", at_handle);
  if(!qmi_ati_validate_client_id(qmi_ati_global.abort_client,__LINE__))
  {
    return;
  }

  at_cl_sp = qmi_at_state.client[qmi_ati_global.abort_client-1];
  if( at_cl_sp == NULL )
  {
    return;
  }
  qmi_ati_global.abort_client = ATI_INVALID_CLIENT_INDEX;

  ind = NULL;

  memset(&ind_msg, 0, sizeof(at_abort_at_cmd_ind_v01));

  ind_msg.at_handle = at_handle;
  //Encode and send indication
  if(!qmi_svc_idl_message_encode(at_svc_obj,
                                 QMI_IDL_INDICATION,
                                 QMI_AT_ABORT_AT_CMD_IND_V01,
                                 &ind_msg,
                                 sizeof(at_abort_at_cmd_ind_v01),
                                 &ind))
  {
    LOG_MSG_ERROR_INT_0("encoding failed for global flow ind ");
  }
  else
  {    
    /*-----------------------------------------------------------------------
      Fill the QMI Framework message header before sending Indication to
      Framework.
    -----------------------------------------------------------------------*/
    common_hdr.client_id = at_cl_sp->common.clid;
    /* if unicast indication, qmi framework map the instance and no need to fill*/
    common_hdr.qmi_instance = ATI_INVALID_INSTANCE;
    common_hdr.service = QMUX_SERVICE_AT;
    /* for indication the transaction ID can be ignored */
    common_hdr.transaction_id = 0;
    
    if( FALSE == ds_qmi_fw_send_ind( &common_hdr, 
                                     ATI_CMD_VAL_ABORT_AT_CMD_IND, 
                                     ind ) )
    {
      LOG_MSG_ERROR_INT_0("Unable to send at event report indication!");
      dsm_free_packet(&ind);
    }
  }
} /* qmi_ati_send_at_cmd_abort_ind */

/*===========================================================================
  FUNCTION QMI_ATI_FWD_AT_CMD_CBACK()

  DESCRIPTION
    

  PARAMETERS
    

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_ati_fwd_at_cmd_cback
(
  dsat_mode_enum_type   at_cmd_mode,
  const tokens_struct_type    *token_ptr,
  const dsat_cmd_state_type    at_state,    
  uint32                atcop_info,
  void                  *user_info_ptr
)
{
  qmi_ati_fwd_cmd_buf_type *cmd_ptr = NULL;
  byte * begin_arg_ptr;
  byte * curr_arg_ptr;
  uint16  len,total_len=0;
  int8 i;
  qmi_ati_client_state_type * at_cl_sp = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  at_cl_sp = qmi_ati_get_at_client_by_cmd_name(token_ptr->name);
  if (NULL == at_cl_sp)
  {
    return FALSE;
  }

  cmd_ptr = (qmi_ati_fwd_cmd_buf_type *)qmi_ati_get_cmd_buf(QMI_CMD_FWD_AT_CMD);
  if( cmd_ptr == NULL)
  {
    return FALSE;
  }

  memset((void *)cmd_ptr,0,sizeof(qmi_ati_fwd_cmd_buf_type));
  
  cmd_ptr->cmd_id = QMI_CMD_FWD_AT_CMD;

  strlcpy((char *)cmd_ptr->name,
          (const char *)token_ptr->name,
          MAX_CMD_SIZE);

  if (token_ptr->args_found > at_state.dsat_max_arg_size)
  {
    LOG_MSG_ERROR_2("Num token args (%d) greater than max allowed (%d)",
                    token_ptr->args_found, at_state.dsat_max_arg_size);
    DS_QMI_AT_ASSERT(0);
    return FALSE;
  }
  cmd_ptr->num_args = token_ptr->args_found;

  begin_arg_ptr = &(cmd_ptr->args[0]);
  curr_arg_ptr = begin_arg_ptr;

  for(i = 0; i < token_ptr->args_found;  i++)
  {
    /* Since we need to copy num_args arguments each of whom are NULL terminated, 
       strlcat will result in NULL character being removed which is uncalled for.
       Hence strlcpy is used here.

       strlcpy(dst,src,size_t): strlcpy expects sizeof dst buffer in size_t field.
       Also it adds a NULL character at the end of each copy. So NULL character 
       is not accounted for in calculating len.
       Hence effective dst size = MAX_LINE_SIZE - total_len - 1(for NULL character)
    */

    if(at_cl_sp->is_legacy_reg)
    {
      /* Storing tokens from last to first as data will be pushed from down to top while
         encoding legacy qmi at message (qmi_ati_fwd_at_cmd_ind) */
      len = strlcpy((char *)curr_arg_ptr,
                    (const char *)token_ptr->arg[(token_ptr->args_found - 1) - i],
                    (MAX_LINE_SIZE - total_len));
    }
    else
    {
      len = strlcpy((char *)curr_arg_ptr,
                    (const char *)token_ptr->arg[i],
                    (MAX_LINE_SIZE - total_len));
    }

    if(len >= (MAX_LINE_SIZE - total_len))
    {
       DS_UTILS_ERROR_FATAL("Buffer copy fail in AT token list");
    }
    /* After each copy, current_arg_ptr is moved next to NULL character. */    
    curr_arg_ptr += len + 1;
    total_len += len + 1;
  }

  cmd_ptr->op = token_ptr->op;
  cmd_ptr->at_handle = atcop_info;
  cmd_ptr->user_data = user_info_ptr;
  cmd_ptr->at_state = at_state;
  
  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);

  return TRUE;
}

/*===========================================================================
  FUNCTION QMI_ATI_FWD_AT_CMD_IND()

  DESCRIPTION
    Fwd the AT cmd received 

  PARAMETERS
    cmd_ptr

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_ati_fwd_at_cmd_ind
(
  uint32 at_handle,
  uint32 op,
  byte * at_name,
  dsat_cmd_state_type at_state,
  uint8  num_args,
  byte * args,
  uint32 client
)
{
  dsm_item_type             *  ind;
  byte * arg_ptr;
  qmi_framework_common_msg_hdr_type    common_hdr;

  PACKED struct PACKED_POST
  {
    PACKED struct PACKED_POST
    {
      uint32 handle;
      uint32 op;
      uint8  name_len;
    } fixed;
    byte name[MAX_CMD_SIZE];
  } at_cmd_info;
  uint16 state_info_tlv_num_bytes, token_info_tlv_num_bytes;
  uint16 len;
  uint8  type, i;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*If "client" is invalid, then  return a error response to atcop*/
  if(!qmi_ati_validate_client_id(client,__LINE__))
  {
    dsatclient_ext_send_response( FALSE, 
                                  qmi_ati_global.atcop_cl_id,
                                  DSAT_CLIENT_RESULT_ERROR,
                                  DSAT_RESP_COMPLETE,
                                  NULL,
                                  at_handle);

    return;
  }

  ind = NULL;
  memset(&at_cmd_info, 0, sizeof(at_cmd_info));
  state_info_tlv_num_bytes = 0;
  token_info_tlv_num_bytes = 0;

  at_cmd_info.fixed.handle = at_handle;
  at_cmd_info.fixed.op = op;

  at_cmd_info.fixed.name_len = strlen((char*)at_name);

  if (MAX_CMD_SIZE < at_cmd_info.fixed.name_len)
  {
    LOG_MSG_ERROR_2( "cmd_len %d is more than max allowed cmd_len %d",
                     at_cmd_info.fixed.name_len, MAX_CMD_SIZE  );
    DS_QMI_AT_ASSERT(0);
    return;
  }

  memscpy(at_cmd_info.name, 
          at_cmd_info.fixed.name_len, 
          at_name, 
          at_cmd_info.fixed.name_len);

  /* Populate STATE INFO TLVs */

  /* CMEC 4th param optional TLV  */
  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_CMEC_4,
                                     sizeof(at_state.dsat_cmec_val_4),
                                     &(at_state.dsat_cmec_val_4),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_cmec_val_4) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_CMEC,
                                     sizeof(at_state.dsat_cmec_val),
                                     &(at_state.dsat_cmec_val[0]),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_cmec_val) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_CCUG,
                                     sizeof(at_state.dsat_ccug_val),
                                     &(at_state.dsat_ccug_val[0]),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_ccug_val) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_CMEE,
                                     sizeof(at_state.dsat_cmee_val),
                                     &(at_state.dsat_cmee_val),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_cmee_val) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_COLP,
                                     sizeof(at_state.dsat_colp_val),
                                     &(at_state.dsat_colp_val),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_colp_val) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_CLIR,
                                     sizeof(at_state.dsat_clir_val),
                                     &(at_state.dsat_clir_val), 
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_clir_val) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_S4,
                                     sizeof(at_state.dsat_s4_val),
                                     &(at_state.dsat_s4_val),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_s4_val) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_S3,
                                     sizeof(at_state.dsat_s3_val),
                                     &(at_state.dsat_s3_val),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_s3_val) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_Q,
                                     sizeof(at_state.dsat_q_val),
                                     &(at_state.dsat_q_val),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_q_val) +
                              sizeof(uint8) + sizeof(uint16);

  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     ATI_PRM_TYPE_STATE_V,
                                     sizeof(at_state.dsat_v_val),
                                     &(at_state.dsat_v_val),
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  state_info_tlv_num_bytes += sizeof(at_state.dsat_v_val) +
                              sizeof(uint8) + sizeof(uint16);

  type = ATI_PRM_TYPE_STATE_INFO;
  if(!((QMI_SVC_PKT_PUSH(&ind, &state_info_tlv_num_bytes, sizeof(uint16))) &&
       (QMI_SVC_PKT_PUSH(&ind, &type, sizeof(uint8)))))
  {
    LOG_MSG_ERROR_INT_0("Unable to generate indication!");
    goto send_result;
  }

  arg_ptr = args;
  /* Populate Nested Token TLVs */
  for(i = 0; i < num_args; i++)
  {
    /* Token strings are terminated with NULL char - 
       to ease the use on the client side */
    len = strlen((char*)arg_ptr) + 1; //for NULL char
    if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                       ATI_PRM_TYPE_TOKEN,
                                       len,
                                       arg_ptr,
                                       FILE_ID_DS_QMI_AT,__LINE__) )
    {
      goto send_result;
    }

    token_info_tlv_num_bytes += len + sizeof(uint8) + sizeof(uint16);
    arg_ptr += len; //since len includes NULL char, no need to add 1

  }

  type = ATI_PRM_TYPE_TOKEN_INFO;
  if(!((QMI_SVC_PKT_PUSH(&ind, &token_info_tlv_num_bytes, sizeof(uint16))) &&
       (QMI_SVC_PKT_PUSH(&ind, &type, sizeof(uint8)))))
  {
    goto send_result;
  }

  /* mandatory TLV */
  if( FALSE == qmi_svc_put_param_tlv2(&ind, 
                                     QMI_TYPE_REQUIRED_PARAMETERS,
                                     sizeof(at_cmd_info.fixed) +
                                     at_cmd_info.fixed.name_len,
                                     &at_cmd_info,
                                     FILE_ID_DS_QMI_AT,__LINE__) )
  {
    goto send_result;
  }

  /* Save the client ID for abort usage */
  LOG_MSG_INFO1_1("Saving abort client id [%d]", client);
  qmi_ati_global.abort_client = client;

  /*-----------------------------------------------------------------------
    Fill the QMI Framework message header before sending Indication to
    Framework.
  -----------------------------------------------------------------------*/
  common_hdr.client_id = (uint8) client;
  /* if unicast indication, qmi framework map the instance and no need to fill*/
  common_hdr.qmi_instance = ATI_INVALID_INSTANCE;
  common_hdr.service = QMUX_SERVICE_AT;
  /* for indication the transaction ID can be ignored */
  common_hdr.transaction_id = 0;
  
  if( FALSE == ds_qmi_fw_send_ind( &common_hdr, 
                                   ATI_CMD_VAL_FWD_AT_CMD_IND, 
                                   ind ) )
  {
    LOG_MSG_ERROR_INT_0("Unable to send at event report indication!");
    dsm_free_packet(&ind);
  }
  return;
  send_result:
    DS_QMI_AT_ASSERT(0);
    dsm_free_packet(&ind);
    return;
          
} /* qmi_ati_fwd_at_cmd_ind */

/*===========================================================================
  FUNCTION QMI_ATI_FWD_AT_CMD_IND_EX()

  DESCRIPTION
    Fwd the AT cmd received 

  PARAMETERS
    cmd_ptr

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_ati_fwd_at_cmd_ind_ex
(
  uint32 at_handle,
  uint32 op,
  byte * at_name,
  dsat_cmd_state_type at_state,
  uint8  num_args,
  byte * args,
  uint32 client
)
{
  dsm_item_type             *  ind;
  qmi_framework_common_msg_hdr_type    common_hdr;
  byte * arg_ptr;
  uint16 len;
  uint16 prev_len;
  at_fwd_at_cmd_ex_ind_v01 ind_msg;
  uint8   i, j;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_2("qmi_ati_fwd_at_cmd_ind_ex handle %d op %d", 
                   at_handle, op);
  
  /*If "client" is invalid, then  return a error response to atcop*/
  if(!qmi_ati_validate_client_id(client,__LINE__))
  {
    dsatclient_ext_send_response( FALSE, 
                                  qmi_ati_global.atcop_cl_id,
                                  DSAT_CLIENT_RESULT_ERROR,
                                  DSAT_RESP_COMPLETE,
                                  NULL,
                                  at_handle);

    return;
  }
  
  memset(&ind_msg, 0, sizeof(at_fwd_at_cmd_ex_ind_v01));  
  ind = NULL;

  ind_msg.at_cmd_info.at_handle = at_handle;
  ind_msg.at_cmd_info.op_code = op;
  memscpy(ind_msg.at_cmd_info.at_cmd_name, 
          strlen((char*)at_name), 
          at_name, 
          strlen((char*)at_name));  

  /* Populate STATE INFO TLVs */

  /* CMEC 4th param optional TLV  */  
  ind_msg.cmec_val_valid = TRUE;
  ind_msg.cmec_val[3] =  at_state.dsat_cmec_val_4;
            
  memscpy(&(ind_msg.cmec_val[0]), 
          sizeof(at_state.dsat_cmec_val),
          &(at_state.dsat_cmec_val), 
          sizeof(at_state.dsat_cmec_val));                 

  memscpy(&(ind_msg.ccug_val), 
          sizeof(at_state.dsat_ccug_val),
           &(at_state.dsat_ccug_val[0]),
          sizeof(at_state.dsat_ccug_val));

  ind_msg.cmee_val_valid = TRUE;
  ind_msg.cmee_val = at_state.dsat_cmee_val;

  ind_msg.colp_val_valid = TRUE;
  ind_msg.colp_val = at_state.dsat_colp_val;

  ind_msg.clir_val_valid = TRUE;
  ind_msg.clir_val = at_state.dsat_clir_val;

  ind_msg.s4_val_valid = TRUE;
  ind_msg.s4_val = at_state.dsat_s4_val;

  ind_msg.s3_val_valid = TRUE;
  ind_msg.s3_val = at_state.dsat_s3_val;

  ind_msg.q_val_valid = TRUE;
  ind_msg.q_val = at_state.dsat_q_val;

  ind_msg.v_val_valid = TRUE;
  ind_msg.v_val = at_state.dsat_v_val;


  ind_msg.token_valid = TRUE;
  prev_len = 0;
  arg_ptr = args;
  /* Populate  Token TLVs */
  for(i = 0; i < num_args; i++)
  {
    /* Token strings are terminated with NULL char - 
       to ease the use on the client side */
    len = strlen((char*)arg_ptr) + 1; //for NULL char
    for(j = 0; ((j < len) && ((j + prev_len) < QMI_AT_NUM_TOKEN_MAX_V01)); j++)
    {
      ind_msg.token[j + prev_len] = arg_ptr[j];
    }
    arg_ptr += len; //since len includes NULL char, no need to add 1
    prev_len += len;
  }

  ind_msg.token_len = prev_len;

  /* Save the client ID for abort usage */
  LOG_MSG_INFO1_1("Saving abort client id [%d]", client);
  qmi_ati_global.abort_client = client;

  //Encode and send indication
  if(!qmi_svc_idl_message_encode(at_svc_obj,
                                 QMI_IDL_INDICATION,
                                 QMI_AT_FWD_AT_CMD_EX_IND_V01,
                                 &ind_msg,
                                 sizeof(at_fwd_at_cmd_ex_ind_v01),
                                 &ind))
  {
    LOG_MSG_ERROR_INT_0("encoding failed for global flow ind ");
  }
  else
  {    
    /*-----------------------------------------------------------------------
      Fill the QMI Framework message header before sending Indication to
      Framework.
    -----------------------------------------------------------------------*/
    common_hdr.client_id = (uint8) client;
    /* if unicast indication, qmi framework map the instance and no need to fill*/
    common_hdr.qmi_instance = ATI_INVALID_INSTANCE;
    common_hdr.service = QMUX_SERVICE_AT;
    /* for indication the transaction ID can be ignored */
    common_hdr.transaction_id = 0;
    
    if( FALSE == ds_qmi_fw_send_ind( &common_hdr, 
                                     ATI_CMD_VAL_FWD_AT_CMD_IND_EX, 
                                     ind ) )
    {
      LOG_MSG_ERROR_INT_0("Unable to send at event report indication!");
      dsm_free_packet(&ind);
    }
  }
  
  return;          
} /* qmi_ati_fwd_at_cmd_ind_ex */

/*===========================================================================
  FUNCTION QMI_ATI_RESET()

  DESCRIPTION
    Reset the issuing AT client's state
    
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
static dsm_item_type * qmi_ati_reset
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *              response;
  qmi_error_type_v01           errval = QMI_ERR_NONE_V01;
  at_reset_resp_msg_v01        resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0("qmi_ati_reset");
  DS_QMI_AT_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in );

  memset(&resp_msg, 0, sizeof(at_reset_resp_msg_v01));
  /*-------------------------------------------------------------------------
    Reset the client's state
  -------------------------------------------------------------------------*/
  qmi_ati_reset_client(cl_sp);

  /*-------------------------------------------------------------------------
    Build the response from back to front
    No error possible in reset, so result is success
  -------------------------------------------------------------------------*/
  response = NULL;

  QMI_ATI_ENCODE_RESPONSE_MSG(QMI_AT_REG_AT_CMD_FWD_EX_RESP_V01,
                              &resp_msg,
                              sizeof(at_reset_resp_msg_v01),
                              errval,
                              &response);

  return response;

} /* qmi_ati_reset() */

/*===========================================================================
  FUNCTION QMI_ATI_REG_AT_CMD_FWD()

  DESCRIPTION
    Reg AT cmds for forwarding
  
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
static dsm_item_type * qmi_ati_reg_at_cmd_fwd
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *       response;
  dsat_cmd_list_type  *at_cmd_list_ptr;
  PACKED struct PACKED_POST
  {
    boolean abort_flag;
    uint8   len;
    byte    name[MAX_CMD_SIZE];
  } at_cmd_info;
  qmi_error_e_type   errval;
  qmi_result_e_type  result;
  boolean                      retval;
  uint8    type, cnt;
  uint16   len_lvl1, len_lvl2;
  void *   value;
  uint32   client;
  uint32   index =0;
  qmi_ati_cmd_reg_buf_type *cmd_reg_buf = NULL;
  qmi_ati_client_state_type * at_cl_sp;
  ds_qmi_fw_async_cmd_buf_type *qmi_resp_buf_p = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AT_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  errval = QMI_ERR_NONE;

  PS_SYSTEM_HEAP_MEM_ALLOC( at_cmd_list_ptr, sizeof(dsat_cmd_list_type) * MAX_CMD_LIST,
                            dsat_cmd_list_type*);
  if (at_cmd_list_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }
  memset(at_cmd_list_ptr, 0, (sizeof(dsat_cmd_list_type ) * MAX_CMD_LIST));

  cnt = 0;
  
  /* Match the client corresponding to the cl_sp received */
  at_cl_sp = (qmi_ati_client_state_type *) cl_sp;
  client = qmi_ati_get_at_client_id_by_cl_sp(at_cl_sp);


  if(!qmi_ati_validate_client_id(client,__LINE__))
  {
    errval = QMI_ERR_INVALID_CLIENT_ID;
    goto send_result;
  }

  if(at_cl_sp->pending_info != NULL)
  {
    LOG_MSG_INFO1_0("New reg at cmd req arrived when another one is pending");
    errval = QMI_ERR_INVALID_OPERATION;
    goto send_result;
  }

  if( ATI_AT_REG_DONE != qmi_ati_global.reg_at_client )
  {
    LOG_MSG_ERROR_1("command registration is requested before AT client registration for client = [%d]"
                   ,client);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }
  
  while (*sdu_in)
  {
    if( !qmi_svc_get_tl( sdu_in, &type, &len_lvl1 ) )
    {
      continue;
    }

    value = NULL;
    switch (type)
    {
      case ATI_PRM_TYPE_AT_CMD_LIST:
      {
        cnt = 0;

        while(*sdu_in)
        {

          if( !qmi_svc_get_tl(sdu_in, &type, &len_lvl2) )
          {
            continue;
          }
          if(!(len_lvl2 < len_lvl1))
          {
            errval = QMI_ERR_MALFORMED_MSG;
            goto send_result;
          }

          value = NULL;
          memset((void*)&at_cmd_info, 0, sizeof(at_cmd_info));
          switch(type)
          {
            case ATI_PRM_TYPE_AT_CMD_INFO:
            {
              value = (void *) &at_cmd_info;
              if(len_lvl2 > sizeof(at_cmd_info))
              {
                errval = QMI_ERR_ARG_TOO_LONG;
                goto send_result;
              }

              if(len_lvl2 != PS_UTILS_DSM_PULLUP( sdu_in, value, len_lvl2))
              {                                                                    
                errval = QMI_ERR_MALFORMED_MSG;
                goto send_result;
              }

              if(!qmi_svc_validate_type_length( type,
                               (len_lvl2 - sizeof(at_cmd_info.abort_flag)- sizeof(at_cmd_info.len)),
                               at_cmd_info.len,
                               FILE_ID_DS_QMI_AT,__LINE__))               
              {
                errval = QMI_ERR_MALFORMED_MSG;
                goto send_result;
              }

              if (cnt < MAX_CMD_LIST)
              {
                at_cmd_list_ptr[cnt].cmd_buff_len = at_cmd_info.len;
                if (MAX_CMD_SIZE < at_cmd_info.len)
                {
                  LOG_MSG_INFO2_0("Invalid command length in TLV");
                  errval = QMI_ERR_MALFORMED_MSG;
                  goto send_result;
                }
                memscpy((byte*)at_cmd_list_ptr[cnt].cmd_buff, 
                        at_cmd_info.len,
                       (byte*)at_cmd_info.name,
                       at_cmd_info.len);
                at_cmd_list_ptr[cnt].is_abortable = at_cmd_info.abort_flag;
                cnt++;
              }
              else
              {
                LOG_MSG_ERROR_INT_0("Maximum number of AT commands reached! Ignoring additional commands");
              }

              break;
            }

            default:
            {
              if(len_lvl2 != PS_UTILS_DSM_PULLUP( sdu_in, value, len_lvl2))
              {
                errval = QMI_ERR_MALFORMED_MSG;
                goto send_result;
              }
              break;
            }
          }
        }
        break;
      }

      default:
      {
        LOG_MSG_INFO2_1 ("Unrecognized TLV type (%d)", type);
        if(len_lvl1 != PS_UTILS_DSM_PULLUP( sdu_in, value, len_lvl1))
        {
          errval = QMI_ERR_MALFORMED_MSG;
          goto send_result;
        }
        break;
      }
    }
  }

  /* Find out number of valid commands existing to update the new commands */
   index = at_cl_sp->cmd_info.cmd_cnt;
   LOG_MSG_INFO2_1("Current command count [%d]", index);

  /* Check the scope to add the new list */
  if ( (index >= MAX_CMD_LIST) || (cnt >= MAX_CMD_LIST) || ((index + cnt) >= MAX_CMD_LIST))
  {
    LOG_MSG_ERROR_2("MAX commands already registered by client current index [%d] new additions [%d]"
                                                                                         ,index,cnt);
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC(cmd_reg_buf, sizeof(qmi_ati_cmd_reg_buf_type),
                           qmi_ati_cmd_reg_buf_type*);
  if(cmd_reg_buf == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }
  cmd_reg_buf->at_cmd_list_ptr = at_cmd_list_ptr;
  cmd_reg_buf->cnt = cnt;
  cmd_reg_buf->client = client;
  /*Save the cmd_buf to send response later*/
  cmd_reg_buf->pending_cmd_buf = cmd_buf_p;

  /*Save the pending buf to check with in response from atcop*/
  at_cl_sp->pending_info = cmd_reg_buf;
  /*send the async buf as the user_data to valiadate later*/
  qmi_resp_buf_p =  (ds_qmi_fw_async_cmd_buf_type*) ds_qmi_fw_new_async_cmd_buf(cmd_buf_p, client);
  if(qmi_resp_buf_p == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  if(FALSE == dsatclient_register_fwd_at_cmd
                (
                  qmi_ati_global.atcop_cl_id,
                  cnt,
                  at_cmd_list_ptr,
                  qmi_ati_reg_at_cmd_cback,
                  qmi_ati_fwd_at_cmd_cback,
                  qmi_ati_abort_at_cmd_cback,
                  qmi_resp_buf_p))
  {
    LOG_MSG_ERROR_INT_0("Unable to register with ATCoP");
    errval = QMI_ERR_INTERNAL;
    at_cl_sp->pending_info = NULL;
    goto send_result;
  }

  // Set legacy command registration to true 
  at_cl_sp->is_legacy_reg = TRUE;

  return QMI_SVC_RESPONSE_PENDING;
 
send_result:
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS 
                                   : QMI_RESULT_FAILURE);
  PS_SYSTEM_HEAP_MEM_FREE(at_cmd_list_ptr);
  
  /* This is to avoid double free in cmd_reg_buf*/
  if(cmd_reg_buf != NULL)
  {
    cmd_reg_buf->at_cmd_list_ptr = NULL;
  }
  
  ds_qmi_fw_free_async_cmd_buf(qmi_resp_buf_p);
  QMI_AT_FREE_CMD_REG_BUF(cmd_reg_buf);

  retval = qmi_svc_put_result_tlv(&response,
                                  result,
                                  errval); 
  CHECK_RETVAL();

  return response;

} /* qmi_ati_reg_at_cmd_fwd() */


/*===========================================================================
  FUNCTION QMI_ATI_REG_AT_CMD_FWD_EX()

  DESCRIPTION
    Reg AT cmds for forwarding
  
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
static dsm_item_type * qmi_ati_reg_at_cmd_fwd_ex
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *       response;
  dsat_cmd_list_type  *at_cmd_list_ptr;
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  uint8    cnt;
  uint32   client;
  uint32   index =0;
  qmi_ati_cmd_reg_buf_type *cmd_reg_buf = NULL;
  qmi_ati_client_state_type * at_cl_sp;
  ds_qmi_fw_async_cmd_buf_type *qmi_resp_buf_p = NULL;
  at_req_at_cmd_fwd_ex_req_v01   req_msg;
  at_req_at_cmd_fwd_ex_resp_v01  resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0("qmi_ati_reg_at_cmd_fwd_ex");
  DS_QMI_AT_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  
  memset(&req_msg, 0, sizeof(at_req_at_cmd_fwd_ex_req_v01) );
  memset(&resp_msg, 0, sizeof(at_req_at_cmd_fwd_ex_resp_v01));    

  PS_SYSTEM_HEAP_MEM_ALLOC( at_cmd_list_ptr, sizeof(dsat_cmd_list_type) * MAX_CMD_LIST,
                            dsat_cmd_list_type*);
  if (at_cmd_list_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  memset(at_cmd_list_ptr, 0, (sizeof(dsat_cmd_list_type ) * MAX_CMD_LIST));
  
  /* Match the client corresponding to the cl_sp received */
  at_cl_sp = (qmi_ati_client_state_type *) cl_sp;
  client = qmi_ati_get_at_client_id_by_cl_sp(at_cl_sp);


  if(!qmi_ati_validate_client_id(client,__LINE__))
  {
    errval = QMI_ERR_INVALID_CLIENT_ID_V01;
    goto send_result;
  }

  if(at_cl_sp->pending_info != NULL)
  {
    LOG_MSG_INFO1_0("New reg at cmd req arrived when another one is pending");
    errval = QMI_ERR_INVALID_OPERATION_V01;
    goto send_result;
  }

  if( ATI_AT_REG_DONE != qmi_ati_global.reg_at_client )
  {
    LOG_MSG_ERROR_1("command registration is requested before AT client registration for client = [%d]"
                   ,client);
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
 
  errval = QMI_ATI_DECODE_REQUEST_MSG(QMI_AT_REG_AT_CMD_FWD_EX_REQ_V01,
  	                              sdu_in,
  	                              &req_msg,
  	                              sizeof(at_req_at_cmd_fwd_ex_req_v01));
  if (errval != QMI_ERR_NONE_V01)
  {
    LOG_MSG_INFO2_0("Decoding of message failed");
    errval = QMI_ERR_MALFORMED_MSG_V01;
    goto send_result;
  }
                                       
  for (cnt = 0; cnt < req_msg.at_cmd_len; cnt++)
  {
    at_cmd_list_ptr[cnt].cmd_buff_len = strlen(req_msg.at_cmd[cnt].at_cmd_name);
    if (MAX_CMD_SIZE < strlen(req_msg.at_cmd[cnt].at_cmd_name))
    {
      LOG_MSG_INFO2_0("Invalid command length in TLV");
      errval = QMI_ERR_MALFORMED_MSG_V01;
      goto send_result;
    }
    memscpy((byte*)at_cmd_list_ptr[cnt].cmd_buff, 
            MAX_CMD_SIZE,
            req_msg.at_cmd[cnt].at_cmd_name,
            strlen(req_msg.at_cmd[cnt].at_cmd_name));
    at_cmd_list_ptr[cnt].is_abortable = req_msg.at_cmd[cnt].abort_flag;  
  }

  /* Find out number of valid commands existing to update the new commands */
   index = at_cl_sp->cmd_info.cmd_cnt;
   LOG_MSG_INFO2_1("Current command count [%d]", index);

  /* Check the scope to add the new list */
  if ( (index > MAX_CMD_LIST) || (cnt > MAX_CMD_LIST) || ((index + cnt) > MAX_CMD_LIST))
  {
    LOG_MSG_ERROR_2("MAX commands already registered by client current index [%d] new additions [%d]"
                                                                                         ,index,cnt);
    errval = QMI_ERR_REQUESTED_NUM_UNSUPPORTED_V01;
    goto send_result;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC(cmd_reg_buf, sizeof(qmi_ati_cmd_reg_buf_type),
                           qmi_ati_cmd_reg_buf_type*);
  if(cmd_reg_buf == NULL)
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  cmd_reg_buf->at_cmd_list_ptr = at_cmd_list_ptr;
  cmd_reg_buf->cnt = cnt;
  cmd_reg_buf->client = client;
  /*Save the cmd_buf to send response later*/
  cmd_reg_buf->pending_cmd_buf = cmd_buf_p;

  /*Save the pending buf to check with in response from atcop*/
  at_cl_sp->pending_info = cmd_reg_buf;
  /*send the async buf as the user_data to valiadate later*/
  qmi_resp_buf_p =  (ds_qmi_fw_async_cmd_buf_type*) ds_qmi_fw_new_async_cmd_buf(cmd_buf_p, client);
  if(qmi_resp_buf_p == NULL)
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }

  if(FALSE == dsatclient_register_fwd_at_cmd
                (
                  qmi_ati_global.atcop_cl_id,
                  cnt,
                  at_cmd_list_ptr,
                  qmi_ati_reg_at_cmd_cback,
                  qmi_ati_fwd_at_cmd_cback,
                  qmi_ati_abort_at_cmd_cback,
                  qmi_resp_buf_p))
  {
    LOG_MSG_ERROR_INT_0("Unable to register with ATCoP");
    errval = QMI_ERR_INTERNAL_V01;
    at_cl_sp->pending_info = NULL;
    goto send_result;
  }

  return QMI_SVC_RESPONSE_PENDING;
 
send_result:
  PS_SYSTEM_HEAP_MEM_FREE(at_cmd_list_ptr);  
  /* This is to avoid double free in cmd_reg_buf*/
  if(cmd_reg_buf != NULL)
  {
    cmd_reg_buf->at_cmd_list_ptr = NULL;
  }
  ds_qmi_fw_free_async_cmd_buf(qmi_resp_buf_p);
  QMI_AT_FREE_CMD_REG_BUF(cmd_reg_buf);
 
  QMI_ATI_ENCODE_RESPONSE_MSG(QMI_AT_REG_AT_CMD_FWD_EX_RESP_V01,
                              &resp_msg,
                              sizeof(at_req_at_cmd_fwd_ex_resp_v01),
                              errval,
                              &response);
  return response;
} /* qmi_ati_reg_at_cmd_fwd_ex() */


/*===========================================================================
  FUNCTION QMI_ATI_SEND_AT_RESPONSE()

  DESCRIPTION
    
    
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
static dsm_item_type * qmi_ati_send_at_repsonse
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *    response;
  qmi_ati_client_state_type  * at_cl_sp;
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  int32    temp;
  dsm_item_type *    sdu_dup; 
  uint32    client; 
  at_fwd_resp_at_cmd_req_v01  req_msg;
  at_fwd_resp_at_cmd_resp_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0("qmi_ati_send_at_repsonse");
  DS_QMI_AT_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);
  
  response = NULL;
  
  memset(&req_msg, 0, sizeof(at_fwd_resp_at_cmd_req_v01) );
  memset(&resp_msg, 0, sizeof(at_fwd_resp_at_cmd_resp_v01));      

  at_cl_sp = (qmi_ati_client_state_type *) cl_sp;

  client = qmi_ati_get_at_client_id_by_cl_sp(at_cl_sp);
  if(!qmi_ati_validate_client_id(client,__LINE__))
  {
    /* Client not registered for AT Fwding */
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  temp = PS_UTILS_DSM_DUP_PACKET( &sdu_dup, *sdu_in, 0, 
                                 (uint16) dsm_length_packet(*sdu_in));
  if (0 == temp)
  {
    dsm_free_packet(sdu_in);
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  
  errval = QMI_ATI_DECODE_REQUEST_MSG(QMI_AT_FWD_RESP_AT_CMD_REQ_V01,
  	                              sdu_in,
  	                              &req_msg,
  	                              sizeof(at_fwd_resp_at_cmd_req_v01));  
  if (errval != QMI_ERR_NONE_V01)
  {
    LOG_MSG_INFO2_0("Decoding of message failed");
    errval = QMI_ERR_MALFORMED_MSG_V01;
    goto send_result;
  }                                     
   
  /* Pull up type  1 byte, length (var) 2byte, at_handle 4 bytes, result 1 byte
      response_type 1 byte at_resp_len 2 bytes. Total : 11 bytes*/         
  (void)dsm_pull32(&sdu_dup);
  (void)dsm_pull32(&sdu_dup);
  (void)dsm_pull16(&sdu_dup);
  (void)dsm_pull8(&sdu_dup);

  dsatclient_ext_send_response(TRUE, 
                               qmi_ati_global.atcop_cl_id,
                               (dsat_client_result_enum_type) req_msg.resp_at_cmd_req.result,
                               (dsat_fwd_resp_enum_type) req_msg.resp_at_cmd_req.response_type,
                               sdu_dup,
                               req_msg.resp_at_cmd_req.at_handle);

  /* Response is sent for the command; Clean up the abortability. */
  qmi_ati_global.abort_client = ATI_INVALID_CLIENT_INDEX;
send_result:   

  QMI_ATI_ENCODE_RESPONSE_MSG(QMI_AT_FWD_RESP_AT_CMD_RESP_V01,                              
                              &resp_msg,
                              sizeof(at_fwd_resp_at_cmd_resp_v01),
                              errval,
                              &response);
  return response;
  
} /* qmi_ati_send_at_repsonse() */
/*===========================================================================
  FUNCTION QMI_ATI_SEND_AT_URC()

  DESCRIPTION
    This function/message is used by Apps to send any AT URC's. 
    
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
static dsm_item_type * qmi_ati_send_at_urc
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *    response;
  qmi_ati_client_state_type  * at_cl_sp;
  qmi_error_type_v01   errval = QMI_ERR_NONE_V01;
  int32    temp;
  dsm_item_type *    sdu_dup; 
  uint32    client; 
  at_send_at_urc_req_v01  req_msg;
  at_send_at_urc_resp_v01 resp_msg;  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0("qmi_ati_send_at_urc");
  DS_QMI_AT_ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  response = NULL;
  at_cl_sp = (qmi_ati_client_state_type *) cl_sp;

  client = qmi_ati_get_at_client_id_by_cl_sp(at_cl_sp);
  if(!qmi_ati_validate_client_id(client,__LINE__))
  {
    /* Client not registered for AT URC */
    dsm_free_packet(sdu_in);
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result; 
  }
  
  memset(&req_msg, 0, sizeof(at_send_at_urc_req_v01) );
  memset(&resp_msg, 0, sizeof(at_send_at_urc_resp_v01)); 

  temp = PS_UTILS_DSM_DUP_PACKET( &sdu_dup, *sdu_in, 0, 
                                 (uint16) dsm_length_packet(*sdu_in));
  if (0 == temp)
  {
    dsm_free_packet(sdu_in);
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result; 
  }  

  errval = QMI_ATI_DECODE_REQUEST_MSG(QMI_AT_SEND_AT_URC_REQ_V01,
  	                              sdu_in,
  	                              &req_msg,
  	                              sizeof(at_send_at_urc_req_v01));   

  if (errval != QMI_ERR_NONE_V01)
  {
    LOG_MSG_INFO2_0("Decoding of message failed");
    errval = QMI_ERR_MALFORMED_MSG_V01;
    goto send_result;
  }  

  /* Remove type - 1byte length(var) - 2byte and  urc_type - 1 byte, 
     urc array lenght : 1 byte. Total - 5 bytes*/
  (void)dsm_pull8(&sdu_dup);
  (void)dsm_pull32(&sdu_dup);


  dsatclient_ext_send_urc(req_msg.urc_req.urc_type, 
                          qmi_ati_global.atcop_cl_id,
                          sdu_dup
                         );
  
send_result: 
  
  QMI_ATI_ENCODE_RESPONSE_MSG(QMI_AT_SEND_AT_URC_RESP_V01,
                              &resp_msg,
                              sizeof(at_send_at_urc_resp_v01),
                              errval,
                              &response);
  return response;  
  
}/* qmi_ati_send_at_urc */


/*===========================================================================
  FUNCTION QMI_ATI_GET_AT_CLIENT_ID_BY_CL_SP()

  DESCRIPTION
    This function returns the client id from a valid client state ptr.
    
  PARAMETERS
    cl_sp : client state ptr

  RETURN VALUE
    client id
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static uint32  qmi_ati_get_at_client_id_by_cl_sp
(
  qmi_ati_client_state_type *  cl_sp
)
{
  if (cl_sp != NULL)
  {
    return cl_sp->common.clid;
  }

  LOG_MSG_ERROR_INT_0("Invalid Client index found");

  return ATI_INVALID_CLIENT_INDEX;

} /* qmi_ati_get_at_client_id_by_cl_sp() */

/*===========================================================================
  FUNCTION qmi_ati_check_valid_async_cmd_buf()

  DESCRIPTION
    This function checks the validity of async_cmd_buf
    
  PARAMETERS
    async_cmd_buf - pointer to the buffer

  RETURN VALUE
     TRUE  - if valid buffer 
     FALSE - if invalid buffer 
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_ati_check_valid_async_cmd_buf
(
  ds_qmi_fw_async_cmd_buf_type * async_cmd_buf
)
{  
  int i; 
  qmi_ati_client_state_type *  cl_sp = NULL;
  qmi_cmd_buf_type * cmd_buf_p = NULL;

  if ((async_cmd_buf != NULL) && (async_cmd_buf->cmd_buf != NULL))
  {
    cmd_buf_p = (qmi_cmd_buf_type *)async_cmd_buf->cmd_buf;

    for(i = 0; i< ATI_MAX_CLIDS; i++)
    {
      cl_sp = (qmi_ati_client_state_type*)qmi_at_state.client[i];
      if( cl_sp && (cl_sp == ((qmi_ati_client_state_type*)cmd_buf_p->x_p->cl_sp) )&& 
          (async_cmd_buf->cookie == qmi_ati_get_at_client_id_by_cl_sp(cl_sp)))
      {
        return TRUE;
      }
    }
  }  
  return FALSE;
}

/*===========================================================================
  FUNCTION QMI_ATI_VALIDATE_CLIENT_ID()

  DESCRIPTION
    This function validates client id.
    
  PARAMETERS
    client : client ID
    line_num :indicates caller line number

  RETURN VALUE
    boolean
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean  qmi_ati_validate_client_id
(
  uint32   client,
  uint16   line_num
)
{
  if ( ( ATI_INVALID_CLIENT_INDEX == client ) ||
  	 ( ATI_MAX_CLIDS < client )|| (client == QMI_SVC_CLID_UNUSED))
  {
    /*invalid client ID*/
    LOG_MSG_ERROR_1("Invalid client ID line num = %d",line_num);
    return FALSE;
  }
  return TRUE;//client id is valid
} /* qmi_ati_validate_client_id() */


/*===========================================================================
  FUNCTION QMI_ATI_GET_AT_CLIENT_BY_CMD_NAME()

  DESCRIPTION
    to get the client id registered for the given command name
    
  PARAMETERS
    cl_sp : client state ptr

  RETURN VALUE
    Id of the client registered for given cmd_name
    QMI_SVC_CLID_UNUSED - if no client registers for given cmd_name
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_ati_client_state_type *  qmi_ati_get_at_client_by_cmd_name
(
  byte *cmd_name
)
{
  uint32                      cli_index = ATI_INVALID_CLIENT_INDEX;
  uint32                      cmd_index = 0; 
  qmi_ati_client_state_type * at_cl_sp = NULL;
  qmi_ati_client_state_type * ret_at_cl_sp = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( cli_index = 0; cli_index < ATI_MAX_CLIDS; cli_index++)
  {
    at_cl_sp = qmi_at_state.client[cli_index];
    if(at_cl_sp == NULL)
    {
      continue;
    }
    for (cmd_index = 0; cmd_index < MAX_CMD_LIST; ++cmd_index )
    {
      if(( 0 == qmi_ati_strncmp_ig_sp_case( (const byte *)cmd_name, 
                         (const byte *)at_cl_sp->cmd_info.reg_cmd_list[cmd_index].cmd_buff,
                         (size_t)strlen((const char*)cmd_name))))
      {
        ret_at_cl_sp = at_cl_sp;
        break;
      }
    }
  }

  if(NULL == ret_at_cl_sp)
  {
    LOG_MSG_ERROR_INT_0("Invalid Command received");
  }

  return ret_at_cl_sp;
} /* qmi_ati_get_at_client_by_cmd_name() */

/*===========================================================================
  FUNCTION QMI_ATI_RESET_CLIENT()

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
static void qmi_ati_reset_client
(
  void * cl_sp
)
{
  qmi_ati_client_state_type * at_cl_sp;
  int i;
  qmi_transaction_type *  x_p;
  uint32                  client;
  qmi_ati_cmd_reg_buf_type * cmd_reg_buf = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  at_cl_sp = (qmi_ati_client_state_type *) cl_sp;

  client = qmi_ati_get_at_client_id_by_cl_sp(at_cl_sp);

  LOG_MSG_INFO2_1( "qmi_ati_reset_client %d",client );

  if(!qmi_ati_validate_client_id(client,__LINE__))
  {
    /* Client not registered for AT Fwding */
    return;
  }

  
  /* Clear any pending transactions */
  if( at_cl_sp->pending_info != NULL)
  {
     cmd_reg_buf = at_cl_sp->pending_info;
     at_cl_sp->pending_info = NULL;

    /* Clear only for the client which is requesting the Reset.  */
     x_p = cmd_reg_buf->pending_cmd_buf->x_p;
      ds_qmi_fw_free_transaction( &x_p );
 
     /* Make sure that the pending registration for commands have corresponding de-registeration */
     if( dsatclient_deregister_fwd_at_cmd(qmi_ati_global.atcop_cl_id,
                                         cmd_reg_buf->cnt,
                                         cmd_reg_buf->at_cmd_list_ptr) == FALSE )
     {
       LOG_MSG_ERROR_INT_1("Error de-registering pending commands for cleint = d",client);
       DS_QMI_AT_ASSERT(0);
    }
     QMI_AT_FREE_CMD_REG_BUF(cmd_reg_buf);
  }

  /* Clear the stored registerd List of commands for this client. */
  /* Find out number of registered commands */
  if (at_cl_sp->cmd_info.cmd_cnt > 0)
  {
    if( dsatclient_deregister_fwd_at_cmd(qmi_ati_global.atcop_cl_id,
                                         at_cl_sp->cmd_info.cmd_cnt,
                                         at_cl_sp->cmd_info.reg_cmd_list) == FALSE )
  {
      LOG_MSG_ERROR_INT_1("Error de-registering commands for cleint = d",client);
      DS_QMI_AT_ASSERT(0);
  }
  }
  /*reset the cmd info struct*/
  memset(&at_cl_sp->cmd_info, 0x0, sizeof(at_cl_sp->cmd_info));

  /* If this is the last client registered, then clean up all the AT registration.  */
  for(client = 0; client < ATI_MAX_CLIDS;++client )
  {
    if (qmi_at_state.client[client] != NULL)
    {
      break;
    }
  }

  if ((client >= ATI_MAX_CLIDS) && (qmi_ati_global.reg_at_client == ATI_AT_REG_DONE))
  {
    /* Last client to Reset; Deregister with  ATCoP  */
    LOG_MSG_INFO2_0("Deregistering with Atcop");
    if(dsatclient_deregister_fwd_client(qmi_ati_global.atcop_cl_id) != TRUE)
    {
      DS_UTILS_ERROR_FATAL("Unable to deregister with ATCoP");
    }
    qmi_ati_global.reg_at_client = ATI_AT_REG_NONE;
    qmi_ati_global.atcop_cl_id = DSAT_INVALID_CLIENT_ID;
  }

  /*-------------------------------------------------------------------------
    Free resources for client
  -------------------------------------------------------------------------*/
  /*-------------------------------------------------------------------------
    q_init() is used here as a way to clear the queue and not as a way to 
    initialize the queue.
  -------------------------------------------------------------------------*/ 
  (void) q_init( &at_cl_sp->common.x_free_q );
  for( i = 0; i < MAX_PENDING_TRANSACTIONS; i++ )
  {
    x_p = &at_cl_sp->common.transactions[i];
    ds_qmi_fw_free_transaction( &x_p );
  }

  /*-------------------------------------------------------------------------
    clear event reporting
  -------------------------------------------------------------------------*/
} /* qmi_ati_reset_client() */

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
static void qmi_ati_process_cmd
(
  void *cmd_ptr
)
{
  qmi_ati_cmd_buf_type      * cmd_buf_ptr = NULL;
  uint32                      client_id = QMI_SVC_CLID_UNUSED;
  qmi_ati_client_state_type * at_cl_sp = NULL;  
/*-------------------------------------------------------------------------*/

  if (NULL == cmd_ptr)
  {
    DS_UTILS_ERROR_FATAL("Ignoring process request on NULL buffer");
    return;
  }
  cmd_buf_ptr = (qmi_ati_cmd_buf_type *)cmd_ptr;

  switch(cmd_buf_ptr->cmd_id)
  {
     case QMI_CMD_AT_INIT_CB:
      qmi_ati_process_svc_init(cmd_buf_ptr->data.init_cb.num_instances);
      break;

    case QMI_CMD_AT_ALLOC_CLID_CB:
      qmi_ati_process_alloc_clid(&cmd_buf_ptr->data.clid_cb.common_msg_hdr);
      break;

    case QMI_CMD_AT_DEALLOC_CLID_CB:
      qmi_ati_process_dealloc_clid(&cmd_buf_ptr->data.clid_cb.common_msg_hdr);
      break;

    case QMI_CMD_AT_CMD_HDLR_CB:
      qmi_ati_process_cmd_hdlr(&cmd_buf_ptr->data.cmd_hdlr_cb.msg_hdr,
                                    cmd_buf_ptr->data.cmd_hdlr_cb.sdu_in );
      break;

    case QMI_CMD_AT_CLIENT_REG_STATUS:
      qmi_ati_process_at_client_reg_status(cmd_buf_ptr->data.client_reg.status,
                                           cmd_buf_ptr->data.client_reg.client_id,
                                           cmd_buf_ptr->data.client_reg.user_data);
      break;

    case QMI_CMD_AT_CMD_REG_STATUS:
      qmi_ati_process_at_cmd_reg_status(cmd_buf_ptr->data.cmd_reg.status,
                                       cmd_buf_ptr->data.cmd_reg.user_data);
      break;

    case QMI_CMD_FWD_AT_CMD:
    {
      qmi_ati_fwd_cmd_buf_type *fwd_cmd_buf_ptr = (qmi_ati_fwd_cmd_buf_type*)cmd_ptr;
      
      at_cl_sp = qmi_ati_get_at_client_by_cmd_name(fwd_cmd_buf_ptr->name);
      if (NULL == at_cl_sp)
      {
        LOG_MSG_INFO1_0("Invalid NULL client ");
        break;
      }

      client_id = qmi_ati_get_at_client_id_by_cl_sp(at_cl_sp);
      if (at_cl_sp->is_legacy_reg)
      {
        qmi_ati_fwd_at_cmd_ind(fwd_cmd_buf_ptr->at_handle,
                               fwd_cmd_buf_ptr->op,
                               fwd_cmd_buf_ptr->name,
                               fwd_cmd_buf_ptr->at_state,
                               fwd_cmd_buf_ptr->num_args,
                               fwd_cmd_buf_ptr->args,
                               client_id);            
      }
      else
      {
        qmi_ati_fwd_at_cmd_ind_ex(fwd_cmd_buf_ptr->at_handle,
                                  fwd_cmd_buf_ptr->op,
                                  fwd_cmd_buf_ptr->name,
                                  fwd_cmd_buf_ptr->at_state,
                                  fwd_cmd_buf_ptr->num_args,
                                  fwd_cmd_buf_ptr->args,
                                  client_id);             
      }                                                  
    }
    
    break;
      
    case QMI_CMD_AT_ABORT_CMD:
      qmi_ati_send_at_cmd_abort_ind(cmd_buf_ptr->data.cmd_abort.handle,
                                   cmd_buf_ptr->data.cmd_abort.user_data);
      break;

    default:
      LOG_MSG_ERROR_INT_1("Unexpected cmd(d) recd in QMI AT cmd handler",
                      cmd_buf_ptr->cmd_id);
      DS_QMI_AT_ASSERT(0);
  }
  qmi_ati_free_cmd_buf(cmd_ptr);
  return;
} /* qmi_ati_process_cmd() */

/*===========================================================================
  FUNCTION QMI_ATI_GET_CMD_BUF()

  DESCRIPTION
    Allocate and assign a QMI AT command buffer from the PS MEM heap
    based on the QMI CMD type
    
  PARAMETERS
    cmd - QMI command type
   
  RETURN VALUE
    cmd_buf_ptr - Pointer to the allocated command buffer

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void *qmi_ati_get_cmd_buf
(
  qmi_cmd_id_e_type cmd_id
)
{
  LOG_MSG_INFO1_1("ati_get_cmd_buf cmd_id = %d",cmd_id);
  switch (cmd_id)
  {
    case QMI_CMD_AT_CLIENT_REG_STATUS:
    case QMI_CMD_AT_CMD_REG_STATUS:
    case QMI_CMD_AT_ABORT_CMD:
    case QMI_CMD_AT_INIT_CB:
    case QMI_CMD_AT_ALLOC_CLID_CB:
    case QMI_CMD_AT_DEALLOC_CLID_CB:
    case QMI_CMD_AT_CMD_HDLR_CB:

    {
      qmi_ati_cmd_buf_type *cmd_buf_ptr;
      PS_SYSTEM_HEAP_MEM_ALLOC(cmd_buf_ptr, sizeof(qmi_ati_cmd_buf_type),
                               qmi_ati_cmd_buf_type*);
      return ((void *)cmd_buf_ptr);
    }

    case QMI_CMD_FWD_AT_CMD:
    {
      qmi_ati_fwd_cmd_buf_type *fwd_cmd_buf_ptr;
      PS_SYSTEM_HEAP_MEM_ALLOC(fwd_cmd_buf_ptr, sizeof(qmi_ati_fwd_cmd_buf_type),
                                qmi_ati_fwd_cmd_buf_type*);
      return ((void *)fwd_cmd_buf_ptr);
    }

    default:
	  break;
  }
  return NULL;
} /* qmi_ati_get_cmd_buf */

/*===========================================================================

FUNCTION QMI_ATI_STRNCMP_IG_SP_CASE

DESCRIPTION
  This function compares two strings for <n> characters, ignoring case.
  Lower case letters are converted to upper case before the 
  difference is taken.

DEPENDENCIES
  None.

RETURN VALUE
  Returns   0 if the strings are identical till <n> characters, except for 
            case otherwise, it returns the difference between 
            the first characters to disagree. The difference is first arg 
            minus second arg.  

SIDE EFFECTS
  None.

===========================================================================*/
int qmi_ati_strncmp_ig_sp_case
(
  const byte *s1,             /*  Pointer to string 1 */
  const byte *s2,             /*  Pointer to string 2 */
  uint32      n               /* Number of characters to compare */
)
{
  byte c1, c2;
  int d;

  if ( n== 0 )
  {
    return -1;
  }
  do
  {
    c1 = *s1++;
    c2 = *s2++;

    d = UPCASE(c1) - UPCASE(c2);
    --n;
  }
  while ( d == 0 && c1 != '\0' && c2 != '\0' && n != 0 );

  return d;
}/* qmi_ati_strncmp_ig_sp_case */

/*===========================================================================
FUNCTION QMI_ATI_INIT_CB()

  DESCRIPTION
  Function gets called from framework whenever the QMI AT service registers with
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
static void qmi_ati_init_cb
(
  uint16 num_instances
)
{
  qmi_ati_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmd_ptr = (qmi_ati_cmd_buf_type *)qmi_ati_get_cmd_buf(QMI_CMD_AT_INIT_CB);
  if( cmd_ptr == NULL)
  {
    return;
  }

  memset(cmd_ptr, 0, sizeof(qmi_ati_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_AT_INIT_CB;
  cmd_ptr->data.init_cb.num_instances = num_instances;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);

} /* qmi_ati_init_cb() */

/*===========================================================================
FUNCTION QMI_ATI_ALLOC_CLID_CB()

DESCRIPTION
  Function gets called from framework whenever a clid is allocated by the
  framework for QMI AT service. This function posts a DCC command for the
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
static boolean qmi_ati_alloc_clid_cb
(  
  qmi_framework_common_msg_hdr_type*   common_msg_hdr  
)
{
  qmi_ati_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_AT_ASSERT(NULL != common_msg_hdr);

  LOG_MSG_INFO1_2("QMI AT alloc clid cb: clid:%d, QMI Instances:%d",
                  common_msg_hdr->client_id, common_msg_hdr->qmi_instance);

  cmd_ptr = (qmi_ati_cmd_buf_type *)qmi_ati_get_cmd_buf(QMI_CMD_AT_ALLOC_CLID_CB);
  if( cmd_ptr == NULL)
  {
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    copy the framework common message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_ati_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_AT_ALLOC_CLID_CB;
  cmd_ptr->data.clid_cb.common_msg_hdr.service = common_msg_hdr->service;
  cmd_ptr->data.clid_cb.common_msg_hdr.client_id = common_msg_hdr->client_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.transaction_id = common_msg_hdr->transaction_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.qmi_instance = common_msg_hdr->qmi_instance;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
  return TRUE;
} /* qmi_ati_alloc_clid_cb */

/*===========================================================================
FUNCTION QMI_ATI_DEALLOC_CLID_CB()

DESCRIPTION 
  Function gets called from framework whenever a clid is deallocated by the
  framework for QMI AT service. This function posts a DCC command for the
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
static void qmi_ati_dealloc_clid_cb
(  
  qmi_framework_common_msg_hdr_type*   common_msg_hdr  
)
{
  qmi_ati_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AT_ASSERT(NULL != common_msg_hdr);
  LOG_MSG_INFO1_2("QMI AT dealloc clid cb: clid:%d, QMI Instances:%d",
                  common_msg_hdr->client_id, common_msg_hdr->qmi_instance);

  cmd_ptr = (qmi_ati_cmd_buf_type *)qmi_ati_get_cmd_buf(QMI_CMD_AT_DEALLOC_CLID_CB);
  if( cmd_ptr == NULL)
  {
    return;
  }

  /*-------------------------------------------------------------------------
    copy the framework common message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_ati_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_AT_DEALLOC_CLID_CB;
  cmd_ptr->data.clid_cb.common_msg_hdr = *common_msg_hdr;
  cmd_ptr->data.clid_cb.common_msg_hdr.client_id = common_msg_hdr->client_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.transaction_id = common_msg_hdr->transaction_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.qmi_instance = common_msg_hdr->qmi_instance;

  /*-------------------------------------------------------------------------
    Positing command to QMI Service task to handle the clid callback
  -------------------------------------------------------------------------*/
  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
} /* qmi_ati_dealloc_clid_cb */

/*===========================================================================
  FUNCTION FRAMEWORK_ATI_CMD_HDLR_CB()

  DESCRIPTION
    Function gets called from framework whenever a new QMI AT request
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
static void  qmi_ati_cmd_hdlr_cb
( 
  qmi_framework_msg_hdr_type * msg_hdr,
  dsm_item_type             ** sdu 
)
{
  qmi_ati_cmd_buf_type *cmd_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_AT_ASSERT(NULL != msg_hdr);
  LOG_MSG_INFO1_3("qmi_ati_cmd_hdlr_cb: clid:%d, QMI Instances:%d, tx_id: %d",
                  msg_hdr->common_hdr.client_id,
                  msg_hdr->common_hdr.qmi_instance,
                  msg_hdr->common_hdr.transaction_id);
  
  cmd_ptr = (qmi_ati_cmd_buf_type *)qmi_ati_get_cmd_buf(QMI_CMD_AT_CMD_HDLR_CB);
  if( cmd_ptr == NULL)
  {
    return;
  }

  /*-------------------------------------------------------------------------
    copy the framework message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_ati_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_AT_CMD_HDLR_CB;
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
} /* qmi_ati_cmd_hdlr_cb */


/*===========================================================================
  FUNCTION QMI_ATI_PROCESS_SVC_INIT()

  DESCRIPTION
    This function processes a init callback request and intialize the service
    related information.

  PARAMETERS
    num_instances:  number of QMI Instances

  RETURN VALUE
    None

  DEPENDENCIES
    QMI AT must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_process_svc_init
(
  uint16 num_instances
)
{
  qmi_ati_state_type *         at_sp;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_0( "qmi_ati init callback " );
  at_sp = &qmi_at_state;
  memset( at_sp, 0, sizeof(qmi_ati_state_type) );


  /*-------------------------------------------------------------------------
    Max QMI Instances
  -------------------------------------------------------------------------*/
  at_sp->num_qmi_instances = num_instances;
  /*-------------------------------------------------------------------------
    initialize client state non-zero fields
  -------------------------------------------------------------------------*/
 if(qmi_ati_global.inited == FALSE)
  {
    memset(&qmi_ati_global, 0, sizeof(qmi_ati_global));
    qmi_ati_global.inited = TRUE;
    qmi_ati_global.atcop_cl_id = DSAT_INVALID_CLIENT_ID;
    qmi_ati_global.abort_client = ATI_INVALID_CLIENT_INDEX;
  }
} /* qmi_ati_process_svc_init */

/*===========================================================================
  FUNCTION QMI_ATI_PROCESS_ALLOC_CLID()

  DESCRIPTION
    This function processes the client alloc callback request. This function
    allocates and initialize the new client state for the allocated client ID.

  PARAMETERS
    common_msg_hdr:  QMI Framework common message header containing the clid
    alloc request information.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI AT must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_process_alloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
)
{
  qmi_ati_client_state_type		   *  cl_sp;  
  qmi_result_e_type                result = QMI_RESULT_FAILURE;
  boolean                          ret_val = FALSE;
  qmi_error_e_type                 errval = QMI_ERR_NONE;
  qmi_framework_common_msg_hdr_type * msg_hdr = NULL;
/*-------------------------------------------------------------------------*/

  DS_QMI_AT_ASSERT(common_msg_hdr && common_msg_hdr->service);

  cl_sp = NULL;

  LOG_MSG_INFO2_0( "qmi_ati process clid allocation" );
  cl_sp = qmi_ati_alloc_cl_sp(common_msg_hdr->client_id);

  if(cl_sp)
  {
    qmi_ati_initialize_client(cl_sp);
    cl_sp->common.clid = common_msg_hdr->client_id;
    cl_sp->instance    = (uint16)common_msg_hdr->qmi_instance;
    
    LOG_MSG_INFO2_4( "qmi_ati clid set to local client state ptr:0x%x clid %d cmd_hdr %p"
                      "Atcop Registration status:%d", 
                       cl_sp, cl_sp->common.clid, common_msg_hdr,
                       qmi_ati_global.reg_at_client);
    result = QMI_RESULT_SUCCESS; 

    if(qmi_ati_global.reg_at_client == ATI_AT_REG_NONE)
    {
      PS_SYSTEM_HEAP_MEM_ALLOC(msg_hdr, sizeof(qmi_framework_common_msg_hdr_type),
                               qmi_framework_common_msg_hdr_type*);
      if(NULL == msg_hdr)
      {
        result = QMI_RESULT_FAILURE;
      }
      else
      {
        msg_hdr->client_id = common_msg_hdr->client_id;
        msg_hdr->qmi_instance = common_msg_hdr->qmi_instance;
        msg_hdr->service  =  common_msg_hdr->service;
        msg_hdr->transaction_id = common_msg_hdr->transaction_id;
  
        qmi_ati_global.reg_at_client = ATI_AT_REG_PENDING;
        ret_val = dsatclient_register_fwd_client(qmi_ati_reg_at_cback, (void*)msg_hdr);
      
        if (ret_val == FALSE)
        {
          PS_SYSTEM_HEAP_MEM_FREE(msg_hdr);
          qmi_ati_global.reg_at_client = ATI_AT_REG_NONE;
          LOG_MSG_ERROR_INT_0("Alloc clid is failed.Unable to register with ATCoP");
          result = QMI_RESULT_FAILURE;  
          errval = QMI_ERR_INTERNAL;     
        }
        else
        {
           LOG_MSG_INFO1_0("Wait for ATCoP registration to complete to send alloc result");
           /*Wait for ATCoP registration to complete and then send a response to the client*/
          return;
        }
      }
    }
    else
    {
      LOG_MSG_INFO1_1("Already registered/pending with ATcop: %d",
                      qmi_ati_global.reg_at_client);
    }
  }
  else
  {
    LOG_MSG_ERROR_INT_0 ("No available service clids!" );
    errval = QMI_ERR_INTERNAL;
  }

    /* Notifying the clid allocation back to client via QMI Framework*/
    qmi_framework_svc_send_alloc_clid_result_ex(result, common_msg_hdr, errval);
  
} /* qmi_ati_process_alloc_clid */

/*===========================================================================
  FUNCTION QMI_ATI_PROCESS_DEALLOC_CLID()

  DESCRIPTION
    This function processes the client dealloc callback request. This function
    resets and deallocates the new client state for the allocated client ID.

  PARAMETERS
    common_msg_hdr:  QMI Framework common message header containing the clid
    dealloc request information.
 
  RETURN VALUE
    None

  DEPENDENCIES
    QMI AT must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_process_dealloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
)
{
  qmi_ati_client_state_type		   *  cl_sp;   
  qmi_result_e_type                result = QMI_RESULT_FAILURE; 
  qmi_error_e_type                 errval = QMI_ERR_NONE;
/*-------------------------------------------------------------------------*/

  DS_QMI_AT_ASSERT(NULL != common_msg_hdr);

  cl_sp = NULL;
  LOG_MSG_INFO2_0( "qmi_ati process clid deallocation" );

  cl_sp = (qmi_ati_client_state_type *) 
          qmi_at_state.client[common_msg_hdr->client_id  - 1];

  if (cl_sp != NULL && cl_sp->common.clid == common_msg_hdr->client_id)
  {
    qmi_ati_reset_client(cl_sp);
    qmi_ati_dealloc_cl_sp(common_msg_hdr->client_id);
    LOG_MSG_INFO2_1( "qmi_ati clid free to local client state ptr: clid%d",
                     common_msg_hdr->client_id );
    result = QMI_RESULT_SUCCESS;
  }
  else
  {
    LOG_MSG_INFO1_0 ("No available service clids!" );
    errval = QMI_ERR_INVALID_CLIENT_ID;
  }

  /* Notifying the clid deallocation back to client via QMI Framework*/
  qmi_framework_svc_send_dealloc_clid_result_ex(result, common_msg_hdr, errval);
} /* qmi_ati_process_dealloc_clid */

/*===========================================================================
FUNCTION QMI_ATI_ALLOC_CL_SP()

  DESCRIPTION
  This function gets called whenever a client state need to be allocated
  given a valid client id. The client state has allocated and deallocated
  dynamically at runtime.
    
  PARAMETERS
  clid: client ID

  RETURN VALUE
  qmi_ati_client_state_type * : ptr to QMI AT client state.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_ati_client_state_type * qmi_ati_alloc_cl_sp
(
  uint8 clid
)
{
  qmi_ati_client_state_type *  cl_sp = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(qmi_ati_validate_client_id(clid,__LINE__))
  {
    PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(cl_sp, sizeof( qmi_ati_client_state_type),
                                    qmi_ati_client_state_type*);
      qmi_at_state.client[clid - 1] = cl_sp;
  }
  return cl_sp;
} /* qmi_ati_alloc_cl_sp */

/*===========================================================================
FUNCTION QMI_ATI_DEALLOC_CL_SP()

DESCRIPTION
  This function gets called whenever a client state needs to be deallocated
  given a valid client id. The client state is allocated and deallocated
  dynamically at runtime.

PARAMETERS
    clid: client ID

RETURN VALUE
  TRUE: client Id deallocated.
  FALSE: client ID not deallocated. Error.
 
DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static boolean qmi_ati_dealloc_cl_sp
(
  uint8 clid
)
{
  qmi_ati_client_state_type *  cl_sp = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_1( "qmi_ati_dealloc_cl_sp clid :%d", clid );

  if (!qmi_ati_validate_client_id(clid,__LINE__))
  {
    return FALSE;
  }

  cl_sp = (qmi_ati_client_state_type *) qmi_at_state.client[clid - 1];
  if(cl_sp)
  {
    q_destroy(&cl_sp->common.x_free_q);
    q_destroy(&cl_sp->common.cmd_free_q);

    PS_SYSTEM_HEAP_MEM_FREE(cl_sp);
    qmi_at_state.client[clid - 1] = NULL;
  }
  else
  {
    LOG_MSG_ERROR_INT_0("Client state pointer is already NULL");
    return FALSE;
  }

  return TRUE;
} /* qmi_ati_dealloc_cl_sp */


/*===========================================================================
  FUNCTION QMI_ATI_INITIALIZE_CLIENT()

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
static void qmi_ati_initialize_client
(
  qmi_ati_client_state_type * cl_sp
)
{
  qmi_ati_state_type *    at_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_AT_ASSERT(NULL != cl_sp);
  at_sp = &qmi_at_state;
  memset( cl_sp , 0, sizeof(qmi_ati_client_state_type) );

  LOG_MSG_INFO2_0( "qmi_ati_initialize_client " );
  cl_sp->instance     = ATI_INVALID_INSTANCE;
  cl_sp->common.clid  = QMI_SVC_CLID_UNUSED;
  
  ds_qmi_fw_common_cl_init(at_sp, &cl_sp->common);

} /* qmi_ati_initialize_client() */

/*===========================================================================
  FUNCTION QMI_ATI_PROCESS_CMD_HDLR()

  DESCRIPTION
    This function processes the service command handler request callback.
    This function creates a transaction and dispatches to the appropriate
    message handler
    
  PARAMETERS
    msg_hdr:  Framework message header for the request.
    sdu_in : request

  RETURN VALUE
    None

  DEPENDENCIES
    QMI AT must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_ati_process_cmd_hdlr
(
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type * sdu_in 
)
{
  qmi_ati_client_state_type 	* cl_sp;
  
/*-------------------------------------------------------------------------*/

  DS_QMI_AT_ASSERT(msg_hdr && sdu_in);
  
  if(!qmi_ati_validate_client_id(msg_hdr->common_hdr.client_id,__LINE__))
  {
    return;
  }

  
  LOG_MSG_INFO2_3( "Process QMI AT svc command handlr callback function: clid%d, tx_id:%d, ctl_flag:%d ",
                   msg_hdr->common_hdr.client_id,
                   msg_hdr->common_hdr.transaction_id,
                   msg_hdr->msg_ctl_flag );

  /*-------------------------------------------------------------------------
    Get client state pointer
  -------------------------------------------------------------------------*/
  cl_sp = (qmi_ati_client_state_type *) 
             qmi_at_state.client[msg_hdr->common_hdr.client_id - 1];
  

  if (cl_sp == NULL)
  {
    LOG_MSG_ERROR_INT_0 ("Process QMI AT svc command handlr callback function: No valid client state!");
    DS_QMI_AT_ASSERT(cl_sp != NULL);
  }
  else
  {
    ds_qmi_fw_recv(&qmi_ati_cfg, (void*)&qmi_at_state,
  	             msg_hdr, &cl_sp->common, sdu_in);
  }
  
} /* qmi_ati_process_cmd_hdlr */

/*===========================================================================
FUNCTION QMI_ATI_SEND_RESPONSE()

DESCRIPTION
  This function calls QMI Framework API to send a QMI AT Service response
  message to clients.

PARAMETERS 
  cmd_buf_p: command buffer
  msg_ptr : dsm item

RETURN VALUE
  TRUE: If responses send to Framework
  FALSE: otherwise

DEPENDENCIES
  QMI AT service must be initialized and registered with Framework

SIDE EFFECTS
  None
===========================================================================*/
static boolean qmi_ati_send_response
(
  qmi_cmd_buf_type *               cmd_buf_p,
  dsm_item_type *                  msg_ptr
)
{
  qmi_ati_client_state_type *	       cl_sp;
  qmi_framework_common_msg_hdr_type    common_hdr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if ( (cmd_buf_p == NULL) || (msg_ptr == NULL) ||
       (cmd_buf_p->x_p == NULL) || (cmd_buf_p->x_p->cl_sp == NULL) )
  {
    DS_UTILS_ERROR_FATAL("Unable to send response to framework!");
    return FALSE;
  }
  
  cl_sp = (qmi_ati_client_state_type *)cmd_buf_p->x_p->cl_sp;
  
  common_hdr.service        = QMUX_SERVICE_AT;
  common_hdr.client_id	    = cl_sp->common.clid;
  common_hdr.transaction_id = cmd_buf_p->x_p->x_id;
  common_hdr.qmi_instance   = cl_sp->instance;
  
  return ds_qmi_fw_send_response(&common_hdr, cmd_buf_p, msg_ptr);

} /* qmi_ati_send_response */

#endif /* FEATURE_DATA_QMI_AT */
