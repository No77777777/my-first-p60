/*==============================================================================

                         Q M I _ U I M _ S A P _ P A R S I N G . C

DESCRIPTION
  The Data Services Qualcomm MSM Interface Device Management Services source
  file.

Copyright (c) 2016-2017 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sap_parsing.c#1 $

when        who    what, where, why
--------    ---    -------------------------------------------------------------
01/03/17    dd     Adding offtarget support
11/04/16    me     Combined multi-line ASSERT in one line
06/22/16    sp     Initial Revision
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "qmi_uim_cat_common.h"
#include "amssassert.h"

#include "qmi_uim_sap_parsing.h"
#include "qmi_uim_parsing.h"
#include "qmi_uim_util.h"

/* These includes need to always be at bottom - offtarget use only */
#ifdef FEATURE_UIM_TEST_FRAMEWORK
#error code not present
#endif /* FEATURE_UIM_TEST_FRAMEWORK */

/*==============================================================================

                             EXTERNAL FUNCTIONS

==============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SAP_STATE()

  DESCRIPTION
    Composes the TLV content of SAP state

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    is_enabled    : flag to indicate if SAP is enabled
    sap_state     : SAP state

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_sap_state
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  boolean                         is_enabled,
  mmgsdi_sap_state_enum_type      sap_state
)
{
  PACKED struct PACKED_POST
  {
    uint8 sap_state;
  } tlv_data;

  ASSERT(response_pptr);

  if (!is_enabled)
  {
    tlv_data.sap_state = UIM_SAP_STATE_NOT_ENABLED_V01;
  }
  else
  {
    switch(sap_state)
    {
      case MMGSDI_SAP_CONNECTING:
        tlv_data.sap_state = UIM_SAP_STATE_CONNECTING_V01;
        break;
      case MMGSDI_SAP_CONNECTED:
        tlv_data.sap_state = UIM_SAP_STATE_CONNECTED_SUCCESSFULLY_V01;
        break;
      case MMGSDI_SAP_CONNECT_FAIL:
      case MMGSDI_SAP_DISCONNECT_FAIL:
        tlv_data.sap_state = UIM_SAP_STATE_CONNECTION_ERROR_V01;
        break;
      case MMGSDI_SAP_DISCONNECTING:
        tlv_data.sap_state = UIM_SAP_STATE_DISCONNECTING_V01;
        break;
      case MMGSDI_SAP_DISCONNECTED:
        tlv_data.sap_state = UIM_SAP_STATE_DISCONNECTED_SUCCESSFULLY_V01;
        break;
      default:
        return FALSE;
    }
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_sap_state */


/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_SAP_EVENT

  DESCRIPTION
    Composes the TLV content of SAP indication

  PARAMETERS
    tlv_id        : id to be used for the TLV
    response_pptr : output sdu
    is_enabled    : flag to indicate if SAP is enabled
    sap_state     : SAP state
    slot_idx      : slot subscript

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_sap_event
(
  byte                            tlv_id,
  dsm_item_type **                response_pptr,
  boolean                         is_enabled,
  mmgsdi_sap_state_enum_type      sap_state,
  mmgsdi_slot_id_enum_type        slot
)
{
  PACKED struct PACKED_POST
  {
    uint8 sap_state;
    uint8 slot;
  } tlv_data;

  ASSERT(response_pptr);

  /* Update sap state */
  if (!is_enabled)
  {
    tlv_data.sap_state = UIM_SAP_STATE_NOT_ENABLED_V01;
  }
  else
  {
    switch(sap_state)
    {
      case MMGSDI_SAP_CONNECTING:
        tlv_data.sap_state = UIM_SAP_STATE_CONNECTING_V01;
        break;
      case MMGSDI_SAP_CONNECTED:
        tlv_data.sap_state = UIM_SAP_STATE_CONNECTED_SUCCESSFULLY_V01;
        break;
      case MMGSDI_SAP_CONNECT_FAIL:
      case MMGSDI_SAP_DISCONNECT_FAIL:
        tlv_data.sap_state = UIM_SAP_STATE_CONNECTION_ERROR_V01;
        break;
      case MMGSDI_SAP_DISCONNECTING:
        tlv_data.sap_state = UIM_SAP_STATE_DISCONNECTING_V01;
        break;
      case MMGSDI_SAP_DISCONNECTED:
        tlv_data.sap_state = UIM_SAP_STATE_DISCONNECTED_SUCCESSFULLY_V01;
        break;
      default:
        return FALSE;
    }
  }

  /* Update slot state */
  switch(slot)
  {
    case MMGSDI_SLOT_1:
      tlv_data.slot = UIM_SLOT_1_V01;
      break;
    case MMGSDI_SLOT_2:
      tlv_data.slot = UIM_SLOT_2_V01;
      break;
    case MMGSDI_SLOT_3:
      tlv_data.slot = UIM_SLOT_3_V01;
      break;
    default:
      return FALSE;
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(tlv_data),
                               (void *)&tlv_data);
} /* qmi_uim_response_sap_event */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SAP_CONNECT()

  DESCRIPTION
    Parses the TLV item of SAP connect

  PARAMETERS
    sdu_in                   : input sdu
    len                      : length of the TLV item
    sap_connection_info_ptr  : output with SAP connection request

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_sap_connect
(
  dsm_item_type **                   sdu_in,
  uint16                             len,
  qmi_uim_tlv_sap_connect_type     * sap_connection_info_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8   connect;
    uint8   slot;
  } tlv_data;

  ASSERT(sdu_in && sap_connection_info_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if(tlv_data.connect > UIM_SAP_OP_CHECK_STATUS_V01)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* Connection operation */
  sap_connection_info_ptr->connect = (uim_sap_connect_op_enum_v01)tlv_data.connect;

  /* Derive Slot id and return */
  return qmi_uim_convert_qmi_slot_to_mmgsdi_slot(tlv_data.slot,
                                                 &(sap_connection_info_ptr->slot));
} /* qmi_uim_tlv_sap_connect */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SAP_REQUEST()

  DESCRIPTION
    Parses the TLV item of SAP request

  PARAMETERS
    sdu_in                      : input sdu
    len                         : length of the TLV item
    sap_request_info_ptr        : output with SAP request info

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_sap_request
(
  dsm_item_type **                        sdu_in,
  uint16                                  len,
  qmi_uim_tlv_sap_request_type          * sap_request_info_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8   request;
    uint8   slot;
  } tlv_data;

  ASSERT(sdu_in && sap_request_info_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  if(tlv_data.request > UIM_SAP_REQUEST_CARD_READER_STATUS_V01)
  {
    return QMI_ERR_INVALID_ARG;
  }

  /* SAP Request */
  sap_request_info_ptr->request = (uim_sap_request_enum_v01)tlv_data.request;

  /* Derive Slot id and return */
  return qmi_uim_convert_qmi_slot_to_mmgsdi_slot(tlv_data.slot,
                                                 &(sap_request_info_ptr->slot));
} /* qmi_uim_tlv_sap_request */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SAP_CONNECTION_CONDITION()

  DESCRIPTION
    Parses the TLV item of SAP connection condiion

  PARAMETERS
    sdu_in                   : input sdu
    len                      : length of the TLV item
    sap_conn_condition_ptr   : output with value of SAP connection condition

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_sap_connection_condition
(
  dsm_item_type **                            sdu_in,
  uint16                                      len,
  qmi_uim_tlv_sap_conn_condition_type       * sap_conn_condition_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint32  conn_condition;
  } tlv_data;

  ASSERT(sdu_in && sap_conn_condition_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Read data */
  switch (tlv_data.conn_condition)
  {
    case UIM_SAP_CONNECTION_COND_BLOCK_VOICE_OR_DATA_V01:
      sap_conn_condition_ptr->condition = MMGSDI_SAP_CONN_COND_BLOCK_VOICE_OR_DATA;
      break;
    case UIM_SAP_CONNECTION_COND_BLOCK_DATA_V01:
      sap_conn_condition_ptr->condition = MMGSDI_SAP_CONN_COND_BLOCK_DATA;
      break;
    case UIM_SAP_CONNECTION_COND_BLOCK_VOICE_V01:
      sap_conn_condition_ptr->condition = MMGSDI_SAP_CONN_COND_BLOCK_VOICE;
      break;
    case UIM_SAP_CONNECTION_COND_BLOCK_NONE_V01:
      sap_conn_condition_ptr->condition = MMGSDI_SAP_CONN_COND_BLOCK_NONE;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_sap_connection_condition */


/*===========================================================================
  FUNCTION QMI_UIM_TLV_SAP_DISCONNECT_MODE()

  DESCRIPTION
    Parses the TLV item of SAP disconnect mode

  PARAMETERS
    sdu_in                      : input sdu
    len                         : length of the TLV item
    sap_disconnection_info_ptr  : output with SAP disconnection mode

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uim_tlv_sap_disconnect_mode
(
  dsm_item_type **                        sdu_in,
  uint16                                  len,
  qmi_uim_tlv_sap_disconnect_mode_type  * sap_disconnection_info_ptr
)
{
  int32             temp;

  PACKED struct PACKED_POST
  {
    uint8   mode;
  } tlv_data;

  ASSERT(sdu_in && sap_disconnection_info_ptr);

  /* Check length */
  if (len != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Extract data */
  temp = dsm_pullup(sdu_in, (void*)&tlv_data, sizeof(tlv_data));
  if (temp != sizeof(tlv_data))
  {
    return QMI_ERR_MALFORMED_MSG;
  }

  /* Connection operation */
  switch (tlv_data.mode)
  {
    case UIM_SAP_DISCONNECT_IMMEDIATE_V01:
      sap_disconnection_info_ptr->mode = MMGSDIBT_DISCONNECT_IMMED;
      break;
    case UIM_SAP_DISCONNECT_GRACEFULL_V01:
      sap_disconnection_info_ptr->mode = MMGSDIBT_DISCONNECT_GRACE;
      break;
    default:
      return QMI_ERR_INVALID_ARG;
  }

  return QMI_ERR_NONE;
} /* qmi_uim_tlv_sap_disconnect_mode */

