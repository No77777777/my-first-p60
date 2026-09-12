/*======================================================

FILE:  DSS_SlottedSessionChangedHandler.cpp

GENERAL DESCRIPTION:
   Implementation of DSSSlottedSessionChangedHandler functions

=====================================================

Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_SlottedSessionChangedHandler.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_SlottedSessionChangedHandler.h"
#include "DSS_Common.h"
#include "DSS_IDSNetworkScope.h"
#include "DSS_IDSNetwork1xPrivScope.h"

using namespace ds::Net;

DSSSlottedSessionChangedHandler::DSSSlottedSessionChangedHandler()
{
   mEventType = EVENT_HANDLER_SLOTT_SESS_CHNG;
}

void DSSSlottedSessionChangedHandler::EventOccurred()
{
   if (mpHandlerData->mpEventData && mpHandlerData->mpEventData->bReg)
   {
      DSSIDSNetworkScope IDSNetworkScope;
      DSSIDSNetwork1xPrivScope IDSNetwork1xPrivScope;
      dss_iface_ioctl_event_enum_type event;
      dss_iface_ioctl_event_info_union_type eventInfo;
      DSSWeakRefScope WeakRefScope;

      if(!WeakRefScope.Init(mpParent)) {
         return;
      }
   
      if (AEE_SUCCESS != IDSNetworkScope.Init(mpParent)) {
         return;
      }
      if (AEE_SUCCESS != IDSNetwork1xPrivScope.Init(IDSNetworkScope.Fetch())) {
         return;
      }

      uint32 sci;
      AEEResult res = IDSNetwork1xPrivScope.Fetch()->GetHDRSlottedModeCycleIndex(&sci);
      if (AEE_SUCCESS != res) {
         LOG_MSG_ERROR_1("SlottedSessionChangedHandler::EventOccurred: "
                         "IDSNetwork1zPrivScope Fetch failed: %d", res);
         return;
      }

      // memset the eventinfo.
      memset(&eventInfo,0,sizeof(dss_iface_ioctl_event_info_union_type));

      event = DSS_IFACE_IOCTL_707_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_SESSION_CHANGED_EV;
      eventInfo.slotted_mode_info.sm_current_sci = (uint8) sci;

      DispatchCB(event, mpHandlerDataClone->mpEventData, &eventInfo);
   }
}

AEEResult DSSSlottedSessionChangedHandler::RegisterIDL()
{
   DSSIDSNetworkScope IDSNetworkScope;
   DSSIDSNetwork1xPrivScope IDSNetwork1xPrivScope;
   DSSWeakRefScope WeakRefScope;
   IQI* pRegObj = NULL;

   if(!WeakRefScope.Init(mpParent)) {
      return AEE_EFAILED;
   }

   IDS_ERR_RET(IDSNetworkScope.Init(mpParent));
   IDS_ERR_RET(IDSNetwork1xPrivScope.Init(IDSNetworkScope.Fetch()));
   
   LOG_MSG_INFO1_1("DSSSlottedSessionChangedHandler::RegisterIDL(): "
                   "Registering to SLOTTED_MODE_CHANGED, Network1xPriv obj 0x%p",
                   IDSNetwork1xPrivScope.Fetch());
   AEEResult res = IDSNetwork1xPrivScope.Fetch()->OnStateChange(mpSignal, Network1xPrivEvent::SLOTTED_MODE_CHANGED, &pRegObj);
   // regObj mechanism is currently not in effect by dsnet layer. 
   // No need for DSS to hold the allocated object to maintain the event registration. 
   // For design simplicity we release the regObj immediately.
   // If and when dsnet layer enforces the registration object concept this code need to be adapted accordingly.
   DSSCommon::ReleaseIf(&pRegObj);

   return res;
}

DSSSlottedSessionChangedHandler* DSSSlottedSessionChangedHandler::CreateInstance()
{
   return new DSSSlottedSessionChangedHandler;
}

