/*===========================================================================

                         Q M I _ U I M _ R E C O V E R Y. C

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2016, 2019 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_recovery.c#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/06/19    vgd    Free uim_msg_ptr only if it is not present in the queue
10/10/16    me     handling duplicates silent recovery 
07/08/16    ar     Move qmi_uimi_recovery and unify fdn/acl get_status func.
05/26/16    vdc    Move internal enums, structs to new internal header file
04/13/16    vdc    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "uim_p.h"
#include "mmgsdilib_common.h"
#include "qmi_uim_recovery.h"
#include "qmi_uim_cat_common.h"
#include "uim_msg.h"
#include "qmi_uim_util.h"
#include "amssassert.h"
#include "qmi_cat.h"
#include "uim_common_efs.h"
#include "uim_v.h"

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/


/*===========================================================================
  FUNCTION QMI_UIMI_RECOVERY_CALLBACK()

  DESCRIPTION
    This function is executed as callback of UIM Recovery API. This
    is executed in the context of UIM task and needs to make a deep
    copy of the results and post a command to the QMI task to be
    processed.

  PARAMETERS

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uimi_recovery_callback
(
  uim_slot_type                     slot,
  const void                       *user_data,
  uimdrv_recovery_stage_enum_type   recovery_stage
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  switch(recovery_stage)
  {
    case UIMDRV_RECOVERY_STAGE_START:
      UIM_MSG_MED_0("Recovery stage start response");
      /* Nothing to do */
      break;

    case UIMDRV_RECOVERY_STAGE_END:
      UIM_MSG_MED_0("Recovery stage end response");
      msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_RECOVERY_CB);
      if (msg_ptr != NULL)
      {
        msg_ptr->data.recovery_data.slot = slot;
        msg_ptr->data.recovery_data.user_data = user_data;

        /* Send command */
        if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
           (present_in_queue))
        {
          uimqmi_free(msg_ptr);
        }
        msg_ptr = NULL;
      }
      break;

    default:
      break;
  }
} /* qmi_uimi_recovery_callback */


/*===========================================================================
  FUNCTION  QMI_UIM_RECOVERY_PROCESS_CALLBACK()

  DESCRIPTION
    Generic QMI processing for external commands

  PARAMETERS
    uim_message_ptr : message to the internal qmi_uim_message
                      containing an external command

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_recovery_process_callback
(
  qmi_uim_message_type * uim_message_ptr
)
{
  qmi_cmd_buf_type *       cmd_buf_p        = NULL;
  qmi_uimi_userdata_type * cb_userdata_ptr  = NULL;
  dsm_item_type *          response         = NULL;
  uint8                    clid             = QMI_SVC_CLID_UNUSED;
  uint32                   client_reg_count = 0;

  UIM_MSG_HIGH_0("Sending response for recovery");

  ASSERT( uim_message_ptr );

  /* Retrieve userdata */
  cb_userdata_ptr = (qmi_uimi_userdata_type*)uim_message_ptr->data.recovery_data.user_data;
  if (cb_userdata_ptr == NULL)
  {
    UIM_MSG_HIGH_0("Null userdata ptr in response");
    return;
  }

  /* Retrieve parameters */
  clid             = cb_userdata_ptr->clid;
  client_reg_count = cb_userdata_ptr->client_reg_count;
  cmd_buf_p        = (qmi_cmd_buf_type*)cb_userdata_ptr->cmd_buf_ptr;

  /* Free userdata */
  qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
  cb_userdata_ptr = NULL;

  /* First check if the requested client is still valid */
  if (FALSE == qmi_uim_util_check_clid(clid, client_reg_count))
  {
    return;
  }

  if (cmd_buf_p == NULL)
  {
    return;
  }

  QMI_UIM_VALIDATE_SP_IN_CMD_BUF(cmd_buf_p);

  /* Update result TLV and Send response */
  uimqmi_put_result_send_response_ind(cmd_buf_p,
                                      response,
                                      QMI_ERR_NONE,
                                      QMUX_SERVICE_UIM,
                                      FALSE,
                                      QMI_SVC_CLID_UNUSED,
                                      (uint16)UIMI_CMD_VAL_WIDTH);
} /* qmi_uim_recovery_process_callback */


/*===========================================================================
  FUNCTION QMI_UIM_RECOVERY_SEND_COMPLETE_INDICATION()

  DESCRIPTION
    This function sends a recovery complete indication to a specific client.

  PARAMETERS
    clid   : Client ID
    slot   : Slot of cooresponding of recovery

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_recovery_send_complete_indication
(
  uint8                         clid,
  mmgsdi_slot_id_enum_type      slot
)
{
  dsm_item_type               * indication   = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED);

  /* Insert recovery slot in TLV */
  if (FALSE == qmi_uim_response_slot(
                   UIMI_TLV_IND_TAG_MANDATORY_1,
                   &indication,
                   slot))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Send indication to the client */
  (void)uimqmi_send_indication(clid,
                               (uint16)UIMI_CMD_VAL_RECOVERY_COMPLETE_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_recovery_send_complete_indication */


/*===========================================================================
  FUNCTION QMI_UIM_RECOVERY_SEND_COMPLETE_INDICATION_TO_ALL()

  DESCRIPTION
    This function processes the recovery complete indication from UIM drivers.
    It passes indicates that recovery has completed successfully to all
    registered QMI clients.

  PARAMETERS
    slot : slot in which the recovery was performed

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_recovery_send_complete_indication_to_all
(
  mmgsdi_slot_id_enum_type  slot
)
{
  uint8                         i           = 0;
  qmi_uimi_client_state_type  * cl_sp       = NULL;

  UIM_MSG_HIGH_1("qmi_uim_recovery_send_complete_indication_to_all: slot = 0x%x",
                 slot);

  if(qmi_uim_state_ptr == NULL)
  {
    return;
  }

  for (i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[i];

    /* Send recovery complete indication if client requested for them
       during client registration (saved in reg_for_recovery_complete_event flag) */
    if ( cl_sp != NULL &&
         cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
         cl_sp->uim_info.reg_event_type.recovery_complete_event )
    {
      qmi_uim_recovery_send_complete_indication(cl_sp->common.clid, slot);
    }
  }
} /* qmi_uim_recovery_send_complete_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_RECOVERY_PROCESS_COMPLETE_EVT()

  DESCRIPTION
    This function processes the recovery complete indication from UIM drivers.
    It indicates to all registered QMI clients and to QMI CAT that recovery
    has completed successfully.

  PARAMETERS
    mmgsdi_slot:  Slot in which the recovery was performed

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_recovery_process_complete_evt
(
  mmgsdi_slot_id_enum_type        mmgsdi_slot
)
{
  uint8                           slot_idx     = 0;

  ASSERT(qmi_uim_global_ptr);

  UIM_MSG_HIGH_1("qmi_uim_recovery_process_complete_evt: mmgsdi slot = 0x%x",
                 mmgsdi_slot);

  /* Notify QMI CAT */
  qmi_cat_recovery_complete(mmgsdi_slot);

  /* Notify QMI UIM clients */
  qmi_uim_recovery_send_complete_indication_to_all(mmgsdi_slot);

  if (uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot, 
                                       &slot_idx, 
                                       qmi_uim_global_ptr->card_state.num_slots) == QMI_ERR_NONE)
  {
    /* Reset the recovery in progress flag */
    qmi_uim_global_ptr->is_recovery_in_progress[slot_idx] = FALSE;
  }
} /* qmi_uim_recovery_process_complete_evt */


/*===========================================================================
  FUNCTION QMI_UIMI_RECOVERY()

  DESCRIPTION
    Recovers the card information

  PARAMETERS
    sp        : service provided state pointer
    cmd_buf_p : ptr to cmd buffer
    cl_sp     : client state pointer
    sdu_in    : incoming request

  RETURN VALUE
    ptr to response or NULL if none.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
dsm_item_type * qmi_uimi_recovery
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type               errval          = QMI_ERR_NONE;
  uim_return_type                uim_status      = UIM_GENERIC_ERROR;
  qmi_uim_tlv_item_type          tlv_data[1];
  uim_slot_type                  uim_slot        = UIM_SLOT_NONE;
  qmi_uimi_userdata_type*        cb_userdata_ptr = NULL;
  uint8                          recovery_flag   = 0;
  uint8                          slot_idx        = 0;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if(qmi_uim_util_mmgsdi_slot_id_to_uim_slot_id(tlv_data[0].data.slot.slot,
                                                &uim_slot) != QMI_ERR_NONE)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }
  
  if (uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.slot.slot, 
                                       &slot_idx, 
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* If recovery is already in progress on the slot, then we must return directly */
  if (qmi_uim_global_ptr->is_recovery_in_progress[slot_idx])
  {
    errval = QMI_ERR_NO_EFFECT;
    goto send_result;
  }

  if (uim_common_efs_read(
        UIM_COMMON_EFS_UIMQMI_QMI_UIM_CONFIG_SILENT_RECOVERY,
        UIM_COMMON_EFS_ITEM_FILE_TYPE,
        UIM_COMMON_EFS_DEVICE,
        &recovery_flag,
        sizeof(recovery_flag)) != UIM_COMMON_EFS_SUCCESS)
  {
    UIM_MSG_HIGH_0("Failed to read recovery configuration from NV");
    recovery_flag = FALSE;
  }

  if (recovery_flag == FALSE)
  {
    UIM_MSG_HIGH_0("External recovery not allowed");
    errval = QMI_ERR_NOT_SUPPORTED;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_RECOVERY,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  uim_status = uim_trigger_silent_recovery(uim_slot,
                                           cb_userdata_ptr,
                                           qmi_uimi_recovery_callback);

  if (uim_status != UIM_SUCCESS)
  {
    qmi_uim_util_free_cb_userdata(cb_userdata_ptr);
    cb_userdata_ptr = NULL;
    errval = qmi_uim_util_convert_uimdrv_status_to_errval(uim_status);
  }
  else
  {
    qmi_uim_global_ptr->is_recovery_in_progress[slot_idx] = TRUE;
  }

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  return qmi_uimi_util_populate_response(errval, FALSE, 0, 0);
} /* qmi_uimi_recovery */

