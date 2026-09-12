/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   Diag I/O Configuration

General Description

Copyright (c) 2012-2018 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                              Edit History

 $Header: //components/rel/core.mpss/10.0/services/diag/platform/bmp/src/diagcomm_cfg.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/27/18   gn      Featurize SIO and DSM code under DIAG_SIO_SUPPORT
06/02/18   nk      Removed blocking on control channel if the packets are not read by apps
08/14/17   sa      Removed data_pending flag as it is not being used.
08/30/16   as      Removed this file's definition of QSOCKET_DIAG_ID,
                     which will be defined in qsocket_ids.h
06/30/16   nk      SMD References Removal
08/25/15   vk      Fixed compilation warnings.
04/08/15   ps      Diag over sockets and dsm removal
11/25/14   ph      Mainlined the feature DIAG_CONSUMER_API.
08/08/13   sg      Command/response separation

===========================================================================*/

#include "diagcomm_io.h"
#include "stdio.h"               /* For snprintf */
#include "assert.h"
#include "qsocket_ids.h"         /* For QSOCKET_DIAG_ID */

/* -------------------------------------------------------------------------
** Definitions and Declarations
** ------------------------------------------------------------------------- */

diagcomm_io_conn_type diagcomm_io_conn[NUM_SMD_PORT_TYPES][NUM_SMD_PORTS];
diagcomm_io_conn_type diagcomm_io_ctrl_conn[NUM_SMD_PORT_TYPES][NUM_SMD_PORTS];
diagcomm_io_conn_type diagcomm_io_cmd_conn[NUM_SMD_PORT_TYPES][NUM_SMD_PORTS];

#define DIAG_READ_BUF_SIZE (8*1024)

#if defined (DIAG_MP) && !defined (DIAG_MP_MASTER)
char diag_ctrl_read_buf[DIAG_READ_BUF_SIZE]; //8k
char diag_cmd_read_buf[DIAG_READ_BUF_SIZE*2]; //16k
#endif

char diag_dci_cmd_read_buf[DIAG_READ_BUF_SIZE];

/*===========================================================================

FUNCTION DIAGCOMM_IO_INIT

DESCRIPTION
  Initilizes Diag variables for interprocess communication and I/O

PARAMETERS
  None

RETURN VALUE
  None

===========================================================================*/
void diagcomm_io_init( void )
{
#if defined(DIAG_MP) || defined(DIAG_SOCKET_SUPPORT)
  int ret;
#endif

  uint8 i, j = 0;

  for( i=0; i < NUM_SMD_PORT_TYPES; i++ )
  {
    for( j=0; j < NUM_SMD_PORTS; j++ )
    {
      (void)memset( &diagcomm_io_conn[i][j], 0, sizeof(diagcomm_io_conn_type) );
      (void)memset( &diagcomm_io_ctrl_conn[i][j], 0, sizeof(diagcomm_io_conn_type) );
      (void)memset( &diagcomm_io_cmd_conn[i][j], 0, sizeof(diagcomm_io_conn_type) );

      diagcomm_io_conn[i][j].port_num = j;
      diagcomm_io_conn[i][j].connected = FALSE;
      diagcomm_io_conn[i][j].channel_connected = FALSE;
      diagcomm_io_conn[i][j].io_type = DIAGCOMM_IO_TYPE_INVALID;
      diagcomm_io_conn[i][j].channel_type = DIAGCOMM_IO_DATA;
      diagcomm_io_conn[DIAGCOMM_PORT_SMD][j].port_type = DIAGCOMM_PORT_SMD;
      diagcomm_io_conn[DIAGCOMM_PORT_SIO][j].port_type = DIAGCOMM_PORT_SIO;
      diagcomm_io_conn[DIAGCOMM_PORT_SIO][j].allow_flow = TRUE;


      diagcomm_io_ctrl_conn[i][j].port_num = j;
      diagcomm_io_ctrl_conn[i][j].connected = FALSE;
      diagcomm_io_ctrl_conn[i][j].channel_connected = FALSE;
      diagcomm_io_ctrl_conn[i][j].io_type = DIAGCOMM_IO_TYPE_INVALID;
      diagcomm_io_ctrl_conn[i][j].channel_type = DIAGCOMM_IO_CTRL;
      diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SMD][j].port_type = DIAGCOMM_PORT_SMD;
      diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][j].port_type = DIAGCOMM_PORT_SIO;
      diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][j].allow_flow = TRUE;

      diagcomm_io_cmd_conn[i][j].port_num = j;
      diagcomm_io_cmd_conn[i][j].connected = FALSE;
      diagcomm_io_cmd_conn[i][j].channel_connected = FALSE;
      diagcomm_io_cmd_conn[i][j].io_type = DIAGCOMM_IO_TYPE_INVALID;
      diagcomm_io_cmd_conn[i][j].channel_type = DIAGCOMM_IO_CMD;
      diagcomm_io_cmd_conn[DIAGCOMM_PORT_SMD][j].port_type = DIAGCOMM_PORT_SMD;
      diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][j].port_type = DIAGCOMM_PORT_SIO;
      diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][j].allow_flow = TRUE;

    }
  }


#if defined(DIAG_MP)
#if defined(DIAG_SOCKET_SUPPORT)
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].isServer = FALSE;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.sa_family = AF_IPC_ROUTER;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addrtype = IPCR_ADDR_NAME;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addr.port_name.instance= DIAG_SOCKET_INST_ID_DATA;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addr.port_name.service = QSOCKET_DIAG_ID;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].send_flags = QMSG_DONTWAIT;
//No incoming data over the data channel.
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf_length = 0;

//CTRL Channel
  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].isServer = FALSE;
  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.sa_family = AF_IPC_ROUTER;
  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addrtype = IPCR_ADDR_NAME;
  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addr.port_name.instance= DIAG_SOCKET_INST_ID_CTRL;
  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addr.port_name.service = QSOCKET_DIAG_ID;
  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].send_flags = QMSG_DONTWAIT;

  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf = &diag_ctrl_read_buf;
  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf_length = sizeof(diag_ctrl_read_buf);
  diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf_mutex.name = "MUTEX_DIAG_CTRL_BUF";
  ret = osal_init_mutex(&diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf_mutex);
  ASSERT(OSAL_SUCCESS == ret);

  //CMD Channel
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].isServer = TRUE;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.sa_family = AF_IPC_ROUTER;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addrtype = IPCR_ADDR_NAME;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addr.port_name.instance= DIAG_SOCKET_INST_ID_CMD;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].addr.address.addr.port_name.service = QSOCKET_DIAG_ID;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].send_flags = QMSG_DONTWAIT;
#endif

  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf = &diag_cmd_read_buf;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf_length = sizeof(diag_cmd_read_buf);
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf_mutex.name = "MUTEX_DIAG_CMD_BUF";
  ret = osal_init_mutex(&diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].buf_mutex);
  ASSERT(OSAL_SUCCESS == ret);



#if defined(DIAG_SOCKET_SUPPORT)
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].isServer = TRUE;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].addr.sa_family = AF_IPC_ROUTER;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].addr.address.addrtype = IPCR_ADDR_NAME;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].addr.address.addr.port_name.instance= DIAG_SOCKET_INST_ID_DCI_CMD;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].addr.address.addr.port_name.service = QSOCKET_DIAG_ID;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].send_flags = QMSG_DONTWAIT;
#endif

  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].buf = &diag_dci_cmd_read_buf;
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].buf_length = sizeof(diag_dci_cmd_read_buf);
  diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].buf_mutex.name = "MUTEX_DIAG_DCI_CMD_BUF";
  ret = osal_init_mutex(&diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].buf_mutex);
  ASSERT(OSAL_SUCCESS == ret);


#else
  #ifdef DIAG_SIO_SUPPORT
  //Always use SIO to USB/UART
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].io_type = DIAGCOMM_IO_TYPE_SIO;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].handle.sio_handle = SIO_NO_STREAM_ID;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].open_params.sio_params.params.port_id = SIO_PORT_NULL;
  #endif /* DIAG_SIO_SUPPORT */
#endif


#if defined(DIAG_SOCKET_SUPPORT)
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].isServer = FALSE;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].addr.sa_family = AF_IPC_ROUTER;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].addr.address.addrtype = IPCR_ADDR_NAME;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].addr.address.addr.port_name.instance= DIAG_SOCKET_INST_ID_DCI_DATA;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].addr.address.addr.port_name.service = QSOCKET_DIAG_ID;
  diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_2].send_flags = QMSG_DONTWAIT;
#endif

} /* diagcomm_io_init */

/*===========================================================================

FUNCTION DIAGCOMM_UPDATE_IO_TYPE

DESCRIPTION
  Changes the connection type to the one specified in change_to_type.

PARAMETERS
  None

RETURN VALUE
  None

===========================================================================*/
void diagcomm_update_io_type(void)
{
#if defined (DIAG_MP) && !defined (DIAG_MP_MASTER)
   diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].io_type = diagcomm_io_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].change_to_type;
   diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].io_type = diagcomm_io_ctrl_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].change_to_type;
   diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].io_type = diagcomm_io_cmd_conn[DIAGCOMM_PORT_SIO][DIAGCOMM_PORT_1].change_to_type;
#endif
}

 /* diagcomm_update_io_type */
