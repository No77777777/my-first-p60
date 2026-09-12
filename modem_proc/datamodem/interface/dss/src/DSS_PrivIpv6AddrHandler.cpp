/*======================================================

FILE:  DSS_PrivIpv6AddrHandler.cpp

GENERAL DESCRIPTION:
   Implementation of DSSPrivIpv6AddrHandler functions

=====================================================

Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PrivIpv6AddrHandler.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_Common.h"
#include "ds_Net_IIPv6Address.h"
#include "ps_mem.h"
#include "DSS_PrivIpv6AddrHandler.h"
#include "DSS_GenScope.h"
#include "DSS_MemoryManagement.h"
#include "dss_errors_def.h"

using namespace ds::Net;
using namespace dss::Error;

DSSPrivIpv6AddrHandler::DSSPrivIpv6AddrHandler()
{
   mEventType = EVENT_HANDLER_IP6PRIV_ADDR;
}

void DSSPrivIpv6AddrHandler::EventOccurred()
{
   AEEResult res;
   dss_error_enum_type dss_error_code = DSS_ERROR_MIN;

   if ((mpHandlerDataClone->mpEventData)->bReg)
   {
      IPv6AddrStateType state;
      IIPv6Address* piNetIpv6Address = 0;
      DSSWeakRefScope WeakRefScope;

      if(!WeakRefScope.Init(mpParent)) {
         return;
      }

      res = mpParent->GetIDSNetIpv6Address(&piNetIpv6Address);
      if (AEE_SUCCESS != res) {
         LOG_MSG_ERROR_1("DSSPrivIpv6AddrHandler::EventOccurred(): "
                         "GetIDSNetIpv6Address() failed: %d", res);
         return;
      }
      DSSGenScope scopeIPv6Address (piNetIpv6Address,DSSGenScope::IDSIQI_TYPE);

      if (NULL != piNetIpv6Address)
      {
         res = piNetIpv6Address->GetState(&state);
         if (AEE_SUCCESS != res) {
            dss_error_code = DSS_ERROR_GET_STATE_FAILED;
            goto bail;
         }
      }
      else
      {
        return;
      }

      // Dispatch the user callback according to the received event.
      dss_iface_ioctl_event_info_union_type eventInfo;
      dss_iface_ioctl_event_enum_type eventStatus;


      if (IPv6AddrState::PRIV_ADDR_WAITING != state) {
         ::ds::INAddr6Type ip6Addr;
         mpParent->GetIsUnique(&eventInfo.priv_ipv6_addr.is_unique);
         if (AEE_SUCCESS != piNetIpv6Address->GetAddress(ip6Addr)) {
            return;
         }
         if (AEE_SUCCESS != DSSConversion::IDS2DSIp6Addr(ip6Addr,&eventInfo.priv_ipv6_addr.ip_addr)){
            return;
         }
      }


      // update the status
      switch(state)
      {
         case IPv6AddrState::PRIV_ADDR_AVAILABLE:
            eventStatus = DSS_IFACE_IOCTL_IPV6_PRIV_ADDR_GENERATED_EV;
            break;
         case IPv6AddrState::PRIV_ADDR_DEPRECATED:
            eventStatus = DSS_IFACE_IOCTL_IPV6_PRIV_ADDR_DEPRECATED_EV;

            break;
         case IPv6AddrState::PRIV_ADDR_DELETED:
            eventStatus = DSS_IFACE_IOCTL_IPV6_PRIV_ADDR_DELETED_EV;
            break;
         case IPv6AddrState::PRIV_ADDR_WAITING:
            // This status do not require event
            return;
         default:
            dss_error_code = DSS_ERROR_IPV6_ADDRESS_EVENT_NOT_SUPPORTED;
            goto bail;
      }

      if (DSS_IFACE_IOCTL_IPV6_PRIV_ADDR_DELETED_EV == eventStatus) {
            DSSNetActive* pNetActive;

            res = mpParent->GetDSSNetActive(&pNetActive);
            if (AEE_SUCCESS != res) {
               dss_error_code = DSS_ERROR_GET_DSSNETACTIVE_FAILED;
               goto bail;
            }

         res = pNetActive->RemoveDSSPrivIpv6Addr(piNetIpv6Address);
         if (AEE_SUCCESS != res) {
            dss_error_code = DSS_ERROR_REMOVE_DSSPRIVIPV6ADDR_FAILED;
            //GetDSSNetActive() gets strong ref, need to release
            PS_MEM_RELEASE(pNetActive);
            goto bail;
         }
         //GetDSSNetActive() gets strong ref, need to release
         PS_MEM_RELEASE(pNetActive);
      }

      DispatchCB(eventStatus, mpHandlerDataClone->mpEventData, &eventInfo);
      return;

      bail:
      LOG_MSG_ERROR_2("DSSPrivIpv6AddrHandler::EventOccurred(): "
                      "failed: restult %d, error code %d", res, dss_error_code);
      return;
   }
}

AEEResult DSSPrivIpv6AddrHandler::RegisterIDL()
{
   IIPv6Address* piNetIpv6Address = NULL;
   DSSWeakRefScope WeakRefScope;
   IQI* pRegObj = NULL;

   if(!WeakRefScope.Init(mpParent)) {
      return AEE_EFAILED;
   }

   IDS_ERR_RET(mpParent->GetIDSNetIpv6Address(&piNetIpv6Address));

   LOG_MSG_INFO1_1("DSSPrivIpv6AddrHandler::RegisterIDL(): "
                   "Registering to QDS_EV_STATE_CHANGED, IPv6Address obj 0x%p",
                   piNetIpv6Address);
   AEEResult res = piNetIpv6Address->OnStateChange(mpSignal, IPv6AddrEvent::QDS_EV_STATE_CHANGED, &pRegObj);
   // regObj mechanism is currently not in effect by dsnet layer. 
   // No need for DSS to hold the allocated object to maintain the event registration. 
   // For design simplicity we release the regObj immediately.
   // If and when dsnet layer enforces the registration object concept this code need to be adapted accordingly.
   DSSCommon::ReleaseIf(&pRegObj);

   DSSCommon::ReleaseIf((IQI**)&piNetIpv6Address);

   return res;
}

DSSPrivIpv6AddrHandler* DSSPrivIpv6AddrHandler::CreateInstance()
{
   return new DSSPrivIpv6AddrHandler;
}

