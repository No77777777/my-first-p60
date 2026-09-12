/*==============================================================================

                              ds_abss_mgr.h

GENERAL DESCRIPTION

Copyright (c) 2018 by Qualcomm Technologies Incorporated. All Rights Reserved.
==============================================================================*/

/*==============================================================================
                           EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/04/17    ml     Created file/Initial version.
==============================================================================*/
#ifndef DS_ABSS_API_H
#define DS_ABSS_API_H

#include "comdef.h"

#include "ds_ASMap.h"
#include "ds_ASString.h"
#include "ds_ASVector.h"
#include "ds_XMLElement.h"
#include "ds_appsrv_utils.h"
#include "ps_sys_ioctl.h"



void ds_abss_init();
void ds_abss_update_iccid_info(uint16 subs_idx, const ASString& iccid_str);

void ds_abss_ioctl_update_app_config(ps_sys_ioctl_3gpp_config_app_pref_type* app_pref_info);
void ds_abss_ioctl_remove_app_config(ps_sys_ioctl_3gpp_remove_app_pref_type* app_remove_info);
int16 ds_abss_ioctl_query_app_config(ps_sys_ioctl_3gpp_get_app_pref* app_pref_info);
void ds_abss_ioctl_set_foreground_app_list(ps_sys_ioctl_3gpp_fg_app_list_info_type* app_list);
void ds_abss_ioctl_get_foreground_app_list(ps_sys_ioctl_3gpp_fg_app_list_info_type* app_list);
void ds_abss_ioctl_query_app_list(ps_sys_ioctl_3gpp_get_app_list* app_list);



#endif /* DS_ABSS_API_H */
