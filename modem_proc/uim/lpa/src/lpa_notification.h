#ifndef LPA_NOTIFICATION_H
#define LPA_NOTIFICATION_H
/*===========================================================================


            L P A   N O T I F I C A T I O N   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2016, 2021 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/lpa/src/lpa_notification.h#2 $ $DateTime: 2021/02/08 01:08:56 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/21/21   dj      Fixing compilation errors
12/07/16   av      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "lpalib.h"
#include "lpa.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/
/* ----------------------------------------------------------------------------
   ENUM:      LPA_NOTIFICATION_CMD_ENUM_TYPE

   DESCRIPTION:
     Indicates the notification command rseponses received from MMGSDI or HTTP.
-------------------------------------------------------------------------------*/
typedef enum {
  LPA_NOTIFICATION_NONE_CMD,
  LPA_NOTIFICATION_RETRIEVE_NOTIFICATION_LIST_CMD,
  LPA_NOTIFICATION_SEND_NOTIFICATION_CMD,
  LPA_NOTIFICATION_REMOVE_NOTIFICATION_FROM_LIST_CMD,
  LPA_NOTIFICATION_CMD_ENUM_MAX                        = 0x7FFFFFFF
} lpa_notification_cmd_enum_type;

/* ----------------------------------------------------------------------------
   ENUM:      LPA_NOTIFICATION_TYPE_ENUM_TYPE

   DESCRIPTION:
     Indicates which type of notification is received by LPA from eUICC.
-------------------------------------------------------------------------------*/
typedef enum {
  LPA_NOTIFICATION_PROFILE_INSTALLATION_RESULT,
  LPA_NOTIFICATION_OTHER_SIGNED_NOTIFICATION
} lpa_notification_type_enum_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_NOTIFICATION_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  uint32                                seq_number;
  lpa_notification_type_enum_type       notification_type;
  lpa_smdp_address_type                 smdp_address;
  union {
  lpa_data_type                         profile_installation_result;
  lpa_data_type                         other_signed_notification;
  } data;
} lpa_notification_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_NOTIFICATION_RETRIEVE_NOTIFICATION_LIST_RESP_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  uint32                                   num_notifications;
  lpa_notification_type                   *notification_list_ptr;
} lpa_notification_retrieve_notification_list_resp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_NOTIFICATION_SEND_NOTIFICATION_RESP_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  uint32                                  http_response;
} lpa_notification_send_notification_resp_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_NOTIFICATION_CMD_DATA_TYPE

   DESCRIPTION:

-------------------------------------------------------------------------------*/
typedef struct {
  lpa_notification_cmd_enum_type                             cmd;
  union {
    lpa_notification_retrieve_notification_list_resp_type    retrieve_notification_list_resp;
    lpa_notification_send_notification_resp_type             send_notification_resp;
  } data;
} lpa_notification_cmd_data_type;

/* ----------------------------------------------------------------------------
   STRUCTURE:      LPA_NOTIFICATION_RESPONSE_DATA_TYPE

   DESCRIPTION:
     Contains information for handling LPA notifications
-------------------------------------------------------------------------------*/
typedef struct {
  q_link_type                                         link;
  lpa_result_enum_type                                status;
  lpa_slot_id_enum_type                               slot_id;
  lpa_notification_cmd_data_type                      cmd_data;
} lpa_notification_response_data_type;

/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION LPA_NOTIFICATION_INIT

DESCRIPTION
  This function, called in lpa_main, is called to initialize the Notification
  module.

DEPENDENCIES
  LPA task must have finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_notification_init (
  void
);

/*===========================================================================
FUNCTION LPA_NOTIFICATION_HANDLE_CMD_SIG

DESCRIPTION
  This function, called in lpa_main, handles processing of all Notifications
  related sub commands.

DEPENDENCIES
  LPA task must be finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_notification_handle_cmd_sig (
  void
);

/*===========================================================================
FUNCTION LPA_NOTIFICATION_CLEANUP

DESCRIPTION
  This function flushes the lpa notification queue.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_notification_cleanup (
  void
);

/*===========================================================================
FUNCTION LPA_NOTIFICATION_INIT

DESCRIPTION
  This function, called in lpa_main, is called to initialize the LPA
  Notification module and globals.

DEPENDENCIES
  LPA task must be finished initialization.

LIMITATIONS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void lpa_notification_init (
  void
);

/*===========================================================================
FUNCTION LPA_NOTIFICATION_RETRIEVE_AND_SEND

DESCRIPTION
  Function retrieves the pending notifications from the card and sends them
  to the appropriate servers.

DEPENDENCIES
  None

RETURN VALUE
  lpa_result_enum_type

SIDE EFFECTS
  None
===========================================================================*/
lpa_result_enum_type lpa_notification_retrieve_and_send (
  lpa_slot_id_enum_type                              slot_id
);

#endif /* LPA_NOTIFICATION_H */

