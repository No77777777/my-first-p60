/*===========================================================================

               I P C    R O U T E R   X A L  G L I N K  R E X

        This file provides Rex OS specific functionality of the glink
                          XAL for the IPC Router

  ---------------------------------------------------------------------------
  Copyright (c) 2014 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  ---------------------------------------------------------------------------
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/mproc/ipc_router/src/ipc_router_xal_glink_rex.c#1 $
$DateTime: 2019/04/24 00:03:26 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/06/14   aep     Initial creation
==========================================================================*/

/*===========================================================================
                          INCLUDE FILES
===========================================================================*/
#include "ipc_router_types.h"
#include "ipc_router_xal_glink_common.h"
#include "rex.h"

/*===========================================================================
                  CONSTANT / MACRO DECLARATIONS
===========================================================================*/

/*===========================================================================
                        PRIVATE TYPE DEFINITIONS 
===========================================================================*/

/** The OS specific private structure of the glink port */
struct ipc_router_xal_glink_os_priv_s {
  /* Has the worker started? */
  boolean                 worker_started;
  /* holder to handle signals before the worker has started */
  rex_sigs_type           pre_sigs;
  /* TCB of the worker thread */
  rex_tcb_type            tcb;
  /* stack of the worker */
  rex_stack_pointer_type  rex_stack;
  /* Name of the worker thread */
  char                    name[REX_TASK_NAME_LEN];
};

/*===========================================================================
                        LOCAL FUNCTIONS
===========================================================================*/

/*===========================================================================
FUNCTION      ipc_router_xal_glink_worker_thread

DESCRIPTION   The Worker thread dedicated to handle RX and TX events
              on this GLINK port.

              priv - Private parameter (port)

RETURN VALUE  None

SIDE EFFECTS  This function never returns.
===========================================================================*/
static void ipc_router_xal_glink_worker_thread(rex_task_func_param_type priv)
{
  ipc_router_xal_glink_port_type *port = (ipc_router_xal_glink_port_type *)priv;
  rex_sigs_type got_sigs;
  rex_sigs_type wait_sigs = EVENT_MASK(IPC_ROUTER_XAL_GLINK_EVENT_LAST) - 1;

  /* Handle events which may have arrived before the worker was
   * started */
  ipc_router_os_sem_lock(&port->lock);
  port->os_priv->worker_started = TRUE;
  got_sigs = port->os_priv->pre_sigs;
  ipc_router_os_sem_unlock(&port->lock);

  while(1)
  {
    ipc_router_xal_glink_handle_events(port, (unsigned int)got_sigs);

    /* Exit the worker thread */
    if(got_sigs & EVENT_MASK(IPC_ROUTER_XAL_GLINK_EVENT_CLOSED))
    {
      port->os_priv->worker_started = FALSE;
      break;
    }

    got_sigs = rex_wait(wait_sigs);
    rex_clr_sigs(rex_self(), got_sigs);
  }
}

/*===========================================================================
                        EXPORTED FUNCTIONS
===========================================================================*/

/*===========================================================================
FUNCTION      ipc_router_xal_glink_os_init

DESCRIPTION   Initializes the OS Private structure

              port - The port structure for this GLINK interface.

RETURN VALUE  IPC_ROUTER_STATUS_SUCCESS on success,
              error code on failure.
===========================================================================*/
int ipc_router_xal_glink_os_init
(
  ipc_router_xal_glink_port_type *port
)
{
  ipc_router_xal_glink_os_priv_type *os_priv;

  os_priv = ipc_router_os_calloc(1, sizeof(*os_priv));
  if(!os_priv)
  {
    return IPC_ROUTER_STATUS_NO_MEM;
  }

  /* Do not set signals on worker when it has not been started */
  os_priv->worker_started = FALSE;
  port->os_priv = os_priv;

  /* 0 size stack implies that we will be operating in a threadless fashion
   * Hijacking the callback functions of GLink. Else we need to create
   * the stack for the worker thread */
  if(port->params.stack_size > 0)
  {
    os_priv->rex_stack = (rex_stack_pointer_type)ipc_router_os_malloc(port->params.stack_size);
    if(!os_priv->rex_stack)
    {
      ipc_router_os_free(os_priv);
      return IPC_ROUTER_STATUS_NO_MEM;
    }
    strlcpy(os_priv->name, "ipcr_", sizeof(os_priv->name));
    strlcat(os_priv->name, port->params.remote_ss, sizeof(os_priv->name));
    os_priv->name[REX_TASK_NAME_LEN - 1] = '\0';

    rex_def_task_ext(&port->os_priv->tcb, 
                    port->os_priv->rex_stack,
                    port->params.stack_size,
                    (rex_priority_type)port->params.priority,
                    ipc_router_xal_glink_worker_thread,
                    (rex_task_func_param_type)port,
                    port->os_priv->name,
                    FALSE);
  }
  else
  {
    os_priv->worker_started = TRUE;
  }

  return IPC_ROUTER_STATUS_SUCCESS;
}

/*===========================================================================
FUNCTION      ipc_router_xal_glink_os_deinit

DESCRIPTION   De-initialize the OS specific elements associated with this port.

              port - The port structure for this GLINK interface.

RETURN VALUE  None

SIDE EFFECTS  Blocks till the worker thread has exited.
===========================================================================*/
void ipc_router_xal_glink_os_deinit
(
  ipc_router_xal_glink_port_type *port
)
{
  if(!port || !port->os_priv)
  {
    return;
  }

  /* Assumes that the common code has closed the port and set the course
   * to let the CLOSED event be set and the wroker to exit. If we are
   * operating in a thread-less fashion, then this will wait till GLink
   * has successfully cleaned up and called the local-disconnect cb */
  while(port->os_priv->worker_started == TRUE)
  {
    rex_sleep(50);
  }
  if(port->params.stack_size > 0)
  {
    rex_kill_task(&port->os_priv->tcb);
    ipc_router_os_free((void *)port->os_priv->rex_stack);
  }
  ipc_router_os_free(port->os_priv);
  port->os_priv = NULL;
}

/*===========================================================================
FUNCTION      ipc_router_xal_glink_set_event

DESCRIPTION   Signal the worker to handle an event

              port - The port structure for this GLINK interface.
              event - The event to be handled

RETURN VALUE  None

SIDE EFFECTS  None
===========================================================================*/
void ipc_router_xal_glink_set_event
(
  ipc_router_xal_glink_port_type *port, 
  ipc_router_xal_glink_event_type event
)
{
  /* If the worker has not started, just set the pre_mask
   * and return */
  ipc_router_os_sem_lock(&port->lock);
  if(port->os_priv->worker_started == FALSE)
  {
    port->os_priv->pre_sigs |= EVENT_MASK(event);
    ipc_router_os_sem_unlock(&port->lock);
    return;
  }
  ipc_router_os_sem_unlock(&port->lock);

  if(port->params.priority > 0)
  {
    rex_set_sigs(&port->os_priv->tcb, EVENT_MASK(event));
  }
  else
  {
    ipc_router_xal_glink_handle_events(port, EVENT_MASK(event));
    if(event == IPC_ROUTER_XAL_GLINK_EVENT_CLOSED)
    {
      port->os_priv->worker_started = FALSE;
    }
  }
}

