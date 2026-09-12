/*================================================================================

DESCRIPTION


Copyright (c) 2009-2017, 2019 - 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_parsing.h#5 $$ $DateTime: 2020/02/17 23:22:31 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
02/13/20   vgd     Get ATR, ICCID, EID from MMGSDI when card_info is populated
12/28/19   sg      Added support for NS+SP category
12/23/19   vdc     Update supported SIMLOCK features
09/17/19   vdc     SimLock solution for activation at Point Of Sale
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
04/25/19   mm      Added support for extended simlock data
07/13/17   av      Enhanced simlock RSU solution
04/21/17   vdc     Added support for refresh enforcement policy
04/18/17   me      Added support to euicc bit in atr 
04/14/17   kv      Added pass-through mode support
04/12/17   ar      Add support for retrieve/configure default server address 
04/06/17   gm      Added support for extended APDU
01/19/17   me      Populating path internally and parsing IMSI, FPLMN, EHPLMN
12/07/16   av      LPA phase2 initial changes
11/03/16   bcho    Added support for ICCID simlock
10/06/16   ar      Added support for SP + EHPLMN simlock
09/22/16   gm      Added support for set APDU behavior
08/18/16   ar      Add support for simlock storage and emergency only mode
08/03/16   vdc     Added support for SPN based SIMLOCK feature
07/15/16   sp      Seperate SAP and ERA-GLONASS related functionality
06/16/16   vdc     Split QMI UIM parsing files for supporting IOE
06/16/16   av      Support for setting terminal capability from AP
05/26/16   vdc     Move internal enums, structs to new internal header file
04/15/16   vdc     Split QMI UIM files for supporting IOE
04/11/16   ar      Support for eUICC
03/15/16   vr      SAM CARD support
02/19/16   vdc     Send control key presence in get config presence
01/20/16   gm      Added support to purge temporary identity
01/12/16   vdc     Populate verify retries correctly
11/16/15   yt      Support for second BSF and key status events
10/23/15   bcho    Support for multi byte GID
09/28/15   vv      Support for reuse code data
08/19/15   vv      Added support to retrieve retry attempts left for locking
08/12/15   stv     To send temporary unlock status indication
07/06/15   bcho    Redundant checks for SIMLOCK
05/15/15   vv      Support for RSU
04/30/15   vdc     Added support for APN for bootstrapping
04/20/15   ar      Support for terminate_app TLV in qmi_uim_close_session
04/08/15   at      Support for SIM detected usecase
03/19/15   at      Introducing new relock feature in SIMLock
03/10/15   vdc     Replace hard coded values with IDL defined values
03/04/15   vdc     Added support for QMI UIM GBA
02/25/15   ar      Addition of QMI_UIM_SEARCH_RECORD interface
02/20/15   vv      Remove perso related code
02/03/15   xj      Add USB UICC and SAP CLIENT supported interface
02/03/15   at      Replacement of Secapis to use a derived key & HMAC
12/26/14   vv      Fix enabling a lock with more than 20 codes of lock data
12/16/14   av      Apply auth restr via NV for IIC only if USIM is selected
12/10/14   vdc     Added support for ERA-GLONASS
10/10/14   vv      Add support for remote simlock configuration
10/08/14   vdc     Added support for UICC activation status indication
09/30/14   vv      Add support for emergency only command
09/25/14   tl      Add support for secure read
09/17/14   tl      Add support for perso and deperso secure
08/29/14   tl      Add support for supply voltage command and indication
07/25/14   vv      Added support for perso command
07/22/14   at      Fix error in ISIM Auth response for different contexts
06/30/14   vv      Remove perso code
06/12/14   av      Enhance GET_SERVICE_STATUS to accomodate all service numbers
05/22/14   tl      Added support for recovery complete indications
05/13/14   vdc     Support for reduced card status indications
05/12/14   df      Add QMI support for status cmd
03/26/14   tl      SIM Lock secondary revisions
03/18/14   tl      Introduce new SIM Lock feature
02/25/14   yt      Support for QMI_UIM_RESELCT_REQ
02/24/14   at      Support for read transparent long indication
12/09/13   spo     Fix for handling auth request for USIM and ISIM
11/20/13   tkl     Added API for Increase
10/27/13   df      Add QMI_UIM_CARD_ERROR_SAP_CONNECTED to error type
10/04/13   vv      Added API to return se13 table release date and source
09/23/13   df      Support for SIM busy
09/09/13   yt      Remove redundant code
08/08/13   ks      Support for subscription manager
07/15/13   av      Introduction of new NULL_BYTES card error type
07/02/13   yt      Dynamically allocate slot and app data
06/21/13   kk      Updated support for triple sim
05/21/13   vv      Extend session closed indication for all sessions
02/22/13   yt      Support for slot 3
02/12/13   at      Added support for file status TLV
02/07/13   yt      Support for activation/deactivation of EFs
11/02/12   yt      Add support for ACL in SET and GET SERVICE STATUS commands
09/07/12   tl      Added support for SGLTE dual slots
08/29/12   at      Optional TlV in SAP connection req to check for active calls
08/28/12   spo     Added support for Session Closed Indication
08/16/12   at      New command handling for QMI_UIM_REFRESH_REGISTER_ALL
08/16/12   tl      Add session close without app termination functionality
05/14/12   at      Add new TLV to QMI_UIM_GET_CARD_STATUS command
04/11/12   at      Indication support for sending long APDUs
03/02/12   vv      Added optional TLV in send APDU request
03/02/12   vv      Added support for querying ESN status
02/27/12   yt      Add function to construct file_length TLV
01/03/12   nb      Added optional TLV for QMI configuration
12/21/11   nmb     Secure channel perso for apps
12/15/11   nb      Added new card error type
10/12/11   at      Handling intermediate get response in SAP commands
10/05/11   yt      Added support for refresh of non provisioning sessions
                   Removed flag to indicate if TLV is mandatory or optional
09/16/11   yt      Added optional TLV for status of hot swap switch
08/16/11   at      Indication support for various commands
08/01/11   at      Support for keyref_id TLV in PIN commands
08/01/11   at      Support for using session id on an open channel id
08/01/11   vs      Support app selection with FCI via logical channel operation
06/27/11   at      Renamed qmi_uim_response_sap_generic()
04/28/11   mib     Fixed crash due to invalid free reading multiple records
04/26/11   vs      Support for subscription ok request
04/07/11   mib     Added support to read multiple records at the same time
04/01/11   mib     Changed TLV for the encrypted PIN passed for verify
03/09/11   at      Added support for PIN1 encryption/decryption
03/03/11   mib     Added support to manage logical channels to send APDUs
03/01/11   at      Added support for SAP request & indication
01/12/11   mib     Added support for CSIM file paths
01/04/11   mib     Added support for Compute IP Authentication
12/21/10   at      Added support for SAP connection request
12/17/10   mib     Split APDU request into two TLVs
12/09/10   at      Added support for send APDU request
10/22/10   mib     Added support for get configuration request
08/19/10   mib     Added support for POSSIBLY_REMOVED error code
08/05/10   tkl     QMI UIM & QMI CAT split from data package
06/16/10   mib     Added support for hidden key
06/07/10   mib     Added support for GET LABEL and SESSION CHANGE requests
05/31/10   mib     Added storage of application label
05/05/10   mib     Improved mechanism to pass TLV id for responses
04/06/10   mib     Added support for authenticate and close session.
                   Changed TLV ids for consistency with other QMI services
01/13/10   mib     Initial version
=============================================================================*/

#ifndef QMI_UIM_PARSING_H
#define QMI_UIM_PARSING_H

/*=============================================================================

                         INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "customer.h"
#include "comdef.h"
#include "mmgsdilib.h"
#include "mmgsdisessionlib.h"
#include "mmgsdilib_common.h"
#include "uim_v.h"
#include "uim_p.h"
#include "simlock_common.h"
#include "gstk_exp_v.h"
#include "simlock_modem_p.h"
#include "gba_lib.h"
#include "user_identity_module_v01.h"
#include "lpalib.h"
#include "ds_qmi_fw_common.h"

/*---------------------------------------------------------------------------
  Request TLVs tags
---------------------------------------------------------------------------*/
#define UIMI_TLV_TAG_MANDATORY_1                 (0x01)
#define UIMI_TLV_TAG_MANDATORY_2                 (0x02)
#define UIMI_TLV_TAG_MANDATORY_3                 (0x03)
#define UIMI_TLV_TAG_MANDATORY_4                 (0x04)
#define UIMI_TLV_TAG_MANDATORY_5                 (0x05)
#define UIMI_TLV_TAG_OPTIONAL_1                  (0x10)
#define UIMI_TLV_TAG_OPTIONAL_2                  (0x11)
#define UIMI_TLV_TAG_OPTIONAL_3                  (0x12)
#define UIMI_TLV_TAG_OPTIONAL_4                  (0x13)
#define UIMI_TLV_TAG_OPTIONAL_5                  (0x14)
#define UIMI_TLV_TAG_OPTIONAL_6                  (0x15)
#define UIMI_TLV_TAG_OPTIONAL_7                  (0x16)
#define UIMI_TLV_TAG_OPTIONAL_8                  (0x17)
#define UIMI_TLV_TAG_OPTIONAL_9                  (0x18)
#define UIMI_TLV_TAG_OPTIONAL_10                 (0x19)
#define UIMI_TLV_TAG_OPTIONAL_11                 (0x1A)
#define UIMI_TLV_TAG_OPTIONAL_12                 (0x1B)
#define UIMI_TLV_TAG_OPTIONAL_13                 (0x1C)
#define UIMI_TLV_TAG_OPTIONAL_14                 (0x1D)
#define UIMI_TLV_TAG_OPTIONAL_15                 (0x1E)
#define UIMI_TLV_TAG_OPTIONAL_16                 (0x1F)
#define UIMI_TLV_TAG_OPTIONAL_17                 (0x20)
#define UIMI_TLV_TAG_OPTIONAL_18                 (0x21)
#define UIMI_TLV_TAG_OPTIONAL_19                 (0x22)
#define UIMI_TLV_TAG_OPTIONAL_20                 (0x23)

/*---------------------------------------------------------------------------
  Request TLVs types
---------------------------------------------------------------------------*/
#define UIMI_TLV_TYPE_SESSION_INFORMATION                    (0x01)
#define UIMI_TLV_TYPE_FILE_ID                                (0x02)
#define UIMI_TLV_TYPE_READ_TRANSPARENT                       (0x03)
#define UIMI_TLV_TYPE_READ_RECORD                            (0x04)
#define UIMI_TLV_TYPE_WRITE_TRANSPARENT                      (0x05)
#define UIMI_TLV_TYPE_WRITE_RECORD                           (0x06)
#define UIMI_TLV_TYPE_REGISTER_REFRESH                       (0x07)
#define UIMI_TLV_TYPE_OK_FOR_REFRESH                         (0x08)
#define UIMI_TLV_TYPE_REFRESH_COMPLETE                       (0x09)
#define UIMI_TLV_TYPE_SET_PIN_PROTECTION                     (0x0A)
#define UIMI_TLV_TYPE_VERIFY_PIN                             (0x0B)
#define UIMI_TLV_TYPE_UNBLOCK_PIN                            (0x0C)
#define UIMI_TLV_TYPE_CHANGE_PIN                             (0x0D)
#define UIMI_TLV_TYPE_DEPERSONALIZATION                      (0x0E)
#define UIMI_TLV_TYPE_SLOT                                   (0x0F)
#define UIMI_TLV_TYPE_EVENT_REG                              (0x10)
#define UIMI_TLV_TYPE_AUTHENTICATE                           (0x11)
#define UIMI_TLV_TYPE_CAPABILITIES_MASK                      (0x12)
#define UIMI_TLV_TYPE_SET_FDN_STATUS                         (0x13)
#define UIMI_TLV_TYPE_APPLICATION_INFORMATION                (0x14)
#define UIMI_TLV_TYPE_SESSION_CHANGE                         (0x15)
#define UIMI_TLV_TYPE_APDU_REQUEST                           (0x16)
#define UIMI_TLV_TYPE_SAP_CONNECT                            (0x17)
#define UIMI_TLV_TYPE_SAP_DISCONNECT_MODE                    (0x18)
#define UIMI_TLV_TYPE_SAP_REQUEST                            (0x19)
#define UIMI_TLV_TYPE_LOGICAL_CHANNEL                        (0x1A)
#define UIMI_TLV_TYPE_AID                                    (0x1B)
#define UIMI_TLV_TYPE_DECRYPTED_PIN                          (0x1C)
#define UIMI_TLV_TYPE_READ_RECORD_LAST                       (0x1D)
#define UIMI_TLV_TYPE_SUBSCRIPTION_OK                        (0x1E)
#define UIMI_TLV_TYPE_FILE_CONTROL_INFO                      (0x1F)
#define UIMI_TLV_TYPE_PIN_KEYREF_ID                          (0x20)
#define UIMI_TLV_TYPE_IND_TOKEN                              (0x21)
#define UIMI_TLV_TYPE_IGNORE_HOTSWAP_SWITCH                  (0x22)
#define UIMI_TLV_TYPE_SAP_INTERMEDIATE_GET_RESP              (0x23)
#define UIMI_TLV_TYPE_ENCRYPTED                              (0x24)
#define UIMI_TLV_TYPE_DECRYPTED_IMSI                         (0x25)
#define UIMI_TLV_TYPE_CONFIGURATION_MASK                     (0x26)
#define UIMI_TLV_TYPE_SKIP_PROCEDURE_BYTES                   (0x27)
#define UIMI_TLV_TYPE_TERMINATE_APP                          (0x28)
#define UIMI_TLV_TYPE_REGISTER_FOR_REFRESH                   (0x29)
#define UIMI_TLV_TYPE_REFRESH_VOTE_FOR_INIT                  (0x2A)
#define UIMI_TLV_TYPE_SAP_CONNECTION_CONDITION               (0x2B)
#define UIMI_TLV_TYPE_SET_ACL_STATUS                         (0x2C)
#define UIMI_TLV_TYPE_FILE_STATUS                            (0x2D)
#define UIMI_TLV_TYPE_EXTENDED_CARD_STATUS                   (0x2E)
#define UIMI_TLV_TYPE_PHYSICAL_SLOT                          (0x2F)
#define UIMI_TLV_TYPE_INCREASE                               (0x30)
#define UIMI_TLV_TYPE_SELECT_MODE                            (0x31)
#define UIMI_TLV_TYPE_STATUS_CMD_MODE                        (0x32)
#define UIMI_TLV_TYPE_STATUS_CMD_RESP                        (0x33)
#define UIMI_TLV_TYPE_SERVICE_NUMBER                         (0x34)
#define UIMI_TLV_TYPE_PERSONALIZATION                        (0x35)
#define UIMI_TLV_TYPE_GW_NW_PERSO_CODE                       (0x36)
#define UIMI_TLV_TYPE_GW_NS_PERSO_CODE                       (0x37)
#define UIMI_TLV_TYPE_GW_SP_PERSO_CODE                       (0x38)
#define UIMI_TLV_TYPE_GW_CP_PERSO_CODE                       (0x39)
#define UIMI_TLV_TYPE_GW_SIM_PERSO_CODE                      (0x3A)
#define UIMI_TLV_TYPE_1X_NW_TYPE1_PERSO_CODE                 (0x3B)
#define UIMI_TLV_TYPE_1X_NW_TYPE2_PERSO_CODE                 (0x3C)
#define UIMI_TLV_TYPE_1X_RUIM_PERSO_CODE                     (0x3D)
#define UIMI_TLV_TYPE_PERSO_ITERATION_CNT                    (0x3E)
#define UIMI_TLV_TYPE_PERSO_UNLOCK_RETRIES_CNT               (0x3F)
#define UIMI_TLV_TYPE_DECRYPTED_PERSO_SECURE                 (0x40)
#define UIMI_TLV_TYPE_DECRYPTED_DEPERSO_SECURE               (0x41)
#define UIMI_TLV_TYPE_ENCRYPTED_EXT                          (0x42)
#define UIMI_TLV_TYPE_DECRYPTED_EMERGENCY_ONLY               (0x43)
#define UIMI_TLV_TYPE_DECRYPTED_SIMLOCK_CONFIG               (0x44)
#define UIMI_TLV_TYPE_PROFILE_ID                             (0x45)
#define UIMI_TLV_TYPE_SEARCH_PATTERN                         (0x46)
#define UIMI_TLV_TYPE_SEARCH_DATA                            (0x47)
#define UIMI_TLV_TYPE_NAF_ID                                 (0x48)
#define UIMI_TLV_TYPE_FORCE_BOOTSTRAPPING                    (0x49)
#define UIMI_TLV_TYPE_PERSO_REACTIVATE_FEATURE               (0x4A)
#define UIMI_TLV_TYPE_ACCESS_POINT_NAME                      (0x4B)
#define UIMI_TLV_TYPE_REMOTE_UNLOCK_CONFIG                   (0x4C)
#define UIMI_TLV_TYPE_IMSI                                   (0x4D)
#define UIMI_TLV_TYPE_PERSO_REUSE_CODE_DATA                  (0x4E)
#define UIMI_TLV_TYPE_GW_SP_EXT_PERSO_CODE                   (0x4F)
#define UIMI_TLV_TYPE_GW_CP_EXT_PERSO_CODE                   (0x50)
#define UIMI_TLV_TYPE_BSF_INFO                               (0x51)
#define UIMI_TLV_TYPE_GBA_KEY_STATUS_IND_REG                 (0x52)
#define UIMI_TLV_TYPE_TEMPORARY_IDENTITY_MASK                (0x53)
#define UIMI_TLV_TYPE_SWITCH_SLOT_EXT                        (0x54)
#define UIMI_TLV_TYPE_EUICC_PROFILE_NAME                     (0x55)
#define UIMI_TLV_TYPE_SET_UNSET_PROFILE                      (0x56)
#define UIMI_TLV_TYPE_EUICC_DOWNLOAD_CODE                    (0x57)
#define UIMI_TLV_TYPE_EUICC_CONFIRMATION_CODE                (0x58)
#define UIMI_TLV_TYPE_TERMINAL_CAPABILITY                    (0x59)
#define UIMI_TLV_TYPE_GW_SPN_PERSO_CODE                      (0x5A)
#define UIMI_TLV_TYPE_APDU_BEHAVIOR_FLAG                     (0x5B)
#define UIMI_TLV_TYPE_GW_SP_EXT2_PERSO_CODE                  (0x5C)
#define UIMI_TLV_TYPE_GW_CP_EXT2_PERSO_CODE                  (0x5D)
#define UIMI_TLV_TYPE_GW_SP_EHPLMN_PERSO_CODE                (0x5E)
#define UIMI_TLV_TYPE_GW_ICCID_PERSO_CODE                    (0x5F)
#define UIMI_TLV_TYPE_EUICC_RESET_OPTION_MASK                (0x60)
#define UIMI_TLV_TYPE_PROFILE_USER_CONSENT                   (0x61)
#define UIMI_TLV_TYPE_USER_CONSENT_SUPPORTED                 (0x62)
#define UIMI_TLV_TYPE_LONG_APDU_REQUEST                      (0x63)
#define UIMI_TLV_TYPE_EUICC_DEFAULT_ADDRESS                  (0x64)
#define UIMI_TLV_TYPE_CARD_POWERUP_MODE                      (0x65)
#define UIMI_TLV_TYPE_REMOTE_UNLOCK_OPERATION                (0x66)
#define UIMI_TLV_TYPE_REMOTE_UNLOCK_CONFIG_EXT               (0x68)
#define UIMI_TLV_TYPE_CONFIG_ID                              (0x6A)

/*---------------------------------------------------------------------------
  Response TLVs tags
---------------------------------------------------------------------------*/
#define UIMI_TLV_TAG_RESULT_CODE                 (0x02)
#define UIMI_TLV_RESP_TAG_OPTIONAL_1             (0x10)
#define UIMI_TLV_RESP_TAG_OPTIONAL_2             (0x11)
#define UIMI_TLV_RESP_TAG_OPTIONAL_3             (0x12)
#define UIMI_TLV_RESP_TAG_OPTIONAL_4             (0x13)
#define UIMI_TLV_RESP_TAG_OPTIONAL_5             (0x14)
#define UIMI_TLV_RESP_TAG_OPTIONAL_6             (0x15)
#define UIMI_TLV_RESP_TAG_OPTIONAL_7             (0x16)
#define UIMI_TLV_RESP_TAG_OPTIONAL_8             (0x17)
#define UIMI_TLV_RESP_TAG_OPTIONAL_9             (0x18)
#define UIMI_TLV_RESP_TAG_OPTIONAL_10            (0x19)
#define UIMI_TLV_RESP_TAG_OPTIONAL_11            (0x1A)
#define UIMI_TLV_RESP_TAG_OPTIONAL_12            (0x1B)
#define UIMI_TLV_RESP_TAG_OPTIONAL_13            (0x1C)

/*---------------------------------------------------------------------------
  Indication TLVs tags
---------------------------------------------------------------------------*/
#define UIMI_TLV_IND_TAG_MANDATORY_1             (0x01)
#define UIMI_TLV_IND_TAG_MANDATORY_2             (0x02)
#define UIMI_TLV_IND_TAG_MANDATORY_3             (0x03)
#define UIMI_TLV_IND_TAG_OPTIONAL_1              (0x10)
#define UIMI_TLV_IND_TAG_OPTIONAL_2              (0x11)
#define UIMI_TLV_IND_TAG_OPTIONAL_3              (0x12)

/*---------------------------------------------------------------------------
  Maximum size of fields in TLV structures
---------------------------------------------------------------------------*/
#define QMI_UIM_TLV_MAX_ENCRYPTED_PIN1_LEN       (255)

/*---------------------------------------------------------------------------
  Size of fields in TLV structures
---------------------------------------------------------------------------*/
#define QMI_UIM_TLV_CHANNEL_ID_LEN               ( 1)

/*---------------------------------------------------------------------------
  Number of provisioning sessions of each type (GW/1x)
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_PROV_SESSIONS                ( 3)

/*---------------------------------------------------------------------------
  Number of card sessions
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_CARD_SESSIONS                ( 3)

/*---------------------------------------------------------------------------
  Number of cards and applications
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_CARD_COUNT                   ( 3)
#define QMI_UIM_MAX_APP_COUNT                    (60)
#define QMI_UIM_MAX_APPS_PER_CARD                (20)
#define QMI_UIM_LEGACY_MAX_APP_COUNT             (24)
#define QMI_UIM_LEGACY_MAX_CARD_COUNT            ( 2)
#define QMI_UIM_LEGACY_MAX_APPS_PER_CARD         ( 8)

/*---------------------------------------------------------------------------
  Maximum number of retries
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_RETRIES                      (0xFF)

/*---------------------------------------------------------------------------
  Generic data magnitudes
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_IMSI_LEN                     (10)

/*---------------------------------------------------------------------------
  Maximum number of perso features
  This should be less than or equal to QMI_UIM_PERSO_FEATURE_MAX_V01 (20)
---------------------------------------------------------------------------*/
#define QMI_UIM_MAX_NUMBER_PERSO_FEATURES        15

/*---------------------------------------------------------------------------
  RFU size of encrypted message
---------------------------------------------------------------------------*/
#define QMI_UIM_ENCRYPTED_MSG_RFU_SIZE           (4)

/*---------------------------------------------------------------------------
  Maximum length of encrypted response
---------------------------------------------------------------------------*/
#define QMI_UIM_ENCRYPTED_RESP_MAX               (1024)

/*=============================================================================

                           STRUCTURE DECLARATIONS

=============================================================================*/

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_FILES_LIST_TYPE

   DESCRIPTION:        Linked list with list of files
     file:             File
     next_ptr:         Pointer to next node
-------------------------------------------------------------------------------*/
typedef struct files_list
{
  mmgsdi_access_type                   file;
  struct files_list *                  next_ptr;
} qmi_uim_files_list_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_BOOLEAN_TYPE

   DESCRIPTION:        Data extracted from boolean TLV
     value:            Boolean value
-------------------------------------------------------------------------------*/
typedef struct
{
  boolean                              value;
} qmi_uim_tlv_boolean_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_UINT32_TYPE

   DESCRIPTION:        Data extracted from uint32 TLV
     value:            uint32 value
-------------------------------------------------------------------------------*/
typedef struct
{
  uint32                              value;
} qmi_uim_tlv_uint32_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_UINT8_TYPE

   DESCRIPTION:        Data extracted from uint8 TLV
     value:            uint8 value
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8                              value;
} qmi_uim_tlv_uint8_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SESSION_INFORMATION_TYPE

   DESCRIPTION:        Data extracted from session information TLV
     session_type:     Session type
     aid:              AID value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_session_type_enum_type          session_type;
  mmgsdi_static_data_type                aid;
} qmi_uim_tlv_session_information_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_FILE_ID_TYPE

   DESCRIPTION:        Data extracted from file id TLV
     file_access:      Path and file id
     file_id    :      file id 
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_access_type                     file_access;
  uint16                                 file_id;
} qmi_uim_tlv_file_id_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_READ_TRANSPARENT_TYPE

   DESCRIPTION:        Data extracted from read transparent TLV
     offset:           Offset value
     length:           Length value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_offset_type                     offset;
  mmgsdi_len_type                        length;
} qmi_uim_tlv_read_transparent_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_READ_RECORD_TYPE

   DESCRIPTION:        Data extracted from read record TLV
     record:           Record value
     length:           Length value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_rec_num_type                    record;
  mmgsdi_len_type                        length;
} qmi_uim_tlv_read_record_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_WRITE_TRANSPARENT_TYPE

   DESCRIPTION:        Data extracted from write transparent TLV
     offset:           Offset value
     data:             Data to be written
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_offset_type                     offset;
  mmgsdi_write_data_type                 data;
} qmi_uim_tlv_write_transparent_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_WRITE_RECORD_TYPE

   DESCRIPTION:        Data extracted from write record TLV
     record:           Record value
     data:             Data to be written
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_rec_num_type                    record;
  mmgsdi_write_data_type                 data;
} qmi_uim_tlv_write_record_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_INCREASE_TYPE

   DESCRIPTION:        Data extracted from write transparent TLV
     offset:           Offset value
     data:             Data to be written
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_increase_data_type                 data;
} qmi_uim_tlv_increase_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SET_PIN_PROTECTION_TYPE

   DESCRIPTION:        Data extracted from set PIN protection TLV
     pin_id:           Pin id
     pin_enable:       Indicates if PIN is enabled or disabled
     pin_data:         PIN value
     pin_value:        Buffer with PIN value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_pin_enum_type                   pin_id;
  boolean                                pin_enable;
  mmgsdi_data_type                       pin_data;
  uint8                                  pin_value[QMI_UIM_PIN_MAX_V01];
} qmi_uim_tlv_set_pin_protection_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_VERIFY_PIN_TYPE

   DESCRIPTION:        Data extracted from verify PIN TLV
     pin_id:           Pin id
     pin_data:         PIN value
     pin_value:        Buffer with PIN value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_pin_enum_type                   pin_id;
  mmgsdi_data_type                       pin_data;
  uint8                                  pin_value[QMI_UIM_PIN_MAX_V01];
} qmi_uim_tlv_verify_pin_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_UNBLOCK_PIN_TYPE

   DESCRIPTION:        Data extracted from unblock PIN TLV
     pin_id:           Pin id
     pin_data:         PUK value
     pin_data:         PIN value
     puk_value:        Buffer with PUK value
     pin_value:        Buffer with PIN value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_pin_enum_type                   pin_id;
  mmgsdi_data_type                       puk_data;
  mmgsdi_data_type                       pin_data;
  uint8                                  puk_value[QMI_UIM_PIN_MAX_V01];
  uint8                                  pin_value[QMI_UIM_PIN_MAX_V01];
} qmi_uim_tlv_unblock_pin_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_CHANGE_PIN_TYPE

   DESCRIPTION:        Data extracted from change PIN TLV
     pin_id:           Pin id
     old_pin_data:     Old PIN value
     new_pin_data:     New PIN value
     old_pin_value:    Buffer with old PIN value
     new_pin_value:    Buffer with new PIN value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_pin_enum_type                   pin_id;
  mmgsdi_data_type                       old_pin_data;
  mmgsdi_data_type                       new_pin_data;
  uint8                                  old_pin_value[QMI_UIM_PIN_MAX_V01];
  uint8                                  new_pin_value[QMI_UIM_PIN_MAX_V01];
} qmi_uim_tlv_change_pin_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_DEPERSONALIZATION_TYPE

   DESCRIPTION:        Data extracted from deactivate perso TLV
     category:         Category to deactivate
     ck:               Control key value
-------------------------------------------------------------------------------*/
typedef struct {
  simlock_category_enum_type             category;
  simlock_ck_data_type                   ck;
} qmi_uim_tlv_depersonalization_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_REGISTER_REFRESH_TYPE

   DESCRIPTION:        Data extracted from register for refresh TLV
     register_op:      Register operation
     vote_for_init:    Indicates if client votes for init
     file_list_ptr:    List of files to register for refresh
-------------------------------------------------------------------------------*/
typedef struct {
  boolean                                register_op;
  boolean                                vote_for_init;
  qmi_uim_files_list_type              * file_list_ptr;
} qmi_uim_tlv_register_refresh_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SLOT_TYPE

   DESCRIPTION:        Data extracted from slot TLV
     slot:             Slot id
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_slot_id_enum_type               slot;
} qmi_uim_tlv_slot_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_CARD_MODE_TYPE

   DESCRIPTION:        Data extracted from card mode TLV
     card_mode:        card mode
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_card_pup_options_enum_type      card_pup_value;
} qmi_uim_tlv_card_mode_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_PHYSICAL_SLOT_TYPE

   DESCRIPTION:        Data extracted from physical slot TLV
     slot:             Physical slot id
-------------------------------------------------------------------------------*/
typedef struct {
  uim_slot_type                          slot;
} qmi_uim_tlv_physical_slot_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_EVENT_REG_TYPE

   DESCRIPTION:        Data extracted from event reg TLV
     card_status:                      Registration to legacy card status events
     sap_events:                       Registration to SAP events
     extended_card_status:             Registration to extended card status events
     provisioning_session_close_event: Registration to prov session closed event
     physical_slots_status:            Registration to physical slots status events
     sim_busy_events :                 Registration to sim busy events
     reduced_card_status_events        Registration to reduced card status events
     recovery_complete_event           Registration to events when recovery completes
     supply_voltage_events:            Registration to supply voltage events
     activation_status_event:          Registration to events for UICC activation status
     simlock_remote_sfs_events:        Registration to simlock remote SFS events
     simlock_temp_unlock_status:       Registration to simlock temp unlock status

-------------------------------------------------------------------------------*/
typedef struct {
  boolean                                card_status;
  boolean                                sap_events;
  boolean                                extended_card_status;
  boolean                                provisioning_session_close_event;
  boolean                                physical_slots_status;
  boolean                                sim_busy_events;
  boolean                                reduced_card_status_events;
  boolean                                recovery_complete_event;
  boolean                                supply_voltage_events;
  boolean                                activation_status_event;
  boolean                                simlock_remote_sfs_events;
  boolean                                simlock_temp_unlock_status;
} qmi_uim_tlv_event_reg_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_AUTHENTICATE_TYPE

   DESCRIPTION:        Data extracted from event reg TLV
     slot:             Slot id
-------------------------------------------------------------------------------*/
typedef struct {
  uim_auth_context_enum_v01              auth_context;
  mmgsdi_data_type                       auth_data;
} qmi_uim_tlv_authenticate_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_CAPABILITIES_MASK_TYPE

   DESCRIPTION:        Data extracted from capabilities mask TLV
     fdn_status:        FDN status
     hidden_key_status: Hidden Key pin status
     index_in_dir:      Index of the application in the EF_DIR file
     esn_me_status:     ESN status
     acl_status:        ACL status
     service_status:    Services status
-------------------------------------------------------------------------------*/
typedef struct {
  boolean                                fdn_status;
  boolean                                hidden_key_status;
  boolean                                index_in_dir;
  boolean                                esn_me_status;
  boolean                                acl_status;
  boolean                                service_status;
} qmi_uim_tlv_capabilities_mask_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_APP_INFORMATION_TYPE

   DESCRIPTION:        Data extracted from application information TLV
     slot:             slot id
     aid:              AID value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_slot_id_enum_type               slot;
  mmgsdi_static_data_type                aid;
} qmi_uim_tlv_app_information_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SESSION_CHANGE_TYPE

   DESCRIPTION:        Data extracted from session change TLV
     session_type:     session type
     activate:         activate or deactivate session
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_session_type_enum_type          session_type;
  boolean                                activate;
} qmi_uim_tlv_session_change_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_APDU_TYPE

   DESCRIPTION:        Data extracted from APDU request TLV
     apdu_data:        APDU data to be written
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_send_apdu_data_type             apdu_data;
} qmi_uim_tlv_apdu_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SAP_CONNECT_TYPE

   DESCRIPTION:        Data extracted from SAP connect TLV
     connect:          Connection operation to be performed
     slot:             slot id
-------------------------------------------------------------------------------*/
typedef struct {
  uim_sap_connect_op_enum_v01            connect;
  mmgsdi_slot_id_enum_type               slot;
} qmi_uim_tlv_sap_connect_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SAP_DISCONNECT_MODE_TYPE

   DESCRIPTION:        Data extracted from SAP disconnect mode TLV
     mode:             Disconnect mode
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_disconnect_mode_enum_type       mode;
} qmi_uim_tlv_sap_disconnect_mode_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SAP_REQUEST_TYPE

   DESCRIPTION:        Data extracted from SAP request TLV
     request:          Request operation to be performed
     slot:             slot id
-------------------------------------------------------------------------------*/
typedef struct {
  uim_sap_request_enum_v01               request;
  mmgsdi_slot_id_enum_type               slot;
} qmi_uim_tlv_sap_request_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_LOGICAL_CHANNEL_TYPE

   DESCRIPTION:        Data extracted from logical channel TLV
     channel_id:       Logical channel
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                                  logical_channel;
} qmi_uim_tlv_logical_channel_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_AID_TYPE

   DESCRIPTION:        Data extracted from AID TLV
     aid:              AID value
     channel_id:       channel id
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_static_data_type                aid;
} qmi_uim_tlv_aid_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_READ_RECORD_LAST_TYPE

   DESCRIPTION:        Data extracted from read record last TLV
     last_record:      Last record value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_rec_num_type                    last_record;
} qmi_uim_tlv_read_record_last_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_FILE_CONTROL_INFO_TYPE

   DESCRIPTION:         Data extracted from file control info TLV
      fci_type:         Select response enumeration indicating the file control
                        info type requested
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_select_rsp_enum_type            fci_type;
} qmi_uim_tlv_file_control_info_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_DECRYPTED_PIN_TYPE

   DESCRIPTION:        Data decoded from the encrypted PIN TLV
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_data_type                       decrypted_pin_data;
  uint8                                  decrypted_pin_value[QMI_UIM_PIN_MAX_V01];
} qmi_uim_tlv_decrypted_pin_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_IMSI_TYPE

   DESCRIPTION:        Structure containing IMSI data
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8                                  len;
  uint8                                  imsi[QMI_UIM_MAX_IMSI_LEN];
} qmi_uim_imsi_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_DECRYPTED_IMSI_TYPE

   DESCRIPTION:        Data decoded from the encrypted IMSI TLV
-------------------------------------------------------------------------------*/
typedef struct {
  qmi_uim_imsi_type                     imsi_data;
} qmi_uim_tlv_decrypted_imsi_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      QMI_UIM_SECURE_HEADER_TYPE

   DESCRIPTION:
     Contains request information corresponding to the command.

-----------------------------------------------------------------------------*/
typedef struct
{
  uint16 cmd_id;
  uint16 request_id;
} qmi_uim_secure_header_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_DECRYPTED_PERSO_SECURE_TYPE

   DESCRIPTION:        Data decoded from the encrypted perso TLV
-------------------------------------------------------------------------------*/
typedef struct {
  qmi_uim_secure_header_type            header;
  simlock_slot_enum_type                slot;
  uint8                                 num_of_lists;
  simlock_category_data_type          * code_ptr;
} qmi_uim_tlv_decrypted_perso_secure_type;

/* -----------------------------------------------------------------------------
   ENUM:      QMI_UIM_DEPERSO_SECURE_OPERATION_TYPE

   DESCRIPTION:
     Indicates the operation type of the deperso secure command
-------------------------------------------------------------------------------*/
typedef enum
{
  QMI_UIM_DEPERSO_UNLOCK_WITHOUT_CK     = 0,
  QMI_UIM_DEPERSO_TEMP_UNLOCK_ENABLE    = 1,
  QMI_UIM_DEPERSO_TEMP_UNLOCK_DISABLE   = 2
} qmi_uim_deperso_secure_operation_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_DECRYPTED_DEPERSO_SECURE_TYPE

   DESCRIPTION:        Data decoded from the encrypted deperso TLV
-------------------------------------------------------------------------------*/
typedef struct {
  qmi_uim_secure_header_type             header;
  simlock_slot_enum_type                 slot;
  simlock_category_enum_type             category_type;
  qmi_uim_deperso_secure_operation_type  operation;
  uint32                                 duration;
} qmi_uim_tlv_decrypted_deperso_secure_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_PIN_KEYREF_ID_TYPE

   DESCRIPTION:        Data extracted from PIN Key reference ID TLV
     keyref_id:        PIN Key reference ID value
-------------------------------------------------------------------------------*/
typedef struct {
  uim_pin_key_ref_enum_v01     keyref_id;
} qmi_uim_tlv_pin_keyref_id_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_GET_CONFIGURATION_MASK_TYPE

   DESCRIPTION:                   Data extracted from config mask TLV
     automatic_selection        : Indicates if user wants automatic selection status
     personalization_status     : Indicates if user wants personalization status
     halt_subscription_status   : Indicates if user wants halt subscription status
     usb_uicc_supported_status  : Indicates if user wants usb uicc supported status
     sap_client_supported_status: Indicates if user wants sap client supported status
     perso_reactivation_status  : Indicates if user wants perso reactivation status
     remote_simlock_storage     : Indicates if user wants to know about config storage
     emergency_only_mode        : Indicates if user wants emergency only mode status
     inactive_simlock_config    : Indicates if user wants inactive simlock configuration
-------------------------------------------------------------------------------*/
typedef struct {
  boolean                                 automatic_selection;
  boolean                                 personalization_status;
  boolean                                 halt_subscription_status;
  boolean                                 usb_uicc_supported_status;
  boolean                                 sap_client_supported_status;
  boolean                                 perso_reactivation_status;
  boolean                                 remote_simlock_storage;
  boolean                                 emergency_only_mode;
  boolean                                 extended_apdu;
  boolean                                 inactive_simlock_config;
} qmi_uim_tlv_get_configuration_mask_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SAP_CONN_CONDITION_TYPE

   DESCRIPTION:        Data extracted from SAP connection condition TLV
     condition:        Connection condition value
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_sap_conn_condition_enum_type    condition;
} qmi_uim_tlv_sap_conn_condition_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_FILE_STATUS_TYPE

   DESCRIPTION:        Data extracted from file status TLV
     file_status:      File status (activate or deactivate)
-------------------------------------------------------------------------------*/
typedef struct {
  uim_file_status_type_v01 file_status;
} qmi_uim_tlv_file_status_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SELECT_MODE_TYPE

   DESCRIPTION:        Data extracted from select mode TLV
     select_mode:      Mode for selecting AID
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_select_mode_enum_type               mode;
} qmi_uim_tlv_select_mode_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_STATUS_CMD_MODE_TYPE

   DESCRIPTION:        Data extracted from status command mode TLV.
     -------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_status_me_app_enum_type mode;
} qmi_uim_tlv_status_cmd_mode_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_STATUS_CMD_RESP_TYPE

   DESCRIPTION:        Data extracted from status command response TLV.
     -------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_status_ret_data_enum_type resp;
} qmi_uim_tlv_status_cmd_resp_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SERVICE_NUMBER_TYPE

   DESCRIPTION:        Data extracted from service number TLV
   service_number:     Service number per the spec
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                                  service_number;
} qmi_uim_tlv_service_number_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_PERSONALIZATION_CK_TYPE

   DESCRIPTION:        CK data extracted from activate perso TLV
     ck:               Control key value
-------------------------------------------------------------------------------*/
typedef struct {
  simlock_ck_data_type         ck;
} qmi_uim_tlv_personalization_ck_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_PERSONALIZATION_CODE_TYPE

   DESCRIPTION:            Lock code data extracted from activate perso TLV
     num_of_list:          Each list contains SIMLOCK_NUM_CODES_MAX (20) codes.
                           The num_of_lists indicate count of such lists.
     code_ptr:             Pointer to lock codes
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                          num_of_lists;
  simlock_category_data_type   * code_ptr;
} qmi_uim_tlv_personalization_code_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_PERSONALIZATION_CATEGORY_TYPE

   DESCRIPTION:            Perso feature extracted from reactivate or reuse
                           code feature TLV
     category:             Category to reactivate or reuse code
-------------------------------------------------------------------------------*/
typedef struct {
  simlock_category_enum_type             category;
} qmi_uim_tlv_personalization_category_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_DECRYPTED_EMERGENCY_ONLY_TYPE

   DESCRIPTION:        Data decoded from the encrypted emergency only TLV
-------------------------------------------------------------------------------*/
typedef struct {
  qmi_uim_secure_header_type             header;
  simlock_device_mode_enum_type          device_mode;
} qmi_uim_tlv_decrypted_emergency_only_type;

/* -----------------------------------------------------------------------------
   ENUM:      QMI_UIM_SIMLOCK_CONFIG_OPERATION_TYPE

   DESCRIPTION:
     Type of simlock configuration operation
-------------------------------------------------------------------------------*/
typedef enum
{
  QMI_UIM_SIMLOCK_CONFIG_OPERATION_READ                = 0,
  QMI_UIM_SIMLOCK_CONFIG_OPERATION_WRITE               = 1,
  QMI_UIM_SIMLOCK_CONFIG_OPERATION_ENABLE_REMOTE_SFS   = 2,
  QMI_UIM_SIMLOCK_CONFIG_OPERATION_DISABLE_REMOTE_SFS  = 3
} qmi_uim_simlock_config_operation_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_DECRYPTED_SIMLOCK_CONFIG_TYPE

   DESCRIPTION:        Data decoded from the encrypted simlock configuration TLV
-------------------------------------------------------------------------------*/
typedef struct {
  qmi_uim_secure_header_type                   header;
  qmi_uim_simlock_config_operation_type        operation;
  union {
    simlock_remote_sfs_request_enum_type       remote_sfs_operation;
    struct {
      simlock_result_enum_type                 result;
      uint8                                    file_id;
      simlock_data_type                        data;
    }                                          sfs_read_operation;
    struct {
      simlock_result_enum_type                 result;
      uint8                                    file_id;
    }                                          sfs_write_operation;
  }operation_type;
} qmi_uim_tlv_decrypted_simlock_config_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SIM_PROFILE_TYPE

   DESCRIPTION:         Data extracted from SIM Profile TLV
   profile_id           Indicates the profile id to activate
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_profile_id_enum_type            profile_id;
} qmi_uim_tlv_sim_profile_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SEARCH_PATTERN_TYPE

   DESCRIPTION:         Data extracted from Search Record TLV
   search_pattern:      Pattern to be searched. Cannot be NULL.
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_search_data_type           pattern;
} qmi_uim_tlv_search_pattern_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SEARCH_DATA_TYPE

   DESCRIPTION:         Data extracted from Search Data TLV
   search_type:         Direction in which SEARCH needs to perform
   first_record_num:    Record Number, from where search will
                        start in file.
                        Valid Range:
                        0x00: Current Record
                        0x01-0xFE: Physical record Number
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_search_direction_enum_type direction;
  mmgsdi_rec_num_type               first_record_num;
} qmi_uim_tlv_search_data_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:   QMI_UIM_TLV_GBA_SESSION_INFO_TYPE

   DESCRIPTION: Data decoded from session information TLV
-------------------------------------------------------------------------------*/
typedef struct {
  gba_session_type    session_type;
  gba_aid_type        aid;
} qmi_uim_tlv_gba_session_info_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_NAF_ID_TYPE

   DESCRIPTION:  Data decoded from the NAF ID TLV
-------------------------------------------------------------------------------*/
typedef struct {
  gba_naf_id_type *naf_id_ptr;
} qmi_uim_tlv_naf_id_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_APN_TYPE

   DESCRIPTION:  Data decoded from the APN TLV
-------------------------------------------------------------------------------*/
typedef struct {
  char      *apn_data_ptr;
} qmi_uim_tlv_apn_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_REMOTE_UNLOCK_CONFIG_TYPE

   DESCRIPTION:  Data from the remote unlock request
-------------------------------------------------------------------------------*/
typedef struct {
  simlock_data_type                          config_data;
} qmi_uim_tlv_remote_unlock_config_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_REMOTE_UNLOCK_OPERATION_TYPE

   DESCRIPTION:  Remote simlock operation type
-------------------------------------------------------------------------------*/
typedef struct {
  uim_remote_unlock_operation_type_enum_v01  operation;
} qmi_uim_tlv_remote_unlock_operation_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_IMSI_TYPE

   DESCRIPTION:  IMSI Data from the verify IMSI request
-------------------------------------------------------------------------------*/
typedef struct {
  simlock_imsi_type       imsi_data;
} qmi_uim_tlv_imsi_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_BSF_INFO_TYPE

   DESCRIPTION:  Address of the BSF server
-------------------------------------------------------------------------------*/
typedef struct {
  gba_bsf_info_type     * bsf_ptr;
} qmi_uim_tlv_bsf_info_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_TEMPORARY_IDENTITY_MASK_TYPE

   DESCRIPTION:        Data extracted from temporary identity mask TLV
     guti:        Purge guti.
-------------------------------------------------------------------------------*/
typedef struct {
  boolean                 guti;
} qmi_uim_tlv_temporary_identity_mask_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_SWITCH_SLOT_EXT_TYPE

   DESCRIPTION:  Physical to logical slot mapping data
-------------------------------------------------------------------------------*/
typedef struct {
  uint8            logical_slot_len;
  uim_slot_type    logical_slot[QMI_UIM_MAX_CARD_COUNT];
} qmi_uim_tlv_switch_slot_ext_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_EUICC_NAME_TYPE

   DESCRIPTION:        Data extracted from name type
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_name_type                 euicc_name_data;
} qmi_uim_tlv_euicc_name_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_DOWNLOAD_CODE_TYPE

   DESCRIPTION:        Data extracted from add profile code
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_data_type                 code;
} qmi_uim_tlv_download_code_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TERMINAL_CAPABILITY_TLV_TYPE

   DESCRIPTION:        Terminal capability tlv
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                  tag;
  uint8                  value_len;
  uint8                  value[QMI_UIM_TERMINAL_CAPABILITY_VALUE_MAX_V01];
  boolean                remove_tlv;
} qmi_uim_terminal_capability_tlv_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_TERMINAL_CAPABILITY_TLV_TYPE

   DESCRIPTION:        Terminal capability tlv contents
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                                num_tlvs;
  qmi_uim_terminal_capability_tlv_type *tlv_ptr;
} qmi_uim_tlv_terminal_capability_tlv_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_APDU_BEHAVIOR_TYPE

   DESCRIPTION:        Data extracted from APDU behavior TLV
     apdu_behavior : APDU behavior
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_apdu_behavior_type            apdu_behavior;
} qmi_uim_tlv_apdu_behavior_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_RESET_OPTION_MASK_TYPE
      DESCRIPTION:                   Data extracted from reset option mask TLV
      reset_test_profiles                   : Delete all the test profiles
      reset_operational_profiles            : Delete all the operational profiles
      reset_smdp_address                    : Reset default SMDP address
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_card_memory_reset_option_type  euicc_reset_option;
} qmi_uim_tlv_reset_option_mask_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_LONG_APDU_REQUEST_TYPE

   DESCRIPTION:        Data extracted from long_request TLV
     total_length   : Total length of APDU
     token          : Token for session
     offset         : offset of current chunk, starting from 0
-------------------------------------------------------------------------------*/
typedef struct {
  uint16        total_length;
  uint32        token;
  uint16        offset;
} qmi_uim_tlv_long_apdu_request_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_EUICC_DEFAULT_ADDRESS_TYPE

   DESCRIPTION:        Data extracted from setting default profile
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_data_type                   smdp_address;
} qmi_uim_tlv_euicc_default_address_type;

/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_TLV_ITEM_TYPE

   DESCRIPTION:        Item containing description of each item in the TLV
     tag:              Tag of the item
     mandatory:        Indicates if mandatory or optional.
     present:          Indicates if present
     data:             Data
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                                             tag;
  uint8                                             type;
  boolean                                           present;
  union {
    qmi_uim_tlv_session_information_type            session_information;
    qmi_uim_tlv_file_id_type                        file_id;
    qmi_uim_tlv_read_transparent_type               read_transparent;
    qmi_uim_tlv_read_record_type                    read_record;
    qmi_uim_tlv_write_transparent_type              write_transparent;
    qmi_uim_tlv_write_record_type                   write_record;
    qmi_uim_tlv_set_pin_protection_type             set_pin_protection;
    qmi_uim_tlv_verify_pin_type                     verify_pin;
    qmi_uim_tlv_unblock_pin_type                    unblock_pin;
    qmi_uim_tlv_change_pin_type                     change_pin;
    qmi_uim_tlv_depersonalization_type              depersonalization;
    qmi_uim_tlv_register_refresh_type               register_refresh;
    qmi_uim_tlv_boolean_type                        ok_to_refresh;
    qmi_uim_tlv_boolean_type                        refresh_complete;
    qmi_uim_tlv_slot_type                           slot;
    qmi_uim_tlv_event_reg_type                      event_reg;
    qmi_uim_tlv_authenticate_type                   authenticate;
    qmi_uim_tlv_capabilities_mask_type              capabilities_mask;
    qmi_uim_tlv_boolean_type                        set_fdn_status;
    qmi_uim_tlv_app_information_type                application_information;
    qmi_uim_tlv_session_change_type                 session_change;
    qmi_uim_tlv_apdu_type                           send_apdu;
    qmi_uim_tlv_sap_connect_type                    sap_connection;
    qmi_uim_tlv_sap_disconnect_mode_type            sap_disconnect_mode;
    qmi_uim_tlv_sap_request_type                    sap_request;
    qmi_uim_tlv_logical_channel_type                logical_channel;
    qmi_uim_tlv_aid_type                            aid;
    qmi_uim_tlv_decrypted_pin_type                  decrypted_pin;
    qmi_uim_tlv_read_record_last_type               read_record_last;
    qmi_uim_tlv_boolean_type                        ok_for_subscription;
    qmi_uim_tlv_file_control_info_type              file_control_info;
    qmi_uim_tlv_pin_keyref_id_type                  pin_keyref_id;
    qmi_uim_tlv_uint32_type                         ind_token;
    qmi_uim_tlv_boolean_type                        ignore_hotswap_switch;
    qmi_uim_tlv_boolean_type                        sap_intermediate_get_resp;
    qmi_uim_tlv_boolean_type                        encrypted;
    qmi_uim_tlv_decrypted_imsi_type                 decrypted_imsi;
    qmi_uim_tlv_get_configuration_mask_type         configuration_mask;
    qmi_uim_tlv_boolean_type                        skip_procedure_bytes;
    qmi_uim_tlv_boolean_type                        terminate_app;
    qmi_uim_tlv_boolean_type                        refresh_registration;
    qmi_uim_tlv_boolean_type                        refresh_vote_for_init;
    qmi_uim_tlv_sap_conn_condition_type             sap_connection_condtion;
    qmi_uim_tlv_boolean_type                        set_acl_status;
    qmi_uim_tlv_file_status_type                    file_status;
    qmi_uim_tlv_boolean_type                        extended_card_status;
    qmi_uim_tlv_physical_slot_type                  physical_slot;
    qmi_uim_tlv_increase_type                       increase_data;
    qmi_uim_tlv_select_mode_type                    select_mode;
    qmi_uim_tlv_status_cmd_mode_type                status_cmd_mode;
    qmi_uim_tlv_status_cmd_resp_type                status_cmd_resp;
    qmi_uim_tlv_service_number_type                 service_number;
    qmi_uim_tlv_personalization_ck_type             personalization_ck;
    qmi_uim_tlv_personalization_code_type           personalization_code;
    qmi_uim_tlv_uint32_type                         personalization_iteration_cnt;
    qmi_uim_tlv_uint32_type                         personalization_retries_cnt;
    qmi_uim_tlv_personalization_category_type       personalization_reactivate;
    qmi_uim_tlv_decrypted_perso_secure_type         decrypted_perso_secure;
    qmi_uim_tlv_decrypted_deperso_secure_type       decrypted_deperso_secure;
    qmi_uim_tlv_boolean_type                        encrypted_ext;
    qmi_uim_tlv_decrypted_emergency_only_type       decrypted_emergency_only;
    qmi_uim_tlv_decrypted_simlock_config_type       decrypted_simlock_config;
    qmi_uim_tlv_sim_profile_type                    sim_profile;
    qmi_uim_tlv_search_pattern_type                 search_pattern;
    qmi_uim_tlv_search_data_type                    search_data;
    qmi_uim_tlv_naf_id_type                         gba_naf_id;
    qmi_uim_tlv_boolean_type                        gba_force_bootstrapping;
    qmi_uim_tlv_apn_type                            apn;
    qmi_uim_tlv_remote_unlock_config_type           remote_unlock_config;
    qmi_uim_tlv_remote_unlock_operation_type        remote_unlock_operation;
    qmi_uim_tlv_imsi_type                           imsi;
    qmi_uim_tlv_personalization_category_type       personalization_reuse_code;
    qmi_uim_tlv_bsf_info_type                       gba_bsf_info;
    qmi_uim_tlv_boolean_type                        gba_key_status_ind_reg;
    qmi_uim_tlv_temporary_identity_mask_type        temporary_identity_mask;
    qmi_uim_tlv_switch_slot_ext_type                switch_slot_ext;
    qmi_uim_tlv_euicc_name_type                     euicc_name;
    qmi_uim_tlv_boolean_type                        set_unset_profile;
    qmi_uim_tlv_download_code_type                  euicc_download_code;
    qmi_uim_tlv_terminal_capability_tlv_type        terminal_capability;
    qmi_uim_tlv_apdu_behavior_type                  apdu_behavior;
    qmi_uim_tlv_reset_option_mask_type              euicc_reset_option_mask;
    qmi_uim_tlv_boolean_type                        euicc_user_consent;
    qmi_uim_tlv_boolean_type                        euicc_user_consent_supported;
    qmi_uim_tlv_long_apdu_request_type              long_apdu_request;
    qmi_uim_tlv_euicc_default_address_type          euicc_default_address;
    qmi_uim_tlv_card_mode_type                      card_pup_option;
    qmi_uim_tlv_uint8_type                          config_id;
  } data;
} qmi_uim_tlv_item_type;

/* -----------------------------------------------------------------------------
   ENUM:      QMI_UIM_CARD_STATE_TYPE

   DESCRIPTION:
     Indication of card state
-------------------------------------------------------------------------------*/
typedef enum {
  QMI_UIM_CARD_STATE_ABSENT         = 0,
  QMI_UIM_CARD_STATE_PRESENT        = 1,
  QMI_UIM_CARD_STATE_ERROR          = 2
} qmi_uim_card_state_type;

/* -----------------------------------------------------------------------------
   ENUM:      QMI_UIM_APP_STATE_TYPE

   DESCRIPTION:
     Indication of application state
-------------------------------------------------------------------------------*/
typedef enum {
  QMI_UIM_APP_STATE_UNKNOWN         = 0,
  QMI_UIM_APP_STATE_DETECTED        = 1,
  QMI_UIM_APP_STATE_PIN_REQUIRED    = 2,
  QMI_UIM_APP_STATE_PUK1_REQUIRED   = 3,
  QMI_UIM_APP_STATE_PERSO           = 4,
  QMI_UIM_APP_STATE_BLOCKED         = 5,
  QMI_UIM_APP_STATE_ILLEGAL         = 6,
  QMI_UIM_APP_STATE_READY           = 7
} qmi_uim_app_state_type;

/* -----------------------------------------------------------------------------
   ENUM:      QMI_UIM_PERSO_STATE_TYPE

   DESCRIPTION:
     Indication of perso state
-------------------------------------------------------------------------------*/
typedef enum {
  QMI_UIM_PERSO_STATE_UNKNOWN       = 0,
  QMI_UIM_PERSO_STATE_IN_PROGRESS   = 1,
  QMI_UIM_PERSO_STATE_READY         = 2,
  QMI_UIM_PERSO_STATE_CODE_REQUIRED = 3,
  QMI_UIM_PERSO_STATE_PUK_REQUIRED  = 4,
  QMI_UIM_PERSO_STATE_PERM_BLOCKED  = 5
} qmi_uim_perso_state_type;

/* -----------------------------------------------------------------------------
   ENUM:      QMI_UIM_CARD_ERROR_TYPE

   DESCRIPTION:
     Indication of card error reason
-------------------------------------------------------------------------------*/
typedef enum {
  QMI_UIM_CARD_ERROR_UNKNOWN                     = 0,
  QMI_UIM_CARD_ERROR_POWER_DOWN                  = 1,
  QMI_UIM_CARD_ERROR_POLL_ERROR                  = 2,
  QMI_UIM_CARD_ERROR_NO_ATR_RECEIVED             = 3,
  QMI_UIM_CARD_ERROR_VOLT_MISMATCH               = 4,
  QMI_UIM_CARD_ERROR_PARITY_ERROR                = 5,
  QMI_UIM_CARD_ERROR_UNKNOWN_POSSIBLY_REMOVED    = 6,
  QMI_UIM_CARD_ERROR_CODE_SIM_TECHNICAL_PROBLEMS = 7,
  QMI_UIM_CARD_ERROR_NULL_BYTES                  = 8,
  QMI_UIM_CARD_ERROR_SAP_CONNECTED               = 9,
  QMI_UIM_CARD_ERROR_CMD_TIMEOUT                 = 10
} qmi_uim_card_error_type;

/* -----------------------------------------------------------------------------
   ENUM:    QMI_UIM_EXTENSION_CATEGORY_TYPE

   DESCRIPTION:        Item contains extension category type for locks
-------------------------------------------------------------------------------*/
typedef enum {
  QMI_UIM_CATEGORY_TYPE_NO_EXT,   /* No extension */
  QMI_UIM_CATEGORY_TYPE_EXT,      /* First extension, 5 bytes GID */
  QMI_UIM_CATEGORY_TYPE_EXT2      /* Second extension, 8 bytes GID */
} qmi_uim_simlock_extension_category_type;


/* -----------------------------------------------------------------------------
   STRUCTURE:      QMI_UIM_CARD_INFO_TYPE

   DESCRIPTION:
     Description of card info
-------------------------------------------------------------------------------*/
typedef struct {
  qmi_uim_card_state_type         card_state;
  mmgsdi_pin_status_enum_type     upin_state;
  uint8                           upin_num_retries;
  uint8                           upuk_num_retries;
  qmi_uim_card_error_type         card_error;
  boolean                         sim_busy;
  boolean                         sim_detection_started;
  boolean                         is_non_telecom_card;
} qmi_uim_card_info_type;


/* -----------------------------------------------------------------------------
   STRUCTURE:      QMI_UIM_APP_INFO_TYPE

   DESCRIPTION:
     Description of application info
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_app_enum_type            app_type;
  qmi_uim_app_state_type          app_state;
  qmi_uim_perso_state_type        perso_state;
  mmgsdi_perso_feature_enum_type  perso_feature;
  uint32                          perso_retries;
  mmgsdi_static_data_type         aid;
  mmgsdi_static_data_type         label;
  boolean                         upin;
  mmgsdi_pin_status_enum_type     pin1_state;
  uint8                           pin1_num_retries;
  uint8                           puk1_num_retries;
  mmgsdi_pin_status_enum_type     pin2_state;
  uint8                           pin2_num_retries;
  uint8                           puk2_num_retries;
  mmgsdi_slot_id_enum_type        slot;
} qmi_uim_app_info_type;


/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_CARD_STATE

   DESCRIPTION:        Item containing description of card state
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                             num_slots;
  qmi_uim_card_info_type          * card[QMI_UIM_MAX_CARD_COUNT];
  uint16                            index_gw[QMI_UIM_MAX_PROV_SESSIONS];
  uint16                            index_1x[QMI_UIM_MAX_PROV_SESSIONS];
  uint16                            num_apps;
  qmi_uim_app_info_type           * application[QMI_UIM_MAX_APP_COUNT];
} qmi_uim_card_state;


/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_ADDITIONAL_RECORDS_TYPE

   DESCRIPTION:        Item containing response for additional records read
-------------------------------------------------------------------------------*/
typedef struct {
  mmgsdi_return_enum_type   mmgsdi_status;
  mmgsdi_sw_type            status_word;
  mmgsdi_data_type          read_data;
} qmi_uim_additional_records_type;


/* -----------------------------------------------------------------------------
   STRUCTURE:    QMI_UIM_REGISTRATION_EVENT_TYPE

   DESCRIPTION:        Structure used to hold the registration status
     card_status:                      Registration to legacy card status events
     sap_events:                       Registration to SAP events
     extended_card_status:             Registration to extended card status events
     provisioning_session_close_event: Registration to prov session closed event
     physical_slots_status:            Registration to physical slots status events
     sim_busy_events:                  Registration to sim busy events
     reduced_card_status_events:       Registration to reduced card status events
     recovery_complete_event:          Registration to recovery complete event
     supply_voltage_events:            Registration to supply voltage events
     reduced_card_status_events        Registration to reduced card status events
     simlock_remote_sfs_events:        Registration to simlock remote SFS events
     simlock_temp_unlock_status:       Registration to simlock temp unlock status
-------------------------------------------------------------------------------*/
typedef struct {
  boolean          card_status;
  boolean          sap_events;
  boolean          extended_card_status;
  boolean          prov_session_close_event;
  boolean          physical_slots_status;
  boolean          sim_busy_events;
  boolean          reduced_card_status_events;
  boolean          recovery_complete_event;
  boolean          supply_voltage_events;
  boolean          reg_for_activation_status_event;
  boolean          simlock_remote_sfs_events;
  boolean          simlock_temp_unlock_status;
} qmi_uim_registration_event_type;


/*=============================================================================

                                  FUNCTIONS

=============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_PARSE_REQUEST()

  DESCRIPTION
    Parses the TLV content of QMI_UIM message

  PARAMETERS
    sdu_in      : input sdu
    parsed_data : output with parsed values
    count       : number of values to be parsed

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_parse_request(
  dsm_item_type **        sdu_in,
  qmi_uim_tlv_item_type * parsed_data,
  uint16                  count
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CARD_RESULT()

  DESCRIPTION
    Composes the TLV content of card status

  PARAMETERS
    tlv_id      : id to be used for the TLV
    response    : output sdu
    status_word : status word, as received from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_card_result(
  byte             tlv_id,
  dsm_item_type ** response,
  mmgsdi_sw_type   status_word
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_READ_ADDITIONAL_RECORDS_RESULT()

  DESCRIPTION
    Composes the TLV content of additional records result

  PARAMETERS
    tlv_id         : id to be used for the TLV
    response       : output sdu
    num_records    : number of records in the array
    records_ptr    : array of records from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_read_additional_records_result(
  byte                              tlv_id,
  dsm_item_type **                  response,
  uint16                            num_records,
  qmi_uim_additional_records_type * records_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_FILE_ATTRIBUTES()

  DESCRIPTION
    Composes the TLV content of read result

  PARAMETERS
    tlv_id    : id to be used for the TLV
    response  : output sdu
    read_data : data read from the card
    raw_value : attributes raw value

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_file_attributes(
  byte                           tlv_id,
  dsm_item_type **               response,
  mmgsdi_file_attributes_type    attribute_data,
  mmgsdi_get_file_attr_data_type raw_value
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_RETRIES_LEFT()

  DESCRIPTION
    Composes the TLV content of PIN retries left

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response            : output sdu
    num_retries         : number of PIN retries
    num_unblock_retries : number of PUK retries

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_retries_left(
  byte             tlv_id,
  dsm_item_type ** response,
  uint32           num_retries,
  uint32           num_unblock_retries
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CARD_STATUS()

  DESCRIPTION
    Composes the TLV content for card status indication or
    get card status request

  PARAMETERS
    tlv_id     : id to be used for the TLV
    indication : output sdu
    card_state : card status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_card_status(
  byte                       tlv_id,
  dsm_item_type **           response,
  const qmi_uim_card_state * card_state
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EXTENDED_CARD_STATUS()

  DESCRIPTION
    Composes the extended card status TLV content for card status indication
    or get card status request

  PARAMETERS
    tlv_id         : id to be used for the TLV
    response       : output sdu
    card_state_ptr : card status
    validity_len   : length of card status validity array
    validity_ptr   : card status validity array

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_extended_card_status(
  byte                       tlv_id,
  dsm_item_type **           response,
  const qmi_uim_card_state * card_state_ptr,
  uint8                      validity_len,
  const boolean            * validity_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EVENT_REG()

  DESCRIPTION
    Composes the TLV content of event reg

  PARAMETERS
    tlv_id                   : id to be used for the TLV
    response_pptr            : output sdu
    reg_event_type           : event registered type

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_event_reg(
  byte                               tlv_id,
  dsm_item_type                   ** response_pptr,
  qmi_uim_registration_event_type    reg_event_type
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_REFRESH_EVENT()

  DESCRIPTION
    Composes the TLV content of refresh event

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response            : output sdu
    session_type        : session type
    aid_ptr             : AID value
    refresh_evt_ptr     : refresh event

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_refresh_event(
  byte                                 tlv_id,
  dsm_item_type **                     response,
  mmgsdi_session_type_enum_type        session_type,
  const mmgsdi_static_data_type *      aid_ptr,
  const mmgsdi_refresh_evt_info_type * refresh_evt_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AUTHENTICATE_CAVE()

  DESCRIPTION
    Composes the TLV content of cave authentication

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    cave_result         : result of CAVE algorithm

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_authenticate_cave(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  mmgsdi_cave_auth_resp_type  cave_result
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AUTHENTICATE_GSM_ALGO()

  DESCRIPTION
    Composes the TLV content of GSM authentication

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    sres_ptr            : SRES value
    kc_ptr              : Kc value

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_authenticate_gsm_algo(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  const uint8 *               sres_ptr,
  const uint8 *               kc_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AUTHENTICATE_GENERIC()

  DESCRIPTION
    Composes the TLV content of a generic authentication where entire
    response is passed as it is.

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    auth_response       : result of authenticate

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_authenticate_generic(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  mmgsdi_data_type            auth_response
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AUTHENTICATE_ISIM()

  DESCRIPTION
    Composes the TLV content of ISIM authentication

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    status        : status
    auth_context  : auth context
    res           : RES value
    ck            : CK value
    ik            : IK value
    auts          : AUTS value

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_authenticate_isim(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  mmgsdi_return_enum_type     status,
  uim_auth_context_enum_v01   auth_context,
  mmgsdi_data_type            res,
  mmgsdi_data_type            ck,
  mmgsdi_data_type            ik,
  mmgsdi_data_type            auts
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SERVICE_STATUS()

  DESCRIPTION
    Composes the TLV content of FDN/ACL status

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    svc_available : Service is available or not
    svc_enabled   : Service is enabled or not

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_service_status(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  boolean                     svc_available,
  boolean                     svc_enabled
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_LABEL()

  DESCRIPTION
    Composes the TLV content of application label

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    label_ptr     : Label

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_label(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  const mmgsdi_static_data_type * label_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_HIDDEN_KEY_STATUS()

  DESCRIPTION
    Composes the TLV content of hidden key status

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    hidden_key_status  : hidden key status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_hidden_key_status(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  mmgsdi_pin_status_enum_type     hidden_key_status
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_INDEX_IN_DIR()

  DESCRIPTION
    Composes the TLV content of index in EF-DIR

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    index_in_dir       : index in EF-DIR file

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_index_in_dir(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint8                           index_in_dir
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_ESN_STATUS()

  DESCRIPTION
    Composes the TLV content of ESN status

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    esn_status    : ESN is changed or not

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_esn_status(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  mmgsdi_esn_me_change_enum_type  esn_status
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SEND_APDU_RESULT()

  DESCRIPTION
    Composes the TLV content of send APDU result

  PARAMETERS
    tlv_id                  : id to be used for the TLV
    response_pptr           : output sdu
    apdu_data               : response APDU data from the card
    procedure_bytes         : procedure bytes, if applicable

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_send_apdu_result(
  byte                                tlv_id,
  dsm_item_type **                    response_pptr,
  mmgsdi_send_apdu_data_type          apdu_data,
  mmgsdi_sw_type                      procedure_bytes
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PROCEDURE_BYTES()

  DESCRIPTION
    Composes the TLV content of procedure bytes

  PARAMETERS
    tlv_id                  : id to be used for the TLV
    response_pptr           : output sdu
    procedure_bytes         : procedure bytes to be sent

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_procedure_bytes
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  mmgsdi_sw_type                procedure_bytes
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GENERIC_PAYLOAD_8BIT()

  DESCRIPTION
    Composes the TLV content of variable data passed

  PARAMETERS
    tlv_id            : id to be used for the TLV
    response_pptr     : output sdu
    response_data     : response data from MMGSDI for the request
    payload_mandatory : specifies if response_data is to be checked for
                        validity

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_generic_payload_8bit
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  mmgsdi_data_type              response_data,
  boolean                       payload_mandatory
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GENERIC_PAYLOAD_16BIT()

  DESCRIPTION
    Composes the TLV content of variable data passed

  PARAMETERS
    tlv_id            : id to be used for the TLV
    response_pptr     : output sdu
    response_data     : response data from MMGSDI for the request
    payload_mandatory : specifies if response_data is to be checked for
                        validity

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_generic_payload_16bit
(
  byte                  tlv_id,
  dsm_item_type **      response_pptr,
  mmgsdi_data_type      response_data,
  boolean               payload_mandatory
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_LOGICAL_CHANNEL()

  DESCRIPTION
    Composes the TLV content of logical channel

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    logical_channel    : logical channel

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_logical_channel(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint8                           logical_channel
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_HOT_SWAP_STATUS()

  DESCRIPTION
    Composes the hot swap switch status TLV content for get card status
    request

  PARAMETERS
    tlv_id                : id to be used for the TLV
    response_pptr         : output sdu
    hot_swap_status_len   : length of hot_swap_status_ptr array
    hot_swap_status_ptr   : array with status of hot swap switch for each slot

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_hot_swap_status
(
  byte                           tlv_id,
  dsm_item_type **               response_pptr,
  boolean                        extended_status,
  uint8                          hot_swap_status_len,
  uim_rt_status_enum  *          hot_swap_status_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_CONVERT_FILE_ACCESS_METHOD()

  DESCRIPTION
    Converts file access method between path and MMGSDI enum

  PARAMETERS
    file_access_ptr : input/output with file value
    access_method   : desired output access method
    session_type    : session type

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_convert_file_access_method(
  mmgsdi_access_type             * file_access_ptr,
  mmgsdi_access_method_enum_type   access_method,
  mmgsdi_session_type_enum_type    session_type
);

/*===========================================================================
  FUNCTION QMI_UIM_CHECK_PIN_DIGITS()

  DESCRIPTION
    Checks the digits of PIN value. Allowed digits are only from
    '0' to '9'. This function does not check the length of the PIN.

  PARAMETERS
    pin_value : value of the pin

  RETURN VALUE
    boolean to indicate success or failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_check_pin_digits (
  mmgsdi_data_type  pin_value
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_FILE_LENGTH()

  DESCRIPTION
    Composes the file length TLV

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    file_length        : file length

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_file_length
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint16                          file_length
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_APDU_LONG_RESPONSE()

  DESCRIPTION
    Composes the TLV content of send APDU long response

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    token         : Token used in the subsequent indications
    total_len     : Total length of the response APDU from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_apdu_long_response
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  uint32                        token,
  uint16                        total_len
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_LONG_RESPONSE_CHUNK()

  DESCRIPTION
    Composes the TLV content of a long response chunk. This is currently used
    in cases of send APDU and read transparent indications.

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    token         : Token used in the previous long response
    total_len     : Total length of the response data from the card
    offset        : Offset of the current chunk of response data
    long_data     : One chunk of the long response data from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_long_response_chunk
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  uint32                        token,
  uint16                        total_len,
  uint16                        offset,
  mmgsdi_data_type              long_data
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CARD_STATUS_VALIDITY()

  DESCRIPTION
    Composes the card status validity TLV content for get card status
    request

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    validity_len    : length of validity status array
    validity_ptr    : pointer to validity status array

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_card_status_validity
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  uint8                           validity_len,
  boolean                       * validity_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SLOT()

  DESCRIPTION
    Composes the TLV content for Slot ID

  PARAMETERS
    slot_id : MMGSDI slot information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_slot
(
  byte                        tlv_id,
  dsm_item_type            ** response_pptr,
  mmgsdi_slot_id_enum_type    slot
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SIM_BUSY()

  DESCRIPTION
    Composes the sim busy TLV content for get card status request.

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    card_state_ptr  : structure containing num of cards and sim busy status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_sim_busy
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  const qmi_uim_card_state      * card_state_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_CARD_MODE()

  DESCRIPTION
    Composes the card mode TLV content for get card status response and indication.

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    card_state_ptr  : structure containing num of cards and card mode

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_card_mode
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  const qmi_uim_card_state      * card_state_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_ACTIVATION_STATUS()

  DESCRIPTION
    Composes the TLV content for card activation status

  PARAMETERS
    tlv_id            : id to be used for the TLV
    response_pptr     : output sdu
    gstk_otasp_status : contains GSTK OTASP activation status

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_activation_status
(
  byte                               tlv_id,
  dsm_item_type                   ** response_pptr,
  gstk_otasp_act_status_enum_type    gstk_otasp_status
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_AID()

  DESCRIPTION
    Composes the TLV content for AID

  PARAMETERS
    aid_ptr : AID information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_aid
(
  byte                             tlv_id,
  dsm_item_type                 ** response_pptr,
  const mmgsdi_static_data_type  * aid_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_FILE_STATUS()

  DESCRIPTION
    Composes the TLV content for file status

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    file_attrib_ptr  : Pointer to file attributes response

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_file_status
(
  byte                                 tlv_id,
  dsm_item_type                     ** response_pptr,
  const mmgsdi_file_attributes_type  * file_attrib_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SESSION_TYPE()

  DESCRIPTION
    Composes the TLV content for Session type

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    session_type     : MMGSDI session type

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_session_type
(
  byte                           tlv_id,
  dsm_item_type               ** response_pptr,
  mmgsdi_session_type_enum_type  session_type
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SESSION_CLOSED_CAUSE()

  DESCRIPTION
    Composes the TLV content for session closure cause

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    cause            : Session close cause

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_session_closed_cause
(
  byte                                    tlv_id,
  dsm_item_type                       **  response_pptr,
  mmgsdi_session_closed_cause_enum_type   cause
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SESSION_CLOSED_FILE_ID()

  DESCRIPTION
    Composes the TLV content for the File ID

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    cause            : Session close cause

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_session_closed_file_id
(
  byte                                   tlv_id,
  dsm_item_type                       ** response_pptr,
  mmgsdi_session_closed_cause_enum_type  cause
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PHYSICAL_SLOTS_STATUS()

  DESCRIPTION
    Composes the TLV content for physical slots status indication

  PARAMETERS
    tlv_id             : id to be used for the TLV
    response_pptr      : output sdu
    slot_status_ptr    : Status of physical slots
    num_slots          : number of physical slots

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_physical_slots_status
(
  byte                               tlv_id,
  dsm_item_type                   ** response_pptr,
  const uim_phy_slot_status_type   * slot_status_ptr,
  uint8                              num_slots
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SE13_TABLE_RELEASE_DATE()

  DESCRIPTION
  Composes the TLV content for the SE.13 table release date

  PARAMETERS
    tlv_id                          : id to be used for the TLV
    response_pptr                   : output sdu
    se13_table_release_info_ptr     : contains SE.13 source and release date

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_se13_table_release_date
(
  byte                                       tlv_id,
  dsm_item_type                           ** response_pptr,
  const mmgsdi_se13_table_details_info     * se13_table_details_info_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SE13_TABLE_RELEASE_SOURCE()

  DESCRIPTION
    Composes the TLV content for the SE.13 table source

  PARAMETERS
    tlv_id                          : id to be used for the TLV
    response_pptr                   : output sdu
    se13_table_release_info_ptr     : contains SE.13 source and release date

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_se13_table_release_source
(
  byte                                       tlv_id,
  dsm_item_type                           ** response_pptr,
  const mmgsdi_se13_table_details_info     * se13_table_details_info_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_INCREASE_RESULT()

  DESCRIPTION
    Composes the TLV content of increase result

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    increase_result : increase data from the card

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_increase_result
(
  byte                         tlv_id,
  dsm_item_type             ** response_pptr,
  mmgsdi_increase_data_type    increase_data
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_BOOLEAN()

  DESCRIPTION
    Composes the TLV content from the value of boolean that is passed in

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    is_true       : boolean

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_boolean
(
  byte                        tlv_id,
  dsm_item_type **            response_pptr,
  boolean                     is_true
);

/*===========================================================================
  FUNCTION QMI_UIM_IS_ISIM_AID()

  DESCRIPTION
    Checks if the current AID belongs to a ISIM or not

  PARAMETERS
    aid_ptr       : value of the AID

  RETURN VALUE
    TRUE for ISIM. FALSE in all other cases

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_is_isim_aid
(
  const mmgsdi_static_data_type * aid_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_IS_USIM_AID()

  DESCRIPTION
    Checks if the current AID belongs to a USIM or not.

  PARAMETERS
    aid_ptr             : value of the AID
    partial_aid_match_ok: dictates whether caller is ok with going ahead
                          with checking for a partial AID match incase
                          complete AID did not match with USIM

  RETURN VALUE
    boolean
      TRUE        : if the AID matched (completely or partially depending on
                    input parameter).
      FALSE       : in all other cases

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_is_usim_aid
(
  const mmgsdi_static_data_type * aid_ptr,
  boolean                         partial_aid_match_ok
);

/*===========================================================================
  FUNCTION QMI_UIM_IS_CSIM_AID()

  DESCRIPTION
    Checks if the current AID belongs to a CSIM or not.

  PARAMETERS
    aid_ptr             : value of the AID
    partial_aid_match_ok: dictates whether caller is ok with going ahead
                          with checking for a partial AID match incase
                          complete AID did not match with USIM

  RETURN VALUE
    boolean
      TRUE        : if the AID matched (completely or partially depending on
                    input parameter).
      FALSE       : in all other cases

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_is_csim_aid
(
  const   mmgsdi_static_data_type * aid_ptr,
  boolean                           partial_aid_match_ok
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_VCC_STATE()

  DESCRIPTION
    Composes the TLV content for Vcc state

  PARAMETERS
    tlv_id          : id to be used for the TLV
    response_pptr   : output sdu
    ldo_state       : UIM LDO state information

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_vcc_state
(
  byte                              tlv_id,
  dsm_item_type                  ** response_pptr,
  uimdrv_qmi_indications_type       ldo_state
);

/*===========================================================================
  FUNCTION  QMI_UIM_RESPONSE_ENCRYPTED_STATUS()

  DESCRIPTION
    Encrypts and builds the response returned to security sensitive clients

  PARAMETERS
    tlv_id             : id to be used for the TLV
    cmd_id             : Command id
    request_id         : Request id
    result             : SimLock result status
    response_pptr      : Output sdu

  RETURN VALUE
    errval             : QMI client-specific error message

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_response_encrypted_status
(
  byte                         tlv_id,
  uint16                       cmd_id,
  uint16                       request_id,
  qmi_error_e_type             error,
  dsm_item_type **             response_pptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_ENCRYPTED_PIN1()

  DESCRIPTION
    Composes the TLV content of the encrypted PIN1 value

  PARAMETERS
    tlv_id                : id to be used for the TLV
    response_pptr         : output sdu
    encrypted_pin1_length : encrypted PIN1 data length
    encrypted_pin1_ptr    : encrypted PIN1 data pointer

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_encrypted_pin1
(
  byte                          tlv_id,
  dsm_item_type **              response_pptr,
  uint8                         encrypted_pin1_length,
  const uint8 *                 encrypted_pin1_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SESSION_INFORMATION()

  DESCRIPTION
    Composes the TLV content for Session information

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    session_type     : MMGSDI session type
    aid_len          : Length of AID
    aid_ptr          : Pointer to AID data

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_session_information
(
  byte                            tlv_id,
  dsm_item_type                ** response_pptr,
  mmgsdi_session_type_enum_type   session_type,
  uint8                           aid_len,
  const uint8                   * aid_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GBA_EVENT()

  DESCRIPTION
    Composes the TLV content for GBA event type

  PARAMETERS
    tlv_id           : TLV id to be sent
    response_pptr    : Pointer to response pointer
    evt_type         : GBA event type

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_gba_event
(
  byte                            tlv_id,
  dsm_item_type                ** response_pptr,
  gba_key_event_enum_type         evt_type
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_TEMPORARY_IDENTITY_MASK()

  DESCRIPTION
    Composes the TLV content for temporary identity mask

  PARAMETERS
    tlv_id                     : TLV id to be sent
    response_pptr              : Pointer to response pointer
    temporary_identity         : temporary identity

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_temporary_identity_mask
(
  byte                                          tlv_id,
  dsm_item_type                              ** response_pptr,
  qmi_uim_tlv_temporary_identity_mask_type      temporary_identity
);

/*===========================================================================
  FUNCTION  QMI_UIM_RESPONSE_GET_PHYSICAL_SLOT_MAPPING()

  DESCRIPTION
    Composes logical physical slot mapping TLV

  PARAMETERS
    tlv_id                       : ID to be used for TLV
    response_pptr                : Pointer to Indication pointer
    logical_phy_mapping          : Logical physical slot mapping

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_physical_slot_mapping
(
  byte                                     tlv_id,
  dsm_item_type                          **response_pptr,
  uim_logical_slot_config_type             logical_phy_mapping
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_GET_EID_INFO()

  DESCRIPTION
    Composes the TLV for EID retrieved

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_eid_resp_ptr    : profile confirmation pointer

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_get_eid_info
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  const lpa_get_eid_resp_type           *get_eid_resp_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_TERMINAL_CAPABILITY()

  DESCRIPTION
    Composes the TLV content of GET terminal capability

  PARAMETERS
    tlv_id                : id to be used for the TLV
    response_pptr         : output sdu
    num_tlvs              : Number of TLVs
    tlvs_data_len         : Total length of the ptr containing the TLVs
    tlvs_data_ptr         : Ptr containing the TLVs

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_terminal_capability
(
  byte                                       tlv_id,
  dsm_item_type **                           response_pptr,
  uint8                                      num_tlvs,
  uint8                                      tlvs_data_len,
  uint8*                                     tlvs_data_ptr
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PARSED_IMSI()

  DESCRIPTION
    This function parses the IMSI data to MCC, MNC and MSIN ascii values and
    composes the TLV content of parsed IMSI.
 
  PARAMETERS
    None

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_parsed_imsi
(
  byte                    tlv_id,
  dsm_item_type**         response_pptr,
  mmgsdi_data_type        response_data,
  uint8                   num_mnc_digits
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_PARSED_PLMN_LIST()

  DESCRIPTION
    This function parses the FPLMN and EHPLMN data to number of plmns and
    plmns(MCC, MNC) in ascii values and composes the TLV content of
    parsed PLMN list.
 
  PARAMETERS
    None

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_parsed_plmn_list
(
  byte                  tlv_id,
  dsm_item_type**       response_pptr,
  mmgsdi_data_type      response_data
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_REFRESH_ENFORCEMENT_POLICY()

  DESCRIPTION
    Composes the TLV content of refresh enforcement policy

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    enforcement_policy  : refresh enforcement policy

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_refresh_enforcement_policy
(
  byte                                   tlv_id,
  dsm_item_type **                       response_pptr,
  mmgsdi_refresh_enforcement_policy_type enforcement_policy
);

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EXTENDED_CARD_INFO()

  DESCRIPTION
    Composes the extended info TLV content, ICCID, EID and ATR for card status 
	indication or get card status request

  PARAMETERS
    tlv_id           : id to be used for the TLV
    response_pptr    : output sdu
    card_state_ptr   : logical slots information

  RETURN VALUE
    boolean          : TRUE if extended_card_info is filled succsully
                       else FALSE

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_extended_card_info
(
  byte                             tlv_id,
  dsm_item_type **                 response_pptr,
  const qmi_uim_card_state       * card_state_ptr
);

#endif /* QMI_UIM_PARSING_H */
