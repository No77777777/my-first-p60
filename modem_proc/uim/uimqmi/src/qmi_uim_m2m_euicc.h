#ifndef QMI_UIM_M2M_EUICC_H
#define QMI_UIM_M2M_EUICC_H
/*==============================================================================

                         Q M I _ U I M _ M 2 M _ E U I C C _ H

DESCRIPTION

 The Data Services Qualcomm MSM Interface Device Management Services source
 file.

Copyright (c) 2017, 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_m2m_euicc.h#2 $

when        who    what, where, why
--------    ---    ------------------------------------------------------------
08/17/20    vgd    Added logic to get EID using both M2M and LPA methods
04/19/17    ku     GET EID for M2M cards
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/
#include "uim_p.h"
#include "mmgsdilib_common.h"
#include "qmi_uim_internal.h"

/*==============================================================================

                             EXTERNAL FUNCTIONS

==============================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_RESPONSE_EID()

  DESCRIPTION
    Composes the TLV for EID retrieved from MMGSDI

  PARAMETERS
    tlv_id              : id to be used for the TLV
    response_pptr       : output sdu
    get_eid_resp_ptr    : profile confirmation pointer

  RETURN VALUE
    TRUE on success. FALSE on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
boolean qmi_uim_response_eid
(
  byte                             tlv_id,
  dsm_item_type                  **response_pptr,
  const mmgsdi_static_data_type   *get_eid_resp_ptr
);

/*===========================================================================
  FUNCTION QMI_UIMI_EXECUTE_MMGSDI_GET_EID

  DESCRIPTION
    Generic function to send GET_EID command TO MMGSDI.

  PARAMETERS
    mmgsdi_slot_id : Slot Id for GET_EID command.
    cmd_buf_p      : ptr to cmd buffer
    cl_sp          : client state pointer

  RETURN VALUE
    QMI_ERR_NONE on success. Other error code on failure

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
qmi_error_e_type qmi_uimi_execute_m2m_get_eid
(
  mmgsdi_slot_id_enum_type       mmgsdi_slot_id,
  void *                         cmd_buf_p,
  void *                         cl_sp
);

#endif /* QMI_UIM_M2M_EUICC_H*/
