#ifndef __GPR_MPD_QURT_I_H__
#define __GPR_MPD_QURT_I_H__


/*========================================================================*/
/* @file gpr_mpd_qurt_i.h
@brief This file contains gpr calls used by user mpd*/
/*========================================================================
Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
All Rights Reserved.  
Confidential and Proprietary - Qualcomm Technologies, Inc.
====================================================================== */
/*========================================================================
Edit History

when       who        what, where, why
--------   ---       --------------------------------------------------
04/19/18   np        Created file.
========================================================================== */

/******************************************************************************
 * Includes                                                                    *
 *****************************************************************************/
#include "ar_osal_error.h"
#include "ar_osal_mem_op.h"
#include "ar_types.h"
#include "gpr_packet.h"
#include "qurt_signal.h"
#include "qurt_qdi_driver.h"

/*****************************************************************************
 * Defines                                                                   *
 ****************************************************************************/
/**
 * Name of the kernel GPR driver. Client must use this name when opening an
 * instance to the kernel GPR driver.
 */
#define GPR_MPD_DEVICE_NAME ( "qc_gpr" )

/**
 * Driver defined methods
 */
#define GPR_MPD_CMDID_WAIT_RX_SIGNAL     (0+QDI_PRIVATE)
#define GPR_MPD_CMDID_REGISTER          (1+QDI_PRIVATE)
#define GPR_MPD_CMDID_DEREGISTER        (2+QDI_PRIVATE)
#define GPR_MPD_CMDID_ASYNC_SEND        (3+QDI_PRIVATE)
#define GPR_MPD_CMDID_PEEK_RX_PACKET    (4+QDI_PRIVATE)
#define GPR_MPD_CMDID_READ_RX_PACKET    (5+QDI_PRIVATE)
#define GPR_MPD_CMDID_SET_UPD_STATE     (6+QDI_PRIVATE)
#define GPR_MPD_CMDID_KILL_RX_SIGNAL    (7+QDI_PRIVATE)

/**
 * GPR MPD Signals
 */
#define GPR_MPD_SIG_RX_PACKET           0x00000001

/*****************************************************************************
 * Rx Packet Retrieval                                                       *
 ****************************************************************************/

/**
 * Retrieves the header of the Rx packet. This allows the client to allocate
 * the correct buffer size for reading the next Rx packet by calling 
 * __gpr_mpd_cmd_read_rx_packet().
 *
 * \return AR_EOK When successful.
 * \return AR_ENOTREADY When there are no rx packets available. The client
 *                       should not retry until an Rx signal is received.
 */
static inline uint32_t __gpr_mpd_cmd_peek_rx_packet(
   int handle,
   gpr_packet_t *ret_packet_header
)
{

   if (NULL == ret_packet_header)
      return AR_EBADPARAM;

   return qurt_qdi_handle_invoke(handle,
                            GPR_MPD_CMDID_PEEK_RX_PACKET, ret_packet_header);
}

/**
 * Reads the next Rx packet into the client provided buffer.
 *
 * \return AR_EOK when successful.
 * \return AR_ENOTREADY When there are no rx packets available.
 * \notes The client should call this API when an Rx signal is received.
 */
static inline uint32_t __gpr_mpd_cmd_read_rx_packet(
   int handle,
   uint8_t *ret_packet,            /* gpr_packet_t * */
   uint32_t size,
   uint32_t *ret_bytes_read
)
{

   if (NULL == ret_packet || 0 == size || NULL == ret_bytes_read)
      return AR_EBADPARAM;

   return qurt_qdi_handle_invoke(handle,
            GPR_MPD_CMDID_READ_RX_PACKET, ret_packet, size, ret_bytes_read);
}

/*****************************************************************************
 * Sets pd state                                                       *
 ****************************************************************************/

static inline int32_t __gpr_mpd_cmd_set_upd_state(int handle, int addr, int my_domain)
{

   return qurt_qdi_handle_invoke(handle, GPR_MPD_CMDID_SET_UPD_STATE, addr, my_domain);
}


/*****************************************************************************
 * Async Send message                                                        *
 ****************************************************************************/

/**
 * Send packet to KDRV from UDRV. KDRV receives the packet and 
 * routes it based on its routing table. Payload for this 
 * command is of type gpr_packet_t and contains the GPR packet
 * to send to Kernel.  
 *
 * \return AR_EOK when successful.
 * \return AR_EBADPARAM if the input parameter is invalid.
 */
static inline uint32_t __gpr_mpd_cmd_async_send(
   int handle,
   uint8_t *packet,         /* gpr_packet_t * */
   uint32_t size
)
{

   if (NULL == packet || size < sizeof(gpr_packet_t))
      return AR_EBADPARAM;

   return qurt_qdi_handle_invoke(handle,
                   GPR_MPD_CMDID_ASYNC_SEND, packet, size);
}


/*****************************************************************************
 * Rx Signal Setup                                                           *
 ****************************************************************************/

/**
 * Retrieve the rx signal from the kernel GPR driver. Client can then wait on
 * the signal by calling qurt_signal_wait(rx_signal). The signal will be
 * raised by the kernel GPR driver when one or more rx packets are available.
 * The signal is not queued. Upon receiving this signal, the client must retrieve 
 * all the available packets in a loop. When the client is no longer interested 
 * in the rx signal, it must call qdi_close( client_handle ) to release the signal 
 * handle.
 *
 * \return AR_EOK when successful.
 * \return AR_EBADPARAM if the input parameter is invalid.
 */
static inline uint32_t __gpr_mpd_cmd_wait_rx_signal(
   int handle
)
{

   return qurt_qdi_handle_invoke(handle,
                   GPR_MPD_CMDID_WAIT_RX_SIGNAL);
}

static inline uint32_t __gpr_mpd_cmd_kill_rx_signal(
   int handle
)
{
   return qurt_qdi_handle_invoke(handle,
                   GPR_MPD_CMDID_KILL_RX_SIGNAL);
}

#endif /* __GPR_MPD_QURT_I_H__ */
