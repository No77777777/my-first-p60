/*========================================================================
*/ /** @file gpr_drv.c
This file contains the main function implementations of gpr driver

 Copyright (c) 2018-2021 Qualcomm Technologies, Inc.
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
#include "ar_osal_log.h"

/*****************************************************************************
 * Defines                                                                   *
 ****************************************************************************/

#define GPR_NUM_PACKET_POOLS 3

#define GPR_NUM_PACKETS_1 (100)
#define GPR_DRV_BYTES_PER_PACKET_1 (512)
#define GPR_NUM_PACKETS_2 (4)
#define GPR_DRV_BYTES_PER_PACKET_2 (4096)
#define GPR_NUM_ISLAND_PACKETS_3 (4)
#define GPR_DRV_BYTES_PER_ISLAND_PACKET_3 (128)

#if defined USES_GPDSP0_IMG
  #define HOST_DOMAIN_ID GPR_IDS_DOMAIN_ID_GDSP0_V
#elif USES_GPDSP1_IMG
  #define HOST_DOMAIN_ID GPR_IDS_DOMAIN_ID_GDSP1_V
#endif
   
/*****************************************************************************
 * Global variables                                                          *
 ****************************************************************************/
/* GPR IPC table containing init,deinit functions for datalink layers depending on
domains a given src domain wishes to establish a link with.
For user pd, ADSP will communicate with local modules through local init/deinit,
and with other domains through the mpd user layer  */
#ifdef CSMD_COSIM
static struct ipc_dl_v2_t gpr_user_ipc_dl_v2_table[GPR_PL_NUM_TOTAL_DOMAINS_V] = {
   { GPR_IDS_DOMAIN_ID_MODEM_V, ipc_dl_csmd_init, ipc_dl_csmd_deinit, TRUE },
   { GPR_IDS_DOMAIN_ID_GDSP0_V, ipc_dl_local_init, ipc_dl_local_deinit, TRUE },
   { GPR_IDS_DOMAIN_ID_APPS_V, ipc_dl_csmd_init, ipc_dl_csmd_deinit, TRUE },
};
#else
static struct ipc_dl_v2_t gpr_user_ipc_dl_v2_table[GPR_PL_NUM_TOTAL_DOMAINS_V] =
   { 
     { GPR_IDS_DOMAIN_ID_GDSP0_V, ipc_dl_local_init, ipc_dl_local_deinit, TRUE },
     { GPR_IDS_DOMAIN_ID_APPS_V, ipc_dl_mpd_user_init, ipc_dl_mpd_user_deinit, TRUE },
   };
#endif
/*****************************************************************************
 * Local function definitions                                                *
 ****************************************************************************/
GPR_INTERNAL uint32_t gpr_drv_init(void)
{
   AR_MSG(DBG_HIGH_PRIO, "GPR INIT START");
   uint32_t rc;
   uint32_t num_domains = sizeof(gpr_user_ipc_dl_v2_table) / sizeof(ipc_dl_v2_t);

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

   packet_pool_info[2].heap_index  = GPR_HEAP_INDEX_1;
   packet_pool_info[2].is_dynamic  = FALSE;
   packet_pool_info[2].num_packets = GPR_NUM_ISLAND_PACKETS_3;
   packet_pool_info[2].packet_size = GPR_DRV_BYTES_PER_ISLAND_PACKET_3;
   packet_pool_info[2].reserved    = 0;

   rc = gpr_drv_internal_init_v2(HOST_DOMAIN_ID, // host_domain_id
                                 num_domains,
                                 gpr_user_ipc_dl_v2_table,
                                 GPR_NUM_PACKET_POOLS,
                                 &packet_pool_info[0]);

   if (rc)
   {
      printf("Could not initialize the GPR infrastructure and datalink layers");
   }
   return rc;
}
