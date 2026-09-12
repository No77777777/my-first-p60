/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 M M G S D I L I B    3 G P P 2     F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the utility functions needed in the MMGSDI module

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


                        COPYRIGHT INFORMATION

Copyright (c) 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*===========================================================================
                        EDIT HISTORY FOR MODULE



when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
05/23/17   nr      DTF handlers for session activation and de-activation
04/19/17   nc      Initial revision 
==============================================================================*/ 

#include "mmgsdilib.h"
#include "mmgsdi.h"
#include "mmgsdiutil.h"
#include "mmgsdilib_p.h"

/*===========================================================================
  FUNCTION: MMGSDI_GET_ESN_ME_CHANGE_FLAG

  This function will return the esn ME change flag obtained during post pin1
  init

  @param[in]: slot

  @dependencies:
    none

  @limitations:
    none

  @return:
   mmgsdi_esn_me_change_enum_type:
     MMGSDI_ESN_ME_UNKNOWN
     MMGSDI_ESN_ME_NO_CHANGE
     MMGSDI_ESN_ME_CHANGE
     MMGSDI_ESN_ME_UPDATE_PENDING
     MMGSDI_ESN_ME_CHANGE_NOT_SUPPORTED

  @sideeffects:
    none
===========================================================================*/
mmgsdi_esn_me_change_enum_type mmgsdi_get_esn_me_change_flag (
  mmgsdi_slot_id_enum_type  slot
)
{
  mmgsdi_slot_data_type          *slot_data_ptr   = NULL;
  mmgsdi_esn_me_change_enum_type  esn_me_cng_flag = MMGSDI_ESN_ME_CHANGE_NOT_SUPPORTED;

  /* Protect access to slot data table */
  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot);
  if(slot_data_ptr != NULL)
  {
    esn_me_cng_flag = slot_data_ptr->esn_me_change_flag;
  }

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  return esn_me_cng_flag;
}/* mmgsdi_get_esn_me_change_flag */


/* ===========================================================================
   FUNCTION: MMGSDI_HIDE_CSIM

  This function provides support for hiding CSIMs from modem clients until
  * the device is reset
  * hotswap
  * This function is called again to stop hiding CSIMs from modem clients

  @param[in] card_slot:       Slot on which the CSIMs should be hidden
  @param[in] hide_csim:       When TRUE, stop hinding the CSIMs by faking
                              session activation for all 1X sessions that were
                              active before this function was called to hide
                              the CSIMs.
                              When FALSE, hide CSIMs by faking that it is
                              disabled.
  @param[in] response_cb_ptr: Response callback
  @param[in] client_ref:      User Data returned upon
                              completion of this cmd.

  @dependencies
  None

  @limitations
  None

  @return
  MMGSDI_SUCCESS : Operation is successful
  MMGSDI_ERROR   : Otherwise

  @sideeffects
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_hide_csim (
  mmgsdi_slot_id_enum_type         card_slot,
  boolean                          hide_csim,
  mmgsdi_callback_type             response_cb_ptr,
  mmgsdi_client_data_type          client_ref
)
{
  mmgsdi_task_cmd_type                  *task_cmd_ptr   = NULL;
  mmgsdi_hide_csim_req_type             *msg_ptr        = NULL;
  mmgsdi_return_enum_type                mmgsdi_status  = MMGSDI_SUCCESS;

  MMGSDIUTIL_MEM_MALLOC_AND_VALIDATE(task_cmd_ptr, sizeof(mmgsdi_task_cmd_type));
  if(task_cmd_ptr == NULL)
  {
    return MMGSDI_MEMORY_ERROR_HEAP_EXHAUSTED;
  }

  msg_ptr = &task_cmd_ptr->cmd.cmd.hide_csim_req;

  /*---------------------------------------------------------------------------
    Populating request header
      1) Client ID
      2) Request TYPE
      3) Client Data Pointer
      4) Response CallBack
  ---------------------------------------------------------------------------*/
  task_cmd_ptr->cmd.cmd_enum                = MMGSDI_HIDE_CSIM_REQ;
  msg_ptr->request_header.client_id         = mmgsdi_get_global_client_id();
  msg_ptr->request_header.request_type      = MMGSDI_HIDE_CSIM_REQ;
  msg_ptr->request_header.orig_request_type = MMGSDI_HIDE_CSIM_REQ;
  msg_ptr->request_header.slot_id           = card_slot;
  msg_ptr->request_header.client_data       = client_ref;
  msg_ptr->request_header.response_cb       = response_cb_ptr;

  msg_ptr->hide_csim = hide_csim;

  /* Put on mmgsdi command queue */
  mmgsdi_status = mmgsdi_cmd(task_cmd_ptr);
  if (mmgsdi_status != MMGSDI_SUCCESS)
  {
    /* Free the pointer since the task_cmd_ptr has not been put onto the command queue */
    MMGSDIUTIL_TMC_MEM_FREE(task_cmd_ptr);
  }

  UIM_MSG_HIGH_2("Queue of MMGSDI command: MMGSDI_HIDE_CSIM_REQ (0x%x) status 0x%x",
                 msg_ptr->hide_csim, mmgsdi_status);

  return mmgsdi_status;
} /* mmgsdi_hide_csim */


/* ===========================================================================
   FUNCTION: MMGSDI_RECEIVE_HIDDEN_CSIM_EVENTS

  This function enables clients to receive hidden CSIM events. The operation
  is performed synchronously.

  @param[in] client_id:                     Client Id of the caller
  @param[in] receive_hidden_csim_events:    When TRUE, do not hide CSIM
                                            events for the client

  @dependencies
  None

  @limitations
  None

  @return
  MMGSDI_SUCCESS : Operation is successful
  MMGSDI_ERROR   : Otherwise

  @sideeffects
  None
===========================================================================*/
mmgsdi_return_enum_type mmgsdi_receive_hidden_csim_events(
  mmgsdi_client_id_type    client_id,
  boolean                  receive_hidden_csim_events
)
{
  uint8  i = 0;

  if(client_id == MMGSDI_CLIENT_ID_ZERO)
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  for (i = 0; i < MMGSDI_MAX_CLIENT_INFO; i++)
  {
    if (mmgsdi_client_id_reg_table[i] == NULL ||
        mmgsdi_client_id_reg_table[i]->client_id != client_id)
    {
      continue;
    }

    UIM_MSG_LOW_1("receive_hidden_csim_events 0x%x", receive_hidden_csim_events);
    mmgsdi_client_id_reg_table[i]->receive_hidden_csim_events = receive_hidden_csim_events;
    break;
  }

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  if (i == MMGSDI_MAX_CLIENT_INFO)
  {
    return MMGSDI_ERROR;
  }

  return MMGSDI_SUCCESS;
} /* mmgsdi_receive_hidden_csim_events */


/* ============================================================================
FUNCTION MMGSDI_CSIM_HIDING_STATUS

DESCRIPTION
  This function tells whether the CSIM is currently hidden from modem clients,
  or not.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  mmgsdi_return_enum_type

SIDE EFFECTS
  None
============================================================================*/
mmgsdi_return_enum_type mmgsdi_csim_hiding_status(
  mmgsdi_slot_id_enum_type             slot_id,
  mmgsdi_csim_hiding_status_enum_type  *csim_hiding_status_ptr
)
{
  mmgsdi_slot_data_type               *slot_data_ptr      = NULL;
  mmgsdi_return_enum_type              mmgsdi_status      = MMGSDI_ERROR;

  if(csim_hiding_status_ptr == NULL ||
     !mmgsdi_util_is_slot_valid(slot_id))
  {
    return MMGSDI_INCORRECT_PARAMS;
  }

  MMGSDIUTIL_ENTER_CLIENT_APP_DATA_CRIT_SECT;

  slot_data_ptr = mmgsdi_util_get_slot_data_ptr(slot_id);

  if(slot_data_ptr)
  {
    mmgsdi_status = MMGSDI_SUCCESS;
    *csim_hiding_status_ptr = slot_data_ptr->csim_hiding_status;

    UIM_MSG_HIGH_1("CSIM hiding status is = 0x%x", *csim_hiding_status_ptr);
  }

  MMGSDIUTIL_LEAVE_CLIENT_APP_DATA_CRIT_SECT;

  return mmgsdi_status;
} /* mmgsdi_csim_hiding_status */

