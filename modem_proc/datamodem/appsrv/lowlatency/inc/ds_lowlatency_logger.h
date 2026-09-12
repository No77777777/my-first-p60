/*===========================================================================
                            DS_LOWLATENCY_LOGGER.H

DESCRIPTION

  Header file for ds lowlatency logger.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                            EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who     what, where, why
--------    ----    ----------------------------------------------------
11/01/19     mj     Initial version.
===========================================================================*/

#ifndef DS_LOWLATENCY_LOGGER_H
#define DS_LOWLATENCY_LOGGER_H

/*===========================================================================

                         INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "log.h"
#include "sys.h"
#include "ps_sys_ioctl.h"
#include "ds_lowlatency_dispatcher.h"

/*===========================================================================
                                 MACROS
===========================================================================*/

#define DS_LOWLATERNCY_LOGGING_VERSION 1

/*===========================================================================
                             TYPE DEFINITIONS
===========================================================================*/

LOG_RECORD_DEFINE(LOG_DS_LOW_LATENCY_INFO_C)
  uint8                                    version;               
  ///< Log packet version. Range: 0...255.
  uint8                                    sub_id;	             
  ///< Subscriber id: 1 is for SUB1, 2 is for SUB2
  ps_sys_ioctl_data_low_latency_info_type  ioctl_data_info;
  ds_low_latency_action_s_type             cached_action_info;
LOG_RECORD_END

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

void ds_lowlatency_low_latency_info_logging_commit
(
  sys_modem_as_id_e_type subs_id
);

#ifdef __cplusplus
}
#endif
#endif /* DS_LOWLATENCY_DISPATCHER_H */

