/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


       U I M   JSON encoder and decoder IMPLEMENTATION

GENERAL DESCRIPTION
  This module contains the procedures for the BTSAP in client mode implementation.
  The module receives the event from the QMI REMOTE CLIENT. The module handles the
  power up and power down sequence of the remote sim card.

Copyright (c) 2016, 2018 - 2019 by QUALCOMM Technologies, Inc. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_json.c#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/08/19    bcho   Fix issues in profile download on phase 2.1 cards
12/18/18    bcho   LPA version checks updated for versions greater than 2.0
03/29/18    bcho   LPA phase2.1 changes
03/13/18    rps    Fix overflow in uim_common_util_alloc_base64string_to_bin
12/09/16    ll     SVN encoder enhancement for multiple digit numbers
12/07/16    av     LPA phase2 initial changes
06/21/16    av     Add SVN check for supporting phase1, phase1.1 and phase2
04/11/16    ll     Initial version
===========================================================================*/
#include "lpa_json.h"
#include "lpa_platform.h"
#include "lpa_lpd.h"
#include "uim_common_lib.h"

#define LPA_JSON_EUICC_CHANLLENGE_SIZE                           16
#define LPA_JSON_SVN_FORMATED_STR_MAX_LEN                        12

#define LPA_JSON_ASCII_HEXADICIMALBASE                           '0'
#define LPA_JSON_ASCII_HEXADICIMALBASE_0_to_9                    '0'
/* 55 + 10 = 65 which is 'A'*/
#define LPA_JSON_ASCII_HEXADICIMALBASE_A_to_F                    55
#define LPA_JSON_ASCII_DOT                                       '.'
#define LPA_JSON_ASCII_NULL                                      0

#define JSON_HTTP_RSP_HEADER_STATUS_EXECUTED_SUCCESS             "Executed-Success"
#define JSON_HTTP_RSP_HEADER_STATUS_PATH                         "header.functionExecutionStatus.status"

#define JSON_HTTP_RSP_HEADER_STATUS_EXECUTED_FAILED              "Failed"
#define JSON_HTTP_RSP_HEADER_STATUS_DATA_SUBJECT_CODE_EUICC      "8.1"
#define JSON_HTTP_RSP_HEADER_STATUS_DATA_REASON_CODE_INSUFFICIENT_MEMORY "4.8"
#define JSON_HTTP_RSP_HEADER_STATUS_CODE_DATA_SUBJECT_CODE_PATH  "header.functionExecutionStatus.statusCodeData.subjectCode"
#define JSON_HTTP_RSP_HEADER_STATUS_CODE_DATA_REASON_CODE_PATH   "header.functionExecutionStatus.statusCodeData.reasonCode"

#define JSON_INIT_AUTH_CMD_EUICCCHANGE_PATH                      "euiccChallenge"
#define JSON_INIT_AUTH_CMD_SVN_PATH_PATH                         "svn"
#define JSON_INIT_AUTH_CMD_CERTINFO_PATH                         "certificateInfo"
#define JSON_INIT_AUTH_CMD_EUICCINFO_PATH                        "euiccInfo1"
#define JSON_INIT_AUTH_CMD_EUICCINFO_PATH_PH2                    "eUICCInfo1"
#define JSON_INIT_AUTH_CMD_SMDPADDRESS_PATH                      "smdpAddress"

#define JSON_INIT_AUTH_RSP_TRANSACTIONID_PATH                    "transactionId"
#define JSON_INIT_AUTH_RSP_DPSIGNED1_PATH                        "dpSigned1"
#define JSON_INIT_AUTH_RSP_SERVERSIGNED1_PATH                    "serverSigned1"
#define JSON_INIT_AUTH_RSP_SMDPSIGNATURE1_PATH                   "smdpSignature1"
#define JSON_INIT_AUTH_RSP_SERVERSIGNATURE1_PATH                 "serverSignature1"
#define JSON_INIT_AUTH_RSP_CERTFORMATTOBEUSED_PATH               "certFormatTobeUsed"
#define JSON_INIT_AUTH_RSP_CURVETOBEUSED_PATH                    "curveTobeUsed"
#define JSON_INIT_AUTH_RSP_EUICC_CI_PK_TO_BE_USED_PATH           "euiccCiPKIdToBeUsed"
#define JSON_INIT_AUTH_RSP_SMDPCERTIFICATE_PATH                  "smdpCertificate"
#define JSON_INIT_AUTH_RSP_SERVERCERTIFICATE_PATH                "serverCertificate"

#define JSON_GET_BPP_CMD_TRANSACTIONID_PATH                      "transactionId"
#define JSON_GET_BPP_CMD_EUICCPREPAREDOWNLOADRESPONSE_PATH_PH1   "euiccPrepareDownloadResponse"
#define JSON_GET_BPP_CMD_EUICCPREPAREDOWNLOADRESPONSE_PATH_PH1_1_up "prepareDownloadResponse"

#define JSON_AUTH_CLIENT_RSP_TRANSACTIONID_PATH                  "transactionId"
#define JSON_AUTH_CLIENT_RSP_PROFILE_METADATA_PATH               "profileMetadata"
#define JSON_AUTH_CLIENT_RSP_SMDPSIGNED2_PATH                    "smdpSigned2"
#define JSON_AUTH_CLIENT_RSP_SMDPSIGNATURE2_PATH                 "smdpSignature2"
#define JSON_AUTH_CLIENT_RSP_SMDPCERTIFICATE_PATH                "smdpCertificate"

#define JSON_AUTH_CLIENT_CMD_TRANSACTIONID_PATH                  "transactionId"
#define JSON_AUTH_CLIENT_CMD_AUTH_SERVER_RESP_PATH               "authenticateServerResponse"

#define JSON_HANDLE_NOTIF_CMD_PENDING_NOTIF_PATH                 "pendingNotification"

#define JSON_CANCEL_SESSION_CMD_TRANSACTIONID_PATH               "transactionId"
#define JSON_CANCEL_SESSION_CMD_CANCEL_SESSION_RESP_PATH         "cancelSessionResponse"

#define JSON_GET_BPP_RSP_BOUNDPROFILEPACKAGE_PATH                "boundProfilePackage"

#define JSON_HANDLE_INSTALL_RESULT_RESULT_PATH                   "result"

/* ==========================================================================
FUNCTION LPA_JSON_UTIL_FORMAT_SVN_TO_STRING

DESCRIPTION:
  Add '.' seperater for SVN

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
==========================================================================*/
static void lpa_json_util_format_svn_to_string (
  const uint8*       svn_number_array,
  uint8              svn_number_array_len,
  char*              svn_formatted_string,
  uint8              svn_formatted_string_max_len
)
{
  uint8 i             = 0;
  uint8 j             = 0;

  if (NULL        == svn_number_array     ||
      LPA_SVN_LEN != svn_number_array_len ||
      NULL        == svn_formatted_string)
  {
    return;
  }

  /* Populate the svn string */
  for (i = 0; (i < LPA_SVN_LEN); i++) 
  {
    /* Check j index for svn_formatted_string to ensure it has enough space for the encoded digits
       and the following '.' or NULL byte
       NOTE:svn_formatted_string_max_len includes the last NULL byte */
    if (svn_number_array[i] <= 9 && (j + 1) < svn_formatted_string_max_len) 
    {
      svn_formatted_string[j++]     = (char)(svn_number_array[i] + 
                                             LPA_JSON_ASCII_HEXADICIMALBASE);
    }
    else if (svn_number_array[i] <= 99 && (j + 2) < svn_formatted_string_max_len) 
    {
      uint8 digit_10                = 0;

      digit_10                      = svn_number_array[i] / 10;
      svn_formatted_string[j++]     = (char)(digit_10 + LPA_JSON_ASCII_HEXADICIMALBASE);
      svn_formatted_string[j++]     = (char)((svn_number_array[i] - digit_10 * 10)  
                                              + LPA_JSON_ASCII_HEXADICIMALBASE);
    }
    else if ((j + 3) < svn_formatted_string_max_len)
    {
      uint8 digit_100               = 0;
      uint8 digit_10                = 0;

      digit_100                     = svn_number_array[i] / 100;
      svn_formatted_string[j++]     = (char)(digit_100 + LPA_JSON_ASCII_HEXADICIMALBASE);
      digit_10                      = (svn_number_array[i] - digit_100 * 100) / 10;
      svn_formatted_string[j++]     = (char)(digit_10 + LPA_JSON_ASCII_HEXADICIMALBASE);
      svn_formatted_string[j++]     = (char)((svn_number_array[i] - digit_100 * 100 - digit_10 * 10)  
                                              + LPA_JSON_ASCII_HEXADICIMALBASE);
    }
    else
    {
      /* No space in svn_formatted_string for the coded string and the following '.' or NULL
         This won't be a valid SVN, hence set the 1st byte as NULL to make it a valid string
         with size 0*/
      svn_formatted_string[0] = LPA_JSON_ASCII_NULL;
      return;
    }

    if ((LPA_SVN_LEN - 1) != i) 
    {
      /* If not the last digit add the '.' */
      svn_formatted_string[j++] = (char)LPA_JSON_ASCII_DOT;
    }
    else
  {
      /* Add the last NULL byte */
      svn_formatted_string[j++] = (char)LPA_JSON_ASCII_NULL;
    }
  }
}/* lpa_json_util_format_svn_to_string */


/* ==========================================================================
FUNCTION LPA_JSON_UTIL_ALLOC_ASCIISTRING_TO_BIN

DESCRIPTION:
  Allocates memory and converts a ASCII string to a binary
  datastream.

DEPENDENCIES:
  Caller needs to free the return pointer

RETURN VALUE:
  Decoded binary array

SIDE EFFECTS:
  None
==========================================================================*/
static uint8* lpa_json_util_alloc_ascii_string_to_bin (
  const char                *input_ptr,
  uint32                    *output_len_ptr
)
{
  uint32 i = 0;
  uint8* decode_buffer;

  if (NULL == input_ptr || NULL == output_len_ptr)
  {
    return NULL;
  }

  *output_len_ptr = strlen(input_ptr)/2;
  decode_buffer   = LPA_MALLOC(sizeof(uint8) * (*output_len_ptr));
  if (NULL == decode_buffer)
  {
    /* malloc failed */
    return NULL;
  }

  while( i < strlen(input_ptr) &&
         input_ptr[i] >= LPA_JSON_ASCII_HEXADICIMALBASE_0_to_9 &&
         input_ptr[i+1] >= LPA_JSON_ASCII_HEXADICIMALBASE_0_to_9 )
  {
    if (input_ptr[i] <= (LPA_JSON_ASCII_HEXADICIMALBASE_0_to_9 + 9))
    {
      decode_buffer[i/2] = (uint8)((input_ptr[i] - LPA_JSON_ASCII_HEXADICIMALBASE_0_to_9) << 4);
    }
    else if (input_ptr[i] < (LPA_JSON_ASCII_HEXADICIMALBASE_A_to_F + 10 + 6))
    {
      decode_buffer[i/2] = (uint8)((input_ptr[i] - LPA_JSON_ASCII_HEXADICIMALBASE_A_to_F) << 4);
    }

    if (input_ptr[i + 1] <= (LPA_JSON_ASCII_HEXADICIMALBASE_0_to_9  + 9))
    {
      decode_buffer[i/2] += (uint8)(input_ptr[i + 1] - LPA_JSON_ASCII_HEXADICIMALBASE_0_to_9);
    }
    else if (input_ptr[i] < (LPA_JSON_ASCII_HEXADICIMALBASE_A_to_F + 10 + 6))
    {
      decode_buffer[i/2] += (uint8)(input_ptr[i + 1] - LPA_JSON_ASCII_HEXADICIMALBASE_A_to_F);
    }
    i += 2;
  }

  return decode_buffer;
}/*lpa_json_util_alloc_ASCIIstring_to_bin*/


/*===========================================================================
FUNCTION LPA_JSON_INITIATE_AUTH_DECODER

DESCRIPTION
  This function parse the ES9+.InitiateAuthentication response JSON string.
  Then populate the json_initiate_auth_ptr passed in from LPA caller

DEPENDENCIES
  Caller need to free all inter pointers in json_initiate_auth_ptr

RETURN VALUE
  Decoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_initiate_auth_decoder (
  lpa_data_type                       * json_data_ptr,
  lpa_lpd_initiate_auth_resp_type     * json_initiate_auth_ptr,
  lpa_slot_id_enum_type                 slot_id
)
{
  ds_json_handler*              json_handler_ptr   = NULL;
  ds_json_value*                json_value_obj_ptr = NULL;
  ds_json_value_info_s          json_value_info;
  lpa_euicc_phase_enum_type     euicc_phase        = LPA_PHASE1_EUICC;

  if( NULL == json_initiate_auth_ptr ||
      NULL == json_data_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));

  if (LPA_SUCCESS != lpa_retrieve_euicc_phase(slot_id, &euicc_phase))
  {
    return LPA_GENERIC_ERROR;
  }
  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  if( DS_JSON_SUCCESS != ds_json_parse_from_content(
                              json_handler_ptr, (const char*)json_data_ptr->data_ptr,
                              json_data_ptr->data_len))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                 JSON_HTTP_RSP_HEADER_STATUS_PATH, 
                                 &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  if (!strcmp(JSON_HTTP_RSP_HEADER_STATUS_EXECUTED_SUCCESS, 
             json_value_info.value.string_value.json_str))
  {
     json_initiate_auth_ptr->http_response = LPA_SUCCESS;
  }
  else
  {
     json_initiate_auth_ptr->http_response = LPA_ERROR_SMDP_RESPONSE;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                       JSON_INIT_AUTH_RSP_TRANSACTIONID_PATH, 
                                                       &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  /* Save the transaction Id string directly to transaction_id_string*/
  json_initiate_auth_ptr->transaction_id_string.data_len = 
    json_value_info.value.string_value.json_str_length;
  json_initiate_auth_ptr->transaction_id_string.data_ptr = 
    LPA_MALLOC(json_initiate_auth_ptr->transaction_id_string.data_len);
  if (NULL == json_initiate_auth_ptr->transaction_id_string.data_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  memscpy(json_initiate_auth_ptr->transaction_id_string.data_ptr,
          json_initiate_auth_ptr->transaction_id_string.data_len,
          json_value_info.value.string_value.json_str,
          json_value_info.value.string_value.json_str_length);

  /* Convert the transaction Id to data and save in transaction_id*/
  json_initiate_auth_ptr->transaction_id.data_ptr = 
    lpa_json_util_alloc_ascii_string_to_bin(json_value_info.value.string_value.json_str,
                                            &(json_initiate_auth_ptr->transaction_id.data_len));

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if (LPA_PHASE1_EUICC == euicc_phase ||
      LPA_PHASE1_1_EUICC == euicc_phase)
  {
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_DPSIGNED1_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
  }
  else
  {
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_SERVERSIGNED1_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
  }
  json_initiate_auth_ptr->dp_signed1.data_ptr = 
    uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                              json_value_info.value.string_value.json_str_length,
                                              &(json_initiate_auth_ptr->dp_signed1.data_len));


  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if (LPA_PHASE1_EUICC == euicc_phase ||
      LPA_PHASE1_1_EUICC == euicc_phase)
  {
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_SMDPSIGNATURE1_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
  }
  else
  {
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_SERVERSIGNATURE1_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
  }
  json_initiate_auth_ptr->smdp_signature1.data_ptr = 
    uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                              json_value_info.value.string_value.json_str_length,
                                              &(json_initiate_auth_ptr->smdp_signature1.data_len));
  if (LPA_PHASE1_EUICC == euicc_phase ||
      LPA_PHASE1_1_EUICC == euicc_phase)
  {
    memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_CERTFORMATTOBEUSED_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
    json_initiate_auth_ptr->cert_format_to_be_used.data_ptr = 
      uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                                json_value_info.value.string_value.json_str_length,
                                                &(json_initiate_auth_ptr->cert_format_to_be_used.data_len));

    memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_CURVETOBEUSED_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
    json_initiate_auth_ptr->curve_to_be_used.data_ptr = 
      uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                                json_value_info.value.string_value.json_str_length,
                                                &(json_initiate_auth_ptr->curve_to_be_used.data_len));
    memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_SMDPCERTIFICATE_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
    json_initiate_auth_ptr->smdp_certificate.data_ptr = 
      uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                                json_value_info.value.string_value.json_str_length,
                                                &(json_initiate_auth_ptr->smdp_certificate.data_len));
  }
  else
  {
    memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_EUICC_CI_PK_TO_BE_USED_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
    json_initiate_auth_ptr->euicc_ci_pk_id_to_be_used.data_ptr = 
      uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                                json_value_info.value.string_value.json_str_length,
                                                &(json_initiate_auth_ptr->euicc_ci_pk_id_to_be_used.data_len));
    memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                         JSON_INIT_AUTH_RSP_SERVERCERTIFICATE_PATH, 
                                                         &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
    json_initiate_auth_ptr->smdp_certificate.data_ptr = 
      uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                                json_value_info.value.string_value.json_str_length,
                                                &(json_initiate_auth_ptr->smdp_certificate.data_len));
  }

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);
  return LPA_SUCCESS;
} /* lpa_json_initiate_auth_decoder */


/*===========================================================================
FUNCTION LPA_JSON_GET_BOUND_PROFILE_PACKAGE_DECODER

DESCRIPTION
  This function parse the ES9+.GetBoundProfilePackage response JSON string.
  Then populate the json_get_bound_profile_package_ptr passed in from LPA caller

DEPENDENCIES
  None

RETURN VALUE
  Decoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_get_bound_profile_package_decoder (
  lpa_data_type*             json_data_ptr,
  lpa_lpd_get_bpp_resp_type* json_get_bound_profile_package_ptr
)
{
  ds_json_handler*     json_handler_ptr         = NULL;
  ds_json_value*       json_value_obj_ptr       = NULL;
  ds_json_value_info_s json_value_info;

  if(NULL == json_get_bound_profile_package_ptr ||
     NULL == json_data_ptr)
  {
    /* No data return pointer provided */
    return LPA_GENERIC_ERROR;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));

  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  if( DS_JSON_SUCCESS != ds_json_parse_from_content(json_handler_ptr, 
                                                    (const char*)json_data_ptr->data_ptr, 
                                                    json_data_ptr->data_len))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                       JSON_HTTP_RSP_HEADER_STATUS_PATH, 
                                                       &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  if(!strcmp(JSON_HTTP_RSP_HEADER_STATUS_EXECUTED_SUCCESS, 
            json_value_info.value.string_value.json_str))
  {
     json_get_bound_profile_package_ptr->http_response = LPA_SUCCESS;
  }
  else
  {
     json_get_bound_profile_package_ptr->http_response = LPA_ERROR_SMDP_RESPONSE;
  }

  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                                       JSON_GET_BPP_RSP_BOUNDPROFILEPACKAGE_PATH, 
                                                       &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_get_bound_profile_package_ptr->bpp.data_ptr = 
    uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                              json_value_info.value.string_value.json_str_length,
                                              &(json_get_bound_profile_package_ptr->bpp.data_len));

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);

  return LPA_SUCCESS;
} /* lpa_json_get_bound_profile_package_decoder */


/*===========================================================================
FUNCTION LPA_JSON_AUTHENTICATE_CLIENT_DECODER

DESCRIPTION
  This function parse the ES9+.AuthenticateClient response JSON string.
  Then populate the lpa_lpd_authenticate_client_resp_type passed in from
  LPA caller

DEPENDENCIES
  Caller need to free all inter pointers in json_authenticate_client_resp_ptr

LIMITATIONS
  Parsing status

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_authenticate_client_decoder (
  lpa_data_type*                         json_data_ptr,
  lpa_lpd_authenticate_client_resp_type* json_authenticate_client_resp_ptr,
  lpa_slot_id_enum_type                  slot_id
)
{
  ds_json_handler*          json_handler_ptr   = NULL;
  ds_json_value*            json_value_obj_ptr = NULL;
  ds_json_value_info_s      json_value_info;
  lpa_euicc_phase_enum_type euicc_phase        = LPA_PHASE1_EUICC;

  if(NULL == json_authenticate_client_resp_ptr ||
     NULL == json_data_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));

  if (LPA_SUCCESS != lpa_retrieve_euicc_phase(slot_id, &euicc_phase))
  {
    return LPA_GENERIC_ERROR;
  }

  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  if(DS_JSON_SUCCESS != ds_json_parse_from_content(
                         json_handler_ptr, (const char*)json_data_ptr->data_ptr,
                         json_data_ptr->data_len))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /* Check if "Insufficient Memory" error is returned as authenticateClientError
      in response of ES9+.AuthenticateClient */
  if (LPA_PHASE2_1_EUICC == euicc_phase)
  {
    memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
    if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                   JSON_HTTP_RSP_HEADER_STATUS_PATH, 
                                   &json_value_info))
    {
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }

    if (!strcmp(JSON_HTTP_RSP_HEADER_STATUS_EXECUTED_FAILED, 
               json_value_info.value.string_value.json_str))
    {
      memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
      if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                     JSON_HTTP_RSP_HEADER_STATUS_CODE_DATA_SUBJECT_CODE_PATH, 
                                     &json_value_info))
      {
        ds_json_destroy_handler(json_handler_ptr);
        return LPA_GENERIC_ERROR;
      }

      if (!strcmp(JSON_HTTP_RSP_HEADER_STATUS_DATA_SUBJECT_CODE_EUICC, 
                 json_value_info.value.string_value.json_str))
      {
        memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
        if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr, 
                                       JSON_HTTP_RSP_HEADER_STATUS_CODE_DATA_REASON_CODE_PATH, 
                                       &json_value_info))
        {
          ds_json_destroy_handler(json_handler_ptr);
          return LPA_GENERIC_ERROR;
        }

        if (!strcmp(JSON_HTTP_RSP_HEADER_STATUS_DATA_REASON_CODE_INSUFFICIENT_MEMORY, 
                   json_value_info.value.string_value.json_str))
        {
          ds_json_destroy_handler(json_handler_ptr);
          return LPA_ERROR_INSUFFICIENT_MEMORY_ON_EUICC;
        }
      }
    }
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr,
                                                       JSON_AUTH_CLIENT_RSP_TRANSACTIONID_PATH,
                                                       &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  /* Save the transaction Id string directly to transaction_id_string*/
  json_authenticate_client_resp_ptr->transaction_id_string.data_len = 
    json_value_info.value.string_value.json_str_length;
  json_authenticate_client_resp_ptr->transaction_id_string.data_ptr = 
    LPA_MALLOC(json_authenticate_client_resp_ptr->transaction_id_string.data_len);
  if(NULL == json_authenticate_client_resp_ptr->transaction_id_string.data_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  memscpy(json_authenticate_client_resp_ptr->transaction_id_string.data_ptr,
          json_authenticate_client_resp_ptr->transaction_id_string.data_len,
          json_value_info.value.string_value.json_str,
          json_value_info.value.string_value.json_str_length);

  json_authenticate_client_resp_ptr->transaction_id.data_ptr =
    lpa_json_util_alloc_ascii_string_to_bin(
      json_value_info.value.string_value.json_str,
      &(json_authenticate_client_resp_ptr->transaction_id.data_len));

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr,
                                                       JSON_AUTH_CLIENT_RSP_PROFILE_METADATA_PATH,
                                                       &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_authenticate_client_resp_ptr->profile_metadata.data_ptr =
    uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                              json_value_info.value.string_value.json_str_length,
                                              &(json_authenticate_client_resp_ptr->profile_metadata.data_len));

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr,
                                                       JSON_AUTH_CLIENT_RSP_SMDPSIGNED2_PATH,
                                                       &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_authenticate_client_resp_ptr->smdp_signed2.data_ptr =
  uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                            json_value_info.value.string_value.json_str_length,
                                            &(json_authenticate_client_resp_ptr->smdp_signed2.data_len));

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr,
                                                       JSON_AUTH_CLIENT_RSP_SMDPSIGNATURE2_PATH,
                                                       &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_authenticate_client_resp_ptr->smdp_signature2.data_ptr =
  uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                            json_value_info.value.string_value.json_str_length,
                                            &(json_authenticate_client_resp_ptr->smdp_signature2.data_len));

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if(DS_JSON_SUCCESS != ds_json_get_value_info_at_path(json_value_obj_ptr,
                                                       JSON_AUTH_CLIENT_RSP_SMDPCERTIFICATE_PATH,
                                                       &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_authenticate_client_resp_ptr->smdp_certificate.data_ptr =
  uim_common_util_alloc_base64string_to_bin(json_value_info.value.string_value.json_str,
                                            json_value_info.value.string_value.json_str_length,
                                            &(json_authenticate_client_resp_ptr->smdp_certificate.data_len));

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);
  return LPA_SUCCESS;
}/*lpa_json_authenticate_client_decoder*/


/*===========================================================================
FUNCTION LPA_JSON_INITIATE_AUTH_ENCODER

DESCRIPTION
  This function encode the ES9+.InitiateAuthentication command to JSON string.

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_initiate_auth_encoder (
  lpa_data_type*                   json_data_ptr,
  lpa_http_initiate_auth_req_type* json_initiate_auth_ptr
)
{
  ds_json_handler*               json_handler_ptr       = NULL;
  ds_json_value*                 json_value_obj_ptr     = NULL;
  ds_json_value_info_s           json_value_info;
  ds_json_serialize_content_s    json_serialize_content;
  char                           svn_formatted_string[LPA_JSON_SVN_FORMATED_STR_MAX_LEN];
  char*                          temp_base64string      = NULL;
  lpa_euicc_phase_enum_type      euicc_phase            = LPA_PHASE1_EUICC;
  int16                          json_response          = DS_JSON_SUCCESS;

  if(NULL == json_initiate_auth_ptr ||
     NULL == json_data_ptr)
  {
    /* No input pointer provided */
    return LPA_GENERIC_ERROR;
  }

  memset(&json_serialize_content, 0, sizeof(ds_json_serialize_content_s));
  memset(svn_formatted_string, 0, LPA_JSON_SVN_FORMATED_STR_MAX_LEN * sizeof(char));

  /* Retrieve the eUICC phase */
  if(LPA_SUCCESS != lpa_retrieve_euicc_phase(json_initiate_auth_ptr->slot_id,
                                            &euicc_phase))
  {
    LPA_MSG_ERR_1("Invalid or unsupported phase conformance, phase=0x%x", euicc_phase);
    return LPA_GENERIC_ERROR;
  }

  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    LPA_MSG_ERR_0("ds_json_get_root failed");
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));

  temp_base64string =
    uim_common_util_alloc_bin_to_base64string(json_initiate_auth_ptr->euicc_challenge,
                                              LPA_JSON_EUICC_CHANLLENGE_SIZE);
  if(NULL == temp_base64string)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_value_info.type = DS_JSON_TYPE_STRING;
  json_value_info.value.string_value.json_str        = temp_base64string;
  json_value_info.value.string_value.json_str_length = strlen(temp_base64string);

  json_response =
    ds_json_set_value_at_path(json_value_obj_ptr,
                              JSON_INIT_AUTH_CMD_EUICCCHANGE_PATH,
                              &json_value_info);

  if(DS_JSON_SUCCESS != json_response)
  {
    LPA_MSG_ERR_0("ds_json_set_value_at_path failed");
    ds_json_destroy_handler(json_handler_ptr);
    LPA_FREE(temp_base64string);
    return LPA_GENERIC_ERROR;
  }
  LPA_FREE(temp_base64string);

  if(LPA_PHASE1_EUICC == euicc_phase ||
     LPA_PHASE1_1_EUICC == euicc_phase)
  {
    memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
    lpa_json_util_format_svn_to_string(json_initiate_auth_ptr->svn,
                                       LPA_SVN_LEN,
                                       svn_formatted_string,
                                       LPA_JSON_SVN_FORMATED_STR_MAX_LEN);
    json_value_info.type = DS_JSON_TYPE_STRING;
    json_value_info.value.string_value.json_str        = svn_formatted_string;
    json_value_info.value.string_value.json_str_length = strlen(svn_formatted_string);
    if(DS_JSON_SUCCESS !=  ds_json_set_value_at_path(json_value_obj_ptr,
                                                     JSON_INIT_AUTH_CMD_SVN_PATH_PATH,
                                                     &json_value_info))
    {
      LPA_MSG_ERR_0("ds_json_set_value_at_path failed");
      ds_json_destroy_handler(json_handler_ptr);
      return LPA_GENERIC_ERROR;
    }
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  json_value_info.type = DS_JSON_TYPE_STRING;

  if(euicc_phase == LPA_PHASE1_EUICC)
  {
    /* For PHASE 1, encode certificate info */
    temp_base64string =
        uim_common_util_alloc_bin_to_base64string(json_initiate_auth_ptr->cert_info.data_ptr,
                                                  json_initiate_auth_ptr->cert_info.data_len);
  }
  else
  {
    /* For PHASE 1.1 or greater, encode entire eUICCInfo*/
    temp_base64string =
      uim_common_util_alloc_bin_to_base64string(json_initiate_auth_ptr->euicc_info1.data_ptr,
                                                json_initiate_auth_ptr->euicc_info1.data_len);
  }
  if(NULL == temp_base64string)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_value_info.value.string_value.json_str        = temp_base64string;
  json_value_info.value.string_value.json_str_length = strlen(temp_base64string);
  if(LPA_PHASE1_EUICC == euicc_phase)
  {
    json_response =
      ds_json_set_value_at_path(json_value_obj_ptr,
                                JSON_INIT_AUTH_CMD_CERTINFO_PATH,
                                &json_value_info);
  }
  else if(LPA_PHASE1_1_EUICC == euicc_phase|| LPA_PHASE2_EUICC == euicc_phase ||
          LPA_PHASE2_1_EUICC == euicc_phase)
  {
    json_response =
      ds_json_set_value_at_path(json_value_obj_ptr,
                                JSON_INIT_AUTH_CMD_EUICCINFO_PATH,
                                &json_value_info);
  }
  else
  {
    LPA_FREE(temp_base64string);
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  if(DS_JSON_SUCCESS != json_response)
  {
    LPA_FREE(temp_base64string);
    LPA_MSG_ERR_0("ds_json_set_value_at_path failed");
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  LPA_FREE(temp_base64string);

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  json_value_info.type = DS_JSON_TYPE_STRING;
  json_value_info.value.string_value.json_str        = json_initiate_auth_ptr->smdp_address.data;
  json_value_info.value.string_value.json_str_length = json_initiate_auth_ptr->smdp_address.data_len;
  ds_json_set_value_at_path(json_value_obj_ptr,
                            JSON_INIT_AUTH_CMD_SMDPADDRESS_PATH,
                            &json_value_info);

  if(DS_JSON_SUCCESS != ds_json_serialize_to_content(json_handler_ptr,
                                                     &json_serialize_content))
  {
    LPA_MSG_ERR_0("ds_json_serialize_to_content failed");
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  json_data_ptr->data_ptr = LPA_MALLOC(json_serialize_content.content_size);
  if(json_data_ptr->data_ptr == NULL)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  json_data_ptr->data_len = json_serialize_content.content_size;
  memscpy(json_data_ptr->data_ptr,
          json_data_ptr->data_len,
          json_serialize_content.content,
          json_serialize_content.content_size);

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);

  return LPA_SUCCESS;
} /* lpa_json_initiate_auth_encoder */


/*===========================================================================
FUNCTION LPA_JSON_GET_BOUND_PROFILE_PACKAGE_ENCODER

DESCRIPTION
  This function encode the ES9+.GetBoundProfilePackage command to JSON string.

DEPENDENCIES
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_get_bound_profile_package_encoder (
  lpa_data_type*               json_data_ptr,
  lpa_http_get_bpp_req_type*   json_get_bpp_ptr
)
{
  ds_json_handler*               json_handler_ptr         = NULL;
  ds_json_value*                 json_value_obj_ptr       = NULL;
  ds_json_value_info_s           json_value_info;
  ds_json_serialize_content_s    json_serialize_content;
  int16                          json_response            = DS_JSON_SUCCESS;
  char*                          temp_base64string        = NULL;
  lpa_euicc_phase_enum_type      euicc_phase              = LPA_PHASE1_EUICC;

  if(NULL == json_get_bpp_ptr ||
     NULL == json_data_ptr)
  {
    /* No input pointer provided */
    return LPA_GENERIC_ERROR;
  }

  /* Retrieve the eUICC phase */
  if(LPA_SUCCESS != lpa_retrieve_euicc_phase(json_get_bpp_ptr->slot_id,
                                             &euicc_phase))
  {
    LPA_MSG_ERR_1("Invalid or unsupported phase conformance, phase=0x%x", euicc_phase);
    return LPA_GENERIC_ERROR;
  }

  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  json_value_info.type = DS_JSON_TYPE_STRING;

  json_value_info.value.string_value.json_str        = (char*)json_get_bpp_ptr->transaction_id_data.data_ptr;
  json_value_info.value.string_value.json_str_length = json_get_bpp_ptr->transaction_id_data.data_len;
  if(DS_JSON_SUCCESS != ds_json_set_value_at_path(json_value_obj_ptr,
                                                  JSON_GET_BPP_CMD_TRANSACTIONID_PATH,
                                                  &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  temp_base64string = uim_common_util_alloc_bin_to_base64string(json_get_bpp_ptr->prepare_download_data.data_ptr, 
                                                                json_get_bpp_ptr->prepare_download_data.data_len);
  if(NULL == temp_base64string)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_value_info.type = DS_JSON_TYPE_STRING;
  json_value_info.value.string_value.json_str        = temp_base64string;
  json_value_info.value.string_value.json_str_length = strlen(temp_base64string);

  if(euicc_phase == LPA_PHASE1_EUICC)
  {
    json_response =
      ds_json_set_value_at_path(json_value_obj_ptr,
                                JSON_GET_BPP_CMD_EUICCPREPAREDOWNLOADRESPONSE_PATH_PH1,
                                &json_value_info);
  }
  else
  {
    json_response =
      ds_json_set_value_at_path(json_value_obj_ptr,
                                JSON_GET_BPP_CMD_EUICCPREPAREDOWNLOADRESPONSE_PATH_PH1_1_up,
                                &json_value_info);
  }

  LPA_FREE(temp_base64string);

  if(DS_JSON_SUCCESS != json_response)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  if(DS_JSON_SUCCESS != ds_json_serialize_to_content(json_handler_ptr,
                                                    &json_serialize_content))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  json_data_ptr->data_len = json_serialize_content.content_size;
  json_data_ptr->data_ptr = LPA_MALLOC(json_data_ptr->data_len);
  if(json_data_ptr->data_ptr == NULL)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  memscpy(json_data_ptr->data_ptr,
          json_data_ptr->data_len,
          json_serialize_content.content,
          json_serialize_content.content_size);

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);

  return LPA_SUCCESS;
} /* lpa_json_get_bound_profile_package_encoder */


/*===========================================================================
FUNCTION LPA_JSON_HANDLE_PROFILE_INSTALL_RESULT_ENCODER

DESCRIPTION
  This function encode the ES9+.HandleProfileInstallationResult command to JSON string.

DEPENDENCIES
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_handle_profile_install_result_encoder (
  lpa_data_type*                                json_data_ptr,
  lpa_http_handle_installation_result_req_type* json_handle_profile_install_result_ptr
)
{
  ds_json_handler*               json_handler_ptr         = NULL;
  ds_json_value*                 json_value_obj_ptr       = NULL;
  ds_json_value_info_s           json_value_info;
  ds_json_serialize_content_s    json_serialize_content;
  char*                          temp_base64string        = NULL;

  if(NULL == json_handle_profile_install_result_ptr ||
     NULL == json_data_ptr)
  {
    /* No input pointer provided */
    return LPA_GENERIC_ERROR;
  }

  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  json_value_info.type = DS_JSON_TYPE_STRING;
  temp_base64string = uim_common_util_alloc_bin_to_base64string(json_handle_profile_install_result_ptr->profile_installation_result.data_ptr,
                                                                json_handle_profile_install_result_ptr->profile_installation_result.data_len);
  if(NULL == temp_base64string)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_value_info.value.string_value.json_str        = temp_base64string;
  json_value_info.value.string_value.json_str_length = strlen(temp_base64string);
  if(DS_JSON_SUCCESS != ds_json_set_value_at_path(json_value_obj_ptr,
                                                  JSON_HANDLE_INSTALL_RESULT_RESULT_PATH,
                                                  &json_value_info))
  {
    LPA_FREE(temp_base64string);
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  LPA_FREE(temp_base64string);

  if(DS_JSON_SUCCESS != ds_json_serialize_to_content(json_handler_ptr,
                                                     &json_serialize_content))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_data_ptr->data_ptr = LPA_MALLOC(json_serialize_content.content_size);
  if(json_data_ptr->data_ptr == NULL)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  json_data_ptr->data_len = json_serialize_content.content_size;
  memscpy(json_data_ptr->data_ptr,
          json_data_ptr->data_len,
          json_serialize_content.content,
          json_serialize_content.content_size);

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);

  return LPA_SUCCESS;
} /* lpa_json_handle_profile_install_result_encoder */


/*===========================================================================
FUNCTION LPA_JSON_AUTHENTICATE_CLIENT_ENCODER

DESCRIPTION
  This function encode the ES9+.AuthenticateClient object to JSON to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_authenticate_client_encoder (
  lpa_data_type*                               json_data_ptr,
  lpa_http_authenticate_client_req_type*       json_authenticate_client_req_ptr
)
{
  ds_json_handler*               json_handler_ptr         = NULL;
  ds_json_value*                 json_value_obj_ptr       = NULL;
  ds_json_value_info_s           json_value_info;
  ds_json_serialize_content_s    json_serialize_content;
  char*                          temp_base64string        = NULL;

  if(NULL == json_authenticate_client_req_ptr ||
     NULL == json_data_ptr)
  {
    /* No input pointer provided */
    return LPA_GENERIC_ERROR;
  }

  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /* add transactionId */
  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  json_value_info.type = DS_JSON_TYPE_STRING;

  json_value_info.value.string_value.json_str        = 
    (char*)json_authenticate_client_req_ptr->transaction_id_data.data_ptr;
  json_value_info.value.string_value.json_str_length = 
    json_authenticate_client_req_ptr->transaction_id_data.data_len;
  if(DS_JSON_SUCCESS != ds_json_set_value_at_path(json_value_obj_ptr,
                                                  JSON_AUTH_CLIENT_CMD_TRANSACTIONID_PATH,
                                                  &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /*add authenticateServerResponse*/
  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  temp_base64string =
      uim_common_util_alloc_bin_to_base64string(json_authenticate_client_req_ptr->authenticate_server_response.data_ptr,
                                                json_authenticate_client_req_ptr->authenticate_server_response.data_len);
  if(NULL == temp_base64string)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_value_info.type = DS_JSON_TYPE_STRING;
  json_value_info.value.string_value.json_str        = temp_base64string;
  json_value_info.value.string_value.json_str_length = strlen(temp_base64string);

  if(DS_JSON_SUCCESS !=
     ds_json_set_value_at_path(json_value_obj_ptr,
                               JSON_AUTH_CLIENT_CMD_AUTH_SERVER_RESP_PATH,
                               &json_value_info))
  {
    LPA_FREE(temp_base64string);
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  LPA_FREE(temp_base64string);

  if(DS_JSON_SUCCESS != ds_json_serialize_to_content(json_handler_ptr,
                                                     &json_serialize_content))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_data_ptr->data_ptr = LPA_MALLOC(json_serialize_content.content_size);
  if(json_data_ptr->data_ptr == NULL)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  json_data_ptr->data_len = json_serialize_content.content_size;
  memscpy(json_data_ptr->data_ptr,
          json_data_ptr->data_len,
          json_serialize_content.content,
          json_serialize_content.content_size);

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);

  return LPA_SUCCESS;
} /* lpa_json_authenticate_client_encoder */


/*===========================================================================
FUNCTION LPA_JSON_HANDLE_NOTIFICATION_ENCODER

DESCRIPTION
  This function encode the ES9+.HandleNotification object to JSON to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_handle_notification_encoder(
  lpa_data_type*                               json_data_ptr,
  lpa_http_handle_notification_req_type*       json_handle_notification_req_ptr
)
{
  ds_json_handler*               json_handler_ptr         = NULL;
  ds_json_value*                 json_value_obj_ptr       = NULL;
  ds_json_value_info_s           json_value_info;
  ds_json_serialize_content_s    json_serialize_content;
  char*                          temp_base64string        = NULL;

  if(NULL == json_handle_notification_req_ptr ||
     NULL == json_data_ptr)
  {
    /* No input pointer provided */
    return LPA_GENERIC_ERROR;
  }

  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /*add pendingNotification*/
  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  if(json_handle_notification_req_ptr->notification.notification_type == 
     LPA_NOTIFICATION_PROFILE_INSTALLATION_RESULT)
  {
    temp_base64string =
        uim_common_util_alloc_bin_to_base64string(json_handle_notification_req_ptr->notification.data.profile_installation_result.data_ptr,
                                                  json_handle_notification_req_ptr->notification.data.profile_installation_result.data_len);
  }
  else
  {
    temp_base64string =
      uim_common_util_alloc_bin_to_base64string(json_handle_notification_req_ptr->notification.data.other_signed_notification.data_ptr,
                                                json_handle_notification_req_ptr->notification.data.other_signed_notification.data_len);
  }

  if(NULL == temp_base64string)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_value_info.type = DS_JSON_TYPE_STRING;
  json_value_info.value.string_value.json_str        = temp_base64string;
  json_value_info.value.string_value.json_str_length = strlen(temp_base64string);

  if(DS_JSON_SUCCESS !=
     ds_json_set_value_at_path(json_value_obj_ptr,
                               JSON_HANDLE_NOTIF_CMD_PENDING_NOTIF_PATH,
                               &json_value_info))
  {
    LPA_FREE(temp_base64string);
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  LPA_FREE(temp_base64string);

  if(DS_JSON_SUCCESS != ds_json_serialize_to_content(json_handler_ptr,
                                                     &json_serialize_content))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_data_ptr->data_ptr = LPA_MALLOC(json_serialize_content.content_size);
  if(json_data_ptr->data_ptr == NULL)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  json_data_ptr->data_len = json_serialize_content.content_size;
  memscpy(json_data_ptr->data_ptr,
          json_data_ptr->data_len,
          json_serialize_content.content,
          json_serialize_content.content_size);

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);

  return LPA_SUCCESS;
} /* lpa_json_handle_notification_encoder */


/*===========================================================================
FUNCTION LPA_JSON_CANCEL_SESSION_ENCODER

DESCRIPTION
  This function encode the ES9+.CancelSession object to JSON to JSON string

DEPENDENCIES
  Caller is responsible to free the data_ptr in json_data_ptr

LIMITATIONS
  None

RETURN VALUE
  Encoding status

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_json_cancel_session_encoder (
  lpa_data_type*                               json_data_ptr,
  lpa_http_cancel_session_server_req_type*     json_cancel_session_req_ptr
)
{
  ds_json_handler*               json_handler_ptr         = NULL;
  ds_json_value*                 json_value_obj_ptr       = NULL;
  ds_json_value_info_s           json_value_info;
  ds_json_serialize_content_s    json_serialize_content;
  char*                          temp_base64string        = NULL;

  if(NULL == json_cancel_session_req_ptr ||
     NULL == json_data_ptr)
  {
    /* No input pointer provided */
    return LPA_GENERIC_ERROR;
  }

  /* Create a JSON handler */
  json_handler_ptr = ds_json_create_handler();

  if(NULL == json_handler_ptr)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Get root node */
  json_value_obj_ptr = ds_json_get_root(json_handler_ptr);
  if(NULL == json_value_obj_ptr)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /* add transactionId */
  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  json_value_info.type = DS_JSON_TYPE_STRING;

  json_value_info.value.string_value.json_str        = 
    (char*)json_cancel_session_req_ptr->transaction_id.data_ptr;
  json_value_info.value.string_value.json_str_length = 
    json_cancel_session_req_ptr->transaction_id.data_len;
  if(DS_JSON_SUCCESS != ds_json_set_value_at_path(json_value_obj_ptr,
                                                  JSON_CANCEL_SESSION_CMD_TRANSACTIONID_PATH,
                                                  &json_value_info))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  /*add cancelSessionResponse*/
  memset(&json_value_info, 0, sizeof(ds_json_value_info_s));
  temp_base64string =
      uim_common_util_alloc_bin_to_base64string(json_cancel_session_req_ptr->cancel_session_euicc_resp.data_ptr,
                                                json_cancel_session_req_ptr->cancel_session_euicc_resp.data_len);
  if(NULL == temp_base64string)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_value_info.type = DS_JSON_TYPE_STRING;
  json_value_info.value.string_value.json_str        = temp_base64string;
  json_value_info.value.string_value.json_str_length = strlen(temp_base64string);

  if(DS_JSON_SUCCESS !=
     ds_json_set_value_at_path(json_value_obj_ptr,
                               JSON_CANCEL_SESSION_CMD_CANCEL_SESSION_RESP_PATH,
                               &json_value_info))
  {
    LPA_FREE(temp_base64string);
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }

  LPA_FREE(temp_base64string);

  if(DS_JSON_SUCCESS != ds_json_serialize_to_content(json_handler_ptr,
                                                     &json_serialize_content))
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_GENERIC_ERROR;
  }
  json_data_ptr->data_ptr = LPA_MALLOC(json_serialize_content.content_size);
  if(json_data_ptr->data_ptr == NULL)
  {
    ds_json_destroy_handler(json_handler_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  json_data_ptr->data_len = json_serialize_content.content_size;
  memscpy(json_data_ptr->data_ptr,
          json_data_ptr->data_len,
          json_serialize_content.content,
          json_serialize_content.content_size);

  /* cleanup */
  ds_json_destroy_handler(json_handler_ptr);

  return LPA_SUCCESS;
} /* lpa_json_cancel_session_encoder */
