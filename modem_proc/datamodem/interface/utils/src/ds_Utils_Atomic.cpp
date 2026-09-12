/*=========================================================================*/
/*!
  @file
  ds_Utils_Atomic.cpp

  @brief
  This file provides implementation for mutually-atomic functions
  in a platform-independent way. See ds_Utils_Atomic.h for details.

  @see
  ds_Utils_Atomic.h
  AEEatomic.h

  Copyright (c) 2009-2014 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================

  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/src/ds_Utils_Atomic.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-03-08 hm  Use single DS implementation for all configurations. 
  2009-11-30 mt  Created module.

===========================================================================*/
/*===========================================================================

                        INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"

#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_ICritSect.h"
#include "ds_Utils_CCritSect.h"
#include "ds_Utils_Atomic.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CSSupport.h"

/*===========================================================================

                      STATIC VARIABLES FOR MODULE

===========================================================================*/
/*---------------------------------------------------------------------------
  This critical section is used by all atomic operations to ensure they are
  mutually exclusive. It is initialized by ds_utils_atomic_Init()
---------------------------------------------------------------------------*/
static ICritSect * pAtomicCritSect = 0;

/*===========================================================================

                     EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/
void ds_utils_atomic_Init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void)DS_Utils_CreateInstance(NULL,
                                AEECLSID_CCritSect,
                                (void **) &pAtomicCritSect);

} /* ds_utils_atomic_Init() */


void ds_utils_atomic_DeInit
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_UTILS_RELEASEIF(pAtomicCritSect);

} /* ds_utils_atomic_Init() */


uint32 ds_utils_atomic_Add
(
  uint32*   pDest,
  int       valueToAdd
)
{
  uint32 ret;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == pDest)
  {
    return 0;
  }

  pAtomicCritSect->Enter();

  *pDest += valueToAdd;
  ret = *pDest;

  pAtomicCritSect->Leave();

  return ret;

} /* ds_utils_atomic_Add() */


uint32 ds_utils_atomic_CompareOrAdd
(
  uint32*    pDest,
  uint32     valueToCompare,
  int        valueToAdd
)
{
  uint32 ret;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == pDest)
  {
    return 0;
  }

  pAtomicCritSect->Enter();

  if (*pDest != valueToCompare)
  {
    *pDest += valueToAdd;
  }
  ret = *pDest;

  pAtomicCritSect->Leave();

  return ret;

} /* ds_utils_atomic_CompareOrAdd() */


