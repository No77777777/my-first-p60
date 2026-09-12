#ifndef GSTK_UIM_LOGGING_H
#define GSTK_UIM_LOGGING_H

/*===========================================================================


              G S T K   U I M   L O G G I N G   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2020 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     -----------------------------------------------------------
01/02/20   sch     Initial revision
=============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/
#include "uim_variation.h"
#include "uim_v.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* Length of Event List TLV in Event Download req APDU */
#define GSTK_ENVELOPE_EVENT_LIST_LEN    3


/* ----------------------------------------------------------------------------
   FUNCTION:      GSTK_LOGGING_POPULATE_MASKING_FN_PTR

   DESCRIPTION
     This function populates the appropriate function pointer that masks
     sensitive information in envelope commands and responses.

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
void gstk_logging_populate_masking_fn_ptr(
  uim_cmd_type                 *uim_cmd_ptr
);

#endif /* GSTK_UIM_LOGGING_H */