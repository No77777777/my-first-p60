/*===========================================================================

                         U I M _ S L O T _ M A P P I N G. C

DESCRIPTION

 The uim slot mapping handling source file.

Copyright (c) 2020 QUALCOMM Technologies, Inc(QTI).
All Rights Reserved.
QUALCOMM Technologies Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/uim.mpss/6.1.0/slotmapping/src/uim_slot_mapping.c#2 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/18/20    shp   Added stringl.h for memscpy
04/03/17    ks    Initial Revision.
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "uim_msg.h"
#include "uimsub_manager.h"
#include "uim_slot_mapping.h"
#include "mmgsdilib_p.h"
#include "mmgsdi.h"
#include <stringl/stringl.h>

/*===========================================================================

                             GLOBAL DECLARATIONS AND DEFINATIONS 

===========================================================================*/
/* If slot mapping is in progress, wait for 2sec before retrying */
#define SLOT_MAPPING_IN_PROGRESS_WAIT_TIME_VALUE 2000

/* ----------------------------------------------------------------------------
   STRUCTURE:      SLOT_MAPPING_PREFERENCE_TABLE_EFS_DATA_TYPE

   DESCRIPTION:
     Structure to store slot mapping preferences
-------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  uint8                        total_logical_slots;
  uint8                        slot_mapping[UIM_MAX_SUBSCRIPTIONS_SUPPORTED][UIM_MAX_INSTANCES];
}slot_mapping_preference_table_efs_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      UIM_PHYSICAL_SLOTS_INFO_TYPE

   DESCRIPTION:
     Structure to store physical slot status
-------------------------------------------------------------------------------*/
typedef struct {
  uint8                        num_slots;
  uim_phy_slot_status_type   * slot_status_ptr;
}uim_physical_slots_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SLOT_MAPPING_TYPE

   DESCRIPTION:
     Structure to store physical and logical slots
-------------------------------------------------------------------------------*/
typedef struct
{
  uim_slot_type                logical_slot_id;
  uim_slot_type                physical_slot_id;
}slot_mapping_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SLOT_MAPPING_INFO_TYPE

   DESCRIPTION:
     Structure to store physical and logical slot mapping
-------------------------------------------------------------------------------*/
typedef struct
{
  uint8                        total_logical_slots;
  slot_mapping_type            slot_mapping[UIM_MAX_SUBSCRIPTIONS_SUPPORTED];
}slot_mapping_info_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      SLOT_MAPPING_TYPE

   DESCRIPTION:
     Structure to store physical and logical slot mapping
-------------------------------------------------------------------------------*/
typedef struct 
{
  boolean                                      is_initialised;
  /* Critical section for updating the slot mapping table */
  rex_crit_sect_type                           critical_section;
  uim_physical_slots_info_type                 physical_slots_info;
  slot_mapping_preference_table_efs_data_type  slot_preferences_table;
}slot_mapping_global_type;

/* Global to store current slot mapping */
static slot_mapping_info_type       current_slot_mapping;

/* Global to store entire slot mapping parameters */
static slot_mapping_global_type     slot_mapping_global_var = {FALSE};

/* Global to store physical slots info from UIM drivers call back */
static uim_physical_slots_info_type physical_slots_info;

/*===========================================================================

                             FUNCTION DEFINATIONS 

===========================================================================*/

/*===========================================================================

FUNCTION SLOT_MAPPING_CONVERT_SLOT_ID_TO_INDEX

DESCRIPTION
  Converts the passed slot type enum to a slot array index.

DEPENDENCIES
  None

RETURN VALUE
  Boolean to indicate success or failure

SIDE EFFECTS
  None

===========================================================================*/
static boolean slot_mapping_convert_slot_id_to_index
(
  uim_slot_type               slot_type,
  uint8                     * slot_index_ptr
)
{
  if (slot_index_ptr == NULL)
  {
    return FALSE;
  }

  switch (slot_type)
  {
    case UIM_SLOT_1:
      *slot_index_ptr = 0;
      break;
    case UIM_SLOT_2:
      *slot_index_ptr = 1;
      break;
    case UIM_SLOT_3:
      *slot_index_ptr = 2;
      break;
    default:
      return FALSE;
  }
  return TRUE;
} /* slot_mapping_convert_slot_id_to_index */


/*===========================================================================

FUNCTION SLOT_MAPPING_CONVERT_INDEX_TO_SLOT_ID

DESCRIPTION
  Converts the passed slot array index to slot type enum.

DEPENDENCIES
  None

RETURN VALUE
  uim_slot_type that returns the slot ID

SIDE EFFECTS
  None

===========================================================================*/
static uim_slot_type  slot_mapping_convert_index_to_slot_id
(
  uint8                        slot_index
)
{
  uim_slot_type     slot_id = UIM_SLOT_NONE;
  
  switch (slot_index)
  {
    case 0:
      slot_id = UIM_SLOT_1;
      break;
    case 1:
      slot_id = UIM_SLOT_2;
      break;
    case 2:
      slot_id = UIM_SLOT_3;
      break;
    default:
      return UIM_SLOT_NONE;
  }
  
  return slot_id;
} /* slot_mapping_convert_index_to_slot_id */


/*===========================================================================
FUNCTION SLOT_MAPPING_READ_SLOT_PREFERENCE_TABLE_FROM_EFS()

DESCRIPTION
  Read the EFS path and get the slot preference mapping table

PARAMETERS
  None
  
RETURN VALUE
  SUCCESS/ERROR

DEPENDENCIES
  Slot mapping initialisation should be done

SIDE EFFECTS
  None

===========================================================================*/
static uim_return_type slot_mapping_read_slot_preference_table_from_efs()
{
  uim_common_efs_status_enum_type         efs_read_state   = UIM_COMMON_EFS_SUCCESS;
  
  efs_read_state = uim_common_efs_read(UIM_COMMON_EFS_SLOT_MAPPING_PREFERENCES_TABLE,
                                       UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                       UIM_COMMON_EFS_DEVICE,
                                       (uint8 *)&(slot_mapping_global_var.slot_preferences_table),
                                       sizeof(slot_mapping_preference_table_efs_data_type));

  if(efs_read_state != UIM_COMMON_EFS_SUCCESS)
  {
    return UIM_GENERIC_ERROR;
  }

  return UIM_SUCCESS;
} /* slot_mapping_read_slot_preference_table_from_efs */


/*===========================================================================
FUNCTION SLOT_MAPPING_NOTIFY_SLOTS_STATUS_CB()

DESCRIPTION
  A callback called by uimdrv when an association of a logical slot to
  a physical slot changes or whenever there is a change to the card state
  in any physical slot.

PARAMETERS
  uint8                   : number of physical slots
  uim_phy_slot_status_type: status of the physical slots

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
static void slot_mapping_notify_slots_status_cb
(
  uint8                           num_physical_slots,
  const uim_phy_slot_status_type *physical_slot_status_ptr
)
{
  boolean                             is_auto_mapping_needed = FALSE;
  uim_common_efs_status_enum_type     efs_read_state         = UIM_COMMON_EFS_SUCCESS;
  
  if (num_physical_slots == 0 ||
      physical_slot_status_ptr == NULL)
  {
    return;
  }

  /* The EFS needs to be read every time before processing the physical slot status
     If QMI_UIM_SWITCH_SLOT happens, then the EFS is overwritten with FALSE */
  efs_read_state = uim_common_efs_read(UIM_COMMON_EFS_AUTOMATIC_SLOT_MAPPING_NEEDED,
                                       UIM_COMMON_EFS_ITEM_FILE_TYPE,
                                       UIM_COMMON_EFS_DEVICE,
                                       (uint8 *)&(is_auto_mapping_needed),
                                       sizeof(is_auto_mapping_needed));

  if((efs_read_state == UIM_COMMON_EFS_SUCCESS) && is_auto_mapping_needed)
  {
    rex_enter_crit_sect(&slot_mapping_global_var.critical_section);

    if(physical_slots_info.slot_status_ptr == NULL)
    {
      physical_slots_info.slot_status_ptr = 
            (uim_phy_slot_status_type*)modem_mem_alloc(
                                       slot_mapping_global_var.physical_slots_info.num_slots * sizeof(uim_phy_slot_status_type),
                                       MODEM_MEM_CLIENT_UIM);
    }

    if(physical_slots_info.slot_status_ptr != NULL)
    {
      memset(physical_slots_info.slot_status_ptr,
             0,
             slot_mapping_global_var.physical_slots_info.num_slots * sizeof(uim_phy_slot_status_type));

      physical_slots_info.num_slots = num_physical_slots;
      (void)memscpy((void**)physical_slots_info.slot_status_ptr,
                    sizeof(uim_phy_slot_status_type) * physical_slots_info.num_slots,
                    (void*)physical_slot_status_ptr,
                    sizeof(uim_phy_slot_status_type) * num_physical_slots);
    }

    rex_leave_crit_sect(&slot_mapping_global_var.critical_section);
    /* Set signal so that physical slot status received from drivers can be processed
       in slot mapping module context */
    (void)mmgsdi_set_slot_mapping_sig();
  }
} /* slot_mapping_notify_slots_status_cb() */


/*===========================================================================
FUNCTION UIM_SLOT_MAPPING_INIT()

DESCRIPTION
  Slot mapping initialisation

PARAMETERS
  None
  
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
void uim_slot_mapping_init
(
  void
)
{
  if( (slot_mapping_global_var.is_initialised) || 
      (UIM_SUCCESS != slot_mapping_read_slot_preference_table_from_efs()) )
  {
    return;
  }
  
  slot_mapping_global_var.is_initialised = TRUE;

  /* Critical section used by Subscription manager */
  rex_init_crit_sect(&slot_mapping_global_var.critical_section);

  if(UIM_PASS != uim_sm_card_slot_status_evt_reg_sync(slot_mapping_notify_slots_status_cb))
  {
    return;
  }

  /* Get number of physical slots from drivers */
  slot_mapping_global_var.physical_slots_info.num_slots = uim_sm_get_num_physical_slots_sync();

  if( (slot_mapping_global_var.physical_slots_info.num_slots == 0) ||
      (slot_mapping_global_var.physical_slots_info.num_slots > UIM_MAX_NUMBER_INSTANCES) )
  {
    return;
  }

  /* Allocate the physical slot status data on the heap */
  slot_mapping_global_var.physical_slots_info.slot_status_ptr =
    (uim_phy_slot_status_type*)modem_mem_alloc(
                                slot_mapping_global_var.physical_slots_info.num_slots * sizeof(uim_phy_slot_status_type),
                                MODEM_MEM_CLIENT_UIM);

  if(slot_mapping_global_var.physical_slots_info.slot_status_ptr != NULL)
  {
    memset(slot_mapping_global_var.physical_slots_info.slot_status_ptr,
           0,
           slot_mapping_global_var.physical_slots_info.num_slots * sizeof(uim_phy_slot_status_type));
  }

  /* Get physical slots status from uimdrv into our global */
  (void) uim_sm_get_logical_slot_status_sync(&slot_mapping_global_var.physical_slots_info.num_slots,
                                              slot_mapping_global_var.physical_slots_info.slot_status_ptr);
} /* slot_mapping_init */


/*===========================================================================
FUNCTION SLOT_MAPPING_IS_LOGICAL_SLOT_ACTIVE()

DESCRIPTION
  This function returns if the logical slot has any valid physical slot associated with it.

PARAMETERS
  logical_slot_id
  
RETURN VALUE
  TRUE/FALSE

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean slot_mapping_is_logical_slot_active
(
  uim_slot_type                   logical_slot_id
)
{
  uint8 phy_slot_idx = 0;

  if (slot_mapping_global_var.physical_slots_info.slot_status_ptr == NULL)
  {
    return FALSE;
  }

  for(phy_slot_idx = 0; phy_slot_idx < slot_mapping_global_var.physical_slots_info.num_slots; phy_slot_idx++)
  {
    if( slot_mapping_global_var.physical_slots_info.slot_status_ptr[phy_slot_idx].logical_slot == logical_slot_id &&
        slot_mapping_global_var.physical_slots_info.slot_status_ptr[phy_slot_idx].activity_status == UIM_SLOT_STATE_ACTIVE )
    {
      return TRUE;
    }
  }
  return FALSE;
} /* slot_mapping_is_logical_slot_active */


/*===========================================================================
FUNCTION SLOT_MAPPING_IS_UICC_AVAILABLE_IN_PHYSICAL_SLOT

DESCRIPTION
  This function returns if the physical slot has any valid UICC in it.

PARAMETERS
  physical_slot_id
  
RETURN VALUE
  TRUE  if UICC is available in the physical slot
  FALSE if there is no valid UICC in the physical slot

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean slot_mapping_is_uicc_available_in_physical_slot
(
  uim_slot_type                   physical_slot_id
)
{
  uint8  index = 0;
  uint8  empty_iccid[UIM_ICCID_SIZE] = {0};

  if(slot_mapping_convert_slot_id_to_index(physical_slot_id, &index) && 
     index < slot_mapping_global_var.physical_slots_info.num_slots)
  {
    if( (slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid_len > 0 && 
         slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid_len <= UIM_ICCID_SIZE) &&
        ( memcmp(slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid,
                 empty_iccid,
                 slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid_len) != 0) )
    {
      return TRUE;
    }
  }
  return FALSE;
} /* slot_mapping_is_uicc_available_in_physical_slot */


/*===========================================================================
FUNCTION SLOT_MAPPING_IS_PHYSICAL_SLOT_POSSIBLE_FOR_LOGICAL_SLOT

DESCRIPTION
  This function parses through the slot preference table and checks if the input
  physical slot is listed against the logical slot or not.

PARAMETERS
  Logical  Slot ID
  Physical Slot ID
  
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean slot_mapping_is_physical_slot_possible_for_logical_slot
(
  uim_slot_type   logical_slot_id,
  uim_slot_type   physical_slot_id
)
{
  uint8 logical_slot_idx  = 0;
  uint8 physical_slot_idx = 0;

  if(slot_mapping_convert_slot_id_to_index(logical_slot_id, &logical_slot_idx) &&
     logical_slot_idx < UIM_MAX_SUBSCRIPTIONS_SUPPORTED)
  {
    for(physical_slot_idx = 0; physical_slot_idx < UIM_MAX_INSTANCES ; physical_slot_idx++)
    {
      if(slot_mapping_global_var.slot_preferences_table.slot_mapping[logical_slot_idx][physical_slot_idx] == physical_slot_id )
      {
        return TRUE;
      }
    }
  }
  return FALSE;
} /* slot_mapping_is_physical_slot_possible_for_logical_slot */


/*===========================================================================
FUNCTION SLOT_MAPPING_UPDATE_CURRENT_LOGICAL_TO_PHYSICAL_SLOT_MAPPING

DESCRIPTION
  This function updates the current logical to physical slot mapping in globals.

PARAMETERS
  None
  
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
static void slot_mapping_update_current_logical_to_physical_slot_mapping
(
  void
)
{
  uint8 phy_slot_idx = 0;

  current_slot_mapping.total_logical_slots = slot_mapping_global_var.slot_preferences_table.total_logical_slots;

  for(phy_slot_idx = 0; phy_slot_idx < slot_mapping_global_var.physical_slots_info.num_slots &&
      slot_mapping_global_var.physical_slots_info.slot_status_ptr != NULL; phy_slot_idx++)
  {
    if( slot_mapping_global_var.physical_slots_info.slot_status_ptr[phy_slot_idx].logical_slot != UIM_SLOT_NONE &&
        slot_mapping_global_var.physical_slots_info.slot_status_ptr[phy_slot_idx].activity_status == UIM_SLOT_STATE_ACTIVE )
    {
      current_slot_mapping.slot_mapping[phy_slot_idx].logical_slot_id  =
        slot_mapping_global_var.physical_slots_info.slot_status_ptr[phy_slot_idx].logical_slot;
      current_slot_mapping.slot_mapping[phy_slot_idx].physical_slot_id =
        slot_mapping_convert_index_to_slot_id(phy_slot_idx);
    }
  }
} /* slot_mapping_update_current_logical_to_physical_slot_mapping */


/*===========================================================================
FUNCTION SLOT_MAPPING_MAP_PHYSICAL_SLOT_TO_LOGICAL_SLOT

DESCRIPTION
  This function calls UIM drv API to mapp physical slot to any logical slot

PARAMETERS
  Logical  Slot ID
  Physical Slot ID
  
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
static void slot_mapping_map_physical_slot_to_logical_slot
(
  uim_slot_type logical_slot_id, uim_slot_type physical_slot_id
)
{
  uint8 i                  = 0;
  uint8 retry_count        = 0;
  uint8 logical_slot_index = 0;
  uim_rpt_status    status = UIM_FAIL;
  rex_sigs_type     sigs   = SLOT_MAPPING_RETRY_SIG | MMGSDI_GSTK_IMEI_SENT_SIG;
  rex_timer_type    slot_mapping_timer;
  rex_sigs_type     sigs_rcvd  = 0x00;

  uim_logical_slot_config_type  logical_phy_mapping;
  
  memset(&logical_phy_mapping, 0, sizeof(logical_phy_mapping));

  slot_mapping_update_current_logical_to_physical_slot_mapping();

  if(slot_mapping_convert_slot_id_to_index(logical_slot_id, &logical_slot_index))
  {
    for (i = 0; i < current_slot_mapping.total_logical_slots; i++)
    {
      if( logical_slot_index != i)
      {
        logical_phy_mapping.slot[i] = current_slot_mapping.slot_mapping[i].physical_slot_id;
      }
      else
      {
        logical_phy_mapping.slot[i] = physical_slot_id;
        UIM_MSG_HIGH_2("Mapped logical_slot 0x%x to physical_slot 0x%x", logical_slot_id, physical_slot_id);
      }
    }

    logical_phy_mapping.num_slots = current_slot_mapping.total_logical_slots;

    /* Execute UIMDRV function to switch the slot */
    status = uim_switch_slot_mapping(&logical_phy_mapping, UIMDRV_SUB_MGR_CLIENT_SLOT_MAPPING_MODULE );

    while( (status == UIM_FAIL) && (retry_count < 3) )
    {
      rex_def_timer(&slot_mapping_timer, rex_self(), SLOT_MAPPING_RETRY_SIG);
      (void)rex_set_timer(&slot_mapping_timer, SLOT_MAPPING_IN_PROGRESS_WAIT_TIME_VALUE);
      sigs_rcvd = mmgsdi_wait(sigs);
      UIM_MSG_HIGH_1("OUT OF WAIT FOR UIM: SIGS 0x%x, retrying switch command", sigs_rcvd);
      /* Undefine the timer before exiting */
      (void)rex_undef_timer(&slot_mapping_timer);
      status = uim_switch_slot_mapping(&logical_phy_mapping, UIMDRV_SUB_MGR_CLIENT_SLOT_MAPPING_MODULE );
      retry_count++;
    }
  }
} /* slot_mapping_map_physical_slot_to_logical_slot */


/*===========================================================================
FUNCTION SLOT_MAPPING_PRIORITY_OF_PHYSICAL_SLOT_FOR_LOGICAL_SLOT

DESCRIPTION
  Read the EFS path and get the slot preference mapping table

PARAMETERS
  Physical slot ID
  Physical slot ID
  
RETURN VALUE
  None

DEPENDENCIES
  Slot mapping initialisation should be done

SIDE EFFECTS
  None

===========================================================================*/
static uint8 slot_mapping_priority_of_physical_slot_for_logical_slot
(
  uim_slot_type   logical_slot_id,
  uim_slot_type   physical_slot_id
)
{
  uint8 phy_slot_idx     = 0;
  uint8 logical_slot_idx = 0;

  if(slot_mapping_convert_slot_id_to_index(logical_slot_id, &logical_slot_idx) &&
     logical_slot_idx < UIM_MAX_SUBSCRIPTIONS_SUPPORTED)
  {
    for(phy_slot_idx = 0; phy_slot_idx < UIM_MAX_INSTANCES ; phy_slot_idx++)
    {
      if(slot_mapping_global_var.slot_preferences_table.slot_mapping[logical_slot_idx][phy_slot_idx] == physical_slot_id )
      {
        break;
      }
    }
  }
  
  return phy_slot_idx;  
} /* slot_mapping_priority_of_physical_slot_for_logical_slot */


/*===========================================================================
FUNCTION SLOT_MAPPING_ALGORITHM_WHEN_CARD_MOVED_TO_ERROR

DESCRIPTION
  Slot mapping algorithm triggered when any physical slot moves to error state

PARAMETERS
  Physical slot ID
  
RETURN VALUE
  None

DEPENDENCIES
  Slot mapping initialisation should be done

SIDE EFFECTS
  None

===========================================================================*/
static void slot_mapping_algorithm_when_card_moved_to_error
(
  uim_slot_type physical_slot_id
)
{
  uint8          physical_slot_index  = 0;
  uim_slot_type  other_physical_slot  = UIM_SLOT_NONE;
  uim_slot_type  logical_slot         = UIM_SLOT_NONE;

  UIM_MSG_HIGH_1("Entered card error algorithm for Physical Slot 0x%x", physical_slot_id);

  if (slot_mapping_convert_slot_id_to_index(physical_slot_id, &physical_slot_index) == FALSE ||
      physical_slot_index >= slot_mapping_global_var.physical_slots_info.num_slots)
  {
    return;
  }
  if (slot_mapping_global_var.physical_slots_info.slot_status_ptr[physical_slot_index].logical_slot == UIM_SLOT_NONE)
  {
    return;
  }

  logical_slot = slot_mapping_global_var.physical_slots_info.slot_status_ptr[physical_slot_index].logical_slot;

  /* For all the physical slots, check if any of the available physical slots 
     can be mapped to this logical slot that got freed */
  for (other_physical_slot = UIM_SLOT_1; other_physical_slot < UIM_SLOT_AUTOMATIC; other_physical_slot++)
  {
    if(other_physical_slot == physical_slot_id)
    {
      continue;
    }

    if (slot_mapping_convert_slot_id_to_index(other_physical_slot, &physical_slot_index) == FALSE ||
        physical_slot_index >= slot_mapping_global_var.physical_slots_info.num_slots)
    {
      continue;
    }

    if ( (slot_mapping_global_var.physical_slots_info.slot_status_ptr[physical_slot_index].activity_status == UIM_SLOT_STATE_INACTIVE)  &&
         (TRUE == slot_mapping_is_uicc_available_in_physical_slot(other_physical_slot)) )
    {
      UIM_MSG_HIGH_2("Since the inactive Physical Slot 0x%x has a valid UICC in it, map it with this logical slot 0x%x",
                      other_physical_slot, logical_slot);
      slot_mapping_map_physical_slot_to_logical_slot(logical_slot, other_physical_slot);
      break;
    }
  }
} /* slot_mapping_algorithm_when_card_moved_to_error */


/*===========================================================================
FUNCTION SLOT_MAPPING_ALGORITHM

DESCRIPTION
  Slot mapping algorithm triggered when any physical slot status changes

PARAMETERS
  Physical slot ID
  
RETURN VALUE
  None

DEPENDENCIES
  Slot mapping initialisation should be done

SIDE EFFECTS
  None

===========================================================================*/
static void slot_mapping_algorithm
(
  uim_slot_type    physical_slot_id
)
{
  uint8 i                            = 0;
  uint8 physical_slot_index          = 0;
  boolean mapping_done               = FALSE;
  uim_slot_type logical_slot_id      = UIM_SLOT_NONE;  
  uim_slot_type other_logical_slot   = UIM_SLOT_NONE;
  uim_slot_type other_pysical_slot   = UIM_SLOT_NONE;
  uim_slot_type mapped_physical_slot = UIM_SLOT_NONE;

  UIM_MSG_HIGH_1("Entered the slot_mapping_algorithm for physical slot 0x%x", physical_slot_id);
  
  /* If the reason for Algorithm trigger is due to card removed or card moving to error, run the below Algo */
  if(FALSE == slot_mapping_is_uicc_available_in_physical_slot(physical_slot_id))
  {
    slot_mapping_algorithm_when_card_moved_to_error(physical_slot_id);
    return;
  }

  if(!slot_mapping_convert_slot_id_to_index(physical_slot_id, &physical_slot_index))
  {
    return;
  }

  for ( logical_slot_id = UIM_SLOT_1; logical_slot_id <= slot_mapping_global_var.slot_preferences_table.total_logical_slots && !mapping_done; logical_slot_id++)
  {
     /* if current physical_slot is already associated with the logical slot, then exit the Algo */
    if(slot_mapping_global_var.physical_slots_info.slot_status_ptr[physical_slot_index].logical_slot == logical_slot_id )
    {
      UIM_MSG_HIGH_2("This physical slot 0x%x is already mapped to the logical slot 0x%x ", physical_slot_id, logical_slot_id);
      break;
    }

     /* if the current physical_slot is not possible for logical slot then continue */
    if( FALSE == slot_mapping_is_physical_slot_possible_for_logical_slot(logical_slot_id, physical_slot_id))
    {
      continue;
    }

    UIM_MSG_HIGH_2("The physical slot 0x%x is listed in the table for logical slot 0x%x", physical_slot_id, logical_slot_id);

    for( physical_slot_index = 0; physical_slot_index < UIM_MAX_INSTANCES; physical_slot_index++ )
    {
      other_pysical_slot = slot_mapping_convert_index_to_slot_id(physical_slot_index);
      if( ( logical_slot_id == slot_mapping_global_var.physical_slots_info.slot_status_ptr[physical_slot_index].logical_slot) && 
          ( TRUE == slot_mapping_is_uicc_available_in_physical_slot(other_pysical_slot) ))
      {
        mapped_physical_slot = other_pysical_slot;
        UIM_MSG_HIGH_2("The current logical slot 0x%x is already mapped with physical slot 0x%x", logical_slot_id, mapped_physical_slot);
        break;
      }
    }

    if( mapped_physical_slot == UIM_SLOT_NONE)
    {
      UIM_MSG_HIGH_2("Since the logical slot 0x%x do not have any physical slot associated, we are mapping this logical slot to physical slot 0x%x", 
                         logical_slot_id, physical_slot_id);
      slot_mapping_map_physical_slot_to_logical_slot(logical_slot_id, physical_slot_id);
      break;
    }

     /* if the logical slot is already associated with any of the physical card of more priority than current physical_slot, then continue. */
    if(  slot_mapping_priority_of_physical_slot_for_logical_slot(logical_slot_id, mapped_physical_slot) <
         slot_mapping_priority_of_physical_slot_for_logical_slot(logical_slot_id, physical_slot_id) )
    {
      UIM_MSG_HIGH_2("The current physical slot 0x%x priority is less than already physical slot 0x%x", physical_slot_id, mapped_physical_slot);
      continue;
    }

    /* if logical slot is already associated with any of the physical card of less priority than current physical_slot then
       check for any other available logical slot before associating the physical slot to this logical slot */
    if(  slot_mapping_priority_of_physical_slot_for_logical_slot(logical_slot_id, mapped_physical_slot) >
         slot_mapping_priority_of_physical_slot_for_logical_slot(logical_slot_id, physical_slot_id) )
    {
      for(other_logical_slot = UIM_SLOT_1; 
          other_logical_slot < slot_mapping_global_var.physical_slots_info.num_slots; 
          other_logical_slot++)
      {
        if(other_logical_slot == logical_slot_id)
        {
          continue;
        }

        if( ( FALSE == slot_mapping_is_logical_slot_active(other_logical_slot)) && 
            ( slot_mapping_is_physical_slot_possible_for_logical_slot(other_logical_slot, physical_slot_id) ))
        {
          UIM_MSG_HIGH_2("Since the other logical slot 0x%x was free, we are mapping this physical slot 0x%x to it", 
                           other_logical_slot, physical_slot_id);
          slot_mapping_map_physical_slot_to_logical_slot(other_logical_slot, physical_slot_id);
          mapping_done = TRUE;
          break;
        }
      }

      if(!mapping_done)
      {
        UIM_MSG_HIGH_2("Since the current physical slot 0x%x priority is greater, we are mapping this to logical slot 0x%x", 
                         physical_slot_id, logical_slot_id);
        slot_mapping_map_physical_slot_to_logical_slot(logical_slot_id, physical_slot_id); 
        break;
      }
    }
  }
} /* slot_mapping_algorithm */


/*===========================================================================
FUNCTION SLOT_MAPPING_STORE_PHYSICAL_SLOTS_STATUS()

DESCRIPTION
  Copies the physical slots status from the passed-in parameter to the
  slot mapping global for physical slots status.

PARAMETERS
  uint8                   : number of physical slots supported
  uim_phy_slot_status_type: status of the physical slots
  uint8                   : Physical slots whose status got changed

RETURN VALUE
  TRUE                    : Global updated with latest physical slots info
  FALSE                   : Otherwise

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean slot_mapping_store_physical_slots_status
(
  uint8                           num_physical_slots,
  const uim_phy_slot_status_type *physical_slot_status_ptr,
  uint8                          *physical_slots_updated_mask_ptr
)
{
  uint8         index           = 0;

  ASSERT(physical_slots_updated_mask_ptr != NULL);

  /* If number of slots received by calling get_num_physical_slots
     during power up is not same as number of physical slots mentioned in
     the uimdrv callback here, there's a serious problem */
  if(slot_mapping_global_var.physical_slots_info.slot_status_ptr == NULL ||
     slot_mapping_global_var.physical_slots_info.num_slots == 0 ||
     physical_slot_status_ptr == NULL ||
     num_physical_slots != slot_mapping_global_var.physical_slots_info.num_slots)
  {
    return FALSE;
  }

  /* Overwrite the slot ampping global ptr for physical slots status info with
     latest info */
  for(index = 0; index < slot_mapping_global_var.physical_slots_info.num_slots && index < UIM_MAX_NUMBER_INSTANCES; index++)
  {
    /* Do not update the card hotswap status if it has not changed since last
       time we updated it (to CARD_INSERTED or CARD_REMOVED) */
    if(physical_slot_status_ptr[index].card_presence_status != UIM_CARD_UNCHANGED &&
       slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].card_presence_status !=
         physical_slot_status_ptr[index].card_presence_status)
    {
      slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].card_presence_status =
        physical_slot_status_ptr[index].card_presence_status;
      *physical_slots_updated_mask_ptr = ( *physical_slots_updated_mask_ptr | (1 << index) ) ;
    }
    if(slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].activity_status !=
         physical_slot_status_ptr[index].activity_status)
    {
      slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].activity_status =
        physical_slot_status_ptr[index].activity_status;
      *physical_slots_updated_mask_ptr = ( *physical_slots_updated_mask_ptr | (1 << index) ) ;
    }
    if(slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].logical_slot !=
         physical_slot_status_ptr[index].logical_slot)
    {
      slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].logical_slot =
        physical_slot_status_ptr[index].logical_slot;
      *physical_slots_updated_mask_ptr = ( *physical_slots_updated_mask_ptr | (1 << index) ) ;
    }
    if(slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid_len !=
         physical_slot_status_ptr[index].iccid_len ||
       memcmp(slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid,
              physical_slot_status_ptr[index].iccid,
              physical_slot_status_ptr[index].iccid_len) != 0)
    {
      slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid_len =
          physical_slot_status_ptr[index].iccid_len;
      (void)memscpy(slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid,
                    sizeof(slot_mapping_global_var.physical_slots_info.slot_status_ptr[index].iccid),
                    physical_slot_status_ptr[index].iccid,
                    physical_slot_status_ptr[index].iccid_len);
      *physical_slots_updated_mask_ptr = ( *physical_slots_updated_mask_ptr | (1 << index) ) ;
    }
  }

  return  *physical_slots_updated_mask_ptr != 0 ? TRUE : FALSE;
} /* slot_mapping_store_physical_slots_status() */


/*===========================================================================
FUNCTION SLOT_MAPPING_SIG_HANDLER

DESCRIPTION
  QMI UIM signal handler for MMGSDI task signals

PARAMETERS
  None.

  Returns TRUE if signal should be cleared from the set signal mask,
  FALSE if further signal processing is needed and hence signal
  should not be cleared.
===========================================================================*/
boolean uim_slot_mapping_sig_handler
(
  void
)
{
  boolean       is_info_updated             = FALSE;
  uint8         physical_slots_updated_mask = 0;
  uint8         slot_index                  = 0;
  uim_slot_type physical_slot               = UIM_SLOT_NONE;

  if (!slot_mapping_global_var.is_initialised)
  {
    return TRUE;
  }

  rex_enter_crit_sect(&slot_mapping_global_var.critical_section);

  is_info_updated = slot_mapping_store_physical_slots_status(
                        physical_slots_info.num_slots,
                        physical_slots_info.slot_status_ptr,
                        &physical_slots_updated_mask);

  modem_mem_free(physical_slots_info.slot_status_ptr, MODEM_MEM_CLIENT_UIM);
  physical_slots_info.slot_status_ptr = NULL;

  rex_leave_crit_sect(&slot_mapping_global_var.critical_section);

  if (is_info_updated == FALSE)
  {
    return TRUE;
  }

  UIM_MSG_HIGH_2("The information got updated on slot mask 0x%x, total phy slots 0x%x",
                   physical_slots_updated_mask,
                   slot_mapping_global_var.physical_slots_info.num_slots);

  /* Update the globals to reflect the current slot mapping information */
  slot_mapping_update_current_logical_to_physical_slot_mapping();    

  /* Trigger the slot mapping alogarithm for all the physical slots whose status is updated */
  for( slot_index = 0; (slot_index < slot_mapping_global_var.physical_slots_info.num_slots); slot_index++ )
  {
    if(physical_slots_updated_mask & (1 << slot_index))
    {
      physical_slot = slot_mapping_convert_index_to_slot_id(slot_index);
      slot_mapping_algorithm(physical_slot);
    }
  }
 
  return TRUE;
} /* uim_slot_mapping_sig_handler */
