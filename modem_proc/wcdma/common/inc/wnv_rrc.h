/*============================================================================
                            W N V _ R R C . H
DESCRIPTION

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2014-2014 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/common/inc/wnv_rrc.h#12 $
$DateTime: 2024/09/20 01:56:28 $
$Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
07/22/20   ha      Enable HI AC even when LO AC absen through NV
12/15/21    ha     FR53313 : Requirement to have configurable pseudo meas defer timer First time entering to DCH through NV
05/21/20    gc      Made changes to block FRLTE for specific PLMNs
18/5/20    gcs     Added NV support to disable/enable feature FRLTE for LTOW CSFB due to supplementary service and Made changes to trigger FRLTE only if NV is enabled  
04/20/20    gcs     FR59710: Made changes to delay FRLTE and WtoL Resel for MAV
06/24/19    gcs     Made changes for FR 57381
11/20/18   ak      Made changes for FR 53313
08/08/18    gc      Made Changes to handle W-Pref Fallback in case of persistent collision
02/23/17    sas     Changes to have NV gaurd to control the Truncated/Full CA feature.
09/27/17    sas     FE-FACH feature enabling through NV for single SIM.
04/12/18   ak      Made changes to read complete sibs independent of their entry in SIBdb.
22/09/16   sg      Made change to handle back to back SMC
02/21/18    nr      Added NV for LTA during activation time
01/18/18    vs      Added NV for allowing LTA during access priority
12/27/17    nr      Made changes to add NV 73993 for modem log util task
10/21/16    sr      FR 38167: Changes to restrict the code to specific PLMN
05/03/16    sp      Made changes to have a maximum wait time for allowing LTAs despite of pending L2 acks.
03/09/16    sn      Changes for FR30881
11/06/15    nr      Made changes to disable EFACH NVs for MSIM by default
09/25/15    sp      Added NV support for MSIM DBDC band combination
07/13/15    sg      Made changes to enable DCHSUPA by default
08/20/15    sp      Enable HSRACH/EDRX for DSDS by default on thor modem
07/30/15    sp      NV changes to control l2 ack wait timer during reselection
05/14/15    ad      Made changes to enabled DB-DC feature with B1+B8 combo by default for single SIM
07/08/15    sr      TRM-U changes for NAS-RRC interface
05/22/15    vi      Modified default value of NV 73514
03/10/15    sas     NV Changes to control reading of all SIBs for every reselection irrespective of SIBDB.
02/28/15    rsr     W+W Phase 2 Checkin.
01/22/15    bc      Changes to provide NV control to allow TA for loop back call and CS Data call
02/23/15    bc      Changes to enable FR22503
02/19/15    sr      Changes to enable FR23776
02/19/15    sn      Changes to enable FR20470 
12/29/14    bc      Made changes to control the range of frequencies to be skipped around a WCDMA cell by an nv.
12/11/14    aa      Made changes to enable CPC by default with DSDA concurrency
10/16/14    bc      Changes to enable FR21174 and FR21035
11/05/14    as      Made changes to enabled HS-RACH and MAC-I features by default for single SIM
09/17/14    sg      Made changes to move all RRC nv macros to wnv_rrc.h
09/03/14    rkmk    Setting WNV_ID_RRC_GANSS_SUPPORT_NV_DEFAULT to TRUE
08/08/14    ac      Update NV defaults for rrc_dsds_concurrency, rrc_dsda_concurrency, rrc_tsts_concurrency
07/21/14    ac      Update RRC_VERSION_I_FOR_RRC_DEFAULT to 5
07/21/14    ac      Rename RRC_DSDS_CONCURRENCY_CELLFACH to DEFAULT_CONFIG_IN_CELL_FACH
07/15/14    ac      Initial checkin 
===========================================================================*/

#ifndef _WNV_RRC_H_
#define _WNV_RRC_H_

#include "comdef.h"
#include "wnv.h"
#include "msgcfg.h"


/*************************************************************************** 
                   Exported structure definitions
****************************************************************************/

#define  WNV_ID_RRC_WTOL_CM_SUPPORT_DEFAULT                   TRUE                        
#define  WNV_ID_RRC_PPAC_SUPPORT_DEFAULT                      TRUE        
#define  WNV_ID_RRC_SAVE_ACQDB_DEFAULT                        FALSE
#define  WNV_ID_RRC_DISABLE_CPC_FDPCH_DEFAULT                 0         
#define  WNV_ID_RRC_DISABLE_CRASH_DEBUG_INFO_DEFAULT          0         
#define  WNV_ID_RRC_WTOL_PS_HO_SUPPORT_DEFAULT                TRUE         
#define  WNV_ID_RRC_WTOL_TDD_PS_HO_SUPPORT_DEFAULT            TRUE           
#define  WNV_ID_RRC_IGNORE_CELL_BAR_RESERVE_STATUS_DEFAULT    0           
#define  WNV_ID_RRC_CSFB_SKIP_SIBL1_OPT_DEFAULT               0            
#define  WNV_ID_RRC_FREQ_LOCK_ITEM_DEFAULT                    0        
#define  WNV_ID_RRC_DO_NOT_FORWARD_PAGE_DEFAULT               0          
#define  WNV_ID_RRC_FREQ_SCAN_TIMER_IN_MS_DEFAULT             1800000           
#define  WNV_ID_RRC_DEEP_SLEEP_NO_SVC_TIMER47_IN_MS_DEFAULT   10000        
#define  WNV_ID_RRC_BPLMN_FREQ_SCAN_TIMER_I48N_MS_DEFAULT     900000         
#define  WNV_ID_RRC_SIB_SLEEP_SB_DEFAULT                      TRUE        
#define  WNV_ID_RRC_CONSTANT_320MS_DRX_DEFAULT                FALSE         
#define  WNV_ID_RRC_PRIORITIZE_850_BAND_DEFAULT               FALSE
#define  WNV_ID_RRC_DEVICE_TYPE_DEFAULT                       0
#define  WNV_ID_RRC_DISABLE_MEAS_DEFAULT                      0
#ifdef FEATURE_QTANG2
#define  WNC_ID_RRC_DORMANCY_ENABLE_DEFAULT                   RRC_NV_DORMANCY_DISABLED
#else
#define  WNC_ID_RRC_DORMANCY_ENABLE_DEFAULT                   RRC_NV_DORMANCY_ENABLED
#endif
#define  WNV_ID_RRC_DEFAULT_CONFIG_IN_CELL_FACH_DEFAULT       0 /*Support HS-FACH and CPC by default*/

#define WNV_ID_WCDMA_RRC_ENABLE_PSC_LOCK_DEFAULT              FALSE

#define WNV_ID_WCDMA_RRC_CSG_PRUNE_COUNTER_DEFAULT            8640
#define WNV_ID_WCDMA_CSG_SRCH_CARRIER_SPACE_DEFAULT           FALSE
#define WNV_ID_WCDMA_ASF_TIMER_DEFAULT                        0

#define WNV_ID_WCDMA_RRC_SNOW3G_ENABLED_DEFAULT               FALSE

#define WNV_ID_WCDMA_RRC_SET_SRB2_ACT_TIME_DEFAULT            0

#define WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB_DEFAULT          FALSE
#define WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB_TIMER_DEFAULT    2000

#define WNV_ID_WCDMA_RRC_FAST_RETURN_TO_LTE_AFTER_CSFB_SS_DEFAULT       TRUE
#define WNV_ID_WCDMA_BLOCK_FRLTE_ON_PLMN_LIST_COUNT_DEFAULT            0

#ifdef FEATURE_CUST_2
#define WNV_ID_WCDMA_RRC_DELAY_RETURN_TO_LTE_DEFAULT                            TRUE
#define WNV_ID_WCDMA_RRC_DELAY_RETURN_TO_LTE_TIMER_DEFAULT                      60

#else
#define WNV_ID_WCDMA_RRC_DELAY_RETURN_TO_LTE_DEFAULT                            FALSE
#define WNV_ID_WCDMA_RRC_DELAY_RETURN_TO_LTE_TIMER_DEFAULT                      0
#endif

#define WNV_ID_RRC_GANSS_SUPPORT_NV_DEFAULT                   TRUE

#define WNV_ID_RRC_FE_FACH_SUPPORT_DEFAULT                    2 /*FE_FACH_ALL_LAYERS*/

#ifdef FEATURE_DUAL_SIM
#define WNV_ID_RRC_DSDS_CONCURRENCY_EXTENDED_DEFAULT          (NV_DS_DCHSDPA_CONCURRENCY | NV_UL_COMPRESSION_CONCURRENCY | NV_DS_CPC_CONCURRENCY | NV_W_BMC_CONCURRENCY)
#define WNV_ID_RRC_DSDA_CONCURRENCY_EXTENDED_DEFAULT          (NV_DS_DCHSDPA_CONCURRENCY | NV_UL_COMPRESSION_CONCURRENCY | NV_DS_CPC_CONCURRENCY | NV_W_BMC_CONCURRENCY)
#define WNV_ID_RRC_TSTS_CONCURRENCY_EXTENDED_DEFAULT          (NV_DS_DCHSDPA_CONCURRENCY | NV_UL_COMPRESSION_CONCURRENCY | NV_W_BMC_CONCURRENCY)
#endif

#define WNV_ID_RRC_NV_ENABLE_MFBI_SUPPORT_DEFAULT             0

#define WNV_ID_RRC_UL_COMPR_CAP_SUPPORT_DEFAULT             1

#define WNV_ID_WCDMA_HSUPA_CM_CTRL_I_DEFAULT                  TRUE
#define WNV_ID_WCDMA_HSUPA_DEFAULT_CATEGORY_I_DEFAULT         8 /* UE suppports DCHSUPA */
#define WNV_ID_WCDMA_OPTIONAL_FEATURE_LIST_I_DEFAULT          ~0

#define WNV_ID_INTERRAT_NACC_SUPPORT_I_DEFAULT                TRUE
#define WNV_ID_GERAN_FEATURE_PACK_1_I_DEFAULT                 FALSE

#define WNV_ID_RRC_INTEGRITY_ENABLED_I_DEFAULT                1
#define WNV_ID_RRC_CIPHERING_ENABLED_I_DEFAULT                1
#define WNV_ID_HSDPA_COMPRESSED_MODE_ENABLED_I_DEFAULT        TRUE
#define WNV_ID_RRC_FAKE_SECURITY_ENABLED_I_DEFAULT            0
#define WNV_ID_WCDMA_RRC_PDCP_DISABLED_I_DEFAULT              TRUE
#define WNV_ID_RRC_VERSION_I_FOR_RRC_DEFAULT                  5 /* value 5 indicates Rel99/Rel5/Rel6/REL7/Rel8/Rel9 */

#define WNV_ID_WCDMA_CS_VOICE_OVER_HSPA_ENABLED_I_DEFAULT FALSE
#define WNV_ID_WCDMA_DL_FREQ_ENABLED_I_DEFAULT            FALSE
#define WNV_ID_WCDMA_DL_FREQ_I_DEFAULT                    0
/*Disabling HS_RACH DB DC feature by default HI onwards*/
#define WNV_ID_RRC_FEATURE_DEFAULT                        (RRC_EFACH_ENABLED|RRC_EFACH_DRX_ENABLED|RRC_MAC_I_ENABLED)
#define WNV_ID_RRC_SIB7_EXP_TIME_DEFAULT                  2000
#define WNV_ID_RRC_ENABLE_MEAS_ID_EXTN_DEFAULT            FALSE

#define WNV_ID_RRC_IDLE_FEATURES_DEFAULT                  0xDF

#define WNV_ID_RRC_DB_DC_BAND_COMB_CONTROL_NV_DEFAULT      (1 << (RRC_DB_DC_BAND1_BAND8-1))

#define WNV_ID_RRC_DSDS_DB_DC_BAND_COMB_CONTROL_NV_DEFAULT (1 << (RRC_DB_DC_BAND1_BAND8-1))
#define WNV_ID_RRC_DSDA_DB_DC_BAND_COMB_CONTROL_NV_DEFAULT (1 << (RRC_DB_DC_BAND1_BAND8-1))
#define WNV_ID_RRC_TSTS_DB_DC_BAND_COMB_CONTROL_NV_DEFAULT (1 << (RRC_DB_DC_BAND1_BAND8-1))

#define WNV_ID_RRC_PLMN_ID_DEFAULT                        0xF

#define WNV_ID_RRC_FREQ_SKIP_RANGE_DEFAULT                22

#define WNV_ID_ENABLE_TA_FOR_CS_DATACALL_NV_DEFAULT       FALSE

#define WNV_ID_RRC_NV_ENABLE_READ_SIBS_FOR_RESEL_DEFAULT  FALSE

#define WNV_ID_RRC_NV_ENABLE_READ_SIBS_DEFAULT  FALSE

#define WNV_ID_RRC_HALF_DRX_LEN_DEFAULT                   FALSE

#define WNV_ID_RRC_CHANGE_TRM_PRIORITY_TMR_DEFAULT        500

#define WNV_ID_RRC_NV_REQ_LTE_FOR_TRUNCATED_CA_DEFAULT    FALSE

#define WNV_ID_RRC_ENABLE_HI_ACCESS_CLASS_NV_DEFAULT 0

#define RRC_NV_DORMANCY_ENABLED 1
#define RRC_NV_LEGACY_DORMANCY_DISABLED 2
#define RRC_NV_DORMANCY_DISABLED 3
#define WNV_ID_ALLOW_LTA_TIME_DEFAULT                     2

#define WNV_ID_RRC_CELL_BAR_UPON_RACH_FAILURE_NV_DEFAULT  FALSE
#define WNV_ID_RRC_CELL_BAR_DURATION_SECS_NV_DEFAULT      20 /*secs*/

#define WNV_ID_RRC_REDIRECT_TO_LTE_AFTER_SRVCC_NV_DEFAULT FALSE
#define WNV_ID_RRC_PSEUDO_MEAS_TRIGGERS_NV_DEFAULT 0

#define RRC_PSEUDO_MEAS_TIMER_T1_DEFAULT     60

#define RRC_PSEUDO_MEAS_TIMER_T2_DEFAULT     180

#define RRC_PSEUDO_MEAS_TIMER_T3_DEFAULT      540

#define RRC_PSEUDO_MEAS_FREQ_L_SRCH_T1_DEFAULT 10

#define RRC_PSEUDO_MEAS_FREQ_L_SRCH_T2_DEFAULT 20

#define RRC_PSEUDO_MEAS_FREQ_L_SRCH_T3_DEFAULT  40

#define RRC_PSEUDO_MEAS_FREQ_L_SRCH_T4_DEFAULT  60

#define RRC_PSEUDO_MEAS_RXAGC_TMR_DEFAULT 5

#define RRC_PSEUDO_MEAS_RXAGC_THRESH_DEFAULT 13

#define WNV_ID_RRC_PSEUDO_MEAS_RSRP_DEFAULT   -100

#define WNV_ID_RRC_PSEUDO_MEAS_RSRQ_DEFAULT    -20

#define RRC_PSEUDO_MEAS_MIN_TIMER_VAL           10

#define WNV_ID_RRC_PSEUDO_MEAS_FORBIDDEN_EARFCN_TIMER_DEFAULT   3600

#define WNV_ID_RRC_PSEUDO_MEAS_DEFER_TMR_VALUE_FIRST_TIME_IN_DCH_DEFAULT   3

/*Pseudo measTrigger NV prams*/

#define RRC_ELEVATOR_ENABLED 0x1

#define RRC_LOW_THRPUT_ENABLED 0x2

/* Dual-SIM concurrency NV values */
#define NV_DS_HSFACH_CONCURRENCY        0x1
#define NV_DS_EDRX_CONCURRENCY          0x2
#define NV_DS_MAC_I_CONCURRENCY         0x4 
#define NV_DS_HSRACH_CONCURRENCY        0x8
#define NV_DS_CPC_CONCURRENCY           0x10
#define NV_DS_DCHSDPA_CONCURRENCY       0x20
#define NV_DS_DCHSUPA_CONCURRENCY       0x40
#define NV_DS_3CHSDPA_CONCURRENCY       0x80 
#define NV_DS_DBDC_HSDPA_CONCURRENCY    0x100 
#define NV_FE_FACH_CONCURRENCY          0x200
#define NV_UL_COMPRESSION_CONCURRENCY   0x400
#define NV_W_BMC_CONCURRENCY            0x800

/* RRC feature NV values */
#define RRC_EFACH_ENABLED                  0x0001
#define RRC_EFACH_DRX_ENABLED              0x0004
#define RRC_HSRACH_ENABLED                 0x0008
#define RRC_MAC_I_ENABLED                  0x0010
#define RRC_SIB5_DELAY_CELL_UPDATE_ENABLED 0x0020
#define RRC_DB_DC_HSDPA_ENABLED            0x0040
#define RRC_EDPCCH_PWR_BOOST_ENABLED       0x0080
#define RRC_SB_3C_BANDII_SUPPORTED         0x0100 /* Single band 3 carrier on band II is supported */
/* RRC CPC disable NV values */
#define NV_FDPCH_DISABLE          0x1
#define NV_EFDPCH_DISABLE         0x2
#define NV_CPC_DTX_DISABLE        0x4

/* CRASH DEBUG disable NV */
#define NV_CRASH_DEBUG_INFO_DISABLE 0x1

#define MAX_UE_MODES 3
#define WRRC_DSDS_MODE 0
#define WRRC_DSDA_MODE 1
#define WRRC_TSTS_MODE 2

#define  RRC_DB_DC_BAND1_BAND8   1
#define  RRC_DB_DC_BAND2_BAND4   2
#define  RRC_DB_DC_BAND1_BAND5   3
#define  RRC_DB_DC_BAND1_BAND9   4
#define  RRC_DB_DC_BAND2_BAND5   5

#define WRRC_ENABLE_LTA_DURING_ACT_TIME 1
#define WRRC_MIN_ACT_TIME_TO_ALLOW_LTA  100

#define RRC_WAIT_TIME_FOR_L2_ACK_FOR_PCH_TRANSITION 500
#define RRC_WAIT_TIME_FOR_L2_ACK_FOR_PCH_TRANSITION_FOR_EDRX 850

/* Default value for pseudo SIB19 */
  #define WRRCNV_PSEUDO_SIB19_UTRA_PRI 3
  #define WRRCNV_PSEUDO_SIB19_S_PRI_SRCH1 13
  #define WRRCNV_PSEUDO_SIB19_THR_SERV_LOW 5
  #define WRRCNV_PSEUDO_SIB19_EUTRA_PRI 7
  #define WRRCNV_PSEUDO_SIB19_EUTRA_MEAS_BW 2 /* rrc_EUTRA_MeasurementBandwidth_mbw25 */
  #define WRRCNV_PSEUDO_SIB19_THR_X_HIGH 5
  #define WRRCNV_PSEUDO_SIB19_THR_X_LOW 5
  #define WRRCNV_PSEUDO_SIB19_EUTRA_QRXLEVMIN (-59)

  #define RRC_L2_RECOVERY_FEATURE_ENABLE 1

/* Default value for LTA optimization */
  #define ALLOW_LTA_THOUGH_L2_ACK_PENDING 1
  #define LTA_BLOCK_TIMER_1SEC 2000

#define WRRC_BUFFER_SMC_SUPPORT 1
#define WRRC_BUFFER_SMC_WAIT_TIMER  500

#define PLMN_LIST_SIZE 5

#define MCC_SIZE                                      10
#define WRRCNV_SIB19_SERVING_UTRA_PRIORITY            6
#define WRRCNV_SIB19_SERVING_UTRA_S_PRIORITYSEARCH1   4
#define WRRCNV_SIB19_SERVING_UTRA_THRESHSERVINGLOW    0
#define WRRCNV_SIB19_GSM_PRIORITY                     7
#define WRRCNV_SIB19_GSM_THRESHX_HIGH                 3
#define WRRCNV_SIB19_GSM_THRESHX_LOW                  1
#define WRRCNV_SIB19_GSM_Q_RXLEVMIN                  (-53)
#define WRRCNV_SIB19_WCDMA_THRESHX_HIGH               30
#define WRRCNV_SIB19_WCDMA_THRESHX_LOW                10
#define WRRCNV_SIB19_WCDMA_Q_QUALMIN_FDD             (-24)
#define WRRCNV_SIB19_WCDMA_Q_RXLEVMIN_FDD            (-119)

#define MCC_LIST_SIZE 5

/* Band priority configuration structure. */
typedef PACKED struct
{
  uint32 band_priority_list[10];	 /* List of Bands*/
} rrc_nv_band_priority_config_type;

typedef PACKED struct PACKED_POST
{
  uint8 enable_lta_during_act_time;	 /* Enable-1 Disable-0 */
  uint16 minimum_act_time_required_to_allow_lta; 
} wcdma_rrc_support_lta_type;

/* Defer cel resel while waiting for l2 ack. First element for normal case second element for edrx case*/
typedef PACKED struct
{
  uint16 legacy_cell_nv_value;	 /* Legacy cell nv value*/
  uint16 edrx_cell_nv_value;	 /* EDRX cell nv value*/
} rrc_nv_defer_cel_resel_waiting_for_l2_ack_type;

typedef PACKED struct PACKED_POST
{
  uint8 enable_smc_buffer;
  uint16 smc_hold_timer;
}rrc_smc_buffer_support_type;

typedef PACKED struct PACKED_POST{ 
  
  /* UTRA FDD serving */
  uint8         utra_priority;
  uint8         s_priority_search1;
  uint8         thresh_serving_low;
  
  /* EUTRA Neighbors */
  uint8         eutra_priority;
  uint8         eutra_meas_bandwidth;
  uint8         eutra_thresh_high;
  uint8         eutra_thresh_low;
  int8          eutra_qrxlevmin;
} rrc_nv_pseudo_sib19_type;


typedef PACKED struct PACKED_POST{ 
  
  uint8  opt_enable_mask;
  uint16 block_lta_tmr;
} rrc_nv_msim_opt_type;

typedef PACKED struct PACKED_POST{ 
  uint8 count;
  uint16 mcc_list[MCC_LIST_SIZE];
}rrc_mcc_list_type;

typedef PACKED struct PACKED_POST{ 
  uint8  plmn_count;
  uint16 mcc_list[PLMN_LIST_SIZE];
  uint16 mnc_list[PLMN_LIST_SIZE];
}rrc_plmn_list_type;

typedef PACKED struct PACKED_POST{ 

  uint8         version;
  uint8         mcc_count;
  uint16        mcc[MCC_SIZE];

  /* UTRA serving */
  uint8         sib19_Serving_utra_priority;
  uint8         sib19_Serving_utra_s_PrioritySearch1;
  uint8         sib19_Serving_utra_threshServingLow;
  
  /* GSM Neighbors */
  uint8         sib19_gsm_priority;
  uint8         sib19_gsm_threshx_high;
  uint8         sib19_gsm_threshx_low;
  int8          sib19_gsm_q_rxlevmin;

  /*WCDMA Neighbours */    
  uint8         sib19_wcdma_threshx_high;
  uint8         sib19_wcdma_threshx_low;
  int16         sib19_wcdma_q_qualmin_fdd;
  int16         sib19_wcdma_q_rxlevmin_fdd;
  
  int16         reserved1;
  int16         reserved2;
} rrc_nv_gsm_fallback_info_type;

typedef PACKED struct PACKED_POST{ 
  /* Timer durations*/
  boolean   feature_enabled;
  uint16      timer_t1;
  uint16      timer_t2;
  uint16      timer_t3;
   /* Frequency of pseudo measurements*/
  uint8        freq_L_srch_t1;
  uint8        freq_L_srch_t2;
  uint8        freq_L_srch_t3;
  uint8        freq_L_srch_t4;

}rrc_nv_pseudo_meas_info_type;

typedef PACKED struct PACKED_POST{ 
  uint16           timer_rxagc ;
  int16           rxagc_threshold;

}rrc_nv_pseudo_meas_rxagc_info_type;

typedef PACKED struct PACKED_POST{
  int32          rsrp;
  int32          rsrq;

}rrc_nv_pseudo_meas_rsrp_rsrq_info_type;

typedef PACKED struct PACKED_POST{
  uint16 triggers_for_pseudo_meas;
}rrc_nv_pseudo_meas_triggers_info_type;


typedef PACKED struct PACKED_POST{ 
  boolean nv_enabled;
  uint16 timer_val;  //in seconds
}rrc_nv_delay_return_to_lte_type;

/* Placeholder for RRC NV types to make memory allocations easier */
typedef union
{
  boolean rrc_wtol_cm_support_nv;
  boolean rrc_ppac_support_nv;                                       
  boolean rrc_save_acqdb_nv;                                         
  rrc_nv_band_priority_config_type  rrc_nv_band_priority_config;    
  wcdma_rrc_support_lta_type wcdma_rrc_support_lta;
  uint8 rrc_disable_cpc_fdpch_nv;                                    
  uint8 rrc_disable_crash_debug_info_nv;                             
  boolean wcdma_rrc_wtol_ps_ho_support_nv;                           
  boolean wcdma_rrc_wtol_tdd_ps_ho_support_nv;                       
  uint8 rrc_ignore_cell_bar_nv;                                      
  uint8 rrc_csfb_skip_sib11_opt_nv;                                  
  uint16 rrc_nv_channel_lock_item_value;                             
  uint8 rrc_do_not_forward_page_nv;                                  
  uint32 wcdma_rrc_freq_scan_timer_nv;                               
  uint32 wcdma_rrc_deep_sleep_no_svc_timer_nv;                       
  uint32 wcdma_rrc_bplmn_freq_scan_timer_nv;                         
  boolean wcdma_rrc_sib_sleep_sb_nv;                                 
  boolean rrc_constant_320ms_drx_nv;                                 
  boolean rrc_prioritize_850_band_nv;
  uint16 rrc_disable_meas_nv;
  uint8 rrc_dormancy_support_nv;
        
  /* NV ID for PSC lock: 70241  */
  boolean rrc_enable_psc_lock_nv;

  uint32 wcdma_rrc_csg_max_prune_count_nv; /* Number of ASF searches in one month */
  boolean wcdma_csg_srch_carrier_space_nv;
  uint8 wcdma_asf_timer_nv;


  boolean snow3g_security_algo_supported;

  uint8 rrc_set_srb2_act_time_nv;

  boolean rrc_fast_return_to_lte_after_csfb_nv;
  uint16 rrc_fast_return_to_lte_after_csfb_timer_nv ;
  rrc_nv_delay_return_to_lte_type rrc_delay_return_to_lte_nv;
  boolean rrc_fast_return_to_lte_after_csfb_ss_nv;
  rrc_plmn_list_type rrc_block_frlte_on_plmn_list;

  boolean rrc_ganss_support_nv;

  uint8 rrc_fe_fach_support_nv;

  /*NV72581*/
  uint16 rrc_dsds_concurrency;

  /*NV72582*/
  uint16 rrc_dsda_concurrency;

  /*NV72583*/
    uint16 rrc_tsts_concurrency;

  /* NV# 72548*/
  boolean rrc_mfbi_support_nv;

  /* NV# 72576 */
  boolean rrc_ul_compr_cap_nv;

  uint8 rrc_device_type_nv;

  uint16 rrc_feature_nv_item_value;

  uint8 rrc_default_cfg_in_cell_fach_nv;

  uint32 rrc_sib7_exp_time;
  
  boolean rrc_meas_id_extn_support;

  uint32 wcdma_rrc_idle_feature;

  uint16 wcdma_rrc_db_dc_band_comb_nv_control;

  uint8 wcdma_freq_skip_range_nv;

  boolean enable_ta_for_cs_datacall_nv;

  boolean rrc_read_sibs_for_resel_enabled_nv;
  
  boolean rrc_read_sibs_enabled_nv;
  
  boolean rrc_half_drx_len;

  uint16 rrc_change_trm_priority_tmr_nv;

  rrc_nv_defer_cel_resel_waiting_for_l2_ack_type wcdma_rrc_defer_waiting_for_l2_ack;

  uint16 wcdma_rrc_msim_db_dc_band_comb_nv_control[MAX_UE_MODES];

  rrc_nv_pseudo_sib19_type wcdma_rrc_psuedo_sib19;

  rrc_nv_msim_opt_type wcdma_rrc_msim_opt;
  

  rrc_plmn_identity_type efs_plmn_id;

  rrc_nv_gsm_fallback_info_type rrc_gsm_fallback_info;

  uint32 modem_log_util_nv;

  uint8 wcdma_rrc_time_to_allow_lta_during_access_priority;

  rrc_nv_pseudo_meas_info_type  rrc_pseudo_meas_info;

  rrc_nv_pseudo_meas_rxagc_info_type  rrc_pseudo_meas_rxagc_info;

  rrc_nv_pseudo_meas_triggers_info_type  rrc_pseudo_meas_triggers_info;

  rrc_nv_pseudo_meas_rsrp_rsrq_info_type     rrc_pseudo_meas_rsrp_rsrq_info;

  boolean rrc_cell_bar_upon_rach_failure_nv;
  uint16  rrc_cell_bar_duration_secs_nv;
  rrc_smc_buffer_support_type rrc_buffer_smc_support;

  boolean rrc_req_lte_for_truncated_ca_nv;

  rrc_mcc_list_type rrc_req_lte_for_truncated_ca_on_mcc_list_nv;
boolean rrc_wtol_redirect_after_srvcc_nv;

  uint16 rrc_pseudo_meas_forbidden_earfcn_timer_nv;
  uint16 rrc_pseudo_meas_defer_tmr_value_first_time_in_dch_nv;
   uint8 rrc_enable_hi_ac;
} wnv_rrc_default;


typedef union
{
  uint32  optional_feature_list;
  uint8   hsupa_category;
  boolean hsupa_cm_ctrl;
  boolean dl_freq_enabled;
  uint16  dl_freq;
  boolean cs_voice_over_hspa_enabled;
  uint8   rrc_version;
  boolean rrc_pdcp_disabled;
  boolean fake_security_enabled;
  uint8   hsdpa_compressed_mode_enabled;
  boolean rrc_ciphering_enabled;
  boolean rrc_integrity_enabled;
  boolean geran_feature_pack_1;
  uint8   interrat_nacc_support;
  boolean rrc_fake_security;
} wnv_rrc_legacy_default;


/*************************************************************************** 
                      Internal structure definitions
****************************************************************************/

/*************************************************************************** 
                          Exported functions NV default functions
****************************************************************************/
wnv_api_status wnv_default_rrc(wsub_id_e_type wsub_id, wnv_nv_id_enum nv_id, void* out_buffer, uint32 output_len, boolean write_to_file);

wnv_api_status wnv_default_rrc_feature(wsub_id_e_type wsub_id, wnv_nv_id_enum nv_id, void* out_buffer, uint32 output_len, boolean write_to_file);

wnv_api_status wnv_default_rrc_device(wsub_id_e_type wsub_id, wnv_nv_id_enum nv_id, void* out_buffer, uint32 output_len, boolean write_to_file);

wnv_api_status wnv_default_rrc_legacy(wsub_id_e_type wsub_id, wnv_nv_id_enum nv_id, void* out_buffer, uint32 output_len, boolean write_to_file);

#endif /* _WNV_RRC_H_*/
