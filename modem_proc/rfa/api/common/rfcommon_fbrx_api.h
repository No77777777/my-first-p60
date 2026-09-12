#ifndef RFCOMMON_FBRX_H
#define RFCOMMON_FBRX_H
/*!
  @file
  rfcommon_fbrx.h

  @brief
  This module contains prototypes and definitions used by common FBRx.
*/

/*==============================================================================

  Copyright (c) 2013 - 2018 Qualcomm Technologies Incorporated. All Rights Reserved

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

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/rfa.mpss/4.11/api/common/rfcommon_fbrx_api.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
02/26/20    ssl    [CR2629611]Increasing RFCOMMON_FBRX_MAX_BUFFERS_LTE for MSIM and ULCA cases
01/25/18    pvsr   Avoid race condition in calculating number of FBRX buffers allocated to LTE
10/26/17   sk      Enable LTE B71 support
11/06/15    JJ     Increase the max num of fbrx handle from 3 to 4
09/21/15    aa     Save TX/FBRx LM buffer info in FBRx state machine
08/20/15    JJ     Added a wrapper api to get fbrx stage info
08/17/15    ska    Increase number of FBRX buffers to 3
07/14/15    aa     Added fbrx_deinit() API for ULCA/DSDA FBRx fixes 
07/14/15    aa     Added support for LTE FBRx C1 NV container
05/22/15    sub    Add new parameters for local updates, max power nv 
04/23/15    ka     Remove hardcoded device 0 for fbrx 
04/01/15    JJ     Increase max fbrx buffer per tech from 1 to 2, for ULCA
03/17/15    sk     Featurize LTE code for 3G flavor compilation
03/11/15    aa     Added support to get valid tx handle from tech
01/29/15    pv     Support for increase the FBRx sample size based on NV control.
01/14/15    aa     Added support to read fbrx therm adc from NV
12/07/14    ndb    Added rfcommon_fbrx_mc_register_vswr_meas_call_back() API
12/03/14    aa     Added support to normalize FBRx_GAIN_VS_TEMP_VS_FREQ items to cal temperature
08/27/14    gh     Added API to check if FBRx ILPC is enabled
08/20/14    ka     Added api to enable/disable fbrx 
08/08/14    aa     Changed gps_state enum type
08/01/14    aa     Added support for rsb comp during calv3 opcode based fbrx setup
07/31/14    aa     Added api to set the gps state and update it to the fbrx dm
07/21/14    JJ     Use therm bins from each tech to get temp comp value
06/10/14    ka     Support for RB-based freq comp 
04/21/14    aa     Added api to return fbrx lm handle 
04/02/14    aa     Added rfcommon_fbrx_nv_get_control_val api
03/21/14    aa     Fix reload nv items for fbrx
03/07/14    aa     Latest FBRx changes for droop cal
02/27/14    aa     Update rfcommon_fbrx_mc_tune_to_chan
02/13/14    aa     Update fbrx temp comp
02/13/14    ka     Added single trigger api 
02/03/14    aa     Get enum tx_cfg_type from rflm_api_fbrx.h
01/30/14    aa     Added support to update fbrx mode
01/14/14    aa     Added support to read FBRx NV's during bootup
01/10/14    aa     Backing out change due to RFCal crash
01/08/14    aa     Added support to read FBRx NV's during bootup
12/12/13    cd     Provide FBRx gain state data to RFLM during linearizer 
                   loading
11/25/13    aa     Updated C2K band class type enum to fix compiler warnings
11/14/13    ka     Added carrier enum
11/14/13    ka     API updates
11/13/13    ka     Update exit mode
10/30/13    ka     Added device wrappers
09/13/13    ka     Initial version

==============================================================================*/

#include "comdef.h"
#include "rfcom.h"
#include "lm_types.h"
#include "rfm_mode_types.h"
#include "rflm_api_cmn.h"
#include "rfdevice_cmn_intf.h"
#include "rfcommon_fbrx_types.h"
#include "rflm_api_fbrx.h"
#include "rfcommon_nv.h"

#define RFCOMMON_FBRX_NUM_MODES  4
#define RFCOMMON_FBRX_NUM_CAL_CHANNELS 16
#define RFCOMMON_FBRX_GAIN_VS_FREQ_NUM_V2 4 /* Num of FBRx gain states for 9x35 support */

#define RFCOMMON_FBRX_THERM_MIN 0
#define RFCOMMON_FBRX_THERM_MAX 255
#define RFCOMMON_FBRX_TEMP_TABLE_SIZ 8
#define RFCOMMON_FBRX_TEMP_BIN_SIZ (RFCOMMON_FBRX_THERM_MAX / (RFCOMMON_FBRX_TEMP_TABLE_SIZ-1))
#define RFCOMMON_FBRX_TXAGC_PA_STATE_MAX 8
#define RFCOMMON_FBRX_TXAGC_TX_LIN_APT_SIZE 64 
#define RFCOMMON_FBRX_MAX_BUFFERS_PER_TECH 1
#define RFCOMMON_FBRX_MAX_BUFFERS_LTE 5   /*LTE Needs four FBRx buffer for ULCA and MSIM L + L*/

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16 rfcommon_fbrx_therm_adc_val_type;

typedef struct
{
  boolean data_valid;
  int32   gain;
  int16   tx_power;
  uint8   ubias;
  uint16  rx_scale;
  uint16  predet_index;
}rfcommon_fbrx_gain_table_type_v2;

typedef struct
{
  boolean apt_only_mode;
  int16   mtpl_db10;
} rfcommon_fbrx_tx_param_type;

typedef struct 
{
  int32 fbrx_lm_handle[RFCOMMON_FBRX_MAX_BUFFERS_LTE];
  boolean is_fbrx_lm_handle_active[RFCOMMON_FBRX_MAX_BUFFERS_LTE];
  int32 tx_lm_handle[RFCOMMON_FBRX_MAX_BUFFERS_LTE];
} rfcommon_fbrx_num_active_tech_buffer_type;

typedef PACK (struct)
{
  /* Number of valid entries in channel list per band */
  uint8  fbrx_gain_chan_size;		   
  /* Channel list , Max list size is 16 */
  uint16 fbrx_gain_chan[RFCOMMON_FBRX_NUM_CAL_CHANNELS];
  /* Variant Marker */
  rfnv_data_variant_marker_type rfnv_data_variant_marker;
  /* Therm ADC value */
  rfcommon_fbrx_therm_adc_val_type fbrx_therm_adc;  
  /* list of FBRX gain for each channel and gain stage */
  rfcommon_fbrx_gain_table_type_v2 fbrx_gain[RFCOMMON_FBRX_NUM_CAL_CHANNELS][RFCOMMON_FBRX_NUM_MODES][RFCOMMON_FBRX_GAIN_VS_FREQ_NUM_V2];
}rfcommon_fbrx_nv_gain_vs_freq_type_v2;

/*----------------------------------------------------------------------------*/
extern uint32 rfcommon_fbrx_mc_get_num_tech_buffers_allocated(rfm_mode_enum_type tech); 

/*----------------------------------------------------------------------------*/
extern rfcommon_fbrx_error_type rfcommon_fbrx_mc_enter_mode
( 
  lm_handle_type *fbrx_lm_handle, 
  rfm_mode_enum_type tech,
  uint8 carrier_idx 
);

/*----------------------------------------------------------------------------*/
extern uint32 rfcommon_fbrx_mc_get_tech_handle( rfm_mode_enum_type tech );

/*----------------------------------------------------------------------------*/
extern uint32 rfcommon_fbrx_mc_get_tech_tx_lm_handle( rfm_mode_enum_type tech, uint8 carrier_idx );

/*----------------------------------------------------------------------------*/
extern uint32 rfcommon_fbrx_mc_get_tech_fbrx_lm_handle( rfm_mode_enum_type tech, uint8 carrier_idx );

/*----------------------------------------------------------------------------*/
extern rfcommon_fbrx_error_type rfcommon_fbrx_mc_init_tx
( 
  lm_handle_type fbrx_lm_handle, 
  rfcommon_fbrx_tx_cfg_type cfg,
  rflm_handle_tx_t tx_handle,
  rfm_mode_enum_type tech,
  uint8 carrier_idx 
);

/*----------------------------------------------------------------------------*/
extern rfcommon_fbrx_error_type rfcommon_fbrx_mc_deinit_tx
( 
  rflm_handle_tx_t tx_handle 
);

/*----------------------------------------------------------------------------*/

extern rfcommon_fbrx_error_type rfcommon_fbrx_mc_register_vswr_meas_call_back
(
  rflm_fbrx_tech_post_process_vswr_measurement_fp_type tech_fp,
  rflm_handle_tx_t tx_handle
);
  
/*----------------------------------------------------------------------------*/

extern rfcommon_fbrx_error_type rfcommon_fbrx_mc_update_mode
( 
  rflm_handle_tx_t tx_handle,
  rfcommon_fbrx_tx_cfg_type cfg 
);

/*----------------------------------------------------------------------------*/
extern rfcommon_fbrx_error_type rfcommon_fbrx_enable
( 
  lm_handle_type fbrx_lm_handle,
  uint8 fbrx_chain 
);

/*----------------------------------------------------------------------------*/
extern rfcommon_fbrx_error_type rfcommon_fbrx_mc_exit
( 
  lm_handle_type fbrx_lm_handle,
  rfm_mode_enum_type tech 
);

/*----------------------------------------------------------------------------*/
extern rfcommon_fbrx_error_type rfcommon_fbrx_mc_tune_to_chan
(
  rfm_device_enum_type rfm_tx_device,
  rflm_handle_tx_t tx_handle, 
  rfm_mode_enum_type tech,
  rfcom_band_type_u band,
  uint32 chan_array[],                                       
  uint8 num_channels,
  lm_handle_type fbrx_lm_handle,
  uint8 fbrx_chain,
  rfcommon_fbrx_tx_param_type tx_params,
  rfcommon_fbrx_nv_container_type c_val  
);

extern rfcommon_fbrx_error_type rfcommon_fbrx_mc_update_temp_comp
(
  rflm_handle_tx_t tx_handle, 
  int16 temperature,
  uint16 ref_cal_temperature,
  uint16 *tx_therm_bins,
  rfcommon_fbrx_nv_container_type c_val
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_fbrx_mc_enable_rf_dev
(
  rfdevice_rxtx_common_class *dev_ptr, 
  uint8 gain_state
);

/*----------------------------------------------------------------------------*/
extern boolean rfcommon_fbrx_mc_disable_rf_dev
(
  rfdevice_rxtx_common_class *dev_ptr
);

void rfcommon_fbrx_update_tx_rx_delay_override(boolean is_override, int32 value);

void rfcommon_fbrx_update_proc_type_override(boolean is_override, uint32 value);


/*----------------------------------------------------------------------------*/
uint8
rfcommon_fbrx_mc_txagc_get_fbrx_gain_state
(
  int32 txlm_handle_id,
  int16 tx_pwr_dBm10
);

/*----------------------------------------------------------------------------*/
boolean 
rfcommon_fbrx_nv_retrieve_wcdma_data
( 
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
);

/*----------------------------------------------------------------------------*/
boolean 
rfcommon_fbrx_nv_retrieve_1x_data
( 
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
);

/*----------------------------------------------------------------------------*/
#ifdef FEATURE_LTE
boolean 
rfcommon_fbrx_nv_retrieve_lte_data
( 
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
);
#endif

/*----------------------------------------------------------------------------*/
boolean 
rfcommon_fbrx_nv_retrieve_tdscdma_data
( 
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
);

/*----------------------------------------------------------------------------*/
boolean 
rfcommon_fbrx_nv_retrieve_control_data
( 
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
);

/*----------------------------------------------------------------------------*/
boolean 
rfcommon_fbrx_nv_retrieve_droop_data
( 
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
);

/*----------------------------------------------------------------------------*/
boolean 
rfcommon_fbrx_nv_retrieve_common_data
( 
  rex_tcb_type* caller_tcb_ptr,
  rex_sigs_type task_nv_wait_sig,
  void (* task_wait_func_ptr)( rex_sigs_type )
);

/*----------------------------------------------------------------------------*/
extern rfcommon_fbrx_error_type 
rfcommon_fbrx_mc_single_trigger
(
  rflm_handle_tx_t tx_handle
);

/*----------------------------------------------------------------------------*/
rfcommon_fbrx_error_type 
rfcommon_fbrx_mc_update_rsb_coeff
( 
  rfm_device_enum_type rfm_tx_device,
  rflm_handle_tx_t tx_handle, 
  lm_handle_type fbrx_lm_handle, 
  rfm_mode_enum_type tech,
  rfcom_band_type_u band 
);

/*----------------------------------------------------------------------------*/
rfcommon_fbrx_error_type 
rfcommon_fbrx_mc_update_nv_control
(
  rflm_handle_tx_t tx_handle, 
  rflm_tech_id_t tech 
);

/*----------------------------------------------------------------------------*/
rflm_tech_id_t rfcommon_fbrx_mc_convert_rfm_mode_to_rflm_tech(rfm_mode_enum_type rfm_tech);

/*----------------------------------------------------------------------------*/
rfcommon_fbrx_control_enum_type
rfcommon_fbrx_nv_get_control_val( rflm_tech_id_t tech );

/*----------------------------------------------------------------------------*/
boolean
rfcommon_fbrx_nv_get_increase_sample_size_nv_status( rflm_tech_id_t tech );

/*----------------------------------------------------------------------------*/
const rfcommon_fbrx_nv_gain_vs_freq_type_v2* rfcommon_fbrx_nv_get_gain_cal_data
(
  rfm_mode_enum_type tech,
  rfcom_band_type_u band,
  rfcommon_fbrx_nv_container_type c_val
);

/*----------------------------------------------------------------------------*/
rfcom_band_type_u 
rfcommon_fbrx_mc_convert_uint8_to_rfcom_band( rfm_mode_enum_type rfm_tech, 
                                              uint8 fbrx_band);
/*----------------------------------------------------------------------------*/
uint8 
rfcommon_fbrx_mc_convert_rfcom_band_to_uint8( rfm_mode_enum_type rfm_tech, 
                                              rfcom_band_type_u rfcom_band);

/*----------------------------------------------------------------------------*/
void rfcommon_fbrx_mc_set_gps_state( rflm_fbrx_gps_state_type gps_state );

/*----------------------------------------------------------------------------*/
void rfcommon_fbrx_mc_enable_fbrx( boolean enable, lm_handle_type txlm_handle  );

/*----------------------------------------------------------------------------*/
rfcommon_fbrx_error_type rfcommon_fbrx_mc_reconfigure_rxlm
( 
  rfm_device_enum_type rfm_tx_device,
  rfm_mode_enum_type tech, 
  rfcommon_fbrx_tx_cfg_type cfg,
  rflm_handle_tx_t tx_handle,
  rfcom_band_type_u band,
  uint8 fbrx_chain
);

/*----------------------------------------------------------------------------*/
rfcommon_fbrx_error_type
rfcommon_fbrx_nv_get_lim_temp_freq_cal_data
( 
  rflm_fbrx_dm_template_t* fbrx_data_ptr,
  uint8 freq_idx,
  uint8 gain_idx,
  int16 *lim_temp_freq_cal_data,
  rfcommon_fbrx_nv_container_type c_val 
);

/*----------------------------------------------------------------------------*/
boolean rfcommon_fbrx_mc_is_ilpc_enabled(rflm_tech_id_t tech );
/*----------------------------------------------------------------------------*/
boolean rfcommon_fbrx_mc_is_fbrx_enabled(rflm_tech_id_t tech );
/*----------------------------------------------------------------------------*/
boolean rfcommon_fbrx_mc_get_stage3_enable_setting(rflm_handle_tx_t handle_id);

/*----------------------------------------------------------------------------*/
uint8 rfcommon_fbrx_mc_get_txc_ref_log_down_sample( rfcommon_fbrx_tx_cfg_type cfg );
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* RFCOMMON_FBRX_H */

