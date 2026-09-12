#ifndef _GPR_QSOCKET_H_
#define _GPR_QSOCKET_H_
/*=============================================================================
Copyright (c) 2019-2020 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

===============================================================================
                                 Edit History

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //source/qcom/qct/multimedia2/gpr/gpr2/mainline/main/latest/datalink/gpr_glink/inc/gpr_glink.h
  $Author: mplcsds1 $

when         who   what, where, why
----------   ---   ------------------------------------------------------------
4/11/18    np      Created
=============================================================================*/

/******************************************************************************
 * Includes                                                                   *
 *****************************************************************************/
#include "qsocket.h"
#include "ipc_dl_api.h"
#include "gpr_packet.h"

#define GPR_CMDID_HACK_INIT_IPC (0x12345678)

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/** Data receive notification callback type*/
typedef uint32_t (*gpr_qsocket_receive_cb)(void *   ptr, /* pointer to the received buffer */
                                           uint32_t length);

/** Data send done notification callback type*/
typedef uint32_t (*gpr_qsocket_send_done_cb)(void *   ptr, /* pointer to the received buffer */
                                             uint32_t length);

typedef struct gpr_to_qsocket_cb_fn_t
{
   gpr_qsocket_receive_cb   rx_cb;      /* Receive Data CB */
   gpr_qsocket_send_done_cb tx_done_cb; /* Send done Data CB */
} gpr_to_qsocket_cb_fn_t;

/******************************************************************************
 * Defines                                                                    *
 *****************************************************************************/
/*IPC datalink init function called from gpr layer for glink*/
GPR_INTERNAL uint32_t ipc_dl_qsocket_init(uint32_t                 src_domain_id,
                                          uint32_t                 dest_domain_id,
                                          const gpr_to_ipc_vtbl_t *p_gpr_to_ipc_vtbl,
                                          ipc_to_gpr_vtbl_t **     pp_ipc_to_gpr_vtbl);

/*IPC datalink de-init function called from gpr layer for glink*/
GPR_INTERNAL uint32_t ipc_dl_qsocket_deinit(uint32_t src_domain_id, uint32_t dest_domain_id);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* _GPR_GLINK_H_ */
