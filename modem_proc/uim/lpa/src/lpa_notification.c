/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


     L P A   N O T I F I C A T I O N S   U T I L I T Y   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the functions which handle LPA notifications from
  the eUICC that are server bound.

                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2019, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_notification.c#3 $ $DateTime: 2021/09/11 17:08:59 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/12/21   skc     Set signal if there are more commands in the queue
10/23/19   vgd     Set le_present to TRUE when P1 is 91 for phase 2 cards
12/07/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "queue.h"

#include "lpa.h"
#include "lpa_platform.h"
#include "lpalib.h"
#include "lpa_mmgsdi.h"
#include "lpa_util.h"
#include "lpa_notification.h"
#include "lpa_asn1_encoder.h"
#include "lpa_asn1_decoder.h"
#include <stringl/stringl.h>
#include "lpa_efs.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

static q_type                             lpa_notification_cmd_q;

typedef struct {
  uint32                                                notifications_sent;
  lpa_notification_retrieve_notification_list_resp_type notification_list;
  uint32                                                seq_number_sent;
  uint8                                                 deferred_cmd_count;
  boolean                                               cmd_in_progress;
} lpa_notification_info_type;

/* Contains information regarding the notifications */
static lpa_notification_info_type         lpa_notification_global_info[LPA_NUM_SLOTS];


/*===========================================================================

                                  FUNCTIONS

===========================================================================*/

/*===========================================================================
FUNCTION LPA_NOTIFICATION_INIT

DESCRIPTION
  This function, called in lpa_main, is called to initialize the Notifications
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
void lpa_notification_init (
  void
)
{
  (void) q_init(&lpa_notification_cmd_q);

  memset(&lpa_notification_global_info, 0x00, sizeof(lpa_notification_global_info));
} /* lpa_notification_init */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_DEFER_REQUEST

DESCRIPTION
  This function defers triggering a new notification request. The deferred
  request will then be picked up once the current one is finished.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_notification_defer_request(
  lpa_slot_id_enum_type              slot_id
)
{
  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_notification_global_info[slot_id].deferred_cmd_count++;

  return LPA_SUCCESS;
} /* lpa_notification_defer_request */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_IS_CMD_IN_PROGRESS

DESCRIPTION
  This function finds out if any notification request is already ongoing.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_notification_is_cmd_in_progress(
  lpa_slot_id_enum_type              slot_id,
  boolean                            *in_progress_ptr
)
{
  if(slot_id >= LPA_NUM_SLOTS ||
     in_progress_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  *in_progress_ptr = lpa_notification_global_info[slot_id].cmd_in_progress;

  return LPA_SUCCESS;
} /* lpa_notification_is_cmd_in_progress */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_SET_CMD_IN_PROGRESS

DESCRIPTION
  This function marks a notification request as ongoing.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_notification_set_cmd_in_progress(
  lpa_slot_id_enum_type              slot_id
)
{
  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_notification_global_info[slot_id].cmd_in_progress = TRUE;

  return LPA_SUCCESS;
} /* lpa_notification_set_cmd_in_progress */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_CHECK_AND_TRIGGER_DEFERRED_CMD

DESCRIPTION
  This function checks if there is any deferred notification cmd. If so,
  it triggers that cmd.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_notification_check_and_trigger_deferred_cmd(
  lpa_slot_id_enum_type              slot_id
)
{
  lpa_result_enum_type               lpa_status = LPA_SUCCESS;

  LPA_MSG_HIGH_0("lpa_notification_check_and_trigger_deferred_cmd");

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(lpa_notification_global_info[slot_id].cmd_in_progress == FALSE &&
     lpa_notification_global_info[slot_id].deferred_cmd_count)
  {
    lpa_status = lpa_notification_retrieve_and_send(slot_id);
    lpa_notification_global_info[slot_id].deferred_cmd_count--;
  }

  return lpa_status;
} /* lpa_notification_check_and_trigger_deferred_cmd */


/*===========================================================================
   FUNCTION:      LPA_NOTIFICATION_FREE_RESPONSE_DATA

   DESCRIPTION:
     This function frees the response pointer that is mallocd (in lpa_mmgsdi or
     in lpa_http) in order to send a response back to LPA Notification module
     via its callback

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
static void lpa_notification_free_response_data(
  lpa_notification_cmd_data_type               *lpa_response_data_ptr
)
{
  uint32                                index = 0;

  if(lpa_response_data_ptr == NULL)
  {
    return;
  }

  switch(lpa_response_data_ptr->cmd)
  {
    case LPA_NOTIFICATION_RETRIEVE_NOTIFICATION_LIST_CMD:
      for(index = 0; index < lpa_response_data_ptr->data.retrieve_notification_list_resp.num_notifications; index++)
      {
        if(lpa_response_data_ptr->data.retrieve_notification_list_resp.notification_list_ptr[index].notification_type ==
             LPA_NOTIFICATION_PROFILE_INSTALLATION_RESULT)
        {
          LPA_FREE(lpa_response_data_ptr->data.retrieve_notification_list_resp.notification_list_ptr[index].data.profile_installation_result.data_ptr);
        }
        else
        {
          LPA_FREE(lpa_response_data_ptr->data.retrieve_notification_list_resp.notification_list_ptr[index].data.other_signed_notification.data_ptr);
        }
      }
      LPA_FREE(lpa_response_data_ptr->data.retrieve_notification_list_resp.notification_list_ptr);
      break;
    default:
       break;
  }
} /* lpa_notification_free_response_data */


/*===========================================================================
   FUNCTION:      LPA_NOTIFICATION_RESET

   DESCRIPTION:
     This function resets/frees any globals in lpa notification module

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None
===========================================================================*/
static void lpa_notification_reset (
  lpa_slot_id_enum_type                lpa_slot
)
{
  uint32                   index             = 0;

  LPA_MSG_HIGH_1("lpa_notification_reset, slot_id=0x%x", lpa_slot);

  if(lpa_slot >= LPA_NUM_SLOTS)
  {
    return;
  }

  for(index = 0; index < lpa_notification_global_info[lpa_slot].notification_list.num_notifications; index++)
  {
    if(lpa_notification_global_info[lpa_slot].notification_list.notification_list_ptr[index].notification_type ==
         LPA_NOTIFICATION_PROFILE_INSTALLATION_RESULT)
    {
      LPA_FREE(lpa_notification_global_info[lpa_slot].notification_list.notification_list_ptr[index].data.profile_installation_result.data_ptr);
    }
    else
    {
      LPA_FREE(lpa_notification_global_info[lpa_slot].notification_list.notification_list_ptr[index].data.other_signed_notification.data_ptr);
    }
  }

  LPA_FREE(lpa_notification_global_info[lpa_slot].notification_list.notification_list_ptr);
  lpa_notification_global_info[lpa_slot].notification_list.num_notifications = 0;

  /* If no notification request is currently in deferred state, clean up the
     global */
  lpa_notification_global_info[lpa_slot].cmd_in_progress = FALSE;
  lpa_notification_global_info[lpa_slot].seq_number_sent = 0;
  lpa_notification_global_info[lpa_slot].notifications_sent = 0;
  if(lpa_notification_global_info[lpa_slot].deferred_cmd_count == 0)
  {
    memset(&lpa_notification_global_info[lpa_slot], 0x00, sizeof(lpa_notification_global_info[lpa_slot]));
  }
} /* lpa_notification_reset */


/*===========================================================================
   FUNCTION:      LPA_NOTIFICATION_QUEUE_CMD

   DESCRIPTION:
     This function queues commands to LPA to processes the Notifications
     procedures.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     lpa_result_enum_type

   SIDE EFFECTS:
     None
===========================================================================*/
static lpa_result_enum_type  lpa_notification_queue_cmd (
  lpa_notification_response_data_type          *task_cmd_ptr
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
    q_put(&lpa_notification_cmd_q, &task_cmd_ptr->link);

    /* Set the command queue signal */
    (void) rex_set_sigs(UIM_LPA_TCB, LPA_NOTIFICATION_CMD_SIG);
  }
  else
  {
    LPA_MSG_ERR_0("LPA task stopped, cannot queue command");
    lpa_status = LPA_GENERIC_ERROR;
  }

  LPA_UTIL_LEAVE_TASK_STATE_CRIT_SECT;
  return lpa_status;
} /* lpa_notification_queue_cmd */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_HTTP_CB

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
static void lpa_notification_http_cb (
  lpa_result_enum_type                      status,
  const lpa_http_resp_data_type            *http_resp_ptr,
  const void                               *user_data_ptr
)
{
  lpa_notification_response_data_type  *task_cmd_ptr = NULL;

  if(http_resp_ptr == NULL)
  {
    return;
  }

  LPA_MSG_HIGH_2("Received lpa_notification_http_cb status=0x%x, slot=0x%x", status, http_resp_ptr->slot_id);

  /* Malloc task cmd pointers and any sub pointers, as needed, and copy
     the response data received from the caller of this callback */
  task_cmd_ptr = (lpa_notification_response_data_type*)LPA_MALLOC(sizeof(lpa_notification_response_data_type));
  if(task_cmd_ptr == NULL)
  {
    return;
  }

  task_cmd_ptr->slot_id            = http_resp_ptr->slot_id;
  task_cmd_ptr->status             = status;
  task_cmd_ptr->cmd_data.cmd       = (lpa_notification_cmd_enum_type) user_data_ptr;

  if(lpa_notification_queue_cmd(task_cmd_ptr) != LPA_SUCCESS)
  {
    lpa_notification_free_response_data(&task_cmd_ptr->cmd_data);
    LPA_FREE(task_cmd_ptr);
  }
} /* lpa_notification_http_cb */


/*===========================================================================
   FUNCTION:      LPA_NOTIFICATION_MMGSDI_CB

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
void lpa_notification_mmgsdi_cb (
  lpa_result_enum_type                             status,
  const lpa_mmgsdi_resp_data_type                 *mmgsdi_resp_ptr,
  const void                                      *user_data_ptr
)
{
  lpa_notification_response_data_type            *task_cmd_ptr = NULL;

  LPA_MSG_HIGH_2("Received lpa_notification_mmgsdi_cb status=0x%x,cmd=0x%x",
                 status, (lpa_notification_cmd_enum_type)user_data_ptr);

  if(mmgsdi_resp_ptr == NULL)
  {
    return;
  }

  /* Malloc task cmd pointers and any sub pointers, as needed, and copy
     the response data received from the caller of this callback */
  task_cmd_ptr = (lpa_notification_response_data_type*)LPA_MALLOC(sizeof(lpa_notification_response_data_type));
  if(task_cmd_ptr == NULL)
  {
    return;
  }

  task_cmd_ptr->slot_id            = mmgsdi_resp_ptr->slot_id;
  task_cmd_ptr->status             = status;
  task_cmd_ptr->cmd_data.cmd       = (lpa_notification_cmd_enum_type)user_data_ptr;

  if(status == LPA_SUCCESS)
  {
    switch(task_cmd_ptr->cmd_data.cmd)
    {
      case LPA_NOTIFICATION_RETRIEVE_NOTIFICATION_LIST_CMD:
        status = lpa_asn1_decode_retrieve_notification_list_resp(
                   mmgsdi_resp_ptr->data_len,
                   mmgsdi_resp_ptr->data_ptr,
                   &(task_cmd_ptr->cmd_data.data.retrieve_notification_list_resp));
        break;
      case LPA_NOTIFICATION_REMOVE_NOTIFICATION_FROM_LIST_CMD:
        status = lpa_asn1_decode_remove_notification_from_list_resp(
                   mmgsdi_resp_ptr->data_len,
                   mmgsdi_resp_ptr->data_ptr);
        break;
      default:
        status = LPA_GENERIC_ERROR;
        break;
    }
  }

  task_cmd_ptr->status             = status;

  if(lpa_notification_queue_cmd(task_cmd_ptr) != LPA_SUCCESS)
  {
    lpa_notification_free_response_data(&task_cmd_ptr->cmd_data);
    LPA_FREE(task_cmd_ptr);
  }
} /* lpa_notification_mmgsdi_cb */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_PROCESS_SEND_NOTIFICATION_RESP

DESCRIPTION
  Processes Send Notification reponse from the server. Sends Remove
  Notification to the eUICC

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_notification_process_send_notification_resp (
  lpa_slot_id_enum_type                                  slot_id
)
{
  lpa_result_enum_type                          lpa_status            = LPA_SUCCESS;
  lpa_data_type                                 data;
  uint32                                        index                 = 0;
  uint32                                        seq_number            = 0;

  memset(&data, 0x00, sizeof(data));

  if(slot_id >= LPA_NUM_SLOTS ||
     lpa_notification_global_info[slot_id].notifications_sent == 0 ||
     lpa_notification_global_info[slot_id].notification_list.notification_list_ptr == NULL)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Lets find out the sequence number of the notification that we just sent
     to the server */
  index = lpa_notification_global_info[slot_id].notifications_sent - 1;
  if(index >= lpa_notification_global_info[slot_id].notification_list.num_notifications)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Remove the notification from the eUICC corresponding to this seq_number */
  seq_number =
    lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].seq_number;

  LPA_MSG_ERR_1("In lpa_notification_process_send_notification_resp, seq_number=0x%x",
                seq_number);

  /* ASN1 encode the Remove Notification command */
  lpa_status = lpa_asn1_encode_remove_notification_from_list_request(seq_number,
                                                                     &data);

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
                                     lpa_notification_mmgsdi_cb,
                                     (void*) LPA_NOTIFICATION_REMOVE_NOTIFICATION_FROM_LIST_CMD);

  LPA_FREE(data.data_ptr);
  data.data_len = 0;

  return lpa_status;
} /* lpa_notification_process_send_notification_resp */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_PROCESS_RETRIEVE_NOTIFICATION_LIS_RESP

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
static lpa_result_enum_type lpa_notification_process_retrieve_notification_list_resp (
  lpa_notification_retrieve_notification_list_resp_type *retrieve_notification_list_ptr,
  lpa_slot_id_enum_type                                  slot_id
)
{
  lpa_result_enum_type                          lpa_status            = LPA_SUCCESS;
  uint32                                        index                 = 0;
  lpa_http_handle_notification_req_type         send_notification_req;

  memset(&send_notification_req, 0x00, sizeof(send_notification_req));

  if(retrieve_notification_list_ptr == NULL ||
     slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(retrieve_notification_list_ptr->num_notifications == 0 ||
     retrieve_notification_list_ptr->notification_list_ptr == NULL)
  {
    /* There is no notification to be sent to any server. This is not an error
       situation */
    return LPA_SUCCESS;
  }

  /* Copy the notifications over to the global so that we can send them over
     to the servers one at a time */

  /* Make sure we clean up the stale value from global before populating
     the new one */
  if(lpa_notification_global_info[slot_id].notification_list.num_notifications > 0)
  {
    for(index = 0; index < lpa_notification_global_info[slot_id].notification_list.num_notifications; index++)
    {
      if(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].notification_type ==
           LPA_NOTIFICATION_PROFILE_INSTALLATION_RESULT)
      {
        LPA_FREE(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.profile_installation_result.data_ptr);
      }
      else
      {
        LPA_FREE(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.other_signed_notification.data_ptr);
      }
    }
    LPA_FREE(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr);
    lpa_notification_global_info[slot_id].notification_list.num_notifications = 0;
  }

  lpa_notification_global_info[slot_id].notification_list.num_notifications =
    retrieve_notification_list_ptr->num_notifications;

  lpa_notification_global_info[slot_id].notification_list.notification_list_ptr =
    LPA_MALLOC(lpa_notification_global_info[slot_id].notification_list.num_notifications * sizeof(lpa_notification_type));
  if(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr == NULL)
  {
    return LPA_ERROR_HEAP_EXHAUSTED;
  }

  lpa_notification_global_info[slot_id].notification_list.num_notifications =
    retrieve_notification_list_ptr->num_notifications;

  index = 0;

  for(index = 0; index < retrieve_notification_list_ptr->num_notifications; index++)
  {
    lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].notification_type =
      retrieve_notification_list_ptr->notification_list_ptr[index].notification_type;

    lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].smdp_address =
      retrieve_notification_list_ptr->notification_list_ptr[index].smdp_address;

    lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].seq_number =
      retrieve_notification_list_ptr->notification_list_ptr[index].seq_number;

   LPA_MSG_HIGH_3("In lpa_notification_process_retrieve_notification_list_resp, notif_type[0x%x]=0x%x, seq_number=0x%x",
                  index,
                  retrieve_notification_list_ptr->notification_list_ptr[index].notification_type,
                  lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].seq_number);

    if(retrieve_notification_list_ptr->notification_list_ptr[index].notification_type ==
         LPA_NOTIFICATION_PROFILE_INSTALLATION_RESULT)
    {
      if(retrieve_notification_list_ptr->notification_list_ptr[index].data.profile_installation_result.data_len == 0)
      {
        /* If an entry for installation result is present, it must be non-zero */
        lpa_status = LPA_GENERIC_ERROR;
        break;
      }
      lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.profile_installation_result.data_ptr =
        (uint8*) LPA_MALLOC(retrieve_notification_list_ptr->notification_list_ptr[index].data.profile_installation_result.data_len);

      if(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.profile_installation_result.data_ptr == NULL)
      {
        /* Dont worry about freeing the pointers that have been allocated
           already... all of them will be free'd in the free function that
           is called for clean up by the caller */
        lpa_status = LPA_ERROR_HEAP_EXHAUSTED;
        break;
      }
      lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.profile_installation_result.data_len =
        retrieve_notification_list_ptr->notification_list_ptr[index].data.profile_installation_result.data_len;
      memscpy(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.profile_installation_result.data_ptr,
              lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.profile_installation_result.data_len,
              retrieve_notification_list_ptr->notification_list_ptr[index].data.profile_installation_result.data_ptr,
              retrieve_notification_list_ptr->notification_list_ptr[index].data.profile_installation_result.data_len);
    }
    else
    {
      if(retrieve_notification_list_ptr->notification_list_ptr[index].data.other_signed_notification.data_len == 0)
      {
        lpa_status = LPA_GENERIC_ERROR;
        break;
      }
      lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.other_signed_notification.data_ptr =
        (uint8*) LPA_MALLOC(retrieve_notification_list_ptr->notification_list_ptr[index].data.other_signed_notification.data_len);

      if(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.other_signed_notification.data_ptr == NULL)
      {
        /* Dont worry about freeing the pointers that have been allocated
           already... all of them will be free'd in the free function that
           is called for clean up by the caller */
        lpa_status = LPA_ERROR_HEAP_EXHAUSTED;
        break;
      }

      lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.other_signed_notification.data_len =
        retrieve_notification_list_ptr->notification_list_ptr[index].data.other_signed_notification.data_len;
      memscpy(lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.other_signed_notification.data_ptr,
              lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index].data.other_signed_notification.data_len,
              retrieve_notification_list_ptr->notification_list_ptr[index].data.other_signed_notification.data_ptr,
              retrieve_notification_list_ptr->notification_list_ptr[index].data.other_signed_notification.data_len);
    }
  }

  if(lpa_status == LPA_SUCCESS)
  {
    /* Send out the first notification */
    send_notification_req.slot_id = slot_id;
    send_notification_req.callback = lpa_notification_http_cb;
    send_notification_req.user_data_ptr = (void*) LPA_NOTIFICATION_SEND_NOTIFICATION_CMD;
    send_notification_req.notification = lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[0];
    lpa_status = lpa_http_build_and_send_handle_notification(&send_notification_req);
    if(lpa_status == LPA_SUCCESS)
    {
      lpa_notification_global_info[slot_id].notifications_sent++;
    }
  }

  LPA_MSG_HIGH_2("lpa_status=0x%x, lpa_notification_global_info[slot_id].notifications_sent=0x%x", lpa_status, lpa_notification_global_info[slot_id].notifications_sent);

  if(lpa_status != LPA_SUCCESS)
  {
    lpa_notification_reset(slot_id);
  }

  return lpa_status;
} /* lpa_notification_process_retrieve_notification_list_resp */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_PROCESS_REMOVE_NOTIFICATION_RESP

DESCRIPTION
  Processes Remove Notification response received from MMGSDI

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
static lpa_result_enum_type lpa_notification_process_remove_notification_resp (
  lpa_slot_id_enum_type                                  slot_id
)
{
  lpa_result_enum_type                          lpa_status            = LPA_SUCCESS;
  uint32                                        index                 = 0;
  lpa_http_handle_notification_req_type         send_notification_req;

  memset(&send_notification_req, 0x00, sizeof(send_notification_req));

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  /* Lets send the next notification, if any */
  index = lpa_notification_global_info[slot_id].notifications_sent;

  if(lpa_notification_global_info[slot_id].notifications_sent >=
       lpa_notification_global_info[slot_id].notification_list.num_notifications)
  {
    /* No more notifications to be sent for the current trigger.
       Check if there is any pending request. If so, entertain the
       request */
    lpa_notification_reset(slot_id);

    lpa_notification_check_and_trigger_deferred_cmd(slot_id);
    return LPA_SUCCESS;
  }

  /* Send out the subsequent notification */
  send_notification_req.slot_id = slot_id;
  send_notification_req.callback = lpa_notification_http_cb;
  send_notification_req.user_data_ptr = (void*) LPA_NOTIFICATION_SEND_NOTIFICATION_CMD;
  send_notification_req.notification = lpa_notification_global_info[slot_id].notification_list.notification_list_ptr[index];
  lpa_status = lpa_http_build_and_send_handle_notification(&send_notification_req);

  if(lpa_status == LPA_SUCCESS)
  {
    lpa_notification_global_info[slot_id].notifications_sent++;
  }

  LPA_MSG_HIGH_2("lpa_status=0x%x, lpa_notification_global_info[slot_id].notifications_sent=0x%x",lpa_status, lpa_notification_global_info[slot_id].notifications_sent);

  return lpa_status;
} /* lpa_notification_process_remove_notification_resp */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_HANDLE_CMD_SIG

DESCRIPTION
  This function, called in lpa_main, handles the processing of all the
  sub commands for handling notifications.

DEPENDENCIES
  LPA task must be finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_notification_handle_cmd_sig (
  void
)
{
  lpa_notification_response_data_type  *task_cmd_ptr  = NULL;
  lpa_result_enum_type                  lpa_status    = LPA_GENERIC_ERROR;

  task_cmd_ptr = (lpa_notification_response_data_type*)(q_get(&lpa_notification_cmd_q));

  (void) rex_clr_sigs(rex_self(), LPA_NOTIFICATION_CMD_SIG);

  if(task_cmd_ptr == NULL)
  {
    return;
  }

  LPA_MSG_HIGH_2("LPA_NOTIFICATION_CMD_SIG received for notification command: 0x%x on slot_id: 0x%x",
                 task_cmd_ptr->cmd_data.cmd, task_cmd_ptr->slot_id);
  lpa_status = task_cmd_ptr->status;

  if(lpa_status == LPA_SUCCESS ||
     task_cmd_ptr->cmd_data.cmd == LPA_NOTIFICATION_SEND_NOTIFICATION_CMD)  // TODO: Hack to delete the notif from eUICC even if sending to server failed
  {
    switch(task_cmd_ptr->cmd_data.cmd)
    {
      case LPA_NOTIFICATION_RETRIEVE_NOTIFICATION_LIST_CMD:
        lpa_status = lpa_notification_process_retrieve_notification_list_resp(
                       &task_cmd_ptr->cmd_data.data.retrieve_notification_list_resp,
                       task_cmd_ptr->slot_id);
        break;

      case LPA_NOTIFICATION_SEND_NOTIFICATION_CMD:
        lpa_status = lpa_notification_process_send_notification_resp(
                       task_cmd_ptr->slot_id);
        break;

      case LPA_NOTIFICATION_REMOVE_NOTIFICATION_FROM_LIST_CMD:
        lpa_status = lpa_notification_process_remove_notification_resp(
                       task_cmd_ptr->slot_id);
        break;

      default:
        lpa_status = LPA_GENERIC_ERROR;
        break;
    }
  }

  /* If the status is not successful, we should go ahead and clean up the
     global for the current trigger. Check if there is any deferred request.
     If so, start the request */
  if(lpa_status != LPA_SUCCESS)
  {
    lpa_notification_reset(task_cmd_ptr->slot_id);

    lpa_notification_check_and_trigger_deferred_cmd(task_cmd_ptr->slot_id);
  }

  lpa_notification_free_response_data(&task_cmd_ptr->cmd_data);
  LPA_FREE(task_cmd_ptr);

  /* Set the signal if there are one or more commands to look at */
  if(q_cnt(&lpa_notification_cmd_q) > 0)
  {
    /*  Items on the queue. Set the signal */
    (void) rex_set_sigs(rex_self(), LPA_NOTIFICATION_CMD_SIG);
  }
} /* lpa_notification_handle_cmd_sig */


/*===========================================================================
FUNCTION LPA_NOTIFICATION_RETRIEVE_AND_SEND

DESCRIPTION
  Function retrieves the pending notifications from the card and sends them
  to the appropriate servers.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_notification_retrieve_and_send (
  lpa_slot_id_enum_type                              slot_id
)
{
  lpa_data_type                       data;
  lpa_result_enum_type                lpa_status = LPA_SUCCESS;
  boolean                             cmd_in_progress = FALSE;

 LPA_MSG_HIGH_1("lpa_notification_retrieve_and_send slot_id=0x%x", slot_id);

  memset(&data, 0x00, sizeof(data));

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(LPA_SUCCESS != lpa_notification_is_cmd_in_progress(slot_id,
                                                        &cmd_in_progress) ||
     cmd_in_progress)
  {
     LPA_MSG_HIGH_1("Deferring the notification processing, cmd_in_progres=0x%x", cmd_in_progress);
    return lpa_notification_defer_request(slot_id);
  }

  if(LPA_SUCCESS != lpa_notification_set_cmd_in_progress(slot_id))
  {
    return LPA_GENERIC_ERROR;
  }

  lpa_status = lpa_asn1_encode_retrieve_notification_list_request(&data);
  if(lpa_status != LPA_SUCCESS)
  {
    LPA_FREE(data.data_ptr);
    return lpa_status;
  }

  /* Send STORE DATA apdu to the card for retrieving the pending notifications */
  lpa_status = lpa_mmgsdi_store_data(slot_id,
                                     LPA_PHASE_2_STORE_DATA_P1,
                                     LPA_MMGSDI_STORE_DATA_FIRST_BLOCK_NUM,
                                     data,
                                     TRUE,
                                     lpa_notification_mmgsdi_cb,
                                     (void*) LPA_NOTIFICATION_RETRIEVE_NOTIFICATION_LIST_CMD);
  LPA_FREE(data.data_ptr);

  return lpa_status;
}/* lpa_notification_retrieve_and_send */
