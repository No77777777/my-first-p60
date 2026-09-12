#ifndef MMGSDI_LOG_PKT_H
#define MMGSDI_LOG_PKT_H
/*===========================================================================


                     MMGSDI  BOOTUP  MESSAGE  LOGGING    
              
                        DEFINITIONS   AND   FUNCTIONS


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2015 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     -----------------------------------------------------------
08/27/15   ma      Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "mmgsdi.h"
#include "mmgsdi_common.h"

/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_INIT

   DESCRIPTION:
     This function initializes the timer and allocates memory to the log packet
     pointer.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_init
(
  void
);


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_NV_ITEMS

   DESCRIPTION:
     This is the logging function for the NV values,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/

void mmgsdi_log_pkt_nv_items
(
  void
);


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_RTRE_UPDATE

   DESCRIPTION:
     This is the logging function to update rtre NV value,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/
void mmgsdi_log_pkt_rtre_update
(
  mmgsdi_attribute_enum_type  attr_type
);


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_LINK_ESTABLISHED

   DESCRIPTION:
     This is the logging function for the link established ,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/

void mmgsdi_log_pkt_link_established
(
  const mmgsdi_notify_link_est_req_type* req_ptr
);


/* ==========================================================================
   FUNCTION:      MMGSDI_LOG_PKT_MMGSDI_EVENT

   DESCRIPTION:
     This is the logging function for the mmgsdi events ,
     extracts the corresponding information and stores it in the packed structure.

   DEPENDENCIES:
    None

   LIMITATIONS:
    None

   RETURN VALUE:
    None

   SIDE EFFECTS:
    None

==========================================================================*/

void mmgsdi_log_pkt_mmgsdi_event
(
  const mmgsdi_event_data_type * event_ptr
);

#endif  /* MMGSDI_LOG_PKT_H */

