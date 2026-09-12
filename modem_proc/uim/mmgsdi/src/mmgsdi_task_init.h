#ifndef MMGSDI_TASK_INIT_H
#define MMGSDI_TASK_INIT_H
/*===========================================================================


           M M G S D I  T A S K  I N I T I A L I Z A T I O N   H E A D E R S


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2011, 2013, 2015 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/mmgsdi/src/mmgsdi_task_init.h#2 $$ $DateTime: 2019/10/30 06:56:05 $

when       who     what, where, why
--------   ---     -----------------------------------------------------------
03/11/15   tl      Add caching of the module tcb pointer
08/09/13   kk      Deprecating uimcallctrl and moving task init functions
12/21/11   shr     Initial version

=============================================================================*/

/*=============================================================================

                     INCLUDE FILES FOR MODULE

=============================================================================*/

#include "uim_variation.h"
#include "comdef.h"
#include "mmgsdilib_p.h"
#include "mmgsdi.h"

/*=============================================================================

                       DATA DECLARATIONS

=============================================================================*/


/*=============================================================================

                       STRUCTURE DECLARATIONS

=============================================================================*/


/*=============================================================================

                                   MMGSDI MACROS

=============================================================================*/


/*=============================================================================

                       FUNCTION PROTOTYPES

=============================================================================*/

/*===========================================================================
FUNCTION MMGSDI_INIT_PRE_STARTUP

DESCRIPTION
  Function called to open base sessions and initialize QMI UIM.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  Function should only be called once during task initialization.
===========================================================================*/
void mmgsdi_init_pre_startup(
  void
);


/*===========================================================================
FUNCTION MMGSDI_INIT_POST_STARTUP

DESCRIPTION
  Function called to define Necessary Timers and initialize certain services.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None.  Function should only be called once during task initialization.
===========================================================================*/
void mmgsdi_init_post_startup(
  void
);

#endif /* MMGSDI_TASK_INIT_H */
