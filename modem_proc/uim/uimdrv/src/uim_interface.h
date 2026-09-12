#ifndef UIM_INTERFACE_H
#define UIM_INTERFACE_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

FILE:       uim_interface.c
				  
OVERVIEW:
   This file consists declarations of wrappers for the client APIs that are invoked by UIM.

Copyright (c) 2016 - 2017 by QUALCOMM Technologies, Incorporated (QTI) and its licensors.
All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_interface.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/04/17   vr       Send SW1 SW2 in fetch response from uimdrv to GSTK
06/09/16   shr      Deprecate GSTK TAL
03/28/16   sam      Initial Draft

===========================================================================*/
#include "gstk_exp.h"
#include "gstk_p.h"
#include "estk.h"
#include "mmgsdilib_p.h"
#include "qmi_uim.h"
#include "qmi_uim_remote.h"


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_card_error
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_card_error
(
  uim_status_type error_condition,
  uim_slot_type slot,
  boolean temporary_error
);

/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_link_est_slot
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_link_est_slot
(
  uim_slot_link_established_data_type link_est_data,
  uim_slot_type                       slot_id
);


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_get_recovery_data_sync
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_get_recovery_data_sync
(
  uim_recovery_data_type *recovery_data_ptr,
  uim_slot_type           uim_slot_id
);


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_sim_busy
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_sim_busy
(
  boolean        sim_busy,
  uim_slot_type  slot
);


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_sim_pre_init
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_sim_pre_init
(
  uim_slot_type                       uim_slot_type
);


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_get_cached_nv_item_ext
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_get_cached_nv_item_ext 
(
  mmgsdi_cached_nv_item_data    *nv_item_data_ptr,
  uint8                          nv_index
);


/*============================================================================= 
  FUNCTION uim_interface_gstk_send_recovery_cmd
=============================================================================*/
gstk_status_enum_type uim_interface_gstk_send_recovery_cmd
(
  uim_slot_type                   slot,
  uimdrv_recovery_stage_enum_type stage
);


/*============================================================================= 
  FUNCTION uim_interface_gstk_uim_fetch_allow
=============================================================================*/
boolean uim_interface_gstk_uim_fetch_allow
(
  gstk_slot_id_enum_type slot
);


/*============================================================================= 
  FUNCTION uim_interface_gstk_send_fetch_cmd_report
=============================================================================*/
gstk_status_enum_type uim_interface_gstk_send_fetch_cmd_report
(
  uim_slot_type   slot,
  uint32          data_length,
  const uint8   * data,
  uim_sw1_type    sw1,
  uim_sw2_type    sw2
);


#ifdef FEATURE_ESTK
/*============================================================================= 
  FUNCTION uim_interface_estk_uim_is_bip_session_in_progress
=============================================================================*/
boolean uim_interface_estk_uim_is_bip_session_in_progress
(
  gstk_slot_id_enum_type slot_id
);
#endif


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_recovery_complete
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_recovery_complete
(
  const uim_slot_type slot
);


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_supply_voltage_ind
=============================================================================*/
void uim_interface_qmi_uim_supply_voltage_ind
(
  uim_slot_type                                     slot,
  uimdrv_qmi_indications_type                       ldo_state,
  uimdrv_qmi_power_management_callback_type         uim_callback_ptr
);


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_register
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_register
(
  qmi_uim_remote_evt_callback_type    * event_cb_ptr,
  qmi_uim_remote_slot_type              slot
);


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_power_down
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_power_down 
(
  qmi_uim_remote_slot_type                  slot,
  qmi_uim_remote_power_down_mode_enum_type  pdown_mode
);


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_get_recovery_data_sync
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_power_up 
(
  qmi_uim_remote_slot_type                slot,
  boolean                                 timeout_isvalid,
  uint32                                  timeout,
  boolean                                 vcc_isvalid,
  qmi_uim_remote_voltage_class_enum_type  vcc
);


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_disconnect
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_disconnect 
(
  qmi_uim_remote_slot_type            slot
);


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_connect
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_connect
(
  qmi_uim_remote_slot_type            slot
);


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_reset
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_reset
(
  qmi_uim_remote_slot_type            slot
);


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_send_apdu
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_send_apdu 
(
  qmi_uim_remote_slot_type               slot,
  void *                                 client_ref_ptr,
  uint16                                 cmd_apdu_len,
  const uint8 *                          cmd_apdu_ptr,
  qmi_uim_remote_apdu_callback_type    * rsp_apdu_cb_ptr
); 
#endif /*UIM_INTERFACE_H*/
