/*
Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.

  $Header: //commercial/MPSS.HA.1.1/Main/modem_proc/gpr/datalinks/gpr_csmd/src/winsock.c#1 $
  $Author: mplcsds1 $
*/
#include "internal_csmd.h"
#ifdef WINSOCK_TRANSPORT
#include <stdio.h>
#include "winsock.h"


/***************************************************************************
 * Defines                                                                   *
 ****************************************************************************/
#define WIN_SOCK_VERSION_MAJOR 2
#define WIN_SOCK_VERSION_MINOR 2

static int ref_count = 0;
typedef int ssize_t;
//#pragma comment (lib, "Ws2_32.lib")

#define APRV2_WINSOCK_LOG_LEVEL_DEBUG ( 1 )
#define APRV2_WINSOCK_LOG_LEVEL_INFO  ( 2 )
#define APRV2_WINSOCK_LOG_LEVEL_ERROR ( 3 )

#define APR2_WINSOCK_ERRBUF_SIZE (1024)

#define APRV2_WINSOCK_LOG(level, format, ...) printf("%s: " format,__FUNCTION__,__VA_ARGS__); \
 printf("\n")

static int apr_socket_get_error_code()
{
	int retval;
	int err_code;
	TCHAR err_message[APR2_WINSOCK_ERRBUF_SIZE];	
	/* It will be char (if we're using ascii) or wchar_t (if we're using unicode) */

	err_code = GetLastError();

	retval = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL, err_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		err_message, sizeof(err_message) / sizeof(TCHAR), NULL);

	if (retval == 0)
	{
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR, "Unable to get the exact error message");
		return 0;
	}
	else
	{
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR, "%ls (code %d)", err_message, err_code);
		return err_code;
	}

}

int apr_socket_init(void)
{

	WSADATA wsaData;
	int result;
	if (ref_count == 0)
	{
		result = WSAStartup(MAKEWORD(WIN_SOCK_VERSION_MAJOR,
			WIN_SOCK_VERSION_MINOR), &wsaData);

		if (result != 0)
		{
			APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
				"socket failed with error: %ld", apr_socket_get_error_code());
			return -1;
		}
	}
	ref_count++;
	return 0;
}

int apr_socket_deinit(void)
{
	ref_count--;
	if (ref_count == 0)
		WSACleanup();
	return 0;
}

int apr_socket_get_addrinfo(const char*host, const char*port, struct addrinfo *hints, struct addrinfo **info)
{
	int sock_result;
	sock_result = getaddrinfo(host, port, hints, info);
	if (sock_result != 0)
	{
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"get_addrinfo failed with error: %ld", apr_socket_get_error_code());
		return -1;
	}

	if ((hints->ai_family != PF_INET) && (hints->ai_family != PF_INET6))
	{
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"socket type not supported");
		return -1;
	}

	return 0;
}

SOCKET apr_socket_open(struct addrinfo *info, int server, int no_of_connections)
{
	SOCKET listen_sock;
	SOCKET client_sock;
	int sock_result;
	listen_sock = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (listen_sock == INVALID_SOCKET)
	{
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"socket failed with error: %ld", apr_socket_get_error_code());
		return -1;
	}

	if (server)
	{
		sock_result = bind(listen_sock, info->ai_addr, (int)info->ai_addrlen);
		if (sock_result == INVALID_SOCKET)
		{
			APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
				"bind failed with error: %ld", apr_socket_get_error_code());
			return -1;
		}

		sock_result = listen(listen_sock, no_of_connections);
		if (sock_result == SOCKET_ERROR)
		{
			APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
				"listen failed with error: %ld", apr_socket_get_error_code());
			return -1;
		}

		client_sock = accept(listen_sock, NULL, NULL);
		if (client_sock == INVALID_SOCKET)
		{
			APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
				"accept failed with error: %ld", apr_socket_get_error_code());
			return -1;
		}

		closesocket(listen_sock);
		return client_sock;
	}

	else
	{
		sock_result = connect(listen_sock, info->ai_addr, (int)info->ai_addrlen);
		if (sock_result == INVALID_SOCKET)
		{
			closesocket(listen_sock);
			APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
				"connect failed with error: %ld", apr_socket_get_error_code());
			return -1;
		}
		return listen_sock;
	}
}

int apr_socket_close(SOCKET sock)
{
	if (shutdown(sock, SD_SEND))
	{
		closesocket(sock);
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"shutdown failed with error: %ld", apr_socket_get_error_code());
		return -1;
	}
	closesocket(sock);
	return 0;
}

int apr_socket_recv(SOCKET sock, void *buffer, uint32_t size)
{
	ssize_t read_size;
	if (buffer == NULL || sock == INVALID_SOCKET)
	{
		return -1;
	}

	if (size == 0)
	{
		return 0;
	}

	read_size = recv(sock, (char *) buffer, size, 0);
	if (read_size == -1)
	{
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"recv failed with error: %ld", apr_socket_get_error_code());
		return -1;
	}

	if (read_size == 0)
	{
		//Host Terminated.
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"recv failed with error: The other host terminated the connection");
		return -1;
	}

	//Successfully Recieved
	return read_size;
}

int apr_socket_send(SOCKET sock, void *buffer, uint32_t size)
{
	ssize_t sent_size;
	if (buffer == NULL || sock == INVALID_SOCKET)
	{
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"buffer size or socket is NULL - Bad Parameter");
		return -1;
	}

	if (size == 0)
	{
		return 0;
	}

	sent_size = send(sock, (char *) buffer, size, 0);
	if (sent_size == -1)
	{
		//Some error
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"send failed with error: %ld", apr_socket_get_error_code());
		return -1;
	}

	if (sent_size == 0)
	{
		//Host Terminated.
		APRV2_WINSOCK_LOG(APRV2_WINSOCK_LOG_LEVEL_ERROR,
			"send failed with error: %ld", apr_socket_get_error_code());
		return -1;
	}

	if (sent_size == size)
	{
		//Successfully sent
		return 0;
	}

	return 0;
}
#endif