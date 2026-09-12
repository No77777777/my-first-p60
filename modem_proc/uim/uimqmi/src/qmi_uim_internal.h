#ifndef QMI_UIM_INTERNAL_H
#define QMI_UIM_INTERNAL_H
/*===========================================================================

                         Q M I _ U I M _ I N T E R N A L. H

DESCRIPTION

 The QMI User Identity Module internal header file.

                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2017, 2019 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_internal.h#5 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
08/17/20    vgd    Added logic to get EID using both M2M and LPA methods
06/08/20    rkg    Revert defensive check added in subscription manager code
09/17/19    vdc    SimLock solution for activation at Point Of Sale
09/17/19    mm     Add EID to SimSlotStatus radio config HAL
12/06/17    srm    Send IND if other slot detection has started late
07/13/17    av     Enhanced simlock RSU solution
04/21/17    sk     Map provisioning sessions to slots
04/12/17    ar     Add support for retrieve/configure default server address
04/06/17    gm     Added support for extended APDU
02/21/17    bcho   Set perso state to IN PROGRESS if any app gets stuck at PIN1
02/16/17    ar     Support for multiple add_profile request from same client
01/19/17    me     Populating path internally and parsing IMSI, FPLMN, EHPLMN
12/07/16    av     LPA phase2 initial changes
12/05/16    vdc    Added support for simlock single lock for full srv policy
10/31/16    me     Added a recovery in progress flag inside qmi_uim_global_type
09/22/16    gm     Added support for set APDU behavior
08/19/16    tkl    Added support for simlock single lock
08/18/16    ar     Add support for simlock storage and emergency only mode
07/19/16    bcho   Move refresh_info to heap in qmi_uim_global_type
07/15/16    sp     Seperate ERA-GLONASS and SAP related QMI UIM functionality
07/14/16    ar     Check Blacklist based on AID source
06/28/16    vdc    Remove callback user data from qmi uim global
06/16/16    av     Support for setting terminal capability from AP
05/26/16    vdc    Initial check-in of QMI UIM internal header file
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_p.h"
#include "simlock_modem_p.h"
#include "gstk_exp_v.h"
#include "gba_lib.h"
#include "lpalib.h"
#include "mmgsdilib_common.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_cat_common.h"
#include "user_identity_module_v01.h"
#include "user_identity_module_impl_v01.h"
#include "ds_qmi_fw_common.h"

/*===========================================================================

                            CONSTANT DEFINITIONS

===========================================================================*/

/*---------------------------------------------------------------------------
  Value used for EFS directory list
---------------------------------------------------------------------------*/
#define QMI_UIM_EFS_DIRECTORY_LIST_SIZE       (4)

/*---------------------------------------------------------------------------
  Defines to be used for non provisioning applications
---------------------------------------------------------------------------*/
#define UIM_MAX_NON_PROV_SESSIONS     (QMI_UIM_MAX_CARD_COUNT * 3)
#define UIM_MAX_NON_PROV_CLIENTS      (UIMQMI_SVC_MAX_CLIDS)

/*---------------------------------------------------------------------------
  Defines to be used for refresh
---------------------------------------------------------------------------*/
#define UIM_REFRESH_MAX_CLIENTS       (UIMQMI_SVC_MAX_CLIDS)
#define UIM_REFRESH_PROV_COUNT        (9)
#define UIM_REFRESH_NON_PROV_COUNT    (6)
#define UIM_REFRESH_SESSION_COUNT     (UIM_REFRESH_PROV_COUNT + UIM_REFRESH_NON_PROV_COUNT)

/*---------------------------------------------------------------------------
  Defines to be used for channels to stream APDUs
---------------------------------------------------------------------------*/
#define UIM_MAX_APDU_CHANNEL_COUNT (QMI_UIM_MAX_CARD_COUNT * 20)

/*---------------------------------------------------------------------------
  Length of IMSI digits data
---------------------------------------------------------------------------*/
#define QMI_UIM_IMSI_DIGIT_LEN                 (15)

/*---------------------------------------------------------------------------
  MNC length for 2, 3 digits
---------------------------------------------------------------------------*/
#define QMI_UIM_MNC_2_DIGITS             0x02
#define QMI_UIM_MNC_3_DIGITS             0x03

/*---------------------------------------------------------------------------
  IMSI Low, High byte
---------------------------------------------------------------------------*/
#define QMI_UIM_IMSI_LOW_BYTE_MASK        0x0F
#define QMI_UIM_IMSI_HIGH_BYTE_MASK       0xF0

/*---------------------------------------------------------------------------
  MNC offset in ef AD is 3 which is the 4th byte
---------------------------------------------------------------------------*/
#define QMI_UIM_MNC_LEN_OFFSET_IN_AD      3

/*---------------------------------------------------------------------------
  Defines to be used for MMGSDI
---------------------------------------------------------------------------*/
#define UIM_INVALID_CLIENT_ID                          (0)
#define UIM_INVALID_SESSION_ID                         (0)
#define UIM_FAKE_SESSION_ID                            (0xFFFFFFFF)
#define UIM_INVALID_APP_INDEX                          (0xFFFF)

/*---------------------------------------------------------------------------
  Values used in APDU check
---------------------------------------------------------------------------*/
#define QMI_UIM_APDU_INS_BYTE_MANAGE_CHANNEL            0x70
#define QMI_UIM_APDU_INS_BYTE_SELECT_FILE               0xA4
#define QMI_UIM_APDU_P1_BYTE_SELECT_DF_NAME             0x04
#define QMI_UIM_APDU_INS_BYTE_AUTHENTICATE              0x88
#define QMI_UIM_APDU_P2_BYTE_USIM_AUTHENTICATE_GSM_ALGO 0x80
#define QMI_UIM_APDU_CLA_BYTE_ICC                       0xA0

/*---------------------------------------------------------------------------
  Macro used in command handlers (common)
---------------------------------------------------------------------------*/
#define CHECK_RETVAL()  if (FALSE == retval) { dsm_free_packet(&response); \
                                               return NULL; }

/*===========================================================================

                                DATA TYPES

===========================================================================*/

/*---------------------------------------------------------------------------
  Type of QMI UIM messages posted to DCC task
---------------------------------------------------------------------------*/
typedef enum
{
  QMI_UIM_MESSAGE_EVENT,
  QMI_UIM_MESSAGE_CNF,
  QMI_UIM_MESSAGE_PHYSICAL_SLOTS_STATUS,
  QMI_UIM_MESSAGE_ALLOC_CLID,
  QMI_UIM_MESSAGE_DEALLOC_CLID,
  QMI_UIM_MESSAGE_FRAMEWORK_CMD,
  QMI_UIM_MESSAGE_RECOVERY_CB,
  QMI_UIM_MESSAGE_SIMLOCK_CB,
  QMI_UIM_MESSAGE_SUPPLY_VOLTAGE_IND,
  QMI_UIM_MESSAGE_SIMLOCK_REMOTE_SFS_CB,
  QMI_UIM_MESSAGE_OTASP_STATUS_IND,
  QMI_UIM_MESSAGE_SIMLOCK_SET_REMOTE_SFS_CB,
  QMI_UIM_MESSAGE_GBA_CB,
  QMI_UIM_MESSAGE_REMOTE_UNLOCK_CB,
  QMI_UIM_MESSAGE_SIMLOCK_TEMPORARY_UNLOCK_CB,
  QMI_UIM_MESSAGE_GBA_EVENT,
  QMI_UIM_MESSAGE_LPA_CB,
  QMI_UIM_MESSAGE_LPA_EVENT,
  QMI_UIM_MESSAGE_LPA_PROFILE_INFO_CB
} qmi_uim_message_enum_type;

typedef enum
{
  UIMI_CMD_VAL_RESET                       = QMI_UIM_RESET_REQ_V01,
  UIMI_CMD_VAL_READ_TRANSPARENT            = QMI_UIM_READ_TRANSPARENT_REQ_V01,
  UIMI_CMD_VAL_READ_RECORD                 = QMI_UIM_READ_RECORD_REQ_V01,
  UIMI_CMD_VAL_WRITE_TRANSPARENT           = QMI_UIM_WRITE_TRANSPARENT_REQ_V01,
  UIMI_CMD_VAL_WRITE_RECORD                = QMI_UIM_WRITE_RECORD_REQ_V01,
  UIMI_CMD_VAL_GET_FILE_ATTRIBUTES         = QMI_UIM_GET_FILE_ATTRIBUTES_REQ_V01,
  UIMI_CMD_VAL_SET_PIN_PROTECTION          = QMI_UIM_SET_PIN_PROTECTION_REQ_V01,
  UIMI_CMD_VAL_VERIFY_PIN                  = QMI_UIM_VERIFY_PIN_REQ_V01,
  UIMI_CMD_VAL_UNBLOCK_PIN                 = QMI_UIM_UNBLOCK_PIN_REQ_V01,
  UIMI_CMD_VAL_CHANGE_PIN                  = QMI_UIM_CHANGE_PIN_REQ_V01,
  UIMI_CMD_VAL_DEPERSONALIZATION           = QMI_UIM_DEPERSONALIZATION_REQ_V01,
  UIMI_CMD_VAL_REFRESH_REGISTER            = QMI_UIM_REFRESH_REGISTER_REQ_V01,
  UIMI_CMD_VAL_REFRESH_OK                  = QMI_UIM_REFRESH_OK_REQ_V01,
  UIMI_CMD_VAL_REFRESH_COMPLETE            = QMI_UIM_REFRESH_COMPLETE_REQ_V01,
  UIMI_CMD_VAL_REFRESH_GET_LAST_EVENT      = QMI_UIM_REFRESH_GET_LAST_EVENT_REQ_V01,
  UIMI_CMD_VAL_EVENT_REG                   = QMI_UIM_EVENT_REG_REQ_V01,
  UIMI_CMD_VAL_GET_CARD_STATUS             = QMI_UIM_GET_CARD_STATUS_REQ_V01,
  UIMI_CMD_VAL_POWER_DOWN                  = QMI_UIM_POWER_DOWN_REQ_V01,
  UIMI_CMD_VAL_POWER_UP                    = QMI_UIM_POWER_UP_REQ_V01,
  UIMI_CMD_VAL_CARD_STATUS_IND             = QMI_UIM_STATUS_CHANGE_IND_V01,
  UIMI_CMD_VAL_REFRESH_IND                 = QMI_UIM_REFRESH_IND_V01,
  UIMI_CMD_VAL_AUTHENTICATE                = QMI_UIM_AUTHENTICATE_REQ_V01,
  UIMI_CMD_VAL_CLOSE_SESSION               = QMI_UIM_CLOSE_SESSION_REQ_V01,
  UIMI_CMD_VAL_GET_SERVICE_STATUS          = QMI_UIM_GET_SERVICE_STATUS_REQ_V01,
  UIMI_CMD_VAL_SET_SERVICE_STATUS          = QMI_UIM_SET_SERVICE_STATUS_REQ_V01,
  UIMI_CMD_VAL_CHANGE_PROVISIONING_SESSION = QMI_UIM_CHANGE_PROVISIONING_SESSION_REQ_V01,
  UIMI_CMD_VAL_GET_LABEL                   = QMI_UIM_GET_LABEL_REQ_V01,
  UIMI_CMD_VAL_GET_CONFIGURATION           = QMI_UIM_GET_CONFIGURATION_REQ_V01,
  UIMI_CMD_VAL_SEND_APDU                   = QMI_UIM_SEND_APDU_REQ_V01,
  UIMI_CMD_VAL_SAP_CONNECTION              = QMI_UIM_SAP_CONNECTION_REQ_V01,
  UIMI_CMD_VAL_SAP_REQUEST                 = QMI_UIM_SAP_REQUEST_REQ_V01,
  UIMI_CMD_VAL_SAP_CONNECTION_IND          = QMI_UIM_SAP_CONNECTION_IND_V01,
  UIMI_CMD_VAL_LOGICAL_CHANNEL             = QMI_UIM_LOGICAL_CHANNEL_REQ_V01,
  UIMI_CMD_VAL_SUBSCRIPTION_OK_REQ         = QMI_UIM_SUBSCRIPTION_OK_REQ_V01,
  UIMI_CMD_VAL_GET_ATR                     = QMI_UIM_GET_ATR_REQ_V01,
  UIMI_CMD_VAL_OPEN_LOGICAL_CHANNEL        = QMI_UIM_OPEN_LOGICAL_CHANNEL_REQ_V01,
  UIMI_CMD_VAL_SESSION_CLOSED_IND          = QMI_UIM_SESSION_CLOSED_IND_V01,
  UIMI_CMD_VAL_REFRESH_REGISTER_ALL        = QMI_UIM_REFRESH_REGISTER_ALL_REQ_V01,
  UIMI_CMD_VAL_SET_FILE_STATUS             = QMI_UIM_SET_FILE_STATUS_REQ_V01,
  UIMI_CMD_VAL_SWITCH_SLOT                 = QMI_UIM_SWITCH_SLOT_REQ_V01,
  UIMI_CMD_VAL_GET_SLOTS_STATUS            = QMI_UIM_GET_SLOTS_STATUS_REQ_V01,
  UIMI_CMD_VAL_SLOTS_STATUS_IND            = QMI_UIM_SLOT_STATUS_CHANGE_IND_V01,
  UIMI_CMD_VAL_READ_TRANSPARENT_LONG_IND   = QMI_UIM_READ_TRANSPARENT_LONG_IND_V01,
  UIMI_CMD_VAL_SIM_BUSY_IND                = QMI_UIM_SIM_BUSY_STATUS_IND_V01,
  UIMI_CMD_VAL_GET_PLMN_NAME_TABLE_INFO    = QMI_UIM_GET_PLMN_NAME_TABLE_INFO_REQ_V01,
  UIMI_CMD_VAL_PERSONALIZATION             = QMI_UIM_PERSONALIZATION_REQ_V01,
  UIMI_CMD_VAL_INCREASE                    = QMI_UIM_INCREASE_REQ_V01,
  UIMI_CMD_VAL_RECOVERY                    = QMI_UIM_RECOVERY_REQ_V01,
  UIMI_CMD_VAL_RESELECT                    = QMI_UIM_RESELECT_REQ_V01,
  UIMI_CMD_VAL_RECOVERY_COMPLETE_IND       = QMI_UIM_RECOVERY_IND_V01,
  UIMI_CMD_VAL_SEND_STATUS                 = QMI_UIM_SEND_STATUS_REQ_V01,
  UIMI_CMD_VAL_GET_SIM_PROFILE             = QMI_UIM_GET_SIM_PROFILE_REQ_V01,
  UIMI_CMD_VAL_SET_SIM_PROFILE             = QMI_UIM_SET_SIM_PROFILE_REQ_V01,
  UIMI_CMD_VAL_SUPPLY_VOLTAGE              = QMI_UIM_SUPPLY_VOLTAGE_REQ_V01,
  UIMI_CMD_VAL_CARD_ACTIVATION_STATUS_IND  = QMI_UIM_CARD_ACTIVATION_STATUS_IND_V01,
  UIMI_CMD_VAL_DEPERSONALIZATION_SECURE    = QMI_UIM_DEPERSONALIZATION_SECURE_REQ_V01,
  UIMI_CMD_VAL_PERSONALIZATION_SECURE      = QMI_UIM_PERSONALIZATION_SECURE_REQ_V01,
  UIMI_CMD_VAL_EMERGENCY_ONLY              = QMI_UIM_EMERGENCY_ONLY_REQ_V01,
  UIMI_CMD_VAL_SIMLOCK_CONFIGURATION       = QMI_UIM_SIMLOCK_CONFIGURATION_REQ_V01,
  UIMI_CMD_VAL_GBA                         = QMI_UIM_GBA_REQ_V01,
  UIMI_CMD_VAL_GET_GBA_IMPI                = QMI_UIM_GET_GBA_IMPI_REQ_V01,
  UIMI_CMD_VAL_SEARCH_RECORD               = QMI_UIM_SEARCH_RECORD_REQ_V01,
  UIMI_CMD_VAL_REMOTE_UNLOCK               = QMI_UIM_REMOTE_UNLOCK_REQ_V01,
  UIMI_CMD_VAL_VERIFY_IMSI                 = QMI_UIM_VERIFY_IMSI_REQ_V01,
  UIMI_CMD_VAL_TEMPORARY_UNLOCK_STATUS_IND = QMI_UIM_TEMPORARY_UNLOCK_STATUS_IND_V01,
  UIMI_CMD_VAL_GBA_KEY_STATUS_IND          = QMI_UIM_GBA_KEY_STATUS_IND_V01,
  UIMI_CMD_VAL_PURGE_TEMPORARY_IDENTITY    = QMI_UIM_PURGE_TEMPORARY_IDENTITY_REQ_V01,
  UIMI_CMD_VAL_SWITCH_SLOT_EXT             = QMI_UIM_SWITCH_SLOT_EXT_REQ_V01,
  UIMI_CMD_VAL_GET_PHYSICAL_SLOT_MAPPING   = QMI_UIM_GET_PHYSICAL_SLOT_MAPPING_REQ_V01,
  UIMI_CMD_VAL_GET_PROFILE_INFO            = QMI_UIM_GET_PROFILE_INFO_REQ_V01,
  UIMI_CMD_VAL_GET_EID                     = QMI_UIM_GET_EID_REQ_V01,
  UIMI_CMD_VAL_DELETE_PROFILE              = QMI_UIM_DELETE_PROFILE_REQ_V01,
  UIMI_CMD_VAL_EUICC_MEMORY_RESET          = QMI_UIM_EUICC_MEMORY_RESET_REQ_V01,
  UIMI_CMD_VAL_ADD_PROFILE                 = QMI_UIM_ADD_PROFILE_REQ_V01,
  UIMI_CMD_VAL_ADD_PROFILE_IND             = QMI_UIM_ADD_PROFILE_IND_V01,
  UIMI_CMD_VAL_UPDATE_NICKNAME             = QMI_UIM_UPDATE_PROFILE_NICKNAME_REQ_V01,
  UIMI_CMD_VAL_TERMINAL_CAPABILITY         = QMI_UIM_TERMINAL_CAPABILITY_REQ_V01,
  UIMI_CMD_VAL_SET_APDU_BEHAVIOR           = QMI_UIM_SET_APDU_BEHAVIOR_REQ_V01,
  UIMI_CMD_VAL_PROFILE_USER_CONSENT        = QMI_UIM_PROFILE_USER_CONSENT_REQ_V01,
  UIMI_CMD_VAL_EUICC_DEFAULT_SERVER_ADDRESS  = QMI_UIM_EUICC_DEFAULT_SERVER_ADDRESS_REQ_V01,
  UIMI_CMD_VAL_MAP_PROVISIONING_SESSIONS_TO_SLOTS = QMI_UIM_MAP_PROVISIONING_SESSIONS_TO_SLOTS_REQ_V01,
  UIMI_CMD_VAL_ACTIVATE_SIMLOCK_CONFIG     = QMI_UIM_ACTIVATE_SIMLOCK_CONFIG_REQ_V01,
  UIMI_CMD_VAL_GET_SIMLOCK_TOKEN           = QMI_UIM_GET_SIMLOCK_TOKEN_REQ_V01,

  /* QC EXTERNAL QMI COMMAND RANGE IS 0x0000 - 0x5555.
     Add the next external QMI Command here */

  /* VENDOR SPECIFIC QMI COMMAND RANGE IS 0x5556 - 0xAAAA.
     IMPORTANT!
     Add the vendor specific QMI Commands within this range only to avoid
     conflicts with QC QMI commands that would get released in future */

  /* RESERVED QC QMI COMMAND RANGE IS 0xAAAB - 0xFFFE
     Internal QMI Commands must be added in DECREASING ORDER from Below */
  UIMI_CMD_VAL_WIDTH                  = 0xFFFF
} qmi_uimi_cmd_val_e_type;

/*---------------------------------------------------------------------------
  Values describe the level of encryption
---------------------------------------------------------------------------*/
typedef enum
{
  QMI_UIM_NO_ENCRYPTION         = 0,
  QMI_UIM_DATA_ONLY_ENCRYPTION  = 1,
  QMI_UIM_COMPLETE_ENCRYPTION   = 2
} qmi_uim_requested_encrytion_type;

/*---------------------------------------------------------------------------
  Values used in AUTH security restrictions EFS item
---------------------------------------------------------------------------*/
typedef enum
{
  QMI_UIM_AUTH_SEC_RESTRICTIONS_NONE   = 0,
  QMI_UIM_AUTH_SEC_RESTRICTIONS_ALL    = 1,
  QMI_UIM_AUTH_SEC_RESTRICTIONS_II_CLA = 2
} qmi_uim_auth_sec_restr_type;

/*---------------------------------------------------------------------------
  Values used in APDU security restrictions EFS item
---------------------------------------------------------------------------*/
typedef enum
{
  QMI_UIM_APDU_SEC_RESTRICTIONS_NONE   = 0,
  QMI_UIM_APDU_SEC_RESTRICTIONS_ALL    = 1,
  QMI_UIM_APDU_SEC_RESTRICTIONS_II_CLA = 2
} qmi_uim_apdu_sec_restr_type;

/*---------------------------------------------------------------------------
  Values used in simlock display EFS item
---------------------------------------------------------------------------*/
typedef enum
{
  QMI_UIM_SIMLOCK_DISPLAY_DEFAULT                                = 0,
  QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ANY_SLOT_POLICY      = 1,
  QMI_UIM_SIMLOCK_DISPLAY_FOR_FULL_SERVICE_ON_ANY_SLOT_POLICY    = 2,
  QMI_UIM_SIMLOCK_DISPLAY_FOR_VALID_CARD_ON_ALL_SLOT_POLICY      = 3
} qmi_uim_simlock_display_type;

/*---------------------------------------------------------------------------
  Type of QMI UIM PUP request state
     QMI_UIM_PUP_NOT_IN_PROGRESS:    No power_up is in progress
     QMI_UIM_PUP_MMGSDI_RSP_PENDING: Power_up req sent to MMGSDI and waiting
                                     for first rsp from MMGSDI
     QMI_UIM_PUP_RSP_SENT_TO_CLIENT: Rsp sent to QMI UIM client as part of first
                                     rsp from MMGSDI. Now, we need to drop second
                                     rsp from MMGSDI.
     QMI_UIM_PUP_CLIENT_RSP_PENDING: No rsp sent to client as part of first rsp
                                     from MMGSDI. Rsp will be send to client as
                                     part of second rsp from MMGSDI.
---------------------------------------------------------------------------*/
typedef enum {
  QMI_UIM_PUP_NOT_IN_PROGRESS                    = 0,
  QMI_UIM_PUP_MMGSDI_RSP_PENDING                 = 1,
  QMI_UIM_PUP_RSP_SENT_TO_CLIENT                 = 2,
  QMI_UIM_PUP_CLIENT_RSP_PENDING                 = 3
} qmi_uim_pup_state_enum_type;

/*---------------------------------------------------------------------------
  AID list types
---------------------------------------------------------------------------*/
typedef enum
{
  UIM_AID_LIST_WHITE_LIST              = 0,
  UIM_AID_LIST_BLACK_LIST              = 1
} qmi_uimi_aid_list_type;

/*---------------------------------------------------------------------------
  Possible refresh states
---------------------------------------------------------------------------*/
typedef enum
{
  UIM_REFRESH_STATE_IDLE               = 0,
  UIM_REFRESH_STATE_WAIT_FOR_OK        = 1,
  UIM_REFRESH_STATE_WAIT_FOR_COMPLETE  = 2
} qmi_uimi_refresh_state_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      QMI_UIM_IMPI_TYPE

   DESCRIPTION:    Structure which stores the IMPI data.
     data_len:     Data length of IMPI data
     data:         IMPI data
-------------------------------------------------------------------------------*/
typedef struct {
  uint8  data_len;
  uint8  data[QMI_UIM_IMPI_MAX_V01];
} qmi_uim_impi_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_IMSI_DIGITS_DATA_TYPE

   DESCRIPTION:        Structure containing IMSI digits data
     imsi_digits_len:  Data length
     imsi_digits:      IMSI data in BCD format
-------------------------------------------------------------------------------*/
typedef struct{
  uint8   imsi_digits_len;
  uint8   imsi_digits[QMI_UIM_IMSI_DIGIT_LEN];
} qmi_uim_imsi_digits_data_type;


/*---------------------------------------------------------------------------
  Structure for QMI UIM messages
---------------------------------------------------------------------------*/
typedef struct
{
  /* This needs to be in first position */
  q_link_type                                     link_next;

  qmi_uim_message_enum_type                       message_type;

  union
  {
    struct
    {
      mmgsdi_return_enum_type                     status;
      mmgsdi_cnf_enum_type                        cnf_type;
      mmgsdi_cnf_type                             cnf_value;
    }                                             cnf;
    struct
    {
      mmgsdi_event_data_type                      evt;
    }                                             event;
    struct
    {
      qmi_framework_common_msg_hdr_type           cmd_hdr;
    }                                             clid_cb;
    struct
    {
      uimqmi_framework_cmd_type                   cmd;
    }                                             framework_cmd;
    struct
    {
      uint8                                       num_slots;
      uim_phy_slot_status_type                  * slot_status_ptr;
    }                                             physical_slots_info;
    struct
    {
      uim_slot_type                               slot;
      const void                                * user_data;
    }                                             recovery_data;
    struct
    {
      simlock_result_enum_type                    status;
      simlock_message_response_data_type          data;
      const void                                * user_data_ptr;
    }                                             simlock;
    struct
    {
      uim_slot_type                               slot;
      uimdrv_qmi_indications_type                 ldo_state;
      uimdrv_qmi_power_management_callback_type   uim_callback;
    }                                             supply_voltage;
    struct
    {
      simlock_file_operation_enum_type            operation;
      simlock_token_id_type                       request_token_id;
      simlock_file_type                           file;
      simlock_data_type                           simlock_data;
      simlock_remote_sfs_response_cb_type         simlock_resp_cb;
    }                                             simlock_remote_sfs_req;
    struct
    {
      gstk_slot_id_enum_type                      slot;
      gstk_otasp_act_status_enum_type             status;
    }                                             otasp_status_ind;
    struct
    {
      simlock_result_enum_type                    status;
      const void                                * user_data_ptr;
    }                                             simlock_set_remote_sfs_resp;
    struct
    {
      gba_result_enum_type                        status;
      gba_response_data_type                      cnf_value;
      const void                                * user_data_ptr;
    }                                             gba_cnf;
    struct
    {
      simlock_result_enum_type                    status;
      simlock_rsu_message_response_data_type      data;
      const void                                * user_data_ptr;
    }                                             remote_unlock_resp;
    struct
    {
      gba_event_data_type                         evt;
    }                                             gba_evt;
    struct
    {
      lpa_result_enum_type                        status;
      lpa_response_data_type                      cnf_value;
      void                                      * user_data_ptr;
    }                                             lpa_cnf;
    struct
    {
      lpa_event_data_type                         evt;
      lpa_result_enum_type                        status;
    }                                             lpa_evt;
  }                              data;
} qmi_uim_message_type;

/*---------------------------------------------------------------------------
  Token information extracted from a command
---------------------------------------------------------------------------*/
typedef struct
{
  boolean                    is_valid;
  uint32                     token;
} qmi_uimi_ind_token_info_type;

/*---------------------------------------------------------------------------
  Userdata used for MMGSDI functions
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_uimi_cmd_val_e_type               request_id;
  qmi_uimi_ind_token_info_type          ind_token;
  uint8                                 clid;
  uint32                                client_reg_count;
  void *                                cmd_buf_ptr;
  /* this structure can be expanded with input parameters
     required at the end. Right now, this is not necessary */
  union
  {
    struct
    {
      qmi_uim_requested_encrytion_type  encryption_status;
      mmgsdi_session_type_enum_type     session_type;
      mmgsdi_access_type                file_access;
    }                                   read_transparent;
    struct
    {
      mmgsdi_access_type                file_access;
      mmgsdi_rec_num_type               first_record;
      uint16                            total_records;
      uint16                            read_records;
      qmi_uim_additional_records_type * records_cnf;
    }                                   read_record;
    struct
    {
      mmgsdi_access_type                file_access;
      mmgsdi_rec_num_type               record;
      mmgsdi_write_data_type            data;
    }                                   write_record;
    struct
    {
      mmgsdi_access_type                file_access;
    }                                   get_file_attributes;
    struct
    {
      uint8                             pin1_length;
      uint8                             pin1_value[QMI_UIM_PIN_MAX_V01];
    }                                   pin_operation;
    struct
    {
      boolean                           hidden_key;
      boolean                           index_in_dir;
      mmgsdi_service_enum_type          service_id;
    }                                   get_service_status;
    struct
    {
      /* These are the items requested by the client */
      boolean                           automatic_selection_request;
      boolean                           personalization_request;
      boolean                           halt_subscription_request;
      boolean                           usb_uicc_supported_request;
      boolean                           sap_client_supported_request;
      boolean                           perso_reactivation_request;
      boolean                           remote_simlock_storage_request;
      boolean                           emergency_only_mode_request;
      boolean                           extended_apdu_request;
      boolean                           inactive_simlock_config;
      /* These are the values to be provided in the response */
      boolean                           automatic_selection;
      boolean                           halt_subscription;
      boolean                           usb_uicc_supported;
      boolean                           sap_client_supported;
      boolean                           extended_apdu;
    }                                   get_configuration;
    struct
    {
      uint8                             ins_value;
    }                                   sap_request;
    struct
    {
      uint8                             channel_index;
      uint8                             session_open_type;
      mmgsdi_session_type_enum_type     session_type;
    }                                   session_open_request;
    struct
    {
      uint8                             channel_index;
    }                                   session_close_request;
    struct
    {
      simlock_category_enum_type        category;
      simlock_slot_enum_type            slot;
      simlock_result_enum_type          result;
      uint32                            num_retries;
      boolean                           ck_present;
      uint16                            request_id;
    }                                   simlock_unlock;
    struct
    {
      boolean                           ck_present;
      uint16                            request_id;
      simlock_slot_enum_type            slot;
      simlock_ck_data_type              ck;
      uint8                             total_list_cnt;
      uint8                             next_list_to_lock;
      simlock_category_data_type      * code_ptr;
    }                                   simlock_lock;
    struct
    {
      uim_auth_context_enum_v01         auth_context;
    }                                   authenticate;
    struct
    {
      uint16                            request_id;
    }                                   simlock_set_device_mode;
    struct
    {
      uint16                            request_id;
    }                                   simlock_set_remote_sfs;
    struct
    {
      uint8                             channel_index;
    }                                   reselect_request;
    struct
    {
      gba_naf_fqdn_type                 fqdn_data;
      uint32                            evt_reg_mask;
    }                                   gba;
    struct
    {
      uint16                            purge_guti_mask;
    }                                   purge_temporary_identity;
    struct
    {
      mmgsdi_slot_id_enum_type          slot;
      boolean                           is_profile_query;
    }                                   lpa;
  }                                     data;
} qmi_uimi_userdata_type;

/*---------------------------------------------------------------------------
  List of files a client has registered for refresh
---------------------------------------------------------------------------*/
typedef struct refresh_files_list
{
  mmgsdi_session_id_type             session_id;
  mmgsdi_path_type                   file;
  struct refresh_files_list        * next_ptr;
}qmi_uim_refresh_file_node_type;

/*---------------------------------------------------------------------------
  Extended APDU data
---------------------------------------------------------------------------*/
typedef struct extended_apdu_data
{
  qmi_uim_tlv_slot_type                  slot;
  uint32                                 token;
  mmgsdi_send_apdu_data_type             apdu_data;
  uint32                                 received_len;
  boolean                                channel_id_valid;
  uint16                                 channel_id;  
}qmi_uim_extended_apdu_data_type;

/*---------------------------------------------------------------------------
  QMI UIM instance state definition & UIM client state definition
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_common_client_state_type       common; /* must be first since we alias to
                                                uimqmi_common_client_state_type */
  int16                              instance; /* must be second since we alias to
                                                  uimqmi_common_client_state_type */
  int16                              service_id; /* must be third since we alias to
                                                  uimqmi_common_client_state_type */
  struct
  {
    qmi_uim_registration_event_type  reg_event_type;
    boolean                          sap_intermediate_get_resp;
    uint32                           refresh_notify_mask;
    boolean                          refresh_vote;
    uint16                           refresh_num_files;
    qmi_uim_refresh_file_node_type * refresh_files_list;
    uint32                           refresh_all_notify_mask;
    boolean                          card_status_ind_postponed;
    boolean                          supply_voltage_pending_ack[QMI_UIM_MAX_CARD_COUNT];
    uint32                           gba_evt_reg_mask;
    uint16                           purge_guti_mask;
    struct
    {
      uint8                          add_profile_initiated;
      boolean                        user_consent_supported;
    }                                add_profile_client_info[QMI_UIM_MAX_CARD_COUNT];
    qmi_uim_extended_apdu_data_type  extended_apdu;
  }uim_info;
} qmi_uimi_client_state_type;

/*---------------------------------------------------------------------------
  Watermark queue to hold deferred responses
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_common_svc_state_type    common; /* must be first since we alias to
                                          uimqmi_common_client_state_type*/
  qmi_uimi_client_state_type * client[UIMQMI_SVC_MAX_CLIDS];
  uint32                       client_reg_count[UIMQMI_SVC_MAX_CLIDS];
} qmi_uimi_state_type;

/*---------------------------------------------------------------------------
  Cache with record type used for write record
---------------------------------------------------------------------------*/
typedef struct qmi_uimi_file_record_type
{
  mmgsdi_session_id_type             session_id;
  mmgsdi_access_type                 file_access;
  mmgsdi_file_structure_enum_type    record_type;
  struct qmi_uimi_file_record_type * next_ptr;
} qmi_uimi_file_record_type;

/*---------------------------------------------------------------------------
  AID value and source
---------------------------------------------------------------------------*/
typedef struct
{
  mmgsdi_static_data_type  aid;
  boolean                  from_efs;
} qmi_uim_aid_entry_type;

/*---------------------------------------------------------------------------
  Whitelist/blacklist details used for APDU security
---------------------------------------------------------------------------*/
typedef struct
{
  qmi_uimi_aid_list_type           aid_list_type;
  uint16                           aid_count;
  qmi_uim_aid_entry_type         * aid_list_ptr;
} qmi_uimi_apdu_sec_aid_info_type;

/*---------------------------------------------------------------------------
  Channel details for Send APDU
---------------------------------------------------------------------------*/
typedef struct
{
  mmgsdi_session_id_type           mmgsdi_session_id;
  mmgsdi_slot_id_enum_type         slot;
  uint8                            logical_channel;
  mmgsdi_static_data_type          aid;
  qmi_uimi_client_state_type     * opening_cl_sp;
} qmi_uim_apdu_channel_info_type;

/*---------------------------------------------------------------------------
  Pending request info for non provisioning sessions
---------------------------------------------------------------------------*/
typedef struct qmi_uimi_pending_request_type
{
  qmi_uimi_cmd_val_e_type                 command_id;
  uint8                                   tlv_data_count;
  qmi_uim_tlv_item_type *                 tlv_data_ptr;
  void *                                  cmd_buf_ptr;
  uint8                                   clid;
  uint32                                  client_reg_count;
  struct qmi_uimi_pending_request_type  * next_ptr;
} qmi_uimi_pending_request_type;

/*---------------------------------------------------------------------------
   Non provisioning session info
---------------------------------------------------------------------------*/
typedef struct
{
  mmgsdi_session_id_type             mmgsdi_session_id;
  mmgsdi_session_type_enum_type      session_type;
  mmgsdi_static_data_type            aid;
  qmi_uimi_pending_request_type    * pending_req_ptr;
  /* Client Info */
  struct
  {
    /* Number of clients linked to the non-prov. session */
    uint16                           client_count;
    /* Client state pointers of all clients linked to the session */
    qmi_uimi_client_state_type *     opening_cl_sp[UIM_MAX_NON_PROV_CLIENTS];
  }                                  client_info;
} qmi_uim_non_prov_session_info_type;

typedef struct
{
  mmgsdi_profile_id_enum_type      profile_id;
  lpa_profile_info_type            profile_info;
} qmi_uim_euicc_profile_info_type;


/* QMI UIM NAA Refresh Info */
typedef struct
{
  qmi_uimi_refresh_state_type        state;
  boolean                            ok_to_init;
  boolean                            complete;
  uint16                             client_count;
  qmi_uimi_client_state_type *       client_list[UIM_REFRESH_MAX_CLIENTS];
  struct
  {
    mmgsdi_session_id_type           session_id;
    mmgsdi_refresh_evt_info_type    *event_ptr;
  }                                  cache;
}qmi_uim_refresh_info_type;

/*---------------------------------------------------------------------------
  Global data for QMI UIM module
---------------------------------------------------------------------------*/
typedef struct
{
  /* Indicates if the global variable has been initialized or not */
  boolean                              init;
  /* MMGSDI client and session ids */
  mmgsdi_client_id_type                mmgsdi_client_id;
  mmgsdi_session_id_type               mmgsdi_gw_session_id[QMI_UIM_MAX_PROV_SESSIONS];
  mmgsdi_session_id_type               mmgsdi_1x_session_id[QMI_UIM_MAX_PROV_SESSIONS];
  mmgsdi_session_id_type               mmgsdi_card_slot_session_id[QMI_UIM_MAX_CARD_SESSIONS];
  /* Non provisioning session ids */
  qmi_uim_non_prov_session_info_type  *non_prov_session[UIM_MAX_NON_PROV_SESSIONS];
  /* Refresh details */
  qmi_uim_refresh_info_type           *refresh_info_ptr[UIM_REFRESH_SESSION_COUNT];
  /* Channel details for Send APDU */
  qmi_uim_apdu_channel_info_type     * apdu_channel[UIM_MAX_APDU_CHANNEL_COUNT];
  /* Session ids for non prov sessions */
  mmgsdi_session_id_type               non_prov_refresh_session_id[UIM_REFRESH_NON_PROV_COUNT];
  /* SAP details */
  struct
  {
    boolean                            is_enabled;
    mmgsdi_sap_state_enum_type         sap_state;
  }                                    sap_info[QMI_UIM_MAX_CARD_COUNT];
  /* APDU security AID list details for APDU security */
  qmi_uimi_apdu_sec_aid_info_type      apdu_sec_aid_info;
  /* Card state */
  qmi_uim_card_state                   card_state;
  /* Card state validity status */
  boolean                              card_state_valid[QMI_UIM_MAX_CARD_COUNT];
  /* Card is eUICC or not */
  boolean                              is_euicc_card[QMI_UIM_MAX_CARD_COUNT];
  /* Cache with record type */
  qmi_uimi_file_record_type *          record_cache_ptr;
  /* Power up commands */
  qmi_uim_pup_state_enum_type          power_up_state[QMI_UIM_MAX_CARD_COUNT];
  /* Token value for long APDU Response/Indication */
  uint32                               apdu_response_token;
  /* Token value for long read data Response/Indication */
  uint32                               long_read_response_token;
  /* Indicates if silent PIN1 verification is supported */
  boolean                              silent_pin1_supported;
  /* Indicates if APDU security restriction is supported */
  qmi_uim_apdu_sec_restr_type          apdu_sec_restr_value;
  /* Indicates close logical channel indication preference */
  boolean                              close_channel_sync;
  /* Indicates if AUTHENTICATE security restriction is supported */
  qmi_uim_auth_sec_restr_type          auth_sec_restr_value;
  /* Indicates if SAP security restriction is supported */
  boolean                              sap_sec_restr_supported;
  /* Indicates if the provisioning is auto or manual*/
  boolean                              automatic_provisioning;
  /* Stores QMI_UIM's EFS directory name */
  char                               * efs_directory_list[QMI_UIM_EFS_DIRECTORY_LIST_SIZE];
  /* Stores status of all physical slots */
  struct
  {
    uint8                              num_slots;
    uim_phy_slot_status_type         * slot_status_ptr;
  }                                    physical_slots_info;
  /* Message queue */
  q_type                               qmi_uim_q;
#ifdef FEATURE_UIM_SSM
  /* SSM info indicates SSM client id and status */
  struct
  {
    uint32                             qmi_uim_ssm_id;
    ssm_err_t                          qmi_uim_ssm_init_err;
  }                                    ssm_info;
#endif /* FEATURE_UIM_SSM */
  /* Stores the supply voltage callback for each slot to notify drivers
     it can proceed with power down */
  uimdrv_qmi_power_management_callback_type
                                       supply_voltage_cb[QMI_UIM_MAX_CARD_COUNT];

  /* Indicates simlock mode preference */
  qmi_uim_simlock_display_type         simlock_display_for_valid_card;

  /* Stores the remote SFS response callback to notify
     simlock task about the pending read/write request */
  simlock_remote_sfs_response_cb_type  simlock_remote_sfs_operation_resp_cb;

  /* Stores the simlock slot policy */
  simlock_slot_policy_enum_type        simlock_slot_policy;

  /* Stores the status if a temporary unlock is runnning for
     a given personalization feature for all slots.*/
  boolean                              temp_unlock_info[QMI_UIM_MAX_CARD_COUNT][QMI_UIM_MAX_NUMBER_PERSO_FEATURES];

  /* This is used to mark if the indication due to some event on some slot is
     dropped because not all the slots have reached beyond slot detection stage
     yet.*/
  boolean                              indication_dropped;

  /* This is used to mark if the indication is sent for some slot while the
     other slot detection has not yet started */
  boolean                              indication_sent;

  /* Info about the eUICC profiles */
  qmi_uim_euicc_profile_info_type     *qmi_uim_euicc_profile_data_ptr[QMI_UIM_MAX_CARD_COUNT][QMI_UIM_PROFILES_MAX_V01];
  boolean                              is_recovery_in_progress[QMI_UIM_MAX_CARD_COUNT];

  /* Stores EID for all slots */
  struct
  {
    boolean                            eid_valid;
    uint8                              eid_len;
    uint8                              eid[UIM_MAX_EID_SIZE];
  } eid_data[QMI_UIM_MAX_CARD_COUNT];
} qmi_uim_global_type;

extern qmi_uim_global_type  * const qmi_uim_global_ptr;

extern qmi_uimi_state_type           *qmi_uim_state_ptr;

/*=============================================================================

                   F U N C T I O N S

=============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_EXECUTE_READ_TRANSPARENT()

  DESCRIPTION
    Reads the content of a transparent file on the card

  PARAMETERS
    command_id        : command id
    session_id        : session id
    file_access       : file to be read
    offset            : offset in the file
    length            : length to be read
    encryption_status : requested encrption type
    token_ptr         : token pointer
    cmd_buf_ptr       : userdata
    clid              : client id of the control point

  RETURN VALUE
    errval      : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_execute_read_transparent
(
  qmi_uimi_cmd_val_e_type             command_id,
  mmgsdi_session_id_type              session_id,
  mmgsdi_access_type                  file_access,
  mmgsdi_offset_type                  offset,
  mmgsdi_len_type                     length,
  qmi_uim_requested_encrytion_type    encryption_status,
  const uint32                      * token_ptr,
  void *                              cmd_buf_ptr,
  uint8                               clid
);

/*===========================================================================
  FUNCTION QMI_UIM_OPEN_SESSION()

  DESCRIPTION
    Opens a new non provisioning session on the fly and enqueues
    the command in the list of the session, so it can be executed
    later on, when the session is successfully opened.

  PARAMETERS
    command_id     : command id
    tlv_data_ptr   : pointer to list of TLVs
    tlv_data_count : number of TLVs
    cmd_buf_ptr    : userdata of the command

  RETURN VALUE
    errval         : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_open_session
(
  qmi_uimi_cmd_val_e_type            command_id,
  qmi_uim_tlv_item_type            * tlv_data_ptr,
  uint8                              tlv_data_count,
  void                             * cmd_buf_ptr,
  qmi_uimi_client_state_type       * cl_sp
);

/*===========================================================================
  FUNCTION QMI_UIMI_MMGSDI_CALLBACK()

  DESCRIPTION
    This function is executed as callback of MMGSDI API. This is
    executed in the context of MMGSDI task and needs to make a deep
    copy of the results and post a command to the QMI task to be
    processed.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_mmgsdi_callback
(
  mmgsdi_return_enum_type status,
  mmgsdi_cnf_enum_type    cnf_type,
  const mmgsdi_cnf_type  *cnf_ptr
);

#endif /* QMI_UIM_INTERNAL_H */
