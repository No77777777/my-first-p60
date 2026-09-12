#ifndef __GPR_MPD_KERNEL_I_H__
#define __GPR_MPD_KERNEL_I_H__

/*========================================================================*/
/**
@file gpr_mpd_kernel_i.h
@brief This file contains the mpd function definitions and defines if compiled in kernel
*/
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
#include "gpr_packet.h"

/******************************************************************************
 * Defines                                                                    *
 *****************************************************************************/
/* Name of the kernel GPR driver. Client must use this name when opening an
 * instance to the kernel GPR driver */
#define GPR_MPD_DEVICE_NAME ( "qc_gpr" )

/* Driver defined methods */
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
#define GPR_MPD_SIG_EXIT                0x00000002

typedef uint32_t ( *gpr_mpd_cb_fn_t) (uint32_t ev, void* payload, uint32_t sz);

GPR_INTERNAL uint32_t gpr_mpd_init ( void );
GPR_INTERNAL uint32_t gpr_mpd_deinit ( uint32_t domain_id );
GPR_INTERNAL uint32_t gpr_pd_restart_call(void);
GPR_INTERNAL uint32_t gpr_pdr_register(void);
GPR_INTERNAL void    gpr_pdr_workerthread(void *param);
GPR_INTERNAL uint32_t gpr_pd_service_monitor(void *param);
GPR_INTERNAL uint32_t gpr_pdr_deinit ( void);
GPR_INTERNAL uint32_t gpr_pdr_init ( void);


/* Send a packet to he dst address specified in the packet. In case
  of UGPR the packet is sent to Kernel. In-case of KGPR the packet gets
  sent to the correct destination based on dst ID. 
 
  @param packet gpr packet to send */
GPR_INTERNAL uint32_t gpr_mpd_send( uint32_t domain_id, void* packet, uint32_t packet_len );


/* Register callback with mpd service. This call-back will get invoked
  for every received message.
 
  @param pfn_rx_cb callback function */
GPR_INTERNAL uint32_t gpr_mpd_set_rx_cb(gpr_mpd_cb_fn_t pfn_rx_cb);

#endif /* __GPR_MPD_KERNEL_I_H__ */

