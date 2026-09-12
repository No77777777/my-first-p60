/*======================================================

FILE:  DSS_QoSNetInitiated.cpp

SERVICES:

GENERAL DESCRIPTION:
Implementation of DSSQoSNetInitiated class

=====================================================

Copyright (c) 2011 - 2014 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_QoSNetInitiated.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-06-07 sz  Created.

===========================================================================*/
#include "comdef.h"

//===================================================================
//   Includes and Public Data Declarations
//===================================================================

//-------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------
#include "DSS_Common.h"
#include "dserrno.h"

#include "DSS_QoSNetInitiated.h"
#include "DSS_Globals.h"
#include "DSS_GenScope.h"
#include "DSS_CritScope.h"
#include "DSS_EventHandler.h"
#include "DSS_MemoryManagement.h"
#include "ds_Errors_Def.h"
#include "DSS_QoSHandler.h"
#include "DSS_QoSModifyHandler.h"
#include "DSS_QoSInfoCodeUpdatedHandler.h"
#include "ds_Net_IQoSPriv.h"
#include "ds_Utils_CCritSect.h"
#include "ds_Utils_CreateInstance.h"

using namespace ds::Net;
using namespace ds::Error;

#define DSS_QOSINIT_ASSERT(expression) \
         dss_qos_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dss_conversion_assert_wrapper()

DESCRIPTION
  Wrapper function for DSS_CONVERSION_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dss_qos_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DSNET_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* dss_conversion_assert_wrapper */

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
//            DSSQoSNetInitiated Functions Definitions
//===================================================================

// TODO: documentation
DSSQoSNetInitiated::DSSQoSNetInitiated(IQoSNetInitiated*   pNetQoSNetInitiated, 
                                       dss_qos_handle_type handle,
                                       dss_iface_id_type   ifaceId,
                                       uint32              dssQosNetInitiatedAssocReq,
                                       dss_iface_ioctl_fltr_match_dir_type fltrMatchDir):
   refCnt(1), weakRefCnt(1), mpQoSNetInitiated(pNetQoSNetInitiated), 
   mNext(NULL), mHandle(handle), mIfaceId(ifaceId), mpQoSNetInitiatedHandler(NULL), 
   mpQoSNetInitiatedModifyHandler(NULL), mpQoSNetInitiatedInfoCodeHandler(NULL),
   mparentNetActive(NULL), mNetHandle(0),
   mDSSQoSNetInitiatedAssocReq(dssQosNetInitiatedAssocReq), mpCritSect(NULL),
   mFltrMatchDir(fltrMatchDir)
{
   int res = AEE_SUCCESS;
   
   if (NULL != pNetQoSNetInitiated) {
      (void)mpQoSNetInitiated->AddRef();
   }
   res = DS_Utils_CreateInstance(NULL, AEECLSID_CCritSect,
                                 (void**)&mpCritSect);
   if (AEE_SUCCESS != res) {
      LOG_MSG_ERROR_1("DSSQoSNetInitiatedReq::DSSQoSNetInitiatedReq(): "
                      "Can't create Critical Section (%d)", res);
      DSS_QOSINIT_ASSERT(0);
   }
}

AEEResult DSSQoSNetInitiated::GetDSSNetActive(DSSNetActive** ppDSSNetActive) {
   if(!mparentNetActive->GetStrongRef())
   {
      LOG_MSG_ERROR_1 ("DSSQoSNetInitiated::GetDSSNetActive(): "
                       "Obj 0x%p, GetStrongRef failed", this);
      return AEE_EFAILED;
   }

   *ppDSSNetActive = mparentNetActive;
   return AEE_SUCCESS;
}

void DSSQoSNetInitiated::SetDSSNetActive(DSSNetActive* pDSSNetActive) {
   mparentNetActive = pDSSNetActive;
   mparentNetActive->AddRefWeak();
}

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::~DSSQoSNetInitiated
//
//  DESCRIPTION:
//  Destructor of the DSSQoSNetInitiated class.
//===================================================================

/*lint -e{1551} */
 void DSSQoSNetInitiated::Destructor() throw()
{
   mpCritSect->Enter();
   FlowID tempFlowID = 0;

   LOG_MSG_INFO1_1 ("DSSQoSNetInitiated::Destructor(): "
                    "Deleting object 0x%p", this);

   DS_UTILS_RELEASE_WEAKREF_IF(mparentNetActive);
   DSSCommon::ReleaseIf((IQI**)&mpQoSNetInitiated);

   PS_MEM_RELEASE(mpQoSNetInitiatedHandler);
   PS_MEM_RELEASE(mpQoSNetInitiatedModifyHandler);
   PS_MEM_RELEASE(mpQoSNetInitiatedInfoCodeHandler);
   // Pass the 8 LSBits of mHandle to ReleaseFlowID since they
   // represent the flow ID that is now being released.
   tempFlowID = DSSIfaceId::StripDownToFlowID(mHandle);
   DSSGlobals::Instance()->ReleaseQoSFlowID((uint8)tempFlowID);
   mpCritSect->Leave();
   DSSCommon::ReleaseIf((IQI**)&mpCritSect);

}
/*lint –restore */

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::InsertToSecList
//
//  DESCRIPTION:
//
//===================================================================

void DSSQoSNetInitiated::InsertToList(DSSQoSNetInitiated* pDSSQoSNetInitiated)
{
   mNext = pDSSQoSNetInitiated;
}

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::GetHandle
//
//  DESCRIPTION:
//
//===================================================================
void DSSQoSNetInitiated::GetHandle(dss_qos_handle_type* pHandle)
{
   *pHandle = mHandle;
}
//===================================================================

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::SetHandle
//
//  DESCRIPTION:
//
//===================================================================
void DSSQoSNetInitiated::SetHandle(dss_qos_handle_type handle)
{
   mHandle = handle;
}
//===================================================================

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::GetQoSNetInitiated
//
//  DESCRIPTION:
//
//===================================================================
AEEResult DSSQoSNetInitiated::GetQoSNetInitiated(IQoSNetInitiated** ppQoSNetInitiated)
{
   if (NULL == mpQoSNetInitiated) {
      LOG_MSG_ERROR_1 ("DSSQoSNetInitiated::GetQoSNetInitiated(): "
                       "Obj 0x%p, mpQoSNetInitiated is NULL", this);
      return QDS_EINVAL;
   }

   *ppQoSNetInitiated = mpQoSNetInitiated;
   (void)mpQoSNetInitiated->AddRef();
   return AEE_SUCCESS;
}

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::GetQoS
//
//  DESCRIPTION:
//
//===================================================================
AEEResult DSSQoSNetInitiated::GetQoS(IQoS** ppQoS)
{
   if (NULL == mpQoSNetInitiated) {
      LOG_MSG_ERROR_1 ("DSSQoSNetInitiated::GetQoS(): "
                       "Obj 0x%p, mpQoSNetInitiated is NULL", this);
      return QDS_EINVAL;
   }

   *ppQoS = mpQoSNetInitiated;
   (void)mpQoSNetInitiated->AddRef();
   return AEE_SUCCESS;
}

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::GetGrantedFlowSpec
//
//  DESCRIPTION:
//
//===================================================================
AEEResult DSSQoSNetInitiated::GetGrantedFlowSpec(dss_iface_ioctl_qos_get_flow_spec_type* qos_flow_spec)
{
   AEEResult res = AEE_SUCCESS;

   // get the QoSNetInitiated object   
   IQoS* pIDSQoS = NULL;   
   IQoSFlowPriv *txQoSFlow = NULL, *rxQoSFlow = NULL;
   uint8 b_id = 0;
   uint8 *bearer_id = &b_id;
   
   res = GetQoS(&pIDSQoS);   
   if ((res != AEE_SUCCESS) || (NULL == pIDSQoS))  {
      LOG_MSG_ERROR_1("DSSQoSNetInitiated::GetGrantedFlowSpec(): "
                      "Obj 0x%p, pIDSQoS is not available", this);
      return res;
   }
   DSSGenScope GenScopeObject(pIDSQoS, DSSGenScope::IDSIQI_TYPE);

   // allow the operation only if the QoSSession is AVAILABLE_MODIFIED
   QoSStateInfoType statusInfo;
   memset(&statusInfo, 0, sizeof(QoSStateInfoType));

   IDS_ERR_RET(pIDSQoS->GetState(&statusInfo));   

   if (QoSState::QDS_AVAILABLE_MODIFIED != statusInfo.state) {
      memset (&qos_flow_spec->rx_flow, 0, sizeof(qos_flow_spec->rx_flow));
      memset (&qos_flow_spec->tx_flow, 0, sizeof(qos_flow_spec->tx_flow));
      return DSS_SUCCESS;
   }

   IQoSPriv* pIDSQoSPriv = NULL;
   IDS_ERR_RET(pIDSQoS->QueryInterface(AEEIID_IQoSPriv, (void**)&pIDSQoSPriv));
   DSSGenScope GenScopeQoSPriv(pIDSQoSPriv, DSSGenScope::IDSIQI_TYPE);

   IDS_ERR_RET(pIDSQoSPriv->GetGrantedFlowSpecPriv(&rxQoSFlow, &txQoSFlow, &bearer_id));

   // Convert new QoS types to old QoS Types
   IDS_ERR_RET(DSSConversion::IDS2DSQoSSpecFlow(rxQoSFlow, &(qos_flow_spec->rx_flow)));
   IDS_ERR_RET(DSSConversion::IDS2DSQoSSpecFlow(txQoSFlow, &(qos_flow_spec->tx_flow)));
   qos_flow_spec->bearer_id = *bearer_id;

   DSSCommon::ReleaseIf((IQI**)&(txQoSFlow));
   DSSCommon::ReleaseIf((IQI**)&(rxQoSFlow));
   bearer_id = NULL;
   return AEE_SUCCESS;
}

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::GetGrantedFlowSpec2
//
//  DESCRIPTION:
//
//===================================================================
AEEResult DSSQoSNetInitiated::GetGrantedFlowSpec2(dss_iface_ioctl_qos_get_granted_flow_spec2_type* qos_getGrantedFlowSpec2)
{
   IQoS* pIDSQoS = NULL;
   AEEResult res = AEE_SUCCESS;

   // get the QoSNetInitiated object   
   res = GetQoS(&pIDSQoS);   
   if ((res != AEE_SUCCESS) || (NULL == pIDSQoS))  {
      LOG_MSG_ERROR_1("DSSQoSNetInitiated::GetGrantedFlowSpec2(): "
                      "Obj 0x%p, pIDSQoS is not available", this);
      return res;
   }
   DSSGenScope GenScopeObject(pIDSQoS, DSSGenScope::IDSIQI_TYPE);

   IQoSPriv* pIDSQoSPriv = NULL;
   IDS_ERR_RET(pIDSQoS->QueryInterface(AEEIID_IQoSPriv, (void**)&pIDSQoSPriv));
   DSSGenScope GenScopeQoSPriv(pIDSQoSPriv, DSSGenScope::IDSIQI_TYPE);

   IQoSFlowPriv *txQoSFlow = NULL, *rxQoSFlow = NULL;
   uint8 b_id = 0;
   uint8 *bearer_id = &b_id;

   QoSStateInfoType statusInfo;
   memset(&statusInfo, 0, sizeof(QoSStateInfoType));
   IDS_ERR_RET(pIDSQoS->GetState(&statusInfo));   

   // TODO: move to DSSConversion.cpp
   switch(statusInfo.state)
   {
   case QoSState::QDS_AVAILABLE_MODIFIED:
      qos_getGrantedFlowSpec2->qos_status = QOS_AVAILABLE;
      break;
   case QoSState::QDS_SUSPENDING:
      qos_getGrantedFlowSpec2->qos_status = QOS_SUSPENDING;
      break;
   case QoSState::QDS_UNAVAILABLE:
      qos_getGrantedFlowSpec2->qos_status = QOS_UNAVAILABLE;
      break;
   case QoSState::QDS_INVALID:
      qos_getGrantedFlowSpec2->qos_status = QOS_STATE_INVALID;
      break;
   case QoSState::QDS_ACTIVATING:
      qos_getGrantedFlowSpec2->qos_status = QOS_ACTIVATING;
      break;
   case QoSState::QDS_SUSPENDED:
      qos_getGrantedFlowSpec2->qos_status = QOS_DEACTIVATED;
      break;
   case QoSState::QDS_RELEASING:
      qos_getGrantedFlowSpec2->qos_status = QOS_RELEASING;
      break;
   case QoSState::QDS_CONFIGURING:
      qos_getGrantedFlowSpec2->qos_status = QOS_CONFIGURING;
      break;
   default:      
      LOG_MSG_ERROR_2 ("DSSQoSNetInitiated::GetGrantedFlowSpec2(): "
                       "Obj 0x%p, unsupported state: %d",
                       this, statusInfo.state);
      return QDS_EFAULT;
   }

   IDS_ERR_RET(pIDSQoSPriv->GetGrantedFlowSpecPriv(&rxQoSFlow, &txQoSFlow, &bearer_id));

   // Convert new QoS types to old QoS Types
   IDS_ERR_RET(DSSConversion::IDS2DSQoSSpecFlow(rxQoSFlow, &(qos_getGrantedFlowSpec2->rx_flow)));
   IDS_ERR_RET(DSSConversion::IDS2DSQoSSpecFlow(txQoSFlow, &(qos_getGrantedFlowSpec2->tx_flow)));
   qos_getGrantedFlowSpec2->bearer_id = *bearer_id;

   DSSCommon::ReleaseIf((IQI**)&(txQoSFlow));
   DSSCommon::ReleaseIf((IQI**)&(rxQoSFlow));
   bearer_id = NULL;
   
   return AEE_SUCCESS;
}

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::GetQoSStatus
//
//  DESCRIPTION:
//
//===================================================================
AEEResult DSSQoSNetInitiated::GetQoSStatus(dss_iface_ioctl_qos_get_status_type* pqos_status_type)
{
   IQoS* pIDSQoS = NULL;
   AEEResult res = AEE_SUCCESS;

   // get the QoSNetInitiated object   
   res = GetQoS(&pIDSQoS);   
   if ((res != AEE_SUCCESS) || (NULL == pIDSQoS))  {
      LOG_MSG_ERROR_1("DSSQoSNetInitiated::GetQoSStatus(): "
                      "Obj 0x%p, pIDSQoS is not available", this);
      return res;
   }   

   DSSGenScope GenScopeObject(pIDSQoS, DSSGenScope::IDSIQI_TYPE);

   // Get the QoS Status
   QoSStateInfoType statusInfo;
   memset(&statusInfo, 0, sizeof(QoSStateInfoType));
   IDS_ERR_RET(pIDSQoS->GetState(&statusInfo));
   
   // TODO: move to DSSConversion.cpp
   switch(statusInfo.state)
   {
   case QoSState::QDS_AVAILABLE_MODIFIED:
      pqos_status_type->qos_status = QOS_AVAILABLE;
      break;
   case QoSState::QDS_SUSPENDING:
      pqos_status_type->qos_status = QOS_SUSPENDING;
      break;
   case QoSState::QDS_UNAVAILABLE:
      pqos_status_type->qos_status = QOS_UNAVAILABLE;
      break;
   case QoSState::QDS_INVALID:
      pqos_status_type->qos_status = QOS_STATE_INVALID;
      break;
   case QoSState::QDS_ACTIVATING:
      pqos_status_type->qos_status = QOS_ACTIVATING;
      break;
   case QoSState::QDS_SUSPENDED:
      pqos_status_type->qos_status = QOS_DEACTIVATED;
      break;
   case QoSState::QDS_RELEASING:
      pqos_status_type->qos_status = QOS_RELEASING;
      break;
   case QoSState::QDS_CONFIGURING:
      pqos_status_type->qos_status = QOS_CONFIGURING;
      break;
   default:      
      LOG_MSG_ERROR_2 ("DSSQoSNetInitiated::GetQoSStatus(): "
                       "Obj 0x%p, unsupported state: %d",
                       this, statusInfo.state);
      return QDS_EFAULT;
   }

   return AEE_SUCCESS;
}

//===================================================================
//  FUNCTION:   DSSQoSNetInitiatedReq::GetIfaceId
//
//  DESCRIPTION:
//
//===================================================================
void DSSQoSNetInitiated::GetIfaceId(dss_iface_id_type* pIfaceId)
{
   *pIfaceId = mIfaceId;
}


//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::GetPhysLinkObject
//
//  DESCRIPTION:
//
//===================================================================
AEEResult DSSQoSNetInitiated::GetPhysLinkObject(IPhysLink** ppPhyslink)
{
   IQoS* pIDSQoS = NULL;
   AEEResult nRes = AEE_SUCCESS;

   // get the QoSNetInitiated object   
   nRes = GetQoS(&pIDSQoS);   
   if ((nRes != AEE_SUCCESS) || (NULL == pIDSQoS))  {
      LOG_MSG_ERROR_1("DSSQoSNetInitiated::GetPhysLinkObject(): "
                      "Obj 0x%p, pIDSQoS is not available", this);
      return nRes;
   }   

   return pIDSQoS->GetTXPhysLink(ppPhyslink);
}
 

//===================================================================
//  FUNCTION:    DSSQoSNetInitiated::RegEventCB  
//
//  DESCRIPTION: Serves DSS_IFACE_IOCTL_REG_EVENT_CB
//===================================================================
AEEResult DSSQoSNetInitiated::RegEventCB(dss_iface_ioctl_ev_cb_type* pEvArg,
                                         dss_iface_id_type ifaceId)
{
   DSSEventHandler* ppEventHandler = 0;
   IDS_ERR_RET(GetEventHandler(pEvArg->event, &ppEventHandler, true));

   // Register to the event.
   IDS_ERR_RET(ppEventHandler->Register(pEvArg->event, pEvArg->event_cb, pEvArg->user_data_ptr, ifaceId));

   return AEE_SUCCESS;
}

//===================================================================
//  FUNCTION:    DSSQoSNetInitiated::GetEventHandler
//
//  DESCRIPTION: Returns the DSSEventHandler for the specified event.
//               If bInit is true, this function will also initialize
//               the handler if it's not initialized.
//===================================================================
AEEResult DSSQoSNetInitiated::GetEventHandler(dss_iface_ioctl_event_enum_type event,
                                              DSSEventHandler** ppEventHandler,
                                              bool bInit)
{
   switch (event) {
      case DSS_IFACE_IOCTL_QOS_AVAILABLE_MODIFIED_EV:
      case DSS_IFACE_IOCTL_QOS_AVAILABLE_DEACTIVATED_EV:
      case DSS_IFACE_IOCTL_QOS_UNAVAILABLE_EV:
         return FetchHandler(&mpQoSNetInitiatedHandler, ppEventHandler, bInit);
      case DSS_IFACE_IOCTL_QOS_MODIFY_ACCEPTED_EV:
      case DSS_IFACE_IOCTL_QOS_MODIFY_REJECTED_EV:
        return FetchHandler(&mpQoSNetInitiatedModifyHandler, ppEventHandler, bInit);
      case DSS_IFACE_IOCTL_QOS_INFO_CODE_UPDATED_EV:
         return FetchHandler(&mpQoSNetInitiatedInfoCodeHandler, ppEventHandler, bInit);

      default:
         LOG_MSG_ERROR_2("DSSQoSNetInitiated::GetEventHandler(): "
                         "Obj 0x%p, unhandled event=%d case", this, event);
         return QDS_EFAULT;
   }
}

template<typename HandlerType>
AEEResult DSSQoSNetInitiated::FetchHandler(HandlerType** pHandler, DSSEventHandler** ppEventHandler, bool bInit)
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

void * DSSQoSNetInitiated::operator new
(
   unsigned int numBytes
)  throw()
{
   return ps_mem_get_buf( PS_MEM_DSAL_NET_QOS_NET_INITIATED_TYPE);
} /* DSSQoSNetInitiated::operator new() */


void DSSQoSNetInitiated::operator delete 
(
   void *  bufPtr
)
{
   PS_MEM_FREE(bufPtr);
   return;
} /* DSSQoSNetInitiated::operator delete() */

uint32 DSSQoSNetInitiated::GetDSSQoSNetInitiatedAssocReq(void)
{
   return mDSSQoSNetInitiatedAssocReq;
}

void DSSQoSNetInitiated::ClearDSSQoSNetInitiatedAssocReq(void)
{
   mDSSQoSNetInitiatedAssocReq = 0;
}

//===================================================================
//  FUNCTION:   DSSQoSNetInitiated::GetFlow
//
//  DESCRIPTION:
//
//===================================================================
int32 DSSQoSNetInitiated::GetFlow()
{
   IQoS* pIDSQoS = NULL;
   AEEResult res = AEE_SUCCESS;

   if(mpCritSect == NULL)
   {
     return 0;
   }
   mpCritSect->Enter();
   // get the QoSNetInitiated object   
   res = GetQoS(&pIDSQoS);   
   if ((res != AEE_SUCCESS) || (NULL == pIDSQoS))  {
      LOG_MSG_ERROR_1("DSSQoSNetInitiated::GetFlow(): "
                      "Obj 0x%p, pIDSQoS is not available", this);
      //Qos may already released
      if(mpCritSect)
      {
        mpCritSect->Leave();
      }
      return res;
   }
   DSSGenScope GenScopeObject(pIDSQoS, DSSGenScope::IDSIQI_TYPE);

   IQoSPriv* pIDSQoSPriv = NULL;
   IDS_ERR_RET(pIDSQoS->QueryInterface(AEEIID_IQoSPriv, (void**)&pIDSQoSPriv));
   DSSGenScope GenScopeQoSPriv(pIDSQoSPriv, DSSGenScope::IDSIQI_TYPE);
   //Qos may already released
   if(mpCritSect)
   {
     mpCritSect->Leave();
   }

  return (pIDSQoSPriv->GetFlowPriv());

}

