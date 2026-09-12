/*!
  @file
  lte_LL1_log_demfront.h

  @brief
  demfront log packet data structures

  @detail
  !!! Please do not modify !!!
  This file was auto-generated on Mon May 17 12:54:15 2010.
  Command line used: perl w2h.pl log_demfront demfront 1 1 1 1 1 1 1 1 1 1 1
   Note: The params, [1 1 1 1 1 1 1 1 1 1 1], correspond to version numbers for
         each log message in the listed order in the ICD

*/

/*===========================================================================

  Copyright (c) 2009 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/lte_LL1_log_demfront.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/
#ifndef LTE_LL1_LOG_DEMFRONT_H
#define LTE_LL1_LOG_DEMFRONT_H



/*==========================================================================*/
/*! @brief
 * 1 LTE LL1 Serving Cell CIR (Log code ID: 0xB121) - Internal
 *
 *  This log packet gives important parameters related to the serving cell 
 *  channel impulse response (CIR). The log record is generated once every k*50 
 *  milliseconds in round robin fashion for each valid Tx-Rx pair, where 'k' is 
 *  configurable. 
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_DEMFRONT_SERVING_CELL_CIR_VERSION 21

/*! @brief serving_cell_cir main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version. Range: 0...255.
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 serving_cell_id                  : 9;  ///< Range: 0...503
  uint32 sub_frame_number                 : 4;  ///< Sub-frame number (Range 0..9)
  uint32 system_frame_number              : 10;  ///< System frame number (range 0..1023)
  uint32 reserved0                        : 1; 
  uint32 cir_size_p                       : 3;  ///< Encoded as:
                                                         ///< 0x0: 32
                                                         ///< 0x1: 64
                                                         ///< 0x2: 128
                                                         ///< 0x3: 256
                                                         ///< 0x5: 512
                                                         ///< 0x6-0x7: Reserved
  uint32 tx_antenna_number                : 2;  ///< 0~3
  uint32 rx_antenna_number                : 1;  ///< Encoded as:
                                                         ///< 0x0: antenna 0
                                                         ///< 0x1: antenna 1

  uint32 reserved1                        : 22; 
  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC 
  uint32 cir_per_rx_tx_pair_pcfl[384]                 ;  ///< Packed array, each item is 24 bits
                                                         ///< Format 24PCFL:
                                                         ///< Bit 0-9: Mantissa for imaginary value
                                                         ///< Bit 10-19: Mantissa for real value
                                                         ///< Bit 20-23: Exponent
}lte_LL1_log_demfront_serving_cell_cir_ind_struct;



/*==========================================================================*/
/*! @brief
 * 2 LTE LL1 Serving Cell RS (Log code ID: 0xB134) - Internal
 *
 *  This log packet gives important parameters related to the serving cell 
 *  channel estimation and interference estimation based on Reference Signals 
 *  (RS). The log record is generated once every DL enabled subframe. 
---------------------------------------------------------------------------*/
#define WB_MAX_CRS_SYMS_PER_SF 6

#define LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT_CER 2 //CER  log all based on it

#ifdef LTE_LL1_FEATURE_THOR_HORXD_LOG_B134
  //should use new version number
  #define LTE_LL1_LOG_DEMFRONT_SERVING_CELL_RS_RECORDS  10
  #define LTE_LL1_LOG_DEMFRONT_SERVING_CELL_RS_VERSION  143 //129 for per rx change
  #define LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT  LTE_LL1_MAX_NUM_UE_RX_ANT
#else
  #define LTE_LL1_LOG_DEMFRONT_SERVING_CELL_RS_RECORDS  20
  #define LTE_LL1_LOG_DEMFRONT_SERVING_CELL_RS_VERSION  161
  #define LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT 4  // Corresponds to HE, as the QXDM structure version 143 is defined by HE Interface
  #define LTE_LL1_DF_LOG_INVALID_UE_RX_ANT LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT  // Invalid rx antenna for QXDM parsing.
#endif

/*! @brief lte_LL1_log_demfront_serving_cell_rs_2_RX_records struct
 */
#define RX_ANT_2 (LTE_LL1_UE_RX_ANT_1+1)
#define LTE_LL1_LOG_DEMFRONT_NUM_BITS_PER_RX_IDX  3

typedef struct
{
  /* Word 0 */
  uint16 offset_system_frame_number           : 4;  ///< Range 0..9
  uint16 sub_frame_number                     : 4;  ///< Range 0..9
  uint8  grant_config_mode                    : 8; /// 0: LFW_NO_GRANT, 1: LFW_GRANT_CRS, 2: LFW_GRANT_UERS, 3: LFW_GRANT_UERS_SIB_PAGE
  uint16 raw_crs_tx01                         : 4;  ///< tx01 raw CRS number
  uint16 raw_crs_tx23                         : 4;  ///< tx23 raw CRS number
  uint16 extra_crs_tx01                       : 4;  ///< tx01 extrapolted CRS number
  uint16 extra_crs_tx23                       : 4;  ///< tx23 extrapolted CRS number

  /* Word 1 */
  uint32 reset_type                           : 16;  ///< 0 -> no reset   1 -> coherent recovery 2 -> soft reset
                                                    ///< 3 -> com reslam 4 -> ant switch        5 -> hard reset
  uint32 presib2_type                         : 2;  ///< 0 -> not in presib2 1 -> in presib2 known subframe type
                                                    ///< 2 -> in presib2 potential MBSFN subframe
  uint32 phy_to_logical_map                   : 12; ///< 3 bits for each Rx, Rx0 at BIT 0-2
                                                    ///< and Rx1 at 3-5 and so on..
                                                    ///< Printed in the packet as {Rx0, Rx1, Rx2, Rx3}
  uint32 reserved4                            : 2;

  uint8 sleep_time_ms[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT];
  uint8 tau[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT];      ///< Tau Value for Individual Rx. IT has value from 0 to 64. It is significant only if Resume/Reset bit is set for particular Rx.
                                                    ///<Tau Value is set as 64 for the case of Reset.
  uint8 kalman_gain_update_en[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT];  ///<bit0: CRS0 ... bit5: CRS5

  /* Word 2-3 */
  uint16 snr_est[RX_ANT_2]; ///< SNR in db format. 16U8 format.

  /* Word 4 */
  uint16 crs_cir_alpha_se_sne_shift[RX_ANT_2];             ///< non-coherent filtering coef shift; per Rx.
  /* Word 5~10 */
  uint16 kalman_iir_alpha1[RX_ANT_2][WB_MAX_CRS_SYMS_PER_SF];  ///< [rx][crs_sym]. 16U15 format

  uint8  alpha_noise_pwr_kalman_gain_shift[RX_ANT_2][WB_MAX_CRS_SYMS_PER_SF]; ///< [rx][crs_sym], instant noise filtering coef shift
                                                                                                      ///< used for KG computation in power saving mode

  uint16 Noise_estimate[RX_ANT_2][4]; ///< [rx][tx], noise power estimate for each rx-tx pair in db (16S8 format)

  uint32 ncf;                                       ///< NCF value (32U16)
  uint32 demod_cov_shift_adj;                       ///< Demod Rnn filtering bias adjustment due to NCF (8S0)
  uint8 demod_raw_cov_iir_alpha[4];                 ///< Demod Rnn filtering alpha for CRS symbols 0-3 (8S6)
  uint32 csf_cov_shift_adj;                         ///< CSF Rnn filtering bias adjustment due to NCF (8S0)
  uint8 csf_raw_cov_iir_alpha[4];                   ///< CSF Rnn filtering alpha for CRS symbols 0-3 (8S6)

  /* For CRS-IC logging*/
  uint32 colliding_crs_tx0                    : 1;  ///< tx0 CRS collide within the 1+3 cells
  uint32 colliding_crs_tx1                    : 1;  ///< tx1 CRS collide within the 1+3 cells
  uint32 colliding_crs_tx2                    : 1;  ///< tx2 CRS collide within the 1+3 cells
  uint32 colliding_crs_tx3                    : 1;  ///< tx3 CRS collide within the 1+3 cells
  uint32 enable_blind_detection               : 1;  ///< 1 -> blind port detection enabled
  uint32 reserved1                            : 11;
  uint8  Blind_port_detect_results[WB_MAX_CRS_SYMS_PER_SF];  ///< 6 crs symbols, combined over rx

} lte_LL1_log_demfront_serving_cell_rs_records_2_RX_s;

/*! @brief lte_LL1_log_demfront_serving_cell_rs_records struct
 */
typedef struct
{
  /* Word 0 */
  uint16 offset_system_frame_number           : 4;  ///< Range 0..9
  uint16 sub_frame_number                     : 4;  ///< Range 0..9
  uint8  grant_config_mode                    : 8; /// 0: LFW_NO_GRANT, 1: LFW_GRANT_CRS, 2: LFW_GRANT_UERS, 3: LFW_GRANT_UERS_SIB_PAGE
  uint16 raw_crs_tx01                         : 4;  ///< tx01 raw CRS number
  uint16 raw_crs_tx23                         : 4;  ///< tx23 raw CRS number
  uint16 extra_crs_tx01                       : 4;  ///< tx01 extrapolted CRS number
  uint16 extra_crs_tx23                       : 4;  ///< tx23 extrapolted CRS number

  /* Word 1 */
  uint32 reset_type                           : 16;  ///< 0 -> no reset   1 -> coherent recovery 2 -> soft reset
                                                    ///< 3 -> com reslam 4 -> ant switch        5 -> hard reset
  uint32 presib2_type                         : 2;  ///< 0 -> not in presib2 1 -> in presib2 known subframe type
                                                    ///< 2 -> in presib2 potential MBSFN subframe
  uint32 phy_to_logical_map                   : 12; ///< 3 bits for each Rx, Rx0 at BIT 0-2
                                                    ///< and Rx1 at 3-5 and so on..
                                                    ///< Printed in the packet as {Rx0, Rx1, Rx2, Rx3}
  uint32 reserved4                            : 2;

  uint8 sleep_time_ms[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT];

  uint8 tau[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT];  ///< Tau Value for Individual Rx. IT has value from 0 to 64. It is significant only if Resume/Reset bit is set for particular Rx.
                                                    ///<Tau Value is set as 64 for the case of Reset.
  /* Word */
  uint8 kalman_gain_update_en[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT];  ///<bit0: CRS0 ... bit5: CRS5

  /* Word 2-3 */
  uint16 snr_est[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT]; ///< SNR in db format. 16U8 format.

  /* Word 4 */
  uint16 crs_cir_alpha_se_sne_shift[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT];             ///< non-coherent filtering coef shift; per Rx.
  /* Word 5~10 */
  uint16 kalman_iir_alpha1[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT][WB_MAX_CRS_SYMS_PER_SF];  ///< [rx][crs_sym]. 16U15 format

  uint8  alpha_noise_pwr_kalman_gain_shift[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT][WB_MAX_CRS_SYMS_PER_SF]; ///< [rx][crs_sym], instant noise filtering coef shift
                                                                                                      ///< used for KG computation in power saving mode

  uint16 Noise_estimate[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT][4]; ///< [rx][tx], noise power estimate for each rx-tx pair in db (16S8 format)

  uint32 ncf;                                       ///< NCF value (32U16)
  uint32 demod_cov_shift_adj;                       ///< Demod Rnn filtering bias adjustment due to NCF (8S0)
  uint8 demod_raw_cov_iir_alpha[4];                 ///< Demod Rnn filtering alpha for CRS symbols 0-3 (8S6)
  uint32 csf_cov_shift_adj;                         ///< CSF Rnn filtering bias adjustment due to NCF (8S0)
  uint8 csf_raw_cov_iir_alpha[4];                   ///< CSF Rnn filtering alpha for CRS symbols 0-3 (8S6)

  /* For CRS-IC logging*/
  uint32 colliding_crs_tx0                    : 1;  ///< tx0 CRS collide within the 1+3 cells
  uint32 colliding_crs_tx1                    : 1;  ///< tx1 CRS collide within the 1+3 cells
  uint32 colliding_crs_tx2                    : 1;  ///< tx2 CRS collide within the 1+3 cells
  uint32 colliding_crs_tx3                    : 1;  ///< tx3 CRS collide within the 1+3 cells
  uint32 enable_blind_detection               : 1;  ///< 1 -> blind port detection enabled
  uint32 reserved1                            : 11;
  uint8  Blind_port_detect_results[WB_MAX_CRS_SYMS_PER_SF];  ///< 6 crs symbols, combined over rx

} lte_LL1_log_demfront_serving_cell_rs_records_s;

typedef union lte_LL1_log_demfront_serving_cell_rs_records
{
  lte_LL1_log_demfront_serving_cell_rs_records_2_RX_s  RX_2[ LTE_LL1_LOG_DEMFRONT_SERVING_CELL_RS_RECORDS ];
  lte_LL1_log_demfront_serving_cell_rs_records_s       RX_4[ LTE_LL1_LOG_DEMFRONT_SERVING_CELL_RS_RECORDS ];
}lte_LL1_log_demfront_serving_cell_rs_records_u;

/*! @brief serving_cell_rs main struct
 */
typedef struct
{
  //WORD 0
  uint32 version                                      : 8;    //< Log packet version. Range: 0...255.
                                                              //< Version = 1 for the log packet structure
                                                              //< described below
  uint32 carrier_index                                : 4;    //< 0-PCC, 1-SCC 
  uint32 Num_cell                                     : 4;
  uint32 cell_id                                      : 9;    //< Range: 0...503, it corresponds to IC cell 
  uint32 noise_estimation_sync_async_deployment_mode  : 1;    //< Encoded as:
                                                              //< 0x0: DEPLOYMENT_MODE_ASYNC
                                                              //< 0x1: DEPLOYMENT_MODE_SYNC
  uint32 nTx                                          : 2;   
  uint32 cir_size_p                                   : 3;   
   
  
  uint32 noise_estimation_fd_average_pattern          : 1;    ///< Encoded as:
                                                              //< 0x0: FD_RB_AVG = 2 RBs
                                                              //< 0x1: FD_RB_AVG = 4 RBs
 // WORD 2-3
  uint16 cell_id_list[ 1+3 ];          // On HE, maximum 3 IC cells per carrier + serving cell, 
                                        // since we are using the same QXDM version for HA, we should retain the same size in QXDM interface file
 
 // WORD 4
//#ifdef LTE_LL1_FEATURE_THOR_HORXD_LOG_B134
  uint32 nRx                                          : 3;    // Support up to 4 Rx
//#else
//  uint32 nRx                                          : 2;    // Support up to 2 Rx
//#endif

  uint32 records_rs_size                              : 5;    // total of SF Range: 1..20   
  uint32 doppler_in_hz                                :14;    // 0 - 16383
  uint32 starting_system_frame_number                 :10;    // 0 - 1023
                                                    
//#ifndef LTE_LL1_FEATURE_THOR_HORXD_LOG_B134
//  uint32 reserved1                                    :1;
//#endif

 //WORD 5
  uint32  corr_est_idx                                : 3; //range 0..6
  uint32  rxcorr_idx                                  : 2; //range 0,1,2
  uint32  txcorr_idx                                  : 1; //range 0,1
  uint32  reserved2                                   : 26;
  lte_LL1_log_demfront_serving_cell_rs_records_u records;

}lte_LL1_log_demfront_serving_cell_rs_ind_struct;

/*==========================================================================*/
/*! @brief
 * 3 LTE LL1 Serving Cell Window Gain (Log code ID: 0xB135) - Internal
 *
 *  This log packet gives important parameters related to the serving cell 
 *  channel estimation based on Reference Signals (RS). The log record is 
 *  generated once every k*50 DL enabled subframes, where 'k' is configurable.
 *  Note: In VPE for DIME and no longer available to FW. May request CR to VPE team
 *  pending resolution of whitened matrices.
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_DEMFRONT_SERVING_CELL_WINDOW_GAIN_VERSION 21
#define LTE_LL1_LOG_DEMFRONT_WINDOW_GAIN_NUMBER_OF_RECORDS 128

/*! @brief serving_cell_window_gain window_gain_number_of_records struct
 */
typedef struct
{
  uint16 window_gain_for_4_taps          ; 
}lte_LL1_log_demfront_serving_cell_window_gain_window_gain_records_s;


/*! @brief serving_cell_window_gain main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version. Range: 0...255.
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 serving_cell_id                  : 9;  ///< Range: 0...503
  uint32 sub_frame_number                 : 4;  ///< Sub-frame number (Range 0..9)
  uint32 system_frame_number              : 10;  ///< System frame number (range 0..1023)
  uint32 rx_antenna_number                : 1; 
  uint32 cir_size                         : 3;  ///< Encoded as:
                                                         ///< 0x0: 32
                                                         ///< 0x1: 64
                                                         ///< 0x2: 128
                                                         ///< 0x3: 256
                                                         ///< 0x5: 512
                                                         ///< 0x6-0x7: Reserved
  uint32 reserved0                        : 17; 
  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC 
  uint32 window_gain_number_of_records    : 8;  ///< 1..128 CIR Size div 4
  lte_LL1_log_demfront_serving_cell_window_gain_window_gain_records_s lte_LL1_log_demfront_serving_cell_window_gain_window_gain_records[LTE_LL1_LOG_DEMFRONT_WINDOW_GAIN_NUMBER_OF_RECORDS];
}lte_LL1_log_demfront_serving_cell_window_gain_ind_struct;



/*==========================================================================*/
/*! @brief
 * 4 LTE LL1 Serving Cell CER (Log code ID: 0xB122)
 *
 *  This log packet gives important parameters related to the serving cell 
 *  channel energy response (CER). The log record is generated once every k*50 
 *  milliseconds in round robin fashion for each valid Tx-Rx pair, where 'k' is 
 *  configurable. 
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_DEMFRONT_SERVING_CELL_CER_VERSION 141
#define LTE_LL1_LOG_DEMFRONT_SCELL_NUMBER_OF_RECORDS_CER_SIZE 512
#define LTE_LL1_LOG_DEMFRONT_SCELL_NUMBER_OF_RECORDS_HALF_CER_SIZE 256

/*! @brief serving_cell_cer scell_number_of_records_cer_size struct
 */
#if 0
typedef struct
{
  uint16 cer_per_rx_tx_pair              ;  ///< 16 bit unsigned
}lte_LL1_log_demfront_serving_cell_cer_scell_records_cer_size_s;
#else
typedef struct
{
  uint16 cer_per_rx_tx_pair[LTE_LL1_DF_LOG_MAX_NUM_UE_RX_ANT_CER];  //< 16 bit unsigned
} lte_LL1_log_demfront_cell_cer_records_s;
#endif

/*! @brief serving_cell_cer main struct
 */
#if 0
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version. Range: 0...255.
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 serving_cell_id                  : 9;  ///< Range: 0...503
  uint32 sub_frame_number                 : 4;  ///< Sub-frame number (Range 0..9)
  uint32 system_frame_number              : 10;  ///< System frame number (range 0..1023)
  uint32 reserved0                        : 1; 
  uint32 cer_size_p                       : 3;  ///< Encoded as:
                                                         ///< 0x0: 32
                                                         ///< 0x1: 64
                                                         ///< 0x2: 128
                                                         ///< 0x3: 256
                                                         ///< 0x5: 512
                                                         ///< 0x6-0x7: Reserved
  uint32 rx_antenna_number                : 1;  ///< Encoded as:
                                                         ///< 0x0: antenna 0
                                                         ///< 0x1: antenna 1
  uint32 energy_type                      : 1;  ///< Encoded as:
                                                         ///< 0x0: signal energy
                                                         ///< 0x1: signal + noise energy
  uint32 configuration                    : 4; 
  uint32 reserved1                        : 9;
  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC  
  uint32 scell_number_of_records_cer_size : 10;  ///< Range: 1..512
  lte_LL1_log_demfront_serving_cell_cer_scell_records_cer_size_s lte_LL1_log_demfront_serving_cell_cer_scell_records_cer_size[LTE_LL1_LOG_DEMFRONT_SCELL_NUMBER_OF_RECORDS_CER_SIZE];
}lte_LL1_log_demfront_serving_cell_cer_ind_struct;

#else

typedef struct
{
  uint32 version                          : 8; ///< Log packet version. Range: 0...255.
                                               ///< Version = 1 for the log packet structure
                                               ///< described below
  uint32 starting_system_frame_number     :10; ///< System frame number (range 0..1023)
  uint32 sub_frame_number                 : 4; ///< Sub-frame number (Range 0..9)
  uint32 cell_id                          : 9; ///< Range: 0...503, it corresponds to servecell and IC cell
  uint32 serving_cell                     : 1; /* True-- serving cell, False-- IC cell (internal) */

  uint32 cell_order                       : 2; /* order in CRS-IC list range of {0,1.2.3} (internal) */
  uint32 IC_enable                        : 1; /* set to (0)N/A when CRS-IC is not enabled (internal) */
  uint32 rx_pair                          : 1; /* 0: rx01,  1: rx23 */
  uint32 configuration                    : 4; /* (internal) */
  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC
  uint32 exp_se_rx0                       : 5;
  uint32 exp_se_rx1                       : 5;  ///< exponet of SE/SNE (2 Rx) used in 32to16 format conversion
                                                ///< need only 5 bits, no Q-format. Range is [0, 16].
                                                ///< The bigger the number, the smaller the real SE/SNE
  uint32 number_of_records                : 10;  //Antenna 0 Number of Record  Range: 1..256

  uint32 rx0_ref_observ_noise_pwr_shift;
  uint32 rx1_ref_observ_noise_pwr_shift;

  uint32 unwrap_bmap_rx0[8];//LVFW_UMWARP_BMAP_LENGTH/32
  uint32 unwrap_bmap_rx1[8];

  lte_LL1_log_demfront_cell_cer_records_s
    lte_LL1_log_demfront_cell_cer_records[LTE_LL1_LOG_DEMFRONT_SCELL_NUMBER_OF_RECORDS_HALF_CER_SIZE];

} lte_LL1_log_demfront_serving_cell_cer_ind_struct;
#endif

/*==========================================================================*/
/*! @brief
 * 5 LTE LL1 Neighbor Cell CER (Log code ID: 0xB123)
 *
 *  This log packet gives important parameters related to the neighbor cell 
 *  Channel Energy Response (CER). The log record is generated event based. 
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_DEMFRONT_NEIGHBOR_CELL_CER_VERSION 1
#define LTE_LL1_LOG_DEMFRONT_NUMBER_OF_NCELL_TAPS_FIXED 32
#define LTE_LL1_LOG_DEMFRONT_NCELL_TAPS_SUM_NUMBER_OF_RECORDS_M_x_N 8

/*! @brief neighbor_cell_cer number_of_ncell_taps_fixed struct
 */
typedef struct
{
  uint16 per_tap_cer_for_rx0             ;  ///< Unitless energy metric. Range: 0-65535
  uint16 per_tap_cer_for_rx1             ;  ///< Unitless energy metric. Range: 0-65535
}lte_LL1_log_demfront_neighbor_cell_cer_ncell_taps_s;


/*! @brief neighbor_cell_cer ncell_taps_sum_number_of_records_m_x_n struct
 */
typedef struct
{
  uint16 cer_per_rx_tx_pair              ;  ///< Unitless energy metric. Range: 0-65535
}lte_LL1_log_demfront_neighbor_cell_cer_ncell_taps_sum_records_m_x_n_s;


/*! @brief neighbor_cell_cer main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version. Range: 0...255.
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 neighbor_cell_id                 : 9;  ///< Range: 0...503
  uint32 sub_frame_number                 : 4;  ///< Sub-frame number (Range 0..9)
  uint32 reserved0                        : 1; 
  uint32 system_frame_number              : 10;  ///< System frame number (range 0..1023)
  lte_LL1_log_demfront_neighbor_cell_cer_ncell_taps_s lte_LL1_log_demfront_neighbor_cell_cer_ncell_taps[LTE_LL1_LOG_DEMFRONT_NUMBER_OF_NCELL_TAPS_FIXED];
  uint32 reserved1                        : 16; 
  uint32 number_of_tx_antennas_m          : 2;  ///< Encoded as:
                                                         ///< 0x0: 1 antenna
                                                         ///< 0x1: 2 antennas
                                                         ///< 0x2: 4 antennas
  uint32 number_of_rx_antennas_n          : 1;  ///< Encoded as:
                                                         ///< 0x0: 1 antenna
                                                         ///< 0x1: 2 antennas
  uint32 reserved2                        : 5; 
  uint32 ncell_taps_sum_number_of_records_m_x_n : 8;  ///< Range: 1-8 (5 bit value)
                                                         ///< 3 bit (MSB) reserved
  lte_LL1_log_demfront_neighbor_cell_cer_ncell_taps_sum_records_m_x_n_s lte_LL1_log_demfront_neighbor_cell_cer_ncell_taps_sum_records_m_x_n[LTE_LL1_LOG_DEMFRONT_NCELL_TAPS_SUM_NUMBER_OF_RECORDS_M_x_N];
}lte_LL1_log_demfront_neighbor_cell_cer_ind_struct;



/*==========================================================================*/
/*! @brief
 * 6 LTE LL1 Serving Cell Interf Estimation (Log code ID: 0xB124) - Internal
 *
 *  This log packet gives important parameters related to the serving cell 
 *  interference estimation results. The log record is generated once every 
 *  k*50 milliseconds, where 'k' is configurable. 
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_DEMFRONT_SERVING_CELL_INTERF_ESTIMATION_VERSION 21
#define LTE_LL1_LOG_DEMFRONT_SCELL_INTERF_NUMBER_OF_RECORDS 55

/*! @brief serving_cell_interf_estimation scell_interf_number_of_records struct
 */
typedef struct
{
  uint64 whitening_matrix                ;  ///< Format:
                                                         ///< Bit 0-13: Imaginary value of off-diagonal term
                                                         ///< with 11 fractional bits
                                                         ///< Bit 14-27: Real value of off-diagonal term with
                                                         ///< 11 fractional bits
                                                         ///< Bit 28-40: Unsigned element with 11 fractional
                                                         ///< bits (2,2) element
                                                         ///< Bit 41-53: Unsigned element with 11 fractional
                                                         ///< bits (1,1) element
                                                         ///< Bit 54: 0: Matrix is upper triangular
                                                         ///< 1: Matrix is lower triangular
                                                         ///< Other bits reserved
}lte_LL1_log_demfront_serving_cell_interf_estimation_scell_interf_records_s;


/*! @brief serving_cell_interf_estimation main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version. Range: 0...255.
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 serving_cell_id                  : 9;  ///< Range: 0...503
  uint32 sub_frame_number                 : 4;  ///< Sub-frame number (Range 0..9)
  uint32 system_frame_number              : 10;  ///< System frame number (range 0..1023)
  uint32 sample_server                    : 1;  ///< 0: Demod
                                                         ///< 1: Searcher
  uint32 reserved0                        : 22; 
  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC 
  uint32 scell_interf_number_of_records   : 6;  ///< Range: 3 to 55. Other values reserved.
                                                         ///< one record for every 2 resource blocks
  lte_LL1_log_demfront_serving_cell_interf_estimation_scell_interf_records_s lte_LL1_log_demfront_serving_cell_interf_estimation_scell_interf_records[LTE_LL1_LOG_DEMFRONT_SCELL_INTERF_NUMBER_OF_RECORDS];
}lte_LL1_log_demfront_serving_cell_interf_estimation_ind_struct;

/*==========================================================================*/
/*! @brief
 * 8 LTE LL1 PDSCH Demapper Configuration (Log code ID: 0xB126)
 *
 *  This log packet gives important parameters related to the serving cell 
 *  Demodulation configuration parameters. The log record is generated once 
 *  every time a PDSCH needs to be demodulated. 
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_DEMFRONT_PDSCH_DEMAPPER_CONFIGURATION_VERSION 23

/*! @brief pdsch_demapper_configuration main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version. Range: 0...255.
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 serving_cell_id                  : 9;  ///< Range: 0...503
  uint32 sub_frame_number                 : 4;  ///< Sub-frame number (Range 0..9)
  uint32 system_frame_number              : 10;  ///< System frame number (range 0..1023)
  uint32 reserved0                        : 1; 
  uint32 pdsch_rnti_id                    : 16;  ///< Describes the RNTI ID
  uint32 pdsch_rnti_type                  : 4;  ///< RNTI type
                                                         ///< 0: C-RNTI
                                                         ///< 1: SPS C-RNTI
                                                         ///< 2: P-RNTI
                                                         ///< 3: RA-RNTI
                                                         ///< 4: Temporary C-RNTI
                                                         ///< 5: SI-RNTI
                                                         ///< 6: Reserved
                                                         ///< 7: Reserved
                                                         ///< 8: MBMS-RNTI
                                                         ///< Other values reserved
  uint32 receiver_type_for_stream_0       : 1;  ///< 0: Minimum Mean Square Error (MMSE)
                                                         ///< 1: Maximum Likelihood (ML)
  uint32 receiver_type_for_stream_1       : 1;  ///< 0: Minimum Mean Square Error (MMSE)
                                                         ///< 1: Maximum Likelihood (ML)
  uint32 sic_enable                       : 1;  ///< 0: Enabled
                                                         ///< 1: Disabled
  uint32 sic_ordering                     : 1;  ///< If SIC is enabled:
                                                         ///< 0: Layer 0 followed by Layer 1
                                                         ///< 1: Layer 1 followed by Layer 0
                                                         ///< If SIC is not enabled:
                                                         ///< 0: Code Word 0 followed by Code Word 1
                                                         ///< 1: Code Word 1 followed by Code Word 0
  uint32 number_of_tx_antennas_m          : 2;  ///< Encoded as:
                                                         ///< 0x0: 1 antenna
                                                         ///< 0x1: 2 antennas
                                                         ///< 0x2: 4 antennas
  uint32 number_of_rx_antennas_n          : 1;  ///< Encoded as:
                                                         ///< 0x0: 1 antenna
                                                         ///< 0x1: 2 antennas

  uint32 reserved1                        : 3;

  uint32 spatial_rank                     : 2;  ///< Range: 0..3
                                                         ///< 0: rank 1
                                                         ///< 1: rank 2
                                                         ///< 2: rank 3 (reserved)
                                                         ///< 3: rank 4 (reserved)
  uint64 rb_allocation_slot_0[2]                      ;  ///< Bitmask that tells which RB is allocated in slot
                                                         ///< 0 (110 bits, "1" means allocated)
                                                         ///< word1: bits 0..63
                                                         ///< word2: bits 64..110)
  uint64 rb_allocation_slot_1[2]                      ;  ///< Bitmask that tells which RB is allocated in slot
                                                         ///< 0 (110 bits, "1" means allocated)
                                                         ///< word1: bits 0..63
                                                         ///< word2: bits 64..110
  uint32 rank_2_single_codeword_enable    : 1;  ///< 0: Enabled
                                                         ///< 1: Disabled
  uint32 frequency_selective_precoding_matrix : 1;  ///< 0: Wideband
                                                         ///< 1: Subband
  uint32 reserved2                        : 2; 
  uint32 pmi_index                        : 4;  ///< Range: 0..15

  uint32 transmission_scheme              : 4;  ///< Range: 0...9
                                                         ///< 0: Reserved
                                                         ///< 1: Single-antenna port; port 0
                                                         ///< 2: Transmit diversity
                                                         ///< 3: Open-loop spatial multiplexing
                                                         ///< 4: Closed-loop spatial multiplexing
                                                         ///< 5: Multi-user MIMO
                                                         ///< 6: Closed-loop Rank=1 precoding
                                                         ///< 7: Single-antenna port; port 5
                                                         ///< 8: single-antenna port: port 7
                                                         ///< 9: dual layer beamforming: port 78
  uint32 port78_en                        : 2;  ///< Range: 0...9
                                                ///< 0: reserved
                                                ///< 1: port 7 only
                                                ///< 2: port 8 only
                                                ///< 3: port 7 and 8
                            
  uint32 reserved3                        : 18;           

  uint32 transport_block_size_for_stream_0 : 17;  ///< Unit: number of bits. Range: 0...75376
  uint32 modulation_for_stream_0          : 2;  ///< 0x0: QPSK
                                                         ///< 0x1: 16QAM
                                                         ///< 0x2: 64QAM
  uint32 traffic_to_pilot_ratio           : 12;  ///< In units of 1/256 from 0 to 15
                                                         ///< 0x0: 0 ... 0xF01: 15.000
  uint32 reserved4                        : 1; 
  uint32 transport_block_size_for_stream_1 : 17;  ///< Unit: number of bits. Range: 0...75376
  uint32 modulation_for_stream_1          : 2;  ///< 0x0: QPSK
                                                         ///< 0x1: 16QAM
                                                         ///< 0x2: 64QAM
  uint32 reserved5                        : 4;  //removed  number_of_pdsch_tm_tasks and source_for tpr.
                                                         ///< 0x1: Calculated
  uint32 pb                               : 2;  ///< If number of TX antennas is equal to 1:
                                                         ///< 0x0: PB = 0; RhoB/RhoA = 1
                                                         ///< 0x1: PB = 1; RhoB/RhoA = 4/5
                                                         ///< 0x2: PB = 2; RhoB/RhoA = 3/5
                                                         ///< 0x3: PB = 1; RhoB/RhoA = 2/5
                                                         ///< If number of TX antennas is equal to 2 or 4:
                                                         ///< 0x0: PB = 0; RhoB/RhoA = 5/4
                                                         ///< 0x1: PB = 1; RhoB/RhoA = 1
                                                         ///< 0x2: PB = 2; RhoB/RhoA = 3/4
                                                         ///< 0x3: PB = 1; RhoB/RhoA = 1/2
  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC 
  uint32 csi_rs_exist                     : 1; ///< 0: NO CSI-RS
  uint32 zp_csi_rs_exist                  : 1; ///< 0: NO ZP CSI-RS
  uint32 csi_rs_sym_skip                  : 1; ///< 1: symbol(s) are skipped due to CSI-RS
}lte_LL1_log_demfront_pdsch_demapper_configuration_ind_struct;

/*==========================================================================*/
/*! @brief
 * 10 LTE LL1 UERS Channel Estimation (Log code ID: 0xB128) - Internal
 *
 *  This log packet gives important parameters related to the serving cell 
 *  channel estimation results in TxMode7. The log record is generated once 
 *  every 'TBD' milliseconds.
 *  Note: UERS tone log (0xB14F) is a super set of it. Will be removed in DIME.
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_DEMFRONT_UERS_CHANNEL_ESTIMATION_VERSION 1
#define LTE_LL1_LOG_DEMFRONT_UERS_CE_NUMBER_OF_RECORDS 110

/*! @brief uers_channel_estimation uers_ce_number_of_records struct
 */
typedef struct
{
  uint32 r11_real_diagonal                : 13;  ///< Unitless energy metric. Range: 0-8191
  uint32 r22_real_diagonal                : 13;  ///< Unitless energy metric. Range: 0-8191
  uint32 reserved0                        : 6; 
  uint32 r21_real_part_of_cross_diagonal  : 14;  ///< Unitless energy metric. Range: 0-16383
  uint32 r21_imag_part_of_cross_diagonal  : 14;  ///< Unitless energy metric. Range: 0-16383
  uint32 lu_or_ul                         : 1;  ///< If R(0,0) > R(1,1), LU else UL Cholesky
                                                         ///< decomposition is chosen for square root
                                                         ///< computation
  uint32 reserved1                        : 3; 
}lte_LL1_log_demfront_uers_channel_estimation_uers_ce_records_s;


/*! @brief uers_channel_estimation main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version. Range: 0...255.
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 serving_cell_id                  : 9;  ///< Range: 0...503
  uint32 reserved0                        : 8; 
  uint32 uers_ce_number_of_records        : 7;  ///< Range: 1 to 110. Other values reserved.
                                                         ///< One record for every  resource block (RB)
  lte_LL1_log_demfront_uers_channel_estimation_uers_ce_records_s lte_LL1_log_demfront_uers_channel_estimation_uers_ce_records[LTE_LL1_LOG_DEMFRONT_UERS_CE_NUMBER_OF_RECORDS];
}lte_LL1_log_demfront_uers_channel_estimation_ind_struct;



/*==========================================================================*/
/*! @brief
 * 12 LTE LL1 Antenna Correlation Results (Log code ID: 0xB120)
 *
 *  This log packet gives antenna correlation related parameters. The 
 *  periodicity of this log packet is configurable. Recommended range is 10 ~ 
 *  100 ms. 
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_DEMFRONT_ANTENNA_CORRELATION_RESULTS_VERSION 21
#define LTE_LL1_LOG_DEMFRONT_NUMBER_OF_RECORDS 4

/*! @brief antenna_correlation_results number_of_records struct
 */
typedef struct
{
  uint32 tx_antenna                       : 2;  ///< Range: 0...3
  uint32 reserved0                        : 30; 
  uint32 antenna_correlation_real        ;  ///< Real part of Rx0 and Rx1 correlation
  uint32 antenna_correlation_imag        ;  ///< Imaginary part of Rx0 and Rx1 correlation
}lte_LL1_log_demfront_antenna_correlation_results_records_s;


/*! @brief antenna_correlation_results main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Version number (Range 0 to 255)
                                                           ///< Version = 1 for the log packet structure
                                                         ///< described below
  uint32 serving_cell_id                  : 9;  ///< Range: 0...503
  uint32 sub_frame_number                 : 4;  ///< Sub-frame number (Range 0..9)
  uint32 system_frame_number              : 10;  ///< System frame number (range 0..1023)
  uint32 reserved0                        : 1; 
  uint32 rssi_rx[2]                                   ;  ///< RSSI per Rx antenna in dBm. Q-format: 32S24.
                                                         ///< For example, -838860800 means
                                                         ///< (-838860800/2^24)=-50dBm.
  uint32 rssi                            ;  ///< Combined RSSI in dBm. Q-format: 32S24.
  uint32 reserved1                        : 26; 
  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC 
  uint32 number_of_records                : 2;  ///< Range: 1...4.
  lte_LL1_log_demfront_antenna_correlation_results_records_s lte_LL1_log_demfront_antenna_correlation_results_records[LTE_LL1_LOG_DEMFRONT_NUMBER_OF_RECORDS];
}lte_LL1_log_demfront_antenna_correlation_results_ind_struct;


/*==========================================================================*/
/*! @brief
* 13 LTE LL1 Serving Cell COM loop Results (Log code ID: 0xB121)
*
*  This log packet gives important parameters related to the serving cell com loop.
*  The log record is generated once every subframe
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_SERVING_CELL_COM_LOOP_VERSION 22
#define LTE_LL1_LOG_SERVING_CELL_NUMBER_OF_COM_RECORDS 20

/*! @brief lte_LL1_log_serving_cell_com_loop_records_t struct
*/
typedef struct
{
  uint32 system_frame_number              : 10; ///< System frame number (range 0..1023)
  uint32 sub_frame_number                 : 4;  ///< Sub-frame number (Range 0..9)
  uint32 enabled                          : 1;  ///< 0-Disabled, 1-Enabled 
  uint32 reserved0                        : 1;  ///< packing bit
  uint32 com_iir_alpha_q15                : 16; ///< COM loop IIR filter coefficients in 16U15
  uint32 com_adj_q6[2]                    ;     ///< Per Rx COM adjustment after unwrapping in 32S6
  uint32 com_unwrap_base_q6[2]            ;     ///< Per Rx COM unwrap base value in 32S6
  uint32 com_acc_stat[2][2]               ;     ///< Per Rx COM accumulation statistic (I and Q) in 32S.0

  uint32 reserved1[4]                     ;     ///< reserved (useful for debug)
}lte_LL1_log_serving_cell_com_loop_records_t;

/*! @brief serving_cell_com_loop main struct
*/
typedef struct
{
  uint32 version                             : 8;  ///< Log packet version. Range: 0...255.
                                                   ///< Version = 2 for the log packet structure
                                                   ///< described below
  uint32 carrier_index                       : 4;  ///< 0-PCC, 1-SCC 
  uint32 number_of_records                   : 5;  ///< Range: 1-20
  uint32 reserved0                           : 15; ///< packing bits
  lte_LL1_log_serving_cell_com_loop_records_t lte_LL1_log_serving_cell_com_loop_records[LTE_LL1_LOG_SERVING_CELL_NUMBER_OF_COM_RECORDS]; 

}lte_LL1_log_serving_cell_com_loop_ind_struct;

#define LTE_LL1_LOG_SERVING_CELL_FTL_CORR_VERSION 121
#define LTE_LL1_LOG_SERVING_CELL_NUMBER_OF_FTL_CORR_RECORDS 20
/*! @brief lte_LL1_log_serving_cell_ftl_corr_records_t struct
*/
typedef struct
{
  uint32 system_frame_number             : 10; ///< System frame number (range 0..1023)
  uint32 sub_frame_number                : 4;  ///< Sub-frame number (Range 0..9)
  uint32 rs_offset                       : 3;  ///< First RS symbol of SF for which correlations are performed
  uint32 num_rs_sym                      : 3;  ///< Number of RS symbol pairs correlated in SF
  uint32 reserved_0                      : 12; ///< Reserved
                                               
  uint32 ftl_snr_iir_alpha               : 16; ///<adjusted IIR filter coefficient
  uint32 reserved_1                      : 16; ///< Reserved
                                               
  int32 r2r0_real_tx0rx0[4];                  ///< {4 syms} x {tx0-rx0, tx1-rx1}                      
  int32 r2r0_imag_tx0rx0[4];                   
  int32 r2r1_real_tx0rx0[4];                       
  int32 r2r1_imag_tx0rx0[4];                        
  int32 r2r2_real_tx0rx0[4];  
  int32 r2r0_real_tx1rx1[4];                                        
  int32 r2r0_imag_tx1rx1[4];                   
  int32 r2r1_real_tx1rx1[4];                       
  int32 r2r1_imag_tx1rx1[4];                        
  int32 r2r2_real_tx1rx1[4];                            
}lte_LL1_log_serving_cell_ftl_corr_records_t;

typedef struct
{
  uint32 version                             : 8;  ///< Log packet version. Range: 0...255.
                                                   ///< Version = 2 for the log packet structure
                                                   ///< described below
  uint32 carrier_index                       : 4;  ///< 0-PCC, 1-SCC 
  uint32 number_of_records                   : 5;  ///< Range: 1-20
  uint32 reserved0                           : 15; ///< packing bits
  lte_LL1_log_serving_cell_ftl_corr_records_t lte_LL1_log_serving_cell_ftl_corr_records[LTE_LL1_LOG_SERVING_CELL_NUMBER_OF_FTL_CORR_RECORDS]; 

}lte_LL1_log_serving_cell_ftl_corr_ind_struct;

#endif //LTE_LL1_LOG_DEMFRONT_H
