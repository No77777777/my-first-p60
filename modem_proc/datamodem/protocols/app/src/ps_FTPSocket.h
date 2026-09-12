#ifndef PS_FTPSOCKET_H
#define PS_FTPSOCKET_H
/*=========================================================================*/
/*!
@file
ps_FTPSocket.h

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

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_FTPSocket.h#2 $
$DateTime: 2019/10/22 02:53:17 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/08/14    fn     Created Module   
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE 

===========================================================================*/

#include "comdef.h"

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
  /**-------------------------------------------------------------------------- 
    *    @class FTPServerConfig
    *    @brief PS FTP server configuration utility
  ---------------------------------------------------------------------------*/
  class FTPSocket
  {
    //---------------------------------------------------------------------------
    // Public methods
    //---------------------------------------------------------------------------
  public:
    /** @fn FTPSocket ()
    *   @brief Default constructor
    */
    FTPSocket( dss_net_policy_info_type* net_policy_info,
               sint15 app_id,
               ps_sockaddr_any_type *localaddr,
               boolean data_sock);

    /** @fn  ~FTPSocket()
    *   @brief Default destructor.
    */
    ~FTPSocket();

    /** @fn  BindSocket()
    *   @brief Initialize to default FTP server configurations
    */
    boolean ListenBindSocket( ps_sockaddr_any_type *bind_addr );

    /** @fn  ConnectSocket()
    *   @brief calls dss_connect
    */
    boolean ConnectSocket( );

    /** @fn  AcceptSocket()
    *   @brief calls dss_accept
    */
    boolean AcceptSocket( sint15* accept_sockfd, ps_sockaddr_any_type* accept_addr);

    /** @fn  ReadSocket()
    *   @brief reads avaliable data from socket
    */
    boolean ReadSocket( sint15* read_len);

    /** @fn  ReadBuffer()
    *   @brief returns pointer to socket read buffer
    */
    uint8* ReadBuffer();

    /** @fn  WriteSocket()
    *   @brief writes anything less than sint15 to the socket
    */
    boolean WriteSocket( sint15 write_len);

    /** @fn  WriteFromEFS()
       @brief writes file to EFS
    */ 
    boolean WriteFromEFS ( int efs_file_handle, uint32 efs_file_size,
                           boolean setup_write );

    /** @fn  WriteBuffer()
    *   @brief returns pointer to socket write buffer
    */
    uint8* WriteBuffer();

    /** @fn  WriteBufferSize()
    *   @brief returns size of write buffer
    */
    int WriteBufferSize();

    /** @fn  GetSockFd()
    *   @brief Gets the socket descriptor
    */
    sint15 GetSockFd( );

    /** @fn  RegisterEvt()
    *   @brief registers for async dss sock evts
    */
    boolean RegisterEvt(uint32 mask);

    /** @fn  CloseSocket()
    *   @brief close socket
    */
    boolean CloseSocket();

  protected:
    sint15                        sockfd;
    sint15                        old_server_sockfd;
    boolean                       is_data_sock;
    dss_net_policy_info_type      *net_policy_ptr;
    dss_protocol_family_enum_type sock_family_type;
    uint16                        dest_port; 
    uint16                        src_port; 
    
  private:

    uint8* sock_write_buffer;
    uint8* sock_read_buffer;
    int write_buffer_size;
    int read_buffer_size;
    boolean shutdown;
    int    sock_write_len;
    int    read_bytes_from_efs;
    int    written_bytes;

    /** @fn  CreateSocket()
    *   @brief Initialize to default FTP server configurations
    */
    boolean CreateSocket( 
                     dss_protocol_family_enum_type sock_family,
                     sint15                 app_id);
  };//FTPSocket
} //PS_FTP

#endif // PS_FTPSOCKET_H

