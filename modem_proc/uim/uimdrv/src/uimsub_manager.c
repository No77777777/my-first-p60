/*===========================================================================


                     U I M   S U B S C R I P T I O N  M A N A G E R

DESCRIPTION
  This contains all the declarations for the UIM Subscription Manager.
  The Subscription Manager allows Dual Slots and two cards simultaneously
  while making the presence of only one known to outside clients.  This
  way subscription information from only one card is published at a time and
  only one card is used by the rest of the modem.

Copyright (c) 2001 - 2020 QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimsub_manager.c#4 $
$DateTime: 2020/02/03 00:52:48 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/31/20   ykr     Removing code to set evt_cb_ptr to NULL
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
06/12/19   ykr     Add extended card status to SimSlotStatus
04/08/19   ng      Updating slot status check via ATR
09/28/18   nr      Fix to update proper card_status to clients
09/05/18   ng      Fix to correct physical_card_status in uim_slot_status_change
02/16/18   dd      Optimization of F3 prints
06/22/17   ku      Fix for UIM Drivers KW errors
04/14/17   kv      Added pass-through mode support
04/07/17   ks      Setting the notify_card_removed to FALSE appropriately
04/03/17   ks      UIM automatic slot mapping support
07/14/16   ks      Enhancing the extended recovery feature
06/23/16   sam     F3 messages optimization
04/20/16   sam     Support for enhanced subscription manager
03/28/16   sam     Support for SAM card
03/09/16   na      Iccid length is being filled from file size received in FCP of EF-ICCID
12/10/15   ks      Fixing UIM not stopping clock issue during no card scenario
10/27/15   hyo     Avoid writing enum to EFS
09/04/15   ks      UIM votes for sleep while powering down inactive slot.
08/20/15   ks      Resending error notification if mmgsdi_notigy_error fails
07/20/15   ks      Remove memory free in subscription manager report callback
08/04/15   sam     FR24498: Powerup logging new changes
04/06/15   ll      UIMDRV migrates to UIM COMMON EFS APIs for all EFS items
01/07/14   sam     Modification of msg macros for FR24498-UIM Powerup Logging
12/05/14   ks      Clearing path and POLL voting upon card insertion
09/18/14   akv     UIMDRV UT enhancements
08/25/14   ll      Fix compile warnings medium
08/14/14   sam     Add support for silent PIN verification for extended recovery
03/21/14   ll      Remove return_ptr and remove/change uim_free to UIM_FREE
01/29/14   sam     Usage of newer style diag macros
02/05/14   sam     Fix for klocwork issues
12/27/13   ks      Subscription manager review fixes
12/21/13   am      Silent Recover Feature Implementation
09/30/13   akv     Removal of multiple efs reads performed by uim tasks
09/24/13   nmb     Remove extra uim_free param
09/17/13   ssr     Added support for new recovery notification for GSTK
09/12/13   js      Fixed compilation error
09/04/13   ks      Subscription manager - Added CARD_STATUS_UNKNOWN
08/13/31   js      UIMDRV Memory De-Couple changes
08/08/13   ks      Advanced Subscription Manager changes
06/19/13   ms      Subscription Manager changes
05/21/12   nmb     UIM Dual SIM Subscription Manager Initial Revision
===========================================================================*/
/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uimdrv_msg.h"
#include "uimsub_manager.h"
#include "uimglobals.h"
#include "uim_hotswap.h"
#include "uimdrv_main.h"

/*===========================================================================
              F U N C T I O N   P R O T O T Y P E S
===========================================================================*/
static void uim_print_sub_mananger_globals(uim_instance_global_type *uim_ptr);


/*===========================================================================
                       FUNCTION DEFINITIONS
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
  uim_status_type           error_condition,
  uim_instance_global_type *uim_ptr,
  boolean                   temporary_error
)
{
  /*return if pointer is NULL*/
  if(NULL == uim_ptr)
  {
    return;
  }
  
  uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].uim_status = error_condition;
  
  /*Update sub manger global with latest physical slot status and notify QMI*/
  uim_sm_update_phy_slot_card_info(uim_ptr);
  uim_sm_notify_clients_slot_status(uim_ptr);

  if(UIM_SLOT_STATE_ACTIVE == uim_ptr->activity_status)
  {
    /*Notify mmgsdi of the error if slot is active*/
	uim_util_mmgsdi_notify_card_error(uim_ptr, error_condition, (uim_slot_type)(uim_ptr->id + 1), temporary_error);
  }
  else if (uim_ptr->state.status != UIM_ERR_S)
  {
    UIMDRV_MSG_HIGH_1(uim_ptr->id, "Power down the In-active slot 0x%x ",
                   (uim_slot_type)(uim_ptr->id + 1));

    /*Set the inactive slot status to error state and power-down */
    uim_ptr->state.status = UIM_ERR_S;
    uim_hotswap_reset_slot(uim_ptr);
    uim_power_down(uim_ptr);
  }
  else
  {
    UIMDRV_MSG_HIGH_1(uim_ptr->id,
                 "Inactive slot 0x%x is already in power-down error state ",
                      (uim_slot_type)(uim_ptr->id + 1));
  }
} /* uim_sm_mmgsdi_notify_card_error */


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
)
{
  /*return if pointer is NULL*/
  if(NULL == uim_ptr)
  {
    return;
  }

  /*Update sub manger global with latest physical slot status and notify QMI*/
  uim_sm_update_phy_slot_card_info(uim_ptr);
  uim_sm_notify_clients_slot_status(uim_ptr);

  if(UIM_SLOT_STATE_ACTIVE == uim_ptr->activity_status)
  {
    UIMDRV_MSG_HIGH_1(uim_ptr->id,"Send link est for active slot instance 0x%x",
                       (uim_slot_type)(uim_ptr->id));
    /*Notify link establishment to mmgsdi if slot is active*/
    uim_gsdi_link_est(uim_ptr);
  }
  else
  {
    /*Set the inactive slot status to error state and power-down */
    UIMDRV_MSG_HIGH_1(uim_ptr->id,"Power down inactive slot instance 0x%x",
                      (uim_slot_type)(uim_ptr->id));
    uim_ptr->state.status = UIM_ERR_S;
    uim_hotswap_reset_slot(uim_ptr);
    uim_power_down(uim_ptr);
  }
} /* uim_sm_link_established */


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
)
{
  uim_logical_slot_config_type logical_slot_config;
  
  /*Return fail if logical slot is not UIM_SLOT_1*/
  if(logical_slot != UIM_SLOT_1)
  {
    return UIM_FAIL;
  }

  memset(&logical_slot_config, 0, sizeof(uim_logical_slot_config_type));

  logical_slot_config.num_slots = 1;
  logical_slot_config.slot[0] = physical_slot;

  return uim_switch_slot_mapping(&logical_slot_config, UIMDRV_SUB_MGR_CLIENT_MAX);
}/*uim_sm_switch_slot*/


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
  uim_evt_callback_type * evt_cb_ptr
)
{
  uint8 i = 0;
  /*return Fail if evt_cb_ptr is NULL*/
  if(NULL == evt_cb_ptr)
  {
    return UIM_FAIL;
  }

  UIM_MSG_MED_1("Registering QMI sub manager callback pointer-0x%x with UIMDRV ", evt_cb_ptr);
  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  for( i = 0; i < UIMDRV_SUB_MGR_CLIENT_MAX; i++)
  {
    if( uim_slot_mapping.sm_data.evt_cb_ptr[i] == NULL )
    {
      uim_slot_mapping.sm_data.evt_cb_ptr[i] = evt_cb_ptr;
      break;
    }
  }
  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  return UIM_PASS;
} /*uim_sm_card_slot_status_evt_reg_sync*/

/*===========================================================================
FUNCTION     UIM_SM_GET_NUM_PHYSICAL_SLOTS_SYNC

DESCRIPTION:
  Function to get number of physical slots tied to the target

RETURN
  uint8       Number of physical slots tied. 0 for invalid slot
================================================================*/
uint8 uim_sm_get_num_physical_slots_sync (void)
{
  return (uint8)uimdrv_hw_nv_config_info.number_of_active_interfaces;
}/*uim_sm_get_num_physical_slots_sync*/


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
)
{
  /*return Fail if pointer is NULL*/
  if(curr_slot_status == NULL || num_phy_slots_ptr == NULL)
  {
    UIM_MSG_ERR_0("Null structure error");
    return UIM_FAIL;
  }

  *num_phy_slots_ptr = uimdrv_hw_nv_config_info.number_of_active_interfaces;

  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  UIM_MSG_HIGH_0("QMI syncing latest slot information");
  uim_memscpy(curr_slot_status, ((*num_phy_slots_ptr)*sizeof(uim_phy_slot_status_type)),
              uim_slot_mapping.sm_data.phy_slot_info, ((*num_phy_slots_ptr)*sizeof(uim_phy_slot_status_type)));
  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);

  return UIM_PASS;
}/*uim_sm_get_logical_slot_status_sync*/


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
)
{
  uint8 i = 0;
  uim_phy_slot_status_type temp_status[UIM_MAX_NUMBER_INSTANCES];

  /*return if pointer is NULL*/
  if(NULL == uim_ptr)
  {
    return UIM_FAIL;
  }

  UIMDRV_MSG_HIGH_0(uim_ptr->id,"Reporting QMI latest physical slot status ");

  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);

  uim_memscpy(temp_status, sizeof(temp_status),
			  uim_slot_mapping.sm_data.phy_slot_info, sizeof(uim_slot_mapping.sm_data.phy_slot_info));

  uim_print_sub_mananger_globals(uim_ptr);

  /*Notify the clients about the latest slot status*/
  for( i = 0; i < UIMDRV_SUB_MGR_CLIENT_MAX; i++)
  {
    if(uim_slot_mapping.sm_data.evt_cb_ptr[i] != NULL)
    {
      UIMDRV_MSG_HIGH_1(uim_ptr->id,"Calling qmi_uim call back to update slot_mapping 0x%x", i);
      (*uim_slot_mapping.sm_data.evt_cb_ptr[i])(uimdrv_hw_nv_config_info.number_of_active_interfaces,
                                                temp_status);
    }
  }

  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);

  return UIM_PASS;
}/*uim_sm_notify_clients_slot_status*/


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
)
{
  uint8  i = 0;

  /*return if pointer is NULL*/
  if(NULL == uim_ptr)
  {
    return;
  }
 
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"uim_sm_init ");

  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  /*Intialise the slot mapping globals only once*/
  if (uim_slot_mapping.sm_data.is_initialised != TRUE)
  {
    uim_slot_mapping.sm_data.is_initialised = TRUE;

    /*set the number of max active slots to the number of slots enabeld HW nv config*/
    uim_slot_mapping.sm_data.max_active_slots = (uint8)uimdrv_hw_nv_config_info.number_of_active_interfaces;
    if(uim_slot_mapping.sm_data.max_active_slots > UIM_MAX_SUBSCRIPTIONS_SUPPORTED)
    {
      /*reset if the active slots is greater than the subscriptions supported by modem*/
      uim_slot_mapping.sm_data.max_active_slots = UIM_MAX_SUBSCRIPTIONS_SUPPORTED;
    }

    uim_slot_mapping.sm_data.active_slots_num = uim_slot_mapping.sm_data.max_active_slots;
   
    for(i=0; i < uimdrv_hw_nv_config_info.number_of_active_interfaces; i++)
    {
      /* Initialize the iccid value of physical slots to 0 and
        card_presence_status to CARD REMOVED */
      uim_slot_mapping.sm_data.phy_slot_info[i].logical_slot = (uim_slot_type)(i+1);
      uim_slot_mapping.sm_data.phy_slot_info[i].iccid_len = UIM_ICCID_SIZE;
      uim_slot_mapping.sm_data.phy_slot_info[i].card_presence_status = UIM_CARD_STATUS_UNKNOWN;
      memset(uim_slot_mapping.sm_data.phy_slot_info[i].iccid, 0x00,
             uim_slot_mapping.sm_data.phy_slot_info[i].iccid_len);

      /*Reset activity status to ACTIVE for enabled slots(in Hw_nv_config)*/
      if(i <  uim_slot_mapping.sm_data.active_slots_num)
      {
        uim_slot_mapping.sm_data.phy_slot_info[i].activity_status =
          UIM_SLOT_STATE_ACTIVE;
      }
      else
      {
        uim_slot_mapping.sm_data.phy_slot_info[i].activity_status =
        UIM_SLOT_STATE_INACTIVE;
      }
    }
  }
  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);
}/* uim_sm_init*/


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
)
{
  int   i                   = 0;
  byte  phy_id              = 0;
  byte  sup_slots           = 0;

  if(NULL == slot_ptr)
  {
    return;
  }
  
  UIM_MSG_HIGH_0("updating logical ID and activity status");
  sup_slots = (uint8)uimdrv_hw_nv_config_info.number_of_active_interfaces;

  /*update the logical ID and activity status*/
  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  for(i=0; i<sup_slots; i++)
  {
    phy_id = (byte)(slot_ptr[i]-1);
    if((phy_id < (byte)UIM_INSTANCE_1) || (phy_id >= UIM_MAX_NUMBER_INSTANCES))
    {
      continue;
    }
    uim_slot_mapping.sm_data.phy_slot_info[phy_id].logical_slot = (uim_slot_type)(i+1);

    if(i < num_active_slots)
    {
      uim_slot_mapping.sm_data.phy_slot_info[phy_id].activity_status = UIM_SLOT_STATE_ACTIVE;
    }
    else
    {
      uim_slot_mapping.sm_data.phy_slot_info[phy_id].activity_status = UIM_SLOT_STATE_INACTIVE;
      uim_slot_mapping.sm_data.phy_slot_info[phy_id].logical_slot    = UIM_SLOT_NONE;
    }
  }
  uim_slot_mapping.sm_data.active_slots_num = num_active_slots;
  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);
}/*uim_sm_update_phy_slot_activity_info*/


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
)
{
  if(NULL == uim_ptr)
  {
    return;
  }

  /*return if instance is invalid*/
  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);
	
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"updating ICCID and card prescence status");
 
  /* Get the latest iccid values of card and card status */

  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);

  if(uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == FALSE)
  {
    uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].card_presence_status =
                        UIM_CARD_STATUS_UNKNOWN;
  }
  else
  {
    uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].card_presence_status =
              uim_ptr->hotswap.card_status;
  }

  /* Update the eUICC flag */
  if (uim_ptr->flag.is_euicc)
  {
     uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].is_euicc = TRUE;
  }  
  else
  {
     uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].is_euicc = FALSE;
  }    

  /* Update the protocol type */
  uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].uim_protocol = uim_ptr->card_cmd.uimdrv_protocol;
  
  /* Update the number of valid apps in EF-DIR */
  uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].num_apps = 
          uim_ptr->cache.num_apps.num_apps_valid;

  /* Update the ICCIC length */
  uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].iccid_len =
          uim_ptr->cache.iccid_buf.num_bytes;

  /* Update the ATR length */
  uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].atr_len=
          uim_ptr->atr.atr_buf.num_bytes; 

  if(uim_ptr->flag.is_euicc)
  {
    uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid_len = 
          uim_ptr->cache.eid_buf.num_bytes;
  }
  else
  {
    uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid_len = 0;
  }		  

  /* Update the ICCID, ATR and EID data */
  if(uim_ptr->state.status != UIM_ERR_S)
  {
    uim_memscpy(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].iccid,
                sizeof(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].iccid),
                uim_ptr->cache.iccid_buf.data,
                uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].iccid_len);
                
    uim_memscpy(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].atr,
                sizeof(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].atr),
                uim_ptr->atr.atr_buf.data,
                uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].atr_len);                

    if(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid_len > 0)
	{
      uim_memscpy(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid,
	              sizeof(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid),
				  uim_ptr->cache.eid_buf.data,
				  uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid_len);
	}				
  }
  else
  {
    memset(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].iccid, 0x00,
           sizeof(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].iccid));
    uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].iccid_len = 0;

    memset(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].atr, 0x00,
           sizeof(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].atr));
    uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].atr_len = 0;    
	
	memset(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid, 0x00,
	       sizeof(uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid));
    uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].eid_len = 0;
  }

  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);

}/*uim_sm_update_phy_slot_card_info*/


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
)
{
  boolean update_status = FALSE;

  /*return if the pointer is NULL*/
  if(NULL == active_slots_ptr)
  {
    return FALSE;
  }
  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);

  /*if number of active slots is not supported then reset ot max active slots*/	
  if((*active_slots_ptr <= 0) || (*active_slots_ptr > uim_slot_mapping.sm_data.max_active_slots))
  {
    *active_slots_ptr = uim_slot_mapping.sm_data.max_active_slots;
     update_status = TRUE;
  }
  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  return update_status;
}/*uim_sm_validate_and_update_num_active_slots_value*/


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
)
{
  /*return if pointer is null*/
  if(NULL == uim_ptr)
  {
    return;
  }
  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  uim_ptr->activity_status = uim_slot_mapping.sm_data.phy_slot_info[uim_ptr->id].activity_status;
  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);
}/*uim_set_activity_status*/


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
)
{
  uim_phy_slot_status_type   phy_slot_status[UIM_MAX_NUMBER_INSTANCES];

  /*return if pointer is null*/
  if(NULL == uim_ptr)
  {
    return FALSE;
  }

  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  memscpy(phy_slot_status,
          sizeof(phy_slot_status),
          uim_slot_mapping.sm_data.phy_slot_info, 
          sizeof(uim_slot_mapping.sm_data.phy_slot_info));
  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);

  if(phy_slot_status[uim_ptr->id].logical_slot != uim_ptr->logical_id)
  {
    return TRUE;
  }
  
  if(phy_slot_status[uim_ptr->id].activity_status!= uim_ptr->activity_status)
  {
    return TRUE;
  }
  return FALSE;
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


/*===========================================================================
FUNCTION UIM_PRINT_SUB_MANAGER_GLOBALS

DESCRIPTION:
  Function to print sub manager globals

LIMITATIONS:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
===========================================================================*/
static void uim_print_sub_mananger_globals(uim_instance_global_type *uim_ptr)
{
  int    i              = 0;
  int    j              = 0;
  uint32 iccid_digit[3] = {0, 0, 0};

  if(uim_ptr == NULL)
  {
    return;
  }

  rex_enter_crit_sect(&uim_slot_mapping.sm_data.critical_section);
  UIMDRV_MSG_HIGH_2(uim_ptr->id,"Max_Active_slots-0x%x, Active_slots-0x%x", 
                                  uim_slot_mapping.sm_data.max_active_slots,
                                  uim_slot_mapping.sm_data.active_slots_num);
    
  for(i = 0; i < uim_slot_mapping.no_of_slots_supported; i++)
  {
    iccid_digit[0] = (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[0] << 24) |
                     (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[1] << 16) |
                     (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[2] << 8)  |
                     (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[3]);  

    iccid_digit[1] = (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[4] << 24) |
                     (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[5] << 16) |
                     (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[6] << 8)  |
                     (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[7]);  

    iccid_digit[2] = (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[8] << 8) |
                     (uim_slot_mapping.sm_data.phy_slot_info[i].iccid[9]);
	
    UIMDRV_MSG_HIGH_7(uim_ptr->id,"PHY_ID-0x%x, LOGICAL_ID-0x%x, ACTIVITY_STATUS-0x%x, iccid_len = 0x%x, ICCID : 0x%x%x%x",
                      (i+1),
                      uim_slot_mapping.sm_data.phy_slot_info[i].logical_slot, 
                      uim_slot_mapping.sm_data.phy_slot_info[i].activity_status, 
                      uim_slot_mapping.sm_data.phy_slot_info[i].iccid_len,
                      iccid_digit[0],
                      iccid_digit[1],
                      iccid_digit[2]);
  }
  rex_leave_crit_sect(&uim_slot_mapping.sm_data.critical_section);
}/*uim_print_sub_mananger_globals*/
