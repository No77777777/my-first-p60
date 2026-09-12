/*==============================================================================

                 Q M I _ U I M _ S A P _ P A R S I N G _ S T U B S _ C

DESCRIPTION
  The Data Services Qualcomm MSM Interface Device Management Services source
  file.

Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_sap_parsing_stubs.c#1 $

when        who    what, where, why
--------    ---    -------------------------------------------------------------
06/22/16    sp     Initial Revision
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include"qmi_uim_sap_parsing.h"

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
  (void)tlv_id;
  (void)response_pptr;
  (void)is_enabled;
  (void)sap_state;
  return FALSE;
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
  (void)tlv_id;
  (void)response_pptr;
  (void)is_enabled;
  (void)sap_state;
  (void)slot;
  return FALSE;
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
  (void)sdu_in;
  (void)len;
  (void)sap_connection_info_ptr;
  return QMI_ERR_NOT_SUPPORTED;
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
  (void)sdu_in;
  (void)len;
  (void)sap_request_info_ptr;
  return QMI_ERR_NOT_SUPPORTED;
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
  (void)sdu_in;
  (void)len;
  (void)sap_conn_condition_ptr;
  return QMI_ERR_NOT_SUPPORTED;
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
  (void)sdu_in;
  (void)len;
  (void)sap_disconnection_info_ptr;
  return QMI_ERR_NOT_SUPPORTED;
} /* qmi_uim_tlv_sap_disconnect_mode */

