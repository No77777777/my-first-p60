/*===========================================================================
  FILE: ds_Net_RanAsstMediaAdaptManager.cpp

  OVERVIEW: This file provides implementation of the RanAsstMediaAdaptManager class.

  DEPENDENCIES: None

  Copyright (c) 2010-2012 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_RanAsstMediaAdaptManager.cpp#1 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2012-05-09 rk  Memory cleanup for fi_ptr_arr in AddFirewallRule.
  2008-05-20 dm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "ps_system_heap.h"
#include "ds_Errors_Def.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Net_Utils.h"
#include "ds_Net_RanAsstMediaAdaptManager.h"
#include "ds_Net_RanAsstMediaAdaptReq.h"
#include "ds_Net_Platform.h"
#include "ds_Net_Conversion.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_IPFilterSpec.h"
#include "ds_Net_QoS_Def.h"
#include "ds_Utils_ICritSect.h"
#include "ds_Utils_CCritSect.h"


using namespace ds::Error;
using namespace ds::Net;
using namespace ds::Net::Conversion;
using namespace NetPlatform;


/*---------------------------------------------------------------------------
  CONSTRUCTOR/DESTRUCTOR
---------------------------------------------------------------------------*/
RanAsstMediaAdaptManager::RanAsstMediaAdaptManager
(
  int32 ifaceHandle
)
: mIfaceHandle (ifaceHandle), refCnt(1)
{
  int res = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  res = DS_Utils_CreateInstance(NULL, AEECLSID_CCritSect,
                                (void**)&mpCritSect);
  if (AEE_SUCCESS != res) {
     LOG_MSG_ERROR_1("RanAsstMediaAdaptManager::RanAsstMediaAdaptManager(): "
                     "Can't create Critical Section (%d)", res);
     ASSERT(0);
  }

} /* RanAsstMediaAdaptManager::RanAsstMediaAdaptManager() */

RanAsstMediaAdaptManager::~RanAsstMediaAdaptManager
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_UTILS_RELEASEIF(mpCritSect);

} /* RanAsstMediaAdaptManager::~RanAsstMediaAdaptManager() */

ds::ErrorType RanAsstMediaAdaptManager::CreateMediaAdaptRanAsstReq
(
  IIPFilterPriv*              pFltrSpec,
  IRanAsstMediaAdaptReqType** ppRAMAReqType
)
{
  ds::ErrorType                 result   = AEE_SUCCESS;
  RanAsstMediaAdaptReqType*     pRAMAReq = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  LOG_MSG_FUNCTION_ENTRY_3("RanAsstMediaAdaptManager::CreateMediaAdaptRanAsstReq(): "
                            "Obj 0x%p, handle 0x%x pIFilterSpec 0x%x ",
                            this, mIfaceHandle, pFltrSpec);
  /*-------------------------------------------------------------------------
    Validation.
  -------------------------------------------------------------------------*/
  if (NULL == pFltrSpec || NULL == ppRAMAReqType)
  {
    return QDS_EFAULT;
  }
  /*-----------------------------------------------------------------------
    Allocate memory for the Ran Asst Media Adapt request object. 
  -----------------------------------------------------------------------*/
  pRAMAReq = new RanAsstMediaAdaptReqType();
  if (NULL == pRAMAReq)
  {
    return AEE_ENOMEMORY;
  }
  /*-----------------------------------------------------------------------
    Update Filter Spec. 
  -----------------------------------------------------------------------*/
  pRAMAReq->SetRanAsstMediaAdaptReqFltrSpec(pFltrSpec);
  /*-----------------------------------------------------------------------
    Return created Ran asst media adapt request  
  -----------------------------------------------------------------------*/
  *ppRAMAReqType = static_cast <IRanAsstMediaAdaptReqType *> (pRAMAReq);

  return result;
}/*CreateMediaAdaptRanAsstReq */


void RanAsstMediaAdaptManager::GetIfaceId
(
  int32* pIfaceId
)
{
  *pIfaceId = mIfaceHandle;
  return;
}

 void  RanAsstMediaAdaptManager::GetCritsect
 (
   ICritSect**  pCritSect
 )
{
  *pCritSect = mpCritSect;
}


ds::ErrorType RanAsstMediaAdaptManager::GetRealTimeBitRate
(
  IRanAsstMediaAdaptReqType* pRAMAReqType,
  PSRAMAInfoType*            bitRateInfo
)
{
  ds::ErrorType                      result = AEE_SUCCESS;
  int32                              ifaceHandle;
  int32                              physLinkHandle;
  RAMAPhyLinkReqIOCTLType            ramaReqType;
  IIPFilterPriv*                     filterSpec;
  ps_iface_ioctl_matching_flow_type* matching_flow_ptr = NULL;
  boolean                            bitRateInfoPresent = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Validate arguments.
  -------------------------------------------------------------------------*/
  if (NULL == pRAMAReqType )
  {
    return QDS_EFAULT;
  }
  memset (&ramaReqType, 0, sizeof(ramaReqType));
  /*-------------------------------------------------------------------------
    Get iface Id and filter spec 
  -------------------------------------------------------------------------*/
  GetIfaceId (&ifaceHandle);
  pRAMAReqType->GetRanAsstMediaAdaptReqFltrSpec(&filterSpec);

  /*-------------------------------------------------------------------------
    Convert DS NET filter spec into PS Filter spec ( DS2PSRAMARequestSpec will alloc memory for
    PS filter spec hence free memory at end of the function)
  -------------------------------------------------------------------------*/
  result = DS2PSRAMARequestSpec(filterSpec, 
                                &ramaReqType.fltr_spec.tx_fltr_template);
  if (AEE_SUCCESS != result)
  {
    return QDS_EFAULT;
  }
  /*-------------------------------------------------------------------------
    Perform Network Initiated monitoring QoS Request IOCTL on the iface handle.
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (ifaceHandle,
                       IFACE_IOCTL_GET_FLOW_LIST_FROM_FLTR,
                       static_cast <void *> (&ramaReqType));
  /*-------------------------------------------------------------------------
    Check if there is any phylink returned in IOCTL result
  -------------------------------------------------------------------------*/
  if ( AEE_SUCCESS == result )
  {
    matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
      list_pop_front(&ramaReqType.matching_flow_list);

   //Free memory for other flows (No op)
    while (NULL != matching_flow_ptr) 
    { 
      if ( FALSE == bitRateInfoPresent)
      {
         physLinkHandle = (int32)PS_FLOW_GET_PHYS_LINK( matching_flow_ptr->flow_ptr);
         //Got the matching Flow now find phylink and call ioctl
         result = PhysLinkIoctl (  
                    physLinkHandle,
                    PHYS_LINK_IOCTL_GET_RAN_ASST_MEDIA_ADAPT,
                    static_cast <void *> (bitRateInfo));
          if ( AEE_SUCCESS == result )
          {
            bitRateInfoPresent= TRUE;
          }
       }
       PS_SYSTEM_HEAP_MEM_FREE(matching_flow_ptr);
       matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
         list_pop_front(&ramaReqType.matching_flow_list);
    }
  }
  PS_SYSTEM_HEAP_MEM_FREE(ramaReqType.fltr_spec.tx_fltr_template.list_ptr);

  return result;
}/*GetRealTimeBitRate */


ds::ErrorType RanAsstMediaAdaptManager::UpdateBitRateInfo
(
  IRanAsstMediaAdaptReqType*        pRAMAReqType,
  PSRAMABitRateInfoType*            bitRateInfo
)
{
  ds::ErrorType                      result = AEE_SUCCESS;
  int32                              ifaceHandle;
  IIPFilterPriv*                     filterSpec;
  RAMAPhyLinkReqIOCTLType            ramaReqType;
  ps_iface_ioctl_matching_flow_type* matching_flow_ptr = NULL;
  boolean                            bitRateInfoPresent = FALSE;
  int32                              physLinkHandle;
  PSSysSubEnumType                   subId = SYS_PRIMARY_SUBS;
  Sys3GPPRanAsstSupportConfType      ramaSupportInfo;
  int16                              psErrno;
  int32                              sysIoctlResult;
  /*-----------------------------------------------------------------------------------
    * NULL Check
   ------------------------------------------------------------------------------------*/
  if(pRAMAReqType == NULL || NULL == bitRateInfo){
    return DSS_ERROR;
  }
  memset (&ramaReqType, 0, sizeof(ramaReqType));
  /*-------------------------------------------------------------------------
    Get iface Id 
  -------------------------------------------------------------------------*/
  GetIfaceId (&ifaceHandle);

  subId = IfaceGetSubsID(ifaceHandle);
  /*-------------------------------------------------------------------------
    Get sys ioctl info and verify ran asst support
  -------------------------------------------------------------------------*/
  memset(&ramaSupportInfo, 0x0, sizeof(ramaSupportInfo));

  sysIoctlResult = PSSysIoctl(SYS_TECH_3GPP, 
                   SYS_IOCTL_3GPP_GET_RAN_ASST_SUPPORT_CONFIG,
                   subId,
                   static_cast <void *> (&ramaSupportInfo),
                   &psErrno);
  
  if( 0     != sysIoctlResult  || 
     FALSE  == ramaSupportInfo.bit_rate_update_support )
  {
    return QDS_EAFNOSUPPORT;
  }
  
  /*-------------------------------------------------------------------------
    Get iface Id and filter spec 
  -------------------------------------------------------------------------*/
  pRAMAReqType->GetRanAsstMediaAdaptReqFltrSpec(&filterSpec);
  /*-------------------------------------------------------------------------
    Convert DS NET filter spec into PS Filter spec ( DS2PSRAMARequestSpec will alloc memory for
    PS filter spec hence free memory at end of the function)
  -------------------------------------------------------------------------*/
  result = DS2PSRAMARequestSpec(filterSpec, 
                                &ramaReqType.fltr_spec.tx_fltr_template);
  if (AEE_SUCCESS != result)
  {
    return QDS_EFAULT;
  }
  /*-------------------------------------------------------------------------
    Perform Network Initiated monitoring QoS Request IOCTL on the iface handle.
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (ifaceHandle,
                       IFACE_IOCTL_GET_FLOW_LIST_FROM_FLTR,
                       static_cast <void *> (&ramaReqType));
  /*-------------------------------------------------------------------------
    Check if there is any phylink returned in IOCTL result
  -------------------------------------------------------------------------*/
  if ( AEE_SUCCESS == result )
  {
    matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
      list_pop_front(&ramaReqType.matching_flow_list);

   //Free memory for other flows (No op)
    while (NULL != matching_flow_ptr) 
    { 
      if ( FALSE == bitRateInfoPresent)
      {
         physLinkHandle = (int32)PS_FLOW_GET_PHYS_LINK( matching_flow_ptr->flow_ptr);
         //Got the matching Flow now find phylink and call ioctl
         result = PhysLinkIoctl (  
                    physLinkHandle,
                    PHYS_LINK_IOCTL_RAN_ASST_MEDIA_ADAPT_UPDATE,
                    static_cast <void *> (bitRateInfo));
          if ( AEE_SUCCESS == result )
          {
            bitRateInfoPresent= TRUE;
          }
       }
       PS_SYSTEM_HEAP_MEM_FREE(matching_flow_ptr);
       matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
         list_pop_front(&ramaReqType.matching_flow_list);
    }
  }
  PS_SYSTEM_HEAP_MEM_FREE(ramaReqType.fltr_spec.tx_fltr_template.list_ptr);

  return result;
}/* RanAsstMediaAdaptManager::UpdateBitRateInfo */

boolean RanAsstMediaAdaptManager::MatchPhyLink
(
  IRanAsstMediaAdaptReqType* pRAMAReqType,
  int32                      PSphysLinkHandle
)
{
  ds::ErrorType                      result = AEE_SUCCESS;
  int32                              ifaceHandle;
  IIPFilterPriv*                     filterSpec;
  RAMAPhyLinkReqIOCTLType            ramaReqType;
  ps_iface_ioctl_matching_flow_type* matching_flow_ptr = NULL;
  boolean                            phyLinkMatched = FALSE;
  int32                              physLinkHandle = 0;
  /*-----------------------------------------------------------------------------------
    * NULL Check
   ------------------------------------------------------------------------------------*/
  if(pRAMAReqType == NULL || 0 == PSphysLinkHandle){
    return phyLinkMatched;
  }
  memset (&ramaReqType, 0, sizeof(ramaReqType));
  /*-------------------------------------------------------------------------
    Get iface Id 
  -------------------------------------------------------------------------*/
  GetIfaceId (&ifaceHandle);
  
  /*-------------------------------------------------------------------------
    Get iface Id and filter spec 
  -------------------------------------------------------------------------*/
  pRAMAReqType->GetRanAsstMediaAdaptReqFltrSpec(&filterSpec);
  /*-------------------------------------------------------------------------
    Convert DS NET filter spec into PS Filter spec ( DS2PSRAMARequestSpec will alloc memory for
    PS filter spec hence free memory at end of the function)
  -------------------------------------------------------------------------*/
  result = DS2PSRAMARequestSpec(filterSpec, 
                                &ramaReqType.fltr_spec.tx_fltr_template);
  if (AEE_SUCCESS != result)
  {
    return phyLinkMatched;
  }
  /*-------------------------------------------------------------------------
    Perform Network Initiated monitoring QoS Request IOCTL on the iface handle.
  -------------------------------------------------------------------------*/
  ramaReqType.fltr_match_dir = (ps_iface_qos_fltr_match_dir_enum_type) QoSFltrMatchDir::QDS_DIR_UL;
  result = IfaceIoctl (ifaceHandle,
                       IFACE_IOCTL_GET_FLOW_LIST_FROM_FLTR,
                       static_cast <void *> (&ramaReqType));
    /*-------------------------------------------------------------------------
      Check if application requested filter spec matched with phy link on which event were sent
    -------------------------------------------------------------------------*/
  if ( AEE_SUCCESS == result )
  {
    matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
      list_pop_front(&ramaReqType.matching_flow_list);
  
    while (NULL != matching_flow_ptr) 
    {
       physLinkHandle = (int32)PS_FLOW_GET_PHYS_LINK( matching_flow_ptr->flow_ptr);
  
       if ( (FALSE == phyLinkMatched) && ((0 != physLinkHandle) && 
            (PSphysLinkHandle == physLinkHandle)))
       {
          phyLinkMatched = TRUE;
       }
     //Continue loop to free all flow pointers memory
      PS_SYSTEM_HEAP_MEM_FREE(matching_flow_ptr);
      matching_flow_ptr = (ps_iface_ioctl_matching_flow_type*)
        list_pop_front(&ramaReqType.matching_flow_list);
    }
  }

  PS_SYSTEM_HEAP_MEM_FREE(ramaReqType.fltr_spec.tx_fltr_template.list_ptr);
  return phyLinkMatched;
}/*RanAsstMediaAdaptManager::MatchPhyLink*/


