#ifndef PS_SOCK_REMOTESOCKET_CACHE_H
#define PS_SOCK_REMOTESOCKET_CACHE_H
/*===========================================================================

                 PS _ SOCK _ REMOTESOCKET _ CACHE . H

DESCRIPTION

  PS SOCK REMOTESOCKET CACHE - Definitions

  
Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_RemoteSocket_Cache.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/03/13    ad    First revision
===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "ds_Utils_ICritSect.h"
#include "ds_Sock_AddrUtils.h"
#include "ds_Utils_INode.h"
#include "ds_Utils_List.h"
#include "ds_Utils_CSSupport.h"
#include "dssocket.h"


using namespace PS::Sock::Platform;

/*===========================================================================

                                 DEFINITIONS

===========================================================================*/
namespace PS
{
  namespace Sock
  {
    namespace RemoteSocket
    { 
      #define MAX_SOCKETS         32
      #define IP_V4V6             (ip_version_enum_type)0x10
      #define PS_IPPROTO_TCPUDP   (ps_ip_protocol_enum_type)0xFF

      #define  INITILIZEV4TCPINFO( socket)                \
                socket.port_no = 0;                       \
                socket.xport_proto = PS_IPPROTO_TCP;      \
                socket.ip_family   = IP_V4;               \
            
      #define  INITILIZEV4UDPINFO( socket)                \
                 socket.port_no = 0;                      \
                 socket.xport_proto = PS_IPPROTO_UDP;     \
                 socket.ip_family   = IP_V4;              \
            
      #define  INITILIZEV6TCPINFO( socket)                \
                 socket.port_no = 0;                      \
                 socket.xport_proto = PS_IPPROTO_TCP;     \
                 socket.ip_family   = IP_V6;              \
            
      #define  INITILIZEV6UDPINFO( socket)                \
                 socket.port_no = 0;                      \
                 socket.xport_proto = PS_IPPROTO_UDP;     \
                 socket.ip_family   = IP_V6;              \

      #define  INITILIZEV4V6TCPUDPINFO( socket)           \
                 socket.port_no = 0;                      \
                 socket.xport_proto = PS_IPPROTO_TCPUDP;  \
                 socket.ip_family   = IP_V4V6;            \

      
      #define  OTHERPROTO(proto)                          \
               (proto == ds::Sock::Protocol::QDS_TCP ?    \
                ds::Sock::Protocol::QDS_UDP :             \
                ds::Sock::Protocol::QDS_TCP)              \
      /**
            @brief Data type for different states of  Sockets Cache
          */
      typedef enum
      {
        SOCKETS_CACHE_INIT                      = 0,
        /*< Open for making socket request */
        REMOTE_SOCKETS_UNAVAILABLE              = 1,
        /*< No QMI clients, return local ephemeral port */
        REMOTE_SOCKETS_REQUESTED                = 2,
        /*<  Sockets request already in transit  */
        REMOTE_SOCKETS_AVAILABLE                = 3,
        /*< Sockets already present  */
        REMOTE_SOCKETS_NOT_SUPPORTED            = 4,
        /*< Remote service is not supporting this pool */
        
        REMOTE_SOCKETS_AVAILABLE_AND_REQUESTED  = 5,
        /*< Requested few more socket  */
        SOCKETS_CACHE_DISABLED                  = 6
        /*< No QMI clients, return ephemeral port */
      } CacheState;
      
        /**
          @brief Remote Socket Cache Class implementing the Remote Socket
                  Interface to Sockets allocated from AP  .
        */    
      class SocketCache : public ds::Utils::INode 
      {
        private:
          ICritSect *                critSectPtr;
          
          /**
            @brief Remote Socket cache for sockets of family IPV4 and protocol UDP
                   It will have objects of type SockInfo. Ports will be stored in network byte order
          */         
          ds::Utils::List            sockIPv4UDPCache;
          
          /**
            @brief Remote Socket cache for sockets of family IPV4 and protocol TCP
                   It will have objects of type SockInfo. Ports will be stored int network byte order
          */         
          ds::Utils::List            sockIPv4TCPCache;
          
          /**
            @brief Remote Socket cache for sockets of family IPV6 and protocol UDP
                   It will have objects of type SockInfo. Ports will be stored int network byte order
          */         
          ds::Utils::List            sockIPv6UDPCache;
          
          /**
            @brief Remote Socket cache for sockets of family IPV6 and protocol TCP
                   It will have objects of type SockInfo. Ports will be stored int network byte order
          */         
          ds::Utils::List            sockIPv6TCPCache;
          /**
            @brief Maximum sockets allowed in this cache
          */
          uint16                     maxAllowedSocketInCache;

          /**
            @brief Maximum sockets allowed in this cache
          */
          CacheState                 state;

          /**
            @brief State of  scoket cache
          */
          
          protected:
            SocketCache() : critSectPtr(NULL),
                maxAllowedSocketInCache(MAX_SOCKETS),
                refCnt(1),
                weakRefCnt(1)
            { //Default Constructor
            }

            ~SocketCache()
			throw()
            { //Default Constructor
            }

          public:
          
          /** 
            @brief  This member function retrieves number of entries in 
                    Remote Socket Cache count for given protocol and family.
                    Note there are four cache one for each combination of
                    protocol and family i.e IPV4UDPSocket Cache, 
                    IPV6UDPSocket Cache, IPV4TCPSocket Cache and
                    IPV6TCPSocket Cache
          
            @param[in] protocol                  Type of protocol
            @param[in] family                    IP family of socket
          
            @return  number of entries in cache
          */        
          uint8 GetRemoteSockCacheCnt
          (
            ds::Sock::ProtocolType  protocol, 
            ds::AddrFamilyType      family
          );
            
            /** 
            @brief  This member function retrieves socket cache entry
                    located at certain index for given protocol/family
                    remote socket cache
          
            @param[in] index                     Index into cache
            @param[in] protocol                  Type of protocol
            @param[in] family                    IP family of socket
          
            @return  Remote Socket Cache entry
          */         
          SockInfo * GetRemoteSockCacheEntry
          (
            uint8                    index,
            ds::Sock::ProtocolType   protocol, 
            ds::AddrFamilyType       family
          );

              /** 
              @brief  This member function retrieves socket cache entry
                      from front for given protocol/family
                      remote socket cache
                      
              @param[in] protocol                  Type of protocol
              @param[in] family                    IP family of socket
            
              @return  Remote Socket Cache entry
            */         
            SockInfo * GetRemoteSockCacheEntry
            (
              ds::Sock::ProtocolType   protocol, 
              ds::AddrFamilyType       family
            );
            SockInfo * GetRemoteSockCacheEntry
            (
              ds::Sock::ProtocolType   protocol, 
              ds::AddrFamilyType       family,
              int16                    portNumber
            );
            /** 
              @brief  This member function returns pointer to Remote Socket cache
                      for given protocol and family
            
              @param[in] protocol                  Type of protocol
              @param[in] family                    IP family of socket
            
              @return  Pointer to Remote Socket Cache
            */        
          ds::Utils::List * GetSockCache
          (
            ds::Sock::ProtocolType  protocol, 
            ds::AddrFamilyType      family
          ); 
            /**
              @brief Initializes Remote Socket Cache
            
              @param None
            
              @retval None
            */        
           void Init(void); 
            
            /**
              @brief Frees Remote Socket Cache
            
              @param None
            
              @retval None
            */          
           void DeInit(void);
          /** 
            @brief  This member function adds Remote socket information for
                    reserved socket on AP into Socket cache.
          
            @param[in] sockInfoPtr   Remote Socket information
          
            @return  None
          */         
          void UpdateSockInfoToCache
          (
            SockInfo        * sockInfoPtr
          );

          /** 
            @brief  This member function return maximum number of sockets 
                      allowed in this cache
                    
            @return  maximum number of socket allowed.
            */        
          uint16 GetAllowedMaxSockCacheCnt(void);
          
          /** 
            @brief  This member function configure maximum number of sockets 
                  allowed in this cache
          
            @param[in] maxCnt   Maximum count
          
            @return  None.
          */        
          void SetAllowedMaxSockCacheCnt
          (
            uint16 maxCnt
          );
          /**
          @brief  Getter and Setter function for memeber variable
            
          @param None
            
          @retval None
          */          
          CacheState GetCacheState
          (
            void
          );

          void SetCacheState
          (
            CacheState newState
          );
          /** 
            @brief  This member function release ephemeral cache associated 
                       with cache handle
                       
             @param[in] sockReleaseList   Socket list to be released to AP

             @return  None
          */
          void ReleaseSocketCaches
          (
             ps_sys_remote_socket_release_list_type *sockReleaseList
          );

          void ReleaseAllSockets
          (
            ds::Utils::List            *sockCache,
            ps_sys_remote_socket_release_list_type *sockReleaseList
          );

          /** 
               @brief  This member function transfer all remote partial allocated 
               socket into local partial list so that application still use symmetric 
               or adjacent bind
                          
                @param[in] None
   
                @return  None
             */
          void TransferPartialAssignSockets
          (
            ds::Utils::List            *destSockCache,
            ds::Utils::List            *srcSockCache
          );

          /** 
            @brief  Default definations for Inode
          */ 
          void Destructor() 
          {
          
          }
          /* Inherited from INode */
          virtual boolean Process (void*  pUserData)
          {
            (void) pUserData;
            
            return TRUE;
          }
          
          DS_UTILS_IWEAKREF_IMPL_DEFAULTS()

          void * operator new
          (
            unsigned int numBytes
          )
          throw()
          {
            void * buf = NULL;
            PS_SYSTEM_HEAP_MEM_ALLOC(buf, numBytes, void*);

            LOG_MSG_INFO1_1("Remote:: :  Num bytes %x ",  numBytes);
            
            return buf;
          }

          void operator delete
          (
            void *  bufPtr
          )
          throw()
          {
            PS_SYSTEM_HEAP_MEM_FREE(bufPtr);

            LOG_MSG_INFO1_1("Remote:: :  free %x ",  bufPtr);
          }

          
      };

    } /* namespace RemoteSocket */
  } /* namespace SOCK */
} /* namespace PS */

#endif /*PS_SOCK_REMOTESOCKET_CACHE_H*/

