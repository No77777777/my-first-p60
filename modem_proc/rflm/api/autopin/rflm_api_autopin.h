#ifndef RFLM_API_AUTOPIN_H
#define RFLM_API_AUTOPIN_H

/*
  @file
  rflm_api_autopin.h

  @brief
  This file contains interfaces defined by RFLM AutoPin.
*/

/*=============================================================================
 
  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document are confidential 
  and proprietary information of Qualcomm Technologies Incorporated and all rights therein are 
  expressly reserved. By accepting this material the recipient agrees that this material and the 
  information contained therein are held in confidence and in trust and will not be used, copied, 
  reproduced in whole or in part, nor its contents revealed in any manner to others without the 
  express written permission of Qualcomm Technologies Incorporated.
 
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rflm.mpss/2.11/api/autopin/rflm_api_autopin.h#1 $

when       who    what, where, why
--------   ---    -------------------------------------------------------------
05/28/16   wxl    Add autopin enable flag check based on tx handle
04/28/16   qzh    Update log packet after FW release
03/17/16   zhh    Added fields to log one shot autopin
03/07/16   zhh    Added new API to calculate the online pin misalign
                  and delta pout
03/08/16   qzh    Add WTR AMAM autopin Txagc related changes
02/18/16   qzh    WTR3925 Autopin port from Thor.
02/16/15    dw    Added delta gain Pin Pout LUT in AutoPin DM
02/15/16   ag     Adding field to log pin from NV
01/27/16   vs     Add txc_rate_ustmr to Autopin structure
12/24/15   vs     Updated CCS time for Autopin enable script reduction
12/15/15   kma    Updated CCS time for DC loop change
12/03/15   whz    Add field of wtr_ccs_end_time
11/10/15   whz    Add WTR delay macros
11/05/15   whz    AutoPin on TA: online support
10/30/15   kma    Increased the max REFLOG capture size to 4096
10/22/15   kma    Add macro to support PAR autopin
09/21/15   whz    Make some macros visible outside
08/20/15   cdb    Add env_scale backoff criteria to autopin run decision
07/21/15   cdb    Add EFS support 
07/16/15   cdb    Abstract Thor/AMAM specific processing 
06/30/15   cdb    Add override controls
06/19/15   whz    Log packet initial support
06/09/15   whz    Provide buffer to store FBRx IQ
06/08/15   whz    Add new API to return the latest Pin accum
06/05/15   whz    Fix FW standalone compilation issue
06/04/15   whz    Initial version

===========================================================================*/

#include "lm_types.h"
#include "rflm_api_cmn.h"
#include "rflm_txagc_api.h"
#include "rflm_api_autopin_fw.h"
#include "rflm_txagc_api.h"
#ifdef FEATURE_TABASCO_MODEM
#include "fw_autopin_intf.h"
#endif
#if (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)
#include "fw_psamp_autopin_intf.h"
#endif
/* from FBRx AG file */
#define RFLM_AUTOPIN_MAX_TX_SAMPLE_SIZE 512
#define RFLM_AUTOPIN_MAX_RX_SAMPLE_SIZE 695

#define RFLM_AUTOPIN_REFLOG_MAX_SAMPLE_SIZE 4096

#define RFLM_AUTOPIN_MAX_PIN_OFFSET (60) /* 6dB */
#define RFLM_AUTOPIN_ENV_SCALE_BACKOFF_LIMIT (50) /* -dB10 */
#define FWPSAMP_CORR_SNR_THRESHOLD (300) /* 3dB*/
#define AUTOPIN_PAR_ERROR_THRESHOLD 0 /*0*/
#ifdef FEATURE_RF_HAS_WTR3925
#define RFLM_AUTOPIN_AMAM_ERROR_IMBALANCE_OFFSET (0)
#define RFLM_AUTOPIN_AMAM_ERROR_DEADZONE (2500)
#endif

#define RFLM_AUTOPIN_NUM_PIN 8 /* AutoPin LUT size*/
#define RFLM_AUTOPIN_NUM_DPD 4 /* Number of DPDs*/

#ifdef FEATURE_TABASCO_MODEM
extern fw_autopin_proc rflm_autopin_fw_autopin_fn;
#endif

extern rflm_handle_tx_t rflm_autopin_current_active_tx_handle;


/*WTR PAR programming delay in USTMR unit*/
#define RF_AUTOPIN_CCS_WTR_PAR_G2_DELAY_DEBUG ((uint32)(19200000/1000000.0*168))
#define RF_AUTOPIN_CCS_WTR_PAR_G2_DELAY       ((uint32)(19200000/1000000.0*156))
#define RF_AUTOPIN_CCS_WTR_PAR_G1_DELAY_DEBUG ((uint32)(19200000/1000000.0*170))
#define RF_AUTOPIN_CCS_WTR_PAR_G1_DELAY       ((uint32)(19200000/1000000.0*157))

typedef struct
{
  rflm_tech_id_t tech_id;
  rflm_txagc_xpt_mode_t xpt_mode;
  uint8 pa_state;
} rflm_autopin_get_run_decision_in_t;


typedef struct
{
  boolean run_autopin;
} rflm_autopin_get_run_decision_out_t;


typedef enum {
  RFLM_AUTOPIN_DEFAULT,
  RFLM_AUTOPIN_ACQ,
  RFLM_AUTOPIN_TRK,
  RFLM_AUTOPIN_ERROR
} rflm_autopin_state;


typedef enum {
RFLM_AUTOPIN_ACQ_TIME_OUT = 192000 ,
RFLM_AUTOPIN_TRK_TIME_OUT = 1920000,
}rflm_autopin_run_period;

#define MAX_PIN_SIGN_SAMPLES  10



typedef struct {
  rflm_handle_tx_t handle;
  boolean buf_in_use;
  rflm_tech_id_t tech;
  boolean ready[RFLM_TXAGC_PA_STATE_MAX];
  rflm_autopin_state run_state[RFLM_TXAGC_PA_STATE_MAX];
  rflm_db10_t pin_accum[RFLM_TXAGC_PA_STATE_MAX];
  /*! @brief Pout compensation from Auto-Pin*/
  rflm_db10_t pout_delta;
  rflm_pin_polarity pin_sign[RFLM_TXAGC_PA_STATE_MAX][MAX_PIN_SIGN_SAMPLES];
  uint8 pa_state;
  rflm_autopin_iqbuf_t txiqbuf;
  rflm_autopin_iqbuf_t rxiqbuf;
  int logging_dm_info_context_id;
  int logging_iq_capture_context_id;
  uint32 env_scale_backoff;
  fw_autopin_capture_input_t reflog_capture_input;
  fw_autopin_psamp_input_t reflog_proc_param;
  fw_autopin_output_params_t reflog_output;
  uint16 wtr_capture_size;
  uint32 wtr_ccs_end_time;
  uint32 txc_rate_ustmr;
  boolean auxiliary_data;
  int16 pin_init;
  boolean init_done;
  rflm_autopin_run_period run_period;
  int8 gain_comp_exp;
  int8 one_shot_pin;
  rflm_autopin_algo_type proc_type;
  boolean run_decision;
  boolean autopin_enable_flag;
} rflm_autopin_ctl_data_t;

/*! @brief  Results of error processing */
typedef struct 
{
  uint16  seq_nr;         /* copy of sequence number passed to Common FW when 
                             autopin processing requested, to uniquely identify 
                             the processing result */
  boolean valid_result;   /* TRUE when valid result is delivered. 
                             FALSE to indicate FW encountered an error during 
                             autopin processing */
  int32   meas_error;     /* Metric measured error vs reference. Autopin  
                             algorithm will use this to decide pin adjustment */
  int16   ss_gain_db;     // Small signal gain - for logging
  uint16  reflog_max_idx; // AMAM LUT max index - for logging
  #if (RFLM_FEATURE_BUILD_MODE==RFLM_FEATURE_BUILD_NORMAL)
  int16   kernal_weights_real[FW_PSAMP_VOLTERRA_MAX_KERNELS]; // Forward model, kernel weights real part - for logging
  int16   kernal_weights_imag[FW_PSAMP_VOLTERRA_MAX_KERNELS]; // Forward model, kernel weights imaginary part - for logging
  #else
  int16   kernal_weights_real[5]; // Forward model, kernel weights real part - for logging
  int16   kernal_weights_imag[5]; // Forward model, kernel weights imaginary part - for logging  
  #endif
  boolean status;
} rflm_autopin_proc_results_t;

typedef struct {
  boolean debug_active;       /* activate debug mode to allow overrides */
  boolean pin_run_freeze;     /* freeze autopin loops */
  boolean pin_accum_freeze;   /* freeze pin accum updates */
  boolean pin_accum_override; /* override pin_accum[] data */
  rflm_db10_t pin_accum_ovr_val[RFLM_TXAGC_PA_STATE_MAX];
} rflm_autopin_override_ctl_t;

/*! @brief LUT for Pin Pout Gain delta */
typedef struct
{
  /*! @brief DPD index corresponding to the Pin LUT */
  uint8 dpd_idx;
  /*! @brief number of Pin misalignment entries in the LUT */
  uint8 num_pin;
  /*! @brief Pin misalignment  */
  int8 pin_misalign[RFLM_AUTOPIN_NUM_PIN];
  /*! @brief gain compression/expansion  */
  int8 gain_comp[RFLM_AUTOPIN_NUM_PIN];
  /*! @brief Delta Pout  */
  int8 pout_delta[RFLM_AUTOPIN_NUM_PIN];      
} rflm_autopin_delta_gain_pin_pout_table_type;

/*! @brief Delta gain Pin Pout LUT data  */
typedef struct
{
  /*! @brief Valid flag for the LUT data */
  boolean valid[RFLM_AUTOPIN_NUM_DPD];
  /*! @brief LUT data */
  rflm_autopin_delta_gain_pin_pout_table_type lut[RFLM_AUTOPIN_NUM_DPD];           
} rflm_autopin_delta_gain_pin_pout_data_type;

typedef struct {
  rflm_autopin_ctl_data_t autopin_ctl;
  rflm_autopin_proc_results_t results_log;
  rflm_autopin_override_ctl_t override_ctl;
  rflm_autopin_delta_gain_pin_pout_data_type pin_pout_lut;
} rflm_autopin_dm_template_t;


#ifdef __cplusplus
extern "C" {
#endif


extern boolean rflm_autopin_in_progress;


boolean rflm_autopin_is_enabled( rflm_tech_id_t tech, rflm_handle_tx_t handle);


rflm_err_t rflm_autopin_get_run_decision( rflm_handle_tx_t handle,
                                          rflm_autopin_get_run_decision_in_t  *in,
                                          rflm_autopin_get_run_decision_out_t *out );


rflm_err_t rflm_autopin_get_latest_pin(rflm_handle_tx_t handle,
                                       uint8 pa_state,
                                       rflm_db10_t *pin);

rflm_err_t rflm_autopin_get_latest_pout(rflm_handle_tx_t handle, rflm_db10_t *pout);

rflm_err_t rflm_autopin_update_proc_result( rflm_handle_tx_t handle, 
                                            rflm_autopin_proc_results_t *results);

void rflm_autopin_cleanup_buf(rflm_autopin_dm_template_t *template_ptr);

void rflm_autopin_set_params(rflm_tech_id_t tech, int32 trk_thresh, uint8 step_size);

rflm_err_t rflm_autopin_update_env_scale(rflm_handle_tx_t handle, 
                                         uint32 env_scale_backoff);

boolean rflm_autopin_is_rgi_max(rflm_handle_tx_t tx_handle);

#ifdef __cplusplus
}
#endif


#endif /* RFLM_API_AUTOPIN_H */
