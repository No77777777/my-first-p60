
/*===========================================================================

                    L 1   T R M    M A N A G E R

DESCRIPTION
  This file contains global declarations and external references
  for the L1 TRM Manager.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2000-2006 by Qualcomm Technologies, Incorporated. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/
/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/inc/tdsl1trm.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/06/14   sy      DR DSDS changes
05/21/12   srk     Added code to to disable OLPC in QTA gap
===========================================================================*/
#ifndef TDSTL1TRM_H
#define TDSTL1TRM_H
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "tdsl1const.h"
//#include "customer.h"
//#include "rex.h"
//#include "queue.h"
//#include "tdsl1def.h"
#include "tdsl1mtask.h"
#include "tdsl1rrcif.h"
#include "tdsl1def.h"
#include "tdssrchsglte.h"

#include "trm.h"
#include "wwan_coex_mgr.h"
#include "subs_prio.h"
#include "tdsl1msgr.h"


/*Enum to identify L1 and RRC to TDRM*/
typedef enum {
TDRM_CLIENT_L1,
TDRM_CLIENT_RRC,
TDRM_CLIENT_DIV,
TDRM_CLIENT_TX,
TDRM_CLIENT_MAX
} tdrm_client_enum_type;

/*Enum to identify TDRM state*/
typedef enum {
TDRM_NO_LOCK_STATE,
TDRM_LOCK_REQ_STATE,
TDRM_LOCK_RES_STATE,
TDRM_IN_LOCK_STATE,
TDRM_STATE_MAX
} tdrm_state_enum_type;

/*Enum to identify L1 and RRC state in TDRM*/
typedef enum {
TDRM_CLI_NO_LOCK_STATE,
TDRM_CLI_LOCK_REQ_STATE,
TDRM_CLI_LOCK_RES_STATE,
TDRM_CLI_IN_LOCK_STATE,
TDRM_CLI_STATE_MAX
} tdrm_client_state_enum_type;

/* band change request return type */
typedef enum {
TDRM_BAND_CHANGE_FAIL,
TDRM_BAND_CHANGE_SUCCESS,
TDRM_BAND_CHANGE_WAITING,
TDRM_BAND_CHANGE_NOT_USED
} tdrm_band_change_return_enum_type;

typedef void (* tdrm_grant_callback_type)
(
  /* The client which is being informed of an event */
  trm_client_enum_t               client,

  /* The event being sent to the client */
  trm_grant_return_enum_type     grant,

  /* Anonymous payload echoed from trm_request_and_notify() */
  trm_request_tag_t               tag
);

typedef void (* tdrm_change_priority_callback_type)
(
	/* The client which is being informed of the modifiy info result */ 
	trm_client_enum_t client,
	
	/* The status of modify info result */ 
	trm_modify_return_enum_type result
);

typedef void (* tdrm_band_grant_callback_type)
(
  trm_client_enum_t client,    /* Client requested for the band change */
  trm_band_type band,          /* Band class */
  trm_band_grant_return_type granted,   /* Status of band grant */
  trm_request_tag_t tag
);

typedef void (* tdrm_unlock_callback_advanced_t)
(
  trm_unlock_callback_data unlock_data
);

/*Structure to hold L1 and RRC specific information in TDRM*/
typedef struct {
tdrm_client_state_enum_type cli_state;

trm_duration_t req_dur;

trm_time_t res_starts_at;

trm_duration_t res_dur;

trm_resource_enum_t req_resource;

trm_reason_enum_t req_reason;

tdrm_grant_callback_type grant_cb;

trm_request_tag_t tag;

tdrm_band_grant_callback_type band_change_grant_cb;
tdrm_change_priority_callback_type change_priority_cb;

trm_frequency_type_t client_cur_band_info;
trm_frequency_type_t client_new_band_info;
boolean              band_change_waiting_flag;
boolean              tdrm_block_wait_grant_callback;
boolean              trm_lock_grant_callback_result;
} tdrm_client_info_struct_type;

#define MAX_TDRM_HIST_LOG_BUF_LEN  64

typedef enum {
TDRM_LOG_RNN_STT,
TDRM_LOG_RNN_RET,
TDRM_LOG_RNN_END,
TDRM_LOG_RES,
TDRM_LOG_REL,
TDRM_LOG_REQ,
TDRM_LOG_CH_PRI,
TDRM_LOG_EXC,
TDRM_LOG_GRANT
} tdrm_log_enum_type;


typedef enum {
TDRM_NOT_CALL_GRANT_CB,
TDRM_CALL_GRANT_CB,
TDRM_CALL_L1_GRANT_CB_PEND,
TDRM_CALL_RRC_GRANT_CB_PEND
} tdrm_log_grant_cb_enum_type;

/*Structure to log tdrm info in TDRM*/
typedef struct {
tdrm_log_enum_type log_type;
tdrm_state_enum_type tdrm_state;
tdrm_client_enum_type tdrm_cli;
tdrm_client_state_enum_type cli_state[TDRM_CLIENT_MAX];
trm_reason_enum_t    cur_reason;
trm_reason_enum_t    req_reason;
trm_request_tag_t tag;
boolean device_swap_needed;
tdrm_client_enum_type swap_req_client;
tdrm_log_grant_cb_enum_type call_grant_cb_status;

}tdrm_hist_log_buf_struct_type;


/*Structure to log tdrm info in TDRM*/
typedef struct {
tdrm_log_grant_cb_enum_type cal_l1_rrc_grant;
uint8 hist_log_idx;
tdrm_hist_log_buf_struct_type hist_log_buf[MAX_TDRM_HIST_LOG_BUF_LEN];
}tdrm_hist_log_struct_type;


/*Structure to hold TDRM information*/
typedef struct {
tdrm_state_enum_type tdrm_state;
trm_frequency_type_t tdrm_pri_band_info;
trm_frequency_type_t tdrm_sec_band_info;
rfm_device_enum_type tdrm_pri_dev_info;
rfm_device_enum_type tdrm_sec_dev_info;
rfm_device_enum_type tdrm_tx_dev_info;
#ifdef FEATURE_RF_USES_RFM_PATH_ARCH
uint32 tdrm_pri_rf_path;
uint32 tdrm_sec_rf_path;
uint32 tdrm_tx_rf_path;
#endif
rfm_device_enum_type multisim_cmd_pri_dev_info;

boolean device_swap_needed;
tdrm_client_enum_type swap_req_client;

boolean modify_duration_grant_cb_is_called;
trm_grant_return_enum_type modify_duration_grant;
int8 pending_sec_grant_cb;
tdrm_client_enum_type change_priority_client;
uint8 wakeup_identifier;

int8 pending_tx_grant_cb;
boolean call_l1_cb;
boolean call_rrc_cb;
trm_reason_enum_t prev_reason;
boolean in_dr_region;
tdrm_client_info_struct_type tdrm_client_info[TDRM_CLIENT_MAX];
sys_proc_type_e_type sub_reason;

tdrm_client_enum_type tx_req_client;
boolean rnn_pending_to_send_flag; /* record there is pending RnN when L1 calls trm_request. RnN would re-send out when reserve_at */
tdrm_client_enum_type prev_requested_client;

tdrm_hist_log_struct_type tdrm_hist_log;

#ifdef FEATURE_TDSCDMA_ADAPTIVE_PAGE_SKIPPING
boolean aps_state;
#endif
} tdrm_db_struct_type;

extern rex_timer_type tdrm_unlock_timer;

/* TDS_L1_TDRM_GRANT_CALLBACK_CMD structure. */
typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  tdsl1def_l1_local_cmd_hdr_type   hdr;

  /* The client which needs the RF resource */
  trm_client_enum_t       client;

  /* The data in the grant callback */
  trm_req_n_not_return_data data;

} tdsl1_dsds_trm_grant_callback_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  tdsl1def_l1_local_cmd_hdr_type   hdr;

  /* The client which needs the RF resource */
  trm_client_enum_t       client;

  /* The data in the grant callback */
  trm_req_n_not_return_data data;

} tdsl1_dsds_trm_secondary_grant_callback_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  tdsl1def_l1_local_cmd_hdr_type   hdr;

  /* The client which needs the RF resource */
  trm_client_enum_t       client;

  /* The data in the grant callback */
  trm_req_n_not_return_data data;

} tdsl1_dsds_trm_tx_grant_callback_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  tdsl1def_l1_local_cmd_hdr_type   hdr;
} tdsl1_tdrm_cm_inv_timeout_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  tdsl1def_l1_local_cmd_hdr_type   hdr;

  /* The client which needs the RF resource */
  trm_client_enum_t       client;

  /* Modify reason result */
  trm_modify_reason_return_type result;

} tdsl1_dsds_trm_modify_reason_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  tdsl1def_l1_local_cmd_hdr_type   hdr;

  /* The client which needs the RF resource */
  trm_client_enum_t       client;

  /* Modify reason result */
  trm_modify_band_return_type result;

  /* The tdrm_client_id that TDRM passed to TRM earlier */
  trm_request_tag_t tag;

} tdsl1_dsds_trm_modify_band_cmd_type;

typedef struct
{
  /* Need this to put command on l1_local_cmd_q. */
  tdsl1def_l1_local_cmd_hdr_type   hdr;

  /* The client which needs the RF resource */
  trm_client_enum_t       client;

  /* Unlock callback result */
  trm_unlock_callback_data data;

} tdsl1_dsds_trm_unlock_cmd_type;

extern tdrm_db_struct_type tdrm_db;

/*Initialization macros*/
#define TDRM_INVALID_TIME_DURATION 0
#define TDRM_INVALID_RES_AT_TIME 0
#define TDRM_INVALID_TAG 0xFF

/*Macros to check validity of TDRM and TDRM_TRM client identities*/
#define TDRM_IS_CLIENT_ID_VALID(tdrm_client_id) (tdrm_client_id < TDRM_CLIENT_MAX)
#define TDRM_IS_TRM_CLIENT_ID_VALID(trm_client_id) (trm_client_id == TRM_TDSCDMA)

/*Macros to check the state of TDRM*/
#define TDRM_STATE_IS_IN_LOCK() (tdrm_db.tdrm_state == TDRM_IN_LOCK_STATE)
#define TDRM_STATE_IS_LOCK_REQ() (tdrm_db.tdrm_state == TDRM_LOCK_REQ_STATE)
#define TDRM_STATE_IS_LOCK_RES() (tdrm_db.tdrm_state == TDRM_LOCK_RES_STATE)
#define TDRM_STATE_IS_NO_LOCK() (tdrm_db.tdrm_state == TDRM_NO_LOCK_STATE)

/*Macros to check the state of TDRM clients*/
#define TDRM_CLI_STATE_IS_IN_LOCK(tdrm_client_id) (tdrm_db.tdrm_client_info[tdrm_client_id].cli_state == TDRM_CLI_IN_LOCK_STATE)
#define TDRM_CLI_STATE_IS_LOCK_REQ(tdrm_client_id) (tdrm_db.tdrm_client_info[tdrm_client_id].cli_state == TDRM_CLI_LOCK_REQ_STATE)
#define TDRM_CLI_STATE_IS_LOCK_RES(tdrm_client_id) (tdrm_db.tdrm_client_info[tdrm_client_id].cli_state == TDRM_CLI_LOCK_RES_STATE)
#define TDRM_CLI_STATE_IS_NO_LOCK(tdrm_client_id) (tdrm_db.tdrm_client_info[tdrm_client_id].cli_state == TDRM_CLI_NO_LOCK_STATE)

/*Macros to get and set the state of TDRM*/
#define TDRM_SET_TDRM_STATE(state_of_tdrm) (tdrm_db.tdrm_state = state_of_tdrm)
#define TDRM_GET_TDRM_STATE() (tdrm_db.tdrm_state)

/*Macros to get device ID info*/
#define TDRM_GET_PRI_DEVICE_ID() (tdrm_db.tdrm_pri_dev_info)
#define TDRM_GET_SEC_DEVICE_ID() (tdrm_db.tdrm_sec_dev_info)
#define TDRM_GET_TX_DEVICE_ID() (tdrm_db.tdrm_tx_dev_info)

#ifdef FEATURE_RF_USES_RFM_PATH_ARCH
/*Macros to get path info*/
#define TDRM_GET_PRI_PATH() (tdrm_db.tdrm_pri_rf_path)
#define TDRM_GET_SEC_PATH() (tdrm_db.tdrm_sec_rf_path)
#define TDRM_GET_TX_PATH() (tdrm_db.tdrm_tx_rf_path)
#endif

/*Macros to get and set the state of TDRM client*/
#define TDRM_SET_TDRM_CLI_STATE(tdrm_client_id, tdrm_cli_state) (tdrm_db.tdrm_client_info[tdrm_client_id].cli_state = tdrm_cli_state)
#define TDRM_GET_TDRM_CLI_STATE(tdrm_client_id) (tdrm_db.tdrm_client_info[tdrm_client_id].cli_state)

/*Macros to get alternate TDRM client id*/
#define TDRM_GET_ALTERNATE_CLIENT_ID(tdrm_client_id) (tdrm_client_enum_type)((tdrm_client_id + (tdrm_client_enum_type)1) % TDRM_CLIENT_MAX)

#define TDRM_GET_TAG_FROM_CLI_ID(tdrm_client_id) (tdrm_db.tdrm_client_info[tdrm_client_id].tag)


/* TRM change priority timer */
#define TDRM_BAND_CHANGE_TIMEOUT_MSEC 1000

/* Delay 33 * 5 sclk (32.768 sclk = 1ms) */
#define TDRM_RXD_SECONDARY_CHAIN_REQUEST_DELAY 33*5

/* Maximum time for TRM to grant access to TD-SCDMA for the primary antenna */
#define TDRM_REQUEST_TIMEOUT_MSEC 5000
/* Initial duration of trm request for primary antenna */
#define TDRM_RXTX_MINIMUM_DURATION  TIMETICK_SCLK_FROM_MS(150)

#define TDSL1_DEFAULT_GRANT_CB (tdsl1_ds_grant_cb)

#define T2L_TO_RELEASE_TDRM_SEC_CHAIN_THRESHOLD_CM 20 //5ms
#define T2L_TO_RELEASE_TDRM_SEC_CHAIN_THRESHOLD_IDLE 40 //40ms
#define ONE_TDS_SUBFM_DURATION_IN_MS   5

#define TAG_RRC_REQUEST_LOCK_FOR_FS_ACQ 1
#define TAG_RRC_REQUEST_LOCK_NOT_FOR_FS_ACQ 0

/*TDSL1 dual sim procedures*/
typedef enum {
  TDSL1_DS_PROC_START=0,
  TDSL1_DS_PROC_SUSPEND=1,
  TDSL1_DS_PROC_RESUME=2,
  TDSL1_DS_PROC_CELL_TRANS_IN_FACH=3,
  TDSL1_DS_PROC_STOP=4,
  TDSL1_DS_PROC_DEACTIVATE=5,
  TDSL1_DS_PROC_FS=6,
  TDSL1_DS_PROC_ACQ=7,
  TDSL1_DS_PROC_SETUP_SBCH=8,
  TDSL1_DS_PROC_SETUP_NBCH_IN_DRX=9,
  TDSL1_DS_PROC_SETUP_PCH=10,
  TDSL1_DS_PROC_SETUP_FACH=11,
  TDSL1_DS_PROC_SETUP_NBCH_IN_FACH=12,
  TDSL1_DS_PROC_SETUP_DCH=13,
  TDSL1_DS_PROC_PAGING=14,
  TDSL1_DS_PROC_CELL_RESEL_START=15,
  TDSL1_DS_PROC_CELL_TRANS_REQ=16,
  TDSL1_DS_PROC_SETUP_SBCH_IN_FACH=17,
  TDSL1_DS_PROC_DROP_ALL=18,
  TDSL1_DS_PROC_BPLMN=19,
  TDSL1_DS_PROC_LFS_ACQ=20,
  #ifdef FEATURE_TDSCDMA_T2X_COLLIDE_WITH_SUB2_PICH_OPT
  TDSL1_DS_PROC_IRAT_MEAS=21,
  #endif
  TDSL1_DS_PROC_RESUME_FOR_RXTX=22,
#ifdef FEATURE_TDSCDMA_T2X_BPLMN_PAGE_RATE_OPT
  TDSL1_DS_PROC_RESUME_FOR_BPLMN = 23,
#endif    
  TDSL1_DS_PROC_MAX
}tdsl1_ds_proc_enum_type;

typedef enum
{
  TDSL1_DS_CHNL_PICH,
  TDSL1_DS_CHNL_PCH,

  /* add more as need */
  TDSL1_DS_CHNL_TYPE_MAX
} tdsl1_ds_channel_type;


/*TDSL1 dual sim paging specific information structure*/
typedef struct {
  boolean pich_lock_fail;
  boolean pch_lock_fail;

  uint16 declare_oos_thresh;
  uint16 declare_oos_thresh_once_s_failure;
  uint16 dwpts_reacq_thresh;

  uint32 trm_lock_consecutive_fail_cnt;

  boolean cell_trans_req_stat;

  boolean tdsl1_in_paging_wakeup;

  boolean lock_fail_at_wakeup;
  boolean declare_oos_once_s_failure;
  boolean dwpts_reacq_after_long_gap;
} tdsl1_ds_paging_spec_info_struct_type;


typedef enum
{
  TDSL1_QTA_STATE_NOT_ACTIVE,
  TDSL1_QTA_STATE_WAIT_TO_START_QTA,
  TDSL1_QTA_STATE_DIS_MEAS_UPA,
  TDSL1_QTA_STATE_INFORM_GL1,
  TDSL1_QTA_STATE_ACTIVE,
  TDSL1_QTA_STATE_QTA_STOPPED,
  TDSL1_QTA_STATE_MAX
} tdsl1_qta_state_enum_type;

typedef enum
{
  TDSL1_QTA_TSN_0 = 0,
  TDSL1_QTA_TSN_SPECIAL,  
  TDSL1_QTA_TSN_1,
  TDSL1_QTA_TSN_2,
  TDSL1_QTA_TSN_3,
  TDSL1_QTA_TSN_4,
  TDSL1_QTA_TSN_5,
  TDSL1_QTA_TSN_6,
  TDSL1_QTA_TSN_INVALID
}tdsl1_qta_tsn_enum_type;

typedef enum
{
  ACQ_INV_TIMER_DEACT = 0,
  ACQ_INV_TIMER_SET_FOR_G2T,
  ACQ_INV_TIMER_SET_FOR_NON_G2T
}tdsl1_acq_inv_timer_state_enum_type;


typedef struct {
/*Range is 0-8191*/
  uint32 sub_fn;

/*Range is 0-51199*/
  uint32 cx8_offset;
  
  tdsl1_qta_tsn_enum_type tsn; /* 0~7 */
  
} tdsl1_qta_time_info_struct_type;


/*Structure to hold QTA Log Packet info */

typedef struct
{
  /* Word 0&1*/
  /*! \brief sleep clock */    
  int64 cur_timetick;
  /* Word 2*/
  /*! \brief rx_sfn */
  uint16 rx_sfn;
  /*! \brief rx_cx8 */
  uint16 rx_cx8;
}tdsl1_ds_qta_time_struct_type;

typedef struct {
  /* word 1~3 */
  /*! \brief QTA start time */
  tdsl1_ds_qta_time_struct_type qta_start_time;
  /* word 4~6 */
  /*! \brief QTA end time */
  tdsl1_ds_qta_time_struct_type qta_end_time;  

  /* word 7~8 */
  /*! \brief QTA duration(QTA end time ¡§C QTA start time) in sleep clock, 32kHz */
  int64 qta_l1_duration_in_slk;
  /* word 9 */
  /*! \brief QTA duration(QTA end time ¡§C QTA start time) in ms */
  uint16 qta_l1_duration_in_ms;
  /*! \brief QTA duration(cxmLockLostTime ¡§C cxmLockAcqTime) in ms */
  uint16 qta_fw_duration_in_ms;
  
  /* word 11 */
  /*! \brief L1M state when QTA happen(DCH:7 or FACH:6) */
  uint8 l1m_state_when_qta;
  /*! \brief HSDPA HARQ flush after QTA (TRUE or FALSE) */
  uint8 dpa_harq_flash_flag;
  /*! \brief Re-ACQ complete indicator after QTA (TRUE or FALSE) */
  uint8 reacq_complete_ind;
  uint8 pad2;
}tdsl1_ds_qta_log_packet_struct;

//For SW DA, the DA mode starts from power up. For HW DA, the DA mode only for FACH and DCH once we have tx for the possible tx bkoff.
#define TDSL1_IN_DSDA_MODE()  (((tdssrchsglte.meas_mode == TDSL1_MEAS_MODE_NORMAL) && (tdsl1_ds_db.tdsl1_in_da_mode)) || \
		  (((tdsl1m_global.l1_state == TDSL1M_FACH) || (tdsl1m_global.l1_state == TDSL1M_DCH)) && \
		   (tdssrchsglte.meas_mode == TDSL1_MEAS_MODE_SGLTE) && (tdsl1_ds_db.tdsl1_in_da_mode)))

#define TDSL1_DR_DSDS_FALLBACK_PCH_EVAL_NUM_DEFAULT 3
#define TDSL1_DR_DSDS_FALLBACK_PCH_EVAL_NUM_MAX 5
#define TDSL1_DR_DSDS_FALLBACK_MAX_PCH_DEC_NUM_ONE_CYCLE 4
#define TDSL1_DR_DSDS_FALLBACK_DESENSE_GLITCH_WINDOW_SIZE \
        (TDSL1_DR_DSDS_FALLBACK_PCH_EVAL_NUM_MAX * TDSL1_DR_DSDS_FALLBACK_MAX_PCH_DEC_NUM_ONE_CYCLE)
typedef struct
{  
  uint8     crc_error_counter_one_cycle;
  uint8     crc_total_counter_one_cycle;
  uint8     crc_error_consecutive_num;
  uint8     crc_success_consecutive_num;
  uint8     pre_crc_error_consecutive_num;
  uint8     pre_crc_success_consecutive_num;
  boolean   current_lack_of_RxD;
  boolean   lack_of_RxD[TDSL1_DR_DSDS_FALLBACK_PCH_EVAL_NUM_MAX];//lack of RxD in past wake-up cycles in which PCH decoding has been performed
  boolean   current_idle_RxD_decision;
  boolean   idle_RxD_decision[TDSL1_DR_DSDS_FALLBACK_PCH_EVAL_NUM_MAX];//idle RxD decision by RxD module in past wake-up cycles in which PCH decoding has been performed
  boolean   desense_glitch_result[TDSL1_DR_DSDS_FALLBACK_DESENSE_GLITCH_WINDOW_SIZE];
  int16     tdsrxdiv_idle_filter_rscp;
  uint32    eval_drx_cycle_counter;
  boolean   tds_to_trigger_fb;
  boolean   tds_fb_in_legacy_ds;
  boolean   pch_without_pich;
} tdsl1_drds_legacy_fb_metrics_type;


typedef struct
{      
  /*the flags when secondary release request is pending*/
   //boolean chain_release_request_in_t2l_meas;
   boolean chain_release_request_to_fb;
   //uint16 chain_release_by_subfm;
   //uint16 chain_release_by_ms;
   boolean secondary_chain_is_granted;
   //boolean t2l_to_release_rightaway;
   uint32 tid;
   trm_band_t cur_lte_band;
   trm_band_t next_lte_band;
} tdsl1_drds_secondary_chain_status_type;

typedef enum
{
  TDSL1_LTA = 0,
  TDSL1_QTA = 1,
  TDSL1_QBTA = 2,
  TDSL1_NON_TA = 3
}tdsl1_qta_qbta_enum_type;

typedef enum
{
  TDSL1_UL_NO_DATA = 0,
  TDSL1_UL_SB = 1,
  TDSL1_UL_NON_SB = 2,
  TDSL1_UL_TX_ALL = 3
}tdsl1_qta_qbta_ul_tx_data_type;

typedef enum
{
  TDS_PRI_UNLOCK_INACTIVE = 0,
  TDS_PRI_UNLOCK_ON_GOING = 1
}tdsl1_pri_unlock_state_enum_type;


/*Structure to hold TDSL1 dual sim information */
typedef struct {
  /*Whether TDSL1 is initializing RF and mdsp*/
  boolean tdsl1_in_ds_prep_mode;

  /*True: UE is in DS mode*/
  boolean tdsl1_in_ds_mode;
  
  /*True: UE is in dual active mode*/
  /*False && tdsl1_in_ds_mode == FALSE : US in the single sim mode*/
  boolean tdsl1_in_da_mode;  

  /* To indicate the UE capability
    TRUE: in dual Rx DSDS, the secondary chin could be released
       FALSE: in legacy DSDS, hold the both chains*/
  boolean tdsl1_in_dr_dsds;
  
   /*the mode indication from TRM*/
  uint32 trm_mode;
  
  /*trm single tx dsda*/
  boolean tx_sharing_da_mode;
 
  tdsl1_drds_legacy_fb_metrics_type tdsl1_dr_ds_fb_metrics;
  tdsl1_drds_secondary_chain_status_type tdsl1_dr_sec_chain_status;
 
  /*which G got the LOCK*/
  trm_client_info_t winning_client;
  /*the current client for unlock request*/
  trm_client_info_t current_client;
  
  /*the tuneaway type fromTRM*/
  
  tdsl1_qta_qbta_enum_type perform_QTA; /* indicate LTA, QTA or QBTA */
  
  /*if the QTA gap is emapty, informed by TRM*/
  boolean qta_gap_empty;
  
  /* device mode from RRC*/
  tdsl1_ds_status_change_ind_type tdsl1_ds_status;

  /* pending device mode if it's not at the stopped mode*/
  tdsl1_ds_status_change_ind_type tdsl1_pending_ds_cmd;

  /*pending_mode flag
     TRUE: the device mode has been applied
     FALSE: the device mode is pending, to be applied at the stopped mode*/
  boolean device_mode_config_is_pending;

  /*debug mask to track dual sim status*/
  uint32 tdsl1_ds_debug_mask;

  /*TDSL1 procedures that are holding TRM lock*/
  uint32 tdsl1_ds_proc_mask;

  /*newest TDSL1 procedure that requested for TRM lock*/
  tdsl1_ds_proc_enum_type ds_curr_proc;

  /*newest TDSL1 procedure that requested for TRM lock*/
  tdsl1_ds_proc_enum_type ds_pend_proc;

  /*previous TDSL1 procedure that requested for TRM lock*/
  tdsl1_ds_proc_enum_type ds_last_proc;

  /*Last cphy_setup_cmd procedure that requested for TRM lock*/
  tdsl1_ds_proc_enum_type last_cphy_setup_proc;

  /*Indicate whether cmd is pending waiting for the sleep proc to be completed*/
  boolean trm_lock_sig_pending_on_sleep;

  /* Used to store the TRM lock signal (success/failure) in
    case TRM signal is received in the middle of paging wakeup;
    only applicable if trm_lock_sig_pending_on_sleep = TRUE */
  rex_sigs_type trm_lock_sig;

  /* Flag to indicate Ext cmd processing with no TRM lock */
  boolean process_ext_cmd_without_lock;
  
  tdsl1_qta_state_enum_type tdsl1_qta_sub_state;
  uint32  tdsl1_qta_start_time_in_sclk; /* QTA start timestamp */
  tdsl1_qta_time_info_struct_type tdsl1_qta_start_time;
  
  boolean qta_suspend_srch_demod_in_ho_flag;/* disable during_dl_sync_flag during QTA, if QTA happens during HO and before DL sends ESTABLISH_IND */
  
  sys_modem_as_id_e_type qta_g_asid; /*T2G QTA G sub ASID*/
  /* To time different states of inter-RAT tuneaway. */
  rex_timer_type     l1m_dsds_tuneaway_timer;
  /* CxM priority table */
  cxm_activity_table_s   tdsl1dsds_qta_cxm_priority_table[MAX_ACTIVITY_TIERS];
  boolean                tdsl1dsds_qta_cxm_priority_table_update_flag;
  boolean                qta_fw_cleanup_ind_recvd;

  /* QTA log packet */
  tdsl1_ds_qta_log_packet_struct  tdsl1dsds_qta_log_pkt;
  uint8                  qta_end_rrc_fw_cmd_rec_flag;
  uint8                  pad[3];
  
  boolean                  l1m_g2t_ta_prio_inv_flag;/* L1M trigger TRM priority inversion when receives RRC CMD */
  tdsl1_acq_inv_timer_state_enum_type  acq_inv_timer_set_flag; //acq inverse timer set flag; 

  boolean    stop_qta_cmd_recvd;
  boolean             use_high_priority_in_sib_read;
  boolean             set_sib_timer_by_rnn;
  trm_reason_enum_t   sib_reason_in_reserve_at;
  uint32              sib_read_in_drx_timer_len;
  rex_timer_type      sib_read_priority_set_timer;

  tdsl1_pri_unlock_state_enum_type pri_unlock_state;

  rex_timer_type     l1m_dsds_wait_lock_timer;

  /* client with which gap is opened or closed */
  trm_client_enum_t    qta_target_client;
}tdsl1_ds_db_struct_type;

/*extern tdsl1_ds_db_struct_type tdsl1_ds_db;*/

  /*Macros to handle TDSL1 dual sim procedure bit mask*/
  #define TDSL1_DS_PROC_BMSK(ds_proc) ((uint32)(1 << ((uint32)ds_proc)))
  
  #define TDSL1_DS_RESET_DS_PROC_BMASK() \
  (tdsl1_ds_db.tdsl1_ds_proc_mask = 0)
  
  #define TDSL1_DS_ADD_PROC_TO_DS_BMASK(ds_proc) \
    (tdsl1_ds_db.tdsl1_ds_proc_mask |= TDSL1_DS_PROC_BMSK(ds_proc))
  
  #define TDSL1_DS_REM_PROC_FROM_DS_BMASK(ds_proc) \
    (tdsl1_ds_db.tdsl1_ds_proc_mask &= (~(TDSL1_DS_PROC_BMSK(ds_proc))))
/*****************************************************************************************************************/
/*   Global Variable defination    */
/*****************************************************************************************************************/
/*Information about current TDSL1 Dual SIM procedure*/
extern  tdsl1_ds_db_struct_type tdsl1_ds_db;








/*****************************************************************************************************************/
/*   Function declaration    */
/*****************************************************************************************************************/
void tdrm_request_and_notify(
  /* The client which needs the RF resource */
  tdrm_client_enum_type tdrm_client_id,
  /* The RF resource which is being requested */
  trm_resource_enum_t             resource,
  /* How long the resource will be needed for (in sclks) */
  trm_duration_t                  duration,
  /* Why the resource is needed (used for priority decisions) */
  trm_reason_enum_t               reason,
  sys_proc_type_e_type sub_reason,
  /* Callback to notify client when resource is granted */
  tdrm_grant_callback_type		grant_callback,
  /* Anonymous payload to be echoed through grant_callback() */
  trm_request_tag_t               tag,
  trm_frequency_type_t            tdrm_cur_band,
  /* Force the trm_request_and_notify to be sent out regardless of TDRM state is IN_LOCK*/
  boolean                         force_to_send_out);


void tdrm_retain_lock(tdrm_client_enum_type tdrm_client_id);

void tdrm_grant_callback(
  /* The client which needs the RF resource */
  trm_client_enum_t client, 
  /*The granted Chain*/
  trm_chain_grant_return_type event,
  /*The tag that TDRM passed to TRM in trm_request_and_notify(...)*/
  trm_request_tag_t tag);

void tdrm_grant_callback_cmd_handler(
  /* The client which needs the RF resource */
  trm_client_enum_t client, 

  /*The granted Chain*/
  trm_chain_grant_return_type event,

  /*The tag that TDRM passed to TRM in trm_request_and_notify(...)*/
  trm_request_tag_t tag);

void tdrm_reserve_at(
  /* The client which needs the RF resource */
  tdrm_client_enum_type               tdrm_client_id,
  /* The RF resource which is being requested */
  trm_resource_enum_t             resource,
  /* When the resource will be needed (sclks timestamp) */
  trm_time_t                      when,
  /* How long the resource will be needed for (in sclks) */
  trm_duration_t                  duration,
  /* Why the resource is needed (used for priority decisions) */
  trm_reason_enum_t               reason,
  trm_frequency_type_t            tdrm_cur_band,
  /* a boolean flag to indicate whether to increase wakeup identifier in TDRM<->TRM API */
  boolean                         increase_wakeup_identifier);

trm_grant_return_enum_type tdrm_request(
  /* The client which needs the RF resource */
  tdrm_client_enum_type tdrm_client_id,
  /* The transceiver resource which is being requested */
  trm_resource_enum_t             resource,
  /* How long the resource will be needed for (in sclks) */
  trm_duration_t                  duration,
  /* Why the resource is needed (used for priority decisions) */
  trm_reason_enum_t               reason,
  trm_frequency_type_t            tdrm_cur_band);

void tdrm_change_priority(
  /* The client whose priority is to be changed */
  tdrm_client_enum_type               tdrm_client_id,
  /* The new resource why the RF lock is held (used for priority decisions) */
  trm_reason_enum_t               reason,
  tdrm_change_priority_callback_type change_priority_cb);

void tdrm_release(tdrm_client_enum_type tdrm_client_id);

boolean tdrm_extend_duration
(
  /* The client which is attempting to extend the lock duration */
  tdrm_client_enum_type tdrm_client_id,
  /* The required extension, in sclks, from "now" */
  trm_duration_t                  new_duration);

void tdrm_unlock_trm_cb(trm_unlock_callback_data unlock_data);

void tdrm_unlock_secondary_trm_cb(
  /* The client which is being informed of an event */
  trm_client_enum_t               client,

  /* The event being sent to the client */
  trm_unlock_event_enum_t         event,

  /* Sclk timestamp for TRM_UNLOCK_BY */
  uint32                          unlock_by_sclk);


void tdrm_init(void);

boolean tdrm_check_cli_state_is_in_lock(tdrm_client_enum_type tdrm_client_id);
boolean tdrm_check_cli_state_is_lock_req(tdrm_client_enum_type tdrm_client_id);

trm_band_t tdrm_get_band(uint16  freq);
boolean tdrm_get_ext_cmd_band_info(tdsl1_ext_cmd_type*  ext_cmd_ptr, trm_band_t curr_band_info, trm_band_t* new_band_info);
void tdrm_band_grant_callback(trm_client_enum_t client, 
                              trm_band_type band,          
                              trm_band_grant_return_type granted,   
                              trm_request_tag_t tag);
tdrm_band_change_return_enum_type tdrm_band_change_request(tdrm_client_enum_type          tdrm_client_id,      /* Client requesting a band change */
                                                           trm_frequency_type_t       tds_new_band_info,         /* New band that the client wants to use */
                                                           tdrm_band_grant_callback_type	tds_band_grant_cb,	/* Band Grant Callback */
                                                           trm_request_tag_t tag);

boolean tdrm_is_ue_dr_capable(void);

boolean tdrm_is_ue_in_dr_mode(void);

void tdrm_register_async_callback(void);

void tdrm_async_callback(trm_async_callback_information_type* event_data);

void tdrm_primary_grant_callback(trm_client_enum_t client, trm_req_n_not_return_data data);

void tdrm_primary_grant_callback_cmd_handler(trm_client_enum_t client, trm_req_n_not_return_data data);

void tdrm_secondary_grant_callback(trm_client_enum_t client, trm_req_n_not_return_data data);

void tdrm_secondary_grant_callback_cmd_handler(trm_client_enum_t client, trm_req_n_not_return_data data);

void tdrm_unlock_secondary_cb_for_retain_lock(trm_unlock_callback_data unlock_data);

void tdrm_unlock_secondary_cb(trm_client_enum_t client, trm_unlock_callback_data data);

void tdrm_unlock_secondary_cmd_handler(trm_client_enum_t client, trm_unlock_callback_data data);

void tdrm_secondary_chain_is_needed(boolean is_needed);

void tdrm_get_info(trm_get_info_input_type* get_info_input, trm_get_info_return_type* get_info_return);

void tdrm_register_feature(trm_register_feature_input_info* register_info);

void tdrm_deregister_feature(trm_deregister_feature_input_info* deregister_info);

void tdrm_set_client_state(trm_set_client_state_input_type* input_client_state, trm_set_client_state_output_data* output_client_state);

void tdrm_change_priority_callback(trm_client_enum_t client, trm_modify_return_enum_type result);

void tdrm_send_reserve_at_to_trm(trm_client_enum_t client, trm_resource_enum_t resource, trm_time_t when, trm_duration_t duration, trm_reason_enum_t reason, trm_band_t band);

void tdrm_send_request_and_notify_to_trm(trm_client_enum_t client, trm_resource_enum_t resource, trm_duration_t duration, trm_reason_enum_t reason,sys_proc_type_e_type sub_reason, trm_band_t band);

trm_grant_return_enum_type tdrm_send_request_to_trm(trm_client_enum_t client, trm_resource_enum_t resource, trm_duration_t duration, trm_reason_enum_t reason, trm_band_t band);

void tdrm_grouped_async_callback(trm_grouped_async_callback_information_type* event_data);
void tdrm_request_and_notify_tx_only(trm_duration_t duration, trm_reason_enum_t reason, sys_proc_type_e_type sub_reason, trm_frequency_type_t tdrm_cur_band, tdrm_client_enum_type tdrm_client_id);
void tdrm_send_txrx_group_rnn_to_trm(trm_duration_t duration, 
                                              trm_reason_enum_t reason, 
                                              sys_proc_type_e_type sub_reason,
                                              trm_band_t band,
                                              tdrm_client_enum_type tdrm_client_id);

void tdrm_tx_grant_callback(trm_client_enum_t client, trm_req_n_not_return_data data);
void tdrm_tx_grant_callback_cmd_handler(trm_client_enum_t client, trm_req_n_not_return_data data);
void tdrm_release_tx(void);
void tdrm_unlock_tx_cb(trm_client_enum_t client, trm_unlock_callback_data data);
void tdrm_unlock_tx_cmd_handler(trm_client_enum_t client, trm_unlock_callback_data data);
boolean tdrm_check_trm_client_tx_capability(trm_client_enum_t trm_client, trm_band_t band);

void tdrm_dr_region_update(trm_reason_enum_t new_reason);

#ifdef  FEATURE_PBR_MARKOV
/*===========================================================================
FUNCTION     tdrm_set_drx_cycle

DESCRIPTION
  This function set the pbr modes to TRM

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdrm_set_pbr_modes(trm_reason_enum_t req_reason);

/*===========================================================================
FUNCTION     tdrm_set_drx_cycle

DESCRIPTION
  This function send the drx cycle to TRM.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdrm_set_drx_cycle(uint32 drx_cycle_len);
#endif

/*===========================================================================
FUNCTION     tdrm_exchange

DESCRIPTION
  This function is called to exchange lock between TD and other IRAT .

DEPENDENCIES
  None.

PARAMETERS
  BAND information.

RETURN VALUE
  NONE.

SIDE EFFECTS
  None.
===========================================================================*/
trm_grant_return_enum_type tdrm_exchange(
  /* The client which needs the RF resource */
  tdrm_client_enum_type  tdrm_client_id,
  /* The client which TD will exchange */
  trm_client_enum_t    client_id);


extern void tdsrrc_start_unlock_by_timer_for_tune_away
(
  /* Sclk timestamp for TRM_UNLOCK_BY */
  uint32                          unlock_by_sclk
);

extern void tdsrrc_event_log_dsds_ta_block
(
  void
);
extern void tdsrrc_set_ta_block_reason
(
  tdsrrc_tuneaway_block_reason_type block_reason
);

extern void tdsrrcwrm_release_lock(void);

extern boolean tdrm_is_next_tuneaway_qta(void);

extern trm_reason_enum_t tdrm_get_reason(void);
  
extern void tdrm_sync_reason(trm_reason_enum_t reason);

extern void tdrm_unsync_reason(void);

extern trm_reason_enum_t tdrm_get_client_req_reason(tdrm_client_enum_type tdrm_client_id);

extern void tdrm_modify_reason_cmd_handler(trm_client_enum_t client, trm_modify_reason_return_type result);

extern void tdrm_modify_band_cmd_handler(trm_client_enum_t client, trm_modify_band_return_type result);

extern boolean tdrm_check_trm_client_is_in_lock(trm_client_enum_t trm_client);

extern boolean tdrm_is_next_tuneaway_page_decode(void);

extern boolean tdrm_is_tx_reason(trm_reason_enum_t reason);

extern trm_priority_comparison_enum_type tdrm_comp_prio(trm_reason_enum_t reason1, trm_reason_enum_t reason2);

/*===========================================================================
FUNCTION     tdrm_check_if_no_client_need_tx

DESCRIPTION
  check if neither RRC nor L1 needs TX

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  boolean

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean tdrm_check_if_no_client_need_tx(void);

/*===========================================================================
FUNCTION     tdrm_hist_log_insert

DESCRIPTION
  \

DEPENDENCIES
  None.

PARAMETERS
  \

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdrm_hist_log_insert(
  /* The client which needs the RF resource */
  tdrm_client_enum_type tdrm_client_id,

  /* Why the resource is needed (used for priority decisions) */
  trm_reason_enum_t               reason,

  /* Anonymous payload to be echoed through grant_callback() */
  trm_request_tag_t               tag,

  tdrm_log_enum_type log_type);




/*===========================================================================
FUNCTION     tdrm_protocol_stack_status_ind

DESCRIPTION
  to inform TRM TDS stack status.

DEPENDENCIES
  None

PARAMETERS
  trm_protocol_stack_status_e_type, sys_modem_as_id_e_type

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/

void tdrm_protocol_stack_status_ind(trm_protocol_stack_status_e_type stack_status, sys_modem_as_id_e_type as_id);

#ifdef FEATURE_PBR_RANDOMIZATION
/*===========================================================================

FUNCTION  tdrm_inform_grant_event

DESCRIPTION inform TRM about TDS paging wakeup info


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdrm_inform_grant_event(tdsl1_ds_proc_enum_type ds_proc);
#endif /* FEATURE_PBR_RANDOMIZATION */

#ifdef FEATURE_TDSCDMA_ADAPTIVE_PAGE_SKIPPING
void tdrm_register_aps(void);

void tdrm_aps_state_update( boolean enable);
#endif

#endif /* #ifndef TDSTL1TRM_H */






