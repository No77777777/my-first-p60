#ifndef DS_WLAN_UTIL_H
#define DS_WLAN_UTIL_H
/*===========================================================================
                           DS_WLAN_UTIL
===========================================================================*/

/*!
  @file
  ds_wlan_util.h

  @brief
  This module implements wlan util functions

  @detail
*/

/*===========================================================================

  Copyright (c) 2014-2020 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/wlan/wlan_utils/inc/ds_wlan_util.h#4 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/02/19   vl      Added support for rat preference IMS_PREFERRED
04/08/19   my      Added changes for FR53445:Non ims apn to follow ims voice rat.
01/08/19    my     Added support for QSH MDUMP collection
10/24/18   ak      Fix to read NV value from correct sub.
06/08/18   js      Changed rel14 emergency param to unauth emergency
05/21/18   vls     Added changes for introducing time period before processing 
                   wifi_switch off
05/23/18   vl      Added changes for FR 47777: IWLAN refactoring for AndroidP.
04/12/18   js      FR46730: Added support for Rel.14 Emergency
03/21/18   vls     Added new NV for controlling ANDSF setting for non-ims PDN
                   based on WFC on/off
03/27/17   fn      Added support for dsda
06/15/16   fn      Added support for dsds capable subs
01/07/16   ss      Adding new NV for controlling whether RAT Preference 
                   setting is enabled or disabled
03/02/15   vl      Added support for WCDMA hysteresis and WLAN WCDMA timers(FR 20728)
07/28/14   op      Added support for processing WLAN Utils commands
05/29/14   fn      Added support to use new API for posting DS commands 
03/20/14   scb     Added support for MMGSDI SUBSCRIPTION TYPE
02/20/14   scb     Added support for MPPM hysteresis timers
01/28/14   vl      Created module
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/

#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA_WLAN_MAPCON
#include "ds3gmmgsdiif.h"
#include "ds3geventmgr.h"
#include "dstask_v.h"
#include "ps_sys.h"

#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif

/*===========================================================================
                             PUBLIC DATA DECLARATIONS
===========================================================================*/

/*---------------------------------------------------------------------------
 WLAN utils use to send generic pointer structure
---------------------------------------------------------------------------*/
typedef struct
{
  void *cmd_data_ptr;
} ds_wlan_cmd_type;

/*---------------------------------------------------------------------------
 MMGSDI_SUBSCRIPTION status structure.
---------------------------------------------------------------------------*/

typedef struct
{
  ds3geventmgr_event_type         mmgsdi_event;
  ps_sys_subscription_enum_type   dsd_subs_id;
} mmgsdi_subscription_status_type;



/*===========================================================================
                          VARIABLES AND MACRO DEFINITIONS
==========================================================================*/

/*-----------------------------------------------------------------------------
  PS_SYS_WLAN_MAX_SUBS to prevent unecessary handling of tertiary sub
-----------------------------------------------------------------------------*/
#ifdef FEATURE_TRIPLE_SIM 
  #define PS_SYS_WLAN_MAX_SUBS PS_SYS_SUBS_MAX
#elif defined(FEATURE_DUAL_SIM)
  #define PS_SYS_WLAN_MAX_SUBS PS_SYS_TERTIARY_SUBS
#else
  #define PS_SYS_WLAN_MAX_SUBS PS_SYS_SECONDARY_SUBS
#endif

/*-----------------------------------------------------------------------------
  SYS_MODEM_AS_WLAN_MAX_SUBS to prevent unecessary handling of tertiary sub
-----------------------------------------------------------------------------*/
#ifdef FEATURE_TRIPLE_SIM 
  #define SYS_MODEM_AS_WLAN_MAX_SUBS SYS_MODEM_AS_ID_3+1
#elif defined(FEATURE_DUAL_SIM)
  #define SYS_MODEM_AS_WLAN_MAX_SUBS SYS_MODEM_AS_ID_3
#else
  #define SYS_MODEM_AS_WLAN_MAX_SUBS SYS_MODEM_AS_ID_2
#endif

/*-----------------------------------------------------------------------------
  Enum to specify the various WLAN offload config types

  DISABLED     : WLAN offload disabled
  WLAN_LB_ONLY : WLAN local breakout enabled
  IWLAN_S2B    : IWLAN_S2B offload feature enabled
                (and inherently WLAN local breakout is enabled)
-----------------------------------------------------------------------------*/
typedef enum
{
  DS_WLAN_OFFLOAD_CONFIG_DISABLED     = 0,
  DS_WLAN_OFFLOAD_CONFIG_WLAN_LB_ONLY = 1,
  DS_WLAN_OFFLOAD_CONFIG_IWLAN_S2B    = 2,
  DS_WLAN_OFFLOAD_CONFIG_MAX,

  DS_WLAN_OFFLOAD_CONFIG_DEFAULT      = DS_WLAN_OFFLOAD_CONFIG_DISABLED
}ds_wlan_offload_config_enum_type;

/*--------------------------------------------------------------------------- 
 ENUM TO SPECIFY PLATFORM CONFIG OR HANDSET TYPE
---------------------------------------------------------------------------*/
typedef enum 
{
  PLATFORM_ANDROID = 0,
  PLATFORM_WP = 1,
  PLATFORM_MAX
} ds_wlan_platform_config_e_type;

/*-------------------------------------------------------------------------
    NV refresh event info
-------------------------------------------------------------------------*/
#ifdef FEATURE_MODEM_CONFIG_REFRESH

typedef struct ds_wlan_nv_refresh_event_info_type_s ds_wlan_nv_refresh_event_info_type;

struct ds_wlan_nv_refresh_event_info_type_s
{
  ds_wlan_offload_config_enum_type            prev_wlan_offload_config;
  ds_wlan_offload_config_enum_type            current_wlan_offload_config;
  ps_sys_subscription_enum_type               ps_subs_id;
};
#endif /*FEATURE_MODEM_CONFIG_REFRESH*/

/*-------------------------------------------------------------------------
  WLAN capable event info
-------------------------------------------------------------------------*/
typedef struct ds_wlan_capable_event_info_type_s ds_wlan_capable_event_info_type;

struct ds_wlan_capable_event_info_type_s
{
  ds_wlan_offload_config_enum_type            current_sub_offload_config;
  ps_sys_subscription_enum_type               ps_subs_id;
  boolean                                     wlan_capable;
};

#define DS_WLAN_DATA_CONF_BUF_SIZE ds_wlan_get_efs_item_file_path_size()

/*---------------------------------------------------------------------------
   Macro to check whether offload is NV is enabled
---------------------------------------------------------------------------*/
#define DS_WLAN_OFFLOAD_CONFIG_NV_ENABLED(wlan_offload_config_nv)          \
        ( (wlan_offload_config_nv == DS_WLAN_OFFLOAD_CONFIG_IWLAN_S2B ) || \
          (wlan_offload_config_nv == DS_WLAN_OFFLOAD_CONFIG_WLAN_LB_ONLY) )

/*===========================================================================
                       EXTERNAL FUNCTION DEFINITIONS
===========================================================================*/

/*===========================================================================
FUNCTION      ds_wlan_powerup_init

DESCRIPTION   Init function called on power-up.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_wlan_powerup_init
(
  void
);

/*===========================================================================
FUNCTION      ds_wlan_powerup_deinit

DESCRIPTION   De-Init function called during shutdown.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/*===========================================================================
FUNCTION      DW_WLAN_EFS_ITEM_FILE_PATH_SIZE

DESCRIPTION   Returns the total wlan EFS item file paths size

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
uint32 ds_wlan_get_efs_item_file_path_size
(
  void
);

/*===========================================================================
  FUNCTION DS_WLAN_UPDATE_DATA_CONFIG_INFO

  DESCRIPTION
    Puts all the required nv item file paths in the data_config_info.conf
    file. Would be called from dsutils during powerup.

  PARAMETERS
    file_paths_buf: To hold the efs item file paths

  DEPENDENCIES
    None.

  RETURN VALUE
    0 for success.
    -1 for failure.

  SIDE EFFECTS
    None.

===========================================================================*/
int32 ds_wlan_update_data_config_info
(
  char  *file_paths_buf
);

/*===========================================================================
  FUNCTION DS_WLAN_UTILS_UPDATE_CAPABLE_SUBS

  DESCRIPTION
    Updates the active subs for wlan to the wlan capable subs
 
  PARAMETERS
    subs_id: subs_id of the active subs

  DEPENDENCIES
    None.

  RETURN VALUE
    TRUE for success.
    FALSE for failure, capable subs did not change

  SIDE EFFECTS
    None.

===========================================================================*/
boolean ds_wlan_utils_update_capable_subs
(
  sys_modem_as_id_e_type                  subs_id,
  boolean                                 capable
);

/*===========================================================================
  FUNCTION DS_WLAN_UTILS_GET_CAPABLE_SUBS

  DESCRIPTION
    Gets the active/capable subs for wlan 
 
  PARAMETERS

  DEPENDENCIES
    None.

  RETURN VALUE
    cm subs id

  SIDE EFFECTS
    None.

===========================================================================*/
sys_modem_as_id_e_type ds_wlan_utils_get_capable_subs();

/*===========================================================================
FUNCTION ds_wlan_get_wifi_oos_linger_timer

DESCRIPTION
  This function gets the wifi unavailable timer value from EFS file

DEPENDENCIES
  The data config file must have been created

RETURN VALUE
  uint32 - timer value if Success else -1

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_wifi_oos_linger_timer
(
  void
);

/*===========================================================================
FUNCTION ds_wlan_get_wlan_proxy_bringup_retry_timer

DESCRIPTION
  This function returns timer value for wlan proxy bringup retry mechanism

DEPENDENCIES
  None

RETURN VALUE
  uint32 - timer value if Success else -1

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_wlan_proxy_bringup_retry_timer
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WLAN_OFFLOAD_CONFIG_NV

DESCRIPTION
  Returns the current value of ds_wlan_offload_config_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  ds_wlan_offload_config_enum_type

SIDE EFFECTS
  None
===========================================================================*/
ds_wlan_offload_config_enum_type ds_wlan_get_wlan_offload_config_nv
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WLAN_OFFLOAD_CONFIG_NV

DESCRIPTION
  Returns the current value of ds_wlan_offload_config_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  ds_wlan_offload_config_enum_type

SIDE EFFECTS
  None
===========================================================================*/
ds_wlan_offload_config_enum_type ds_wlan_get_wlan_offload_config_nv_per_subs
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_HYST_LTE_NULL_TIMER_PER_SUB

DESCRIPTION
  Returns the current value of ds_wlan_hyst_lte_null_timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_hyst_lte_null_timer

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_hyst_lte_null_timer_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_HYST_LTE_NULL_TIMER

DESCRIPTION
  Returns the current value of ds_wlan_hyst_lte_null_timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_hyst_lte_null_timer

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_hyst_lte_null_timer
(
  void
);
#ifdef FEATURE_DATA_IWLAN_S2B
/*===========================================================================
FUNCTION ds_wlan_get_iwlan_s2b_mtu_val

DESCRIPTION
  Returns the current value of ds_iwlan_s2b_mtu_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_iwlan_s2b_mtu_val

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_iwlan_s2b_mtu_val
(
  void
);
#endif /*FEATURE_DATA_IWLAN_S2B*/

/*===========================================================================
FUNCTION DS_WLAN_GET_HYST_EHRPD_NULL_TIMER_PER_SUB

DESCRIPTION
  Returns the current value of ds_wlan_hyst_ehrpd_null_timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_hyst_ehrpd_null_timer

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_hyst_ehrpd_null_timer_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_HYST_EHRPD_NULL_TIMER

DESCRIPTION
  Returns the current value of ds_wlan_hyst_ehrpd_null_timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_hyst_ehrpd_null_timer

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_hyst_ehrpd_null_timer
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_HYST_WCDMA_NULL_TIMER_PER_SUB

DESCRIPTION
  Returns the current value of ds_wlan_hyst_wcdma_null_timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_hyst_wcdma_null_timer

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_hyst_wcdma_null_timer_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_HYST_WCDMA_NULL_TIMER

DESCRIPTION
  Returns the current value of ds_wlan_hyst_wcdma_null_timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_hyst_wcdma_null_timer

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_hyst_wcdma_null_timer
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WWAN_THROTTLE_TIMER_IN_IWLAN_COVERAGE_PER_SUB

DESCRIPTION
  Returns the current value of ds_wlan_wwan_throttle_timer_in_iwlan_coverage.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_wwan_throttle_timer_in_iwlan_coverage

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_wwan_throttle_timer_in_iwlan_coverage_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WWAN_THROTTLE_TIMER_IN_IWLAN_COVERAGE

DESCRIPTION
  Returns the current value of wwan_throttle_timer_in_iwlan_coverage.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of wwan_throttle_timer_in_iwlan_coverage

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_wwan_throttle_timer_in_iwlan_coverage
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_IS_DATA_ROAMING_APPLICABLE_PER_SUB

DESCRIPTION
  Returns the current value of ds_wlan_is_data_roaming_applicable.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_is_data_roaming_applicable

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_is_data_roaming_applicable_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_IS_DATA_ROAMING_APPLICABLE

DESCRIPTION
  Returns the current value of ds_wlan_is_data_roaming_applicable.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_is_data_roaming_applicable

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_is_data_roaming_applicable
(
  void
);

/*===========================================================================
FUNCTION ds_wlan_get_is_emergency_epdg_support_enabled

DESCRIPTION
  Returns the value of emergency_epdg_support_enabled from IWLAN_S2B config

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE 
   TRUE - if emergency epdg support is enabled
   FALSE - if emergency epdg support is disabled

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_is_emergency_epdg_support_enabled
(
  sys_modem_as_id_e_type            cm_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_IS_UNATH_EMERGENCY_SUPPORT_ENABLED

DESCRIPTION
  Returns the value of unauth_emergency_support_enabled from IWLAN_S2B config

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE 
   TRUE - if Unauth emergency support is enabled
   FALSE - if Unauth emergency support is disabled

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_is_unauth_emergency_support_enabled
(
  sys_modem_as_id_e_type            cm_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_CONFIG_THROTTLE_WWAN_IN_IWLAN_COVERAGE_PER_SUB

DESCRIPTION
  Returns the current value of
  ds_wlan_throttle_wwan_in_iwlan_coverage.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of 
  ds_wlan_throttle_wwan_in_iwlan_coverage

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_config_throttle_wwan_in_iwlan_coverage_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_CONFIG_THROTTLE_WWAN_IN_IWLAN_COVERAGE

DESCRIPTION
  Returns the current value of
  throttle_wwan_in_iwlan_coverage.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of 
  throttle_wwan_in_iwlan_coverage

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_config_throttle_wwan_in_iwlan_coverage
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_T_MINONWWAN_FOR_IMS_ENABLED_PER_SUB

DESCRIPTION
  Returns the current value of ds_wlan_t_minonwwan_for_ims_enabled_per_sub.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_t_minonwwan_for_ims_enabled

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_t_minonwwan_for_ims_enabled_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_T_MINONWWAN_FOR_IMS_ENABLED

DESCRIPTION
  Returns the current value of ds_wlan_t_minonwwan_for_ims_enabled.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_t_minonwwan_for_ims_enabled

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_t_minonwwan_for_ims_enabled
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_IS_PLATFORM_CONFIG_WP

DESCRIPTION
  Returns the possible value 0- Linux, 1 - WP.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE possible value 0- Linux, 1 - WP.

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_is_platform_config_wp
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_HYST_LTE_ATTACH_WAIT_TIMER_PER_SUB

DESCRIPTION
  Returns the current value of ds_wlan_hyst_lte_attach_wait_timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_hyst_lte_attach_wait_timer
  

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_hyst_lte_attach_wait_timer_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_HYST_LTE_ATTACH_WAIT_TIMER

DESCRIPTION
  Returns the current value of ds_wlan_hyst_lte_attach_wait_timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_hyst_lte_attach_wait_timer
  

SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_hyst_lte_attach_wait_timer
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WLAN_THROTTLE_TIMER_PER_SUB

DESCRIPTION
  Returns the current value of ds_mppm_wlan_throttle_timer_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_mppm_wlan_throttle_timer_val
  

SIDE EFFECTS
  None
===========================================================================*/
rex_timer_cnt_type ds_wlan_get_wlan_throttle_timer_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WLAN_THROTTLE_TIMER

DESCRIPTION
  Returns the current value of ds_mppm_wlan_throttle_timer_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_mppm_wlan_throttle_timer_val
  

SIDE EFFECTS
  None
===========================================================================*/
rex_timer_cnt_type ds_wlan_get_wlan_throttle_timer
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WIFI_SWITCH_HYS_TIMER_PER_SUB

DESCRIPTION
  Returns the current value of ds_mppm_wifi_switch_hys_timer_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_mppm_wifi_switch_hys_timer_val
  

SIDE EFFECTS
  None
===========================================================================*/
rex_timer_cnt_type ds_wlan_get_wifi_switch_hys_timer_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WIFI_SWITCH_HYS_TIMER

DESCRIPTION
  Returns the current value of ds_mppm_wifi_switch_hys_timer_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_mppm_wifi_switch_hys_timer_val
  

SIDE EFFECTS
  None
===========================================================================*/
rex_timer_cnt_type ds_wlan_get_wifi_switch_hys_timer
(
  void
);

/*===========================================================================
FUNCTION DS_MPPM_WLAN_WCDMA_THROTTLE_TIMER_PER_SUB

DESCRIPTION
  Returns the current value of ds_mppm_wlan_wcdma_throttle_timer_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_mppm_wlan_wcdma_throttle_timer_val
  

SIDE EFFECTS
  None
===========================================================================*/
rex_timer_cnt_type ds_wlan_get_wlan_wcdma_throttle_timer_per_sub
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WLAN_WCDMA_THROTTLE_TIMER

DESCRIPTION
  Returns the current value of ds_mppm_wlan_wcdma_throttle_timer_val.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_mppm_wlan_wcdma_throttle_timer_val
  

SIDE EFFECTS
  None
===========================================================================*/
rex_timer_cnt_type ds_wlan_get_wlan_wcdma_throttle_timer
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_AP_ASSIST_INTENT_RESP_TIMER

DESCRIPTION

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE 
  

SIDE EFFECTS
  None
===========================================================================*/
rex_timer_cnt_type ds_wlan_get_ap_assist_intent_resp_timer
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WLAN_PROXY_SIO_CONFIG_TIMER

DESCRIPTION
  Returns the timer value for SIO configurations

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE


SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_wlan_proxy_sio_config_timer
(
  void
);

/*===========================================================================
FUNCTION DS_WLAN_GET_WLAN_PROXY_RM_CONFIG_TIMER

DESCRIPTION
  Returns the timer value for RM configurations

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE


SIDE EFFECTS
  None
===========================================================================*/
uint32 ds_wlan_get_wlan_proxy_rm_config_timer
(
  void
);

/*===========================================================================
FUNCTION ds_wlan_get_wlan_rat_pref_setting_is_disabled

DESCRIPTION
  Returns the current value of rat_pref_for_non_ims_pdn_is_disabled.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of rat_pref_for_non_ims_pdn_is_disabled

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_wlan_rat_pref_setting_is_disabled
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION ds_wlan_get_wfc_applicability_for_non_ims_pdn

DESCRIPTION
  Returns the current value of wfc_for_non_ims_pdn_is_applicable.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of wfc_for_non_ims_pdn_is_applicable

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_wfc_applicability_for_non_ims_pdn
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION ds_wlan_get_ap_assist_mode_enabled

DESCRIPTION
  Returns the current value of ds_wlan_ap_assist_mode_enabled.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_ap_assist_mode_enabled

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_ap_assist_mode_enabled
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION ds_wlan_get_is_3g_epc_rat

DESCRIPTION
  Returns the current value of ds_wlan_is_3g_epc_rat.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE value of ds_wlan_is_3g_epc_rat

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_get_is_3g_epc_rat
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION DS_WLAN_POST_CMD

DESCRIPTION
  A wrapper function for posting DS command. Posts a DS command and
  allocates buffer space for ds command.

DEPENDENCIES
  None

PARAMETERS
  cmd_id - id of the command to be posted 
  payload_data - pointer to a structure to be send along with the DS cmd
  size_t payload_size - size of the payload_data to be sent
  module_type - module type, WLAN util supports MPPM, iWLAN S2B, and WLAN
  queue_type - destination cmd queue, by default it will use default queue

RETURN VALUE 
  FALSE - Fail to post DS command 
  TRUE -  Successfully posted DS command
  

SIDE EFFECTS
  None
===========================================================================*/
boolean ds_wlan_post_cmd
(
  ds_cmd_enum_type cmd_id,
  void *payload_data,
  size_t payload_size
);

/*===========================================================================
FUNCTION      ds_wlan_process_cmd

DESCRIPTION   Handle commands specific to WLAN Utils

DEPENDENCIES  None.

RETURN VALUE  None

SIDE EFFECTS  None 
===========================================================================*/
void ds_wlan_process_cmd
(
  ds_cmd_type     *cmd_ptr
);

/*===========================================================================
FUNCTION      ds_wlan_read_efs_nv_items

DESCRIPTION   Called to read WLAN related EFS NV items.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_wlan_read_efs_nv_items
(
  void
);

/*===========================================================================
FUNCTION      ds_wlan_read_efs_nv_items_per_subs

DESCRIPTION   Called to read WLAN related EFS NV items.

PARAMETERS    None

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ds_wlan_read_efs_nv_items_per_subs
(
  sys_modem_as_id_e_type                  subs_id
);

#ifdef FEATURE_QSH_MDUMP

/*===========================================================================

FUNCTION ds_wlan_utils_qsh_cb

DESCRIPTION
  Processes the QSH request .
  Currently we handle only QSH_ACTION_MDUMP.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
QSH_MDUMP_FN_ATTR 
void  ds_wlan_utils_qsh_cb();

/*===========================================================================

FUNCTION ds_wlan_qsh_mdump_collect

DESCRIPTION
  Processes the QSH request .
  Collects mdmup for all module
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
  
===========================================================================*/
QSH_MDUMP_FN_ATTR 
void  ds_wlan_qsh_mdump_collect();

/*===========================================================================
FUNCTION ds_wlan_util_qsh_mdump_collect
  
DESCRIPTION
  This function collects the mdump for active_wlan_subs_id ds_wlan_is_data_roaming_applicable 
  ds_wlan_offload_config_nv static variables

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR 
void ds_wlan_util_qsh_mdump_collect();


#endif /* FEATURE_QSH_MDUMP */

/*===========================================================================
FUNCTION ds_wlan_get_ims_voice_rat_setting_enable_for_non_ims_pdn

DESCRIPTION
  Returns the current value of non ims apns follow ims vioce rat NV.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE TRUE/FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean 
ds_wlan_get_ims_voice_rat_setting_enable_for_non_ims_pdn
(
  ps_sys_subscription_enum_type ps_subs_id
);

/*===========================================================================
FUNCTION ds_wlan_get_cellular_rat_pref_is_ims_rat_pref

DESCRIPTION
  Returns the current value of CELLULAR_RAT_PREF_IS_IMS_RAT_PREF NV.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE TRUE/FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean 
ds_wlan_get_cellular_rat_pref_is_ims_rat_pref
(
  ps_sys_subscription_enum_type ps_subs_id
);

#endif /* FEATURE_DATA_WLAN_MAPCON */
#endif /* DS_WLAN_UTIL_H */
