/*==============================================================================

                              ds_abss_event_handler.h

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
#ifndef DS_ABSS_EVENT_HANDLER_H
#define DS_ABSS_EVENT_HANDLER_H

#include "comdef.h"
#include "ds_appsrv_utils.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ds_abss_event_init();

void ds_abss_iccid_info_chg_hdlr(ds_appsrv_iccid_info_s_type *iccid_info_ptr);

int ds_abss_event_ioctl_enable_app(void* arg_val_ptr, int16* ps_errno);
int ds_abss_event_ioctl_update_app_pref(void* arg_val_ptr, int16* ps_errno);
int ds_abss_event_ioctl_remove_app_pref(void* arg_val_ptr, int16* ps_errno);
int ds_abss_event_ioctl_get_app_pref(void* arg_val_ptr, int16* ps_errno);
int ds_abss_event_ioctl_set_foreground_app_list(void* arg_val_ptr, int16* ps_errno);
int ds_abss_event_ioctl_get_foreground_app_list(void* arg_val_ptr, int16* ps_errno);
int ds_abss_event_ioctl_get_app_list(void* arg_val_ptr, int16* ps_errno);


#ifdef __cplusplus
}
#endif

#endif /* DS_ABSS_EVENT_HANDLER_H */
