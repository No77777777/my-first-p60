/*!
  @file
  lte_LL1_log_uers.h

  @brief
  UERS log packet data structures 

  @detail
  !!! Please do not modify !!!
  This file was auto-generated on Wed Oct 21 13:01:18 2009.
  Command line used: perl w2h.pl log_cqi cqi 0 0 0 0 0 0
   Note: The params, [0 0 0 0 0 0], correspond to version numbers for
         each log message in the listed order in the ICD

*/

/*===========================================================================

  Copyright (c) 2011 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/lte_LL1_log_uers.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/
#ifndef LTE_LL1_LOG_UERS_H
#define LTE_LL1_LOG_UERS_H


/*==========================================================================*/
/*! @brief
 * 1 UERS tone log (Log code ID:   0xB14F) 
 *  This log packet gives important parameters related to UERS
 *  channel estimation results in TxMode7.
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_UERS_TONES_VERSION 161

typedef struct
{
  uint32 uers_snr_rx0;  ///< UERS SNR for rx0
  uint32 uers_snr_rx1;  ///< UERS SNR for rx1

}uers_snr_log_t;


typedef struct
{
  uint16 layer0_alpha0;                         ///< Alpha for layer 0, parameter 0, 16S11
  uint16 layer1_alpha0;                         ///< Alpha for layer 1, parameter 0, 16S11
  uint16 alpha1;                                ///< Alpha for layer 0/1, parameter 1, 16S11
  uint16 alpha;                                 ///< Alpha for parameters in bit-mask, 16S11
  uint16 mask                             : 9;  ///< Bit-mask of allocated alphas
  uint16 reserved                         : 7;
} lte_LL1_uers_log_alpha_t;


/*! @brief uers_tones_log main struct
 */
typedef struct
{
  // Word 0
  uint32 version                          : 8;  ///< Log packet version: 141
  uint32 carrier_index                    : 4;  ///< Carrier index
  uint32 sub_frame_num                    : 4;  ///< Subframe number: 0..9
  uint32 frame_number                     :10;  ///< Frame number: 0..1023
  uint32 cxn_id                           : 1;  ///< Connection ID
  uint32 num_symbols_slot0                : 2;  ///< Symbols in slot 0: 0..2
  uint32 num_symbols_slot1                : 2;  ///< Symbols in slot 1: 0..2
  uint32 cp_mode                          : 1;  ///< 0: Normal CP, 1: Extended CP

  // Word 1
  uint32 num_uers_rbs                     : 7;  ///< Number of RBs in system bandwidth: 6..100
  uint32 is_34rx_en                       : 1;  ///< 4 Rx enable
  uint32 port_group_1_en                  : 1;  ///< Rank 3-4
  uint32 mu_mimo_en                       : 1;  ///< TM8/9 rank-1
  uint32 bmod_en                          : 1;  ///< BMOD enable
  uint32 occ4                             : 1;  ///< OCC-4
  uint32 enhanced_rnn_en                  : 1;  ///< Enhanced Rnn enabled
  uint32 prb_bundling_en                  : 1;  ///< PRB bundling enabled
  uint32 layer_sel                        : 2;  ///< OCC4 rank-1: PORT7, PORT8, PORT11, PORT13
                                                ///< OCC4 rank-2: PORT7+8, PORT11+13
                                                ///< OCC2 rank-1: PORT7, PORT8
  uint32 uers_cinr_from_theta             : 1;  ///< UERS CINR parameter based
  uint32 occ4_rank1_intf0_port            : 2;  ///< Enum: PORT7, PORT8, PORT11, PORT13
  uint32 occ4_rank1_intf1_port            : 2;  ///< Enum: PORT7, PORT8, PORT11, PORT13
  uint32 occ4_rank1_intf2_port            : 2;  ///< Enum: PORT7, PORT8, PORT11, PORT13
  uint32 tx_scheme                        : 5;  ///< Enum: lte_LL1_tx_scheme_e
  uint32 reserved1                        : 4;

  // Word 2
  uint32 delay_estimation                 : 18; ///< Delay spread estimation in us (18U13): 0..182272
  uint32 doppler_estimation               : 10; ///< Doppler estimate in Hz
  uint32 rank                             : 4;  ///< Transmission rank: 1..4

  // Word 3
  uint32 delay_index                      : 8;  ///< Delay spread bin index: 0..4
  uint32 doppler_index                    : 8;  ///< Doppler bin index: 0..5
  uint32 cinr_index                       : 8;  ///< CINR bin index: 0..5
  uint32 cinr_index_pg1                   : 8;  ///< CINR bin index (pg1): 0..5

  // Word 4
  uint32 bundling_delay_index             : 8;  ///< PRB bundling delay spread bin index: 0..5
  uint32 bundling_cinr_index              : 8;  ///< PRB bundling CINR bin index: 0..7
  uint32 bundling_cinr_index_pg1          : 8;  ///< PRB bundling CINR bin index (pg1): 0..7
  uint32 num_params                       : 4;  ///< Number of channel parameters: 6 or 8
  uint32 bundling_num_params              : 4;  ///< Number of channel parameters for PRB bundling

  // Word 5
  uint32 n1                               : 2;  ///< Number of time-constant parameters
  uint32 n2                               : 2;  ///< Number of time-linear parameters
  uint32 n3                               : 2;  ///< Number of time-quadratic parameters
  uint32 n1_pg1                           : 2;  ///< Number of time-constant parameters (pg1)
  uint32 n2_pg1                           : 2;  ///< Number of time-linear parameters (pg1)
  uint32 n3_pg1                           : 2;  ///< Number of time-quadratic parameters (pg1)
  uint32 bundling_n1                      : 3;  ///< Number of time-constant parameters for PRB bundling
  uint32 bundling_n2                      : 3;  ///< Number of time-linear parameters for PRB bundling
  uint32 bundling_n3                      : 3;  ///< Number of time-quadratic parameters for PRB bundling
  uint32 bundling_n1_pg1                  : 3;  ///< Number of time-constant parameters for PRB bundling (pg1)
  uint32 bundling_n2_pg1                  : 3;  ///< Number of time-linear parameters for PRB bundling (pg1)
  uint32 bundling_n3_pg1                  : 3;  ///< Number of time-quadratic parameters for PRB bundling (pg1)
  uint32 pg1_reset_cinr                   : 1;  ///< PG1 CINR reset from PG0
  uint32 reserved5                        : 1;

  int32  uers_timing_com                  ; ///< COM timing in taps, 32S10
  int32  uers_timing_petl                 ; ///< PETL timing in taps, 32S10
  uint16 ee_iir_alpha                     ; ///< IIR alpha for EE state, 16U15
  uint16 ee_iir_alpha_pg1                 ; ///< IIR alpha for EE pg1 state, 16U15

  int16  uers_crs_snr                     ; ///< UERS SNR (CRS-based) averaged over rx, in dB, 16S8
  int16  uers_crs_snr_pg1                 ; ///< UERS SNR (CRS-based) averaged over rx (pg1), in dB, 16S8
  int16  uers_theta_snr                   ; ///< UERS SNR (theta-based) averaged over rx, in dB, 16S8
  int16  uers_theta_snr_pg1               ; ///< UERS SNR (theta-based) averaged over rx, in dB (pg1), 16S8
  int16  crs_noise_power                  ; ///< CRS noise power, in dB, 16S8
  int16  uers_noise_power                 ; ///< UERS noise power, in dB, 16S8
  uint32 wht_bias                         ; ///< Whitening matrix diagonal loading, 32U33
  uint32 bundling_wht_bias                ; ///< Whitening matrix diagonal loading for PRB bundling, 32U33

  lte_LL1_uers_log_alpha_t alpha          ; ///< Parameter scaling for noise estimation
  lte_LL1_uers_log_alpha_t alpha_pg1_mu   ; ///< Parameter scaling for noise estimation (pg1 or MU)
  int16  beta[16]                         ; ///< Parameter scaling for signal estimation, 16S13
  int16  beta_pg1_intf2[16]               ; ///< Parameter scaling for signal estimation (pg1 or 2nd interferer), 16S13
  int16  beta_intf3[16]                   ; ///< Parameter scaling for signal estimation (3rd interferer), 16S13
  lte_LL1_uers_log_alpha_t bundling_alpha ; ///< Parameter scaling for noise estimation for PRB bundling
  lte_LL1_uers_log_alpha_t bundling_alpha_pg1 ; ///< Parameter scaling for noise estimation for PRB bundling (pg1)
  int16  bundling_beta[18]                ; ///< Parameter scaling for signal estimation for PRB bundling, 16S13
  int16  bundling_beta_pg1[18]            ; ///< Parameter scaling for signal estimation for PRB bundling (pg1), 16S13

  int16  uers_tonepwr_rx[4]               ; ///< Per rx tone power, in dB, 16S8
  int16  cinr_boost_rx[4]                 ; ///< Per rx CINR boost factor, in dB, 16S8
  int16  uers_snr_rx[4]                   ; ///< Per rx UERS SNR, CRS based, in dB, 16S8
  int16  uers_theta_snr_rx[4]             ; ///< Per rx UERS SNR, theta based, in dB, 16S8

  int16  uers_tonepwr_rx_pg1[4]           ; ///< Per rx tone power, in dB, 16S8
  int16  cinr_boost_rx_pg1[4]             ; ///< Per rx CINR boost factor, in dB, 16S8
  int16  uers_snr_rx_pg1[4]               ; ///< Per rx UERS SNR, CRS based, in dB, 16S8
  int16  uers_theta_snr_rx_pg1[4]         ; ///< Per rx UERS SNR, theta based, in dB, 16S8

}lte_LL1_log_uers_tones_ind_struct;

/*==========================================================================*/
/*! @brief
 * 1 UERS results log (Log code ID:   0xB30D)
 *  This log packet gives results related to UERS
---------------------------------------------------------------------------*/

/*! @brief uers_results_log main struct
 */
typedef struct
{
  // Word 0
  uint32 version                          : 8;  ///< Log packet version: 141
  uint32 carrier_index                    : 4;  ///< Carrier index
  uint32 sub_frame_num                    : 4;  ///< Subframe number: 0..9
  uint32 frame_number                     :10;  ///< Frame number: 0..1023
  uint32 cxn_id                           : 1;  ///< Connection ID
  uint32 reserved_0                       : 5;  ///< Reserved

  uint32 ee_pg0_rx[4]                        ;  ///< UERS energy for PG0 (4 Rx)
  uint32 ee_pg1_rx[4]                        ;  ///< UERS energy for PG1 (4 Rx)

  uint32 parameter_power_scale_factor    : 5; ///< Left shift amount for parameter_power values
  uint32 interferer_port                 : 3; ///< Detected interferer port for FD-MIMO: 0-port7, 1-port8, 2-port11, 3-port13, 4-invalid
  uint32 mu_mimo_ind                     : 3; ///< 0: no_indication 1 : mu_mimo_indication 2: invalid
  uint32 reserved7                       : 21;

  /* Parameter power logging */
  uint16 uers_parameter_power_result_rx0[18];     ///< Parameter energy for rx0
  uint16 uers_parameter_power_result_rx1[18];     ///< Parameter energy for rx1
  uint16 uers_parameter_power_result_rx2[18];     ///< Parameter energy for rx2
  uint16 uers_parameter_power_result_rx3[18];     ///< Parameter energy for rx3
  uint16 uers_parameter_power_result_rx0_pg1[18]; ///< Parameter energy for rx0 (pg1)
  uint16 uers_parameter_power_result_rx1_pg1[18]; ///< Parameter energy for rx1 (pg1)
  uint16 uers_parameter_power_result_rx2_pg1[18]; ///< Parameter energy for rx2 (pg1)
  uint16 uers_parameter_power_result_rx3_pg1[18]; ///< Parameter energy for rx3 (pg1)

  /* MU-MIMO detection result logging */
  uint8  mu_mimo_num_rb_port7            ;    ///< Number of RB with port 7 interferer detected
  uint8  mu_mimo_num_rb_port8            ;    ///< Number of RB with port 8 interferer detected
  uint8  mu_mimo_num_rb_port11           ;    ///< Number of RB with port 11 interferer detected
  uint8  mu_mimo_num_rb_port13           ;    ///< Number of RB with port 13 interferer detected
  uint16 mu_mimo_port7_power             ;    ///< Port-7 detected power, in dB, 16S8
  uint16 mu_mimo_port8_power             ;    ///< Port-8 detected power, in dB, 16S8
  uint16 mu_mimo_port11_power            ;    ///< Port-11 detected power, in dB, 16S8
  uint16 mu_mimo_port13_power            ;    ///< Port-13 detected power, in dB, 16S8
  uint32 uers_mu_mimo_detection_result[13];   ///< Four bits per RB with MU detection result
                                              ///< b0000 - No interferer detected
                                              ///< b0001 - PORT7 interferer detected
                                              ///< b0010 - PORT8 interferer detected
                                              ///< b0100 - PORT11 interferer detected
                                              ///< b1000 - PORT13 interferer detected
  uint32 mu_mimo_det_result[4];               ///< Bit-map of RBs with interferer detected */

  uint8  bmod_num_rb_qpsk               ;     ///< qpsk RB stats
  uint8  bmod_num_rb_16qam              ;     ///< 16QAM RB stats
  uint8  bmod_num_rb_64qam              ;     ///< 64QAM RB stats
  uint8  bmod_num_rb_256qam             ;     ///< 256QAM RB stats
  uint8  bmod_num_rb_1024qam            ;     ///< 1024QAM RB stats
  uint8  bmod_num_rb_lmmse              ;     ///< MMSE RB  stats
  uint8  bmod_num_rb_mrc                ;     ///< MRC RB stats

}lte_LL1_log_uers_results_ind_struct;

#endif //LTE_LL1_LOG_UERS_H
