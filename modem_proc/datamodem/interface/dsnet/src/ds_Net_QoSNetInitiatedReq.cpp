/*===========================================================================
  FILE: QoSNetInitiatedReq.cpp

  OVERVIEW: This file provides implementation of the QoSNetInitiatedReq class.

  DEPENDENCIES: None

  Copyright (c) 2011 - 2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_QoSNetInitiatedReq.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2015-06-24 pg  Added to support qos handle when registration to net initiated qos.
  2011-06-05 sz  Created module.

===========================================================================*/
#include "comdef.h"

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ps_iface_defs.h"
#include "ds_Net_QoSNetInitiatedReq.h"
#include "ds_Net_Conversion.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CSignalBus.h"
#include "ds_Net_EventManager.h"
#include "ds_Net_IQoSNetInitiated.h"

using namespace ds::Net;
using namespace ds::Net::Conversion;
using namespace ds::Error;
using namespace NetPlatform;

#define DS_NET_QOSREQ_ASSERT(expression) \
         dsnet_qosreq_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dsnet_init_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_NET_INIT_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_qosreq_assert_wrapper
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
}/* dsnet_init_assert_wrapper */

/*===========================================================================

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  CONSTRUCTOR/DESTRUCTOR
---------------------------------------------------------------------------*/
QoSNetInitiatedReq::QoSNetInitiatedReq
(
  int32                         ifaceHandle,
  int32                         qosNetInitiatedReqHandle
)
  /*-----------------------------------------------------------------------
    QoSNetInitiatedReqHandle is mObjHandle for this class.
  -----------------------------------------------------------------------*/
: Handle (qosNetInitiatedReqHandle),
  mIfaceHandle (ifaceHandle),
  mpSigBusStateChange(NULL),
  mMatchingFlowHandlesQueue(NULL),
  refCnt (1),
  weakRefCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_3 ("QoSNetInitiatedReq::QoSNetInitiatedReq(): "
                   "Creating object 0x%p, if handle 0x%x, req handle 0x%x",
                   this, ifaceHandle, qosNetInitiatedReqHandle);

  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,
                                 (void **) &mpSigBusStateChange);

  mMatchingFlowHandlesQueue = new Utils::List();

  if (AEE_SUCCESS != Handle::Init(EventManager::qosObjList))
  {
    LOG_MSG_ERROR_1 ("QoSNetInitiatedReq::QoSNetInitiatedReq(): "
                     "Handle::Init failed, Obj 0x%p", this);
    return;
  }

} /* QoSNetInitiatedReq::QoSNetInitiatedReq() */

void QoSNetInitiatedReq::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  // prevent new events from coming from ps_iface objects
    Close();

  DS_UTILS_RELEASEIF (mpSigBusStateChange);

  if (0 != mMatchingFlowHandlesQueue) {
    delete mMatchingFlowHandlesQueue;
  }

  //Should be last statement. Call destructor for the base class.
  Handle::Destructor();

  mpICritSect->Leave();

} /* QoSNetInitiatedReq::Destructor() */


QoSNetInitiatedReq::~QoSNetInitiatedReq
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  //NO-OP: only used for freeing memory.

} /* QoSNetInitiatedReq::~QoSNetInitiatedReq() */


ds::ErrorType QoSNetInitiatedReq::OnStateChange 
(
 ::ISignal*            signalObj, 
 ds::Net::EventType    eventID,
 IQI**                 regObj
 )
{
  AEEResult res = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    We are intentionally registering this object for NIQ notifications 
    from ps_iface before asking DSS to process real time NIQs.
    This shall allow us to get events for additional NIQs in case Network
    initiates them while DSS is handling NIQ notifications as part of real
    time check.
  -------------------------------------------------------------------------*/
  res = Handle::OnStateChange(signalObj, eventID, regObj);
  if (AEE_SUCCESS != res) {
    LOG_MSG_ERROR_3 ("QoSNetInitiatedReq::OnStateChange(): "
                     "Handle::OnStateChange failed, Obj 0x%p, event %d, error %d",
                     this, eventID, res);
    return res;
  }

  /*-------------------------------------------------------------------------
    Real Time check for matching flows
  -------------------------------------------------------------------------*/
  if (mMatchingFlowHandlesQueue->Count() > 0)
  {
    (void) signalObj->AddRef();
    (void) signalObj->Set();
    (void) signalObj->Release();
  }

  return res;
}

int QoSNetInitiatedReq::Close
(
  void
)
{
  /* Specification for PS IOCTL */
  QoSNetInitiatedReqReleaseType  qosNetInitiatedReqReleaseInfo;     
  AEEResult res = AEE_SUCCESS;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset (&qosNetInitiatedReqReleaseInfo, 0, sizeof(qosNetInitiatedReqReleaseInfo));
  
  /* Get the handle for the PS QoS Network Initiated object */
  qosNetInitiatedReqReleaseInfo.handle = GetHandle();
  
  if (0 != qosNetInitiatedReqReleaseInfo.handle) {
    
  /*-------------------------------------------------------------------------
    Perform Network Initiated QoS Release Monitoring IOCTL on the iface handle.
  -------------------------------------------------------------------------*/
  res =  IfaceIoctl (mIfaceHandle,
                     IFACE_IOCTL_QOS_NET_INITIATED_RELEASE,
                     static_cast <void *> (&qosNetInitiatedReqReleaseInfo));

  if (AEE_SUCCESS != res)
  {
    LOG_MSG_ERROR_3("QoSNetInitiatedReq::Close(): "
                    "IFACE_IOCTL_QOS_NET_INITIATED_RELEASE failed, "
                    "Obj 0x%p request handle 0x%x result %d",
                    this, qosNetInitiatedReqReleaseInfo.handle, res);
  }

  SetHandle(0);
  }

  return res;
}

int QoSNetInitiatedReq::GetLastMatchingFlowHandle
(
  int32* pFlowHandle,
  QoSFltrMatchDirType* pFltrMatchDir
)
{
  if (NULL == pFlowHandle)
  {
     return QDS_EFAULT;
  }

  mpICritSect->Enter();

  FlowHandle* pHandle = (FlowHandle*) mMatchingFlowHandlesQueue->PopFront();
  if (NULL == pHandle)
  {
    mpICritSect->Leave();
    LOG_MSG_ERROR_0 ("QoSNetInitiatedReq::GetLastMatchingFlowHandle(): "
                     "FlowHandleQueue is empty");
    return QDS_EFAULT;
  }
  
  *pFlowHandle = pHandle->GetHandle();
  *pFltrMatchDir = pHandle->GetFltrMatchDir();
  /*-------------------------------------------------------------------
    Need to call release twice here, because the object was added to the 
    list with refCnt 1
  -------------------------------------------------------------------*/
  pHandle->Release();
  DS_UTILS_RELEASEIF(pHandle);
  mpICritSect->Leave();

   return AEE_SUCCESS;
}

boolean QoSNetInitiatedReq::IsPendingMatchingFlow
(
 void
)
{
  return (mMatchingFlowHandlesQueue->Count() > 0);
}

/*---------------------------------------------------------------------------
  Event processing.
---------------------------------------------------------------------------*/
boolean QoSNetInitiatedReq::Process
(
  void* pUserData
)
{
  EventInfoType* pEventInfo = NULL;
  ps_iface_net_initiated_qos_available_info_type* flowAddedEventInfo = NULL;
  int32 handle = 0;
/*-------------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
    User data should never be NULL for event processing.
  -------------------------------------------------------------------------*/
  if (NULL == pUserData)
  {
    DS_NET_QOSREQ_ASSERT (0);
    return FALSE;
  }

  pEventInfo = static_cast <EventInfoType *> (pUserData);

  LOG_MSG_INFO1_5 ("QoSNetInitiatedReq::Process(): "
                   "obj 0x%p handle 0x%x psEventName %d eventName 0x%x user handle 0x%x",
                   this,
                   GetHandle(),
                   pEventInfo->psEventName,
                   pEventInfo->eventName,
                   pEventInfo->userHandle);

  mpICritSect->Enter();

  do
  {     
    handle = GetHandle();

    // pEventInfo->userHandle (unlike pEventInfo->handle) may be 0 for some events.
    // Not all events use this data field.
    // QoSNetInitiatedReq object handle may be 0 if we already called Close(),
    // see dss_iface_ioctl_qos_net_initiated_release().
    // We can call Close() without releasing the object
    // (to prevent processing of further events).
    // So this if statement (handle == 0) prevents a case where we called Close()
    // and then Process() events with pEventInfo->userHandle 0,
    // which are irrelevant for this object.
    // I.e. QoSNetInitiatedReq object shouldn't process events if its handle is 0.
    if (handle != pEventInfo->userHandle || handle == 0)
    {
      // event should be disregarded
      break;
    }

    /* Store the flowHandle. It shall be needed to create a corresponding
       ds::Net::QoSNetInitiated object upon QCMAPP/DSS request */
    flowAddedEventInfo = (ps_iface_net_initiated_qos_available_info_type*)
                           (pEventInfo->psEventInfo);
    // Check if the direction is valid
    if (PS_QOS_FILTR_DIR_MAX > flowAddedEventInfo->fltr_match_dir)
    {
      FlowHandle* flowHandle = new FlowHandle((int32)flowAddedEventInfo->flow_ptr,
                                              flowAddedEventInfo->fltr_match_dir);
      if((NULL != flowHandle) && mMatchingFlowHandlesQueue->Count())
      {
         FlowHandle* oldHandle = CheckDup( mMatchingFlowHandlesQueue,
                                           (int32 *)flowHandle);
         if(NULL != oldHandle)
         {
           oldHandle->SetFltrMatchDir(flowAddedEventInfo->fltr_match_dir);
           delete flowHandle;
           break;
         }
       }
      (void) mMatchingFlowHandlesQueue->PushBack(flowHandle);
    }
    
    /*-----------------------------------------------------------------------
     Event belongs to this handle. Call Notify()
    -----------------------------------------------------------------------*/
    Notify (pEventInfo->eventName);
  } while (0);

  mpICritSect->Leave();

  return TRUE;

} /* Process() */


ds::ErrorType QoSNetInitiatedReq::GetSignalBus
(
  ds::Net::EventType  eventID,
  ISignalBus **       ppISigBus
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == ppISigBus)
  {
    DS_NET_QOSREQ_ASSERT(0);
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  switch (eventID)
  {
    case QoSNetInitiatedReqEvent::QDS_EV_STATE_CHANGED:
      *ppISigBus = mpSigBusStateChange;
      (void)(*ppISigBus)->AddRef();
      mpICritSect->Leave();
      return AEE_SUCCESS;

    default:
      *ppISigBus = NULL;
      mpICritSect->Leave();
      return QDS_EINVAL;
  }

} /* GetSignalBus() */

int QoSNetInitiatedReq::QueryInterface
(
 AEEIID iid,
 void **ppo
 )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (iid)
  {
  case AEEIID_IQoSNetInitiatedReq:
    *ppo = static_cast <IQoSNetInitiatedReq *> (this);
    (void) AddRef();
    break;

  case AEEIID_IQI:
    *ppo = reinterpret_cast <void *> (this);
    (void) AddRef ();
    break;

  default:
    LOG_MSG_ERROR_2 ("QoSNetInitiatedReq::QueryInterface(): "
                     "Class 0x%x unsupported", this, iid);
    return AEE_ECLASSNOTSUPPORT;
  }

  return AEE_SUCCESS;

}/* QueryInterface() */

ds::ErrorType QoSNetInitiatedReq::RealTimeCheck
(
  const QoSSpecNetInitiatedType* pQoSNetInitiatedSpec,
  QoSFltrMatchDirType            direction
)
{
  bool       noMemory = false;
  bool       checkFailed = false;
  int16      ps_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2  ("QoSNetInitiatedReq::RealTimeCheck"
                             "Obj 0x%p, if handle 0x%x", this, mIfaceHandle);

  /* Specification for PS IOCTL */
  ps_iface_ioctl_fltr_check_type     cl_fltr_type;
  ps_iface_ioctl_matching_flow_type* matching_flow_ptr = NULL;
  FlowHandle* pFlowHandle = NULL;
  int res = 0, res2 = 0;

  memset (&cl_fltr_type, 0, sizeof(ps_iface_ioctl_fltr_check_type));
  
  res = DS2PSQoSNetInitiatedRequestSpec(pQoSNetInitiatedSpec, 
                                        (PSQoSNetInitiatedSpecType*) &cl_fltr_type.fltr_spec);
  if (AEE_SUCCESS != res)
  {
    LOG_MSG_ERROR_1("QoSNetInitiatedReq::RealTimeCheck(): "
                    "DS2PSQoSNetInitiatedRequestSpec() failed, "
                    "result %d", res);
    return res;
  }

  mpICritSect->Enter();
  list_init(&cl_fltr_type.matching_flow_list);
  cl_fltr_type.fltr_match_dir = (ps_iface_qos_fltr_match_dir_enum_type) direction;
  /*-------------------------------------------------------------------------
    Perform Network Initiated QoS Real Time Check IOCTL on the iface handle.
  -------------------------------------------------------------------------*/
  res2 = ps_iface_ioctl((ps_iface_type*) mIfaceHandle,
                        PS_IFACE_IOCTL_GET_FLOW_LIST_FROM_FLTR,
                        &cl_fltr_type,
                        &ps_errno);
  
  if ((AEE_SUCCESS != res2) && (AEE_EWOULDBLOCK != res2))
  {
    checkFailed = true;
    goto bail;
  }
  matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
    list_pop_front(&cl_fltr_type.matching_flow_list);
  while (NULL != matching_flow_ptr) 
  {
    pFlowHandle = new FlowHandle((int32)matching_flow_ptr->flow_ptr,
                                 matching_flow_ptr->fltr_match_dir);
    if (NULL == pFlowHandle) {
      noMemory = true;
      res = AEE_ENOMEMORY;
      goto bail;
    }

    mMatchingFlowHandlesQueue->PushBack(pFlowHandle);
    PS_SYSTEM_HEAP_MEM_FREE(matching_flow_ptr);
    matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
      list_pop_front(&cl_fltr_type.matching_flow_list);
  }

/* fall through */

bail:
  /*-------------------------------------------------------------------------
    Free the memory allocated in DS2PSQoSNetInitiatedRequestSpec
  -------------------------------------------------------------------------*/
  (void) CleanupPSQoSNetInitiatedSpec ((PSQoSNetInitiatedSpecType*) &cl_fltr_type.fltr_spec);

  /*-------------------------------------------------------------------------
    Empty the matching flow list and destroy it
  -------------------------------------------------------------------------*/
  matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
    list_pop_front(&cl_fltr_type.matching_flow_list);
  while (NULL != matching_flow_ptr) 
  {
    PS_SYSTEM_HEAP_MEM_FREE(matching_flow_ptr);
    matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
      list_pop_front(&cl_fltr_type.matching_flow_list);
  }
  list_destroy(&cl_fltr_type.matching_flow_list);
  mpICritSect->Leave();
  
  LOG_MSG_FUNCTION_EXIT_5 ("QoSNetInitiatedReq::RealTimeCheck"
                           "Obj 0x%p, res %d, res2 %d"
                           "IFACE IOCTL_QOS_NET REAL_TIME_CHECK failed %d"
                           "No memory for creating FlowHandle object %d",
                           this, res, res2, checkFailed, noMemory);

  if (AEE_EWOULDBLOCK == res2 )
  {
    res = AEE_EWOULDBLOCK;
  }
  return res;
}

FlowHandle* QoSNetInitiatedReq::CheckDup(ds::Utils::List *list, int32 *flow)
{
  FlowHandle *mflow = (FlowHandle *)flow;
  while(list->CurrentItem())
  {
    FlowHandle *mq = (FlowHandle *)list->CurrentItem();
    if(mflow->GetHandle() == mq->GetHandle())
    {
      return mq;
    }
    list->Next();
  }
  return NULL;
}/* CheckDup */

