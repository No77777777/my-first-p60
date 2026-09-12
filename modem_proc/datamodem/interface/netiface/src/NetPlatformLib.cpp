/*===========================================================================
  FILE: NetPlatformLib.cpp

  OVERVIEW: This file provides implementation for the network platform
  library.

  DEPENDENCIES: None

  Copyright (c) 2008-2020 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/


/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are signaled in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/netiface/src/NetPlatformLib.cpp#4 $
  $DateTime: 2021/05/25 01:37:25 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2014-12-04 ag  New Iface, Flow, Phys link event registration for memory optimization
  2013-11-01 rk  Removed feature mobileap.  
  2013-02-04 svj New DIAG event to generate data cal status information  
  2012-07-27 ash Added support for IPv6 Hybrid Architecture.
  2010-12-23 ss  Cleaning up of Globals for thread safety in Q6 Free
                 Floating environment.
  2008-07-02 hm  Created module.

===========================================================================*/

/*---------------------------------------------------------------------------
  Include Files
---------------------------------------------------------------------------*/
#include "datamodem_variation.h"
#include "comdef.h"
#include <stringl/stringl.h>

#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_Conversion.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_Platform.h"

#include "ps_iface_ioctl.h"
#include "ps_flow_ioctl.h"
#include "ps_phys_link_ioctl.h"
#include "ps_iface.h"
#include "ps_flow.h"
#include "ps_iface_flow.h"
#include "ps_flowi_event.h"
#include "ps_flowi_utils.h"
#include "ps_route.h"
#include "ps_routei.h"
#include "ps_in.h"
#include "ps_iface_ipfltr.h"
#include "dserrno.h"
#include "ps_acl.h"
#include "ps_route_scope.h"
#include "ps_ip6_addr.h"
#include "ds_Utils_Conversion.h"
#include "ds_Net_INetwork.h"
#include "ds_Net_IPolicy.h"
#include "ds_Net_IPolicyPriv.h"
#include "ps_metai_info.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_wmk_logging.h"
#include "ps_ifacei_event.h"
#include "ps_phys_linki_event.h"
#include "ps_sys_ioctl.h"
#include "ps_iface_defs.h"
#include "ps_system_heap.h"


#if defined (FEATURE_DATA_PS_IPV6)
#include "ps_iface_addr_v6.h"
#include "ps_ifacei_addr_v6.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ps_utils.h"
#include "ps_crit_sect.h"
#include "ps_stat_iface.h"
#include "ps_policy_mgr.h"
#include "ps_policyi_mgr.h"
#include "ps_logging_helper.h"


/*---------------------------------------------------------------------- 
  Macros for ERROR_MSGs
----------------------------------------------------------------------*/
#define LOG_MSG_ERROR_PS_INT_0(str) \
	  ds_logging_error_log_0(__FILENAME__ ,__LINE__,str)
		
#define LOG_MSG_ERROR_PS_INT_1(str, arg1) \
	  ds_logging_error_log_1(__FILENAME__ ,__LINE__,str, (uint32)arg1)
	
#define LOG_MSG_ERROR_PS_INT_2(str, arg1, arg2) \
	  ds_logging_error_log_2(__FILENAME__ ,__LINE__,str, (uint32)arg1, \
	  (uint32)arg2)
	  

/*===========================================================================

FUNCTION DS_LOGGING_ERROR_LOG_0

DESCRIPTION
  Wrapper function for reporting error messages
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ds_logging_error_log_0
(
  const char*   file_name,
  unsigned int  line_num,  
  const char*   format
)
{
  if ((NULL == file_name) || (NULL == format))
  {
    return;
  }
  MSG_SPRINTF_3(MSG_SSID_DS, LOG_MSG_ERROR_LEVEL,      
                "%s line: %d: %s",
                file_name, line_num, format);
}/* ds_logging_error_log_0 */

/*===========================================================================

FUNCTION DS_LOGGING_ERROR_LOG_1

DESCRIPTION
  Wrapper function for reporting error messages
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ds_logging_error_log_1
(
  const char*   file_name,
  unsigned int  line_num,
  const char*   format,
  uint32        arg1
)
{
  if ((NULL == file_name) || (NULL == format))
  {
    return;
  }
  MSG_SPRINTF_4(MSG_SSID_DS, LOG_MSG_ERROR_LEVEL,      
                "%s line: %d: %s, arg1: %lu",
                file_name, line_num, format, arg1);
}/* ds_logging_error_log_1 */

/*===========================================================================

FUNCTION DS_LOGGING_ERROR_LOG_2

DESCRIPTION
  Wrapper function for reporting error messages
 
DEPENDENCIES 

RETURN VALUE
 
SIDE EFFECTS

===========================================================================*/
void ds_logging_error_log_2
(
  const char*   file_name,
  unsigned int  line_num,
  const char*   format,
  uint32        arg1,
  uint32        arg2
)
{
  if ((NULL == file_name) || (NULL == format))
  {
    return;
  }
  MSG_SPRINTF_5(MSG_SSID_DS, LOG_MSG_ERROR_LEVEL,      
                "%s line: %d: %s, arg1: %lu, arg2: %lu",
                file_name, line_num, format, arg1, arg2);
}/* ds_logging_error_log_2 */

/*---------------------------------------------------------------------- 
  Wrappers for ASSERT and ERROR_FATAL's
----------------------------------------------------------------------*/
#define PS_UTILS_ASSERT(val)\
  ds_logging_assert( __FILENAME__ , __FUNCTION__ , __LINE__, val)

#define PS_UTILS_ERROR_FATAL(xx_exp) \
  ds_logging_err_fatal( __FILENAME__ , __FUNCTION__ , __LINE__, xx_exp)
  

/*===========================================================================
FUNCTION DS_LOGGING_ERR_FATAL

DESCRIPTION
  Wrapper function for ERROR_FATALS
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void ds_logging_err_fatal
(  
  const char      *file_name,
  const char      *func_name,
  unsigned int     line_num,
  const char       *str
)
{ 

  /* Print the File name ,Function name and Line number before calling ERR_FATAL */
  if (NULL != file_name && NULL != func_name) 
  {
    LOG_MSG_STR_INFO1_03("ERROR_FATAL at %s , function: %s ,line:%d ",
                          file_name,func_name,line_num);
  }
      
  if (NULL == str)
  {
    ERR_FATAL("Assertion failed at %s line: %d", file_name, line_num, 0 );
    return;
  }

  ERR_FATAL("Error Fatal: %s at %s line: %d", str,file_name, line_num);

}/* ds_logging_err_fatal */

/*===========================================================================

FUNCTION DS_LOGGING_ASSERT

DESCRIPTION
  Wrapper function for ASSERTION
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
void ds_logging_assert
(
  const char      *file_name,
  const char      *func_name,
  unsigned int     line_num,
  int              expression
)
{
  /* Print the file name and line number before calling ERR_FATAL */
  if ((!expression))
  {
    LOG_MSG_STR_INFO1_03("ASSERT at %s , function: %s , line: %d ASSERT",
                         file_name,func_name,line_num);
	
    ERR_FATAL("Assertion failed at %s ,function: %s , line: %d", file_name, func_name,line_num);
  }

}/* ds_logging_assert */

ps_iface_type * ps_route_look_up_iface_by_policy
(
  acl_policy_info_type          * aclPolInfoptr,
  boolean                       * arbCandidate,
  uint32                        * ifacePriorityMask,
  ps_iface_net_down_reason_type * net_down_reason
);

extern int ps_route_bring_up_iface_by_policy_ex
(
  acl_policy_info_type          * acl_pi_ptr,
  ps_iface_type                 ** if_ptr,
  int16                         * ps_errno,
  void                          * client_data_ptr,
  ps_iface_net_down_reason_type * net_down_reason
);

#ifdef __cplusplus
}
#endif

using namespace NetPlatform;
using namespace ds::Error;
using namespace ds::Net;
using namespace ds::Utils::Conversion;

static IfaceEventCbackType     ifaceCbackFcnPtr = NULL;
static FlowEventCbackType      flowCbackFcnPtr = NULL;
static PhysLinkEventCbackType  physLinkCbackFcnPtr = NULL;

void NetPlatform::Init
(
  void
)
{
  int32 result;
  int   dummy = 1;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_0 ("NetPlatform::Init(): "
                   "ds Net Platform Init()");

  result = IfaceRegAllEvents ((void*) &dummy);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = FlowRegAllEvents ((void*) &dummy);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = PhysLinkRegAllEvents ((void*) &dummy);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  return;

bail:
  PS_UTILS_ERROR_FATAL("NetPlatform::Init(): Err registering for events.");
  return;

} /* Init() */


int32 NetPlatform::GenerateAclPolicy
(
  IPolicy               *netPolicyPtr,
  ACLPolicyInfoType     *aclPolicyPtr
)
{
  int32 result;
  ds::AddrFamilyType        addrFamily;
  int                       appType;
  ds::Net::IfaceNameType    ifaceName;
  IfaceGroupType            ifaceGroup;
  PolicyFlagType            policyFlag;
  int                       umtsProfileNum;
  int                       cdmaProfileNum;
  boolean                   isRouteable;
  ps_iface_id_type          ifaceId;
  IPolicyPriv               *netPolicyPrivPtr = NULL;
  static char               apnName[255];
  int                       apnLen;
  uint32                    subsId;
  uint32                    apnType;
  boolean                   apnTypeFlag;
  boolean                   apnNameFlag;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == aclPolicyPtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::GenerateAclPolicy(): "
                     "NULL args");
    return QDS_EFAULT;
  }

  /*-----------------------------------------------------------------------
    If input Policy object is NULL, then return default inited ACL policy.
  -----------------------------------------------------------------------*/
  memset (aclPolicyPtr, 0, sizeof(ACLPolicyInfoType));
  acl_init_policy_info (aclPolicyPtr);

  if (NULL == netPolicyPtr)
  {
    return AEE_SUCCESS;
  }

  /*-----------------------------------------------------------------------
    Get the parameters set in the Policy object.
  -----------------------------------------------------------------------*/
  result = netPolicyPtr->QueryInterface(AEEIID_IPolicyPriv,
                                        (void **)&netPolicyPrivPtr);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetAddressFamily (&addrFamily);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetAppType (&appType);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetIfaceName (&ifaceName);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetIfaceGroup (&ifaceGroup);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetPolicyFlag (&policyFlag);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetCDMAProfileNumber(&cdmaProfileNum);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetUMTSProfileNumber(&umtsProfileNum);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetRoutable (&isRouteable);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetIfaceId((::ds::Net::IfaceIdType *)&ifaceId);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetUMTSAPNName(apnName, 255, &apnLen);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetSubsId((::ds::Net::SubsInfoType *)&subsId);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = netPolicyPrivPtr->GetApnType((::ds::Net::ApnInfoType *)&apnType);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = 
      netPolicyPrivPtr->GetBringUpApnTypeFlag((::ds::Net::BringUpUsingApnType*)&apnTypeFlag);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  result = 
      netPolicyPrivPtr->GetBringUpApnNameFlag((::ds::Net::BringUpUsingApnName*)&apnNameFlag);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  LOG_MSG_INFO1_9("NetPlatform::GenerateAclPolicy():"
       "ifaceId 0x%x, PolicyFlag %d, IfaceName 0x%x, IfaceGroup 0x%x, AddressFamily %d, "
       "CDMAIDSProfileNumber %d, UMTSIDSProfileNumber %d, Routable %d, appType %d",
	ifaceId,policyFlag,ifaceName,ifaceGroup,addrFamily,cdmaProfileNum,umtsProfileNum,
        isRouteable,appType);

  LOG_MSG_INFO1_3("NetPlatform::GenerateAclPolicy():"
               "apnlen %d subsid %d UMTSapnname 0x%p", apnLen, subsId, apnName);
  
  /*-----------------------------------------------------------------------
    Set the parameters obtained from Policy object into ACL policy.
  -----------------------------------------------------------------------*/
  aclPolicyPtr->app_identifier  = (unsigned int)appType;
  aclPolicyPtr->pdp_info        = (void *) umtsProfileNum;
  aclPolicyPtr->data_session_profile_id = cdmaProfileNum;
  aclPolicyPtr->policy_flag     = (dss_iface_policy_flags_enum_type)
                                  policyFlag;
  aclPolicyPtr->is_routeable    = isRouteable;
  aclPolicyPtr->apn.length      = apnLen;
  (void) strlcpy ((char *) aclPolicyPtr->apn.name,
                      (const char *) apnName,
                      255);
  aclPolicyPtr->subs_id         = subsId;
  aclPolicyPtr->apn_type        = (ps_iface_apn_enum_type)apnType;
  aclPolicyPtr->bring_up_using_apn_type = apnTypeFlag;
  aclPolicyPtr->bring_up_using_apn_name = apnNameFlag;
  /*-----------------------------------------------------------------------
    If iface ID is set, set the same into ACL policy.
    Else if iface name is set (non-zero), use the given iface name.
    Else if iface group is set (non-zero), use the iface group.
    Else use IFACE_ANY_DEFAULT iface name.
  -----------------------------------------------------------------------*/
  if (PS_IFACE_INVALID_ID != ifaceId)
  {
    aclPolicyPtr->iface.kind = DSS_IFACE_ID;
    aclPolicyPtr->iface.info.id = ifaceId;
  }
  else if (0 != ifaceName)
  {
    aclPolicyPtr->iface.kind = DSS_IFACE_NAME;
    aclPolicyPtr->iface.info.name = (dss_iface_name_enum_type) ifaceName;
  }
  else if (0 != ifaceGroup)
  {
    aclPolicyPtr->iface.kind = DSS_IFACE_NAME;
    aclPolicyPtr->iface.info.name = (dss_iface_name_enum_type) ifaceGroup;
  }
  else
  {
    aclPolicyPtr->iface.kind = DSS_IFACE_NAME;
    aclPolicyPtr->iface.info.name = DSS_IFACE_ANY_DEFAULT;
  }

  /*-----------------------------------------------------------------------
    The values of QDS_AF_INET, QDS_AF_INET6, QDS_AF_UNSPEC and DS_AF_ANY are
    not the same as IFACE_IPV4_ADDR_FAMILY etc used by ACL addr family
  -----------------------------------------------------------------------*/
  switch (addrFamily)
  {
    case ds::AddrFamily::QDS_AF_INET:
      aclPolicyPtr->ip_family = IFACE_IPV4_ADDR_FAMILY;
      break;
    case ds::AddrFamily::QDS_AF_INET6:
      aclPolicyPtr->ip_family = IFACE_IPV6_ADDR_FAMILY;
      break;
    case ds::AddrFamily::QDS_AF_UNSPEC:
      aclPolicyPtr->ip_family = IFACE_UNSPEC_ADDR_FAMILY;
      break;
    default:
      aclPolicyPtr->ip_family = IFACE_ANY_ADDR_FAMILY;
      break;
  }

  (void) netPolicyPrivPtr->Release();
  return AEE_SUCCESS;

bail:
  LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::GenerateAclPolicy(): "
                   "Conversion to ACL policy failed, err %d", result);
  if (NULL != netPolicyPrivPtr)
  {
    (void) netPolicyPrivPtr->Release();
  }
  return result;

} /* GenerateAclPolicy() */

int32 NetPlatform::RegIfaceCbackFcn
(
  IfaceEventCbackType cbackFcnPtr,
  void *              userDataPtr
)
{
  if (NULL == cbackFcnPtr)
  {
    PS_UTILS_ERROR_FATAL(0);
    return QDS_EFAULT;
  }

  ifaceCbackFcnPtr = cbackFcnPtr;
  return AEE_SUCCESS;
}

int32 NetPlatform::RegFlowCbackFcn
(
  FlowEventCbackType  cbackFcnPtr,
  void *              userDataPtr
)
{
  if (NULL == cbackFcnPtr)
  {
    PS_UTILS_ASSERT(0);
    return QDS_EFAULT;
  }

  flowCbackFcnPtr = cbackFcnPtr;
  return AEE_SUCCESS;
}

int32 NetPlatform::RegPhysLinkCbackFcn
(
  PhysLinkEventCbackType cbackFcnPtr,
  void *                 userDataPtr
)
{
  if (NULL == cbackFcnPtr)
  {
    PS_UTILS_ASSERT(0);
    return QDS_EFAULT;
  }

  physLinkCbackFcnPtr = cbackFcnPtr;
  return AEE_SUCCESS;
}

/*---------------------------------------------------------------------------
  Local structure that hold information about events being registered for.
---------------------------------------------------------------------------*/
static struct
{
  void *   psIfaceEvHandlePtr;
  void *   psFlowEvHandlePtr;
  void *   physLinkEvHandlePtr;
} EventHandleBuf;  

/*---------------------------------------------------------------------------
  Map all the PS errno values to return values for ds
---------------------------------------------------------------------------*/
//TODO: Return value would be ds::ErrorType
int NetPlatform::MapErrorCode (int16 psErrno)
throw()
{
  return PSErrnoGetDSErrno( psErrno);
} /* MapErrorCode() */

typedef iface_event_info_cache_type IfaceEventInfoCacheType;
typedef flow_event_info_cache_type FlowEventInfoCacheType;
typedef phys_link_event_info_cache_type PhysLinkEventInfoCacheType;

/*---------------------------------------------------------------------------
  Functions to cache event info.
---------------------------------------------------------------------------*/
static void CacheEventInfo
(
  int32                        handle,
  ps_iface_event_enum_type     psEvent,
  ps_iface_event_info_u_type   eventInfo
)
{
  ps_iface_type*              ifacePtr;
  ps_flow_type*               flowPtr;
  ps_phys_link_type*          physLinkPtr;
  IfaceEventInfoCacheType*    ifaceEventInfoCache;
  FlowEventInfoCacheType*     flowEventInfoCache;
  PhysLinkEventInfoCacheType* physLinkEventInfoCache;

  if (0 == handle)
  {
    LOG_MSG_ERROR_PS_INT_0 ("CacheEventInfo(): "
                     "NULL handle");
    return;
  }

  ifacePtr    = (ps_iface_type *) handle;
  flowPtr     = (ps_flow_type *) handle;
  physLinkPtr = (ps_phys_link_type *) handle;

  ifaceEventInfoCache    = &ifacePtr->event_info_cache;
  flowEventInfoCache     = &flowPtr->event_info_cache;
  physLinkEventInfoCache = &physLinkPtr->event_info_cache;


  //TODO: What is n_filters event info used for

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);
  switch (psEvent)
  {

    /* Phys link state events */
    case PHYS_LINK_DOWN_EV:
    case PHYS_LINK_COMING_UP_EV:
    case PHYS_LINK_UP_EV:
    case PHYS_LINK_GOING_DOWN_EV:
    case PHYS_LINK_GONE_EV:
    case PHYS_LINK_RESUMING_EV:
    case PHYS_LINK_GOING_NULL_EV:
      memscpy (&physLinkEventInfoCache->phys_link_event_info,
               sizeof (physLinkEventInfoCache->phys_link_event_info),
              &eventInfo.phys_link_event_info,
              sizeof (physLinkEventInfoCache->phys_link_event_info));
      break;

    case PHYS_LINK_FLOW_ENABLED_EV:
    case PHYS_LINK_FLOW_DISABLED_EV:
      physLinkEventInfoCache->flow_mask = eventInfo.flow_mask;
      break;

    case IFACE_UP_EV:
    case IFACE_COMING_UP_EV:
    case IFACE_CONFIGURING_EV:
    case IFACE_ROUTEABLE_EV:
    case IFACE_GOING_DOWN_EV:
      ifaceEventInfoCache->state = eventInfo.state;
      break;

    case IFACE_DOWN_EV:
      /* For Iface down event, also update the state so that
         GetPreviousState() can return correct state info */
      ifaceEventInfoCache->state = eventInfo.state;
      memscpy (&ifaceEventInfoCache->iface_down_info,
              sizeof (ifaceEventInfoCache->iface_down_info),
              &eventInfo.iface_down_info,
              sizeof (ifaceEventInfoCache->iface_down_info));
      break;

    case IFACE_ENABLED_EV:
    case IFACE_DISABLED_EV:
      //TODO
      break;

    case IFACE_FLOW_ENABLED_EV:
    case IFACE_FLOW_DISABLED_EV:
      ifaceEventInfoCache->flow_mask = eventInfo.flow_mask;
      break;

    case IFACE_ADDR_CHANGED_EV:
      ifaceEventInfoCache->ip_addr = eventInfo.ip_addr;
      break;

    case IFACE_ADDR_FAMILY_CHANGED_EV:
      memscpy (&ifaceEventInfoCache->addr_family_info,
              sizeof (ifaceEventInfoCache->addr_family_info),
              &eventInfo.addr_family_info,
              sizeof (ifaceEventInfoCache->addr_family_info));
      break;

    case IFACE_MTU_CHANGED_EV:
      break;

    case IFACE_DELETE_EV:
      //TODO
      break;

    case IFACE_IPFLTR_UPDATED_EV:
      memscpy (&ifaceEventInfoCache->ipfltr_info,
              sizeof (ifaceEventInfoCache->ipfltr_info),
              &eventInfo.ipfltr_info,
              sizeof (ifaceEventInfoCache->ipfltr_info));
      break;

    case IFACE_PRI_PHYS_LINK_CHANGED_EV:
      memscpy (&physLinkEventInfoCache->pri_changed_info,
              sizeof (physLinkEventInfoCache->pri_changed_info),
              &eventInfo.pri_changed_info,
              sizeof (physLinkEventInfoCache->pri_changed_info));
      break;

    case IFACE_VALID_RA_EV:
      ifaceEventInfoCache->ra_lifetime = eventInfo.ra_lifetime;
      break;

    case IFACE_MT_REQUEST_EV:
      ifaceEventInfoCache->mt_handle = eventInfo.mt_handle;
      break;

    case IFACE_MCAST_REGISTER_SUCCESS_EV:
    case IFACE_MCAST_REGISTER_FAILURE_EV:
    case IFACE_MCAST_DEREGISTERED_EV:
      ifaceEventInfoCache->mcast_info = eventInfo.mcast_info;
      break;

    case IFACE_BEARER_TECH_CHANGED_EV:
      memscpy (&ifaceEventInfoCache->bearer_tech_changed_info,
              sizeof (ifaceEventInfoCache->bearer_tech_changed_info),
              &eventInfo.bearer_tech_changed_info,
              sizeof (ifaceEventInfoCache->bearer_tech_changed_info));
      break;

#ifdef FEATURE_DATA_WLAN_MAPCON  
    case IFACE_BEARER_TECH_EX_CHANGED_EV:
      memscpy (&ifaceEventInfoCache->bearer_tech_ex_changed_info,
               sizeof (ifaceEventInfoCache->bearer_tech_ex_changed_info),
        &eventInfo.bearer_tech_ex_changed_info,
        sizeof (ifaceEventInfoCache->bearer_tech_ex_changed_info));
      break;
#endif /* FEATURE_DATA_WLAN_MAPCON */         

    case IFACE_QOS_AWARE_SYSTEM_EV:
    case IFACE_QOS_UNAWARE_SYSTEM_EV:
      ifaceEventInfoCache->qos_aware_info_code =
        eventInfo.qos_aware_info_code;
      break;

    case IFACE_FLOW_ADDED_EV:
    case IFACE_FLOW_DELETED_EV:
      //TODO
      break;

    case IFACE_ENABLE_HDR_REV0_RATE_INERTIA_SUCCESS_EV:
    case IFACE_ENABLE_HDR_REV0_RATE_INERTIA_FAILURE_EV:
      ifaceEventInfoCache->hdr_rev0_rate_inertia_failure_code =
        eventInfo.hdr_rev0_rate_inertia_failure_code;
      break;


    case IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_SESSION_CHANGED_EV:
      //TODO
      break;

    case IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_SUCCESS_EV:
    case IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_FAILURE_EV:
      memscpy (&ifaceEventInfoCache->slotted_mode_info,
               sizeof (ifaceEventInfoCache->slotted_mode_info),
              &eventInfo.slotted_mode_info,
              sizeof (ifaceEventInfoCache->slotted_mode_info));
      break;


    case IFACE_PREFIX_UPDATE_EV:
      ifaceEventInfoCache->prefix_info = eventInfo.prefix_info;
      break;

    case IFACE_IPV6_PRIV_ADDR_GENERATED_EV:
    case IFACE_IPV6_PRIV_ADDR_DEPRECATED_EV:
    case IFACE_IPV6_PRIV_ADDR_EXPIRED_EV:
    case IFACE_IPV6_PRIV_ADDR_DELETED_EV:
      ifaceEventInfoCache->priv_ipv6_addr = eventInfo.priv_ipv6_addr;
      break;

    case IFACE_OUTAGE_NOTIFICATION_EV:
      ifaceEventInfoCache->outage_notification_info =
        eventInfo.outage_notification_info;
      break;


    case IFACE_707_NETWORK_SUPPORTED_QOS_PROFILES_CHANGED_EV:
      //TODO
      break;

    case IFACE_RF_CONDITIONS_CHANGED_EV:
      ifaceEventInfoCache->rf_conditions_change_info =
        eventInfo.rf_conditions_change_info;
      break;

    case IFACE_EXTENDED_IP_CONFIG_EV:
      ifaceEventInfoCache->extended_ip_config_succeeded =
        eventInfo.extended_ip_config_succeeded;
      break;
    case IFACE_EXTENDED_IP_CONFIG_EX_EV:
      break;

    case IFACE_LINGERING_EV:
      //TODO
      break;

    case IFACE_MBMS_CONTEXT_ACT_SUCCESS_EV:
    case IFACE_MBMS_CONTEXT_ACT_FAILURE_EV:
    case IFACE_MBMS_CONTEXT_DEACT_SUCCESS_EV:
    case IFACE_MBMS_CONTEXT_DEACT_FAILURE_EV:
      //TODO
      break;

    case IFACE_AUTHENTICATING_EV:
    case IFACE_APP_PREEMPTED_EV:
      //TODO
      break;

    case IFACE_MCAST_STATUS_EV:
      break;

    case IFACE_APN_PARAM_CHANGED_EV:
      ifaceEventInfoCache->changed_apn_param_info.apn_mask =
        eventInfo.apn_param_change_info_type.apn_mask;
      memscpy(&(ifaceEventInfoCache->changed_apn_param_info.new_apn_param_values),
              sizeof(ifaceEventInfoCache->changed_apn_param_info.new_apn_param_values),
              &(eventInfo.apn_param_change_info_type.new_apn_param_values),
              sizeof(ifaceEventInfoCache->changed_apn_param_info.new_apn_param_values));
      memscpy(&(ifaceEventInfoCache->changed_apn_param_info.old_apn_param_values),
              sizeof(ifaceEventInfoCache->changed_apn_param_info.old_apn_param_values),
              &(eventInfo.apn_param_change_info_type.old_apn_param_values),
              sizeof(ifaceEventInfoCache->changed_apn_param_info.old_apn_param_values));
      break;

    case FLOW_NULL_EV:
    case FLOW_ACTIVATING_EV:
    case FLOW_ACTIVATED_EV:
    case FLOW_SUSPENDING_EV:
    case FLOW_SUSPENDED_EV:
    case FLOW_RESUMING_EV:
    case FLOW_GOING_NULL_EV:
    case FLOW_CONFIGURING_EV:
    case FLOW_INFO_CODE_UPDATED_EV:
      memscpy (&flowEventInfoCache->flow_event_info,
               sizeof (flowEventInfoCache->flow_event_info),
              &eventInfo.flow_event_info,
              sizeof (flowEventInfoCache->flow_event_info));
      break;

    case FLOW_TX_ENABLED_EV:
    case FLOW_TX_DISABLED_EV:
      flowEventInfoCache->flow_mask = eventInfo.flow_mask;
      break;

    case FLOW_RX_FLTR_UPDATED_EV:
      memscpy (&ifaceEventInfoCache->rx_fltr_info,
               sizeof (ifaceEventInfoCache->rx_fltr_info),
              &eventInfo.rx_fltr_info,
              sizeof (ifaceEventInfoCache->rx_fltr_info));
      break;

    case FLOW_MODIFY_ACCEPTED_EV:
      flowEventInfoCache->modify_result = PS_FLOW_MODIFY_RESULT_ACCEPTED;
      break;

    case FLOW_MODIFY_REJECTED_EV:
      flowEventInfoCache->modify_result = PS_FLOW_MODIFY_RESULT_REJECTED;
      break;

    case FLOW_PRIMARY_MODIFY_RESULT_EV:
      memscpy (&flowEventInfoCache->primary_qos_modify_result_info, 
               sizeof (flowEventInfoCache->primary_qos_modify_result_info),
              &eventInfo.primary_qos_modify_result_info,
              sizeof (flowEventInfoCache->primary_qos_modify_result_info));
      break;

#ifdef FEATURE_DATA_WLAN_MAPCON        
    case IFACE_HANDOFF_INIT_EV: 
    case IFACE_HANDOFF_STATUS_SUCCESS_EV:
    case IFACE_HANDOFF_STATUS_FAILURE_EV:
      break;
#endif /* FEATURE_DATA_WLAN_MAPCON */      

    case IFACE_DPD_PING_INFO_EV:

      if(ifaceEventInfoCache->dpd_ping_stat_info == NULL)
      {
        PS_SYSTEM_HEAP_MEM_ALLOC(ifaceEventInfoCache->dpd_ping_stat_info,
                                 sizeof(ps_iface_dpd_ping_stat_info_type),
                                 ps_iface_dpd_ping_stat_info_type*);
      }

      if (ifaceEventInfoCache->dpd_ping_stat_info != NULL)
      {
        memset(ifaceEventInfoCache->dpd_ping_stat_info,
               0 ,
              sizeof(ps_iface_dpd_ping_stat_info_type));

        ifaceEventInfoCache->dpd_ping_stat_info->num_pings = 
                                              eventInfo.dpd_ping_stat_info->num_pings;
        ifaceEventInfoCache->dpd_ping_stat_info->pkts_loss = 
                                              eventInfo.dpd_ping_stat_info->pkts_loss;
        ifaceEventInfoCache->dpd_ping_stat_info->avg_rtt = 
                                              eventInfo.dpd_ping_stat_info->avg_rtt;
        ifaceEventInfoCache->dpd_ping_stat_info->user_data_ptr = 
                                              eventInfo.dpd_ping_stat_info->user_data_ptr;
        memscpy(&(ifaceEventInfoCache->dpd_ping_stat_info->rtt),
                sizeof(uint16)*(ifaceEventInfoCache->dpd_ping_stat_info->num_pings),
                &(eventInfo.dpd_ping_stat_info->rtt),
                sizeof(uint16)*(eventInfo.dpd_ping_stat_info->num_pings));
      }
      break;

    default:
      break;

  } /* switch (psEvent) */
  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

} /* CacheEventInfo() */

/*---------------------------------------------------------------------------
  Callback function for iface, flow and phys link events.
---------------------------------------------------------------------------*/
static void IfaceEventCback
(
  ps_iface_type             *ifacePtr,
  ps_iface_event_enum_type   event,
  ps_iface_event_info_u_type eventInfo,
  void                      *userDataPtr
)
{
  int32                      ifaceHandle;
/*-------------------------------------------------------------------------*/

  DS_INTERFACE_LOG_MSG_INFO1_2 (PS_LOGGING_CAT_DSNET,"IfaceEventCback(), handle=0x%p, ev=%d",
                                ifacePtr, event);

  if ( NULL == ifacePtr )
  {
    LOG_MSG_ERROR_PS_INT_0 ("IfaceEventCback(), NULL iface_ptr");
    return;
  }

  ifaceHandle = (int32) ifacePtr;

  /* Store event info */
  CacheEventInfo (ifaceHandle, event, eventInfo);

  if (0 != ifaceCbackFcnPtr)
  {
    ifaceCbackFcnPtr(ifaceHandle, (int32) event, eventInfo, userDataPtr);
  }
} /* IfaceEventCback() */


static void FlowEventCback
(
  ps_flow_type                 *flowPtr,
  ps_iface_event_enum_type      event,
  ps_iface_event_info_u_type    eventInfo,
  void                         *userDataPtr
)
{
  int32                         flowHandle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#if 0
  LOG_MSG_INFO2_2 ("FlowEventCback(): handle 0x%p, ev %d", flowPtr, event);
#endif

  if (NULL == flowPtr)
  {
    LOG_MSG_ERROR_PS_INT_0("FlowEventCback(): "
                    "NULL flow handle");
    return;
  }

  flowHandle = (int32) flowPtr;

  /* Store event info */
  CacheEventInfo (flowHandle, event, eventInfo);
  if (0 != flowCbackFcnPtr)
  {
    flowCbackFcnPtr(flowHandle, (int32) event, eventInfo, userDataPtr);
  }
} /* FlowEventCback() */

static void PhysLinkEventCback
(
  ps_phys_link_type           *physLinkPtr,
  ps_iface_event_enum_type     event,
  ps_iface_event_info_u_type   eventInfo,
  void                        *userDataPtr
)
{
  int32                        physLinkHandle;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_2 ("PhysLinkEventCback() handle=0x%p, ev=%d",
                   (int32)physLinkPtr, event);

  if (NULL == physLinkPtr)
  {
    LOG_MSG_ERROR_PS_INT_0("PhysLinkEventCback(): "
                    "NULL phys link handle");
    return;
  }

  physLinkHandle = (int32) physLinkPtr;

  /* Store event info */
  CacheEventInfo (physLinkHandle, event, eventInfo);
  if (0 != physLinkCbackFcnPtr)
  {
    physLinkCbackFcnPtr(physLinkHandle, (int32) event, eventInfo, userDataPtr);
  }
} /* PhysLinkEventCback() */

/*===========================================================================

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
/**
  @brief      Helper IOCTL fcn which also verified if NULL arg is passed.

  @param[in]  ifaceHandle - Interface Handle
  @param[in]  ioctlName - Name of the IOCTL
  @param      argValPtr - Pointer to arg val. This is an in/out param.

  @see        NetPlatform::IfaceIoctlEnumType

  @return     AEE_SUCCESS on successful IOCTL call
              Error code on error.
*/
int32 NetPlatform::IfaceIoctlNonNullArg
(
  int32 ifaceHandle,
  NetPlatform::IfaceIoctlEnumType ioctlName,
  void *argValPtr
)
throw()
{
  if (NULL == argValPtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::IfaceIoctlNonNullArg(): "
                     "NULL args");
    return QDS_EFAULT;
  }

  return IfaceIoctl(ifaceHandle, ioctlName, argValPtr);

} /* IfaceIoctlNonNullArg() */

/**
  @brief      This function calls the ps_iface_ioctl to perform interface
              specific operations.

  @param[in]  ifaceHandle - Interface Handle
  @param[in]  ioctlName - Name of the IOCTL
  @param      argValPtr - Pointer to arg val. This is an in/out param.

  @see        NetPlatform::IfaceIoctlEnumType

  @return     AEE_SUCCESS on successful IOCTL call
              Error code on error.
*/
int32 NetPlatform::IfaceIoctl
(
  int32 ifaceHandle,
  NetPlatform::IfaceIoctlEnumType ioctlName,
  void *argValPtr
)
throw()
{
  ps_iface_type*   ifacePtr;
  int32            result;
  int16            psErrno;
  boolean          isEventInfoCached;
  int64            timeToOutage;
  int64            outageDuration;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_2 ("NetPlatform::IfaceIoctl(): "
                   "IfaceIoctl() iface handle 0x%p, ioctl name %d",
                   ifaceHandle, ioctlName);

  ifacePtr = (ps_iface_type *) ifaceHandle;

  /*-------------------------------------------------------------------------
    If net handle associated with this object is NULL, return QDS_EBADF
    GET_ALL_IFACES is a special case, iface pointer need not be valid here.
  -------------------------------------------------------------------------*/
  if (0 == ifacePtr && IFACE_IOCTL_GET_ALL_IFACES != ioctlName)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::IfaceIoctl(): "
                     "NULL iface pointer");
    return QDS_EBADF;
  }

  /*-------------------------------------------------------------------------
    For the following IOCTLs, get the cached info from iface layer.
  -------------------------------------------------------------------------*/
  isEventInfoCached = FALSE;

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  switch (ioctlName)
  {
    case IFACE_IOCTL_GET_PREVIOUS_STATE:
      if (NULL == argValPtr)
      {
        result = QDS_EFAULT;
        break;
      }

      *((ps_iface_state_enum_type *)argValPtr) =
        ifacePtr->event_info_cache.state;
      isEventInfoCached = TRUE;
      break;

    case IFACE_IOCTL_GET_PREVIOUS_IP_ADDR:
      if (NULL == argValPtr)
      {
        result = QDS_EFAULT;
        break;
      }

      memscpy (argValPtr,
               sizeof (ps_ip_addr_type),
              &ifacePtr->event_info_cache.ip_addr,
              sizeof (ps_ip_addr_type));
      isEventInfoCached = TRUE;
      break;

    case IFACE_IOCTL_GET_PREVIOUS_BEARER_TECHNOLOGY:
      if (NULL == argValPtr)
      {
        result = QDS_EFAULT;
        break;
      }
      memscpy (argValPtr,
               sizeof (ps_iface_bearer_technology_type),
              &ifacePtr->event_info_cache.bearer_tech_changed_info.old_bearer_tech,
              sizeof (ps_iface_bearer_technology_type));
      isEventInfoCached = TRUE;
      break;

    case IFACE_IOCTL_GET_QOS_AWARE_INFO_CODE:
      if (NULL == argValPtr)
      {
        result = QDS_EFAULT;
        break;
      }
      *((ps_extended_info_code_enum_type *)argValPtr) =
        ifacePtr->event_info_cache.qos_aware_info_code;
      isEventInfoCached = TRUE;
      break;

    case IFACE_IOCTL_GET_HDR_REV0_RATE_INERTIA_RESULT_INFO_CODE:
      if (NULL == argValPtr)
      {
        result = QDS_EFAULT;
        break;
      }
      *((ps_hdr_rev0_rate_inertia_failure_code_enum_type *)argValPtr) =
        ifacePtr->event_info_cache.hdr_rev0_rate_inertia_failure_code;
      isEventInfoCached = TRUE;
      break;

    case IFACE_IOCTL_GET_OUTAGE_NOTIFICATION_INFO:
      if (NULL == argValPtr)
      {
        result = QDS_EFAULT;
        break;
      }

      memscpy (argValPtr,
               sizeof (ps_iface_outage_notification_event_info_type),
              &ifacePtr->event_info_cache.outage_notification_info,
              sizeof (ps_iface_outage_notification_event_info_type));

      timeToOutage = (int64)((OutageNotificationInfoType*)argValPtr)->time_to_outage - (int64)msclock();
      outageDuration = (int64)((OutageNotificationInfoType*)argValPtr)->duration;
      if (timeToOutage > 0)
      {
        ((OutageNotificationInfoType*)argValPtr)->time_to_outage = (uint32)timeToOutage;
      }
      else if (outageDuration + timeToOutage > 0)
      {
        ((OutageNotificationInfoType*)argValPtr)->time_to_outage = 0;
        ((OutageNotificationInfoType*)argValPtr)->duration = (uint32)(outageDuration + timeToOutage);
      }
      else
      {
        ((OutageNotificationInfoType*)argValPtr)->time_to_outage = 0;
        ((OutageNotificationInfoType*)argValPtr)->duration = 0;
      }
      isEventInfoCached = TRUE;
      break;

    case IFACE_IOCTL_GET_CHANGED_APN_PARAM_INFO:
      if (NULL == argValPtr)
      {
        result = QDS_EFAULT;
        break;
      }
      memscpy(argValPtr,
              sizeof(ps_iface_changed_apn_param_info_type),
              &ifacePtr->event_info_cache.changed_apn_param_info,
              sizeof(ps_iface_changed_apn_param_info_type));
      isEventInfoCached = TRUE;
      break;

    default:
      break;
  } /* switch */

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  //TODO: Not checking for EFAULT for result

  if (TRUE == isEventInfoCached)
  {
    return AEE_SUCCESS;
  }

  result = ps_iface_ioctl (ifacePtr,
                           (ps_iface_ioctl_type)ioctlName,
                           argValPtr,
                           &psErrno);
  if (DSS_ERROR == result)
  {
    LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::IfaceIoctl(): "
                     "ps iface ioctl returned -1, err %d", psErrno);
    result = MapErrorCode(psErrno);
  }

  return result;
}

/**
  @brief      This function calls the ps_iface_ioctl to perform interface
              specific operations.

  @param[in]  ifaceHandle - Interface Handle
  @param[in]  ioctlName - Name of the IOCTL
  @param      argValPtr - Pointer to arg val. This is an in/out param.

  @see        NetPlatform::IfaceIoctlEnumType

  @return     AEE_SUCCESS on successful IOCTL call
              Error code on error.
*/
int32 NetPlatform::IfaceIoctlByIfaceId
(
  int32                              ifaceId,
  NetPlatform::IfaceIoctlEnumType    ioctlName,
  void                             * argValPtr
)
{
  ps_iface_type *  ifacePtr;
  int32            result;
  int16            psErrno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Determine the specific ps_iface_ptr based on the iface ID
  -------------------------------------------------------------------------*/
  ifacePtr = ps_iface_get_handle( (ps_iface_id_type) ifaceId );
  if (ifacePtr == NULL)
  {
    LOG_MSG_ERROR_PS_INT_1("NetPlatform::IfaceIoctlByIfaceId(): "
                    "Invalid iface id: %d", ifaceId);
    return QDS_EBADF;
  }

  result = ps_iface_ioctl (ifacePtr,
                           (ps_iface_ioctl_type)ioctlName,
                           argValPtr,
                           &psErrno);
  if (DSS_ERROR == result)
  {
    LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::IfaceIoctlByIfaceId(): "
                     "ps iface ioctl returned -1, err %d", psErrno);
    result = MapErrorCode(psErrno);
  }

  return result;
}

int32 NetPlatform::FlowIoctlNonNullArg
(
  int32                          flowHandle,
  FlowIoctlEnumType              ioctlName,
  void                          *argValPtr
)
{
  if (NULL == argValPtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::FlowIoctlNonNullArg(): "
                     "NULL args");
    return QDS_EFAULT;
  }

  return FlowIoctl (flowHandle, ioctlName, argValPtr);
}

/**
  @brief      This function calls the ps_flow_ioctl to perform flow
              specific operations.

  @param[in]  flowHandle - Interface Handle
  @param[in]  ioctlName - Name of the IOCTL
  @param      argValPtr - Pointer to arg val. This is an in/out param.

  @see        NetPlatform::FlowIoctlEnumType

  @return     AEE_SUCCESS on successful IOCTL call
              Error code on error.
*/
int32 NetPlatform::FlowIoctl
(
 int32 flowHandle,
 NetPlatform::FlowIoctlEnumType ioctlName,
 void *argValPtr
)
{
  ps_flow_type*    flowPtr;
  int32            result;
  int16            psErrno;
  boolean          isEventInfoCached;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_2 ("NetPlatform::FlowIoctl(): "
                   "FlowIoctl() flow handle 0x%p, ioctl name %d",
                   flowHandle, ioctlName);

  flowPtr = (ps_flow_type *) flowHandle;

  if (NULL == flowPtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::FlowIoctl(): "
                     "NULL flow handle");
    return AEE_SUCCESS;
  }

  isEventInfoCached = FALSE;

  PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

  switch (ioctlName)
  {
    case FLOW_IOCTL_GET_MODIFY_RESULT:
    isEventInfoCached = TRUE;

    *((QoSResultType *)argValPtr) =
      (QoSResultType)
      flowPtr->event_info_cache.modify_result;
    break;


    case FLOW_IOCTL_GET_PRIMARY_QOS_MODIFY_RESULT:
    isEventInfoCached = TRUE;

    *((boolean *)argValPtr) =
      flowPtr->event_info_cache.primary_qos_modify_result_info. \
      is_modify_succeeded;
    break;


    case FLOW_IOCTL_GET_FLOW_UPDATED_INFO_CODE:
    isEventInfoCached = TRUE;

    *((QoSInfoCodeType*)argValPtr) =
      (QoSInfoCodeType)
      flowPtr->event_info_cache.flow_event_info.info_code;
    break;

    default:
    /* Do nothing */
    break;
  }

  PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

  if (TRUE == isEventInfoCached)
  {
    return AEE_SUCCESS;
  }

  result = ps_flow_ioctl (flowPtr,
                          (ps_flow_ioctl_type)ioctlName,
                          argValPtr,
                          &psErrno);
  if (-1 == result)
  {
    LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::FlowIoctl(): "
                     "ps flow ioctl failed, err %d", psErrno);
    result = MapErrorCode(psErrno);
  }

  return result;
}

/**
  @brief      This function calls the ps_phys_link_ioctl to perform phys
              link specific operations.

  @param[in]  physLinkHandle - Interface Handle
  @param[in]  ioctlName - Name of the IOCTL
  @param      argValPtr - Pointer to arg val. This is an in/out param.

  @see        NetPlatform::PhysLinkIoctlEnumType

  @return     AEE_SUCCESS on successful IOCTL call
              Error code on error.
*/
int32 NetPlatform::PhysLinkIoctl
(
 int32 physLinkHandle,
 NetPlatform::PhysLinkIoctlEnumType ioctlName,
 void *argValPtr
)
{
  ps_phys_link_type*   physLinkPtr;
  int32                result;
  int16                psErrno;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_2 ("NetPlatform::PhysLinkIoctl(): "
                   "phys_link_ioctl() handle 0x%p, ioctl name %d",
                   physLinkHandle, ioctlName);

  physLinkPtr = (ps_phys_link_type *) physLinkHandle;

  if (NULL == physLinkPtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::PhysLinkIoctl(): "
                     "NULL phys link handle");
    return AEE_SUCCESS;
  }

  /*-------------------------------------------------------------------------
    For the GET_PREVIOUS_STATE IOCTL get the cached event info.
  -------------------------------------------------------------------------*/
  if (PHYS_LINK_IOCTL_GET_PREVIOUS_STATE == ioctlName)
  {
    if (NULL == argValPtr)
    {
      return QDS_EFAULT;
    }

    PS_ENTER_CRIT_SECTION(&global_ps_crit_section);

    (*(phys_link_state_type*)argValPtr) =
      physLinkPtr->event_info_cache.phys_link_event_info.state;

    PS_LEAVE_CRIT_SECTION(&global_ps_crit_section);

    return AEE_SUCCESS;
  }

  result = ps_phys_link_ioctl (physLinkPtr,
                               (ps_phys_link_ioctl_type)ioctlName,
                               argValPtr,
                               &psErrno);
  if (-1 == result)
  {
    LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::PhysLinkIoctl(): "
                     "ps phys link ioctl failed, err %d", psErrno);
    result = MapErrorCode(psErrno);
  }

  return result;
}


int32 NetPlatform::IfaceRegAllEvents
(
  void*  userDataPtr
)
{
  int32 result;

  /*---------------------------------------------------------------------------
    Local array that holds the list of iface events being registered for
    Note - To subscribe/register for a new event, just add it in the array
  ---------------------------------------------------------------------------*/

  ps_iface_event_enum_type	ifaceEvents[] = 
  {
    IFACE_ADDR_CHANGED_EV,
    IFACE_ADDR_FAMILY_CHANGED_EV,
    IFACE_UP_EV,
    IFACE_CONFIGURING_EV,
    IFACE_ROUTEABLE_EV,
    IFACE_DOWN_EV,
    IFACE_ACTIVE_OUT_OF_USE_EV,
    IFACE_COMING_UP_EV,
    IFACE_GOING_DOWN_EV,
    IFACE_LINGERING_EV,
    IFACE_MT_REQUEST_EV,
    IFACE_MCAST_REGISTER_SUCCESS_EV,
    IFACE_MCAST_REGISTER_FAILURE_EV,
    IFACE_MCAST_DEREGISTERED_EV,
    IFACE_IPV6_EXTERNAL_ADDR_DELETED_EV,
    IFACE_BEARER_TECH_CHANGED_EV,
    IFACE_FAST_DORMANCY_STATUS_EV,
    IFACE_EXTENDED_IP_CONFIG_EV,
    IFACE_OUTAGE_NOTIFICATION_EV,
    IFACE_RF_CONDITIONS_CHANGED_EV,
    IFACE_APP_PREEMPTED_EV,
    IFACE_ENABLE_HDR_REV0_RATE_INERTIA_SUCCESS_EV,
    IFACE_ENABLE_HDR_REV0_RATE_INERTIA_FAILURE_EV,
    IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_SUCCESS_EV,
    IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_FAILURE_EV,
    IFACE_ENABLE_HDR_SET_EIDLE_SLOTTED_MODE_SESSION_CHANGED_EV,

#ifdef FEATURE_HDR_BCMCS_2p0
    #error code not present
#endif /* FEATURE_HDR_BCMCS_2p0 */

#ifdef FEATURE_DATA_WLAN_MAPCON    
    IFACE_BEARER_TECH_EX_CHANGED_EV,
#endif /* FEATURE_DATA_WLAN_MAPCON */

#ifdef FEATURE_DATA_PS_IPV6
    IFACE_PREFIX_UPDATE_EV,
    IFACE_IPV6_PRIV_ADDR_GENERATED_EV,
    IFACE_IPV6_PRIV_ADDR_DEPRECATED_EV,
    IFACE_IPV6_PRIV_ADDR_DELETED_EV,
#endif /* FEATURE_DATA_PS_IPV6 */

#ifdef FEATURE_DATA_PS_QOS
    IFACE_FLOW_ADDED_EV,
    IFACE_FLOW_DELETED_EV,
    IFACE_QOS_AWARE_SYSTEM_EV,
    IFACE_QOS_UNAWARE_SYSTEM_EV,
    IFACE_707_NETWORK_SUPPORTED_QOS_PROFILES_CHANGED_EV,
    IFACE_PRI_PHYS_LINK_CHANGED_EV,
    IFACE_NET_INITIATED_QOS_AVAILABLE_EV,
#endif /* FEATURE_DATA_PS_QOS */

#ifdef FEATURE_DATA_WLAN_MAPCON    
    IFACE_HANDOFF_INIT_EV,
    IFACE_HANDOFF_STATUS_SUCCESS_EV,
    IFACE_HANDOFF_STATUS_FAILURE_EV,
#endif /* FEATURE_DATA_WLAN_MAPCON */

    IFACE_EXTENDED_IP_CONFIG_EX_EV,
    IFACE_APN_PARAM_CHANGED_EV,
    IFACE_DPD_PING_INFO_EV
  };

  /*-------------------------------------------------------------------------
    The event registration functions need to be called only once. This static
    variable keep track of if the registration is happening more than once.
  -------------------------------------------------------------------------*/
  static boolean ifaceEventsRegistered = FALSE;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_0 ("NetPlatform::IfaceRegAllEvents(): "
                   "IfaceRegAllEvents() called");

  if (TRUE == ifaceEventsRegistered)
  {
    PS_UTILS_ERROR_FATAL("NetPlatform::IfaceRegAllEvents(): IfaceRegAllEvents() called more than once!");
    return AEE_EFAILED;
  }

 /*----------------------------------------------------------------------------------------------------------------------------
   Subscribe to list of events on iface
 --------------------------------------------------------------------------*/

  EventHandleBuf.psIfaceEvHandlePtr = ps_iface_alloc_event_handle
    ( NULL, IfaceEventCback, userDataPtr);

  if (NULL == EventHandleBuf.psIfaceEvHandlePtr)
  {
    result = AEE_ENOMEMORY;
    goto bail;
  }

  if (ps_iface_subscribe_event_list(EventHandleBuf.psIfaceEvHandlePtr,
                                    ifaceEvents,
                                    ARRAY_SIZE(ifaceEvents)) != 0)
  {
    ps_iface_free_event_handle(EventHandleBuf.psIfaceEvHandlePtr);
    result = QDS_EFAULT;
    goto bail;
  }

  ifaceEventsRegistered = TRUE;
  return AEE_SUCCESS;

bail:
  LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::IfaceRegAllEvents(): "
                   "Ev reg failed, err %d", result);
  PS_UTILS_ASSERT(0);
  return result;

}

int32 NetPlatform::FlowRegAllEvents
(
  void*  userDataPtr
)
{
  int32 result;
  /*---------------------------------------------------------------------------
    Local array that holds the list of flow events being registered for
    Note - To subscribe/register for a new event, just add it in the array
  ---------------------------------------------------------------------------*/
  
  ps_iface_event_enum_type	flowEvents[] = 
  {
    FLOW_TX_ENABLED_EV,
  
#ifdef FEATURE_DATA_PS_QOS
    FLOW_ACTIVATED_EV,
    FLOW_SUSPENDED_EV,
    FLOW_NULL_EV,
    FLOW_MODIFY_ACCEPTED_EV,
    FLOW_MODIFY_REJECTED_EV,
    FLOW_PRIMARY_MODIFY_RESULT_EV,
    FLOW_INFO_CODE_UPDATED_EV
#endif /* FEATURE_DATA_PS_QOS */
  };
  
  /*-------------------------------------------------------------------------
    The event registration functions need to be called only once. This static
    variable keep track of if the registration is happening more than once.
  -------------------------------------------------------------------------*/
  static boolean flowEventsRegistered = FALSE;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_0 ("NetPlatform::FlowRegAllEvents(): "
                   "FlowRegAllEvents() called");
  if (TRUE == flowEventsRegistered)
  {
    PS_UTILS_ERROR_FATAL("NetPlatform::FlowRegAllEvents(): FlowRegAllEvents() called more than once!");
    return AEE_EFAILED;
  }

  /*---------------------------------------------------------------------------------------------------------------------------------
     Subscribe to list of events on flow
   --------------------------------------------------------------------------*/
  EventHandleBuf.psFlowEvHandlePtr = ps_flow_alloc_event_handle
    ( NULL, FlowEventCback, userDataPtr);

  if (NULL == EventHandleBuf.psFlowEvHandlePtr)
  {
    result = AEE_ENOMEMORY;
    goto bail;
  }

  if (ps_flow_subscribe_event_list( EventHandleBuf.psFlowEvHandlePtr,
                                    flowEvents,
                                    ARRAY_SIZE(flowEvents)) != 0)
  {
    ps_flow_free_event_handle( EventHandleBuf.psFlowEvHandlePtr);
    result = QDS_EFAULT;
    goto bail;
  }

  flowEventsRegistered = TRUE;
  return AEE_SUCCESS;

bail:
  LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::FlowRegAllEvents(): "
                   "Ev reg failed, err %d", result);
  PS_UTILS_ASSERT(0);
  return result;

} /* FlowRegAllEvents() */


int32 NetPlatform::PhysLinkRegAllEvents
(
  void*  userDataPtr
)
{
  int32 result;

    
  /*---------------------------------------------------------------------------
    Local array that holds the list of phys link events being registered for
    Note - To subscribe/register for a new event, just add it in the array
  ---------------------------------------------------------------------------*/
  
  ps_iface_event_enum_type	physLinkEvents [] = 
  {
    PHYS_LINK_FLOW_ENABLED_EV,
    PHYS_LINK_UP_EV,
    PHYS_LINK_DOWN_EV,
    PHYS_LINK_GONE_EV,
    PHYS_LINK_COMING_UP_EV,
    PHYS_LINK_GOING_DOWN_EV,
    PHYS_LINK_GOING_NULL_EV,
    PHYS_LINK_RESUMING_EV,    
    PHYS_LINK_RAN_ASST_MEDIA_ADAPT_INFO_EV,
    PHYS_LINK_PACKET_DISCARD_INFO_EV
  };
  /*-------------------------------------------------------------------------
    The event registration functions need to be called only once. This static
    variable keep track of if the registration is happening more than once.
  -------------------------------------------------------------------------*/
  static boolean physLinkEventsRegistered = FALSE;
/*-------------------------------------------------------------------------*/

  LOG_MSG_INFO2_0 ("NetPlatform::PhysLinkRegAllEvents(): "
                   "PhysLinkRegAllEvents() called");

  if (TRUE == physLinkEventsRegistered)
  {
    PS_UTILS_ERROR_FATAL("NetPlatform::PhysLinkRegAllEvents(): PhysLinkRegAllEvents() called more than once!");
    return AEE_EFAILED;
  }

  /*--------------------------------------------------------------------------------------------------------------------
     Subscribe to list of events on phys link
   --------------------------------------------------------------------------*/

  EventHandleBuf.physLinkEvHandlePtr = ps_phys_link_alloc_event_handle
    ( NULL, PhysLinkEventCback, userDataPtr);

  if (NULL == EventHandleBuf.physLinkEvHandlePtr )
  {
    result = AEE_ENOMEMORY;
    goto bail;
  }

  if (ps_phys_link_subscribe_event_list( EventHandleBuf.physLinkEvHandlePtr,
                                         physLinkEvents,
                                         ARRAY_SIZE(physLinkEvents)) != 0)
  {
    ps_phys_link_free_event_handle( EventHandleBuf.physLinkEvHandlePtr);
    result = QDS_EFAULT;
    goto bail;
  }

  physLinkEventsRegistered = TRUE;
  return AEE_SUCCESS;

bail:
  LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::PhysLinkRegAllEvents(): "
                   "Ev reg failed, err %d", result);
  PS_UTILS_ASSERT(0);
  return result;

}

int32 NetPlatform::IfaceBringUpCmd
(
 int32 ifaceHandle,
 void *userDataPtr
)
{
  ps_iface_type*   ifacePtr;
  int32            result;
  int16            psErrno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_1 ("NetPlatform::IfaceBringUpCmd(): "
                   "IfaceBringUpCmd() iface handle 0x%p", ifaceHandle);

  ifacePtr = (ps_iface_type *) ifaceHandle;

  if (NULL == ifacePtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::IfaceBringUpCmd(): "
                     "NULL iface handle");
    return QDS_EBADF;
  }

  result = ps_iface_bring_up_cmd(ifacePtr, &psErrno, userDataPtr);

  if (DSS_ERROR ==  result)
  {
    if (DS_EWOULDBLOCK != psErrno)
    {
      LOG_MSG_ERROR_PS_INT_2 ("NetPlatform::IfaceBringUpCmd(): "
                       "IfaceBringUpCmd() returned %d, err %d",
                       result, psErrno);
    }
    result = MapErrorCode (psErrno);
  }

  return result;

} /* IfaceBringUpCmd() */


int32 NetPlatform::IfaceTearDownCmd
(
 int32 ifaceHandle,
 void *userDataPtr
)
{
  ps_iface_type*   ifacePtr;
  int32            result;
  int16            psErrno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_1 ("NetPlatform::IfaceTearDownCmd(): "
                   "IfaceTearDownCmd() iface handle 0x%p", ifaceHandle);

  ifacePtr = (ps_iface_type *) ifaceHandle;

  if (NULL == ifacePtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::IfaceTearDownCmd(): "
                     "NULL iface handle");
    return QDS_EBADF;
  }

  result = ps_iface_tear_down_cmd(ifacePtr, &psErrno, userDataPtr);

  if (-1 == result)
  {
    if (DS_EWOULDBLOCK != psErrno)
    {
      LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::IfaceTearDownCmd(): "
                       "IfaceTearDownCmd() failed, err %d", result);
    }
    result = MapErrorCode (psErrno);
  }

  return result;

} /* IfaceTearDownCmd() */


int32 NetPlatform::IfaceTearDownCmdEx
(
 int32     ifaceHandle,
 IPolicy *   pIDSNetPolicy,
 void *    userDataPtr
)
{
  ps_iface_type*   ifacePtr;
  int32            result;
  int16            psErrno;
  NetPlatform::ACLPolicyInfoType  aclPolicy;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_1 ("NetPlatform::IfaceTearDownCmdEx(): "
                   "IfaceTearDownCmd() iface handle 0x%p", ifaceHandle);

  ifacePtr = (ps_iface_type *) ifaceHandle;

  if (NULL == ifacePtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::IfaceTearDownCmdEx(): "
                     "NULL iface handle");
    return QDS_EBADF;
  }

  result = GenerateAclPolicy (pIDSNetPolicy, &aclPolicy);
  if (AEE_SUCCESS != result)
  {
    LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::IfaceTearDownCmdEx(): "
                     "Conversion to ACL policy failed, err %d", result);
    return result;
  }
  result = ps_route_tear_down_iface_by_policy (ifacePtr,
                                               &aclPolicy,
                                      &psErrno,
                                      userDataPtr);

  if (-1 == result)
  {
    if (DS_EWOULDBLOCK != psErrno)
    {
      LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::IfaceTearDownCmdEx(): "
                       "IfaceTearDownCmd() failed, err %d", result);
    }
    result = MapErrorCode (psErrno);
  }

  return result;

} /* IfaceTearDownCmd() */

int32 NetPlatform::IfaceLookUpByPolicy
(
  IPolicy *    pIDSNetPolicy,
  int32 *      pIfaceHandle
)
{
  NetPlatform::ACLPolicyInfoType  aclPolicy;
  int32                           result = AEE_SUCCESS;
  ps_iface_type *                 ifacePtr;
  uint32                          ifacePrioMask = 0;
  boolean                         arbCandidate = FALSE;
  ps_iface_net_down_reason_type   net_down_reason = PS_NET_DOWN_REASON_NOT_SPECIFIED;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == pIfaceHandle)
  {
    PS_UTILS_ASSERT(0);
    return QDS_EFAULT;
  }

  result = GenerateAclPolicy (pIDSNetPolicy, &aclPolicy);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  aclPolicy.lookup_only = TRUE;
  aclPolicy.bring_up    = FALSE;

  ifacePtr = ps_route_look_up_iface_by_policy(&aclPolicy, &arbCandidate, 
                                              &ifacePrioMask, &net_down_reason);
  if (ifacePtr == NULL)
  {
    result = QDS_ENETNONET;
    goto bail;
  }

  *pIfaceHandle = (int32) ifacePtr;
  LOG_MSG_INFO2_0 ("NetPlatform::IfaceLookUpByPolicy(): "
                   "Lookup succeeded");
  return AEE_SUCCESS;

bail:
  LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::IfaceLookUpByPolicy(): "
                   "Lookup failed, err %d", result);
  return result;
}

int32 NetPlatform::IfaceBringUpByPolicy
(
  IPolicy *    pIDSNetPolicy,
  int32 *      pIfaceHandle,
  int32                         * pAppPriority,
  ps_iface_net_down_reason_type * net_down_reason
)
{
  NetPlatform::ACLPolicyInfoType  aclPolicy;
  int32                           result = AEE_SUCCESS;
  ps_iface_type *                 ifacePtr;
  int16                           psErrno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == pIfaceHandle)
  {
    PS_UTILS_ASSERT(0);
    return QDS_EFAULT;
  }

  if (NULL == pAppPriority)
  {
    PS_UTILS_ASSERT(0);
    return QDS_EINVAL;
  }

  result = GenerateAclPolicy (pIDSNetPolicy, &aclPolicy);
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  aclPolicy.lookup_only = FALSE;
  aclPolicy.bring_up    = TRUE;

  result = ps_route_bring_up_iface_by_policy_ex(&aclPolicy, &ifacePtr, &psErrno, NULL, net_down_reason);
  if (-1 == result)
  {
    result = MapErrorCode (psErrno);
  }

  *pAppPriority = ps_policy_mgr_get_priority
                 (
                   aclPolicy.app_identifier,
                   aclPolicy.data_session_profile_id
                 );

  if (AEE_SUCCESS == result ||
      AEE_EWOULDBLOCK == result ||
      QDS_ENETCLOSEINPROGRESS == result)
  {
    *pIfaceHandle = (int32) ifacePtr;
  }
  else
  {
    goto bail;
  }

  return result;

bail:
  LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::IfaceBringUpByPolicy(): "
                   "Bringup failed, err %d", result);
  return result;
}



int32 NetPlatform::PhysLinkUpCmd
(
 int32 physLinkHandle,
 void *userDataPtr
)
{
  ps_phys_link_type*   physLinkPtr;
  int32                result;
  int16                psErrno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_1 ("NetPlatform::PhysLinkUpCmd(): "
                   "PhysLinkUpCmd() phys link handle 0x%x", physLinkHandle);

  physLinkPtr = (ps_phys_link_type *) physLinkHandle;

  if (NULL == physLinkPtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::PhysLinkUpCmd(): "
                     "NULL phys link handle");
    return QDS_EINVAL;
  }

  result = ps_phys_link_up_cmd(physLinkPtr, &psErrno, userDataPtr);
  if (-1 == result)
  {
    result = MapErrorCode (psErrno);
  }
  else
  {
    result = AEE_SUCCESS;
  }

  ps_wmk_logging_modem_embedded_calls_info(physLinkPtr,
                                           PS_WMK_LOGGING_REORIG_FROM_DORMANCY);

  return result;

} /* PhysLinkUpCmd() */


int32 NetPlatform::IfaceGetAddr
(
  int32                     ifaceHandle,
  const ds::INAddr6Type *   v6RemoteIPAddrPtr,
  ds::INAddr6Type *         ifaceIPAddr
)
{
  ps_iface_type *       psIfacePtr;
  ps_ip_addr_type       ipAddr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == v6RemoteIPAddrPtr || 0 == ifaceIPAddr)
  {
    PS_UTILS_ERROR_FATAL("NetPlatform::IfaceGetAddr(): NULL args");
    return QDS_EFAULT;
  }

  psIfacePtr = reinterpret_cast <ps_iface_type *> ( ifaceHandle);

  if (NULL == psIfacePtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::IfaceGetAddr(): "
                     "NULL iface handle");
    return QDS_EINVAL;
  }

  ps_iface_get_ipv6_addr
  (
    psIfacePtr,
    reinterpret_cast <const struct ps_in6_addr *> ( v6RemoteIPAddrPtr),
    &ipAddr
  );

  if (ipAddr.type != IPV6_ADDR)
  {
    LOG_MSG_ERROR_PS_INT_2("NetPlatform::IfaceGetAddr(): "
                    "Invalid addr type %d expected %d",
                    ipAddr.type, IPV6_ADDR);
    return QDS_EINVAL;
  }

  memscpy( ifaceIPAddr, 
           sizeof( ds::INAddr6Type), 
           ipAddr.addr.v6.ps_s6_addr, 
           sizeof( ds::INAddr6Type));
  return AEE_SUCCESS;

} /* NetPlatform::IfaceGetAddr() */


boolean NetPlatform::IsDefaultIPv6AddrStateValid
(
  int32  psIfaceHandle
)
{
#ifdef FEATURE_DATA_PS_IPV6
  ps_iface_type *  psIfacePtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  psIfacePtr = reinterpret_cast < ps_iface_type *> ( psIfaceHandle);
  if (NULL == psIfacePtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::IsDefaultIPv6AddrStateValid(): "
                     "NULL iface handle");
    return FALSE;
  }

  return PS_IFACE_IPV6_DEFAULT_ADDR_STATE_VALID( psIfacePtr);
#else
  return FALSE;
#endif

} /* NetPlatform::IsDefaultIPv6AddrStateValid() */

int32 NetPlatform::IPv6PrivAddrIncRefCntEx
(
  const ds::INAddr6Type *  ipAddrPtr,
  boolean                  includeExternalAddr,
  ds::ErrorType *          dsErrnoPtr
)
{
#ifdef FEATURE_DATA_PS_IPV6
  struct ps_in6_addr *  privIPv6AddrPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ipAddrPtr || 0 == dsErrnoPtr)
  {
    PS_UTILS_ERROR_FATAL("NetPlatform::IPv6PrivAddrIncRefCnt(): NULL args");
    return -1;
  }

  privIPv6AddrPtr = (struct ps_in6_addr *) ipAddrPtr;
  if (-1 == ps_ip_addr_ipv6_priv_addr_inc_ref_cnt_ex( privIPv6AddrPtr, 
                                                      includeExternalAddr ))
  {
    *dsErrnoPtr = QDS_EINVAL;
    return -1;
  }

  return DSS_SUCCESS;
#else
  *dsErrnoPtr = DS_EOPNOTSUPP;
  return DSS_ERROR;
#endif
} /* NetPlatform::IPv6PrivAddrIncRefCntEx() */


int32 NetPlatform::IPv6PrivAddrIncRefCnt
(
  const ds::INAddr6Type *  ipAddrPtr,
  ds::ErrorType *          dsErrnoPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return IPv6PrivAddrIncRefCntEx(ipAddrPtr, FALSE, dsErrnoPtr);
} /* NetPlatform::IPv6PrivAddrIncRefCnt() */


int32 NetPlatform::IPv6PrivAddrDecRefCnt
(
  const ds::INAddr6Type *  ipAddrPtr,
  ds::ErrorType *          dsErrnoPtr
)
throw()
{
#ifdef FEATURE_DATA_PS_IPV6
  struct ps_in6_addr *  privIPv6AddrPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ipAddrPtr || 0 == dsErrnoPtr)
  {
    PS_UTILS_ERROR_FATAL("NetPlatform::IPv6PrivAddrDecRefCnt(): NULL args");
    return -1;
  }

  privIPv6AddrPtr = (struct ps_in6_addr *) ipAddrPtr;
  if (-1 == ps_ip_addr_ipv6_priv_addr_dec_ref_cnt( privIPv6AddrPtr))
  {
    *dsErrnoPtr = QDS_EINVAL;
    return -1;
  }

  return DSS_SUCCESS;
#else
  *dsErrnoPtr = DS_EOPNOTSUPP;
  return DSS_ERROR;
#endif
} /* NetPlatform::IPv6PrivAddrDecRefCnt() */


int32 NetPlatform::RouteDataPathLookup
(
  ip_pkt_info_type *  ipPktPtr,
  bool                isSystemSocket,
  IPolicy *           policyObjPtr,
  boolean             isAddrBasedRoutingOnly,
  int32 *             ifaceHandlePtr
)
{
  ps_iface_type *                    psIfacePtr;
  int32                              retVal;
  NetPlatform::ACLPolicyInfoType     aclPolicy;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ipPktPtr || 0 == ifaceHandlePtr)
  {
    PS_UTILS_ERROR_FATAL("NetPlatform::RouteDataPathLookup(): NULL args");
    return -1;
  }

  retVal = GenerateAclPolicy (policyObjPtr, &aclPolicy);
  if (AEE_SUCCESS != retVal)
  {
    LOG_MSG_ERROR_PS_INT_1 ("NetPlatform::RouteDataPathLookup(): "
                     "Err getting ACL policy, %d", retVal);
    return retVal;
  }

  aclPolicy.bring_up    = FALSE;
  aclPolicy.lookup_only = FALSE;

  /*-------------------------------------------------------------------------
    Set data path policy flag appropriately
      System socket    - POLICY_NOT_DOWN
      Routeable socket - POLICY_UP_OR_ROUTEABLE
      Regular socket   - POLICY_UP_ONLY
  -------------------------------------------------------------------------*/
  if (isSystemSocket)
  {
    aclPolicy.data_path_policy_flag = PS_ACL_POLICY_NOT_DOWN;
  }
  else if (aclPolicy.is_routeable)
  {
    aclPolicy.data_path_policy_flag = PS_ACL_POLICY_UP_OR_ROUTEABLE;
  }
  else
  {
    aclPolicy.data_path_policy_flag = PS_ACL_POLICY_UP_ONLY;
  }

  retVal = ps_route_data_path_lookup( ipPktPtr,
                                    &aclPolicy,
                                    isAddrBasedRoutingOnly,
                                    &psIfacePtr);

  *ifaceHandlePtr = reinterpret_cast <int32> ( psIfacePtr);
  return retVal;

} /* NetPlatform::RouteDataPathLookup() */


uint32 NetPlatform::IPFltrExecute
(
  int32                                ifaceHandle,
  ps_iface_ipfltr_client_id_enum_type  ipFltrClientID,
  ip_pkt_info_type *                   ipPktInfoPtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ipPktInfoPtr)
  {
    PS_UTILS_ERROR_FATAL("NetPlatform::IPFltrExecute(): NULL args");
    return 0;
  }

  /*-------------------------------------------------------------------------
    Iface handle can be NULL to use global IP filters not associated with
    any particular interface.
  -------------------------------------------------------------------------*/

  return
    ps_iface_ipfltr_execute( reinterpret_cast <ps_iface_type *> ( ifaceHandle),
                             ipFltrClientID,
                             ipPktInfoPtr);

} /* NetPlatform::IPFltrExecute() */


int32 NetPlatform::GetDefaultPSFlowFromRtMetaInfo
(
  ps_rt_meta_info_type *  rtMetaInfoPtr
)
{
  void *  routingCachePtr;
  int32   psIfaceHandle;
  int32   defaultPSFlowHandle = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == rtMetaInfoPtr)
  {
    LOG_MSG_ERROR_PS_INT_0("NetPlatform::GetDefaultPSFlowFromRtMetaInfo(): "
                    "NULL rt meta info");
    return 0;
  }

  routingCachePtr = PS_RT_META_GET_ROUTING_CACHE( rtMetaInfoPtr);
  psIfaceHandle   = reinterpret_cast <int32> ( routingCachePtr);

  (void) PSGetDefaultFlow( psIfaceHandle, &defaultPSFlowHandle);

  return defaultPSFlowHandle;
} /* NetPlatform::GetDefaultPSFlowFromRtMetaInfo() */


int32 NetPlatform::GetPSFlowFromRtMetaInfo
(
  ps_rt_meta_info_type *  rtMetaInfoPtr
)
{
  ps_flow_type *  psFlowPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == rtMetaInfoPtr)
  {
    LOG_MSG_ERROR_PS_INT_0("NetPlatform::GetPSFlowFromRtMetaInfo(): "
                    "NULL rt meta info");
    return 0;
  }

  psFlowPtr = PS_FLOW_GET_FLOW_FROM_RT_META_INFO( rtMetaInfoPtr);
  return reinterpret_cast <int32> ( psFlowPtr);

} /* NetPlatform::GetPSFlowFromRtMetaInfo() */


int32 NetPlatform::GetPhysLinkFromRtMetaInfo
(
  ps_rt_meta_info_type *  rtMetaInfoPtr
)
{
  ps_flow_type *  psFlowPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == rtMetaInfoPtr)
  {
    LOG_MSG_ERROR_PS_INT_0("NetPlatform::GetPhysLinkFromRtMetaInfo(): "
                    "NULL rt meta info");
    return 0;
  }

  psFlowPtr = PS_FLOW_GET_FLOW_FROM_RT_META_INFO( rtMetaInfoPtr);
  return reinterpret_cast <int32> ( PS_FLOW_GET_PHYS_LINK( psFlowPtr));

} /* NetPlatform::GetPhysLinkFromRtMetaInfo() */


uint8 NetPlatform::PSGetIfaceIndex
(
  int32 ifaceHandle
)
{
  return (uint8) PS_IFACE_GET_IFACE_INDEX((ps_iface_type *)ifaceHandle);
}

uint8 NetPlatform::PSGetFlowInstance
(
  int32 ifaceHandle
)
{
  //TODO: How do we find out the flow instance from iface pointer?
  return 0;
}


int32 NetPlatform::PSGetPhysLink
(
  int32     ifaceHandle,
  int32    *physLinkHandle
)
{

  if (NULL == physLinkHandle)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::PSGetPhysLink(): "
                     "NULL args");
    return QDS_EFAULT;
  }

  *physLinkHandle = 0;

  if (0 == ifaceHandle)
  {
    return AEE_EFAILED;
  }

  *physLinkHandle = (int32) PS_IFACE_GET_PHYS_LINK ((ps_iface_type *)ifaceHandle);
  return AEE_SUCCESS;

} /* GetPhysLink() */

int32 NetPlatform::PSGetDefaultFlow
(
  int32  argIfaceHandle,
  int32* defaultFlowPtr
)
{
  ps_iface_type *ifacePtr = (ps_iface_type *) argIfaceHandle;

  if (NULL == defaultFlowPtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::PSGetDefaultFlow(): "
                     "Inv. args");
    return QDS_EFAULT;
  }

  if (0 == argIfaceHandle)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::PSGetDefaultFlow(): "
                     "NULL iface ptr");
    return QDS_EINVAL;
  }

  *defaultFlowPtr = (int32) PS_IFACE_GET_DEFAULT_FLOW(ifacePtr);
  return AEE_SUCCESS;

} /* PSGetDefaultFlow() */

int32 NetPlatform::PSGetPhysLinkFromFlow
(
  int32   flowHandle,
  int32*  physLinkHandle
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == physLinkHandle)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::PSGetPhysLinkFromFlow(): "
                     "NULL args to GetPhysLinkFromFlow");
    goto bail;
  }

  *physLinkHandle = (int32) PS_FLOW_GET_PHYS_LINK((ps_flow_type *)flowHandle);
  if (0 == *physLinkHandle)
  {
    goto bail;
  }

  return AEE_SUCCESS;

bail:
  return QDS_EINVAL;

} /* PSGetPhysLinkFromFlow() */

int32 NetPlatform::PSGetPhysLinkFromIface
(
  int32  ifaceHandle,
  int32* physLinkHandle
)
{
  if (NULL == physLinkHandle)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NetPlatform::PSGetPhysLinkFromIface(): "
                     "NULL args to GetPhysLinkFromIface");
    goto bail;
  }

  if (0 == ifaceHandle)
  {
    *physLinkHandle = 0;
    goto bail;
  }

  *physLinkHandle = (int32) PS_IFACEI_GET_PHYS_LINK((ps_iface_type *)ifaceHandle);
  if (0 == *physLinkHandle)
  {
    goto bail;
  }
  return AEE_SUCCESS;

bail:
  return AEE_EFAILED;

} /* PSGetPhysLinkFromIface() */

int32 NetPlatform::PSGetIfaceId
(
  int32  ifaceHandle,
  int32* ifaceId
)
{
  *ifaceId = PS_IFACE_GET_ID((ps_iface_type *) ifaceHandle);
  return AEE_SUCCESS;
}

int32 NetPlatform::PSIfaceIPFilterAdd
(
  int32                                   ifaceHandle,
  ps_iface_name_enum_type                 ifaceGroup,
  ps_iface_ipfltr_client_id_enum_type     clientID,
  const PSIfaceIPFilterAddParamType     * fltrParams,
  int32                                 * fltrHandle
)
{
  ps_iface_type*        ifacePtr;
  int16                 psErrno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == fltrHandle)
  {
    return QDS_EFAULT;
  }

  ifacePtr = (ps_iface_type *)ifaceHandle;
  if (ifacePtr != NULL && !PS_IFACE_IS_VALID(ifacePtr))
  {
    return QDS_EFAULT;
  }

  if (ifacePtr == NULL)
  {
    *fltrHandle = ps_iface_ipfltr_add_by_group(ifaceGroup, 
                                               clientID, 
                                               fltrParams, 
                                               &psErrno);
  }
  else
  {  
  *fltrHandle = ps_iface_ipfltr_add(ifacePtr, clientID, fltrParams, &psErrno);
  }

  if (PS_IFACE_IPFLTR_INVALID_HANDLE == *fltrHandle)
  {
    return MapErrorCode(psErrno);
  }
  return AEE_SUCCESS;

} /* PSIfaceIPFilterAdd() */

int32 NetPlatform::PSIfaceIPFilterDelete
(
  int32                                  ifaceHandle,
  ps_iface_ipfltr_client_id_enum_type    clientId,
  int32                                  fltrHandle
)
throw()
{
  ps_iface_type*        ifacePtr;
  int16                 psErrno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ifacePtr = (ps_iface_type *)ifaceHandle;

  if (ifacePtr != NULL && !PS_IFACE_IS_VALID(ifacePtr))
  {
    return QDS_EFAULT;
  }

  if (-1 == ps_iface_ipfltr_delete (ifacePtr, clientId, fltrHandle, &psErrno))
  {
    return MapErrorCode (psErrno);
  }

  return AEE_SUCCESS;

} /* PSIfaceIPFilterDelete() */


bool NetPlatform::IsPSIfaceTxEnabled
(
  int32  psIfaceHandle
)
{
  ps_iface_type *  psIfacePtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  psIfacePtr = reinterpret_cast < ps_iface_type *> ( psIfaceHandle);
  if (TRUE == PS_IFACE_FLOW_ENABLED( psIfacePtr))
  {
    return true;
  }

  return false;
} /* NetPlatform::IsPSIfaceTxEnabled() */


bool NetPlatform::IsPSIfaceCommonTxEnabled
(
  int32  psIfaceHandle,
  int32  psFlowHandle
)
{
  ps_iface_type *  psIfacePtr;
  ps_flow_type *   psFlowPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  psIfacePtr = reinterpret_cast < ps_iface_type *> ( psIfaceHandle);
  psFlowPtr  = reinterpret_cast < ps_flow_type *> ( psFlowHandle);

  if (TRUE == PS_IFACE_COMMON_IS_FLOW_ENABLED( psIfacePtr, psFlowPtr))
  {
    return true;
  }

  return false;
} /* NetPlatform::IsPSIfaceCommonTxEnabled() */

bool NetPlatform::IsPSIfaceCommonTxDoSEnabled
(
  int32  psIfaceHandle,
  int32  psFlowHandle
)
{
  ps_iface_type *  psIfacePtr;
  ps_flow_type *   psFlowPtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  psIfacePtr = reinterpret_cast < ps_iface_type *> ( psIfaceHandle);
  psFlowPtr  = reinterpret_cast < ps_flow_type *> ( psFlowHandle);

  if (PS_IFACE_FLOW_ENABLED(psIfacePtr) &&
      PS_FLOW_IS_TX_ENABLED(psFlowPtr))
  {
     return true;
  }

  return false;
} /* NetPlatform::IsPSIfaceCommonTxDoSEnabled() */

int32 NetPlatform::GetQoSStatus
(
  int32             flowHandle,
  QoSStatusType *   pQoSStatus
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == pQoSStatus)
  {
    return QDS_EFAULT;
  }

  switch (PS_FLOWI_GET_STATE((ps_flow_type *)flowHandle))
  {
    case FLOW_ACTIVATING:
    case FLOW_RESUMING:
      *pQoSStatus = QOS_ACTIVATING;
      break;

    case FLOW_CONFIGURING:
      *pQoSStatus = QOS_CONFIGURING;
      break;

    case FLOW_ACTIVATED:
      *pQoSStatus = QOS_AVAILABLE;
      break;

    case FLOW_SUSPENDING:
      *pQoSStatus = QOS_SUSPENDING;
      break;

    case FLOW_SUSPENDED:
      *pQoSStatus = QOS_DEACTIVATED;
      break;

    case FLOW_GOING_NULL:
      *pQoSStatus = QOS_RELEASING;
      break;

    case FLOW_NULL:
    case FLOW_INIT:
      *pQoSStatus = QOS_UNAVAILABLE;
      break;

    default:
      return QDS_EINVAL;
  }

  return AEE_SUCCESS;
} /* GetQoSStatus */

void NetPlatform::GetLastNetworkDownReason
(
  int32                     ifaceHandle,
  NetworkDownReasonType *   pNetDownReason
)
{
  ps_iface_type*  ifacePtr = (ps_iface_type *)ifaceHandle;;

  if (NULL == pNetDownReason)
  {
    PS_UTILS_ASSERT(0);
    return;
  }

  if (NULL == ifacePtr)
  {
    *pNetDownReason = PS_NET_DOWN_REASON_NOT_SPECIFIED;
    return;
  }

  *pNetDownReason = ifacePtr->event_info_cache.iface_down_info.netdown_reason;

} /* GetLastNetworkDownReason() */

IfaceStateEnumType NetPlatform::GetIfaceState
(
  int32 ifaceHandle
)
{
  return ps_iface_state((ps_iface_type *)ifaceHandle);
}


PSFlowStateType NetPlatform::GetPSFlowState
(
  int32  psFlowHandle
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return PS_FLOW_GET_STATE(reinterpret_cast <ps_flow_type *> (psFlowHandle));
} /* NetPlatform::GetPSFlowState() */


ps_iface_type * NetPlatform::PSIfaceGetHandle
(
  int32  ifaceId
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return ps_iface_get_handle( (ps_iface_id_type) ifaceId );
} /* NetPlatform::PSIfaceGetHandle() */


bool NetPlatform::IsPSFlowDelaySensitive
(
  int32  psFlowHandle
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*lint -e{506} */
  if (PS_FLOW_GET_CAPABILITY( (ps_flow_type *) psFlowHandle,
                              PS_FLOW_CAPABILITY_DELAY_SENSITIVE))
  {
    return true;
  }

  return false;
} /* NetPlatform::IsPSFlowDelaySensitive() */


ds::ErrorType NetPlatform::GetRouteScopeByPolicy
(
  IPolicy *              netPolicyPtr,
  ps_route_scope_type *  routeScopePtr
)
{
  ds::ErrorType      dsErrno;
  ACLPolicyInfoType  aclPolicy;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == routeScopePtr)
  {
    LOG_MSG_ERROR_PS_INT_0( "NULL arg");
    return QDS_EFAULT;
  }

  dsErrno = GenerateAclPolicy( netPolicyPtr, &aclPolicy);
  if (AEE_SUCCESS != dsErrno)
  {
    LOG_MSG_ERROR_1( "GenerateAclPolicy failed, err 0x%x", dsErrno);
    return dsErrno;
  }

  aclPolicy.proc_id = PS_IFACE_PROC_ID_ANY;

  *routeScopePtr = ps_route_scope_by_policy( 0, &aclPolicy);

  /* Add V4 and V6 routescope always */
  (void) AddLoopbackToRouteScope (TRUE, routeScopePtr);

  PS_ROUTE_SCOPE_PRINT( routeScopePtr);
  return AEE_SUCCESS;

} /* NetPlatform::GetRouteScopeByPolicy() */


ds::ErrorType NetPlatform::GetRouteScopeByIfaceHandle
(
  int32                  ifaceHandle,
  bool                   addV6LoopbackIface,
  ps_route_scope_type *  routeScopePtr
)
{
  ps_iface_type *  psIfacePtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == routeScopePtr)
  {
    LOG_MSG_ERROR_PS_INT_0( "NULL arg");
    return QDS_EFAULT;
  }

  if (0 == ifaceHandle)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NULL iface handle");
    return QDS_EINVAL;
  }

  /*-------------------------------------------------------------------------
    Fetch ps iface corresponding to ifaceHandle and update route scope
  -------------------------------------------------------------------------*/
  psIfacePtr = reinterpret_cast <ps_iface_type *> ( ifaceHandle);
  if (FALSE == ps_route_scope_by_iface_handle( psIfacePtr, routeScopePtr))
  {
    LOG_MSG_ERROR_PS_INT_0( "ps_route_scope_by_iface_handle failed");
  }

  /* Add V4 and V6 routescope always */
  (void) AddLoopbackToRouteScope( TRUE, routeScopePtr);

  PS_ROUTE_SCOPE_PRINT( routeScopePtr);
  return AEE_SUCCESS;

} /* NetPlatform::GetRouteScopeByIfaceHandle() */


ds::ErrorType NetPlatform::GetRouteScopeByIfaceId
(
  int32                  ifaceId,
  bool                   addV6LoopbackIface,
  ps_route_scope_type *  routeScopePtr
)
{
  ps_iface_type *  psIfacePtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == routeScopePtr)
  {
    LOG_MSG_ERROR_PS_INT_0( "NULL arg");
    return QDS_EFAULT;
  }

  /*-------------------------------------------------------------------------
    Fetch ps iface corresponding to ifaceId and update route scope
  -------------------------------------------------------------------------*/
  psIfacePtr = ps_iface_get_handle( (ps_iface_id_type) ifaceId );

  if (NULL == psIfacePtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NULL iface handle");
    return QDS_EINVAL;
  }

  if (FALSE == ps_route_scope_by_iface_handle( psIfacePtr, routeScopePtr))
  {
    LOG_MSG_ERROR_PS_INT_0( "ps_route_scope_by_iface_handle failed");
  }

  /* Add V4 and V6 routescope always */
  (void) AddLoopbackToRouteScope( TRUE, routeScopePtr);

  PS_ROUTE_SCOPE_PRINT( routeScopePtr);
  return AEE_SUCCESS;

} /* NetPlatform::GetRouteScopeByIfaceId() */


ds::ErrorType NetPlatform::AddLoopbackToRouteScope
(
  bool                   addV6LoopbackIface,
  ps_route_scope_type *  routeScopePtr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == routeScopePtr)
  {
    LOG_MSG_ERROR_PS_INT_0( "NULL arg");
    return QDS_EFAULT;
  }

  if (addV6LoopbackIface)
  {
    ps_route_scope_add_lo( routeScopePtr, TRUE);
  }
  else
  {
    ps_route_scope_add_lo( routeScopePtr, FALSE);
  }

  return AEE_SUCCESS;

} /* NetPlatform::AddLoopbackToRouteScope() */


ds::ErrorType NetPlatform::DeleteIfaceHandleFromRouteScope
(
  ps_route_scope_type *  routeScopePtr,
  int32                  ifaceHandle
)
{
  ps_iface_type *  psIfacePtr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == routeScopePtr)
  {
    LOG_MSG_ERROR_PS_INT_0( "NULL arg");
    return QDS_EFAULT;
  }

  psIfacePtr = reinterpret_cast <ps_iface_type *> ( ifaceHandle);

  if (NULL == psIfacePtr)
  {
    LOG_MSG_ERROR_PS_INT_0 ("NULL iface handle");
    return QDS_EINVAL;
  }

  ps_route_scope_delete( routeScopePtr, PS_IFACE_GET_IFACE_INDEX( psIfacePtr));

  PS_ROUTE_SCOPE_PRINT( routeScopePtr);
  return AEE_SUCCESS;

} /* NetPlatform::DeleteIfaceHandleFromRouteScope() */

boolean NetPlatform::RouteScopeIntersect
(
  ps_route_scope_type *  pRouteScope1,
  ps_route_scope_type *  pRouteScope2
)
{
  ps_route_scope_type    tempRouteScope;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (NULL == pRouteScope1 || NULL == pRouteScope2)
  {
    return TRUE;
  }

  tempRouteScope = ps_route_scope_intersect(*pRouteScope1, *pRouteScope2);

  ps_route_scope_delete (&tempRouteScope, ps_get_v6_lo_rscope_idx());
  ps_route_scope_delete (&tempRouteScope, ps_get_v4_lo_rscope_idx());

  return !(ps_is_route_scope_empty (tempRouteScope));

} /* NetPlatform::RouteScopeIntersect() */

int32 NetPlatform::GetIfaceStats
(
  int32                          ifaceHandle,
  ds::Net::NetworkStatsType      *stats
)
{
  ds::ErrorType           result  = AEE_SUCCESS;
  ps_stat_iface_i_s_type  psStats = {0};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == stats)
  {
    LOG_MSG_ERROR_PS_INT_0("NetPlatform::GetIfaceStats(): "
                    "NULL arg");
    result = QDS_EFAULT;
    goto bail;
  }
  if(E_SUCCESS != ps_stat_get_iface(
                                     PS_STAT_IFACE_ALL,
                                     (void*)ifaceHandle,
                                     (void*)&psStats,
                                     sizeof(ps_stat_iface_i_s_type)
                                   )
    )
  {
    result = AEE_EFAILED;
    goto bail;
  }
  /*-----------------------------------------------------------------------
    convert to output parameter
  -----------------------------------------------------------------------*/
  stats->bytesRX = psStats.bytes_rx;
  stats->bytesTX = psStats.bytes_tx;
  stats->mcastPktsRX = psStats.mcast_pkts_rx;
  stats->mcastPktsTX = psStats.mcast_pkts_tx;
  stats->pktsDroppedRX = psStats.pkts_dropped_rx;
  stats->pktsDroppedTX = psStats.pkts_dropped_tx;
  stats->pktsRX = psStats.pkts_rx;
  stats->pktsTX = psStats.pkts_tx;
  stats->state = psStats.state;
  result = AEE_SUCCESS;

/* fall through */

bail:

  return result;

} /* NetPlatform::GetIfaceStats() */

int32 NetPlatform::ResetIfaceStats
(
  int32                          ifaceHandle
)
{
  ds::ErrorType result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  (void) ps_stat_reset_iface_stats_by_iface_ptr((void*)ifaceHandle);

  return result;

} /* NetPlatform::GetIfaceStats() */

void * NetPlatform::EnableNat
(
  IPolicy       *pIPolicy,
  ds::ErrorType *errnoPtr
)
{
  return NULL;
}  /* NetPlatform::EnableNat() */

ds::ErrorType NetPlatform::DisableNat
(
  void   *natHandle
)
{
return QDS_EOPNOTSUPP;
} /* NetPlatform::DisableNat() */

ds::ErrorType NetPlatform::EnableRoamingAutoconnect
(
  void
)
{
return QDS_EOPNOTSUPP;
} /* NetPlatform::EnableRoamingAutoconnect() */

ds::ErrorType NetPlatform::DisableRoamingAutoconnect
(
  void
)
{
return QDS_EOPNOTSUPP;
} /* NetPlatform::DisableRoamingAutoconnect */

bool NetPlatform::IsPSFlowDefault
(
  int32  psFlowHandle
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*lint -e{506} */
  if (PS_FLOW_GET_CAPABILITY( (ps_flow_type *) psFlowHandle,
                               PS_FLOW_CAPABILITY_DEFAULT))
  {
    return true;
  }

  return false;
} /* NetPlatform::IsPSFlowDefault() */

int32 NetPlatform::PSSysIoctl
(
  PSSysTechEnumType              techType,
  PSSysIoctlEnumType             ioctlName,
  PSSysSubEnumType               subID,
  void                          *argValPtr,
  int16                         *psErrnoPtr
) throw()
{

  int32            result;
  int16            psErrno;
  /*-------------------------------------------------------------------------*/

  result  = ps_sys_ioctl_ex((ps_sys_tech_enum_type)techType,
                            (ps_sys_ioctl_enum_type)ioctlName,
                            (ps_sys_subscription_enum_type)subID,
                            argValPtr,
                            &psErrno);
  
  if (0 == result)
  {
    LOG_MSG_ERROR_2 ("NetPlatform::PSSysIoctl(): "
                     "ps iface ioctl returned -1, err %d ioctlName %d", psErrno, ioctlName);
    *psErrnoPtr = MapErrorCode(psErrno);
  }

  return result;
}/* NetPlatform::PSSysIoctl */


PSSysSubEnumType NetPlatform::IfaceGetSubsID
(
  int32                              ifaceHandle 
)
{
  ps_iface_type*   ifacePtr;
  uint32           subs_id;
  /*-------------------------------------------------------------------------*/

  ifacePtr = (ps_iface_type *) ifaceHandle;

  subs_id = PS_IFACE_GET_SUBS_ID(ifacePtr);

  LOG_MSG_INFO2_2 ("NetPlatform::IfaceIoctl(): "
                   "IfaceIoctl() iface handle 0x%p, ioctl name %d",
                   ifaceHandle, subs_id);

  return (PSSysSubEnumType)subs_id;
}/*NetPlatform::IfaceGetSubsID*/

