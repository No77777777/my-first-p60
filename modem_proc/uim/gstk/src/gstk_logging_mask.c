/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 G S T K  U I M   L O G G I N G   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains utility functions to support masking of private data
in toolkit commands and responses.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                        EDIT HISTORY FOR MODULE



when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/21/21   shp     changed memcpy to memscpy 
01/02/20   sch     Initial revision
==============================================================================*/

#include "uim_variation.h"
#include "gstk_exp.h"
#include "gstkutil.h"
#include "gstk_envelope_cmd.h"
#include "gstk_logging_mask.h"
#include <stringl/stringl.h>
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/* Type for holding log masking information for ENVELOPE and PROACTIVE cmd APDUs */
typedef struct
{
  uint8   cmd_tag;
  /* The function pointer to mask the request APDU */
  boolean (*req_masking_fn_ptr)(const uint8 *input_ptr, uint8 *output_ptr,
                                               uint32 len);
  /* The function pointer to mask the response APDU */
  boolean (*rsp_masking_fn_ptr)(const uint8 *input_ptr, uint8 *output_ptr,
                                               uint32 len);
}gstk_masking_type;


/* masking function declarations*/
static boolean gstk_log_masking_fn_envelope_req(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
);

static boolean gstk_log_masking_fn_call_control_rsp(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
);

static boolean gstk_log_masking_fn_event_download_req(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
);

static boolean gstk_log_masking_fn_all(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
);

/* Table for ENVELOPE masking fn ptrs */
const gstk_masking_type gstk_envelope_masking_tbl[] =
{
  {GSTK_MENU_SELECTION_TAG,  gstk_log_masking_fn_envelope_req,       NULL},
  {GSTK_CALL_CONTROL_TAG,    gstk_log_masking_fn_envelope_req,       gstk_log_masking_fn_call_control_rsp},
  {GSTK_EVENT_DOWNLOAD_TAG,  gstk_log_masking_fn_event_download_req, NULL},
  {GSTK_SMS_PP_DOWNLOAD_TAG, gstk_log_masking_fn_envelope_req,       gstk_log_masking_fn_all},
  {GSTK_GEO_LOC_REPORT_TAG,  gstk_log_masking_fn_envelope_req,       NULL}
};


/* ----------------------------------------------------------------------------
   FUNCTION:      GSTK_LOG_MASKING_FN_ENVELOPE_REQ

   DESCRIPTION
     This function masks the envelope cmd req APDUs of
     Menu Selection, Call Control, SMS-PP download, and GeoLocation Report

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
static boolean gstk_log_masking_fn_envelope_req(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
)
{
  uint32  offset = 0;

  if (input_ptr == NULL || output_ptr == NULL || length == 0)
  {
    return FALSE;
  }

  /* Mask everything but Tag, Length, and Device Identities - ETSI TS 102 223 (7) */
  offset = GSTK_PROACTIVE_CMD_LEN_OFFSET + gstk_find_length_of_length_value(&input_ptr[1])
        + sizeof(gstk_device_identities_tag_tlv_type);
		
  /* Avoid integer value wrap around during mem operation*/
  if(length < offset)
  {
	return FALSE;
  }
  
  /* leave envelope tag and length and device id unmasked */
  memscpy(output_ptr, length, input_ptr, offset);
  /* mask rest of command */
  memset(output_ptr + offset, 0xFF, length - offset);

  return TRUE;
}/* gstk_log_masking_fn_envelope_req */


/* ----------------------------------------------------------------------------
   FUNCTION:      GSTK_LOG_MASKING_FN_CALL_CONTROL_RSP

   DESCRIPTION
     This function masks the rsp APDU of Call Control

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
static boolean gstk_log_masking_fn_call_control_rsp(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
)
{
  uint32  offset = 0;

  if (input_ptr == NULL || output_ptr == NULL || length == 0)
  {
    return FALSE;
  }

  /* mask everything but first byte and length - ETSI TS 102 223 (7.3.1.6) */
  offset = GSTK_PROACTIVE_CMD_LEN_OFFSET + gstk_find_length_of_length_value(&input_ptr[1]);
  
  /* Avoid integer value wrap around during mem operation */
  if(length < offset)
  {
	return FALSE;
  }
  
  /* leave result and length unmasked */
  memscpy(output_ptr, length, input_ptr, offset);
  /* mask rest of response */
  memset(output_ptr + offset, 0xFF, length - offset);

  return TRUE;
}/* gstk_log_masking_fn_call_control_rsp */


/* ----------------------------------------------------------------------------
   FUNCTION:      GSTK_LOG_MASKING_FN_EVENT_DOWNLOAD_REQ

   DESCRIPTION
     This function masks the req APDU of Event Download

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
static boolean gstk_log_masking_fn_event_download_req(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
)
{
  uint32  offset = 0;

  if (input_ptr == NULL || output_ptr == NULL || length == 0)
  {
    return FALSE;
  }

  /* Mask everything but Tag, Length, Event List, and Device Identities - ETSI TS 102 223 (7.5) */
  offset = GSTK_PROACTIVE_CMD_LEN_OFFSET + gstk_find_length_of_length_value(&input_ptr[1])
            + GSTK_ENVELOPE_EVENT_LIST_LEN + sizeof(gstk_device_identities_tag_tlv_type);
			
  /* Avoid integer value wrap around during mem operation */
  if(length < offset)
  {
	return FALSE;
  }
  
  /* leave envelope tag and length and event list and device id unmasked */
  memscpy(output_ptr, length, input_ptr, offset);
  /* mask rest of command */
  memset(output_ptr + offset, 0xFF, length - offset);

  return TRUE;
}/* gstk_log_masking_fn_event_download_req */


/* ----------------------------------------------------------------------------
   FUNCTION:      GSTK_LOG_MASKING_FN_ALL

   DESCRIPTION
     This function masks the entire APDU

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
static boolean gstk_log_masking_fn_all(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
)
{
  if (input_ptr == NULL || output_ptr == NULL || length == 0)
  {
    return FALSE;
  }

  /* mask entire response */
  memset(output_ptr, 0xFF, length);

  return TRUE;
}/* gstk_log_masking_fn_all */


/* ----------------------------------------------------------------------------
   FUNCTION:      GSTK_LOG_MASKING_FN_TERMINAL_RESPONSE

   DESCRIPTION
     This function masks the terminal response APDU

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
static boolean gstk_log_masking_fn_terminal_response(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
)
{
  uint32            masking_offset        = 0;
  uint32            result_length_offset  = 0;
  uint32            length_tlv_len        = 0;
  uint32            additional_info_len   = 0;

  if (input_ptr == NULL || output_ptr == NULL || length == 0)
  {
    return FALSE;
  }

  /* Mask everything but Command Details, Device Identities and Result - ETSI TS 102 223 (6.8) */
  /* result_tlv has variable length so determine the length */
  result_length_offset = sizeof(gstk_command_details_tlv_type) + sizeof(gstk_device_identities_tag_tlv_type) + 1;
  length_tlv_len = gstk_find_length_of_length_value(&input_ptr[result_length_offset]);
  if (length_tlv_len == 1)
  {
    additional_info_len = input_ptr[result_length_offset];
  }
  else if (length_tlv_len == 2)
  {
    additional_info_len = input_ptr[result_length_offset+1];
  }

  masking_offset = result_length_offset + length_tlv_len + additional_info_len;
  
  /* Avoid integer value wrap around during mem operation */
  if(length < masking_offset)
  {
	return FALSE;
  }
  
  /* leave cmd details and device ids and result unmasked */
  memscpy(output_ptr, length, input_ptr, masking_offset);
  /* mask rest of command */
  memset(output_ptr + masking_offset, 0xFF, length - masking_offset);

  return TRUE;
} /* gstk_log_masking_fn_terminal_response */


/* ----------------------------------------------------------------------------
   FUNCTION:      GSTK_LOG_MASKING_FN_PROACTIVE_CMD

   DESCRIPTION
     This function masks the proactive cmd APDU based on cmd type

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     FALSE if invalid input
     TRUE if output APDU is set

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
boolean gstk_log_masking_fn_proactive_cmd(
   const uint8         *input_ptr,
   uint8               *output_ptr,
   uint32               length
)
{
  uint8                           cmd_type            = 0;
  uint32                          offset              = 0;
  uint32                          cmd_details_offset  = 0;
  gstk_command_details_tlv_type   cmd_details;

  if (input_ptr == NULL || output_ptr == NULL || length == 0)
  {
    return FALSE;
  }

  /* Get the command_details struct in header and parse the command type */
  cmd_details_offset = GSTK_PROACTIVE_CMD_LEN_OFFSET + gstk_find_length_of_length_value(&input_ptr[1]);
  memset(&cmd_details, 0, sizeof(gstk_command_details_tlv_type));
  memscpy(&cmd_details, sizeof(gstk_command_details_tlv_type), input_ptr + cmd_details_offset, sizeof(gstk_command_details_tlv_type));
  cmd_type = cmd_details.type_of_command;

  switch(cmd_type)
  {
    case GSTK_CMD_STK_DISPLAY_TEXT:
    case GSTK_CMD_STK_GET_INKEY:
    case GSTK_CMD_STK_GET_INPUT:
    case GSTK_CMD_STK_SET_UP_MENU:
    case GSTK_CMD_STK_SELECT_ITEM:
    case GSTK_CMD_STK_SEND_SHORT_MESSAGE:
    case GSTK_CMD_STK_SET_UP_CALL:
    case GSTK_CMD_STK_PERFORM_CARD_APDU:
    case GSTK_CMD_STK_SET_UP_IDLE_MODE_TEXT:
    case GSTK_CMD_STK_RUN_AT_COMMAND:
    case GSTK_CMD_STK_SEND_DTMF:
    case GSTK_CMD_STK_LAUNCH_BROWSER:
    case GSTK_CMD_STK_OPEN_CHANNEL:
    case GSTK_CMD_STK_SEND_DATA:
      /* Mask everything but Tag, length, Command Details, and Device Identities - ETSI TS 102 223 (6.6) */
      offset = GSTK_PROACTIVE_CMD_LEN_OFFSET + gstk_find_length_of_length_value(&input_ptr[1])
              + sizeof(gstk_command_details_tlv_type) + sizeof(gstk_device_identities_tag_tlv_type);
			  
	  /* Avoid integer value wrap around during mem operation */
	  if(length < offset)
      {
	    return FALSE;
      }
	  
      /* leave proactive tag and length and cmd details and device ids unmasked */
      memscpy(output_ptr, length, input_ptr, offset);
      /* mask rest of command */
      memset(output_ptr + offset, 0xFF, length - offset);
      break;
    default:
      /* Other commands do not need log masking */
      memscpy(output_ptr, length, input_ptr, length);
      break;
  }

  return TRUE;
} /* gstk_log_masking_fn_proactive_cmd */


/* ----------------------------------------------------------------------------
   FUNCTION:      GSTK_LOGGING_POPULATE_MASKING_FN_PTR

   DESCRIPTION
     This function populates the appropriate function pointer based on the
     command. APDU logging occurs only when using a test imsi, or the efs item
     private_logging_enabled is set. Otherwise this function will mask APDUs.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
void gstk_logging_populate_masking_fn_ptr(
   uim_cmd_type          *uim_cmd_ptr
)
{
   uint8                          i                = 0;
   uint8                          table_size       = 0;
   gstk_status_enum_type          gstk_status      = GSTK_ERROR;
   mmgsdi_slot_id_enum_type       slot             = MMGSDI_MAX_SLOT_ID_ENUM;
   uim_cmd_name_type              cmd_name_type;
   uint8                          envelope_cmd_tag;
   gstk_command_details_tlv_type  cmd_details;

   if (uim_cmd_ptr == NULL)
   {
      return;
   }

   gstk_status = gstk_util_convert_from_uim_slot_to_mmgsdi_slot(uim_cmd_ptr->hdr.slot, &slot);
   if (gstk_status != GSTK_SUCCESS)
   {
     return;
   }

   /* If it is a test card or private logging EFS item is set, no need to populate the function pointers,
     because there is no need to mask anything */
   if(mmgsdi_is_private_logging_enabled_for_slot(slot))
   {
     return;
   }

   cmd_name_type = uim_cmd_ptr->hdr.command;
   envelope_cmd_tag = uim_cmd_ptr->envelope.data[uim_cmd_ptr->envelope.offset];

   if(cmd_name_type == UIM_ENVELOPE_F)
   {
     /* set masking fn ptrs for envelope commands */
     table_size = sizeof(gstk_envelope_masking_tbl)/sizeof(gstk_masking_type);
     for (i = 0; i < table_size; i++)
     {
       if (envelope_cmd_tag == gstk_envelope_masking_tbl[i].cmd_tag)
       {
         uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = gstk_envelope_masking_tbl[i].req_masking_fn_ptr;
         uim_cmd_ptr->hdr.test_mode_rsp_log_masking_fn_ptr = gstk_envelope_masking_tbl[i].rsp_masking_fn_ptr;
       }
     }
   }
   else if(cmd_name_type == UIM_TERMINAL_RESPONSE_F)
   {
    /* Extract command type from command details - ETSI TS 102 223 (6.8) */
    memset(&cmd_details, 0, sizeof(gstk_command_details_tlv_type));
    memscpy(&cmd_details, sizeof(gstk_command_details_tlv_type), uim_cmd_ptr->terminal_response.data, sizeof(gstk_command_details_tlv_type));
      switch(cmd_details.type_of_command)
      {
        case GSTK_CMD_STK_GET_INKEY:
        case GSTK_CMD_STK_GET_INPUT:
        case GSTK_CMD_STK_PROVIDE_LOCAL_INFO:
        case GSTK_CMD_STK_PERFORM_CARD_APDU:
        case GSTK_CMD_STK_RUN_AT_COMMAND:
        case GSTK_CMD_STK_RECEIVE_DATA:
          uim_cmd_ptr->hdr.test_mode_req_log_masking_fn_ptr = gstk_log_masking_fn_terminal_response;
          break;
        default:
          /* Other commands do not require masking */
          break;
      }
   }
} /* gstk_logging_populate_masking_fn_ptr */
