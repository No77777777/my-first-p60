/*======================================================

FILE:  DSS_EventHandlerPrivIpv6Addr.cpp

GENERAL DESCRIPTION:
   Implementation of DSS_EventHandlerPrivIpv6Addr functions

=====================================================

Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_EventHandlerPrivIPV6Addr.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_PrivIpv6Addr.h"
#include "DSS_Globals.h"
#include "DSS_Common.h"
#include "DSS_CritScope.h"
#include "DSS_EventHandlerPrivIPV6Addr.h"
#include "DSS_EventHandlerNetApp.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CCritSect.h"


void DSSEventHandlerPrivIpv6Addr::DispatchCB(dss_iface_ioctl_event_enum_type event,
                                             EventData* ped,
                                             dss_iface_ioctl_event_info_union_type* eventInfo)
{
   dss_iface_id_type ifaceId = 0;

   mpParent->GetFlowID(&ifaceId);

   // take only the appID part of the iface ID (the middle 16 bits)
   sint15 appID =  (sint15)((ifaceId >> 8) & 0xFFFF);

   LOG_MSG_INFO1_3("DSSEventHandlerPrivIpv6Addr::DispatchCB(): "
                   "DSS called App callback, event: %d, ifaceId: %u, appID: %d",
                   event, ifaceId, appID);
   if((NULL != ped) && (NULL != ped->userCB))
   {
      ped->userCB(event, *eventInfo, ped->userData, appID, ifaceId);
      LOG_MSG_INFO1_3("DSSEventHandlerPrivIpv6Addr::DispatchCB(): "
                      "DSS returned from App callback, event: %d, ifaceId: %u, appID: %d",
                      event, ifaceId, appID);
   }
}

AEEResult DSSEventHandlerPrivIpv6Addr::Init(DSSPrivIpv6Addr* parentNetAppParam)
{
   mpParent = parentNetAppParam;
   (void) parentNetAppParam->AddRefWeak();
   ISignalFactory *piSignalFactory = 0;

   IDS_ERR_RET(DSSGlobals::Instance()->GetSignalFactory(&piSignalFactory));
   IDS_ERR_RET(piSignalFactory->CreateSignal(&signalHandler,
                                             SignalCB,
                                             this,
                                             &mpSignal, &mpSignalCtl));
   IDS_ERR_RET(DSSGlobals::Instance()->GetCritSect(&mpCritSect));

   // piSignalFactory is singleton and does not require a release

   return AEE_SUCCESS;
}

void DSSEventHandlerPrivIpv6Addr::Destructor() throw()
{
   DS_UTILS_RELEASE_WEAKREF_IF(mpParent);

   DSSEventHandler::Destructor();
}
