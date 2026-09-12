#ifndef DS_UL_MONITOR_WCDMA_LATENCY_OPT_H
#define DS_UL_MONITOR_WCDMA_LATENCY_OPT_H
/*===========================================================================
 
                        DS_UL_MONITOR_API.H

DESCRIPTION
  Header file of WCDMA latency optimization module
 
Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/appsrv/ul_monitor/inc/ds_ul_monitor_wcdma_latency_opt.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/11/16   hr      Initial version
===========================================================================*/
#include "datamodem_variation.h"

#ifdef FEATURE_DATA_WCDMA_LATENCY_OPT

/*===========================================================================
FUNCTION      DS_UL_MONITOR_WCDMA_LATENCY_OPT_FILTER_INIT

DESCRIPTION   Initializes the DNS filter
 
DEPENDENCIES  None

RETURN VALUE  None 

SIDE EFFECTS  None
===========================================================================*/
void ds_ul_monitor_wcdma_latency_opt_filter_init
(
  void
);

#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================
FUNCTION      DS_UL_MONITOR_WCDMA_LATENCY_OPT_EVENT_INIT

DESCRIPTION   Register for event callbacks
 
DEPENDENCIES  None

RETURN VALUE  None 

SIDE EFFECTS  None
===========================================================================*/
void ds_ul_monitor_wcdma_latency_opt_event_init
(
  void
);

#ifdef __cplusplus
}
#endif

#endif /* FEATURE_DATA_WCDMA_LATENCY_OPT */
#endif /* DS_UL_MONITOR_WCDMA_LATENCY_OPT_H */
