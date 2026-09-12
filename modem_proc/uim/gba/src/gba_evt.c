/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*


                     G B A   E V E N T   F U N C T I O N S


GENERAL DESCRIPTION

  This source file contains the GBA functions which handle event notifications.

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
#include "uim_variation.h"

#include "gba.h"
#include "gba_lib.h"
#include "gba_platform.h"
#include "gba_timer.h"
#include "gba_storage.h"
#include "gba_util.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/

/* Max number of client entries with event callback info */
#define GBA_MAX_CLIENT_INFO              16

typedef struct {
  gba_session_type         session_type;
  gba_bsf_enum_type        bsf_type;
  gba_evt_callback_type    evt_callback;
  uint32                   user_data;
} gba_evt_client_info_type;

static gba_evt_client_info_type * gba_evt_client_info_tbl[GBA_MAX_CLIENT_INFO];


/*=============================================================================

                       FUNCTION DEFINITIONS

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
)
{
  uint8 client_index = 0;

  for(client_index = 0; client_index < GBA_MAX_CLIENT_INFO; client_index++)
  {
    if(gba_evt_client_info_tbl[client_index] == NULL)
    {
      break;
    }
  }

  if(client_index == GBA_MAX_CLIENT_INFO)
  {
    GBA_MSG_ERR_0("Unable to store GBA client event callback");
    return GBA_GENERIC_ERROR;
  }

  gba_evt_client_info_tbl[client_index] = GBA_MALLOC(
     sizeof(gba_evt_client_info_type));
  if(gba_evt_client_info_tbl[client_index] == NULL)
  {
    return GBA_GENERIC_ERROR;
  }

  gba_evt_client_info_tbl[client_index]->session_type = session_type;
  gba_evt_client_info_tbl[client_index]->bsf_type     = bsf_type;
  gba_evt_client_info_tbl[client_index]->evt_callback = evt_callback;
  gba_evt_client_info_tbl[client_index]->user_data    = evt_user_data;

  return GBA_SUCCESS;
} /* gba_evt_store_client_info */


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
  gba_session_type         session_type,
  gba_bsf_enum_type        bsf_type,
  gba_key_event_enum_type  evt_type,
  gba_evt_callback_type    exclude_evt_cb
)
{
  uint8                           i                = 0;
  uint8                           bsf_index        = gba_util_bsf_type_to_bsf_index(bsf_type);
  gba_event_data_type             evt_data         = {0};
  gba_storage_profile_data_type * profile_data_ptr = NULL;

  profile_data_ptr = gba_storage_get_profile(session_type);
  if(profile_data_ptr == NULL)
  {
    return GBA_GENERIC_ERROR;
  }

  for(i = 0; i < GBA_MAX_CLIENT_INFO; i++)
  {
    if(gba_evt_client_info_tbl[i] == NULL ||
       gba_evt_client_info_tbl[i]->evt_callback == NULL ||
       gba_evt_client_info_tbl[i]->session_type != session_type ||
       gba_evt_client_info_tbl[i]->bsf_type != bsf_type ||
       gba_evt_client_info_tbl[i]->evt_callback == exclude_evt_cb)
    {
      continue;
    }

    memset(&evt_data, 0x00, sizeof(evt_data));
    evt_data.session_type = session_type;
    evt_data.evt          = evt_type;
    evt_data.user_data    = gba_evt_client_info_tbl[i]->user_data;
    evt_data.btid_ptr     = &profile_data_ptr->bootstrap_info[bsf_index].btid;
    evt_data.aid_ptr      = &profile_data_ptr->aid;

    GBA_MSG_MED_3("sending event 0x%x to cb 0x%x with user data 0x%x",
                  evt_type,
                  gba_evt_client_info_tbl[i]->evt_callback,
                  gba_evt_client_info_tbl[i]->user_data);

    gba_evt_client_info_tbl[i]->evt_callback(&evt_data);

    /* Free the entry after the key event is sent as GBA will not maintain
       the key that the client previously received and the client is expected
       to request bootstrapping (and register for events) again if it is
       interested in a key. */
    GBA_FREE(gba_evt_client_info_tbl[i]);
  }

  gba_storage_free_profile(&profile_data_ptr);

  return GBA_SUCCESS;
}/* gba_evt_send_key_event */


/*===========================================================================
FUNCTION GBA_EVT_SEND_KEY_EVENT_FOR_SESSION

DESCRIPTION
  Sends the key event to registered clients for the given session for all
  BSF types.

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
)
{
  gba_bsf_enum_type bsf_type     = GBA_BSF_DEFAULT;

  for(bsf_type = GBA_BSF_DEFAULT; bsf_type <= GBA_BSF_NON_DEFAULT; bsf_type++)
  {
    gba_evt_send_key_event(session_type,
                           bsf_type,
                           evt_type,
                           NULL);
  }
} /* gba_evt_send_key_event_for_session */

