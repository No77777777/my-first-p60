/*!
  @file
  lte_LL1_log_lted.h

  @brief
  LTE-D logging structures

*/
/*===========================================================================

  Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/lte_LL1_log_lted.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/

#ifndef LTE_LL1_LOG_LTED_H
#define LTE_LL1_LOG_LTED_H


#include "intf_common.h"

/*===========================================================================

                   FUNCTION DECLARATIONS FOR MODULE

===========================================================================*/

/*---------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------*/
/*! Maximum number of AGC adjustments */
#define LTE_LL1_RXAGC_LTED_MAX_NUM_ADJ 1

/*! Maximum number of DRID logged in the vector log packet */
#define LTE_LL1_LTED_LOG_MAX_NUM_DRID 2

/*! Number of DRID in the 20 MHz case */
#define LTE_LL1_LTED_LOG_NUM_DRID 50

/*! Number of DRID logged in the PSDCH log buffer */
#define LTE_LL1_LOG_LTED_DEMBACK_NUMBER_OF_RECORDS (50)

/*---------------------------------------------------------------------------
  Types
  -------------------------------------------------------------------------*/

#define LTE_LL1_LOG_RXAGC_LTED_VERSION 22

/*---------------------------------------------------------------------------
  LTE LL1 LTE-D RxAGG Log (Log code ID: 0xB154)
  -------------------------------------------------------------------------*/
/*! This log packet contains information regarding the LTE-D Fast RxAGC
 *  procedures. The packet is logged every time an LTE-D DL subframe is
 *  scheduled, which currently can happen in any LTE UL subframe. The highest
 *  frequency is therefore 6 times every 10 ms for TDD
---------------------------------------------------------------------------*/
typedef struct {

  /*! Log packet version */
  uint32 version      :8;

  /*! Frame number */
  uint32 frame        :10;

  /*! Subframe number */
  uint32 subframe     :4;

  /*! Reserved bits (pad to 32 bits) */
  uint32 reserved0    :10;

  /*! Subframe boundary time, in USTMR units */
  uint32 lted_sf_start_ustmr;

  /*! Symbol0 boundary time, in USTMR units */
  uint32 symbol0_end_boundary_ustmr;

  /*! Time when initial RFLM is called, in USTMR units */
  uint32 initial_rflm_call_ustmr;

  /*! Time when initial RFLM returns, in USTMR units */
  uint32 intial_rflm_ret_ustmr;

  /*! RxAGC timeline, in us from the subframe boundary
    [0] - signal rec - sf start
    [1] - poll end - sf start
    [2] - rflm call - sf start
    [3] - rflm ret - - sf start 
 */
  int8 timeline_us[4];

  /*! Estimated DSP speed, in MHz */
  uint32 est_dsp_mhz :10;

  /*! Padding to 32 bits */
  uint32 reserved1   :22;

  struct
  {
    /*! Time programmed for WBEE start, in vsrc units */
    uint32 wbee_start_time_vsrc[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Time programmed for WBEE stop, in vsrc units */
    uint32 wbee_stop_time_vsrc[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Time programmed for WBDC start, in vsrc units */
    uint32 dc_start_time_vsrc[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Time programmed for WBDC stop, in vsrc units */
    uint32 dc_stop_time_vsrc[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! WBEE register value read (per antenna) */
    uint32 wbee[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /* WBEE in dBFs: wbeedBFs10/10 */
    /*! Wbdc compensated WBEE estimate */
    int32 wbeedBFs10[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Time when WBEE programming is issued, in USTMR units */
    uint32 wbee_programming_ustmr[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Time when signal posted to thread, in USTMR units */
    uint32 signal_posted_ustmr;

    /*! Time when signal received by thread, in USTMR units */
    uint32 signal_received_ustmr;

    /*! Time when polling ended, in USTMR units */
    uint32 poll_end_ustmr;

    /*! Time when WBEE is read, in USTMR units */
    uint32 wbee_read_ustmr[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Time when DC is read, in USTMR units */
    uint32 dc_read_ustmr[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Time when Vector is notified of AGC change, in USTMR units */
    uint32 agc_idx_update_ustmr;

    /*! Time when fast RFLM is called, in USTMR units */
    uint32 fast_rflm_call_ustmr;

    /*! Time when fastRFLM returns, in USTMR units */
    uint32 fast_rflm_ret_ustmr;

    /*! Time at which LNA gain is expected to settle, in USTMR units */
    uint32 expected_lna_settle_ustmr;

    /*! DC register value read (per antenna, Re/Im) */
    int32 dc[LTE_LL1_MAX_NUM_UE_RX_ANT][2];

    /* LNA gain in dB: lna_gain_offset_dBx10/10 */
    /*! LNA gain in dB*10 */
    int16 lna_gain_offset_dBx10[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Programmed LNA gain */
    int8 lna_gain_state[LTE_LL1_MAX_NUM_UE_RX_ANT];

    /*! Symbol index at which the LNA gain settles */
    int8 settling_symb_idx;

    /*! Padding to 32 bits */
    int8 reserved;

  } adjustments[LTE_LL1_RXAGC_LTED_MAX_NUM_ADJ];

} lte_LL1_log_rxagc_lted_ind_struct;

#define LTE_LL1_LOG_LTED_SF_IND_VERSION 22

/*---------------------------------------------------------------------------
  LTE LL1 LTE-D Subframe Indication (Log code ID: 0xB155)
  -------------------------------------------------------------------------*/
/*! This log packet contains the main information regarding the LTE-D DL 
 *  procedures. The packet is logged every time an LTE-D DL subframe is
 *  scheduled, which currently can happen in any LTE UL subframe. The highest
 *  frequency is therefore 6 times every 10 ms for TDD
---------------------------------------------------------------------------*/
typedef struct {

  /*! Log packet version */
  uint64 version           :8;

  /*! Frame number */
  uint64 frame             :10;

  /*! Subframe number */
  uint64 subframe          :4;

  /*! LTE-D SF count since the beginning of the discovery period */
  uint64 lted_dl_sf_count  :32;

  /*! HARQ reset */
  uint64 harq_reset        :1;

  /*! Reserved bits (pad to 64) */
  uint64 reserved0         :9;

  /*! Mask of PRB's for the current subframe */
  uint64 prb_mask[2];

  /*! Pruned PRB mask */
  uint64 prb_mask_pruned[2];

  /*! RX FFT energy per antenna */
  uint32 rxfft_energy[LTE_LL1_MAX_NUM_UE_RX_ANT];

  /*! Number of scheduled decodes since the beginning of the discovery period */
  uint32 num_scheduled_drid_total;

  /* To convert SNR to dB:
     snr_dB = 10 * log10(snr / 8)
  */
  /*! Post equalization SNR, per DRID */
  int16 snr[LTE_LL1_LTED_LOG_NUM_DRID];

  /*! RSSI per antenna and DRID, in dBm */
  int8 rssi[LTE_LL1_MAX_NUM_UE_RX_ANT][LTE_LL1_LTED_LOG_NUM_DRID];

  /*! WB RSSI per antenna, in dBm */
  int8 wb_rssi[LTE_LL1_MAX_NUM_UE_RX_ANT];

  /*! Number of DRID in this subframe */
  int8 num_drid;

  /*! Number of DRID in this subframe */
  int8 num_drid_pruned;

} lte_LL1_log_lted_vector_sf_ind_struct;

#define LTE_LL1_LOG_LTED_VECTOR_DEBUG_VERSION 21

/*---------------------------------------------------------------------------
  LTE LL1 LTE-D Vector Debug Log (Log code ID: 0xB156)
  -------------------------------------------------------------------------*/
/*! This log packet contains extra debug information for the LTE-D DL 
 *  vector operations. The packet is logged every time an LTE-D DL subframe is
 *  scheduled, which currently can happen in any LTE UL subframe. The highest
 *  frequency is therefore 6 times every 10 ms for TDD
---------------------------------------------------------------------------*/
typedef struct {

  /*! Log packet version */
  uint64 version           :8;

  /*! Frame number */
  uint64 frame             :10;

  /*! Subframe number */
  uint64 subframe          :4;

  /*! Reserved bits */
  uint64 reserved0         :42;

  /*! Mask of PRB's for the current subframe */
  uint64 prb_mask[2];

  /*! @internal Sampling rate, in Hz */
  uint32 sampling_rate;

  /* To convert timing_delay to us:
     timing_delay_us = (timing_delay / 2^32) * rxfft_len * 1000 / sampling_rate
  */
  /*! Timing offset est for antenna, per DRID */
  int32 timing_delay[LTE_LL1_MAX_NUM_UE_RX_ANT][LTE_LL1_LTED_LOG_NUM_DRID];

  /* To convert freq_offset to Hz:
     freq_offset_hz = (freq_offset / 2^32) * sampling_rate / (rxfft_len + cp_len)
  */
  /*! Freq offset est for antenna,  per DRID */
  int32 freq_offset[LTE_LL1_MAX_NUM_UE_RX_ANT][LTE_LL1_LTED_LOG_NUM_DRID];

  /*! @internal RxFFT length */
  uint32 rxfft_len    :12;

  /*! @internal Average CP length across all symbols */
  uint32 cp_len       :10;

  /*! Number of DRID in this subframe */
  uint32 num_drid     :6;

  /*! Reserved bits for padding */
  uint32 reserved1    :4;

  /*! NB scale shifts for antenna, per DRID */
  int8 nb_scales[LTE_LL1_RXAGC_LTED_MAX_NUM_ADJ][LTE_LL1_MAX_NUM_UE_RX_ANT][LTE_LL1_LTED_LOG_NUM_DRID];

  /*! Wideband scales */
  int8 wb_scales[LTE_LL1_RXAGC_LTED_MAX_NUM_ADJ][LTE_LL1_MAX_NUM_UE_RX_ANT];

  /*! Number of times a new WB/NB scale is computed */
  int8 num_scales_computations;

  /*! Symbol index corresponding to the last RxAGC adjustment */
  int8 final_scales_sym_index;

} lte_LL1_log_lted_vector_debug_log_t;

typedef struct
{
  uint32 energy_metric       :21; //!< LLR ratio
  uint32 iteration_num       :4;  //!< Number of TB iterations required
  uint32 code_block_crc_pass :1;  //!< set if CB crc pass
  uint32 early_termination   :1;  //!< set if decoding is early terminated
  uint32 harq_combine_enable :1;  //!< set if HARQ combining was enabled
  uint32 deint_decode_bypass :1;  //!< shd never be set for lted
  uint32 bitr_timeout        :1;  //!< @internal set if bit read times out
  uint32 llrw_timeout        :1;  //!< @internal set if llr write time out
  uint32 llrr_timeout        :1;  //!< @internal set if llr read time out

} lte_LL1_log_demback_psdch_decoding_results_cblk_s;

typedef struct
{
  uint32 transport_block_crc     :1;  //!< 0: Fail, 1: Pass
  uint32 ndi                     :1;  //!< NDI bit
  uint32 code_block_size_plus    :13; //!< Unit: number of bits. Range: 0...8191
  uint32 num_code_block_plus     :4;  //!< 1  to 13
  uint32 max_tdec_iter           :4;  //!< 8,7,6, max_iteration_num
  uint32 retransmission_number   :3;  //!< 0x0: First transmissions, 0x1: Second transmission, 0x7: 8th transmission
  uint32 rvid                    :2;  //!< 0: RVID 0 ... 0x3: RVID 3
  uint32 companding_stats        :2;  //!< 0x0: 3 bit LLR (soft bits), 0x1: 4 bit LLR, 0x2: 6 bit LLR
  uint32 harq_combining          :1;  //!< 0: Harq not combine, 1: Harq combine
  uint32 decob_tb_crc            :1;  //!< tb_crc_readout from HW Decob TB Status

  uint32 effective_code_rate     :10; //!< coderate x 1024 ,  Range: 0x0000 = 0.0...0x3E8: 1.0 { different between ECP/NCP
  uint32 num_re                  :16; //!< 288/240 NCP/ECP
  uint32 bitw_timeout            :1;  //!< @internal set if bit write times out for the TB
  uint32 unused                  :5;
  lte_LL1_log_demback_psdch_decoding_results_cblk_s lte_LL1_log_demback_psdch_decoding_results_cblk_records;

} lte_LL1_log_demback_psdch_decoding_results_streams_s;

typedef struct
{
  uint32 sub_frame_offset   :16; //!< Offset relative to the starting SFN
  uint32 harq_id            :10; //!< 0-799
  uint32 rp_id              :4;  //!< 0-15
  uint32 harq_log_status    :2;  //!< 0-Normal, 1-Reset, 2 - Overwritten

  uint32 rnti_type          :4;  //!< 0 - C_RNTI , 1 - SPS_C_RNTI, 2 - P_RNTI, 3 - RA_RNTI, 4 - T_C_RNTI, 5- SI_RNTI,6 - TPC_PUSCH_RNTI, 7 - TPC_PUCCH_RNTI, 8 -MBMS_RNTI, 9 -PSDCH
  uint32 prb_idx            :7;  //!< 0-99
  uint32 reserved           :21;
  lte_LL1_log_demback_psdch_decoding_results_streams_s lte_LL1_log_demback_psdch_decoding_results_streams;

} lte_LL1_log_demback_psdch_decoding_results_records_s;

#define LTE_LL1_LOG_PSDCH_DECODE_REPORT_VERSION 21

/*---------------------------------------------------------------------------
  LTE LL1 LTE-D PSDCH Decode Report (Log code ID: 0xB157)
  -------------------------------------------------------------------------*/
/*! This log packet contains information regarding the LTE-D DL decode.
 *  The packet is logged after LTE_LL1_LOG_LTED_DEMBACK_NUMBER_OF_RECORDS (50)
 *  code blocks are decoded. In the case of maximum allocation (50 DRID
 *  per subframe) this can happen for each LTE-D subframe. This makes the
 *  maximum frequency 6 times every 10 subframes for TDD  
---------------------------------------------------------------------------*/
typedef struct
{
  uint32 version                      :8;  //!< Log packet version. Range: 0...255. Version = 1 for the log packet structure described below
  uint32 serving_cell_id              :9;  //!< Range: 0...503.
  uint32 starting_sub_frame_number    :4;  //!< Sub-frame num.ber (Range 0..9)
  uint32 starting_system_frame_number :10; //!< System frame number (range 0..1023)
  uint32 log_buf_activated_flag       :1;  //!< @internal Whether log buf is used for logging or not

  uint32 ue_category                  :2;  //!< To distinguish between UE capable of supporting 50 or 400 processes
  uint32 num_dl_harq                  :10; //!< Will be different if we support 400 or 800 process
  uint32 num_dl_harq_stored           :10; //!< @internal num_of_DL_harq emem is holding
  uint32 carrier_index                :4;  //!< 0-PCC, 1-SCC
  uint32 number_of_records            :6;  //!< Range: 0..49
  lte_LL1_log_demback_psdch_decoding_results_records_s lte_LL1_log_demback_psdch_decoding_results_records[LTE_LL1_LOG_LTED_DEMBACK_NUMBER_OF_RECORDS];

} lte_LL1_log_demback_psdch_decoding_results_ind_struct;

#endif /* LTE_LL1_LOG_LTED_H */
