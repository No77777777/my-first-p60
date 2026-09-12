/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            G S T K   C O N T A C T L E S S  S T A T E  C H A N G E D


GENERAL DESCRIPTION
  This source file contains functions to handle contact less state changed
  proactive command


FUNCTIONS
  gstk_contactless_state_changed_req
    This function parses the Contactless State Changed Proactive Command
    from the card and sends it to the client.

  gstk_contactless_state_changed_cnf_cleanup
    This function cleans up any allocated pointer on the stack.

  gstk_contactless_state_changed_cnf
    This function packs the Contactless State Changed terminal response
    and send the command to UIM server.

  gstk_contactless_state_changed_cnf_cleanup
    This function cleans up any allocated pointer on the stack.

  gstk_contactless_state_changed_parsing_err_term_rsp
    This function receive terminal response as a result of parsing error.

INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2003-2016 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/gstk/src/gstk_contactless_state_changed.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/07/16   shr     GSTK/ESTK F3 reduction
05/26/16   shr     F3 reduction: Malloc Failure, Memcpy, Null Ptr checks
01/11/16   shr     Send 0x30 TR result instead of 0x20 in no client usecase
11/12/15   vr      F3 Logging inprovements
08/10/15   vr      Split Activate and contactless proactive command files
07/02/15   shr     Use unsigned int for offset
06/25/15   shr     Fix potential buffer overflow while building Envelope/TR
04/08/15   dy      Fix crash by setting user_data to GSTK client id
02/27/15   dy      Initial Version

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "gstk.h"
#include "gstk_proactive_cmd.h"
#include "gstkutil.h"
#include "gstk_terminal_rsp.h"

#include "err.h"
#include "string.h"
#include "uim_selective_logging.h"

/*===========================================================================
FUNCTION gstk_contactless_state_req_cleanup

DESCRIPTION
  This function clean up any stack allocated pointer.

PARAMETERS
  request_ptr : [Input] Pointer to gstk_cmd_from_card_type
  cmd_data_ptr: [Input] Pointer to cmd_data_ptr

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
static void gstk_contactless_state_req_cleanup(
  gstk_cmd_from_card_type *request_ptr,
  uint8                   *cmd_data_ptr)
{
  gstk_util_release_card_cmd_memory(request_ptr);
  if (cmd_data_ptr != NULL) {
    gstk_free(cmd_data_ptr);
  }
} /* gstk_contactless_state_req_cleanup */

/*===========================================================================
FUNCTION gstk_contactless_state_cnf_cleanup

DESCRIPTION
  This function cleans up any allocated pointer on the stack.

PARAMETERS
  contactless_state_cnf_ptr: [Input] Pointer to message that is required to
                             be processed and sent to UIM
  STK_response_ptr         : [Input] Pointer to the STK intermediate message

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
static void gstk_contactless_state_cnf_cleanup(
  gstk_contactless_state_changed_cnf_type  *contactless_state_cnf_ptr,
  display_text_terminal_response_type  *STK_response_ptr)
{
   if(STK_response_ptr == NULL || contactless_state_cnf_ptr == NULL)
  {
    UIM_MSG_ERR_2("gstk_contactless_state_cnf_cleanup(): Err STK_response_ptr 0x%x, "
                  "contactless_state_cnf_ptr 0x%x",
                  STK_response_ptr, contactless_state_cnf_ptr);
    return;
  }

  /* free display text cnf and display tlv */
  gstk_util_release_upper_layer_cmd_memory(
    (gstk_cmd_type*)((void*)contactless_state_cnf_ptr));

  if (STK_response_ptr->data.result.additional_info != NULL) {
    gstk_free(STK_response_ptr->data.result.additional_info);
    STK_response_ptr->data.result.additional_info = NULL;
  }
} /* gstk_display_text_cnf_cleanup */

/*===========================================================================
FUNCTION gstk_contactless_state_parsing_err_term_rsp

DESCRIPTION
  This function receive terminal response as a result of parsing error.

PARAMETERS
  cmd_details_ref_id  : [Input] command details ref id
  result              : [Input] GSTK result
  additional_info_ptr : [Input] additional info pointer

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
static void gstk_contactless_state_parsing_err_term_rsp(
  uint32                              cmd_details_ref_id,
  gstk_general_result_enum_type       result,
  const gstk_additional_info_type    *additional_info_ptr)
{
  gstk_status_enum_type               gstk_status       = GSTK_SUCCESS;
  gstk_terminal_rsp_extra_param_type  dummy_extra_param = {0};

  UIM_MSG_ERR_0("Receive Data TLV parsing error");

  /* initialize dummy_extra_param */
  dummy_extra_param.present = FALSE;

  /* check for Null ptr */
  GSTK_RETURN_IF_NULL_PTR(gstk_curr_inst_ptr);

  /* Send Terminal Response to Card */
  /* COMMAND_DATA_NOT_UNDERSTOOD_BY_ME */
  gstk_status = gstk_send_terminal_response(
                  gstk_curr_inst_ptr->client_id,     /* client_id */
                  0,                  /* user_data */
                  cmd_details_ref_id, /* command_details_ref */
                  command_details_ref_table_ptr[GSTK_CMD_REF_TABLE_INDEX(cmd_details_ref_id)].command_details.command_number,  /* command_num */
                  GSTK_CONTACTLESS_STATE_CHANGED_CNF,   /* command response */
                  result, /* general_result */
                  additional_info_ptr,   /* additional result */
                  &dummy_extra_param  /* extra param */ );
  if (gstk_status != GSTK_SUCCESS) {
    UIM_MSG_ERR_1("TR fail, 0x%x", gstk_status);
  }
} /* gstk_contactless_state_parsing_err_term_rsp */

/*===========================================================================
FUNCTION gstk_contactless_state_changed_req

DESCRIPTION
  This function parses the Contactless State Changed Proactive Command
  from the card and sends it to the client.

  In case of any parsing errors or when GSTK can't send the command to the
  client, it will call the gstk_send_terminal_response function so that
  gstk can send the terminal response back to the card.

PARAMETERS
  command: [Input] Pointer to message parsed and sent to clients

DEPENDENCIES
  None

RETURN VALUE
  gstk_status_enum_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_status_enum_type gstk_contactless_state_changed_req  (
  gstk_proactive_cmd_type* command_ptr)
{
  uint8                              *cmd_data_ptr         = NULL;
  int32                               cmd_data_len         = 0;
  uint32                              cmd_details_ref_id   = 0;
  int32                               curr_offset          = 0;
  gstk_status_enum_type               gstk_status          = GSTK_SUCCESS;
  int32                               new_offset           = 0;
  gstk_additional_info_type           additional_info      = {0};
  boolean                             more_tlv             = FALSE;
  gstk_general_result_enum_type       gen_result           = GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND;
  
  /* Cannot do {0x00} in declaration because of compilation warning
     on certain target: "enumerated type mixed with another type" */
  gstk_cmd_from_card_type             request;

  UIM_MSG_HIGH_0("IN GSTK_CONTACTLESS_STATE_CHANGED_REQ()");

  GSTK_RETURN_ERROR_IF_NULL_PTR(gstk_curr_inst_ptr, GSTK_ERROR);

  if(!command_ptr)
  {
    UIM_MSG_ERR_0("command_ptr is NULL");
    return GSTK_NULL_INPUT_PARAM;
  }

  /* initialize request */
  memset(&request, 0x00, sizeof(gstk_cmd_from_card_type));
  request.cmd.contactless_state_changed_cmd_req.state = GSTK_CONTACTLESS_FUNC_STATE_INVALID;

  gstk_status = gstk_preprocess_proactive_cmd((gstk_proactive_cmd_type*)command_ptr,
                                              &cmd_data_ptr,
                                              &cmd_data_len,
                                              &cmd_details_ref_id );

  /* There exists mandatory tlv, so cmd_data_ptr should not be null */
  if ((gstk_status != GSTK_SUCCESS) || (cmd_data_ptr == NULL))
  {
    gstk_contactless_state_parsing_err_term_rsp(cmd_details_ref_id,
                                                GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME,
                                                &additional_info);
    /* release memory allocation for receive data request info data and cmd_data_ptr accordingly */
    gstk_contactless_state_req_cleanup(&request, cmd_data_ptr);
    return gstk_status;
  }

  /* build the proactive cmd request into the gstk_exp.h _req structure */

  /* Message Header */
  request.hdr_cmd.command_id = GSTK_CONTACTLESS_STATE_CHANGED_REQ;
  request.hdr_cmd.cmd_detail_reference = cmd_details_ref_id;

  /* command number */
  request.hdr_cmd.command_number =
      command_details_ref_table_ptr[GSTK_CMD_REF_TABLE_INDEX(cmd_details_ref_id)].command_details.command_number;

  /* Contactless functionality state */
  gstk_status = gstk_parser_contactless_func_state_tlv(cmd_data_ptr,
                                                       cmd_data_len,
                                                       curr_offset,
                                                       GSTK_CONTACTLESS_FUNC_STATE_TAG,
                                                       &(request.cmd.contactless_state_changed_cmd_req.state),
                                                       &new_offset,
                                                       &more_tlv,
                                                       GSTK_MANDATORY,
                                                       &cmd_details_ref_id);

  if (gstk_status != GSTK_SUCCESS || more_tlv == TRUE)
  {
    gstk_contactless_state_parsing_err_term_rsp(cmd_details_ref_id,
                                                GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME,
                                                &additional_info);
    /* release memory allocation for receive data request info data and cmd_data_ptr accordingly */
    gstk_contactless_state_req_cleanup(&request, cmd_data_ptr);
    return gstk_status;
  }

  /* Send command to clients */
  gstk_status = gstk_util_send_message_to_clients(&request);

  if(gstk_status != GSTK_SUCCESS)
  {
    if(gstk_status == GSTK_NO_CLIENT_REG_FOUND)
    {
      gen_result = GSTK_COMMAND_BEYOND_THE_ME_CAPABILITIES;
    }  

    UIM_MSG_HIGH_0("Contactless State Changed not able to send to client");

    /* Send Terminal Response to Card */
    gstk_set_additional_info(
      &additional_info, GSTK_NO_SPECIFIC_CAUSE_CAN_BE_GIVEN);

    gstk_contactless_state_parsing_err_term_rsp(
      cmd_details_ref_id,
      gen_result,
      &additional_info);
  }
  else
  {
    /* Start Timer */
    gstk_util_start_client_response_timer(cmd_details_ref_id,
                                          GSTK_TERM_RSP_WAIT_TIME);
    /* reset gstk_status to SUCCESS since there has been no error*/
    /* This is needed since some of the TLVs are optional, which might
    ** cause the various parsing functions to return invalid_command */
    gstk_status = GSTK_SUCCESS;
  }

  /* release memory allocation for text string if not null */
  gstk_contactless_state_req_cleanup(&request, cmd_data_ptr);
  return(gstk_status);
} /* gstk_contactless_state_changed_req*/

/*===========================================================================
FUNCTION gstk_contactless_state_changed_cnf

DESCRIPTION
  This function packs the Contactless State Changed terminal response
  and send the command to UIM server.

PARAMETERS
  contactless_state_changed_cnf_ptr: [Input] Pointer to message that is required to
                                      be processed and sent to UIM

DEPENDENCIES
  None

RETURN VALUE
  gstk_status_enum_type

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
gstk_status_enum_type gstk_contactless_state_changed_cnf (
  gstk_contactless_state_changed_cnf_type *contactless_state_changed_cnf_ptr )
{
  gstk_status_enum_type                             gstk_status  = GSTK_SUCCESS;
  contactless_state_changed_terminal_response_type  STK_response = {0};
  uim_cmd_type                                     *uim_cmd_ptr  = NULL;
  uint32                                            offset       = 0;
  uint32                                            ref_id       = 0;

  UIM_MSG_HIGH_0("IN GSTK_CONTACTLESS_STATE_CHANGED_CNF ");

  if(contactless_state_changed_cnf_ptr == NULL)
  {
    UIM_MSG_ERR_0("Contactless State Changed Cnf ERR: NULL");
    return GSTK_BAD_PARAM;
  }

  /* initialize STK_response */
  memset(&STK_response, 0x00, sizeof(contactless_state_changed_terminal_response_type));

  /* Pack various TLVs */

  /* Command details */
  /* from the cached table */
  gstk_status = gstk_packer_cmd_details_tlv(
      contactless_state_changed_cnf_ptr->cmd_details_ref_id,
      contactless_state_changed_cnf_ptr->message_header.command_id,
      contactless_state_changed_cnf_ptr->command_number,
      &STK_response.data.command_details );

  if (gstk_status != GSTK_SUCCESS) {
    gstk_contactless_state_cnf_cleanup(contactless_state_changed_cnf_ptr, &STK_response);
    return gstk_status;
  }

  /* device ID */
  gstk_status = gstk_packer_dev_id_tlv(
      GSTK_ME_DEVICE,
      GSTK_UICC_SIM_DEVICE,
      &STK_response.data.device_id );


  if (gstk_status != GSTK_SUCCESS) {
    gstk_contactless_state_cnf_cleanup(contactless_state_changed_cnf_ptr, &STK_response);
    return gstk_status;
  }

  /* result tag */
  gstk_status = gstk_packer_result_tlv(
      contactless_state_changed_cnf_ptr->command_result,
      contactless_state_changed_cnf_ptr->result_additional_info,
      &STK_response.data.result);

  if (gstk_status != GSTK_SUCCESS) {
    gstk_contactless_state_cnf_cleanup(contactless_state_changed_cnf_ptr, &STK_response);
    return gstk_status;
  }

  /* total data length:  command details data + 2   (2 is for the tag and length field)
                      + device id data + 2
                      + result data + 2
  */
  STK_response.num_bytes = STK_response.data.command_details.length +
                           STK_response.data.device_id.device_tag_length +
                           STK_response.data.result.result_tag_length +
                           6;

  /* Send response to STK application on SIM */
  /* Send Message to UIM */
  /* get a buffer for the request */
  uim_cmd_ptr = gstk_get_uim_buf();
  if (uim_cmd_ptr != NULL ) {
    gstk_status = gstk_util_populate_uim_buf_hdr_info(UIM_TERMINAL_RESPONSE_F, &uim_cmd_ptr);
    if (gstk_status == GSTK_SUCCESS) {
      if (uim_cmd_ptr != NULL) {
        ref_id = contactless_state_changed_cnf_ptr->cmd_details_ref_id;

        /* clear the command detail table reference slot before since we are about to reply to
          the card */
        gstk_util_cmd_details_ref_table_free_slot(contactless_state_changed_cnf_ptr->cmd_details_ref_id);

        /* set user_data */
        uim_cmd_ptr->hdr.user_data               = gstk_curr_inst_ptr->client_id;
        /* populate the terminal_response info */
        uim_cmd_ptr->terminal_response.num_bytes = STK_response.num_bytes;

        /* copy command details */
        if(gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                                   &STK_response.data.command_details,
                                   offset,
                                   (size_t)(STK_response.data.command_details.length + 2),
                                   sizeof(uim_cmd_ptr->terminal_response.data),
                                   sizeof(gstk_command_details_tlv_type)) <
           (size_t)(STK_response.data.command_details.length + 2))
        {
          UIM_MSG_ERR_0("gstk_contactless_state_changed_cnf: gstk_byte_offset_memcpy failed");
          gstk_status = GSTK_MEMORY_ERROR;
        }
        /* copy device */
        else if(gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                                        &STK_response.data.device_id,
                                        offset +=
                                        (int)(STK_response.data.command_details.length + 2),
                                        (size_t)(STK_response.data.device_id.device_tag_length + 2),
                                        sizeof(uim_cmd_ptr->terminal_response.data),
                                        sizeof(gstk_device_identities_tag_tlv_type)) <
                (size_t)(STK_response.data.device_id.device_tag_length + 2))
        {
          UIM_MSG_ERR_0("gstk_contactless_state_changed_cnf: gstk_byte_offset_memcpy failed");
          gstk_status = GSTK_MEMORY_ERROR;
        }
        /* copy result */
        else if(gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                                        &STK_response.data.result,
                                        offset +=
                                        (int)(STK_response.data.device_id.device_tag_length + 2),
                                        3,/* result tag +  length + general result */
                                        sizeof(uim_cmd_ptr->terminal_response.data),
                                        sizeof(gstk_result_tag_tlv_type)) < 3)
        {
          UIM_MSG_ERR_0("gstk_contactless_state_changed_cnf: gstk_byte_offset_memcpy failed");
          gstk_status = GSTK_MEMORY_ERROR;
        }
        offset = offset + 3;
        /* has additional info */
        if(STK_response.data.result.result_tag_length > 1 &&
                (gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                                         STK_response.data.result.additional_info,
                                         offset,
                                         (size_t)(STK_response.data.result.result_tag_length - 1),
                                         sizeof(uim_cmd_ptr->terminal_response.data),
                                         ((STK_response.data.result.result_tag_length - 1)*
                                          sizeof(byte))) <
                (size_t)(STK_response.data.result.result_tag_length - 1)))
        {
          UIM_MSG_ERR_0("gstk_contactless_state_changed_cnf: gstk_byte_offset_memcpy failed");
          gstk_status = GSTK_MEMORY_ERROR;
        }

        if (gstk_status == GSTK_SUCCESS)
        {
          GSTK_UTIL_SELECTIVE_DUMP_BYTE_ARRAY("Contactless State Changed TR",
                                              uim_cmd_ptr->terminal_response.data,
                                              uim_cmd_ptr->terminal_response.num_bytes);
          gstk_send_cmd_to_uim_server(uim_cmd_ptr); /* clear signal and call uim_cmd */
        }
        else
        {
          gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
          uim_cmd_ptr = NULL;
        }
      }
    }
  } /* uim_cmd_ptr != NULL */
  else {
    gstk_status = GSTK_MEMORY_ERROR;
  }

  gstk_contactless_state_cnf_cleanup(contactless_state_changed_cnf_ptr, &STK_response);
  return gstk_status;
} /* gstk_contactless_state_changed_cnf */

/*===========================================================================
FUNCTION gstk_update_class_r_tp

DESCRIPTION
  This function will return false in stub code to disable TP and TRUE otherwise
  for class R TP bits

PARAMETERS
  None

DEPENDENCIES
  None

RETURN VALUE
  None

COMMENTS
  None

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
boolean gstk_update_class_r_tp ( void )
{
  return TRUE;
} /* gstk_update_class_r_tp */
