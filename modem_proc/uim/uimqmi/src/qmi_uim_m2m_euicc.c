/*==============================================================================

                              Q M I _ U I M _ M 2 M _ E U I C C  _ C


DESCRIPTION
  The Data Services Qualcomm MSM Interface Device Management Services source
  file.

Copyright (c) 2017, 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
==============================================================================*/
/*==============================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_m2m_euicc.c#2 $

when        who    what, where, why
--------    ---    -------------------------------------------------------------
08/17/20    vgd    Added logic to get EID using both M2M and LPA methods
04/19/17    ku     GET EID for M2M cards
==============================================================================*/

/*==============================================================================

                          INCLUDE FILES FOR MODULE

==============================================================================*/

#include "uim_variation.h"
#include "mmgsdilib.h"
#include "uim_msg.h"
#include "qmi_uim_util.h"
#include "amssassert.h"
#include "qmi_uim.h"
#include "modem_mem.h"
#include "qmi_uim_internal.h"
#include <stringl/stringl.h>

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
)
{
  PACKED struct PACKED_POST
  {
    uint8 eid_len;
    uint8 eid[QMI_UIM_EID_LEN_V01];
  } tlv_data_eid;

  ASSERT(response_pptr);

  if(get_eid_resp_ptr == NULL || get_eid_resp_ptr->data_len > QMI_UIM_EID_LEN_V01)
  {
    return FALSE;
  }

  tlv_data_eid.eid_len = get_eid_resp_ptr->data_len;
  if(get_eid_resp_ptr->data_len > 0)
  {
    (void)memscpy((void *)tlv_data_eid.eid,
                  sizeof(tlv_data_eid.eid),
                  (void *)get_eid_resp_ptr->data_ptr,
                  get_eid_resp_ptr->data_len);
  }

  return qmi_svc_put_param_tlv(response_pptr,
                               tlv_id,
                               sizeof(uint8) + tlv_data_eid.eid_len,
                               (void *)&tlv_data_eid);
} /* qmi_uim_response_eid */


/*===========================================================================
  FUNCTION QMI_UIMI_EXECUTE_M2M_GET_EID

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
)
{
  qmi_error_e_type         errval             = QMI_ERR_NONE;
  qmi_uimi_userdata_type*  cb_userdata_ptr    = NULL;
  mmgsdi_return_enum_type  mmgsdi_status      = MMGSDI_SUCCESS;
  
  cb_userdata_ptr = qmi_uim_util_get_cb_userdata(UIMI_CMD_VAL_GET_EID,
                                                 cmd_buf_p,
                                                 NULL,
                                                ((qmi_uimi_client_state_type *)cl_sp)->common.clid);
  if (cb_userdata_ptr == NULL)
  {
    return QMI_ERR_NO_MEMORY;
  }

  /* Execute M2M function to get EID */
  mmgsdi_status = mmgsdi_get_m2m_eid(mmgsdi_slot_id,
                                     qmi_uimi_mmgsdi_callback,
                                     (mmgsdi_client_data_type)cb_userdata_ptr);

  QMI_UIM_UTIL_FREE_USERDATA_IF_ERROR(mmgsdi_status, cb_userdata_ptr);
  
  errval = qmi_uim_util_convert_mmgsdi_status_to_errval(mmgsdi_status);
  
  return errval;
}/* qmi_uimi_execute_m2m_get_eid*/

