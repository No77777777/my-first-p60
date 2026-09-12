/*!
  @file
  ds_Utils_SignalHandler.cpp

  @brief
  This file provides implementation for ds::Utils::SignalHandler class.

  @see
  ds_Utils_SignalHandler.h
  AEEISignalHandler.h

  Copyright (c) 2009 - 2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/src/ds_Utils_SignalHandler.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-06-24  mt Modified to require and use IWeakRef.
  2009-11-16  mt Created the module.

===========================================================================*/

/*---------------------------------------------------------------------------
  Include Files
---------------------------------------------------------------------------*/
#include "comdef.h"

#include "ds_Utils_Atomic.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Errors_Def.h"
#include "ds_Utils_SignalHandler.h"

using namespace ds::Utils;
using namespace ds::Error;

/*===========================================================================

                       CONSTRUCTOR/DESTRUCTOR

===========================================================================*/
SignalHandler::SignalHandler()
throw()
: mpOwner(0), refCnt(1)
{
  LOG_MSG_INFO3_1 ("SignalHandler::SignalHandler(): "
                   "Obj 0x%p", this);

} /* SignalHandler() */

SignalHandler::~SignalHandler()
throw()
{
  LOG_MSG_INFO3_1 ("SignalHandler::~SignalHandler(): "
                   "Obj 0x%p", this);

} /* ~SignalHandler() */

uint32 SignalHandler::Release()
throw()
{
  uint32 tmpRefCnt = ds_utils_atomic_Add(&refCnt, -1);
  LOG_MSG_INFO3_2("SignalHandler::Release(): "
                  "Release(), obj 0x%p, refCnt %d", this, tmpRefCnt);
  if (tmpRefCnt == 0)
  {
    /* Don't call DS_UTILS_RELEASE_WEAKREF_IF here.
      SignalHandler base holds SignalHandler which again holds
      signalHandlerBase So above macro, if weak ref count is zero deleting
      the object and setting mpOwner to NULL which leads to corruption as
      SignalHandlerBase is already released and mpOwner is part of
      signalHandlerBase Object.
      In Short it is "containment coupled with a circular reference"
    */
    mpOwner->ReleaseWeak(); /* Reference taken in Init() */
  }
  return tmpRefCnt;
}

/*===========================================================================

                       PUBLIC FUNCTION DEFINITIONS

===========================================================================*/

int CDECL SignalHandler::Notify(SignalHandlerCallBack pSignalHandlerCallBack,
                                SignalHandlerBase* pSignalHandlerBaseParam)
{

  if ((mpOwner != NULL) && mpOwner->GetStrongRef()) {
    // Got the strong reference - can call the callback
    pSignalHandlerCallBack(pSignalHandlerBaseParam);

    (void)mpOwner->Release();
  }

  return AEE_ENOMORE; /* Do not reenable the signal */
}

void SignalHandler::Init(IWeakRef *pOwner)
throw()
{
  mpOwner = pOwner;
  (void)mpOwner->AddRefWeak(); /* released when SignalHandler's refcnt goes
                                  down to 0 */
}
