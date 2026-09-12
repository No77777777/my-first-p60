/*=========================================================================*/
/*!
@file
ps_FTPSocket.cpp

@brief
Provides helper routines for mock TCP application off-target test execution (QTF)

Copyright (c) 2014-21 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/
/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/data.mpss/3.5..2.1/protocols/app/src/ps_FTPSocket.cpp#3 $
$DateTime: 2021/02/12 03:50:40 $$Author: pwbldsvc $
 
when        who    what, where, why
--------    ---    ----------------------------------------------------------
02/12/21    rk     Compiler Version Upgrade to 8.5.01 fixes
06/06/18    fn     Added filters for connected port 
02/08/14    fn     Created Module  
===========================================================================*/

/*===========================================================================

INCLUDE FILES FOR MODULE

===========================================================================*/

#include <stringl/stringl.h>
#include "comdef.h"

#include "ps_FTPSocket.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ps_system_heap.h"
#include "ds_Utils_DebugMsg.h"
#include "dssocket.h"
#include "dssinternal.h"
#include "dss_config.h"
#include "fs_public.h"
#include "ps_app_logging_util.h"

#ifdef __cplusplus
}
#endif  // extern "C"

using namespace PS_FTP;

/*===========================================================================

DEFINITIONS

===========================================================================*/
#define FTP_SERVER_DEFAULT_DATA_BUF_SIZE (50*1024)
#define FTP_SERVER_DEFAULT_CTRL_BUF_SIZE (256)
#define FTP_SOCK_MAX_SINGLE_WRITE (32767)

/*===========================================================================
 
PRIVATE METHODS
 
===========================================================================*/

/**-------------------------------------------------------------------------- 
*   @fn dss_ftp_reg_ip_filter 
*   @brief register for ip filters
---------------------------------------------------------------------------*/
void dss_ftp_reg_ip_filter 
(
  dss_iface_id_type   iface_id,
  sint15              sockfd,
  dss_protocol_family_enum_type  ip_family,
  uint16              ftp_src_port,
  uint16              ftp_dest_port
)
{
  ip_filter_type     filter;
  sint15             dss_errno = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Register a filter for WWAN technolgieis for FTP packets to route properly
  -------------------------------------------------------------------------*/
  memset( &filter, 0, sizeof(ip_filter_type) );

  if ( DSS_PF_INET == ip_family )
  {
    /* set up the incoming filter */

    filter.ip_vsn  = IP_V4;
    filter.ip_hdr.v4.field_mask = 
           (ipfltr_ip4_hdr_field_mask_type)IPFLTR_MASK_IP4_NEXT_HDR_PROT;
    filter.ip_hdr.v4.next_hdr_prot = (uint8)PS_IPPROTO_TCP;

  }
  else if ( DSS_PF_INET6 == ip_family )
  {
    filter.ip_vsn  = IP_V6;
    filter.ip_hdr.v6.field_mask = 
           (ipfltr_ip6_hdr_field_mask_type)IPFLTR_MASK_IP6_NEXT_HDR_PROT;
    filter.ip_hdr.v6.next_hdr_prot = (uint8)PS_IPPROTO_TCP;
  }
  else
  {
    PS_APP_ERR_LOG_1(ip_family);
    return;
  }

  if ( ftp_dest_port != 0 )
  {
    filter.next_prot_hdr.tcp.field_mask  |= 
           (ipfltr_tcp_hdr_field_mask_type)IPFLTR_MASK_TCP_DST_PORT;
    filter.next_prot_hdr.tcp.dst.port = ftp_dest_port;
  }

  if ( ftp_src_port != 0 )
  {
    filter.next_prot_hdr.tcp.field_mask  |= 
           (ipfltr_tcp_hdr_field_mask_type)IPFLTR_MASK_TCP_SRC_PORT;
    filter.next_prot_hdr.tcp.src.port = ftp_src_port;
  }

  PS_APP_DEBUG_3("Registering FTP IP TCP filter sock=%d, iface id %d ip_family %d",
                  sockfd,
                  iface_id,
                  ip_family); 
  PS_APP_DEBUG_3("TCP destination to modem src port = %d dest port = %d filter mask 0x%x",
                  ps_ntohs(filter.next_prot_hdr.tcp.src.port),
                  ps_ntohs(filter.next_prot_hdr.tcp.dst.port),
                  filter.next_prot_hdr.tcp.field_mask); 

  if ( 0 !=
       dss_reg_ip_filter ( sockfd,
                           iface_id,
                           IPFLTR_DEFAULT_TYPE,
                           1,
                           (void *)&filter,
                           &dss_errno ) )
  {
    /*Unable to add filter for FTP*/
    PS_APP_ERR_LOG_0();
    return;
  }

} /*dss_ftp_reg_ip_filter*/

/**-------------------------------------------------------------------------- 
*   @fn dss_ftp_async_select
*   @brief register for sockfd events
---------------------------------------------------------------------------*/
boolean dss_ftp_async_select
(
  sint15 sockfd,
  sint31 event_mask
)
{
  sint31 dss_ret;
  sint15 dss_errno;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dss_ret = dss_async_select( sockfd, event_mask, &dss_errno );
  if ( DSS_SUCCESS != dss_ret )
  {
    PS_APP_ERR_LOG_3(event_mask, sockfd, dss_errno);
    return FALSE;
  }
  return TRUE;
} /*dss_ftp_async_select*/

/*===========================================================================

PUBLIC METHODS

===========================================================================*/
/**-------------------------------------------------------------------------- 
*   @fn FTPSocket()
*   @brief Standard constructor
---------------------------------------------------------------------------*/
FTPSocket::FTPSocket
(
  dss_net_policy_info_type* net_policy_info,
  sint15 app_id,
  ps_sockaddr_any_type *localaddr,
  boolean data_sock
)
: sockfd(-1),
  is_data_sock(data_sock),
  net_policy_ptr(net_policy_info),
  sock_write_buffer(NULL),
  sock_read_buffer(NULL),
  write_buffer_size(0),
  read_buffer_size(0),
  shutdown(FALSE)
{

  if ( localaddr->type != DSS_PF_INET &&
       localaddr->type != DSS_PF_INET6 )
  {
    PS_APP_ERR_LOG_2(localaddr->type, net_policy_info->family);
    localaddr->type = (dss_protocol_family_enum_type) net_policy_info->family;
  }

  if ( localaddr->type == DSS_PF_INET6 )
  {
    sock_family_type = DSS_PF_INET6;
    if(FALSE == CreateSocket(DSS_PF_INET6, app_id))
    {
      delete this;
      return;
    }
  }
  else
  {
    sock_family_type = DSS_PF_INET;
    if(FALSE == CreateSocket(DSS_PF_INET, app_id))
    {
      delete this;
      return;
    }
  }

  if ( TRUE == data_sock )
  {
    write_buffer_size = FTP_SERVER_DEFAULT_DATA_BUF_SIZE;
    read_buffer_size = FTP_SERVER_DEFAULT_DATA_BUF_SIZE;
  }
  else
  {
    write_buffer_size = FTP_SERVER_DEFAULT_CTRL_BUF_SIZE;
    read_buffer_size = FTP_SERVER_DEFAULT_CTRL_BUF_SIZE;
  }

  PS_APP_DEBUG_2("Allocating socket read buf len %d, write buf len %d", 
                  read_buffer_size, write_buffer_size);
  sock_read_buffer = (uint8*)ps_system_heap_mem_alloc(read_buffer_size);
  sock_write_buffer = (uint8*) ps_system_heap_mem_alloc(write_buffer_size);

  if ( NULL == sock_read_buffer || NULL == sock_write_buffer )
  {
    PS_APP_ERR_LOG_2(sock_read_buffer, sock_write_buffer);
    delete this;
    return;
  }
 
  memset(sock_read_buffer,0, read_buffer_size);
  memset(sock_write_buffer,0, write_buffer_size);

}

/**-------------------------------------------------------------------------- 
*   @fn  ~FTPSocket()
*   @brief Default destructor.
---------------------------------------------------------------------------*/
FTPSocket::~FTPSocket()
{
  if ( NULL != sock_read_buffer )
  {
    PS_SYSTEM_HEAP_MEM_FREE(sock_read_buffer);
  }
  if ( NULL != sock_write_buffer )
  {
    PS_SYSTEM_HEAP_MEM_FREE(sock_write_buffer);
  }
}

/**-------------------------------------------------------------------------- 
*   @fn  CreateSocket()
*   @brief Initialize to default FTP server configurations
---------------------------------------------------------------------------*/
boolean FTPSocket::CreateSocket
( 
  dss_protocol_family_enum_type sock_family,
  sint15                        app_id
                      
)
{
  sint15 dss_ret, dss_errno;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( NULL == net_policy_ptr )
  {
    PS_APP_ERR_LOG_0();
    return FALSE;
  }

  PS_APP_DEBUG_2("CreateSocket appID %d, family %d", 
                  app_id, sock_family);

  dss_ret = dss_socket(app_id, 
                      (byte)sock_family, 
                      DSS_SOCK_STREAM, 
                      PS_IPPROTO_TCP, 
                      &dss_errno);

  if ( dss_ret > 0 )
  {
    sockfd = dss_ret;
    return TRUE;
  }
  else
  {
    PS_APP_ERR_LOG_2(dss_ret, dss_errno);
  }

  return FALSE;
}

/**-------------------------------------------------------------------------- 
*   @fn  ListenSocket()
*   @brief Initialize to default FTP server configurations
---------------------------------------------------------------------------*/
boolean FTPSocket::ListenBindSocket
( 
  ps_sockaddr_any_type       *localaddr
)
{
  sint15 dss_ret = DSS_SUCCESS, dss_errno;
  ip_addr_type interface_ip_addr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( sockfd <= 0 || NULL == localaddr)
  {
    PS_APP_ERR_LOG_2(sockfd, localaddr);
    return FALSE;
  }

  if( localaddr->type != sock_family_type )
  {
    PS_APP_ERR_LOG_2(localaddr->type, sock_family_type);
    localaddr->type = sock_family_type;
  }

  /*-------------------------------------------------------------------------
   Determine if we want to bind on v4 or v6 socket
  -------------------------------------------------------------------------*/
  if ( localaddr->type == DSS_PF_INET)
  {
    localaddr->sockaddr.v4.ps_sin_family = (uint16)localaddr->type;

    PS_APP_DEBUG_2("Binding v4 socket %d on input port 0x%x",
                     sockfd, localaddr->sockaddr.v4.ps_sin_port);

    IPV4_ADDR_MSG(localaddr->sockaddr.v4.ps_sin_addr.ps_s_addr);
    dss_ret = dss_bind( sockfd,
                       (struct ps_sockaddr *)&localaddr->sockaddr.v4,
                       sizeof(struct ps_sockaddr_in),
                       &dss_errno);

    if ( DSS_SUCCESS != dss_ret )
    {
      PS_APP_DEBUG_2("Failed to bind v4 sock dss_ret %d, errno %i",
                      dss_ret, dss_errno);
      return FALSE;
    }

    PS_APP_DEBUG_2("Successfully Binded v4 socket %d on result port 0x%x",
                     sockfd,
                     localaddr->sockaddr.v4.ps_sin_port);
    IPV4_ADDR_MSG(localaddr->sockaddr.v4.ps_sin_addr.ps_s_addr);

    /*-------------------------------------------------------------------------
     Register a filter on this port
    -------------------------------------------------------------------------*/
    dss_ftp_reg_ip_filter(net_policy_ptr->iface.info.id,
                         sockfd,
                         localaddr->type,
                         0, /*any src port*/
                         localaddr->sockaddr.v4.ps_sin_port );

    dest_port = localaddr->sockaddr.v6.ps_sin6_port;
    src_port = 0;
  }
  else if (  localaddr->type == DSS_PF_INET6 )
  {
    localaddr->sockaddr.v6.ps_sin6_family = localaddr->type;

    /* Get and fill in the scope id */
    localaddr->sockaddr.v6.ps_sin6_scope_id = 
      dss_get_scope_id_by_iface_id(net_policy_ptr->iface.info.id, 
                                   &dss_errno);

    PS_APP_DEBUG_3("Setting scope id %i for ipv6 iface id %i dss_errno = %i",
                   localaddr->sockaddr.v6.ps_sin6_scope_id,
                   net_policy_ptr->iface.info.id,
                   dss_errno);

    PS_APP_DEBUG_2("Binding v6 socket %d on port 0x%x",
                   sockfd,
                   localaddr->sockaddr.v6.ps_sin6_port);

    IPV6_ADDR_MSG(localaddr->sockaddr.v6.ps_sin6_addr.in6_u.u6_addr64);

    dss_ret = dss_bind( sockfd,
                       (struct ps_sockaddr *)&localaddr->sockaddr.v6,
                       sizeof(struct ps_sockaddr_in6),
                       &dss_errno);

    PS_APP_DEBUG_2("FTP Server Binded v6 socket %d on port 0x%x",
                   sockfd,
                   localaddr->sockaddr.v6.ps_sin6_port);

    IPV6_ADDR_MSG(localaddr->sockaddr.v6.ps_sin6_addr.in6_u.u6_addr64);

    if ( DSS_SUCCESS != dss_ret )
    {
      PS_APP_ERR_DEBUG_2("Failed to bind v6 sock dss_ret %d, errno %i",
                      dss_ret, dss_errno);
      return FALSE;
    }
    /*-------------------------------------------------------------------------
     Register a filter on this port
    -------------------------------------------------------------------------*/
    dss_ftp_reg_ip_filter(net_policy_ptr->iface.info.id,
                        sockfd,
                        localaddr->type,
                        0, /*any src port*/
                        localaddr->sockaddr.v6.ps_sin6_port);

    dest_port = localaddr->sockaddr.v6.ps_sin6_port;
    src_port = 0;

  }
  else
  {
    PS_APP_ERR_LOG_1(localaddr->type);
  }

  /*-------------------------------------------------------------------------
   Start bind socket to listen for TCP connection
      DS_EWOULDBLOCK should not happen for listen!
  -------------------------------------------------------------------------*/
  dss_ret = dss_listen(sockfd, DSS_SOMAXCONN, &dss_errno);

  if ( DSS_SUCCESS != dss_ret || (FALSE == RegisterEvt(DS_ACCEPT_EVENT)))
  {
    PS_APP_ERR_DEBUG_2("Dss Listen fail dss_ret %i, errno %i or unable to "
                    "resigster event",
                    dss_ret, dss_errno);
    return FALSE;
  }

  /*-------------------------------------------------------------------------
   Check user sock info and fill in corresponding information
  -------------------------------------------------------------------------*/
  dss_ret = dss_iface_ioctl( 
     net_policy_ptr->iface.info.id,
     DSS_IFACE_IOCTL_GET_IP_ADDR,
     (void *) &interface_ip_addr,
     &dss_errno);

  if ( dss_ret != DSS_SUCCESS )
  {
     PS_APP_ERR_DEBUG_2("Dss get ip addr fail dss_ret %i, errno %i",
                       dss_ret, dss_errno);
    return FALSE;
  }

  if ( interface_ip_addr.type == IPV4_ADDR )
  {
    localaddr->type = DSS_PF_INET;
    memscpy(&localaddr->sockaddr.v4.ps_sin_addr.ps_s_addr,
            sizeof(localaddr->sockaddr.v4.ps_sin_addr.ps_s_addr),
            &interface_ip_addr.addr.v4,
            sizeof(interface_ip_addr.addr.v4));
  }
  else if ( interface_ip_addr.type == IPV6_ADDR )
  {
    localaddr->type = DSS_PF_INET6;
    memscpy(&localaddr->sockaddr.v6.ps_sin6_addr.in6_u.u6_addr64,
            sizeof(uint64)+sizeof(uint64),
            &interface_ip_addr.addr.v6,
            sizeof(uint64)+sizeof(uint64));
  }

  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn  ConnectSocket()
*   @brief Initialize to default FTP server configurations
---------------------------------------------------------------------------*/
boolean FTPSocket::ConnectSocket( )
{
  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn  AcceptSocket()
*   @brief calls dss_accept
--------------------------------------------------------------------------*/
boolean FTPSocket::AcceptSocket
( 
   sint15* accept_sockfd, 
   ps_sockaddr_any_type* accept_addr 
)
{
  struct ps_sockaddr *connecting_addr = NULL;
  ps_sockaddr_any_type dummy_sockaddr;
  sint15 dss_ret, dss_errno;
  uint16 addrlen;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( NULL == accept_sockfd || sockfd != *accept_sockfd )
  {
    PS_APP_ERR_DEBUG_1("Invalid inputs accept sockfd 0x%p",
                       accept_sockfd);
    return FALSE;
  }

  if( NULL != accept_addr && accept_addr->type != sock_family_type )
  {
    PS_APP_ERR_DEBUG_2("Invalid inputs accept_addr->type %d family_type %d",
                       accept_addr->type, sock_family_type);
    return FALSE;
  }

  memset(&dummy_sockaddr, 0, sizeof(ps_sockaddr_any_type));

  /*-------------------------------------------------------------------------
    Accept TCP connection
  -------------------------------------------------------------------------*/
  if ( DSS_AF_INET == sock_family_type )
  {
    if ( NULL == accept_addr )
    {
      dummy_sockaddr.sockaddr.v4.ps_sin_family = DSS_AF_INET;
      connecting_addr = (struct ps_sockaddr *)&(dummy_sockaddr.sockaddr.v4);
    }
    else
    {
      connecting_addr = (struct ps_sockaddr *)&(accept_addr->sockaddr.v4); 
    }
    addrlen = sizeof( struct ps_sockaddr_in);
  }
  else
  {
    if ( NULL == accept_addr )
    {
      dummy_sockaddr.sockaddr.v6.ps_sin6_family = DSS_AF_INET6;
      connecting_addr = (struct ps_sockaddr *)&(dummy_sockaddr.sockaddr.v6);
    }
    else
    {
      connecting_addr = (struct ps_sockaddr *)&(accept_addr->sockaddr.v6);
    }
    addrlen = sizeof(struct ps_sockaddr_in6);
  }

  /*FN Todo, add print statement for connecting addr*/

  dss_ret = dss_accept(sockfd, connecting_addr, &addrlen, &dss_errno );
  /*-------------------------------------------------------------------------
    In the case of dss_ret > 0 a new socket will be created for this
    TCP connection
  -------------------------------------------------------------------------*/
  if ( dss_ret > 0 )
  {
    PS_APP_DEBUG_3("Accepted connection new sockfd = %i, "
                   "remoteaddr family = %d, old sockfd %i",
                   dss_ret, connecting_addr->ps_sa_family , sockfd );

    *accept_sockfd = dss_ret;
    old_server_sockfd = sockfd;
    sockfd = dss_ret;

    /*-------------------------------------------------------------------------
     Register a filter on this port
    -------------------------------------------------------------------------*/
    PS_APP_DEBUG_3("Register filter on accepted sockd , "
                   "src port %d, dest_port %d iface_id 0x%x",
                   src_port, dest_port, net_policy_ptr->iface.info.id );

    dss_ftp_reg_ip_filter(net_policy_ptr->iface.info.id,
                        sockfd,
                        sock_family_type,
                        src_port, 
                        dest_port);

    dss_ret = dss_close(old_server_sockfd, &dss_errno);
    if (DSS_ERROR == dss_ret && DS_EWOULDBLOCK == dss_errno)
    {
      dss_ftp_async_select(old_server_sockfd, DS_CLOSE_EVENT);
    }

    if( FALSE == RegisterEvt(DS_WRITE_EVENT | DS_READ_EVENT | DS_CLOSE_EVENT))
    {
      return FALSE;
    }
    return TRUE;
  }

  PS_APP_ERR_DEBUG_2("Unable to accept connection sockfd %i, errno = %i",
                     dss_ret, dss_errno);

  return TRUE;
}

/** @fn  ReadSocket()
*   @brief reads avaliable data from socket
*/
boolean FTPSocket::ReadSocket
( 
  sint15* read_len
)
{
  sint15 dss_ret, dss_errno;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( NULL == read_len || NULL == sock_read_buffer )
  {
    PS_APP_ERR_LOG_3(read_len, sock_read_buffer, sockfd);
    return FALSE;
  }

  memset(sock_read_buffer, 0, read_buffer_size);
  dss_ret = dss_read(sockfd, 
                     sock_read_buffer,
                     read_buffer_size,
                     &dss_errno );

  PS_APP_DEBUG_2("ReadSocket: Dss_read dss_ret %i dss_errno %i",dss_ret,dss_errno);

  if (DSS_ERROR == dss_ret && DS_EWOULDBLOCK == dss_errno)
  {
    RegisterEvt( DS_READ_EVENT | DS_CLOSE_EVENT);
    *read_len = dss_ret;
    return FALSE;
  }
  else if ( dss_ret > 0 )
  {
    RegisterEvt( DS_READ_EVENT | DS_CLOSE_EVENT );

    *read_len = dss_ret;
    return TRUE;
  }

  PS_APP_ERR_DEBUG_3( "Read fail sockfd %d, ret = %i, errno = %i", 
                   sockfd, dss_ret, dss_errno);
  return FALSE;
}

/** @fn  WriteSocket()
*   @brief writes anything less than sint15 to the socket
*/
boolean FTPSocket::WriteSocket 
(
  sint15 write_len
)
{
  sint15 dss_ret, dss_errno;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( NULL == sock_write_buffer || write_len > write_buffer_size ) 
  {
    PS_APP_ERR_LOG_3(sock_write_buffer,write_buffer_size, write_len );
    return FALSE;
  }

  dss_ret = dss_write(sockfd,
            sock_write_buffer,
            write_len,
            &dss_errno);


  if ( dss_ret == DSS_ERROR && dss_errno == DS_EWOULDBLOCK )
  {
    PS_APP_DEBUG_2("WriteSocket: dss_write dss_ret %i  errno = %i",
              dss_ret, dss_errno);
    return RegisterEvt(DS_WRITE_EVENT | DS_CLOSE_EVENT);
  }
  else if ( dss_ret == write_len )
  {
     PS_APP_DEBUG_1("WriteSocket: all bytes written dss_ret = %i",dss_ret);
    memset(sock_write_buffer, 0, write_len+1);
    return RegisterEvt( DS_WRITE_EVENT | DS_READ_EVENT | DS_CLOSE_EVENT );
  }

  PS_APP_ERR_DEBUG_2("WriteSocket:  dss_ret %i dss_errno %i",
            dss_ret, dss_errno);
  return FALSE;
}

/** @fn  WriteFromEFS()
*   @brief writes file to EFS
*/
boolean FTPSocket::WriteFromEFS
(
  int efs_file_handle,
  uint32 efs_file_size,
  boolean setup_write
)
{
  sint15 dss_ret = DSS_SUCCESS, dss_errno = DS_ENOERR;
  int write_bytes;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  PS_APP_ERR_DEBUG_1("WriteFromEFS, %d", setup_write);

  if ( NULL == sock_write_buffer || sockfd <= 0 ) 
  {
    PS_APP_ERR_LOG_3(sock_write_buffer,write_buffer_size, sockfd );
    return FALSE;
  }

  if ( TRUE == setup_write )
  {
    if ( efs_file_size > FTP_SOCK_MAX_SINGLE_WRITE )
    {
      sock_write_len = FTP_SOCK_MAX_SINGLE_WRITE;
    }
    else
    {
      sock_write_len = efs_file_size;
    }

    read_bytes_from_efs = 0;
    written_bytes = 0;
  }

  if ( (read_bytes_from_efs < efs_file_size) &&
       ((read_bytes_from_efs - written_bytes) < write_buffer_size))
  {
    read_bytes_from_efs += efs_read(efs_file_handle,
                                &sock_write_buffer[(read_bytes_from_efs % write_buffer_size)],
                                sock_write_len);
    PS_APP_ERR_DEBUG_2("WriteSocket: total read_bytes %d, buf idx %d", read_bytes_from_efs,
                       (read_bytes_from_efs % write_buffer_size) );
  }

  if ( read_bytes_from_efs > written_bytes  )
  {
    PS_APP_ERR_DEBUG_2("WriteSocket: total read_bytes %d, buf idx %d", read_bytes_from_efs,
                       (read_bytes_from_efs % write_buffer_size) );

    write_bytes = (read_bytes_from_efs - written_bytes) > FTP_SOCK_MAX_SINGLE_WRITE ? 
      FTP_SOCK_MAX_SINGLE_WRITE : (read_bytes_from_efs - written_bytes);
    dss_ret = dss_write( sockfd, 
                         &sock_write_buffer[written_bytes],
                         write_bytes,
                         &dss_errno);
  }

  if ( dss_ret == DSS_ERROR && dss_errno == DS_EWOULDBLOCK )
  {
    PS_APP_ERR_DEBUG_2("WriteSocket: dss_write dss_ret %i  errno = %i",
                       dss_ret, dss_errno);
    if ( FALSE == RegisterEvt(DS_WRITE_EVENT | DS_CLOSE_EVENT))
    {
      return FALSE;
    }
  }
  else if ( dss_ret > 0 )
  {
    written_bytes += dss_ret;
    PS_APP_ERR_DEBUG_3("WriteSocket: bytes written dss_ret = %i total written = %i total read %i",
                       dss_ret, written_bytes, read_bytes_from_efs);

    if ( written_bytes >= efs_file_size )
    {
      return TRUE;
    }
    if( FALSE == RegisterEvt( DS_WRITE_EVENT | DS_CLOSE_EVENT ))
    {
      return FALSE;
    }
  }

  PS_APP_ERR_LOG_2(dss_ret, dss_errno);
  return FALSE;
}

/**-------------------------------------------------------------------------- 
*   @fn  ReadBuffer()
*   @brief returns pointer to socket read buffer
---------------------------------------------------------------------------*/
uint8* FTPSocket::ReadBuffer()
{
  return sock_read_buffer;
}

/**-------------------------------------------------------------------------- 
*   @fn  WriteBuffer()
*   @brief returns pointer to socket write buffer
---------------------------------------------------------------------------*/
uint8* FTPSocket::WriteBuffer()
{
  return sock_write_buffer;
}

/**-------------------------------------------------------------------------- 
*   @fn  WriteBufferSize()
*   @brief returns size of write buffer
---------------------------------------------------------------------------*/
int FTPSocket::WriteBufferSize()
{
  return write_buffer_size;
}

/**-------------------------------------------------------------------------- 
*   @fn  GetSockFd()
*   @brief Gets the socket descriptor
---------------------------------------------------------------------------*/
sint15 FTPSocket::GetSockFd()
{
  return sockfd;
}

/**-------------------------------------------------------------------------- 
*   @fn  RegisterEvt()
*   @brief async register for events 
---------------------------------------------------------------------------*/
boolean FTPSocket::RegisterEvt(uint32 mask)
{
  if( FALSE == dss_ftp_async_select(sockfd, mask) )
  {
    return FALSE;
  }
  return TRUE;
}

/**-------------------------------------------------------------------------- 
*   @fn  CloseSocket()
*   @brief close socket
--------------------------------------------------------------------------*/
boolean FTPSocket::CloseSocket()
{
  sint15 dss_errno = 0, dss_ret = DSS_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
    Call dss_shutdown to shut down connected sockets
    if DS_ENOTCONN 114 is return then the socket can be closed
  -------------------------------------------------------------------------*/
  if ( FALSE == shutdown )
  {
    dss_ret = dss_shutdown(sockfd, DSS_SHUT_RDWR, &dss_errno);

    PS_APP_ERR_DEBUG_3("dss_shutdown sockfd=%i dss_errno = %i",
                       sockfd, dss_errno, dss_ret);

    if ( dss_ret != DSS_SUCCESS )
    {
      PS_APP_ERR_LOG_3(sockfd, dss_errno, dss_ret);
    }

    shutdown = TRUE;
  }

  /*-------------------------------------------------------------------------
    Now call dss_close to see if we can free the resource
  -------------------------------------------------------------------------*/
  dss_ret = dss_close(sockfd, &dss_errno);

  if ( dss_ret == DSS_SUCCESS )
  {
    PS_APP_DEBUG_1("Socket closed successfully %i", sockfd);
    sockfd = -1;
    shutdown = FALSE;
    return TRUE;
  }
  else if ( DSS_ERROR == dss_ret && DS_EWOULDBLOCK == dss_errno  )
  {
    if( FALSE == RegisterEvt( DS_CLOSE_EVENT))
    {
      return FALSE;
    }
    else
    {
      /*FN todo should async deselect other events*/
      PS_APP_DEBUG_1("Socket closing waiting for CLOSE event %i", sockfd);
    }
  }
  else
  {
    PS_APP_ERR_LOG_2(dss_errno, sockfd);
  }

  return FALSE;
}
