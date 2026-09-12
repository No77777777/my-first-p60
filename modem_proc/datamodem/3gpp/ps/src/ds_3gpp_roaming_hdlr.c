/*!
  @file
  ds_pdn_limit_hdlr.c

  @brief
  REQUIRED brief one-sentence description of this C module.

  @detail
  OPTIONAL detailed description of this C module.
  - DELETE this section if unused.

*/

/*===========================================================================

  Copyright (c) 2009-2019 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps/src/ds_3gpp_roaming_hdlr.c#3 $

when         who     what, where, why
--------     ---     --------------------------------------------------------
05/15/13     vs      Initial File
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"

#include "ds_3gpp_roaming_hdlr.h"
#include "dsumtspdpregint.h"
#include "list.h"
#include "modem_mem.h"
#include "ds_3gppi_utils.h"
#include "ds_3gpp_hdlr.h"
#include "ps_sys_conf.h"
#include "ds_3gpp_pdn_context.h"
#include "ds_3gpp_pdn_throttle_sm.h"
#include "ds_3gpp_plmn_hdlr.h"
#include "ds_3gpp_device_ev_hdlr.h"
#include "ds_3gpp_nv_manager.h"
#include "ds3gmmgsdiif.h"
#include "ds3gdsdif.h"
#include "dsutil.h"
#include "queue.h"

#ifdef FEATURE_DATA_LTE
  #ifdef FEATURE_DATA_WLAN_MAPCON
    #include "ds_3gpp_kamgr_ext.h"
  #endif /*FEATURE_DATA_WLAN_MAPCON*/
  #include "ds_eps_pdn_context_ex.h"
#endif /*FEATURE_DATA_LTE*/

#include "ds_3gpp_pdn_throttle_sm.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

/*===========================================================================

                   INTERNAL DEFINITIONS AND TYPES

===========================================================================*/
/*---------------------------------------------------------------------------- 
  Maximum value of PDN Disconnect Wait Time. If PDN Disconnection Wait Time is
  configured to be 0xFF, it is the client app's responsibility to deregister with
  the network (which would inturn trigger PDN Disconnect Request).
 -----------------------------------------------------------------------------*/
#define DS_3GPP_ROAMING_MAX_PDN_DISCON_WAIT_TIME 0xFF

/*---------------------------------------------------------------------------- 
  This structure defines an item in the PDN Disconnect Wait Timer List.
-----------------------------------------------------------------------------*/
typedef struct
{
  /* Pointer to the next item in list */
  list_link_type             link;
  /* Profile number for which the wait timer was running */
  uint8                      profile_num;
  /* PDN Disconnect Wait Timer */
  rex_timer_type            *timer_ptr;
  /* The subs id */
  sys_modem_as_id_e_type     subs_id;
}ds_3gpp_roaming_wait_timer_item_type;

/*---------------------------------------------------------------------------- 
  This structure defines all the information related to the Roaming Handler
  Module.
-----------------------------------------------------------------------------*/
typedef struct
{
  struct subs_index_s
  {
    boolean                             roaming_status;  /* Whether UE is in Roaming or not */
    ds_sys_roaming_type_enum_type       roaming_type;
  } subs_index[DS3GSUBSMGR_SUBS_ID_MAX]; 

  list_type    wait_timer_list; /* PDN Disconnect Wait Timer List */

} ds_3gpp_roaming_info_type;

typedef enum
{
  DS_3GPP_ROAMING_QMI_MIN   = 0,
  DS_3GPP_ROAMING_QMI_HPLMN = DS_3GPP_ROAMING_QMI_MIN,
  DS_3GPP_ROAMING_QMI_EXCP_HPLMN = 1,
  DS_3GPP_ROAMING_QMI_DOMESTIC_RPLMN = 2,
  DS_3GPP_ROAMING_QMI_INTERNATIONAL_RPLMN = 3,
  DS_3GPP_ROAMING_QMI_MAX = 4
}ds_3gpp_roaming_qmi_tag_e;

/*===========================================================================

                         LOCAL VARIABLES

===========================================================================*/

/*---------------------------------------------------------------------------- 
  This variable declares all the information related to the Roaming Handler
  Module.
-----------------------------------------------------------------------------*/
static ds_3gpp_roaming_info_type ds_3gpp_roaming_info = {{{NULL}}};

/*===========================================================================

                    INTERNAL FUNCTION PROTOTYPES

===========================================================================*/
  
/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_BLOCK_APN_PER_PROFILE

  DESCRIPTION
  This function blocks given APN corresponding to given profile.
  
  PARAMETERS  
  Subscription id
  Profile Information Ptr
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_block_apn_per_profile
(
  sys_modem_as_id_e_type     subs_id,
  ds_umts_pdp_profile_type   *profile_info_ptr
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_BLOCK_APNS

  DESCRIPTION
  This function blocks all APNs which are disallowed during Roaming.
  
  PARAMETERS  
  Subscription id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_block_apns
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_UNBLOCK_APN_PER_PROFILE

  DESCRIPTION
  This function unblocks APN corresponding to the given profile.
  
  PARAMETERS  
  Subscription id
  Profile Information Ptr
  Boolean Pointer set to True if the attach apn is unblocked
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_unblock_apn_per_profile
(
  sys_modem_as_id_e_type    subs_id,
  ds_umts_pdp_profile_type  *profile_info_ptr,
  boolean*                  is_attach_apn_ptr
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_UNBLOCK_APNS

  DESCRIPTION
  This function unblocks all APNs which were disallowed during Roaming.
  
  PARAMETERS  
  Subscription id
  Boolean Pointer set to True if the attach apn is unblocked
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_unblock_apns
(
  sys_modem_as_id_e_type subs_id,
  boolean*               is_attach_apn_ptr
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_ADVERTISE_THROT_INFO

  DESCRIPTION
  This function advertises Throttling Information to all clients and
  disables lte if needed
  
  PARAMETERS
  1. disable_lte_if_needed: Flag indicating whether LTE needs to be disabled
  2. Subscription id
  3. Boolean set to True if the attach apn is unblocked
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/ 
static void ds_3gpp_roaming_advertise_throt_info
(
  boolean                disable_lte_if_needed,
  sys_modem_as_id_e_type subs_id,
  boolean                is_attach_apn
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_ALLOCATE_AND_START_WAIT_TIMER

  DESCRIPTION
  This function tries to allocate and start the wait timer.
  
  PARAMETERS  
  Profile for which the wait timer is configured.
  subs_id: The subscription for which to start the timer
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  TRUE, if Wait Timer started or no need to Start Wait Timer
  FALSE, if Wait Timer Did not Start
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static boolean ds_3gpp_roaming_allocate_and_start_wait_timer
(
  ds_umts_pdp_profile_type   *profile_info_ptr,
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_WAIT_TIMER_EXP_CB

  DESCRIPTION
  This function callback is invoked when the PDN Disconnect wait timer
  expires.
  
  PARAMETERS
 
  Callback Data
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_wait_timer_exp_cb
(
  unsigned long cb_data
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_PROCESS_PLMN_LIST_CHG_CMD

  DESCRIPTION
  This function is used to process the change in PLMN List.
  
  PARAMETERS  
  Subscription Id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gpp_roaming_process_plmn_list_chg_cmd
(
  sys_modem_as_id_e_type   subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_GET_CURRENT_PLMN_LIST

  DESCRIPTION
  This function gets the current PLMN List. If current PLMN is same as RPLMN
  or part of EPLMN list, copy RPLMN and EPLMN list to current PLMN list.
  Otherwise, copy current PLMN to current PLMN list.
  
  PARAMETERS
  Subscription Id
  New PLMN Id
  Current PLMN List
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
static void ds_3gpp_roaming_get_current_plmn_list
(
  sys_modem_as_id_e_type    subs_id,
  sys_plmn_id_s_type        new_plmn_id,
  ds_sys_plmn_list_s_type  *curr_plmn_list_p
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_IS_PLMN_FOUND_IN_LIST

  DESCRIPTION
  This function checks whether the PLMN can be found in a given PLMN List.
  
  PARAMETERS
  PLMN Id
  PLMN List Ptr
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Boolean.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_roaming_is_plmn_found_in_list
(
  sys_plmn_id_s_type          plmn_id,
  list_type                  *list_ptr
);


/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_UNBLOCK_ROAMING_THROTTLING

DESCRIPTION
  This function unblocks roaming throttling for given APN or for all
  APNs blocked due to roaming.
  
PARAMETERS
  1. APN name to be throttled
  2. Subscription Id
  3. Flag to indicate whether only the given APN should be unblocked or
     all APNs blocked due to roaming should be unblocked.
  4. Boolean Pointer set to True if the attach apn is unblocked
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.
===========================================================================*/
static void ds_3gpp_roaming_unblock_throttling
(
  byte*                                apn,
  sys_modem_as_id_e_type               subs_id,
  boolean                              unblock_all_apns,
  boolean*                             is_attach_apn_ptr
);


/*===========================================================================


                               FUNCTIONS

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_3gpp_roaming_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for MEM Pool global variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/ 
QSH_MDUMP_FN_ATTR 
void ds_3gpp_roaming_qsh_mdump_collect()
{
  qsh_mdump_collect_high(&ds_3gpp_roaming_info, 
  	sizeof(ds_3gpp_roaming_info_type));
}
#endif /* FEATURE_QSH_MDUMP */


/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_HDLR_POWERUP_INIT

  DESCRIPTION
  This function performs Powerup Initialization of the Module. So far only to
  initialize the wait_timer list.
  
  PARAMETERS  
  None. 
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_roaming_hdlr_powerup_init
(
  void
)
{
  list_init(&ds_3gpp_roaming_info.wait_timer_list);
}/*ds_3gpp_roaming_hdlr_powerup_init*/

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_GET_STATUS

  DESCRIPTION
  This function tells whether the UE is Roaming or not.
  
  PARAMETERS  
  Subscription Id 
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  TRUE,  if UE is roaming
  FALSE, otherwise
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_roaming_get_status
(
  sys_modem_as_id_e_type subs_id
)
{
  boolean status = FALSE;
  /*--------------------------------------------------------------------*/

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return FALSE;
  }  

  ds3gpp_enter_global_crit_section();
  status = ds_3gpp_roaming_info.subs_index[subs_id].roaming_status;
  ds3gpp_leave_global_crit_section();

  return status;
} /* ds_3gpp_roaming_get_status */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_GET_TYPE

  DESCRIPTION
  This function tells the UE roaming type (domestic / international roaming)
  if the UE is roaming. If the UE is in home, the type would be unspecified.
  
  PARAMETERS  
  Subscription Id 
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Roaming type defined in ds_sys_roaming_type_enum_type
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
ds_sys_roaming_type_enum_type ds_3gpp_roaming_get_type
(
  sys_modem_as_id_e_type              subs_id
)
{
  ds_sys_roaming_type_enum_type  type = DS_SYS_ROAMING_UNSPECIFIED;
  /*--------------------------------------------------------------------*/

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return FALSE;
  }  

  ds3gpp_enter_global_crit_section();
  type = ds_3gpp_roaming_info.subs_index[subs_id].roaming_type;
  ds3gpp_leave_global_crit_section();

  return type;
} /* ds_3gpp_roaming_get_type */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_SET_STATUS

  DESCRIPTION
  This function sets the roaming statusand roaming type.
  
  PARAMETERS  
  Subscription Id
  Roaming Status
  Roaming Type
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_roaming_set_status
(
  sys_modem_as_id_e_type          subs_id,
  boolean                         status,
  ds_sys_roaming_type_enum_type   roaming_type
)
{
  boolean                               prev_status = FALSE;
  ds_sys_roaming_type_enum_type         prev_roaming_type = DS_SYS_ROAMING_UNSPECIFIED;
  ds3gdevmgr_device_settings_info_type *device_settings_info_p = NULL;
  ds3geventmgr_filter_type             *filter_info_p = NULL;
  ds3geventmgr_event_info_type         *event_info_p = NULL;
  boolean                               is_attach_apn = FALSE;
  boolean                               is_attach_apn_unblock_roaming = FALSE;
  boolean                               is_attach_apn_data_settings = FALSE;
  sys_sys_mode_e_type                   sys_mode = SYS_SYS_MODE_NONE;
  /*--------------------------------------------------------------------*/

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return;
  }  
  device_settings_info_p = (ds3gdevmgr_device_settings_info_type*)modem_mem_alloc(
                          sizeof(ds3gdevmgr_device_settings_info_type),
                          MODEM_MEM_CLIENT_DATA);
  if (device_settings_info_p == NULL)
  {
    goto func_exit;
  }
  memset(device_settings_info_p, 0, 
         sizeof(ds3gdevmgr_device_settings_info_type));

  ds3gpp_enter_global_crit_section();

  prev_status = ds_3gpp_roaming_info.subs_index[subs_id].roaming_status;
  prev_roaming_type = ds_3gpp_roaming_info.subs_index[subs_id].roaming_type;
  ds_3gpp_roaming_info.subs_index[subs_id].roaming_status = status;
  ds_3gpp_roaming_info.subs_index[subs_id].roaming_type = roaming_type;

  ds3gpp_leave_global_crit_section();

  if (status != prev_status)
  {
    DS_3GPP_MSG2_HIGH_EX(subs_id,
          "Roaming status. Old: %d, New: %d (1: Roaming, 0: Home)",
          prev_status,
          status);

    #ifdef FEATURE_DATA_LTE
      ds_eps_pdn_cntxt_ex_check_attach_pending(subs_id); 
    #endif /* FEATURE_DATA_LTE */

    if (status == TRUE)
    {
      ds_3gpp_roaming_block_apns(subs_id);
    }
    else
    {
      ds_3gpp_roaming_unblock_apns(subs_id,&is_attach_apn_unblock_roaming);
    }

    if (ds3gdevmgr_get_device_settings(device_settings_info_p, subs_id))
    {
      ds3gpp_device_ev_hdlr_set_ps_data_off_change_reason
            (subs_id, PS_SYS_PS_DATA_AVAIL_CHG_REASON_ROAM_STATUS);
      ds3gpp_device_ev_hdlr_process_data_settings(device_settings_info_p,
                                                  &is_attach_apn_data_settings);
    } 
   
    is_attach_apn = (is_attach_apn_unblock_roaming || is_attach_apn_data_settings);
    ds_3gpp_roaming_advertise_throt_info(status, subs_id,is_attach_apn);

#ifdef FEATURE_DATA_LTE
    DS_3GPP_MSG0_LOW("Roaming Status change, invoking KAMGR");
    ds_3gpp_kamgr_handle_roaming_status_chg(subs_id);
#endif /* FEATURE_DATA_LTE */
  }

  if (status != prev_status || roaming_type != prev_roaming_type)
  {
    /* Notify clients of roaming status change */
    filter_info_p = (ds3geventmgr_filter_type*)modem_mem_alloc(
                          sizeof(ds3geventmgr_filter_type),
                          MODEM_MEM_CLIENT_DATA);
    
    event_info_p = (ds3geventmgr_event_info_type*)modem_mem_alloc(
                          sizeof(ds3geventmgr_event_info_type),
                          MODEM_MEM_CLIENT_DATA);
    if (filter_info_p == NULL || event_info_p == NULL)
    {
      goto func_exit;
    }
    memset(filter_info_p,0,sizeof(ds3geventmgr_filter_type));
    memset(event_info_p,0,sizeof(ds3geventmgr_event_info_type));

    filter_info_p->ps_subs_id = (ps_sys_subscription_enum_type)
                                        ds3gsubsmgr_subs_id_cm_to_ds(subs_id);
    filter_info_p->tech = PS_SYS_TECH_ALL;

    event_info_p->roaming_status_info.is_roaming = status;
    event_info_p->roaming_status_info.roaming_type = roaming_type;
    event_info_p->roaming_status_info.sys_mode 
      = ds3g_get_current_network_mode_ex(subs_id);

    ds3geventmgr_set_event_info(DS3GEVENTMGR_ROAMING_STATUS_EV,
                                (void*)event_info_p,
                                filter_info_p);

   /*notify DSD of roaming status change for all 3GPP RATs*/
    for (sys_mode = SYS_SYS_MODE_NO_SRV; sys_mode < SYS_SYS_MODE_MAX; sys_mode++)
    {
      if (DS_3GPP_IS_3GPP_CURRENT_MODE(sys_mode)) 
      {
        ds3gdsdif_notify_dsd_roam_status_chg_event(sys_mode, subs_id, status, roaming_type); 
      }     
    }
  }

  ds_3gpp_roaming_advertise_apns_blocked_due_to_roaming(subs_id, FALSE);

  func_exit:
     if (device_settings_info_p != NULL)
     {
       modem_mem_free(device_settings_info_p, MODEM_MEM_CLIENT_DATA);
     }
     if (event_info_p != NULL)
     {
       modem_mem_free(event_info_p, MODEM_MEM_CLIENT_DATA);
     }
     if (filter_info_p != NULL)
     {
       modem_mem_free(filter_info_p, MODEM_MEM_CLIENT_DATA);
     }

     return;

} /* ds_3gpp_roaming_set_status */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_CONVERT_TO_PLMN_FORMAT

  DESCRIPTION
  This function converts MCC MNC to PLMN Format
  
  PARAMETERS  
  MCC MNC Info
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  PLMN Id
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
sys_plmn_id_s_type ds_3gpp_roaming_convert_to_plmn_format
(
  ds_3gpp_roaming_mcc_mnc_info_s info
)
{
  return ds_3gpp_config_mgr_convert_to_plmn_format(info);
}/*ds_3gpp_roaming_convert_to_plmn_format */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_IS_PLMN_MATCH

  DESCRIPTION  Compare two PLMN IDs and determines if they are equal.
               The PLMN IDs may contain wildcard MNC FFF.
  
  PARAMETERS   plmn_1                  The first PLMN ID.
               plmn_2                  The second PLMN ID.
    
  DEPENDENCIES None.
  
  RETURN VALUE TRUE - PLMN matches
               FALSE - PLMN does not match
   
  SIDE EFFECTS None.
  
===========================================================================*/
boolean ds_3gpp_roaming_is_plmn_match
(
  sys_plmn_id_s_type  plmn_1,
  sys_plmn_id_s_type  plmn_2
)
{
  uint32     plmn1_mcc_digit_1 = plmn_1.identity[0] & 0x0F;
  uint32     plmn1_mcc_digit_2 = plmn_1.identity[0] / 0x10;
  uint32     plmn1_mcc_digit_3 = plmn_1.identity[1] & 0x0F;
  uint32     plmn1_mnc_digit_1 = plmn_1.identity[2] & 0x0F;
  uint32     plmn1_mnc_digit_2 = plmn_1.identity[2] / 0x10;
  uint32     plmn1_mnc_digit_3 = plmn_1.identity[1] / 0x10;

  uint32     plmn2_mcc_digit_1 = plmn_2.identity[0] & 0x0F;
  uint32     plmn2_mcc_digit_2 = plmn_2.identity[0] / 0x10;
  uint32     plmn2_mcc_digit_3 = plmn_2.identity[1] & 0x0F;
  uint32     plmn2_mnc_digit_1 = plmn_2.identity[2] & 0x0F;
  uint32     plmn2_mnc_digit_2 = plmn_2.identity[2] / 0x10;
  uint32     plmn2_mnc_digit_3 = plmn_2.identity[1] / 0x10;
  boolean    ret_val = FALSE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  if ( (plmn1_mcc_digit_1 == plmn2_mcc_digit_1) &&
       (plmn1_mcc_digit_2 == plmn2_mcc_digit_2) &&
       (plmn1_mcc_digit_3 == plmn2_mcc_digit_3) )
  {
    /*---------------------------------------------------------------------- 
     If one PLMN id has MNC - EEE which means it is matching with any MNC.
    ---------------------------------------------------------------------*/ 
    if( ((plmn1_mnc_digit_1 == 0x0E) && (plmn1_mnc_digit_2 == 0x0E)
        &&(plmn1_mnc_digit_3 == 0x0E)) || ((plmn2_mnc_digit_1 == 0x0E) &&
        (plmn2_mnc_digit_2 == 0x0E) &&(plmn2_mnc_digit_3 == 0x0E)) )
    {
      ret_val = TRUE;
    }
    else if( (plmn1_mnc_digit_1 == plmn2_mnc_digit_1) &&
             (plmn1_mnc_digit_2 == plmn2_mnc_digit_2) &&
             (plmn1_mnc_digit_3 == plmn2_mnc_digit_3))
    {
      ret_val = TRUE;
    }
  }

  DATA_3GPP_MSG7(MSG_LEGACY_HIGH,
                 "PLMN1 %d, %d, %d, PLMN2 %d, %d, %d, match? %d",
                 plmn_1.identity[0], plmn_1.identity[1], plmn_1.identity[2],
                 plmn_2.identity[0], plmn_2.identity[1], plmn_2.identity[2],
                 ret_val);

  return ret_val;
}/*ds_3gpp_roaming_is_plmn_match*/

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_INFORM_NEW_PLMN_INFO

  DESCRIPTION
  This function is used to notify the Roaming Module of PLMN change.
  
  PARAMETERS  
  Subscription Id
  New PLMN Id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_roaming_inform_new_plmn_info
(
  sys_modem_as_id_e_type subs_id,
  sys_plmn_id_s_type     new_plmn_id
)
{
  list_type                      *list_ptr = NULL;
  boolean                         match_excp_found = FALSE;
  boolean                         roaming_status = TRUE;
  ds_sys_roaming_type_enum_type   roaming_type = DS_SYS_ROAMING_UNSPECIFIED;
  boolean                         check_ehplmn_list = FALSE;
  mmgsdi_session_id_type          session_id;
  ds_sys_plmn_list_s_type        *curr_plmn_list_p = NULL;
  uint8                           plmn_index;
  boolean                         read_sessid_success = TRUE;
  boolean                         intl_roam_list_configured = FALSE;
  boolean                         dom_roam_list_configured = FALSE;
  ds_3gppi_local_err_val_e_type   local_err = DS_3GPPI_LOCAL_ERR_VAL_NONE;
  /*--------------------------------------------------------------------*/

  DS_3GPP_MSG1_HIGH_EX(subs_id,
        "Processing new PLMN Info on subs id %d", subs_id );

  do 
  {
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_0;
      break;
  }  

  /*---------------------------------------------------------------------- 
    If neither of the following configures is configured,
    UE listens to CM roaming indication.
    1. New extended EFS file - /data/3gpp/home_plmn_list.xml
    2. NV72561.
    3. NV73590.
  --------------------------------------------------------------------*/
  check_ehplmn_list = ds_3gpp_nv_manager_get_check_ehplmn_list(subs_id);

#ifndef TEST_FRAMEWORK
  /*-------------------------------------------------------------------- 
    If EHPLMN list NV is enabled and SIM is not ready return from this
    point.
    Roaming/Home PLMN decision will be made based on NV configuration
  --------------------------------------------------------------------*/
 if((check_ehplmn_list) &&
     (ds3g_mmgsdi_get_session_id_from_sub_id(subs_id,&session_id) == FALSE))
  {
    DS_3GPP_MSG0_HIGH_EX(subs_id, "Subscription is not ready and check_ehplmn is TRUE, "
                      "continue to check home plmn list"); 
    read_sessid_success = FALSE;
  }
#endif

  if ((ds_3gpp_config_mgr_get_home_plmn_list_size(subs_id) == 0) &&
      (check_ehplmn_list == FALSE || read_sessid_success == FALSE) &&
      (ds_3gpp_config_mgr_get_domestic_roam_plmn_list_size(subs_id)==0))
  {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_1;
      break;
  }
  curr_plmn_list_p = (ds_sys_plmn_list_s_type*)modem_mem_alloc(
                    sizeof(ds_sys_plmn_list_s_type),
                    MODEM_MEM_CLIENT_DATA);
  if (curr_plmn_list_p == NULL)
  {
      local_err = DS_3GPPI_LOCAL_ERR_VAL_2;
      break;
  }

  memset(curr_plmn_list_p, 0 ,sizeof(ds_sys_plmn_list_s_type));

    /*----------------------------------------------------------------- 
     Fetch the current PLMN list
     -----------------------------------------------------------------*/
  ds_3gpp_roaming_get_current_plmn_list(subs_id, new_plmn_id, curr_plmn_list_p);

    for (plmn_index = 0; plmn_index < curr_plmn_list_p->length; plmn_index++)
  {
    match_excp_found = FALSE;

    /* check if any PLMN from current PLMN List is in EHPLMN List */
    if (check_ehplmn_list && read_sessid_success)
    {
        if (ds_3gpp_is_plmn_in_ehplmn_list(curr_plmn_list_p->plmn[plmn_index], subs_id))
      {
        roaming_status = FALSE;
        roaming_type = DS_SYS_ROAMING_UNSPECIFIED;
        break;
      }
    }

    /*-------------------------------------------------------------------- 
      Check if PLMN is in excp_home_plmn_list, if yes, continue to check next
      PLMN in current PLMN list.
    --------------------------------------------------------------------*/
    if (ds_3gpp_config_mgr_get_excp_home_plmn_list_size(subs_id) != 0)
    {
      list_ptr = ds_3gpp_config_mgr_get_single_plmn_list(DS_3GPP_CONFIG_EPLMN, subs_id);
    match_excp_found = 
              ds_3gpp_roaming_is_plmn_found_in_list(curr_plmn_list_p->plmn[plmn_index], list_ptr);
    }

    /*-------------------------------------------------------------------- 
      PLMN is not in excp_home_plmn_list, check if PLMN is in home_plmn_list.
    --------------------------------------------------------------------*/
    if (match_excp_found == FALSE)
    {
      /*-------------------------------------------------------------------- 
        Check if PLMN is in home_plmn_list.
      --------------------------------------------------------------------*/
      list_ptr = ds_3gpp_config_mgr_get_single_plmn_list(DS_3GPP_CONFIG_HPLMN, subs_id);
          if(ds_3gpp_roaming_is_plmn_found_in_list(curr_plmn_list_p->plmn[plmn_index], list_ptr))
      {
        roaming_status = FALSE;
        roaming_type = DS_SYS_ROAMING_UNSPECIFIED;
        break;
      }
    }
    }//for loop ends

  /*----------------------------------------------------------------
      From the CM, it is roaming, we need to check if it is domestic
      roaming or international roaming
      The check logic should be in another loop to avoid overriden.
    roaming type must be determined after the roaming status. 
     
    We determine the Roaming Type based on the decision Box below. 
    Only Applicable when UE is determined to be already in Roaming. 
  ------------------------------------------------------------------*/
  if (roaming_status == TRUE) 
  {
      /*------------------------------------------------------------------------- 
      | International Roam           |  Domestic Roam               | Result     |
      |-------------------------------------------------------------|------------|
      |Tag Configured ?| PLMN Match? |Tag Configured ?| PLMN Match? |            |
      |-------------------------------------------------------------|------------|
      | Yes            | No          | No             | -           | DOMESTIC   |
      | Yes            | No          | Yes            | Yes         | DOMESTIC   |
      | Yes            | No          | Yes            | No          | UNSPECIFIED|
      | Yes            | Yes         | {Dont Care}    | {Dont Care} | INT'L      |
      | No             | -           | Yes            | Yes         | DOMESTIC   |
      | No             | -           | Yes            | No          | INT'L      |
      | No             | -           | No             | -           | UNSPECIFIED|
      ----------------------------------------------------------------------------*/

      intl_roam_list_configured = 
         (ds_3gpp_config_mgr_get_international_roam_plmn_list_size(subs_id) != 0 ?
         TRUE : FALSE);

      dom_roam_list_configured = 
         (ds_3gpp_config_mgr_get_domestic_roam_plmn_list_size(subs_id) != 0 ?
         TRUE : FALSE);

      if ( TRUE == intl_roam_list_configured )
      {
        list_ptr = ds_3gpp_config_mgr_get_single_plmn_list(
                                         DS_3GPP_CONFIG_INTERNATIONAL_ROAM_PLMN,
                                         subs_id);

        for (plmn_index = 0; plmn_index < curr_plmn_list_p->length; plmn_index++)
        {
          if (ds_3gpp_roaming_is_plmn_found_in_list(curr_plmn_list_p->plmn[plmn_index],
                                                    list_ptr))
          {
           /*International Roam has the higest priority, so once a 
             match is found, UE is declared to be in Int'l Roam and
             we break out of the for loop*/
            roaming_type = DS_SYS_ROAMING_INTERNATIONAL;
            break;
          }
        }
        
        /*--------------------------------------------------------------- 
        Same PLMNs can be configured in both Intl and Domestic Roam 
        PLMN lists in carrier XML. In that case, Intl PLMN takes the higher
        priority.
         
        Thus, break out of do..while() loop here if we have found
        the match
        ----------------------------------------------------------------*/
        if ( DS_SYS_ROAMING_INTERNATIONAL == roaming_type )
        {
          break;
        }
      }/*international roam plmn list is configured in XML*/

      if ( TRUE == dom_roam_list_configured )
    {
      list_ptr = ds_3gpp_config_mgr_get_single_plmn_list(
                                       DS_3GPP_CONFIG_DOMESTIC_ROAM_PLMN,
                                       subs_id);

        for (plmn_index = 0; plmn_index < curr_plmn_list_p->length; plmn_index++)
      {
          if (ds_3gpp_roaming_is_plmn_found_in_list(curr_plmn_list_p->plmn[plmn_index],
                                                      list_ptr))
        {
          roaming_type = DS_SYS_ROAMING_DOMESTIC;
          break;
        }
        else
        {
          roaming_type = DS_SYS_ROAMING_INTERNATIONAL;
            //If Intl Roam list is configured, override roam type
            if ( TRUE == intl_roam_list_configured )
            {
              roaming_type = DS_SYS_ROAMING_UNSPECIFIED;
        }
      }
    }
  }
      else if ( TRUE == intl_roam_list_configured )
      { 
        /*If we are here it means that we were unable to find a PLMN match 
        in the intl roam list and the domestic roam list is not configured, 
        so UE is determined to be in domestic roam*/
        roaming_type = DS_SYS_ROAMING_DOMESTIC;
        break;
      }
    }/*roaming status is TRUE*/
  }while (0); 


  if (local_err != DS_3GPPI_LOCAL_ERR_VAL_NONE) 
  {
    DS_3GPP_ERROR_LOG_1("ds_3gpp_roaming_inform_new_plmn_info err: %d", local_err);
  }
  else
  {
    DS_3GPP_ERROR_LOG_4(
                         "Roaming Status: %d (1: Roaming, 0: Home), "
                         "Roaming Type: %d (1: Domestic, 2: Int'l), "
                         "XML config ? Int'l: %d , Domestic: %d ",
                         roaming_status,
                         roaming_type,
                         intl_roam_list_configured,
                         dom_roam_list_configured);

    /*We have determined the roaming status, so set it*/
  ds_3gpp_roaming_set_status(subs_id, roaming_status, roaming_type);
  }

  if (curr_plmn_list_p != NULL)
  {
    modem_mem_free(curr_plmn_list_p, MODEM_MEM_CLIENT_DATA);
  }
  return;
} /* ds_3gpp_roaming_inform_new_plmn_info */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_UPDATE_PLMN_INFO_ON_SUBS_READY

  DESCRIPTION
  This function is invoked when subscription ready ev is received.
  This function calls roaming_inform_new_plnn_onfo
  which will then take action to perform blocking/unblocking.
  
  PARAMETERS  
  Subscription Id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_roaming_update_plmn_info_on_subs_ready
(
  sys_modem_as_id_e_type subs_id
)
{
  sys_plmn_id_s_type zero_plmn_id;
  sys_plmn_id_s_type current_plmn_id;
  /*- - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - -*/

  /*--------------------------------------------------------------------------
    Memset the Zero PLMN Id to 0
   --------------------------------------------------------------------------*/
  memset((void*)&zero_plmn_id, 0, sizeof(sys_plmn_id_s_type));
  ds_3gpp_pdn_cntx_get_current_plmn_ex((&current_plmn_id),
                                        subs_id);

  if(memcmp((void*)&(current_plmn_id),
              (void*)&(zero_plmn_id),
              sizeof(sys_plmn_id_s_type)) != 0)
  {
    DS_3GPP_MSG0_LOW("Updating PLMN info on subs_ready ev");
    ds_3gpp_roaming_inform_new_plmn_info(subs_id,current_plmn_id);
  }

  return;
}/*ds_3gpp_roaming_update_plmn_info_on_subs_ready*/



/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_FLUSH_WAIT_TIMER_ENTRY

  DESCRIPTION
  This function is called to clear the entire wait timer list.
  
  PARAMETERS  
  None.
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gpp_roaming_flush_wait_timer_list
(
  void
)
{
  list_type                             *list_ptr = NULL;
  ds_3gpp_roaming_wait_timer_item_type  *current_list_item_ptr = NULL;
  ds_3gpp_roaming_wait_timer_item_type  *next_list_item_ptr = NULL;
  /*---------------------------------------------------------------------*/

  list_ptr = &ds_3gpp_roaming_info.wait_timer_list;
  
  current_list_item_ptr = 
    (ds_3gpp_roaming_wait_timer_item_type*)list_peek_front( list_ptr );

  while (current_list_item_ptr != NULL)
  {
    next_list_item_ptr = 
      list_peek_next(list_ptr, &(current_list_item_ptr->link));


    DS_3GPP_MSG1_LOW("Clearing the wait timer memory for profile %d", 
                      current_list_item_ptr->profile_num);

    rex_clr_timer(current_list_item_ptr->timer_ptr);

    rex_delete_timer_ex(current_list_item_ptr->timer_ptr);

    list_pop_item(list_ptr, &(current_list_item_ptr->link) );

    modem_mem_free(current_list_item_ptr,MODEM_MEM_CLIENT_DATA);

    current_list_item_ptr = next_list_item_ptr;
    
  }
  return;
}

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_CLEAR_WAIT_TIMER_ENTRY

  DESCRIPTION
  This function is called to clear the wait timer corresponding to the given
  profile.
  
  PARAMETERS  
  Profile Num
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
void ds_3gpp_roaming_clear_wait_timer_entry
(
  uint8 profile_num  
)
{
  list_type                             *list_ptr = NULL;
  ds_3gpp_roaming_wait_timer_item_type  *current_list_item_ptr = NULL;
  /*---------------------------------------------------------------------*/

  list_ptr = &ds_3gpp_roaming_info.wait_timer_list;
  
  current_list_item_ptr = 
    (ds_3gpp_roaming_wait_timer_item_type*)list_peek_front( list_ptr );

  while (current_list_item_ptr != NULL)
  {
    if ( current_list_item_ptr->profile_num == profile_num )
    {
      DS_3GPP_MSG1_LOW("Clearing the wait timer memory for profile %d",
                        profile_num);

      rex_clr_timer(current_list_item_ptr->timer_ptr);

      rex_delete_timer_ex(current_list_item_ptr->timer_ptr);

      list_pop_item(list_ptr, &(current_list_item_ptr->link) );

      modem_mem_free(current_list_item_ptr,MODEM_MEM_CLIENT_DATA);

      break;
    }

    current_list_item_ptr = 
      list_peek_next(list_ptr, &(current_list_item_ptr->link));
  }

  return;
}

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_WAIT_TIMER_EXP_CB

  DESCRIPTION
  This function callback is invoked when the PDN Disconnect wait timer
  expires.
  
  PARAMETERS  
  Callback Data
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_wait_timer_exp_cb
(
  unsigned long cb_data
)
{
  uint8                                  profile_num = 0;
  ds_3gpp_roaming_wait_timer_item_type  *cb_data_ptr = NULL;
  list_type                             *list_ptr = NULL;
  ds_3gpp_roaming_wait_timer_item_type  *current_list_item_ptr = NULL;
  sys_modem_as_id_e_type                 subs_id = SYS_MODEM_AS_ID_NONE;
  /*---------------------------------------------------------------------*/

  cb_data_ptr = (ds_3gpp_roaming_wait_timer_item_type*)cb_data;

  if (cb_data_ptr == NULL)
  {
    DS_3GPP_ERROR_LOG_0("Null ptr passed");
    return;
  }

  profile_num = cb_data_ptr->profile_num;
  subs_id = cb_data_ptr->subs_id;

  DS_3GPP_MSG2_MED_EX(subs_id,
                "Wait timer expired, Profile number: %d subs_id: %d "
                "Try to tear down the PDN", profile_num,subs_id);

  ds_3gpp_pdn_cntx_teardown_pdn_per_profile
    (profile_num, PS_NET_DOWN_REASON_APN_DISALLOWED_ON_ROAMING, subs_id);    

  list_ptr = &ds_3gpp_roaming_info.wait_timer_list;
  
  current_list_item_ptr = 
    (ds_3gpp_roaming_wait_timer_item_type*)list_peek_front( list_ptr );

  while (current_list_item_ptr != NULL)
  {
    if ( current_list_item_ptr->profile_num == profile_num &&
         current_list_item_ptr->subs_id == subs_id )
    {
      DS_3GPP_MSG0_LOW("Clearing the wait timer memory");

      rex_delete_timer_ex(current_list_item_ptr->timer_ptr);

      list_pop_item(list_ptr, &(current_list_item_ptr->link) );

      modem_mem_free(current_list_item_ptr,MODEM_MEM_CLIENT_DATA);

      break;
    }

    current_list_item_ptr = 
      list_peek_next(list_ptr, &(current_list_item_ptr->link));
  }
  
} /* ds_3gpp_roaming_wait_timer_exp_cb */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_ALLOCATE_AND_START_WAIT_TIMER

  DESCRIPTION
  This function tries to allocate and start the wait timer.
  
  PARAMETERS  
  Profile for which the wait timer is configured.
  subs_id: The subscription for which to start the timer
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  TRUE, if Wait Timer started or no need to Start Wait Timer
  FALSE, if Wait Timer Did not Start
   
  SIDE EFFECTS 
  None.
===========================================================================*/ 

static boolean ds_3gpp_roaming_allocate_and_start_wait_timer
(
  ds_umts_pdp_profile_type   *profile_info_ptr,
  sys_modem_as_id_e_type      subs_id
)
{
  ds_3gpp_roaming_wait_timer_item_type *wait_timer_list_item_ptr = NULL;
  uint8                                 pdn_discon_wait_time = 0;
  uint8                                 profile_num = 0;
  boolean                               ret_val = FALSE;
  /*----------------------------------------------------------------*/

  DS3GPP_ASSERT(profile_info_ptr != NULL);

  pdn_discon_wait_time = profile_info_ptr->pdn_discon_wait_time;
  profile_num = profile_info_ptr->context.pdp_context_number;

  DS_3GPP_MSG3_HIGH_EX(subs_id,
           "Profile number: %d PDN Discon Wait Time: %d min sub %d", 
           profile_num, pdn_discon_wait_time, subs_id );

  if (pdn_discon_wait_time != 0)
  {
    if (pdn_discon_wait_time == DS_3GPP_ROAMING_MAX_PDN_DISCON_WAIT_TIME)
    {
      DS_3GPP_MSG0_LOW("Responsibility of Client APP to perform dereg "
                        "when it deems necessary. ");

      ret_val = TRUE;
    }
    else
    {
      wait_timer_list_item_ptr = 
        modem_mem_alloc(sizeof(ds_3gpp_roaming_wait_timer_item_type),
                            MODEM_MEM_CLIENT_DATA );

      if (wait_timer_list_item_ptr != NULL)
      {
        wait_timer_list_item_ptr->timer_ptr =  
          rex_create_timer_ex (ds_3gpp_roaming_wait_timer_exp_cb, 
                               (unsigned long)wait_timer_list_item_ptr);

        if (wait_timer_list_item_ptr->timer_ptr != NULL)
        {
          wait_timer_list_item_ptr->profile_num = profile_num;
          wait_timer_list_item_ptr->subs_id = subs_id;

          list_push_front(&ds_3gpp_roaming_info.wait_timer_list, 
                              &(wait_timer_list_item_ptr->link));

          rex_set_timer(wait_timer_list_item_ptr->timer_ptr, 
                        pdn_discon_wait_time * 60 * 1000);

          DS_3GPP_MSG3_HIGH_EX(subs_id,
                            "Started the PDN Disconnect Wait Timer "
                            "for %d minutes, profile: %d, sub %d", 
                            pdn_discon_wait_time, profile_num, subs_id );
          ret_val = TRUE;
        }
        else
        {
          modem_mem_free(wait_timer_list_item_ptr,MODEM_MEM_CLIENT_DATA);
          DS_3GPP_MSG0_LOW("Cannot allocate wait timer !");
        }      
      }
      else
      {
        DS_3GPP_MSG0_LOW("Cannot allocate memory !");
      }
    }
  }

  return ret_val;
} /* ds_3gpp_roaming_allocate_and_start_wait_timer */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_BLOCK_APN_PER_PROFILE

  DESCRIPTION
  This function blocks given APN corresponding to given profile
  because of Roaming.
  
  PARAMETERS  
  Subscription id
  Profile Information Ptr
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_block_apn_per_profile
(
  sys_modem_as_id_e_type     subs_id,
  ds_umts_pdp_profile_type   *profile_info_ptr
)
{
  ds_pdn_context_state_e            state = DS_PDN_CONTEXT_STATE_DOWN;
  boolean                           tear_down_pdn = TRUE;
  ds_pdn_context_s*                 pdn_cntxt_p = NULL;
  ds_3gpp_iface_s                   *ds_v4_iface_ptr = NULL;
  ds_3gpp_iface_s                   *ds_v6_iface_ptr = NULL;
  /*---------------------------------------------------------------------------*/

  DS3GPP_ASSERT(profile_info_ptr != NULL);

  pdn_cntxt_p = ds_pdn_cntxt_get_pdn_context_from_cid
                  (profile_info_ptr->context.pdp_context_number,subs_id);

  if(ds_3gpp_pdn_cntx_validate_pdn_context(pdn_cntxt_p))
  {
    state = pdn_cntxt_p->ds_pdn_context_dyn_p->state;
    DS_3GPP_MSG1_HIGH_EX(subs_id,"PDN Cntxt is in %d state", state);
  }

  switch (state)
  {
    case DS_PDN_CONTEXT_STATE_UP:
    {
      ds_v4_iface_ptr = 
        pdn_cntxt_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl
          [DS_3GPP_PDN_CONTEXT_IPV4_IFACE];

      ds_v6_iface_ptr = 
        pdn_cntxt_p->ds_pdn_context_dyn_p->ds_3gpp_iface_tbl
          [DS_3GPP_PDN_CONTEXT_IPV6_IFACE];

      if ( (ds_v4_iface_ptr != NULL && 
            PS_IFACE_IS_IN_USE(&(ds_v4_iface_ptr->ps_iface))) ||
          (ds_v6_iface_ptr != NULL && 
           PS_IFACE_IS_IN_USE(&(ds_v6_iface_ptr->ps_iface))) )
      {

        if (ds_3gpp_roaming_allocate_and_start_wait_timer (profile_info_ptr,
                                                           subs_id))
        {
          tear_down_pdn = FALSE;
        }
      }
      break;
    }

    case DS_PDN_CONTEXT_STATE_DOWN:
    case DS_PDN_CONTEXT_STATE_GOING_DOWN:
    {
      tear_down_pdn = FALSE;
      break;
    }

    default:
      break;
  }

  if (tear_down_pdn)
  {
    ds_3gpp_pdn_cntx_teardown_pdn_per_profile 
      (profile_info_ptr->context.pdp_context_number, 
         PS_NET_DOWN_REASON_APN_DISALLOWED_ON_ROAMING,subs_id);
  }

  ds_3gpp_pdn_throt_set_global_throt_mask
    (profile_info_ptr->context.apn, subs_id, 
       DS_3GPP_PDN_THROT_ROAMING_DISABLED_MASK);

} /* ds_3gpp_roaming_block_apn_per_profile */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_BLOCK_APNS

  DESCRIPTION
  This function blocks all APNs which are disallowed during Roaming.
  
  PARAMETERS  
  Subscription id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_block_apns
(
  sys_modem_as_id_e_type subs_id
)
{
  ds_umts_pdp_profile_type               profile_info;
  ds_umts_pdp_profile_status_etype       result = DS_UMTS_PDP_FAIL;
  int                                    i = 0; 
  ds3gsubsmgr_subs_id_e_type             subs_index = 0;
  uint8                                  profile_nums_arr[DS_UMTS_MAX_PDP_PROFILE_NUM];
  uint8                                  profile_count = 0;
  /*----------------------------------------------------------------------*/

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return;
  }  

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);

  profile_count = 
    ds_3gpp_profile_cache_get_valid_profile_numbers(
      dsumts_subs_mgr_get_subs_id((sys_modem_as_id_e_type)subs_index), 
      profile_nums_arr, DS_UMTS_MAX_PDP_PROFILE_NUM);

  for (i=0; i< profile_count; i++)
  {
    memset(&profile_info, 0, sizeof(ds_umts_pdp_profile_type));

    result = ds_umts_get_pdp_profile_all_data_per_subs(profile_nums_arr[i], 
                               dsumts_subs_mgr_get_subs_id(subs_id), &profile_info);

    if (result != DS_UMTS_PDP_SUCCESS)
    {
      continue;
    }

    if (profile_info.roaming_disallowed)
    {
      DS_3GPP_MSG2_HIGH_EX(subs_id,
                        "Roaming Disallowed Flag: %d, Profile Id: %d ",
                        profile_info.roaming_disallowed, profile_nums_arr[i]);
      ds_3gpp_roaming_block_apn_per_profile(subs_id, &profile_info);
    }
  } /* for (i=0; i< profile_count; i++) */

} /* ds_3gpp_roaming_block_apns */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_UNBLOCK_APN_PER_PROFILE

  DESCRIPTION
  This function unblocks APN corresponding to the given profile.
  
  PARAMETERS  
  Subscription id
  Profile Information Ptr
  Boolean Pointer set to True if the attach apn is unblocked
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_unblock_apn_per_profile
(
  sys_modem_as_id_e_type    subs_id,
  ds_umts_pdp_profile_type  *profile_info_ptr,
  boolean*                  is_attach_apn_ptr
)
{
  /*-------------------------------------------------------------------------*/


  if (is_attach_apn_ptr == NULL)
  {
    DS_3GPP_MSG0_LOW("is_attach_apn_ptr is NULL !");
    return;
  }

  if (profile_info_ptr != NULL)
  {
    ds_3gpp_roaming_clear_wait_timer_entry
      ((uint8)profile_info_ptr->context.pdp_context_number);

    ds_3gpp_roaming_unblock_throttling
      (profile_info_ptr->context.apn,subs_id, FALSE,is_attach_apn_ptr);
  }
  else
  {
    DS_3GPP_MSG0_LOW("Profile info ptr is NULL !");
  }
} /* ds_3gpp_roaming_unblock_apn_per_profile */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_UNBLOCK_APNS

  DESCRIPTION
  This function unblocks all APNs which were disallowed during Roaming.
  
  PARAMETERS  
  Subscription id
  Boolean Pointer set to True if the attach apn is unblocked
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
static void ds_3gpp_roaming_unblock_apns
(
  sys_modem_as_id_e_type subs_id,
  boolean*               is_attach_apn_ptr
)
{
  /*---------------------------------------------------------------------*/


  if (is_attach_apn_ptr == NULL)
  {
    DS_3GPP_MSG0_LOW("is_attach_apn_ptr is NULL !");
    return;
  }

  ds_3gpp_roaming_flush_wait_timer_list();

  ds_3gpp_roaming_unblock_throttling(NULL, subs_id, TRUE,is_attach_apn_ptr);

} /* ds_3gpp_roaming_unblock_apns */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_ADVERTISE_APNS_BLOCKED_DUE_TO_ROAMING

  DESCRIPTION
  This function advertises all Roaming Blocked APNs to its clients
  
  PARAMETERS  
  Subscription id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/  
void ds_3gpp_roaming_advertise_apns_blocked_due_to_roaming
(
  sys_modem_as_id_e_type subs_id,
  boolean                advertise_empty_list
)
{
  ds_umts_pdp_profile_type                       *profile_info_p = NULL;
  ds_umts_pdp_profile_status_etype               result = DS_UMTS_PDP_FAIL;
  int                                            i = 0; 
  ds3gsubsmgr_subs_id_e_type                     subs_index = 0;
  int16                                          ps_errno = 0;
  ps_sys_roaming_disallowed_info_type            *roaming_disallowed_info_p = NULL;
  uint8                                          num_valid_apns = 0;
  uint8                                         profile_nums_arr[DS_UMTS_MAX_PDP_PROFILE_NUM];
  uint8                                         profile_count = 0;
  /*----------------------------------------------------------------------*/

  subs_index = ds3gsubsmgr_subs_id_cm_to_ds3g(subs_id);
  roaming_disallowed_info_p = (ps_sys_roaming_disallowed_info_type*)modem_mem_alloc(
                             sizeof(ps_sys_roaming_disallowed_info_type),
                             MODEM_MEM_CLIENT_DATA);
  if (roaming_disallowed_info_p == NULL)
  {
    return;
  }

  memset(roaming_disallowed_info_p, 0, 
         sizeof(ps_sys_roaming_disallowed_info_type));

  profile_info_p = (ds_umts_pdp_profile_type*)modem_mem_alloc(
                             sizeof(ds_umts_pdp_profile_type),
                             MODEM_MEM_CLIENT_DATA);
  if (profile_info_p == NULL)
  {
    modem_mem_free(roaming_disallowed_info_p, MODEM_MEM_CLIENT_DATA);
    return;
  }

  if (ds_3gpp_roaming_get_status(subs_id) && advertise_empty_list == FALSE )
  {
    profile_count = ds_3gpp_profile_cache_get_valid_profile_numbers
      (dsumts_subs_mgr_get_subs_id((sys_modem_as_id_e_type)subs_index),
       profile_nums_arr, DS_UMTS_MAX_PDP_PROFILE_NUM);

    for (i=0; i < profile_count; i++)
    {
      memset(profile_info_p, 0, sizeof(ds_umts_pdp_profile_type));

      result = 
        ds_umts_get_pdp_profile_all_data_per_subs(profile_nums_arr[i], 
          dsumts_subs_mgr_get_subs_id((sys_modem_as_id_e_type)subs_index), 
          profile_info_p);

      if (result != DS_UMTS_PDP_SUCCESS)
      {
        continue;
      }

      if (profile_info_p->roaming_disallowed && num_valid_apns < PS_SYS_MAX_APNS)
      {
        memscpy(roaming_disallowed_info_p->apn_list[num_valid_apns].apn_name , 
                PS_SYS_MAX_APN_LEN,
                profile_info_p->context.apn, 
                strlen((const char*)profile_info_p->context.apn) + 1);
        DATA_3GPP_MSG_SPRINTF_3_EX(MSG_LEGACY_HIGH, subs_id,
                           "Roaming Disallowed Flag: %d, Profile Id: %d, "
                           "Preparing Roaming Disallowed Ind for APN: %s",
             profile_info_p->roaming_disallowed, profile_nums_arr[i],
             roaming_disallowed_info_p->apn_list[num_valid_apns].apn_name);

        num_valid_apns++;

      }
    }

    if (num_valid_apns > 0)
    {
      roaming_disallowed_info_p->is_roaming_disallowed = TRUE;
      roaming_disallowed_info_p->num_apn_names = num_valid_apns;
    }
  } /* if (ds_3gpp_roaming_get_status(subs_id)) */

  DS_3GPP_MSG1_LOW("Roaming Disallowed Ind. Number of Blocked APNs: %d ",
                    roaming_disallowed_info_p->num_apn_names);

  if (0 > ps_sys_conf_set_ex(PS_SYS_TECH_3GPP,
                             PS_SYS_CONF_3GPP_ROAMING_DISALLOWED_INFO,
                             (ps_sys_subscription_enum_type)
                              ds3gsubsmgr_subs_id_ds3g_to_ds(subs_index),
                              roaming_disallowed_info_p,
                              &ps_errno))
  {
    DS_3GPP_MSG0_LOW("Sending Roaming Disallowed Info Failed");
  }
  modem_mem_free(roaming_disallowed_info_p, MODEM_MEM_CLIENT_DATA);
  modem_mem_free(profile_info_p, MODEM_MEM_CLIENT_DATA);
} /* ds_3gpp_roaming_advertise_apns_blocked_due_to_roaming */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_ADVERTISE_THROT_INFO

  DESCRIPTION
  This function advertises Throttling Information to all clients and
  disables lte if needed
  
  PARAMETERS
  1. disable_lte_if_needed: Flag indicating whether LTE needs to be disabled
  2. Subscription id
  3. Boolean set to True if the attach apn is unblocked
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/ 
static void ds_3gpp_roaming_advertise_throt_info
(
  boolean                disable_lte_if_needed,
  sys_modem_as_id_e_type subs_id,
  boolean                is_attach_apn
)
{
  ds_3gpp_pdn_throt_advertise_blocked_apns_current_plmn(subs_id);

#ifdef FEATURE_DATA_LTE
  if (disable_lte_if_needed)
  {
    ds_3gpp_pdn_cntxt_disable_lte_if_attach_profile_unavailable(subs_id);
  }
  else
  {
    if (is_attach_apn)
    {
      ds_3gpp_pdn_cntxt_enable_lte_if_attach_profile_available(subs_id);
    }
  }
#endif /* FEATURE_DATA_LTE */

} /* ds_3gpp_roaming_advertise_throt_info */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_PROCESS_DISALLOWED_FLAG_CHG_CMD

  DESCRIPTION
  This function is used to process the change in Roaming Disallowed Flag
  
  PARAMETERS  
  Profile Number
  Roaming Disallowed Flag
  Subscription Id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gpp_roaming_process_disallowed_flag_chg_cmd
(
  uint16                 profile_num,
  boolean                roaming_disallowed_flag,
  sys_modem_as_id_e_type subs_id
)
{
  ds_umts_pdp_profile_type           profile_info;
  ds_umts_pdp_profile_status_etype   result = DS_UMTS_PDP_FAIL;     
  boolean                            is_attach_apn = FALSE;   
  /*------------------------------------------------------------------------*/

  ds3gpp_enter_global_crit_section();
  if (ds_3gpp_roaming_get_status(subs_id) == FALSE)
  {
    ds3gpp_leave_global_crit_section();
    DS_3GPP_MSG0_HIGH_EX(subs_id, "UE is not roaming. "
                      "No need to process change to disallowed flag");
    return;
  }
  ds3gpp_leave_global_crit_section();
  
  result = ds_umts_get_pdp_profile_all_data_per_subs(profile_num, 
                            dsumts_subs_mgr_get_subs_id(subs_id), &profile_info);
  
  if (result != DS_UMTS_PDP_SUCCESS)
  {
    DS_3GPP_ERROR_LOG_1_EX(subs_id, "Cannot read profile num d ", profile_num);
    return;
  }
  
  if (roaming_disallowed_flag == TRUE)
  {
    ds_3gpp_roaming_block_apn_per_profile(subs_id, &profile_info);   
  }
  else
  {
    ds_3gpp_roaming_unblock_apn_per_profile(subs_id, &profile_info,&is_attach_apn);
  }

  ds_3gpp_roaming_advertise_throt_info(roaming_disallowed_flag, 
                                       subs_id,
                                       is_attach_apn);
  
  ds_3gpp_roaming_advertise_apns_blocked_due_to_roaming(subs_id, FALSE);  

  return;
} /* ds_3gpp_roaming_process_disallowed_flag_chg_cmd */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_PROCESS_PLMN_LIST_CHG_CMD

  DESCRIPTION
  This function is used to process the change in PLMN List.
  
  PARAMETERS  
  Subscription Id
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/
void ds_3gpp_roaming_process_plmn_list_chg_cmd
(
  sys_modem_as_id_e_type   subs_id
)
{
  sys_plmn_id_s_type       current_plmn_id;
  sys_plmn_id_s_type       zero_plmn_id;
  ds_eps_attach_sm_s       *attach_sm_ptr = NULL;
  /*------------------------------------------------------------------------*/

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return;
  }  

  memset(&current_plmn_id, 0, sizeof(sys_plmn_id_s_type));
  memset(&zero_plmn_id, 0, sizeof(sys_plmn_id_s_type));

  attach_sm_ptr = ds_eps_get_attach_sm_per_sub_id(subs_id);

  if((ds3gpp_get_current_network_mode(subs_id) == SYS_SYS_MODE_LTE ) ||
    (attach_sm_ptr && attach_sm_ptr->attach_state == DS_EPS_PROCESSED_CNTXT_TXFR_IND))
  {
    ds_3gpp_pdn_cntx_get_current_lte_plmn(&current_plmn_id, subs_id);
  }
  else
  {
    ds3g_get_current_plmn_id(&current_plmn_id, subs_id);
  }

  if(memcmp(&(current_plmn_id),
            &(zero_plmn_id),
            sizeof(sys_plmn_id_s_type)) != 0)
  {
    ds_3gpp_roaming_inform_new_plmn_info(subs_id, current_plmn_id);
  }

  return;
} /* ds_3gpp_roaming_process_plmn_list_chg_cmd */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_WRITE_HPLMN_LIST_CMD_PROCESS

  DESCRIPTION    
  This function is used to process the cmd to write hplmn list efs file.
  
  PARAMETERS     
  sys_modem_as_id_e_type         subs_id
  ps_sys_3gpp_plmn_list_type   *hplmn_list_ptr
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
===========================================================================*/
static void ds_3gpp_roaming_write_hplmn_list_cmd_process
(
  sys_modem_as_id_e_type         subs_id,
  ps_sys_3gpp_plmn_list_type   *hplmn_list_ptr
)
{
  char                                    *change_info_tag[DS_3GPP_ROAMING_QMI_MAX] = {NULL};
  ds_3gpp_roaming_qmi_tag_e                index;

  /*------------------------------------------------------------------------*/
  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return;
  }  

  if(NULL == hplmn_list_ptr)
  {
    return;
  }

  /*------------------------------------------------------------------------
    Once there is QMI command to update the PLMN info. All the PLMN list
    would be updated. If the num_list = 0, the existing list in XML file
    would be delted.
  -----------------------------------------------------------------------*/
  change_info_tag[DS_3GPP_ROAMING_QMI_HPLMN] = (char *)modem_mem_alloc(
                        strlen(DS_3GPP_CONFIG_HOME_PLMN_LIST_TAG)+1,
                        MODEM_MEM_CLIENT_DATA );
  change_info_tag[DS_3GPP_ROAMING_QMI_EXCP_HPLMN] = (char *)modem_mem_alloc(
                        strlen(DS_3GPP_CONFIG_EXCEPTION_HPLMN_LIST_TAG)+1,
                        MODEM_MEM_CLIENT_DATA );
  change_info_tag[DS_3GPP_ROAMING_QMI_DOMESTIC_RPLMN] = (char *)modem_mem_alloc(
                        strlen(DS_3GPP_CONFIG_DOMESTIC_ROAM_PLMN_LIST_TAG)+1,
                        MODEM_MEM_CLIENT_DATA );
  change_info_tag[DS_3GPP_ROAMING_QMI_INTERNATIONAL_RPLMN] = (char *)modem_mem_alloc(
                        strlen(DS_3GPP_CONFIG_INTL_ROAM_PLMN_LIST_TAG)+1,
                        MODEM_MEM_CLIENT_DATA );
  if (change_info_tag[DS_3GPP_ROAMING_QMI_HPLMN] != NULL && 
      change_info_tag[DS_3GPP_ROAMING_QMI_EXCP_HPLMN] != NULL &&
      change_info_tag[DS_3GPP_ROAMING_QMI_DOMESTIC_RPLMN] != NULL &&
      change_info_tag[DS_3GPP_ROAMING_QMI_INTERNATIONAL_RPLMN] != NULL)
  {
    memset((void *)change_info_tag[DS_3GPP_ROAMING_QMI_HPLMN],
            '\0',
            strlen(DS_3GPP_CONFIG_HOME_PLMN_LIST_TAG)+1);
    memset((void *)change_info_tag[DS_3GPP_ROAMING_QMI_EXCP_HPLMN],
            '\0',
            strlen(DS_3GPP_CONFIG_EXCEPTION_HPLMN_LIST_TAG)+1);
    memset((void *)change_info_tag[DS_3GPP_ROAMING_QMI_DOMESTIC_RPLMN],
            '\0',
            strlen(DS_3GPP_CONFIG_DOMESTIC_ROAM_PLMN_LIST_TAG)+1);
    memset((void *)change_info_tag[DS_3GPP_ROAMING_QMI_INTERNATIONAL_RPLMN],
            '\0',
            strlen(DS_3GPP_CONFIG_INTL_ROAM_PLMN_LIST_TAG)+1);
    strlcpy(change_info_tag[DS_3GPP_ROAMING_QMI_HPLMN],
            DS_3GPP_CONFIG_HOME_PLMN_LIST_TAG,
            strlen(DS_3GPP_CONFIG_HOME_PLMN_LIST_TAG)+1);
    strlcpy(change_info_tag[DS_3GPP_ROAMING_QMI_EXCP_HPLMN],
            DS_3GPP_CONFIG_EXCEPTION_HPLMN_LIST_TAG,
            strlen(DS_3GPP_CONFIG_EXCEPTION_HPLMN_LIST_TAG)+1);
    strlcpy(change_info_tag[DS_3GPP_ROAMING_QMI_DOMESTIC_RPLMN],
            DS_3GPP_CONFIG_DOMESTIC_ROAM_PLMN_LIST_TAG,
            strlen(DS_3GPP_CONFIG_DOMESTIC_ROAM_PLMN_LIST_TAG)+1);
    strlcpy(change_info_tag[DS_3GPP_ROAMING_QMI_INTERNATIONAL_RPLMN],
            DS_3GPP_CONFIG_INTL_ROAM_PLMN_LIST_TAG,
            strlen(DS_3GPP_CONFIG_INTL_ROAM_PLMN_LIST_TAG)+1);
  }

  if (TRUE == ds_3gpp_config_mgr_qmi_write_to_efs(subs_id,
                                                  (void *)hplmn_list_ptr,
                                                  change_info_tag,
                                                  DS_3GPP_ROAMING_QMI_MAX)) 
  {
    ds_3gpp_roaming_process_plmn_list_chg_cmd(subs_id);
  }
  else
  {
    DS_3GPP_MSG0_MED_EX(subs_id, "Unable to write to plmn list efs");
  }
  for (index = DS_3GPP_ROAMING_QMI_MIN; index < DS_3GPP_ROAMING_QMI_MAX; index++)
  {
     if (change_info_tag[index] != NULL )
     {
       modem_mem_free(change_info_tag[index], MODEM_MEM_CLIENT_DATA);
     }
  }
  return;
} /* ds_3gpp_roaming_write_hplmn_list_cmd_process */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_HDLR_PROCESS_CMDS

  DESCRIPTION
  This function processes any commands that need to be handled by
  Roaming Handler Module.
  
  PARAMETERS
  1. Cmd Ptr
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_roaming_hdlr_process_cmds
(
  const ds_cmd_type      *cmd_ptr
)
{
  ds_3gpp_roam_info_type                  *roam_info_ptr = NULL;
  sys_modem_as_id_e_type                  *subs_id_ptr = NULL;
  ds_3gpp_config_mgr_plmn_list_info_type  *hplmn_list_info_ptr = NULL;

  DS3GPP_ASSERT(cmd_ptr != NULL);
  DS3GPP_ASSERT(cmd_ptr->cmd_payload_ptr != NULL);

  switch (cmd_ptr->hdr.cmd_id)
  {
    case DS_CMD_3GPP_ROAMING_DISALLOWED_FLAG_CHG:
    {
      roam_info_ptr = (ds_3gpp_roam_info_type*)cmd_ptr->cmd_payload_ptr;

      ds_3gpp_roaming_process_disallowed_flag_chg_cmd(
         roam_info_ptr->profile_num,
         roam_info_ptr->roaming_disallowed_flag,
         roam_info_ptr->subs_id);
      break;
    }

    case DS_CMD_3GPP_PLMN_LIST_CHG:
    {
      subs_id_ptr = (sys_modem_as_id_e_type*)cmd_ptr->cmd_payload_ptr;
      ds_3gpp_roaming_process_plmn_list_chg_cmd(*subs_id_ptr);
      break;
    }

    case DS_CMD_3GPP_WRITE_HPLMN_LIST:
    {
      hplmn_list_info_ptr = 
          (ds_3gpp_config_mgr_plmn_list_info_type*)cmd_ptr->cmd_payload_ptr;

      ds_3gpp_roaming_write_hplmn_list_cmd_process(
          hplmn_list_info_ptr->subs_id,
          hplmn_list_info_ptr->hplmn_list_ptr);
      break;
    }

    default:
    {
      DS_3GPP_MSG1_LOW("Unrecognized cmd: %d, ignoring",cmd_ptr->hdr.cmd_id);
      break;
    }
  }

  return;
} /* ds_3gpp_roaming_hdlr_process_cmds */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_IS_APN_DISALLOWED

  DESCRIPTION
  This function checks whether PDN Connection to given APN is allowed or not.
  
  PARAMETERS
  1. APN Name
  2. Subscription Id
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  TRUE, if PDN Connection is allowed
  FALSE, otherwise
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_roaming_is_apn_disallowed
(
  byte                   *apn,
  sys_modem_as_id_e_type subs_id
)
{
  ds_umts_pdp_profile_type                       profile_info;
  ds_umts_pdp_profile_status_etype               result = DS_UMTS_PDP_FAIL;
  int                                            i; 
  boolean                                        ret_val = FALSE;
  uint8                                         profile_nums_arr[DS_UMTS_MAX_PDP_PROFILE_NUM];
  uint8                                         profile_count = 0;

  do
  {
    ds3gpp_enter_global_crit_section();
    if (ds_3gpp_roaming_get_status(subs_id) == FALSE)
    {
      ds3gpp_leave_global_crit_section();
      DS_3GPP_MSG0_HIGH_EX(subs_id, "UE is not roaming. "
                        "No need to process change to disallowed flag");
      break;
    }
    ds3gpp_leave_global_crit_section();

    profile_count = ds_3gpp_profile_cache_get_valid_profile_numbers(
                                                 dsumts_subs_mgr_get_subs_id(subs_id),
                                                 profile_nums_arr, 
                                                                     DS_UMTS_MAX_PDP_PROFILE_NUM);


    for (i=0; i< profile_count; i++)
    {
      memset(&profile_info, 0, sizeof(ds_umts_pdp_profile_type));
      result = ds_umts_get_pdp_profile_all_data_per_subs(profile_nums_arr[i],
                                 dsumts_subs_mgr_get_subs_id(subs_id), &profile_info);

      if (result != DS_UMTS_PDP_SUCCESS)
      {
        continue;
      }

      if (profile_info.roaming_disallowed)
      {
        DS_3GPP_MSG2_HIGH_EX(subs_id,
            "Roaming Disallowed Flag: %d, Profile Id: %d ",
            profile_info.roaming_disallowed, profile_nums_arr[i]);
      }

      if ( memcmp(profile_info.context.apn, apn, strlen((const char*)apn) + 1) == 0 )
      {
        if (profile_info.roaming_disallowed)
        {
          ret_val = TRUE;
          break;
        }
      }
    }
  } while (0); 
  
  return ret_val;
} /* ds_3gpp_roaming_is_apn_disallowed */

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_GET_CURRENT_PLMN_LIST

  DESCRIPTION
  This function gets the current PLMN List. If current PLMN is same as RPLMN
  or part of EPLMN list, copy RPLMN and EPLMN list to current PLMN list.
  Otherwise, copy current PLMN to current PLMN list.
  
  PARAMETERS
  Subscription Id
  New PLMN Id
  Current PLMN List
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_roaming_get_current_plmn_list
(
  sys_modem_as_id_e_type     subs_id,
  sys_plmn_id_s_type         new_plmn_id,
  ds_sys_plmn_list_s_type   *curr_plmn_list_p
)
{
  sys_plmn_id_s_type         rplmn;
  sys_plmn_list_s_type       eplmn_list;
  boolean                    match_found = FALSE;
  int                        i;
  /*-----------------------------------------------------------------------*/

  if (curr_plmn_list_p == NULL)
  {
    return;
  }

  if (!ds3gsubsmgr_is_subs_id_valid(subs_id))
  {
    return;
  }  

  memset(&rplmn, 0, sizeof(sys_plmn_id_s_type)); 
  memset(&eplmn_list, 0, sizeof(sys_plmn_list_s_type));

  /*-------------------------------------------------------------------------
    Get EPLMN list and RPLMN
  -------------------------------------------------------------------------*/
  ds_3gpp_get_eplmn_list_per_sub(subs_id,
                                 &eplmn_list,
                                 &rplmn);

  /* Compare current PLMN with RPLMN and EPLMN list */
  if (sys_plmn_match(rplmn, new_plmn_id))
  {
    match_found = TRUE;
  }
  else
  {
    for (i = 0; i < eplmn_list.length; i++)
    {
      if (sys_plmn_match(eplmn_list.plmn[i], new_plmn_id))
      {
        match_found = TRUE;
        break;
      }
    }
  }

   /* If current PLMN is same as RPLMN or part of EPLMN list, copy RPLMN and
     EPLMN list to current PLMN list. Otherwise, copy current PLMN to current
     PLMN list */
  if (match_found &&
      eplmn_list.length <= SYS_PLMN_LIST_MAX_LENGTH )
  {
    /*Copy the EPLMN list*/
    memscpy((void*)&curr_plmn_list_p->plmn,
            sizeof(sys_plmn_id_s_type)*(eplmn_list.length),
            &eplmn_list.plmn,
            sizeof(sys_plmn_id_s_type)*(eplmn_list.length));
    curr_plmn_list_p->length = eplmn_list.length;

    /*Copy the RPLMN list*/
    memscpy(&curr_plmn_list_p->plmn[eplmn_list.length],
            sizeof(sys_plmn_id_s_type),
            &rplmn,
            sizeof(sys_plmn_id_s_type));
    curr_plmn_list_p->length++;
  }
  else
  {
    memscpy(&curr_plmn_list_p->plmn[0],
            sizeof(sys_plmn_id_s_type),
            &new_plmn_id,
            sizeof(sys_plmn_id_s_type));
    curr_plmn_list_p->length = 1;
  }
}/* ds_3gpp_roaming_get_current_plmn_list */

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_IS_PLMN_FOUND_IN_LIST

  DESCRIPTION
  This function checks whether the PLMN can be found in a given PLMN List.
  
  PARAMETERS
  PLMN Id
  PLMN List Ptr
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Boolean.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_roaming_is_plmn_found_in_list
(
  sys_plmn_id_s_type          plmn_id,
  list_type                  *list_ptr
)
{
  boolean                                     ret = FALSE;
  ds_3gpp_config_mgr_plmn_list_item_type     *item_ptr = NULL;
  /*-----------------------------------------------------------------------*/
  if (list_ptr == NULL)
  {
    return FALSE;
  }

  item_ptr = (ds_3gpp_config_mgr_plmn_list_item_type*)
                 list_peek_front( list_ptr );

  while (item_ptr != NULL)
  {
    if (ds_3gpp_roaming_is_plmn_match(plmn_id, item_ptr->plmn_id))
    {
      ret = TRUE;
      break;
    }
    item_ptr = list_peek_next(list_ptr, &(item_ptr->link));
  }

  return ret;
}/*ds_3gpp_roaming_is_plmn_found_in_list*/

/*===========================================================================
FUNCTION DS_3GPP_PDN_THROT_UNBLOCK_ROAMING_THROTTLING

DESCRIPTION
  This function unblocks roaming throttling for given APN or for all
  APNs blocked due to roaming.
  
PARAMETERS
  1. APN name to be throttled
  2. Subscription Id
  3. Flag to indicate whether only the given APN should be unblocked or
     all APNs blocked due to roaming should be unblocked.
  4. Boolean Pointer set to True if the attach apn is unblocked
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.
===========================================================================*/
static void ds_3gpp_roaming_unblock_throttling
(
  byte*                                apn,
  sys_modem_as_id_e_type               subs_id,
  boolean                              unblock_all_apns,
  boolean*                             is_attach_apn_ptr
)
{
  uint8                                    apn_index = 0;
  ds_3gpp_pdn_throttle_sm_apn_based_dyn_s *apn_index_ptr = NULL;
  ds_3gpp_pdn_throttle_sm_apn_based_s     *apn_based_tbl_ptr;
  /*-------------------------------------------------------------------*/

  apn_based_tbl_ptr = ds_3gpp_pdn_throt_get_apn_based_tbl(subs_id);

  if (apn_based_tbl_ptr == NULL)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "apn_based_tbl_ptr is NULL");
    return;
  }

  if (is_attach_apn_ptr == NULL)
  {
    DS_3GPP_ERROR_LOG_0_EX(subs_id, "is_attach_apn_ptr is NULL !");
    return;
  }

  for (apn_index = 0; apn_index < DS_MAX_APN_NUM; apn_index++)
  {
    apn_index_ptr = apn_based_tbl_ptr->apn_based_dyn_tbl[apn_index];

    if (unblock_all_apns == FALSE)
    {
      if (apn != NULL)
      {
        if (strcasecmp((char*)apn_index_ptr->apn, (char*)apn) != 0)
        {
          continue;
        }
      }
      else
      {
        DS_3GPP_ERROR_LOG_0_EX(subs_id, "APN ptr is NULL !");
        return;
      }
    }

    if( (NULL != apn_index_ptr) && 
        (TRUE == apn_index_ptr->valid) && 
        (apn_index_ptr->global_throttle_mask &
           DS_3GPP_PDN_THROT_ROAMING_DISABLED_MASK ) )
    {
      DATA_3GPP_MSG_SPRINTF_1_EX(MSG_LEGACY_HIGH,subs_id,
                                 "Clearing Roaming Throttling"
                                 " for APN: %s", (char*)apn_index_ptr->apn);

      if (ds_dsd_apm_is_apn_in_attach_pdn_list(subs_id, (char*)apn_index_ptr->apn) &&
          (*is_attach_apn_ptr == FALSE))
      {
        *is_attach_apn_ptr = TRUE;
      }

      ds_3gpp_pdn_throt_reset_global_throt_mask
        (apn_index_ptr->apn, subs_id, DS_3GPP_PDN_THROT_ROAMING_DISABLED_MASK);

      ds_3gpp_pdn_throt_cleanup_apn_index_entry_if_needed
        (apn_index_ptr,FALSE,subs_id);
    }
  }
  return;
} /* ds_3gpp_pdn_throt_unblock_roaming_throttling */
