#ifndef MMGSDI_UIM_LOGGING_H
#define MMGSDI_UIM_LOGGING_H

/*===========================================================================


              M M G S D I   U I M   L O G G I N G   H E A D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2017 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order. 
 
when       who     what, where, why
--------   ---     ----------------------------------------------------------- 
01/19/17   nc       Initial revision
=============================================================================*/

/*=========================================================================== 
 
                     INCLUDE FILES FOR MODULE    
 
=============================================================================*/
#include "uim_variation.h"
#include "mmgsdi.h"
#include "mmgsdilib_common.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/


/* ----------------------------------------------------------------------------
   FUNCTION:      MMGSDI_LOGGING_POPULATE_MASKING_FN_PTR

   DESCRIPTION
     This function populates the appropriate function pointer that masks
     sensitive information

   DEPENDENCIES:
     None

   LIMITATIONS:
     None

   RETURN VALUE:
     None

   SIDE EFFECTS:
     None

-------------------------------------------------------------------------------*/
void mmgsdi_logging_populate_masking_fn_ptr(
  uim_cmd_type                        *uim_cmd_ptr,
  mmgsdi_app_enum_type                 app_type);


#endif /* MMGSDI_UIM_LOGGING_H */
