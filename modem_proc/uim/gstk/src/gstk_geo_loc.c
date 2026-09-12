/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                     G S T K  G E O G R A P H I C A L  L O C A T I O N  R E Q U E S T


GENERAL DESCRIPTION
  This source file contains functions to process geographical location
  proactive command


FUNCTIONS
  gstk_geo_loc_req
    This function parses the Geo Loc Command from the card
    and sends it to the client.

  gstk_geo_loc_cnf
    This function packs the Geo Loc terminal response and sends
    the command to UIM server.

  gstk_geo_loc_cnf_cleanup
    This function cleans up any allocated pointer on the stack.

  gstk_geo_loc_req_cleanup
    This function cleans up any allocated pointer on the stack


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2015 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All  Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/gstk/src/gstk_geo_loc.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/23/17   gs      Move Geo Loc Envelope to respective file
10/07/16   shr     GSTK/ESTK F3 reduction
09/23/16   gs      F3 reduction / cleanup
05/27/16   shr     F3 reduction: Malloc Failure, Memcpy, Null Ptr checks
01/11/16   shr     Send 0x30 TR result instead of 0x20 in no client usecase
11/12/15   vr      F3 Logging inprovements
06/25/15   shr     Fix potential buffer overflow while building Envelope/TR
03/26/15   gm      Initial Version

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* GSTK Header Files   */
#include "gstk.h"
#include "gstk_proactive_cmd.h"
#include "gstkutil.h"
#include "gstk_terminal_rsp.h"
#include "uim_selective_logging.h"
#include "gstk_envelope_cmd.h"

/*===========================================================================
FUNCTION gstk_geo_loc_cnf_cleanup

DESCRIPTION
  This function clean up any stack allocated pointer.

PARAMETERS
  geo_loc_cnf_ptr  : [Input] Pointer to message that is required to
                             be processed and sent to UIM
  STK_response_ptr : [Input] Pointer to the STK intermediate message

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
static void gstk_geo_loc_cnf_cleanup(
  gstk_geo_loc_cnf_type          *geo_loc_cnf_ptr,
  geo_loc_terminal_response_type *STK_response_ptr)
{
  if((STK_response_ptr == NULL)||(geo_loc_cnf_ptr == NULL))
  {
    return;
  }

  /* free Geo Loc tlv */
  gstk_util_release_upper_layer_cmd_memory(
    (gstk_cmd_type*)((void*)geo_loc_cnf_ptr));
  if (STK_response_ptr->data.result.additional_info != NULL) {
    gstk_free(STK_response_ptr->data.result.additional_info);
    STK_response_ptr->data.result.additional_info = NULL;
  }
} /* gstk_geo_loc_cnf_cleanup */

/*===========================================================================
FUNCTION gstk_geo_loc_req_cleanup

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
static void gstk_geo_loc_req_cleanup(
  gstk_cmd_from_card_type *request_ptr,
  uint8                   *cmd_data_ptr)
{
  if(request_ptr == NULL)
  {
    return;
  }
  gstk_util_release_card_cmd_memory(request_ptr);
  if (cmd_data_ptr != NULL) {
    gstk_free(cmd_data_ptr);
  }
} /* gstk_geo_loc_req_cleanup */

/*===========================================================================
FUNCTION gstk_geo_loc_parsing_err_term_rsp

DESCRIPTION
  This function send terminal response as a result of parsing error.

PARAMETERS
  cmd_details_ref_id : [Input]  command details reference id
  result             : [Input] GSTK result
  additional_info_ptr: [Input] Pointer to addtional info

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
static void gstk_geo_loc_parsing_err_term_rsp(
  uint32                              cmd_details_ref_id,
  gstk_general_result_enum_type       result,
  const gstk_additional_info_type     *additional_info_ptr)
{
  gstk_status_enum_type               gstk_status       = GSTK_SUCCESS;
  gstk_terminal_rsp_extra_param_type  dummy_extra_param = {0};

  GSTK_RETURN_IF_NULL_PTR(gstk_curr_inst_ptr);

  UIM_MSG_ERR_0("Geo Loc TLV parsing error");

  gstk_status = gstk_util_compare_ref_id(cmd_details_ref_id);
  if (gstk_status != GSTK_SUCCESS) {
    return;
  }

  /* Send Terminal Response to Card */
  gstk_status = gstk_send_terminal_response(
                  gstk_curr_inst_ptr->client_id,     /* client_id */
                  0,                  /* user_data */
                  cmd_details_ref_id, /* command_details_ref */
                  command_details_ref_table_ptr[GSTK_CMD_REF_TABLE_INDEX(cmd_details_ref_id)].command_details.command_number,  /* command_num */
                  GSTK_GEO_LOC_CNF,   /* command response */
                  result, /* general_result */
                  additional_info_ptr,   /* additional result */
                  &dummy_extra_param  /* extra param */ );
  if (gstk_status != GSTK_SUCCESS) {
    UIM_MSG_ERR_1("TR fail, 0x%x", gstk_status);
  }
} /* gstk_geo_loc_parsing_err_term_rsp */

/*===========================================================================
FUNCTION gstk_geo_loc_req

DESCRIPTION
  This function parses the Geographical location request Command from the card
  and sends it to the client.

  In case of any parsing errors or when GSTK can't send the command to the
  client, it will call the gstk_send_terminal_response function so that  gstk
  can send the terminal response back to the card.

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
gstk_status_enum_type gstk_geo_loc_req (
  gstk_proactive_cmd_type *command_ptr
)
{
  gstk_status_enum_type               gstk_status         = GSTK_SUCCESS;
  uint8                               *cmd_data_ptr       = NULL;
  int32                               cmd_data_len        = 0;
  uint32                              cmd_details_ref_id  = 0;
  int32                               curr_offset         = 0;
  int32                               new_offset          = 0;
  boolean                             more_tlv            = FALSE;
  gstk_additional_info_type           additional_info     = {0};
  gstk_general_result_enum_type       gen_result          = GSTK_ME_CURRENTLY_UNABLE_TO_PROCESS_COMMAND;

  /* Cannot do {0x00} in declaration because of compilation warning
     on certain target: "enumerated type mixed with another type" */
  gstk_cmd_from_card_type             request;

  UIM_MSG_HIGH_1(" IN GSTK_GEO_LOC_REQ: command_ptr=0x%x", command_ptr);

  if(!command_ptr)
  {
    return GSTK_BAD_PARAM;
  }

  /* initialize request to 0 */
  memset(&request, 0x00, sizeof(gstk_cmd_from_card_type));

  /* Check and populate command details into the cmd_details_ref_table
  ** Check Device ID validity
  ** put the rest of the command into the cmd_data_ptr, and specifies the length of
  ** the cmd_data_ptr in the cmd_data_len
  */
  gstk_status = gstk_preprocess_proactive_cmd((gstk_proactive_cmd_type*)command_ptr,
                                              &cmd_data_ptr,
                                              &cmd_data_len,
                                              &cmd_details_ref_id );

  if ((gstk_status != GSTK_SUCCESS) || (cmd_data_ptr == NULL))
  {

    gstk_geo_loc_parsing_err_term_rsp(cmd_details_ref_id,
                                          GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME,
                                          &additional_info);

    /* release memory allocation for Geo Loc request info data and cmd_data_ptr accordingly */
    gstk_geo_loc_req_cleanup(&request, cmd_data_ptr);
    return gstk_status;
  }

  /* build the proactive cmd request into the gstk_exp.h _req structure */

  /* Message Header */
  request.hdr_cmd.command_id = GSTK_GEO_LOC_REQ;
  request.hdr_cmd.cmd_detail_reference = cmd_details_ref_id;

  /* command number */
  request.hdr_cmd.command_number =
      command_details_ref_table_ptr[GSTK_CMD_REF_TABLE_INDEX(cmd_details_ref_id)].command_details.command_number;

  /* optional alpha TLV */
  gstk_status = gstk_parser_alpha_identifier_tlv_ext( GSTK_ALPHA_IDENTIFIER_TAG,
                                                      cmd_data_ptr,
                                                      cmd_data_len,
                                                      curr_offset,
                                                      &(request.cmd.geo_loc_pro_cmd_req.alpha),
                                                      &new_offset,
                                                      &more_tlv,
                                                      GSTK_OPTIONAL,
                                                      &cmd_details_ref_id);

  if (gstk_status != GSTK_SUCCESS) {
    gstk_geo_loc_parsing_err_term_rsp(cmd_details_ref_id,
                                        GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME,
                                        &additional_info);
    /* release memory allocation for Geo Loc request info data and cmd_data_ptr accordingly */
    gstk_geo_loc_req_cleanup(&request, cmd_data_ptr);
    return gstk_status;
  }
  else {
    if (!more_tlv) {
      gstk_geo_loc_parsing_err_term_rsp(cmd_details_ref_id,
                                          GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME,
                                          &additional_info);
      /* release memory allocation for Geo Loc request info data and cmd_data_ptr accordingly */
      gstk_geo_loc_req_cleanup(&request, cmd_data_ptr);
      return GSTK_INVALID_COMMAND;
    }
    curr_offset = new_offset;
  }

  /* optional ICON TLV */
  if(gstk_valid_tag(cmd_data_ptr[curr_offset],GSTK_ICON_IDENTIFIER_TAG)) {
    /* there is icon TLV */
    /* Icon ID */
    gstk_status = gstk_parser_icon_id_tlv_ext(
        &(request.cmd.geo_loc_pro_cmd_req.alpha),
        cmd_data_ptr,
        cmd_data_len,
        curr_offset,
        &(request.cmd.geo_loc_pro_cmd_req.icon),
        &new_offset,
        &more_tlv,
        GSTK_OPTIONAL,
        &cmd_details_ref_id);

    if(gstk_status != GSTK_SUCCESS) {
      gstk_geo_loc_parsing_err_term_rsp(cmd_details_ref_id,
                                          GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME,
                                          &additional_info);
      /* release memory allocation for Geo Loc request info data and cmd_data_ptr accordingly */
      gstk_geo_loc_req_cleanup(&request, cmd_data_ptr);
      return gstk_status;
    }
    else {
      if (!more_tlv) {
        gstk_geo_loc_parsing_err_term_rsp(cmd_details_ref_id,
                                            GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME,
                                            &additional_info);
        /* release memory allocation for Geo Loc request info data and cmd_data_ptr accordingly */
        gstk_geo_loc_req_cleanup(&request, cmd_data_ptr);
        return GSTK_INVALID_COMMAND;
      }
      curr_offset = new_offset;
    }
  } /* optional icon tlv */

  /* Continue with Geographical location parameter  parsing: Mandatory */
  gstk_status = gstk_parser_geo_loc_para_tlv(cmd_data_ptr,
                                        cmd_data_len,
                                        curr_offset,
                                        &(request.cmd.geo_loc_pro_cmd_req.geo_loc_para),
                                        &new_offset,
                                        &more_tlv,
                                        GSTK_MANDATORY,
                                        &cmd_details_ref_id);

  if(gstk_status != GSTK_SUCCESS) {
    gstk_geo_loc_parsing_err_term_rsp(cmd_details_ref_id,
                                        GSTK_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME,
                                        &additional_info);
    /* release memory allocation for Geo Loc request info data and cmd_data_ptr accordingly */
    gstk_geo_loc_req_cleanup(&request, cmd_data_ptr);
    return gstk_status;
  }
  else {
    curr_offset = new_offset;
  }

  if (more_tlv) {
    /* There are more tlvs after the last spec specified tlv, ignore */
    UIM_MSG_HIGH_0("Card Provide more TLV after final TLV element, Ignore");
  }

  /* Send command to clients */
  gstk_status = gstk_util_send_message_to_clients(&request);

  if(gstk_status != GSTK_SUCCESS)
  {
    if(gstk_status == GSTK_NO_CLIENT_REG_FOUND)
    {
      gen_result = GSTK_COMMAND_BEYOND_THE_ME_CAPABILITIES;
    }

    /* Send Terminal Response to Card */
    gstk_set_additional_info(&additional_info,GSTK_NO_SPECIFIC_CAUSE_CAN_BE_GIVEN);

    gstk_geo_loc_parsing_err_term_rsp(cmd_details_ref_id,
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

  /* release memory allocation for Geo Loc request info data and cmd_data_ptr accordingly */
  gstk_geo_loc_req_cleanup(&request, cmd_data_ptr);
  return(gstk_status);
} /* gstk_geo_loc_req */

/*===========================================================================
FUNCTION gstk_geo_loc_cnf

DESCRIPTION
  This function packs the geographical location request terminal response and sends
  the command to UIM server.

PARAMETERS
  geo_loc_cnf_ptr: [Input] Pointer to message that is required to
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
gstk_status_enum_type gstk_geo_loc_cnf (
  gstk_geo_loc_cnf_type* geo_loc_cnf_ptr
)
{
  gstk_status_enum_type                  gstk_status   = GSTK_SUCCESS;
  geo_loc_terminal_response_type         STK_response  = {0};
  uim_cmd_type                          *uim_cmd_ptr   = NULL;
  uint32                                 offset        = 0;
  uint32                                 ref_id        = 0;

  UIM_MSG_HIGH_1("IN GSTK Geo Loc cnf: geo_loc_cnf_ptr=0x%x",
                 geo_loc_cnf_ptr);

  if(geo_loc_cnf_ptr == NULL)
  {
    return GSTK_BAD_PARAM;
  }

  /* initialize STK_response */
  memset(&STK_response, 0x00, sizeof(geo_loc_terminal_response_type));

  /* Pack various TLVs */

  /* Command details */
  /* from the cached table */
  gstk_status = gstk_packer_cmd_details_tlv(
      geo_loc_cnf_ptr->cmd_details_ref_id,
      geo_loc_cnf_ptr->message_header.command_id,
      geo_loc_cnf_ptr->command_number,
      &STK_response.data.command_details );

  if (gstk_status != GSTK_SUCCESS) {
    gstk_geo_loc_cnf_cleanup(geo_loc_cnf_ptr, &STK_response);
    return gstk_status;
  }

  /* device ID */
  gstk_status = gstk_packer_dev_id_tlv(
      GSTK_ME_DEVICE,
      GSTK_UICC_SIM_DEVICE,
      &STK_response.data.device_id );

  if (gstk_status != GSTK_SUCCESS) {
    gstk_geo_loc_cnf_cleanup(geo_loc_cnf_ptr, &STK_response);
    return gstk_status;
  }

  /* result tag */
  gstk_status = gstk_packer_result_tlv(
      geo_loc_cnf_ptr->command_result,
      geo_loc_cnf_ptr->result_additional_info,
      &STK_response.data.result);

  if (gstk_status != GSTK_SUCCESS) {
    gstk_geo_loc_cnf_cleanup(geo_loc_cnf_ptr, &STK_response);
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
        ref_id = geo_loc_cnf_ptr->cmd_details_ref_id;

        /* clear the command detail table reference slot before since we are about to reply to
          the card */
        gstk_util_cmd_details_ref_table_free_slot(geo_loc_cnf_ptr->cmd_details_ref_id);

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
          gstk_status = GSTK_MEMORY_ERROR;
        }
        offset = offset + 3;
        /* has additional info */
        if((STK_response.data.result.result_tag_length > 1) &&
           (gstk_byte_offset_memcpy(uim_cmd_ptr->terminal_response.data,
                                    STK_response.data.result.additional_info,
                                    offset,
                                    (size_t)(STK_response.data.result.result_tag_length - 1),
                                    sizeof(uim_cmd_ptr->terminal_response.data),
                                    ((STK_response.data.result.result_tag_length - 1)*
                                     sizeof(byte))) <
            (size_t)(STK_response.data.result.result_tag_length - 1)))
        {
          gstk_status = GSTK_MEMORY_ERROR;
        }

        if (gstk_status == GSTK_SUCCESS)
        {
          GSTK_UTIL_SELECTIVE_DUMP_BYTE_ARRAY("Geographical location request TR",
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

  gstk_geo_loc_cnf_cleanup(geo_loc_cnf_ptr, &STK_response);
  return gstk_status;
} /* gstk_geo_loc_cnf */


/*===========================================================================
FUNCTION gstk_geo_loc_report_cleanup

DESCRIPTION
  This function cleans up any allocated pointer on the stack.

PARAMETERS
  geo_loc_report_cmd: [Input] Pointer to message that is required to
                              be processed and sent to UIM

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
static void gstk_geo_loc_report_cleanup(
    gstk_envelope_geo_loc_report_type*           geo_loc_report_cmd)
{
  gstk_util_release_upper_layer_cmd_memory((gstk_cmd_type*)((void*)geo_loc_report_cmd));
} /* gstk_geo_loc_report_cleanup */


/*===========================================================================
FUNCTION   gstk_geo_loc_report_ind

DESCRIPTION
  This function builds the envelope of Geographical location reporting

PARAMETERS
  geo_loc_report_cmd: [Input] The struct of Geographical location reporting

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
gstk_status_enum_type gstk_geo_loc_report_ind(
  gstk_envelope_geo_loc_report_type *geo_loc_report_cmd)
{
   gstk_status_enum_type                       gstk_status         = GSTK_SUCCESS;
   gstk_envelope_geo_loc_report_command_type   STK_envelope_cmd;
   uim_cmd_type                                *uim_cmd_ptr;
   uint8                                       offset              = 0;

   if(geo_loc_report_cmd == NULL)
   {
     UIM_MSG_ERR_0("Geo Loc ERR : NULL");
     return GSTK_BAD_PARAM;
   }

   /* initialize STK_envelope_cmd */
   memset(&STK_envelope_cmd, 0, sizeof(gstk_envelope_geo_loc_report_command_type));

   /* Pack various TLVs */
   STK_envelope_cmd.tag = GSTK_GEO_LOC_REPORT_TAG;

   /* device ID */
   gstk_status = gstk_packer_dev_id_tlv(
       GSTK_ME_DEVICE,
       GSTK_UICC_SIM_DEVICE,
       &STK_envelope_cmd.device_id );

   if (gstk_status != GSTK_SUCCESS) {
       gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
       return gstk_status;
   }

   if(geo_loc_report_cmd->nmea_sentence.length != 0)
   {
     /* nmea tag */
     gstk_status = gstk_packer_nmea_sentence_tlv(
           &geo_loc_report_cmd->nmea_sentence,
           &STK_envelope_cmd.nmea);

     if (gstk_status != GSTK_SUCCESS)
     {
         gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
         return gstk_status;
     }
   }

   /* Send command to STK application on SIM */
   /* Send Message to UIM */
   /* get a buffer for the request */
   uim_cmd_ptr = gstk_get_uim_buf();
   if (uim_cmd_ptr != NULL ) {
     gstk_status = gstk_util_populate_uim_buf_hdr_info(UIM_ENVELOPE_F, &uim_cmd_ptr);
     if ((gstk_status == GSTK_SUCCESS) && (uim_cmd_ptr != NULL)) {
         /* set user_data */
         uim_cmd_ptr->hdr.user_data               = geo_loc_report_cmd->message_header.user_data;

         /* initialized to 3 for value portion of the TLVs
            i.e., the final offset will be either 0 or 1 depends
            on whether the total length is > 0x7F or not */
         offset = 3;

         /* copy device */
         /* No Need to check STK_envelope_cmd.device_id.device_tag_length as is
            fixed length in packer function
         */
         if(gstk_byte_offset_memcpy(uim_cmd_ptr->envelope.data,
                                    &STK_envelope_cmd.device_id,
                                    offset,
                                    (size_t)(STK_envelope_cmd.device_id.device_tag_length +
                                             GSTK_TAG_LENGTH_LEN),
                                    sizeof(uim_cmd_ptr->envelope.data),
                                    sizeof(STK_envelope_cmd.device_id)) <
            (size_t)(STK_envelope_cmd.device_id.device_tag_length + GSTK_TAG_LENGTH_LEN))
         {
           UIM_MSG_ERR_0("gstk_geo_loc_report_ind(): gstk_byte_offset_memcpy failed");
           gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
           gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
           return GSTK_MEMORY_ERROR;
         }

	 offset = (int)(offset + STK_envelope_cmd.device_id.device_tag_length +
                        GSTK_TAG_LENGTH_LEN);

         /* total length: + device tlv */
         STK_envelope_cmd.length = STK_envelope_cmd.device_id.device_tag_length +
                                   GSTK_TAG_LENGTH_LEN;

         if(STK_envelope_cmd.nmea.length > 0)
         {
         if(offset > UIM_MAX_ENVELOPE_BYTES - 1)
         {
           UIM_MSG_ERR_0("Length of command exceeding UIM_MAX_ENVELOPE_BYTES");
           gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
           gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
           return GSTK_MEMORY_ERROR;
         }
         /* copy nmea */
         /* copy tag, length */
         uim_cmd_ptr->envelope.data[offset++] = STK_envelope_cmd.nmea.tag;
         if (STK_envelope_cmd.nmea.length > GSTK_TLV_LENGTH_1_BYTE_OFFSET_LIMIT) {
           if(offset > UIM_MAX_ENVELOPE_BYTES - 1)
           {
             UIM_MSG_ERR_0("Length of command exceeding UIM_MAX_ENVELOPE_BYTES");
             gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
             gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
             return GSTK_MEMORY_ERROR;
           }
           uim_cmd_ptr->envelope.data[offset++] = GSTK_2_BYTE_LENGTH_FIRST_VALUE;
           /* Adding up length of the command to make sure it will not overflow the uint8 */
           if((STK_envelope_cmd.length + 1 + STK_envelope_cmd.nmea.length + GSTK_TAG_LENGTH_LEN) >
              (0xFF - GSTK_TAG_LENGTH_LEN - 1))
           {
             UIM_MSG_ERR_0("Length of command exceeding 0xFF");
             gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
             gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
             return GSTK_INVALID_LENGTH;
           }
           STK_envelope_cmd.length++;
         }

         if(offset > UIM_MAX_ENVELOPE_BYTES - 1)
         {
           UIM_MSG_ERR_0("Length of command exceeding UIM_MAX_ENVELOPE_BYTES");
           gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
           gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
           return GSTK_MEMORY_ERROR;
         }
         uim_cmd_ptr->envelope.data[offset++] = STK_envelope_cmd.nmea.length;

         if (gstk_check_envelope_offset(uim_cmd_ptr, offset, STK_envelope_cmd.nmea.length) !=
                                                                   GSTK_SUCCESS){
           gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
           gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
           return GSTK_MEMORY_ERROR;
         }

         if(offset > UIM_MAX_ENVELOPE_BYTES - 1)
         {
           UIM_MSG_ERR_1("Length of command exceeding UIM_MAX_ENVELOPE_BYTES offset 0x%x",
                         offset);
           gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
           gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
           return GSTK_INVALID_LENGTH;
         }

         gstk_byte_offset_memcpy(uim_cmd_ptr->envelope.data,
                           STK_envelope_cmd.nmea.nmea,
                           offset,
                           STK_envelope_cmd.nmea.length,
                           sizeof(uim_cmd_ptr->envelope.data),
                           STK_envelope_cmd.nmea.length);

         offset = offset + STK_envelope_cmd.nmea.length;
         /* total length: + tpdu tlv */
         STK_envelope_cmd.length += STK_envelope_cmd.nmea.length + GSTK_TAG_LENGTH_LEN;
         }

         /* populate the envelope command info */
         if(STK_envelope_cmd.length > GSTK_TLV_LENGTH_1_BYTE_OFFSET_LIMIT) {
           /* account for 0x80 */
           uim_cmd_ptr->envelope.offset = 0;
           uim_cmd_ptr->envelope.data[0] = STK_envelope_cmd.tag;
           uim_cmd_ptr->envelope.data[1] = GSTK_2_BYTE_LENGTH_FIRST_VALUE;
           uim_cmd_ptr->envelope.data[2] = STK_envelope_cmd.length;
           STK_envelope_cmd.length++;
           uim_cmd_ptr->envelope.num_bytes = STK_envelope_cmd.length + GSTK_TAG_LENGTH_LEN;
         }
         else {
           uim_cmd_ptr->envelope.offset = 1;
           uim_cmd_ptr->envelope.data[1] = STK_envelope_cmd.tag;
           uim_cmd_ptr->envelope.data[2] = STK_envelope_cmd.length;
           uim_cmd_ptr->envelope.num_bytes = STK_envelope_cmd.length + GSTK_TAG_LENGTH_LEN;
         }

         GSTK_UTIL_SELECTIVE_DUMP_BYTE_ARRAY(
                  "Env: GEO LOC REPORT",
                  &uim_cmd_ptr->envelope.data[uim_cmd_ptr->envelope.offset],
                  uim_cmd_ptr->envelope.num_bytes);

         gstk_send_cmd_to_uim_server(uim_cmd_ptr); /* clear signal and call uim_cmd */
     }
   } /* uim_cmd_ptr != NULL */
   else {
     gstk_status = GSTK_MEMORY_ERROR;
   }

   gstk_geo_loc_report_cleanup(geo_loc_report_cmd);
   return gstk_status;
} /* gstk_geo_loc_report_ind */

