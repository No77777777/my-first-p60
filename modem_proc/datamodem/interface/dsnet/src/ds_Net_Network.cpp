/*===========================================================================
  FILE: Network.cpp

  OVERVIEW: This file provides implementation of the Network class.

  DEPENDENCIES: None

  Copyright (c) 2008-2020 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

 $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_Network.cpp#5 $
  $DateTime: 2021/05/25 01:37:25 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2015-21-05 pg  Added IOCTL support for PDN detach.
  2013-11-01 rk  Removed feature mobileap.
  2011-09-30 kr  Free floating changes
  2011-08-31 hs  Added GetSubnetMask
  2011-08-30 hs  Added IOCTL GetGatewayAddress.
  2011-08-24 hs  Added IOCTL GetDeviceName
  2010-06-02 cp  IOCTL changes for Soft AP DHCP server.
  2009-02-27 hm  Added IPv6 priv addr and prefixes support.
  2008-03-10 hm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include <stringl/stringl.h>
extern "C"
{
#include "ps_logging_helper.h"
}
#include <string.h>
#include "ds_Utils_StdErr.h"
#include "ds_Net_INetwork1x.h"
#include "ds_Net_INetwork1xPriv.h"
#include "ds_Net_INetworkUMTS.h"
#include "ds_Net_IMCastMBMSCtrlPriv.h"
#include "ds_Net_INetworkControl.h"
#include "ds_Net_IFirewallManager.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_Conversion.h"
#include "ds_Net_Network.h"
#include "ds_Net_Platform.h"
#include "ds_Net_BearerTech.h"
#include "ds_Net_BearerTechEx.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_EventManager.h"
#include "ds_Net_Network1X.h"
#include "ds_Net_NetworkUMTS.h"
#include "ds_Net_MCastMBMSCtrl.h"
#include "ds_Net_NetworkIPv6.h"
#include "ds_Net_NetworkFactory.h"
#include "ds_Net_QoSDefault.h"
#include "ds_Net_QoSSecondary.h"
#include "ds_Net_MCastSession.h"
#include "ds_Net_IPFilterSpec.h"
#include "ds_Net_FirewallManager.h"
#include "ds_Net_Conversion.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CSignalBus.h"
#include "ps_system_heap.h"
#include "ds_Utils_SockAddrInternalTypes.h"
#include "ps_policy_mgr.h"
#include "ds_sys.h"
#include "dss_errors_def.h"
#include "ds_Net_RanAsstMediaAdaptReq.h"
#include "ps_utils.h"

using namespace ds::Net;
using namespace ds::Net::Conversion;
using namespace ds::Error;
using namespace NetPlatform;
using namespace dss::Error;

#define DS_NET_NETWORK_ASSERT(expression) \
         dsnet_network_assert_wrapper(__LINE__, expression)

/*===========================================================================

FUNCTION dsnet_init_assert_wrapper()

DESCRIPTION
  Wrapper function for DS_NET_INIT_ASSERT
 
DEPENDENCIES 
  None

RETURN VALUE
  None
 
SIDE EFFECTS

===========================================================================*/
static void dsnet_network_assert_wrapper
(
  unsigned int     line_num,
  int              expression
)
{
  if ( !expression )
  {
    ERR_FATAL("DSNET_FATAL at line:%d ",  
                                   line_num,0,0);  
  }
}/* dsnet_init_assert_wrapper */


/*===========================================================================

                     PRIVATE FUNCTION DEFINITIONS

===========================================================================*/
ds::ErrorType Network::IPAddrIOCTL
(
  IfaceIoctlEnumType   ioctlName,
  ds::IPAddrType*      pDSIPAddr
)
{
  ds::ErrorType        result;
  PSIPAddrType         psIPAddr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Validation.
  -------------------------------------------------------------------------*/
  if (0 == pDSIPAddr)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Get the iface handle and call the relevant IOCTL
    -----------------------------------------------------------------------*/
    memset (&psIPAddr, 0, sizeof(PSIPAddrType));

    result = IfaceIoctl (GetHandle(),
                         ioctlName,
                         static_cast <void *> (&psIPAddr));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert IOCTL result back to ds::IPAddrType
    -----------------------------------------------------------------------*/
    result = PS2DSIPAddr (&psIPAddr, pDSIPAddr);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_3 ("Network::IPAddrIOCTL(): Result 0x%x, IOCTL %d, "
                           "obj 0x%p", result, ioctlName, this);

  mpICritSect->Leave();
  return result;

} /* Network::IPAddrIOCTL() */

/*===========================================================================

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
Network::Network
(
  Policy *          argPolicyPtr,
  NetworkModeType   networkMode
) : Handle(),
    mNetworkMode(networkMode),
    mFlags(0),
    mLastNetDownReason(0),
    mpPolicy (argPolicyPtr),
    mpIPFilterReg (NULL),
    mWouldblock(FALSE),
    mBringupAgain (FALSE),
    mBringupFirst (FALSE),
    mTeardown (FALSE),
    mOutageEventOccurred(FALSE),
    mFastDormStatus(0),

    // state change
    mpSigBusIfaceUp(0),
    mpSigBusIfaceGoingDown(0),
    mpSigBusIfaceDown(0),
    mpSigBusIfaceActiveOutOfUse(0),
    mpSigBusIfaceComingUp(0),
    mpSigBusIfaceConfiguring(0),
    mpSigBusIfaceRouteable(0),

    mpTechObjNetwork1x (NULL),
    mpTechObjNetwork1xPriv (NULL),
    mpNetworkUMTS (NULL),
    mpNetworkIPv6 (NULL),
    mCachedPreviousState(NetworkState::QDS_CLOSED),
    mAppPriority(PS_POLICY_MGR_LEGACY_PRIORITY),
    refCnt(1),
    weakRefCnt(1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("Network::Network(): Obj 0x%p, mode %d, argPolicyPtr 0x%x",
                            this, networkMode, argPolicyPtr);

  //TODO Move this to Init function
  if (0 == argPolicyPtr)
  {
    DS_NET_NETWORK_ASSERT (0);
    return;
  }

  (void) mpPolicy->AddRef();

  mLastNetDownReason = NetDownReason::QDS_NOT_SPECIFIED;

} /* Network::Network() */

ds::ErrorType Network::Init
(
  void
)
{
  ds::ErrorType  result;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    /*-----------------------------------------------------------------------
      Create signal buses to hold event registration info
    -----------------------------------------------------------------------*/
    result = DS_Utils_CreateInstance(0,
                                     AEECLSID_CSignalBus,
                                     (void **) &mpSigBusIfaceUp);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_MP_SIG_BUS_IFACE_UP_FAILED;
      break;
    }

    result = DS_Utils_CreateInstance(0,
                                     AEECLSID_CSignalBus,
                                     (void **) &mpSigBusIfaceGoingDown);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_MP_SIG_BUS_IFACE_GOING_DOWN_FAILED;
      break;
    }

    result = DS_Utils_CreateInstance(0,
                                     AEECLSID_CSignalBus,
                                     (void **) &mpSigBusIfaceDown);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_MP_SIG_BUS_IFACE_DOWN_FAILED;
      break;
    }

    result = DS_Utils_CreateInstance(0,
                                     AEECLSID_CSignalBus,
                                     (void **) &mpSigBusIfaceActiveOutOfUse);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_MP_SIG_BUS_IFACE_ACTIVE_OUT_OF_USE_FAILED;
      break;
    }

    result = DS_Utils_CreateInstance(0,
                                     AEECLSID_CSignalBus,
                                     (void **) &mpSigBusIfaceComingUp);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_MP_SIG_BUS_IFACE_COMING_UP_FAILED;
      break;
    }

    result = DS_Utils_CreateInstance(0,
                                     AEECLSID_CSignalBus,
                                     (void **) &mpSigBusIfaceConfiguring);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_MP_SIG_BUS_IFACE_CONFIGURING_FAILED;
      break;
    }

    result = DS_Utils_CreateInstance(0,
                                     AEECLSID_CSignalBus,
                                     (void **) &mpSigBusIfaceRouteable);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_MP_SIG_BUS_IFACE_ROUTEABLE_FAILED;
      break;
    }


    result = Handle::Init(EventManager::networkObjList);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_INIT_FAILED;
      break;
    }

    result = AEE_SUCCESS;

  } while (0);

  /*-------------------------------------------------------------------------
    Cleanup in case of error (::Destructor won't do this in case of error)
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS != result)
  {
    LOG_MSG_ERROR_2("Network::Init() :  "
                    "Obj 0x%p failed, error code %d",
                    this, ds_net_error_code);
    
    DS_UTILS_RELEASEIF (mpPolicy);

    // state change
    DS_UTILS_RELEASEIF (mpSigBusIfaceUp);
    DS_UTILS_RELEASEIF (mpSigBusIfaceGoingDown);
    DS_UTILS_RELEASEIF (mpSigBusIfaceDown);
    DS_UTILS_RELEASEIF (mpSigBusIfaceActiveOutOfUse);
    DS_UTILS_RELEASEIF (mpSigBusIfaceComingUp);
    DS_UTILS_RELEASEIF (mpSigBusIfaceConfiguring);
    DS_UTILS_RELEASEIF (mpSigBusIfaceRouteable);
  }

  return result;

} /* Network::Init() */

void Network::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Due to 2 stage constructor pattern - Network::Init can fail.
    No need to free resources (including Handle::Destructor call)
    in this certain case as it is done in Network::Init
  -------------------------------------------------------------------------*/
  if (0 == mpICritSect)
  {
    LOG_MSG_ERROR_1("Network::Destructor(): Obj 0x%p "
                    "Network wasn't fully initialized", this);
    return;
  }

  mpICritSect->Enter();

  /*lint -save -e1550, -e1551 */

  /*-------------------------------------------------------------------------
    Release underlying PS_IFACE, if any is associated. Base class
    destructor is invoked after derived class is destroyed. So we can be
    confident that mpICritSect (maintained by Handle base object) would be
    valid during the course of destructor.
  -------------------------------------------------------------------------*/
  (void) Stop();

  if (0 != mpTechObjNetwork1x)
  {
    DS_UTILS_RELEASEIF (mpTechObjNetwork1x);

    mpTechObjNetwork1xPriv = 0;
  }
  /*lint -restore */

  DS_UTILS_RELEASEIF (mpPolicy);
  DS_UTILS_RELEASEIF (mpNetworkUMTS);
  DS_UTILS_RELEASEIF (mpNetworkIPv6);
  DS_UTILS_RELEASEIF (mpIPFilterReg);

  // state change
  DS_UTILS_RELEASEIF (mpSigBusIfaceUp);
  DS_UTILS_RELEASEIF (mpSigBusIfaceGoingDown);
  DS_UTILS_RELEASEIF (mpSigBusIfaceDown);
  DS_UTILS_RELEASEIF (mpSigBusIfaceActiveOutOfUse);
  DS_UTILS_RELEASEIF (mpSigBusIfaceComingUp);
  DS_UTILS_RELEASEIF (mpSigBusIfaceConfiguring);
  DS_UTILS_RELEASEIF (mpSigBusIfaceRouteable);

  /*-------------------------------------------------------------------------
    Should be the last statement. Call destructor of the base class.
  -------------------------------------------------------------------------*/
  Handle::Destructor();

  mpICritSect->Leave();

} /* Network::Destructor() */

Network::~Network
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    NO-OP: used only for freeing memory
  -------------------------------------------------------------------------*/

} /* Network::~Network() */

ds::ErrorType Network::CreateQoSManager
(
  IQoSManager **  ppIQoSMgr
)
{
  ds::ErrorType  result;
  bool           allocFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppIQoSMgr)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Create a new QoSManager object all the time. If the network object's
      handle changes anytime, old QoSManager does not make sense.
    -----------------------------------------------------------------------*/
    *ppIQoSMgr = static_cast <IQoSManager *> (new QoSManager (GetHandle(),
                                                              mNetworkMode));
    if (0 == *ppIQoSMgr)
    {
      allocFailed = true;
      result = AEE_ENOMEMORY;
      break;
    }

    result = AEE_SUCCESS;
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::CreateQoSManager(): "
                           "Result 0x%x, QoSManager 0x%p, error code %d, "
                           "obj 0x%p", result, *ppIQoSMgr, allocFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::CreateQoSManager() */
#ifdef FEATUTE_DATA_PS_MCAST
ds::ErrorType Network::CreateMCastManager
(
  IMCastManager **  ppIMCastMgr
)
{
  ds::ErrorType  result;
  bool           allocFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppIMCastMgr)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Create a new MCastManager object all the time. If the network object's
      handle changes anytime, old MCastManager does not make sense.
    -----------------------------------------------------------------------*/
    *ppIMCastMgr =
      static_cast<IMCastManager *> (new MCastManager (GetHandle()
                                                      mNetworkMode));
    if (0 == *ppIMCastMgr)
    {
      allocFailed = true;
      result = AEE_ENOMEMORY;
      break;
    }

    result = AEE_SUCCESS;
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::CreateMCastManager(): "
                           "Result 0x%x, MCastManager 0x%p, error code %d, "
                           "obj 0x%p", result, *ppIMCastMgr, allocFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::CreateMCastManager() */
#endif // FEATUTE_DATA_PS_MCAST
ds::ErrorType Network::LookupInterface
(
  void
)
{
  ds::ErrorType  result;
  int32          ifaceHandle = 0;
  bool           ifaceAlreadyUp = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  ifaceHandle = GetHandle();

  do
  {
    /*-----------------------------------------------------------------------
      If the interface is already brought up/looked up, return SUCCESS
    -----------------------------------------------------------------------*/
    if (0 != ifaceHandle)
    {
      ifaceAlreadyUp = true;
      result = AEE_SUCCESS;
      break;
    }

    /*-----------------------------------------------------------------------
      Perform routing lookup and bring up the network by policy.
    -----------------------------------------------------------------------*/
    result =
      IfaceLookUpByPolicy (static_cast <IPolicy *> (mpPolicy), &ifaceHandle);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Set the interface obtained as the handle in Handle object.
    -----------------------------------------------------------------------*/
    SetHandle(ifaceHandle);
  } while (0);

  if(AEE_SUCCESS == result)
  {
    LOG_MSG_QTRACE_HFT_MED(PS_TAG_DSNET, PS_SYS_DEFAULT_SUBS,
                           "Network::LookupInterface(): Result 0x%x, iface handle 0x%x, iface was already"
                           "brought up %d, obj 0x%x", result, ifaceHandle, ifaceAlreadyUp, this);
  }
  else
  {
    LOG_MSG_FUNCTION_EXIT_2 ("Network::LookupInterface(): "
                             "Wasn't able to get valid handle from PS"
                             "Result 0x%x, obj 0x%p", result, this);
  }
  
  mpICritSect->Leave();
  return result;

} /* Network::LookupInterface() */

ds::ErrorType Network::ForceLookupInterfaceByIfaceId
(
  ds::Net::IfaceIdType ifaceId
)
{
  Policy             tmpPolicy;
  ds::ErrorType      result;
  int32              ifaceHandle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    If the interface is already brought up/looked up, return SUCCESS
  -------------------------------------------------------------------------*/
  tmpPolicy.SetIfaceId(ifaceId);

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Perform routing lookup and bring up the network by policy.
    -----------------------------------------------------------------------*/
    result =
      IfaceLookUpByPolicy (static_cast <IPolicy*> (&tmpPolicy), &ifaceHandle);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Set the interface obtained as the handle in Handle object.
    -----------------------------------------------------------------------*/
    SetHandle(ifaceHandle);
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::ForceLookupInterfaceByIfaceId(): "
                           "Result 0x%x, ifaceId 0x%x, ifacehandle 0x%x, obj 0x%p",
                           result, ifaceId, ifaceHandle, this);
  mpICritSect->Leave();
  return result;

} /* Network::ForceLookupInterfaceByIfaceId() */

ds::ErrorType Network::GetStaleIfaceId
(
  ds::Net::IfaceIdType *  pIfaceId
)
{
  ds::ErrorType  result  = AEE_SUCCESS;
  int32          ifaceId = INVALID_IFACE_ID;
  bool           getIfaceIdFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == pIfaceId)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Transform stale iface handle to iface ID
    -----------------------------------------------------------------------*/
    result = PSGetIfaceId (mStaleIfaceHandle, &ifaceId);
    if (AEE_SUCCESS != result)
    {
      getIfaceIdFailed = true;
      break;
    }

    *pIfaceId = static_cast <ds::Net::IfaceIdType> (ifaceId);
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetStaleIfaceId(): "
                           "Result 0x%x, stale iface id 0x%x, error getting "
                           "iface id %d, obj 0x%p",
                           result, *pIfaceId, getIfaceIdFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetStaleIfaceId() */


ds::ErrorType Network::GetPolicy
(
  IPolicy **  ppIDSNetPolicy
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2 ("Network::GetPolicy(): Obj 0x%p, ppIDSNetPolicy 0x%x",
                             this, ppIDSNetPolicy);

  if (0 == ppIDSNetPolicy)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  *ppIDSNetPolicy = static_cast <IPolicy *> (mpPolicy);
  (void) mpPolicy->AddRef();

  mpICritSect->Leave();
  return AEE_SUCCESS;

} /* Network::GetPolicy() */

ds::ErrorType Network::SetPolicy
(
  IPolicy *  pIPolicy
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2 ("Network::SetPolicy(): Obj 0x%p, pIPolicy 0x%x",
                             this, pIPolicy);

  if (0 == pIPolicy)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  if (0 != mpPolicy)
  {
    (void) mpPolicy->Release();
  }

  mpPolicy = reinterpret_cast <Policy *> (pIPolicy);
  (void) mpPolicy->AddRef();

  mpICritSect->Leave();
  return AEE_SUCCESS;

} /* Network::SetPolicy() */

/*---------------------------------------------------------------------------
  Functions inherited from INetwork
---------------------------------------------------------------------------*/
ds::ErrorType Network::GetIfaceState
(
  NetworkStateType *  argNetState
)
{
  ds::ErrorType          result;
  int32                  ifaceHandle;
  IfaceStateEnumType     ifaceState;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == argNetState)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    ifaceHandle = GetHandle();
    if (0 == ifaceHandle)
    {
      *argNetState = NetworkState::QDS_CLOSED;
      result = AEE_SUCCESS;
      break;
    }

    /*-----------------------------------------------------------------------
      Call network platform IOCTL to get the interface state.
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (ifaceHandle, IFACE_IOCTL_GET_STATE, &ifaceState);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Map Iface state to network state.
    -----------------------------------------------------------------------*/
    result = PS2DSIfaceState(ifaceState, argNetState);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetIfaceState(): "
                           "Result 0x%x, netstatus %d, iface handle %d obj 0x%p",
                           result, *argNetState, ifaceHandle, this);
  mpICritSect->Leave();
  return result;

} /* Network::GetIfaceState() */

ds::ErrorType Network::GetLastNetDownReason
(
  NetDownReasonType *  argLastNetDownReason
)
{
  int32                              ifaceHandle;
  NetPlatform::NetworkDownReasonType lastNetDownReason;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == argLastNetDownReason)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  /*-----------------------------------------------------------------------
    In case the underlying interface is in DOWN state, but DOWN_EV havn't
    reached the ds_Net yet, take the net down reason from iface itself,
    otherwise 0x00 reason would be propagated to DSS / application
  -----------------------------------------------------------------------*/
  ifaceHandle = GetHandle();

  if(0 == ifaceHandle)
  {
    *argLastNetDownReason = mLastNetDownReason;
  }
  else
  {
    NetPlatform::GetLastNetworkDownReason (ifaceHandle, &lastNetDownReason);
    *argLastNetDownReason = (NetDownReasonType) lastNetDownReason;
  }

  LOG_MSG_FUNCTION_EXIT_2 ("Network::GetLastNetDownReason(): Reason %d, obj 0x%p",
                           mLastNetDownReason, this);
  mpICritSect->Leave();
  return AEE_SUCCESS;

} /* Network::GetLastNetDownReason() */


ds::ErrorType Network::GetIPAddr
(
  ds::IPAddrType *  argIPAddress
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return IPAddrIOCTL (IFACE_IOCTL_GET_IP_ADDR, argIPAddress);

} /* GetIPAddr() */

ds::ErrorType Network::GetNatPublicIPAddress
(
  ds::IPAddrType *  argIPAddress
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return IPAddrIOCTL (IFACE_IOCTL_GET_NAT_PUBLIC_IP_ADDR, argIPAddress);

} /* GetIPAddress() */

ds::ErrorType Network::DhcpArpCacheUpdate
(
  const ds::Net::DhcpArpCacheUpdateType *  dhcpArpCacheUpdateParam
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_DHCP_ARP_CACHE_UPDATE,
                       (void *) dhcpArpCacheUpdateParam);

  mpICritSect->Leave();
  return result;

} /* Network::DhcpArpCacheUpdate() */

ds::ErrorType Network::DhcpArpCacheClear
(
  const ds::Net::DhcpArpCacheClearType *  dhcpArpCacheClearParam
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_DHCP_ARP_CACHE_CLEAR,
                       (void *) dhcpArpCacheClearParam);

  mpICritSect->Leave();
  return result;

} /* Network::DhcpArpCacheClear() */

ds::ErrorType Network::GetDhcpDeviceInfo
(
  ds::Net::DhcpGetDeviceInfoType *  connDevInfo
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == connDevInfo)
  {
    return QDS_EFAULT;
  }

  if (0 == connDevInfo->dev_info || 0 == connDevInfo->dev_infoLen ||
      0 != connDevInfo->dev_infoLenReq)
  {
    return QDS_EINVAL;
  }

  LOG_MSG_INVALID_INPUT_5 ("Network::GetDhcpDeviceInfo(): Invalid arg, "
                           "devInfo ptr 0x%p, info 0x%p, len %d, lenReq %d, "
                           "obj 0x%p",
                           connDevInfo,
                           connDevInfo->dev_info,
                           connDevInfo->dev_infoLen,
                           connDevInfo->dev_infoLenReq,
                           this);

  mpICritSect->Enter();

  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_DHCP_SERVER_GET_DEVICE_INFO,
                       static_cast <void *> (connDevInfo));

  mpICritSect->Leave();
  return result;

} /* Network::GetDhcpDeviceInfo() */

//TODO Aren't NULL checks reqd for pSipDomainNames, pSipDomainNamesLenReq
//Also check if all info is printed. For example, how many domain names are
//requested by user and how many are returned by dsNet
ds::ErrorType Network::GetSIPServerDomainNames
(
  DomainName *  pSipDomainNames,
  int           sipServerDomainNamesLen,
  int *         pSipDomainNamesLenReq
)
{
  SipServerDomainNameInfoType   psSipDomainNames;
  ds::ErrorType                 result;
  int                           index;
  int                           seqNumItems = 0;
  bool                          failedToConvert = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Prepare the IOCTL arguments to get the SIP server domain names.
  -------------------------------------------------------------------------*/
  memset (&psSipDomainNames, 0, sizeof (psSipDomainNames));

  psSipDomainNames.count = PS_IFACE_MAX_SIP_SERVER_DOMAIN_NAMES;

  PS_SYSTEM_HEAP_MEM_ALLOC(
            psSipDomainNames.name_array,
            sizeof (DomainNameType) * PS_IFACE_MAX_SIP_SERVER_DOMAIN_NAMES,
            dss_iface_ioctl_domain_name_type*);

  if (0 == psSipDomainNames.name_array)
  {
    return AEE_ENOMEMORY;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Call the iface IOCTL to get SIP server domain names
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_SIP_SERV_DOMAIN_NAMES,
                         static_cast <void *> (&psSipDomainNames));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    result = ds::Utils::Conversion::ProcessLenReq (sipServerDomainNamesLen,
                                                   pSipDomainNamesLenReq,
                                                   psSipDomainNames.count,
                                                   &seqNumItems);
    if (AEE_SUCCESS != result)
    {
      failedToConvert = true;
      break;
    }

    /*-----------------------------------------------------------------------
      Convert the individual SIP domain names into out argument
    -----------------------------------------------------------------------*/
    for (index = 0; index < seqNumItems; index++)
    {
      memscpy(pSipDomainNames[index],
        sizeof(DomainName),
        psSipDomainNames.name_array[index].domain_name,
        sizeof(DomainName));
    }
  } while (0);

  PS_SYSTEM_HEAP_MEM_FREE(psSipDomainNames.name_array);

  LOG_MSG_FUNCTION_EXIT_3 ("Network::GetSIPServerDomainNames(): "
                           "Result 0x%x, failed to convert error %d, obj 0x%p",
                           result, failedToConvert, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetSIPServerDomainNames() */

//TODO Aren't NULL checks reqd for pSipDomainNames, pSipDomainNamesLenReq
//Also check if all info is printed. For example, how many domain names are
//requested by user and how many are returned by dsNet
ds::ErrorType Network::GetSIPServerAddr
(
  ds::IPAddrType *  pSipServerAddr,
  int               sipServerAddrLen,
  int *             pSipServerAddrLenReq
)
{
  SipServerAddrInfoType sipServerAddr;
  ds::ErrorType         result;
  ds::ErrorType         copyResult  = AEE_SUCCESS;
  int32                 index;
  int                   seqNumItems = 0;
  int                   numCopied   = 0;
  bool                  failedToConvert = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset (&sipServerAddr, 0, sizeof (sipServerAddr));

  sipServerAddr.count = PS_IFACE_MAX_SIP_SERVER_ADDRESSES;

  PS_SYSTEM_HEAP_MEM_ALLOC(
    sipServerAddr.addr_array,
    (sizeof(ip_addr_type) * PS_IFACE_MAX_SIP_SERVER_ADDRESSES),
    ip_addr_type*);

  if (0 == sipServerAddr.addr_array)
  {
    return AEE_ENOMEMORY;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Call the iface IOCTL to get SIP server addresses
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_SIP_SERV_ADDR,
                         static_cast <void *> (&sipServerAddr));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert results to ds format
    -----------------------------------------------------------------------*/
    result = ds::Utils::Conversion::ProcessLenReq( sipServerAddrLen,
                                                   pSipServerAddrLenReq,
                                                   sipServerAddr.count,
                                                   &seqNumItems);
    if (AEE_SUCCESS != result)
    {
      failedToConvert = true;
      break;
    }

    for (index = 0; index < seqNumItems; index++)
    {
      copyResult = PS2DSIPAddr (&(sipServerAddr.addr_array[index]),
                                &pSipServerAddr[numCopied]);
      if (AEE_SUCCESS != copyResult)
      {
        /*-------------------------------------------------------------------
          Do not copy invalid address
        -------------------------------------------------------------------*/
        LOG_MSG_INFO1_2( "Network::GetSIPServerAddr(): Not copying addr "
                         "at index %d, obj 0x%p", index, this);
        continue;
      }

      numCopied++;
    }

    *pSipServerAddrLenReq = numCopied;
  } while (0);

  PS_SYSTEM_HEAP_MEM_FREE(sipServerAddr.addr_array);

  LOG_MSG_FUNCTION_EXIT_4( "Network::GetSIPServerAddr(): "
                           "Result 0x%x, returning %d items, failed to convert"
                           "error %d, obj 0x%p", result, *pSipServerAddrLenReq,
                           failedToConvert, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetSIPServerAddr() */

ds::ErrorType Network::GetQosAware
(
  boolean *  pGetQosAware
)
{
  ds::ErrorType  result;
  int32          ifaceHandle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    HACK to make event propagation work even after call goes down
  -------------------------------------------------------------------------*/
  ifaceHandle = GetHandle();
  LOG_MSG_INFO1_3("Network::GetQosAware(): "
                  "stale handle 0x%x, new handle 0x%x, obj 0x%p",
                  mStaleIfaceHandle, ifaceHandle, this);
  if (0 == ifaceHandle && 0 != mStaleIfaceHandle)
  {
    ifaceHandle = mStaleIfaceHandle;
  }

  result = IfaceIoctlNonNullArg (ifaceHandle,
                                 IFACE_IOCTL_ON_QOS_AWARE_SYSTEM,
                                 static_cast <void *> (pGetQosAware));

  mpICritSect->Leave();
  return result;

} /* Network::GetQosAware() */


ds::ErrorType Network::GetOutageInfo
(
  OutageInfoType *  pOutageInfo
)
{
  ds::ErrorType                result;
  OutageNotificationInfoType   outageInfo;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == pOutageInfo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      In case that outage event didn't occurred, return invalid state
    -----------------------------------------------------------------------*/
    if (FALSE == mOutageEventOccurred)
    {
      pOutageInfo->state        = OutageState::QDS_INVALID;
      pOutageInfo->timeToOutage = 0;
      pOutageInfo->duration     = 0;

      result = AEE_SUCCESS;
      break;
    }

    /*-----------------------------------------------------------------------
      Call the iface IOCTL to get outage info.
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_OUTAGE_NOTIFICATION_INFO,
                        static_cast <void *> (&outageInfo));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert to out-params.
    -----------------------------------------------------------------------*/
    pOutageInfo->timeToOutage = outageInfo.time_to_outage;
    pOutageInfo->duration     = outageInfo.duration;

    if (0 != pOutageInfo->timeToOutage)
    {
      pOutageInfo->state = OutageState::QDS_VALID;
    }
    else if (0 != pOutageInfo->duration)
    {
      pOutageInfo->state = OutageState::QDS_STARTED;
    }
    else
    {
      pOutageInfo->state = OutageState::QDS_EXPIRED;
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_6 ("Network::GetOutageInfo(): Result 0x%x, "
                           "outage state %d, timeToOutage %d, duration %d, "
                           "outage event occured %d, obj 0x%p",
                           result,
                           pOutageInfo->state,
                           pOutageInfo->timeToOutage,
                           pOutageInfo->duration,
                           mOutageEventOccurred,
                           this);
  mpICritSect->Leave();
  return result;

} /* Network::GetOutageInfo() */

ds::ErrorType Network::GetFastDormStatusInfo
(
  uint8 *  piFastDormStatusInfo
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == piFastDormStatusInfo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();
  *piFastDormStatusInfo = mFastDormStatus;

  LOG_MSG_FUNCTION_EXIT_2 ("Network::GetFastDormStatusInfo(): "
                           "Dormancy status %d, obj 0x%p",
                           mFastDormStatus, this);
  mpICritSect->Leave();
  return AEE_SUCCESS;

} /* Network::GetFastDormStatusInfo() */

ds::ErrorType Network::GetBearerInfo
(
  IBearerInfo **  ppIDSNetBearerInfo
)
{
  BearerTechType           bearerTechInfo;
  BearerTechRateType       pDataBearerRate;
  ds::ErrorType            result;
  int32                    ifaceHandle;
  ds::Net::IfaceNameType   ifaceName;
  bool                     usingStaleHandle = false;
  bool                     bearerTechAllocFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == ppIDSNetBearerInfo)
  {
    return QDS_EFAULT;
  }

  *ppIDSNetBearerInfo = NULL;

  memset (&bearerTechInfo, 0, sizeof (BearerTechType));

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    HACK to make event propagation work even after call goes down
  -------------------------------------------------------------------------*/
  ifaceHandle = GetHandle();
  if (0 == ifaceHandle && 0 != mStaleIfaceHandle)
  {
    ifaceHandle = mStaleIfaceHandle;
    usingStaleHandle = true;
  }

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get bearer technology
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (ifaceHandle,
                       IFACE_IOCTL_GET_BEARER_TECHNOLOGY,
                       static_cast <void *> (&bearerTechInfo));
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Get bearer technology rate
  -------------------------------------------------------------------------*/
  result = IfaceIoctlNonNullArg (ifaceHandle,
                                 IFACE_IOCTL_GET_DATA_BEARER_RATE,
                                 static_cast <void *> (&pDataBearerRate));
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Convert the network type to iface name
  -------------------------------------------------------------------------*/
  switch (bearerTechInfo.current_network)
  {
    case PS_IFACE_NETWORK_CDMA:
    {
      ifaceName = IfaceName::IFACE_CDMA_SN;
      break;
    }

    case PS_IFACE_NETWORK_UMTS:
    {
      ifaceName = IfaceName::IFACE_UMTS;
      break;
    }

    case PS_IFACE_NETWORK_WLAN:
    {
      ifaceName = IfaceName::IFACE_WLAN;
      break;
    }

    default:
    {
      LOG_MSG_ERROR_2 ("Network::GetBearerInfo(): Unknown network %d, obj 0x%p",
                       bearerTechInfo.current_network, this);
      result = QDS_INTERNAL;
      goto bail;
    }
  }

  /*-------------------------------------------------------------------------
    Construct the Bearer tech object using the IOCTL result.
  -------------------------------------------------------------------------*/
  *ppIDSNetBearerInfo =
    static_cast <IBearerInfo *>
      (new BearerTech (ifaceName,
                       bearerTechInfo.data.cdma_type.rat_mask,
                       bearerTechInfo.data.cdma_type.so_mask,
                       bearerTechInfo.data.umts_type.rat_mask,
                       &pDataBearerRate));
  if (0 == *ppIDSNetBearerInfo)
  {
    bearerTechAllocFailed = true;
    result = AEE_ENOMEMORY;
    goto bail;
  }

  result = AEE_SUCCESS;

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetBearerInfo(): "
                           "Result 0x%x, using stale handle %d, bearer tech"
                           "alloc failed %d, obj 0x%p", result, 
                           usingStaleHandle, bearerTechAllocFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetBearerInfo() */

ds::ErrorType Network::GetBearerTechEx
(
  IBearerTechEx **  ppIDSNetBearerTechEx
)
{
  BearerTechExType         bearerTechExInst;
  ds::ErrorType            result;
  int32                    ifaceHandle;
  bool                     usingStaleHandle = false;
  bool                     bearerTechAllocFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppIDSNetBearerTechEx)
  {
    return QDS_EFAULT;
  }

  *ppIDSNetBearerTechEx = NULL;

  memset (&bearerTechExInst, 0, sizeof (BearerTechExType));

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    HACK to make event propagation work even after call goes down
  -------------------------------------------------------------------------*/
  ifaceHandle = GetHandle();
  if (0 == ifaceHandle && 0 != mStaleIfaceHandle)
  {
    ifaceHandle = mStaleIfaceHandle;
    usingStaleHandle = true;
  }

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get bearer technology
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (ifaceHandle,
                       IFACE_IOCTL_GET_BEARER_TECHNOLOGY_EX,
                       static_cast <void *> (&bearerTechExInst));
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Construct the Bearer tech object using the IOCTL result.
  -------------------------------------------------------------------------*/
  *ppIDSNetBearerTechEx =
    static_cast <IBearerTechEx *>
      (new BearerTechEx ((uint32)bearerTechExInst.technology, 
                         (uint32)bearerTechExInst.rat_value, 
                         (uint32)bearerTechExInst.so_mask));
  if (0 == *ppIDSNetBearerTechEx)
  {
    bearerTechAllocFailed = true;
    result = AEE_ENOMEMORY;
    goto bail;
  }

  result = AEE_SUCCESS;

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetBearerTechEx(): "
                           "Result 0x%x, using stale handle %d, bearer tech"
                           "alloc failed %d, obj 0x%p", result, 
                           usingStaleHandle, bearerTechAllocFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetBearerTechEx() */

ds::ErrorType Network::GetDataPathBridgeMode
(
  DataPathBridgeModeType* pGetDataPathBridgeMode
)
{
  ds::ErrorType                        result = AEE_SUCCESS;
  ps_iface_ioctl_data_path_info_type   dataPathInfo;
  bool                                 getDataPathFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == pGetDataPathBridgeMode)
  {
    return QDS_EFAULT;
  }

  memset (pGetDataPathBridgeMode, 0, sizeof (DataPathBridgeModeType));
  
  memset (&dataPathInfo, 0, sizeof (dataPathInfo));

  dataPathInfo.client_type = PS_IFACE_IOCTL_CLIENT_DSS;

  
  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get data path
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_GET_DATA_PATH,
                       static_cast <void *> (&dataPathInfo));

  if (AEE_SUCCESS != result)
  {
    getDataPathFailed = true;
    goto bail;
  }
  
  pGetDataPathBridgeMode->dataPath =  dataPathInfo.data_path;

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::GetDataPathBridgeMode(): "
                           "Result 0x%x, get data path failed %d, obj 0x%p",
                           result, getDataPathFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetDataPathBridgeMode() */

ds::ErrorType Network::SetDataPathBridgeMode
(
  DataPathBridgeModeType* pSetDataPathBridgeMode
)
{
  ds::ErrorType                        result = AEE_SUCCESS;
  ps_iface_ioctl_data_path_info_type   dataPathInfo;
  bool                                 setDataPathFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == pSetDataPathBridgeMode)
  {
    return QDS_EFAULT;
  }

  dataPathInfo.client_type = PS_IFACE_IOCTL_CLIENT_DSS;
  dataPathInfo.data_path   = 
    (ps_iface_ioctl_data_path_enum_type) pSetDataPathBridgeMode->dataPath;

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get data path
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_SET_DATA_PATH_BRIDGE_MODE,
                       static_cast <void *> (&dataPathInfo));

  if (AEE_SUCCESS != result)
  {
    setDataPathFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::SetDataPathBridgeMode(): "
                           "Result 0x%x, set data path failed %d, obj 0x%p",
                           result, setDataPathFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::SetDataPathBridgeMode() */

ds::ErrorType Network::GetOpResPco
(
  NetworkOpReservedPcoType* pGetOpResPco
)
{
  ds::ErrorType result = AEE_SUCCESS;
  bool          getOpResPcoFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == pGetOpResPco)
  {
    return QDS_EFAULT;
  }

  memset (pGetOpResPco, 0, sizeof (NetworkOpReservedPcoType));

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get data path
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_GET_OP_RES_PCO,
                       static_cast <void *> (pGetOpResPco));

  if (AEE_SUCCESS != result)
  {
    getOpResPcoFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::GetOpResPco(): "
                           "Result 0x%x, GetOpResPco failed %d, obj 0x%p",
                           result, getOpResPcoFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetOpResPco() */

ds::ErrorType Network::GetOpResPcoList
(
  NetworkOpReservedPcoListType* pGetOpResPcoList
)
{
  ds::ErrorType result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == pGetOpResPcoList)
  {
    result = QDS_EFAULT;
    goto bail;
  }

  memset (pGetOpResPcoList, 0, sizeof (NetworkOpReservedPcoListType));

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get data path
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_GET_OP_RES_PCO_LIST,
                       static_cast <void *> (pGetOpResPcoList));

  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_2 ("Network::GetOpResPcoList(): Result 0x%x, obj 0x%p",
                           result, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetOpResPcoList() */


ds::ErrorType Network::GetMSISDN
(
  MSISDNType* pGetMSISDN
)
{
  ds::ErrorType result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == pGetMSISDN)
  {
    result = QDS_EFAULT;
    goto bail;
  }

  memset (pGetMSISDN, 0, sizeof (MSISDNType));

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get data path
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_GET_MSISDN_INFO,
                       static_cast <void *> (pGetMSISDN));

  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_2 ("Network::GetMSISDN(): Result 0x%x, obj 0x%p",
                           result, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetMSISDN() */

ds::ErrorType Network::_3GPPLTEReestabCalltypeGet
(
  _3GPPLTEReestabCalltypeStruct* p3GPPLTEReestabCalltypeGet
)
{
  ds::ErrorType result = AEE_SUCCESS;
  bool          get3GPPLTEReestabCalltypeFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == p3GPPLTEReestabCalltypeGet)
  {
    return QDS_EFAULT;
  }

  memset (p3GPPLTEReestabCalltypeGet, 0, sizeof (_3GPPLTEReestabCalltypeStruct));

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get 3GPP LTE Reestab Calltype
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_3GPP_GET_LTE_REESTAB_CALLTYPE,
                       static_cast <void *> (p3GPPLTEReestabCalltypeGet));

  if (AEE_SUCCESS != result)
  {
    get3GPPLTEReestabCalltypeFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::_3GPPLTEReestabCalltypeGet(): "
                           "Result 0x%x, 3GPPLTEReestabCalltypeGet() failed %d,"
                           "obj 0x%p",
                           result, get3GPPLTEReestabCalltypeFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::_3GPPLTEReestabCalltypeGet() */

ds::ErrorType Network::_3GPPLTEReestabCalltypeSet
(
  _3GPPLTEReestabCalltypeStruct* p3GPPLTEReestabCalltypeSet
)
{
  ds::ErrorType result = AEE_SUCCESS;
  bool          set3GPPLTEReestabCalltypeFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == p3GPPLTEReestabCalltypeSet)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to set 3GPP LTE Reestab Calltype
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_3GPP_SET_LTE_REESTAB_CALLTYPE,
                       static_cast <void *> (p3GPPLTEReestabCalltypeSet));

  if (AEE_SUCCESS != result)
  {
    set3GPPLTEReestabCalltypeFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::_3GPPLTEReestabCalltypeSet(): "
                           "Result 0x%x, 3GPPLTEReestabCalltypeSet failed %d,"
                           " obj 0x%p",
                           result, set3GPPLTEReestabCalltypeFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::_3GPPLTEReestabCalltypeSet() */

ds::ErrorType Network::SetSilentRedial
(
  SilentRedialStruct *pSilentRedial
)
{
  ds::ErrorType result = AEE_SUCCESS;
  bool          setSilentRedialFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == pSilentRedial)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to set SilentRedial
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_SET_SILENT_REDIAL,
                       static_cast <void *>(pSilentRedial));

  if (AEE_SUCCESS != result)
  {
    setSilentRedialFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::SetSilentRedial(): "
                           "Result 0x%x, setSilentRedial failed %d, obj 0x%p",
                           result, setSilentRedialFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::SetSilentRedial() */

ds::ErrorType Network::_3GPPFORCEPDNdisconnect()
{
  ds::ErrorType result = AEE_SUCCESS;
  bool          disconnect3GPPFORCEPDNFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to disconnect PDN
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_3GPP_FORCE_PDN_DISCONNECT,
                       NULL);

  if (AEE_SUCCESS != result)
  {
    disconnect3GPPFORCEPDNFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::_3GPPFORCEPDNdisconnect(): "
                           "Result 0x%x, 3GPPFORCEPDNdisconnect failed %d, "
                           "obj 0x%p",
                           result, disconnect3GPPFORCEPDNFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::_3GPPFORCEPDNdisconnect() */

ds::ErrorType Network::Setkeepalive(KeepAliveType pKeepalive)
{
  ds::ErrorType result = AEE_SUCCESS;
  bool          setkeepaliveFailed = false;

   mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to disconnect PDN
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_KEEP_ALIVE,
                       &pKeepalive);

  if (AEE_SUCCESS != result)
  {
    setkeepaliveFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::Setkeepalive(): "
                           "Result 0x%x, setkeepalive failed %d, obj 0x%p",
                           result, setkeepaliveFailed, this);
  mpICritSect->Leave();

  return result;
}/* Network::Setkeepalive() */

ds::ErrorType Network::StartDpdPing(dpdPingConfigType pDpdPingConfig)
{
  ds::ErrorType result = AEE_SUCCESS;
  bool          startDpdFailed = false;

   mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to disconnect PDN
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_WLAN_START_DPD_PING,
                       &pDpdPingConfig);

  if (AEE_SUCCESS != result)
  {
    startDpdFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::StartDpdPing(): "
                           "Result 0x%x, StartDpdPing failed %d, obj 0x%p",
                           result, startDpdFailed, this);
  mpICritSect->Leave();

  return result;
}/* Network::StartDpdPing() */



ds::ErrorType Network::GetAddressFamily
(
  ds::AddrFamilyType *  argAddressFamily
)
{
  ds::ErrorType        result;
  PSIPAddrFamilyType   addressFamily;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == argAddressFamily)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Call the iface IOCTL to get address family
    -----------------------------------------------------------------------*/
    memset (&addressFamily, 0, sizeof (PSIPAddrFamilyType));
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_IP_FAMILY,
                         static_cast <void *> (&addressFamily));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    result = PS2DSAddrFamily(addressFamily, argAddressFamily);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_3 ("Network::GetAddressFamily(): Result 0x%x, "
                           "family %d, obj 0x%p", result, addressFamily, this);
  mpICritSect->Leave();
  return result;

} /* Network::GetAddressFamily() */

//TODO Aren't NULL checks reqd for pSipDomainNames, pSipDomainNamesLenReq
//Also check if all info is printed. For example, how many domain names are
//requested by user and how many are returned by dsNet
ds::ErrorType Network::GetDomainNameSearchList
(
  DomainName *  pDomainNameSearchList,
  int           domainNameSearchListLen,
  int *         pDomainNameSearchListLenReq
)
{
  DomainNameSearchListType  domainNameSearchList;
  ds::ErrorType             result;
  int                       index;
  int                       seqNumItems = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  memset (&domainNameSearchList, 0, sizeof (domainNameSearchList));
  PS_SYSTEM_HEAP_MEM_ALLOC(
    domainNameSearchList.name_array,
    sizeof (DomainNameType) * PS_IFACE_MAX_SEARCH_LIST_DOMAIN_NAMES,
    dss_iface_ioctl_domain_name_type*);

  if (0 == domainNameSearchList.name_array)
  {
    return AEE_ENOMEMORY;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Call the iface IOCTL to get domain name search list
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_DOMAIN_NAME_SEARCH_LIST,
                         static_cast <void *> (&domainNameSearchList));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert domain name search list into ds format
    -----------------------------------------------------------------------*/
    result = ds::Utils::Conversion::ProcessLenReq (domainNameSearchListLen,
                                                   pDomainNameSearchListLenReq,
                                                   domainNameSearchList.count,
                                                   &seqNumItems);
    if (AEE_SUCCESS != result)
    {
      LOG_MSG_ERROR_1("Network::GetDomainNameSearchList(): Failed to convert "
                      "domain name search list in PS format to QCM format, "
                      "obj 0x%p", this);
      break;
    }

    /*-----------------------------------------------------------------------
      Convert the individual domain name search list items into out argument
    -----------------------------------------------------------------------*/
    for (index = 0; index < seqNumItems; index++)
    {
      memscpy(pDomainNameSearchList[index],
        sizeof(DomainName),
        domainNameSearchList.name_array[index].domain_name,
        sizeof(DomainName));
    }
  } while (0);

  PS_SYSTEM_HEAP_MEM_FREE(domainNameSearchList.name_array);

  LOG_MSG_FUNCTION_EXIT_3 ("Network::GetDomainNameSearchList(): "
                           "Result 0x%x, returning %d items, obj 0x%p",
                           result, seqNumItems, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetDomainNameSearchList() */

ds::ErrorType Network::GetNetMTU
(
  int *  pMTU
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (GetHandle(),
                                 IFACE_IOCTL_GET_MTU,
                                 static_cast <void *> (pMTU));

  mpICritSect->Leave();
  return result;

} /* Network::GetNetMTU() */

ds::ErrorType Network::GetNetPathMTU
(
  int *  pMTU
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (GetHandle(),
                                 IFACE_IOCTL_GET_PATH_MTU,
                                 static_cast <void *> (pMTU));

  mpICritSect->Leave();
  return result;

} /* Network::GetNetPathMTU() */


ds::ErrorType Network::GetHWAddress
(
  HWAddressType *  pHWAddr
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (GetHandle(),
                                 IFACE_IOCTL_GET_HW_ADDR,
                                 static_cast <void *> (pHWAddr));

  mpICritSect->Leave();
  return result;

} /* Network::GetHWAddress() */

ds::ErrorType Network::IsLaptopCallActive
(
  boolean *  pIsActive
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (GetHandle(),
                                 IFACE_IOCTL_IS_LAPTOP_CALL_ACTIVE,
                                 static_cast <void *> (pIsActive));

  mpICritSect->Leave();
  return result;

} /* Network::IsLaptopCallActive() */


ds::ErrorType Network::GoDormant
(
  DormantReasonType  dormantReason
)
{
  IPhysLink *     txPhysLink = 0;
  ds::ErrorType   result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Obtain Tx phys link and issue dormancy
    -----------------------------------------------------------------------*/
    result = GetTXPhysLink(&txPhysLink);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    result = txPhysLink->GoDormant(dormantReason);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_3 ("Network::GoDormant(): Result 0x%x, obj 0x%p dormancy reason %d",
                           result, this, dormantReason);

  DS_UTILS_RELEASEIF(txPhysLink);

  mpICritSect->Leave();
  return result;

} /* Network::GoDormant() */

ds::ErrorType Network::EnableDNSDuringIPCP
(
  boolean  enable
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (GetHandle(),
                                 IFACE_IOCTL_IPCP_DNS_OPT,
                                 static_cast <void *> (&enable));

  LOG_MSG_FUNCTION_EXIT_3 ("Network::EnableDNSDuringIPCP(): "
                           "Result 0x%x, obj 0x%p, enable %d", result, this, enable);
  mpICritSect->Leave();
  return result;

} /* Network::EnableDNSDuringIPCP() */

ds::ErrorType Network::RefreshDHCPConfigInfo
(
  void
)
{
  ds::ErrorType   result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get hardware address.
  -------------------------------------------------------------------------*/
  result =
    IfaceIoctl (GetHandle(), IFACE_IOCTL_REFRESH_DHCP_CONFIG_INFO, NULL);

  mpICritSect->Leave();
  return result;

} /* Network::RefreshDHCPConfigInfo() */

ds::ErrorType Network::GetCurrRFCondition
(
  RFConditionType *  pDSRFCondition
)
{
  ds::ErrorType        result;
  RFConditionsType     rfCondition;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == pDSRFCondition)
  {
    return QDS_EFAULT;
  }

  memset (&rfCondition, 0, sizeof (RFConditionsType));

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Call the iface IOCTL to get RF conditions.
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_RF_CONDITIONS,
                         static_cast <void *> (&rfCondition));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert to out args
    -----------------------------------------------------------------------*/
    *pDSRFCondition = (RFConditionType) rfCondition.rf_conditions;
  } while (0);

  mpICritSect->Leave();
  return result;

} /* Network::GetCurrRFCondition() */


ds::ErrorType Network::GetIfaceName
(
  ds::Net::IfaceNameType *  pIfaceName
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == pIfaceName)
  {
    return QDS_EFAULT;
  }

  memset (pIfaceName, 0, sizeof (ds::Net::IfaceNameType));

  mpICritSect->Enter();

  result = IfaceIoctl(GetHandle(),
                      IFACE_IOCTL_GET_IFACE_NAME,
                      static_cast <void *> (pIfaceName));

  LOG_MSG_FUNCTION_EXIT_3 ("Network::GetIfaceName(): "
                           "Result 0x%x, iface name %d, obj 0x%p",
                           result, *pIfaceName, this);
  mpICritSect->Leave();
  return result;

} /* Network::GetIfaceName() */

//TODO Enhance logging
ds::ErrorType Network::GetDNSAddr
(
  ::ds::IPAddrType *  pDNSAddrs,
  int                 dnsAddrsLen,
  int *               pDNSAddrsLenReq
)
{
  AllDNSAddrsType   allDNSAddrs;
  ds::ErrorType     result;
  ds::ErrorType     copyResult = AEE_SUCCESS;
  int               index;
  int               seqNumItems = 0;
  int               numCopied = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    Prepare the IOCTL arguments to get the SIP server domain names.
  -----------------------------------------------------------------------*/
  memset (&allDNSAddrs, 0, sizeof (allDNSAddrs));

  PS_SYSTEM_HEAP_MEM_ALLOC(
    allDNSAddrs.dns_addrs_ptr,
    sizeof (ip_addr_type) * PS_IFACE_NUM_DNS_ADDRS,
    ip_addr_type*);

  if (0 == allDNSAddrs.dns_addrs_ptr)
  {
    return AEE_ENOMEMORY;
  }

  allDNSAddrs.num_dns_addrs = PS_IFACE_NUM_DNS_ADDRS;

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Call the iface IOCTL to get all DNS addresses
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_ALL_DNS_ADDRS,
                         static_cast <void *> (&allDNSAddrs));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    result = ds::Utils::Conversion::ProcessLenReq (dnsAddrsLen,
                                                   pDNSAddrsLenReq,
                                                   allDNSAddrs.num_dns_addrs,
                                                   &seqNumItems);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert to out params.
    -----------------------------------------------------------------------*/
    for (index = 0; index < seqNumItems; index++)
    {
      copyResult = PS2DSIPAddr (&allDNSAddrs.dns_addrs_ptr[index],
                                &pDNSAddrs[numCopied]);
      if (AEE_SUCCESS != copyResult)
      {
        /*-------------------------------------------------------------------
          Do not copy invalid address
        -------------------------------------------------------------------*/
        LOG_MSG_INFO1_2( "Network::GetDNSAddr(): Not copying addr "
                         "at index %d, obj 0x%p", index, this);
        continue;
      }

      numCopied++;
    }
  } while (0);

  /*-------------------------------------------------------------------
    LenReq parameter should reflect the number of valid addresses
    which were copied to output parameter
  -------------------------------------------------------------------*/
  if (NULL != pDNSAddrsLenReq)
  {
    *pDNSAddrsLenReq = numCopied;
  }

  PS_SYSTEM_HEAP_MEM_FREE(allDNSAddrs.dns_addrs_ptr);

  mpICritSect->Leave();

  return result;
} /* Network::GetDNSAddr() */

ds::ErrorType CDECL Network::GetNetworkStatistics
(
  NetworkStatsType *  stats
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  result = NetPlatform::GetIfaceStats(GetHandle(), stats);

  mpICritSect->Leave();

  return result;
} /* Network::GetIfaceStats() */

ds::ErrorType Network::ResetNetworkStatistics
(
  void
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  result = NetPlatform::ResetIfaceStats(GetHandle());

  mpICritSect->Leave();

  return result;
} /* Network::ResetIfaceStats() */

ds::ErrorType Network::GetDHCPRefreshResult
(
  boolean* res
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  //TODO: Refresh results are obtained through events.
  //Store this information in the platform layer.
  (void) res;
  return 0;
} /* GetDHCPRefreshResult() */


ds::ErrorType Network::GetTXPhysLink
(
  ::IPhysLink **  txPhysLinkObj
)
{
  IQoSManager *   pQoSManager = NULL;
  IQoS *          pQoSDefault = NULL;
  ds::ErrorType   result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == txPhysLinkObj)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    result = CreateQoSManager (&pQoSManager);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    result = pQoSManager->GetQosDefault (&pQoSDefault);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    result = pQoSDefault->GetTXPhysLink(txPhysLinkObj);
  } while (0);

  DS_UTILS_RELEASEIF(pQoSManager);
  DS_UTILS_RELEASEIF(pQoSDefault);

  mpICritSect->Leave();

  return result;
} /* Network::GetTXPhysLink() */

ds::ErrorType Network::GetRXPhysLink
(
  ::IPhysLink **  rxPhysLinkObj
)
{
  IQoSManager *  pQoSManager = NULL;
  IQoS *         pQoSDefault = NULL;
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == rxPhysLinkObj)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    result = CreateQoSManager (&pQoSManager);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    result = pQoSManager->GetQosDefault (&pQoSDefault);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    result = pQoSDefault->GetRXPhysLink(rxPhysLinkObj);
  } while (0);

  DS_UTILS_RELEASEIF(pQoSManager);
  DS_UTILS_RELEASEIF(pQoSDefault);

  mpICritSect->Leave();

  return result;
} /* Network::GetRXPhysLink() */


ds::ErrorType Network::SetPDNTeardownConfig
(
  PDNTeardownConfigType* pPDNTeardownConfig
)
{
  ds::ErrorType result = AEE_SUCCESS;
  ps_iface_ioctl_pdn_teardown_config_type   pdnTeardownConfig;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  do
  {
    mpICritSect->Enter();

    if (0 == pPDNTeardownConfig)
    {
      result = QDS_EFAULT;
      break;
    }

    pdnTeardownConfig.local_abort = pPDNTeardownConfig->local_abort;

    /*-------------------------------------------------------------------------
      Call the iface IOCTL to get data path
    -------------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_SET_PDN_TEARDOWN_CONFIG,
                         static_cast <void *> (&pdnTeardownConfig));
  
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while(0);

  /* Fall through */
  mpICritSect->Leave();

  return result;
} /* Network::SetPDNTeardownConfig() */

ds::ErrorType Network::GetPDNTeardownConfig
(
  PDNTeardownConfigType* pPDNTeardownConfig
)
{
  ds::ErrorType result = AEE_SUCCESS;
  ps_iface_ioctl_pdn_teardown_config_type   pdnTeardownConfig;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  do
  {
    mpICritSect->Enter();

    memset (&pdnTeardownConfig, 0, 
            sizeof (ps_iface_ioctl_pdn_teardown_config_type));
            
    if (0 == pPDNTeardownConfig)
    {
      break;
    }

    memset (pPDNTeardownConfig, 0, sizeof (PDNTeardownConfigType));

    /*-------------------------------------------------------------------------
      Call the iface IOCTL to get data path
    -------------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_PDN_TEARDOWN_CONFIG,
                         static_cast <void *> (&pdnTeardownConfig));

    if (AEE_SUCCESS != result)
    {
      break;
    }
  
    pPDNTeardownConfig->local_abort =  pdnTeardownConfig.local_abort;
  } while(0);

  /* Fall through */
  LOG_MSG_INFO2_4 ("Network::GetPDNTeardownConfig(): Result 0x%x, "
                   "obj 0x%p pdnTeardownConfig %d, pPDNTeardownConfig 0x%x",
                   result, this, pdnTeardownConfig.local_abort, 
                   pPDNTeardownConfig);

  mpICritSect->Leave();

  return result;
} /* Network::GetPDNTeardownConfig() */


/*---------------------------------------------------------------------------
  Inherited function from ds::Utils::INode interface.
---------------------------------------------------------------------------*/
boolean Network::Process
(
  void *  userDataPtr
)
{
  EventInfoType *  eventInfoPtr;
  int32            ifaceHandle;
  PSIPAddrType     pIPv6ExternalAddrDeletedEvInfo;
  ApnParamChangeList pApnParamChangeList ={0};
  HandoffEventInfoType *ho_ev = NULL;
  HandoffFailureEventInfoType * fail_ev = NULL;   
  DpdPingStatEvType           * pdpdPingStatInfo = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == userDataPtr)
  {
    return FALSE;
  }

  eventInfoPtr = static_cast <EventInfoType *> (userDataPtr);

  mpICritSect->Enter();

  ifaceHandle = GetHandle();

  DS_INTERFACE_LOG_MSG_INFO1_4 (PS_LOGGING_CAT_DSNET,
  	                        "Network::Process(): "
                   "Handle 0x%x, event 0x%x, event handle 0x%x, obj 0x%p",
                                (unsigned int) ifaceHandle,
                                (unsigned int) eventInfoPtr->eventName,
                                (unsigned int) eventInfoPtr->handle,
                   this);

  /*-------------------------------------------------------------------------
    Process the event only if
      1. It is a network Event
      2. If this object registered for the event
  -------------------------------------------------------------------------*/
  switch (eventInfoPtr->eventGroup)
  {
    case EVENT_GROUP_NETWORK:
    {
      if (EventIsNetworkState(eventInfoPtr->eventName))
      {
        ProcessIfaceStateEvent (eventInfoPtr);
      }
      else if (ifaceHandle == eventInfoPtr->handle)
      {
        if (NetworkExtEvent::QDS_EV_EXTENDED_IP_CONFIG_EX == eventInfoPtr->eventName)
        {
          mCachedExtendedMask = *(::ds::Net::NetworkExtendedIpConfigExType*)eventInfoPtr->psEventInfo;
        }
        else if((NetworkExtEvent::QDS_EV_HANDOFF_INIT == eventInfoPtr->eventName) 
                 || (NetworkExtEvent::QDS_EV_HANDOFF_SUCCESS == eventInfoPtr->eventName)
                 || (NetworkExtEvent::QDS_EV_HANDOFF_FAILURE == eventInfoPtr->eventName))
        {
          if(NetworkExtEvent::QDS_EV_HANDOFF_FAILURE == eventInfoPtr->eventName)
          { 
            fail_ev = (HandoffFailureEventInfoType*)eventInfoPtr->psEventInfo;
            mcachedHandoffFailureinfo.srat = (uint16)fail_ev->srat;
            mcachedHandoffFailureinfo.trat = (uint16)fail_ev->trat;
            mcachedHandoffFailureinfo.failure_reason = (uint32)fail_ev->ho_failure_reason;
            mcachedHandoffFailureinfo.ancillary_info.mask = (uint32)fail_ev->ancillary_info.ancillary_info_mask;
          }
          else
          {  /*Init or Sucess event */
            ho_ev = (HandoffEventInfoType*)eventInfoPtr->psEventInfo;
            mcachedHandoffinfo.srat = (uint16)ho_ev->srat;
            mcachedHandoffinfo.trat = (uint16)ho_ev->trat;
            mcachedHandoffinfo.ancillary_info.mask = (uint32)ho_ev->ancillary_info.ancillary_info_mask;
          }
    }
        else if (NetworkEvent::QDS_EV_OUTAGE == eventInfoPtr->eventName)
        {
          mOutageEventOccurred = TRUE;
        }
        else if (NetworkEvent::QDS_EV_FAST_DORM_STATUS ==
                   eventInfoPtr->eventName)
        {
          mFastDormStatus = (uint8) ((uint32)(eventInfoPtr->psEventInfo));
        }
        else if (NetworkEvent::QDS_EV_IPV6_EXT_ADDR_DEL_EV == 
                 eventInfoPtr->eventName)
        {
          memscpy( (void *)&pIPv6ExternalAddrDeletedEvInfo,
            sizeof(PSIPAddrType),
            eventInfoPtr->psEventInfo,
            sizeof(PSIPAddrType));
          /*-------------------------------------------------------------------------
            Cache the deleted address in ds_Net, later the client will fetch it
          -------------------------------------------------------------------------*/          
          memscpy(mDeletedIPv6Addr,
            sizeof (ds::INAddr6Type),
            &pIPv6ExternalAddrDeletedEvInfo.addr.v6,
            sizeof (ds::INAddr6Type));
        }
        else if (NetworkExtEvent::QDS_EV_APN_PARAM_EV == 
                 eventInfoPtr->eventName)
        {
          memscpy( (void *)&pApnParamChangeList,
            sizeof(ApnParamChangeList),
            eventInfoPtr->psEventInfo,
            sizeof(ApnParamChangeList));
        }

        else if (NetworkExtEvent::QDS_EV_DPD_PING_STAT_EV == 
                 eventInfoPtr->eventName)
        {
          pdpdPingStatInfo = (DpdPingStatEvType*)eventInfoPtr->psEventInfo;

          mcachedDpdPingStatInfo.num_pings = pdpdPingStatInfo->num_pings;
          mcachedDpdPingStatInfo.pkts_loss = pdpdPingStatInfo->pkts_loss;
          mcachedDpdPingStatInfo.avg_rtt = pdpdPingStatInfo->avg_rtt;
          mcachedDpdPingStatInfo.user_data_ptr = pdpdPingStatInfo->user_data_ptr;
          
          memset(&mcachedDpdPingStatInfo.rtt, 0, sizeof(uint16)*255);
          memscpy(&mcachedDpdPingStatInfo.rtt,
            (sizeof(uint16)*(mcachedDpdPingStatInfo.num_pings)),
            &pdpdPingStatInfo->rtt,
            (sizeof(uint16)*(mcachedDpdPingStatInfo.num_pings)));
        }

        Notify (eventInfoPtr->eventName);
      }
      else if (ifaceHandle == 0 &&
               mStaleIfaceHandle == eventInfoPtr->handle &&
               (NetworkExtEvent::QDS_EV_BEARER_TECH_CHANGED ==
                  eventInfoPtr->eventName ||
                  NetworkExtEvent::QDS_EV_BEARER_TECH_CHANGED_EX ==
                  eventInfoPtr->eventName ||
                NetworkExtEvent::QDS_EV_QOS_AWARENESS ==
                  eventInfoPtr->eventName))
      {
        LOG_MSG_INFO1_2("Network::Process(): "
                        "Posting event 0x%x even though handle is 0 because of "
                        "stale handle 0x%x match",
                        eventInfoPtr->eventName, mStaleIfaceHandle);
        Notify (eventInfoPtr->eventName);
      }

      break;
    }

    default:
    {
      break;
    }
  } /* switch */

  mpICritSect->Leave();

  return TRUE;

} /* Network::Process() */

ds::ErrorType Network::OnStateChange
(
  ISignal *    signalObj,
  EventType    eventID,
  IQI **       regObj
 )
{
  ds::ErrorType  result = AEE_SUCCESS;
  EventType networkEventType = -1;
  NetworkStateType netState = NetworkState::QDS_CLOSED;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == signalObj)
  {
    return QDS_EFAULT;
  }

  switch (eventID)
  {
    // network state
    case NetworkEvent::QDS_EV_IFACE_UP:
    case NetworkEvent::QDS_EV_IFACE_GOING_DOWN:

    // dss cannot register to the following events,
    // no signals buses exist to support it:
    //NetworkEvent::QDS_EV_IFACE_ENABLED
    //NetworkEvent::QDS_EV_IFACE_DISABLED

    case NetworkEvent::QDS_EV_IFACE_DOWN:
    case NetworkEvent::QDS_EV_IFACE_ACTIVE_OUT_OF_USE:
    case NetworkEvent::QDS_EV_IFACE_COMING_UP:
    case NetworkEvent::QDS_EV_IFACE_CONFIGURING:
    case NetworkEvent::QDS_EV_IFACE_ROUTEABLE:

    case NetworkEvent::QDS_EV_IP_ADDR_CHANGED:
    case NetworkEvent::QDS_EV_OUTAGE:
    case NetworkEvent::QDS_EV_RF_CONDITIONS_CHANGED:
    case NetworkExtEvent::QDS_EV_QOS_AWARENESS:
    case NetworkExtEvent::QDS_EV_BEARER_TECH_CHANGED:
    case NetworkExtEvent::QDS_EV_BEARER_TECH_CHANGED_EX:
    case NetworkEvent::QDS_EV_IPV6_EXT_ADDR_DEL_EV:
    case NetworkControlEvent::QDS_EV_EXTENDED_IP_CONFIG:
    case NetworkExtEvent::QDS_EV_HANDOFF_INIT:
    case NetworkExtEvent::QDS_EV_HANDOFF_SUCCESS:
    case NetworkExtEvent::QDS_EV_HANDOFF_FAILURE:
    case NetworkExtEvent::QDS_EV_EXTENDED_IP_CONFIG_EX:
    case NetworkExtEvent::QDS_EV_APN_PARAM_EV:
    case NetworkExtEvent::QDS_EV_DPD_PING_STAT_EV:
    {
      break;
    }

    default:
    {
      LOG_MSG_INVALID_INPUT_2 ("Network::OnStateChange(): "
                               "Unknown event 0x%x, obj 0x%p", eventID, this);
      return AEE_EUNSUPPORTED;
    }
  }

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Post event immediately for BEARER_TECH_CHANGED and STATE_CHANGED events
    so that application can query for the current state
  -------------------------------------------------------------------------*/
  if (NetworkExtEvent::QDS_EV_BEARER_TECH_CHANGED == eventID || 
      NetworkExtEvent::QDS_EV_BEARER_TECH_CHANGED_EX == eventID)
  {
     (void) signalObj->AddRef();
     (void) signalObj->Set();
     (void) signalObj->Release();
  }

  if (EventIsNetworkState(eventID))
  {
    (void) GetIfaceState (&netState);
    networkEventType = NetworkState2Event(netState);
    if (networkEventType == eventID)
    {
      (void) signalObj->AddRef();
      (void) signalObj->Set();
      (void) signalObj->Release();
    }
  }

  result = Handle::OnStateChange(signalObj, eventID, regObj);

  mpICritSect->Leave();

  return result;
} /* Network::OnStateChange() */

/*---------------------------------------------------------------------------
  Methods from INetworkPriv interface.
---------------------------------------------------------------------------*/
ds::ErrorType Network::GetIfaceId
(
  IfaceIdType *  pIfaceId
)
{
  ds::ErrorType  result;
  int32          handle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == pIfaceId)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    handle = GetHandle();
    if (0 == handle)
    {
      /*---------------------------------------------------------------------
        Perform network lookup as  network lookup or bringup has not happened
        yet
      ---------------------------------------------------------------------*/
      result = LookupInterface();
      if (AEE_SUCCESS != result)
      {
        break;
      }

      handle = GetHandle();
    }

    result = PSGetIfaceId (handle, (int32 *) pIfaceId);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetIfaceId(): "
                           "Result 0x%x, handle 0x%x, iface id 0x%x, obj 0x%p",
                           result, handle, *pIfaceId, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetIfaceId() */

ds::ErrorType Network::GetIfaceIdNoLookup
(
 IfaceIdType* pIfaceId
)
{
  ds::ErrorType result;
  int32         handle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == pIfaceId)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do 
  {
    handle = GetHandle();

    result = PSGetIfaceId (handle, (int32 *)pIfaceId);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);
  
  DS_INTERFACE_LOG_MSG_INFO1_4(PS_LOGGING_CAT_DSNET,
	                         "Network::GetIfaceIdNoLookup(): "
                             "Result 0x%x, handle 0x%x, iface id 0x%x, obj 0x%p",
                            (unsigned int) result,(unsigned int) handle,
                            (unsigned int) *pIfaceId, this);

  mpICritSect->Leave();

  return result;
} /* Network::GetIfaceIdNoLookup() */

ds::ErrorType Network::GetPreviousState
(
  NetworkStateType *  argNetState
)
{
  ds::ErrorType       result;
  int32               ifaceHandle;
  IfaceStateEnumType  ifaceState;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == argNetState)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    ifaceHandle = GetHandle();
    if (0 == ifaceHandle)
    {
      *argNetState = mCachedPreviousState;
      result       = AEE_SUCCESS;
      break;
    }

    /*-----------------------------------------------------------------------
      Call network platform IOCTL to get the interface state
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (ifaceHandle,
                         IFACE_IOCTL_GET_PREVIOUS_STATE,
                         &ifaceState);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Map Iface state to network state
    -----------------------------------------------------------------------*/
    result = PS2DSIfaceState(ifaceState, argNetState);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetPreviousState(): "
                           "Result 0x%x, Previous netstatus %d, ifaceHandle %d"
                           ", obj 0x%p",
                           result, *argNetState, ifaceHandle, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetPreviousState() */

ds::ErrorType Network::GetState
(
  NetworkStateType *  argNetState
)
{
  ds::ErrorType  result;
  int32          ifaceHandle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == argNetState)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      If the handle is not set in the network object, return QDS_CLOSED.
    -----------------------------------------------------------------------*/
    ifaceHandle = GetHandle();
    if (0 == ifaceHandle)
    {
      *argNetState = NetworkState::QDS_CLOSED;
      result       = AEE_SUCCESS;
      break;
    }

    /*-----------------------------------------------------------------------
      If the bring up flag is set, return QDS_OPEN_IN_PROGRESS
    -----------------------------------------------------------------------*/
    if (TRUE == mBringupAgain)
    {
      *argNetState = NetworkState::QDS_OPEN_IN_PROGRESS;
      result       = AEE_SUCCESS;
      break;
    }

    /*-----------------------------------------------------------------------
      Otherwise, return the associated iface state
    -----------------------------------------------------------------------*/
    result = GetIfaceState (argNetState);
  } while (0);

  LOG_MSG_FUNCTION_EXIT_5 ("Network::GetState(): Result 0x%x, "
                           "netstatus %d, ifaceHandle 0x%x, mBringUpAgain %d, "
                           "obj 0x%p", result, *argNetState, ifaceHandle,
                           mBringupAgain, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetState() */


ds::ErrorType Network::GetPreviousIPAddr
(
  ds::IPAddrType *  argIPAddress
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return IPAddrIOCTL (IFACE_IOCTL_GET_PREVIOUS_IP_ADDR, argIPAddress);

} /* Network::GetPreviousIPAddr() */


ds::ErrorType Network::GetPreviousBearerInfo
(
  IBearerInfo **  ppIDSNetBearerInfo
)
{
  BearerTechType           bearerTechInfo;
  ds::ErrorType            result;
  int32                    ifaceHandle;
  ds::Net::IfaceNameType   ifaceName;
  bool                     ifaceHandleIsStale = false;
  bool                     bearerTechAllocFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppIDSNetBearerInfo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    HACK to make event propagation work even after call goes down
  -------------------------------------------------------------------------*/
  ifaceHandle = GetHandle();
  if (0 == ifaceHandle && 0 != mStaleIfaceHandle)
  {
    ifaceHandle = mStaleIfaceHandle;
    ifaceHandleIsStale = true;
  }

  *ppIDSNetBearerInfo = NULL;
  memset (&bearerTechInfo, 0, sizeof (BearerTechType));

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get bearer technology.
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (ifaceHandle,
                       IFACE_IOCTL_GET_PREVIOUS_BEARER_TECHNOLOGY,
                       static_cast <void *> (&bearerTechInfo));
  if (AEE_SUCCESS != result)
  {
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Convert the network type to iface name.
  -------------------------------------------------------------------------*/
  switch (bearerTechInfo.current_network)
  {
    case PS_IFACE_NETWORK_CDMA:
    {
      ifaceName = IfaceName::IFACE_CDMA_SN;
      break;
    }

    case PS_IFACE_NETWORK_UMTS:
    {
      ifaceName = IfaceName::IFACE_UMTS;
      break;
    }

    case PS_IFACE_NETWORK_WLAN:
    {
      ifaceName = IfaceName::IFACE_WLAN;
      break;
    }

    default:
    {
      LOG_MSG_ERROR_2 ("Network::GetPreviousBearerInfo(): Unknown network %d, "
                       "obj 0x%p", bearerTechInfo.current_network, this);
      result = QDS_INTERNAL;
      goto bail;
    }
  }

  /*-------------------------------------------------------------------------
    Construct the Bearer tech object using the IOCTL result.
  -------------------------------------------------------------------------*/
  *ppIDSNetBearerInfo =
    static_cast <IBearerInfo *>
      (new BearerTech (ifaceName,
                       bearerTechInfo.data.cdma_type.rat_mask,
                       bearerTechInfo.data.cdma_type.so_mask,
                       bearerTechInfo.data.umts_type.rat_mask,
                       NULL));
  if (0 == *ppIDSNetBearerInfo)
  {
    bearerTechAllocFailed = true;
    result = AEE_ENOMEMORY;
    goto bail;
  }

  result = AEE_SUCCESS;

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetPreviousBearerInfo(): "
                           "Result 0x%x, iface handle is stale %d, bearer tech "
                           "alloc failed %d, obj 0x%p",
                           result, ifaceHandleIsStale,
                           bearerTechAllocFailed, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetPreviousBearerInfo() */

ds::ErrorType Network::GetQoSAwareInfoCode
(
  QoSInfoCodeType *  infoCode
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (GetHandle(),
                                 IFACE_IOCTL_GET_QOS_AWARE_INFO_CODE,
                                 static_cast <void *> (infoCode));

  mpICritSect->Leave();
  return result;

} /* Network::GetQoSAwareInfoCode() */


ds::ErrorType Network::GetAllIfaces
(
  IfaceIdType *  ifaceIdArr,
  int            ifaceIdArrLen,
  int *          ifaceIdArrLenReq
)
{
  PSAllIfacesType  allIfacesInfo;
  ds::ErrorType    result;
  unsigned int     index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_3 ("Network::GetAllIfaces(): "
                            "Obj 0x%p, iface id arr len %d, iface id arr 0x%p",
                            this, ifaceIdArrLen, ifaceIdArr);

  if (ifaceIdArrLen <= 0)
  {
    return AEE_SUCCESS;
  }

  if (0 == ifaceIdArr)
  {
    return QDS_EFAULT;
  }

  memset (&allIfacesInfo, 0, sizeof(allIfacesInfo));

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Call the IOCTL
    -----------------------------------------------------------------------*/
    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_GET_ALL_IFACES,
                         static_cast <void *> (&allIfacesInfo));
    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert to out params.
    -----------------------------------------------------------------------*/
    for (index = 0;
         index < allIfacesInfo.number_of_ifaces && 
         index < (unsigned int)ifaceIdArrLen;
         index++)
    {
      ifaceIdArr[index] = allIfacesInfo.ifaces[index];
    }

    if (0 != ifaceIdArrLenReq)
    {
      *ifaceIdArrLenReq = (int) allIfacesInfo.number_of_ifaces;
    }
  } while (0);

  mpICritSect->Leave();

  return result;
} /* Network::GetAllIfaces() */


ds::ErrorType Network::QueryInterface
(
  AEEIID   iid,
  void **  ppo
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  switch (iid)
  {
    case AEEIID_INetwork:
    case AEEIID_IQI:
    {
      *ppo   = static_cast <INetwork *> (this);
      result = AEE_SUCCESS;

      (void) AddRef();
      break;
    }

    case AEEIID_INetworkPriv:
    {
      *ppo   = static_cast <INetworkPriv *> (this);
      result = AEE_SUCCESS;

      (void) AddRef();
      break;
    }

    case AEEIID_INetworkControl:
    {
      *ppo   = static_cast <INetworkControl *> (this);
      result = AEE_SUCCESS;

      (void) AddRef();
      break;
    }

    case AEEIID_INetworkExt:
    {
      *ppo   = static_cast <INetworkExt *> (this);
      result = AEE_SUCCESS;

      (void) AddRef();
      break;
    }

    case AEEIID_INetworkExt2:
    {
      *ppo   = static_cast <INetworkExt2 *> (this);
      result = AEE_SUCCESS;

      (void) AddRef();
      break;
    }

    case AEEIID_IIPFilterManagerPriv:
    {
      *ppo   = static_cast <IIPFilterManagerPriv *> (this);
      result = AEE_SUCCESS;

      (void) AddRef();
      break;
    }

    case AEEIID_IQoSManager:
    {
      result = CreateQoSManager (reinterpret_cast <IQoSManager **> (ppo));
      break;
    }
#ifdef FEATUTE_DATA_PS_MCAST
    case AEEIID_IMCastManager:
    {
      result = CreateMCastManager (reinterpret_cast <IMCastManager **> (ppo));
      break;
    }
#endif // FEATUTE_DATA_PS_MCAST
    case AEEIID_IFirewallManager:
    {
      result =
        CreateNetFirewallManager (reinterpret_cast <IFirewallManager **> (ppo));
      break;
    }

    case AEEIID_IRanAsstMediaAdaptManager:
    {
      result =
        CreateNetRanAsstManager (reinterpret_cast <IRanAsstMediaAdaptManager **> (ppo));
      break;
    }

    case AEEIID_INetwork1x:
    case AEEIID_INetwork1xPriv:
    case AEEIID_INetworkUMTS:
    case AEEIID_INetworkIPv6:
    case AEEIID_INetworkIPv6Priv:
    {
      result = GetTechObject(iid, ppo);
      break;
    }

    default:
    {
      result = AEE_ECLASSNOTSUPPORT;
      break;
    }
  }

  LOG_MSG_INFO3_4 ("Network::QueryInterface(): "
                           "Result 0x%x, ppo 0x%p, obj 0x%p, iid 0x%x",
                           result, *ppo, this, iid);
  mpICritSect->Leave();

  return result;
} /* Network::QueryInterface() */

ds::ErrorType Network::CreateNetFirewallManager
(
  IFirewallManager **  ppIFirewallMgr
)
{
  ds::ErrorType  result;
  int32          ifaceHandle;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == ppIFirewallMgr)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    ifaceHandle = GetHandle();
    if (0 == ifaceHandle)
    {
      result = QDS_EINVAL;
      break;
    }

    /*-----------------------------------------------------------------------
      Create a new FirewallManager object all the time. If the network
      object's handle changes anytime, old FirewallManager does not make sense.
    -----------------------------------------------------------------------*/
    *ppIFirewallMgr =
      static_cast <IFirewallManager *> (new FirewallManager (ifaceHandle));
    if (0 == *ppIFirewallMgr)
    {
      result = AEE_ENOMEMORY;
      break;
    }

    result = AEE_SUCCESS;
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::CreateNetFirewallManager(): "
                           "Result 0x%x, FirewallManager 0x%p, iface handle %d"
                           ", obj 0x%p",
                           result, *ppIFirewallMgr,ifaceHandle, this);
  mpICritSect->Leave();
  return result;

} /* Network::CreateNetFirewallManager() */
ds::ErrorType Network::GetNetworkIPv6
(
  AEEIID   iid,
  void **  ppo
)
{
#ifdef FEATURE_DATA_PS_IPV6
  ds::ErrorType  result;
  bool           ipv6SetupFailed = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INVALID_INPUT_3 ("Network::GetNetworkIPv6(): "
                           "ppo 0x%p iid 0x%x, obj 0x%p",ppo, iid, this);

  if (0 == ppo)
  {
    return QDS_EFAULT;
  }

  if (AEEIID_INetworkIPv6 != iid && AEEIID_INetworkIPv6Priv != iid)
  {
    return AEE_ECLASSNOTSUPPORT;
  }

  mpICritSect->Enter();

  do
  {
    if (0 == mpNetworkIPv6)
    {
      //TODO What if handle is 0?
      mpNetworkIPv6 = new NetworkIPv6(GetHandle());
      if (0 == mpNetworkIPv6)
      {
        ipv6SetupFailed = true;
        result = AEE_ENOMEMORY;
        break;
      }

      //TODO Combine constructor and Init to do CreateInstance
      result = mpNetworkIPv6->Init();
      if (AEE_SUCCESS != result)
      {
        ipv6SetupFailed = true;
        break;
      }
    }
    else
    {
      mpNetworkIPv6->SetIfaceHandle(GetHandle());
    }

    /*-----------------------------------------------------------------------
      We are returning an interface. Perform AddRef()
    -----------------------------------------------------------------------*/
    (void) mpNetworkIPv6->AddRef();

    if (AEEIID_INetworkIPv6 == iid)
    {
      *ppo = static_cast <INetworkIPv6 *> (mpNetworkIPv6);
    }
    else
    {
      *ppo = static_cast <INetworkIPv6Priv *> (mpNetworkIPv6);
    }

    result = AEE_SUCCESS;
  } while (0);

  /*-------------------------------------------------------------------------
    Error handling
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS != result)
  {
    DS_UTILS_RELEASEIF(mpNetworkIPv6);
  }

  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetNetworkIPv6(): "
                           "Result 0x%x, NetworkIPv6 0x%p, ipv6 setup failed %d"
                           ", obj 0x%p",
                           result, mpNetworkIPv6, ipv6SetupFailed, this);
  mpICritSect->Leave();
  return result;

#else
  return AEE_ECLASSNOTSUPPORT;
#endif /* FEATURE_DATA_PS_IPV6 */

} /* Network::GetNetworkIPv6() */

ds::ErrorType Network::GetNetworkUMTS
(
  AEEIID   iid,
  void **  ppo
)
{
  ds::ErrorType            result;
  ds::Net::IfaceNameType   ifaceName;
  bool                     ifaceError = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INVALID_INPUT_3 ("Network::GetNetworkUMTS(): "
                           "Invalid iid 0x%x, ppo 0x%p, obj 0x%p",
                           iid, ppo, this);

  if (0 == ppo)
  {
    return QDS_EFAULT;
  }

  if (AEEIID_INetworkUMTS != iid)
  {
    return AEE_ECLASSNOTSUPPORT;
  }

  mpICritSect->Enter();

  do
  {
    if (0 == mpNetworkUMTS)
    {
      /*---------------------------------------------------------------------
        Make sure that iface name is compatible with IFACE_UMTS
      ---------------------------------------------------------------------*/
      result = GetIfaceName(&ifaceName);
      if (AEE_SUCCESS != result)
      {
        ifaceError = true;
        break;
      }

      if (IfaceName::IFACE_UMTS != ifaceName &&
          IfaceName::IFACE_STA != ifaceName &&
          IfaceName::IFACE_EPC != ifaceName)
      {
        ifaceError = true;
        result = AEE_ECLASSNOTSUPPORT;
        break;
      }

      mpNetworkUMTS = static_cast <INetworkUMTS *> (new NetworkUMTS(this));
      if (0 == mpNetworkUMTS)
      {
        result = AEE_ENOMEMORY;
        break;
      }
    }

    *ppo =  mpNetworkUMTS;
    (void) mpNetworkUMTS->AddRef();

    result = AEE_SUCCESS;
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetNetworkUMTS(): "
                           "Result 0x%x, NetworkUMTS 0x%p, iface error %d, obj 0x%p",
                           result, mpNetworkUMTS, ifaceError, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetNetworkUMTS() */


ds::ErrorType Network::GetNetwork1x
(
  AEEIID   iid,
  void **  ppo
)
{
  ds::ErrorType                 result;
  ds::Net::IfaceNameType        ifaceName;
  Network1X*                    pNetwork1x = 0;
  bool                          ifaceError = false;
  bool                          network1xError = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INVALID_INPUT_3 ("Network::GetNetwork1x(): "
                           "Invalid iid 0x%x, ppo 0x%p, obj 0x%p", iid, ppo, this);

  if (0 == ppo)
  {
    return QDS_EFAULT;
  }

  if (AEEIID_INetwork1x != iid && AEEIID_INetwork1xPriv != iid)
  {
    return AEE_ECLASSNOTSUPPORT;
  }

  mpICritSect->Enter();

  do
  {
    if (0 == mpTechObjNetwork1x)
    {
      /*---------------------------------------------------------------------
        Make sure that iface name is compatible with IFACE_UMTS
      ---------------------------------------------------------------------*/
      result = GetIfaceName(&ifaceName);
      if (AEE_SUCCESS != result)
      {
        ifaceError = true;
        break;
      }

      if (IfaceName::IFACE_CDMA_SN != ifaceName &&
          IfaceName::IFACE_EPC != ifaceName &&
          IfaceName::IFACE_STA != ifaceName &&
          IfaceName::IFACE_CDMA_BCAST != ifaceName &&
          IfaceName::IFACE_CDMA_AN != ifaceName)
      {
        ifaceError = true;
        result = AEE_ECLASSNOTSUPPORT;
        break;
      }

      //TODO Combine in to CreateInstance
      pNetwork1x = new Network1X (this, GetHandle());
      if (0 == pNetwork1x)
      {
        network1xError = true;
        result = AEE_ENOMEMORY;
        break;
      }

      result = pNetwork1x->Init();
      if (AEE_SUCCESS != result)
      {
        network1xError = true;
        break;
      }

      mpTechObjNetwork1x     = static_cast <INetwork1x *> (pNetwork1x);
      mpTechObjNetwork1xPriv = static_cast <INetwork1xPriv *> (pNetwork1x);
    }

    if (AEEIID_INetwork1x == iid)
    {
      *ppo = mpTechObjNetwork1x;
      (void) mpTechObjNetwork1x->AddRef();
    }
    else
    {
      *ppo = mpTechObjNetwork1xPriv;
      (void) mpTechObjNetwork1xPriv->AddRef();
    }

    result = AEE_SUCCESS;

  } while (0);

  /*-------------------------------------------------------------------------
    Release Network1x in case of failure
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS != result)
  {
    DS_UTILS_RELEASEIF(pNetwork1x);
  }

  LOG_MSG_FUNCTION_EXIT_5 ("Network::GetNetwork1x(): "
                           "Result 0x%x, Network1x 0x%p, iface error %d, "
                           "network1x error %d, obj 0x%p",
                           result, *ppo, ifaceError, network1xError, this);
  mpICritSect->Leave();

  return result;
} /* Network::GetNetwork1x() */


ds::ErrorType Network::GetTechObject
(
  AEEIID   iid,
  void **  ppo
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    switch (iid)
    {
      case AEEIID_INetworkIPv6:
      case AEEIID_INetworkIPv6Priv:
      {
        result = GetNetworkIPv6 (iid, ppo);
        break;
      }

      case AEEIID_INetworkUMTS:
      {
        result = GetNetworkUMTS (iid, ppo);
        break;
      }

      case AEEIID_INetwork1x:
      case AEEIID_INetwork1xPriv:
      {
        result = GetNetwork1x (iid, ppo);
        break;
      }

      default:
      {
        result = AEE_ECLASSNOTSUPPORT;
        break;
      }
    }
  } while (0);

  LOG_MSG_FUNCTION_EXIT_4 ("Network::GetTechObject(): "
                           "Result 0x%x, ppo 0x%p, obj 0x%p, iid 0x%x",
                           result, *ppo, this, iid);
  mpICritSect->Leave();

  return result;
} /* Network::GetTechObject() */

/*---------------------------------------------------------------------------
  Inherited functions from IIPFilterMgr.
---------------------------------------------------------------------------*/
ds::ErrorType Network::RegisterFilters
(
  int                  fi_result,
  IIPFilterPriv **     ppIIPFilterSpec,
  int                  filtersLen,
  IIPFilterRegPriv **  ppIIPFfilterReg,
  IfaceGroupType       iface_group
)
{
  PSIfaceIPFilterAddParamType   filterAddParam;
  ds::ErrorType                 result;
  int32                         ifaceHandle;
  int32                         fltrHandle = PS_IFACE_IPFLTR_INVALID_HANDLE;
  int                           index;
  PSIFaceNameEnumType           ifaceGroup;
  dss::Error::dss_error_enum_type  ds_net_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_4 ("Network::RegisterFilters(): "
                            "Obj 0x%p,filter 0x%p, #of fltr %d, fltr result %d",
                            this, ppIIPFilterSpec, filtersLen, fi_result);

  if (0 == ppIIPFilterSpec || 0 >= filtersLen)
  {
    return QDS_EFAULT;
  }

  memset (&filterAddParam, 0, sizeof(filterAddParam));

  /*-------------------------------------------------------------------------
    Allocate memory to hold array to ip_filter_type
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC( filterAddParam.fi_ptr_arr,
                            sizeof (ip_filter_type) * filtersLen, 
                            void*);

  if  (0 == filterAddParam.fi_ptr_arr)
  {
    return AEE_ENOMEMORY;
  }

  filterAddParam.enable             = TRUE;
  filterAddParam.is_validated       = FALSE;
  filterAddParam.num_filters        = (uint8) filtersLen;
  filterAddParam.fi_result          = fi_result;
  filterAddParam.filter_type        = IPFLTR_DEFAULT_TYPE;
  filterAddParam.fltr_compare_f_ptr = NULL;

  for (index = 0; index < filtersLen; index++)
  {
    //TODO add validation
    (void) DS2PSIPFilterSpec
           (
             ppIIPFilterSpec[index],
             &(((ip_filter_type *) filterAddParam.fi_ptr_arr)[index])
           );
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      If filters are already installed, uninstall them.
    -----------------------------------------------------------------------*/
    DS_UTILS_RELEASEIF(mpIPFilterReg);

    ifaceHandle = GetHandle();
    if (AEE_SUCCESS != DS2PSIFaceNameType(iface_group, &ifaceGroup))
    {
      ds_net_error_code = DSS_ERROR_CONVERT_GROUP_FAILED;
      result = AEE_EFAILED;
      break;
    }
	
    if((ifaceHandle == 0) && (ifaceGroup == ANY_DEFAULT_GROUP)) 
    { 
      /* Iface went down state */ 
      ds_net_error_code = DSS_ERROR_INSTALL_FLTR_FAILED; 
      result = AEE_EFAILED; 
      break; 
    } 

    result = PSIfaceIPFilterAdd (ifaceHandle,
                                 ifaceGroup,
                                 IP_FLTR_CLIENT_SOCKETS,
                                 &filterAddParam,
                                 &fltrHandle);
    if (AEE_SUCCESS != result)
    {
      ds_net_error_code = DSS_ERROR_INSTALL_FLTR_FAILED;
      break;
    }

    //TODO Ideally the constructor shud have taken filters as param ->
    //CreateInstance()
    mpIPFilterReg = new IPFilterReg (ifaceHandle, fltrHandle);
    if (0 == mpIPFilterReg)
    {
      ds_net_error_code = DSS_ERROR_IPFILTERREG_ALLOC_FAILED;
      result = AEE_ENOMEMORY;
      break;
    }

    (void) mpIPFilterReg->AddRef();
    result = AEE_SUCCESS;

    *ppIIPFfilterReg = static_cast <IIPFilterRegPriv *> (mpIPFilterReg);

    LOG_MSG_FUNCTION_EXIT_2 ("Network::RegisterFilters(): "
                             "Fltr handle %d, obj 0x%p", fltrHandle, this);
  } while (0);

  /*-------------------------------------------------------------------------
    Error handling
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS != result)
  {
    LOG_MSG_FUNCTION_EXIT_3 ("Network::RegisterFilters(): "
                             "result 0x%x, error code %d, obj 0x%p",
                             result, ds_net_error_code, this);
    (void)
      PSIfaceIPFilterDelete (ifaceHandle, IP_FLTR_CLIENT_SOCKETS, fltrHandle);
  }

  PS_SYSTEM_HEAP_MEM_FREE (filterAddParam.fi_ptr_arr);

  mpICritSect->Leave();

  return result;
} /* Network::RegisterFilters() */

ds::ErrorType CDECL Network::GetQoSMode
(
  QoSModeType* qosMode
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    currently this API is not supported
  -------------------------------------------------------------------------*/
  return AEE_EUNSUPPORTED;

} /* Network::GetQoSMode() */

ds::ErrorType CDECL Network::GetDormancyInfoCode
(
  DormancyInfoCodeType *  pDormancyInfoCode
)
{
  PhysLink *     pPhysLink = NULL;
  IPhysLink *    pIPhysLink = NULL;
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Get Dormancy info code from phys link.
    -----------------------------------------------------------------------*/
    result = GetTXPhysLink (&pIPhysLink);
    if (AEE_SUCCESS != result)
    {
      break;
    }

    pPhysLink = reinterpret_cast <PhysLink *> (pIPhysLink);
    result    = pPhysLink->GetDormancyInfoCode (pDormancyInfoCode);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);

  DS_UTILS_RELEASEIF(pIPhysLink);
  mpICritSect->Leave();

  return result;
} /* Network::GetDormancyInfoCode() */

ds::ErrorType Network::GetSignalBus
(
  ds::Net::EventType  eventID,
  ISignalBus **       ppISigBus
)
{
  ds::ErrorType  result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  if (0 == ppISigBus)
  {
    return QDS_EFAULT;
  }

  *ppISigBus = NULL;

  mpICritSect->Enter();

  switch (eventID)
  {
    case NetworkEvent::QDS_EV_IFACE_UP:
    {
      *ppISigBus = mpSigBusIfaceUp;
      break;
    }

    case NetworkEvent::QDS_EV_IFACE_GOING_DOWN:
    {
      *ppISigBus = mpSigBusIfaceGoingDown;
      break;
    }

    case NetworkEvent::QDS_EV_IFACE_DOWN:
    {
      *ppISigBus = mpSigBusIfaceDown;
      break;
    }

    case NetworkEvent::QDS_EV_IFACE_ACTIVE_OUT_OF_USE:
    {
      *ppISigBus = mpSigBusIfaceActiveOutOfUse;
      break;
    }

    case NetworkEvent::QDS_EV_IFACE_COMING_UP:
    {
      *ppISigBus = mpSigBusIfaceComingUp;
      break;
    }

    case NetworkEvent::QDS_EV_IFACE_CONFIGURING:
    {
      *ppISigBus = mpSigBusIfaceConfiguring;
      break;
    }

    case NetworkEvent::QDS_EV_IFACE_ROUTEABLE:
    {
      *ppISigBus = mpSigBusIfaceRouteable;
      break;
    }

    default:
    {
      result = QDS_EINVAL;
      break;
    }
  }

  /*-------------------------------------------------------------------------
    Handle the case where event is supported by this object.
  -------------------------------------------------------------------------*/
  if (AEE_SUCCESS == result)
  {
    if (0 == *ppISigBus)
    {
      result = QDS_EINVAL;
    }
    else
    {
      result = AEE_SUCCESS;
      (void) (*ppISigBus)->AddRef();
    }
  }

  LOG_MSG_INFO3_4 ("Network::GetSignalBus(): "
                   "Result 0x%x, signal bus 0x%p, obj 0x%p, event 0x%x",
                   result, *ppISigBus, this, eventID);

  mpICritSect->Leave();
  return result;

} /* Network::GetSignalBus() */

ds::ErrorType Network::GoNull
(
  ReleaseReasonType  nullReason
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (GetHandle(),
                                 IFACE_IOCTL_GO_NULL,
                                 static_cast <void *> (&nullReason));

  LOG_MSG_FUNCTION_EXIT_3 ("Network::GoNull(): "
                           "Result 0x%x, obj 0x%p, GO_NULL reason %d",
                           result, this, nullReason);
  mpICritSect->Leave();

  return result;
} /* Network::GoNull() */


void Network::SetHandle
(
  int32  objHandle
)
{
  int32                                    ifaceHandle;
  NetPlatform::NetworkDownReasonType       lastNetDownReason;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_2 ("Network::SetHandle(): Obj 0x%p, handle 0x%x",
                            this, objHandle);

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Reset mBringupAgain, mBringupFirst, mTeardown flags.
    Cache the last network down reason.
  -------------------------------------------------------------------------*/
  if (0 == objHandle)
  {
    mBringupFirst = FALSE;
    mWouldblock   = FALSE;
    mBringupAgain = FALSE;
    mTeardown     = FALSE;

    ifaceHandle = GetHandle();
    if (0 != ifaceHandle)
    {
      NetPlatform::GetLastNetworkDownReason (ifaceHandle, &lastNetDownReason);
      mLastNetDownReason = (NetDownReasonType) lastNetDownReason;
    }
  }

  /*-------------------------------------------------------------------------
    Call parent method
  -------------------------------------------------------------------------*/
  Handle::SetHandle(objHandle);

  mpICritSect->Leave();
  return;

} /* Network::SetHandle() */


ds::ErrorType Network::SetFMCTunnelParams
(
  const FMCTunnelParamsType *  tunnelParams
)
{
  NetPlatform::PSFMCTunnelParamsType     psTunnelParams;
  ds::SockAddrInternalType               tempAddr;
  ds::SockAddrIN6InternalType            v6RemoteAddr;
  ds::ErrorType                          result;
  uint16                                 psFamily;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&psTunnelParams, 0, sizeof(NetPlatform::PSFMCTunnelParamsType));
  memset(&tempAddr, 0, sizeof(ds::SockAddrInternalType));
  memset(&v6RemoteAddr, 0, sizeof(ds::SockAddrIN6InternalType));

  /*-------------------------------------------------------------------------
    Convert to PS socket address
  -------------------------------------------------------------------------*/
  memscpy(&tempAddr,
    sizeof(ds::SockAddrStorageType),
    tunnelParams->tunnelEndPointAddr,
    sizeof(ds::SockAddrStorageType));

  tempAddr.v4.family = ps_ntohs(tempAddr.v4.family);

  switch (tempAddr.v4.family)
  {
    case AddrFamily::QDS_AF_INET:
    {
      result = DS2PSSockAddrFamily(tempAddr.v4.family, &psFamily);
      if (AEE_SUCCESS != result)
      {
        LOG_MSG_INVALID_INPUT_2("Network::SetFMCTunnelParams(): "
                                "Wrong addr family %d, obj 0x%p",
                                tempAddr.v4.family, this);
        return QDS_EINVAL;
      }

      tempAddr.v4.family                      = psFamily;
      psTunnelParams.tunnel_end_point_info_ptr =
        reinterpret_cast <ps_sockaddr *> (&tempAddr);

      break;
    }

    case AddrFamily::QDS_AF_INET6:
    {
      memscpy(&v6RemoteAddr,
        sizeof(ds::SockAddrIN6InternalType),
        tunnelParams->tunnelEndPointAddr,
        sizeof(ds::SockAddrIN6InternalType));

      v6RemoteAddr.family = ps_ntohs(v6RemoteAddr.family);

      result = DS2PSSockAddrFamily(v6RemoteAddr.family, &psFamily);
      if (AEE_SUCCESS != result)
      {
        LOG_MSG_INVALID_INPUT_2("Network::SetFMCTunnelParams(): "
                                "Wrong addr family %d, obj 0x%p",
                                v6RemoteAddr.family, this);
        return QDS_EINVAL;
      }

      v6RemoteAddr.family                      = psFamily;
      psTunnelParams.tunnel_end_point_info_ptr =
        reinterpret_cast <ps_sockaddr *> (&v6RemoteAddr);

      break;
    }

    case AddrFamily::QDS_AF_UNSPEC:
    default:
    {
      LOG_MSG_INVALID_INPUT_2("Network::SetFMCTunnelParams(): "
                              "Wrong addr family %d, obj 0x%p",
                              tempAddr.v4.family, this);
      return QDS_EINVAL;
    }
  }

  psTunnelParams.addr_len       = tunnelParams->addrLen;
  psTunnelParams.is_nat_present = tunnelParams->IsNatPresent;
  psTunnelParams.stream_id      = tunnelParams->streamId;

  mpICritSect->Enter();

  result = IfaceIoctlNonNullArg (GetHandle(),
                                 IFACE_IOCTL_UW_FMC_SET_TUNNEL_PARAMS,
                                 static_cast <void *> (&psTunnelParams));

  mpICritSect->Leave();
  return result;

} /* Network::SetFMCTunnelParams() */

ds::ErrorType Network::ResetFMCTunnelParams
(
  void
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  result = IfaceIoctl(GetHandle(),
                      IFACE_IOCTL_UW_FMC_RESET_TUNNEL_PARAMS,
                      NULL);

  mpICritSect->Leave();

  return result;
} /* Network::ResetFMCTunnelParams() */

ds::ErrorType Network::GetDeviceName
(
  DeviceName *  pDeviceName
)
{
  ds::ErrorType  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  result = IfaceIoctl(GetHandle(),
                      IFACE_IOCTL_GET_DEVICE_NAME,
                      static_cast <void *> (pDeviceName));

  mpICritSect->Leave();
  return result;

} /* Network::GetDeviceName() */

ds::ErrorType Network::GetGatewayAddress
(
  ds::IPAddrType *  argIPAddress
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return IPAddrIOCTL (IFACE_IOCTL_GET_GATEWAY_ADDR, argIPAddress);

} /* Network::GetGatewayAddress() */

ds::ErrorType Network::GetSubnetMask
(
  ds::IPAddrType *  pSubnetMask
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return IPAddrIOCTL (IFACE_IOCTL_GET_SUBNET_MASK, pSubnetMask);

} /* Network::GetSubnetMask */

boolean Network::EventIsNetworkState
(
  int32 eventId
)
{
  boolean ret = false;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (eventId)
  {
    case NetworkEvent::QDS_EV_IFACE_UP:
    case NetworkEvent::QDS_EV_IFACE_GOING_DOWN:
    case NetworkEvent::QDS_EV_IFACE_ENABLED:
    case NetworkEvent::QDS_EV_IFACE_DISABLED:
    case NetworkEvent::QDS_EV_IFACE_DOWN:
    case NetworkEvent::QDS_EV_IFACE_ACTIVE_OUT_OF_USE:
    case NetworkEvent::QDS_EV_IFACE_COMING_UP:
    case NetworkEvent::QDS_EV_IFACE_CONFIGURING:
    case NetworkEvent::QDS_EV_IFACE_ROUTEABLE:
      ret = true;
      break;
  }

  return ret;

} /* Network::EventIsNetworkState */

EventType
Network::NetworkState2Event
(
  NetworkStateType argNetState
)
{
  int32 ret = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch (argNetState)
  {
    case NetworkState::QDS_CLOSED:
    case NetworkState::QDS_LINGERING:
      ret = NetworkEvent::QDS_EV_IFACE_DOWN;
      break;

    case NetworkState::QDS_OPEN_IN_PROGRESS:
      ret = NetworkEvent::QDS_EV_IFACE_COMING_UP;
      break;

    case NetworkState::QDS_OPEN:
      ret = NetworkEvent::QDS_EV_IFACE_UP;
      break;

    case NetworkState::QDS_CLOSE_IN_PROGRESS:
      ret = NetworkEvent::QDS_EV_IFACE_GOING_DOWN;
      break;

    default:
      LOG_MSG_ERROR_2 ("Network::NetworkState2Event(): "
                       "network state %d unsupported, obj 0x%p",
                       argNetState, this);
  }

  return ret;

} /* Network::NetworkState2Event */

ds::ErrorType Network::GetExtIpv6DelAddr
(
  ds::INAddr6Type value
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  memscpy ( (void *)value, sizeof(ds::INAddr6Type),
           (const void *) mDeletedIPv6Addr, 
           sizeof(ds::INAddr6Type));

  mpICritSect->Leave();
  
  return AEE_SUCCESS;

}

ds::ErrorType Network::GetExtIPConfigEx
(
 ::ds::Net::NetworkExtendedIpConfigExType* value
 )
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  mpICritSect->Enter();

  *value = mCachedExtendedMask;

  mpICritSect->Leave();

  return AEE_SUCCESS;

}

ds::ErrorType Network::GetApnParamInfo
(
 ApnParamChangeList* pGetApnParamInfo
)
{
  ds::ErrorType result = AEE_EFAILED;
  bool          getApnParamInfoFailed = false;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   if (0 == pGetApnParamInfo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to get data path
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_GET_CHANGED_APN_PARAM_INFO,
                       static_cast <void *> (pGetApnParamInfo));

  if (AEE_SUCCESS != result)
  {
    getApnParamInfoFailed = true;
    goto bail;
  }

  /* Fall through */

bail:
  LOG_MSG_FUNCTION_EXIT_3 ("Network::GetApnParamInfo(): "
                           "Result 0x%x, getApnParamInfo failed %d;, obj 0x%p",
                           result, getApnParamInfoFailed, this);
  mpICritSect->Leave();

  return result;
}

ds::ErrorType Network::GetMipInfo
(
  ds::IPAddrType* pMipInfo
)
{
  ds::ErrorType  result  = AEE_SUCCESS;
  ps_iface_ioctl_mip_ma_info_type psMipInfo;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  // Verify that input params are non-NULL
  if (0 == pMipInfo)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Get the iface handle and call the relevant IOCTL
    -----------------------------------------------------------------------*/
    memset (&psMipInfo, 0, sizeof(psMipInfo));

    result = IfaceIoctl (GetHandle(),
                         IFACE_IOCTL_707_GET_MIP_MA_INFO,
                         static_cast <void*> (&psMipInfo));

    if (AEE_SUCCESS != result)
    {
      break;
    }

    /*-----------------------------------------------------------------------
      Convert IOCTL result back to ds::IPAddrType
    -----------------------------------------------------------------------*/
    result = PS2DSIPAddr (&psMipInfo.fa_addr, pMipInfo);
    if (AEE_SUCCESS != result)
    {
      break;
    }
  } while (0);

  mpICritSect->Leave();
  return result;
} /* Network::GetMipInfo() */

ds::ErrorType Network::GetHandoffInfo
(
 HandoffinfoType *value
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  *value = mcachedHandoffinfo;

  mpICritSect->Leave();

  return AEE_SUCCESS;

}


ds::ErrorType Network::GetHandoffFailureInfo
(
 HandoffFailureinfoType *value
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();

  *value = mcachedHandoffFailureinfo;

  mpICritSect->Leave();

  return AEE_SUCCESS;

}

ds::ErrorType Network::GetDpdPingStat
(
  DpdPingStatInfoType* value
 )
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_1 ("Network::GetDpdPingStat(): Obj 0x%p", this);

  mpICritSect->Enter();

  *value = mcachedDpdPingStatInfo;

  LOG_MSG_FUNCTION_EXIT_1 ("Network::GetDpdPingStat(): , obj 0x%p", this);

  mpICritSect->Leave();

  return AEE_SUCCESS;

}


ds::ErrorType Network::CreateNetRanAsstManager
(
  IRanAsstMediaAdaptManager**  ppIRanAsstManager
)
{
  ds::ErrorType                result      = 0;
  int32                        ifaceHandle = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == ppIRanAsstManager)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  do
  {
    /*-----------------------------------------------------------------------
      Create a new RAN Asst req object all the time.
    -----------------------------------------------------------------------*/
    ifaceHandle = GetHandle();
    if (0 == ifaceHandle)
    {
      result = QDS_EINVAL;
      break;
    }

    /*-----------------------------------------------------------------------
      Create a new RanAsstMediaAdaptManager object all the time. If the network
      object's handle changes anytime, old FirewallManager does not make sense.
    -----------------------------------------------------------------------*/
    *ppIRanAsstManager =
      static_cast <IRanAsstMediaAdaptManager *> (new RanAsstMediaAdaptManager(ifaceHandle));
    if (0 == *ppIRanAsstManager)
    {
      result = AEE_ENOMEMORY;
      break;
    }

    result = AEE_SUCCESS;
  } while (0);

  mpICritSect->Leave();

  LOG_MSG_FUNCTION_EXIT_4 ("Network::CreateNetRanAsstManager(): "
                           "Result 0x%x, ppIRanAsst 0x%p,  "
                           "obj 0x%p iface handler %p", result, *ppIRanAsstManager,
                            this, ifaceHandle);
  return result;
} /* Network::CreateNetRanAsstManager() */

ds::ErrorType Network::InitDPDcheck(void)
{
  ds::ErrorType result = AEE_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   mpICritSect->Enter();

  /*-------------------------------------------------------------------------
    Call the iface IOCTL to initiate DPD check
  -------------------------------------------------------------------------*/
  result = IfaceIoctl (GetHandle(),
                       IFACE_IOCTL_WLAN_INITIATE_DPD_CHECK,
                       NULL);

  LOG_MSG_FUNCTION_EXIT_2 ("Network::InitDPDcheck(): "
                           "Result 0x%x obj 0x%p",result, this);
  mpICritSect->Leave();

  return result;
} /* Network::InitDPDcheck() */

ds::ErrorType Network::IsDefaultPhysLink
(
  int32    physLinkHandle,
  boolean *isDefaultPhysLink
)
{
  ds::ErrorType result = AEE_EFAILED;
  int32        defaultPhysLinkHandle = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == isDefaultPhysLink)
  {
    return result;
  }

  mpICritSect->Enter();

  if (AEE_SUCCESS == NetPlatform::PSGetPhysLinkFromIface(GetHandle(),
                       &defaultPhysLinkHandle))
  {
    result = AEE_SUCCESS;
    if(physLinkHandle == defaultPhysLinkHandle)
    {
      *isDefaultPhysLink = TRUE;
    }
    else
    {
      *isDefaultPhysLink = FALSE;
    }
  }  

  LOG_MSG_FUNCTION_EXIT_4 ("Network::IsDefautPhysLink(): "
                           "Result 0x%x obj 0x%p physLinkHandle 0x%x 0x%x", result, this,
                           physLinkHandle, defaultPhysLinkHandle);
  mpICritSect->Leave();

  return result;
} /* Network::InitDPDcheck() */


