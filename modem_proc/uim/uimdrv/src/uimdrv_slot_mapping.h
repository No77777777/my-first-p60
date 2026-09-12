#ifndef UIMDRV_SLOT_MAPPING_H
#define UIMDRV_SLOT_MAPPING_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  UIM_SLOT_MAPPING HEADER

DESCRIPTION
  This contains all the declarations of the slot mapping related APIs and globals.

  Copyright (c) 2016 - 2017 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/uimdrv_slot_mapping.h#1 $
$DateTime: 2019/10/30 06:56:05 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/05/17   gm       Update only active slots in uim_get_hw_slot_info
05/10/17   kv       Enable subs manager and sam card mode
01/19/17   ks       UIM automatic slot mapping support
04/21/16   sam      Modification of slot mapping state enum names
04/20/16   sam      Support for enhanced subscription manager
03/28/16   sam      Initial Draft

===========================================================================*/

/* Max number of sub manager clients supported */
#define UIMDRV_SUB_MANAGER_CLIENT_MAX 3

/*------------------------------------------------------------------------------
STRUCT:
  UIM_SLOT_MAPPING_TABLE_TYPE
DESCRIPTION:
  structure to store one entry of slot mapping table
------------------------------------------------------------------------------*/
typedef struct {
  uim_slot_type  logical_id;                     /*logical ID of the slot*/
  uim_slot_type  phy_id;                         /*physical ID of the slot*/
  boolean        logical_slot_powered_down;      /*boolean to check if currently this logical ID 
                                                   is powered down or not*/
}uim_slot_mapping_table_type;


/*------------------------------------------------------------------------------
STRUCT:
  UIM_PHY_SLOT_READY_TO_SWITCH_VOTE_TYPE
DESCRIPTION:
  structure to store one entry of physical slot vote
------------------------------------------------------------------------------*/
typedef struct {
  boolean    required;               /*is vot required for this physical slot*/
  boolean    value;                  /*value of the vote of this physical slot*/
}uim_phy_slot_ready_to_switch_vote_type;
	
	
/* ---------------------------------------------------------------
ENUM:      UIM_SLOT_MAPPING_STATE_TYPE

DESCRIPTION:
 Enum lists the states of the slot mapping
   ----------------------------------------------------------------*/
typedef enum
{
  UIM_SLOTMAP_ST_NOT_INTIALIZED,       /*slot mapping nto yet intialised*/
  UIM_SLOTMAP_ST_IN_PROGRESS,          /*slot mappin is in progress*/
  UIM_SLOTMAP_ST_IDLE                  /*in idle state.ok to accept new slot mapping*/
}uim_slot_mapping_state;


/*------------------------------------------------------------------------------
STRUCT:
  UIM_SM_DATA_TYPE
DESCRIPTION:
  Global strurture to store all information required for
  Subscription manager configuration
------------------------------------------------------------------------------*/
typedef struct{
   boolean                   is_initialised;
   uint8                     max_active_slots;
   uint8                     active_slots_num;
   uim_phy_slot_status_type  phy_slot_info[UIM_MAX_NUMBER_INSTANCES];/*Data of all physical slots*/
   uim_evt_callback_type    *evt_cb_ptr[UIMDRV_SUB_MANAGER_CLIENT_MAX]; /* callback configured with UIM */
   rex_crit_sect_type        critical_section;
}uim_sm_data_type;


/*------------------------------------------------------------------------------
STRUCT:
  UIM_SLOT_MAPPING_GLOBAL_TYPE
DESCRIPTION:
  structure to store one entry of physical slot vote
------------------------------------------------------------------------------*/
typedef struct {
  /*Critical section for updating the slot mapping table*/
  rex_crit_sect_type                      critical_section;

  /*Current state of slot mapping*/
  uim_slot_mapping_state                  state;

/*No of slots enabled on device. slot mapping will be done 
  only for these number of slots*/
  byte	                                  no_of_slots_supported;

  /*Slot mapping table. the table entries are chronologicaly mainted based on LOGICAL ID*/
  uim_slot_mapping_table_type             logical_slot_info[UIM_MAX_NUMBER_INSTANCES]; 

  /*Voting global to check if the physical slot(uim task) is ready to switch*/
  uim_phy_slot_ready_to_switch_vote_type  phy_slot_ready_to_switch_vote[UIM_MAX_NUMBER_INSTANCES];  

  /*flag to check if the physical slot assignment is completed or not. Used to update state of slot mapping*/
  boolean                                 phy_slot_assigned[UIM_MAX_NUMBER_INSTANCES];

  /*global to sotre sub manager variables*/
  uim_sm_data_type                        sm_data;
}uim_slot_mapping_global_type;

/*slot mapping global*/
extern uim_slot_mapping_global_type uim_slot_mapping;

/*------------------------------------------------------------------------------
STRUCT:
  UIM_SLOT_MAPPING_CMD_SAVED_DATA_TYPE
DESCRIPTION:
  structure to store recieved commands callback and user data
------------------------------------------------------------------------------*/
typedef struct {
   void (*rpt_function)( uim_rpt_type * );
   uint32 userdata;
} uim_slot_mapping_cmd_saved_data_type;


/*------------------------------------------------------------------------------
STRUCT:
UIM_EXTENDED_SLOT_MAPPING_EFS_DATA_TYPE
DESCRIPTION:
  structure to store recieved commands callback and user data
------------------------------------------------------------------------------*/
typedef PACKED struct PACKED_POST {
  uint8 version;
  uint8 active_slots_num;
  uint8 logical_slot_config[UIM_MAX_NUMBER_INSTANCES];
  uint8 rfu[64];
} uim_extended_slot_mapping_efs_data_type;


/**
*  This function is used to intialize the slot mapping during device bootup
*
*  @param uim_ptr   Pointer to the current instance of the UIM global structure
*/
void uimdrv_slot_mapping_init(uim_instance_global_type *uim_ptr);


/**
*  This function converts the physical ID ot logical ID
*
*
*  @param slot_id   Physical ID 

*  @return uim_slot_type   Logical ID of that slot
*/
uim_slot_type uim_convert_physical_to_logical_slot_id(uim_slot_type slot_id);


/**
*  This function converts the logical ID to physical ID
*
*
*  @param slot_id   Logical ID 

*  @return uim_slot_type   physical ID of that slot
*/
uim_slot_type uim_convert_logical_to_physical_slot_id(uim_slot_type slot_id);


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
uim_slot_mapping_cmd_saved_data_type* uim_slot_mapping_translate_cmd(uim_cmd_type *cmd_ptr);


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
);


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
);


/**
*  this is used for freeing user data when callback is skipped
*
*  @param cmd_ptr   Pointer to the command
*/
void uim_slot_mapping_free_userdata
(
  uim_cmd_type *cmd_ptr
);


/**
* This API is to get the number of active slots.
*
*  @param  max_num_slot_supported   Max number of slot supported

*  @return uim_rpt_status           Number of active slots
*/
uint8 uim_get_num_active_slot
(
  uint8 max_num_slot_supported
);
#endif /*UIM_SLOT_MAPPING_H*/
