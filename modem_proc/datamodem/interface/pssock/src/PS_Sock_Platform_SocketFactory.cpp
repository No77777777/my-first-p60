/*===========================================================================
  FILE: PS_Sock_Platform_SocketFactory.cpp

  OVERVIEW: This file implements PS::Sock::Platform::SocketFactory class.

  DEPENDENCIES: None

  Copyright (c) 2008-2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
===========================================================================*/

/*===========================================================================
  EDIT HISTORY FOR MODULE

  Please notice that the changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_Platform_SocketFactory.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $

  when       who what, where, why
  ---------- --- ------------------------------------------------------------
  2013-09-10 kr  Update with port reservation changes for ePDG
  2011-11-01 am  Moved eph port to start from NAT reserved end range.
  2008-05-14 msr Created module

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"

#include "ps_in.h"
#include "PS_Sock_Platform_SocketFactory.h"
#include "PS_Sock_Platform_Socket.h"
#include "ds_Utils_DebugMsg.h"
#include "ds_Utils_List.h"
#include "ps_mem.h"
#include "stringl.h"
#include "PS_Sock_RemoteSocket_Manager.h"
#include "dss_errors_def.h"

using namespace ds::Error;
using namespace ds::Sock;
using namespace ds::Utils;
using namespace PS::Sock::Platform;
using namespace PS::Sock::ProtocolStack;
using namespace dss::Error;
using namespace PS::Sock::RemoteSocket;

SocketFactory * SocketFactory::instance = 0;

/*===========================================================================

                          CONSTRUCTOR / DESTRUCTOR

===========================================================================*/
SocketFactory::SocketFactory(void)
throw()
: platformDisabledEventMask(0),
  refCnt(0)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_1 ("SocketFactory::SocketFactory(): "
                   "Creating SocketFactory 0x%p", this);

} /* SocketFactory::SocketFactory() */


SocketFactory::~SocketFactory(void)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_1 ("SocketFactory::~SocketFactory(): "
                   "Deleting SocketFactory 0x%p", this);
  
  instance = 0;

} /* SocketFactory::~SocketFactory() */

/*===========================================================================

                         PUBLIC MEMBER FUNCTIONS

===========================================================================*/
SocketFactory* SocketFactory::CreateInstance(void)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Allocate a SocketFactory object if it is not already allocated.

    Since factory pattern is used, a new object is not allocated each time
    CreateInstance() is called
  -------------------------------------------------------------------------*/
  if (0 == instance) {
    instance = new SocketFactory();
    if (0 == instance) {
      DS_UTILS_ERROR_FATAL("SocketFactory::CreateInstance(): No mem for SocketFactory");
      return 0;
    }
    instance->SetDefaultEphemeralCacheHandle(UINT32_MAX);
  }
  
  (void) instance->AddRef();

  return instance;

} /* SocketFactory::Instance() */


/*===========================================================================

                         PUBLIC MEMBER FUNCTIONS

===========================================================================*/
Socket * SocketFactory::CreateSocket
(
  ds::AddrFamilyType   family,
  SocketType           type,
  ProtocolType         protocol,
  ICritSect *          critSectPtr,
  ds::ErrorType *      dsErrnoPtr
)
{
  Socket  *                                  platformSockPtr;
  PS::Sock::ProtocolStack::IEventListener *  eventListenerPtr;
  dss::Error::dss_error_enum_type ps_sock_error_code = DSS_ERROR_MIN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


  platformSockPtr =
    Socket::CreateInstance( family, type, protocol, critSectPtr, dsErrnoPtr);
  if (0 == platformSockPtr)
  {
    ps_sock_error_code = DSS_ERROR_CREATE_INSTANCE_FAILED;
    goto bail;
  }

  /*-------------------------------------------------------------------------
    Post PLATFORM_DISABLED event to the newly created socket if the platform
    is disabled
  -------------------------------------------------------------------------*/
  if (0 != platformDisabledEventMask)
  {
    eventListenerPtr = platformSockPtr->GetIEventListener();
    if (0 == eventListenerPtr)
    {
      *dsErrnoPtr = QDS_EINVAL;
      ps_sock_error_code = DSS_ERROR_NULL_ADDRESS;
      goto bail;
    }

    eventListenerPtr->ProcessPlatformDisabledEvent();
  }

  /*-------------------------------------------------------------------------
    Add Socket to Factory
  -------------------------------------------------------------------------*/
  (void) AddItem( static_cast <ds::Utils::INode *> ( platformSockPtr));

  return platformSockPtr;

bail:
  LOG_MSG_ERROR_2("SocketFactory::CreateSocket(): "
                  "sock 0x%x error code %d", 
                  platformSockPtr, ps_sock_error_code);
  if (0 != platformSockPtr)
  {
    (void) platformSockPtr->Close();
  }

  return 0;

} /* SocketFactory::CreateSocket() */


void SocketFactory::DeleteSocket
(
  Socket *  sockPtr
)
throw()
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == sockPtr)
  {
    DS_UTILS_ERROR_FATAL("SocketFactory::DeleteSocket(): NULL arg");
    return;
  }

  /*-------------------------------------------------------------------------
    Remove object from the list of socket objects
  -------------------------------------------------------------------------*/
  RemoveItem( static_cast <INode *> ( sockPtr));
  return;

} /* SocketFactory::DeleteSocket() */


bool SocketFactory::IsSockAddrInUse
(
  ProtocolType                 protocol,
  const ds::SockAddrIN6InternalType*   v6LocalAddrPtr,
  bool                         reUseAddr,
  bool                         isEphemeralPort
)
{
  Socket *  platformSockPtr;
  bool      addrInUse = false;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (0 == v6LocalAddrPtr)
  {
    DS_UTILS_ERROR_FATAL("SocketFactory::IsSockAddrInUse(): NULL addr");
    return true;
  }

  /*-------------------------------------------------------------------------
    It is invalid for port to be 0. When port is 0, ephemeral port needs to be
    generated and there is no need to check if address is in use
  -------------------------------------------------------------------------*/
  if (0 == v6LocalAddrPtr->port)
  {
    DS_UTILS_ERROR_FATAL("SocketFactory::IsSockAddrInUse(): 0 port");
    return true;
  }

  /*-------------------------------------------------------------------------
    Iterate through the list of platform sockets and check if addr is already
    in use
  -------------------------------------------------------------------------*/
  for (First(); !IsDone(); Next())
  {
    platformSockPtr = static_cast <Socket *> ( CurrentItem());
    if (0 == platformSockPtr)
    {
      LOG_MSG_ERROR_0("SocketFactory::IsSockAddrInUse(): "
                      "NULL sock");
      continue;
    }

    /* Obtain strong ref to item before performing any processing */
    if (TRUE == platformSockPtr->GetStrongRef())
    {
      addrInUse =
        platformSockPtr->IsSockAddrInUse (protocol, v6LocalAddrPtr, reUseAddr, isEphemeralPort);

      (void) platformSockPtr->Release();
            
      if (true == addrInUse)
      {
        break;
      }
    }
  }

  LOG_MSG_FUNCTION_EXIT_2("SocketFactory::IsSockAddrInUse(): "
                          "Returning %d for Proto %d", addrInUse, protocol);
  return addrInUse;

} /* SocketFactory::IsSockAddrInUse() */

void SocketFactory::ProcessEvent
(
  GlobalEventType  globalEvent
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_FUNCTION_ENTRY_1("SocketFactory::ProcessEvent(): "
                           "Processing ev %d", globalEvent);

  switch (globalEvent)
  {
    case GlobalEvent::PS_MEM_BUF_AVAILABLE:
    {
      ResetPlatformDisabledEventMask( PlatformDisabledEventBitMask::PS_MEM);
      break;
    }

    case GlobalEvent::PS_MEM_BUF_NOT_AVAILABLE:
    {
      SetPlatformDisabledEventMask( PlatformDisabledEventBitMask::PS_MEM);
      break;
    }

    case GlobalEvent::DSM_ITEM_AVAILABLE:
    {
      ResetPlatformDisabledEventMask( PlatformDisabledEventBitMask::DSM);
      break;
    }

    case GlobalEvent::DSM_ITEM_NOT_AVAILABLE:
    {
      SetPlatformDisabledEventMask( PlatformDisabledEventBitMask::DSM);
      break;
    }

    default:
    {
      DS_UTILS_ASSERT( 0);
      break;
    }
  }

  return;

} /* SocketFactory::ProcessEvent() */


/*===========================================================================

                         PRIVATE MEMBER FUNCTIONS

===========================================================================*/
void SocketFactory::SetPlatformDisabledEventMask
(
  PlatformDisabledEventBitMaskType  eventBitMask
)
{
  Socket *                                   platformSockPtr;
  PS::Sock::ProtocolStack::IEventListener *  eventListenerPtr;
  PlatformDisabledEventBitMaskType           tmpBitMask;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  tmpBitMask                 = platformDisabledEventMask;
  platformDisabledEventMask |= eventBitMask;

  LOG_MSG_INFO1_3("SocketFactory::SetPlatformDisabledEventMask(): "
                  "Disabled ev mask %d Setting ev mask %d prev bit mask %d",
                   platformDisabledEventMask, eventBitMask, tmpBitMask);

  /*-------------------------------------------------------------------------
    Iterate through the list of platform sockets and post PLATFORM_DISABLED
    event on each socket on transition from 0 to non-0
  -------------------------------------------------------------------------*/
  if (0 == tmpBitMask)
  {
    for (First(); !IsDone(); Next())
    {
      platformSockPtr = static_cast <Socket *> ( CurrentItem());
      if (0 == platformSockPtr)
      {
        LOG_MSG_ERROR_0("SocketFactory::SetPlatformDisabledEventMask(): "
                        "NULL sock");
        continue;
      }

      /* Obtain strong ref to item before performing any processing */
      if (TRUE == platformSockPtr->GetStrongRef())
      {
        eventListenerPtr = platformSockPtr->GetIEventListener();
        if (0 == eventListenerPtr)
        {
          DS_UTILS_ASSERT( 0);
          return;
        }

        eventListenerPtr->ProcessPlatformDisabledEvent();
        (void) platformSockPtr->Release();
      }

    }
  }
} /* SocketFactory::SetPlatformDisabledEventMask() */


void SocketFactory::ResetPlatformDisabledEventMask
(
  PlatformDisabledEventBitMaskType  eventBitMask
)
{
  Socket *                                   platformSockPtr;
  PS::Sock::ProtocolStack::IEventListener *  eventListenerPtr;
  PlatformDisabledEventBitMaskType           tmpBitMask;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  tmpBitMask                 = platformDisabledEventMask;
  platformDisabledEventMask &= ~eventBitMask;

  LOG_MSG_INFO1_3("SocketFactory::ResetPlatformDisabledEventMask(): "
                  "Disabled ev mask %d Resetting ev mask %d prev bit mask %d",
                   platformDisabledEventMask, eventBitMask, tmpBitMask);

  /*-------------------------------------------------------------------------
    Iterate through the list of platform sockets and post PLATFORM_ENABLED
    event on each socket if platform is not disabled any more

    Event is posted only on transition from non-0 to 0 so that event is not
    spuriously posted when for example PS_MEM wasn't disabled previously but
    PS_MEM_BUF_AVAILABLE event is posted to socket platform
  -------------------------------------------------------------------------*/
  if (0 == platformDisabledEventMask && 0 != tmpBitMask)
  {

    for (First(); !IsDone(); Next())
    {
      platformSockPtr = static_cast <Socket *> ( CurrentItem());
      if (0 == platformSockPtr)
      {
        LOG_MSG_ERROR_0("SocketFactory::ResetPlatformDisabledEventMask(): "
                        "NULL sock");
        continue;
      }

      /* Obtain strong ref to item before performing any processing */
      if (TRUE == platformSockPtr->GetStrongRef())
      {
      
        eventListenerPtr = platformSockPtr->GetIEventListener();
        if (0 == eventListenerPtr)
        {
          DS_UTILS_ASSERT( 0);
          return;
        }

        eventListenerPtr->ProcessPlatformEnabledEvent();
      
        (void) platformSockPtr->Release();
      }

    }
  }

  return;

} /* SocketFactory::ResetPlatformDisabledEventMask() */

void SocketFactory::SetDefaultEphemeralCacheHandle
(
  uint32  handle
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  defaultEphemeralCacheHandle = handle;
}/*SocketFactory::SetDefaultEphemeralCacheHandle*/

uint32 SocketFactory::GetDefaultEphemeralCacheHandle
(
  void
) 
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return defaultEphemeralCacheHandle;
}/*SocketFactory::GetDefaultEphemeralCacheHandle*/

void SocketFactory::ReserveDefaultEphemeralPorts
(
  void
)
{
  PortPreference            socketPref;
  Manager *                      remoteSockManagerPtr  = NULL;
  int32                          resultHandle = 0;
  int                            numOfSockets = (MAX_SOCKETS / 4);
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( UINT32_MAX == GetDefaultEphemeralCacheHandle())
  {
    remoteSockManagerPtr = PS::Sock::RemoteSocket::Manager::GetInstance(); 
    remoteSockManagerPtr->InitSocketsPref(&socketPref);
    socketPref.pref_count  = numOfSockets;       
    resultHandle = remoteSockManagerPtr->RequestSocketsPref(&socketPref ,
                        SocketFactory::ReserveDefaultEphemeralPortsCB, NULL);

    if ( 0 != resultHandle &&  -1 != resultHandle)
    {
      SetDefaultEphemeralCacheHandle(resultHandle);
    }
  }
   LOG_MSG_INFO1_2("SocketFactory::ReserveDefaultEphemeralPorts(): "
                   " Port reserve request result %u  CacheHandle %u",
                     resultHandle,
                     GetDefaultEphemeralCacheHandle());

}/*ReserveDefaultEphemeralPorts */


void SocketFactory::ReserveDefaultEphemeralPortsCB
(
  int8   requestStatus,      
  int32   cacheHandle,  
  void *  userData     
)
{
  SocketFactory                            * platformSockFactoryPtr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  platformSockFactoryPtr = SocketFactory::CreateInstance();
  DS_UTILS_ASSERT( 0 != platformSockFactoryPtr);
  
  if (0 == requestStatus)
  {
    platformSockFactoryPtr->SetDefaultEphemeralCacheHandle(cacheHandle);
  }
  DS_UTILS_RELEASEIF(platformSockFactoryPtr);
  
  LOG_MSG_INFO1_2("SocketFactory::EphemeralPortsCB(): "
                  "status  %u  CacheHandle %u",
                   requestStatus,
                   cacheHandle);
  
}/*SocketFactory::ReserveDefaultEphemeralPortsCB*/

