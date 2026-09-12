/*========================================================================
*/ /** @file gpr_drv.c
This file contains the main function implementations of gpr driver

 Copyright (c) 2018-2020 Qualcomm Technologies, Inc.
 All Rights Reserved.  
 Confidential and Proprietary - Qualcomm Technologies, Inc.
*/ /*====================================================================== */

/*========================================================================
Edit History

when       who    what, where, why
--------  -----     -------------------------------------------------------
04/11/18   np      Created file.
========================================================================== */

/******************************************************************************
 * Includes                                                                    *
 *****************************************************************************/
#include "ar_msg.h"
#include "gpr_api_i.h"
#include "gpr_csmd.h"
#include "gpr_mpd.h"
#include "gpr_glink.h"
#include "gpr_qsocket.h"
#include "ar_osal_log.h"
#include "ar_osal_mem_op.h"
#include "DDIPlatformInfo.h"

/*****************************************************************************
 * Defines                                                         *
 ****************************************************************************/
#define GPR_NUM_PACKET_POOLS (2)
#define GPR_NUM_PACKETS_1 (100)
#define GPR_DRV_BYTES_PER_PACKET_1 (512)
#define GPR_NUM_PACKETS_2 (4)
#define GPR_DRV_BYTES_PER_PACKET_2 (4096)

#if defined(SIM) && defined(SATELLITE)
#define HOST_DOMAIN_ID GPR_IDS_DOMAIN_ID_MODEM_V
#else
#define HOST_DOMAIN_ID GPR_IDS_DOMAIN_ID_ADSP_V
#endif

/*****************************************************************************
 * Global variables                                                          *
 ****************************************************************************/
/* Local GPR IPC table containing init,deinit functions and vtbl function pointer to be populated for all domains*/
#if defined(SIM)
static struct ipc_dl_v2_t gpr_kernel_ipc_dl_v2_table[GPR_PL_NUM_TOTAL_DOMAINS_V] = {
   { GPR_IDS_DOMAIN_ID_MODEM_V, ipc_dl_local_init, ipc_dl_local_deinit, TRUE },
   { GPR_IDS_DOMAIN_ID_ADSP_V, ipc_dl_mpd_kernel_init, ipc_dl_mpd_kernel_deinit, TRUE },
   { GPR_IDS_DOMAIN_ID_APPS_V, ipc_dl_local_init, ipc_dl_local_deinit, TRUE },
};
#else
static struct ipc_dl_v2_t gpr_kernel_ipc_dl_v2_table[GPR_PL_NUM_TOTAL_DOMAINS_V] = {
   { GPR_IDS_DOMAIN_ID_MODEM_V, ipc_dl_glink_init, ipc_dl_glink_deinit, TRUE },
   { GPR_IDS_DOMAIN_ID_ADSP_V, ipc_dl_mpd_kernel_init, ipc_dl_mpd_kernel_deinit, TRUE },
   { GPR_IDS_DOMAIN_ID_APPS_V, ipc_dl_glink_init, ipc_dl_glink_deinit, TRUE },
   { GPR_IDS_DOMAIN_ID_SDSP_V, ipc_dl_glink_init, ipc_dl_glink_deinit, TRUE },
};
#endif

/*****************************************************************************
 * Local function definitions                                                *
 ****************************************************************************/
GPR_INTERNAL uint32_t gpr_drv_init(void)
{
   AR_MSG(DBG_HIGH_PRIO, "GPR INIT START");
   uint32_t rc;

// If this is a fusion , over write the routing table..
#ifndef SIM
   if (DalPlatformInfo_IsFusion())
   {
      static struct ipc_dl_v2_t gpr_kernel_ipc_dl_v2_table_qsock[GPR_PL_NUM_TOTAL_DOMAINS_V] = {
         { GPR_IDS_DOMAIN_ID_MODEM_V, ipc_dl_qsocket_init, ipc_dl_qsocket_deinit, TRUE },
         { GPR_IDS_DOMAIN_ID_ADSP_V, ipc_dl_mpd_kernel_init, ipc_dl_mpd_kernel_deinit, TRUE },
         { GPR_IDS_DOMAIN_ID_APPS_V, ipc_dl_glink_init, ipc_dl_glink_deinit, TRUE },
      };
      ar_mem_cpy(&gpr_kernel_ipc_dl_v2_table,
                 sizeof(gpr_kernel_ipc_dl_v2_table),
                 &gpr_kernel_ipc_dl_v2_table_qsock,
                 sizeof(gpr_kernel_ipc_dl_v2_table_qsock));
   }
#endif

   gpr_packet_pool_info_v2_t packet_pool_info[GPR_NUM_PACKET_POOLS];

   // init pool info
   packet_pool_info[0].heap_index  = GPR_HEAP_INDEX_DEFAULT;
   packet_pool_info[0].is_dynamic  = FALSE;
   packet_pool_info[0].num_packets = GPR_NUM_PACKETS_1;
   packet_pool_info[0].packet_size = GPR_DRV_BYTES_PER_PACKET_1;
   packet_pool_info[0].reserved    = 0;

   packet_pool_info[1].heap_index  = GPR_HEAP_INDEX_DEFAULT;
   packet_pool_info[1].is_dynamic  = FALSE;
   packet_pool_info[1].num_packets = GPR_NUM_PACKETS_2;
   packet_pool_info[1].packet_size = GPR_DRV_BYTES_PER_PACKET_2;
   packet_pool_info[1].reserved    = 0;

   uint32_t num_domains = sizeof(gpr_kernel_ipc_dl_v2_table) / sizeof(ipc_dl_v2_t);
   rc                   = gpr_drv_internal_init_v2(HOST_DOMAIN_ID,
                                 num_domains,
                                 gpr_kernel_ipc_dl_v2_table,
                                 GPR_NUM_PACKET_POOLS,
                                 &packet_pool_info[0]);
   return rc;
}
