/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                     G B A   E V E N T   F U N C T I O N S


GENERAL DESCRIPTION

  This header file contains the GBA functions which handle event notifications.

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
09/03/15   yt      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "gba_lib.h"
#include "gba.h"

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION GBA_EVT_STORE_CLIENT_INFO

DESCRIPTION
  Function creates an entry in the client_info_tbl and stores the client event
  callback info. The entry is automatically removed when an event is sent to
  that client.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  gba_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
gba_result_enum_type gba_evt_store_client_info (
  gba_session_type              session_type,
  gba_bsf_enum_type             bsf_type,
  gba_evt_callback_type         evt_callback,
  uint32                        evt_user_data
);

/*===========================================================================
FUNCTION GBA_EVT_SEND_KEY_EVENT

DESCRIPTION
  Sends the key events to registered clients. The exclude_evt_cb parameter
  is passed if that client callback should not receive this event. For example,
  in case of KEY_REPLACED_EVT, we don't want to send the event to the client
  that has requested forced bootstrapping resulting in replacement
  of the key.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  gba_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
gba_result_enum_type gba_evt_send_key_event (
  gba_session_type          session_type,
  gba_bsf_enum_type         bsf_type,
  gba_key_event_enum_type   evt_type,
  gba_evt_callback_type     exclude_evt_cb
);

/*===========================================================================
FUNCTION GBA_EVT_SEND_KEY_EVENT_FOR_SESSION

DESCRIPTION
  Sends the key event to registered clients for the session type

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void gba_evt_send_key_event_for_session (
  gba_session_type         session_type,
  gba_key_event_enum_type  evt_type
);

