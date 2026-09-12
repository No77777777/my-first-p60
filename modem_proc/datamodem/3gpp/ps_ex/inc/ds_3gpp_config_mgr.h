#ifndef DS_3GPP_CONFIG_MGR_H
#define DS_3GPP_CONFIG_MGR_H
/*===========================================================================

 3 G   D A T A   S E R V I C E S   C O N F I G U R A T I O N   M A N A G E R

                           H E A D E R   F I L E

GENERAL DESCRIPTION
  This software unit contains functions for configurations relating to
  3G Data Services co.
  It provides APIs for external clients to query and update data 3GPP
  configurations.
 
  Copyright (c) 2001-2019 by Qualcomm Technologies Incorporated.
  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $PVCSPath:
  $Header:

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/20/17   rs      Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "datamodem_variation.h"
#include "comdef.h"
#include "ds3gsubsmgr.h"
#include "sys_v.h"
#include "mcfg_fs.h"
#include "ps_iface_defs.h"
#include "list.h"
#include "ps_sys.h"

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

=============================================================================*/
/*---------------------------------------------------------------------------- 
  Maximum number of Home PLMNs that can be configured in the EFS file
-----------------------------------------------------------------------------*/
#define DS_3GPP_ROAMING_MAX_SUPPORTED_HOME_PLMNS 8
#define DS_3GPP_MAX_THROT_RATS 5
#define DS_3GPP_MAX_UI_RATS    12

#define DS_3GPP_CONFIG_HOME_PLMN_LIST_TAG          "HPLMN_list"
#define DS_3GPP_CONFIG_EXCEPTION_HPLMN_LIST_TAG    "Exception_HPLMN_list"
#define DS_3GPP_CONFIG_DOMESTIC_ROAM_PLMN_LIST_TAG "Domestic_Roaming_PLMN_list"
#define DS_3GPP_CONFIG_INTL_ROAM_PLMN_LIST_TAG     "International_Roaming_PLMN_list"

typedef enum
{
  DS_3GPP_UI_COND_INVALID = -1,
  DS_3GPP_UI_COND_AND = 0,
  DS_3GPP_UI_COND_OR,
  DS_3GPP_UI_COND_MAX
} ds3g_config_ui_cond_e_type;

typedef enum
{
  DS_3GPP_UI_RAT_MIN = -1,
  DS_3GPP_UI_RAT_GPRS = 0, 
  DS_3GPP_UI_RAT_EDGE,
  DS_3GPP_UI_RAT_HSPA,
  DS_3GPP_UI_RAT_HSPA_PLUS,
  DS_3GPP_UI_RAT_UMTS,
  DS_3GPP_UI_RAT_LTE,
  DS_3GPP_UI_RAT_LTE_PLUS,
  DS_3GPP_UI_RAT_FOUR_G,
  DS_3GPP_UI_RAT_FOUR_G_PLUS,
  DS_3GPP_UI_RAT_FOUR_POINT_FIVE_G,
  DS_3GPP_UI_RAT_FOUR_POINT_FIVE_G_PLUS,
  DS_3GPP_UI_RAT_ILLEGAL,
  DS_3GPP_UI_RAT_MAX = DS_3GPP_UI_RAT_ILLEGAL
} ds_3gpp_config_ui_rat_e_type;

typedef struct 
{
  list_link_type  link;
  ds3g_config_ui_cond_e_type condition_type;
  boolean         dl_256qam;
  boolean         dl_256qam_present;
  boolean         ul_64qam;
  boolean         ul_64qam_present;
  boolean         mimo_4_layers;
  boolean         mimo_4_layers_present;
  boolean         ca;
  boolean         ca_present;
  boolean         connect_mode;
  boolean         connect_mode_present;
}ds_3gpp_config_ui_conditions_type;

typedef struct
{
  boolean                ca_dl_available;
  boolean                ca_ul_available;
  uint8                  num_configured_scells;
  boolean                dl_256_qam;
  boolean                ul_64_qam;
  boolean                mimo_4_layers;
  boolean                connect_mode;  
}ds_3gpp_config_ca_mimo_info_type;


/*---------------------------------------------------------------------------- 
 This enum defines all the different types of PLMN lists
-----------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_CONFIG_HPLMN = 0,
  DS_3GPP_CONFIG_EPLMN,
  DS_3GPP_CONFIG_DOMESTIC_ROAM_PLMN,
  DS_3GPP_CONFIG_INTERNATIONAL_ROAM_PLMN
}ds_3gpp_config_mgr_plmn_type_info;

/*----------------------------------------------------------------------------
  This structure stores the PS Data Off exempt services configuration.
-----------------------------------------------------------------------------*/
typedef struct
{
  ps_sys_ps_data_avail_list_type_enum_type list_type;
  /** List type*/
  ps_sys_ps_data_avail_info_type           service_info[PS_SYS_APPL_SERVICE_NAME_MAX];
  /** Array of Service and type*/
}ds_3gpp_config_mgr_psdataoff_cfg_type;

/*---------------------------------------------------------------------------- 
 This enum defines all the reset conditions currently supported for IP fallback
-----------------------------------------------------------------------------*/
typedef enum
{
  DS_3GPP_CONFIG_RESET_ON_LPM_MODE = 0,
  DS_3GPP_CONFIG_RESET_ON_USIM_REMOVAL,
  DS_3GPP_CONFIG_RESET_ON_PLMN_CHANGE,  
  DS_3GPP_CONFIG_RESET_ON_DETACH,
  DS_3GPP_CONFIG_RESET_ON_PWR_OFF,  
  DS_3GPP_INVALID_CONDITION
}reset_condition_enum_type;

/*---------------------------------------------------------------------------- 
  Structure which incorporates a single MCC/MNC info entry
-----------------------------------------------------------------------------*/
typedef struct
{
  uint8  mcc[3];
  uint8  mnc[3];
}ds_3gpp_roaming_mcc_mnc_info_s;

/*---------------------------------------------------------------------------- 
  Structure which represents the home PLMN List Information read from the EFS
-----------------------------------------------------------------------------*/
typedef struct
{
  uint8                          num_valid_entries;
                                        /* Number of valid entries in the EFS*/
  ds_3gpp_roaming_mcc_mnc_info_s mcc_mnc_info
                                   [DS_3GPP_ROAMING_MAX_SUPPORTED_HOME_PLMNS];
                                       /* Home PLMN information stored in
                                          MCC-MNC format                     */
} ds_3gpp_roaming_efs_info_s;
/*---------------------------------------------------------------------------- 
  This structure defines an item in the Home PLMN List
-----------------------------------------------------------------------------*/
typedef struct
{
  list_link_type     link;     /* Pointer to the next item in list */
  sys_plmn_id_s_type plmn_id;  /* Home PLMN Id*/
} ds_3gpp_config_mgr_plmn_list_item_type;

/*---------------------------------------------------------------------------- 
  This structure defines an item in the Home PLMN List
-----------------------------------------------------------------------------*/
typedef struct
{
  ps_sys_3gpp_plmn_list_type    *hplmn_list_ptr;
  sys_modem_as_id_e_type         subs_id;
}ds_3gpp_config_mgr_plmn_list_info_type;

/*---------------------------------------------------------------------------- 
  This structure defines all the information related to throttling timer value
-----------------------------------------------------------------------------*/
typedef struct
{
  list_link_type      link;
  uint64              timer_value;
}ds_3gpp_config_mgr_timer_value_item_type;

/*---------------------------------------------------------------------------- 
  This structure defines all the information related to PS net down cause codes
-----------------------------------------------------------------------------*/
typedef struct
{
  list_link_type                 link;
  ps_iface_net_down_reason_type  ps_down_reason;
}ds_3gpp_config_mgr_ps_down_item_type;

/*---------------------------------------------------------------------------- 
  This structure defines all the information related to reset condition for
  fallback logic
-----------------------------------------------------------------------------*/
typedef struct
{
  list_link_type                 link;
  reset_condition_enum_type      reset_value;
}ds_3gpp_config_mgr_reset_cond_item_type;

/*---------------------------------------------------------------------------- 
  This structure stores the cause codes and the corresponding timer values
  ----------------------------------------------------------------------------*/
typedef struct 
{
  list_link_type  link;
  list_type       ps_net_down_reason;
  list_type       timer_value;
  list_type       reset_condition;
  boolean         per_plmn;
}ds_3gpp_config_mgr_pdn_throt_type;

/*---------------------------------------------------------------------------- 
  This structure defines all the information related to slice throttling
  configuration.
-----------------------------------------------------------------------------*/
typedef struct
{
  list_link_type                          link;  
  char                                    apn[DS_UMTS_MAX_APN_STRING_LEN+1];
  list_type                               pdn_throt_config;
} ds_3gpp_slice_throt_list_item_type;


/*---------------------------------------------------------------------------- 
  This structure defines all the information related to IPV4 fallback
  configuration.
-----------------------------------------------------------------------------*/
typedef struct
{
  list_link_type                          link;  
  boolean                                 ip_fallback_config_present;
  byte                                    apn[DS_UMTS_MAX_APN_STRING_LEN+1];
  list_type                               ps_net_down_reason;  
  list_type                               reset_condition;
} ds_3gpp_ipv4_fallback_list_item_type;

/*---------------------------------------------------------------------------- 
  This structure defines all the information related to roaming plmn list
-----------------------------------------------------------------------------*/
typedef struct
{
  list_type  home_plmn_list;  /* List of Home PLMNs */
  list_type  excp_home_plmn_list; /* List of exception Home PLMNs,
                                     higher priority than Home PLMN list*/
  list_type  domestic_roam_plmn_list;
  list_type  international_roam_plmn_list;/* List of International Roam PLMNs*/
} ds_3gpp_config_mgr_plmn_config_type;

/*------------------------------------------------------------------------
  Queue block inserted in hplmn list cache 
------------------------------------------------------------------------*/
typedef struct
{
  q_link_type               link;
  ps_sys_3gpp_mcc_mnc_type  hplmn_list;
}ds_3gpp_config_mgr_hplmn_list_cache_block_type;

/*---------------------------------------------------------------------------- 
  This structure defines all the information related to PS data off
  ----------------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_config_mgr_psdataoff_cfg_type  global_psdataoff_list; /* Release 14+ */
} ds_3gpp_config_mgr_ps_data_off_type;

/*---------------------------------------------------------------------------
  This structure defines all the information specific to APNs
---------------------------------------------------------------------------*/
typedef struct
{
  char         apn_name[PS_SYS_MAX_APN_LEN];
  char         overriden_apn[PS_SYS_MAX_APN_LEN];
  boolean      apn_override_present; 
  list_type    per_rat_pdn_throt[DS_3GPP_MAX_THROT_RATS];
  boolean      valid;
}ds_3gpp_config_mgr_per_apn_config_type;

/*---------------------------------------------------------------------------- 
  This structure defines generic config structure for storing the throttling config
  ----------------------------------------------------------------------------*/
typedef struct
{
  ds_3gpp_ipv4_fallback_list_item_type  ipv4_fallback;
  ds_3gpp_config_mgr_plmn_config_type   plmn_config;     
  /* HPLMN, EHPLMN, Domestic PLMN list, INTERNATIONAL PLMN List */
  ds_3gpp_config_mgr_ps_data_off_type   ps_data_off_config;
  list_type                             per_rat_ui_config[DS_3GPP_MAX_UI_RATS];
  list_type                             per_rat_pdn_throt[DS_3GPP_MAX_THROT_RATS];
  list_type                             default_pdn_throt_config;
  list_type                             slice_throttling_config;
}ds_3gpp_config_mgr_generic_config_type;
/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_POWERUP_INIT_PER_SUBS_ID

DESCRIPTION 
  This function performs initialization of the data 3gpp configuration
  module per subscription

PARAMETERS 
  subs_id  -- Subscription ID
    
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None

===========================================================================*/
void ds_3gpp_config_mgr_powerup_init_per_subs_id
(
  sys_modem_as_id_e_type                subs_id 
);

/*===========================================================================
FUNCTION      DS_3GPP_CFG_MGR_IS_DOWN_REASON_CONFIGURED_IN_FALLBACK_DB

DESCRIPTION 
  This function checks whether the given CC passed to this function is
  present in the 3gpp config db.

PARAMETERS 
  cause_code  -- ps net down reason 
  subs_idex   -- Subscription Index
   
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None

===========================================================================*/
boolean ds_3gpp_cfg_mgr_is_down_reason_configured_in_fallback_db
(
  ps_iface_net_down_reason_type         cause_code,
  sys_modem_as_id_e_type                subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_CFG_MGR_IS_RESET_COND_CONFIGURED_IN_FALLBACK_CFG_DB

DESCRIPTION 
  This function checks whether the given reset enum passed to this function is
  present in the 3gpp config db.

PARAMETERS 
  reset_enum_value  -- reset value
  subs_idex         -- Subscription Index
   
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None

===========================================================================*/
boolean ds_3gpp_cfg_mgr_is_reset_cond_configured_in_fallback_cfg_db
(
  reset_condition_enum_type         reset_enum_value,
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_IP_FALLBACK_EFS_CONFIGURED

DESCRIPTION 
  This utility function checks the internal cache to see if
  data_3gpp_config.xml file was configured in EFS or not.

PARAMETERS 
  subs_id           -- Subscription Index
  
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE    -- if cause code is present in the list
  FALSE   -- otherwise
   
SIDE EFFECTS 
  None

===========================================================================*/
boolean ds_3gpp_config_mgr_ip_fallback_efs_configured
(
  sys_modem_as_id_e_type            subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_POWERUP_INIT

DESCRIPTION 
  This function performs initialization of the data 3gpp configuration
  module. Should have a powerup_init not per sub so it would
  fit the single sub case. This would be called during
  the module bootup

PARAMETERS 
  subs_id  -- Subscription ID
    
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None

===========================================================================*/
void ds_3gpp_config_mgr_powerup_init
(
  void
);

/*===========================================================================
  FUNCTION DS_3GPP_CONFIG_MGR_CONVERT_TO_PLMN_FORMAT

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
sys_plmn_id_s_type ds_3gpp_config_mgr_convert_to_plmn_format
(
  ds_3gpp_roaming_mcc_mnc_info_s mcc_mnc_info
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_GET_HOME_PLMN_LIST_SIZE

  DESCRIPTION
  This function gets the size of the Home PLMN List
  
  PARAMETERS
  Subscription Id
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Number of Home PLMNs configured
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
uint8 ds_3gpp_config_mgr_get_home_plmn_list_size
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_GET_EXCP_HOME_PLMN_LIST_SIZE

  DESCRIPTION
  This function gets the size of the exception Home PLMN List
  
  PARAMETERS
  Subscription Id
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Number of Home PLMNs configured
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
uint8 ds_3gpp_config_mgr_get_excp_home_plmn_list_size
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_GET_DOMESTIC_ROAM_PLMN_LIST_SIZE

  DESCRIPTION
  This function gets the size of the domestic roaming PLMN List
  
  PARAMETERS
  Subscription Id
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Number of Home PLMNs configured
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
uint8 ds_3gpp_config_mgr_get_domestic_roam_plmn_list_size
(
  sys_modem_as_id_e_type subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_ROAMING_HDLR_CLEAR_PER_SUBS_ID

  DESCRIPTION
  This function performs resetting of the roaming module per subscription
  
  PARAMETERS  
  None. 
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/

void ds_3gpp_cfg_mgr_hdlr_clear_per_subs_id
(
  sys_modem_as_id_e_type     subs_id 
);

/*===========================================================================
  FUNCTION DS_3GPP_CONFIG_MGR_GET_PLMN_LIST

  DESCRIPTION
  This function retrieve the certain plmn list from config db
  
  PARAMETERS  
  plmn_type: The type of the plmn list required
  subs_id: The subscription for which to start the timer
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Pointer to the plmn list
  NULL if the sub id is invalid or the type is not defined
   
  SIDE EFFECTS 
  None.
===========================================================================*/ 
list_type *ds_3gpp_config_mgr_get_single_plmn_list
(
  ds_3gpp_config_mgr_plmn_type_info          plmn_type,
  sys_modem_as_id_e_type                     subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_CONFIG_MGR_ROAM_HDLR_CLEAR_PER_SUBS_ID

  DESCRIPTION
  This function performs resetting of the roaming module per subscription
  
  PARAMETERS  
  None. 
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_config_mgr_roam_hdlr_clear_per_subs_id
(
  sys_modem_as_id_e_type     subs_id 
);

/*===========================================================================
  FUNCTION DS_3GPP_CONFIG_MGR_ROAM_HDLR_POWERUP_INIT_PER_SUBS_ID

  DESCRIPTION
  This function performs initialization of the roaming module per subscription
  
  PARAMETERS  
  None. 
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/

boolean ds_3gpp_config_mgr_roam_list_init_per_subs_id
(
  sys_modem_as_id_e_type     subs_id 
);

/*===========================================================================
  FUNCTION DS_3GPP_CONFIG_MGR_QMI_WRITE_TO_EFS

  DESCRIPTION This function writes configuration info to EFS file.
  
  PARAMETERS  sys_modem_as_id_e_type        subs_id
              void                         *new_info
              char                         *tag_change_config[]
              int                           tag_change_config_size
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Boolean.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
boolean ds_3gpp_config_mgr_qmi_write_to_efs
(
  sys_modem_as_id_e_type        subs_id,
  void                         *new_config,
  char                         *tag_change_config[],
  int                           tag_change_config_size
);

/*===========================================================================
  FUNCTION DS_3GPP_CONFIG_MGR_READ_HPLMN_NV_PER_SUBS_ID

  DESCRIPTION
  This function read the hplmn from NV72561
  
  PARAMETERS  
  None. 
    
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  None.
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
void ds_3gpp_config_mgr_read_hplmn_nv_per_subs_id
(
  sys_modem_as_id_e_type     subs_id
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
boolean ds_3gpp_config_mgr_allocate_and_start_wait_timer
(
  ds_umts_pdp_profile_type   *profile_info_ptr,
  sys_modem_as_id_e_type      subs_id
);

/*===========================================================================
FUNCTION       DS_3GPP_CONFIG_MGR_SET_HPLMN_LIST

DESCRIPTION    This utility function is invoked via PS_SYS IOCTL when AP
               invokes the QMI API to set hplmn list

PARAMETERS     arg_val_ptr: hplmn list info provided by AP
               subs_id: Active PS subs
               ps_errno: Error code if any to be returned to PS

DEPENDENCIES   NONE

RETURN VALUE   0: If Operation was successful
               -1: If for some reason the input params are invalid

SIDE EFFECTS   NONE
===========================================================================*/
int ds_3gpp_config_mgr_set_plmn_list
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
FUNCTION       DS_3GPP_CONFIG_MGR_GET_HPLMN_LIST

DESCRIPTION    This utility function is invoked via PS_SYS IOCTL when AP
               invokes the QMI API to get hplmn list

PARAMETERS     arg_val_ptr: hplmn list info
               subs_id: Active PS subs
               ps_errno: Error code if any to be returned to PS

DEPENDENCIES   NONE

RETURN VALUE   0: If Operation was successful
               -1: If for some reason the input params are invalid

SIDE EFFECTS   NONE
===========================================================================*/
int ds_3gpp_config_mgr_get_plmn_list
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno
);

/*===========================================================================
  FUNCTION DS_3GPP_CONFIG_MGR_GET_PSDATAOFF_DEFAULT_CFG

  DESCRIPTION
  This function retrieves the PS Data Off default configuration.
  
  PARAMETERS  
  [in] list_type: global, roam or home
  [in] subs_id: subscription
  [out] Pointer to the PS Data Off default config as read from EFS (or if
    reading as failed, it returns default values according to spec)
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  TRUE if able to successfully retrieve the default PS Data Off config
  FALSE otherwise
   
  SIDE EFFECTS 
  None.
===========================================================================*/ 
boolean ds_3gpp_config_mgr_get_psdataoff_default_cfg
(
  ps_sys_ps_data_avail_list_type_enum_type          psdataoff_list_type,
  sys_modem_as_id_e_type                            subs_id,
  const ds_3gpp_config_mgr_psdataoff_cfg_type     **psdataoff_def_cfg_ptr
);
/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_GET_PDN_THROT_PTR

DESCRIPTION 
  This function get PDN throttle configuration by APN name.

PARAMETERS 
  byte*           apn_name
  subs_id   -- Subscription ID
    
DEPENDENCIES 
  None
  
RETURN VALUE 
  pointer of ds_3gpp_config_mgr_per_apn_config_type 
   
SIDE EFFECTS 
  None
 
===========================================================================*/
ds_3gpp_config_mgr_per_apn_config_type* 
ds_3gpp_config_mgr_get_per_apn_pdn_throt_ptr
(
  byte                    *apn_name,
  sys_modem_as_id_e_type   subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_GET_GENERIC_PDN_THROT_PTR

DESCRIPTION 
  This function get generic PDN throttle configuration 

PARAMETERS 
  subs_id   -- Subscription ID
    
DEPENDENCIES 
  None
  
RETURN VALUE 
  pointer of ds_3gpp_config_mgr_generic_config_type 
   
SIDE EFFECTS 
  None
 
===========================================================================*/
ds_3gpp_config_mgr_generic_config_type* 
ds_3gpp_config_mgr_get_generic_pdn_throt_ptr
(
  sys_modem_as_id_e_type   subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_GET_PER_APN_PDN_THROT_PTR

DESCRIPTION 
  This function get PDN throttle configuration by APN name.

PARAMETERS 
  byte*           apn_name
  subs_id   -- Subscription ID
    
DEPENDENCIES 
  None
  
RETURN VALUE 
  pointer of ds_3gpp_config_mgr_per_apn_config_type 
   
SIDE EFFECTS 
  None
 
===========================================================================*/
ds_3gpp_config_mgr_per_apn_config_type* 
ds_3gpp_config_mgr_get_per_apn_pdn_throt_ptr
(
  byte                    *apn_name,
  sys_modem_as_id_e_type   subs_id
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_GET_GENERIC_PDN_THROT_PTR

DESCRIPTION 
  This function get generic PDN throttle configuration 

PARAMETERS 
  subs_id   -- Subscription ID
    
DEPENDENCIES 
  None
  
RETURN VALUE 
  pointer of ds_3gpp_config_mgr_generic_config_type 
   
SIDE EFFECTS 
  None
 
===========================================================================*/
ds_3gpp_config_mgr_generic_config_type* 
ds_3gpp_config_mgr_get_generic_pdn_throt_ptr
(
  sys_modem_as_id_e_type   subs_id
);
/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_GET_SO_MASK_FROM_UI_CFG

DESCRIPTION   This function returns all so masks which are satisfied
              as per the aux info ptr status and ui config set in xml

DEPENDENCIES  

RETURN VALUE  uint32 so mask

SIDE EFFECTS  
===========================================================================*/
uint32 ds_3gpp_config_get_so_mask_from_ui_cfg
(
  ds3gsubsmgr_subs_id_e_type        subs_index,
  ds_3gpp_config_ca_mimo_info_type    *lte_info_ptr
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_IS_UI_CONFIG_PRESENT

DESCRIPTION   This function indicates whether UI config is present or not
        
PARAMETERS

DEPENDENCIES  None
        
RETURN VALUE  boolean
   
SIDE EFFECTS  None
===========================================================================*/
boolean ds_3gpp_config_is_ui_config_present
(
  ds3gsubsmgr_subs_id_e_type  subs_indx
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_INIT_DATA_3GPP_CFG_PER_SUB

DESCRIPTION 
  This function takes care of freeing memory associated with data 3GPP config
  DB

PARAMETERS 
  subs_id     -- Subscription Index
  
DEPENDENCIES 
  None
  
RETURN VALUE 
  None
   
SIDE EFFECTS 
  None
===========================================================================*/
void ds_3gpp_config_mgr_init_data_3gpp_cfg_per_sub
(
  sys_modem_as_id_e_type     subs_id 
);

/*===========================================================================
FUNCTION      DS_3GPP_CONFIG_MGR_GET_BASE_APN

DESCRIPTION 
  This function checks gets the base APN corresponding to the overriden APN
  if configured in XML

PARAMETERS    
  overriden_apn  -- Pointer to the profile configured overriden APN
  apn            -- Pointer to store the base APN name
  subs_id        -- Subscription ID
 
DEPENDENCIES 
  None
  
RETURN VALUE 
  TRUE   -- If APN override is configured
  FALSE  -- Otherwise
   
SIDE EFFECTS 
  None

===========================================================================*/ 
boolean ds_3gpp_config_mgr_get_base_apn
(
  byte                               *overriden_apn,
  byte                              **apn,
  sys_modem_as_id_e_type              subs_id
);

/*===========================================================================
  FUNCTION DS_3GPP_CONFIG_MGR_GET_INTERNATIONAL_ROAM_PLMN_LIST_SIZE

  DESCRIPTION
  This function gets the size of the International roaming PLMN List
  
  PARAMETERS
  Subscription Id
  
  DEPENDENCIES 
  None.
  
  RETURN VALUE 
  Number of PLMNs configured in the list
   
  SIDE EFFECTS 
  None.
  
===========================================================================*/
uint8 ds_3gpp_config_mgr_get_international_roam_plmn_list_size
(
  sys_modem_as_id_e_type subs_id
);

#endif /* DS_3GPP_CONFIG_MGR_H */
