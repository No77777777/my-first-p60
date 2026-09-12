/*===========================================================================
  FILE: QoSNetInitiated.cpp

  OVERVIEW: This file provides implementation of the QoSNetInitiated class.

  DEPENDENCIES: None

  Copyright (c) 2011-2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_QoSNetInitiated.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-09-30 kr  Free floating changes
  2011-07-07 sz  Created module.

===========================================================================*/

#include "comdef.h"

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ps_iface_defs.h"
#include "ds_Net_QoSNetInitiated.h"
#include "ds_Net_Conversion.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CSignalBus.h"
#include "ds_Net_EventManager.h"
#include "dss_errors_def.h"

using namespace ds::Net;
using namespace ds::Net::Conversion;
using namespace ds::Error;
using namespace NetPlatform;  
using namespace dss::Error;

#define DS_NET_QOSINIT_ASSERT(expression) \
         dsnet_qosinit_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dsnet_qosinit_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_NET_INIT_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_qosinit_assert_wrapper
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
QoSNetInitiated::QoSNetInitiated
(
  int32                         ifaceHandle,
  int32                         flowHandle,
  NetworkModeType               networkMode
)
: QoS (ifaceHandle, flowHandle, networkMode),    
  refCnt (1),
  weakRefCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_3 ("QoSNetInitiated::QoSNetInitiated(): "
                   "Creating object 0x%p, if handle 0x%x, flow handle 0x%x",
                   this, ifaceHandle, flowHandle);

  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,
                                 (void **) &mpSigBusStateChange);
  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,    
                                 (void **) &mpSigBusModifyResult);
  (void) DS_Utils_CreateInstance(0,
                                 AEECLSID_CSignalBus,    
                                 (void **) &mpSigBusInfoCodeUpdated);

  if (AEE_SUCCESS != Handle::Init(EventManager::qosObjList))
  {
    LOG_MSG_ERROR_1 ("QoSNetInitiated::QoSNetInitiated(): "
                     "Handle::Init failed, Obj 0x%p", this);
    return;
  }
} /* QoSNetInitiated::QoSNetInitiated() */

void QoSNetInitiated::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_UTILS_RELEASEIF (mpSigBusStateChange);
  DS_UTILS_RELEASEIF (mpSigBusModifyResult);
  DS_UTILS_RELEASEIF (mpSigBusInfoCodeUpdated);
  
  //Should be last statement. Call destructor for the base class.
  QoS::Destructor();

} /* QoSNetInitiated::Destructor() */

QoSNetInitiated::~QoSNetInitiated
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  //NO-OP: only used for freeing memory.

} /* QoSNetInitiated::~QoSNetInitiated() */


int QoSNetInitiated::GetState  
(
  QoSStateInfoType* argStatusInfo
)
{      
  PSFlowStateType        flowState;
  int32 result = 0;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Validation.
  -------------------------------------------------------------------------*/
  if (NULL == argStatusInfo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  if (0 == GetHandle())
  {
    argStatusInfo->infoCode  = QoSInfoCode::QDS_NOT_SPECIFIED;
    argStatusInfo->state     = QoSState::QDS_UNAVAILABLE;
    mpICritSect->Leave();
    return AEE_SUCCESS;
  }

  /*-----------------------------------------------------------------------
    Call the flow IOCTL to get the status of the Qos session.
  -----------------------------------------------------------------------*/
  result = FlowIoctl  (GetHandle(),
                       FLOW_IOCTL_QOS_GET_STATUS,
                       (void *)&flowState);
  if (AEE_SUCCESS != result)
  {
    ds_net_error_code = DSS_ERROR_FLOW_IOCTL_GET_QOS_STATUS_FAILED;
    mpICritSect->Leave();
    goto bail;
  }

  /*-----------------------------------------------------------------------
    Call the flow IOCTL to get the info code of the Qos session.
  -----------------------------------------------------------------------*/
  result = FlowIoctl  (GetHandle(),
                       FLOW_IOCTL_GET_FLOW_UPDATED_INFO_CODE,
                       (void *)&(argStatusInfo->infoCode));
  if (AEE_SUCCESS != result)
  {
    ds_net_error_code = DSS_ERROR_FLOW_IOCTL_GET_FLOW_UPDATED_INFO_FAILED;
    mpICritSect->Leave();
    goto bail;
  }

  argStatusInfo->state = PS2DSFlowState(flowState);

  LOG_MSG_INFO1_3 ("QoSNetInitiated::GetState(): "
                   "Obj 0x%p, state %d, info code %d",
                   this, argStatusInfo->state, argStatusInfo->infoCode);

  mpICritSect->Leave();
  return AEE_SUCCESS;

bail:
  
  LOG_MSG_ERROR_3 ("QoSNetInitiated::GetState(): "
                   "Result %d, error code %d, obj 0x%p",
                   result, ds_net_error_code, this);
  return result;
} /* GetState() */


/*---------------------------------------------------------------------------
  Inherited functions from IQoSPriv.
---------------------------------------------------------------------------*/
ds::ErrorType QoSNetInitiated::GetGrantedFlowSpecPriv
(
 ::IQoSFlowPriv** rxFlow,
 ::IQoSFlowPriv** txFlow,
 uint8**          bearerID
 )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == GetHandle())
  {
    LOG_MSG_INFO1_1 ("QoSNetInitiated::GetGrantedFlowSpecPriv(): "
                     "NULL handle, obj 0x%p", this);
    return QDS_EINVAL;
  }

  return QoS::GetGrantedFlowSpecInternal(
    FLOW_IOCTL_QOS_GET_GRANTED_FLOW_SPEC2,
    rxFlow,
    txFlow,
    bearerID);

} /* GetGrantedFlowSpecPriv() */


/*---------------------------------------------------------------------------
  Event processing.
---------------------------------------------------------------------------*/
boolean QoSNetInitiated::Process
(
  void* pUserData
)
{
  EventInfoType*          pEventInfo;
/*-------------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
    User data should never be NULL for event processing.
  -------------------------------------------------------------------------*/
  if (NULL == pUserData)
  {
    DS_NET_QOSINIT_ASSERT (0);
    return FALSE;
  }

  pEventInfo = static_cast <EventInfoType *> (pUserData);

  mpICritSect->Enter();
  
  LOG_MSG_INFO1_3 ("QoSNetInitiated::Process(): "
                   "this handle 0x%x, ev 0x%x, ev handle 0x%x",
                   GetHandle(), pEventInfo->eventName, pEventInfo->handle);

  if (GetHandle() == pEventInfo->handle)
  {
    /*-----------------------------------------------------------------------
      For FLOW_NULL event, reset the handle before notifying.
    -----------------------------------------------------------------------*/
    if (FLOW_NULL_EV == pEventInfo->psEventName)
    {
      SetHandle (0);
    }

    /*-----------------------------------------------------------------------
      Event belongs to this handle. Call Notify()
    -----------------------------------------------------------------------*/
    Notify (pEventInfo->eventName);
  }

  mpICritSect->Leave();
  return TRUE;

} /* Process() */


ds::ErrorType QoSNetInitiated::GetSignalBus
(
  ds::Net::EventType  eventID,
  ISignalBus **       ppISigBus
)
{
  ds::ErrorType ret = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == ppISigBus)
  {
    DS_NET_QOSINIT_ASSERT(0);
    return QDS_EFAULT;
  }

  mpICritSect->Enter();
  switch (eventID)
  {
    case QoSEvent::QDS_EV_STATE_CHANGED:
      *ppISigBus = mpSigBusStateChange;
      break;

    case QoSEvent::QDS_EV_MODIFY_RESULT:
      *ppISigBus = mpSigBusModifyResult;
      break;

    case QoSEvent::QDS_EV_INFO_CODE_UPDATED:
      *ppISigBus = mpSigBusInfoCodeUpdated;
      break;

    default:
      *ppISigBus = NULL;
      mpICritSect->Leave();
      LOG_MSG_ERROR_2 ("QoSNetInitiated::GetSignalBus(): "
                       "event not supported Obj 0x%p event 0x%x",
                       this, eventID);

      ret = QDS_EINVAL;
      goto bail;
      
  }

  (void)(*ppISigBus)->AddRef();
  mpICritSect->Leave();

bail:

  return ret;

} /* GetSignalBus() */

int QoSNetInitiated::QueryInterface
(
 AEEIID iid,
 void **ppo
 )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (iid)
  {
  case AEEIID_IQoSNetInitiated:
    *ppo = static_cast <IQoSNetInitiated *> (this);
    (void) AddRef();
    break;

  case AEEIID_IQoSPriv:
    *ppo = static_cast <IQoSPriv *> (this);
    (void) AddRef();
    break;

  case AEEIID_IQI:
    *ppo = reinterpret_cast <void *> (this);
    (void) AddRef ();
    break;

  default:
    LOG_MSG_ERROR_1 ("QoSNetInitiated::QueryInterface(): "
                     "Class 0x%x unsupported", iid);
    return AEE_ECLASSNOTSUPPORT;
  }

  return AEE_SUCCESS;

}/* QueryInterface() */


ds::ErrorType QoSNetInitiated::GetGrantedFilterSpec
(
  IIPFilterPriv** rxFilter,
  int rxFilterLen,
  int* rxFilterLenReq,
  IIPFilterPriv** txFilter,
  int txFilterLen,
  int* txFilterLenReq
)
{
  int32 result = 0;
  int32 flowHandle = 0;
  NetPlatform::QoSGetGrantedFilterSpecType  getFilterSpec;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();
  memset(&getFilterSpec, 0, sizeof(QoSGetGrantedFilterSpecType));

  flowHandle = GetHandle();
  if (0 == flowHandle)
  {
    result = QDS_EINVAL;
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Perform flow IOCTL to get granted flow spec.
  -------------------------------------------------------------------------*/
  result = FlowIoctl (flowHandle,
                      FLOW_IOCTL_QOS_GET_GRANTED_FILTER_SPEC,
                      (void *)&getFilterSpec);
  if (AEE_SUCCESS != result)
  {
    ds_net_error_code = DSS_ERROR_FLOW_IOCTL_QOS_GET_GRANTED_FILTER_SPEC_FAILED;
    goto bail;
  }

  result = PS2DSIPFilterSpecType(&(getFilterSpec.rx_filter_spec),
                                 (IPFilterSpec**)(rxFilter),
                                 rxFilterLen,
                                 rxFilterLenReq);

  if(AEE_SUCCESS != result)
  {
    ds_net_error_code = DSS_ERROR_PS2_DSIP_FILTER_SPEC_TYPE_RX_FAILED;
    goto bail;
  }

  result = PS2DSIPFilterSpecType(&(getFilterSpec.tx_filter_spec),
                                 (IPFilterSpec**)(txFilter),
                                 txFilterLen,
                                 txFilterLenReq);

  if(AEE_SUCCESS != result)
  {
    ds_net_error_code = DSS_ERROR_PS2_DSIP_FILTER_SPEC_TYPE_TX_FAILED;
    goto bail;
  }

  result = AEE_SUCCESS;
  mpICritSect->Leave();
  return result;

bail:
  mpICritSect->Leave();
  LOG_MSG_ERROR_3 ("QoSNetInitiated::GetGrantedFilterSpec(): "
                   "result %d, error code %d, obj 0x%p",
                   result, ds_net_error_code, this);
    
  return result;

}/* GetGrantedFilterSpec() */

int32 QoSNetInitiated::GetQoSNetInitiatedId()
{
  return GetHandle();
} /* GetQoSNetInitiatedId() */

int32 QoSNetInitiated::GetFlowPriv()
{
  return QoS::GetFlowhandle();
} /* GetFlowPriv() */


