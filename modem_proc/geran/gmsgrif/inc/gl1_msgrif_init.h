#ifndef GL1_MSGRIF_INIT_H
#define GL1_MSGRIF_INIT_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  GL1 MSGR Interface Initialisation
                       Header File

GENERAL DESCRIPTION
   This header file contains declarations relevant to L1 task processing.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmsgrif/inc/gl1_msgrif_init.h#2 $ $DateTime: 2020/07/28 11:01:41 $ $Author: pwbldsvc $

when       who      what, where, why
--------   -------- --------------------------------------------- 
24/07/20   sal      CR2736787 Added G2X cleanup done handler to issue rf_meas_exit immediately on cleanup confirmation.
29/10/17   km       CR2133917 GL1 support for L+L
20/08/15   npt      CR878988 GL1 NV control of GFW recovery features
25/06/15   sp       CR856664: Register G2W IRAT messenger handlers on GSM Start mode request by using correct AS ID
01/06/15   pjr      CR846441 Removed constant NUM_GERAN_GFW_ROUTER_MSGS
15/04/15   pa       CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
29/04/15   npt      CR804870 RF timeline error recovery mechanism
27/04/15   ws       CR815011 Featurise cfcm_cpu_monitor handling uder FEATURE_DUAL_DATA
09/04/15   jj       CR819956 Thor 2.x RF API change, G2W use MSGR
16/03/15   pg       CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
27/02/15   am       CR646584 WLAN coex
21/01/15   jj       CR 784462 remove FEATURE_THOR_G2X_MSGR_REWORK 
17/10/14   jj       CR741205 moving  G2X CNF from l1 task to MSGR 
26/09/14   pg       CR729410 make DRX exit mode MSGR RSP handling non-blocking 
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
19/03/14   cja      CR632037 Use RF Task for X2G
15/02/14   ip/npt  CR547955: Define multi thread usage on wake up from sleep
01/11/13   pa	   CR569692: X2G via RF Task. 
14/10/13   pa      CR536820: Added GL1 Msgr If Task. Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "msgr_types.h"
#include "queue.h"
#include "rfgsm_msg.h"


/*===========================================================================

                     DATA STRUCTURES

===========================================================================*/

#define GL1_MSGRIF_RF_TASK_MSGR_QUEUE_SIZE 13
#if defined(GERAN_L1_IP2CAL) && defined(GERAN_L1_HLLL_LNASTATE)
#define NUM_GERAN_RF_TASK_MSGR_MSGS             7
#elif defined(GERAN_L1_IP2CAL) || defined(GERAN_L1_HLLL_LNASTATE)
#define NUM_GERAN_RF_TASK_MSGR_MSGS             6
#else
#define NUM_GERAN_RF_TASK_MSGR_MSGS             5
#endif

typedef union
{
  msgr_hdr_struct_type                   hdr;
#ifdef FEATURE_GSM_RX_TX_SPLIT
  rfa_rf_gsm_rx_wakeup_cnf_s             rfa_rf_gsm_rx_wakeup_cnf;
#else
  rfa_rf_gsm_wakeup_cnf_s                rfa_rf_gsm_wakeup_cnf;
#endif
  rfa_rf_gsm_rx_burst_cnf_s              rfa_rf_gsm_rx_burst_cnf;
  rfa_rf_gsm_set_tx_band_cnf_s           rfa_rf_gsm_tx_band_cnf;
  rfa_rf_gsm_tx_burst_cnf_s              rfa_rf_gsm_set_tx_burst_cnf;
  rfa_rf_gsm_tx_enable_cnf_s             rfa_rf_gsm_tx_enable_cnf;
#ifdef GERAN_L1_IP2CAL
  rfa_rf_gsm_start_ip2_cal_cnf_s         rfa_rf_gsm_start_ip2_cal_cnf;
#endif
#ifdef GERAN_L1_HLLL_LNASTATE
  rfa_rf_gsm_process_burst_metrics_cnf_s rfa_rf_gsm_get_metrics_cnf;
#endif
} u_gl1_msgrif_rf_task_msgr;


typedef struct
{
  q_link_type           link;
  u_gl1_msgrif_rf_task_msgr  msg;

} gl1_msgrif_rf_task_msgr_t;

#define  GL1_MSGRIF_GFW_ASYNC_MSGR_QUEUE_SIZE  15

typedef union
{
  msgr_hdr_struct_type              hdr;
  GfwHostAsyncRsp                   gfwHostAsyncRsp;
  GfwHostSleepRsp                   gfwHostSleepRsp;
  GfwHostRfmExitModeRsp             gfwHostRfmExitModeRsp;
#ifdef FEATURE_WLAN_COEX_SW_CXM
  GfwWlanCxmConfigRsp               gfwWlanCxmConfigRsp;
#endif
  GfwAsyncCcsStatusCmd              gfwAsyncCcsStatusCmd;
  GfwHostCleanupDoneCmd             gfwHostCleanupDoneCmd;
} u_gl1_msgrif_gfw_async_msgr;

typedef struct
{
  q_link_type       link;
  u_gl1_msgrif_gfw_async_msgr  msg;
} gl1_msgrif_gfw_async_msgr_t;

/*===========================================================================

                         FUNCTION DECLARATIONS

===========================================================================*/

void gl1_msgrif_initialise(gas_id_t gas_id);
void gl1_msgrif_task_mutex_unlock(gas_id_t gas_id);
void gl1_msgrif_task_mutex_lock(gas_id_t gas_id);

void gl1_msgrif_init_rf_task_msgr_intf(gas_id_t gas_id);
void gl1_msgrif_init_async_msgr_intf(gas_id_t gas_id);

void  gl1_msgrif_init_G2X_msgr_intf(gas_id_t gas_id);

#ifdef FEATURE_DUAL_DATA
void gl1_msgrif_init_cfcm_cpu_monitor_msgr_intf(gas_id_t gas_id);
#endif
void gl1_msgrif_init_vfr_map_msgr_intf(gas_id_t gas_id);

extern void gl1_msgrif_init_teardown(gas_id_t gas_id);

void register_LTE_irat_messages(gas_id_t gas_id);
void register_TDSCDMA_irat_messages(gas_id_t gas_id);
void teardown_LTE_irat_messages(gas_id_t gas_id);
void teardown_TDSCDMA_irat_messages(gas_id_t gas_id);

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
void register_WCDMA_irat_messages(gas_id_t gas_id);
void deregister_WCDMA_irat_messages(gas_id_t gas_id);
#endif
#endif /* GL1_MSGRIF_INIT_H */
