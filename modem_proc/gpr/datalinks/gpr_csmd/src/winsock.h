/*========================================================================
  This file contains AFE MMPM related apis
  
  Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
  All Rights Reserved.  
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  $Header: //commercial/MPSS.HA.1.1/Main/modem_proc/gpr/datalinks/gpr_csmd/src/winsock.h#1 $
 ====================================================================== */
#ifndef _WINSOCK_H_
#define _WINSOCK_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#include "ar_osal_types.h"
#include <winsock2.h>
#include <ws2tcpip.h>

/***************************************************************************
 * Defines                                                                   *
 ****************************************************************************/
#define APR_SOCKOPEN_SERVER (1)
#define APR_SOCKOPEN_CLIENT (0)

int apr_socket_init(void);
int apr_socket_deinit(void);
int apr_socket_get_addrinfo(const char*host, const char*port,  struct addrinfo *hints, struct addrinfo **info);
SOCKET apr_socket_open(struct addrinfo *info, int server, int no_of_connections);
int apr_socket_close(SOCKET sock);
int apr_socket_recv(SOCKET sock, void *buffer, uint32_t size);
int apr_socket_send(SOCKET sock, void *buffer, uint32_t size);




#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* _WINSOCK_H_ */