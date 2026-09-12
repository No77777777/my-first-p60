/*======================================================

FILE:  DSS_EventHandlerRanAsstMediaAdaptReq.cpp

GENERAL DESCRIPTION:
   Implementation of DSS_EventHandlerRanAsstMediaAdaptReq functions

=====================================================

Copyright (c) 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_EventHandlerRanAsstMediaAdaptReq.cpp#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-07-05 sz  Created.

===========================================================================*/

#include "comdef.h"

#include "DSS_EventHandlerRanAsstMediaAdaptReq.h"
#include "DSS_Globals.h"
#include "DSS_Common.h"
#include "DSS_CritScope.h"
#include "DSS_EventHandlerNetApp.h"
#include "DSS_RanAsstMediaAdaptReq.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CCritSect.h"


void DSSEventHandlerRanAsstMediaAdaptReq::DispatchCB(
   dss_iface_ioctl_event_enum_type event, 
   EventData* ped,
   dss_iface_ioctl_event_info_union_type* eventInfo)
{
   dss_iface_id_type          ifaceId = 0;
   /*-------------------------------------------------------------------------
     Get Iface ID
   -------------------------------------------------------------------------*/
   mpParent->GetIfaceId(&ifaceId);

   LOG_MSG_INFO1_4("DSSEventHandlerRanAsstMediaAdaptReq::DispatchCB(): "
                   "DSS_IFACE_IOCTL_RAN_ASST_MEDIA_ADAPTATION_EV, "
                   "DSS calling App callback, event: %d, ifaceId: %u, appID: %d invoke cb %d",
                   event, ifaceId,
                   ((NULL != ped) ? ped->appId : 0),
                   ((NULL != ped && NULL != ped->userCB ) ? TRUE : FALSE));

   if((NULL != ped) && (NULL != ped->userCB))
   {
      ped->userCB(event, *eventInfo, ped->userData, ped->appId, ifaceId);
   }
}

AEEResult DSSEventHandlerRanAsstMediaAdaptReq::Init(DSSRanAsstMediaAdaptReq* parentNetAppParam)
{
   mpParent = parentNetAppParam;
   (void) mpParent->AddRefWeak();
   ISignalFactory *piSignalFactory = 0;
   IDS_ERR_RET(DSSGlobals::Instance()->GetSignalFactory(&piSignalFactory));
   IDS_ERR_RET(piSignalFactory->CreateSignal(&signalHandler,
                                             SignalCB,
                                             this,
                                             &mpSignal,
                                             &mpSignalCtl));

   IDS_ERR_RET(DSSGlobals::Instance()->GetCritSect(&mpCritSect));
   return AEE_SUCCESS;
}

void DSSEventHandlerRanAsstMediaAdaptReq::Destructor() throw()
{
   DS_UTILS_RELEASE_WEAKREF_IF(mpParent);
   DSSEventHandler::Destructor();
}

