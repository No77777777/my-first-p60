#ifndef __GPR_MPD_USER_I_H__
#define __GPR_MPD_USER_I_H__


/*========================================================================*/
/*@file gpr_mpd_user_i.h
@brief This file contains the function definitions for user mpd */
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
#include "gpr_comdef.h"
#include "gpr_packet.h"

/******************************************************************************
 * Defines                                                                    *
 *****************************************************************************/
typedef uint32_t ( *gpr_mpd_cb_fn_t) (uint32_t ev, void* payload, uint32_t sz);

/* gpr callback function*/
GPR_INTERNAL uint32_t gpr_ipc_mpd_cb(uint32_t event_id, void* buf, uint32_t length);

/* mpd user init function */
GPR_INTERNAL uint32_t gpr_mpd_init ( void );

/* mpd user deinit function */
GPR_INTERNAL uint32_t gpr_mpd_deinit ( uint32_t domain_id );

/*
  Send a packet to he dst address specified in the packet. In case
  of UGPR the packet is sent to Kernel. In-case of KGPR the packet gets
  sent to the correct destination based on dst ID. 
 
  @param packet gpr packet to send
*/
GPR_INTERNAL uint32_t gpr_mpd_send( uint32_t domain_id, void* packet, uint32_t packet_len );


/*
  Register callback with mpd service. This call-back will get invoked
  for every received message.
 
  @param pfn_rx_cb callback function
*/
GPR_INTERNAL uint32_t gpr_mpd_set_rx_cb(gpr_mpd_cb_fn_t pfn_rx_cb);

#endif /* __GPR_MPD_USER_I_H__ */

