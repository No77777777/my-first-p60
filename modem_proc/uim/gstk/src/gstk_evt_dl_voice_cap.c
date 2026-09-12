/*===========================================================================


            G S T K   E V T  D L  T E R M I N A L  C A P

GENERAL DESCRIPTION
  This source file contains functions to handle Event Downloads based on the 
  Voice Capabilities

INITIALIZATION AND SEQUENCING REQUIREMENTS

===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016 - 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


$Header: //components/rel/uim.mpss/6.1.0/gstk/src/gstk_evt_dl_voice_cap.c#2 $

when         who     what, where, why
--------   ---     -------------------------------------------------------------------- 
09/16/19   dt      Add memory freeing for cause and transaction id
10/08/18   gs      Fix the offset for Media Type tag in Call Disconnected ENVELOPE  
10/24/16   gs      Changes in TP based on RAT and App Type
10/07/16   shr     GSTK/ESTK F3 reduction
09/23/16   gs      F3 reduction / cleanup
09/15/16   gs      Initial Version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "gstk_envelope_cmd.h"
#include "gstk_envelope_rsp.h"
#include "uim.h"
#include "gstkutil.h"
#include "uim_selective_logging.h"

/*===========================================================================
FUNCTION gstk_mt_call_evt_ind

DESCRIPTION

  This function builds the mt call command and sends it to UIM

PARAMETERS
  mt_evt_cmd: [Input] Pointer to the envelope command to be processed

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
gstk_status_enum_type gstk_mt_call_evt_ind(
        gstk_envelope_mt_evt_ind_type    * mt_evt_cmd)
{
   gstk_status_enum_type                       gstk_status = GSTK_SUCCESS;
   gstk_envelope_mt_evt_dl_command_type        STK_envelope_cmd;
   uim_cmd_type                                *uim_cmd_ptr;
   int                                         offset = 0;

   if(mt_evt_cmd == NULL)
   {
     return GSTK_BAD_PARAM;
   }

   /* initialize STK_envelope_cmd */
   memset(&STK_envelope_cmd, 0, sizeof(gstk_envelope_mt_evt_dl_command_type));

   /* Pack various TLVs */
   STK_envelope_cmd.event_download_hdr.tag = GSTK_EVENT_DOWNLOAD_TAG;


   /* transaction id tag */
   if(mt_evt_cmd->mt_evt_data.transaction_id.length > 0)
   {
     STK_envelope_cmd.transacton_id.transaction_list =
       GSTK_CALLOC(mt_evt_cmd->mt_evt_data.transaction_id.length);
     if(STK_envelope_cmd.transacton_id.transaction_list == NULL)
     {
       return GSTK_MEMORY_ERROR;
     }
   }
   gstk_status = gstk_packer_transaction_id_tlv(
       &mt_evt_cmd->mt_evt_data.transaction_id,
       &STK_envelope_cmd.transacton_id,
       GSTK_MT_CALL,
       mt_evt_cmd->call_evt_info);

   if (gstk_status != GSTK_SUCCESS) {
       GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list);
       return gstk_status;
   }

   /* event list */
   gstk_status = gstk_packer_evt_list_tlv(
       GSTK_MT_CALL,
       &STK_envelope_cmd.event_download_hdr.evt_list );

   if (gstk_status != GSTK_SUCCESS) {
      return gstk_status;
   }

   /* device ID */
   gstk_status = gstk_packer_dev_id_tlv(
       GSTK_NETWORK_DEVICE,
       GSTK_UICC_SIM_DEVICE,
       &STK_envelope_cmd.event_download_hdr.device_id );


   if (gstk_status != GSTK_SUCCESS) {
       return gstk_status;
   }

   /* address tag */
   if(mt_evt_cmd->mt_evt_data.address.length != 0) {
     gstk_status = gstk_packer_address_tlv(GSTK_ADDRESS_TAG,
                             &mt_evt_cmd->mt_evt_data.address,
                             &STK_envelope_cmd.address);
     if(gstk_status != GSTK_SUCCESS) {
       return gstk_status;
     }
   }
   if(mt_evt_cmd->mt_evt_data.subaddress.length != 0) {
     gstk_status = gstk_packer_subaddress_tlv(
          &mt_evt_cmd->mt_evt_data.subaddress,
          &STK_envelope_cmd.subaddress);
     if (gstk_status != GSTK_SUCCESS) {
         return gstk_status;
     }
   }

   /* total data length:  evt id data + 2
                        + device id data + 2 (2 is for the tag and length field)
                        + transaction id data + 2
                        + address data + 2
                        + subaddress data + 2
   */
   STK_envelope_cmd.event_download_hdr.length =
                             STK_envelope_cmd.event_download_hdr.evt_list.length +
                             STK_envelope_cmd.event_download_hdr.device_id.device_tag_length +
                             STK_envelope_cmd.transacton_id.length +
                             6;
   if(mt_evt_cmd->mt_evt_data.address.length != 0) {
     STK_envelope_cmd.event_download_hdr.length = STK_envelope_cmd.event_download_hdr.length +
                               STK_envelope_cmd.address.length +
                               GSTK_TAG_LENGTH_LEN;
   }
   if(mt_evt_cmd->mt_evt_data.subaddress.length != 0) {
     STK_envelope_cmd.event_download_hdr.length = STK_envelope_cmd.event_download_hdr.length +
                               STK_envelope_cmd.subaddress.length +
                               GSTK_TAG_LENGTH_LEN;
   }

   /* Send command to STK application on SIM */
   /* Send Message to UIM */
   /* get a buffer for the request */
   uim_cmd_ptr = gstk_get_uim_buf();
   if (uim_cmd_ptr != NULL ) {
     gstk_status = gstk_util_populate_uim_buf_hdr_info(UIM_ENVELOPE_F, &uim_cmd_ptr);
     if ((gstk_status == GSTK_SUCCESS) && (uim_cmd_ptr != NULL)) {
       /* set to 0 so that uim will get the data from index 0 */
       uim_cmd_ptr->envelope.offset = 0x00;

       /* set user_data */
       uim_cmd_ptr->hdr.user_data               = mt_evt_cmd->message_header.user_data;

       /* populate the envelope command info */
       uim_cmd_ptr->envelope.num_bytes = STK_envelope_cmd.event_download_hdr.length
                                           + GSTK_TAG_LENGTH_LEN;

       uim_cmd_ptr->envelope.data[offset++] = STK_envelope_cmd.event_download_hdr.tag;
       uim_cmd_ptr->envelope.data[offset++] = STK_envelope_cmd.event_download_hdr.length;

       /* copy evt list */
       if(gstk_byte_offset_memcpy(uim_cmd_ptr->envelope.data,
                                  &(STK_envelope_cmd.event_download_hdr.evt_list),
                                  offset,
                                  (size_t)(STK_envelope_cmd.event_download_hdr.evt_list.length + 2),
                                  sizeof(uim_cmd_ptr->envelope.data),
                                  sizeof(STK_envelope_cmd.event_download_hdr.evt_list)) <
          (size_t)(STK_envelope_cmd.event_download_hdr.evt_list.length + 2))
       {
         gstk_status = GSTK_MEMORY_ERROR;
       }
       /* copy device */
       else if(gstk_byte_offset_memcpy(uim_cmd_ptr->envelope.data,
                                       &(STK_envelope_cmd.event_download_hdr.device_id),
                                       offset +=
                                       (int)(STK_envelope_cmd.event_download_hdr.evt_list.length + 2),
                                       (size_t)(STK_envelope_cmd.event_download_hdr.device_id.device_tag_length
                                                + GSTK_TAG_LENGTH_LEN),
                                       sizeof(uim_cmd_ptr->envelope.data),
                                       sizeof(STK_envelope_cmd.event_download_hdr.device_id)) <
               (size_t)(STK_envelope_cmd.event_download_hdr.device_id.device_tag_length
                        + GSTK_TAG_LENGTH_LEN))
       {
         gstk_status = GSTK_MEMORY_ERROR;
       }
       /* copy transaction id */
       /* tag, length */
       else if(gstk_byte_offset_memcpy(uim_cmd_ptr->envelope.data,
                                       &(STK_envelope_cmd.transacton_id),
                                       offset +=
                                       (int)(STK_envelope_cmd.event_download_hdr.device_id.device_tag_length +
                                             GSTK_TAG_LENGTH_LEN),
                                       GSTK_TAG_LENGTH_LEN,
                                       sizeof(uim_cmd_ptr->envelope.data),
                                       sizeof(STK_envelope_cmd.transacton_id)) < GSTK_TAG_LENGTH_LEN)
       {
         gstk_status = GSTK_MEMORY_ERROR;
       }

       offset = (int)(offset + GSTK_TAG_LENGTH_LEN);
       if(gstk_status == GSTK_SUCCESS &&
          STK_envelope_cmd.transacton_id.length != 0 &&
          (STK_envelope_cmd.transacton_id.transaction_list != NULL) &&
          ((size_t)offset <= (sizeof(uim_cmd_ptr->envelope.data) - STK_envelope_cmd.transacton_id.length)))
       {
         if(memscpy(&(uim_cmd_ptr->envelope.data[offset]),
                    sizeof(uim_cmd_ptr->envelope.data)-offset,
                    STK_envelope_cmd.transacton_id.transaction_list,
                    (size_t)STK_envelope_cmd.transacton_id.length) <
            (size_t)STK_envelope_cmd.transacton_id.length)
         {
           gstk_status = GSTK_MEMORY_ERROR;
         }
         offset = (int)(offset + STK_envelope_cmd.transacton_id.length);
       }

       /* copy address */
       if(gstk_status == GSTK_SUCCESS &&
          STK_envelope_cmd.address.length != 0 &&
          (offset + STK_envelope_cmd.address.length + GSTK_TAG_LENGTH_LEN - 1)
            < UIM_MAX_ENVELOPE_BYTES) { /* has address */
          if(memscpy(&(uim_cmd_ptr->envelope.data[offset]),
                     sizeof(uim_cmd_ptr->envelope.data)-offset,
                     &STK_envelope_cmd.address,
                     (size_t)(STK_envelope_cmd.address.length + GSTK_TAG_LENGTH_LEN)) <
             (size_t)(STK_envelope_cmd.address.length + GSTK_TAG_LENGTH_LEN))
          {
            gstk_status = GSTK_MEMORY_ERROR;
          }
         offset = (int)(offset + STK_envelope_cmd.address.length + GSTK_TAG_LENGTH_LEN);
       }

       /* copy subaddress */
       if(gstk_status == GSTK_SUCCESS &&
          STK_envelope_cmd.subaddress.length != 0 &&
          (offset + STK_envelope_cmd.subaddress.length + GSTK_TAG_LENGTH_LEN - 1)
            < UIM_MAX_ENVELOPE_BYTES) { /* has subaddress */
          if(memscpy(&(uim_cmd_ptr->envelope.data[offset]),
                     sizeof(uim_cmd_ptr->envelope.data)-offset,
                     &STK_envelope_cmd.subaddress,
                     (size_t)(STK_envelope_cmd.subaddress.length + GSTK_TAG_LENGTH_LEN)) <
             (size_t)(STK_envelope_cmd.subaddress.length + GSTK_TAG_LENGTH_LEN))
          {
            gstk_status = GSTK_MEMORY_ERROR;
          }
         offset = (int)(offset + STK_envelope_cmd.subaddress.length + GSTK_TAG_LENGTH_LEN);
       }
       if (gstk_status == GSTK_SUCCESS)
       {
       GSTK_UTIL_SELECTIVE_DUMP_BYTE_ARRAY("MT evt",
                                           &uim_cmd_ptr->envelope.data[uim_cmd_ptr->envelope.offset],
                                           uim_cmd_ptr->envelope.num_bytes);
       }

       GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list);

       if (gstk_status == GSTK_SUCCESS)
       {
         gstk_send_cmd_to_uim_server(uim_cmd_ptr); /* clear signal and call uim_cmd */
       }
       else
       {
         gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
         uim_cmd_ptr = NULL;
       }
     }
   } /* uim_cmd_ptr != NULL */
   else {
     gstk_status = GSTK_MEMORY_ERROR;
   }

   return gstk_status;

} /* gstk_mt_call_evt_ind*/


/*===========================================================================
FUNCTION gstk_call_connected_evt_ind

DESCRIPTION

  This function builds the call connected envelope command and sends it to UIM

PARAMETERS
  call_connected_evt_cmd: [Input] Pointer to the envelope command to be
                                  processed

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
gstk_status_enum_type gstk_call_connected_evt_ind(
        gstk_envelope_call_connected_evt_ind_type    * call_connected_evt_cmd)
{
   gstk_status_enum_type                                gstk_status = GSTK_SUCCESS;
   gstk_envelope_call_connected_evt_dl_command_type     STK_envelope_cmd;
   uim_cmd_type                                         *uim_cmd_ptr;
   int                                                  offset = 0;

   if(call_connected_evt_cmd == NULL)
   {
     return GSTK_BAD_PARAM;
   }

   /* initialize STK_envelope_cmd */
   memset(&STK_envelope_cmd, 0, sizeof(gstk_envelope_call_connected_evt_dl_command_type));

   /* Pack various TLVs */
   STK_envelope_cmd.event_download_hdr.tag = GSTK_EVENT_DOWNLOAD_TAG;


   /* transaction id tag */
   if(call_connected_evt_cmd->transaction_id.length > 0)
   {
     STK_envelope_cmd.transacton_id.transaction_list =
       GSTK_CALLOC(call_connected_evt_cmd->transaction_id.length);
     if(STK_envelope_cmd.transacton_id.transaction_list == NULL)
     {
       return GSTK_MEMORY_ERROR;
     }
   }
   gstk_status = gstk_packer_transaction_id_tlv(
       &call_connected_evt_cmd->transaction_id,
       &STK_envelope_cmd.transacton_id,
       GSTK_CALL_CONNECTED,
       call_connected_evt_cmd->call_evt_info);

   if (gstk_status != GSTK_SUCCESS) {
       GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list); 
       return gstk_status;
   }

   /* event list */
   gstk_status = gstk_packer_evt_list_tlv(
       GSTK_CALL_CONNECTED,
       &STK_envelope_cmd.event_download_hdr.evt_list );

   if (gstk_status != GSTK_SUCCESS) {
     GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list); 
      return gstk_status;
   }

   /* device ID */
   if(call_connected_evt_cmd->near_end) {
     gstk_status = gstk_packer_dev_id_tlv(
         GSTK_ME_DEVICE,
         GSTK_UICC_SIM_DEVICE,
         &STK_envelope_cmd.event_download_hdr.device_id );
   }
   else {
     gstk_status = gstk_packer_dev_id_tlv(
         GSTK_NETWORK_DEVICE,
         GSTK_UICC_SIM_DEVICE,
         &STK_envelope_cmd.event_download_hdr.device_id );
   }

   if (gstk_status != GSTK_SUCCESS) {
     GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list);
     return gstk_status;
   }

   /* total data length:  evt id data + 2
                        + device id data + 2 (2 is for the tag and length field)
                        + transaction id data + 2
   */
   STK_envelope_cmd.event_download_hdr.length =
                             STK_envelope_cmd.event_download_hdr.evt_list.length +
                             STK_envelope_cmd.event_download_hdr.device_id.device_tag_length +
                             STK_envelope_cmd.transacton_id.length +
                             6;

   /* Send command to STK application on SIM */
   /* Send Message to UIM */
   /* get a buffer for the request */
   uim_cmd_ptr = gstk_get_uim_buf();
   if (uim_cmd_ptr != NULL ) {
     gstk_status = gstk_util_populate_uim_buf_hdr_info(UIM_ENVELOPE_F, &uim_cmd_ptr);
     if ((gstk_status == GSTK_SUCCESS) && (uim_cmd_ptr != NULL)) {
       /* set to 0 so that uim will get the data from index 0 */
       uim_cmd_ptr->envelope.offset = 0x00;

       /* set user_data */
       uim_cmd_ptr->hdr.user_data               = call_connected_evt_cmd->message_header.user_data;

       /* populate the envelope command info */
       uim_cmd_ptr->envelope.num_bytes = STK_envelope_cmd.event_download_hdr.length + GSTK_TAG_LENGTH_LEN;

       uim_cmd_ptr->envelope.data[offset++] = STK_envelope_cmd.event_download_hdr.tag;
       uim_cmd_ptr->envelope.data[offset++] = STK_envelope_cmd.event_download_hdr.length;

       /* copy evt list */
       if(memscpy(&(uim_cmd_ptr->envelope.data[offset]),
                  sizeof(uim_cmd_ptr->envelope.data)-offset,
                  &STK_envelope_cmd.event_download_hdr.evt_list,
                  (size_t)(STK_envelope_cmd.event_download_hdr.evt_list.length
                    + GSTK_TAG_LENGTH_LEN)) <
          (size_t)(STK_envelope_cmd.event_download_hdr.evt_list.length + GSTK_TAG_LENGTH_LEN))
       {
         gstk_status = GSTK_MEMORY_ERROR;
       }
       /* copy device */
       else if(gstk_byte_offset_memcpy(uim_cmd_ptr->envelope.data,
                                       &(STK_envelope_cmd.event_download_hdr.device_id),
                                       offset +=
                                       (int)(STK_envelope_cmd.event_download_hdr.evt_list.length +
                                             GSTK_TAG_LENGTH_LEN),
                                       (size_t)(STK_envelope_cmd.event_download_hdr.device_id.device_tag_length +
                                                GSTK_TAG_LENGTH_LEN),
                                       sizeof(uim_cmd_ptr->envelope.data),
                                       sizeof(STK_envelope_cmd.event_download_hdr.device_id)) <
               (size_t)(STK_envelope_cmd.event_download_hdr.device_id.device_tag_length +
                        GSTK_TAG_LENGTH_LEN))
       {
         gstk_status = GSTK_MEMORY_ERROR;
       }
       /* copy transaction id */
       /* tag, length */
       else if(gstk_byte_offset_memcpy(uim_cmd_ptr->envelope.data,
                                       &(STK_envelope_cmd.transacton_id),
                                       offset +=
                                       (int)(STK_envelope_cmd.event_download_hdr.device_id.device_tag_length +
                                             GSTK_TAG_LENGTH_LEN),
                                       GSTK_TAG_LENGTH_LEN,
                                       sizeof(uim_cmd_ptr->envelope.data),
                                       sizeof(STK_envelope_cmd.transacton_id)) < GSTK_TAG_LENGTH_LEN)
       {
         gstk_status = GSTK_MEMORY_ERROR;
       }
       offset = (int)(offset + GSTK_TAG_LENGTH_LEN);
       if(gstk_status == GSTK_SUCCESS &&
          STK_envelope_cmd.transacton_id.length != 0
          &&
          (STK_envelope_cmd.transacton_id.transaction_list != NULL)
          &&
          ((size_t)offset <= (sizeof(uim_cmd_ptr->envelope.data)
                      - STK_envelope_cmd.transacton_id.length)))
       {
         if(memscpy(&(uim_cmd_ptr->envelope.data[offset]),
                    sizeof(uim_cmd_ptr->envelope.data)-offset,
                    STK_envelope_cmd.transacton_id.transaction_list,
                    (size_t)STK_envelope_cmd.transacton_id.length) <
            (size_t)STK_envelope_cmd.transacton_id.length)
         {
           gstk_status = GSTK_MEMORY_ERROR;
         }
         offset = (int)(offset + STK_envelope_cmd.transacton_id.length);
       }

       if (gstk_status == GSTK_SUCCESS)
       {
         GSTK_UTIL_SELECTIVE_DUMP_BYTE_ARRAY("Call connected evt",
                                             &uim_cmd_ptr->envelope.data[uim_cmd_ptr->envelope.offset],
                                             uim_cmd_ptr->envelope.num_bytes);
       }

       if (gstk_status == GSTK_SUCCESS)
       {
         gstk_send_cmd_to_uim_server(uim_cmd_ptr); /* clear signal and call uim_cmd */
       }
       else
       {
         gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
         uim_cmd_ptr = NULL;
       }
     }
   } /* uim_cmd_ptr != NULL */
   else {
     gstk_status = GSTK_MEMORY_ERROR;
   }

   GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list); 
   return gstk_status;

} /* gstk_call_connected_evt_ind*/

/*===========================================================================
FUNCTION gstk_call_disconnected_evt_ind

DESCRIPTION

  This function builds the call disconnected envelope command and sends it
  to UIM

PARAMETERS
  call_connected_evt_cmd: [Input] Pointer to the envelope command to be
                                  processed

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
gstk_status_enum_type gstk_call_disconnected_evt_ind(
        gstk_envelope_call_disconnected_evt_ind_type    * call_disconnected_evt_cmd)
{
   gstk_status_enum_type                                   gstk_status = GSTK_SUCCESS;
   gstk_envelope_call_disconnected_evt_dl_command_type     STK_envelope_cmd;
   uim_cmd_type                                            *uim_cmd_ptr;
   int                                                     offset = 0;

   if(call_disconnected_evt_cmd == NULL)
   {
     return GSTK_BAD_PARAM;
   }

   /* initialize STK_envelope_cmd */
   memset(&STK_envelope_cmd, 0, sizeof(gstk_envelope_call_disconnected_evt_dl_command_type));

   /* Pack various TLVs */
   STK_envelope_cmd.event_download_hdr.tag = GSTK_EVENT_DOWNLOAD_TAG;

   /* transaction id tag */
   if(call_disconnected_evt_cmd->call_disconnected_data.transaction_id.length > 0)
   {
     STK_envelope_cmd.transacton_id.transaction_list =
       GSTK_CALLOC(
                  call_disconnected_evt_cmd->call_disconnected_data.transaction_id.length);
     if(STK_envelope_cmd.transacton_id.transaction_list == NULL)
     {
       return GSTK_MEMORY_ERROR;
     }
   }
   gstk_status = gstk_packer_transaction_id_tlv(
       &call_disconnected_evt_cmd->call_disconnected_data.transaction_id,
       &STK_envelope_cmd.transacton_id,
       GSTK_CALL_DISCONNECTED,
       call_disconnected_evt_cmd->call_evt_info);

   if (gstk_status != GSTK_SUCCESS) {
     GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list);
       return gstk_status;
   }

   /* event list */
   gstk_status = gstk_packer_evt_list_tlv(
       GSTK_CALL_DISCONNECTED,
       &STK_envelope_cmd.event_download_hdr.evt_list );

   if (gstk_status != GSTK_SUCCESS) 
   {
     GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list); 
     return gstk_status;
   }

   /* device ID */
   if(call_disconnected_evt_cmd->near_end) {
     gstk_status = gstk_packer_dev_id_tlv(
         GSTK_ME_DEVICE,
         GSTK_UICC_SIM_DEVICE,
         &STK_envelope_cmd.event_download_hdr.device_id );
   }
   else {
     gstk_status = gstk_packer_dev_id_tlv(
         GSTK_NETWORK_DEVICE,
         GSTK_UICC_SIM_DEVICE,
         &STK_envelope_cmd.event_download_hdr.device_id );
   }

   if (gstk_status != GSTK_SUCCESS) 
   {
     GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list); 
     return gstk_status;
   }

   /* cause tag */
   if(call_disconnected_evt_cmd->call_disconnected_data.cause.present == TRUE)
   {
     UIM_MSG_HIGH_0("Call Disconnected Cause present ");
     if(call_disconnected_evt_cmd->call_disconnected_data.cause.length > 0)
     {
       STK_envelope_cmd.cause.cause =
         GSTK_CALLOC(call_disconnected_evt_cmd->call_disconnected_data.cause.length);
       if(STK_envelope_cmd.cause.cause == NULL)
       {
         GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list);
         return GSTK_MEMORY_ERROR;
       }
     }
     gstk_status = gstk_packer_cause_tlv(
         &call_disconnected_evt_cmd->call_disconnected_data.cause,
         &STK_envelope_cmd.cause);
     }

   if (gstk_status != GSTK_SUCCESS)
   {
     GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list);
     GSTK_FREE(STK_envelope_cmd.cause.cause);
     return gstk_status;
   }


   /* Send command to STK application on SIM */
   /* Send Message to UIM */
   /* get a buffer for the request */
   uim_cmd_ptr = gstk_get_uim_buf();
   if (uim_cmd_ptr != NULL ) {
     gstk_status = gstk_util_populate_uim_buf_hdr_info(UIM_ENVELOPE_F, &uim_cmd_ptr);
     if ((gstk_status == GSTK_SUCCESS) && (uim_cmd_ptr != NULL)) {
       /* set user_data */
       uim_cmd_ptr->hdr.user_data               = call_disconnected_evt_cmd->message_header.user_data;

       /* initialized to 3 for value portion of the TLVs
          i.e., the final offset will be either 0 or 1 depends
          on whether the total length is > 0x7F or not */
       offset = 3;

       /* copy evt list */
       (void)memscpy(&(uim_cmd_ptr->envelope.data[offset]),
              sizeof(uim_cmd_ptr->envelope.data)-offset,
              &STK_envelope_cmd.event_download_hdr.evt_list,
              (int)(STK_envelope_cmd.event_download_hdr.evt_list.length
                     + GSTK_TAG_LENGTH_LEN));
       offset = (int)(offset + STK_envelope_cmd.event_download_hdr.evt_list.length
                     + GSTK_TAG_LENGTH_LEN);
       STK_envelope_cmd.event_download_hdr.length =
                      STK_envelope_cmd.event_download_hdr.evt_list.length
                     + GSTK_TAG_LENGTH_LEN;

       /* copy device */
       if(gstk_byte_offset_memcpy(uim_cmd_ptr->envelope.data,
                                  &(STK_envelope_cmd.event_download_hdr.device_id),
                                  offset,
                                  (size_t)(STK_envelope_cmd.event_download_hdr.device_id.device_tag_length +
                                  GSTK_TAG_LENGTH_LEN),
                                  sizeof(uim_cmd_ptr->envelope.data),
                                  sizeof(STK_envelope_cmd.event_download_hdr.device_id)) <
          (size_t)(STK_envelope_cmd.event_download_hdr.device_id.device_tag_length +
                   GSTK_TAG_LENGTH_LEN))
       {
         gstk_status = GSTK_MEMORY_ERROR;
       }

       offset = (int)(offset + STK_envelope_cmd.event_download_hdr.device_id.device_tag_length
                   + GSTK_TAG_LENGTH_LEN);

       STK_envelope_cmd.event_download_hdr.length +=
                         STK_envelope_cmd.event_download_hdr.device_id.device_tag_length
                         + GSTK_TAG_LENGTH_LEN;

       /* copy transaction id */
       /* tag, length */
       if (gstk_status == GSTK_SUCCESS &&
           (offset + GSTK_TAG_LENGTH_LEN -1) < UIM_MAX_ENVELOPE_BYTES)
       {
         if(memscpy(&(uim_cmd_ptr->envelope.data[offset]),
              sizeof(uim_cmd_ptr->envelope.data)-offset,
              &STK_envelope_cmd.transacton_id,
              GSTK_TAG_LENGTH_LEN) < GSTK_TAG_LENGTH_LEN)
         {
           gstk_status = GSTK_MEMORY_ERROR;
         }
         offset = (int)(offset + GSTK_TAG_LENGTH_LEN);
       }
       if(gstk_status == GSTK_SUCCESS &&
          STK_envelope_cmd.transacton_id.length != 0
          &&
          (offset + STK_envelope_cmd.transacton_id.length - 1) < UIM_MAX_ENVELOPE_BYTES
          &&
          STK_envelope_cmd.transacton_id.transaction_list != NULL)
       {
         if(memscpy(&(uim_cmd_ptr->envelope.data[offset]),
                    sizeof(uim_cmd_ptr->envelope.data)-offset,
                    STK_envelope_cmd.transacton_id.transaction_list,
                    (size_t)STK_envelope_cmd.transacton_id.length) !=
            (size_t)STK_envelope_cmd.transacton_id.length)
         {
           gstk_status = GSTK_MEMORY_ERROR;
         }
         offset = (int)(offset + STK_envelope_cmd.transacton_id.length);
       }
       if (gstk_status == GSTK_SUCCESS)
       {
       STK_envelope_cmd.event_download_hdr.length +=
                         STK_envelope_cmd.transacton_id.length
                         + GSTK_TAG_LENGTH_LEN;

       if((call_disconnected_evt_cmd->call_disconnected_data.cause.present == TRUE)
          &&
          (((STK_envelope_cmd.cause.length > GSTK_TLV_LENGTH_1_BYTE_OFFSET_LIMIT)
            && (offset + 2) < UIM_MAX_ENVELOPE_BYTES)
           ||
           ((STK_envelope_cmd.cause.length <= GSTK_TLV_LENGTH_1_BYTE_OFFSET_LIMIT)
            && (offset + 1) < UIM_MAX_ENVELOPE_BYTES))) {
         /* copy cause */
         /* tag, length */
         uim_cmd_ptr->envelope.data[offset++] = STK_envelope_cmd.cause.tag;
         if (STK_envelope_cmd.cause.length > GSTK_TLV_LENGTH_1_BYTE_OFFSET_LIMIT) {
           uim_cmd_ptr->envelope.data[offset++] = GSTK_2_BYTE_LENGTH_FIRST_VALUE;
           STK_envelope_cmd.event_download_hdr.length++;
         }
         if(offset < UIM_MAX_ENVELOPE_BYTES)
         {
           uim_cmd_ptr->envelope.data[offset++] = STK_envelope_cmd.cause.length;
         }

         if(STK_envelope_cmd.cause.length != 0
            &&
            (offset + STK_envelope_cmd.cause.length -1) < UIM_MAX_ENVELOPE_BYTES
            &&
            STK_envelope_cmd.cause.cause != NULL)
         {
           (void)memscpy(&(uim_cmd_ptr->envelope.data[offset]),
                  sizeof(uim_cmd_ptr->envelope.data)-offset,
                  STK_envelope_cmd.cause.cause,
                  (int)STK_envelope_cmd.cause.length);
           offset = (int)(offset + STK_envelope_cmd.cause.length);
         }

         STK_envelope_cmd.event_download_hdr.length +=
                         STK_envelope_cmd.cause.length
                         + GSTK_TAG_LENGTH_LEN;
       }

       /* populate the envelope command info */
       /* total data length:  evt id data + 2
                    + device id data + 2 (2 is for the tag and length field)
                    + transaction id data + 2
                    + cause data + 2
       */
       if(STK_envelope_cmd.event_download_hdr.length > GSTK_TLV_LENGTH_1_BYTE_OFFSET_LIMIT) {
         /* account for 0x80 */
         uim_cmd_ptr->envelope.offset = 0;
         uim_cmd_ptr->envelope.data[0] = STK_envelope_cmd.event_download_hdr.tag;
         uim_cmd_ptr->envelope.data[1] = GSTK_2_BYTE_LENGTH_FIRST_VALUE;
         uim_cmd_ptr->envelope.data[2] = STK_envelope_cmd.event_download_hdr.length;
         STK_envelope_cmd.event_download_hdr.length++;
         uim_cmd_ptr->envelope.num_bytes = STK_envelope_cmd.event_download_hdr.length
                                           + GSTK_TAG_LENGTH_LEN;

       }
       else {
         uim_cmd_ptr->envelope.offset = 1;
         uim_cmd_ptr->envelope.data[1] = STK_envelope_cmd.event_download_hdr.tag;
         uim_cmd_ptr->envelope.data[2] = STK_envelope_cmd.event_download_hdr.length;
         uim_cmd_ptr->envelope.num_bytes = STK_envelope_cmd.event_download_hdr.length
                                            + GSTK_TAG_LENGTH_LEN;
       }

       GSTK_UTIL_SELECTIVE_DUMP_BYTE_ARRAY("Call disconnected evt",
                                           &uim_cmd_ptr->envelope.data[uim_cmd_ptr->envelope.offset],
                                           uim_cmd_ptr->envelope.num_bytes);
       }

       if (gstk_status == GSTK_SUCCESS)
       {
         gstk_send_cmd_to_uim_server(uim_cmd_ptr); /* clear signal and call uim_cmd */
       }
       else
       {
         gstk_send_cmd_to_uim_free_q(uim_cmd_ptr);
         uim_cmd_ptr = NULL;
       }
     }
   } /* uim_cmd_ptr != NULL */
   else {
     gstk_status = GSTK_MEMORY_ERROR;
   }
   
   GSTK_FREE(STK_envelope_cmd.transacton_id.transaction_list);
   GSTK_FREE(STK_envelope_cmd.cause.cause);

   return gstk_status;

} /* gstk_call_disconnected_evt_ind*/

/*===========================================================================
FUNCTION gstk_check_evt_dl_voice_cap

DESCRIPTION
  This function check whether the voice capabilites are enabled

RETURN VALUE
  boolean

SIDE EFFECTS
  None

SEE ALSO
  None
===========================================================================*/
boolean gstk_check_evt_dl_voice_cap(void)
{
  return TRUE;
} /* gstk_check_evt_dl_voice_cap */

