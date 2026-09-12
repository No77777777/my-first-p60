#ifndef DIAGCOMM_IO_SOCKET_H 
#define DIAGCOMM_IO_SOCKET_H
/*==========================================================================

              Diag I/O Wrapper APIs for Interprocessor Communication and I/O 

General Description
  
Copyright (c) 2015, 2017 by Qualcomm Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

===========================================================================*/

/*===========================================================================

                              Edit History

 $Header: //components/rel/core.mpss/10.0/services/diag/DCM/common/src/diagcomm_io_socket.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/22/17   rs      Replaced ASSERT on socket send failure due to heap exhaustion with retry mechanism
03/06/17   rs      Made srv_addr an array of single object instead of singleton.
04/08/15   ps      Diag over sockets


===========================================================================*/
#if defined(DIAG_SOCKET_SUPPORT)

#include "diagcomm_io.h"         /* For diagcomm_io_conn_type */


#define READ_TASK_PRIORITY 5

/* Macro defines the number of service addresses request */
#define DIAG_NUM_SRV_ADDR_ITEMS 1

int diagcomm_io_socket_init(diagcomm_io_conn_type *ctrl_conn);

int diagcomm_io_socket_open(diagcomm_io_conn_type * conn);

void diagcomm_io_socket_close(diagcomm_io_conn_type * conn);

int diagcomm_io_socket_send(diagcomm_io_conn_type *conn, void *buf, uint32 length );

void * diagcomm_io_socket_create_thread(const char *name, void * main_func, void *arg, int stacksize, int prio);

boolean diagcomm_io_socket_check_support(diagcomm_io_conn_type *conn);

extern void diag_retry_tx_timer_cb(uint32 params);

#endif /* DIAG_SOCKET_SUPPORT */

#endif //DIAGCOMM_IO_SOCKET_H


