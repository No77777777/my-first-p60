/*======================================================

FILE:  DSSPhysLinkRanAsstMediaAdaptHandler.cpp

GENERAL DESCRIPTION:
   Implementation of DSSPhysLinkRanAsstMediaAdaptHandler functions

=====================================================

Copyright (c) 2008 - 2020 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PhysLinkRanAsstMediaAdaptHandler.cpp#2 $
  $DateTime: 2020/09/30 22:53:48 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-13 en  History added.

===========================================================================*/

#include "DSS_PhysLinkRanAsstMediaAdaptHandler.h"
#include "DSS_Common.h"
#include "DSS_GenScope.h"
#include "dss_errors_def.h"
#include "DSS_WeakRefScope.h"
#include "DSS_Globals.h"
#include "DSS_GenScope.h"
#include "DSS_MemoryManagement.h"
#include "dss_errors_def.h"
#include "DSS_RanAsstMediaAdaptReq.h"
#include "ds_Net_Platform.h"
//#include "ds_Net_Conversion.h"
#include "ds_Net_IRanAsstMediaAdaptManager.h"

using namespace ds::Net;
using namespace dss::Error;
using namespace NetPlatform;
//using namespace ds::Net::Conversion;

class DSSRanAsstMediaAdaptReq;

//class DSSNetActive;
DSSPhysLinkRanAsstMediaAdaptHandler::DSSPhysLinkRanAsstMediaAdaptHandler()
{
  mEventType = EVENT_HANDLER_PHYS_LINK_RAN_ASST_MEDIA_ADAPT;
  mRegisteredIDLEvent = PhysLinkEvent::QDS_EV_RAN_ASST_MEDIA_ADAPT_EV;
}

void DSSPhysLinkRanAsstMediaAdaptHandler::EventOccurred()
{

  dss_iface_ioctl_event_info_union_type  eventInfo;
  IRanAsstMediaAdaptReqType*             pRAMAReqType;
  AEEResult                              result = AEE_SUCCESS;
  boolean                                phyLinkMatched = FALSE;
  IPhysLink*                             pPhysLink      = NULL;
  DSSNetMonitored*                       pDSSNetMonitor = NULL;
  DSSNetActive*                          pDSSNetActive  = NULL;
  IRanAsstMediaAdaptManager             *pRanAsstManager  = NULL;

  
  RanAsstMediaAdaptEventinfoType         PSeventInfo;
  
  memset( &eventInfo, 0x0, sizeof(eventInfo));
  memset (&PSeventInfo, 0x0, sizeof(PSeventInfo));
  
  mpParent->GetIDSNetIRanAsstMediaAdaptReq(&pRAMAReqType);
  DSSGenScope scopeNetRamaReq(pRAMAReqType,DSSGenScope::IDSIQI_TYPE);
  DS_NULL_CHECK_RETURN(pRAMAReqType);
  
  /*-------------------------------------------------------------------------
    Get net moniter  object from DSSRAMAReq to register event to phylink
  -------------------------------------------------------------------------*/
  result = mpParent->GetDSSNetActive(&pDSSNetActive);
  DSSGenScope scopeDSSNetActive(pDSSNetActive,DSSGenScope::IDSIQI_TYPE);
  DS_NULL_CHECK_RETURN(pDSSNetActive);
  /*-----------------------------------------------------------------------------------
    * Get  RanAsstMediaAdapt Manger class object
   ------------------------------------------------------------------------------------*/
  pDSSNetActive->GetNetRanAsstManager(&pRanAsstManager);
  DSSGenScope scopeRanAsstMgr(pRanAsstManager,DSSGenScope::IDSIQI_TYPE);
  DS_NULL_CHECK_RETURN(pRanAsstManager);
  /*-------------------------------------------------------------------------
    Get net moniter  object from DSSRAMAReq to register event to phylink
  -------------------------------------------------------------------------*/
  result = mpParent->GetDSSNetMonitored(&pDSSNetMonitor);
  DSSGenScope scopeDSSNetMonitor(pDSSNetMonitor,DSSGenScope::IDSIQI_TYPE);
  DS_NULL_CHECK_RETURN(pDSSNetMonitor);
  
  result = pDSSNetMonitor->GetPhysLink(&pPhysLink);
  DSSGenScope scopeDSSNetPhyLink(pPhysLink,DSSGenScope::IDSIQI_TYPE);
  DS_NULL_CHECK_RETURN(pPhysLink);
  
  /*-------------------------------------------------------------------------
    Get Stored event cache info
  -------------------------------------------------------------------------*/
  pPhysLink->GetRAMAEventInfo(&PSeventInfo);

  LOG_MSG_INFO1_1("DSSPhysLinkRanAsstMediaAdaptHandler::EventOccurred(): "
                  "DSS_IFACE_IOCTL_RAN_ASST_MEDIA_ADAPTATION_EV, PhysLink obj 0x%p",
                  PSeventInfo.handle);

 phyLinkMatched = pRanAsstManager->MatchPhyLink(pRAMAReqType, PSeventInfo.handle);

  /*-------------------------------------------------------------------------
    Dispatch event to Application
  -------------------------------------------------------------------------*/
  if (TRUE == phyLinkMatched )
  {
    pRAMAReqType->GetRanAsstMediaAdaptHandle(&eventInfo.ran_asst_media_adapt_info.ran_asst_handle);
    eventInfo.ran_asst_media_adapt_info.bit_rate_info.bit_rate 
      = PSeventInfo.bitRate;
    eventInfo.ran_asst_media_adapt_info.bit_rate_info.direction 
      = static_cast <ps_phys_link_data_flow_direction_enum_type>(PSeventInfo.direction);
    eventInfo.ran_asst_media_adapt_info.is_rohc_enabled        
      = PSeventInfo.isRohcEnabled;
      
    DispatchCB(DSS_IFACE_IOCTL_RAN_ASST_MEDIA_ADAPTATION_EV, mpHandlerDataClone->mpEventData, &eventInfo);
  }

  return;
}

AEEResult DSSPhysLinkRanAsstMediaAdaptHandler::RegisterIDL()
{
  IPhysLink*         pPhysLink      = NULL;
  DSSWeakRefScope    WeakRefScope;
  IQI*               pRegObj        = NULL;
  DSSNetMonitored*   pDSSNetMonitor = NULL;
  AEEResult          res;
  /*-------------------------------------------------------------------------
    Get net moniter  object from DSSRAMAReq to register event to phylink
  -------------------------------------------------------------------------*/
  res = mpParent->GetDSSNetMonitored(&pDSSNetMonitor);
  DSSGenScope scopeDSSNetMonitor(pDSSNetMonitor,DSSGenScope::IDSIQI_TYPE);
  //DS_NULL_CHECK_RETURN(pDSSNetMonitor); Todo for shannon
  /*-------------------------------------------------------------------------
    Get phylink  object from Net Mointer to register event 
  -------------------------------------------------------------------------*/
  IDS_ERR_RET(pDSSNetMonitor->GetPhysLink(&pPhysLink));
  DSSGenScope scopeDSSNetPhyLink(pPhysLink,DSSGenScope::IDSIQI_TYPE);
  
  LOG_MSG_INFO1_1("DSSPhysLinkRanAsstMediaAdaptHandler::RegisterIDL(): "
                  "Registering to DSS_IFACE_IOCTL_RAN_ASST_MEDIA_ADAPTATION_EV, PhysLink obj 0x%p",
                  pPhysLink);
  
  res = pPhysLink->OnStateChange(mpSignal, mRegisteredIDLEvent, &pRegObj);
  DSSCommon::ReleaseIf(&pRegObj);  
  return res;
}

DSSPhysLinkRanAsstMediaAdaptHandler* DSSPhysLinkRanAsstMediaAdaptHandler::CreateInstance()
{
  return new DSSPhysLinkRanAsstMediaAdaptHandler;
}

