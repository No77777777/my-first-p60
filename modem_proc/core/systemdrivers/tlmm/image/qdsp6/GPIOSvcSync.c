/*===========================================================================

FILE:      GPIOSvcSync.c

DESCRIPTION:
  This file contains the GPIO abstraction of the synchronization objects.
  It is expected to be image-specific where implementations differ across
  images.

PUBLIC CLASSES:
  GPIO_SvcSynInit
  GPIO_SyncEnter
  GPIO_SyncExit

===========================================================================
Copyright (c) 2017 QUALCOMM Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
========================================================================================================*/

/*========================================================================================================

  EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.mpss/10.0/systemdrivers/tlmm/image/qdsp6/GPIOSvcSync.c#1 $

  when       who     what, where, why
  --------   ---     -------------------------------------------------------------------------------------
  03/02/17   dcf     Created.
  ======================================================================================================*/

/*=======================================================================================================

  INCLUDE FILES FOR MODULE

  =====================================================================================================*/

#include "qurt.h"
#include "qurt_futex.h"
#include "qurt_atomic_ops.h"
#include "GPIOSvcSync.h"


/*=======================================================================================================

  INTERNAL DATA STRUCTURES

  =====================================================================================================*/

/*
 * Structure definition for the TLMM synchronization object.
 */
struct TLMMSyncType
{
  qurt_mutex_t     Mutex;
};

/*
 * TLMM Synchronization object.
 */
struct TLMMSyncType TLMMSync;

/*
 * Boolean flag for specifying initialization.
 */
static boolean bInitialized = FALSE;


/*=======================================================================================================

                            APPLICATION PROGRAMMERS INTERFACE FUNCTIONS

  =====================================================================================================*/


/*========================================================================================================

 FUNCTION    GPIO_SvcSynInit

 DESCRIPTION  See GPIOSrvSync.h

========================================================================================================*/

boolean GPIO_SvcSynInit(void)
{
  if ( bInitialized == FALSE )
  {
    qurt_mutex_init(&TLMMSync.Mutex);
    bInitialized = TRUE;
  }
  return TRUE;
}


/*========================================================================================================

 FUNCTION    GPIO_SyncEnter

 DESCRIPTION  See GPIOSrvSync.h

========================================================================================================*/

void GPIO_SyncEnter(void)
{
  if ( bInitialized != TRUE )
  {
    GPIO_SvcSynInit();
  }
  qurt_rmutex_lock(&TLMMSync.Mutex);
}


/*========================================================================================================

 FUNCTION    GPIO_SyncEnter

 DESCRIPTION  See GPIOSrvSync.h

========================================================================================================*/

void GPIO_SyncExit(void)
{
  if ( bInitialized != TRUE )
  {
    GPIO_SvcSynInit();
  }
  qurt_rmutex_unlock(&TLMMSync.Mutex);
}

