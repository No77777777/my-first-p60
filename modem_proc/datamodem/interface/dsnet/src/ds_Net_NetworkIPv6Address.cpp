/*===========================================================================
  FILE: NetworkIPv6Address.cpp

  OVERVIEW: This file provides implementation of the NetworkIPv6Address class

  DEPENDENCIES: None

  Copyright (c) 2009 - 2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dsnet/src/ds_Net_NetworkIPv6Address.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2011-09-30 kr  Free floating changes
  2009-02-27 hm  Created module.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include <stringl/stringl.h>
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_StdErr.h"
#include "ds_Net_NetworkIPv6Address.h"
#include "ds_Net_Conversion.h"
#include "ds_Net_MemManager.h"
#include "ds_Net_EventDefs.h"
#include "ds_Net_EventManager.h"
#include "ds_Net_Platform.h"
#include "ds_Utils_CreateInstance.h"
#include "ds_Utils_CSignalBus.h"

using namespace ds::Net::Conversion;
using namespace ds::Net;
using namespace ds::Error;
using namespace NetPlatform;

#define DS_NET_IPV6ADDR_ASSERT(expression) \
         dsnet_ipv6addr_assert_wrapper(__LINE__, expression)

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
static void dsnet_ipv6addr_assert_wrapper
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

                     PUBLIC FUNCTION DEFINITIONS

===========================================================================*/
/*---------------------------------------------------------------------------
  CONSTRUCTOR/DESTRUCTOR
---------------------------------------------------------------------------*/

NetworkIPv6Address::NetworkIPv6Address
(
  const ds::INAddr6Type      ipv6Addr,
  IPv6AddrStateType          ipv6AddrState
)
: Handle (),
  mState (ipv6AddrState),
  mpSigBusStateChange (0),
  refCnt (1),
  weakRefCnt (1)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_2 ("NetworkIPv6Address::NetworkIPv6Address(): "
                   "Object 0x%p, state %d", this, mState);

  (void) memscpy (mIPv6Addr, sizeof (ds::INAddr6Type),
    ipv6Addr, sizeof (ds::INAddr6Type));

} /* NetworkIPv6Address() */

ds::ErrorType NetworkIPv6Address::Init
(
  int32 ifaceHandle
)
{
  int32  result;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_2 ("NetworkIPv6Address::Init(): "
                   "Init object 0x%p ifaceHandle 0x%x", this, ifaceHandle);
  
  if (0 == ifaceHandle)
  {
    return QDS_EFAULT;
  }


  result = DS_Utils_CreateInstance(0, AEECLSID_CSignalBus,
                                   (void **) &mpSigBusStateChange);
  if (AEE_SUCCESS != result)
  {
    LOG_MSG_ERROR_1 ("NetworkIPv6Address::Init() :"
                     "Failed to init the signalBus, obj 0x%p", this);
    return result;
  }

  result = Handle::Init(EventManager::networkIPv6ObjList);
  if (AEE_SUCCESS != result)
  {
    LOG_MSG_ERROR_1 ("NetworkIPv6Address::Init() :"
                     "Failed to init the Handle, obj 0x%p", this);
    return result;
  }

  SetHandle (ifaceHandle);

  return result;

} /* NetworkIPv6Address::Init() */


void NetworkIPv6Address::Destructor
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_UTILS_RELEASEIF (mpSigBusStateChange);

  //Should be last statement. Call destructor for the base class.
  Handle::Destructor();

} /* NetworkIPv6Address::Destructor() */


NetworkIPv6Address::~NetworkIPv6Address
(
  void
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  //NO-OP: only used for freeing memory.

} /* NetworkIPv6Address::~NetworkIPv6Address() */


/*---------------------------------------------------------------------------
  Handle class forwarders.
---------------------------------------------------------------------------*/
ds::ErrorType NetworkIPv6Address::OnStateChange
(
  ISignal*                                signalObj,
  ds::Net::EventType                      eventID,
  IQI**                                   regObj
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_3  ("NetworkIPv6Address::OnStateChange(): "
                    "Obj 0x%p, ev %d, sig 0x%p", this, eventID, signalObj);

  if (IPv6AddrEvent::QDS_EV_STATE_CHANGED == eventID)
  {
    return Handle::OnStateChange (signalObj, eventID, regObj);
  }

  return QDS_EOPNOTSUPP;

} /* OnStateChange() */


/*---------------------------------------------------------------------------
  INode interface Methods
---------------------------------------------------------------------------*/
boolean NetworkIPv6Address::Process
(
  void*                                   userDataPtr
)
{
  EventInfoType*                          eventInfoPtr;
  IPv6PrivAddrEventInfoType*              pIPv6PrivAddrEventInfo = NULL;
  int32                                   ifaceHandle;
  unsigned int                            tempAddr[sizeof(ds::INAddr6Type) / sizeof(unsigned int)];
  unsigned int                            ps_in6addr_any_uint[sizeof(ds::INAddr6Type) / sizeof(unsigned int)];
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ifaceHandle = GetHandle();

  LOG_MSG_FUNCTION_ENTRY_3  ("NetworkIPv6Address::Process(): "
                             "Obj 0x%p, if handle 0x%x, user data 0x%x",
                             this, ifaceHandle, userDataPtr);
  memset((void *)&tempAddr, 0, sizeof(ds::INAddr6Type));
  memscpy((void *)&ps_in6addr_any_uint, sizeof(ds::INAddr6Type),
    (void*) &ps_in6addr_any, sizeof(ds::INAddr6Type));

  if (NULL == userDataPtr)
  {
    return FALSE;
  }

  if (0 == ifaceHandle)
  {
    DS_UTILS_ERROR_FATAL("NetworkIPv6Address::Process(): NULL iface handle");
    return FALSE;
  }

  eventInfoPtr = static_cast <EventInfoType *>(userDataPtr);

  LOG_MSG_INFO1_3 ("NetworkIPv6Address::Process(): "
                   "ev name %d, ev handle 0x%x, this if handle 0x%x",
                   eventInfoPtr->eventName, eventInfoPtr->handle, ifaceHandle);

  /*-------------------------------------------------------------------------
    Processing for IPv6AddrEvent::STATE_CHANGED.
  -------------------------------------------------------------------------*/
  if (IPv6AddrEvent::QDS_EV_STATE_CHANGED == eventInfoPtr->eventName)
  {
    pIPv6PrivAddrEventInfo =
      (IPv6PrivAddrEventInfoType *) eventInfoPtr->psEventInfo;
    if (NULL == pIPv6PrivAddrEventInfo)
    {
      return FALSE;
    }

    /*-----------------------------------------------------------------------
      Process this event:
        1. For tentative objects (object's state is PRIV_ADDR_WAITING):
           a. This object's address is IN6_ANY_ADDR
           b. This object's handle matches the event handle.
           c. The event is IFACE_IPV6_PRIV_ADDR_GENERATED_EV.
        2. For other objects
           a. It belongs to current IPv6 address object
      Processing of this event is same for shared and unique IPv6 addresses.
    -----------------------------------------------------------------------*/
    if (IPv6AddrState::PRIV_ADDR_WAITING == mState)
    {
      memscpy((void *)tempAddr, sizeof(ds::INAddr6Type),
        (const void *)mIPv6Addr, sizeof(ds::INAddr6Type));

      if((uint32)IFACE_IPV6_PRIV_ADDR_GENERATED_EV ==
          eventInfoPtr->psEventName                &&
          eventInfoPtr->handle == ifaceHandle      &&
          PS_IN6_ARE_ADDR_EQUAL (tempAddr, &ps_in6addr_any_uint))
      {

        /*-------------------------------------------------------------------
          Update the state of object before notifying.
        -------------------------------------------------------------------*/
        mpICritSect->Enter();
        mState = IPv6AddrState::PRIV_ADDR_AVAILABLE;
        (void) memscpy(mIPv6Addr, sizeof (ds::INAddr6Type),
          &pIPv6PrivAddrEventInfo->ip_addr.addr.v6,
          sizeof (ds::INAddr6Type));
        mpICritSect->Leave();
        Notify (eventInfoPtr->eventName);
      }

      /*---------------------------------------------------------------------
        Update the state of object before notifying.
      ---------------------------------------------------------------------*/
      /* This address object has consumed the event, stop traversal */
      return FALSE;
    } /* if (IPv6AddrState::PRIV_ADDR_WAITING == mState) */

    /*-----------------------------------------------------------------------
      The event and handle should match to continue processing
    -----------------------------------------------------------------------*/
    mpICritSect->Enter();
    memscpy((void *)tempAddr, sizeof(ds::INAddr6Type),
      (const void *)mIPv6Addr, sizeof(ds::INAddr6Type));
    if (eventInfoPtr->handle != ifaceHandle ||
        !PS_IN6_ARE_ADDR_EQUAL (&pIPv6PrivAddrEventInfo->ip_addr.addr.v6,
                                tempAddr))
    {
      mpICritSect->Leave();
      return TRUE;
    }

    switch (eventInfoPtr->psEventName)
    {
      case IFACE_IPV6_PRIV_ADDR_GENERATED_EV:
        /* Do not notify, as this event is valid for only TENTATIVE objs */
	mpICritSect->Leave();
        return TRUE;

      case IFACE_IPV6_PRIV_ADDR_DEPRECATED_EV:
        mState = IPv6AddrState::PRIV_ADDR_DEPRECATED;
        break;

      case IFACE_IPV6_PRIV_ADDR_EXPIRED_EV:
        mState = IPv6AddrState::PRIV_ADDR_WAITING;
        break;

      case IFACE_IPV6_PRIV_ADDR_DELETED_EV:
        mState = IPv6AddrState::PRIV_ADDR_DELETED;
        break;

      default:
        mpICritSect->Leave();
        DS_NET_IPV6ADDR_ASSERT (0);
        return FALSE;
    } /* switch */

    mpICritSect->Leave();
    /*-------------------------------------------------------------------------
      Notify the app.
    -------------------------------------------------------------------------*/
    (void) Notify (eventInfoPtr->eventName);

  } /* STATE_CHANGED event */


  return FALSE;
} /* Process() */

ds::ErrorType NetworkIPv6Address::GetAddress
(
  ds::INAddr6Type ipv6Addr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  mpICritSect->Enter();
  memscpy (ipv6Addr, sizeof (ds::INAddr6Type),
    mIPv6Addr, sizeof (ds::INAddr6Type));
  mpICritSect->Leave();

  return AEE_SUCCESS;

} /* GetAddress() */

ds::ErrorType NetworkIPv6Address::GetState
(
  IPv6AddrStateType* pState
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == pState)
  {
    return QDS_EFAULT;
  }

  mpICritSect->Enter();
  *pState = mState;
  mpICritSect->Leave();

  return AEE_SUCCESS;

} /* GetState() */


ds::ErrorType NetworkIPv6Address::GetSignalBus
(
  ds::Net::EventType  eventID,
  ISignalBus **       ppISigBus
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == ppISigBus)
  {
    DS_UTILS_ERROR_FATAL("NetworkIPv6Address::GetSignalBus(): NULL args");
    return QDS_EFAULT;
  }

  mpICritSect->Enter();

  switch (eventID)
  {
    case  IPv6AddrEvent::QDS_EV_STATE_CHANGED:
      *ppISigBus = mpSigBusStateChange;
      (void)(*ppISigBus)->AddRef();
      mpICritSect->Leave();
      return AEE_SUCCESS;

    default:
      *ppISigBus = NULL;
      mpICritSect->Leave();
      return QDS_EINVAL;
  }

} /* GetSignalBus() */

