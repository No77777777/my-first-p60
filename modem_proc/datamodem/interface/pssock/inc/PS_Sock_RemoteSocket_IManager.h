#ifndef PS_SOCK_REMOTESOCKET_IMANAGER_H
#define PS_SOCK_REMOTESOCKET_IMANAGER_H
/*===========================================================================

               PS _ SOCK _ SOCKET _ IMANAGER . H

DESCRIPTION

  PS SOCK SOCKET IREMOTEMANGER - Definitions

  
Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/inc/PS_Sock_RemoteSocket_IManager.h#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
09/09/13    BVD    First revision
===========================================================================*/

/*===========================================================================

                                INCLUDE FILES

===========================================================================*/
#include "ds_Utils_ICritSect.h"
#include "ds_Sock_AddrUtils.h"
#include "ds_Utils_INode.h"
#include "ds_Utils_List.h"
#include "ds_Utils_CSSupport.h"
#include "ps_sys_event.h"
#include "ps_sys.h"
#include "ps_system_heap.h"
#include "dssocket.h"

/*===========================================================================

                                 DEFINITIONS

===========================================================================*/

namespace PS
{
  namespace Sock
  {
    namespace RemoteSocket
    {
      #define MAX_EPHEMERAL_CACHES     5
      typedef dss_port_rel_cb_fcn      PortRelCallBack;
      typedef dss_port_req_cb_fcn      PortReqCallBack;
      typedef dss_port_preference_info PortPreference;
      
      /**
        @brief Data type for different states or Remote Socket Manager
      */
      typedef enum
      {
        INIT                          = 0,
        /*< Open for making socket request */
        REMOTE_SERVICE_UNAVAILABLE    = 1,
        /*< No QMI clients, remote socket service is unavailable */
        REMOTE_SERVICE_AVAILABLE_V0   = 2,
        /*< Remote socket service is available,Port preference not supported */
        REMOTE_SERVICE_AVAILABLE_V1   = 3
        /*< Remote socket service is available, Port preference supported */
      } RemoteServiceStatus;    

      typedef enum
      {
        EPHEMERAL_FLAG_NONE           = 0,
        /*< No special processing required*/
        EPHEMERAL_FLAG_UDP_ENCAPS     = 1,
        /*< Enable UDP encapsulation */
        EPHEMERAL_FLAG_MAX            = 0xFF
      } EphemeralSocketFlag;    

      typedef enum
      {
        REMOTE_SOCKET_MIN_CMD             = 0,
        REMOTE_SOCKET_IFACE_UP_IND_CMD    = 1,
        /*Command posted for Iface Up Indication */
        REMOTE_SOCKET_SERVICE_AVAIL_CMD    = 2,
       /*Command posted for Remote service available indication */
        REMOTE_SOCKET_SERVICE_UNAVAIL_CMD  = 3,
        /*Command posted for Remote service un-available indication */
        REMOTE_SOCKET_SOCKETS_ALLOC_CMD   = 4,
        /*Command posted for Remote Sockets allocation indication*/
        REMOTE_SOCKET_CMD_MAX
        /*This command should be last cmd */
      }RemoteSocketCommandId;
      /**
        @brief Structure will be used to store and send remote socket service available info.
        to PS task. Remote Socket Manager process REMOTE_SOCKET_SERVICE_AVAIL_CMD cmd
      */
      typedef struct
      {
        ps_sys_remote_socket_service_avail_type  info;
        void                                     *user_data_ptr;
      }RemoteSocketServiceAvilInfo;
      /**
        @brief Structure will be used to store and send remote socket service unavailable info.
        to PS task. Remote Socket Manager process REMOTE_SOCKET_SERVICE_UNAVAIL_CMD cmd
      */
      typedef struct
      {
        ps_sys_remote_socket_service_avail_type  info;
        void                                    *user_data_ptr;
      }RemoteSocketServiceUnAvilInfo;
      /**
        @brief Structure will be used to store and send allocated remote sockets info.
        to PS task. Remote Socket Manager process REMOTE_SOCKET_SOCKETS_ALLOC_CMD cmd
      */
      typedef struct
      {
        ps_sys_remote_socket_allocated_list_type  sockets;
      }RemoteSocketAllocInfo;

      /**
        @brief Command info structure
      */
      typedef struct
      {
        RemoteSocketCommandId  cmdId;
        union
        {
          RemoteSocketServiceAvilInfo      avilInfo;
          RemoteSocketServiceUnAvilInfo    unAvilInfo;
          RemoteSocketAllocInfo            allocatedSocketInfo;
        }cmdInfo;
      }RemoteSocketCMDInfoType;
      
      /**
        @brief Class representing Cache entry for Remote socket information
               of the sockets created by AP/TE. Each object will have
               port, handle, protocol and family information for remote
               socket
      */      
      class SockInfo: public ds::Utils::INode 
      {
        public:
          SockInfo():refCnt(1),
                     weakRefCnt(1),
                     port(0),
                     handle(0),
                     protocol(0),
                     family(0){}
         
          SockInfo(uint16                 port_num, 
                   uint32                 port_handle, 
                   ds::Sock::ProtocolType protocolVal, 
                   ds::AddrFamilyType     familyVal): 
                   refCnt(1),
                   weakRefCnt(1)
          {
            port       = port_num;
            handle     = port_handle;
            protocol   = protocolVal;
            family     = familyVal;
          }

          void Destructor() 
          {
          
          }

          inline int16 GetPort()
          {
            return port;
          }
          
          inline void SetPort( int16 socketPort)
          {
             port = socketPort;
          }
          
          inline uint32 GetHandle()
          {
            return handle;
          }

          inline void SetHandle( uint32 socketHandle)
          {
             handle = socketHandle;
          }
   
          inline ds::Sock::ProtocolType GetProtocol()
          {
            return protocol;
          }

          inline void SetProtocol( ds::Sock::ProtocolType Socketprotocol)
          {
            protocol = Socketprotocol;
          }

          inline ds::AddrFamilyType GetFamily()
          {
            return family;
          }  

          inline void SetFamily( ds::AddrFamilyType  socketFamily)
          {
             family = socketFamily;
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
            return buf;
          }

          void operator delete
          (
            void *  bufPtr
          )
          throw()
          {
            PS_SYSTEM_HEAP_MEM_FREE(bufPtr);
          }
          
          uint16                   port;
          uint32                  handle;
          ds::Sock::ProtocolType  protocol;
          ds::AddrFamilyType      family;
      };      

      /**
        @brief Interface representing the Remote Socket Manager. Functions
               defined here needs to be implemented by Main class. User of
               Remote Manager would only use this Interface to create 
               object.
      */
      class IManager
      {   
        public:
        
        /**
          @brief Destructor.

          @param None

          @retval None
        */        
        virtual ~IManager() = 0;
        
        /**
          @brief Returns Singleton Manager object.

          @param None

          @retval address  Manager is created successfully
          @retval 0        Out of memory
        */
        static IManager * GetInstance(void); 
        
        /** 
          @brief This member function retrieves socket from the sockets cache based 
                 on given ephemeral handle. If remote sockets are not available then 
                 it will generate ports based on socket Preferences associated with 
                 ephemeral handle. For every socket consumed from the cache it will make 
                 requet to AP/netmger to reserve one more socket.


          @param[in] cacheHandle              Handle representing remote sockets cache
          @param[in] sockInfo                 Socket information
          @param[in] flag                     special processing required 

          @return  TRUE                   On Success.
          @return  FALSE                  On failure. 
        */        
        virtual boolean RetrieveAndRequestSocket
        (
          uint32                     cacheHandle,
          SockInfo                  *sockInfo,
          EphemeralSocketFlag        flag
        ) = 0;
        
        /** 
          @brief  This member function releases the remote socket by sending
                  remote socket handle to AP/TE to close that socket

          @param[in] sockInfo                 Socket information
          @param[in] cacheHandle              Handle representing remote sockets cache

          @return  0            On Success.
          @return  -1           On failure. 
        */            
        virtual int16 ReleaseRemoteSocket
        (
            SockInfo              *sockInfo,
            uint32                 cacheHandle
        ) = 0;
        
#ifdef TEST_FRAMEWORK
        #error code not present
#endif /* TEST_FRAMEWORK */
        /** 
          @brief  This member function returns current state of
                  Remote socket service

          @param None

          @return  Current State of the Remote Socket service
        */           
        virtual RemoteServiceStatus GetState(void) = 0;        
        
        /** 
          @brief  This member function sends UDP encapsulation option
                  of socket to AP/TE

          @param[in] handle                    Remote Socket handle
          @param[in] udpEncapsSockOpt          Is udp encaps option set

          @return  None
        */        
        virtual void SendUDPEncapsSockOpt
        (
          uint32   handle, 
          boolean  udpEncapsSockOpt
        ) = 0;
        /** 
          @brief  This member function initialize remote socket preference

          @param[out] socketPref                Sockets preferences

           @return  None
        */  
        virtual void InitSocketsPref
        (
          PortPreference          * socketPref
        ) = 0;
        /** 
          @brief  This member function will initialize an ephemeral cache based 
                  on port preference. It will send MAX sockets request to AP  If 
                  remote service is available and return success(0). Otherwise it 
                  will return cache handle.

           @param[in] socketPref                Sockets preferences
           @param[in] callBackFunc              Client call back function
           @param[in] userData                  User data

           @return     Handle                   Valid handle if pref arleady requested. 
                                                Call back will  not be invoked
                       0                        Request accepted. Handle will be notified via 
                                                call back.
                      -1                        On Failure.
        */  
        virtual int32 RequestSocketsPref
        (
          PortPreference          * socketPref,
          PortReqCallBack           callBackFunc,
          void                    * userData
        ) = 0;
        /** 
          @brief  This member function release ephemeral cache associated with cache handle

           @param[in] handle                    Ephemeral cache handle
           @param[in] CallBackFunc              Client call back function
           @param[in] userData                  User data

           @return  None
        */
        virtual int16   ReleaseSocketsPref
        (
          int32                 handle,
          PortRelCallBack       callBackFunc,
          void                * userData
        ) = 0;
        
      };
      
      namespace RemoteSocketUtils
      {
      
          /** 
            @brief  This member function converts protocol from PS Protocol type
                    to dS Sock Type
          
            @param[in] protocol   PS Type protocol
          
            @return  DS Sock type Protocol
          */           
          ds::Sock::ProtocolType ConvertToDSSockProtocol
          (
            ps_ip_protocol_enum_type protocol
          );
          
          /** 
            @brief  This member function converts family from PS IP family type
                    to dS Addr Type family
          
            @param[in] family   PS Type IP Family
          
            @return  DS Addr type IP family
          */  
          ds::AddrFamilyType ConvertToDSSockFamily
          (
            ip_version_enum_type family
          );

        /** 
          @brief  This member function converts protocol from DS Sock type
                  to PS type protocol

          @param[in] protocol   DS Sock type protocol

          @return  PS type Protocol
        */      
        ps_ip_protocol_enum_type ConvertToPSProtocol
        (
          ds::Sock::ProtocolType  protocol
        );
          
        /** 
          @brief  This member function converts family from DS Addr type
                  to PS type family

          @param[in] family   DS Addr type family

          @return  PS type family
        */          
        ip_version_enum_type ConvertToPSFamily
        (
          ds::AddrFamilyType family
        );   
      }
      
    } /* namespace RemoteSocket */
  } /* namespace SOCK */
} /* namespace PS */

#endif /* PS_SOCK_REMOTESOCKET_IMANAGER_H */
