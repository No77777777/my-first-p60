#ifndef __CMGR_API_H__ 
#define __CMGR_API_H__


/*=========================================================================

           M O D E M   C L O C K   A N D   P O W E R   M A N A G E R

                E X T E R N A L   H E A D E R   F I L E


  GENERAL DESCRIPTION
    This file contains the external interface functions and definitions for
    Modem Clock and Power Manager (MCPM).

  EXTERNALIZED FUNCTIONS
    cmgr_scenario_config

  INITIALIZATION AND SEQUENCING REQUIREMENTS
    

        Copyright (c) 2021 by QUALCOMM Technologies, Inc.  All Rights Reserved.


==========================================================================*/

/*==========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mpower.mpss/13.1/api/cmgr_api.h#13 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------
11/15/19   sb      concurrency manager changes


==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/



#include <comdef.h> 



typedef enum
{
  CMGR_1X_TECH,
  CMGR_DO_TECH,
  CMGR_WCDMA_TECH,
  CMGR_WCDMA1_TECH,
  CMGR_LTE_TECH,
  CMGR_LTE1_TECH,
  CMGR_TDSCDMA_TECH,
  CMGR_GERAN_TECH,
  CMGR_GERAN1_TECH,
  CMGR_GPS_TECH,
  CMGR_WLAN_TECH,
  CMGR_MAX_TECH,
}cmgr_tech_type;


typedef enum
{
  CMGR_FC_NONE,
  CMGR_FC_DOWN,
  CMGR_FC_UP
}cmgr_flow_ctrl_cmd_e;



typedef enum
{
  CMGR_STEADY_START,
  CMGR_STEADY_UPDATE, /*turn off old & turn on new*/
  CMGR_STEADY_END,

  CMGR_TRANSIENT_START,
  CMGR_TRANSIENT_UPDATE,
  CMGR_TRANSIENT_END,
  CMGR_TRANSIENT_CAN_WAIT,
  CMGR_TRANSIENT_WAIT_CANCEL,
  CMGR_TRANSIENT_RESERVE,
  CMGR_TRANSIENT_RESERVE_CANCEL,
  CMGR_TYPE_MAX
}cmgr_scenario_type_e;



typedef enum
{
  CMGR_SCENARIO_NOT_RECOGNIZED,
  CMGR_ACCEPTED,
  /*for START/UPDATE scenario_type*/
  CMGR_REJECTED,//due to ongoing concurrency

  /*for CAN_WAIT type*/
  CMGR_WAIT,//FC triggered on other RAT
  /*in case of CMGR_TRANSIENT_WAIT_CANCEL*/
  CMGR_WAIT_CANCEL_OK,
  CMGR_WAIT_CANCEL_NOK,/*if scenario allow processing ongoing and we get cancel*/
  /*reject case concurrency seen, both reasons possible at same time*/
  CMGR_WAIT_REJECT_PRIORITY_LOW,
  CMGR_WAIT_REJECT_TIME_LESS,/*fc_time > transient_start_time*/

  /*for RESERVE type*/
  CMGR_RESERVE_TENTATIVE_ACCEPT,
  /*in case of CMGR_TRANSIENT_RESERVE_CANCEL*/
  CMGR_RESERVE_CANCEL_OK,
  CMGR_RESERVE_CANCEL_NOK,/*if scenario allow processing ongoing and we get cancel*/
  /*reject case*/
  CMGR_RESERVE_REJECT_PRIORITY_LOW,/*high prio transient ongoing*/
  CMGR_RESERVE_REJECT_TIME_LESS,/*fc_time > reserve_time*/
}cmgr_scenario_status_e;





typedef enum
{
  CMGR_1X = 0,
  CMGR_1X_SECONDARY = 1,

  CMGR_HDR = 2,
  CMGR_HDR_SECONDARY = 3,

  CMGR_GPS = 4,

  CMGR_UMTS = 5,
  CMGR_UMTS_SECONDARY = 6,

  CMGR_GSM1 = 7,

  CMGR_GSM2 = 8,

  CMGR_WCDMA = 9,

  CMGR_TDSCDMA = 10,
  CMGR_TDSCDMA_SECONDARY = 11,

  CMGR_UMTS_CA = 12,
  CMGR_UMTS_CA_SECONDARY = 13,

  CMGR_LTE = 14,
  CMGR_LTE_SECONDARY = 15,

  CMGR_LTE_SUB2 = 16,
  CMGR_LTE_SUB2_SECONDARY = 17,

  CMGR_CM = 18,

  CMGR_GSM3 = 19,

  CMGR_GSM_SECONDARY = 20,


  CMGR_RF = 21,

  /* Dummy client for L+G DSDS */
  CMGR_IRAT = 22,

  /* WLAN client */
  CMGR_WLAN = 23,

  CMGR_GPRS1 = 24,

  CMGR_GPRS2 = 25,

  CMGR_GSM1_SECONDARY = 26,

  CMGR_GSM2_SECONDARY = 27,

  CMGR_GSM3_SECONDARY = 28,

  /* Client for SUB2 Reselection Exchange */
  CMGR_IRAT2 = 29,

  /* SUB2 WCDMA PRx Client */
  CMGR_UMTS2 = 30,

  /* SUB2 WCDMA DRx Client */
  CMGR_UMTS2_SECONDARY = 31,

  CMGR_UMTS_SECONDARY2 = CMGR_UMTS2_SECONDARY,

  /* Tx clients: rxtx split */
  CMGR_1X_TX = 32,

  CMGR_HDR_TX = 33,

  CMGR_UMTS_TX = 34,

  CMGR_UMTS2_TX = 35,

  CMGR_UMTS_CA_TX = 36,

  CMGR_GSM1_TX = 37,

  CMGR_GSM2_TX = 38,

  CMGR_TDSCDMA_TX = 39,

  CMGR_LTE_TX = 40,

  CMGR_LTE_SUB2_TX = 41,

  CMGR_GPRS1_TX = 42,

  CMGR_GPRS2_TX = 43,

  CMGR_IRAT_TX = 44,

  CMGR_IRAT2_TX = 45,

  CMGR_WLAN_SECONDARY = 46,

  CMGR_WLAN_5G = 47,

  CMGR_WLAN_5G_SECONDARY = 48,

  CMGR_LAST_STATIC_CLIENT = CMGR_WLAN_5G_SECONDARY,

  /* There is a gap 12 clients between static cand dynamic client ids
     New static clients can be added here without affecting Dynamic client ids
     if new static  cleint id are  <= 60. */

  /* dynamic clients */

  CMGR_LTE_CA = 61,

  CMGR_LTE_CA_SECONDARY = 62,

  CMGR_LTE_CA_TX = 63,

  CMGR_LTE_CA1 = 64,

  CMGR_LTE_CA1_SECONDARY = 65,

  CMGR_LTE_CA1_TX = 66,

  CMGR_LTE_CA2 = 67,

  CMGR_LTE_CA2_SECONDARY = 68,

  CMGR_LTE_CA2_TX = 69,

  CMGR_LTE_HO_SECONDARY1 = 70,

  CMGR_LTE_4_RX0 = CMGR_LTE_HO_SECONDARY1,

  CMGR_LTE_HO_SECONDARY2 = 71,

  CMGR_LTE_4_RX1 = CMGR_LTE_HO_SECONDARY2,

  CMGR_LTE_CA_4_RX0 = 72,

  CMGR_LTE_CA_4_RX1 = 73,

  CMGR_LTE_CA1_4_RX0 = 74,

  CMGR_LTE_CA1_4_RX1 = 75,

  CMGR_LTE_CA2_4_RX0 = 76,

  CMGR_LTE_CA2_4_RX1 = 77,

  CMGR_LTE_EMBMS1 = 78,

  CMGR_LTE_EMBMS2 = 79,

  CMGR_LTED = 80,
  CMGR_LTED_SECONDARY = 81,

  CMGR_LTED_TX = 82,

  CMGR_LTE_CGI = 83,

  CMGR_LTE_CGI_SECONDARY = 84,

  CMGR_LTE_SUB2_CA = 85,

  CMGR_LTE_SUB2_CA_SECONDARY = 86,

  CMGR_LTE_SUB2_CA_TX = 87,

  CMGR_LTE_SUB2_CA1 = 88,

  CMGR_LTE_SUB2_CA1_SECONDARY = 89,

  CMGR_LTE_SUB2_CA1_TX = 90,

  CMGR_LTE_SUB2_CA2 = 91,

  CMGR_LTE_SUB2_CA2_SECONDARY = 92,

  CMGR_LTE_SUB2_CA2_TX = 93,

  CMGR_LTE_SUB2_4_RX0 = 94,

  CMGR_LTE_SUB2_4_RX1 = 95,

  CMGR_LTE_SUB2_CA_4_RX0 = 96,

  CMGR_LTE_SUB2_CA_4_RX1 = 97,

  CMGR_LTE_SUB2_CA1_4_RX0 = 98,

  CMGR_LTE_SUB2_CA1_4_RX1 = 99,

  CMGR_LTE_SUB2_CA2_4_RX0 = 100,

  CMGR_LTE_SUB2_CA2_4_RX1 = 101,

  CMGR_LTE_SUB2_EMBMS1 = 102,

  CMGR_LTE_SUB2_EMBMS2 = 103,

  CMGR_LTE_SUB2_LTED = 104,

  CMGR_LTE_SUB2_LTED_SECONDARY = 105,

  CMGR_LTE_SUB2_LTED_TX = 106,

  CMGR_LTE_SUB2_CGI = 107,

  CMGR_LTE_SUB2_CGI_SECONDARY = 108,

  CMGR_LTE_CA3 = 109,
  CMGR_NR5G_CA3 =  CMGR_LTE_CA3,

  CMGR_LTE_CA3_SECONDARY = 110,
  CMGR_NR5G_CA3_SECONDARY =  CMGR_LTE_CA3_SECONDARY,

  CMGR_LTE_CA3_TX = 111,
  CMGR_NR5G_CA3_TX = CMGR_LTE_CA3_TX,

  CMGR_LTE_CA3_4_RX0 = 112,
  CMGR_NR5G_CA3_4_RX0 = CMGR_LTE_CA3_4_RX0,

  CMGR_LTE_CA3_4_RX1 = 113,
  CMGR_NR5G_CA3_4_RX1 = CMGR_LTE_CA3_4_RX1,

  CMGR_LTE_SUB2_CA3 = 114,

  CMGR_LTE_SUB2_CA3_SECONDARY = 115,

  CMGR_LTE_SUB2_CA3_TX = 116,

  CMGR_LTE_SUB2_CA3_4_RX0 = 117,

  CMGR_LTE_SUB2_CA3_4_RX1 = 118,

  /* For internal bound-checking and array-sizing */
  CMGR_LAST_CLIENT = CMGR_LTE_SUB2_CA3_4_RX1,

  /* Should always be CMGR_LAST_CLIENT + 1 */
  CMGR_MAX_CLIENTS,

  CMGR_NO_CLIENT    = 0xFF
}
cmgr_client_enum_t;





typedef enum
{
  /* Perform general access attempt */
   CMGR_ACCESS = 0,

  /* Perform access as soon as possible. CMGR asks retain lock holders to
     release the lock immediately. */
   CMGR_ACCESS_URGENT = 1,

   /* Determining the mobile location */
   CMGR_LOCATION_FIX = 2,

   /* Deprecated reason */
   CMGR_DEMOD_GPS = CMGR_LOCATION_FIX,

  /* In a traffic call */
   CMGR_TRAFFIC = 3,

  /* Monitoring paging channel */
   CMGR_DEMOD_PAGE = 4,

  /* Acquiring or reacquiring the system (DR Reason) */
   CMGR_ACQUISITION = 5,

  /* Diversity */
   CMGR_DIVERSITY = 6,

  /* Monitoring paging channel with a small SCI */
   CMGR_SMALL_SCI_PAGE = 7,

  /**/
   CMGR_CHANNEL_MAINTENANCE = 8,

  /* For priority inversion  */
   CMGR_ACQUISITION_INV = 9,

  /* For Background Traffic */
   CMGR_BG_TRAFFIC = 10,

  /* Small SCI Page Continuation */
   CMGR_SMALL_SCI_PAGE_CONTINUATION = 11,

  /* For IRAT measurements */
   CMGR_IRAT_MEASUREMENT = 12,

  /* Sleep */
   CMGR_SLEEP = 13,

  /* inverted priority for channel maintenance */
   CMGR_CHANNEL_MAINTENANCE_INV = 14,

  /* Inter-RAT measurment with diversity */
   CMGR_DIVERSITY_IRAT_MEASUREMENT = 15,

   CMGR_SLEEP_DIVERSITY = 16,

  /* Low priority EMBMS */
   CMGR_EMBMS = 17,

  /* Inverted EMBMS priority */
   CMGR_EMBMS_INV = 18,

   CMGR_EMBMS_LOW = 19,

  /* For LTE Single chain exit */
   CMGR_DR_TRAFFIC = 20,

  /* Acquiring or reacquiring the system (SR Reason)  */
   CMGR_ACQUISITION_CELL_SELECT = 21,

  /* For priority inversion  */
   CMGR_ACQUISITION_CELL_SELECT_INV = 22,

  /* CTCH (BroadCast MultiCast) */
   CMGR_CTCH = 23,

  /* Inverted CTCH (BroadCast MultiCast) priority */
   CMGR_CTCH_INV = 24,

   /* Used to prevent single chain exit in LTE */
   CMGR_HIGH_PRIORITY_IDLE = 25,

   /*Used for special priority mapping*/
   CMGR_CHANNEL_MAINTENANCE_HIGH = 26,

  /*Used for CM_HIGH to ACCESS_LOW switching*/
   CMGR_ACCESS_LOW = 27,

  /* For internal bound-checking and array-sizing */
   CMGR_REASON_MAX = 28,

  /*default value*/
  CMGR_NO_REASON = 0xFF,

  /* for backward compatibility as techs use NUM_REASON enum */
  CMGR_NUM_REASONS = CMGR_NO_REASON

}
cmgr_reason_enum_t;




/*autogen code start keyword - do not change*/
/* define of all scenarios enums*/
typedef enum
{
  CMGR_1X_SLEEP,
  CMGR_1X_WAKEUP,
  CMGR_1X_IDLE,
  CMGR_1X_MEAS_1XTOL,
  CMGR_1X_VOICE,
  CMGR_1X_DATA,
}cmgr_1x_steady_scenarios;


typedef enum
{
  CMGR_1X_SRCH_FING_NOM,
  CMGR_1X_SRCH_FING_MED,
  CMGR_1X_SRCH_FING_HIGH,
  CMGR_1X_SRCH_FING_NOM_MED,
  CMGR_1X_SRCH_FING_MED_NOM,
  CMGR_1X_SRCH_FING_HIGH_NOM,
  CMGR_1X_SRCH_FING_NOM_HIGH,
  CMGR_1X_SRCH_FING_MED_HIGH,
  CMGR_1X_SRCH_FING_HIGH_MED,
}cmgr_1x_transient_scenarios;


typedef enum
{
  CMGR_DO_START_REQ,
  CMGR_DO_STOP_REQ,
  CMGR_DO_WAKE_UP_REQ,
  CMGR_DO_GO_TO_SLEEP_REQ,
  CMGR_DO_START_DATA_TX_REQ,
  CMGR_DO_STOP_DATA_TX_REQ,
  CMGR_DO_PARMS_UPDATE_REQ,
}cmgr_do_steady_scenarios;


typedef enum
{
  CMGR_HDR_NQTA,
  CMGR_HDR_RXD_ENABLE,
}cmgr_do_transient_scenarios;


typedef enum
{
  CMGR_WCDMA_WVOICE,
}cmgr_wcdma_steady_scenarios;


typedef enum
{
  CMGR_WCDMA_W2W_GAP,
  CMGR_WCDMA_W2G_GAP,
  CMGR_WCDMA_W2L_GAP,
  CMGR_WCDMA_WIDLE_WAKEUP,
}cmgr_wcdma_transient_scenarios;


typedef enum
{
  CMGR_LTE_STEADY_CA,
  CMGR_LTE_STEADY_256_QAM,
  CMGR_LTE_STEADY_PDCCH_1RX,
  CMGR_LTE_STEADY_VOLTE_BIMC,
  CMGR_LTE_STEADY_SLEEP,
}cmgr_lte_steady_scenarios;


typedef enum
{
  CMGR_LTE_TRANS_CA,
  CMGR_LTE_TRANS_L2G_INIT_MEAS_DRIVER,
  CMGR_LTE_TRANS_BUILD_RF_SCRIPT,
  CMGR_LTE_TRANS_AGAP,
  CMGR_LTE_TRANS_DEACT_SRCH,
  CMGR_LTE_TRANS_POS,
  CMGR_LTE_TRANS_QTA,
  CMGR_LTE_TRANS_QTA_L2L,
  CMGR_LTE_TRANS_QTA_L2W,
  CMGR_LTE_TRANS_QTA_L2G,
  CMGR_LTE_TRANS_QTA_L21X,
  CMGR_LTE_TRANS_QTA_L2T,
  CMGR_LTE_TRANS_GAPMGR_DEFAULT,
  CMGR_LTE_TRANS_GAPMGR_PRS_INTRA,
  CMGR_LTE_TRANS_GAPMGR_PRS_INTER,
  CMGR_LTE_TRANS_GAPMGR_LTE,
  CMGR_LTE_TRANS_GAPMGR_WCDMA,
  CMGR_LTE_TRANS_GAPMGR_TDS,
  CMGR_LTE_TRANS_GAPMGR_HRPD,
  CMGR_LTE_TRANS_GAPMGR_GERAN,
  CMGR_LTE_TRANS_GAPMGR_CDMA1X,
  CMGR_LTE_TRANS_DLS,
  CMGR_LTE_TRANS_DS,
}cmgr_lte_transient_scenarios;


typedef enum
{
  CMGR_TDS_STEADY_RXD_OFF,
  CMGR_TDS_STEADY_RXD_ON,
}cmgr_tds_steady_scenarios;


typedef enum
{
  CMGR_TDS_TRANS_RXD_OFF2ON,
  CMGR_TDS_TRANS_CPHY_SETUP,
  CMGR_TDS_TRANS_MEAS_GAP,
  CMGR_TDS_TRANS_QTA_GAP,
}cmgr_tds_transient_scenarios;


typedef enum
{
  CMGR_GERAN_PSCAN,
}cmgr_gsm_steady_scenarios;


typedef enum
{
  CMGR_GERAN_PTM_G2W_GAP,
  CMGR_GERAN_PTM_G2L_GAP,
  CMGR_GERAN_VOICE_G2W_GAP,
  CMGR_GERAN_VOICE_G2L_GAP,
  CMGR_GERAN_NCELL_ACQ_GAP,
  CMGR_GERAN_IDLE_G2W_GAP,
  CMGR_GERAN_IDLE_G2L_GAP,
}cmgr_gsm_transient_scenarios;


typedef enum
{
  CMGR_GPS_OFF,
  CMGR_GPS_ACQ_NORMAL,
  CMGR_GPS_ACQ_OPPORTUNISTIC,
  CMGR_GPS_TRK,
  CMGR_GPS_DPO,
}cmgr_gps_steady_scenarios;


typedef enum
{
  CMGR_GPS_TRANSIENT_OFF,
  CMGR_GPS_PE_FIX_COMPUTATION,
  CMGR_GPS_MC_MEAS_PROC,
  CMGR_GPS_PE_MC_FIX,
  CMGR_GPS_CD_FIX_COMPUTE,
  CMGR_GPS_PE_CD_FIX,
  CMGR_GPS_CD_MC_FIX,
  CMGR_GPS_CD_PE_MC_FIX,
  CMGR_GPS_TRANSIENT_MAX,
}cmgr_gps_transient_scenarios;


typedef enum
{
  WAL_WLAN_STEADY_STATE_INVALID,
  WAL_WLAN_STEADY_STATE_CSS_PWDN,
  WAL_WLAN_STEADY_STATE_SLEEP,
  WAL_WLAN_STEADY_STATE_SWDTIM,
  WAL_WLAN_STEADY_STATE_SAP_IDLE,
  WAL_WLAN_STEADY_STATE_PERF_BW_20_LP_1X1,
  WAL_WLAN_STEADY_STATE_PERF_BW_20_LP,
  WAL_WLAN_STEADY_STATE_PERF_BW_20,
  WAL_WLAN_STEADY_STATE_PERF_BW_40,
  WAL_WLAN_STEADY_STATE_PERF_BW_80,
  WAL_WLAN_STEADY_STATE_PERF_BW_160,
  WAL_WLAN_STEADY_STATE_COLD_BOOT,
  WAL_WLAN_STEADY_STATE_COEX_TDM,
  WAL_WLAN_STEADY_STATE_PERF_BW_20_LP_1X1_FLOW_CONTROLLED,
  WAL_WLAN_STEADY_STATE_PERF_BW_20_LP_FLOW_CONTROLLED,
  WAL_WLAN_STEADY_STATE_PERF_BW_20_FLOW_CONTROLLED,
  WAL_WLAN_STEADY_STATE_PERF_BW_40_FLOW_CONTROLLED,
  WAL_WLAN_STEADY_STATE_PERF_BW_80_FLOW_CONTROLLED,
  WAL_WLAN_STEADY_STATE_PERF_BW_160_FLOW_CONTROLLED,
  WAL_WLAN_STEADY_STATE_MAX,
}cmgr_wlan_steady_scenarios;


typedef enum
{
  PLAT_TRANSIENT_STATE_INVALID,
  PLAT_TRANSIENT_STATE_DPD_CAL,
  PLAT_TRANSIENT_STATE_CSS_DOWN,
  PLAT_TRANSIENT_STATE_FW_ASSISTED_SLEEP,
  PLAT_TRANSIENT_STATE_PLAT_INIT,
  PLAT_TRANSIENT_STATE_AUTO_SEQ,
  PLAT_TRANSIENT_STATE_FW_DIRECT_WAKEUP,
  PLAT_TRANSIENT_STATE_WHAL_RESET,
  PLAT_TRANSIENT_STATE_WHAL_ATTACH,
  PLAT_TRANSIENT_STATE_CB_ADC,
  PLAT_TRANSIENT_STATE_CB_DAC,
  PLAT_TRANSIENT_STATE_CB_RXDCO,
  PLAT_TRANSIENT_STATE_CB_PROCESS,
  PLAT_TRANSIENT_STATE_CB_NOISE_FLOOR,
  PLAT_TRANSIENT_STATE_CB_TPC_CAL,
  PLAT_TRANSIENT_STATE_CB_PDET,
  PLAT_TRANSIENT_STATE_CB_IM2,
  PLAT_TRANSIENT_STATE_CB_RXDCO_IQ,
  PLAT_TRANSIENT_STATE_CB_COMB_TXLO_TXIQ_RXIQ,
  PLAT_TRANSIENT_STATE_CB_LNA,
  PLAT_TRANSIENT_STATE_CB_DPD_LP_RXDCO,
  PLAT_TRANSIENT_STATE_CB_DPD_LP_RXIQ,
  PLAT_TRANSIENT_STATE_CB_IBF,
  PLAT_TRANSIENT_STATE_HW_RESET,
  PLAT_TRANSIENT_STATE_CSS_UP,
  PLAT_TRANSIENT_STATE_TRANSITION_TO_FW_CONTROL,
  PLAT_TRANSIENT_STATE_RETENTION_SET_CHANNEL,
  PLAT_TRANSIENT_STATE_FW_TRIGGER_WAKEUP,
  PLAT_TRANSIENT_STATE_CHANNEL_CHANGE,
  PLAT_TRANSIENT_STATE_CB_RXDCO_DTIM,
  PLAT_TRANSIENT_STATE_CB_DPD_MEMORYLESS,
  PLAT_TRANSIENT_STEADY_START_CSS_PWDN,
  PLAT_TRANSIENT_STEADY_START_SLEEP,
  PLAT_TRANSIENT_STEADY_START_SWDTIM,
  PLAT_TRANSIENT_STEADY_START_SAP_IDLE,
  PLAT_TRANSIENT_STEADY_START_PERF_BW_20_LP_1X1,
  PLAT_TRANSIENT_STEADY_START_PERF_BW_20_LP,
  PLAT_TRANSIENT_STEADY_START_PERF_BW_20,
  PLAT_TRANSIENT_STEADY_START_PERF_BW_40,
  PLAT_TRANSIENT_STEADY_START_PERF_BW_80,
  PLAT_TRANSIENT_STEADY_START_PERF_BW_160,
  PLAT_TRANSIENT_STEADY_START_COLD_BOOT,
  PLAT_TRANSIENT_STEADY_START_COEX_TDM,
  PLAT_TRANSIENT_STATE_MAX,
}cmgr_wlan_transient_scenarios;


/*autogen code ends keyword - do not change*/



typedef struct
{
  cmgr_scenario_type_e type;
  uint32 scenario_e; /*enum ID defined by tech, used by conc_mgr*/
  uint32 update_scenario_e; /*type:update, this is new scenario to be turned on*/
                            /*along with turn off of above scenario*/
  uint32 priority; /*uint32 number agreed priority for scenario, may change in case of inversion*/
  uint32 flow_ctrl_time_msec; /*time in msec needed for steady state to flow control*/
                              /* sent along with steady state scenario enum*/
                              /*a value of 0xFFFFFFFF means it cannot be flow controlled, i.e. voice call*/
  uint32 * return_info; /*used in case of return status wait/reserve reject*/
                        /*if not NULL, then cmgr will fill it with priority / FC_time of winning tech*/
  uint32 duration_msec;/*uint32 duration after which transient scenario is expected to end, */
                         /*needed to handle can_wait (of other tech), reserve*/
  uint64 start_timestamp; /*uint64 ticks @19.2MHz qurt_sysclock_get_hw_ticks or DalTimetick_GetTimetick64 */
                    /*incase of can_wait, indicates timestamp beyond which scenario is not needed*/
                    /* tech can get notified before this time*/
                    /*incase of reserve, this indicates start timestamp for the scenari*/
                    /*client is waiting until this time*/
}cmgr_params;


typedef void ( * cmgr_cb_func_type )
(
  cmgr_tech_type tech,
  uint32 scenario, /*enum of scenario*/
  cmgr_scenario_status_e status, /* accepted, rejected,... */
  uint32 additional_info/*used in case of can_wait or reserve... */
              /* this will have priority or Fc_time of winning tech depending on status*/
);

typedef void ( * cmgr_flow_ctrl_func_type )
(
  cmgr_tech_type tech,
  cmgr_flow_ctrl_cmd_e fc_cmd,
  uint32 current_steady_scenario_e
);

/* =========================================================================
 **  Function : cmgr_register_cb
 ** =========================================================================*/
/** Function is used by tech to register function which conc mgr calls to indicate
 * status of scenario incase of can_wait / reserve cases.


 @param state [in]   -- state update  requested.
 @return
 None

 @dependencies
 None.
 */

void cmgr_register_cb
(
  cmgr_tech_type tech,/*enum for tech*/
  cmgr_cb_func_type techx_cb_func /*callback function*/
);
/* =========================================================================
 **  Function : cmgr_register_flow_ctrl_cb
 ** =========================================================================*/
/** function is called by tech to register flow control function pointer with conc mgr


 @param state [in]   -- state update  requested.
 @return
 None

 @dependencies
 None.
 */

void cmgr_register_flow_ctrl_cb
(
  cmgr_tech_type tech,/*enum MCPM_tech_type*/
  cmgr_flow_ctrl_func_type techx_flow_ctrl_func /*callback function*/
);

/* =========================================================================
 **  Function : cmgr_flow_ctrl_done
 ** =========================================================================*/
/**function will be called by tech to inform conc_mgr that flow control action is complete


 @param state [in]   -- state update  requested.
 @return
 None

 @dependencies
 None.
 */
void cmgr_flow_ctrl_done
(
  cmgr_tech_type tech,/*enum MCPM_tech_type*/
  cmgr_flow_ctrl_cmd_e completed_fc_cmd,
  uint32 current_steady_scenario_e
);

/* =========================================================================
 **  Function : cmgr_scenario_config
 ** =========================================================================*/
/**


 @param state [in]   -- state update  requested.
 @return
 None

 @dependencies
 None.
 */
cmgr_scenario_status_e cmgr_scenario_config
(
  cmgr_tech_type tech,
  cmgr_params * input
);

/* =========================================================================
 **  Function : cmgr_get_priority
 ** =========================================================================*/
/*
 * This function cmgr_get_priority is used to get priority for CMGR
 * inputs are client and reason, and these are same as used in TRM.
 * L1 WAN techs are expected to use same params to get priority which
 * they will use to call cmgr_scenario_config api call.
 *
 * This api is just to ease integration process, since L1 techs do not
 * have priority info, only clinet and reason info, which they had used
 * to get priority from TRM.
 */
uint8 cmgr_get_priority(
  cmgr_client_enum_t client,
  cmgr_reason_enum_t reason
);

#endif /* __CMGR_API_H__ */


