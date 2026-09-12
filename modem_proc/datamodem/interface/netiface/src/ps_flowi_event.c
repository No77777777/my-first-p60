/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           P S _ F L O W I _ E V E N T . C


GENERAL DESCRIPTION
  This is the implementation of functions used to handle PS flow events.

EXTERNAL FUNCTIONS
  PS_FLOW_EVENT_INIT()
    Initializes the global queues for ps_flow

  PS_FLOW_ALLOC_EVENT_CBACK_BUF()
    Allocates memory used to register for ps_flow event callbacks

  PS_FLOW_FREE_EVENT_CBACK_BUF()
    Free the memory used to register for ps_flow event callbacks

  PS_FLOW_EVENT_CBACK_REG()
    Registers callbacks for events on a flow

  PS_FLOW_EVENT_CBACK_DEREG()
    Deregisters callbacks for a specific event on an flow

  PS_FLOWI_PHYS_LINK_EV_CBACK_REG()
    Registers a flow for all phys link state change events on the phys link
    it is bound to

  PS_FLOWI_PHYS_LINK_EV_CBACK_DEREG()
    Deregisters a flow for all phys link state change events on the phys link
    it is bound to.

  PS_FLOWI_INVOKE_EVENT_CBACKS()
    Invokes all of the event callbacks for a given flow and event

  PS_FLOWI_ASSOC_FLOW_EV_CBACK_REG()
    Registers callback for events on an associated flow

   PS_FLOWI_ASSOC_FLOW_EV_CBACK_DEREG()
    Deregisters callback for events on an associated flow

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2005-2014 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/ps_flowi_event.c#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $
when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/01/07    ssh    QoS support for logical ifaces
09/12/06    msr    Removed redundant state field in event_info structure
04/25/06    msr    L4/Tasklock code review changes
02/22/06    msr    Using single critical section
02/06/06    msr    Updated for L4 tasklock/crit sections.
11/10/05    msr    Fixed Lint errors.
10/18/05    msr    Removed support for FLOW_ACTIVATE_MODIFIED_EV and
                   PHYS_LINK_UP_MODIFIED_EV
08/16/05    msr    Fixed PS_BRANCH_TASKFREE()
08/15/05    mct    Added qos configure support.
05/12/05    ks     Fixed Lint errors.
05/03/05    msr    Using a macro to register an event with phys link
04/17/05    msr    Created file.
===========================================================================*/


/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "datamodem_variation.h"
#include "customer.h"
#include "comdef.h"

#include "err.h"
#include "amssassert.h"


#include "ps_crit_sect.h"
#include "ps_iface_defs.h"
#include "ps_phys_linki_event.h"
#include "ps_flow_event.h"
#include "ps_flowi_event.h"
#include "ps_flow.h"
#include "ps_flowi.h"
#include "ds_flow_control.h"
#include "ps_mem.h"
#include "ps_iface_flow.h"
#include "ps_iface_logical_flowi.h"
#include "dcc_task_svc.h"
#include "dcc_task_defs.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_system_heap.h"
#include "ps_ifacei.h"
#include "ps_utils.h"


/*===========================================================================

                           LOCAL DATA DECLARATIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  This variable is used to store callbacks that are intended to be called
  when events happen on ANY flow.
---------------------------------------------------------------------------*/
static q_type  global_flow_event_handle_q;

typedef struct
{
  ps_flow_event_cback_type  event_cback_f_ptr;
  void                    * user_data_ptr;
} ps_flowi_event_cb_info_type;

/*===========================================================================

                             INTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
FUNCTION PS_FLOWI_IS_EVENT_SET()

DESCRIPTION
  This function checks if the bit corresponding to the event is already set in
  the event handle. 
 
PARAMETERS
  event_handle  : event handle
  event         : event to check for
 
RETURN VALUE
  TRUE if bit set
  FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean ps_flowi_is_event_set
( 
  ps_flowi_event_handle_type  * event_handle, 
  ps_iface_event_enum_type      event 
)  
{
  uint32 result = 0;
  if (event >= FLOW_MIN_EV && event < FLOW_MAX_EV )
  {
    result = (event_handle)->event_mask & (uint32)(1 << (event - FLOW_MIN_EV));  
  }
  if (result)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
} /* ps_flowi_is_event_set */

/*===========================================================================
FUNCTION PS_FLOWI_SET_EVENT_BIT()

DESCRIPTION
  This function sets the bit corresponding to the event in the event handle. 
 
PARAMETERS
  event_handle  : event handle
  event         : event to set
 
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_flowi_set_event_bit
( 
  ps_flowi_event_handle_type  * event_handle, 
  ps_iface_event_enum_type      event 
)                 
{
  event_handle->event_mask |= (uint32)(1 << (event - FLOW_MIN_EV));
} /* ps_flowi_set_event_bit */

/*===========================================================================
FUNCTION PS_FLOWI_UNSET_EVENT_BIT()

DESCRIPTION
  This function unsets the bit corresponding to the event in the event handle. 
 
PARAMETERS
  event_handle  : event handle
  event         : event to set
 
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_flowi_unset_event_bit
( 
  ps_flowi_event_handle_type  * event_handle, 
  ps_iface_event_enum_type      event 
)                 
{
  event_handle->event_mask &=  ~((uint32)(1 << (event - FLOW_MIN_EV)));
} /* ps_flowi_unset_event_bit */

/*===========================================================================
FUNCTION PS_FLOWI_COMPARE_EVENT_HANDLE()

DESCRIPTION
  This function compares passed in cb fn and user data ptr against values in
  the event handle queue. 
 
PARAMETERS
  item_ptr      : event handle in queue
  compare_val   : struct containing cb fn and user data ptr
 
RETURN VALUE
  TRUE if values match
  FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static int ps_flowi_compare_event_handle
( 
  void *item_ptr,
  void *compare_val 
)
{
  ps_flowi_event_handle_type  *evt_handle = (ps_flowi_event_handle_type*)item_ptr;
  ps_flowi_event_cb_info_type *cb_info = (ps_flowi_event_cb_info_type*)compare_val;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ((evt_handle->event_cback_f_ptr == cb_info->event_cback_f_ptr) &&
      (evt_handle->user_data_ptr == cb_info->user_data_ptr))
  {
    return TRUE;
  }
  return FALSE;
} /* ps_flowi_compare_event_handle */

/*===========================================================================
FUNCTION PS_FLOWI_PHYS_LINK_EVENT_CBACK()

DESCRIPTION
  This event callback will be registered by ps_flow as event callback with
  phys link. This function is called when a phys link event of interest
  happens, and flow either calls commands or posts indications to change its
  state appropriately. This makes flows transparent to mode handlers when
  phys link is FLOW_COUPLED.

PARAMETERS
  flow_ptr      : ptr to the flow on which we are operating
  event         : the event for which callbacks need to be called
  event_info    : event information that needs to be passed into callback.
  user_data_ptr : pointer to user data info

RETURN VALUE
  None

DEPENDENCIES
  Must be called inside TASKLOCK

SIDE EFFECTS
  None
===========================================================================*/
static void ps_flowi_phys_link_event_cback
(
  ps_phys_link_type           * phys_link_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_data_ptr
)
{
  ps_flow_type  * flow_ptr;
#ifdef FEATURE_DATA_PS_QOS
  int16           ps_errno;
#endif /* FEATURE_DATA_PS_QOS */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_PHYS_LINK_IS_VALID(phys_link_ptr) || user_data_ptr == NULL)
  {
    LOG_MSG_ERROR_INT_0("ps_flowi_phys_link_event_cback(): "
                    "Invalid parameters are passed");
    return;
  }

  flow_ptr = (ps_flow_type *) user_data_ptr;
  if (!PS_FLOW_IS_VALID(flow_ptr))
  {
    LOG_MSG_ERROR_1("ps_flowi_phys_link_event_cback(): "
                    "Flow, 0x%p, is invalid", flow_ptr);
    DS_UTILS_ASSERT(0);
    return;
  }

  /*-------------------------------------------------------------------------
    A ps_flow registers for phys link events only when phys link is not
    DECOUPLED
  -------------------------------------------------------------------------*/
  if (PS_PHYS_LINKI_GET_CAPABILITY(phys_link_ptr,
                                   PS_PHYS_LINK_CAPABILITY_FLOW_DECOUPLED))
  {
    LOG_MSG_ERROR_1("ps_flowi_phys_link_event_cback(): "
                    "Phys link, 0x%p is not COUPLED to a flow", phys_link_ptr);
    DS_UTILS_ASSERT(0);
    return;
  }

  /*-------------------------------------------------------------------------
    A ps_flow registers for phys link events only on the phys link it is
    bound to and deregisters when it is unbound
  -------------------------------------------------------------------------*/
  if (PS_FLOWI_GET_PHYS_LINK(flow_ptr) != phys_link_ptr)
  {
    LOG_MSG_ERROR_2("ps_flowi_phys_link_event_cback(): "
                    "Flow, 0x%p, is not bound to phys link, 0x%p",
                    flow_ptr, phys_link_ptr);
    DS_UTILS_ASSERT(0);
    return;
  }

  LOG_MSG_INFO2_3("ps_flowi_phys_link_event_cback(): "
                  "Processing event, %d on phys link 0x%p for flow 0x%p",
                  event, phys_link_ptr, flow_ptr);

  /*-------------------------------------------------------------------------
    Based on phys link event, call commands/indications to change flow's
    state accordingly. Since only ps_flow_activate_ind() and
    ps_flow_go_null_ind() are allowed to be posted on default flow, don't
    post other commands and indications on default flow
  -------------------------------------------------------------------------*/
  switch (event)
  {
    case PHYS_LINK_UP_EV:
      ps_flow_activate_ind(flow_ptr,
                           event_info.phys_link_event_info.info_code);
      break;

    case PHYS_LINK_GONE_EV:
      if (event_info.phys_link_event_info.state != PHYS_LINK_NULL)
      {
        ps_flow_go_null_ind(flow_ptr,
                            event_info.phys_link_event_info.info_code);
      }
      break;

#ifdef FEATURE_DATA_PS_QOS
    case PHYS_LINK_DOWN_EV:
      if (!PS_FLOWI_GET_CAPABILITY(flow_ptr, PS_FLOW_CAPABILITY_DEFAULT))
      {
        ps_flow_suspend_ind(flow_ptr,
                            event_info.phys_link_event_info.info_code);
      }
      break;

    case PHYS_LINK_COMING_UP_EV:
      if (!PS_FLOWI_GET_CAPABILITY(flow_ptr, PS_FLOW_CAPABILITY_DEFAULT))
      {
        (void) ps_flow_activate_cmd(flow_ptr, &ps_errno, NULL);
      }
      break;

    case PHYS_LINK_RESUMING_EV:
      if (!PS_FLOWI_GET_CAPABILITY(flow_ptr, PS_FLOW_CAPABILITY_DEFAULT))
      {
        (void) ps_flow_resume_cmd(flow_ptr, &ps_errno, NULL);
      }
      break;

    case PHYS_LINK_GOING_DOWN_EV:
      if (!PS_FLOWI_GET_CAPABILITY(flow_ptr, PS_FLOW_CAPABILITY_DEFAULT))
      {
        (void) ps_flow_suspend_cmd(flow_ptr, &ps_errno, NULL);
      }
      break;

    case PHYS_LINK_GOING_NULL_EV:
      if (!PS_FLOWI_GET_CAPABILITY(flow_ptr, PS_FLOW_CAPABILITY_DEFAULT))
      {
        (void) ps_flow_go_null_cmd(flow_ptr, &ps_errno, NULL);
      }
      break;
#else
    case PHYS_LINK_COMING_UP_EV:
    case PHYS_LINK_GOING_DOWN_EV:
    case PHYS_LINK_DOWN_EV:
    case PHYS_LINK_RESUMING_EV:
    case PHYS_LINK_GOING_NULL_EV:
      break;
#endif /* FEATURE_DATA_PS_QOS */

    default:
      DS_UTILS_ASSERT(0);
      LOG_MSG_ERROR_INT_1("ps_flowi_phys_link_event_cback(): "
                      "Unknown event %d", event);
      break;
  }
} /* ps_flowi_phys_link_event_cback() */



/*===========================================================================
FUNCTION PS_FLOWI_ASSOC_FLOW_EV_CBACK()

DESCRIPTION
  This function processes flow events received from the flow associated
  with a flow.

PARAMETERS
  assoc_flow_ptr: ptr to the flow on which the event occured
  event         : the event for which callbacks need to be called
  event_info    : event information that needs to be passed into callback.
  user_data_ptr : pointer to user data info
                  (registered via ps_flow_assoc_flow_ev_cback_reg)

RETURN VALUE
  None

DEPENDENCIES
  Must be called inside TASKLOCK

SIDE EFFECTS
  None
===========================================================================*/
static void ps_flowi_assoc_flow_ev_cback
(
  ps_flow_type                * assoc_flow_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info,
  void                        * user_data_ptr
)
{
  ps_logical_flow_assoc_flow_ev_cmd_type * assoc_flow_cmd_ptr;
  ps_flow_type                           * logical_flow_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if 0
  LOG_MSG_FUNCTION_ENTRY_2("ps_flowi_assoc_flow_ev_cback(): "
                           "Event %d on flow 0x%p", event, assoc_flow_ptr);
#endif

  PS_SYSTEM_HEAP_MEM_ALLOC(assoc_flow_cmd_ptr,
                           sizeof(ps_logical_flow_assoc_flow_ev_cmd_type),
                           ps_logical_flow_assoc_flow_ev_cmd_type*);
  if (assoc_flow_cmd_ptr == NULL)
  {
    return;
  }

  assoc_flow_cmd_ptr->event         = event;
  assoc_flow_cmd_ptr->event_info    = event_info;
  assoc_flow_cmd_ptr->user_data_ptr = user_data_ptr;
  assoc_flow_cmd_ptr->assoc_flow_ptr = assoc_flow_ptr;

  do
  {

    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

    logical_flow_ptr = ((ps_flow_type *) user_data_ptr);
    if (!PS_FLOW_IS_VALID(logical_flow_ptr))
    {
      LOG_MSG_INVALID_INPUT_1("ps_flowi_assoc_flow_ev_cback(): "
                              "Got invalid logical flow_ptr 0x%p",
                              logical_flow_ptr);
      break;
    }

    assoc_flow_cmd_ptr->logical_flow_cookie =
                        PS_FLOWI_GET_COOKIE(logical_flow_ptr);

    /*-----------------------------------------------------------------------
      TODO: dcc_set_cmd_handler has to be moved to a new function called
      from dcci_init
    -----------------------------------------------------------------------*/
    (void) dcc_set_cmd_handler
           (
             DCC_LOGICAL_FLOW_ASSOC_FLOW_EV_CMD,
             ps_flowi_process_assoc_flow_ev
           );

    dcc_send_cmd_ex(DCC_LOGICAL_FLOW_ASSOC_FLOW_EV_CMD, assoc_flow_cmd_ptr);
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    LOG_MSG_FUNCTION_EXIT_2("ps_flowi_assoc_flow_ev_cback(): "
                            "Success: Event %d on flow 0x%p",
                            event, assoc_flow_ptr);
    return;
  } while(0);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  /* Failure case: Release the allocated DCC cmd buffer */
  PS_SYSTEM_HEAP_MEM_FREE(assoc_flow_cmd_ptr);

#if 0
  LOG_MSG_FUNCTION_EXIT_2("ps_flowi_assoc_flow_ev_cback(): "
                          "Fail: Event %d on flow 0x%p",
                          event, assoc_flow_ptr);
#endif
  return;

} /* ps_flowi_assoc_flow_ev_cback() */



/*===========================================================================

                             EXTERNAL FUNCTIONS

===========================================================================*/
/*===========================================================================
FUNCTION PS_FLOW_EVENT_INIT()

DESCRIPTION
  This function initializes the global queues for ps_flow.

PARAMETERS
  None

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void ps_flow_event_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Initialize global event queue
  -------------------------------------------------------------------------*/
  (void)q_init(&global_flow_event_handle_q);

} /* ps_flow_event_init() */

/*===========================================================================
FUNCTION PS_FLOWI_PHYS_LINK_EV_CBACK_REG()

DESCRIPTION
  This function registers a flow for all phys link state change events on
  the phys link it is bound to.

PARAMETERS
  flow_ptr : ptr to flow which is interested in phys link events

RETURN VALUE
  None

DEPENDENCIES
  flow_ptr must be valid and it must be bound to a phys link

SIDE EFFECTS
  None
===========================================================================*/
void ps_flowi_phys_link_ev_cback_reg
(
  ps_flow_type  * flow_ptr
)
{
  ps_iface_error_enum_type ps_error_info = PS_IFACE_SUCCESS;
  int                      retval = 0;

  /*-------------------------------------------------------------------------
    Note - To subscribe/register for a new phys link event, just add it in 
    this array
  -------------------------------------------------------------------------*/
  ps_iface_event_enum_type  phys_link_events [] = 
  {
    PHYS_LINK_COMING_UP_EV,
    PHYS_LINK_UP_EV,
    PHYS_LINK_GOING_DOWN_EV,
    PHYS_LINK_DOWN_EV,
    PHYS_LINK_RESUMING_EV,
    PHYS_LINK_GOING_NULL_EV,
    PHYS_LINK_GONE_EV
  };
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_UTILS_ASSERT(PS_FLOWI_GET_PHYS_LINK(flow_ptr) != NULL);
  
  do
  {
    flow_ptr->flow_private.phys_link_ev_cback_buf = 
      ps_phys_link_alloc_event_handle(PS_FLOWI_GET_PHYS_LINK(flow_ptr),
                                      ps_flowi_phys_link_event_cback,
                                      (void *) flow_ptr);
    if (NULL == flow_ptr->flow_private.phys_link_ev_cback_buf)
    {
      ps_error_info = PS_IFACE_ERROR_HANDLE_ALLOC_FAILED;
      break;
    }

    retval = ps_phys_link_subscribe_event_list
  	     (
               flow_ptr->flow_private.phys_link_ev_cback_buf,
  	       phys_link_events,
  	       ARRAY_SIZE(phys_link_events)
             );
 
    if (0 != retval)
    {
      ps_error_info = PS_IFACE_ERROR_EVENT_REG_FAILED;
      ps_phys_link_free_event_handle(flow_ptr->flow_private.phys_link_ev_cback_buf);
      flow_ptr->flow_private.phys_link_ev_cback_buf = NULL;
      DS_UTILS_ASSERT(0);
      break;
    }
    
    return;
  } while(0);

  LOG_MSG_ERROR_INT_1("ps_flowi_phys_link_ev_cback_reg(): Error = %d", ps_error_info);

} /* ps_flowi_phys_link_ev_cback_reg() */

/*===========================================================================
FUNCTION PS_FLOWI_PHYS_LINK_EV_CBACK_DEREG()

DESCRIPTION
  This function de-registers a flow for all phys link state change events on
  the phys link it is bound to.

PARAMETERS
  flow_ptr : ptr to flow which is interested in phys link events

RETURN VALUE
  None

DEPENDENCIES
  flow_ptr must be valid and it must be bound to a phys link. If ps_low is
  not registered for phys link events, this function will result in crash

SIDE EFFECTS
  None
===========================================================================*/
void ps_flowi_phys_link_ev_cback_dereg
(
  ps_flow_type  * flow_ptr
)
{
/*- - - - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*-------------------------------------------------------------------------
   NOTE: Directly use free_event_handle when all the events are to be
   unsubscribed.
  -------------------------------------------------------------------------*/

  ps_phys_link_free_event_handle
    (flow_ptr->flow_private.phys_link_ev_cback_buf);
  flow_ptr->flow_private.phys_link_ev_cback_buf = NULL;  

} /* ps_flowi_phys_link_ev_cback_dereg() */



/*===========================================================================
FUNCTION PS_FLOWI_INVOKE_EVENT_CBACKS()

DESCRIPTION
  This function will invoke all of the event callbacks for a given flow
  and event.  It will also call the global callbacks for said event, if any.

PARAMETERS
  flow_ptr   : ptr to the flow on which we are operating
  event      : the event for which callbacks need to be called
  event_info : event information that needs to be passed into callback.

RETURN VALUE
  None

DEPENDENCIES
  Must be called inside TASKLOCK()

SIDE EFFECTS
  None
===========================================================================*/
void ps_flowi_invoke_event_cbacks
(
  ps_flow_type                * flow_ptr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    event_info
)
{
  q_type                  * working_q_ptr;
  q_type                  * working_global_q_ptr;
  ps_flowi_event_handle_type * event_handle_ptr, * next_event_handle_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (!PS_FLOW_IS_VALID(flow_ptr))
  {
    LOG_MSG_ERROR_1("ps_flowi_invoke_event_cbacks(): "
                    "Invalid flow, 0x%p, is passed", flow_ptr);
    DS_UTILS_ASSERT(0);
    return;
  }

  if(( event < FLOW_MIN_EV ) || ( event >= FLOW_MAX_EV ))
  {
    LOG_MSG_ERROR_INT_1("ps_flowi_invoke_event_cbacks(): "
                        "Invalid flow event passed %d", event);
    DS_UTILS_ASSERT(0);
    return;
  }

  working_q_ptr =
    &(flow_ptr->flow_private.event_handle_q);
  working_global_q_ptr = &global_flow_event_handle_q;

  for (;;)
  {
    /*-----------------------------------------------------------------------
      Call callbacks for all items in queue
    -----------------------------------------------------------------------*/
    event_handle_ptr = (ps_flowi_event_handle_type *) q_check(working_q_ptr);
    while (event_handle_ptr != NULL)
    {
      next_event_handle_ptr = 
        (ps_flowi_event_handle_type *) (q_next(working_q_ptr, &(event_handle_ptr->link)));

      if (ps_flowi_is_event_set(event_handle_ptr, event))
      {
        event_handle_ptr->event_cback_f_ptr(flow_ptr,
                                         event,
                                         event_info,
                                         event_handle_ptr->user_data_ptr);
      }

      event_handle_ptr = next_event_handle_ptr;
     
    } /* while(items in queue) */

    /*-----------------------------------------------------------------------
      If the working_q_ptr is set to qlobal_event_handle_q then we are done,
      otherwise we need to call those callbacks
    -----------------------------------------------------------------------*/
    if (working_q_ptr == working_global_q_ptr)
    {
      break;
    }
    else
    {
      working_q_ptr = working_global_q_ptr;
    }
  } /* for (all specific and global callbacks) */

} /* ps_flowi_invoke_event_cbacks() */


/*===========================================================================
FUNCTION PS_FLOWI_ASSOC_FLOW_EV_CBACK_REG

DESCRIPTION
  This function registers a flow for events on its associated ps_flow.
  Only the stable events (such as activated, suspended etc.) are registered.
  The transitory events (such as activating, suspending etc.) are not
  handled by a logical flow.

  If this function returns error, the caller is expected to delete
  the logical flow. Else the behaviour is undeterministic.

PARAMETERS
  flow_ptr                      : ptr to flow
  assoc_flow_event_cback_f_ptr  : event callback function to be registered
  ps_errno                      : return the specific error

RETURN VALUE
   0 : on success
  -1 : on failure (any of the passed in parameters is bad)

DEPENDENCIES
  flow_ptr must be valid and it must be associated to another flow_ptr

SIDE EFFECTS
  None
===========================================================================*/
int32 ps_flowi_assoc_flow_ev_cback_reg
(
  ps_flow_type                        * flow_ptr,
  ps_flow_assoc_flow_event_cback_type   assoc_flow_event_cback_f_ptr,
  int16                               * ps_errno
)
{
  ps_flow_type             * assoc_flow_ptr;
  boolean                    is_default_flow;
  uint8                      event_arr_index = 0;
  ps_iface_event_enum_type   ps_flow_events [FLOW_MAX_EV - FLOW_MIN_EV];
  ps_iface_error_enum_type   ps_error_info = PS_IFACE_SUCCESS;
  int                        retval = 0;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    assoc_flow_ptr = PS_FLOWI_GET_ASSOC_PS_FLOW(flow_ptr);
    DS_UTILS_ASSERT(assoc_flow_ptr != NULL && assoc_flow_ptr != flow_ptr);

    is_default_flow =
      PS_FLOW_GET_CAPABILITY(assoc_flow_ptr, PS_FLOW_CAPABILITY_DEFAULT);

    /*-------------------------------------------------------------------------
      Use default if no callback function is supplied
      -------------------------------------------------------------------------*/
    if (assoc_flow_event_cback_f_ptr == NULL)
    {
      assoc_flow_event_cback_f_ptr = ps_flowi_assoc_flow_ev_cback;
    }

#ifdef FEATURE_DATA_REFLECTIVE_QOS
    LOG_MSG_INFO1_3("Registering for flow event flow_ptr 0x%x"
                      " assoc_flow_ptr 0x%x is_default_flow %d",
                      flow_ptr, assoc_flow_ptr,  is_default_flow);
#endif /* FEATURE_DATA_REFLECTIVE_QOS */


    /*-------------------------------------------------------------------------
      Register only for appropriate events on the default flow.
    -------------------------------------------------------------------------*/
    if ( ! is_default_flow)
    {
      ps_flow_events[event_arr_index++] = FLOW_ACTIVATED_EV;
      ps_flow_events[event_arr_index++] = FLOW_SUSPENDED_EV;
      ps_flow_events[event_arr_index++] = FLOW_NULL_EV;
      ps_flow_events[event_arr_index++] = FLOW_INFO_CODE_UPDATED_EV;
      ps_flow_events[event_arr_index++] = FLOW_MODIFY_ACCEPTED_EV;
      ps_flow_events[event_arr_index++] = FLOW_MODIFY_REJECTED_EV;
#ifdef FEATURE_DATA_REFLECTIVE_QOS
      ps_flow_events[event_arr_index++] = FLOW_REFLECTIVE_QOS_FILTER_UPDATED_EV;
#endif /* FEATURE_DATA_REFLECTIVE_QOS */
    }

    ps_flow_events[event_arr_index++] = FLOW_TX_ENABLED_EV;
    ps_flow_events[event_arr_index++] = FLOW_TX_DISABLED_EV;

    flow_ptr->flow_private.logical_flow.ps_flow_ev_cback_buf = 
      ps_flow_alloc_event_handle(assoc_flow_ptr,
  	                         assoc_flow_event_cback_f_ptr,
  	                         flow_ptr);

    if (NULL == flow_ptr->flow_private.logical_flow.ps_flow_ev_cback_buf)
    {
      ps_error_info = PS_IFACE_ERROR_HANDLE_ALLOC_FAILED;
      *ps_errno = DS_ENOMEM;
      break;
    }

    retval = ps_flow_subscribe_event_list
             (
  	       flow_ptr->flow_private.logical_flow.ps_flow_ev_cback_buf,
  	       ps_flow_events,
  	       event_arr_index
  	     );
    if (0 != retval)
    {
      ps_flow_free_event_handle
      (
        flow_ptr->flow_private.logical_flow.ps_flow_ev_cback_buf
      );

      flow_ptr->flow_private.logical_flow.ps_flow_ev_cback_buf = NULL;
      ps_error_info = PS_IFACE_ERROR_EVENT_REG_FAILED;
      *ps_errno = DS_EINVAL;
      break;
    }

    return 0;
  } while(0);

  LOG_MSG_ERROR_INT_1("ps_flowi_assoc_flow_ev_cback_reg(): Error %d ",
  	                  ps_error_info);
  return -1;

} /* ps_flowi_assoc_flow_ev_cback_reg() */

void ps_flowi_assoc_flow_ev_cback_dereg
(
  ps_flow_type  * flow_ptr
)
{
  ps_flow_type               * assoc_flow_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_1("ps_flowi_assoc_flow_ev_cback_dereg(): "
                           "flow 0x%p", flow_ptr);

  assoc_flow_ptr = PS_FLOWI_GET_ASSOC_PS_FLOW(flow_ptr);
  if (assoc_flow_ptr == NULL || !PS_FLOW_IS_VALID(assoc_flow_ptr))
  {
    DS_UTILS_ASSERT(0);
    LOG_MSG_FUNCTION_EXIT_1("ps_flowi_assoc_flow_ev_cback_dereg(): "
                            "Fail, flow 0x%p", flow_ptr);
    return;
  }

  if (NULL != flow_ptr->flow_private.logical_flow.ps_flow_ev_cback_buf)
  {
    if (PS_FLOW_IS_VALID(assoc_flow_ptr) && 
  	  PS_FLOWI_GET_COOKIE(assoc_flow_ptr)
          == PS_FLOWI_GET_ASSOC_COOKIE(flow_ptr))
    {
      /*------------------------------------------------------------------
        Note: Free will take care of unsubcribe also.Directly use
        free_event_handle when all the events are to be unsubscribed. 
      -------------------------------------------------------------------*/
      ps_flow_free_event_handle
      (
        flow_ptr->flow_private.logical_flow.ps_flow_ev_cback_buf
      );
      flow_ptr->flow_private.logical_flow.ps_flow_ev_cback_buf = NULL;
    }
  }

  LOG_MSG_FUNCTION_EXIT_1("ps_flowi_assoc_flow_ev_cback_dereg(): "
                          "Success, flow 0x%p", flow_ptr);

} /* ps_flowi_assoc_flow_ev_cback_dereg */

void ps_flowi_process_assoc_flow_ev
(
  dcc_cmd_enum_type   cmd,
  void              * user_data_ptr
)
{
#ifdef FEATURE_DATA_PS_QOS
  ps_flow_modify_param_type                modify_param;
  ps_logical_flow_assoc_flow_ev_cmd_type * assoc_flow_cmd_ptr;
  ps_iface_event_info_u_type               event_info;
  ps_flow_type                           * flow_ptr;
  ps_flow_type                           * assoc_flow_ptr;
  int32                                    ret_val;
  ps_iface_event_enum_type                 event;
  int16                                    ps_errno;
  uint8                                    logical_flow_cookie;
#ifdef FEATURE_DATA_REFLECTIVE_QOS
  ps_iface_ipfltr_handle_type              fltr_add_handle = 
                                             PS_IFACE_IPFLTR_INVALID_HANDLE;   
#endif /* FEATURE_DATA_REFLECTIVE_QOS */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2("ps_flowi_process_assoc_flow_ev(): "
                           "cmd %d called with user_data_ptr 0x%p",
                           cmd, user_data_ptr);

  if (user_data_ptr == NULL)
  {
    LOG_MSG_INVALID_INPUT_0("ps_flowi_process_assoc_flow_ev(): "
                            "NULL DS cmd ptr");
    return;
  }

  if (cmd != DCC_LOGICAL_FLOW_ASSOC_FLOW_EV_CMD)
  {
    LOG_MSG_INVALID_INPUT_1("ps_flowi_process_assoc_flow_ev(): "
                            "Invalid DCC cmd id %d", cmd);
    PS_SYSTEM_HEAP_MEM_FREE(user_data_ptr);
    return;
  }

  assoc_flow_cmd_ptr = (ps_logical_flow_assoc_flow_ev_cmd_type*) user_data_ptr;

  assoc_flow_ptr      = assoc_flow_cmd_ptr->assoc_flow_ptr;
  event               = assoc_flow_cmd_ptr->event;
  event_info          = assoc_flow_cmd_ptr->event_info;
  logical_flow_cookie = assoc_flow_cmd_ptr->logical_flow_cookie;
  flow_ptr            =(ps_flow_type *) (assoc_flow_cmd_ptr->user_data_ptr);

  PS_SYSTEM_HEAP_MEM_FREE(user_data_ptr);

  LOG_MSG_INFO1_3("ps_flowi_process_assoc_flow_ev(): "
                  "Processing event, %d on assoc_ps_flow 0x%p for flow 0x%p",
                  event, assoc_flow_ptr, flow_ptr);

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  if (!PS_FLOW_IS_VALID(flow_ptr))
  {
    LOG_MSG_INFO1_2("ps_flowi_process_assoc_flow_ev(): "
                    "Ignoring event %d as flow 0x%p is invalid",
                    event, flow_ptr);
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return;
  }
  else if (PS_FLOWI_GET_COOKIE(flow_ptr) != logical_flow_cookie)
  {
    LOG_MSG_INFO1_2("ps_flowi_process_assoc_flow_ev(): "
                    "Ignoring event %d as flow 0x%p is re-allocated",
                    event, flow_ptr);
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return;
  }

  /*-------------------------------------------------------------------------
    Make sure that logical flow is associated to assoc_flow_ptr
  -------------------------------------------------------------------------*/
  if (PS_FLOWI_GET_ASSOC_PS_FLOW(flow_ptr) != assoc_flow_ptr)
  {
    LOG_MSG_ERROR_3("ps_flowi_process_assoc_flow_ev(): "
                    "Flow 0x%p is not associated to flow 0x%p but to 0x%p",
                    flow_ptr,
                    assoc_flow_ptr,
                    PS_FLOWI_GET_ASSOC_PS_FLOW(flow_ptr));
    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
    return;
  }

  /*-------------------------------------------------------------------------
    Ignore all events except for FLOW_NULL_EV as it is possible that
    associated flow is alread deleted by the time DCC task is scheduled to
    process events on assoc flow.

    Associated flow is not deleted only if it still valid and if cookie matches
  -------------------------------------------------------------------------*/
  if (event != FLOW_NULL_EV)
  {
    if (!PS_FLOW_IS_VALID(assoc_flow_ptr))
    {
      LOG_MSG_INFO1_3("ps_flowi_process_assoc_flow_ev(): "
                      "Ignoring event %d on flow 0x%p as assoc flow 0x%p is "
                      "already deleted", event, flow_ptr, assoc_flow_ptr);
      PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      return;
    }
    else if (PS_FLOWI_GET_COOKIE(assoc_flow_ptr) !=
             PS_FLOWI_GET_ASSOC_COOKIE(flow_ptr))
    {
      LOG_MSG_INFO1_3("ps_flowi_process_assoc_flow_ev(): "
                      "Ignoring event %d on flow 0x%p as assoc flow 0x%p is "
                      "re-allocated", event, flow_ptr, assoc_flow_ptr);
      PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
      return;
    }
  }

  /*-------------------------------------------------------------------------
    Process solid state events by posting corresponding indications etc.
  -------------------------------------------------------------------------*/
  switch (event)
  {
    case FLOW_ACTIVATED_EV:
      ps_flow_activate_ind(flow_ptr, event_info.phys_link_event_info.info_code);
      break;

    case FLOW_NULL_EV:
      ps_flow_go_null_ind(flow_ptr, event_info.phys_link_event_info.info_code);
      if (ps_iface_delete_flow(flow_ptr->flow_private.iface_ptr,
                               flow_ptr,
                               &ps_errno) != 0)
      {
        LOG_MSG_ERROR_1("ps_flowi_process_assoc_flow_ev(): "
                        "Could not delete PS flow 0x%p", flow_ptr);
      }

      break;

    case FLOW_SUSPENDED_EV:
      ps_flow_suspend_ind(flow_ptr, event_info.phys_link_event_info.info_code);
      break;

    case FLOW_INFO_CODE_UPDATED_EV:
      ps_flow_generic_ind(flow_ptr,
                          FLOW_INFO_CODE_UPDATED_EV,
                          &(event_info.phys_link_event_info.info_code));
      break;

    case FLOW_MODIFY_ACCEPTED_EV:
      if (!PS_FLOWI_IS_QOS_MODIFY_IN_PROGRESS(flow_ptr))
      {
        LOG_MSG_INFO1_1("ps_flowi_process_assoc_flow_ev(): "
                        "Network inited qos modify accept 0x%p", flow_ptr);

        /*---------------------------------------------------------------------
          Copy the modify info pointer from the underlying assoc flow
        ---------------------------------------------------------------------*/
        memset(&modify_param, 0, sizeof(ps_flow_modify_param_type));
        ret_val = ps_flowi_get_modify_qos_spec_from_flow
                  (
                    flow_ptr,
                    &modify_param.qos_spec,
                    &ps_errno
                  );

        if (0 != ret_val)
        {
          LOG_MSG_INFO1_2("ps_flowi_process_assoc_flow_ev(): "
                          "Fail ps_flowi_get_qos_spec_type_from_flow, flow 0x%p,"
                          "errno %d", flow_ptr, ps_errno);
          break;
        }

        ret_val = ps_iface_modify_flow(flow_ptr->flow_private.iface_ptr,
                                       flow_ptr,
                                       &modify_param,
                                       &ps_errno);

        ps_iface_logical_flowi_free_qos_spec(&(modify_param.qos_spec));

        if (0 != ret_val)
        {
          LOG_MSG_INFO1_2("ps_flowi_process_assoc_flow_ev(): "
                          "Fail ps_iface_modify_flow, flow 0x%p, errno %d",
                          flow_ptr, ps_errno);
          break;
        }
      }

      if (ps_iface_modify_flow_accepted(flow_ptr->flow_private.iface_ptr,
                                        flow_ptr,
                                        event_info.flow_event_info.info_code,
                                        &ps_errno ) != 0 )
      {
        LOG_MSG_ERROR_1("ps_flowi_process_assoc_flow_ev(): "
                        "Could not modify PS flow 0x%p", flow_ptr);
      }

      break;

    case FLOW_MODIFY_REJECTED_EV:
      if (PS_FLOWI_IS_QOS_MODIFY_IN_PROGRESS(flow_ptr))
      {
          if (ps_iface_modify_flow_rejected
              (
                flow_ptr->flow_private.iface_ptr,
                flow_ptr,
                event_info.flow_event_info.info_code,
                &ps_errno
              ) != 0 )
          {
            LOG_MSG_ERROR_1("ps_flowi_process_assoc_flow_ev(): "
                            "Could not modify PS flow 0x%p", flow_ptr);
            DS_UTILS_ASSERT(0);
          }
      }

      break;
      
#ifdef FEATURE_DATA_REFLECTIVE_QOS
    case FLOW_REFLECTIVE_QOS_FILTER_UPDATED_EV:
    {
      event_info.reflective_qos_fltr_info.flow_ptr = flow_ptr;
      
      fltr_add_handle = ps_ifacei_install_reflective_qos_fltr
                       (
                         flow_ptr->flow_private.iface_ptr,
                         &(event_info.reflective_qos_fltr_info)
                       );                       
      if (PS_IFACE_IPFLTR_INVALID_HANDLE == fltr_add_handle)
      {
        LOG_MSG_ERROR_INT_0("ps_flowi_process_assoc_flow_ev : "
                        "Filter add failed");  
        ret_val = -1;   
        break;   
      } 
      PS_FLOWI_SET_TX_FLTR_HANDLE(flow_ptr, fltr_add_handle);                                              
      break;  
    }      
#endif /* FEATURE_DATA_REFLECTIVE_QOS */      

    case FLOW_TX_ENABLED_EV:
      ps_flow_enable_tx(flow_ptr, DS_FLOW_PROXY_MASK);
      break;

    case FLOW_TX_DISABLED_EV:
      ps_flow_disable_tx(flow_ptr, DS_FLOW_PROXY_MASK);
      break;

    case FLOW_FLTR_AUX_INFO_UPDATED_EV:
      ret_val = ps_iface_logical_flowi_process_fltr_aux_info_updated_ev
                (
                  assoc_flow_ptr->flow_private.iface_ptr,
                  assoc_flow_ptr,
                  flow_ptr,
                  &ps_errno
                );

      if (0 != ret_val)
      {
        LOG_MSG_ERROR_2("ps_flowi_process_assoc_flow_ev(): "
                        "ps_iface_logical_flowi_process_fltr_aux_info_updated_ev() "
                        "failed, PS flow 0x%p err %d", flow_ptr, ps_errno);
      }

      break;

    default:
      DS_UTILS_ASSERT(0);
      LOG_MSG_ERROR_INT_1("ps_flowi_process_assoc_flow_ev(): "
                      "Invalid event %d", event);
      break;
  } /* switch (event) */

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
#endif /* FEATURE_DATA_PS_QOS */

  return;
} /* ps_flowi_process_assoc_flow_ev() */

/**
  @brief This function is used to allocate the memory used to register for ps_flow
  event callbacks.
  Once no longer needed it should be freed using ps_flow_free_event_handle 
 
  @param 
    flow_ptr[in]  ptr to flow on which to operate on
    event_cback_f_ptr[in] the callback to be registered
    user_data_ptr[in]  data to be called with the callback
   
  @return
     NULL if allocation failed
     ptr to memory being allocated
  
  @dependencies 
    None
*/
void *ps_flow_alloc_event_handle
(
  ps_flow_type              * flow_ptr,
  ps_flow_event_cback_type    event_cback_f_ptr,
  void                      * user_data_ptr
)
{
  ps_flowi_event_handle_type  *handle_ptr = NULL;
  q_type                      *working_q_ptr = NULL;
  ps_flowi_event_cb_info_type  cb_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (event_cback_f_ptr == NULL)
  {
    return NULL;
  }
  if ((flow_ptr != NULL) && !(PS_FLOW_IS_VALID(flow_ptr)))
  {
    return NULL; //ASSERT?? 
  }

  /*-------------------------------------------------------------------------
    Check that a handle with same cb fn ptr and user_data_ptr doesnt already
    exist.
  -------------------------------------------------------------------------*/
  cb_info.event_cback_f_ptr = event_cback_f_ptr;
  cb_info.user_data_ptr = user_data_ptr;

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    If this registration is for global callbacks, then use the global
    variable as the working pointer, otherwise use the q from the flow
  -------------------------------------------------------------------------*/
  if (flow_ptr == NULL)
  {
    working_q_ptr = &global_flow_event_handle_q;
  }
  else 
  {
    working_q_ptr = 
     &(flow_ptr->flow_private.event_handle_q);
  }
  do
  {
    if ( NULL != q_linear_search( working_q_ptr,
                                ps_flowi_compare_event_handle,
                                &cb_info ) )
    {
      LOG_MSG_ERROR_2("ps_flow_alloc_event_handle: already present cb_fn_ptr 0x%p, user_data_ptr 0x%p",
                      event_cback_f_ptr, user_data_ptr);
      break;
    }

    /*-------------------------------------------------------------------------
      Allocate a new event callback buffer.
    -------------------------------------------------------------------------*/
    PS_SYSTEM_HEAP_MEM_ALLOC(handle_ptr, sizeof(ps_flowi_event_handle_type),
                             ps_flowi_event_handle_type *);

    if (handle_ptr == NULL)
    {
      break;
    }

    /*-------------------------------------------------------------------------
      Initialize the event buffer
    -------------------------------------------------------------------------*/
    memset(handle_ptr,0,sizeof(ps_flowi_event_handle_type));
    (void) q_link(handle_ptr, &(handle_ptr->link));
    handle_ptr->event_cback_f_ptr = event_cback_f_ptr;
    handle_ptr->user_data_ptr     = user_data_ptr;
    handle_ptr->flow_ptr         = flow_ptr;

    /* Enqueue the empty buffer */
    q_put(working_q_ptr, &(handle_ptr->link));
  } while (0);

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  return handle_ptr;

} /* ps_flow_alloc_event_handle() */


/**
  @brief This function is used to free the memory used to register for ps_flow
  event callbacks 
 
  @param 
    event_handle[in]  the event handle to be freed
   
  @return
     None
  
  @dependencies 
    ps_flow_alloc_event_handle should have been called
*/
void ps_flow_free_event_handle
(
  void *evt_handle
)
{
  ps_flowi_event_handle_type* event_handle = NULL;
  q_type                    * working_q_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (evt_handle == NULL)
  {
    ASSERT(0);
    return;
  }

  event_handle = (ps_flowi_event_handle_type*)evt_handle;
  /*-------------------------------------------------------------------------
    Validate the buffer that is passed in is on a queue
  -------------------------------------------------------------------------*/
  if (!Q_ALREADY_QUEUED(&(event_handle->link)))
  {
    ASSERT(0);
    return;
  }
  /*-------------------------------------------------------------------------
    Choose appropriate queue based on whether callback is global or per iface
  -------------------------------------------------------------------------*/
  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
  if (event_handle->flow_ptr == NULL)
  {
    working_q_ptr = &global_flow_event_handle_q;
  }
  else 
  {
    working_q_ptr = 
     &(event_handle->flow_ptr->flow_private.event_handle_q);
  }

  /* Delete from the queue */
  q_delete(working_q_ptr, &(event_handle->link));

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  /*-------------------------------------------------------------------------
    Invalidate the buffer so that the caller cannot use it without
    explicitly allcating it again.
  -------------------------------------------------------------------------*/
  memset(event_handle, 0, sizeof(ps_flowi_event_handle_type));

  PS_SYSTEM_HEAP_MEM_FREE(event_handle);

} /* ps_flow_free_event_handle() */


/**
  @brief Subscribe to a list of events on a flow.  Whenever an event in the list 
  occurs, the callback for that event is called.  If an event in
  the list is true when registration happens the callback is 
  called immediately.  This will fail if the event handle (with 
  callback and user data pointer) is NULL.  The same event 
  handle can be used to subscribe to an additional set of events. If an 
  event in the list had previously been subscribed to, it will 
  be a NO-OP. 
 
  @param 
    event_handle[in]  Event handle that holds the callback
    pointer. Should not be NULL.
    events[in]  Array of events that triggers the callbacks
    num_events[in]  Number of elements in the array
   
  @return
     0 on success
     -1 on failure
  
  @dependencies 
    Event handle MUST be allocated using
    ps_flow_alloc_event_handle()
*/
int ps_flow_subscribe_event_list
(
  void                     *evt_handle,
  ps_iface_event_enum_type *events,
  uint8                     num_events 

)
{
  ps_flowi_event_handle_type *  event_handle = NULL, *first_event_handle = NULL;
  ps_iface_event_info_u_type    event_info;
  boolean                       need_to_invoke_cback = FALSE;
  boolean                       is_priority_flow_event_reg = FALSE;
  uint8                         itr;
  q_type                      * working_q_ptr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (evt_handle == NULL || events == NULL || num_events == 0)
  {
    ASSERT(0);
    return -1;
  }

  event_handle = (ps_flowi_event_handle_type*)evt_handle;
  /*-------------------------------------------------------------------------
    Validate the buffer that is passed in is on a queue
  -------------------------------------------------------------------------*/
  if (!Q_ALREADY_QUEUED(&(event_handle->link)))
  {
    ASSERT(0);
    return -1;
  }

  /* Validate all events in the list */
  for (itr=0; itr<num_events; itr++)
  {
    if (events[itr] < FLOW_MIN_EV || events[itr] >= FLOW_MAX_EV)
    {

      LOG_MSG_ERROR_INT_1("ps_flow_subscribe_event_list(): "
                      "Invalid Flow event %d to register for ps_flow",
                      events[itr]);
      return -1;
    }
  }

  /*-------------------------------------------------------------------------
    As the contents of the buffer are supposed to be set by ps_flow
    functions it should NEVER have a NULL callback
  -------------------------------------------------------------------------*/
  if (event_handle->event_cback_f_ptr == NULL)
  {
    ASSERT(0);
    return -1;
  }

  /*-------------------------------------------------------------------------
    if not the global queue then make sure that the interface is valid
  -------------------------------------------------------------------------*/
  if (event_handle->flow_ptr != NULL &&
      !PS_FLOW_IS_VALID(event_handle->flow_ptr))
  {
    LOG_MSG_ERROR_1("ps_flow_subscribe_event_list(): "
                    "Invalid flow in event handle, 0x%p", event_handle->flow_ptr);
    return -1;
  }


  for (itr=0; itr<num_events; itr++)
  {
    /* NO-OP if event is already set in bitmask */
    if (ps_flowi_is_event_set(event_handle, events[itr]))
    {
      continue;
    }

    ps_flowi_set_event_bit(event_handle, events[itr]);

    if (events[itr] == FLOW_TX_ENABLED_EV || events[itr] == FLOW_TX_DISABLED_EV)
    {
      is_priority_flow_event_reg = TRUE;
    }

    /* For global registation no need to invoke cback */
    if (event_handle->flow_ptr == NULL)
    {
      continue;
    }

    need_to_invoke_cback = FALSE;

    /*-------------------------------------------------------------------------
      Initializing "event_info" 
    -------------------------------------------------------------------------*/
    memset(&event_info, 0, sizeof(event_info));
    event_info.flow_event_info.state = FLOW_STATE_INVALID;

    /*-------------------------------------------------------------------------
      check if the event is true - if so call the callback immediately
    -------------------------------------------------------------------------*/
    switch (events[itr])
    {
      case FLOW_NULL_EV:
      if (PS_FLOWI_GET_STATE(event_handle->flow_ptr) == FLOW_NULL)
      {
        event_info.flow_event_info.state     = FLOW_NULL;
        event_info.flow_event_info.info_code = PS_EIC_NOT_SPECIFIED;
        need_to_invoke_cback                 = TRUE;
      }
      break;

    case FLOW_ACTIVATING_EV:
      if (PS_FLOWI_GET_STATE(event_handle->flow_ptr) == FLOW_ACTIVATING)
      {
        event_info.flow_event_info.state     = FLOW_ACTIVATING;
        event_info.flow_event_info.info_code = PS_EIC_NOT_SPECIFIED;
        need_to_invoke_cback                 = TRUE;
      }
      break;

    case FLOW_CONFIGURING_EV:
      if (PS_FLOWI_GET_STATE(event_handle->flow_ptr) == FLOW_CONFIGURING)
      {
        event_info.flow_event_info.state     = FLOW_CONFIGURING;
        event_info.flow_event_info.info_code = PS_EIC_NOT_SPECIFIED;
        need_to_invoke_cback                 = TRUE;
      }
      break;

    case FLOW_ACTIVATED_EV:
      if (PS_FLOWI_GET_STATE(event_handle->flow_ptr) == FLOW_ACTIVATED)
      {
        event_info.flow_event_info.state     = FLOW_ACTIVATED;
        event_info.flow_event_info.info_code = PS_EIC_NOT_SPECIFIED;
        need_to_invoke_cback                 = TRUE;
      }
      break;

    case FLOW_SUSPENDING_EV:
      if (PS_FLOWI_GET_STATE(event_handle->flow_ptr) == FLOW_SUSPENDING)
      {
        event_info.flow_event_info.state     = FLOW_SUSPENDING;
        event_info.flow_event_info.info_code = PS_EIC_NOT_SPECIFIED;
        need_to_invoke_cback                 = TRUE;
      }
      break;

    case FLOW_SUSPENDED_EV:
      if (PS_FLOWI_GET_STATE(event_handle->flow_ptr) == FLOW_SUSPENDED)
      {
        event_info.flow_event_info.state     = FLOW_SUSPENDED;
        event_info.flow_event_info.info_code = PS_EIC_NOT_SPECIFIED;
        need_to_invoke_cback                 = TRUE;
      }
      break;

    case FLOW_RESUMING_EV:
      if (PS_FLOWI_GET_STATE(event_handle->flow_ptr) == FLOW_RESUMING)
      {
        event_info.flow_event_info.state     = FLOW_RESUMING;
        event_info.flow_event_info.info_code = PS_EIC_NOT_SPECIFIED;
        need_to_invoke_cback                 = TRUE;
      }
      break;

    case FLOW_GOING_NULL_EV:
      if (PS_FLOWI_GET_STATE(event_handle->flow_ptr) == FLOW_GOING_NULL)
      {
        event_info.flow_event_info.state     = FLOW_GOING_NULL;
        event_info.flow_event_info.info_code = PS_EIC_NOT_SPECIFIED;
        need_to_invoke_cback                 = TRUE;
      }
      break;

    case FLOW_TX_ENABLED_EV:
      if (PS_FLOWI_IS_TX_ENABLED(event_handle->flow_ptr))
      {
        event_info.flow_mask = DS_FLOW_IS_ENABLED;
        need_to_invoke_cback = TRUE;
      }
      break;

    case FLOW_TX_DISABLED_EV:
      if (!PS_FLOWI_IS_TX_ENABLED(event_handle->flow_ptr))
      {
        event_info.flow_mask = PS_FLOWI_GET_TX_MASK(event_handle->flow_ptr);
        need_to_invoke_cback = TRUE;
      }
      break;

    default:
      break;

    } /* switch(event) */

    /*-------------------------------------------------------------------------
      If the callback needs to be called, do so
    -------------------------------------------------------------------------*/
    if (need_to_invoke_cback)
    {
      event_handle->event_cback_f_ptr(event_handle->flow_ptr,
                                      events[itr],
                                      event_info,
                                      event_handle->user_data_ptr);
    }
  } /* for loop around event list*/

  /* Move the event_handle to the head of the queue if a priority flow event has been
     registered for. */
  if (is_priority_flow_event_reg)
  {

    /*-------------------------------------------------------------------------
      Choose appropriate queue based on whether callback is global or per iface
    -------------------------------------------------------------------------*/
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
    if (event_handle->flow_ptr == NULL)
    {
      working_q_ptr = &global_flow_event_handle_q;
    }
    else 
    {
      working_q_ptr = 
       &(event_handle->flow_ptr->flow_private.event_handle_q);
    }

    first_event_handle = q_check(working_q_ptr);
    if ((q_cnt(working_q_ptr) > 1) && (first_event_handle != event_handle))
    {
      /* Delete from the queue */
      q_delete(working_q_ptr, &(event_handle->link));
      /* Add to head of the queue */
      q_insert(working_q_ptr,  &(event_handle->link), &(first_event_handle->link));
    }

    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);
  }

  return 0;

} /* ps_flow_subscribe_event_list() */

/**
  @brief Unsubscribe from a list of events on a flow. This will
  fail if the event handle (with callback and user data pointer) is NULL.
  If an event in the list had not previously been subcribed to, it will be a NO-OP.
  If an event handle is no longer needed, it should be freed using
  ps_flow_free_event_handle 
 
  @param 
    event_handle[in]  Event handle that holds the callback
    pointer. Should not be NULL.
    events[in]  Array of events to unsubscribe from
    num_events[in]  Number of elements in the array
   
  @return
     0 on success
     -1 on failure
  
  @dependencies 
    ps_flow_alloc_event_handle should have been called
*/
int ps_flow_unsubscribe_event_list
(
  void                     *evt_handle,
  ps_iface_event_enum_type *events,
  uint8                     num_events 
)
{
  q_type                      * working_q_ptr = NULL;
  ps_flowi_event_handle_type  * event_handle = NULL, * last_event_handle = NULL;
  boolean                    is_priority_flow_event_unreg = FALSE;
  uint8                      itr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (evt_handle == NULL || events == NULL || num_events == 0)
  {
    ASSERT(0);
    return -1;
  }

  event_handle = (ps_flowi_event_handle_type*)evt_handle;
  /*-------------------------------------------------------------------------
    Validate the buffer that is passed in is on a queue
  -------------------------------------------------------------------------*/
  if (!Q_ALREADY_QUEUED(&(event_handle->link)))
  {
    ASSERT(0);
    return -1;
  }

  /* Validate all events in the list */
  for (itr=0; itr<num_events; itr++)
  {
    if (events[itr] < FLOW_MIN_EV || events[itr] >= FLOW_MAX_EV)
    {

      LOG_MSG_ERROR_INT_1("ps_flow_unsubscribe_event_list(): "
                      "Invalid Flow event %d to de-register for ps_flow",
                      events[itr]);
      return -1;
    }
  }

  /*-------------------------------------------------------------------------
    As the contents of the buffer are supposed to be set by ps_flow
    functions it should NEVER have a NULL callback
  -------------------------------------------------------------------------*/
  if (event_handle->event_cback_f_ptr == NULL)
  {
    ASSERT(0);
    return -1;
  }

  /*-------------------------------------------------------------------------
    if not the global queue then make sure that the interface is valid
  -------------------------------------------------------------------------*/
  if (event_handle->flow_ptr != NULL &&
      !PS_FLOW_IS_VALID(event_handle->flow_ptr))
  {
    LOG_MSG_ERROR_1("ps_flow_unsubscribe_event_list(): "
                    "Invalid flow in event handle, 0x%p", event_handle->flow_ptr);
    return -1;
  }

  for (itr=0; itr<num_events; itr++)
  {
    LOG_MSG_INFO2_1("ps_flow_unsubscribe_event_list(): Dereg for event %d",
                    events[itr]);
    /* NO-OP if event is already unset in bitmask */
    if (!ps_flowi_is_event_set(event_handle, events[itr]))
    {
      continue;
    }

    ps_flowi_unset_event_bit(event_handle, events[itr]);
    /* Make sure both flow events are not registered for */
    if (((events[itr] == FLOW_TX_ENABLED_EV) &&
        (!ps_flowi_is_event_set(event_handle, FLOW_TX_DISABLED_EV))) ||
        ((events[itr] == FLOW_TX_DISABLED_EV) &&
         (!ps_flowi_is_event_set(event_handle, FLOW_TX_ENABLED_EV))))
    {
      is_priority_flow_event_unreg = TRUE;
    }
  }
  /*-------------------------------------------------------------------------
    Move the event_handle to the tail of the queue if flow events have been
    de-registered from
  -------------------------------------------------------------------------*/
  if (is_priority_flow_event_unreg)
  {
    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
    if (event_handle->flow_ptr == NULL)
    {
      working_q_ptr = &global_flow_event_handle_q;
    }
    else 
    {
      working_q_ptr = 
       &(event_handle->flow_ptr->flow_private.event_handle_q);
    }

    last_event_handle = q_last_check(working_q_ptr);
    if ((q_cnt(working_q_ptr) > 1) && (last_event_handle != event_handle))
    {
      /* Delete from the queue */
      q_delete(working_q_ptr, &(event_handle->link));
      /* Add to tail of the queue */
      q_insert_after(working_q_ptr, &(event_handle->link), &(last_event_handle->link));
    }

    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  }
  return 0;

} /* ps_flow_unsubscribe_event_list() */


