/*=========================================================================*/
/*!
@file
ps_FTPserverMgr.cpp

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

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_FTPServerMgr.cpp#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
06/06/18    fn     Fix crash found in qdma testing
02/08/14    fn     Created Module  
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <stringl/stringl.h>
#include "comdef.h"

#include "ps_FTPServer.h"
#include "ps_FTPServerCore.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "ds_Utils_DebugMsg.h"
#include "ps_ftp_server_def.h"
#include "ps_system_heap.h"
#include "dssocket.h"
#include "ps_iface.h"
#include "ps_svc.h"
#include "ps_app_logging_util.h"

#ifdef __cplusplus
}
#endif  // extern "C"

using namespace PS_FTP;

/*===========================================================================

DEFINITIONS

===========================================================================*/
/** @def FTPServerIDLE */
#define FTPServerIDLE         0x00000000

/** @def FTPServerInit */
#define FTPServerInit         0x00000001

/** @def FTPServerConfigure */
#define FTPServerConfigured   0x00000010

/** @def  FTPServerStarted */
#define FTPServerStarted      0x00000100

/** @def  FTPServerAborting */
#define FTPServerAborting     0x00001000

/** @def FTPServerStopped */
#define FTPServerStopped      0x00010000

/*===========================================================================

INTERNAL DATA DEFINITIONS

===========================================================================*/
struct ps_ftp_net_evt_info
{
  const sint15            nethandle;
  const dss_iface_id_type iface_id;
  const sint15            error;
  const void*             netcb_userdata;

  ps_ftp_net_evt_info(sint15 nhandle, dss_iface_id_type iface, sint15 err, 
                      void* userdata)
  : nethandle(nhandle), iface_id(iface), error(err), 
    netcb_userdata(userdata) { }
};

struct ps_ftp_sock_evt_info
{
  const sint15 nethandle;
  const sint15 sockfd;
  const uint32 event_mask;
  const void* sockcb_userdata;

  ps_ftp_sock_evt_info(sint15 nhandle, sint15 sfd, uint16 emask, void* userdata)
  : nethandle(nhandle), sockfd(sfd), event_mask(emask), 
    sockcb_userdata(userdata) { }
};

/*===========================================================================

EXPORTED METHODS

===========================================================================*/
static void ps_ftp_server_client_notify
(
  uint32 event_mask,
  void*  ftp_server,
  void*  ftp_data,
  uint16 ftp_data_len
);
static void ps_ftp_server_net_cb
(
  sint15            nethandle,
  dss_iface_id_type iface_id,
  sint15            err,
  void*             net_cb_user_data
);

static void ps_ftp_server_sock_cb
(
  sint15 nethandle,
  sint15 sockfd,
  uint32 event_mask,
  void*  sock_cb_user_data
);


/*===========================================================================

PUBLIC METHODS

===========================================================================*/
/**-------------------------------------------------------------------------- 
*   @fn FTPServer()
*   @brief Standard constructor
---------------------------------------------------------------------------*/
FTPServer::FTPServer()
: 
  net_policy_ptr(NULL),
  ftp_server_config(NULL),
  ftp_server_evt_cback(NULL),
  ftp_server_evt_mask(0x0),
  mpFTPServerCore(NULL),
  FTPServerAppId(-1),
  ftp_server_state(FTPServerIDLE),
  ftp_server_inst(-1)
{
  ftp_server_state |= FTPServerInit;

  net_policy_ptr = (dss_net_policy_info_type*)
    ps_system_heap_mem_alloc(sizeof(dss_net_policy_info_type));

  if ( NULL == net_policy_ptr )
  {
    delete this;
    return;
  }

  ftp_server_config = (ps_ftp_server_config_type *)
            ps_system_heap_mem_alloc(sizeof(ps_ftp_server_config_type));

  if ( NULL == ftp_server_config )
  {
    delete this;
    return;
  }

  memset(net_policy_ptr, 0, sizeof(dss_net_policy_info_type));
  memset(ftp_server_config, 0, sizeof(ps_ftp_server_config_type));
}

/** @fn FTPServer()
*   @brief constructor to include inst
*/
FTPServer::FTPServer
(
   ps_ftp_server_inst_type inst
)
: net_policy_ptr(NULL),
  ftp_server_config(NULL),
  ftp_server_evt_cback(NULL),
  ftp_server_evt_mask(0x0),
  mpFTPServerCore(NULL),
  FTPServerAppId(-1),
  ftp_server_state(FTPServerIDLE),
  ftp_server_inst(inst)
{
  ftp_server_state |= FTPServerInit;

  net_policy_ptr = (dss_net_policy_info_type*)
    ps_system_heap_mem_alloc(sizeof(dss_net_policy_info_type));

  if ( NULL == net_policy_ptr )
  {
    delete this;
    return;
  }

  ftp_server_config = (ps_ftp_server_config_type *)
            ps_system_heap_mem_alloc(sizeof(ps_ftp_server_config_type));

  if ( NULL == ftp_server_config )
  {
    delete this;
    return;
  }

  memset(net_policy_ptr, 0, sizeof(dss_net_policy_info_type));
  memset(ftp_server_config, 0, sizeof(ps_ftp_server_config_type));
}


/**-------------------------------------------------------------------------- 
*   @fn  ~FTPServer()
*   @brief Default destructor.
---------------------------------------------------------------------------*/
FTPServer::~FTPServer ()
{
  sint15 dss_errno = DS_ENOERR, dss_ret = DSS_ERROR;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( FTPServerAppId > 0 )
  {
    dss_ret = dss_close_netlib2(FTPServerAppId, &dss_errno);
    if( DSS_SUCCESS != dss_ret )
    {
      PS_APP_ERR_DEBUG_3("dss_close_netlib(), net_handle %d, ret %d, errno %d", 
                         FTPServerAppId, dss_ret, dss_errno);
    }
    FTPServerAppId = -1;
  }

  if ( NULL != net_policy_ptr )
  {
    PS_SYSTEM_HEAP_MEM_FREE(net_policy_ptr);
    net_policy_ptr = NULL;
  }

  if ( NULL != ftp_server_config )
  {
    PS_SYSTEM_HEAP_MEM_FREE(ftp_server_config);
    ftp_server_config = NULL;
  }

  if ( NULL != mpFTPServerCore)
  {
    delete mpFTPServerCore;
    mpFTPServerCore = NULL;
  }

}

/**-------------------------------------------------------------------------- 
*   @fn virtual boolean Init()
*   @brief Initialization function.
---------------------------------------------------------------------------*/
boolean FTPServer::Init()
{
  ftp_server_state |= FTPServerInit;
  return TRUE;
}

/**--------------------------------------------------------------------------  
*   @fn virtual boolean Configure()
*   @brief Configure function.
---------------------------------------------------------------------------*/
boolean FTPServer::Configure
(
  dss_net_policy_info_type* net_policy_input, 
  ps_ftp_server_config_type* ftp_server_config_input,
  dss_ftp_server_event_cback_fn_type  reg_ftp_server_evt_cback,
  uint32                              reg_ftp_server_event_mask
)
{
  sint15 dss_errno = DS_ENOERR;
  dss_iface_ioctl_state_type iface_state;
  int retval;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( ((ftp_server_state & FTPServerInit) == 0) )
  {
    PS_APP_ERR_LOG_1(ftp_server_state);
    return FALSE;
  }

  /*-------------------------------------------------------------------------
    Initialize netpolicy
      if not provided use prefer UP policy but not a typical
      if provided check iface state
  -------------------------------------------------------------------------*/
  if ( NULL == net_policy_input ) 
  {
    PS_APP_DEBUG_0("Configure: warning netpolicy not provided to configure FTP "
                   "server using default and policy_up preferred");
    dss_init_net_policy_info(net_policy_ptr);
    net_policy_ptr->policy_flag = DSS_IFACE_POLICY_UP_PREFERRED;
  }
  else
  {
     memscpy(net_policy_ptr, sizeof(dss_net_policy_info_type),
            net_policy_input, sizeof(dss_net_policy_info_type));

    if ( DSS_IFACE_NAME == net_policy_ptr->iface.kind )
    {
      PS_APP_DEBUG_1("Configure: warning iface_id not provided, "
                     "using iface name inst 0",
                     net_policy_ptr->iface.info.name);

      net_policy_ptr->iface.kind = DSS_IFACE_ID;
      net_policy_ptr->iface.info.id = (dss_iface_id_type)PS_IFACE_GET_ID3(
         (ps_iface_name_enum_type)net_policy_ptr->iface.info.name,0,0);
    }
    if ( DSS_IFACE_INVALID_ID == net_policy_ptr->iface.info.id )
    {
      PS_APP_ERR_DEBUG_1("Configure:Invalid iface id in netpolicy",
                         net_policy_ptr->iface.info.id);
      return FALSE;
    }
  }

  /*-------------------------------------------------------------------------
    FTP server will open a netlib in the case user did not provide a
    netpolicy_ptr and obtain a valid iface id
  -------------------------------------------------------------------------*/ 
  FTPServerAppId = dss_open_netlib2 (ps_ftp_server_net_cb,
                                     (void*)this,
                                     ps_ftp_server_sock_cb, 
                                     (void*)this, 
                                     net_policy_ptr, 
                                     &dss_errno);

 
  if ( DSS_ERROR == FTPServerAppId )
  {
    PS_APP_ERR_DEBUG_3("Configure: Unable to open netlib for application "
                       "net_policy.family %d iface id %d errno %i",
                       net_policy_ptr->family, net_policy_ptr->iface.info.id, dss_errno);
    return FALSE;
  }

  net_policy_ptr->iface.kind = DSS_IFACE_ID;
  net_policy_ptr->iface.info.id = dss_get_iface_id(FTPServerAppId);

  /*-----------------------------------------------------------------------
    Check to make sure the iface is in UP or ROUTABLE state
  -----------------------------------------------------------------------*/
  retval = dss_iface_ioctl(net_policy_ptr->iface.info.id, 
                           DSS_IFACE_IOCTL_GET_STATE,
                           &iface_state,
                           &dss_errno);
  if ( (DSS_ERROR == retval) ||
      (IFACE_UP != iface_state && IFACE_ROUTEABLE != iface_state) )
  {
    PS_APP_ERR_LOG_2(dss_errno, iface_state);
    return FALSE;
  }
 
  PS_APP_DEBUG_3("Configure: netpolicy iface id %ld, iface state = %d, family %d",
                net_policy_ptr->iface.info.id, iface_state,
                net_policy_ptr->family );

  /*-------------------------------------------------------------------------
    Initialize FTP server configurations
  -------------------------------------------------------------------------*/
  if ( NULL == ftp_server_config_input )
  {
    /*FN todo call and initialize ftp server config to defaults*/
  }
  else
  {
    memscpy(ftp_server_config, sizeof(ps_ftp_server_config_type),
            ftp_server_config_input, sizeof(ps_ftp_server_config_type));
  }

  ftp_server_evt_cback = reg_ftp_server_evt_cback;
  ftp_server_evt_mask = reg_ftp_server_event_mask;
  ftp_server_state |= FTPServerConfigured;
  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn virtual boolean Start()
*   @brief Start the FTP server
---------------------------------------------------------------------------*/
boolean FTPServer::Start
( 
  ps_sockaddr_any_type* local_sockaddr
)
{

  if ( (ftp_server_state & FTPServerConfigured) == 0 ||
       (NULL == local_sockaddr) ||
       (ftp_server_state & FTPServerStarted) != 0 )
  {
    PS_APP_ERR_LOG_1(ftp_server_state);
    return FALSE;
  }


  if ( NULL == mpFTPServerCore )
  {
    mpFTPServerCore = new FTPServerCore(
       net_policy_ptr,
       ftp_server_config,
       ps_ftp_server_client_notify,
       (void*)this,
       FTPServerAppId,
       ftp_server_evt_mask );
  }

  if (FALSE == mpFTPServerCore->Start(local_sockaddr))
  {
    PS_APP_ERR_DEBUG_1("FTP server start failure 0x%x", ftp_server_state);
    delete mpFTPServerCore;
    mpFTPServerCore = NULL;
    return FALSE;
  }

  ftp_server_state |= FTPServerStarted;
  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn virtual boolean Abort()
*   @brief Abort the FTP server
---------------------------------------------------------------------------*/
boolean FTPServer::Abort()
{
  if ( (ftp_server_state & FTPServerStarted) == 0 )
  {
    PS_APP_ERR_DEBUG_1("Abort: Not started, %ld",ftp_server_state);
    return FALSE;
  }

  if ( NULL != mpFTPServerCore)
  {
    mpFTPServerCore->Abort();
    mpFTPServerCore = NULL;
  }

  ftp_server_state |= FTPServerAborting;
  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn virtual boolean Stop()
*   @brief Stop the FTP server
---------------------------------------------------------------------------*/
boolean FTPServer::Stop()
{
  ftp_server_state |= FTPServerStopped;

  if ( (ftp_server_state & FTPServerStarted) == 0 )
  {
    PS_APP_ERR_DEBUG_1("Stop: Not started, %ld",ftp_server_state);
    return FALSE;
  }

  if ( NULL != mpFTPServerCore)
  {
    mpFTPServerCore->Stop();
  }

  ftp_server_state &= (~FTPServerStarted);
  ftp_server_state &= (~FTPServerAborting);
  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn virtual boolean DenyConnection()
*   @brief Deny a connection
---------------------------------------------------------------------------*/
boolean FTPServer::DenyConnection()
{

  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn virtual boolean AcceptConnection()
*   @brief Accept a connection
---------------------------------------------------------------------------*/
boolean FTPServer::AcceptConnection()
{
  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn virtual boolean AcceptConnection()
*   @brief Accept a connection
---------------------------------------------------------------------------*/
boolean FTPServer::RegisterNotifyCallback()
{
  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn virtual sint15 getServerAppId()
*   @brief Gives the app id value
---------------------------------------------------------------------------*/
sint15 FTPServer::getServerAppId ( void)
{
  return FTPServerAppId;
}


/**-------------------------------------------------------------------------- 
*   @fn  ConnectionServerEventNotify ()
*   @brief Function call by the core to notify server events
---------------------------------------------------------------------------*/
void FTPServer::EventNotifyClient
( 
  uint32 event_mask,
  void  *ftp_data,
  uint16 ftp_data_len                      
)
{
  uint32                     mask = 0;
  uint8                      i = 0;

  /*------------------------------------------------------------------------
    Verify inputs
  ------------------------------------------------------------------------*/

  /*------------------------------------------------------------------------
    Notify user only if both the event mask from server and user is set
  ------------------------------------------------------------------------*/
  PS_APP_DEBUG_3("EventNotifyClient: Event 0x%lx recieved for server_inst %i "
                  "register event mask 0x%lx",
                  event_mask, ftp_server_inst, ftp_server_evt_mask);

  if ( NULL != ftp_server_evt_cback)
  {
    do
    {
      mask = 1 << i;
      if( ( event_mask & mask )&&
        ( ftp_server_evt_mask & mask ))
      {
        ftp_server_evt_cback(
         ftp_server_inst,
         event_mask & mask,
         ftp_data,
         ftp_data_len);
      }
      
      i++;
    }
    while ( mask < DSS_FTP_SERVER_CLOSED_EVT ); 
  }
  else
  {
    PS_APP_DEBUG_0("Not notifying client since user_evt_cback is null" );
  }

  if ( DSS_FTP_SERVER_CLOSED_EVT & event_mask )
  {
    PS_APP_DEBUG_0("closed event, deleting FTPServer" );
    delete this;
  }
}

/**-------------------------------------------------------------------------- 
*   @fn  ProcessSockEvt ()
*   @brief Calls into FTP Server Core to process socket evt
---------------------------------------------------------------------------*/
void FTPServer::ProcessSockEvt
(
  uint32 event_mask,
  sint15 evt_sockfd
)
{
  if ( NULL != mpFTPServerCore )
  {
    mpFTPServerCore->ProcessSockEvt(event_mask, evt_sockfd);
  }
  else
  {
    PS_APP_ERR_DEBUG_2("FTP Server core not started yet sockfd %d, event mask 0x%x",
                    evt_sockfd, event_mask);
  }
   
}
/*===========================================================================

PRIVATE METHODS

===========================================================================*/
void ps_ftp_server_client_notify
(
  uint32 event_mask,
  void*  ftp_server,
  void*  ftp_data,
  uint16 ftp_data_len
)
{
  FTPServer* mpFTPServer = NULL;
  if ( NULL == ftp_server )
  {
    PS_APP_ERR_LOG_2(ftp_server, event_mask );
    return;
  }

  mpFTPServer = (FTPServer*) ftp_server;
  mpFTPServer->EventNotifyClient(event_mask, ftp_data, ftp_data_len);

}


void ps_ftp_server_net_cb
(
  sint15            nethandle,
  dss_iface_id_type iface_id,
  sint15            err,
  void*             net_cb_user_data
)
{
  if ( NULL == net_cb_user_data )
  {
  }
}

static void ps_ftp_sock_evt_handler
(
   ps_cmd_enum_type cmd,
   void* sockdata
)
{
  ps_ftp_sock_evt_info* sock_evt = (ps_ftp_sock_evt_info*) sockdata;
  FTPServer* ftp_server = NULL;

  if (NULL == sock_evt || NULL == sock_evt->sockcb_userdata || 
      cmd != PS_FTP_SOCK_CMD)
  {
    PS_APP_ERR_LOG_2(sock_evt, cmd);

    if ( NULL != sock_evt )
    {
      delete sock_evt;
    }
    return;
  }

  ftp_server = (FTPServer*) sock_evt->sockcb_userdata;

  ftp_server->ProcessSockEvt(sock_evt->event_mask, 
                             sock_evt->sockfd );
  delete sock_evt;
}

void ps_ftp_server_sock_cb
(
  sint15 nethandle,
  sint15 sockfd,
  uint32 event_mask,
  void*  sock_cb_user_data
)
{
  ps_ftp_sock_evt_info* sock_evt = new ps_ftp_sock_evt_info(nethandle, sockfd, event_mask,
                                                            sock_cb_user_data);

  if(NULL == sock_evt || NULL == sock_cb_user_data)
  {
    PS_APP_ERR_LOG_2(sock_evt, sock_cb_user_data);

    if ( NULL != sock_evt )
    {
      delete sock_evt;
    }

    return;
  }

  (void)ps_set_cmd_handler(PS_FTP_SOCK_CMD, ps_ftp_sock_evt_handler);
	ps_send_cmd(PS_FTP_SOCK_CMD,(void*) sock_evt );
  return;
}
