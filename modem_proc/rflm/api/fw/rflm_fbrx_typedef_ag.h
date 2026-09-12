
/*
WARNING: This file is auto-generated.

Generated at:    Wed Jun  1 13:23:16 2016
Generated using: fbrx_autogen.pl v1.0.0
Generated from:  v1.0.12 of Jacala_FBRxLM_Register_Settings.xlsx
*/

/*=============================================================================

          F B R X    A U T O G E N    F I L E

GENERAL DESCRIPTION
  This file is auto-generated and it captures the uK parameter settings for 
  different modes of FBRx operation provided by rflm_fbrx_settings

Copyright (c) 2009, 2010, 2011, 2012, 2013, 2014, 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

$DateTime: 2021/03/09 05:37:22 $
$Header: //components/rel/rflm.mpss/2.11/api/fw/rflm_fbrx_typedef_ag.h#1 $

=============================================================================*/

/*=============================================================================
                           REVISION HISTORY
Version    Author   Date   
         Comments   IR Number   Other POC (Comma Delimited)   Release Validation Notes   Dependencies   AG File Location   
1.0.12   Clarence Wong   4/28/2016   
         "Reduce NUM_VSWR_SKIP_SAMPLES from 512 to 256 for C2K.   This makes delay+VSWR_capture_length to fit within 104us constant power region for C2K.                  
Old delay(34us) + VSWR_length (512+512+512 @ 19.6608MHz = 78us) = 112us                           
New delay(34us) + VSWR_length (512+256+512 @ 19.6608MHz = 65us) = 99us"       ckwong, ankitaga   
         Recommended and validated by FBRx team    None   https://sharepoint.qualcomm.com/qct/Modem-Tech/Projects/Q6Silver/Tabasco/Systems/Common/RFPI/FBRxLM%20Spreadsheet/Jacala_FBRxLM_Register_Settings.xlsx?Web=1            
1.0.11   Clarence Wong   3/10/2016   
         Reduce N_CORR_LAGS from 32 to 16 for W.   Timeline optimization, expect ~100us savings.       ckwong, ankitaga   Recommended and validated by FBRx team    None   https://sharepoint.qualcomm.com/qct/Modem-Tech/Projects/Q6Silver/Tabasco/Systems/Common/RFPI/FBRxLM%20Spreadsheet/Jacala_FBRxLM_Register_Settings.xlsx?Web=1   
1.0.10   Clarence Wong   3/10/2016   
         Added TXROT_CLK_HZ in dynamic param.  RFSW to get this from RXLM, common FW need this to calcuate Tx freq rotation for W and TDS.       ckwong, ankitaga   Recommended and validated by FBRx team    None   https://sharepoint.qualcomm.com/qct/Modem-Tech/Projects/Q6Silver/Tabasco/Systems/Common/RFPI/FBRxLM%20Spreadsheet/Jacala_FBRxLM_Register_Settings.xlsx?Web=1   
1.0.9   Clarence Wong   3/2/2016   
         "Modified NUM_TX_IN_SAMPLES_C0    NUM_VSWR_SKIP_SAMPLES to be powers of 2    since Jacala Tx ref log length can only be powers of 2.            
NUM_REF_SAMPLES would be internally calculated by common FW for different proc_type.  Setting NUM_REF_SAMPLES = NUM_TX_IN_SAMPLES_C0 - 16 (initial 16 Rx samples are 0) as an upper bound for RFSW calculations on RMS and DC dBFS debug logging."       ckwong, ankitaga   
         Recommended and validated by FBRx team    None   https://sharepoint.qualcomm.com/qct/Modem-Tech/Projects/Q6Silver/Tabasco/Systems/Common/RFPI/FBRxLM%20Spreadsheet/Jacala_FBRxLM_Register_Settings.xlsx?Web=1            
1.0.8   Clarence Wong   2/23/2016   
         Moved PREDET_INDEX from static settings to power-dependent settings.   As we remove FBRX gain cal for LTE20/40, we can only populate the PREDET_INDEX from LTE10 cal to LTE20/40 FBRX NV settings.   Such PREDET_INDEX would be roughly accurate for LTE20/40, as we have seen slight differences due to sampling rate difference and tx_rx_delay tuning difference.  In order to improve the PREDET_INDEX accuracy for LTE20/40, we would like RFSW to dynamically and periodically update PREDET_INDEX upon a successful FBRX capture during online operations.       ckwong, ankitaga   Recommended and validated by FBRx team    None   https://sharepoint.qualcomm.com/qct/Modem-Tech/Projects/Q6Silver/Tabasco/Systems/Common/RFPI/FBRxLM%20Spreadsheet/Jacala_FBRxLM_Register_Settings.xlsx?Web=1   
1.0.7   Clarence Wong   2/17/2016   
         Added FBRX_NB_RATE_HZ to dynamic settings.   RFSW will calculate this based on VSRC output rate and the number of WB/NB decimator enabled.   Only need to calculate this once per Tx tune.       ckwong, ankitaga   Recommended and validated by FBRx team    None   https://sharepoint.qualcomm.com/qct/Modem-Tech/Projects/Q6Silver/Tabasco/Systems/Common/RFPI/FBRxLM%20Spreadsheet/Jacala_FBRxLM_Register_Settings.xlsx?Web=1   
1.0.6   Clarence Wong   2/9/2016   
         "Change UL_CARRIER_FREQ_HZ to INV_UL_CARRIER_FREQ_HZ    this moves the inverse calculation from FW to RFSW    where we only need to calculate this once per Tx tune.            
, ,    ankitaga"   Recommended and validated by FBRx team    
         None   https://sharepoint.qualcomm.com/qct/Modem-Tech/Projects/Q6Silver/Tabasco/Systems/Common/RFPI/FBRxLM%20Spreadsheet/Jacala_FBRxLM_Register_Settings.xlsx?Web=1               
1.0.5   Clarence Wong   2/3/2016   
         Added UL_CARRIER_FREQ_HZ in dynamic params.   This is needed by tech FW to calculate VSRC adjustment for FBRX capture (based on AFC correction factor and UL carrier freq).       ckwong, ankitaga   Recommended and validated by FBRx team    None   https://sharepoint.qualcomm.com/qct/Modem-Tech/Projects/Q6Silver/Tabasco/Systems/Common/RFPI/FBRxLM%20Spreadsheet/Jacala_FBRxLM_Register_Settings.xlsx?Web=1   
1.0.4   Clarence Wong   1/22/2016   
         "Moved PROCESSING_TYPE from dynamic to power dep.                  

=============================================================================*/

/*=============================================================================
                           INCLUDE FILES
=============================================================================*/


#ifndef RFLM_FBRX_TYPEDEF_AG_H
#define RFLM_FBRX_TYPEDEF_AG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "comdef.h"


/*==============================================================================

            EXTERNAL DEFINITIONS AND TYPES : MACROS

==============================================================================*/ 

#define RFLM_FBRX_RX_FILTER_TAPS_NUM 16


/*==============================================================================

            EXTERNAL DEFINITIONS AND TYPES : ENUMS

==============================================================================*/ 

typedef enum
{
  RFLM_FBRX_3X3 = 0, 
  RFLM_FBRX_RMS = 1, 
  RFLM_FBRX_RMS_3X3 = 2, 
}rflm_fbrx_algorithm_t;

typedef enum
{
  RFLM_FBRX_1_STAGE = 0, 
  RFLM_FBRX_2_STAGE = 1, 
}rflm_fbrx_upsample_stage_t;

typedef enum
{
  RFLM_FBRX_ENV_SCALE = 0, 
  RFLM_FBRX_IQ_SCALE = 1, 
}rflm_fbrx_es_iq_t;

typedef enum
{
  RFLM_FBRX_REAL_FILTER = 0, 
  RFLM_FBRX_COMPLEX_FILTER = 1, 
}rflm_fbrx_rx_filter_mode_t;

typedef enum
{
  RFLM_FBRX_RUN_POWER_MEAS_TYPE = 1, 
  RFLM_FBRX_RUN_VSWR_MEAS_TYPE = 2, 
  RFLM_FBRX_RUN_VSWR_PHASE_MEAS_TYPE = 3, 
  RFLM_FBRX_RUN_SWPT_MEAS_TYPE = 4, 
  RFLM_FBRX_RUN_NO_MEAS_TYPE = 255, 
}rflm_fbrx_meas_req_type_e;


/*==============================================================================

            EXTERNAL DEFINITIONS AND TYPES : STRUCTURES
     ANY CHNAGES TO THIS SECTION REQUIRES FBRX CMN FW INTERFACE CHNAGES 
==============================================================================*/ 

typedef struct
{
  uint16 threshold_xcorr_1; /* Threshold for TA |Xcor| uniformity (Test 1: small RB case) */ 
  uint32 rx_filter_taps[RFLM_FBRX_RX_FILTER_TAPS_NUM]; /* Sc16t time domain taps for RX filter. Thor typically use 8 or 10-tap filters.  Enough to keep 16 taps (also done in Atlas).  NV supports 32 taps though. */ 
  uint16 num_rx_taps; /* num_rx_taps */ 
  rflm_fbrx_rx_filter_mode_t rx_filter_mode; /* rx_filter_mode */ 
  uint8 txfe_index; /* TxFE index:  0 = TxFE-0, 1 = TxFE-1 */ 
  boolean ca_enable; /* Enable Carrier Aggregation , 0: Disable, 1 : Enable; Likely to use RMS ratio */ 
  rflm_fbrx_upsample_stage_t tx_num_upsample_stages; /* TDS might need upsampling since TDS TX ref log rate is 10.24 and FBRX rate is 20.48 */ 
  uint8 num_corr_lags; /* Number of Time Alignment Cross Correlations . Can be only 4,8,16,32 */ 
  uint16 num_tx_in_samples_c0; /* Number of samples to be read from DTR-IB for ca0; NUM_TX_IN_SAMPLES_C0 >= NREF + NUM_CORR_LAGS - 1; */ 
  uint16 num_ref_samples; /* number of rx samples; NUM_RX_IN_SAMPLES = NUM_RX_SAMPLE + NUM_RX_TAPS - 1; common FW would internally calculate this; setting this to worst case for RFSW RMS/DC calculations */ 
  uint16 num_vswr_skip_samples; /* Number of samples to discard between VSWR incident capture and VSWR reflected capture.   Need RFSW profiling on the coupler switch timing uncertainty.   Budget is 15us ~ 500 @ 30.72. */ 
  boolean apply_tau_en; /* 0: Do not apply Tau,  1: Apply Tau */ 
  uint16 threshold_gain; /* Threshold for GAIN Calculation */ 
  uint16 threshold_rsb; /* Threshold for RSB Calculation */ 
  uint16 threshold_loft; /* Threshold for LOFT Calculation */ 
  int16 tx_sample_offset; /* TX sample offset */ 
  int16 rx_sample_offset; /* RX sample offset */ 
  uint32 reserved; /* reserved fields for future use */ 
}rflm_fbrx_static_param_t;

typedef struct
{
  uint32 txc_input_freq_hz; /* RFSW to get this from TXLM; CFW need this for TX capture timing calculation */ 
  uint32 txc_rollover_period_ns; /* RFSW to get this from TXLM; CFW need this for TX capture timing calculation */ 
  uint8 txc_ref_log_down_sample; /* RFSW to get this from TXLM; CFW need this for TX capture timing calculation */ 
  uint32 inv_ul_carrier_freq_hz; /* On Jacala, all tech FW need to adjust VSRC for the FBRX capture, which involves inverse of ul_carrier_freq_hz, Q32 format */ 
  uint32 fbrx_nb_rate_hz; /* RFSW to compute from RXLM; FBRX_NB_RATE_HZ = VSRC_OUTPUT_RATE_HZ >> NUM_WB_NB_DEC; CommonFW needs this for capture timing calculations */ 
  uint32 txrot_clk_hz; /* RFSW to get this from TXLM; commonFW need this for Tx ref log freq rotation for W and TDS */ 
  uint32 reserved; /* reserved fields for future use */ 
}rflm_fbrx_dynamic_param_t;

typedef struct
{
  uint32 gain_fbrx; /* fbrx expected gain based on external cal */ 
  uint16 threshold_ls; /* Threshold for Total LS Error */ 
  uint32 current_gain; /* Current Gain compensation */ 
  uint32 current_loft; /* Current LOFT compensation */ 
  uint32 current_rsb; /* Current RSB compensation */ 
  uint32 current_gain_imbalance; /* Current gain imbalance estimate */ 
  uint32 current_phase_imbalance; /* Current phase imbalance estimate */ 
  uint8 input_bias; /* Input bias to be applied to format conversion from FBRx buffer. Thor used this for 20PCFL4->16SC conversion.  Jacala FBRX samples are directly captured in 16SC. */ 
  rflm_fbrx_es_iq_t es_iq_sel; /* Select envelope scaling or IQ scaling */ 
  uint16 scale_rx; /* Rx scale factor */ 
  uint16 thresh_gain_high; /* threshold gain high, used in W clamping region */ 
  uint16 thresh_gain_low; /* threshold gain low */ 
  uint16 scale_tx; /* scale_tx 0x7FFF */ 
  rflm_fbrx_meas_req_type_e meas_type; /* 1: power measurement, 2: VSWR Magnitude, 3: VSWR Mag/Phase, 4: Switchpoint dual capture, 0xFF: no type. Jacala only uses capture id 1 and 3. */ 
  rflm_fbrx_algorithm_t processing_type; /* Select processing type : 3x3, or RMS Ratio, RMS+3x3 */ 
  boolean txfe_update; /* Immediate DTR update flag for baseband gain correction. VSWR and Switchpoint dual captures don't do updates. */ 
  uint32 input_capture_id; /* Input Capture ID, unique id associatd with each fbrx capture. Useful to detect any mismatch inbetween the requested capture and retuned fbrx results */ 
  boolean estimate_tau_en; /* 0: use predetermined Tau , 1 : Estimate new Tau.  Toggle based on num of RB's in LTE. */ 
  uint16 predet_index; /* shift in Tx samples to be applied if time-alignment is disabled */ 
  uint32 reserved; /* reserved fields for future use */ 
}rflm_fbrx_pwr_dependent_param_t;

typedef struct
{
  rflm_fbrx_static_param_t fbrx_static_param;
  rflm_fbrx_dynamic_param_t fbrx_dynamic_param;
  rflm_fbrx_pwr_dependent_param_t fbrx_pwr_dependent_param;
}rflm_fbrx_struct_type_ag;

extern rflm_fbrx_struct_type_ag rflm_fbrx_c2k_txfe_dac_in_rate_19p6608mhz_230p4mhz; 
extern rflm_fbrx_struct_type_ag rflm_fbrx_lte20_txfe_dac_in_rate_30p72mhz_230p4mhz; 
extern rflm_fbrx_struct_type_ag rflm_fbrx_lte40_txfe_dac_in_rate_61p44mhz_230p4mhz; 
extern rflm_fbrx_struct_type_ag rflm_fbrx_lte10_txfe_dac_in_rate_30p72mhz_230p4mhz; 
extern rflm_fbrx_struct_type_ag rflm_fbrx_w_txfe_dac_in_rate_30p72mhz_230p4mhz; 
extern rflm_fbrx_struct_type_ag rflm_fbrx_tds_txfe_dac_in_rate_10p24mhz_230p4mhz; 

#ifdef __cplusplus
}
#endif



#endif


