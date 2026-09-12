#ifndef SIMLOCK_MMGSDI_H
#define SIMLOCK_MMGSDI_H
/*===========================================================================


            S I M L O C K   M M G S D I   H E A D E R


===========================================================================*/

/*===========================================================================
                        COPYRIGHT INFORMATION

Copyright (c) 2018 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/simlock/src/simlock_mmgsdi.h#2 $$ $DateTime: 2019/10/30 06:56:05 $

when              who     what, where, why
--------   ---     -----------------------------------------------------------
05/08/18         nr      Initial revision
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "uim_variation.h"
#include "comdef.h"

#if defined (FEATURE_SIMLOCK) && defined (FEATURE_SIMLOCK_RSU)

#include "simlock_common.h"

/*===========================================================================
FUNCTION SIMLOCK_MMGSDI_INIT

DESCRIPTION
  This function handles the registration of simlock with MMGSDI.

DEPENDENCIES
  NONE

LIMITATIONS
  NONE

RETURN VALUE
  simlock_result_enum_type

SIDE EFFECTS
  NONE
===========================================================================*/
simlock_result_enum_type simlock_mmgsdi_init
(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_HANDLE_MMGSDI_EVT

DESCRIPTION
  This function read the MMGSDI events from the queue and handle it.
  Typically we do not have to keep the mmgsdi event data around, so it
  is freed immediately after processing.

DEPENDENCIES
  None

RETURN VALUE
  Void
  
SIDE EFFECTS
  None
===========================================================================*/
void simlock_handle_mmgsdi_evt
(
  void
);

/*===========================================================================
FUNCTION SIMLOCK_MMGSDI_FLUSH_QUEUE

DESCRIPTION
  This function is called to clear the simlock_mmgsdi_evt_q.

DEPENDENCIES
  None

LIMITATIONS
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void simlock_mmgsdi_flush_queue (
  void
);

#endif /* FEATURE_SIMLOCK && FEATURE_SIMLOCK_RSU */

#endif /* SIMLOCK_MMGSDI_H */

