/*!
  @file
  fw_psamp_fbrx_ilpc.h

  @brief
  Definitions for FBRX ILPC sample processing.
*/

/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by this document
  are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly
  reserved. By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in trust
  and will not be used, copied, reproduced in whole or in part, nor its
  contents revealed in any manner to others without the express written
  permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //source/qcom/qct/modem/fw/components/fw_psamp.mpss/rel/17.0/api/fw_psamp_fbrx_ilpc_intf.h#2 $
$DateTime: 2020/05/07 08:13:33 $
$Author: pwfwsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/16/15   NB      Initial version.
===========================================================================*/

#ifndef _FW_PSAMP_FBRX_ILPC_INTF_H_
#define _FW_PSAMP_FBRX_ILPC_INTF_H_

#include "rflm_fbrx_typedef_ag.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  // Tx samples, packed in 8 bit I/Q as HW packs (upper 8 bits are I, lower 8
  // bits are Q). Also make it a uint32 pointer
  //
  uint32 * tx_capture;

  // Rx samples in packed PCFL20, same as HW outputs.
  //
  uint32 * rx_capture;

  // Number of Tx samples.
  //
  uint16 tx_len;

  // Number of Rx samples.
  //
  uint16 rx_len;

  // Offset of search for Tx within Rx (in samples.) Can be positive or
  // negative. Offset is non-zero only for testing.
  //
  int16 rx_offset;

  // Number of correlations (Set to 0xFFFF if you want to correlate over maximum
  // possible based on rx_offset/ rx_len and search_width )
  uint16 search_width; 

  // Spectral inversion
  uint32 rx_spec_inv;

  // estimate_tau_en will be always set to TRUE for first capture to enable search peak index
  // for VSWR second time capture, need to set to FALSE to disable peak index search
  boolean estimate_tau_en;

} fw_psamp_samp_capt_proc_input_t;

/*! @brief Structure for VSWR input parameters */
typedef struct {
  // Peak of correlation of Rx sample within Tx samples.
  // Will be used for second time VSWR processing
  uint32 peak_idx;
  
  // Fractional part of peak index of Rx sample within Tx samples.
  // Will be used for second time VSWR processing
  float peak_frac;

  // Use aligned sample data length from incident capture 
  // results for reflected sample process
  uint32 align_samp_data_len;

} fw_psamp_vswr_input_t;

/*! @brief Structure for Tx rotation input parameters */
typedef struct {
  /*! @brief Frequency compensation offset enable flag. TRUE for WCDMA/TDS, FALSE for others */
  boolean tx_rot_enable;
  /*! @brief Frequency compensation offset. Will be needed for WCDMA and TDS */
  int32 csr_freq_offset;
  /*! @brief TXC rotator rate in Hz */
  uint32 tx_rot_clk_hz;
  /*! @brief FBRX NB rate in Hz. VSRC_OUTPUT_RATE_HZ >> NUM_WB_NB_DEC */
  uint32 fbrx_nb_rate_hz;

  uint32 num_samp_gap;

  float phase_start_i;
  float phase_start_q;
} fw_psamp_tx_rot_input_t;

/*! @brief Structure for Rx undroop filter input parameters */
typedef struct {
  /*! @brief Number of Rx taps for undroop filter */
  uint16 num_rx_taps;
  /*! @brief Undroop filter taps */
  uint32 *rx_filter_taps;
  /*! @brief Filter mode. 0: real, 1: complex */
  rflm_fbrx_rx_filter_mode_t rx_filter_mode;
} fw_psamp_rx_undroop_input_t;

/*! @brief Structure for time alignment parameters */
typedef struct 
{
  /*! @brief Boolean to enable timing alignment */
  boolean enable_timing_align;
  /*! @brief Pre-determined index to use in case time alignment is not enabled 
    or if timealignment does not return a peak */
  uint16 predet_index;
  /*! @brief Capture and processing input */
  fw_psamp_samp_capt_proc_input_t capt_proc;
  /*! @brief VSWR process parameters */
  fw_psamp_vswr_input_t  vswr_proc;
  /*! @brief Tx rotation parameters */
  fw_psamp_tx_rot_input_t tx_rot_params;
  /*! @brief Rx undroop filter parameters */
  fw_psamp_rx_undroop_input_t rx_undroop_params;
} fw_psamp_fbrx_ilpc_time_align_params_t;

/*! @brief Input required for computing updated gain parameters */
typedef struct 
{
  /*! @brief Calibration gain in 16U15 */
  uint16 gain_fbrx;
  /*! @brief Scale Rx */
  uint16 scale_rx;
  /*! @brief Scale Tx */
  uint16 scale_tx;
  /*! @brief Current gain compensation */
  uint32 current_gain;
  /*! @brief Current alpha in 15S17 */
  int16 current_alpha_m1;
  /*! @brief Current beta in 15S16 */
  int16 current_beta;
  /*! @brief Current LOFT in 15S14 (reserved) */
  int16 current_loft;
  /*! @brief CA enable (reserved) */
  boolean ca_enable;
} fw_psamp_fbrx_ilpc_state_params_t;

typedef struct
{
  /*! @brief Some indentifier for the ILPC processing, 
    if more than 1 is scheduled back to back, no guarantee which one
    finishes earlier. This identifier will help */
  uint32 seq_id;
  /*! @brief ILPC timing alignment parameters */
  fw_psamp_fbrx_ilpc_time_align_params_t timing_align_params;
  /*! @brief Number of time aligned samples to use for processing */
  uint32 Nref;
  /*! @brief ILPC computation state parameters */
  fw_psamp_fbrx_ilpc_state_params_t fbrx_ilpc_state;
} fw_psamp_fbrx_ilpc_input_t;

// Outputs from timing alignment function.
//
typedef struct
{
  // Processing status. Non-zero values indicate error.
  //
  uint8 status;

  // Tx DC I power in 1/100 dB full-scale.
  //
  int16 tx_dc_i;

  // Tx DC Q power in 1/100 dB full-scale.
  //
  int16 tx_dc_q;

  // Tx maximum magnitude in 1/100 dB full-scale.
  //
  int16 tx_max;

  // Post-DC cancellation Tx RMS power in 1/100 dB full-scale.
  //
  int16 tx_rms;

  // Rx DC I power in 1/100 dB full-scale.
  //
  int16 rx_dc_i;

  // Rx DC Q power in 1/100 dB full-scale.
  //
  int16 rx_dc_q;

  // Rx maximum magnitude in 1/100 dB full-scale.
  //
  int16 rx_max;

  // Rx RMS power in 1/100 dB full-scale.
  //
  int16 rx_rms;

  // Rx scaling applied during conversion of PCFL20 samples to SC16.
  // Units are 1/100 dB.
  //
  int16 rx_scale;

  // Time offset from Tx to Rx samples. Units are 1e-4.
  //
  int32 peak_offset;

  // Flatness of Tx-Rx correlation peak -- maximum integer offset magnitude
  // relative to average of both shoulders. Units are 1/100 dB.
  //
  int16 peak_flatness;

  // Correlation coefficient for Tx-Rx integer sample time alignment.
  // Units are 1/100 dB.
  //
  int16 peak_corr;

  // Peak of correlation of Rx sample within Tx samples.
  // Will be used for second time VSWR processing
  int32 peak_idx;
  
  // Fractional part of peak index of Rx sample within Tx samples.
  // Will be used for second time VSWR processing
  float peak_frac;

  // Tx RMS power after Tx-Rx time alignment. Units are 1/100 dB full-scale.
  //
  int16 corr_tx_rms;

  // Rx RMS power after Tx-Rx time alignment. Units are 1/100 dB full-scale.
  //
  int16 corr_rx_rms;

  // Correlation coefficient for Tx-Rx sub-sample time alignment.
  // Units are 1/100 dB.
  //
  int16 corr_coeff;

  // Time alignment SNR. Units are 1/100 dB.
  //
  int16 corr_snr;

  // Round up correlation peak index 
  uint8 xcorr_peak_idx;

  // Time aligned sample data length 
  uint32 align_samp_data_len;

  // Post time aligned Tx sample
  uint32 *tx_sample_time_aligned;
  
  // Post time aligned Rx sample after DC cancellation
  uint32 *rx_sample_time_aligned; 

  // Incident sample process timing alignment status
  uint32 time_align_status;

} fw_psamp_samp_output_t;

typedef struct 
{
  /*! @brief Measured gain error (16U14) */
  uint16 delta_gain;
  /*! @brief Estimate gain (16U15) */
  uint16 gain_est;
  /*! @brief Next gain */
  uint16 next_gain;
  /*! @brief MEasured RSB (16u16) */
  uint16 rsb;
  /*! @brief Updated IQMC parameter (alpha -1 ) 15S17 */
  int16 next_alpha_m1;
  /*! @brief Updated IQMC parameter (beta) 15S16 */
  int16 next_beta;
  /*! @brief Next LOFT value 15S14 (reserved ) */
  int16 next_loft;
} fw_psamp_fbrx_ilpc_output_t;

typedef struct 
{
  /*! @brief TX covariance (32U0) */
  uint64 Kssc;
  /*! @brief Rx covariance */
  uint64 Kyyc;
  /*! @brief RX autocorrelation (32U0) */
  uint64 Ryyc;
  /*! @brief Rx-TX cross-covariance ( 32BitQ|32bitI) */
  uint64 Kys;
  /*! @brief TX sum (32bit I | 32bit Q) */
  uint64 Rs1;
  /*! @brief RX sum (32bit I | 32bit Q) */
  uint64 Ry1;
  /*! @brief Model A coefficient 16SC14 ( Gain) */
  uint32 A;
  /*! @brief Model B coefficient 16SC16 ( DC) */
  uint32 B;
  /*! @brief Model C coefficient 16SC16( RSB) */
  uint32 C;
  /*! @brief Gain imbalanace 16S15 */
  int16 epsilon_div2;
  /*! @brief Phase imbalanace 16S15 */
  int16 dphi_div2;
  /*! @brief Complex gain estimate 16SC14 */
  uint32 H;
  /*! @brief Gain magnitude estimate 16U15 */
  uint16 G;
  /*! @brief RMS error 16S15 */
  int16 LSE;
  /*! @brief Complex squared error (16SC15) */
  uint32 LSE_Cplx;
  } fw_psamp_fbrx_ilpc_debug_output_t;


/*! @brief Output to be sent to RFSW . Contains reall updated params and 
  debug output */
typedef struct 
{
  /*! @brief Echo back the sequence ID for FBRX complete */
  uint32 seq_id;
  /*! @brief Information about actual samples (like DC/ RMS etc) */
  fw_psamp_samp_output_t samp_debug_params;
  /*! @brief New updates to existing parameters prior to thresholding */
  fw_psamp_fbrx_ilpc_output_t new_params;
  /*! @brief Debug parameters computed along the way for ILPC */
  fw_psamp_fbrx_ilpc_debug_output_t debug_params;

} fw_psamp_fbrx_ilpc_output_params_t;

/*! @brief Result callback type */
typedef void (*fw_psamp_fbrx_ilpc_result_fnptr) (fw_psamp_fbrx_ilpc_output_params_t *outp);

typedef struct _fw_psamp_ilpc_job_s
{
  fw_psamp_fbrx_ilpc_result_fnptr result_funcptr;
  fw_psamp_fbrx_ilpc_input_t param;
  fw_psamp_fbrx_ilpc_output_params_t result;
} fw_psamp_fbrx_ilpc_job_t;



/*! @brief Autopin processing function callback type */
typedef void (*fw_psamp_fbrx_ilpc_proc)(fw_psamp_fbrx_ilpc_job_t *job );

extern void fw_psamp_fbrx_ilpc_core_proc(
   /*! Input parameters */
   fw_psamp_fbrx_ilpc_job_t *job,
   uint32 thread_idx );

extern void fw_psamp_fbrx_ilpc_rms_proc(
  /*! Input parameters */
  fw_psamp_fbrx_ilpc_job_t *job,
  uint32 thread_idx );

#ifdef __cplusplus
} // extern "C"
#endif


#endif // _FW_PSAMP_FBRX_ILPC_INTF_H_

