/*============================================================================
  FILE:         uim_slot_mapping.c

  OVERVIEW:
   The file contains the slot mapping related APIs.

  EXTERNALIZED FUNCTIONS:
    uim_switch_slot_mapping
    Api to switch logical to physical slot mapping.

    uim_get_logical_to_physical_mapping
    Api to query the current slot mapping

  DEPENDENCIES: N/A

                Copyright (c) 2016 - 2018, 2020 QUALCOMM Technologies, Inc(QTI) and 
                its Licensors.All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimdrv_slot_mapping.c#2 $
$DateTime: 2020/02/23 22:25:47 $
$Author: pwbldsvc $

when          who        what, where, why
--------      ----       ---------------------------------------------------------
02/20/20      rkg        Trigger power-up for inactive slot
02/06/18      ku         Do not update EFS for no of active slots
12/28/17      gm         Update only active slots in uim_get_hw_slot_info
05/10/17      kv         Enable subs manager and sam card mode
05/10/17      ku         Switch  card mode if no MF is indicated in ATR
04/24/17      ks         Slot mapping EFS read fail support
04/14/17      kv         Added pass-through mode support
01/19/17      ks         UIM automatic slot mapping support
09/22/16      gm         Added support for set APDU behavior
06/23/16      sam        F3 messages optimization
04/21/16      sam        Modification of slot mapping state enum names
04/20/16      sam        Support for enhanced subscription manager
03/28/16      sam        Null check for uim_ptr in slot conversion functions
03/28/16      sam        Initial Revision
============================================================================*/
#include "rex.h"
#include "fs_public.h"
#include "err.h"
#include "uim_common_efs.h"
#include "uimdrv_msg.h"
#include "uimdrv_main.h"
#include "uimglobals.h"
#include "uim_remote.h"
#include "uim_slot_mapping.h"


/*slot mapping global*/
uim_slot_mapping_global_type uim_slot_mapping;


/**
*  This function is used to validate and set any unassigned slot mapping
*
*
*  @param num_slots   num of slots for which the mapping is present in the table 
*  @param *slot_ptr   slot mappin where the index is the logical_ID and value is the physical ID

*  @return boolean   TRUE: slot mapping is valid and any unassigend slots are mapped
                     FALSE: slot mapping is invalid
*/
static boolean uim_validate_and_set_slot_mapping(byte num_slots, byte *slot_ptr);


/**
*  This function invoked by each task to ifnor its readiness to switch slot.
   It also checks if all the slots are ready to switch the logical slot and sets the 
   OK_TO_SWITCH_SIG to all clients.

*  the concurency between all task needs to  be maintaned before assigning 
*  the new logical_ID to that physical task. 
   
* otherwise there is a possiblity that 2 tasks may end up with the sam logical ID  due to race condition
* (one task has processecd the switch request assigend the new Logical ID. 
*  But the task with sam ID in older mapping might still not have completed.
*  leading to both tasks (2 physical slots having same Logical ID)
*
*  @param instance ID   Physical ID of the caller
*/
static void uim_ok_to_switch_logical_slot(uim_instance_enum_type instance_id);


/**
*  this is a custom reprot callback for a command which does the 
  slot conversion to LOGICAL ID and call the client call back
*
*  @param reprot_ptr   Pointer to the report_buf
*/
static void uim_slot_mapping_translate_cmd_rpt_cb (uim_rpt_type *report_ptr);


/**
*  This function is used to update the slot mapping state
   to IDLE if all the physical slots have reported complete.

   each task (physical slot) invokes this function to report that 
    the mapping is completed
*
*  @param slot_id   Physical ID 
*/
static void update_slot_mapping_state(uim_instance_enum_type instance_id);


/**
*  the function is used to clear all the pending signals, globals of 
   the previous session
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.
*  @param uim_ptr      Pointer to the current instance of the UIM global structure
*/
static void uim_clean_pending_signals_on_switch_slot
(
  rex_sigs_type *sig_mask_ptr,
  uim_instance_global_type *uim_ptr
);


/**
*  This function is used to print the currrent slot mapping
*/
static void print_slot_mapping(void);


/**
*  This function is used to print the currrent slot mapping
*/
static void print_slot_mapping(void)
{
  byte i =0;
  UIM_MSG_HIGH_1("current_slot_mapping_state-0x%x. new slot mapping:", uim_slot_mapping.state);

  for(i=0; i<UIM_MAX_NUMBER_INSTANCES; i++)
  {
    UIM_MSG_HIGH_3("logical_id-0x%x : Physical_id-0x%x. slot powered down-0x%x", 
                      uim_slot_mapping.logical_slot_info[i].logical_id, 
                      uim_slot_mapping.logical_slot_info[i].phy_id, 
                      uim_slot_mapping.logical_slot_info[i].logical_slot_powered_down);
  }

}/*print_slot_mapping*/


/**
*  This function is used to update the slot mapping state
   to IDLE if all the physical slots have reported complete.

   each task (physical slot) invokes this function to report that 
    the mapping is completed
*
*  @param slot_id   Physical ID 
*/
static void update_slot_mapping_state(uim_instance_enum_type instance_id)
{
  byte sup_slots = uim_slot_mapping.no_of_slots_supported;
  byte i =0;
 
  rex_enter_crit_sect(&uim_slot_mapping.critical_section);

  /*this Physical slot has been assigned*/
  uim_slot_mapping.phy_slot_assigned[instance_id] = TRUE;

  /*check if all concerned slots have finished mapping*/
  for(i=0; i<sup_slots; i++)
  {
    if(uim_slot_mapping.phy_slot_assigned[i] == FALSE)
    {
      rex_leave_crit_sect(&uim_slot_mapping.critical_section);
      return;
    }
  }

  /*All slots have compelted mapping move to IDLE state. 
    so that further switch slot requests are accepted*/
  uim_slot_mapping.state= UIM_SLOTMAP_ST_IDLE;
  UIM_MSG_HIGH_1("update_slot_mapping_state. state changed to -0x%x", uim_slot_mapping.state);
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);
  return;
}/*update_slot_mapping_state*/


/**
*  This function is used to validate and set any unassigned pslot mapping.
   unassigned physical slots will be implicitly assigned ot the remaining logical slots in chronological order.
*
*
*  @param num_slots   num of slots for which the mapping is present in the table. 
                      if it is less than the enabled slots(sup_slots), 
                      then the remaining are unassigend slots and will be assigend in this function
*  @param *slot_ptr   slot mappin where the index is the logical_ID and value is the physical ID

*  @return boolean   TRUE: slot mapping is valid and any unassigend slots are mapped
                     FALSE: slot mapping is invalid
*/
static boolean uim_validate_and_set_slot_mapping(byte num_slots, byte *slot_ptr)
{
  byte            i                                           = 0;
  byte            j                                           = 0;
  uim_slot_type   phy_id                                      = UIM_SLOT_NONE;
  byte            sup_slots                                   = 0;
  boolean         phy_slot_assigned[UIM_MAX_NUMBER_INSTANCES] = {FALSE, FALSE, FALSE, FALSE};
  
  UIM_MSG_HIGH_1("uim_validate_and_set_slot_mapping. num_slots-0x%x", num_slots);
  
  /*Report fail if the pointer is NULL*/
  if(NULL == slot_ptr)
  {
    return FALSE;
  }

  sup_slots = uim_slot_mapping.no_of_slots_supported;
  
  /*num_slots cannot be greater than the enabeld slots if so return error*/
  if(sup_slots < num_slots)
  {
    UIM_MSG_HIGH_2("uim_validate_and_set_slot_mapping. invalid num_slots:0x%x > sup_slots:0x%x", num_slots, sup_slots);
    return FALSE;
  }

  for(i=0; (i<UIM_MAX_NUMBER_INSTANCES); i++)
  { 
    phy_id = (uim_slot_type)slot_ptr[i];
    if(i<sup_slots)
    {
      /*check validity of the already assigned slots*/
      if(i<num_slots)
      {
        /*if the physical slot ID value is disabled or invalid return error*/
        if((sup_slots < (byte)phy_id) || (UIM_SLOT_NONE >= phy_id))
        {
          return FALSE;
        }

        /*check if the same physical slot ID is being assigned twice*/
        if(FALSE == phy_slot_assigned[(phy_id - 1)])
        {
          phy_slot_assigned[(phy_id - 1)] = TRUE;
        }
        else
        {
          /*if the same ID is assigned twice. return error*/
          return FALSE;
        }
      }
      else
      {
        /*assign unassigend physical slots in chronological order to the unassigend logical slots*/
        for(j=0; j<sup_slots; j++)
        {
          if(FALSE == phy_slot_assigned[j])
          {
            slot_ptr[i] = j+1;
            phy_slot_assigned[j] = TRUE;
            break;
          }
        }
      }
    }
    else
    {
      /*set the physical slot ID to 0 for the invalid logical  IDs*/
      slot_ptr[i]  = 0;
    }
  }  
  return TRUE;
}/*uim_validate_and_set_slot_mapping*/


/**
*  This function is used to intialize the slot mapping during device bootup
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*/
void uimdrv_slot_mapping_init(uim_instance_global_type *uim_ptr)
{
  boolean                                 set_default      = FALSE;
  byte                                    i                = 0;
  byte                                    sup_slots        = 0;
  uim_common_efs_status_enum_type         efs_read_state   = UIM_COMMON_EFS_SUCCESS;
  uim_extended_slot_mapping_efs_data_type uim_slot_mapping_efs_data;
  boolean                                 is_automatic_slot_mapping_needed  = FALSE;
  uim_slot_mapping_preference_table_efs_data_type  uim_slot_mapping_preference_table;

  if(NULL == uim_ptr)
  {
    return;
  }

  RETURN_IF_INSTANCE_INVALID(uim_ptr->id);

  /*Set the supported slots to number of enabled slots*/
  uim_slot_mapping.no_of_slots_supported = (byte)uimdrv_hw_nv_config_info.number_of_active_interfaces;
  sup_slots = uim_slot_mapping.no_of_slots_supported;

  UIMDRV_MSG_HIGH_1(uim_ptr->id, "uim_slot_mapping_init. no_of_slots_sup:0x%x", sup_slots);

  /*all uim tasks invoke this function. intialize only once*/
  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  if (UIM_SLOTMAP_ST_NOT_INTIALIZED == uim_slot_mapping.state)
  {
    /*get the slot mapping from EFS*/
    uim_common_efs_read(UIM_COMMON_EFS_AUTOMATIC_SLOT_MAPPING_NEEDED,
                        UIM_COMMON_EFS_ITEM_FILE_TYPE,
                        UIM_COMMON_EFS_DEVICE,
                        (uint8 *)&(is_automatic_slot_mapping_needed),
                        sizeof(is_automatic_slot_mapping_needed));

    efs_read_state = uim_common_efs_read(UIM_COMMON_EFS_SLOT_MAPPING_PREFERENCES_TABLE,
                                         UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                         UIM_COMMON_EFS_DEVICE,
                                         (uint8 *)&(uim_slot_mapping_preference_table),
                                         sizeof(uim_slot_mapping_preference_table));

    if ( (UIM_COMMON_EFS_SUCCESS != efs_read_state) ||  
         (is_automatic_slot_mapping_needed == FALSE) )
    {
      efs_read_state = uim_common_efs_read(UIM_COMMON_EFS_UIMDRV_EXTENDED_SLOT_MAPPING,
                                           UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                           UIM_COMMON_EFS_DEVICE,
                                           (uint8 *)&(uim_slot_mapping_efs_data),
                                           sizeof(uim_extended_slot_mapping_efs_data_type));
    }
    else
    {
      for(i =0; i<sup_slots && i<uim_slot_mapping_preference_table.total_logical_slots; i++)
      {
        uim_slot_mapping_efs_data.logical_slot_config[i] = 
          uim_slot_mapping_preference_table.slot_mapping[i][0];
      }
      uim_slot_mapping_efs_data.active_slots_num = uim_slot_mapping_preference_table.total_logical_slots;
    }

    if (UIM_COMMON_EFS_SUCCESS == efs_read_state)
    {
      /*check if the mapping is valid. if not set default mapping*/
      if(!uim_validate_and_set_slot_mapping(uim_slot_mapping_efs_data.active_slots_num, uim_slot_mapping_efs_data.logical_slot_config))
      {
        set_default = TRUE;
        UIMDRV_MSG_HIGH_0(uim_ptr->id, "uim_slot_mapping_init. invalid mapping SET DEFAULT");
      }
      else if(!uim_is_feature_sam_enabled())
      {
        /*check if the num_active_slots is valid. if not set to max_active_Slots and update hte efs*/
        (void)uim_sm_validate_and_update_num_active_slots_value(&uim_slot_mapping_efs_data.active_slots_num);
      }
    }
    else
    {
      /*if efs read failed set to default*/
      set_default = TRUE;
      UIMDRV_MSG_HIGH_0(uim_ptr->id, "uim_slot_mapping_init.efs read failed SET DEFAULT");
    }
  
    if(TRUE == set_default)
    {
      memset(&uim_slot_mapping_efs_data, 0, sizeof(uim_extended_slot_mapping_efs_data_type));
      uim_slot_mapping_efs_data.version = 0;

      /*set active slots to max active slots*/
      (void)uim_sm_validate_and_update_num_active_slots_value(&uim_slot_mapping_efs_data.active_slots_num);
      /*set default mapping. (1-1(L[0]=1), 2-2(L[1]=2), ...)*/
      for(i=0; i<UIM_MAX_NUMBER_INSTANCES; i++)
      {
        if(i<sup_slots)
        {
          uim_slot_mapping_efs_data.logical_slot_config[i] = i+1;
        }
        else
        {
          uim_slot_mapping_efs_data.logical_slot_config[i] = 0;
        }
      }

     is_automatic_slot_mapping_needed = FALSE;

     /* Auto slot mapping is not needed incase default configuration is applied */
      uim_common_efs_write(UIM_COMMON_EFS_AUTOMATIC_SLOT_MAPPING_NEEDED,
                           UIM_COMMON_EFS_ITEM_FILE_TYPE,
                           UIM_COMMON_EFS_DEVICE,
                           (char *)&(is_automatic_slot_mapping_needed),
                           sizeof(is_automatic_slot_mapping_needed));
    }

    /*update the slot_mapping_global with this mapping*/
    for(i=0; i<UIM_MAX_NUMBER_INSTANCES; i++)
    {
      uim_slot_mapping.logical_slot_info[i].logical_id = (uim_slot_type)(i+1);
      uim_slot_mapping.logical_slot_info[i].phy_id     = (uim_slot_type)(uim_slot_mapping_efs_data.logical_slot_config[i]);
    }

    /*change the state to in progress*/
    uim_slot_mapping.state = UIM_SLOTMAP_ST_IN_PROGRESS;

    print_slot_mapping();

    /*update sub mamager globals with latest activity status of each slot*/
    uim_sm_update_phy_slot_activity_info(uim_slot_mapping_efs_data.active_slots_num,uim_slot_mapping_efs_data.logical_slot_config);
  }	 

  /*set the logical slot for each uim task(physical slot)*/
  uim_ptr->logical_id = UIM_SLOT_NONE;
	
  for(i=0; i<UIM_MAX_NUMBER_INSTANCES; i++)
  {
    if((uim_slot_type)(uim_ptr->id+1) == uim_slot_mapping.logical_slot_info[i].phy_id)
    {
      uim_ptr->logical_id = uim_slot_mapping.logical_slot_info[i].logical_id;
      break;
    }
  }
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);
  
  /*indacate that this tasks slot mapping is completed*/
  update_slot_mapping_state(uim_ptr->id);
}/* uimdrv_slot_mapping_init */


/**
*  This API is invoked by clients to change/set the logical to 
  physical slot mapping in UIM drivers
*
*  @param logical_slot_config   Pointer to slot configuration

*  @return uim_rpt_status    UIM_PASS- if the slot mapping is accepted
                             UIM_FAIL-  if Slot mapping is rejected
*/
uim_rpt_status uim_switch_slot_mapping(uim_logical_slot_config_type *logical_slot_config_ptr,
                                       uim_subscription_manager_client_enum_type client)
{
  byte                                     i                 = 0; 
  uim_instance_global_type                 *uim_ptr          = NULL;
  uim_common_efs_status_enum_type          efs_write_status  = UIM_COMMON_EFS_SUCCESS;
  uim_extended_slot_mapping_efs_data_type  uim_slot_mapping_efs_data;
  boolean                                  is_automatic_slot_mapping_needed = FALSE;

  byte sup_slots = uim_slot_mapping.no_of_slots_supported;
	
  /*Report fail if the pointer is NULL*/
  if(NULL == logical_slot_config_ptr)
  {
    return UIM_FAIL;
  }

  UIM_MSG_HIGH_1("uim_switch_slot_mapping from client 0x%x", client);

  if(client == UIMDRV_SUB_MGR_CLIENT_QMI_UIM)
  {
    uim_common_efs_write(UIM_COMMON_EFS_AUTOMATIC_SLOT_MAPPING_NEEDED,
                         UIM_COMMON_EFS_ITEM_FILE_TYPE,
                         UIM_COMMON_EFS_DEVICE,
                         (char *)&(is_automatic_slot_mapping_needed),
                         sizeof(is_automatic_slot_mapping_needed));
  }

  /*Reort fail if the HW NV config is not yet processsed*/
  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  if (UIM_SLOTMAP_ST_IDLE != uim_slot_mapping.state)
  {
    UIM_MSG_HIGH_1("uim_switch_slot_mapping reject request. current state not idle. it is -0x%x", uim_slot_mapping.state);
    rex_leave_crit_sect(&uim_slot_mapping.critical_section);
    return UIM_FAIL;
  }
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);

  memset(&uim_slot_mapping_efs_data, 0, sizeof(uim_extended_slot_mapping_efs_data_type));
  
  uim_memscpy(uim_slot_mapping_efs_data.logical_slot_config,
              sizeof(uim_slot_mapping_efs_data.logical_slot_config),
             logical_slot_config_ptr->slot, 
             sizeof(logical_slot_config_ptr->slot));
  
#ifdef FEATURE_UIM_TEST_FRAMEWORK
  #error code not present
#endif

  /*validate the slot mapping recived. if valid set the unassigned slots implicitly*/  
  if(FALSE == uim_validate_and_set_slot_mapping(logical_slot_config_ptr->num_slots, uim_slot_mapping_efs_data.logical_slot_config))
  {
    UIM_MSG_HIGH_0("uim_switch_slot_mapping reject request. invalid slot mpping");
    return UIM_FAIL;
  }

  uim_slot_mapping_efs_data.version = 0;
  uim_slot_mapping_efs_data.active_slots_num = logical_slot_config_ptr->num_slots;

   /*check if the num_active_slots is valid. if not set to max_active_slots*/
  (void)uim_sm_validate_and_update_num_active_slots_value(&uim_slot_mapping_efs_data.active_slots_num);

  /*update the EFS with the new slot mapping*/
  efs_write_status= uim_common_efs_write(UIM_COMMON_EFS_UIMDRV_EXTENDED_SLOT_MAPPING,
                                         UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                         UIM_COMMON_EFS_DEVICE,
                                         (char *)&(uim_slot_mapping_efs_data),
                                         sizeof(uim_slot_mapping_efs_data));
  UIM_MSG_HIGH_1("uim_switch_slot_mapping efs write SUCCESS/Fail - 0x%x", efs_write_status);

  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  /*update the slot mapping global with the new logical slot info*/
  for(i =0; i<UIM_MAX_NUMBER_INSTANCES; i++)
  {
    uim_slot_mapping.logical_slot_info[i].logical_id = (uim_slot_type)(i+1);
    uim_slot_mapping.logical_slot_info[i].phy_id = (uim_slot_type)uim_slot_mapping_efs_data.logical_slot_config[i];
    if(i <sup_slots)
    {
      uim_slot_mapping.phy_slot_ready_to_switch_vote[i].required = TRUE;
      uim_slot_mapping.phy_slot_ready_to_switch_vote[i].value = FALSE;
    }
  }  

  /*clear the physical slot voting global*/
  memset(uim_slot_mapping.phy_slot_assigned, 0, sizeof(uim_slot_mapping.phy_slot_assigned));
  
  /*set the slot mapping state to in progress*/
  uim_slot_mapping.state = UIM_SLOTMAP_ST_IN_PROGRESS;

  print_slot_mapping();
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);

  /*update sub mamager globals with latest activity status of each slot*/
  uim_sm_update_phy_slot_activity_info(uim_slot_mapping_efs_data.active_slots_num,uim_slot_mapping_efs_data.logical_slot_config);

  /*post switch slot signal to the uim tasks*/
  for(i =0; i<sup_slots; i++)
  {
    uim_ptr = uim_get_instance_ptr((uim_instance_enum_type)((uim_slot_type)(uim_slot_mapping_efs_data.logical_slot_config[i]- 1)));
    if(NULL != uim_ptr)
    {
      (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_SWITCH_LOGICAL_SLOT_SIG);
    }
  }
  return UIM_PASS;
}


/**
* This API is invoked by clients to get the current logical to 
  physical slot mapping.
*
*  @param logical_slot_config   Pointer to slot configuration which will be filled in this function

*  @return uim_rpt_status     UIM_PASS-  if valid slot mapping is being returned
                              UIM_FAIL-  if existing slot mapping is invalid
*/
uim_rpt_status uim_get_logical_to_physical_mapping(uim_logical_slot_config_type *logical_slot_config)
{
  byte sup_slots = uim_slot_mapping.no_of_slots_supported;
  byte i = 0;
  
  if(NULL == logical_slot_config)
  {
    return UIM_FAIL;
  }

  UIM_MSG_MED_0("uim_get_logical_to_physical_mapping");

  /*Reort fail if the slot mapping is not yet intialized processsed*/
  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  if(UIM_SLOTMAP_ST_NOT_INTIALIZED == uim_slot_mapping.state)
  {
    rex_leave_crit_sect(&uim_slot_mapping.critical_section);
    return UIM_FAIL;
  }

  logical_slot_config->num_slots = sup_slots;
  
  for(i=0; i<UIM_MAX_NUMBER_INSTANCES; i++)
  {
    logical_slot_config->slot[i] = uim_slot_mapping.logical_slot_info[i].phy_id;
  }
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);
  return UIM_PASS;
}/*uim_switch_slot_mapping*/


/**
* This API is to get the number of active slots.
*
*  @param  max_num_slot_supported   Max number of slot supported

*  @return uim_rpt_status           Number of active slots
*/
uint8 uim_get_num_active_slot(uint8 max_num_slot_supported)
{
  byte                                             i                                 = 0;
  byte                                             sup_slots                         = 0;
  uim_common_efs_status_enum_type                  efs_read_state                    = UIM_COMMON_EFS_SUCCESS;
  boolean                                          is_automatic_slot_mapping_needed  = FALSE;
  uim_slot_mapping_preference_table_efs_data_type  uim_slot_mapping_preference_table;  
  uim_extended_slot_mapping_efs_data_type          uim_slot_mapping_efs_data;

  memset(&uim_slot_mapping_efs_data, 0x00, sizeof(uim_slot_mapping_efs_data));
  memset(&uim_slot_mapping_preference_table, 0x00, sizeof(uim_slot_mapping_preference_table));

  /* get the slot mapping from EFS */
  (void) uim_common_efs_read(UIM_COMMON_EFS_AUTOMATIC_SLOT_MAPPING_NEEDED,
                      UIM_COMMON_EFS_ITEM_FILE_TYPE,
                      UIM_COMMON_EFS_DEVICE,
                      (uint8 *)&(is_automatic_slot_mapping_needed),
                      sizeof(is_automatic_slot_mapping_needed));
  
  efs_read_state = uim_common_efs_read(UIM_COMMON_EFS_SLOT_MAPPING_PREFERENCES_TABLE,
                                       UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                       UIM_COMMON_EFS_DEVICE,
                                       (uint8 *)&(uim_slot_mapping_preference_table),
                                       sizeof(uim_slot_mapping_preference_table));
  
  if ( (UIM_COMMON_EFS_SUCCESS != efs_read_state) ||  
       (is_automatic_slot_mapping_needed == FALSE) )
  {
    efs_read_state = uim_common_efs_read(UIM_COMMON_EFS_UIMDRV_EXTENDED_SLOT_MAPPING,
                                         UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                         UIM_COMMON_EFS_DEVICE,
                                         (uint8 *)&(uim_slot_mapping_efs_data),
                                         sizeof(uim_extended_slot_mapping_efs_data_type));

    if(UIM_COMMON_EFS_SUCCESS != efs_read_state)
    {
      uim_slot_mapping_efs_data.active_slots_num = max_num_slot_supported;
    }                                         
  }
  else
  {
    uim_slot_mapping_efs_data.active_slots_num = uim_slot_mapping_preference_table.total_logical_slots;
  }
    
  return uim_slot_mapping_efs_data.active_slots_num;
}/* uim_get_num_active_slot */


/**
*  This function invoked by each task to ifnor its readiness to switch slot.
   It also checks if all the slots are ready to switch the logical slot and sets the 
   OK_TO_SWITCH_SIG to all clients.

*  the concurency between all task needs to  be maintaned before assigning 
*  the new logical_ID to that physical task. 
   
* otherwise there is a possiblity that 2 tasks may end up with the sam logical ID  due to race condition
* (one task has processecd the switch request assigend the new Logical ID. 
*  But the task with sam ID in older mapping might still not have completed.
*  leading to both tasks (2 physical slots having same Logical ID)
*
*  @param instance ID   Physical ID of the caller
*/
static void uim_ok_to_switch_logical_slot(uim_instance_enum_type instance_id)
{
  byte i=0;
  uim_instance_global_type *uim_ptr;
  boolean set_sig[UIM_MAX_NUMBER_INSTANCES] = {FALSE, FALSE, FALSE, FALSE};
	
  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  /*vote that the current physical slot is ready to switch the slot*/
  if(uim_slot_mapping.phy_slot_ready_to_switch_vote[instance_id].required == TRUE)
  {
    uim_slot_mapping.phy_slot_ready_to_switch_vote[instance_id].value = TRUE;
  }

  UIMDRV_MSG_HIGH_2(instance_id,"uim_ok_to_switch_logical_slot. vote req-0x%x , vote value-0x%x  ",
                             uim_slot_mapping.phy_slot_ready_to_switch_vote[instance_id].required, 
		              uim_slot_mapping.phy_slot_ready_to_switch_vote[instance_id].value);

  /*chek if all concerned slots are ready to switch slots*/
  for(i=0; i<UIM_MAX_NUMBER_INSTANCES; i++)
  {
    if(uim_slot_mapping.phy_slot_ready_to_switch_vote[i].required == TRUE)
    {
      set_sig[i] = TRUE;
      if(uim_slot_mapping.phy_slot_ready_to_switch_vote[i].value == FALSE)
      {
        rex_leave_crit_sect(&uim_slot_mapping.critical_section);
        return;
      }
    }
  }

  /*All slots aready to switch. reset voting global*/
  memset(uim_slot_mapping.phy_slot_ready_to_switch_vote, 0, sizeof(uim_slot_mapping.phy_slot_ready_to_switch_vote));
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);

  /*set ok to switch for the concerned slots*/
  UIMDRV_MSG_HIGH_0(instance_id,"uim_ok_to_switch_logical_slot . ALL slots ARE OK");

  for(i=0; i<UIM_MAX_NUMBER_INSTANCES; i++)
  {
    if(set_sig[i] == TRUE)
    {
      uim_ptr = uim_get_instance_ptr((uim_instance_enum_type)i);
      if(NULL != uim_ptr)
      {
        (void) rex_set_sigs( uim_ptr->tcb_ptr, UIM_READY_TO_SWITCH_SIG);
      }
    }
  }
}/*uim_ok_to_switch_logical_slot*/


 /**
*  This function converts the physical ID ot logical ID
*
*
*  @param slot_id   Physical ID 

*  @return uim_slot_type   Logical ID of that slot
*/
uim_slot_type uim_convert_physical_to_logical_slot_id(uim_slot_type slot_id)
{
  uim_instance_global_type *uim_ptr  = NULL;
  
  if ((UIM_SLOT_NONE == slot_id) || ( UIM_SLOT_AUTOMATIC <= slot_id))
  {
    UIM_MSG_ERR_1("Invalid Slot Reference 0x%x", slot_id);
    return UIM_SLOT_NONE;
  }

  uim_ptr = uim_get_instance_ptr((uim_instance_enum_type)(slot_id - 1));
  if(NULL == uim_ptr)
  {
    return UIM_SLOT_NONE;
  }
  else
  {
    return (uim_ptr->logical_id);
  }
}/*uim_convert_physical_to_logical_slot_id*/


/**
*  This function converts the logical ID to physical ID
*
*
*  @param slot_id   Logical ID 

*  @return uim_slot_type   physical ID of that slot
*/
uim_slot_type uim_convert_logical_to_physical_slot_id(uim_slot_type slot_id)
{
  uint8 i = 1;
  uim_slot_type return_id = UIM_SLOT_NONE;
  uim_slot_type temp_id = UIM_SLOT_NONE;
  uim_instance_global_type *uim_ptr  = NULL;

  if ((UIM_SLOT_NONE == slot_id) || ( UIM_SLOT_AUTOMATIC <= slot_id))
  {
    UIM_MSG_ERR_1("Invalid Slot Reference 0x%x", slot_id);
    return UIM_SLOT_NONE;
  }
  
  for(i=1; i<=UIM_NUM_DRV_SLOTS; i++)
  {
    uim_ptr = uim_get_instance_ptr((uim_instance_enum_type)(i - 1));
    if(NULL == uim_ptr)
    {
      continue;
    }
    temp_id = uim_ptr->logical_id;
    if(temp_id == slot_id)
    {
      return_id = (uim_slot_type)i;
      break;
    }
  }
  return return_id;
}/*uim_convert_logical_to_physical_slot_id*/
  

/**
*  when a command is posted to UIM a callback is also given by cilent
*  in callback the slot ID needs to be reported is logical ID.
*
* So, in order to report LOGICAL ID at the time of queue itself, this function is invoked 
* to replace the client reprot call back witha a new callback 
* 
* this callback will convert the ID to logica lID and call the client callback
*
*  @param cmd_ptr   Pointer to the current command 
*/
uim_slot_mapping_cmd_saved_data_type* uim_slot_mapping_translate_cmd
(
  uim_cmd_type *cmd_ptr
)
{
  uim_slot_mapping_cmd_saved_data_type *slot_mapping_saved_client_info = NULL;

  if(NULL == cmd_ptr)
  {
    return NULL;
  }
  /* Send all external commands to the physical slot ID but save the client's
     data to restore in the response before calling client's rpt_function */
  slot_mapping_saved_client_info = uim_malloc(sizeof(uim_slot_mapping_cmd_saved_data_type));

  if(slot_mapping_saved_client_info != NULL)
  {
    slot_mapping_saved_client_info->rpt_function = cmd_ptr->hdr.rpt_function;
    slot_mapping_saved_client_info->userdata	 = cmd_ptr->hdr.user_data;
    cmd_ptr->hdr.options                        |= UIM_OPTION_ALWAYS_RPT;
    cmd_ptr->hdr.slot 				 = uim_convert_logical_to_physical_slot_id(cmd_ptr->hdr.slot);
    cmd_ptr->hdr.rpt_function 		         = uim_slot_mapping_translate_cmd_rpt_cb;
    cmd_ptr->hdr.user_data			 = (int32)slot_mapping_saved_client_info;
  }
  return slot_mapping_saved_client_info;
} /* uim_slot_mapping_translate_cmd */



/**
*  this is a custom reprot callback for a command which does the 
  slot conversion to LOGICAL ID and call the client call back
*
*  @param reprot_ptr   Pointer to the report_buf
*/
static void uim_slot_mapping_translate_cmd_rpt_cb
(
  uim_rpt_type *report_ptr
)
{
  uim_slot_mapping_cmd_saved_data_type *slot_mapping_saved_client_info = NULL;

  if(NULL == report_ptr)
  {
    return;
  }
  
  /* Get the saved data we stored during the cmd translation */
  slot_mapping_saved_client_info = (uim_slot_mapping_cmd_saved_data_type*)report_ptr->user_data;

  if(slot_mapping_saved_client_info == NULL)
  {
    return;
  }

  /* Restore user data and always report using LOGICAL_ID to clients of UIM */
  report_ptr->user_data = slot_mapping_saved_client_info->userdata;
  report_ptr->slot      = uim_convert_physical_to_logical_slot_id(report_ptr->slot);

  /* Execute the callback saved from the client */
  if(slot_mapping_saved_client_info->rpt_function != NULL)
  {
    (*(slot_mapping_saved_client_info->rpt_function))(report_ptr);
  }

  /* free the saved command data entry */
  UIM_FREE(slot_mapping_saved_client_info);
  report_ptr->user_data = 0;
}/*uim_slot_mapping_translate_cmd_rpt_cb*/ 


/**
*  this is used for freeing user data when callback is skipped
*
*  @param cmd_ptr   Pointer to the command
*/
void uim_slot_mapping_free_userdata
(
  uim_cmd_type *cmd_ptr
)
{
  uim_slot_mapping_cmd_saved_data_type *slot_mapping_saved_client_info_ptr = NULL;

  if(NULL == cmd_ptr)
  {
    return;
  }

  if (cmd_ptr->hdr.rpt_function == uim_slot_mapping_translate_cmd_rpt_cb)
  {
    slot_mapping_saved_client_info_ptr = (uim_slot_mapping_cmd_saved_data_type*)cmd_ptr->hdr.user_data;
    UIM_FREE(slot_mapping_saved_client_info_ptr);
  }
}/*uim_slot_mapping_free_userdata*/


/**
*  the function is used to clear all the pending signals, globals of 
   the previous session
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.
*  @param uim_ptr      Pointer to the current instance of the UIM global structure
*/
static void uim_clean_pending_signals_on_switch_slot
(
  rex_sigs_type *sig_mask_ptr,
  uim_instance_global_type *uim_ptr
)
{
  uim_cmd_type   *hos_cmd_ptr      = NULL; /* Pointer to received command */

  UIMDRV_MSG_HIGH_0(uim_ptr->id, "uim_clean_pending_signals_on_switch_slot");

  uim_clear_poll_timer(uim_ptr);
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_POLL_TIMER_SIG);
  uim_ptr->flag.poll_pending = FALSE;
  *sig_mask_ptr &= ~UIM_POLL_TIMER_SIG; 

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_SIG);
  *sig_mask_ptr &= ~UIM_CMD_RSP_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_RSP_TIMEOUT_SIG);
  *sig_mask_ptr &= ~UIM_CMD_RSP_TIMEOUT_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_TRANSACTION_SIG);
  *sig_mask_ptr &= ~UIM_TRANSACTION_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_CMD_Q_SIG);
  *sig_mask_ptr &= ~UIM_CMD_Q_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_SIMULATE_NULL_TIMER_EXP_SIG);
  *sig_mask_ptr &= ~UIM_SIMULATE_NULL_TIMER_EXP_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_FETCH_PENDING_SIG);
  *sig_mask_ptr &= ~UIM_FETCH_PENDING_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_MCGF_NV_REFRESH_SIG);
#ifdef FEATURE_MODEM_CONFIG_REFRESH
  if(*sig_mask_ptr & UIM_MCGF_NV_REFRESH_SIG)
  {
    /* Call MCFG call back that the refresh is done */
    (void)mcfg_refresh_done_w_status(uim_ptr->state.nv_refresh_reference_id,
                                   MCFG_REFRESH_STATUS_INTERNAL_ERROR);
  }
#endif /* FEATURE_MODEM_CONFIG_REFRESH */
  *sig_mask_ptr &= ~UIM_MCGF_NV_REFRESH_SIG;


  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_EXT_RECOVERY_TIMER_EXP_SIG);
  if((TRUE == uim_ptr->extended_recovery.extended_recovery_enabled) && 
     (TRUE == uim_ptr->extended_recovery.extended_recovery_in_progress))
  {
    /*abandon the extended recovery and clear the relevent globals*/
    uim_extended_recovery_event_handler(uim_ptr, UIM_EXTENDED_RECOVERY_EVT_TERMINATE);
  }

  *sig_mask_ptr &= ~UIM_EXT_RECOVERY_TIMER_EXP_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_SUSPICIOUS_CARD_REM_SIG);
  if(*sig_mask_ptr & UIM_SUSPICIOUS_CARD_REM_SIG)
  {
    uim_ptr->hotswap.card_status = UIM_CARD_REMOVED;
    uim_ptr->hotswap.hotswap_card_status = UIM_CARD_REMOVED;
    uim_hotswap_unmask_interrupt(uim_ptr);
  }
  *sig_mask_ptr &= ~UIM_SUSPICIOUS_CARD_REM_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_HOTSWAP_CMD_CARD_REM_SIG);
  if(*sig_mask_ptr & UIM_HOTSWAP_CMD_CARD_REM_SIG)
  {
    hos_cmd_ptr = (uim_cmd_type *)q_get(&uim_ptr->hotswap.hotswap_rem_cmd_q);
    if (hos_cmd_ptr != NULL)
    {
      /* Free the UIM buffer for next request */
      hos_cmd_ptr ->hdr.slot = UIM_SLOT_AUTOMATIC;
      q_put( &uim_ptr->hotswap.hotswap_free_q,
             q_link(hos_cmd_ptr,&hos_cmd_ptr->hdr.cmd_hdr.link));
    }
  }
  *sig_mask_ptr &= ~UIM_HOTSWAP_CMD_CARD_REM_SIG;

  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_HOTSWAP_CMD_CARD_INS_SIG);
  if(*sig_mask_ptr & UIM_HOTSWAP_CMD_CARD_INS_SIG)
  {
    hos_cmd_ptr = (uim_cmd_type *)q_get(&uim_ptr->hotswap.hotswap_ins_cmd_q);
    if (hos_cmd_ptr != NULL)
    {
      /* Free the UIM buffer for next request */
      hos_cmd_ptr ->hdr.slot = UIM_SLOT_AUTOMATIC;
      q_put( &uim_ptr->hotswap.hotswap_free_q, q_link(hos_cmd_ptr,&hos_cmd_ptr->hdr.cmd_hdr.link));
    }
  }
  *sig_mask_ptr &= ~UIM_HOTSWAP_CMD_CARD_INS_SIG;
}/*uim_clean_pending_signals_on_switch_slot*/


/**
*  the function is used to set the logical slot power down flag
*
*  @param uim_ptr      Pointer to the current instance of the UIM global structure
*  @param value        value to be set
*/
void uim_set_logical_slot_power_down_flag
(
  uim_instance_global_type *uim_ptr,
  boolean                   value
)
{
  if((UIM_SLOT_3 < uim_ptr->logical_id) || (UIM_SLOT_NONE >= uim_ptr->logical_id))
  {
    return;
  }

  UIMDRV_MSG_HIGH_2(uim_ptr->id, "uim_set_logical_slot_power_down_flag. logical_slot- 0x%x value - 0x%x",uim_ptr->logical_id, value);
  
  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  uim_slot_mapping.logical_slot_info[uim_ptr->logical_id -1].logical_slot_powered_down = value;
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);
}/*uim_set_logical_slot_power_down_flag*/


/**
*  This function handles the UIM_HANDLE_SWITCH_SLOT_SIG signal.
*
*  SIDE EFFECTS
*    This function may change the value of *imask_ptr or *sig_mask_ptr.
*
*  @param sig_mask_ptr Pointer to the OR's set of signals that UIM task needs to
*                      handle.  Can be updated in this function.
*  @param imask_ptr Pointer to the mask of signals that the uim task is interested
*                   in handling.   The mask may be changed by this function.
*  @param uim_ptr Pointer to the current instance of the UIM global structure
*  @return boolean Return TRUE to indicate that immediately
*          going back to the top of the main loop is desired.
*/
boolean uim_handle_switch_logical_slot_sig
(
  rex_sigs_type *sig_mask_ptr,
  rex_sigs_type *imask_ptr,
  uim_instance_global_type *uim_ptr
)
{   
  byte            i                         = 0;
  byte            num_ch                    = 0;
  rex_sigs_type   sigs_received;
  boolean         logical_slot_powered_down = FALSE;
  
  /*return if instance is invalid*/
  if (uim_ptr->id >= UIM_INSTANCE_INVALID || uim_ptr->id < UIM_INSTANCE_1)
  {
    return TRUE;
  }
  
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_SWITCH_LOGICAL_SLOT_SIG);

  /*return if switch slot processing is not required*/  
  if(!uim_sm_is_swtich_slot_processing_required(uim_ptr))
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"No change in Logical ID or Activity status. Do nothing");

    /*say ok/ready to switch logical slot*/ 
    uim_ok_to_switch_logical_slot(uim_ptr->id);

    sigs_received = uim_pet_and_wait(UIM_READY_TO_SWITCH_SIG , uim_ptr);

    (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_READY_TO_SWITCH_SIG);
    
    /*indicate that the slot mapping for this physical slot is completed*/
    update_slot_mapping_state(uim_ptr->id);

    return FALSE;
  }
  
  if((UIM_SLOT_3 < uim_ptr->logical_id) || (UIM_SLOT_NONE >= uim_ptr->logical_id))
  {
    return TRUE;
  }

  UIMDRV_MSG_MED_3(uim_ptr->id, "current Logical_id-0x%x, Card_mode-0x%x, activity_status-0x%x", uim_ptr->logical_id, uim_ptr->card_mode, uim_ptr->activity_status);
  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  logical_slot_powered_down = uim_slot_mapping.logical_slot_info[uim_ptr->logical_id -1].logical_slot_powered_down;
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);

  /*notify error only if logical slot not powered down*/
  if(logical_slot_powered_down == FALSE)
  {
    if(UIM_SLOT_STATE_ACTIVE == uim_ptr->activity_status)
    {
      uim_notify_error(UIM_CARD_REMOVED_S, uim_ptr);
    }
  }

/* the below order of processing is very important -
   1. power dowm
   2. clear peding signals
   3. send error report for current command
   4. Move the state to error_st
   5. Flush commands in queue
   6. set logical ID to none

any change of sequence may lead to crashes
*/
  uim_power_down(uim_ptr);

  if(IS_BTSAP_HANDLE_ACTIVE(uim_ptr))
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"Calling QMI remote API to power down the remote SIM");
    /*cmd_rsp_sig is set in the error event which is generated due to power down */
    uim_remote_card_power_down(uim_ptr);
    uim_btsap_release_connection_req(uim_ptr);
  }

  /*clear any pending RSP/RSP_timeout or other signals*/
  uim_clean_pending_signals_on_switch_slot(sig_mask_ptr, uim_ptr);

  /*send error report for any ongoing command*/
  if ( (uim_ptr->command.cmd_ptr != NULL) &&  (uim_ptr->flag.command_in_progress == TRUE)   &&
       (!uim_ptr->flag.static_buffer_used) && (uim_ptr->flag.command_requeued == FALSE))
  {
    UIMDRV_MSG_HIGH_2(uim_ptr->id,"Switch_slot- Reporting err for current cmd: 0x%x status: 0x%x",
                      uim_ptr->command.cmd_ptr->hdr.command,
                      uim_ptr->command.cmd_ptr->hdr.status);

    if (((int)uim_ptr->command.cmd_ptr->hdr.options & (int)UIM_OPTION_ALWAYS_RPT) != 0)
    {
      /* Set the status words to default before sending the rpt */
      uim_ptr->command.rpt_buf.sw1 = (uim_sw1_type) 0;
      uim_ptr->command.rpt_buf.sw2 = (uim_sw2_type) 0;
      uim_send_err_rpt(uim_ptr->command.cmd_ptr, uim_ptr);
    }
  }
  
  uim_ptr->state.status = UIM_ERR_S;

  /* Flush immediately all pending command on that slot. inculding power_Down and reset_f commands */
  uim_flush_command(UIM_FLUSH_COMPLETE, uim_ptr);

  /*reset the logical ID to NONE*/
  uim_ptr->logical_id = UIM_SLOT_NONE;
  
  /* Set the flag to false since the command is completed */
  uim_ptr->flag.command_in_progress = FALSE;

  /* Clear out any votes to keep UIM powered on */
  uim_ptr->state.ctl = UIM_NONE;
  
  uim_ptr->flag.powerdown = TRUE;

  (*imask_ptr) |= UIM_CMD_Q_SIG;

  /*set powerup_inactive_slot flag if powerup is ongoing*/
  if(TRUE == uim_ptr->flag.me_powerup)
  {
    uim_ptr->flag.me_powerup = FALSE;
  }

  /* reset the directory struct and power down the UIM */
  uim_reset_dir(uim_ptr);
  
  /* Reset the static buffer used flag */
  uim_ptr->flag.static_buffer_used = FALSE;
  
  /* Reset the static command count */
  uim_ptr->command.static_cmd_buf.hdr.cmd_count = 0;
  /* Reset recovery triggered flag */
  uim_ptr->flag.recovery_triggered = FALSE;
  if(uim_ptr->command.mode == UIM_RECOVERY_MODE)
  {
    uim_ptr->command.mode = UIM_NORMAL_MODE;
  }

  /*say ok/ready to switch logical slot*/ 
  uim_ok_to_switch_logical_slot(uim_ptr->id);

  /*wait for all slots to be ready to switch*/   
  sigs_received = uim_pet_and_wait(UIM_READY_TO_SWITCH_SIG , uim_ptr);
  if(!(sigs_received & UIM_READY_TO_SWITCH_SIG))
  {
    return TRUE;     
  }
  (void) rex_clr_sigs( uim_ptr->tcb_ptr, UIM_READY_TO_SWITCH_SIG);
   
  UIMDRV_MSG_HIGH_0(uim_ptr->id,"ok to switch logical slot. assign logical slot");

  /*assign the new logical ID to this physical slot*/   
  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  for(i=0; i<UIM_MAX_NUMBER_INSTANCES; i++)
  {
    if((uim_slot_type)(uim_ptr->id+1) == uim_slot_mapping.logical_slot_info[i].phy_id)
    {
      uim_ptr->logical_id = uim_slot_mapping.logical_slot_info[i].logical_id;
      break;
    }
  }
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);

  /*Set the new activity status for this slot*/  
  uim_set_activity_status(uim_ptr);

  /*indicate that the slot mapping for this physical slot is completed*/
  update_slot_mapping_state(uim_ptr->id);
   
   
  if(uim_ptr->logical_id == UIM_SLOT_NONE)
  {
    UIMDRV_MSG_HIGH_0(uim_ptr->id,"Invalid logical ID return");
    return TRUE;
  }

  /*set teh card_mode based on the logical slot ID*/
  uim_sam_set_card_mode(imask_ptr,uim_ptr);
   
  UIMDRV_MSG_MED_3(uim_ptr->id, "New Logical_id-0x%x, New Card_mode-0x%x, New activity_status 0x%x", uim_ptr->logical_id, uim_ptr->card_mode, uim_ptr->activity_status);

  rex_enter_crit_sect(&uim_slot_mapping.critical_section);
  logical_slot_powered_down = uim_slot_mapping.logical_slot_info[uim_ptr->logical_id -1].logical_slot_powered_down;
  rex_leave_crit_sect(&uim_slot_mapping.critical_section);

  if(uim_ptr->activity_status == UIM_SLOT_STATE_ACTIVE)
  {
    /*check if the newly assigend logical slot is in powered down state.
    if so, mask hotswap interrupt if nto already masked. and clear any pending interrupts*/ 
    if(logical_slot_powered_down == TRUE)
    {
      UIMDRV_MSG_HIGH_1(uim_ptr->id,"SWITCH_SLOT- logical slot[%d] powered down .not powering up. mask interrupts for this physical slot", uim_ptr->logical_id);
      if((uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE) &&
         (uim_ptr->hotswap.hotswap_me_power_down == FALSE))
      {
        uim_hotswap_reset_slot(uim_ptr);
        uim_hotswap_clear_interrupt(uim_ptr);
        uim_hotswap_mask_interrupt(uim_ptr);
        uim_ptr->hotswap.hotswap_me_power_down = TRUE;
        uim_power_down(uim_ptr);
      }
      /*Notify QMI as we are not going to furhter powerup this slot*/
      uim_sm_notify_clients_slot_status(uim_ptr);
      return TRUE;
    }
    else
    {
      /*we are attempting powerup, unmask hotdwap interrupts if it was masked prior to switch*/
      if (uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE &&
          uim_ptr->hotswap.hotswap_me_power_down == TRUE)
      {
        UIMDRV_MSG_HIGH_1(uim_ptr->id,"SWITCH_SLOT- logical slot[%d] powered down .unmask interrupts for this physical slot as it may be switched ot a different logical slot.", uim_ptr->logical_id);
        uim_ptr->hotswap.hotswap_me_power_down = FALSE;
        uim_ptr->hotswap.card_status = uim_hotswap_query_card_status(uim_ptr);
        uim_ptr->hotswap.hotswap_card_status = uim_ptr->hotswap.card_status;
        uim_ptr->hotswap.hotswap_notify_card_removed = FALSE;
        uim_hotswap_reset_slot(uim_ptr);
        uim_hotswap_clear_interrupt(uim_ptr);
        uim_hotswap_unmask_interrupt(uim_ptr);
      }
    }
  }

  /*Unmask hotswap interrupt if slot is inactive*/
  if(uim_ptr->activity_status == UIM_SLOT_STATE_INACTIVE)
  {
    if (uim_ptr->hotswap.hotswap_me_power_down == TRUE &&
        uim_hw_if.intctrl[uim_ptr->id].m_hs_enable == TRUE)
    {
      UIMDRV_MSG_HIGH_1(uim_ptr->id,"Slot inactive. hotswap is maksed. unmask it.", uim_ptr->logical_id);
      uim_ptr->hotswap.hotswap_me_power_down = FALSE;
      uim_ptr->hotswap.card_status = uim_hotswap_query_card_status(uim_ptr);
      uim_ptr->hotswap.hotswap_card_status = uim_ptr->hotswap.card_status;
      uim_ptr->hotswap.hotswap_notify_card_removed = FALSE;
      uim_hotswap_reset_slot(uim_ptr);
      uim_hotswap_clear_interrupt(uim_ptr);
      uim_hotswap_unmask_interrupt(uim_ptr);
    }
  }

  /*prepare UIM for a powerup*/
  uim_ptr->hotswap.hotswap_notify_card_removed = FALSE;
  uim_reset_globals(uim_ptr);
  uim_ptr->command.static_cmd_buf.hdr.cmd_count = 1;
  uim_ptr->debug.convention_change_count = 1;
  uim_ptr->card_cmd.instrn_toggle_count = 1;
  uim_ptr->command.static_cmd_buf.hdr.slot =
  (uim_slot_type)(uim_ptr->id + 1);
  uim_ptr->flag.me_powerup = TRUE;
  /* The status of UIM is set to Uninitialized since reset the UIM */
  uim_ptr->state.status = UIM_UNINITIALIZED_S;

  uim_reset_uim(imask_ptr, uim_ptr->flag.me_powerup, uim_ptr);
  return FALSE;
} /* uim_handle_switch_logical_slot_sig */
