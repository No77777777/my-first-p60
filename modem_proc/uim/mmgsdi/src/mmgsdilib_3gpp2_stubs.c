/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                 M M G S D I L I B    3 G P P 2     S T U B S


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
04/19/17   nc      Initial revision 
==============================================================================*/ 

#include "mmgsdilib.h"
#include "mmgsdi.h"
#include "mmgsdiutil.h"


/*===========================================================================
FUNCTION MMGSDI_GET_ESN_ME_CHANGE_FLAG

DESCRIPTION
  This function will return the esn ME change flag obtained during post pin1
  init

PARAMS:
  none

DEPENDENCIES:

LIMITATIONS:

RETURN VALUE:
   mmgsdi_esn_me_change_enum_type:
     MMGSDI_ESN_ME_UNKNOWN
     MMGSDI_ESN_ME_NO_CHANGE
     MMGSDI_ESN_ME_CHANGE
     MMGSDI_ESN_ME_UPDATE_PENDING
     MMGSDI_ESN_ME_CHANGE_NOT_SUPPORTED

SIDE EFFECTS:
===========================================================================*/
mmgsdi_esn_me_change_enum_type mmgsdi_get_esn_me_change_flag (
  mmgsdi_slot_id_enum_type slot
)
{
  (void)slot;
  return MMGSDI_ESN_ME_CHANGE_NOT_SUPPORTED;
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
  mmgsdi_client_data_type          client_ref)
{
  (void)card_slot;
  (void)hide_csim;
  (void)response_cb_ptr;
  (void)client_ref;
  return MMGSDI_NOT_SUPPORTED;
} /* mmgsdi_hide_csim */


/* ===========================================================================
   FUNCTION: MMGSDI_RECEIVE_HIDDEN_CSIM_EVENTS
=========================================================================== */
/*
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
  (void)client_id;
  (void)receive_hidden_csim_events;
  return MMGSDI_NOT_SUPPORTED;
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
  (void)slot_id;
  (void)csim_hiding_status_ptr;
  return MMGSDI_NOT_SUPPORTED;
} /* mmgsdi_csim_hiding_status */

