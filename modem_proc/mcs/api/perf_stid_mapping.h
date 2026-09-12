/*!
  @file
  perf_stid_mapping.h

  @brief
  STID macros/enums for helping gather accurate MPPS task level information.

*/

/*===========================================================================

  Copyright (c) 2019 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

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

$Header: //components/rel/mcs.mpss/5.2/api/perf_stid_mapping.h#1 $

===========================================================================*/

#ifndef PERF_STID_MAPPING_H
#define PERF_STID_MAPPING_H

/*===========================================================================

                         MACRO DEFINITIONS

===========================================================================*/

/*
 * STID macro naming convention
 * PERF_STID_<CATEGORY>_<GROUP>
 *
 * --------PMUSTID RANGES----------
 * CATEGORY     |  MIN   |  MAX   |
 * --------------------------------
 * LFW          |   1    |   10   |
 * LTE ML1      |   11   |   15   |
 * LTE L2       |   16   |   20   |
 * LTE RF       |   21   |   30   |
 * IPA          |   31   |   35   |
 * 5GNR FW      |   36   |   45   |
 * 5GNR ML1     |   46   |   55   |
 * 5GNR L2      |   56   |   75   | 
 * 5GNR RF      |   76   |   85   |
 * GNSS         |   86   |   90   |
 * V2X          |   91   |   100  |
 * OTHER_TASK   |   101  |   249  |
 * STID_GROUP   |   250  |   254  |
 * IDLE         |   255  |   255  |
 * --------------------------------
 */

/* LFW Task STID */
#define PERF_STID_LFW_DEFAULT                 1
#define PERF_STID_LFW_SCHD_CMN                2
#define PERF_STID_LFW_SCHD_DL                 3
#define PERF_STID_LFW_UL                      4
#define PERF_STID_LFW_FEND                    5
#define PERF_STID_LFW_PDCCH_IRQ_HANDLER_CCHP  6
#define PERF_STID_LFW_SCH_SCHED               7
#define PERF_STID_LFW_OSTMR_ISR               8

/* LTE ML1 Task STID */
#define PERF_STID_ML1_DEFAULT                 11
#define PERF_STID_ML1_GM                      12
#define PERF_STID_ML1_MGR                     13

/* L2 LTE Task STID*/
#define PERF_STID_L2_LTE_DEFAULT              16
#define PERF_STID_L2_LTE_RLC_DL               17
#define PERF_STID_L2_LTE_MAC_UL               18
#define PERF_STID_L2_LTE_MAC_DL               19

/* RF LTE Task STID*/
#define PERF_STID_RF_LTE_DEFAULT              21
#define PERF_STID_RF_LTE_RFLM_SLOT_SCHED      22
#define PERF_STID_RF_LTE_RFLM_CCS             23

/* IPA Task STID */
#define PERF_STID_IPA_DEFAULT                 31
#define PERF_STID_IPA_DL_PROD                 32
#define PERF_STID_IPA_DL                      33
#define PERF_STID_IPA_UL                      34

/* NR5G FW Task */
#define PERF_STID_FW_NR5G_DEFAULT             36
#define PERF_STID_FW_NR5G_LOOP                37
#define PERF_STID_FW_NR5G_FTL_TTL             38
#define PERF_STID_FW_NR5G_TRAFFIC             39
#define PERF_STID_FW_NR5G_MAINTENANCE         40

/* NR5G L2 SW STID */
#define PERF_STID_L2_NR5G_DEFAULT             56
#define PERF_STID_L2_NR5G_MAC_DL              57
#define PERF_STID_L2_NR5G_MAC_UL              58
#define PERF_STID_L2_NR5G_DL                  59
#define PERF_STID_L2_NR5G_UL                  60
#define PERF_STID_L2_NR5G_UL_PB               61

/* NR5G RF Task */
#define PERF_STID_RF_NR5G_DEFAULT             76
#define PERF_STID_RF_NR5G_HIGH                77

/* GNSS Task STID */
#define PERF_STID_GNSS_DEFAULT                86
#define PERF_STID_GNSS_UCC                    87
#define PERF_STID_GNSS_MGPMC_NF               88

/* V2X SW Task STID */
#define PERF_STID_V2X_SW_DEFAULT              91
#define PERF_STID_V2X_SW_L1_MGR               92
#define PERF_STID_V2X_SW_L1_SCHDLR            93
#define PERF_STID_V2X_SW_L2_TX                94
#define PERF_STID_V2X_SW_L2_RX                95


#endif /* PERF_STID_MAPPING_H */