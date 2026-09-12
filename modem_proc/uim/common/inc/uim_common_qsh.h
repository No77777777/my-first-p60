#ifndef UIM_COMMON_QSH_H
#define UIM_COMMON_QSH_H
/*===========================================================================


                U I M   C O M M O N   Q S H   H E A D E R


DESCRIPTION
    This is the header file for the UIM QSH event logging module

   Copyright (c) 2016 - 2017 by QUALCOMM Technologies, Inc. All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/common/inc/uim_common_qsh.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
05/23/17   nr      DTF handlers for session activation and de-activation
02/01/17   bcho    Populate subs_id before notifying QSH for UIM event
07/21/16   bcho    Initial version


=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "comdef.h"
#include "qsh.h"
#include "usim_qsh_ext.h"
#include "sys.h"

/* Enumeration for the type of Event */
typedef enum {
  UIM_QSH_EVENT_NOTIFY_NONE = -1,
  UIM_QSH_EVENT_NOTIFY_SESSION_ACTIVATION = 0,   /* Notify Session activation */
  UIM_QSH_EVENT_NOTIFY_SESSION_DEACTIVATION = 1, /* Notify Session de-activation */
  UIM_QSH_EVENT_NOTIFY_MAX
}uim_qsh_event_notify_e;

/*==========================================================================
FUNCTION UIM_COMMON_QSH_PROCESS_ACTION_EVENT_CONFIG_PARAMS

DESCRIPTION
  This function processes QSH event config action

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
void uim_common_qsh_process_action_event_config_params
(
  qsh_client_cb_params_s* qsh_cb_params_ptr,
  boolean                 is_mode_sync
);

/*==========================================================================
FUNCTION UIM_COMMON_QSH_IS_EVENT_ENABLED

DESCRIPTION
  This function provides status of UIM QSH events

DEPENDENCIES
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None
==========================================================================*/
boolean uim_common_qsh_is_event_enabled
(
  usim_qsh_event_e qsh_event
);

/*==========================================================================
FUNCTION UIM_COMMON_QSH_INIT

DESCRIPTION
  This function registers UIM with QSH module 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
void uim_common_qsh_init
(
  void
);

/*==========================================================================
FUNCTION UIM_COMMON_QSH_EVENT_NOTIFY

DESCRIPTION
  This function notifies QSH module for UIM event.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
void uim_common_qsh_event_notify
(
  usim_qsh_event_e       qsh_event,
  sys_modem_as_id_e_type as_id
);

#endif /*UIM_COMMON_QSH_H*/
