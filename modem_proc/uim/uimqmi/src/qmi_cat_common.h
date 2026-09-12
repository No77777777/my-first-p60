/*================================================================================

DESCRIPTION
QMI CAT common header file


Copyright (c) 2015-2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_cat_common.h#2 $$ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
09/06/16   vr      Cache the GET_INPUT command
03/03/16   tkl     Add indication support for TR and Envelope cmd
02/05/16   vr      Validate event confirmation
10/27/15   vr      Multi-profile switch handling
09/08/15   vr      Initial version

=============================================================================*/

#ifndef QMI_CAT_COMMON_H
#define QMI_CAT_COMMON_H


/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "gstk_exp.h"
#include "mmgsdilib_common.h"

#include "qmi_framework.h"
#include "qmi_svc_defs.h"
#include "ds_qmi_fw_common.h"

#include "qmi_uim_cat_common.h"

#include "card_application_toolkit_v02.h"

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  Macro used in command handlers (common)
---------------------------------------------------------------------------*/
#define CHECK_RETVAL()  if (FALSE == retval) { dsm_free_packet(&response); \
                                               return NULL; }

/*---------------------------------------------------------------------------
  Macro used to free pointers
---------------------------------------------------------------------------*/
#define QMI_CAT_FREE_IF_NOT_NULL(ptr)                       \
      if (ptr)                                              \
      {                                                     \
        uimqmi_free(ptr);                                   \
        ptr = NULL;                                         \
      }

/*---------------------------------------------------------------------------
  Number of cards
---------------------------------------------------------------------------*/
#define QMI_CAT_MAX_CARD_COUNT                    (3)

/*---------------------------------------------------------------------------
  Max number of SCWS Channel
---------------------------------------------------------------------------*/
#define QMI_CATI_MAX_SCWS_CHANNEL                 (7)

/*---------------------------------------------------------------------------
  Max number of BIP Channel
---------------------------------------------------------------------------*/
#define QMI_CATI_MAX_BIP_CHANNEL                  (7)

/*---------------------------------------------------------------------------
  Values used for cache (decoded)
---------------------------------------------------------------------------*/
#define CATI_CACHED_EVT_DECODED_SETUP_MENU                   (0)
#define CATI_CACHED_EVT_DECODED_SETUP_IDLE_MODE_TEXT         (1)
#define CATI_CACHED_EVT_DECODED_SETUP_EVT_LIST_IDLE_SCREEN   (2)
#define CATI_CACHED_EVT_DECODED_SETUP_EVT_LIST_USER_ACT      (3)
#define CATI_CACHED_EVT_DECODED_SETUP_EVT_LIST_LANG_SEL      (4)
#define CATI_CACHED_EVT_DECODED_SETUP_EVT_LIST_HCI_CONN      (5)
#define CATI_CACHED_EVT_DECODED_SETUP_EVT_LIST_BROWSER_TERM  (6)
#define CATI_CACHED_EVT_DECODED_SETUP_EVT_CONTACTLESS_S_REQ  (7)
#define CATI_CACHED_EVT_DECODED_DISPLAY_TEXT                 (8)
#define CATI_CACHED_EVT_DECODED_SCWS_OPEN_CHANNEL            (9)
#define CATI_CACHED_EVT_DECODED_PROVIDE_LOCAL_INFO_LANG     (10)
#define CATI_CACHED_EVT_DECODED_PROVIDE_LOCAL_INFO_BATTERY  (11)
#define CATI_CACHED_EVT_DECODED_GET_INPUT                   (12)
#define CATI_MAX_DECODED_CACHED_EVTS                        (13)

/*---------------------------------------------------------------------------
  Values used for cache (raw)
---------------------------------------------------------------------------*/
#define CATI_CACHED_EVT_SETUP_MENU                          (0)
#define CATI_CACHED_EVT_SETUP_IDLE_MODE_TEXT                (1)
#define CATI_CACHED_EVT_SETUP_EVT_LIST                      (2)
#define CATI_CACHED_EVT_DISPLAY_TEXT                        (3)
#define CATI_CACHED_EVT_PROVIDE_LOCAL_INFO                  (4)
#define CATI_CACHED_EVT_GET_INPUT                           (5)
#define CATI_MAX_RAW_CACHED_EVTS                            (6)

/*---------------------------------------------------------------------------
  Values used for recovery cache (raw)
---------------------------------------------------------------------------*/
#define CATI_QMI_CLIENT_CACHED_EVT_SETUP_MENU                (0)
#define CATI_QMI_CLIENT_CACHED_EVT_SETUP_IDLE_MODE_TEXT      (1)
#define CATI_QMI_CLIENT_CACHED_EVT_SETUP_EVT_LIST            (2)
#define CATI_MAX_RAW_QMI_CLIENT_CACHED_EVTS                  (3)


/*===========================================================================

                                DATA TYPES

===========================================================================*/
/*---------------------------------------------------------------------------
  QMI CAT dispaly alpha confirmation type
---------------------------------------------------------------------------*/
typedef enum
{
  CATI_DISPLAY_ALPHA_NO_AUTOMATIC_CONF                   = 0x00,
  CATI_DISPLAY_ALPHA_AUTOMATIC_FALSE                     = 0x01,
  CATI_DISPLAY_ALPHA_AUTOMATIC_TRUE                      = 0x02
} qmi_cati_display_alpha_conf_e_type;

/*---------------------------------------------------------------------------
  QMI CAT message type
---------------------------------------------------------------------------*/
typedef enum
{
  CATI_REQ_MSG_TYPE                                      = 0x00,
  CATI_RESP_MSG_TYPE                                     = 0x01,
  CATI_IND_MSG_TYPE                                      = 0x02
} qmi_cati_message_e_type;

/*---------------------------------------------------------------------------
  Type of QMI CAT messages posted to DCC task
---------------------------------------------------------------------------*/
typedef enum
{
  QMI_CAT_MESSAGE_RAW_PROACTIVE_COMMAND,
  QMI_CAT_MESSAGE_DECODED_PROACTIVE_COMMAND,
  QMI_CAT_MESSAGE_DECODED_BIP_STATUS,
  QMI_CAT_MESSAGE_SCWS_COMMAND,
  QMI_CAT_MESSAGE_ENVELOPE_RESPONSE,
  QMI_CAT_MESSAGE_DECODED_ENVELOPE_RESPONSE,
  QMI_CAT_MESSAGE_END_PROACTIVE_SESSION,
  QMI_CAT_MESSAGE_TERM_RSP_SW1_SW2,
  QMI_CAT_MESSAGE_TERM_RSP_INDICATION,
  QMI_CAT_MESSAGE_ALLOC_CLID,
  QMI_CAT_MESSAGE_DEALLOC_CLID,
  QMI_CAT_MESSAGE_FRAMEWORK_CMD,
  QMI_CAT_MESSAGE_MMGSDI_CNF,
  QMI_CAT_MESSAGE_MMGSDI_EVT,
  QMI_CAT_MESSAGE_RECOVERY_COMPLETE,
  QMI_CAT_MESSAGE_TP_DL_IND
} qmi_cat_message_enum_type;

typedef enum
{
  CATI_CMD_VAL_RESET                     = QMI_CAT_RESET_REQ_V02,
  CATI_CMD_VAL_SET_EVENT_REPORT          = QMI_CAT_SET_EVENT_REPORT_REQ_V02,
  CATI_CMD_VAL_GET_SERVICE_STATE         = QMI_CAT_GET_SERVICE_STATE_REQ_V02,
  CATI_CMD_VAL_SEND_TR                   = QMI_CAT_SEND_TR_REQ_V02,
  CATI_CMD_VAL_SEND_ENVELOPE_CMD         = QMI_CAT_SEND_ENVELOPE_CMD_REQ_V02,
  CATI_CMD_VAL_GET_EVENT_REPORT          = QMI_CAT_GET_EVENT_REPORT_REQ_V02,
  CATI_CMD_VAL_SEND_DECODED_TR           = QMI_CAT_SEND_DECODED_TR_REQ_V02,
  CATI_CMD_VAL_DECODED_ENVELOPE_CMD      = QMI_CAT_SEND_DECODED_ENVELOPE_CMD_REQ_V02,
  CATI_CMD_VAL_EVENT_CONFIRMATION_CMD    = QMI_CAT_EVENT_CONFIRMATION_REQ_V02,
  CATI_CMD_VAL_SCWS_OPEN_CH              = QMI_CAT_SCWS_OPEN_CHANNEL_REQ_V02,
  CATI_CMD_VAL_SCWS_CLOSE_CH             = QMI_CAT_SCWS_CLOSE_CHANNEL_REQ_V02,
  CATI_CMD_VAL_SCWS_SEND_DATA            = QMI_CAT_SCWS_SEND_DATA_REQ_V02,
  CATI_CMD_VAL_SCWS_DATA_AVAILABLE       = QMI_CAT_SCWS_DATA_AVAILABLE_REQ_V02,
  CATI_CMD_VAL_SCWS_CHANNEL_STATUS       = QMI_CAT_SCWS_CHANNEL_STATUS_REQ_V02,
  CATI_CMD_VAL_GET_TERMINAL_PROFILE      = QMI_CAT_GET_TERMINAL_PROFILE_REQ_V02,
  CATI_CMD_VAL_SET_CONFIGURATION         = QMI_CAT_SET_CONFIGURATION_REQ_V02,
  CATI_CMD_VAL_GET_CONFIGURATION         = QMI_CAT_GET_CONFIGURATION_REQ_V02,
  CATI_CMD_VAL_GET_CACHED_PROACTIVE_CMD  = QMI_CAT_GET_CACHED_PROACTIVE_CMD_REQ_V02,
  /* QC EXTERNAL QMI COMMAND RANGE IS 0x0000 - 0x5555.
     Add the next external QMI Command here */

  /* VENDOR SPECIFIC QMI COMMAND RANGE IS 0x5556 - 0xAAAA.
     IMPORTANT!
     Add the vendor specific QMI Commands within this range only to avoid
     conflicts with QC QMI commands that would get released in future */

  /* RESERVED QC QMI COMMAND RANGE IS 0xAAAB - 0xFFFE
     Internal QMI Commands must be added in DECREASING ORDER from Below */
  CATI_CMD_VAL_MAX,
  CATI_CMD_VAL_WIDTH                     = 0xFFFF
} qmi_cati_cmd_val_e_type;

/*---------------------------------------------------------------------------
  QMI CAT card states
---------------------------------------------------------------------------*/
typedef enum
{
  /* when card is not present in the device or any card error */
  QMI_CAT_CARD_STATE_ERROR,
  /* card state between tp_dl and card insert event */
  QMI_CAT_CARD_STATE_INIT_IN_PROGRESS,
  /* card state after card insert event */
  QMI_CAT_CARD_STATE_PRESENT,
  /* card state during profile switch */
  QMI_CAT_CARD_STATE_INVALID
} qmi_cati_card_state_e_type;

/*---------------------------------------------------------------------------
  Structure holding cached proactive command (raw format)
---------------------------------------------------------------------------*/
typedef struct
{
  union
  {
    /* raw evt (setup menu or setup evt list) */
    struct
    {
      uint32    cmd_ref_id;
      uint16    raw_data_len;
      uint8     raw_data[QMI_CAT_RAW_PROACTIVE_CMD_MAX_LENGTH_V02];
    } cati_raw_pc_evt;

    /* setup evt list */
    struct
    {
      uint32     pc_setup_evt_list;
    } cati_setup_evt_list_cmd_evt;
  } cati_evt_rpt;
} qmi_cati_cached_evt_info_type;

/*---------------------------------------------------------------------------
  Structures holding cached proactive command
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_cati_cached_evt_info_type     cached_event;
} qmi_cati_raw_cache_type;

typedef struct
{
  gstk_cmd_from_card_type           cached_event;
} qmi_cati_decoded_cache_type;

/*---------------------------------------------------------------------------
  Token information extracted from a command
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                    is_valid;
  uint32                     token;
} qmi_cati_ind_token_info_type;

/*---------------------------------------------------------------------------
  Structure holding userdata required during GSTK response
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_cati_cmd_val_e_type               request_id;
  uint8                                 clid;
  uint32                                client_reg_count;
  qmi_cati_ind_token_info_type          ind_token;
  void *                                cmd_buf_ptr;
} qmi_cati_userdata_type;

/*---------------------------------------------------------------------------
  Structure holding dynamically allocated cache
---------------------------------------------------------------------------*/
struct qmi_cati_cache_node
{
  struct qmi_cati_cache_node      * next_ptr;
  qmi_cati_userdata_type          * cb_userdata_ptr;
};

typedef struct qmi_cati_cache_node qmi_cati_cmd_buf_cache_type;

/*---------------------------------------------------------------------------
  Structure for QMI CAT messages
---------------------------------------------------------------------------*/
typedef struct
{
  /* This needs to be in first position */
  q_link_type                               link_next;

  qmi_cat_message_enum_type                 message_type;

  union
  {
    struct
    {
      gstk_slot_id_enum_type                slot_id;
      uint32                                cmd_ref_id;
      uint8                                 raw_hdr_tag;
      int32                                 raw_data_len;
      uint8                                 raw_data_buffer[ QMI_CAT_RAW_PROACTIVE_CMD_MAX_LENGTH_V02 ];
      boolean                               is_cached_cmd;
    }                                       raw_proactive_cmd;
    struct
    {
      gstk_cmd_from_card_type               decoded_cmd;
      boolean                               is_cached_cmd;
    }                                       decoded_proactive_cmd;
    struct
    {
      gstk_slot_id_enum_type                slot_id;
      uint8                                 ch_id;
      cat_bip_status_enum_v02               bip_status;
    }                                       decoded_bip_status;
    struct
    {
      gstk_cmd_from_card_type               scws_cmd;
    }                                       scws_cmd;
    struct
    {
      boolean                               envelope_data_not_present;
      gstk_cmd_from_card_type               envelope_rsp;
      void *                                user_data;
    }                                       envelope_rsp;
    struct
    {
      gstk_cmd_from_card_type               envelope_rsp;
    }                                       decoded_envelope_rsp;
    struct
    {
      gstk_slot_id_enum_type                    slot_id;
      cat_proactive_session_end_type_enum_v02   proactive_session_end_type;
      gstk_tag_enum_type                        tag;
    }                                           end_proactive_cmd;
    struct
    {
      gstk_cmd_from_card_type               sw1_sw2_data;
    }                                       terminal_response_sw1_sw2_info;
    struct
    {
      qmi_framework_common_msg_hdr_type     cmd_hdr;
    }                                       clid_cb;
    struct
    {
      uimqmi_framework_cmd_type             cmd;
    }                                       framework_cmd;
    struct
    {
      mmgsdi_cnf_enum_type                  cnf_type;
      mmgsdi_cnf_type                       cnf_value;
    }                                       mmgsdi_cnf;
    struct
    {
      mmgsdi_events_enum_type               evt;
      mmgsdi_slot_id_enum_type              slot;
    }                                       mmgsdi_evt;
    struct
    {
      mmgsdi_slot_id_enum_type              slot;
    }                                       recovery_ind;
    struct
    {
      gstk_slot_id_enum_type                slot_id;
    }                                       tp_dl_ind;
    struct
    {
      gstk_cmd_from_card_type               terminal_response_rsp;
    }                                       terminal_response_rsp;
  }                                         data;
} qmi_cat_message_type;

/*---------------------------------------------------------------------------
  QMI_CAT Global state information - initialized in qmi_cat_init()
  Global to all QMI instances
---------------------------------------------------------------------------*/
typedef struct
{
  /* Message queue */
  q_type                                     qmi_cat_q;
  /* Client ids */
  gstk_client_id_type                        cat_gstk_client_id[QMI_CAT_MAX_CARD_COUNT];
  gstk_client_id_type                        cat_gstk_client_id_external[QMI_CAT_MAX_CARD_COUNT];
  /* Number of slots device supports */
  uint8                                      slot_count;
  /* Global masks of registered events */
  uint32                                     evt_reg_mask[QMI_CAT_MAX_CARD_COUNT];
  uint32                                     decoded_evt_reg_mask[QMI_CAT_MAX_CARD_COUNT];
  uint32                                     full_func_evt_reg_mask[QMI_CAT_MAX_CARD_COUNT];
  /* Flag to set if the event received is registered as a full functionality for the ongoing proactive command */
  boolean                                    full_func_evt[QMI_CAT_MAX_CARD_COUNT];
  /* Cache of proactive commands */
  qmi_cati_raw_cache_type                  * raw_cache[QMI_CAT_MAX_CARD_COUNT][CATI_MAX_RAW_CACHED_EVTS];
  qmi_cati_raw_cache_type                  * recovery_raw_cache[QMI_CAT_MAX_CARD_COUNT][CATI_MAX_RAW_QMI_CLIENT_CACHED_EVTS];
  qmi_cati_decoded_cache_type              * decoded_cache[QMI_CAT_MAX_CARD_COUNT][CATI_MAX_DECODED_CACHED_EVTS];
  gstk_cmd_enum_type                         last_gstk_cmd[QMI_CAT_MAX_CARD_COUNT];
  mmgsdi_client_id_type                      cat_mmgsdi_client_id;
  /* Configuration mode */
  cat_config_mode_enum_v02                	 config_mode;
  /* Flag to indication return value for display alpha confirmation */
  qmi_cati_display_alpha_conf_e_type         display_alpha_conf;
  /* Flag to control use of sms_pp_envelope commands */
  boolean                                    sms_pp_envelope_blocked;
  /* Reference command id for event confirmation */
  uint32                                     ref_command_id[QMI_CAT_MAX_CARD_COUNT];
  /* BIP channel status */
  cat_bip_status_enum_v02                    bip_status_cache[QMI_CAT_MAX_CARD_COUNT][QMI_CATI_MAX_BIP_CHANNEL];
  /* Track numbers of SETUP_EVENT_XX_NOTIFY that is waiting for TR */
  uint8                                      decoded_setup_evt_in_progress[QMI_CAT_MAX_CARD_COUNT];
  /* command buffer of pending scws requests*/
  qmi_cati_cmd_buf_cache_type              * scws_cmd_buf[QMI_CAT_MAX_CARD_COUNT][QMI_CATI_MAX_SCWS_CHANNEL];
  /* card state */
  qmi_cati_card_state_e_type                 card_state[QMI_CAT_MAX_CARD_COUNT];
  /* Flag to check event confirmation is for user confirmation or not */
  boolean                                    user_cnf_required[QMI_CAT_MAX_CARD_COUNT];
}qmi_cati_global_type;

/*---------------------------------------------------------------------------
  QMI Card Application Toolkit Service (CAT) instance state definition &
  CAT client state definition
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_common_client_state_type   common; /* must be first since we alias to 
                                            uimqmi_common_client_state_type */
  int16                          instance; /* must be second since we alias to
                                              uimqmi_common_client_state_type */
  int16                          service_id; /* must be third since we alias to
                                                uimqmi_common_client_state_type */
  uint32                         evt_report_mask[QMI_CAT_MAX_CARD_COUNT];
  uint32                         decoded_evt_report_mask[QMI_CAT_MAX_CARD_COUNT];
} qmi_cati_client_state_type;

/*---------------------------------------------------------------------------
  Watermark queue to hold deferred responses
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_common_svc_state_type    common; /* must be first since we alias to 
                                          uimqmi_common_client_state_type */
  qmi_cati_client_state_type * client[UIMQMI_SVC_MAX_CLIDS];
  uint32                       client_reg_count[UIMQMI_SVC_MAX_CLIDS];
} qmi_cati_state_type;


/*===========================================================================

                               GLOBAL DATA

===========================================================================*/
extern qmi_cati_global_type                  *qmi_cati_global_ptr;
extern qmi_cati_state_type                   *qmi_cat_state_ptr;


/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
  FUNCTION QMI_CAT_GET_CB_USERDATA

  DESCRIPTION
    To obtain the userdata used in GSTK callbacks

  PARAMETERS
    request_id             : Type of the request
    cmd_buf_ptr            : QMI command buffer required for response
    ind_token              : indication token

  RETURN VALUE
    qmi_cati_userdata_type : Poiner to the structure used for userdata

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_cati_userdata_type* qmi_cat_get_cb_userdata
(
  qmi_cati_cmd_val_e_type              request_id,
  qmi_cmd_buf_type *                   cmd_buf_ptr,
  qmi_cati_ind_token_info_type         ind_token
);

/*===========================================================================
  FUNCTION QMI_CAT_FREE_CB_USERDATA

  DESCRIPTION
    To free the structure used for MMGSDI callbacks

  PARAMETERS
    request_id             : Type of the request
    cmd_buf_ptr            : QMI command buffer required for response

  RETURN VALUE
    qmi_uimi_userdata_type : Poiner to the structure used for userdata

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_cat_free_cb_userdata
(
  qmi_cati_userdata_type*  cb_userdata_ptr
);

/*===========================================================================
  FUNCTION QMI_CAT_CHECK_CLID()

  DESCRIPTION
    To validate if the passed client id is stil valid within the passed CAT
    service state ptr

  PARAMETERS
    clid             : Client ID
    client_reg_count : Client's count when the command was requested

  RETURN VALUE
    TRUE if successful, else FALSE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_cat_check_clid
(
  uint8                         clid,
  uint32                        client_reg_count
);

/*===========================================================================
  FUNCTION QMI_CAT_CREATE_MESSAGE

  DESCRIPTION
    Allocates the memory for a QMI CAT message and initializes it.

  PARAMETERS
    message_type  : type of QMI CAT message

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_cat_message_type * qmi_cat_create_message
(
  qmi_cat_message_enum_type message_type
);

/*===========================================================================
  FUNCTION QMI_CAT_POST_MESSAGE

  DESCRIPTION
    Adds a message to the QMI CAT queue and posts an event to DCC task,
    so that it can be processed later in the right context.

  PARAMETERS
    cat_message_ptr  : QMI CAT message

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_cat_post_message
(
  qmi_cat_message_type * cat_message_ptr
);

/*===========================================================================
  FUNCTION QMI_CATI_MAP_GSTK_STATUS_TO_ERRVAL

  DESCRIPTION
    Map gstk status to QMI-Error value

  PARAMETERS
    gstk_status_enum_type: gstk_status

  RETURN VALUE
    qmi_error_e_type: QMI Error type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cati_map_gstk_status_to_errval
(
  gstk_status_enum_type  gstk_status
);

/*===========================================================================
  FUNCTION QMI_CATI_CONVERT_GSTK_SLOT_ID_TO_QMI_SLOT_INDEX

  DESCRIPTION
    to get qmi slot index from gstk slot id

  PARAMETERS
    gstk_slot_id
    qmi_slot_index

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_cati_convert_gstk_slot_id_to_qmi_slot_index
(
  gstk_slot_id_enum_type    gstk_slot_id,
  uint8                   * slot_index_ptr
);

#endif /* QMI_CAT_COMMON_H */
