/*======================================================

FILE:  DSS_NetworkStateHandler.cpp

GENERAL DESCRIPTION:
   Implementation of DSS_NetworkStateHandler functions

=====================================================

Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_NetworkStateHandler.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-13 en  History added.

===========================================================================*/

#include "DSS_NetworkStateHandler.h"
#include "DSS_Common.h"
#include "DSS_IDSNetworkPrivScope.h"
#include "DSS_IDSNetworkScope.h"
#include "dss_errors_def.h"

using namespace ds::Net;
using namespace dss::Error;

DSSNetworkStateHandler::DSSNetworkStateHandler() :
  DSSEventHandlerNetApp()
{
  mEventType = EVENT_HANDLER_NETWORK_STATE;
}

void DSSNetworkStateHandler::EventOccurred()
{
  DSSIDSNetworkPrivScope IDSNetworkPrivScope;
  DSSIDSNetworkScope IDSNetworkScope;
  dss_iface_ioctl_event_info_union_type eventInfo;
  NetworkStateType previousNetState = NetworkState::QDS_CLOSED;
  AEEResult res = AEE_SUCCESS;
  dss_error_enum_type dss_error_code = DSS_ERROR_MIN;

  if (mpHandlerDataClone->mpEventData->bReg) {
    DSSWeakRefScope WeakRefScope;

    if(!WeakRefScope.Init(mpParent))
    {
      return;
    }

    res = IDSNetworkScope.Init(mpParent);
    if (AEE_SUCCESS != res)
    {
      dss_error_code = DSS_ERROR_IDSNETWORKSCOPE_INIT_FAILED;
      goto bail;
    }

    LOG_MSG_INFO1_1("DSSNetworkStateHandler::EventOccurred(): "
                    "Event occurred = %d", mEvent);
    
    res = IDSNetworkPrivScope.Init(mpParent);
    if (AEE_SUCCESS != res)
    {
      dss_error_code = DSS_ERROR_IDSNETWORKPRIVSCOPE_INIT_FAILED;
      goto bail;
    }

    res = IDSNetworkPrivScope.Fetch()->GetPreviousState(&previousNetState);
    if (AEE_SUCCESS != res)
    {
      dss_error_code = DSS_ERROR_GET_PREVIOUSSTATE_FAILED;
      goto bail;
    }

    // convert net state to iface state
    eventInfo.iface_state_info = DSSConversion::NetState2IfaceState(previousNetState);

    DispatchCB(mEvent, mpHandlerDataClone->mpEventData, &eventInfo);
    return;

    bail:
    LOG_MSG_ERROR_2("DSSNetworkStateHandler::EventOccurred(): "
                    "result %d, error code %d", res, dss_error_code);
  }
}

AEEResult DSSNetworkStateHandler::RegisterIDL()
{
  DSSIDSNetworkPrivScope IDSNetworkPrivScope;
  DSSWeakRefScope WeakRefScope;
  IQI* pRegObj = NULL;

  if(!WeakRefScope.Init(mpParent)) {
    return AEE_EFAILED;
  }

  IDS_ERR_RET(IDSNetworkPrivScope.Init(mpParent));

  mRegisteredIDLEvent = DSSConversion::Event2DsnetEvent(mEvent);

  LOG_MSG_INFO1_2("DSSNetworkStateHandler::RegisterIDL(): "
                  "Registering to event id %d , NetworkPriv obj 0x%p",
                  mRegisteredIDLEvent, IDSNetworkPrivScope.Fetch());

  AEEResult res = IDSNetworkPrivScope.Fetch()->OnStateChange(mpSignal, mRegisteredIDLEvent, &pRegObj);
  // regObj mechanism is currently not in effect by dsnet layer. 
  // No need for DSS to hold the allocated object to maintain the event registration. 
  // For design simplicity we release the regObj immediately.
  // If and when dsnet layer enforces the registration object concept this code need to be adapted accordingly.
  DSSCommon::ReleaseIf(&pRegObj);

  return res;
}

DSSNetworkStateHandler* DSSNetworkStateHandler::CreateInstance()
{
  return new DSSNetworkStateHandler;
}

