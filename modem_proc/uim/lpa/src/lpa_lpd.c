/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            L P A   L P D   C O M M A N D   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the LPA functions which handles LPA profile
  management requests coming from the user.

                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_lpd.c#5 $ $DateTime: 2021/09/11 17:08:59 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/12/21   skc     Set signal if there are more commands in the queue
04/27/20   sg      Added debug prints for RSP Profile Download KPIs
11/04/19   vdc     Calculate data length of BPP as per ISO/IEC 8825-1
10/23/19   vgd     Set le_present to TRUE when P1 is 91 for phase 2 cards
03/08/19   bcho    Fix issues in profile download on phase 2.1 cards
03/29/18   bcho    LPA phase2.1 changes
04/12/17   av      Add support to configure/retrieve default server address
04/11/17   av      Fixes for RAT decoding etc
01/12/17   av      Bug fixes for phase2 PRRID related logic
12/07/16   av      LPA phase2 initial changes
09/22/16   ll      Peek into profile installation result
06/21/16   av      Add SVN check for supporting phase1, phase1.1 and phase2
05/19/16   ll      ASN1 decoder enhancement
05/15/16   ll      ASN1 encoder code clean up
04/27/16   av      Fixing some compiler warnings and adding more checks
04/27/16   ar      Correcting the array check
04/11/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "queue.h"

#include "lpa.h"
#include "lpa_platform.h"
#include "lpa_cmd.h"
#include "lpalib.h"
#include "lpa_state.h"
#include "lpa_mmgsdi.h"
#include "lpa_util.h"
#include "lpa_lpd.h"
#include "lpa_http.h"
#include "lpa_asn1_encoder.h"
#include "lpa_asn1_decoder.h"
#include "lpa_json.h"
#include <stringl/stringl.h>
#include "lpa_efs.h"
#include "uim_common_lib.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
#define LPA_GET_EUICC_INFO_APDU_DATA_P2                                     0x20
#define LPA_LOAD_BPP_APDU_DATA_P1                                           0x91
#define LPA_PREPARE_DOWNLOAD_APDU_DATA_P1                                   0x91
#define LPA_RECEIPT_RESULT_CODE_BYTE0_INSTALLATION_SUCCESS                  0x00
#define LPA_RECEIPT_RESULT_CODE_BYTE0_INSTALLATION_FAIL                     0x01
#define LPA_RECEIPT_RESULT_CODE_BYTE1_ICCID_EXISTS_PHASE1                   0x01
#define LPA_RECEIPT_RESULT_CODE_BYTE1_EUICC_INSUFFICIENT_MEMORY_PHASE1      0x02
#define LPA_RECEIPT_RESULT_CODE_BYTE1_FAIL_DUE_TO_INTERRUPTION_PHASE1       0x03

#define LPA_RECEIPT_RESULT_CODE_BYTE1_ICCID_EXISTS                          0x09
#define LPA_RECEIPT_RESULT_CODE_BYTE1_EUICC_INSUFFICIENT_MEMORY             0x0A
#define LPA_RECEIPT_RESULT_CODE_BYTE1_FAIL_DUE_TO_INTERRUPTION              0x0B

#define LPA_RECEIPT_RESULT_CODE_BYTE1_UNSUPPORTED_PROFILE_CLASS             0x06
#define LPA_RECEIPT_RESULT_CODE_BYTE1_PPR_NOT_ALLOWED                       0x0C
#define LPA_PROFILE_METADATA_TAG_LEN                                        0x01
#define LPA_PROFILE_METADATA_MAC_LEN                                        0x08

static q_type                             lpa_lpd_cmd_q;

typedef enum {
  LPA_LPD_CANCEL_SESSION_END_USER_REJECTION,
  LPA_LPD_CANCEL_SESSION_POSTPONED,
  LPA_LPD_CANCEL_SESSION_TIMEOUT,
  LPA_LPD_CANCEL_SESSION_PPR_ID_NOT_ALLOWED,
  LPA_LPD_CANCEL_SESSION_META_DATA_MISMATCH,
  LPA_LPD_CANCEL_SESSION_LOAD_BPP_EXECUTION_ERROR
} lpa_lpd_cancel_session_reason_enum_type;

typedef struct {
  lpa_add_profile_method_enum_type        method;
  lpa_smdp_address_type                   default_smdp_address;
  lpa_smdp_address_type                   root_smds_address;
  lpa_activation_code_type                activation_code;
  lpa_data_type                           hashed_confirmation_code;
  lpa_data_type                           hashed_confirmation_code_tx_id;
  uint8                                   euicc_challenge[LPA_EUICC_CHALLENGE_LEN];
  lpa_data_type                           transaction_id;
  lpa_data_type                           transaction_id_string;
  lpa_data_type                           cert_info;
  lpa_data_type                           euicc_info1;
  lpa_get_rat_resp_type                   rat;
  boolean                                 any_operational_profile;
  lpa_util_device_info_type               device_info;
  lpa_bpp_tlvs_data_type                  bpp_tlvs;
  uint32                                  last_loaded_bpp_tlv_counter;
  lpa_data_type                           smdp_signed2;
  lpa_data_type                           smdp_signature2;
  lpa_data_type                           smdp_certificate;
  lpa_lpd_cancel_session_reason_enum_type cancel_session_reason;
  lpa_data_type                           profile_metadata;
} lpa_lpd_info_type;

/* Contains information regarding the add profile procedure */
static lpa_lpd_info_type                  lpa_lpd_global_info[LPA_NUM_SLOTS];

/*===========================================================================

                                  FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION LPA_LPD_SEND_PROFILE_DOWNLOAD_EVENT

DESCRIPTION
  This function sends a profile download event to the client by invoking
  the client event callback.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_lpd_send_profile_download_event (
  lpa_result_enum_type                 status,
  lpa_slot_id_enum_type                slot_id,
  uint8                                percent_downloaded,
  boolean                              installation_complete,
  boolean                              reset_all
)
{
  lpa_event_data_type              download_progress_event;

  memset(&download_progress_event, 0x00, sizeof(download_progress_event));
  download_progress_event.evt_type = LPA_PROFILE_DOWNLOAD_PROGRESS_EVENT;
  download_progress_event.slot_id = slot_id;
  download_progress_event.evt_data.profile_download_progress.percent_downloaded =
    percent_downloaded;
  download_progress_event.evt_data.profile_download_progress.installation_complete =
    installation_complete;

  lpa_send_event(status,
                 &download_progress_event,
                 slot_id,
                 reset_all);
  if(reset_all)
  {
    lpa_http_cleanup(slot_id);
  }
} /* lpa_lpd_send_profile_download_event */


/*===========================================================================
FUNCTION LPA_LPD_REQUEST_USER_CONSENT_EVENT

DESCRIPTION
  This function sends a profile download event to the client by invoking
  the client event callback.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_lpd_request_user_consent_event (
  lpa_result_enum_type                 status,
  lpa_slot_id_enum_type                slot_id,
  lpa_ppr_ids_type                     ppr_ids,
  boolean                              provide_user_consent
)
{
  lpa_event_data_type                  user_consent_event;

  memset(&user_consent_event, 0x00, sizeof(user_consent_event));

  user_consent_event.evt_type = LPA_REQUEST_USER_CONSENT_EVENT;
  user_consent_event.slot_id = slot_id;
  user_consent_event.evt_data.request_user_consent.ppr_ids = ppr_ids;
  user_consent_event.evt_data.request_user_consent.provide_user_consent =
    provide_user_consent;

  lpa_send_event(status,
                 &user_consent_event,
                 slot_id,
                 FALSE);
} /* lpa_lpd_request_user_consent_event */


/*===========================================================================
FUNCTION LPA_LPD_GET_PROFILE_INSTALLATION_RESULT

DESCRIPTION
  Function calls the asn1 installation result decoder and converts the result
  code to LPA status.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_get_profile_installation_result (
  uint32                                             data_len,
  uint8                                             *data_ptr,
  lpa_slot_id_enum_type                              slot_id
)
{
  lpa_lpd_profile_installation_result_code_array_type result_code_array;
  lpa_result_enum_type                                installation_result = LPA_SUCCESS;
  lpa_euicc_phase_enum_type                           euicc_phase         = LPA_PHASE1_1_EUICC;

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(result_code_array, 0x00, LPA_INSTALLATION_RESULT_CODE_LEN);

  (void) lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(euicc_phase == LPA_PHASE1_1_EUICC ||
     euicc_phase == LPA_PHASE1_EUICC)
  {
    if (LPA_SUCCESS != lpa_asn1_decode_profile_installation_result_phase1(data_len,
                                                                          data_ptr,
                                                                          result_code_array))
    {
      installation_result = LPA_GENERIC_ERROR;
    }
    else if (LPA_RECEIPT_RESULT_CODE_BYTE0_INSTALLATION_FAIL == result_code_array[0])
    {
      switch (result_code_array[1])
      {
        case LPA_RECEIPT_RESULT_CODE_BYTE1_ICCID_EXISTS_PHASE1:
          installation_result = LPA_ERROR_ICCID_EXISTS_ON_EUICC;
          break;
        case LPA_RECEIPT_RESULT_CODE_BYTE1_EUICC_INSUFFICIENT_MEMORY_PHASE1:
          installation_result = LPA_ERROR_INSUFFICIENT_MEMORY_ON_EUICC;
          break;
        case LPA_RECEIPT_RESULT_CODE_BYTE1_FAIL_DUE_TO_INTERRUPTION_PHASE1:
          installation_result = LPA_ERROR_PROFILE_INSTALLATION_INTERRUPTED_ON_EUICC;
          break;
        default:
          installation_result = LPA_ERROR_PROFILE_INSTALLATION_UNKNOWN_ERR;
          break;
      }
    }
    else if (LPA_RECEIPT_RESULT_CODE_BYTE0_INSTALLATION_SUCCESS != result_code_array[0])
    {
      installation_result = LPA_GENERIC_ERROR;
    }
  }
  else
  {
    /* Phase 2 */
    if(LPA_SUCCESS != lpa_asn1_decode_profile_installation_result(data_len,
                                                                  data_ptr,
                                                                  result_code_array))
    {
      installation_result = LPA_GENERIC_ERROR;
    }
    else if (LPA_RECEIPT_RESULT_CODE_BYTE0_INSTALLATION_FAIL == result_code_array[0])
    {
      switch (result_code_array[1])
      {
        case LPA_RECEIPT_RESULT_CODE_BYTE1_ICCID_EXISTS:
          installation_result = LPA_ERROR_ICCID_EXISTS_ON_EUICC;
          break;
        case LPA_RECEIPT_RESULT_CODE_BYTE1_EUICC_INSUFFICIENT_MEMORY:
          installation_result = LPA_ERROR_INSUFFICIENT_MEMORY_ON_EUICC;
          break;
        case LPA_RECEIPT_RESULT_CODE_BYTE1_FAIL_DUE_TO_INTERRUPTION:
          installation_result = LPA_ERROR_PROFILE_INSTALLATION_INTERRUPTED_ON_EUICC;
          break;
        case LPA_RECEIPT_RESULT_CODE_BYTE1_UNSUPPORTED_PROFILE_CLASS:
          installation_result = LPA_ERROR_UNSUPPORTED_PROFILE_CLASS;
          break;
        case LPA_RECEIPT_RESULT_CODE_BYTE1_PPR_NOT_ALLOWED:
          installation_result = LPA_ERROR_PROFILE_INSTALLATION_PPR_ID_NOT_ALLOWED;
          break;
        default:
          installation_result = LPA_ERROR_PROFILE_INSTALLATION_UNKNOWN_ERR;
          break;
      }
    }
    else if (LPA_RECEIPT_RESULT_CODE_BYTE0_INSTALLATION_SUCCESS != result_code_array[0])
    {
      installation_result = LPA_GENERIC_ERROR;
    }
  }

  return installation_result;
}/*lpa_lpd_get_profile_installation_result*/


/*===========================================================================
FUNCTION LPA_LPD_CMD_INIT

DESCRIPTION
  This function, called in lpa_main, is called to initialize the LPD command
  module.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_lpd_cmd_init (
  void
)
{
  (void) q_init(&lpa_lpd_cmd_q);
} /* lpa_lpd_cmd_init */


/*===========================================================================
   FUNCTION:      LPA_LPA_FREE_RESPONSE_DATA

   DESCRIPTION:
     This function frees the response pointer that is mallocd (in lpa_mmgsdi or
     in lpa_http) in order to send a response back to LPA LPD via its callback

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
void lpa_lpd_free_response_data(
  lpa_lpd_cmd_data_type                *lpa_response_data_ptr
)
{
  if(lpa_response_data_ptr == NULL)
  {
    return;
  }

  switch(lpa_response_data_ptr->cmd)
  {
    case LPA_LPD_GET_EUICC_CHALLENGE_RESP_CMD:
    case LPA_LPD_HANDLE_INSTALLATION_RESULT_RESP_CMD:
       break;
    case LPA_LPD_GET_EUICC_INFO_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.get_euicc_info_resp.cert_info.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.get_euicc_info_resp.euicc_info1.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.get_euicc_info_resp.euicc_info1.data_ptr);
      break;
    case LPA_LPD_GET_RAT_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.get_rat_resp.rules_auth_table.ppar_entry_ptr);
      break;
    case LPA_LPD_AUTHENTICATE_SERVER_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.authenticate_server_resp.data.data_ptr);
      break;
    case LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.prepare_download_resp.prepare_download.data_ptr);
      break;
    case LPA_LPD_LOAD_BPP_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.load_bpp_resp.load_bpp.data_ptr);
      break;
    case LPA_LPD_INITIATE_AUTH_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.initiate_auth_resp.transaction_id.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.initiate_auth_resp.transaction_id_string.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.initiate_auth_resp.dp_signed1.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.initiate_auth_resp.smdp_signature1.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.initiate_auth_resp.cert_format_to_be_used.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.initiate_auth_resp.curve_to_be_used.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.initiate_auth_resp.smdp_certificate.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.initiate_auth_resp.euicc_ci_pk_id_to_be_used.data_ptr);
      break;
    case LPA_LPD_GET_BPP_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.get_bpp_resp.profile_metadata.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.get_bpp_resp.bpp.data_ptr);
      break;
    case LPA_LPD_CANCEL_SESSION_EUICC_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.cancel_session_euicc_resp.data.data_ptr);
      break;
    case LPA_LPD_GET_EUICC_CONFIG_ADDRESS_RESP_CMD:
      LPA_FREE(lpa_response_data_ptr->data.get_euicc_config_address_resp.default_smdp_address.data_ptr);
      LPA_FREE(lpa_response_data_ptr->data.get_euicc_config_address_resp.default_smds_address.data_ptr);
      break;
    default:
       break;
  }
} /* lpa_lpd_free_response_data */


/*===========================================================================
   FUNCTION:      LPA_LPD_RESET

   DESCRIPTION:
     This function resets/frees any globals in lpa lpd.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
void lpa_lpd_reset (
  lpa_slot_id_enum_type                lpa_slot
)
{
  uint8                   i             = 0;

  LPA_MSG_HIGH_1("lpa_lpd_reset, slot_id=0x%x", lpa_slot);

  if(lpa_slot >= LPA_NUM_SLOTS)
  {
    return;
  }

  LPA_FREE(lpa_lpd_global_info[lpa_slot].hashed_confirmation_code.data_ptr);
  LPA_FREE(lpa_lpd_global_info[lpa_slot].hashed_confirmation_code_tx_id.data_ptr);
  LPA_FREE(lpa_lpd_global_info[lpa_slot].cert_info.data_ptr);
  LPA_FREE(lpa_lpd_global_info[lpa_slot].euicc_info1.data_ptr);
  LPA_FREE(lpa_lpd_global_info[lpa_slot].rat.rules_auth_table.ppar_entry_ptr);
  LPA_FREE(lpa_lpd_global_info[lpa_slot].transaction_id.data_ptr);
  LPA_FREE(lpa_lpd_global_info[lpa_slot].transaction_id_string.data_ptr);
  while(i < (sizeof(lpa_lpd_global_info[lpa_slot].bpp_tlvs.tlvs_data)/sizeof(lpa_lpd_global_info[lpa_slot].bpp_tlvs.tlvs_data[0])) &&
        i < lpa_lpd_global_info[lpa_slot].bpp_tlvs.tlvs_num &&
        lpa_lpd_global_info[lpa_slot].bpp_tlvs.tlvs_data[i].data_ptr)
  {
    LPA_FREE(lpa_lpd_global_info[lpa_slot].bpp_tlvs.tlvs_data[i].data_ptr);
    i++;
  }
  LPA_FREE(lpa_lpd_global_info[lpa_slot].smdp_signed2.data_ptr);
  LPA_FREE(lpa_lpd_global_info[lpa_slot].smdp_signature2.data_ptr);
  LPA_FREE(lpa_lpd_global_info[lpa_slot].smdp_certificate.data_ptr);

  memset(&lpa_lpd_global_info[lpa_slot], 0x00, sizeof(lpa_lpd_global_info[lpa_slot]));
} /* lpa_lpd_reset */


/*===========================================================================
   FUNCTION:      LPA_LPD_QUEUE_CMD

   DESCRIPTION:
     This function queues commands to LPA to processes the LPD procedures.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static lpa_result_enum_type  lpa_lpd_queue_cmd (
  lpa_lpd_response_data_type          *task_cmd_ptr
)
{
  lpa_result_enum_type    lpa_status    = LPA_SUCCESS;

  if (task_cmd_ptr == NULL)
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  LPA_UTIL_ENTER_TASK_STATE_CRIT_SECT;

  if (!lpa_task_stopped)
  {
    (void)q_link(task_cmd_ptr, &task_cmd_ptr->link);

    /* Put the message on the queue */
    q_put(&lpa_lpd_cmd_q, &task_cmd_ptr->link);

    /* Set the command queue signal */
    (void) rex_set_sigs(UIM_LPA_TCB, LPA_LPD_CMD_SIG);
  }
  else
  {
    LPA_MSG_ERR_0("LPA task stopped, cannot queue command");
    lpa_status = LPA_GENERIC_ERROR;
  }

  LPA_UTIL_LEAVE_TASK_STATE_CRIT_SECT;
  return lpa_status;
} /* lpa_lpd_queue_cmd */


/*===========================================================================
FUNCTION LPA_LPD_HTTP_CB

DESCRIPTION

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void lpa_lpd_http_cb (
  lpa_result_enum_type                      status,
  const lpa_http_resp_data_type            *http_resp_ptr,
  const void                               *user_data_ptr
)
{
  lpa_lpd_response_data_type  *task_cmd_ptr = NULL;
  lpa_data_type                lpa_json_data;

  memset(&lpa_json_data, 0x00, sizeof(lpa_json_data));

  if(http_resp_ptr == NULL)
  {
    return;
  }

  /* Malloc task cmd pointers and any sub pointers, as needed, and copy
     the response data received from the caller of this callback */
  task_cmd_ptr = (lpa_lpd_response_data_type*)LPA_MALLOC(sizeof(lpa_lpd_response_data_type));
  if(task_cmd_ptr == NULL)
  {
    return;
  }

  task_cmd_ptr->cmd_data.cmd = (lpa_lpd_cmd_enum_type) user_data_ptr;

  LPA_MSG_HIGH_2("lpa_lpd_http_cb status=0x%x, for lpd_cmd=0x%x", status, task_cmd_ptr->cmd_data.cmd);

  /* MALLOC and copy any pointers inside resp_ptr */
  switch(task_cmd_ptr->cmd_data.cmd)
  {
    case LPA_LPD_INITIATE_AUTH_RESP_CMD:
      if(http_resp_ptr->data_len > 0)
      {
        /* Write the data received from DS to EFS file for testing purposes */
        (void) lpa_efs_write(LPA_FILE_JSON_INIT_AUTH_FROM_DS,
                             http_resp_ptr->data_len,
                             http_resp_ptr->data_ptr,
                             http_resp_ptr->slot_id);
        lpa_json_data.data_len = http_resp_ptr->data_len;
        lpa_json_data.data_ptr = http_resp_ptr->data_ptr;
        status = lpa_json_initiate_auth_decoder(&lpa_json_data,
                                                &(task_cmd_ptr->cmd_data.data.initiate_auth_resp),
                                                http_resp_ptr->slot_id);
      }
      else
      {
        status = LPA_GENERIC_ERROR;
      }
      break;
    case LPA_LPD_GET_BPP_RESP_CMD:
      if(http_resp_ptr->data_len > 0)
      {
        /* Write the data received from DS to EFS file for testing purposes */
        (void) lpa_efs_write(LPA_FILE_JSON_GET_BPP_FROM_DS,
                             http_resp_ptr->data_len,
                             http_resp_ptr->data_ptr,
                             http_resp_ptr->slot_id);
        lpa_json_data.data_len = http_resp_ptr->data_len;
        lpa_json_data.data_ptr = http_resp_ptr->data_ptr;
        status = lpa_json_get_bound_profile_package_decoder(&lpa_json_data, &(task_cmd_ptr->cmd_data.data.get_bpp_resp));
        LPA_MSG_HIGH_1("lpa_json_get_bound_profile_package_decoder status=0x%x", status);
      }
      else
      {
        status = LPA_GENERIC_ERROR;
      }
      break;
    case LPA_LPD_AUTHENTICATE_CLIENT_RESP_CMD:
      if(http_resp_ptr->data_len > 0)
      {
        /* Write the data received from DS to EFS file for testing purposes */
        (void) lpa_efs_write(LPA_FILE_JSON_AUTHENTICATE_CLIENT_FROM_DS,
                             http_resp_ptr->data_len,
                             http_resp_ptr->data_ptr,
                             http_resp_ptr->slot_id);
        lpa_json_data.data_len = http_resp_ptr->data_len;
        lpa_json_data.data_ptr = http_resp_ptr->data_ptr;
        status = lpa_json_authenticate_client_decoder(&lpa_json_data,
                                                      &(task_cmd_ptr->cmd_data.data.authenticate_client_resp),
                                                      http_resp_ptr->slot_id);
		LPA_MSG_HIGH_1("lpa_json_authenticate_client_decoder status=0x%x", status);
      }
      else
      {
        status = LPA_GENERIC_ERROR;
      }
      break;
    case LPA_LPD_CANCEL_SESSION_SERVER_RESP_CMD:
    case LPA_LPD_HANDLE_NOTIFICATION_RESP_CMD:
    case LPA_LPD_HANDLE_INSTALLATION_RESULT_RESP_CMD:
      /* We don't receive any response for these commands */
      status = LPA_SUCCESS;
      break;
    default:
      status = LPA_GENERIC_ERROR;
      break;
  }

  task_cmd_ptr->slot_id            = http_resp_ptr->slot_id;
  task_cmd_ptr->status             = status;

  if(lpa_lpd_queue_cmd(task_cmd_ptr) != LPA_SUCCESS)
  {
    lpa_lpd_free_response_data(&task_cmd_ptr->cmd_data);
    LPA_FREE(task_cmd_ptr);
  }
} /* lpa_lpd_http_cb */


/*===========================================================================
   FUNCTION:      LPA_LPA_MMGSDI_CB_PARSE_LEGACY_RESP

   DESCRIPTION:
     This function parses according to phase1/1.1 or SGP22, the response
     received from the eUICC

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_lpd_mmgsdi_cb_parse_legacy_resp (
  const lpa_mmgsdi_resp_data_type                 *mmgsdi_resp_ptr,
  lpa_lpd_cmd_enum_type                            lpd_cmd,
  lpa_lpd_response_data_type                      *resp_ptr
)
{
  lpa_result_enum_type               status = LPA_SUCCESS;

  if(mmgsdi_resp_ptr == NULL ||
     resp_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  switch(mmgsdi_resp_ptr->cmd_type)
  {
    case LPA_MMGSDI_APDU_STORE_DATA:
      /* STORE DATA is sent for two operations: PrepareDownload and
         Load BPP */
      if(lpd_cmd == LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD)
      {
        if(mmgsdi_resp_ptr->data_len == 0 ||
           mmgsdi_resp_ptr->data_ptr == NULL)
        {
          status = LPA_GENERIC_ERROR;
          break;
        }
        resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_ptr =
          LPA_MALLOC(mmgsdi_resp_ptr->data_len);
        if(resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_ptr == NULL)
        {
          status = LPA_ERROR_HEAP_EXHAUSTED;
          break;
        }
        resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_len =
          mmgsdi_resp_ptr->data_len;
        memscpy(resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_ptr,
                resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_len,
                mmgsdi_resp_ptr->data_ptr,
                mmgsdi_resp_ptr->data_len);
      }
      else if(lpd_cmd == LPA_LPD_LOAD_BPP_RESP_CMD)
      {
        if(mmgsdi_resp_ptr->data_len == 0 ||
           mmgsdi_resp_ptr->data_ptr == NULL)
        {
          status = LPA_SUCCESS;
          break;
        }

        resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_ptr =
          LPA_MALLOC(mmgsdi_resp_ptr->data_len);
        if(resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_ptr == NULL)
        {
          status = LPA_ERROR_HEAP_EXHAUSTED;
          break;
        }
        resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_len =
          mmgsdi_resp_ptr->data_len;
        memscpy(resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_ptr,
                resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_len,
                mmgsdi_resp_ptr->data_ptr,
                mmgsdi_resp_ptr->data_len);
      }
      else
      {
        status = LPA_GENERIC_ERROR;
      }
      break;
    case LPA_MMGSDI_APDU_GET_CHALLENGE:
      /* For GET EUICC CHALLENGE, we need to do some validation of the
         response received from the card - we must receive 16
         bytes in the data */
      lpd_cmd = LPA_LPD_GET_EUICC_CHALLENGE_RESP_CMD;
      if(mmgsdi_resp_ptr->data_len != LPA_EUICC_CHALLENGE_LEN ||
         mmgsdi_resp_ptr->data_ptr == NULL)
      {
        status = LPA_GENERIC_ERROR;
        break;
      }
      /* Copy the euicc Challenge that can be fed into the Initiate Auth
         request by LPD */
      memscpy(resp_ptr->cmd_data.data.get_euicc_challenge_resp.euicc_challenge,
              sizeof(resp_ptr->cmd_data.data.get_euicc_challenge_resp.euicc_challenge),
              mmgsdi_resp_ptr->data_ptr,
              mmgsdi_resp_ptr->data_len);
      break;
    case LPA_MMGSDI_APDU_GET_DATA:
      /* This response is for GET_DATA APDU sent for eUICCInfo. Lets validate
         the response */
      lpd_cmd = LPA_LPD_GET_EUICC_INFO_RESP_CMD;
      if(mmgsdi_resp_ptr->data_len < LPA_SVN_LEN ||
         mmgsdi_resp_ptr->data_ptr == NULL)
      {
        status = LPA_GENERIC_ERROR;
        break;
      }
      /* Copy the euicc Info that can be fed into the Initiate Auth
         request by LPD */
      resp_ptr->cmd_data.data.get_euicc_info_resp.euicc_info1.data_ptr =
        LPA_MALLOC(mmgsdi_resp_ptr->data_len);
      if(resp_ptr->cmd_data.data.get_euicc_info_resp.euicc_info1.data_ptr == NULL)
      {
        status = LPA_ERROR_HEAP_EXHAUSTED;
        break;
      }
      resp_ptr->cmd_data.data.get_euicc_info_resp.euicc_info1.data_len =
        mmgsdi_resp_ptr->data_len;
      memscpy(resp_ptr->cmd_data.data.get_euicc_info_resp.euicc_info1.data_ptr,
              resp_ptr->cmd_data.data.get_euicc_info_resp.euicc_info1.data_len,
              mmgsdi_resp_ptr->data_ptr,
              mmgsdi_resp_ptr->data_len);
      /* Parse SVN from the euicc Info that can be fed into the Initiate Auth
         request by LPD */
      status = lpa_asn1_decode_get_euicc_info_resp(
                 mmgsdi_resp_ptr->data_len,
                 mmgsdi_resp_ptr->data_ptr,
                 &resp_ptr->cmd_data.data.get_euicc_info_resp);
      break;
    default:
      status = LPA_GENERIC_ERROR;
      break;
  }

  return status;
} /* lpa_lpd_mmgsdi_cb_parse_legacy_resp */


/*===========================================================================
   FUNCTION:      LPA_LPD_MMGSDI_CB_PARSE_RESP

   DESCRIPTION:
     This function parses according to SGP22, the response received from
     the eUICC

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
lpa_result_enum_type lpa_lpd_mmgsdi_cb_parse_resp (
  const lpa_mmgsdi_resp_data_type                 *mmgsdi_resp_ptr,
  lpa_lpd_cmd_enum_type                            lpd_cmd,
  lpa_lpd_response_data_type                      *resp_ptr
)
{
  lpa_result_enum_type         lpa_status = LPA_SUCCESS;

  if(mmgsdi_resp_ptr == NULL ||
     resp_ptr == NULL ||
     mmgsdi_resp_ptr->cmd_type != LPA_MMGSDI_APDU_STORE_DATA)
  {
    return LPA_GENERIC_ERROR;
  }

  switch(lpd_cmd)
  {
    case LPA_LPD_GET_EUICC_CHALLENGE_RESP_CMD:
      lpa_status = lpa_asn1_decode_get_euicc_challenge_resp(
                     mmgsdi_resp_ptr->data_len,
                     mmgsdi_resp_ptr->data_ptr,
                     &(resp_ptr->cmd_data.data.get_euicc_challenge_resp));
      break;
    case LPA_LPD_GET_EUICC_INFO_RESP_CMD:
      lpa_status = lpa_asn1_decode_get_euicc_info_resp(
                     mmgsdi_resp_ptr->data_len,
                     mmgsdi_resp_ptr->data_ptr,
                     &(resp_ptr->cmd_data.data.get_euicc_info_resp));
      break;
    case LPA_LPD_GET_RAT_RESP_CMD:
      lpa_status = lpa_asn1_decode_get_rat_resp(
                     mmgsdi_resp_ptr->data_len,
                     mmgsdi_resp_ptr->data_ptr,
                     &(resp_ptr->cmd_data.data.get_rat_resp));
      break;
    case LPA_LPD_GET_PROFILES_INFO_RESP_CMD:
      lpa_status = lpa_asn1_decode_get_profiles_info_resp(
                     mmgsdi_resp_ptr->data_len,
                     mmgsdi_resp_ptr->data_ptr,
                     &(resp_ptr->cmd_data.data.get_profiles_info_resp));
      break;
    case LPA_LPD_GET_EUICC_CONFIG_ADDRESS_RESP_CMD:
      lpa_status = lpa_asn1_decode_euicc_config_address_resp(
                     mmgsdi_resp_ptr->data_len,
                     mmgsdi_resp_ptr->data_ptr,
                     &(resp_ptr->cmd_data.data.get_euicc_config_address_resp));
      break;
    case LPA_LPD_AUTHENTICATE_SERVER_RESP_CMD:
      if(mmgsdi_resp_ptr->data_len == 0 ||
         mmgsdi_resp_ptr->data_ptr == NULL)
      {
        lpa_status = LPA_GENERIC_ERROR;
        break;
      }
      resp_ptr->cmd_data.data.authenticate_server_resp.data.data_ptr =
        LPA_MALLOC(mmgsdi_resp_ptr->data_len);
      if(resp_ptr->cmd_data.data.authenticate_server_resp.data.data_ptr == NULL)
      {
        lpa_status = LPA_ERROR_HEAP_EXHAUSTED;
        break;
      }
      resp_ptr->cmd_data.data.authenticate_server_resp.data.data_len =
        mmgsdi_resp_ptr->data_len;
      memscpy(resp_ptr->cmd_data.data.authenticate_server_resp.data.data_ptr,
              resp_ptr->cmd_data.data.authenticate_server_resp.data.data_len,
              mmgsdi_resp_ptr->data_ptr,
              mmgsdi_resp_ptr->data_len);
      break;
    case LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD:
      if(mmgsdi_resp_ptr->data_len == 0 ||
         mmgsdi_resp_ptr->data_ptr == NULL)
      {
        lpa_status = LPA_GENERIC_ERROR;
        break;
      }
      resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_ptr =
        LPA_MALLOC(mmgsdi_resp_ptr->data_len);
      if(resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_ptr == NULL)
      {
        lpa_status = LPA_ERROR_HEAP_EXHAUSTED;
        break;
      }
      resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_len =
        mmgsdi_resp_ptr->data_len;
      memscpy(resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_ptr,
              resp_ptr->cmd_data.data.prepare_download_resp.prepare_download.data_len,
              mmgsdi_resp_ptr->data_ptr,
              mmgsdi_resp_ptr->data_len);
      break;
    case LPA_LPD_CANCEL_SESSION_EUICC_RESP_CMD:
      if(mmgsdi_resp_ptr->data_len == 0 ||
         mmgsdi_resp_ptr->data_ptr == NULL)
      {
        lpa_status = LPA_GENERIC_ERROR;
        break;
      }
      resp_ptr->cmd_data.data.cancel_session_euicc_resp.data.data_ptr =
        LPA_MALLOC(mmgsdi_resp_ptr->data_len);
      if(resp_ptr->cmd_data.data.cancel_session_euicc_resp.data.data_ptr == NULL)
      {
        lpa_status = LPA_ERROR_HEAP_EXHAUSTED;
        break;
      }
      resp_ptr->cmd_data.data.cancel_session_euicc_resp.data.data_len =
        mmgsdi_resp_ptr->data_len;
      memscpy(resp_ptr->cmd_data.data.cancel_session_euicc_resp.data.data_ptr,
              resp_ptr->cmd_data.data.cancel_session_euicc_resp.data.data_len,
              mmgsdi_resp_ptr->data_ptr,
              mmgsdi_resp_ptr->data_len);
      break;
    case LPA_LPD_LOAD_BPP_RESP_CMD:
      if(mmgsdi_resp_ptr->data_len == 0 ||
         mmgsdi_resp_ptr->data_ptr == NULL)
      {
        lpa_status = LPA_SUCCESS;
        break;
      }
      resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_ptr =
        LPA_MALLOC(mmgsdi_resp_ptr->data_len);
      if(resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_ptr == NULL)
      {
        lpa_status = LPA_ERROR_HEAP_EXHAUSTED;
        break;
      }
      resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_len =
        mmgsdi_resp_ptr->data_len;
      memscpy(resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_ptr,
              resp_ptr->cmd_data.data.load_bpp_resp.load_bpp.data_len,
              mmgsdi_resp_ptr->data_ptr,
              mmgsdi_resp_ptr->data_len);
      break;
    default:
      lpa_status = LPA_GENERIC_ERROR;
      break;
  }

  LPA_MSG_HIGH_2("lpa_lpd_mmgsdi_cb_parse_resp status=0x%x for lpd_cmd=0x%x",
                 lpa_status, lpd_cmd);

  return lpa_status;
} /* lpa_lpd_mmgsdi_cb_parse_resp */


/*===========================================================================
   FUNCTION:      LPA_LPA_MMGSDI_CB

   DESCRIPTION:
     This is a callback function called by LPA_MMGSDI module used by it
     to report response for the MMGSDI bound request sent by LPA.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
void lpa_lpd_mmgsdi_cb (
  lpa_result_enum_type                             status,
  const lpa_mmgsdi_resp_data_type                 *mmgsdi_resp_ptr,
  const void                                      *user_data_ptr
)
{
  lpa_lpd_response_data_type             *task_cmd_ptr = NULL;

  if(mmgsdi_resp_ptr == NULL)
  {
    return;
  }

  LPA_MSG_HIGH_2("Received lpa_lpd_mmgsdi_cb status=0x%x,cmd=0x%x",
                 status, (lpa_lpd_cmd_enum_type)user_data_ptr);

  /* Malloc task cmd pointers and any sub pointers, as needed, and copy
     the response data received from the caller of this callback */
  task_cmd_ptr = (lpa_lpd_response_data_type*)LPA_MALLOC(sizeof(lpa_lpd_response_data_type));
  if(task_cmd_ptr == NULL)
  {
    return;
  }

  task_cmd_ptr->slot_id            = mmgsdi_resp_ptr->slot_id;
  task_cmd_ptr->cmd_data.cmd       = (lpa_lpd_cmd_enum_type)user_data_ptr;


  if(status == LPA_SUCCESS)
  {
    lpa_euicc_phase_enum_type                     euicc_phase = LPA_PHASE1_1_EUICC;

    (void) lpa_retrieve_euicc_phase(task_cmd_ptr->slot_id, &euicc_phase);

    if(euicc_phase == LPA_PHASE1_EUICC ||
       euicc_phase == LPA_PHASE1_1_EUICC)
    {
      status = lpa_lpd_mmgsdi_cb_parse_legacy_resp(mmgsdi_resp_ptr,
                                                   (lpa_lpd_cmd_enum_type)user_data_ptr,
                                                   task_cmd_ptr);
    }
    else
    {
      status = lpa_lpd_mmgsdi_cb_parse_resp(mmgsdi_resp_ptr,
                                            (lpa_lpd_cmd_enum_type)user_data_ptr,
                                            task_cmd_ptr);
    }
  }

  task_cmd_ptr->status             = status;

  if(lpa_lpd_queue_cmd(task_cmd_ptr) != LPA_SUCCESS)
  {
    lpa_lpd_free_response_data(&task_cmd_ptr->cmd_data);
    LPA_FREE(task_cmd_ptr);
  }
} /* lpa_lpd_mmgsdi_cb */


/*===========================================================================
FUNCTION LPA_LPD_START_ADD_PROFILE_PROCEDURE

DESCRIPTION

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_lpd_start_add_profile_procedure (
  const lpa_add_profile_req_type   *add_profile_req_ptr,
  lpa_slot_id_enum_type             slot_id
)
{
  lpa_result_enum_type       lpa_status  = LPA_SUCCESS;
  lpa_euicc_phase_enum_type  euicc_phase = LPA_PHASE1_1_EUICC;

  if(add_profile_req_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_efs_cleanup(slot_id);

  /* Write raw activation code to the file */
  (void) lpa_efs_write(LPA_FILE_ACTIVATION_CODE_FROM_USER,
                       add_profile_req_ptr->activation_code.data_len,
                       add_profile_req_ptr->activation_code.data_ptr,
                       slot_id);

  /* Parse activation code and copy it to the global that can later be used
     to send the Initiate Auth request to SMDP */
  lpa_status = lpa_util_parse_activation_code(
                 &add_profile_req_ptr->activation_code,
                 &(lpa_lpd_global_info[slot_id].activation_code),
                 &(lpa_lpd_global_info[slot_id].method));
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  /* If activation code says confirmation code is required, make sure it
     was provided by the user. If it was provided, hash it and save it
     for later */
  if(lpa_lpd_global_info[slot_id].activation_code.confirmation_code_required &&
     (add_profile_req_ptr->confirmation_code.data_len == 0 ||
      add_profile_req_ptr->confirmation_code.data_ptr == NULL))
  {
    LPA_MSG_ERR_0("Confirmation code required but not present!");
    return LPA_ERROR_CONFIRMATION_CODE_REQUIRED;
  }

  if(add_profile_req_ptr->confirmation_code.data_len > 0 &&
     add_profile_req_ptr->confirmation_code.data_ptr)
  {
    /* Confirmation code is present. Hash it. Note that the hashing function
       below MALLOCs the out pointer. Make sure to free it when done */
    lpa_status = lpa_util_hash_sha256(add_profile_req_ptr->confirmation_code.data_ptr,
                                      add_profile_req_ptr->confirmation_code.data_len,
                                      &(lpa_lpd_global_info[slot_id].hashed_confirmation_code));
    if(lpa_status != LPA_SUCCESS)
    {
      LPA_FREE(lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_ptr);
      return lpa_status;
    }
    LPA_MSG_HIGH_0("Confirmation code hashing successful");
  }

  lpa_status = lpa_retrieve_euicc_phase(slot_id,
                                        &euicc_phase);
  if(LPA_SUCCESS != lpa_status)
  {
    return lpa_status;
  }

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    lpa_status = lpa_mmgsdi_get_challenge(slot_id,
                                          lpa_lpd_mmgsdi_cb,
                                          (void*) LPA_LPD_GET_EUICC_CHALLENGE_RESP_CMD);
  }
  else
  {
    lpa_data_type                         data;

    memset(&data, 0x00, sizeof(data));

    lpa_status = lpa_asn1_encode_get_euicc_challenge_request(&data);
    if(LPA_SUCCESS == lpa_status)
    {
      lpa_status = lpa_mmgsdi_store_data(slot_id,
                                         LPA_PHASE_2_STORE_DATA_P1,
                                         LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                         data,
                                         TRUE,
                                         lpa_lpd_mmgsdi_cb,
                                         (void*) LPA_LPD_GET_EUICC_CHALLENGE_RESP_CMD);
    }

    LPA_FREE(data.data_ptr);
    data.data_len = 0;
  }

  if(lpa_status == LPA_SUCCESS)
  {
    /* We should now send response to LPA client for the Add Profile request.
       Note that the remaining updates regarding the various sub procedures
       involved, we will send event to the LPA client.
       Send the response but do not clear timers etc (by passing FALSE into
       lpa_send_response() */
    lpa_response_data_type            *resp_ptr = NULL;

    resp_ptr = (lpa_response_data_type*) LPA_MALLOC(sizeof(lpa_response_data_type));
    if(resp_ptr != NULL)
    {
      resp_ptr->msg_type = LPA_ADD_PROFILE_MSG;
    }
    lpa_send_response(lpa_status,
                      resp_ptr,
                      slot_id,
                      FALSE);
    LPA_FREE(resp_ptr);
    /* Do not set the sig */
  }
  return lpa_status;
} /* lpa_lpd_start_add_profile_procedure */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_USER_CONSENT

DESCRIPTION

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_lpd_process_user_consent (
  boolean                                 go_ahead,
  lpa_slot_id_enum_type                   slot_id
)
{
  lpa_result_enum_type              lpa_status   = LPA_SUCCESS;
  lpa_lpd_response_data_type       *task_cmd_ptr = NULL;

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Malloc task cmd pointers and any sub pointers, as needed, and copy
     the response data received from the caller of this callback */
  task_cmd_ptr = (lpa_lpd_response_data_type*)LPA_MALLOC(sizeof(lpa_lpd_response_data_type));
  if(task_cmd_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  task_cmd_ptr->slot_id                             = slot_id;
  task_cmd_ptr->cmd_data.cmd                        = LPA_LPD_USER_CONSENT_CMD;
  task_cmd_ptr->cmd_data.data.user_consent.go_ahead = go_ahead;
  task_cmd_ptr->status                              = LPA_SUCCESS;

  lpa_status = lpa_lpd_queue_cmd(task_cmd_ptr);

  if(lpa_status != LPA_SUCCESS)
  {
    lpa_lpd_free_response_data(&task_cmd_ptr->cmd_data);
    LPA_FREE(task_cmd_ptr);
  }

  return lpa_status;
} /* lpa_lpd_process_user_consent */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_GET_EUICC_CHALLENGE_RESP

DESCRIPTION
  Processes euicc challenge received from MMGSDI

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_get_euicc_challenge_resp (
  lpa_lpd_get_euicc_challenge_resp_type  *get_euicc_challenge_ptr,
  lpa_slot_id_enum_type                   slot_id
)
{
  lpa_data_type                           data;
  lpa_euicc_phase_enum_type               euicc_phase = LPA_PHASE1_1_EUICC;
  lpa_result_enum_type                    lpa_status  = LPA_SUCCESS;

  memset(&data, 0x00, sizeof(data));

  if(get_euicc_challenge_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Copy euicc challenge over to the global that we will later use for
     sending request to SMDP  */
  memscpy(lpa_lpd_global_info[slot_id].euicc_challenge,
          sizeof(lpa_lpd_global_info[slot_id].euicc_challenge),
          get_euicc_challenge_ptr->euicc_challenge,
          sizeof(get_euicc_challenge_ptr->euicc_challenge));

  /* Also write the eUICCChallenge that is retrieved, to the file */
  (void) lpa_efs_write(LPA_FILE_EUICC_CHAL_FROM_CARD,
                       sizeof(get_euicc_challenge_ptr->euicc_challenge),
                       get_euicc_challenge_ptr->euicc_challenge,
                       slot_id);

  /* Now lets get the euicc Info from the card... without leaving the current
       state, which is LPA_STATE_INIT */
  /* If the following fails, it is ok because in that case we treat the
     euicc phase as phase1.1 by default */
  (void) lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    lpa_status = lpa_mmgsdi_get_data(slot_id,
                                     (uint8)LPA_GET_EUICC_INFO_APDU_DATA_P2,
                                     data,
                                     lpa_lpd_mmgsdi_cb,
                                     (void*) LPA_LPD_GET_EUICC_INFO_RESP_CMD);
  }
  else
  {
    lpa_status = lpa_asn1_encode_get_euicc_info_request(&data);
    if(lpa_status == LPA_SUCCESS)
    {
      lpa_status = lpa_mmgsdi_store_data(slot_id,
                                         LPA_PHASE_2_STORE_DATA_P1,
                                         LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                         data,
                                         TRUE,
                                         lpa_lpd_mmgsdi_cb,
                                         (void*) LPA_LPD_GET_EUICC_INFO_RESP_CMD);
    }
    LPA_FREE(data.data_ptr);
    data.data_len = 0;
  }

  return lpa_status;
} /* lpa_lpd_process_get_euicc_challenge_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_PREPARE_DOWNLOAD_RESP

DESCRIPTION
  Processes prepare download (STORE DATA) response from the card

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_process_prepare_download_resp (
  lpa_lpd_prepare_download_resp_type  *prepare_download_resp_ptr,
  lpa_slot_id_enum_type                slot_id
)
{
  lpa_result_enum_type            lpa_status   = LPA_SUCCESS;
  lpa_http_get_bpp_req_type       get_bpp_req;
  lpa_euicc_phase_enum_type       euicc_phase  = LPA_PHASE1_1_EUICC;

  if(prepare_download_resp_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  /* At this point we have retrieved prepare download response from the
     card. Both the card and the SMPD have been authenticated.
     Go ahead and send Get BPP request to SMDP */
  (void) lpa_retrieve_euicc_phase(slot_id, &euicc_phase);

  if(euicc_phase == LPA_PHASE1_EUICC ||
     euicc_phase == LPA_PHASE1_1_EUICC)
  {
    lpa_status = lpa_state_change(LPA_STATE_INITIATE_AUTH_RESP_RCVD, LPA_STATE_GET_BPP_SENT, slot_id);
  }
  else
  {
    lpa_status = lpa_state_change(LPA_STATE_AUTHENTICATE_CLIENT_RESP_RCVD, LPA_STATE_GET_BPP_SENT, slot_id);
    if(lpa_status != LPA_SUCCESS)
    {
      lpa_status = lpa_state_change(LPA_STATE_USER_CONSENT_RCVD, LPA_STATE_GET_BPP_SENT, slot_id);
    }
  }

  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  /* No need to decode the ASN1 formatted prepareDownloadResponse from the
     card... send it as-is into the GET_BPP request to SMDP */
  memset(&get_bpp_req, 0x00, sizeof(lpa_http_get_bpp_req_type));
  get_bpp_req.prepare_download_data = prepare_download_resp_ptr->prepare_download;
  get_bpp_req.slot_id = slot_id;
  get_bpp_req.lpd_callback = lpa_lpd_http_cb;
  get_bpp_req.user_data_ptr = (void*) LPA_LPD_GET_BPP_RESP_CMD;
  if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_DEFAULT_SMDP)
  {
    /* Add Profile initiated using default SMDP address */
    get_bpp_req.smdp_address = lpa_lpd_global_info[slot_id].default_smdp_address;
  }
  else if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_SMDS)
  {
    /* Add Profile initiated using root SMDS address */
    get_bpp_req.smdp_address = lpa_lpd_global_info[slot_id].root_smds_address;
  }
  else
  {
    /* Add Profile initiated using SMDS address in activation code */
    get_bpp_req.smdp_address = lpa_lpd_global_info[slot_id].activation_code.smdp_address;
  }
  get_bpp_req.transaction_id_data   = lpa_lpd_global_info[slot_id].transaction_id_string;

  /* Write the data received from card to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_PREP_DOWNLOAD_RESP_FROM_CARD,
                       prepare_download_resp_ptr->prepare_download.data_len,
                       prepare_download_resp_ptr->prepare_download.data_ptr,
                       slot_id);

  return lpa_http_build_and_send_get_bpp(&get_bpp_req);
} /* lpa_lpd_process_process_prepare_download_resp */


/*===========================================================================
FUNCTION LPA_LPD_START_CANCEL_SESSION_PROCEDURE

DESCRIPTION
  Initiates a cancel session request thereby canceling the ongoing profile
  download

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_start_cancel_session_procedure (
  lpa_slot_id_enum_type                          slot_id,
  lpa_lpd_cancel_session_reason_enum_type        reason
)
{
  lpa_data_type                  cancel_session_euicc_encoded;
  lpa_result_enum_type           lpa_status                   = LPA_SUCCESS;
  lpa_euicc_phase_enum_type      euicc_phase                  = LPA_PHASE2_EUICC;

  memset(&cancel_session_euicc_encoded, 0x00, sizeof(cancel_session_euicc_encoded));

  (void)lpa_retrieve_euicc_phase(slot_id,
                                 &euicc_phase);


  /* Lets send CancelSession to the eUICC. After receiving a response for
     that from the eUICC, we will send CancelSession to the SMDP+ */
  lpa_status = lpa_asn1_encode_cancel_session_request(
                 lpa_lpd_global_info[slot_id].transaction_id_string,
                 reason,
                 &cancel_session_euicc_encoded,
                 euicc_phase);

  if(lpa_status == LPA_SUCCESS)
  {
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PREPARE_DOWNLOAD_APDU_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       cancel_session_euicc_encoded,
                                       TRUE,
                                       lpa_lpd_mmgsdi_cb,
                                       (void*) LPA_LPD_CANCEL_SESSION_EUICC_RESP_CMD);
  }
  LPA_FREE(cancel_session_euicc_encoded.data_ptr);
  cancel_session_euicc_encoded.data_len = 0;

  return lpa_status;
} /* lpa_lpd_start_cancel_session_procedure*/


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_USER_CONSENT

DESCRIPTION
  Processes user consent received from the user

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_process_user_consent (
  lpa_lpd_user_consent_type           *user_consent_ptr,
  lpa_slot_id_enum_type                slot_id
)
{
  lpa_result_enum_type                    lpa_status = LPA_SUCCESS;
  lpa_data_type                           prepare_download_encoded;
  lpa_asn1_prepare_download_request_type  prepare_download_raw;

  memset(&prepare_download_encoded, 0x00, sizeof(prepare_download_encoded));
  memset(&prepare_download_raw, 0x00, sizeof(prepare_download_raw));

  if(user_consent_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  LPA_MSG_HIGH_1("In lpa_lpd_process_process_user_consent, go_ahead=0x%x", user_consent_ptr->go_ahead);

  lpa_status = lpa_state_change(LPA_STATE_WAITING_FOR_USER_CONSENT,
                                LPA_STATE_USER_CONSENT_RCVD,
                                slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  if(user_consent_ptr->go_ahead == FALSE)
  {
    lpa_lpd_global_info[slot_id].cancel_session_reason =
      LPA_LPD_CANCEL_SESSION_END_USER_REJECTION;
    return lpa_lpd_start_cancel_session_procedure(
             slot_id,
             LPA_LPD_CANCEL_SESSION_END_USER_REJECTION);
  }

  /* User wants to continue the profile download process. Lets continue the
     process by sending prepareDownload request */
  prepare_download_raw.dp_signed_raw =
    lpa_lpd_global_info[slot_id].smdp_signed2;
  prepare_download_raw.smdp_signature_raw =
    lpa_lpd_global_info[slot_id].smdp_signature2;
  prepare_download_raw.hashed_confirmation_code =
    lpa_lpd_global_info[slot_id].hashed_confirmation_code_tx_id;
  prepare_download_raw.smdp_certificate_raw =
    lpa_lpd_global_info[slot_id].smdp_certificate;

  /* Following encoder may malloc prepare_download_encoded - so make sure
     to free it before exiting the function */
  lpa_status = lpa_asn1_encode_prepare_download_request(
                 prepare_download_raw, &prepare_download_encoded, slot_id);

  /* Write the data received from DS to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_ASN1_ENCODED_PREP_DOWNLOAD,
                       prepare_download_encoded.data_len,
                       prepare_download_encoded.data_ptr,
                       slot_id);

  /* Send the prepare download STORE DATA to the card */
  if(lpa_status == LPA_SUCCESS &&
     prepare_download_encoded.data_len > 0 &&
     prepare_download_encoded.data_ptr)
  {
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PREPARE_DOWNLOAD_APDU_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       prepare_download_encoded,
                                       TRUE,
                                       lpa_lpd_mmgsdi_cb,
                                       (void*) LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD);
  }
  else
  {
    LPA_MSG_HIGH_1("Error in lpa_asn1_encode_prepare_download_request, status=0x%x",
                   lpa_status);
  }

  LPA_FREE(prepare_download_encoded.data_ptr);

  return lpa_status;
} /* lpa_lpd_process_user_consent */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_AUTHENTICATE_SERVER_RESP

DESCRIPTION
  Processes Autheticate Server (STORE DATA) response from the card

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_authenticate_server_resp (
  lpa_lpd_authenticate_server_resp_type  *authenticate_server_resp_ptr,
  lpa_slot_id_enum_type                   slot_id
)
{
  lpa_result_enum_type                   lpa_status = LPA_SUCCESS;
  lpa_http_authenticate_client_req_type  authenticate_client_req;

  if(authenticate_server_resp_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  /* At this point we have retrieved Authenticate Server response from the
     card. Its time for sending the request to SMxx to Authenticate the
     Client */
  lpa_status = lpa_state_change(LPA_STATE_INITIATE_AUTH_RESP_RCVD, LPA_STATE_AUTHENTICATE_CLIENT_SENT, slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  /* No need to decode the ASN1 formatted AuthenticateServer from the
     card... send it as-is into the AuthenticateClient request to SMDP */
  memset(&authenticate_client_req, 0x00, sizeof(authenticate_client_req));
  authenticate_client_req.authenticate_server_response = authenticate_server_resp_ptr->data;
  authenticate_client_req.slot_id = slot_id;
  authenticate_client_req.lpd_callback = lpa_lpd_http_cb;
  authenticate_client_req.user_data_ptr = (void*) LPA_LPD_AUTHENTICATE_CLIENT_RESP_CMD;
  if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_DEFAULT_SMDP)
  {
    /* Add Profile initiated using default SMDP address */
    authenticate_client_req.smdp_address = lpa_lpd_global_info[slot_id].default_smdp_address;
  }
  else if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_SMDS)
  {
    /* Add Profile initiated using root SMDS address */
    authenticate_client_req.smdp_address = lpa_lpd_global_info[slot_id].root_smds_address;
  }
  else
  {
    /* Add Profile initiated using SMDS address in activation code */
    authenticate_client_req.smdp_address = lpa_lpd_global_info[slot_id].activation_code.smdp_address;
  }
  authenticate_client_req.transaction_id_data   = lpa_lpd_global_info[slot_id].transaction_id_string;

  /* Write the data received from card to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_AUTHENTICATE_SERVER_RESP_FROM_CARD,
                       authenticate_server_resp_ptr->data.data_len,
                       authenticate_server_resp_ptr->data.data_ptr,
                       slot_id);

  return lpa_http_build_and_send_authenticate_client(&authenticate_client_req);
} /* lpa_lpd_process_authenticate_server_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_GET_EUICC_INFO_LEGACY_RESP

DESCRIPTION
  Processes phase1/1.1 specific euicc info received from MMGSDI

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_get_euicc_info_legacy_resp (
  lpa_lpd_get_euicc_info_resp_type            *get_euicc_info_ptr,
  lpa_slot_id_enum_type                        slot_id
)
{
  lpa_result_enum_type                    lpa_status            = LPA_SUCCESS;
  lpa_http_initiate_auth_req_type         initiate_auth_req;

  if(get_euicc_info_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(get_euicc_info_ptr->euicc_info1.data_ptr == NULL||
     get_euicc_info_ptr->euicc_info1.data_len == 0)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Write the data received from card to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_EUICC_INFO_FROM_CARD,
                       get_euicc_info_ptr->euicc_info1.data_len,
                       get_euicc_info_ptr->euicc_info1.data_ptr,
                       slot_id);

  /* Make sure we clean up the stale value from global before populating
     the new one */
  LPA_FREE(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr);
  memset(&lpa_lpd_global_info[slot_id].euicc_info1,
         0x00,
         sizeof(lpa_lpd_global_info[slot_id].euicc_info1));

  /* Copy euicc info over to the global that we will later use for
     sending Initiate Auth request to SMDP */
  lpa_lpd_global_info[slot_id].euicc_info1.data_ptr =
    LPA_MALLOC(get_euicc_info_ptr->euicc_info1.data_len);
  if(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  lpa_lpd_global_info[slot_id].euicc_info1.data_len =
    get_euicc_info_ptr->euicc_info1.data_len;
  memscpy(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr,
          lpa_lpd_global_info[slot_id].euicc_info1.data_len,
          get_euicc_info_ptr->euicc_info1.data_ptr,
          get_euicc_info_ptr->euicc_info1.data_len);

  lpa_status = lpa_set_svn(slot_id,
                           sizeof(get_euicc_info_ptr->svn),
                           get_euicc_info_ptr->svn);
  if(LPA_SUCCESS != lpa_status)
  {
    LPA_FREE(lpa_lpd_global_info[slot_id].cert_info.data_ptr);
    LPA_FREE(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr);
    return lpa_status;
  }

  if(get_euicc_info_ptr->cert_info.data_ptr)
  {
    /* Make sure we clean up the stale value from global before populating
       the new one */
    LPA_FREE(lpa_lpd_global_info[slot_id].cert_info.data_ptr);
    memset(&lpa_lpd_global_info[slot_id].cert_info,
           0x00,
           sizeof(lpa_lpd_global_info[slot_id].cert_info));

    lpa_lpd_global_info[slot_id].cert_info.data_ptr =
      LPA_MALLOC(get_euicc_info_ptr->cert_info.data_len);
    if(lpa_lpd_global_info[slot_id].cert_info.data_ptr == NULL)
    {
      LPA_FREE(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr);
      return LPA_ERROR_HEAP_EXHAUSTED;
    }
    lpa_lpd_global_info[slot_id].cert_info.data_len =
      get_euicc_info_ptr->cert_info.data_len;
    memscpy(lpa_lpd_global_info[slot_id].cert_info.data_ptr,
            lpa_lpd_global_info[slot_id].cert_info.data_len,
            get_euicc_info_ptr->cert_info.data_ptr,
            get_euicc_info_ptr->cert_info.data_len);
  }
  /* At this point we have retrieved all of the following:
     1. SMDP address
     2. EUICC Challenge
     3. SVN
     4. Entire EUICC Info/Cert Info
     So, we have everything needed to send Initiate Auth request to SMDP via
     HTTP request. Trigger Initiate Auth request and move the state */
  lpa_status = lpa_state_change(LPA_STATE_INIT, LPA_STATE_INITIATE_AUTH_SENT, slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    LPA_FREE(lpa_lpd_global_info[slot_id].cert_info.data_ptr);
    LPA_FREE(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr);
    return lpa_status;
  }

  memset(&initiate_auth_req, 0x00, sizeof(lpa_http_initiate_auth_req_type));

  memscpy(initiate_auth_req.euicc_challenge,
          sizeof(initiate_auth_req.euicc_challenge),
          lpa_lpd_global_info[slot_id].euicc_challenge,
          sizeof(lpa_lpd_global_info[slot_id].euicc_challenge));
  initiate_auth_req.euicc_info1 = lpa_lpd_global_info[slot_id].euicc_info1;
  initiate_auth_req.cert_info = lpa_lpd_global_info[slot_id].cert_info;
  initiate_auth_req.smdp_address = lpa_lpd_global_info[slot_id].activation_code.smdp_address;

  memscpy(initiate_auth_req.svn,
          sizeof(initiate_auth_req.svn),
          get_euicc_info_ptr->svn,
          sizeof(get_euicc_info_ptr->svn));
  initiate_auth_req.slot_id = slot_id;
  initiate_auth_req.lpd_callback = lpa_lpd_http_cb;
  initiate_auth_req.user_data_ptr = (void*) LPA_LPD_INITIATE_AUTH_RESP_CMD;

  return lpa_http_build_and_send_initiate_auth(&initiate_auth_req);
} /* lpa_lpd_process_get_euicc_info_legacy_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_GET_EUICC_INFO_RESP

DESCRIPTION
  Processes euicc info received from MMGSDI

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_get_euicc_info_resp (
  lpa_lpd_get_euicc_info_resp_type            *get_euicc_info_ptr,
  lpa_slot_id_enum_type                        slot_id
)
{
  lpa_result_enum_type                    lpa_status            = LPA_SUCCESS;
  lpa_data_type                           data;

  memset(&data, 0x00, sizeof(data));

  if(get_euicc_info_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(get_euicc_info_ptr->euicc_info1.data_ptr == NULL||
     get_euicc_info_ptr->euicc_info1.data_len == 0)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Write the data received from card to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_EUICC_INFO_FROM_CARD,
                       get_euicc_info_ptr->euicc_info1.data_len,
                       get_euicc_info_ptr->euicc_info1.data_ptr,
                       slot_id);

  /* Make sure we clean up the stale value from global before populating
     the new one */
  LPA_FREE(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr);
  memset(&lpa_lpd_global_info[slot_id].euicc_info1,
         0x00,
         sizeof(lpa_lpd_global_info[slot_id].euicc_info1));

  /* Copy euicc info over to the global that we will later use for
     sending request to SMDP  */
  lpa_lpd_global_info[slot_id].euicc_info1.data_ptr =
    LPA_MALLOC(get_euicc_info_ptr->euicc_info1.data_len);
  if(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  lpa_lpd_global_info[slot_id].euicc_info1.data_len =
    get_euicc_info_ptr->euicc_info1.data_len;
  memscpy(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr,
          lpa_lpd_global_info[slot_id].euicc_info1.data_len,
          get_euicc_info_ptr->euicc_info1.data_ptr,
          get_euicc_info_ptr->euicc_info1.data_len);

  lpa_status = lpa_set_svn(slot_id,
                           sizeof(get_euicc_info_ptr->svn),
                           get_euicc_info_ptr->svn);
  if(LPA_SUCCESS != lpa_status)
  {
    LPA_FREE(lpa_lpd_global_info[slot_id].euicc_info1.data_ptr);
    return lpa_status;
  }

  /* Now lets get the RAT table from the card... without leaving
     the current state, which is LPA_STATE_INIT */
  lpa_status = lpa_asn1_encode_get_rat_request(&data);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  /* Send STORE DATA apdu to the card for get RAT operation */
  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     LPA_PHASE_2_STORE_DATA_P1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     data,
                                     TRUE,
                                     lpa_lpd_mmgsdi_cb,
                                     (void*) LPA_LPD_GET_RAT_RESP_CMD);

  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_lpd_process_get_euicc_info_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_GET_RAT_RESP

DESCRIPTION
  Processes Rules Authorization Table received from MMGSDI

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_get_rat_resp (
  lpa_get_rat_resp_type                       *get_rat_ptr,
  lpa_slot_id_enum_type                        slot_id
)
{
  lpa_result_enum_type                    lpa_status            = LPA_SUCCESS;
  lpa_data_type                           data;

  memset(&data, 0x00, sizeof(data));

  if(get_rat_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Make sure we clean up the stale value from global before populating
     the new one */
  LPA_FREE(lpa_lpd_global_info[slot_id].rat.rules_auth_table.ppar_entry_ptr);
  memset(&lpa_lpd_global_info[slot_id].rat,
         0x00,
         sizeof(lpa_get_rat_resp_type));

  /* Copy over the RAT to the global */
  if(get_rat_ptr->rules_auth_table.num_ppar_entries > 0 &&
     get_rat_ptr->rules_auth_table.ppar_entry_ptr)
  {
    lpa_lpd_global_info[slot_id].rat.rules_auth_table.num_ppar_entries =
      get_rat_ptr->rules_auth_table.num_ppar_entries;

    lpa_lpd_global_info[slot_id].rat.rules_auth_table.ppar_entry_ptr =
      (lpa_ppar_table_entry_type*) LPA_MALLOC(get_rat_ptr->rules_auth_table.num_ppar_entries * sizeof(lpa_ppar_table_entry_type));

    memscpy(lpa_lpd_global_info[slot_id].rat.rules_auth_table.ppar_entry_ptr,
            (get_rat_ptr->rules_auth_table.num_ppar_entries * sizeof(lpa_ppar_table_entry_type)),
            get_rat_ptr->rules_auth_table.ppar_entry_ptr,
            (get_rat_ptr->rules_auth_table.num_ppar_entries * sizeof(lpa_ppar_table_entry_type)));
  }

  /* Now lets perform the Get Profiles Info from the card... without leaving
     the current state, which is LPA_STATE_INIT */
  lpa_status = lpa_asn1_encode_get_profiles_info_request(&data);
  if(lpa_status == LPA_SUCCESS)
  {
    /* Send STORE DATA apdu to the card for get RAT operation */
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PHASE_2_STORE_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       data,
                                       TRUE,
                                       lpa_lpd_mmgsdi_cb,
                                       (void*) LPA_LPD_GET_PROFILES_INFO_RESP_CMD);
  }
  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_lpd_process_get_rat_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_GET_PROFILES_INFO_RESP

DESCRIPTION
  Processes Get Profiles Info response received from MMGSDI

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_get_profiles_info_resp (
  lpa_get_profiles_info_resp_type             *get_profiles_info_ptr,
  lpa_slot_id_enum_type                        slot_id
)
{
  lpa_result_enum_type                    lpa_status            = LPA_SUCCESS;
  lpa_data_type                           data;
  lpa_http_initiate_auth_req_type         initiate_auth_req;
  uint8                                   index                 = 0;

  memset(&data, 0x00, sizeof(data));

  if(get_profiles_info_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Make sure we clean up the stale value from global before populating
     the new one */
  lpa_lpd_global_info[slot_id].any_operational_profile = FALSE;

  /* Find out if any profile on the eUICC is an operational profile.
     This info will be used by LPA during the Profile Policy check phase. */
  for(index = 0;
      index < get_profiles_info_ptr->num_profiles &&
      index < sizeof(get_profiles_info_ptr->profile_info)/sizeof(get_profiles_info_ptr->profile_info[0]);
      index++)
  {
    if(get_profiles_info_ptr->profile_info[index].profile_class ==
         LPA_PROFILE_CLASS_OPERATIONAL)
    {
      lpa_lpd_global_info[slot_id].any_operational_profile = TRUE;
      break;
    }
  }

  /* Lets see if we need to retrieve the default SMDP or root SMDS address */
  if(lpa_lpd_global_info[slot_id].method != LPA_ADD_PROFILE_METHOD_ACTIVATION_CODE)
  {
    lpa_status = lpa_asn1_encode_get_euicc_config_address_request(&data);
    if(lpa_status == LPA_SUCCESS)
    {
      /* Send STORE DATA apdu to the card for get RAT operation */
      lpa_status = lpa_mmgsdi_store_data(slot_id,
                                         LPA_PHASE_2_STORE_DATA_P1,
                                         LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                         data,
                                         TRUE,
                                         lpa_lpd_mmgsdi_cb,
                                         (void*) LPA_LPD_GET_EUICC_CONFIG_ADDRESS_RESP_CMD);
    }
    LPA_FREE(data.data_ptr);
    data.data_len = 0;
  }
  else
  {
    /* Now we can go ahead and send Initiate Auth request */
    lpa_status = lpa_state_change(LPA_STATE_INIT, LPA_STATE_INITIATE_AUTH_SENT, slot_id);
    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }

    memset(&initiate_auth_req, 0x00, sizeof(lpa_http_initiate_auth_req_type));

    memscpy(initiate_auth_req.euicc_challenge,
            sizeof(initiate_auth_req.euicc_challenge),
            lpa_lpd_global_info[slot_id].euicc_challenge,
            sizeof(lpa_lpd_global_info[slot_id].euicc_challenge));
    initiate_auth_req.euicc_info1 = lpa_lpd_global_info[slot_id].euicc_info1;
    initiate_auth_req.smdp_address = lpa_lpd_global_info[slot_id].activation_code.smdp_address;
    initiate_auth_req.slot_id = slot_id;
    initiate_auth_req.lpd_callback = lpa_lpd_http_cb;
    initiate_auth_req.user_data_ptr = (void*) LPA_LPD_INITIATE_AUTH_RESP_CMD;

    lpa_status = lpa_http_build_and_send_initiate_auth(&initiate_auth_req);
  }

  return lpa_status;
} /* lpa_lpd_process_get_profiles_info_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_GET_EUICC_CONFIG_ADDRESS_RESP

DESCRIPTION
  Processes eUICC Configured address received from MMGSDI

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_get_euicc_config_address_resp (
  lpa_default_server_address_resp_type        *euicc_config_address_ptr,
  lpa_slot_id_enum_type                        slot_id
)
{
  lpa_result_enum_type                    lpa_status            = LPA_SUCCESS;
  lpa_data_type                           data;
  lpa_http_initiate_auth_req_type         initiate_auth_req;

  memset(&data, 0x00, sizeof(data));

  if(euicc_config_address_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(euicc_config_address_ptr->default_smdp_address.data_len > LPA_SERVER_ADDRESS_LEN_MAX ||
     euicc_config_address_ptr->default_smds_address.data_len > LPA_SERVER_ADDRESS_LEN_MAX)
  {
    LPA_MSG_ERR_2("Invalid server len retrieved from card: smdp_address len=0x%x, smds address len=0x%x",
                  euicc_config_address_ptr->default_smdp_address.data_len,
                  euicc_config_address_ptr->default_smds_address.data_len);
    return LPA_GENERIC_ERROR;
  }

  /* Make sure we clean up the stale value from global before populating
     the new one */
  if(euicc_config_address_ptr->default_smdp_address.data_len > 0 &&
     euicc_config_address_ptr->default_smdp_address.data_ptr)
  {
    lpa_lpd_global_info[slot_id].default_smdp_address.data_len =
      euicc_config_address_ptr->default_smdp_address.data_len;

    memscpy(lpa_lpd_global_info[slot_id].default_smdp_address.data,
            sizeof(lpa_lpd_global_info[slot_id].default_smdp_address.data),
            euicc_config_address_ptr->default_smdp_address.data_ptr,
            euicc_config_address_ptr->default_smdp_address.data_len);
  }

  if(euicc_config_address_ptr->default_smds_address.data_len > 0 &&
     euicc_config_address_ptr->default_smds_address.data_ptr)
  {
    lpa_lpd_global_info[slot_id].root_smds_address.data_len =
      euicc_config_address_ptr->default_smds_address.data_len;

    memscpy(lpa_lpd_global_info[slot_id].root_smds_address.data,
            sizeof(lpa_lpd_global_info[slot_id].root_smds_address.data),
            euicc_config_address_ptr->default_smds_address.data_ptr,
            euicc_config_address_ptr->default_smds_address.data_len);
  }

  /* Now we can go ahead and send Initiate Auth request */
  lpa_status = lpa_state_change(LPA_STATE_INIT, LPA_STATE_INITIATE_AUTH_SENT, slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  memset(&initiate_auth_req, 0x00, sizeof(lpa_http_initiate_auth_req_type));

  memscpy(initiate_auth_req.euicc_challenge,
          sizeof(initiate_auth_req.euicc_challenge),
          lpa_lpd_global_info[slot_id].euicc_challenge,
          sizeof(lpa_lpd_global_info[slot_id].euicc_challenge));
  initiate_auth_req.euicc_info1 = lpa_lpd_global_info[slot_id].euicc_info1;
  if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_DEFAULT_SMDP)
  {
    /* Add Profile initiated using default SMDP address */
    initiate_auth_req.smdp_address = lpa_lpd_global_info[slot_id].default_smdp_address;
  }
  else if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_SMDS)
  {
    /* Add Profile initiated using root SMDS address */
    initiate_auth_req.smdp_address = lpa_lpd_global_info[slot_id].root_smds_address;
  }
  else
  {
    /* Add Profile initiated using SMDS address in activation code */
    initiate_auth_req.smdp_address = lpa_lpd_global_info[slot_id].activation_code.smdp_address;
  }
  initiate_auth_req.slot_id = slot_id;
  initiate_auth_req.lpd_callback = lpa_lpd_http_cb;
  initiate_auth_req.user_data_ptr = (void*) LPA_LPD_INITIATE_AUTH_RESP_CMD;

  return lpa_http_build_and_send_initiate_auth(&initiate_auth_req);
} /* lpa_lpd_process_get_euicc_config_address_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_LOAD_BPP_RESP

DESCRIPTION
  Function processes the LOAD BPP response from the eUICC.
  If there is any error in this function, we must FREE all the BPP pointers
  stored in the global as part of GET BPP to SMDP earlier.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_load_bpp_resp (
  lpa_lpd_load_bpp_resp_type                *load_bpp_resp_ptr,
  lpa_slot_id_enum_type                      slot_id
)
{
  lpa_result_enum_type         lpa_status                   = LPA_SUCCESS;
  lpa_euicc_phase_enum_type    euicc_phase                  = LPA_PHASE1_1_EUICC;
  boolean                      is_profile_download_complete = FALSE;
  do
  {
    if(load_bpp_resp_ptr == NULL ||
       slot_id >= LPA_NUM_SLOTS)
    {
      lpa_status = LPA_GENERIC_ERROR;
      break;
    }

    /* Write the data received from card to EFS file for testing purposes */
    (void) lpa_efs_write(LPA_FILE_LOAD_BPP_FROM_CARD,
                         load_bpp_resp_ptr->load_bpp.data_len,
                         load_bpp_resp_ptr->load_bpp.data_ptr,
                         slot_id);

    if(load_bpp_resp_ptr->load_bpp.data_len > 0 &&
       NULL != load_bpp_resp_ptr->load_bpp.data_ptr)
    {
      /* Card sends a non-null response only after the last LOAD BPP TLV
         is processed by it. Since we received non-null response here, treat
         it as completion of load BPP process.
         This response contains Response and Receipt that needs to be sent to
         the SMDP via build and handle installation result or Notifications,
         depending upon the phase */

      is_profile_download_complete = TRUE;

      lpa_status = lpa_state_change(LPA_STATE_GET_BPP_RESP_RCVD,
                                    LPA_STATE_HANDLE_INSTALLATION_RESULT_SENT,
                                    slot_id);
      if(lpa_status != LPA_SUCCESS)
      {
        break;
      }

      (void) lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
      if(euicc_phase == LPA_PHASE1_1_EUICC ||
         euicc_phase == LPA_PHASE1_EUICC)
      {
        lpa_http_handle_installation_result_req_type   profile_installation_result_req;

        memset(&profile_installation_result_req,
               0x00,
               sizeof(lpa_http_handle_installation_result_req_type));
        profile_installation_result_req.profile_installation_result = load_bpp_resp_ptr->load_bpp;
        profile_installation_result_req.slot_id = slot_id;
        profile_installation_result_req.lpd_callback = lpa_lpd_http_cb;
        if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_DEFAULT_SMDP)
        {
          /* Add Profile initiated using default SMDP address */
          profile_installation_result_req.smdp_address = lpa_lpd_global_info[slot_id].default_smdp_address;
        }
        else if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_SMDS)
        {
          /* Add Profile initiated using root SMDS address */
          profile_installation_result_req.smdp_address = lpa_lpd_global_info[slot_id].root_smds_address;
        }
        else
        {
          /* Add Profile initiated using SMDS address in activation code */
          profile_installation_result_req.smdp_address = lpa_lpd_global_info[slot_id].activation_code.smdp_address;
        }
        profile_installation_result_req.user_data_ptr = (void*) LPA_LPD_HANDLE_INSTALLATION_RESULT_RESP_CMD;
        (void) lpa_http_build_and_send_handle_installation_result(
                 &profile_installation_result_req);
      }
      else
      {
        lpa_notification_retrieve_and_send(slot_id);
      }
      lpa_status = lpa_lpd_get_profile_installation_result(load_bpp_resp_ptr->load_bpp.data_len,
                                                           load_bpp_resp_ptr->load_bpp.data_ptr,
                                                           slot_id);
    }
    /* Check global and see if there are any pending BPP TLVs that need to be
       sent to the card. If so, send the STORE DATA */
    else if(lpa_lpd_global_info[slot_id].last_loaded_bpp_tlv_counter < lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_num)
    {
      /* Stays in LPA_STATE_GET_BPP_RESP_RCVD state */
      lpa_status = lpa_mmgsdi_store_data(slot_id,
                                         LPA_LOAD_BPP_APDU_DATA_P1,
                                         LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                         lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[lpa_lpd_global_info[slot_id].last_loaded_bpp_tlv_counter],
                                         TRUE,
                                         lpa_lpd_mmgsdi_cb,
                                         (void*) LPA_LPD_LOAD_BPP_RESP_CMD);
      lpa_lpd_global_info[slot_id].last_loaded_bpp_tlv_counter ++;
    }
    else
    {
      lpa_status = LPA_GENERIC_ERROR;
    }
  }
  while (0);

  if(lpa_status != LPA_SUCCESS ||
     is_profile_download_complete)
  {
    uint8 i = 0;

    /* Free the global that contains the BPPs retrieved earlier from SMDP.
       We no longer need that global data as we are reporting error to
       the client (via an event) */
    while(slot_id < LPA_NUM_SLOTS &&
          i < lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_num &&
          i < (sizeof(lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data)/sizeof(lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[0])) &&
          lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[i].data_ptr)
    {
      LPA_FREE(lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[i].data_ptr);
      i++;
    }
  }

  if(lpa_status == LPA_SUCCESS &&
     is_profile_download_complete)
  {
    lpa_status = lpa_state_change(LPA_STATE_HANDLE_INSTALLATION_RESULT_SENT,
                                  LPA_STATE_IDLE,
                                  slot_id);
    if(lpa_status == LPA_SUCCESS)
    {
      /* Send profile download complete indication to the client irrespective of the
           handle profile installation result response from the server */
      lpa_lpd_send_profile_download_event(LPA_SUCCESS,
                                          slot_id,
                                          0x64,  /* 100% */
                                          TRUE,
                                          TRUE);
    }
  }

  return lpa_status;
} /* lpa_lpd_process_load_bpp_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCSES_GET_BPP_RESP

DESCRIPTION
  Function processes the http response from SMDP for GET BPP and processes it.
  The response received from SMDP is already in ASN1 format.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_get_bpp_resp (
  lpa_lpd_get_bpp_resp_type               *get_bpp_resp_ptr,
  lpa_slot_id_enum_type                    slot_id
)
{
  lpa_result_enum_type         lpa_status = LPA_SUCCESS;
  lpa_euicc_phase_enum_type    euicc_phase = LPA_PHASE1_1_EUICC;

  if(get_bpp_resp_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_lpd_global_info[slot_id].last_loaded_bpp_tlv_counter = 0;
  memset(&lpa_lpd_global_info[slot_id].bpp_tlvs, 0x00, sizeof(lpa_lpd_global_info[slot_id].bpp_tlvs));

   /* Write the data received from lpa_http to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_JSON_DECODED_GET_BPP,
                       get_bpp_resp_ptr->bpp.data_len,
                       get_bpp_resp_ptr->bpp.data_ptr,
                       slot_id);

  /* We received the HTTP response for Get BPP which contains
     the profile info that needs to be sent in chunks to the card.
     The parsing function will store the parsed tlvs_num and tlvs_data
     to the global. From that point on, we need to sent separate STORE
     DATA commands for each of those tlvs */
  (void) lpa_retrieve_euicc_phase(slot_id, &euicc_phase);
  if(euicc_phase == LPA_PHASE1_1_EUICC ||
     euicc_phase == LPA_PHASE1_EUICC)
  {
    lpa_status = lpa_util_parse_bpp(get_bpp_resp_ptr->bpp,
                                    &lpa_lpd_global_info[slot_id].bpp_tlvs);
  }
  else
  {
    uint8   meta_data_start_index = 0;
    uint8   meta_data_total_index = 0;

    lpa_status = lpa_asn1_decode_bpp(get_bpp_resp_ptr->bpp.data_len,
                                     get_bpp_resp_ptr->bpp.data_ptr,
                                     &lpa_lpd_global_info[slot_id].bpp_tlvs,
                                     &meta_data_start_index,
                                     &meta_data_total_index);

    /* Compare profile meta data received in bound profile package with profile
       meta data retrieved from response of ES9+.Authenticate Client. If both
       does not match, LPA will reject  downlod */
    if(lpa_status == LPA_SUCCESS && euicc_phase == LPA_PHASE2_1_EUICC)
    {
      uint16  tlv_index             = 0;
      uint32  meta_data_len         = 0;
      boolean match_found           = FALSE;

      for(tlv_index = meta_data_start_index; tlv_index < meta_data_start_index + meta_data_total_index &&
                         tlv_index  < lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_num &&
                         tlv_index < LPA_UTIL_BPP_MAX_TLVS; tlv_index++)
      {
        uim_common_data_type bpp_metadata            = {0, NULL};
        uint16               bpp_metadata_length_len = LPA_PROFILE_METADATA_TAG_LEN;

        bpp_metadata.data_len = lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[tlv_index].data_len;
        bpp_metadata.data_ptr = lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[tlv_index].data_ptr;

        if(FALSE == uim_common_calc_data_object_len(&bpp_metadata,
                                                    &bpp_metadata_length_len,
                                                    NULL))
        {
          UIM_MSG_ERR_1("Incorrect calculation of BPP length, TLV Index: 0x%x", tlv_index);
          match_found = FALSE;
          break;
        }

        /* BPP Metadata length of length bytes returns length including TAG byte length.
           So, decrement the TAG byte length */
        bpp_metadata_length_len = bpp_metadata_length_len - LPA_PROFILE_METADATA_TAG_LEN;

        UIM_MSG_HIGH_1("Meta Data length: 0x%x", bpp_metadata_length_len);

        /* First byte of TLV data is a tag, next three bytes are for length.
           There are 8 byte MAC at the end of every TLV data chunk. */
        if((lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[tlv_index].data_len <= LPA_PROFILE_METADATA_TAG_LEN + bpp_metadata_length_len + LPA_PROFILE_METADATA_MAC_LEN) ||
           (meta_data_len +
              lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[tlv_index].data_len -
              (LPA_PROFILE_METADATA_TAG_LEN + bpp_metadata_length_len + LPA_PROFILE_METADATA_MAC_LEN) >
            lpa_lpd_global_info[slot_id].profile_metadata.data_len))
        {
          match_found = FALSE;
          break;
        }

        if(lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[tlv_index].data_ptr == NULL)
        {
          match_found = FALSE;
          break;
        }

        if(memcmp(&lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[tlv_index].data_ptr[LPA_PROFILE_METADATA_TAG_LEN + bpp_metadata_length_len],
                  &lpa_lpd_global_info[slot_id].profile_metadata.data_ptr[meta_data_len],
                  lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[tlv_index].data_len - (LPA_PROFILE_METADATA_TAG_LEN + bpp_metadata_length_len + LPA_PROFILE_METADATA_MAC_LEN)) != 0)
        {
          match_found = FALSE;
          break;
        }
        else
        {
          match_found = TRUE;
        }
        meta_data_len = meta_data_len + 
                        lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[tlv_index].data_len - 
                        (LPA_PROFILE_METADATA_TAG_LEN + bpp_metadata_length_len + LPA_PROFILE_METADATA_MAC_LEN);

        if(meta_data_len != lpa_lpd_global_info[slot_id].profile_metadata.data_len)
        {
          match_found = FALSE;
        }
      }

      if(match_found == FALSE)
      {
        /* Cancel the ongoing profile download */
        LPA_MSG_HIGH_0("Meta data mismatch error. Cancelling the profile download");
        
        lpa_lpd_global_info[slot_id].cancel_session_reason =
          LPA_LPD_CANCEL_SESSION_META_DATA_MISMATCH;

        return lpa_lpd_start_cancel_session_procedure(
                 slot_id,
                 LPA_LPD_CANCEL_SESSION_META_DATA_MISMATCH);
      }
    }
  }

  if(lpa_status != LPA_SUCCESS ||
     lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_num == 0)
  {
    LPA_MSG_HIGH_1("Error parsing BPP, tlvs_num=0x%x",
                   lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_num);

    if(euicc_phase == LPA_PHASE2_1_EUICC)
    {
      /* Cancel the ongoing profile download */
      LPA_MSG_HIGH_1("Load BPP execution error. Cancelling the profile download, status=0x%x", lpa_status);

      lpa_lpd_global_info[slot_id].cancel_session_reason =
        LPA_LPD_CANCEL_SESSION_LOAD_BPP_EXECUTION_ERROR;

      return lpa_lpd_start_cancel_session_procedure(
               slot_id,
               LPA_LPD_CANCEL_SESSION_LOAD_BPP_EXECUTION_ERROR);
    }

    return LPA_GENERIC_ERROR;
  }

  lpa_status = lpa_state_change(LPA_STATE_GET_BPP_SENT, LPA_STATE_GET_BPP_RESP_RCVD, slot_id);

  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     LPA_LOAD_BPP_APDU_DATA_P1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     lpa_lpd_global_info[slot_id].bpp_tlvs.tlvs_data[lpa_lpd_global_info[slot_id].last_loaded_bpp_tlv_counter],
                                     TRUE,
                                     lpa_lpd_mmgsdi_cb,
                                     (void*) LPA_LPD_LOAD_BPP_RESP_CMD);
  if(lpa_status == LPA_SUCCESS)
  {
    lpa_lpd_global_info[slot_id].last_loaded_bpp_tlv_counter++;
  }

  return lpa_status;
} /* lpa_lpd_process_get_bpp_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCSES_INITIATE_AUTH_LEGACY_RESP

DESCRIPTION
  Function processes the http response for INITIATE AUTH and processes it
  according to phase1/1.1 of SGP22.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_initiate_auth_legacy_resp (
  lpa_lpd_initiate_auth_resp_type            *initiate_auth_ptr,
  lpa_slot_id_enum_type                       slot_id
)
{
  lpa_result_enum_type                    lpa_status            = LPA_SUCCESS;
  lpa_data_type                           prepare_download_encoded;
  lpa_asn1_prepare_download_request_type  prepare_download_raw;

  if(initiate_auth_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS ||
     initiate_auth_ptr->transaction_id.data_len == 0 ||
     initiate_auth_ptr->transaction_id.data_len > LPA_TRANSACTION_ID_LEN ||
     initiate_auth_ptr->transaction_id_string.data_len == 0)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&prepare_download_encoded, 0x00, sizeof(prepare_download_encoded));
  memset(&prepare_download_raw, 0x00, sizeof(prepare_download_raw));

  lpa_status = lpa_state_change(LPA_STATE_INITIATE_AUTH_SENT, LPA_STATE_INITIATE_AUTH_RESP_RCVD, slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  /* Save the transactionID in global for getBPP command */
  lpa_lpd_global_info[slot_id].transaction_id_string.data_len = initiate_auth_ptr->transaction_id_string.data_len;
  lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr =
                                    (uint8 *)LPA_MALLOC(initiate_auth_ptr->transaction_id_string.data_len);
  if(NULL == lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  memscpy(lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr,
          lpa_lpd_global_info[slot_id].transaction_id_string.data_len,
          initiate_auth_ptr->transaction_id_string.data_ptr,
          initiate_auth_ptr->transaction_id_string.data_len);

  if (lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len > 0 &&
      lpa_lpd_global_info[slot_id].hashed_confirmation_code_tx_id.data_len == 0)
  {
    lpa_data_type                           code_to_be_hashed;

    memset(&code_to_be_hashed, 0x00, sizeof(code_to_be_hashed));

    code_to_be_hashed.data_len = lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len + initiate_auth_ptr->transaction_id.data_len;

    code_to_be_hashed.data_ptr = LPA_MALLOC(code_to_be_hashed.data_len);
    if (code_to_be_hashed.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }

    memscpy(code_to_be_hashed.data_ptr,
            code_to_be_hashed.data_len,
            lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_ptr,
            lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len);

    memscpy((code_to_be_hashed.data_ptr + lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len),
            (code_to_be_hashed.data_len - lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len),
          initiate_auth_ptr->transaction_id.data_ptr,
          initiate_auth_ptr->transaction_id.data_len);

    lpa_status = lpa_util_hash_sha256(code_to_be_hashed.data_ptr,
                                      code_to_be_hashed.data_len,
                                      &(lpa_lpd_global_info[slot_id].hashed_confirmation_code_tx_id));
    LPA_FREE(code_to_be_hashed.data_ptr);

    if(lpa_status != LPA_SUCCESS)
    {
      LPA_FREE(lpa_lpd_global_info[slot_id].hashed_confirmation_code_tx_id.data_ptr);
      return lpa_status;
    }
  }
  /* We received the HTTP response for InitAuth which contains:
     transactionID
     dpSigned1
     smdpSignature1
     certFormatTobeUsed
     curveTobeUsed
     smdpCertificate.
     Call asn1 encoder to encode the PrepareDownload request... this function
     mallocs pointer inside prepare_download_data, so make sure to free it
     after sending STORE DATA to the card */
  prepare_download_raw.dp_signed_raw = initiate_auth_ptr->dp_signed1;
  prepare_download_raw.smdp_signature_raw = initiate_auth_ptr->smdp_signature1;
  prepare_download_raw.activation_code_token.data_ptr =
    (uint8*) lpa_lpd_global_info[slot_id].activation_code.activation_token.data;
  prepare_download_raw.activation_code_token.data_len =
    lpa_lpd_global_info[slot_id].activation_code.activation_token.data_len;
  prepare_download_raw.smdpoid_string =
    lpa_lpd_global_info[slot_id].activation_code.smdp_id;
  prepare_download_raw.hashed_confirmation_code =
    lpa_lpd_global_info[slot_id].hashed_confirmation_code_tx_id;
  prepare_download_raw.cert_format_to_be_used_raw =
    initiate_auth_ptr->cert_format_to_be_used;
  prepare_download_raw.curve_to_be_used_raw = initiate_auth_ptr->curve_to_be_used;
  prepare_download_raw.smdp_certificate_raw = initiate_auth_ptr->smdp_certificate;
  lpa_status = lpa_util_get_device_info(&prepare_download_raw.device_info, slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    LPA_MSG_HIGH_1("Error in lpa_util_get_device_info, status=0x%x",
                   lpa_status);
    return LPA_GENERIC_ERROR;
  }

  /* Following encoder may malloc prepare_download_encoded - so make sure
     to free it before exiting the function */
  lpa_status = lpa_asn1_encode_prepare_download_request(
                 prepare_download_raw, &prepare_download_encoded, slot_id);

  /* Write the data received from DS to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_ASN1_ENCODED_PREP_DOWNLOAD,
                       prepare_download_encoded.data_len,
                       prepare_download_encoded.data_ptr,
                       slot_id);

  /* Send the prepare download STORE DATA to the card */
  if(lpa_status == LPA_SUCCESS &&
     prepare_download_encoded.data_len > 0 &&
     prepare_download_encoded.data_ptr)
  {
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PREPARE_DOWNLOAD_APDU_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       prepare_download_encoded,
                                       TRUE,
                                       lpa_lpd_mmgsdi_cb,
                                       (void*) LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD);
  }
  else
  {
    LPA_MSG_HIGH_1("Error in lpa_asn1_encode_prepare_download_request, status=0x%x",
                   lpa_status);
  }

  LPA_FREE(prepare_download_encoded.data_ptr);

  return lpa_status;
} /* lpa_lpd_process_initiate_auth_legacy_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCSES_INITIATE_AUTH_RESP

DESCRIPTION
  Function processes the http response for INITIATE AUTH and processes it.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_initiate_auth_resp (
  lpa_lpd_initiate_auth_resp_type            *initiate_auth_ptr,
  lpa_slot_id_enum_type                       slot_id
)
{
  lpa_result_enum_type                       lpa_status            = LPA_SUCCESS;
  lpa_data_type                              data;
  lpa_asn1_ctx_params_for_common_auth_type   ctx_params_raw;

  if(initiate_auth_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS ||
     initiate_auth_ptr->transaction_id.data_len == 0 ||
     initiate_auth_ptr->transaction_id.data_len > LPA_TRANSACTION_ID_LEN ||
     initiate_auth_ptr->transaction_id_string.data_len == 0)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&data, 0x00, sizeof(data));
  memset(&ctx_params_raw, 0x00, sizeof(ctx_params_raw));

  lpa_status = lpa_state_change(LPA_STATE_INITIATE_AUTH_SENT, LPA_STATE_INITIATE_AUTH_RESP_RCVD, slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  /* Save the transactionID in global for getBPP command */
  lpa_lpd_global_info[slot_id].transaction_id_string.data_len = initiate_auth_ptr->transaction_id_string.data_len;
  lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr =
                                    (uint8 *)LPA_MALLOC(initiate_auth_ptr->transaction_id_string.data_len);
  if(NULL == lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  memscpy(lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr,
          lpa_lpd_global_info[slot_id].transaction_id_string.data_len,
          initiate_auth_ptr->transaction_id_string.data_ptr,
          initiate_auth_ptr->transaction_id_string.data_len);

  /* We received the HTTP response for InitAuth which contains:
     transactionID
     dpSigned1
     smdpSignature1
     euicc_ci_pk_id_to_be_used
     smdpCertificate
     Call asn1 encoder to encode the AuthenticateServer request...this function
     mallocs pointer inside auth_server_raw, so make sure to free them
     after sending STORE DATA to the card */
  lpa_status = lpa_util_get_device_info(&ctx_params_raw.device_info, slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    LPA_MSG_HIGH_1("Error in lpa_util_get_device_info, status=0x%x",
                   lpa_status);
    return LPA_GENERIC_ERROR;
  }

  if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_ACTIVATION_CODE)
  {
    ctx_params_raw.matching_id.data_ptr = (uint8*) lpa_lpd_global_info[slot_id].activation_code.activation_token.data;
    ctx_params_raw.matching_id.data_len = lpa_lpd_global_info[slot_id].activation_code.activation_token.data_len;
  }
  else
  {
    /* Matching ID is empty */
    ctx_params_raw.matching_id.data_ptr = NULL;
    ctx_params_raw.matching_id.data_len = 0;
  }
  lpa_status = lpa_asn1_encode_authenticate_server_request(
                 initiate_auth_ptr->dp_signed1,
                 initiate_auth_ptr->smdp_signature1,
                 initiate_auth_ptr->euicc_ci_pk_id_to_be_used,
                 initiate_auth_ptr->smdp_certificate,
                 ctx_params_raw,
                 &data,
                 slot_id);

  /* Write the data received from DS to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_ASN1_ENCODED_AUTHENTICATE_SERVER,
                       data.data_len,
                       data.data_ptr,
                       slot_id);

  /* Send the Authenticate Server STORE DATA to the card */
  if(lpa_status == LPA_SUCCESS &&
     data.data_len > 0 &&
     data.data_ptr)
  {
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PHASE_2_STORE_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       data,
                                       TRUE,
                                       lpa_lpd_mmgsdi_cb,
                                       (void*) LPA_LPD_AUTHENTICATE_SERVER_RESP_CMD);
  }
  else
  {
    LPA_MSG_HIGH_1("Error in lpa_asn1_encode_authenticate_server_request, status=0x%x",
                   lpa_status);
  }

  LPA_FREE(data.data_ptr);

  return lpa_status;
} /* lpa_lpd_process_initiate_auth_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCSES_AUTHENTICATE_CLIENT_RESP

DESCRIPTION
  Function processes the http response for AUTHENTICATE CLIENT and processes
  it.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_authenticate_client_resp (
  lpa_lpd_authenticate_client_resp_type      *authenticate_client_resp_ptr,
  lpa_slot_id_enum_type                       slot_id
)
{
  lpa_result_enum_type                    lpa_status            = LPA_SUCCESS;
  lpa_data_type                           prepare_download_encoded;
  lpa_asn1_prepare_download_request_type  prepare_download_raw;
  lpa_profile_policy_rules_type           ppr;
  lpa_rules_auth_table_type               rules_auth_table;
  boolean                                 send_event            = FALSE;
  boolean                                 user_consent_needed   = FALSE;
  boolean                                 ppr_ids_allowed       = TRUE;
  lpa_ppr_ids_type                        ppr_ids_profile_metadata;

  if(authenticate_client_resp_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS ||
     authenticate_client_resp_ptr->transaction_id.data_len == 0 ||
     authenticate_client_resp_ptr->transaction_id_string.data_len == 0 ||
     authenticate_client_resp_ptr->transaction_id.data_len > LPA_TRANSACTION_ID_LEN ||
     authenticate_client_resp_ptr->smdp_signed2.data_len == 0 ||
     authenticate_client_resp_ptr->smdp_signature2.data_len == 0 ||
     authenticate_client_resp_ptr->smdp_certificate.data_len == 0)
  {
    return LPA_GENERIC_ERROR;
  }

  memset(&prepare_download_encoded, 0x00, sizeof(prepare_download_encoded));
  memset(&prepare_download_raw, 0x00, sizeof(prepare_download_raw));
  memset(&ppr, 0x00, sizeof(ppr));
  memset(&rules_auth_table, 0x00, sizeof(rules_auth_table));
  memset(&ppr_ids_profile_metadata, 0x00, sizeof(ppr_ids_profile_metadata));

  lpa_status = lpa_state_change(LPA_STATE_AUTHENTICATE_CLIENT_SENT, LPA_STATE_AUTHENTICATE_CLIENT_RESP_RCVD, slot_id);
  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  /* First copy over the authenticate_client response to the global that
     can later be used to trigger prepare_download incase we need to
     defer prepare_download until we receive user confirmation based on the
     PPR and the RAT */

  /* Clear the transaction ID that was received as part of InitiateAuth
     response and copy over the transactionID in global for getBPP command */
  LPA_FREE(lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr);
  lpa_lpd_global_info[slot_id].transaction_id_string.data_len = 0;

  lpa_lpd_global_info[slot_id].transaction_id_string.data_len =
    authenticate_client_resp_ptr->transaction_id_string.data_len;
  lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr =
    (uint8 *)LPA_MALLOC(authenticate_client_resp_ptr->transaction_id_string.data_len);
  if(NULL == lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  memscpy(lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr,
          lpa_lpd_global_info[slot_id].transaction_id_string.data_len,
          authenticate_client_resp_ptr->transaction_id_string.data_ptr,
          authenticate_client_resp_ptr->transaction_id_string.data_len);

  if (lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len > 0 &&
      lpa_lpd_global_info[slot_id].hashed_confirmation_code_tx_id.data_len == 0)
  {
    lpa_data_type                           code_to_be_hashed;

    memset(&code_to_be_hashed, 0x00, sizeof(code_to_be_hashed));

    code_to_be_hashed.data_len = lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len + authenticate_client_resp_ptr->transaction_id.data_len;

    code_to_be_hashed.data_ptr = LPA_MALLOC(code_to_be_hashed.data_len);
    if (code_to_be_hashed.data_ptr == NULL)
    {
      return LPA_ERROR_HEAP_EXHAUSTED;
    }

    memscpy(code_to_be_hashed.data_ptr,
            code_to_be_hashed.data_len,
            lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_ptr,
            lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len);

    memscpy((code_to_be_hashed.data_ptr + lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len),
            (code_to_be_hashed.data_len - lpa_lpd_global_info[slot_id].hashed_confirmation_code.data_len),
            authenticate_client_resp_ptr->transaction_id.data_ptr,
            authenticate_client_resp_ptr->transaction_id.data_len);

    lpa_status = lpa_util_hash_sha256(code_to_be_hashed.data_ptr,
                                      code_to_be_hashed.data_len,
                                      &(lpa_lpd_global_info[slot_id].hashed_confirmation_code_tx_id));
    LPA_FREE(code_to_be_hashed.data_ptr);

    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }
  }

  /* Also copy over the smdp_signed2, smdp_signature2 and smdp_certificate
     as we might need to postpone sending prepareDownload to after receiving
     a user consent. Make sure we clean any stale values */
  LPA_FREE(lpa_lpd_global_info[slot_id].smdp_signed2.data_ptr);
  lpa_lpd_global_info[slot_id].smdp_signed2.data_len = 0;

  LPA_FREE(lpa_lpd_global_info[slot_id].smdp_signature2.data_ptr);
  lpa_lpd_global_info[slot_id].smdp_signature2.data_len = 0;

  LPA_FREE(lpa_lpd_global_info[slot_id].smdp_certificate.data_ptr);
  lpa_lpd_global_info[slot_id].smdp_certificate.data_len = 0;

  LPA_FREE(lpa_lpd_global_info[slot_id].profile_metadata.data_ptr);
  lpa_lpd_global_info[slot_id].profile_metadata.data_len = 0;

  lpa_lpd_global_info[slot_id].smdp_signed2.data_len =
    authenticate_client_resp_ptr->smdp_signed2.data_len;

  lpa_lpd_global_info[slot_id].smdp_signed2.data_ptr =
    (uint8 *)LPA_MALLOC(authenticate_client_resp_ptr->smdp_signed2.data_len);
  if(NULL == lpa_lpd_global_info[slot_id].smdp_signed2.data_ptr)
  {
    LPA_FREE(lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  memscpy(lpa_lpd_global_info[slot_id].smdp_signed2.data_ptr,
          lpa_lpd_global_info[slot_id].smdp_signed2.data_len,
          authenticate_client_resp_ptr->smdp_signed2.data_ptr,
          authenticate_client_resp_ptr->smdp_signed2.data_len);

  lpa_lpd_global_info[slot_id].smdp_signature2.data_len =
    authenticate_client_resp_ptr->smdp_signature2.data_len;
  lpa_lpd_global_info[slot_id].smdp_signature2.data_ptr =
    (uint8 *)LPA_MALLOC(authenticate_client_resp_ptr->smdp_signature2.data_len);
  if(NULL == lpa_lpd_global_info[slot_id].smdp_signature2.data_ptr)
  {
    LPA_FREE(lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr);
    LPA_FREE(lpa_lpd_global_info[slot_id].smdp_signed2.data_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  memscpy(lpa_lpd_global_info[slot_id].smdp_signature2.data_ptr,
          lpa_lpd_global_info[slot_id].smdp_signature2.data_len,
          authenticate_client_resp_ptr->smdp_signature2.data_ptr,
          authenticate_client_resp_ptr->smdp_signature2.data_len);

  lpa_lpd_global_info[slot_id].smdp_certificate.data_len =
    authenticate_client_resp_ptr->smdp_certificate.data_len;
  lpa_lpd_global_info[slot_id].smdp_certificate.data_ptr =
    (uint8 *)LPA_MALLOC(authenticate_client_resp_ptr->smdp_certificate.data_len);
  if(NULL == lpa_lpd_global_info[slot_id].smdp_certificate.data_ptr)
  {
    LPA_FREE(lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr);
    LPA_FREE(lpa_lpd_global_info[slot_id].smdp_signed2.data_ptr);
    LPA_FREE(lpa_lpd_global_info[slot_id].smdp_signature2.data_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }
  memscpy(lpa_lpd_global_info[slot_id].smdp_certificate.data_ptr,
          lpa_lpd_global_info[slot_id].smdp_certificate.data_len,
          authenticate_client_resp_ptr->smdp_certificate.data_ptr,
          authenticate_client_resp_ptr->smdp_certificate.data_len);

  /* We received ProfileMetadata in the AuthenticateClient response. We
     need to check the PRRID/Owner in the Metadata and verify it against
     the entries in the RAT table. If the PRRID/Owner entries of this
     profile are allowed as per the RAT, send indication to the user.
     If RAT table says that we need to wait for a user-confirmation, move
     our state appropriately and wait.
     If RAT table says that PPRIDs set in the ProfileMetadata are not
     allowed, cancel the download process with error 'PPRID not allowed'. */
  lpa_status = lpa_asn1_decode_profile_metadata(authenticate_client_resp_ptr->profile_metadata.data_len,
                                                authenticate_client_resp_ptr->profile_metadata.data_ptr,
                                                &ppr.operator_id,
                                                &ppr.ppr_ids);

  if(lpa_status != LPA_SUCCESS)
  {
    return lpa_status;
  }

  LPA_MSG_MED_3("decode_profile_metadata, status=0x%x, ppr1=0x%x, ppr2=0x%x", lpa_status, ppr.ppr_ids.ppr1, ppr.ppr_ids.ppr2);
  LPA_MSG_MED_3("mnc[0]=0x%x, mnc[1]=0x%x, mnc[2]=0x%x", ppr.operator_id.mnc.mnc_data[0], ppr.operator_id.mnc.mnc_data[1], ppr.operator_id.mnc.mnc_data[2]);
  LPA_MSG_MED_3("mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x", ppr.operator_id.mcc[0], ppr.operator_id.mcc[1], ppr.operator_id.mcc[2]);
  LPA_MSG_MED_3("gid1.gid_len=0x%x, gid1[0]=0x%x, gid1[1]=0x%x", ppr.operator_id.gid1.gid_len, ppr.operator_id.gid1.gid_data[0], ppr.operator_id.gid1.gid_data[1]);
  LPA_MSG_MED_3("gid2.gid_len=0x%x, gid2[0]=0x%x, gid2[1]=0x%x", ppr.operator_id.gid2.gid_len, ppr.operator_id.gid2.gid_data[0], ppr.operator_id.gid2.gid_data[1]);

  /* Store profile meta data. It will be compared with profile meta data
     received in bound profile package */

  lpa_lpd_global_info[slot_id].profile_metadata.data_ptr =
    (uint8 *)LPA_MALLOC(authenticate_client_resp_ptr->profile_metadata.data_len);

  if(NULL == lpa_lpd_global_info[slot_id].profile_metadata.data_ptr)
  {
    LPA_FREE(lpa_lpd_global_info[slot_id].transaction_id_string.data_ptr);
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  lpa_lpd_global_info[slot_id].profile_metadata.data_len =
    authenticate_client_resp_ptr->profile_metadata.data_len;

  memscpy(lpa_lpd_global_info[slot_id].profile_metadata.data_ptr,
          lpa_lpd_global_info[slot_id].profile_metadata.data_len,
          authenticate_client_resp_ptr->profile_metadata.data_ptr,
          authenticate_client_resp_ptr->profile_metadata.data_len);

  /* Store the PPRIDs of the profiles being dowloaded */
  memscpy(&ppr_ids_profile_metadata,
          sizeof(ppr_ids_profile_metadata),
          &ppr.ppr_ids,
          sizeof(ppr.ppr_ids));
  if(ppr.ppr_ids.ppr1 ||
     ppr.ppr_ids.ppr2 ||
     ppr.ppr_ids.ppr3)
  {
    /* If any of the PPRID is set, we need to compare that against the RAT and
       check if the RAT table allows that PPRID or not.
       If any of the set PPRID is not allowed, we should cancel the ongoing
       profile download with error 'PPRID not allowed' as per section 3.1.3
       'Profile Download and Installation' of spec SGP22 */
    lpa_status = lpa_cmd_compare_rat_and_update_pprids(&ppr,
                                                       lpa_lpd_global_info[slot_id].rat.rules_auth_table);
    if(lpa_status != LPA_SUCCESS)
    {
      return lpa_status;
    }

    /* Check if any PPR id is allowed for this operator. If so, we need to send
       the event to the user */
    if((ppr_ids_profile_metadata.ppr1 &&
        !ppr.ppr_ids.ppr1) ||
       (ppr_ids_profile_metadata.ppr2 &&
        !ppr.ppr_ids.ppr2) ||
       (ppr_ids_profile_metadata.ppr3 &&
        !ppr.ppr_ids.ppr3) ||
       (ppr_ids_profile_metadata.ppr1 &&
        lpa_lpd_global_info[slot_id].any_operational_profile))
    {
      ppr_ids_allowed = FALSE;
    }
    else if(ppr.ppr_ids.ppr1 ||
            ppr.ppr_ids.ppr2 ||
            ppr.ppr_ids.ppr3)
    {
      send_event = TRUE;
      if(ppr.user_consent.ppr1_user_consent_required ||
         ppr.user_consent.ppr2_user_consent_required ||
         ppr.user_consent.ppr3_user_consent_required)
      {
        user_consent_needed = TRUE;
      }
    }
  }

  if(ppr_ids_allowed == FALSE)
  {
    /* Cancel the ongoing profile download */
    LPA_MSG_HIGH_1("PPRID not allowed. Cancelling the profile download, status=0x%x", lpa_status);
    lpa_lpd_global_info[slot_id].cancel_session_reason =
      LPA_LPD_CANCEL_SESSION_PPR_ID_NOT_ALLOWED;
    return lpa_lpd_start_cancel_session_procedure(
             slot_id,
             LPA_LPD_CANCEL_SESSION_PPR_ID_NOT_ALLOWED);
  }

  if(send_event)
  {
    lpa_lpd_request_user_consent_event(
      LPA_SUCCESS,
      slot_id,
      ppr.ppr_ids,
      user_consent_needed);

    LPA_MSG_MED_2("Sending PPR info to user, user_consent_needed=0x%x, status=0x%x", user_consent_needed, lpa_status);
    if (user_consent_needed)
    {
      return lpa_state_change(LPA_STATE_AUTHENTICATE_CLIENT_RESP_RCVD, LPA_STATE_WAITING_FOR_USER_CONSENT, slot_id);
    }
  }

  /* We received the HTTP response for AuthenticateClient which contains:
     transactionID
     dpSigned2
     smdpSignature2
     smdpCertificate.
     Call asn1 encoder to encode the PrepareDownload request... this function
     mallocs pointer inside prepare_download_data, so make sure to free it
     after sending STORE DATA to the card */
  prepare_download_raw.dp_signed_raw = authenticate_client_resp_ptr->smdp_signed2;
  prepare_download_raw.smdp_signature_raw = authenticate_client_resp_ptr->smdp_signature2;
  prepare_download_raw.hashed_confirmation_code =
    lpa_lpd_global_info[slot_id].hashed_confirmation_code_tx_id;
  prepare_download_raw.smdp_certificate_raw = authenticate_client_resp_ptr->smdp_certificate;

  /* Following encoder may malloc prepare_download_encoded - so make sure
     to free it before exiting the function */
  lpa_status = lpa_asn1_encode_prepare_download_request(
                 prepare_download_raw, &prepare_download_encoded, slot_id);

  /* Write the data received from DS to EFS file for testing purposes */
  (void) lpa_efs_write(LPA_FILE_ASN1_ENCODED_PREP_DOWNLOAD,
                       prepare_download_encoded.data_len,
                       prepare_download_encoded.data_ptr,
                       slot_id);

  /* Send the prepare download STORE DATA to the card */
  if(lpa_status == LPA_SUCCESS &&
     prepare_download_encoded.data_len > 0 &&
     prepare_download_encoded.data_ptr)
  {
    lpa_status = lpa_mmgsdi_store_data(slot_id,
                                       LPA_PREPARE_DOWNLOAD_APDU_DATA_P1,
                                       LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                       prepare_download_encoded,
                                       TRUE,
                                       lpa_lpd_mmgsdi_cb,
                                       (void*) LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD);
  }
  else
  {
    LPA_MSG_HIGH_1("Error in lpa_asn1_encode_prepare_download_request, status=0x%x",
                   lpa_status);
  }

  LPA_FREE(prepare_download_encoded.data_ptr);

  return lpa_status;
} /* lpa_lpd_process_authenticate_client_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_CANCEL_SESSION_SERVER_RESP

DESCRIPTION
  Function processes the http response from SMxx for Cancel Session.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_cancel_session_server_resp (
  lpa_slot_id_enum_type                           slot_id
)
{
  lpa_result_enum_type         lpa_status = LPA_SUCCESS;

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  do
  {
    lpa_result_enum_type       cancel_reason_status = LPA_GENERIC_ERROR;

    lpa_status = lpa_state_change(LPA_STATE_CANCEL_SESSION_SERVER_SENT, LPA_STATE_CANCEL_SESSION_SERVER_RESP_RCVD, slot_id);
    if(lpa_status != LPA_SUCCESS)
    {
      break;
    }

    /* Call the LPA client event callback one last time to indicate that the
       installation is cancelled */
    if(lpa_lpd_global_info[slot_id].cancel_session_reason ==
         LPA_LPD_CANCEL_SESSION_END_USER_REJECTION)
    {
      cancel_reason_status = LPA_ERROR_PROFILE_INSTALLATION_END_USER_REJECTION;
    }
    else if(lpa_lpd_global_info[slot_id].cancel_session_reason ==
              LPA_LPD_CANCEL_SESSION_PPR_ID_NOT_ALLOWED)
    {
      cancel_reason_status = LPA_ERROR_PROFILE_INSTALLATION_PPR_ID_NOT_ALLOWED;
    }

    lpa_lpd_send_profile_download_event(cancel_reason_status,
                                        slot_id,
                                        0x00,  /* 100% */
                                        FALSE,
                                        TRUE);
  }
  while (0);

  return lpa_status;
} /* lpa_lpd_process_cancel_session_server_resp */


/*===========================================================================
FUNCTION LPA_LPD_PROCESS_CANCEL_SESSION_EUICC_RESP

DESCRIPTION
  Function processes the CANCEL SESSION response from the eUICC.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_lpd_process_cancel_session_euicc_resp (
  lpa_lpd_cancel_session_euicc_resp_type    *cancel_session_euicc_resp_ptr,
  lpa_slot_id_enum_type                      slot_id
)
{
  lpa_result_enum_type                     lpa_status = LPA_SUCCESS;
  lpa_http_cancel_session_server_req_type  cancel_session_server_req;

  memset(&cancel_session_server_req, 0x00, sizeof(cancel_session_server_req));

  do
  {
    if(cancel_session_euicc_resp_ptr == NULL ||
       slot_id >= LPA_NUM_SLOTS)
    {
      lpa_status = LPA_GENERIC_ERROR;
      break;
    }

    /* Write the data received from card to EFS file for testing purposes */
    (void) lpa_efs_write(LPA_FILE_CANCEL_SESSION_EUICC_FROM_CARD,
                         cancel_session_euicc_resp_ptr->data.data_len,
                         cancel_session_euicc_resp_ptr->data.data_ptr,
                         slot_id);

    lpa_status = lpa_state_change(LPA_STATE_USER_CONSENT_RCVD,
                                  LPA_STATE_CANCEL_SESSION_SERVER_SENT,
                                  slot_id);
    if(lpa_status != LPA_SUCCESS)
    {
      lpa_status = lpa_state_change(LPA_STATE_AUTHENTICATE_CLIENT_RESP_RCVD,
                                    LPA_STATE_CANCEL_SESSION_SERVER_SENT,
                                    slot_id);
      if(lpa_status != LPA_SUCCESS)
      {
        break;
      }
    }

    if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_DEFAULT_SMDP)
    {
      /* Add Profile initiated using default SMDP address */
      cancel_session_server_req.smdp_address = lpa_lpd_global_info[slot_id].default_smdp_address;
    }
    else if(lpa_lpd_global_info[slot_id].method == LPA_ADD_PROFILE_METHOD_SMDS)
    {
      /* Add Profile initiated using root SMDS address */
      cancel_session_server_req.smdp_address = lpa_lpd_global_info[slot_id].root_smds_address;
    }
    else
    {
      /* Add Profile initiated using SMDS address in activation code */
      cancel_session_server_req.smdp_address = lpa_lpd_global_info[slot_id].activation_code.smdp_address;
    }
    cancel_session_server_req.transaction_id = lpa_lpd_global_info[slot_id].transaction_id_string;
    cancel_session_server_req.cancel_session_euicc_resp = cancel_session_euicc_resp_ptr->data;

    cancel_session_server_req.slot_id = slot_id;
    cancel_session_server_req.lpd_callback = lpa_lpd_http_cb;
    cancel_session_server_req.user_data_ptr = (void*) LPA_LPD_CANCEL_SESSION_SERVER_RESP_CMD;
    lpa_status = lpa_http_build_and_send_cancel_session(
                   &cancel_session_server_req);
  }
  while (0);

  return lpa_status;
} /* lpa_lpd_process_cancel_session_euicc_resp */


/*===========================================================================
FUNCTION LPA_LPD_HANDLE_CMD_SIG

DESCRIPTION
  This function, called in lpa_main, handles the processing of all the
  sub commands for achieving a profile download.

DEPENDENCIES
  LPA task must be finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_lpd_handle_cmd_sig (
  void
)
{
  lpa_lpd_response_data_type       *task_cmd_ptr  = NULL;
  lpa_result_enum_type              lpa_status    = LPA_GENERIC_ERROR;
  lpa_state_enum_type               lpa_state     = LPA_STATE_IDLE;
  lpa_euicc_phase_enum_type         euicc_phase   = LPA_PHASE1_1_EUICC;

  task_cmd_ptr = (lpa_lpd_response_data_type*)(q_get(&lpa_lpd_cmd_q));

  (void) rex_clr_sigs(rex_self(), LPA_LPD_CMD_SIG);

  if(task_cmd_ptr == NULL)
  {
    return;
  }

  LPA_MSG_HIGH_3("LPA_LPD_CMD_SIG received for lpd command: 0x%x on slot_id: 0x%x, status=0x%x",
                 task_cmd_ptr->cmd_data.cmd, task_cmd_ptr->slot_id, task_cmd_ptr->status);
  lpa_status = task_cmd_ptr->status;

  (void) lpa_retrieve_euicc_phase(task_cmd_ptr->slot_id, &euicc_phase);
  lpa_state = lpa_state_get_current(task_cmd_ptr->slot_id);

  if (lpa_status == LPA_SUCCESS ||
      task_cmd_ptr->cmd_data.cmd == LPA_LPD_GET_RAT_RESP_CMD) /* RAT support is optional for eUICC */
  {
    switch(lpa_state)
    {
      case LPA_STATE_INIT:
        /* Trigger Initiate Auth */
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_GET_EUICC_CHALLENGE_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_get_euicc_challenge_resp(
                         &task_cmd_ptr->cmd_data.data.get_euicc_challenge_resp,
                         task_cmd_ptr->slot_id);
        }
        else if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_GET_EUICC_INFO_RESP_CMD)
        {
          if(euicc_phase == LPA_PHASE1_EUICC ||
             euicc_phase == LPA_PHASE1_1_EUICC)
          {
            lpa_status = lpa_lpd_process_get_euicc_info_legacy_resp(
                         &task_cmd_ptr->cmd_data.data.get_euicc_info_resp,
                         task_cmd_ptr->slot_id);
          }
          else
          {
            lpa_status = lpa_lpd_process_get_euicc_info_resp(
                           &task_cmd_ptr->cmd_data.data.get_euicc_info_resp,
                           task_cmd_ptr->slot_id);
          }
        }
        else if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_GET_RAT_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_get_rat_resp(
                           &task_cmd_ptr->cmd_data.data.get_rat_resp,
                           task_cmd_ptr->slot_id);
        }
        else if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_GET_PROFILES_INFO_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_get_profiles_info_resp(
                           &task_cmd_ptr->cmd_data.data.get_profiles_info_resp,
                           task_cmd_ptr->slot_id);
        }
        else if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_GET_EUICC_CONFIG_ADDRESS_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_get_euicc_config_address_resp(
                           &task_cmd_ptr->cmd_data.data.get_euicc_config_address_resp,
                           task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

       case LPA_STATE_INITIATE_AUTH_SENT:
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_INITIATE_AUTH_RESP_CMD)
        {
          if(euicc_phase == LPA_PHASE1_EUICC ||
             euicc_phase == LPA_PHASE1_1_EUICC)
          {
            lpa_status = lpa_lpd_process_initiate_auth_legacy_resp(
                           &task_cmd_ptr->cmd_data.data.initiate_auth_resp,
                           task_cmd_ptr->slot_id);
          }
          else
          {
            lpa_status = lpa_lpd_process_initiate_auth_resp(
                           &task_cmd_ptr->cmd_data.data.initiate_auth_resp,
                           task_cmd_ptr->slot_id);
          }
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      case LPA_STATE_INITIATE_AUTH_RESP_RCVD:
        if ((euicc_phase == LPA_PHASE1_EUICC ||
             euicc_phase == LPA_PHASE1_1_EUICC) &&
            task_cmd_ptr->cmd_data.cmd == LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_process_prepare_download_resp(
                         &task_cmd_ptr->cmd_data.data.prepare_download_resp,
                         task_cmd_ptr->slot_id);
        }
        else if (task_cmd_ptr->cmd_data.cmd == LPA_LPD_AUTHENTICATE_SERVER_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_authenticate_server_resp(
                         &task_cmd_ptr->cmd_data.data.authenticate_server_resp,
                         task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      case LPA_STATE_AUTHENTICATE_CLIENT_SENT:
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_AUTHENTICATE_CLIENT_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_authenticate_client_resp(
                         &task_cmd_ptr->cmd_data.data.authenticate_client_resp,
                         task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      case LPA_STATE_AUTHENTICATE_CLIENT_RESP_RCVD:
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_process_prepare_download_resp(
                         &task_cmd_ptr->cmd_data.data.prepare_download_resp,
                         task_cmd_ptr->slot_id);
        }
        else if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_CANCEL_SESSION_EUICC_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_cancel_session_euicc_resp(
                         &task_cmd_ptr->cmd_data.data.cancel_session_euicc_resp,
                         task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      case LPA_STATE_WAITING_FOR_USER_CONSENT:
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_USER_CONSENT_CMD)
        {
          lpa_status = lpa_lpd_process_process_user_consent(
                         &task_cmd_ptr->cmd_data.data.user_consent,
                         task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      case LPA_STATE_USER_CONSENT_RCVD:
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_PREPARE_DOWNLOAD_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_process_prepare_download_resp(
                         &task_cmd_ptr->cmd_data.data.prepare_download_resp,
                         task_cmd_ptr->slot_id);
        }
        else if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_CANCEL_SESSION_EUICC_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_cancel_session_euicc_resp(
                         &task_cmd_ptr->cmd_data.data.cancel_session_euicc_resp,
                         task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      case LPA_STATE_CANCEL_SESSION_SERVER_SENT:
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_CANCEL_SESSION_SERVER_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_cancel_session_server_resp(
                         task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      case LPA_STATE_GET_BPP_SENT:
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_GET_BPP_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_get_bpp_resp(
                         &task_cmd_ptr->cmd_data.data.get_bpp_resp,
                         task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      case LPA_STATE_GET_BPP_RESP_RCVD:
        if(task_cmd_ptr->cmd_data.cmd == LPA_LPD_LOAD_BPP_RESP_CMD)
        {
          lpa_status = lpa_lpd_process_load_bpp_resp(
                         &task_cmd_ptr->cmd_data.data.load_bpp_resp,
                         task_cmd_ptr->slot_id);
        }
        else
        {
          lpa_status = LPA_GENERIC_ERROR;
        }
        break;

      default:
        lpa_status = LPA_GENERIC_ERROR;
        break;
    }
  }

  /* If the status is not successful, we must send the event back to the LPA
     client indicating that the add profile request is complete and has
     failed */
  if(lpa_status != LPA_SUCCESS)
  {
    LPA_MSG_HIGH_1("Error lpa_lpd_handle_cmd_sig, status=0x%x",
                   lpa_status);

    if(lpa_status == LPA_ERROR_LOAD_BPP_EXECUTION_ERROR &&
       lpa_state == LPA_STATE_GET_BPP_RESP_RCVD &&
       task_cmd_ptr->cmd_data.cmd == LPA_LPD_LOAD_BPP_RESP_CMD &&
       euicc_phase == LPA_PHASE2_1_EUICC)
    {
      /* Cancel the ongoing profile download */
      LPA_MSG_HIGH_1("Load BPP execution error. Cancelling the profile download, status=0x%x", lpa_status);

      lpa_lpd_global_info[task_cmd_ptr->slot_id].cancel_session_reason =
        LPA_LPD_CANCEL_SESSION_LOAD_BPP_EXECUTION_ERROR;

      (void)lpa_lpd_start_cancel_session_procedure(
              task_cmd_ptr->slot_id,
              LPA_LPD_CANCEL_SESSION_LOAD_BPP_EXECUTION_ERROR);
    }
    else
    {
      /* Send the event to LPA client and clear the timer */
      lpa_lpd_send_profile_download_event(lpa_status,
                                          task_cmd_ptr->slot_id,
                                          0x00,
                                          FALSE,
                                          TRUE);
    }
  }

  if(lpa_state_get_current(task_cmd_ptr->slot_id) == LPA_STATE_IDLE)
  {
    lpa_reset(task_cmd_ptr->slot_id);

    /* Set the signal if there are one or more commands still in queue
       and LPA is currently in IDLE state. */
    lpa_cmd_set_sig();
  }

  lpa_lpd_free_response_data(&task_cmd_ptr->cmd_data);
  LPA_FREE(task_cmd_ptr);

  /* Set the signal if there are one or more commands to look at */
  if(q_cnt(&lpa_lpd_cmd_q) > 0)
  {
    /*  Items on the queue. Set the signal */
    (void) rex_set_sigs(rex_self(), LPA_LPD_CMD_SIG);
  }
} /* lpa_lpd_handle_cmd_sig */
