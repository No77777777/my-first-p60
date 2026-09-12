/*===========================================================================
               PS _ SOCK _ REMOTESOCKET _ CACHE . CPP

DESCRIPTION

  PS SOCK REMOTESOCKET CACHE - Implementation.

FUNCTIONS

Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_RemoteSocket_Cache.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/03/15    ad     Initial version 
===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/

#include <stringl/stringl.h>
#include "ds_Utils_DebugMsg.h"
#include "ps_system_heap.h"
#include "ds_Utils_CCritSect.h"
#include "ds_Utils_CreateInstance.h"
#include "PS_Sock_RemoteSocket_Manager.h"
#include "PS_Sock_RemoteSocket_Cache.h"
#include "ps_crit_sect.h"
#include "ds_Addr_Def.h"
#include "ds_Sock_Def.h"

using namespace PS::Sock::RemoteSocket;
using namespace ds::Error;
using namespace ds::Utils;
/*===========================================================================

                         PUBLIC CLASS FUNCTIONS

===========================================================================*/

void SocketCache::Init(void)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  if (AEE_SUCCESS != DS_Utils_CreateInstance (NULL,
                                                AEECLSID_CCritSect,
                                                (void **) &(critSectPtr)))
  {
    DS_UTILS_ERROR_FATAL("SocketCache::Init(): Cannot create crit sect");
  }
  state = SOCKETS_CACHE_INIT;
  
} /* SocketCache::Init */

CacheState SocketCache::GetCacheState()
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  return state;
}/*SocketCache::GetCacheState*/

void SocketCache::SetCacheState
(
  CacheState newState
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
   state =  newState;
}/*SocketCache::SetCacheState*/

void SocketCache::ReleaseAllSockets
(
  ds::Utils::List            *sockCache,
  ps_sys_remote_socket_release_list_type *sockReleaseList
)
{
  SockInfo        * sockInfoPtr  = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  
  /*-------------------------------------------------------------------------
   Remove all sockets from the list and Fill handles into remote release list
  -------------------------------------------------------------------------*/ 
  while (sockCache->Count() > 0)
  {
     sockInfoPtr = (SockInfo *)sockCache->PopFront();
     
     if ( NULL != sockInfoPtr && NULL != sockReleaseList && UINT32_MAX != sockInfoPtr->GetHandle() 
          && sockReleaseList->num_sockets != PS_SYS_MAX_REMOTE_SOCKETS )
     {
       sockReleaseList->socket_list[sockReleaseList->num_sockets++] = sockInfoPtr->GetHandle();
     }
     
    delete sockInfoPtr;
  }
}/*SocketCache::DeInitCache*/


void SocketCache::TransferPartialAssignSockets
(
  ds::Utils::List            *destSockCache,
  ds::Utils::List            *srcSockCache
)
{

  SockInfo        * sockInfoPtr  = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 

  if( NULL == destSockCache || NULL == srcSockCache )
  {
    return;
  }
  /*-------------------------------------------------------------------------
   Transfer all sockets from source cache to destination cache.
   Mark handle to 0xFF
  -------------------------------------------------------------------------*/ 
  while (srcSockCache->Count() > 0)
  {
    sockInfoPtr = (SockInfo *)srcSockCache->PopFront();
    if (NULL != sockInfoPtr)
    {  
      sockInfoPtr->SetHandle(UINT32_MAX);
      destSockCache->PushBack(sockInfoPtr); 
    }
  }

  LOG_MSG_INFO1_2(" TransferPartialAssignSockets : After src count %d dest count ",
                    srcSockCache->Count(),
                    destSockCache->Count());
}/* SocketCache::TransferPartialAssignSockets */


void SocketCache::DeInit(void)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Release all internal lists
  -------------------------------------------------------------------------*/ 
  ReleaseAllSockets(&sockIPv4UDPCache, NULL);
  ReleaseAllSockets(&sockIPv6UDPCache, NULL);
  ReleaseAllSockets(&sockIPv4TCPCache, NULL);
  ReleaseAllSockets(&sockIPv6TCPCache, NULL);

  DS_UTILS_RELEASEIF(critSectPtr);
} /* SocketCache::DeInit */

ds::Utils::List * SocketCache::GetSockCache
(
  ds::Sock::ProtocolType protocol,
  ds::AddrFamilyType     family
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (ds::Sock::Protocol::QDS_UDP == protocol)
  {
    if (ds::AddrFamily::QDS_AF_INET == family)
    {
      return &sockIPv4UDPCache;
    }
    if (ds::AddrFamily::QDS_AF_INET6 == family)
    {
      return &sockIPv6UDPCache;
    }    
  }
  
  if (ds::Sock::Protocol::QDS_TCP == protocol)
  {
    if (ds::AddrFamily::QDS_AF_INET == family)
    {
      return &sockIPv4TCPCache;
    }
    if (ds::AddrFamily::QDS_AF_INET6 == family)
    {
      return &sockIPv6TCPCache;
    }    
  }  
  return NULL;
} /* SocketCache::GetSockCache */

uint8 SocketCache::GetRemoteSockCacheCnt
(
  ds::Sock::ProtocolType  protocol, 
  ds::AddrFamilyType      family
)
{
  ds::Utils::List *  sockCachePtr;
  uint8              count = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  critSectPtr->Enter();
  
  sockCachePtr = GetSockCache(protocol, family); 
  
  if (NULL != sockCachePtr)
  {
    count =  sockCachePtr->Count();
  }
  
  critSectPtr->Leave();
  return count;
} /* SocketCache::GetRemoteSockCacheCnt */


SockInfo * SocketCache::GetRemoteSockCacheEntry
(
  uint8                   index,
  ds::Sock::ProtocolType  protocol, 
  ds::AddrFamilyType      family
)
{
  ds::Utils::List *  sockCachePtr = NULL;
  SockInfo        *  sockInfoPtr = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  critSectPtr->Enter();
  /*-------------------------------------------------------------------------
   Get socket cache entry based on index, protocol and family
  -------------------------------------------------------------------------*/ 
  sockCachePtr = GetSockCache(protocol,family); 
  
  if (NULL != sockCachePtr && sockCachePtr->Count() > 0)
  {
    sockInfoPtr = (SockInfo *)sockCachePtr->Get(index);
  }
  
  critSectPtr->Leave();
  
  return sockInfoPtr;
} /* SocketCache::GetRemoteSockCacheEntry */

SockInfo * SocketCache::GetRemoteSockCacheEntry
(
  ds::Sock::ProtocolType  protocol, 
  ds::AddrFamilyType      family
)
{
  ds::Utils::List *  sockCachePtr = NULL;
  SockInfo        *  sockInfoPtr  = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  critSectPtr->Enter();
  
  sockCachePtr = GetSockCache(protocol, family); 
  
  /*-------------------------------------------------------------------------
   Get socket cache entry based on protocol and family
  -------------------------------------------------------------------------*/ 
  if (NULL != sockCachePtr && sockCachePtr->Count() > 0)
  {
    sockInfoPtr = (SockInfo *)sockCachePtr->PopFront();
  }
  
  critSectPtr->Leave();
  
  return sockInfoPtr;
} /* SocketCache::GetRemoteSockCacheEntry */

SockInfo * SocketCache::GetRemoteSockCacheEntry
(
  ds::Sock::ProtocolType  protocol, 
  ds::AddrFamilyType      family,
  int16                   portNumber
)
{
   ds::Utils::List *  sockCachePtr = NULL;
   SockInfo        *  sockInfoPtr  = NULL;
   int                sockCount = 0;
 /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   critSectPtr->Enter();
   
   sockCachePtr = GetSockCache(protocol, family); 
   /*-------------------------------------------------------------------------
    Get socket cache entry based on port, protocol and family
   -------------------------------------------------------------------------*/ 
   if (NULL != sockCachePtr)
   {
     for ( sockCount = 0; sockCount < sockCachePtr->Count() ; sockCount++)
     {
       sockInfoPtr = (SockInfo *)sockCachePtr->Get(sockCount);
       
       if (NULL != sockInfoPtr && portNumber == sockInfoPtr->GetPort())
       {
         sockCachePtr->RemoveItem(sockInfoPtr);
         break;
       }

       sockInfoPtr = NULL;
       
     }
   }
   critSectPtr->Leave();
   return sockInfoPtr;
}/* SocketCache::GetRemoteSockCacheEntry */

void SocketCache::UpdateSockInfoToCache
(
  SockInfo    * sockInfoPtr
)
{
  ds::Utils::List * sockCache = NULL;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 

  if ( NULL != sockInfoPtr )
  {
    sockCache = GetSockCache( sockInfoPtr->protocol, sockInfoPtr->family);
     if ( NULL == sockCache )
     {
        return;
     }
  }
  else
  {
    return;
  }

  critSectPtr->Enter();
  
  sockCache->PushBack(sockInfoPtr); 

  critSectPtr->Leave();
} /* SocketCache::UpdateSockInfoToCache */


uint16 SocketCache::GetAllowedMaxSockCacheCnt(void)
{
  uint16 maxCnt = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  critSectPtr->Enter();
    maxCnt = maxAllowedSocketInCache;
  critSectPtr->Leave();
  
  return maxCnt;
}/* SocketCache::GetAllowedMaxSockCacheCnt */

void  SocketCache::SetAllowedMaxSockCacheCnt(uint16 maxCnt)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  critSectPtr->Enter();
    maxAllowedSocketInCache = MIN(MAX_SOCKETS, maxCnt);
  critSectPtr->Leave();
}/* SocketCache::GetAllowedMaxSockCacheCnt */


void SocketCache::ReleaseSocketCaches
(
  ps_sys_remote_socket_release_list_type *sockReleaseList
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  ReleaseAllSockets(&sockIPv4UDPCache, sockReleaseList);
  ReleaseAllSockets(&sockIPv6UDPCache, sockReleaseList);
  ReleaseAllSockets(&sockIPv4TCPCache, sockReleaseList);
  ReleaseAllSockets(&sockIPv6TCPCache, sockReleaseList);

}/*SocketCache::ReleaseSocketCaches*/


