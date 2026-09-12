/*!
    @file
    lte_ml1_qsh_ext.h
    
    @brief
    QSH frame work for live event/metrics and dump

    @detail
*/

/*===========================================================================

    Copyright (c) 2008-2015 Qualcomm Technologies, Inc. All Rights Reserved

    Qualcomm Technologies, Inc. Proprietary

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

$Header: //components/rel/lte.mpss/6.6.1/api/lml1_qsh_ext.h#2 $

when         who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/
#ifndef _LML1_QSH_EXT_H_
#define _LML1_QSH_EXT_H_

#include <qsh.h>
#include "lte_l1_ftm.h"
#include "lml1_qsh_mini_dump_ext.h"

/*****************************************************************************
 *
 * QSH version
 *
 ****************************************************************************/
#define LML1_QSH_MAJOR_VER 1
#define LML1_QSH_MINOR_VER 0

/*****************************************************************************
 *
 * QSH Metrics
 *
 ****************************************************************************/
 
#define LML1_QSH_ANT_NUM      2
#define LML1_QSH_DL_NUM_CARRIERS  2
 
typedef enum 
{
  CC_DISABLE = 0,
  CC_CONFIGURED = 1,
  CC_ENABLED = 2
} cc_status_e;

typedef enum
{
//[EVENT NAME]                  = [ENUM VALUE],    // [SUPPORTED] [DESCRIPTION]
  LML1_QSH_EVENT_SERVING_RSRP_DELTA       =  0,    // [x] RSRP difference for 2 antenna is more than 20 dB
  LML1_QSH_EVENT_RX_HIGH_TIMING_ERROR     =  1,    // [x] Timing error
  LML1_QSH_EVENT_PAGE_DEC_FAIL            =  2,    // [x] Page decode fail
  LML1_QSH_EVENT_PAGE_MISS                =  3,    // [x] Page miss
  LML1_QSH_EVENT_ASDIV_SWITCH             =  4,    // [x] ASDIV ping pongs
  LML1_QSH_EVENT_RACH_START               =  5,    // [x] RACH event
  LML1_QSH_EVENT_ACQ_BSCAN                =  6,    // [x] Band scan event
  LML1_QSH_EVENT_HIGH_BLER                =  7,    // [x] High BLER
  LML1_QSH_EVENT_CQI_VARIATION            =  8,    // [x] Major CQI variations
  LML1_QSH_EVENT_RSSI_FLUCTUATION         =  9,    // [x] RSSI fluctuations  
  LML1_QSH_EVENT_LOCK_WAIT_TIMEOUT        =  10,   // [x] RF chain hogging by one client affecting the other
  LML1_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE  =  11,   // [x] Common event for any failure case
  LML1_QSH_EVENT_TUNE_AWAY                =  12,   // [ ] Tune away 
  LML1_QSH_EVENT_CONFLICT_CHECK           =  13,   // [ ] Conflict check
  LML1_QSH_EVENT_SCC                      =  14,   // [ ] SCC en/dis or hw rel/acq
  LML1_QSH_EVENT_DIV                      =  15,   // [ ] Div related events (rf/fw div en/dis)
  LML1_QSH_EVENT_HORXD                    =  16,   // [ ] HORXD enable/disable event
  LML1_QSH_EVENT_4X4MIMO                  =  17,   // [ ] 4x4 MIMO enable/disable event
  LML1_QSH_EVENT_TOTAL_BW                 =  18,   // [ ] Total Aggregated BW change event
  LML1_QSH_EVENT_256QAM                   =  19,   // [ ] Total Aggregated BW change event
  LML1_QSH_EVENT_PBCH_START               =  20,   // [ ] PBCH start event
  LML1_QSH_EVENT_PBCH_STOP                =  21,   // [ ] PBCH stop event
  LML1_QSH_EVENT_AGAP_START               =  22,   // [ ] AGAP start event
  LML1_QSH_EVENT_AGAP_STOP                =  23,   // [ ] AGAP stop event
  LML1_QSH_EVENT_SCC_ACT                  =  24,   // [ ] SCC Activation triggered event
  LML1_QSH_EVENT_SCC_DEACT                =  25,   // [ ] SCC deactivation triggered event
  LML1_QSH_EVENT_SCC_ADDITION             =  26,   // [ ] SCC addition triggered event
  LML1_QSH_EVENT_SCC_DELETION             =  27,   // [ ] SCC deletion triggered event
  LML1_QSH_EVENT_SI_MOD_PERIOD_START      =  28,   // [ ] SI MOD period start event
  LML1_QSH_EVENT_DLGAP_START              =  29,   // [ ] DL GAP start event
  LML1_QSH_EVENT_CSWITCH_START            =  30,   // [ ] CSWITCH start event
  LML1_QSH_EVENT_LTA_START                =  31,   // [ ] LTA start
  LML1_QSH_EVENT_LTA_END                  =  32,   // [ ] LTA end
  LML1_QSH_EVENT_QTA_START                =  33,   // [ ] QTA start
  LML1_QSH_EVENT_QTA_END                  =  34,   // [ ] QTA end
  LML1_QSH_EVENT_DTA_START                =  35,   // [ ] DTA start
  LML1_QSH_EVENT_DTA_END                  =  36,   // [ ] DTA end
  LML1_QSH_EVENT_CGI_START                =  37,   // [ ] CGI Start
  LML1_QSH_EVENT_CDRX_START               =  38,   // [ ] CDRX start
  LML1_QSH_EVENT_CDRX_END                 =  39,   // [ ] CDRX stop
  LML1_QSH_EVENT_vRLF_RLM                 =  40,   // [ ] vRLF declared by RLM.
  LML1_QSH_EVENT_IDLE_DRX_WAKEUP          =  41,   // [ ] Idle DRX wakeup event
  LML1_QSH_EVENT_GAP_ALLOC_IND            =  42,   // [ ] Gap alloc ind, 15ms before OTA gap start
  LML1_QSH_EVENT_PRE_GAP_IND              =  43,   // [ ] Pre Gap ind, 3ms before OTA gap start
  LML1_QSH_EVENT_GAP_START_IND            =  44,   // [ ] OTA gap start  
  LML1_QSH_EVENT_GAP_END_IND              =  45,   // [ ] OTA gap end  
  LML1_QSH_EVENT_MEAS_START_DURING_CDRX   =  46,   // [ ] MEAS start during during CDRX OFF
  LML1_QSH_EVENT_WAKEUP_START             =  47,   // [ ] Wakeup Procedure Start  
  LML1_QSH_EVENT_PANIC_SEARCH             =  48,   // [ ] Panic search starts
  LML1_QSH_EVENT_PAGE_DECODE_START        =  49,   // [ ] This Page Decode start event is triggered 2ms before OTA
  LML1_QSH_EVENT_CC_INFO                  =  50,   // [x] CC status change event
  LML1_QSH_EVENT_TAM_SPV_TIMER_EXPIRY     =  51,   // SPV timer expiry event
  LML1_QSH_EVENT_CQI_TRANSMISSION         =  52,   // Report periodic and aperiodic CQI transmissins
  LML1_QSH_EVENT_WAKEUP_DONE_IND          =  53,   // wakeup done indicatoin
  LML1_QSH_EVENT_ARD_FORCE_RX             =  54,   // ARD Force Rx event
  LML1_QSH_EVENT_UE_MSG1                  =  55,   //MSG1 send event
  LML1_QSH_EVENT_SR_REACHING_DSR_TRANSMAX =  56,   //MSG1 send event 
  LML1_QSH_EVENT_DOPPLER_IND              =  57,   // PCC Doppler measurement status report 
  LML1_QSH_EVENT_LOW_MEM_DUP_TB_TRANS     =  58,   // This event is report when leaving DSM low memory condition
  LML1_QSH_EVENT_HIGH_CPU_DUP_TB_TRANS    =  59,   // This event is report from CFM high until CFM low (x %)
  LML1_QSH_EVENT_HIGH_CPU_DOWNGRADE_RI_NUM = 60,   // This event is report when leaving HORXD downgrade due to FC
  LML1_QSH_EVENT_LOW_MEM_CON_HIGH_CPU_DUP_TB_TRANS  = 61, //This event is report when leaving concurrent low memory condition and the high CPU condition
  LML1_QSH_EVENT_START_RACH_FAIL          =  62,   // Start Rach Fail Event
  LML1_QSH_EVENT_DLM_NUM_LAYER_CHANGE     =  63,
  LML1_QSH_EVENT_SCC_DEACT_SRCH_REQ       =  64,
  LML1_QSH_EVENT_SCC_DEACT_MEAS_REQ       =  65,
  LML1_QSH_EVENT_CRSIC_ENABLE             =  66,
  LML1_QSH_EVENT_QICE_ENABLE              =  67,
  LML1_QSH_EVENT_SHORT_TO_LONG_DRX_CYCLE  =  68,
  LML1_QSH_EVENT_CPHY_ABORT               =  69,
  LML1_QSH_EVENT_OFFLINE_WAKEUP           =  70,   // Sleepmgr sleep wakeup type offline
  LML1_QSH_EVENT_ONLINE_WAKEUP            =  71,
  LML1_QSH_EVENT_SR_REQ_FROM_MAC          =  72,   // GM Received SR Req from MAC
  LML1_QSH_EVENT_RACH_MSG3                =  73,   //RACH MSG3 event
  LML1_QSH_EVENT_SWQTA_W2L_START          =  74,   //SWQTA W2L Measurement
 
  LML1_QSH_EVENT_LPL_QTA_X2L_START        =  78,   // LpL QTA X2L Start Event

  LML1_QSH_EVENT_MAX_EVENT
}lml1_qsh_event_e;

/*
 * Enum for metrics
 */
typedef enum 
{
  LML1_QSH_METRIC_RSRP,
  LML1_QSH_METRIC_RSSI,
  LML1_QSH_METRIC_SINR,
  LML1_QSH_METRIC_BLER,
  LML1_QSH_METRIC_MCS,
  LML1_QSH_METRIC_RACH_PWR,
  LML1_QSH_METRIC_NON_RACH_PWR,
  LML1_QSH_METRIC_GRANT,
  LML1_QSH_METRIC_EVENT,
  LML1_QSH_METRIC_RRC_MSG,
  LML1_QSH_METRIC_RSRQ,
  LML1_QSH_METRIC_ANT_RF_DATA,
  LML1_QSH_METRIC_UL_BLER,
  LML1_QSH_METRIC_DL_CC_LAYERS,
  LML1_QSH_METRIC_CC_INFO,
  LML1_QSH_METRIC_MAX
} lml1_qsh_metric_e;

/* 
 * LTE ML1 runtime metrics
 */
typedef struct
{
  qsh_metric_hdr_s             hdr;
  int32                        rsrp_avg; /* Avg RSRP within sampling period */
} lml1_qsh_metric_rsrp_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  int32                        rssi_avg; /* Avg RSSI within sampling period */
} lml1_qsh_metric_rssi_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  int32                        sinr_avg; /* Avg RSSI within sampling period */
} lml1_qsh_metric_sinr_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  uint32                       tb_count; /* Accumulated BLER within sampling period */
  uint32                       tb_err_count;
} lml1_qsh_metric_bler_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  uint8                        ul_mcs; /* Avg UL MCS within sampling period */
} lml1_qsh_metric_mcs_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  int8                         avg_rach_pwr; /* Avg Rach power in dbm sampling period */
} lml1_qsh_metric_rach_tx_power_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  int8                         avg_non_rach_pwr; /* Avg Rach power in dbm sampling period */
} lml1_qsh_metric_non_rach_tx_power_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  uint8                        ul_rb;  /* Avg #RBs in grant within sampling period */
} lml1_qsh_metric_grant_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  int32                        rsrq_avg; /* Avg RSRQ within sampling period */
} lml1_qsh_metric_rsrq_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  uint32                       dl_cc_max_layer[LML1_QSH_DL_NUM_CARRIERS];   /* max number of layers per CC */
} lml1_qsh_metric_dl_cc_layers_s;
typedef struct
{
  uint8                         cc_idx;     /* CC index */
  boolean                       activated;  /* CC activated state */
  uint8                         cc_bw_mhz;  /* CC BW in MHz */
} lte_ml1_qsh_metric_cc_data_s;

typedef struct
{
  qsh_metric_hdr_s              hdr;
  lte_ml1_qsh_metric_cc_data_s  data_per_cc[LML1_QSH_DL_NUM_CARRIERS];   /* status per CC */
} lml1_qsh_metric_cc_info_s;

typedef struct
{
  qsh_metric_hdr_s             hdr;
  uint32                       total_tx_cnt; /* Total Harq ack/nack */
  uint32                       tx_nack;      /* Harq nack */
} lml1_qsh_metric_ul_bler_s;

/* 
 * LTE ML1 runtime events
 */
typedef struct 
{
  qsh_metric_hdr_s    hdr;
  uint32              earfcn;         /* EARFCN/ UARFCN */
  uint16              pci;            /* PCI/PSC of Serving Cell */
  uint32              bw;             /* Total Bandwidth for LTE */
  int16               tdd_config;     /* LTE UL/DL Configuration */
  cc_status_e         cc_status;      /* Current Carrier Agg Status */
} lml1_qsh_metric_event_s;

/*
 * LTE ML1 RRC message
 */
typedef struct
{
  uint8      status;
} lml1_qsh_metric_cphy_start_cnf_s;


typedef struct
{
  uint8      status;
} lml1_qsh_metric_cphy_stop_cnf_s;

typedef struct
{
  uint8            status;
  uint8            suspend_cause;
}lml1_qsh_metric_cphy_suspend_cnf_s;

typedef struct
{
  uint8            status;
  uint8            cause;
}lml1_qsh_metric_cphy_resume_cnf_s;

typedef struct
{
  uint8      status;
  uint32     earfcn;
  uint16     phy_cell_id;
} lml1_qsh_metric_cphy_cell_select_cnf_s;

typedef struct
{
  uint8      status;
  uint16     common_cfg_validity;
} lml1_qsh_metric_cphy_common_cfg_cnf_s;

typedef struct
{
  uint8                       status;
  boolean                     is_acq_needed;
} lml1_qsh_metric_cphy_con_release_cnf_s;

typedef struct
{
  uint8      status;
  uint32     dedicated_cfg_validity;
} lml1_qsh_metric_cphy_dedicated_cfg_cnf_s;

typedef struct
{
  uint8      status;
  uint16 com_mob_parm_validity;
  uint32 dedicated_parm_validity;
} lml1_qsh_metric_cphy_handover_cnf_s;

typedef struct
{
  uint8 status;
} lml1_qsh_metric_cphy_idle_meas_cfg_cnf_s;

typedef struct
{
  uint8 status;
} lml1_qsh_metric_cphy_conn_meas_cfg_cnf_s;

typedef struct
{
  boolean result;
  uint8 scell_id;
} lml1_qsh_metric_cphy_start_rach_cnf_s;

typedef struct
{
  uint8 trans_id;
  uint8 status;
  uint32 earfcn;
  boolean cell_barred;
} lml1_qsh_metric_cphy_acq_cnf_s;

typedef struct
{
  uint8 rlf_reason;
} lml1_qsh_metric_cphy_rl_failure_ind_s;



typedef struct 
{
   qsh_metric_hdr_s            hdr;
   /*! UMID*/
   uint32 umid;
   /*! UMID Info*/
   union
   {
     lml1_qsh_metric_cphy_start_cnf_s       cphy_start_cnf;       /* LTE_CPHY_START_CNF */
     lml1_qsh_metric_cphy_stop_cnf_s        cphy_stop_cnf;        /* LTE_CPHY_STOP_CNF */
     lml1_qsh_metric_cphy_suspend_cnf_s     cphy_suspend_cnf;     /* LTE_CPHY_SUSPEND_CNF */
     lml1_qsh_metric_cphy_resume_cnf_s      cphy_resume_cnf;    /* LTE_CPHY_RESUME_CNF */
     lml1_qsh_metric_cphy_cell_select_cnf_s cphy_cell_select_cnf;  /* LTE_CPHY_CELL_SELECT_CNF */
     lml1_qsh_metric_cphy_common_cfg_cnf_s  cphy_common_cfg_cnf;   /* LTE_CPHY_COMMON_CFG_CNF */
     lml1_qsh_metric_cphy_con_release_cnf_s cphy_con_release_cnf;  /* LTE_CPHY_CON_RELEASE_CNF */
     lml1_qsh_metric_cphy_dedicated_cfg_cnf_s cphy_dedicated_cfg_cnf;  /* LTE_CPHY_DEDICATED_CFG_CNF */
     lml1_qsh_metric_cphy_handover_cnf_s    cphy_handover_cnf;         /* LTE_CPHY_HANDOVER_CNF */
     lml1_qsh_metric_cphy_idle_meas_cfg_cnf_s cphy_idle_meas_cfg_cnf;  /* LTE_CPHY_IDLE_MEAS_CFG_CNF */
     lml1_qsh_metric_cphy_conn_meas_cfg_cnf_s cphy_conn_meas_cfg_cnf;  /* LTE_CPHY_CONN_MEAS_CFG_CNF */
     lml1_qsh_metric_cphy_start_rach_cnf_s  cphy_start_rach_cnf;       /* LTE_CPHY_START_RACH_CNF */
     lml1_qsh_metric_cphy_acq_cnf_s         cphy_acq_cnf;              /* LTE_CPHY_ACQ_CNF */
     lml1_qsh_metric_cphy_rl_failure_ind_s  cphy_rl_failure_ind;       /* LTE_CPHY_RL_FAILURE_IND */
   } umid_info_u;   
} lml1_qsh_metrics_rrc_msg_s; 

/*
 * Antenna rf data
 */
typedef struct
{
  qsh_metric_hdr_s             hdr;
  int32                        rssi[LML1_QSH_ANT_NUM];
  int32                        rsrp[LML1_QSH_ANT_NUM];
  int32                        rsrq[LML1_QSH_ANT_NUM];
  int32                        sinr[LML1_QSH_ANT_NUM];
}lml1_qsh_metric_ant_rf_data_s;

/*****************************************************************************
 *
 * QSH Dump
 *
 ****************************************************************************/
/*
 * Enum for dump
 */
typedef enum 
{
  LML1_QSH_DUMP_TAG_MGR,
  LML1_QSH_DUMP_TAG_DLM,
  LML1_QSH_DUMP_TAG_ULM,
  LML1_QSH_DUMP_TAG_GM,
  LML1_QSH_DUMP_TAG_SM_CONN,
  LML1_QSH_DUMP_TAG_SM_IDLE,
  LML1_QSH_DUMP_TAG_GAPMGR,
  LML1_QSH_DUMP_TAG_RFMGR,
  LML1_QSH_DUMP_TAG_IRAT,
  LML1_QSH_DUMP_TAG_SCHDLR,
  LML1_QSH_DUMP_TAG_MSG,
  LML1_QSH_DUMP_TAG_SLEEPMGR,
  LML1_QSH_DUMP_TAG_MCLK,
  LML1_QSH_DUMP_TAG_MAX
} lml1_qsh_dump_tag_e;
#if defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH )
typedef enum
{
  LML1_QSH_EVENT_NOTIFY_RLF_OOS,
  LML1_QSH_EVENT_NOTIFY_SERV_ATTEN,
  LML1_QSH_EVENT_NOTIFY_NBR_ATTEN, 
  LML1_QSH_EVENT_NOTIFY_MAX  	
}lml1_qsh_event_notify_e;
#endif
/*
 * LTE ML1 minidump (mini set)
 */
typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t MGR_SM;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_manager_data_s manager_data;
  uint8                                  ta_category_index; 
}lml1_qsh_dump_tag_mgr_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t   ULM_SM;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_ulm_s         ml1_ulm;
}lml1_qsh_dump_tag_ulm_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t   GM_SM;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_data_s     gm_db;
}lml1_qsh_dump_tag_gm_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t   GAPMGR_STM;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_data_s gapmgr_data;
} lml1_qsh_dump_tag_gapmgr_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t RFMGR_STM;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_rfmgr_trm_s rfmgr_trm;
}lml1_qsh_dump_tag_rfmgr_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t SCHDLR2_STM;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_mode_data_s schdlr_mode;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_systime_info_s schdlr_systime;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_buffer_s  schdlr_log;
}lml1_qsh_dump_tag_schdlr_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_debug_log_s mgr_gm_msg_log;
}lml1_qsh_dump_tag_msg_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t DLM_SM;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_rx_cfg_proc_status_s rx_cfg_proc_status;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_ca_data_s            ca_data;
} lml1_qsh_dump_tag_dlm_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t SM_CON_STM;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t SM_CONN_TL_MGR_STM;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_tl_mgr_stm_s tl_mgr_stm;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_pbch_mgr_stm_s pbch_mgr_stm;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_meas_inter_freq_meas_s  inter_freq_meas;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_inter_freq_stm_info_s    inter_freq_stm;
} lml1_qsh_dump_tag_sm_conn_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  int32       curr_op_mode;
  union 
  {
    struct 
    {
      lte_ml1_qsh_dump_tag_mini_stm_state_machine_t  ACQ_STM;
      int32                                config_request;
      uint32                               last_acq_earfcn;
      int32                                acq_rx_cfg_req_type;
      int32                                prev_state;
      lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_acq_pbch_info_s pbch_info;
      lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_acq_s     sm_acq_data;
    } acq_data;
    struct
    {
      lte_ml1_qsh_dump_tag_mini_stm_state_machine_t  IDLE_STM;    
      lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_idle_s    sm_idle;
      lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_idle_lte_search_meas_pbch_info_s meas_pbch_info;
      uint16                                         paging_cycle;
      lte_ml1_qsh_dump_tag_mini_lte_ml1_mdb_cell_struct_s serving_cell;      
    } idle_data;
  } stm;
} lml1_qsh_dump_tag_sm_idle_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_irat_meas_s    irat_meas_s;
} lml1_qsh_dump_tag_irat_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sleepmgr_t        sleepmgr_data;
  lte_ml1_qsh_dump_tag_mini_stm_state_machine_t       SLEEPMGR_STM;
} lml1_qsh_dump_tag_sleepmgr_s;

typedef PACK(struct)
{
  qsh_dump_tag_hdr_s hdr;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mcvs_ctrl_s     mcvs;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mcpm_s          mcpm;
} lml1_qsh_dump_tag_mclk_s;
#endif /* _LML1_QSH_EXT_H_ */
