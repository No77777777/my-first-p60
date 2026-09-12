/*!
  @file
  ds_3gpp_nv_manager.h

  @brief
  Manages NV/EFS items related to 3GPP MH

  @detail
  OPTIONAL detailed description of this C header file.
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

$Header: //components/rel/data.mpss/3.5..2.1/3gpp/ps_ex/inc/ds_3gpp_nv_manager_ex.h#6 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/14   sb      Initial version.
===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#ifndef DS3GPPNVMANAGEREX_H
#define DS3GPPNVMANAGEREX_H

#include "datamodem_variation.h"
#ifdef FEATURE_DATA_3GPP
#include "dstask_v.h"
#include "ds_3gpp_hdlr.h"
#include "dsm_queue.h"
#include "ds3gmgr.h"
#include "ds_3gppi_utils.h"
#include "sys.h"
#include "ds_3gpp_flow_control.h"

#ifdef FEATURE_DATA_LTE

#ifdef FEATURE_DATA_4_5G

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_EX_READ_UI_DISPLAY_4_5G_SUPPORT

DESCRIPTION
  This function reads UI_DISPLAY_4_5G_SUPPORT NV item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_ex_read_ui_display_4_5g_support
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_EX_GET_UI_DISPLAY_4_5G_SUPPORT

DESCRIPTION
  This function returns the value of UI_DISPLAY_4_5G_SUPPORT configuration.

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
uint8 ds_3gpp_nv_manager_ex_get_ui_display_4_5g_support
(
  sys_modem_as_id_e_type  subs_id
);

#endif /* FEATURE_DATA_4_5G */

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_EX_GET_FCF_PARAMS

DESCRIPTION
  This function is used to retrieve the FCF parameters per
  subscription
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  ds_bearer_cntxt_fcf_timer_struct_type -FCF parameters information

SIDE EFFECTS
  None.

===========================================================================*/
ds_3gpp_fcf_bidir_struct_type ds_3gpp_nv_manager_ex_get_fcf_params
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_EX_READ_DETACH_OR_DISCONNECT_ACTION_TYPE

DESCRIPTION
  This function is used to read attach action NV item
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  ds_3gpp_nv_manager_ex_read_detach_or_disconnect_action_type
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_GENERIC_THROT_CONFIG

DESCRIPTION
  This function gets the generic throttling nv value for each subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether generic throttling  nv is enabled.

SIDE EFFECTS
===========================================================================*/
boolean ds_3gpp_nv_manager_get_generic_throt_config
(
  sys_modem_as_id_e_type subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_GENERIC_THROTTLING_NV

DESCRIPTION
  This function reads the generic throttling NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE 
 
SIDE EFFECTS
===========================================================================*/
void ds_3gpp_nv_manager_read_generic_throttling_nv
(
  sys_modem_as_id_e_type    subs_id
);
/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_READ_GENERIC_THROT_SKIP_SM_RETRY_FAILURE_NV

DESCRIPTION
  This function reads the generic throttle skip sm retry NV item for each
  subscription
 

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE 
 
SIDE EFFECTS
===========================================================================*/
void ds_3gpp_nv_manager_read_generic_throt_skip_sm_retry_nv
(
  sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_GET_GENERIC_THROT_SKIP_SM_RETRY_CONFIG

DESCRIPTION
  This function gets the generic throttle skip sm retry nv value for each 
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  Boolean Flag indicating whether generic throttle skip sm retry nv is enabled.

SIDE EFFECTS
===========================================================================*/
boolean ds_3gpp_nv_manager_get_generic_throt_skip_sm_retry_config
(
  sys_modem_as_id_e_type subs_id
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_EX_GET_DETACH_OR_DISCONNECT_ACTION_TYPE

DESCRIPTION
  This function is used to retrieve the action type for the action to be
  performed when AP modifies the attach pdn list
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  uint32. - Action type info

SIDE EFFECTS
  None.

===========================================================================*/
uint32  ds_3gpp_nv_manager_ex_get_detach_or_disconnect_action_type
(
   sys_modem_as_id_e_type    subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGGER_EX_READ_FCF_PARAMS

DESCRIPTION
  This function is used to read FCF parameters nv item
 
  PARAMETERS
  subs_id- subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void ds_3gpp_nv_manager_ex_read_fcf_params
(
   sys_modem_as_id_e_type    subs_id
);

#endif /* FEATURE_DATA_LTE */

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_EX_INIT

DESCRIPTION
  This function initializes the nv manager module. This function is called
  during boot up. This function is responsible for intializing the nv info
  for primary subscription.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_nv_manager_ex_init(void);



/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_EX_INIT_PER_SUBS_ID

DESCRIPTION
  This function is responsible for intializing the nv info for each
  subscription.
 
  PARAMETERS
  subs_id - subscription id

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void ds_3gpp_nv_manager_ex_init_per_subs_id
(
  sys_modem_as_id_e_type subs_id
);

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */


/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_EX_READ_CASE_SENSITIVE

DESCRIPTION
  This function reads USR_PWD_CASE_SENSITIVE NV item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_ex_read_case_sensitive
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_EX_GET_CASE_SENSITIVE

DESCRIPTION
  This function returns the value of USR_PWD_CASE_SENSITIVE configuration.

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8 ds_3gpp_nv_manager_ex_get_case_sensitive
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION DS_3GPP_NV_MANAGER_EX_READ_KAMGR_ALWAYS_ON

DESCRIPTION
  This function reads KAMGR_ALWAYS_ON NV item for each
  subscription

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  


SIDE EFFECTS

===========================================================================*/
void ds_3gpp_nv_manager_ex_read_kamgr_always_on
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_EX_READ_IGNORE_IMS_SETTING_NV 
 
DESCRIPTION
  This function reads the ignore ims setting nv configuration

PARAMETERS
  subs_id subscription id

DEPENDENCIES
  None.

RETURN VALUE  
  None.

SIDE EFFECTS 
  None. 

===========================================================================*/
void ds_3gpp_nv_manager_ex_read_ignore_ims_setting_nv
(
  sys_modem_as_id_e_type subs_id
);


/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_EX_GET_IGNORE_IMS_SETTING_NV

DESCRIPTION
  This function returns the value of ignore_ims_setting nv configuration.

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None

===========================================================================*/
boolean ds_3gpp_nv_manager_ex_get_ignore_ims_setting
(
  sys_modem_as_id_e_type  subs_id
);

/*===========================================================================
FUNCTION  DS_3GPP_NV_MANAGER_EX_GET_KAMGR_ALWAYS_ON

DESCRIPTION
  This function returns the value of KAMGR_ALWAYS_ON configuration.

PARAMETERS
  subs_id  subscription 
    
DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None

===========================================================================*/
uint8 ds_3gpp_nv_manager_ex_get_kamgr_always_on
(
  sys_modem_as_id_e_type  subs_id
);

#endif /* FEATURE_DATA_3GPP */
#endif /* DS3GPPNVMANAGEREX_H */
