/*======================================================

FILE:  DSS_RanAsstReq.cpp

SERVICES:

GENERAL DESCRIPTION:
Implementation of DSSRanAsstMediaAdaptReq class

=====================================================

Copyright (c) 2011 - 2018 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_RanAsstMediaAdaptReq.cpp#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------

===========================================================================*/

//===================================================================
//   Includes and Public Data Declarations
//===================================================================

#include "comdef.h"

//-------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------
#include "DSS_Common.h"
#include "dserrno.h"
#include "dss_iface_ioctl.h"

#include "ds_Utils_CCritSect.h"

#include "DSS_RanAsstMediaAdaptReq.h"
#include "DSS_Globals.h"
#include "DSS_CritScope.h"
#include "DSS_EventHandler.h"
#include "DSS_MemoryManagement.h"
#include "ds_Errors_Def.h"


#include "ds_Utils_CreateInstance.h"

using namespace ds::Net;
using namespace ds::Error;
//-------------------------------------------------------------------
// Constant / Define Declarations
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Type Declarations (typedef, struct, enum, etc.)
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Global Constant Data Declarations
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Global Data Declarations
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Forward Declarations
//-------------------------------------------------------------------

//===================================================================
//              Macro Definitions
//===================================================================

//===================================================================
//            DSSQoSNetInitiatedReq Functions Definitions
//===================================================================

DSSRanAsstMediaAdaptReq::DSSRanAsstMediaAdaptReq(IRanAsstMediaAdaptReqType* pNetRanAsstReq,
  dss_iface_id_type ifaceId): refCnt(1), weakRefCnt(1), mpNetRanAsstReq(pNetRanAsstReq), 
   mNext(NULL), mpRanAsstReqEventHandler(NULL), mParentNetActive(NULL),
   mParentDSSNetMonitor(NULL),mIfaceId(ifaceId)
{
   //int res = AEE_SUCCESS;
   if (NULL != pNetRanAsstReq) {
      (void)pNetRanAsstReq->AddRef();
   }
}

AEEResult DSSRanAsstMediaAdaptReq::GetDSSNetActive(DSSNetActive** ppDSSNetActive){
    mParentNetActive->AddRef(); 
   *ppDSSNetActive = mParentNetActive;
   return AEE_SUCCESS;
}

void DSSRanAsstMediaAdaptReq::SetDSSNetActive(DSSNetActive* pDSSNetActive) {
   mParentNetActive = pDSSNetActive;
   mParentNetActive->AddRefWeak();
   return ;
}

AEEResult DSSRanAsstMediaAdaptReq::GetDSSNetMonitored(DSSNetMonitored** ppDSSNetMonitored){
   mParentDSSNetMonitor->AddRef();     
  *ppDSSNetMonitored = mParentDSSNetMonitor;
  return AEE_SUCCESS;
}

void DSSRanAsstMediaAdaptReq::SetDSSNetMonitored(DSSNetMonitored* pDSSNetMonitored){

  mParentDSSNetMonitor = pDSSNetMonitored;
  mParentDSSNetMonitor->AddRefWeak();
  return ;
}

void DSSRanAsstMediaAdaptReq::GetIfaceId(dss_iface_id_type* pIfaceId){
   *pIfaceId = mIfaceId;
}

//===================================================================
//  FUNCTION:   DSSRanAsstMediaAdaptReq::~DSSRanAsstMediaAdaptReq:
//
//  DESCRIPTION:
//  Destructor of the DSSRanAsstMediaAdaptReq class.
//===================================================================

/*lint -e{1551} */
 void DSSRanAsstMediaAdaptReq::Destructor() throw()
{
  LOG_MSG_INFO1_1 ("DSSRanAsstMediaAdaptReq::Destructor(): "
                   "Deleting object 0x%p", this);

   DS_UTILS_RELEASE_WEAKREF_IF(mParentNetActive);
   DS_UTILS_RELEASE_WEAKREF_IF(mParentDSSNetMonitor);
   
   DSSCommon::ReleaseIf((IQI**)&mpNetRanAsstReq);

   PS_MEM_RELEASE(mpRanAsstReqEventHandler);
   
}
/*lint –restore */

//===================================================================
//  FUNCTION:   DSSRanAsstMediaAdaptReq::InsertToSecList
//
//  DESCRIPTION:
//
//===================================================================

void DSSRanAsstMediaAdaptReq::InsertToList(DSSRanAsstMediaAdaptReq* pDSSRanAsstMediaAdaptReq)
{
   mNext = pDSSRanAsstMediaAdaptReq;
}

//===================================================================
//  FUNCTION:    DSSRanAsstMediaAdaptReq::RegEventCB  
//
//  DESCRIPTION: Register DSS_IFACE_IOCTL_RAN_ASST_MEDIA_ADAPTATION_EV with Phylink 
//===================================================================
AEEResult DSSRanAsstMediaAdaptReq::RegEventCB(dss_iface_ioctl_ev_cb_type* pEvArg,
                                        dss_iface_id_type ifaceId)
{
   DSSEventHandler* ppEventHandler = 0;
   
   IDS_ERR_RET(GetEventHandler(pEvArg->event, &ppEventHandler, true));
   IDS_ERR_RET(ppEventHandler->Register(pEvArg->event, pEvArg->event_cb, pEvArg->user_data_ptr, ifaceId));
   return AEE_SUCCESS;
}

//===================================================================
//  FUNCTION:    DSSQoSNetInitiatedReq::GetEventHandler
//
//  DESCRIPTION: Returns the DSSEventHandler for the specified event.
//               If bInit is true, this function will also initialize
//               the handler if it's not initialized.
//===================================================================
AEEResult DSSRanAsstMediaAdaptReq::GetEventHandler(dss_iface_ioctl_event_enum_type event,
                                               DSSEventHandler** ppEventHandler,
                                               bool bInit)
{
   switch (event) {
      case DSS_IFACE_IOCTL_RAN_ASST_MEDIA_ADAPTATION_EV:
         return FetchHandler(&mpRanAsstReqEventHandler, ppEventHandler, bInit);         

      default:
         LOG_MSG_ERROR_2("DSSRanAsstMediaAdaptReq::GetEventHandler(): "
                         "Obj 0x%p, unhandled event=%d case", this, event);
         return QDS_EFAULT;
   }
}

template<typename HandlerType>
AEEResult DSSRanAsstMediaAdaptReq::FetchHandler(HandlerType** pHandler, DSSEventHandler** ppEventHandler, bool bInit)
{
   if (bInit) {
      if (NULL == *pHandler) {
         *pHandler = HandlerType::CreateInstance();
         if (NULL == *pHandler) {
            return AEE_ENOMEMORY;
         }
         IDS_ERR_RET((*pHandler)->Init(this));
      }
   } else {
      // bInit is false , we expect pHandler to have a value here
      if (0 == *pHandler) {
         return QDS_EFAULT;
      }
   }
   *ppEventHandler = *pHandler;
   return AEE_SUCCESS;
}


void * DSSRanAsstMediaAdaptReq::operator new
(
   unsigned int numBytes
)  throw()
{
   return ps_system_heap_mem_alloc( numBytes);
} /* DSSRanAsstMediaAdaptReq::operator new() */


void DSSRanAsstMediaAdaptReq::operator delete
(
   void *  bufPtr
)
{
   PS_SYSTEM_HEAP_MEM_FREE(bufPtr);
   return;
} /* DSSRanAsstMediaAdaptReq::operator delete() */

