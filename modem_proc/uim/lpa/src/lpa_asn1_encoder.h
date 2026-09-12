#ifndef LPA_ASN1_ENCODER_H
#define LPA_ASN1_ENCODER_H
/*===========================================================================


                      L P A   A S N 1   E N C O D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_asn1_encoder.h#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/29/18   bcho    LPA phase2.1 changes
12/07/16   av      LPA phase2 initial changes
05/15/16   ll      ASN1 encoder code clean up
03/28/16   ll      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "lpa.h"
#include "lpa_util.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ASN1_PPROFILE_MANAGE_ACTION_TYPE

   DESCRIPTION:
     This structure is used as input for
-----------------------------------------------------------------------------*/
typedef enum {
  LPA_ASN1_ENCODE_ENABLE_PROFILE,
  LPA_ASN1_ENCODE_DISABLE_PROFILE,
  LPA_ASN1_ENCODE_DELETE_PROFILE,
  LPA_ASN1_ENCODE_UNKNOW_ACTION
}lpa_asn1_profile_manage_action_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ASN1_PREPARE_DOWNLOAD_REQUEST_TYPE

   DESCRIPTION:
     This structure is used as input for encoding PrepareDownloadRequest request
     All the fields with _raw are either already in ASN1 format or dosen't need
     to be further encode to ASN1 format.
-----------------------------------------------------------------------------*/
typedef struct
{
  lpa_data_type             dp_signed_raw;              /*TAG_SEQUENCE*/
  lpa_data_type             smdp_signature_raw;         /*TAG_SMDPSIGNATURE*/
  lpa_data_type             activation_code_token;      /*TAG_UTF8STRING phase 1/1.1*/
  lpa_util_device_info_type device_info;                /*TAG_SEQUENCE phase 1/1.1*/
  lpa_smdp_id_type          smdpoid_string;             /*TAG_OBJECT_IDENTIFIER phase 1/1.1*/
  lpa_data_type             hashed_confirmation_code;   /*TAG_OCTET*/
  lpa_data_type             cert_format_to_be_used_raw; /*NOTAG CHOICE phase 1/1.1*/
  lpa_data_type             curve_to_be_used_raw;       /*TAG_OCTET phase 1/1.1*/
  lpa_data_type             smdp_certificate_raw;       /*TAG_SEQUENCE*/
}lpa_asn1_prepare_download_request_type;


/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ASN1_CTX_PARAMS_FOR_COMMON_AUTH_TYPE

   DESCRIPTION:
     This structure specifies the ctxParamsForCommonAuthentication
-----------------------------------------------------------------------------*/
typedef struct
{
  lpa_data_type             matching_id;
  lpa_util_device_info_type device_info;
} lpa_asn1_ctx_params_for_common_auth_type;


typedef union
{
  lpa_isdp_aid_type  isdp_aid;
  lpa_iccid_type     iccid;
} lpa_asn1_identify_profile_choice_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_ASN1_SET_NICKNAME_REQUEST_TYPE

   DESCRIPTION:
     This structure specifies the LPA slot IDs
-----------------------------------------------------------------------------*/
typedef struct
{
  lpa_iccid_type            iccid;
  lpa_name_type             profile_nickname;
} lpa_asn1_set_nickname_request_type;

/*=============================================================================

                        FUNCTIONS PROTOTYPES

=============================================================================*/
/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_EUICC_CONFIG_ADDRESS_REQUEST

DESCRIPTION
  This function encodes the GetEuiccConfiguredAddresses

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_euicc_config_address_request (
  lpa_data_type*     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_SET_DEFAULT_DPADDRESS_REQUEST

DESCRIPTION
  This function encodes the SetDefaultDpAddress

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_set_default_dpaddress_request (
  lpa_data_type      default_dpaddress,
  lpa_data_type*     out_ptr
);

/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_PREPARE_DOWNLOAD_REQUEST

DESCRIPTION
  This function encodes PrepareDownloadRequest

DEPENDENCIES
  Caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_prepare_download_request (
  lpa_asn1_prepare_download_request_type prepare_download_request,
  lpa_data_type*                         out_ptr,
  lpa_slot_id_enum_type                     slot_id
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_EUICC_CHALLENGE_REQUEST

DESCRIPTION
  This function encodes the GetEuiccChallengeRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_euicc_challenge_request (
  lpa_data_type*     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_EUICC_INFO_REQUEST

DESCRIPTION
  This function encodes the GetEUICCInfo

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_euicc_info_request (
  lpa_data_type*     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_LIST_NOTIFICATION_REQUEST

DESCRIPTION
  This function encodes the ListNotification

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_list_notification_request (
  uint8              profile_management_operation_byte_raw,
  lpa_data_type*     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_RETRIEVE_NOTIFICATION_LIST_REQUEST

DESCRIPTION
  This function encodes the RetrieveNotificationsList

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_retrieve_notification_list_request (
  lpa_data_type*     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_REMOVE_NOTIFICATION_FROM_LIST_REQUEST

DESCRIPTION
  This function encodes the RemoveNotificationFromList

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_remove_notification_from_list_request (
  uint32             seq_number_raw,
  lpa_data_type*     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_AUTHENTICATE_SERVER_REQUEST

DESCRIPTION
  This function encodes the AuthenticateServer

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_authenticate_server_request (
  lpa_data_type                            server_signed1,
  lpa_data_type                            server_signature1,
  lpa_data_type                            euicc_ci_pk_id_to_be_used,
  lpa_data_type                            server_certificate,
  lpa_asn1_ctx_params_for_common_auth_type ctx_params_for_common_auth_raw,
  lpa_data_type*                           out_ptr,
  lpa_slot_id_enum_type                    slot_id
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_CANCEL_SESSION_REQUEST

DESCRIPTION
  This function encodes the CancelSession

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_cancel_session_request (
  lpa_data_type              transaction_id,
  uint8                      cancel_session_reason,
  lpa_data_type*             out_ptr,
  lpa_euicc_phase_enum_type  phase
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_PROFILES_INFO_REQUEST

DESCRIPTION
  This function encodes the GetProfilesInfo

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_profiles_info_request (
  lpa_data_type*     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_MANAGE_PROFILE_REQUEST

DESCRIPTION
  This function encodes the EnableProfile, DisableProfile or DeleteProfile
  based on enable_flag

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_manage_profile_request (
  lpa_iccid_type                      iccid,
  boolean                             refresh_flag,
  lpa_data_type*                      out_ptr,
  lpa_asn1_profile_manage_action_type action
);

/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_EUICC_MEMORY_RESET_REQUEST

DESCRIPTION
  This function encodes the EuiccMemoryResetRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_euicc_memory_reset_request (
  lpa_card_memory_reset_option_type reset_option,
  lpa_data_type*                    out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_EID_REQUEST

DESCRIPTION
  This function encodes the GetEuiccDataRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_eid_request (
  lpa_data_type*     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_SET_NICKNAME_REQUEST

DESCRIPTION
  This function encodes the SetNicknameRequest

DEPENDENCIES
  Caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_set_nickname_request (
  lpa_asn1_set_nickname_request_type set_nickname_request,
  lpa_data_type*                     out_ptr
);


/*===========================================================================
FUNCTION LPA_ASN1_ENCODE_GET_RAT_REQUEST

DESCRIPTION
  This function encodes the GetRatRequest

DEPENDENCIES
  caller is responsible to free the the data pointer inside of out_ptr

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_asn1_encode_get_rat_request (
  lpa_data_type*     out_ptr
);
#endif /* LPA_ASN1_ENCODER_H */
