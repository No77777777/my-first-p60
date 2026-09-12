/*===========================================================================
                    WRRC ULOG Implementation file

DESCRIPTION
  This file contains global declarations, MACROS and external references
  for the ULOG functionality to be used by WRRC

EXTERNALIZED FUNCTIONS

  wrrc_get_ulog_handle
    This function returns the ulog handle required for ULOG tracing.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2015 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrculog.c#2 $
$DateTime: 2020/01/24 03:56:36 $
$Author: pwbldsvc $

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/30/15   sp      Created this file. Declared init functions for ULOG 
                   functionality to be used by WRRC
08/11/15   sp      Reduced ULOG size to 2Kb                   
===========================================================================*/


/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/

#ifndef TEST_FRAMEWORK


#include "rrculog.h"

#define WRRC_LOG_NAME "WRRC_DEBUG"

/*The size is choosen based on initial testing, which suggest that
on average this will be able to log 150 traces with the ULOG*/
#define WRRC_LOG_SIZE 2048 


/*WRM ULOG handle*/
/*W+W: Not duplicating below handler as it is sparingly used. 
  So the alocated 2KB buffer for both the subs should be good enough for now*/
static ULogHandle wrrc_ulog_handle = NULL; 

/*===========================================================================
FUNCTION     WRRC_ULOG_INIT

DESCRIPTION
  This function is invoked once only to initialize the ulog handle for the tracing
  purpose.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void wrrc_ulog_init(sys_modem_as_id_e_type wrrc_as_id)
{
  if(!wrrc_ulog_handle)
  {
     ULogFront_RealTimeInit(&wrrc_ulog_handle, 
                             WRRC_LOG_NAME, 
                             WRRC_LOG_SIZE, 
                             ULOG_MEMORY_LOCAL, 
                             ULOG_LOCK_OS);
  }
  /* else , Handle is already created earlier. No op needed then. Just return*/
  return;
}

/*===========================================================================
FUNCTION     WRRC_GET_ULOG_HANDLE

DESCRIPTION
  This function is invoked for getting the wrrc ulog handle. This is required for
  ULOG logging to the corresponding handle only.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  wrrc_ulog_handle - ULogHandle in use by WRRC code.

SIDE EFFECTS
  None.
===========================================================================*/
ULogHandle wrrc_get_ulog_handle(void)
{
  return wrrc_ulog_handle;
}
#endif /*TEST_FRAMEWORK*/


