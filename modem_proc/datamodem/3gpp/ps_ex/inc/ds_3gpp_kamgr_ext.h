#ifndef DS_3GPP_KAMGR_EXT_H
#define DS_3GPP_KAMGR_EXT_H
/*===========================================================================

  Copyright (c) 2009-2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "datamodem_variation.h"
#include "sys.h"
#include "msg.h"
#include "event.h"
#include "queue.h"

#include "ps_sys_conf.h"
#include "ps_sys_event.h"

#include "dsutil.h"
#include "ds3gmgr.h"
#ifdef FEATURE_QSH_MDUMP
#include "qsh.h"
#endif


/*------------------------------------------------------------------------ 
Define the RAT mask NULL
------------------------------------------------------------------------*/
#define DS_3GPP_KAMGR_RAT_NULL             0x0000

/*===========================================================================

                          PUBLIC FUNCTION DEFINATIONS

===========================================================================*/

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION ds_3gpp_kamgr_qsh_mdump_collect
  
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
void ds_3gpp_kamgr_qsh_mdump_collect
(
  void
);
#endif


/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_INIT

DESCRIPTION
  Initialization of all the required structures and variable for Keep alive
  manager
  
PARAMETERS
 None
 
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_kamgr_init (void);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_READ_EFS_INFO

DESCRIPTION
  This function is responsible for reading the EFS file and WLAN offload config
  NV and deciding whether Keep alive manager should be enabled or not
  
PARAMETERS 
  None

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_kamgr_read_efs_info
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION       DS_3GPP_KAMGR_PROCESS_CMD

DESCRIPTION    Process DS cmds 
 
PARAMETERS     cmd_ptr :              Command information pointer 

DEPENDENCIES   NONE

RETURN VALUE   NONE

SIDE EFFECTS   NONE
===========================================================================*/
void ds_3gpp_kamgr_process_cmd
(
  const ds_cmd_type                    *cmd_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_CHECK_FOR_PDN_TEARDOWN

DESCRIPTION
  This function validates DATA UI/Data Roaming Setting along with WLAN
  availability and In use flag of iface to decide whether KAMGR's PDN
  can be torn down or not
  
PARAMETERS 
  subs_id: Currently active Default data subs

DEPENDENCIES
  None

RETURN VALUE
  TRUE: If PDN is eligible for teardown
  FALSE: If PDN doesn't need to be torn down

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_kamgr_check_for_pdn_teardown
(
  sys_modem_as_id_e_type                 subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_KAMGR_PROCESS_APN_NAME_CHG

DESCRIPTION
  This function is responsible for update the internal structure with the
  updated APN name
  
PARAMETERS 
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_kamgr_process_apn_name_chg
(
  uint16                                profile_id,
  sys_modem_as_id_e_type                subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_ROAMING_STATUS_CHG_CBACK

DESCRIPTION
  This function is called from roaming handler when roaming status changes
  
PARAMETERS 
  subs_id: Current Active PS subs

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_kamgr_handle_roaming_status_chg
(
  sys_modem_as_id_e_type                  subs_id
);

/*==============================================================================
FUNCTION  DS_3GPP_KAMGR_DEALLOC_KAMGR_INSTANCE

DESCRIPTION
  This function is called to clean-up KAMGR global instance on an NV refresh
  or during KAMGR resource clean-up
  
PARAMETERS 
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

================================================================================*/
void ds_3gpp_kamgr_dealloc_kamgr_instance
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_QMI_BASED_BRING_UP_IND

DESCRIPTION
  This function posts a CMD to DS task to process QMI request for KAMGR
  bring-up
  
PARAMETERS 
  arg_val_ptr: Void pointer containing profile to use for KAMGR bring-up 
  subs_id: Current active PS subs
  ps_errno: PS error type to be returned on fuction termination
 
DEPENDENCIES
  None.

RETURN VALUE
  (Int) 0: successfully posted a cmd
       -1: validation failures

SIDE EFFECTS
  None

===========================================================================*/
int ds_3gpp_kamgr_qmi_based_bring_up_ind
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno  
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_QMI_BASED_TEAR_DOWN_IND

DESCRIPTION
  This function posts a CMD to DS task to process QMI request for KAMGR
  tear-down
  
PARAMETERS
  subs_id: Current active PS subs
  ps_errno: PS error type to be returned on fuction termination
 
DEPENDENCIES
  None.

RETURN VALUE
  (Int) 0: successfully posted a cmd
       -1: validation failures

SIDE EFFECTS
  None

===========================================================================*/
int ds_3gpp_kamgr_qmi_based_tear_down_ind
(
  void                    *arg_val_ptr,
  sys_modem_as_id_e_type   subs_id,
  int16                   *ps_errno  
);

/*===========================================================================
FUNCTION DS_3GPP_KAMGR_SET_WLAN_STATUS

DESCRIPTION
  This function process the system info given by MPPM to decide whether
  WLAN is available or not

PARAMETERS
  subs_indx:    Subscription Id
  sys_info_ptr:                    System information reported by MPPM

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_kamgr_set_wlan_status
(
  ds3gsubsmgr_subs_id_e_type                subs_indx,
  ps_sys_system_status_ex_type             *sys_info_ptr
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_GET_QMI_BRING_UP_IND

DESCRIPTION
  This function is to return QMI Bring Up indication value
  
PARAMETERS 
  subs_id: subscription id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE -- If QMI Bring Up indication is TRUE
  FALSE -- If QMI Bring Up indication is FALSE
 
SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_kamgr_get_qmi_bring_up_ind
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_WLAN_IS_AVAILABLE

DESCRIPTION
  This function is to return wlan status
  
PARAMETERS 
  subs_id: subscription id

DEPENDENCIES
  None.

RETURN VALUE
  TRUE -- If wlan is available 
  FALSE -- If wlan is not available 
 
SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_kamgr_wlan_is_available
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_PROCESS_DATA_SETTINGS_AND_ADVERTISE_THROTTLE_INFO

DESCRIPTION
  This function is to process data settings and advertise throttle info
  
PARAMETERS 
  subs_id: subscription id

DEPENDENCIES
  None.

RETURN VALUE
  NONE 
 
SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_kamgr_process_data_settings_and_advertise_throttle_info
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_DEFAULT_APN_CHG_CBACK

DESCRIPTION
  This function processes the apn change indication accepting
  the new apn name.
  
PARAMETERS
  subs_id
  apn name for new default apn

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_kamgr_default_apn_chg_cback
(
  sys_modem_as_id_e_type      subs_id,
  char                       *apn_name
);

/*===========================================================================
FUNCTION  DS_3GPP_KAMGR_DEFAULT_PROFILE_CHG_CBACK

DESCRIPTION
  This function processes the apn change indication accepting
  the profile number for new default apn.
  
PARAMETERS
  subs_id
  profile number for new default apn

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ds_3gpp_kamgr_default_profile_chg_cback
(
  sys_modem_as_id_e_type      subs_id,
  uint16                      profile_no
);

/*===========================================================================
FUNCTION DS_3GPP_KAMGR_SET_TEARDOWN_TIMER

DESCRIPTION
  This function set the KA PDN tear down timer if the timer is configured.
  
PARAMETERS
  Subscription Id
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE-- KA PDN tear down timer is set
  FALSE -- KA PDN tear down timer is not set
 
SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_kamgr_set_teardown_timer
(
  ds3gsubsmgr_subs_id_e_type       subs_indx
);

/*===========================================================================
FUNCTION DS_3GPP_KAMGR_CLR_TEARDOWN_TIMER

DESCRIPTION
  This function clear the KA PDN tear down timer if the timer is configured.
  
PARAMETERS
  Subscription Id
 
DEPENDENCIES
  None.

RETURN VALUE
  None.
 
SIDE EFFECTS
  None.
===========================================================================*/
void ds_3gpp_kamgr_clr_teardown_timer
(
  ds3gsubsmgr_subs_id_e_type       subs_indx
);
/*===========================================================================
FUNCTION DS_3GPP_KAMGR_SET_WIFI_AND_WIFI_CALLING_STATUS

DESCRIPTION
  This function is called when allocating kamgr instance. Will get wifi and
  wifi calling status from PS and cache in kamgr.

PARAMETERS
  subs_indx: Subscription Id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_kamgr_set_wifi_and_wifi_calling_status
(
  ds3gsubsmgr_subs_id_e_type             subs_indx
);

/*===========================================================================
FUNCTION DS_3GPP_KAMGR_SET_WIFI_CALLING_STATUS

DESCRIPTION
  This function sets wifi calling status.

PARAMETERS
  subs_indx: Subscription Id
  rat_pref:  rat preference

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_kamgr_set_wifi_calling_status
(
  ds3gsubsmgr_subs_id_e_type               subs_indx,
  ps_sys_rat_preference_enum_type          rat_pref
);

/*===========================================================================
FUNCTION DS_3GPP_KAMGR_WIFI_CALLING_IS_ON

DESCRIPTION
  This function get wifi calling status.
  
PARAMETERS
  Subscription Id
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE--wifi calling is on
  FALSE -- wifi calling in off
 
SIDE EFFECTS
  None.
===========================================================================*/
boolean ds_3gpp_kamgr_wifi_calling_is_on
(
  ds3gsubsmgr_subs_id_e_type       subs_indx
);
#endif /* DS_3GPP_KAMGR_EXT_H */
