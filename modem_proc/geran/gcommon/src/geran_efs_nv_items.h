#ifndef GERAN_EFS_NV_ITEMS_H
#define GERAN_EFS_NV_ITEMS_H

/*============================================================================
  @file geran_efs_nv_items.h

  @brief This header file contains a list of the GERAN EFS-NV items

                Copyright (c) 2012-2015 Qualcomm Technologies, Inc.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/* $Header: //components/rel/geran.mpss/7.4.0/gcommon/src/geran_efs_nv_items.h#1 $ */
/* $DateTime: 2019/06/06 23:27:31 $$Author: mplcsds1 $ */

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

when       who      what, where, why
--------   -------- ---------------------------------------------
25/04/18   rv      CR2225983 - GERAN F3 and log pkt Reduction TA.3.0.c1
25/01/16   cws      CR962379  Disable/enable ASDIV in IDLE/RACH state by NV
06/07/17   mk       CR2071218 FR44121 -Synchronous Ncell Quality Monitor - Added NV control
02/02/17   br       CR1103497 FR 39735: Allowing TA during PAGE RE-ORG mode in GERAN
14/02/17   mk       CR2004610 : FR 40251 (SeNS) - Add NV item to enable SeNS feature
21/04/17   br       CR1041084 FR 36219: GDET – GMSK Modulation Detector
12/08/15   cws      CR864883  FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
10/03/15   sjv      CR805970: SCell RSSI Threshold set through a new NV for DR to SR Fallback

===========================================================================*/

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/

#include "customer.h"
#include "comdef.h"

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/

const char* const geran_efs_nv_items[] =
{
 "/nv/item_files/gsm/gl1/search_all_w_cell",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_type_2_en",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_trafi_mdm_threshold",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_idle_mdm_threshold",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_trafi_sens",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_idle_sens",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_qsc_large_delta",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_qsc_small_delta",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_qsc_hysteriesis_time",
  "/nv/item_files/gsm/gl1/antenna_switch_tx_div_min_delta_btw_antenna",
  "/nv/item_files/gsm/gl1/antenna_switching_tx_diversity_threshold",
  "/nv/item_files/gsm/gl1/antenna_switching_tx_diversity_enable",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_type_2_idle_suspension_time",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_type_2_traffic_crisis_mode_thre",
  "/nv/item_files/gsm/gl1/antenna_switching_gsm_type_2_idle_pingpong_mitigation_thre",
  "/nv/item_files/gsm/gl1/antenna_switch_UL_DL_delta_table",
  "/nv/item_files/gsm/gl1/antenna_switch_control_mask",
  "/nv/item_files/gsm/gl1/gsm_rx_diversity",
  "/nv/item_files/gsm/gl1/gsm_commercial_recovery_restart",
  "/nv/item_files/gsm/gl1/gl1_nv_gfw_recovery",
  "/nv/item_files/gsm/l1/l1_debug",
  "/nv/item_files/gsm/l1/l1_q6_clock",
  "/nv/item_files/modem/geran/gfw_debug",
  "/nv/item_files/modem/geran/vamos_support",
  "/nv/item_files/modem/geran/gfw_diag_group1",
  "/nv/item_files/modem/geran/gfw_diag_group2",
  "/nv/item_files/modem/geran/gfw_diag_group3",
  "/nv/item_files/modem/geran/gfw_diag_group4",
  "/nv/item_files/modem/geran/grr/g2l_blind_redir_control",
  "/nv/item_files/modem/geran/g2x_ta_support",
  "/nv/item_files/modem/geran/gfw_aeq_phase4_control",
  "/nv/item_files/gsm/gl1/lif_feature_control",
  "/nv/item_files/modem/geran/parallel_sch_support",
  "/nv/item_files/modem/geran/g2x_idle_tuneaway_support",
  "/nv/item_files/gsm/gl1/RF_Debug_Crash_Enable",
  "/nv/item_files/gsm/gl1/RF_Debug_Crash_Enable_sub2",
  "/nv/item_files/gsm/gl1/RF_Debug_Crash_Enable_sub3",
  "/nv/item_files/gsm/gl1/RF_Debug_upper_power_limit",
  "/nv/item_files/gsm/gl1/RF_Debug_Lower_Limit",
  "/nv/item_files/gsm/gl1/RF_Debug_band",
  "/nv/item_files/gsm/gl1/RF_Debug_arfcn",
  "/nv/item_files/gsm/gl1/gsm_dr_to_sr_fallback_rssi_thresh",
  "/nv/item_files/modem/geran/gdet_support",
  "/nv/item_files/modem/geran/gdet_threshold",
  "/nv/item_files/gsm/gl1/gl1_sens_fr_control",
  "/nv/item_files/gsm/gl1/gl1_synq_fr_control",
  "/nv/item_files/gsm/gl1/gl1_disable_conditional_logging"  
};

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 * -------------------------------------------------------------------------*/


#endif /* GERAN_EFS_NV_ITEMS_H */

/* EOF */

