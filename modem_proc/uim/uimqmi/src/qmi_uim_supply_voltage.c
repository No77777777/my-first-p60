/*===========================================================================

           Q M I _ U I M _ S U P P L Y _ V O L T A G E . C

DESCRIPTION

 This file contains the functions for Supply Voltage functionality

Copyright (c) 2016, 2019 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_supply_voltage.c#1 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/06/19    vgd    Free uim_msg_ptr only if it is not present in the queue
11/04/16    me     Combined multi-line ASSERT in one line
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
05/20/16    ar     Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "qmi_uim_util.h"
#include "qmi_uim_cat_common.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_supply_voltage.h"
#include "uim_msg.h"
#include "uim_v.h"
#include "err.h"
#include "amssassert.h"

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_ALL_REG_CLIENTS_ACK_VCC_DEACTIVATION()

  DESCRIPTION
    Indicates whether all registered clients have acknowledged Vcc
    power down.

  PARAMETERS
    mmgsdi_slot : MMGSDI slot id

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_all_reg_clients_ack_vcc_deactivation
(
  mmgsdi_slot_id_enum_type  mmgsdi_slot
)
{
  uint8             i          = 0;
  uint8             slot_idx   = 0;
  uim_slot_type     uim_slot   = UIM_SLOT_NONE;
  qmi_error_e_type  errval     = QMI_ERR_NONE;

  errval = uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                            &slot_idx,
                                            qmi_uim_global_ptr->card_state.num_slots);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  for (i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    if(qmi_uim_state_ptr->client[i] &&
       qmi_uim_state_ptr->client[i]->uim_info.supply_voltage_pending_ack[slot_idx] == TRUE)
    {
      return QMI_ERR_NONE;
    }
  }

  errval = qmi_uim_util_mmgsdi_slot_id_to_uim_slot_id(mmgsdi_slot,
                                                      &uim_slot);
  if (errval != QMI_ERR_NONE)
  {
    return errval;
  }

  /* Send acknowledgement back to UIM drivers notifying them that all
     clients have notified with ok to proceed with Vcc power down */
  if (qmi_uim_global_ptr->supply_voltage_cb[slot_idx])
  {
    qmi_uim_global_ptr->supply_voltage_cb[slot_idx](uim_slot, UIMDRV_LDO_AWAITING_DEACTIVATION_IND_ACK);
  }

  /* After notifying UIM drivers we need to clear the saved supply voltage acknowledgement
     callback indicating that the indication for Vcc deactivate has been acknowledged. */
  qmi_uim_global_ptr->supply_voltage_cb[slot_idx] = NULL;

  return QMI_ERR_NONE;
} /* qmi_uim_all_reg_clients_ack_vcc_deactivation */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_SUPPLY_VOLTAGE_INDICATION()

  DESCRIPTION
    This function sends a supply voltage indication to a specific client.

  PARAMETERS
    clid          : Client ID
    mmgsdi_slot   : Slot of cooresponding supply voltage change
    ldo_state     : Current LDO state to be notified

  RETURN VALUE
    qmi_error_e_type

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static qmi_error_e_type qmi_uim_send_supply_voltage_indication
(
  uint8                           clid,
  mmgsdi_slot_id_enum_type        mmgsdi_slot,
  uimdrv_qmi_indications_type     ldo_state
)
{
  dsm_item_type               * indication   = NULL;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && qmi_uim_global_ptr);

  /* Insert slot in TLV */
  if (FALSE == qmi_uim_response_slot(
                   UIMI_TLV_IND_TAG_MANDATORY_1,
                   &indication,
                   mmgsdi_slot))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return QMI_ERR_INTERNAL;
  }

  /* Insert ldo_state in TLV */
  if (FALSE == qmi_uim_response_vcc_state(
                   UIMI_TLV_IND_TAG_MANDATORY_2,
                   &indication,
                   ldo_state))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return QMI_ERR_INTERNAL;
  }

  /* Send indication to the client */
  if(FALSE == uimqmi_send_indication(clid,
                                     (uint16)UIMI_CMD_VAL_SUPPLY_VOLTAGE,
                                     indication,
                                     QMUX_SERVICE_UIM))
  {
    return QMI_ERR_INTERNAL;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_send_supply_voltage_indication */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_SUPPLY_VOLTAGE_INDICATION_TO_ALL()

  DESCRIPTION
    This function will send the supply voltage indication to all registered
    clients.

  PARAMETERS
    uim_message_ptr: QMI UIM message

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_supply_voltage_indication_to_all
(
  const qmi_uim_message_type * uim_message_ptr
)
{
  uint8                                         i                = 0;
  qmi_uimi_client_state_type                  * cl_sp            = NULL;
  uint8                                         slot_idx         = 0;
  mmgsdi_slot_id_enum_type                      mmgsdi_slot      = MMGSDI_MAX_SLOT_ID_ENUM;
  qmi_error_e_type                              errval           = QMI_ERR_NONE;

  if(qmi_uim_state_ptr == NULL ||
     uim_message_ptr == NULL)
  {
    return;
  }

  errval = qmi_uim_util_uim_slot_id_to_mmgsdi_slot_id(uim_message_ptr->data.supply_voltage.slot,
                                                      &mmgsdi_slot);
  if (errval != QMI_ERR_NONE)
  {
    return;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  switch (uim_message_ptr->data.supply_voltage.ldo_state)
  {
    case UIMDRV_LDO_ACTIVATED_IND:
      qmi_uim_global_ptr->supply_voltage_cb[slot_idx] = NULL;
      break;
    case UIMDRV_LDO_AWAITING_DEACTIVATION_IND:
      qmi_uim_global_ptr->supply_voltage_cb[slot_idx] =
        uim_message_ptr->data.supply_voltage.uim_callback;
      break;
    default:
      UIM_MSG_ERR_1("Invalid LDO state: 0x%x",
                    uim_message_ptr->data.supply_voltage.ldo_state);
      return;
  }

  /* After saving the supply voltage callback, we need to clear the supply voltage acknowledgement
     bit indicating that none of the clients have sent acknowledgments just in case accidentally
     any client did not vote in the previous Vcc deactivation. */
  for (i = 0; i < UIMQMI_SVC_MAX_CLIDS; i++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[i];
    if(cl_sp == NULL)
    {
      continue;
    }

    cl_sp->uim_info.supply_voltage_pending_ack[slot_idx] = FALSE;

    /* Send supply voltage indication if client requested for them during client event
       registration (saved in reg_event_type.supply_voltage_events flag) */
    if ( cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
         cl_sp->uim_info.reg_event_type.supply_voltage_events)
    {
      errval = qmi_uim_send_supply_voltage_indication(
                   cl_sp->common.clid,
                   mmgsdi_slot,
                   uim_message_ptr->data.supply_voltage.ldo_state);

      if(uim_message_ptr->data.supply_voltage.ldo_state == UIMDRV_LDO_AWAITING_DEACTIVATION_IND &&
         errval == QMI_ERR_NONE)
      {
        cl_sp->uim_info.supply_voltage_pending_ack[slot_idx] = TRUE;
      }
    }
  }
} /* qmi_uim_send_supply_voltage_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_SUPPLY_VOLTAGE_IND()

  DESCRIPTION
    This function processes the supply voltage indication from UIM drivers.
    It indicates either Vcc needs to be deactivated or that Vcc activation has
    occurred to all registered QMI clients.

  PARAMETERS
    uim_message_ptr: QMI UIM message

  RETURN VALUE
    None

  DEPENDENCIES
    QMI UIM must already have been initialized and registered with Framework

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_process_supply_voltage_ind
(
  const qmi_uim_message_type * uim_message_ptr
)
{
  uint8                     slot_idx     = 0;
  mmgsdi_slot_id_enum_type  mmgsdi_slot  = MMGSDI_MAX_SLOT_ID_ENUM;
  qmi_error_e_type          errval       = QMI_ERR_NONE;

  if(uim_message_ptr == NULL)
  {
    return;
  }

  errval = qmi_uim_util_uim_slot_id_to_mmgsdi_slot_id(uim_message_ptr->data.supply_voltage.slot,
                                                      &mmgsdi_slot);
  if (errval != QMI_ERR_NONE)
  {
    return;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(mmgsdi_slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  qmi_uim_send_supply_voltage_indication_to_all(uim_message_ptr);

  (void)qmi_uim_all_reg_clients_ack_vcc_deactivation(mmgsdi_slot);
} /* qmi_uim_process_supply_voltage_ind */


/*===========================================================================
  FUNCTION QMI_UIM_SUPPLY_VOLTAGE_IND()

  DESCRIPTION
    This function is invoked by the UIM drivers when the Vcc needs to
    be deactivated or when Vcc has been activated.

  PARAMETERS
    slot                   : UIM slot id
    ldo_state              : LDO state
    uim_callback_ptr       : callback to ack drivers

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_supply_voltage_ind
(
  uim_slot_type                                     slot,
  uimdrv_qmi_indications_type                       ldo_state,
  uimdrv_qmi_power_management_callback_type         uim_callback_ptr
)
{
  qmi_uim_message_type * msg_ptr          = NULL;
  boolean                present_in_queue = FALSE;

  UIM_MSG_HIGH_2("Received supply voltage indication, slot: 0x%x, ldo_state: 0x%x",
                 slot, ldo_state);

  msg_ptr = qmi_uim_util_create_message(QMI_UIM_MESSAGE_SUPPLY_VOLTAGE_IND);
  if (msg_ptr == NULL)
  {
    return;
  }

  msg_ptr->data.supply_voltage.slot             = slot;
  msg_ptr->data.supply_voltage.ldo_state        = ldo_state;
  msg_ptr->data.supply_voltage.uim_callback     = uim_callback_ptr;

  /* Send command */
  if((!qmi_uim_util_post_message(msg_ptr, &present_in_queue)) &&
     (present_in_queue))
  {
    uimqmi_free(msg_ptr);
  }
  msg_ptr = NULL;
} /* qmi_uim_supply_voltage_ind */


/*===========================================================================
  FUNCTION QMI_UIMI_SUPPLY_VOLTAGE()

  DESCRIPTION
    Marks that the client has acknowledged the supply voltage indication
    and once all clients have responded, notifies uimdrv to proceed with
    Vcc deactivation.

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
dsm_item_type * qmi_uimi_supply_voltage
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                            errval             = QMI_ERR_NONE;
  qmi_uim_tlv_item_type                       tlv_data[1];
  uim_slot_type                               uim_slot           = UIM_SLOT_NONE;
  uint8                                       slot_idx           = 0;
  dsm_item_type *                             response           = NULL;
  qmi_result_e_type                           result             = QMI_RESULT_SUCCESS;
  boolean                                     retval             = FALSE;

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SLOT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 1);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = qmi_uim_util_mmgsdi_slot_id_to_uim_slot_id(tlv_data[0].data.slot.slot,
                                                      &uim_slot);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  errval = uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.slot.slot,
                                            &slot_idx,
                                            qmi_uim_global_ptr->card_state.num_slots);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if (!((qmi_uimi_client_state_type*)cl_sp)->uim_info.supply_voltage_pending_ack[slot_idx])
  {
    UIM_MSG_ERR_0("Client is not pending any supply voltage acknowledgement");
    errval = QMI_ERR_DEVICE_NOT_READY;
    goto send_result;
  }

  ((qmi_uimi_client_state_type*)cl_sp)->uim_info.supply_voltage_pending_ack[slot_idx] = FALSE;

  /* Only send acknowledgement to UIM drivers once all registered clients have responded
     with their acknowledgements */
  errval = qmi_uim_all_reg_clients_ack_vcc_deactivation(tlv_data[0].data.slot.slot);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 1);

  /* Send the response */
  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);
  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_supply_voltage */

