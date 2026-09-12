/*============================================================================
  FILE:         uim_common_qsh.c

  OVERVIEW:     The file defines the UIM QSH event logging module

  DEPENDENCIES: N/A

                Copyright (c) 2016 - 2019 QUALCOMM Technologies, Inc(QTI).
                All Rights Reserved.
                QUALCOMM Technologies Confidential and Proprietary
============================================================================*/

/*============================================================================
  EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.  Please
  use ISO format for dates.

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/common/src/uim_common_qsh.c#2 $

when       who      what, where, why
--------   ---      -----------------------------------------------------------
01/29/19   kb       Add support to send response when session handler is done
08/17/18   rps      Add support for mini dump phase 2
05/23/17   nr       DTF handlers for session activation and de-activation
03/12/17   bcho     Update handling of QSH_ACTION_CFG
02/01/17   bcho     Populate subs_id before notifying QSH for UIM event
08/04/16   bcho     Support added for QSH generic failure event
07/21/16   bcho     Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "uim_msg.h"
#include "uim_common_qsh.h"
#include "mmgsdilib_p.h"
#include "uim_p.h"
#include "gstk_p.h"
#include "qmi_uim.h"

/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
#define UIM_COMMON_QSH_ACTION_MASK 0x000000FF
#define UIM_COMMON_QSH_EVENT_MASK  0x0000FF00
#define UIM_COMMON_QSH_EVENT_SHIFT 0x08

#ifdef FEATURE_QSH_EVENT_METRIC

/* Array to store QSH event status */
static boolean uim_common_qsh_event_enabled_array[USIM_QSH_EVENT_MAX] = {0};


/*=============================================================================

                          UIM COMMON FUNCTIONS
                     Definitions used throughout UIM

=============================================================================*/

/*==========================================================================
FUNCTION UIM_COMMON_QSH_IS_FAILURE_EVENT

DESCRIPTION
  To check whether QSH event is failure event or not

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None
==========================================================================*/
static boolean uim_common_qsh_is_failure_event(
  usim_qsh_event_e qsh_event
)
{
  switch(qsh_event)
  {
    case USIM_QSH_EVENT_CARD_ERROR:
    case USIM_QSH_EVENT_RECOVERY_TRIGGERED:
      return TRUE;
    /* All other cases are handled as non-failures */
    default:
      break;
  }
  return FALSE;
}/* uim_common_qsh_is_failure_event */


#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif  /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */


#ifdef FEATURE_QSH_MDUMP
/*==========================================================================
FUNCTION UIM_QSH_MDUMP_DUMP_GLOBALS

DESCRIPTION
  Used to dump globals of different modules

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
QSH_MDUMP_FN_ATTR void uim_qsh_mdump_dump_globals(void)
{
  uim_qsh_mdump_mmgsdi();
  uim_qsh_mdump_uimdrv();
  uim_qsh_mdump_gstk();
  uim_qsh_mdump_uimqmi();
} /* uim_qsh_mdump_dump_globals */
#endif /* FEATURE_QSH_MDUMP */


/*==========================================================================
FUNCTION UIM_COMMON_QSH_CB

DESCRIPTION
  This is callback function registered with QSH module to set status of UIM QSH events

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif /* FEATURE_QSH_MDUMP */
static void uim_common_qsh_cb(
  qsh_client_cb_params_s* qsh_cb_params_ptr
)
{
  if(qsh_cb_params_ptr == NULL ||
     (qsh_cb_params_ptr->action != QSH_ACTION_CFG
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif  /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
#ifdef FEATURE_QSH_MDUMP
      && qsh_cb_params_ptr->action != QSH_ACTION_MDUMP
#endif  /* FEATURE_QSH_MDUMP */
     )
    )
  {
    return;
  }

  switch(qsh_cb_params_ptr->action_mode)
  {
    case QSH_ACTION_MODE_SYNC_REQUIRED:
    case QSH_ACTION_MODE_SYNC_OPTIONAL:
      switch(qsh_cb_params_ptr->action)
      {
        case QSH_ACTION_CFG:
          uim_common_qsh_process_action_event_config_params(qsh_cb_params_ptr,
                                                            TRUE);
          break;

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
        #error code not present
#endif  /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */

#ifdef FEATURE_QSH_MDUMP
        case QSH_ACTION_MDUMP:
          (void)uim_qsh_mdump_dump_globals();
          break;
#endif /* FEATURE_QSH_MDUMP */

        default:
          break;
      }
      break;

    case QSH_ACTION_MODE_ASYNC_REQUIRED:
      (void)mmgsdi_qsh_action_event_config(qsh_cb_params_ptr);
      break;

    default:
      break;
  }
}/* uim_common_qsh_cb */
#endif /*FEATURE_QSH_EVENT_METRIC */


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
void uim_common_qsh_process_action_event_config_params(
  qsh_client_cb_params_s* qsh_cb_params_ptr,
  boolean                 is_mode_sync
)
{
#ifdef FEATURE_QSH_EVENT_METRIC
  qsh_client_action_done_s cb_done;
  qsh_event_action_e       action     = QSH_EVENT_ACTION_DISABLE;
  usim_qsh_event_e         event_type = USIM_QSH_EVENT_MAX;

  if(qsh_cb_params_ptr == NULL)
  {
    return;
  }

  memset(&cb_done, 0x00, sizeof(cb_done));

  /* cmd_code format: length of payload, sub_id, event_id, CFG_action
     Note that UIM does not have any payload data with cmd code; also it does
     not maintain per sub events */
  action = UIM_COMMON_QSH_ACTION_MASK & qsh_cb_params_ptr->action_params.cfg.cmd_code;
  event_type = (usim_qsh_event_e)((UIM_COMMON_QSH_EVENT_MASK & qsh_cb_params_ptr->action_params.cfg.cmd_code) >> UIM_COMMON_QSH_EVENT_SHIFT);

  if(event_type < USIM_QSH_EVENT_MAX)
  {
    uim_common_qsh_event_enabled_array[event_type] = (action == QSH_EVENT_ACTION_ENABLE) ? TRUE : FALSE;
  }

  /*  QSH should be intimated that our event configuration is updated */
  qsh_client_action_done_init(&cb_done);

  cb_done.cb_params_ptr = qsh_cb_params_ptr;

  cb_done.action_mode_done = is_mode_sync ? QSH_ACTION_MODE_DONE_SYNC : QSH_ACTION_MODE_DONE_ASYNC;
  qsh_client_action_done(&cb_done);
#else
  (void)qsh_cb_params_ptr;
  (void)is_mode_sync;
#endif /* FEATURE_QSH_EVENT_METRIC */
}/* uim_common_qsh_process_action_event_config_params */


/*==========================================================================
FUNCTION UIM_COMMON_QSH_IS_EVENT_ENABLED

DESCRIPTION
  This function provides status of UIM QSH events

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None
==========================================================================*/
boolean uim_common_qsh_is_event_enabled(
  usim_qsh_event_e qsh_event
)
{
#ifdef FEATURE_QSH_EVENT_METRIC
  if(qsh_event < USIM_QSH_EVENT_MAX)
  {
    /* If DEBUG_ANY_FAILURE event is enabled, returns TRUE; if not, fallback to
       failure events' status */
    if(uim_common_qsh_is_failure_event(qsh_event) == FALSE)
    {
      return uim_common_qsh_event_enabled_array[qsh_event];
    }
    else
    {
      return uim_common_qsh_event_enabled_array[USIM_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE] ||
             uim_common_qsh_event_enabled_array[qsh_event];
    }
  }
#else
  (void) qsh_event;
#endif /* FEATURE_QSH_EVENT_METRIC */
  return FALSE;
}/* uim_common_qsh_is_event_enabled */


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
void uim_common_qsh_init(
  void
)
{
#ifdef FEATURE_QSH_EVENT_METRIC
  qsh_client_reg_s uim_client_reg;

  memset(&uim_client_reg, 0x00, sizeof(uim_client_reg));

  /* Initialize uim client */
  qsh_client_reg_init(&uim_client_reg);

  /*Populate UIM client info to QSH */
  uim_client_reg.client = QSH_CLT_USIM;
  uim_client_reg.client_cb_ptr = uim_common_qsh_cb;
  uim_client_reg.major_ver = USIM_QSH_MAJOR_VER;
  uim_client_reg.minor_ver = USIM_QSH_MINOR_VER;
  uim_client_reg.cb_action_support_mask = QSH_ACTION_CFG
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
                                          #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
#ifdef FEATURE_QSH_MDUMP
                                          | QSH_ACTION_MDUMP
#endif /* FEATURE_QSH_MDUMP */
  ;
  qsh_client_reg(&uim_client_reg);
#endif /* FEATURE_QSH_EVENT_METRIC */
}/* uim_common_qsh_init */


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
void uim_common_qsh_event_notify(
  usim_qsh_event_e       qsh_event,
  sys_modem_as_id_e_type as_id
)
{
#ifdef FEATURE_QSH_EVENT_METRIC
  qsh_client_event_notify_params_s  event_notify_params;

  memset(&event_notify_params, 0x00, sizeof(event_notify_params));

  if(qsh_event >= USIM_QSH_EVENT_MAX)
  {
    return;
  }

  qsh_client_event_notify_init(&event_notify_params);

  QSH_LOG(QSH_CLT_USIM, QSH_CAT_EVENT, QSH_MSG_TYPE_HIGH, "UIM event 0x%x",
          qsh_event);

  UIM_MSG_LOW_1("QSH UIM event: 0x%x", qsh_event);

  /* For failure events, notify with DEBUG_ANY_FAILURE if it is enabled.
     Note that even if failure events are enabled we use DEBUG_ANY_FAILURE in
     above scenario */
  if((uim_common_qsh_is_failure_event(qsh_event)) &&
     uim_common_qsh_event_enabled_array[USIM_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE])
  {
    qsh_event = USIM_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE;
  }

  event_notify_params.subs_id = as_id;
  event_notify_params.client = QSH_CLT_USIM;
  event_notify_params.id = qsh_event;
  event_notify_params.event_data = NULL;

  qsh_client_event_notify(&(event_notify_params));
#else
  (void)qsh_event;
#endif /* FEATURE_QSH_EVENT_METRIC */
}/* uim_common_qsh_event_notify */
