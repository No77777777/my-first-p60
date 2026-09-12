/*===========================================================================
               PS _ SOCK _ REMOTESOCKET _EPHEMERAL . CPP

DESCRIPTION

  PS SOCK REMOTESOCKET _ ADJACENT_EPHEMERAL - Implementation.

FUNCTIONS

Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_RemoteSocket_Ephemeral.cpp#2 $
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
#include "PS_Sock_RemoteSocket_Manager.h"
#include "ds_Addr_Def.h"
#include "ds_Sock_Def.h"
#include "PS_Sock_RemoteSocket_Ephemeral.h"
#include "PS_Sock_Platform_SocketFactory.h"

using namespace PS::Sock::RemoteSocket;
using namespace ds::Error;
using namespace ds::Utils;
/*===========================================================================

                                    PUBLIC CLASS FUNCTIONS

===========================================================================*/

EphemeralPortManager::EphemeralPortManager
(
  PortPreference          * sockets_pref,
  PortReqCallBack           cb_fn,
  void                    * userdata
):refCount(1)
{
  SetClientCallBackPending(TRUE);
  SetClientCallBackData( userdata);
  SetClientCallBackFunc(cb_fn);
  SetSocketsPreference( sockets_pref );
  Init();

}/*EphemeralPortManager::EphemeralPortManager*/
void EphemeralPortManager::Init(void)
{
  uint32    maxSocketCount = 0;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Init Parent class */
   SocketCache::Init();

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      Initialize MAX sockets based on Application preference .
      Preferred count is number of sockets required irrespective of Family and transport  
      1 pref count is equal to V4TCP V4UDP V6TCP V6UDP
       - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   maxSocketCount = socketPreference.pref_count * 4;
  
   SetAllowedMaxSockCacheCnt(maxSocketCount);
  
   /*Init Handle*/
   SetSocketsEphemeralHandle 
   (
     ps_handle_mgr_get_handle(PS_HANDLE_MGR_CLIENT_SOCKET_EPHEMERAL_CACHE, 0)
   );

} /* EphemeralPortManager::Init */

void EphemeralPortManager::DeInit(void)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*DeInit Parent class*/
  SocketCache::DeInit();
  ReleaseAllSockets(&partialRemoteSockCache, NULL);
  ReleaseAllSockets(&partialLocalSockCache, NULL);

} /* EphemeralPortManager::DeInit */
boolean  EphemeralPortManager::MatchSocketsPreference
(
    PortPreference  *pref
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( socketPreference.port_property   == pref->port_property )
  {

    if(0 == pref->base_port_range && 0 == pref->max_port_range)
    {
      if( (socketPreference.base_port_range !=
            LocalEphemeralPortGen::GetEphemeralBasePort()) ||
           (socketPreference.max_port_range  != 
           (LocalEphemeralPortGen::GetEphemeralBasePort()+ 
            LocalEphemeralPortGen::GetEphemeralPortOffset()))) return FALSE;
    }
    else
    {
       if (socketPreference.base_port_range != pref->base_port_range ||
            socketPreference.max_port_range  != pref->max_port_range) return FALSE;
    }
     
    return TRUE;
    
  }
  return FALSE;

}/*EphemeralPortManager::MatchSocketsPreference*/

boolean EphemeralPortManager::InitilizeMaxSocketsRequest
(
  ps_sys_remote_socket_request_type * remoteSocketsList
)
{
   boolean res = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == remoteSocketsList )
  {
    return res;
  }
  
  remoteSocketsList->request_handle  = GetSocketsEphemeralHandle();
  remoteSocketsList->port_property   = socketPreference.port_property;
  remoteSocketsList->base_port_range = socketPreference.base_port_range;
  remoteSocketsList->max_port_range  = socketPreference.max_port_range;

  if ( DSS_PORT_PROP_SYMMETRIC == socketPreference.port_property )
  {
    res = InitilizeSymEphemeralList( &remoteSocketsList->sockets_list);
  }
  else if ( DSS_PORT_PROP_ADJACENT_START_EVEN == socketPreference.port_property)
  {
    res = InitilizeAdjEphemeralList( &remoteSocketsList->sockets_list);
  }
  else
  {
    res = InitilizeEphemeralList( &remoteSocketsList->sockets_list);
  }

  return res;
}/*EphemeralPortManager::InitilizeMaxSocketsRequest*/


boolean EphemeralPortManager::InitilizeEphemeralList
(
  ps_sys_remote_socket_list_type    * remoteSocketsList
)
{
  int         cntSock  = 0;
  uint8       maxCount = GetAllowedMaxSockCacheCnt();
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 

  if ( 0 ==  maxCount )
  {
    return FALSE; 
  }
  /*-------------------------------------------------------------------------
   Request sockets each for type IPV4UDPSocket, IPV4TCPSocket, 
   IPV6UDPSocket IPV6TCPSocket
 -------------------------------------------------------------------------*/  
  memset(remoteSocketsList, 0, sizeof(ps_sys_remote_socket_list_type));
  
  remoteSocketsList->num_sockets = maxCount;

  while ( cntSock < maxCount)
  {

    INITILIZEV4TCPINFO(remoteSocketsList->socket_list[cntSock]);
    cntSock++;
    
    if(cntSock < maxCount)
    {
      INITILIZEV4UDPINFO(remoteSocketsList->socket_list[cntSock]);
      cntSock++;
    }
    
    if(cntSock < maxCount)
    {
      INITILIZEV6TCPINFO(remoteSocketsList->socket_list[cntSock]);
      cntSock++;
    }
    
    if(cntSock < maxCount)
    {
      INITILIZEV6UDPINFO(remoteSocketsList->socket_list[cntSock]);
      cntSock++;
    }
  }
  return TRUE;
}/*EphemeralPortManager::InitilizeEphemeralList*/

boolean EphemeralPortManager::InitilizeAdjEphemeralList
(
  ps_sys_remote_socket_list_type    * remoteSocketsList
)
{
  boolean     res      = FALSE;
  uint8       maxCount = GetAllowedMaxSockCacheCnt();
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-----------------------------------------------------------------------
    Modem fills in the request_socket_list per pair and AP figures out the number of sockets 
    needed to fulfil that request and fill in the response structure accordingly
    1 Socket pair will have two ports.(1 even and 1 odd number)
  -----------------------------------------------------------------------*/ 
  if ( 0 != maxCount )
  {
    res = InitilizeEphemeralList( remoteSocketsList); 
  }
  return res;
  
}/*EphemeralPortManager::InitilizeAdjEphemeralList*/


boolean EphemeralPortManager::InitilizeSymEphemeralList
(
  ps_sys_remote_socket_list_type    * remoteSocketsList
)
{
  boolean     res      = FALSE;
  int         cntSock  = 0;
  uint8       maxSockPair = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  
  /*-----------------------------------------------------------------------
    Modem fills in the request_socket_list per pair and AP figures out the number of sockets 
    needed to fulfil that request and fill in the response structure accordingly
  -----------------------------------------------------------------------*/  
  maxSockPair = GetAllowedMaxSockCacheCnt() / 4;
  
  if ( 0 ==  maxSockPair )
  {
    return res; 
  }
  
  memset(remoteSocketsList, 0, sizeof(ps_sys_remote_socket_list_type));
  /*-----------------------------------------------------------------------
    Length means number of pairs 
  -----------------------------------------------------------------------*/  
  remoteSocketsList->num_sockets = maxSockPair;

  while (cntSock < maxSockPair)
  {
    INITILIZEV4V6TCPUDPINFO(remoteSocketsList->socket_list[cntSock]);
    cntSock++;
  }
  return TRUE;
}/*EphemeralPortManager::InitilizeSymEphemeralList*/

boolean EphemeralPortManager::IsLegacyEphemeralCache()
{
  uint16                                     basePort = 0;
  uint16                                     offset   = 0;
  boolean                                    result = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  basePort = LocalEphemeralPortGen::GetEphemeralBasePort();
  offset   = LocalEphemeralPortGen::GetEphemeralPortOffset();
  
  if ( DSS_PORT_PROP_MIN   == socketPreference.port_property &&
       basePort <= socketPreference.base_port_range &&
       socketPreference.max_port_range <= basePort + offset )
  {
    result =  TRUE;
  }
  return result;
}/*EphemeralPortManager::IsLegacyEphemeralCache*/

void EphemeralPortManager::UpdateSockInfoToCache
(
  ps_sys_remote_socket_allocated_type * remoteSockInfoPtr
)
{

  SockInfo            *   sockInfoPtr  = NULL;
  int                     sockCount    = 0;
  uint16                  pPort        = 0;
  uint16                  sPort        = 0;
  ds::AddrFamilyType      pFamily;
  ds::AddrFamilyType      sFamily;
  ds::Sock::ProtocolType  pProtocol;
  ds::Sock::ProtocolType  sProtocol;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  LOG_MSG_INFO1_5("UpdateSockInfoToCache status %d  port_no %d (HostOrder)"
                  "  handle %d xport_proto %d ip_family %d",
                  remoteSockInfoPtr->status, 
                  ps_ntohs(remoteSockInfoPtr->socket_info.port_no), 
                  remoteSockInfoPtr->remote_socket_handle, 
                  remoteSockInfoPtr->socket_info.xport_proto, 
                  remoteSockInfoPtr->socket_info.ip_family);   
  
  /*-----------------------------------------------------------------------
    Create new object for storing Remote socket handle information and
    store in appropriate cache
  -----------------------------------------------------------------------*/    
  if (PS_SYS_REMOTE_SOCKET_ALLOC_SUCCESS == remoteSockInfoPtr->status)
  {
    if ( TRUE == remoteSockInfoPtr->is_ephemeral_port )
    {
      sockInfoPtr =  
        new SockInfo
        (
          remoteSockInfoPtr->socket_info.port_no,
          remoteSockInfoPtr->remote_socket_handle,
          RemoteSocketUtils::ConvertToDSSockProtocol
            (remoteSockInfoPtr->socket_info.xport_proto),
          RemoteSocketUtils::ConvertToDSSockFamily
            (remoteSockInfoPtr->socket_info.ip_family)
        );
      if ( sockInfoPtr != NULL )
      {
        SocketCache::UpdateSockInfoToCache(sockInfoPtr);
      }
    }
    else
    {
      for ( sockCount = 0; sockCount < partialLocalSockCache.Count(); sockCount++ )
      {
        sockInfoPtr = (SockInfo *)partialLocalSockCache.Get(sockCount);
        if ( NULL != sockInfoPtr )
        {
          pPort     = remoteSockInfoPtr->socket_info.port_no;
          pFamily   = RemoteSocketUtils::ConvertToDSSockFamily(remoteSockInfoPtr->socket_info.ip_family);
          pProtocol = RemoteSocketUtils::ConvertToDSSockProtocol(remoteSockInfoPtr->socket_info.xport_proto);
          
          sPort     = sockInfoPtr->GetPort();
          sFamily   = sockInfoPtr->GetFamily();
          sProtocol = sockInfoPtr->GetProtocol();
          
          if ( pPort ==  sPort && pFamily == sFamily && pProtocol == sProtocol)
          {
            partialLocalSockCache.RemoveItem(sockInfoPtr);
            sockInfoPtr->SetHandle(remoteSockInfoPtr->remote_socket_handle);
            partialRemoteSockCache.PushBack(sockInfoPtr);
            break;
          }
        }
      }
    }
  }
  return;
} /* EphemeralPortManager::UpdateSockInfoToCache*/

boolean EphemeralPortManager::RetrieveAndRequestSocket
(
  EphemeralSocketFlag                     bindFlag,
  SockInfo                               *sockInfo,
  ps_sys_remote_socket_request_type      *remoteSocketsList,
  ps_sys_remote_socket_release_list_type *sockReleaseList
)
{

  boolean result = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  LOG_MSG_INFO1_4("RetrieveAndRequestSocket Requested port num %d "
                  "(HostOrder) Protocol %d Family %d Flag %d",
                   ps_ntohs(sockInfo->GetPort()),
                   sockInfo->GetProtocol(),
                   sockInfo->GetFamily(),
                   bindFlag);
  
  switch ( socketPreference.port_property )
  {
    case DSS_PORT_PROP_SYMMETRIC:
    case DSS_PORT_PROP_ADJACENT_START_EVEN:
    {
      result =  RetrieveSockFromSpecificCacheList( bindFlag, 
                  sockInfo, remoteSocketsList, sockReleaseList);
      break;
    }
    case DSS_PORT_PROP_MIN:
    default:
    {
      result =  RetrieveSockFromDefaultCacheList( bindFlag,
                  sockInfo, remoteSocketsList, sockReleaseList);
      break;
    }
  }
  
  LOG_MSG_INFO1_6(" RetrieveAndRequestSocket Prop %d  Remote "
                  " Release len %d  Request len %d  Cache Handle %d "
                  " Port num %d (HostOrder) remote sock handle %d",
                    socketPreference.port_property,
                    sockReleaseList->num_sockets,
                    remoteSocketsList->sockets_list.num_sockets,
                    remoteSocketsList->request_handle,
                    ps_ntohs(sockInfo->GetPort()),
                    sockInfo->GetHandle());

  return result;
  
}/*EphemeralPortManager::RetrieveAndRequestSocket*/

void  EphemeralPortManager::InitilizeRemoteSocketRequest
(
  SockInfo                            *ReqSockInfo,
  ps_sys_remote_socket_request_type   *remoteSocketsList
)
{
  PortPreference                 pref;
  uint8                          index =0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  pref = GetSocketsPreference();
  
  remoteSocketsList->request_handle           = GetSocketsEphemeralHandle();
  remoteSocketsList->port_property            = pref.port_property;
  remoteSocketsList->base_port_range          = pref.base_port_range;
  remoteSocketsList->max_port_range           = pref.max_port_range;

  index = remoteSocketsList->sockets_list.num_sockets;
  
  if (index < GetAllowedMaxSockCacheCnt() )
  {
    /*-----------------------------------------------------------------------
      Initilize remote socket entry based on requested socket
    -----------------------------------------------------------------------*/ 
    remoteSocketsList->sockets_list.socket_list[index].port_no =  0;
    remoteSocketsList->sockets_list.socket_list[index].ip_family =  
        RemoteSocketUtils::ConvertToPSFamily(ReqSockInfo->GetFamily());
    remoteSocketsList->sockets_list.socket_list[index].xport_proto =  
        RemoteSocketUtils::ConvertToPSProtocol(ReqSockInfo->GetProtocol());
    
    /*-----------------------------------------------------------------------
       In case of Symmetric, xprot should be TCPUDP type
    -----------------------------------------------------------------------*/ 
    if ( DSS_PORT_PROP_SYMMETRIC == remoteSocketsList->port_property )
    {
      remoteSocketsList->sockets_list.socket_list[index].xport_proto = PS_IPPROTO_TCPUDP;
    }
    index++;
  }
  remoteSocketsList->sockets_list.num_sockets = index;
  return;
}/*EphemeralPortManager::InitilizeRemoteSocketRequest*/

boolean EphemeralPortManager::IsSockInUse
(
  SockInfo   * sockInfoPtr
)
{
  SocketFactory                * platformSockFactoryPtr;
  ds::SockAddrIN6InternalType    tmpSockAddr;
  boolean                        sockInUseFlag  = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  platformSockFactoryPtr = SocketFactory::CreateInstance();

  tmpSockAddr.family = ds::AddrFamily::QDS_AF_INET6;
  
  memscpy( tmpSockAddr.addr, sizeof( ds::INAddr6Type),
           &ps_in6addr_any, sizeof( ds::INAddr6Type));
  
  tmpSockAddr.port = sockInfoPtr->GetPort();
  
  /*-----------------------------------------------------------------------
      Check if  socket is in use  (Assuming port number is in network byte order)
  -----------------------------------------------------------------------*/ 
  if (true == platformSockFactoryPtr->IsSockAddrInUse( 
      sockInfoPtr->GetProtocol(), &tmpSockAddr, false, true))
  {
    sockInUseFlag = TRUE;
  }  
  DS_UTILS_RELEASEIF(platformSockFactoryPtr);

  return sockInUseFlag;
}
boolean EphemeralPortManager::RetrieveSockFromDefaultCacheList
(
  EphemeralSocketFlag                     bindFlag,
  SockInfo                               *reqSockInfo,
  ps_sys_remote_socket_request_type      *remoteSocketsList,
  ps_sys_remote_socket_release_list_type *sockReleaseList
)
{

  SockInfo                     * sockInfoPtr           = NULL;
  boolean                        localPortAllocation   = FALSE;
  SocketFactory                * platformSockFactoryPtr;
  ds::SockAddrIN6InternalType    tmpSockAddr;
  uint32                         portNum               = 0;
  uint32                         handle                = UINT32_MAX;
  boolean                        socketFound           = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-----------------------------------------------------------------------
      1. When Remote Service Available, Get a Socket from cache based on Protocol an family 
      2. Check if Retrieve socket is in use 
      3. Initilize a remote socket request for AP
  -----------------------------------------------------------------------*/ 
  if (REMOTE_SOCKETS_AVAILABLE == GetCacheState() ||
      REMOTE_SOCKETS_AVAILABLE_AND_REQUESTED == GetCacheState())
  {
    if( 0 != reqSockInfo->GetPort())
    {
      return FALSE;
    }
    do
    {
      sockInfoPtr = GetRemoteSockCacheEntry(reqSockInfo->GetProtocol(), 
                    reqSockInfo->GetFamily());
      
      if (NULL != sockInfoPtr && TRUE == IsSockInUse(sockInfoPtr) && 
            sockReleaseList->num_sockets != PS_SYS_MAX_REMOTE_SOCKETS)
      {
         socketFound = FALSE;
         
         sockReleaseList->socket_list[sockReleaseList->num_sockets++] =
              sockInfoPtr ->GetHandle();
         
         delete sockInfoPtr;
      }
      else
      {
         socketFound = TRUE;
      }
      
    }
    while (FALSE == socketFound);
    
    if (NULL != sockInfoPtr )
    {
      handle =  sockInfoPtr ->GetHandle();
      portNum = sockInfoPtr->GetPort();
      SetCacheState(REMOTE_SOCKETS_AVAILABLE_AND_REQUESTED);
      delete sockInfoPtr;
    }
    else
    {
      localPortAllocation = TRUE;
    }
  }
  else
  {
    localPortAllocation = TRUE;
  }
  
  if ( TRUE == localPortAllocation)
  {
    platformSockFactoryPtr = SocketFactory::CreateInstance();
      
    DS_UTILS_ASSERT( 0 != platformSockFactoryPtr); 
    /*-----------------------------------------------------------------
      For IPSEC, IPSEC module always sets the socket option first
      followed by bind operation. if they dont do that way it would result 
      in failure. If socket option for udp encaps is set and if AP
      is not avaiable then return 32012 port which is the port used
      by AP when remote socket bind is not enabled.
    -----------------------------------------------------------------*/
    if ( EPHEMERAL_FLAG_UDP_ENCAPS == bindFlag)
    {
      tmpSockAddr.port = ps_htons(32012);
      
      if (FALSE == 
          platformSockFactoryPtr->IsSockAddrInUse( reqSockInfo->GetProtocol(), 
                                                   &tmpSockAddr, 
                                                    false,
                                                    false))
      {
        portNum = tmpSockAddr.port;
          
        LOG_MSG_INFO2_1("Manager::RetrieveSockFromDefaultCacheList : "
                        "Returning 32012 port %d for ipsec", tmpSockAddr.port);
      }
    }

    DS_UTILS_RELEASEIF(platformSockFactoryPtr);
      
    if (0 == portNum)
    {
      portNum = 
        LocalEphemeralPortGen::GenerateEphemeralPort(reqSockInfo->GetProtocol());
    }
  }
  reqSockInfo->SetHandle(handle);
  reqSockInfo->SetPort(portNum);
  
  /*-----------------------------------------------------------------
    Intilize Remote socket request for AP based on port preference 
    ICMP sockets request will send to AP
    -----------------------------------------------------------------*/
  if (ds::Sock::Protocol::QDS_TCP ==  reqSockInfo->protocol ||
      ds::Sock::Protocol::QDS_UDP ==  reqSockInfo->protocol)
  {
    InitilizeRemoteSocketRequest(reqSockInfo, remoteSocketsList);
  }
  
  return TRUE;
    
}/*EphemeralPortManager::RetrieveSockFromDefaultCacheList*/

boolean EphemeralPortManager::RetrieveSockFromSpecificCacheList
(
  EphemeralSocketFlag                  bindFlag,
  SockInfo                            *reqSockInfo,
  ps_sys_remote_socket_request_type   *remoteSocketsList,
  ps_sys_remote_socket_release_list_type *sockReleaseList
)
{

  boolean                 result = TRUE;
  SockInfo              * PriSockInfoPtr      = NULL;
  SockInfo              * SecSockInfoPtr      = NULL;
  uint32                  portNum             = 0;
  uint32                  handle              = UINT32_MAX;
  boolean                 localPortAllocation = FALSE;
  boolean                 foundSockets        = FALSE;
  int16                   SecSockPort;
  ds::Sock::ProtocolType  SecSockProtocol;
  boolean                 reqRemoteSocketFlag = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if (REMOTE_SOCKETS_AVAILABLE == GetCacheState() ||
      REMOTE_SOCKETS_AVAILABLE_AND_REQUESTED == GetCacheState())
  {
    /*-----------------------------------------------------------------
        1. When port number is 0
        2. Reterive socket from the appropriate cache
        3. Look for companion port number into another cache
        4. If found remove it and update it into partial list
        5. return this socket FD
        6. send request to AP 
    -----------------------------------------------------------------*/
    if (0 == reqSockInfo->GetPort())
    {
      do
      {
        PriSockInfoPtr = GetRemoteSockCacheEntry(reqSockInfo->GetProtocol(), 
                           reqSockInfo->GetFamily());

        if ( (NULL != PriSockInfoPtr) && (sockReleaseList->num_sockets < PS_SYS_MAX_REMOTE_SOCKETS - 1))
        {
          if (DSS_PORT_PROP_SYMMETRIC == socketPreference.port_property )
          {
            SecSockPort     = PriSockInfoPtr->GetPort() ;
            SecSockProtocol = OTHERPROTO(reqSockInfo->GetProtocol());
          }
          else
          {
            SecSockPort     = ps_htons ( ps_ntohs (PriSockInfoPtr->GetPort()) + 1);
            SecSockProtocol = reqSockInfo->GetProtocol();
          }          
          SecSockInfoPtr = GetRemoteSockCacheEntry(SecSockProtocol,
                             reqSockInfo->GetFamily(), SecSockPort);
          
          /*-----------------------------------------------------------------------
             Check if both sockets are in Use or not.
          -----------------------------------------------------------------------*/
          if ( NULL != SecSockInfoPtr)
          {
            if (TRUE == IsSockInUse(PriSockInfoPtr) || TRUE == IsSockInUse(SecSockInfoPtr))
            {
              
              sockReleaseList->socket_list[sockReleaseList->num_sockets++] =
                   SecSockInfoPtr ->GetHandle();

              sockReleaseList->socket_list[sockReleaseList->num_sockets++] =
                   PriSockInfoPtr ->GetHandle();
                 
              InitilizeRemoteSocketRequest(PriSockInfoPtr, remoteSocketsList );

              foundSockets = FALSE;

              delete SecSockInfoPtr;
            }
            else
            {
              handle  =  PriSockInfoPtr ->GetHandle();
              portNum =  PriSockInfoPtr->GetPort();
              partialRemoteSockCache.PushBack(SecSockInfoPtr); 
              foundSockets = TRUE;
            }
          }
          else
          {
            sockReleaseList->socket_list[sockReleaseList->num_sockets++] =
              PriSockInfoPtr ->GetHandle();
            InitilizeRemoteSocketRequest(PriSockInfoPtr, remoteSocketsList );
            foundSockets = FALSE;
          }
          delete PriSockInfoPtr;
        }
        else
        {
          localPortAllocation = TRUE;
          
        }
      }while (FALSE == foundSockets && FALSE == localPortAllocation);
    
    }//Second second binding
    else
    {
      /*-----------------------------------------------------------------
        1. When port number is not 0
        2. Get companion socket from remote partial cache list
        3. Dont send new ports request to AP
        -----------------------------------------------------------------*/
      reqRemoteSocketFlag = FALSE;
      SecSockInfoPtr = RetrieveSockFromPartialCaches(  reqSockInfo->GetProtocol(),
                                                       reqSockInfo->GetFamily(), 
                                                       reqSockInfo->GetPort(),
                                                       TRUE);
      if ( NULL != SecSockInfoPtr)
      {
         handle  =  SecSockInfoPtr ->GetHandle();
         portNum =  SecSockInfoPtr->GetPort();
         delete SecSockInfoPtr;
      }
      else
      {
         result = FALSE;
      }  
    }
    SetCacheState(REMOTE_SOCKETS_AVAILABLE_AND_REQUESTED);
  }
  else
  {
    localPortAllocation = TRUE;
  }
  if ( TRUE == localPortAllocation)
  {
    /*-----------------------------------------------------------------
       1. When port number is 0
       2. Generate ephemeral port based on property  
       3. Ephemeral port Generator checks the port availability (Both Sockets shouldn’t be in use)
       4. Create a companion socket and store into local partial list
       5. Send new sockets (Pair) request to AP
    -----------------------------------------------------------------*/
    if ( 0 == reqSockInfo->GetPort())
    {
      
      portNum  = LocalEphemeralPortGen::GenerateEphemeralPort( 
                   reqSockInfo->GetProtocol(), socketPreference);
      
      handle   = UINT32_MAX;
      
      if ( 0 != portNum)
      {
        if (DSS_PORT_PROP_SYMMETRIC == socketPreference.port_property )
        {
          SecSockPort     = portNum ;
          SecSockProtocol = OTHERPROTO(reqSockInfo->GetProtocol());
        }
        else
        {
          
          SecSockPort     = ps_htons (( ps_ntohs (portNum)) + 1);
          SecSockProtocol = reqSockInfo->GetProtocol();
        }          
        
        SecSockInfoPtr =  new SockInfo(SecSockPort, UINT32_MAX, 
                                SecSockProtocol, reqSockInfo->GetFamily());         
  
        partialLocalSockCache.PushBack(SecSockInfoPtr); 
        
      }
      else
      {
        result = FALSE;
      }
    }
    else
    {
      /*-----------------------------------------------------------------
        1. When port number is not 0
        2. Get companion socket from local partial cache list
        3. Dont send new ports request to AP
        -----------------------------------------------------------------*/
      reqRemoteSocketFlag = FALSE;
      SecSockInfoPtr = RetrieveSockFromPartialCaches(  reqSockInfo->GetProtocol(),
                                                       reqSockInfo->GetFamily(), 
                                                       reqSockInfo->GetPort(),
                                                       TRUE);
      if ( NULL != SecSockInfoPtr)
      {
         handle  =  SecSockInfoPtr ->GetHandle();
         portNum =  SecSockInfoPtr->GetPort();
         delete SecSockInfoPtr;
      }
      else
      {
         result = FALSE;
      }  
    }
  }
  reqSockInfo->SetHandle(handle);
  reqSockInfo->SetPort(portNum);
  /*-----------------------------------------------------------------
    Intilize Remote socket request for AP based on port preference 
    -----------------------------------------------------------------*/
  if ( TRUE == reqRemoteSocketFlag)
  {
    InitilizeRemoteSocketRequest(reqSockInfo, remoteSocketsList );
  }
  
  return result;
    
}/*EphemeralPortManager::RetrieveSockFromSpecificCacheList*/

SockInfo * EphemeralPortManager::RetrieveSockCacheEntry
(
  ds::Sock::ProtocolType  protocol, 
  ds::AddrFamilyType      family,
  int16                   portNumber,
  ds::Utils::List *       sockCachePtr,
  boolean                 getAndRemoveFlag
)
{
  SockInfo        *  sockInfoPtr  = NULL;
  int                sockCount = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL != sockCachePtr)
  {
    for ( sockCount = 0; sockCount < sockCachePtr->Count() ; sockCount++)
    {
      sockInfoPtr = (SockInfo *)sockCachePtr->Get(sockCount);
      
      if ( sockInfoPtr != NULL &&
           portNumber == sockInfoPtr->GetPort()&&
           family     == sockInfoPtr->GetFamily() &&
           protocol   == sockInfoPtr->GetProtocol())
      {
        if (TRUE == getAndRemoveFlag)
        {
          sockCachePtr->RemoveItem(sockInfoPtr);
        }
        break;
      }
      sockInfoPtr = NULL;
    }
  }
  return sockInfoPtr;
}/*EphemeralPortManager::GetPartialSockCacheEntry*/

SockInfo * EphemeralPortManager::RetrieveSockFromPartialCaches
(
  ds::Sock::ProtocolType   protocol, 
  ds::AddrFamilyType       family,
  int16                    portNumber,
  boolean                  getAndRemoveFlag
)
{
  SockInfo     * SockInfoPtr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-----------------------------------------------------------------------
    1. Search into partial remote socket  cahce
    1. Not Fount: Search into partial local socket  cahce
  -----------------------------------------------------------------------*/
  SockInfoPtr = RetrieveSockCacheEntry( protocol,
                                        family, 
                                        portNumber, 
                                       &partialRemoteSockCache,
                                        getAndRemoveFlag);
  if ( NULL == SockInfoPtr)
  {
    SockInfoPtr = RetrieveSockCacheEntry( protocol,
                                          family, 
                                          portNumber, 
                                          &partialLocalSockCache,
                                          getAndRemoveFlag);
  }
  return SockInfoPtr;

}/*EphemeralPortManager::GetSockFromPartialCaches*/

void EphemeralPortManager::ReleaseRemoteSocket
(
  SockInfo                               *relSockInfo,
  ps_sys_remote_socket_release_list_type *sockReleaseList
)
{
  ds::Utils::List        *partialAllocatedList;
  SockInfo               *sockInfoPtr  = NULL;
  int                     sockCount    = 0;
  int16                   secSockPort;
  uint32                  portNum = 0;
  ds::Sock::ProtocolType  secSockProtocol;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 

  /*--------------------------------------------------------------------
    1. Release socket to AP Immediately when preference are none(Default modem cache)
    2. When preferences are set then check for companion  socket.
    3. If companion sockets is in partial list then Release both the sockets
    4. If companion socket is not in partial list then store this one into partial cache
  ---------------------------------------------------------------------*/
    switch ( socketPreference.port_property )
    {
      case DSS_PORT_PROP_SYMMETRIC:
      case DSS_PORT_PROP_ADJACENT_START_EVEN:
      {
        /*--------------------------------------------------------------------
               Port number is in Network byte order so convert into host before math operation.
               Adj Pool: Check released port is even or odd before getting companion Socket
        ---------------------------------------------------------------------*/
        portNum = relSockInfo->GetPort() ;
            
        if (DSS_PORT_PROP_SYMMETRIC == socketPreference.port_property )
        {
          secSockPort     = portNum;
          secSockProtocol = OTHERPROTO(relSockInfo->GetProtocol());
        }
        else
        {         
          portNum = ps_ntohs(portNum);
          
          if ( portNum % 2 == 0)
          {//next odd number
            secSockPort =     ps_htons(portNum + 1);
          }
          else
          { //previous even number
            secSockPort =     ps_htons(portNum - 1);
          }
          secSockProtocol = relSockInfo->GetProtocol();
        }          
        if ( UINT32_MAX == relSockInfo->GetHandle())
        {
          partialAllocatedList = &partialLocalSockCache;
        }
        else
        {
          partialAllocatedList = &partialRemoteSockCache;
        }
        for ( sockCount = 0; sockCount < partialAllocatedList->Count() ; sockCount++)
        {
          sockInfoPtr = (SockInfo *)partialAllocatedList->Get(sockCount);
         
          if ( sockInfoPtr != NULL && 
              secSockPort    == sockInfoPtr->GetPort() &&
              secSockProtocol == sockInfoPtr->GetProtocol()&&
              relSockInfo->GetFamily() == sockInfoPtr->GetFamily())
          {
            partialAllocatedList->RemoveItem(sockInfoPtr);
            break;
          }
          sockInfoPtr = NULL;
        }
        /*--------------------------------------------------------------------
          Companion socket found
        ---------------------------------------------------------------------*/
        if ( NULL != sockInfoPtr && sockReleaseList->num_sockets < PS_SYS_MAX_REMOTE_SOCKETS - 1 )
        {
          if( UINT32_MAX != relSockInfo->GetHandle())
          {
            sockReleaseList->socket_list[sockReleaseList->num_sockets] = 
                relSockInfo->GetHandle();
            sockReleaseList->num_sockets++;
          }
          if( UINT32_MAX != sockInfoPtr->GetHandle())
          {
            sockReleaseList->socket_list[sockReleaseList->num_sockets] =
                sockInfoPtr->GetHandle();
            sockReleaseList->num_sockets++;
          }
          //Free companion socket 
          delete sockInfoPtr;
        }
        else
        { // this is first socket to be freed so add into partial queue
          sockInfoPtr =  new SockInfo(relSockInfo->GetPort(), 
                                      relSockInfo->GetHandle(),
                                      relSockInfo->GetProtocol(),
                                      relSockInfo->GetFamily());
          
          partialAllocatedList->PushBack(sockInfoPtr); 

          //Don't release any socket to AP
          sockReleaseList->num_sockets = 0;
          
          LOG_MSG_INFO1_4(" ReleaseRemoteSocket : Added Socket into partial list "
                          " Port %d (HostOrder) Handle %d Family %d Proto %d", 
                            dss_ntohs(relSockInfo->GetPort()),
                            relSockInfo->GetHandle(),
                            relSockInfo->GetFamily(),
                            relSockInfo->GetProtocol());
        }
        break;
      }
      case DSS_PORT_PROP_MIN:
      default:
      {
        //Don't release any socket to AP if it was not allocated by AP
        if( UINT32_MAX != relSockInfo->GetHandle())
        {
          sockReleaseList->num_sockets    = 1;
          sockReleaseList->socket_list[0] = relSockInfo->GetHandle();
         }
        break;
      }
    }
    return;
}/*EphemeralPortManager::ReleaseRemoteSocket*/

void EphemeralPortManager::ReleaseSocketCaches
(
  ps_sys_remote_socket_release_list_type *sockReleaseList
)
{
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  SocketCache::ReleaseSocketCaches( sockReleaseList );
  ReleaseAllSockets(&partialLocalSockCache, sockReleaseList);
  ReleaseAllSockets(&partialRemoteSockCache, sockReleaseList);

}/*EphemeralPortManager::ReleaseSocketCaches*/


void EphemeralPortManager::ReleaseUnusedSocketCaches
(
)
{
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  SocketCache::ReleaseSocketCaches( NULL );
}/*EphemeralPortManager::ReleaseUnusedSocketCaches*/

void EphemeralPortManager::TransferPartialAssignSockets()
{

  SocketCache::TransferPartialAssignSockets( &partialLocalSockCache, &partialRemoteSockCache);

}/* EphemeralPortManager::TransferPartialAssignSockets */

void  EphemeralPortManager::RetrieveLocalAllocatedSockets
(
  ps_sys_remote_socket_request_type * remoteSocketsList
)
{
  uint8              index        = 0;
  SockInfo        *  sockInfoPtr  = NULL;
  int                sockCount    = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if ( NULL == remoteSocketsList)
  {
    return;
  }
  /*--------------------------------------------------------------------
    Modem can allocated sockets when AP is not available or boots up late.
    Symmetric and Adjacent pools can have companion sockets stored in local
    partial cache
  ---------------------------------------------------------------------*/
  for ( sockCount = 0; (sockCount < partialLocalSockCache.Count() && 
        remoteSocketsList->sockets_list.num_sockets < PS_SYS_MAX_REMOTE_SOCKETS); 
        sockCount++ )
  {
    sockInfoPtr = (SockInfo *)partialLocalSockCache.Get(sockCount);
    if (NULL != sockInfoPtr)
    {
      index = remoteSocketsList->sockets_list.num_sockets;
    
      remoteSocketsList->sockets_list.socket_list[index].ip_family   = 
        RemoteSocketUtils::ConvertToPSFamily(sockInfoPtr->GetFamily());
    
      remoteSocketsList->sockets_list.socket_list[index].xport_proto = 
        RemoteSocketUtils::ConvertToPSProtocol(sockInfoPtr->GetProtocol());

      remoteSocketsList->sockets_list.socket_list[index].port_no  = 
        sockInfoPtr->GetPort();
    
      remoteSocketsList->sockets_list.num_sockets++;
    }
  }
  return;
}/*EphemeralPortManager::RetrieveLocalAllocatedSockets*/


void  EphemeralPortManager::SetClientCallBackData
(
  void           *userdata
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  userData = userdata;
}/*EphemeralPortManager::SetClientCallBackData*/

void*  EphemeralPortManager::GetClientCallBackData()
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  return userData;
}/*EphemeralPortManager::GetClientCallBackData*/

void  EphemeralPortManager::SetClientCallBackPending
(
  boolean  flag
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  callBackPending = flag;
}/*EphemeralPortManager::GetClientCallBackData*/

boolean  EphemeralPortManager::IsClientCallBackPending()
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return callBackPending;
}/*EphemeralPortManager::isClientCallBackPending*/

void  EphemeralPortManager::SetClientCallBackFunc
(
  PortReqCallBack           callback
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  callBackFunction  = callback;
}/*EphemeralPortManager::SetClientCallBackFunc*/

PortReqCallBack  EphemeralPortManager::GetClientCallBackFunc
(
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   return callBackFunction ;
}/*EphemeralPortManager::SetClientCallBackFunc*/
void  EphemeralPortManager::AddRefCount()
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  refCount = refCount + 1;
}/*EphemeralPortManager::AddRefCount*/
void  EphemeralPortManager::DecRefCount()
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  refCount = refCount -1;
}/*EphemeralPortManager::DecRefCount*/

uint8 EphemeralPortManager::GetRefCount()
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  return refCount;
}/*EphemeralPortManager::GetRefCount*/
void EphemeralPortManager::SetSocketsEphemeralHandle
(
  uint32  handle
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  socketsEphemeralHandle = handle;
} /* EphemeralPortManager::SetSocketsEphemeralHandle */    

uint32 EphemeralPortManager::GetSocketsEphemeralHandle
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return socketsEphemeralHandle;
} /* EphemeralPortManager::GetSocketsEphemeralHandle*/ 
 void EphemeralPortManager::SetSocketsPreference
(
  PortPreference    *pref
)              
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*--------------------------------------------------------------------
    1. If pref count is 0 then apply default PrefCount
    2. If Base port and Max port are zero then apply modem ephemeral port ranges
  ---------------------------------------------------------------------*/ 
  socketPreference.port_property   = pref->port_property;
  
  if ( 0 != pref->pref_count)
  {
    socketPreference.pref_count    = pref->pref_count;
  }
  else
  {
     socketPreference.pref_count   = MAX_SOCKETS / 4;
  }
  
  if ( 0 == pref->base_port_range && 0 == pref->max_port_range)
  {
    socketPreference.base_port_range = LocalEphemeralPortGen::GetEphemeralBasePort();
    socketPreference.max_port_range  = 
         socketPreference.base_port_range + 
         LocalEphemeralPortGen::GetEphemeralPortOffset();
  }
  else
  {
     socketPreference.base_port_range = pref->base_port_range;
     socketPreference.max_port_range  = pref->max_port_range;
  }
}/* EphemeralPortManager::SetSocketsCachePref*/ 
PortPreference EphemeralPortManager::GetSocketsPreference
(
  void
)  
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return socketPreference;
}/* EphemeralPortManager::GetSocketsCachepref*/ 

void EphemeralPortManager::ValidateReceivedRemoteSock
(
  ps_sys_remote_socket_allocated_list_type   * sockAllocatedPtr,
  ps_sys_remote_socket_release_list_type     * sockReleaseInfo
)
{
  ps_sys_remote_socket_allocated_type  * sockInfoPtr;
  int                                    cnt_sock;
  boolean                                invalidSocket = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == sockAllocatedPtr || NULL == sockReleaseInfo)
  {
    return ;
  }
  memset(sockReleaseInfo, 0x0, sizeof(ps_sys_remote_socket_release_list_type));
  /*--------------------------------------------------------------------
    1. Preforming basic validation. 
    2 We can enhance/add more validation condition as per the requirement
  ---------------------------------------------------------------------*/ 
  switch ( socketPreference.port_property )
  {
    case DSS_PORT_PROP_SYMMETRIC:
    case DSS_PORT_PROP_ADJACENT_START_EVEN:
    case DSS_PORT_PROP_MIN:
    default:
    {
      for (cnt_sock = 0; cnt_sock < sockAllocatedPtr->num_sockets; cnt_sock++)
      {
        sockInfoPtr = &(sockAllocatedPtr->socket_list[cnt_sock]);
 
        if (PS_SYS_REMOTE_SOCKET_ALLOC_SUCCESS != sockInfoPtr->status ||
           (0 == sockInfoPtr->socket_info.port_no) ||
           (IP_V4  != sockInfoPtr->socket_info.ip_family &&
            IP_V6  != sockInfoPtr->socket_info.ip_family)||
           (PS_IPPROTO_TCP  != sockInfoPtr->socket_info.xport_proto &&
           PS_IPPROTO_UDP  != sockInfoPtr->socket_info.xport_proto))
        {
          invalidSocket = TRUE;
        }
        
        if ( TRUE == invalidSocket)
        {
        
          LOG_MSG_INFO2_5("ValidateReceivedRemoteSock : Found "
                          "Invalid Socket status %d port %d proto %d family %d handle %d",
                           sockInfoPtr->status,
                           sockInfoPtr->socket_info.port_no,
                           sockInfoPtr->socket_info.xport_proto,
                           sockInfoPtr->socket_info.ip_family,
                           sockInfoPtr->remote_socket_handle);
          //Fill release handle
          sockReleaseInfo->socket_list[sockReleaseInfo->num_sockets++] = 
            sockInfoPtr->remote_socket_handle;
          sockInfoPtr->status = PS_SYS_REMOTE_SOCKET_ALLOC_IN_USE_FAILURE;
          invalidSocket = FALSE;
        }
        
      }
     break;
    }
  }
  return ;
}/*EphemeralPortManager::ValidateReceivedRemoteSock*/


uint8 EphemeralPortManager::GetPartialCacheCount
(
    boolean                  isLocal
) 
{
  uint8   count = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( TRUE == isLocal)
  {
    count =  partialLocalSockCache.Count();
  }
  else
  {
    count =  partialRemoteSockCache.Count();
  }
  return count;
  
}/* EphemeralPortManager::GetPartialCacheCount */


