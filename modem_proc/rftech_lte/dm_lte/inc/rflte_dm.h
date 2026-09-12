#ifndef RFLTE_DM_H
#define RFLTE_DM_H

/*!

@file rflte_dm.h

*/

/*===========================================================================

  Copyright (c) 2009 - 2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/rftech_lte.mpss/3.11/dm_lte/inc/rflte_dm.h#1 $

when       who     what, where, why 
--------------------------------------------------------------------------------
09/22/20   sl      [CR2782349]Fixing mismatch in TX handle
06/03/20   ssl     [CR2694495]Passing Sub_id as input argument to WTR APIs
02/15/20   ssl     allocating txagc_lin_state for each sub
01/21/20   ssl     FR 57240: Enabling Best RX instead of DRX only, in IDLE 1RX 
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
01/09/18   ssl     adding input txlm_buffer_idx to rflte_dm_set_hdet_read_flag API 
08/01/18   pvsr    Update FBRx handle based on conflict check for both carriers
12/12/16   sk      HKADC CLK ON/OFF changes for UL 64-QAM on WTR2965
07/01/16   vc      PCC 1Rx scripts rebuild for SCC addition and save in new script ID
06/22/16   sk      FTM-RF support phase 2
05/13/16   vc      Dynamic 1Rx implementation
04/29/16   sk      LTE VSWR support
03/23/16   sk      Jacala LTE FBRX tune-away support
02/09/16   mah     Alt Path handling for Tune away
01/12/16   cv       changes to turn eLNA on/off in TDD mode
01/11/15   sml     Phase 4 : Reading device status registers in RFLTE
01/07/16   am      Carry autopin support flag in fed static
12/09/15   am      FTM mode support for Autopin WTR and Reflog capture 
11/26/15   mah     Moved the MTPL/DTPL out of Tx dynamic structure to reduce any conflict in buffer switching.
11/25/15   am      asm trigger update for tdd split bands.
11/18/15   am      Fixing bug for delay parameter in 0x184C log structure
11/18/15   cv      Added support for PA biass offset w.r.t EARFCN
11/02/15   kg      PAR Autopin support
11/06/15   rp      Initialize fbrx cell data once, in pcc enter mode
11/06/15   rp      Added Txlm Handle based Temp comp algorithm
11/04/15   mah     txlm handle fix for all api inside rflte_core_computer_p_cmax_update
11/03/15   am      Add support for extracting all active PA states.
10/21/15   mah     Change in Update DAC Backoff API to get the handle ID.
10/21/15   mah     Added support for WTR DAC Set-point back-off
10/21/15   mah     WTR ICI support: pass the correct CA mode
10/21/15   am      Fix hdet issue wih CDRx timeline optimization changes
10/06/15   sg      Add API to force the PA switchpoints to APT mode
                   during TX retune
09/30/15   jj      WTR hopping related fbrx change
09/28/15   mg      FR26416: TDD HORXD LIF Changes
09/25/15   sb      FR30081:Apply gain offsets for ASDIV switch position change
09/24/15    pv     Added support for Intra-band ULCA PA swp.
09/24/15   jj      Fix max tx pwr NV with correct txlm handle
09/21/15   sk      Make AFC TxLM Handle based
09/15/15   pv      Added support to avoid SM lookup for TxLM index and use index from tune config.
09/04/15   mg      Support for GRFC tuner
09/03/15   rp      [Phase 1] WTR hopping - Handle based DM access.
09/02/15   kr      Added support for tx max pwr based EARFCN NV
08/26/15   sk      [Phase 1] WTR hopping - Move TX MC SM variables to hadnle based DM
08/20/15   stw     FR 26008 : SCC1 SCC2 Non-Contiguous CA
08/18/15   vkm     Set correct value of alternate path in RFC calls
08/13/15    sg     Remove the unused path variable
08/13/15   kab     WTR special writes support for 4FMOD performance improvement
08/06/15    pv     Intra-band ULCA LUT hook up.
08/03/15    sk     Use correct PA states to set/reset TxAGC Lin Static data
07/25/15    JJ     Added FBRx coupler/switch re-program support
07/20/15   pkg     Changes to support FR 24616:Algorithm to mitigate VBatt
                   droop (down to 2.5V) via TX backoff.
07/20/15   mg      WTR interface changes to populate tuned devices per carrier 
                   array and current Rx operation mode to pass down to device APIs
06/26/15    pv     Intra-band ULCA LUT hook up.
06/25/15   ska     Adding support for TxAGC logs 0x184C version3
04/28/15   sb      [RX/TX split] Fixes to support TX/RX cal
04/24/15   mg      FED MC layer cleanup changes
04/23/15    pv     PA switchpoint management support for Intra-band ULCA.
04/02/15   bsh     FED MC layer cleanup
03/26/15   stw     Added API for SRX CA Mask Update in DM
02/23/15   gvn     Resolve bugs in Split intraband retune design
02/13/15   stw     Expose rflte_dm_get_rflm_rx_path_from_path_index API 
02/04/14   gvn     Rx temp comp support
01/23/15   kab     RSB NV support
01/22/15   yzw     Added carrier idx as parameter into FTM TxAGC related APIS
01/13/15   stw     Intraband CA PCC + SCC2 support
12/30/14   JJ      Added an api for ULCA Tx rotator offset
01/07/15   ska     Added fix to get default offsets for gain states
11/26/14   ndb     Added Coupler Forward/reverse Event
11/20/14   gvn     Fix alt path usage, pass alt path to apis needing it
11/19/14   svi     Do not populate the intra-band freq comp during IRAT tune-away
11/16/14   gvn     Fix Crash due to ext_rffe_buffer overflow
10/21/14   gvn     Re-tune Tx and Rx in case split band changes on intra scell add or delete
10/08/14   kab     Support to Set DM with given LO Offset from Center Freq
09/29/14   kab     Update DM with Rx LO spacing from Carrier Center Frequency
09/17/14   bsh     support rx enable/disable
09/16/14   npi     Update QFE PA's SM's on wakeup
09/11/14   vb      RSB/IQMC per LNA gain state support
08/18/14   kab     TxPL changes to support SGLTE Tx power backoff with Coex.
08/15/14    st     CA Alt Path Reconfiguration Script Creation and Event Handling
08/14/14    pv     Added support for PA swp adjust mechanism.
08/14/14    pv     Added support for FBRx + GPS concurrency based scripts.
08/07/14    pv     Added support for TDD DRx eLNA ASM programming during Rx burst.
08/06/14   ndb     Added support for extended EARFCN for LTE
08/06/14   svi     Fix not to destroy Tx Lin State machine during SCELL exit mode
08/01/14   sk      Remove Common Txagc Seq creation in non-cal mode for LTE
07/30/14   gvn     Intra-band fixes: get sca lna settings, use correct path idx in intra freq comp
07/18/14   jf      Return TX power in linear region to ML1
07/17/14   pv      Added support for call-back API for spur-mitigation based re-tune in CA mode.
07/15/14   bsh     Remove tech DM dependency for irat and rxagc
07/14/14   bsh     Remove cell deletion from Rx DM during exit
07/14/14   sbm     Added support for therm read-done flag.
07/02/14   jf      AsDiv support
06/26/14   bar     Added support for therm read.
06/25/14   cri     Reduce FTM RF log packet size
06/20/14    pv     Added support for TDD PortA/B swap for ASM.
06/18/14   sbm     Added support for WTR HDET
06/18/14   cri     Reload DPD tables after DLS wakeup
06/04/14    pv     Added support for FBRx RB Based Freq comp.
05/15/14   gvn     Add intra-band support for overlapping split bands
05/13/14   jf      Add CL/AOL support
05/07/14   svi     Tx Config Timeline optimization
05/01/14   bsh     Remove obsolete API
04/24/14    jf     Add script populate function
04/24/14   cri     wmore clipper enablement and nv support
04/23/14   bsh     Rx scripts moved from tech DM to handle pool buffer
04/17/14   svi     PA static NV and code redesign
04/11/14   npi     Clean up common device pointer usage
03/20/14   bsh     Add intra band ca lna offests/scripts and dm_rx cleanup
03/16/14    pv     Added support for new interface for common DM script updates.
03/14/14   bsh     NlIC changes: code clean up 
03/11/14   bsh     Adding NLIC support
03/03/14    pv     Added support for Trigger power level for FBRx.
02/24/14    pv     Added support for FBRx Gain error processing with MTPL.
01/28/14   jf      Append RGI_0 script to TX_ON off script, and trigger HW write 
                   RGI_0 at tx_config 
02/24/14    pv     Added support for FBRx Gain error processing with MTPL.
02/20/14   bsh     Support FTM RF Cal verification on BOLT
02/13/14    pv     Added support for FBRx Gain error processing.
02/12/14   jf      Add xpt logging for ENH TXAGC
02/06/14   bsh     Freq comp support for RxAGC
02/05/14   cri     Freq comp optimization for DM population
01/30/14   kab     AFC RF Event and AFC Gating
01/24/14   cri     DRIF/IREF/LUT enablement
01/08/13   bsh     Fix compiler warning in RFLM
12/17/13   can     Enable LTE TPL.
12/10/13   svi     RFLM FED TxAGC Logging checkin 1
12/02/13   svi     Update FED if Band is FDD or TDD
11/12/13   bsh     Use the new CMD to check cell validity in DM 
11/08/13   svi     FED DM population needs carrier_idx
11/07/13   svi     Added FED Tx DM population support
11/06/13   gvn     Remove update_tx_on_off_script, 
                   use ccs events instead for tx on off 
11/06/13   svi     Added FED Rx DM population support
10/31/13   svi     Fed Shim Layer Checkin - 2
10/26/13   bsh     support for rx fed
08/05/09   can     Initial version. 


===========================================================================*/

/*===========================================================================

                  INCLUDE FILES

===========================================================================*/
#include "rfdevice_lte_type_defs.h"
#include "rflm_lte.h"
#include "rfc_card.h"
#include "comdef.h"
#include "rflte_nv.h"
#include "comdef.h"
#include "rf_lte_mdsp_rf_event.h"
#include "rfcommon_mdsp_event_intf.h"
#include "rfcommon_msm.h"
#include "rflte_mc.h"
#include "ftm_lte_log.h"
#include "rflte_core_freq_comp.h"


#define RFLTE_DM_RX_DEFAULT_CELL_ID 0
#define RFLTE_DM_RX_DEFAULT_NUM_CELLS 2
#define RFLTE_DM_RX_INVALID_CELL_ID -1
#define RFLTE_DM_RX_INTRA_BAND_CELL_ID 1
#define RFLTE_DM_RX_ALT_PATH_CELL_ID 2

// Equivalent to 2us
#define RFLTE_DEVICE_STATUS_READ_DELAY 40
// Equivalent to 80 us
#define RFLTE_DEVICE_STATUS_READ_RX_OFF_DELAY -1536
// Equivalent to 200 us
#define RFLTE_DEVICE_STATUS_READ_NON_TRX_DELAY 3840



#ifdef FEATURE_RF_HAS_QTUNER

/*----------------------------------------------------------------------------*/

/*!
  @brief
  This module contains Tuner Related RFLTE Enums and Macros:
  @{
*/

/*----------------------------------------------------------------------------*/
/*! 
  @brief  
  Enumeration indicating whether Tuner Operation is RX only or RxTx
*/
typedef enum
{

  RF_LTE_TUNER_UPDATE_TUNE_CODE_RXTX    = 0,  /*!< Update Tune Code for RXTX */
  RF_LTE_TUNER_UPDATE_TUNE_CODE_RX_ONLY = 1,  /*!< Update Tune Code for RX Only */
  RF_LTE_TUNER_UPDATE_TUNE_CODE_INVALID,
  RF_LTE_TUNER_UPDATE_TUNE_CODE_MAX     = RF_LTE_TUNER_UPDATE_TUNE_CODE_INVALID,
  
}rf_lte_tuner_update_tune_code_rf_state_t;

/*----------------------------------------------------------------------------*/
/*! 
  @brief  
  Macro Indicating the script timing for Chain 0 RXTX Tune Code
*/
#define RF_LTE_TUNER_UPDATE_TUNE_CODE_RXTX_CHAIN_0_SCRIPT_TIMING  0

/*----------------------------------------------------------------------------*/
/*! 
  @brief  
  Macro Indicating the script timing for Chain 1 RXTX Tune Code
*/
#define RF_LTE_TUNER_UPDATE_TUNE_CODE_RXTX_CHAIN_1_SCRIPT_TIMING  0

/*----------------------------------------------------------------------------*/
/*! 
  @brief  
  Macro Indicating the script timing for Chain 0 RX Tune Code
*/
#define RF_LTE_TUNER_UPDATE_TUNE_CODE_RX_CHAIN_0_SCRIPT_TIMING    0

/*----------------------------------------------------------------------------*/
/*! 
  @brief  
  Macro Indicating the script timing for Chain 1 RX Tune Code
*/
#define RF_LTE_TUNER_UPDATE_TUNE_CODE_RX_CHAIN_1_SCRIPT_TIMING    0

/*----------------------------------------------------------------------------*/

/*! @} */

#endif /* FEATURE_RF_HAS_QTUNER */


typedef struct
{
  /* Script Index */
  uint8 script_index;

  /* FBRx Gain State */
  uint8 fbrx_gain_state;

}rflte_dm_write_event_buffer_intf_type_s;

typedef struct
{
    boolean buffer_allocated;
    uint32*  buff_ptr;
} rflte_dm_rx_buff_alloc_info;

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This structure has pointers to store the Tx AGC Lin Data populated to
  the DM
  
*/
typedef struct
{

  /*!> Tx Lin Data for all sweeps for each PA state */
  tx_linearizer_table_data_type* rflte_tx_lin_data[RF_LTE_MAX_NUM_PA_STATES];

  /*!> Tx Lin Data for first sweep for each PA state */  
  tx_linearizer_table_data_type* rflte_tx_lin_data_first_sweep[RF_LTE_MAX_NUM_PA_STATES];

  /*!> Lin Table Per PA state */
  tx_linearizer_table_type *tx_table[RF_LTE_MAX_NUM_PA_STATES];

  /*!> Lin Table for first sweep Per PA state */
  tx_linearizer_table_type *tx_table_fs[RF_LTE_MAX_NUM_PA_STATES];

  /*!> DPD Scaling Per PA state */  
  xpt_dpd_scaling_value_type dpd_scale_factor[RF_LTE_MAX_NUM_PA_STATES];

} rflte_txagc_lin_state_type;

extern rflte_txagc_lin_state_type txagc_lin_state[2];

int32 rflte_dm_pass_gnss_grfc
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type* script_data_ptr,
  rfc_logic_type tx_on_off
);
							   
boolean rf_lte_dm_rf_event_start_push_commit_to_memory
(
  uint32 event_id, 
  uint8 num_scripts, 
  const rf_hal_bus_resource_script_type **script_ptr_list, 
  void * shared_mem_buf_ptr, 
  uint32 shared_mem_max_size_bytes, 
  boolean is_issue_seq,
  uint8 max_tasks_allocated,
  boolean use_timer_conversion,
  int16 time_offset,
  uint8 event_priority
);		

void rflte_dm_write_event_buffer
(
  rflte_mc_carrier_info_type *carrier_info,
  const rf_hal_bus_resource_script_type *resource_script,  
  rflm_lte_rf_script_id_type_e script_id, 
  rflte_dm_write_event_buffer_intf_type_s intf_param
);

void rflte_dm_port_switch_transition_detected(void);

void rflte_dm_init_tx 
(
  uint32 carrier_idx
);

void rflte_dm_init_fbrx_cell_data();

void rflte_dm_reset_event_buffer_index
(
  rflte_mc_carrier_info_type carrier_info
);

void rflte_dm_tx_init_static_config
(
  uint8  sub_id,
  uint32 carrier_idx, 
  rflte_nv_tbl_type * rflte_nv_tbl_ptr,
  lm_handle_type lm_buffer_index,
  rfcom_lte_earfcn_type tx_channel,
  boolean intra_band_ulca_active
);

boolean rflte_dm_tx_pa_swp_adjust(uint8 sub_id,uint32 carrier_idx,rflte_nv_tbl_type *rflte_nv_tbl_ptr);

boolean rflte_dm_tx_pa_swp_restore(uint32 carrier_idx,rflte_nv_tbl_type *rflte_nv_tbl_ptr,
                                   boolean intra_band_ulca_flag_is_enabled,lm_handle_type lm_buffer_index );

void rflte_dm_txagc_populate_txlin_data
(
  uint8  sub_id,
  uint32 carrier_idx,
  rfcom_device_enum_type device,
  rfcom_lte_band_type band,
  rfcom_lte_bw_type bw,
  uint32 tx_freq,
  rflte_nv_tbl_type *rflte_nv_tbl_ptr,
  rfcom_lte_bw_type cal_bw,
  lm_handle_type lm_buffer_index
);

void
rflte_dm_txagc_repopulate_dpd_data
(
   uint8  sub_id,
   uint32 carrier_idx,
   rfcom_device_enum_type device,
   rflte_nv_tbl_type *rflte_nv_tbl_ptr
);

void rflte_dm_adjust_iq_scaling_based_on_bw
(
  uint8  sub_id,
  rflte_mc_carrier_info_type *carrier_info, 
  rfcom_lte_bw_type bw,
  uint32 carrier_idx
);

void rflte_dm_update_default_baseband_gains
(
   rflte_mc_carrier_info_type carrier_info, 
   rflte_nv_tbl_type *rflte_nv_tbl_ptr,
   uint32 default_iq_gain
);

void rflte_dm_update_default_clipper_settings
(
   rflte_mc_carrier_info_type carrier_info,  
   rflte_nv_tbl_type *rflte_nv_tbl_ptr
);

void rflte_dm_update_xpt_mode
(
  lm_handle_type txlm_handle,                             
  tx_linearizer_aux_data_type aux_lin_data,
  uint8 pa_state_index,
  boolean intra_band_ulca_active
);

void rflte_dm_update_tx_on_wr_on_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type* script_data_ptr,
  boolean gnss_blanking_enabled
);

void rflte_dm_update_tx_en_wr_on_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type *script_data_ptr
);

void rflte_dm_update_tx_on_wr_off_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type* script_data_ptr,
  boolean gnss_blanking_enabled
);

void rflte_dm_update_tx_en_wr_off_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type *script_data_ptr,
  rfcom_lte_bw_type bw_for_lut_look_up
);

void rflte_dm_update_tdd_asm_tx_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type* script_data_ptr
);

void rflte_dm_update_tdd_asm_tx_script
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type* script_data_ptr,
  uint8 script_index
);

void rflte_dm_update_ant_swap_script
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type *script_data_ptr, 
  boolean ant_swap_en
);

void rflte_dm_update_tdd_asm_rx_script
(
  rfm_device_enum_type prx_device,
  rfm_device_enum_type drx_device,
  rfcom_lte_band_type prx_band,
  rfcom_lte_band_type drx_band,
  rflte_mc_carrier_info_type carrier_info,
  rfdevice_lte_script_data_type *script_data_ptr,
  uint8 script_index
);

uint32 rflte_dm_rx_config_tdd_asm_rx_tx_scripts(uint8 sub_id,rfcom_lte_band_type prx_band, rfcom_lte_band_type drx_band, uint8 rx_config_mask);

void rflte_dm_build_tdd_asm_rx_tx_script(uint8 sub_id,rfcom_lte_band_type prx_band, rfcom_lte_band_type drx_band,
                                         uint8 tx_active_carrier, lm_handle_type txlm_handle_idx);

void rflte_dm_update_rf_events_timing
(
  uint8  sub_id,
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band
);

void rflte_dm_update_pa_script
(
  rflte_mc_carrier_info_type *carrier_info,
  rfm_device_enum_type device,
  rfcom_lte_band_type band,
  rfcom_lte_bw_type bw,
  rflte_nv_tbl_type *rflte_nv_tbl_ptr,
  rfdevice_lte_script_data_type *script_data_ptr,
  boolean update_scripts_to_dm,
  uint8 port_index
);

void rflte_dm_update_papm_on_event
(
    uint32 carrier_idx,
    rfm_device_enum_type rfm_device,
    rfcom_lte_band_type band,
    rfcom_lte_bw_type bw,
    rfdevice_lte_script_data_type* script_data_ptr,
    lm_handle_type lm_buffer_index 
);

void rflte_dm_update_papm_standby_event
(
    uint32 carrier_idx,
    rfm_device_enum_type rfm_device,
    rfcom_lte_band_type band,
    rfdevice_lte_script_data_type* script_data_ptr,
    lm_handle_type lm_buffer_index 
);

void rflte_dm_update_wtr_wxe
(
   rflte_mc_carrier_info_type carrier_info,
   rfm_device_enum_type rfm_device,
   rfcom_lte_band_type band,
   rfcom_lte_bw_type bw,
   rfdevice_lte_script_data_type *script_data_ptr
);
   
void rflte_dm_update_qpoet_script
(
   rflte_mc_carrier_info_type carrier_info,
   rfm_device_enum_type rfm_device,
   rfcom_lte_band_type band,
   rfdevice_lte_script_data_type *script_data_ptr
);

void rflte_dm_oride_txagc_output_power
(
   uint8  sub_id,
   uint32 carrier_idx,
   boolean pwr_oride_enable,
   int32 counts_pwr_oride_val
);

void rflte_dm_ftm_rf_enabled
(
   uint32  carrier_idx,
   boolean enable
);

void rflte_dm_cal_mode_enabled
(
  uint8 sub_id,
  boolean enable
);

void 
rflte_dm_update_rx_on_off_scripts(uint8                             sub_id,
                                  rflte_mc_carrier_info_type        carrier_info,
                                  rfcom_lte_band_type               band,
                                  rfdevice_lte_script_data_type*    script_data_ptr,
                                  lm_handle_type                    rxlm_handle,
                                  boolean                           on_off_flag);

void rflte_dm_update_rx_mode_enable_disable_scripts( rflte_mc_carrier_info_type        carrier_info,
                                                rfcom_lte_band_type               band,
                                                rfdevice_lte_script_data_type     *script_data_ptr,
                                                lm_handle_type                    handle_id,
                                                boolean                           on_off_flag );

void rflte_dm_update_rx_path_enable_disable_scripts( rflte_mc_carrier_info_type       carrier_info,
                                                     rfcom_lte_band_type              band,
                                                     rfdevice_lte_script_data_type    *script_data_ptr,
                                                     lm_handle_type                   handle_id,
                                                     boolean                          enable_disable_flag,
                                                     rfcom_lte_ca_mode_type           ca_mode,
                                                     boolean                          rebuild_pcc_script);

void rflte_dm_rxagc_band_config(uint8  sub_id,
                                rflte_mc_carrier_info_type carrier_info,
                                rfcom_lte_band_type rf_band,
                                rflte_nv_tbl_type *lte_nv_tbl_ptr,
                                rfcom_lte_ca_mode_type ca_mode,
                                lm_handle_type rxlm_buffer_idx,
                                rfcom_lte_earfcn_type curr_rx_chan,
                                rfcom_lte_bw_type rx_bw,
                                boolean is_irat_mode,
                                uint8 alt_path,
                                rfdevice_rx_mode_type current_rx_mode,
                                rfdevice_lte_dev_cfg_params *dev_list_per_carrier );

void rflte_dm_rxagc_alt_path_config(uint8  sub_id,
                                    rflte_mc_carrier_info_type carrier_info,
                                    rfcom_lte_band_type rf_band,
                                    rflte_nv_tbl_type *lte_nv_tbl_ptr,
                                    boolean alt_path_ca_active,
                                    rfcom_lte_ca_mode_type ca_mode,
                                    lm_handle_type rxlm_buffer_idx,
                                    rfcom_lte_earfcn_type curr_rx_chan,
                                    uint8 alt_path,
                                    rfcom_lte_bw_type rx_bw,
                                    rfdevice_rx_mode_type current_rx_mode,
                                    rfdevice_lte_dev_cfg_params *dev_list_per_carrier );


boolean rflte_dm_rxagc_band_config_per_cell(uint8  sub_id,
                                            uint32 cell_id, rflte_mc_carrier_info_type carrier_info,
                                            rfcom_lte_band_type rf_band, rflte_nv_tbl_type *lte_nv_tbl_ptr,
                                            lm_handle_type rxlm_buffer_idx, rfcom_lte_earfcn_type curr_rx_chan, rfcom_lte_ca_mode_type ca_mode,
                                            boolean is_irat_mode,
                                            uint8 alt_path,
                                            rfcom_lte_bw_type rfcom_bw,
                                            rfdevice_rx_mode_type current_rx_mode,
                                            rfdevice_lte_dev_cfg_params *dev_list_per_carrier,
                                            uint32 asdiv_sw_pos);


boolean rflte_dm_rx_add_cells_to_dm( lm_handle_type rxlm_buffer_idx, 
                                     cell_id_t* cell_id ,
                                     uint8 num_cells_to_add);

void rflte_dm_rx_delete_cells_from_dm(rflte_mc_carrier_info_type carrier_info);

boolean rflte_dm_populate_rsb_coeff( rflte_mc_carrier_info_type carrier_info,
                                     lm_handle_type rxlm_buffer_idx,
                                     rfcom_lte_band_type rf_band,
                                     rfcom_lte_earfcn_type curr_rx_chan,
                                     rfcom_lte_bw_type rx_bw,
                                     rfcom_lte_ca_mode_type ca_mode,
                                     uint16 therm_val,
                                     boolean populate_default_rsb,
                                     rflte_nv_tbl_type *lte_nv_tbl_ptr,
                                     uint32 asdiv_sw_pos,
                                     uint8 carrier_alt_path,
                                     rfdevice_trx_sub_type sub_id);

boolean rflte_dm_repopulate_rx_gain_offsets(uint32 cell_id, 
                                            rflte_mc_carrier_info_type carrier_info,
                                            lm_handle_type rxlm_buffer_idx,
                                            rfdevice_lte_get_rx_gain_settings_type   rx_gain_settings,
                                            rfcom_lte_earfcn_type curr_rx_chan,
                                            rfcom_lte_ca_mode_type ca_mode,
                                            rflte_nv_tbl_type *lte_nv_tbl_ptr,
                                            rfcom_lte_band_type rf_band);


void 
rflte_dm_update_txpll_afc_script
(
uint32                           cell_index,
rf_hal_bus_resource_type         script_type,
rfdevice_lte_script_data_type   *afc_script_buffer,
int32                            rx_freq_error_in_hz,
lm_handle_type                   txlm_buffer_idx
);

void 
rflte_dm_update_rxpll_afc_script
(
uint32                           cell_index,
lte_LL1_ul_txpll_script_array_t *rxpll_script_buf_ptr
);

uint32 rflte_dm_get_path_from_device(uint8                 sub_id,
                                     rfm_device_enum_type  device,
                                     uint32                carrier_idx);


void 
rflte_dm_intra_band_ca_active
(
boolean            active_flag
);

boolean
rflte_dm_get_intra_band_ca_active
(
  void
);

int16 
rflte_dm_get_inband_rxagc_val
(
  uint8 sub_id,
  rfm_device_enum_type device, 
  uint32 carrier_idx
);

void 
rflte_dm_set_forced_rx_gain_state
(
rfm_device_enum_type                 device, 
uint8                                rx_gain_force_state, 
int16                                expected_agc_cal, 
uint32                               carrier_idx
);

void 
rflte_dm_set_rx_gain_force
(
uint32                       carrier_idx,
rfm_device_enum_type         device
);

uint8 
rflte_dm_get_rx_gain_state
(
uint32                       rflm_path,
uint32                      carrier_idx
);


void 
rflte_dm_set_rx_gain_rise_fall_thrshlds
(
rfm_device_enum_type             device, 
uint8                            index, 
int16                            rise_thrshlds, 
uint32                           carrier_idx
);

int16 
rflte_dm_get_delta_rx_gain_offset
(
rfm_device_enum_type           device,
uint32                         carrier_idx,
uint8                          index
);

void 
rflte_dm_update_therm_script
( 
lm_handle_type                    txlm_buffer_idx,
rfm_device_enum_type              device,
rfcom_lte_band_type               band, 
boolean                           update_read_scripts
);

int32 
rflte_dm_get_oride_txagc
(
  uint8   sub_id,
  uint32  carrier_idx
);

uint8 
rflte_dm_get_current_pa_state
(
  uint8 sub_id
);

uint8 
rflte_dm_get_start_rb_index
(
  uint8   sub_id,
  uint32  carrier_idx
);

uint8 
rflte_dm_get_num_rbs
(
  uint8   sub_id,
  uint32  carrier_idx
);

int32 
rflte_dm_get_lte_pwr_db10
(
  uint8    sub_id,
  uint32   carrier_idx
);

uint16 
rflte_dm_get_tx_gain_index
(
  uint8    sub_id,
  uint32   carrier_idx
);

uint16 
rflte_dm_get_baseband_backoff
(
  uint8    sub_id,
  uint32   carrier_idx
);

void 
rflte_dm_set_therm_read_period 
(
  lm_handle_type         txlm_buffer_idx,
  int32                  therm_read_period_ms
);

uint16 rflte_dm_get_pa_smps_pdm(uint8 sub_id,uint32 carrier_idx);

void rflte_dm_update_tdd_fdd_band_type
( 
  boolean band_is_tdd, 
  rflte_mc_carrier_info_type  *carrier_info
);

void rflte_dm_update_tpl(uint32 carrier_idx, 
                         uint8 subframe_num, 
                         int32 tpl_srs);

void rflte_dm_rx_intra_band_ca_active(uint8 sub_id, uint8 active_mask );

void rflte_dm_rx_single_rx_ca_active(uint8 sub_id, uint8 active_mask );

void rflte_dm_rx_chain_mask(uint8 sub_id, uint8 active_mask , uint8 carrier_idx);

uint8 rflte_dm_rx_get_chain_mask(uint8 sub_id,uint8 carrier_idx);

void rflte_dm_rx_alt_path_ca_active(uint8 sub_id, boolean active_flag, uint8 carrier_idx, uint8 path);


void rflte_dm_rx_set_rxlm_buffer_idx(uint8 sub_id,uint32 carrier_idx, uint32 path_idx, lm_handle_type rxlm_handle);

void rflte_dm_update_tpl_srs(uint32 carrier_idx, 
                             uint8 subframe_num, 
                             int32 tpl_srs);

/* API to init RFLM FED Txagc Logging */
void rflte_dm_init_debug ( void );

void rflte_dm_common_rxagc_init_debug(void);

void
rflte_dm_update_p_cmax
(
   uint8 subframe_num,
   uint32 carrier_idx,
   lm_handle_type txlm_buffer_idx,
   rflte_nv_tbl_type   *rflte_nv_tbl_ptr,
   /* Freq comp */
   rflte_core_freq_comp_type *freq_comp,
   rflte_core_freq_comp_type *freq_comp_srs,
   boolean srs_enabled,
   boolean lin_vs_freq_vs_temp_enabled,
   boolean dpd_enabled,
   /* PA SWPTS */
   uint8 mpr_value,
   rflte_core_txpl_ns_type ns_x,
   uint8 backoff_value,
   /* TPL */
   int32 tpl,
   int32 dtpl,
   int32 tpl_coex,
   int32 dtpl_coex,
   int32 tx_max_ue_class_pwr
);

void 
rflte_dm_update_fbrx_rb_based_gain(
  uint8 subframe_num,
  uint32 carrier_idx,
  int32   fbrx_gain,
  int16   tx_power,
  uint8   ubias,
  uint16  rx_scale,
  uint16  predet_index,
  uint8 fbrx_gain_state,
  lm_handle_type txlm_handle
);

void rflte_dm_get_xpt_logging_data
(
   uint8   sub_id,
   uint32  carrier_idx, 
   ftm_log_lte_tx_xpt_data_path_type * xpt_data 
);

void rflte_dm_get_dm_logging_data
( 
   uint8   sub_id,
   uint32  carrier_idx, 
   ftm_log_lte_tx_agc_enh_data_type *ftm_lte_tx_agc_enh_data 
);

boolean rflte_dm_set_rgi_0(uint32 carrier_idx, 
                           rfm_device_enum_type rfm_device,
                           rfcom_lte_band_type band,
                           rf_device_execution_type execution_type,
                           rfdevice_tx_lut_data_type* lut_data,
                           rfcom_lte_bw_type bw_for_lut_look_up,
                           lm_handle_type txlm_handle_idx);

boolean rflte_dm_populate_rgi0_dm_script(rflm_lte_tech_data_s *lte_dm ,
                                         rflm_lte_fed_static_s *fed_static,                                        
                                         rfdevice_tx_lut_data_type* lut_data);

void rflte_dm_populate_rgi_0_for_tdd
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type *script_data_ptr,
  rfcom_lte_bw_type bw_for_lut_look_up
);

/* RFSW sets this flag to FALSE after completing the Gain-correction processing */
void rflte_dm_set_fbrx_update_active_flag(uint8 sub_id,uint32 carrier_idx, boolean fbrx_active_status);

/* RFSW sets this flag to TRUE/FALSE to control the FBRx Gain corr feature */
void rflte_dm_set_fbrx_master_flag (uint32 carrier_idx, boolean fbrx_master_flag, lm_handle_type txlm_buffer_idx);

#ifdef FEATURE_RF_HAS_WTR3925
/* RFSW sets this flag to TRUE/FALSE and sets Pcell Drx LM handle to handle FBRX conflict cases */
void rflte_dm_set_fbrx_drx_conflict(uint8 sub_id,rflte_mc_carrier_info_type carrier_info, lm_handle_type pcc_drx_lm_handle, lm_handle_type scc_drx_lm_handle, rfcom_lte_band_type band);
void rflte_dm_set_fbrx_vswr_offset (lm_handle_type tx_handle,uint32 sample_offset);
#endif

/* To Set/Reset Therm_status flag     */
void rflte_dm_set_therm_status_flag(uint8 sub_id,uint32 carrier_idx, boolean therm_status_flag, lm_handle_type  txlm_buffer_idx);

/* To Set/Reset Therm_status flag     */
void rflte_dm_set_therm_progress_status_flag
(
  uint32 carrier_idx, 
  boolean therm_status_flag, 
  lm_handle_type txlm_buffer_idx
);

/* RFSW to set the power level for triggering the FBRx measurement */
boolean rflte_dm_set_fbrx_trigger_pwr_lvl_db10 
(
  rflte_mc_carrier_info_type carrier_info,
                                                int16 pwr_lvl, 
  uint8 slot_type
);

void rflte_dm_set_pwr_ctrl_mode
(
  rflte_mc_carrier_info_type carrier_info,
  rflm_lte_tx_pwr_ctrl_type pwr_ctrl_mode
);

rflm_lte_tx_pwr_ctrl_type rflte_dm_get_pwr_ctrl_mode(uint8 sub_id,uint32 carrier_idx);

/* RFSW to enable hdet trigger based on timer */
boolean rflte_dm_trigger_hdet_active_flag 
(
  lm_handle_type  txlm_buffer_idx,
  boolean hdet_enable
);

boolean rflte_dm_set_hdet_read_flag (lm_handle_type  txlm_buffer_idx,
                                     boolean hdet_read_finished);
boolean rflte_dm_set_par_done_flag(uint8   sub_id,
                                   uint32  carrier_idx,
                                   boolean hdet_read_finished);

boolean rflte_dm_get_hdet_read_flag(uint8 sub_id,uint32 carrier_idx);

boolean rflte_dm_get_hdet_trigger_flag(uint8 sub_id,uint32 carrier_idx);

/* RFSW to set the power level for triggering the HDET measurement */
boolean rflte_dm_set_hdet_trigger_pwr_lvl_db10 (uint32 carrier_idx, 
                                                int16 pwr_lvl,
                                                lm_handle_type  txlm_buffer_idx);

/* API to retreive the flag from DM */
boolean rflte_dm_get_fbrx_master_flag(uint32 carrier_idx,
                                      lm_handle_type  txlm_buffer_idx);

/* API to retreive the flag from DM */
boolean rflte_dm_get_fbrx_slow_ramp_status_flag(uint8 sub_id,uint32 carrier_idx);

void rflte_dm_reset_fbrx_slow_ramp_status_flag
(
  rflte_mc_carrier_info_type carrier_info
);

int16 rflte_mc_fed_get_rx_gain_offset_from_dm(uint8 sub_id,
                                              rfm_device_enum_type device, 
                                              uint32 carrier_idx, 
                                              uint8 lna_state, 
                                              lm_handle_type rxlm_buf_index);

void rflte_dm_update_fbrx_ldo_off_to_on_setup_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfdevice_lte_script_data_type *script_data_ptr
);

#ifdef FEATURE_RF_HAS_WTR3925
void rflte_dm_update_fbrx_wtr_path_on_setup_event
(
   rflte_mc_carrier_info_type carrier_info,
   rfdevice_lte_script_data_type *script_data_ptr,
   uint8 fbrx_gain_state
);
#else
void rflte_dm_update_hkadc_override_on_event
(
   rflte_mc_carrier_info_type carrier_info,
   rfdevice_lte_script_data_type *script_data_ptr
);

void rflte_dm_update_hkadc_override_off_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfdevice_lte_script_data_type *script_data_ptr
);
#endif

void rflte_dm_update_fbrx_ldo_on_to_off_disable_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfdevice_lte_script_data_type *script_data_ptr
);

void rflte_dm_update_fbrx_ldo_always_on_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfdevice_lte_script_data_type *script_data_ptr
);

void rflte_dm_update_fbrx_path_config_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfdevice_lte_script_data_type *script_data_ptr
);

void rflte_dm_update_fbrx_config_coupler_fwd_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfdevice_lte_script_data_type *script_data_ptr
);

void rflte_dm_update_fbrx_config_coupler_rev_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfdevice_lte_script_data_type *script_data_ptr
);

void rflte_dm_update_fbrx_adc_hdet_enable_setup_event
(
   rflte_mc_carrier_info_type carrier_info,
   rfdevice_lte_script_data_type *script_data_ptr,
   rflm_lte_ccs_event_type_e ccs_event_id
);

void rflte_dm_set_rxlm_buffer_idx(uint32 carrier_idx, rfcom_device_enum_type device, lm_handle_type rxlm_handle);

boolean rflte_dm_set_asdiv_lm_index(rflte_mc_carrier_info_type* carrier_info,
                                    lm_handle_type rxlm_buffer_idx);

void 
rflte_dm_update_tx_num_pa_states
(
  rflte_nv_tbl_type *lte_nv_tbl_ptr,
  rflte_mc_carrier_info_type carrier_info,
  boolean intra_band_ulca_active
);


boolean rflte_dm_populate_script(rflm_lte_fed_static_s* fed_static, 
                                 const rf_hal_bus_resource_script_type **resource_script,
                                 int16 resource_script_idx,
                                 rflm_lte_ccs_event_type_e event_id,
                                 rflm_lte_rf_script_type* tx_off_script);

void
rflte_dm_init_txagc_lin_state
(
  uint8  sub_id,
  uint32 carrier_idx,
  uint16 lte_pa_state_max
);

void
rflte_dm_reset_txagc_lin_state
(
  uint8  sub_id,
  uint32 carrier_idx,
  uint16 lte_pa_state_max
);

boolean
rflte_dm_allocate_txagc_lin_state
(
  uint8  sub_id,
  uint32 carrier_idx,
  uint16 lte_pa_state_max
);

void rflte_dm_tx_get_linear_tx_power(uint8  sub_id,
                                     rflte_mc_linear_tx_power_input_s* tx_power_input,
                                     rflte_mc_linear_tx_power_output_s* tx_power_output);

int16 rflte_dm_get_bias_offset(uint32 tx_freq, 
                               rfcom_lte_band_type band,
                               uint32 carrier_idx, 
                               rflte_nv_tbl_type *rflte_nv_tbl_ptr);

void rflte_dm_set_tx_lo_offset_in_KHz(rflte_mc_carrier_info_type carrier_info, int32 tx_lo_offset_in_KHz);
void rflte_dm_rx_set_temp_comp(uint8 sub_id,uint32 carrier_idx, uint8 path, int16 temp_comp );

#ifdef FEATURE_RF_HAS_QTUNER
boolean rflte_dm_common_update_tuner_config_event(uint8  sub_id);
#endif /* FEATURE_RF_HAS_QTUNER */

void rflte_dm_update_rx_lo_offset_in_KHz(uint8 sub_id,rflte_mc_carrier_info_type *carrier_info, int32 freq_offset);

void rflte_dm_set_rx_lo_offset_in_KHz(uint8 sub_id,rflte_mc_carrier_info_type *carrier_info, int32 rx_lo_offset_in_KHz);

boolean rflte_dm_get_rflm_path_from_carr_info ( rflte_mc_carrier_info_type   *carrier_info, uint32 *path_index_ptr );


#if defined(FEATURE_RF_ASDIV)
boolean rflte_dm_asdiv_reload_lna_data
(
   uint32 curr_sw_pos /*!< New Antenna Position */,
   uint32 prev_sw_pos,
   rflm_dm_handle_id_t rflm_handle_idx,
   rfcom_lte_band_type rx_band,
   uint32 rx_freq,
   rfcom_lte_bw_type rfcom_bw,
   uint8 nv_container,
   boolean intra_band_ca_active,
   uint32 carrier_index,
   rfm_device_enum_type device,
   rfdevice_rx_mode_type curr_rx_mode,
   rfdevice_lte_dev_cfg_params *dev_list_per_carrier
);
#endif

boolean rflte_dm_init_rxagc_mutex
(
	 rflm_dm_handle_id_t rxlm_handle,
	 boolean init_mutex
);

void rflte_dm_set_txagc_params(uint8 sub_id,uint32 carrier_idx, rflm_lte_sw_to_fed_s *sw_to_fed, lm_handle_type txlm_handle[RFA_RF_LTE_MAX_TX_CELLS_SUPPORTED],boolean intra_ulca_active_flag);

void rflte_dm_update_dac_backoff_val(rflte_mc_carrier_info_type carrier_info,
                             rfcom_lte_band_type rf_band,
                             rfcom_lte_bw_type rfcom_bw);


void rflte_dm_tx_pa_swp_force_to_apt_mode(uint32 carrier_idx, lm_handle_type txlm_buffer_idx);

void rflte_dm_build_hdet_par_event
(
  rflte_mc_carrier_info_type carrier_info,
  rfdevice_lte_script_data_type *script_data_ptr,
   rfm_device_enum_type rfm_device,
  rfcom_lte_band_type band,
  rfcom_lte_bw_type tx_bw,
  rfcom_lte_earfcn_type tx_channel,
  boolean is_par_supported
);

uint8 rflte_dm_get_rx_tx_asm_buf_idx(lm_handle_type  txlm_buffer_index);

void rflte_dm_set_rx_tx_asm_buf_idx(lm_handle_type  txlm_buffer_index);

uint8 rflte_dm_update_rx_tx_asm_buf_idx(lm_handle_type  txlm_buffer_index);


#endif /*RFLTE_DM_H*/
