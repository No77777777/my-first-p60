/*======================================================

FILE:  DSS_PrimaryQoSModifyStatusHandler.cpp

GENERAL DESCRIPTION:
   Implementation of DSSPrimaryQoSModifyStatusHandler functions

=====================================================

Copyright (c) 2008 - 2013 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

=====================================================*/
/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dss/src/DSS_PrimaryQoSModifyStatusHandler.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2010-04-18 en  History added.

===========================================================================*/

#include "DSS_PrimaryQoSModifyStatusHandler.h"
#include "DSS_Common.h"
#include "DSS_GenScope.h"
#include "ps_mem.h"
#include "ds_Errors_Def.h"
#include "dss_errors_def.h"

using namespace ds::Net;
using namespace ds::Error;
using namespace dss::Error;

DSSPrimaryQoSModifyStatusHandler::DSSPrimaryQoSModifyStatusHandler()
{
   mEventType = EVENT_HANDLER_QOS_PRI_MOD_STAT;
}

void DSSPrimaryQoSModifyStatusHandler::EventOccurred()
{
   dss_error_enum_type dss_error_code = DSS_ERROR_MIN;

   if (mpHandlerDataClone->mpEventData->bReg)
   {
      QoSResultType result;
      IQoS* piNetQoSDefault = 0;
      DSSWeakRefScope WeakRefScope;

      if(!WeakRefScope.Init(mpDSSNetActive)) {
         return;
      }

      AEEResult res = mpDSSNetActive->GetQoSDefault(&piNetQoSDefault);
      if ((AEE_SUCCESS != res) || (NULL == piNetQoSDefault)) {
         LOG_MSG_ERROR_1("DSSPrimaryQoSModifyStatusHandler::EventOccurred(): "
                         "GetQoSDefault() failed: %d", res);
         return ;
      }
      DSSGenScope scopeQosDefault(piNetQoSDefault,DSSGenScope::IDSIQI_TYPE);

      res = piNetQoSDefault->GetModifyResult(&result);
      if (AEE_SUCCESS != res) {
         dss_error_code = DSS_ERROR_GET_MODIFYRESULT_FAILED;
         goto bail;
      }

      // Dispatch the user callback according to the received event.
      dss_iface_ioctl_event_enum_type eventStatus;
      dss_iface_ioctl_event_info_union_type eventInfo;
      memset(&eventInfo, 0, sizeof(dss_iface_ioctl_event_info_union_type));

      // update the status
      switch(result)
      {
         case QoSModifyResult::QDS_ACCEPTED:
            eventStatus = DSS_IFACE_IOCTL_QOS_MODIFY_ACCEPTED_EV;
            break;
         case QoSModifyResult::QDS_REJECTED:
            eventStatus = DSS_IFACE_IOCTL_QOS_MODIFY_REJECTED_EV;
            break;
         default:
            dss_error_code = DSS_ERROR_QOSEVENT_NOT_SUPPORTED;
            goto bail;
      }

       if (DSS_IFACE_IOCTL_QOS_MODIFY_ACCEPTED_EV == eventStatus ||
          DSS_IFACE_IOCTL_QOS_MODIFY_REJECTED_EV == eventStatus) {
          mpDSSNetActive->RemovePrimaryQoSModifyStatusHandler();
       }

      DispatchCB(eventStatus, mpHandlerDataClone->mpEventData, &eventInfo);
      return;

      bail:
      LOG_MSG_ERROR_2("DSSPrimaryQoSModifyStatusHandler::EventOccurred(): "
                      "failed: result %d, error code %d", res, dss_error_code);
   }
}

AEEResult DSSPrimaryQoSModifyStatusHandler::RegisterIDL()
{
   IQoS* piNetQoSDefault = 0;
   DSSWeakRefScope WeakRefScope;
   IQI* pRegObj = NULL;

   if(!WeakRefScope.Init(mpDSSNetActive)) {
      return AEE_EFAILED;
   }

   IDS_ERR_RET(mpDSSNetActive->GetQoSDefault(&piNetQoSDefault));
   if (NULL == piNetQoSDefault) {
      return QDS_EINVAL;
   }
   DSSGenScope scopeQosDefault(piNetQoSDefault,DSSGenScope::IDSIQI_TYPE);
   
   LOG_MSG_INFO1_1("DSSPrimaryQoSModifyStatusHandler::RegisterIDL(): "
                   "Registering to QDS_EV_MODIFY_RESULT, QoSDefault obj 0x%p",
                   piNetQoSDefault);
   AEEResult res = piNetQoSDefault->OnStateChange(mpSignal, QoSEvent::QDS_EV_MODIFY_RESULT, &pRegObj);
   // regObj mechanism is currently not in effect by dsnet layer. 
   // No need for DSS to hold the allocated object to maintain the event registration. 
   // For design simplicity we release the regObj immediately.
   // If and when dsnet layer enforces the registration object concept this code need to be adapted accordingly.
   DSSCommon::ReleaseIf(&pRegObj);

   return res;
}

DSSPrimaryQoSModifyStatusHandler* DSSPrimaryQoSModifyStatusHandler::CreateInstance()
{
   return new DSSPrimaryQoSModifyStatusHandler;
}

