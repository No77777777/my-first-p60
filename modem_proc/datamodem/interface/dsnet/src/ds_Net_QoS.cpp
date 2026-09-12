/*===========================================================================
  FILE: QoS.cpp

  OVERVIEW: This file provides implementation of the QoS class.

  DEPENDENCIES: None

  Copyright (c) 2007 - 2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_QoS.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-09-30 kr  Free floating changes
  2008-03-25 hm  Created module.

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_QoS.h"
#include "ds_Net_Platform.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_EventManager.h"
#include "ds_Net_QoSFlowSpec.h"
#include "ds_Net_IQoS1x.h"
#include "ds_Net_QoS1X.h"

using namespace ds::Net;
using namespace ds::Error;
using namespace NetPlatform;

/*===========================================================================

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
QoS::QoS
(
  int32            ifaceHandle,
  int32            flowHandle,
  NetworkModeType  networkMode
) : Handle (flowHandle),
    mIfaceHandle (ifaceHandle),
    mpPhysLink (NULL),
    mpQoS1x(0),
    mNetworkMode(networkMode)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_4 ("QoS::QoS(): Obj 0x%p, iface handle 0x%x, "
                            "flow handle 0x%x, mode %d",
                            this, ifaceHandle, flowHandle, networkMode);
} /* QoS::QoS() */

void QoS::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  DS_UTILS_RELEASEIF (mpPhysLink);
  DS_UTILS_RELEASEIF (mpQoS1x);

  /*-------------------------------------------------------------------------
    Should be the last statement. Call destructor of the base class.
  -------------------------------------------------------------------------*/
  Handle::Destructor();

  mpICritSect->Leave();

} /* QoS::Destructor() */

QoS::~QoS
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    NO-OP: used only for freeing memory
  -------------------------------------------------------------------------*/

} /* QoS::~QoS() */

ds::ErrorType QoS::GetTXPhysLink
(
  ::IPhysLink **  txPhysLinkObj
)
{
  ds::ErrorType  result;
  int32          physLinkHandle;
  int32          flowHandle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == txPhysLinkObj)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      A QoS object can at most be associated with only one flow object.
      If the phys link is already set, return the same.
    -----------------------------------------------------------------------*/
    if (0 != mpPhysLink)
    {
      *txPhysLinkObj = static_cast <IPhysLink *> (mpPhysLink);
      result         = AEE_SUCCESS;

      (void) mpPhysLink->AddRef();
      break;
    }

    /*-----------------------------------------------------------------------
      Get the phys link associated with the flow handle of this QoS object
      Get the flow handle for the QoS object.
    -----------------------------------------------------------------------*/
    flowHandle = GetHandle();
    result     = NetPlatform::PSGetPhysLinkFromFlow (flowHandle,
                                                     &physLinkHandle);
    if (AEE_SUCCESS != result || 0 == physLinkHandle)
    {
      /*---------------------------------------------------------------------
        Get the phys link from iface instead.
        This condition can happen if the iface is not yet brought up
        and hence the default flow is not bound to phys link.
        This is valid for default flow only
      ---------------------------------------------------------------------*/
      if (true == NetPlatform::IsPSFlowDefault(flowHandle))
      {
        result = NetPlatform::PSGetPhysLinkFromIface (mIfaceHandle,
                                                      &physLinkHandle);
        if (AEE_SUCCESS != result || 0 == physLinkHandle)
        {
          break;
        }
      }
      else // cannot fetch the valid handle from PS
      {
        break;
      }
    }

    /*-----------------------------------------------------------------------
      Create a new phys link object
    -----------------------------------------------------------------------*/
    mpPhysLink = new PhysLink (physLinkHandle, mNetworkMode);
    if (0 == mpPhysLink)
    {
      LOG_MSG_ERROR_1 ("QoS::GetTXPhysLink(): Failed to alloc PhysLink, "
                       "obj 0x%p", this);
      result = AEE_ENOMEMORY;
      break;
    }

    *txPhysLinkObj = static_cast <IPhysLink *> (mpPhysLink);
    (void) mpPhysLink->AddRef();
  } while (0);

  LOG_MSG_FUNCTION_EXIT_3 ("QoS::GetTXPhysLink(): "
                           "Result 0x%x, tx phys link 0x%p, obj 0x%p",
                           result, *txPhysLinkObj, this);
  mpICritSect->Leave();

  return result;
} /* QoS::GetTXPhysLink() */


ds::ErrorType QoS::GetRXPhysLink
(
  ::IPhysLink **  rxPhysLinkObj
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Current implementation treats RX and TX as same phys links
  -------------------------------------------------------------------------*/
  return GetTXPhysLink(rxPhysLinkObj);

} /* QoS::GetRXPhysLink() */

ds::ErrorType QoS::GetModifyResult
(
  QoSResultType *  resultCode
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = FlowIoctlNonNullArg (GetHandle(),
                                FLOW_IOCTL_GET_MODIFY_RESULT,
                                static_cast <void *> (resultCode));

  mpICritSect->Leave();
  return result;

} /* QoS::GetModifyResult() */


ds::ErrorType QoS::GetUpdatedInfoCode
(
  QoSInfoCodeType *  infoCode
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = FlowIoctlNonNullArg (GetHandle(),
                                FLOW_IOCTL_GET_FLOW_UPDATED_INFO_CODE,
                                static_cast <void *> (infoCode));

  mpICritSect->Leave();
  return result;

} /* QoS::GetUpdatedInfoCode() */

ds::ErrorType QoS::GetTXQueueLevel
(
  QoSTXQueueLevelType *  argTXQLevel
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = FlowIoctlNonNullArg (GetHandle(),
                                FLOW_IOCTL_GET_TX_QUEUE_LEVEL,
                                static_cast <void *> (argTXQLevel));

  mpICritSect->Leave();
  return result;

} /* QoS::GetTXQueueLevel() */

ds::ErrorType QoS::GetGrantedFlowSpecInternal
(
  NetPlatform::FlowIoctlEnumType  ioctlKind,
  ::IQoSFlowPriv **               rxFlowObj,
  ::IQoSFlowPriv **               txFlowObj,
  uint8 **                        bearerID
)
{
  NetPlatform::QoSGetGrantedFlowSpecType    grantedFlowSpec;
  ds::ErrorType                             result;
  int32                                     flowHandle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_4("QoS::GetGrantedFlowSpecInternal(): "
                           "obj 0x%p,tx flow 0x%p, rx flow 0x%p,ioctl king %d",
                           this, txFlowObj, rxFlowObj, ioctlKind);

  if (0 == GetHandle())
  {
     LOG_MSG_INFO1_1 ("QoS::GetGrantedFlowSpecInternal(): "
                      "NULL handle, obj 0x%p", this);
     return QDS_EINVAL;
  }

  if (0 == txFlowObj || 0 == rxFlowObj)
  {
    return QDS_EFAULT;
  }

  if (NetPlatform::FLOW_IOCTL_QOS_GET_GRANTED_FLOW_SPEC2 != ioctlKind &&
      NetPlatform::FLOW_IOCTL_PRIMARY_QOS_GET_GRANTED_FLOW_SPEC != ioctlKind)
  {
    return QDS_EFAULT;
  }

  *txFlowObj = *rxFlowObj = NULL;

  mpICritSect->Enter();

  do
  {
    flowHandle = GetHandle();
    if (0 == flowHandle)
    {
      result = QDS_EINVAL;
      break;
    }

    memset (&grantedFlowSpec, 0, sizeof(grantedFlowSpec));

    /*-----------------------------------------------------------------------
      Perform flow IOCTL to get granted flow spec.
    -----------------------------------------------------------------------*/
    result = FlowIoctl (flowHandle,
                        ioctlKind,
                        static_cast <void *> (&grantedFlowSpec));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert to out args
    -----------------------------------------------------------------------*/
    *txFlowObj = static_cast <IQoSFlowPriv *>
                   (new QoSFlowSpec(&grantedFlowSpec.tx_ip_flow));
    *rxFlowObj = static_cast <IQoSFlowPriv *>
                   (new QoSFlowSpec(&grantedFlowSpec.rx_ip_flow));
    **bearerID  = grantedFlowSpec.bearer_id;

    if (0 == *txFlowObj || 0 == *rxFlowObj)
    {
      result = AEE_ENOMEMORY;
      break;
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_6 ("QoS::GetGrantedFlowSpecInternal(): "
                           "Result 0x%x, tx flow spec 0x%p, "
                           "rx flow spec 0x%p, obj 0x%p, ioctlKind %d,"
                           "flow handle %d",
                           result, *txFlowObj, *rxFlowObj, this, ioctlKind,
                           flowHandle);
  mpICritSect->Leave();

  if (AEE_SUCCESS != result)
  {
    DS_UTILS_RELEASEIF (*txFlowObj);
    DS_UTILS_RELEASEIF (*rxFlowObj);
  }

  return result;
} /* QoS::GetGrantedFlowSpecInternal() */

ds::ErrorType QoS::GetQoS1x
(
  IQoS1x **  ppQoS1x
)
{
  ds::ErrorType  result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == ppQoS1x)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Create QoS1x object and cache it in QoS object, if not cached before
    -----------------------------------------------------------------------*/
    if (0 == mpQoS1x)
    {
      mpQoS1x = new QoS1X(GetHandle());
      if (0 == mpQoS1x)
      {
        LOG_MSG_ERROR_1 ("QoS::GetQoS1x(): Failed to alloc QoS1x, obj 0x%p",
                         this);
        result = AEE_ENOMEMORY;
        break;
      }
    }

    *ppQoS1x = mpQoS1x;

    (void) mpQoS1x->AddRef();
  } while (0);

  LOG_MSG_FUNCTION_EXIT_3 ("QoS::GetQoS1x(): Result 0x%x, QoS 1x 0x%p, obj 0x%p",
                           result, *ppQoS1x, this);
  mpICritSect->Leave();

  return result;
} /* QoS::GetQoS1x() */

ds::ErrorType QoS::GetTechObject
(
  AEEIID   iid,
  void **  ppo
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("QoS::GetTechObject(): Obj 0x%p, iid 0x%x, ppo 0x%p",
                            this, iid, ppo);

  if (0 == ppo)
  {
    return QDS_EFAULT;
  }

  if (AEEIID_IQoS1x != iid)
  {
    return AEE_ECLASSNOTSUPPORT;
  }

  return GetQoS1x((IQoS1x **) ppo);
} /* QoS::GetTechObject() */

int32 QoS::GetFlowhandle()
{
  return GetHandle();
} /* QoS::GetFlowhandle() */

