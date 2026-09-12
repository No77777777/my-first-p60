/*============================================================================
  FILE:         uim_interface.c

  OVERVIEW:
   This file consists of wrappers for the client APIs that are invoked by UIM.
   the wrappers are used to conver the physical ID to logical ID before invoking the actual client API

  DEPENDENCIES: N/A

                Copyright (c) 2001 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uim_interface.c#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when          who        what, where, why
--------      ----       ---------------------------------------------------------
05/04/17      vr         Send SW1 SW2 in fetch response from uimdrv to GSTK
06/09/16      shr        Deprecate GSTK TAL
04/20/16      sam        Support for enhanced subscription manager
03/28/16      sam        Initial Revision
============================================================================*/
#include "uimglobals.h"
#include "uim_interface.h"
#include "uim_slot_mapping.h"

/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_card_error

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_card_error
(
  uim_status_type error_condition,
  uim_slot_type slot,
  boolean temporary_error
)
{
  slot = uim_convert_physical_to_logical_slot_id(slot);
  
  return mmgsdi_notify_card_error(error_condition, slot, temporary_error);
}/*uim_interface_mmgsdi_notify_card_error*/


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_link_est_slot

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_link_est_slot
(
  uim_slot_link_established_data_type link_est_data,
  uim_slot_type                       slot_id
)
{
  slot_id = uim_convert_physical_to_logical_slot_id(slot_id);

  return mmgsdi_notify_link_est_slot(link_est_data, slot_id);
}/*uim_interface_mmgsdi_notify_link_est_slot*/


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_get_recovery_data_sync

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_get_recovery_data_sync
(
  uim_recovery_data_type *recovery_data_ptr,
  uim_slot_type           uim_slot_id
)
{
  uim_slot_id = uim_convert_physical_to_logical_slot_id(uim_slot_id);
  
  return mmgsdi_get_recovery_data_sync(recovery_data_ptr, uim_slot_id);
}/*uim_interface_mmgsdi_get_recovery_data_sync*/


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_sim_busy

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_sim_busy
(
  boolean        sim_busy,
  uim_slot_type  slot
)
{
  slot = uim_convert_physical_to_logical_slot_id(slot);
   
  return mmgsdi_notify_sim_busy(sim_busy, slot);
}/*uim_interface_mmgsdi_notify_sim_busy*/


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_sim_pre_init

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_sim_pre_init
(
  uim_slot_type                       uim_slot_type
)
{
  uim_slot_type = uim_convert_physical_to_logical_slot_id(uim_slot_type);
  
  return mmgsdi_notify_sim_pre_init(uim_slot_type);
}/*uim_interface_mmgsdi_notify_sim_pre_init*/


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_get_cached_nv_item_ext

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_get_cached_nv_item_ext 
(
  mmgsdi_cached_nv_item_data    *nv_item_data_ptr,
  uint8                          nv_index
)
{
  nv_index = (uint8)uim_convert_physical_to_logical_slot_id((uim_slot_type)nv_index);
  
  return mmgsdi_get_cached_nv_item_ext(nv_item_data_ptr, nv_index);
}/*uim_interface_mmgsdi_get_cached_nv_item_ext*/


/*============================================================================= 
  FUNCTION uim_interface_gstk_send_recovery_cmd

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
gstk_status_enum_type uim_interface_gstk_send_recovery_cmd
(
  uim_slot_type                   slot,
  uimdrv_recovery_stage_enum_type stage
)
{
  slot = uim_convert_physical_to_logical_slot_id(slot);
  
  return gstk_send_recovery_cmd(slot, stage);
}/*uim_interface_gstk_send_recovery_cmd*/


/*============================================================================= 
  FUNCTION uim_interface_gstk_uim_fetch_allow

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
boolean uim_interface_gstk_uim_fetch_allow
(
  gstk_slot_id_enum_type slot
)
{
  slot = (gstk_slot_id_enum_type)uim_convert_physical_to_logical_slot_id((uim_slot_type)slot);
  
  return gstk_uim_fetch_allow(slot);
}/*uim_interface_gstk_uim_fetch_allow*/


/*============================================================================= 
  FUNCTION uim_interface_gstk_send_fetch_cmd_report

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
gstk_status_enum_type uim_interface_gstk_send_fetch_cmd_report
(
  uim_slot_type   slot,
  uint32          data_length,
  const uint8   * data,
  uim_sw1_type    sw1,
  uim_sw2_type    sw2
)
{
  slot = uim_convert_physical_to_logical_slot_id(slot);

  return gstk_uim_send_fetch_cmd_report(slot, data_length, data, sw1, sw2);
}/*uim_interface_gstk_send_fetch_cmd_report*/


#ifdef FEATURE_ESTK
/*============================================================================= 
  FUNCTION uim_interface_estk_uim_is_bip_session_in_progress

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
boolean uim_interface_estk_uim_is_bip_session_in_progress
(
  gstk_slot_id_enum_type slot_id
)
{
  slot_id = (gstk_slot_id_enum_type)uim_convert_physical_to_logical_slot_id((uim_slot_type)slot_id);

  return estk_uim_is_bip_session_in_progress(slot_id);
}/*uim_interface_estk_uim_is_bip_session_in_progress*/
#endif


/*============================================================================= 
  FUNCTION uim_interface_mmgsdi_notify_recovery_complete

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
mmgsdi_return_enum_type uim_interface_mmgsdi_notify_recovery_complete
(
  uim_slot_type slot
)
{
  slot = uim_convert_physical_to_logical_slot_id(slot);

 return mmgsdi_notify_recovery_complete(slot);
}/*uim_interface_mmgsdi_notify_recovery_complete*/


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_supply_voltage_ind

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
void uim_interface_qmi_uim_supply_voltage_ind
(
  uim_slot_type                                     slot,
  uimdrv_qmi_indications_type                       ldo_state,
  uimdrv_qmi_power_management_callback_type         uim_callback_ptr
)
{
  slot = uim_convert_physical_to_logical_slot_id(slot);

  qmi_uim_supply_voltage_ind(slot, ldo_state, uim_callback_ptr);
  return;
}/*uim_interface_qmi_uim_supply_voltage_ind*/


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_register

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_register
(
  qmi_uim_remote_evt_callback_type    * event_cb_ptr,
  qmi_uim_remote_slot_type              slot
)
{
  slot =(qmi_uim_remote_slot_type)(uim_convert_physical_to_logical_slot_id((uim_slot_type)(slot+1))-1);
  
  return qmi_uim_remote_register(event_cb_ptr, slot);
}/*uim_interface_qmi_uim_remote_register*/


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_power_down

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_power_down 
(
  qmi_uim_remote_slot_type                  slot,
  qmi_uim_remote_power_down_mode_enum_type  pdown_mode
)
{
  slot =(qmi_uim_remote_slot_type)(uim_convert_physical_to_logical_slot_id((uim_slot_type)(slot+1))-1);

  return qmi_uim_remote_card_power_down(slot, pdown_mode);
}/*uim_interface_qmi_uim_remote_card_power_down*/


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_power_up

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_power_up 
(
  qmi_uim_remote_slot_type                slot,
  boolean                                 timeout_isvalid,
  uint32                                  timeout,
  boolean                                 vcc_isvalid,
  qmi_uim_remote_voltage_class_enum_type  vcc
)
{
  slot =(qmi_uim_remote_slot_type)(uim_convert_physical_to_logical_slot_id((uim_slot_type)(slot+1))-1);

  return qmi_uim_remote_card_power_up(slot,
                                      timeout_isvalid,
                                      timeout,
                                      vcc_isvalid,
                                      vcc);
}/*uim_interface_qmi_uim_remote_card_power_up*/


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_disconnect

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_disconnect 
(
  qmi_uim_remote_slot_type            slot
)
{
  slot =(qmi_uim_remote_slot_type)(uim_convert_physical_to_logical_slot_id((uim_slot_type)(slot+1))-1);
  
  return qmi_uim_remote_card_disconnect(slot);
}/*uim_interface_qmi_uim_remote_card_disconnect*/


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_connect

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_connect
(
  qmi_uim_remote_slot_type            slot
)
{
  slot =(qmi_uim_remote_slot_type)(uim_convert_physical_to_logical_slot_id((uim_slot_type)(slot+1))-1);

  return qmi_uim_remote_card_connect(slot);
}/*uim_interface_qmi_uim_remote_card_connect*/


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_card_reset

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_card_reset
(
  qmi_uim_remote_slot_type            slot
)
{
  slot =(qmi_uim_remote_slot_type)(uim_convert_physical_to_logical_slot_id((uim_slot_type)(slot+1))-1);

  return qmi_uim_remote_card_reset(slot);
}/*uim_interface_qmi_uim_remote_card_reset*/


/*============================================================================= 
  FUNCTION uim_interface_qmi_uim_remote_send_apdu

  Interface function where physical slot ID is converted to logical ID 
  before invoking the client API
=============================================================================*/
qmi_uim_remote_result_type uim_interface_qmi_uim_remote_send_apdu 
(
  qmi_uim_remote_slot_type               slot,
  void *                                 client_ref_ptr,
  uint16                                 cmd_apdu_len,
  const uint8 *                          cmd_apdu_ptr,
  qmi_uim_remote_apdu_callback_type    * rsp_apdu_cb_ptr
)
{
  slot =(qmi_uim_remote_slot_type)(uim_convert_physical_to_logical_slot_id((uim_slot_type)(slot+1))-1);

  return qmi_uim_remote_send_apdu (slot,
                                   client_ref_ptr,
                                   cmd_apdu_len,
                                   cmd_apdu_ptr,
                                   rsp_apdu_cb_ptr);
} /*uim_interface_qmi_uim_remote_send_apdu*/
