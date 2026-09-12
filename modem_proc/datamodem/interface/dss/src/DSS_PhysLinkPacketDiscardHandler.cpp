/*======================================================

FILE:  DSSPhysLinkPacketDiscardHandler.cpp

GENERAL DESCRIPTION:
   Implementation of DSSPhysLinkPacketDiscardHandler functions

=====================================================

Copyright (c) 2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PhysLinkPacketDiscardHandler.cpp#1 $
  $DateTime: 2021/05/25 01:37:25 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2020-12-11 skc  History added.

===========================================================================*/

#include "DSS_PhysLinkPacketDiscardHandler.h"
#include "DSS_Common.h"
#include "DSS_GenScope.h"
#include "dss_errors_def.h"

using namespace ds::Net;

DSSPhysLinkPacketDiscardHandler::DSSPhysLinkPacketDiscardHandler()
{
  mEventType = EVENT_HANDLER_PHYS_LINK_PACKET_DISCARD;
  mRegisteredIDLEvent = PhysLinkEvent::QDS_EV_PACKET_DISCARD_EV;
}

void DSSPhysLinkPacketDiscardHandler::EventOccurred()
{
  
  LOG_MSG_INFO1_1("debug_s:DSSPhysLinkPacketDiscardHandler::EventOccurred(): "
					 "even DSS_IFACE_IOCTL_PACKET_DISCARD_EV occurred  bReg %d", mpHandlerDataClone->mpEventData->bReg);
  if (mpHandlerDataClone->mpEventData->bReg)
  {
    dss_iface_ioctl_event_info_union_type eventInfo;
    DSSWeakRefScope WeakRefScope;
    IPhysLink* pPhysLink = NULL;
    PacketDiscardInfoType packetDiscardInfo;
    boolean isDefaultPhysLink = FALSE;

    if (!WeakRefScope.Init(mpParent))
    {
      return;
    }

    if (AEE_SUCCESS != mpParent->GetPhysLink(&pPhysLink))
    {
      return;
    }

    memset(&packetDiscardInfo, 0, sizeof(PacketDiscardInfoType));

    pPhysLink->GetPacketDiscardInfo(&packetDiscardInfo);

    if (AEE_SUCCESS == mpParent->IsDefaultPhysLink(packetDiscardInfo.physLinkHandle, &isDefaultPhysLink) &&
        TRUE == isDefaultPhysLink)
    {	
      memset(&eventInfo, 0, sizeof(dss_iface_ioctl_event_info_union_type));
      DispatchCB(DSS_IFACE_IOCTL_PACKET_DISCARD_EV, mpHandlerDataClone->mpEventData, &eventInfo);
    }
  }
  return;
}

AEEResult DSSPhysLinkPacketDiscardHandler::RegisterIDL()
{
  IPhysLink* pPhysLink = NULL;
  DSSWeakRefScope WeakRefScope;
  IQI* pRegObj = NULL;

  if(!WeakRefScope.Init(mpParent))
  {
    return AEE_EFAILED;
  }

  AEEResult res = mpParent->GetPhysLink(&pPhysLink);
  if (AEE_SUCCESS != res)
  {
    LOG_MSG_ERROR_1("DSSPhysLinkStateHandler::RegisterIDL(): "
                    "GetPhysLink() failed: %d", res);
    return res;
  }

  LOG_MSG_INFO1_1("DSSPhysLinkStateHandler::RegisterIDL(): "
                  "Registering to QDS_EV_PACKET_DISCARD_EV, PhysLink obj 0x%p",
                  pPhysLink);
  res = pPhysLink->OnStateChange(mpSignal, mRegisteredIDLEvent, &pRegObj);
  // regObj mechanism is currently not in effect by dsnet layer. 
  // No need for DSS to hold the allocated object to maintain the event registration. 
  // For design simplicity we release the regObj immediately.
  // If and when dsnet layer enforces the registration object concept this code need to be adapted accordingly.
  DSSCommon::ReleaseIf(&pRegObj);
   
  DSSCommon::ReleaseIf((IQI**)&pPhysLink);

  return res;
}

DSSPhysLinkPacketDiscardHandler* DSSPhysLinkPacketDiscardHandler::CreateInstance()
{
  return new DSSPhysLinkPacketDiscardHandler;
}

