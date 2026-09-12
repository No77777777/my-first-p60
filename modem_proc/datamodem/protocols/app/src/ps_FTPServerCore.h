#ifndef PS_FTPSERVERCORE_H
#define PS_FTPSERVERCORE_H
/*=========================================================================*/
/*!
@file
ps_FTPServerCore.h

@brief
Application that provides FTP server service 
 
Copyright (c) 2014-15 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_FTPServerCore.h#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module  
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE 

===========================================================================*/

#include "comdef.h"

#include "ps_FTPServer.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ps_ftp_server_def.h"
#ifdef __cplusplus
}
#endif  // extern "C"

/*===========================================================================

CLASS DEFINITION

===========================================================================*/
namespace PS_FTP
{
  class FTPSocket;
  class FTPCmdResp;

  /**-------------------------------------------------------------------------- 
    *    @class FTPServerCore
    *    @brief PS FTP server core
  ---------------------------------------------------------------------------*/
  class FTPServerCore
  {
  public:

    /** @fn FTPServerCore ()
    *   @brief constructor
    */
    FTPServerCore(
       dss_net_policy_info_type* net_policy_info,
       ps_ftp_server_config_type* ftp_server_config_info,
       ps_ftp_server_event_cback_fn_type client_notify_cback,
       void* ftp_server,
       sint15 FTPServerAppId,
       uint32 evt_mask);

    /** @fn  ~FTPServerCore()
    *   @brief Default destructor.
    */
    ~FTPServerCore();

    /** @fn  Start
    *   @brief Starts the FTP Server
    */
    boolean Start(ps_sockaddr_any_type* sockaddr);

    /** @fn  ProcessSockEvt
    *   @brief Processes Socket Events
    */
    void ProcessSockEvt( uint32 event_mask, 
                         sint15 sockfd );
    /** @fn  Stops
    *   @brief Stops the FTP Server gracefully
    */
    void Stop();

    /** @fn  Aborts
    *   @brief Aborts the FTP Server
    */
    void Abort();

  protected:
                    
  private:
    typedef enum ps_ftp_server_login_status
    {
      PS_FTP_INIT_LOGIN = 0,
      PS_FTP_SAME_LOGIN_ST = PS_FTP_INIT_LOGIN,
      PS_FTP_WELCOME_LOGIN,
      PS_FTP_USERNAME_LOGIN,
      PS_FTP_PASSWORD_LOGIN,
      PS_FTP_ACCOUNT_LOGIN,
      PS_FTP_AUTHENTICATED_LOGIN,
      PS_FTP_CLOSING_LOGIN,
      PS_FTP_CLOSED_LOGIN,
      PS_FTP_MAX_LOGIN_STATUS = 15
    }ps_ftp_server_login_status_e_type;

    typedef enum ps_ftp_server_data_state
    {
      PS_FTP_DATA_INIT_DATA_ST = 0,
      PS_FTP_DATA_SAME_ST = PS_FTP_DATA_INIT_DATA_ST,
      PS_FTP_DATA_PASSIVE_ST,
      PS_FTP_DATA_ACTIVE_ST,
      PS_FTP_DATA_CONNECTED_ST,
      PS_FTP_DATA_TRANSFERING_ST,
      PS_FTP_DATA_TRANSFER_COMPLETE_ST,
      PS_FTP_DATA_CLOSED_ST,
      PS_FTP_DATA_MAX_ST = 15
    }ps_ftp_server_data_state_e_type;


    dss_net_policy_info_type      *net_policy_ptr;
    ps_ftp_server_event_cback_fn_type notify_cback;
    void                          *ftp_server_class;
    sint15                        app_id;
    dss_protocol_family_enum_type  sock_ip_family;
    ps_sockaddr_any_type          *sockaddr_info;
    uint16                        login_status_bmask;
    ps_ftp_server_login_status_e_type login_state;
    uint16                        data_state_bmask;
    ps_ftp_server_data_state_e_type  data_state;

    FTPSocket               *mpCtrlServerSock;
    sint15                        ctrl_sockfd;
    FTPSocket               *mpDataServerSock;
    FTPCmdResp              *mpCmdResp;
    sint15                        data_sockfd;
    int                           efs_file_handle;
    uint32                        efs_file_size;
    boolean                       data_cmd_success;
  
    /** @fn  EventNotify
    *   @brief notify client of events
    */      
    void EventNotify ( uint32  event_mask, void* ftpdata, uint16 ftpdata_len);

    /** @fn  ProcessDataSockEvt
    *   @brief process data socket events
    */
    void ProcessDataSockEvt(uint32 event_mask);

    /** @fn  ProcessCtrlSockEvt
    *   @brief process ctrl socket events
    */
    void ProcessCtrlSockEvt(uint32 event_mask);

    /** @fn  MoveLoginState
    *   @brief Updates login state and mask
    */
    void MoveLoginState( ps_ftp_server_login_status_e_type next_st );

    /** @fn  MoveDataState
    *   @brief Updates data state and mask
    */
    void MoveDataState( ps_ftp_server_data_state_e_type next_st );

    /** @fn  CleanSocket
    *   @brief Closes socket
    */
    boolean CleanSocket(FTPSocket * mpSock);

    /** @fn  StartCtrlServer
    *   @brief Starts the FTP session with FTP client
    */
    void StartCtrlServer( void );

    /** @fn ProcessLogin()
    *   @brief processes logins cmd
    */
    void ProcessLogin( void );

    /** @fn ProcessCtrlCmd()
    *   @brief processes ctrl cmd
    */
    void ProcessCtrlCmd( void );

    /** @fn  Accept
    *   @brief Accepts incoming connections
    */
    void Accept( boolean is_data );

   /** @fn DataComplete()
   *   @brief Closes data connection upon completion of data cmds rfc 959
   *   section 5.2 connections 
   */
   void DataCloseComplete( );

  };// FTPServerCore

} //PS_FTP

#endif //PS_FTPSERVERCORE_H

