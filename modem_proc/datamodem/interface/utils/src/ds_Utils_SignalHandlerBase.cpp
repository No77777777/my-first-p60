/**
   @file
   ds_Utils_SignalHandlerBase.cpp

   @details
   This file implements the ds::Utils::SignalHandlerBase class.

   @see
   ds_Utils_SignalHandlerBase.h

   Copyright (c) 2010 Qualcomm Technologies Incorporated.
   All Rights Reserved.
   Qualcomm Confidential and Proprietary
*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/utils/src/ds_Utils_SignalHandlerBase.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-01-11  mt Created the module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

#include "ds_Utils_SignalHandlerBase.h"

#include "ds_Utils_DebugMsg.h"
#include "ps_mem_ext.h"

using namespace ds::Utils;

SignalHandlerBase::SignalHandlerBase() :
   refCnt(1), weakRefCnt(1)
{
   LOG_MSG_INFO3_1 ("SignalHandlerBase::SignalHandlerBase(): "
                    "Obj 0x%p", this);
   signalHandler.Init(this);

} /* SignalHandlerBase::SignalHandlerBase() */

void SignalHandlerBase::Destructor() throw()
{
   (void)signalHandler.Release();

} /* SignalHandlerBase::Destructor() */

SignalHandlerBase::~SignalHandlerBase() throw() {}

void SignalHandlerBase::operator delete
(
   void *  bufPtr
) throw()
{
   PS_MEM_FREE(bufPtr);

} /* SignalHandlerBase::operator delete() */
