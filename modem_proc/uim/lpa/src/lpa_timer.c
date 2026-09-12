/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


            L P A   T I M E R   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the functions for setting/clearing timers.

                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2019 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================
                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_timer.c#3 $ $DateTime: 2019/12/27 02:07:35 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/11/19   vgd     Increase sanity timer value to 7 mins 
12/07/16   av      LPA phase2 initial changes
10/06/16   av      Introduce DS http post retry timer logic
04/11/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "rex.h"

#include "lpa.h"
#include "lpa_platform.h"
#include "lpa_timer.h"
#include "lpa_cmd.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

#define LPA_SANITY_TIMER_VALUE                           420000 /* 7 minutes */
#define LPA_GET_BPP_RETRY_TIMER_VALUE                    120000 /* 2 minutes */
#define LPA_HANDLE_INSTALLATION_RESULT_RETRY_TIMER_VALUE  60000 /* 1 minute */

static rex_timer_type                      lpa_sanity_timer[LPA_NUM_SLOTS];
/* One http retry timer is enough for both GET_BPP and HANDLE_INSTALLATION_
   RESULT as both these commands cannot be in progress at same time */
static rex_timer_type                      lpa_http_retry_timer[LPA_NUM_SLOTS];

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION LPA_TIMER_CLEANUP

DESCRIPTION
  Undefines the various LPA timers.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_timer_cleanup (
  lpa_slot_id_enum_type          slot_id
)
{
  if(slot_id < LPA_NUM_SLOTS)
  {
    (void)rex_undef_timer(&lpa_sanity_timer[slot_id]);
    (void)rex_undef_timer(&lpa_http_retry_timer[slot_id]);
  }
} /* lpa_timer_cleanup */


/*===========================================================================
FUNCTION LPA_TIMER_CLEAR

DESCRIPTION
  Function clears the timer that is passed in as argument.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_timer_clear (
  lpa_slot_id_enum_type          slot_id,
  lpa_timer_enum_type            timer_type
)
{
  if(slot_id >= LPA_NUM_SLOTS)
  {
    return;
  }
  switch(timer_type)
  {
    case LPA_TIMER_TYPE_SANITY_TIMER:
      (void)rex_clr_timer(&lpa_sanity_timer[slot_id]);
      break;
    case LPA_TIMER_TYPE_GET_BPP_RETRY_TIMER:
    case LPA_TIMER_TYPE_HANDLE_INSTALLATION_RESULT_RETRY_TIMER:
      (void)rex_clr_timer(&lpa_http_retry_timer[slot_id]);
      break;
    default:
      break;
  }
} /* lpa_timer_clear */


/*===========================================================================
FUNCTION LPA_TIMER_SET

DESCRIPTION
  Function sets the timer that is passed in as argument.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_timer_set (
  lpa_slot_id_enum_type          slot_id,
  lpa_timer_enum_type            timer_type
)
{
  uint32                 timer_sig = LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_1;

  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  switch(timer_type)
  {
    case LPA_TIMER_TYPE_SANITY_TIMER:
      if(slot_id == LPA_SLOT_1)
      {
        timer_sig = LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_1;
      }
      else if(slot_id == LPA_SLOT_2)
      {
        timer_sig = LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_2;
      }
      else
      {
        return LPA_GENERIC_ERROR;
      }
      (void)rex_clr_sigs(rex_self(), timer_sig);
      (void)rex_set_timer(&lpa_sanity_timer[slot_id], LPA_SANITY_TIMER_VALUE);
      break;
    case LPA_TIMER_TYPE_GET_BPP_RETRY_TIMER:
    case LPA_TIMER_TYPE_HANDLE_INSTALLATION_RESULT_RETRY_TIMER:
      if(slot_id == LPA_SLOT_1)
      {
        timer_sig = LPA_HTTP_RETRY_TIMER_EXPIRE_SIG_SLOT_1;
      }
      else if(slot_id == LPA_SLOT_2)
      {
        timer_sig = LPA_HTTP_RETRY_TIMER_EXPIRE_SIG_SLOT_2;
      }
      else
      {
        return LPA_GENERIC_ERROR;
      }
      (void)rex_clr_sigs(rex_self(), timer_sig);
      if(timer_type == LPA_TIMER_TYPE_GET_BPP_RETRY_TIMER)
      {
        (void)rex_set_timer(&lpa_http_retry_timer[slot_id],
                            LPA_GET_BPP_RETRY_TIMER_VALUE);
      }
      else
      {
        (void)rex_set_timer(&lpa_http_retry_timer[slot_id],
                            LPA_HANDLE_INSTALLATION_RESULT_RETRY_TIMER_VALUE);
      }
      break;
    default:
      return LPA_GENERIC_ERROR;
  }

  return LPA_SUCCESS;
} /* lpa_timer_set */


/*===========================================================================
FUNCTION LPA_TIMER_GET

DESCRIPTION
  Function gets the current countdown value of the timer, that is passed in
  as argument, in milliseconds.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_timer_get (
  lpa_slot_id_enum_type          slot_id,
  lpa_timer_enum_type            timer_type,
  uint32                         *timer_cnt_ptr
)
{
  if(slot_id >= LPA_NUM_SLOTS)
  {
    return LPA_GENERIC_ERROR;
  }

  if(timer_cnt_ptr == NULL)
  {
    return LPA_ERROR_INCORRECT_PARAMS;
  }

  switch(timer_type)
  {
    case LPA_TIMER_TYPE_SANITY_TIMER:
      *timer_cnt_ptr = (uint32) rex_get_timer(&lpa_sanity_timer[slot_id]);
      break;
    case LPA_TIMER_TYPE_GET_BPP_RETRY_TIMER:
    case LPA_TIMER_TYPE_HANDLE_INSTALLATION_RESULT_RETRY_TIMER:
      *timer_cnt_ptr = (uint32) rex_get_timer(&lpa_http_retry_timer[slot_id]);
      break;
    default:
      return LPA_GENERIC_ERROR;
  }

  LPA_MSG_MED_3("lpa_timer_get for timer_type=0x%x, lpa_slot=0x%x, remaining_time=0x%x",
                timer_type, slot_id, *timer_cnt_ptr );

  return LPA_SUCCESS;
} /* lpa_timer_get */


/*===========================================================================
FUNCTION LPA_TIMER_HANDLE_SANITY_TIMER_EXPIRE_SIG

DESCRIPTION
  This function, called in lpa_task, is called to process sanity timer
  expiration signal for a given slot.

DEPENDENCIES
  lpa_task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_timer_handle_sanity_timer_expire_sig (
  lpa_slot_id_enum_type               slot_id
)
{
  uint32                 timer_sig   = LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_1;
  lpa_message_enum_type  current_cmd = LPA_GET_PROFILES_INFO_MSG;

  /* Send the response and clear the timer */
  LPA_MSG_HIGH_0("LPA_SANITY_TIMER_EXPIRE_SIG");

  if(slot_id == LPA_SLOT_1)
  {
    timer_sig = LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_1;
  }
  else if(slot_id == LPA_SLOT_2)
  {
    timer_sig = LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_2;
  }
  else
  {
    return;
  }

   /* Clear the signal */
  (void)rex_clr_sigs(rex_self(), timer_sig);

  if(LPA_SUCCESS == lpa_get_current_cmd(slot_id, &current_cmd)) 
  {
    if(current_cmd == LPA_ADD_PROFILE_MSG) 
    {
      /* For ADD_PROFILE, what is pending is the profile download
         event. So, send the indication instead of sending a
         response for ADD_PROFILE to the client */
      lpa_event_data_type              download_progress_event;

      memset(&download_progress_event, 0x00, sizeof(download_progress_event));
      download_progress_event.evt_type = LPA_PROFILE_DOWNLOAD_PROGRESS_EVENT;
      download_progress_event.slot_id = slot_id;
      download_progress_event.evt_data.profile_download_progress.percent_downloaded =
        0;
      download_progress_event.evt_data.profile_download_progress.installation_complete =
        FALSE;

      lpa_send_event(LPA_ERROR_COMMAND_TIMEOUT,
                     &download_progress_event,
                     slot_id,
                     TRUE);
    }
    else
    {
      lpa_send_response(LPA_ERROR_COMMAND_TIMEOUT,
                        NULL,
                        slot_id,
                        TRUE);
    }
  }
} /* lpa_timer_handle_sanity_timer_expire_sig */


/*===========================================================================
FUNCTION LPA_TIMER_INIT

DESCRIPTION
  Function initializes and defines the various LPA timers.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_timer_init (
  void
)
{
  rex_def_timer(&lpa_sanity_timer[LPA_SLOT_1], UIM_LPA_TCB,
                LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_1);
  rex_def_timer(&lpa_sanity_timer[LPA_SLOT_2], UIM_LPA_TCB,
                LPA_SANITY_TIMER_EXPIRE_SIG_SLOT_2);
  rex_def_timer(&lpa_http_retry_timer[LPA_SLOT_1], UIM_LPA_TCB,
                LPA_HTTP_RETRY_TIMER_EXPIRE_SIG_SLOT_1);
  rex_def_timer(&lpa_http_retry_timer[LPA_SLOT_2], UIM_LPA_TCB,
                LPA_HTTP_RETRY_TIMER_EXPIRE_SIG_SLOT_2);
} /* lpa_timer_init */

