#ifndef LPALIB_H
#define LPALIB_H
/*===========================================================================


            L P A   L I B R A R Y   D E F I N I T I O N S

                      A N D   F U N C T I O N S


  This file contains library functions that can be triggered as a result
  of trigger from user application

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved. QUALCOMM Technologies Proprietary.
Export of this technology or software is
regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/inc/lpalib.h#4 $ $DateTime: 2021/02/08 01:08:56 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
01/21/21   dj      Fixing compilation errors
04/19/19   bcho    Increase GID max length
03/29/18   bcho    LPA phase2.1 changes
09/15/17   me      Fixing compiler errors   
04/12/17   av      Add support to configure/retrieve default server address
02/22/17   ar      Call LPA APIs only if LPA is modem centric
01/12/17   av      Bug fixes for phase2 PRRID related logic
12/07/16   av      LPA phase2 initial changes
09/22/16   ll      Peek into profile installation result
05/19/16   ll      ASN1 decoder enhancement
04/14/16   av      Initial Revision

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "comdef.h"

/* The following are max/mins as per the specs */
#define LPA_EID_LEN                          16
#define LPA_ICCID_LEN                        10
#define LPA_ISDP_AID_LEN                     16
#define LPA_ISDP_AID_LEN_MIN                  5
#define LPA_NAME_LEN_MAX                     64
#define LPA_SPN_LEN_MAX                      32
#define LPA_ICON_LEN_MAX                   1024
#define LPA_NUM_PPR_ID_MAX                    3
#define LPA_MCC_LEN                           3
#define LPA_MNC_LEN_MAX                       3

/* The following max/mins are implementation specific and not mandated by
   the spec */
#define LPA_NUM_PROFILES_MAX                  8
#define LPA_NUM_OPERATOR_ID_MAX              16
#define LPA_GID_LEN_MAX                      16

/*=============================================================================

                   ENUMERATED DATA DECLARATIONS

=============================================================================*/
/* ----------------------------------------------------------------------------
   ENUM:      LAP_RESULT_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the result of LPA request message
-----------------------------------------------------------------------------*/
typedef enum {
  LPA_SUCCESS,
  LPA_GENERIC_ERROR,
  LPA_ERROR_INCORRECT_PARAMS,
  LPA_ERROR_COMMAND_TIMEOUT,
  LPA_ERROR_HEAP_EXHAUSTED,
  LPA_ERROR_NETWORK,
  LPA_ERROR_NETWORK_SSL,
  LPA_ERROR_NETWORK_DNS,
  LPA_ERROR_NETWORK_SOCKET,
  LPA_ERROR_SMDP_RESPONSE,
  LPA_ERROR_SIM_NOT_READY,
  LPA_ERROR_PROFILE_NOT_IN_DISABLED_STATE,
  LPA_ERROR_PROFILE_NOT_IN_ENABLED_STATE,
  LPA_ERROR_PROFILE_NOT_FOUND,
  LPA_ERROR_REFERENCE_DATA_NOT_FOUND,
  LPA_ERROR_OPERATION_NOT_ALLOWED,
  LPA_ERROR_CONFIRMATION_CODE_REQUIRED,
  LPA_ERROR_CMD_QUEUE_FULL,
  LPA_ERROR_SELECT_ISDR_FAILED,
  LPA_ERROR_NOT_SUPPORTED,
  LPA_ERROR_ICCID_EXISTS_ON_EUICC,
  LPA_ERROR_INSUFFICIENT_MEMORY_ON_EUICC,
  LPA_ERROR_PROFILE_INSTALLATION_INTERRUPTED_ON_EUICC,
  LPA_ERROR_PROFILE_INSTALLATION_UNKNOWN_ERR,
  LPA_ERROR_PROFILE_INSTALLATION_END_USER_REJECTION,
  LPA_ERROR_PROFILE_INSTALLATION_PPR_ID_NOT_ALLOWED,
  LPA_ERROR_NO_EFFECT_FOR_ACTION,
  LPA_ERROR_PROFILE_WRONG_PROFILE_REENABLING,
  LPA_ERROR_SMDP_INVALID_CERTIFICATE,
  LPA_ERROR_SMDP_INVALID_SIGNATURE,
  LPA_ERROR_SMDP_UNSUPPORTED_CURVE,
  LPA_ERROR_SMDP_NO_SESSION_CONTEXT,
  LPA_ERROR_SMDP_INVALID_TRANSACTION_ID,
  LPA_ERROR_SMDP_INVALID_OID,
  LPA_ERROR_SMDP_EUICC_CHALLENGE_MISMATCH,
  LPA_ERROR_SMDP_CI_PK_UNKNOWN,
  LPA_ERROR_UNSUPPORTED_PROFILE_CLASS,
  LPA_ERROR_LOAD_BPP_EXECUTION_ERROR,
  LPA_ERROR_CAT_BUSY
} lpa_result_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      LPA_SLOT_ID_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the LPA slot IDs
-----------------------------------------------------------------------------*/
typedef enum {
  LPA_SLOT_1       = 0x00,
  LPA_SLOT_2,
  LPA_NUM_SLOTS
} lpa_slot_id_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      LPA_EVENT_ENUM_TYPE

   DESCRIPTION: The events from the LPA
-------------------------------------------------------------------------------*/
typedef enum {
  LPA_PROFILE_DOWNLOAD_PROGRESS_EVENT,
  LPA_REQUEST_USER_CONSENT_EVENT
} lpa_event_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      LPA_PROFILE_STATE_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the state of a LPA profile on the eUICC
-----------------------------------------------------------------------------*/
typedef enum {
  LPA_PROFILE_STATE_DISABLED,
  LPA_PROFILE_STATE_ENABLED
} lpa_profile_state_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      LPA_ICON_TYPE_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the type of icon wrt the profile info
-----------------------------------------------------------------------------*/
typedef enum {
  LPA_ICON_TYPE_JPEG,
  LPA_ICON_TYPE_PNG
} lpa_icon_type_enum_type;

 /* ----------------------------------------------------------------------------
   ENUM:      LPA_PROFILE_CLASS_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the class of profile
-----------------------------------------------------------------------------*/
typedef enum {
  LPA_PROFILE_CLASS_TEST,
  LPA_PROFILE_CLASS_PROVISIONING,
  LPA_PROFILE_CLASS_OPERATIONAL
} lpa_profile_class_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      LPA_MESSAGE_ENUM_TYPE

   DESCRIPTION:
     This enum specifies the message types LPA supports
-------------------------------------------------------------------------------*/
typedef enum {
  LPA_GET_PROFILES_INFO_MSG,
  LPA_ENABLE_PROFILE_MSG,
  LPA_DISABLE_PROFILE_MSG,
  LPA_DELETE_PROFILE_MSG,
  LPA_CARD_MEMORY_RESET_MSG,
  LPA_GET_EID_MSG,
  LPA_ADD_PROFILE_MSG,
  LPA_ADD_OR_UPDATE_PROFILE_NICKNAME_MSG,
  LPA_GET_RAT_MSG, /* This is an internal request */
  LPA_SEND_USER_CONSENT_MSG,
  LPA_GET_DEFAULT_SERVER_ADDRESS_MSG,
  LPA_SET_DEFAULT_SERVER_ADDRESS_MSG,
  LPA_MESSAGE_ENUM_MAX                                 = 0x7FFFFFFF
} lpa_message_enum_type;

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
typedef uint8 lpa_eid_type[LPA_EID_LEN];

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ISDP_AID_TYPE

   DESCRIPTION:
     This structure contains the ISDP-AID.
     length:    Number of ISDP-AID bytes (max length can be 16 bytes per SGP.22)
     data:      ISDP-AID
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8 data_len;
  uint8 data[LPA_ISDP_AID_LEN];
} lpa_isdp_aid_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ICCID_TYPE

   DESCRIPTION:
     This structure contains the ICCID.
     length:    Number of ICCID bytes (max length can be 10 bytes per SGP.22)
     data:      ICCID
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8 data_len;
  uint8 data[LPA_ICCID_LEN];
} lpa_iccid_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_DATA_TYPE

   DESCRIPTION:
     This structure is generic structure to hold a stream of bytes.
     data_len:  Lenght of data
     data_ptr:  Data
-------------------------------------------------------------------------------*/
typedef struct
{
  uint32  data_len;
  uint8*  data_ptr;
} lpa_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_NAME_TYPE

   DESCRIPTION:
     This structure is generic structure to hold profile name/nickname.
     name_len:  Lenght of name
     name:      Name
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8   name_len;
  uint8   name[LPA_NAME_LEN_MAX];
} lpa_name_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_SPN_TYPE

   DESCRIPTION:
     This structure is generic structure to hold service provider name.
     name_len:  Lenght of name
     name:      Name
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8   name_len;
  uint8   name[LPA_SPN_LEN_MAX];
} lpa_spn_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_MCC_TYPE

   DESCRIPTION:
     This data structure is generic structure to hold MCC in ASCII format. Any
     digit can be wildcarded by setting it to 'E'.
-------------------------------------------------------------------------------*/
typedef char lpa_mcc_type[LPA_MCC_LEN];

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_MNC_TYPE

   DESCRIPTION:
     This structure is generic structure to hold MNC in ASCII format. Any
     digit can be wildcarded by setting it to 'E'.
     mnc_len:   Number of digits of MNC
     mnc_data:  MNC
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8    mnc_len;
  char     mnc_data[LPA_MNC_LEN_MAX];
} lpa_mnc_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_GID_TYPE

   DESCRIPTION:
     This structure is generic structure to hold GIDx.
     gid_len:   GID length
     gid_data:  GID
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8    gid_len;
  uint8    gid_data[LPA_GID_LEN_MAX];
} lpa_gid_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_OPERATOR_ID_TYPE

   DESCRIPTION:
     This structure is generic structure to hold an OperatorId entry as per
     SGP22.
     mcc     : MCC digits of the operator. Any digit can be wildcarded by
               setting appropriate character to 'E'
     mnc     : MNC digits of the operator. Any digit can be wildcarded by
               setting appropriate character to 'E'
     gid1    : GID1. It can be wildcarded by setting it to 0
     gid2    : GID2. It can be wildcarded by setting it to 0
-------------------------------------------------------------------------------*/
typedef struct
{
  lpa_mcc_type        mcc;
  lpa_mnc_type        mnc;
  lpa_gid_type        gid1;
  lpa_gid_type        gid2;
} lpa_operator_id_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_PPR_IDS_TYPE

   DESCRIPTION:
   This structure specifies the Profile Policy Rule IDs
-------------------------------------------------------------------------------*/
typedef struct {
  boolean ppr1;
  boolean ppr2;
  boolean ppr3;
} lpa_ppr_ids_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_PPR_USER_CONSENT_TYPE

   DESCRIPTION:
   This structure specifies the booleans corresponding to user consent for all
   PPRs
-------------------------------------------------------------------------------*/
typedef struct {
  boolean ppr1_user_consent_required;
  boolean ppr2_user_consent_required;
  boolean ppr3_user_consent_required;
} lpa_ppr_user_consent_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_PPAR_TABLE_TYPE

   DESCRIPTION:
     This structure is generic structure to hold a PPAR entry.
     ppr_ids              : Any combination of three PPR IDs defined in SGP22
     num_operator_ids     : Number of operator IDs (MCC/MNC/GIDx)
     operator_ids         : Operator IDs (MCC/MNC/GIDx)
     user_consent_required: Whether user consent is required for the PPR/
                            Operator combo
-------------------------------------------------------------------------------*/
typedef struct
{
  lpa_ppr_ids_type           ppr_ids;
  uint8                      num_operator_ids;
  lpa_operator_id_type       operator_ids[LPA_NUM_OPERATOR_ID_MAX];
  boolean                    user_consent_required;
} lpa_ppar_table_entry_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_RULES_AUTH_TABLE_TYPE

   DESCRIPTION:
     This structure is generic structure to hold Rules Authorization Table.
     num_ppar_entries:  Number of PPAR entries
     ppar_entry_ptr:    PPAR entries
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8                           num_ppar_entries;
  lpa_ppar_table_entry_type      *ppar_entry_ptr;
} lpa_rules_auth_table_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_PROFILE_POLICY_RULES_TYPE

   DESCRIPTION:
     Using this structure LPA returns the Profile Profile Rules (PPRs), as
     allowed for the operator of the profile by the Rules Authorization Table
     (RAT) on the card.
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_ppr_ids_type                ppr_ids;
  lpa_ppr_user_consent_type       user_consent;
  lpa_operator_id_type            operator_id;
} lpa_profile_policy_rules_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_PROFILE_INFO_TYPE

   DESCRIPTION:
     This structure contains information about a profile. The item is valid if
     it has a non-default (non-zero in most cases) value.
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_iccid_type                iccid;
  lpa_isdp_aid_type             isdp_aid;
  lpa_profile_state_enum_type   state;
  lpa_name_type                 nickname;
  lpa_spn_type                  spn;
  lpa_name_type                 name;
  lpa_icon_type_enum_type       icon_type;
  lpa_data_type                 icon;
  lpa_profile_class_enum_type   profile_class;
  lpa_profile_policy_rules_type ppr;
} lpa_profile_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_CARD_MEMORY_RESET_OPTION_TYPE

   DESCRIPTION:
     This structure specifies what all is to be reset on the card
-----------------------------------------------------------------------------*/
typedef struct {
  boolean        reset_test_profiles;       /* Delete all the test profiles */
  boolean        reset_operational_profiles;/* Delete all the operational
                                               profiles */
  boolean        reset_smdp_address;        /* Reset default SMDP address */
} lpa_card_memory_reset_option_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_GET_PROFILES_INFO_RESP_TYPE

   DESCRIPTION:
     This structure is for the LPA response corresponding to the get profiles
     info request.
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                           num_profiles;
  lpa_profile_info_type           profile_info[LPA_NUM_PROFILES_MAX];
} lpa_get_profiles_info_resp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_GET_EID_RESP_TYPE

   DESCRIPTION:
     This structure is for the LPA response corresponding to the get EID
     request
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_eid_type               eid;
} lpa_get_eid_resp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_GET_RAT_RESP_TYPE

   DESCRIPTION:
     Using this structure LPA returns the Rules Authorization Table

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_rules_auth_table_type       rules_auth_table;
} lpa_get_rat_resp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_DEFAULT_SERVER_ADDRESS_RESP_TYPE

   DESCRIPTION:
     This structure is for the LPA response corresponding to the retrieval of
     default SMDS and/or SMDP+ addresses from the eUICC.
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_data_type            default_smds_address;
  lpa_data_type            default_smdp_address;
} lpa_default_server_address_resp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_RESPONSE_DATA_TYPE

   DESCRIPTION:
     This structure is for the various response messages in LPA
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_message_enum_type  msg_type;
  union {
    lpa_get_profiles_info_resp_type                   get_profiles_info_resp;
    lpa_get_eid_resp_type                             get_eid_resp;
    lpa_get_rat_resp_type                             get_rat_resp;
    lpa_default_server_address_resp_type              default_server_address_resp;
  } message;
} lpa_response_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_PROFILE_DOWNLOAD_PROGRESS_EVENT_TYPE

   DESCRIPTION:
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                                              percent_downloaded;
  boolean                                            installation_complete;
} lpa_profile_download_progress_event_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_REQUEST_USER_CONSENT_EVENT_TYPE

   DESCRIPTION: Event sent to AP for displaying warning message to the user
                and asking user to provide user-consent, if applicable, based
                on the PPRIDs. Currently this event applies only to the
                AddProfiles operation.
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_ppr_ids_type                                   ppr_ids;
  boolean                                            provide_user_consent;
} lpa_request_user_consent_event_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_EVENT_DATA_TYPE

   DESCRIPTION:
     This structure is for the various event messages from LPA.
-------------------------------------------------------------------------------*/
typedef struct {
  lpa_event_enum_type                               evt_type;
  lpa_slot_id_enum_type                             slot_id;
  union{
    lpa_profile_download_progress_event_type        profile_download_progress;
    lpa_request_user_consent_event_type             request_user_consent;
  } evt_data;
} lpa_event_data_type;

/* ----------------------------------------------------------------------------
   DATA TYPE:      LPA_CALLBACK_TYPE

   This is client callback funtion type. The parameter to the callback function
   contains the results of the procedures performed at the client's request.
-----------------------------------------------------------------------------*/
typedef void (*lpa_callback_type) (lpa_result_enum_type          status,
                                   const lpa_response_data_type *response_ptr,
                                   void                         *user_data_ptr);

/* ----------------------------------------------------------------------------
   DATA TYPE:      LPA_EVT_CALLBACK_TYPE

   Event callback function associated to a client for receiving LPA related
   events.
-------------------------------------------------------------------------------*/
typedef void (*lpa_event_callback_type) (lpa_result_enum_type       status,
                                         const lpa_event_data_type *event);

/*=============================================================================

                     FUNCTION DECLARATIONS

=============================================================================*/
/*=============================================================================
FUNCTION        LPA_GET_PROFILES_INFO

DESCRIPTION
  This function allows the user to get complete information about all the
  profiles on the card.

PARAMETERS
  slot_id               :   Slot
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_get_profiles_info (
  lpa_slot_id_enum_type                 slot_id,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_ENABLE_PROFILE

DESCRIPTION
  This function allows the user to enable a profile identified by the ICCID
  on the card.
PARAMETERS
  slot_id               :   Slot
  iccid                 :   ICCID of the profile on the card
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_enable_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_DISABLE_PROFILE

DESCRIPTION
  This function allows the user to disable a profile identified by the ICCID
  on the card.
PARAMETERS
  slot_id               :   Slot
  iccid                 :   ICCID of the profile on the card
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_disable_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_DELETE_PROFILE

DESCRIPTION
  This function allows the user to delete a profile identified by the ICCID
  on the card.

PARAMETERS
  slot_id               :   Slot
  iccid                 :   ICCID of the profile on the card
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_delete_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_CARD_MEMORY_RESET

DESCRIPTION
  This function deletes from the card all or part of installed profiles based
  on the option field. If option field indicates that SMDP Address is also to
  be reset, card performs that operation as well.
  Note that if any profile is ENABLED when this command is received by the
  ISDR, the card issues a REFRESH after performing the memory reset for that
  profile.
PARAMETERS
  slot_id               :   Slot
  option                :   Option indicating what all is to be reset. This
                            field is ignored by LPA when the card is identified
                            to be a phase1/1.1 card
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_card_memory_reset (
  lpa_slot_id_enum_type                 slot_id,
  lpa_card_memory_reset_option_type      option,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_GET_EID

DESCRIPTION
  This function allows the user to retrieve the EID of the card.

PARAMETERS
  slot_id               :   Slot
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_get_eid (
  lpa_slot_id_enum_type                 slot_id,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_ADD_PROFILE

DESCRIPTION
  This function allows the user to download a profile from the SMDP server
  to the card. If the activation code suggests the confirmation code is
  needed, and no confirmation code is passed in to this API, the API call
  returns error. There are following ways/methods to trigger the download:
  1. By using activation code
  2. By asking ISDR to use SMDS discovery method
  3. By asking ISDR to use default SMDP+ address on the eUICC.
  If the method is not the first one above, the 'activation_code' string is
  used to indicate which of the two remaining methods are to be used.
  If activation code starts with 'UseSMDS', use method #2.
  If activation code starts with 'UseSMDP', use method #3.

PARAMETERS
  slot_id                 :   Slot
  activation_code         :   Activation code that is sent to the server to
                              download a profile. Max length is 255 characters
                              per SGP.22.
                              If the profile is to be downloaded using 'Default
                              SMDS' or 'Default SMDP+' methods, this string
                              is used to indicate that... in that case, this
                              is not actually an activation code but just a way
                              to indicate the 'profile download method'
  confirmation_code       :   Confirmation code, if mandated by the
                              activation_code.
  response_cb_ptr         :   Command response call back
  event_cb_ptr            :   Event callback called every n time, with the
                              profile download progress
  user_data_ptr           :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:                          The command structure was properly
                                          generated and queued onto the LPA
                                          command queue.
    LPA_ERROR_INCORRECT_PARAMS:                 The parameters supplied to the API
                                          are not within appropriate ranges.
    LPA_ERROR_CONFIRMATION_CODE_REQUIRED: Confirmation code is required but
                                          not provided by the caller.
    LPA_GENERIC_ERROR:                    The command was not queued to the LPA
                                          command queue.
===============================================================================*/
lpa_result_enum_type  lpa_add_profile (
  lpa_slot_id_enum_type                 slot_id,
  lpa_data_type                         activation_code,
  lpa_data_type                         confirmation_code,
  lpa_callback_type                     response_cb_ptr,
  lpa_event_callback_type               event_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_ADD_OR_UPDATE_PROFILE_NICKNAME

DESCRIPTION
  This function allows the user to add or update the nickname of a profile.

PARAMETERS
  slot_id               :   Slot
  iccid                 :   ICCID of the target profile
  nickname              :   Nickname of the target profile. Max length can be
                            64 characters per SGP.22
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_add_or_update_profile_nickname (
  lpa_slot_id_enum_type                 slot_id,
  lpa_iccid_type                        iccid,
  lpa_name_type                         nickname,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_SEND_USER_CONSENT

DESCRIPTION
  This function allows the user to send its consent to go ahead with a certain
  operation for which it has been asked asked by LPA (by sending an event/
  indication) to provide a consent.

PARAMETERS
  slot_id               :   Slot
  go_ahead              :   Yes or No
  msg_type              :   Message for which consent is sent by user
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_send_user_consent (
  lpa_slot_id_enum_type                 slot_id,
  boolean                               go_ahead,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_DEFAULT_SERVER_ADDRESS

DESCRIPTION
  This function allows the user to retrieve the default SM-DS address and
  default SM-DP+ (if any) configured on the eUICC.
  This function also allows the user to configure or remove the default
  SM-DP+ address on the eUICC.

PARAMETERS
  slot_id               :   Slot
  set_address           :   If TRUE, it is a SET request. If FALSE, it is a
                            GET request.
  default_smdp_address  :   If it is a SET request, the default SM-DP+ address
                            to be set
  response_cb_ptr       :   Command response call back
  user_data_ptr         :   User data

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:           The command structure was properly generated and
                           queued onto the LPA command queue.
    LPA_ERROR_INCORRECT_PARAMS:  The parameters supplied to the API are not within
                           appropriate ranges.
    LPA_GENERIC_ERROR:     The command was not queued to the LPA command
                           queue.
===============================================================================*/
lpa_result_enum_type  lpa_default_server_address (
  lpa_slot_id_enum_type                 slot_id,
  boolean                               set_address,
  lpa_data_type                         default_smdp_address,
  lpa_callback_type                     response_cb_ptr,
  void                                 *user_data_ptr
);

/*=============================================================================
FUNCTION        LPA_IS_LPA_SUPPORTED

DESCRIPTION
  Indicates if LPDd, LUId, LDSd, LUIe and modem LPA are supported or not.
  This info is needed to be sent to the card in the terminal capability.

PARAMETERS
  lpdd_supported_ptr       :   Boolean to indicate if lpdd is supported
  luid_supported_ptr       :   Boolean to indicate if luid is supported
  ldsd_supported_ptr       :   Boolean to indicate if ldsd is supported
  luie_supported_ptr       :   Boolean to indicate if luie is supported
  modem_lpa_supported_ptr  :   Boolean to indicate if modem LPA is supported

RETURN VALUE
  Void
===============================================================================*/
void lpa_is_lpa_supported(
  boolean                            *lpdd_supported_ptr,
  boolean                            *luid_supported_ptr,
  boolean                            *ldsd_supported_ptr,
  boolean                            *luie_supported_ptr,
  boolean                            *modem_lpa_supported_ptr
);

/*=============================================================================
FUNCTION        LPA_SET_QMI_UIM_HTTP_SIG

DESCRIPTION
  This function sets the passed signal to the LPA task

PARAMETERS
  None

RETURN VALUE
  lpa_result_enum_type
    LPA_SUCCESS:                The request was processed successfully
    LPA_ERROR_INCORRECT_PARAMS: The parameters supplied to the API are not within
                                appropriate ranges.
===============================================================================*/
lpa_result_enum_type lpa_set_qmi_uim_http_cmd_q_sig(void);

#endif /* LPALIB_H */
