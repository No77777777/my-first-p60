#ifndef UIMSUB_MANAGER_H
#define UIMSUB_MANAGER_H
/*===========================================================================

        E X T E R N A L   U I M  C A R D  M A N A G E R
                            D E F I N I T I O N S

DESCRIPTION
  This contains all the declarations for the UIM Subscription Manager.
  The Subscription Manager allows Dual Slots and two cards simultaneously
  while making the presence of only one known to outside clients.  This
  way subscription information from only one card is published at a time and
  only one card is used by the rest of the modem.

 Copyright (c) 2012-2016 QUALCOMM Technologies, Inc (QTI) and its licensors.All Rights Reserved.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimsub_manager.h#2 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/03/17   ks      UIM automatic slot mapping support
04/20/16   sam     Support for enhanced subscription manager
03/28/16   sam     Support for SAM card
10/27/15   hyo     Avoid writing enum to EFS
07/20/15   ks      Remove memory free in subscription manager report callback
08/14/14   sam     Adding header file of messsage macros
08/13/31   js      UIMDRV Memory De-Couple changes
08/08/13   ks      Advanced Subscription Manager changes
05/21/12   nmb     UIM Dual SIM Subscription Manager Initial Revision
===========================================================================*/
/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uimi.h"
#include "uim_v.h"


/*===========================================================================
                       FUNCTION PROTOTYPES
===========================================================================*/

/*============================================================================
   FUNCTION:      UIM_SM_MMGSDI_NOTIFY_CARD_ERROR

   DESCRIPTION:
     Based on the activity status the function takes a descion 
     to notify MMGSDI or power down the slot.
     Active   - Notify error to mmgsdi
     Inactive - Power down the slot

   DEPENDENCIES:
     None

   RETURN VALUE:
     None
============================================================================*/
void uim_sm_mmgsdi_notify_card_error
(
  uim_status_type          error_condition,
  uim_instance_global_type *uim_ptr,
  boolean                  temporary_error
);


/*===========================================================================
FUNCTION UIM_SM_LINK_ESTABLISHED

DESCRIPTION
   Based on the activity status the function takes a descion 
   to notify MMGSDI of link establishment or power down the slot.
   Active   - Notify error to mmgsdi
   Inactive - Power down the slot

DEPENDENCIES
  None

RETURN VALUE
  None
===========================================================================*/
void uim_sm_link_established
(
  uim_instance_global_type *uim_ptr
);


/*===========================================================================
FUNCTION UIM_SM_SWITCH_SLOT

DESCRIPTION
  This function will queue the UIM command to switch to new active slot
  and update the activity status of the slots

PARAMS:
  uim_slot_type:  Logical slot numbet to be switched to
                  Reserved for future targets
  uim_slot_type:  Physical slot number to be switched to

LIMITATIONS:
  None

RETURN VALUE:
  uim_rpt_status
  UIM_PASS:          If command is successfully queued
  UIM_FAIL:          Unsuccessful queue of switch command


SIDE EFFECTS:
 None
===========================================================================*/
uim_rpt_status uim_sm_switch_slot 
(
  uim_slot_type logical_slot,
  uim_slot_type physical_slot
);


/*===========================================================================
FUNCTION UIM_SM_CARD_SLOT_STATUS_EVT_REG_SYNC

DESCRIPTION
  Function to register qmi callback to UIMDRV to get the latest physical
  slots information.

PARAMS:
   qmi_uim_notify_card_slot_status:Callback function pointer to be registered

LIMITATIONS:
  None

RETURN VALUE:
  uim_rpt_status
  UIM_PASS:          If registration was successful
  UIM_FAIL:          Unsuccessful registration.

SIDE EFFECTS:
 None

===========================================================================*/
uim_rpt_status uim_sm_card_slot_status_evt_reg_sync
(
  uim_evt_callback_type *evt_cb_ptr
);


/*===========================================================================
FUNCTION     UIM_SM_GET_NUM_PHYSICAL_SLOTS_SYNC

DESCRIPTION:
  Function to get number of physical slots tied to the target

RETURN
  uint8       Number of physical slots tied. 0 for invalid slot
================================================================*/
uint8 uim_sm_get_num_physical_slots_sync 
(
  void
);


/*===========================================================================
FUNCTION UIM_SM_GET_LOGICAL_SLOT_STATUS_SYNC

DESCRIPTION:
  This function will return the status of all physical slots

PARAMS:
  uim_phy_slot_status_type :   Get current status of all logical slot;

LIMITATIONS:
    None

RETURN VALUE:
  uim_rpt_status
  UIM_PASS:     Physical slot status was successfully synced
  UIM_FAIL:     Invalid logical slot given or unsuccessful syncing

SIDE EFFECTS:
  None.
===========================================================================*/
uim_rpt_status uim_sm_get_logical_slot_status_sync 
(
  uint8                      *num_phy_slots_ptr,
  uim_phy_slot_status_type   *curr_slot_status
);


/*===========================================================================
FUNCTION UIM_SM_NOTIFY_CLIENTS_SLOT_STATUS

DESCRIPTION:
 Notify qmi the latest physical slot status information

LIMITATIONS:
    None

RETURN VALUE:
  uim_rpt_status
  UIM_PASS:     Physical slot status was successfully synced
  UIM_FAIL:     Invalid logical slot given or unsuccessful syncing

SIDE EFFECTS:
  None.
===========================================================================*/
uim_rpt_status uim_sm_notify_clients_slot_status
(
  uim_instance_global_type *uim_ptr
);


/*===========================================================================
FUNCTION UIM_SM_INIT

DESCRIPTION:
  Initialize globals related to subscription manager

LIMITATIONS:
  None

RETURN VALUE:
  void

SIDE EFFECTS:
  None.
===========================================================================*/
void uim_sm_init
(
  uim_instance_global_type *uim_ptr
);


/*===========================================================================
FUNCTION UIM_SM_UPDATE_PHY_SLOT_ACTIVITY_INFO

DESCRIPTION:
  Update the sub manager globals with the latest activity status and 
  logical ID of each slot

LIMITATIONS:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
===========================================================================*/
void uim_sm_update_phy_slot_activity_info
(
  byte num_active_slots, 
  byte *slot_ptr
);


/*===========================================================================
FUNCTION UIM_SM_UPDATE_PHY_SLOT_CARD_INFO

DESCRIPTION:
  Update the sub manager globals with the latest ICCID and 
  card prescence status of each slot

LIMITATIONS:
    None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
===========================================================================*/
void uim_sm_update_phy_slot_card_info
(
  uim_instance_global_type *uim_ptr
);


/*===========================================================================
FUNCTION UIM_SM_VALIDATE_AND_UPDATE_NUM_ACTIVE_SLOTS_VALUE

DESCRIPTION:
  Validate if the num of active slots requested is suppoted or not.
  if not, then update it with the max supported active slots.
LIMITATIONS:
  None

RETURN VALUE:
  FALSE: if num active slots is supported
  TRUE:  if num of active slots is not supported and the value is 
         updated with the max active slots supported value

SIDE EFFECTS:
  None.
===========================================================================*/
boolean uim_sm_validate_and_update_num_active_slots_value
(
  byte *active_slots_ptr
);


/*===========================================================================
FUNCTION UIM_SET_ACTIVITY_STATUS

DESCRIPTION:
  set the activity status varaible in the uim_ptr global based on the 
  sub manager global

LIMITATIONS:
  None

RETURN VALUE:
  void

SIDE EFFECTS:
  None.
===========================================================================*/
void uim_set_activity_status
(
  uim_instance_global_type *uim_ptr
);


/*===========================================================================
FUNCTION UIM_SM_IS_SWITCH_SLOT_PROCESSING_REQUIRED

DESCRIPTION:
  Function to check if switch slot porcessing is required or not.
  If both activity status and logical ID is unchanged then the processing can be skipped

LIMITATIONS:
  None

RETURN VALUE:
  TUE:   Switch slot processing required
  FALSE: Switch slot processing not required

SIDE EFFECTS:
  None.
===========================================================================*/
boolean uim_sm_is_swtich_slot_processing_required
(
  uim_instance_global_type *uim_ptr
);


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
boolean uim_is_sub_manager_enabled(void);
#endif /* UIMSUB_MANAGER_H */
