#ifndef PS_SOCK_REMOTESOCKET_EPHEMERAL_H
#define PS_SOCK_REMOTESOCKET_EPHEMERAL_H
/*===========================================================================

                 PS _ SOCK _ REMOTESOCKET _ EPHEMERAL . H

DESCRIPTION

  PS SOCK REMOTESOCKET EPHEMERAL - Definitions

  
Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_RemoteSocket_Ephemeral.h#2 $
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
#include "PS_Sock_RemoteSocket_Cache.h"

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
          @brief Socket Cache Ephemeral Class implementing the internal routine to retrieve/store 
                   sockets received from AP.It also generate modem local ephemeral ports if remote 
                   sockets are unavailable 
        */    
      class EphemeralPortManager : public SocketCache
      {
        private:
        /**
              @brief  reference count assosiated with this ephemeral cache
            */

          uint8                refCount;
        /**
          @brief  handle assosiated with this ephemeral cache
        */
          uint32               socketsEphemeralHandle;
        
        /**
          @brief  Preference assosiated with this ephemeral cache
        */
          PortPreference       socketPreference;

        /**
          @brief  List of all remote allocated sockets whose companion sockets has retrieved.
        */
          ds::Utils::List      partialRemoteSockCache;

        /**
          @brief  List of all local allocated sockets whose companion sockets has retrieved.
        */
          ds::Utils::List      partialLocalSockCache;
        /**
          @brief Client call back function 
        */        
          PortReqCallBack      callBackFunction;
        /**
        @brief Client user data
        */    
          void                 * userData;
        /**
        @brief Client call back invoke status 
        */    
          boolean                      callBackPending;

        public:
          EphemeralPortManager() 
          { //Default Constructor
          }
          
          ~EphemeralPortManager()
		  throw()
          { //Default Destructor
          }      
          /**
          @brief constructor
            
          @param None
            
          @retval None
          */        
          EphemeralPortManager
          (
            PortPreference          * port_pref,
            PortReqCallBack           cb_fn,
            void                    * userdata
          ); 
             
          /**
          @brief Initializes Remote Socket Cache
            
          @param None
            
          @retval None
          */        
           void Init(void); 
            
          /**
          @brief  De-Initializes Remote Socket Cache
            
          @param None
            
          @retval None
          */          
           void DeInit(void);
          /**
          @brief  Match socket preference
            
          @param [in]pref  Preference to be matched 
            
          @retval TRUE     Preference matched
                     FALSE    Preference not matched
          */  
          boolean  MatchSocketsPreference
          (
             PortPreference  *pref
          );
          /**
          @brief  Retrieve all socket those were allocated in absence of remote 
                    socket service from ephemeral cache
            
          @param [in]remoteSocketsList  Allocated  socket list
            
          @retval TRUE     Preference matched
                     FALSE    Preference not matched
          */  
          void  RetrieveLocalAllocatedSockets
          (
            ps_sys_remote_socket_request_type * remoteSocketsList
          );
          /**
          @brief  Initialize a max sockets request for AP based on sockets preference 
                    associated with ephemeral cache
            
          @param [in]remoteSocketsList  Allocated  socket list
            
          @retval TRUE     SUCCESS
                     FALSE    FAILURE
          */  
          boolean  InitilizeMaxSocketsRequest
          (
            ps_sys_remote_socket_request_type * remoteSocketsList
          );
          /**
          @brief  Initialize a max sockets request for AP based on sockets preference 
                    associated with ephemeral cache
            
          @param [in]remoteSocketsList  Allocated  socket list
          
          @retval TRUE     SUCCESS
                     FALSE    FAILURE

          */  
          boolean InitilizeEphemeralList
          (
            ps_sys_remote_socket_list_type    * remoteSocketsList
          );
          /**
          @brief  Initialize a max sockets request for AP based on sockets preference 
                    associated with adjacent ephemeral cache
            
          @param [in]remoteSocketsList  Allocated  socket list
            
          @retval TRUE     SUCCESS
                     FALSE    FAILURE
          */  
          boolean InitilizeAdjEphemeralList
          (
            ps_sys_remote_socket_list_type    * remoteSocketsList
          );
          /**
          @brief  Initialize a max sockets request for AP based on sockets preference 
                    associated with Symmetric ephemeral cache
            
          @param [in]remoteSocketsList  Allocated  socket list
            
          @retval TRUE     SUCCESS
                     FALSE    FAILURE
          */  
          boolean InitilizeSymEphemeralList
          (
            ps_sys_remote_socket_list_type    * remoteSocketsList
          );
          /**
          @brief    Initialize a sockets request for AP based on requested sockets
            
          @param ReqSockInfo[in]         Requested Sock info
                     remoteSocketsList[in]  Allocated  socket list
            
          @retval None
          */  
          void  InitilizeRemoteSocketRequest
          (
            SockInfo                            *ReqSockInfo,
            ps_sys_remote_socket_request_type   *remoteSocketsList
          );
          /**
          @brief   Associated ephemeral cache is legacy cache
            
          @param None
            
          @retval TRUE     Preference matched
                     FALSE    Preference not matched
          */  
          boolean IsLegacyEphemeralCache
          (
            void
          );
          /** 
            @brief  This member function retrieves socket from the cache corresponding 
                    to handle. If there are no entries in cache, it will generate one 
                    random port Based on port preference  and will return it to called.
                    For every socket consumed from the cache it will make 
                    requet to AP/netmger to reserve one more socket. This 
                    way total entries in cache is max sockets (20 sockets)
          
            @param [in]sockInfo                 Socket information
            @param [in]flag                      Special processing required 
            @param [in]remoteSocketsList   Socket list to be requested to AP
            @param [in]sockReleaseList   Socket list to be released to AP
              
            @return TRUE           On Success.
            @return  FALSE         On failure. 
          */ 
          boolean RetrieveAndRequestSocket
          (
            EphemeralSocketFlag                     bindFlag,
            SockInfo                               *sockInfo,
            ps_sys_remote_socket_request_type      *remoteSocketsList,
            ps_sys_remote_socket_release_list_type *sockReleaseList
          );

          boolean RetrieveSockFromDefaultCacheList
          (
            EphemeralSocketFlag                     bindFlag,
            SockInfo                               *sockInfo,
            ps_sys_remote_socket_request_type      *remoteSocketsList,
            ps_sys_remote_socket_release_list_type *sockReleaseList
          );

          boolean RetrieveSockFromSpecificCacheList
          (
            EphemeralSocketFlag                     bindFlag,
            SockInfo                               *sockInfo,
            ps_sys_remote_socket_request_type      *remoteSocketsList,
            ps_sys_remote_socket_release_list_type *sockReleaseList
          );
          SockInfo * RetrieveSockCacheEntry
          (
            ds::Sock::ProtocolType   protocol, 
            ds::AddrFamilyType       family,
            int16                    portNumber,
            ds::Utils::List *        sockCachePtr,
            boolean                  getAndRemoveFlag
          );
          
          SockInfo * RetrieveSockFromPartialCaches
          (
              ds::Sock::ProtocolType   protocol, 
              ds::AddrFamilyType       family,
              int16                    portNumber,
              boolean                  getAndRemoveFlag
          );
          /** 
            @brief  This member function releases the remote socket by sending
                    remote socket handle to AP/TE to close that socket
          
            @param[in] sockInfo             Socket information
            @param[in] sockReleaseList   Socket list to be released to AP
          
            @return  0            On Success.
            @return  -1           On failure. 
          */  
          void ReleaseRemoteSocket
          (
            SockInfo                               *sockInfo,
            ps_sys_remote_socket_release_list_type *sockReleaseList
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
          
          /** 
            @brief  This member function release ephemeral cache associated 
                       with cache handle
                       
             @param[in] None

             @return  None
          */
          void ReleaseUnusedSocketCaches
          (
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
          );

          /** 
            @brief  This member function validate the remote socket list
                       
             @param[in]    sockInfoPtr          Socket to be validated
             @param[out]  sockReleaseInfo   Socket list to be released to AP

             @return  None
             
          */
          void ValidateReceivedRemoteSock
          (
            ps_sys_remote_socket_allocated_list_type   * sockAllocatedPtr,
            ps_sys_remote_socket_release_list_type     * sockReleaseInfo
          );
          
          /** 
            @brief  This member function will check is same port is in use 
                       
             @param[in]    sockInfoPtr          Socket to be validated

             @return  TRUE if it is in use
             
          */
          boolean IsSockInUse
          (
           SockInfo   * sockInfoPtr
          );
          /** 
            @brief  This member function adds Remote socket information for
                    reserved socket on AP into Socket cache.
          
            @param[in] remoteSockInfoPtr   Remote Socket information
          
            @return  None
          */         
          void UpdateSockInfoToCache
          (
            ps_sys_remote_socket_allocated_type * remoteSockInfoPtr
          );
          /**
          @brief  Increment reference count
            
          @param None
            
          @retval None
          */          
          void  AddRefCount();
          /**
          @brief  Decrement reference count
            
          @param None
            
          @retval None
          */          
          void  DecRefCount();
          /**
          @brief  Get the reference count
            
          @param None
            
          @retval None
          */          
          uint8 GetRefCount();
           /**
           @brief  Getter and Setter function for memeber variable
             
           @param None
             
           @retval None
           */          
           void SetSocketsEphemeralHandle
          (
            uint32  handle
          );              
          
          uint32 GetSocketsEphemeralHandle
          (
            void
          );  
          
          void SetSocketsPreference
          (
            PortPreference  *pref
          );              
          
          PortPreference GetSocketsPreference
          (
            void
          );

          void  SetClientCallBackFunc
          (
            PortReqCallBack           callback
          );
          
          PortReqCallBack  GetClientCallBackFunc
          (
          );
          
          void  SetClientCallBackData
          (
            void           *userdata
          );
          
          void*  GetClientCallBackData
          (
          );

          boolean  IsClientCallBackPending
          (
          );
          
          void  SetClientCallBackPending
          (
            boolean  flag
          );
          
          uint8 GetPartialCacheCount
          (
              boolean        isLocal
          ) ;

      };/* Class EphemeralPortManager */

    } /* namespace RemoteSocket */
  } /* namespace SOCK */
} /* namespace PS */


#endif /*PS_SOCK_REMOTESOCKET_EPHEMERAL_H*/

