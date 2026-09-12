/*==============================================================================

                         Q M I _ U I M _ S A P . C

DESCRIPTION
  The Data Services Qualcomm MSM Interface Device Management Services source
  file.

Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sap.c#1 $

when        who    what, where, why
--------    ---    -------------------------------------------------------------
11/04/16    me     Combined multi-line ASSERT in one line
06/22/16    sp     Initial Revision
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include "uim_variation.h"
#include "uim_msg.h"
#include "amssassert.h"
#include "qmi_uim_cat_common.h"
#include "qmi_uim.h"
#include "mmgsdisessionlib_v.h"
#include "fs_public.h"

#include "qmi_uim_sap.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_sap_parsing.h"
#include "qmi_uim_internal.h"
#include "qmi_uim_util.h"

/*==============================================================================

                             EXTERNAL FUNCTIONS

==============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_IS_SAP_REQUEST_REJECTED()

  DESCRIPTION
    If APDU security restrictions are enabled on the device, this function
    parses the incoming SAP APDU request and checks if it is a SELECT command
    using the DF name (P1 = 0x04) and specifying AID of a non-telecom
    application. Access to such non-3GPP/2 RIDs are rejected.

  PARAMETERS
    apdu_data  : Incoming SAP APDU request

  RETURN VALUE
    Boolean indicating if the request is to be rejected or not.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static boolean qmi_uim_is_sap_request_rejected
(
  mmgsdi_send_apdu_data_type    apdu_data
)
{
  const uint8 rid_value_3gpp[]  =  { 0xA0, 0x00, 0x00, 0x00, 0x87 };
  const uint8 rid_value_3gpp2[] =  { 0xA0, 0x00, 0x00, 0x03, 0x43 };

  ASSERT(qmi_uim_global_ptr);

  /* If no APDU security restrictions, nothing to check */
  if (qmi_uim_global_ptr->apdu_sec_restr_value == QMI_UIM_APDU_SEC_RESTRICTIONS_NONE)
  {
    return FALSE;
  }

  /* Check these bytes:
     Byte 2: INS
     Byte 3: P1
     Bytes 6 through 10: RID (first 5 bytes of AID), if needed
  */
  if ((apdu_data.data_ptr != NULL) &&
      (apdu_data.data_len >= 3))
  {
    if ((apdu_data.data_ptr[1] == QMI_UIM_APDU_INS_BYTE_SELECT_FILE) &&
        (apdu_data.data_ptr[2] == QMI_UIM_APDU_P1_BYTE_SELECT_DF_NAME))
    {
      /* Reject if it is an attempt to provide a truncated AID < 5 bytes */
      if (apdu_data.data_len < 10)
      {
        UIM_MSG_ERR_1("ADPU rejected due to security restrictions: data_len: 0x%x",
                      apdu_data.data_len);
        return TRUE;
      }
      /* Also reject if an RID other than 3GPP or 3GPP2 is provided */
      if ((memcmp(&apdu_data.data_ptr[5], rid_value_3gpp, sizeof(rid_value_3gpp)) != 0) &&
          (memcmp(&apdu_data.data_ptr[5], rid_value_3gpp2, sizeof(rid_value_3gpp2)) != 0))
      {
        UIM_MSG_ERR_0("ADPU rejected due to security restrictions: restricted AID");
        return TRUE;
      }
    }
  }

  return FALSE;
} /* qmi_uim_is_sap_request_rejected */


/*===========================================================================
  FUNCTION QMI_UIMI_SAP_CONNECTION()

  DESCRIPTION
    Send connection related SAP commands to the card

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
dsm_item_type * qmi_uimi_sap_connection
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                errval          = QMI_ERR_NONE;
  dsm_item_type *                 response        = NULL;
  boolean                         retval          = TRUE;
  qmi_result_e_type               result          = QMI_RESULT_SUCCESS;
  mmgsdi_return_enum_type         mmgsdi_status   = MMGSDI_SUCCESS;
  uint8                           slot_idx        = 0;
  uim_sap_connect_op_enum_v01     op_type         = UIM_SAP_OP_CHECK_STATUS_V01;
  qmi_uimi_userdata_type*         cb_userdata_ptr = NULL;
  qmi_uim_tlv_item_type           tlv_data[4];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SAP_CONNECT;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_SAP_DISCONNECT_MODE;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;
  tlv_data[2].type      = UIMI_TLV_TYPE_SAP_INTERMEDIATE_GET_RESP;
  tlv_data[2].tag       = UIMI_TLV_TAG_OPTIONAL_2;
  tlv_data[3].type      = UIMI_TLV_TYPE_SAP_CONNECTION_CONDITION;
  tlv_data[3].tag       = UIMI_TLV_TAG_OPTIONAL_3;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 4);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.sap_connection.slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  if(slot_idx >= QMI_UIM_MAX_CARD_COUNT ||
     slot_idx >= qmi_uim_global_ptr->card_state.num_slots)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  op_type = tlv_data[0].data.sap_connection.connect;

  /* Check to see if SAP connect is blocked/disabled based on the NV */
  if (qmi_uim_global_ptr->sap_sec_restr_supported &&
      op_type == UIM_SAP_OP_CONNECT_V01)
  {
    errval = QMI_ERR_ACCESS_DENIED;
    UIM_MSG_ERR_0("SAP request rejected due to security restrictions");
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SAP_CONNECTION,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Perform respective connection operation */
  switch (op_type)
  {
    case UIM_SAP_OP_DISCONNECT_V01:
      /* Call MMGSDI function to disconnect */
      if (qmi_uim_global_ptr->sap_info[slot_idx].is_enabled)
      {
        mmgsdi_status = mmgsdi_sap_disconnect(
                              qmi_uim_global_ptr->mmgsdi_client_id,
                              tlv_data[0].data.sap_connection.slot,
                              MMGSDIBT_SERVER,
                              tlv_data[1].data.sap_disconnect_mode.mode,
                              qmi_uimi_mmgsdi_callback,
                              (mmgsdi_client_data_type)cb_userdata_ptr);
        /* Convert MMGSDI status in QMI code */
        errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

        /* Send SAP indication with disconnected successfully to the client if
           the queuing of the SAP disconnect is successful. We do this as it
           is not expected that the SAP disconnection would fail. This allows
           to better handle the card error event and disconnection CNF from MMGSDI */
        if(errval == QMI_ERR_NONE)
        {
          /* Simulate SAP state as DISCONNECTED and send the SAP indication
             to all the SAP registered clients */
          qmi_uim_global_ptr->sap_info[slot_idx].sap_state = MMGSDI_SAP_DISCONNECTED;
          qmi_uim_send_sap_status_indication_to_all(tlv_data[0].data.sap_connection.slot);

          /* Set is_enabled flag to FALSE after sending SAP disconnected
             indication to clients */
          qmi_uim_global_ptr->sap_info[slot_idx].is_enabled = FALSE;
        }
      }
      else
      {
        errval = QMI_ERR_NO_EFFECT;
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;

    case UIM_SAP_OP_CONNECT_V01:
      /* Call MMGSDI function to connect */
      if (!qmi_uim_global_ptr->sap_info[slot_idx].is_enabled)
      {
        /* We use the condition from connection condtion TLV, if present.
           If TLV is not present, we default to either voice or data */
        mmgsdi_sap_conn_condition_enum_type conn_condition =
          tlv_data[3].present ? tlv_data[3].data.sap_connection_condtion.condition :
                                MMGSDI_SAP_CONN_COND_BLOCK_VOICE_OR_DATA;

        /* Get client state pointer */
        qmi_uimi_client_state_type * client_sp = (qmi_uimi_client_state_type *) cl_sp;

        mmgsdi_status = mmgsdi_sap_connect_ext(
                            qmi_uim_global_ptr->mmgsdi_client_id,
                            tlv_data[0].data.sap_connection.slot,
                            MMGSDIBT_SERVER,
                            conn_condition,
                            qmi_uimi_mmgsdi_callback,
                            (mmgsdi_client_data_type)cb_userdata_ptr);
        /* Convert MMGSDI status in QMI code */
        errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

        /* Update userdata with the intermediate get response info */
        if (tlv_data[2].present && (errval == QMI_ERR_NONE))
        {
          client_sp->uim_info.sap_intermediate_get_resp =
            tlv_data[2].data.sap_intermediate_get_resp.value;
        }
        else
        {
          client_sp->uim_info.sap_intermediate_get_resp = FALSE;
        }
      }
      else
      {
        errval = QMI_ERR_NO_EFFECT;
        mmgsdi_status = MMGSDI_ERROR;
      }
      break;

    case UIM_SAP_OP_CHECK_STATUS_V01:
      /* Send a synchronous reponse for check status */
      if (FALSE == qmi_uim_response_sap_state(
                      UIMI_TLV_RESP_TAG_OPTIONAL_1,
                      &response,
                      qmi_uim_global_ptr->sap_info[slot_idx].is_enabled,
                      qmi_uim_global_ptr->sap_info[slot_idx].sap_state))
      {
        errval = QMI_ERR_NO_MEMORY;
        dsm_free_packet(&response);
      }
      /* Fake setting just to make sure cb_userdata_ptr is cleaned */
      mmgsdi_status = MMGSDI_ERROR;
      break;

    default:
      /* At this point, this shouldnt happen */
      mmgsdi_status = MMGSDI_ERROR;
      errval = QMI_ERR_INVALID_ARG;
      break;
  }

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 4);

  result = (errval == QMI_ERR_NONE ? QMI_RESULT_SUCCESS
                                   : QMI_RESULT_FAILURE);

  /* Only successful connect/disconnect requests have pending response */
  if ((errval == QMI_ERR_NONE) &&
      (op_type != UIM_SAP_OP_CHECK_STATUS_V01))
  {
    return QMI_SVC_RESPONSE_PENDING;
  }

  retval = qmi_svc_put_result_tlv(&response, result, errval);
  CHECK_RETVAL();

  return response;
} /* qmi_uimi_sap_connection */


/*===========================================================================
  FUNCTION QMI_UIMI_SAP_REQUEST()

  DESCRIPTION
    Send various SAP requests to the card

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
dsm_item_type * qmi_uimi_sap_request
(
  void *            sp,
  void *            cmd_buf_p,
  void *            cl_sp,
  dsm_item_type **  sdu_in
)
{
  qmi_error_e_type                errval          = QMI_ERR_NONE;
  mmgsdi_return_enum_type         mmgsdi_status   = MMGSDI_SUCCESS;
  uint8                           slot_idx        = 0;
  int32                           channel_id      = 0;
  qmi_uimi_userdata_type*         cb_userdata_ptr = NULL;
  qmi_uim_tlv_item_type           tlv_data[2];

  ASSERT(sp && cmd_buf_p && cl_sp && sdu_in && qmi_uim_global_ptr);

  memset(tlv_data, 0, sizeof(tlv_data));
  tlv_data[0].type      = UIMI_TLV_TYPE_SAP_REQUEST;
  tlv_data[0].tag       = UIMI_TLV_TAG_MANDATORY_1;
  tlv_data[1].type      = UIMI_TLV_TYPE_APDU_REQUEST;
  tlv_data[1].tag       = UIMI_TLV_TAG_OPTIONAL_1;

  /* Parse TLV */
  errval = qmi_uim_parse_request(sdu_in, tlv_data, 2);
  if (errval != QMI_ERR_NONE)
  {
    goto send_result;
  }

  if (uimqmi_mmgsdi_slot_to_slot_index(tlv_data[0].data.sap_request.slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  if(slot_idx >= QMI_UIM_MAX_CARD_COUNT ||
     slot_idx >= qmi_uim_global_ptr->card_state.num_slots)
  {
    errval = QMI_ERR_INVALID_ARG;
    goto send_result;
  }

  /* Continue only if SAP connection is up */
  if (!qmi_uim_global_ptr->sap_info[slot_idx].is_enabled)
  {
    errval = QMI_ERR_INTERNAL;
    goto send_result;
  }

  /* Error if no send_apdu TLV present for send APDU request */
  if((tlv_data[0].data.sap_request.request == UIM_SAP_REQUEST_SEND_APDU_V01) &&
     (!tlv_data[1].present))
  {
    UIM_MSG_ERR_0("send APDU request requires send_apdu TLV");
    errval = QMI_ERR_MISSING_ARG;
    goto send_result;
  }

  /* Create userdata */
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_SAP_REQUEST,
                                                 cmd_buf_p,
                                                 NULL,
                                                 ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    errval = QMI_ERR_NO_MEMORY;
    goto send_result;
  }

  /* Perform respective request operation */
  switch (tlv_data[0].data.sap_request.request)
  {
    case UIM_SAP_REQUEST_GET_ATR_V01:
      mmgsdi_status = mmgsdi_sap_get_atr(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        tlv_data[0].data.sap_request.slot,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr);
      break;
    case UIM_SAP_REQUEST_SEND_APDU_V01:
      /* Check if SAP request is restricted */
      if (qmi_uim_is_sap_request_rejected(tlv_data[1].data.send_apdu.apdu_data))
      {
        mmgsdi_status = MMGSDI_ACCESS_DENIED;
      }
      else
      {
        mmgsdi_status = mmgsdi_sap_send_apdu(
                          qmi_uim_global_ptr->mmgsdi_client_id,
                          tlv_data[0].data.sap_request.slot,
                          channel_id,
                          tlv_data[1].data.send_apdu.apdu_data,
                          qmi_uimi_mmgsdi_callback,
                          (mmgsdi_client_data_type)cb_userdata_ptr);

        /* Also save the INS byte if needed in userdata */
        if ((tlv_data[1].data.send_apdu.apdu_data.data_ptr) &&
            (tlv_data[1].data.send_apdu.apdu_data.data_len >= 2))
        {
          cb_userdata_ptr->data.sap_request.ins_value =
              tlv_data[1].data.send_apdu.apdu_data.data_ptr[1];
        }
      }
      break;
    case UIM_SAP_REQUEST_POWER_SIM_OFF_V01:
      mmgsdi_status = mmgsdi_sap_power_off(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        tlv_data[0].data.sap_request.slot,
                        MMGSDIBT_SERVER,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr);
      break;
    case UIM_SAP_REQUEST_POWER_SIM_ON_V01:
      mmgsdi_status = mmgsdi_sap_power_on(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        tlv_data[0].data.sap_request.slot,
                        MMGSDIBT_SERVER,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr);
      break;
    case UIM_SAP_REQUEST_RESET_SIM_V01:
      mmgsdi_status = mmgsdi_sap_reset(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        tlv_data[0].data.sap_request.slot,
                        MMGSDIBT_SERVER,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr);
      break;
    case UIM_SAP_REQUEST_CARD_READER_STATUS_V01:
      mmgsdi_status = mmgsdi_sap_card_reader_status(
                        qmi_uim_global_ptr->mmgsdi_client_id,
                        tlv_data[0].data.sap_request.slot,
                        MMGSDIBT_SERVER,
                        qmi_uimi_mmgsdi_callback,
                        (mmgsdi_client_data_type)cb_userdata_ptr);
      break;
    default:
      /* At this point, this shouldnt happen */
      mmgsdi_status = MMGSDI_ERROR;
      break;
  }

  /* Convert MMGSDI status in QMI code */
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);

  /* Free userdata in case of error */
  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);

send_result:
  /* Free the buffer used for data */
  qmi_uim_util_free_tlv_content(tlv_data, 2);

  return qmi_uimi_util_populate_response(errval,
                                         FALSE,
                                         0,
                                         0);
} /* qmi_uimi_sap_request */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_SAP_STATUS_INDICATION()

  DESCRIPTION
    This function sends a SAP status indication to a specific client

  PARAMETERS
    clid   : Client ID
    slot   : Card slot

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
static void qmi_uim_send_sap_status_indication
(
  uint8                         clid,
  mmgsdi_slot_id_enum_type      slot
)
{
  dsm_item_type* indication = NULL;
  uint8          slot_idx   = 0;

  ASSERT(clid != QMI_SVC_CLID_UNUSED && qmi_uim_global_ptr);

  if (uimqmi_mmgsdi_slot_to_slot_index(slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return;
  }

  if(slot_idx >= QMI_UIM_MAX_CARD_COUNT ||
     slot_idx >= qmi_uim_global_ptr->card_state.num_slots)
  {
    UIM_MSG_ERR_1("Invalid slot : 0x%x", slot);
    return;
  }

  /* Insert sap state & slot item in TLV */
  if (FALSE == qmi_uim_response_sap_event(
                   UIMI_TLV_RESP_TAG_OPTIONAL_1,
                   &indication,
                   qmi_uim_global_ptr->sap_info[slot_idx].is_enabled,
                   qmi_uim_global_ptr->sap_info[slot_idx].sap_state,
                   slot))
  {
    /* Ignore error... there is nothing we can do about it.
       The event will not be delivered to the client */
    dsm_free_packet(&indication);
    return;
  }

  /* Send indication to the client */
  (void)uimqmi_send_indication(clid,
                               (uint16)UIMI_CMD_VAL_SAP_CONNECTION_IND,
                               indication,
                               QMUX_SERVICE_UIM);
} /* qmi_uim_send_sap_status_indication */


/*===========================================================================
  FUNCTION QMI_UIM_SEND_SAP_STATUS_INDICATION_TO_ALL()

  DESCRIPTION
    This function sends a SAP status indication to all QMI_UIM clients

  PARAMETERS
    slot_id : Card slot

  RETURN VALUE
    None.

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_uim_send_sap_status_indication_to_all
(
  mmgsdi_slot_id_enum_type    slot_id
)
{
  uint8                         j           = 0;
  qmi_uimi_client_state_type  * cl_sp       = NULL;

  if(qmi_uim_state_ptr == NULL)
  {
    return;
  }

  for (j = 0; j < UIMQMI_SVC_MAX_CLIDS; j++)
  {
    cl_sp = (qmi_uimi_client_state_type*)qmi_uim_state_ptr->client[j];

    if ( cl_sp != NULL &&
         cl_sp->common.clid != QMI_SVC_CLID_UNUSED &&
         cl_sp->uim_info.reg_event_type.sap_events )
    {
      /* Send indication to specific client */
      qmi_uim_send_sap_status_indication(cl_sp->common.clid,
                                         slot_id);
    }
  }
} /* qmi_uim_send_sap_status_indication_to_all */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_SAP_EVT()

  DESCRIPTION
    This function processes the SAP events from MMGSDI

  PARAMETERS
    event_ptr: pointer to event

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_process_sap_evt
(
  const mmgsdi_event_data_type * event_ptr
)
{
  boolean                       result      = FALSE;
  uint8                         slot_idx    = 0;
  mmgsdi_slot_id_enum_type      slot        = MMGSDI_SLOT_1;
  mmgsdi_sap_state_enum_type    sap_state   = MMGSDI_MAX_SAP_STATE_ENUM;

  ASSERT(event_ptr && qmi_uim_global_ptr);

  switch (event_ptr->evt)
  {
    case MMGSDI_SAP_CONNECT_EVT:
      slot = event_ptr->data.sap_connect.slot;
      sap_state = event_ptr->data.sap_connect.connect_state;
      break;
    case MMGSDI_SAP_DISCONNECT_EVT:
      slot = event_ptr->data.sap_disconnect.slot;
      sap_state = event_ptr->data.sap_disconnect.disconnect_state;
      break;
    case MMGSDI_CARD_ERROR_EVT:
      slot = event_ptr->data.card_error.slot;
      break;
    case MMGSDI_CARD_REMOVED_EVT:
      slot = event_ptr->data.card_removed.slot;
      break;
    default:
      return result;
  }

  UIM_MSG_HIGH_3("Event: 0x%x, slot=0x%x, sap state=0x%x",
                 event_ptr->evt, slot, sap_state);

  if (uimqmi_mmgsdi_slot_to_slot_index(slot,
                                       &slot_idx,
                                       qmi_uim_global_ptr->card_state.num_slots) != QMI_ERR_NONE)
  {
    return result;
  }

  if(slot_idx >= QMI_UIM_MAX_CARD_COUNT ||
     slot_idx >= qmi_uim_global_ptr->card_state.num_slots)
  {
    return result;
  }

  switch (event_ptr->evt)
  {
    case MMGSDI_SAP_CONNECT_EVT:
    case MMGSDI_SAP_DISCONNECT_EVT:
      /* Update global SAP state */
      if (qmi_uim_global_ptr->sap_info[slot_idx].sap_state != sap_state)
      {
        result = TRUE;
        qmi_uim_global_ptr->sap_info[slot_idx].sap_state = sap_state;

        /* After connection errors have been posted to clients, we reset our flag */
        if (sap_state == MMGSDI_SAP_CONNECTING ||
            sap_state == MMGSDI_SAP_CONNECTED  ||
            sap_state == MMGSDI_SAP_DISCONNECTING)
        {
          qmi_uim_global_ptr->sap_info[slot_idx].is_enabled = TRUE;
        }
        else
        {
          qmi_uim_global_ptr->sap_info[slot_idx].is_enabled = FALSE;
        }
      }
      break;
    case MMGSDI_CARD_ERROR_EVT:
    case MMGSDI_CARD_REMOVED_EVT:
      /* Send SAP disconnect indication only if we are connected and the card
         error info is not due to SAP connected */
      if ((qmi_uim_global_ptr->sap_info[slot_idx].sap_state == MMGSDI_SAP_CONNECTING ||
           qmi_uim_global_ptr->sap_info[slot_idx].sap_state == MMGSDI_SAP_CONNECTED ||
           qmi_uim_global_ptr->sap_info[slot_idx].sap_state == MMGSDI_SAP_DISCONNECTING) &&
          qmi_uim_global_ptr->sap_info[slot_idx].is_enabled                              &&
          event_ptr->data.card_error.info != MMGSDI_CARD_ERR_SAP_CONNECTED)
      {
        /* Update global SAP state and send the indication*/
        qmi_uim_global_ptr->sap_info[slot_idx].sap_state = MMGSDI_SAP_DISCONNECTED;
        qmi_uim_send_sap_status_indication_to_all(slot);

        /* Update is_enabled global after sending SAP indication */
        qmi_uim_global_ptr->sap_info[slot_idx].is_enabled = FALSE;
      }
      break;
    default:
      break;
  }

  return result;
} /* qmi_uim_process_sap_evt */

