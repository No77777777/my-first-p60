/*=============================================================================

             O N - B O A R D   D I A G N O S T I C S   A P P

GENERAL DESCRIPTION
  This source file contains the stubs functions for the OBDAPP module

Copyright (c) 2015 QUALCOMM Technologies, Inc (QTI) and its licensors.
All Rights Reserved.  QUALCOMM Technologies Proprietary.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

=============================================================================*/


/*=============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/uim.mpss/6.1.0/obdapp/src/obdapp_stubs.c#2 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------
08/30/15   at      File created.

=============================================================================*/


/*=============================================================================

                           INCLUDE FILES

=============================================================================*/
#include "obdapp_p.h"


/*==================================================================
FUNCTION OBD_HANDLE_NEW_STATE

DESCRIPTION
   This functions depending on the new state of the obdapp_ctxt
   will invoke required functions to acheive the authentication

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

==================================================================*/
void obd_handle_new_state
(
  void
)
{
  return;
} /* obd_handle_new_state */


/*===========================================================================
FUNCTION OBD_INIT

DESCRIPTION
  Initialisation function of OBDAPP.. Initializes required varaibles, obdapp
  context structure and any other registrations with Diag etc.. Checks if OBDAPP
  Keys are present with key hanlder otherwise OBDAPP doesn't go forward..


PARAMETER

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None

COMMENTS
  None
===========================================================================*/
boolean obd_init
(
  rex_tcb_type   * gsdi_task_tcb_ptr
)
{
  (void)gsdi_task_tcb_ptr;
  return FALSE;
} /* obd_init */

