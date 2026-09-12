
/*==========================================================================*/
/*!
  @file
  ds_Net_RanAsstMediaAdaptReq.cpp

  @brief
  This file provides implementation for the DS::Net::RanAsstMediaAdaptReq class.

  @see  ds_Net_Firewall.h

  Copyright (c) 2010 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_RanAsstMediaAdaptReq.cpp#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2008-05-20 dm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_StdErr.h"
#include "ds_Errors_Def.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Net_RanAsstMediaAdaptReq.h"
#include "ds_Net_Platform.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_IPFilterSpec.h"
#include "ds_Net_Conversion.h"
#include "ds_Net_EventManager.h"
#include "ds_Utils_CSignalBus.h"
#include "ds_Utils_CreateInstance.h"
#include "ps_handle_mgr.h"

using namespace ds::Net;
using namespace ds::Error;
using namespace ds::Net::Conversion;
using namespace NetPlatform;

RanAsstMediaAdaptReqType::RanAsstMediaAdaptReqType
(
  void
)
: mFilterSpec(NULL), refCnt(1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  //Assign handle for this request
  mranAsstHandle = ps_handle_mgr_get_handle(PS_HANDLE_MGR_CLIENT_RAN_ASST_DSS, 0);

  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,
                                 (void **) &mpSigBusStateChange);

} /* RanAsstMediaAdaptReqType() */


RanAsstMediaAdaptReqType::~RanAsstMediaAdaptReqType
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL != mFilterSpec )
  {
     DS_UTILS_RELEASEIF(mFilterSpec);
  }

  DS_UTILS_RELEASEIF (mpSigBusStateChange);

  /*-------------------------------------------------------------------------
    NO-OP: used only for freeing memory
  -------------------------------------------------------------------------*/

} /* RanAsstMediaAdaptReqType::RanAsstMediaAdaptReqType() */

void RanAsstMediaAdaptReqType::Destructor
(
  void
)
throw()
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    
} /* RanAsstMediaAdaptReqType::~RanAsstMediaAdaptReqType() */

/*---------------------------------------------------------------------------
  Inherited functions from IRanAsstMediaAdaptReqType.
---------------------------------------------------------------------------*/
ds::ErrorType RanAsstMediaAdaptReqType::GetRanAsstMediaAdaptReqFltrSpec
(
  IIPFilterPriv** ppIFilterSpec
)
{
  int32                    result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Validation
  -------------------------------------------------------------------------*/
  if ( NULL == ppIFilterSpec )
  {
    result = AEE_ENOMEMORY;
  }
  else
  {
    *ppIFilterSpec =  static_cast<IIPFilterPriv *>(mFilterSpec);
  }

  return result;
}/* GetRanAsstMediaAdaptReqFltrSpec */

ds::ErrorType RanAsstMediaAdaptReqType::SetRanAsstMediaAdaptReqFltrSpec
(
  IIPFilterPriv*  pIFilterSpec
)
{
  int32                    result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Validation
  -------------------------------------------------------------------------*/
  if ( NULL == pIFilterSpec )
  {
    result = AEE_ENOMEMORY;
  }
  else
  {
     mFilterSpec = static_cast<IPFilterSpec *>(pIFilterSpec);
     mFilterSpec->AddRef();
  }

  return result;
}/* GetRanAsstMediaAdaptReqFltrSpec */


ds::ErrorType RanAsstMediaAdaptReqType::GetRanAsstMediaAdaptHandle
(
  uint32* handle
)
{
  int32                     result = AEE_SUCCESS;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  *handle =  mranAsstHandle;

  return result;
}/*GetRanAsstMediaAdaptHandle*/

boolean RanAsstMediaAdaptReqType::Process
(
  void *pUserData
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) pUserData;
  DATA_ERR_FATAL("RanAsstMediaAdaptReqType::Process(): Process is unsupported");
  return FALSE;

} /* RanAsstMediaAdaptReqType::Process() */

