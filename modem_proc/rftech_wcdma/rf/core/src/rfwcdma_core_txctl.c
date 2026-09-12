/*!
  @file
  rfwcdma_core_txctl.c

  @brief
  Provides TX control. Example PA ctl is handled here.

  @details


*/

/*===========================================================================

Copyright (c) 2008 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.

                           EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/rftech_wcdma.mpss/3.11/rf/core/src/rfwcdma_core_txctl.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/04/20   nik     KW error fix in rfwcdma_core_txctl_config_tune_device
11/15/19   nik     QET5102 delay changes
02/22/18   nik     KW error fix
05/17/16   ag      Fix QSEQ Max Power Dip
01/28/16   sd      Feature differentiation for WTRs TA2.3
12/17/15   ag      Increase script size for Autopin
12/11/15   ak      Changes to do the Tx config correctly for SC<->DC transition
11/26/15   ndb     Pass the frequency to tuner API's
11/30/15   ag      Add functionality to take wtr script size for autopin dynamically
11/18/15   asn     Add support to check if RACH or DCH
11/19/15   ak      Fix Memory Leak
11/18/15   ag      Featurize Autopin in TA, fetch wtr script size from dm
11/10/15   ljz     Skip LDO On\off during selftest
11/05/15   ag      Add build scripts for Autopin events
09/24/15   asn     Push device transactions corresponding to RGI=0 in Tx OFF device script
09/23/15   ska	   Set default coupler gain and output type
07/28/15   yb      Add few debug message
08/17/15   vs      Increase RFFE script for Tx to 20 to include Therm transactions 
07/28/15   yb      VSWR support
07/20/15   nv      [TA]Use different setpoints for SC/DC case for TA 
07/20/15   nv      Update pda ctrl offset for different carrier spacing in DC case
07/14/15   rc      Add power logging support
06/18/15   vbh     Call the util routine to map carrier mask to carr_id for ant tuner 
06/04/15   rmb     Support for querying correct TQ buffer index maintained by RFWCDMA in CCS
                   for IRAT and standalone scenarios.  
06/02/15   gh/zhh  Add support for building therm_enable script
05/22/15   kr      Fix IQ and ENV. gain scale setting for DC-HSUPA for EPT/ET
05/12/15   vbh     Antenna tuner interface updates
07/04/15   ak      Migrating to new WCDMA state machine
07/04/15   ak      Rx/Tx Split Changes
04/28/15   rmb     Remove feature flag RF_HAS_MEAS_INTF_TH_2_0 but retain code
                   added under this flag. 
04/07/15   rmb     Remove reference to unused structure
04/06/15   rmb     Merge back meas interface changes onto Mainline with feature flag 
03/31/15   jr      Add ASM device to TxAGC configuration
03/19/15   rmb     Add Tx specific GRFC script based API.
03/02/15   rmb    Add channel type as UL channel to Tx params for WTR Tx Tune.
02/12/15   dw      Fix 2x xpt setpoint
01/22/15   kai     Fixed set frequency adjustment function
01/22/15   ak      Changes to program the PDA values correctly for Tabasco
12/09/14   kai     W DAC clock reduction for power optimization
11/28/14   kr      Support no PAPM device
11/04/14   ac      ftm flag to denote the txagc state 
10/21/14   zhh     Fix F3 Logs
09/19/14   dw      Move assigning Tx tune script buffer out of featurizaion
09/15/14   joz     replace FEATURE_DIMEPM_MODEM by FEATURE_JOLOKIA_MODEM
09/15/14   vbh     Added device as a parameter to support Tx sharing
09/15/14   dw      WXE based Tx tune support for Single Tx
09/02/14   dw      Disable TxAGC during Tx bw change
08/15/14   zhh    populate the new MPR to DM in rfwcdma_core_get_tx_mpr_param()
02/26/14   zhh    Update XPT Delay structure to support DC-UPA
08/11/14   dw      Fix script buffer leak for Tx tune event
08/11/14   ak      Changes to update the PDA offsets correctly for DimePM
08/06/14   dw      Disable TxAGC in Tx init when Tx BW change
08/01/14   dw      Initial support for single Tx DSDA
07/10/14   aa      Update maximum TX RFFE transactions
06/18/14   aa      DCHSUPA: DC config always in SC mode
06/03/14   aro     Destroy RF Buffer to avoid MemLeak
05/15/14   aro     Device call to populate Power Meter trigger script
05/15/14   rmb     Added API for config device scripts in Tx.
05/01/14   dw      Added update ET delay and set freq adjust in MDSP async set data command
04/20/14   bn      WCDMA 8 PA state support and related cleanup
04/09/14   dw      DCHSUPA feature cleanup
03/19/13   dw      Add missing call to rfwcdma_mdsp_async_config_tx_mc()
03/11/13   dw      Add Bolt DCHSUPA support - fix hardcoding of RF_WCDMA_TX_MC_PDA_CTL_OFFSET
03/24/14   vbh     Clean up rfc_common_get_cmn_device_object
03/24/14   rmb     Move the btf delay paramter udpate from Tx config to Rx config band.
03/13/14   kai     [1]Query APT default IQ gain from TxLM
                   [2]Populate XPT mode per PA state from linearizer
03/11/14   dw      Add Bolt DCHSUPA support - fix hardcoding of RF_WCDMA_TX_MC_PDA_CTL_OFFSET
02/21/14   rmb     Add global NV pointers for each of band specific static NVs.
02/05/14   dw      Increase generic script size, clear buffer after creation
01/10/14   kai     Added XPT mode default settings
01/02/14   dw      Added warm time, PLL settling time and RF transaction time to DM interface
11/25/13   kai     Added generic API to query WCDMA Tx device pointers
11/05/13   bn      [1] New Timing offset for BOLT
                   [2] Swaping the common TxAGC API call order.
                       * rfwcdma_mdsp_async_config_tx_band
                       * rfwcdma_core_txctl_txagc_config
                   PA SP needs to be populated before the generic seq creation.
                   New restrictions from timeline optimization in CMN FED TxAGC
10/16/13   dw      Move local mdsp_cfg data structures to heap
10/08/13   kai     Added common TxAGC
09/16/13   dw      Added support for FTM Cal mode and DRx scripts
09/10/13   dw      [1]Fix off-target error  
                   [2]Spell correction.
									 [3]Add some Macro definition to remove hardcoding 
09/09/13   dw      Core changes for new Bolt MDSP module - new API to config Tx band/chan
08/14/13  ska     Correcting the Tx init sequence to fix a Tx performance issue
08/01/13   kr       replace memcpy with memscpy
07/24/13   kai     Added XPT path delay offset vs temp vs freq
07/22/13   rmb     Fix CW
07/19/13   rmb     Initialize XPT Params in rfwcdma_core_txctl_init_bsp_agc_ctl(); 
06/25/12   ac      tx freezeaccum api change for both car
06/25/13   aro     Moved PA compensation computation after linearizer load
06/23/13   kai     Added WCDMA 16channels xpt delay support
05/10/13   bn      Support EPT_ET_CFG configuration
03/29/13   bn      MDSP set QPOET register address in EPT/ET mode
12/11/12   dw      Add support for DTx and DRx/LPM.
12/11/12   ac      pa_ range event builder support, pa q current as a txagc task
10/12/12   ac      removing the the timing from the tx_event function
09/28/12   ac      device interface to pass band to rfc_wcdma_get_device
09/28/12   swb     Add support for RFC virtual PA mapping
09/18/12   ac      rfc to take device as parameter
05/08/12   tks     Add support for CPC DTx 
04/17/12   gh      Add support for TX_GTR_THRESH
04/16/12   kai     Added support for Tx_AGC_ADJ_PDM_DELAY setting in apt_setting_txlm 
		   Removed old rfwcdma_msm_set_tx_adj_pdm_delay(int16 delay) function
03/01/01   dw      [1] Add support for HSPA APT
                   [2] Rework some call flows for Tx AGC/linearizer control
03/17/12   jfc     Removed clkrgm* header files
02/24/12   tks     Modified CPC DTX & DRX variables for WTR1605 support
02/03/12   kai     Deleting unused PRACH PA threshold functions
01/31/12   swb     Fix bug for programming TX and PA timing delays
01/12/12   dw      Mainline FEATURE_RF_CPC_DRX 
12/08/11   dw      populate TxAGC gain SSBI timing to FW
11/18/11   ac      Enable PA Comp Up/Down to vary with frequency
09/19/11   dw      clean up rfwcdma_core_txctl_set_tx_modulator()
07/12/11   vb      rfc cleanup and rfc interface changes to suit the new auto-gen 
                   of rf card files
04/20/11   jhe     Minor clean up
03/02/11   dw      Nikel updates
02/15/11   ac/dw   merge cpc drx support 
02/21/11   ka/dw   Use new device interface for CPC DTx  
01/28/11   dw      Initial TxLM support for WCDMA
10/26/10   ka      CPC DTx support 
10/19/10   vb      All device functions are now accessed thru device interface functions
10/11/10   ad      Added preliminary support for WCDMA APT Phase 1   
07/19/10   ka      Removed warnings 
04/22/10   ad      Disable MODEM_PA_CTL when Tx is turned off     
03/16/10   kguo    Merged from  scmm_merge_final branch

02/10/10   ka      Removed clkregime calls. Will be done in HAL if necessary.
01/21/10   bn      Lint Fixes
01/21/09   bn      Update tx rotator angle values to MDSP
01/10/10   kguo    Merged mdm9k dev branch to main\latest
12/15/09   bn      Adding support for HDET ADC page setting
10/23/09   kguo    Merged changes for 9k bringup
09/17/09   ckl     Moved some functions from mc file.
05/06/09   bn      Removing TX_GENERAL_CTL settings in txdac_ctl function
03/19/09   ckl     Arrange order of header files
03/17/09   ckl     Added RF_HAS_PRACH_PA_RANGE compile option
03/09/09   ckl     Code cleanup
11/06/08   av      Initial version.

============================================================================*/
#include "npa.h"
#include "rfcommon_atuner_intf.h"
#include "rfa_variation.h"
#include "rfumts.h"
#include "rfnv_wcdma.h"
#include "rfwcdma_mdsp.h"
#include "rfdevice_wcdma_intf.h"
#include "rfwcdma_core_txctl.h"
#include "rfwcdma_msm.h"
#include "rfwcdma_mdsp.h"
#include "rfwcdma_core.h"
#include "ftm.h"
#include "rfdevice_wcdma_intf.h" 
#include "rfdevice_hdet_wcdma_intf.h" 
#include "rfcommon_msg.h"
#include "rfwcdma_core_xpt.h"
#include "rfdevice_cmn_type_defs.h"
#include "rfcommon_autopin.h"


/* memscpy */
#include "stringl.h"
#include "rfwcdma_mdsp_types.h"
#include "rfcommon_nv_mm.h"
#include "rfcommon_core_xpt.h"
#include "rfwcdma_mdsp_async.h"
#include "rf_hal_buffer.h"
#include "rfdevice_wcdma_pa_intf.h"
#include "rfwcdma_core_util.h"
#include "rfcommon_math.h"
#include "modem_mem.h"
#include "rfdevice_wcdma_asm_intf.h"
#include "rfdevice_wcdma_pa_intf.h"
#include "rfdevice_wcdma_coupler_intf.h"
#include "rfdevice_hdet_wcdma_intf.h"
#include "rfc_card_wcdma.h"
#include "rfwcdma_core_txplim.h"
#include "rfwcdma_core_antenna_tuner.h"
#include "rfwcdma_mdsp_sync.h"
#include "rfwcdma_data.h"
#ifdef FEATURE_RF_POWER_REPORTING
#include "rf_power_reporting.h"
#endif

/*----------------------------------------------------------------------------*/
/*! 
  @brief
  Applies IQ setpoint/Envelope Scale back off for 2x Tx waveform (10MHz DC)
 

 
*/
#define SETPOINT_BACKOFF_2X(s) ((double)s*(4/1.67))

#define SETPOINT_BACKOFF_2X_EPT_ET(s) ((double)s*(2/1.67))

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines generic SSBI script size(max) for a Tx event 
*/
#define RF_WCDMA_TX_GEN_SSBI_SCRIPT_SIZE                                       10

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines generic RFFE script size(max) for a Tx event
 
*/
#define RF_WCDMA_TX_GEN_RFFE_SCRIPT_SIZE                                       30
/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines generic GRFC script size(max) for a Tx event
*/
#define RF_WCDMA_TX_GEN_GRFC_SCRIPT_SIZE                                       10


/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines generic RFFE script size(max) for Tx tune event
 
*/
#define RF_WCDMA_TX_TUNE_RFFE_SCRIPT_SIZE                                     300

/*----------------------------------------------------------------------------*/

/*! 
    @brief
    This number defines generic RFFE script size(max) for a Autopin event
 
*/
#define RF_WCDMA_AUTOPIN_RFFE_SCRIPT_SIZE                                     75
/*----------------------------------------------------------------------------*/

/*! 
    @brief
    This number defines base value of Tx Phase Delta Accumulator
*/
#define RF_WCDMA_TX_MC_PDA_CTL_BASE                                            0

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines base value of Tx Phase Delta Accumulator in SC mode
*/
#define RF_WCDMA_TX_MC_PDA_CTL_OFFSET_SC                                       0

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines railed PA switchpoint: PA state above of which is invalid
*/
#define RF_WCDMA_PA_SWITCHPOINT_INVALID                                    32767

/*----------------------------------------------------------------------------*/
/*! 
    @brief
    This number defines maximum PA state swtich hysterisis timer in BPG
*/
#define RF_WCDMA_PA_HYSTERISIS_MAX                                           100

/* Set timing params */
rfcommon_core_txlin_agc_timing_params rfwcdma_txagc_timing_params = 
{
-80,  /* wtr_early_trig_offset */
-50,  /* wtr_late_trig_offset */
-75, /* pa_trig_offset */
-140, /* papm_early_trig_offset */
-20   /* papm_late_trig_offset */
};

//extern void rfwcdma_mdsp_set_pa_timer_hysterisis( int16 timer_hys );

/* NPA client for Q6 clock voting */
boolean rfwcdma_core_npa_handle_initialised;       ///<Flag indicating if q6clk NPA handle is initialized.
npa_client_handle rfwcdma_core_npa_handle_q6clk;   ///<NPA client handle

/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function initialize Tx band sensitive AGC parameters.

  @param band WCDMA operational band

*/
void rfwcdma_core_txctl_init_bsp_agc_ctl(rfm_device_enum_type device,rfcom_wcdma_band_type band)
{

  /* Null pointer check */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_txctl_init_bsp_agc_ctl: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
     RF_MSG_1(RF_ERROR,
               "rfwcdma_core_txctl_init_bsp_agc_ctl: Not a valid band %d",
               band);
     return;
  }

  #if 0
  rfwcdma_mdsp_init_pa_map(rfnv_wcdma_tbl_ptr[band]->pa_static.pa_range_map, band);
  
  /* Updating timer  */
  rfwcdma_mdsp_set_pa_timer_hysterisis(rfnv_wcdma_tbl_ptr[band]->timer_hysterisis);

  rfwcdma_mdsp_set_pa_range_thresh(band);
  
  rfwcdma_mdsp_set_tx_rotator_angle(band);
  #endif
  
  rfwcdma_mdsp_set_tx_agc_update_ssbi_time(rfnv_wcdma_tbl_ptr[band]->dch_agc_update_tx_agc_time);

  rfwcdma_mdsp_set_tx_agc_time(rfnv_wcdma_tbl_ptr[band]->agc_update_tx_agc_time);

  rfwcdma_mdsp_set_pa_current_time(0);

  rfwcdma_mdsp_set_pa_range_update_time(rfnv_wcdma_tbl_ptr[band]->pa_gain_up_time,
                                rfnv_wcdma_tbl_ptr[band]->pa_gain_down_time );


  rfwcdma_mdsp_set_band_tx_delays(
                                   rfnv_wcdma_tbl_ptr[band]->agc_tx_on_rise_delay,
                                   rfnv_wcdma_tbl_ptr[band]->agc_tx_on_fall_delay,
                                   rfnv_wcdma_tbl_ptr[band]->agc_pa_on_rise_delay,
                                   rfnv_wcdma_tbl_ptr[band]->agc_pa_on_fall_delay
                                 );
  //rfwcdma_mdsp_set_btf_delay((int16)rfnv_wcdma_tbl_ptr->enc_btf_dly);

  /* Set Tx AGC control LUT SSBI addr registers */
  rfwcdma_mdsp_write_tx_agc_sbi_ctl( band );

  /* Initialize TXP2 */
  rfwcdma_core_txctl_set_tx_gtr_thresh_params(device, band);

}
/*--------------------------------------------------------------------------*/
/*!
  @brief

  @details
  This function initializes MSM and mDSP PA parameters.

*/
void rfwcdma_core_txctl_init_pa_ctl(void)
{
  /*!@todo anupamav: to init pa ctl the band is not needed. Need to modify the mdsp function */
  rfwcdma_mdsp_init_pa_ctl();

}
/*----------------------------------------------------------------------------*/
/*!
  @todo push to utility
  @brief
  This function freeze/unfreezes the TX accumulator.

  @details
*/
int16 rfwcdma_core_txctl_set_tx_freezeaccum(uint8 val,rfcom_multi_carrier_id_type car_idx)
{
  rfwcdma_mdsp_set_txfreezeaccum(val,car_idx);

  return 0;
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function enables UL closed loop power control by removing the TX AGC
  gain value override.

  @details
*/
int16 rfwcdma_core_txctl_enable_pwr_ctrl(void)
{
  rfwcdma_mdsp_enable_pwr_ctrl();

  return 0;
}

/*--------------------------------------------------------------------------*/
/*!
  @brief
  Check if we want to populate the SMPS PA Char table from NV or not
  
  @details

*/
boolean rfwcdma_core_txctl_smps_pa_nv_validity(rfnv_wcdma_nv_tbl_type *rfnv_wcdma_tbl)
{
  boolean *nv_flag = NULL;

  // nv_flag = rfnv_wcdma_tbl->smps_pa_bias->nv_valid_flag;

  if (nv_flag != NULL)
  {
    return(nv_flag[0] && nv_flag[1]);
  }

  return FALSE;
}

/*--------------------------------------------------------------------------*/
/*!
  @brief
  
  @details
  This function gathers all required information and programs TX_GTR_THRESH
  parameters
*/
void rfwcdma_core_txctl_set_tx_gtr_thresh_params(rfm_device_enum_type device, 
                                                 rfcom_wcdma_band_type band)
{
  static boolean is_grfc_initialized = FALSE;
  static int32 tx_gtr_thresh_grfc_engine_num = -1;
  rfc_sig_cfg_type rfc_grfc_list[RFC_MAX_NUM_OF_GRFC];
  rfc_sig_cfg_type *rfc_tx_grfc_tbl_ptr = NULL;
  int32 number_of_grfc = -1;
  int16 rise_agc_val, fall_agc_val;

  /* Query GRFC number if it had not been done */
  if(!is_grfc_initialized)
  {
    /* Get Tx GRFC Info */
    rfc_tx_grfc_tbl_ptr =  (rfc_sig_cfg_type *) rfc_wcdma_get_mode_config_data(RFC_MODE_TX_GRFC_ENGINE_DATA);

    /* Check if sig table from mode_config_data is NULL */
    if(rfc_tx_grfc_tbl_ptr == NULL)
    {
      RF_MSG(RF_ERROR,
             "rfwcdma_core_txctl_set_tx_gtr_thresh_params: NULL Tx GRFC info");
      return;
    }

    /* Null pointer check */
    if (band < RFCOM_NUM_WCDMA_BANDS)
    {
      if (rfnv_wcdma_tbl_ptr[band] == NULL)
      {
        RF_MSG_1(RF_ERROR,
                 "rfwcdma_core_txctl_set_tx_gtr_thresh_params: NULL NV tbl ptr for band %d",
                 band);
        return;
      }
    }

    else
    {
       RF_MSG_1(RF_ERROR,
                 "rfwcdma_core_txctl_set_tx_gtr_thresh_params: Not a valid band %d",
                 band);
       return;
    }

    /* Query GRFC engine number. If RF Card does not support that GRFC pin then it will return -1 */
    number_of_grfc = rfc_common_get_grfc_info(rfc_tx_grfc_tbl_ptr,RFC_TX_GTR_THRESH,&rfc_grfc_list[0]);

    /* If GRFC is supported then pick up the GRFC engine number */
    if(number_of_grfc >=0)
    {
      tx_gtr_thresh_grfc_engine_num = rfc_grfc_list[0].sig_name;
    }

    is_grfc_initialized = TRUE;
  }

  /* Check if GRFC is supported and whether TXP2 is enabled on curr band */
  if((tx_gtr_thresh_grfc_engine_num >= 0) && rfnv_wcdma_tbl_ptr[band]->tx_gtr_thresh.active)
  {
    /* NV is defined in units of  dB*10 whereas FW uses TxAGC units 
       txagcval = (txagcvaldbm - RF_MIN_TX_POWER_DBM) * RF_TXAGC_RESOLUTION + RFAGC_MIN_AGC_VAL
    */

    rise_agc_val = (rfnv_wcdma_tbl_ptr[band]->tx_gtr_thresh.rise_value*RF_TXAGC_RESOLUTION)/10 - RF_MIN_TX_POWER_DBM*RF_TXAGC_RESOLUTION;
    rise_agc_val +=  RFAGC_MIN_AGC_VAL;
    
    fall_agc_val = (rfnv_wcdma_tbl_ptr[band]->tx_gtr_thresh.fall_value*RF_TXAGC_RESOLUTION)/10 - RF_MIN_TX_POWER_DBM*RF_TXAGC_RESOLUTION;
    fall_agc_val +=  RFAGC_MIN_AGC_VAL;
    
    /* Configure TX_GTR_THRESH params*/
    rfwcdma_mdsp_set_tx_gtr_thresh_params(rfnv_wcdma_tbl_ptr[band]->tx_gtr_thresh.active,
                                        rise_agc_val,
                                        fall_agc_val,
                                        (uint8)rfnv_wcdma_tbl_ptr[band]->tx_gtr_thresh.index,
                                        tx_gtr_thresh_grfc_engine_num
                                        );
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function retrieves the PA parameter from NV

  @details
  This function is a utility function to populate PA parameter structure from NV.
  It includes:
  1. PA switchpoints
  2. PA timer hysterisis
  3. BTF delay
  4. PA phase comp

*/
static void rfwcdma_core_get_pa_param
( 
  rfcom_wcdma_band_type band,
  rfwcdma_mdsp_pa_band_param_type* pa_param,
  boolean tx_bw_change
)
{
  int8 pa_idx = 0;
  if (pa_param == NULL)
  {
    RF_MSG(RF_ERROR,
           "rfwcdma_core_get_pa_param: NULL PA params");
    return;
  }

  /* NV table pointer needs to be intialized */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_get_pa_param: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
     RF_MSG_1(RF_ERROR,
               "rfwcdma_core_get_pa_param: Not a valid band %d",
               band);
     return;
  }

  /* Active PA state | PA Range | Timer Hysterisis Values */
  switch(rfnv_wcdma_get_pa_static_nv_version(band))
  {
  
    case RFWCDMA_NV_LEGACY_FOUR_STATE_PA:
      for(pa_idx = 0;pa_idx<RFCOM_TXLIN_NUM_LINEARIZERS;pa_idx++)
      {
        /* Active PA for legacy 4 state PA */
        if(pa_idx < 4)
        {
          pa_param->active_pa_state[pa_idx] = TRUE;
        }
        else
        {
          pa_param->active_pa_state[pa_idx] = FALSE;
        }

        /* PA switchpoint timer hysterisis, limit at 100 bpg, same for all switchpoints */
        /* FED expect the units to be in TxAGC counts 
           100BPG => 10Slots ie, 2 TxAGC on a average per slot
           100/5 => 20 TxAGC counts */
        pa_param->timer_hysteresis[pa_idx] = 
          MAX(rfnv_wcdma_tbl_ptr[band]->pa_static.pa_fall_sw[PA_STATIC_TIMER_HYS_INDEX], RF_WCDMA_PA_HYSTERISIS_MAX);
        pa_param->timer_hysteresis[pa_idx] = (pa_param->timer_hysteresis[pa_idx])/5;
        
        /* PA switchpoint */
        /* Legacy NVs are in dB10 units convert to dBm10 for FED */
        if(pa_idx < RFCOM_TXLIN_NUM_LINEARIZERS-1)
        {
          pa_param->fall[pa_idx] = TXAGC_TO_DBM10((int32)rfnv_wcdma_tbl_ptr[band]->pa_static.pa_fall_sw[pa_idx+1]);
          pa_param->rise[pa_idx] = TXAGC_TO_DBM10((int32)rfnv_wcdma_tbl_ptr[band]->pa_static.pa_rise_sw[pa_idx+1]);    
        }
        
        /* PA phase comp offset at switchpoints from baseband rotator */
        pa_param->rot_angle[pa_idx] = (int16)rfnv_wcdma_tbl_ptr[band]->pa_static.tx_rot_angle[pa_idx];

      }
      break;
      
  case RFWCDMA_NV_PA_STATIC_VER_1:
  case RFWCDMA_NV_PA_STATIC_VER_2:

      for(pa_idx = 0;pa_idx<RFCOM_TXLIN_NUM_LINEARIZERS;pa_idx++)
      {
        if(pa_idx < rfnv_wcdma_tbl_ptr[band]->pa_static.header[PA_STATIC_NUM_ACTIVE_PA])
        {
          pa_param->active_pa_state[pa_idx] = TRUE;
        }
        else
        {
          pa_param->active_pa_state[pa_idx] = FALSE;
        }

        /* PA switchpoint timer hysterisis, limit at 100 bpg, same for all switchpoints */
        /* FED expect the units to be in TxAGC counts 
           PA static NV timer hysterisis values are in usec
           Typical value is 6670us => 6670/66.7 => 100BPGs 
           100BPG => 10Slots ie, 2 TxAGC on a average per slot
           100/5 => 20 TxAGC counts */           
        pa_param->timer_hysteresis[pa_idx] = ((rfnv_wcdma_tbl_ptr[band]->pa_static.pa_fall_sw[PA_STATIC_TIMER_HYS_INDEX])*10)/667;
        pa_param->timer_hysteresis[pa_idx] = MAX(pa_param->timer_hysteresis[pa_idx], RF_WCDMA_PA_HYSTERISIS_MAX);
        pa_param->timer_hysteresis[pa_idx] = (pa_param->timer_hysteresis[pa_idx])/5;
        
        /* PA switchpoint */
        /* PA static NVs are in dBm10 units directly passed on to FED */
        if(pa_idx < RFCOM_TXLIN_NUM_LINEARIZERS-1)
        {
          pa_param->fall[pa_idx] = (int32)rfnv_wcdma_tbl_ptr[band]->pa_static.pa_fall_sw[pa_idx+1];
          pa_param->rise[pa_idx] = (int32)rfnv_wcdma_tbl_ptr[band]->pa_static.pa_rise_sw[pa_idx+1];    
        }
        
        /* PA phase comp offset at switchpoints from baseband rotator */
        pa_param->rot_angle[pa_idx] = (int16)rfnv_wcdma_tbl_ptr[band]->pa_static.tx_rot_angle[pa_idx];
        
      }
      break;
      
    default:
      for(pa_idx = 0;pa_idx<RFCOM_TXLIN_NUM_LINEARIZERS;pa_idx++)
      {
        pa_param->active_pa_state[pa_idx] = FALSE;
      }
      break;
  }
  
  /* Set Max RACH power limit and Max DCH power limit */
  pa_param->max_dch_pow_limit = rfwcdma_core_txplim_get_mtpl_nv_item(RFCOM_TXPLIM_DCH, band);
  pa_param->max_dch_pow_limit = pa_param->max_dch_pow_limit + (RF_TXAGC_RESOLUTION * RF_MIN_TX_POWER_DBM);

  pa_param->max_rach_pow_limit = rfwcdma_core_txplim_get_mtpl_nv_item(RFCOM_TXPLIM_RACH, band);
  pa_param->max_rach_pow_limit = pa_param->max_rach_pow_limit + (RF_TXAGC_RESOLUTION * RF_MIN_TX_POWER_DBM);

  /* For FTM Cal mode, disable FW running TxAGC*/
  /* Tx bandwidth change do not enable TxAGC untill the end of init Tx */
  if ((IS_FTM_CAL_MODE())||(tx_bw_change))
  {
    pa_param->txagc_disable = TRUE;
  }
  else
  {
    pa_param->txagc_disable = FALSE;
  }
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function retrieves the Tx MPR parameters from NV

  @details
  This function is a utility function to populate Tx MPR parameter structure from NV.
  It includes:
  1. MPR value for each MPR waveform bin
  2. Beta backoff value  for each MPR waveform bin
  3. TxAGC offset for each MPR waveform bin
  4. PA switchpoint backoff for each MPR waveform bin (excluding R99 waveform)

*/
static void rfwcdma_core_get_tx_mpr_param
( 
  rfcom_wcdma_band_type band,
  rfwcdma_mdsp_mpr_param_type* mpr_param
)
{
  uint8 mpr_bin = 0;
  int8 mpr_table_entries;
  rfm_device_enum_type rfm_tx_device = RFM_MAX_WAN_DEVICES;
   /*Update the rfm_device for tx*/
  rfm_tx_device = rfwcdma_data_get_tx_device();
   
  if (mpr_param == NULL)
  {
    RF_MSG(RF_ERROR,
           "rfwcdma_core_get_tx_mpr_param: NULL MPR params");
    return;
  }

  /* NV table pointer needs to be intialized */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_get_tx_mpr_param: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
     RF_MSG_1(RF_ERROR,
               "rfwcdma_core_get_tx_mpr_param: Not a valid band %d",
               band);
     return;
  }

  /*If combination value exist, use new value, otherwisw use old value*/
  mpr_table_entries = rfnv_wcdma_tbl_ptr[band]->mpr_backoff.mpr_table_entries;
  if((mpr_table_entries > 0) && 
      (mpr_table_entries <= RF_WCDMA_MAX_NUM_MPR_BINS))
  {
    
    RF_MSG_2( RF_LOW, "rfwcdma_core_txplim_load_mpr_backoff: multi_carrier_idx=%d "
                         "mpr_table_entries=%d", rfwcdma_data_get_num_carriers(rfm_tx_device), mpr_table_entries);

    for ( mpr_bin = 0; mpr_bin < mpr_table_entries; mpr_bin++ )
    {
      if(RFCOM_DUAL_CARRIER == rfwcdma_data_get_num_carriers(rfm_tx_device))
      {
        mpr_param->mpr_val[mpr_bin] = 
                 (uint8)rfnv_wcdma_tbl_ptr[band]->mpr_backoff.dc_backoff.tx_mpr_backoff[mpr_bin];
        mpr_param->beta_backoff[mpr_bin] = 
                 rfnv_wcdma_tbl_ptr[band]->mpr_backoff.dc_backoff.tx_beta_scaling_comp[mpr_bin];
        mpr_param->txagc_offset[mpr_bin] = 
                 (uint8)rfnv_wcdma_tbl_ptr[band]->mpr_backoff.dc_backoff.tx_agc_offset[mpr_bin];
      }
      else
      {
        mpr_param->mpr_val[mpr_bin] = 
                 (uint8)rfnv_wcdma_tbl_ptr[band]->mpr_backoff.sc_backoff.tx_mpr_backoff[mpr_bin];
        mpr_param->beta_backoff[mpr_bin] = 
                 rfnv_wcdma_tbl_ptr[band]->mpr_backoff.sc_backoff.tx_beta_scaling_comp[mpr_bin];
        mpr_param->txagc_offset[mpr_bin] = 
                 (uint8)rfnv_wcdma_tbl_ptr[band]->mpr_backoff.sc_backoff.tx_agc_offset[mpr_bin];
      }
      
      if(mpr_bin < (mpr_table_entries - 1))
      {
        if(RFCOM_DUAL_CARRIER == rfwcdma_data_get_num_carriers(rfm_tx_device))
        {
          mpr_param->pa_thresh_backoff[mpr_bin] = 
                 (uint8)rfnv_wcdma_tbl_ptr[band]->mpr_backoff.dc_backoff.pa_mpr_backoff[mpr_bin];
        }
        else
        {
          mpr_param->pa_thresh_backoff[mpr_bin] = 
                  (uint8)rfnv_wcdma_tbl_ptr[band]->mpr_backoff.sc_backoff.pa_mpr_backoff[mpr_bin];
        }
      }
    }
  }
  else
  {
    mpr_table_entries = RFTXPLIM_MPR_BETA_TABLE_ENTRIES;

    for ( mpr_bin=0; mpr_bin < mpr_table_entries; mpr_bin++ )
    {
      /* MPR value (max power back off value)*/
      mpr_param->mpr_val[mpr_bin]      = (uint8)rfnv_wcdma_tbl_ptr[band]->wcdma_rel6_tx_mpr_backoff[mpr_bin];

      /* Beta backoff value */
      mpr_param->beta_backoff[mpr_bin] = rfnv_wcdma_tbl_ptr[band]->wcdma_rel6_tx_beta_scaling_comp[mpr_bin];

      /* TxAGC offset value*/
      mpr_param->txagc_offset[mpr_bin] = (uint8)rfnv_wcdma_tbl_ptr[band]->wcdma_rel6_tx_agc_offset[mpr_bin];

      /* PA switchpoint backoff value */
      if(mpr_bin < mpr_table_entries - 1)
      {
        mpr_param->pa_thresh_backoff[mpr_bin] = (uint8)rfcommon_nv_mm_tbl.wcdma_rel7_pa_mpr_backoff[mpr_bin];
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function retrieves the XPT parameters from NV

  @details
  This function is a utility function to populate XPT parameter structure from NV.
  It includes:
  1.
 
  @note
  Some parameters will likely be obsolete when we move to common TxAGC 

*/
static void rfwcdma_core_get_xpt_param
(
  rfcom_wcdma_band_type band,
  rfwcdma_mdsp_xpt_param_type* xpt_param,
  uint32 txlm_handle
)
{
  tx_band_cal_type *tx_band_cal_data = rfwcdma_core_txlin_get_band_data(band);
  tx_linearizer_aux_data_type aux_lin_version = TX_LINEARIZER_NO_AUX_DATA;
  int pa_index = 0;

  /* Null pointer check */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    /* NV table pointer needs to be intialized */
    if ((rfnv_wcdma_tbl_ptr[band] == NULL)||(xpt_param == NULL)||(tx_band_cal_data == NULL))
    {
      RF_MSG( RF_ERROR, "rfwcdma_core_get_xpt_param: NULL pointer passed in!");
    }
    else
    {
      /* Populate XPT mode per PA state from linearizer */
      for ( pa_index = 0; pa_index < RFCOM_TXLIN_NUM_LINEARIZERS; pa_index++)
      {
        aux_lin_version = 
                  tx_band_cal_data->tx_pa_state_cal_data[pa_index].\
                    linearizer_index[TX_LINEARIZER_VOICE][0].node_data.aux_lin_data;
        switch ( aux_lin_version )
        {
          case TX_LINEARIZER_NO_AUX_DATA:
            /* APT mode */
            xpt_param->xpt_mode[pa_index] = RFLM_TXAGC_APT;
            break;
          case TX_LINEARIZER_EPT_DPD_V2_DATA:
            /* EPT mode */
            xpt_param->xpt_mode[pa_index] = RFLM_TXAGC_EPT;
            break;
          case TX_LINEARIZER_ET_DPD_DATA:
            /* ET mode */
            xpt_param->xpt_mode[pa_index] = RFLM_TXAGC_ET;
            break;
          default:
            RF_MSG( RF_ERROR, 
                    "rfwcdma_core_get_xpt_param: Invalid dpd_type" );
            xpt_param->xpt_mode[pa_index] = RFLM_TXAGC_APT;
            break;
        }
      }

      /* Remove later as WFW dependence removed */
      xpt_param->ept_pa_state = 0x2;
      xpt_param->smps_mode = 3;

      #ifdef FEATURE_TABASCO_MODEM 

      /* IQ set point for APT, EPT, ET*/
      xpt_param->iq_setpoint_apt = 
              (uint16)rfwcdma_msm_get_default_txc_gain(txlm_handle);
      xpt_param->iq_setpoint_ept = 
              (uint16)rfwcdma_core_xpt_get_cfg_param(band,EPT_CFG_CMN_IQ_GAIN_UNITY);
      xpt_param->iq_setpoint_et = 
              (uint16)rfwcdma_core_xpt_get_cfg_param(band,EPT_CFG_CMN_IQ_GAIN_UNITY);
      
      /* Envelope Scale for EPT and ET */
      xpt_param->env_scale_setpoint_ept =  
              (int16)rfwcdma_core_xpt_get_cfg_param(band,EPT_CFG_CMN_EVN_SCALE_UNITY);
      xpt_param->env_scale_setpoint_et =  
              (int16)rfwcdma_core_xpt_get_cfg_param(band,ET_CFG_CMN_ENV_SCALE_UNITY);
      
      /* IQ setpoint and Envelop Scale for 2x (10MHz Tx DC)*/
      xpt_param->iq_setpoint_apt_2x = (int16)SETPOINT_BACKOFF_2X(xpt_param->iq_setpoint_apt);
      xpt_param->iq_setpoint_ept_2x = (int16)(xpt_param->iq_setpoint_ept);
      xpt_param->iq_setpoint_et_2x = (int16)(xpt_param->iq_setpoint_et);
      
      xpt_param->env_scale_setpoint_ept_2x = 
              (int16)SETPOINT_BACKOFF_2X_EPT_ET(xpt_param->env_scale_setpoint_ept);
      xpt_param->env_scale_setpoint_et_2x = 
              (int16)SETPOINT_BACKOFF_2X_EPT_ET(xpt_param->env_scale_setpoint_et);

     #else

      /* IQ set point for APT, EPT, ET*/
      xpt_param->iq_setpoint_apt = 
              (uint16)rfwcdma_msm_get_default_txc_gain(txlm_handle);
      xpt_param->iq_setpoint_ept = 
              (uint16)rfwcdma_core_xpt_get_cfg_param(band,EPT_CFG_CMN_IQ_GAIN_UNITY);
      xpt_param->iq_setpoint_et = 
              (uint16)rfwcdma_core_xpt_get_cfg_param(band,EPT_CFG_CMN_IQ_GAIN_UNITY);
      
      /* Envelope Scale for EPT and ET */
      xpt_param->env_scale_setpoint_ept =  
              (int16)rfwcdma_core_xpt_get_cfg_param(band,EPT_CFG_CMN_EVN_SCALE_UNITY);
      xpt_param->env_scale_setpoint_et =  
              (int16)rfwcdma_core_xpt_get_cfg_param(band,ET_CFG_CMN_ENV_SCALE_UNITY);
      
      /* IQ setpoint and Envelop Scale for 2x (10MHz Tx DC)*/
      xpt_param->iq_setpoint_apt_2x = (int16)xpt_param->iq_setpoint_apt;
      xpt_param->iq_setpoint_ept_2x = (int16)xpt_param->iq_setpoint_ept;
      xpt_param->iq_setpoint_et_2x = (int16)xpt_param->iq_setpoint_et;
      
      xpt_param->env_scale_setpoint_ept_2x = (int16)xpt_param->env_scale_setpoint_ept;
      xpt_param->env_scale_setpoint_et_2x =  (int16)xpt_param->env_scale_setpoint_et;

      #endif
	  
    }
  }
  else
  {
     RF_MSG( RF_ERROR, "rfwcdma_core_get_xpt_param: invalid band!");
  }

}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function gets Tx device configuration
 
  @details
  This function is a utility function to get Tx device configuration(script)
  in from device driver and populate mdsp Tx cfg data structure. It includes
  1. PA ON script
  2. PA OFF script
  3. Tx ON script
  4. Tx OFF script
  5. DTx script
 
  @note
  PA state, current, bias, WTR gain and other TxAGC script will be handled by common
  TxAGC. 

*/

static void rfwcdma_core_get_tx_device_cfg
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  uint16 tx_chan,
  uint32 txlm_handle,
  rfwcdma_mdsp_tx_device_cfg_type* dev_cfg
)
{
  rfwcdma_core_tx_dev_cfg_type dev_cfg_type = TX_DEV_CFG_NUM;
  rf_buffer_intf *tx_dev_cfg_buf[TX_DEV_CFG_NUM] = {NULL};
  rfc_wcdma_core_config_type rfc_core_config = {0};
  const rf_hal_bus_resource_script_type* resource_ptr = NULL;
  rfdevice_coupler_tech_cfg_type coupler_tech_cfg;
  uint8 num_of_rffe_trans = 0;
  uint16 channels[RFCOM_MAX_CARRIERS] = {0};
  uint32 wtr_tune_execution_time = 0;
  rfdevice_tx_lut_data_type lut_data = {0};
  rfdevice_rxtx_common_class *device_ptr = NULL;
  boolean device_api_status = FALSE;
  rfcom_band_type_u tech_band = {0};

  rflm_autopin_dm_template_t *autopin_template = NULL;
  fw_autopin_capture_input_t *reflog_capture_input = NULL;
  uint16 wtr_capture_size = 0;
  boolean is_ftm_mode = IS_FTM_IN_TEST_MODE();

  if (dev_cfg == NULL)
  {
    RF_MSG(RF_ERROR,
           "rfwcdma_core_get_tx_device_cfg: NULL device config");
    return;
  }

  /* NV table pointer needs to be intialized */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_get_tx_device_cfg: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
     RF_MSG_1(RF_ERROR,
               "rfwcdma_core_get_tx_device_cfg: Not a valid band %d",
               band);
     return;
  }
  
  for (dev_cfg_type = TX_ON_CFG; dev_cfg_type < TX_DEV_CFG_NUM; dev_cfg_type ++)
  {
    /* Tx TUNE needs a bigger buffer because of its large script size */
    if (dev_cfg_type == TX_TUNE)
    {
      tx_dev_cfg_buf[dev_cfg_type] = rf_buffer_create(
        RF_WCDMA_TX_GEN_SSBI_SCRIPT_SIZE,
        RF_WCDMA_TX_TUNE_RFFE_SCRIPT_SIZE,
        RF_WCDMA_TX_GEN_GRFC_SCRIPT_SIZE);
    }
#ifdef FEATURE_RF_HAS_WTR2965 
    else if ((dev_cfg_type == ENABLE_AUTOPIN_FBRX_G1)||(dev_cfg_type == ENABLE_AUTOPIN_FBRX_G2))
    {
      tx_dev_cfg_buf[dev_cfg_type] = rf_buffer_create(
        RF_WCDMA_TX_GEN_SSBI_SCRIPT_SIZE,
        RF_WCDMA_AUTOPIN_RFFE_SCRIPT_SIZE,
        RF_WCDMA_TX_GEN_GRFC_SCRIPT_SIZE);
    }
#endif 	 	
    else
    {
      tx_dev_cfg_buf[dev_cfg_type] = rf_buffer_create(
        RF_WCDMA_TX_GEN_SSBI_SCRIPT_SIZE,
        RF_WCDMA_TX_GEN_RFFE_SCRIPT_SIZE,
        RF_WCDMA_TX_GEN_GRFC_SCRIPT_SIZE);
    }

    if (tx_dev_cfg_buf[dev_cfg_type] == NULL)
    {
      RF_MSG_1( RF_ERROR, 
                "rfwcdma_core_get_tx_device_cfg: RF buffer allocation failed:%d ",
                dev_cfg); 
      break;
    }

    rf_buffer_clear(tx_dev_cfg_buf[dev_cfg_type]);
  }

  /* -----------------------------Get Tx ON script--------------------------------------------- */

  rfdevice_wcdma_set_rf_on(device, band, TRUE, RFDEVICE_CREATE_SCRIPT, tx_dev_cfg_buf[TX_ON_CFG]); 
  dev_cfg->tx_on.script = tx_dev_cfg_buf[TX_ON_CFG];

  /*WTR ON rise delay in cx8 unit. tx on time = frame boundary - 2BPG + tx_on_rise_delay*/
  dev_cfg->tx_on.delay = rfnv_wcdma_tbl_ptr[band]->agc_tx_on_rise_delay;


  /* -----------------------------Get Tx OFF script--------------------------------------------- */

  rfdevice_wcdma_set_rf_on(device, band, FALSE, RFDEVICE_CREATE_SCRIPT, tx_dev_cfg_buf[TX_OFF_CFG]); 

  /* Append the device scripts for setting RGI = 0 into Tx OFF script
  This is to leave the PA in the lowest gain state after Tx OFF */
  lut_data.bbf_script = tx_dev_cfg_buf[TX_OFF_CFG];
  lut_data.tx_gain_script = tx_dev_cfg_buf[TX_OFF_CFG];
  lut_data.trigger_script = tx_dev_cfg_buf[TX_OFF_CFG];

  /* Get the device pointer for the WTR */
  device_ptr = rfdevice_wcdma_tx_get_common_dev_ptr(device, band);

  /* Set the WCDMA band */
  tech_band.wcdma_band = band;

  /* Query LUT data */
  device_api_status = rfdevice_cmn_get_tx_lut_scripts( device_ptr, 
                                                       RFM_IMT_MODE, 
                                                       tech_band,
                                                       0, // RGI = 0 means PA range = 0
                                                       0, // RGI = 0
                                                       RFDEVICE_CREATE_SCRIPT,
                                                       &lut_data );

  if (device_api_status == FALSE) 
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_get_tx_device_cfg: Get Tx LUT scripts failed for RGI 0 during Tx OFF script preparation");
  }

  dev_cfg->tx_off.script = tx_dev_cfg_buf[TX_OFF_CFG];

  /* WTR OFF fall delay in cx8 unit. tx off time = frame boundary + 2BPG + tx_on_fall_delay*/
  dev_cfg->tx_off.delay = rfnv_wcdma_tbl_ptr[band]->agc_tx_on_fall_delay;

  /* -----------------------------Get PA ON script--------------------------------------------- */

  (void)rfdevice_wcdma_pa_on_off(device,
                                 band,
                                 TRUE,
                                 tx_dev_cfg_buf[PA_ON_CFG],
                                 RFDEVICE_CREATE_SCRIPT,
                                 RFCOM_INVALID_TIMING_OFFSET);

  dev_cfg->pa_on.script = tx_dev_cfg_buf[PA_ON_CFG];

  /* PA ON rise delay in cx8 unit. PA on time = frame boundary - 2BPG + pa_on_rise_delay */
  dev_cfg->pa_on.delay = rfnv_wcdma_tbl_ptr[band]->agc_pa_on_rise_delay;

  /* -----------------------------Get PA OFF script--------------------------------------------- */

  (void)rfdevice_wcdma_pa_on_off(device,
                                 band,
                                 FALSE,
                                 tx_dev_cfg_buf[PA_OFF_CFG],
                                 RFDEVICE_CREATE_SCRIPT,
                                 RFCOM_INVALID_TIMING_OFFSET);
  dev_cfg->pa_off.script = tx_dev_cfg_buf[PA_OFF_CFG];

  /* PA OFF fall delay in cx8 unit. PA off time = frame boundary + 2BPG + pa_on_fall_delay */
  dev_cfg->pa_off.delay = rfnv_wcdma_tbl_ptr[band]->agc_pa_on_fall_delay;

  /* -----------------------------Get DTx Disable script--------------------------------------------- */

  rfdevice_wcdma_tx_get_cpc_dtx_val(device,
                                    band,
                                    TRUE,
                                    RFDEVICE_CREATE_SCRIPT,
                                    tx_dev_cfg_buf[DTX_DISABLE_CFG]);

  dev_cfg->dtx[RF_WCDMA_DTX_DISABLE].script = tx_dev_cfg_buf[DTX_DISABLE_CFG];

  /* Timing of DTx script is 0 delay, but FW will schedule the trigger ~3BPH ahead of DTx gap end */
  dev_cfg->dtx[RF_WCDMA_DTX_DISABLE].delay = 0;

  /* -----------------------------Get DTx Enable script--------------------------------------------- */

  /* Get DTx enable script */
  rfdevice_wcdma_tx_get_cpc_dtx_val(device,
                                    band,
                                    FALSE,
                                    RFDEVICE_CREATE_SCRIPT,
                                    tx_dev_cfg_buf[DTX_ENABLE_CFG]);

  dev_cfg->dtx[RF_WCDMA_DTX_ENABLE].script = tx_dev_cfg_buf[DTX_ENABLE_CFG];

  dev_cfg->dtx[RF_WCDMA_DTX_ENABLE].delay = 0;  
  
  /* Get Tx PLL settling time from RFC*/
  (void)rfc_wcdma_command_dispatch(device, RFC_WCDMA_GET_CORE_CONFIG, (void *)(&rfc_core_config));
  dev_cfg->pll_settling_time = (uint32)WCDMA_CONV_US_TO_USTMR(rfc_core_config.tune_time);

  /* 2BPG for Tx ON OFF offset execution - this is a start offset and is fixed */
  dev_cfg->tx_on_off_offset_time = (uint32)WCDMA_CONV_US_TO_USTMR(132);
      
  /* ----------Get Power Meter Trigger Script script--------------------------------------------- */

  /* Get Script from Device */
  rfdevice_hdet_wcdma_get_trigger_scripts( device,
                                           band,
                                           tx_dev_cfg_buf[POWER_METER_TRIGGER],
                                           is_ftm_mode);

  resource_ptr = rf_buffer_get_script_ptr(tx_dev_cfg_buf[POWER_METER_TRIGGER], RF_HAL_BUS_RESOURCE_RFFE);
    
  if(resource_ptr != NULL)
  {
    num_of_rffe_trans = resource_ptr->num_trans;
    RF_MSG_1(RF_MED, "HDET RFFE Script, num_transactions=%d", num_of_rffe_trans); 
  }
  dev_cfg->power_meter_trig.script = tx_dev_cfg_buf[POWER_METER_TRIGGER];
  dev_cfg->power_meter_trig.delay = 0;

  /* ----------Get Coupler FWD script--------------------------------------------- */

  /* Get Script from Device */
  coupler_tech_cfg.mode = RFM_IMT_MODE;
  coupler_tech_cfg.band.wcdma_band = band;
  coupler_tech_cfg.port = 0;
  coupler_tech_cfg.direction = RFDEVICE_COUPLER_DIRECTION_FWD;
  coupler_tech_cfg.gain = RFDEVICE_COUPLER_GAIN_DEFAULT;
  coupler_tech_cfg.output_type = RFDEVICE_COUPLER_OUTPUT_TYPE_DEFAULT;
  rfdevice_wcdma_coupler_config(device,coupler_tech_cfg,
  	                             tx_dev_cfg_buf[TX_CONFIG_COUPLER_FWD],
  	                             RFDEVICE_CREATE_SCRIPT,
  	                             RFCOM_INVALID_TIMING_OFFSET);
    

  resource_ptr = rf_buffer_get_script_ptr(tx_dev_cfg_buf[TX_CONFIG_COUPLER_FWD], RF_HAL_BUS_RESOURCE_RFFE);
      
  if(resource_ptr != NULL)
  {
    RF_MSG_1(RF_MED, "COUPLER FWD RFFE Script, num_transactions=%d", resource_ptr->num_trans); 
  }

  resource_ptr = rf_buffer_get_script_ptr(tx_dev_cfg_buf[TX_CONFIG_COUPLER_FWD], RF_HAL_BUS_RESOURCE_GRFC);

  if(resource_ptr != NULL)
  {
    RF_MSG_4(RF_MED, "COUPLER FWD GRFC Script, num_transactions=%d, grfc_id:%d, start_logic:%d, start_delta:%d", 
      resource_ptr->num_trans, resource_ptr->script_ptr.grfc->grfc_id,
     resource_ptr->script_ptr.grfc->start_logic, resource_ptr->script_ptr.grfc->start_delta); 
  }
  
  dev_cfg->coupler_fwd.script = tx_dev_cfg_buf[TX_CONFIG_COUPLER_FWD];
  dev_cfg->coupler_fwd.delay = 0;


  /* ----------Get Coupler REV script--------------------------------------------- */

  /* Get Script from Device */
  coupler_tech_cfg.mode = RFM_IMT_MODE;
  coupler_tech_cfg.band.wcdma_band = band;
  coupler_tech_cfg.port = 0;
  coupler_tech_cfg.direction = RFDEVICE_COUPLER_DIRECTION_REV;
  coupler_tech_cfg.gain = RFDEVICE_COUPLER_GAIN_DEFAULT;
  coupler_tech_cfg.output_type = RFDEVICE_COUPLER_OUTPUT_TYPE_DEFAULT;
  rfdevice_wcdma_coupler_config(device,
  	                             coupler_tech_cfg,
  	                             tx_dev_cfg_buf[TX_CONFIG_COUPLER_REV],
  	                             RFDEVICE_CREATE_SCRIPT,
  	                             RFCOM_INVALID_TIMING_OFFSET);
    

  resource_ptr = rf_buffer_get_script_ptr(tx_dev_cfg_buf[TX_CONFIG_COUPLER_REV], RF_HAL_BUS_RESOURCE_RFFE);
      
  if(resource_ptr != NULL)
  {
    RF_MSG_4(RF_MED, "COUPLER REV GRFC Script, num_transactions=%d, grfc_id:%d, start_logic:%d, start_delta:%d", 
      resource_ptr->num_trans, resource_ptr->script_ptr.grfc->grfc_id,
     resource_ptr->script_ptr.grfc->start_logic, resource_ptr->script_ptr.grfc->start_delta); 
  }

  resource_ptr = rf_buffer_get_script_ptr(tx_dev_cfg_buf[TX_CONFIG_COUPLER_REV], RF_HAL_BUS_RESOURCE_GRFC);

  if(resource_ptr != NULL)
  {
    RF_MSG_1(RF_MED, "COUPLER REV GRFC Script, num_transactions=%d", resource_ptr->num_trans); 
  }
  
  dev_cfg->coupler_rev.script = tx_dev_cfg_buf[TX_CONFIG_COUPLER_REV];
  dev_cfg->coupler_rev.delay = 0;

 
  /* Get Tx tune script */
  /* Temporary featurizaion for Tx sharing as we dont want to do preload in non S-Tx build
  Assigning the RF buffer has to be out of the featurizaion to avoid leak */
  dev_cfg->tx_tune.script = tx_dev_cfg_buf[TX_TUNE];

  #ifdef FEATURE_MODEM_TX_SHARING
  channels[0] = tx_chan;
  rfwcdma_core_txctl_config_tune_device( device,
                                         band,
                                         channels,
                                         txlm_handle,
                                         tx_dev_cfg_buf[TX_TUNE],
                                         RFDEVICE_CREATE_SCRIPT,
                                         &wtr_tune_execution_time ) ;

  RF_MSG_1(RF_HIGH, "WTR Tx tune trigger execution time:%d ", wtr_tune_execution_time); 

  dev_cfg->tx_tune_time = (uint32)WCDMA_CONV_US_TO_USTMR(wtr_tune_execution_time/10);

  dev_cfg->tx_tune.delay = 0;  
  #endif       

  /* ----------Get THERM Enable Script --------------------------------------------- */
  rfdevice_wcdma_therm_enable( device,
                               band,
                               RFDEVICE_CREATE_SCRIPT,
                               tx_dev_cfg_buf[THERM_ENABLE]
                              );

  resource_ptr = rf_buffer_get_script_ptr(tx_dev_cfg_buf[THERM_ENABLE], RF_HAL_BUS_RESOURCE_RFFE);

  dev_cfg->therm_enable.script = tx_dev_cfg_buf[THERM_ENABLE];
  dev_cfg->therm_enable.delay = 0;

  /* ----------Get Enable Autopin script--------------------------------------------- */
  
#ifdef FEATURE_RF_HAS_WTR2965 
    autopin_template = (rflm_autopin_dm_template_t *)rflm_dm_get_autopin_buf_ptr(txlm_handle);
	   
    if (autopin_template == NULL)
    {
      RF_MSG_1(RF_HIGH, "rfwcdma_core_autopin_init: Failed to get AutoPin buffer from DM for Tx handle %d, can't populate input parameters", txlm_handle);
	  
    }

     else
     {
       wtr_capture_size = autopin_template->autopin_ctl.wtr_capture_size;
       RF_MSG_1(RF_HIGH, "Enable Autopin Script: wtr_capture_size %d ", wtr_capture_size); 
     }

  
   /* Get the device pointer for the WTR */
   device_ptr = rfdevice_wcdma_tx_get_common_dev_ptr(device, band);

   if(device_ptr == NULL)
   	{
   	 RF_MSG(RF_ERROR, "Enable Autopin Script: Tx Dev pointer is NULL  "); 
   	}

     	
       /* Get Script from Device for Gain State 1 */
	rfdevice_cmn_fbrx_autopin_enable(device_ptr,
					 RFDEVICE_CREATE_SCRIPT,
 					 tx_dev_cfg_buf[ENABLE_AUTOPIN_FBRX_G1],
  					 1 /*WTR2955_FBRX_G1*/,
  					 wtr_capture_size,
  					 wtr_debug_mem_enable);

	resource_ptr = rf_buffer_get_script_ptr(tx_dev_cfg_buf[ENABLE_AUTOPIN_FBRX_G1], RF_HAL_BUS_RESOURCE_RFFE);
    
  if(resource_ptr != NULL)
  {
    num_of_rffe_trans = resource_ptr->num_trans;
    RF_MSG_1(RF_MED, "Autopin RFFE Script G1, num_transactions=%d", num_of_rffe_trans); 
  }

	dev_cfg->autopin_fbrx_gain1.script = tx_dev_cfg_buf[ENABLE_AUTOPIN_FBRX_G1];
	dev_cfg->autopin_fbrx_gain1.delay = 0;  
	
       /* Get Script from Device for Gain State 2 */
	rfdevice_cmn_fbrx_autopin_enable(device_ptr,
					 RFDEVICE_CREATE_SCRIPT,
 					 tx_dev_cfg_buf[ENABLE_AUTOPIN_FBRX_G2],
  					 2 /*WTR2955_FBRX_G2*/,
  					 wtr_capture_size,
  					 wtr_debug_mem_enable);

  resource_ptr = rf_buffer_get_script_ptr(tx_dev_cfg_buf[ENABLE_AUTOPIN_FBRX_G2], RF_HAL_BUS_RESOURCE_RFFE);
    
  if(resource_ptr != NULL)
  {
    num_of_rffe_trans = resource_ptr->num_trans;
    RF_MSG_1(RF_MED, "Autopin RFFE Script G2, num_transactions=%d", num_of_rffe_trans); 
  }

	dev_cfg->autopin_fbrx_gain2.script = tx_dev_cfg_buf[ENABLE_AUTOPIN_FBRX_G2];
        dev_cfg->autopin_fbrx_gain2.delay = 0;  

#endif 
  
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function performs the required clean up of the Tx device config for
  MDSP Tx config.
 
  @details
  This function performs the required clean up of the Tx device config for
  MDSP Tx config. Currently it destroies the local RF buffer created when
  building the devices to free up the memory.

  
*/
static void rfwcdma_core_cleanup_tx_device_cfg
(
  rfcom_wcdma_band_type band,
  rfwcdma_mdsp_tx_device_cfg_type* dev_cfg
)
{
  boolean api_status = TRUE;
  
  /* Null pointer check */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    /* NV table pointer needs to be intialized */
    if ((rfnv_wcdma_tbl_ptr[band] == NULL)||(dev_cfg == NULL))
    {
      return;
    }
  }

  else
  {
     return;
  }

  api_status &= rf_buffer_destroy(dev_cfg->tx_on.script);
  api_status &= rf_buffer_destroy(dev_cfg->tx_off.script);

  api_status &= rf_buffer_destroy(dev_cfg->pa_on.script);
  api_status &= rf_buffer_destroy(dev_cfg->pa_off.script);

  api_status &= rf_buffer_destroy(dev_cfg->dtx[RF_WCDMA_DTX_ENABLE].script);
  api_status &= rf_buffer_destroy(dev_cfg->dtx[RF_WCDMA_DTX_DISABLE].script);

  api_status &= rf_buffer_destroy(dev_cfg->power_meter_trig.script);

  
#ifdef FEATURE_RF_HAS_WTR2965 

  api_status &= rf_buffer_destroy(dev_cfg->autopin_fbrx_gain1.script);

  api_status &= rf_buffer_destroy(dev_cfg->autopin_fbrx_gain2.script);
#endif
  
  api_status &= rf_buffer_destroy(dev_cfg->tx_tune.script);

  api_status &= rf_buffer_destroy(dev_cfg->therm_enable.script);

  api_status &= rf_buffer_destroy(dev_cfg->coupler_fwd.script);

  api_status &= rf_buffer_destroy(dev_cfg->coupler_rev.script);

  if (!api_status)
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_cleanup_tx_device_cfg: fail!"); 
  }
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Populate Tx AGC configuration to RFLM through common Tx AGC configuration
 
  @details
  Provides functionality to query RFC specific information for devices and
  passes on the information to common Tx AGC configuration. Common TxAGC
  will then build Tx AGC sequence information to be used by RFLM TxAGC

  @param band 
  RFM WCDMA band 
 
  @param txlm_handle
  Handle that points to the RFLM buffers
 
  @return
  TRUE if successful, else FALSE 
*/
boolean 
rfwcdma_core_txctl_txagc_config
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band, 
  lm_handle_type txlm_handle
)
{
  boolean api_status = TRUE;
  rfcom_band_type_u rfcom_band;
  rfcommon_core_txlin_agc_device_info device_info;

  /* Retrieve device info */
  api_status &= rfwcdma_core_txctl_get_device_info( device,
                                                    band, 
                                                    &device_info );

  /* Update common band information */
  rfcom_band.wcdma_band = band;

  if (api_status)
  {
      /* Call common Tx AGC configuration */
      api_status &=  rfcommon_core_txlin_config_agc( 
                                   RFM_IMT_MODE, 
                                   device, 
                                   rfcom_band, 
                                   txlm_handle, 
                                   RFCOM_TXLIN_TXAGC_TX_TO_TX, /* WCDMA uses Tx script buffer 0 */ 
                                   device_info.wtr_device, 
                                   device_info.pa_device,
                                   device_info.papm_device,
                                   NULL, /* rf_buffer_intf* tech_asm_tx_agc */ 
                                   NULL, /* tech-specific transactions */
                                   &rfwcdma_txagc_timing_params );
  }
  
  if (!api_status)
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_txagc_config:"
                      " Failed " );
  }

  return api_status;

} /* rfwcdma_core_txctl_txagc_config */

/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function config band specific paramter for Tx
 
  @details
  This function queries NV and device/RFC modules to get Tx band specific
  parameters/scripts and populate the mdsp data structure, then pass it
  to MDSP layer

  @param band
  WCDMA operational band
 
  @param txlm_handle
  TxLM handle
 
  @param tx_bw_change
  Indicator of Tx bandwidth change in this config 
 
*/
void rfwcdma_core_txctl_config_band
( 
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band, 
  uint32 txlm_handle,
  boolean tx_bw_change
)
{
  rfwcdma_mdsp_tx_cfg_bsp_type* mdsp_cfg = NULL;
  boolean api_status = TRUE;
  rfm_device_enum_type prx_device = RFM_INVALID_DEVICE;

   /* NV table pointer needs to be intialized */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_txctl_config_band: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
    RF_MSG_1(RF_ERROR,
               "rfwcdma_core_txctl_config_band: Not a valid band %d",
               band);
    return;
  }

  mdsp_cfg = (rfwcdma_mdsp_tx_cfg_bsp_type*)modem_mem_calloc
    (1, sizeof(rfwcdma_mdsp_tx_cfg_bsp_type), MODEM_MEM_CLIENT_RFA);

  if (mdsp_cfg == NULL)
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_config_band: mdsp_cfg mem_alloc failed!"); 
    return;
  }
    /* Populate PA paramters */
  rfwcdma_core_get_pa_param(band, &mdsp_cfg->pa_band_param, tx_bw_change);

  /* Populate MPR parameters */
  rfwcdma_core_get_tx_mpr_param(band, &mdsp_cfg->mpr_param);

  /* Populate XPT parameters */
  rfwcdma_core_get_xpt_param(band,&mdsp_cfg->xpt_param,txlm_handle);

  /* NPA client for Q6 clock voting in FED */
  mdsp_cfg->npa_handle_initialised = rfwcdma_core_npa_handle_initialised;
  mdsp_cfg->npa_handle_q6clk = rfwcdma_core_npa_handle_q6clk;
  
  /* Pass all parameter/scripts to MDSP */
  mdsp_cfg->rflm_handle = txlm_handle;

  prx_device = rfwcdma_data_get_associated_device_prx_from_tx(device);
  /* Extract the device pair index using the Assoicated Rx device for the Tx device */
  mdsp_cfg->device_pair_idx = rfwcdma_core_get_device_pair(prx_device);

  api_status = rfwcdma_mdsp_async_config_tx_band(mdsp_cfg);

  /* Populate Tx AGC configuration to RFLM 
     through common Tx AGC configuration */
  rfwcdma_core_txctl_txagc_config(device,band, txlm_handle);
  
  if (!api_status)
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_init_bsp_agc_ctl: mdsp config fail! "); 
  }
      
  /* Free up mdsp_cfg */
  modem_mem_free(mdsp_cfg,MODEM_MEM_CLIENT_RFA);
}

/*!
  @brief
  This function populates the PA compensation in TxAGC to FW from NV. The compensation will 
  kick in for one alot after PA state change and will be reverted after that. This is for 
  tweaking ILPC performance (Tx power spikes around switchpoints) 

  @details

  @param band
  WCDMA band 
 
  @param tx_freq
  Tx frequency 

*/
static void rfwcdma_core_get_pa_comp_vs_freq 
(
  rfcom_wcdma_band_type band,
  uint32 tx_freq,
  rfwcdma_mdsp_tx_cfg_chsp_type* cfg
)
{
  int8 pa_idx = 0;
  int8 freq_idx = 0;
  tx_band_cal_type *tx_band_cal_data = rfwcdma_core_txlin_get_band_data(band);
  int16 pa_comp_vs_freq_up[RF_WCDMA_MAX_NUM_PA_STATES] = {0};
  int16 pa_comp_vs_freq_dn[RF_WCDMA_MAX_NUM_PA_STATES] = {0};
  int16 pa_comp_vs_freq_array[RFCOM_TXLIN_NUM_CHANNELS] = {0};
  
  /* NULL pointer checks */
  if ((cfg == NULL)||(tx_band_cal_data == NULL))
  {
    MSG_1(MSG_SSID_RF, MSG_LEGACY_ERROR,"rfwcdma_core_get_pa_comp_vs_freq: null pointer!", 0);
    return;
  }

  /* NV table pointer needs to be intialized */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_get_pa_comp_vs_freq: NULL NV tbl ptr for band %d",
               band);
      return;
    }
  }

  else
  {
     RF_MSG_1(RF_ERROR,
               "rfwcdma_core_get_pa_comp_vs_freq: Not a valid band %d",
               band);
     return;
  }
  
  /* Process pa_comp_vs_freq NV for R1,R2,R3,R4,R5,R6,R7 */
  for(pa_idx = 1; pa_idx<RF_WCDMA_MAX_NUM_PA_STATES; pa_idx++)
  {

    /* Extract and interpolate for pa_comp_vs_freq_up */
    for(freq_idx = 0; freq_idx<RFCOM_TXLIN_NUM_CHANNELS; freq_idx++)
    {
      pa_comp_vs_freq_array[freq_idx] = rfnv_wcdma_tbl_ptr[band]->pa_static.pa_comp_up_vs_freq[freq_idx][pa_idx];
    }
    pa_comp_vs_freq_up[pa_idx] = rf_lininterp_u32_s16( &(tx_band_cal_data->tx_cal_freq[0]),
                                                        pa_comp_vs_freq_array,
                                                        tx_band_cal_data->tx_cal_chan_size,
                                                        tx_freq );
                                                        
    /* Add PA comp freq comp ot base PA comp value and populate the cfg data structure */                                                    
    cfg->pa_comp_up[pa_idx] = rfnv_wcdma_tbl_ptr[band]->pa_static.pa_comp_up[pa_idx] + pa_comp_vs_freq_up[pa_idx];

    /* Extract and interpolate for pa_comp_vs_freq_down */
    for(freq_idx = 0; freq_idx<RFCOM_TXLIN_NUM_CHANNELS; freq_idx++)
    {
      pa_comp_vs_freq_array[freq_idx] = rfnv_wcdma_tbl_ptr[band]->pa_static.pa_comp_dn_vs_freq[freq_idx][pa_idx];
    }
    pa_comp_vs_freq_dn[pa_idx] = rf_lininterp_u32_s16( &(tx_band_cal_data->tx_cal_freq[0]),
                                                        pa_comp_vs_freq_array,
                                                        tx_band_cal_data->tx_cal_chan_size,
                                                        tx_freq );
                                                        
    /* Add PA comp freq comp ot base PA comp value and populate the cfg data structure */                                                    
    cfg->pa_comp_down[pa_idx] = rfnv_wcdma_tbl_ptr[band]->pa_static.pa_comp_dn[pa_idx] + pa_comp_vs_freq_dn[pa_idx];

  }

                           
}

/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function config Tx channel/frequency specific parameters
  
  @details
  This function queries NV module to get Tx channel specific parameters/scripts
  and populate the mdsp data structure, then pass it to MDSP layer
 
  @param band
  WCDMA operational band
 
  @param tx_chan
  WCDMA Tx ARFCN
 
  @param txlm_handle
  TxLM handle
 
  @note
  rfwcdma_core_txctl_config_band needs to be called for band config first. Then can be called
  within the same band channel tuning without re-config band.
 
*/
boolean rfwcdma_core_txctl_config_chan
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band, 
  uint16 tx_chan,
  uint32 txlm_handle,
  boolean tx_bw_change  
)
{
  rfwcdma_mdsp_tx_cfg_chsp_type* mdsp_cfg = NULL;
  boolean api_status = TRUE;
  uint32 tx_freq = rfwcdma_core_get_freq_from_ul_arfcn(tx_chan);

  /* NV table pointer needs to be intialized */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
               "rfwcdma_core_txctl_config_chan: NULL NV tbl ptr for band %d",
               band);
      return FALSE;
    }
  }

  else
  {
    RF_MSG_1(RF_ERROR,
               "rfwcdma_core_txctl_config_chan: Not a valid band %d",
               band);
    return FALSE;
  }

  mdsp_cfg = (rfwcdma_mdsp_tx_cfg_chsp_type*)modem_mem_calloc
    (1, sizeof(rfwcdma_mdsp_tx_cfg_chsp_type), MODEM_MEM_CLIENT_RFA);

  if (mdsp_cfg == NULL)
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_config_chan: mdsp_cfg mem_alloc failed!"); 
    return FALSE;
  }

  /* Update and Populate Tx AGC linearizers - to be removed */
  api_status = rfwcdma_core_txlin_update(device, band, tx_chan, txlm_handle);

  /* Get PA comp data */
  rfwcdma_core_get_pa_comp_vs_freq(band, tx_freq, mdsp_cfg);
  
  /* Update the XPT path delay value . This will be skipped for SC-DC transition. Needed for ET only.*/
  mdsp_cfg->xpt_delay = rfwcdma_core_xpt_path_delay_comp(device,band,tx_chan,FALSE,0,rfwcdma_data_get_num_carriers(device));
  
 if(!tx_bw_change)
 {
  /* Populate Tx device config */
  rfwcdma_core_get_tx_device_cfg(device, band, tx_chan, txlm_handle, &mdsp_cfg->device_cfg);

  /* update TxLM handle */
  mdsp_cfg->rflm_handle = txlm_handle;

  api_status &= rfwcdma_mdsp_async_config_tx_chan(mdsp_cfg);

  /* Do the necessary cleanup of device cfg/RF buffer */
  rfwcdma_core_cleanup_tx_device_cfg(band, &mdsp_cfg->device_cfg);
 }
 else
 {
     RF_MSG_3(RF_HIGH, "rfwcdma_core_txctl_config_chan: Skipping Building Tx Eevnts Dev %d,"
                       "Txlm handle %d, bw change %d", device, txlm_handle, tx_bw_change );
 }

  /* Free up mdsp_cfg */
  modem_mem_free(mdsp_cfg,MODEM_MEM_CLIENT_RFA);

  if (!api_status)
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_config_chan: fail! "); 
  }
  return api_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  Retrieve WCDMA device pointers as required by Tx AGC configuration
 
  @details
  Provides functionality to query RFC specific information for devices and
  passes on the information to common Tx AGC configuration
   
  @param band 
  WCDMA operational band
 
  @param device_info 
  pointer to the device pointer structure
 
  @return
  TRUE if successful, else FALSE 
*/
boolean 
rfwcdma_core_txctl_get_device_info
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  rfcommon_core_txlin_agc_device_info *device_info
)
{
  boolean api_status = TRUE;
  void **pa_obj = NULL;
  void **papm_obj = NULL;
  rfdevice_rxtx_common_class* device_ptr = NULL;

  /* ---------------------------------- PA -------------------------------- */
  /* Get the RF device */
  pa_obj = rfc_wcdma_get_device( device, 
                                 RFC_CONFIG_TX, 
                                 band, 
                                 RFDEVICE_PA );
  if (pa_obj != NULL)
  {
    if (pa_obj[0] != NULL)
    {
      device_info->pa_device = pa_obj[0];  
    }
    else
    {
      RF_MSG( RF_ERROR, "rfwcdma_core_txctl_get_device_info:"
                        " Null PA device not supported" );
      api_status &= FALSE;
    }
  }
  else
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_get_device_info:"
                      " Null PA device not supported" );
    api_status &= FALSE;
  }

  /* ---------------------------------- WTR ------------------------------- */
  /* always get the device pointer for the 1st WTR until device support added */
    device_ptr= rfdevice_wcdma_tx_get_common_dev_ptr(device, band);

  if ( device_ptr != NULL )
  {
    device_info->wtr_device = (void*)device_ptr;
  }
  else
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_get_device_info:"
                      " Null WTR device not supported" );
    api_status &= FALSE;
  }

  /* ---------------------------------- PAPM ------------------------------ */
  /* Get Power tracker object from RFC */
  papm_obj = rfc_wcdma_get_device( device, 
                                   RFC_CONFIG_TX, 
                                   band, 
                                   RFDEVICE_PAPM );

  if (papm_obj != NULL)
  {
    if (papm_obj[0] != NULL)
    {
      device_info->papm_device = papm_obj[0];
    }
    else
    {
      RF_MSG_1( RF_ERROR, "rfwcdma_core_txctl_get_device_info:"
                " Null PAPM device not supported_papm_obj, api_status %d",api_status );
      device_info->papm_device  = NULL;
      //api_status &= FALSE;
    }
  }
  else
  {
    RF_MSG_1( RF_ERROR, "rfwcdma_core_txctl_get_device_info:"
                      " Null PAPM device not supported, api_status %d",api_status );
    device_info->papm_device = NULL;
    //api_status &= FALSE;
  }

  if (!api_status)
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_get_device_info:"
                      " Failed!" );
  }
  return api_status;
} /* rfwcdma_core_txctl_get_device_info */

/*--------------------------------------------------------------------------*/
/*!
  @brief
  This function config Tx multi carrier specific parameters
  
  @details
  This function config Tx parameters that are only multi carrier specific.
  Althogh it takes Tx channel as parameter, regular Tx channel sepcific parameters
  are configured by chan config.
 
  @param tx_chan
  Tx channel array for UL multi-carrier config. Currently only support SC and DC.
 
  @param multi_carrier_idx
  Tx multi carrier index. urrently only support SC and DC.
 
  @param band
  Tx band
 
  @param txlm_handle
  RFLM/TxLM handle 
 
*/
void rfwcdma_core_txctl_config_mc
(
  rfcom_wcdma_band_type band, 
  uint32 multi_carrier_idx,
  uint16 tx_chan[RFCOM_MAX_CARRIERS],
  uint32 txlm_handle,
  rfcom_tx_multi_carrier_cfg_type tx_cfg_mode   
)
{
  rfwcdma_mdsp_tx_mc_cfg_type* mdsp_cfg = NULL;
  int64 tx_freq_hz_offset, tx_freq_hz_offset_shift = 0;
  uint32 txc_input_freq_hz;
#ifdef FEATURE_THOR_MODEM
  uint32 txfe_input = 0;
#endif
  mdsp_cfg = (rfwcdma_mdsp_tx_mc_cfg_type*)modem_mem_calloc
    (1, sizeof(rfwcdma_mdsp_tx_mc_cfg_type), MODEM_MEM_CLIENT_RFA);

  if (mdsp_cfg == NULL)
  {
    RF_MSG( RF_ERROR, "rfwcdma_core_txctl_config_mc: mdsp_cfg mem_alloc failed!"); 
    return;
  }

  RF_MSG_3( RF_ERROR, "rfwcdma_core_txctl_config_mc, chan0:%d, chan1:%d, band:%d",tx_chan[0],tx_chan[1],band); 

  /* Populate RFLM/TxLM handle */
  mdsp_cfg->rflm_handle = (rflm_dm_handle_id_t)txlm_handle;

  /* Populate PDA base and offset */
  mdsp_cfg->tx_pda_ctrl_base[0] = RF_WCDMA_TX_MC_PDA_CTL_BASE;
  mdsp_cfg->tx_pda_ctrl_base[1] = RF_WCDMA_TX_MC_PDA_CTL_BASE;

  mdsp_cfg->tx_state = (rflm_tx_multi_carrier_cfg_type)tx_cfg_mode;

  /* SC (5MHz) config */
  if (multi_carrier_idx == RFWCDMA_MC_SC)
  {
    mdsp_cfg->tx_pda_ctrl_offset[0]  = RF_WCDMA_TX_MC_PDA_CTL_OFFSET_SC;
    mdsp_cfg->tx_pda_ctrl_offset[1]  = RF_WCDMA_TX_MC_PDA_CTL_OFFSET_SC;
  }
  else if (multi_carrier_idx == RFWCDMA_MC_DC)
  {
  /* DC (10MHz) config*/
   #ifdef FEATURE_THOR_MODEM
    tx_freq_hz_offset = 
      ((int64)rfwcdma_core_get_freq_from_uarfcn_no_limit(band, tx_chan[1], RFWCDMA_CORE_UARFCN_UL)-
       (int64)rfwcdma_core_get_freq_from_uarfcn_no_limit(band, tx_chan[0], RFWCDMA_CORE_UARFCN_UL))*1000/2;
   #else
    tx_freq_hz_offset = 
      ((int64)rfwcdma_core_get_freq_from_uarfcn_no_limit(band, tx_chan[0], RFWCDMA_CORE_UARFCN_UL)-
       (int64)rfwcdma_core_get_freq_from_uarfcn_no_limit(band, tx_chan[1], RFWCDMA_CORE_UARFCN_UL))*1000/2;
   #endif
    /* convert Frequency offset to 24 bit for 2's complement */
    // tx_freq_hz_offset &= 0x00FFFFFF;


    /* PDA control offset = frequency_offset_in_hz * Rotator resoltuion / TxFE_input_rate **/
  //  mdsp_cfg->tx_pda_ctrl_offset[0] = (int32)((tx_freq_hz_offset<<RFWCDMA_MSM_TX_ROT_ROSOLUTION)/RFWCDMA_MSM_TXFE_IN_RATE);

    tx_freq_hz_offset_shift = ((int64)tx_freq_hz_offset)<<RFWCDMA_MSM_TX_ROT_ROSOLUTION;

#ifdef FEATURE_THOR_MODEM
    txfe_input = (uint32)rfwcdma_msm_get_txfe_in_rate(band);
    mdsp_cfg->tx_pda_ctrl_offset[0] = tx_freq_hz_offset_shift/txfe_input;
    mdsp_cfg->tx_pda_ctrl_offset[1] = (~mdsp_cfg->tx_pda_ctrl_offset[0]+1);
    /* Final unit is 24 bit resolution */
    mdsp_cfg->tx_pda_ctrl_offset[0] &= 0x00FFFFFF;
    mdsp_cfg->tx_pda_ctrl_offset[1] &= 0x00FFFFFF;
    RF_MSG_2( RF_HIGH, "PDA offset 0 %d PDA offset 1 %d", mdsp_cfg->tx_pda_ctrl_offset[0], mdsp_cfg->tx_pda_ctrl_offset[1]);
#else
    txc_input_freq_hz = rfwcdma_msm_get_txc_input_freq(txlm_handle);
    if (txc_input_freq_hz == 0)
    {
      RF_MSG_1(RF_ERROR,"rfwcdma_core_txctl_config_mc, txc_input_freq_hz:%d ",txc_input_freq_hz);
      /* Free up mdsp_cfg */
      modem_mem_free(mdsp_cfg,MODEM_MEM_CLIENT_RFA);
      return;
    }	
    mdsp_cfg->tx_pda_ctrl_offset[0] = tx_freq_hz_offset_shift/(txc_input_freq_hz);
    mdsp_cfg->tx_pda_ctrl_offset[1] = (~mdsp_cfg->tx_pda_ctrl_offset[0]+1);
    /* Final unit is 24 bit resolution */
    mdsp_cfg->tx_pda_ctrl_offset[0] &= 0x003FFFFF;
    mdsp_cfg->tx_pda_ctrl_offset[1] &= 0x003FFFFF;
    RF_MSG_3( RF_HIGH, "PDA offset 0 0x%x PDA offset 1 0x%x,  txc_input_freq_hz %d ", mdsp_cfg->tx_pda_ctrl_offset[0], mdsp_cfg->tx_pda_ctrl_offset[1], txc_input_freq_hz);

#endif    
  

  }
  else
  {
    RF_MSG_1(RF_ERROR,
             "rfwcdma_core_txctl_config_mc, unsupport multi carrier idx:%d",
             multi_carrier_idx);
  }

  /* Todo: MC freq comp needs to be added, populate with 0 for now */

  /* Push the multi carrier cfg to MDSP/FED */
  rfwcdma_mdsp_async_config_tx_mc(mdsp_cfg);

  /* Free up mdsp_cfg */
  modem_mem_free(mdsp_cfg,MODEM_MEM_CLIENT_RFA);
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function disables TxAGC update in FED
  @details
  This function disable TxAGC update in FED 
 
  @param rflm_handle
  RFLM handle
 
  @param disable
  Boolean to disable/enable TxAGC. 1 = Disable
 
  @return
  TRUE if successful, else FALSE
 
*/
boolean rfwcdma_core_tx_disable_txagc
(
  rflm_dm_handle_id_t rflm_handle,
  boolean disable
)
{
  rfwcdma_mdsp_set_data_param_type disable_txagc_param = { 0 };

  disable_txagc_param.set_data_cmd = RFWCDMA_MDSP_DISABLE_TxAGC;
  disable_txagc_param.rflm_handle = rflm_handle;
  disable_txagc_param.data.gen_b = disable; 
  return(rfwcdma_mdsp_async_set_data(&disable_txagc_param));
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function disables TxAGC update in ftm mode
  @details
  This function disables TxAGC update in ftm mode
 
  @param rflm_handle
  RFLM handle
 
  @param disable
  Boolean to disable/enable TxAGC. 1 = Disable
 
  @return
  TRUE if successful, else FALSE
 
*/
boolean rfwcdma_core_tx_ftm_disable_ldo_off
(
  rflm_dm_handle_id_t rflm_handle,
  boolean disable
)
{
  rfwcdma_mdsp_set_data_param_type ftm_disable_ldo_off_param = { 0 };

  ftm_disable_ldo_off_param.set_data_cmd = RFWCDMA_MDSP_FTM_DISABLE_LDO_OFF;
  ftm_disable_ldo_off_param.rflm_handle = rflm_handle;
  ftm_disable_ldo_off_param.data.gen_b = disable; 
  return(rfwcdma_mdsp_async_set_data(&ftm_disable_ldo_off_param));
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function applies a Tx frequency error (in Hz) compensation value to mdsp
  @details
  This function applies a Tx frequency error (in Hz) compensation value to mdsp
  module 
 
  @param rflm_handle
  RFLM handle
 
  @param band
  WCDMA band
 
  @param rx_chan
  Rx channel number
 
  @param TxFreqError
  he Frequency compensation to apply in Hz
 
  @return
  TRUE if successful, else FALSE
 
*/
boolean rfwcdma_core_tx_set_freq_adjust
(
  rflm_dm_handle_id_t rflm_handle,
  rfcom_wcdma_band_type band,
  uint16 rx_chan,
  int32 tx_freq_error
)
{
  boolean ret_val = TRUE;
  rfwcdma_mdsp_set_data_param_type set_freq_offset_param = { 0 };
  /* Initialize all agc command parameters to 0 */
  rfwcdma_mdsp_agc_cmd_param_type agc_cmd_param;
  memset(&agc_cmd_param, 0, sizeof(rfwcdma_mdsp_agc_cmd_param_type));

  /* Populate MDSP paramters */
  set_freq_offset_param.set_data_cmd = RFWCDMA_MDSP_SET_FREQ_ADJUST;
  set_freq_offset_param.rflm_handle = rflm_handle;
  set_freq_offset_param.data.set_freq_adjust.band = band;
  set_freq_offset_param.data.set_freq_adjust.rx_chan = rx_chan ;
  set_freq_offset_param.data.set_freq_adjust.tx_freq_error = tx_freq_error;
  
  ret_val &= rfwcdma_mdsp_async_set_data(&set_freq_offset_param);

  /* Send dummy RxAGC cmd to FW */
  ret_val &= rfwcdma_mdsp_sync_send_cmd( RFWCDMA_MDSP_RXAGC_CMD_ID, &agc_cmd_param );

  return ret_val;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*!
  @brief
  Perform script based or immediate device writes for ASM, PA, QPOET and WTR

  @details
  based on the device information passed this routine would create scripts for ASM, PA, QPOET and WTR.

  @param device
  Device for which device scripts need to be created or written immediately

  @param band
  WCDMA operational UL band on the device

  @param channels
  List of UL channels for the tune
 
  @param txlm_handle
  TxLM handle for the device of operation

  @param tx_device_buffer
  RF buffer array passed my the caller for populating the device scripts.

  @param execution_type
  Execution type - Immediate or script based execution

  @param wtr_tune_execution_time
  WTR tune script execution time 

  @return api_status
  API status indicating success or failure
*/
boolean rfwcdma_core_txctl_config_tune_device( rfm_device_enum_type device,
                                               rfcom_wcdma_band_type band,
                                               uint16 channels[RFCOM_MAX_CARRIERS],
                                               uint32 txlm_handle,
                                               rf_buffer_intf *tx_device_buffer,
                                               rf_device_execution_type execution_type,
                                               uint32* wtr_tune_execution_time )
{

  rfdevice_coupler_tech_cfg_type coupler_tech_cfg;
  uint32 tx_freq = 0; 
  rfc_wcdma_apt_supported_type apt_config = { RFCOM_BAND_IMT, RFC_WCDMA_APT_NOT_SUPPORTED, 0 };
  rfdevice_wcdma_tx_tune_param_type tx_params;
  rfdevice_dpd_enum_type dpd_state = RFDEVICE_DPD_DISABLED;
  boolean api_status = TRUE;
  rfcommon_atuner_cl_ctrl_info_type cl_info;
  rfdevice_meas_script_data_type tune_script_data;
  rfcom_multi_carrier_hspa_id_type carr_id;
  rfcommon_atuner_config_req_type atuner_cfg_req;
  rfcom_band_type_u band_u;

  memset(&atuner_cfg_req, 0, sizeof(rfcommon_atuner_config_req_type)); 

  /* This API handles only single carrier Tx tune */
  carr_id = RFCOM_SINGLE_CARRIER;
  tx_freq = rfwcdma_core_get_freq_from_ul_arfcn(channels[carr_id]);

  /* Update WCDMA Tx band information */
  tx_band.wcdma_band = band;
  band_u.wcdma_band = band;

  atuner_cfg_req.rfm_device = device;
  atuner_cfg_req.rx_tx = RFCMN_ATUNER_TX_DEVICE;
  atuner_cfg_req.band = band_u;
  atuner_cfg_req.mode = RFCOM_WCDMA_MODE;
  atuner_cfg_req.chan_num = channels[carr_id];

  atuner_cfg_req.carrier_id = rfwcdma_core_map_carr_mask_to_atuner_id(rfwcdma_data_get_carrier_mask(device));

  atuner_cfg_req.freq = rfwcdma_core_get_freq_from_uarfcn_no_limit(band_u.wcdma_band,channels[carr_id],RFWCDMA_CORE_UARFCN_UL);


  RF_MSG_1(RF_ERROR, "rfwcdma_core_txctl_config_tune_device called on device %d", device);

  /* Init GPIO/GRFC for current WCDMA band on tx chain */
  (void)rfc_wcdma_tx_band_config(device, band);

   /* Prepare for Tx PLL tune */
  /* Get DPD state */
  if (rfwcdma_core_xpt_is_enabled(band))
  {
    dpd_state = RFDEVICE_DPD_ENABLED;
    RF_MSG_1(RF_MED, "rfwcdma_mc_enable_tx DPD enabled", 0);
  }
  else
  {
    dpd_state = RFDEVICE_DPD_DISABLED;
  }

  RF_MSG_3(RF_HIGH, "Tune Tx chan:%d, band:%d, type:%d", channels[carr_id], band, execution_type);

  /***************** 1. WTR device config  ***************************/

    /* Set LUT table vs. PA Range mapping */
  api_status &= rfdevice_wcdma_tx_cmd_dispatch(device,
                                               band,
                                               RFDEVICE_SET_LUT_PA_RANGE_MAP,
                                               &rfnv_wcdma_tbl_ptr[band]->pa_static.pa_range_map);


  if (execution_type == RFDEVICE_EXECUTE_IMMEDIATE)
    /* Tx init and Tx enable is only needed in immeidate execute case. In WXE
    based Tx tune, registers are preloaded to WXE */
  {
  /*! If same RF device, device library should take care that it
                  shouldn't init it again esp when in RX mode */
  api_status &= rfdevice_wcdma_tx_init(device,
                                       RFCOM_WCDMA_MODE,
                                       band,
                                       execution_type,
                                       tx_device_buffer);

  /* Enable transceiver's Tx path */
  api_status &= rfdevice_wcdma_tx_enable(device,
                                         band,
                                         execution_type,
                                         tx_device_buffer);
  }

  /* Tune Tx PLL */
  /* Always use single carrier in config state */
  tx_params.band       = band;
  tx_params.bw         = RFDEV_WCDMA_BW_1X;
  tx_params.dpd_state  = dpd_state;
  tx_params.channel[0] = channels[carr_id];
  tx_params.ul_dl = RFWCDMA_CORE_UARFCN_UL;

   if (execution_type == RFDEVICE_EXECUTE_IMMEDIATE)
   {
    api_status &= rfdevice_wcdma_tx_set_channel(device, RFCOM_WCDMA_MODE,
                                                tx_params.band, tx_params,
                                                execution_type, tx_device_buffer);
   }
   else if (execution_type == RFDEVICE_CREATE_SCRIPT)
   {

    /* Use new API for Wxified WTR Tx tune script */

    /* Preload WXE immediately */
    tune_script_data.preload_action = RFDEVICE_MEAS_EXECUTE_IMMEDIATE;
    tune_script_data.preload_buf = NULL;

    /* Get trigger script in the buffer */
    tune_script_data.meas_action = RFDEVICE_MEAS_CREATE_SCRIPT;
    tune_script_data.meas_buf = tx_device_buffer;

    /* Todo: what buffer mask to use?*/
    tune_script_data.buffer_mask = 0;                 

    api_status &= rfdevice_wcdma_tx_tune_to_chan(device, RFCOM_WCDMA_MODE,tx_params.band,tx_params,
                                                 &tune_script_data);
   }

   #ifdef FEATURE_RF_POWER_REPORTING
   /* associate handle with device */
   rflm_dm_handle_meta_s handle_meta;
   rflm_dm_get_handle_info(  txlm_handle , &handle_meta);
   handle_meta.devide_id = device;
   handle_meta.device_id = device;
   rflm_dm_set_handle_info( txlm_handle , &handle_meta );
   /* log channel number */
   rf_power_reporting_update_channel( device, tx_params.channel[0]  );
   #endif

  /* Extract Tx tune script execution time in usx10 from the Tx tune script */
  (void)rf_buffer_get_script_execution_time(tx_device_buffer,
                                            RF_BUFFER_RFFE,
                                            wtr_tune_execution_time);

  /*HDET enable*/
  (void)rfdevice_hdet_wcdma_init(device,
                                 tx_device_buffer,
                                 execution_type,
                                 band,
                                 tx_freq);

  /* Do HDET temp comp when enabling Tx*/
  (void)rfdevice_hdet_wcdma_measure_temp_comp(device,
                                              band,
                                              execution_type,
                                              tx_device_buffer);

  /* Apply PDET correction code */
  api_status &= rfdevice_wcdma_tx_cmd_dispatch(device, band,
                                              RFDEVICE_WCDMA_SET_PDET_CORRECTION_CODE, 0);

  /* HDET autocalibration */
  (void)rfdevice_wcdma_tx_cmd_dispatch(device, band, RFDEVICE_DO_HDET_AUTOCAL, 0);

  /***************** 2. PA device config  ***************************/
  api_status &= rfdevice_wcdma_pa_init(device,
                                       band,
                                       NULL,
                                       tx_device_buffer,
                                       execution_type);

  /* Wakeup PA for specific WCDMA band */
  api_status &= rfdevice_wcdma_pa_wakeup(device,
                                         band,
                                         tx_device_buffer,
                                         execution_type,
                                         RFCOM_INVALID_TIMING_OFFSET);

  /* Configure the PA for specific WCDMA band */
  api_status &= rfdevice_wcdma_pa_config(device,
                                         band,
                                         tx_device_buffer,
                                         execution_type,
                                         RFCOM_INVALID_TIMING_OFFSET,
                                         RFDEVICE_PA_BW_5MHZ);

  /***************** 3. ASM/Coupler device config  ***************************/

  /* Wakeup TX ASM for specific WCDMA band */
  api_status &= rfdevice_wcdma_asm_wakeup_tx(device,
                                             band,
                                             tx_device_buffer,
                                             execution_type,
                                             RFCOM_INVALID_TIMING_OFFSET);

  /* Configure the ASM for specific WCDMA band */
  api_status &= rfdevice_wcdma_asm_enable_tx(device,
                                             band,
                                             tx_device_buffer,
                                             execution_type,
                                             RFCOM_INVALID_TIMING_OFFSET);

  coupler_tech_cfg.mode = RFM_IMT_MODE;
  coupler_tech_cfg.band.wcdma_band = band;
  coupler_tech_cfg.port = 0;
  coupler_tech_cfg.direction = RFDEVICE_COUPLER_DIRECTION_FWD;
  coupler_tech_cfg.gain = RFDEVICE_COUPLER_GAIN_DEFAULT;
  coupler_tech_cfg.output_type = RFDEVICE_COUPLER_OUTPUT_TYPE_DEFAULT;

  api_status &= rfdevice_wcdma_coupler_config(device,
                                              coupler_tech_cfg,
                                              tx_device_buffer,
                                              execution_type,
                                              RFCOM_INVALID_TIMING_OFFSET);


  /***************** 4. PAPM device config  ***************************/

  /* Enable QPOET. Passing NULL for the script buffer instructs */
  /* the PA class to perform the operation directly. */
  /* QPOET enable should be immediate for settling time issue. It needs to be implemented for Votings.*/

  void **qpoet_obj = NULL;
  qpoet_obj = rfc_wcdma_get_device( device,
                                    RFC_CONFIG_TX,
                                    band,
                                    RFDEVICE_PAPM );

  boolean skip_bias = FALSE;
  
  if(qpoet_obj == NULL)
  {
    RF_MSG_1(RF_ERROR, "[RFWCDMA]rfwcdma_core_txctl_config_tune_device : qpoet_obj is null on device %d", device);
  }
  else
  {
  skip_bias = rfdevice_papm_standby_delay_req(qpoet_obj[0]);
  }
  
  RF_MSG_1(RF_HIGH, "[RFWCDMA]rfwcdma_core_txctl_config_tune_device: papm_enable() skip_bias %d", skip_bias);  

  if (!rfdevice_wcdma_papm_enable(device, band, NULL))
  {
    RF_MSG_2(RF_ERROR, "[RFWCDMA]Power tracker enable failed on device %d, band %d", device, band);
  }
  else
  {
    /* If power tracker is enabled, get the APT config and configure it to APT mode
    by default, this is because QPOET has 20us settling time when brought from standby
    to APT mode so need to set to a default bias earlier in time before AGC is enabled*/
    apt_config.band = band;
    rfc_wcdma_command_dispatch(device,
    RFC_WCDMA_GET_APT_CONFIG,
    (void *)(&apt_config));

    /* Skip rfdevice_wcdma_papm_set_mode_bias() only if device is QET5102 */
    if(skip_bias == FALSE)
    {
    RF_MSG_1(RF_HIGH, "[RFWCDMA]rfwcdma_core_txctl_config_tune_device: skip_bias %d", skip_bias);
	
    if (!rfdevice_wcdma_papm_set_mode_bias(device,
                                           band,
                                           RFDEVICE_APT_MODE,
                                           apt_config.default_smps_val,
                                           tx_device_buffer,
                                           RFDEVICE_EXECUTE_IMMEDIATE) 
        )
    {
      RF_MSG_3(RF_ERROR, "Failed to set default APT bias for power tracker on device %d, band %d, bias:%d",
      device, band, apt_config.default_smps_val);
    }
  }
  }

  /***************** 5. Tuner device config  ***************************/

  /*Program Tuner for Tx mode*/
  /* Null pointer check */
  if (band < RFCOM_NUM_WCDMA_BANDS)
  {
    if (rfnv_wcdma_tbl_ptr[band] == NULL)
    {
      RF_MSG_1(RF_ERROR,
      "rfwcdma_core_txctl_config_tune_device: NULL NV tbl ptr for band %d",
      band);
    }
    else
    {
      cl_info.cl_nv_ctrl_info = &rfnv_wcdma_tbl_ptr[band]->ant_tuner_cl_ctrl_info;
      cl_info.lm_handle = txlm_handle;

      (void)rfwcdma_core_antenna_tuner_program_tx_mode(&atuner_cfg_req,
      execution_type, 
      tx_device_buffer,
      &(cl_info));
    }
  }
  else
  {
    RF_MSG_1(RF_ERROR,
    "rfwcdma_mc_enable_tx: Not a valid band %d",
    band);
  }

  return api_status;
}

/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function creates an NPA client one-time only.  It is called during WCDMA init

  @details
*/
void rfwcdma_core_create_npa_client(void)
{
  if( rfwcdma_core_npa_handle_initialised == FALSE )
  {
    /* Create NPA handle only once*/
    rfwcdma_core_npa_handle_q6clk = npa_create_sync_client( "/clk/cpu", "RFLM_W_TX", NPA_CLIENT_REQUIRED );
    
    /* Check the Handle status */
    if( rfwcdma_core_npa_handle_q6clk != NULL )
    {
      /* Flag the handle initialized as successful as q6clk handle is good */
      rfwcdma_core_npa_handle_initialised = TRUE;
    }
  }
}
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function will generate GRFC script for all signals in this Tx Device
 
  @param device
  Tx RFM Device for which GRFC script needs to generated
 
  @param band
  RFCOM WCDMA Band for which GRGC script needs to be generated
 
  @param grfc_buf_ptr
  GRFC SCript Buffer Pointer has the list of GRFC writes
 
  @return status
  Status of the script generation api
*/
boolean rfwcdma_core_txctl_generate_grfc_script
(
  rfm_device_enum_type device,
  rfcom_wcdma_band_type band,
  rf_buffer_intf *grfc_buf_ptr
)
{
  boolean api_status = FALSE;
  rfc_sig_cfg_type *rfc_tx_grfc_tbl_ptr = NULL;

  if( (device >= RFM_MAX_DEVICES) || (band >= RFCOM_NUM_WCDMA_BANDS) )
  {
    RF_MSG_2(RF_ERROR, "rfwcdma_core_txctl_generate_grfc_script: Invalid device %d or band %d", 
             device, band);
    return api_status;
  }

  rfc_tx_grfc_tbl_ptr =  
    (rfc_sig_cfg_type *)rfc_wcdma_get_tx_band_config( device,
                                                      band, 
                                                      RFC_GRFC_DATA );
  
   /*==========================================================================
    For each valid pointer, Look up GRFC Engine Number 
           and populate the IRAT GRFC buffer with correct logic.
   ==========================================================================*/

  
  if( rfc_tx_grfc_tbl_ptr != NULL )
  { 
    api_status = rfc_common_generate_grfc_script( rfc_tx_grfc_tbl_ptr, grfc_buf_ptr ,
                                                  0 /* delay */ );
  }

  else
  {
    RF_MSG_2(RF_HIGH, "rfwcdma_core_txctl_generate_grfc_script: No GRFCs for device %d, band %d", 
             device, band);
    api_status = TRUE;
  }

  return api_status;
  
} /*rfwcdma_core_txctl_generate_grfc_script*/
/*----------------------------------------------------------------------------*/
/*!
  @brief
  This function set the VSWR selftest status
  @details
    This function set the VSWR selftest status
 
  @param rflm_handle
  RFLM handle
 
  @param disable
  Boolean to ind selftest running: 0= not running. 1 = running
 
  @return
  TRUE if successful, else FALSE
 
*/
boolean rfwcdma_core_tx_set_common_selftest_ind
(
  uint8 selftest_modeset_flag
)
{
  rfwcdma_mdsp_set_data_param_type vswr_selftest_ind = { 0 };
  rfm_device_enum_type device;
  rflm_dm_handle_id_t rflm_handle;

  device = rfwcdma_data_get_tx_device();
  if(device >= RFM_WCDMA_MAX_DEVICE)
  {
    RF_MSG(RF_HIGH,"rfwcdma_core_tx_set_common_selftest_ind: No device in Tx status");
    return FALSE;
  }
  
  rflm_handle = rfwcdma_data_get_rflm_buf_idx(device);

  RF_MSG_3(RF_HIGH,"rfwcdma_core_tx_set_common_selftest_ind: device %d rflm_handle %d selftest_modeset_flag %d",
                    device, rflm_handle, selftest_modeset_flag);
  
  vswr_selftest_ind.set_data_cmd = RFWCDMA_MDSP_SET_SELFTEST_IND;
  vswr_selftest_ind.rflm_handle = rflm_handle;

  if(0 == selftest_modeset_flag)
  {
    vswr_selftest_ind.data.gen_b = FALSE;
  }
  else
  {
    vswr_selftest_ind.data.gen_b = TRUE;
  }
  
  return(rfwcdma_mdsp_async_set_data(&vswr_selftest_ind));
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*!
  @brief
  Query TxAGC per carrier for MAAT
 
  @details
  Returns the WCDMA TxAGC based on the parameter passed. Returns FALSE if 
 
  @param tx_maat_info_ptr
  Pointer to the variable to populate the MAAT Tx AGC Data
 
  @return
  Indicates the validity of MAAT TxAGC read. If no active Tx Device found, return FALSE
*/
boolean
rfwcdma_core_tx_get_maat_agc_data
(
  rfa_wcdma_maat_tx_info_type_s *tx_maat_info_ptr
)
{
  boolean                             ret_val = TRUE;
  boolean                             device_found = FALSE;
  rfwcdma_mdsp_get_data_param_type    mdsp_param = {0};
  rfwcdma_core_txlin_record_type     *curr_tx_lin;
  const rfwcdma_data_status_type      *tx_state_data = NULL;
  rfm_device_enum_type                tx_device= RFM_INVALID_DEVICE;
  rfm_wcdma_carrier_type              carrier;

 /* Param validation */
  if ( tx_maat_info_ptr == NULL )
  {
    RF_MSG(RF_ERROR, "[RFWCDMA][MC]rf_wcdma_tx_get_maat_agc_data: NULL param" );
    return FALSE;
  } /* if ( tx_agc_val == NULL ) */

  tx_device = rfwcdma_data_get_tx_device();
  tx_state_data = rfwcdma_get_radio_status(tx_device);
    if( tx_state_data != NULL )
    {
      /* Get current Lin data */
      curr_tx_lin = rfwcdma_core_get_current_txlin_data( );

      /* NULL pointer check */
      if ( curr_tx_lin != NULL )
      {

        /* Get TxAGC parameter */

        mdsp_param.get_data_cmd = RFWCDMA_MDSP_GET_TXAGC_PARAM;
        mdsp_param.rflm_handle = rfwcdma_data_get_rflm_buf_idx(tx_device);
        ret_val &= rfwcdma_mdsp_async_get_tx_data(&mdsp_param);

        if ( ret_val == TRUE )
        {
          if ( NULL != curr_tx_lin->current_lin_tbl[mdsp_param.data.get_txagc_param.pa_state].table &&
                tx_state_data->rf_state == RFWCDMA_STATE_TX)
          {
            device_found = TRUE;
          } /* if ( NULL != curr_tx_lin->current_lin_tbl_comp[fed_tx_agc_data.pa_state].lin_data.table ) */
          else
          {
            RF_MSG_2( RF_ERROR, "[RFWCDMA][MC]rf_wcdma_tx_get_maat_agc_data: RFMDevice 0x%x TX State %d - NULL Sweep table shadow ptr", 
                                  tx_device, rfwcdma_data_get_rf_state(tx_device));
            return FALSE;
          }/* if !( NULL != curr_tx_lin->current_lin_tbl_comp[fed_tx_agc_data.pa_state].lin_data.table &&
                    TRUE == fed_tx_agc_data.tx_on_status ) */
        } /* if ( ret_val == TRUE ) */
        else
        {
          RF_MSG_1( RF_ERROR, "[RFWCDMA][MC]rf_wcdma_tx_get_maat_agc_data: RFMDevice 0x%x - MSDP failure", 
                                tx_device );

          return FALSE;
        } /* if ( ret_val == TRUE ) */
      }
      else
      {
          RF_MSG_1( RF_ERROR, "[RFWCDMA][MC]rf_wcdma_tx_get_maat_agc_data: RFMDevice 0x%x - NULL TxLin data", 
                              tx_device );
        return FALSE;
      } /* if ( curr_tx_lin == NULL ) */
    }



  if( device_found == TRUE )
  {
    mdsp_param.get_data_cmd = RFWCDMA_MDSP_GET_TXAGC_VAL;
    ret_val &= rfwcdma_mdsp_async_get_tx_data(&mdsp_param);
    memset(tx_maat_info_ptr, 0, sizeof(rfa_wcdma_maat_tx_info_type_s));
    if ( ret_val == TRUE )
    {
      for ( carrier = RFM_WCDMA_CARRIER_0; carrier < RFM_WCDMA_CARRIER_MAX; carrier++ )
      {
        if( (tx_state_data->carrier_mask) & (1<<carrier) )
        {
          tx_maat_info_ptr->carrier[carrier].txpwr_db10 = (int16)(mdsp_param.data.gen_w);
          tx_maat_info_ptr->carrier[carrier].tx_channel = tx_state_data->curr_chan[carrier];
          tx_maat_info_ptr->carrier[carrier].rfm_path.rfm_device =tx_device;
          tx_maat_info_ptr->carrier[carrier].carrier_id = carrier;
          tx_maat_info_ptr->active_carrier_mask |= 1 << carrier;

          RF_MSG_5(RF_HIGH, "[RFWCDMA][MC]rf_wcdma_tx_get_maat_agc_data: MAAT update info: Channel %d, "
                          "RFMDevice 0x%x, Carrier_id %d, TxAGC %d, Active Carrier Mask %d",
                          tx_maat_info_ptr->carrier[carrier].tx_channel,
                          (tx_maat_info_ptr->carrier[carrier].rfm_path.rfm_device),
                          tx_maat_info_ptr->carrier[carrier].carrier_id,
                          tx_maat_info_ptr->carrier[carrier].txpwr_db10,
                          tx_maat_info_ptr->active_carrier_mask );
      }
    }
    }
    else
    {
          RF_MSG( RF_ERROR, "[RFWCDMA][MC]rf_wcdma_tx_get_maat_agc_data: Error in getting tx data");
            return FALSE;
    }
  }
  else
  {
    ret_val = FALSE;
  }

  return ret_val;
} /* rf_wcdma_tx_get_maat_agc_data */


