#ifndef PS_SOCK_REMOTESOCKET_Manager_H
#define PS_SOCK_REMOTESOCKET_Manager_H
/*===========================================================================

                 PS _ SOCK _ REMOTESOCKET _ Manager . H

DESCRIPTION

  PS SOCK REMOTESOCKET MANGER - Definitions

  
Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_RemoteSocket_Manager.h#2 $
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
#include "ps_sys_ioctl.h"
#include "ps_sys.h"
#include "PS_Sock_Platform_Socket.h"
#include "PS_Sock_RemoteSocket_IManager.h"
#include "ps_in.h"
#include "dssocket.h"
#include "ps_handle_mgr.h"
#include "PS_Sock_Platform_LocalEphemeralPortGen.h"
#include "PS_Sock_RemoteSocket_Ephemeral.h"

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
      /**
        @brief Remote Socket Manager Class implementing the Remote Socket
               Manager Interface.
      */    
      class Manager : public IManager
      {
        private:
        
        /**
          @brief Singleton instance of Remote Socket Manager.
        */          
        static Manager  * instance;
        
        /**
          @brief Private Constructor Singleton instance.
        */         
        Manager() 
        { //Private Constructor
        }
        
        ICritSect *                     critSectPtr;
        /**
          @brief Represents current state of Remote Socket service
        */        
        RemoteServiceStatus            remoteServiceStatus;
        
        /**
          @brief Remote socket manger registers for iface up indication on Null
                 iface and requests for max remote sockets when first call is up. 
                 psIfaceEvHandlePtr is handle to iface up event registration
        */         
        void                          * psIfaceEvHandlePtr;
        /**
          @brief List of ephemeral sockets caches
        */   
        ds::Utils::List               * EphPortMgrInstList;
            
        
        /** 
          @brief  This member function returns PS Sock Socket pointer from 
                   its handle
                  
          @param[in] ps_sock_platform_handle Platform Socket handle
          
          @return  Pointer to PS Sock Socket
        */        
        Socket * GetPlatformSocketPtr
        (
          uint32  ps_sock_platform_handle
        );    
        
        /** 
          @brief  This member function goes through all PS Sock Sockets
                  whose ports are between 32000 to 36999, it will
                  send request to AP/TE to block those ports and return
                  remote socket handle which would be stored in each
                  socket on modem

          @param None

          @return  Number of Remote Sockets requested
        */         
        int ReserveAllocatedSocket
        (
          void
        );

        /** 
          @brief  This member function goes through all PS Sock Sockets
                  and set 0xFF into remote socket handle those have valid 
                  remote handle set. 
          @param None

          @return  None
        */         
        void InvalidateAssignedRemoteSocketHandles
        (
          void
        );

        
        /** 
          @brief  This member function goes through all allocated ephemeral
                  caches in Remote socket manager and send a request to AP to
                  reserve ports based on sockets preferences associated with 
                  ephemeral cache.

          @param None

          @return  None
        */  
        void ReserveAllSocketCaches
        (
          void
        );


        /** 
          @brief  This member function goes through all allocated ephemeral
                  caches in Remote socket manager and Free up all resouces 
          @param None

          @return  None
        */  
        void UnAvailAllRemoteSocketCaches
        (
          void
        );

        
        /**
          @brief Returns ephemeral cache from cache handle.

          @param request_handle[in]    epehemeral cache handle

          @retval address  of ephemeral cache 
          @retval NULL     Handle is not associated with cache
        */
        EphemeralPortManager *  GetEphemeralMgrInstFromHandle
        (
          uint32  requestHandle
        );
        /**
          @brief Returns legacy ephemeral cache from cache handle.

          @param request_handle[in]    epehemeral cache handle

          @retval address  of ephemeral cache 
          @retval NULL     Handle is not associated with cache
        */
        EphemeralPortManager *  GetLegacyEphemeralMgrInst
        (
        );
        /** 
          @brief  This member function checks if remote socket request is
                  permissible and if so it will send request to AP/TE
                  to allocate remote sockets
                  
          @param[in] remoteSocketsList   Structure containing sockets to be reserved
          @param[in] isLegacyEphemeralReq Is request for legacy ephemeral cache

          @return  Status of remote service 
        */ 
        RemoteServiceStatus RequestRemoteSocketList
        (
           ps_sys_remote_socket_request_type * remoteSocketsList,
           boolean                             isLegacyEphemeralReq
        );

        /** 
          @brief  This member function goes through all PS Sock Sockets and finds
                  the matching socket and provides the remote socket handle to
                  matching PS Sock Socket.
                  
          @param[in] remoteSockInfoPtr   Structure containing socket reserved by AP

          @return  None
        */        
        void AddHandleToSocket
        (
          ps_sys_remote_socket_allocated_type * remoteSockInfoPtr
        );
        
        public:
        /**
          @brief Returns Singleton Manager object.

          @param None

          @retval address  Manager is created successfully
          @retval 0        Out of memory
        */
        static Manager * GetInstance(void);   
        
        /**
          @brief Initializes Remote Socket Manager

          @param None

          @retval None
        */        
        static void Init(void); 
        
        /**
          @brief Frees Remote Socket Manager

          @param None

          @retval None
        */          
        static void DeInit(void);
        
        /**
          @brief Destructor of Remote Socket Manager

          @param None

          @retval None
        */      
        ~Manager()
        {
        }        
        /** 

        @brief  This function is called in PS context to execute the remote socket commands
        
        @param[in] cmd        command type
        @param[in] user_data_ptr      User data ptr
        
        @return  None
        */
        static void RemoteSocketCmdHandler 
        (
          ps_cmd_enum_type   cmd,
          void              *user_data_ptr
        );

        /** 
          @brief  Remote socket manger registers for iface up indication and
                  requests for max remote sockets when first call is up. 
                  psIfaceEvHandlePtr is handle to iface up event registration

          @param[in] psIfacePtr        PS Iface ptr
          @param[in] psIfaceEvent      PS iface event
          @param[in] psIfaceEventInfo  Event info
          @param[in] userDataPtr       Ptr to User Data

          @return  None
        */           
        static void PSIfaceEventCback
        (
          ps_iface_type *             psIfacePtr,
          ps_iface_event_enum_type    psIfaceEvent,
          ps_iface_event_info_u_type  psIfaceEventInfo,
          void *                      userDataPtr
        );   
        /** 
          @brief  Remote socket manger registers for iface up indication and
                  requests for max remote sockets when first call is up. 
          @return  None
        */           
        void PSIfaceEventHandler
        (
        
        );

        /** 
          @brief  Remote socket manger registers for AP/TE availabe 
                  event at bootup. On recieving the event, it will check
                  if there were any sockets created before AP/TE
                  was available and if so it will request remote sockets
                  matching those sockets. It will also send request for
                  reserving sockets for its cache.

          @param[in] tech_type        Technology type
          @param[in] event_name       PS Sys Event
          @param[in] event_info_ptr   Event info
          @param[in] user_data_ptr    Ptr to User Data

          @return  None
        */         
        static void RemoteSocketServiceStatusCB
        (
          ps_sys_tech_enum_type              tech_type,
          ps_sys_event_enum_type             event_name,
          ps_sys_subscription_enum_type      subscription_id,
          void                             * event_info_ptr,
          void                             * user_data_ptr
        );
        /** 
          @brief  Remote socket manger registers for AP/TE availabe 
                  event at bootup. On recieving the event, it will check
                  if there were any sockets created before AP/TE
                  was available and if so it will request remote sockets
                  matching those sockets. It will also send request for
                  reserving sockets for its cache.

          @param[in] event_info_ptr   Event info
          @return  None
        */         
        void RemoteSocketServiceAvailHandler
        (
          ps_sys_remote_socket_service_avail_type      * serviceAvailptr
        );

        /** 
          @brief  Remote socket manger registers for AP/TE unavailabe 
                  event at bootup. On recieving the event, it will set 
                  remote service status as un-available and clear-up
                  all caches

          @param[in] event_info_ptr   Event info
          @return  None
        */         
        void RemoteSocketServiceUnavailHandler
        (
          ps_sys_remote_socket_service_avail_type      * serviceAvailptr
        );

        /** 
          @brief  Remote socket manger registers for callback for ioctl
                  PS_SYS_IOCTL_REMOTE_SOCKET_ALLOCATED. AP/TE would
                  send indication informing the sockets have been reserved.
                  On recieving this callback, remote socket manager would
                  go through all sockets reserved and depending on type
                  of socket, it would either add socket to its cache or
                  it will go through all PS Sock Sockets and find the 
                  matching socket and sets its Remote Socket Handle                  

          @param[in] ioctl_name        Ioctl name
          @param[in] arg_val_ptr       Ioctl information
          @param[in] subscription_id   Subscription id
          @param[out] ps_errno         Error information

          @return  0 on SUCCESS otherwise FAILURE
        */           
        static int16 RemoteSocketAllocIoctl
        (
          ps_sys_ioctl_enum_type          ioctl_name,
          void                          * arg_val_ptr,
          ps_sys_subscription_enum_type   subscription_id,
          int16                         * ps_errno
        );
        /** 
          @brief  Remote socket manger registers for callback for ioctl
                  PS_SYS_IOCTL_REMOTE_SOCKET_ALLOCATED. AP/TE would
                  send indication informing the sockets have been reserved.
                  On recieving this callback, remote socket manager would
                  go through all sockets reserved and depending on type
                  of socket, it would either add socket to its cache or
                  it will go through all PS Sock Sockets and find the 
                  matching socket and sets its Remote Socket Handle                  
          @param[in] sockAllocatedPtr     information
          
          @return  0 on SUCCESS otherwise FAILURE
        */           
        int16 RemoteSocketAllocIoctlHandler
        (
          ps_sys_remote_socket_allocated_list_type      * sockAllocatedPtr
        );

        /** 
          @brief  This member function release ephemeral cache associated with cache handle

           @param[in] handle                    Ephemeral cache handle
           @param[in] CallBackFunc              Client call back function
           @param[in] userData                  User data

           @return  None
        */
        int16   ReleaseSocketsPref
        (
          int32                 handle,
          PortRelCallBack       callBackFunc,
          void                * userData
        );
        /**
          @brief Invoke client release call back function

          @param[in] CallBackFunc              Client call back function
          @param[in] userData                  User data

          @retval None
        */ 
        void  NotifySocketsReleased
        (
          PortRelCallBack       callBackFunc,
          void                * userData
        );
        /**
          @brief Invoke client allocated call back function

          @param[in] CallBackFunc              Client call back function
          @param[in] userData                  User data

          @retval None
        */ 
        void  NotifySocketsAllocated
        (
          EphemeralPortManager  * cachePtr,
          boolean                 isAnySocketAlloc
        );

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
        int32 RequestSocketsPref
        (
          PortPreference          * socket_pref,
          PortReqCallBack           cb_fn,
          void                    * userdata
        );
        /** 
          @brief  This member function initialize remote socket preference

          @param[out] socketPref                Sockets preferences

           @return  None
        */  
        void InitSocketsPref
        (
          PortPreference          * socketPref
        ) ;
        /** 
          @brief  This member function check socket preference

          @param[in] socketPref           Sockets preferences
          @param[out] handle              Ephemeral cache handle

           @return  TRUE                  Socket preference matched
                    FALSE                 Otherwise
        */
        boolean CheckSocketsPref
        (
          PortPreference          * socket_pref,
          uint32                  * handle
        );  
        /** 
          @brief  This member function retrieves socket from the cache corresponding 
                  to handle. If there are no entries in cache, it will generate one 
                  random port Based on port preference  and will return it to called.
                  For every socket consumed from the cache it will make 
                  requet to AP/netmger to reserve one more socket. This 
                  way total entries in cache is max sockets (20 sockets)

          @param[in] cacheHandle              Handle representing remote sockets cache
          @param[in] sockInfo                  Socket information
          @param[in] flag                      Special processing required 

          @return  TRUE            On Success.
          @return  FALSE           On failure. 
        */         
        boolean RetrieveAndRequestSocket
        (
          uint32                     cacheHandle,
          SockInfo                  *sockInfo,
          EphemeralSocketFlag        flag
        );
        /** 
          @brief  This member function releases the remote socket by sending
                  remote socket handle to AP/TE to close that socket

          @param[in] sockInfo                  Socket information
          @param[in] cacheHandle               Handle representing remote sockets cache

          @return  0            On Success.
          @return  -1           On failure. 
        */            
        int16 ReleaseRemoteSocket
        (
          SockInfo               *sockInfo,
          uint32                 cacheHandle
        );
        /** 
          @brief  This member function returns current state of
                  Remote socket service

          @param None

          @return  Current State of the Remote Socket service
        */           
        RemoteServiceStatus GetState(void); 
        
        /** 
          @brief  This member function sends UDP encapsulation option
                  of socket to AP/TE

          @param[in] handle                    Remote Socket handle
          @param[in] udpEncapsSockOpt          Is udp encaps option set

          @return  None
        */        
        void SendUDPEncapsSockOpt
        (
          uint32   handle, 
          boolean  udpEncapsSockOpt
        );
        
#ifdef TEST_FRAMEWORK
        #error code not present
#endif /* TEST_FRAMEWORK */
        
      };
      
    } /* namespace RemoteSocket */
  } /* namespace SOCK */
} /* namespace PS */

#endif /* PS_SOCK_REMOTESOCKET_Manager_H */
