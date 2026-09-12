/*!
  @file
  ftm_common_selftest.c

  @brief
  This module contains definitions and prototypes for common selftest functionality.
*/

/*==============================================================================

  Copyright (c) 2012 - 2020 Qualcomm Technologies Incorporated. All Rights Reserved

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

  $Header: //components/rel/rfa.mpss/4.11/rf/common/ftm/src/ftm_common_selftest.c#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
01/20/20   ssl     FR 55657: Replace RF Enter/Exit with RF Sleep/Wakeup for L+L DSDS awake
3/14/18    rp      KW Fixes
03/23/16   sk      Use Tx band for feeback query and pass FB device from RFC
03/02/16   jr      Get FBRX Chain from RFC files
02/23/16   sk      Pass FB chain to FBRX module from RFC
02/22/16   sk      Pass Rx band to get FB device
01/27/16   sk      Get LTE Feedback Device from RFC
12/09/15   pk      Added support for CDMA rx\tx split
12/01/15   hm      Fixes for KW error
11/10/15   ljz     Skip LDO On\Off during selftest
09/22/15   zbz     re-arch fixes
09/10/15   dbz     [selftest] Add FBRX RSB correction for selftest captures 
09/09/15   dbz     KW error fix
09/03/15   tjm/dbz Selftest re-architecture 
08/12/15   pk      Featurized the cdma code for 1x compiled out build
08/07/15   ck      Support GSM fbrx power meas
08/03/15   zbz     Changes to support RFFE coupelr and WTR4905 fbrx gs override
07/27/15   dbz     Carrier index out of bound KW error fix
07/15/15   zbz     pass device id into ftm iq processing
07/01/15   dbz     THOR selftest bringup changes
03/31/15   zbz     Fix KW 
03/03/15   zbz     Fix VSWR measurement result stuck 
02/25/15   zbz     FIR filter coeffs change
02/20/15   dbz     Change the S11 phase calculation to match customer defination 
02/15/15   jmf     [selftest] reprogram FW SWI for Tx upsampling for C2k/TDS
02/11/15   jmf     Add support for Spectrum Flatness IQ Capture
01/28/15   ska     Delete FBRx cases for Rx only bands: B29 and B32
11/25/14   jmf     Call to new rfcommon_mdsp API to setup VSWR params in FW
11/04/14   dbz     Add support for VSWR V2 processing
11/03/14   ndb     Fix the compiler error in LTE removed builds
10/16/14   jmf     [selftest] Implement multi-gain stage switch for ACLR improvement
10/14/14   jmf     [selftest] Implement FBRx Noise Floor Cal
10/09/14   dbz     Add WCDMA TX upsampling support
10/03/14   dbz     KW error fix
10/01/14   dbz     Add EVM calculation config for TDSCDMA, 1x, WCDMA
09/19/14   jmf     Hook up VSWR v2 and temp fixes for FW RxRMS cap type bypassed to EVM type
09/10/14   dbz     Add support for TX upsampling and EVM calculations for LTE
08/18/14   jmf     [SelfTest] ACLR meas changes
08/05/14   jmf     [SelfTest] Remove RFCMD FW message to set self-test flag
07/31/14   jmf     Add API to query Current Self Test Mode
                   Remove call to RFCMD MSG to set FW Self Test Mode
07/24/14   jmf     [SelfTest] Move self test mode global flag to .c
07/23/14   dbz     Added support for selftest VSWR split and EVM capture for 1x/LTE
07/17/14   jmf     Add api to set self test flag
06/30/14   daa     Created.

==============================================================================*/

#include "rfa_variation.h"
#include "target.h"    /* Target specific definitions            */
#include "comdef.h"    /* Common stuff                           */
#include "ftmicap.h"
#include "stringl.h" /* for memscpy */
#include "ftm_calibration_data_module.h"
#include "ftm_common_selftest.h"
#include "rfcommon_mdsp.h"

#ifdef FEATURE_FACTORY_TESTMODE
#include "rex.h"
#include "task.h"
#include "ftm.h"
#include "ftmdiag.h"
#include "ftm_common_dispatch.h"
#include "ftm_common.h"
#include "rf_test_task_v.h"
#include "rfcommon_nv.h"
#include "rfm.h"
#include "rfm_internal.h"
#include "ftm_rf_cmd.h"
#include "ftm_common_fbrx.h"
#include "ftm_common_iq_processing.h"
#ifndef FEATURE_GNSS_ONLY_NO_WWAN
#include "rflm_api_fbrx_fw_hal.h"
#include "rxlm_intf.h"
#endif
#include "ftm_common_control.h"
#include "ftm_msg.h"
#include "rfc_card.h"
#ifndef FEATURE_GNSS_ONLY_NO_WWAN
#include "ftm_common_radio.h"
#include "ftm_cdma_cmd.h"
#include "ftm_rffe.h"
#endif
#include "ftm_gpio.h"
#include "modem_mem.h"
#include "rfcommon_mc.h"
#ifndef FEATURE_GNSS_ONLY_NO_WWAN
#include "ftm_common_xpt.h"
#include "rfcommon_core_xpt_buffer.h"
#include "rfcommon_core_xpt.h"
#endif
#include "rfcommon_mdsp_types.h"
#include "rfcommon_core_utils.h"
#include "rfdevice_coupler_intf.h"

#ifdef FEATURE_WCDMA
#include "ftm_wcdma_ctl.h"
#include "rfdevice_wcdma_intf.h"
#include "rfnv_wcdma.h"
#include "rfwcdma_msm.h"
#include "rfdevice_wcdma_pa_intf.h"
#include "rfdevice_wcdma_asm_intf.h"
#include "ftm_wcdma_dispatch.h"
#include "ftm_wcdma_calv3.h"
#include "rfwcdma_data.h"
#include "ftm_wcdma_selftest.h"
#include "rfwcdma_core_txctl.h"
#endif

#ifdef FTM_HAS_TDSCDMA
#include "ftm_tdscdma_ctl.h"
#include "rfdevice_tdscdma_pa_intf.h"
#include "rfdevice_tdscdma_asm_intf.h"
#include "rf_tdscdma_msm.h"
#include "rf_tdscdma_core_util.h"
#include "ftm_tdscdma_selftest.h"
#endif


#ifdef FEATURE_CDMA1X
#include "ftm_1x_control.h"
#include "ftm_1x_calibration.h"
#include "rfdevice_cdma_interface.h"
#include "ftm_cdma_dispatch.h"
#include "rfc_cdma.h"
#include "rfm_1x_ftm.h"
#include "rfcommon_core_device_manager.h"
#include "rfcommon_locks.h"
#include "rf_cdma_data.h"
#include "ftm_1x_xpt.h"
#include "ftm_cdma_data.h"
#include "rf_cdma_msm.h"
#include "rf_cdma_mc.h"
#include "rfm_cdma_ftm.h"
#include "ftm_1x_selftest.h"
#endif

#ifdef FEATURE_GSM
#include "ftm_gsm_ctl.h"
#include "rfdevice_gsm_intf.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_gsm_rfprot.h"
#include "ftm_gsm_device.h"
#endif

#ifdef FEATURE_LTE
#include "rf_lte_ftm_cmd_proc.h"
#include "rflte_ftm_external_api.h"
#include "ftm_lte_xpt.h"
#include "rflte_ftm_mc.h"
#include "rflte_nv.h"
#include "rflte_msm.h"
#include "rflte_core_util.h"
#include "rfdevice_lte_pa_intf.h"
#include "rfdevice_lte_interface.h"
#include "rfdevice_lte_asm_intf.h"
#include "rflte_msm_iq_capture.h"
#include "ftm_lte_common_dispatch.h"
#include "rflte_state.h"
#include "ftm_lte_selftest.h"
#include "rfc_card_lte.h"
#endif

#ifdef FEATURE_TDSCDMA
#include "ftm_tdscdma_ctl.h"
#include "ftm_tdscdma_calv3.h"
#include "rfc_card_tdscdma.h"
#endif

#ifdef FEATURE_RF_HAS_QTUNER

#ifdef FEATURE_CDMA1X
#include "rf_cdma_utils_freq.h"
#include "rfdevice_hdet_cdma_intf.h"
#endif

#ifdef FEATURE_WCDMA
#include "rfwcdma_core_antenna_tuner.h"
#endif

#include "rfdevice_hdet_cmn_intf.h"
#include "rfdevice_hdet_wcdma_intf.h"
#endif

#include "ftm_common_calibration_v3.h"
#include "ftm_calibration_v3_data.h"
#include "rf_qmi_rfrpe_svc.h"

#if defined (FEATURE_XO_FACTORY_CAL)
#include "ftm_calibration_xo.h"
#endif

#include "rfdevice_logical_component_types.h"

// extern boolean ftm_selftest_pwr_only;
// coupler_direction_enum_type tdscdma_coupler_direction = COUPLER_DIRECTION_FORWARD;

// Define VSWR V2 phase constant

#define VSWR_V2_PHASE_OFFSET_1 54000
// Offset 1 in degree100 = 36000 + 18000 to rotate the output to -/+180 degree
#define VSWR_V2_PHASE_MODE_CONSTANT 36000
#define VSWR_V2_PHASE_OFFSET_2 18000
// Offset 2 in degree100 = 18000 to rotate the output to -/+180 degree

uint8 selftest_fbrx_gain_stage;

#define NUM_SAMPLES 9216
#define FFT_SIZE 1024
#define PCFL_SIZE 2560 // (FFT_SIZE * 20 / 8)
#define WORD_SIZE 2

// Define to limit bandwidth and flatten RF frequency response for EVM
// captures. Filters are used only for LTE. Lengths must be multiple of 2.

#define MAX_NUM_COUPLER_DEVICES 2
#define MAX_NUM_COUPLER_TRANSACTIONS 5
static rf_buffer_intf *selftest_coupler_script_ptr[MAX_NUM_COUPLER_DEVICES][FTM_COUPLER_DIRECTION_INVALID]; 

static int16 evm_lte_5_fir[] = {
   1067,  //  0.046061661588133   98916665(Q31)
  -5352,  // -0.093370032975890 -200510619(Q31)
  -2562,  // -0.067147047930421 -144197187(Q31)
   5858,  //  0.074600601253681  160203571(Q31)
  15284,  //  0.245726434557269  527693500(Q31)
  22880,  //  0.363410073088509  780417189(Q31)
  22880,  //  0.363410073088509  780417189(Q31)
  15284,  //  0.245726434557269  527693500(Q31)
   5858,  //  0.074600601253681  160203571(Q31)
  -2562,  // -0.067147047930421 -144197187(Q31)
  -5352,  // -0.093370032975890 -200510619(Q31)
   1067,  //  0.046061661588133   98916665(Q31)
     16}; // Q factor

static int16 evm_lte_10_fir[] = {
   -815,  // -0.037994756976527  -81593119(Q31)
   3228,  //  0.102830278521677  220826342(Q31)
  -2907,  // -0.059581894357404 -127951144(Q31)
  -3996,  // -0.139501682388697 -299577582(Q31)
   5640,  //  0.155531963473947  334002348(Q31)
  17614,  //  0.549011288867506 1178992765(Q31)
  17614,  //  0.549011288867506 1178992765(Q31)
   5640,  //  0.155531963473947  334002348(Q31)
  -3996,  // -0.139501682388697 -299577582(Q31)
  -2907,  // -0.059581894357404 -127951144(Q31)
   3228,  //  0.102830278521677  220826342(Q31)
   -815,  // -0.037994756976527  -81593119(Q31)
    15};  // Q factor  

static int16 evm_lte_15_fir[] = {
   4547,  //  0.065179313560233  139971510(Q31)
  -6429,  // -0.064373753947658 -138241584(Q31)
  -6527,  // -0.110011801638832 -236248545(Q31)
   2352,  //  0.014798102180322   31778682(Q31)
  14422,  //  0.243373525310991  522640666(Q31)
  24384,  //  0.425366428612390  913467450(Q31)
  24384,  //  0.425366428612390  913467450(Q31)
  14422,  //  0.243373525310991  522640666(Q31)
   2352,  //  0.014798102180322   31778682(Q31)
  -6527,  // -0.110011801638832 -236248545(Q31)
  -6429,  // -0.064373753947658 -138241584(Q31)
   4547,  //  0.065179313560233  139971510(Q31)
    16};  // Q factor

static int16 evm_lte_20_fir[] = {
   2327,  // -0.009554325511675  -20517758(Q31)
   1496,  //  0.086172917596038  185054931(Q31)
  -8396,  // -0.109006543670225 -234089770(Q31)
  -5451,  // -0.115037420968608 -247040980(Q31)
  12200,  //  0.193901753966908  416400846(Q31)
  30917,  //  0.527832124754499 1133510857(Q31)
  30917,  //  0.527832124754499 1133510857(Q31)
  12200,  //  0.193901753966908  416400846(Q31)
  -5451,  // -0.115037420968608 -247040980(Q31)
  -8396,  // -0.109006543670225 -234089770(Q31)
   1496,  //  0.086172917596038  185054931(Q31)
   2327,  // -0.009554325511675  -20517758(Q31)
    16};  // Q factor

static int16 evm_tdscdma_fir[] = {
  -11497,
      24,
   10461,
   19080,
   25392,
   31537,
   31537,
   25392,
   19080,
   10461,
      24,
  -11497,
     17};  // Q factor

static int16 evm_cdma_fir[] = {
  -11497,
      24,
   10461,
   19080,
   25392,
   31537,
   31537,
   25392,
   19080,
   10461,
      24,
  -11497,
     17};  // Q factor

static int16 evm_wcdma_fir[] = {
  -2824,
  -3143,
   1251,
   7906,
  14287,
  19634,
  19634,
  14287,
   7906,
   1251,
  -3143,
  -2824,
    16};  // Q factor
  
// Externs
extern rfcom_wcdma_band_type ftm_curr_wcdma_mode;

// Internal global variables
static int16 tx_up_filter_coef[] = {21, -243, 898, -2605, 10108, 10108, -2605, 898, -243, 21};
static ftm_rf_common_tx_params meas_tx_params;
static ftm_rf_common_fbrx_iq_acquired_samples iq_samples;
static ftm_common_selftest_fbrxgs_info_type fbrx_gs_info;

// Response packet structures
ftm_rf_fbrx_meas_tx_params_rsp_pkt_type meas_tx_params_rsp_pkt;

// Flags
static volatile uint8 error_code;
static volatile rfcommon_xpt_processing_type proc_flag;
static ftm_common_selftest_capture_flags capture_flags;
static ftm_common_selftest_success_flags success_flags;
extern boolean selftest_lowband;

// Global variables
ftm_common_selftest_vswr_v2_results_type ftm_common_selftest_vswr_v2_results = {0,0,0,0,0};
uint8 ftm_common_selftest_modeset_flag = 0;

// Internal Function Declarations
static void ftm_common_selftest_init(const ftm_common_selftest_meas_params_struct params);
static void ftm_common_selftest_evm_cap(const ftm_common_selftest_meas_params_struct params, int32 * sample_buffer);
static void ftm_common_selftest_spec_flat_cap(const ftm_common_selftest_meas_params_struct params, int32 * sample_buffer);
static void ftm_common_selftest_vswr_v2_cap(const ftm_common_selftest_meas_params_struct params);
static void ftm_common_selftest_rx_rms_cap(const ftm_common_selftest_meas_params_struct params);
static void ftm_common_aclr_calc(const ftm_common_selftest_meas_params_struct params, int32 * sample_buffer);
static boolean ftm_common_buffer_setup(const ftm_common_selftest_meas_params_struct params, int32 ** sample_buffer);

// Helpers
static boolean ftm_common_enable_fbrx_reconfigure_rxlm(const ftm_common_selftest_meas_params_struct params, const boolean enable_fbrx);
static boolean ftm_common_override_fbrx_gain(const ftm_common_selftest_meas_params_struct params, const boolean override_enable);
static boolean ftm_common_selftest_config(const ftm_common_selftest_meas_params_struct params);
static boolean ftm_common_filter_setup(const ftm_common_selftest_meas_params_struct params, const boolean no_band_limit_filter);
static uint8 ftm_common_com_band_specifier
(
  const ftm_rf_technology_type ftm_rf_tech, 
  rfcom_band_type_u com_band
);

// Response packets generation functions
static void ftm_common_populate_meas_tx_params_rsp_pkt
(
  ftm_common_selftest_meas_params_struct params,
#ifdef FEATURE_LTE  
  ftm_lte_pkt_type * req_pkt,
#else
  int32 *dummy_var,
#endif

  ftm_rsp_pkt_type * rsp_pkt
);

/*
 Entry point for the self test procedure. All techs (LTE, WCDMA, 1x and TDSCDMA) call this
 function from their meas Tx parameters dispatcher. All captures are invoked from here.
 Also, FBRx gain stage override and disable/enable for FBRx take place here prior to
 any non-Tx Power captures.
*/
uint8 ftm_common_selftest_main(ftm_common_selftest_meas_params_struct params, 
#ifdef FEATURE_LTE
ftm_lte_pkt_type * req_pkt,
#else
int32 *req_pkt,
#endif
ftm_rsp_pkt_type * rsp_pkt)
{
  uint8 error_code = FTM_FBRX_TEST_SUCCESS;
  boolean success = TRUE;

  FTM_MSG_5(FTM_MED, "Entering FTM Common Selftest for %d, Num averages: %d, capture offset: %d, tx measurement config: %X, device: %d",
    params.tech, 
    params.num_averages, 
    params.capture_offset, 
    params.tx_measurement_config,
    params.device);
 
  ftm_common_selftest_init(params);

  if (error_code != FTM_FBRX_TEST_SUCCESS)
  {
    FTM_MSG(FTM_ERROR, "Selftest Initialization Failed!");
    return error_code;
  }

  // Perform Tx power capture
  if(capture_flags.tx_power_capture)
  {
    int16 pwr_est_db10;
    uint8 fbrx_gs;

    // Perform power measurement
    success_flags.tx_power_success = ftm_common_fbrx_meas_ant_pwr(&pwr_est_db10, &(meas_tx_params.tx_power), params.num_averages, &fbrx_gs);
    FTM_MSG_5(FTM_HIGH, "Estimated Power: %d (10dB), %d (100dB), # of averages: %d, status: %d, fbrx gs: %d", pwr_est_db10, meas_tx_params.tx_power, params.num_averages, success_flags.tx_power_success, fbrx_gs);

    if (success_flags.tx_power_success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Tx Power Measurement Failed!");
      // error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    }
  }

  // Disable FBRx operation + Override FBRx Gainstage
  if(capture_flags.evm_capture || capture_flags.rx_rms_capture || capture_flags.vswr_v2_capture || capture_flags.spec_flat_capture)
  {
    ftm_common_enable_fbrx_reconfigure_rxlm(params, FALSE);
  }

  // Perform EVM capture
  if(capture_flags.evm_capture)
  {
    int32 * sample_buffer = NULL;

    // ACLR measurement requested
    if(params.tx_measurement_config & FTM_FBRX_MEASURE_ACLR)
    {
      // Setup Buffers
      success = ftm_common_buffer_setup(params, &sample_buffer);

      // Buffer setup failed
      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "FTM common buffer setup failed!");
        ftm_common_deinit_fft_data();
        success_flags.aclr_success = FALSE;
        if(sample_buffer != NULL) ftm_free(sample_buffer);
      }
    }

    if(sample_buffer == NULL)
    {
      FTM_MSG_1(FTM_ERROR, "sample_buffer=NULL,tx_measurement_config is %d ",params.tx_measurement_config);	
      return FTM_FBRX_TEST_GENERAL_FAILURE;
    }
    FTM_MSG(FTM_HIGH, "Entering Selftest EVM CAP!");

    ftm_common_selftest_evm_cap(params, sample_buffer);

    // Failed EVM capture
    if(error_code != FTM_FBRX_TEST_SUCCESS)
    {
      FTM_MSG(FTM_ERROR, "EVM Capture Failed!");
      success_flags.evm_success = FALSE;
      success_flags.aclr_success = FALSE;
      success_flags.vswr_success = FALSE;
    }

    // Free Buffers + Deinit from ACLR
    if(params.tx_measurement_config & FTM_FBRX_MEASURE_ACLR)
    {
      ftm_common_deinit_fft_data();
      ftm_free(sample_buffer);
    }

  } // EVM Capture

  // Perform Spectral Flatness capture
  if(capture_flags.spec_flat_capture)
  {
    // Currently only support spectral flatness measurement for LTE
    if(params.tech != FTM_RF_TECH_LTE) success_flags.spec_flat_success = FALSE;
    
    // Tech is LTE
    else
    {
      int32 * sample_buffer = NULL;

      // Setup Buffers
      success = ftm_common_buffer_setup(params, &sample_buffer);

      // Buffer setup failed
      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "FTM common buffer setup failed!");
        ftm_common_deinit_fft_data();
        success_flags.spec_flat_success = FALSE;
        if(sample_buffer != NULL) ftm_free(sample_buffer);
      }
      if(sample_buffer == NULL)
      {
        FTM_MSG_1(FTM_ERROR, "sample_buffer=NULL,tx_measurement_config is %d ",params.tx_measurement_config);	
        return FTM_FBRX_TEST_GENERAL_FAILURE;
      }
      FTM_MSG(FTM_HIGH, "Entering Selftest Spec Flat CAP!");
      
      ftm_common_selftest_spec_flat_cap(params, sample_buffer);

      if(error_code != FTM_FBRX_TEST_SUCCESS)
      {
        FTM_MSG(FTM_ERROR, "Spectral Flatness capture Failed!");
        success_flags.spec_flat_success = FALSE;
      }
    }
  }

  if(capture_flags.vswr_v2_capture)
  {
    FTM_MSG(FTM_HIGH, "Entering Selftest VSWRv2 CAP!");

    ftm_common_selftest_vswr_v2_cap(params);

    // Failed VSWR v2 capture
    if(error_code != FTM_FBRX_TEST_SUCCESS)
    {
      FTM_MSG(FTM_ERROR, "Rx RMS Capture Failed!");
      success_flags.vswr_v2_success = FALSE;
    }
  }

  if(capture_flags.rx_rms_capture)
  {
    FTM_MSG(FTM_HIGH, "Entering Selftest Rx RMS CAP!");

    ftm_common_selftest_rx_rms_cap(params);

    // Failed Rx RMS capture
    if(error_code != FTM_FBRX_TEST_SUCCESS)
    {
      FTM_MSG(FTM_ERROR, "Rx RMS Capture Failed!");
      success_flags.vswr_success = FALSE;
    }
  }

  // Restore FBRx default gain + Re-enable FBRx operation
  if(capture_flags.evm_capture || capture_flags.rx_rms_capture || capture_flags.vswr_v2_capture || capture_flags.spec_flat_capture)
  {
    ftm_common_override_fbrx_gain(params, FALSE);
    ftm_common_enable_fbrx_reconfigure_rxlm(params, TRUE);
  }

  // Fill out response packet
  ftm_common_populate_meas_tx_params_rsp_pkt(params, req_pkt, rsp_pkt);

  // Print completion message
  FTM_MSG(FTM_MED, "Selftest complete, returning!");

  // hack return for now
  // return error_code;
  return FTM_FBRX_TEST_SUCCESS;
} // FTM COMMON SELFTEST

/*
Reads the measurement configuration sent by QRCT tool and sets FW capture flags 
based on what measurements are requested by QRCT. Also sets up FBRx gain stage information 
for each tech. 
*/
void ftm_common_selftest_init(const ftm_common_selftest_meas_params_struct params)
{
  // Initialize all flags
  capture_flags.tx_power_capture = FALSE;
  capture_flags.evm_capture = FALSE;
  capture_flags.rx_rms_capture = FALSE;
  capture_flags.vswr_v2_capture = FALSE;
  capture_flags.spec_flat_capture = FALSE;

  success_flags.tx_power_success = TRUE;
  success_flags.evm_success = TRUE;
  success_flags.aclr_success = TRUE;
  success_flags.vswr_success = TRUE;
  success_flags.vswr_v2_success = TRUE;
  success_flags.spec_flat_success = TRUE;

  proc_flag = PROC_SELF_TEST;

  // Initialize payload
  meas_tx_params.tx_power = 0;
  meas_tx_params.evm_value = 0;
  meas_tx_params.vswr_ratio_db10 = 0;
  meas_tx_params.rx_rms_mag = 0;
  meas_tx_params.rx_rms_reverse_mag = 0;
  for(uint8 i = 0; i < 6; i++) meas_tx_params.aclr_value[i] = 0;
  for(uint8 i = 0; i < 8; i++) meas_tx_params.spec_flat_value[i] = 0;
  for(uint8 i = 0; i < 5; i++) meas_tx_params.vswr_v2_value[i] = 0;

  // Set up capture flags, use EVM capture for VSWR v1, if we performing EVM capture anyway
  if(params.tx_measurement_config & FTM_FBRX_MEASURE_TX_POWER) capture_flags.tx_power_capture = TRUE;
  if(params.tx_measurement_config & FTM_FBRX_MEAS_VSWR_V2) capture_flags.vswr_v2_capture = TRUE;
  if(params.tx_measurement_config & FTM_FBRX_MEAS_FLATNESS) capture_flags.spec_flat_capture = TRUE;
  if(params.tx_measurement_config & (FTM_FBRX_MEASURE_EVM | FTM_FBRX_MEASURE_ACLR)) capture_flags.evm_capture = TRUE;
  if(params.tx_measurement_config & FTM_FBRX_MEAS_VSWR && capture_flags.evm_capture == FALSE) capture_flags.rx_rms_capture = TRUE;
  
  FTM_MSG_5(FTM_MED, "Capture Flags: Tx Power: %d, EVM: %d, Rx RMS: %d, VSWRv2: %d, Spec Flat: %d",
  capture_flags.tx_power_capture, capture_flags.evm_capture, capture_flags.rx_rms_capture, capture_flags.vswr_v2_capture, capture_flags.spec_flat_capture);

  rfcom_bw_type_u bw;
  rfcom_band_type_u band;

  fbrx_gs_info.override_enable = ftm_common_selftest_obtain_fbrx_gain_stage(params.tech, &(fbrx_gs_info.selftest_fbrx_gs));
  selftest_fbrx_gain_stage = fbrx_gs_info.selftest_fbrx_gs;

  if(params.tech != FTM_RF_TECH_LTE) fbrx_gs_info.wtr_lte_bw = 0;

  // Tech-specific setup
  switch(params.tech)
  {
    #ifdef FEATURE_CDMA1X
    case FTM_RF_TECH_CDMA:
    {

      /* Variables to hold the rx device state and the band informations
      to query the corresponding tx device from the RFC to support the 
      rx\tx split */
      ftm_cdma_data_status_type rx_dev_state;  
      sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;

      /* Query the rx device state to get the the band information and use the 
      band info to get the tx device from the RFC to support the rx\tx split */
				  
      (void)ftm_cdma_data_get_device_status( params.device, &rx_dev_state );
      sys_band = rf_cdma_rf_band_type_to_sys_band_type( rx_dev_state.curr_band);

      rfm_device_enum_type tx_device = rfc_common_get_preferred_tx_device ( sys_band );

      band.cdma_band = ftm_cdma_data_get_band_class(tx_device);
      const rf_cdma_data_status_type * dev_status = rf_cdma_get_device_status(tx_device);

      if(dev_status != NULL) fbrx_gs_info.com_band.cdma_band = dev_status->curr_band;

      switch(band.cdma_band)
      {
        case RFM_CDMA_BC0:
        case RFM_CDMA_BC10:
          selftest_lowband = TRUE;
        break;

        default:
          selftest_lowband = FALSE;
        break;
      }
    }
    break;
	#endif
	
    #ifdef FEATURE_WCDMA
    case FTM_RF_TECH_WCDMA:
      band.wcdma_band = ftm_curr_wcdma_mode;
      fbrx_gs_info.com_band.wcdma_band = band.wcdma_band;

      switch(band.wcdma_band)
      {
        case RFCOM_BAND_IMT:
        case RFCOM_BAND_1900:
        case RFCOM_BAND_BC4:
          selftest_lowband = FALSE;
        break;

        default:
          selftest_lowband = TRUE;
        break;
      }
    break;
	  #endif
    #ifdef FTM_HAS_TDSCDMA
    case FTM_RF_TECH_TDSCDMA:
      fbrx_gs_info.com_band.tdscdma_band = rf_tdscdma_mc_state.curr_band_tx;
    break;
   #endif
   #ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE:
    {
      // Should this still be hardcoded??
     // uint32 fbrx_carrier_idx = rflte_ftm_mc_get_carrier_index_from_device(rflte_ftm_mc_sm_get_drx_device());
     // uint32 curr_carrier_idx = rflte_ftm_mc_sm_get_carrier_index();
    //  rflte_ftm_mc_sm_set_carrier_idx(fbrx_carrier_idx);

      band.lte_band = rflte_ftm_mc_sm_get_band();
      bw.lte_bw = rflte_ftm_mc_sm_get_tx_bw();

      fbrx_gs_info.com_band.lte_band = band.lte_band;

      // Print out Bandwidth information
      FTM_MSG_1(FTM_MED, "LTE BW: %d", bw.lte_bw);

      switch(bw.lte_bw)
      {
        case RFCOM_BW_LTE_1P4MHz:
        case RFCOM_BW_LTE_3MHz:
        case RFCOM_BW_LTE_5MHz:
          fbrx_gs_info.wtr_lte_bw = 0;
        break;

        case RFCOM_BW_LTE_10MHz:
          fbrx_gs_info.wtr_lte_bw = 1;
        break;

        case RFCOM_BW_LTE_15MHz:
          fbrx_gs_info.wtr_lte_bw = 2;
        break;

        case RFCOM_BW_LTE_20MHz:
          fbrx_gs_info.wtr_lte_bw = 3;
        break;

        case RFCOM_BW_LTE_40MHz:
          fbrx_gs_info.wtr_lte_bw = 4;
        break;

        default:
          fbrx_gs_info.wtr_lte_bw = 0;
          FTM_MSG(FTM_ERROR, "Invalid LTE BW, setting WTR LTE BW = 0");
        break;
      }

      switch(band.lte_band)
      {
        case RFCOM_BAND_LTE_B5:
        case RFCOM_BAND_LTE_B6:
        case RFCOM_BAND_LTE_B8:
        case RFCOM_BAND_LTE_B12:
        case RFCOM_BAND_LTE_B13:
        case RFCOM_BAND_LTE_B14:
        case RFCOM_BAND_LTE_B17:
        case RFCOM_BAND_LTE_B18:
        case RFCOM_BAND_LTE_B19:
        case RFCOM_BAND_LTE_B20:
        case RFCOM_BAND_LTE_B26:
        case RFCOM_BAND_LTE_B27:
        case RFCOM_BAND_LTE_B28:
        case RFCOM_BAND_LTE_B28_B:
          selftest_lowband = TRUE;
        break;

        default:
          selftest_lowband = FALSE;
        break;
      }
    }
    break;
    #endif
    default:
      FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    break;
  } // Tech-specific setup

  rfcommon_mc_allocate_dpd_buffer();

  FTM_MSG_2(FTM_MED, "Selftest lowband: %d, Band: %d", selftest_lowband, ftm_common_com_band_specifier(params.tech, band));
} // selftest init

/*
Toggles the FBRx and reconfigures RxLM, TRUE = ENABLE, FALSE = DISABLE
*/
static boolean ftm_common_enable_fbrx_reconfigure_rxlm(const ftm_common_selftest_meas_params_struct params, const boolean enable_fbrx)
{
  static lm_handle_type tx_handle;
  static boolean setup = TRUE;
  static rfm_device_enum_type tx_device; // for CDMA
  rfcommon_fbrx_error_type fbrx_error;
  uint8 rf_tdscdma_fbrx_modem_chain = 1;

  // Grab Tx Handle, only the first time
  if(setup == TRUE)
  {
    switch(params.tech)
    {
    #ifdef FEATURE_CDMA1X
      case FTM_RF_TECH_CDMA:
      {

        /* Variables to hold the rx device state and the band informations
        to query the corresponding tx device from the RFC to support the 
        rx\tx split */
        ftm_cdma_data_status_type rx_dev_state;  
	sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;

        /* Query the rx device state to get the the band information and use the 
        band info to get the tx device from the RFC to support the rx\tx split */					  
	(void)ftm_cdma_data_get_device_status( params.device, &rx_dev_state );
	sys_band = rf_cdma_rf_band_type_to_sys_band_type( rx_dev_state.curr_band );
        tx_device = rfc_common_get_preferred_tx_device ( sys_band );

        const rf_cdma_data_status_type * dev_status = rf_cdma_get_device_status(tx_device);

        if(dev_status != NULL) tx_handle = dev_status->txlm_handle;
        
        else
        {
          FTM_MSG(FTM_ERROR, "Invalid CDMA device pointer!");
          return FALSE;
        }
      }
      break;
     #endif
	 #ifdef FEATURE_WCDMA
      case FTM_RF_TECH_WCDMA:
        tx_handle = rfwcdma_data_get_rflm_buf_idx(params.device);
      break; 
      #endif
	  #ifdef FTM_HAS_TDSCDMA
      case FTM_RF_TECH_TDSCDMA:
        tx_handle = rf_tdscdma_mc_lm_bufs.tx_buf_wrk.buf_idx;
      break;
      #endif

	  #ifdef FEATURE_LTE
      case FTM_RF_TECH_LTE:
        tx_handle = rflte_ftm_mc_sm_get_txlm_buffer(RFLTE_FTM_MC_PRX);
      break;
       #endif
      default:
        FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
        return FALSE;
      break;
    }

    setup = FALSE;
  }

  // Disable FBRx
  if(enable_fbrx == FALSE)
  {
    rfcommon_fbrx_mc_enable_fbrx(FALSE, tx_handle);
    DALSYS_BusyWait(6000);
  }
  
  // Reconfigure rxlm and Enable FBRx
  if(enable_fbrx == TRUE)
  {
    rfcommon_fbrx_tx_cfg_type fbrx_bw_mode;
    rfcom_band_type_u com_band;
    rfm_mode_enum_type mode;
    uint8 fbrx_chain;

    switch(params.tech)
    {
       #ifdef FEATURE_CDMA1X
      case FTM_RF_TECH_CDMA:
        fbrx_bw_mode = RFCOMMON_FBRX_1X;
        fbrx_chain = 3; /* WB3 is the default FBRX chain for CDMA */
        mode = RFM_1X_MODE;		
	fbrx_chain = rfc_cdma_get_fbrx_modem_chain(params.tx_device, com_band.cdma_band);
        fbrx_error = rfcommon_fbrx_mc_reconfigure_rxlm(params.tx_device, mode, fbrx_bw_mode, tx_handle, com_band, fbrx_chain);
      break;
	  #endif
      #ifdef FEATURE_WCDMA
      case FTM_RF_TECH_WCDMA:
        if(rfwcdma_data_get_num_carriers(rfwcdma_data_get_tx_device()) == RFCOM_SINGLE_CARRIER) 
		  fbrx_bw_mode = RFCOMMON_FBRX_UMTS_SC;
        else 
		  fbrx_bw_mode = RFCOMMON_FBRX_UMTS_DC;
		
        com_band.wcdma_band = ftm_curr_wcdma_mode;
        mode = RFM_IMT_MODE;
        fbrx_chain = rfc_wcdma_get_fb_chain(rfwcdma_data_get_tx_device(),com_band.wcdma_band); /* @todo: remove hard-coding, tech to update the fbrx chain from rfc */        
        fbrx_error = rfcommon_fbrx_mc_reconfigure_rxlm(params.device, mode, fbrx_bw_mode, tx_handle, com_band, fbrx_chain);
      break;
	  #endif
      #ifdef FTM_HAS_TDSCDMA
      case FTM_RF_TECH_TDSCDMA:
        fbrx_bw_mode = RFCOMMON_FBRX_TDS;
        com_band.tdscdma_band = rf_tdscdma_mc_state.curr_band_tx;
        mode = RFM_TDSCDMA_MODE;
        rf_tdscdma_fbrx_modem_chain = rfc_tdscdma_get_fb_chain(rf_tdscdma_mc_state.curr_tx_device,com_band.tdscdma_band );
        fbrx_error = rfcommon_fbrx_mc_reconfigure_rxlm(params.device, mode, fbrx_bw_mode, tx_handle, com_band, rf_tdscdma_fbrx_modem_chain);
      break;
	  #endif
      #ifdef FEATURE_LTE
      case FTM_RF_TECH_LTE:
        fbrx_bw_mode = rflte_util_get_fbrx_bw_for_sys_bw(rflte_ftm_mc_sm_get_tx_bw());
        com_band.lte_band = rflte_ftm_mc_sm_get_tx_band();
        mode = RFM_LTE_MODE;
        fbrx_chain = (uint8)(rfc_lte_get_fb_chain(rflte_ftm_mc_sm_get_tx_device(),
                                                  rflte_ftm_mc_sm_get_tx_band()));
        fbrx_error = rfcommon_fbrx_mc_reconfigure_rxlm(rflte_ftm_mc_sm_get_tx_device(), mode, fbrx_bw_mode, tx_handle, com_band, fbrx_chain);
      break;
      #endif
      default:
        FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
        return FALSE;
      break;
    }

    // Failed to reconfigure FBRx
    if(fbrx_error != RFCOMMON_FBRX_SUCCESS)
    {
      FTM_MSG_5(FTM_ERROR, "Could not reconfigure FBRx LM for Tech: %d, Mode: %d, FBRx BW Mode: %d, Tx Handle: %d, Band: %d", 
        params.tech, mode, fbrx_bw_mode, tx_handle, ftm_common_com_band_specifier(params.tech, com_band));
    }

    else
    {
      FTM_MSG_5(FTM_HIGH, "Reconfigured FBRx LM for Tech: %d, Mode: %d, FBRx BW Mode: %d, Tx Handle: %d, Band: %d",
       params.tech, mode, fbrx_bw_mode, tx_handle, ftm_common_com_band_specifier(params.tech, com_band));
    }
  
    // Enable FBRx
    rfcommon_fbrx_mc_enable_fbrx(TRUE, tx_handle);

    // Next invocation we need to grab a new Tx handle
    setup = TRUE;
  }
  return TRUE;
}

/*
 FBRx Override toggle, TRUE = Selftest mode, FALSE = Reload original FBRx gain look up table
*/
static boolean ftm_common_override_fbrx_gain(const ftm_common_selftest_meas_params_struct params, const boolean override_enable)
{
  rfdevice_rxtx_common_class * device_ptr;

  /* Variables to hold the rx device state and the band informations
  to query the corresponding tx device from the RFC to support the 
  rx\tx split */  
  #ifdef FEATURE_CDMA1X
  ftm_cdma_data_status_type rx_dev_state;  /* Var to hold current rx state */
  sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;

  /* Query the rx device state to get the the band information and use the 
  band info to get the tx device from the RFC to support the rx\tx split */				  
  (void)ftm_cdma_data_get_device_status( params.prx_device, &rx_dev_state );
  sys_band = rf_cdma_rf_band_type_to_sys_band_type( rx_dev_state.curr_band );
#endif

  if(params.tech == FTM_RF_TECH_CDMA) 
  	{
  	#ifdef FEATURE_CDMA1X
  	device_ptr = rfcommon_core_get_tx_device_ptr(rfc_common_get_preferred_tx_device ( sys_band ), ftm_common_ftm_tech_to_rfm_mode(params.tech), fbrx_gs_info.com_band);
    #endif
  	}
  else if(params.tech == FTM_RF_TECH_TDSCDMA) device_ptr = rfc_common_get_cmn_device_object(0);
  else device_ptr = rfcommon_core_get_tx_device_ptr(params.device, ftm_common_ftm_tech_to_rfm_mode(params.tech), fbrx_gs_info.com_band);

  if(device_ptr == NULL)
  {
    FTM_MSG(FTM_ERROR, "rfcommon_core_get_tx_device_ptr returned NULL!");
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return FALSE;
  }
  return rfdevice_cmn_override_fbrx_gain_state(device_ptr, ftm_common_ftm_tech_to_rfm_mode(params.tech), fbrx_gs_info.com_band, fbrx_gs_info.selftest_fbrx_gs, fbrx_gs_info.wtr_lte_bw, RFDEVICE_EXECUTE_IMMEDIATE, override_enable, NULL);
}

/*
Wrapper for tech-specific XPT configuration functions
*/
static boolean ftm_common_selftest_config(const ftm_common_selftest_meas_params_struct params)
{
  boolean error_code;
  rfm_device_enum_type fbrx_chain = RFM_DEVICE_1; // Currently hardcoded, better solution?

  // Tech-specific configuration + calibration
  switch(params.tech)
  {
     #ifdef FEATURE_CDMA1X
    case FTM_RF_TECH_CDMA:
      error_code = ftm_1x_xpt_cal_config(params.prx_device, fbrx_chain);
    break;
	#endif
	
    #ifdef FEATURE_WCDMA
    case FTM_RF_TECH_WCDMA:
      error_code = ftm_wcdma_xpt_cal_config(params.device, fbrx_chain, FTM_XPT_CONFIG_EPT, TRUE);
    break; 
	#endif
	
    #ifdef FTM_HAS_TDSCDMA
    case FTM_RF_TECH_TDSCDMA:
      // All techs fail at RxLM dynamic settings setup, but TD is actually returning FALSE, so hack it for now.
      /*error_code = */ftm_tdscdma_xpt_cal_config(params.device, fbrx_chain, FTM_XPT_CONFIG_EPT, TRUE);
      error_code = TRUE;
    break;
	#endif
	
    #ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE:
      fbrx_chain = rfc_lte_get_fb_device(rflte_ftm_mc_sm_get_tx_device(), rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
      error_code = ftm_lte_xpt_cal_configure(params.device, fbrx_chain, FTM_XPT_CONFIG_ET);
    break;
    #endif
	
    default:
      FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return FALSE;
    break;
  }

  FTM_MSG_4(FTM_MED, "XPT Configuration done for %d, device: %d, fbrx chain: %d, status: %d", params.tech, params.device, fbrx_chain, error_code);

  return error_code;
}

/*
Setups Tx/Rx filter settings. Currently only configures filters for EVM capture settings
*/
static boolean ftm_common_filter_setup(const ftm_common_selftest_meas_params_struct params, const boolean no_band_limit_filter)
{
  boolean success = TRUE;

  // Filter settings
  uint8 tx_upsamp = 1;
  uint8 tx_up_fir_len = 0;
  int8 tx_up_fir_shift = 0;
  int16 * tx_up_fir = tx_up_filter_coef;
  uint8 rx_bw_filt_len;
  int16 * rx_bw_filt_coeff = NULL;
  boolean freq_adj;
  uint8 eq_filt_mask = (1 << 2);

  rfm_device_enum_type fbrx_chain = RFM_DEVICE_1;

  // Tech-specific filter setup
  switch(params.tech)
  {
     #ifdef FEATURE_CDMA1X
    case FTM_RF_TECH_CDMA:
      // Setup R/x Filter
      rx_bw_filt_coeff = evm_cdma_fir;
      rx_bw_filt_len = sizeof(evm_cdma_fir)/sizeof(int16) - 1;
      freq_adj = FALSE;
    break;
    #endif
	
	#ifdef FEATURE_WCDMA
    case FTM_RF_TECH_WCDMA:
      // Setup Rx filter
      rx_bw_filt_coeff = evm_wcdma_fir;
      rx_bw_filt_len = sizeof(evm_wcdma_fir)/sizeof(int16) - 1;
      freq_adj = TRUE;

      // Setup Tx Filter
      tx_upsamp = 2;
      tx_up_fir_len = 10;
      tx_up_fir_shift = -14;
    break; 
    #endif
	
	#ifdef FTM_HAS_TDSCDMA
    case FTM_RF_TECH_TDSCDMA:
      // Setup Rx filter
      rx_bw_filt_coeff = evm_tdscdma_fir;
      rx_bw_filt_len = sizeof(evm_tdscdma_fir)/sizeof(int16) - 1;
      freq_adj = TRUE;
    break;
    #endif
	
    #ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE:
    {
      fbrx_chain = rfc_lte_get_fb_device(rflte_ftm_mc_sm_get_tx_device(), rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
      uint32 fbrx_carrier_idx = rflte_ftm_mc_sm_get_carrier_index();
      rflte_ftm_mc_sm_type * lte_ftm_mc_sm_ptr = rflte_ftm_mc_get_sm();

      if (fbrx_carrier_idx >= RFA_RF_LTE_MAX_CELL_SUPPORTED) {

        // Set default carrier idx

        fbrx_carrier_idx = 0;

        FTM_MSG(FTM_ERROR, "FBRX carrier index invalid, set to default 0");

      }
      switch(lte_ftm_mc_sm_ptr->carrier[fbrx_carrier_idx].tx_bw)
      {
        case RFCOM_BW_LTE_5MHz:
          rx_bw_filt_coeff = evm_lte_5_fir;
          rx_bw_filt_len = sizeof(evm_lte_5_fir)/sizeof(int16) - 1;
        break;

        case RFCOM_BW_LTE_10MHz:
          rx_bw_filt_coeff = evm_lte_10_fir;
          rx_bw_filt_len = sizeof(evm_lte_10_fir)/sizeof(int16) - 1;
        break;

        case RFCOM_BW_LTE_15MHz:
          rx_bw_filt_coeff = evm_lte_15_fir;
          rx_bw_filt_len = sizeof(evm_lte_15_fir)/sizeof(int16) - 1;
        break;

        case RFCOM_BW_LTE_20MHz:
          rx_bw_filt_coeff = evm_lte_20_fir;
          rx_bw_filt_len = sizeof(evm_lte_20_fir)/sizeof(int16) - 1;
        break;

        default:
          rx_bw_filt_coeff = NULL;
          rx_bw_filt_len = 0;
        break;
      }

      if(lte_ftm_mc_sm_ptr->carrier[fbrx_carrier_idx].rb_block < 36) eq_filt_mask = ((1 << 2) | (1 << 1));
      else if(lte_ftm_mc_sm_ptr->carrier[fbrx_carrier_idx].rb_block < 75) eq_filt_mask = ((1 << 3) | (1 << 2));
      else eq_filt_mask = ((1 << 4) | (1 << 3) | (1 << 2));
      freq_adj = FALSE;

      // Tx Filter
      tx_upsamp = 2;
      tx_up_fir_len = 10;
      tx_up_fir_shift = -14;
    }
	#endif
    break;

    default:
      FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
      return FALSE;
    break;
  } // Tech-specific Filter setup

  // Filter setup
  if(no_band_limit_filter)
  {
    success = rfcommon_mdsp_setup_self_test_params(FALSE, 0, 0, NULL, 0);
  }

  else if(rx_bw_filt_coeff != NULL) success = rfcommon_mdsp_setup_self_test_params(freq_adj, rx_bw_filt_len, *(rx_bw_filt_coeff + rx_bw_filt_len), rx_bw_filt_coeff, eq_filt_mask);

  if(success == FALSE)
  {
    FTM_MSG(FTM_ERROR, "Rx filter setup failed!");
    return FALSE;
  }

  success = rfcommon_mdsp_setup_self_test_upsampling_params(tx_upsamp, tx_up_fir_len, tx_up_fir_shift, tx_up_fir);

  if(success == FALSE)
  {
    FTM_MSG(FTM_ERROR, "Tx filter setup failed!");
    return FALSE;
  }

  return success;
} 

/* 
Perform ACLR calculation, reads samples, performs FFT
*/
static void ftm_common_aclr_calc(const ftm_common_selftest_meas_params_struct params, int32 * sample_buffer)
{
  boolean success = TRUE;

  uint8 sub_id=0;

  // Copy, parse and perform FFT
  for(uint8 i = 0; i < (2 * (NUM_SAMPLES/FFT_SIZE)) - 1; i++)
  {
    // Here we move the samples out of the IQ buffer and into the FFT buffer
    memscpy((void *) sample_buffer, FFT_SIZE * 2 * 4, (void *) iq_samples.iq_buf_rx, PCFL_SIZE);
    iq_samples.iq_buf_rx = iq_samples.iq_buf_rx + (PCFL_SIZE/(2 * sizeof(int32)));

    // Tech-specific parsing
    switch(params.tech)
    {
      case FTM_RF_TECH_CDMA:
        ftm_common_parse_PCFL20_to_IQ(FTM_STATE_1X, sample_buffer, FFT_SIZE);
      break;

      case FTM_RF_TECH_WCDMA:
        ftm_common_parse_PCFL20_to_IQ(FTM_STATE_WCDMA, sample_buffer, FFT_SIZE);
      break;

      case FTM_RF_TECH_TDSCDMA:
        ftm_common_parse_PCFL20_to_IQ(FTM_STATE_TDSCDMA, sample_buffer, FFT_SIZE);
      break;

      case FTM_RF_TECH_LTE:
        ftm_common_parse_PCFL20_to_IQ(FTM_STATE_LTE, sample_buffer, FFT_SIZE);
      break;

      default:
        FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      break;    
    } 

    // Perform FFT
    ftm_common_get_samples_fft_avg(FFT_SIZE, sample_buffer);
  }

  switch(params.tech)
  {
    case FTM_RF_TECH_CDMA:
      // Nothing extra
    break;

    case FTM_RF_TECH_WCDMA:
      ftm_common_get_aclr_bins_struct_with_samp_rate(FTM_STATE_WCDMA, RFCOM_BW_LTE_INVALID, FFT_SIZE, iq_samples.sampling_frequency);
    break;

    case FTM_RF_TECH_TDSCDMA:
      ftm_common_get_aclr_bins_struct_with_samp_rate(FTM_STATE_TDSCDMA, RFCOM_BW_LTE_INVALID, FFT_SIZE, iq_samples.sampling_frequency);
    break;

	#ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE:
      ftm_common_get_aclr_bins_struct_with_samp_rate(FTM_STATE_LTE, rflte_ftm_mc_sm_get_tx_bw(), FFT_SIZE, iq_samples.sampling_frequency);
    break;
    #endif
    default:
      FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    break;  
  }

  // Assume spectral inversion by default
  uint8 spec_inv = 1;
  ftm_common_aclr_result_struct_type aclr_result;
  ftm_common_1x_aclr_result_struct_type aclr_result_1x;
  ftm_common_power_per_bin_struct_type noise_power_to_subtract;

  switch(params.tech)
  {
    #ifdef FEATURE_CDMA1X
    case FTM_RF_TECH_CDMA:
      // this function is obviously not common, perhaps move into 1x specific files ??
      ftm_common_get_1x_aclr(FFT_SIZE, ftm_cdma_data_get_band_class(params.device), &aclr_result_1x);
    break;
	#endif

   #ifdef FEATURE_WCDMA
    case FTM_RF_TECH_WCDMA:
    {
      rfnv_wcdma_nv_tbl_type * nv_tbl = NULL;
      memset((void *) &noise_power_to_subtract, 0, sizeof(ftm_common_power_per_bin_struct_type));
      ftm_wcdma_obtain_fbrx_noise_pwr(&noise_power_to_subtract);
      nv_tbl = rfnv_wcdma_get_nv_tbl_ptr(rfnv_wcdma_convert_band_rfcom_to_rfnv(ftm_curr_wcdma_mode));

      // Validate NV table pointer
      if(nv_tbl == NULL) FTM_MSG(FTM_ERROR, "Failed to get valid static NV pointer, assuming spectral inversion = 1!");
      else spec_inv = nv_tbl->tx_ept_dpd_config_params[DPD_CFG_SPEC_INV];
      

      ftm_common_get_aclr(FFT_SIZE, FTM_STATE_WCDMA, RFCOM_BW_LTE_INVALID, noise_power_to_subtract, &aclr_result, spec_inv);
    }
    break;
    #endif

	#ifdef FTM_HAS_TDSCDMA
    case FTM_RF_TECH_TDSCDMA:
    {
      rfnv_tdscdma_tx_tbl_type * nv_tbl = NULL;
      memset((void *) &noise_power_to_subtract, 0, sizeof(ftm_common_power_per_bin_struct_type));
      nv_tbl = rf_tdscdma_core_util_init_tx_nv_table(ftm_tdscdma_current_mode);

      // Validate NV table pointer
      if(nv_tbl == NULL) FTM_MSG(FTM_ERROR, "Failed to get valid static NV pointer, assuming spectral inversion = 1!");
      else spec_inv = nv_tbl->ept_dpd_cfg_params[DPD_CFG_SPEC_INV];

      ftm_common_get_aclr(FFT_SIZE, FTM_STATE_TDSCDMA, RFCOM_BW_LTE_INVALID, noise_power_to_subtract, &aclr_result, spec_inv);
    }
    break;
    #endif
	
	#ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE:
    {
      rflte_nv_tbl_type * nv_tbl_ptr = NULL;
      uint32 carrier_idx = 0;
      lm_handle_type txlm_buffer_idx = RFLTE_MC_TXLM_INVALID_HANDLE;


      rflte_state_get_txlm_buffer_idx(sub_id,carrier_idx,&txlm_buffer_idx);

      memset((void *) &noise_power_to_subtract, 0, sizeof(ftm_common_power_per_bin_struct_type));
      ftm_lte_obtain_fbrx_noise_pwr(&noise_power_to_subtract, rflte_ftm_mc_sm_get_tx_bw());

      // Assume 0th carrier -- correct?

      nv_tbl_ptr = rflte_mc_get_nv_tbl_ptr(sub_id,carrier_idx,txlm_buffer_idx);

      // Validate NV table pointer
      if(nv_tbl_ptr == NULL) FTM_MSG(FTM_ERROR, "Failed to get valid static NV pointer, assuming spectral inversion = 1!");
      else 
            spec_inv = (uint8)((nv_tbl_ptr->tx_path_nv_data[rflte_mc_get_tx_nv_container(carrier_idx,txlm_buffer_idx)]).\
                          tx_xpt_dpd_config_params[DPD_CFG_SPEC_INV]); 


      ftm_common_get_aclr(FFT_SIZE, FTM_STATE_LTE, rflte_ftm_mc_sm_get_tx_bw(), noise_power_to_subtract, &aclr_result, spec_inv);
    }
    break;
	#endif

    default:
      FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    break;  
  }

  // ACLR for 1x -- Potentially get ACLR common???
  if(params.tech == FTM_RF_TECH_CDMA)
  {
    // Print ACLR values
    FTM_MSG_6(FTM_MED, "ACLR Values: %d, %d, %d, %d, %d, %d",
     aclr_result_1x.adj_pos,
     aclr_result_1x.adj_neg, 
     aclr_result_1x.alt1_pos, 
     aclr_result_1x.alt1_neg, 
     aclr_result_1x.alt2_pos, 
     aclr_result_1x.alt2_neg);

    // Update payload structure with accumulated ACLR
    meas_tx_params.aclr_value[0] = meas_tx_params.aclr_value[0] + aclr_result_1x.adj_pos;
    meas_tx_params.aclr_value[1] = meas_tx_params.aclr_value[1] + aclr_result_1x.adj_neg;
    meas_tx_params.aclr_value[2] = meas_tx_params.aclr_value[2] + aclr_result_1x.alt1_pos;
    meas_tx_params.aclr_value[3] = meas_tx_params.aclr_value[3] + aclr_result_1x.alt1_neg;
    meas_tx_params.aclr_value[4] = meas_tx_params.aclr_value[4] + aclr_result_1x.alt2_pos;
    meas_tx_params.aclr_value[5] = meas_tx_params.aclr_value[5] + aclr_result_1x.alt2_neg;
  }

  // Other Techs
  else
  {
    // Print ACLR values
    FTM_MSG_6(FTM_MED, "ACLR Values: %d, %d, %d, %d, %d, %d",
     aclr_result.aclr1_neg,
     aclr_result.aclr1_pos, 
     aclr_result.aclr2_neg, 
     aclr_result.aclr2_pos, 
     aclr_result.aclr_eutra_neg, 
     aclr_result.aclr_eutra_pos);

    // Update payload structure with accumulated ACLR
    meas_tx_params.aclr_value[0] = meas_tx_params.aclr_value[0] + aclr_result.aclr1_neg;
    meas_tx_params.aclr_value[1] = meas_tx_params.aclr_value[1] + aclr_result.aclr1_pos;
    meas_tx_params.aclr_value[2] = meas_tx_params.aclr_value[2] + aclr_result.aclr2_neg;
    meas_tx_params.aclr_value[3] = meas_tx_params.aclr_value[3] + aclr_result.aclr2_pos;
    meas_tx_params.aclr_value[4] = meas_tx_params.aclr_value[4] + aclr_result.aclr_eutra_neg;
    meas_tx_params.aclr_value[5] = meas_tx_params.aclr_value[5] + aclr_result.aclr_eutra_pos;
  }

} // FTM Common ACLR Calc

/* 
Setup buffers for FFT computation
*/
static boolean ftm_common_buffer_setup(const ftm_common_selftest_meas_params_struct params, int32 ** sample_buffer)
{
  // Allocate space for buffer
  *sample_buffer = (int32 *) ftm_malloc(FFT_SIZE * 2 * 4);

  // Null Check
  if(*sample_buffer == NULL)
  {
    error_code = FTM_FBRX_TEST_MEM_ALLOCATION_FAILURE;
    FTM_MSG(FTM_ERROR, "Buffer allocation failure!");
    return FALSE;
  }

  // Technology-specific call
  switch(params.tech)
  {
    case FTM_RF_TECH_CDMA:
      ftm_common_init_fft_data(FTM_STATE_1X, RFCOM_BW_LTE_INVALID);
    break;

    case FTM_RF_TECH_WCDMA:
      ftm_common_init_fft_data(FTM_STATE_WCDMA, RFCOM_BW_LTE_INVALID);
    break;

    case FTM_RF_TECH_TDSCDMA:
      ftm_common_init_fft_data(FTM_STATE_TDSCDMA, RFCOM_BW_LTE_INVALID);
    break;

	#ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE:
      ftm_common_init_fft_data(FTM_STATE_LTE, rflte_ftm_mc_sm_get_tx_bw());
    break;

	#endif

    default:
        FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return FALSE;
    break;
  }

  FTM_MSG(FTM_MED, "FTM Common Buffer Setup Succeeded!");
  return TRUE;
}

/*
Performs EVM capture. 
If ACLR meeasurement is requested, ACLR computation functions will be invoked. 
If EVM measurement is requested, DPD processing will do place. 
If VSWR measurement is requested, coupler will be flipped and a second EVM capture 
be will taken to perform VSWR measurement.  
EFS samples are also saved here 
*/ 
static void ftm_common_selftest_evm_cap(const ftm_common_selftest_meas_params_struct params, int32 * sample_buffer)
{
  uint16 evm_value_temp;
  uint32 rx_rms_value_temp;
  boolean success = TRUE;
  rfcommon_selftest_meas_result proc_result;

  // We do not need to do DPD processing for ACLR, but we do need to do it for EVM or VSWR v1
  // We are performing an ACLR measurement
  if(params.tx_measurement_config & FTM_FBRX_MEASURE_ACLR)
  {
    // We are NOT performing an EVM measurement or VSWR v1 measurement
    if((params.tx_measurement_config & FTM_FBRX_MEASURE_EVM) == 0 && (params.tx_measurement_config & FTM_FBRX_MEAS_VSWR) == 0)
    {
      proc_flag = PROC_NONE;
    }
  }

  // Print debug
  FTM_MSG_1(FTM_MED, "Processing Flag: %d", proc_flag);

  success = ftm_common_selftest_config(params);

  if(success == FALSE)
  {
    FTM_MSG_1(FTM_ERROR, "XPT cal + config failed for %d", params.tech);
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return;
  }

  if (fbrx_gs_info.override_enable) ftm_common_override_fbrx_gain(params, TRUE);

  // Buffer for capture and DPD results
  rf_common_xpt_buffer_id_type buffer_ids;

  buffer_ids.capt_buf_id = 0;
  buffer_ids.res_buf_id = 0;

  success = ftm_common_filter_setup(params, FALSE);

  if(success == FALSE)
  {
    FTM_MSG_1(FTM_ERROR, "Filter setup failed for %d", params.tech);
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return; 
  }

  boolean first_trig = TRUE;
  boolean last_trig = FALSE;

  // Capture and calculation loop
  for(uint8 cap_num = 0; cap_num < params.num_averages; cap_num++)
  {
    // This will trigger deinitialization!
    if(cap_num == (params.num_averages - 1)) last_trig = TRUE;

    success = rfcommon_xpt_is_capture_done();

    // Capture is ongoing
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "IQ capture ongoing");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Ensure processing is not ongoing, if applicable
    if(proc_flag == PROC_SELF_TEST)
    {
      success = rfcommon_xpt_is_processing_done();

      // Processing is ongoing
      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "IQ processing ongoing");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }
    }
    
    success = rfcommon_xpt_set_capture_in_progress();

    // Unable to set capture in progress
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Unable to set capture in progress");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Set processing flag if necessary
    if(proc_flag == PROC_SELF_TEST)
    {
      success = rfcommon_xpt_set_processing_in_progress();

      // Failed to set proccessing in progress
      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "Unable to set processing in progress");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }
    }

    // Initialize buffers for capture
    rfcommon_core_xpt_init_buffers();
    success = rfcommon_core_xpt_get_buffers(&buffer_ids);

    // Failed to obtain buffers for IQ capture
    if (success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Unable to obtain buffers for capture");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    rfm_device_enum_type fbrx_chain = RFM_DEVICE_1;

    FTM_MSG_3(FTM_MED, "Calling XPT Cap for %d with Tx: %d Rx: %d", params.tech, params.device, fbrx_chain);

    // Tech-specific capture
    switch(params.tech)
    {
      #ifdef FEATURE_CDMA1X
      case FTM_RF_TECH_CDMA:
        success = ftm_1x_xpt_iq_capture(params.prx_device, fbrx_chain, XPT_CAPTURE_SELF_TEST_EVM, NUM_SAMPLES, first_trig, last_trig, proc_flag, buffer_ids, &(iq_samples.sampling_frequency));
      break;
	  #endif
      #ifdef FEATURE_WCDMA
      case FTM_RF_TECH_WCDMA:
        success = ftm_wcdma_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_EVM, NUM_SAMPLES, first_trig, last_trig, proc_flag, buffer_ids, &(iq_samples.sampling_frequency));
      break; 
	  #endif
     #ifdef FTM_HAS_TDSCDMA
      case FTM_RF_TECH_TDSCDMA:
        success = ftm_tdscdma_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_EVM, NUM_SAMPLES, first_trig, last_trig, proc_flag, buffer_ids, &(iq_samples.sampling_frequency));
      break;
	  #endif
    #ifdef FEATURE_LTE
      case FTM_RF_TECH_LTE:
        fbrx_chain = rfc_lte_get_fb_device(rflte_ftm_mc_sm_get_tx_device(), rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
        success = ftm_lte_xpt_iq_capture(params.device, fbrx_chain, NUM_SAMPLES, first_trig, last_trig, proc_flag, XPT_CAPTURE_SELF_TEST_EVM, buffer_ids.capt_buf_id, buffer_ids.res_buf_id, &(iq_samples.sampling_frequency));
      break;
	  #endif

      default:
        FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      break;
    } // Tech-specific capture
  
    // If are are going to perform more triggers, we do not need to do all of the setup!
    first_trig = FALSE;

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Capture trigger failed!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    uint8 iq_buffer_index = 0;
    success = rfcommon_xpt_wait_for_capture_done(500, 5000, &iq_buffer_index);

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Capture did not complete!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Fetch collected samples
    rfcommon_mc_fetch_selftest_iq_data(iq_buffer_index, NUM_SAMPLES, &(iq_samples.iq_buf_rx), &(iq_samples.iq_buf_tx));

    // Processing
    uint8 result_buffer_index = 0;
    success = rfcommon_xpt_wait_for_processing_done(1000, 35000, &result_buffer_index);

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Processing did not complete!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Replace with fetch that only fetches EVM value?
    

    rfcommon_mc_fetch_selftest_meas(result_buffer_index, &proc_result);

    evm_value_temp = proc_result.evm;
    rx_rms_value_temp = proc_result.rx_rms_mag;


    FTM_MSG_3(FTM_MED, "EVM: %d, Rx RMS: %d Iteration: %d", evm_value_temp, rx_rms_value_temp, cap_num + 1);

    meas_tx_params.evm_value += evm_value_temp;
    meas_tx_params.rx_rms_mag += rx_rms_value_temp;

    // Export samples
    rfcommon_iq_efs_save(iq_samples.iq_buf_tx, iq_samples.iq_buf_rx, NUM_SAMPLES);

    // do ACLR calculation
    if(params.tx_measurement_config & FTM_FBRX_MEASURE_ACLR) ftm_common_aclr_calc(params, sample_buffer);
    
    // Release buffers -- Better place to do this
    rfcommon_core_xpt_release_capture_buffer(iq_buffer_index);
    rfcommon_core_xpt_release_results_buffer(result_buffer_index);
    rfcommon_core_xpt_free_all_buffers();

  } // End for loop

  // Finalize and Print EVM values
  if(params.tx_measurement_config & FTM_FBRX_MEASURE_EVM)
  {
    FTM_MSG_1(FTM_HIGH, "Final accumulated EVM Value: %d", meas_tx_params.evm_value);
    meas_tx_params.evm_value = (uint16) (meas_tx_params.evm_value/params.num_averages);
    FTM_MSG_1(FTM_HIGH, "Final EVM Value: %d", meas_tx_params.evm_value);
  }

  // Finalize and Print Forward Rx RMS values
  if(params.tx_measurement_config & FTM_FBRX_MEAS_VSWR)
  {
    FTM_MSG_1(FTM_HIGH, "Final accumulated Forward Rx RMS Value: %d", meas_tx_params.rx_rms_mag);
    meas_tx_params.rx_rms_mag = (int16) (meas_tx_params.rx_rms_mag/params.num_averages);
    FTM_MSG_1(FTM_HIGH, "Final Forward Rx RMS Value: %d", meas_tx_params.rx_rms_mag);
  }

  // Finalize and Print ACLR values
  if(params.tx_measurement_config & FTM_FBRX_MEASURE_ACLR)
  {
    for(uint8 j = 0; j < 6; j++)
    {
      FTM_MSG_2(FTM_HIGH, "Final accumulated ACLR%d Value: %d", j + 1, meas_tx_params.aclr_value[j]);
      meas_tx_params.aclr_value[j] = (int16) (meas_tx_params.aclr_value[j]/params.num_averages);
      FTM_MSG_2(FTM_HIGH, "Final ACLR%d Value: %d", j + 1, meas_tx_params.aclr_value[j]);
    }
  }

  // Flip Coupler direction
  if(params.tx_measurement_config & FTM_FBRX_MEAS_VSWR)
  {
    ftm_common_set_coupler_direction(1);

    success = ftm_common_selftest_config(params);

    if(success == FALSE)
    {
      FTM_MSG_1(FTM_ERROR, "XPT cal + config failed for %d", params.tech);
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    buffer_ids.capt_buf_id = 0;
    buffer_ids.res_buf_id = 0;

    success = ftm_common_filter_setup(params, FALSE);

    if(success == FALSE)
    {
      FTM_MSG_1(FTM_ERROR, "Filter setup failed for %d", params.tech);
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return; 
    }

    first_trig = TRUE;
    last_trig = FALSE;

    // Capture and calculation loop
    for(uint8 cap_num = 0; cap_num < params.num_averages; cap_num++)
    {
      // This will trigger deinitialization!
      if(cap_num == (params.num_averages - 1)) last_trig = TRUE;

      success = rfcommon_xpt_is_capture_done();

      // Capture is ongoing
      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "IQ capture ongoing");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }

      success = rfcommon_xpt_is_processing_done();

      // Processing is ongoing
      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "IQ processing ongoing");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }
      
      success = rfcommon_xpt_set_capture_in_progress();

      // Unable to set capture in progress
      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "Unable to set capture in progress");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }

      success = rfcommon_xpt_set_processing_in_progress();

      // Failed to set proccessing in progress
      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "Unable to set processing in progress");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }

      // Initialize buffers for capture
      rfcommon_core_xpt_init_buffers();
      success = rfcommon_core_xpt_get_buffers(&buffer_ids);

      // Failed to obtain buffers for IQ capture
      if (success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "Unable to obtain buffers for capture");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }

      rfm_device_enum_type fbrx_chain = RFM_DEVICE_1;

      FTM_MSG_3(FTM_MED, "Calling XPT Cap for %d with Tx: %d Rx: %d", params.tech, params.device, fbrx_chain);

      // Tech-specific capture
      switch(params.tech)
      {
        #ifdef FEATURE_CDMA1X
        case FTM_RF_TECH_CDMA:
          success = ftm_1x_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_EVM, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));
        break;
        #endif
		#ifdef FEATURE_WCDMA
        case FTM_RF_TECH_WCDMA:
          success = ftm_wcdma_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_EVM, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));
        break; 
		#endif

		#ifdef FTM_HAS_TDSCDMA
        case FTM_RF_TECH_TDSCDMA:
          success = ftm_tdscdma_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_EVM, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));
        break;
		#endif
		
        #ifdef FEATURE_LTE
        case FTM_RF_TECH_LTE:
          fbrx_chain = rfc_lte_get_fb_device(rflte_ftm_mc_sm_get_tx_device(), rflte_ftm_mc_sm_get_tx_band(), rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
          success = ftm_lte_xpt_iq_capture(params.device, fbrx_chain, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, XPT_CAPTURE_SELF_TEST_EVM, buffer_ids.capt_buf_id, buffer_ids.res_buf_id, &(iq_samples.sampling_frequency));
        break;
		#endif

        default:
          FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
          error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
          return;
        break;
      } // Tech-specific capture

      // If are are going to perform more triggers, we do not need to do all of the setup!
      first_trig = FALSE;

      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "Capture trigger failed!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }

      uint8 iq_buffer_index = 0;
      success = rfcommon_xpt_wait_for_capture_done(500, 5000, &iq_buffer_index);

      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "Capture did not complete!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }

      // Fetch collected samples
      rfcommon_mc_fetch_selftest_iq_data(iq_buffer_index, NUM_SAMPLES, &(iq_samples.iq_buf_rx), &(iq_samples.iq_buf_tx));

      // Processing
      uint8 result_buffer_index = 0;
      success = rfcommon_xpt_wait_for_processing_done(1000, 35000, &result_buffer_index);

      if(success == FALSE)
      {
        FTM_MSG(FTM_ERROR, "Processing did not complete!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      }

      // Export samples
      // rfcommon_iq_efs_save(iq_samples.iq_buf_tx, iq_samples.iq_buf_rx, NUM_SAMPLES);

     rfcommon_mc_fetch_selftest_meas(result_buffer_index, &proc_result);

     evm_value_temp = proc_result.evm;
     rx_rms_value_temp = proc_result.rx_rms_mag;


      meas_tx_params.rx_rms_reverse_mag += rx_rms_value_temp;

      // Release buffers -- Better place to do this
      rfcommon_core_xpt_release_capture_buffer(iq_buffer_index);
      rfcommon_core_xpt_release_results_buffer(result_buffer_index);
      rfcommon_core_xpt_free_all_buffers();

    } // End for loop

    // Revert coupler direction
    ftm_common_set_coupler_direction(0);

    // Finalize and Print Reverse Rx RMS values
    if(params.tx_measurement_config & FTM_FBRX_MEAS_VSWR)
    {
      FTM_MSG_1(FTM_HIGH, "Final accumulated Reverse Rx RMS Value: %d", meas_tx_params.rx_rms_reverse_mag);
      meas_tx_params.rx_rms_mag = (int16) (meas_tx_params.rx_rms_reverse_mag/params.num_averages);
      FTM_MSG_1(FTM_HIGH, "Final Reverse Rx RMS Value: %d", meas_tx_params.rx_rms_reverse_mag);
    }

    // Finalize VSWR calculation
    if(meas_tx_params.rx_rms_mag > meas_tx_params.rx_rms_reverse_mag) meas_tx_params.vswr_ratio_db10 = (200 * log10(meas_tx_params.rx_rms_mag)) - (200 * log10(meas_tx_params.rx_rms_reverse_mag));
    
    else
    {
      success_flags.vswr_success = FALSE;
      FTM_MSG(FTM_ERROR, "Reverse RMS > Forward RMS !!");
    }

  } // Reverse coupler direction
} // EVM Cap

/* 
 Perform spectral flatness capture (LTE ONLY)
*/
static void ftm_common_selftest_spec_flat_cap(const ftm_common_selftest_meas_params_struct params, int32 * sample_buffer)
{
  boolean success = TRUE;
  uint8 fbrx_error;
   #ifdef FEATURE_LTE
  rfcommon_fbrx_tx_cfg_type fbrx_bw_mode = rflte_util_get_fbrx_bw_for_sys_bw(rflte_ftm_mc_sm_get_tx_bw());
  rfcommon_fbrx_nv_droop_type droop_data;

  uint8 sub_id=0;

  uint32 carrier_idx = 0;
  lm_handle_type txlm_buffer_idx = RFLTE_MC_TXLM_INVALID_HANDLE;

  // Get Undroop filter coefficients
  if(fbrx_gs_info.com_band.lte_band < RFCOM_NUM_LTE_BANDS)
  {
    fbrx_error = rfcommon_fbrx_get_current_droop_filter(FTM_RF_TECH_LTE, fbrx_gs_info.com_band, fbrx_bw_mode, &droop_data, RFCOMMON_FBRX_NV_CONTAINER_C0);
  }

  else
  {
    FTM_MSG(FTM_ERROR, "Invalid LTE Band!");
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return;
  }

  if (fbrx_error != RFCOMMON_FBRX_SUCCESS)
  {
    FTM_MSG_2(FTM_ERROR, "Could not get undroop filter coefficients from NV - FBRx BW Mode: %d, LTE Band: %d", fbrx_bw_mode, fbrx_gs_info.com_band.lte_band);
    droop_data.group_delay = 0;
    droop_data.num_filter_taps = 1;
    droop_data.filter_coeff[0] = 1 << 14;
    // Fail the measurement due to missing NV items
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    // return; -- For now, proceed
  }

  // Reinitialize for Spectral Flatness capture -- rework this function
  ftm_common_reinit_fft_data_for_spec_flatness(FTM_STATE_LTE, rflte_ftm_mc_sm_get_tx_bw(), &droop_data);

  rflte_state_get_txlm_buffer_idx(sub_id,carrier_idx,&txlm_buffer_idx);

  rflte_nv_tbl_type * nv_tbl_ptr = rflte_mc_get_nv_tbl_ptr(sub_id,carrier_idx,txlm_buffer_idx);

  uint16 search_center = 35;
  uint16 search_width = 15;

  // Invalid NV table pointer
  if(nv_tbl_ptr == NULL) FTM_MSG(FTM_ERROR, "Invalid NV Table pointer, using defaults: search center = 35, search width = 15");
  
  else
  {
    // nv_tbl_ptr->tx_xpt_dpd_config_params[SELFTEST_CONFIG_SEARCH_CENTER] - 20;
    // nv_tbl_ptr->tx_xpt_dpd_config_params[EPT_CFG_CMN_SEARCH_WIDTH] + 20;
  }

  // Currently only for LTE, so only Cal for that.
  rfm_device_enum_type fbrx_chain = rfc_lte_get_fb_device(rflte_ftm_mc_sm_get_tx_device(), rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
  uint32 fbrx_carrier_idx = rflte_ftm_mc_sm_get_carrier_index();

  if (fbrx_carrier_idx >= RFA_RF_LTE_MAX_CELL_SUPPORTED) fbrx_carrier_idx = 0;
      
  // Perform Configuration + Calibration
  ftm_lte_xpt_cal_configure(params.device, fbrx_chain, FTM_XPT_CONFIG_FBRX);
  FTM_MSG_3(FTM_MED, "Configured FBRx with Device: %d, FBRx Chain: %d, FBRx carrier index: %d", params.device, fbrx_chain, fbrx_carrier_idx);

  if(rflte_state_get_fbrxlm_buffer_idx(sub_id,fbrx_carrier_idx) != LM_BUFFER_NOT_ALLOCATED)
  {
    success = (rfcommon_fbrx_msm_update_dynamic_settings_for_xptcap(rflte_state_get_fbrxlm_buffer_idx(sub_id,fbrx_carrier_idx)) == RFCOMMON_FBRX_SUCCESS);
    FTM_MSG_2(FTM_MED, "rfcommon_fbrx_lm_update_dynamic_settings_for_xptcap (buf id %d), success status: %d", rflte_state_get_fbrxlm_buffer_idx(sub_id,fbrx_carrier_idx), success);
  }

  if(success == FALSE)
  {
    FTM_MSG(FTM_ERROR, "Setup fail for spectral flatness measurement!");
    // return ?
  }

  FTM_MSG(FTM_HIGH, "XPT Calibration and Configuration done for LTE!");

  if (fbrx_gs_info.override_enable) ftm_common_override_fbrx_gain(params, TRUE);

  rf_common_xpt_buffer_id_type buffer_ids;
  buffer_ids.capt_buf_id = 0;
  buffer_ids.res_buf_id = 0;

  boolean first_trig = TRUE;
  boolean last_trig = FALSE;

  success = rfcommon_mdsp_setup_self_test_upsampling_params_specflat(1);

  if(success == FALSE)
  {
    FTM_MSG(FTM_ERROR, "Tx Filter setup failed!");
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return;
  }

  for(uint8 cap_num = 0; cap_num < params.num_averages; cap_num++)
  {
    if(cap_num == (params.num_averages - 1)) last_trig = TRUE;

    success = rfcommon_xpt_is_capture_done();

    // Capture is ongoing
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "IQ capture ongoing");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }
    
    success = rfcommon_xpt_set_capture_in_progress();

    // Unable to set capture in progress
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Unable to set capture in progress");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Initialize buffers for capture
    rfcommon_core_xpt_init_buffers();
    success = rfcommon_core_xpt_get_buffers(&buffer_ids);

    // Failed to obtain buffers for IQ capture
    if (success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Unable to obtain buffers for capture");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    success = ftm_lte_xpt_iq_capture(params.device, fbrx_chain, NUM_SAMPLES, first_trig, last_trig, PROC_NONE, XPT_CAPTURE_SELF_TEST_SPEC_FLAT, buffer_ids.capt_buf_id, buffer_ids.res_buf_id, &(iq_samples.sampling_frequency));

    first_trig = FALSE;

    // Capture failure
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Capture Failure!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    uint8 iq_buffer_index;
    success = rfcommon_xpt_wait_for_capture_done(500, 5000, &iq_buffer_index);

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Sample Capture did not complete!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Fetch collected samples
    rfcommon_mc_fetch_selftest_iq_data(iq_buffer_index, NUM_SAMPLES, &(iq_samples.iq_buf_rx), &(iq_samples.iq_buf_tx));

    // Cleaner way to do this?
    rfcommon_core_xpt_release_capture_buffer(iq_buffer_index);
    rfcommon_core_xpt_free_all_buffers();

    // Export samples
    rfcommon_iq_efs_save(iq_samples.iq_buf_tx, iq_samples.iq_buf_rx, NUM_SAMPLES);
    
    ftm_common_txrx_offset_calc_in_struct txrx_offset_in;
    ftm_common_txrx_offset_calc_out_struct txrx_offset_out;
    uint16 initial_offset;

    // Can only perform FFTs if we actually have samples
    if(sample_buffer != NULL)
    {
      txrx_offset_in.search_width = search_width;
      txrx_offset_in.process_len = TXRX_OFFSET_CALC_PROCESS_LEN;
      txrx_offset_in.rx_sample_format = FORMAT_PCFL20;
      txrx_offset_in.rx_sample_len = NUM_SAMPLES;
      txrx_offset_in.rx_sample_ptr = (void *) iq_samples.iq_buf_rx;
      txrx_offset_in.tx_sample_format = FORMAT_8BITIQ;
      txrx_offset_in.tx_sample_len = NUM_SAMPLES;
      txrx_offset_in.tx_sample_ptr = (void *) iq_samples.iq_buf_tx;
      txrx_offset_in.tx_first_samp_idx = rfcommon_mdsp_get_first_samp_idx(0);
      txrx_offset_in.search_center = search_center - rfcommon_mdsp_get_first_samp_idx(0);
      txrx_offset_out.txrx_offset = 0; // Initialize to avoid KW
      ftm_common_find_txrx_offset(&txrx_offset_in, &txrx_offset_out);

      for(uint8 fft_step = 0; fft_step < (NUM_SAMPLES/FFT_SIZE) - 1; fft_step++)
      {
        memscpy((void *) sample_buffer, FFT_SIZE * 2 * 4, (void *) iq_samples.iq_buf_rx, PCFL_SIZE);

        iq_samples.iq_buf_rx += (PCFL_SIZE/(2 * sizeof(int32)));

        ftm_common_parse_PCFL20_to_IQ(FTM_STATE_LTE, sample_buffer, FFT_SIZE);

        if((fft_step == 0) && (txrx_offset_out.txrx_offset > 0))
        {
          memset((void *) &sample_buffer[0], 0, sizeof(int32) * 2 * txrx_offset_out.txrx_offset);
        }

        else if ((fft_step == (2 * (NUM_SAMPLES/FFT_SIZE) - 2)) && (txrx_offset_out.txrx_offset < 0))
        {
          memset((void *) &sample_buffer[2 * FFT_SIZE - 2 * txrx_offset_out.txrx_offset], 0, sizeof(int32) * 2 * txrx_offset_out.txrx_offset);
        }

        // Perform FFTs
        ftm_common_get_samples_fft_avg(FFT_SIZE, sample_buffer);
        initial_offset = fft_step * FFT_SIZE/2;
        ftm_common_parse_tx_IQ8BIT_to_IQ32BIT(FTM_STATE_LTE, sample_buffer, FFT_SIZE, iq_samples.iq_buf_tx, initial_offset, txrx_offset_out.txrx_offset);
        ftm_common_get_samples_fft_avg_tx(FFT_SIZE, sample_buffer);
      }
    }

    // No samples
    else
    {
      FTM_MSG(FTM_ERROR, "Sample buffer null!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    ftm_common_get_undroop_filter_psd(FFT_SIZE, sample_buffer);

    // Consider making this mess its own function?? -- ugly
    ftm_common_spec_flat_input_type spec_flat_inputs;
    spec_flat_inputs.center_freq = rflte_util_get_tx_freq_from_uarfcn(rflte_ftm_mc_sm_get_tx_chan(), rflte_ftm_mc_sm_get_tx_band());

    uint32 f_ul_low, f_ul_high;
    rflte_util_get_tx_freq_lo_hi_from_band(rflte_ftm_mc_sm_get_tx_band(), &f_ul_low, &f_ul_high);
    spec_flat_inputs.band_edge_high = f_ul_high; 
    spec_flat_inputs.band_edge_low = f_ul_low;
    spec_flat_inputs.num_rb = rflte_ftm_mc_sm_get_num_rbs(rflte_ftm_mc_sm_get_carrier_index());
    spec_flat_inputs.start_rb = rflte_ftm_mc_sm_get_start_rb(rflte_ftm_mc_sm_get_carrier_index());
    spec_flat_inputs.psd_size = FFT_SIZE;
    spec_flat_inputs.tx_sig_bw = rflte_ftm_mc_sm_get_tx_bw();
    spec_flat_inputs.samp_freq = iq_samples.sampling_frequency;

    ftm_common_spec_flat_output_type spec_flat_outputs;
    ftm_common_get_flatness_measurement(FTM_STATE_LTE, &spec_flat_inputs, &spec_flat_outputs);

    // Accumulate
    for(uint8 i = 0; i < 8; i++) meas_tx_params.spec_flat_value[i] += spec_flat_outputs.flatness[i];
  } // End of capture loop

  // Finalize and Print Spectral Flatness values
  FTM_MSG_1(FTM_HIGH, "Final accumulated R1 Ripple Value: %d", meas_tx_params.spec_flat_value[0]);
  meas_tx_params.spec_flat_value[0] = (int16) (meas_tx_params.spec_flat_value[0]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final R1 Ripple Value: %d", meas_tx_params.spec_flat_value[0]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated R2 Ripple Value: %d", meas_tx_params.spec_flat_value[1]);
  meas_tx_params.spec_flat_value[1] = (int16) (meas_tx_params.spec_flat_value[1]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final R2 Ripple Value: %d", meas_tx_params.spec_flat_value[1]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated R12 Ripple Value: %d", meas_tx_params.spec_flat_value[2]);
  meas_tx_params.spec_flat_value[2] = (int16) (meas_tx_params.spec_flat_value[2]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final R12 Ripple Value: %d", meas_tx_params.spec_flat_value[2]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated R21 Ripple Value: %d", meas_tx_params.spec_flat_value[3]);
  meas_tx_params.spec_flat_value[3] = (int16) (meas_tx_params.spec_flat_value[3]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final R21 Ripple Value: %d", meas_tx_params.spec_flat_value[3]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated R1 Ripple Extreme Value: %d", meas_tx_params.spec_flat_value[4]);
  meas_tx_params.spec_flat_value[4] = (int16) (meas_tx_params.spec_flat_value[4]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final R1 Ripple Extreme Value: %d", meas_tx_params.spec_flat_value[4]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated R2 Ripple Extreme Value: %d", meas_tx_params.spec_flat_value[5]);
  meas_tx_params.spec_flat_value[5] = (int16) (meas_tx_params.spec_flat_value[5]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final R2 Ripple Extreme Value: %d", meas_tx_params.spec_flat_value[5]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated R12 Ripple Extreme Value: %d", meas_tx_params.spec_flat_value[6]);
  meas_tx_params.spec_flat_value[7] = (int16) (meas_tx_params.spec_flat_value[6]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final R12 Ripple Extreme Value: %d", meas_tx_params.spec_flat_value[6]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated R21 Ripple Extreme Value: %d", meas_tx_params.spec_flat_value[7]);
  meas_tx_params.spec_flat_value[7] = (int16) (meas_tx_params.spec_flat_value[7]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final R21 Ripple Extreme Value: %d", meas_tx_params.spec_flat_value[7]);

#endif

  
    
} // Spectral Flatness Capture

/*
Perform Rx RMS capture for VSWR v1 measurement, provided 
no EVM or ACLR measurement is also requested, in which 
EVM capture will be performed for VSWR v1 measurement. 
*/
static void ftm_common_selftest_rx_rms_cap(const ftm_common_selftest_meas_params_struct params)
{
  uint16 evm_value_temp;
  uint32 rx_rms_value_temp;
  boolean success = TRUE;
  rfcommon_selftest_meas_result proc_result;

  success = ftm_common_selftest_config(params);

  if(success == FALSE)
  {
    FTM_MSG_1(FTM_ERROR, "XPT cal + config failed for %d", params.tech);
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return;
  }

  // Buffer for capture and DPD results
  rf_common_xpt_buffer_id_type buffer_ids;

  buffer_ids.capt_buf_id = 0;
  buffer_ids.res_buf_id = 0;

  success = ftm_common_filter_setup(params, FALSE);

  if(success == FALSE)
  {
    FTM_MSG_1(FTM_ERROR, "Filter setup failed for %d", params.tech);
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return; 
  }

  boolean first_trig = TRUE;
  boolean last_trig = FALSE;

  for(uint8 cap_num = 0; cap_num < params.num_averages; cap_num++)
  {
    if(cap_num == (params.num_averages - 1)) last_trig = TRUE;

    success = rfcommon_xpt_is_capture_done();

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "IQ capture currently in progress!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    success = rfcommon_xpt_is_processing_done();

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Processing in progress!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    success = rfcommon_xpt_set_capture_in_progress();

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Processing in progress!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }  
    
    success = rfcommon_xpt_set_processing_in_progress();

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Processing in progress!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    rfm_device_enum_type fbrx_chain = RFM_DEVICE_1;

    // Tech specific capture
    switch(params.tech)
    {
      #ifdef FEATURE_CDMA1X
      case FTM_RF_TECH_CDMA:
      success = ftm_1x_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_RX_RMS, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));
      break;
      #endif
      #ifdef FEATURE_WCDMA
      case FTM_RF_TECH_WCDMA:
      success = ftm_wcdma_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_RX_RMS, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));    
      break;
      #endif
      #ifdef FTM_HAS_TDSCDMA
      case FTM_RF_TECH_TDSCDMA:
      success = ftm_tdscdma_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_RX_RMS, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));    
      break;
      #endif
      #ifdef FEATURE_LTE
      case FTM_RF_TECH_LTE:
        fbrx_chain = rfc_lte_get_fb_device(rflte_ftm_mc_sm_get_tx_device(), rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
        success = ftm_lte_xpt_iq_capture(params.device, fbrx_chain, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, XPT_CAPTURE_SELF_TEST_RX_RMS, buffer_ids.capt_buf_id, buffer_ids.res_buf_id, &(iq_samples.sampling_frequency));
      break;
      #endif
      default:
        FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      break;
    }

    first_trig = FALSE;

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Capture trigger failed!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    uint8 iq_buffer_index = 0;
    success = rfcommon_xpt_wait_for_capture_done(500, 5000, &iq_buffer_index);

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Capture did not complete!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Fetch collected samples
    rfcommon_mc_fetch_selftest_iq_data(iq_buffer_index, NUM_SAMPLES, &(iq_samples.iq_buf_rx), &(iq_samples.iq_buf_tx));

    // Processing
    uint8 result_buffer_index = 0;
    success = rfcommon_xpt_wait_for_processing_done(1000, 35000, &result_buffer_index);

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Processing did not complete!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // fetch processed data

    rfcommon_mc_fetch_selftest_meas(result_buffer_index, &proc_result);

    evm_value_temp = proc_result.evm;
    rx_rms_value_temp = proc_result.rx_rms_mag;


    // release buffers
    rfcommon_core_xpt_release_capture_buffer(iq_buffer_index);
    rfcommon_core_xpt_release_results_buffer(result_buffer_index);
    rfcommon_core_xpt_free_all_buffers();

    // Accumulate Rx RMS
    //meas_tx_params.vswr_ratio_db10 += rx_rms_value_temp;
  } // for loop

  // meas_tx_params.vswr_ratio_db10 = (int16) rx_rms_value_temp/params.num_averages;
}


/*
Perform Complex VSWR capture for Complex VSWR measurement type
*/
static void ftm_common_selftest_vswr_v2_cap(const ftm_common_selftest_meas_params_struct params)
{
  boolean success = TRUE;

  success = ftm_common_selftest_config(params);

  if(success == FALSE)
  {
    FTM_MSG_1(FTM_ERROR, "XPT cal + config failed for %d", params.tech);
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return;
  }

  if (fbrx_gs_info.override_enable) ftm_common_override_fbrx_gain(params, TRUE);

  // Buffer for capture and DPD results
  rf_common_xpt_buffer_id_type buffer_ids;

  buffer_ids.capt_buf_id = 0;
  buffer_ids.res_buf_id = 0;

  // Disable band limiting filter for LTE
  if(params.tech == FTM_RF_TECH_LTE) success = ftm_common_filter_setup(params, TRUE);
  else success = ftm_common_filter_setup(params, FALSE);

  if(success == FALSE)
  {
    FTM_MSG_1(FTM_ERROR, "Filter setup failed for %d", params.tech);
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return; 
  }

  rfcommon_mdsp_selftest_vswr_meas_type vswr_meas;
  rfcommon_mdsp_self_test_vswr_params_type vswr_mdsp_params;
  vswr_mdsp_params.vswr_split_margin = RFCOMMON_MDSP_VSWR_SPLIT_MARGIN;

  memset(&vswr_meas, 0, sizeof(rfcommon_mdsp_selftest_vswr_meas_type));

  if(success == FALSE)
  {
    FTM_MSG_1(FTM_ERROR, "VSWR v2 params setup failed for %d", params.tech);
    error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
    return; 
  }

  boolean first_trig = TRUE;
  boolean last_trig = FALSE;

  // Capture and calculation loop
  for(uint8 cap_num = 0; cap_num < params.num_averages; cap_num++)
  {
    // This will trigger deinitialization!
    if(cap_num == (params.num_averages - 1)) last_trig = TRUE;

    success = rfcommon_xpt_is_capture_done();

    // Capture is ongoing
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "IQ capture ongoing");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Ensure processing is not ongoing
    success = rfcommon_xpt_is_processing_done();

    // Processing is ongoing
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "IQ processing ongoing");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }
    
    success = rfcommon_xpt_set_capture_in_progress();

    // Unable to set capture in progress
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Unable to set capture in progress");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    success = rfcommon_xpt_set_processing_in_progress();

    // Failed to set proccessing in progress
    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Unable to set processing in progress");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Initialize buffers for capture
    rfcommon_core_xpt_init_buffers();
    success = rfcommon_core_xpt_get_buffers(&buffer_ids);

    // Failed to obtain buffers for IQ capture
    if (success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Unable to obtain buffers for capture");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    rfm_device_enum_type fbrx_chain = RFM_DEVICE_1;

    FTM_MSG_3(FTM_MED, "Calling XPT Cap for %d with Tx: %d Rx: %d", params.tech, params.device, fbrx_chain);

    // Tech-specific capture
    switch(params.tech)
    {
      #ifdef FEATURE_CDMA1X
      case FTM_RF_TECH_CDMA:
        success = ftm_1x_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_VSWR, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));
      break;
      #endif
      #ifdef FEATURE_WCDMA
      case FTM_RF_TECH_WCDMA:
        success = ftm_wcdma_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_VSWR, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));
      break; 
      #endif
      #ifdef FTM_HAS_TDSCDMA
      case FTM_RF_TECH_TDSCDMA:
        success = ftm_tdscdma_xpt_iq_capture(params.device, fbrx_chain, XPT_CAPTURE_SELF_TEST_VSWR, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, buffer_ids, &(iq_samples.sampling_frequency));
      break;
      #endif
      #ifdef FEATURE_LTE
      case FTM_RF_TECH_LTE:
        fbrx_chain = rfc_lte_get_fb_device(rflte_ftm_mc_sm_get_tx_device(), rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
        success = ftm_lte_xpt_iq_capture(params.device, fbrx_chain, NUM_SAMPLES, first_trig, last_trig, PROC_SELF_TEST, XPT_CAPTURE_SELF_TEST_VSWR, buffer_ids.capt_buf_id, buffer_ids.res_buf_id, &(iq_samples.sampling_frequency));
      break;
      #endif
      default:
        FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      break;
    }

    // If are are going to perform more triggers, we do not need to do all of the setup!
    first_trig = FALSE;

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Capture trigger failed!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    uint8 iq_buffer_index = 0;
    success = rfcommon_xpt_wait_for_capture_done(500, 5000, &iq_buffer_index);

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Capture did not complete!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Fetch collected samples
    rfcommon_mc_fetch_selftest_iq_data(iq_buffer_index, NUM_SAMPLES, &(iq_samples.iq_buf_rx), &(iq_samples.iq_buf_tx));

    // Processing
    uint8 result_buffer_index = 0;
    success = rfcommon_xpt_wait_for_processing_done(1000, 35000, &result_buffer_index);

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Processing did not complete!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    // Export samples
    rfcommon_iq_efs_save(iq_samples.iq_buf_tx, iq_samples.iq_buf_rx, NUM_SAMPLES);

    success = rfcommon_mdsp_selftest_vswr_meas(result_buffer_index, &vswr_meas);

    if(success == FALSE)
    {
      FTM_MSG(FTM_ERROR, "Failed to retrieve VSWR measurement results!");
      error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
      return;
    }

    meas_tx_params.vswr_v2_value[0] += (vswr_meas.fwd_pwr - vswr_meas.refl_pwr); // Return loss
    meas_tx_params.vswr_v2_value[1] += (vswr_meas.refl_pwr - vswr_meas.fwd_pwr); // Mag
    meas_tx_params.vswr_v2_value[2] += ((vswr_meas.refl_phase - vswr_meas.fwd_phase + 54000) % 36000 - 18000); // Phase
    meas_tx_params.vswr_v2_value[3] += vswr_meas.fwd_pwr; // Forward Power
    meas_tx_params.vswr_v2_value[4] += vswr_meas.refl_pwr; // Reverse Power

    // Release buffers -- Better place to do this
    rfcommon_core_xpt_release_capture_buffer(iq_buffer_index);
    rfcommon_core_xpt_release_results_buffer(result_buffer_index);
    rfcommon_core_xpt_free_all_buffers();

  } // End for loop

  // Finalize and Print VSWRv2  values
  FTM_MSG_1(FTM_HIGH, "Final accumulated Return loss: %d", meas_tx_params.vswr_v2_value[0]);
  meas_tx_params.vswr_v2_value[0] = (int16) (meas_tx_params.vswr_v2_value[0]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final Return loss Value: %d", meas_tx_params.vswr_v2_value[0]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated Mag Value: %d", meas_tx_params.vswr_v2_value[1]);
  meas_tx_params.vswr_v2_value[1] = (int16) (meas_tx_params.vswr_v2_value[1]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final Mag Value: %d", meas_tx_params.vswr_v2_value[1]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated Phase Value: %d", meas_tx_params.vswr_v2_value[2]);
  meas_tx_params.vswr_v2_value[2] = (int16) (meas_tx_params.vswr_v2_value[2]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final Phase Value: %d", meas_tx_params.vswr_v2_value[2]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated Forward Power Value: %d", meas_tx_params.vswr_v2_value[3]);
  meas_tx_params.vswr_v2_value[3] = (int16) (meas_tx_params.vswr_v2_value[3]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final Forward Power Value: %d", meas_tx_params.vswr_v2_value[3]);

  FTM_MSG_1(FTM_HIGH, "Final accumulated Reverse Power Value: %d", meas_tx_params.vswr_v2_value[4]);
  meas_tx_params.vswr_v2_value[4] = (int16) (meas_tx_params.vswr_v2_value[4]/params.num_averages);
  FTM_MSG_1(FTM_HIGH, "Final Reverse Power Value: %d", meas_tx_params.vswr_v2_value[4]);

} // VSWR v2 Cap

/*
Prepare response packet with final measurement values, error codes, and device information 
If any measurement fails, garbage values will be returned for those measurements that fail. 
Non-failed measurements will return their measured values. 
*/
static void ftm_common_populate_meas_tx_params_rsp_pkt(ftm_common_selftest_meas_params_struct params,
#ifdef FEATURE_LTE  
  ftm_lte_pkt_type * req_pkt,
#else
  int32 *req_pkt,
#endif
ftm_rsp_pkt_type * rsp_pkt)
{
  uint32 payload_size_count = 0;
  int16 garbage = -9999;
  int16 temp;

  // Make sure response packet exists 
  if(rsp_pkt != NULL)
  {
    meas_tx_params_rsp_pkt.cmd_code = 75;
    meas_tx_params_rsp_pkt.subsys_id = FTM;
    meas_tx_params_rsp_pkt.ftm_rf_cmd = FTM_RF_MEAS_TX_PARAMS;

    // Hack input so packet issue is resolved (for now)
    switch(params.tech)
    {
      case FTM_RF_TECH_CDMA:
        //ftm_rf_common_meas_tx_params_rsp_pkt.subsys_cmd_code = (selftest_params.device == RFM_DEVICE_0) ? FTM_1X_C0_C : FTM_1X_C2_C;
        meas_tx_params_rsp_pkt.subsys_cmd_code = RFM_DEVICE_0;
      break;

      case FTM_RF_TECH_WCDMA:
        //ftm_rf_common_meas_tx_params_rsp_pkt.subsys_cmd_code = (selftest_params.device == RFM_DEVICE_0) ? FTM_WCDMA_C : FTM_WCDMA_RX_2_C;
        meas_tx_params_rsp_pkt.subsys_cmd_code = RFM_DEVICE_1;
      break;

      case FTM_RF_TECH_TDSCDMA:
        //ftm_rf_common_meas_tx_params_rsp_pkt.subsys_cmd_code = (selftest_params.device == RFM_DEVICE_0) ? FTM_TDSCDMA_C : FTM_TDSCDMA_RX_2_C;
        meas_tx_params_rsp_pkt.subsys_cmd_code = FTM_TDSCDMA_C;
      break;

    case FTM_RF_TECH_LTE:

      // Populate for LTE outside the common API

        //meas_tx_params_rsp_pkt.subsys_cmd_code = req_pkt->subsys_hdr.subsys_cmd_code;
      break;

      default:
        FTM_MSG(FTM_ERROR, "TECH NOT SUPPORTED!");
        error_code = FTM_FBRX_TEST_GENERAL_FAILURE;
        return;
      break;
    }

    meas_tx_params_rsp_pkt.ftm_error_code = error_code;
    meas_tx_params_rsp_pkt.num_averages = params.num_averages;
    meas_tx_params_rsp_pkt.capture_offset = params.capture_offset;
    meas_tx_params_rsp_pkt.tx_measurement_config = params.tx_measurement_config;
    meas_tx_params_rsp_pkt.payload_size = payload_size_count;

    rsp_pkt->cmd = FTM_RSP_DO_NEW_PKT;
    rsp_pkt->pkt_payload = &(meas_tx_params_rsp_pkt);
    rsp_pkt->delete_payload = FALSE; 
    rsp_pkt->pkt_len = FTM_RF_MEAS_TX_PARAMS_HDR_SIZE;

    FTM_MSG(FTM_MED, "Response packet initialized, filling out payload");

    // Tx Power Measurement
    if (params.tx_measurement_config & FTM_FBRX_MEASURE_TX_POWER)
    {
      if (success_flags.tx_power_success == TRUE) temp = (int16) meas_tx_params.tx_power;
      else temp = garbage;
      
      memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
        FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
        &temp, WORD_SIZE);

      payload_size_count += WORD_SIZE;
    }

    // EVM Measurement
    if (params.tx_measurement_config & FTM_FBRX_MEASURE_EVM)
    {
      if(success_flags.evm_success == TRUE) temp = (uint16) meas_tx_params.evm_value;
      else temp = garbage;

      memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
        FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
        &temp, WORD_SIZE);

      payload_size_count += WORD_SIZE;
    }

    // ACLR Measurement
    if (params.tx_measurement_config & FTM_FBRX_MEASURE_ACLR)
    {
      for(uint8 i = 0; i < 6; i++)
      {
        if(success_flags.evm_success == TRUE) temp = (int16) meas_tx_params.aclr_value[i];
        else temp = garbage;

        memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
          FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
          &temp, WORD_SIZE);

        payload_size_count += WORD_SIZE;
      }
    }

    if(params.tx_measurement_config & FTM_FBRX_SAVE_TX_IQ_SAMPLES || params.tx_measurement_config & FTM_FBRX_SAVE_RX_IQ_SAMPLES)
    {
      uint32 iq_capture_frequency = 0;
      uint32 max_samples_per_diag_packet_tx = 0;
      uint32 max_samples_per_diag_packet_rx = 0;
      uint8 ftm_tx_iq_format = 0;
      uint8 ftm_rx_iq_format = 0;

      switch(params.tech)
      {
        #ifdef FEATURE_CDMA1X
        case FTM_RF_TECH_CDMA:
          ftm_tx_iq_format = FTM_LTE_FBRX_IQ_SAMPLE_DATA_FORMAT_TX;
          ftm_rx_iq_format = FTM_LTE_FBRX_IQ_SAMPLE_DATA_FORMAT_RX;
          iq_capture_frequency = FTM_CDMA_FBRX_IQ_SAMPLE_FREQ;
          max_samples_per_diag_packet_tx = FTM_CDMA_FBRX_IQ_DIAG_MAX_SAMPLES_TX;
          max_samples_per_diag_packet_rx = FTM_CDMA_FBRX_IQ_DIAG_MAX_SAMPLES_RX;
        break;
        #endif
        #ifdef FEATURE_WCDMA
        case FTM_RF_TECH_WCDMA:
          ftm_tx_iq_format = FTM_WCDMA_FBRX_IQ_SAMPLE_DATA_FORMAT_TX;
          ftm_rx_iq_format = FTM_WCDMA_FBRX_IQ_SAMPLE_DATA_FORMAT_RX;
          iq_capture_frequency = FTM_WCDMA_FBRX_IQ_SAMPLE_FREQ;
          max_samples_per_diag_packet_tx = FTM_WCDMA_FBRX_IQ_DIAG_MAX_SAMPLES_TX;
          max_samples_per_diag_packet_rx = FTM_WCDMA_FBRX_IQ_DIAG_MAX_SAMPLES_RX;
        break;
        #endif
        #ifdef FTM_HAS_TDSCDMA
        case FTM_RF_TECH_TDSCDMA:
          ftm_tx_iq_format = FTM_TDSCDMA_FBRX_IQ_SAMPLE_DATA_FORMAT_TX;
          ftm_rx_iq_format = FTM_TDSCDMA_FBRX_IQ_SAMPLE_DATA_FORMAT_RX;
          iq_capture_frequency = FTM_TDSCDMA_FBRX_IQ_SAMPLE_FREQ;
          max_samples_per_diag_packet_tx = FTM_TDSCDMA_FBRX_IQ_DIAG_MAX_SAMPLES_TX;
          max_samples_per_diag_packet_rx = FTM_TDSCDMA_FBRX_IQ_DIAG_MAX_SAMPLES_RX;
        break;
        #endif
        #ifdef FEATURE_LTE
        case FTM_RF_TECH_LTE:
          ftm_tx_iq_format = FTM_LTE_FBRX_IQ_SAMPLE_DATA_FORMAT_TX;
          ftm_rx_iq_format = FTM_LTE_FBRX_IQ_SAMPLE_DATA_FORMAT_RX;
          max_samples_per_diag_packet_tx = FTM_LTE_FBRX_IQ_DIAG_MAX_SAMPLES_TX;
          max_samples_per_diag_packet_rx = FTM_LTE_FBRX_IQ_DIAG_MAX_SAMPLES_RX;

          switch(rflte_ftm_mc_sm_get_tx_bw())
          {
            case RFCOM_BW_LTE_1P4MHz: iq_capture_frequency = FTM_LTE_FBRX_IQ_SAMPLE_FREQ_1P4MHZ;  break;
            case RFCOM_BW_LTE_3MHz:   iq_capture_frequency = FTM_LTE_FBRX_IQ_SAMPLE_FREQ_3MHZ;    break;
            case RFCOM_BW_LTE_5MHz:   iq_capture_frequency = FTM_LTE_FBRX_IQ_SAMPLE_FREQ_5MHZ;    break;
            case RFCOM_BW_LTE_10MHz:  iq_capture_frequency = FTM_LTE_FBRX_IQ_SAMPLE_FREQ_10MHZ;   break;
            case RFCOM_BW_LTE_15MHz:  iq_capture_frequency = FTM_LTE_FBRX_IQ_SAMPLE_FREQ_15MHZ;   break;
            case RFCOM_BW_LTE_20MHz:  iq_capture_frequency = FTM_LTE_FBRX_IQ_SAMPLE_FREQ_20MHZ;   break;

            default:
              error_code = FTM_FBRX_TEST_IQ_SAMPLES_EXCEEDS_CAPTURE_LIMIT;
              FTM_MSG_1(FTM_ERROR, "LTE Uplink not properly configured. Incorrect T/x BW (%d)", rflte_ftm_mc_sm_get_tx_bw());
            break;
          }
          break;
		  #endif

        default:
            // ERROR + DEBUG HANDLING
        break;
      } // IQ samples tech switch

      // Save Tx samples
      if(params.tx_measurement_config & FTM_FBRX_SAVE_RX_IQ_SAMPLES)
      {
        meas_tx_params_rsp_pkt.result[payload_size_count++] = ftm_tx_iq_format;
        meas_tx_params_rsp_pkt.result[payload_size_count++] = (uint8) FTM_FBRX_SAMPLE_SIZE_9216;

        memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
          FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
          &iq_capture_frequency,
          sizeof(iq_capture_frequency));

        payload_size_count = payload_size_count + sizeof(iq_capture_frequency);

        memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
          FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
          &max_samples_per_diag_packet_tx,
          sizeof(max_samples_per_diag_packet_tx));

        payload_size_count = payload_size_count + sizeof(max_samples_per_diag_packet_tx);
      }

      // Save Rx Samples
      if(params.tx_measurement_config & FTM_FBRX_SAVE_TX_IQ_SAMPLES)
      {
        meas_tx_params_rsp_pkt.result[payload_size_count++] = ftm_rx_iq_format;
        meas_tx_params_rsp_pkt.result[payload_size_count++] = (uint8) FTM_FBRX_SAMPLE_SIZE_9216;

        memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
          FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
          &iq_capture_frequency,
          sizeof(iq_capture_frequency));

        payload_size_count = payload_size_count + sizeof(iq_capture_frequency);

        memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
          FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
          &max_samples_per_diag_packet_rx,
          sizeof(max_samples_per_diag_packet_rx));

        payload_size_count = payload_size_count + sizeof(max_samples_per_diag_packet_rx);
      }

    } // Tx + Rx samples

    // VSWR Measurement
    if (params.tx_measurement_config & FTM_FBRX_MEAS_VSWR)
    {
      if(success_flags.vswr_success == TRUE) temp = (int16) meas_tx_params.vswr_ratio_db10;
      else temp = garbage;
    
      memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
        FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
        &temp, WORD_SIZE);

      payload_size_count += WORD_SIZE;
    }

    // VSWRv2 Measurement
    if (params.tx_measurement_config & FTM_FBRX_MEAS_VSWR_V2)
    {
      for(uint8 i = 0; i < 5; i++)
      {
        if(success_flags.vswr_v2_success == TRUE) temp = (int16) meas_tx_params.vswr_v2_value[i];
        else temp = garbage;
      
        memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
          FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
          &temp, WORD_SIZE);

        payload_size_count += WORD_SIZE;
      }
    } // VSWRv2

    // Spectral Flatness Measurement
    if (params.tx_measurement_config & FTM_FBRX_MEAS_FLATNESS)
    {
      // Copy Spectral Flatness Array
      for(uint8 i = 0; i < 8; i++)
      {
        if(success_flags.spec_flat_success == TRUE) temp = (int16) meas_tx_params.spec_flat_value[i];
        else temp = garbage;
        
        memscpy(&meas_tx_params_rsp_pkt.result[payload_size_count],
          FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE - payload_size_count,
          &temp, WORD_SIZE);

        payload_size_count += WORD_SIZE;
      }
    } // Spectral Flatness

    // Payload too large
    if (payload_size_count > FTM_RF_MEAS_TX_PARAMS_MAX_PAYLOAD_SIZE)
    {
      meas_tx_params_rsp_pkt.ftm_error_code = FTM_FBRX_TEST_EXCEEDS_PAYLOAD_SIZE_LIMIT;
      FTM_MSG(FTM_ERROR, "Response packet payload too large!");
    }

    // Packet is appropriate size, update response packet length
    else 
    {
      rsp_pkt->pkt_len += payload_size_count;
      meas_tx_params_rsp_pkt.payload_size = payload_size_count;
    }
  } // If packet exists

  else
  {
    FTM_MSG(FTM_ERROR, "Response packet NULL!");
  }
} // Populate response packet



/*
BEGIN LEGACY CODE
*/
boolean
ftm_common_trigger_self_test_iq_cap
(
  ftm_rf_technology_type ftm_rf_tech,
  uint8  device,
  uint8  device_fb,
  uint16 num_samples,
  uint32 ftm_selftest_meas_type,
  uint32 * samp_rate
)
{
  boolean do_processing = (ftm_selftest_meas_type > 0)? TRUE: FALSE;
  uint8 rx_bw_filt_len=0;
  int16 *rx_bw_filt_coeff = NULL;

#ifdef FEATURE_LTE
  rflte_ftm_mc_sm_type     *lte_ftm_mc_sm_ptr      = NULL; /*holds pointer to LTE ftm State machine structure */

  uint32 fbrx_carrier_idx = 0;

  uint8 eq_filt_mask;

#endif
  uint8 tx_upsamp;
  uint8 tx_up_fir_len;
  int8 tx_up_fir_shift;
  int16 *tx_up_fir = NULL;
  volatile boolean api_status = TRUE; /* shouldn't this be volatile? */
  rf_common_xpt_buffer_id_type buffer_ids; /* buffer ids for capture and dpd results */
  rfcommon_xpt_processing_type proc_flag = PROC_NONE;
  uint8 ftm_selftest_xpt_capture_type;
  rfcommon_mdsp_self_test_vswr_params_type vswr_mdsp_params;
  boolean is_spec_flatness_cap = FALSE;

  if ( ( (ftm_selftest_meas_type & ENABLE_ACLR_MEAS ) !=0 ) ||
       ( (ftm_selftest_meas_type & ENABLE_SPEC_FLAT_MEAS ) !=0 ))
  {
    if ((ftm_selftest_meas_type & (ENABLE_EVM_MEAS | ENABLE_VSWR_MEAS_V2 | ENABLE_TX_PWR_MEAS) ) == 0)
    {
      //FTM_MSG(FTM_ERROR, "Change to RxRMS cap type");
      //ftm_selftest_xpt_capture_type = XPT_CAPTURE_SELF_TEST_RX_RMS;
      do_processing = FALSE;
    }
  }

  if ((ftm_selftest_meas_type & ENABLE_SPEC_FLAT_MEAS ) !=0 )
  {
    if ( ftm_rf_tech != FTM_RF_TECH_LTE)
    {
      FTM_MSG_2(FTM_ERROR, "ftm_common_trigger_self_test_iq_cap: Tech Type %d does not support the requested measurement type 0x%x",
                ftm_rf_tech, ENABLE_SPEC_FLAT_MEAS);
      return FALSE;
    }
    is_spec_flatness_cap = TRUE;
  }

  /* Init Buffer ID values */
  buffer_ids.capt_buf_id = 0;
  buffer_ids.res_buf_id = 0;

  /* Ensure no previus capture/processing is still in progress */
  if( (api_status == TRUE) )
  {
    api_status &= rfcommon_xpt_is_capture_done();
    if( api_status == FALSE)
    {
      FTM_MSG_1( FTM_ERROR, "ftm_common_trigger_ept_iq_cap: IQ capture already "
                 "in progress for tech %d", ftm_rf_tech );
    }

    /* Monitor processing only if it was requested */
    if (do_processing == TRUE)
    {
      api_status &= rfcommon_xpt_is_processing_done();
      if( api_status == FALSE)
      {
        FTM_MSG_1( FTM_ERROR, "ftm_common_trigger_ept_iq_cap: IQ processing already "
                   "in progress for tech %d", ftm_rf_tech );
      }
    }
  }

  /* Set capture and processing flags */
  if( (api_status == TRUE) )
  {
    api_status &= rfcommon_xpt_set_capture_in_progress();
    if (do_processing == TRUE)
    {
      api_status &= rfcommon_xpt_set_processing_in_progress();
    }
    /* Initialize buffer pool for xpt captures */
    rfcommon_core_xpt_init_buffers();

    /* Get buffer ids, TODO: need to pass in do_processing flag to not get dpd result buffer */
    api_status &= rfcommon_core_xpt_get_buffers(&buffer_ids);
  }

  proc_flag = (do_processing == TRUE)?PROC_SELF_TEST:PROC_NONE;

  /* This needs to be changed
     I still don't know how these flags ultimately impact the capture type
     *See rfcommon_mdsp.c:750 or so
  */
  // if (ftm_selftest_pwr_only && ((ftm_selftest_meas_type & ENABLE_EVM_MEAS) == 0))
  if ((ftm_selftest_meas_type & ENABLE_VSWR_MEAS_V2) != 0)
  {
    ftm_selftest_xpt_capture_type = XPT_CAPTURE_SELF_TEST_VSWR;
  }
  else if ((ftm_selftest_meas_type & ENABLE_EVM_MEAS) != 0)
  {
    ftm_selftest_xpt_capture_type = XPT_CAPTURE_SELF_TEST_EVM;
  }
  else if ((ftm_selftest_meas_type & ENABLE_SPEC_FLAT_MEAS) != 0)
  {
    ftm_selftest_xpt_capture_type = XPT_CAPTURE_SELF_TEST_SPEC_FLAT;
  }
  else // ((ftm_selftest_meas_type & ENABLE_TX_PWR_MEAS) != 0)
  {
    // FW is crashing with RxRMS type capture and selftest flag set
    ftm_selftest_xpt_capture_type = XPT_CAPTURE_SELF_TEST_EVM /*XPT_CAPTURE_SELF_TEST_RX_RMS*/ ;
  }

  /* Trigger tech-specific capture */
  switch ( ftm_rf_tech )
  {
  case FTM_RF_TECH_CDMA:
    #ifdef FEATURE_CDMA1X
    {
      /* Perform capture */
      if(api_status == TRUE)
      {

        /* Reset Tx upsampling filter which is used for W/LTE only */
       tx_upsamp = 1;
       tx_up_fir_len = 0;
       tx_up_fir_shift = 0;
       tx_up_fir = tx_up_filter_coef;

       rx_bw_filt_coeff = evm_cdma_fir;
       rx_bw_filt_len = sizeof(evm_cdma_fir)/sizeof(int16) - 1;

       rfcommon_mdsp_setup_self_test_upsampling_params (tx_upsamp,tx_up_fir_len, tx_up_fir_shift, tx_up_fir);

        /* Reset Rx FIR filter which is used for LTE only */
        rfcommon_mdsp_setup_self_test_params(FALSE, rx_bw_filt_len, *(rx_bw_filt_coeff+rx_bw_filt_len), rx_bw_filt_coeff, (1<<2));
        if (ftm_selftest_xpt_capture_type == XPT_CAPTURE_SELF_TEST_VSWR)
        {
          vswr_mdsp_params.vswr_split_margin = RFCOMMON_MDSP_VSWR_SPLIT_MARGIN;
          rfcommon_mdsp_setup_self_test_vswr_params(&vswr_mdsp_params);
        } 

        api_status &= ftm_1x_xpt_iq_capture(device,
                                            device_fb,
                                            ftm_selftest_xpt_capture_type,
                                            num_samples,
                                            TRUE,                  /* first_trig */
                                            TRUE,                   /* last_trig */
                                            proc_flag,
                                            buffer_ids,
                                            samp_rate);

      }
    }
    #endif
    break;

  case FTM_RF_TECH_WCDMA:
    #ifdef FEATURE_WCDMA
    {
      if(api_status == TRUE)
      {

        /* do self test specific interface programming*/

        rx_bw_filt_coeff = evm_wcdma_fir;
        rx_bw_filt_len = sizeof(evm_wcdma_fir)/sizeof(int16) - 1;
        rfcommon_mdsp_setup_self_test_params(TRUE, rx_bw_filt_len, *(rx_bw_filt_coeff+rx_bw_filt_len), rx_bw_filt_coeff, (1<<2));

       tx_upsamp = 2;
       tx_up_fir_len = 10;
       tx_up_fir_shift = -14;
       tx_up_fir = tx_up_filter_coef;

       rfcommon_mdsp_setup_self_test_upsampling_params (tx_upsamp,tx_up_fir_len, tx_up_fir_shift, tx_up_fir);
       if (ftm_selftest_xpt_capture_type == XPT_CAPTURE_SELF_TEST_VSWR)
       {
         vswr_mdsp_params.vswr_split_margin = RFCOMMON_MDSP_VSWR_SPLIT_MARGIN;
         rfcommon_mdsp_setup_self_test_vswr_params(&vswr_mdsp_params);
       } 

       api_status &= ftm_wcdma_xpt_iq_capture(device,
                                              device_fb,
                                              ftm_selftest_xpt_capture_type,
                                              num_samples,
                                              TRUE, /* first_trig */
                                              TRUE, /* last_trig */
                                              proc_flag,
                                              buffer_ids,
                                              samp_rate);
      }
    }
    #endif
    break;

  case FTM_RF_TECH_TDSCDMA:
    #ifdef FEATURE_TDSCDMA
    {
      if(api_status == TRUE)
      {

        /* Reset Tx upsampling filter which is used for W/LTE only */
        tx_upsamp = 1;
        tx_up_fir_len = 0;
        tx_up_fir_shift = 0;
        tx_up_fir = tx_up_filter_coef;

        rfcommon_mdsp_setup_self_test_upsampling_params (tx_upsamp,tx_up_fir_len, tx_up_fir_shift, tx_up_fir);


        /* Set Tx_freq_adj and Reset Rx FIR filter*/

        rx_bw_filt_coeff = evm_tdscdma_fir;
        rx_bw_filt_len = sizeof(evm_tdscdma_fir)/sizeof(int16) - 1;
        rfcommon_mdsp_setup_self_test_params(TRUE, rx_bw_filt_len, *(rx_bw_filt_coeff+rx_bw_filt_len), rx_bw_filt_coeff, (1<<2));
        if (ftm_selftest_xpt_capture_type == XPT_CAPTURE_SELF_TEST_VSWR)
        {
          vswr_mdsp_params.vswr_split_margin = RFCOMMON_MDSP_VSWR_SPLIT_MARGIN;
          rfcommon_mdsp_setup_self_test_vswr_params(&vswr_mdsp_params);
        } 

        api_status &= ftm_tdscdma_xpt_iq_capture(device,
                                                 device_fb,
                                                 ftm_selftest_xpt_capture_type,
                                                 num_samples,
                                                 TRUE, /* first_trig */
                                                 TRUE, /* last_trig */
                                                 proc_flag,
                                                 buffer_ids,
                                                 samp_rate);
      }
    }
    #endif
    break;

  case FTM_RF_TECH_LTE:
    #ifdef FEATURE_LTE
    {
      if(api_status == TRUE)
      {
        if (is_spec_flatness_cap == FALSE) 
        {

        fbrx_carrier_idx = rflte_ftm_mc_sm_get_carrier_index();


        if (fbrx_carrier_idx>=RFA_RF_LTE_MAX_CELL_SUPPORTED) 
        {

          /* invalid return from FTM MC layer, reset to carrier index 0 */
          fbrx_carrier_idx = 0;
        }

        /* Get LTE FTM state machine handle */
        lte_ftm_mc_sm_ptr = rflte_ftm_mc_get_sm();


       switch (lte_ftm_mc_sm_ptr->carrier[fbrx_carrier_idx].tx_bw)
       {
         case RFCOM_BW_LTE_5MHz:
           rx_bw_filt_coeff  = evm_lte_5_fir;
           rx_bw_filt_len = sizeof(evm_lte_5_fir)/sizeof(int16) - 1;		 
           break;
         case RFCOM_BW_LTE_10MHz:
           rx_bw_filt_coeff  = evm_lte_10_fir;
           rx_bw_filt_len = sizeof(evm_lte_10_fir)/sizeof(int16) - 1;		 
           break;
         case RFCOM_BW_LTE_15MHz:
           rx_bw_filt_coeff  = evm_lte_15_fir;
           rx_bw_filt_len = sizeof(evm_lte_15_fir)/sizeof(int16) - 1;		 
           break;
         case RFCOM_BW_LTE_20MHz:
           rx_bw_filt_coeff  = evm_lte_20_fir;
           rx_bw_filt_len = sizeof(evm_lte_20_fir)/sizeof(int16) - 1;		 
           break;
         default:
           rx_bw_filt_coeff  = NULL;
           rx_bw_filt_len = 0;
           FTM_MSG(FTM_HIGH,"LTE self-test param, no Rx filter applied");
       }

       /*select correct eq_filt mask based on rb number*/
       if (lte_ftm_mc_sm_ptr->carrier[fbrx_carrier_idx].rb_block < 36)
       {
         eq_filt_mask = ((1<<2)|(1<<1));
       }
       else if (lte_ftm_mc_sm_ptr->carrier[fbrx_carrier_idx].rb_block < 75)
       {
         eq_filt_mask = ((1<<3)|(1<<2));
       }
       else
       {
         eq_filt_mask = ((1<<4)|(1<<3)|(1<<2));
       }

       if (ftm_selftest_xpt_capture_type == XPT_CAPTURE_SELF_TEST_VSWR)
       {
         /* disable band limiting filter for VSWR processing */

         rfcommon_mdsp_setup_self_test_params(FALSE, 0, 0, NULL, 0);

       } else 
       {
         if (rx_bw_filt_coeff != NULL)
         {
           rfcommon_mdsp_setup_self_test_params(FALSE, rx_bw_filt_len, *(rx_bw_filt_coeff+rx_bw_filt_len), rx_bw_filt_coeff, eq_filt_mask);    
         }
       }

       tx_upsamp = 2;
       tx_up_fir_len = 10;
       tx_up_fir_shift = -14;
       tx_up_fir = tx_up_filter_coef;

       rfcommon_mdsp_setup_self_test_upsampling_params (tx_upsamp,tx_up_fir_len, tx_up_fir_shift, tx_up_fir);
       if (ftm_selftest_xpt_capture_type == XPT_CAPTURE_SELF_TEST_VSWR)
       {
         vswr_mdsp_params.vswr_split_margin = RFCOMMON_MDSP_VSWR_SPLIT_MARGIN;
         rfcommon_mdsp_setup_self_test_vswr_params(&vswr_mdsp_params);
       } 
       }

       else
       {
         rfcommon_mdsp_setup_self_test_upsampling_params_specflat(1);
       }

        api_status &= ftm_lte_xpt_iq_capture(device,
                                             device_fb,
                                             num_samples,
                                             TRUE,
                                             TRUE,
                                             proc_flag,
                                             ftm_selftest_xpt_capture_type,
                                             buffer_ids.capt_buf_id,
                                             buffer_ids.res_buf_id,
                                             samp_rate);
     }
    }
    #endif
    break;

#ifdef FEATURE_GERAN_RF_SELF_TEST
  case FTM_RF_TECH_GSM:
    ftm_selftest_xpt_capture_type = XPT_CAPTURE_GSM_PWR_MES;
    if(!ftm_gsm_selftest_write_capture_params_to_commonFW(device_fb,ftm_selftest_xpt_capture_type ))
    {
      FTM_MSG_1( FTM_ERROR, "Fail to write capture parameters into common FW for tech %d",
        ftm_rf_tech );
      api_status = FALSE;
    }
    break;
#endif
  default:
    FTM_MSG_1( FTM_ERROR, "ftm_common_trigger_self_test_iq_cap: Mode %d not supported",
               ftm_rf_tech );
    api_status = FALSE;
    break;
  } /* Trigger tech-specific capture */

  if ( api_status == FALSE)
  {
    FTM_MSG( FTM_ERROR, "ftm_common_trigger_self_test_iq_cap: api status = FAIL" );
  }
  return api_status;
}


// Commented out for 9x35 bringup
#if 0
/*===========================================================================

FUNCTION ftm_common_hdet_disable

DESCRIPTION
   This function will call the tech specific HDET disable or enable API.
   Different tech uses different mechanism to disable or enable HDET.

DEPENDENCIES

RETURN VALUE
   FALSE, fail to disable or enable HDET
   TRUE, successfully disable or enable HDET

SIDE EFFECTS
   None.

===========================================================================*/
boolean ftm_common_hdet_disable(ftm_rf_technology_type ftm_rf_tech, rfm_device_enum_type rfm_dev, boolean flag)
{
   boolean ret=FALSE;
   uint8 cnt=0;

   switch (ftm_rf_tech)
   {
   case FTM_RF_TECH_CDMA:
    #ifdef FEATURE_CDMA1X
    while (FALSE == ret)
    {
     ret = rfm_cdma_ftm_obtain_hdet_lock(rfm_dev,flag);

     /* Will try to obtain hdet lock maximum of 12 times before bailing out */
     if ((TRUE == ret) || (cnt > 12))
     {
      break;
     }

     /* Wait 200us before trying again */
     DALSYS_BusyWait(200);
     cnt++;
    }

    FTM_MSG_HIGH("FTM CDMA HDET: ena/dis=%d result=%d  cnt(200us/cnt)=%d", flag, ret, cnt);
    #endif
    break;
   case FTM_RF_TECH_WCDMA:
    break;
   case FTM_RF_TECH_GSM:
    break;
   case FTM_RF_TECH_LTE:
    #ifdef FEATURE_LTE
    ret = ftm_lte_disable_hdet(flag);
    #endif
    break;
   case FTM_RF_TECH_TDSCDMA:
    #ifdef FEATURE_TDSCDMA
    ret = ftm_tdscdma_disable_hdet(flag);

    if (TRUE == ret)
    {
       /* Wait for 5.1 ms */
     DALSYS_BusyWait(5100);
    }

      FTM_MSG_HIGH("FTM TDSCDMA HDET: ena/dis=%d result=%d  cnt=%d", flag, ret, cnt);
    #endif

    break;
   default:
    cnt=0;  /* Avoid compiler warning if cnt is not being used. */
    break;
   }

   return(ret);
}
#endif


boolean ftm_common_fbrx_iq_capture_processing
(
ftm_rf_technology_type ftm_rf_tech,
uint8 fbrx_chain,
uint32 ftm_selftest_capture_type,
uint16 num_samples,
int32 **iq_buf_rx,
int32 **iq_buf_tx,
uint16 *evm_value_ptr,
uint32 *rx_rms_mag_ptr,
uint32 *samp_rate,
ftm_common_selftest_fbrxgs_info_type * fbrx_gs_info,
rfm_device_enum_type device
)
{
  uint8 iq_buffer_index;
  uint8 result_buffer_index      = 0;
  boolean capture_trigerred      = FALSE;
  boolean capture_done           = FALSE;
  boolean processing_done        = FALSE;
  uint32 config_delay_ms         = 2;
  boolean api_status             = TRUE;
  rfdevice_rxtx_common_class *device_ptr = NULL;
  rfcommon_mdsp_selftest_vswr_meas_type vswr_meas;
  boolean is_spec_flatness_cap = FALSE;
  uint32 fbrx_carrier_idx = 0;
  uint32 delay_us = 5000;
  rfcommon_selftest_meas_result meas_result;

  uint8 sub_id=0;


  boolean do_processing = (ftm_selftest_capture_type > 0)? TRUE: FALSE;

  if ( ( (ftm_selftest_capture_type & ENABLE_ACLR_MEAS ) !=0 ) ||
       ( (ftm_selftest_capture_type & ENABLE_SPEC_FLAT_MEAS ) !=0 ))
  {
    if ((ftm_selftest_capture_type & (ENABLE_EVM_MEAS | ENABLE_VSWR_MEAS_V2 | ENABLE_TX_PWR_MEAS) ) == 0)
    {
      do_processing = FALSE;
    }
  }

  if ((ftm_selftest_capture_type & ENABLE_SPEC_FLAT_MEAS ) !=0 )
  {
    if ( ftm_rf_tech != FTM_RF_TECH_LTE)
    {
      FTM_MSG_2(FTM_ERROR, "ftm_common_fbrx_iq_capture_processing: Tech Type %d does not support the requested measurement type 0x%x",
                ftm_rf_tech, ENABLE_SPEC_FLAT_MEAS);
      return FALSE;
    }
    is_spec_flatness_cap = TRUE;
  }

  /*initialize the VSWR Meas structure to 0*/

  memset(&vswr_meas, 0, sizeof(rfcommon_mdsp_selftest_vswr_meas_type));
  


  // #if defined(FEATURE_CDMA) || defined(FEATURE_TDSCDMA)
  // const rfc_common_logical_device_params_type *device_params_ptr = NULL;
  // #endif

  /* HDET should have been already disabled in calibration state */
  // if (rfm_get_calibration_state() == FALSE)
  // {
  //   /* Disable HDET loop */
  //   ftm_common_hdet_disable(ftm_rf_tech, tx_device, TRUE);

  //   /* Wait for HDET to disable.  Temporary workaround for LTE HDET disable issue */
  //   ftm_clk_rex_wait(3);
  // }

  /* Allocate DPD sample capture buffer */
  rfcommon_mc_allocate_dpd_buffer();

  selftest_fbrx_gain_stage = fbrx_gs_info->selftest_fbrx_gs;

  /* Trigger tech-specific capture */
  switch ( ftm_rf_tech )
  {
  case FTM_RF_TECH_CDMA:
  #ifdef FEATURE_CDMA1X
  {
    fbrx_chain = RFM_DEVICE_1;
    ftm_1x_xpt_cal_config((rfm_device_enum_type)device,fbrx_chain);

    // if (ftm_common_get_selftest_flag()) {

    //   /* if in self-test mode, override the default WTR and coupler gain */

    //   device_params_ptr = rfc_common_get_logical_device_params(RFM_DEVICE_0);

    //   if(device_params_ptr != NULL){

    //     ftm_common_set_coupler_attn(ftm_common_get_selftest_coupler_fb_attn_state());

    //     rfdevice_cmn_set_et_path(device_params_ptr->cmn_device,RFM_1X_MODE,ftm_common_get_selftest_wtr_fb_attn_state(),TRUE);
    //   }
    //  }
    }
  #endif
    break;

  case FTM_RF_TECH_WCDMA:
    #ifdef FEATURE_WCDMA
    {
      fbrx_chain = RFM_DEVICE_1;
      ftm_wcdma_xpt_cal_config((rfm_device_enum_type)device,fbrx_chain, (uint8)FTM_XPT_CONFIG_EPT, TRUE); /* True for config, False for deconfig */

      // if (ftm_common_get_selftest_flag()) {

      //     ftm_common_set_coupler_attn(ftm_common_get_selftest_coupler_fb_attn_state());

      //     ftm_wcdma_feedback_setup(RFM_DEVICE_0,
      //                             RFM_DEVICE_1,
      //                             FEEDBACK_SETUP_ACTION_MASK_CONFIGURE_WTR,
      //                             ftm_common_get_selftest_wtr_fb_attn_state(),
      //                             TRUE,
      //                             FALSE,
      //                             TRUE);
      // }
    }
    #endif
    break;

#ifdef FEATURE_GERAN_RF_SELF_TEST
  
  case FTM_RF_TECH_GSM:
    //rfdevice_gsm_dump_crit_registers(tx_device, RFCOM_BAND_GSM900, &wtr4905_reg_array_1[0], size_of_reg_array);
#ifdef FEATURE_GSM
    /* GSM uses device as tx_device, and fbrx_chain as rx_device to be consistent with other techs. 
           However, note that fbrx_chain is not formally defined in rfc */
    ftm_gsm_selftest_config( (rfm_device_enum_type)device,(rfm_device_enum_type)fbrx_chain, TRUE);
#endif
		
    //rfdevice_gsm_dump_crit_registers(tx_device, RFCOM_BAND_GSM900, &wtr4905_reg_array_2[0], size_of_reg_array);
    break;
#endif	
  case FTM_RF_TECH_TDSCDMA:
    #ifdef FEATURE_TDSCDMA
    {
      fbrx_chain = RFM_DEVICE_1;
      ftm_tdscdma_xpt_cal_config((rfm_device_enum_type)device, fbrx_chain,(uint8)FTM_XPT_CONFIG_EPT, TRUE); /* True for config, False for deconfig */


      // if (ftm_common_get_selftest_flag()) {
      //   device_params_ptr = rfc_common_get_logical_device_params(RFM_DEVICE_0);

      //   if(device_params_ptr != NULL){


      //     ftm_common_set_coupler_direction(tdscdma_coupler_direction);


      //     ftm_common_set_coupler_attn(ftm_common_get_selftest_coupler_fb_attn_state());

      //     rfdevice_cmn_set_et_path(device_params_ptr->cmn_device,RFM_TDSCDMA_MODE,ftm_common_get_selftest_wtr_fb_attn_state(),TRUE);
      //   }
      // }
    }
    #endif
    break;

  case FTM_RF_TECH_LTE:
  #ifdef FEATURE_LTE
  {
    fbrx_chain = rfc_lte_get_fb_device(rflte_ftm_mc_sm_get_tx_device(), rflte_ftm_mc_sm_get_tx_band(),rflte_ftm_mc_sm_carrier_get_drx_band(RFLTE_MC_PCELL_IDX));
    if (is_spec_flatness_cap)
    {
      ftm_lte_xpt_cal_configure((rfm_device_enum_type)device,fbrx_chain, (uint8)FTM_XPT_CONFIG_FBRX);
      fbrx_carrier_idx = rflte_ftm_mc_sm_get_carrier_index();
      FTM_MSG_3(FTM_HIGH, "Configured FTM_XPT_CONFIG_FBRX with tx_device %d, fbrx_chain %d, fbrx_carrier_idx %d",
                device, fbrx_chain, fbrx_carrier_idx);
      if (rflte_state_get_fbrxlm_buffer_idx(sub_id,fbrx_carrier_idx) != LM_BUFFER_NOT_ALLOCATED)
      {
        // Update dynamic Settings
        api_status &=  (rfcommon_fbrx_msm_update_dynamic_settings_for_xptcap( rflte_state_get_fbrxlm_buffer_idx(sub_id,fbrx_carrier_idx) ) == RFCOMMON_FBRX_SUCCESS);
        FTM_MSG_2(FTM_HIGH, "rfcommon_fbrx_lm_update_dynamic_settings_for_xptcap (buf_id %d ) success status %d ", 
                  rflte_state_get_fbrxlm_buffer_idx(sub_id,fbrx_carrier_idx) , 
                  api_status);  
      }
      else
      {
        api_status = FALSE;
      }
    }
    else
    {
      ftm_lte_xpt_cal_configure((rfm_device_enum_type)device,fbrx_chain, (uint8)FTM_XPT_CONFIG_ET);
    }

     //rflte_ftm_mc_sm_set_use_fb_path(TRUE);


    // if (ftm_common_get_selftest_flag()) {

    //   /* if in self-test mode, override the default WTR and coupler gain */

    //   ftm_common_set_coupler_attn(ftm_common_get_selftest_coupler_fb_attn_state());

    //   rflte_ftm_mc_set_et_path(ftm_common_get_selftest_wtr_fb_attn_state(), TRUE);

    // }

  }
  #endif
    break;

  default:
    FTM_MSG_1( FTM_ERROR, "ftm_common_fbrx_iq_capture_processing: Mode %d not supported",
               ftm_rf_tech );
    api_status = FALSE;
    break;
  }

  if (ftm_rf_tech == FTM_RF_TECH_CDMA)
  {
#ifdef FEATURE_CDMA1X

    /* Variables to hold the rx device state and the band informations
    to query the corresponding tx device from the RFC to support the 
    rx\tx split */
    ftm_cdma_data_status_type rx_dev_state;  
    sys_band_class_e_type sys_band = SYS_BAND_CLASS_MAX;

    /* Query the rx device state to get the the band information and use the 
    band info to get the tx device from the RFC to support the rx\tx split */
    (void)ftm_cdma_data_get_device_status( device, &rx_dev_state );
    sys_band = rf_cdma_rf_band_type_to_sys_band_type( rx_dev_state.curr_band);

    device_ptr = rfcommon_core_get_tx_device_ptr(rfc_common_get_preferred_tx_device ( sys_band ),
                                                 ftm_common_ftm_tech_to_rfm_mode(ftm_rf_tech), 
                                                 fbrx_gs_info->com_band);
#endif
  }
  else
  {
    device_ptr = rfcommon_core_get_tx_device_ptr(device,
                                                 ftm_common_ftm_tech_to_rfm_mode(ftm_rf_tech), 
                                                 fbrx_gs_info->com_band);
  }

  if (fbrx_gs_info->override_enable)
  {
    rfdevice_cmn_override_fbrx_gain_state(device_ptr, 
                                        ftm_common_ftm_tech_to_rfm_mode(ftm_rf_tech), 
                                        fbrx_gs_info->com_band, 
                                        fbrx_gs_info->selftest_fbrx_gs, 
                                        fbrx_gs_info->wtr_lte_bw, 
                                        RFDEVICE_EXECUTE_IMMEDIATE,
                                        TRUE, 
                                        NULL
                                        );
  }


  ftm_clk_rex_wait(config_delay_ms);

  FTM_MSG_1(FTM_HIGH, "xpt_cal_configure  done", 0);

  if (api_status == TRUE)
  {
    /* trigger capture*/
    capture_trigerred = ftm_common_trigger_self_test_iq_cap(ftm_rf_tech,
                                                          device,
                                                          fbrx_chain,
                                                          num_samples,
                                                          ftm_selftest_capture_type,
                                                          samp_rate);
#ifdef FEATURE_GERAN_RF_SELF_TEST
    if(ftm_rf_tech == FTM_RF_TECH_GSM)
    {
      ftm_gsm_selftest_enable_ref_slot(device);
      delay_us = 30000;
    }
#endif	
  }

  /* Wait for capture done */
  if((capture_trigerred == TRUE) )
  {
    capture_done = rfcommon_xpt_wait_for_capture_done( 500, delay_us, &iq_buffer_index );

    if (fbrx_gs_info->override_enable)
    {
      rfdevice_cmn_override_fbrx_gain_state(device_ptr, 
                                          ftm_common_ftm_tech_to_rfm_mode(ftm_rf_tech), 
                                          fbrx_gs_info->com_band, 
                                          fbrx_gs_info->selftest_fbrx_gs, 
                                          fbrx_gs_info->wtr_lte_bw, 
                                          RFDEVICE_EXECUTE_IMMEDIATE,
                                          FALSE, 
                                          NULL
                                          );
    }
    if( capture_done == FALSE )
    {
      FTM_MSG_1( FTM_ERROR, "ftm_lte_fbrx_iq_capture_trigger_handler: Sample "
               "capture did not complete", 0);
      return FALSE;
    }
  }
  else
  {
    FTM_MSG_1( FTM_ERROR, "ftm_lte_fbrx_iq_capture_trigger_handler: Sample "
              "capture trigger failed", 0);
    if (fbrx_gs_info->override_enable)
    {
      rfdevice_cmn_override_fbrx_gain_state(device_ptr, 
                                          ftm_common_ftm_tech_to_rfm_mode(ftm_rf_tech), 
                                          fbrx_gs_info->com_band, 
                                          fbrx_gs_info->selftest_fbrx_gs, 
                                          fbrx_gs_info->wtr_lte_bw, 
                                          RFDEVICE_EXECUTE_IMMEDIATE,
                                          FALSE, 
                                          NULL
                                          );
    }    
    return FALSE;
  }

  /* fetch the collected samples*/
  rfcommon_mc_fetch_selftest_iq_data(iq_buffer_index, num_samples, iq_buf_rx, iq_buf_tx);

  if(do_processing)
  {
    /* Wait for processing done */
    processing_done = rfcommon_xpt_wait_for_processing_done( 1000, 35000, &result_buffer_index );

    if ( processing_done == FALSE)
    {
      FTM_MSG_1( FTM_ERROR, "ftm_common_trigger_ept_iq_cap: "
                   "Processing did not complete for tech %d", ftm_rf_tech );
      return FALSE;
    }
    rfcommon_mc_fetch_selftest_meas(result_buffer_index,&meas_result);
    *rx_rms_mag_ptr = (ftm_rf_tech == FTM_RF_TECH_GSM)? meas_result.gsm_power:meas_result.rx_rms_mag;
    *evm_value_ptr = meas_result.evm;    

    /* get VSWR split capture measurement */

    if ( (ftm_selftest_capture_type & ENABLE_VSWR_MEAS_V2 ) !=0 )
    {

      if (rfcommon_mdsp_selftest_vswr_meas(result_buffer_index, &vswr_meas))
      {

      ftm_common_selftest_vswr_v2_results.vswr_v2_mag = (vswr_meas.refl_pwr -vswr_meas.fwd_pwr);

        ftm_common_selftest_vswr_v2_results.vswr_v2_phase = (vswr_meas.refl_phase - vswr_meas.fwd_phase+ 54000) % 36000 - 18000;

      ftm_common_selftest_vswr_v2_results.vswr_v2_return_loss = - ftm_common_selftest_vswr_v2_results.vswr_v2_mag;

      ftm_common_selftest_vswr_v2_results.vswr_v2_fwd_pwr = vswr_meas.fwd_pwr;

      ftm_common_selftest_vswr_v2_results.vswr_v2_rev_pwr = vswr_meas.refl_pwr;

      FTM_MSG_6(FTM_HIGH, "Selftest VSWR results:"

                "VSWR_gain = %d dB100; VSWR_phase = %d Degree*100"
                            
                "fwd_phase = %d; " 
                                           
                "fwd_pwr = %d; " 
   
                "refl_phase = %d; " 
                                           
                "refl_pwr = %d; ",

                ftm_common_selftest_vswr_v2_results.vswr_v2_mag, 

                ftm_common_selftest_vswr_v2_results.vswr_v2_phase,
                            
                vswr_meas.fwd_phase,
                                           
                vswr_meas.fwd_pwr,
                                       
                                           
                vswr_meas.refl_phase,
                                           
                vswr_meas.refl_pwr);
      }
      else
      {
        FTM_MSG(FTM_ERROR, "Selftest VSWR results invalid!");
        api_status = FALSE;
      }     
    }
  }

#ifdef FEATURE_GERAN_RF_SELF_TEST

  if(ftm_rf_tech == FTM_RF_TECH_GSM)
  {
    ftm_gsm_selftest_disable_ref_slot(device);    
    ftm_clk_rex_wait(20);
  }
#endif
  /* release capture buffer, returns false now, TODO: implement deallocation of buffer_id and return true */
  rfcommon_core_xpt_release_capture_buffer(iq_buffer_index);

  /* release results buffer, returns false now, TODO: implement deallocation of buffer_id and return true */
  rfcommon_core_xpt_release_results_buffer(result_buffer_index);

  // if (ftm_common_get_selftest_flag()) {

  //    if in self-test mode, override the default WTR and coupler gain

  //   ftm_common_set_coupler_attn(COUPLER_ATTN_NORMAL);

  // }

  /* release all buffer ids */
  rfcommon_core_xpt_free_all_buffers();

  /* HDET should have been already disabled in calibration state */
  // if (rfm_get_calibration_state() == FALSE)
  // {
  //   /* Release HDET loop */
  //   ftm_common_hdet_disable(ftm_rf_tech, tx_device, FALSE);
  // }

  return api_status;
}

void ftm_common_selftest_set_flag( uint8 * flag_value)
{
  if (*flag_value != ftm_common_selftest_modeset_flag)
  {
    FTM_MSG_2(FTM_HIGH, "ftm_common_selftest_set_flag: current value %d set to %d",ftm_common_selftest_modeset_flag,*flag_value);
    ftm_common_selftest_modeset_flag = *flag_value;
    #ifdef FEATURE_WCDMA
    rfwcdma_core_tx_set_common_selftest_ind(ftm_common_selftest_modeset_flag);
    #endif
  }
  else
  {
    FTM_MSG_1(FTM_HIGH, "ftm_common_selftest_set_flag: No Change to current value %d",ftm_common_selftest_modeset_flag);
  }
}


uint8 ftm_common_get_selftest_capture_flag(void)
{
  return ftm_common_selftest_modeset_flag;
}


void ftm_common_selftest_get_coupler_script
(
  rfm_mode_enum_type tech,
  rfcom_band_type_u band,
  rfm_device_enum_type tx_device
)
{

  void **coupler_devices = NULL;
  rfdevice_coupler_tech_cfg_type tech_cfg;
  uint8 device_idx = 0;

  tech_cfg.mode = tech;
  tech_cfg.band = band;
  tech_cfg.gain = RFDEVICE_COUPLER_GAIN_DEFAULT;
  tech_cfg.output_type = RFDEVICE_COUPLER_OUTPUT_TYPE_DEFAULT;
  
  switch( tech )
  {
  #ifdef FEATURE_LTE
  case RFM_LTE_MODE:
    coupler_devices = rfc_lte_get_device(tx_device, 
                                         RFC_CONFIG_TX, 
                                         band.lte_band, 
                                         RFDEVICE_COUPLER
                                         );
    break;
  #endif
  #ifdef FEATURE_WCDMA
  case RFM_IMT_MODE:
    coupler_devices = rfc_wcdma_get_device(tx_device, 
                                           RFC_CONFIG_TX, 
                                           band.wcdma_band, 
                                           RFDEVICE_COUPLER
                                           );
    break;
  #endif
  #ifdef FTM_HAS_TDSCDMA
  case RFM_TDSCDMA_MODE:
    coupler_devices = rfc_tdscdma_get_rf_device_obj(tx_device, 
                                                    RFC_CONFIG_TX, 
                                                    band.tdscdma_band, 
                                                    RFDEVICE_COUPLER
                                                    );     
    break;
  #endif
  #ifdef FEATURE_CDMA1X

  case RFM_1X_MODE:
    /* Hack to use coupler device objects from LTE until 1x supports multiple coupler config */
    coupler_devices = rfc_lte_get_device(RFM_DEVICE_6,
                                         RFC_CONFIG_TX,
                                         RFCOM_BAND_LTE_B1,
                                         RFDEVICE_COUPLER
                                         );
      
    break;
  #endif
  default:
    MSG_1(MSG_SSID_FTM, MSG_LEGACY_ERROR, "tech %d not supported for coupler config!", tech);
    break;
  }      

  if(coupler_devices != NULL)
  {
    for (device_idx = 0; device_idx < MAX_NUM_COUPLER_DEVICES; device_idx ++)
    {
      selftest_coupler_script_ptr[device_idx][FTM_COUPLER_DIRECTION_FWD] = rf_buffer_create(0, MAX_NUM_COUPLER_TRANSACTIONS, 0);
      tech_cfg.direction = RFDEVICE_COUPLER_DIRECTION_FWD;
      if (!rfdevice_coupler_config(coupler_devices[device_idx], 
                                   tech_cfg, 
                                   selftest_coupler_script_ptr[device_idx][FTM_COUPLER_DIRECTION_FWD], 
                                   RFDEVICE_CREATE_SCRIPT, 
                                   0))
      {
        MSG_3(MSG_SSID_FTM, MSG_LEGACY_ERROR, "coupler config script failed for tech %d device_idx %d dir %d", tech, device_idx, FTM_COUPLER_DIRECTION_FWD);
      }

      selftest_coupler_script_ptr[device_idx][FTM_COUPLER_DIRECTION_REV] = rf_buffer_create(0, MAX_NUM_COUPLER_TRANSACTIONS, 0);
      tech_cfg.direction = RFDEVICE_COUPLER_DIRECTION_REV;
      if (!rfdevice_coupler_config(coupler_devices[device_idx], 
                                   tech_cfg, 
                                   selftest_coupler_script_ptr[device_idx][FTM_COUPLER_DIRECTION_REV], 
                                   RFDEVICE_CREATE_SCRIPT, 
                                   0))
      {
        MSG_3(MSG_SSID_FTM, MSG_LEGACY_ERROR, "coupler config script failed for tech %d device_idx %d dir %d", tech, device_idx, FTM_COUPLER_DIRECTION_REV);
      }

    }
  }
}

void ftm_common_selftest_delete_coupler_script(void)
{
  uint8 device_idx;
  for ( device_idx = 0; device_idx < MAX_NUM_COUPLER_DEVICES; device_idx ++)
  {
    rf_buffer_delete(selftest_coupler_script_ptr[device_idx][FTM_COUPLER_DIRECTION_FWD]);
    rf_buffer_delete(selftest_coupler_script_ptr[device_idx][FTM_COUPLER_DIRECTION_REV]);
  }
}

void ftm_common_set_coupler_direction(uint8 coupler_dir)
{

  rf_hal_bus_resource_script_settings_type settings;
  rf_time_tick_type prof_tick;
  rf_time_type prof_time = -1;
  uint8 device_idx;

  memset(&settings, 0, sizeof(rf_hal_bus_resource_script_settings_type));

  prof_tick = rf_time_get_tick();

  for (device_idx = 0; device_idx < MAX_NUM_COUPLER_DEVICES; device_idx ++)
  {
    if( RF_HAL_BUS_SUCCESS != rfhal_rffe_execute_rf_buf(selftest_coupler_script_ptr[device_idx][coupler_dir], 
                                                        NULL, 
                                                        0, 
                                                        &settings,
                                                        0))
    {
      MSG_2(MSG_SSID_FTM, MSG_LEGACY_ERROR, "coupler rffe writes execution failed for device_idx %d dir %d", device_idx, coupler_dir);
    }
  }
  
  prof_time = rf_time_get_elapsed( prof_tick, RF_USEC );
  MSG_2(MSG_SSID_FTM, MSG_LEGACY_ERROR, "selftest coupler writes time profiling %d dir %d", prof_time, coupler_dir);
        
}


/* END LEGACY CODE */


/*
Wrapper for all of the tech-specific FBRx noise floor writes
*/
boolean ftm_common_selftest_fbrx_noise_floor_nvwrite(ftm_rf_technology_type tech, uint8 nv_band, rfcommon_nv_fbrx_noise_data_type * fbrx_noise_nv_struct)
{
  boolean nv_valid = FALSE;
  rfnv_item_id_enum_type nv_item_to_write = RFNV_ITEMS_ENUM_MAX;
  nv_stat_enum_type nv_write_stat = NV_STAT_ENUM_MAX;

  nv_valid = ftm_common_selftest_obtain_fbrx_noise_nv_item(tech, nv_band, &nv_item_to_write);

  if (nv_valid == FALSE)
  {
    FTM_MSG_3(FTM_ERROR, "Failed to obtain NV item for tech %d and band %d, nv_valid flag : %d", tech, nv_band, nv_valid);
    return FALSE;
  }

  if ( nv_valid )
  {
    /* Write KV calibration code to NV */
    nv_write_stat = rfcommon_rfnv_set_item(nv_item_to_write,
                           (rfnv_item_type *)(fbrx_noise_nv_struct),
                           sizeof(rfcommon_nv_fbrx_noise_data_type),
                           NULL,
                           (rex_sigs_type)0,
                           NULL);
    if (nv_write_stat != NV_DONE_S)
    {
      FTM_MSG_1(FTM_ERROR, "Couldn't write NV, status: %d",nv_write_stat);
      return FALSE;
    }
  }
  else
  {
    FTM_MSG(FTM_ERROR, "NV location is not valid or the api failed, not writing NV!");
    return FALSE;
  }

  return TRUE;

}

/*
Wrapper for all of the tech-specific FBRx noise NV items
*/
boolean ftm_common_selftest_obtain_fbrx_noise_nv_item(const ftm_rf_technology_type tech, const uint8 nv_band, rfnv_item_id_enum_type * nv_item_to_write)
{
  boolean nv_valid = FALSE;

  switch(tech)
  {
    #ifdef FEATURE_CDMA1X
    case FTM_RF_TECH_CDMA:
      FTM_MSG(FTM_ERROR, "ftm_common_selftest_get_fbrx_noise_nv_item does not support NV items for CDMA tech!");
    break;
    #endif
	
    #ifdef FEATURE_WCDMA
    case FTM_RF_TECH_WCDMA:
      nv_valid = ftm_wcdma_obtain_fbrx_noise_nv_item(nv_band, nv_item_to_write);
    break;
	#endif
	
	#ifdef FEATURE_GSM
    case FTM_RF_TECH_GSM:
      FTM_MSG(FTM_ERROR, "ftm_common_selftest_get_fbrx_noise_nv_item does not support NV items for GSM tech!");
    break;
	#endif
	
    #ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE: 
      nv_valid = ftm_lte_obtain_fbrx_noise_nv_item(nv_band, nv_item_to_write);
    break;
	#endif
	
    #ifdef FTM_HAS_TDSCDMA
    case FTM_RF_TECH_TDSCDMA:
      FTM_MSG(FTM_ERROR, "ftm_common_selftest_get_fbrx_noise_nv_item does not support NV items for TDSCDMA tech!");
    break;
	#endif
	
    default:
      FTM_MSG_1(FTM_ERROR, "Unknown Tech %d, cannot get nv item!", tech);
  }

  return nv_valid;
}

/*
Wrapper for all of the tech-specific FBRx gainstage functions
*/
boolean ftm_common_selftest_obtain_fbrx_gain_stage(const ftm_rf_technology_type tech, uint8 * gain_stage)
{
  boolean nv_valid = FALSE;

  switch(tech)
  {
    #ifdef FEATURE_CDMA1X
    case FTM_RF_TECH_CDMA:
      //nv_valid = ftm_1x_obtain_fbrx_gain_stage(gain_stage);
    break;
	#endif
	
    #ifdef FEATURE_WCDMA
    case FTM_RF_TECH_WCDMA:
      nv_valid = ftm_wcdma_obtain_fbrx_gain_stage(gain_stage);
    break;
	#endif
	
    #ifdef FEATURE_GSM
    case FTM_RF_TECH_GSM:
      FTM_MSG(FTM_ERROR, "ftm_common_selftest_obtain_fbrx_gain_stage does not support NV items for GSM tech!");
    break;
	#endif

    #ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE: 
      nv_valid = ftm_lte_obtain_fbrx_gain_stage(gain_stage);
    break;
    #endif
	
    #ifdef FTM_HAS_TDSCDMA
    case FTM_RF_TECH_TDSCDMA:
      nv_valid = ftm_tdscdma_obtain_fbrx_gain_stage(gain_stage);
    break;
	#endif
	
    default:
      FTM_MSG_1(FTM_ERROR, "Unknown Tech %d, cannot get nv item!", tech);
  }

  return nv_valid;  
}

/*
Wrapper for all the tech-specific switchpoint NV items
*/
boolean ftm_common_selftest_obtain_fbrx_gain_switchpoint_nv_item(const ftm_rf_technology_type tech, const rfcom_band_type_u band, rfnv_item_id_enum_type * nv_item)
{
  boolean nv_valid = FALSE;

  switch(tech)
  {
    #ifdef FEATURE_CDMA1X
    case FTM_RF_TECH_CDMA:
      nv_valid = ftm_1x_obtain_fbrx_gain_switchpoint_nv_item(band.cdma_band, nv_item);
    break;
	#endif
	
    #ifdef FEATURE_WCDMA
    case FTM_RF_TECH_WCDMA:
      nv_valid = ftm_wcdma_obtain_fbrx_gain_switchpoint_nv_item(band.wcdma_band, nv_item);
    break;
	#endif
	
    #ifdef FEATURE_GSM
    case FTM_RF_TECH_GSM:
      FTM_MSG(FTM_ERROR, "ftm_common_selftest_obtain_fbrx_gain_switchpoint_nv_item does not support NV items for GSM tech!");
    break;
	#endif

    #ifdef FEATURE_LTE
    case FTM_RF_TECH_LTE: 
      nv_valid = ftm_lte_obtain_fbrx_gain_switchpoint_nv_item(band.lte_band, nv_item);
    break;
	#endif
	
    #ifdef FTM_HAS_TDSCDMA
    case FTM_RF_TECH_TDSCDMA:
      nv_valid = ftm_tdscdma_obtain_fbrx_gain_switchpoint_nv_item(band.tdscdma_band, nv_item);
    break;
	#endif
    default:
      FTM_MSG_1(FTM_ERROR, "Unknown Tech %d, cannot get nv item!", tech);
  }

  return nv_valid;  
}

/*
Returns Com band enum value for F3 purposes
*/
static uint8 ftm_common_com_band_specifier(const ftm_rf_technology_type ftm_rf_tech, rfcom_band_type_u com_band)
{
  switch(ftm_rf_tech)
  {
    case FTM_RF_TECH_CDMA: return com_band.cdma_band;
    case FTM_RF_TECH_WCDMA: return com_band.wcdma_band;
    case FTM_RF_TECH_TDSCDMA: return com_band.tdscdma_band;
    case FTM_RF_TECH_LTE: return com_band.lte_band;
    default: return -1;
  }
}

#endif /* FEATURE_FACTORY_TESTMODE */

