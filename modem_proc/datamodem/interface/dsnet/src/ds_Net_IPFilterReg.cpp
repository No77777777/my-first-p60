/*==========================================================================*/
/*!
  @file
  ds_Net_IPFilterReg.cpp

  @brief
  This file provides implementation for the ds::Net::IPFilterReg class.

  @see  ds_Net_IPFilterReg.h

  Copyright (c) 2008-2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_IPFilterReg.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-09-30 kr  Free floating changes
  2008-08-10 hm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_StdErr.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Net_IPFilterReg.h"
#include "ds_Net_Platform.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_EventManager.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Net_Utils.h"
#include "ds_Utils_CSignalBus.h"

using namespace ds::Net;

IPFilterReg::IPFilterReg
(
  int32 ifaceHandle,
  int32 fltrHandle
)
: mIfaceHandle (ifaceHandle),
  mFltrHandle (fltrHandle),
  refCnt(1),
  weakRefCnt(1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_3 ("IPFilterReg::IPFilterReg(): "
                   "Creating obj 0x%p, if handle 0x%x, fltr handle 0x%x",
                   this, ifaceHandle, mFltrHandle);

} /* IPFilterReg() */

void IPFilterReg::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) NetPlatform::PSIfaceIPFilterDelete (mIfaceHandle,
                                             IP_FLTR_CLIENT_SOCKETS,
                                             mFltrHandle);

} /* IPFilterReg::Destructor() */

IPFilterReg::~IPFilterReg
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  //NO-OP: only used for freeing memory.

} /* ~IPFilterReg() */



