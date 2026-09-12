/*===========================================================================


              G S T K    T E R M I N A L    R E S P O N S E



GENERAL DESCRIPTION
  This source file contains the major terminal response handling


INITIALIZATION AND SEQUENCING REQUIREMENTS

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/uim.mpss/6.1.0/gstk/src/gstk_process_tr.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/06/19   ks      Clear User data before sending the TR report to client
10/07/16   shr     GSTK/ESTK F3 reduction
10/03/16   dd      Removed lint comments
09/23/16   gs      F3 reduction / cleanup
05/27/16   shr     F3 reduction: Malloc Failure, Memcpy, Null Ptr checks
04/29/16   gs      Initial Version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "gstk_terminal_rsp.h"
#include "gstkutil.h"
#include "uim_selective_logging.h"

/*===========================================================================
FUNCTION gstk_process_raw_terminal_response

DESCRIPTION
  This function packs raw terminal response and sends response to the card
  through the UIM server.

PARAMETERS
  raw_term_resp_ptr: [Input] Pointer to raw terminal response that is required
                             to be processed and sent to UIM

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
gstk_status_enum_type gstk_process_raw_terminal_response (
  gstk_client_raw_term_resp_type   *raw_term_resp_ptr )
{
  gstk_status_enum_type gstk_status     = GSTK_SUCCESS;
  uim_cmd_type          *uim_cmd_ptr    = NULL;
  uint32                ref_id          = 0x00;

  UIM_MSG_HIGH_0("IN gstk_send_raw_terminal_response");

  if (raw_term_resp_ptr == NULL)
  {
    return GSTK_BAD_PARAM;
  }

  if (raw_term_resp_ptr->raw_term_resp.data_len == 0)
  {
    return GSTK_BAD_PARAM;
  }

  if (raw_term_resp_ptr->raw_term_resp.data_buffer_ptr == NULL)
  {
    return GSTK_NULL_INPUT_PARAM;
  }

  GSTK_RETURN_ERROR_IF_NULL_PTR(gstk_curr_inst_ptr, GSTK_ERROR);
  uim_cmd_ptr = gstk_get_uim_buf();

  if(uim_cmd_ptr == NULL)
  {
    gstk_util_release_upper_layer_cmd_memory(
      (gstk_cmd_type*)((void*)raw_term_resp_ptr));
    return GSTK_MEMORY_ERROR;
  }

  ref_id = raw_term_resp_ptr->cmd_details_ref_id;

  if (command_details_ref_table_ptr[GSTK_CMD_REF_TABLE_INDEX(ref_id)].expected_cmd_rsp_type ==
      GSTK_SETUP_EVENT_LIST_CNF)
  {
    command_details_ref_table_ptr[GSTK_CMD_REF_TABLE_INDEX(ref_id)].pending_responses--;

    /* Save the info in client table for later to use to send SW1/SW2 of TR to the client */
    gstk_shared_data.gstk_client_table[raw_term_resp_ptr->client_id - 1].send_setup_event_sw1_sw2 = TRUE;
    gstk_shared_data.gstk_client_table[raw_term_resp_ptr->client_id - 1].user_data_updated = TRUE;
    gstk_shared_data.gstk_client_table[raw_term_resp_ptr->client_id - 1].user_data =
                                                  raw_term_resp_ptr->message_header.user_data;

    gstk_status = gstk_convert_raw_setup_evt_list_cnf(raw_term_resp_ptr);
    gstk_util_release_upper_layer_cmd_memory(
      (gstk_cmd_type*)((void*)raw_term_resp_ptr));
    gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
    return gstk_status;
  }

  gstk_status = gstk_util_populate_uim_buf_hdr_info(UIM_TERMINAL_RESPONSE_F,  &uim_cmd_ptr);
  if (gstk_status != GSTK_SUCCESS)
  {
    /* Release the upper cmd layer memory and the uim_cmd_ptr */
    gstk_util_release_upper_layer_cmd_memory(
      (gstk_cmd_type*)((void*)raw_term_resp_ptr));
    gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
    return gstk_status;
  }

  gstk_shared_data.gstk_client_table[raw_term_resp_ptr->client_id - 1].user_data_updated = TRUE;
  /* Store the user data supplied */
  gstk_shared_data.gstk_client_table[raw_term_resp_ptr->client_id -1].user_data =
    raw_term_resp_ptr->message_header.user_data;

  /* populate UIM command buffer */
  uim_cmd_ptr->hdr.user_data                = raw_term_resp_ptr->client_id;
  uim_cmd_ptr->terminal_response.num_bytes  =
   (uint8)raw_term_resp_ptr->raw_term_resp.data_len;
  if(gstk_memcpy(uim_cmd_ptr->terminal_response.data,
                 raw_term_resp_ptr->raw_term_resp.data_buffer_ptr,
                 (raw_term_resp_ptr->raw_term_resp.data_len),
                 sizeof(uim_cmd_ptr->terminal_response.data),
                 (raw_term_resp_ptr->raw_term_resp.data_len)) <
     (raw_term_resp_ptr->raw_term_resp.data_len))
  {
    /* Release the upper cmd layer memory and the uim_cmd_ptr */
    gstk_util_release_upper_layer_cmd_memory(
      (gstk_cmd_type*)((void*)raw_term_resp_ptr));
    gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
    return GSTK_MEMORY_ERROR;
  }

  /* print out data */
  GSTK_UTIL_SELECTIVE_DUMP_BYTE_ARRAY("raw_term_resp_ptr->raw_term_resp.data",
    raw_term_resp_ptr->raw_term_resp.data_buffer_ptr,
    (raw_term_resp_ptr->raw_term_resp.data_len));

  gstk_util_release_upper_layer_cmd_memory(
    (gstk_cmd_type*)((void*)raw_term_resp_ptr));

  /* clear the command detail table reference slot before since we are about to reply to
  the card */
  gstk_util_cmd_details_ref_table_free_slot(raw_term_resp_ptr->cmd_details_ref_id);

  gstk_send_cmd_to_uim_server(uim_cmd_ptr); /* clear signal and call uim_cmd */
  return gstk_status;
} /* gstk_process_raw_terminal_response */
