#ifndef IPC_ROUTER_XAL_SIO_H
#define IPC_ROUTER_XAL_SIO_H
/*===========================================================================
                      I P C    R O U T E R    X A L    S I O

DESCRIPTION
   This file specifies the interface of an OS independent implementation of
   the SIO component of Transport Abstraction Layer for the IPC router.

  ---------------------------------------------------------------------------
  Copyright (c) 2010-2011, 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  ---------------------------------------------------------------------------
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/mproc/ipc_router/src/ipc_router_xal_sio.h#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------

===========================================================================*/


/*===========================================================================
                          INCLUDE FILES
===========================================================================*/
#include "sio.h"
#include "ipc_router_xal.h"

/*===========================================================================
                        EXPORTED FUNCTION PROTOTYPES
===========================================================================*/
extern ipc_router_xal_ops_type ipc_router_xal_sio;
typedef struct
{
  dsm_mempool_id_type pool_id;
  sio_port_id_type port_id;
  unsigned int mtu;
  sio_stream_id_type (*open)(sio_open_type *);
  void (*close)(sio_stream_id_type, void (*)(void));
  void (*transmit)(sio_stream_id_type, dsm_item_type *);
  void (*ioctl)(sio_stream_id_type, sio_ioctl_cmd_type, sio_ioctl_param_type *);
} ipc_router_xal_sio_param_type;

/* Helper macro to create a  Bulk Data SIO interface */
#define IPC_ROUTER_XAL_SIO_DATA_PARAMS(PORT, POOL, MTU) { \
  POOL, PORT, MTU, \
  sio_open, sio_close, \
  sio_transmit, sio_ioctl \
}

/* Helper macro to create a Control SIO interface */
#define IPC_ROUTER_XAL_SIO_CONTROL_PARAMS(PORT, POOL, MTU) { \
  POOL, PORT, MTU, \
  sio_control_open, sio_control_close, \
  sio_control_transmit, sio_control_ioctl \
}

#endif /* IPC_ROUTER_XAL_SIO_H */
