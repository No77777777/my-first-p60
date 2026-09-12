#ifndef GL1_HW_RXDIVERSITY_H
#define GL1_HW_RXDIVERSITY_H
/*===========================================================================
                      GL1 RX DIVERSITY HEADER FILE
DESCRIPTION
   This file handles Rx Diversity feature.


EXTERNALIZED FUNCTIONS
   


INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2008-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/inc/gl1_hw_rxdiversity.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when        who     what, where, why
--------    ---     -------------------------------------------------------- 
30/08/16    tsk    CR1058305: Update to 0x5096 RxD log packet with additional details.
11/11/15   pg      CR935148 Request TRM for DRx chain only for supported bands 
11/10/15   tsk      CR937467: Relocate dynamic RxD handler to end of gstmr_frame_tick_process.
11/08/15   tsk     CR929882: Handle RxD exit processing.
12/08/15   pa      CR887572: RxD: Schedule DRX wakeup after Rx/Tx and request TRM in task context.
28/07/15   pa      CR880706: GL1 Dynamic RxD: Wait for FW exit command before enabling it.
21/07/15   jk      CR864645:Data RxD ON/OFF changes for GCF TC 14.18.3 & 14.18.1
20/07/15   pa      CR875967:Disable Data Rxd on Multimode sub if other sub is non3pp capable. 
30/06/15   ws      CR852519 - Resolve GL1 enum conversion compile warnings
02/06/15   pg      CR846507:Allow non-signaling RF devices to be controlled by FTM 
29/05/15   pa      CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD.
31/03/15   pa      CR815621:GRM restructuring for TH 2.0. 
12/03/15   pa      CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
21/01/15   pa      CR758792:RxD support for multi-sim.
12/12/14   pa      CR763859:Handling of RxD clients in GL1 TRM structures.
13/11/14   pa      CR755557:Support for granted band/rf device in Idle mode RxD.
===========================================================================*/

#include "geran_variation.h"
#ifndef CUSTOMER_H
  #include "customer.h"
#endif
#ifndef COMDEF_H
  #include "comdef.h"
#endif

#include "rfm.h"
#include "gpl1_grm_intf.h"

#ifdef FEATURE_GSM_RX_DIVERSITY

/*===========================================================================

           Structures, Enums and Hash definitions

===========================================================================*/
#define GL1_EFS_RxD   "/nv/item_files/gsm/gl1/gsm_rx_diversity"

#define GSM_RXDIV_BAND_900     (1 << RFCOM_BAND_GSM900)
#define GSM_RXDIV_BAND_1800    (1 << RFCOM_BAND_GSM1800)
#define GSM_RXDIV_BAND_1900    (1 << RFCOM_BAND_GSM1900)
#define GSM_RXDIV_BAND_850     (1 << RFCOM_BAND_GSM850)
#define GSM_RXDIV_ALL_BANDS    (GSM_RXDIV_BAND_900 | GSM_RXDIV_BAND_1800 | GSM_RXDIV_BAND_1900 | GSM_RXDIV_BAND_850)

#define GL1_RXD_AGC_PWR_LEVEL     (-70*16)

/*Keep TRM RxD Chain atleast for 104 Frames. If algorithm decides to disable 
  RxD for consecutive 104 frames then disable RxD.*/
#define   GL1_MAX_DYNAMIC_RXD_ON_FRAMES     (104)

/* Max TRM denial count in enabling CS slot RxD. Try once, if not allocated
   don't request for diversity during the call*/
#define   GL1_MAX_CS_RXD_TRM_DENIAL_COUNT   (1)

/* Try after every 26 frames if TRM denial on PS channel*/
#define   GL1_MAX_FRAMES_ENABLE_PS_RXD      (26)

#define GL1_PACKET_MEAS_PERIOD_THRESHOLD    (20)
#define GL1_PRX_STREAM_SELECTION            (70)
#define GL1_UNLOCK_RXDIV_DELAY              (4)

#define FEATURE_GSM_RX_DIVERSITY_ADDL
/* 
                           ---> GL1_GFW_PRX_ENTER  <----
                          |           |                 |
  GL1_GFW_INVALID_ENTER <-            |               GL1_GFW_WAIT_FOR_DRX_EXIT
                          |           |                 /|\
                          |          \|/                 |
                          ----> GL1_GFW_PRX_DIVRX_ENTER--
*/
typedef enum
{
  GL1_GFW_INVALID_ENTER, /* GL1 Task initialisation*/
  GL1_GFW_PRX_ENTER, /* No diversity chain.*/
  GL1_GFW_PRX_DIVRX_ENTER, /* Prx and Drx Rf wake up. FW is configured for both chain*/
  GL1_GFW_WAIT_FOR_DRX_EXIT /* Prx is awake Drx is sleep. FW is asked for Drx Exit. 
                              No Rxd enabled at busrt level */
}gl1_hw_gfw_enter_mode_type;

typedef enum
{
  GL1_RF_INVALID_SLEEP_WAKEUP_MODE,
  GL1_RF_PRX_SLEEP_DIVRX_SLEEP_MODE,
  GL1_RF_PRX_WAKEUP_DIVRX_SLEEP_MODE,
  GL1_RF_PRX_WAKEUP_DIVRX_WAKEUP_MODE
}gl1_hw_rf_sleep_wakeup_mode_type;

typedef enum
{
  GL1_RXDIV_INVALID_MULTIMODE = -1,
  GL1_RXDIV_SINGLE_MULTIMODE,
  GL1_RXDIV_DUAL_MULTIMODE,
  GL1_RXDIV_MAX_MULTIMODE,
}gl1_hw_rxdiv_sim_configuration;

typedef enum
{
  GL1_RXDIV_INVALID_DEVICE_MODE = -1,
  GL1_RXDIV_DSDS,
  GL1_RXDIV_DSDA,
  GL1_RXDIV_MAX_DEVICE_MODE,
}gl1_hw_rxdiv_device_mode;

typedef enum
{
  GL1_RXDIV_INVALID_RAT_COMBINATION = -1,
  GL1_RXDIV_XPLUSG, //X+G or G+W,
  GL1_RXDIV_GPLUSG,
  GL1_RXDIV_MAX_COMBINATION
}gl1_hw_rxdiv_rat_combination;

typedef enum
{
  GL1_RXDIV_INVALID_SIM_TYPE = -1,
  GL1_RXDIV_SIM_1,
  GL1_RXDIV_SIM_2,
  GL1_RXDIV_MAX_SIM_TYPE
}gl1_hw_rxdiv_sim_type;

typedef enum
{
  GL1_RXDIV_INVALID_SUB_STATE = -1,
  GL1_RXDIV_IDLE,
  GL1_RXDIV_DATA,
  GL1_RXDIV_VOICE,
  GL1_RXDIV_MAX_SUB_STATE
}gl1_hw_rxdiv_sub_state;

typedef enum
{
  GL1_RXDIV_L1_DEFAULT_STATE,
  GL1_RXDIV_L1_IDLE_STATE,
  GL1_RXDIV_L1_RANDOM_ACCESS_STATE,
  GL1_RXDIV_L1_DEDICATED_STATE,
  GL1_RXDIV_L1_DTM_STATE,
  GL1_RXDIV_L1_TRANSFER_STATE
}gl1_hw_rxdiv_l1_state;

typedef struct
{
  trm_band_t            band;
  rfm_device_enum_type  device;
} gl1_hw_trm_rf_devices_type;

typedef enum
{
  GL1_DYN_RXD_NULL, /*Dynamic RxD configuration is not in process*/
  GL1_DYN_RXD_TRM_REQUESTED,
  GL1_DYN_RXD_RF_WAKEUP,
  GL1_DYN_RXD_ACTIVE, /*Dynamic RxD configuration is not in process*/
  GL1_DYN_RXD_FW_EXIT
} gl1_hw_rxd_dynamic_rxd_state;

typedef struct
{
  gl1_hw_rxd_dynamic_rxd_state        gl1_dynamic_rxd_state;
  /*Counter to keep diversity chain in voice and data*/
  uint8                               gl1_dynamic_rxd_enabled_frames;
  uint8                               gl1_dynamic_rxd_trm_denial_count;

  /*Enable/Disable RxD on start of block*/
  boolean                             gl1_rxd_wait_for_block_start;
  boolean                             gl1_dynamic_rxd_abort;
  boolean                             gl1_dynamic_force_divrx_exit;
  boolean                             gl1_dynamic_rxd_tick_required;
} gl1_hw_dynamic_rxd_type;

typedef struct
{
  gl1_hw_trm_rf_devices_type          gl1_hw_trm_rf_devices_DivRx[MAX_BAND_INFO_GRM];
  gl1_hw_dynamic_rxd_type             gl1_dynamic_rxd_data;
  uint32                              gl1_gsm_rxlm_buf_idx_DivRx;
  uint16                              gl1_hw_DivRx_band_supported;

  rfm_device_enum_type                gl1_hw_trm_rf_device_DivRx;
  rfgsm_enter_mode_type               gl1_rf_enter_mode_type;
  gl1_hw_gfw_enter_mode_type          gl1_imrd_gfw_enter_mode_type;
  gl1_hw_rf_sleep_wakeup_mode_type    gl1_rf_sleep_wakeup_mode_type;

  boolean                             gl1_hw_enable_rxdiv_agc;
  boolean                             gl1_enable_rxdiversity;

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
  boolean                             gl1_enable_rxdiversity_data;
  boolean                             gl1_enable_rxdiversity_data_dynamic;
  uint32                              gl1_ps_divrx_sel_cnt;
  uint32                              gl1_total_ps_burst_cnt;
  boolean                             gl1_disable_rxdiversity_data;
  uint32                              gl1_packet_meas_period_cnt;
  uint32                              gl1_packet_meas_threshold;
#endif /* FEATURE_GSM_RX_DIVERSITY_DATA */
}gl1_hw_rxdiv_data_t;

#ifdef FEATURE_GSM_RX_DIVERSITY
  typedef PACKED struct PACKED_POST {
    uint16                HardRxQual;
    uint16                RxQualhist;
    uint16                BEP;
    uint32                xCCHTimerVal;
    boolean               IdleforceRet;
    uint8                 RxDNVValues;
  } gl1RxDLogAddnl_t;

  gl1RxDLogAddnl_t gl1RxdAddnlData[NUM_GERAN_DATA_SPACES];
#endif /* FEATURE_GSM_RX_DIVERSITY */

/*===========================================================================

           Functions declarations

===========================================================================*/
boolean gl1_hw_trm_init_DivRx(gas_id_t gas_id);

void gl1_hw_cleanup_divrx_rfdevice_info(gas_id_t gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
boolean gl1_alloc_IMRD_rxdiv_rxlm_buff_idx(gas_id_t gas_id);

void gl1_exit_idle_diversity_mode(gas_id_t gas_id);

uint32 gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id_t gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

void gl1_hw_set_rf_enter_status(rfgsm_enter_mode_type rf_type, 
                                gas_id_t                  gas_id);

rfgsm_enter_mode_type gl1_hw_get_rf_enter_status(gas_id_t gas_id);

void gl1_hw_set_gfw_enter_status(gl1_hw_gfw_enter_mode_type gfw_type,
                                 gas_id_t                   gas_id);

gl1_hw_gfw_enter_mode_type gl1_hw_get_gfw_enter_status(gas_id_t gas_id);

void gl1_hw_set_rf_sleep_wakeup_mode(gl1_hw_rf_sleep_wakeup_mode_type sleep_wakeup_type, 
                                     gas_id_t                         gas_id);

gl1_hw_rf_sleep_wakeup_mode_type gl1_hw_get_rf_sleep_wakeup_mode(gas_id_t gas_id);

void gl1_hw_set_rxdiv_wait_for_block_start(boolean  block_start,
                                           gas_id_t gas_id);

boolean gl1_hw_get_rxdiv_wait_for_block_start(gas_id_t gas_id);

gl1_hw_rxdiv_data_t *gl1_hw_get_gsm_rxdiv_data_ptr(gas_id_t gas_id);

rfm_device_enum_type gl1_hw_get_gsm_rxdiv_rf_id(gas_id_t gas_id);

uint32 gl1_hw_map_sys_band_to_gsm_rxdiv_band(trm_band_t  band,
                                             gas_id_t    gas_id);

boolean gl1_hw_gsm_trm_band_supported(trm_band_t trm_band,gas_id_t gas_id);

void gl1_hw_set_gsm_rxdiv_rf_id(rfm_device_enum_type  rf_device_id, 
                                gas_id_t              gas_id);

boolean gl1_hw_is_rxdiv_allowed(grm_divrx_request_type divrx_req, gas_id_t gas_id);

void gl1_hw_rxdiv_handle_l1_change_state(gl1_hw_rxdiv_l1_state  current_state, 
                                         gl1_hw_rxdiv_l1_state  next_state,
                                         gas_id_t               gas_id);

boolean gl1_hw_dtm_process_rxd_cfg(const mdsp_dtm_rx_params_type  *rx_params_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
                                   boolean                        imrd_channel, 
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                                   gas_id_t                       gas_id);

boolean gl1_hw_dtm_process_slot_rxd_cfg(mdsp_dtm_rx_params_type   *rx_params_ptr,
                                        sys_band_T                band,
                                        boolean                   is_block_start,
                                        gas_id_t                  gas_id);

boolean gl1_hw_dtm_process_ftm_slot_rxd_cfg(mdsp_dtm_rx_params_type *rx_params_ptr,
                                            sys_band_T              band,
                                            uint8                   tn,
                                            gl1_rxd_control_type    rxd_ctl_flag,
                                            gas_id_t                gas_id);

boolean gl1_hw_dtm_process_cs_slot_rxd_cfg(mdsp_dtm_rx_params_type  *rx_params_ptr,
                                           sys_band_T               band,
                                           uint8                    tn,
                                           gl1_rxd_control_type     rxd_ctl_flag,
                                           gas_id_t                 gas_id);

boolean gl1_hw_dtm_process_idle_slot_rxd_cfg(mdsp_dtm_rx_params_type  *rx_params_ptr,
                                            sys_band_T                band,
                                            uint8                     tn,
                                            gl1_rxd_control_type      rxd_ctl_flag,
                                            gas_id_t                  gas_id);

boolean gl1_hw_dtm_process_ps_slot_rxd_cfg(mdsp_dtm_rx_params_type  *rx_params_ptr,
                                           sys_band_T               band,
                                           uint8                    tn,
                                           gl1_rxd_control_type     rxd_ctl_flag,
                                           gas_id_t                 gas_id);

void gl1_hw_dtm_validate_cfg_cs_ps_slot_rxd(const mdsp_dtm_rx_params_type *rx_params_ptr,
                                            boolean                       is_block_start,
                                            gl1_rxd_control_type          rxd_ctl_flag,
                                            gas_id_t                      gas_id);

boolean gl1_hw_msim_rxd_allowed(gl1_hw_rxdiv_sub_state sub_state, gas_id_t gas_id);											

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
void gl1_run_ps_divrx_stream_sel_avg (boolean prx_stream_selected, gas_id_t gas_id);

boolean gl1_get_ps_divrx_stream_sel_rate (gas_id_t gas_id);

void gl1_reset_ps_divrx_sel_avg (gas_id_t gas_id);

void gl1_set_disable_data_diversity(boolean disable_rxdiversity_data,gas_id_t gas_id);

boolean gl1_get_disable_data_diversity(gas_id_t gas_id);

void gl1_packet_meas_period_cnt(boolean increment_cnt,gas_id_t gas_id);

uint32 gl1_get_packet_meas_period_cnt(gas_id_t gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

void gl1_hw_set_dynamic_rxd_state(gl1_hw_rxd_dynamic_rxd_state  state,
                                  gas_id_t                      gas_id);

gl1_hw_rxd_dynamic_rxd_state gl1_hw_get_dynamic_rxd_state(gas_id_t gas_id);

void gl1_hw_set_force_divrx_exit(boolean  force_divrx_exit,
                                 gas_id_t gas_id);

boolean gl1_hw_get_force_divrx_exit(gas_id_t gas_id);

void gl1_hw_set_dynamic_rxd_abort(boolean  abort,
                                  gas_id_t gas_id);

boolean gl1_hw_get_dynamic_rxd_abort(gas_id_t gas_id);

void gl1_hw_set_dynamic_rxd_tick_required(boolean  tick,
                                          gas_id_t gas_id);

boolean gl1_hw_get_dynamic_rxd_tick_required(gas_id_t gas_id);

void call_in_task_grm_divrx_request(grm_request_data_t *params_p);

void call_in_task_grm_divrx_release(grm_client_enum_t   divrx_client_id,
                                    gas_id_t            gas_id);

void gl1_hw_rxdiv_handle_task_grm_rane(grm_request_data_t *msg);

void gl1_msg_dynamic_rxd_tick(gas_id_t gas_id);

void gl1_msg_process_rxd_active(gas_id_t gas_id);

#endif /*FEATURE_GSM_RX_DIVERSITY*/
#endif /* GL1_HW_RXDIVERSITY_H */

/* EOF */
