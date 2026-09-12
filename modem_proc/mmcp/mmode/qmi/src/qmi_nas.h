#ifndef _DS_QMI_NAS_H
#define _DS_QMI_NAS_H
/*===========================================================================

                         D S _ Q M I _ N A S . H

DESCRIPTION

 The Data Services Qualcomm Wireless Data Services MSM Interface header file.

EXTERNALIZED FUNCTIONS

  qmi_nas_init()
    Register the Wireless Data Service with QMUX for all applicable QMI links.

  qmi_nas_gen_serving_system_ind_cb()
    Call back function called when the servign system changes.

  qmi_nas_rssi_change_cb()
    Callback to be called when signal strength changes.


Copyright (c) 2004-2009 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/qmi/src/qmi_nas.h#2 $ $DateTime: 2022/09/01 04:52:39 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
08/10/11    sm     Moving qmi_nas.h to modem/api/mmode directory
===========================================================================*/
#include "modem_mem.h"
#include "mmgsdilib_common.h"
#include "sys.h"
#include "qm_util.h"

#define QMI_NAS_MEM_ALLOC(siz)  QM_MEM_ALLOC(siz)
#define QMI_NAS_MEM_FREE(ptr)  QM_MEM_FREE(ptr)

/*===========================================================================
  FUNCTION QMI_NAS_PROCESS_MMGSDI_OPERATION_COMPLETE()

  DESCRIPTION
    The demultiplexing routine within NAS subsystem to send out pending
    responses based on MMGSDI confirmations.

  PARAMETERS
    cnf_type : Type of MMGSDI confirmation
    cnf_data : Response data

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_nas_process_mmgsdi_operation_complete
(
  mmgsdi_cnf_enum_type    cnf_type,
  mmgsdi_cnf_type        *cnf_data
);
/*=============================================================================
FUNCTION: QMI_NAS_GET_HOME_MCC_MNC

DESCRIPTION: Reads the MCC and MNC from SIM.

PARAMETERS:
  None

RETURN VALUE:
  None

DEPENDENCIES:
  None

SIDE EFFECTS:
  None
=============================================================================*/
void qmi_nas_get_home_mcc_mnc
(
  void
);

/*===========================================================================
  FUNCTION QMI_NASI_GENERATE_GET_CSP_PLMN_MODE_BIT_RESP()

  DESCRIPTION
    Send response for QMI_NAS_GET_CSP_PLMN_MODE_BIT

  PARAMETERS
    user_data   : user data
    ph_cmd_err  : error type

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/

void qmi_nasi_generate_get_csp_plmn_mode_bit_resp( void );

/*===========================================================================
  FUNCTION QMI_NAS_MAP_MODE_PREF_QMI_TO_CM()

  DESCRIPTION
    Map uint16 QMI mode pref to cm_mode_pref_e_type

  PARAMETERS
    qmi_val: mode pref value from QMI

  RETURN VALUE
    mode pref value in CM format (bitmask). Return CM_MODE_PREF_NONE
    for error

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern cm_mode_pref_e_type qmi_nas_map_mode_pref_qmi_to_cm (uint16 qmi_val);

/*===========================================================================
  FUNCTION QMI_NAS_MAP_MODE_PREF_CM_TO_QMI()

  DESCRIPTION
    Map cm_mode_pref_e_type to uint16 QMI mode pref

  PARAMETERS
    cm_val: mode pref value from CM

  RETURN VALUE
    mode pref value in QMI format (bitmask). Return QMI_NAS_MODE_PREF_NONE
    for error

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
extern uint16 qmi_nas_map_mode_pref_cm_to_qmi (cm_mode_pref_e_type cm_val);

/*===========================================================================
  FUNCTION qmi_nasi_generate_access_barring_status_ind()

  DESCRIPTION
    Generate the nr5g cell barring status indication

  PARAMETERS
    msg_p  Pointer to MSGR data for Cell barring status

  RETURN VALUE
    None

  DEPENDENCIES
    None

  SIDE EFFECTS
    None
===========================================================================*/
void qmi_nasi_generate_access_barring_status_ind
(
  mmode_qmi_nas_access_barring_status_ind_msg_type *barring_info,
  sys_sys_mode_e_type sys_mode,
  sys_modem_as_id_e_type asubs_id
);

#endif /* _DS_QMI_NAS_H */

