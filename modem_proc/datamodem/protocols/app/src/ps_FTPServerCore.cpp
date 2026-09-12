/*=========================================================================*/
/*!
@file
ps_FTPSeverCore.cpp

@brief
Provides helper routines for mock TCP application off-target test execution (QTF)

Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_FTPServerCore.cpp#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module 
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <stringl/stringl.h>
#include "comdef.h"

#include "ps_FTPServerCore.h"
#include "ps_FTPSocket.h"
#include "ps_FTPCmdResp.h"
#include "ps_AppEFS.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"
#include "dssocket.h"
#include "ps_ftp_server_pb.h"
#include "ps_app_logging_util.h"

#ifdef __cplusplus
}
#endif  // extern "C"

using namespace PS_FTP;
using namespace PS_APP;
/*===========================================================================

DEFINITIONS

===========================================================================*/

/*===========================================================================

INTERNAL DATA DEFINITIONS

===========================================================================*/
#define FTP_SERVER_MAX_REQ_LEN 255

/*===========================================================================

EXPORTED METHODS

===========================================================================*/


/*===========================================================================

Globals

===========================================================================*/

/*===========================================================================

METHODS

===========================================================================*/


/**-------------------------------------------------------------------------- 
*   @fn FTPServerCore()
*   @brief Standard constructor
---------------------------------------------------------------------------*/
FTPServerCore::FTPServerCore
(
  dss_net_policy_info_type* net_policy_info,
  ps_ftp_server_config_type* ftp_server_config_info,
  ps_ftp_server_event_cback_fn_type client_notify_cback,
  void* ftp_server,
  sint15 serverAppId,
  uint32 evt_mask
):
net_policy_ptr ( net_policy_info),
notify_cback ( client_notify_cback),
ftp_server_class ( ftp_server),
app_id ( serverAppId),
sock_ip_family(DSS_PF_INET),
sockaddr_info(NULL),
login_status_bmask(PS_FTP_INIT_LOGIN),
login_state(PS_FTP_INIT_LOGIN),
data_state_bmask(PS_FTP_DATA_INIT_DATA_ST),
data_state(PS_FTP_DATA_INIT_DATA_ST),
mpCtrlServerSock(NULL),
mpDataServerSock(NULL),
efs_file_handle(-1),
efs_file_size(0),
data_cmd_success(FALSE)
{
  mpCmdResp = new FTPCmdResp();
  if(NULL == mpCmdResp )
  {
    delete this;
  }
}

/**-------------------------------------------------------------------------- 
*   @fn  ~FTPServerCore()
*   @brief Default destructor.
---------------------------------------------------------------------------*/
FTPServerCore::~FTPServerCore()
{
  if( NULL != mpCtrlServerSock )
  {
    delete mpCtrlServerSock;
    mpCtrlServerSock = NULL;
  }

  if( NULL != mpDataServerSock )
  {
    delete mpDataServerSock;
    mpDataServerSock = NULL;
  }

  if ( NULL != sockaddr_info )
  {
    PS_SYSTEM_HEAP_MEM_FREE(sockaddr_info);
    sockaddr_info = NULL;
  }

  if( NULL != mpCmdResp )
  {
    delete mpCmdResp;
    mpCmdResp = NULL;
  }
}


/**-------------------------------------------------------------------------- 
*   @fn Start()
*   @brief Starts FTP Server socket and waits for connections
---------------------------------------------------------------------------*/
boolean FTPServerCore::Start
( 
  ps_sockaddr_any_type* local_sockaddr
)
{
  if( NULL == mpCtrlServerSock )
  {
    mpCtrlServerSock = new FTPSocket(net_policy_ptr,app_id, 
                                           local_sockaddr, FALSE);
    if( NULL == mpCtrlServerSock)
    {
      PS_APP_ERR_DEBUG_1("Start: Unable to allocate memory for server socket 0x%p",
                    mpCtrlServerSock);
      return FALSE;
    }
  }

  if ( FALSE ==  mpCtrlServerSock->ListenBindSocket(local_sockaddr) )
  {
    CleanSocket(mpCtrlServerSock);
    return FALSE;
  }

  ctrl_sockfd = mpCtrlServerSock->GetSockFd();

  EventNotify(DSS_FTP_SERVER_READY_EVT , NULL, 0);

  sockaddr_info = (ps_sockaddr_any_type*) 
    ps_system_heap_mem_alloc(sizeof(ps_sockaddr_any_type));

  if( NULL == sockaddr_info )
  {
    return FALSE;
  }

  memscpy(sockaddr_info, sizeof(ps_sockaddr_any_type),
          local_sockaddr, sizeof(ps_sockaddr_any_type));

  sock_ip_family = local_sockaddr->type;

  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn Start()
*   @brief Starts FTP Server socket and waits for connections
---------------------------------------------------------------------------*/
void FTPServerCore::ProcessSockEvt
( 
  uint32 event_mask, 
  sint15 sockfd 
)
{
  if(sockfd == data_sockfd)
  {
    if( NULL == mpDataServerSock )
    {
      PS_APP_ERR_DEBUG_2("mpDataServerSock is null, event mask 0x%x, data_sockfd %i",
                         event_mask, sockfd);
    }
    ProcessDataSockEvt(event_mask);
  }
  else if(sockfd == ctrl_sockfd)
  {
    if( NULL == mpCtrlServerSock )
    {
      PS_APP_ERR_DEBUG_2("mpCtrlServerSock is null, event mask 0x%x, ctrl_sockfd %i",
                         event_mask, sockfd);
    }
    ProcessCtrlSockEvt(event_mask);
  }
  else
  {
    PS_APP_ERR_DEBUG_2("ProcessSockEvt: Event 0x%lx for unknown sockfd %d",
                       event_mask, sockfd);
  }

}

/**-------------------------------------------------------------------------- 
*   @fn Stop()
*   @brief Stops FTP Server socket and waits for connections
---------------------------------------------------------------------------*/
void FTPServerCore::Stop()
{
  if( NULL != mpCtrlServerSock  )
  {
    if( TRUE == CleanSocket(mpCtrlServerSock) )
    {
      mpCtrlServerSock = NULL;
      EventNotify(DSS_FTP_SERVER_CLOSED_EVT, NULL, 0);
    }
  }
}

/**-------------------------------------------------------------------------- 
*   @fn Abort()
*   @brief Abort FTP Server
---------------------------------------------------------------------------*/
void FTPServerCore::Abort()
{
  delete this;
}

/**-------------------------------------------------------------------------- 
*   @fn  ConnectionServerEventNotify ()
*   @brief Function call by the core to notify server events
---------------------------------------------------------------------------*/
void FTPServerCore::EventNotify
( 
  uint32                        event,
  void*                         ftpdata,
  uint16                        ftpdata_len
)
{
  notify_cback(event, ftp_server_class, ftpdata, ftpdata_len);
}

/**-------------------------------------------------------------------------- 
*   @fn ProcessDataSockEvt()
*   @brief process data socket events
---------------------------------------------------------------------------*/
void FTPServerCore::ProcessDataSockEvt(uint32 event_mask)
{

  PS_APP_DEBUG_2("Data_state %d evt mask 0x%x",data_state, event_mask);

  switch( data_state )
  {
     case PS_FTP_DATA_PASSIVE_ST:
       if( event_mask & DS_ACCEPT_EVENT)
       {
         /* Accept and initialize for data cmds*/
         Accept(TRUE);
         data_cmd_success = FALSE;
       }
       else
       {
         PS_APP_DEBUG_1("Ignoring event in PS_FTP_DATA_PASSIVE_ST state evt mask 0x%x",
                        event_mask);
       }
       break;
     case PS_FTP_DATA_TRANSFERING_ST:
       if( event_mask & DS_WRITE_EVENT )
       {
         if( NULL != mpDataServerSock &&
             TRUE == 
             mpDataServerSock->WriteFromEFS(efs_file_handle, efs_file_size, FALSE) )
         {
           data_cmd_success = TRUE;
           MoveDataState(PS_FTP_DATA_TRANSFER_COMPLETE_ST);
         }
         return;
       }
       break;
     case PS_FTP_DATA_CONNECTED_ST:
     default:
       if( ! (event_mask & DS_CLOSE_EVENT) )
       {
         PS_APP_ERR_DEBUG_2("Ignoring event in state 0x%d evt mask 0x%x",
                           data_state, event_mask);
       }
       break;
  }

  if( event_mask & DS_CLOSE_EVENT )
  {
    if(TRUE == CleanSocket(mpDataServerSock))
    {
      mpDataServerSock = NULL;
      PS_APP_DEBUG_1("Data connection closed %d", data_state);
    }
  }
}

/**-------------------------------------------------------------------------- 
*   @fn ProcessCtrlSockEvt()
*   @brief process ctrl socket events
---------------------------------------------------------------------------*/
void FTPServerCore::ProcessCtrlSockEvt(uint32 event_mask)
{
  PS_APP_DEBUG_2("Process ctrl sock ev, evt_msk 0x%x, login state %d",
                   event_mask, login_state);
  switch( login_state )
  {
     case PS_FTP_INIT_LOGIN:
       if( event_mask & DS_ACCEPT_EVENT)
       {
         StartCtrlServer();
       }
       else
       {
         PS_APP_DEBUG_1("Ignoring event in PS_FTP_INIT_LOGIN state evt mask 0x%x",
                         event_mask);
       }
       break;
     case PS_FTP_WELCOME_LOGIN:
     case PS_FTP_USERNAME_LOGIN:
     case PS_FTP_PASSWORD_LOGIN:
     case PS_FTP_ACCOUNT_LOGIN:
       if( event_mask & DS_READ_EVENT)
       {
         ProcessLogin();
       }
       break;
     case PS_FTP_AUTHENTICATED_LOGIN:
       if( event_mask & DS_READ_EVENT)
       {
         ProcessCtrlCmd();
       }
       break;
     case PS_FTP_CLOSING_LOGIN:
     case PS_FTP_CLOSED_LOGIN:
       /* handle below in close event*/
       break;
     default:
       PS_APP_ERR_DEBUG_2("Unexpected state %d, event mask 0x%x", 
                       login_state, event_mask);
       break;
  }

  if( event_mask & DS_CLOSE_EVENT )
  {
    if(NULL == mpCtrlServerSock ||
       TRUE == CleanSocket(mpCtrlServerSock))
    {
      mpCtrlServerSock = NULL;
      EventNotify(DSS_FTP_SERVER_CLOSED_EVT, NULL, 0);
    }
  }

}

/**-------------------------------------------------------------------------- 
*   @fn  MoveLoginState
*   @brief Updates login state and mask
--------------------------------------------------------------------------*/
void FTPServerCore::MoveLoginState
( 
   ps_ftp_server_login_status_e_type next_st 
)
{
  PS_APP_DEBUG_3("FTP Server Core: current login state %d mask 0x%x, next state 0x%d",
                 login_state, login_status_bmask, next_st);

  if( next_st > PS_FTP_SAME_LOGIN_ST && next_st < PS_FTP_MAX_LOGIN_STATUS )
  {
    login_status_bmask |= (1 << next_st);
    login_state = next_st;  
  }

  PS_APP_DEBUG_3("FTP Server Core: updated login state %d mask 0x%x",
                 login_state, login_status_bmask, next_st);
}

/**--------------------------------------------------------------------------
*   @fn  MoveDataState
*   @brief Updates data state and mask
--------------------------------------------------------------------------*/
void FTPServerCore::MoveDataState
( 
   ps_ftp_server_data_state_e_type next_st
)
{
  PS_APP_DEBUG_3("FTP Server Core: current data state %d mask 0x%x, next state 0x%d",
                  data_state, data_state_bmask, next_st);

  if( next_st > PS_FTP_DATA_SAME_ST && next_st < PS_FTP_DATA_MAX_ST )
  {
    data_state_bmask |= (1 << next_st);
    data_state = next_st;
  }
  PS_APP_DEBUG_3("FTP Server Core: updated data state %d mask 0x%x",
                  data_state, data_state_bmask, next_st);

  switch( next_st)
  {
     case PS_FTP_DATA_INIT_DATA_ST:
       data_cmd_success = FALSE;
       efs_file_handle = -1;
       efs_file_size = 0;
       data_sockfd = -1;
       break;
     case PS_FTP_DATA_TRANSFERING_ST:
       /*start the data transfer*/
       if( TRUE == 
           mpDataServerSock->WriteFromEFS(efs_file_handle, efs_file_size, TRUE) )
       {
         data_cmd_success = TRUE;
         MoveDataState(PS_FTP_DATA_TRANSFER_COMPLETE_ST);
         return;
       }
       break;
     case PS_FTP_DATA_TRANSFER_COMPLETE_ST:
       if(TRUE == CleanSocket(mpDataServerSock))
       {
         mpDataServerSock = NULL;
         return;
       }
       break;
     case PS_FTP_DATA_CLOSED_ST:
       /*send data transfer complete and reset data state */
       DataCloseComplete();
       MoveDataState(PS_FTP_DATA_INIT_DATA_ST);
       break;
     default:
         break;
     
  }
}

/**--------------------------------------------------------------------------
*   @fn  MoveDataState
*   @brief Updates data state and mask
--------------------------------------------------------------------------*/
boolean FTPServerCore::CleanSocket
( 
   FTPSocket *mpSock
)
{
  if( NULL == mpSock)
  {
    return TRUE;
  }

  /*close data connection first if we are trying to close ctrl connection*/
  if( mpSock == mpCtrlServerSock && NULL != mpDataServerSock)
  {
    mpDataServerSock->CloseSocket(); 
  }

  /*close the intended socket, if successful delete socket */
  if(TRUE == mpSock->CloseSocket( ))
  {
    
    if( mpSock == mpCtrlServerSock )
    {
      MoveLoginState(PS_FTP_CLOSED_LOGIN);
    }
    else if ( mpSock == mpDataServerSock )
    {
      MoveDataState(PS_FTP_DATA_CLOSED_ST);
    }
    delete mpSock;
    return TRUE;
  }

  /* For control socket, move it to closing to finish cleanup */
  if( mpSock == mpCtrlServerSock )
  {
    MoveLoginState(PS_FTP_CLOSING_LOGIN);
  }

  return FALSE;
}


/**-------------------------------------------------------------------------- 
*   @fn StartCtrlServer()
*   @brief Starts the FTP session with FTP client
---------------------------------------------------------------------------*/
void FTPServerCore::StartCtrlServer( void )
{
  PS_APP_DEBUG_1("StartCtrlServer: Recieved accept, starting FTP session %d",
                 login_state);

  if( NULL == mpCtrlServerSock )
  {
    PS_APP_ERR_LOG_1(mpCtrlServerSock);
    return;
  }

  /*FN todo add ability for client to accept*/
  EventNotify(DSS_FTP_SERVER_ACCEPT_EVT, NULL, 0);

  if(FALSE == mpCtrlServerSock->AcceptSocket(&ctrl_sockfd, NULL))
  {
    CleanSocket(mpCtrlServerSock);
    mpCtrlServerSock = NULL;
    return;
  }

  mpCmdResp->SendCmd( FTP_SERVER_MSG_SERVER_READY ,
                              mpCtrlServerSock, NULL );

  if(FALSE == mpCtrlServerSock->RegisterEvt(DS_READ_EVENT | 
                                            DS_WRITE_EVENT |
                                             DS_CLOSE_EVENT))
  {
    CleanSocket(mpCtrlServerSock);
    mpCtrlServerSock = NULL;
    return;
  }

  MoveLoginState(PS_FTP_WELCOME_LOGIN);

  /*Notify client*/
  EventNotify(DSS_FTP_SERVER_CONNECTED_EVT, NULL, 0);
 
}

/**-------------------------------------------------------------------------- 
*   @fn ProcessLogin()
*   @brief processes logins cmd
---------------------------------------------------------------------------*/
void FTPServerCore::ProcessLogin( void )
{
  ftp_msg_response_type ftp_cmd;
  ps_ftp_server_login_status_e_type next_state = PS_FTP_SAME_LOGIN_ST;
  ftp_server_msg_e_type server_resp_msg = FTP_SERVER_MSG_INVALID;
  char * buffer = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( NULL == mpCmdResp )
  {
    PS_APP_ERR_DEBUG_1("ProcessLogin: Error with mpCmdResp 0x%p", mpCmdResp);
    return;
  }

  buffer = (char*)ps_system_heap_mem_alloc( FTP_SERVER_MAX_REQ_LEN );

  if( NULL == buffer )
  {
    PS_APP_ERR_LOG_1(FTP_SERVER_MAX_REQ_LEN );
    return; 
  }

  if( FALSE == mpCmdResp->ParseCmd(&ftp_cmd, mpCtrlServerSock, buffer))
  {
    ftp_cmd.client_cmd_resp.cmd = FTP_MAX_COMMAND_STRINGS;
    server_resp_msg = FTP_SERVER_MSG_UNKNOWN_CMD;
  }

  switch ( ftp_cmd.client_cmd_resp.cmd )
  {
    case FTP_MSG_USER_CMD:
       /*FN debug FN to do check username later*/
       if( login_status_bmask & (1 <<PS_FTP_WELCOME_LOGIN) )
       {
         server_resp_msg = FTP_SERVER_MSG_PASS_REQUIRE;
         next_state = PS_FTP_USERNAME_LOGIN;
       }
       else
       {
         PS_APP_ERR_DEBUG_2("Invalid state %d, and mask 0x%x",login_state, login_status_bmask );
       }
       break;
     case FTP_MSG_PASS_CMD:
       if( login_status_bmask & (1 << PS_FTP_USERNAME_LOGIN) )
       {
         server_resp_msg = FTP_SERVER_MSG_WELCOME;
         next_state = PS_FTP_PASSWORD_LOGIN;
       }
       else
       {
         PS_APP_ERR_DEBUG_2("Invalid state %d, and mask 0x%x",login_state, login_status_bmask );
       }
       break;
    default:
       PS_APP_ERR_LOG_3(ftp_cmd.client_cmd_resp.cmd, login_state, login_status_bmask );
       server_resp_msg = FTP_SERVER_MSG_UNKNOWN_CMD;
       break;
  }

  /*Send response to client*/
  if( FALSE == mpCmdResp->SendCmd(server_resp_msg, 
                                  mpCtrlServerSock, NULL))
  {
    PS_SYSTEM_HEAP_MEM_FREE(buffer);
    CleanSocket(mpCtrlServerSock);
    mpCtrlServerSock = NULL;    
    return;
  }

  /*Move to next login state*/
  MoveLoginState(next_state);

  /*Once passwod is verified, the client is authenticated*/
  if( next_state == PS_FTP_PASSWORD_LOGIN)
  {
    MoveLoginState(PS_FTP_AUTHENTICATED_LOGIN);
  }

  PS_SYSTEM_HEAP_MEM_FREE(buffer);
}


/**-------------------------------------------------------------------------- 
*   @fn ProcessLogin()
*   @brief processes logins cmd
---------------------------------------------------------------------------*/
void FTPServerCore::ProcessCtrlCmd( void )
{
  ftp_msg_response_type ftp_cmd;
  ps_ftp_server_login_status_e_type next_state = PS_FTP_SAME_LOGIN_ST;
  ps_ftp_server_data_state_e_type next_data_st = PS_FTP_DATA_SAME_ST;
  ftp_server_msg_e_type server_resp_msg = FTP_SERVER_MSG_INVALID;
  void * server_msg_arg = NULL;
  boolean file_readable = FALSE;
  ps_sockaddr_any_type data_sockaddr;
  char * buffer = NULL;
  AppEFS* mpEFS = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*Check user is authenticated to send commands*/
  if( NULL == mpCmdResp ||
      !(login_status_bmask & 1 << PS_FTP_AUTHENTICATED_LOGIN) )
  {
    PS_APP_ERR_LOG_1(mpCmdResp);
    return;
  }

  buffer = (char*)ps_system_heap_mem_alloc( FTP_SERVER_MAX_REQ_LEN );

  if( NULL == buffer )
  {
    PS_APP_ERR_LOG_0();
    return; 
  }

  if( FALSE == mpCmdResp->ParseCmd(&ftp_cmd, mpCtrlServerSock, buffer))
  {
    /*FN debug FN todo need to respond to client with Error?*/
    PS_SYSTEM_HEAP_MEM_FREE(buffer);
    return;
  }

  switch ( ftp_cmd.client_cmd_resp.cmd )
  {
     case FTP_MSG_PASV_CMD:
       if( NULL == mpDataServerSock )
       {
         memscpy(&data_sockaddr, sizeof(ps_sockaddr_any_type),
                 sockaddr_info, sizeof(ps_sockaddr_any_type));
         
         if( DSS_AF_INET6 == data_sockaddr.type )
         {
           data_sockaddr.sockaddr.v6.ps_sin6_port = 0;
           memset(&data_sockaddr.sockaddr.v6.ps_sin6_addr,0, 
                  sizeof(struct ps_in6_addr));
         }
         else
         {
           data_sockaddr.sockaddr.v4.ps_sin_port = 0;
           memset(&data_sockaddr.sockaddr.v4.ps_sin_addr,0, 
                  sizeof(struct ps_in_addr));
         }

         mpDataServerSock = new FTPSocket(
            net_policy_ptr,app_id, &data_sockaddr, TRUE);
       }
       if ( (NULL == mpDataServerSock) ||
            FALSE ==  mpDataServerSock->ListenBindSocket(&data_sockaddr) )
       {
         /*return error response*/
         if (TRUE == CleanSocket(mpDataServerSock))
         {
           mpDataServerSock = NULL;
         }
         return;
       }

       data_sockfd = mpDataServerSock->GetSockFd();
       server_msg_arg = (void*) &data_sockaddr;
       server_resp_msg = FTP_SERVER_MSG_PASV;
       next_state = PS_FTP_SAME_LOGIN_ST;
       next_data_st = PS_FTP_DATA_PASSIVE_ST;
       break;
     case FTP_MSG_RETR_CMD:
       mpEFS = new AppEFS();
       efs_file_handle =  mpEFS->OpenFile((const char*)ftp_cmd.client_cmd_resp.cmd_arg, 
                                          NULL,
                                          &efs_file_size, 
                                          FALSE);
       delete mpEFS;
       PS_APP_DEBUG_2("WriteSocket: open file handle %i, file size %i",
                       efs_file_handle, efs_file_size);

       if( efs_file_handle > 0 )
       {
         file_readable = TRUE;
         next_data_st = PS_FTP_DATA_TRANSFERING_ST;
       }
       else
       {
         file_readable = FALSE;
         next_data_st = PS_FTP_DATA_CONNECTED_ST;
       }

       next_state = PS_FTP_SAME_LOGIN_ST;
       server_resp_msg = FTP_SERVER_MSG_RETR;
       server_msg_arg = &file_readable;

       break;
    case FTP_MSG_SYST_CMD:
      break;
    case FTP_MSG_EPSV_CMD:
      break;
    case FTP_MSG_TYPE_CMD:
      break;
    case FTP_MSG_PORT_CMD:
      break;
    case FTP_MSG_QUIT_CMD:
      break;
    default:
      server_resp_msg = FTP_SERVER_MSG_UNKNOWN_CMD;
      break;
  }

  /*Send response to client*/
  if( FALSE == mpCmdResp->SendCmd(server_resp_msg, mpCtrlServerSock,
                                          server_msg_arg))
  {
    CleanSocket(mpCtrlServerSock);
    PS_SYSTEM_HEAP_MEM_FREE(buffer);
    return;
  }

  if( NULL != ftp_cmd.client_cmd_resp.cmd_arg )
  {
    PS_SYSTEM_HEAP_MEM_FREE(buffer);
  }

  /*Move to next login state and data state*/
  MoveLoginState(next_state);
  MoveDataState(next_data_st);


}

/**-------------------------------------------------------------------------- 
*   @fn FTPServerDataStart()
*   @brief Starts the FTP data connection with FTP client
---------------------------------------------------------------------------*/
void FTPServerCore::Accept( boolean is_data )
{

  FTPSocket *mpTempServerSock = NULL;
  sint15* psockfd = NULL;

  PS_APP_DEBUG_3("StartCtrlServer: Recieved accept, login st %d data_st %d, is_data %d",
                 login_state, data_state, is_data);

  /*FN todo add ability for client to accept*/
  EventNotify(DSS_FTP_SERVER_ACCEPT_EVT, NULL, 0);

  if( TRUE == is_data )
  {
    mpTempServerSock = mpDataServerSock;
    psockfd = &data_sockfd;
  }
  else
  {
    mpTempServerSock = mpCtrlServerSock;
    psockfd = &ctrl_sockfd;
  }

  if( NULL == mpTempServerSock )
  {
    PS_APP_ERR_LOG_3(is_data,mpDataServerSock,mpCtrlServerSock);
    return;
  }


  if(FALSE == mpTempServerSock->AcceptSocket(psockfd, NULL))
  {
    CleanSocket(mpTempServerSock);
    return;
  }

  if( FALSE == is_data )
  {
    mpCmdResp->SendCmd( FTP_SERVER_MSG_SERVER_READY ,
                                mpCtrlServerSock, NULL );
  }

  if(FALSE == mpTempServerSock->RegisterEvt(DS_READ_EVENT | 
                                            DS_WRITE_EVENT |
                                             DS_CLOSE_EVENT))
  {
    CleanSocket(mpTempServerSock);
    return;
  }

  if( TRUE == is_data )
  {
    MoveDataState(PS_FTP_DATA_CONNECTED_ST);
  }
  else
  {
    MoveLoginState(PS_FTP_WELCOME_LOGIN);
  }

  /*Notify client*/
  EventNotify(DSS_FTP_SERVER_CONNECTED_EVT, NULL, 0);
 
}

/**-------------------------------------------------------------------------- 
*   @fn DataCloseComplete()
*   @brief Closes data connection upon completion of data cmds rfc 959
*   section 5.2 connections 
---------------------------------------------------------------------------*/
void FTPServerCore::DataCloseComplete
( 
  void
)
{
  PS_APP_DEBUG_1("Sending data complete / closing conn, transfer status %d", 
                  data_cmd_success);
  EventNotify(DSS_FTP_SERVER_TRANSFER_COMPLETE_EVT, &data_cmd_success, 
              sizeof(data_cmd_success));
  if( FALSE == mpCmdResp->SendCmd(FTP_SERVER_MSG_TRANSFER_COMPLETE, 
                                  mpCtrlServerSock, &data_cmd_success))
  {
    PS_APP_ERR_DEBUG_0("Unable to send transfer complete response");
  }

}

