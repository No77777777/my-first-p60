/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            G B A   T I M E R   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the GBA timing functions.

                        COPYRIGHT INFORMATION

Copyright (c) 2015 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/16/15   yt      Support for second BSF and key status events
03/27/15   tl      Add support for GBA cancel
01/26/15   tl      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "rex.h"

#include "gba.h"
#include "gba_bootstrap.h"
#include "gba_cmd.h"
#include "gba_lib.h"
#include "gba_platform.h"
#include "gba_response.h"
#include "gba_state.h"
#include "gba_timer.h"
#include "gba_lifetime.h"
#include "gba_util.h"
#include "gba_evt.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

#define GBA_SANITY_TIMER_VALUE             30000 /* 30 seconds */

/* Derive session and BSF indices from the key validity timer index */
#define GBA_TIMER_INDEX_TO_SESSION_AND_BSF_INDEX(timer_index, session_index, bsf_index) \
  session_index = ((timer_index) >> 16) & 0xFF;                                         \
  bsf_index = (timer_index) & 0xFF;

/* Compose index of key validity timer from session and BSF indices */
#define GBA_SESSION_AND_BSF_INDEX_TO_TIMER_INDEX(session_index, bsf_index, timer_index) \
  timer_index = ((session_index) << 16) + (bsf_index);

/* Timer used to determine the expiration of a GBA transaction,
   from the beginning to the moment the callback is sent to the client. */
static rex_timer_type                      gba_sanity_timer;

/* Timer for the validity of the Ks for each session and BSF. */
static rex_timer_type                      gba_key_validity_timer[GBA_MAX_SESSIONS][GBA_MAX_NUM_BSF];

static q_type                              gba_timer_cmd_q;

typedef struct {
  q_link_type                      link;
  uint32                           timer_index;
} gba_timer_cmd_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION GBA_TIMER_CLEANUP

DESCRIPTION
  Undefines the GBA sanity timer.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gba_timer_cleanup (
  void
)
{
  (void)rex_undef_timer(&gba_sanity_timer);
} /* gba_timer_cleanup */


/*===========================================================================
FUNCTION GBA_CLEAR_TIMER

DESCRIPTION
  Function clears the GBA sanity timer.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gba_clear_timer (
  void
)
{
  (void)rex_clr_timer(&gba_sanity_timer);
} /* gba_clear_timer */


/*===========================================================================
FUNCTION GBA_SET_TIMER

DESCRIPTION
  Function sets the GBA sanity timer.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gba_set_timer (
  void
)
{
  (void)rex_clr_sigs(rex_self(), GBA_SANITY_TIMER_EXPIRE_SIG);

  (void)rex_set_timer(&gba_sanity_timer, GBA_SANITY_TIMER_VALUE);
} /* gba_set_timer */


/*===========================================================================
FUNCTION GBA_HANDLE_SANITY_TIMER_EXPIRE_SIG

DESCRIPTION
  This function, called in gba_task, is called to process sanity timer
  expiration signal.

DEPENDENCIES
  gba_task must be finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gba_handle_sanity_timer_expire_sig (
  void
)
{
  GBA_MSG_HIGH_0("GBA_SANITY_TIMER_EXPIRE_SIG");

  /* Clear the signal */
  (void)rex_clr_sigs(rex_self(), GBA_SANITY_TIMER_EXPIRE_SIG);

  gba_send_response(GBA_TIMEOUT,
                    NULL);

  gba_reset();

  /* Set the signal if there are one or more commands still in queue
     and GBA is currently in IDLE state. */
  gba_cmd_set_sig();
} /* gba_handle_sanity_timer_expire_sig */


/*===========================================================================
FUNCTION GBA_CLEAR_KEY_VALIDITY_TIMER

DESCRIPTION
  Function clears the GBA key validity timer.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gba_clear_key_validity_timer (
  gba_session_type       session_type,
  gba_bsf_enum_type      bsf_type
)
{
  uint8                    session_index    = GBA_MAX_SESSIONS;
  uint8                    bsf_index        = gba_util_bsf_type_to_bsf_index(bsf_type);
  timer_attrib_struct_type timer_attributes = {0};
  timer_error_type         timer_status     = TE_SUCCESS;

  if(gba_util_get_session_index_and_mmgsdi_session_type(
       session_type, &session_index, NULL) != GBA_SUCCESS)
  {
    return;
  }

  timer_status = timer_get_params(&gba_key_validity_timer[session_index][bsf_index],
                                  &timer_attributes);
  if(timer_status == TE_SUCCESS &&
     timer_attributes.timer_state != TIMER_UNDEF_FLAG)
  {
    rex_clr_timer(&gba_key_validity_timer[session_index][bsf_index]);
    rex_undef_timer(&gba_key_validity_timer[session_index][bsf_index]);
  }
} /* gba_clear_key_validity_timer */


/*===========================================================================
FUNCTION GBA_CLEAR_KEY_VALIDITY_TIMER_FOR_SESSION

DESCRIPTION
  Function clears the GBA key validity timer for the session type.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gba_clear_key_validity_timer_for_session (
  gba_session_type       session_type
)
{
  gba_bsf_enum_type bsf_type = GBA_BSF_DEFAULT;

  for(bsf_type = GBA_BSF_DEFAULT; bsf_type <= GBA_BSF_NON_DEFAULT; bsf_type++)
  {
    gba_clear_key_validity_timer(session_type, bsf_type);
  }
} /* gba_clear_key_validity_timer_for_session */


/*===========================================================================
FUNCTION GBA_HANDLE_KEY_VALIDITY_TIMER_EXPIRE_SIG

DESCRIPTION
  This function, called in gba_task, is called to process key validity timer
  expiration signal.

DEPENDENCIES
  gba_task must be finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gba_handle_key_validity_timer_expire_sig (
  void
)
{
  gba_timer_cmd_type          * task_cmd_ptr  = NULL;
  uint8                         session_index = 0;
  uint8                         bsf_index     = 0;
  gba_bsf_enum_type             bsf_type      = GBA_BSF_DEFAULT;
  gba_session_type              session_type  = GBA_SESSION_MAX;

  task_cmd_ptr = (gba_timer_cmd_type*)(q_get(&gba_timer_cmd_q));

  /* If there is a command on either of the queues */
  if(task_cmd_ptr != NULL)
  {
    GBA_MSG_HIGH_1("GBA_KEY_VALIDITY_TIMER_EXPIRE_SIG received, timer index 0x%x",
                   task_cmd_ptr->timer_index);

    GBA_TIMER_INDEX_TO_SESSION_AND_BSF_INDEX(task_cmd_ptr->timer_index,
                                             session_index,
                                             bsf_index);

    if((gba_util_session_index_to_session_type(session_index,
                                              &session_type) == GBA_SUCCESS) &&
       (gba_util_bsf_index_to_bsf_type(bsf_index, &bsf_type) == GBA_SUCCESS))
    {
      (void)gba_evt_send_key_event(session_type,
                                   bsf_type,
                                   GBA_KEY_EXPIRED_EVT,
                                   NULL);

      /* Undefine the timer on expiry */
      gba_clear_key_validity_timer(session_type, bsf_type);
    }

    GBA_FREE(task_cmd_ptr);
  }

  (void) rex_clr_sigs(rex_self(), GBA_KEY_VALIDITY_TIMER_EXPIRE_SIG);

  /* Set the signal if there are one or more commands to look at */
  if (q_cnt(&gba_timer_cmd_q) > 0)
  {
    /*  Items on the queue. Set the signal */
    (void) rex_set_sigs(rex_self(), GBA_KEY_VALIDITY_TIMER_EXPIRE_SIG);
  }
} /* gba_handle_key_validity_timer_expire_sig */


/*===========================================================================
FUNCTION GBA_KEY_VALIDITY_TIMER_EXPIRY_CB

DESCRIPTION
  Callback for key validity timer

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void gba_key_validity_timer_expiry_cb(
  uint32         timer_index)
{
  gba_timer_cmd_type  * gba_timer_cmd_ptr = NULL;

  GBA_ENTER_TASK_STATE_CRIT_SECT;

  /* Do not queue command if GBA task has been stopped */
  if(gba_task_stopped == FALSE)
  {
    gba_timer_cmd_ptr = (gba_timer_cmd_type *)GBA_MALLOC(sizeof(gba_timer_cmd_type));
    if(gba_timer_cmd_ptr != NULL)
    {
      gba_timer_cmd_ptr->timer_index = timer_index;

      (void)q_link(gba_timer_cmd_ptr, &gba_timer_cmd_ptr->link);

      /* Put the message on the queue */
      q_put(&gba_timer_cmd_q, &gba_timer_cmd_ptr->link);

      /* Set the command queue signal */
      (void) rex_set_sigs(UIM_GBA_TCB, GBA_KEY_VALIDITY_TIMER_EXPIRE_SIG);
    }
  }
  else
  {
    GBA_MSG_ERR_0("GBA task stopped, cannot queue command");
  }

  GBA_LEAVE_TASK_STATE_CRIT_SECT;
} /* gba_key_validity_timer_expiry_cb */


/*===========================================================================
FUNCTION GBA_SET_KEY_VALIDITY_TIMER

DESCRIPTION
  Function sets the GBA key validity timer.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gba_set_key_validity_timer (
  gba_session_type       session_type,
  gba_bsf_enum_type      bsf_type,
  gba_lifetime_type      lifetime
)
{
  uint8  session_index     = GBA_MAX_SESSIONS;
  uint8  bsf_index         = gba_util_bsf_type_to_bsf_index(bsf_type);
  uint32 key_validity_time = 0;
  uint32 timer_index       = 0;

  if((gba_util_get_session_index_and_mmgsdi_session_type(session_type,
                                                         &session_index,
                                                         NULL) != GBA_SUCCESS) ||
     session_index >= GBA_MAX_SESSIONS)
  {
    return;
  }

  key_validity_time = gba_lifetime_get_remaining_time(lifetime);

  if(key_validity_time > 0)
  {
    /* Clear the timer first if it was set previously. */
    gba_clear_key_validity_timer(session_type, bsf_type);

    GBA_SESSION_AND_BSF_INDEX_TO_TIMER_INDEX(session_index,
                                             bsf_index,
                                             timer_index);

    /* Define and start the timer. */
    rex_def_timer_ex (&gba_key_validity_timer[session_index][bsf_index],
                      gba_key_validity_timer_expiry_cb,
                      timer_index);
    (void)rex_set_timer(&gba_key_validity_timer[session_index][bsf_index],
                        key_validity_time * 1000);
  }
} /* gba_set_key_validity_timer */


/*===========================================================================
FUNCTION GBA_TIMER_INIT

DESCRIPTION
  Function initializes and defines the GBA sanity timer.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gba_timer_init (
  void
)
{
  rex_def_timer (&gba_sanity_timer, UIM_GBA_TCB, GBA_SANITY_TIMER_EXPIRE_SIG);

  (void) q_init(&gba_timer_cmd_q);
} /* gba_timer_init */

