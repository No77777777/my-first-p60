#ifndef PS_FTPSERVER_H
#define PS_FTPSERVER_H
/*=========================================================================*/
/*!
@file
ps_FTPServer.h

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

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/inc/ps_FTPServer.h#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module   
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE 

===========================================================================*/

#include "comdef.h"

/*===========================================================================

INCLUDE FILES FOR MODULE (extern C includes)

===========================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#include "ps_ftp_server_def.h"

#ifdef __cplusplus
}
#endif  // extern "C"

/*===========================================================================

EXPORTED MACROS

===========================================================================*/


/*===========================================================================

EXPORTED FUNCTIONS

===========================================================================*/
namespace PS_FTP
{
  typedef void (*ps_ftp_server_event_cback_fn_type)
  (
    uint32  event_mask,
    void*   ftp_server,
    void*   ftp_data,
    uint16  ftp_data_len
  );

  class FTPServerCore;

  /**-------------------------------------------------------------------------- 
    *                       @class FTPServer
    *                       @brief PS FTP server application
  ---------------------------------------------------------------------------*/
  class FTPServer
  {

  public:

    /** @fn FTPServer()
    *   @brief Default constructor
    */
    FTPServer();

    /** @fn FTPServer()
    *   @brief constructor to include inst
    */
    FTPServer(ps_ftp_server_inst_type inst);


    /** @fn  ~FTPServer()
    *   @brief Default destructor.
    */
    virtual ~FTPServer ();

    /** @fn virtual boolean Init()
    *   @brief Initialization function.
    */
    virtual boolean Init();

    /** @fn virtual boolean Configure()
    *   @brief Configure function.
    *  
    *   @param [in] net_policy_input valid net policy for FTP server
    *   @param [in] ftp_server_config_input FTP server
    *          configurations
    */
    virtual boolean Configure( dss_net_policy_info_type* net_policy_input, 
                               ps_ftp_server_config_type* ftp_server_config_input,
                               dss_ftp_server_event_cback_fn_type reg_ftp_server_evt_cback,
                               uint32 reg_ftp_server_event_mask);

    /** @fn virtual boolean Start()
    *   @brief Start the FTP server.
    */
    virtual boolean Start( ps_sockaddr_any_type* local_sockaddr);

    /** @fn virtual boolean Start()
    *   @brief Stop the FTP server
    */
    virtual boolean Stop();

    /** @fn virtual boolean Abort()
    *   @brief Stop the FTP server
    */
    virtual boolean Abort();

    /** @fn virtual boolean DenyConnection()
    *   @brief Deny a connection
    */
    virtual boolean DenyConnection();

    /** @fn virtual boolean AcceptConnection()
    *   @brief  Accept a connection
    */
    virtual boolean AcceptConnection();

    /** @fn virtual sint15 getServerAppId()
    *   @brief Gives the app id value
    */
    virtual sint15 getServerAppId ();

    /** @fn virtual void EventNotifyClient()
    *   @brief Use to notify the client of the FTP events
    */
    void EventNotifyClient(uint32 event_mask, void* ftp_userdata, uint16 ftpdata_len);

    /** @fn  ProcessSockEvt ()
    *   @brief Calls into FTP Server Core to process socket evt
    */
    void ProcessSockEvt(uint32 event_mask,sint15 evt_sockfd);

    //---------------------------------------------------------------------------
    // Protected methods
    //---------------------------------------------------------------------------
  protected:

    // register notify callback
    virtual boolean RegisterNotifyCallback();

    //---------------------------------------------------------------------------
    // Protected data fields
    //---------------------------------------------------------------------------
  protected:
   
    /** @var  dss_net_policy_info_type* net_policy_ptr
    *   @brief  Stores net policy information for FTP server 
    */
    dss_net_policy_info_type* net_policy_ptr;

    /** @var  ps_ftp_server_config_type* ftp_server_config
    *   @brief  Stores FTP server configurations 
    */
    ps_ftp_server_config_type* ftp_server_config;

    /** @var  dss_ftp_server_event_cback_fn_type
    *         ftp_server_event_cback
    *   @brief  FTP Server event callback
    */
    dss_ftp_server_event_cback_fn_type ftp_server_evt_cback;

    /** @var  uint32 ftp_server_event_mask
    *   @brief  Mask for registering for FTP server Event callbacks
    */
    uint32 ftp_server_evt_mask;

    /** @var  FTPServerConnection* mpFtpConnection
    *   @brief  Stores FTP connection class information
    */
    FTPServerCore* mpFTPServerCore;

    /** @var   sint15 FTPServerAppId
    *   @brief  dss application id 
    */
    sint15 FTPServerAppId;

  private:
    uint32 ftp_server_state;
    ps_ftp_server_inst_type ftp_server_inst;
  }; //FTPServer
} // PS_FTP

#endif // PS_FTPSERVER_H

