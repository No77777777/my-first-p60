/*===========================================================================
                      I P C    R O U T E R    X A L    S I O 

DESCRIPTION
   This file creates an OS independent implementation of the SIO component
   of Transport Abstraction Layer for the IPC router.

  ---------------------------------------------------------------------------
  Copyright (c) 2010-2013 Qualcomm Technologies, Inc.  All Rights Reserved.
  Qualcomm Technologies Proprietary and Confidential.
  ---------------------------------------------------------------------------
===========================================================================*/

/*===========================================================================

                           EDIT HISTORY FOR FILE

$Header: //components/rel/core.mpss/10.0/mproc/ipc_router/src/ipc_router_xal_sio.c#1 $ $DateTime: 2019/04/24 00:03:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/17/10   ih      Initial revision.

===========================================================================*/


/*===========================================================================
                          INCLUDE FILES
===========================================================================*/
#define FEATURE_DSM_WM_CB
#include "ipc_router_types.h"
#include "ipc_router_protocol.h"
#include "dsm.h"
#include "sio.h"
#include "ipc_router_xal.h"
#include "ipc_router_packet.h"
#include "dsm_init.h"
#include "ipc_router_xal_sio.h"

/*===========================================================================
                  CONSTANT / MACRO DACLARATIONS
===========================================================================*/
#define SIO_LO_WM  200
#define SIO_HI_WM  1500
#define SIO_DNE_WM 0x7FFFFFFF

/*===========================================================================
                        TYPE DECLARATIONS
===========================================================================*/
typedef struct
{
  /* SIO related stuff */
  sio_stream_id_type            stream_id;
  dsm_watermark_type            wm_frm_sio;
  dsm_watermark_type            wm_to_sio;
  q_type                        wm_q_frm;
  q_type                        wm_q_to;
  boolean                       paused;
  void                          *cb_handle;
  boolean                       rx_start;
  dsm_item_type                 *rx_leftover;
  ipc_router_packet_type        *rx_packet;
  uint32                        rx_packet_size;
  ipc_router_xal_sio_param_type param;
  ipc_router_packet_queue_type  tx_queue;
  ipc_router_packet_type        *tx_leftover;
  uint32                        tx_ref;
  ipc_router_os_sem             tx_ref_lock;
}
ipc_router_xal_sio_port_type;

#ifdef IPC_ROUTER_NATIVE_PACKET
/* The basic structure of the native packet */
struct native_packet_s
{
  struct native_packet_s *next;
  uint32                 length;
  void                   *buffer;
};
#endif

/*===========================================================================
  STATIC VARIABLES
  ===========================================================================*/

/*===========================================================================
  INTERNAL FUNCTIONS
  ===========================================================================*/
static dsm_item_type *ipc_router_packet_to_dsm_item
(
  ipc_router_xal_sio_port_type *port, 
  ipc_router_packet_type **pkt
)
{
#ifdef IPC_ROUTER_NATIVE_PACKET
  dsm_item_type *item = NULL;
  struct native_packet_s *prev = NULL, *i = (struct native_packet_s *)*pkt;
  uint32 total_len = 0;

  while(i)
  {
    uint32 this_len = i->length;
    if(port->param.mtu > 0 && (this_len + total_len) > port->param.mtu)
    {
      this_len = port->param.mtu - total_len;
      if(this_len == 0)
      {
        /* We have hit the MTU limit. Break so we return this as the
         * next packet ptr */
        break;
      }
    }

    total_len += this_len;

    if(dsm_pushdown_tail_long(&item, i->buffer, this_len, port->param.pool_id) != this_len)
    {
      /* Avoid trying to figure out how much was converted. Just free and try
       * again later on */
      dsm_free_packet(&item);
      return NULL;
    }

    if(this_len != i->length)
    {
      /* move the buffer forward as they will be considered a 
       * different packet and will be sent subsequently */
      i->buffer = (void *)((uint8 *)i->buffer + this_len);
      i->length -= this_len;
      break;
    }
    prev = i;
    i = i->next;
  }
  if(i != (struct native_packet_s *)*pkt)
  {
    /* If we are not reusing the same packet for the
     * next packet fragment, then free */
    if(prev)
    {
      prev->next = NULL;
    }
    ipc_router_packet_free(pkt);
  }
  *pkt = (ipc_router_packet_type *)i;
  return item;
#else
  if(port->param.mtu > 0)
  {
    dsm_item_type *head = (dsm_item_type *)(*pkt), *tail = NULL;
    if(dsm_split_packet(&head, &tail, port->param.mtu, port->param.pool_id) == 0)
    {
      /* Memory failure */
      return NULL;
    }
    *pkt = (ipc_router_packet_type *)tail;
    return head;
  }
  else
  {
    dsm_item_type *item = *pkt;
    *pkt = NULL;
    return item;
  }
#endif
}

static ipc_router_packet_type *dsm_item_to_ipc_router_packet(dsm_item_type *item)
{
#ifdef IPC_ROUTER_NATIVE_PACKET
  ipc_router_packet_type *pkt = ipc_router_packet_new();
  dsm_item_type *i = item;
  if(!pkt)
  {
    return NULL;
  }
  while(i)
  {
    if(ipc_router_packet_copy_payload(&pkt, i->data_ptr, i->used) != i->used)
    {
      ipc_router_packet_free(&pkt);
      return NULL;
    }
    i = i->pkt_ptr;
  }
  dsm_free_packet(&item);
  return pkt;
#else
  return (ipc_router_packet_type *)item;
#endif
}

static void append_packet(ipc_router_packet_type *_head, ipc_router_packet_type *tail)
{
#ifdef IPC_ROUTER_NATIVE_PACKET
  struct native_packet_s *head = (struct native_packet_s *)_head;

  while(head->next != NULL)
  {
    head = head->next;
  }
  head->next = (struct native_packet_s *)tail;

#else
  dsm_item_type *head = (dsm_item_type *)_head;
  while(head->pkt_ptr != NULL)
  {
    head = head->pkt_ptr;
  }
  head->pkt_ptr = (dsm_item_type *)tail;
#endif
}

/*===========================================================================
  FUNCTION ipc_router_xal_sio_nonempty_cb

  DESCRIPTION
  Process receive queue

  RETURN VALUE
  None

  SIDE EFFECTS
  ===========================================================================*/
static void ipc_router_xal_sio_nonempty_cb
(
 struct dsm_watermark_type_s * wm,
 void *data
 )
{
  ipc_router_xal_sio_port_type *port = (ipc_router_xal_sio_port_type *)data;
  int rc;
  
  if(!port || !port->rx_start)
    return;

  while(NULL != port->rx_leftover || NULL != (port->rx_leftover = dsm_dequeue(&port->wm_frm_sio)))
  {
    ipc_router_packet_type *pkt = dsm_item_to_ipc_router_packet(port->rx_leftover);
    if(!pkt)
    {
      /* TODO Print error? */
      break;
    }

    port->rx_leftover = NULL;

    if(port->rx_packet)
    {
      append_packet(port->rx_packet, pkt);

      if(port->rx_packet_size == 0)
      {
        /* We might have previously received a packet smaller than the header
         * and hence, hoping that the new chunk lets us read the full packet header
         * again */
        (void)ipc_router_protocol_peek_size(port->rx_packet, &port->rx_packet_size);
      }

      if(port->rx_packet_size > 0 && ipc_router_packet_length(port->rx_packet) >= port->rx_packet_size)
      {
        ipc_router_xal_recv(port->rx_packet, port->cb_handle);
        port->rx_packet = NULL;
        port->rx_packet_size = 0;
      }
    }
    else
    {
      port->rx_packet_size = 0;
      port->rx_packet = pkt;
      rc = ipc_router_protocol_peek_size(pkt, &port->rx_packet_size);

      if(rc == IPC_ROUTER_STATUS_SUCCESS && ipc_router_packet_length(port->rx_packet) >= port->rx_packet_size)
      {
        ipc_router_xal_recv(pkt, port->cb_handle);
        port->rx_packet = NULL;
        port->rx_packet_size = 0;
      }
    }
  }
}


/*===========================================================================
  FUNCTION ipc_router_xal_sio_dtr_callback

  DESCRIPTION
  Handle DTR callback from SIO and inform any transport errors through the
  registered ipc_router_xal_err_cb.

  RETURN VALUE
  None

  SIDE EFFECTS
  ===========================================================================*/
static void ipc_router_xal_sio_dtr_callback(void *cb_data)
{
  ipc_router_xal_sio_port_type *port = (ipc_router_xal_sio_port_type *)cb_data;

  if(port)
  {
    sio_ioctl_param_type param;
    boolean dte_asserted = FALSE;

    param.dte_ready_asserted = &dte_asserted;
    port->param.ioctl(port->stream_id, SIO_IOCTL_DTE_READY_ASSERTED, &param);

    if (!dte_asserted)
    {
      if(port->paused == FALSE)
      {
        port->paused = TRUE;
        ipc_router_xal_error(port->cb_handle);
        /* TODO: Local cleanup */
      }
    }
    else 
    {
      if(port->paused == TRUE)
      {
        port->paused = FALSE;
        ipc_router_xal_resume(port->cb_handle);
      }
    }
  }
}

/*===========================================================================
  FUNCTION ipc_router_xal_sio_open

  DESCRIPTION
  Open the specified SIO port.

  RETURN VALUE
  Handle to xport

  SIDE EFFECTS
  ===========================================================================*/
static void *ipc_router_xal_sio_open
(
 void *xport_params,
 void *cb_handle
 )
{
  sio_open_type sio_port;
  sio_ioctl_param_type sio_param;
  ipc_router_xal_sio_port_type *port;
  ipc_router_xal_sio_param_type *param = (ipc_router_xal_sio_param_type *)xport_params;

  if(param->open == NULL || param->close == NULL || 
     param->transmit == NULL || param->ioctl == NULL)
  {
    return NULL;
  }

  port = (ipc_router_xal_sio_port_type *)ipc_router_os_calloc(1, sizeof(*port));
  if (NULL == port)
   return NULL;

  ipc_router_os_mem_set(&sio_port, 0, sizeof(sio_port));

  port->cb_handle = cb_handle;
  port->paused = FALSE;
  port->param = *param;
  ipc_router_packet_queue_init(&port->tx_queue);
  ipc_router_os_sem_init(&port->tx_ref_lock);

  if(port->param.pool_id == 0)
  {
    port->param.pool_id = DSM_DS_SMALL_ITEM_POOL;
  }

  /* Set up the watermark from sio */
  dsm_queue_init( &port->wm_frm_sio,
      SIO_DNE_WM,
      &port->wm_q_frm );
  port->wm_frm_sio.lo_watermark = SIO_LO_WM;
  port->wm_frm_sio.hi_watermark = SIO_HI_WM;
  port->wm_frm_sio.non_empty_func_ptr = ipc_router_xal_sio_nonempty_cb;
  port->wm_frm_sio.non_empty_func_data = port;

  /* Set up the watermark to sio */
  dsm_queue_init( &port->wm_to_sio,
      SIO_DNE_WM,
      &port->wm_q_to );
  port->wm_to_sio.lo_watermark = SIO_LO_WM;
  port->wm_to_sio.hi_watermark = SIO_HI_WM;

  sio_port.stream_mode    = SIO_GENERIC_MODE;
  sio_port.rx_queue       = &port->wm_frm_sio;
  sio_port.tx_queue       = &port->wm_to_sio;
  sio_port.rx_bitrate     = SIO_BITRATE_BEST;
  sio_port.tx_bitrate     = SIO_BITRATE_BEST;
  sio_port.port_id        = port->param.port_id;
  sio_port.tail_char_used = FALSE;
  sio_port.tail_char      = 0;
  sio_port.rx_func_ptr    = NULL;
  sio_port.rx_flow        = SIO_FCTL_BEST;
  sio_port.tx_flow        = SIO_FCTL_BEST;

  
  if(SIO_NO_STREAM_ID == (port->stream_id = port->param.open(&sio_port)))
  {
    IPC_ROUTER_OS_MESSAGE(IPC_ROUTER_MSG_ERR, 
        "ipc_router_xal_sio_open: unable to open port %d\n", 
        (int)sio_port.port_id, 0,0);
    ipc_router_packet_queue_deinit(&port->tx_queue);
    ipc_router_os_sem_deinit(&port->tx_ref_lock);
    ipc_router_os_free(port);
    return NULL;
  }

  port->param.ioctl(port->stream_id, SIO_IOCTL_INBOUND_FLOW_ENABLE, NULL);
  port->param.ioctl(port->stream_id, SIO_IOCTL_CD_ASSERT, NULL);
  port->param.ioctl(port->stream_id, SIO_IOCTL_DSR_ASSERT, NULL);
  port->param.ioctl(port->stream_id, SIO_IOCTL_RI_DEASSERT, NULL);

  sio_param.enable_dte_ready_event_ext.cb_func = ipc_router_xal_sio_dtr_callback;
  sio_param.enable_dte_ready_event_ext.cb_data = port;
  port->param.ioctl(port->stream_id, SIO_IOCTL_ENABLE_DTR_EVENT_EXT, &sio_param);

  return port;
}

/*===========================================================================
  FUNCTION ipc_router_xal_sio_close

  DESCRIPTION
  Close the specified SIO port.

  RETURN VALUE
  IPC_ROUTER_STATUS_SUCCESS
  IPC_ROUTER_STATUS_INVALID_PARAM
  IPC_ROUTER_STATUS_INTERRUPTED
  IPC_ROUTER_STATUS_TIMEOUT

  SIDE EFFECTS
  ===========================================================================*/
static int ipc_router_xal_sio_close
(
 void *handle
 )
{
  ipc_router_xal_sio_port_type *port = (ipc_router_xal_sio_port_type *)handle;
  if(port)
  {
    port->param.close(port->stream_id, NULL);
    /* cheap way of preventing race to nonempty_cb */
    port->rx_start = FALSE;
    ipc_router_xal_closed(port->cb_handle);
    ipc_router_packet_queue_deinit(&port->tx_queue);
    ipc_router_os_sem_deinit(&port->tx_ref_lock);
    ipc_router_os_free(port);
  }
  return IPC_ROUTER_STATUS_SUCCESS;
}

/*===========================================================================
  FUNCTION ipc_router_xal_sio_start_read

  DESCRIPTION
  Start processing rx data for the specified port.

  Once the function is invoked, the registered ipc_router_xal_rx_cb can
  happen at any time, even before this function returns.

  RETURN VALUE
  IPC_ROUTER_STATUS_SUCCESS
  IPC_ROUTER_STATUS_INVALID_PARAM
  IPC_ROUTER_STATUS_INTERRUPTED

  SIDE EFFECTS
  ===========================================================================*/
static int ipc_router_xal_sio_start_read
(
 void *handle
 )
{
  ipc_router_xal_sio_port_type *port = (ipc_router_xal_sio_port_type *)handle;
  if (NULL == port)
    return IPC_ROUTER_STATUS_INVALID_PARAM;

  port->rx_start = TRUE;

  /* Now check to see if there is any data in the Rx watermark */
  ipc_router_xal_sio_nonempty_cb(&port->wm_frm_sio, handle);

  return IPC_ROUTER_STATUS_SUCCESS;
}

/*===========================================================================
  FUNCTION ipc_router_xal_sio_flush

  DESCRIPTION
  Flushes the TX queue onto the transport. 

  SIDE EFFECTS
  ===========================================================================*/
static void ipc_router_xal_sio_flush
(
  ipc_router_xal_sio_port_type *port 
)
{
  dsm_item_type *item;

  ipc_router_os_sem_lock(&port->tx_ref_lock);
  port->tx_ref++;
  if(port->tx_ref > 1)
  {
    /* Allow only one person in this function */
    ipc_router_os_sem_unlock(&port->tx_ref_lock);
    return;
  }

  while(port->tx_ref > 0)
  {
    ipc_router_os_sem_unlock(&port->tx_ref_lock);
    /* Proceed without locks held */
    while(NULL != port->tx_leftover || 
          NULL != (port->tx_leftover = ipc_router_packet_queue_get(&port->tx_queue)))
    {
      item = ipc_router_packet_to_dsm_item(port, &port->tx_leftover);
      if(!item)
      {
        /* Allocation failure. we will try again to flush the TX Queue
         * when we try to send again */
        break;
      }
      port->param.transmit(port->stream_id, item);
    }
    ipc_router_os_sem_lock(&port->tx_ref_lock);
    port->tx_ref--;
  }
  ipc_router_os_sem_unlock(&port->tx_ref_lock);
}

/*===========================================================================
  FUNCTION ipc_router_xal_sio_write

  DESCRIPTION
  Write data to the specified port.

  RETURN VALUE
  IPC_ROUTER_STATUS_SUCCESS
  IPC_ROUTER_STATUS_INVALID_PARAM
  IPC_ROUTER_STATUS_INTERRUPTED

  SIDE EFFECTS
  ===========================================================================*/
static int ipc_router_xal_sio_write
(
 void *handle,
 ipc_router_packet_type *msg
 )
{
  ipc_router_xal_sio_port_type *port = (ipc_router_xal_sio_port_type *)handle;

  if (!port || !msg)
    return IPC_ROUTER_STATUS_INVALID_PARAM;

  ipc_router_packet_queue_put(&port->tx_queue, msg);

  ipc_router_xal_sio_flush(port);

  return IPC_ROUTER_STATUS_SUCCESS;
}

ipc_router_xal_ops_type ipc_router_xal_sio =
{
   ipc_router_xal_sio_open,
   ipc_router_xal_sio_start_read,
   ipc_router_xal_sio_close,
   ipc_router_xal_sio_write,
};

