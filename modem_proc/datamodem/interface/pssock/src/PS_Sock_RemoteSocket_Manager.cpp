/*===========================================================================
               PS _ SOCK _ REMOTESOCKET _ Manager . CPP

DESCRIPTION

  PS SOCK REMOTESOCKET MANAGER - Implementation.

FUNCTIONS

Copyright (c) 2015 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
===========================================================================*/
/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/pssock/src/PS_Sock_RemoteSocket_Manager.cpp#2 $
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/20/15    bvd     Initial version 
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
#include "ps_sys_event.h"
#include "ps_crit_sect.h"
#include "ps_sys_conf.h"
#include "ps_sys.h"
#include "ps_iface_defs.h"
#include "ps_iface.h"
#include "ps_ifacei_event.h"
#include "ps_handle_mgr.h"
#include "PS_Sock_Platform_SocketFactory.h"
#include "ps_mem.h"
#include "ds_Addr_Def.h"
#include "ds_Sock_Def.h"
#include "ps_svc.h"

using namespace PS::Sock::RemoteSocket;
using namespace ds::Error;
using namespace ds::Utils;

Manager * Manager::instance = NULL;

/*===========================================================================

                         PUBLIC CLASS FUNCTIONS

===========================================================================*/
void Manager::Init(void)
{
  int16                         ps_errno;
  ps_iface_event_enum_type      iface_ev_list[] = { IFACE_UP_EV }; 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == instance) 
  {
    /*--------------------------------------------------------------------
     Create instance of Remote Socket Manager if it hasnt been created
    ---------------------------------------------------------------------*/  
    instance = new Manager();
    if (0 == instance) {
      DS_UTILS_ERROR_FATAL("Manager::CreateInstance(): No mem for Manager");
      return;
    }                  
             
    /*--------------------------------------------------------------------
      Register for AP/TE available event
    ---------------------------------------------------------------------*/               
    ps_sys_event_reg_ex(PS_SYS_TECH_ALL, 
                        PS_SYS_EVENT_REMOTE_SOCKET_SERVICE_AVAIL,
                        PS_SYS_PRIMARY_SUBS, //Subscription agnostic, but use primary for consistency
                        Manager::RemoteSocketServiceStatusCB, 
                        instance, 
                        &ps_errno);         
                     
    /*--------------------------------------------------------------------
      Register for ioctl callback when AP indicates, it has reserved
      requested sockets
    ---------------------------------------------------------------------*/   
    ps_sys_ioctl_register_ioctl_cback_ex(PS_SYS_IOCTL_REMOTE_SOCKET_ALLOCATED,
                                         Manager::RemoteSocketAllocIoctl,
                                         PS_SYS_PRIMARY_SUBS,
                                         &ps_errno);
                                         
   /*------------------------------------------------------------------------
     Subscribe to iface up events on NULL iface
   ------------------------------------------------------------------------*/
    instance->psIfaceEvHandlePtr = 
      ps_iface_alloc_event_handle(NULL, Manager::PSIfaceEventCback, instance);
    if (NULL == instance->psIfaceEvHandlePtr)
    {
      DS_UTILS_ASSERT(0);
    }

    if (0 != ps_iface_subscribe_event_list
             (
               instance->psIfaceEvHandlePtr,
               iface_ev_list,
               sizeof(iface_ev_list) / sizeof(ps_iface_event_enum_type)
              )
        )
    {
      DS_UTILS_ERROR_FATAL("Couldn't register for iface up event");
    }
    /*--------------------------------------------------------------------
      Initialize Remote Socket Manager State Machine to Init state
    ---------------------------------------------------------------------*/     
    instance->remoteServiceStatus = INIT;   
 
    if (AEE_SUCCESS != DS_Utils_CreateInstance (NULL,
                                                AEECLSID_CCritSect,
                                                (void **) &(instance->critSectPtr)))
    {
      DS_UTILS_ERROR_FATAL("Socket::AllocateMemberObjects(): Cannot create crit sect");
    }
    /*--------------------------------------------------------------------
      Initialize Modem ephemeral Generator
    ---------------------------------------------------------------------*/ 
    LocalEphemeralPortGen::Init();
    /*--------------------------------------------------------------------
      Initialize Cache handles
    ---------------------------------------------------------------------*/ 
    ps_handle_mgr_init_client(PS_HANDLE_MGR_CLIENT_SOCKET_EPHEMERAL_CACHE,
                              MAX_EPHEMERAL_CACHES,
                              100,
                              0x7FFF);
    /*--------------------------------------------------------------------
       Create a empty cache list 
    ---------------------------------------------------------------------*/
   instance->EphPortMgrInstList = new List();

    if ( NULL == instance->EphPortMgrInstList)
    {
      LOG_MSG_ERROR_0("Manager::Init :"
                     "Insufficient memory ");
      DS_UTILS_ASSERT(0);
    }    
    /*--------------------------------------------------------------------
       Register command handler with PS task
    ---------------------------------------------------------------------*/
    (void) ps_set_cmd_handler(PS_REMOTE_SOCKET_CMD, Manager::RemoteSocketCmdHandler);
      
  }
} /* Manager::Init */

void Manager::DeInit(void)
{
  int16                    ps_errno;
  EphemeralPortManager   * EphPortMgrInst  = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 != instance) 
  {
    ps_sys_event_dereg_ex(PS_SYS_TECH_ALL, 
                          PS_SYS_EVENT_REMOTE_SOCKET_SERVICE_AVAIL,
                          PS_SYS_PRIMARY_SUBS,
                          Manager::RemoteSocketServiceStatusCB, 
                          &ps_errno);                       
    
    /*---------------------------------------------------------------------
      Unsubscribe from iface up event
    ----------------------------------------------------------------------*/
    ps_iface_free_event_handle(instance->psIfaceEvHandlePtr);
    /*--------------------------------------------------------------------
      De-Initialize all caches
    ---------------------------------------------------------------------*/ 
    while (instance->EphPortMgrInstList->Count() > 0)
    {
       EphPortMgrInst  = 
        static_cast <EphemeralPortManager *> ( instance->EphPortMgrInstList->PopFront());
      if( NULL != EphPortMgrInst )
      {
        EphPortMgrInst->DeInit();
        delete EphPortMgrInst ;
      }
    }
    delete instance->EphPortMgrInstList;
    
    instance->EphPortMgrInstList = NULL;

    DS_UTILS_RELEASEIF(instance->critSectPtr);
    
    delete instance;
    instance = NULL;
    
  }
} /* Manager::DeInit */

void Manager::RemoteSocketCmdHandler 
(
  ps_cmd_enum_type   cmd,
  void              *user_data_ptr
)
{
  RemoteSocketCMDInfoType     *cmdInfoPtr;
  Manager                     *instance;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_UTILS_ASSERT (cmd == PS_REMOTE_SOCKET_CMD);

  if ( NULL == user_data_ptr )
  {
    return;
  }
  /*-----------------------------------------------------------------------
   Typecast User Data Ptr into Remote Socket command info
    -----------------------------------------------------------------------*/ 
  cmdInfoPtr = ( RemoteSocketCMDInfoType *)user_data_ptr;
  /*-----------------------------------------------------------------------
   Get Remote Socket Manager instance
    -----------------------------------------------------------------------*/ 
  instance = Manager::GetInstance();

  do
  {

   LOG_MSG_INFO1_3("RemoteSocketCmdHandler(): "
       "cmdInfoPtr 0x%x command Id %d instancePtr 0x%x", 
       cmdInfoPtr, cmdInfoPtr->cmdId, instance);
       
    if (NULL == instance)
    {
       break;
    }
    
    switch( cmdInfoPtr->cmdId )
    {
      case REMOTE_SOCKET_IFACE_UP_IND_CMD:
      {
       instance->PSIfaceEventHandler();
       
       break;
      }
      
      case REMOTE_SOCKET_SERVICE_AVAIL_CMD:
      {
       instance->RemoteSocketServiceAvailHandler(&cmdInfoPtr->cmdInfo.avilInfo.info);
       break;
      }
    
      case REMOTE_SOCKET_SERVICE_UNAVAIL_CMD:
      {
       instance->RemoteSocketServiceUnavailHandler(&cmdInfoPtr->cmdInfo.unAvilInfo.info);
       break;
      }
    
      case REMOTE_SOCKET_SOCKETS_ALLOC_CMD:
      {
       instance->RemoteSocketAllocIoctlHandler(&cmdInfoPtr->cmdInfo.allocatedSocketInfo.sockets);
       break;
      }
      default:
      {
        break;
      }
    }
  }while(0);
  
  /*-----------------------------------------------------------------------
   Free command Info ptr allocated in callback function
    -----------------------------------------------------------------------*/ 
  PS_SYSTEM_HEAP_MEM_FREE(cmdInfoPtr);
  
  return;
}/*RemoteSocketCmdHandler*/

void Manager::PSIfaceEventCback
(
  ps_iface_type *             psIfacePtr,
  ps_iface_event_enum_type    psIfaceEvent,
  ps_iface_event_info_u_type  psIfaceEventInfo,
  void *                      userDataPtr
)
{
  RemoteSocketCMDInfoType  * cmdInfoPtr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO1_3("Manager::PSIfaceEventCback(): "
                   "Ev %d Iface 0x%x userDataPtr 0x%x", 
                   psIfaceEvent, psIfacePtr, userDataPtr);
  /*-----------------------------------------------------------------------
    Allocate memory for event info
  -----------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(cmdInfoPtr, sizeof(RemoteSocketCMDInfoType),
                           RemoteSocketCMDInfoType* );
  
  if (NULL == cmdInfoPtr)
  {
    LOG_MSG_ERROR_0("PSIfaceEventCback: Unable to allocate memory");
    return;
  }
  /*-----------------------------------------------------------------------
    Set command ID
  -----------------------------------------------------------------------*/
  cmdInfoPtr->cmdId = REMOTE_SOCKET_IFACE_UP_IND_CMD;
  
  ps_send_cmd (PS_REMOTE_SOCKET_CMD, (void*)cmdInfoPtr);
}/* PSIfaceEventCback */

void Manager::PSIfaceEventHandler
(

)
{
  SocketFactory     * platformSockFactoryPtr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*---------------------------------------------------------------------
    Avoid processing subsequent Iface event processing. There is possibility where
    Iface event call back has called but not processed yet
  ----------------------------------------------------------------------*/
  if( NULL == psIfaceEvHandlePtr )
  {
    return;
  }
  platformSockFactoryPtr = SocketFactory::CreateInstance();
  DS_UTILS_ASSERT( 0 != platformSockFactoryPtr);
  
  /*--------------------------------------------------------------------
    Request Platform socket factory to reserve ephemeral ports if they were not reserved .
  ---------------------------------------------------------------------*/  
  platformSockFactoryPtr->ReserveDefaultEphemeralPorts();
  
  DS_UTILS_RELEASEIF(platformSockFactoryPtr);
  /*---------------------------------------------------------------------
    Unsubscribe from iface up event
  ----------------------------------------------------------------------*/
  ps_iface_free_event_handle(psIfaceEvHandlePtr); 

  /*---------------------------------------------------------------------
    Reset the pointer 
  ----------------------------------------------------------------------*/
  psIfaceEvHandlePtr = NULL;

  return;
} /* Manager::PSIfaceEventHandler() */

void Manager::RemoteSocketServiceStatusCB
(
  ps_sys_tech_enum_type              tech_type,
  ps_sys_event_enum_type             event_name,
  ps_sys_subscription_enum_type      subscription_id,
  void                             * event_info_ptr,
  void                             * user_data_ptr
)
{
  RemoteSocketCMDInfoType                    * cmdInfoPtr;
  ps_sys_remote_socket_service_avail_type    * serviceAvailptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (PS_SYS_EVENT_REMOTE_SOCKET_SERVICE_AVAIL != event_name || NULL == event_info_ptr)
  {
    LOG_MSG_ERROR_1("Manager::RemoteSocketServiceStatusCB : "
                    "Invalid event name recieved 0x%x", event_info_ptr);
    return;
  }
   
  serviceAvailptr = 
    (ps_sys_remote_socket_service_avail_type *)event_info_ptr;
    
  /*-----------------------------------------------------------------------
    Allocate memory for event info
  -----------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(cmdInfoPtr, sizeof(RemoteSocketCMDInfoType),
                            RemoteSocketCMDInfoType* );
  if (NULL == cmdInfoPtr)
  {
    LOG_MSG_ERROR_0("RemoteSocketServiceStatusCB: Unable to allocate memory");
    return;
  }
  /*-----------------------------------------------------------------------
    Set command ID
  -----------------------------------------------------------------------*/
  if (TRUE == serviceAvailptr->remote_socket_service_avail)
  {
    cmdInfoPtr->cmdId = REMOTE_SOCKET_SERVICE_AVAIL_CMD;
    /*-----------------------------------------------------------------------
      Copy command info payload
    -----------------------------------------------------------------------*/
    cmdInfoPtr->cmdInfo.avilInfo.user_data_ptr = user_data_ptr;
    
    cmdInfoPtr->cmdInfo.avilInfo.info.remote_socket_service_avail = 
      serviceAvailptr->remote_socket_service_avail;
  
    cmdInfoPtr->cmdInfo.avilInfo.info.remote_service_version =
      serviceAvailptr->remote_service_version;
  }
  else
  {
    cmdInfoPtr->cmdId = REMOTE_SOCKET_SERVICE_UNAVAIL_CMD;
    /*-----------------------------------------------------------------------
      Copy command info payload
    -----------------------------------------------------------------------*/
    cmdInfoPtr->cmdInfo.unAvilInfo.user_data_ptr = user_data_ptr;
    
    cmdInfoPtr->cmdInfo.unAvilInfo.info.remote_socket_service_avail = 
      serviceAvailptr->remote_socket_service_avail;
  
    cmdInfoPtr->cmdInfo.unAvilInfo.info.remote_service_version =
      serviceAvailptr->remote_service_version;
  }

  LOG_MSG_INFO1_4("Manager::RemoteSocketServiceStatusCB : "
                  "instance 0x%x remote binding allowed %d curr state %d serviceAvailptr 0x%x",
                  user_data_ptr, 
                  serviceAvailptr->remote_socket_service_avail,
                  serviceAvailptr->remote_service_version,
                  serviceAvailptr);
  
  ps_send_cmd (PS_REMOTE_SOCKET_CMD, (void*)cmdInfoPtr);

}/* RemoteSocketServiceStatusCB */

void Manager::RemoteSocketServiceAvailHandler
(
  ps_sys_remote_socket_service_avail_type          * serviceAvailptr
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/    

  if (NULL == serviceAvailptr) 
  {
    return;
  }

  LOG_MSG_INFO1_2("Manager::RemoteSocketServiceAvailHandler : "
                  "remote binding allowed %d curr state %d",
                  serviceAvailptr->remote_socket_service_avail,
                  serviceAvailptr->remote_service_version);
 
  critSectPtr->Enter();
  /*-----------------------------------------------------------------------
    Update remote service status
  -----------------------------------------------------------------------*/ 
  if (serviceAvailptr->remote_socket_service_avail == TRUE)
  {
    if ( PS_SYS_REMOTE_SOCKET_HANDLING_V1 == serviceAvailptr->remote_service_version )
    {
      remoteServiceStatus = REMOTE_SERVICE_AVAILABLE_V1;
    }
    else
    {
      remoteServiceStatus = REMOTE_SERVICE_AVAILABLE_V0;
    }
    /*-----------------------------------------------------------------------
      Go through all PS Sock sockets and find all the sockets whose 
      remote socket handle is NULL and have valid pool ID
    -----------------------------------------------------------------------*/    
     ReserveAllocatedSocket();
    /*-----------------------------------------------------------------------
      Go through all ephemeral caches and reserve sockets for 
      every cache based on property 
    -----------------------------------------------------------------------*/   
    ReserveAllSocketCaches();
   
  }
  else
  {
    remoteServiceStatus = REMOTE_SERVICE_UNAVAILABLE;
  }
 
  critSectPtr->Leave();
} /* Manager::RemoteSocketServiceAvailHandler */

void Manager::RemoteSocketServiceUnavailHandler
(
  ps_sys_remote_socket_service_avail_type      * serviceAvailptr
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/    
  if (NULL == serviceAvailptr) 
  {
    return;
  }
  critSectPtr->Enter();
  /*-----------------------------------------------------------------------
  1  Reset remote service status
  2. Release all caches and set status to un-available
      2.1  Move partially assigned sockets ( Adjacent or symmetric socket) to 
           local partially allocated sockets. 
  3  Invalidate remote socket handle in platform socket.
     Set 0xFF into remote socket handle in platform sockets
  -----------------------------------------------------------------------*/
  remoteServiceStatus = REMOTE_SERVICE_UNAVAILABLE;

  /*-----------------------------------------------------------------------
    Step 2
  -----------------------------------------------------------------------*/   
  UnAvailAllRemoteSocketCaches();
  /*-----------------------------------------------------------------------
    Step 3
  -----------------------------------------------------------------------*/ 
  InvalidateAssignedRemoteSocketHandles();

  critSectPtr->Leave();
}/* RemoteSocketServiceUnavailHandler */

void Manager::UnAvailAllRemoteSocketCaches()
{
  EphemeralPortManager              * EphPortMgrInst ;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  
  if (NULL == EphPortMgrInstList)
  {
    return;
  }

  critSectPtr->Enter();

  /*-----------------------------------------------------------------------
    Go through all ephemeral caches and release sockets for 
    every cache 
  -----------------------------------------------------------------------*/  
  for (EphPortMgrInstList->First(); 
       !EphPortMgrInstList->IsDone(); 
       EphPortMgrInstList->Next())
  {

    EphPortMgrInst  = 
      static_cast <EphemeralPortManager *> ( EphPortMgrInstList->CurrentItem());
    
    if (0 == EphPortMgrInst )
    {
      continue;
    }
    /*-------------------------------------------------------------------------
      1.  Set cache state to Unavailable
      2.  Release all un-used/un-assigned socket 
      3.  Transfer partially assigned sockets into local partially assigned list
      4.  Send request to AP
    -------------------------------------------------------------------------*/
    EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_UNAVAILABLE);

    EphPortMgrInst->ReleaseUnusedSocketCaches();

    EphPortMgrInst->TransferPartialAssignSockets( );
  }

  critSectPtr->Leave();  
  return ;

}/* UnAvailAllRemoteSocketCaches */

void Manager::ReserveAllSocketCaches()
{
  EphemeralPortManager              * EphPortMgrInst ;
  ps_sys_remote_socket_request_type * remoteSocketsList = NULL;
  int16                               ps_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 

  if (NULL == EphPortMgrInstList)
  {
    return;
  }
  
  PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(remoteSocketsList, 
     sizeof(ps_sys_remote_socket_request_type),
     ps_sys_remote_socket_request_type*);

   if(NULL == remoteSocketsList)
   {
     LOG_MSG_ERROR_0("Manager::ReserveAllSocketCaches :"
                     "Insufficient memory ");
     DS_UTILS_ASSERT(0);
     
   }
  critSectPtr->Enter();
  /*-----------------------------------------------------------------------
    Go through all ephemeral caches and reserve sockets for 
    every cache based on property 
  -----------------------------------------------------------------------*/  
  for (EphPortMgrInstList->First(); 
       !EphPortMgrInstList->IsDone(); 
       EphPortMgrInstList->Next())
  {
    EphPortMgrInst  = 
      static_cast <EphemeralPortManager *> ( EphPortMgrInstList->CurrentItem());

    if (0 == EphPortMgrInst )
    {
      continue;
    }
    /*-----------------------------------------------------------------------
    Only Legacy ephemeral sockets can be requested to AP when remote 
    service version is 0.(Old AP)
    -----------------------------------------------------------------------*/  
    if(REMOTE_SOCKETS_UNAVAILABLE == EphPortMgrInst->GetCacheState())
    {
      if ( (REMOTE_SERVICE_AVAILABLE_V1 == remoteServiceStatus) ||
           ( REMOTE_SERVICE_AVAILABLE_V0 == remoteServiceStatus &&
             TRUE == EphPortMgrInst->IsLegacyEphemeralCache ()))
      {
        /*-------------------------------------------------------------------------
          1.  Initialize maximum socket request for every cache 
          2.  Retrieve locally allocated sockets from partial list 
          3.  Set cache state
          4. Send request to AP
        -------------------------------------------------------------------------*/
        memset( remoteSocketsList, 0x0, sizeof(ps_sys_remote_socket_request_type));
        
        EphPortMgrInst->InitilizeMaxSocketsRequest( remoteSocketsList);

        EphPortMgrInst->RetrieveLocalAllocatedSockets( remoteSocketsList );
        
        EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_REQUESTED);
        
        ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                            PS_SYS_EVENT_REMOTE_SOCKET_REQUEST,
                            PS_SYS_PRIMARY_SUBS,                 
                            remoteSocketsList,
                            &ps_errno);
      }
      else if(REMOTE_SERVICE_AVAILABLE_V0 == remoteServiceStatus)
      {
        EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_NOT_SUPPORTED);
      }
    }
  }

  if(NULL != remoteSocketsList)
  {
    PS_SYSTEM_HEAP_MEM_FREE (remoteSocketsList);
  }
  critSectPtr->Leave();  
  return ;

}/*Manager::ReserveAllSocketCaches*/

EphemeralPortManager *  Manager::GetEphemeralMgrInstFromHandle
(
  uint32                         request_handle
)
{
  EphemeralPortManager   * EphPortMgrInst  = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  for (EphPortMgrInstList->First(); 
       !EphPortMgrInstList->IsDone(); 
       EphPortMgrInstList->Next())
  {
    EphPortMgrInst  = 
      static_cast <EphemeralPortManager *> ( EphPortMgrInstList->CurrentItem());

    if (0 == EphPortMgrInst )
    {
      continue;
    }

    if( request_handle == EphPortMgrInst->GetSocketsEphemeralHandle())
    {
      return EphPortMgrInst;
    }
  }

  return NULL;
}/* Manager::GetEphemeralMgrInstFromHandle */


EphemeralPortManager *  Manager::GetLegacyEphemeralMgrInst
(
)
{
  EphemeralPortManager   * EphPortMgrInst = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  for (EphPortMgrInstList->First(); 
       !EphPortMgrInstList->IsDone(); 
       EphPortMgrInstList->Next())
  {
    EphPortMgrInst = 
      static_cast <EphemeralPortManager *> ( EphPortMgrInstList->CurrentItem());

    if (0 == EphPortMgrInst)
    {
      continue;
    }

    if( EphPortMgrInst->IsLegacyEphemeralCache())
    {
      return EphPortMgrInst;
    }
  }
  return NULL;

}/* Manager::GetLegacyEphemeralMgrInst */

int16 Manager::RemoteSocketAllocIoctl
(
  ps_sys_ioctl_enum_type          ioctl_name,
  void                          * arg_val_ptr,
  ps_sys_subscription_enum_type   subscription_id,
  int16                         * ps_errno
)
{
  RemoteSocketCMDInfoType                     * cmdInfoPtr;
  ps_sys_remote_socket_allocated_list_type    * sockAllocatedPtr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (PS_SYS_IOCTL_REMOTE_SOCKET_ALLOCATED != ioctl_name || NULL == arg_val_ptr)
  {
    LOG_MSG_ERROR_2("Manager::RemoteSocketAllocIoctl :"
                    "Invalid ioctl recieved ioctl %d ptr 0x%x", ioctl_name, arg_val_ptr);
    return -1;
  }

  sockAllocatedPtr = 
    (ps_sys_remote_socket_allocated_list_type *)arg_val_ptr;

  LOG_MSG_INFO1_3("Manager::RemoteSocketAllocIoctl() :  ioctl_name %d"
                  " handle %d num sockets %d ", ioctl_name, 
                  sockAllocatedPtr->request_handle, sockAllocatedPtr->num_sockets);

  /*-----------------------------------------------------------------------
Allocate memory for event info
---------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(cmdInfoPtr, sizeof(RemoteSocketCMDInfoType),
                           RemoteSocketCMDInfoType* );
  
  if (NULL == cmdInfoPtr)
  {
    LOG_MSG_ERROR_0("RemoteSocketAllocIoctl: Unable to allocate memory");
    return -1;
  }
  /*-----------------------------------------------------------------------
    Set command ID
  -----------------------------------------------------------------------*/
  cmdInfoPtr->cmdId = REMOTE_SOCKET_SOCKETS_ALLOC_CMD;
  /*-----------------------------------------------------------------------
    Copy command info payload
  -----------------------------------------------------------------------*/
  cmdInfoPtr->cmdInfo.allocatedSocketInfo.sockets.request_handle 
    = sockAllocatedPtr->request_handle;

  cmdInfoPtr->cmdInfo.allocatedSocketInfo.sockets.num_sockets = 
    sockAllocatedPtr->num_sockets;

  memscpy(&cmdInfoPtr->cmdInfo.allocatedSocketInfo.sockets.socket_list[0],
    (sizeof(ps_sys_remote_socket_allocated_type) * PS_SYS_MAX_REMOTE_SOCKETS ),
    &sockAllocatedPtr->socket_list[0],
    (sizeof(ps_sys_remote_socket_allocated_type) * sockAllocatedPtr->num_sockets ));
    
  ps_send_cmd (PS_REMOTE_SOCKET_CMD, (void*)cmdInfoPtr);

  return 0;

}/*RemoteSocketAllocIoctl*/

int16 Manager::RemoteSocketAllocIoctlHandler
(
  ps_sys_remote_socket_allocated_list_type      * sockAllocatedPtr
)
{
  ps_sys_remote_socket_allocated_type           * sockInfoPtr;
  int                                             cnt_sock;
  EphemeralPortManager                          * EphPortMgrInst = NULL;
  ps_sys_remote_socket_release_list_type          sockReleaseInfo;
  int16                                           psErrno;
  boolean                                         isAnySocketAlloc = TRUE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/                  
  if (NULL == sockAllocatedPtr)
  {
    return -1;
  }
  LOG_MSG_INFO1_3("Manager::RemoteSocketAllocIoctlHandler() :  handle %d "
                  "num sockets %d Serv Status %d", sockAllocatedPtr->request_handle,
                  sockAllocatedPtr->num_sockets, remoteServiceStatus);
    
  memset(&sockReleaseInfo, 0, sizeof(sockReleaseInfo));
  
  critSectPtr->Enter();

  if (REMOTE_SERVICE_AVAILABLE_V1 == remoteServiceStatus)
  {
    if( 0 !=  sockAllocatedPtr->request_handle)
    {
      EphPortMgrInst = GetEphemeralMgrInstFromHandle( sockAllocatedPtr->request_handle );
    }
  }
  else
  {
     EphPortMgrInst = GetLegacyEphemeralMgrInst(); 
  }
  /*-----------------------------------------------------------------------
      Validate  recevied sockets 
    -----------------------------------------------------------------------*/ 
  if(NULL != EphPortMgrInst)
  {
    EphPortMgrInst->ValidateReceivedRemoteSock( sockAllocatedPtr, &sockReleaseInfo);

    /*-----------------------------------------------------------------------
        Special case: Check if AP is unable to Allocate any socket in first request.
        Notify Application about failure
      -----------------------------------------------------------------------*/ 
    if( sockReleaseInfo.num_sockets == sockAllocatedPtr->num_sockets &&
        REMOTE_SOCKETS_REQUESTED == EphPortMgrInst->GetCacheState())
    {
      isAnySocketAlloc = FALSE;
    }
  }
    /*-----------------------------------------------------------------------
      1. Go through all sockets reserved by AP/TE
      2. If socket was requested with port 0 (is_ephemeral_port was set to
         TRUE) then add the socket to cache
      3. If socket was requested with specific port(is_ephemeral_port was 
         set to FALSE) then find the socket in list of all PS Sock Sockets
         and add remote socket handle to that socket
    -----------------------------------------------------------------------*/     
  for (cnt_sock = 0; cnt_sock < sockAllocatedPtr->num_sockets; cnt_sock++)
  {
    sockInfoPtr = &(sockAllocatedPtr->socket_list[cnt_sock]);
    
    if (NULL != sockInfoPtr)
    {
      if (REMOTE_SERVICE_AVAILABLE_V1 == remoteServiceStatus)
      {
        if ( NULL != EphPortMgrInst)
        {
          EphPortMgrInst->UpdateSockInfoToCache(sockInfoPtr); 
          EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_AVAILABLE);
        }
        else
        {
          AddHandleToSocket(sockInfoPtr);
        }
      }
      else
      {
        if (TRUE == sockInfoPtr->is_ephemeral_port && NULL != EphPortMgrInst)
        {
          EphPortMgrInst->UpdateSockInfoToCache(sockInfoPtr); 
          EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_AVAILABLE);
        }
        else
        {
          AddHandleToSocket(sockInfoPtr);        
        }
      }
    }
  } 

  critSectPtr->Leave();

  if ( NULL != EphPortMgrInst)
  {
    NotifySocketsAllocated(EphPortMgrInst, isAnySocketAlloc);
  }
   /*-----------------------------------------------------------------
    1. Release invalid sockets to AP
    2. Re-request to AP for new sockets
  ------------------------------------------------------------------*/  
  if ( 0 != sockReleaseInfo.num_sockets)
  {
    ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                        PS_SYS_EVENT_REMOTE_SOCKET_RELEASE,
                        PS_SYS_PRIMARY_SUBS,
                        &sockReleaseInfo,
                        &psErrno);
  }
  
  return 0;
} /* Manager::RemoteSocketAllocIoctlHandler */


/*===========================================================================

                         PRIVATE MEMBER FUNCTIONS

===========================================================================*/
Socket * Manager::GetPlatformSocketPtr
(
  uint32   ps_sock_platform_handle
)
{
  Socket *  platformSockPtr = 0;
  int32     index;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if (PS_HANDLE_MGR_INVALID_HANDLE == ps_sock_platform_handle)
  {
    LOG_MSG_ERROR_1("Manager::GetPlatformSocketPtr(): "
                    "Invalid handle %d", ps_sock_platform_handle);
    return NULL;
  }  
  
  /*-------------------------------------------------------------------------
    Generate a handle for platform socket. This handle will be the platform
    socket descriptor
  -------------------------------------------------------------------------*/
  index = 
    ps_handle_mgr_get_index( PS_HANDLE_MGR_CLIENT_SOCKET_PLATFORM,
                              ps_sock_platform_handle);
  
  platformSockPtr = 
    (Socket *)ps_mem_index_to_buf(index, PS_MEM_PLATFORM_SOCKET_TYPE);
  if (NULL == platformSockPtr)
  {
    LOG_MSG_ERROR_2("Manager::GetPlatformSocketPtr(): "
                    "Invalid index %d sock ptr 0x%p", 
                    index, platformSockPtr);
  }
  
  return platformSockPtr;
} /* Manager::GetPlatformSocketPtr */

void Manager::InvalidateAssignedRemoteSocketHandles
(
  void
)
{
  SocketFactory                            * platformSockFactoryPtr;
  Socket                                   * platformSockPtr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  critSectPtr->Enter();

  platformSockFactoryPtr = SocketFactory::CreateInstance();
  DS_UTILS_ASSERT( 0 != platformSockFactoryPtr);
  /*-------------------------------------------------------------------------
    1. Iterate through the list of platform sockets
    2. Find the platform socket information with remote socket handle is not UINT32_MAX
       and has ephemeral cache handle set to valid handle
    3. Invalidate remote socket and set handle to UINT32_MAX
  -------------------------------------------------------------------------*/ 
  for (platformSockFactoryPtr->First(); 
       !platformSockFactoryPtr->IsDone(); 
       platformSockFactoryPtr->Next())
  {
    platformSockPtr = 
      static_cast <Socket *> ( platformSockFactoryPtr->CurrentItem());
    if (0 == platformSockPtr)
    {
      LOG_MSG_ERROR_0("Manager::ReserveAllocatedSocket: "
                      "NULL sock");
      continue;
    }
    
    if (TRUE == platformSockPtr->GetStrongRef())
    {
      /*Step 2*/
      if ((UINT32_MAX !=  platformSockPtr->GetEphemeralCacheHandle()) &&
            (UINT32_MAX !=  platformSockPtr->GetRemoteSockHandle()))
      {
        /*Step 3*/
        platformSockPtr->SetRemoteSockHandle(UINT32_MAX);
      }
      (void) platformSockPtr->Release();
    }
  }
  DS_UTILS_RELEASEIF(platformSockFactoryPtr);
  critSectPtr->Leave();

}/*InvalidateAssignedRemoteSocketHandles*/

int Manager::ReserveAllocatedSocket
(
  void
)
{
  SocketFactory                            * platformSockFactoryPtr;
  Socket                                   * platformSockPtr;
  ps_sys_remote_socket_request_type          remoteSocketsList;
  int16                                      ps_errno;
  int                                        numSockets = 0;
  uint16                                     basePort = 0;
  uint16                                     offset   = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/      

  critSectPtr->Enter();
     
  memset(&remoteSocketsList, 0, 
         sizeof(ps_sys_remote_socket_request_type));
         
  platformSockFactoryPtr = SocketFactory::CreateInstance();
  DS_UTILS_ASSERT( 0 != platformSockFactoryPtr);

  basePort = LocalEphemeralPortGen::GetEphemeralBasePort();
  offset   = LocalEphemeralPortGen::GetEphemeralPortOffset();
  /*-------------------------------------------------------------------------
    1. Iterate through the list of platform sockets
    2. Find the platform socket information with remote socket handle as UINT32_MAX
       and has ephemeral cache handle set to valid handle
    3. Send request to AP to block all those specific ports
  -------------------------------------------------------------------------*/  
  /*---------------------------------------------------------------------
    Step 1
  ---------------------------------------------------------------------*/  
  for (platformSockFactoryPtr->First(); 
       !platformSockFactoryPtr->IsDone(); 
       platformSockFactoryPtr->Next())
  {
    platformSockPtr = 
      static_cast <Socket *> ( platformSockFactoryPtr->CurrentItem());
    if (0 == platformSockPtr)
    {
      LOG_MSG_ERROR_0("Manager::ReserveAllocatedSocket: "
                      "NULL sock");
      continue;
    }
    if (TRUE == platformSockPtr->GetStrongRef())
    {
      /*-----------------------------------------------------------------
        Step 2
      -----------------------------------------------------------------*/     
      if ((UINT32_MAX !=  platformSockPtr->GetEphemeralCacheHandle()) &&
          (UINT32_MAX ==  platformSockPtr->GetRemoteSockHandle()))
      {

        if( (REMOTE_SERVICE_AVAILABLE_V1 == remoteServiceStatus) ||
            (ps_ntohs(platformSockPtr->GetPort()) > basePort && 
             ps_ntohs(platformSockPtr->GetPort()) <= basePort + offset))

        {
          remoteSocketsList.sockets_list.socket_list[numSockets].port_no =  
           platformSockPtr->GetPort();
          remoteSocketsList.sockets_list.socket_list[numSockets].ip_family = 
           RemoteSocketUtils::ConvertToPSFamily(platformSockPtr->GetFamily());
          remoteSocketsList.sockets_list.socket_list[numSockets].xport_proto =  
            RemoteSocketUtils::ConvertToPSProtocol(platformSockPtr->GetProtocol());
          numSockets++;
          if (PS_SYS_MAX_REMOTE_SOCKETS <= numSockets)
          {
            break;
          }
        }
      }
      
      (void) platformSockPtr->Release();
    }
  }  
  remoteSocketsList.sockets_list.num_sockets = numSockets;
  
  DS_UTILS_RELEASEIF(platformSockFactoryPtr);
  
  /*-----------------------------------------------------------------
    Step 3
  -----------------------------------------------------------------*/      
  if (numSockets > 0)
  {
    ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                        PS_SYS_EVENT_REMOTE_SOCKET_REQUEST,
                        PS_SYS_PRIMARY_SUBS,
                        &remoteSocketsList,
                        &ps_errno);
  }                   
 
  critSectPtr->Leave();

  return numSockets;
} /* Manager::ReserveAllocatedSocket */

void Manager::AddHandleToSocket
(
  ps_sys_remote_socket_allocated_type      * remoteSockInfoPtr
)
{
  SocketFactory                            * platformSockFactoryPtr;
  Socket                                   * platformSockPtr;
  boolean                                    socketConsumed = FALSE;
  int16                                      retVal;
  uint16                                     port_val;
  int32                                      udpEpncapsVal;
  int32                                      udpEpncapsLen;
  SockInfo                                   sockInfo;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  critSectPtr->Enter();
  
  platformSockFactoryPtr = SocketFactory::CreateInstance();
  DS_UTILS_ASSERT( 0 != platformSockFactoryPtr);  
       
  /*-------------------------------------------------------------------------
    1. Iterate through the list of platform sockets
    2. Match platform socket information with remote socket information
    3. If they match then update platform socket's remote socket handle
       to handle sent by AP. If platform socket has UDP encaps option 
       set then send indication to AP to set UDP encaps option for that
       remote socket handle
    4. If remote socket information doesnt match any PS Sock Socket then
       free the remote socket
  -------------------------------------------------------------------------*/
  
  /*---------------------------------------------------------------------
    Step 1
  ---------------------------------------------------------------------*/   
  for (platformSockFactoryPtr->First(); 
       !platformSockFactoryPtr->IsDone(); 
       platformSockFactoryPtr->Next())
  {
    platformSockPtr = 
      static_cast <Socket *> ( platformSockFactoryPtr->CurrentItem());
    if (0 == platformSockPtr)
    {
      LOG_MSG_ERROR_0("Manager::AddHandleToSocket: "
                      "NULL sock");
      continue;
    }
        
    if (TRUE == platformSockPtr->GetStrongRef())
    {
      /*-----------------------------------------------------------------
        Step 2
      -----------------------------------------------------------------*/        
      port_val = platformSockPtr->GetPort();
      if ((port_val ==
          remoteSockInfoPtr->socket_info.port_no) && 
          (platformSockPtr->GetFamily() ==
          RemoteSocketUtils::ConvertToDSSockFamily
          (remoteSockInfoPtr->socket_info.ip_family)) &&
          (platformSockPtr->GetProtocol() ==
          RemoteSocketUtils::ConvertToDSSockProtocol
          (remoteSockInfoPtr->socket_info.xport_proto))  &&
          UINT32_MAX ==  platformSockPtr->GetRemoteSockHandle() &&
          TRUE == platformSockPtr->IsSocketAlive())
      {
        /*-----------------------------------------------------------------
          Only in Successful allocation, add Handle to the socket
        -----------------------------------------------------------------*/
        if(PS_SYS_REMOTE_SOCKET_ALLOC_SUCCESS == remoteSockInfoPtr->status)
        {
          platformSockPtr->SetRemoteSockHandle
          (
            remoteSockInfoPtr->remote_socket_handle
          );       
          if (AEE_SUCCESS == 
              platformSockPtr->GetOpt(ds::Sock::OptLevel::QDS_LEVEL_UDP, 
                                      ds::Sock::OptName::QDS_UDP_ENCAPS, 
                                      &udpEpncapsVal, 
                                      &udpEpncapsLen))                                    
          {
            if (1 == udpEpncapsVal)
            {
              SendUDPEncapsSockOpt(remoteSockInfoPtr->remote_socket_handle, TRUE);
            }            
          }                         
        }
        socketConsumed = TRUE;
      }
      
      (void) platformSockPtr->Release();

      if (TRUE == socketConsumed)
      {
        break;
      }
    }
  } 

  /*-----------------------------------------------------------------
    Step 4
  -----------------------------------------------------------------*/    
  if (FALSE == socketConsumed) 
  {
    sockInfo.SetHandle(remoteSockInfoPtr->remote_socket_handle);
    
    retVal = ReleaseRemoteSocket
             (
               &sockInfo,
               UINT32_MAX
             );
    if (retVal != 0)
    {
      LOG_MSG_ERROR_1("Manager::AddHandleToSocket : "
                      "Release socket failed err %d", retVal);       
    }
  } 
  DS_UTILS_RELEASEIF(platformSockFactoryPtr); 
  critSectPtr->Leave();
} /* Manager::AddHandleToSocket */

/*===========================================================================

                         PUBLIC MEMBER FUNCTIONS

===========================================================================*/
Manager * Manager::GetInstance(void)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (0 == instance) 
  {
    DS_UTILS_ASSERT(0);
  }
  return instance;
} /* Manager::Instance() */

RemoteServiceStatus Manager::GetState
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return remoteServiceStatus;
} /* Manager::GetState */

boolean Manager::CheckSocketsPref
(
  PortPreference          * socketsPref,
  uint32                   * handle
)
{
  boolean                   isFound = FALSE;
  EphemeralPortManager    * EphPortMgrInst;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if ( NULL == EphPortMgrInstList)
  {
    return isFound;
  }

  critSectPtr->Enter();
  /*-------------------------------------------------------------------------
    1. Iterate through the list of platform sockets
    2. Match preferences with existing caches 
    3. If preferences matched then return Handler
  -------------------------------------------------------------------------*/  
  for (EphPortMgrInstList->First(); 
       !EphPortMgrInstList->IsDone(); 
       EphPortMgrInstList->Next())
  {
    EphPortMgrInst = 
      static_cast <EphemeralPortManager *> ( EphPortMgrInstList->CurrentItem());

    if (0 == EphPortMgrInst)
    {
      continue;
    }
    if(TRUE == EphPortMgrInst->MatchSocketsPreference(socketsPref))
    {
        isFound = TRUE;
      * handle  =  EphPortMgrInst->GetSocketsEphemeralHandle();
        EphPortMgrInst->AddRefCount();
      break;
    }
  }
  critSectPtr->Leave();
  LOG_MSG_INFO1_2("Manager::CheckSocketsPref is found %d handle %u", isFound, *handle);
  
  return isFound;
}/* Manager::CheckSocketsPref*/

int32 Manager::RequestSocketsPref
(
  PortPreference          * socketPref,
  PortReqCallBack           callBackFunc,
  void                    * userData
)
{
  uint32                              poolHandle = -1;
  EphemeralPortManager              * EphPortMgrInst = NULL;
  ps_sys_remote_socket_request_type * remoteSocketsList = NULL;
  RemoteServiceStatus                 status;
  boolean                             isLegacyEphemeralReq = FALSE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if (NULL == socketPref || NULL == callBackFunc || NULL == EphPortMgrInstList)
    
  {
    return -1;
  }
  
  critSectPtr->Enter();
  /*-------------------------------------------------------------------------
    1.  Return Previously created cache ID when a cache is already exist with same preference 
    2.  Create a sockets cache and initilize with preference.
    3.  Create a remote sockets request a paylod for AP/TE based on preference
    4.  Send request to AP to reserve sockets 
    5. Add Newly created cache into the EphPortMgrInstList
  -------------------------------------------------------------------------*/ 
  do
  {
    if (TRUE == CheckSocketsPref( socketPref, &poolHandle) )
    {
      break;
    }
    
    if (MAX_EPHEMERAL_CACHES == EphPortMgrInstList->Count())
    {
      break;
    }
    
    if (NULL == ( EphPortMgrInst = new EphemeralPortManager( socketPref,
          callBackFunc, userData)))
    {
      break;
    }

    PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(remoteSocketsList, 
     sizeof(ps_sys_remote_socket_request_type),
     ps_sys_remote_socket_request_type*);

    if (FALSE == EphPortMgrInst->InitilizeMaxSocketsRequest( remoteSocketsList))
    {
      delete EphPortMgrInst;
      EphPortMgrInst = NULL;
      break;
    }

    isLegacyEphemeralReq = EphPortMgrInst->IsLegacyEphemeralCache ();
        
    status = RequestRemoteSocketList(remoteSocketsList, isLegacyEphemeralReq);

    switch ( status )
    {
      case REMOTE_SERVICE_AVAILABLE_V1:
      {
        poolHandle = 0;
        EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_REQUESTED);
        break;
      }
      case REMOTE_SERVICE_AVAILABLE_V0:
      {
        if (TRUE == isLegacyEphemeralReq)
        {
          poolHandle = 0;
          EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_REQUESTED);
        }
        else
        {
          poolHandle = EphPortMgrInst->GetSocketsEphemeralHandle();
          EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_NOT_SUPPORTED);
          EphPortMgrInst->SetClientCallBackPending(FALSE);
        }
        break;
      }
      case REMOTE_SERVICE_UNAVAILABLE:
      {
        poolHandle = EphPortMgrInst->GetSocketsEphemeralHandle();
        EphPortMgrInst->SetCacheState(REMOTE_SOCKETS_UNAVAILABLE);
        EphPortMgrInst->SetClientCallBackPending(FALSE);
        break;
      }
      default :
      {
        poolHandle = -1;
        delete EphPortMgrInst;
        EphPortMgrInst = NULL;
      }
    }
    
    if ( -1 != poolHandle && NULL != EphPortMgrInst)
    {
      EphPortMgrInstList->PushBack(EphPortMgrInst);
    }
    
  }while(0);

  if (NULL != remoteSocketsList)
  {
    PS_SYSTEM_HEAP_MEM_FREE (remoteSocketsList);
  }
  
  critSectPtr->Leave();

  LOG_MSG_INFO1_6("Manager::RequestPortPref  property %d port count %d "
                  " base port range %d max port range %d result %u instance %x",
                  socketPref->port_property, 
                  socketPref->pref_count, 
                  socketPref->base_port_range, 
                  socketPref->max_port_range,
                  poolHandle,
                  EphPortMgrInst);   

  return (int32)poolHandle;

}/*RequestSocketsPref*/

void Manager::InitSocketsPref
(
  PortPreference          * socket_pref
) 
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  socket_pref->port_property   = DSS_PORT_PROP_MIN;
  socket_pref->pref_count      = 0;
  socket_pref->base_port_range = LocalEphemeralPortGen::GetEphemeralBasePort();
  socket_pref->max_port_range  = socket_pref->base_port_range +
                                   LocalEphemeralPortGen::GetEphemeralPortOffset();
}/*Manager::InitSocketsPref*/

RemoteServiceStatus Manager::RequestRemoteSocketList
(
  ps_sys_remote_socket_request_type * remoteSocketsList,
  boolean                             isLegacyEphemeralReq
)
{
  ps_sys_remote_socket_service_avail_type  sock_remote_service_avail;
  int16                                    ps_errno;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  critSectPtr->Enter();

  memset(&sock_remote_service_avail, 0, sizeof(sock_remote_service_avail));
  
  do
  {
    ps_sys_conf_get_ex(PS_SYS_TECH_ALL, 
                       PS_SYS_CONF_REMOTE_SOCKET_SERVICE_AVAIL,
                       PS_SYS_PRIMARY_SUBS, 
                       &sock_remote_service_avail,
                       &ps_errno);  
    
    if (FALSE == sock_remote_service_avail.remote_socket_service_avail)
    {
      remoteServiceStatus = REMOTE_SERVICE_UNAVAILABLE;
        
      break;
    }
    else
    {
      switch ( sock_remote_service_avail.remote_service_version )
      {
       
        case PS_SYS_REMOTE_SOCKET_HANDLING_V1:
        {
          ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                              PS_SYS_EVENT_REMOTE_SOCKET_REQUEST,
                              PS_SYS_PRIMARY_SUBS,
                              remoteSocketsList,
                              &ps_errno);
     
          remoteServiceStatus = REMOTE_SERVICE_AVAILABLE_V1;
          break;
        }
        case PS_SYS_REMOTE_SOCKET_HANDLING_V0:
        {
          if(TRUE == isLegacyEphemeralReq)
          {
            ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                                PS_SYS_EVENT_REMOTE_SOCKET_REQUEST,
                                PS_SYS_PRIMARY_SUBS,
                                remoteSocketsList,
                                &ps_errno);
          }
          remoteServiceStatus = REMOTE_SERVICE_AVAILABLE_V0;
          break;
        }
        default:
        {
          remoteServiceStatus = REMOTE_SERVICE_UNAVAILABLE;
          break;
        }
      }
    }
    
  }while(0);
  
  critSectPtr->Leave();

  LOG_MSG_INFO1_5("Manager::remoteServiceStatus remote service avail %d "
                  " version %d,  result %d,  Legacy ephemeral %d Handle %d",
                  sock_remote_service_avail.remote_socket_service_avail, 
                  sock_remote_service_avail.remote_service_version , 
                  remoteServiceStatus, 
                  isLegacyEphemeralReq,
                  remoteSocketsList->request_handle);

  return remoteServiceStatus;
}/* Manager::RequestRemoteSocketList */

boolean Manager::RetrieveAndRequestSocket
(
  uint32                     cacheHandle,
  SockInfo                  *sockInfo,
  EphemeralSocketFlag        bindFlag
)
{
  boolean                                  result = FALSE;
  ps_sys_remote_socket_request_type      * remoteSocketsList = NULL;
  ps_sys_remote_socket_release_list_type * sockReleaseList = NULL;
  boolean                                  isLegacyEphemeralReq = FALSE;
  EphemeralPortManager                   * EphPortMgrInst = NULL;
  int16                                    ps_errno;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  
  /*-------------------------------------------------------------------------
    1.  Retrieve cache pointer from cache handle 
    2.  Retrieve Socket from ephemeral cache .
    3.  Send new remote socket request to AP when request list length non zero
    4.  Release Socket handle to AP when release list length non zero
  -------------------------------------------------------------------------*/
  critSectPtr->Enter();
  
  do
  {
    if ( NULL == sockInfo || 0 == cacheHandle)
    {
      break;
    }
    EphPortMgrInst = GetEphemeralMgrInstFromHandle( cacheHandle );
    if ( NULL == EphPortMgrInst)
    {
      break ;
    }
    PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(remoteSocketsList, 
     sizeof(ps_sys_remote_socket_request_type),
     ps_sys_remote_socket_request_type*);
    
    PS_SYSTEM_HEAP_MEM_ALLOC_ASSERT(sockReleaseList, 
     sizeof(ps_sys_remote_socket_release_list_type),
     ps_sys_remote_socket_release_list_type*);

    if ( NULL == remoteSocketsList || NULL == sockReleaseList )
    {
      break ;
    }
    memset(sockReleaseList, 0, sizeof(ps_sys_remote_socket_release_list_type));
    memset(remoteSocketsList, 0, sizeof(ps_sys_remote_socket_request_type));
    result = EphPortMgrInst->RetrieveAndRequestSocket( bindFlag, sockInfo, 
                              remoteSocketsList, sockReleaseList);
    
    if ( 0 != remoteSocketsList->sockets_list.num_sockets)
    {
      isLegacyEphemeralReq = EphPortMgrInst->IsLegacyEphemeralCache ();
      RequestRemoteSocketList( remoteSocketsList, isLegacyEphemeralReq);
    }
    
    if ( 0 != sockReleaseList->num_sockets)
    {
      ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                          PS_SYS_EVENT_REMOTE_SOCKET_RELEASE,
                          PS_SYS_PRIMARY_SUBS,
                          sockReleaseList,
                          &ps_errno);
    }
  }while(0);

  if (NULL != remoteSocketsList)
  {
    PS_SYSTEM_HEAP_MEM_FREE (remoteSocketsList);
  }
  if (NULL != sockReleaseList)
  {
    PS_SYSTEM_HEAP_MEM_FREE (sockReleaseList);
  }

  critSectPtr->Leave();
  return result;
}/* Manager::RetrieveAndRequestSocket */

int16 Manager::ReleaseRemoteSocket
(
  SockInfo               *sockInfo,
  uint32                  cacheHandle
)
{
  int16                                       result = 0;
  int16                                       ps_errno;
  ps_sys_remote_socket_release_list_type      sockReleaseInfo;
  EphemeralPortManager                      * EphPortMgrInst = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  if( NULL == sockInfo)
  {
    return -1;
  }
  
  LOG_MSG_INFO1_2("Manager::ReleaseRemoteSocket() : Sock handle %d  Cache handle %d ", 
    sockInfo->GetHandle(), cacheHandle);

  memset(&sockReleaseInfo, 0, 
           sizeof(ps_sys_remote_socket_release_list_type));
        
  critSectPtr->Enter();
  /*-------------------------------------------------------------------------
    1.  Retrieve cache pointer from cache handle 
    2.  Release socket from ephemeral cache .
    3.  Release Sockets to AP when release list length non zero
    Note: Cache handle can be MAX or 0 when socket is not associated  with any cache
            Cache pointer can be NULL when Cache is already released 
  -------------------------------------------------------------------------*/
  if( UINT32_MAX == cacheHandle)
  {
    if (UINT32_MAX != sockInfo->GetHandle())
    {
      sockReleaseInfo.num_sockets    = 1;
      sockReleaseInfo.socket_list[0] = sockInfo->GetHandle();
    } 
  }
  else
  {
    EphPortMgrInst = GetEphemeralMgrInstFromHandle( cacheHandle );
    if (NULL != EphPortMgrInst)
    {
      EphPortMgrInst->ReleaseRemoteSocket(sockInfo, &sockReleaseInfo);
    }
    else if (UINT32_MAX != sockInfo->GetHandle() )
    {
      sockReleaseInfo.num_sockets = 1;
      sockReleaseInfo.socket_list[0] = sockInfo->GetHandle();
    }
  }

  if ( 0 != sockReleaseInfo.num_sockets)
  {
    ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                        PS_SYS_EVENT_REMOTE_SOCKET_RELEASE,
                        PS_SYS_PRIMARY_SUBS,
                        &sockReleaseInfo,
                        &ps_errno);
  }
  
  critSectPtr->Leave();
  return result;
} /* Manager::ReleaseRemoteSocket */


int16  Manager::ReleaseSocketsPref
(
  int32                 cacheHandle,
  PortRelCallBack       callBackFunc,
  void                * userData
)
{
  int16                                     ps_errno;
  ps_sys_remote_socket_release_list_type    sockReleaseInfo;
  EphemeralPortManager                    * EphPortMgrInst = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  if ( 0 == cacheHandle || UINT32_MAX == cacheHandle || NULL == callBackFunc)
    
  {
    return -1;
  }
  
  LOG_MSG_INFO1_1("Manager::ReleaseSocketsPref() :  Cache handle %d ",  cacheHandle);
  /*-------------------------------------------------------------------------
    1.  Retrieve cache pointer from cache handle 
    2.  Decrement the ref count.
    3.  When Ref count become 0 Remove cache from the list
    4.  Release all sockets associated with this cache
    5.  Send Release indication to AP 
    6.  Delete the cache
    7   Notify to Application
  -------------------------------------------------------------------------*/
  memset(&sockReleaseInfo, 0, 
           sizeof(ps_sys_remote_socket_release_list_type));
  
  critSectPtr->Enter();

  EphPortMgrInst = GetEphemeralMgrInstFromHandle( cacheHandle );
  
  if ( NULL != EphPortMgrInst)
  {
   EphPortMgrInst->DecRefCount();

   if ( 0 == EphPortMgrInst->GetRefCount())
   {
      EphPortMgrInstList->RemoveItem(EphPortMgrInst);
      EphPortMgrInst->ReleaseSocketCaches(&sockReleaseInfo);
      EphPortMgrInst->DeInit();
    
      if ( 0 != sockReleaseInfo.num_sockets)
      {
        ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                            PS_SYS_EVENT_REMOTE_SOCKET_RELEASE,
                            PS_SYS_PRIMARY_SUBS,
                            &sockReleaseInfo,
                            &ps_errno);
      }
      delete EphPortMgrInst;
    }
  }
  critSectPtr->Leave();

  if ( NULL != EphPortMgrInst)
  {
    /*-------------------------------------------------------------------------
    Notify to Application :: In future we can get the release confrimation from Ap  
    -------------------------------------------------------------------------*/ 
    NotifySocketsReleased(callBackFunc, userData);
  }
  return 0;
}/*Manager::ReleaseSocketsPref*/

void  Manager::NotifySocketsReleased
(
  PortRelCallBack       callBackFunc,
  void                * userData
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  

  if ( NULL != callBackFunc)
  {
    callBackFunc( DSS_SUCCESS, userData);
  }
  
  LOG_MSG_INFO1_2("Manager::NotifySocketsReleased() : "
                   "call back %x userData %x ", callBackFunc, userData);
  
}/*Manager::NotifySocketsReleased*/

void Manager::NotifySocketsAllocated
(
  EphemeralPortManager  * cachePtr,
  boolean                 isAnySocketAlloc
)
{
  PortReqCallBack   appCallBack = NULL;
  
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if ( NULL != cachePtr && TRUE == cachePtr->IsClientCallBackPending())
  {
    LOG_MSG_INFO1_4("Manager::NotifySocketsAllocated() : "
                    "call back pending %d callback %x handle %d isAnySocketAlloc %d", 
                     cachePtr->IsClientCallBackPending(),
                     appCallBack, cachePtr->GetSocketsEphemeralHandle(), isAnySocketAlloc);
    
    appCallBack = cachePtr->GetClientCallBackFunc();
     
    if( TRUE == isAnySocketAlloc )
    {
      /*-------------------------------------------------------------------------
       Notify to Application :: Successfully get some sockets from AP 
      -------------------------------------------------------------------------*/ 
      appCallBack( DSS_SUCCESS, 
                   cachePtr->GetSocketsEphemeralHandle(),
                   cachePtr->GetClientCallBackData());
    }
    else
    {
        /*-------------------------------------------------------------------------
         Notify to Application ::Report Failure to application as we are not able to get Any socket from AP.
         Application either continue to use this handle(in such case we allocate ports with in modem) or 
         Application can release the preference and request later .
        -------------------------------------------------------------------------*/ 
      appCallBack( DSS_ERROR, 
                   cachePtr->GetSocketsEphemeralHandle(),
                   cachePtr->GetClientCallBackData());
    }
    cachePtr->SetClientCallBackPending(FALSE);
  }
}/*Manager::NotifySocketsAllocated*/

void Manager::SendUDPEncapsSockOpt
(
  uint32    sockHandle,
  boolean   udpEncapsSockOpt
)
{
  ps_sys_remote_socket_set_option_type       sockOptInfo;
  int16                                      ps_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  LOG_MSG_INFO1_1("Manager::SendUDPEncapsSockOpt() : handle %d ", sockHandle);

  if (UINT32_MAX != sockHandle)
  {
    memset(&sockOptInfo, 0, sizeof(ps_sys_remote_socket_set_option_type));                       
    sockOptInfo.socket_handle =  sockHandle;     
    sockOptInfo.is_udp_encaps = udpEncapsSockOpt;        
    ps_sys_event_ind_ex(PS_SYS_TECH_ALL, 
                        PS_SYS_EVENT_REMOTE_SOCKET_SET_OPTION,
                        PS_SYS_PRIMARY_SUBS,
                        &sockOptInfo,
                        &ps_errno);  
  }
} /* Manager::SendUDPEncapsSockOpt */

ds::AddrFamilyType RemoteSocketUtils::ConvertToDSSockFamily
(
  ip_version_enum_type family
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if (family == IP_V4)
  {
    return ds::AddrFamily::QDS_AF_INET;
  }
  else if (family == IP_V6)
  {
    return ds::AddrFamily::QDS_AF_INET6;
  } 
  else
  {
    return ds::AddrFamily::QDS_AF_UNSPEC;
  }  
}/* RemoteSocketUtils::ConvertToDSSockFamily */

ds::Sock::ProtocolType RemoteSocketUtils::ConvertToDSSockProtocol
(
  ps_ip_protocol_enum_type protocol
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if (protocol == PS_IPPROTO_UDP)
  {
    return ds::Sock::Protocol::QDS_UDP;
  }
  else if (protocol == PS_IPPROTO_TCP)
  {
    return ds::Sock::Protocol::QDS_TCP;
  } 
  else
  {
    return ds::Sock::Protocol::QDS_UNSPEC;
  }  
}/* RemoteSocketUtils::ConvertToDSSockProtocol */

ip_version_enum_type RemoteSocketUtils::ConvertToPSFamily
(
  ds::AddrFamilyType family
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if (family == ds::AddrFamily::QDS_AF_INET)
  {
    return IP_V4;
  }
  else
  {
    return IP_V6;
  } 
}/* RemoteSocketUtils::ConvertToPSFamily */

ps_ip_protocol_enum_type RemoteSocketUtils::ConvertToPSProtocol
(
  ds::Sock::ProtocolType protocol
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 
  if (protocol == ds::Sock::Protocol::QDS_UDP)
  {
    return PS_IPPROTO_UDP;
  }
  else if (protocol == ds::Sock::Protocol::QDS_TCP)
  {
    return PS_IPPROTO_TCP;
  } 
  else
  {
    return PS_IPPROTO_TCP_UDP;
  }  
}/* RemoteSocketUtils::ConvertToPSProtocol */

#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */
