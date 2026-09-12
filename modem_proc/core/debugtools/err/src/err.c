/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         E R R O R     R E P O R T I N G    S E R V I C E S

GENERAL DESCRIPTION
  This module provides error reporting services for both fatal and
  non-fatal errors.  This module is not a task, but rather a set of
  callable procedures which run in the context of the calling task.

Copyright (c) 1992 - 2018 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        Edit History

$Header: //components/rel/core.mpss/10.0/debugtools/err/src/err.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/27/15   din     Adapted from err.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "qurt.h"
#include "qurt_event.h"
#include "erri.h"
#include <stdlib.h>
#include <stringl.h>
#include "tms_utils.h"

/*===========================================================================

                 Defines and variable declarations for module

===========================================================================*/

/* Struct used to hold coredump data */
coredump_type coredump;

/*===========================================================================

                              Function definitions

===========================================================================*/

/*===========================================================================

FUNCTION ERR_INITIALIZE_COREDUMP

DESCRIPTION
  Initializes coredump

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/
void err_initialize_coredump (void)
{
  /* Set type and version values */
  coredump.version = ERR_COREDUMP_VERSION;
  coredump.arch.type = ERR_ARCH_COREDUMP_TYPE;
  coredump.arch.version = ERR_ARCH_COREDUMP_VER;  
  coredump.os.type = ERR_OS_COREDUMP_TYPE;
  coredump.os.version = ERR_OS_COREDUMP_VER;
  coredump.err.version = ERR_COREDUMP_VER;

} /* err_initialize_coredump */

/*===========================================================================

FUNCTION ERR_INIT

DESCRIPTION
  This function initializes error services and calls into target & process 
  based error intializing routine.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  For Root PD on MPSS, Error log is loaded from NVM into RAM resident cache.

===========================================================================*/

void err_init (void)
{
  if(qurt_atomic_compare_and_set(&err_fatal_params.mutex_init, FALSE, TRUE))
  {
    ERR_MUTEX_INIT(&err_fatal_params.mutex);
  }

  err_initialize_coredump();
  err_update_image_versioning_info();

} /* err_init */

/*===========================================================================

FUNCTION ERR_FATAL_TRY_LOCK
DESCRIPTION
  Updates Error functionality at the entry of error. This is intentionally 
  used from exception handler.
============================================================================*/
void err_fatal_try_lock(void)
{
  if(err_fatal_params.mutex_init==TRUE)
  {
    ERR_MUTEX_TRY_LOCK(&err_fatal_params.mutex);
  }
  
  /* tms_get_timetick is not island mode safe */
  tms_get_timetick(&(coredump.err.start_time));

} /* err_fatal_try_lock */

/*===========================================================================

FUNCTION ERR_FATAL_UNLOCK
DESCRIPTION
  Unlocks Error mutex lock.
============================================================================*/
void err_fatal_unlock(void)
{
  if(err_fatal_params.mutex_init==TRUE)
  {
    ERR_MUTEX_UNLOCK(&err_fatal_params.mutex);
  }

} /* err_fatal_unlock */


/*=========================================================================

FUNCTION err_update_reentracny_flag

DESCRIPTION
  Updates Crumb trail mask

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define ERR_REENTRANCY_STRING "ERR_FATAL reentrancy violation, remove cb until resolved"
void err_update_reentrancy_flag(void)
{
  /* Record secondary failure to coredump */
  strlcpy(coredump.err.int_msg, ERR_REENTRANCY_STRING, 
          sizeof(ERR_REENTRANCY_STRING));

} /* err_update_reentrancy_flag */
