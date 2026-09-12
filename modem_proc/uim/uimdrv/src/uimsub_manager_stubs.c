/*===========================================================================


                     U I M   S U B S C R I P T I O N  M A N A G E R STUBS

DESCRIPTION
  This contains stubs files for Subscription manager

Copyright (c) 2012-2016 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.


===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimsub_manager_stubs.c#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/03/17   ks      UIM automatic slot mapping support
04/20/16   sam     Intial Revision
===========================================================================*/
/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uimsub_manager.h"
#include "uimglobals.h"
#include "uim_hotswap.h"
#include "uimdrv_main.h"

/*===========================================================================
                       FUNCTION DECLARATIONS
===========================================================================*/

/*============================================================================= 
  STUBS FUNCTION uim_sm_mmgsdi_notify_card_error
  
  Description:
    Invoke mmgsdi api to notify error
=============================================================================*/
void uim_sm_mmgsdi_notify_card_error
(
  uim_status_type           error_condition,
  uim_instance_global_type *uim_ptr,
  boolean                   temporary_error
)
{
  uim_util_mmgsdi_notify_card_error(uim_ptr, error_condition, (uim_slot_type)(uim_ptr->id + 1), temporary_error);
} /* uim_sm_mmgsdi_notify_card_error */


/*============================================================================= 
  STUBS FUNCTION uim_sm_link_established
  
  Description:
    Invoke mmgsdi api to notify link establishment
=============================================================================*/
void uim_sm_link_established(uim_instance_global_type *uim_ptr)
{
  uim_gsdi_link_est(uim_ptr);
} /* uim_sm_link_established */


/*============================================================================= 
  STUBS FUNCTION uim_sm_switch_slot
=============================================================================*/
uim_rpt_status uim_sm_switch_slot
(
  uim_slot_type logical_slot,
  uim_slot_type physical_slot
)
{
 (void)logical_slot;
 (void)physical_slot;
  return UIM_FAIL;
}/*uim_sm_switch_slot*/


/*============================================================================= 
  STUBS FUNCTION uim_sm_card_slot_status_evt_reg_sync
=============================================================================*/
uim_rpt_status uim_sm_card_slot_status_evt_reg_sync
(
  uim_evt_callback_type * evt_cb_ptr
)
{
  (void)evt_cb_ptr;
  return UIM_FAIL;
} /*uim_sm_card_slot_status_evt_reg_sync*/


/*============================================================================= 
  STUBS FUNCTION uim_sm_get_num_physical_slots_sync
=============================================================================*/
uint8 uim_sm_get_num_physical_slots_sync (void)
{
  return (uint8)uimdrv_hw_nv_config_info.number_of_active_interfaces;
}/*uim_sm_get_num_physical_slots_sync*/


/*============================================================================= 
  STUBS FUNCTION uim_sm_get_logical_slot_status_sync
=============================================================================*/
uim_rpt_status uim_sm_get_logical_slot_status_sync 
(
  uint8                      *num_phy_slots_ptr,
  uim_phy_slot_status_type   *curr_slot_status
)
{
  (void)num_phy_slots_ptr;
  (void)curr_slot_status;
  return UIM_FAIL;
}/*uim_sm_get_logical_slot_status_sync*/


/*============================================================================= 
  STUBS FUNCTION uim_sm_notify_clients_slot_status
=============================================================================*/
uim_rpt_status uim_sm_notify_clients_slot_status
(
  uim_instance_global_type *uim_ptr
)
{
  (void)uim_ptr;
  return UIM_FAIL;
}/*uim_sm_notify_clients_slot_status*/


/*============================================================================= 
  STUBS FUNCTION uim_sm_init
=============================================================================*/
void uim_sm_init
(
  uim_instance_global_type *uim_ptr
)
{
  (void)uim_ptr;
  return;
}/* uim_sm_init*/


/*============================================================================= 
  STUBS FUNCTION uim_sm_update_phy_slot_activity_info
=============================================================================*/
void uim_sm_update_phy_slot_activity_info
(
  byte num_active_slots, 
  byte *slot_ptr
)
{
  (void)num_active_slots;
  (void)slot_ptr;
  return;
}/*uim_sm_update_phy_slot_activity_info*/


/*============================================================================= 
  STUBS FUNCTION uim_sm_update_phy_slot_card_info
=============================================================================*/
void uim_sm_update_phy_slot_card_info
(
  uim_instance_global_type *uim_ptr
)
{
  (void)uim_ptr;
  return;
}


/*============================================================================= 
  STUBS FUNCTION uim_sm_validate_and_update_num_active_slots_value
=============================================================================*/
boolean uim_sm_validate_and_update_num_active_slots_value
(
  byte *active_slots_ptr
)
{
  (void)active_slots_ptr;
  return FALSE;
}


/*============================================================================= 
  STUBS FUNCTION uim_set_activity_status
=============================================================================*/
void uim_set_activity_status
(
  uim_instance_global_type *uim_ptr
)
{
  uim_ptr->activity_status = UIM_SLOT_STATE_ACTIVE;
}

/*============================================================================= 
  STUBS FUNCTION uim_sm_is_swtich_slot_processing_required
=============================================================================*/
boolean uim_sm_is_swtich_slot_processing_required
(
  uim_instance_global_type *uim_ptr
)
{
  (void)uim_ptr;
  return TRUE;
}


/*===========================================================================
FUNCTION UIM_IS_SUB_MANAGER_ENABLED

DESCRIPTION:
  Function to check if Sub manager feature is enabled

LIMITATIONS:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
===========================================================================*/
boolean uim_is_sub_manager_enabled(void)
{
#ifdef FEATURE_UIM_DS_SUBSCRIPTION_MANAGER
  return TRUE;
#else
  return FALSE;
#endif
}/*uim_is_sub_manager_enabled*/
