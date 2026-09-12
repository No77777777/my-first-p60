/*======================================================

FILE:  DSS_EventHandlerQoS.cpp

GENERAL DESCRIPTION:
   Implementation of DSS_EventHandlerQoS functions

=====================================================

Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_EventHandlerQoS.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_EventHandlerQoS.h"
#include "DSS_Globals.h"
#include "DSS_Common.h"
#include "DSS_CritScope.h"
#include "DSS_EventHandlerNetApp.h"
#include "DSS_QoSNetInitiated.h"
#include "DSS_NetActive.h"
#include "DSS_GenScope.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CCritSect.h"


void DSSEventHandlerQoS::DispatchCB(dss_iface_ioctl_event_enum_type event, EventData* ped,
                                 dss_iface_ioctl_event_info_union_type* eventInfo)
{
   dss_iface_id_type ifaceId = 0;

   mpParent->GetHandle(&ifaceId);

   // take only the appID part of the iface ID (the middle 16 bits)
   sint15 appID =  DSSIfaceId::GetAppId(ifaceId);

   LOG_MSG_INFO1_3("DSSEventHandlerQoS::DispatchCB(): "
                   "DSS called App callback, event: %d, ifaceId: %u, appID: %d",
                   event, ifaceId, appID);
   if((NULL != ped) && (NULL != ped->userCB))
   {
      ped->userCB(event, *eventInfo, ped->userData, appID, ifaceId);
      LOG_MSG_INFO1_3("DSSEventHandlerQoS::DispatchCB(): "
                      "DSS returned from App callback, event: %d, ifaceId: %u, appID: %d",
                      event, ifaceId, appID);
   }
}

dss_iface_ioctl_fltr_match_dir_type DSSEventHandlerQoS::GetFltrMatchDir()
{
  dss_qos_handle_type                   qos_handle;
  DSSNetActive                        * pNetActive;
  DSSQoSNetInitiated                  * pDSSQoSNetInitiated = NULL;
  AEEResult                             res;
  sint15                                dss_errno;

  res = mpParent->GetDSSNetActive(&pNetActive);
  if (AEE_SUCCESS != res || NULL == pNetActive) {
    LOG_MSG_ERROR_1("DSSEventHandlerQoS::GetFltrMatchDir(): "
                    "Failed to GetDSSNetActive. Req Object: 0x%p",
                    mpParent);
    PS_MEM_RELEASE(pNetActive);

    return PS_QOS_FILTR_DIR_MAX;
  }

  mpParent->GetHandle(&qos_handle);
  res = pNetActive->GetDSSQoSNetInitiated(qos_handle, &pDSSQoSNetInitiated);
  PS_MEM_RELEASE(pNetActive);

  DSSGenScope scopeDSSQoSNetInitiated(pDSSQoSNetInitiated, DSSGenScope::IDSIQI_TYPE);
  if (AEE_SUCCESS != res || NULL == pDSSQoSNetInitiated) {                                  
    // Could not find corresponding QoSNetInitiated objects
    dss_errno = DSSConversion::IDS2DSErrorCode(res);
    LOG_MSG_ERROR_2("DSSEventHandlerQoS::GetFltrMatchDir(): "
                    "QoSNetInitiated not found, error %d, errno %d",
                    res, dss_errno);

    return PS_QOS_FILTR_DIR_MAX;
  }

  return pDSSQoSNetInitiated->GetFltrMatchDir();
}

AEEResult DSSEventHandlerQoS::Init(DSSQoS* parentNetAppParam)
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

   // piSignalFactory is singleton and does not require a release

   return AEE_SUCCESS;
}

void DSSEventHandlerQoS::Destructor() throw()
{
   DS_UTILS_RELEASE_WEAKREF_IF(mpParent);

   DSSEventHandler::Destructor();
}
