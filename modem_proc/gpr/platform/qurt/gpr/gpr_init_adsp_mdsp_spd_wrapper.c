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
#include "ar_osal_log.h"
#include "gpr_qsocket.h"
#include "DDIPlatformInfo.h"


/*****************************************************************************
 * Defines                                                         *
 ****************************************************************************/
#define GPR_NUM_PACKETS_1 ( 100 )
#define GPR_DRV_BYTES_PER_PACKET_1 ( 512 )
#define GPR_NUM_PACKETS_2 ( 4 )
#define GPR_DRV_BYTES_PER_PACKET_2 ( 4096 )

/*****************************************************************************
 * Global variables                                                          *
 ****************************************************************************/
/* GPR IPC table containing init,deinit functions for datalink layers depending on 
domains a given src domain wishes to establish a link with.
For single pd, ADSP will communicate with local modules through local init/deinit, 
and with other domains through the respective datalink layers  */

#ifdef SIM
static struct ipc_dl_t gpr_spd_ipc_dl_table[GPR_PL_NUM_TOTAL_DOMAINS_V]= {
		 {GPR_IDS_DOMAIN_ID_MODEM_V,ipc_dl_local_init, ipc_dl_local_deinit},
		 {GPR_IDS_DOMAIN_ID_ADSP_V,ipc_dl_local_init, ipc_dl_local_deinit},
		 {GPR_IDS_DOMAIN_ID_APPS_V,ipc_dl_local_init,ipc_dl_local_deinit},
};

#else

static struct ipc_dl_t gpr_spd_ipc_dl_table[GPR_PL_NUM_TOTAL_DOMAINS_V]= {
		 {GPR_IDS_DOMAIN_ID_MODEM_V,ipc_dl_local_init,ipc_dl_local_deinit},
		 {GPR_IDS_DOMAIN_ID_ADSP_V,ipc_dl_local_init,ipc_dl_local_deinit},
		 {GPR_IDS_DOMAIN_ID_APPS_V,ipc_dl_glink_init, ipc_dl_glink_deinit},
};

#endif //SIM

/*****************************************************************************
 * Local function definitions                                                *
 ****************************************************************************/
GPR_INTERNAL uint32_t gpr_drv_init(void)
{
   AR_MSG(DBG_HIGH_PRIO, "GPR INIT START");
   uint32_t  rc;
   uint32_t num_domains = sizeof(gpr_spd_ipc_dl_table)/sizeof(ipc_dl_t);
   
      //If this is a fusion , over write the routing table..
   #ifndef SIM
   if ( DalPlatformInfo_IsFusion (  ) )
   {
	     static struct ipc_dl_t gpr_spd_ipc_dl_table_qsock[GPR_PL_NUM_TOTAL_DOMAINS_V]= {
		 {GPR_IDS_DOMAIN_ID_MODEM_V,ipc_dl_local_init,ipc_dl_local_deinit},
		 {GPR_IDS_DOMAIN_ID_ADSP_V,ipc_dl_qsocket_init,ipc_dl_qsocket_deinit},
		 {GPR_IDS_DOMAIN_ID_APPS_V,ipc_dl_glink_init, ipc_dl_glink_deinit},
         };
		 memcpy( &gpr_spd_ipc_dl_table, &gpr_spd_ipc_dl_table_qsock, sizeof(gpr_spd_ipc_dl_table_qsock));

		 
   }
   #endif
   
   rc = gpr_drv_internal_init(gpr_spd_ipc_dl_table,
                              num_domains,
							  GPR_IDS_DOMAIN_ID_MODEM_V,
                              GPR_NUM_PACKETS_1,
                              GPR_DRV_BYTES_PER_PACKET_1,
                              GPR_NUM_PACKETS_2,
                              GPR_DRV_BYTES_PER_PACKET_2);
   return rc;
}
