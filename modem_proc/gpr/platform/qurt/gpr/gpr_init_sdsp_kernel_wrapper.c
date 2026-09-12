/*========================================================================
*/ /** @file gpr_drv.c
This file contains the main function implementations of gpr driver

 Copyright (c) 2018-2019 Qualcomm Technologies, Inc.
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
#include "DDIPlatformInfo.h"

/*****************************************************************************
 * Defines                                                         *
 ****************************************************************************/
#define GPR_NUM_PACKETS_1 ( 100 )
#define GPR_DRV_BYTES_PER_PACKET_1 ( 512 )
#define GPR_NUM_PACKETS_2 ( 4 )
#define GPR_DRV_BYTES_PER_PACKET_2 ( 4096 )

#if defined(SIM) && defined (SATELLITE)
#define HOST_DOMAIN_ID GPR_IDS_DOMAIN_ID_MODEM_V
#else
#define HOST_DOMAIN_ID GPR_IDS_DOMAIN_ID_SDSP_V
#endif

/*****************************************************************************
 * Global variables                                                          *
 ****************************************************************************/
/* Local GPR IPC table containing init,deinit functions and vtbl function pointer to be populated for all domains*/
#if defined(SIM)
static struct ipc_dl_t gpr_kernel_ipc_dl_table[GPR_PL_NUM_TOTAL_DOMAINS_V]= {
		 {GPR_IDS_DOMAIN_ID_MODEM_V,ipc_dl_local_init,ipc_dl_local_deinit},
		 {GPR_IDS_DOMAIN_ID_ADSP_V,ipc_dl_mpd_kernel_init,ipc_dl_mpd_kernel_deinit},
		 {GPR_IDS_DOMAIN_ID_APPS_V,ipc_dl_local_init, ipc_dl_local_deinit},
};
#else
static struct ipc_dl_t gpr_kernel_ipc_dl_table[GPR_PL_NUM_TOTAL_DOMAINS_V]= {
		 {GPR_IDS_DOMAIN_ID_MODEM_V,ipc_dl_glink_init,ipc_dl_glink_deinit},
		 {GPR_IDS_DOMAIN_ID_SDSP_V,ipc_dl_mpd_kernel_init,ipc_dl_mpd_kernel_deinit},
		 {GPR_IDS_DOMAIN_ID_APPS_V,ipc_dl_glink_init, ipc_dl_glink_deinit},
		 {GPR_IDS_DOMAIN_ID_ADSP_V,ipc_dl_glink_init, ipc_dl_glink_deinit},
};
#endif





	
/*****************************************************************************
 * Local function definitions                                                *
 ****************************************************************************/
GPR_INTERNAL uint32_t gpr_drv_init(void)
{
   AR_MSG(DBG_HIGH_PRIO, "GPR INIT START");
   uint32_t rc;
   
   
   uint32_t num_domains = sizeof(gpr_kernel_ipc_dl_table) / sizeof(ipc_dl_t);
   rc                   = gpr_drv_internal_init(gpr_kernel_ipc_dl_table,
                              num_domains,
                              HOST_DOMAIN_ID,
                              GPR_NUM_PACKETS_1,
                              GPR_DRV_BYTES_PER_PACKET_1,
                              GPR_NUM_PACKETS_2,
                              GPR_DRV_BYTES_PER_PACKET_2);
   return rc;
}