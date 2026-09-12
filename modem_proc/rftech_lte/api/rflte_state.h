/*!
  @file rflte_state.h

  @brief
  This file contains all declarations necessary to use the LTE State Machine.

  @details

*/

/*===========================================================================

  Copyright (c) 2013 - 2021 Qualcomm Technologies Incorporated. All Rights Reserved

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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_lte.mpss/3.11/api/rflte_state.h#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/20/22   rk      [CR3361508]Fix No HDET issue due to GRFC control
03/16/21   cv      [CR2827428]Fix FTL SNR with TX PLL and RX PLL timing 
07/29/20   ssl     FR65168: Turn OFF WTR RX PLL in RF RX wake up
07/29/20   ssl     FR65168: Turn OFF WTR Tx PLL in TX wake up
03/30/20   ssl     [CR2634739]rflte_state_set_ant_position input argument update
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
11/08/19   ssl     FR 56310: Skip the preparation of RX Sleep scripts if configuration is not changed
10/18/19   ssl     [CR2548513]Removal of unused legacy API'S support
03/07/18   ks      QAT char offset update
08/16/16   ndb     Added band_to_carrier_idx_map in rflte state machine
05/02/16   sg      CDRx Rx/Tx wakeup/sleep optimization
04/27/16   vc      Avoiding script building in sleep/wakeup on same config
04/06/16   rp      Code changes to return proper EARFCN type from state machine.
12/02/15   sml     Phase 2 device status read
11/26/15    vc     RF LTE timiline optimization
11/25/15   am      asm trigger update for tdd split bands.
11/25/15   vkm     Per Carrier common lock mechanism to prevent deadlocks.
11/18/15   cv      Added support for PA biass offset w.r.t EARFCN
11/05/15    pv     Added support to clean up Tx handle based resources.
11/04/15   mah     Changes to enable handle bases TxPL SM.
11/03/15   sk      Pass UE mode as an argument.
10/27/15   vkm     Reverting FR30081
10/21/15   mah     Support to record the CA Mode in the state machine
10/06/15   sg      Make DPD status get/set APIs handle based
09/30/15   jj      WTR hopping related fbrx change
09/28/15   ska     Adding support for DL active mask bitmask for NS ULCA
09/25/15   pkg     Put new API out of unused Feature flag to avoid compilation issues.
09/25/15   pkg     Changes to avoid crash in RF Cal when previous band Cal fails.
09/25/15   sb      FR30081:Apply gain offsets for ASDIV switch position change
09/04/15    pv     Adding Dynamic TxLM Buffer index based on ML1 input params to support ULCA.
09/04/15    pv     Added support for lock for MTPL updte APIs
09/01/15   kab     Add Overheat backoff value to TXPL state machine
08/26/15   sk      [Phase 1] WTR hopping - Move TX MC SM variables to hadnle based DM
08/21/15   stw     FR 26008 : SCC1 SCC2 Non-Contiguous CA
08/20/15   sg      Add the max_backoff_dtpl needed for IMD backoff calculation part of the TxPL SM 
08/18/15   sg      Add a flag to indicate if ASDiv critical section has been initialized
08/13/15   sg      Add API to return the device used for TQ allocation for a carrier
08/11/15   sg      Fix parameters passed to rflte_state_set_ant_position()
08/07/15   sg      FR27572: Add AsDiv SAR backoff support
07/21/15   stw     Added rflte_state_set_bw_class_combo_ptr
07/20/15   pkg     Changes to support FR 24616:Algorithm to mitigate VBatt
                   droop (down to 2.5V) via TX backoff.
07/16/15   sb      Add checks to catch duplicate WB indices
06/08/15   stw     RF-RRC Interface for Bandwidth Class Combos
05/19/15   sdu     Changing the name of Alt_path API
04/20/15    sk     Re-structure Tx path specific NV's per Tx NV container
04/17/15   mg      Changes to support Tx Rx Split design part 3
04/16/15   pkg     Changes to support Tx Rx Split Band design part 2.
3/23/15    JJ      ULCA FBRx Cal support with xpt override fix
03/20/15   pkg     UL CA related changes.
03/18/15   pkg     Changes to support Script based Tx Tune.
03/12/15   mg      Fix to track the tdd_mode flag per carrier
02/04/15   jf      64QAM initial support
12/15/14   ars     Added fix to handle intra CA with split bands where PRx and DRx are on different splits.
11/14/14    pv     HORxD Feature Bring up changes.
11/12/14   bsh     Cleanup TQ assignment for all cells
11/04/14   gvn     Re-tune Tx and Rx in case split band changes on intra scell add or delete
09/12/14    pv     Path to Carrier type conversion. 
09/11/14   kab     Separate placeholders for PRx,Drx Bands in LTE State Machine
08/27/14   svi     Move Alternate Path State Variable to Path Specific
08/21/14   svi     Added Support for Rx Config Script Creation
08/18/14   kab     TxPL changes to support SGLTE Tx power backoff with Coex.
08/15/14   st      CA Alt Path Reconfiguration Support
08/04/14    pv     Added support for PA swp adjust mechanism.
08/01/14   gvn     Update state machine with correct pcell band in case of split intraband case
07/31/14   ndb     Added support for extended EARFCN for LTE
07/30/14   svi     Notch Application based on Tx AFC - Phase 2
07/08/14   gvn     Modify algorithm for split-intraband support 
06/20/14    pv     Added support for TDD PortA/B swap for ASM.
06/18/14   sbm     Added state machine support to determine tx pwr ctrl mode.
06/13/14   svi     Notch Application based on Tx AFC
06/04/14    pv     Added support for FBRx RB Based Freq comp.
05/07/14   svi     Tx Config Timeline optimization
04/20/14   gvn     S-LTE Support
04/17/14    pv     Added support for passing threshold for power tracking method.
04/17/14   svi     PA static NV and code redesign
04/11/14   bsh     Split Band Support
03/27/14   svi     [ASDIV] Added API to get l2l irat state for last irat channel
03/11/14   bsh     Adding NLIC support - add rx freq error in sm
01/28/14   jf      Append RGI_0 script to TX_ON off script, and trigger HW write 
                   RGI_0 at tx_config
02/21/14   cri     Fix null access crash during freq comp
02/20/14   cri     Update QRCT log packet for Bolt to work with FED
02/18/14    pv     Added support for FBRx Gain error processing with Common FBRx Trigger.
02/18/14   cri     Only allocate freq comp during tx tune
02/04/14    pv     Added support for FBRx Gain error processing.
01/08/13   npi     Track current state of Intra-band CA
11/26/13   bsh     Change the get rxlm API name to match other APIs + New APIs for txlm 
11/12/13   bsh     Delete the states related to Rx Dm 
10/26/13   bsh     support for rx fed
09/05/13   bsh     Initial check in

===========================================================================*/

#ifndef RFLTE_STATE_H
#define RFLTE_STATE_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "rfcom.h"
#include "msgr_types.h"
#include "lm_types.h"
#include "rfc_common.h"
#include "rflte_msg.h"
#include "rf_hal_common.h"
#include "rfcommon_atuner_intf.h"
#include "rflte_mc.h"
#include "rflte_nv.h"
#include "rfdevice_lte_type_defs.h"
#include "rfc_msm_signal_info_ag.h"
#include "rflte_msg.h"
#include "rflm_lte.h"
#include "rflm_txagc_api.h"
#include "rflte_core_freq_comp.h"
#include "rflte_ext_mc.h"
#ifdef FEATURE_RF_ASDIV
  #include "rfcommon_asdiv_manager.h"
#endif
#include "rfcommon_locks.h"

/* This is needed to implement timer for our HDET/FBRx Read */
#include "timer.h"
#include "ftm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Structure Definitions */
/*--------------------------------------------------------------------------*/

typedef struct
{
  /* State of TXPL module */
  rflte_core_txpl_mode_type mode;
  uint32                    counter;
  rf_lock_data_type lock;
  boolean txpl_lock_init_done;

  rflte_core_txpl_pwr_trking_type pwr_corr_mode;

  /* ACQ mode filtering configuration */
  uint16   init_mode_alpha;
  uint16   init_mode_period_ms;
  uint16   init_mode_count;
  uint16   init_mode_exit_wait;

  /* ACQ mode filtering configuration */
  uint16   acq_mode_alpha;
  uint16   acq_mode_period_ms;
  uint16   acq_mode_count;
  uint16   acq_mode_exit_wait;
  int16    acq_mode_resolution;

  /* TRK mode filtering configuration */
  uint16   trk_mode_alpha;
  uint16   trk_mode_period_ms;
  uint16   trk_mode_count;
  uint16   trk_mode_exit_wait;
  int16    trk_mode_resolution;

  /* TX power limits */
  uint16 txpl_sar;
  uint16 txpl_sar_default_pos;
  uint16 txpl_sar_swap_pos;
  uint16 txpl_nv;
  uint16 txpl_overheat;
  uint16 txpl_pcmax;
  uint16 txpl_pemax;
  int16 txpl_dtpl;
  int16 txpl_sgsar;
  int16 txpl_coex;
  int16 txpl_coex_wlan_limit;
  int16 txpl_subframe_coex;
  uint16 txpl_overheat_backoff;  
  int16 vbatt_backoff;

  /*TDD Config backoff for SAR limit*/
  uint16 tdd_cfg_backoff;

  /* SRS TX power limits */
  uint16 txpl_srs_pcmax;
  uint16 txpl_srs_pemax;
  int16 txpl_srs_dtpl;
  int16 txpl_srs_coex_wlan_limit;

  /* HDET read error */
  uint16 hdet_raw_read;
  int16  hdet_agc;
  int16 hdet_txagc_inst_err;
  float hdet_txagc_filtered_err;

  /* FBRx Gain error */
  int16 fbrx_gain_current_err;
  int16 fbrx_gain_err_inst_vs_txagc_val;
  float fbrx_gain_filtered_err;
  int16 fbrx_threshhold;

  /* HDET read configurations */
  boolean lpm_enabled;
  uint16 pdet_switch_point;
  uint16 hdet_threshhold;

  /* output of MTPL/HDET loop */
  int16 mtpl;
  int16 srs_mtpl;

  /* logging */
  uint8 mpr;
  uint8 ampr;
  uint8 ns_flag;
  uint8 rb_block_start_1;
  uint8 rb_block_start_2;
  uint8 rb_block_num;
  rflm_lte_mod_type_s mod;

  uint8 srs_mpr;
  uint8 srs_ampr;
  uint8 srs_ns_flag;
  uint8 srs_rb_block_start;
  uint8 srs_rb_block_num;

  /* DAC setpoint override */
  boolean dac_setpoint_override;
  uint16  dac_setpoint;

  /* timer params  */
  /* This is needed for maintaining time intervals
     based on our power tracking timer.
     Power tracking timer handle. Can be used to trigger
     FBRx/HDET/TDET measurement */
  boolean timer_created;
  timer_type pwr_trking_read_timer;

  /* The loop timer for repeating the pwr trking algorithm */
  uint32 loop_period;

  /* Carrier Index, this Structure belongs to */
  uint32 curr_carrier_idx;

  /* Freq comp values for both carrier indices */
  rflte_core_freq_comp_type freq_comp_offsets;

  /* FBRx Freq comp values for both carrier indices */
  rflte_core_fbrx_freq_comp_type fbrx_freq_comp_offsets;

  boolean tx_tune_update; 

  /* This is used to store the PCELL and SCELL DTPL value returned
  from rflte_core_txpl_calc_target_power() and used in computation
  of IMD backoff values in SCELL inter-band cases */
  int16 max_backoff_dtpl;

} rflte_core_txpl_sm_type;

typedef struct 
{

  /* Mapping of first sweep RGIs to qsorted linearizer */
  uint8  first_sweep_rgis[RF_LTE_MAX_NUM_PA_STATES][RFCOM_TXLIN_NUM_SEGMENTS];
  uint8  cal_rgis[RF_LTE_MAX_NUM_PA_STATES][RFCOM_TXLIN_NUM_SEGMENTS];
  uint16 pa_bias[RF_LTE_MAX_NUM_PA_STATES][RFCOM_TXLIN_NUM_SEGMENTS];
} rflte_state_lin_mapping_type;

/* Timer call-back Funtion */
void rflte_core_pwr_trking_timer_read_cb(timer_cb_data_type data);

/*----------------------------------------------------------------------------*/
void rflte_mc_update_single_rx_ca_state
(
  uint8 sub_id,
  uint32 parent_cell_index, 
  uint32 umid
);

#if 0
/*----------------------------------------------------------------------------*/
void rflte_mc_reset_single_rx_ca_mc_state
(
  uint8 sub_id,
  uint32 cell_index
);

/*----------------------------------------------------------------------------*/
void rflte_mc_update_single_tx_ca_state
(
  uint8 sub_id,
  uint32 parent_cell_index, 
  uint32 umid
);
/*----------------------------------------------------------------------------*/
void rflte_mc_single_tx_ca_tx_disable_reset_mc_state
(
  uint8 sub_id,
  uint32 parent_cell_index
);
#endif

/*----------------------------------------------------------------------------*/
extern void rflte_get_alt_path_from_current_device_table
(
  uint8 sub_id,
  rfm_device_enum_type device,
  uint8* alt_path 
);

/*----------------------------------------------------------------------------*/
uint32 rflte_mc_get_next_cell_single_tx_ca
(
  uint8 sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
#if 0
uint32 rflte_mc_get_parent_cell_single_tx_ca
(
  uint8 sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
uint32 rflte_mc_get_single_tx_ca_num_tx_cells
(
  uint8 sub_id,
  uint32 carrier_index
);
#endif
/*----------------------------------------------------------------------------*/
uint32 rflte_mc_get_next_cell_single_rx_ca
(
  uint8 sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
boolean rflte_mc_is_rx_cell_member_single_rx_ca
(
  uint8 sub_id,
  uint32 cell_index
);
/*----------------------------------------------------------------------------*/
boolean is_single_rx_parent_cell
(
  uint8 sub_id,
  uint32 cell_index
);

/*----------------------------------------------------------------------------*/
void rflte_mc_single_rx_ca_exit_reset_mc_state
(
  uint8 sub_id,
  uint32 parent_cell_index
);

/*----------------------------------------------------------------------------*/
#if 0
void rflte_mc_get_single_rx_ca_state
( 
   uint8 sub_id,
   dl_ca_single_rx_type *ul_ca_single_rx_mc_state_ptr,
   uint32 cell_index
);
/*----------------------------------------------------------------------------*/
uint32 rflte_mc_get_single_rx_ca_num_rx_cells
(
  uint8 sub_id,
  uint32 carrier_index
);
/*----------------------------------------------------------------------------*/
uint32 rflte_mc_get_parent_cell_single_rx_ca
(
  uint8 sub_id,
  uint32 cell_index
);
#endif


/*----------------------------------------------------------------------------*/
rflte_mc_status_type rflte_mc_validate_state_input(uint8 sub_id,uint32 carrier_idx, uint32 umid, void* data);

/*----------------------------------------------------------------------------*/
void rflte_mc_update_state(uint8 sub_id,uint32 carrier_idx, uint32 umid, void *payload_ptr);

/*----------------------------------------------------------------------------*/
rf_lte_state_enum_type rflte_mc_get_state(uint8 sub_id,uint32 carrier_idx);


/*----------------------------------------------------------------------------*/

rfcom_lte_band_type rflte_state_get_curr_band_tx( uint8 carr_index, lm_handle_type lm_buffer_index );
void rflte_state_set_curr_band_tx( uint8 carr_index, rfcom_lte_band_type band_tx, lm_handle_type lm_buffer_index);

/*----------------------------------------------------------------------------*/

rf_lte_power_control_mode_type rflte_state_get_power_ctrl_mode(uint32 carrier_idx,lm_handle_type lm_buffer_index);
void rflte_state_set_power_ctrl_mode(
uint32 carrier_idx,
lm_handle_type lm_buffer_index,
rf_lte_power_control_mode_type pwr_ctrl_mode
);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_get_tuner_present(uint8 sub_id);
/*----------------------------------------------------------------------------*/
void rflte_mc_set_tuner_present
(
  uint8    sub_id,
  boolean  status
);
/*----------------------------------------------------------------------------*/
void rflte_state_set_dyn_scell_switch_on(uint8 sub_id,boolean dyn_scell_switch_on);

boolean rflte_state_get_dyn_scell_switch_on(uint8 sub_id);

/*----------------------------------------------------------------------------*/

void rflte_mc_get_single_tx_ca_state
(
  uint8                  sub_id,
  ul_ca_single_tx_type  *ul_ca_single_tx_mc_state_ptr,
  uint32                 cell_index
);

/*----------------------------------------------------------------------------*/
#if 0
void rflte_mc_reset_single_tx_ca_mc_state
(
 uint8 sub_id,
 uint32 cell_index
);
#endif

/*----------------------------------------------------------------------------*/
boolean rflte_mc_is_tx_cell_member_single_tx_ca
(
  uint8    sub_id,
  uint32   cell_index
);
/*----------------------------------------------------------------------------*/

rflte_nv_tbl_type* rflte_mc_get_nv_tbl_ptr(uint8 sub_id,uint32 carrier_idx, lm_handle_type lm_buffer_index);

void rflte_mc_set_nv_tbl_ptr(uint8 sub_id,uint32 carrier_idx, lm_handle_type lm_buffer_index, rflte_nv_tbl_type* nv_tbl_ptr);

/*----------------------------------------------------------------------------*/

uint32 rflte_mc_get_tx_nv_container(uint32 carrier_idx, 
                                    lm_handle_type lm_buffer_index);

void rflte_mc_set_tx_nv_container(uint32 carrier_idx, 
                                  lm_handle_type lm_buffer_index, 
                                  uint32 tx_nv_container);

/*----------------------------------------------------------------------------*/

rflte_core_txpl_ns_type rflte_mc_get_active_ns_flag_based_hw_ctl(uint8 sub_id,uint32 carrier_idx);

void rflte_mc_set_active_ns_flag_based_hw_ctl(uint8   sub_id,
                                              uint32  carrier_idx,
                                              rflte_core_txpl_ns_type ns_x);

/*----------------------------------------------------------------------------*/

rfdevice_lte_script_data_type* rflte_state_get_script_data_ptr(uint8    sub_id,
                                                               uint32   carrier_idx, 
                                                               rflte_mc_carrier_type carrier_type);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_get_init_status(uint8 sub_id);
/*---------------------------------------------------------------------------*/
void rflte_mc_set_init_status(uint8 sub_id,boolean flag);
/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_state_get_curr_band_rx(uint8 sub_id,uint8 cell_idx, rflte_mc_carrier_type carrier_type);
/*----------------------------------------------------------------------------*/
void rflte_state_set_curr_band_rx(uint8 sub_id,
                                  uint8 cell_idx, 
                                  rfcom_lte_band_type rx_band, 
                                  rflte_mc_carrier_type carrier_type);
/*----------------------------------------------------------------------------*/
void rflte_state_init(uint8 sub_id,boolean nv_init_done, ftm_mode_type curr_op_mode);

uint32 rflte_state_get_carrier_idx_from_band (uint8 sub_id,rfcom_lte_band_type band);

/*----------------------------------------------------------------------------*/

void rflte_state_set_carrier_idx_for_band (uint8 sub_id,uint32 carrier_idx, rfcom_lte_band_type band);

/*----------------------------------------------------------------------------*/
void rflte_state_tx_init(lm_handle_type txlm_handle);
/*----------------------------------------------------------------------------*/
void rflte_state_reset_path_state(uint8 sub_id,uint32 cell_index);
/*----------------------------------------------------------------------------*/
void rflte_state_reset_l2l_irat_state(uint8 sub_id);
/*----------------------------------------------------------------------------*/
rfcom_device_enum_type rflte_state_get_prx_rfm_device(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_prx_rfm_device(uint8   sub_id,
                                    uint32  carrier_index, 
                                    rfcom_device_enum_type rfm_device);
/*----------------------------------------------------------------------------*/
rfcom_device_enum_type rflte_state_get_drx_rfm_device(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_drx_rfm_device(uint8   sub_id,
                                    uint32  carrier_index, 
                                    rfcom_device_enum_type rfm_device,
                                    rf_lte_dev_type dev_type);
/*----------------------------------------------------------------------------*/
rfcom_device_enum_type rflte_state_get_drx2_rfm_device(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
rfcom_device_enum_type rflte_state_get_drx3_rfm_device(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
rfcom_device_enum_type rflte_state_get_tx_rfm_device(uint32 carrier_index, 
                                                     lm_handle_type lm_buffer_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_tx_rfm_device(uint32 carrier_index, 
                                   lm_handle_type lm_buffer_index, 
                                   rfcom_device_enum_type rfm_device);

void rflte_state_set_tx_carrier (uint32 carrier_idx,
                                 lm_handle_type lm_buffer_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_horxd_flag(uint8 sub_id,uint32 carrier_index, uint8 path_bitmask);
/*----------------------------------------------------------------------------*/
uint8 rflte_state_get_horxd_flag(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/

rfcom_lte_earfcn_type rflte_state_get_curr_chan_rx(uint8 sub_id,uint32 cell_idx);

void rflte_state_set_curr_chan_rx(uint8 sub_id,uint32 cell_index, rfcom_lte_earfcn_type rx_chan);

rfcom_lte_earfcn_type rflte_state_get_curr_chan_tx(uint32 cell_idx, lm_handle_type lm_buffer_index);

void rflte_state_set_curr_chan_tx(uint32 cell_index, lm_handle_type lm_buffer_index, rfcom_lte_earfcn_type tx_chan);

/*----------------------------------------------------------------------------*/
void rflte_state_set_nv_status(boolean status);

/*----------------------------------------------------------------------------*/
boolean rflte_state_get_nv_status(uint8 sub_id);


/*----------------------------------------------------------------------------*/

void rflte_state_set_ftm_ns_mode(uint8 sub_id,boolean flag);

boolean rflte_state_get_ftm_ns_mode(uint8 sub_id);
/*----------------------------------------------------------------------------*/

void rflte_state_set_ftm_mode(uint8 sub_id,boolean flag);
boolean rflte_state_get_ftm_mode(uint8 sub_id);

#if 0
/*----------------------------------------------------------------------------*/
void rflte_state_set_ftm_ns_mode(boolean flag);  // Already Decalred above
boolean rflte_state_get_ftm_ns_mode(void);
/*----------------------------------------------------------------------------*/
#endif

boolean rflte_state_get_xpt_status(uint32 carrier_idx,lm_handle_type lm_buffer_index );
void rflte_state_set_xpt_status(uint32 carrier_idx, lm_handle_type lm_buffer_index, boolean xpt_flag);

/*----------------------------------------------------------------------------*/
boolean rflte_state_get_dpd_status(uint32 carrier_index, lm_handle_type lm_buffer_index);
void rflte_state_set_dpd_status(uint32 carrier_index, lm_handle_type lm_buffer_index, boolean dpd_in_use);

/*----------------------------------------------------------------------------*/
int32 rflte_state_get_rfc_msm_timing(uint8 sub_id,rfc_timing_param_type timing_index);

/*----------------------------------------------------------------------------*/
void rflte_state_set_tdd_mode(uint8 sub_id,boolean tdd_mode, uint32 carrier_idx);
boolean rflte_state_get_tdd_mode(uint8 sub_id,uint32 carrier_idx);
/*----------------------------------------------------------------------------*/

void rflte_state_set_gnss_blanking_status(uint32 carrier_index, lm_handle_type lm_buffer_index, boolean gnss_status);
boolean rflte_state_get_gnss_blanking_status(uint32 carrier_index, lm_handle_type lm_buffer_index);
/*----------------------------------------------------------------------------*/
boolean rflte_state_get_lin_vs_temp_vs_freq_status(uint32 carrier_idx,lm_handle_type lm_buffer_index );
void rflte_state_set_lin_vs_temp_vs_freq_status(uint32 carrier_idx, lm_handle_type lm_buffer_index , boolean status);

/*----------------------------------------------------------------------------*/
rfcom_lte_bw_type rflte_state_get_bw_tx(uint32 carrier_index, lm_handle_type lm_buffer_index );
void rflte_state_set_bw_tx(uint32 carrier_index, lm_handle_type lm_buffer_index, rfcom_lte_bw_type bw_tx);
rfcom_lte_bw_type rflte_state_get_bw_rx(uint8 sub_id,uint32 carrier_index);
void rflte_state_set_bw_rx(uint8 sub_id,uint32 carrier_index, rfcom_lte_bw_type bw_rx);
/*----------------------------------------------------------------------------*/
void rflte_state_set_intraband_ca_mask(uint8 sub_id,uint32 carrier_index, uint32 mask);
uint32 rflte_state_get_intraband_ca_mask(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/

void rflte_state_set_ca_mode(uint8 sub_id,uint32 carrier_index, rfcom_lte_ca_mode_type mode);
rfcom_lte_ca_mode_type rflte_state_get_ca_mode(uint8 sub_id,uint32 carrier_index);

/*----------------------------------------------------------------------------*/
#if 0
void rflte_mc_single_tx_ca_mc_reset_state(uint8 sub_id,uint32 cell_index);

/*----------------------------------------------------------------------------*/
void rflte_mc_update_state(uint32 carrier_idx, uint32 umid, void *payload_ptr);
//This Api already declared above
#endif

/*----------------------------------------------------------------------------*/
rf_lte_state_enum_type rflte_state_get_rf_state(uint8 sub_id,uint32 carrier_index);
void rflte_state_set_rf_state(uint8 sub_id,uint32 carrier_index, rf_lte_state_enum_type rf_state);

/*----------------------------------------------------------------------------*/
void rflte_state_single_rx_ca_reset_mc_state(uint8 sub_id,uint32 cell_index);
/*----------------------------------------------------------------------------*/
void 
rflte_state_ul_ca_reset_mc_state
(
  uint8   sub_id,
  uint32  cell_index
);

/*----------------------------------------------------------------------------*/
boolean rflte_mc_check_intra_band_ca
(
   uint8                        sub_id,
   rfcom_lte_band_type         *cell0_band, 
   rfcom_lte_earfcn_type        cell0_rx_channel,
   rfcom_lte_bw_type            cell0_rx_bw,
   rflte_mc_carrier_info_type  *cell0_carr_info,
   rflte_mc_carrier_info_type  *cell1_carr_info
);

/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_l2l_irat_state_last_irat_band(uint8 sub_id);

void rflte_state_set_l2l_irat_state_last_irat_band(uint8 sub_id,uint32 irat_band);

/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_state_get_dl_ca_single_rx_rflte_chan_rx(uint8 sub_id,uint32 carrier_idx);

void rflte_state_set_dl_ca_single_rx_rflte_chan_rx(uint8 sub_id,uint32 carrier_idx, rfcom_lte_earfcn_type rflte_chan_rx);

/*----------------------------------------------------------------------------*/
rfcom_lte_earfcn_type rflte_state_get_ul_ca_single_tx_rflte_chan_tx(uint8 sub_id,uint32 carrier_idx);

void rflte_state_set_ul_ca_single_tx_rflte_chan_tx(uint8 sub_id,uint32 carrier_idx, rfcom_lte_earfcn_type rflte_chan_tx);

/*----------------------------------------------------------------------------*/
uint32 rflte_state_get_ul_ca_single_tx_rflte_freq_tx(uint8 sub_id,uint32 cell_index);

void rflte_state_set_ul_ca_single_tx_rflte_freq_tx(uint8 sub_id,uint32 cell_index, uint32 rflte_freq_tx);

/*----------------------------------------------------------------------------*/
uint32 rflte_state_get_ul_ca_single_tx_rflte_chan_freq_tx(uint8 sub_id,uint32 cell_index);

void rflte_state_set_ul_ca_single_tx_rflte_chan_freq_tx(uint8 sub_id,uint32 cell_index, uint32 rflte_chan_freq_tx);

/*----------------------------------------------------------------------------*/
boolean rflte_state_get_et_is_in_use_status(uint32 carrier_index, lm_handle_type lm_buffer_index);

void rflte_state_set_et_is_in_use_status(uint32 carrier_index, lm_handle_type lm_buffer_index, boolean et_flag);

/*----------------------------------------------------------------------------*/
rfcom_lte_bw_type rflte_state_get_dl_ca_single_rx_rflte_bw(uint8 sub_id,uint32 cell_index);

void rflte_state_set_dl_ca_single_rx_rflte_bw(uint8 sub_id,uint32 cell_index, rfcom_lte_bw_type rflte_bw);

/*----------------------------------------------------------------------------*/
boolean rflte_state_get_l2l_irat_state_lte_mc_in_suspended_rx_state(uint8 sub_id);

void rflte_state_set_l2l_irat_state_lte_mc_in_suspended_rx_state(uint8 sub_id,boolean irat_rx_state);

/*----------------------------------------------------------------------------*/
boolean rflte_state_get_intra_band_ca_active_status(uint8 sub_id,uint32 cell_index);

void rflte_state_set_intra_band_ca_active_status(uint8 sub_id,uint32 cell_index, boolean intra_band_ca_flag);

/*----------------------------------------------------------------------------*/
boolean rflte_state_find_intra_band_ca_active_status(uint8 sub_id,uint32 cell_index0 , uint32 cell_index1);
/*----------------------------------------------------------------------------*/
boolean rflte_state_get_intra_band_ca_active_current_status(uint8 sub_id,uint32 cell_index);

void rflte_state_set_intra_band_ca_active_current_status(uint8 sub_id,uint32 cell_index, boolean intra_band_ca_flag);

/*----------------------------------------------------------------------------*/
boolean rflte_state_get_wake_up_tq_enable_flag(uint8 sub_id);

void rflte_state_set_wake_up_tq_enable_flag(uint8 sub_id,boolean wake_up_tq_flag);

/*----------------------------------------------------------------------------*/
rfcom_lte_bw_type rflte_state_get_ul_ca_single_tx_rflte_bw(uint8 sub_id,uint32 cell_index);

void rflte_state_set_ul_ca_single_tx_rflte_bw(uint8 sub_id,uint32 cell_index, rfcom_lte_bw_type rflte_bw);

/*----------------------------------------------------------------------------*/
const int8* rflte_state_get_bb_device_filter_coeff(uint32 cell_index, lm_handle_type lm_buffer_index);

void rflte_state_set_bb_device_filter_coeff(uint32 cell_index, lm_handle_type lm_buffer_index, const int8* filter_coeff);
/*----------------------------------------------------------------------------*/
int32 rflte_state_get_ul_ca_single_tx_rx_freq_error(uint8 sub_id,uint32 cell_index);

void rflte_state_set_ul_ca_single_tx_rx_freq_error(uint8 sub_id,uint32 cell_index, int32 rx_freq_error);

/*----------------------------------------------------------------------------*/
uint32 rflte_state_get_next_rx_cell_idx_in_union(uint8 sub_id,uint32 cell_index);

void rflte_state_set_next_rx_cell_idx_in_union(uint8 sub_id,uint32 cell_index, uint32 next_cell_idx);

uint32 rflte_state_get_next_tx_cell_idx_in_union(uint8 sub_id,uint32 cell_index);

void rflte_state_set_next_tx_cell_idx_in_union(uint8 sub_id,uint32 cell_index, uint32 next_cell_idx);

/*----------------------------------------------------------------------------*/
uint32 rflte_state_get_dl_ca_single_rx_rflte_freq_rx(uint8 sub_id,uint32 cell_index);

void rflte_state_set_dl_ca_single_rx_rflte_freq_rx(uint8 sub_id,uint32 cell_index, uint32 rflte_freq_rx);

/*----------------------------------------------------------------------------*/
uint32 rflte_state_get_dl_ca_single_rx_rflte_chan_freq_rx(uint8 sub_id,uint32 cell_index);

void rflte_state_set_dl_ca_single_rx_rflte_chan_freq_rx(uint8 sub_id,uint32 cell_index, uint32 rflte_chan_freq_rx);

/*----------------------------------------------------------------------------*/
void rflte_state_set_l2l_irat_state_last_irat_channel(uint8 sub_id,uint32 channel);

/*----------------------------------------------------------------------------*/
uint32 rflte_state_get_l2l_irat_state_last_irat_channel(uint8 sub_id);

/*----------------------------------------------------------------------------*/
void rflte_state_set_rfc_msm_timing_info(uint8 sub_id,rfc_timing_info_type* timing_info_ptr);

/*----------------------------------------------------------------------------*/
boolean rflte_state_get_olta_enable_status(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/

lm_handle_type rflte_state_get_rxlm_buffer_idx(uint8 sub_id,rfcom_device_enum_type device, uint32 carrier_idx);
/*----------------------------------------------------------------------------*/

void rflte_state_set_rxlm_buffer_idx(uint8 sub_id,uint32 carrier_idx, uint32 path_idx, lm_handle_type rxlm_buffer_idx);
/*----------------------------------------------------------------------------*/

void rflte_state_set_txlm_buffer_idx(uint8 sub_id,uint32 carrier_idx, lm_handle_type txlm_buffer_idx);

boolean rflte_state_get_txlm_buffer_idx(uint8 sub_id,uint32 carrier_idx, lm_handle_type *txlm_buffer_idx);

boolean rflte_state_extract_txlm_buffer_idx(uint8   sub_id,
                                            rflte_mc_carrier_info_type  carrier_info,
                                            lm_handle_type  *txlm_buffer_idx);

boolean rflte_state_validate_txlm_buffer_idx(lm_handle_type txlm_buffer_idx);

boolean rflte_state_get_txlm_dm_buf(lm_handle_type txlm_buffer_idx, void *lte_data);


/*----------------------------------------------------------------------------*/

void rflte_state_set_gsm_concurrency_path_info(uint8 sub_id,uint32 carrier_idx, rfc_coex_filter_status_type gsm_concurrency_path);
/*----------------------------------------------------------------------------*/

rfc_coex_filter_status_type rflte_state_get_gsm_concurrency_path_info(uint8 sub_id, uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

lm_handle_type rflte_state_get_fbrxlm_buffer_idx(uint8 sub_id, uint32 carrier_idx);
/*----------------------------------------------------------------------------*/
void rflte_state_set_fbrxlm_buffer_idx(uint8 sub_id, uint32 carrier_idx, lm_handle_type fbrx_buf_idx);
/*----------------------------------------------------------------------------*/
boolean rflte_state_get_fbrx_init_done_status(uint32 carrier_index, lm_handle_type lm_buffer_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_fbrx_init_done_status(uint32 carrier_index, lm_handle_type lm_buffer_index, boolean fbrx_init_done);
/*----------------------------------------------------------------------------*/
void rflte_state_reset_lin_maps(void);
/*----------------------------------------------------------------------------*/
void rflte_state_update_first_sweep_lin_maps(uint8 pa_state, tx_linearizer_table_type *lin_data);
/*----------------------------------------------------------------------------*/
void rflte_state_update_cal_lin_maps(uint8 pa_state, tx_linearizer_table_type *lin_data, int16 bias_off);
/*----------------------------------------------------------------------------*/
uint8 rflte_state_get_first_sweep_lin_mapping(uint8 pa_state, uint8 lin_index);
/*----------------------------------------------------------------------------*/
uint8 rflte_state_get_cal_lin_mapping(uint8 pa_state, uint8 lin_index);
/*----------------------------------------------------------------------------*/
uint16 rflte_state_get_cal_bias_mapping(uint8 pa_state, uint8 lin_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_active_pa_state(uint32 carrier_idx, lm_handle_type lm_buffer_index, uint8 pa_state_index, boolean active_status);
/*----------------------------------------------------------------------------*/
boolean rflte_state_get_pa_state_active_status(uint32 carrier_idx, lm_handle_type lm_buffer_index, uint8 pa_state_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_rx_freq_error(uint32 carrier_idx, lm_handle_type lm_buffer_index, int32 rx_freq_error_in_hz);
/*----------------------------------------------------------------------------*/
int32 rflte_state_get_rx_freq_error(uint32 carrier_idx, lm_handle_type lm_buffer_index);

/*----------------------------------------------------------------------------*/
/* This API is to set the path override index for a given carrier */
void rflte_state_set_path_override_index(uint8 sub_id,uint32 carrier_idx, uint8 path_override_index, rflte_mc_carrier_type curr_carr_type);

/*----------------------------------------------------------------------------*/
/* This API  returns the current path override index used in the system */
uint8 rflte_state_get_path_override_index(uint8 sub_id,uint32 carrier_idx, rflte_mc_carrier_type curr_carr_type);

/*----------------------------------------------------------------------------*/
rf_lte_state_enum_type rflte_state_get_rx_ta_flag_status(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
boolean rflte_state_get_rx_wakeup_script_opt_flag_status(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_rx_wakeup_script_opt_flag_status(uint8 sub_id,uint32 carrier_index, boolean state);
/*----------------------------------------------------------------------------*/
boolean rflte_state_get_rx_sleep_script_opt_flag_status(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_rx_sleep_script_opt_flag_status(uint8 sub_id,uint32 carrier_index, boolean state);
/*----------------------------------------------------------------------------*/
uint32 rflte_state_get_asdiv_pos_in_scripts(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
void rflte_state_set_asdiv_pos_in_scripts(uint8 sub_id,uint32 carrier_index, uint32 state);


rf_lte_update_tx_fed_static_dm_type 
rflte_state_get_tx_fed_static_dm_update
(
  uint32                                carrier_idx,
  lm_handle_type                        lm_buffer_index 
);

/*----------------------------------------------------------------------------*/
void 
rflte_state_config_tx_fed_static_dm_update
(
  uint32                                carrier_idx,
  lm_handle_type                        lm_buffer_index,
  rf_lte_update_tx_fed_static_dm_type   config_state
);
/*----------------------------------------------------------------------------*/\
rfcom_lte_band_type rflte_state_get_original_pcell_band_before_intraband_split_scell_add(uint8 sub_id);
/*----------------------------------------------------------------------------*/
void rflte_state_set_original_pcell_band_before_intraband_split_scell_add(uint8 sub_id,rfcom_lte_band_type rx_band);
/*----------------------------------------------------------------------------*/
rfcom_lte_band_type rflte_state_get_original_pcell_drx_band_before_intraband_split_scell_add(uint8 sub_id);
/*----------------------------------------------------------------------------*/
void rflte_state_set_original_pcell_drx_band_before_intraband_split_scell_add(uint8 sub_id,rfcom_lte_band_type rx_band);
/*----------------------------------------------------------------------------*/
rflte_mc_notch_programmed_config_type rflte_state_get_prx_notch_config(uint8 sub_id,uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

void rflte_state_set_prx_notch_config(uint8 sub_id,uint32 carrier_idx, rflte_mc_notch_programmed_config_type notch_config );
/*----------------------------------------------------------------------------*/

uint64 rflte_state_get_prx_dl_lo_rx_hz(uint8 sub_id,uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

void rflte_state_set_prx_dl_lo_rx_hz(uint8 sub_id,uint32 carrier_idx, uint64 dl_lo_rx_hz );
/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_prx_num_notches(uint8 sub_id,uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

void rflte_state_set_prx_num_notches(uint8 sub_id,uint32 carrier_idx, uint32 prx_num_notches );
/*----------------------------------------------------------------------------*/

void rflte_state_increase_prx_num_notches(uint8 sub_id,uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_prx_notch_application_freq(uint8 sub_id,uint32 carrier_idx, uint32 notch_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_prx_notch_application_freq(uint8 sub_id,uint32 carrier_idx, uint32 notch_application_freq, uint32 notch_index );
/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_prx_notch_depth(uint8 sub_id,uint32 carrier_idx, uint32 notch_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_prx_notch_depth(uint8 sub_id,uint32 carrier_idx, uint32 notch_depth, uint32 notch_index );
/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_prx_spur_threshold(uint8 sub_id,uint32 carrier_idx, uint32 notch_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_prx_spur_threshold(uint8 sub_id,uint32 carrier_idx, uint32 spur_thresh, uint32 notch_index );
/*----------------------------------------------------------------------------*/

rflte_mc_notch_programmed_config_type rflte_state_get_drx_notch_config(uint8 sub_id,uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

void rflte_state_set_drx_notch_config(uint8 sub_id,uint32 carrier_idx, rflte_mc_notch_programmed_config_type notch_config );
/*----------------------------------------------------------------------------*/

uint64 rflte_state_get_drx_dl_lo_rx_hz(uint8 sub_id,uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

void rflte_state_set_drx_dl_lo_rx_hz(uint8 sub_id,uint32 carrier_idx, uint64 dl_lo_rx_hz );
/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_drx_num_notches(uint8 sub_id,uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

void rflte_state_set_drx_num_notches(uint8 sub_id,uint32 carrier_idx, uint32 drx_num_notches );
/*----------------------------------------------------------------------------*/

void rflte_state_increase_drx_num_notches(uint8 sub_id,uint32 carrier_idx );
/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_drx_notch_application_freq(uint8 sub_id,uint32 carrier_idx, uint32 notch_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_drx_notch_application_freq(uint8 sub_id,uint32 carrier_idx, uint32 notch_application_freq, uint32 notch_index );
/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_drx_notch_depth(uint8 sub_id,uint32 carrier_idx, uint32 notch_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_drx_notch_depth(uint8 sub_id,uint32 carrier_idx, uint32 notch_depth, uint32 notch_index );
/*----------------------------------------------------------------------------*/

uint32 rflte_state_get_drx_spur_threshold(uint8 sub_id,uint32 carrier_idx, uint32 notch_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_drx_spur_threshold(uint8 sub_id,uint32 carrier_idx, uint32 spur_thresh, uint32 notch_index );
/*----------------------------------------------------------------------------*/

int32 rflte_state_get_afc_rx_freq_error_in_hz (uint32 carrier_idx, lm_handle_type lm_buffer_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_afc_rx_freq_error_in_hz (uint32 carrier_idx, lm_handle_type lm_buffer_index, int32 error_in_hz );
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void rflte_state_set_alt_path(uint8 sub_id,rfcom_device_enum_type rfm_device, uint8 alt_path);
/*----------------------------------------------------------------------------*/

uint8 rflte_state_get_alt_path(uint8 sub_id,rfcom_device_enum_type rfm_device);
/*----------------------------------------------------------------------------*/

void rflte_state_set_drx_spur_type(uint8 sub_id,uint32 carrier_idx, rflte_mc_spur_mitigated_type spur_type, uint32 notch_index );
/*----------------------------------------------------------------------------*/

rflte_mc_spur_mitigated_type rflte_state_get_drx_spur_type(uint8 sub_id,uint32 carrier_idx, uint32 notch_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_prx_spur_type(uint8 sub_id,uint32 carrier_idx, rflte_mc_spur_mitigated_type spur_type, uint32 notch_index );
/*----------------------------------------------------------------------------*/

rflte_mc_spur_mitigated_type rflte_state_get_prx_spur_type(uint8 sub_id,uint32 carrier_idx, uint32 notch_index );
/*----------------------------------------------------------------------------*/

void rflte_state_set_alternate_path(uint8 sub_id,uint32 carrier_idx, rflte_mc_carrier_type carrier_type, uint8 alternate_path );
/*----------------------------------------------------------------------------*/

uint8 rflte_state_get_alternate_path(uint8 sub_id, uint32 carrier_idx, rflte_mc_carrier_type carrier_type );
/*----------------------------------------------------------------------------*/

uint16 rflte_state_get_active_dl_carrier_bitmask(uint8 sub_id);
/*----------------------------------------------------------------------------*/

void rflte_state_set_active_dl_carrier_bitmask(uint8 sub_id,uint16 active_dl_carrier_bitmask);
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get TQ flag from state structure 

  @details

  @param
  carrier_index
*/
boolean rflte_state_get_tq_allocated_flag(uint8 sub_id,uint32 carrier_idx);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set TQ flag in state structure based on whether it was allocated/dealloacted 

  @details

  @param
  carrier_index, flag
*/
void rflte_state_set_tq_allocated_flag(uint8 sub_id,uint32 carrier_idx, boolean flag);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function will set intra_band ulca flag.
 
  @param
  intra_band ulca flag

  @return
  void
*/

void rflte_state_set_intra_band_ul_ca_flag(uint8 sub_id,boolean flag);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function will get intra_band ulca flag.
 
  @param
  NONE
  
  @return
  boolean
*/

boolean rflte_state_get_intra_band_ul_ca_flag(uint8 sub_id);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Set Cal BW that tool sends to us during radio setup

  @details

  @param
  cal_tx_bw
*/
void rflte_state_set_cal_tx_bw(uint8 sub_id,rfcom_lte_bw_type cal_tx_bw);
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Cal BW that tool sends to us during radio setup

  @details

  @param
  cal_tx_bw
*/
rfcom_lte_bw_type rflte_state_get_cal_tx_bw(uint8 sub_id);

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Get Tx/Rx NV Container from mc state machine. 

  @details

  @param
  carrier_index, nv_cont, rx_tx.
*/

boolean rflte_state_set_nv_container(uint8   sub_id,
                                     uint32  carrier_idx,
                                     uint8   nv_cont,
                                     uint8   rx_tx);
/*----------------------------------------------------------------------------*/
boolean rflte_state_get_nv_container(uint8   sub_id,
                                     uint32  carrier_idx,
                                     uint8*  nv_cont,
                                     uint8   rx_tx);
/*----------------------------------------------------------------------------*/
rflte_mc_bw_class_combos_s* rflte_state_get_bw_class_combo_ptr(void);

/*----------------------------------------------------------------------------*/

uint8* rflte_state_get_dev_rxlm_chain_array( void );

/*----------------------------------------------------------------------------*/

boolean rflte_state_validate_rxlm_chain(uint8 sub_id);

/*----------------------------------------------------------------------------*/

void rflte_state_set_bw_class_combo_ptr(rflte_mc_bw_class_combos_s* bw_class_combos_ptr);

/*----------------------------------------------------------------------------*/

#ifdef FEATURE_RF_ASDIV
/*----------------------------------------------------------------------------*/
rfcommon_asdiv_position_enum_type rflte_state_get_ant_position(uint8 sub_id);
/*----------------------------------------------------------------------------*/

void rflte_state_set_ant_position(void *data, rfcommon_asdiv_position_type *ant_position );

void rflte_state_set_qat_char_ofst(uint8 sub_id);

/*----------------------------------------------------------------------------*/

void rflte_state_set_asdiv_handle(uint8 sub_id,rfcommon_asdiv_cb_handler_type asdiv_cb_handle);
/*----------------------------------------------------------------------------*/

rfcommon_asdiv_cb_handler_type rflte_state_get_asdiv_handle(uint8 sub_id);
/*----------------------------------------------------------------------------*/

void rflte_state_set_asdiv_flag_init_done(uint8 sub_id,boolean asdiv_flag_init_done);
/*----------------------------------------------------------------------------*/

boolean rflte_state_get_asdiv_flag_init_done(uint8 sub_id);
/*----------------------------------------------------------------------------*/

#endif

rfcom_device_enum_type rflte_state_get_tq_rfm_device(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/

void rflte_state_set_active_txlm_buf_idx(uint8 sub_id,uint32 carrier_index, uint8 handle_idx);
/*----------------------------------------------------------------------------*/

uint8 rflte_state_get_active_txlm_buf_idx(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/

rfm_device_enum_type rflte_mc_state_get_device(uint8 sub_id,uint32 carr_idx, rflte_path_type path);

void rflte_state_set_active_fbrx_buf_idx(uint8 sub_id,uint32 carrier_index, uint8 handle_idx);
/*----------------------------------------------------------------------------*/

uint8 rflte_state_get_active_fbrx_buf_idx(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
void rflte_state_switch_active_txlm_buffer_index(uint8 sub_id,rflte_core_txpl_switch_txlm_buf_input_param* input_param);

void rflte_state_switch_active_fbrxlm_buffer_index(uint8 sub_id,uint32 carrier_idx);

/*----------------------------------------------------------------------------*/

void rflte_state_update_tx_states_post_tune(uint8 sub_id,uint32 carr_idx_bmsk);

/*----------------------------------------------------------------------------*/
void rflte_state_set_tx_band_for_tx_device(uint8                   sub_id,
                                           rfcom_device_enum_type  tx_rfm_device, 
                                           rfcom_lte_band_type     tx_band);
/*----------------------------------------------------------------------------*/

rfcom_lte_band_type 
rflte_state_get_tx_band_from_tx_device(rfcom_device_enum_type tx_rfm_device);

/*----------------------------------------------------------------------------*/

void rflte_state_set_rflte_mc_tx_carrier_lock_init_done(uint8 sub_id,uint32 carrier_index, boolean init_done);

/*----------------------------------------------------------------------------*/

boolean rflte_state_get_rflte_mc_tx_carrier_lock_init_done(uint8 sub_id,uint32 carrier_index);

/*----------------------------------------------------------------------------*/
#ifdef FEATURE_RF_ASDIV
rfcommon_asdiv_position_type *rflte_state_get_asdiv_sw_pos_ptr(uint8 sub_id);

/*----------------------------------------------------------------------------*/
void rflte_state_set_asdiv_sw_pos(uint8 sub_id,rfcommon_asdiv_position_type sw_pos);

/*----------------------------------------------------------------------------*/
rfcommon_asdiv_position_type  rflte_state_get_asdiv_sw_pos(uint8 sub_id);

/*----------------------------------------------------------------------------*/
void rflte_state_set_asdiv_gain_update_handle(uint8 sub_id,rfcommon_asdiv_cb_handler_type asdiv_cb_handle);

/*----------------------------------------------------------------------------*/
rflte_mc_asdiv_cb_data_type * const rflte_state_get_asdiv_data_ptr(uint8 sub_id);

/*----------------------------------------------------------------------------*/
rfcommon_asdiv_cb_handler_type rflte_state_get_asdiv_gain_update_handle(uint8 sub_id);

/*----------------------------------------------------------------------------*/
rfcommon_asdiv_position_type  rflte_state_get_prev_asdiv_sw_pos(uint8 sub_id);

/*----------------------------------------------------------------------------*/
void rflte_state_update_prev_asdiv_sw_pos(uint8 sub_id,rfcommon_asdiv_position_type sw_pos);

/*----------------------------------------------------------------------------*/
#endif
/*----------------------------------------------------------------------------*/

void rflte_state_set_device_status_registers(uint8 sub_id,
                                             uint8 device, uint8 channel, uint8 slave_id, 
                                             uint16 reg, uint8 extented_type);

/*----------------------------------------------------------------------------*/
void rflte_state_set_device_status_enable(uint8 sub_id,uint8 device, boolean enable);

/*----------------------------------------------------------------------------*/
uint8 rflte_state_get_device_status_reg_length(uint8 sub_id,uint8 device);

/*----------------------------------------------------------------------------*/
boolean rflte_state_get_device_status_enable(uint8 sub_id,uint8 device);

/*----------------------------------------------------------------------------*/
uint8 rflte_state_get_device_status_slave_id(uint8 sub_id,uint8 device, uint8 reg_count);

/*----------------------------------------------------------------------------*/
uint8 rflte_state_get_device_status_channel(uint8 sub_id,uint8 device, uint8 reg_count);

/*----------------------------------------------------------------------------*/
uint16 rflte_state_get_device_status_register(uint8 sub_id,uint8 device, uint8 reg_count);

/*----------------------------------------------------------------------------*/
uint8 rflte_state_get_device_status_extended_type(uint8 sub_id,uint8 device, uint8 reg_count);

/*----------------------------------------------------------------------------*/
/* Below APIs added to read and write the Tx script optimization flags */
/*----------------------------------------------------------------------------*/
boolean rflte_state_get_tx_wakeup_script_opt_flag_status(uint8 sub_id,uint32 carrier_index);

/*----------------------------------------------------------------------------*/
void rflte_state_set_tx_wakeup_script_opt_flag_status(uint8 sub_id,uint32 carrier_index, boolean status);


boolean rflte_get_rx_sleep_status_on_sub(uint8 sub_id);

/*----------------------------------------------------------------------------*/
void rflte_set_rx_sleep_status_on_sub(uint8 sub_id, boolean status);

void rflte_set_rx_wakeup_status(uint8 sub_id,uint32 carrier_index,boolean status);

boolean rflte_get_rx_wakeup_status(uint8 sub_id,uint32 carrier_index);

/*----------------------------------------------------------------------------*/
void rflte_set_tx_wakeup_status(uint8 sub_id,uint32 carrier_index,boolean status);

boolean rflte_get_tx_wakeup_status(uint8 sub_id,uint32 carrier_index);
/*----------------------------------------------------------------------------*/
boolean rflte_state_is_tx_state(uint8 sub_id,uint32 carrier_idx);

/*----------------------------------------------------------------------------*/
uint32 rflte_state_get_tx_tune_script_bmsk(uint8 sub_id,uint32 carrier_index);

/*----------------------------------------------------------------------------*/
void rflte_state_set_tx_tune_script_bmsk(uint8 sub_id,uint32 carrier_index, uint32 *script_exec_bmsk);

/*----------------------------------------------------------------------------*/

void rflte_state_set_sleep_wakeup_flag( uint8 sub_id, boolean wakeup_flag );

void rflte_state_set_rx_enable_disable_flag( uint8 sub_id, boolean rx_enable_flag );

boolean rflte_state_get_sleep_wakeup_flag( uint8 sub_id );

boolean rflte_state_get_rx_enable_disable_flag( uint8 sub_id );

void rflte_state_set_tx_tune_flag( uint8 sub_id, boolean txtune_flag );

boolean rflte_state_get_tx_tune_flag( uint8 sub_id );


#ifdef __cplusplus
}
#endif

#endif /* RFLTE_STATE_H */

