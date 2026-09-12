/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            M M G S D I   C R S M   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the processing of all Restricted SIM Access
  related functions

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2014 - 2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_crsm.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/04/16   vdc     Pass APDU parameters by pointer for restricted sim access
04/26/16   ar      Removing the mmgsdi_cnf_map_req_to_cnf_enum_type()
08/31/15   yt      Remove length information from requests and confirmations
06/26/15   av      Make channel app check more robust
04/29/15   bcho    Support for App specific cache
03/18/15   yt      Fix for 1X session access
03/13/15   yt      Fix handling of SELECT by path from current DF
03/13/15   yt      Fix handling of SELECT by File ID
03/10/15   yt      Use correct length to compare AIDs for SELECT by DF name
03/10/15   yt      Return error response when status words are not present
12/29/14   yt      Resolve compiler warnings
12/10/14   yt      Conversion of APDU streaming on channel 0 to MMGSDI requests
11/14/14   hh      Fix compilation warnings
07/25/14   yt      Initial version
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_msg.h"
#include "intconv.h"
#include "mmgsdi_crsm.h"
#include "mmgsdi.h"
#include "mmgsdiutil.h"
#include "mmgsdi_file.h"
#include "mmgsdi_gen.h"
#include "mmgsdi_gen_rsp.h"


/*===========================================================================
                       G L O B A L S
============================================================================*/
static mmgsdi_path_type mmgsdi_crsm_default_channel_curr_file[MMGSDI_MAX_NUM_SLOTS];

#define MMGSDI_CRSM_P1_SELECT_BY_FILE_ID              0x00
#define MMGSDI_CRSM_P1_SELECT_BY_DF_NAME              0x04
#define MMGSDI_CRSM_P1_SELECT_BY_PATH_FROM_MF         0x08
#define MMGSDI_CRSM_P1_SELECT_BY_PATH_FROM_CURR_DF    0x09
#define MMGSDI_CRSM_P2_SELECT_RETURN_FCP              0x04
#define MMGSDI_CRSM_P2_SELECT_RETURN_NO_DATA          0x0C
#define MMGSDI_CRSM_P1_SFI_REF                        0x80
#define MMGSDI_CRSM_P1_SFI_REF_MASK                   0xE0
#define MMGSDI_CRSM_P1_SFI_MASK                       0x1F
#define MMGSDI_CRSM_P2_SFI_REF                        0xF8
#define MMGSDI_CRSM_P2_ACCESS_MODE_MASK               0x07
#define MMGSDI_CRSM_P2_AUTH_CONTEXT_MASK              0x87
#define MMGSDI_CRSM_P2_AUTH_CONTEXT_GSM               0x80
#define MMGSDI_CRSM_P2_AUTH_CONTEXT_3G                0x81
#define MMGSDI_CRSM_RUN_GSM_ALGO_DATA_LEN             0x10

/*===========================================================================
                       F U N C T I O N S
============================================================================*/

/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_POPULATE_CNF_STATUS_WORDS

  DESCRIPTION:
    This function populates the status words for client cnf.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    None

  SIDE EFFECTS:
    None
=============================================================================*/
static void mmgsdi_crsm_populate_cnf_status_words
(
  mmgsdi_return_enum_type    mmgsdi_status,
  mmgsdi_sw_type             mmgsdi_sw,
  mmgsdi_sw_type           * client_sw_ptr
)
{
  if (client_sw_ptr == NULL)
  {
    return;
  }

  /* If status words are not returned by MMGSDI, populate them for the following
     values of mmgsdi_status */
  if (mmgsdi_status == MMGSDI_SUCCESS && !mmgsdi_sw.present)
  {
    client_sw_ptr->present = TRUE;
    client_sw_ptr->sw1     = GSDI_SIM_SW1_NORM_END;
    client_sw_ptr->sw2     = GSDI_SIM_SW2_NORM_END;
  }
  else if ((mmgsdi_status == MMGSDI_NOT_FOUND || mmgsdi_status == MMGSDI_NOT_SUPPORTED) &&
           !mmgsdi_sw.present)
  {
    client_sw_ptr->present = TRUE;
    client_sw_ptr->sw1     = GSDI_SW1_INCORRECT_PARAMETERS;
    client_sw_ptr->sw2     = GSDI_SW2_FILE_NOT_FOUND;
  }
  else if (mmgsdi_status == MMGSDI_ACCESS_DENIED && !mmgsdi_sw.present)
  {
    client_sw_ptr->present = TRUE;
    client_sw_ptr->sw1     = GSDI_SW1_COMMAND_NOT_ALLOWED;
    client_sw_ptr->sw2     = GSDI_SW2_SECURITY_STATUS_NOT_SATISFIED;
  }
  else if (mmgsdi_status == MMGSDI_EF_INCONSISTENT && !mmgsdi_sw.present)
  {
    client_sw_ptr->present = TRUE;
    client_sw_ptr->sw1     = GSDI_SW1_REFERENCE_MANAGEMENT;
    client_sw_ptr->sw2     = GSDI_SW2_FILE_INCONSISTENT_WITH_CMD;
  }
  else if (mmgsdi_status == MMGSDI_INCORRECT_LENGTH && !mmgsdi_sw.present)
  {
    client_sw_ptr->present = TRUE;
    client_sw_ptr->sw1     = GSDI_SW1_WRONG_LENGTH;
    client_sw_ptr->sw2     = GSDI_SW2_NO_INFO_GIVEN;
  }
  else if (mmgsdi_status == MMGSDI_INCORRECT_PARAMS && !mmgsdi_sw.present)
  {
    client_sw_ptr->present = TRUE;
    client_sw_ptr->sw1     = GSDI_SW1_INCORRECT_P1_OR_P2;
    client_sw_ptr->sw2     = GSDI_SW2_NO_INFO_GIVEN;
  }
  else
  {
    client_sw_ptr->present = mmgsdi_sw.present;
    client_sw_ptr->sw1     = mmgsdi_sw.sw1;
    client_sw_ptr->sw2     = mmgsdi_sw.sw2;
  }
} /* mmgsdi_crsm_populate_cnf_status_words */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_RESTRICTED_SIM_ACCESS_CB

  DESCRIPTION:
    Callback function for restricted SIM access requests. This function
    populates the restricted_sim_access_cnf and calls the client callback.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    None

  SIDE EFFECTS:
    None
=============================================================================*/
static void mmgsdi_crsm_restricted_sim_access_cb
(
  mmgsdi_return_enum_type    mmgsdi_status,
  mmgsdi_cnf_enum_type       cnf,
  const mmgsdi_cnf_type    * cnf_ptr
)
{
  mmgsdi_cnf_type                        * client_cnf_ptr       = NULL;
  uim_instrns_enum_type                    command              = 0;
  mmgsdi_slot_id_enum_type                 slot_id              = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_client_id_type                    client_id            = 0;
  mmgsdi_client_data_type                  client_ref           = 0;
  mmgsdi_len_type                          requested_len        = 0;
  mmgsdi_callback_type                     response_cb          = NULL;
  mmgsdi_restricted_sim_access_data_type * crsm_client_data_ptr = NULL;

  if (cnf_ptr == NULL)
  {
    return;
  }

  crsm_client_data_ptr =
    (mmgsdi_restricted_sim_access_data_type *)cnf_ptr->response_header.client_data;
  if (crsm_client_data_ptr == NULL)
  {
    UIM_MSG_ERR_0("NULL client data for restricted SIM access");
    return;
  }

  command     = (uim_instrns_enum_type)crsm_client_data_ptr->command;
  slot_id     = crsm_client_data_ptr->slot_id;
  client_id   = crsm_client_data_ptr->client_id;
  client_ref  = crsm_client_data_ptr->client_ref;
  response_cb = crsm_client_data_ptr->response_cb;

  if(crsm_client_data_ptr->apdu_params.params_valid)
  {
    requested_len = (mmgsdi_len_type)crsm_client_data_ptr->apdu_params.p3;
  }

  MMGSDIUTIL_TMC_MEM_FREE(crsm_client_data_ptr);

  if(response_cb == NULL)
  {
    return;
  }

  if (command != READ_BINARY && command != UPDATE_BINARY &&
      command != READ_RECORD && command != UPDATE_RECORD &&
      command != GET_RESPONSE && command != STATUS)
  {
    UIM_MSG_ERR_1("Invalid command 0x%x for restricted SIM access", command);
    return;
  }

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(client_cnf_ptr,sizeof(mmgsdi_cnf_type));
  if (client_cnf_ptr == NULL)
  {
    return;
  }

  /* Populate the response header for cnf to be returned to client */
  client_cnf_ptr->response_header.client_id     = client_id;
  client_cnf_ptr->response_header.client_data   = client_ref;
  client_cnf_ptr->response_header.mmgsdi_status = mmgsdi_status;
  client_cnf_ptr->response_header.slot_id       = slot_id;
  client_cnf_ptr->response_header.response_type = MMGSDI_RESTRICTED_SIM_ACCESS_REQ;

  mmgsdi_crsm_populate_cnf_status_words(mmgsdi_status,
                                        cnf_ptr->response_header.status_word,
                                        &client_cnf_ptr->response_header.status_word);

  switch (cnf)
  {
    case MMGSDI_READ_REQ:
      if (cnf_ptr->read_cnf.read_data.data_len > 0)
      {
        if (cnf_ptr->read_cnf.read_data.data_len < requested_len)
        {
          client_cnf_ptr->response_header.status_word.present = TRUE;
          client_cnf_ptr->response_header.status_word.sw1     = GSDI_SW1_WRONG_LENGTH;
          client_cnf_ptr->response_header.status_word.sw2     = GSDI_SW2_NO_INFO_GIVEN;
        }
        else
        {
          client_cnf_ptr->restricted_sim_access_cnf.response_data.data_len =
            cnf_ptr->read_cnf.read_data.data_len;
          client_cnf_ptr->restricted_sim_access_cnf.response_data.data_ptr =
            cnf_ptr->read_cnf.read_data.data_ptr;
        }
      }
      break;
    case MMGSDI_GET_FILE_ATTR_REQ:
      if (cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len > 0)
      {
        if (requested_len > 0 &&
            requested_len < cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len)
        {
          client_cnf_ptr->restricted_sim_access_cnf.response_data.data_len =
            requested_len;
        }
        else
        {
          client_cnf_ptr->restricted_sim_access_cnf.response_data.data_len =
            cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len;
        }
        client_cnf_ptr->restricted_sim_access_cnf.response_data.data_ptr =
          cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_ptr;
      }
      break;
    case MMGSDI_CARD_STATUS_REQ:
      if (cnf_ptr->status_cnf.status_data.data_len > 0)
      {
        if (requested_len > 0 &&
            requested_len < cnf_ptr->status_cnf.status_data.data_len)
        {
          client_cnf_ptr->restricted_sim_access_cnf.response_data.data_len =
            requested_len;
        }
        else
        {
          client_cnf_ptr->restricted_sim_access_cnf.response_data.data_len =
            cnf_ptr->status_cnf.status_data.data_len;
        }
        client_cnf_ptr->restricted_sim_access_cnf.response_data.data_ptr =
          cnf_ptr->status_cnf.status_data.data_ptr;
      }
      break;
    case MMGSDI_WRITE_REQ:
      /* No response data to copy for WRITE commands */
    default:
      break;
  }

  response_cb(mmgsdi_status,
              MMGSDI_RESTRICTED_SIM_ACCESS_REQ,
              client_cnf_ptr);

  MMGSDIUTIL_TMC_MEM_FREE(client_cnf_ptr);
} /* mmgsdi_crsm_restricted_sim_access_cb */


/*===========================================================================
FUNCTION MMGSDI_CRSM_CACHE_GET_RESPONSE

DESCRIPTION
  Function will cache the APDU Response Data in the GET RESPONSE table.

DEPENDENCIES
  The APDU_PTR provided must contain a valid pointer and all memory must
  be appropriately allocated to match the data maintained in this table.

RETURN VALUE
  mmgsdi_return_enum_type:

SIDE EFFECTS
  None.

===========================================================================*/
static mmgsdi_return_enum_type  mmgsdi_crsm_cache_get_response
(
  mmgsdi_client_id_type      client_id,
  mmgsdi_slot_id_enum_type   slot_id,
  uint8                      channel_id,
  mmgsdi_sw_type             status_words,
  const mmgsdi_data_type   * apdu_ptr
)
{
  mmgsdi_data_type           get_rsp_data                  = {0};
  uint8                      get_rsp_sw[MMGSDI_GEN_SW_LEN] = {0};
  mmgsdi_return_enum_type    mmgsdi_status                 = MMGSDI_ERROR;

  MMGSDIUTIL_RETURN_IF_NULL_2(apdu_ptr, apdu_ptr->data_ptr);
  if (apdu_ptr->data_len <= 0)
  {
    return MMGSDI_ERROR;
  }

  /* Copy status words. SW1/SW2 need to be duplicated at the end to be
     consistent with the current GET RESPONSE caching/retrieval logic in MMGSDI. */
  get_rsp_sw[0] = status_words.sw1;
  get_rsp_sw[1] = status_words.sw2;
  get_rsp_sw[2] = GSDI_SIM_SW1_PROCEDURE_BYTE_61;
  get_rsp_sw[3] = apdu_ptr->data_len;

  get_rsp_data.data_len = apdu_ptr->data_len + MMGSDI_GEN_SW_LEN;
  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(get_rsp_data.data_ptr,
                                     get_rsp_data.data_len);
  if (get_rsp_data.data_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* Populate response data followed by the status words. */
  mmgsdi_memscpy((void *)get_rsp_data.data_ptr, get_rsp_data.data_len,
                 (void *)apdu_ptr->data_ptr, apdu_ptr->data_len);
  mmgsdi_memscpy((void *)(get_rsp_data.data_ptr + apdu_ptr->data_len), MMGSDI_GEN_SW_LEN,
                 (void *)get_rsp_sw, MMGSDI_GEN_SW_LEN);

  mmgsdi_status = mmgsdi_gen_get_rsp_table_add(client_id,
                                               slot_id,
                                               MMGSDI_DEFAULT_CHANNEL,
                                               &get_rsp_data);

  MMGSDIUTIL_TMC_MEM_FREE(get_rsp_data.data_ptr);

  return mmgsdi_status;
} /* mmgsdi_crsm_cache_get_response */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_APDU_ACCESS_CB

  DESCRIPTION:
    Callback function for APDU access requests. This function
    populates the send_apdu_ext_cnf and calls the client callback.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    None

  SIDE EFFECTS:
    None
=============================================================================*/
static void mmgsdi_crsm_apdu_access_cb
(
  mmgsdi_return_enum_type    mmgsdi_status,
  mmgsdi_cnf_enum_type       cnf,
  const mmgsdi_cnf_type    * cnf_ptr
)
{
  mmgsdi_cnf_type                        * client_cnf_ptr       = NULL;
  uint8                                    slot_index           = 0;
  uim_instrns_enum_type                    command              = 0;
  mmgsdi_send_apdu_options_enum_type       apdu_option          = MMGSDI_SEND_APDU_MAX_ENUM;
  mmgsdi_slot_id_enum_type                 slot_id              = MMGSDI_MAX_SLOT_ID_ENUM;
  mmgsdi_client_id_type                    client_id            = 0;
  mmgsdi_client_data_type                  client_ref           = 0;
  mmgsdi_apdu_params_type                  apdu_params          = {0};
  mmgsdi_len_type                          requested_len        = 0;
  uint8                                    apdu_sw[2]           = {0};
  uint8                                  * data_ptr             = NULL;
  uint32                                   response_len         = 0;
  mmgsdi_callback_type                     response_cb          = NULL;
  mmgsdi_restricted_sim_access_data_type * apdu_client_data_ptr = NULL;
  uint8                                    sres_kc_data[MMGSDI_GSM_ALGO_SRES_LEN + MMGSDI_GSM_ALGO_KC_LEN] = {0};

  if (cnf_ptr == NULL)
  {
    return;
  }

  apdu_client_data_ptr =
    (mmgsdi_restricted_sim_access_data_type *)cnf_ptr->response_header.client_data;
  if (apdu_client_data_ptr == NULL)
  {
    UIM_MSG_ERR_0("NULL client data for restricted SIM access");
    return;
  }

  slot_id     = apdu_client_data_ptr->slot_id;
  client_id   = apdu_client_data_ptr->client_id;
  command     = (uim_instrns_enum_type)apdu_client_data_ptr->command;
  client_ref  = apdu_client_data_ptr->client_ref;
  response_cb = apdu_client_data_ptr->response_cb;
  apdu_option = apdu_client_data_ptr->apdu_option;

  apdu_params.params_valid = apdu_client_data_ptr->apdu_params.params_valid;
  apdu_params.p1           = apdu_client_data_ptr->apdu_params.p1;
  apdu_params.p2           = apdu_client_data_ptr->apdu_params.p2;
  requested_len            = apdu_client_data_ptr->apdu_params.p3;

  if (mmgsdi_status == MMGSDI_SUCCESS &&
      (command == SELECT ||
       (((command == READ_BINARY || command == UPDATE_BINARY) &&
         ((apdu_params.p1 & MMGSDI_CRSM_P1_SFI_REF_MASK) == MMGSDI_CRSM_P1_SFI_REF)) ||
        ((command == READ_RECORD || command == UPDATE_RECORD) &&
         (apdu_params.p2 & MMGSDI_CRSM_P2_SFI_REF)))))
  {
    /* In case of a successful SELECT or SFI access, copy the path
       that was selected into the global path for default channel */
    if (mmgsdi_util_get_slot_index(slot_id, &slot_index) != MMGSDI_SUCCESS)
    {
      MMGSDIUTIL_TMC_MEM_FREE(apdu_client_data_ptr);
      return;
    }
    memset(&mmgsdi_crsm_default_channel_curr_file[slot_index], 0, sizeof(mmgsdi_path_type));
    mmgsdi_memscpy((void *)&mmgsdi_crsm_default_channel_curr_file[slot_index],
                   sizeof(mmgsdi_path_type),
                   (void *)&apdu_client_data_ptr->file_id_and_path,
                   sizeof(mmgsdi_path_type));
  }

  MMGSDIUTIL_TMC_MEM_FREE(apdu_client_data_ptr);

  if (response_cb == NULL)
  {
    return;
  }

  if (command != READ_BINARY && command != UPDATE_BINARY &&
      command != READ_RECORD && command != UPDATE_RECORD &&
      command != SELECT && command != AUTHENTICATE)
  {
    UIM_MSG_ERR_1("Invalid command 0x%x for APDU access", command);
    return;
  }

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(client_cnf_ptr,sizeof(mmgsdi_cnf_type));
  if (client_cnf_ptr == NULL)
  {
    return;
  }

  /* Populate the response header for cnf to be returned to client */
  client_cnf_ptr->response_header.client_id     = client_id;
  client_cnf_ptr->response_header.client_data   = client_ref;
  client_cnf_ptr->response_header.slot_id       = slot_id;
  client_cnf_ptr->response_header.response_type = MMGSDI_SEND_APDU_EXT_REQ;

  mmgsdi_crsm_populate_cnf_status_words(mmgsdi_status,
                                        cnf_ptr->response_header.status_word,
                                        &client_cnf_ptr->response_header.status_word);
  apdu_sw[0] = client_cnf_ptr->response_header.status_word.sw1;
  apdu_sw[1] = client_cnf_ptr->response_header.status_word.sw2;
  response_len = sizeof(apdu_sw);

  /* If status words could not be populated in case of error, return error
     cnf to client. Else return success along with status words */
  if (client_cnf_ptr->response_header.status_word.present == FALSE &&
      mmgsdi_status != MMGSDI_SUCCESS)
  {
    client_cnf_ptr->response_header.mmgsdi_status = cnf_ptr->response_header.mmgsdi_status;
  }
  else
  {
    client_cnf_ptr->response_header.mmgsdi_status = MMGSDI_SUCCESS;
    mmgsdi_status = MMGSDI_SUCCESS;
  }

  /* Delete GET RESPONSE cache for the client if present. The cache will be
     populated below from GET_FILE_ATTR/AUTH response if needed. */
  (void)mmgsdi_gen_get_rsp_table_delete(client_id,
                                        slot_id,
                                        MMGSDI_DEFAULT_CHANNEL);

  switch (cnf)
  {
    case MMGSDI_READ_REQ:
      if (cnf_ptr->read_cnf.read_data.data_len > 0)
      {
        if (cnf_ptr->read_cnf.read_data.data_len < requested_len)
        {
          apdu_sw[0] = GSDI_SW1_WRONG_LENGTH;
          apdu_sw[1] = GSDI_SW2_NO_INFO_GIVEN;
          client_cnf_ptr->response_header.status_word.sw1 = apdu_sw[0];
          client_cnf_ptr->response_header.status_word.sw2 = apdu_sw[1];
        }
        else
        {
          response_len += cnf_ptr->read_cnf.read_data.data_len;
          data_ptr      = cnf_ptr->read_cnf.read_data.data_ptr;
        }
      }
      break;
    case MMGSDI_GET_FILE_ATTR_REQ:
      if (cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len > 0)
      {
        if (apdu_option == MMGSDI_SEND_APDU_RETURN_IMPLICIT_STATUS_WORDS &&
            apdu_params.p2 == MMGSDI_CRSM_P2_SELECT_RETURN_FCP)
        {
          /* Store the response data in GET RESPONSE cache and return
             implicit status words based on the length of the response. */
          mmgsdi_status = mmgsdi_crsm_cache_get_response(
                            client_id,
                            slot_id,
                            MMGSDI_DEFAULT_CHANNEL,
                            client_cnf_ptr->response_header.status_word,
                            (mmgsdi_data_type *)&cnf_ptr->get_file_attr_cnf.raw_file_attr_data);
          if (mmgsdi_status != MMGSDI_SUCCESS)
          {
            UIM_MSG_HIGH_0("Unable to cache get response data");
          }
          apdu_sw[0] = GSDI_SIM_SW1_PROCEDURE_BYTE_61;
          if (cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len > MMGSDI_GEN_MAX_GET_RESPONSE_REQ_LEN)
          {
            apdu_sw[1] = 0;
          }
          else
          {
            apdu_sw[1] = (uint8)cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len;
          }
          client_cnf_ptr->response_header.status_word.sw1 = apdu_sw[0];
          client_cnf_ptr->response_header.status_word.sw2 = apdu_sw[1];
        }
        else if (apdu_option == MMGSDI_SEND_APDU_NO_IMPLICIT_STATUS_WORDS &&
                 apdu_params.p2 == MMGSDI_CRSM_P2_SELECT_RETURN_FCP)
        {
          /* Return response data if P2 indicates so. Else, return only SW1/SW2 */
          if (requested_len > 0 &&
              requested_len < cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len)
          {
            response_len += requested_len;
          }
          else
          {
            response_len += cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_len;
          }
          data_ptr = cnf_ptr->get_file_attr_cnf.raw_file_attr_data.data_ptr;
        }
      }
      break;
    case MMGSDI_WRITE_REQ:
      break;
    case MMGSDI_USIM_AUTH_REQ:
      if (cnf_ptr->usim_auth_cnf.auth_response.data_len > 0)
      {
        if (apdu_option == MMGSDI_SEND_APDU_RETURN_IMPLICIT_STATUS_WORDS)
        {
          /* Store the response data in GET RESPONSE cache and return
             implicit status words based on the length of the response. */
          mmgsdi_status = mmgsdi_crsm_cache_get_response(
                            client_id,
                            slot_id,
                            MMGSDI_DEFAULT_CHANNEL,
                            client_cnf_ptr->response_header.status_word,
                            (mmgsdi_data_type *)&cnf_ptr->usim_auth_cnf.auth_response);
          if (mmgsdi_status != MMGSDI_SUCCESS)
          {
            UIM_MSG_HIGH_0("Unable to cache get response data");
          }
          apdu_sw[0] = GSDI_SIM_SW1_PROCEDURE_BYTE_61;
          if (cnf_ptr->usim_auth_cnf.auth_response.data_len > MMGSDI_GEN_MAX_GET_RESPONSE_REQ_LEN)
          {
            apdu_sw[1] = 0;
          }
          else
          {
            apdu_sw[1] = (uint8)cnf_ptr->usim_auth_cnf.auth_response.data_len;
          }
          client_cnf_ptr->response_header.status_word.sw1 = apdu_sw[0];
          client_cnf_ptr->response_header.status_word.sw2 = apdu_sw[1];
        }
        else
        {
          if (requested_len > 0 &&
              requested_len < cnf_ptr->usim_auth_cnf.auth_response.data_len)
          {
            response_len += requested_len;
          }
          else
          {
            response_len += cnf_ptr->usim_auth_cnf.auth_response.data_len;
          }
          data_ptr = cnf_ptr->usim_auth_cnf.auth_response.data_ptr;
        }
      }
      break;
    case MMGSDI_SESSION_RUN_GSM_ALGO_REQ:
      {
        mmgsdi_memscpy(sres_kc_data,
                       sizeof(sres_kc_data),
                       cnf_ptr->session_run_gsm_algo_cnf.sres_resp,
                       sizeof(cnf_ptr->session_run_gsm_algo_cnf.sres_resp));
        mmgsdi_memscpy(sres_kc_data + MMGSDI_GSM_ALGO_SRES_LEN,
                       sizeof(sres_kc_data) - MMGSDI_GSM_ALGO_SRES_LEN,
                       cnf_ptr->session_run_gsm_algo_cnf.kc_resp,
                       sizeof(cnf_ptr->session_run_gsm_algo_cnf.kc_resp));

        if (apdu_option == MMGSDI_SEND_APDU_RETURN_IMPLICIT_STATUS_WORDS)
        {
          mmgsdi_data_type run_gsm_rsp_data = {0};

          run_gsm_rsp_data.data_len = MMGSDI_GSM_ALGO_SRES_LEN + MMGSDI_GSM_ALGO_KC_LEN;
          run_gsm_rsp_data.data_ptr = sres_kc_data;

          /* Store the response data in GET RESPONSE cache and return
             implicit status words based on the length of the response. */
          mmgsdi_status = mmgsdi_crsm_cache_get_response(
                            client_id,
                            slot_id,
                            MMGSDI_DEFAULT_CHANNEL,
                            client_cnf_ptr->response_header.status_word,
                            (mmgsdi_data_type *)&run_gsm_rsp_data);
          if (mmgsdi_status != MMGSDI_SUCCESS)
          {
            UIM_MSG_HIGH_0("Unable to cache get response data");
          }
          apdu_sw[0] = GSDI_SIM_SW1_NORM_END_W_SW2_LENGTH;
          apdu_sw[1] = MMGSDI_GSM_ALGO_SRES_LEN + MMGSDI_GSM_ALGO_KC_LEN;

          client_cnf_ptr->response_header.status_word.sw1 = apdu_sw[0];
          client_cnf_ptr->response_header.status_word.sw2 = apdu_sw[1];
        }
        else
        {
          response_len += sizeof(sres_kc_data);
          data_ptr      = sres_kc_data;
        }
      }
      break;
    default:
      break;
  }

  if (data_ptr == NULL)
  {
    /* Return only status words if response data is not available */
    client_cnf_ptr->send_apdu_ext_cnf.apdu_data.data_ptr  = apdu_sw;
  }
  else
  {
    /* Return response data and status words in the same payload */
    MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(client_cnf_ptr->send_apdu_ext_cnf.apdu_data.data_ptr,
                                       response_len);
    if (client_cnf_ptr->send_apdu_ext_cnf.apdu_data.data_ptr == NULL)
    {
      MMGSDIUTIL_TMC_MEM_FREE(client_cnf_ptr);
      return;
    }
    mmgsdi_memscpy((void *)client_cnf_ptr->send_apdu_ext_cnf.apdu_data.data_ptr,
                   response_len,
                   (void *)data_ptr,
                   response_len - sizeof(apdu_sw));
    mmgsdi_memscpy((void *)(client_cnf_ptr->send_apdu_ext_cnf.apdu_data.data_ptr + response_len - sizeof(apdu_sw)),
                   sizeof(apdu_sw),
                   (void *)apdu_sw,
                   sizeof(apdu_sw));
  }
  client_cnf_ptr->send_apdu_ext_cnf.apdu_data.data_len  = response_len;

  response_cb(mmgsdi_status,
              MMGSDI_SEND_APDU_EXT_REQ,
              client_cnf_ptr);

  if (response_len > sizeof(apdu_sw))
  {
    /* Free APDU data only if it was allocated meaning data in addition
       to SW1/SW2 is being returned */
    MMGSDIUTIL_TMC_MEM_FREE(client_cnf_ptr->send_apdu_ext_cnf.apdu_data.data_ptr);
  }
  MMGSDIUTIL_TMC_MEM_FREE(client_cnf_ptr);
} /* mmgsdi_crsm_apdu_access_cb */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_COPY_RESTRICTED_SIM_ACCESS_CLIENT_DATA

  DESCRIPTION:
    Allocate heap memory and store the client data passed via
    mmgsdi_restricted_sim_access() API.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    None

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_crsm_copy_restricted_sim_access_client_data
(
  mmgsdi_client_id_type                     client_id,
  mmgsdi_slot_id_enum_type                  slot_id,
  uim_instrns_enum_type                     command,
  mmgsdi_apdu_params_type                   apdu_params,
  mmgsdi_send_apdu_options_enum_type        apdu_option,
  mmgsdi_path_type                          file_id_and_path,
  mmgsdi_callback_type                      response_cb_ptr,
  mmgsdi_client_data_type                   client_ref,
  mmgsdi_restricted_sim_access_data_type ** crsm_client_data_pptr
)
{
  MMGSDIUTIL_RETURN_IF_NULL(crsm_client_data_pptr);

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(*crsm_client_data_pptr,
                                     sizeof(mmgsdi_restricted_sim_access_data_type));
  if(*crsm_client_data_pptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  (*crsm_client_data_pptr)->client_id   = client_id;
  (*crsm_client_data_pptr)->slot_id     = slot_id;
  (*crsm_client_data_pptr)->command     = (uint8)command;
  (*crsm_client_data_pptr)->response_cb = response_cb_ptr;
  (*crsm_client_data_pptr)->client_ref  = client_ref;

  (*crsm_client_data_pptr)->apdu_params.params_valid = apdu_params.params_valid;
  (*crsm_client_data_pptr)->apdu_params.p1           = apdu_params.p1;
  (*crsm_client_data_pptr)->apdu_params.p2           = apdu_params.p2;
  (*crsm_client_data_pptr)->apdu_params.p3           = apdu_params.p3;
  (*crsm_client_data_pptr)->apdu_option              = apdu_option;

  mmgsdi_memscpy((void *)&(*crsm_client_data_pptr)->file_id_and_path,
                 sizeof(mmgsdi_path_type),
                 &file_id_and_path,
                 sizeof(mmgsdi_path_type));
  return MMGSDI_SUCCESS;
} /* mmgsdi_crsm_copy_restricted_sim_access_client_data */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_FIND_SESSION_FOR_ACCESS

  DESCRIPTION:
    This function finds the MMGSDI session that can be used for sending the
    command based on slot id and file path.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_crsm_find_session_for_access
(
  mmgsdi_slot_id_enum_type                 card_slot,
  mmgsdi_path_type                         file_id_and_path,
  uim_instrns_enum_type                    command,
  mmgsdi_session_id_type                 * session_id_ptr,
  mmgsdi_app_enum_type                   * app_type_ptr,
  boolean                                  ch_zero_session
)
{
  boolean ef_under_mf = FALSE;
  uint8   slot_index  = 0;

  MMGSDIUTIL_RETURN_IF_NULL_2(session_id_ptr, app_type_ptr);

  if ((mmgsdi_util_get_slot_index(card_slot, &slot_index) != MMGSDI_SUCCESS) ||
      slot_index >= MMGSDI_MAX_NUM_SLOTS)
  {
    UIM_MSG_ERR_1("Invalid slot id 0x%x", card_slot);
    return MMGSDI_INCORRECT_PARAMS;
  }

  /* If only file id is present in the path buffer, check if it belongs to
     an EF directly under MF. In this case we need to use the card session. */
  if (file_id_and_path.path_len == 1 &&
      ((file_id_and_path.path_buf[0] & 0xFF00) == 0x2F00 ||
       file_id_and_path.path_buf[0] == (uint16)MMGSDI_MF_LEVEL)&&
      command != STATUS)
  {
    ef_under_mf = TRUE;
  }

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  if (ef_under_mf ||
      (file_id_and_path.path_len > 1 &&
       (file_id_and_path.path_buf[1] != MMGSDI_GSM_LEVEL &&
        file_id_and_path.path_buf[1] != MMGSDI_RUIM_LEVEL &&
        file_id_and_path.path_buf[1] != MMGSDI_ADF_LEVEL &&
        file_id_and_path.path_buf[0] != MMGSDI_GSM_LEVEL &&
        file_id_and_path.path_buf[0] != MMGSDI_RUIM_LEVEL &&
        file_id_and_path.path_buf[0] != MMGSDI_ADF_LEVEL) &&
       command != STATUS))
  {
    /* Use card slot session for accesses outside GSM/RUIM DF or USIM/CSIM ADF. */
    *session_id_ptr = mmgsdi_generic_data_ptr->slot_session_id[slot_index];
  }
  else
  {
    if (ch_zero_session)
    {
      /* Find session active on channel zero */
      if ((file_id_and_path.path_buf[0] == MMGSDI_RUIM_LEVEL ||
           file_id_and_path.path_buf[1] == MMGSDI_RUIM_LEVEL))
      {
        /* For RUIM access, find the 1x session active on this slot since
           the default channel is shared by RUIM and SIM */
        (void)mmgsdi_util_find_prov_session_for_slot(card_slot,
                                                     session_id_ptr,
                                                     app_type_ptr,
                                                     MMGSDI_TECH_3GPP2);
      }
      else
      {
        mmgsdi_int_app_info_type * app_info_ptr = NULL;

        if ((mmgsdi_util_get_mmgsdi_session_on_ch_zero(
               card_slot,
               &app_info_ptr,
               session_id_ptr) == MMGSDI_SUCCESS) &&
             app_info_ptr != NULL)
        {
          *app_type_ptr = app_info_ptr->app_data.app_type;
        }
      }
    }
    else
    {
      /* Use GW provisioning session for access within GSM DF or USIM ADF.
         STATUS command is meant to indicate the status of a currently-selected
         application/DF and requires a provisioning session. */
      (void)mmgsdi_util_find_prov_session_for_slot(card_slot,
                                                   session_id_ptr,
                                                   app_type_ptr,
                                                   MMGSDI_TECH_3GPP);
    }
  }

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  return MMGSDI_SUCCESS;
} /* mmgsdi_crsm_find_session_for_access */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_UPDATE_PATH

  DESCRIPTION:
    This function updates the path for the file id depending on the app type.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

  SIDE EFFECTS:
    None
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_crsm_update_path
(
  mmgsdi_app_enum_type                     app_type,
  mmgsdi_path_type                       * path_ptr
)
{
  MMGSDIUTIL_RETURN_IF_NULL(path_ptr);

  if (path_ptr->path_len != 1)
  {
    return MMGSDI_SUCCESS;
  }

  if ((app_type == MMGSDI_APP_USIM ||
       app_type == MMGSDI_APP_CSIM) &&
      (path_ptr->path_buf[0] & 0xFF00) == 0x6F00)
  {
    path_ptr->path_len = 3;
    path_ptr->path_buf[2] = path_ptr->path_buf[0];
    path_ptr->path_buf[0] = (uint16)MMGSDI_MF_LEVEL;
    path_ptr->path_buf[1] = (uint16)MMGSDI_ADF_LEVEL;
  }
  else if ((app_type == MMGSDI_APP_USIM ||
            app_type == MMGSDI_APP_CSIM) &&
           path_ptr->path_buf[0] == (uint16)MMGSDI_ADF_LEVEL)
  {
    path_ptr->path_len = 2;
    path_ptr->path_buf[0] = (uint16)MMGSDI_MF_LEVEL;
    path_ptr->path_buf[1] = (uint16)MMGSDI_ADF_LEVEL;
  }
  else if (app_type == MMGSDI_APP_SIM &&
           (path_ptr->path_buf[0] & 0xFF00) == 0x6F00)
  {
    path_ptr->path_len = 3;
    path_ptr->path_buf[2] = path_ptr->path_buf[0];
    path_ptr->path_buf[0] = (uint16)MMGSDI_MF_LEVEL;
    path_ptr->path_buf[1] = (uint16)MMGSDI_GSM_LEVEL;
  }
  else if (app_type == MMGSDI_APP_SIM &&
           (path_ptr->path_buf[0] == (uint16)MMGSDI_GSM_LEVEL))
  {
    path_ptr->path_len = 2;
    path_ptr->path_buf[0] = (uint16)MMGSDI_MF_LEVEL;
    path_ptr->path_buf[1] = (uint16)MMGSDI_GSM_LEVEL;
  }
  else if (app_type == MMGSDI_APP_RUIM &&
           (path_ptr->path_buf[0] == (uint16)MMGSDI_RUIM_LEVEL))
  {
    path_ptr->path_len = 2;
    path_ptr->path_buf[0] = (uint16)MMGSDI_MF_LEVEL;
    path_ptr->path_buf[1] = (uint16)MMGSDI_RUIM_LEVEL;
  }
  else if ((path_ptr->path_buf[0] & 0xFF00) == 0x2F00)
  {
    /* For other app types assume the file to be under MF */
    path_ptr->path_len = 2;
    path_ptr->path_buf[1] = path_ptr->path_buf[0];
    path_ptr->path_buf[0] = (uint16)MMGSDI_MF_LEVEL;
  }
  else if (path_ptr->path_buf[0] == (uint16)MMGSDI_MF_LEVEL)
  {
    /* Not an error: probably SELECT command for MF */
    path_ptr->path_len = 1;
    path_ptr->path_buf[0] = (uint16)MMGSDI_MF_LEVEL;
  }
  else
  {
    return MMGSDI_ERROR;
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_crsm_update_path */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_CALCULATE_FILE_ACCESS

  DESCRIPTION:
    This function calculates the file access parameter to be sent in the
    MMGSDI command. If the path_len is 1, the file path is also updated based
    on the file id and app type.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_crsm_calculate_file_access
(
  mmgsdi_app_enum_type                     app_type,
  mmgsdi_path_type                       * file_id_and_path_ptr,
  mmgsdi_access_type                     * file_access_ptr
)
{
  mmgsdi_return_enum_type mmgsdi_status = MMGSDI_ERROR;
  mmgsdi_file_enum_type   file_enum     = MMGSDI_NO_FILE_ENUM;
  mmgsdi_path_type        path;

  MMGSDIUTIL_RETURN_IF_NULL_2(file_access_ptr, file_id_and_path_ptr);

  memset(&path, 0x00, sizeof(path));

  /* If only file id is present (path_len = 1), add path from MF depending on
     the app type */
  if (file_id_and_path_ptr->path_len == 1)
  {
    if (file_id_and_path_ptr->path_buf[0] == (uint16)MMGSDI_MF_LEVEL)
    {
      /* Return success as the access may be for MF */
      file_access_ptr->access_method = MMGSDI_BY_PATH_ACCESS;
      mmgsdi_memscpy((void *)&file_access_ptr->file.path_type, sizeof(mmgsdi_path_type),
                     (void *)file_id_and_path_ptr, sizeof(mmgsdi_path_type));
      return MMGSDI_SUCCESS;
    }
    mmgsdi_status = mmgsdi_crsm_update_path(app_type,
                                            file_id_and_path_ptr);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  mmgsdi_memscpy((void *)&path, sizeof(path), (void *)file_id_and_path_ptr, sizeof(mmgsdi_path_type));

  mmgsdi_status = mmgsdi_file_get_file_from_path(app_type,
                                                 path,
                                                 &file_enum);

  if (mmgsdi_status == MMGSDI_SUCCESS && file_enum != MMGSDI_NO_FILE_ENUM)
  {
    file_access_ptr->access_method = MMGSDI_EF_ENUM_ACCESS;
    file_access_ptr->file.file_enum = file_enum;
  }
  else if (mmgsdi_status == MMGSDI_NOT_FOUND)
  {
    /* Use BY_PATH access if file enum is not found */
    file_access_ptr->access_method = MMGSDI_BY_PATH_ACCESS;
    mmgsdi_memscpy((void *)&file_access_ptr->file.path_type, sizeof(mmgsdi_path_type),
                     (void *)&path, sizeof(mmgsdi_path_type));
  }
  else
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_crsm_calculate_file_access */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_QUEUE_RESTRICTED_SIM_ACCESS_CMD_EXT

  DESCRIPTION:
    This function translates the AT+CRSM request to an MMGSDI request and
    calls the appropriate API. An internal response callback function is
    specified from which the client callback is called.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

  SIDE EFFECTS:
    None
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_crsm_queue_restricted_sim_access_cmd_ext
(
  mmgsdi_session_id_type                   session_id,
  mmgsdi_access_type                       file_access,
  mmgsdi_app_enum_type                     app_type,
  uim_instrns_enum_type                    command,
  mmgsdi_apdu_params_type                  *apdu_params_ptr,
  mmgsdi_data_type                         data,
  mmgsdi_client_data_type                  client_data,
  mmgsdi_callback_type                     internal_cb
)
{
  mmgsdi_return_enum_type          mmgsdi_status = MMGSDI_ERROR;
  mmgsdi_file_structure_enum_type  record_type   = MMGSDI_MAX_FILE_STRUCTURE_ENUM;
  mmgsdi_rec_num_type              record_num    = 0;
  mmgsdi_len_type                  req_len       = 0;
  mmgsdi_offset_type               offset        = 0;
  mmgsdi_status_me_app_enum_type   me_app_status = MMGSDI_STATUS_APP_NO_INDICATION;
  mmgsdi_status_ret_data_enum_type ret_data      = MMGSDI_STATUS_DATA_SELECT;
  
  if(apdu_params_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  /* P1, P2, P3 parameters are mandatory for READ/UPDATE commands */
  if (apdu_params_ptr->params_valid == FALSE &&
      command != STATUS &&
      command != GET_RESPONSE)
  {
    UIM_MSG_ERR_1("Parameters P1, P2, P3 not valid for command 0x%x", command);
    return MMGSDI_INCORRECT_PARAMS;
  }

  switch (command)
  {
    case STATUS:
      if (apdu_params_ptr->params_valid)
      {
        /* Indication of application status */
        switch (apdu_params_ptr->p1)
        {
          case 0x00:
            me_app_status = MMGSDI_STATUS_APP_NO_INDICATION;
            break;
          case 0x01:
            me_app_status = MMGSDI_STATUS_APP_INITIALIZED;
            break;
          case 0x02:
            me_app_status = MMGSDI_STATUS_APP_TERMINATED;
            break;
          default:
            return MMGSDI_INCORRECT_PARAMS;
        }

        /* Data returned in response */
        switch (apdu_params_ptr->p2)
        {
          case 0x00:
            ret_data = MMGSDI_STATUS_DATA_SELECT;
            break;
          case 0x01:
            ret_data = MMGSDI_STATUS_TLV_DATA;
            break;
          case 0x0C:
            ret_data = MMGSDI_STATUS_NO_DATA;
            break;
          default:
            return MMGSDI_INCORRECT_PARAMS;
        }
      }
      mmgsdi_status = mmgsdi_session_send_card_status(session_id,
                                                      me_app_status,
                                                      ret_data,
                                                      internal_cb,
                                                      client_data);
      break;

    case READ_RECORD:
      record_num = (mmgsdi_rec_num_type)apdu_params_ptr->p1;
      if (record_num == 0)
      {
        return MMGSDI_INCORRECT_PARAMS;
      }
      req_len = (mmgsdi_len_type)apdu_params_ptr->p3;
      mmgsdi_status = mmgsdi_session_read_record(session_id,
                                                 file_access,
                                                 record_num,
                                                 req_len,
                                                 internal_cb,
                                                 client_data);
      break;

    case READ_BINARY:
      if ((apdu_params_ptr->p1 & MMGSDI_CRSM_P1_SFI_REF_MASK) == MMGSDI_CRSM_P1_SFI_REF)
      {
        offset = (mmgsdi_offset_type)apdu_params_ptr->p2;
      }
      else
      {
        offset = ((mmgsdi_offset_type)apdu_params_ptr->p1 << 8) +
                  (mmgsdi_offset_type)apdu_params_ptr->p2;
      }
      req_len = (mmgsdi_len_type)apdu_params_ptr->p3;
      mmgsdi_status = mmgsdi_session_read_transparent(session_id,
                                                      file_access,
                                                      offset,
                                                      req_len,
                                                      internal_cb,
                                                      client_data);
      break;

    case UPDATE_BINARY:
      if ((data.data_len <= 0) ||
          (data.data_ptr == NULL) ||
          (data.data_len != (mmgsdi_len_type)apdu_params_ptr->p3))
      {
        return MMGSDI_INCORRECT_PARAMS;
      }
      if ((apdu_params_ptr->p1 & MMGSDI_CRSM_P1_SFI_REF_MASK) == MMGSDI_CRSM_P1_SFI_REF)
      {
        offset = (mmgsdi_offset_type)apdu_params_ptr->p2;
      }
      else
      {
        offset = ((mmgsdi_offset_type)apdu_params_ptr->p1 << 8) +
                  (mmgsdi_offset_type)apdu_params_ptr->p2;
      }
      mmgsdi_status = mmgsdi_session_write_transparent(session_id,
                                                       file_access,
                                                       offset,
                                                       data,
                                                       internal_cb,
                                                       client_data);
      break;

    case UPDATE_RECORD:
      record_num = (mmgsdi_rec_num_type)apdu_params_ptr->p1;
      if ((data.data_len <= 0) || (data.data_ptr == NULL) ||
          (data.data_len != (mmgsdi_len_type)apdu_params_ptr->p3))
      {
        return MMGSDI_INCORRECT_PARAMS;
      }

      /* MMGSDI currently supports modes PREVIOUS (0x03) for Cyclic files and
         ABSOLUTE (0x04) for Linear Fixed files. */
      if ((apdu_params_ptr->p2 & MMGSDI_CRSM_P2_ACCESS_MODE_MASK) == 0x03)
      {
        record_type = MMGSDI_CYCLIC_FILE;
      }
      else
      {
        record_type = MMGSDI_LINEAR_FIXED_FILE;
      }

      mmgsdi_status = mmgsdi_session_write_record(session_id,
                                                  file_access,
                                                  record_type,
                                                  record_num,
                                                  data,
                                                  internal_cb,
                                                  client_data);
      break;

    case GET_RESPONSE:
    case SELECT:
      mmgsdi_status = mmgsdi_session_get_file_attr_ext(session_id,
                                                       file_access,
                                                       internal_cb,
                                                       client_data,
                                                       TRUE);
      break;

    case AUTHENTICATE:
      /* This is reached only in case of 3GPP AUTHENTICATE command */
      if (app_type == MMGSDI_APP_USIM)
      {
        mmgsdi_autn_context_enum_type autn_context = MMGSDI_AUTN_USIM_GSM_CONTEXT;

        switch (apdu_params_ptr->p2 & MMGSDI_CRSM_P2_AUTH_CONTEXT_MASK)
        {
          case MMGSDI_CRSM_P2_AUTH_CONTEXT_GSM:
            autn_context = MMGSDI_AUTN_USIM_GSM_CONTEXT;
            break;
          case MMGSDI_CRSM_P2_AUTH_CONTEXT_3G:
            autn_context = MMGSDI_AUTN_USIM_3G_CONTEXT;
            break;
          default:
            return MMGSDI_ERROR;
        }
        mmgsdi_status = mmgsdi_session_uicc_authenticate(session_id,
                                                         autn_context,
                                                         data,
                                                         internal_cb,
                                                         client_data);
      }
      else if (app_type == MMGSDI_APP_SIM &&
               apdu_params_ptr->p1 == 0 &&
               apdu_params_ptr->p2 == 0 &&
               data.data_len == MMGSDI_CRSM_RUN_GSM_ALGO_DATA_LEN)
      {
        mmgsdi_status = mmgsdi_session_run_gsm_algo(session_id,
                                                    data,
                                                    internal_cb,
                                                    client_data);
      }
      break;

    default:
      /* As per 3GPP TS 27.007, other command values are not valid for AT+CRSM.
         RETRIEVE DATA and SET DATA are not currently supported in MMGSDI. */
      mmgsdi_status = MMGSDI_INCORRECT_PARAMS;
      break;
  }

  return mmgsdi_status;
} /* mmgsdi_crsm_queue_restricted_sim_access_cmd_ext */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_QUEUE_RESTRICTED_SIM_ACCESS_CMD

  DESCRIPTION:
    This function translates the AT+CRSM request to an MMGSDI request and
    calls the appropriate API. An internal response callback function is
    specified from which the client callback is called.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_crsm_queue_restricted_sim_access_cmd
(
  mmgsdi_session_id_type                   session_id,
  mmgsdi_access_type                       file_access,
  uint8                                    command,
  mmgsdi_apdu_params_type                  *apdu_params_ptr,
  mmgsdi_data_type                         data,
  mmgsdi_restricted_sim_access_data_type * crsm_client_data_ptr
)
{
  if(apdu_params_ptr == NULL)
  {
    return MMGSDI_ERROR;
  }

  return mmgsdi_crsm_queue_restricted_sim_access_cmd_ext(session_id,
                                                         file_access,
                                                         MMGSDI_APP_NONE,
                                                         (uim_instrns_enum_type)command,
                                                         apdu_params_ptr,
                                                         data,
                                                         (mmgsdi_client_data_type)crsm_client_data_ptr,
                                                         mmgsdi_crsm_restricted_sim_access_cb);
} /* mmgsdi_crsm_queue_restricted_sim_access_cmd */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_CALCULATE_CMD_PARAMS_AND_FILE_PATH

  DESCRIPTION:
    This function calculates the command parameters and file path from the
    APDU data.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

  SIDE EFFECTS:
    None
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_crsm_calculate_cmd_params_and_file_path
(
  mmgsdi_slot_id_enum_type                 card_slot,
  uim_instrns_enum_type                    command,
  mmgsdi_send_apdu_data_type               apdu_data,
  mmgsdi_apdu_params_type                * apdu_params_ptr,
  mmgsdi_path_type                       * file_id_and_path_ptr,
  mmgsdi_data_type                       * cmd_data_ptr
)
{
  uint8                                    slot_index    = 0;
  uint8                                    i             = 0;
  mmgsdi_return_enum_type                  mmgsdi_status = MMGSDI_ERROR;

  MMGSDIUTIL_RETURN_IF_NULL_4(apdu_data.data_ptr,
                              apdu_params_ptr,
                              file_id_and_path_ptr,
                              cmd_data_ptr);

  if (mmgsdi_util_get_slot_index(card_slot, &slot_index) != MMGSDI_SUCCESS)
  {
    return MMGSDI_ERROR;
  }

  apdu_params_ptr->params_valid = TRUE;
  apdu_params_ptr->p1           = apdu_data.data_ptr[UIM_7816_APDU_P1_OFFSET];
  apdu_params_ptr->p2           = apdu_data.data_ptr[UIM_7816_APDU_P2_OFFSET];
  apdu_params_ptr->p3           = 0;

  /* Calculate P3 for various commands along with command data if applicable */
  if (apdu_data.data_len > UIM_7816_APDU_MIN_SIZE)
  {
    if (command == SELECT || command == AUTHENTICATE)
    {
      /* Le for Case 3/4 commands */
      if (apdu_data.data_len >
          (mmgsdi_len_type)(apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET] + UIM_CASE3_4_7816_LC_OFFSET + 1))
      {
        apdu_params_ptr->p3 =
          apdu_data.data_ptr[apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET] + UIM_CASE3_4_7816_LC_OFFSET + 1];
      }
      if (command == AUTHENTICATE)
      {
        /* Authentication related data */
        cmd_data_ptr->data_len = apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET];
        cmd_data_ptr->data_ptr = &apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1];
      }
    }
    else if (command == READ_RECORD || command == READ_BINARY)
    {
      /* Le for Case 2 commands */
      apdu_params_ptr->p3 = apdu_data.data_ptr[UIM_CASE2_7816_LE_OFFSET];
    }
    else if (command == UPDATE_RECORD || command == UPDATE_BINARY)
    {
      /* Data to be written to the card */
      cmd_data_ptr->data_len = apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET];
      cmd_data_ptr->data_ptr = &apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1];
      apdu_params_ptr->p3 = cmd_data_ptr->data_len;
    }
  }

  /* For SELECT, construct file ID and path according to P1 parameter. */
  if (command == SELECT)
  {
    if (apdu_params_ptr->p1 == MMGSDI_CRSM_P1_SELECT_BY_DF_NAME)
    {
      mmgsdi_int_app_info_type  * app_info_ptr = NULL;

      MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;
      mmgsdi_status = mmgsdi_util_get_channel_app(card_slot,
                                                  MMGSDI_DEFAULT_CHANNEL,
                                                  MMGSDI_APP_NONE,
                                                  &app_info_ptr);
      if (mmgsdi_status == MMGSDI_SUCCESS &&
          app_info_ptr != NULL &&
          app_info_ptr->app_data.aid.data_len >= apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET] &&
          (memcmp(app_info_ptr->app_data.aid.data_ptr,
                  &apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1],
                  apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET]) == 0))
      {
        file_id_and_path_ptr->path_len = 2;
        file_id_and_path_ptr->path_buf[0] = MMGSDI_MF_LEVEL;
        file_id_and_path_ptr->path_buf[1] = MMGSDI_ADF_LEVEL;
      }
      MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

      if (file_id_and_path_ptr->path_len == 0)
      {
        return MMGSDI_INCORRECT_PARAMS;
      }
    }
    else if (apdu_params_ptr->p1 == MMGSDI_CRSM_P1_SELECT_BY_PATH_FROM_CURR_DF)
    {
      if (mmgsdi_crsm_default_channel_curr_file[slot_index].path_len == 0 ||
          mmgsdi_crsm_default_channel_curr_file[slot_index].path_len > MMGSDI_MAX_PATH_LEN)
      {
        UIM_MSG_ERR_1("Unable to add file to current path; path_len: 0x%x",
                      mmgsdi_crsm_default_channel_curr_file[slot_index].path_len);
        return MMGSDI_ERROR;
      }
      mmgsdi_memscpy(file_id_and_path_ptr, sizeof(mmgsdi_path_type),
                     &mmgsdi_crsm_default_channel_curr_file[slot_index], sizeof(mmgsdi_path_type));
      if (file_id_and_path_ptr->path_len < MMGSDI_MAX_PATH_LEN &&
          (file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] == 0x3F00 ||
           (file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] & 0xFF00) == 0x7F00 ||
           (file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] & 0xFF00) == 0x5F00))
      {
        /* If the current selection is a DF add the new file at the end of the path */
        for (i = 1; (i < apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET]) && (file_id_and_path_ptr->path_len < MMGSDI_MAX_PATH_LEN); i += 2)
        {
          file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len] =
            apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + i] * 0x100 +
            apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + i + 1];
          file_id_and_path_ptr->path_len++;
        }
      }
      else if ((file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] & 0xFF00) == 0x2F00 ||
               (file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] & 0xFF00) == 0x6F00 ||
               (file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] & 0xFF00) == 0x4F00)
      {
        /* If the current selection is an EF, replace the file at the end of the path */
        file_id_and_path_ptr->path_len--;
        for (i = 1; (i < apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET]) && (file_id_and_path_ptr->path_len < MMGSDI_MAX_PATH_LEN); i += 2)
        {
          file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len] =
            apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + i] * 0x100 +
            apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + i + 1];
          file_id_and_path_ptr->path_len++;
        }
      }
    }
    else if (apdu_params_ptr->p1 == MMGSDI_CRSM_P1_SELECT_BY_PATH_FROM_MF)
    {
      uint8 path_index = 0;

      uint16 path_start = apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1] * 0x100 +
                          apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 2];
      if (path_start != (uint16)MMGSDI_MF_DIR)
      {
        file_id_and_path_ptr->path_buf[path_index] = (uint16)MMGSDI_MF_DIR;
        path_index++;
      }
      for (i = 1; (i < apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET]) && (path_index < MMGSDI_MAX_PATH_LEN); i += 2)
      {
        file_id_and_path_ptr->path_buf[path_index] =
          apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + i] * 0x100 +
          apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + i + 1];
        path_index++;
      }
      file_id_and_path_ptr->path_len = path_index;
    }
    else if (apdu_params_ptr->p1 == MMGSDI_CRSM_P1_SELECT_BY_FILE_ID)
    {
      uint16 file_id         = 0;
      uint16 file_id_level   = 0;
      uint16 curr_file_level = 0;
      uint16 curr_dir_level  = MMGSDI_MF_LEVEL;

      if (apdu_data.data_len >= UIM_CASE3_4_7816_LC_OFFSET + 3)
      {
        file_id = apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 1] * 0x100 +
                    apdu_data.data_ptr[UIM_CASE3_4_7816_LC_OFFSET + 2];
      }
      file_id_level   = (file_id & 0xFF00);

      if (mmgsdi_crsm_default_channel_curr_file[slot_index].path_len == 0 &&
          file_id != MMGSDI_MF_LEVEL &&
          file_id != 0)
      {
        UIM_MSG_ERR_0("No file currently selected; cannot select by File ID");
        return MMGSDI_ERROR;
      }

      if (mmgsdi_crsm_default_channel_curr_file[slot_index].path_len > 0)
      {
        mmgsdi_memscpy(file_id_and_path_ptr, sizeof(mmgsdi_path_type),
                       &mmgsdi_crsm_default_channel_curr_file[slot_index], sizeof(mmgsdi_path_type));

        curr_file_level = (file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] & 0xFF00);
      }

      if (file_id_and_path_ptr->path_len > 1)
      {
        if(curr_file_level == 0x2F00 ||
           curr_file_level == 0x6F00 ||
           curr_file_level == 0x4F00)
        {
          curr_dir_level = (file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 2] & 0xFF00);
        }
        else if (curr_file_level == 0x5F00 ||
                 curr_file_level == 0x7F00)
        {
          curr_dir_level = curr_file_level;
        }
      }

      if (file_id == 0)
      {
        /* According to ETSI 102.221, if P1 = '00' and data field is empty,
           P2 is set to '0C' and MF becomes the current directory */
        apdu_params_ptr->p2 = MMGSDI_CRSM_P2_SELECT_RETURN_NO_DATA;
        file_id_and_path_ptr->path_len = 1;
        file_id_and_path_ptr->path_buf[0] = (uint16)MMGSDI_MF_DIR;
      }
      else if (file_id == MMGSDI_MF_LEVEL)
      {
        /* MF can be selected irrespective of the current selection */
        file_id_and_path_ptr->path_len = 1;
        file_id_and_path_ptr->path_buf[0] = MMGSDI_MF_LEVEL;
      }
      else if (file_id == MMGSDI_ADF_LEVEL)
      {
        /* ADF can be selected irrespective of the current selection */
        file_id_and_path_ptr->path_len = 2;
        file_id_and_path_ptr->path_buf[0] = MMGSDI_MF_LEVEL;
        file_id_and_path_ptr->path_buf[1] = file_id;
      }
      else if ((file_id_level == 0x7F00) &&
               (curr_dir_level == MMGSDI_MF_LEVEL ||
                curr_dir_level == 0x7F00 ||
                curr_dir_level == 0x5F00))
      {
        /* SELECT 7FXX if the current DF is MF or another DF at 7FXX/5FXX level */
        file_id_and_path_ptr->path_len = 2;
        file_id_and_path_ptr->path_buf[0] = MMGSDI_MF_LEVEL;
        file_id_and_path_ptr->path_buf[1] = file_id;
      }
      else if (file_id_level == curr_file_level)
      {
        /* SELECT file which is at the same level as the current file */
        file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] =
          file_id;
      }
      else if (((file_id_level == 0x5F00 ||
                 file_id_level == 0x6F00) &&
                curr_file_level == 0x7F00) ||
               (file_id_level == 0x4F00 &&
                curr_file_level == 0x5F00) ||
               (file_id_level == 0x2F00 &&
                curr_file_level == MMGSDI_MF_LEVEL))
      {
        /* SELECT EF/DF which is directly under current DF */
        file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len++] =
          file_id;
      }
      else if (((file_id_level == 0x7F00) &&
                (curr_file_level == 0x6F00)) ||
               (((file_id_level == 0x5F00) ||
                 (file_id_level == 0x7F00)) &&
                (curr_file_level == 0x4F00)))
      {
        /* SELECT DF which is the parent of the current EF or DF */
        file_id_and_path_ptr->path_len--;
        file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1] =
          file_id;
      }
      else
      {
        UIM_MSG_ERR_1("Invalid file ID for SELECT: 0x%x", file_id);
        return MMGSDI_ERROR;
      }
    }
    else
    {
      UIM_MSG_ERR_1("Unsupported P1 for SELECT: 0x%x", apdu_params_ptr->p1);
      return MMGSDI_ERROR;
    }
  }
  else if (command == READ_RECORD || command == READ_BINARY ||
           command == UPDATE_RECORD || command == UPDATE_BINARY)
  {
    /* For READ/UPDATE commands, copy the path of the currently selected file
       from the global */
    if (mmgsdi_crsm_default_channel_curr_file[slot_index].path_len == 0)
    {
      return MMGSDI_ERROR;
    }
    mmgsdi_memscpy(file_id_and_path_ptr, sizeof(mmgsdi_path_type),
                   &mmgsdi_crsm_default_channel_curr_file[slot_index], sizeof(mmgsdi_path_type));
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_crsm_calculate_cmd_params_and_file_path */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_GET_FILE_ENUM_AND_PATH_FROM_SFI

  DESCRIPTION:
    This function calculates the file access and path based on the SFI value.

  DEPENDENCIES:
    None

  LIMITATIONS:
    Currently support lookup for EFs under MF or under CSIM/USIM ADF only.

  RETURN VALUE:
    mmgsdi_return_enum_type

  SIDE EFFECTS:
    None
=============================================================================*/
static mmgsdi_return_enum_type mmgsdi_crsm_get_file_enum_and_path_from_sfi
(
  uint8                                    sfi,
  mmgsdi_app_enum_type                     app_type,
  mmgsdi_path_type                       * file_id_and_path_ptr,
  mmgsdi_access_type                     * file_access_ptr
)
{
  mmgsdi_return_enum_type     mmgsdi_status = MMGSDI_ERROR;
  mmgsdi_path_type            path_to_df    = {0};
  uint16                      curr_file     = 0;

  MMGSDIUTIL_RETURN_IF_NULL_2(file_id_and_path_ptr, file_id_and_path_ptr);
  MMGSDIUTIL_RETURN_IF_OUT_OF_RANGE(file_id_and_path_ptr->path_len, 1, MMGSDI_MAX_PATH_LEN);

  curr_file = file_id_and_path_ptr->path_buf[file_id_and_path_ptr->path_len - 1];

  /* Compose path to DF based on the currently selected file ID. This path
     is used to search for file enum in the correct SFI table. SFI access
     is currently supported only for files under MF or under ADF. */
  if ((curr_file == (uint16)MMGSDI_MF_DIR) ||
      ((curr_file & 0xFF00) == 0x2F00))
  {
    path_to_df.path_len = 1;
    path_to_df.path_buf[0] = (uint16)MMGSDI_MF_DIR;
  }
  else if ((curr_file == (uint16)MMGSDI_ADF_LEVEL) ||
           ((curr_file & 0xFF00) == 0x6F00 &&
            (app_type == MMGSDI_APP_USIM || app_type == MMGSDI_APP_CSIM)))
  {
    path_to_df.path_len = 2;
    path_to_df.path_buf[0] = (uint16)MMGSDI_MF_DIR;
    path_to_df.path_buf[1] = (uint16)MMGSDI_ADF_LEVEL;
  }
  else
  {
    return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_file_get_file_from_sfi(sfi,
                                                app_type,
                                                path_to_df,
                                                &file_access_ptr->file.file_enum);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }
  file_access_ptr->access_method = MMGSDI_EF_ENUM_ACCESS;

  /* Update file path for directory maintenance after successful access */
  mmgsdi_status = mmgsdi_file_get_path_from_file(file_access_ptr->file.file_enum,
                                                 file_id_and_path_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_crsm_get_file_enum_and_path_from_sfi */


/* ============================================================================
  FUNCTION:      MMGSDI_CRSM_DEFAULT_CHANNEL_APDU_ACCESS

  DESCRIPTION:
    This function translates the APDU streaming command on channel zero to a
    high-level MMGSDI API call.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type

  SIDE EFFECTS:
    None
=============================================================================*/
mmgsdi_return_enum_type mmgsdi_crsm_default_channel_apdu_access
(
  mmgsdi_client_id_type                    client_id,
  mmgsdi_slot_id_enum_type                 card_slot,
  mmgsdi_send_apdu_options_enum_type       apdu_option,
  mmgsdi_send_apdu_data_type               apdu_data,
  mmgsdi_client_data_type                  client_ref,
  mmgsdi_callback_type                     client_response_cb
)
{
  uim_instrns_enum_type                    command              = 0;
  mmgsdi_app_enum_type                     app_type             = MMGSDI_APP_NONE;
  mmgsdi_return_enum_type                  mmgsdi_status        = MMGSDI_ERROR;
  mmgsdi_session_id_type                   session_id           = 0;
  mmgsdi_apdu_params_type                  apdu_params          = {0};
  mmgsdi_data_type                         cmd_data             = {0};
  mmgsdi_path_type                         file_id_and_path     = {0};
  boolean                                  sfi_access           = FALSE;
  mmgsdi_restricted_sim_access_data_type * apdu_client_data_ptr = NULL;
  mmgsdi_access_type                       file_access;

  MMGSDIUTIL_RETURN_IF_NULL(apdu_data.data_ptr);

  if (apdu_data.data_len < UIM_CASE1_7816_APDU_SIZE)
  {
    return MMGSDI_ERROR;
  }

  switch ((uim_instrns_enum_type)apdu_data.data_ptr[UIM_7816_APDU_INSTRN_OFFSET])
  {
    case SELECT:
    case AUTHENTICATE:
    case UPDATE_BINARY:
    case UPDATE_RECORD:
    case READ_BINARY:
    case READ_RECORD:
      command = (uim_instrns_enum_type)apdu_data.data_ptr[UIM_7816_APDU_INSTRN_OFFSET];
      break;
    default:
      return MMGSDI_ERROR;
  }

  mmgsdi_status = mmgsdi_crsm_calculate_cmd_params_and_file_path(card_slot,
                                                                 command,
                                                                 apdu_data,
                                                                 &apdu_params,
                                                                 &file_id_and_path,
                                                                 &cmd_data);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    return mmgsdi_status;
  }

  mmgsdi_status = mmgsdi_crsm_find_session_for_access(card_slot,
                                                      file_id_and_path,
                                                      command,
                                                      &session_id,
                                                      &app_type,
                                                      TRUE);
  if (mmgsdi_status != MMGSDI_SUCCESS || session_id == MMGSDI_INVALID_SESSION_ID)
  {
    UIM_MSG_ERR_1("Unable to find MMGSDI session for sending APDU command 0x%x",
                  command);
    return MMGSDI_ERROR;
  }

  /* Only 3GPP authentication is supported */
  if (command == AUTHENTICATE &&
      (app_type != MMGSDI_APP_USIM && app_type != MMGSDI_APP_SIM))
  {
    return MMGSDI_ERROR;
  }

  memset(&file_access, 0x00, sizeof(file_access));

  /* If the read/write access is being done using short file identifiers (SFI),
     extract the SFI value and search for the corresponding file path/enum
     from the MMGSDI tables. Access using SFI can be converted to an MMGSDI
     request only if the path/enum for that SFI is already known. */
  if (((command == READ_BINARY || command == UPDATE_BINARY) &&
       ((apdu_params.p1 & MMGSDI_CRSM_P1_SFI_REF_MASK) == MMGSDI_CRSM_P1_SFI_REF)) ||
      ((command == READ_RECORD || command == UPDATE_RECORD) &&
       (apdu_params.p2 & MMGSDI_CRSM_P2_SFI_REF)))
  {
    uint8 sfi = 0;

    if (command == READ_RECORD || command == UPDATE_RECORD)
    {
      sfi = (apdu_params.p2 & MMGSDI_CRSM_P2_SFI_REF) >> 3;
    }
    else if (command == READ_BINARY || command == UPDATE_BINARY)
    {
      sfi = apdu_params.p1 & MMGSDI_CRSM_P1_SFI_MASK;
    }

    mmgsdi_status = mmgsdi_crsm_get_file_enum_and_path_from_sfi(sfi,
                                                                app_type,
                                                                &file_id_and_path,
                                                                &file_access);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      UIM_MSG_ERR_1("Unable to find file enum for sfi 0x%x", sfi);
      return mmgsdi_status;
    }
    sfi_access = TRUE;
  }

  /* Calculate file enum for the currently-selected path in case the access
     is not being done using SFI */
  if (command != AUTHENTICATE && !sfi_access)
  {
    mmgsdi_status = mmgsdi_crsm_calculate_file_access(app_type,
                                                      &file_id_and_path,
                                                      &file_access);
    if (mmgsdi_status != MMGSDI_SUCCESS)
    {
      return mmgsdi_status;
    }
  }

  /* Store client data on heap so that the response from MMGSDI can be
     returned back to the client later */
  mmgsdi_status = mmgsdi_crsm_copy_restricted_sim_access_client_data(client_id,
                                                                     card_slot,
                                                                     command,
                                                                     apdu_params,
                                                                     apdu_option,
                                                                     file_id_and_path,
                                                                     client_response_cb,
                                                                     client_ref,
                                                                     &apdu_client_data_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS || apdu_client_data_ptr == NULL)
  {
    return mmgsdi_status;
  }

  /* Map the APDU command to an MMGSDI request and queue to MMGSDI. */
  mmgsdi_status = mmgsdi_crsm_queue_restricted_sim_access_cmd_ext(session_id,
                                                                  file_access,
                                                                  app_type,
                                                                  command,
                                                                  &apdu_params,
                                                                  cmd_data,
                                                                  (mmgsdi_client_data_type)apdu_client_data_ptr,
                                                                  mmgsdi_crsm_apdu_access_cb);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    MMGSDIUTIL_TMC_MEM_FREE(apdu_client_data_ptr);
  }

  UIM_MSG_HIGH_2("Queue of MMGSDI req for APDU command 0x%x: status 0x%x",
                 command, mmgsdi_status);
  return mmgsdi_status;
} /* mmgsdi_crsm_default_channel_apdu_access */

