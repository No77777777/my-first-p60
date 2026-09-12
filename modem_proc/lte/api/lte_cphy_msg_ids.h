#ifndef LTE_CPHY_MSG_IDS_H
#define LTE_CPHY_MSG_IDS_H
/*!
  @file lte_cphy_msg_ids.h

  @brief
   The Message IDs used to interact with the LTE L1 software module.

   This file should contain all CPHY message ids.

*/

/*==============================================================================

  Copyright (c) 2009 - 2012 QUALCOMM Technologies Incorporated. All Rights
  Reserved

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

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/lte.mpss/6.6.1/api/lte_cphy_msg_ids.h#4 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
03/03/16   el      FR 28315 - MAC layer implementation
02/25/16   sks     FR 28315 Add LTED interface
08/17/15   dsp     CR 837798: UL Timing offset bet PCell and SCell
06/30/15   ymu     GM handling for dummy SR request to check connectivity after LTA
06/01/15   avj     FR 26957: Modem Power Consumption Statistics - LTE Tx power
12/03/12   st      CR 402042: Added a new message to send from MAC to Ml1
07/17/12   st      MAC-ML1 interface change for Rel 10 CA Scell MAC CE
09/06/11   anm     Added eMBMS related interface
05/25/11   tjc     IDs for PRS commands
02/10/10   fhu     Added support for DS to have AT commands to pass down
                    RX Chain requests
06/01/10   mm      ID for SERV CELL MEAS req\cnf
05/12/10   dk      ID for MAC DRX Command
05/12/10   da      ID definition for NMR info req\cnf
03/17/10   tjc     Added GPS timetag request message
12/08/09   sah     Initial Checkin

==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/


/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/


/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/


/*! @brief
  Enum for the external control saps used to interface with L1
*/
enum
{
  /*=========================================================================*/
  /*                        Request/Confirm                                  */
  /*=========================================================================*/
  LTE_CPHY_ACQ_ID = 0x0,
  LTE_CPHY_START_ID = 0x1,
  LTE_CPHY_STOP_ID = 0x2,
  LTE_CPHY_ABORT_ID = 0x3,
  LTE_CPHY_COMMON_CFG_ID = 0x4,
  LTE_CPHY_DEDICATED_CFG_ID = 0x5,
  LTE_CPHY_CON_RELEASE_ID = 0x6,
  LTE_CPHY_HANDOVER_ID = 0x7,
  LTE_CPHY_BAND_SCAN_ID = 0x8,
  LTE_CPHY_SYSTEM_SCAN_ID = 0x9,
  LTE_CPHY_CELL_SELECT_ID = 0xa,
  LTE_CPHY_START_RACH_ID = 0xb,
  LTE_CPHY_RACH_RC_ID = 0xc,
  LTE_CPHY_RAR_PARAMS_ID = 0xd,
  LTE_CPHY_RACH_ABORT_ID = 0xe,
  LTE_CPHY_RACH_CFG_ID = 0xf,
  LTE_CPHY_OUT_OF_SYNC_ID = 0x10,
  LTE_CPHY_APPLY_TA_PARAMS_ID = 0x11,
  LTE_CPHY_SIB_SCHED_ID = 0x12,
  LTE_CPHY_CELL_BAR_ID = 0x13,
  LTE_CPHY_IDLE_DRX_CFG_ID = 0x14,
  LTE_CPHY_IDLE_MEAS_CFG_ID = 0x15,
  LTE_CPHY_CONN_MEAS_CFG_ID = 0x16,
  LTE_CPHY_CELL_RESEL_CANCEL_ID = 0x17,
  LTE_CPHY_CANCEL_CONN_ID = 0x18,
  LTE_CPHY_SUSPEND_ID = 0x19,
  LTE_CPHY_RESUME_ID = 0x1a,
  LTE_CPHY_IRAT_CDMA_SYSTEM_TIME_CHG_ID = 0x1b,
  LTE_CPHY_BPLMN_START_ID = 0x1c,
  LTE_CPHY_BPLMN_STOP_ID = 0x1d,
  LTE_CPHY_BPLMN_CELL_ID = 0x1e,
  LTE_CPHY_BPLMN_SKIP_ID = 0x1f,
  LTE_CPHY_SEND_UL_SR_ID = 0x20,
  LTE_CPHY_DEACTIVATE_SPS_ID = 0x21,
  LTE_CPHY_UL_TO_OOS_ID = 0x22,
  LTE_CPHY_TDD_CFG_ID   = 0x23,
  LTE_CPHY_GPS_TIMETAG_ID = 0x24,
  LTE_CPHY_NMR_INFO_ID = 0x25,
  LTE_CPHY_MAC_DRX_ID = 0x26,
  LTE_CPHY_SERV_CELL_MEAS_ID = 0x27,
  LTE_CPHY_GPS_MRL_ID = 0x28,
  LTE_CPHY_GPS_EVENT_CB_ID = 0x29,
  LTE_CPHY_RFCHAIN_ID = 0x2A,
  LTE_CPHY_PRS_START_ID = 0x2D,
  LTE_CPHY_PRS_UPDATE_CELLS_ID = 0x2E,
  LTE_CPHY_PRS_STOP_ID = 0x2F,
  LTE_CPHY_ABORT_IRAT_CGI_ID = 0x30,
  LTE_CPHY_MCCH_CHANGE_NOTIFICATION_ID = 0x31,
  LTE_CPHY_MSI_ID = 0x32,
  LTE_CPHY_CDMA_SYSTIME_ID = 0x33,
  LTE_CPHY_HRPD_MEAS_RESULTS_ID = 0x34,
  LTE_CPHY_DRX_OPT_ID = 0x35,
  LTE_CPHY_GPS_TIMEXFER_ID = 0x36,
  LTE_CPHY_GPS_TIMEXFER_ABORT_ID = 0x37,
  LTE_CPHY_BPLMN_SUSPEND_ID = 0x38,
  LTE_CPHY_MBSFN_SIGNAL_STRENGTH_ID= 0x39,
  LTE_CPHY_UEINFO_RPT_ID= 0x3a,
  LTE_CPHY_GPS_RXTX_REQ_ID = 0x3B,
  LTE_CPHY_GPS_RXTX_CNF_ID = 0x3C,
  LTE_CPHY_UEINFO_MDT_SESSION_ID = 0x3D,
  LTE_CPHY_UTC_TIME_UPDATE_REQ_ID = 0x3E,
  LTE_CPHY_UTC_TIME_UPDATE_CNF_ID = 0x3F,
  LTE_CPHY_CELL_UNBAR_ID = 0x40,
  LTE_CPHY_BEST_MBMS_NEIGH_FREQ_ID = 0x41,
  LTE_CPHY_TRM_PRIORITY_ID = 0x42,
  LTE_CPHY_RELEASE_TRM_ID = 0x43,
  LTE_CPHY_CDMA_CHANNEL_BAR_ID = 0x44,
  LTE_CPHY_DEBUG_DEADLOCK_TMR_EXP_ID = 0x45,
  LTE_CPHY_UEINFO_CONN_FAILURE_RPT_REQ_ID = 0x46,
  LTE_CPHY_UEINFO_CONN_FAILURE_RPT_CNF_ID = 0x47,
  LTE_CPHY_IND_HANDOVER_SFN_STATUS_ID = 0x48,
  LTE_CPHY_NV_UPDATE_ID = 0x49,
  LTE_CPHY_DL_TPUT_ESTM_REPORT_ID = 0x4A,
  LTE_CPHY_GPS_MRL_SUBSCRIBE_ID = 0x4B,
  LTE_CPHY_HARQ_ACK_TX_STS_ID = 0x4C,
  LTE_CPHY_ACHIEVABLE_TPUT_INFO_ID = 0x4D,
  LTE_CPHY_UL_CONFIGURABLE_TPUT_INFO_ID = 0x4E,
  LTE_CPHY_REQ_EMBMS_SET_PRIORITY_ID = 0x4F,
  LTE_CPHY_REQ_EMBMS_GET_MSP_DATA_ID = 0x50,
  LTE_CPHY_IND_EMBMS_GET_MSP_DATA_ID = 0x51,
  LTE_CPHY_SCELL_CANCEL_ID = 0x52,
  LTE_CPHY_SCELL_MEAS_CFG_ID = 0x53,
  LTE_CPHY_UL_TX_POWER_INFO_ID = 0x54,
  LTE_CPHY_ACHIEVABLE_TPUT_STATUS_ID = 0X55,
  LTE_CPHY_SCELL_FC_STATUS_ID = 0x56,
  LTE_CPHY_TRM_UNLOCK_ALLOWED_ID     = 0x57,
  LTE_CPHY_SEND_GM_ADJ_SR_MAX_TX_ID = 0x58,
  LTE_CPHY_DTOD_CFG_ID = 0x59,
  LTE_CPHY_DTOD_DED_CFG_ID = 0x5A,
  LTE_CPHY_IND_DTOD_CELL_DETECT_ID = 0x5b,
  LTE_CPHY_IND_DTOD_SCAN_ID = 0x5c,
  LTE_CPHY_ACTIVATE_LTED_TX_ID = 0x5D,
  LTE_CPHY_PAC_TX_REQ_ID = 0x5E,
  LTE_CPHY_PAC_TX_CNF_ID = 0x5F,
  LTE_CPHY_ACTIVATE_LTED_DL_ID = 0x60,
  LTE_CPHY_LTED_SIB_SCHED_ID = 0x61,
  LTE_CPHY_LTED_CELL_SEL_ID = 0x62,
  LTE_CPHY_OOS_WARNING_ID = 0x63,
  LTE_CPHY_DDS_CHANGE_ID = 0x64,
  LTE_CPHY_POWER_CLASS_UPDATE_ID = 0x65,
  LTE_CPHY_VOLTE_STATUS_ID = 0x66,
  LTE_CPHY_KPI_BASED_THRES_ID = 0x67,
#ifdef LTE_FEATURE_PAGE_SHARING
  LTE_CPHY_PAGE_MERGE_ID = 0x68,
  LTE_CPHY_PAGE_SPLIT_ID = 0x69,
#endif
  LTE_CPHY_DO_TO_L_STATUS_ID = 0x73,
};

  /*=========================================================================*/
  /*                          Indications                                    */
  /*=========================================================================*/
enum
{
  LTE_CPHY_IND_PDCCH_ORDER_ID = 0x0,
  LTE_CPHY_IND_RA_TIMER_EXPIRED_ID = 0x1,
  LTE_CPHY_IND_MIB_ID = 0x2,
  LTE_CPHY_IND_MOD_PRD_BND_ID = 0x3,
  LTE_CPHY_IND_DL_WEAK_ID = 0x4,
  LTE_CPHY_IND_CELL_RESEL_ID = 0x5,
  LTE_CPHY_IND_OOS_ID = 0x6,
  LTE_CPHY_IND_RL_FAILURE_ID = 0x7,
  LTE_CPHY_IND_MEAS_REPORT_ID = 0x8,
  LTE_CPHY_IND_MSG3_TRANSMISSION_ID = 0x9,
  LTE_CPHY_IND_BPLMN_TIME_AVAIL_ID = 0xa,
  LTE_CPHY_IND_BPLMN_COMPLETE_ID = 0xb,
  LTE_CPHY_IND_UL_PKT_BUILD_ID = 0xc,
  LTE_CPHY_IND_UL_PKT_FAILED_ID = 0xd,
  LTE_CPHY_IND_PDCCH_ORDER_RACH_SUCCESS_ID = 0xe,
  LTE_CPHY_IND_RSSI_ID = 0xf,
  LTE_CPHY_IND_SIGNAL_REPORT_CFG_ID = 0x10,
  LTE_CPHY_IND_MAC_RAA_ID = 0x11,
  LTE_CPHY_IND_T310_STATUS_ID = 0x12,
  LTE_CPHY_IND_SCELL_STATUS_ID = 0x13,
  LTE_CPHY_IND_RA_PID_OFF_ID = 0x14,
  LTE_CPHY_IND_UEINFO_MDT_REPORT_ID = 0x15,
  LTE_CPHY_IND_CA_EVENT_ID = 0x16,
  LTE_CPHY_IND_IRAT_ASF_NEEDED_ID = 0x17,
  LTE_CPHY_IND_RF_AVAILABLE_ID = 0x18,
  LTE_CPHY_IND_RF_UNAVAILABLE_ID = 0x19,
  LTE_CPHY_IND_IFREQ_OTDOA_ID = 0x1a,
  LTE_CPHY_IND_BPLMN_SIB_RECEIVED_ID = 0x1b,
  LTE_CPHY_IND_BLACKLISTED_CSG_PCI_RANGE_ID = 0x1c,
  LTE_CPHY_IND_UE_MODE_CHANGE_ID = 0x1d,
  LTE_CPHY_IND_TCELL_RF_UNAVAILABLE_ID = 0x1e,
  LTE_CPHY_IND_CDRX_PKT_TIMING_ID = 0x1f,
  LTE_CPHY_IND_SCELL_TUNEAWAY_STATUS_ID = 0x20,
  LTE_CPHY_IND_RLF_DEBUG_ID = 0x21,
  LTE_CPHY_IND_CONFIGURED_TPUT_INFO_ID = 0x22,
  LTE_CPHY_IND_ACHIEVABLE_TPUT_INFO_START_ID = 0x23,
  LTE_CPHY_IND_ACHIEVABLE_TPUT_INFO_STOP_ID = 0x24,
  LTE_CPHY_IND_RACH_MSG1_SCHED_ID = 0x25,
  LTE_CPHY_IND_SCELL_MEAS_REPORT_ID = 0x26,
  LTE_CPHY_IND_UL_TX_POWER_ID = 0x27,
  LTE_CPHY_IND_MAC_TAG_OOS_ID = 0x28,
  LTE_CPHY_IND_TTL_COR_ID = 0x29,
  LTE_CPHY_IND_START_LTED_TX_ID = 0x2a,
  LTE_CPHY_IND_LTED_START_DL_PERIOD_ID = 0x2b,
  LTE_CPHY_IND_UL_PROSE_PAC_BUILD_ID = 0x2c,
  LTE_CPHY_IND_IRAT_CGI_START_ID = 0x2d,
  LTE_CPHY_IND_IRAT_CGI_END_ID = 0x2e,
  LTE_CPHY_IND_LTED_RESUME_ID = 0x2f,
  LTE_CPHY_IND_LTED_BACKOFF_ID = 0x30,
  LTE_CPHY_IND_CA_EVENT_EXT_ID = 0x31,
  LTE_CPHY_ELS_MCE_TRIGGER_ID = 0x32,
  LTE_CPHY_ELS_MCE_CANCEL_ID = 0x33,
  LTE_CPHY_ELS_MCE_NACK_ID = 0x34,
  LTE_CPHY_IND_CAT_DOWNGRADE_ID = 0x35,
  LTE_CPHY_IND_MAC_FC_ID = 0x36,
  LTE_CPHY_IND_MAC_FC_STATE_ID = 0x37,
  LTE_CPHY_IND_SR_TRIGGER_BY_ML1_ID     = 0x38,
  LTE_CPHY_IND_EXTEND_RRC_CONNECTION_ID = 0x39,
  LTE_CPHY_IND_WIFI_MOBILITY_STATUS_ID = 0x3a,
  LTE_CPHY_IND_SRVCC_STATUS_ID = 0x3b,
  LTE_CPHY_IND_PAGE_MERGE_REEVALUATE_ID = 0x3c,
  LTE_CPHY_IND_MIMO_MOD_EVENT_ID = 0x3d,
  LTE_CPHY_IMS_PRE_HO_ID    = 0x3e,
};



#endif /* LTE_CPHY_MSG_IDS_H */
