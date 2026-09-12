/*!
  @file
  lte_LL1_smem.h

  @brief
  Shared memory interface for LTE LL1.

  @detail
  Constants and structures for LTE LL1 FW-SW shared memory.

*/

/*===========================================================================

  Copyright (c) 2009 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/lte_LL1_smem.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/

#ifndef LTE_LL1_SMEM_H
#define LTE_LL1_SMEM_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "modem_fw_memmap.h"
#include "intf_prs_sm.h"
#include "intf_prs_fd_logging_types.h"


/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

// XXX TODO: These should be removed, or defined in terms of FW SMEM header.

/* uart utility driver on ARM9 */
//#define Q6_SMEM_UART_PHYS_START_ADDR 0x03a30000

/* PLT server buffer address */
#define Q6_SMEM_PLT_PHYS_START_ADDR  FW_SMEM_LTE_PLT_ADDR
#define Q6_SMEM_LTE_START_ADDR  FW_SMEM_LTE_ADDR

/* Enum for VPE clock setting for NLIC modes */
typedef enum
{
  LTE_LL1_SMEM_NLIC_CLK_SETTING_SVS2 = 0,
  LTE_LL1_SMEM_NLIC_CLK_SETTING_SVS,
  LTE_LL1_SMEM_NLIC_CLK_SETTING_NOMINAL,
  LTE_LL1_SMEM_NLIC_CLK_SETTING_TURBO,
  LTE_LL1_SMEM_NLIC_CLK_SETTING_MAX,
  LTE_LL1_SMEM_NLIC_CLK_SETTING_INVALID,
  LTE_LL1_SMEM_NLIC_CLK_SETTING_DEFAULT,
  _LTE_LL1_SMEM_NLIC_CLK_SETTING_FORCE_32_BIT = 0x7FFFFFFF
}lte_LL1_smem_nlic_clk_setting_e;

typedef struct
{
  /* PRS smem region */
  lte_LL1_prs_occasion_meas_s prs_smem;

  /* Other LTE smem region can be defined here */
  lte_LL1_smem_nlic_clk_setting_e nlic_clock_setting; 

  /* PRS FD Logging */
  lte_LL1_prs_fd_logging_db_s prs_fd_log_db;

  /* Filter UL BLER value */
  uint32 filtered_ul_bler[LTE_LL1_DL_NUM_CARRIERS];

  /* BSR value*/
  uint32 bsr_level;
  
  /* ARD SMEM params */
  /*ML1 thresholds*/
   uint32 s_ml1_1rx_threshold[LTE_LL1_DL_NUM_CARRIERS]; //2rx->1rx threshold from ML1, Q8 format
   uint32 s_ml1_2rx_threshold[LTE_LL1_DL_NUM_CARRIERS]; //1rx->2rx threshold from ML1, Q8 format

   uint32 wc2[LTE_LL1_DL_NUM_CARRIERS]; // num of SFs to monitor for 2->1

   boolean enable_trigger_to_ml1[LTE_LL1_DL_NUM_CARRIERS];   //send an IND to Ml1 if the 1rx/2rx state changes

   uint32 rs_snr[LTE_LL1_DL_NUM_CARRIERS];               //Q8 format, FW can write rs_snr, ML1 can use it for debug purpose while reading snr_cond_met_for_1rx or ind msg.
   boolean snr_cond_met_for_1rx[LTE_LL1_DL_NUM_CARRIERS]; //to indicate if snr_cond_met for 1rx

   int32 rsrp_max_threshold[LTE_LL1_DL_NUM_CARRIERS]; //Q7 format, consistent with ML1 neighbor rsrp format
   int32 rsrp_min_threshold[LTE_LL1_DL_NUM_CARRIERS];
   lte_LL1_ue_rx_ant_enum phy_index[LTE_LL1_DL_NUM_CARRIERS]; //ML1 to send the phy index for FW to monitor

   /* Payloads of ARX container MSG */
   //te_LL1_sys_arx_container_payload_t arx_container_req_smem[LTE_LL1_ARX_CONTAINER_MAX_PAYLOADS];
   lte_LL1_ue_rx_ant_enum phy_index_echo[LTE_LL1_DL_NUM_CARRIERS];//indicate which phy index FW is monitoring
   
   /* EFS - forced 1rx - ML1 to set below flag during EFS so that FW can skip validation checks of ARD.
    * Scenario: with EFS - During measurement gaps there is sys config app request with 1rx, there is no disable before. 
    * FW validation check failed as it saw 1rx config as part of sys config app while ARD database is in 1rx. 
    * Usually it will be 2rx and move to 1rx, due to EFS ML1 is sending 1rx always and not taking care of sequence of events.
 .  */
   boolean efs_1rx_enabled;

}lte_LL1_smem_t;

#endif /* LTE_LL1_SMEM_H */
