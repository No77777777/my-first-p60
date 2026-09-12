/*======================================================

FILE:  DSS_QoSAwareUnAwareHandler.cpp

GENERAL DESCRIPTION:
   Implementation of DSS_QoSAwareUnAwareHandler functions

=====================================================

Copyright (c) 2008 - 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_QoSAwareUnAwareHandler.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_QoSAwareUnAwareHandler.h"
#include "DSS_Common.h"
#include "DSS_IDSNetworkPrivScope.h"
#include "DSS_IDSNetworkExtScope.h"
#include "DSS_MemoryManagement.h"
#include "dss_errors_def.h"

using namespace ds::Net;
using namespace dss::Error;

DSSQoSAwareUnAwareHandler::DSSQoSAwareUnAwareHandler()
{
   mEventType = EVENT_HANDLER_QOS_AW_UNAW;
}

void DSSQoSAwareUnAwareHandler::EventOccurred()
{
   DSSIDSNetworkExtScope IDSNetworkExtScope;
   DSSIDSNetworkPrivScope IDSNetworkPrivScope;
   AEEResult nRet;
   boolean qosAware = FALSE;
   dss_iface_ioctl_event_enum_type event = DSS_IFACE_IOCTL_MIN_EV;
   dss_iface_ioctl_event_info_union_type eventInfo;
   QoSInfoCodeType infoCode;
   DSSWeakRefScope WeakRefScope;
   dss_error_enum_type dss_error_code = DSS_ERROR_MIN;

   if (mpHandlerDataClone->mpEventData->bReg)
   {
      if(!WeakRefScope.Init(mpParent)) {
         return;
      }

      // initialize all event info fields to 0
      memset(&eventInfo,0,sizeof(dss_iface_ioctl_event_info_union_type));

      if ( AEE_SUCCESS != IDSNetworkExtScope.Init(mpParent) ) {
         return;
      }

      nRet = IDSNetworkExtScope.Fetch()->GetQosAware(&qosAware);
      if (AEE_SUCCESS != nRet) {
         dss_error_code = DSS_ERROR_GET_QOSAWARE_FAILED;
         goto bail;
      }

      // transform qos aware result to relevant event
      event = DSSConversion::QosAware2Event(qosAware);

      // This if statement checks that the event raised
      // is relevant to this event handler object
      if(mEvent != event)
      {
        return;
      }

      LOG_MSG_INFO1_1("DSSQoSAwareUnAwareHandler::EventOccurred(): "
                   "Event occurred = %d", mEvent);

      nRet = IDSNetworkPrivScope.Init(mpParent);
      if (AEE_SUCCESS != nRet) {
         dss_error_code = DSS_ERROR_IDSNETWORKPRIVSCOPE_INIT_FAILED;
         goto bail;
      }

      nRet = IDSNetworkPrivScope.Fetch()->GetQoSAwareInfoCode(&infoCode);
      if (AEE_SUCCESS != nRet) {
         dss_error_code = DSS_ERROR_GET_QOSAWAREINFOCODE_FAILED;
         goto bail;
      }

      nRet = DSSConversion::IDS2DSQoSAwareInfoCode(infoCode, &(eventInfo.qos_aware_info_code));
      if (AEE_SUCCESS != nRet) {
         dss_error_code = DSS_ERROR_IDS2DSQOSAWAREINFOCODE_FAILED;
         goto bail;
      }

      DispatchCB(event, mpHandlerDataClone->mpEventData, &eventInfo);
      return;

      bail:
      LOG_MSG_ERROR_2("DSSQoSAwareUnAwareHandler::EventOccurred(): "
                      "failed: result %d, error code %d", nRet, dss_error_code);
      return;
   }
}

AEEResult DSSQoSAwareUnAwareHandler::RegisterIDL()
{
   DSSIDSNetworkPrivScope IDSNetworkPrivScope;
   INetworkExt* pINetworkExt = NULL;
   AEEResult res;
   DSSWeakRefScope WeakRefScope;
   IQI* pRegObj = NULL;

   if(!WeakRefScope.Init(mpParent)) {
      return AEE_EFAILED;
   }

   IDS_ERR_RET(IDSNetworkPrivScope.Init(mpParent));
   IDS_ERR_RET(IDSNetworkPrivScope.Fetch()->QueryInterface(AEEIID_INetworkExt, (void**)&pINetworkExt));

   LOG_MSG_INFO1_1("DSSQoSAwareUnAwareHandler::RegisterIDL(): "
                   "Registering to QDS_EV_QOS_AWARENESS, NetworkExt obj 0x%p",
                   pINetworkExt);
   res = pINetworkExt->OnStateChange(mpSignal, NetworkExtEvent::QDS_EV_QOS_AWARENESS, &pRegObj);
   // regObj mechanism is currently not in effect by dsnet layer.
   // No need for DSS to hold the allocated object to maintain the event registration. 
   // For design simplicity we release the regObj immediately.
   // If and when dsnet layer enforces the registration object concept this code need to be adapted accordingly.
   DSSCommon::ReleaseIf(&pRegObj);

   DSSCommon::ReleaseIf((IQI**)&pINetworkExt);

   return res;
}

DSSQoSAwareUnAwareHandler* DSSQoSAwareUnAwareHandler::CreateInstance()
{
   return new DSSQoSAwareUnAwareHandler;
}

