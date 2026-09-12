#ifndef QMI_UIM_SUPPLY_VOLTAGE_H
#define QMI_UIM_SUPPLY_VOLTAGE_H

/*================================================================================

DESCRIPTION


Copyright (c) 2016 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary

$Header: //components/rel/uim.mpss/6.1.0/uimqmi/src/qmi_uim_supply_voltage.h#1 $$ $DateTime: 2019/10/30 06:56:05 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
07/26/16   sp      Move qmi_uimi_ functions to corresponding files
06/16/16   vdc     Split QMI UIM parsing files for supporting IOE
05/20/16   ar      Initial version
=============================================================================*/

/*=============================================================================

                         INCLUDE FILES FOR MODULE

=============================================================================*/
#include "comdef.h"
#include "mmgsdilib_common.h"
#include "qmi_uim.h"
#include "qmi_svc_defs.h"
#include "qmi_uim_internal.h"

/*=============================================================================

                                  FUNCTIONS

=============================================================================*/

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
);

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
);

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
);

#endif /* QMI_UIM_SUPPLY_VOLTAGE_H */
