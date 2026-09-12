/*======================================================

FILE:  DSS_DpdPingStatHandler.cpp

GENERAL DESCRIPTION:
   Implementation of DSS_DpdPingStatHandler functions

=====================================================

Copyright (c) 2019 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_DpdPingStatHandler.cpp#3 $
  $DateTime: 2020/01/20 01:22:01 $
  $Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2014-07-20 sb  Created module.

===========================================================================*/
#include "DSS_DpdPingStatHandler.h"
#include "DSS_Common.h"
#include "DSS_IDSNetworkScope.h"
#include "DSS_IDSNetworkExtScope.h"
#include "ds_Net_INetworkExt.h"
#include <stringl/stringl.h>



using namespace ds::Net;

DSSDpdPingStatHandler::DSSDpdPingStatHandler()
{
   mEventType = EVENT_HANDLER_DPD_PING_STAT;
}

void DSSDpdPingStatHandler::EventOccurred()
{

  DSSIDSNetworkExtScope IDSNetworkExtScope;
  DpdPingStatInfoType   dpdpingstatev;

  if ( AEE_SUCCESS != IDSNetworkExtScope.Init(mpParent) ) {
        return;
  }
  AEEResult res = IDSNetworkExtScope.Fetch()->GetDpdPingStat(&dpdpingstatev);
  if(AEE_SUCCESS != res)
  {
     return;
  }
  if (mpHandlerDataClone->mpEventData->bReg) 
  {
     dss_iface_ioctl_event_info_union_type eventInfo;
     memset(&eventInfo, 0, sizeof(dss_iface_ioctl_event_info_union_type));
     
     eventInfo.dpd_ping_info.num_pings = dpdpingstatev.num_pings;
     eventInfo.dpd_ping_info.pkts_loss = dpdpingstatev.pkts_loss;
     eventInfo.dpd_ping_info.avg_rtt = dpdpingstatev.avg_rtt;
     eventInfo.dpd_ping_info.user_data_ptr = dpdpingstatev.user_data_ptr;
     memcpy(&(eventInfo.dpd_ping_info.rtt), &(dpdpingstatev.rtt), sizeof(uint16)*(dpdpingstatev.num_pings));
     DispatchCB(DSS_IFACE_IOCTL_DPD_PING_INFO_EV, mpHandlerDataClone->mpEventData, &eventInfo);
  }

}

AEEResult DSSDpdPingStatHandler::RegisterIDL()
{
   DSSIDSNetworkScope IDSNetworkScope;
   INetworkExt *piNetworkExt = 0;
   AEEResult res = AEE_SUCCESS;
   DSSWeakRefScope WeakRefScope;
   IQI* pRegObj = NULL;

   if(!WeakRefScope.Init(mpParent)) {
      return AEE_EFAILED;
   }

   IDS_ERR_RET(IDSNetworkScope.Init(mpParent));

   IDS_ERR_RET(IDSNetworkScope.Fetch()->QueryInterface(AEEIID_INetworkExt,
                                                       (void**)&piNetworkExt));

   LOG_MSG_INFO1_1("DSSDpdPingStatHandler::RegisterIDL(): "
                   "Registering to QDS_EV_DPD_PING_STAT_EV, NetworkExt obj 0x%p",
                   piNetworkExt);
   res = piNetworkExt->OnStateChange(mpSignal, NetworkExtEvent::QDS_EV_DPD_PING_STAT_EV, &pRegObj);
   // regObj mechanism is currently not in effect by dsnet layer. 
   // No need for DSS to hold the allocated object to maintain the event registration. 
   // For design simplicity we release the regObj immediately.
   // If and when dsnet layer enforces the registration object concept this code need to be adapted accordingly.
   DSSCommon::ReleaseIf(&pRegObj);

   DSSCommon::ReleaseIf((IQI**)&piNetworkExt);

   return res;
}

DSSDpdPingStatHandler* DSSDpdPingStatHandler::CreateInstance()
{
   return new DSSDpdPingStatHandler;
}

