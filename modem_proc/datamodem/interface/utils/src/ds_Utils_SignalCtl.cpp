/*==========================================================================*/
/*!
  @file
  ds_Utils_SignalCtl.cpp

  @brief
  This file provides implementation for ds::Utils::SignalCtl class. 
  To simplify things all logic is in the Signal code, SignalCtl merely
  forwards the calls.

  @see
  ds_Utils_SignalCtl.h
  AEEISignalCtl.h

  Copyright (c) 2008-2009 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/


/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are signaled in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/src/ds_Utils_SignalCtl.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-02 hm  Created module.

===========================================================================*/

/*---------------------------------------------------------------------------
  Include Files
---------------------------------------------------------------------------*/
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_SignalCtl.h"
#include "ds_Utils_StdErr.h"
#include "ps_utils.h"

using namespace ds::Utils;
using namespace ds::Error;


/*===========================================================================

                       CONSTRUCTOR/DESTRUCTOR

===========================================================================*/
SignalCtl* SignalCtl::CreateInstance
(
  Signal *pSignal
)
throw()
{
  return new SignalCtl(pSignal);
} /* CreateInstance() */

SignalCtl::SignalCtl
(
  Signal* argSig
)
throw()
: sigPtr(argSig),
  refCnt(1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_QTRACE_HFT_MED(PS_TAG_UTILS, PS_SYS_DEFAULT_SUBS,
                   "SignalCtl::SignalCtl(): "
                   "SigCtl obj 0x%x for Sig 0x%x", this, argSig);

  (void) sigPtr->AddRef();

} /* SignalCtl() */


SignalCtl::~SignalCtl
(
  void 
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO3_1 ("SignalCtl::~SignalCtl(): "
                   "obj 0x%p", this);

  if (NULL != sigPtr)
  {
    sigPtr->Detach();
    (void) sigPtr->Release();
    sigPtr = NULL;
  }

} /* ~SignalCtl() */


/*===========================================================================

                       PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
ds::ErrorType CDECL SignalCtl::Detach
(
  void
)
throw()
{
/*-------------------------------------------------------------------------*/

  sigPtr->Detach();
  return AEE_SUCCESS;

} /* Detach() */

ds::ErrorType CDECL SignalCtl::Enable
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  sigPtr->Enable();
  return AEE_SUCCESS;

} /* Enable() */

ds::ErrorType CDECL SignalCtl::Set
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return sigPtr->SignalCtlSet();

} /* Set() */

