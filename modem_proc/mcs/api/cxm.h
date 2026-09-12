#ifndef __CXM_H__
#define __CXM_H__
/*!
  @file cxm.h

  @brief
   APIs exposed by the CXM Layer

*/

/*=============================================================================

  Copyright (c) 2020 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document
  are confidential and proprietary information of Qualcomm Technologies
  Incorporated and all rights therein are expressly reserved.  By accepting
  this material the recipient agrees that this material and the information
  contained therein are held in confidence and in trust and will not be used,
  copied, reproduced in whole or in part, nor its contents revealed in any
  manner to others without the express written permission of Qualcomm
  Technologies Incorporated.

=============================================================================*/

/*==============================================================================

$Header: //components/rel/mcs.mpss/5.2/api/cxm.h#3 $

=============================================================================*/

/*=============================================================================

                           INCLUDE FILES

=============================================================================*/
#include <comdef.h>
#include "msgr.h"
#include "cxm_intf_types.h"
#include "sys.h"

/* External headers */
#include <rfm_device_types.h>
#include <lte_l1_types.h>

/* wwan_coex_mgr.h_START - move/rename when ready */
#include <rfm_common_sar.h>
/* wwan_coex_mgr.h_END - move/rename when ready */

/*=============================================================================

                       COMMON TYPES AND DEFINES

=============================================================================*/
/* @brief Maximum supported sets of freq_bw (link) info sets for a tech */
#define CXM_MAX_SUPPORTED_LINK_SETS 24

/* wwan_coex_mgr.h_START - move/rename when ready */
/* Maximum supported channels for each tech for SAR */
#define CXM_STX_MAX_CHANNELS 64
/* Maximum supported rows & columns for STX's look-up tables (LUTs) */
#define CXM_STX_MAX_LUT_ROWS 5
#define CXM_STX_MAX_LUT_COLS 2
/* wwan_coex_mgr.h_END - move/rename when ready */

/* Max CA chain supported. Each CA chain could have different tech specific 
   parameters */
#define CXM_MAX_CA_SUPPORTED 3

/* Max number of Tx Chains for WLAN */
#define CXM_MAX_WLAN_TX_CHAINS 2

/*! @brief CXM Msgr IDs for All Messages */
enum
{
  /* CxM common IDs */
  CXM_WWAN_TECH_STATE_ID         = 0x00,
  CXM_WWAN_TECH_STATE_UPDATE_ID  = 0x01,
  
  /* WCN-WWAN COEX IDs */
  COEX_BOOT_PARAMS_ID            = 0x20,
  COEX_ACTIVE_POLICY_LTE_ID      = 0x21,
  COEX_TECH_SLEEP_WAKEUP_ID      = 0x22,
  COEX_TECH_TX_ADV_NTC_ID        = 0x23,
  COEX_WCN_TXFRMDNL_REPORT_ID    = 0x24,
  COEX_TECH_TX_FRM_DNL_REPORT_ID = 0x25,
  COEX_TX_PWR_LMT_LTE_CNDTNS_ID  = 0x26,
  COEX_METRICS_LTE_BLER_ID       = 0x27,
  COEX_METRICS_LTE_SINR_ID       = 0x28,
  /*COEX_FW_COUNTERS_RESET_ID    = 0x29, - defined in cxm_fw_msg.h*/
  /*COEX_TRIGGER_WCN_PRIO_ID     = 0x2A, - defined in cxm_fw_msg.h*/
  COEX_TECH_FRAME_TIMING_ID      = 0x2B,
  /*COEX_CONFIG_PARAMS_ID        = 0x2C, - defined in cxm_fw_msg.h*/
  COEX_TECH_ACTIVITY_ADV_NTC_ID  = 0x2D,
  COEX_METRICS_TDSCDMA_ID        = 0x2E,
  COEX_METRICS_GSM1_ID           = 0x2F,
  COEX_METRICS_GSM2_ID           = 0x30,
  COEX_METRICS_GSM3_ID           = 0x31,
  COEX_TECH_METRICS_ID           = 0x32,
  COEX_ACTIVE_POLICY_TDSCDMA_ID  = 0x33,
  COEX_ACTIVE_POLICY_GSM1_ID     = 0x34,
  COEX_ACTIVE_POLICY_GSM2_ID     = 0x35,
  COEX_ACTIVE_POLICY_GSM3_ID     = 0x36,
  COEX_HIGH_PRIORITY_ID          = 0x37,
  COEX_TECH_TX_PWR_REPORT_ID     = 0x38,
  COEX_LOG_ASYNC_ID              = 0x39,
  COEX_TECH_L3_STATE_ID          = 0x3A,
  COEX_TX_ANT_SEL_ID             = 0x3B,
  COEX_WLAN_CONN_STATE_ID        = 0x3C,
  COEX_LTEU_RRC_INFO_ID          = 0x3D,

  /* WWAN-WWAN COEX IDs*/
  STX_SET_POWER_ID               = 0x40,
  STX_SET_TX_STATE_ID            = 0x41,
  STX_CLR_TX_STATE_ID            = 0x42,
  STX_SET_TX_STATE_WITH_CHS_ID   = 0x43,
  
  COEX_POWER_ID                  = 0x44,
  SET_ACTIVITY_TL_ID             = 0x45,
  INACTIVITY_CLIENT_REG_ID       = 0x46,

  FREQID_LIST_GSM1_ID            = 0x47,
  FREQID_LIST_GSM2_ID            = 0x48,
  FREQID_LIST_WCDMA_ID           = 0x49,
  FREQID_LIST_ONEX_ID            = 0x50,
  FREQID_LIST_HDR_ID             = 0x51,
  FREQID_LIST_TDSCDMA_ID         = 0x52,
  FREQID_LIST_LTE_ID             = 0x53,
  FREQID_LIST_GSM3_ID            = 0x54,
  INACTIVITY_REPORT_ONEX_ID      = 0x55,
  INACTIVITY_REPORT_HDR_ID       = 0x56,
  REQUEST_ACTIVITY_INFO_GSM1_ID  = 0x57,
  REQUEST_ACTIVITY_INFO_GSM2_ID  = 0x58,
  REQUEST_ACTIVITY_INFO_GSM3_ID  = 0x59,
  WWCOEX_STATE_UPDATE_ID         = 0x60,
  SAR_SET_DSI_ID                 = 0x61,
  FILTERED_POWER_ID              = 0x62,
  CHAIN_OWNER_UPDATE_ID          = 0x63,

  BAND_AVOID_FREQ_ID             = 0x64,
  BAND_AVOID_PWR_ID              = 0x65,
  BAND_AVOID_BLIST_RSP_ID        = 0x66,
  BAND_AVOID_BLIST_IND_GSM1_ID   = 0x67,
  BAND_AVOID_BLIST_IND_GSM2_ID   = 0x68,
  BAND_AVOID_BLIST_IND_GSM3_ID   = 0x69,
  BAND_AVOID_BLIST_IND_LTE_ID    = 0x70,
  BAND_AVOID_BLIST_IND_ONEX_ID   = 0x71,
  BAND_AVOID_BLIST_IND_HDR_ID    = 0x72,
  BAND_AVOID_BLIST_IND_TDS_ID    = 0x73,
  BAND_AVOID_BLIST_IND_WCDMA_ID  = 0x74,

  DESENSE_IND_REQ_ID             = 0x75,

  SET_SLOT_ACT_TL_ID             = 0x76,
  SET_SLOT_ACT_TL_ID_LTE_REC     = 0x77,
  SET_SLOT_ACT_TL_ID_TDS_REC     = 0x78,
  SET_SLOT_ACT_TL_ID_WCDMA_REC   = 0x79,

  TECH_DISTRESS_ID               = 0x7A,
  COEX_TECH_STATE_BCAST_ID       = 0x7B,
  SET_SLOT_ACT_TL_ID_UNUSED      = 0x7C,
  REPORT_GNSS_ACTIVITY_ID        = 0x7D,
  GNSS_TECH_STATE_BCAST_ID       = 0x7E,
  GNSS_TECH_STATE_REBCAST_ID     = 0x80,

  WLAN_RTSAR_PWR_ID              = 0x81,
  WLAN_RTSAR_PLIMIT_ID           = 0x82,
  WLAN_RTSAR_CFG_MSG_ID          = 0x83,
  WLAN_RTSAR_MSG_ID              = 0x84,
  MCS_RTSAR_MSG_ID               = 0x85,
  COEX_GNSS_STATE_ID             = 0x86,
  
  COEX_SIGNAL_HANDLER_ID         = 0x87,

  WLAN_RTSAR_CONN_INFO_ID        = 0x88,

  COEX_BT_CHAN_MAP_ID            = 0x8E,

  COEX_GNSS_RX_STATE_ID          = 0x8F,

  COEX_OPRT_MODE_ID              = 0x90,
};

/*! @brief RF device Type */
typedef rfm_device_enum_type cxm_rfdev_type;

/* wwan_coex_mgr.h_START - move/rename when ready */
/*! @brief SAR DSI (device state index) Type */
typedef rfm_common_sar_state_type cxm_sar_dsi_type;
/* wwan_coex_mgr.h_END - move/rename when ready */

/* Note: cxm_tech_type, the enum for supported WWAN techs is in cxm_intf_types.h */

/*! @brief CXM WCN Tech Identifier Type */
typedef enum
{
  CXM_WCN_TECH_DFLT_INVLD = -1, /*!< Default/Invalid wcn tech ID */
  CXM_TECH_WIFI           =  0, /*!< WIFI */
  CXM_TECH_BT,                  /*!< Bluetooth */
  CXM_WCN_TECH_MAX              /*!< Max wcn tech ID supported */
}cxm_wcn_tech_type;

/*! @brief Tech link (band_channel) direction [UL/DL or both] */
typedef enum
{
  CXM_LNK_DRCTN_INVLD,     /*!< Default/Invalid link direction */
  CXM_LNK_DRCTN_UL,        /*!< Up-Link info-set */
  CXM_LNK_DRCTN_DL,        /*!< Down-Link info-set */
  CXM_LNK_DRCTN_UL_AND_DL, /*!< Up-Link and Down-Link info-set */
  CXM_LNK_DRCTN_MAX        /*!< Max value supported */
}cxm_tech_link_direction;

/*! @brief Tech frame type [FDD or TDD opration] */
typedef enum
{
  CXM_FRAME_TYPE_INVLD,     /*!< Default/Invalid frame type */
  CXM_FRAME_TYPE_FDD,       /*!< TDD frame type */
  CXM_FRAME_TYPE_TDD,       /*!< FDD frame type */
  CXM_FRAME_TYPE_MAX
}cxm_tech_frame_type;


/* @brief Tech Link (band_channel) special type indicator bit masks
          Used to identify diversity links and power monitors.
          and if the links belong to Primary vs. CA1 vs. CA2 */
#define CXM_LNK_TYPE_UNUSED                    0x00000000 /* Primary Carrier */
#define CXM_LNK_TYPE_POWER_MONITOR             0x00000001 /* Power Monitor */
#define CXM_LNK_TYPE_DIVERSITY                 0x00000002 /* Diversity */
#define CXM_LNK_TYPE_CA1                       0x00000100 /* Carrier Aggregation 1 */
#define CXM_LNK_TYPE_CA2                       0x00000200 /* Carrier Aggregation 2 */
#define CXM_LNK_TYPE_IRAT                      0x00000004 /* IRAT */
#define CXM_LNK_TYPE_UNSUPPORTED               0xFFFFFCF8

/* @brief Tech link (band_channel) type to help identify 
          any additional special characteristic of the link 
          It is a 32 bit mask which uses the defines 
          CXM_LNK_TYPE_* shown above */
typedef uint32 cxm_tech_link_type;

/*! @brief Tech link info expected by CxM */
typedef struct
{
  uint32                  frequency; /*!< Tech operating frequency in KHz */
  uint32                  bandwidth; /*!< Tech operating bandwidth in Hz */
  uint32                  band;      /*!< Tech operating band */
  uint32                  channel;   /*!< Tech operating channel */
  cxm_rfdev_type          rf_device; /*!< RF device the tech is using */
  cxm_tech_link_type      type;      /*!< Type (Diversity/PowerMonitor) */
  cxm_tech_link_direction direction; /*!< Direction (Up-link/Down-Link/Both) */
  cxm_tech_frame_type     frame_type;/*!< FDD or TDD carrier */
}cxm_tech_link_info_s;

/*! @brief LTE specific state/parameters info set */
typedef struct
{
  lte_l1_frame_struct_e                 frame_structure; /*!< Frame Structure FDD or TDD per LTE stds 36.201 Section 4.2.1 */
  lte_l1_tdd_ul_dl_cfg_index_e          tdd_config;      /*!< TDD configuration per 36.211, table 4.2.2, valid only for TDD frame structure */
  lte_l1_tdd_special_subframe_pattern_e ssp;             /*!< Special subframe pattern */
  lte_l1_cp_e                           dl_cp;           /*!< DL CP */
  lte_cphy_ul_cyclic_prefix_length_e    ul_cp;           /*!< UL CP */
  uint32                                frame_offset;    /*!< Frame offset (timing advance) in uSecs */
  int8                                  cc_frame_offset; /*!< Offset between Primary CC and this CC in uSec */
}cxm_lte_info_s;

/*! @brief TDSCDMA specific state/parameters info set */
typedef struct
{  
  uint32 reserved; /*!< unused reserved - space holder for now */
}cxm_tds_info_s;

/*! @brief Union for all TECH specific data */
typedef union
{
  cxm_lte_info_s lte_params; /*!< LTE specific parameters */
  cxm_tds_info_s tds_params; /*!< TDS specific parameters */ 
}cxm_tech_specific_params_u;

/*! @brief Complete tech data set for CxM */
typedef struct
{
  uint8                      num_link_info_sets;                     /*!< Current # of link info sets */
  cxm_tech_link_info_s       link_list[CXM_MAX_SUPPORTED_LINK_SETS]; /*!< Operating list of link/freq info */
  cxm_tech_specific_params_u params;                                 /*!< DEPRECATED: DO NOT USE */
  cxm_tech_specific_params_u cc_params[CXM_MAX_CA_SUPPORTED];        /*!< Tech specific params for coexistence */
}cxm_tech_data_s;

/* wwan_coex_mgr.h_START - move/rename when ready */
/*! @brief Enum for STX power type */
typedef enum
{
  STX_PWR_INVLD,        /*!< Default/Invalid value */
  STX_RX_VALID_ONLY,    /*!< Only RX pwr is valid */
  STX_TX_VALID_ONLY,    /*!< Only TX pwr is valid */
  STX_RX_TX_VALID_BOTH, /*!< Both pwrs are valid */
  STX_PWR_MAX           /*!< Max value supported */
}cxm_pwr_type;

typedef enum
{  
  STX_LIMIT_SAR_ONLY,     /*!< Only SAR limit has changed */  
  STX_LIMIT_COEX_ONLY,    /*!< Only COEX limit has changed */  
  STX_LIMIT_SAR_COEX_BOTH /*!< Both SAR and COEX limits have changed */
}cxm_stx_plimit_type;

typedef enum 
{
  CXM_SLOT_PRIO_LOW,   /*!< Low prio slot, used by WLAN */  
  CXM_SLOT_PRIO_HIGH   /*!< High prio slot, used by WLAN */
}cxm_slot_prio_e;

typedef enum 
{
  CXM_WLAN_RTSAR_ENABLE_CMD,   /*!< RTSAR enable command */
  CXM_WLAN_RTSAR_DISABLE_CMD,  /*!< RTSAR disable command */
  CXM_WLAN_RTSAR_ENABLE_ACK,   /*!< RTSAR enable acknowledgement */
  CXM_WLAN_RTSAR_DISABLE_ACK,   /*!< RTSAR disable acknowledgement */
  CXM_WLAN_RTSAR_ENABLE_REQ    /*!< RTSAR enable request from WLAN */
}cxm_wlan_rtsar_msg_e;

/* Max number of transmitters for WLAN 
 * Each Tx chain can transmit on either 2.4 GHz or 5 GHz band
 */

typedef enum
{
  CXM_WLAN_RTSAR_TX_CH0_2G,
  CXM_WLAN_RTSAR_TX_CH0_5G,
  CXM_WLAN_RTSAR_TX_CH1_2G,
  CXM_WLAN_RTSAR_TX_CH1_5G,
  CXM_WLAN_RTSAR_TX_MAX
} cxm_wlan_rtsar_tx_type;


/* wwan_coex_mgr.h_END - move/rename when ready */


/* Enumeration to indicate different Procedures of Higher layers */
typedef enum
{
  CXM_PROC_NONE      = -1,
  CXM_PROC_ACQ_END   = 0,
  CXM_PROC_ACQ_START = 1,
  CXM_PROC_MAX
} cxm_mmcp_ind_e;

/*! @brief Message used by CXM to get Higher Layer Procedure */
typedef struct
{
  /* Subs Info */
  sys_modem_as_id_e_type   subs_id;

  /* CXM update per sub */
  cxm_mmcp_ind_e           procedure;
}cxm_mmcp_ind_input_s;

/*=============================================================================

                       MESSAGE DATA TYPES/STRUCTURES

=============================================================================*/
/*! @brief Message various WWAN tech's to send info to CxM 
  Note: This structure should be memset to 0 before populating the
        needed fields as this will ensure that CXM can identify
        if a new update is unique from previous one*/
typedef struct
{
  msgr_hdr_struct_type msg_hdr;   /*!< msgr header containing msg_id */
  cxm_tech_type        tech_id;   /*!< WWAN Technology */
  cxm_tech_data_s      tech_data; /*!< WWAN state/parameters info */
}cxm_wwan_tech_state_ind_s;

/*! @brief Message for CxM to request WWAN tech to resend their state */
typedef struct
{
  msgr_hdr_struct_type msg_hdr; /*!< msgr header containing msg_id */
}cxm_wwan_tech_state_update_req_s;

/* wwan_coex_mgr.h_START - move/rename when ready */
/*! @brief STX Pwr Indication */
typedef struct
{
  msgr_hdr_struct_type hdr;          /*!< msgr header containing msg_id */
  cxm_tech_type        tech_id;      /*!< WWAN Technology */
  int16                num_carriers; /*!< Number of Uplink Carriers */
  union
  {
    struct
    {
      int16                rx_pwr;     /*!< Rx power */
      int16                tx_pwr;     /*!< Tx power */
      cxm_pwr_type         valid_pwrs; /*!< Valid entries in the struct */
    };
    struct
    {
      int16                rx_pwr;     /*!< Rx power */
      int16                tx_pwr;     /*!< Tx power */
      cxm_pwr_type         valid_pwrs; /*!< Valid entries in the struct */
    } carriers[CXM_MAX_CA_SUPPORTED];
  };
}cxm_stx_set_pwr_ind_s;

/*! @brief STX Tech State Msgr Indication */
typedef struct
{
  msgr_hdr_struct_type hdr;      /*!< msgr header containing msg_id */
  cxm_tech_type        tech_id;  /*!< WWAN Technology */
  cxm_rfdev_type       tx_rfdev; /*!< Which RF Device is used for Tx? */
  uint32               band;     /*!< Tx'ing on which band? */
  uint32               channel;  /*!< Tx'ing on which channel? */
}cxm_stx_tech_state_ind_s;

/*! @brief STX Tech Tx off Msgr Indication */
typedef struct
{
  msgr_hdr_struct_type hdr;     /*!< msgr header containing msg_id */
  cxm_tech_type        tech_id; /*!< WWAN Technology */
}cxm_stx_tech_txoff_ind_s;

/*! @brief STX Tech State Channel List Msgr Indication */
typedef struct
{
  msgr_hdr_struct_type hdr;          /*!< msgr header containing msg_id */
  cxm_tech_type        tech_id;      /*!< WWAN Technology */
  cxm_rfdev_type       tx_rfdev;     /*!< Which RF Device is used for Tx? */
  uint32               band;         /*!< Tx'ing on which band? */
  uint32               num_channels; /*!< Number of channels in the list (<64) */
  uint32               channels[CXM_STX_MAX_CHANNELS]; /*!< Channel list*/
}cxm_stx_tech_state_chs_ind_s;

/*! @brief WLAN Pwr Indication */
typedef struct
{
  msgr_hdr_struct_type hdr;        /*!< msgr header containing msg_id */
  struct
  {
    int16              tx_pwr_2_4_ghz_dbm;     /*!< Tx power */
    int16              tx_pwr_5_0_ghz_dbm;     /*!< Tx power */
  } tx_chain[CXM_MAX_WLAN_TX_CHAINS];
} cxm_wlan_rtsar_pwr_report_s;

/*! @brief WLAN Pwr limit Structure */
typedef struct
{
  struct
  {
    int16   sar_plimit_2_4_ghz_band;     /*!< Tx power limit for 2.4 GHz band */
    int16   sar_plimit_5_0_ghz_band;     /*!< Tx power limit for 5.0 GHz band*/
  } tx_chain[CXM_MAX_WLAN_TX_CHAINS];
} cxm_wlan_rtsar_pwr_limit_t;

/*! @brief WLAN Pwr limit Indication */
typedef struct
{
  msgr_hdr_struct_type  hdr;        /*!< msgr header containing msg_id */
  cxm_wlan_rtsar_pwr_limit_t mimo_limits;     /*!< Tx power limit for single band & DBS case*/
  cxm_wlan_rtsar_pwr_limit_t non_mimo_limits; /*!< Tx power limit for MIMO case */
} cxm_wlan_rtsar_pwr_limit_s;


/*! @brief WLAN RT-SAR Initial Configuration message */

typedef struct
{
  msgr_hdr_struct_type  hdr;        /*!< msgr header containing msg_id */
  cxm_wlan_rtsar_msg_e  type;
  uint16                reporting_period_msec;
} cxm_wlan_rtsar_cfg_msg_s;

/*! @brief WLAN RT-SAR handshake message used in both directions               
*  MCS -> WLAN  - UMID WLAN_RTSAR_MSG_ID */ 
typedef struct
{
  msgr_hdr_struct_type  hdr;        /*!< msgr header containing msg_id */
  cxm_wlan_rtsar_msg_e  type;
} cxm_wlan_rtsar_msg_s;

typedef cxm_wlan_rtsar_msg_s cxm_mcs_rtsar_msg_s;

/*! @brief Message used by CXM to handle CXM signals as MSGR msg */
typedef struct
{
  msgr_hdr_struct_type       msg_hdr;      /*!< msgr header containing msg_id */
  cxm_coex_signal_handler_e  coex_signal;  /*!< Signal to be handled as msg */
}cxm_coex_signal_handler_ind_s;


/*! @brief Message used by CXM to handle CM operating mode */
typedef struct
{
  msgr_hdr_struct_type       msg_hdr;     /*!< MSGR header containing msg_id */
  sys_oprt_mode_e_type       oprt_mode;   /*!< Operating mode recevied */
}cxm_oprt_mode_info_t;

/* wwan_coex_mgr.h_END - move/rename when ready */

/*=============================================================================

                        MSGR UMID Definitions

=============================================================================*/

/*! @brief These are multi-purpose MSGR messages used for Simultaneous-Tech SAR limiting,
   WWAN - WLAN Coexistence mitigation and WWAN - WWAN Coexistence mitigation
 */
enum 
{
  /* These messages are used for overall Coexistence mitigation
   */

  /*! Used by specific WWAN tech to send out it's latest state/parameter info set */
  MSGR_DEFINE_UMID( MCS, CXM, IND, WWAN_TECH_STATE,
                    CXM_WWAN_TECH_STATE_ID, cxm_wwan_tech_state_ind_s ), 
  
  /*! Used by CxM to request (all registered) WWAN techs to resend their current state */
  MSGR_DEFINE_UMID( MCS, CXM, REQ, WWAN_TECH_STATE_UPDATE,
                    CXM_WWAN_TECH_STATE_UPDATE_ID, cxm_wwan_tech_state_update_req_s),

/* wwan_coex_mgr.h_START - move/rename when ready */
  /* These indications are used for SAR Power limiting & WWAN - WWAN Coexistence mitigation
   */

  /* Technology Power Indication ( Tech L1 to MCS )
   * Contains average UL and DL Power information. This message is expected 
   * periodically every TBD secs. 
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, STX_SET_POWER,
                    STX_SET_POWER_ID, cxm_stx_set_pwr_ind_s ),


  /* Technology Set Tx State Indication 
     Expected in the following cases:
     a. Tech enters system access or call originiation state
     b. Tech changes Tx band and/or channel during transmission
     c. Tech changes Tx RF device during transmission
   
     NOTE: To be deprecated once all techs move to UMID MCS_CXM_WWAN_TECH_STATE_IND.
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, STX_SET_TX_STATE,
                    STX_SET_TX_STATE_ID, cxm_stx_tech_state_ind_s ),

  /* Tech Clear Tx State Indication
     Expected when tech exists connected state and Tx device is off
   
     NOTE: To be deprecated once all techs move to UMID MCS_CXM_WWAN_TECH_STATE_IND.
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, STX_CLR_TX_STATE,
                    STX_CLR_TX_STATE_ID, cxm_stx_tech_txoff_ind_s ),

  /* Technology Set Tx State With Channels Indication 
     Similar to the Tehnology State Tx State Indication with provision for user to 
     specify a list of channels.
   
     NOTE: To be deprecated once all techs move to UMID MCS_CXM_WWAN_TECH_STATE_IND.
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, STX_SET_TX_STATE_WITH_CHS,
                    STX_SET_TX_STATE_WITH_CHS_ID, cxm_stx_tech_state_chs_ind_s ),

  /* WLAN Power Report Indication ( WLAN to MCS )
   * Contains average UL Power information. This message is expected 
   * periodically every TBD secs. 
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, WLAN_RTSAR_PWR,
                    WLAN_RTSAR_PWR_ID, cxm_wlan_rtsar_pwr_report_s ),

   /* WLAN Power Limit Indication ( MCS to WLAN )
   * Contains average UL Power Limit information. This message is expected 
   * periodically every TBD secs. 
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, WLAN_RTSAR_PLIMIT,
                    WLAN_RTSAR_PLIMIT_ID, cxm_wlan_rtsar_pwr_limit_s ),
  
  /* WLAN Init Configuration message ( MCS to WLAN )
   * Contains initial UL Power Limit information. This message is expected 
   * once - everytime RTSAR is enabled on WLAN 
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, WLAN_RTSAR_CFG_MSG,
                    WLAN_RTSAR_CFG_MSG_ID, cxm_wlan_rtsar_cfg_msg_s ),

  /* WLAN Handshake message ( WLAN -> MCS )
   * Contains handshake status between WLAN and MCS for RT-SAR state.
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, MCS_RTSAR_MSG,
                    MCS_RTSAR_MSG_ID, cxm_mcs_rtsar_msg_s ),

  /* WLAN Handshake message ( MCS -> WLAN )
   * Contains handshake status between WLAN and MCS for RT-SAR state.
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, WLAN_RTSAR_MSG,
                    WLAN_RTSAR_MSG_ID, cxm_wlan_rtsar_msg_s ),

  /* To handle COEX signals as MSGR msg ( MCS -> MCS )
   * Contains CXM signal info that should be handled as MSGR msg
   */
  MSGR_DEFINE_UMID( MCS, CXM, IND, COEX_SIGNAL_HANDLER,
                    COEX_SIGNAL_HANDLER_ID, cxm_coex_signal_handler_ind_s ),

  /*! Used by specific WWAN tech to send out it's latest state/parameter info set */
  MSGR_DEFINE_UMID( MCS, CXM, IND, WLAN_RTSAR_CONN_INFO,
                    WLAN_RTSAR_CONN_INFO_ID, cxm_wlan_conn_info_t ), 

  /*! Used by specific WWAN tech to send out it's latest state/parameter info set */
  MSGR_DEFINE_UMID( MCS, CXM, IND, SYS_OPRT_MODE,
                    COEX_OPRT_MODE_ID, cxm_oprt_mode_info_t),

/* wwan_coex_mgr.h_END - move/rename when ready */

};

#endif /* __CXM_H__ */
