#ifndef TDSULDPCHCTRL_H
#define TDSULDPCHCTRL_H

/*===========================================================================
                    T D S U L D P C H C T R L . H

GENERAL DESCRIPTION
  This header file contains the MACRO definitions, variables and functions
  statement for tdsuldpch.c
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright (c) 2011 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsuldpchctrl.h#1 $ 
  $DateTime: 2019/08/19 10:51:23 $ 
  $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
08/02/11    cxc     File created.
                    
===========================================================================*/
#include "tdscdma_variation.h"
#include "tdsl1rrcif.h"
#include "tdsl1macif.h"
#include "tdsl1macdata.h"
#include "tdsl1mtask.h"
#include "tdsl1ulcfg.h"
#include "tdsl1m_rcinit.h"
  
#ifdef FEATURE_QSH_EVENT_METRIC
#include "tdsl1_qsh_ext.h"
#endif

/* ----------------------------------------------------------------------- */
/*                          Macro and Constant                             */
/* ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_PWR_TFC_ELMN

/* 20ms slide window by step of 5ms for TFC elimination */
#define TDSULDPCHCTRL_TFC_ELMN_SLIDE_WIN_SIZE                4

#define TDSULDPCHCTRL_TFC_ELMN_STOP_10MS                     1
#endif

//#ifdef FEATURE_WCDMA_SUSP_BFI_TWO_FR_BF_ACT
#define TDSULDPCHCTRL_INVALID_CFN_TO_STOP_REQ_MAC_DATA  ((uint32)-1)
//#endif


#define TDSULDPCHCTRL_INVALID_SBGP       0
/* Set default SBGP 40ms */
#define TDSULDPCHCTRL_DEFAULT_SBGP       4

#define TDSULDPCHCTRL_LOG2_TABLE_SIZE    33

/* DPCH desired power SF16 normalization. Add 12 dB to tx power */
#define TDSULDPCHCTRL_DESIRED_PWR_NORMALIZED_DELTA_DB 12

/* Return TRUE if UL DPCH state is active */
#define TDS_UL_DPCH_STATE_IS_ACTIVE() \
  (tdsuldpchctrl_global->tdsuldpchctrl_state != TDS_UL_DPCH_STATE_INACTIVE)

/* Return TRUE if UL DPCH state is active and not ready */
#define TDS_UL_DPCH_STATE_IS_ACTIVE_AND_NOT_READY() \
  ((tdsuldpchctrl_global->tdsuldpchctrl_state != TDS_UL_DPCH_STATE_INACTIVE) && \
  (tdsuldpchctrl_global->tdsuldpchctrl_state != TDS_UL_DPCH_STATE_READY))

#define TDS_UL_DPCH_STATE_IS_DISABLE() \
  (tdsuldpchctrl_global->tdsuldpchctrl_state == TDS_UL_DPCH_STATE_DISABLED)


#ifdef FEATURE_TDSCDMA_PWR_TFC_ELMN
/* If TDM is not configured, the previous frame UL DPCH state is SB or TX 
 * which means TX is ON because SB and DPCH data shall be tranmitted at the 
 * current frame. Else if TDM is configured, available frame should be considered.
 */
#define TDS_PREV_FRM_UL_DPCH_STATE_IS_SB_OR_TX() \
  ((tdsuldpchctrl_global->tdsuldpchctrl_prev_frm_state == TDS_UL_DPCH_STATE_SB) || \
  (tdsuldpchctrl_global->tdsuldpchctrl_prev_frm_state == TDS_UL_DPCH_STATE_TX))

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/* FOR FREE FLOATING ... A mutex shared between L1 and MAC*/
/* Mutex to perform atomic state transistions */

#define TDSL1_TFC_EXCESS_PWR_INTLOCK()  REX_ISR_LOCK(&tdsl1_tfc_excess_pwr_states_mutex)
#define TDSL1_TFC_EXCESS_PWR_INTFREE()  REX_ISR_UNLOCK(&tdsl1_tfc_excess_pwr_states_mutex)

#else

#define TDSL1_TFC_EXCESS_PWR_INTLOCK()  {}
#define TDSL1_TFC_EXCESS_PWR_INTFREE()  {}

#endif /*FEATURE_TDSCDMA_FREE_FLOATING_TASK*/

#endif/* FEATURE_TDSCDMA_PWR_TFC_ELMN */

#define TDSULDPCHCTRL_SB_BIT_IN_QTA   0x01
#define TDSULDPCHCTRL_DATA_BIT_IN_QTA   0x02


#define TDS_UL_DPCH_ISR_LOCK()    REX_ISR_LOCK(&tdsuldpchctrl_isr_mutex)
#define TDS_UL_DPCH_ISR_UNLOCK()  REX_ISR_UNLOCK(&tdsuldpchctrl_isr_mutex)


/* ----------------------------------------------------------------------- */
/*                        Enumeration Definition                           */
/* ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_E4A_OPT
typedef enum
{
  TDSULDPCH_E4A_EVA_INVALID,
  TDSULDPCH_E4A_EVA_PWR_REACH_MTPL,
  TDSULDPCH_E4A_EVA_PWR_BELOW_MTPL,
} tdsuldpch_e4a_eva_result_enum_type;

#endif /*FEATURE_TDSCDMA_E4A_OPT*/


/* ----------------------------------------------------------------------- */
/*                         Structure Definition                            */
/* ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_PWR_TFC_ELMN

/* For each TFCI, the structure stores power elimination history in the past 2 
 * frames and maintains power state subframe by subframe. 
 */
typedef struct 
{
  /* The number of uplink timeslot whose BetaMax are less than Beta of this TFC 
   * in the past 2 radio frame. It saves the history number for each subframe. 
   */
  uint8 num_ts_elmn_hist[TDSULDPCHCTRL_TFC_ELMN_SLIDE_WIN_SIZE];

  /* The number of uplink timeslot in the past 2 radio frame. It saves the 
   * history number for each subframe. 
   */
  uint8 num_ts_hist[TDSULDPCHCTRL_TFC_ELMN_SLIDE_WIN_SIZE];

  /* Flag indicate the TFCI in Excess Power state or not. */
  boolean  excess_pwr;
}tdsuldpchctrl_tfc_elmn_tab_type;


/* Save each UL timeslot Tx power info */
typedef struct 
{
  uint8  ul_ts_num;
  /* Q12.4 format */
  int16 total_tx_pwr;
  /* Q12.4 format */
  int16 max_tx_pwr;
  
  /*uint16 backoff_pwr;  */
  
}tdsuldpchctrl_ts_tx_pwr_info_type;


/* The struture is used to save Tx power info for each timeslot from FW per 
 * subframe, includes the total Tx power, the maximum allowed Tx power and the 
 * power back off value. 
 */
typedef struct 
{
  uint8 num_ul_ts;
  
  tdsuldpchctrl_ts_tx_pwr_info_type ul_ts_tx_pwr_info[TDSL1_UL_MAX_NUM_TS];
  
} tdsuldpchctrl_fw_tx_pwr_info_type;


//extern tdsl1_ul_tfc_pwr_state_enum_type tdsuldpchctrl_global->tdsl1_ul_tfc_pwr_state[TDSL1_UL_TFC_MAX];

#endif /* FEATURE_TDSCDMA_PWR_TFC_ELMN */

typedef struct
{
	uint8 num_ts;	/*slot occupation of this TFC*/
	tdsl1_tti_enum_type max_tti; /*max tti of trch in this TFC*/
}tdsuldpchctrl_tfc_format_type;

/*struct for dynamic mem*/
#define TDSULDPCHCTRL_HIST_MAX_CNT_8 8
#define TDSULDPCHCTRL_HIST_MAX_CNT_16 16

#ifdef FEATURE_TDSCDMA_PLT 
#define TDSULDPCHCTRL_HIST_MAX_CNT_32 32
#define TDSULDPCHCTRL_HIST_MAX_CNT_64 64
#endif //FEATURE_TDSCDMA_PLT

#ifdef FEATURE_TDSCDMA_E4A_OPT
#define TDSULDPCH_E4A_PWR_OFFSET 0  /*dB*/
#define TDSULDPCH_E4A_PWR_BAR_TIMER 4  /*frames*/
#define TDSULDPCH_E4A_PWR_BAR_RECOVERY_TIMER 16  /*frames*/
#endif /*FEATURE_TDSCDMA_E4A_OPT*/

typedef struct
{
  #ifdef FEATURE_TDSCDMA_PWR_TFC_ELMN

  /* Elimination history buffer frame index */
  uint8 tdsuldpchctrl_tfc_elmn_hist_frame_idx;

  /* For each TFCI, the structure stores power elimination history in the past 2 
   * frames and maintains power state subframe by subframe. 
   */
  tdsuldpchctrl_tfc_elmn_tab_type   tdsuldpchctrl_tfc_elmn_tab[TDSL1_UL_TFC_MAX];

  /* Array sorted by BetaD in ascending order. It maps each Beta to its associated 
   * TFCI. The array element value is the corresponding index of 
   * tdsl1_ul_gain_factor[TDSL1_UL_TFC_MAX].  
   */
  uint16 tdsuldpchctrl_sorted_gain_tfci_map[TDSL1_UL_TFC_MAX];

  /* Status for each TFC at the current subframe: 
   * 0: supported; 1: Excess; 2: No change(DTXed or TDMed or Out-of-sync) 
   */
  tdsl1_ul_tfc_pwr_state_enum_type tdsl1_ul_tfc_pwr_state[TDSL1_UL_TFC_MAX];

  tdsuldpchctrl_fw_tx_pwr_info_type tdsuldpchctrl_fw_tx_pwr_info;

  /* Flag indicates whether do TFC elimination evaluation on current subframe.
   * TRUE: do evaluation; FALSE: not do evaluation.
   * For DPCH initial setup or recfg, start the evaluation after L1SW collect 4 
   * subframes pwr info from FW. After that, do evaluation each subframe for 
   * speical burst or data by using 20ms window size by step of 5ms.
   */
  boolean tdsuldpchctrl_tfc_elmn_eval_flg;

  /* For UL DPCH reconfigurtion, L1SW always configure TFCs one frame advance. 
   * But UE still has data over the air next frame after recfg which is based on 
   * old configuration and L1SW shall ignore this frame.
   * TRUE means stop TFC elimination, FALSE otheriwise
   */
  boolean tdsuldpchctrl_tfc_elmn_is_stop_for_recfg;
  /* Use to delay one frame to do TFC elimination */
  uint8 tdsuldpchctrl_tfc_elmn_stop_cnt_for_recfg;

  /* Save the previous frame state of UL DPCH for TFC elmination schedule.
   * If the state is SB or TX, L1SW can get Tx pwr info from FW and do evaluation.
   * If the state is INACTIVE, READY, NO TX or DISABLED, no need to do such thing 
   * since no data transmission. 
   * Note that the variable is not included in FEATURE_TDSCDMA_PWR_TFC_ELMN because 
   * it can be used easily for debugging.
   */
  tdsl1_ul_dpch_state_enum_type tdsuldpchctrl_prev_frm_state;

  /* Array save the gamma square for each SF. The sequence is the same as 
   * TDSCDMA_FW_DPCH_TX_PWR_TIMING_CONFIG_CMD. From TS1 to TS5.
   * The format is Q11.5. 
   */
  uint16 tdsuldpchctrl_cfg_sf_gamma_square_map[TFW_MAX_UL_SLOT][TFW_MAX_UL_PHY_PER_SLOT];
  boolean tdsul_special_test_settings_status ;

  #endif /* FEATURE_TDSCDMA_PWR_TFC_ELMN */

  int16  tdsuldpchctrl_cfg_sf_txpower_gain_map[TFW_MAX_UL_SLOT][TFW_MAX_UL_PHY_PER_SLOT];


  /* tdsuldpchctrl_global->tdsuldpchctrl_pre_frm_beta[0] for CCTrCH1, tdsuldpchctrl_global->tdsuldpchctrl_pre_frm_beta[1] for CCTrCH2 */
  uint16 tdsuldpchctrl_pre_frm_beta[TFW_MAX_UL_CCTRCH_NUM];


  /*Three cases need send special burst:
   1.For DPCH initial setup and reconfig, SB shall be programmed continuously 
   at most 160ms following detection of in sync. 
   2.In sync from out of sync, SB shall be programmed continuously until the next 
   minimum TTI boundary. 
   3.DTX, only one frame SB shall be programmed.

   tdsuldpchctrl_global->tdsuldpchctrl_sb_max_sent_num:
   The max number of frame that send special burst. 
   Different case has different value:
   1.tdsuldpchctrl_global->tdsuldpchctrl_sb_max_sent_num=16; 
   2.calculate according to minimum TTI boundary; 
   3.tdsenc_sb_max_sent_num=1.

   tdsuldpchctrl_global->tdsuldpchctrl_sb_counter:
   Compare with tdsenc_sb_max_sent_num, tdsenc_sb_counter is used to judge whether special burst 
   transmission shall be stopped.
   tdsenc_sb_counter plus one every time send special burst. Reset to 0 when reach 
   tdsenc_sb_max_sent_num or change to UL_DPCH_STATE_TX.
   */
  uint16 tdsuldpchctrl_sb_max_sent_num;
  uint16 tdsuldpchctrl_sb_counter;

  /* UL won't enter DTX pattern during the first 160ms after DL in-sync.
   * UL shall transmit data if got data or special burst if no data got. 
   */
  uint8 tdsuldpchctrl_160ms_counter_after_dl_in_sync;

  /* SBGP is Special Burst Generation Period. It is provided by higher layers
   through IE "Special Burst Scheduling" in UPLINK PHYSICAL CHANNEL CONTROL */
  uint16 tdsuldpchctrl_dtx_sbgp;

  /* Enter DTX or not */
  boolean tdsuldpchctrl_enter_dtx_flg;

  /* Counter to check whehter met SBGP */
  uint16 tdsuldpchctrl_dtx_sbgp_counter;

  /* Used to indicate NW enable/disable DTX pattern. 
   * 1.UL DTX is disabled by default.
   * 2.NW shall turn off the UL DTX by sending Uplink Physical Channel Control(UPCC) 
   * message with IE "Special Burst Scheduling" absent.
   * 3.NW shall turn on the UL DTX and configure the SGBP by sending Uplink Physical 
   * Channel Control(UPCC) message with IE "Special Burst Scheduling" included.
   * 4.If DTX is disabled, SB shall be transmitted continuously if no data got 
   * from MAC.  
   */
  boolean tdsuldpchctrl_dtx_is_enabled;

  /* For test purpose, UE can not support UL DTX feature even if NW require to 
   * enable UL DTX. The default value support UL DTX feature.
   */
  boolean tdsuldpchctrl_support_dtx;

  /* Save the max tti frame num */
  uint8 tdsuldpchctrl_max_tti_frm_num;

  /* Indicates that L1 setup is over and waiting for MAC configure done.
   * TURE: wait becuase MAC setup is not finished. FALSE:no need to wait */
  boolean tdsuldpchctrl_wait_mac_cfg_done;

  /* The flag is set in case of DPCH initial setup or reconfiguration and used to
   * indicate UL TDM is configured or not. 
   * TRUE: UL TDM is configured. FALSE: UL TDM is not configured.
   */
  boolean tdsuldpchctrl_tdm_cfg;

  /* The flag is set to TRUE while two frame before UL TDM activation time */

  /* Flag is TRUE for
   * DPCH initial setup
   * HHO
   * inter-RAT HHO or HHO failure recovery.
   */
  boolean tdsuldpchctrl_init_setup_flg;

  /* Indicate if UL DPCH is in reconfig */
  boolean tdsuldpchctrl_recfg_flg;

  /* Indicate seamless reconfig which is configured by RRC */
  boolean tdsuldpchctrl_seamless_recfg_flg;

  /* Flag is TRUE for BHO */
  boolean tdsuldpchctrl_bho_flg;

  /* Used to determine whether the current frame is the 1st frame after DPCH init
   * setup by L1M. 
   * Due to tdsenc_get_data() is triggered every 5ms. But for DPCH,
   * it shall be triggered every 10ms. For better performance, notice that the 1st 
   * tdsenc_get_data() after PRACH drop shall be triggered no matter whether it 
   * is frame boundary or not.
   * TRUE: when L1M add DPCH in tdsulcmd_phychan_add_cmd().
   * FALSE: if current frame is not the 1st frame after DPCH setup.
   */
  boolean tdsuldpchctrl_1st_frm_after_init_setup_flg;

  /* Variable used to store parameters in CPHY_UL_PHY_CHANNEL_CTRL_REQ */
  tdsl1_ul_phych_control_cmd_type  tdsuldpchctrl_ul_phych_ctrl_cmd;

 /* Indicate whether L1 receive CPHY_UL_PHY_CHANNEL_CTRL_REQ from RRC */
  boolean tdsuldpchctrl_rcv_upcc_cmd_flg;

  /* Indicate whether SBGP is changed by CPHY_UL_PHY_CHANNEL_CTRL_REQ */
  boolean tdsuldpchctrl_sbgp_is_changed;

  /* If "Beacon PL Est" in "Uplink DPCH info"/"Uplink Physical Channel Control" is 
   * set TRUE by NW, pathloss shall be added in both open and closed loop Tx power
   * calculation.
   * Else, L1SW only add it in open loop Tx power.
   */
  boolean tdsuldpchctrl_update_pathloss_in_open_loop;
  boolean tdsuldpchctrl_update_pathloss_in_close_loop;

  /* UL DPCH state */
  tdsl1_ul_dpch_state_enum_type tdsuldpchctrl_state;

  /* Flag to indicate that the UE is out of sync.
   * When in sync, UL TX shall be delayed (PA off) until phychan establishment
   * indication and tdsuldpchctrl_global->tdsuldpchctrl_out_of_sync becomes FALSE.
   */
  boolean tdsuldpchctrl_out_of_sync;

  /* This flag indicates the 1st time DL in-out-sync status after DPCH initial 
   * setup, HHO or BHO. 
   * Set FALSE when DL in-sync. Reset to TRUE for initial setup/HHO/BHO
   */
  boolean tdsuldpchctrl_out_of_sync_after_cfg_flg;

  /* This flag is used to control the time to start SB counter in case of DPCH 
   * initial setup or recfg. SB shall be sent at most 160ms+10ms
   * after DL in-sync if no data got from MAC. Then enter DTX pattern.
   */
  boolean tdsuldpchctrl_160ms_after_dl_in_sync_flg;

  uint8 tdsuldpchctrl_crf;  /* current radio frame number */
  uint8 tdsuldpchctrl_nrf;  /* next radio frame number */
  uint8 tdsuldpchctrl_nnrf; /* next next radio frame number */

  /* CFN passed to MAC for build frame indication */
  uint8 tdsuldpchctrl_build_frm_ind_cfn;

  /* Flag to indicate if ENC driver is in a cleanup */
  boolean tdsuldpchctrl_cleanup_flg;

  /* CFN to suspend build_frame_ind() which means do not request data from MAC
   * two frames before act time. 
   */
  uint32  tdsuldpchctrl_cfn_to_stop_req_mac_data;

  /* indicator whether UL TX data buffer is empty at cleanup */
  boolean tdsuldpchctrl_tx_data_trashed;

  /* Record latest DPCH_CONFIG message which was sent to FW 
   * in DPCH TX procedure. */
  tfw_ul_dpch_config_cmd_t tdsuldpchctrl_hist_config_cmd[2];

  /* Record latest DPCH_TIMING_CONFIG message which was sent to FW 
   * in DPCH TX procedure. */
  tfw_dpch_tx_pwr_timing_config_cmd_t tdsuldpchctrl_hist_timing_config_cmd;

  /* Store mac UL data received and DPCH config commands sent to FW
   * The first 8 (or 32) logs contain the first mac data received/dpch command sent after CPHY setup, while
   * The later 8 (or 32) logs contain the latest mac data received/dpch command sent (as the circular queue way) 
   */
  uint8 tdsuldpchctrl_hist_mac_ul_tx_cnt;
  tdsl1_ul_tx_data_type tdsuldpchctrl_hist_mac_ul_tx_buf[TDSULDPCHCTRL_HIST_MAX_CNT_16];
  uint16 tdsuldpchctrl_hist_mac_ul_tx_ssfn_log[TDSULDPCHCTRL_HIST_MAX_CNT_16];

  #ifdef FEATURE_TDSCDMA_PLT 
  uint8 tdsuldpchctrl_hist_config_cmd_log_cnt;
  tfw_ul_dpch_config_cmd_t tdsuldpchctrl_hist_config_cmd_log[TDSULDPCHCTRL_HIST_MAX_CNT_64];
  uint16 tdsuldpchctrl_hist_config_cmd_seq_ssfn_log[TDSULDPCHCTRL_HIST_MAX_CNT_64];
  uint16 tdsuldpchctrl_hist_config_cmd_seq_sfn_log[TDSULDPCHCTRL_HIST_MAX_CNT_64];
  uint8 tdsuldpchctrl_hist_config_cmd_seq_cfn_log[TDSULDPCHCTRL_HIST_MAX_CNT_64];

  uint16 tdsuldpchctrl_hist_config_cmd_wall_ssfn_log[TDSULDPCHCTRL_HIST_MAX_CNT_64];
  uint16 tdsuldpchctrl_hist_config_cmd_wall_chipx8_log[TDSULDPCHCTRL_HIST_MAX_CNT_64];
  uint16 tdsuldpchctrl_hist_config_cmd_rx_ssfn_log[TDSULDPCHCTRL_HIST_MAX_CNT_64];
  uint16 tdsuldpchctrl_hist_config_cmd_rx_chipx8_log[TDSULDPCHCTRL_HIST_MAX_CNT_64];  
  #endif /*FEATURE_TDSCDMA_PLT */

  #if defined(FEATURE_TDSCDMA_UL_MEMPOOL_LOGGING) || defined(FEATURE_TDSCDMA_PLT)
  boolean tdsuldpch_enable_sb_Logging;
  #endif /* FEATURE_TDSCDMA_UL_MEMPOOL_LOGGING || FEATURE_TDSCDMA_PLT*/

  #ifdef FEATURE_TDSCDMA_E4A_OPT
  boolean tdsuldpchctrl_e4a_barred;
  uint16 tdsuldpchctrl_e4a_counter;
  #endif /*FEATURE_TDSCDMA_E4A_OPT*/

  /* Timg advance in 1/8 chip */
  int16 tdsuldpchctrl_tadv;
  /* TPC accumulator */
  int8  tdsuldpchctrl_tpc_accu;

  /* By setting TPC step size/ss step size to 0, provide debug method to disable 
   * close loop power control and synchronization 
   */
  boolean tdsuldpchctrl_enable_close_loop_pwr_ctrl;
  boolean tdsuldpchctrl_enable_close_loop_sync_ctrl;
  /* Provide a way to hack dpchTxPwr */
  boolean tdsuldpchctrl_debug_hack_txpwr;
  int16 tdsuldpchctrl_debug_dpchTxPwr;

  #if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
  boolean tdsuldpchctrl_is_all_ul_ts_punched;
  #endif/*FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT||FEATURE_TDSCDMA_DMO_SUPPORT*/

  tdsuldpchctrl_tfc_format_type tdsuldpchctrl_tfc_format[TDSL1_UL_TFC_MAX];
  //boolean tdsuldpchctrl_is_all_ul_ts_punched;
  boolean tdsuldpchctrl_allowed_tfci_list[TDSL1_UL_TFC_MAX];
  uint8 tdsuldpchctrl_uldpch_ts_bitmap;

  int16 tdsuldpchctrl_all_ts_filtered_ul_tx_power_q4; /*Set to be invalid value*/

  #ifdef FEATURE_TDSCDMA_DSDA
  boolean tdsuldpchctrl_already_bump_prio_for_srb;
  uint8 tdsuldpchctrl_srb_tti_count;
  #endif /*FEATURE_TDSCDMA_DSDA*/

}tdsuldpchctrl_global_struct_type;

extern tdsuldpchctrl_global_struct_type *tdsuldpchctrl_global;

/* ----------------------------------------------------------------------- */
/*                          Variable Statement                             */
/* ----------------------------------------------------------------------- */


/* ----------------------------------------------------------------------- */
/*                          Function Statement                             */
/* ----------------------------------------------------------------------- */

#ifdef FEATURE_TDSCDMA_PWR_TFC_ELMN
/*===========================================================================
FUNCTION        tdsuldpchctrl_init_tfc_elmn

DESCRIPTION     This function initializes data structures for power based
                TFC selection/elimination. Then sort BetaD in ascending order.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    Some global data structures are initialized.
===========================================================================*/
void tdsuldpchctrl_init_tfc_elmn( void );

/*===========================================================================
FUNCTION tdsuldpchctrl_set_special_test_settings_status

DESCRIPTION
  This function is used to set the NV69731 status.
  if this NV status is on, means UE is performing RRM test, we need to do TFC elmn
  if this NV status is off, we won't do TFC elmn. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
==========================================================================*/

void tdsuldpchctrl_set_special_test_settings_status(tdsrrc_special_test_settings_status_e_type special_test_status);

#endif

void tdsuldpchctrl_init_sf_txpower_gain( void );



#ifdef FEATURE_TDSCDMA_HSUPA
/*=========================================================================

FUNCTION tdsuldpchctrl_get_hist_dpch_config

DESCRIPTION
  This function is invoked by HSUPA to get the latest two DPCH_CONFIG for
  calculation of power headroom for EPUCH transmission.

DEPENDENCIES
  DPCH_CONFIG is available only if DPCH is configured.
  Two DPCH_CONFIG are put in an array alternately. That's to say, if the 
  first DPCH_CONFIG is put in first place, the second one is put in second
  place, the third is put in first place again and overlap the first 
  DPCH_CONFIG, and so on.

RETURN VALUE
  The pointer to the start address of DPCH_CONFIG array.

SIDE EFFECTS
  None.

=========================================================================*/
tdsl1_ul_dpch_config_struct_type * tdsuldpchctrl_get_hist_dpch_config(void);

#endif /* FEATURE_TDSCDMA_HSUPA */


/*=========================================================================
FUNCTION        tdsuldpchctrl_init

DESCRIPTION     This function initialize the variables defined in this
                module.

DEPENDENCIES    

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsuldpchctrl_init(void);


/*=========================================================================

FUNCTION tdsuldpchctrl_process_dpch_data

DESCRIPTION
  This function 

DEPENDENCIES
  The function is mainly used for DPCH state transition and each state handling.
  1.DPCH state transition and each state handling
  2.MAC data request

RETURN VALUE
  return if any exception preventing DPCH processing or no need to run DPCH 
  processing.

SIDE EFFECTS
  None


=========================================================================*/
void tdsuldpchctrl_process_dpch_data(void);

/*=========================================================================

FUNCTION ENC_TX_ENABLE

DESCRIPTION
  Enable the PA and the TX chain. This is typically used when we are exiting
  an out-of-sync condition and return to in-sync operation.

DEPENDENCIES

RETURN VALUE
  return FALSE if not in TDSULSTATES_PRACH_TX or TDSULSTATES_DPCH_TX

SIDE EFFECTS
  None.

=========================================================================*/
boolean tdsuldpchctrl_tx_enable(void);

/*=========================================================================

FUNCTION ENC_TX_DISABLE

DESCRIPTION
  Disable the PA and the TX chain. This is typically used when we are
  encounter an out-of-sync condition.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsuldpchctrl_tx_disable(void);


#if 0
/*===========================================================================
FUNCTION tdsuldpchctrl_unfreeze_tpc

DESCRIPTION
  This function unfreezes TPC accum.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  Unfreezes TPC accum
==========================================================================*/
void tdsuldpchctrl_unfreeze_tpc(void);
#endif

/*===========================================================================
FUNCTION	tdsuldpchctrl_norm_l

DESCRIPTION 
  
  Produces the number of left shift needed to normalize the 32 bit varia- 
  ble l_var1 for positive values on the interval with minimum of 1073741824 and 
  maximum of 2147483647, and for negative values on the interval with minimum of
  -2147483648 and maximum of -1073741824; in order to normalize the result, the 
  following operation must be done :         |
               norm_L_var1 = L_shl(L_var1,norm_l(L_var1))
               
  Complexity weight : 30

  Input: L_var1.
     32 bit long signed integer (Word32) whose value falls in the range : 
     0x8000 0000 <= var1 <= 0x7fff ffff.
  
  

DEPENDENCIES 
  None

RETURN VALUE 
  var_out: 16 bit short signed integer (Word16) whose value falls in the range: 
  0x0000 0000 <= var_out <= 0x0000 001f.                

SIDE EFFECTS 
  None
===========================================================================*/
int16 tdsuldpchctrl_norm_l(int32 L_var1);

/*===========================================================================
FUNCTION tdsuldpchctrl_init_num_trch_for_l1_ul_tx_buf

DESCRIPTION
  This function is used by other module to initialize num_trch for 
  tdsenc_l1_ul_tx_buf variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_init_num_trch_for_l1_ul_tx_buf(void);


/*===========================================================================
FUNCTION tdsuldpchctrl_set_ul_dpch_state

DESCRIPTION
  This function is used by other module to set UL DPCH state. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsuldpchctrl_set_ul_dpch_state(tdsl1_ul_dpch_state_enum_type ul_dpch_state);


/*===========================================================================
FUNCTION tdsuldpchctrl_set_l1_ul_tx_buf

DESCRIPTION
  This function is used by other module to set tdsenc_l1_ul_tx_buf variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_l1_ul_tx_buf(tdsl1_ul_tx_data_type *l1_ul_tx_buf);

/*===========================================================================
FUNCTION tdsuldpchctrl_set_cleanup_flg

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_cleanup_flg variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_cleanup_flg(boolean cleanup_flg);

/*===========================================================================
FUNCTION tdsuldpchctrl_set_wait_mac_cfg_done

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_wait_mac_cfg_done variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_wait_mac_cfg_done(boolean wait_mac_cfg_done);


/*===========================================================================
FUNCTION tdsuldpchctrl_set_cfn_to_stop_req_mac_data

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_cfn_to_stop_req_mac_data 
  variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_cfn_to_stop_req_mac_data(uint32 cfn_to_stop_req_mac_data);


/*===========================================================================
FUNCTION tdsuldpchctrl_set_init_setup_flg

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_init_setup_flg variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_init_setup_flg(boolean dpch_init_setup_flg);


/*===========================================================================
FUNCTION tdsuldpchctrl_set_init_setup_flg

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_bho_flg variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_bho_flg(boolean dpch_bho_flg);


/*===========================================================================
FUNCTION tdsuldpchctrl_set_first_frm_after_init_setup_flg

DESCRIPTION
  This function is used by other module to set 
  tdsuldpchctrl_1st_frm_after_init_setup_flg variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_first_frm_after_init_setup_flg(boolean first_frm_after_init_setup_flg);


/*===========================================================================
FUNCTION tdsuldpchctrl_set_recfg_flg

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_recfg_flg 
  variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_recfg_flg(boolean reconfig_flg);

/*===========================================================================
FUNCTION tdsuldpchctrl_set_seamless_recfg_flg

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_seamless_recfg_flg 
  variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_seamless_recfg_flg(boolean seamless_recfg_flg);

/*===========================================================================
FUNCTION tdsuldpchctrl_set_rcv_ul_phych_ctrl_cmd_flg

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_rcv_upcc_cmd_flg 
  variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_rcv_ul_phych_ctrl_cmd_flg(boolean rcv_ul_phych_ctrl_cmd_flg);

/*===========================================================================
FUNCTION tdsuldpchctrl_set_ul_phych_ctrl_cmd_var

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_ul_phych_ctrl_cmd 
  variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_set_ul_phych_ctrl_cmd_var(tdsl1_ul_phych_control_cmd_type *ul_phych_control_cmd);


/*===========================================================================
FUNCTION tdsuldpchctrl_set_tdm_cfg_flg

DESCRIPTION
  This function is used by other module to set tdsuldpchctrl_tdm_cfg 
  variable. 
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsuldpchctrl_set_tdm_cfg_flg(boolean tdm_flg);

/*=========================================================================

FUNCTION tdsuldpchctrl_set_ul_cctrch_ts_pair

DESCRIPTION
  This function set UL CCTrCh-TimeSlot Pairs for FW UL close loop power control.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsuldpchctrl_set_ul_cctrch_ts_pair(tfw_dpch_tx_pwr_timing_config_t *cfg_msg_ptr);


/*=========================================================================

FUNCTION tdsuldpchctrl_save_tadv_pwr_before_ho

DESCRIPTION
  This function is used to save TAdv and tx power before HO. If HO failed, they
  can be used for recovery.

DEPENDENCIES

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/
void tdsuldpchctrl_save_tadv_pwr(void);


/*=========================================================================

FUNCTION tdsenc_update_trch_active_status

DESCRIPTION
  Get old cell tadv which is saved before handover

DEPENDENCIES
  None
  
RETURN VALUE
  int16 in 1/8 chip

SIDE EFFECTS
  None.

=========================================================================*/
int16 tdsuldpchctrl_get_cell_tadv(void);


/*=========================================================================

FUNCTION tdsuldpchctrl_get_recfg_flg

DESCRIPTION
  Get reconfiguration flag

DEPENDENCIES
  None
  
RETURN VALUE
  int16 in 1/8 chip

SIDE EFFECTS
  None.

=========================================================================*/
boolean tdsuldpchctrl_get_recfg_flg(void);

/*===========================================================================
FUNCTION tdsuldpchctrl_delay_drop_to_next_frm

DESCRIPTION
  This function is used to delay DPCH drop. After receive drop from L1M, to 
  avoid missing current frame data for graceful drop, the drop will be done 
  at the beginning of the next radio frame boundary.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsuldpchctrl_delay_drop_to_next_frm(void);


/*===========================================================================
FUNCTION        tdsuldpchctrl_stop_build_frame_ind_for_recfg

DESCRIPTION     This function cleans UL TX data buffer and stops
                build_fram_ind()

DEPENDENCIES    Must be called for DCH seamless recfg

RETURN VALUE    None

SIDE EFFECTS    As said above
==========================================================================*/
void tdsuldpchctrl_stop_build_frame_ind_for_recfg(void);

//#ifdef FEATURE_WCDMA_SUSP_BFI_TWO_FR_BF_ACT
/*===========================================================================
FUNCTION        ENC_SUSPEND_BFI_TWO_FRAMES_BEFORE_ACT_TIME

DESCRIPTION     This function suspends build_fram_ind() two frames before the
                activation time. It does NOT clean UL TX data buffer.

DEPENDENCIES    Must be called within TDS_INTLOCK

RETURN VALUE    None

SIDE EFFECTS    As said above
==========================================================================*/
void tdsuldpchctrl_suspend_bfi_two_frames_before_act_time(tdsl1_ext_cmd_type* cmd);


//#endif /* FEATURE_WCDMA_SUSP_BFI_TWO_FR_BF_ACT */

/*===========================================================================
FUNCTION        tdsuldpchctrl_calc_tfc_slot_occupation_info

DESCRIPTION     This function is used to 
                1. calculate the slot occupation info after RM processing

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
void tdsuldpchctrl_calc_tfc_format_info(void);

/*===========================================================================
FUNCTION        tdsuldpchctrl_set_uldpch_ts_bitmap

DESCRIPTION     This function is used to 
                1. Calculate the ULDPCH TS bitmap
								| X | X | X | X | X | X | X | X |
								 ts6 ts5 ts4 ts3 ts2 ts1 spt ts0		
DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
void tdsuldpchctrl_set_uldpch_ts_bitmap(void);


#ifdef FEATURE_TDSCDMA_DSDA
/*=========================================================================
FUNCTION tdsuldpch_get_cfg_timeslot_bitmask

DESCRIPTION
  L1M/DSDA use this interface to query ul timeslot configuration

DEPENDENCIES
  None.

RETURN VALUE
  DPCH time slot bitmap.

SIDE EFFECTS
  None.
=========================================================================*/
uint16 tdsuldpchctrl_get_uldpch_timeslot_bitmask(void);

#endif /*FEATURE_TDSCDMA_DSDA*/


/*===========================================================================
FUNCTION    tdsl1_alloc_dynamic_mem

DESCRIPTION
          when receive TDSL1_CPHY_START_TDSCDMA_MODE_REQ, L1M start to malloc memory for L1 modules, and initialize them.
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsuldpchctrl_alloc_dynamic_mem(void);

/*===========================================================================
 
FUNCTION    tdsl1dec_free_dynamic_mem

DESCRIPTION
          when receive TDSL1_CPHY_STOP_TDSCDMA_MODE_REQ, L1M start to free memory for L1 modules.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsuldpchctrl_free_dynamic_mem(void);

#ifdef FEATURE_QSH_EVENT_METRIC

boolean tdsl1ulcfg_get_qsh_metric(tdsl1_qsh_metric_ul_dpch_tx_pwr_s* qsh_metric_ul_dpch_tx_pwr_ptr);

#endif /*FEATURE_QSH_EVENT_METRIC*/


#endif/* TDSULDPCHCTRL_H */
