#ifndef MMGSDICALLERCONTEXT_H
#define MMGSDICALLERCONTEXT_H
/*===========================================================================


            M M G S D I  P R O C E S S  I N  C L I E N T  C O N T E X T
            
                 D E F I N I T I O N S  A N D   F U N C T I O N S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2014, 2016, 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_caller_context.h#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
05/07/18   rps     Retrieving attribute cache from caller context
12/29/16   dt      Support for QXDM sim data collection.
01/07/16   sp      Handle prl read in caller context
11/05/14   ar      Remove mmgsdi_get_card_reader_status () API
08/16/14   lj      Optimize app capabilities with cached data in client context
08/11/14   bcho    Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "uim_variation.h"
#include "mmgsdi.h"


/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_CHECK_SERVICE_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to retrieve service status from cached service table.
     If the function is invoked from MMGSDI tasks context we simply return
     without any processing.
     This function uses critical sections.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The retrieval of service status from cached
                               service table succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_check_service_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_READ_CACHE_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to read the item from MMGSDI cache. If the read
     succeeds or the item is not found on the card, a response is built
     immediately and cnf is sent to the client. App state must be ready;
     otherwise we simply return without doing anything.
     If the function is invoked from MMGSDI tasks context, we simply return
     without any processing.
     This function uses critical sections.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The read from cache succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_read_cache_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_CHECK_APP_CAPABILITIES_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to retrieve app capabilites from cached app info.
     If the function is invoked from MMGSDI tasks context we simply return
     without any processing.
     This function uses critical sections.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The retrieval of service status from cached
                               service table succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/

mmgsdi_return_enum_type mmgsdi_caller_context_check_app_capabilities_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_SAP_CARD_READER_STATUS_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to retrieve card reader status if the slot_data_ptr is available for
     that particular Slot. This function is only used for SAP_CARD_READER_STATUS.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The retrieval of service status from cached
                               service table succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:

==========================================================================*/

mmgsdi_return_enum_type mmgsdi_caller_context_sap_card_reader_status_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_READ_PRL_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to read PRL data from MMGSDI cache.for 1x prov sessions
     If the PRL data is not validated then we simply return from the caller this function
     and queue the command to MMGSDI.
     This function uses critical sections.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The read from cache succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:
     None
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_read_prl_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
);

/* ==========================================================================
   FUNCTION:      MMGSDI_CALLER_CONTEXT_GET_ATR_STATUS_AND_NOTIFY_CLIENT

   DESCRIPTION:
     This function tries to read atr data from MMGSDI cache.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     mmgsdi_return_enum_type:

      MMGSDI_SUCCESS:          The read from cache succeeded and client has
                               notified with the cnf
      MMGSDI_ERROR:            Otherwise

   SIDE EFFECTS:
     None
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_get_atr_status_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
);

/* ==========================================================================
  FUNCTION:    MMGSDI_CALLER_CONTEXT_GET_FILE_ATTR_CACHE_AND_NOTIFY_CLIENT

  DESCRIPTION:
    This function tries to get cached file attributes and a response is built
    immediately and cnf is sent to the client.This function uses critical sections.

  DEPENDENCIES:
    None

  LIMITATIONS:
    None

  RETURN VALUE:
    mmgsdi_return_enum_type:

    MMGSDI_SUCCESS:      The get file attributes from cache succeeded
                         and client has notified with the cnf
    MMGSDI_ERROR:        Otherwise

  SIDE EFFECTS:
    None
==========================================================================*/
mmgsdi_return_enum_type mmgsdi_caller_context_get_file_attr_cache_and_notify_client(
  const mmgsdi_task_cmd_type             *task_cmd_ptr
);
#endif /* MMGSDICALLERCONTEXT_H */
