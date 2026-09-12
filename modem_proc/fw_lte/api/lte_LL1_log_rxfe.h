/*!
  @file
  lte_LL1_log_rxfe.h

  @brief
  rxfe log packet data structures

  @detail
  !!! Please do not modify !!!
  This file was auto-generated on Mon Nov 14 17:14:23 2011.
  Command line used: perl w2h.pl log_rxfe rxfe 2 4
   Note: The params, [2 4], correspond to version numbers for
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

$Header: //components/rel/fw_lte.mpss/17.0.c4/api/lte_LL1_log_rxfe.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/
#ifndef LTE_LL1_LOG_RXFE_H
#define LTE_LL1_LOG_RXFE_H

#include "intf_common.h"

#define LTE_LL1_LOG_RXFE_RS_NUMBER_OF_RECORDS_FIXED 4
#define LTE_LL1_LOG_RXFE_NUMBER_OF_RECORDS 14

/*! @brief agc_int rs_number_of_records_fixed struct
 */
typedef struct
{
  uint32 tx0_crs_ee_raw                  ;  ///< 32S31 for  raw symbol CRS energy estimate;
                                                         ///< calculate in linear scale as x/pow(2,31) where x =
                                                         ///< CRS EE raw
  uint32 tx1_crs_ee_raw                  ;  ///< 32S31 for raw symbol CRS energy estimate; calculate
                                                         ///< in linear scale as x/pow(2,31) where x = CRS EE raw
}lte_LL1_log_rxfe_agc_int_rs_records_s;


/*! @brief agc_int number_of_records struct
 */
typedef struct
{
  uint32 rxfft_scale                      :16;  ///< 16-bit signed RxFFT gain mantissa used in hardware
                                                         ///< programming; calculate in linear scale as
                                                         ///< x/pow(2,15) where x = RxFFT scale
  uint32 rxfft_input_bias                 : 5;  ///< 5-bit signed RxFFT gain exponent used in hardware
                                                         ///< programming;
  uint32 reserved0                        : 11; 
}lte_LL1_log_rxfe_agc_int_records_s;

/*==========================================================================*/
/*! @brief
 * 1 AGC (Log code ID: 0xB111)
 *
 *  This log packet gives important parameters related to the receiver 
 *  front end automatic gain control (AGC). The log record is generated 
 *  every 20 records (10ms with 1Rx antenna per record).
 *  In CA scenario, Primary cell and secondary cell are logged in separate log packets.
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_RXFE_NUMBER_OF_RECORDS_FIXED 20
#define LTE_LL1_LOG_RXFE_AGC_VERSION 141  //new version number is needed

/*! @brief agc number_of_records_fixed struct
 */
typedef struct
{
  uint32 rx_antenna_index                 : 2;  ///< Range 0 to 2 (0: Rx0, 1:Rx1, 2: both Rx)

  uint32 system_frame_number              :10;  ///< System frame number; range is 0 to 1023

  uint32 sub_frame_number                 : 4;  ///< Subframe number; range is 0 to 9
                                                
  uint32 lna_gain_state                   : 4;  ///< LNA Gain State
                                                         ///< Range 0..5
  uint32 sf_type                          : 2;  ///< range 0 to 3,  0 for special subframe
                                                         ///< 1: uplink subframe, 2: downlink subframe
                                                         ///< 3: uplink or downlink subframe
  uint32 reserved0                        :10;

  uint32 dvga_bias                        : 5;  ///< 5-bit signed DVGA exponent used in HW
                                                         ///< programming. Calculate DVGA in dB as
                                                         ///< 20*log10[x/pow(2,15)*pow(2,y)] where x= DVGA
                                                         ///< scale, y= DVGA bias
  uint32 dvga_scale                       :16;  ///< 16-bit signed DVGA mantissa used in HW
                                                         ///< programming. Calculate in linear scale as
                                                         ///< x/pow(2,15) where x= DVGA scale
  uint32 reserved6                       : 11;

  uint32 broadband_rssi_estimate          :12;  ///< 12-bit signed Broadband RSSI estimate value in
                                                         ///< 0.0625 dBm resolution. Range: (-120 dBm to -10
                                                         ///< dBm).
                                                         ///< To be displayed in dBm scale after calculating
                                                         ///< x/16  where
                                                         ///< x = Broadband RSSI estimate
                                                         ///< x=-1 (unsigned raw number is all 1s) is to be
                                                         ///< interpreted as field not populated
  uint32 measured_total_rssi              :10;  ///< 10-bit signed RSSI value for all tones in 0.25
                                                         ///< dBm resolution. Range: (-120 dBm to -10 dBm).
                                                         ///< To be displayed in dBm scale after calculating
                                                         ///< x/4  where
                                                         ///< x = Measured total RSSI
                                                         ///< x=-1 (unsigned raw number is all 1s) is to be
                                                         ///< interpreted as field not populated
  uint32 measured_inband_rssi             :10;  ///< 10-bit signed RSSI value for all tones in 0.25
                                                         ///< dBm resolution. Range: (-120 dBm to -10 dBm).
                                                         ///< To be displayed in dBm scale after calculating
                                                         ///< x/4  where
                                                         ///< x = Measured inband  RSSI
                                                         ///< x=-1 (unsigned raw number is all 1s) is to be
                                                         ///< interpreted as field not populated
  /// @ internal
  /****beyond this is for internal use*/                                        
  uint32 previous_lna_offset              :12; ///< 12-bit signed value for previous LNA offset  in
                                               ///< 0.0625 dB resolution. To be displayed in dB scale
                                               ///< after calculating  x/16 where
                                               ///< x =previous LNA offset
                                               ///<range is -128 to 127
                                        
  uint32 inst_total_rssi                  :10; ///< 10-bit signed RSSI value for all tones in 0.25            
                                               ///< dBm resolution. Range: (-120 dBm to -10 dBm).    
                                               ///< To be displayed in dBm scale after calculating   
                                               ///< x/4  where                                       
                                               ///< x = Measured inband  RSSI                        
                                               ///< x=-1 (unsigned raw number is all 1s) is to be    
                                               ///< interpreted as field not populated               
  uint32 next_lna_gain_state               :4; ///< LNA Gain State for next subframe; range is 0 to 5
                                        
  uint32 deficit_bias                      :3; ///< Integer in [2,7] range representing the deficit
                                               ///< bias to avoid saturation in FFT
  uint32 reserved1                         :3;
                                        
  uint16 inner_loop_gain                     ; ///< 16bit. To be displayed in [0,1] dividing by 2^15.  
                                               /// (left shift existing value by 16)
  uint16 outer_loop_gain                     ; ///< 16bit number. To be displayed in [0,1] dividing by 2^15. 
                                               /// (left shift existing value of 31 bit by 16 )
  uint32 bb_ee                               ; ///< 32u16 for the broadband energy estimate per
                                               ///< subframe; calculate in linear scale as x/pow(2,16)
                                               ///< where x = BB EE reading from HW
  uint32 dc_i_offset                         ; ///< Current instantaneous DC_i; 32S31, x * 841.1/(2^31)
                                               ///< in unit(mv)
  uint32 dc_q_offset                         ; ///< Current instantaneous DC_q; 32S31, x * 841.1/(2^31)
                                               /// < in unit(mv)
  uint32 aci_power                           ; ///< 32S24 for the ACI power per subframe; calculate in
                                               ///< dB scale as x/pow(2,24) where x = ACI power
  uint32 dc_max_ee                           ; ///< 32S31 for max DC energy for CRS symbols; calculate
                                               ///< in linear scale as x/pow(2,31)  where x = DC max EE
                                               ///< where x = inband accum EE for non-CRS
  uint32 fft_ee_max                          ; ///< 32S31 for the max FFT energy estimate per subframe;
                                               ///< calculate in linear scale as x/pow(2,31)  where x =
                                               ///< FFT EE max
  uint32 hybrid_ee                           ; ///< 32S31 for the HybridEE energy estimate per subframe;
                                               ///< calculate in linear scale as x/pow(2,31)  where x =
                                               ///< HybridEE
  uint32 DVGA_dB                             ; ///< 32S24 for previous DVGA; calculate in dB
                                               ///< scale as x/pow(2,24) where x = previous DVGA.;                    
  uint32 agc_error_inst                   :16; ///< unfiltered AGC error on current subframe;
                                               ///< calculate in dB scale as x/pow(2,8) where x = AGC
  uint32 agc_error_accum                  :16; ///< previous DVGA; calculate in dB
                                               ///< scale as x/pow(2,8) where x = previous DVGA.;
                                               ///< range is -128 dB to 127 dB  uint32 reserved3  

  lte_LL1_log_rxfe_agc_int_rs_records_s lte_LL1_log_rxfe_agc_int_rs_records[LTE_LL1_LOG_RXFE_RS_NUMBER_OF_RECORDS_FIXED];  //log 4 crsee value
  lte_LL1_log_rxfe_agc_int_records_s    lte_LL1_log_rxfe_agc_int_records[LTE_LL1_LOG_RXFE_RS_NUMBER_OF_RECORDS_FIXED];  //log only 4 scale bias values
}lte_LL1_log_rxfe_agc_records_s;


/*! @brief agc main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Log packet version; range is 0 to 255
                                                         ///< Version - 2 for the log packet structure
                                                         ///< described below
  uint32 number_of_records                : 5;  ///< Range is 1 to 20
  uint32 agc_mode                         : 4;  ///< Range is 0 to 15 (0:Freq scan, 1:Init Acq,
                                                         ///< 2:Traffic, 3:PBCH, 4:Inter-freq, 5:Online-DRX,
                                                         ///< 6:Offline-DRX, 7: X2L idle, 8: X2L_CONN,
                                                         ///< 9: TDD_SRCH_PIPELINE, 10: X2L_TDD_MEAS_LNA,
                                                         ///< 11: X2L_TDD_MEAS_VCELLTIME, 15:OFF)
  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC 
  uint32 reserved0                        :11; 
  lte_LL1_log_rxfe_agc_records_s lte_LL1_log_rxfe_agc_records[LTE_LL1_LOG_RXFE_NUMBER_OF_RECORDS_FIXED];
}lte_LL1_log_rxfe_agc_ind_struct;



/*==========================================================================*/
/*! @brief
 * 2  AGC INT(Log code ID: 0xB112) -Internal
 *
 *  This internal log packet gives important parameters related to the 
 *  receiver front end automatic gain control (AGC). The log record is 
 *  generated every 1 ms.
 *  In CA scenario, Primary cell and secondary cell are logged in separate log packets.
---------------------------------------------------------------------------*/
#define LTE_LL1_LOG_RXFE_AGC_INT_VERSION 101

/*! @brief agc_int main struct
 */
typedef struct
{
  uint32 version                          : 8;  ///< Range is 0 to 255
                                                         ///< Version = 4 for the log packet structure described
                                                         ///< below
#ifdef LTE_LL1_FEATURE_THOR_HORXD_LOG_B112
  uint32 reserved5                        : 3;  
  uint32 sub_frame_number                 : 4;  ///< Subframe number; range is 0 to 9
  uint32 system_frame_number              :10;  ///< System frame number; range is 0 to 1023
  uint32 rx_antenna_index                 : 2;   ///< Range 0,1,2,3
#else
  uint32 reserved5                        : 4;  
  uint32 sub_frame_number                 : 4;  ///< Subframe number; range is 0 to 9
  uint32 system_frame_number              :10;  ///< System frame number; range is 0 to 1023
  uint32 rx_antenna_index                 : 1;   ///< Range 0,1,2,3
#endif
  uint32 agc_mode                         : 4;  ///< To be displayed as:
                                                         ///< 0: "Freq scan"
                                                         ///< 1: "Init Acq"
                                                         ///< 2: "Traffic"
                                                         ///< 3: "PBCH"
                                                         ///< 4: "Inter-freq"
                                                         ///< 5: "Online-DRX"
                                                         ///< 6: "Offline-DRX"
                                                         ///< 7: "Inter-RAT/Inter-Freq Idle"
                                                         ///< 8: "Inter-RAT/Inter-freq Connected"
                                                         ///< 9: "TDD_SRCH_PIPELINE" 
                                                         ///< 10: "X2L_TDD_MEAS_LNA"
                                                         ///< 11: "X2L_TDD_MEAS_VCELLTIME"
                                                         ///< 15: "OFF"
  uint32 reserved0                        : 1; 

  uint32 carrier_index                    : 4; ///< 0-PCC, 1-SCC 
  uint32 reserved                         : 28; 

  uint16 inner_loop_gain                 ;  ///< 16S15 number. To be displayed in [0,1] range after
                                                         ///< dividing by 2^15.
  uint16 inner_loop_gain_marginal        ;  ///< 16S15 number. To be displayed in [0,1] range after
                                                         ///< dividing by 2^15.
  uint32 outer_loop_gain                 ;  ///< 32S31 number. To be displayed in [0,1] range after
                                                         ///< dividing by 2^31.
  uint32 dc_loop_constant                 : 4;  ///< DC loop constant K. DC loop gain is 2^K.
  uint32 measured_in_band_rssi            :12;  ///< 12-bit signed RSSI value for user-allocated tones
                                                         ///< in 0.0625 dBm resolution; range is -120 dBm to -10
                                                         ///< dBm.
                                                         ///< To be displayed in dBm scale after calculating
                                                         ///< x/16 where
                                                         ///< x = Measured in-band RSSI
  uint32 measured_total_rssi              :12;  ///< 12-bit signed RSSI value for all tones in 0.0625
                                                         ///< dBm resolution. Range: (-120 dBm to -10 dBm).
                                                         ///< To be displayed in dBm scale after calculating
                                                         ///< x/16 where
                                                         ///< x = Measured total RSSI
  uint32 reserved1                        : 4; 
  uint32 broadband_rssi_estimate          :12;  ///< 12-bit signed broadband RSSI estimate in 0.0625 dBm
                                                         ///< resolution. Range: (-120 dBm to -10 dBm).
                                                         ///< To be displayed in dBm scale after calculating
                                                         ///< x/16 where
                                                         ///< x = broadband RSSI
  uint32 curr_total_rssi_to_iir           :12;  ///< 12-bit signed RSSI value, input to iir, in 0.0625
                                                         ///< dBm resolution. Range: (-120 dBm to -10 dBm).
                                                         ///< To be displayed in dBm scale after calculating
                                                         ///< x/16 where
                                                         ///< x = current total RSSI, input to IIR
  uint32 reserved2                        :8; 
  uint32 fft_ee_max                      ;  ///< 32S31 for the max FFT energy estimate per subframe;
                                                         ///< calculate in linear scale as x/pow(2,31) where x =
                                                         ///< FFT EE max
  uint32 crs_ee_avg                      ;  ///< 32S31 for  the average CRS energy estimate per
                                                         ///< subframe; calculate in linear scale as x/pow(2,31)
                                                         ///< where x = CRS EE avg
  uint32 fft_ee_accum                    ;  ///< 32S31 for the accum FFT energy estimate per subframe;
                                                         ///< calculate in linear scale as x/pow(2,31) where x =
                                                         ///< FFT EE accum
  uint32 hybrid_ee                       ;  ///< 32S31 for the hybrid energy estimate per subframe;
                                                         ///< calculate in linear scale as x/pow(2,31) where x =
                                                         ///< hybrid EE
  uint32 bb_ee                           ;  ///< 32u16 for the broadband energy estimate per
                                                         ///< subframe; calculate in linear scale as x/pow(2,16)
                                                         ///< where x = BB EE reading from HW
  uint32 aci_power                       ;  ///< 32S24 for the ACI power per subframe; calculate in
                                                         ///< dB scale as x/pow(2,24) where x = ACI power
  uint32 dc_i_offset                     ;  ///< Current instantaneous DC_i; 32S31, x * 841.1/(2^31)
                                                         ///< in unit(mv)
  uint32 dc_q_offset                     ;  ///< Current instantaneous DC_q; 32S31, x * 841.1/(2^31)
                                                         ///< unit(mv)
  uint32 dc_accum_ee_crs                 ;  ///< 32S31 for accumulated DC energy for CRS symbols.
                                                         ///< calculate in linear scale as x/pow(2,31)  where x =
                                                         ///< DC accum EE
  uint32 inband_accum_ee_crs             ;  ///< 32S31 for accumulated Inband energy for CRS
                                                         ///< symbols; calculate in linear scale as x/pow(2,31)
                                                         ///< where x = inband accum EE
  uint32 outband_accum_ee_crs            ;  ///< 32S31 for accumulated Outband energy for CRS
                                                         ///< symbols; calculate in linear scale as x/pow(2,31)
                                                         ///< where x = outband accum EE
  uint32 dc_max_ee_crs                   ;  ///< 32S31 for max DC energy for CRS symbols; calculate
                                                         ///< in linear scale as x/pow(2,31)  where x = DC max EE
  uint32 inband_max_ee_crs               ;  ///< 32S31 for max Inband energy for CRS symbols;
                                                         ///< calculate in linear scale as x/pow(2,31)  where x =
                                                         ///< inband max EE
  uint32 outband_max_ee_crs              ;  ///< 32S31 for max Outband energy for CRS symbols;
                                                         ///< calculate in linear scale as x/pow(2,31)  where x =
                                                         ///< outband max EE
  uint32 dc_accum_ee_non_crs             ;  ///< 32S31 for accumulated DC energy for  non-CRS
                                                         ///< symbols; calculate in linear scale as x/pow(2,31)
                                                         ///< where x = dc accum EE for non-CRS
  uint32 inband_accum_ee_non_crs         ;  ///< 32S31 for accumulated Inband energy for non-CRS
                                                         ///< symbols; calculate in linear scale as x/pow(2,31)
                                                         ///< where x = inband accum EE for non-CRS
  uint32 outband_accum_ee_non_crs        ;  ///< 32S31 for accumulated Outband energy for non-CRS
                                                         ///< symbols; calculate in linear scale as x/pow(2,31)
                                                         ///< where x = outband accum EE for non-CRS
  uint32 dc_max_ee_non_crs               ;  ///< 32S31 for max DC energy for non-CRS symbols;
                                                         ///< calculate in linear scale as x/pow(2,31)  where x =
                                                         ///< dc max EE for non-CRS
  uint32 inband_max_ee_non_crs           ;  ///< 32S31 for max Inband energy for non-CRS symbols;
                                                         ///< calculate in linear scale as x/pow(2,31)  where x =
                                                         ///< inband max EE for non-CRS
  uint32 outband_max_ee_non_crs          ;  ///< 32S31 for max Outband energy for non-CRS symbols;
                                                         ///< calculate in linear scale as x/pow(2,31)  where x =
                                                         ///< outband max EE for non-CRS
  lte_LL1_log_rxfe_agc_int_rs_records_s lte_LL1_log_rxfe_agc_int_rs_records[LTE_LL1_LOG_RXFE_RS_NUMBER_OF_RECORDS_FIXED];
  uint32 previous_lna_gain_state          : 3;  ///< LNA Gain State for next subframe; range is 0 to 5
  uint32 previous_lna_offset              :12;  ///< 12-bit signed value for previous LNA offset  in
                                                         ///< 0.0625 dB resolution. To be displayed in dB scale
                                                         ///< after calculating  x/16 where
                                                         ///< x =previous LNA offset
                                                         ///<range is -128 to 127
  uint32 reserved3                        :17; 
  uint32 previous_DVGA_dB                ;  ///< 32S24 for previous DVGA; calculate in dB
                                                         ///< scale as x/pow(2,24) where x = previous DVGA.;
                                                         ///< range is -128 dB to 127 dB  uint32 reserved3                        :17; 

  uint32 agc_error                       ;  ///< 32S24 for unfiltered AGC error on current subframe;
                                                         ///< calculate in dB scale as x/pow(2,24) where x = AGC
                                                         ///< error; range is -128 dB to 127 dB
  uint32 agc_error_accum                 ;  ///< 32S24 for  AGC accumulated error; calculate in dB
                                                         ///< scale as x/pow(2,24) where x = AGC error accum.;
                                                         ///< range is -128 dB to 127 dB
  uint32 next_lna_gain_state              : 3;  ///< LNA Gain State for next subframe; range is 0 to 5
  uint32 next_lna_offset                  :12;  ///< 12-bit signed value for next LNA offset  in 0.0625
                                                         ///< dB resolution. To be displayed in dB scale after
                                                         ///< calculating  x/16 where
                                                         ///< x =next LNA offset
                                                         ///<range is -128 to 127
  uint32 aci_detect                       : 1;  ///< 0 - No jammer detected
                                                         ///< 1 - Jammer detected
  uint32 deficit_bias                     : 3;  ///< Integer in [2,7] range representing the deficit
                                                         ///< bias to avoid saturation in FFT
  uint32 number_of_records                : 4;  ///< Integer between [0-15]
  uint32 reserved4                        : 9; 
  lte_LL1_log_rxfe_agc_int_records_s lte_LL1_log_rxfe_agc_int_records[LTE_LL1_LOG_RXFE_NUMBER_OF_RECORDS];
}lte_LL1_log_rxfe_agc_int_ind_struct;


/*==========================================================================*/
/*! @brief
 * Rx ant info (Log code ID: 0xB15B)
 *
 *  This log packet logs the Rx ant info per carrier
 *  In CA, each carrier will have its own packet
 *  This packet is generated upon state change only
---------------------------------------------------------------------------*/
#ifdef FR63568_ARD_DYNAMIC_1RXTO2RX_THRESHOLD
#define LOG_LTE_LL1_RX_ANT_INFO_VERSION     162
#else
#define LOG_LTE_LL1_RX_ANT_INFO_VERSION     161
#endif

/* Log packet is designed for 4 rx(in HE, HI) so define buffers for 4rx
 * but for HA use only 2rx */
#define LTE_LL1_MAX_NUM_UE_RX_ANT_LOG_PKT   4
/** 3 words per structure */
typedef struct
{
  uint8 physical_rx;
  uint8 lna_state_index;
  int8  rxfft_deficit_bias;
  uint8 reserved;
  int32 dvga_dB;
  int32 tot_rssi_accum_dB;
} lte_LL1_log_rx_agc_info_struct;

#ifdef FR63568_ARD_DYNAMIC_1RXTO2RX_THRESHOLD
typedef struct
{
  uint32  system_frame               : 10;///< System frame number
  uint32  subframe                   : 4; ///< Subframe number
  boolean threshold_updated          : 1; ///< threshold update on this subframe (TRUE) or not (FALSE)
  uint32  decodability               : 1; ///< PDCCH decodability 1 or 0
  uint32  agg_level_used             : 4; ///< PDCCH agg level 1,2,4 or 8
  uint32  rnti_type                  : 8; ///< RNTI type
                                          ///< typedef enum
                                          ///< {
                                          ///< LTE_LL1_C_RNTI = 0,
                                          ///< LTE_LL1_SPS_C_RNTI,
                                          ///< LTE_LL1_P_RNTI,
                                          ///< LTE_LL1_RA_RNTI,
                                          ///< LTE_LL1_T_C_RNTI,
                                          ///< LTE_LL1_SI_RNTI,       
                                          ///< LTE_LL1_TPC_PUSCH_RNTI,       
                                          ///< LTE_LL1_TPC_PUCCH_RNTI, 
                                          ///< LTE_LL1_MBMS_RNTI,
                                          ///< LTE_LL1_CC_RNTI,
                                          ///< LTE_LL1_SRS_TPC_RNTI_1,
                                          ///< LTE_LL1_SRS_TPC_RNTI_2,
                                          ///< LTE_LL1_RNTI_COUNT,
                                          ///< LTE_LL1_RLM_RNTI = LTE_LL1_RNTI_COUNT,
                                          ///< LTE_LL1_C_RNTI_TX_ANT_SEL = LTE_LL1_RNTI_COUNT,
                                          ///< LTE_LL1_T_C_RNTI_TX_ANT_SEL
                                          ///< } lte_LL1_rnti_type_enum;
  boolean  pdcch_sched               : 1; ///< PDCCH scheduled ( TRUE or FALSE)
  boolean  pdcch_crc_pass            : 1; ///< PDCCH crc pass  ( TRUE or FALSE)
  uint32   reserved                  : 2;
  int32 snr_threshold;                    ///< SNR threshold dBQ8, print snr_threshold/256 in dB
} lte_LL1_log_dyn_c1rx_info_struct;
#endif

typedef struct
{
  /****************** Word 1 ******************/
  uint32 version                          : 8; ///< Range is 0 to 255
                                               ///< Version = 161 for the log packet structure described
                                               ///< below
  uint32 sub_frame_num                    : 4; ///< Subframe number; range is 0 to 9
  uint32 system_frame_num                 :10; ///< System frame number; range is 0 to 1023
  uint32 cxn_idx                          : 1; ///< 0-CXN_0 1-CXN_1
  uint32 carrier_index                    : 3; ///< 0-PCC, 1-SCC1, 2-SCC2, 3-SCC3, 4-SCC4, 5-SCC5
  uint32 info_chg_cmd_type                : 2; ///< typedef enum
                                               ///< {
                                               ///<   LFW_ARD_CTRL_CMD_CONFIG_APP   = 0,
                                               ///<   LFW_ARD_CTRL_CMD_ANT_MODE_CHG = 1,
                                               ///<   LFW_ARD_CTRL_CMD_CSF_STATE_CHG = 2,
                                               ///< } lte_LL1_rx_info_chg_cmd_type_enum;
  uint32 num_rx_to_enable                 : 3; ///< Num of Rx to enable
  uint32 only_mapping_chg                 : 1; ///< (TRUE/FALSE) Indicates if num_rx are same but just mapping changed

  /****************** Word 2 ******************/
  /// @internal
  uint32 enable_mask                      : 4; ///< Rx enable mask BIT(0) - PRx and so on (in Hex)
  /// @internal
  uint32 disable_mask                     : 4; ///< Rx disable mask BIT(0) - PRx and so on (in Hex)

  uint32 num_rx_agc                       : 3; ///< Num of Rx for which AGC is sent

  uint32 agc_mode                         : 4; ///< typedef enum
                                               ///< {
                                               ///<   LTE_LL1_RXAGC_MODE_FREQ_SCAN,
                                               ///<   LTE_LL1_RXAGC_MODE_ACQ,
                                               ///<   LTE_LL1_RXAGC_MODE_TRAFFIC,
                                               ///<   LTE_LL1_RXAGC_MODE_PBCH,
                                               ///<   LTE_LL1_RXAGC_MODE_INTER_FREQ,
                                               ///<   LTE_LL1_RXAGC_MODE_ONL_DRX,
                                               ///<   LTE_LL1_RXAGC_MODE_OFFLINE_DRX,
                                               ///<   LTE_LL1_RXAGC_MODE_X2L_IDLE,
                                               ///<   LTE_LL1_RXAGC_MODE_X2L_CONN,
                                               ///<   LTE_LL1_RXAGC_MODE_TDD_SRCH_PIPELINE,
                                               ///<   LTE_LL1_RXAGC_MODE_X2L_TDD_MEAS_LNA,
                                               ///<   LTE_LL1_RXAGC_MODE_X2L_TDD_MEAS_VCELLTIME,
                                               ///<   LTE_LL1_NUM_RXAGC_MODE,
                                               ///<   LTE_LL1_RXAGC_MODE_DEFAULT=15
                                               ///< } lte_LL1_rxagc_mode_e;
  /// @internal
  uint32 start_eval                       : 1; ///< (TRUE/FALSE)
  /// @internal
  uint32 valid_rx_groups                  : 6; ///< Rx groups that should be considered for best 2 eval (in HEX)
  /// @internal
  uint32 skip_speff_based_eval            : 1; ///< (TRUE/FALSE)
  /// @internal
  uint32 speff_refresh                    : 1; ///< (TRUE/FALSE) Speff refresh cmd from ML1
  /// @internal
  uint32 csf_action                       : 3; ///< typedef enum
                                               ///< {
                                               ///<   LFW_ARD_CTRL_CSF_ACTION_NONE = 0,
                                               ///<   LFW_ARD_CTRL_CSF_ACTION_RESUME = 1,
                                               ///<   LFW_ARD_CTRL_CSF_ACTION_FREEZE = 2,
                                               ///<   LFW_ARD_CTRL_CSF_ACTION_EVAL = 3,
                                               ///<   LFW_ARD_CTRL_CSF_ACTION_REFRESH = 4,
                                               ///<   LFW_ARD_CTRL_CSF_ACTION_SPEFF_CONC = 5,
                                               ///< } lfw_ard_ctrl_csf_action_enum;
  /// @internal
  uint32 csf_speff_reset                  : 1; ///< (TRUE/FALSE)
  /// @internal
  uint32 csirs_filter_reset               : 1; ///< (TRUE/FALSE)
  /// @internal
  uint32 csf_w1_reset                     : 1; ///< (TRUE/FALSE)
  /// @internal
  uint32 forced_csf_reset                 : 1; ///< (TRUE/FALSE)
  /// @internal
  uint32 auto_refresh_after_eval          : 1; ///< (TRUE/FALSE)

  /****************** Word 3 ******************/
  /// @internal
  uint32 csf_num_rx                       : 3;  /// Max 4
  /// @internal
  uint32 curr_csf_state                   : 3;  ///< typedef enum
                                                ///< {
                                                ///<   LFW_ARD_CTRL_CSF_STATE_STEADY   = 0,
                                                ///<   LFW_ARD_CTRL_CSF_STATE_FALLBACK = 1,
                                                ///<   LFW_ARD_CTRL_CSF_STATE_EVAL     = 2,
                                                ///<   LFW_ARD_CTRL_CSF_STATE_EVAL_SKIP_SPEFF = 3,
                                                ///<   LFW_ARD_CTRL_CSF_STATE_CONC     = 4,
                                                ///<   LFW_ARD_CTRL_CSF_STATE_REFRESH  = 5,
                                                ///<   LFW_ARD_CTRL_CSF_STATE_MAX
                                                ///< } lfw_ard_ctrl_csf_state_enum;
  /// @internal
  uint32 new_csf_state                    : 3;  ///< lfw_ard_ctrl_csf_state_enum
  /// @internal
  uint32 last_ss_logical_mask             : 4;  ///< Logical mask used by CSF
  /// @internal
  uint32 last_ss_phy_mask                 : 4;  ///< Phy mask used by CSF
  /// @internal
  uint32 rx_action                        :12;  ///< 3 bit action x num of rx (4) = 12 bits
                                                ///< typedef enum
                                                ///< {
                                                ///<   // No action
                                                ///<   LTE_LL1_ACTION_NONE = 0,
                                                ///<   // Resume (timeout value is needed if its a resume)
                                                ///<   LTE_LL1_ACTION_RESUME,
                                                ///<   // Reset (Complete hard reset)
                                                ///<   LTE_LL1_ACTION_RESET,
                                                ///<   // Freeze (used by CSF to freeze rank SPEFF)
                                                ///<   LTE_LL1_ACTION_FREEZE
                                                ///< } lte_LL1_sys_reset_resume_action_e;
  /// @internal
  uint32 best2_selection_ref_carrier      : 3;  ///< 0-PCC, 1-SCC1, 2-SCC2, 3-SCC3, 4-SCC4, 5-SCC5

  /****************** Word 4 ******************/
  uint8 previous_mapping[LTE_LL1_MAX_NUM_UE_RX_ANT_LOG_PKT]; ///< Previous Rx info mapping index0 - PRx and so on
                                                 ///< 4 means invalid -> Print "NA"
  /****************** Word 5 ******************/
  uint8 new_mapping[LTE_LL1_MAX_NUM_UE_RX_ANT_LOG_PKT];      ///< New Rx info mapping index0 - PRx and so on
                                                 ///< 4 means invalid -> Print "NA"
  /****************** Word 6 ******************/
  /// @internal
  uint8 csf_mapping[LTE_LL1_MAX_NUM_UE_RX_ANT_LOG_PKT];      ///< CSF Rx mapping index0 - PRx and so on
                                                 ///< 4 means invalid -> Print "NA"
  /****************** Word 7 ******************/
  /// @internal
  uint8 last_eval_mapping[LTE_LL1_MAX_NUM_UE_RX_ANT_LOG_PKT];///< Last EVAL mode mapping index0 - PRx and so on
                                                 ///< 4 means invalid -> Print "NA"
  /****************** Word 8 ******************/
  /// @internal
  uint8 td_samp_buf_idx[LTE_LL1_MAX_NUM_UE_RX_ANT_LOG_PKT];  ///< TD buffer indices
                                                 ///< 20 is invalid -> Print "NA"
                                                 //
  /****************** Word 9-48******************/////< Only print if info_chg_cmd_type  == LFW_ARD_CTRL_CMD_C1RX_EVAL
  /// @internal
#ifdef FR63568_ARD_DYNAMIC_1RXTO2RX_THRESHOLD
  lte_LL1_log_dyn_c1rx_info_struct dyn_c1rx_info[LTE_LL1_DYN_C1RX_NUM_SF_WINDOW]; ///< Dynamic 1rx->2rx info

  /****************** Word 49-52******************/////< Only print if info_chg_cmd_type  == LFW_ARD_CTRL_CMD_C1RX_EVAL
  /// @internal
  int32 snr_th_coderate[LTE_LL1_MAX_NUM_AGG_LVL]; ///< Dynamic 1rx->2rx threshold raw dBQ8, print snr_threshold/256 in dB

  /****************** Word 53******************/////< Only print if info_chg_cmd_type  == LFW_ARD_CTRL_CMD_C1RX_EVAL
  int32 rsrp_min_threshold; ///< Dynamic 1rx->2rx rsrp min threshold raw dBQ7, print (rsrp_min_threshold/128 -180) in dBm

  /****************** Word 54******************/////< Only print if info_chg_cmd_type  == LFW_ARD_CTRL_CMD_C1RX_EVAL
  int32 rsrp_max_threshold; ///< Dynamic 1rx->2rx rsrp max threshold raw dBQ7,  print (rsrp_max_threshold/128 -180) in dBm
#endif

  /****************** Word 55-66 **************/  ///< AGC info, only print entries = num_rx_agc
  /// @internal
  lte_LL1_log_rx_agc_info_struct rxagc_table[LTE_LL1_MAX_NUM_UE_RX_ANT_LOG_PKT];

  /****************** Word 21 **************/    ///< Rx timeout value (max 64)
  /// @internal
  uint8 rx_timeout_value[LTE_LL1_MAX_NUM_UE_RX_ANT_LOG_PKT]; ///< Only print if info_chg_cmd_type  != LFW_ARD_CTRL_CMD_C1RX_EVAL

  /****************** Word 22 **************/
  /// @internal
  uint32 ard_mimo_action                  : 2;  ///< typedef enum
                                                ///< {
                                                ///<    // Current (No action)
                                                ///<    LTE_LL1_MIMO_CURRENT_POWER_MODE = 0,
                                                ///<    // LPM (Set MIMO buffer control to LPM)
                                                ///<    LTE_LL1_MIMO_LOW_POWER_MODE = 1,
                                                ///<    // HPM (Set MIMO buffer control to HPM)
                                                ///<    LTE_LL1_MIMO_HIGH_POWER_MODE = 2
                                                ///< } lte_LL1_sys_mimo_state_enum;
                                                ///< Only print if info_chg_cmd_type  != LFW_ARD_CTRL_CMD_C1RX_EVAL

  /// @internal
  boolean ard_mimo_soft_reset             : 1;  ///< (TRUE/FALSE)
  /// @internal
#ifdef FR63568_ARD_DYNAMIC_1RXTO2RX_THRESHOLD
  boolean condition_met_for_1rx           : 1; ///< (TRUE/FALSE)///< Only print if info_chg_cmd_type  = LFW_ARD_CTRL_CMD_C1RX_EVAL
  /// @internal
  boolean  eval_1rx                       : 1; ///< (TRUE/FALSE)///< Print always
  /// @internal
  uint8 rx_idx_c1rx                       : 2;
  ///@internal
  uint32  reserved                        : 25;
#else
  uint32  reserved                        : 29;
#endif


}lte_LL1_log_rx_ant_info_struct;

#endif //LTE_LL1_LOG_RXFE_H
