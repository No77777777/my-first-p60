/*============================================================================
  FILE:         uim_slot_mapping_stubs.c

  OVERVIEW:
   This is a stubs file for uim_slot_mapping.c.

  DEPENDENCIES: N/A

              Copyright (c) 2016 - 2017 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
              QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimdrv_slot_mapping_stubs.c#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when          who        what, where, why
--------      ----       ---------------------------------------------------------
12/05/17      gm         Update only active slots in uim_get_hw_slot_info
05/10/17      kv         Enable subs manager and sam card mode
01/19/17      ks         UIM automatic slot mapping support
03/28/16      sam        Initial Revision
============================================================================*/
#include "rex.h"
#include "err.h"
#include "uimdrv_msg.h"
#include "uimglobals.h"
#include "uim_slot_mapping.h"

/*slot mapping global*/
uim_slot_mapping_global_type uim_slot_mapping;

/*============================================================================= 
  STUBS FUNCTION uim_slot_mapping_init
=============================================================================*/
void uimdrv_slot_mapping_init(uim_instance_global_type *uim_ptr)
{
  uim_ptr->logical_id = (uim_slot_type)(uim_ptr->id+1);
}/*uimdrv_slot_mapping_init*/


/*============================================================================= 
  STUBS FUNCTION uim_switch_slot_mapping
=============================================================================*/
uim_rpt_status uim_switch_slot_mapping(uim_logical_slot_config_type               *logical_slot_config, 
                                       uim_subscription_manager_client_enum_type   client)
{
  (void)logical_slot_config;
  UIM_MSG_HIGH_0("FEATURE_SAM_CARD not enabled. returning fail");
  return UIM_FAIL;
}/*uim_switch_slot_mapping*/


/*============================================================================= 
  STUBS FUNCTION uim_get_logical_to_physical_mapping
=============================================================================*/
uim_rpt_status uim_get_logical_to_physical_mapping(uim_logical_slot_config_type *logical_slot_config)
{
  (void)logical_slot_config;
  UIM_MSG_HIGH_0("FEATURE_SAM_CARD not enabled. returning fail");
  return UIM_FAIL;
}/*uim_get_logical_to_physical_mapping*/


/*============================================================================= 
  STUBS FUNCTION uim_convert_physical_to_logical_slot_id
=============================================================================*/
uim_slot_type uim_convert_physical_to_logical_slot_id(uim_slot_type slot_id)
{
  return slot_id;
}/*uim_convert_physical_to_logical_slot_id*/


/*============================================================================= 
  STUBS FUNCTION uim_convert_logical_to_physical_slot_id
=============================================================================*/
uim_slot_type uim_convert_logical_to_physical_slot_id(uim_slot_type slot_id)
{
  return slot_id;
}/*uim_convert_logical_to_physical_slot_id*/


/*============================================================================= 
  STUBS FUNCTION uim_slot_mapping_translate_cmd
=============================================================================*/
uim_slot_mapping_cmd_saved_data_type*  uim_slot_mapping_translate_cmd
(
  uim_cmd_type *cmd_ptr
)
{
  (void)cmd_ptr;
  return NULL;
}/*uim_slot_mapping_translate_cmd*/


/*============================================================================= 
  STUBS FUNCTION uim_slot_mapping_free_userdata
=============================================================================*/
void uim_slot_mapping_free_userdata
(
  uim_cmd_type *cmd_ptr
)
{
  (void)cmd_ptr;
  return;
}/*uim_slot_mapping_free_userdata*/


/*============================================================================= 
  STUBS FUNCTION uim_set_logical_slot_power_down_flag
=============================================================================*/
void uim_set_logical_slot_power_down_flag
(
  uim_instance_global_type *uim_ptr,
  boolean                   value
)
{
  (void)uim_ptr;
  (void)value;
  return;
}/*uim_set_logical_slot_power_down_flag*/


/*============================================================================= 
  STUBS FUNCTION uim_handle_switch_logical_slot_sig
=============================================================================*/
boolean uim_handle_switch_logical_slot_sig
(
  rex_sigs_type *sig_mask_ptr,
  rex_sigs_type *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  (void)sig_mask_ptr;
  (void)imask_ptr;
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_SWITCH_LOGICAL_SLOT_SIG);
  UIMDRV_MSG_ERR_0(uim_ptr->id,"Error:SAM feature not enabled");
  return FALSE;
}/*uim_handle_switch_logical_slot_sig*/


/*============================================================================= 
  STUBS FUNCTION uim_get_num_active_slot
=============================================================================*/
uint8 uim_get_num_active_slot(uint8 max_num_slot_supported)
{
  return max_num_slot_supported;
}/* uim_get_num_active_slot */


