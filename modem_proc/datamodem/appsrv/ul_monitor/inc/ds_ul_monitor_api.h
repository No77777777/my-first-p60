#ifndef DS_UL_MONITOR_API_H
#define DS_UL_MONITOR_API_H
/*===========================================================================
 
                        DS_UL_MONITOR_API.H

DESCRIPTION
  Header file of uplink monitor module APIs.
 
Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/ul_monitor/inc/ds_ul_monitor_api.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/11/16   hr      Initial version
===========================================================================*/
#include "datamodem_variation.h"

#ifdef FEATURE_DATA_WCDMA_LATENCY_OPT

#include "comdef.h"
#include "dsm_item.h"
#include "ps_ipfltr_defs.h"
#include "ps_dpm.h"

/*===========================================================================
                           MACRO AND TYPE DEFINITIONS
===========================================================================*/
typedef void (*ds_ul_monitor_cback_fptr_type)
(
  ps_dpm_ul_monitor_info_type *dpm_monitor_info_ptr,
  void                        *user_data
);

typedef enum
{
  UL_MONITOR_ACTION_MIN = 0,
  UL_MONITOR_ACTION_PASS_THROUGH = UL_MONITOR_ACTION_MIN,
  UL_MONITOR_ACTION_DROP,          /* 1 */
  UL_MONITOR_ACTION_MAX
}ds_ul_monitor_action_e_type;

typedef struct
{
  void  *fi_ptr_arr;
  uint8  num_filters;
}ds_ul_monitor_filter_type;

typedef struct
{
  boolean                        enabled;
  ds_ul_monitor_action_e_type    action;
  ds_ul_monitor_cback_fptr_type  cback_fptr;
  void                          *cback_user_data;
}ds_ul_monitor_info_type;

/*===========================================================================
                      EXTERNAL FUNCTIONS DEFINITIONS
===========================================================================*/
/*===========================================================================
FUNCTION      DS_UL_MONITOR_INIT

DESCRIPTION   Initializes the uplink monitor module
 
DEPENDENCIES  None

RETURN VALUE  None 

SIDE EFFECTS  None
===========================================================================*/
void ds_ul_monitor_init
(
  void
);

/*===========================================================================
FUNCTION      DS_UL_MONITOR_REG_FLTR

DESCRIPTION   Adds a filter so as to monitor uplink traffic
 
DEPENDENCIES  None

RETURN VALUE  A handle to the monitor        if success
              PS_IFACE_IPFLTR_INVALID_HANDLE if failure,
                                             and errno will be specified 

SIDE EFFECTS  Filters have to be registered on IPA HW during bootup, client 
              callback will be invoked if matching packet is detected on
              uplink when filter is enabled
===========================================================================*/
uint32 ds_ul_monitor_reg_fltr
(
  ds_ul_monitor_info_type   *monitor_info,
  ds_ul_monitor_filter_type *monitor_filter,
  int16                     *err_code
);

/*===========================================================================
FUNCTION      DS_UL_MONITOR_DEREG_FLTR

DESCRIPTION   Removes a filter from uplink traffic monitoring
 
DEPENDENCIES  None

RETURN VALUE  TRUE  if success
              FALSE if failure, and err_code will be specified 

SIDE EFFECTS  Filters may not be added back after dereg if HW only support
              filter reg during bootup 
===========================================================================*/
boolean ds_ul_monitor_dereg_fltr
(
  uint32  fltr_handle,
  int16  *err_code
);    

/*===========================================================================
FUNCTION      DS_UL_MONITOR_SET_FLTR_ENABLED

DESCRIPTION   Start monitoring uplink traffic with a specific filter
 
DEPENDENCIES  None

RETURN VALUE  TRUE  if success
              FALSE if failure

SIDE EFFECTS  Client callback will be invoked if matching packet is 
              detected on uplink when filter is enabled
===========================================================================*/
boolean ds_ul_monitor_set_fltr_enabled
(
  uint32   fltr_handle,
  boolean  enabled
);

#endif /* FEATURE_DATA_WCDMA_LATENCY_OPT */
#endif /* DS_UL_MONITOR_API_H */
