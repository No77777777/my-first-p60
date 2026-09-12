/*===========================================================================

                         D S _ Q M I _ W D A . C

DESCRIPTION

  The Data Services QMI Wireless Data Administrative Service source file.

EXTERNALIZED FUNCTIONS

  qmi_wda_init()
    Initialize the QMI WDA service

Copyright (c) 2012-2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/qmidata/src/ds_qmi_wda.c#3 $
  $DateTime: 2022/11/08 05:26:32 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/06/15    wc     Fix packet filtering issue
11/18/14    sk     QMI-WDS/WDA Services to use Enc/Dec IDL lib.
06/24/14    wc     QMAP DL min padding
10/22/13    sah    Corrected validation check in qmi_wdai_packet_filter_add_rule that was rejecting 
                   valid mandatory TLV due to optional or malformed tlv
09/27/13    wc     Support dynamic port configuration
04/25/13    wc     MBIM mPDN support
04/11/13    wc     Add UL aggregation threholds TLVs
08/24/12    gk     Made sure that rmenet set_data_format is not called on second 
                   instance when NO_EFFECT error is returned in first call
08/06/12    gk     Added support for QMI logging on what messages/TLVs 
                   are supported
07/26/12    sb     Fixed KW Errors.
07/11/12    pgm    Added Support for RNDIS agg protocol.
02/02/12    cy     Added support for MBIM NTB parameters
02/01/12    cy     Support for packet filter feature
09/19/11    sy     Created module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"

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
#include "dcc_task_defs.h"
#include "dcc_task_svc.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "qmi_svc_defs.h"
#include "qmi_svc_utils.h"
#include "qmi_framework.h"
#include "ds_qmi_wda.h"
#include "ds_rmnet_meta_sm.h"
#include "ds_rmnet_utils.h"
#include "ds_rmnet_xport.h"
#include "ds_rmnet_shim.h"

#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_sys_conf.h"
#include "ps_dpm_dl_opt.h"
#include "modem_mem.h"
#include "qmi_si.h"
#include "qmi_idl_lib.h"
#include "wireless_data_administrative_service_v01.h"
#include "ps_utils.h"
#include "ps_sys_dins.h"

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Major and Minor Version Nos for WDA
---------------------------------------------------------------------------*/
#define WDAI_BASE_VER_MAJOR    (1)
#define WDAI_BASE_VER_MINOR    (16)

#define WDAI_ADDENDUM_VER_MAJOR  (0)
#define WDAI_ADDENDUM_VER_MINOR  (0)

#define WDA_MAX_CLIDS_INSTANCE  (4)
#define WDA_MAX_CLIDS    (WDA_MAX_CLIDS_INSTANCE * QMI_INSTANCE_MAX)

#define WDA_MAX_POWERSAVE_MODE_CLIENTS 3 /* QMI and HW for DL marker */
#define WDS_POWERSAVE_DISABLE_ALL_MASK 0

#define DS_QMI_WDA_ASSERT(expression) \
         ds_qmi_wda_assert_wrapper(__LINE__, expression)


/*===========================================================================

                                DATA TYPES

===========================================================================*/


/*---------------------------------------------------------------------------
  WDA Command enum type - not equal to the actual command values!
  mapping is in qmi_wda_cmd_callbacks table

  DO NOT REORDER THIS ENUM!
---------------------------------------------------------------------------*/
typedef enum
{
  WDAI_CMD_MIN                          = 0,
  WDAI_CMD_SET_DATA_FORMAT              = WDAI_CMD_MIN,
  WDAI_CMD_GET_DATA_FORMAT                 ,
  WDAI_CMD_PACKET_FILTER_ENABLE,
  WDAI_CMD_PACKET_FILTER_DISABLE,
  WDAI_CMD_PACKET_FILTER_GET_STATE,
  WDAI_CMD_PACKET_FILTER_ADD_RULE,
  WDAI_CMD_PACKET_FILTER_DELETE_RULE,
  WDAI_CMD_PACKET_FILTER_GET_RULE_HANDLES,
  WDAI_CMD_PACKET_FILTER_GET_RULE,
  WDAI_CMD_SET_LOOPBACK_STATE,
  WDAI_CMD_GET_LOOPBACK_STATE,
  WDAI_CMD_SET_QMAP_SETTINGS,
  WDAI_CMD_GET_QMAP_SETTINGS,
  WDAI_CMD_SET_POWERSAVE_CONFIG,
  WDAI_CMD_SET_POWERSAVE_MODE,
  WDAI_CMD_MAX,
  WDAI_CMD_WIDTH                        = 0xFFFF
} qmi_wdai_cmd_e_type;

typedef enum
{
  WDAI_CMD_VAL_SET_DATA_FORMAT               = 0x0020,
  WDAI_CMD_VAL_GET_DATA_FORMAT               = 0x0021,
  WDAI_CMD_VAL_PACKET_FILTER_ENABLE             = 0x0022,
  WDAI_CMD_VAL_PACKET_FILTER_DISABLE            = 0x0023,
  WDAI_CMD_VAL_PACKET_FILTER_GET_STATE          = 0x0024,
  WDAI_CMD_VAL_PACKET_FILTER_ADD_RULE           = 0x0025,
  WDAI_CMD_VAL_PACKET_FILTER_DELETE_RULE        = 0x0026,
  WDAI_CMD_VAL_PACKET_FILTER_GET_RULE_HANDLES   = 0x0027,
  WDAI_CMD_VAL_PACKET_FILTER_GET_RULE           = 0x0028,
  WDAI_CMD_VAL_SET_LOOPBACK_STATE               = 0x0029,
  WDAI_CMD_VAL_GET_LOOPBACK_STATE               = 0x002A,
  WDAI_CMD_VAL_SET_QMAP_SETTINGS                = 0x002B,
  WDAI_CMD_VAL_GET_QMAP_SETTINGS                = 0x002C,
  WDAI_CMD_VAL_SET_POWERSAVE_CONFIG             = 0x002D,
  WDAI_CMD_VAL_SET_POWERSAVE_MODE               = 0x002E,

/* QC EXTERNAL QMI COMMAND RANGE IS 0x0000 - 0x5555. 
   Add the next external QMI Command here */

/* VENDOR SPECIFIC QMI COMMAND RANGE IS 0x5556 - 0xAAAA.
   IMPORTANT!
   Add the vendor specific QMI Commands within this range only to avoid 
   conflicts with QC QMI commands that would get released in future */

/* RESERVED QC QMI COMMAND RANGE IS 0xAAAB - 0xFFFE */
  WDAI_CMD_VAL_WIDTH                      = 0xFFFF
} qmi_wdai_cmd_val_e_type;


/*---------------------------------------------------------------------------
  QMI WDA instance state definition &
  WDA client state definition
---------------------------------------------------------------------------*/

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
      void           * user_data;
      dsm_item_type  * pkt;
    } wdai_resp;
  } data;
} qmi_wdai_cmd_buf_type;

/*---------------------------------------------------------------------------
  QMI WDA Service configuration
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_framework_svc_config_type    fw_cfg;
  qmi_svc_cmd_hdlr_type *          cmd_hdlr_array;
  uint16                           cmd_num_entries;
  void *                           sp;
  boolean                          registered;   /* Service registration status */
} qmi_wda_svc_config_type;

typedef struct qmi_wdai_client_state_s
{
  uint8      client_id;
  uint32     ep_id;
  uint32     requested_mask;
  struct qmi_wdai_client_state_s *next_ptr;
} qmi_wdai_client_state_type;

typedef struct
{
  qmi_wdai_client_state_type *client_list;
  uint8  num_clients;
} qmi_wdai_state_type;

static qmi_wdai_state_type  qmi_wda_state[QMI_INSTANCE_MAX];
qmi_idl_service_object_type qmi_wda_svc_obj;

/*---------------------------------------------------------------------------
  Clients information registered for powersave mode
---------------------------------------------------------------------------*/
typedef struct
{
  uint32 ep_id;
  uint32 supported_mask;
  uint8  num_clients;
  qmi_wda_powersave_client_info_type client_info_arr[WDA_MAX_POWERSAVE_MODE_CLIENTS];
} qmi_wdai_powersave_service_info_type;

qmi_wdai_powersave_service_info_type qmi_wda_powersave_service_info;

/*===========================================================================

                               INTERNAL DATA

===========================================================================*/

/*---------------------------------------------------------------------------
  QMI service command handlers
  forward declarations & cmd handler dispatch table definition
---------------------------------------------------------------------------*/

static dsm_item_type* qmi_wdai_set_data_format(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_get_data_format(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_packet_filter_enable(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_packet_filter_disable(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_packet_filter_get_state(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_packet_filter_add_rule(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_packet_filter_delete_rule(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_packet_filter_get_rule_handles(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_packet_filter_get_rule(void*, void*, void*, dsm_item_type **);

#define WDA_HDLR(a,b)  QMI_SVC_HDLR( a, (qmi_svc_hdlr_ftype)b )



static dsm_item_type* qmi_wdai_set_loopback_state
(
  void*,
  void*,
  void*,
  dsm_item_type **
);

static dsm_item_type* qmi_wdai_get_loopback_state
(
  void*,
  void*,
  void*,
  dsm_item_type **
);

static dsm_item_type* qmi_wdai_set_qmap_settings(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_get_qmap_settings(void*, void*, void*, dsm_item_type **);

static dsm_item_type* qmi_wdai_set_powersave_config(void*, void*, void*, dsm_item_type **);
static dsm_item_type* qmi_wdai_set_powersave_mode(void*, void*, void*, dsm_item_type **);

static qmi_wdai_client_state_type * qmi_wdai_get_cl_sp_by_clid
(
  uint8 clid
);

static qmi_svc_cmd_hdlr_type  qmi_wdai_cmd_callbacks[WDAI_CMD_MAX] =
{
  WDA_HDLR(WDAI_CMD_VAL_SET_DATA_FORMAT, 
               qmi_wdai_set_data_format),
  WDA_HDLR(WDAI_CMD_VAL_GET_DATA_FORMAT, 
               qmi_wdai_get_data_format),
  WDA_HDLR(WDAI_CMD_VAL_PACKET_FILTER_ENABLE, 
               qmi_wdai_packet_filter_enable),
  WDA_HDLR(WDAI_CMD_VAL_PACKET_FILTER_DISABLE, 
               qmi_wdai_packet_filter_disable),
  WDA_HDLR(WDAI_CMD_VAL_PACKET_FILTER_GET_STATE, 
               qmi_wdai_packet_filter_get_state),
  WDA_HDLR(WDAI_CMD_VAL_PACKET_FILTER_ADD_RULE, 
               qmi_wdai_packet_filter_add_rule),
  WDA_HDLR(WDAI_CMD_VAL_PACKET_FILTER_DELETE_RULE, 
               qmi_wdai_packet_filter_delete_rule),
  WDA_HDLR(WDAI_CMD_VAL_PACKET_FILTER_GET_RULE_HANDLES, 
               qmi_wdai_packet_filter_get_rule_handles),
  WDA_HDLR(WDAI_CMD_VAL_PACKET_FILTER_GET_RULE, 
               qmi_wdai_packet_filter_get_rule),
  WDA_HDLR(WDAI_CMD_VAL_SET_LOOPBACK_STATE, 
               qmi_wdai_set_loopback_state),
  WDA_HDLR(WDAI_CMD_VAL_GET_LOOPBACK_STATE, 
               qmi_wdai_get_loopback_state),
  WDA_HDLR(WDAI_CMD_VAL_SET_QMAP_SETTINGS, 
               qmi_wdai_set_qmap_settings),
  WDA_HDLR(WDAI_CMD_VAL_GET_QMAP_SETTINGS, 
               qmi_wdai_get_qmap_settings),
  WDA_HDLR(WDAI_CMD_VAL_SET_POWERSAVE_CONFIG, 
               qmi_wdai_set_powersave_config),
  WDA_HDLR(WDAI_CMD_VAL_SET_POWERSAVE_MODE, 
               qmi_wdai_set_powersave_mode)
};

/*---------------------------------------------------------------------------
  QMI callback definition
---------------------------------------------------------------------------*/
static boolean qmi_wdai_alloc_clid_cb
(
  qmi_framework_common_msg_hdr_type * svc_common_hdr
);

static void qmi_wdai_dealloc_clid_cb
(
  qmi_framework_common_msg_hdr_type * svc_common_hdr
); 

static void qmi_wdai_init_cb
(
  uint16 num_instances
);

static void qmi_wdai_cmd_hdlr_cb
( 
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type ** sdu
);

/*---------------------------------------------------------------------------
  QMI Service (QMI_WDA) configuration definition
---------------------------------------------------------------------------*/
static qmi_wda_svc_config_type  qmi_wdai_cfg;

static struct
{ 
  boolean inited;
} qmi_wdai_global = {FALSE,};

/*===========================================================================

                       FORWARD FUNCTION DECLARATIONS 

===========================================================================*/
static void qmi_wdai_process_cmd(void *);

static void qmi_wdai_process_svc_init
(
  uint16 num_instances
);

static void qmi_wdai_process_cmd_hdlr
(
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type * sdu_in 
);

static void qmi_wdai_process_alloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
);

static void qmi_wdai_process_dealloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
);

static void qmi_wdai_send_response
(
  qmi_framework_msg_hdr_type *  msg_hdr,
  dsm_item_type              *  msg_ptr
);

static uint32 qmi_wdai_parse_data_format_req
(
  wda_set_data_format_req_msg_v01* req_msg,
  rmnet_data_format_type* data_format
);

static void *qmi_wdai_get_cmd_buf(qmi_cmd_id_e_type);

boolean qmi_wdai_encode_msg
(
  uint16_t message_id,
  void *resp_msg,
  uint32_t size,
  qmi_error_type_v01 err_code,
  dsm_item_type **response
);

qmi_error_type_v01 qmi_wdai_decode_msg
(
  uint16_t message_id,
  dsm_item_type **msg_ptr,
  void *req_msg,
  uint32_t req_size
);

static void qmi_wdai_powersave_dl_marker_cb
(
  uint32  ep_id,
  uint32  mask
);

/*-------------------------------------------------------------------------
  Macros 
---------------------------------------------------------------------------*/
#define QMI_WDAI_ENCODE_RESPONSE_MSG(message_id,resp_msg,size,err_code,response) \
         qmi_wdai_encode_msg(message_id,resp_msg,size,err_code,response)

#define QMI_WDAI_DECODE_REQUEST_MSG(message_id,sdu_in,req_msg,req_size)\
         qmi_wdai_decode_msg(message_id,sdu_in,req_msg,req_size);

/*===========================================================================

FUNCTION DS_QMI_WDA_ASSERT_WRAPPER()

DESCRIPTION
  Wrapper function for DS_QMI_WDA ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void ds_qmi_wda_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DS_QMI_WDA_ERR_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* ds_qmi_wda_assert_wrapper */

/*===========================================================================
  FUNCTION QMI_WDA_INIT()

  DESCRIPTION
    Register the WDA service with QMI Framework 

  PARAMETERS
    None

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_wda_init
(
  void
)
{
  qmi_framework_err_e_type  reg_result = QMI_FRAMEWORK_ERR_NONE;
  uint32                 idl_major_ver = WDAI_BASE_VER_MAJOR;
  uint32                 idl_minor_ver = WDAI_BASE_VER_MINOR;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0("QMI WDA service init");

  /*-------------------------------------------------------------------------
    Setting QMI WDA service command handler with dcc task process
  -------------------------------------------------------------------------*/
  qmi_task_set_svc_cmd_handler(QMUX_SERVICE_WDA, qmi_wdai_process_cmd);

  /*-------------------------------------------------------------------------
    QMI WDS Admin service configuration setup
  -------------------------------------------------------------------------*/
  /* Retrieving IDL major and minor version for QMI WDA service */
#ifndef TEST_FRAMEWORK  
  qmi_idl_get_idl_version(wda_get_service_object_v01(),&idl_major_ver);
  qmi_idl_get_idl_minor_version(wda_get_service_object_v01(),&idl_minor_ver);
#endif /* TEST_FRAMEWORK */
  qmi_wdai_cfg.fw_cfg.base_version.major     = idl_major_ver;
  qmi_wdai_cfg.fw_cfg.base_version.minor     = idl_minor_ver;

  qmi_wdai_cfg.fw_cfg.addendum_version.major = WDAI_ADDENDUM_VER_MAJOR;
  qmi_wdai_cfg.fw_cfg.addendum_version.minor = WDAI_ADDENDUM_VER_MINOR;

  qmi_wdai_cfg.fw_cfg.cbs.alloc_clid         = qmi_wdai_alloc_clid_cb;
  qmi_wdai_cfg.fw_cfg.cbs.dealloc_clid       = qmi_wdai_dealloc_clid_cb;
  qmi_wdai_cfg.fw_cfg.cbs.init_cback         = qmi_wdai_init_cb;
  qmi_wdai_cfg.fw_cfg.cbs.cmd_hdlr           = qmi_wdai_cmd_hdlr_cb;

  qmi_wdai_cfg.cmd_hdlr_array                = qmi_wdai_cmd_callbacks;
  qmi_wdai_cfg.cmd_num_entries               = WDAI_CMD_MAX;

  /*-------------------------------------------------------------------------
    Calling QMI Framework API to register the service with Framework
  -------------------------------------------------------------------------*/
  reg_result = qmi_framework_reg_service( QMUX_SERVICE_WDA,
                                          &qmi_wdai_cfg.fw_cfg );

  if (reg_result != QMI_FRAMEWORK_ERR_NONE)
  {
    DS_QMI_WDA_ASSERT(0);
    return;
  }
  qmi_wda_svc_obj =  wda_get_service_object_v01();

  qmi_wdai_cfg.registered  = TRUE;

  /*-------------------------------------------------------------------------
     Register to listen powersave mode with DL marker mask for RMNET shim
  -------------------------------------------------------------------------*/
  qmi_wda_reg_powersave_mode(QMI_WDA_POWERSAVE_CONFIG_DL_MARKER,
                             qmi_wdai_powersave_dl_marker_cb);
} /* qmi_wda_init */

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/

/*===========================================================================
  FUNCTION qmi_wdai_process_cmd()

  DESCRIPTION
    This function processes a QMI WDA command

    It is called by the QMI command handler and will dispatch the
    associated command/event handler function.

  PARAMETERS
    cmd_ptr:  private data buffer containing the QMI WDA command
              information.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI WDA must already have been initialized

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_wdai_process_cmd
(
  void *cmd_ptr
)
{
  qmi_wdai_cmd_buf_type *cmd_buf_ptr = NULL;
/*-------------------------------------------------------------------------*/

  DS_QMI_WDA_ASSERT(cmd_ptr != NULL);
  cmd_buf_ptr = (qmi_wdai_cmd_buf_type *)cmd_ptr;

  LOG_MSG_INFO1_1("QMI WDA process cmd: cmd_id:%d", cmd_buf_ptr->cmd_id);

  switch(cmd_buf_ptr->cmd_id)
  {
    case QMI_CMD_WDA_INIT_CB:
    {
      qmi_wdai_process_svc_init(cmd_buf_ptr->data.init_cb.num_instances);
      break;
    }

    case QMI_CMD_WDA_CMD_HDLR_CB:
    {
      qmi_wdai_process_cmd_hdlr(&cmd_buf_ptr->data.cmd_hdlr_cb.msg_hdr,
                                    cmd_buf_ptr->data.cmd_hdlr_cb.sdu_in );
      break;
    }

    case QMI_CMD_WDA_ALLOC_CLID_CB:
    {
      qmi_wdai_process_alloc_clid(&cmd_buf_ptr->data.clid_cb.common_msg_hdr);
      break;
    }

    case QMI_CMD_WDA_DEALLOC_CLID_CB:
    {
      qmi_wdai_process_dealloc_clid(&cmd_buf_ptr->data.clid_cb.common_msg_hdr);
      break;
    }

    default:
      DS_QMI_WDA_ASSERT(0);
  }
  PS_SYSTEM_HEAP_MEM_FREE(cmd_ptr);
  return;
} /* qmi_wdai_process_cmd() */

/*===========================================================================
FUNCTION QMI_WDAI_INIT_CB()

  DESCRIPTION
  Function gets called from framework whenever the QMI WDA service registers with
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
static void qmi_wdai_init_cb
(
  uint16 num_instances
)
{
  qmi_wdai_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  cmd_ptr = (qmi_wdai_cmd_buf_type *)qmi_wdai_get_cmd_buf(QMI_CMD_WDA_INIT_CB);
  if( cmd_ptr == NULL)
  {
    return;
  }

  memset(cmd_ptr, 0, sizeof(qmi_wdai_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_WDA_INIT_CB;
  cmd_ptr->data.init_cb.num_instances = num_instances;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);

} /* qmi_wdai_init_cb() */

/*===========================================================================
FUNCTION QMI_WDAI_ALLOC_CLID_CB()

DESCRIPTION
  Function gets called from framework whenever a clid is allocated by the
  framework for QMI WDA service. This function posts a DCC command for the
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
static boolean qmi_wdai_alloc_clid_cb
(  
  qmi_framework_common_msg_hdr_type*   common_msg_hdr
)
  {
  qmi_wdai_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_WDA_ASSERT(common_msg_hdr != NULL);

  LOG_MSG_INFO1_2("QMI WDA alloc clid cb: clid:%d, QMI Instances:%d",
                  common_msg_hdr->client_id, common_msg_hdr->qmi_instance);

  cmd_ptr = (qmi_wdai_cmd_buf_type *)qmi_wdai_get_cmd_buf(QMI_CMD_WDA_ALLOC_CLID_CB);
  if( cmd_ptr == NULL)
    {
    return FALSE;
    }

  /*-------------------------------------------------------------------------
    copy the framework common message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_wdai_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_WDA_ALLOC_CLID_CB;
  cmd_ptr->data.clid_cb.common_msg_hdr.service = common_msg_hdr->service;
  cmd_ptr->data.clid_cb.common_msg_hdr.client_id = common_msg_hdr->client_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.transaction_id = common_msg_hdr->transaction_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.qmi_instance = common_msg_hdr->qmi_instance;

  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
  return TRUE;
} /* qmi_wdai_alloc_clid_cb */

/*===========================================================================
FUNCTION QMI_WDAI_DEALLOC_CLID_CB()

DESCRIPTION 
  Function gets called from framework whenever a clid is deallocated by the
  framework for QMI WDA service. This function posts a DCC command for the
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
static void qmi_wdai_dealloc_clid_cb
(  
  qmi_framework_common_msg_hdr_type*   common_msg_hdr  
)
{
  qmi_wdai_cmd_buf_type *cmd_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_QMI_WDA_ASSERT(common_msg_hdr != NULL);
  LOG_MSG_INFO1_2("QMI WDA dealloc clid cb: clid:%d, QMI Instances:%d",
                  common_msg_hdr->client_id, common_msg_hdr->qmi_instance);

  cmd_ptr = (qmi_wdai_cmd_buf_type *)qmi_wdai_get_cmd_buf(QMI_CMD_WDA_DEALLOC_CLID_CB);
  if( cmd_ptr == NULL)
  {
    return;
  }

  /*-------------------------------------------------------------------------
    copy the framework common message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_wdai_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_WDA_DEALLOC_CLID_CB;
  cmd_ptr->data.clid_cb.common_msg_hdr.service = common_msg_hdr->service;
  cmd_ptr->data.clid_cb.common_msg_hdr.client_id = common_msg_hdr->client_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.transaction_id = common_msg_hdr->transaction_id;
  cmd_ptr->data.clid_cb.common_msg_hdr.qmi_instance = common_msg_hdr->qmi_instance;

  /*-------------------------------------------------------------------------
    Positing command to QMI Service task to handle the clid callback
  -------------------------------------------------------------------------*/
  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
} /* qmi_wdai_dealloc_clid_cb */

/*===========================================================================
  FUNCTION FRAMEWORK_WDAI_CMD_HDLR_CB()

  DESCRIPTION
    Function gets called from framework whenever a new QMI WDA request
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
static void  qmi_wdai_cmd_hdlr_cb
( 
  qmi_framework_msg_hdr_type * msg_hdr,
  dsm_item_type             ** sdu 
)
{
  qmi_wdai_cmd_buf_type *cmd_ptr;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_WDA_ASSERT(msg_hdr != NULL);
  LOG_MSG_INFO1_3("qmi_wdai_cmd_hdlr_cb: clid:%d, QMI Instances:%d, tx_id: %d",
                  msg_hdr->common_hdr.client_id,
                  msg_hdr->common_hdr.qmi_instance,
                  msg_hdr->common_hdr.transaction_id);
  
  cmd_ptr = (qmi_wdai_cmd_buf_type *)qmi_wdai_get_cmd_buf(QMI_CMD_WDA_CMD_HDLR_CB);
  if( cmd_ptr == NULL)
  {
    return;
  }

  /*-------------------------------------------------------------------------
    copy the framework message header to local struct
  -------------------------------------------------------------------------*/
  memset(cmd_ptr, 0, sizeof(qmi_wdai_cmd_buf_type));
  cmd_ptr->cmd_id = QMI_CMD_WDA_CMD_HDLR_CB;
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
    Posting command to QMI Service task to handle the service request
  -------------------------------------------------------------------------*/
  dcc_send_cmd_ex(DCC_QMI_CMD, cmd_ptr);
} /* qmi_wdai_cmd_hdlr_cb */

/*===========================================================================
  FUNCTION QMI_WDAI_GET_CMD_BUF()

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
static void *qmi_wdai_get_cmd_buf
(
  qmi_cmd_id_e_type cmd_id
)
{
  LOG_MSG_INFO1_1("wdai_get_cmd_buf cmd_id = %d",cmd_id);
  switch (cmd_id)
  {
    case QMI_CMD_WDA_INIT_CB:
    case QMI_CMD_WDA_ALLOC_CLID_CB:
    case QMI_CMD_WDA_DEALLOC_CLID_CB:
    case QMI_CMD_WDA_CMD_HDLR_CB:
    {
      qmi_wdai_cmd_buf_type *cmd_buf_ptr;
      PS_SYSTEM_HEAP_MEM_ALLOC(cmd_buf_ptr, sizeof(qmi_wdai_cmd_buf_type),
                               qmi_wdai_cmd_buf_type*);
      return ((void *)cmd_buf_ptr);
    }

    default:
	break;
  }

  return NULL;
} /* qmi_wdai_get_cmd_buf */

/*===========================================================================
  FUNCTION QMI_WDAI_PROCESS_SVC_INIT()

  DESCRIPTION
    This function processes a init callback request and intialize the service
    related infos.

  PARAMETERS
    num_instances:  nnumber of QMI Instances

  RETURN VALUE
    None

  DEPENDENCIES
    QMI WDA must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_wdai_process_svc_init
(
  uint16 num_instances
)
{
/*-------------------------------------------------------------------------*/
  /* check that num_instances is within range in case ASSERT is removed */
  if (num_instances > QMI_INSTANCE_MAX)
  {
    LOG_MSG_ERROR_INT_1("QMI num inst out of range (d)!", num_instances);
    DS_QMI_WDA_ASSERT(0);
    return;
  }

  LOG_MSG_INFO1_0( "qmi_wdai init callback " );
  memset( qmi_wda_state, 0, sizeof(qmi_wdai_state_type) * QMI_INSTANCE_MAX );

  /*-------------------------------------------------------------------------
    initialize client state non-zero fields
  -------------------------------------------------------------------------*/
  if(qmi_wdai_global.inited == FALSE)
  {
    qmi_wdai_global.inited = TRUE;
  }
} /* qmi_wdai_process_svc_init */

/*===========================================================================
  FUNCTION QMI_WDAI_PROCESS_CMD_HDLR()

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
    QMI WDA must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_wdai_process_cmd_hdlr
(
  qmi_framework_msg_hdr_type* msg_hdr,
  dsm_item_type * sdu_in 
)
{
  int32                         temp;
  uint16                        cmd_type;
  uint16                        sdu_len;
  uint16                        remaining_bytes;
  qmi_svc_cmd_hdlr_type         *cmd_hdlr;
  uint16                        cmd;
  boolean                       retval;
  dsm_item_type *               response = NULL;
  uint8                         err_val =0;   
  qmi_wdai_state_type           *wda_sp = NULL;
  qmi_wdai_client_state_type    *cl_sp = NULL;

  #define ERR_SVC_HDR_CMD_TYPE 1
  #define ERR_SVC_HDR_SDU_LEN  2
  #define ERR_SVC_SDU_LEN      3
  #define ERR_SVC_NO_RESPONSE  4
/*-------------------------------------------------------------------------*/

  DS_QMI_WDA_ASSERT(msg_hdr && sdu_in);
  LOG_MSG_INFO1_3( "Process QMI WDA svc command handler callback function: clid%d, tx_id:%d, ctl_flag:%d ",
                   msg_hdr->common_hdr.client_id,
                   msg_hdr->common_hdr.transaction_id,
                   msg_hdr->msg_ctl_flag );

  /*-----------------------------------------------------------------------
    Extract service message header
  -----------------------------------------------------------------------*/
  remaining_bytes = (uint16) dsm_length_packet(sdu_in);

  temp = dsm_pull16( &sdu_in ); 
  if (temp == -1)
  {
    err_val = ERR_SVC_HDR_CMD_TYPE;
    dsm_free_packet( &sdu_in );
    goto send_result;
  }
  cmd_type = ps_ntohs(temp);
  
  temp = dsm_pull16( &sdu_in );
  if (temp == -1)
  {
    err_val = ERR_SVC_HDR_SDU_LEN;
    dsm_free_packet( &sdu_in );
    goto send_result;

  }

  sdu_len = ps_ntohs(temp);
  remaining_bytes -= 4;
  
  if (sdu_len > remaining_bytes)
  {
    err_val = ERR_SVC_SDU_LEN;
    dsm_free_packet( &sdu_in );
    goto send_result;
  }
  else if (sdu_len < remaining_bytes)
  {
    /* no bundling for now */
    LOG_MSG_ERROR_3 ("QMI WDA service does not support bundling message into one transaction. sdu_len %d, remaining_bytes %d, msg_len %d",
                     sdu_len, remaining_bytes, msg_hdr->msg_len);
    dsm_trim_packet( &sdu_in, sdu_len );
  }

  /*-------------------------------------------------------------------------
    Process the service request and send a response back to client via
    QMI Framework.
  -------------------------------------------------------------------------*/
  cmd_hdlr = qmi_wdai_cfg.cmd_hdlr_array;
  for (cmd = 0; cmd < qmi_wdai_cfg.cmd_num_entries; cmd++, cmd_hdlr++)
  {
    if (cmd_type == cmd_hdlr->cmd_type)
    {
      break;
    }
  }

  /* Send the response if request is invalid.*/
  if ( cmd == qmi_wdai_cfg.cmd_num_entries )
  {
    LOG_MSG_ERROR_1( "Unrecognized type (=%d) for WDAI service ! Return error",
                     cmd_type  );

    retval = qmi_svc_put_result_tlv( &response,
                                     (qmi_result_e_type)QMI_RESULT_FAILURE_V01,
                                     (qmi_error_e_type)QMI_ERR_INVALID_QMI_CMD_V01);
    if (FALSE == retval)
    {
     dsm_free_packet(&response);
     response = NULL;
    }

    dsm_free_packet(&sdu_in);

  }
  else
  {
    /*-------------------------------------------------------------------------
      call appropirate QMI WDSI ADMIN message handler and send the response back to
      QMI Framework.
    -------------------------------------------------------------------------*/
    if (msg_hdr->common_hdr.qmi_instance > QMI_INSTANCE_MAX)
    {
      LOG_MSG_ERROR_INT_1("QMI inst out of range (d)!", msg_hdr->common_hdr.qmi_instance);
      DS_QMI_WDA_ASSERT(0);
      return;
    }
    wda_sp = (qmi_wdai_state_type*) &qmi_wda_state[msg_hdr->common_hdr.qmi_instance];
    cl_sp = (qmi_wdai_client_state_type *)qmi_wdai_get_cl_sp_by_clid(msg_hdr->common_hdr.client_id);
    if(wda_sp == NULL || cl_sp == NULL )
    {
      err_val = ERR_SVC_NO_RESPONSE;
      goto send_result;
    }

    response = cmd_hdlr->request_hdlr( wda_sp, msg_hdr, cl_sp, &sdu_in );

    dsm_free_packet(&sdu_in);

    if (response == NULL)
    {
      err_val = ERR_SVC_NO_RESPONSE;
      goto send_result;
    }

  }

  if(FALSE == qmi_svc_prepend_msg_hdr(&response, cmd_type)) 
  {
    return;
  }

  msg_hdr->msg_ctl_flag = QMI_FLAG_MSGTYPE_RESP;
  msg_hdr->msg_len  = (uint16) dsm_length_packet(response);

  /*-------------------------------------------------------------------------
    Send response if ready.
  -------------------------------------------------------------------------*/
  qmi_wdai_send_response( msg_hdr, response );
  send_result:
   if(err_val != 0)
   {
     LOG_MSG_ERROR_2("error %d service %x",err_val,msg_hdr->common_hdr.service);
   }
   return;

} /* qmi_wdai_process_cmd_hdlr */

/*===========================================================================
  FUNCTION QMI_WDAI_INITIALIZE_CLIENT()

  DESCRIPTION
    Initialize the new client state for the given client state ptr.
    
  PARAMETERS
    cl_sp :  client state pointer
    wda_sp : WDA service state pointer
    client_id : client identity
    
  RETURN VALUE
    None
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_wdai_initialize_client
(
  qmi_wdai_client_state_type  * cl_sp,
  qmi_wdai_state_type         * wda_sp,
  uint8                       client_id  
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_WDA_ASSERT(cl_sp && wda_sp);
  memset(cl_sp, 0x0, sizeof(qmi_wdai_client_state_type));

  cl_sp->client_id = client_id;
  cl_sp->next_ptr = NULL;
} /* qmi_wdai_initialize_client() */

/*===========================================================================
  FUNCTION QMI_WDAI_VALIDATE_CLIENT_ID()

  DESCRIPTION
    Initialize the new client state for the given client state ptr.
    
  PARAMETERS
    clid : client identity
    
  RETURN VALUE
    TRUE - if clid is valid 
    FALSE  if clid is invalid
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean  qmi_wdai_validate_client_id
(
  uint32   clid
)
{
  if ((clid == QMI_SVC_CLID_UNUSED ) ||
      (clid > WDA_MAX_CLIDS) )
  {
    LOG_MSG_INFO2_1( "Invalid client ID clid :%d", clid );
    return FALSE;
  }
  return TRUE;//client id is valid
} /* qmi_wdai_validate_client_id() */

/*===========================================================================
  FUNCTION QMI_WDAI_ALLOC_CLIENT()

  DESCRIPTION
    Utility function to allocate a new client for wda service
    
  PARAMETERS
    common_msg_hdr  : QMI Framework common msg hdr
    wda_sp :     WDA service state pointer
    
  RETURN VALUE
   Valid client state pointer allocated
   NULL if some error occurs
    
  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_wdai_client_state_type * wdai_alloc_client
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr,
  qmi_wdai_state_type             *wda_sp
)
{
  qmi_wdai_client_state_type *  cl_sp = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( (common_msg_hdr == NULL) ||
       (common_msg_hdr->service != QMUX_SERVICE_WDA) ||
       (common_msg_hdr->qmi_instance >= QMI_INSTANCE_MAX) ||
       (common_msg_hdr->client_id == QMI_SVC_CLID_UNUSED) ||
       (common_msg_hdr->client_id > WDA_MAX_CLIDS) ||
       (wda_sp->num_clients >= WDA_MAX_CLIDS_INSTANCE))  /*check for max clients per instnace*/
  {
    LOG_MSG_ERROR_INT_0("Invalid common_msg_hdr");
    return FALSE;
  }
  /*check if this client_id is already allocated*/
  cl_sp = (qmi_wdai_client_state_type *)qmi_wdai_get_cl_sp_by_clid(common_msg_hdr->client_id);
  if(cl_sp != NULL)
  {
     LOG_MSG_ERROR_INT_1("WDA client_id [%d] is already allocated",
                     common_msg_hdr->client_id );
     return NULL;
  }

  PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(cl_sp, sizeof(qmi_wdai_client_state_type),
                                  qmi_wdai_client_state_type*);
  return cl_sp;
}/* wdai_alloc_client */

/*===========================================================================
  FUNCTION QMI_WDAI_PROCESS_ALLOC_CLID()

  DESCRIPTION
    This function process the client alloc callback request. This function
    allocates and initialize the new client state for the allocated client ID.

  PARAMETERS
    common_msg_hdr:  private data buffer containing the clid alloc request information.

  RETURN VALUE
    None

  DEPENDENCIES
    QMI WDA must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_wdai_process_alloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
)
{
  qmi_result_type_v01             result = QMI_RESULT_FAILURE_V01;
  qmi_wdai_state_type             *wda_sp = NULL;
  qmi_wdai_client_state_type      *cl_sp = NULL;
  qmi_error_type_v01              errval = QMI_ERR_NONE_V01;
/*-------------------------------------------------------------------------*/

  DS_QMI_WDA_ASSERT(common_msg_hdr != NULL);

  if (common_msg_hdr->qmi_instance >= QMI_INSTANCE_MAX)
  {
    return;
  }

  wda_sp = (qmi_wdai_state_type *)
           &qmi_wda_state[common_msg_hdr->qmi_instance];

  cl_sp = wdai_alloc_client(common_msg_hdr, wda_sp);
  if( cl_sp != NULL )
  {
    LOG_MSG_INFO1_1( "qmi_wdai process clid:%d allocation", common_msg_hdr->client_id);
    //initialize the client state ptr
    qmi_wdai_initialize_client(cl_sp, wda_sp, common_msg_hdr->client_id);
    //attached new cl_sp to wda_sp client list
    cl_sp->next_ptr = wda_sp->client_list;
    wda_sp->client_list = cl_sp;
    wda_sp->num_clients++;
    //client allocation sucess
    result = QMI_RESULT_SUCCESS_V01;
  }
  else
  {
     LOG_MSG_ERROR_3( "QMI WDA clid allocation failed, qmi_instance [%d], client_id [%d], wda_sp clients:%d",
                       common_msg_hdr->qmi_instance,
                      common_msg_hdr->client_id, 
                      wda_sp->num_clients);
      result = QMI_RESULT_FAILURE_V01;
      errval = QMI_ERR_INTERNAL_V01;
  }

  /* Notifying the clid allocation back to client via QMI Framework*/
   qmi_framework_svc_send_alloc_clid_result_ex(result, common_msg_hdr, 
                                           errval);

} /* qmi_wdai_process_alloc_clid */

/*===========================================================================
  FUNCTION QMI_WDAI_DEALLOC_CLIENT()

  DESCRIPTION
    Utility function to deallocate given client of wda service
    
  PARAMETERS
    common_msg_hdr  : QMI Framework common msg hdr
    errval : Error value if some error occurs
    
  RETURN VALUE
  TRUE - if deallocation is sucess
  FALSE  if deallocation fails

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean wdai_dealloc_clid
(
  qmi_framework_common_msg_hdr_type *common_msg_hdr,
  qmi_error_type_v01                *errval
  
)
{
  qmi_wdai_state_type             *wda_sp = NULL;
  qmi_wdai_client_state_type      *cl_sp = NULL;
  qmi_wdai_client_state_type      *prev_cl_sp = NULL;
/*-------------------------------------------------------------------------*/

  if ( (common_msg_hdr == NULL) ||
       (common_msg_hdr->service != QMUX_SERVICE_WDA) ||
       (common_msg_hdr->qmi_instance >= QMI_INSTANCE_MAX) ||
       (common_msg_hdr->client_id == QMI_SVC_CLID_UNUSED) ||
       (common_msg_hdr->client_id > WDA_MAX_CLIDS) ||
       (errval == NULL))
  {
    DS_UTILS_ERROR_FATAL("Invalid common_msg_hdr");
    return FALSE;
  }

  wda_sp = (qmi_wdai_state_type *)
             &qmi_wda_state[common_msg_hdr->qmi_instance];
  if(wda_sp == NULL || wda_sp->client_list == NULL)
  {
    *errval = QMI_ERR_INTERNAL_V01;
    return FALSE;
  }
  /*Find the realted cl_sp of client_id*/
  cl_sp = wda_sp->client_list;
  do
  {
    if(cl_sp->client_id == common_msg_hdr->client_id)
    {
      break;
    }
    prev_cl_sp = cl_sp;
    cl_sp = cl_sp->next_ptr;
  }while(cl_sp);

  if(cl_sp == NULL)
  {
    *errval = QMI_ERR_INTERNAL_V01;
    return FALSE;
  }
  /*remove cl_sp from the list of clients*/
  if(prev_cl_sp == NULL)
  { 
    /*found one is the first in the list*/
    wda_sp->client_list = cl_sp->next_ptr;
  }
  else
  {
    prev_cl_sp->next_ptr = cl_sp->next_ptr;
  }
  wda_sp->num_clients--;

  /*clear any dependencies with global and other variables, if any*/
  /*release cl_sp */
  PS_SYSTEM_HEAP_MEM_FREE(cl_sp);

  LOG_MSG_INFO2_2( "Released WDA clid %d QMI inst %d",
                 common_msg_hdr->client_id,
                 common_msg_hdr->qmi_instance );
  return TRUE;
}

/*===========================================================================
  FUNCTION QMI_WDAI_GET_CL_SP_BY_CLID()

  DESCRIPTION
    Utility function to get related client state ptr of given client id
    
  PARAMETERS
    clid : client identity

  RETURN VALUE
    Valid client state pointer to  of given clid
    NULL if not found

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_wdai_client_state_type * qmi_wdai_get_cl_sp_by_clid
(
  uint8 clid
)
{
  int i;
  qmi_wdai_client_state_type * cl_sp = NULL;
  qmi_wdai_state_type       * wda_sp = NULL; 
/*-------------------------------------------------------------------------*/

  if(!qmi_wdai_validate_client_id(clid))
  {
    LOG_MSG_ERROR_INT_1("invalid clid [d] passed", clid);
    return NULL;
  }

  for(i=0; i<QMI_INSTANCE_MAX; i++)
  {
    wda_sp = (qmi_wdai_state_type*)&qmi_wda_state[i];
    if( (wda_sp == NULL) || (wda_sp->client_list == NULL) ) //no clients registered for this instance
    {
      continue;
    }
    cl_sp = wda_sp->client_list;
    while(cl_sp)
    {
      if(cl_sp->client_id == clid)
      {
        return cl_sp;
      }
      cl_sp = cl_sp->next_ptr;
    }
  }
  return NULL;
}

/*===========================================================================
  FUNCTION QMI_WDAI_PROCESS_DEALLOC_CLID()

  DESCRIPTION
    This function process the client dealloc callback request. This function
    reset and deallocates the new client state for the allocated client ID.

  PARAMETERS
    common_msg_hdr:  private data buffer containing the clid dealloc
    request information.
 
  RETURN VALUE
    None

  DEPENDENCIES
    QMI WDA must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_wdai_process_dealloc_clid
(
  qmi_framework_common_msg_hdr_type * common_msg_hdr
)
{
  qmi_result_type_v01           result = QMI_RESULT_FAILURE_V01;
  qmi_error_type_v01            errval = QMI_ERR_NONE_V01;
  qmi_wdai_client_state_type    *cl_sp = NULL;
  qmi_wda_powersave_client_info_type * clnt_info_ptr = NULL;
  uint8                                clnt_idx = 0;
  uint32                               ep_id;
  uint32                               mask = WDS_POWERSAVE_DISABLE_ALL_MASK;
/*-------------------------------------------------------------------------*/

  DS_QMI_WDA_ASSERT(common_msg_hdr != NULL);

  if (common_msg_hdr->qmi_instance >= QMI_INSTANCE_MAX)
  {
    return;
  }

  LOG_MSG_INFO1_1( "qmi_wdai process clid: %d deallocation", common_msg_hdr->client_id );

  cl_sp = (qmi_wdai_client_state_type *)qmi_wdai_get_cl_sp_by_clid(common_msg_hdr->client_id);
  if(cl_sp)
  {
    result = wdai_dealloc_clid(common_msg_hdr, &errval) ?
                           QMI_RESULT_SUCCESS_V01 : QMI_RESULT_FAILURE_V01;

    ep_id = cl_sp->ep_id;
    mask = cl_sp->requested_mask;

    /*-------------------------------------------------------------------------
      Remove the client from the list for powersave mode
    -------------------------------------------------------------------------*/
    if(QMI_RESULT_SUCCESS_V01 == result && WDS_POWERSAVE_DISABLE_ALL_MASK != mask)
    {
      for(clnt_idx = 0; clnt_idx < qmi_wda_powersave_service_info.num_clients; ++clnt_idx)
      {
        clnt_info_ptr = &qmi_wda_powersave_service_info.client_info_arr[clnt_idx];
        if(WDS_POWERSAVE_DISABLE_ALL_MASK != (uint32)clnt_info_ptr->conf_mask)
        {
          clnt_info_ptr->callback(ep_id, WDS_POWERSAVE_DISABLE_ALL_MASK);
        }
      }
    }
  }
  else
  {
    result = QMI_RESULT_FAILURE_V01;
    errval = QMI_ERR_INVALID_CLIENT_ID_V01;
  }

  /* Notifying the clid allocation back to client via QMI Framework*/
  qmi_framework_svc_send_dealloc_clid_result_ex(result, common_msg_hdr,
                                              errval);

} /* qmi_wdai_process_dealloc_clid */


/*===========================================================================
FUNCTION QMI_WDAI_SEND_RESPONSE()

DESCRIPTION
  This function calls QMI Framework API to send a QMI WDA Service response to
  clients.

PARAMETERS 
  msg_hdr  : Framework message header for the request.
  response : Response message

RETURN VALUE
  None

DEPENDENCIES
  QMI WDA service must be initialized and registered with Framework

SIDE EFFECTS
  None
===========================================================================*/
static void qmi_wdai_send_response
(
  qmi_framework_msg_hdr_type      *msg_hdr,
  dsm_item_type *                  response
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_QMI_WDA_ASSERT(msg_hdr && response);

  if(TRUE != qmi_framework_svc_send_response(msg_hdr, response))
  {
    LOG_MSG_ERROR_INT_0( "Unable to send QMI WDA response to QMI Framework" );
  }

} /* qmi_wdai_send_response */

/*===========================================================================
  FUNCTION QMI_WDAI_SET_DATA_FORMAT()

  DESCRIPTION
    Handle the QMI WDS Admin set data format message.

    Dispatches a request to the specified service to set the data format.

  PARAMETERS
    svc_sp:      QMI_WDA's service instance state pointer for this qmi link
                 returned by qmux_reg_service()
    msg_hdr:     Message Header
    cl_sp:       Coresponding client state pointer
    sdu_in:      input command data

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_wdai_set_data_format
(
  void           * svc_sp,
  void           * msg_hdr,
  void           * cl_sp,
  dsm_item_type ** sdu_in
)
{
  dsm_item_type *         response;
  qmi_error_type_v01      errval;
  rmnet_data_format_type  data_format;
  uint32                  data_format_mask = 0;
  uint32                  ep_id;
  wda_set_data_format_req_msg_v01* req_msg = NULL;
  wda_set_data_format_resp_msg_v01* resp_msg = NULL;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  errval = QMI_ERR_NONE_V01;
  response = NULL;
  memset((void *)&data_format, 0, sizeof(rmnet_data_format_type));

  PS_SYSTEM_HEAP_MEM_ALLOC(resp_msg,
                           sizeof(wda_set_data_format_resp_msg_v01),
                           wda_set_data_format_resp_msg_v01*);
  if(resp_msg == NULL)
  {
    qmi_svc_put_result_tlv(&response, (qmi_result_e_type)QMI_RESULT_FAILURE_V01,
                           (qmi_error_e_type)QMI_ERR_NO_MEMORY_V01);
    return response;
  }
  memset(resp_msg,0,sizeof(wda_set_data_format_resp_msg_v01));
  
  PS_SYSTEM_HEAP_MEM_ALLOC(req_msg,
                           sizeof(wda_set_data_format_req_msg_v01),
                           wda_set_data_format_req_msg_v01*);
  if(req_msg == NULL)
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  memset(req_msg,0,sizeof(wda_set_data_format_req_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_SET_DATA_FORMAT_REQ_V01,
                                       sdu_in,
                                       req_msg,
                                       sizeof(wda_set_data_format_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  data_format_mask = qmi_wdai_parse_data_format_req(req_msg,&data_format);
  
  /*-------------------------------------------------------------------------
    Check ep_id TLV
  -------------------------------------------------------------------------*/
  ep_id = qmux_get_default_ep_id_from_inst(
      ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance);

  if (req_msg->ep_id_valid)
  {
    // Converting to uint32
    if (QMI_EP_ID_VALID(req_msg->ep_id))
    {
      ep_id = QMI_EP_ID_TO_INTERNAL_EP_ID(req_msg->ep_id);
    }
    else
    {
      LOG_MSG_ERROR_2("Invalid ep_id 0x%x:0x%x",
                      req_msg->ep_id.ep_type, req_msg->ep_id.iface_id);
      errval = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
    }
  }
  /*-------------------------------------------------------------------------
    Perform validity checks for DL max agg pkts.
  -------------------------------------------------------------------------*/
  if ((data_format_mask & RMNET_DATA_FORMAT_MASK_DL_DATA_AGG))
  {
    /*MBIM mode*/
    if (data_format.dl_data_agg_protocol == RMNET_ENABLE_DATA_AGG_MBIM)
    {
      data_format.dl_data_agg_max_num = 
       (req_msg->dl_data_aggregation_max_datagrams > PS_DL_OPT_MBIM_MAX_NDP_AGGR_DGRMS) ? 
       PS_DL_OPT_MBIM_MAX_NDP_AGGR_DGRMS : req_msg->dl_data_aggregation_max_datagrams;
    }
  }

  /*-------------------------------------------------------------------------
    Perform validity checks for Link Protocol Mode
  -------------------------------------------------------------------------*/
  if( (data_format_mask & RMNET_DATA_FORMAT_MASK_LINK_PROT) && 
      ((data_format.link_prot == RMNET_INVALID_MODE) ||
       (data_format.link_prot & (~RMNET_ALL_MODE))) )
  {
    LOG_MSG_INFO2_1 ("Invalid link protocol(s) specified: 0x%x!",
                     data_format.link_prot);
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    Perform validity checks for DL_DATA_AGG_MAX_SIZE.
  -------------------------------------------------------------------------*/
  if ( (data_format_mask & RMNET_DATA_FORMAT_MASK_DL_DATA_AGG_SIZE))
  {
    /*MBIM mode*/
    if ( (data_format.dl_data_agg_protocol == RMNET_ENABLE_DATA_AGG_MBIM) &&
        (data_format.dl_data_agg_max_size < RMNET_DATA_FORMAT_MINIMUM_DL_DATA_AGG_SIZE))
    {
      LOG_MSG_INFO2_1 ("Invalid dl_data_agg_max_size specified for MBIM: 0x%d!",
                       data_format.dl_data_agg_max_size);
      errval = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
    }
  }

   /*-------------------------------------------------------------------------
    Perform action/gather required information for response
    - get the service configuration block of the specified QMI service
    - allocate a client ID from the specified QMI service
  -------------------------------------------------------------------------*/
  LOG_MSG_INFO1_6("Mask:%d, QoS format[%d], Link Prot[%d], "
                  "UL data agg[%d], DL data agg[%d], NDP sig[%d]",
                  data_format_mask,
                  data_format.qos,
                  data_format.link_prot,
                  data_format.ul_data_agg_protocol,
                  data_format.dl_data_agg_protocol,
                  data_format.ndp_signature );
  LOG_MSG_INFO1_4("DL data agg max num[%d], DL data agg max size[%d], "
                  "DL min padding[%d], flow cntl[%d]",
                  data_format.dl_data_agg_max_num,
                  data_format.dl_data_agg_max_size,
                  data_format.dl_min_padding,
                  data_format.te_flow_control);

  if (RMNET_SET_DATA_FORMAT_QMI_CTL == rmnet_phys_xport_data_format_client(ep_id))
  {
    LOG_MSG_ERROR_0("Data format already set by QMI CTL");
    errval = QMI_ERR_INVALID_OPERATION_V01;
    goto send_result;
  }

  if (rmnet_phys_xport_set_data_format(ep_id,
                                   &data_format,
                                   data_format_mask,
                                   RMNET_SET_DATA_FORMAT_QMI_WDA) == FALSE)
  {
    errval = QMI_ERR_INVALID_OPERATION_V01;
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    Attach the flow control TLV
  -------------------------------------------------------------------------*/
  if(data_format.qos == FALSE)
  {
    resp_msg->flow_control = data_format.te_flow_control;
    resp_msg->flow_control_valid = TRUE;
  }
  else
  {
    LOG_MSG_INFO2_0("ignoring flow_control TLV as qos format is set and TRUE");
  }
  /*-------------------------------------------------------------------------
    DL minimum padding
  -------------------------------------------------------------------------*/
  resp_msg->dl_minimum_padding = data_format.dl_min_padding;
  resp_msg->dl_minimum_padding_valid = TRUE;
  
  if (data_format.ul_data_agg_max_num != 0)
  {
    /*-------------------------------------------------------------------------
      Attach the UL data aggregation max size TLV response
    -------------------------------------------------------------------------*/
    resp_msg->ul_data_aggregation_max_size = data_format.ul_data_agg_max_size;
	resp_msg->ul_data_aggregation_max_size_valid = TRUE;
    /*-------------------------------------------------------------------------
      Attach the UL data aggregation max datagrams TLV response
    -------------------------------------------------------------------------*/
    resp_msg->ul_data_aggregation_max_datagrams = data_format.ul_data_agg_max_num;
	resp_msg->ul_data_aggregation_max_datagrams_valid = TRUE;
  }

  /*-------------------------------------------------------------------------
    Attach the DL data aggregation max size TLV response
    Note: DL data aggr max size is returned in bytes for most use cases except
    for AP embedded case modem returns this in Kbytes.
  -------------------------------------------------------------------------*/
  resp_msg->dl_data_aggregation_max_size = data_format.dl_data_agg_max_size;
  resp_msg->dl_data_aggregation_max_size_valid = TRUE;

  /*-------------------------------------------------------------------------
    Attach the DL data aggregation max datagrams TLV response
  -------------------------------------------------------------------------*/
  resp_msg->dl_data_aggregation_max_datagrams = data_format.dl_data_agg_max_num;
  resp_msg->dl_data_aggregation_max_datagrams_valid = TRUE;

  /*-------------------------------------------------------------------------
    Attach the NDP signature TLV response
  -------------------------------------------------------------------------*/
  resp_msg->ndp_signature = data_format.ndp_signature;
  resp_msg->ndp_signature_valid = TRUE;

  /*-------------------------------------------------------------------------
    Attach the DL data aggregation protocol TLV response
  -------------------------------------------------------------------------*/
  resp_msg->dl_data_aggregation_protocol = (wda_dl_data_agg_protocol_enum_v01)data_format.dl_data_agg_protocol;
  resp_msg->dl_data_aggregation_protocol_valid = TRUE;

  /*-------------------------------------------------------------------------
    Attach the UL data aggregation protocol TLV response
  -------------------------------------------------------------------------*/
  resp_msg->ul_data_aggregation_protocol = (wda_ul_data_agg_protocol_enum_v01)data_format.ul_data_agg_protocol;
  resp_msg->ul_data_aggregation_protocol_valid = TRUE;
  
  /*-------------------------------------------------------------------------
    Attach the Link protocol TLV response
  -------------------------------------------------------------------------*/
  resp_msg->link_prot = (wda_link_layer_protocol_enum_v01)data_format.link_prot;
  resp_msg->link_prot_valid =  TRUE;
  
  /*-------------------------------------------------------------------------
    Attach the QoS TLV response
  -------------------------------------------------------------------------*/
  resp_msg->qos_format = data_format.qos;
  resp_msg->qos_format_valid = TRUE;
  
  /*-------------------------------------------------------------------------
    build response
  -------------------------------------------------------------------------*/
send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_SET_DATA_FORMAT_RESP_V01,
                               (void*)resp_msg,
                               sizeof(wda_set_data_format_resp_msg_v01),
                               errval,
                               &response);
  if(req_msg!= NULL)  PS_SYSTEM_HEAP_MEM_FREE(req_msg);
  if(resp_msg!= NULL) PS_SYSTEM_HEAP_MEM_FREE(resp_msg);

  return response;
} /* qmi_wdai_set_data_format */

/*===========================================================================
  FUNCTION QMI_WDAI_GET_DATA_FORMAT()

  DESCRIPTION
    Handle the QMI WDS Admin get data format message.

    Dispatches a request to the specified service to get the data format.

  PARAMETERS
    svc_sp:      QMI_WDA's service instance state pointer for this qmi link
                 returned by qmux_reg_service()
    msg_hdr:     Message Header
    cl_sp:       Coresponding client state pointer
    sdu_in:      input command data

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_wdai_get_data_format
(
  void           * svc_sp,
  void           * msg_hdr,
  void           * cl_sp,
  dsm_item_type ** sdu_in
)
{
  dsm_item_type *         response;
  qmi_error_type_v01        errval;
  rmnet_data_format_type  data_format;
  uint32                  ep_id;
  wda_get_data_format_resp_msg_v01 *resp_msg = NULL;
  wda_get_data_format_req_msg_v01 req_msg;  

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  errval = QMI_ERR_NONE_V01;
  response = NULL;
  memset((void *)&data_format, 0, sizeof(rmnet_data_format_type));
  memset(&req_msg,0,sizeof(wda_get_data_format_req_msg_v01));

  PS_SYSTEM_HEAP_MEM_ALLOC(resp_msg,
                           sizeof(wda_get_data_format_resp_msg_v01),
                           wda_get_data_format_resp_msg_v01*);
  if (resp_msg == NULL)
  {
    qmi_svc_put_result_tlv(&response, (qmi_result_e_type)QMI_RESULT_FAILURE_V01, 
                           (qmi_error_e_type)QMI_ERR_NO_MEMORY_V01);
    return response;
  }
  memset(resp_msg,0,sizeof(wda_get_data_format_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_GET_DATA_FORMAT_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_get_data_format_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  
  /*-------------------------------------------------------------------------
    Check ep_id TLV
  -------------------------------------------------------------------------*/
  ep_id = qmux_get_default_ep_id_from_inst(
      ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance);

  if (req_msg.ep_id_valid)
  {
    // Converting to uint32
    if (QMI_EP_ID_VALID(req_msg.ep_id))
    {
      ep_id = QMI_EP_ID_TO_INTERNAL_EP_ID(req_msg.ep_id);
    }
    else
    {
      LOG_MSG_ERROR_2("Invalid ep_id 0x%x:0x%x",
                      req_msg.ep_id.ep_type, req_msg.ep_id.iface_id);
      errval = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
    }
  }

  if (!rmnet_phys_xport_get_data_format(ep_id, &data_format))
  {
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    Attach the flow control TLV
  -------------------------------------------------------------------------*/
  resp_msg->flow_control = data_format.te_flow_control;
  resp_msg->flow_control_valid = TRUE;
  /*-------------------------------------------------------------------------
    DL minimum padding
  -------------------------------------------------------------------------*/
  resp_msg->dl_minimum_padding = data_format.dl_min_padding;
  resp_msg->dl_minimum_padding_valid = TRUE;

  if (data_format.ul_data_agg_max_num != 0)
  {
    /*-------------------------------------------------------------------------
      Attach the UL data aggregation max size TLV response
    -------------------------------------------------------------------------*/
    resp_msg->ul_data_aggregation_max_size = data_format.ul_data_agg_max_size;
    resp_msg->ul_data_aggregation_max_size_valid = TRUE;
    /*-------------------------------------------------------------------------
      Attach the UL data aggregation max datagrams TLV response
    -------------------------------------------------------------------------*/
    resp_msg->ul_data_aggregation_max_datagrams = data_format.ul_data_agg_max_num;
    resp_msg->ul_data_aggregation_max_datagrams_valid = TRUE;
  }

  /*-------------------------------------------------------------------------
    Attach the DL data aggregation max size TLV response
  -------------------------------------------------------------------------*/
  resp_msg->dl_data_aggregation_max_size = data_format.dl_data_agg_max_size;
  resp_msg->dl_data_aggregation_max_size_valid = TRUE;

  /*-------------------------------------------------------------------------
    Attach the DL data aggregation max datagrams TLV response
  -------------------------------------------------------------------------*/
  resp_msg->dl_data_aggregation_max_datagrams = data_format.dl_data_agg_max_num;
  resp_msg->dl_data_aggregation_max_datagrams_valid = TRUE;

  /*-------------------------------------------------------------------------
    Attach the NDP signature TLV response
  -------------------------------------------------------------------------*/
  resp_msg->ndp_signature = data_format.ndp_signature;
  resp_msg->ndp_signature_valid = TRUE;

  /*-------------------------------------------------------------------------
    Attach the DL data aggregation protocol TLV response
  -------------------------------------------------------------------------*/
  resp_msg->dl_data_aggregation_protocol = 
          (wda_dl_data_agg_protocol_enum_v01)data_format.dl_data_agg_protocol;
  resp_msg->dl_data_aggregation_protocol_valid = TRUE;

  /*-------------------------------------------------------------------------
    Attach the UL data aggregation protocol TLV response
  -------------------------------------------------------------------------*/
  resp_msg->ul_data_aggregation_protocol = 
          (wda_ul_data_agg_protocol_enum_v01)data_format.ul_data_agg_protocol;
  resp_msg->ul_data_aggregation_protocol_valid = TRUE;
  
  /*-------------------------------------------------------------------------
    Attach the Link protocol TLV response
  -------------------------------------------------------------------------*/
  resp_msg->link_prot = (wda_link_layer_protocol_enum_v01)data_format.link_prot;
  resp_msg->link_prot_valid =  TRUE;
  
  /*-------------------------------------------------------------------------
    Attach the QoS TLV response
  -------------------------------------------------------------------------*/
  resp_msg->qos_format = data_format.qos;
  resp_msg->qos_format_valid = TRUE;

/*-------------------------------------------------------------------------
    build response
  -------------------------------------------------------------------------*/
send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_GET_DATA_FORMAT_RESP_V01,
                               (void*)resp_msg,
                               sizeof(wda_get_data_format_resp_msg_v01),
                               errval,
                               &response);
  if(resp_msg!= NULL) PS_SYSTEM_HEAP_MEM_FREE(resp_msg);
  
  return response;
} /* qmi_wdai_get_data_format */

/*===========================================================================
  FUNCTION QMI_WDAI_PACKET_FILTER_ENABLE()

  DESCRIPTION
    Handle the QMI WDA packet filter enable message.

    Dispatches a request to the specified service to
    enable packet filter setting.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_packet_filter_enable
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *        response;
  qmi_error_type_v01       errval;
  qmi_instance_e_type    qmi_instance;
  wda_packet_filter_enable_req_msg_v01 req_msg;
  wda_packet_filter_enable_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  qmi_instance = ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance;

  response          = NULL;
  errval            = QMI_ERR_NONE_V01;

  memset(&req_msg,0,sizeof(wda_packet_filter_enable_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_packet_filter_enable_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_PACKET_FILTER_ENABLE_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_packet_filter_enable_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }

  LOG_MSG_INFO2_1 ("Got Pkt filter restrictive setting: %d", req_msg.filter_is_restrictive);

  if( req_msg.filter_is_restrictive != 0 && req_msg.filter_is_restrictive != 1 )
  {
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  if (!rmnet_utils_packet_filter_change(IPS_ID_TO_FILTER_ID(
                                            req_msg.ips_id_valid,
                                            req_msg.ips_id,
                                            qmi_instance),
                                        TRUE, 
                                        req_msg.filter_is_restrictive))
  {
    errval = QMI_ERR_INTERNAL_V01;
  }

send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_PACKET_FILTER_ENABLE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_packet_filter_enable_resp_msg_v01),
                               errval,
                               &response);
  return response;

} /* qmi_wdai_packet_filter_enable() */

/*===========================================================================
  FUNCTION QMI_WDAI_PACKET_FILTER_DISABLE()

  DESCRIPTION
    Handle the QMI WDA packet filter disable message.

    Dispatches a request to the specified service to
    disable packet filter setting.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_packet_filter_disable
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *        response;
  qmi_error_type_v01       errval;
  qmi_instance_e_type    qmi_instance; 
  wda_packet_filter_disable_req_msg_v01 req_msg;
  wda_packet_filter_disable_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  qmi_instance = ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance;

  response          = NULL;
  errval            = QMI_ERR_NONE_V01;
  memset(&req_msg,0,sizeof(wda_packet_filter_disable_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_packet_filter_disable_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_PACKET_FILTER_DISABLE_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_packet_filter_disable_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  
  if (!rmnet_utils_packet_filter_change(IPS_ID_TO_FILTER_ID(
                                            req_msg.ips_id_valid,
                                            req_msg.ips_id,
                                            qmi_instance),
                                        FALSE, 
                                        FALSE))
  {
    errval = QMI_ERR_INTERNAL_V01;
  }

send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_PACKET_FILTER_DISABLE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_packet_filter_disable_resp_msg_v01),
                               errval,
                               &response);
  return response;
} /* qmi_wdai_packet_filter_disable() */

/*===========================================================================
  FUNCTION QMI_WDAI_PACKET_FILTER_GET_STATE()

  DESCRIPTION
    Handle the QMI WDA packet filter get state message.

    Dispatches a request to the specified service to
    get current packet filter setting.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_packet_filter_get_state
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *         response;
  qmi_error_type_v01      errval;
  qmi_instance_e_type     qmi_instance;
  boolean                 enabled = FALSE;
  boolean                 restrictive = FALSE;
  boolean                 is_filter_q_empty = FALSE; //check for packet filter queue
  wda_packet_filter_get_state_req_msg_v01 req_msg;
  wda_packet_filter_get_state_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&req_msg,0,sizeof(wda_packet_filter_get_state_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_packet_filter_get_state_resp_msg_v01));
  
  qmi_instance = ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance;

  response          = NULL;
  errval            = QMI_ERR_NONE_V01;

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_PACKET_FILTER_GET_STATE_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_packet_filter_get_state_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  
  if(!rmnet_utils_packet_filter_get_state(IPS_ID_TO_FILTER_ID(
                                              req_msg.ips_id_valid,
                                              req_msg.ips_id,
                                              qmi_instance),
                                          &enabled, 
                                          &restrictive,
                                          &is_filter_q_empty))
  {
    errval = QMI_ERR_INTERNAL_V01;
  }
  else
  {
    /*-------------------------------------------------------------------------
      Attach the packet filter pkt allowed TLV response only when packet filter 
      queue is not empty
    -------------------------------------------------------------------------*/
    if(!is_filter_q_empty)
    {
      resp_msg.filter_is_restrictive_valid  = TRUE;
      resp_msg.filter_is_restrictive = restrictive;
    }

    /*-------------------------------------------------------------------------
      Attach the packet filter enabled TLV response
    -------------------------------------------------------------------------*/
    resp_msg.filtering_is_enabled_valid = TRUE;
    resp_msg.filtering_is_enabled = enabled;
  }

send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_PACKET_FILTER_GET_STATE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_packet_filter_get_state_resp_msg_v01),
                               errval,
                               &response);
  return response;
} /* qmi_wdai_packet_filter_get_state() */

/*===========================================================================
  FUNCTION QMI_WDAI_PACKET_FILTER_ADD_RULE()

  DESCRIPTION
    Handle the QMI WDA packet filter add rule message.

    Dispatches a request to the specified service to
    add one packet filter rule entry.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_packet_filter_add_rule
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *        response;
  uint32                 handle = 0;
  qmi_error_type_v01     errval = QMI_ERR_NONE_V01;
  qmi_instance_e_type    qmi_instance;
  wda_packet_filter_add_rule_req_msg_v01* req_msg = NULL;
  wda_packet_filter_add_rule_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  qmi_instance = ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance;
  response          = NULL;
  errval            = QMI_ERR_NONE_V01;
  memset(&resp_msg,0,sizeof(wda_packet_filter_add_rule_resp_msg_v01));
  PS_SYSTEM_HEAP_MEM_ALLOC(req_msg,
                           sizeof(wda_packet_filter_add_rule_req_msg_v01),
                           wda_packet_filter_add_rule_req_msg_v01*);
  if(req_msg == NULL)
  {
    errval = QMI_ERR_NO_MEMORY_V01;
    goto send_result;
  }
  memset(req_msg,0,sizeof(wda_packet_filter_add_rule_req_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_PACKET_FILTER_ADD_RULE_REQ_V01,
                                       sdu_in,
                                       req_msg,
                                       sizeof(wda_packet_filter_add_rule_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  if ((req_msg->rule.pattern_len == 0) 
      || (req_msg->rule.pattern_len > QMI_WDA_PACKET_FILTER_SIZE_MAX_V01) 
      || (req_msg->rule.pattern_len != req_msg->rule.mask_len)) 
  {
    LOG_MSG_ERROR_2 ("Got Pkt filter add config wrong size: "
                     "pattern %d mask%d",
                     req_msg->rule.pattern_len, req_msg->rule.mask_len);
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  /*---------------------------------------------------------------------
    Install fitlers to rm ifaces
  ---------------------------------------------------------------------*/                            
  if(!rmnet_utils_packet_filter_add_rule(IPS_ID_TO_FILTER_ID(
                                             req_msg->ips_id_valid,
                                             req_msg->ips_id,
                                             qmi_instance),
                                         &handle, 
                                         req_msg->rule.pattern_len, 
                                         (req_msg->rule.pattern), 
                                         (req_msg->rule.mask)))
  {
    errval = QMI_ERR_INTERNAL_V01; 
    goto send_result;
  }
  else
  {
    resp_msg.rule_valid = TRUE;
    resp_msg.rule.mask_len = req_msg->rule.mask_len;
    resp_msg.rule.pattern_len = req_msg->rule.pattern_len;
    memscpy(resp_msg.rule.pattern,sizeof(resp_msg.rule.pattern),
            req_msg->rule.pattern,resp_msg.rule.pattern_len);
    memscpy(resp_msg.rule.mask,sizeof(resp_msg.rule.mask),
            req_msg->rule.mask,resp_msg.rule.mask_len);
    resp_msg.handle_valid = TRUE;
    resp_msg.handle = handle;
  }
send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_PACKET_FILTER_ADD_RULE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_packet_filter_add_rule_resp_msg_v01),
                               errval,
                               &response);
  if(req_msg != NULL)
  {
    PS_SYSTEM_HEAP_MEM_FREE(req_msg);
  }
  return response;
} /* qmi_wdai_packet_filter_add_rule() */

/*===========================================================================
  FUNCTION QMI_WDAI_PACKET_FILTER_DELETE_RULE()

  DESCRIPTION
    Handle the QMI WDA packet filter delete rule message.

    Dispatches a request to the specified service to
    delete all or one packet filter rule entry.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_packet_filter_delete_rule
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *         response;
  qmi_error_type_v01        errval;
  qmi_instance_e_type     qmi_instance;
  wda_packet_filter_delete_rule_req_msg_v01 req_msg;
  wda_packet_filter_delete_rule_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  qmi_instance = ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance;

  errval = QMI_ERR_NONE_V01;
  response = NULL;
  memset(&req_msg,0,sizeof(wda_packet_filter_delete_rule_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_packet_filter_delete_rule_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_PACKET_FILTER_DELETE_RULE_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_packet_filter_delete_rule_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  
  /*-------------------------------------------------------------------------
    Perform validity checks for Packet Filter Handle
  -------------------------------------------------------------------------*/
  if( (req_msg.handle_valid) && (req_msg.handle != 0) )
  {
    /* delete one rule using handle */
    if (!rmnet_utils_packet_filter_delete_rule(req_msg.handle))
    {
      errval = QMI_ERR_INTERNAL_V01;
    }
    else
    {
      resp_msg.handle_valid = TRUE;
      resp_msg.handle = req_msg.handle;
    }
  }
  else
  {
    /* delete all rules with handle set to 0 */
    if (!rmnet_utils_packet_filter_delete_all_rules(IPS_ID_TO_FILTER_ID(
                                                        req_msg.ips_id_valid,
                                                        req_msg.ips_id,
                                                        qmi_instance)))
    {
      errval = QMI_ERR_INTERNAL_V01;
    }
  }

  /*-------------------------------------------------------------------------
    build response
  -------------------------------------------------------------------------*/

send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_PACKET_FILTER_DELETE_RULE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_packet_filter_delete_rule_resp_msg_v01),
                               errval,
                               &response);
  return response;
} /* qmi_wdai_packet_filter_delete_rule() */


/*===========================================================================
  FUNCTION QMI_WDAI_PACKET_FILTER_GET_RULE_HANDLES()

  DESCRIPTION
    Handle the QMI WDA packet filter get rule handles message.

    Dispatches a request to the specified service to
    get list of packet filter rule handles.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_packet_filter_get_rule_handles
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *         response;
  qmi_error_type_v01      errval;
  qmi_instance_e_type   qmi_instance;
  wda_packet_filter_get_rule_handles_req_msg_v01 req_msg;
  wda_packet_filter_get_rule_handles_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  qmi_instance = ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance;

  LOG_MSG_INFO2_0 ("Got Pkt filter get config list");

  response          = NULL;
  errval            = QMI_ERR_NONE_V01;
  memset(&req_msg,0,sizeof(wda_packet_filter_get_rule_handles_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_packet_filter_get_rule_handles_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_PACKET_FILTER_GET_RULE_HANDLES_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_packet_filter_get_rule_handles_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }

  if (!rmnet_utils_packet_filter_get_rule_handles(IPS_ID_TO_FILTER_ID(
                                                      req_msg.ips_id_valid,
                                                      req_msg.ips_id,
                                                      qmi_instance),
                                                  (uint8*)&resp_msg.handle_len, 
                                                  &resp_msg.handle[0]))
  {
    errval = QMI_ERR_INTERNAL_V01;
  }
  else
  {
    resp_msg.handle_valid = TRUE;
  }
 
send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_PACKET_FILTER_GET_RULE_HANDLES_RESP_V01,
                               &resp_msg,
                               sizeof(wda_packet_filter_get_rule_handles_resp_msg_v01),
                               errval,
                               &response);
  return response;
} /* qmi_wdai_packet_filter_get_rule_handles() */

/*===========================================================================
  FUNCTION QMI_WDAI_PACKET_FILTER_GET_RULE()

  DESCRIPTION
    Handle the QMI WDA packet filter get rule message.

    Dispatches a request to the specified service to
    get one packet filter rule entry.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_packet_filter_get_rule
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *         response;
  qmi_error_type_v01      errval;
  uint8  rule[QMI_WDA_PACKET_FILTER_SIZE_MAX_V01*2+2];
  wda_packet_filter_get_rule_req_msg_v01 req_msg;
  wda_packet_filter_get_rule_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  response          = NULL;
  errval            = QMI_ERR_NONE_V01;
  memset(&req_msg,0,sizeof(wda_packet_filter_get_rule_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_packet_filter_get_rule_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_PACKET_FILTER_GET_RULE_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_packet_filter_get_rule_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }

  if (req_msg.handle == 0)
  {
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  LOG_MSG_INFO2_1 ("Got Pkt filter get config handle: %d", req_msg.handle);

  if (!rmnet_utils_packet_filter_get_rule(req_msg.handle, 
                                          rule))
  {
    errval = QMI_ERR_INVALID_HANDLE_V01;
  }
  else
  {
    resp_msg.rule.pattern_len = rule[0];
    resp_msg.rule.mask_len = rule[resp_msg.rule.pattern_len+1];
    resp_msg.rule_valid = TRUE;
    resp_msg.handle_valid = TRUE;
    resp_msg.handle = req_msg.handle;
    if (resp_msg.rule.pattern_len > QMI_WDA_PACKET_FILTER_SIZE_MAX_V01 || 
        resp_msg.rule.pattern_len != resp_msg.rule.mask_len)
    {
      errval = QMI_ERR_INTERNAL_V01;
    }
    else 
    {
      /* copy pattern*/
      memscpy(resp_msg.rule.pattern, sizeof(resp_msg.rule.pattern), 
              rule+1, resp_msg.rule.pattern_len);
      /*copy mask*/
      memscpy(resp_msg.rule.mask, sizeof(resp_msg.rule.mask), 
              rule + 1 + resp_msg.rule.pattern_len + 1, resp_msg.rule.mask_len);
    }
  }

  /*-------------------------------------------------------------------------
    build response
  -------------------------------------------------------------------------*/
send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_PACKET_FILTER_GET_RULE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_packet_filter_get_rule_resp_msg_v01),
                               errval,
                               &response);
  return response;
} /* qmi_wdai_packet_filter_get_rule() */

/*===========================================================================
  FUNCTION QMI_WDAI_SET_LOOPBACK_STATE()

  DESCRIPTION
    Handle the QMI WDA set loopback state message.

    Dispatches a request to the specified service to
    set loopback state.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_set_loopback_state
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *        response;
  qmi_error_type_v01       errval;
  int16                  ps_errno  = 0;
  uint16                 ps_result;
  wda_set_loopback_state_req_msg_v01 req_msg;
  wda_set_loopback_state_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  response          = NULL;
  errval            = QMI_ERR_NONE_V01;

  memset(&req_msg,0,sizeof(wda_set_loopback_state_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_set_loopback_state_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_SET_LOOPBACK_STATE_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_set_loopback_state_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }

  if( req_msg.loopback_state != 0 && req_msg.loopback_state != 1 )
  {
    errval = QMI_ERR_INVALID_ARG_V01;
    goto send_result;
  }

  ps_result = ps_sys_conf_set(PS_SYS_TECH_ALL, PS_SYS_CONF_ENABLE_LOOPBACK, 
                              (void *)&(req_msg.loopback_state), &ps_errno);

  if (ps_result)
  {
    LOG_MSG_ERROR_INT_1("PS sys config failed: d", ps_errno);
    errval = QMI_ERR_INTERNAL_V01;
  } 

send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_SET_LOOPBACK_STATE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_set_loopback_state_resp_msg_v01),
                               errval,
                               &response);
  return response;
} /* qmi_wdai_set_loopback_state() */

/*===========================================================================
  FUNCTION QMI_WDAI_GET_LOOPBACK_STATE()

  DESCRIPTION
    Handle the QMI WDA get loopback state message.

    Dispatches a request to the specified service to
    get current lookback state.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_get_loopback_state
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *                  response;
  qmi_error_type_v01               errval;
  int16                            ps_errno  = 0;
  uint16                           ps_result;
  wda_get_loopback_state_resp_msg_v01 resp_msg; 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  response = NULL;
  errval = QMI_ERR_NONE_V01;
  memset(&resp_msg,0,sizeof(wda_get_loopback_state_resp_msg_v01));

  ps_result = ps_sys_conf_get(PS_SYS_TECH_ALL, PS_SYS_CONF_ENABLE_LOOPBACK, 
                             &(resp_msg.loopback_state_is_enabled), &ps_errno);
  if (ps_result)
  {
    LOG_MSG_ERROR_INT_1("PS sys config failed:d", ps_errno);
    errval = QMI_ERR_INTERNAL_V01;
  }
  else 
  {
    resp_msg.loopback_state_is_enabled_valid = TRUE;
  }

  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_GET_LOOPBACK_STATE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_get_loopback_state_resp_msg_v01),
                               errval,
                               &response);
  return response;

} /* qmi_wdai_get_loopback_state() */

/*===========================================================================
  FUNCTION QMI_WDAI_SET_QMAP_SETTINGS()

  DESCRIPTION
    Handle the QMI WDA set qmap settings message.

    Dispatches a request to the specified service to
    set qmap settings.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_set_qmap_settings
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *        response;
  qmi_error_type_v01       errval;
  rmnet_qmap_settings_type     qmap_settings;
  uint32                       ep_id;
  wda_set_qmap_settings_req_msg_v01 req_msg;
  wda_set_qmap_settings_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&qmap_settings, 0, sizeof(qmap_settings));
  memset(&req_msg,0,sizeof(wda_set_qmap_settings_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_set_qmap_settings_resp_msg_v01));
  
  response          = NULL;
  errval            = QMI_ERR_NONE_V01;

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_SET_QMAP_SETTINGS_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_set_qmap_settings_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  /*-------------------------------------------------------------------------
    Check ep_id TLV
  -------------------------------------------------------------------------*/
  ep_id = qmux_get_default_ep_id_from_inst(
      ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance);

  if (req_msg.ep_id_valid)
  {
    // Converting to uint32
    if (QMI_EP_ID_VALID(req_msg.ep_id))
    {
      ep_id = QMI_EP_ID_TO_INTERNAL_EP_ID(req_msg.ep_id);
    }
    else
    {
      LOG_MSG_ERROR_2("Invalid ep_id 0x%x:0x%x",
                      req_msg.ep_id.ep_type, req_msg.ep_id.iface_id);
      errval = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
    }
  }

  if ( !rmnet_phys_xport_get_qmap_settings(ep_id, &qmap_settings) )
  {
    LOG_MSG_ERROR_1("Failed to get QMAP settings for EP 0x%x", ep_id);
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }

  if (req_msg.in_band_flow_control_valid)
  {
    qmap_settings.in_band_fc = (req_msg.in_band_flow_control)? TRUE : FALSE;
  }

  if ( !rmnet_phys_xport_set_qmap_settings(ep_id, &qmap_settings) )
  {
    LOG_MSG_ERROR_1("Failed to set QMAP settings for EP 0x%x", ep_id);
    errval = QMI_ERR_NOT_SUPPORTED_V01;
    goto send_result;
  }

  /*---------------------------------------------------------------------
    Send response
  ---------------------------------------------------------------------*/
  memset(&qmap_settings, 0, sizeof(qmap_settings));
  if ( !rmnet_phys_xport_get_qmap_settings(ep_id, &qmap_settings) )
  {
    LOG_MSG_ERROR_1("Failed to get QMAP settings for EP 0x%x", ep_id);
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  resp_msg.in_band_flow_control_valid = TRUE;
  resp_msg.in_band_flow_control = qmap_settings.in_band_fc ? 1 : 0;

send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_SET_QMAP_SETTINGS_RESP_V01,
                               &resp_msg,
                               sizeof(wda_set_qmap_settings_resp_msg_v01),
                               errval,
                               &response);
  return response;

} /* qmi_wdai_set_qmap_settings() */

/*===========================================================================
  FUNCTION QMI_WDAI_GET_QMAP_SETTINGS()

  DESCRIPTION
    Handle the QMI WDA get qmap settings message.

    Dispatches a request to the specified service to
    get current qmap settings.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type * qmi_wdai_get_qmap_settings
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *        response;
  qmi_error_type_v01       errval;
  uint32                           ep_id;
  rmnet_qmap_settings_type         qmap_settings;
  wda_get_qmap_settings_req_msg_v01 req_msg;
  wda_get_qmap_settings_resp_msg_v01 resp_msg;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  memset(&qmap_settings, 0, sizeof(qmap_settings));
  response = NULL;
  errval = QMI_ERR_NONE_V01;
  memset(&req_msg,0,sizeof(wda_get_qmap_settings_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_set_qmap_settings_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_GET_QMAP_SETTINGS_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_get_qmap_settings_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }
  /*-------------------------------------------------------------------------
    Check ep_id TLV
  -------------------------------------------------------------------------*/
  ep_id = qmux_get_default_ep_id_from_inst(
      ((qmi_framework_msg_hdr_type*)msg_hdr)->common_hdr.qmi_instance);

  if (req_msg.ep_id_valid)
  {
    // Converting to uint32
    if (QMI_EP_ID_VALID(req_msg.ep_id))
    {
      ep_id = QMI_EP_ID_TO_INTERNAL_EP_ID(req_msg.ep_id);
    }
    else
    {
      LOG_MSG_ERROR_2("Invalid ep_id 0x%x:0x%x",
                      req_msg.ep_id.ep_type, req_msg.ep_id.iface_id);
      errval = QMI_ERR_INVALID_ARG_V01;
      goto send_result;
    }
  }

  if ( !rmnet_phys_xport_get_qmap_settings(ep_id, &qmap_settings) )
  {
    LOG_MSG_ERROR_1("Failed to get QMAP settings for EP 0x%x", ep_id);
    errval = QMI_ERR_INTERNAL_V01;
    goto send_result;
  }
  resp_msg.in_band_flow_control_valid = TRUE;
  resp_msg.in_band_flow_control = qmap_settings.in_band_fc ? TRUE : FALSE;

send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_GET_QMAP_SETTINGS_RESP_V01,
                               &resp_msg,
                               sizeof(wda_get_qmap_settings_resp_msg_v01),
                               errval,
                               &response);
  return response;
} /* qmi_wdai_get_qmap_settings() */

/*===========================================================================
  FUNCTION QMI_WDAI_PARSE_DATA_FORMAT_REQ()

  DESCRIPTION
    Fills rmnet data format type from set_data_format request message

  PARAMETERS
    req_msg -- request message struct from set_data_format
    data-format -- rmnet structure to be filled

  RETURN VALUE
    uint32 - mask for valid parameters

  DEPENDENCIES
    none

  SIDE EFFECTS
    None
===========================================================================*/
static uint32 qmi_wdai_parse_data_format_req
(
  wda_set_data_format_req_msg_v01* req_msg,
  rmnet_data_format_type* data_format
)
{
  
  uint32 data_format_mask = 0;

  if(req_msg->qos_format_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_QOS;
    data_format->qos = req_msg->qos_format;
  }
  if(req_msg->link_prot_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_LINK_PROT;
    data_format->link_prot = (rmnet_sm_link_prot_e_type)req_msg->link_prot;
  }
  if(req_msg->ul_data_aggregation_protocol_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_UL_DATA_AGG;
    data_format->ul_data_agg_protocol = 
                (rmnet_data_agg_enum_type)req_msg->ul_data_aggregation_protocol;
  }
  if(req_msg->dl_data_aggregation_protocol_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_DL_DATA_AGG;
    data_format->dl_data_agg_protocol = 
                (rmnet_data_agg_enum_type)req_msg->dl_data_aggregation_protocol;
  }
  if(req_msg->ndp_signature_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_NDP_SIGNATURE;
    data_format->ndp_signature = req_msg->ndp_signature;
  }
  if(req_msg->dl_data_aggregation_max_datagrams_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_DL_DATA_AGG_NUM;
    data_format->dl_data_agg_max_num = req_msg->dl_data_aggregation_max_datagrams;
  }
  if(req_msg->dl_data_aggregation_max_size_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_DL_DATA_AGG_SIZE;
    data_format->dl_data_agg_max_size = req_msg->dl_data_aggregation_max_size;
  }
  if(req_msg->dl_minimum_padding_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_DL_MIN_PADDING;
    data_format->dl_min_padding = req_msg->dl_minimum_padding;
  }
  if(req_msg->flow_control_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_FLOW_CONTROL;
    /* Honor the flow_control TLV only if qos is not present or 0 */
    if (data_format->qos == FALSE)
    {
      data_format->te_flow_control = (req_msg->flow_control)? TRUE : FALSE;
    }
    else
    {
      LOG_MSG_INFO2_0("Ignoring flow_control TLV since qos_format is present and TRUE");
    }

  }

 #ifdef FEATURE_41207
  if(req_msg->ul_data_aggregation_max_datagrams_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_UL_DATA_AGG_NUM;
    data_format->ul_data_agg_max_num = req_msg->ul_data_aggregation_max_datagrams;
  }
  if(req_msg->ul_data_aggregation_max_size_valid)
  {
    data_format_mask |= RMNET_DATA_FORMAT_MASK_UL_DATA_AGG_SIZE;
    data_format->ul_data_agg_max_size = req_msg->ul_data_aggregation_max_size;
  }
 #endif /*FEATURE_41207 */
 
  return data_format_mask;
}/* qmi_wdai_parse_data_format_req */

/*===========================================================================
  FUNCTION QMI_WDAI_ENCODE_MSG()

  DESCRIPTION
    This function is a wrapper functions for qmi_svc_idl_message_encode
    assgins error and result codes to response and invlokes idl encode function.
 
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
boolean qmi_wdai_encode_msg
(
  uint16_t         message_id,
  void            *resp_msg,
  uint32_t         size,
  qmi_error_type_v01 err_code,
  dsm_item_type  **response
)
{
  qmi_result_e_type result;
  qmi_response_type_v01* res_ptr = NULL;
  
  result = (err_code == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                     : QMI_RESULT_FAILURE);
  res_ptr = (qmi_response_type_v01*)resp_msg;
  res_ptr->result = (qmi_result_type_v01)result;
  res_ptr->error = (qmi_error_type_v01)err_code;

  LOG_MSG_INFO1_3("qmi_wdai_encode_msg msg_id = %d err code = %d result code %d",
                                              message_id,err_code,result);
  if( !qmi_svc_idl_message_encode(qmi_wda_svc_obj,
      QMI_IDL_RESPONSE,message_id,resp_msg,size,response))
  {
    dsm_free_packet(response);
    response = NULL;
    LOG_MSG_ERROR_INT_1("Encoding failed for message id: d",message_id);
    if(!qmi_svc_put_result_tlv(response, 
                               (qmi_result_e_type)QMI_RESULT_FAILURE_V01,
                               (qmi_error_e_type)QMI_ERR_INTERNAL_V01))
    {
      dsm_free_packet(response);
      response = NULL;
      return FALSE;
    }
  }
  return TRUE;
}


/*===========================================================================
  FUNCTION QMI_WDAI_DECODE_MSG()

  DESCRIPTION
    This function is a wrapper functions for qmi_svc_idl_message_decode
    assgins error and result codes to response and invlokes idl encode function.
 
  PARAMETERS
    message_id : unique message ID.
    msg_ptr    : input buffer with all the TLV's.
    req_msg    : c struct to be filled
    req_size   : sizeof c structure

  RETURN VALUE
    qmi_error_type_v01

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_type_v01 qmi_wdai_decode_msg
(
  uint16_t        message_id,
  dsm_item_type **msg_ptr,
  void           *req_msg,
  uint32_t        req_size
)
{
  if(req_msg != NULL)
  {
    return qmi_svc_idl_message_decode(qmi_wda_svc_obj,
               message_id,msg_ptr,req_msg,req_size);
  }
  else
  {
    return QMI_ERR_INTERNAL_V01;
  }
}

void qmi_wda_reg_powersave_mode
(
  qmi_wda_powersave_config_mask_e_type  conf_mask,
  qmi_wda_powersave_mode_cb_type        callback
)
{
  qmi_wda_powersave_client_info_type * client_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(qmi_wda_powersave_service_info.num_clients >= WDA_MAX_POWERSAVE_MODE_CLIENTS)
  {
    LOG_MSG_ERROR_2("qmi_wda_reg_powersave_mode: Failed to register for "
                    "powersave mode, mask %d, callback 0x%p",
                     conf_mask, callback);
    return;
  }

  client_ptr = &qmi_wda_powersave_service_info.client_info_arr[qmi_wda_powersave_service_info.num_clients];
  client_ptr->conf_mask = conf_mask;
  client_ptr->callback = callback;
  qmi_wda_powersave_service_info.supported_mask |= client_ptr->conf_mask;
  qmi_wda_powersave_service_info.num_clients++;
} /* qmi_wda_reg_powersave_mode */

/*===========================================================================
  FUNCTION QMI_WDAI_SET_POWERSAVE_CONFIG()

  DESCRIPTION
    This function registers powersave mode with supported mask by clients.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_wdai_set_powersave_config
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type *        response = NULL;

  qmi_error_type_v01       errval = QMI_ERR_NONE_V01;
  wda_set_powersave_config_req_msg_v01 req_msg;
  wda_set_powersave_config_resp_msg_v01 resp_msg;
#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS*/
  qmi_wdai_client_state_type                * clnt_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&req_msg,0,sizeof(wda_set_powersave_config_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_set_powersave_config_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_SET_POWERSAVE_CONFIG_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_set_powersave_config_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }

  clnt_sp = (qmi_wdai_client_state_type *)cl_sp;
  clnt_sp->ep_id = QMI_EP_ID_TO_INTERNAL_EP_ID(req_msg.ep_id);

  /*-------------------------------------------------------------------------
    Check if the mask is supported from cached mask
  ------------------------------------------------------------------------*/
  if (req_msg.req_data_cfg_valid)
  {
    clnt_sp->requested_mask =
      (qmi_wda_powersave_service_info.supported_mask & req_msg.req_data_cfg);
  }
  else
  {
    errval = QMI_ERR_MISSING_ARG_V01;
    goto send_result;
  }

#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS*/

  /*---------------------------------------------------------------------
    Send response
  ---------------------------------------------------------------------*/
  resp_msg.data_cfg_valid = TRUE;
  resp_msg.data_cfg = clnt_sp->requested_mask;

send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_SET_POWERSAVE_CONFIG_RESP_V01,
                               &resp_msg,
                               sizeof(wda_set_powersave_config_resp_msg_v01),
                               errval,
                               &response);

  return response;
} /* qmi_wdai_set_powersave_config */

/*===========================================================================
  FUNCTION QMI_WDAI_SET_POWERSAVE_MODE()

  DESCRIPTION
    This function notify registered clients of the status of powersave mode.

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    dsm * pointing to the response to be sent to host
    NULL if no response was generated or command was discarded

  DEPENDENCIES
    qmi_wda_init() must have been called

  SIDE EFFECTS
    None
===========================================================================*/
static dsm_item_type* qmi_wdai_set_powersave_mode
(
  void *            sp,
  void *            msg_hdr,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  dsm_item_type                            * response = NULL;

  qmi_error_type_v01                         errval = QMI_ERR_NONE_V01;
  wda_set_powersave_mode_req_msg_v01         req_msg;
  wda_set_powersave_mode_resp_msg_v01        resp_msg;
  qmi_wda_powersave_client_info_type       * clnt_info_ptr = NULL;
  uint8                                      clnt_idx = 0;
  uint32                                     mask = WDS_POWERSAVE_DISABLE_ALL_MASK;
#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS*/
  qmi_wdai_client_state_type               * clnt_sp;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&req_msg,0,sizeof(wda_set_powersave_mode_req_msg_v01));
  memset(&resp_msg,0,sizeof(wda_set_powersave_mode_resp_msg_v01));

  errval = QMI_WDAI_DECODE_REQUEST_MSG(QMI_WDA_SET_POWERSAVE_MODE_REQ_V01,
                                       sdu_in,
                                       &req_msg,
                                       sizeof(wda_set_powersave_mode_req_msg_v01));
  if(errval != QMI_ERR_NONE_V01)
  {
    goto send_result;
  }

  /*-------------------------------------------------------------------------
    Check mask and notify if powersave mode is enabled/disabled for all
    registered clients
  ------------------------------------------------------------------------*/
  clnt_sp = (qmi_wdai_client_state_type *)cl_sp;
  if(1 == req_msg.powersave_control_flag)
  {
    /*---------------------------------------------------------------------
      Return error if trying to set powersave mode before configured
    ---------------------------------------------------------------------*/
    if(WDS_POWERSAVE_DISABLE_ALL_MASK == clnt_sp->requested_mask)
    {
      errval = QMI_ERR_INVALID_OPERATION_V01;
      goto send_result;
    }
    mask = clnt_sp->requested_mask;
  }
  else
  {
    mask = WDS_POWERSAVE_DISABLE_ALL_MASK;
  }

  for(clnt_idx = 0; clnt_idx < qmi_wda_powersave_service_info.num_clients; ++clnt_idx)
  {
    clnt_info_ptr = &qmi_wda_powersave_service_info.client_info_arr[clnt_idx];
    if(0 != (clnt_info_ptr->conf_mask & clnt_sp->requested_mask))
    {
      clnt_info_ptr->callback(clnt_sp->ep_id, mask);
    }
  }

#ifdef FEATURE_LAPP_UFS
  #error code not present
#endif /* FEATURE_LAPP_UFS*/

  /*---------------------------------------------------------------------
    Send response
  ---------------------------------------------------------------------*/
send_result:
  QMI_WDAI_ENCODE_RESPONSE_MSG(QMI_WDA_SET_POWERSAVE_MODE_RESP_V01,
                               &resp_msg,
                               sizeof(wda_set_powersave_mode_resp_msg_v01),
                               errval,
                               &response);

  return response;
} /* qmi_wdai_set_powersave_mode */

/*===========================================================================
  FUNCTION QMI_WDAI_POWERSAVE_DL_MARKER_CB()

  DESCRIPTION
    This the callback that we have registered for powersave mode with
    DL marker mask in lieu of rmnet shim

  PARAMETERS
    ep_id:   Not used
    mask:    Mask for powersave mode

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_wdai_powersave_dl_marker_cb
(
  uint32  ep_id,
  uint32  mask
)
{
  boolean is_enabled = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*---------------------------------------------------------------------
    Enable DL marker if powersave mode is disabled and vice versa
  ---------------------------------------------------------------------*/
  if(0 == (mask & QMI_WDA_POWERSAVE_CONFIG_DL_MARKER))
  {
    is_enabled = TRUE;
  }
  else
  {
    is_enabled = FALSE;
  }

  rmnet_shim_ipa_set_dl_burst_marker(is_enabled);
} /* qmi_wdai_powersave_dl_marker_cb */

