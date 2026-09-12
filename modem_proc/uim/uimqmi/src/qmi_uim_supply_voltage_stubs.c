/*===========================================================================

        Q M I _ U I M _ S U P P L Y _ V O L T A G E _ S T U B S . C

DESCRIPTION

 This file contains the stub functions for Supply Voltage functionality

Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_supply_voltage_stubs.c#1 $ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
07/26/16    sp     Move qmi_uimi_ functions to corresponding files
06/16/16    vdc    Split QMI UIM parsing files for supporting IOE
05/20/16    ar     Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "qmi_uim_supply_voltage.h"
#include "uim_v.h"

/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/

/*===========================================================================
  FUNCTION QMI_UIM_ALL_REG_CLIENTS_ACK_VCC_DEACTIVATION()

  DESCRIPTION
    Stub function

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
  (void)mmgsdi_slot;
  return QMI_ERR_NOT_SUPPORTED;
} /* qmi_uim_all_reg_clients_ack_vcc_deactivation */


/*===========================================================================
  FUNCTION QMI_UIM_PROCESS_SUPPLY_VOLTAGE_IND()

  DESCRIPTION
    Stub function

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
  (void)uim_message_ptr;
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
  (void)slot;
  (void)ldo_state;
  (void)uim_callback_ptr;
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
  boolean        retval   = TRUE;
  dsm_item_type *response = NULL;

  (void)sp;
  (void)cmd_buf_p;
  (void)cl_sp;
  (void)sdu_in;

  retval = qmi_svc_put_result_tlv(&response, QMI_RESULT_FAILURE, QMI_ERR_NOT_SUPPORTED);
  CHECK_RETVAL();

  return response;
}/* qmi_uimi_supply_voltage */

